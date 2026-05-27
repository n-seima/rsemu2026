/*************************************************************************************
  개			   요 : Direct Draw와 DIB를 기반으로 한 윈도우용 그래픽 출력 클래스.

  제한			사항 :	본 클래스를 제작자의 허락 없이 어떤식으로도 전용하는것을 금합니다.

  주의			사항 :	"CONTROL.H"에서 원하는 ㅎ그래픽 환경을 선택할것..
						dOUTPUT_CANVAS	->	DD를 기반으로 한다. (풀 스크린 즉, 겜에 적합하다.)
						dOUTPUT_DIB		->	DIB를 기반으로 한다.(창 모드, 에디터에 적합하다.)
						CANVAS의 경우 빠른 속도를 가지지만 유동적이지 못하다.
						DIB의 경우 CANVAS에 비해 느리지만 어떤 환경(16컬러에서 24비트 컬러까지..)
						에서도 이상 없이 돌아 간다.
						
  필요	  라이브러리 :	ddraw.lib<ddraw.h>,dxguid.lib

  작성			날짜 :	98.12.03정도..
  첫번째		갱신 :	98.12.09~
  세번째		갱신 :	99.01.06~
						함수 포인터를 이용함..

	xxxxx	Normal				:	아무런 변환 없이 걍 찍어준다.
	xxxxxHC Half	Convert		:	이미지의 컬러를 반으로 줄여 출력한다.
	xxxxxAC Alpha	Convert		:	이미지의 컬러에 임의의 값으로 변환후 출력한다.
	xxxxxHM Half	Mix     	:	이미지와 배경을 반반 섞는다.
	xxxxxAM Alpha	Mix     	:	이미지와 배경을 임의의 비율로 섞는다.
	xxxxxAB Alpha	Blending	:	알파 블렌딩..

	 위에 함수들은 Alpha Mix나 Alpha Convert 같이 알파값이 필요한 곳 외에도 알파값을
	주었는데.. 이유는 간단하다.. 함수 포인터 땜시.. ==;

	출력 속도는 normal>HC>HM>AC>AM>AB


*************************************************************************************/

#ifndef _classDRAW_H_
#define _classDRAW_H_

#include <windows.h>

#include "SFC.H"
#include "cDIB.H"

#define	dPUT_NORMAL				0
#define	dPUT_HALFMIX			1
#define	dPUT_ALPHAMIX			2
#define	dPUT_HALFCONVERT 		3
#define	dPUT_ALPHACONVERT		4
#define	dPUT_ALPHABLENDING		5

class cDRAW
{
public:
	static WORD	*p2SCREEN;
	static UINT	*p4SCREEN;
	static int	WIDTH,HEIGHT,halfWidth;
	static int	DotPerScreen,BytePerScreen,HalfScreen;

				cDRAW();

	static BOOL	Init(cDIB *dib);
	static BOOL	Init(int width,int height,WORD *screen);

	static void	Clear	();
	static void	Line	(WORD color,int x1,int y1,int x2,int y2);
	static void	XLine	(WORD color,int x1,int x2,int y,int thick=1);
	static void	YLine	(WORD color,int y1,int y2,int x,int thick=1);
	static void	XLineAM	(WORD color,int alpha,int x1,int x2,int y,int thick=1);
	static void	YLineAM	(WORD color,int alpha,int y1,int y2,int x,int thick=1);
	static void	XLineHM	(WORD color,int x1,int x2,int y,int thick=1);
	static void	YLineHM	(WORD color,int y1,int y2,int x,int thick=1);
	static void	Box		(WORD color,int x1,int y1,int x2,int y2,int thick=1);
	static void	BoxAM(WORD color,int alpha,int x1,int y1,int x2,int y2,int thick=1);
	static void	BoxHM(WORD color,int x1,int y1,int x2,int y2,int thick=1);
	static void PutPixel(WORD color,int x,int y);
	static void PutPixel(WORD color,int x,int y,int size);

/*	16bit	화면 체우기								*/
	static void	Fill	(WORD color);
	static void	FillHM	(WORD color);
	static void	FillHC	(WORD color);
	static void	FillAM	(WORD color,int alpha);
	static void	FillAC	(WORD color,int alpha);

/*	16bit	영역 체우기.. 박스다..					*/
	static void	Fill	(WORD color,int x1,int y1,int x2,int y2);
	static void	FillHM	(WORD color,int x1,int y1,int x2,int y2);
	static void	FillHC	(WORD color,int x1,int y1,int x2,int y2);
	static void	FillAM	(WORD color,int x1,int y1,int x2,int y2,int alpha);
	static void	FillAC	(WORD color,int x1,int y1,int x2,int y2,int alpha);

