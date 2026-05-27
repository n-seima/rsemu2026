#include "cIMAGE.H"
#include "cDRAW.H"
#include <stdio.h>


cBMP::cBMP()
    :cIMAGE()
{	ptr		=	NULL;
	m_pPLT	=	NULL;
}

cBMP::~cBMP()
{	pKILL(ptr);
}

BOOL 
cBMP::Load(char *fn,BOOL change16bit)
{	FILE	*fp;

	fp=fopen(fn,"rb");
	if (fp==NULL) return MSGOUT("Error!! in class cPICTURE::Load(... - '%s' 화일을 찾을 수 없습니다.",fn);

	BOOL result = Load(fp);

	if (p8IMAGE && change16bit)
	{	p16IMAGE	=	new WORD [Width*Height+2];
		p16IMAGE[0]	=	(WORD)Width;
		p16IMAGE[1]	=	(WORD)Height;

		for (int i = 0 ; i < Width*Height; i++)	p16IMAGE[i+2]	=	pPLT[p8IMAGE[i+4]];
		pKILL(p8IMAGE);
	}

	fclose(fp);

	return result;
}

//
//	팔레트 부르기
BOOL 
cBMP::loadPalette(char *_lpstrFileName,BOOL _bIsWantRGB565)
{	FILE	*fp;

	fp	=	fopen(_lpstrFileName,"rb");
	if (fp==NULL) return cMSG::Error("Error!! in class cPICTURE::Load(... - '%s' 화일을 찾을 수 없습니다.",_lpstrFileName);

	pKILL(m_pPLT);

	BITMAPINFOHEADER	InfoHd;

	fread((void*)&bmFileHeader,sizeof(bmFileHeader),1,fp);

    if (bmFileHeader.bfType != 0x4d42)  // 비트맵인지 확인한다.(BMP화일인지..
		return MSGOUT("Error!! in cBMP::Load(... - BMP 화일이 아닙니다.");

	fread(&InfoHd,sizeof(BITMAPINFOHEADER),1,fp);

	if(InfoHd.biBitCount	!=	8)
	{	fclose(fp);
		return	FALSE;
	}

	m_iBPP	=	8;

	fseek(fp,InfoHd.biSize-sizeof(BITMAPINFOHEADER),SEEK_CUR);

	RGBQUAD	rgbQuad[256];

    if (InfoHd.biClrUsed == 0)	Colors	=	256;
    else						Colors	=	InfoHd.biClrUsed;

	memset(rgbQuad,0,sizeof(rgbQuad));

	fread(rgbQuad,Colors,sizeof(RGBQUAD),fp);

	m_pPLT	=	new WORD [256];

	for (int i=0;i<256;i++)
	{
		int r	=	rgbQuad[i].rgbRed;
		int g	=	rgbQuad[i].rgbGreen;
		int b	=	rgbQuad[i].rgbBlue;

		m_aPalette[i*3	]	=	r;
		m_aPalette[i*3+1]	=	g;
		m_aPalette[i*3+2]	=	b;

		if (_bIsWantRGB565)
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
}	//	cBMP::loadPalette(char *_lpstrFileName,BOOL _bIsWantRGB565)


BOOL 
cBMP::Load(FILE *fp)
{	pKILL(p16IMAGE);
	pKILL(p8IMAGE);
	pKILL(ptr);

	fread((void*)&bmFileHeader,sizeof(bmFileHeader),1,fp);

    if (bmFileHeader.bfType != 0x4d42)  // 비트맵인지 확인한다.(BMP화일인지..
		return MSGOUT("Error!! in cBMP::Load(... - BMP 화일이 아닙니다.");

		// 비트맵 데이타의 크기를 알아 낸다. 화일 사이즈 - 헤더 사이즈..
    dibSize = bmFileHeader.bfSize - sizeof(bmFileHeader);

    ptr		=	new BYTE[dibSize];  // 비트맵 이미지를 읽어 들인다..
    fread((void*)ptr, dibSize,1,fp);

	Info		= (LPBITMAPINFO) ptr;
    InfoHd		= (LPBITMAPINFOHEADER) ptr;

    RGBTable	= (RGBQUAD*)(ptr + InfoHd->biSize); 

    if ((InfoHd->biClrUsed == 0) && (InfoHd->biBitCount < 9)) 
		 Colors = (1 << InfoHd->biBitCount);
    else Colors=InfoHd->biClrUsed;

    data		= ptr + InfoHd->biSize + Colors * sizeof(RGBQUAD);

	//		위에까지 데이터 읽어 오기..

	BPP			= InfoHd->biBitCount;		// bit Per Pixel
	rWidth		= InfoHd->biWidth;
	bytePerLine	= rWidth*3;
	Width		= InfoHd->biWidth;
	Height		= InfoHd->biHeight;
	if (Width%4) Width=Width/4*4+4;
	if (bytePerLine%4) bytePerLine=bytePerLine/4*4+4;
	if (BPP == 8)	bytePerLine	=	Width;

	if (BPP > 8)
	{	p16IMAGE	=	new WORD [Width*Height+2];

		int addr	=	(Height-1)*bytePerLine;
		int ip		=	2,i,j;

		WORD tc		= RGB24To16(data[addr+2],data[addr+1],data[addr]);

		p16IMAGE[0]	=	Width;
		p16IMAGE[1]	=	Height;

		BPP			=	16;

		for (i=0;i<Height;i++,addr-=bytePerLine)
			for (j=0;j<Width*3;j+=3)
			{	if (j >= rWidth*3 )	p16IMAGE[ip++]	=	tc;
				else				p16IMAGE[ip++]	=	RGB24To16(data[addr+j+2],data[addr+j+1],data[addr+j ]);
			}

		pKILL(ptr);
		return TRUE;
	}
	else
	if (BPP == 8)
	{	int	ip	=	4,i;
		pPLT	= new WORD	[256];
		p8IMAGE	= new BYTE	[Width*Height+4];

		*(WORD *)(p8IMAGE	)	=	Width;
		*(WORD *)(p8IMAGE+2	)	=	Height;

		for (i=0;i<256;i++)
		{	int r	=	RGBTable[i].rgbRed;
			int g	=	RGBTable[i].rgbGreen;
			int b	=	RGBTable[i].rgbBlue;
			r		=	(r>>3)<<10;
			g		=	(g>>3)<<5;
			b		=	(b>>3);
			pPLT[i]	=	(WORD)r+g+b;
		}

		memset(p8IMAGE+4,0,Width*Height);

		int		addr=(Height-1)*bytePerLine;
		BYTE	tc	= data[addr];

		for (i=0;i<Height;i++,addr-=bytePerLine)
		{	memcpy(p8IMAGE+ip		,data+addr	,rWidth);
			memset(p8IMAGE+ip+rWidth,tc			,Width-rWidth);
			ip	+=	Width;
		}

		pKILL(ptr);
		return TRUE;
	}

	return	MSGOUT("ERROR!! in cBMP::Load(... - '%d' bit Image 는 지원 되지 않습니다.",BPP);
}