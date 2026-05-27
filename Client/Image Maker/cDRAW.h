/*************************************************************************************
  개			  요 :  스크린과 독립된 그래픽 출력 클래스.

  제한			사항 :	본 클래스를 제작자의 허락 없이 어떤식으로도 전용하는것을 금합니다.
						
  필요	  라이브러리 :	ddraw.lib<ddraw.h>,dxguid.lib

  작성			날짜 :	98.12.03정도..
  첫번째		갱신 :	98.12.09~
  세번째		갱신 :	99.01.06~
						함수 포인터를 이용함..
  네번째		갱신 :	99.06.06~
						광원효과 추가(는 벌써..) 한걸.. 최적화..

	xxxxx	Normal				:	아무런 변환 없이 걍 찍어준다.
	xxxxxHB Half	Blending	:	이미지와 배경을 반반 섞는다.
	xxxxxAB Alpha	Blending	:	이미지와 배경을 임의의 비율로 섞는다.
	xxxxxFX						:	...뭐의 약자냐.. ==; 각종 광원처리및 여러가지 효과들..

*************************************************************************************/

#ifndef _classDRAW_H_
#define _classDRAW_H_

#include <windows.h>

#include "SFC.H"

#define	d4HM_MASK				0x7bde7bde
#define	d2HM_MASK				0x7bde
#define	d4AM_MASK_1				0x7c1f03e0
#define	d4AM_MASK_2				0x03e07c1f

#define	dGET_RED_SHIFT			10
#define	dGET_GREEN_MASK			31

extern	WORD					redEFFECT[7][32][32];
extern	WORD					blueEFFECT[7][32][32];
extern	WORD					greenEFFECT[7][64][64];

typedef struct
{	int	pos;
	int	size;
} zoomTable;

extern zoomTable	zoomXTable[1024];
extern zoomTable	zoomYTable[768];
extern inline void	MakeSpriteRTable(int x,int y,int xs,int ys,int xLen,int yLen);
extern inline void	fMakeSpriteRTable(int x,int y,int xs,int ys,int xLen,int yLen);

class cDRAW
{
public:
	static int					WIDTH,HEIGHT,halfScreen,halfWidth,byteWidth,dotPerScreen,bytePerScreen;
	static WORD					*p2SCREEN;
	static UINT					*p4SCREEN;

	static BOOL	Init			();
	static BOOL	Active			(WORD *screen,int width,int height);

	static void	PutPixel		(WORD color,int x,int y);

