/***********************************************************

Structure of archive block (low order byte first):
-----preheader
 1	basic header size
		= 25 + strlen(filename) (= 0 if end of archive)
 1	basic header algebraic sum (mod 256)
-----basic header
 5	method ("-sn0-" = stored, "-sn5-" = compressed)
 4	compressed size (including extended headers)
 4	original size
 4	not used
 1	0x20
 1	0x01
 1	filename length (x)
 x	filename
 2	original file's CRC
 1	0x20
 2	first extended header size (0 if none)
-----first extended header, etc.
-----compressed file

***********************************************************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <limits.h>

#include "SFC.H"

DWORD	cNUX::TotalSize				=	0;
DWORD	cNUX::FileSize				=	0;
DWORD	cNUX::FileCount				=	0;
DWORD	cNUX::FileRate				=	0;
DWORD	cNUX::ProcessRate			=	0;
DWORD	cNUX::ProcessSize			=	0;
char	cNUX::CurrentFile[128];
char	cNUX::ArcName[128];
BOOL	cNUX::s_bIsMerge			=	FALSE;
BOOL	cNUX::s_bIsMergeFolder		=	FALSE;

void	(*cNUX::ProcessExtract)()	=	NULL;
void	(*cNUX::ProcessPress)()		=	NULL;
BOOL	cNUX::bSTOP					=	FALSE;
BOOL	bStop						=	FALSE;

DWORD	inFileSize					=	0;
DWORD	ISFOLDER					=	0xfefefefe;
DWORD	appendPos					=	0;					//	Append 할때의 데이터 위치(화일수와 헤더의 위치가 들어간 위치다.)
DWORD	ISMERGEFILE					=	0xefefefef;			//	Merge화일이다.
DWORD	ISPRESSFILE					=	0xfefefefe;			//	Press화일이다.
DWORD	ISMERGEFOLDER				=	0xfefe1111;			//	폴더를 Merge한 화일이다.
BOOL	bAPPEND						=	FALSE;
const	DWORD	BLOCKSIZE			=	1024*1024*4;

char	tempArc[512]				=	{0,};
char	processFolder[512];


#define BITBUFSIZ	(CHAR_BIT * sizeof bitbuf)
#define INIT_CRC	0							/* CCITT: 0xFFFF */
#define DICBIT		13							/* 12(-sn4-) or 13(-sn5-) */
#define DICSIZ		(1U << DICBIT)
#define MATCHBIT	8							/* bits for MAXMATCH - THRESHOLD */
#define MAXMATCH	256							/* formerly F (not more than UCHAR_MAX + 1) */
#define THRESHOLD	3							/* choose optimal value */
#define PERC_FLAG	0x8000U
#define NC			(UCHAR_MAX + MAXMATCH + 2 - THRESHOLD)
												/* alphabet = {0, 1, 2, ..., NC - 1} */
#define CBIT		9							/* $\lfloor \log_2 NC \rfloor + 1$ */
#define CODE_BIT	16							/* codeword length */

#define namelen		header[19]
#define filename	((char *)&header[20])

FILE	*arcFile	=	NULL, *inFile=	NULL, *outFile=	NULL;
WORD	crc, bitbuf;

void	make_crctable(void);
void	fillbuf(int n);
WORD	getbits(int n);
/* void putbit(int bit); */
void	putbits(int n, DWORD x);
int		fread_crc(BYTE *p, int n, FILE *f);
void	fwrite_crc(BYTE *p, int n, FILE *f);
void	init_getbits(void);
void	init_putbits(void);

BOOL	encode(void);
void	decode_start(void);
void	decode(DWORD count, BYTE text[]);

void	huf_encode_start(void);
void	huf_decode_start(void);
WORD	decode_c(void);
WORD	decode_p(void);
void	output(WORD c, WORD p);
void	huf_encode_end(void);
void	make_table(int nchar, BYTE bitlen[],int tablebits, WORD table[]);
int		make_tree(int nparm, WORD freqparm[],BYTE lenparm[], WORD codeparm[]);

int		unpackable;						// global, set in io.c
ULONG	CompressSize, OriginalSize;		// global

static	BYTE	buffer[DICSIZ];
static	BYTE	header[1024];
static	BYTE	headersize, headersum;
static	WORD	file_crc;

static	char	folderName[1024];

static	BOOL	stop()
{	if (bStop)
	{	if (outFile) fclose(outFile);
		if (inFile ) fclose(inFile);
		if (arcFile) fclose(arcFile);
		outFile	=	NULL;
		inFile	=	NULL;
		arcFile	=	NULL;

		remove(tempArc);
		return TRUE;
	}

	return FALSE;
}

static WORD ratio(ULONG a, ULONG b)  /* [(1000a + [b/2]) / b] */
{	int i;

	for (i = 0; i < 3; i++)	if (a <= ULONG_MAX / 10) a *= 10;  else b /= 10;
	if ((ULONG)(a + (b >> 1)) < a) {  a >>= 1;  b >>= 1;  }
	if (b == 0) return 0;

	return (WORD)((a + (b >> 1)) / b);
}

static void put_to_header(int i, int n, ULONG x)
{	while (--n >= 0) 
	{	header[i++] = (BYTE)((WORD)x & 0xFF);
		x >>= 8;
	}
}

static ULONG get_from_header(int i, int n)
{	ULONG s;

	s = 0;
	while (--n >= 0) s = (s << 8) + header[i + n];  /* little endian */
	return s;
}

static WORD calc_headersum(void)
{	int i;
	WORD s;

	s = 0;
	for (i = 0; i < headersize; i++) s += header[i];
	return s & 0xFF;
}

static char read_header(void)
{	headersize = (BYTE) fgetc(arcFile);

	if (headersize == 0) return 0;					//	end of archive
	if (headersize == 1)							//	폴더를 생성해야 한다.
	{	unsigned int isfolder;
		BYTE	size;
		
		fread(&isfolder	,4		,1,arcFile);
		if (isfolder!=ISFOLDER) return 0;

		fread(&size		,1		,1,arcFile);
		fread(folderName,size	,1,arcFile);
		folderName[size]	=	NULL;

		return 2;
	}

	headersum  = (BYTE) fgetc(arcFile);
	fread_crc(header, headersize, arcFile);			//	CRC not used
	if (calc_headersum() != headersum) return -1;
	CompressSize = get_from_header(5, 4);
	OriginalSize = get_from_header(9, 4);
	file_crc = (WORD)get_from_header(headersize - 5, 2);
	filename[namelen] = '\0';

	return 1;  // success
}

static void write_header(void)
{	fputc(headersize, outFile);
	/* We've destroyed file_crc by null-terminating filename. */
	put_to_header(headersize - 5, 2, (ULONG)file_crc);
	fputc(calc_headersum(), outFile);
	fwrite_crc(header, headersize, outFile);  /* CRC not used */
}

static void store(void)
{	WORD n;

	OriginalSize	=	0;
	crc				=	INIT_CRC;
	int				l_iSleepCount = 0;

	while ((n = fread((char *)buffer, DICSIZ,1,inFile)) != 0) 
	{	fwrite_crc(buffer, n, outFile);  
		OriginalSize += n;

		cNUX::FileRate	=	ftell(inFile)*100/inFileSize;
		if (cNUX::ProcessPress) cNUX::ProcessPress();
		l_iSleepCount++;
		if(l_iSleepCount%10000==0)	Sleep(1);		//	add by jihyun
	}

	CompressSize = OriginalSize;
}

static void skip(void)
{	if (arcFile)	fseek(arcFile, CompressSize, SEEK_CUR);
}

/***********************************************************
	huf.c -- static Huffman
***********************************************************/

#define NP (DICBIT + 1)
#define NT (CODE_BIT + 3)
#define PBIT 4  /* smallest integer such that (1U << PBIT) > NP */
#define TBIT 5  /* smallest integer such that (1U << TBIT) > NT */
#if NT > NP
	#define NPT NT
#else
	#define NPT NP