	static void	Grade	(WORD color1,WORD color2,int x1,int y1,int x2,int y2,int xs);		//	그라데이션

/*	16bit	이미지 읽어 오기..						*/
	static void GetImage	(int x,int y,int xs,int ys,WORD *img);
	static WORD*GetImage	(int x,int y,int xs,int ys);

/*	16bit	이미지 찍어주기..						*/
	static void	PutImage	(int x,int y,int xs,int ys,WORD *img);
	static void	PutImageHalf(int x,int y,int xs,int ys,WORD *img);
	static void	PutImageHM	(int x,int y,int xs,int ys,WORD *img);
	static void	PutImageHC	(int x,int y,int xs,int ys,WORD *img);
	static void	PutImageAM	(int x,int y,int xs,int ys,WORD *img,int alpha);
	static void	PutImageAC	(int x,int y,int xs,int ys,WORD *img,int alpha);

/*	16bit	이미지 압축 스프라이트 출력 클래스..	*/
	static void	PutSprite16   (int x,int y,WORD *sprite,int alpha);
	static void	PutSpriteHalf (int x,int y,WORD *sprite,int alpha);
	static void	PutSpriteHM16 (int x,int y,WORD *sprite,int alpha);
	static void	PutSpriteHC16 (int x,int y,WORD *sprite,int alpha);
	static void	PutSpriteAB16 (int x,int y,WORD *sprite,int alpha);
	static void	PutSpriteAM16 (int x,int y,WORD *sprite,int alpha);
	static void	PutSpriteAC16 (int x,int y,WORD *sprite,int alpha);
	static void	fPutSprite16  (int x,int y,WORD *sprite,int alpha);
	static void	fPutSpriteHM16(int x,int y,WORD *sprite,int alpha);
	static void	fPutSpriteHC16(int x,int y,WORD *sprite,int alpha);
	static void	fPutSpriteAB16(int x,int y,WORD *sprite,int alpha);
	static void	fPutSpriteAM16(int x,int y,WORD *sprite,int alpha);
	static void	fPutSpriteAC16(int x,int y,WORD *sprite,int alpha);

/*	8bit	이미지 압축 스프라이트 출력 클래스..	*/
	static void	PutSprite8	 (int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	PutSpriteHM8 (int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	PutSpriteAB8 (int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	PutSpriteAM8 (int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	fPutSprite8	 (int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	fPutSpriteHM8(int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	fPutSpriteAB8(int x,int y,BYTE *sprite,WORD *plt,int alpha);
	static void	fPutSpriteAM8(int x,int y,BYTE *sprite,WORD *plt,int alpha);

	static void PutShadow	 (int x,int y,BYTE *shadow);

/*	압축 font	출력 클래스..	*/
	static void	PutFontNormal	(int x,int y,WORD color,BYTE *font,BOOL bMC);
	static void	PutFontHB		(int x,int y,WORD color,BYTE *font,BOOL bMC);
	static void	PutFont			(int x,int y,WORD color,BYTE *font,int alpha,BOOL bMC);
	static void	PutFontAnti		(int x,int y,WORD color,BYTE *font,int effect=dEFFECT_SOFT);
	static void PutFontBuffer	(int x,int y,int bXSize,int bYSize,BYTE color,BYTE *buffer,BYTE *font);


/*	불꽃을 위한 거..			*/
	static void	PutFlame	(int x,int y,int xs,int ys,BYTE *img,WORD *plt);
	static void	PutFlame	(int x,int y,int xs,int ys,BYTE *img,WORD *plt,WORD *compare);
	static void	PutFlameAB	(int x,int y,int xs,int ys,BYTE *img,WORD *plt);
	static void	PutFlameFX	(int x,int y,int xs,int ys,BYTE *img,WORD *plt,int effect);


	static void	PutSprite	(int x,int y,BYTE *sprite,WORD *plt,int put,int alpha,BOOL flip);
	static void	PutSprite	(int x,int y,WORD *sprite,int put,int alpha,BOOL flip);

	static void	(*pPutSprite16[6][2])(int x,int y,WORD *sprite,int alpha);
	static void	(*pPutSprite8[6][2])(int x,int y,BYTE *sprite,WORD *plt,int alpha);

	static void	redSprite8(int x,int y,BYTE *sprite,WORD *pplt);
	static void	PutBuilding(int x,int y,BYTE *sprite,WORD *plt);
	static void	redPutBuilding(int x,int y,BYTE *sprite,WORD *plt);
	static void PutSpriteColor(int x,int y,WORD *sprite,WORD color);
	static void	PutBuildingLayer(int x,int y,BYTE *sprite,WORD color);
	static void	PutSprite8Anti(int x,int y,BYTE *sprite,WORD *plt,int alpha);

};	


#endif