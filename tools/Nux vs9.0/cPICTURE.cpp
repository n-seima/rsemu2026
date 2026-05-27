#include <stdio.h>
#include <io.h>
#include "cPICTURE.H"
#include "cMESSAGE.H"

cBMP::cBMP()
{	p16IMAGE=	NULL;	
	p8IMAGE =	NULL;	
	pPLT	=	NULL;
	Width	=	0;
	Height	=	0;
	PALETTE	=	NULL;
	ptr		=	NULL;
}

cBMP::~cBMP()
{	pKILL(pPLT);
	pKILL(p16IMAGE);
	pKILL(p8IMAGE);
	pKILL(PALETTE);
	pKILL(ptr);
}

BOOL 
cBMP::Loadas16(char *fn)
{	pKILL(pPLT);
	pKILL(p16IMAGE);
	pKILL(p8IMAGE);
	pKILL(PALETTE);
	pKILL(ptr);

	FILE	*fp;

	fp=fopen(fn,"rb");
	if (fp==NULL) return cMSG::Put(_ms("'%s' 화일을 찾을 수 없습니다.",fn));

	fread((void*)&bmFileHeader,sizeof(bmFileHeader),1,fp);
    if (bmFileHeader.bfType != 0x4d42)  // 비트맵인지 확인한다.(BMP화일인지..
		return cMSG::Put(_ms("'%s' 화일은 BMP 화일이 아닙니다.",fn));

		// 비트맵 데이타의 크기를 알아 낸다. 화일 사이즈 - 헤더 사이즈..
	fileLength=filelength(fileno(fp));
    dibSize = fileLength - sizeof(bmFileHeader);

    ptr =new BYTE[dibSize];  // 비트맵 이미지를 읽어 들인다..
    fread((void*)ptr, dibSize,1,fp);
    fclose(fp);

	Info		= (LPBITMAPINFO) ptr;
    InfoHd		= (LPBITMAPINFOHEADER) ptr;

    RGBTable	= (RGBQUAD*)(ptr + InfoHd->biSize); 

    if ((InfoHd->biClrUsed == 0) && (InfoHd->biBitCount < 9)) 
		 Colors = (1 << InfoHd->biBitCount);
    else Colors=InfoHd->biClrUsed;

	if (Colors > 256 ) Colors = 0;

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

	if (InfoHd->biSize + Colors * sizeof(RGBQUAD) + bytePerLine*Height > dibSize)
	{	pKILL(ptr);
		return cMSG::Put(_ms("'%s' 화일 데이터를 인식 할 수 없습니다.",fn));
	}

	if (BPP > 8)
	{	p16IMAGE= new WORD [Width*Height];

		int addr=(Height-1)*bytePerLine;
		int ip=0,i,j;
		WORD color,tc;
		BYTE r,g,b;

		r=data[bytePerLine*(Height-1)+2]*31/255;
		g=data[bytePerLine*(Height-1)+1]*63/255;
		b=data[bytePerLine*(Height-1)+0]*31/255;
		tc = RGBmix(r,g,b);

		for (i=0;i<Height;i++,addr-=bytePerLine)
			for (j=0;j<Width*3;j+=3)
			{	if (j >= rWidth*3 )	p16IMAGE[ip++]=tc;
				else
				{	r=data[addr+j+2]*31/255;
					g=data[addr+j+1]*63/255;
					b=data[addr+j  ]*31/255;
					color = RGBmix(r,g,b);
					p16IMAGE[ip++]=color;
				}
			}

		pKILL(ptr);
		return TRUE;
	}
	else if (BPP == 8)
	{	PALETTE	= new RGBQUAD	[256];
		pPLT	= new WORD		[256];
		p16IMAGE= new WORD		[Width*Height];
		int ip=0,i,j;
		BYTE r,g,b;
		
		for (i=0;i<256;i++)
		{	r					=	RGBTable[i].rgbRed*31/255;
			g					=	RGBTable[i].rgbGreen*63/255;
			b					=	RGBTable[i].rgbBlue*31/255;
			PALETTE[i].rgbRed	=	RGBTable[i].rgbRed;
			PALETTE[i].rgbGreen	=	RGBTable[i].rgbGreen;
			PALETTE[i].rgbBlue	=	RGBTable[i].rgbBlue;
			pPLT[i]				=	RGBmix(r,g,b);
		}

		for (i=0;i<Height;i++)	for (j=0;j<Width;j++)	p16IMAGE[i*Width+j]=pPLT[data[bytePerLine*(Height-1)]];

		int addr=(Height-1)*bytePerLine;
		for (i=0;i<Height;i++,addr-=bytePerLine)
			for (j=0;j<Width;j++)
				if (j >= rWidth )	p16IMAGE[ip++]=pPLT[data[bytePerLine*(Height-1)]];
				else				p16IMAGE[ip++]=pPLT[data[addr+j]];

		pKILL(ptr);
		return TRUE;
	}

	return	cMSG::Put(_ms("ERROR!! in cBMP::Loadas16(... '%d' bit Image 는 지원 되지 않습니다.",BPP));
}