#endif

WORD left[2 * NC - 1], right[2 * NC - 1];

static BYTE *buf, c_len[NC], pt_len[NPT];
static WORD bufsiz = 0, blocksize;
static WORD	c_freq[2 * NC - 1], c_table[4096], c_code[NC],p_freq[2 * NP - 1], pt_table[256], pt_code[NPT],t_freq[2 * NT - 1];

/***** encoding *****/

static void count_t_freq(void)
{	int i, k, n, count;

	for (i = 0; i < NT; i++) t_freq[i] = 0;
	n = NC;
	while (n > 0 && c_len[n - 1] == 0) n--;
	i = 0;
	while (i < n) 
	{	k = c_len[i++];
		if (k == 0) 
		{	count = 1;
			while (i < n && c_len[i] == 0) {  i++;  count++;  }
			if (count <= 2) t_freq[0] += count;
			else if (count <= 18) t_freq[1]++;
			else if (count == 19) {  t_freq[0]++;  t_freq[1]++;  }
			else t_freq[2]++;
		} else t_freq[k + 2]++;
	}
}

static void write_pt_len(int n, int nbit, int i_special)
{	int i, k;

	while (n > 0 && pt_len[n - 1] == 0) n--;
	putbits(nbit, n);
	i = 0;
	while (i < n) 
	{	k = pt_len[i++];
		if (k <= 6) putbits(3, k);
		else putbits(k - 3, (1U << (k - 3)) - 2);
		if (i == i_special) 
		{	while (i < 6 && pt_len[i] == 0) i++;
			putbits(2, (i - 3) & 3);
		}
	}
}

static void write_c_len(void)
{	int i, k, n, count;

	n = NC;

	while (n > 0 && c_len[n - 1] == 0) n--;

	putbits(CBIT, n);
	i = 0;

	while (i < n) 
	{	k = c_len[i++];
		if (k == 0) 
		{	count = 1;
			while (i < n && c_len[i] == 0) {  i++;  count++;  }
			if (count <= 2) 
			{	for (k = 0; k < count; k++)
					putbits(pt_len[0], pt_code[0]);
			} else if (count <= 18) 
			{	putbits(pt_len[1], pt_code[1]);
				putbits(4, count - 3);
			} else if (count == 19) 
			{	putbits(pt_len[0], pt_code[0]);
				putbits(pt_len[1], pt_code[1]);
				putbits(4, 15);
			} else 
			{	putbits(pt_len[2], pt_code[2]);
				putbits(CBIT, count - 20);
			}
		} else putbits(pt_len[k + 2], pt_code[k + 2]);
	}
}

static void encode_c(int c)
{	putbits(c_len[c], c_code[c]);
}

static void encode_p(WORD p)
{	WORD c, q;

	c = 0;  q = p;  while (q) {  q >>= 1;  c++;  }
	putbits(pt_len[c], pt_code[c]);
	if (c > 1) putbits(c - 1, p & (0xFFFFU >> (17 - c)));
}

static void send_block(void)
{	WORD i, k, flags, root, pos, size;

	root = make_tree(NC, c_freq, c_len, c_code);
	size = c_freq[root];  putbits(16, size);
	if (root >= NC) 
	{	count_t_freq();
		root = make_tree(NT, t_freq, pt_len, pt_code);
		if (root >= NT) 
		{	write_pt_len(NT, TBIT, 3);
		} else 
		{	putbits(TBIT, 0);  putbits(TBIT, root);
		}
		write_c_len();
	} else 
	{   putbits(TBIT, 0);  putbits(TBIT, 0);
		putbits(CBIT, 0);  putbits(CBIT, root);
	}

	root = make_tree(NP, p_freq, pt_len, pt_code);
	if (root >= NP) 
	{	write_pt_len(NP, PBIT, -1);
	} else 
	{	putbits(PBIT, 0);  putbits(PBIT, root);
	}
	pos = 0;
	for (i = 0; i < size; i++) 
	{	if (i % CHAR_BIT == 0) flags = buf[pos++];  else flags <<= 1;
		if (flags & (1U << (CHAR_BIT - 1))) 
		{	encode_c(buf[pos++] + (1U << CHAR_BIT));
			k = buf[pos++] << CHAR_BIT;  k += buf[pos++];
			encode_p(k);
		} else encode_c(buf[pos++]);
		if (unpackable) return;
	}
	for (i = 0; i < NC; i++) c_freq[i] = 0;
	for (i = 0; i < NP; i++) p_freq[i] = 0;
}

static WORD output_pos, output_mask;

void output(WORD c, WORD p)
{	static WORD cpos;

	if ((output_mask >>= 1) == 0) 
	{	output_mask = 1U << (CHAR_BIT - 1);
		if (output_pos >= bufsiz - 3 * CHAR_BIT) 
		{	send_block();
			if (unpackable) return;
			output_pos = 0;
		}
		cpos = output_pos++;  buf[cpos] = 0;
	}

	buf[output_pos++] = (BYTE) c;  c_freq[c]++;

	if (c >= (1U << CHAR_BIT)) 
	{	buf[cpos] |= output_mask;
		buf[output_pos++] = (BYTE)(p >> CHAR_BIT);
		buf[output_pos++] = (BYTE) p;
		c = 0;
		while (p) {  p >>= 1;  c++;  }
		p_freq[c]++;
	}
}

void huf_encode_start(void)
{	int i;

	if (bufsiz == 0) 
	{	bufsiz = 16 * 1024U;
		while ((buf = (BYTE *)malloc(bufsiz)) == NULL) 
		{	bufsiz = (bufsiz / 10U) * 9U;
			if (bufsiz < 4 * 1024U) printf("Decode Error!!","Out of memory.");
		}
	}

	buf[0] = 0;
	for (i = 0; i < NC; i++) c_freq[i] = 0;
	for (i = 0; i < NP; i++) p_freq[i] = 0;
	output_pos = output_mask = 0;
	init_putbits();
}

void huf_encode_end(void)
{	if (! unpackable) 
	{	send_block();
		putbits(CHAR_BIT - 1, 0);  /* flush remaining bits */
	}
}

/***** decoding *****/

static void read_pt_len(int nn, int nbit, int i_special)
{
	int i, c, n;
	WORD mask;

	n = getbits(nbit);
	if (n == 0) 
	{	c = getbits(nbit);
		for (i = 0; i < nn; i++) pt_len[i] = 0;
		for (i = 0; i < 256; i++) pt_table[i] = c;
	} else 
	{	i = 0;
		while (i < n) 
		{	c = bitbuf >> (BITBUFSIZ - 3);
			if (c == 7) 
			{	mask = 1U << (BITBUFSIZ - 1 - 3);
				while (mask & bitbuf) {  mask >>= 1;  c++;  }
			}
			fillbuf((c < 7) ? 3 : c - 3);
			pt_len[i++] = c;
			if (i == i_special) 
			{	c = getbits(2);
				while (--c >= 0) pt_len[i++] = 0;
			}
		}
		while (i < nn) pt_len[i++] = 0;
		make_table(nn, pt_len, 8, pt_table);
	}
}

static void read_c_len(void)
{	int i, c, n;
	WORD mask;

	n = getbits(CBIT);
	if (n == 0) 
	{	c = getbits(CBIT);
		for (i = 0; i < NC; i++) c_len[i] = 0;
		for (i = 0; i < 4096; i++) c_table[i] = c;
	} else 
	{	i = 0;
		while (i < n) 
		{	c = pt_table[bitbuf >> (BITBUFSIZ - 8)];
			if (c >= NT) 
			{	mask = 1U << (BITBUFSIZ - 1 - 8);
				do 
				{	if (bitbuf & mask) c = right[c];
					else               c = left [c];
					mask >>= 1;
				} while (c >= NT);
			}

			fillbuf(pt_len[c]);
			if (c <= 2) 
			{	if      (c == 0) c = 1;
				else if (c == 1) c = getbits(4) + 3;
				else             c = getbits(CBIT) + 20;
				while (--c >= 0) c_len[i++] = 0;
			} else c_len[i++] = c - 2;
		}
		while (i < NC) c_len[i++] = 0;
		make_table(NC, c_len, 12, c_table);
	}
}

