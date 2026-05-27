//////////////////////////////////////////////////////////////////////////////////********
//	SFC.H

//	∏˚Ã°¡ÅE¿ØøÅE— ∑Á∆æ∞ÅE∏˚Ã°¡ÅE¿ØøÅE— ∏≈≈©∑Œ
//	MMX∑π¡ˆΩ∫≈ÕøÕ FPU∑π¡ˆΩ∫≈Õ∏¶ ¿ÃøÅE— ∏ﬁ∏∏Æ ¿ÅE€.

//	¿€º∫ ≥Ø¬• : 1999.02.22
//	1¬ÅE ∞ªΩ≈ : 1999.03.15	-	* MMX∑π¡ˆΩ∫≈ÕøÕFPU∑π¡ˆΩ∫≈Õ∏¶ ¿ÃøÅE— ∏ﬁ∏∏Æ ¿ÅE€√ﬂ∞°
//								* UTIL.H -> GLOBAL.H ∑Œ πŸ≤ﬁ.
//	2¬ÅE ∞ªΩ≈ : 1999.03.24	-	* cRECT global class∏¶ √ﬂ∞°«‘¿ª Ω√¿€¿∏∑Œ ¿ÅE™ ≈¨∑°Ω∫ ¿€º∫.
//	3¬ÅE ∞ªΩ≈ : 1999.05.14  -   * sine,cosine ≈◊¿Ã∫ÅE√ﬂ∞°
//	4¬ÅE ∞ªΩ≈ : 2000.06.09	-	* sine,cosine ≈◊¿Ã∫ÅE¡¶∞≈

//								* cSTRING,cSTRINGS,cFOLDER,cFILE,cRECT,cRECTS,cRECT_SIZE,cRECTS_SIZE,CPos,cPOINTS class
//								√ﬂ∞° π◊ ±≥¡§

//								* æ–√ÅE≈¨∑°Ω∫ cNUX √ﬂ∞°

//								* _ms,_exportFileName,_exportFolderName,_exportExt,_setExt,_fileSize,IsFolder
//								µ˚‹« ¿ÅE™ «‘ºÅE√ﬂ∞°π◊ ±≥¡§

//								* ¿ÅE™ ∫ØºÅEg_strLogFolder √ﬂ∞°(«¡∑Œ±◊∑•¿Ã Ω««‡µ» ∆˙¥ÅE

//								* GLOBAL ¿ª SFC∑Œ πŸ≤ﬁ

//	5¬ÅE ∞ªΩ≈ : 2000.06.22	-	* _fileSize ¡¶∞≈
//								* cFILE∏¶ cFILES∑Œ πŸ≤ﬁ
//								* cFILE »≠¿œ ¡¶æÅE≈¨∑°Ω∫ √ﬂ∞°
//								* cNUX »≠¿œ ∫¥«’ ±‚¥… √ﬂ∞°
//								* IsFile √ﬂ∞°
//								* cFILES ≈¨∑°Ω∫ ¡¶∞≈. -.- cFOLDER∞ÅE«’√ƒ πˆ∑»¥Ÿ.


/////////////////////////////////////////////////////////////////////////////////*********/

#ifndef _classSFC_H
#define _classSFC_H
#include <winsock2.H>
//#include <windows.h>
#include <stdio.h>
//#include <dxerr8.h>

#ifdef _DEBUG
#define	dDEBUG_MODE		1
#endif

//#define	dDEBUG_MODE				1

#ifdef dDEBUG_MODE
#define TEST(err,method)        if (err) method;
#define CRY(err,message)        if (err) ERRMSG(message);
#define EXPEL(err)              if (err) goto LABEL_EXILE;
#define DEBUG_ERRMSG            cMSG::Error
#else
#define TEST(a,b)               (void(0))
#define CRY(a,b)                (void(0))
#define EXPEL(err)              (void(0))
#define DEBUG_ERRMSG            (FALSE)
#endif

#define	WM_DEBUGTEXT			(WM_USER+2)

#define	dGET_FILE_NAME			0x480
#define	dGET_FILE_EXT			0x470

#define	d1MEGA					1048576

#define pKILL(buff) if (buff) {delete [] buff	;buff=NULL;}	// ∆˜¿Œ≈Õ ªË¡¶..
#define KILL(buff)	if (buff) {delete buff		;buff=NULL;}	// ≈¨∑°Ω∫ ªË¡¶..
#undef  ATTEMPT
#define ATTEMPT(x)	{ if (FAILED(x)) goto ERROR_LABEL; }		//	ø°∑Ø √≥∏Æ..
#undef  RELEASE
#define RELEASE(x)	{ if (x != NULL) {x->Release(); x = NULL;} }//	DX∞≥√º ªË¡¶..

#define	MSGOUT		cMSG::Output
#define	DXMSGOUT	cMSG::DxOutput
#define	ERRMSG		cMSG::Error
#define	ERRORED		Errored()
#define	SAFETY		Safety()

template <class A> inline void swap(A &a, A &b)
{
	A   c;
    c = a;
    a = b;
    b = c;
}


//	¿ÅE™ ∫ØºˆµÅE
extern	char		g_strLogFolder[1024];					//	«¡∑Œ±◊∑• Ω««ÅE∆˙¥ÅE
extern	BYTE		*g_pTemplateBuffer;
extern	int			g_iTemplateBufferSize;
extern	int			StringToNumber(char *_strText,int _iLimitValue,int _iDecimal);
extern	int			GetLastDay(int _iYear,int _iMonth);

#define	dDEFAULT_TEMPLATE_BUFFER_SIZE	(1024*40)	//	40Kb

inline	void
AllocTemplateBuffer(int _iSize)
{
	if (g_iTemplateBufferSize	>=	_iSize)	return;

	_iSize	=	max(_iSize,dDEFAULT_TEMPLATE_BUFFER_SIZE);

	pKILL(g_pTemplateBuffer);

	g_pTemplateBuffer		=	new BYTE [_iSize];

	g_iTemplateBufferSize	=	_iSize;
}

inline	void
FreeTemplateBuffer()
{
	pKILL(g_pTemplateBuffer);
	g_iTemplateBufferSize	=	0;
}

