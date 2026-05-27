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

#include "SFC.H"
#include "cTRANS.H"

#define	dPUT_SOFT_DODGE			0	//	불효과
#define	dPUT_DIFFERENT			1	//	중독
#define	dPUT_DODGE_BURN			2	//	불에 그을린 효과
#define	dPUT_SOFT				3	//	연기..
#define	dPUT_DODGE				4	//	빛계열 마법
#define	dPUT_LIGHT				5	//	다중 투영효과..
#define	dPUT_EXTREAM_LIGHT		6	//	무쟈게 밝게..
#define	dPUT_NORMAL				7	//	걍.. 찍는다.
#define	dPUT_HALF_BLENDING		8	//	반투명
#define	dPUT_COLOR				9	//	특정 컬러로 찍는다.
#define	dPUT_ALPHA_BLENDING		10	//	알파 블렌딩
#define	dPUT_FLIP				11	//	알파 블렌딩

#define dRGB565					0	//	RGB 565
#define dRGB555					1	//	RGB 555
#define dBGR565					2	//	BGR 565

#define	dSPRITE_BPP16			0
#define	dSPRITE_FLIP			11
#define	dSPRITE_ZOOM			22
#define	dSPRITE_BPP8			44

#define	d4HM_MASK_555			0x7bde7bde
#define	d2HM_MASK_555			0x7bde
#define	d4AM_MASK_555_1			0x7c1f03e0
#define	d4AM_MASK_555_2			0x03e07c1f

#define	d2HM_MASK_565			0xf7de
#define	d4HM_MASK_565			0xf7def7de
#define	d4AM_MASK_565_1			0xf81f07e0
#define	d4AM_MASK_565_2			0x07e0f81f

#define	dGET_RED_SHIFT555		10
#define	dGET_GREEN_MASK555		31
#define	dGET_RED_SHIFT565		11
#define	dGET_GREEN_MASK565		63

extern	int						g_iScreenWidth,g_iScreenHeight;			//	화면 사이즈
extern	WORD					redEFFECT[7][32][32];
extern	WORD					blueEFFECT[7][32][32];
extern	WORD					greenEFFECT[7][64][64];

extern	int						g_iPixelFormat;							//	픽셸 포맷
extern	int						g_iRealPixelFormat;						//	진짜 픽셸 포맷?

extern	WORD					(*RGBmix  )(int r,int g,int b );
extern	WORD					RGB565mix(int r,int g,int b );
extern	WORD					RGB555mix(int r,int g,int b );
extern	WORD					BGR565mix(int r,int g,int b );

typedef struct
{	int	pos;
	int	size;
} zoomTable;

extern zoomTable	zoomXTable[3096];
extern zoomTable	zoomYTable[3096];
extern inline void	MakeSpriteRTable(int x,int y,int xs,int ys,int xLen,int yLen);
extern inline void	fMakeSpriteRTable(int x,int y,int xs,int ys,int xLen,int yLen);

#define dMAG_FILTER_WIDTH		100
#define dMAG_FILTER_HIGHT		100

class cDRAW
{
public:
	static int					_x,_y,_xs,_ys,_x1,_y1,_x2,_y2,_Color,_Thick,_Alpha,_DestAlpha,_Effect,_XRate,_YRate,_Zoom,_PutMethod,_Flip;
	static	int					s_iMaxWidth,s_iMaxHeight;

	static WORD					*_pPLT;
	static BYTE					*_p8SPRITE,*_p8IMAGE,*_lpLAYER;
	static WORD					*_p16SPRITE,*_p16IMAGE;
	static DWORD				*_pdwMag;

public:
	static int					WIDTH,HEIGHT,halfScreen,halfWidth,byteWidth,dotPerScreen,bytePerScreen;
	static cRECT				s_rectClippingArea;
	static WORD					*p2SCREEN;
	static UINT					*p4SCREEN;
	static WORD					*s_lpPushedScreen;
	static int					s_iPushedWidth,s_iPushedHeight;

	static BOOL	Init			();
	static BOOL	Active			(WORD *screen,int width,int height);
	static void	PushInfo		()
	{
		CRY(s_lpPushedScreen,"Error in cDRAW::PushInfo - already exist pushed info");
		s_lpPushedScreen	=	p2SCREEN;
		s_iPushedWidth		=	WIDTH;
		s_iPushedHeight		=	HEIGHT;
	}
	static void	PopInfo()
	{
		CRY(!s_lpPushedScreen,"Error in cDRAW::PopInfo - valid pushed info");
		Active(s_lpPushedScreen,s_iPushedWidth,s_iPushedHeight);
		s_lpPushedScreen	=	NULL;
	}