WORD decode_c(void)
{	WORD j, mask;

	if (blocksize == 0) 
	{	blocksize = getbits(16);
		read_pt_len(NT, TBIT, 3);
		read_c_len();
		read_pt_len(NP, PBIT, -1);
	}

	blocksize--;
	j = c_table[bitbuf >> (BITBUFSIZ - 12)];
	if (j >= NC) 
	{	mask = 1U << (BITBUFSIZ - 1 - 12);
		do 
		{	if (bitbuf & mask) j = right[j];
			else               j = left [j];
			mask >>= 1;
		} while (j >= NC);
	}

	fillbuf(c_len[j]);
	return j;
}

WORD decode_p(void)
{	WORD j, mask;

	j = pt_table[bitbuf >> (BITBUFSIZ - 8)];

	if (j >= NP) 
	{	mask = 1U << (BITBUFSIZ - 1 - 8);
		do 
		{	if (bitbuf & mask) j = right[j];
			else               j = left [j];
			mask >>= 1;
		} while (j >= NP);
	}

	fillbuf(pt_len[j]);

	if (j != 0) j = (1U << (j - 1)) + getbits(j - 1);

	return j;
}

void huf_decode_start(void)
{	init_getbits();
	blocksize = 0;
}


/***********************************************************
	decode.c
***********************************************************/

static int remainByte;  /* remaining bytes to copy */

void decode_start(void)
{	huf_decode_start();
	remainByte = 0;
}

void decode(DWORD count, BYTE buffer[])
	/* The calling function must keep the number of
	   bytes to be processed.  This function decodes
	   either 'count' bytes or 'DICSIZ' bytes, whichever
	   is smaller, into the array 'buffer[]' of size
	   'DICSIZ' or more.
	   Call decode_start() once for each new file
	   before calling this function. */
{
	static	WORD i;
	WORD	r, c;

	r	=	0;

	while (--remainByte >= 0) 
	{	buffer[r] = buffer[i];
		i = (WORD)((i + 1) & (DICSIZ - 1));
		if (++r == count) return;
	}

	for ( ; ; ) 
	{	c = decode_c();
		if (c <= UCHAR_MAX) 
		{	buffer[r] = (BYTE)c;
			if (++r == count) return;
		} else 
		{	remainByte = c - (UCHAR_MAX + 1 - THRESHOLD);
			i = (WORD)((r - decode_p() - 1) & (DICSIZ - 1));
			while (--remainByte >= 0) 
			{	buffer[r] = buffer[i];
				i = (WORD)((i + 1) & (DICSIZ - 1));
				if (++r == count) return;
			}
		}
	}
}

/***********************************************************
	encode.c -- sliding dictionary with percolating update
***********************************************************/
#define PERCOLATE  1
#define NIL        0
#define MAX_HASH_VAL (3 * DICSIZ + (DICSIZ / 512 + 1) * UCHAR_MAX)

typedef short node;

static BYTE *text, *childcount;
static node pos, matchpos, avail,*position, *parent, *prev, *next = NULL;
static int remainder, matchlen;

#if MAXMATCH <= (UCHAR_MAX + 1)
	static BYTE *level;
#else
	static WORD *level;
#endif

static void allocate_memory(void)
{	if (next != NULL) return;

    text		= (BYTE *)malloc(DICSIZ * 2 + MAXMATCH);
	level		= (BYTE *)malloc((DICSIZ + UCHAR_MAX + 1) * sizeof(*level));
	childcount	= (BYTE *)malloc((DICSIZ + UCHAR_MAX + 1) * sizeof(*childcount));

	#if PERCOLATE
	  position	= (short *)malloc((DICSIZ + UCHAR_MAX + 1) * sizeof(*position));
	#else
	  position	= (short *)malloc(DICSIZ * sizeof(*position));
	#endif

	parent		= (short *)malloc(DICSIZ * 2 * sizeof(*parent));
	prev		= (short *)malloc(DICSIZ * 2 * sizeof(*prev));
	next		= (short *)malloc((MAX_HASH_VAL + 1) * sizeof(*next));

	if (next == NULL) printf("Decode Error!!","Out of memory.");
}

static void init_slide(void)
{	node i;

	for (i = DICSIZ; i <= DICSIZ + UCHAR_MAX; i++) 
	{	level[i] = 1;
		#if PERCOLATE
			position[i] = NIL;  /* sentinel */
		#endif
	}
	for (i = DICSIZ; i < DICSIZ * 2; i++) parent[i] = NIL;
	avail = 1;
	for (i = 1; i < DICSIZ - 1; i++) next[i] = i + 1;
	next[DICSIZ - 1] = NIL;
	for (i = DICSIZ * 2; i <= MAX_HASH_VAL; i++) next[i] = NIL;
}

#define HASH(p, c) ((p) + ((c) << (DICBIT - 9)) + DICSIZ * 2)

static node child(node q, BYTE c)
	/* q's child for character c (NIL if not found) */
{	node r;

	r = next[HASH(q, c)];
	parent[NIL] = q;  /* sentinel */
	while (parent[r] != q) r = next[r];

	return r;
}

static void makechild(node q, BYTE c, node r)
	/* Let r be q's child for character c. */
{	node h, t;

	h			= (short)HASH(q, c);
	t			= next[h];
	next[h]		= r;
	next[r]		= t;
	prev[t]		= r;
	prev[r]		= h;
	parent[r]	= q;
	childcount[q]++;
}

void split(node old)
{	node newnode, t;

	newnode				= avail;
	avail				= next[newnode];
	childcount[newnode] = 0;
	t					= prev[old];
	prev[newnode]		= t;
	next[t]				= newnode;
	t					= next[old];
	next[newnode]		= t;
	prev[t]				= newnode;
	parent[newnode]		= parent[old];
	level[newnode]		= matchlen;
	position[newnode]	= pos;

	makechild(newnode, text[matchpos + matchlen], old);
	makechild(newnode, text[pos + matchlen], pos);
}

static void insert_node(void)
{
	node q, r, j, t;
	BYTE c, *t1, *t2;

	if (matchlen >= 4) {
		matchlen--;
		r = (short)((matchpos + 1) | DICSIZ);
		while ((q = parent[r]) == NIL) r = next[r];
		while (level[q] >= matchlen) {
			r = q;  q = parent[q];
		}
		#if PERCOLATE
			t = q;
			while (position[t] < 0) {
				position[t] = pos;  t = parent[t];
			}
			if (t < DICSIZ) position[t] = pos | PERC_FLAG;
		#else
			t = q;
			while (t < DICSIZ) {
				position[t] = pos;  t = parent[t];
			}
		#endif
	} else {
		q = (short)(text[pos] + DICSIZ);
		c = text[pos + 1];
		if ((r = child(q, c)) == NIL) {
			makechild(q, c, pos);  matchlen = 1;
			return;
		}
		matchlen = 2;
	}
	for ( ; ; ) {
		if (r >= DICSIZ) {
			j = MAXMATCH;  matchpos = r;
		} else {
			j = level[r];
			matchpos = position[r] & ~PERC_FLAG;
		}
		if (matchpos >= pos) matchpos -= DICSIZ;
		t1 = &text[pos + matchlen];  t2 = &text[matchpos + matchlen];
		while (matchlen < j) {
			if (*t1 != *t2) {  split(r);  return;  }
			matchlen++;  t1++;  t2++;
		}
		if (matchlen >= MAXMATCH) break;
		position[r] = pos;
		q = r;
		if ((r = child(q, *t1)) == NIL) {
			makechild(q, *t1, pos);  return;
		}
		matchlen++;
	}
	t = prev[r];  prev[pos] = t;  next[t] = pos;
	t = next[r];  next[pos] = t;  prev[t] = pos;
	parent[pos] = q;  parent[r] = NIL;
	next[r] = pos;  /* special use of next[] */
}