/*
//	¿ÅE™ «‘ºˆµÅEextern
void		memset16(WORD *dest,WORD value,unsigned int size);	//	16∫Ò∆Æ øÅE.
extern	inline int	random(int range);									//	∑£¥˝«‘ºÅEextern	inline int	largeRandom(int range);									//	∑£¥˝«‘ºÅEextern	inline BOOL	SafeRange(int value,int minValue,int maxValue);		//	πÅEß √º≈©
extern	inline void	RotateValue(int &value,int maxValue);				//	≈©±‚∏¶ √º≈©«ÿº≠ ≈©∏ÅE0¿∏∑Œ..
extern	inline void	ClipValue(int &value,int minValue,int maxValue);	//	≈©±‚∏¶ √º≈©«ÿº≠ ¿⁄∏•¥Ÿ.
extern	inline BOOL	SetLogFolder(){return	SetCurrentDirectory(g_strLogFolder);}
extern	char*		GetTimeString(int _iYear,int _iMonth,int _iDay,int _iHour,int _iMinute,int _iSecond);	//	Ω√∞£ πÆ¿⁄ø≠ æÚ±ÅE
extern	BOOL		_makeShotcut(char* src, char* lnkName, char* lnkPath);	//	ºÙƒ∆¿ª ∏∏µÁ¥Ÿ.
*/

// global functions
extern void memset16(WORD* dest, WORD value, unsigned int size);
extern int random(int range);
extern int largeRandom(int range);
extern BOOL SafeRange(int value, int minValue, int maxValue);
extern void RotateValue(int& value, int maxValue);
extern void ClipValue(int& value, int minValue, int maxValue);
extern BOOL SetLogFolder();
extern char* GetTimeString(int _iYear, int _iMonth, int _iDay, int _iHour, int _iMinute, int _iSecond);
extern BOOL _makeShotcut(char* src, char* lnkName, char* lnkPath);



inline	char* _cdecl _ms(char *str,...)
{	
	static	char	s_tempStr[1024];
	va_list argptr;
	va_start(argptr, str);
	vsprintf(s_tempStr,str,argptr);
	va_end(argptr);

	return s_tempStr;
}

extern	char		*_exportFileName(char *str);			//	»≠¿œ ¿Ã∏ß∏∏ ªÃæ∆ ≥Ω¥Ÿ.
extern	char		*_exportName(char *str);
extern	char		*_exportFolderName(char *str);			//	∆˙¥ÅE¿Ã∏ß∏∏ ªÃæ∆ ≥Ω¥Ÿ.
extern	char		*_exportExt(char *str);					//	»Æ¿Â¿⁄∏∏ ªÃæ∆ ≥Ω¥Ÿ.
extern	void		_setExt(char *srcStr,char *destStr);	//	»Æ¿Â¿⁄∏¶ º≥¡§«—¥Ÿ.
extern	BOOL		IsFolder(char *fn);						//	¿Ã ≥‡ºÆ¿Ã ∆˙¥ı≥ƒ?
extern	BOOL		IsFile(char *fn);						//	¿Ã ≥‡ºÆ »≠¿œ¿Ã≥ƒ?
extern	BOOL		Safety();								//	æ»¿ÅEœ¥Ÿ!!
extern	BOOL		Errored();								//	ø°∑Ø∞° ≥µ¥Ÿ!!
extern	void		EXIT(char *title,char *main,...);					//	∞¡ ≥°≥Ω¥Ÿ.
extern	BOOL		ustrncpy(char *dest,char *src,int size);

inline	int
GetRange(int x1,int y1,int x2,int y2)
{	
	x1	-=	x2;
	y1	-=	y2;

	if (x1 >= 0x7fff || x1 <= -0x7fff)	return	0x7ffffff;
	if (y1 >= 0x7fff || y1 <= -0x7fff)	return	0x7ffffff;

	return	(x1*x1+y1*y1);
}

inline	int
GetOvalRange(int x1,int y1,int x2,int y2)
{	
	x1	-=	x2;
	y1	-=	y2;
	y1	*=	2;

	if (x1 >= 0x7fff || x1 <= -0x7fff)	return	0x7ffffff;
	if (y1 >= 0x7fff || y1 <= -0x7fff)	return	0x7ffffff;

	return	(x1*x1+y1*y1);
}

class	cINDEX
{
public:
	int 	Value;
	int 	Index;
};

extern	int Compare( const void *a, const void *b);

class CPos
{
public:
	int		x,y;

	void	Set(int a,int b)	{x =a,y =b;}
	void	Move(int a,int b)	{x+=a,y+=b;}
	BOOL	Match(CPos *pos);
	BOOL	Match(int a,int b);

	void	AdjustScreen(HWND hwnd);
	void	SetMousePos(HWND hwnd);
	void	SetMousePos();

	CPos	operator=(const CPos & );
	CPos	operator+(const CPos & );
	CPos	operator-(const CPos & );
};

class c4POINT : public CPos
{
public:
	int		mx,my;
};

class cPOINTS
{
public:
	short	x,y;

	void	Set(short a,short b)	{x =a,y =b;}
	void	Move(short a,short b)	{x+=a,y+=b;}
	BOOL	Match(CPos *pos);
	BOOL	Match(short a,short b);

	void	AdjustScreen(HWND hwnd);
	void	SetMousePos(HWND hwnd);
	void	SetMousePos();

	cPOINTS	operator=(const cPOINTS & );
	cPOINTS	operator+(const cPOINTS & );
	cPOINTS	operator-(const cPOINTS & );
};

class cRECT
{
public:
	int		x1,y1,x2,y2;

	cRECT	operator=(const cRECT &src)
	{	x1	=	src.x1;
		y1	=	src.y1;
		x2	=	src.x2;
		y2	=	src.y2;

		return src;
	}

	inline	BOOL	isIn(int x,int y)
	{
		if (x < x1) return FALSE;
		if (x > x2) return FALSE;
		if (y < y1) return FALSE;
		if (y > y2) return FALSE;
		
		return TRUE;
	}

	inline	BOOL	isCrash(cRECT *compare)
	{	if (compare->x1 > x2) return FALSE;
		if (compare->x2 < x1) return FALSE;
		if (compare->y1 > y2) return FALSE;
		if (compare->y2 < y1) return FALSE;

		return TRUE;
	}

	inline	void	set(int a1,int b1,int a2,int b2)
	{	x1	=	a1;
		y1	=	b1;
		x2	=	a2;
		y2	=	b2;
	}

	inline	void	set(cRECT *_lpRect)
	{	
		memcpy(this,_lpRect,sizeof(cRECT));
	}

	inline	void	clientFrame(HWND hwnd)
	{	GetClientRect(hwnd,(RECT *)this);
	}

	inline	void	outFrame(HWND hwnd)
	{	GetWindowRect(hwnd,(RECT *)this);
	}

	inline	void	move(int x,int y)
	{	x2	+=	(x-x1);
		y2	+=	(y-y1);
		x1	=	x;
		y1	=	y;
	}

	inline	void	add(int x,int y)
	{	x1	+=	x;
		y1	+=	y;
		x2	+=	x;
		y2	+=	y;
	}

	inline	void	reform()
	{	if (x1 > x2 ) swap(x1,x2);
		if (y1 > y2 ) swap(y1,y2);
	}