	static inline	void	SetClippginArea(int _iX1,int _iY1,int _iX2,int _iY2)
	{	
		s_rectClippingArea.x1	=	max(_iX1,0);
		s_rectClippingArea.x2	=	min(_iX2,WIDTH);
		s_rectClippingArea.y1	=	max(_iY1,0);
		s_rectClippingArea.y2	=	min(_iY2,HEIGHT);
	}
	static inline	void		FreeClippginArea(){s_rectClippingArea.set(0,0,WIDTH,HEIGHT);}
	static void	PutPixel		(WORD color,int x,int y);
	static void	PutPixelHB		(WORD color,int x,int y);
	static void	PutPixelAB		(WORD color,int x,int y,int alpha);
	static void	initMagFilter	();

	static void	Line			(WORD color,int x1,int y1,int x2,int y2);
	static void	BrightLine		(int _iBright,int x1,int y1,int x2,int y2);
	static void	LineHB			(WORD color,int x1,int y1,int x2,int y2);
	static void	LineAB			(WORD color,int alpha,int x1,int y1,int x2,int y2);
	static void	XLine			(WORD color,int x1,int x2,int y,int thick=1);
	static void	YLine			(WORD color,int y1,int y2,int x,int thick=1);
	static void	XLineAB			(WORD color,int alpha,int x1,int x2,int y,int thick=1);
	static void	YLineAB			(WORD color,int alpha,int y1,int y2,int x,int thick=1);
	static void	XLineHB			(WORD color,int x1,int x2,int y,int thick=1);
	static void	YLineHB			(WORD color,int y1,int y2,int x,int thick=1);
	static void	Box				(WORD color,int x1,int y1,int x2,int y2,int thick=1);
	static void	BrightBox		(int _iBright,int x1,int y1,int x2,int y2,int thick=1);
	static void	BrightBox		(int _iBright,cRECT *rect,int thick=1)
								{	BrightBox(_iBright,rect->x1,rect->y1,rect->x2,rect->y2,thick);
								}

	static void	BoxAB			(WORD color,int alpha,int x1,int y1,int x2,int y2,int thick=1);
	static void	BoxHB			(WORD color,int x1,int y1,int x2,int y2,int thick=1);

	static void	Circle			(WORD _wColor,int _iX,int _iY,int _iRadius,int _iStep=1);
	static void	OvalCircle		(WORD _wColor,int _iX,int _iY,int _iRadius,int _iStep=1);
	static void	ParticleOvalCircle(WORD _wColor,int _iX,int _iY,int _iRadius,int _iAngle,int _iAngleRange,int _iStep=1);
	static void	DrawParticleGradationOvalCircle(WORD _wColor,int _iX,int _iY,int _iRadius,int _iAngle,int _iAngleRange,int _iGradationStep,int _iGradationTimes,int _iStep=1);


	static inline void	Box		(WORD color,cRECT *rect,int thick=1)
								{	Box	(color,rect->x1,rect->y1,rect->x2,rect->y2,thick);
								}
	static inline void	BoxAB	(WORD color,int alpha,cRECT *rect,int thick=1)
								{	BoxAB(color,alpha,rect->x1,rect->y1,rect->x2,rect->y2,thick);
								}
	static inline void	BoxHB	(WORD color,cRECT *rect,int thick=1)
								{	BoxHB(color,rect->x1,rect->y1,rect->x2,rect->y2,thick);
								}

/*	16bit	화면 체우기								*/
	static void	Clear			();
	static void	Fill			(WORD color);
	static void	FillHB			(WORD color);
	static void	FillAB			(WORD color,int alpha);

/*	16bit	영역 체우기.. 박스다..					*/
	static void	Fill			(WORD color,int x1,int y1,int x2,int y2);
	static void	FillHB			(WORD color,int x1,int y1,int x2,int y2);
	static void	FillAB			(WORD color,int x1,int y1,int x2,int y2,int alpha);
	static void	Fill			(WORD color,cRECT *rect)
								{	Fill(color,rect->x1,rect->y1,rect->x2,rect->y2);
								}
	static void	FillHB			(WORD color,cRECT *rect)
								{	FillHB(color,rect->x1,rect->y1,rect->x2,rect->y2);
								}
	static void	FillAB			(WORD color,cRECT *rect,int alpha)
								{	FillAB(color,rect->x1,rect->y1,rect->x2,rect->y2,alpha);
								}
	static void	FillBright		(int _iBright,int x1,int y1,int x2,int y2);
	static void	FillBright		(int _iBright,cRECT *rect)
								{	FillBright(_iBright,rect->x1,rect->y1,rect->x2,rect->y2);
								}

/*	16bit	이미지 읽어 오기..						*/
	static void GetImage		(int x,int y,int xs,int ys,WORD *img);
	static WORD*GetImage		(int x,int y,int xs,int ys);
	static WORD*GetImage		();

/*	16bit	이미지 찍어주기..						*/
	static void	PutClippedImage	(int _iX,int _iY,int _iWidth,int _iHeight,WORD *_lpImage,int _iDx,int _iDy);
	static void	PutClippedImageHB(int _iX,int _iY,int _iWidth,int _iHeight,WORD *_lpImage,int _iDx,int _iDy);