static void delete_node(void)
{
	#if PERCOLATE
		node q, r, s, t, u;
	#else
		node r, s, t, u;
	#endif

	if (parent[pos] == NIL) return;
	r = prev[pos];  s = next[pos];
	next[r] = s;  prev[s] = r;
	r = parent[pos];  parent[pos] = NIL;
	if (r >= DICSIZ || --childcount[r] > 1) return;
	#if PERCOLATE
		t = position[r] & ~PERC_FLAG;
	#else
		t = position[r];
	#endif
	if (t >= pos) t -= DICSIZ;
	#if PERCOLATE
		s = t;  q = parent[r];
		while ((u = position[q]) & PERC_FLAG) 
		{	u &= ~PERC_FLAG;  if (u >= pos) u -= DICSIZ;
			if (u > s) s = u;
			position[q] = (short)(s | DICSIZ);
			q = parent[q];
		}
		if (q < DICSIZ) 
		{	if (u >= pos) u -= DICSIZ;
			if (u > s) s = u;
			position[q] = (short)(s | DICSIZ | PERC_FLAG);
		}
	#endif
	s = child(r, text[t + level[r]]);
	t = prev[s];  u = next[s];
	next[t] = u;  prev[u] = t;
	t = prev[r];  next[t] = s;  prev[s] = t;
	t = next[r];  prev[t] = s;  next[s] = t;
	parent[s] = parent[r];  parent[r] = NIL;
	next[r] = avail;  avail = r;
}

static BOOL get_next_match(void)
{	int n;

	remainder--;

	if (++pos == DICSIZ * 2) 
	{	memmove(&text[0], &text[DICSIZ], DICSIZ + MAXMATCH);
		n	=	fread_crc(&text[DICSIZ + MAXMATCH], DICSIZ, inFile);
		remainder += n;
		pos =	DICSIZ;

		cNUX::ProcessSize	=	cNUX::TotalSize + ftell(inFile);
		cNUX::FileRate		=	ratio(OriginalSize,inFileSize)/10;
		if (cNUX::ProcessPress) cNUX::ProcessPress();

		if (stop()) return FALSE;
	}

	delete_node();
	insert_node();

	return TRUE;
}

BOOL encode(void)
{	int lastmatchlen;
	node lastmatchpos;
	int l_iSleepCount = 0;

	allocate_memory();
	init_slide();
	huf_encode_start();
	remainder = fread_crc(&text[DICSIZ], DICSIZ + MAXMATCH, inFile);

	matchlen = 0;
	pos = DICSIZ;
	insert_node();

	if (matchlen > remainder) matchlen = remainder;

	while (remainder > 0 && ! unpackable) 
	{	lastmatchlen = matchlen;
		lastmatchpos = matchpos;
		if (!get_next_match()) return FALSE;

		if (matchlen > remainder)	matchlen = remainder;

		if (matchlen > lastmatchlen || lastmatchlen < THRESHOLD)	output(text[pos - 1], 0);
		else 
		{	output((WORD)(lastmatchlen + (UCHAR_MAX + 1 - THRESHOLD)),(WORD)((pos - lastmatchpos - 2) & (DICSIZ - 1)));
			while (--lastmatchlen > 0)
				if (!get_next_match()) return FALSE;
			if (matchlen > remainder) matchlen = remainder;
		}
		l_iSleepCount++;
		if(l_iSleepCount%10000==0)	Sleep(1);		//	add by jihyun
//		cNUX::FileRate	=	max(remainder,0)*100/size;
	}

	huf_encode_end();

	return TRUE;
}

#include <stdarg.h>

#define CRCPOLY  0xA001  /* ANSI CRC-16 */
                         /* CCITT: 0x8408 */
#define UPDATE_CRC(c)	crc = crctable[(crc ^ (c)) & 0xFF] ^ (crc >> CHAR_BIT)

static WORD		crctable[UCHAR_MAX + 1]		=	{1,};
static WORD		subbitbuf;
static int		bitcount;

void make_crctable(void)
{	WORD i, j, r;

	for (i = 0; i <= UCHAR_MAX; i++) 
	{	r = i;
		for (j = 0; j < CHAR_BIT; j++)
			if (r & 1) r = (r >> 1) ^ CRCPOLY;
			else       r >>= 1;

		crctable[i] = r;
	}
}

void fillbuf(int n)  /* Shift bitbuf n bits left, read n bits */
{	bitbuf <<= n;

	while (n > bitcount) 
	{	bitbuf |= subbitbuf << (n -= bitcount);
		if (CompressSize != 0) 
		{	CompressSize--;
			subbitbuf = (BYTE) getc(arcFile);
		} else subbitbuf = 0;

		bitcount = CHAR_BIT;
	}

	bitbuf |= subbitbuf >> (bitcount -= n);
}

WORD getbits(int n)
{	WORD x;

	x = bitbuf >> (BITBUFSIZ - n);  fillbuf(n);

	return x;
}

void putbits(int n, DWORD x)  /* Write rightmost n bits of x */
{	if (n < bitcount) 
	{	subbitbuf |= x << (bitcount -= n);
	} else 
	{	if (CompressSize < OriginalSize) 
		{	putc(subbitbuf | (x >> (n -= bitcount)), outFile);
			CompressSize++;
		} else unpackable = 1;

		if (n < CHAR_BIT) 
		{	subbitbuf = (WORD)(x << (bitcount = CHAR_BIT - n));
		} else 
		{	if (CompressSize < OriginalSize) 
			{	putc(x >> (n - CHAR_BIT), outFile);
				CompressSize++;
			} else unpackable = 1;
			subbitbuf = (WORD)(x << (bitcount = 2 * CHAR_BIT - n));
		}
	}
}

int fread_crc(BYTE *p, int n, FILE *f)
{	int i;

	i	=	n	= fread(p, 1,n,  f);
	OriginalSize += n;

	while (--i >= 0) UPDATE_CRC(*p++);

	return n;
}

void fwrite_crc(BYTE *p, int n, FILE *f)
{	if (fwrite(p, n,1, f) < 1) printf("Decode Error!!","Unable to write");
	while (--n >= 0) UPDATE_CRC(*p++);
}

void init_getbits(void)
{	bitbuf = 0;  subbitbuf = 0;  bitcount = 0;
	fillbuf(BITBUFSIZ);
}

void init_putbits(void)
{	bitcount = CHAR_BIT;  subbitbuf = 0;
}

void make_table(int nchar, BYTE bitlen[], int tablebits, WORD table[])
{	WORD count[17], weight[17], start[18], *p;
	WORD i, k, len, ch, jutbits, avail, nextcode, mask;

	for (i = 1; i <= 16		; i++	) count[i]	= 0;
	for (i = 0; i < nchar	; i++	) count[bitlen[i]]++;

	start[1] = 0;
	for (i = 1; i <= 16; i++)	start[i + 1] = start[i] + (count[i] << (16 - i));
	if (start[17] != (WORD)(1U << 16)) printf("Table Making Error!!","Bad table");

	jutbits = 16 - tablebits;

	for (i = 1; i <= tablebits; i++) 
	{	start[i] >>= jutbits;
		weight[i] = 1U << (tablebits - i);
	}

	while (i <= 16) weight[i++] = 1U << (16 - i);

	i = start[tablebits + 1] >> jutbits;

	if (i != (WORD)(1U << 16)) 
	{	k = 1U << tablebits;
		while (i != k) table[i++] = 0;
	}

	avail = nchar;
	mask = 1U << (15 - tablebits);
	for (ch = 0; ch < nchar; ch++) 
	{	if ((len = bitlen[ch]) == 0) continue;
		nextcode = start[len] + weight[len];
		if (len <= tablebits) 
		{	for (i = start[len]; i < nextcode; i++) table[i] = ch;
		} else 
		{	k = start[len];
			p = &table[k >> jutbits];
			i = len - tablebits;
			while (i != 0) 
			{	if (*p == 0) 
				{	right[avail] = left[avail] = 0;
					*p = avail++;
				}

				if (k & mask) p = &right[*p];
				else          p = &left[*p];

				k <<= 1;
				i--;
			}
			*p = ch;
		}
		start[len] = nextcode;
	}
}