	/*
	inline	void	expand(int _iXS,int _iYS)	//	ªÁ¿Ã¡ÅE≈∞øÅEÅE	{
		x1	-=	_iXS,y1	-=	_iYS,x2	+=	_iXS,y2	+=	_iYS;
	}
	*/

	inline	void	expand(int _iXS, int _iYS)
	{
		x1 -= _iXS;
		y1 -= _iYS;
		x2 += _iXS;
		y2 += _iYS;
	}


	inline	BOOL	isSame(cRECT *_lpRect)
	{
		if (x1==_lpRect->x1 && y1==_lpRect->y1 && x2==_lpRect->x2 && y2==_lpRect->y2) return TRUE;

		return	FALSE;
	}

	int				getRange(int _iX,int _iY,BOOL _bIsOval=FALSE);

	inline	int		getLeft()					{return min(x1,x2);}
	inline	int		getRight()					{return max(x1,x2);}
	inline	int		getTop()					{return min(y1,y2);}
	inline	int		getBottom()					{return max(y1,y2);}

	inline	DWORD	getWidth()					{return getRight()-getLeft()+1;}
	inline	DWORD	getHeight()					{return getBottom()-getTop()+1;}
	inline	DWORD	getSize()					{return getWidth()*getHeight();}

	inline	void	getCenter(int &a,int &b)	{a=getLeft()+getWidth()/2,b=getTop()+getHeight()/2;}
	inline	void	getCenter(CPos	*_lpPos)	{_lpPos->x=getLeft()+getWidth()/2,_lpPos->y=getTop()+getHeight()/2;}
};

class cRECTS
{
public:
	short	x1,y1,x2,y2;

	inline	BOOL	isIn(int x,int y)
	{
		if (x < x1) return FALSE;
		if (x > x2) return FALSE;
		if (y < y1) return FALSE;
		if (y > y2) return FALSE;
		
		return TRUE;
	}

	inline	BOOL	isCrash(cRECTS *compare)
	{	if (compare->x1 > x2) return FALSE;
		if (compare->x2 < x1) return FALSE;
		if (compare->y1 > y2) return FALSE;
		if (compare->y2 < y1) return FALSE;

		return TRUE;
	}

	inline	void	set(short a1,short b1,short a2,short b2)
	{	x1	=	a1;
		y1	=	b1;
		x2	=	a2;
		y2	=	b2;

		if (x1 > x2) swap(x1,x2);
		if (y1 > y2) swap(x1,x2);
	}

	inline	void	move(short x,short y)
	{	x2	+=	(x-x1);
		y2	+=	(y-y1);
		x1	=	x;
		y1	=	y;
	}

	inline	void	add(short x,short y)
	{	x1	+=	x;
		y1	+=	y;
		x2	+=	x;
		y2	+=	y;
	}

	inline	void	reform()
	{	if (x1 > x2 ) swap(x1,x2);
		if (y1 > y2 ) swap(y1,y2);
	}

	inline	cRECTS	operator=(const cRECTS &src)
	{	x1	=	src.x1;
		y1	=	src.y1;
		x2	=	src.x2;
		y2	=	src.y2;

		return src;
	}

	inline	short	getLeft()					{return min(x1,x2);}
	inline	short	getRight()					{return max(x1,x2);}
	inline	short	getTop()					{return min(y1,y2);}
	inline	short	getBottom()					{return max(y1,y2);}

	inline	DWORD	getWidth()					{return getRight()-getLeft()+1;}
	inline	DWORD	getHeight()					{return getBottom()-getTop()+1;}
	inline	DWORD	getSize()					{return getWidth()*getHeight();}

	inline	void	getCenter(short &a,short &b)	{a=getLeft()+(getRight()-getLeft())/2,b=getTop()+(getBottom()-getTop())/2;}
};

class	cCRC32
{
	static	DWORD			s_adwCrc32Table[256];

	static	inline	void	CalcCrc32(const BYTE byte, DWORD &dwCrc32);
public:
	static	BOOL			StringToCrc32(char *_lpstrString,DWORD &_dwCrc32);
	static	BOOL			BufferToCrc32(void *_lpBuffer,int _iBufferSize,DWORD &_dwCrc32);
	static	BOOL			FileToCrc32(char *_lpstrFileName,DWORD &dwCrc32);

};

class cSTRING
{
public:
	char			*String;
	int				Length;					//	πÆ¿Â¿« ªÁ¿Ã¡ÅE	
	int				Value;
	cSTRING			*pPRE,*pNEXT;			//	¿Ã¿ÅE¿Ã»ƒ

					cSTRING();
					cSTRING(char *str);
					~cSTRING();

	void			Reset();
	
	char*	_cdecl	Set(char *str,...);
	char*	_cdecl	Add(char *str,...);
	char*	_cdecl	Insert(char *str,...);
	BOOL			Match(char *str);
	void			Minus(char *minus);
	void			SetValue(int value);

	cSTRING			operator=(const cSTRING & );
	cSTRING			operator+(const cSTRING & );
	cSTRING			operator-(const cSTRING & );

	static	char*	Minus(char *str,char *minus);
	static	char*	UpperCase(char *str);
	static	char*	LowCase(char *str);
};

class	CStream
{
public:
	char			*m_pBuffer;
	DWORD			m_dwBufferSize;
	DWORD			m_dwBufferPoint;

					CStream()
					{
						m_pBuffer	=	NULL;
					}

					~CStream()
					{
						close();
					}

	void			init(DWORD _dwSize = 1024*10);
	void			reset();
	void			close();

	BOOL			addStringP(char *_lpstrText,...);
	BOOL			addString(char *_lpstrText);
	BOOL			writeToFile(char *_lpstrFileName,BOOL _bIsReset);
};

#define	dSORT_BY_EXT	0
#define	dSORT_BY_NAME	1
#define	dSORT_ASC		0
#define	dSORT_DESC		1
#define	dSORT_NOT		2

class cSTRINGS
{
public:
	int			maxString,Count;	//	«—∞ÅEπÆ¿ÂºˆøÕ «ˆ¿ÅEπÆ¿ÂºÅE	
	cSTRING		*pHEAD,*pTAIL;		//	√≥¿Ω∞ÅE∏∂¡ˆ∏∑
	int			CurString;			//	«ˆ¿ÅEπÆ¿ÅE
				cSTRINGS(int count = 0xffff);
				~cSTRINGS();

	void		Init(int count);
	void		Reset();

	BOOL		Add		(char *str,int Value=0,BOOL check=FALSE);
	BOOL		Add		(cSTRINGS *str);
	BOOL		Remove	(int index);
	BOOL		Remove	(char *str);
	cSTRING*	Pick	(int index=0);
	char*		GetStr	(int index=0);
	int			GetIndex(char *str);