	static void	PutImage		(int x,int y,WORD *img);
	static void	PutImageScale	(int x,int y,WORD *img,int _iScale);
	static void	PutImageHB		(int x,int y,WORD *img);
	static void	PutImageAB		(int x,int y,WORD *img,int alpha);
	static void PutSpriteByReferenceImage(int _iX,int _iY,WORD *_lpImage,BYTE *_lpReferenceShape);
	static void PutSpriteByReferenceImage(int _iX,int _iY,WORD *_lpImage,BYTE *_lpReferenceShape,int _iScale);

/*	8bit	이미지 찍어주기..						*/
	static void	PutImage		(int x,int y,BYTE *img,WORD *plt);
	static void	PutImageScale	(int x,int y,BYTE *img,WORD *plt,int _iScale);
	static void	PutImageHB		(int x,int y,BYTE *img,WORD *plt);
	static void	PutImageAB		(int x,int y,BYTE *img,WORD *plt,int alpha);
	static void PutSpriteByReferenceImage(int _iX,int _iY,BYTE *_lpImage,WORD *_lpPlt,BYTE *_lpReferenceShape);
	static void PutSpriteByReferenceImage(int _iX,int _iY,BYTE *_lpImage,WORD *_lpPlt,BYTE *_lpReferenceShape,int _iScale);

/*	압축 font	출력 클래스..	*/
	static void	PutFont			(int x,int y,WORD color,BYTE *font);
	static void	PutFontHB		(int x,int y,WORD color,BYTE *font);
	static void	PutFontAB		(int x,int y,WORD color,BYTE *font,int alpha);

	static void PutReferenceSprite(int _iX,int _iY,WORD *_lpSprite,WORD *_lpImage);
	
	static void	PutScreenReferenceBlackEffect();

	static void FixGaroPixel(int _iX,int _iY , int _iRadius);
	static void FixSeroPixel(int _iX,int _iY , int _iRadius);

	static void putMagniFinder(int _iX,int _iY );

	static void	PutScreenReferenceTvEffect(int interVal);

	static BOOL	IsIn			(WORD *_lpData,int _iX,int _iY);
	static BOOL	IsIn			(BYTE *_lpData,int _iX,int _iY);
};

#define	dMAX_SHADOW_THUMB	20

/*	그림자	출력..				*/
class	cSHADOW_INFO
{	public:
		WORD	begin,end;
};

class cSHADOW			: public cDRAW
{
public:
	static	cSHADOW_INFO	m_aShadowInfo[800][dMAX_SHADOW_THUMB];
	static	BYTE			m_ucShadowCount[800];

	static	void			ResetShadow		();
	static	void			AddShadowMask	(int x1,int y,int x2);
	static	void			ShadowMask		(int x,int y,BYTE *shadow);
	static	void			PutShadowMask	();

	static	void			PutShadow		(int x,int y,BYTE *shadow);
	static	void			PutShadow		(int x,int y,BYTE *shadow,int iAlpha);
	static	void			PutShadow		(int x,int y,BYTE *sprite,int xRate,int yRate,int _iMX,int _iMY);
	static	void			PutShadow		(int x,int y,BYTE *sprite,int xRate,int yRate,int iAlpha,int _iMX,int _iMY);
};

class cLAYER555			: public cDRAW
{
public:
	static void	PutLayer		();
	static void	PutLayerHB		();
	static void	PutLayerAB		();
	static void	PutLayerFX		();

