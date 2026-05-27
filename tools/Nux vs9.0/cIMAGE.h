#ifndef _classIMAGE_H
#define	_classIMAGE_H

#include "sfc.h"

//--------------------------------------------------------------------------------
//	IMAGE 클래스
//--------------------------------------------------------------------------------

class cIMAGE
{
public:
	WORD				*m_p16IMAGE,*m_pPLT;
	BYTE				*m_p8IMAGE;
	int					m_iWidth,m_iHeight,m_iBPP;

						cIMAGE()
						{	m_p16IMAGE	=	NULL;
							m_pPLT		=	NULL;
							m_p8IMAGE	=	NULL;
						}

						~cIMAGE()
						{	
							pKILL(m_p16IMAGE);
							pKILL(m_pPLT);
							pKILL(m_p8IMAGE);
						}

	BOOL				Put(int x,int y);
	virtual	BOOL		Load(char *fn,BOOL change16bit=FALSE){return TRUE;};
};

//--------------------------------------------------------------------------------
//	BMP 화일
//--------------------------------------------------------------------------------
class cBMP	:	public cIMAGE
{
public:
	int					m_iRWidth,m_iBytePerLine,m_iColors;

	UINT				m_dwFileLength,m_dwDibSize;
	LPBITMAPINFO		m_lpInfo; 
	LPBITMAPINFOHEADER	m_lpInfoHd;
	BITMAPFILEHEADER	m_stFileHeader;
	BYTE				*m_lpData,*m_pPtr;
	RGBQUAD				*m_lpRGBTable;

						cBMP();
						~cBMP();

	BOOL				Load(char *fn,BOOL change16bit=FALSE);
	BOOL				LoadCustom(char *fn);
	BOOL				LoadPalette(char *fn,BOOL rgb565=FALSE);
	BOOL				Load(FILE *fp,BOOL bLoadOnly = FALSE);
	BOOL				Save(char *fn);
};

//--------------------------------------------------------------------------------
//	TGA 화일
//--------------------------------------------------------------------------------
class cTGA	:	public cIMAGE
{
public:
	BOOL			Load(char *fn,BOOL change16bit=FALSE);
	BOOL			Load(FILE *fp);

	static	BOOL	Save(char *fn,WORD *image,BOOL overWrite=TRUE);
	static	BOOL	Save(char *fn,WORD *image,int width,int height,BOOL overWrite=TRUE);
};


//--------------------------------------------------------------------------------
//	PCX 화일
//--------------------------------------------------------------------------------
class cPCX	:	public cIMAGE
{
public:

	BOOL			Load(char *fn,BOOL change16bit=FALSE);
	BOOL			Load(FILE *fp);

	static	BOOL	Save(char *fn,WORD *image,BOOL overWrite=TRUE);
};


#endif