	BOOL		Sort	(int term=dSORT_ASC);

	static	int	_cdecl	Compare( const void *a, const void *b);
};

#define	dMAX_FILE_LEN		60

class	cFILENAME
{
public:
	char	Name[dMAX_FILE_LEN];
	int		m_iValue;
	BYTE	isFolder;

			~cFILENAME()	{};
	void	Set(char *name,BOOL folder)
			{	strcpy(Name,name);
				isFolder	=	folder;
			}
};

class	cFILE
{
public:
	FILE				*fp;
	DWORD				frontPos,endPos,Length;
	BOOL				isMerge;
	WORD				m_wIsFFlush;

						cFILE();
						~cFILE();

	BOOL				Open(char *fn,char *mode,BOOL _bIsFFlush=FALSE);
	BOOL				Close();
	BOOL				Seek(int pos,int seekTerm=SEEK_CUR);
	DWORD				Locate();
	BOOL				Read(void *buffer,DWORD size,DWORD itemSize=1);
	BOOL				Write(void *buffer,DWORD size,DWORD itemSize=1);

	BOOL				writeString(char *_lpstrString);
	BOOL	_cdecl		writeStreamCR(char *str,...);
	BOOL	_cdecl		writeStream(char *str,...);
	BOOL				writeStreamNP(char *str,BOOL _bIsPutReturn=TRUE);
	BOOL				Skip(int skip);
	BOOL				Rewind();			//	»≠¿œ √≥¿Ω¿∏∑Œ
	BOOL				End();				//	»≠¿œ ≥°¿∏∑Œ
	BOOL				IsOpen();
	BOOL				Set(cFILE *file,DWORD size);
	BOOL				Copy(char *fn);

	static	DWORD		Size(char *fn);
	static	DWORD		Size(FILE *tfp);
	static	BOOL		Burn(char *fn,char *buff,DWORD size);
};

class	cFOLDER
{
public:
	static	WORD		SortTerm;

	static	void		SetSort			(WORD	sort)
						{	SortTerm	=	sort;
							if (SortTerm < 0        ) SortTerm = 0;
							if (SortTerm > dSORT_NOT) SortTerm = dSORT_NOT;
						}

	static	BOOL		Delete			(char *delDir);				//	∆˙¥ı¿« »≠¿œ¿ª ∫Ò∑‘«— ∏µÁ∞… ¡ˆø˚–Ÿ.(¡ˆøÅEÅEæ¯¥¬ ∞≈∏ÅEº”º∫¿ª πŸ≤„º≠ ¡ˆø˚–Ÿ.)
	static	BOOL		Create			(char *folder,BOOL changeFolder=FALSE);				//	∆˙¥ı∏¶ ∏∏µÁ¥Ÿ.
	static	BOOL		Change			(char *folder);										//	∆˙¥ı∏¶ ¿Ãµø«—¥Ÿ.

	static	BOOL		getFolders		(char *root,cSTRINGS *folders);		//	∆˙¥ıµÈ¿ª æÚæÅEø¬¥Ÿ.
	static	cSTRINGS*	GetFolders		(char *root);		//	∆˙¥ıµÈ¿ª æÚæÅEø¬¥Ÿ.
	static	cSTRINGS*	GetFiles		(char *ext="*.*",char *root=NULL,BOOL includeFolder=FALSE,WORD sort=dSORT_BY_EXT);//	»≠¿œµÈ¿ª æÚæÅEø¬¥Ÿ.
	static	cSTRINGS*	GetFiles		(WORD sort);																//	»≠¿œµÈ¿ª æÚæÅEø¬¥Ÿ.

	static	DWORD		GetFileCount	(char *ext="*.*",char *folder=NULL,BOOL includeFolder=FALSE,BOOL includeSub=FALSE);	//	∆˙¥ıæ»ø° »≠¿œ¿« ºÅE	
	static	DWORD		GetFolderCount	(char *folder=NULL);										//	∆˙¥ı¿« ºÅE	
	static	DWORD		GetFolderSize	(char *ext="*.*",char *root=NULL,BOOL includeSub=FALSE);	//	∆˙¥ıæ»ø° »≠¿œµÈ¿« ªÁ¿Ã¡ÅE
	static	int	_cdecl	Compare			(const void *a, const void *b);
	static	char		*Tree(HWND hwnd,BOOL change,char *title,char *folder=NULL,char *dest=NULL);	//	∆˙¥ı∆Æ∏Æ Ω©
};

//	æ–√ÅE»≠¿œ ∫¥«’ ∞ÅE√..
#define	dMAX_MERGE_FILE	2046

class cMERGEINFO
{
public:
	char	FName[40];
	DWORD	Locate;
	DWORD	Size;
};

class	cNUX		//	∂Û∆ææ˚”Œ »£µŒ∂Û¥¬ ∂ÅEªÁøÅE√ cNUX.cpp∏¶ ∏µ≈©«ÿæﬂ «—¥Ÿ. 
{
public:
	cFILE				File;
	cMERGEINFO			*INDEX;
	DWORD				AppendPos,Count;

						cNUX();
						~cNUX();

	BOOL				Load(char *fn);		//	ø¨¥Ÿ.
	BOOL				Close();

	cFILE*				Open(char *fn);
	cFILE*				Open(DWORD index);

	BOOL				Export(char *fn);
	inline	cMERGEINFO	*Seek(char *fn);

	static	BOOL		bSTOP;

	static	void		(*ProcessExtract)();
	static	void		(*ProcessPress)();
	static	int	_cdecl	Compare_Name( const void *a, const void *b);

	static	DWORD		TotalSize,FileSize,FileCount,ProcessRate,ProcessSize,FileRate;
	static	char		CurrentFile[128];		//	«ˆ¿ÅEæ–√‡«œ∞ÅE¿÷¥¬ »≠¿œ »§¿∫ æ–√‡¿ª «Æ∞ÅE¿÷¥¬ »≠¿œ
	static	char		ArcName[128];			//	æ–√ÅE»≠¿œ