/***********************************************************
	maketree.c -- make Huffman tree
***********************************************************/

static int    n, heapsize;
static short  heap[NC + 1];
static WORD *freq, *sortptr, len_cnt[17];
static BYTE  *len;

static void count_len(int i)  /* call with i = root */
{
	static int depth = 0;

	if (i < n) len_cnt[(depth < 16) ? depth : 16]++;
	else {
		depth++;
		count_len(left [i]);
		count_len(right[i]);
		depth--;
	}
}

static void make_len(int root)
{
	int i, k;
	ULONG cum;

	for (i = 0; i <= 16; i++) len_cnt[i] = 0;
	count_len(root);
	cum = 0;
	for (i = 16; i > 0; i--)
		cum += len_cnt[i] << (16 - i);
	while (cum != (1U << 16)) {
		fprintf(stderr, "17");
		len_cnt[16]--;
		for (i = 15; i > 0; i--) {
			if (len_cnt[i] != 0) {
				len_cnt[i]--;  len_cnt[i+1] += 2;  break;
			}
		}
		cum--;
	}
	for (i = 16; i > 0; i--) {
		k = len_cnt[i];
		while (--k >= 0) len[*sortptr++] = i;
	}
}

static void downheap(int i)
	/* priority queue; send i-th entry down heap */
{
	int j, k;

	k = heap[i];
	while ((j = 2 * i) <= heapsize) {
		if (j < heapsize && freq[heap[j]] > freq[heap[j + 1]])
		 	j++;
		if (freq[k] <= freq[heap[j]]) break;
		heap[i] = heap[j];  i = j;
	}
	heap[i] = k;
}

static void make_code(int n, BYTE len[], WORD code[])
{
	int    i;
	WORD start[18];

	start[1] = 0;
	for (i = 1; i <= 16; i++)
		start[i + 1] = (start[i] + len_cnt[i]) << 1;
	for (i = 0; i < n; i++) code[i] = start[len[i]]++;
}

int make_tree(int nparm, WORD freqparm[],
				BYTE lenparm[], WORD codeparm[])
	/* make tree, calculate len[], return root */
{
	int i, j, k, avail;

	n = nparm;  freq = freqparm;  len = lenparm;
	avail = n;  heapsize = 0;  heap[1] = 0;
	for (i = 0; i < n; i++) {
		len[i] = 0;
		if (freq[i]) heap[++heapsize] = i;
	}
	if (heapsize < 2) {
		codeparm[heap[1]] = 0;  return heap[1];
	}
	for (i = heapsize / 2; i >= 1; i--)
		downheap(i);  /* make priority queue */
	sortptr = codeparm;
	do {  /* while queue has at least two entries */
		i = heap[1];  /* take out least-freq entry */
		if (i < n) *sortptr++ = i;
		heap[1] = heap[heapsize--];
		downheap(1);
		j = heap[1];  /* next least-freq entry */
		if (j < n) *sortptr++ = j;
		k = avail++;  /* generate new node */
		freq[k] = freq[i] + freq[j];
		heap[1] = k;  downheap(1);  /* put into queue */
		left[k] = i;  right[k] = j;
	} while (heapsize > 1);
	sortptr = codeparm;
	make_len(k);
	make_code(nparm, lenparm, codeparm);
	return k;  /* return root */
}


/****************************************************************
		내가 추가한 넘들
****************************************************************/
void pressFolder(char *fn)	//	폴더 추가 (헤더에 쓴다.)
{	if (bStop) return;

	BYTE stringsize = 1;

	fwrite(&stringsize	,1			,1,outFile);
	fwrite(&ISFOLDER	,4			,1,outFile);

	stringsize = (BYTE)strlen(fn);

	fwrite(&stringsize	,1			,1,outFile);
	fwrite(fn			,stringsize	,1,outFile);
}

BOOL
cNUX::PressCreate(char *fn,BOOL _bIsMerge)
{	
	s_bIsMerge	=	_bIsMerge;

	if (outFile) return printf("Error!! in cNUX::PressCreate","Achive File Already Opened!!");
	bStop	=	FALSE;
	bSTOP	=	FALSE;
	bAPPEND	=	FALSE;

	GetCurrentDirectory(512,processFolder);

	if (crctable[0]!=0)	make_crctable();

	strcpy(ArcName,fn);

	strcpy(tempArc,_ms("%s\\%s",processFolder,tmpnam(NULL)));

	outFile		=	fopen(tempArc, "wb");

	if (!outFile) return printf("Compress Error!!","'%s' File Create Error!!",fn);

	ProcessRate	=	0;
	ProcessSize	=	0;
	TotalSize	=	0;
	FileSize	=	0;
	FileCount	=	0;
	FileRate	=	0;

	appendPos	=	ftell(outFile);
	fwrite(&FileCount	,4,1,outFile);
	fwrite(&FileSize	,4,1,outFile);
	fwrite(&TotalSize	,4,1,outFile);

	return TRUE;
}

BOOL
cNUX::PressAppend(char *fn,BOOL _bIsMerge)
{	
	s_bIsMerge	=	_bIsMerge;

	if (outFile) return printf("Error!! in cNUX::PressCreate","Achive File Already Opened!!");
	bStop	=	FALSE;
	bSTOP	=	FALSE;
	bAPPEND	=	TRUE;
	GetCurrentDirectory(512,processFolder);

	if (crctable[0]!=0)	make_crctable();

	strcpy(ArcName,fn);
	outFile		=	fopen(ArcName, "rb+");
	fseek(outFile,0,SEEK_END);

	appendPos	=	ftell(outFile);

	ProcessRate	=	0;
	ProcessSize	=	0;
	TotalSize	=	0;
	FileSize	=	0;
	FileCount	=	0;
	FileRate	=	0;

	fwrite(&FileCount	,4,1,outFile);
	fwrite(&FileSize	,4,1,outFile);
	fwrite(&TotalSize	,4,1,outFile);

	if (!outFile) return printf("Compress Error!!","'%s' File Create Error!!",fn);

	return TRUE;
}

