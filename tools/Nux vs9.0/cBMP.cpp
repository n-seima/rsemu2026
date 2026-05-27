#include <stdio.h>
#include "cIMAGE.H"
#include "cMESSAGE.H"


cBMP::cBMP()
    :cIMAGE()
{	m_pPtr		=	NULL;
}

cBMP::~cBMP()
{
	pKILL(m_p16IMAGE);
	pKILL(m_pPLT);
	pKILL(m_p8IMAGE);
	pKILL(m_pPtr);
}

BOOL 
cBMP::Load(char *fn,BOOL change16bit)
{	FILE	*fp;

	fp=fopen(fn,"rb");
	if (fp==NULL) return cMSG::Output("Error!! in class cBMP::Load(... - can not find follow file '%s'",fn);

	BOOL result = Load(fp,FALSE);

	if (m_p8IMAGE && change16bit)
	{	m_p16IMAGE		=	new WORD [m_iWidth*m_iHeight+2];
		m_p16IMAGE[0]	=	(WORD)m_iWidth;
		m_p16IMAGE[1]	=	(WORD)m_iHeight;

		for (int i = 0 ; i < m_iWidth*m_iHeight; i++)	m_p16IMAGE[i+2]	=	m_pPLT[m_p8IMAGE[i+4]];
		pKILL(m_p8IMAGE);
	}

	fclose(fp);

	return result;
}

BOOL 
cBMP::LoadCustom(char *fn)
{	FILE	*fp;

	fp	=	fopen(fn,"rb");
	if (fp==NULL) return cMSG::Output("Error!! in class cBMP::Load(... - can not find follow file '%s'",fn);

	BOOL result = Load(fp,TRUE);

	fclose(fp);

	return result;
}

BOOL 
cBMP::Load(FILE *fp,BOOL bCustom)
{	pKILL(m_p16IMAGE);
	pKILL(m_p8IMAGE);
	pKILL(m_pPtr);

	fread((void*)&m_stFileHeader,sizeof(m_stFileHeader),1,fp);

    if (m_stFileHeader.bfType != 0x4d42)  // 비트맵인지 확인한다.(BMP화일인지..
		return cMSG::Output("Error!! in class cBMP::Load(... - this file not bmp file");

		// 비트맵 데이타의 크기를 알아 낸다. 화일 사이즈 - 헤더 사이즈..
    m_dwDibSize = m_stFileHeader.bfSize - sizeof(m_stFileHeader);

    m_pPtr		=	new BYTE[m_dwDibSize];  // 비트맵 이미지를 읽어 들인다..
    fread((void*)m_pPtr, m_dwDibSize,1,fp);

	m_lpInfo		= (LPBITMAPINFO) m_pPtr;
    m_lpInfoHd		= (LPBITMAPINFOHEADER) m_pPtr;

    m_lpRGBTable	= (RGBQUAD*)(m_pPtr + m_lpInfoHd->biSize); 

    if ((m_lpInfoHd->biClrUsed == 0) && (m_lpInfoHd->biBitCount < 9)) 
		 m_iColors	=	(1 << m_lpInfoHd->biBitCount);
    else m_iColors	=	m_lpInfoHd->biClrUsed;

    m_lpData		= m_pPtr + m_lpInfoHd->biSize + m_iColors * sizeof(RGBQUAD);

	//		위에까지 데이터 읽어 오기..

	m_iBPP			= m_lpInfoHd->biBitCount;		// bit Per Pixel
	m_iRWidth		= m_lpInfoHd->biWidth;
	m_iBytePerLine	= m_iRWidth*3;
	m_iWidth		= m_lpInfoHd->biWidth;
	m_iHeight		= m_lpInfoHd->biHeight;

	if (m_iWidth		%4	)	m_iWidth		=	m_iWidth/4*4+4;
	if (m_iBytePerLine	%4	)	m_iBytePerLine	=	m_iBytePerLine/4*4+4;
	if (m_iBPP			== 8)	m_iBytePerLine	=	m_iWidth;

	if (m_iBPP > 8)
	{	
		m_p16IMAGE	=	new WORD [m_iWidth*m_iHeight+2];

		int addr	=	(m_iHeight-1)*m_iBytePerLine;
		int ip		=	2,i,j;

		WORD tc		= RGBmix(m_lpData[addr+2],m_lpData[addr+1],m_lpData[addr]);

		m_p16IMAGE[0]	=	m_iWidth;
		m_p16IMAGE[1]	=	m_iHeight;

		for (i=0;i<m_iHeight;i++,addr-=m_iBytePerLine)
			for (j=0;j<m_iWidth*3;j+=3)
			{	if (j >= m_iRWidth*3 )	m_p16IMAGE[ip++]	=	tc;
				else					m_p16IMAGE[ip++]	=	RGBmix(m_lpData[addr+j+2],m_lpData[addr+j+1],m_lpData[addr+j ]);
			}

		if (!bCustom)	pKILL(m_pPtr);

		return TRUE;
	}
	else
	if (m_iBPP == 8)
	{
		int	ip	=	4,i;

		m_pPLT	= new WORD	[256];

		for (i=0;i<256;i++)
		{	int r		=	m_lpRGBTable[i].rgbRed;
			int g		=	m_lpRGBTable[i].rgbGreen;
			int b		=	m_lpRGBTable[i].rgbBlue;

			r			=	(r>>3)<<10;
			g			=	(g>>3)<<5;
			b			=	(b>>3);
			m_pPLT[i]	=	(WORD)r+g+b;
		}

		if (!bCustom)
		{
			m_p8IMAGE	= new BYTE	[m_iWidth*m_iHeight+4];

			*(WORD *)(m_p8IMAGE		)	=	m_iWidth;
			*(WORD *)(m_p8IMAGE+2	)	=	m_iHeight;

			memset(m_p8IMAGE+4,0,m_iWidth*m_iHeight);

			int		addr=(m_iHeight-1)*m_iBytePerLine;
			BYTE	tc	= m_lpData[addr];

			for (i=0;i<m_iHeight;i++,addr-=m_iBytePerLine)
			{	memcpy(m_p8IMAGE+ip				,m_lpData+addr	,m_iRWidth);
				memset(m_p8IMAGE+ip+m_iRWidth	,tc				,m_iWidth-m_iRWidth);
				ip	+=	m_iWidth;
			}

			pKILL(m_pPtr);
		}

		return TRUE;
	}

	return	cMSG::Output("ERROR!! in cBMP::Load(... - can not support '%d' bit Image",m_iBPP);
}


