#include "cIMAGE.H"

#pragma pack(1)
struct PCX_HEADER_TYPE 
{	char	manufacturer;
	char	version;
	char	encoding;
	char	bits_per_pixel;
	short	sx, sy, ex, ey;
	short	hres, vres;
	char	pallete[48];
	char	reserved;
	char	color_planes;
	short	bytes_per_line;
	short	pallete_type;
	char	filler[58];
} PCX_HEADER;
#pragma pack()

BYTE	palette[768];

void
decode(BYTE *buffer, int fullsize,int bpl,int width, cFILE *file)
{   int index, data, num,count = 0;

    for(index = 0; index < fullsize;) 
	{	data = fgetc(file->getFp());

        if (data >= 0xc0)
		{	num	=	data & 0x3f;
			data = fgetc(file->getFp());
			while(num--)
			{	index++;
				if ((index % bpl) < width)	buffer[count++] = data;
			}
		}
        else
		{	index++;
			if ((index % bpl) < width)	buffer[count++] = data;
		}
    }
}

BOOL
cPCX::Load(char * fname,BOOL change16bit)
{	pKILL(m_p16IMAGE);
	pKILL(m_p8IMAGE);
	pKILL(m_pPLT);

	cFILE	file;

	if (!file.open(fname,"rb"))	return ERRMSG("Error in cPCX::Load","can't find '%s' file",fname);

	file.read(&PCX_HEADER, sizeof(PCX_HEADER));

	m_iWidth	= PCX_HEADER.ex - PCX_HEADER.sx + 1;
	m_iHeight	= PCX_HEADER.ey - PCX_HEADER.sy + 1;

/*
	MSGOUT("PCX file : %s", fname);
	MSGOUT("- HEADER INFO -");
	MSGOUT("  manufacturer: %d", PCX_HEADER.manufacturer);
	MSGOUT("       version: %d", PCX_HEADER.version);
	MSGOUT("      encoding: %d", PCX_HEADER.encoding);
	MSGOUT("bits_per_pixel: %d", PCX_HEADER.bits_per_pixel);
	MSGOUT("sx, sy, ex, ey: %d, %d, %d, %d", PCX_HEADER.sx, PCX_HEADER.sy, PCX_HEADER.ex, PCX_HEADER.ey);
	MSGOUT("    hres, vres: %d, %d", PCX_HEADER.hres, PCX_HEADER.vres);
	MSGOUT("   pallete[48]:");
	MSGOUT("      reserved:");
	MSGOUT("  color_planes: %d", PCX_HEADER.color_planes);
	MSGOUT("bytes_per_line: %d", PCX_HEADER.bytes_per_line);
	MSGOUT("  pallete_type: %d", PCX_HEADER.pallete_type);
	MSGOUT("    filler[58]:");
*/
	if ( (PCX_HEADER.manufacturer != 10) || (PCX_HEADER.version != 5) )
		return ERRMSG("Error in cPCX::Load","It's not a PCX File '%s'",fname);

	if ( PCX_HEADER.color_planes == 3 )
	{	BYTE	*buffer	=	_new(BYTE,m_iWidth * m_iHeight * 3,NULL,"cPCX::Load buffer");

		decode(buffer,PCX_HEADER.bytes_per_line*m_iHeight*3,PCX_HEADER.bytes_per_line, m_iWidth, &file);

		m_p16IMAGE	=	Pcx24To16(buffer,m_iWidth,m_iHeight);			//	이미지 변환 24bit -> 16bit
		_free(buffer);

		m_iBPP		=	16;
	}
	else
	{	m_p8IMAGE	=	_new(BYTE,m_iWidth * m_iHeight+4,NULL,"cPCX::m_p8IMAGE");
		*(WORD *)(m_p8IMAGE		)	=	m_iWidth;
		*(WORD *)(m_p8IMAGE+2	)	=	m_iHeight;

		decode(m_p8IMAGE+4,PCX_HEADER.bytes_per_line*m_iHeight,PCX_HEADER.bytes_per_line, m_iWidth, &file);

		file.skip(1);
		file.read(palette, 256 * 3);
		m_pPLT		=	Correct8BitPalette(palette);
		m_iBPP		=	8;
	}

	file.close();

	if (change16bit && m_p8IMAGE)
	{	m_p16IMAGE	=	_new(WORD,m_iWidth*m_iHeight+2,NULL,"cPCX::m_p16IMAGE");
		m_p16IMAGE[0]	=	(WORD)m_iWidth;
		m_p16IMAGE[1]	=	(WORD)m_iHeight;

		for (DWORD i = 0 ; i < (DWORD)m_iWidth*m_iHeight; i++)	m_p16IMAGE[i+2]	=	m_pPLT[m_p8IMAGE[i+4]];

		_free(m_p8IMAGE);
	}

	return TRUE;
}