DWORD
cNUX::PressClose()
{	if (!outFile) return printf("Error!! in PressFolder","Achive File not Created!!");

	SetCurrentDirectory(processFolder);

	if (bStop)	stop();
	else
	{	fputc(0, outFile);		/* end of archive */
		fwrite(&appendPos	,sizeof(DWORD  ),1,outFile);	//	화일의 시작위치
		fwrite(&ISPRESSFILE	,sizeof(DWORD  ),1,outFile);	//	화일의 시작위치

		FileSize	=	ftell(outFile);

		fseek(outFile,appendPos,SEEK_SET);
		fwrite(&FileCount	,4,1,outFile);
		fwrite(&FileSize	,4,1,outFile);
		fwrite(&TotalSize	,4,1,outFile);

		fclose(outFile);
		outFile	=	NULL;

		if (!bAPPEND)
		{	remove(ArcName);
			MoveFile(tempArc,ArcName);
		}
	}

	return TotalSize;
}
BOOL
cNUX::PressFile(char *fn)
{	if (bStop) return FALSE;

	if (!outFile) return printf("Error!! in cNUX::PressFile","Achive File not Created!!");

	long headerpos, arcpos;

	strcpy(filename,_exportFileName(fn));

	if ((inFile = fopen(fn, "rb")) == NULL)
		return PressFolder(fn,TRUE,TRUE);

	strcpy(cNUX::CurrentFile,filename);

	fseek(inFile,0L,SEEK_END);
	inFileSize = ftell(inFile);
	fseek(inFile,0L,SEEK_SET);

//	skip();
	headerpos	=	ftell(outFile);
	namelen		=	strlen(filename);
	headersize	=	25 + namelen;

	memcpy(header, "-sn5-", 5);												/* compress		*/
	write_header();															/* temporarily	*/

	arcpos		= ftell(outFile);
	OriginalSize= CompressSize = 0;
	unpackable	= 0;
	crc			= INIT_CRC;

	if (s_bIsMerge)
	{	header[3] = '0';													/* store		*/
		rewind(inFile);
		fseek(outFile, arcpos, SEEK_SET);
		store();
	}
	else
	{	if (!encode()) return FALSE;

		if (unpackable) 
		{	header[3] = '0';													/* store		*/
			rewind(inFile);
			fseek(outFile, arcpos, SEEK_SET);
			store();
		}
	}

	file_crc = crc ^ INIT_CRC;
	fclose(inFile);
	put_to_header(5, 4, CompressSize);
	put_to_header(9, 4, OriginalSize);

	memcpy(header + 13, "\0\0\0\0\x20\x01", 6);
	memcpy(header + headersize - 3, "\x20\0\0", 3);

	fseek(outFile, headerpos, SEEK_SET);
	write_header();  /* true header */

	fseek(outFile, 0L, SEEK_END);

	FileRate	=	100;
	if (ProcessPress) ProcessPress();

	TotalSize	+=	inFileSize;
	FileCount++;

	//int r = ratio(CompressSize, OriginalSize);
	//	printf(" %d.%d%%\n", r / 10, r % 10);

	return TRUE;  /* success */
}
/*
BOOL
cNUX::PressFile(char *fn)
{	if (bStop) return FALSE;

	if (!outFile) return printf("Error!! in cNUX::PressFile","Achive File not Created!!");

	long headerpos, arcpos;

	strcpy(filename,_exportFileName(fn));

	if ((inFile = fopen(fn, "rb")) == NULL)	return PressFolder(fn,TRUE,TRUE);

	strcpy(cNUX::CurrentFile,filename);

	fseek(inFile,0L,SEEK_END);
	inFileSize = ftell(inFile);
	fseek(inFile,0L,SEEK_SET);

//	skip();
	headerpos	=	ftell(outFile);
	namelen		=	strlen(filename);
	headersize	=	25 + namelen;

	memcpy(header, "-sn5-", 5);												// compress		
	cMSG::Put("a",fn);
	write_header();															// temporarily	
	cMSG::Put(fn,fn);

	arcpos		= ftell(outFile);
	OriginalSize= CompressSize = 0;
	unpackable	= 0;
	crc			= INIT_CRC;

	if (!encode()) return FALSE;

	if (unpackable) 
	{	header[3] = '0';													// store		
		rewind(inFile);
		fseek(outFile, arcpos, SEEK_SET);
		store();
	}

	file_crc = crc ^ INIT_CRC;
	fclose(inFile);
	put_to_header(5, 4, CompressSize);
	put_to_header(9, 4, OriginalSize);

	memcpy(header + 13, "\0\0\0\0\x20\x01", 6);
	memcpy(header + headersize - 3, "\x20\0\0", 3);

	fseek(outFile, headerpos, SEEK_SET);
	write_header();  // true header 

	fseek(outFile, 0L, SEEK_END);

	FileRate	=	100;
	if (ProcessPress) ProcessPress();

	TotalSize	+=	inFileSize;
	FileCount++;

	//int r = ratio(CompressSize, OriginalSize);
	//	printf(" %d.%d%%\n", r / 10, r % 10);

	return TRUE;  // success 
}
*/
void
cNUX::PressAllFile()
{	if (bStop) return;

	cSTRINGS	*files	=	cFOLDER::GetFiles(dSORT_NOT);

	if (files)
	{	
		for (int i=0;i<files->Count;i++)	PressFile(files->GetStr(i));

		KILL(files);
	}
}

BOOL
cNUX::PressFolder(char *fn,BOOL includeCurrent,BOOL includeSubFolder)
{	if (bStop			)	return FALSE;
	if (!outFile		)	return cMSG::Output("Error!! in cNUX::PressFolder %s","Achive File not Created!!");
	if (!IsFolder(fn)	)	return FALSE;

	SetCurrentDirectory(fn);

	cSTRINGS *strs	=	NULL;

	if (includeSubFolder)	strs	=	cFOLDER::GetFolders(fn);
	else
	{	if (includeCurrent) pressFolder(_exportFileName(fn));	//	서브디렉토리는 추가 안하고 디렉토리 이름만..

		PressAllFile();

		return TRUE;
	}

	char	curDir[512],temp[512];

	GetCurrentDirectory(512,curDir);

	if (strs)
	{	cSTRING	*str	=	strs->Pick(0);

		if (includeCurrent)	strcpy(temp,_exportFolderName(fn));
		else
		{	strcpy(temp,fn);
			PressAllFile();
		}

		while(str->pNEXT)
		{	if (!includeCurrent)
				if (str->Match(fn))
				{	str	=	str->pNEXT;
					continue;
				}

			SetCurrentDirectory(curDir);
			SetCurrentDirectory(str->String);

			str->Minus(_ms("%s\\",temp));
			pressFolder(str->String);

			PressAllFile();

			str	=	str->pNEXT;
		}

		KILL(strs);

		SetCurrentDirectory(curDir);

		return TRUE;
	}

	return TRUE;
}