	static void	Line			(WORD color,int x1,int y1,int x2,int y2);
	static void	XLine			(WORD color,int x1,int x2,int y,int thick=1);
	static void	YLine			(WORD color,int y1,int y2,int x,int thick=1);
	static void	XLineAB			(WORD color,int alpha,int x1,int x2,int y,int thick=1);
	static void	YLineAB			(WORD color,int alpha,int y1,int y2,int x,int thick=1);
	static void	XLineHB			(WORD color,int x1,int x2,int y,int thick=1);
	static void	YLineHB			(WORD color,int y1,int y2,int x,int thick=1);
	static void	Box				(WORD color,int x1,int y1,int x2,int y2,int thick=1);
	static void	BoxAB			(WORD color,int alpha,int x1,int y1,int x2,int y2,int thick=1);
	static void	BoxHB			(WORD color,int x1,int y1,int x2,int y2,int thick=1);

/*	16bit	화면 체우기								*/
	static void	Clear			();
	static void	Fill			(WORD color);
	static void	FillHB			(WORD color);
	static void	FillAB			(WORD color,int alpha);

/*	16bit	영역 체우기.. 박스다..					*/
	static void	Fill			(WORD color,int x1,int y1,int x2,int y2);
	static void	FillHB			(WORD color,int x1,int y1,int x2,int y2);
	static void	FillAB			(WORD color,int x1,int y1,int x2,int y2,int alpha);

/*	16bit	이미지 읽어 오기..						*/
	static void GetImage		(int x,int y,int xs,int ys,WORD *img);
	static WORD*GetImage		(int x,int y,int xs,int ys);

/*	16bit	이미지 찍어주기..						*/
	static void	PutImage		(int x,int y,WORD *img);
	static void	PutImageScale	(int x,int y,int iScale,WORD *img);
	static void	PutImageHB		(int x,int y,WORD *img);
	static void	PutImageAB		(int x,int y,WORD *img,int alpha);

/*	8bit	이미지 찍어주기..						*/
	static void	PutImage		(int x,int y,BYTE *img,WORD *plt);
	static void	PutImageScale	(int x,int y,BYTE *img,WORD *plt,int iScale);
	static void	PutImageHB		(int x,int y,BYTE *img,WORD *plt);
	static void	PutImageAB		(int x,int y,BYTE *img,WORD *plt,int alpha);

/*	압축 font	출력 클래스..	*/
	static void	PutFontNormal	(int x,int y,WORD color,BYTE *font,BOOL bMC);
	static void	PutFontHB		(int x,int y,WORD color,BYTE *font,BOOL bMC);
	static void	PutFont			(int x,int y,WORD color,BYTE *font,int alpha,BOOL bMC);

/*	그림자	출력..				*/
	static void PutShadow		(int x,int y,BYTE *shadow);
	static void	PutShadowR		(int x,int y,BYTE *sprite,int xRate,int yRate);
	static void	fPutShadowR		(int x,int y,BYTE *sprite,int xRate,int yRate);

	static void	PutLayer		(int x,int y,WORD color,BYTE *sprite);
	static void	fPutLayer		(int x,int y,WORD color,BYTE *sprite);
	static void	PutLayerR		(int x,int y,WORD color,BYTE *sprite,int xRate,int yRate);
	static void	fPutLayerR		(int x,int y,WORD color,BYTE *sprite,int xRate,int yRate);

	static void	AA				(int x,int y,int xs,int ys);				//	안티 얼라이싱
	static void	AA				(int x,int y,int xs,int ys,WORD color);
};

