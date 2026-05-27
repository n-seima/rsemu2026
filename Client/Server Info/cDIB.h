/********************************************************************************

	class CANVAS DIB version	창모드 플밍을 위한것이다.
								범용적이지만 속도가 느리기 때문에 게임은 무리이고
								에디터 제작에 사용한다.

********************************************************************************/
#ifndef _classDIB_H_
#define _classDIB_H_

#include <windows.h>

#include "SFC.H"

#define	d4HM_RGB555			0x7bde7bde
#define	d2HM_RGB555			0x7bde
#define	d4AM_RGB555_1		0x7c1f03e0
#define	d4AM_RGB555_2		0x03e07c1f

#define dRGB565		0 
#define dRGB555		1
#define dBGR565		2

#define dYELLOW		0 
#define dCYAN		1
#define dRED		2
#define dGREEN		3
#define dGRAY		4
#define dBLUE		5
#define dPURPLE		6

extern	WORD		_WHITE		,_BLACK;
extern	WORD		_LTGRAY		,_GRAY		,_DEEPGRAY;
extern	WORD		_LTCYAN		,_CYAN		,_DEEPCYAN;
extern	WORD		_LTBLUE		,_BLUE		,_DEEPBLUE;
extern	WORD		_LTRED		,_RED		,_DEEPRED;
extern	WORD		_LTGREEN	,_GREEN		,_DEEPGREEN;
extern	WORD		_LTYELLOW	,_YELLOW	,_DEEPYELLOW;
extern	WORD		_LTPURPLE	,_PURPLE	,_DEEPPURPLE;

/*		알파 블렌딩을 위한 테이블..	*/
extern	WORD		_ALPHATABLE[0x7fff+1];
extern	BYTE		_ALPHAVALUE[0x7fff+1];

/*		그라데이션 컬러 데이타..	*/
extern	WORD		_gCOL[7][16];
extern	WORD		RGBmix(BYTE r,BYTE g,BYTE b);

#define				dEFFECT_SOFT_DODGE		0	//	불효과
#define				dEFFECT_DIFFERENT		1	//	중독
#define				dEFFECT_DODGE_BURN		2	//	불에 그을린 효과
#define				dEFFECT_SOFT			3	//	연기..
#define				dEFFECT_DODGE			4	//	빛계열 마법
#define				dEFFECT_LIGHT			5	//	다중 투영효과..
#define				dEFFECT_EXTREAM_LIGHT	6	//	무쟈게 밝게..
#define				dEFFECT_HALF_BLENDING	7	//	반투명
#define				dEFFECT_ALPHA_BLENDING	8	//	알파 블렌딩
//#define				dEFFECT_NORMAL			10	//	걍.. 찍는다.

extern	WORD		redEFFECT[7][32][32];
extern	WORD		blueEFFECT[7][32][32];
extern	WORD		greenEFFECT[7][64][64];

class cDIB
{
public:
	HWND		hDIBWND;
	HDC			hDC;
	WORD		*p2SCREEN;
	HBITMAP		hBM;
	HBITMAP		DefaultBitmap;
	BOOL		isACTIVE;			//	활성화 상태

	HFONT		hFONT;				//	폰트
	int			bkMODE,textColor;	//	텍스트 출력 모드,컬러

	int			WIDTH,HEIGHT;

				cDIB();
				cDIB(HWND hwnd);

	BOOL		Init(HWND hwnd);
	BOOL		Flip(cRECT *rect=NULL);
	BOOL		Active(BOOL active);
	void		Close();

	BOOL		TextSetting(HFONT hfont,int mode,int r,int g,int b);
};


#endif