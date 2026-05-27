
/*

  개			  요 :  그래픽 데이터 화일 처리 클래스..

  제한			사항 :	본 클래스를 제작자의 허락 없이 어떤식으로도 전용하는것을 금합니다.

  작성			날짜 :	1998.12.09

*/


#ifndef _classPICTURE_H_
#define _classPICTURE_H_

#include <windows.h>

#include "SFC.H"
#include "cDRAW.H"

class cBMP
{
public:
	WORD				*p16IMAGE;
	BYTE				*p8IMAGE;
	WORD				*pPLT;
	int					Width,Height,BPP,rWidth,bytePerLine,Colors;
	RGBQUAD				*PALETTE;

	UINT				fileLength,dibSize;
	LPBITMAPINFO		Info; 
	LPBITMAPINFOHEADER	InfoHd;
	BITMAPFILEHEADER	bmFileHeader;
	BYTE				*data,*ptr;
	RGBQUAD				*RGBTable;


						cBMP();
						~cBMP();

	BOOL				Loadas16(char *fn);
	BOOL				Load(char *fn);
	BOOL				Save(char *fn);
	void				Put(int x,int y);
};


class ClassPCX
{
public:
	

};


#endif