class c8BITSPRITE	: public cDRAW
{
public:
	static void	PutSprite		(int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	PutSpriteColor	(int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	PutSpriteHB		(int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	PutSpriteAB		(int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	PutSpriteFX		(int x,int y,BYTE *sprite,WORD *plt,int fx);

	static void	fPutSprite		(int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	fPutSpriteColor	(int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	fPutSpriteHB	(int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	fPutSpriteAB	(int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	fPutSpriteFX	(int x,int y,BYTE *sprite,WORD *plt,int fx);

	static void	PutSpriteR		(int x,int y,BYTE *sprite,WORD *plt,int xrate,int yrate,int alpha);
	static void	PutSpriteRColor	(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha);
	static void	PutSpriteRHB	(int x,int y,BYTE *sprite,WORD *plt,int xrate,int yrate,int alpha);
	static void	PutSpriteRAB	(int x,int y,BYTE *sprite,WORD *plt,int xrate,int yrate,int alpha);
	static void	PutSpriteRFX	(int x,int y,BYTE *sprite,WORD *plt,int xrate,int yrate,int fx);

	static void	fPutSpriteR		(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha);
	static void	fPutSpriteRColor(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha);
	static void	fPutSpriteRHB	(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha);
	static void	fPutSpriteRAB	(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha);
	static void	fPutSpriteRFX	(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int fx);
};

class c16BITSPRITE	: public cDRAW
{
public:
	static void	PutSprite		(int x,int y,WORD *sprite,int alpha);
	static void	PutSpriteColor	(int x,int y,WORD *sprite,int alpha);
	static void	PutSpriteHB		(int x,int y,WORD *sprite,int alpha);
	static void	PutSpriteAB		(int x,int y,WORD *sprite,int alpha);
	static void	PutSpriteFX		(int x,int y,WORD *sprite,int fx);

	static void	fPutSprite		(int x,int y,WORD *sprite,int alpha);
	static void	fPutSpriteColor	(int x,int y,WORD *sprite,int alpha);
	static void	fPutSpriteHB	(int x,int y,WORD *sprite,int alpha);
	static void	fPutSpriteAB	(int x,int y,WORD *sprite,int alpha);
	static void	fPutSpriteFX	(int x,int y,WORD *sprite,int fx);

	static void	PutSpriteR		(int x,int y,WORD *sprite,int xrate,int yrate,int alpha);
	static void	PutSpriteRColor	(int x,int y,WORD *sprite,int xrate,int yrate,int alpha);
	static void	PutSpriteRHB	(int x,int y,WORD *sprite,int xrate,int yrate,int alpha);
	static void	PutSpriteRAB	(int x,int y,WORD *sprite,int xrate,int yrate,int alpha);
	static void	PutSpriteRFX	(int x,int y,WORD *sprite,int xrate,int yrate,int fx);

	static void	fPutSpriteR		(int x,int y,WORD *sprite,int xRate,int yRate,int alpha);
	static void	fPutSpriteRColor(int x,int y,WORD *sprite,int xRate,int yRate,int alpha);
	static void	fPutSpriteRHB	(int x,int y,WORD *sprite,int xRate,int yRate,int alpha);
	static void	fPutSpriteRAB	(int x,int y,WORD *sprite,int xRate,int yRate,int alpha);
	static void	fPutSpriteRFX	(int x,int y,WORD *sprite,int xRate,int yRate,int fx);
};

extern	void	(*PutSprite16[22])	(int x,int y,WORD *sprite,int alpha);
extern	void	(*PutSpriteR16[22])	(int x,int y,WORD *sprite,int xRate,int yRate,int alpha);
extern	void	(*PutSprite8[22])	(int x,int y,BYTE *sprite,WORD *plt,int alpha);
extern	void	(*PutSpriteR8[22])	(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha);
extern	void	(*PutFont[3])		(int x,int y,WORD color,int alpha,BYTE *font,BOOL bMC);

extern	BOOL	CONVERTSPRITE(WORD *sprite);
extern	BOOL	CONVERTIMAGE(WORD *image);
extern	BOOL	CONVERTPALETTE(WORD *plt,int range);

extern	WORD	*CONVERT24TO16(BYTE *buffer,int xs,int ys);
extern	WORD	*CONVERT24TO16(BYTE *buffer,BYTE *plt,int xs,int ys);
extern	BYTE	*CONVERT16TO24(WORD *buffer);
extern	WORD	*CONVERTPALETTE(BYTE *plt);

extern	inline	WORD	RGB24ToRGB565(int r,int g,int b);
extern	inline	WORD	RGB24ToRGB555(int r,int g,int b);
extern	inline	WORD	RGB24ToBGR565(int r,int g,int b);
extern	inline	WORD	RGB24To16(int r,int g,int b);
extern	inline	void	RGB16To24(WORD color,BYTE &r,BYTE &g,BYTE &b);
extern	inline	WORD	RGB565ToBGR565(WORD color);
extern	inline	WORD	RGB565ToRGB555(WORD color);
extern	inline	WORD	CONVERTPIXEL(WORD color);

extern	inline	DWORD	MIX4BYTE(DWORD spriteColor,DWORD screenColor,int alpha,int dest_alpha);
extern	inline	DWORD	fMIX4BYTE(DWORD spriteColor,DWORD screenColor,int alpha,int dest_alpha);
extern	inline	WORD	MIX2BYTE(WORD spriteColor,WORD screenColor,int alpha,int dest_alpha);
extern	inline	WORD	FXMIX(WORD spriteColor,WORD screenColor,int effect);


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

/*		그라데이션 컬러 데이타..	*/
extern	WORD		_gCOL[7][16];

#endif