	static	BOOL		PressCreate(char *fn);															//	ªı∑ŒøÅEæ–√‡»≠¿œ ª˝º∫
	static	BOOL		PressAppend(char *fn);
	static	BOOL		PressFile(char *fn);															//	»≠¿œ √ﬂ∞°
	static	void		PressAllFile();																	//	«ˆ¿ÅE∆˙¥ıø° ∏µÅE»≠¿œ¿ª √ﬂ∞°«—¥Ÿ.
	static	BOOL		PressFolder(char *fn,BOOL includeCurrent=FALSE,BOOL includeSubFolder=FALSE);	//	∆˙¥ıæ»ø° ∏µÅE»≠¿œ¿ª √ﬂ∞°
	static	DWORD		PressClose();																	//	æ–√‡»≠¿œ ¥›±ÅE	
	static	void		Stop();																			//	¿€æ˜¿ª ¡ﬂ¡ˆ«—¥Ÿ.
	static	BOOL		PressExtract(char *fn,char *destFolder,cSTRINGS *files=NULL);					//	æ–√‡»≠¿œ «Æ±ÅE
	//	∫ÅEæ–√ÅE«ÅEÅE8Byte(»≠¿œ ºˆøÕ «ÅEÅE¿ßƒ°)
	static	BOOL		MergeCreate(char *fn);														
	static	BOOL		MergeAppend(char *fn);
	static	BOOL		MergeFile(char *fn);
	static	BOOL		MergeClose();
	static	BOOL		MergeExtract(char *fn,char *destFolder);

	static	void		SetFunc(void (*press)(),void (*extract)());
};

#pragma comment(lib,"ZipArchive.lib")

extern	int	ZipCompress(unsigned char * _lpSourcedata, unsigned char *_lpTargetData,int _iDataSize);
extern	int	ZipUncompress(unsigned char * _lpSourcedata, unsigned char *_lpTargetData,int _iDataSize,int _iSourceSize);

class cMSG
{
private:
	static	char				msgBuffer[1024];
	static	char*				DXERROR(HRESULT hErr);
	static	HWND				hDEBUGWND;

public:

	static	int _cdecl	YESNO(char *headmsg,char *mainmsg,...);
	static	int _cdecl	YESNOCANCEL(char *headmsg,char *mainmsg,...);
	static	void		Test(int num=-1);

	static	void		Init();
	static	void		Close();
	static	BOOL _cdecl	Put	 (HWND hwnd,char *msg);
	static	BOOL _cdecl	Put  (char *headmsg,char *mainmsg=NULL,...);
	static	BOOL _cdecl	Output(char *msg,...);
	static	BOOL		Output(HRESULT result){return Output(DXERROR(result));}

	static	BOOL		Output(HRESULT result,char *where)
						{	return Output(_ms("%s-%s",where,DXERROR(result)));
						}
	static	BOOL		DxOutput(char *where,HRESULT result)
						{	return Output(_ms("%s-%s",where,DXERROR(result)));
						}

	static	BOOL _cdecl Error(char *headmsg,char *mainmsg=NULL,...);
	static	BOOL		Error(HRESULT result){return Error(DXERROR(result));}
	static	BOOL		Error(HRESULT result,char *where)
						{	return Error(where,DXERROR(result));
						}
	static	BOOL		Error(char *where,HRESULT result)
						{	return Error(where,DXERROR(result));
						}
};


class CTimeInfo
{
public:
	WORD	m_wYear,m_wMonth,m_wDay,m_wHour,m_wMinute,m_wSecond,m_wWeekDay;
	static	char	s_strText[60];
	static	char	s_strText2[60];

			CTimeInfo();

	void	update();
	void	increaseMinute(int _iMinute);
	void	increaseHour(int _iHour);
	void	increaseDay(int _iDay);
	void	increaseSecond(int _iSecond);

	int		getGapDay(CTimeInfo *_lpTime);

	int		getMinuteValue(int _iCorrectYear);

	static	char*	GetTime(BOOL _bIsYear=TRUE,BOOL _bIsMonth=TRUE,BOOL _bIsDay=TRUE,BOOL _bIsHour=TRUE,BOOL _bIsMinute=TRUE,BOOL _bIsSecond=TRUE);
	static	char*	GetTime2();
	static	char*	GetTimeForLog();
};


//////////////////////////////////////////////////////////////////////////////////

//	class	cTIMER	:	≈∏¿Ã∏” √≥∏Æ

//////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib,"winmm.lib")

class cTIMER
{
public:
	static	LONGLONG	s_llTps,s_llLastTime;			//	tps √ ¥ÅE∏˚’¯¿Ã≥™ ∆¢¥¬∞°!!??
	static	int			s_iFps,s_iFrameCounter;	//	FPS √¯¡§¿ª ¿ß«— ºˆƒ°

	LONGLONG			m_llTick;		//	¿Ã∏ß ±◊¥ÅEŒ.. Frame Per Second
	LONGLONG			m_llOldTime;		//	«¡∑π¿” Ω∫≈∞«Œ¿ª ¿ß«—.. Ω√∞£..
	LONGLONG			m_llNewTime;
	LONGLONG			m_llOverTick;
	LONGLONG			m_llTimerStartTime;
	double				m_dblTick;

	int					m_iSkipCount;	//	«¡∑π¿” Ω∫≈µ«— »ΩºÅE

						cTIMER();

	void				startTimer();
	int					stopTimer();
	BOOL				setFPS(int fps);	//	√ ¥ÅE∏˚„¡∑π¿”¿∏∑Œ µπ∏±±˚€ÅE?
	BOOL				isSkipFrame();
	void				resetFrameSkip();

	static	inline	int	getMiliSec(LONGLONG _llTime)
	{
		_llTime	=	_llTime*1000000/s_llTps;

		return (int)_llTime;
	}

	static	inline	int	GetTime()
	{
		LARGE_INTEGER	time;

		QueryPerformanceCounter( &time );

		return (int)(time.LowPart>>2);	//	32∫Ò∆Æ «Æ∑Œ πﬁ¿∏∏ÅE∞ËªÅEœ±ÅE¬Ø≥™¥œ±ÅE. 30∫Ò∆Æ∑Œ πŸ≤€¥Ÿ..
										//	ªÁ¿ŒµÅE«√∑°±‚∞° æ˚ﬁº±∏ ¿˙¬º±∏.. ±√Ω√∑∑..
	}
	static	inline	int	GetFPS(){return s_iFps;}
	static	int			UpdateFPS();
};

///////////////////////////////////////////////////////////////////////////////////
//	∑Œ±◊ »≠¿œ ∞ÅE√
///////////////////////////////////////////////////////////////////////////////////

class	CLogFile
{
public:
	char				m_strFileName[64];
	DWORD				m_dwNameHashCode;
	cFILE				m_file;
	CRITICAL_SECTION	m_csLogFile;

	void				init(char *_lpstrFileName,char *_lpstrLogFolder=NULL);
	void				close();
	void	_cdecl		writeLog(char *_lpstrMsg,...);
	void	_cdecl		writeLogWithoutTime(char *_lpstrMsg,...);
	void				_writeLog(char *_lpstrMsg,BOOL _bIsWantCarraigeReturn=TRUE);
	void				_writeLogWithoutTime(char *_lpstrMsg,BOOL _bIsWantCarraigeReturn=TRUE);
};

#define	dMAX_LOG_FILE_COUNT	100