BOOL
cNUX::PressExtract(char *fn,char *destFolder,cSTRINGS *files)
{	if (arcFile) return printf("Error!! in cNUX::PressExtract","Achive File Aleady Opened!!",fn);	//	화일을 찾을 수 없다.

	bStop		=	FALSE;
	bSTOP		=	FALSE;
	ProcessRate	=	0;
	ProcessSize	=	0;
	TotalSize	=	0;
	FileSize	=	0;
	FileCount	=	0;
	FileRate	=	0;

//	초기화
	char	curFolder[512];
	GetCurrentDirectory(512,curFolder);

	strcpy(ArcName,fn);

	arcFile = fopen(fn, "rb");

	if (!arcFile)
	{
		return printf("Error!! in cNUX::PressExtract","'%s' File not Open",fn);	//	화일을 찾을 수 없다.
	}

	DWORD	ispress;

	fseek(arcFile,-4,SEEK_END);
	fread(&ispress,4,1,arcFile);

	if (ispress!=ISPRESSFILE)
	{	
		fclose(arcFile);
		arcFile	=	NULL;

		if (ispress==ISMERGEFILE || ispress==ISMERGEFOLDER)	return MergeExtract(fn,destFolder,files);

		return	printf("Error!! in cNUX::PressExtract","'%s' File not NUX file",fn);	//	화일을 찾을 수 없다.
	}

	fseek(arcFile,-8,SEEK_END);
	fread(&appendPos,4,1,arcFile);
	fseek(arcFile,appendPos,SEEK_SET);

	fread(&FileCount,4,1,arcFile);
	fread(&FileSize	,4,1,arcFile);
	fread(&TotalSize,4,1,arcFile);

	if (destFolder)
		if(SetCurrentDirectory( destFolder) == 0) 
			if (!cFOLDER::Create(destFolder,TRUE)) return printf("Error!! in cNUX::PressExtract","Don't Create \"%s\" Folder",destFolder);

	GetCurrentDirectory(512,processFolder);

	if (crctable[0]!=0)	make_crctable();

//	압축 풀기 시작

	int		n, method;
	WORD	ext_headersize;

//	outFile	=	NULL;

	while(1)
	{	int read	=	read_header();
		if (read == 0)	break;
		if (read == -1)
		{	if (outFile)	fclose(outFile);
			outFile	=	NULL;
			SetCurrentDirectory(processFolder);

			return printf("Error in cNUX::PressExtract","CRC Error!!");	//	CRC Error
		}
		if (read == 2)
		{	SetCurrentDirectory(processFolder);
			if (!IsFolder(folderName))
			{	if (!cFOLDER::Create(folderName,TRUE)) return printf("Error in cNUX::PressExtract","Don't Create \"%s\" Folder",folderName);
				if (files)
				{	char folder[512];
					GetCurrentDirectory(512,folder);
					files->Add(folder);
				}
			}else 
				SetCurrentDirectory(folderName);
			continue;
		}
		
		outFile = fopen(filename, "wb");

		if (!outFile)
		{	SetCurrentDirectory(processFolder);

			return printf("Error in cNUX::PressExtract","'%s' File Create Error!!",filename);
		}

		strcpy(cNUX::CurrentFile,filename);

		if (files)
		{	char folder[512];
			GetCurrentDirectory(512,folder);
			files->Add(_ms("%s\\%s",folder,filename));
		}

		crc			= INIT_CRC;
		method		= header[3];
		header[3]	= ' ';

		if (!strchr("045", method) || memcmp("-sn -", header, 5)) 
		{	if (outFile)	fclose(outFile);
			outFile	=	NULL;
			SetCurrentDirectory(processFolder);

			return printf("Error in cNUX::PressExtract","'%s' File header Error!!",filename);
		}

		ext_headersize = (WORD)get_from_header(headersize - 2, 2);

		while (ext_headersize != 0)
		{	//	fprintf(stderr, "There's an extended header of size %u.\n",ext_headersize);
			CompressSize -= ext_headersize;

			if (fseek(arcFile, ext_headersize - 2, SEEK_CUR))
			{	if (outFile)	fclose(outFile);
				outFile	=	NULL;
				SetCurrentDirectory(processFolder);

				return printf("Error in cNUX::PressExtract","File read Error!!");
			}

			ext_headersize = fgetc(arcFile);
			ext_headersize += (WORD)fgetc(arcFile) << 8;
		}

		crc = INIT_CRC;

		if (method != '0') decode_start();

		DWORD size		=	OriginalSize;
		FileRate		=	0;

		while (OriginalSize != 0) 
		{	n = (WORD)((OriginalSize > DICSIZ) ? DICSIZ : OriginalSize);

			if (method != '0') decode(n, buffer);
			else 
			if (fread((char *)buffer, n,1, arcFile) != (unsigned int)n)
			{	if (outFile)	fclose(outFile);
				outFile	=	NULL;

				SetCurrentDirectory(processFolder);

				return printf("Error in cNUX::PressExtract","File read Error!!");
			}
			
			fwrite_crc(buffer, n, outFile);
			OriginalSize -= n;

			if (size > d1MEGA)		FileRate	=	100-(OriginalSize/(size/100));
			else					FileRate	=	100-(OriginalSize/size);

			DWORD fsize = ftell(outFile);

			if(TotalSize > 0)
			{
				if (TotalSize<=d1MEGA)	ProcessRate	=	(ProcessSize+fsize)*100/TotalSize;
				else					ProcessRate	=	(ProcessSize+fsize)/(TotalSize/100);
			}
			
			if (ProcessExtract)	ProcessExtract();

			if (stop())	return FALSE;
		}

		FileRate	=	100;

		ProcessSize	+=	cFILE::Size(outFile);

		if(TotalSize > 0 && ProcessSize > 0)
		{
			if (TotalSize <= d1MEGA)	ProcessRate	=	ProcessSize*100/TotalSize;
			else						ProcessRate	=	ProcessSize/(TotalSize/100);
		}
		if (ProcessExtract)	ProcessExtract();

		fclose(outFile);
		outFile	=	NULL;

		if ((crc ^ INIT_CRC) != file_crc)
		{	if (outFile)	fclose(outFile);
			outFile	=	NULL;

			SetCurrentDirectory(processFolder);

			return printf("Error in PressExtract","CRC error(last piece)");
		}
	}

	ProcessRate	=	100;
	if (ProcessExtract)	ProcessExtract();

	if (outFile)	fclose(outFile);
	if (arcFile)	fclose(arcFile);

	arcFile	=	NULL;
	outFile	=	NULL;

	SetCurrentDirectory(curFolder);

	return TRUE;
}

void
cNUX::SetFunc(void (*press)(),void (*extract)())
{	ProcessExtract	=	extract;
	ProcessPress	=	press;
}

void
cNUX::Stop()
{	bStop	=	TRUE;
	bSTOP	=	TRUE;
}

/***************************************************************************************
	압축하지 않고 걍 합치기.
***************************************************************************************/

cMERGEINFO		Info[dMAX_MERGE_FILE];
static	char	tempBLOCK[BLOCKSIZE];

BOOL
cNUX::MergeCreate(char *fn)
{	if (outFile) return printf("Error!! in cNUX::MergeCreate","Achive File Already Opened!!");

	bAPPEND		=	FALSE;
	bStop		=	FALSE;
	bSTOP		=	FALSE;
	s_bIsMergeFolder=	FALSE;

	GetCurrentDirectory(512,processFolder);

	strcpy(ArcName,fn);

	strcpy(tempArc,_ms("%s\\%s",processFolder,tmpnam(NULL)));

	outFile		=	fopen(tempArc, "wb");
	if (!outFile) return printf("Merge Error!!","templete file creation Error!!",tempArc);

	ProcessRate	=	0;
	ProcessSize	=	0;
	TotalSize	=	0;
	FileSize	=	0;
	FileCount	=	0;
	FileRate	=	0;

	appendPos	=	ftell(outFile);

	fwrite(&FileCount	,4,1,outFile);
	fwrite(&FileCount	,4,1,outFile);
	fwrite(&FileCount	,4,1,outFile);

	return TRUE;
}

BOOL
cNUX::MergeAppend(char *fn)
{	if (outFile) return printf("Error!! in cNUX::MergeCreate","Achive File Already Opened!!");

	bAPPEND		=	TRUE;
	bStop		=	FALSE;
	bSTOP		=	FALSE;
	s_bIsMergeFolder=	FALSE;

	ProcessRate	=	0;
	ProcessSize	=	0;
	TotalSize	=	0;
	FileSize	=	0;
	FileCount	=	0;
	FileRate	=	0;

	GetCurrentDirectory(512,processFolder);

	strcpy(ArcName,fn);

	outFile		=	fopen(fn, "rb+");
	if (!outFile) return printf("Error in cNUX::MergeAppend!!","'%s' File Append Error!!",fn);

	fseek(outFile,0L,SEEK_END);
	appendPos	=	ftell(outFile);

	fwrite(&FileCount	,4,1,outFile);
	fwrite(&FileCount	,4,1,outFile);
	fwrite(&FileCount	,4,1,outFile);

	return TRUE;
}

BOOL
cNUX::MergeClose()
{	if (!outFile) return printf("Error!! in PressFolder","Achive File not Created!!");

	SetCurrentDirectory(processFolder);

	int	loc	=	ftell(outFile)-appendPos;		//	

	if (!s_bIsMergeFolder)	qsort((void *)Info,FileCount,sizeof(Info[0]) ,Compare_Name);

	fwrite(Info	,sizeof(Info[0])*FileCount,1,outFile);

	fwrite(&appendPos		,sizeof(DWORD  ),1,outFile);

	if (s_bIsMergeFolder)	fwrite(&ISMERGEFOLDER	,sizeof(DWORD  ),1,outFile);
	else				fwrite(&ISMERGEFILE		,sizeof(DWORD  ),1,outFile);

	fseek(outFile,appendPos,SEEK_SET);
	fwrite(&FileCount	,4,1,outFile);		//	화일 수
	fwrite(&TotalSize	,4,1,outFile);		//	전체 데이터 사이즈
	fwrite(&loc			,4,1,outFile);		//	헤더 위치
	fclose(outFile);

	outFile	=	NULL;

	if (!bAPPEND)
	{	remove(ArcName);
		MoveFile(tempArc,ArcName);
	}

	return TRUE;
}

void
cNUX::MergeAllFile()
{	if (bStop)	return;

	cSTRINGS	*files	=	cFOLDER::GetFiles(dSORT_NOT);

	if (files)
	{	for (int i=0;i<files->Count;i++)
			MergeFile(files->GetStr(i));

		KILL(files);
	}
}