	static void	fPutLayer		();
	static void	fPutLayerHB		();
	static void	fPutLayerAB		();
	static void	fPutLayerFX		();

	static void	PutLayerR		();
	static void	PutLayerRHB		();
	static void	PutLayerRAB		();
	static void	PutLayerRFX		();

	static void	fPutLayerR		();
	static void	fPutLayerRHB	();
	static void	fPutLayerRAB	();
	static void	fPutLayerRFX	();
};

class cLAYER565			: public cDRAW
{
public:
	static void	PutLayerHB		();
	static void	PutLayerAB		();
	static void	PutLayerFX		();

	static void	fPutLayerHB		();
	static void	fPutLayerAB		();
	static void	fPutLayerFX		();

	static void	PutLayerRHB		();
	static void	PutLayerRAB		();
	static void	PutLayerRFX		();

	static void	fPutLayerRHB	();
	static void	fPutLayerRAB	();
	static void	fPutLayerRFX	();
};

class c8BITSPRITE555	: public cDRAW
{
public:
	static void	PutSprite		();
	static void	PutSpriteColor	();
	static void	PutSpriteHB		();
	static void	PutSpriteAB		();
	static void	PutSpriteFX		();

	static void	fPutSprite		();
	static void	fPutSpriteColor	();
	static void	fPutSpriteHB	();
	static void	fPutSpriteAB	();
	static void	fPutSpriteFX	();

	static void	PutSpriteR		();
	static void	PutSpriteRColor	();
	static void	PutSpriteRHB	();
	static void	PutSpriteRAB	();
	static void	PutSpriteRFX	();

	static void	fPutSpriteR		();
	static void	fPutSpriteRColor();
	static void	fPutSpriteRHB	();
	static void	fPutSpriteRAB	();
	static void	fPutSpriteRFX	();
};

class c8BITSPRITE565	: public cDRAW
{
public:
	static void	PutSpriteHB		();
	static void	PutSpriteAB		();
	static void	PutSpriteFX		();

	static void	fPutSpriteHB	();
	static void	fPutSpriteAB	();
	static void	fPutSpriteFX	();

	static void	PutSpriteRHB	();
	static void	PutSpriteRAB	();
	static void	PutSpriteRFX	();

	static void	fPutSpriteRHB	();
	static void	fPutSpriteRAB	();
	static void	fPutSpriteRFX	();
};

class c16BITSPRITE555	: public cDRAW
{
public:
	static void	PutSprite		();
	static void	PutSpriteColor	();
	static void	PutSpriteHB		();
	static void	PutSpriteAB		();
	static void	PutSpriteFX		();

	static void	fPutSprite		();
	static void	fPutSpriteColor	();
	static void	fPutSpriteHB	();
	static void	fPutSpriteAB	();
	static void	fPutSpriteFX	();

	static void	PutSpriteR		();
	static void	PutSpriteRColor	();
	static void	PutSpriteRHB	();
	static void	PutSpriteRAB	();
	static void	PutSpriteRFX	();

	static void	fPutSpriteR		();
	static void	fPutSpriteRColor();
	static void	fPutSpriteRHB	();
	static void	fPutSpriteRAB	();
	static void	fPutSpriteRFX	();
};

class c16BITSPRITE565	: public cDRAW
{
public:
	static void	PutSpriteHB		();
	static void	PutSpriteAB		();
	static void	PutSpriteFX		();

	static void	fPutSpriteHB	();
	static void	fPutSpriteAB	();
	static void	fPutSpriteFX	();

	static void	PutSpriteRHB	();
	static void	PutSpriteRAB	();
	static void	PutSpriteRFX	();

	static void	fPutSpriteRHB	();
	static void	fPutSpriteRAB	();
	static void	fPutSpriteRFX	();
};

extern	void	(*fnPutSprite[88])();

extern	inline	DWORD	MIX4BYTE555	(DWORD spriteColor,DWORD screenColor,int alpha,int dest_alpha);
extern	inline	DWORD	fMIX4BYTE555(DWORD spriteColor,DWORD screenColor,int alpha,int dest_alpha);
extern	inline	WORD	MIX2BYTE555	(WORD  spriteColor,WORD  screenColor,int alpha,int dest_alpha);
extern	inline	DWORD	MIX4BYTE565	(DWORD spriteColor,DWORD screenColor,int alpha,int dest_alpha);
extern	inline	DWORD	fMIX4BYTE565(DWORD spriteColor,DWORD screenColor,int alpha,int dest_alpha);
extern	inline	WORD	MIX2BYTE565	(WORD  spriteColor,WORD  screenColor,int alpha,int dest_alpha);