class	CLogManager
{
	int					getLogFile(char *_lpstrFileName);
	char				*m_lpstrLogFile;
public:
	int					m_iLogFileCount,m_iCommonLog;
	CLogFile			m_aLogFile[dMAX_LOG_FILE_COUNT];
	char				m_strLogFolder[32];

						CLogManager();
						~CLogManager();

	void				setLogFolderName(char *_lpstrFolder);
	int		_cdecl		writeLogWithoutTime(char *_lpstrFileName,char *_lpstrMsg,...);
	int		_cdecl		writeLog(char *_lpstrFileName,char *_lpstrMsg,...);
	void	_cdecl		writeLog(int _iFileIndex,char *_lpstrMsg,...);
	int		_cdecl		writeLog2(char *_lpstrFileName,char *_lpstrMsg,...);
	int					add(char *_lpstrFileName);
};

extern	CLogManager	g_log;

#define	CLOG	g_log.writeLog
#define	CLOGWT	g_log.writeLogWithoutTime
#define	CTLOG	g_log.writeLog2

//////////////////////////////////////////////////////////////////////////////////

//	class	cMCU	:	∏ﬁ∏∏Æ ∞ÅEÆ

//////////////////////////////////////////////////////////////////////////////////
extern	DWORD	GetTotalMemory();
extern	DWORD	GetAvailMemory();

class cMCU
{
public:

	DWORD			m_dwFullSize,m_dwFrontPointer,m_dwRearPointer,m_dwTempAlloc;
	BOOL			m_isRearStack;//µ⁄ø°º≠ ∫Œ≈Õ Ω◊¿Ã¥¬ ≥—¿Ã¥Ÿ.
	char			*m_pMemory;
	cMCU			*m_lpReference;

	char			m_strName[64];
#ifdef	dDEBUG_MODE
	DWORD			m_dwaLog[102400],m_dwLogCount;
#endif

					cMCU();
					~cMCU();

	BOOL			init(DWORD size,char *name);
	BOOL			init(cMCU *_lpMcu,char *name,BOOL _isRearStack);
	void			close();							//	∏ﬁ∏∏Æ∏¶ ¿€ªÅE_- ≥Ω¥Ÿ.
	void			reset();

#ifdef	dDEBUG_MODE
	void*			alloc(DWORD size,char *_lpstrName,BOOL _isRearStack=FALSE);
	void*			tempAlloc(DWORD size,char *name);
	void			check(DWORD address);
#else
	void*			alloc(DWORD size,BOOL _isRearStack=FALSE);
	void*			tempAlloc(DWORD size,char *name="temp");
#endif
	void			tempFree();
	DWORD			getRemainMemorySize();
	DWORD			getFilledMemorySize();

};

//////////////////////////////////////////////////////////////////////////////////
//	class	cDen	:	º“±º-_-
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//	class	cDen	:	º“±º-_-
//////////////////////////////////////////////////////////////////////////////////
template <class T>
class	cDen	//	Ω√∏ÆæÛ¿Ã 16∫Ò∆Æ¥Ÿ.
{
	T					*m_pList;
	T					*m_lpList;
	WORD				*m_pOrderingBuffer;
	int					m_iLimitCount;
	int					m_iCount,m_iRookie;

public:
						cDen()
						{
							m_pList				=	NULL;
							m_iLimitCount		=	0;
							m_iCount			=	0;
							m_iRookie			=	0;
							m_pOrderingBuffer	=	NULL;
						}

						~cDen()
						{
							close();
						}

	void				init(int _iCount,BOOL _bIsOrdering=FALSE)//√ ±‚º≥¡§
	{
		close();

		m_pList			=	new T [_iCount];
		m_lpList		=	m_pList;
		m_iLimitCount	=	_iCount;

		if (_bIsOrdering)
		{
			m_pOrderingBuffer=	new WORD [_iCount];

			memset(m_pOrderingBuffer,0xff,sizeof(WORD)*_iCount);
		}
	}

	void				init(T *_lpList,int _iCount,BOOL _bIsOrdering=FALSE)//√ ±‚º≥¡§
	{
		close();

		m_lpList		=	_lpList;
		m_iLimitCount	=	_iCount;

		if (_bIsOrdering)
		{
			m_pOrderingBuffer=	new WORD [_iCount];

			memset(m_pOrderingBuffer,0xff,sizeof(WORD)*_iCount);
		}
	}

	inline	void		reset()//∏Æº¬
	{
		m_iCount		=	0;
		m_iRookie		=	0;

		if (m_pList)
			for (int i=0;i<m_iLimitCount;i++)
				m_lpList[i].reset();
	}

	void				close()//¥›±ÅE
	{
		pKILL(m_pList);
		pKILL(m_pOrderingBuffer);

		m_lpList		=	NULL;
		reset();
	}

	inline	T*			get(int _iSerial)//_iIndexπ¯¬∞ ∏‚πÅE∏Æ≈œ
	{
		if (_iSerial						>=	m_iLimitCount	)
			return	NULL;
		if (m_lpList[_iSerial].m_wSerial	==	0xffff			)
			return	NULL;

		return	&m_lpList[_iSerial];
	}
	inline	T*			getByOrdering(int _iIndex)	//	º¯¬˜¿˚¿∏∑Œ ∏µÁ∞… ∞Àªˆ«“∂ß ªÁøÅE
	{
		if (_iIndex	>=	m_iCount	||	_iIndex	<	0)
			return	NULL;

		return	&m_lpList[m_pOrderingBuffer[_iIndex]];
	}
	inline	T*			notCheckedget(int _iIndex)	//_iIndexπ¯¬∞ ∏‚πÅE∏Æ≈œ
	{
		return	&m_lpList[_iIndex];
	}
	inline	T*			getList()			{return	m_lpList;}//∏ÆΩ∫∆Æ ¿⁄√º∏¶ ∏Æ≈œ
	inline	int			getCount()			{return	m_iCount;}//«ˆ¿ÅEƒ´ø˚‚Æ
	inline	int			getMaxCount()		{return	m_iLimitCount;}//√÷¥ÅE¿Œø¯ºÅE
	int					add(T* _lpData)//∏‚πÅE√ﬂ∞°
	{
		if (m_iCount	>=	m_iLimitCount)
			return	0xffff;

		int	iIndex	=	m_iRookie;

		m_lpList[m_iRookie].copy(_lpData);
		m_lpList[m_iRookie].m_wSerial	=	m_iRookie;

		if (m_pOrderingBuffer)
			m_pOrderingBuffer[m_iCount]	=	m_iRookie;

		m_iCount++;

		for (int i=m_iRookie+1;i<m_iLimitCount;i++)
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;

				return	iIndex;
			}