void
cNUX::mergeFolder(char *fn)
{	
	if (!outFile) return;

	DWORD size				=	strlen(fn)+1;
	strcpy(Info[FileCount].FName,"folder");

	Info[FileCount].Locate	=	ftell(outFile);
	Info[FileCount].Size	=	size;
	TotalSize				+=	size;

	fwrite(fn,size,1,outFile);

	FileCount++;
}

BOOL
cNUX::MergeFolder(char *fn,BOOL includeCurrent,BOOL includeSubFolder)
{	if (bStop			)	return FALSE;
	if (!outFile		)	return cMSG::Output("Error!! in cNUX::MergeFolder","Achive File not Created!!");
	if (!IsFolder(fn)	)	return FALSE;

	s_bIsMergeFolder	=	TRUE;

	SetCurrentDirectory(fn);

	cSTRINGS *strs	=	NULL;

	if (includeSubFolder)	strs	=	cFOLDER::GetFolders(fn);
	else
	{	if (includeCurrent) mergeFolder(_exportFileName(fn));	//	서브디렉토리는 추가 안하고 디렉토리 이름만..

		MergeAllFile();

		return TRUE;
	}

	char	curDir[512],temp[512];

	GetCurrentDirectory(512,curDir);

	if (strs)
	{	cSTRING	*str	=	strs->Pick(0);

		if (includeCurrent)	strcpy(temp,_exportFolderName(fn));
		else
		{	strcpy(temp,fn);
			MergeAllFile();
		}

		while(str->pNEXT)
		{	if (!includeCurrent)
				if (str->Match(fn))
				{	str	=	str->pNEXT;
					continue;
				}

			SetCurrentDirectory(curDir);
			SetCurrentDirectory(str->String);

			str->Minus(_ms("%s\\",temp));

			mergeFolder(str->String);

			MergeAllFile();

			str	=	str->pNEXT;
		}

		KILL(strs);

		SetCurrentDirectory(curDir);

		return TRUE;
	}

	return TRUE;
}

BOOL
cNUX::MergeFile(char *fn)
{	if (bStop		)	return FALSE;
	if (!outFile	)	return printf("Error!! in cNUX::MergeFile","Achive File not Created!!");
	if (IsFolder(fn))	return printf("Error!! in cNUX::MergeFile","'%s' file not file",fn);;

	FILE *fp	=	fopen(fn,"rb");

	if (!fp)		return printf("Error!! in cNUX::MergeFile","'%s' File not found!!",fn);

	strcpy(Info[FileCount].FName,_exportFileName(fn));

	DWORD size				=	cFILE::Size(fn);
	Info[FileCount].Locate	=	ftell(outFile);
	Info[FileCount].Size	=	size;
	TotalSize				+=	size;

	FileSize=	0;

	for (DWORD j=0;j<size/BLOCKSIZE;j++)
	{	fread (tempBLOCK,BLOCKSIZE,1,fp);
		fwrite(tempBLOCK,BLOCKSIZE,1,outFile);

		FileSize	+=	BLOCKSIZE;
		ProcessSize	+=	BLOCKSIZE;

		FileRate	=	min(FileSize	/max((size/100),1),100);
		if (ProcessPress)	ProcessPress();
	}

	fread (tempBLOCK,size%BLOCKSIZE,1,fp);
	fwrite(tempBLOCK,size%BLOCKSIZE,1,outFile);

	ProcessSize	+=	(size%BLOCKSIZE);
	FileRate	=	100;
	if (ProcessPress)	ProcessPress();

	fclose(fp);

	FileCount++;

	return TRUE;  /* success */
}

BOOL
cNUX::MergeExtract(char *fn,char *destFolder,cSTRINGS *files)
{	if (arcFile) return printf("Error!! in cNUX::MergeExtract","Achive File Aleady Opened!!",fn);	//	이미 열려 있다.

	ProcessRate	=	0;
	ProcessSize	=	0;
	TotalSize	=	0;
	FileSize	=	0;
	FileCount	=	0;
	FileRate	=	0;

//	초기화
	GetCurrentDirectory(512,processFolder);

	strcpy(ArcName,fn);

	arcFile = fopen(fn, "rb");

	if (!arcFile) return printf("Error!! in cNUX::MergeExtract","'%s' File Not Open",fn);			//	화일을 찾을 수 없다.

	DWORD	Locate	,i	,j;
	DWORD	ismerge;

	fseek(arcFile	,-4,SEEK_END);
	fread(&ismerge	,4,1,arcFile);

	if (ismerge	!=	ISMERGEFILE && ismerge	!=	ISMERGEFOLDER)
	{	fclose(arcFile);
		arcFile	=	NULL;
		if (ismerge==ISPRESSFILE)	return PressExtract(fn,destFolder);
		return	printf("Error!! in cNUX::MergeExtract","'%s' File Not Merge File!!",fn);			//	합쳐진 화일이 아니다.
	}

	if (destFolder)
		if(SetCurrentDirectory( destFolder) == 0) 
			if (!cFOLDER::Create(destFolder,TRUE)) 
				return printf("Error!! in cNUX::MergeExtract","Don't Create \"%s\" Folder",destFolder);

	fseek(arcFile	,-8,SEEK_END);			//	화일의 헤더 위치
	fread(&appendPos,4,1,arcFile);			//	를 읽는다.

	fseek(arcFile	,appendPos,SEEK_SET);	//	헤더로 간다.
	fread(&FileCount,4,1,arcFile);			//	화일 수
	fread(&TotalSize,4,1,arcFile);			//	화일 수
	fread(&Locate	,4,1,arcFile);			//	화일 정보 위치

	fseek(arcFile,Locate,SEEK_SET);

	fread(Info,sizeof(Info[0])*FileCount,1,arcFile);

	for (i=0;i<FileCount;i++)
	{	DWORD	size;

		fseek(arcFile,Info[i].Locate,SEEK_SET);

		if (stricmp(Info[i].FName,"folder")==0)
		{	fread(tempBLOCK,Info[i].Size,1,arcFile);

			SetCurrentDirectory(processFolder);
			if (!IsFolder(tempBLOCK))
			{	if (!cFOLDER::Create(tempBLOCK,TRUE)) return printf("Error in cNUX::MergeExtract","Don't Create \"%s\" Folder",tempBLOCK);
				if (files)
				{	char folder[512];
					GetCurrentDirectory(512,folder);
					files->Add(folder);
				}
			}	else SetCurrentDirectory(tempBLOCK);
			continue;
		}

		FILE	*fp	=	fopen(Info[i].FName,"wb");
		strcpy(CurrentFile,Info[i].FName);

		if (!fp)
		{	printf("Error in cNUX::MergeExtract","File Create Error!!");
			continue;
		}

		size	=	Info[i].Size;

		FileSize=	0;

		for (j=0;j<(size/BLOCKSIZE);j++)
		{	fread (tempBLOCK,BLOCKSIZE,1,arcFile);
			fwrite(tempBLOCK,BLOCKSIZE,1,fp);

			FileSize	+=	BLOCKSIZE;
			ProcessSize	+=	BLOCKSIZE;
	
			FileRate	=	min(FileSize	/max((size/100),1),100);
			ProcessRate	=	min(ProcessSize	/max((TotalSize/100),1),100);
			if (ProcessExtract)	ProcessExtract();
		}

		fread (tempBLOCK,size%BLOCKSIZE,1,arcFile);
		fwrite(tempBLOCK,size%BLOCKSIZE,1,fp);

		ProcessSize	+=	(size%BLOCKSIZE);
		ProcessRate	=	min(ProcessSize	/max((TotalSize/100),1),100);
		FileRate	=	100;
		if (ProcessExtract)	ProcessExtract();

		fclose(fp);
	}

	fclose(arcFile);

	arcFile	=	NULL;

	return TRUE;
}