extern	inline	DWORD	GET4BYTEALPHAVALUE555(DWORD color,int alpha);
extern	inline	DWORD	fGET4BYTEALPHAVALUE555(DWORD color,int alpha);
extern	inline	WORD	GET2BYTEALPHAVALUE555(WORD color,int alpha);
extern	inline	DWORD	GET4BYTEALPHAVALUE565(DWORD color,int alpha);
extern	inline	DWORD	fGET4BYTEALPHAVALUE565(DWORD color,int alpha);
extern	inline	WORD	GET2BYTEALPHAVALUE565(WORD color,int alpha);

extern	inline	WORD	FXMIX555(WORD spriteColor,WORD screenColor,int effect);
extern	inline	WORD	FXMIX565(WORD spriteColor,WORD screenColor,int effect);
extern	inline	WORD	FXMIX555(int r,int g,int b,WORD screenColor,int effect);
extern	inline	WORD	FXMIX565(int r,int g,int b,WORD screenColor,int effect);

extern	inline	WORD	GetMixColor(WORD spriteColor,WORD screenColor);
extern	inline	WORD	GetMixColor(WORD  spriteColor,WORD  screenColor,int alpha,int dest_alpha);
extern	inline	WORD	GetFXMixColor(WORD  spriteColor,WORD  screenColor,int effect);

#define	_PutSprite(x,y,bpp,flip,spr16,spr8,plt,xrate,yrate,effect,alpha,mx,my)				\
	cDRAW::_x			=	x;																\
	cDRAW::_y			=	y;																\
	cDRAW::_p16SPRITE	=	spr16;															\
	cDRAW::_p8SPRITE	=	spr8;															\
	cDRAW::_pPLT		=	plt;															\
	cDRAW::_XRate		=	xrate;															\
	cDRAW::_YRate		=	yrate;															\
	cDRAW::_Effect		=	effect;															\
	cDRAW::_Alpha		=	alpha;															\
	cDRAW::_DestAlpha	=	32-alpha;														\
	cDRAW::_Color		=	alpha;															\
	cDRAW::_Flip		=	flip;															\
	cDRAW::s_iMaxWidth	=	mx;																\
	cDRAW::s_iMaxHeight	=	my;																\
	if (cDRAW::_XRate < 0)																	\
	{	cDRAW::_XRate	=	-cDRAW::_XRate;													\
		cDRAW::_Flip	=	11-cDRAW::_Flip;												\
	}																						\
	if (cDRAW::_XRate != 100 || cDRAW::_YRate != 100)	cDRAW::_Zoom	=	dSPRITE_ZOOM;	\
	else												cDRAW::_Zoom	=	0;				\
	fnPutSprite[bpp+flip+cDRAW::_Effect+cDRAW::_Zoom]();

extern	inline	void	_PutSprite16(int x,int y,int flip,WORD *spr16,int xrate,int yrate,int alpha,int fx);
extern	inline	void	_PutSprite16Normal(int x,int y,int flip,WORD *spr16);
extern	inline	void	_PutSprite8(int x,int y,int flip,BYTE *spr8,WORD *plt,int xrate,int yrate,int alpha,int fx);
extern	inline	void	_PutSprite8Normal(int x,int y,int flip,BYTE *spr8,WORD *plt);

extern	inline	void	_ReadySprite(int x,int y,int bpp,int flip,WORD *spr16,BYTE *spr8,WORD *plt,int xrate,int yrate,int alpha,int fx);
extern	inline	void	_ReaddySprite16(int x,int y,int flip,WORD *spr16,int xrate,int yrate,int alpha,int fx);
extern	inline	void	_ReaddySprite16Normal(int x,int y,int flip,WORD *spr16);
extern	inline	void	_ReaddySprite8(int x,int y,int flip,BYTE *spr8,WORD *plt,int xrate,int yrate,int alpha,int fx);
extern	inline	void	_ReaddySprite8Normal(int x,int y,int flip,BYTE *spr8,WORD *plt);

extern	inline	void	_PutLayer(int x,int y,WORD color,int flip,BYTE *layer,int xrate,int yrate,int alpha,int fx);

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
extern	WORD		_SKYBLUE;

extern	int			g_iWorldX,g_iWorldY;

#endif