		return	iIndex;
	}

	inline	T*			getRookie(int *_lpSerial)//∑Á≈∞
	{
		if (m_iCount	>=	m_iLimitCount)
			return	NULL;

		*_lpSerial	=	m_iRookie;

		return	&m_lpList[m_iRookie];
	}

	inline	int			getRookieSerial()//∑Á≈∞ Ω√∏ÆæÅE∏Æ≈œ
	{
		if (m_iCount	>=	m_iLimitCount)
			return	0xffff;

		return	m_iRookie;
	}

	inline	void		increaseCount()//ƒ´ø˚‚Æ ¡ı∞°
	{
		if (m_lpList[m_iRookie].m_wSerial	==	0xffff)
			return;

		if (m_pOrderingBuffer)
			m_pOrderingBuffer[m_iCount]	=	m_iRookie;

		m_iCount++;

		for (int i=m_iRookie+1;i<m_iLimitCount;i++)
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;

				break;
			}
	}

	BOOL				remove(int _iIndex)//∏‚πÅE¡¶∞≈
	{
		if (m_lpList[_iIndex].m_wSerial	==	0xffff)
			return	FALSE;

		m_lpList[_iIndex].reset();

		if (m_pOrderingBuffer)
			for (int i=0;i<m_iCount;i++)
				if	(m_pOrderingBuffer[i]	==	_iIndex)
				{
					m_pOrderingBuffer[i]	=	m_pOrderingBuffer[m_iCount-1];
					break;
				}

		if (_iIndex	<	m_iRookie)
			m_iRookie	=	_iIndex;

		m_iCount--;

		return	TRUE;
	}

	BOOL				insert(int _iSerial,T *_lpData)//∆Ø¡§ ¿Œµ¶Ω∫ø° ∏‚πÅE√ﬂ∞°
	{
		if (m_lpList[_iSerial].m_wSerial	!=	0xffff)
			return	FALSE;

		m_lpList[_iSerial].copy(_lpData);
		m_lpList[_iSerial].m_wSerial	=	_iSerial;

		if (m_pOrderingBuffer)
			m_pOrderingBuffer[m_iCount]	=	_iSerial;

		m_iCount++;

		for (int i=m_iRookie;i<m_iLimitCount;i++)
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;

				return	TRUE;
			}

		return	TRUE;
	}

	void			rebuild()
	{
		int	i;

		m_iRookie	=	-1;
		m_iCount	=	0;

		for (i=0;i<m_iLimitCount;i++)
		{
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				if	(m_iRookie	==	-1)
					m_iRookie	=	i;

				continue;
			}

			m_iCount++;
		}

		if (m_pOrderingBuffer)
		{
			m_iCount	=	0;

			for (i=0;i<m_iLimitCount;i++)
			{
				if (m_lpList[i].m_wSerial	==	0xffff)
					continue;

				m_pOrderingBuffer[m_iCount++]	=	i;
			}
		}
	}
};

//////////////////////////////////////////////////////////////////////////////////
//	æœ»£»≠
//////////////////////////////////////////////////////////////////////////////////

DWORD	EncodeBuffer(int _iSeedValue,BYTE *_lpBuffer,int _iBufferSize);
DWORD	DecodeBuffer(int _iSeedValue,BYTE *_lpBuffer,int _iBufferSize);

//////////////////////////////////////////////////////////////////////////////////
//	class	cTHREAD	:	æ≤∑πµÅE//////////////////////////////////////////////////////////////////////////////////

class cTHREAD
{
public:
	HANDLE		ThreadHandle;	// Ω∫∑πµÅE«⁄µÅE	
	DWORD		ThreadAddr;     // Ω∫∑πµÅE¡÷º“

				cTHREAD();
				~cTHREAD();

	void		Create(HWND hwnd,void (__cdecl *func)(void *),void *_lpData=NULL);
	void		Close();

};

//////////////////////////////////////////////////////////////////////////////////

//	µπˆ±ÅEÅEƒ⁄µÂµÅE ¡ª ¡ˆ¿˙∫– «ÿµµ ∏≈øÅE¡ﬂø‰«œ¥œ ¬ÅE∆∂ÅE --;;

//////////////////////////////////////////////////////////////////////////////////
#define	dCHECK_PTR_HEAD			0x12345678					//	∆˜¿Œ≈Õ∏¶ √º≈©«—¥Ÿ. «ÅEÅE
#define	dCHECK_PTR_TAIL			0x87654321					//	∆˜¿Œ≈Õ∏¶ √º≈©«—¥Ÿ. ∏∂π´∏Æ
#define	dCHECK_MCU_HEAD			dCHECK_PTR_TAIL				//	MCU¿« ∆˜¿Œ≈Õ∏¶ √º≈©«—¥Ÿ. «ÅEÅE
#define	dCHECK_MCU_TAIL			dCHECK_PTR_HEAD				//	MCU¿« ∆˜¿Œ≈Õ∏¶ √º≈©«—¥Ÿ. ∏∂π´∏Æ
#define	dPTR_STRING_SIZE		64							//	∆˜¿Œ≈Õ ¿Ã∏ß ªÁ¿Ã¡ÅE
#ifdef dDEBUG_MODE			//	µπˆ±ÅE∏µÂ≥ƒ?
//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠#ifdef dDEBUG_MODE	µπˆ±ÅE∏µÂ¥Ÿ.
#define	dCHECK_POINTER_SIZE		(64+12)						//	∆˜¿Œ≈Õ ¿Ã∏ß ªÁ¿Ã¡ÅE
//	∏ﬁ∏∏Æ∏¶ º≥¡§ ∏≈≈©∑Œ. type «ÅEnumbers ∞πºÅEmcu cMCU ∆˜¿Œ≈Õ NULL¿Ã∏ÅE±◊≥… ∏ﬁ∏∏Æø° º≥¡§«—¥Ÿ. name µπˆ±ÅEÅE∆˜¿Œ≈Õ ¿Ã∏ß
#define	_New(type,numbers,mcu,name)		(type *)safeMalloc((numbers)*sizeof(type),mcu,name)

//	∏ﬁ∏∏Æ∏¶ «ÿ¡¶ ∏≈≈©∑Œ. ¿œ¥‹ ¿÷¥¬¡ÅE?) »Æ¿Œ«œ∞ÅE¿÷¿∏∏ÅE√º≈©«— ¥Ÿ¿Ωø° «ÿ¡¶«œ∞ÅE∆˜¿Œ≈Õ∏¶ √ ±‚»≠ Ω√≈≤¥Ÿ.
#define	_Free(ptr)								\
if (ptr)										\
{	_checkPtr((char *)ptr);						\
	free((char *)(ptr)-(dPTR_STRING_SIZE+8));	\
	ptr		=	NULL;							\
}