BOOL 
cBMP::LoadPalette(char *fn,BOOL rgb565)
{	FILE	*fp;

	fp	=	fopen(fn,"rb");

	if (fp==NULL) 
		return cMSG::Output("Error!! in class cBMP::Load(... - can not find follow file '%s'",fn);

	pKILL(m_p16IMAGE);
	pKILL(m_p8IMAGE);
	pKILL(m_pPtr);
	pKILL(m_pPLT);

	BITMAPINFOHEADER	InfoHd;

	fread((void*)&m_stFileHeader,sizeof(m_stFileHeader),1,fp);

    if (m_stFileHeader.bfType != 0x4d42)  // 비트맵인지 확인한다.(BMP화일인지..
		return cMSG::Output("Error!! in class cBMP::Load(... - follow file not bmp file '%s'",fn);

	fread(&InfoHd,sizeof(BITMAPINFOHEADER),1,fp);

	if(InfoHd.biBitCount	!=	8)
	{	fclose(fp);
		return	FALSE;
	}

	m_iBPP	=	8;

	fseek(fp,InfoHd.biSize-sizeof(BITMAPINFOHEADER),SEEK_CUR);

	RGBQUAD	rgbQuad[256];

    if (InfoHd.biClrUsed == 0)	m_iColors	=	256;
    else						m_iColors	=	InfoHd.biClrUsed;

//	cMSG::Put("color","count : %d",m_iColors);

	memset(rgbQuad,0,sizeof(rgbQuad));

	fread(rgbQuad,m_iColors,sizeof(RGBQUAD),fp);

	m_pPLT	=	new WORD [256];

	for (int i=0;i<256;i++)
	{
		int r	=	rgbQuad[i].rgbRed;
		int g	=	rgbQuad[i].rgbGreen;
		int b	=	rgbQuad[i].rgbBlue;

		if (rgb565)
		{
			r					=	(r>>3)<<11;
			g					=	(g>>2)<<5;
			b					=	(b>>3);
			m_pPLT[i]			=	(WORD)r+g+b;
		}
		else
		{
			r					=	(r>>3)<<10;
			g					=	(g>>3)<<5;
			b					=	(b>>3);
			m_pPLT[i]			=	(WORD)r+g+b;
		}
	}

	fclose(fp);

	return TRUE;
}


BOOL 
cBMP::Save(char *fn)
{
	FILE	*fp;

	SetFileAttributes(fn,FILE_ATTRIBUTE_ARCHIVE);

	fp	=	fopen(fn,"wb");
		
	if (!fp)	return FALSE;

	fwrite((void*)&m_stFileHeader,sizeof(m_stFileHeader),1,fp);
    fwrite(m_pPtr, m_dwDibSize ,1,fp);
	fclose(fp);

	return TRUE;
}