BOOL 
cBMP::Load(char *fn)
{	pKILL(pPLT);
	pKILL(p16IMAGE);
	pKILL(p8IMAGE);
	pKILL(PALETTE);
	pKILL(ptr);

	FILE	*fp;

	fp=fopen(fn,"rb");
	if (fp==NULL) return cMSG::Put("Error!! in class cPICTURE :: Init(...","'%s' 화일을 찾을 수 없습니다.",fn);

	fread((void*)&bmFileHeader,sizeof(bmFileHeader),1,fp);

    if (bmFileHeader.bfType != 0x4d42)  // 비트맵인지 확인한다.(BMP화일인지..
		return cMSG::Put("Error!! in class cPICTURE :: Init(...","BMP 화일이 아닙니다.",fn);

		// 비트맵 데이타의 크기를 알아 낸다. 화일 사이즈 - 헤더 사이즈..
	fseek(fp,0L,SEEK_END);
	fileLength=ftell(fp);
	fseek(fp,sizeof(bmFileHeader),SEEK_SET);
    dibSize = fileLength - sizeof(bmFileHeader);

    ptr =new BYTE[dibSize];  // 비트맵 이미지를 읽어 들인다..
    fread((void*)ptr, dibSize,1,fp);
    fclose(fp);

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

	if (BPP > 8)
	{	p16IMAGE= new WORD [Width*Height];
		int addr=(Height-1)*bytePerLine;
		int ip=0,i,j;
		WORD color,tc;
		BYTE r,g,b;

		r=data[2]*31/255;
		g=data[1]*63/255;
		b=data[0]*31/255;
		tc = RGBmix(r,g,b);

		for (i=0;i<Height;i++) for (j=0;j<Width;j++) p16IMAGE[i*Width+j]=tc;

		for (i=0;i<Height;i++,addr-=bytePerLine)
			for (j=0;j<Width*3;j+=3)
			{	if (j >= rWidth*3 )	p16IMAGE[ip++]=tc;
				else
				{	r=data[addr+j+2]*31/255;
					g=data[addr+j+1]*63/255;
					b=data[addr+j  ]*31/255;
					color = RGBmix(r,g,b);
					p16IMAGE[ip++]=color;
				}
			}

		pKILL(ptr);
		return TRUE;
	}
	else
	if (BPP == 8)
	{	PALETTE	= new RGBQUAD	[256];
		pPLT	= new WORD		[256];
		p8IMAGE	= new BYTE		[Width*Height];
		int ip=0,i,j;
		BYTE r,g,b;

		for (i=0;i<256;i++)
		{	r					=	RGBTable[i].rgbRed*31/255;
			g					=	RGBTable[i].rgbGreen*63/255;
			b					=	RGBTable[i].rgbBlue*31/255;
			PALETTE[i].rgbRed	=	RGBTable[i].rgbRed;
			PALETTE[i].rgbGreen	=	RGBTable[i].rgbGreen;
			PALETTE[i].rgbBlue	=	RGBTable[i].rgbBlue;
			pPLT[i]				=	RGBmix(r,g,b);
		}

		for (i=0;i<Height;i++)	for (j=0;j<Width;j++)	p8IMAGE[i*Width+j]=0;

		int addr=(Height-1)*Width;
		for (i=0;i<Height;i++,addr-=Width)
			for (j=0;j<Width;j++)
				if (j >= rWidth )	p8IMAGE[ip++]=0;
				else				p8IMAGE[ip++]=data[addr+j];

		pKILL(ptr);
		return TRUE;
	}

	return	cMSG::Put("ERROR!! in cBMP::Load(...","'%d' bit Image 는 지원 되지 않습니다.",BPP);
}

BOOL 
cBMP::Save(char *fn)
{	FILE	*fp;

	fp=fopen(fn,"wb");

	fwrite((void*)&bmFileHeader,sizeof(bmFileHeader),1,fp);
    fwrite(ptr, dibSize,1,fp);
	fclose(fp);

	return TRUE;
}

void
cBMP::Put(int x,int y)
{	cDRAW::PutImage(x,y,Width,Height,p16IMAGE);
}