//	∏ﬁ∏∏Æ∏¶ º≥¡§ «‘ºÅE type «ÅEnumbers ∞πºÅEmcu cMCU ∆˜¿Œ≈Õ NULL¿Ã∏ÅE±◊≥… ∏ﬁ∏∏Æø° º≥¡§«—¥Ÿ. name µπˆ±ÅEÅE∆˜¿Œ≈Õ ¿Ã∏ß
inline void*  safeMalloc(unsigned int size,cMCU *mcu,char *name)
{	if (mcu)
	{	void	*ptr	=	mcu->alloc(size,name);

		return	ptr;
	}

	char	*ptr	=	(char *)malloc(size+4+4+4+dPTR_STRING_SIZE);	//	size + head(4) + size(4) + str(64) + tail(4)

	*(unsigned int *)(ptr							)	=	dCHECK_PTR_HEAD;			//	«ÅEÅE	
	*(unsigned int *)(ptr+4							)	=	(size+8+dPTR_STRING_SIZE);	//	≤ø∏Æ ¿ßƒ°
	*(unsigned int *)(ptr+8+dPTR_STRING_SIZE+size	)	=	dCHECK_PTR_TAIL;			//	≤ø∏Æ
	strncpy((char *)(ptr+8),name,dPTR_STRING_SIZE-1);										//	¿Ã∏ß

	ptr	+=	(dPTR_STRING_SIZE+8);

	return ptr;
};

//	∏ﬁ∏∏Æ∏¶ √º≈© «‘ºÅE ∏ﬁ∏∏Æ∏¶ «ÿ¡¶ «œ±ÅE¿ÅE° p¿« æ’µ⁄∏¶ ∞ÀªÁ«ÿ ±˙¡ˆ¡ÅEæ æ“¥¬¡ÅE»Æ¿Œ«—¥Ÿ.
inline void _checkPtr(char *p)
{	unsigned int	head,tail,size;

	char *ptr	=	p-(dPTR_STRING_SIZE+8);

	head		=	*(DWORD *)(ptr		);		//	«ÅEÅE
	size		=	*(DWORD *)(ptr+4	);		//	«ÅEÅE
	tail		=	*(DWORD *)(ptr+size	);		//	«ÅEÅE

	if (head	!= dCHECK_PTR_HEAD)
	{	if (head	== dCHECK_MCU_HEAD)
				ERRMSG(_ms("Error!! pointer %s are cMCU's Client!!",ptr+8));
		else	ERRMSG(_ms("Error!! pointer %s's HEAD offset damaged!!",ptr+8));
	}

	if (tail	!= dCHECK_PTR_TAIL)	ERRMSG(_ms("Error!! pointer %s's TAIL offset damaged!!",ptr+8));
};
//¶≠#ifdef dDEBUG_MODE	µπˆ±ÅE∏µÅE//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

#else	
//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠#ifdef dDEBUG_MODE	µπˆ±ÅE∏µÂ∞° æ∆¥œ¥Ÿ.
#define	dCHECK_POINTER_SIZE		0						//	∆˜¿Œ≈Õ ¿Ã∏ß ªÁ¿Ã¡ÅE
//	ø°∑Ø√º≈© æ»«—¥Ÿ.
//	∏ﬁ∏∏Æ «“¥ÅE∏≈≈©∑Œ... mcu ¿Ø¥÷¿Ã ¿÷¥Ÿ∏ÅE±◊ ≥—«—≈◊ º≥¡§«œ∞ÅEæ∆¥œ∏ÅE.. ∞¡ «—¥Ÿ.
#define	_New(type,numbers,mcu,c)	(type *)safeMalloc((numbers)*sizeof(type),mcu)
inline void*  safeMalloc(unsigned int size,cMCU *mcu)
{	void*	ptr;

	if (mcu)
	{	ptr	=	mcu->alloc(size);

		return ptr;
	}

	ptr		=	(void *)malloc(size);

	return ptr;
}

//	∏ﬁ∏∏Æ «ÿ¡¶ ∏≈≈©∑Œ... ¿Ã ∏ﬁ≈©∑Œ∞° ¬ÅE∂«— ∏≈≈©∑Œ¿Œ¡ÅE»Æ¿Œ«—¥Ÿ.
#define	_Free(ptr)											\
if (ptr)													\
{	if (*(DWORD*)((char*)(ptr)-4)	==	dCHECK_PTR_TAIL)	\
	{	ptr	=	NULL;										\
		ERRMSG("Error!! cMCU's Inheritor are destroy!!");	\
	}														\
	free(ptr);												\
	ptr	=	NULL;											\
}


#endif
//¶≠#ifdef dDEBUG_MODE	µπˆ±ÅE∏µÂ∞° æ∆¥œ¥Ÿ.
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

inline unsigned long
GetHashCode(BYTE* _lpString)
{
	int	iResult=0,iXOR=0,iCH=0,i=0;

	for (;_lpString[i];i++)
	{
		iCH		=	_lpString[i];

		if (iCH >= 'a' && iCH <= 'z')
			iCH	-=	('a'-'A');

		iResult	=	( ( iResult << 8 ) | iCH ) % 16777213UL; // 16777213UL
		iXOR		^=	iCH;
	}

	return iResult | (iXOR<<24);
}

inline unsigned long
GetHashCode(char* _lpString)
{
	return	GetHashCode((BYTE*) _lpString);
}

#ifdef	_FOR_JAPAN
	#ifndef	STRICMP
		#define	STRICMP	lstrcmpi
	#endif	
#elif	_FOR_THAI
	#ifndef	STRICMP
		#define	STRICMP	lstrcmpi
	#endif	
#else
	#ifndef	STRICMP
		#define	STRICMP	stricmp
	#endif	
#endif

#ifndef	STRCMP
#define	STRCMP	STRICMP
#endif

#define	mCS(cs)	CCriticalSection c_s(&cs)	//	≈©∏Æ∆ºƒ√ ºΩº« Ω√¿€

class	CCriticalSection
{
public:
	CRITICAL_SECTION	*m_lpCs;

	CCriticalSection(CRITICAL_SECTION * _lpCs)
	{
		m_lpCs	=	_lpCs;
		EnterCriticalSection(m_lpCs);
	}

	~CCriticalSection()
	{
		LeaveCriticalSection(m_lpCs);
	}
};

extern	inline		BOOL
IsNumberText(char *_lpstrText)
{
	int	iLength	=	strlen(_lpstrText);

	if (iLength	==	0)
		return	FALSE;

	for (;iLength;iLength--)
	{
		if ((BYTE)_lpstrText[iLength-1] > '9' || (BYTE)_lpstrText[iLength-1] < '0')
			return	FALSE;
	}

	return	TRUE;
}

#endif