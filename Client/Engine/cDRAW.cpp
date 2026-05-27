#include "cDRAW.H"
#include "cANGLE.H"

int			g_iPixelFormat;			//	픽셸 포맷
int			g_iRealPixelFormat;		//	진짜 픽셸 포맷
int			g_iScreenWidth;			//	화면 사이즈
int			g_iScreenHeight;
int			g_iWorldX=0,g_iWorldY=0;

WORD (*RGBmix  )(int r,int g,int b ) = RGB565mix;

WORD RGB565mix(int r,int g,int b)
{
	r>>=3;
	g>>=2;
	b>>=3;

	return 	(WORD) (  (((WORD)(r<<11))) + (((WORD)(g<<5))) + ((WORD)b)  );
}

WORD RGB555mix(int r,int g,int b)
{
	r>>=3;
	g>>=3;
	b>>=3;

	return 	(WORD) (  (((WORD)(r<<10))) + (((WORD)(g<<5))) + ((WORD)b)  );
}

WORD BGR565mix(int r,int g,int b)
{
	r>>=3;
	g>>=2;
	b>>=3;

	return 	(WORD) (  (((WORD)(b<<11))) + (((WORD)(g<<5))) + ((WORD)r)  );
}

inline	WORD	
GetMixColor(WORD spriteColor,WORD screenColor)
{
	spriteColor	=	spriteColor&d2HM_MASK_555;

	if	(g_iPixelFormat == dRGB555)
		screenColor	=	((screenColor&d2HM_MASK_555)>>1)+(spriteColor>>1);
	else
		screenColor	=	((screenColor&d2HM_MASK_565)>>1)+(spriteColor>>1);

	return	screenColor;
}

inline	WORD	
GetMixColor(WORD  spriteColor,WORD  screenColor,int alpha,int dest_alpha)
{
	if	(g_iPixelFormat	==	dRGB555)
		return	MIX2BYTE555(spriteColor,screenColor,alpha,dest_alpha);

	return	MIX2BYTE565(spriteColor,screenColor,alpha,dest_alpha);
}

inline	WORD
GetFXMixColor(WORD  spriteColor,WORD  screenColor,int effect)
{
	if (g_iPixelFormat	==	dRGB555)
		return	FXMIX555(spriteColor,screenColor,effect);

	return	FXMIX565(spriteColor,screenColor,effect);
}

////////////////////////////////////////////////////////////////////////////
//	컬러
////////////////////////////////////////////////////////////////////////////


WORD		_WHITE		,_BLACK;
WORD		_LTGRAY		,_GRAY		,_DEEPGRAY;
WORD		_LTCYAN		,_CYAN		,_DEEPCYAN;
WORD		_LTBLUE		,_BLUE		,_DEEPBLUE;
WORD		_LTRED		,_RED		,_DEEPRED;
WORD		_LTGREEN	,_GREEN		,_DEEPGREEN;
WORD		_LTYELLOW	,_YELLOW	,_DEEPYELLOW;
WORD		_LTPURPLE	,_PURPLE	,_DEEPPURPLE;
WORD		_SKYBLUE;


static	DWORD	mixFirst,mixSecond,mixColor,backWord,fxRed,fxGreen,fxBlue;

inline	DWORD	
MIX4BYTE555(DWORD spriteColor,DWORD screenColor,int alpha,int dest_alpha)
{	mixFirst	=	screenColor&d4AM_MASK_555_1;
	mixFirst	>>=	5;
	mixFirst	*=	dest_alpha;
	mixFirst	&=	d4AM_MASK_555_1;

	mixSecond	=	screenColor&d4AM_MASK_555_2;
	mixSecond	*=	dest_alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_555_2;

	mixColor	=	mixFirst + mixSecond;

	mixFirst	=	spriteColor&d4AM_MASK_555_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_555_1;

	mixSecond	=	spriteColor&d4AM_MASK_555_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_555_2;

	return	mixColor+	mixFirst + mixSecond;
}

inline	DWORD	
fMIX4BYTE555(DWORD spriteColor,DWORD screenColor,int alpha,int dest_alpha)
{
	mixFirst	=	screenColor&d4AM_MASK_555_1;
	mixFirst	>>=	5;
	mixFirst	*=	dest_alpha;
	mixFirst	&=	d4AM_MASK_555_1;

	mixSecond	=	screenColor&d4AM_MASK_555_2;
	mixSecond	*=	dest_alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_555_2;

	mixColor	=	mixFirst + mixSecond;

	backWord	=	((spriteColor<<16)+(spriteColor>>16));

	mixFirst	=	backWord&d4AM_MASK_555_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_555_1;

	mixSecond	=	backWord&d4AM_MASK_555_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_555_2;

	return	(mixColor+	mixFirst + mixSecond);
}

inline	WORD	
MIX2BYTE555(WORD spriteColor,WORD screenColor,int alpha,int dest_alpha)
{
	mixFirst	=	screenColor&d4AM_MASK_555_1;
	mixFirst	>>=	5;
	mixFirst	*=	dest_alpha;
	mixFirst	&=	d4AM_MASK_555_1;

	mixSecond	=	screenColor&d4AM_MASK_555_2;
	mixSecond	*=	dest_alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_555_2;

	mixColor	=	mixFirst + mixSecond;

	mixFirst	=	spriteColor&d4AM_MASK_555_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_555_1;

	mixSecond	=	spriteColor&d4AM_MASK_555_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_555_2;

	return	(WORD)(mixColor	+ mixFirst + mixSecond);
}

inline	WORD	
CONVERT2BYTE555(WORD spriteColor,int alpha)
{
	mixFirst	=	spriteColor&d4AM_MASK_555_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_555_1;

	mixSecond	=	spriteColor&d4AM_MASK_555_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_555_2;

	return	(WORD)(mixFirst + mixSecond);
}

inline	WORD	
CONVERT2BYTE565(WORD spriteColor,int alpha)
{
	mixFirst	=	spriteColor&d4AM_MASK_565_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_565_1;

	mixSecond	=	spriteColor&d4AM_MASK_565_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_565_2;

	return	(WORD)(mixFirst + mixSecond);
}

inline	DWORD	
MIX4BYTE565(DWORD spriteColor,DWORD screenColor,int alpha,int dest_alpha)
{	
	mixFirst	=	screenColor&d4AM_MASK_565_1;
	mixFirst	>>=	5;
	mixFirst	*=	dest_alpha;
	mixFirst	&=	d4AM_MASK_565_1;

	mixSecond	=	screenColor&d4AM_MASK_565_2;
	mixSecond	*=	dest_alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_565_2;

	mixColor	=	mixFirst + mixSecond;

	mixFirst	=	spriteColor&d4AM_MASK_565_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_565_1;

	mixSecond	=	spriteColor&d4AM_MASK_565_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_565_2;

	return	(mixColor+	mixFirst + mixSecond);
}

inline	DWORD	
fMIX4BYTE565(DWORD spriteColor,DWORD screenColor,int alpha,int dest_alpha)
{
	mixFirst	=	screenColor&d4AM_MASK_565_1;
	mixFirst	>>=	5;
	mixFirst	*=	dest_alpha;
	mixFirst	&=	d4AM_MASK_565_1;

	mixSecond	=	screenColor&d4AM_MASK_565_2;
	mixSecond	*=	dest_alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_565_2;

	mixColor	=	mixFirst + mixSecond;

	backWord	=	((spriteColor<<16)+(spriteColor>>16));

	mixFirst	=	backWord&d4AM_MASK_565_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_565_1;

	mixSecond	=	backWord&d4AM_MASK_565_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_565_2;

	return	(mixColor+	mixFirst + mixSecond);
}

inline	WORD	
MIX2BYTE565(WORD spriteColor,WORD screenColor,int alpha,int dest_alpha)
{
	mixFirst	=	screenColor&d4AM_MASK_565_1;
	mixFirst	>>=	5;
	mixFirst	*=	dest_alpha;
	mixFirst	&=	d4AM_MASK_565_1;

	mixSecond	=	screenColor&d4AM_MASK_565_2;
	mixSecond	*=	dest_alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_565_2;

	mixColor	=	mixFirst + mixSecond;

	mixFirst	=	spriteColor&d4AM_MASK_565_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_565_1;

	mixSecond	=	spriteColor&d4AM_MASK_565_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_565_2;

	return	(WORD)(mixColor	+ mixFirst + mixSecond);
}

////////////////////////////////////////////////////////////////////////////
//	특정컬러를 alpha값으로 감소 시킨다.
////////////////////////////////////////////////////////////////////////////

inline	DWORD	
GET4BYTEALPHAVALUE555(DWORD color,int alpha)
{	
	mixFirst	=	color&d4AM_MASK_555_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_555_1;

	mixSecond	=	color&d4AM_MASK_555_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_555_2;

	return	(mixFirst + mixSecond);
}

inline	DWORD	
fGET4BYTEALPHAVALUE555(DWORD color,int alpha)
{	backWord	=	((color<<16)+(color>>16));

	mixFirst	=	backWord&d4AM_MASK_555_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_555_1;

	mixSecond	=	backWord&d4AM_MASK_555_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_555_2;

	return	(mixFirst + mixSecond);
}

inline	DWORD	
GET4BYTEALPHAVALUE565(DWORD color,int alpha)
{	
	mixFirst	=	color&d4AM_MASK_565_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_565_1;

	mixSecond	=	color&d4AM_MASK_565_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_565_2;

	return	(mixFirst + mixSecond);
}

inline	DWORD	
fGET4BYTEALPHAVALUE565(DWORD color,int alpha)
{	backWord	=	((color<<16)+(color>>16));

	mixFirst	=	backWord&d4AM_MASK_565_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_565_1;

	mixSecond	=	backWord&d4AM_MASK_565_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_565_2;

	return	(mixFirst + mixSecond);
}

inline	WORD
GET2BYTEALPHAVALUE555(WORD color,int alpha)
{
	mixFirst	=	color&d4AM_MASK_555_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_555_1;

	mixSecond	=	color&d4AM_MASK_555_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_555_2;

	return	(WORD)(mixFirst + mixSecond);
}

inline	WORD
GET2BYTEALPHAVALUE565(WORD color,int alpha)
{
	mixFirst	=	color&d4AM_MASK_565_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_565_1;

	mixSecond	=	color&d4AM_MASK_565_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_565_2;

	return	(WORD)(mixFirst + mixSecond);
}

////////////////////////////////////////////////////////////////////////////
//	광원 효과
////////////////////////////////////////////////////////////////////////////

WORD	redEFFECT[7][32][32];
WORD	blueEFFECT[7][32][32];
WORD	greenEFFECT[7][64][64];

inline WORD
FXMIX555(WORD spriteColor,WORD screenColor,int effect)
{	
	fxRed	=redEFFECT	[effect][ spriteColor>>dGET_RED_SHIFT555		][ screenColor>>dGET_RED_SHIFT555		];
	fxGreen	=greenEFFECT[effect][(spriteColor>>5)&dGET_GREEN_MASK555	][(screenColor>>5)&dGET_GREEN_MASK555	];
	fxBlue	=blueEFFECT	[effect][ spriteColor&31						][ screenColor&31						];

	return (WORD)(fxRed+fxGreen+fxBlue);
}

inline WORD
FXMIX565(WORD spriteColor,WORD screenColor,int effect)
{	
	fxRed	=redEFFECT	[effect][ spriteColor>>dGET_RED_SHIFT565		][ screenColor>>dGET_RED_SHIFT565		];
	fxGreen	=greenEFFECT[effect][(spriteColor>>5)&dGET_GREEN_MASK565	][(screenColor>>5)&dGET_GREEN_MASK565	];
	fxBlue	=blueEFFECT	[effect][ spriteColor&31						][ screenColor&31						];

	return (WORD)(fxRed+fxGreen+fxBlue);
}

inline WORD
FXMIX(WORD spriteColor,WORD screenColor,int effect)
{
	if (g_iPixelFormat	==	dRGB555)
	{
		fxRed	=redEFFECT	[effect][ spriteColor>>dGET_RED_SHIFT555		][ screenColor>>dGET_RED_SHIFT555		];
		fxGreen	=greenEFFECT[effect][(spriteColor>>5)&dGET_GREEN_MASK555	][(screenColor>>5)&dGET_GREEN_MASK555	];
		fxBlue	=blueEFFECT	[effect][ spriteColor&31						][ screenColor&31						];
	}
	else
	{
		fxRed	=redEFFECT	[effect][ spriteColor>>dGET_RED_SHIFT565		][ screenColor>>dGET_RED_SHIFT565		];
		fxGreen	=greenEFFECT[effect][(spriteColor>>5)&dGET_GREEN_MASK565	][(screenColor>>5)&dGET_GREEN_MASK565	];
		fxBlue	=blueEFFECT	[effect][ spriteColor&31						][ screenColor&31						];
	}

	return (WORD)(fxRed+fxGreen+fxBlue);
}

inline WORD
FXMIX555(int r,int g,int b,WORD screenColor,int effect)
{	fxRed	=redEFFECT	[effect][r][ screenColor>>dGET_RED_SHIFT555		];
	fxGreen	=greenEFFECT[effect][g][(screenColor>>5)&dGET_GREEN_MASK555	];
	fxBlue	=blueEFFECT	[effect][b][ screenColor&31						];

	return (WORD)(fxRed+fxGreen+fxBlue);
}

inline WORD
FXMIX565(int r,int g,int b,WORD screenColor,int effect)
{	fxRed	=redEFFECT	[effect][r][ screenColor>>dGET_RED_SHIFT565		];
	fxGreen	=greenEFFECT[effect][g][(screenColor>>5)&dGET_GREEN_MASK565	];
	fxBlue	=blueEFFECT	[effect][b][ screenColor&31						];

	return (WORD)(fxRed+fxGreen+fxBlue);
}

/*********************************************************************************************
//	스프라이트 확대 축소 출력을 위한 테이블	생성 -	배율과 사이즈에 따른 테이블값이 생성된다.

	x,y		  위치
	xs,ys     스프라이트 실제 사이즈..
	xLen,yLen 확대/축소된 스프라이트 사이즈..

*********************************************************************************************/
////////////////////////////////////////////////////////////////////////////
//	확대 축소 테이블 만들기..
////////////////////////////////////////////////////////////////////////////

//	스프라이트 확대 축소 출력을 위한 테이블
zoomTable	zoomXTable[3096];
zoomTable	zoomYTable[3096];

inline void
MakeSpriteRTable(int x,int y,int xs,int ys,int xLen,int yLen)
{
	int xp,yp,length,i,num;
	int	iCounter;

	xp		=0	,	length	=0;

	for (iCounter=xs,i=0;iCounter;--iCounter,++i)
	{
		length				+=	xLen;		//	length를 체크해서 xs보다 크면은.. 점 사이즈를 더해준다.
		num					=	length/xs;

		zoomXTable[i].pos	=	xp;			//	위치.. 이점의 위치.. xp
		length				-=	num*xs;		//	길이에서 빼준다..
		zoomXTable[i].size	=	num;		//	이점의 실제 사이즈..
		xp					+=	num;		//	다음점의 위치..

		if	(x+zoomXTable[i].pos + zoomXTable[i].size >= cDRAW::s_rectClippingArea.x2	)
			zoomXTable[i].size = cDRAW::s_rectClippingArea.x2 - x-zoomXTable[i].pos;

		if	(x+zoomXTable[i].pos < cDRAW::s_rectClippingArea.x1	)
		{
			if	(x+zoomXTable[i].pos + zoomXTable[i].size <= cDRAW::s_rectClippingArea.x1	)
				zoomXTable[i].size = 0;
			else
			{
				zoomXTable[i].size	+=	((x-cDRAW::s_rectClippingArea.x1)+zoomXTable[i].pos);
				zoomXTable[i].pos	=	cDRAW::s_rectClippingArea.x1-x;
			}
		}

		zoomXTable[i].size	=	max(zoomXTable[i].size,0);
	}

	yp		=0	,	length	=0;

	for (iCounter=ys,i=0;iCounter;--iCounter,++i)
	{	
		length				+=	yLen;
		num					=	length/ys;

		zoomYTable[i].pos	=	yp;
		length				-=	num*ys;
		zoomYTable[i].size	=	num;
		yp					+=	num;

		if	(y+zoomYTable[i].size + zoomYTable[i].pos >= cDRAW::s_rectClippingArea.y2) 
			zoomYTable[i].size = cDRAW::s_rectClippingArea.y2 - y - zoomYTable[i].pos;

		if	(y+zoomYTable[i].pos < cDRAW::s_rectClippingArea.y1	)
		{	
			if	(y+zoomYTable[i].pos + zoomYTable[i].size <= cDRAW::s_rectClippingArea.y1	)
				zoomYTable[i].size	=	0;
			else
			{	
				zoomYTable[i].size	+=	((y-cDRAW::s_rectClippingArea.y1)+zoomYTable[i].pos);
				zoomYTable[i].pos	=	-y;
			}
		}

		zoomYTable[i].size	=	max(zoomYTable[i].size,0);
	}
}

inline void
fMakeSpriteRTable(int x,int y,int xs,int ys,int xLen,int yLen)
{	int xp,yp,length,i,num;

	xp		=0	,	length	=0;
	for (i=0;i<xs;i++)
	{	length				+=	xLen;
		num					=	length/xs;

		zoomXTable[i].pos	=	xLen-xp-1;
		length				-=	num*xs;
		zoomXTable[i].size	=	num;
		xp					+=	num;

		if (x+zoomXTable[i].pos >= cDRAW::WIDTH	)
		{	zoomXTable[i].size	-=  x+zoomXTable[i].pos - cDRAW::WIDTH+1;
			zoomXTable[i].pos	=	cDRAW::WIDTH - x-1;
		}
		if (x+zoomXTable[i].pos - zoomXTable[i].size <=  0		)	zoomXTable[i].size = (x+zoomXTable[i].pos);
	}

	yp		=0	,	length	=0;
	for (i=0;i<ys;i++)
	{	length				+=	yLen;
		num					=	length/ys;

		zoomYTable[i].pos	=	yp;
		length				-=	num*ys;
		zoomYTable[i].size	=	num;
		yp					+=	num;

		if (y+zoomYTable[i].size + zoomYTable[i].pos >= cDRAW::HEIGHT) 
			zoomYTable[i].size = cDRAW::HEIGHT - y - zoomYTable[i].pos;

		if (y+zoomYTable[i].pos < 0	)
		{	if (y+zoomYTable[i].pos + zoomYTable[i].size <= 0	)	zoomYTable[i].size = 0;
			else
			{	zoomYTable[i].size += (y+zoomYTable[i].pos);
				zoomYTable[i].pos = -y;
			}
		}
	}
}

void		(*fnPutSprite[88])();
void		(*fnPutLayer[44])();

int			cDRAW::_x			;
int			cDRAW::_y			;
int			cDRAW::_xs			;
int			cDRAW::_ys			;
int			cDRAW::_x1			;
int			cDRAW::_y1			;
int			cDRAW::_x2			;
int			cDRAW::_y2			;
int			cDRAW::_Color		;
int			cDRAW::_Thick		;
int			cDRAW::_Effect		;
int			cDRAW::_Alpha		;
int			cDRAW::_DestAlpha	;
int			cDRAW::_XRate		;
int			cDRAW::_YRate		;
int			cDRAW::_Zoom		;
int			cDRAW::_Flip		;
int			cDRAW::_PutMethod	;
WORD		*cDRAW::_pPLT		;
BYTE		*cDRAW::_p8SPRITE	;
BYTE		*cDRAW::_p8IMAGE	;
WORD		*cDRAW::_p16SPRITE	;
WORD		*cDRAW::_p16IMAGE	;
DWORD		*cDRAW::_pdwMag		;
BYTE		*cDRAW::_lpLAYER	;
cRECT		cDRAW::s_rectClippingArea;
int			cDRAW::s_iMaxWidth	;
int			cDRAW::s_iMaxHeight	;

WORD*		cDRAW::s_lpPushedScreen	=	NULL;
int			cDRAW::s_iPushedWidth;
int			cDRAW::s_iPushedHeight;

inline	void
_PutLayer(int x,int y,WORD color,int flip,BYTE *layer,int xrate,int yrate,int effect,int alpha)
{	cDRAW::_x			=	x;
	cDRAW::_y			=	y;
	cDRAW::_lpLAYER		=	layer;
	cDRAW::_XRate		=	xrate;
	cDRAW::_YRate		=	yrate;
	cDRAW::_Effect		=	effect;
	cDRAW::_Alpha		=	alpha;
	cDRAW::_DestAlpha	=	32-alpha;
	cDRAW::_Color		=	color;

	if (xrate < 0)
	{	cDRAW::_XRate	=	-cDRAW::_XRate;
		flip			=	11-flip;
	}

	if (cDRAW::_XRate != 100 || cDRAW::_YRate != 100)	cDRAW::_Zoom	=	dSPRITE_ZOOM;
	else												cDRAW::_Zoom	=	0;

	cDRAW::_PutMethod	=	flip+cDRAW::_Effect+cDRAW::_Zoom;

	fnPutLayer[cDRAW::_PutMethod]();
}

int		cDRAW::WIDTH		=	0;
int		cDRAW::HEIGHT		=	0;
WORD	*cDRAW::p2SCREEN	=	NULL;
UINT	*cDRAW::p4SCREEN	=	NULL;
int		cDRAW::halfScreen	=	0;
int		cDRAW::halfWidth	=	0;
int		cDRAW::byteWidth	=	0;
int		cDRAW::dotPerScreen	=	0;
int		cDRAW::bytePerScreen=	0;

#define	dSPRITE_BPP16		0
#define	dSPRITE_FLIP		11
#define	dSPRITE_ZOOM		22
#define	dSPRITE_BPP8		44

void
initSprite()
{	int		spriteTerm;

	if (g_iPixelFormat	==	dRGB555)
	{	spriteTerm		=	0;
		fnPutSprite[dPUT_NORMAL						]=	c16BITSPRITE555::PutSprite;
		fnPutSprite[dPUT_HALF_BLENDING				]=	c16BITSPRITE555::PutSpriteHB;
		fnPutSprite[dPUT_ALPHA_BLENDING				]=	c16BITSPRITE555::PutSpriteAB;
		fnPutSprite[dPUT_COLOR						]=	c16BITSPRITE555::PutSpriteColor;
		fnPutSprite[dPUT_SOFT_DODGE					]=	c16BITSPRITE555::PutSpriteFX;
		fnPutSprite[dPUT_DIFFERENT					]=	c16BITSPRITE555::PutSpriteFX;
		fnPutSprite[dPUT_DODGE_BURN					]=	c16BITSPRITE555::PutSpriteFX;
		fnPutSprite[dPUT_SOFT						]=	c16BITSPRITE555::PutSpriteFX;
		fnPutSprite[dPUT_DODGE						]=	c16BITSPRITE555::PutSpriteFX;
		fnPutSprite[dPUT_LIGHT						]=	c16BITSPRITE555::PutSpriteFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT				]=	c16BITSPRITE555::PutSpriteFX;

		spriteTerm	=	dSPRITE_FLIP;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c16BITSPRITE555::fPutSprite;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c16BITSPRITE555::fPutSpriteHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c16BITSPRITE555::fPutSpriteAB;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c16BITSPRITE555::fPutSpriteColor;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c16BITSPRITE555::fPutSpriteFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c16BITSPRITE555::fPutSpriteFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c16BITSPRITE555::fPutSpriteFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c16BITSPRITE555::fPutSpriteFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c16BITSPRITE555::fPutSpriteFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c16BITSPRITE555::fPutSpriteFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c16BITSPRITE555::fPutSpriteFX;

		spriteTerm	=	dSPRITE_ZOOM;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c16BITSPRITE555::PutSpriteR;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c16BITSPRITE555::PutSpriteRHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c16BITSPRITE555::PutSpriteRAB;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c16BITSPRITE555::PutSpriteRColor;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c16BITSPRITE555::PutSpriteRFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c16BITSPRITE555::PutSpriteRFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c16BITSPRITE555::PutSpriteRFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c16BITSPRITE555::PutSpriteRFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c16BITSPRITE555::PutSpriteRFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c16BITSPRITE555::PutSpriteRFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c16BITSPRITE555::PutSpriteRFX;

		spriteTerm	=	dSPRITE_FLIP+dSPRITE_ZOOM;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c16BITSPRITE555::fPutSpriteR;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c16BITSPRITE555::fPutSpriteRHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c16BITSPRITE555::fPutSpriteRAB;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c16BITSPRITE555::fPutSpriteRColor;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c16BITSPRITE555::fPutSpriteRFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c16BITSPRITE555::fPutSpriteRFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c16BITSPRITE555::fPutSpriteRFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c16BITSPRITE555::fPutSpriteRFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c16BITSPRITE555::fPutSpriteRFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c16BITSPRITE555::fPutSpriteRFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c16BITSPRITE555::fPutSpriteRFX;


		//	8비트 스프라이트
		spriteTerm	=	dSPRITE_BPP8;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c8BITSPRITE555::PutSprite;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c8BITSPRITE555::PutSpriteHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c8BITSPRITE555::PutSpriteAB;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c8BITSPRITE555::PutSpriteColor;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c8BITSPRITE555::PutSpriteFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c8BITSPRITE555::PutSpriteFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c8BITSPRITE555::PutSpriteFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c8BITSPRITE555::PutSpriteFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c8BITSPRITE555::PutSpriteFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c8BITSPRITE555::PutSpriteFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c8BITSPRITE555::PutSpriteFX;

		spriteTerm	=	dSPRITE_BPP8+dSPRITE_FLIP;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c8BITSPRITE555::fPutSprite;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c8BITSPRITE555::fPutSpriteHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c8BITSPRITE555::fPutSpriteAB;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c8BITSPRITE555::fPutSpriteColor;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c8BITSPRITE555::fPutSpriteFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c8BITSPRITE555::fPutSpriteFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c8BITSPRITE555::fPutSpriteFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c8BITSPRITE555::fPutSpriteFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c8BITSPRITE555::fPutSpriteFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c8BITSPRITE555::fPutSpriteFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c8BITSPRITE555::fPutSpriteFX;

		spriteTerm	=	dSPRITE_BPP8+dSPRITE_ZOOM;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c8BITSPRITE555::PutSpriteR;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c8BITSPRITE555::PutSpriteRHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c8BITSPRITE555::PutSpriteRAB;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c8BITSPRITE555::PutSpriteRColor;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c8BITSPRITE555::PutSpriteRFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c8BITSPRITE555::PutSpriteRFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c8BITSPRITE555::PutSpriteRFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c8BITSPRITE555::PutSpriteRFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c8BITSPRITE555::PutSpriteRFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c8BITSPRITE555::PutSpriteRFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c8BITSPRITE555::PutSpriteRFX;

		spriteTerm	=	dSPRITE_BPP8+dSPRITE_ZOOM+dSPRITE_FLIP;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c8BITSPRITE555::fPutSpriteR;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c8BITSPRITE555::fPutSpriteRHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c8BITSPRITE555::fPutSpriteRAB;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c8BITSPRITE555::fPutSpriteRColor;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c8BITSPRITE555::fPutSpriteRFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c8BITSPRITE555::fPutSpriteRFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c8BITSPRITE555::fPutSpriteRFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c8BITSPRITE555::fPutSpriteRFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c8BITSPRITE555::fPutSpriteRFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c8BITSPRITE555::fPutSpriteRFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c8BITSPRITE555::fPutSpriteRFX;

		//	레이어
		spriteTerm	=	0;
		fnPutLayer[dPUT_NORMAL						]=	cLAYER555::PutLayer;
		fnPutLayer[dPUT_HALF_BLENDING				]=	cLAYER555::PutLayerHB;
		fnPutLayer[dPUT_ALPHA_BLENDING				]=	cLAYER555::PutLayerAB;
		fnPutLayer[dPUT_SOFT_DODGE					]=	cLAYER555::PutLayerFX;
		fnPutLayer[dPUT_DIFFERENT					]=	cLAYER555::PutLayerFX;
		fnPutLayer[dPUT_DODGE_BURN					]=	cLAYER555::PutLayerFX;
		fnPutLayer[dPUT_SOFT						]=	cLAYER555::PutLayerFX;
		fnPutLayer[dPUT_DODGE						]=	cLAYER555::PutLayerFX;
		fnPutLayer[dPUT_LIGHT						]=	cLAYER555::PutLayerFX;
		fnPutLayer[dPUT_EXTREAM_LIGHT				]=	cLAYER555::PutLayerFX;

		spriteTerm	=	dSPRITE_FLIP;
		fnPutLayer[dPUT_NORMAL			+spriteTerm	]=	cLAYER555::fPutLayer;
		fnPutLayer[dPUT_HALF_BLENDING	+spriteTerm	]=	cLAYER555::fPutLayerHB;
		fnPutLayer[dPUT_ALPHA_BLENDING	+spriteTerm	]=	cLAYER555::fPutLayerAB;
		fnPutLayer[dPUT_SOFT_DODGE		+spriteTerm	]=	cLAYER555::fPutLayerFX;
		fnPutLayer[dPUT_DIFFERENT		+spriteTerm	]=	cLAYER555::fPutLayerFX;
		fnPutLayer[dPUT_DODGE_BURN		+spriteTerm	]=	cLAYER555::fPutLayerFX;
		fnPutLayer[dPUT_SOFT			+spriteTerm	]=	cLAYER555::fPutLayerFX;
		fnPutLayer[dPUT_DODGE			+spriteTerm	]=	cLAYER555::fPutLayerFX;
		fnPutLayer[dPUT_LIGHT			+spriteTerm	]=	cLAYER555::fPutLayerFX;
		fnPutLayer[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	cLAYER555::fPutLayerFX;

		spriteTerm	=	dSPRITE_ZOOM;
		fnPutLayer[dPUT_NORMAL			+spriteTerm	]=	cLAYER555::PutLayerR;
		fnPutLayer[dPUT_HALF_BLENDING	+spriteTerm	]=	cLAYER555::PutLayerRHB;
		fnPutLayer[dPUT_ALPHA_BLENDING	+spriteTerm	]=	cLAYER555::PutLayerRAB;
		fnPutLayer[dPUT_SOFT_DODGE		+spriteTerm	]=	cLAYER555::PutLayerRFX;
		fnPutLayer[dPUT_DIFFERENT		+spriteTerm	]=	cLAYER555::PutLayerRFX;
		fnPutLayer[dPUT_DODGE_BURN		+spriteTerm	]=	cLAYER555::PutLayerRFX;
		fnPutLayer[dPUT_SOFT			+spriteTerm	]=	cLAYER555::PutLayerRFX;
		fnPutLayer[dPUT_DODGE			+spriteTerm	]=	cLAYER555::PutLayerRFX;
		fnPutLayer[dPUT_LIGHT			+spriteTerm	]=	cLAYER555::PutLayerRFX;
		fnPutLayer[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	cLAYER555::PutLayerRFX;

		spriteTerm	=	dSPRITE_FLIP+dSPRITE_ZOOM;
		fnPutLayer[dPUT_NORMAL			+spriteTerm	]=	cLAYER555::fPutLayerR;
		fnPutLayer[dPUT_HALF_BLENDING	+spriteTerm	]=	cLAYER555::fPutLayerRHB;
		fnPutLayer[dPUT_ALPHA_BLENDING	+spriteTerm	]=	cLAYER555::fPutLayerRAB;
		fnPutLayer[dPUT_SOFT_DODGE		+spriteTerm	]=	cLAYER555::fPutLayerRFX;
		fnPutLayer[dPUT_DIFFERENT		+spriteTerm	]=	cLAYER555::fPutLayerRFX;
		fnPutLayer[dPUT_DODGE_BURN		+spriteTerm	]=	cLAYER555::fPutLayerRFX;
		fnPutLayer[dPUT_SOFT			+spriteTerm	]=	cLAYER555::fPutLayerRFX;
		fnPutLayer[dPUT_DODGE			+spriteTerm	]=	cLAYER555::fPutLayerRFX;
		fnPutLayer[dPUT_LIGHT			+spriteTerm	]=	cLAYER555::fPutLayerRFX;
		fnPutLayer[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	cLAYER555::fPutLayerRFX;
	}
	else
	{	spriteTerm		=	0;
		fnPutSprite[dPUT_NORMAL						]=	c16BITSPRITE555::PutSprite;
		fnPutSprite[dPUT_COLOR						]=	c16BITSPRITE555::PutSpriteColor;
		fnPutSprite[dPUT_HALF_BLENDING				]=	c16BITSPRITE565::PutSpriteHB;
		fnPutSprite[dPUT_ALPHA_BLENDING				]=	c16BITSPRITE565::PutSpriteAB;
		fnPutSprite[dPUT_SOFT_DODGE					]=	c16BITSPRITE565::PutSpriteFX;
		fnPutSprite[dPUT_DIFFERENT					]=	c16BITSPRITE565::PutSpriteFX;
		fnPutSprite[dPUT_DODGE_BURN					]=	c16BITSPRITE565::PutSpriteFX;
		fnPutSprite[dPUT_SOFT						]=	c16BITSPRITE565::PutSpriteFX;
		fnPutSprite[dPUT_DODGE						]=	c16BITSPRITE565::PutSpriteFX;
		fnPutSprite[dPUT_LIGHT						]=	c16BITSPRITE565::PutSpriteFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT				]=	c16BITSPRITE565::PutSpriteFX;

		spriteTerm	=	dSPRITE_FLIP;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c16BITSPRITE555::fPutSprite;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c16BITSPRITE555::fPutSpriteColor;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c16BITSPRITE565::fPutSpriteHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c16BITSPRITE565::fPutSpriteAB;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c16BITSPRITE565::fPutSpriteFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c16BITSPRITE565::fPutSpriteFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c16BITSPRITE565::fPutSpriteFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c16BITSPRITE565::fPutSpriteFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c16BITSPRITE565::fPutSpriteFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c16BITSPRITE565::fPutSpriteFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c16BITSPRITE565::fPutSpriteFX;

		spriteTerm	=	dSPRITE_ZOOM;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c16BITSPRITE555::PutSpriteR;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c16BITSPRITE555::PutSpriteRColor;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c16BITSPRITE565::PutSpriteRHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c16BITSPRITE565::PutSpriteRAB;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c16BITSPRITE565::PutSpriteRFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c16BITSPRITE565::PutSpriteRFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c16BITSPRITE565::PutSpriteRFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c16BITSPRITE565::PutSpriteRFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c16BITSPRITE565::PutSpriteRFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c16BITSPRITE565::PutSpriteRFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c16BITSPRITE565::PutSpriteRFX;

		spriteTerm	=	dSPRITE_FLIP+dSPRITE_ZOOM;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c16BITSPRITE555::fPutSpriteR;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c16BITSPRITE555::fPutSpriteRColor;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c16BITSPRITE565::fPutSpriteRHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c16BITSPRITE565::fPutSpriteRAB;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c16BITSPRITE565::fPutSpriteRFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c16BITSPRITE565::fPutSpriteRFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c16BITSPRITE565::fPutSpriteRFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c16BITSPRITE565::fPutSpriteRFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c16BITSPRITE565::fPutSpriteRFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c16BITSPRITE565::fPutSpriteRFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c16BITSPRITE565::fPutSpriteRFX;

		//	8비트 스프라이트
		spriteTerm	=	dSPRITE_BPP8;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c8BITSPRITE555::PutSprite;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c8BITSPRITE555::PutSpriteColor;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c8BITSPRITE565::PutSpriteHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c8BITSPRITE565::PutSpriteAB;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c8BITSPRITE565::PutSpriteFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c8BITSPRITE565::PutSpriteFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c8BITSPRITE565::PutSpriteFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c8BITSPRITE565::PutSpriteFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c8BITSPRITE565::PutSpriteFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c8BITSPRITE565::PutSpriteFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c8BITSPRITE565::PutSpriteFX;

		spriteTerm	=	dSPRITE_BPP8+dSPRITE_FLIP;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c8BITSPRITE555::fPutSprite;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c8BITSPRITE555::fPutSpriteColor;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c8BITSPRITE565::fPutSpriteHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c8BITSPRITE565::fPutSpriteAB;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c8BITSPRITE565::fPutSpriteFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c8BITSPRITE565::fPutSpriteFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c8BITSPRITE565::fPutSpriteFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c8BITSPRITE565::fPutSpriteFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c8BITSPRITE565::fPutSpriteFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c8BITSPRITE565::fPutSpriteFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c8BITSPRITE565::fPutSpriteFX;

		spriteTerm	=	dSPRITE_BPP8+dSPRITE_ZOOM;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c8BITSPRITE555::PutSpriteR;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c8BITSPRITE555::PutSpriteRColor;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c8BITSPRITE565::PutSpriteRHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c8BITSPRITE565::PutSpriteRAB;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c8BITSPRITE565::PutSpriteRFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c8BITSPRITE565::PutSpriteRFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c8BITSPRITE565::PutSpriteRFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c8BITSPRITE565::PutSpriteRFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c8BITSPRITE565::PutSpriteRFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c8BITSPRITE565::PutSpriteRFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c8BITSPRITE565::PutSpriteRFX;

		spriteTerm	=	dSPRITE_BPP8+dSPRITE_ZOOM+dSPRITE_FLIP;
		fnPutSprite[dPUT_NORMAL			+spriteTerm	]=	c8BITSPRITE555::fPutSpriteR;
		fnPutSprite[dPUT_COLOR			+spriteTerm	]=	c8BITSPRITE555::fPutSpriteRColor;
		fnPutSprite[dPUT_HALF_BLENDING	+spriteTerm	]=	c8BITSPRITE565::fPutSpriteRHB;
		fnPutSprite[dPUT_ALPHA_BLENDING	+spriteTerm	]=	c8BITSPRITE565::fPutSpriteRAB;
		fnPutSprite[dPUT_SOFT_DODGE		+spriteTerm	]=	c8BITSPRITE565::fPutSpriteRFX;
		fnPutSprite[dPUT_DIFFERENT		+spriteTerm	]=	c8BITSPRITE565::fPutSpriteRFX;
		fnPutSprite[dPUT_DODGE_BURN		+spriteTerm	]=	c8BITSPRITE565::fPutSpriteRFX;
		fnPutSprite[dPUT_SOFT			+spriteTerm	]=	c8BITSPRITE565::fPutSpriteRFX;
		fnPutSprite[dPUT_DODGE			+spriteTerm	]=	c8BITSPRITE565::fPutSpriteRFX;
		fnPutSprite[dPUT_LIGHT			+spriteTerm	]=	c8BITSPRITE565::fPutSpriteRFX;
		fnPutSprite[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	c8BITSPRITE565::fPutSpriteRFX;


		//	레이어
		spriteTerm	=	0;
		fnPutLayer[dPUT_NORMAL						]=	cLAYER555::PutLayer;
		fnPutLayer[dPUT_HALF_BLENDING				]=	cLAYER565::PutLayerHB;
		fnPutLayer[dPUT_ALPHA_BLENDING				]=	cLAYER565::PutLayerAB;
		fnPutLayer[dPUT_SOFT_DODGE					]=	cLAYER565::PutLayerFX;
		fnPutLayer[dPUT_DIFFERENT					]=	cLAYER565::PutLayerFX;
		fnPutLayer[dPUT_DODGE_BURN					]=	cLAYER565::PutLayerFX;
		fnPutLayer[dPUT_SOFT						]=	cLAYER565::PutLayerFX;
		fnPutLayer[dPUT_DODGE						]=	cLAYER565::PutLayerFX;
		fnPutLayer[dPUT_LIGHT						]=	cLAYER565::PutLayerFX;
		fnPutLayer[dPUT_EXTREAM_LIGHT				]=	cLAYER565::PutLayerFX;

		spriteTerm	=	dSPRITE_FLIP;
		fnPutLayer[dPUT_NORMAL			+spriteTerm	]=	cLAYER555::fPutLayer;
		fnPutLayer[dPUT_HALF_BLENDING	+spriteTerm	]=	cLAYER565::fPutLayerHB;
		fnPutLayer[dPUT_ALPHA_BLENDING	+spriteTerm	]=	cLAYER565::fPutLayerAB;
		fnPutLayer[dPUT_SOFT_DODGE		+spriteTerm	]=	cLAYER565::fPutLayerFX;
		fnPutLayer[dPUT_DIFFERENT		+spriteTerm	]=	cLAYER565::fPutLayerFX;
		fnPutLayer[dPUT_DODGE_BURN		+spriteTerm	]=	cLAYER565::fPutLayerFX;
		fnPutLayer[dPUT_SOFT			+spriteTerm	]=	cLAYER565::fPutLayerFX;
		fnPutLayer[dPUT_DODGE			+spriteTerm	]=	cLAYER565::fPutLayerFX;
		fnPutLayer[dPUT_LIGHT			+spriteTerm	]=	cLAYER565::fPutLayerFX;
		fnPutLayer[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	cLAYER565::fPutLayerFX;

		spriteTerm	=	dSPRITE_ZOOM;
		fnPutLayer[dPUT_NORMAL			+spriteTerm	]=	cLAYER555::PutLayerR;
		fnPutLayer[dPUT_HALF_BLENDING	+spriteTerm	]=	cLAYER565::PutLayerRHB;
		fnPutLayer[dPUT_ALPHA_BLENDING	+spriteTerm	]=	cLAYER565::PutLayerRAB;
		fnPutLayer[dPUT_SOFT_DODGE		+spriteTerm	]=	cLAYER565::PutLayerRFX;
		fnPutLayer[dPUT_DIFFERENT		+spriteTerm	]=	cLAYER565::PutLayerRFX;
		fnPutLayer[dPUT_DODGE_BURN		+spriteTerm	]=	cLAYER565::PutLayerRFX;
		fnPutLayer[dPUT_SOFT			+spriteTerm	]=	cLAYER565::PutLayerRFX;
		fnPutLayer[dPUT_DODGE			+spriteTerm	]=	cLAYER565::PutLayerRFX;
		fnPutLayer[dPUT_LIGHT			+spriteTerm	]=	cLAYER565::PutLayerRFX;
		fnPutLayer[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	cLAYER565::PutLayerRFX;

		spriteTerm	=	dSPRITE_FLIP+dSPRITE_ZOOM;
		fnPutLayer[dPUT_NORMAL			+spriteTerm	]=	cLAYER555::fPutLayerR;
		fnPutLayer[dPUT_HALF_BLENDING	+spriteTerm	]=	cLAYER565::fPutLayerRHB;
		fnPutLayer[dPUT_ALPHA_BLENDING	+spriteTerm	]=	cLAYER565::fPutLayerRAB;
		fnPutLayer[dPUT_SOFT_DODGE		+spriteTerm	]=	cLAYER565::fPutLayerRFX;
		fnPutLayer[dPUT_DIFFERENT		+spriteTerm	]=	cLAYER565::fPutLayerRFX;
		fnPutLayer[dPUT_DODGE_BURN		+spriteTerm	]=	cLAYER565::fPutLayerRFX;
		fnPutLayer[dPUT_SOFT			+spriteTerm	]=	cLAYER565::fPutLayerRFX;
		fnPutLayer[dPUT_DODGE			+spriteTerm	]=	cLAYER565::fPutLayerRFX;
		fnPutLayer[dPUT_LIGHT			+spriteTerm	]=	cLAYER565::fPutLayerRFX;
		fnPutLayer[dPUT_EXTREAM_LIGHT	+spriteTerm	]=	cLAYER565::fPutLayerRFX;
	}
}


#define	dPUT_SOFT_DODGE			0	//	불효과
#define	dPUT_DIFFERENT			1	//	중독
#define	dPUT_DODGE_BURN			2	//	불에 그을린 효과
#define	dPUT_SOFT				3	//	연기..
#define	dPUT_DODGE				4	//	빛계열 마법
#define	dPUT_LIGHT				5	//	다중 투영효과..
#define	dPUT_EXTREAM_LIGHT		6	//	무쟈게 밝게..

void
makeFXTable()
{	int		spr,bg;

	if (g_iPixelFormat == dRGB555)
	{	for (spr=0;spr<32;spr++)
			for (bg=0;bg<32;bg++)
			{	int soft=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));

				redEFFECT[dPUT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr))<<10;
				redEFFECT[dPUT_DODGE_BURN]		[spr][bg]=(min(((bg*(32-spr)))/32 , 31))<<10;
				redEFFECT[dPUT_SOFT]			[spr][bg]=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31))<<10;
				redEFFECT[dPUT_DODGE]			[spr][bg]=(min((bg*32)/(32-spr),31))<<10;
				redEFFECT[dPUT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,31))<<10;
				redEFFECT[dPUT_LIGHT]			[spr][bg]=(max(bg,spr))<<10;
				redEFFECT[dPUT_SOFT_DODGE]		[spr][bg]=(min((soft*32)/(32-spr),31))<<10;
			}

		for (spr=0;spr<32;spr++)
			for (bg=0;bg<32;bg++)
			{	int soft=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));

				greenEFFECT[dPUT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr))<<5;
				greenEFFECT[dPUT_DODGE_BURN]	[spr][bg]=(min(((bg*(32-spr)))/32 , 31))<<5;
				greenEFFECT[dPUT_SOFT]			[spr][bg]=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31))<<5;
				greenEFFECT[dPUT_DODGE]			[spr][bg]=(min((bg*32)/(32-spr),31))<<5;
				greenEFFECT[dPUT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,31))<<5;
				greenEFFECT[dPUT_LIGHT]			[spr][bg]=(max(bg,spr))<<5;
				greenEFFECT[dPUT_SOFT_DODGE]	[spr][bg]=(min((soft*32)/(32-spr),31))<<5;
			}

		for (spr=0;spr<32;spr++)
			for (bg=0;bg<32;bg++)
			{	int soft=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));

				blueEFFECT[dPUT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr));
				blueEFFECT[dPUT_DODGE_BURN]		[spr][bg]=(min(((bg*(32-spr)))/32 , 31));
				blueEFFECT[dPUT_SOFT]			[spr][bg]=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));
				blueEFFECT[dPUT_DODGE]			[spr][bg]=(min((bg*32)/(32-spr),31));
				blueEFFECT[dPUT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,31));
				blueEFFECT[dPUT_LIGHT]			[spr][bg]=(max(bg,spr));
				blueEFFECT[dPUT_SOFT_DODGE]		[spr][bg]=(min((soft*32)/(32-spr),31));
			}
	}
	if (g_iPixelFormat == dRGB565)
	{	for (spr=0;spr<32;spr++)
			for (bg=0;bg<32;bg++)
			{	int soft=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));

				redEFFECT[dPUT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr))<<11;
				redEFFECT[dPUT_DODGE_BURN]		[spr][bg]=(min(((bg*(32-spr)))/32 , 31))<<11;
				redEFFECT[dPUT_SOFT]			[spr][bg]=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31))<<11;
				redEFFECT[dPUT_DODGE]			[spr][bg]=(min((bg*32)/(32-spr),31))<<11;
				redEFFECT[dPUT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,31))<<11;
				redEFFECT[dPUT_LIGHT]			[spr][bg]=(max(bg,spr))<<11;
				redEFFECT[dPUT_SOFT_DODGE]		[spr][bg]=(min((soft*32)/(32-spr),31))<<11;
			}

		for (spr=0;spr<64;spr++)
			for (bg=0;bg<64;bg++)
			{	int soft=(min(max(bg,spr)+((64-max(bg,spr))*min(bg,spr)/64),63));
				greenEFFECT[dPUT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr))<<5;
				greenEFFECT[dPUT_DODGE_BURN]	[spr][bg]=(min(((bg*(64-spr)))/64 , 63))<<5;
				greenEFFECT[dPUT_SOFT]			[spr][bg]=(min(max(bg,spr)+((64-max(bg,spr))*min(bg,spr)/64),63))<<5;
				greenEFFECT[dPUT_DODGE]			[spr][bg]=(min((bg*64)/(64-spr),63))<<5;
				greenEFFECT[dPUT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,63))<<5;
				greenEFFECT[dPUT_LIGHT]			[spr][bg]=(max(bg,spr))<<5;
				greenEFFECT[dPUT_SOFT_DODGE]	[spr][bg]=(min((soft*64)/(64-spr),63))<<5;
			}

		for (spr=0;spr<32;spr++)
			for (bg=0;bg<32;bg++)
			{	int soft=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));
				blueEFFECT[dPUT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr));
				blueEFFECT[dPUT_DODGE_BURN]		[spr][bg]=(min(((bg*(32-spr)))/32 , 31));
				blueEFFECT[dPUT_SOFT]			[spr][bg]=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));
				blueEFFECT[dPUT_DODGE]			[spr][bg]=(min((bg*32)/(32-spr),31));
				blueEFFECT[dPUT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,31));
				blueEFFECT[dPUT_LIGHT]			[spr][bg]=(max(bg,spr));
				blueEFFECT[dPUT_SOFT_DODGE]		[spr][bg]=(min((soft*32)/(32-spr),31));
			}
	}
	if (g_iPixelFormat == dBGR565)
	{	for (spr=0;spr<32;spr++)
			for (bg=0;bg<32;bg++)
			{	int soft=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));

				redEFFECT[dPUT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr))<<11;
				redEFFECT[dPUT_DODGE_BURN]		[spr][bg]=(min(((bg*(32-spr)))/32 , 31))<<11;
				redEFFECT[dPUT_SOFT]			[spr][bg]=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31))<<11;
				redEFFECT[dPUT_DODGE]			[spr][bg]=(min((bg*32)/(32-spr),31))<<11;
				redEFFECT[dPUT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,31))<<11;
				redEFFECT[dPUT_LIGHT]			[spr][bg]=(max(bg,spr))<<11;
				redEFFECT[dPUT_SOFT_DODGE]		[spr][bg]=(min((soft*32)/(32-spr),31))<<11;
			}

		for (spr=0;spr<64;spr++)
			for (bg=0;bg<64;bg++)
			{	int soft=(min(max(bg,spr)+((64-max(bg,spr))*min(bg,spr)/64),63));

				greenEFFECT[dPUT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr))<<5;
				greenEFFECT[dPUT_DODGE_BURN]	[spr][bg]=(min(((bg*(64-spr)))/64 , 63))<<5;
				greenEFFECT[dPUT_SOFT]			[spr][bg]=(min(max(bg,spr)+((64-max(bg,spr))*min(bg,spr)/64),63))<<5;
				greenEFFECT[dPUT_DODGE]			[spr][bg]=(min((bg*64)/(64-spr),63))<<5;
				greenEFFECT[dPUT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,63))<<5;
				greenEFFECT[dPUT_LIGHT]			[spr][bg]=(max(bg,spr))<<5;
				greenEFFECT[dPUT_SOFT_DODGE]	[spr][bg]=(min((soft*64)/(64-spr),63))<<5;
			}

		for (spr=0;spr<32;spr++)
			for (bg=0;bg<32;bg++)
			{	int soft=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));

				blueEFFECT[dPUT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr));
				blueEFFECT[dPUT_DODGE_BURN]		[spr][bg]=(min(((bg*(32-spr)))/32 , 31));
				blueEFFECT[dPUT_SOFT]			[spr][bg]=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));
				blueEFFECT[dPUT_DODGE]			[spr][bg]=(min((bg*32)/(32-spr),31));
				blueEFFECT[dPUT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,31));
				blueEFFECT[dPUT_LIGHT]			[spr][bg]=(max(bg,spr));
				blueEFFECT[dPUT_SOFT_DODGE]		[spr][bg]=(min(((soft*(32-spr)))/32 , 31));
			}
	}
}

void
makeColorTable()
{	
	_WHITE		=	RGBmix(255,255,255);
	_BLACK		=	RGBmix(  0,  0,  0);

	_LTGRAY		=	RGBmix(180,180,180);
	_GRAY 		=	RGBmix(128,128,128);
	_DEEPGRAY	=	RGBmix( 64, 64, 64);

	_LTCYAN 	=	RGBmix(  0,255,255);
	_CYAN		=	RGBmix(  0,128,128);
	_DEEPCYAN	=	RGBmix(  0, 64, 64);

	_LTBLUE 	=	RGBmix(  0,  0,255);
	_BLUE		=	RGBmix(  0,  0,128);
	_DEEPBLUE	=	RGBmix(  0,  0, 64);

	_LTRED  	=	RGBmix(255,  0,  0);
	_RED	 	=	RGBmix(128,  0,  0);
	_DEEPRED 	=	RGBmix( 64,  0,  0);

	_LTGREEN	=	RGBmix(  0,255,  0);
	_GREEN		=	RGBmix(  0,128,  0);
	_DEEPGREEN	=	RGBmix(  0, 64,  0);

	_LTYELLOW	=	RGBmix(255,255,  0);
	_YELLOW		=	RGBmix(128,128,  0);
	_DEEPYELLOW	=	RGBmix( 64, 64,  0);

	_LTPURPLE	=	RGBmix(255,  0,255);
	_PURPLE		=	RGBmix(128,  0,128);
	_DEEPPURPLE	=	RGBmix( 64,  0, 64);

	_SKYBLUE	=	RGBmix(165,203,247);
}

BOOL
cDRAW::Init()
{
	initSprite();
	makeFXTable();
	makeColorTable();
//	initMagFilter();

	return TRUE;
}

BOOL
cDRAW::Active(WORD *screen,int width,int height)
{	p2SCREEN		=	screen;
	p4SCREEN		=	(UINT *)p2SCREEN;

	WIDTH			=	width;
	HEIGHT			=	height;

	halfScreen		=	WIDTH*HEIGHT/2;
	halfWidth		=	WIDTH/2;
	byteWidth		=	WIDTH*2;
	dotPerScreen	=	WIDTH*HEIGHT;
	bytePerScreen	=	WIDTH*HEIGHT*2;

	s_rectClippingArea.set(0,0,WIDTH,HEIGHT);

	return TRUE;
}

/****************************************************************************
선긋는 함수들..
void	XLine(WORD color,int x1,int x2,int y,int thick)
void	YLine(WORD color,int y1,int y2,int x,int thick)
void	XLineAB(WORD color,int alpha,int x1,int x2,int y,int thick)
void	YLineAB(WORD color,int alpha,int y1,int y2,int x,int thick)
void	XLineHB(WORD color,int x1,int x2,int y,int thick)
void	YLineHB(WORD color,int y1,int y2,int x,int thick)

*****************************************************************************/

void
cDRAW::PutPixel(WORD color,int x,int y)
{
	if	(x < 0		)
		return;
	if	(y < 0		)
		return;
	if	(x >= WIDTH	)
		return;
	if	(y >= HEIGHT	)
		return;

	p2SCREEN[x+y*WIDTH]	=	color;
}

void
cDRAW::PutPixelHB(WORD color,int x,int y)
{
	if	(x < 0		)
		return;
	if	(y < 0		)
		return;
	if	(x >= WIDTH	)
		return;
	if	(y >= HEIGHT)
		return;

	color	=	color&d2HM_MASK_555;

	if	(g_iPixelFormat == dRGB555)
		p2SCREEN[x+y*WIDTH]	=	((p2SCREEN[x+y*WIDTH]&d2HM_MASK_555)>>1)+(color>>1);
	else
		p2SCREEN[x+y*WIDTH]	=	((p2SCREEN[x+y*WIDTH]&d2HM_MASK_565)>>1)+(color>>1);
}

void
cDRAW::PutPixelAB	(WORD color,int x,int y,int alpha)
{	
	if (x < 0		) return;
	if (y < 0		) return;
	if (x >= WIDTH	) return;
	if (y >= HEIGHT	) return;

	if	(g_iPixelFormat == dRGB555)
		p2SCREEN[x+y*WIDTH]	=	MIX2BYTE555(color,p2SCREEN[x+y*WIDTH],alpha,32-alpha);
	else
		p2SCREEN[x+y*WIDTH]	=	MIX2BYTE565(color,p2SCREEN[x+y*WIDTH],alpha,32-alpha);
}

void
cDRAW::Line(WORD color,int x1,int y1,int x2,int y2)
{	int	slant,address,len=0,x=0;

	if (x2==x1)	{YLine(color,y1,y2,x1);return;}
	if (y2==y1)	{XLine(color,x1,x2,y1);return;}

	slant	=	(x2-x1)*(y2-y1);

	int width	=	max(x2,x1)-min(x2,x1)+1;
	int height	=	max(y2,y1)-min(y2,y1)+1;

	if (x1>x2)
	{
		address	=	x1;
		x1		=	x2;
		x2		=	address;

		address	=	y1;
		y1		=	y2;
		y2		=	address;
	}

	address	=	x1+y1*WIDTH;

	if (slant > 0)
	{	if (width< height)
			for (int i=0;i<height;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 >= HEIGHT) return;

				if (x1 >= 0 && y1 >= 0)	p2SCREEN[address+x]	=	color;

				len					+=	width;
				address				+=	WIDTH;
				y1++;
				if (len >= height)	len-=height,x++,x1++;
			}
		else
			for (int i=0;i<width;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 >= HEIGHT) return;

				if (x1 >= 0 && y1 >= 0)	p2SCREEN[address+i]	=	color;

				len					+=	height;
				x1++;
				if (len >= width ) len-=width,address+=WIDTH,y1++;
			}
	}
	else
	{	if (width< height)
			for (int i=0;i<height;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 <  0		) return;

				if (x1 >= 0 && y1 < HEIGHT) 	p2SCREEN[address+x]	=	color;

				len					+=	width;
				address				-=	WIDTH;
				y1--;
				if (len >= height)	len-=height,x++,x1++;
			}
		else
			for (int i=0;i<width;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 <  0     ) return;

				if (x1 >= 0 && y1 < HEIGHT) 	p2SCREEN[address+i]	=	color;

				len					+=	height;
				x1++;
				if (len >= width ) len-=width,address-=WIDTH,y1--;
			}
	}
}

void
cDRAW::BrightLine(int _iBright,int x1,int y1,int x2,int y2)
{
	int	slant,address,len=0,x=0;

	slant	=	(x2-x1)*(y2-y1);

	int width	=	max(x2,x1)-min(x2,x1)+1;
	int height	=	max(y2,y1)-min(y2,y1)+1;

	if (y1==y2)
	{
		int	xs,i,y=y1;

		if (x1	>	x2) 
			swap(x1,x2);

		x1	=	max(x1,0);
		x2	=	min(x2,WIDTH-1);

		if (y < 0	||	y	>= HEIGHT)
			return;

		xs	=	x2-x1+1;

		if (xs <= 0 )
			return;

		int	iAddr	=	x1+y*WIDTH;

		for (i=0;i<xs;i++)
			p2SCREEN[iAddr+i]	=	GetBrightColor(p2SCREEN[iAddr+i],_iBright);

		return;
	}

	if (x1==x2)
	{
		int	ys,i,x=x1;

		if (y1	>	y2) 
			swap(y1,y2);

		y1	=	max(y1,0);
		y2	=	min(y2,HEIGHT-1);

		if (x	<0	||	x	>= WIDTH)
			return;

		ys	=	y2-y1+1;

		if (ys <= 0 )
			return;

		int	iAddr	=	x+y1*WIDTH;

		for (i=0;i<ys;i++,iAddr+=WIDTH)
			p2SCREEN[iAddr]	=	GetBrightColor(p2SCREEN[iAddr],_iBright);

		return;
	}

	if (x1>x2)
	{
		address	=	x1;
		x1		=	x2;
		x2		=	address;

		address	=	y1;
		y1		=	y2;
		y2		=	address;
	}

	address	=	x1+y1*WIDTH;

	if (slant > 0)
	{
		if (width< height)
			for (int i=0;i<height;i++)
			{
				if (x1 >= WIDTH )
					return;
				if (y1 >= HEIGHT)
					return;

				if (x1 >= 0 && y1 >= 0)
					p2SCREEN[address+x]	=	GetBrightColor(p2SCREEN[address+x],_iBright);

				len					+=	width;
				address				+=	WIDTH;
				y1++;
				if (len >= height)
					len-=height,x++,x1++;
			}
		else
			for (int i=0;i<width;i++)
			{	
				if (x1 >= WIDTH ) 
					return;
				if (y1 >= HEIGHT) 
					return;

				if (x1 >= 0 && y1 >= 0)
					p2SCREEN[address+i]	=	GetBrightColor(p2SCREEN[address+i],_iBright);

				len					+=	height;
				x1++;
				if (len >= width ) 
					len-=width,address+=WIDTH,y1++;
			}
	}
	else
	{
		if (width< height)
			for (int i=0;i<height;i++)
			{	
				if (x1 >= WIDTH ) 
					return;
				if (y1 <  0		) 
					return;

				if (x1 >= 0 && y1 < HEIGHT)
					p2SCREEN[address+x]	=	GetBrightColor(p2SCREEN[address+x],_iBright);

				len					+=	width;
				address				-=	WIDTH;
				y1--;
				if (len >= height)	
					len-=height,x++,x1++;
			}
		else
			for (int i=0;i<width;i++)
			{	
				if (x1 >= WIDTH ) 
					return;
				if (y1 <  0     ) 
					return;

				if (x1 >= 0 && y1 < HEIGHT)
					p2SCREEN[address+i]	=	GetBrightColor(p2SCREEN[address+i],_iBright);

				len					+=	height;
				x1++;
				if (len >= width ) 
					len-=width,address-=WIDTH,y1--;
			}
	}
}

void
cDRAW::LineHB(WORD color,int x1,int y1,int x2,int y2)
{	
	int	slant,address,len=0,x=0;

	if (x2==x1)	{YLineHB(color,y1,y2,x1);return;}
	if (y2==y1)	{XLineHB(color,x1,x2,y1);return;}

	slant	=	(x2-x1)*(y2-y1);

	int width	=	max(x2,x1)-min(x2,x1)+1;
	int height	=	max(y2,y1)-min(y2,y1)+1;

	if (x1>x2)
	{
		address	=	x1;
		x1		=	x2;
		x2		=	address;

		address	=	y1;
		y1		=	y2;
		y2		=	address;
	}

	address	=	x1+y1*WIDTH;

	if (g_iPixelFormat==dRGB555)
	{
		color	=	color&d2HM_MASK_555;
		if (slant > 0)
		{	if (width< height)
				for (int i=0;i<height;i++)
				{	if (x1 >= WIDTH ) return;
					if (y1 >= HEIGHT) return;

					if (x1 >= 0 && y1 >= 0)
						p2SCREEN[address+x]	=	((p2SCREEN[address+x]&d2HM_MASK_555)>>1)+(color>>1);

					len					+=	width;
					address				+=	WIDTH;
					y1++;
					if (len >= height)	len-=height,x++,x1++;
				}
			else
				for (int i=0;i<width;i++)
				{	if (x1 >= WIDTH ) return;
					if (y1 >= HEIGHT) return;

					if (x1 >= 0 && y1 >= 0)
						p2SCREEN[address+i]	=	((p2SCREEN[address+i]&d2HM_MASK_555)>>1)+(color>>1);

					len					+=	height;
					x1++;
					if (len >= width ) len-=width,address+=WIDTH,y1++;
				}
		}
		else
		{	if (width< height)
				for (int i=0;i<height;i++)
				{	if (x1 >= WIDTH ) return;
					if (y1 <  0		) return;

					if (x1 >= 0 && y1 < HEIGHT)
						p2SCREEN[address+x]	=	((p2SCREEN[address+x]&d2HM_MASK_555)>>1)+(color>>1);

					len					+=	width;
					address				-=	WIDTH;
					y1--;
					if (len >= height)	len-=height,x++,x1++;
				}
			else
				for (int i=0;i<width;i++)
				{	if (x1 >= WIDTH ) return;
					if (y1 <  0     ) return;

					if (x1 >= 0 && y1 < HEIGHT)
						p2SCREEN[address+i]	=	((p2SCREEN[address+i]&d2HM_MASK_555)>>1)+(color>>1);

					len					+=	height;
					x1++;
					if (len >= width ) len-=width,address-=WIDTH,y1--;
				}
		}

		return;
	}

	color	=	color&d2HM_MASK_565;
	if (slant > 0)
	{	if (width< height)
			for (int i=0;i<height;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 >= HEIGHT) return;

				if (x1 >= 0 && y1 >= 0)
					p2SCREEN[address+x]	=	((p2SCREEN[address+x]&d2HM_MASK_565)>>1)+(color>>1);

				len					+=	width;
				address				+=	WIDTH;
				y1++;
				if (len >= height)	len-=height,x++,x1++;
			}
		else
			for (int i=0;i<width;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 >= HEIGHT) return;

				if (x1 >= 0 && y1 >= 0)
					p2SCREEN[address+i]	=	((p2SCREEN[address+i]&d2HM_MASK_565)>>1)+(color>>1);

				len					+=	height;
				x1++;
				if (len >= width ) len-=width,address+=WIDTH,y1++;
			}
	}
	else
	{	if (width< height)
			for (int i=0;i<height;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 <  0		) return;

				if (x1 >= 0 && y1 < HEIGHT)
					p2SCREEN[address+x]	=	((p2SCREEN[address+x]&d2HM_MASK_565)>>1)+(color>>1);

				len					+=	width;
				address				-=	WIDTH;
				y1--;
				if (len >= height)	len-=height,x++,x1++;
			}
		else
			for (int i=0;i<width;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 <  0     ) return;

				if (x1 >= 0 && y1 < HEIGHT)
					p2SCREEN[address+i]	=	((p2SCREEN[address+i]&d2HM_MASK_565)>>1)+(color>>1);

				len					+=	height;
				x1++;
				if (len >= width ) len-=width,address-=WIDTH,y1--;
			}
	}

}

void
cDRAW::LineAB(WORD color,int alpha,int x1,int y1,int x2,int y2)
{
	int	slant,address,len=0,x=0;

	if (x2==x1)	{YLineAB(color,alpha,y1,y2,x1);return;}
	if (y2==y1)	{XLineAB(color,alpha,x1,x2,y1);return;}

	slant	=	(x2-x1)*(y2-y1);

	int width	=	max(x2,x1)-min(x2,x1)+1;
	int height	=	max(y2,y1)-min(y2,y1)+1;

	if (x1>x2)
	{
		address	=	x1;
		x1		=	x2;
		x2		=	address;

		address	=	y1;
		y1		=	y2;
		y2		=	address;
	}

	address	=	x1+y1*WIDTH;

	if (g_iPixelFormat	==	dRGB555)
	{
		if (slant > 0)
		{	if (width< height)
				for (int i=0;i<height;i++)
				{	if (x1 >= WIDTH ) return;
					if (y1 >= HEIGHT) return;

					if (x1 >= 0 && y1 >= 0)	
						p2SCREEN[address+x]	=	MIX2BYTE555(color,p2SCREEN[address+x],alpha,32-alpha);

					len					+=	width;
					address				+=	WIDTH;
					y1++;
					if (len >= height)	len-=height,x++,x1++;
				}
			else
				for (int i=0;i<width;i++)
				{	if (x1 >= WIDTH ) return;
					if (y1 >= HEIGHT) return;

					if (x1 >= 0 && y1 >= 0)
						p2SCREEN[address+i]	=	MIX2BYTE555(color,p2SCREEN[address+i],alpha,32-alpha);

					len					+=	height;
					x1++;
					if (len >= width ) len-=width,address+=WIDTH,y1++;
				}
		}
		else
		{	if (width< height)
				for (int i=0;i<height;i++)
				{	if (x1 >= WIDTH ) return;
					if (y1 <  0		) return;

					if (x1 >= 0 && y1 < HEIGHT)
						p2SCREEN[address+x]	=	MIX2BYTE555(color,p2SCREEN[address+x],alpha,32-alpha);

					len					+=	width;
					address				-=	WIDTH;
					y1--;
					if (len >= height)	len-=height,x++,x1++;
				}
			else
				for (int i=0;i<width;i++)
				{	if (x1 >= WIDTH ) return;
					if (y1 <  0     ) return;

					if (x1 >= 0 && y1 < HEIGHT)
						p2SCREEN[address+i]	=	MIX2BYTE555(color,p2SCREEN[address+i],alpha,32-alpha);

					len					+=	height;
					x1++;
					if (len >= width ) len-=width,address-=WIDTH,y1--;
				}
		}

		return;
	}
	if (slant > 0)
	{	if (width< height)
			for (int i=0;i<height;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 >= HEIGHT) return;

				if (x1 >= 0 && y1 >= 0)	
					p2SCREEN[address+x]	=	MIX2BYTE565(color,p2SCREEN[address+x],alpha,32-alpha);

				len					+=	width;
				address				+=	WIDTH;
				y1++;
				if (len >= height)	len-=height,x++,x1++;
			}
		else
			for (int i=0;i<width;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 >= HEIGHT) return;

				if (x1 >= 0 && y1 >= 0)
					p2SCREEN[address+i]	=	MIX2BYTE565(color,p2SCREEN[address+i],alpha,32-alpha);

				len					+=	height;
				x1++;
				if (len >= width ) len-=width,address+=WIDTH,y1++;
			}
	}
	else
	{	if (width< height)
			for (int i=0;i<height;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 <  0		) return;

				if (x1 >= 0 && y1 < HEIGHT)
					p2SCREEN[address+x]	=	MIX2BYTE565(color,p2SCREEN[address+x],alpha,32-alpha);

				len					+=	width;
				address				-=	WIDTH;
				y1--;
				if (len >= height)	len-=height,x++,x1++;
			}
		else
			for (int i=0;i<width;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 <  0     ) return;

				if (x1 >= 0 && y1 < HEIGHT)
					p2SCREEN[address+i]	=	MIX2BYTE565(color,p2SCREEN[address+i],alpha,32-alpha);

				len					+=	height;
				x1++;
				if (len >= width ) len-=width,address-=WIDTH,y1--;
			}
	}
}

void
cDRAW::XLine(WORD color,int x1,int x2,int y,int thick)
{	
	int	xs,i;

	if (x1	>	x2) 
		swap(x1,x2);

	x1	=	max(x1,0);
	x2	=	min(x2,WIDTH-1);

	if (y+thick > HEIGHT)
		thick = HEIGHT-y;
	if (y < 0 )
	{	
		if (y+thick < 0)
			return;
		thick=thick+y;
		y=0;
	}

	xs=x2-x1+1;
	if (xs <= 0 ) return;

	UINT	fc,*p4_scr,addr=0,addr2=0;		

	fc=(UINT) (color+(color<<16));			//	4바이트로 만듦..

	addr=x1+y*WIDTH;

	for (i=0;i<thick;i++,addr+=WIDTH)
	{	p4_scr=(UINT *)&p2SCREEN[addr];		//	4바이트 주소..
		__asm
		{
			mov  edi, dword ptr [ p4_scr]
			mov  ax,  color
			ror  eax, 16
			mov  ax,  color
			mov  ecx, xs

			shr  ecx, 1
			rep stosd                       // 4바이트 단위로 전송한다.
						
			mov  ecx, xs
			test ecx, 1
			jz   end_xline
			stosw                       // 한점이 남을때 다시 찍는다.
			end_xline:
		}
	}
}

void
cDRAW::YLine(WORD color,int y1,int y2,int x,int thick)
{	int	ys,i,j;

	if (y1	>	y2)	swap(y1,y2);

	y1	=	max(y1,0);
	y2	=	min(y2,HEIGHT-1);

	if (x+thick > WIDTH ) thick = WIDTH-x;
	if (x < 0 )
	{	if (x+thick < 0) return;
		thick=thick+x;
		x=0;
	}

	ys=y2-y1+1;
	if (ys <= 0 ) return;

	int addr=x+y1*WIDTH;

	for (i=0;i<ys;i++,addr+=WIDTH)	for (j=0;j<thick;j++)	p2SCREEN[addr+j]=color;
}

void
cDRAW::XLineAB(WORD color,int alpha,int x1,int x2,int y,int thick)
{
	if (x1 > x2)
	{
		int	x=	x1;

		x1	=	x2;
		x2	=	x;
	}
	FillAB(color,x1,y,x2,y+thick-1,alpha);
}

void
cDRAW::YLineAB(WORD color,int alpha,int y1,int y2,int x,int thick)
{
	if (y1 > y2)
	{
		int	y=	y1;

		y1	=	y2;
		y2	=	y;
	}
	FillAB(color,x,y1,x+thick-1,y2,alpha);
}

void
cDRAW::XLineHB(WORD color,int x1,int x2,int y,int thick)
{
	if (x1 > x2)
	{
		int	x=	x1;

		x1	=	x2;
		x2	=	x;
	}

	FillHB(color,x1,y,x2,y+thick-1);
}

void
cDRAW::YLineHB(WORD color,int y1,int y2,int x,int thick)
{
	if (y1 > y2)
	{
		int	y=	y1;

		y1	=	y2;
		y2	=	y;
	}
		
	FillHB(color,x,y1,x+thick-1,y2);
}

//-------------------------------------------------------------------------
//void	Box(WORD color,int x1,int y1,int x2,int y2,int thick)
//	:	걍 박스..
//void	BoxHB(WORD color,int x1,int y1,int x2,int y2,int thick)
//	:	반투명 박스..
//void	BoxAB(WORD color,int alpha,int x1,int y1,int x2,int y2,int thick)
//	:	알파값 있는 박스..
//-------------------------------------------------------------------------

void
cDRAW::Box(WORD color,int x1,int y1,int x2,int y2,int thick)
{
	if (x1 > x2)
		swap(x1,x2);

	if (y1 > y2)
		swap(y1,y2);

	XLine(color,x1,x2,y1		,thick);
	XLine(color,x1,x2,y2-thick+1,thick);
	YLine(color,y1,y2,x1		,thick);
	YLine(color,y1,y2,x2-thick+1,thick);
}

void
cDRAW::BrightBox(int _iBright,int x1,int y1,int x2,int y2,int thick)
{
	int	i;

	for (i=0;i<thick;i++)
		BrightLine(_iBright,x1+1,y1-i,x2-1,y1-i);

	for (i=0;i<thick;i++)
		BrightLine(_iBright,x1+1,y2+i,x2-1,y2+i);

	for (i=0;i<thick;i++)
		BrightLine(_iBright,x1-i,y1+1-thick,x1-i,y2-1+thick);

	for (i=0;i<thick;i++)
		BrightLine(_iBright,x2+i,y1+1-thick,x2+i,y2-1+thick);
}

void
cDRAW::BoxHB(WORD color,int x1,int y1,int x2,int y2,int thick)
{	if (x1 > x2) swap(x1,x2);
	if (y1 > y2) swap(y1,y2);
	XLineHB(color,x1,x2,y1		  ,thick);
	XLineHB(color,x1,x2,y2-thick+1,thick);
	YLineHB(color,y1,y2,x1		  ,thick);
	YLineHB(color,y1,y2,x2-thick+1,thick);
}

void
cDRAW::BoxAB(WORD color,int alpha,int x1,int y1,int x2,int y2,int thick)
{	if (x1 > x2) swap(x1,x2);
	if (y1 > y2) swap(y1,y2);
	XLineAB(color,alpha,x1,x2,y1		,thick);
	XLineAB(color,alpha,x1,x2,y2-thick+1,thick);
	YLineAB(color,alpha,y1,y2,x1		,thick);
	YLineAB(color,alpha,y1,y2,x2-thick+1,thick);
}

//
//	원을 그린다.
//
void
cDRAW::Circle(WORD _wColor,int _iX,int _iY,int _iRadius,int _iStep)
{
	int			iAngle;
	CPos		Pos,OldPos;

	OldPos.x	=	_iX;
	OldPos.y	=	_iY;

	GetAnglePos(&OldPos,0,_iRadius);

	for (iAngle=_iStep;iAngle<360;iAngle+=_iStep)
	{
		Pos.x	=	_iX;
		Pos.y	=	_iY;

		GetAnglePos(&Pos,iAngle,_iRadius);

		cDRAW::Line(_wColor,OldPos.x,OldPos.y,Pos.x,Pos.y);
		OldPos	=	Pos;
	}

	Pos.x	=	_iX;
	Pos.y	=	_iY;

	GetAnglePos(&Pos,0,_iRadius);

	cDRAW::Line(_wColor,OldPos.x,OldPos.y,Pos.x,Pos.y);
}	//	cDRAW::Circle(WORD _wColor,int _iX,int _iY,int _iRadius,int _iStep)

//
//	2:1의 타원을 그린다.
void
cDRAW::OvalCircle(WORD _wColor,int _iX,int _iY,int _iRadius,int _iStep)
{
	int			iAngle;
	CPos		Pos,OldPos;

	OldPos.x	=	_iX;
	OldPos.y	=	_iY;

	GetOvalAnglePos(&OldPos,0,_iRadius);

	for (iAngle=_iStep;iAngle<360;iAngle+=_iStep)
	{
		Pos.x	=	_iX;
		Pos.y	=	_iY;

		GetOvalAnglePos(&Pos,iAngle,_iRadius);

		cDRAW::LineAB(_wColor,24,OldPos.x,OldPos.y,Pos.x,Pos.y);
		OldPos	=	Pos;
	}

	Pos.x	=	_iX;
	Pos.y	=	_iY;

	GetOvalAnglePos(&Pos,0,_iRadius);

	cDRAW::LineAB(_wColor,24,OldPos.x,OldPos.y,Pos.x,Pos.y);
}	//	cDRAW::Circle(WORD _wColor,int _iX,int _iY,int _iRadius,int _iStep)

//
//	부분적인걸-_-? 그린다.
void
cDRAW::ParticleOvalCircle(WORD _wColor,int _iX,int _iY,int _iRadius,int _iAngle,int _iAngleRange,int _iStep)
{
	int			iAngle,iDestAngle=_iAngle+_iAngleRange;
	CPos		Pos,OldPos;

	OldPos.x	=	_iX;
	OldPos.y	=	_iY;

	GetOvalAnglePos(&OldPos,iDestAngle%360,_iRadius);

	for (iAngle=iDestAngle-_iStep;iAngle>_iAngle;iAngle-=_iStep)
	{
		Pos.x	=	_iX;
		Pos.y	=	_iY;

		GetOvalAnglePos(&Pos,iAngle%360,_iRadius);

		cDRAW::Line(_wColor,OldPos.x,OldPos.y,Pos.x,Pos.y);
		OldPos	=	Pos;
	}

	Pos.x	=	_iX;
	Pos.y	=	_iY;

	GetOvalAnglePos(&Pos,_iAngle%360,_iRadius);

	cDRAW::Line(_wColor,OldPos.x,OldPos.y,Pos.x,Pos.y);
}	//	cDRAW::Circle(WORD _wColor,int _iX,int _iY,int _iRadius,int _iStep)

//
//	부분적인걸-_-? 그린다.
void
cDRAW::DrawParticleGradationOvalCircle(WORD _wColor,int _iX,int _iY,int _iRadius,int _iAngle,int _iAngleRange,int _iGradationStep,int _iGradationTimes,int _iStep)
{
	int			iAngle,iDestAngle=_iAngle+_iAngleRange,iAlpha=32;
	CPos		Pos,OldPos;
	int			iGradationTimes	=	0;

	OldPos.x	=	_iX;
	OldPos.y	=	_iY;

	GetOvalAnglePos(&OldPos,iDestAngle%360,_iRadius);

	for (iAngle=iDestAngle-_iStep;iAngle>_iAngle;iAngle-=_iStep)
	{
		Pos.x	=	_iX;
		Pos.y	=	_iY;

		GetOvalAnglePos(&Pos,iAngle%360,_iRadius);

		cDRAW::LineAB(_wColor,iAlpha,OldPos.x,OldPos.y,Pos.x,Pos.y);
		OldPos	=	Pos;

		iGradationTimes++;

		if (iGradationTimes	>=	_iGradationTimes)
		{
			iAlpha	-=	_iGradationStep;
			iAlpha	=	max(iAlpha,0);
		}
	}

	iAlpha	-=	_iGradationStep;
	iAlpha	=	max(iAlpha,0);
	Pos.x	=	_iX;
	Pos.y	=	_iY;

	GetOvalAnglePos(&Pos,_iAngle%360,_iRadius);

	cDRAW::LineAB(_wColor,iAlpha,OldPos.x,OldPos.y,Pos.x,Pos.y);
}	//	cDRAW::Circle(WORD _wColor,int _iX,int _iY,int _iRadius,int _iStep)

/****************************************************************************
void	Clear()								화면을 깨까시..
void	Fill(WORD color)					normal	
void	FillHB(WORD color)					half blending
void	FillAB(WORD color,int alpha)		alpha blending

	화면 전체를 한가지 컬러로 채운다.
*****************************************************************************/

void
cDRAW::Clear()
{	UINT *p4	=p4SCREEN;
	int	 dsize	=halfScreen;

	__asm
	{	mov  edi, dword ptr [p4]
		mov  eax, 0x00000000
		mov  ecx, dsize
		rep	 stosd                       // 4바이트 단위로 전송한다.
	}
}

void
cDRAW::Fill(WORD color)
{	if (color==0)
	{	Clear();
		return;
	}

	UINT *p4	=p4SCREEN;
	int	 dsize	=halfScreen;

	__asm
	{	mov  edi, dword ptr [p4]
		mov  ax,  color
		ror  eax, 16
		mov  ax,  color
		mov  ecx, dsize
		rep	 stosd                       // 4바이트 단위로 전송한다.
	}
}

void
cDRAW::FillHB(WORD color)
{	UINT	fc4,hc4;

	fc4	=(UINT) (color+(color<<16));

	if (g_iPixelFormat==dRGB555)
	{
		hc4	=(fc4&d4HM_MASK_555)>>1;
		for (int i=0;i<halfScreen;i++)
			p4SCREEN[i]=((p4SCREEN[i]&d4HM_MASK_555)>>1)+hc4;
	}
	else
	{	
		hc4	=(fc4&d4HM_MASK_565)>>1;
		for (int i=0;i<halfScreen;i++)
			p4SCREEN[i]=((p4SCREEN[i]&d4HM_MASK_565)>>1)+hc4;
	}
}

void
cDRAW::FillAB(WORD color,int alpha)
{
	if	(alpha	==	0)
		return;

	if	(alpha	==	32)
	{
		Fill(color);
		return;
	}

	int		dest_alpha=32-alpha;
	UINT	ac4,sc4,fc4;

	fc4	=	(UINT) (color+(color<<16));			//	4바이트로 만듦..

	if (g_iPixelFormat==	dRGB555)
	{
		ac4		=	GET4BYTEALPHAVALUE555(fc4,alpha);

		for (int i=0;i<halfScreen;i++)
		{
			sc4	=	p4SCREEN[i];
			sc4	=	GET4BYTEALPHAVALUE555(sc4,dest_alpha);

			p4SCREEN[i]	=	ac4 + sc4;
		}
	}
	else
	{
		ac4		=	GET4BYTEALPHAVALUE565(fc4,alpha);

		for (int i=0;i<halfScreen;i++)
		{
			sc4	=	p4SCREEN[i];
			sc4	=	GET4BYTEALPHAVALUE565(sc4,dest_alpha);

			p4SCREEN[i]	=	ac4 + sc4;
		}
	}

	return;
}

/****************************************************************************
void	Fillxx(WORD color,int x1,int y1,int x2,int y2)

void	Fill(WORD color,int x1,int y1,int x2,int y2)					normal
void	FillHB(WORD color,int x1,int y1,int x2,int y2)					half blending
void	FillAB(WORD color,int x1,int y1,int x2,int y2,int alpha)		alpha blending
void	roundFill(WORD color,int x1,int y1,int x2,int y2)				둥그렇게..
void	roundFillHB(WORD color,int x1,int y1,int x2,int y2)				둥그렇게..
void	roundFillAB(WORD color,int x1,int y1,int x2,int y2,int alpha)	둥그렇게..

	화면 일부를 한가지 컬러로 채운다.
*****************************************************************************/


void
cDRAW::Fill(WORD color,int x1,int y1,int x2,int y2)
{
	int	xs,ys,i,j;

	x1=max(x1,0);
	y1=max(y1,0);
	x2=min(x2,WIDTH-1);
	y2=min(y2,HEIGHT-1);
// 	x2=max(x2,x1);
// 	y2=max(y2,y1);

	xs=x2-x1+1;
	ys=y2-y1+1;

	if (xs <= 0  || ys <= 0)
		return;

	UINT	fc4,*p4_scr,addr=0;

	fc4=(UINT) (color+(color<<16));			//	4바이트로 만듦..
	p4_scr=(UINT *)&p2SCREEN[x1+y1*WIDTH];	//	4바이트 주소..

	if (xs%2)							//	4바이트로 나누어지는 경우..
	{
		int mica=x2+y1*WIDTH;

		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
		{
			for (j=0;j<xs/2;j++)
				p4_scr[addr+j]=fc4;

			p2SCREEN[mica]=color;			//	안 나눠 지니까.. 쪼가리가 남는다..
		}
	}
	else
	{	
		for (i=0;i<ys;i++,addr+=halfWidth)
			for (j=0;j<xs/2;j++)
				p4_scr[addr+j]=fc4;
	}
}

void
cDRAW::FillHB(WORD color,int x1,int y1,int x2,int y2)
{	int	xs,ys,i,j;

	x1=max(x1,0);
	y1=max(y1,0);
	x2=min(x2,WIDTH-1);
	y2=min(y2,HEIGHT-1);

	xs=x2-x1+1;
	ys=y2-y1+1;

	if (xs <= 0  || ys <= 0) return;

	UINT	fc4,*p4_scr,addr=0,hc4;
	WORD	hc2;

	p4_scr	=	(UINT *)&p2SCREEN[x1+y1*WIDTH];	//	4바이트 주소..

	if (g_iPixelFormat == dRGB555)
	{	fc4=(UINT) (color+(color<<16));			//	4바이트로 만듦..
		hc4=(fc4&d4HM_MASK_555)>>1;
		hc2=(color&d2HM_MASK_555)>>1;

		if (xs%2)							//	4바이트로 나누어지는 경우..
		{	int	mica=x2+y1*WIDTH;

			for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
			{	for (j=0;j<xs/2;j++) p4_scr[addr+j]=((p4_scr[addr+j]&d4HM_MASK_555)>>1)+hc4;
				p2SCREEN[mica]=((p2SCREEN[mica]&d2HM_MASK_555)>>1)+hc2;			//	안 나눠 지니까.. 쪼가리가 남는다..
			}
		}
		else	
		{	for (i=0;i<ys;i++,addr+=halfWidth)
				for (j=0;j<xs/2;j++)
					p4_scr[addr+j]=((p4_scr[addr+j]&d4HM_MASK_555)>>1)+hc4;
		}
	}
	else
	{	fc4=(UINT) (color+(color<<16));			//	4바이트로 만듦..
		hc4=(fc4&d4HM_MASK_565)>>1;
		hc2=(color&d2HM_MASK_565)>>1;

		if (xs%2)							//	4바이트로 나누어지는 경우..
		{	int	mica=x2+y1*WIDTH;

			for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
			{	for (j=0;j<xs/2;j++) p4_scr[addr+j]=((p4_scr[addr+j]&d4HM_MASK_565)>>1)+hc4;
				p2SCREEN[mica]=((p2SCREEN[mica]&d2HM_MASK_565)>>1)+hc2;			//	안 나눠 지니까.. 쪼가리가 남는다..
			}
		}
		else	
		{	for (i=0;i<ys;i++,addr+=halfWidth)
				for (j=0;j<xs/2;j++)
					p4_scr[addr+j]=((p4_scr[addr+j]&d4HM_MASK_565)>>1)+hc4;
		}
	}


}

void
cDRAW::FillAB(WORD color,int x1,int y1,int x2,int y2,int alpha)
{
	int	xs,ys,i,j;

	x1=max(x1,0);
	y1=max(y1,0);
	x2=min(x2,WIDTH-1);
	y2=min(y2,HEIGHT-1);

	xs=x2-x1+1;
	ys=y2-y1+1;

	if (xs <= 0  || ys <= 0) return;

	int		dest_alpha=32-alpha;
	UINT	*p4_scr,addr=0,ac4,sc4,fc4;
	WORD	ac2,sc2;

	fc4		=	(UINT) (color+(color<<16));			//	4바이트로 만듦..
	p4_scr	=	(UINT*)&p2SCREEN[x1+y1*WIDTH];	//	4바이트 주소..

	if (g_iPixelFormat == dRGB555)
	{
		ac4		=	GET4BYTEALPHAVALUE555(fc4,alpha);
		ac2		=	GET2BYTEALPHAVALUE555(color,alpha);

		if (xs%2==0)							//	4바이트로 나누어지는 경우..
		{
			for (i=0;i<ys;i++,addr+=halfWidth)
				for (j=0;j<xs/2;j++)
				{
					sc4		=	GET4BYTEALPHAVALUE555(p4_scr[addr+j],dest_alpha);
					p4_scr[addr+j]	=	ac4 + sc4;
				}
		}
		else									//	안나눠 지는 경우.. ==;
		{
			int mica=x2+y1*WIDTH;

			for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
			{
				for (j=0;j<xs/2;j++)
				{
					sc4		=	GET4BYTEALPHAVALUE555(p4_scr[addr+j],dest_alpha);
					p4_scr[addr+j]	=	ac4 + sc4;
				}

				sc2				=	GET2BYTEALPHAVALUE555(p2SCREEN[mica],dest_alpha);

				p2SCREEN[mica]	=	ac2 + sc2;
			}
		}
	}
	else
	{	ac4		=	GET4BYTEALPHAVALUE565(fc4,alpha);
		ac2		=	GET2BYTEALPHAVALUE565(color,alpha);

		if (xs%2==0)							//	4바이트로 나누어지는 경우..
		{	for (i=0;i<ys;i++,addr+=halfWidth)
				for (j=0;j<xs/2;j++)
				{	sc4		=	GET4BYTEALPHAVALUE565(p4_scr[addr+j],dest_alpha);
					p4_scr[addr+j]	=	ac4 + sc4;
				}
		}
		else									//	안나눠 지는 경우.. ==;
		{	int mica=x2+y1*WIDTH;

			for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
			{	for (j=0;j<xs/2;j++)
				{	sc4		=	GET4BYTEALPHAVALUE565(p4_scr[addr+j],dest_alpha);
					p4_scr[addr+j]	=	ac4 + sc4;
				}
				sc2				=	GET2BYTEALPHAVALUE565(p2SCREEN[mica],dest_alpha);

				p2SCREEN[mica]	=	ac2 + sc2;
			}
		}
	}
}

void
cDRAW::FillBright(int _iBright,int x1,int y1,int x2,int y2)
{
	int	xs,ys,i,j;

	x1=max(x1,0);
	y1=max(y1,0);
	x2=min(x2,WIDTH-1);
	y2=min(y2,HEIGHT-1);

	xs=x2-x1+1;
	ys=y2-y1+1;

	int	iAddr	=	x1+y1*WIDTH;

	if (xs <= 0  || ys <= 0)
		return;

	for (i=0;i<ys;i++,iAddr+=WIDTH)
		for (j=0;j<xs;j++)
			p2SCREEN[iAddr+j]	=	GetBrightColor(p2SCREEN[iAddr+j],_iBright);
}

/****************************************************************************
void	GetImage(int x,int y,int xs,int ys,WORD *img)
WORD*	GetImage(int x,int y,int xs,int ys)
	임의의 지점의 이미지를 읽어 온다.
****************************************************************************/

void
cDRAW::GetImage(int x,int y,int xs,int ys,WORD *img)
{
	TEST(!img,ERRMSG("cDRAW::GetImage(int x,int y,int xs,int ys,WORD *img)"));

	img[0]	=	(WORD )xs;
	img[1]	=	(WORD )ys;

	int width=xs,offset=2,addr=0,i;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0)	return;

	addr	=	x+y*WIDTH;
	xs		*=	2;

	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width) memcpy(img+offset,p2SCREEN+addr,xs);
}

WORD *
cDRAW::GetImage(int x,int y,int xs,int ys)
{	int offset=2,addr=0,i,width=xs;

	WORD *img	=	_New(WORD,(xs*ys+2),NULL,"cDRAW::GetImage img");

	img[0]	=	(WORD )xs;
	img[1]	=	(WORD )ys;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return img;

	addr	=	x+y*WIDTH;

	xs	*=	2;
	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width) memcpy(img+offset,p2SCREEN+addr,xs);

	return img;
}

WORD*
cDRAW::GetImage()
{	return GetImage(0,0,WIDTH,HEIGHT);
}

/****************************************************************************
void	PutImagexx(int x,int y,int xs,int ys,WORD *img)

void	PutImage(int x,int y,int xs,int ys,WORD *img)					normal
void	PutImageHB(int x,int y,int xs,int ys,WORD *img)					half blending
void	PutImageHC(int x,int y,int xs,int ys,WORD *img)					half converting
void	PutImageAB(int x,int y,int xs,int ys,WORD *img,int alpha)		alpha blending
void	PutImageAC(int x,int y,int xs,int ys,WORD *img,int alpha)		alpha converting
	임의의 지점에 이미지를 찍어준다.
*****************************************************************************/

void
cDRAW::PutClippedImage(int _iX,int _iY,int _iWidth,int _iHeight,WORD *_lpImage,int _iDx,int _iDy)
{
	int		iImageWidth	=	_lpImage[0];
	int		iImageHeight=	_lpImage[1];
	WORD	*lpImage	=	_lpImage+2+_iDx+_iDy*iImageWidth;
	int		iAddr		=	_iX+_iY*WIDTH;
	int		iOffset		=	0;
	int		i;

	_iWidth	=	min(iImageWidth-_iDx ,_iWidth );
	_iHeight=	min(iImageHeight-_iDy,_iHeight);
	_iWidth	*=	2;

	if (_iWidth	<=	0)
		return;

	for (i=0;i<_iHeight;i++,iAddr+=WIDTH,iOffset+=iImageWidth)
		memcpy(p2SCREEN+iAddr,lpImage+iOffset,_iWidth);

}

void
cDRAW::PutClippedImageHB(int _iX,int _iY,int _iWidth,int _iHeight,WORD *_lpImage,int _iDx,int _iDy)
{
	int		iImageWidth	=	_lpImage[0];
	int		iImageHeight=	_lpImage[1];
	WORD	*lpImage	=	_lpImage+2+_iDx+_iDy*iImageWidth;
	int		iAddr		=	0;
	int		iOffset		=	0;
	int		i,j;

	_iWidth	=	min(iImageWidth-_iDx ,_iWidth );
	_iHeight=	min(iImageHeight-_iDy,_iHeight);

	if(g_iPixelFormat == dRGB555)
	{
		if (_iWidth%2)
		{
			UINT *p4_scr=(UINT *)&p2SCREEN[_iX+_iY*WIDTH];

			int mica	=_iX+_iWidth-1+_iY*WIDTH;
			
			for (i=0;i<_iHeight;i++,iAddr+=halfWidth,mica+=WIDTH,iOffset+=iImageWidth)
			{
				UINT *p4_img=(UINT *)&lpImage[iOffset];

				for (j=0;j<_iWidth/2;j++) 
					p4_scr[iAddr+j]=(((p4_scr[iAddr+j]&d4HM_MASK_555)>>1)+((p4_img[j]&d4HM_MASK_555)>>1));
				p2SCREEN[mica]=((p2SCREEN[mica]&d2HM_MASK_555)+(lpImage[iOffset+_iWidth]&d2HM_MASK_555))>>1;
			}
		}
		else
		{	UINT *p4_scr=(UINT *)&p2SCREEN[_iX+_iY*WIDTH];
			for (i=0;i<_iHeight;i++,iAddr+=halfWidth,iOffset+=iImageWidth)
			{	UINT *p4_img=(UINT *)&lpImage[iOffset];
				for (j=0;j<_iWidth/2;j++)
					p4_scr[iAddr+j]=(((p4_scr[iAddr+j]&d4HM_MASK_555)>>1)+((p4_img[j]&d4HM_MASK_555)>>1));
			}
		}

		return;
	}

	if (_iWidth%2)
	{
		UINT *p4_scr=(UINT *)&p2SCREEN[_iX+_iY*WIDTH];

		int mica	=_iX+_iWidth-1+_iY*WIDTH;

		for (i=0;i<_iHeight;i++,iAddr+=halfWidth,mica+=WIDTH,iOffset+=iImageWidth)
		{
			UINT *p4_img=(UINT *)&lpImage[iOffset];

			for (j=0;j<_iWidth/2;j++) 
				p4_scr[iAddr+j]=(((p4_scr[iAddr+j]&d4HM_MASK_565)>>1)+((p4_img[j]&d4HM_MASK_565)>>1));
			p2SCREEN[mica]=((p2SCREEN[mica]&d2HM_MASK_565)+(lpImage[iOffset+_iWidth]&d2HM_MASK_565))>>1;
		}
	}
	else
	{	UINT *p4_scr=(UINT *)&p2SCREEN[_iX+_iY*WIDTH];
		for (i=0;i<_iHeight;i++,iAddr+=halfWidth,iOffset+=iImageWidth)
		{	UINT *p4_img=(UINT *)&lpImage[iOffset];
			for (j=0;j<_iWidth/2;j++)
				p4_scr[iAddr+j]=(((p4_scr[iAddr+j]&d4HM_MASK_565)>>1)+((p4_img[j]&d4HM_MASK_565)>>1));
		}
	}
}

void
cDRAW::PutImage(int x,int y,WORD *image)
{	TEST(!image,ERRMSG("cDRAW::PutImage(int x,int y,WORD *image)"));

	int width,offset=0,addr=0,i;
	int		xs	=	image[0],ys	=	image[1];
	WORD	*img=	&image[2];

	width	=	xs;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	addr	=	x+y*WIDTH;

	xs*=2;

	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width) memcpy(p2SCREEN+addr,img+offset,xs);
}


void
cDRAW::PutImageScale(int x,int y,WORD *image,int _iScale)
{	TEST(!image,ERRMSG("cDRAW::PutImageScale(int x,int y,WORD *image)"));


	int		addr,bufferPoint;
	int		xs	=	image[0],ys	=	image[1];
	WORD	*img=	&image[2];

	int	xLen=xs*_iScale/100				;
	int	yLen=ys*_iScale/100				;	//	확대/축소된 이미즈 사이즈

	MakeSpriteRTable(x,y,xs,ys,xLen,yLen);

//	클리핑 안해!!
//	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
//	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
//	if (x+xs> WIDTH	) xs=WIDTH	-x;
//	if (y+ys> HEIGHT) ys=HEIGHT	-y;
//	if (xs <= 0 || ys <= 0) return;

	bufferPoint	=	0;
	addr		=	x+y*WIDTH;

	for (y=0;y<ys;y++)
	{
		for (int yp=0;yp<zoomYTable[y].size;yp++)
		{
			for (x=0;x<xs;x++)
				for (int xp=0;xp<zoomXTable[x].size;xp++)
				{
					int	pos			=	addr+zoomXTable[x].pos+xp;

					p2SCREEN[pos]	=	img[bufferPoint+x];
				}

			addr		+=	WIDTH;
		}

		bufferPoint		+=	xs;
	}
}


void
cDRAW::PutImage(int x,int y,BYTE *image,WORD *plt)
{
	TEST(!image,ERRMSG("cDRAW::PutImage(int x,int y,BYTE *image,WORD *plt)"));
	TEST(!plt,ERRMSG("cDRAW::PutImage(int x,int y,BYTE *image,WORD *plt)"));

	int width,offset=0,addr=0,i,j;
	int		xs	=	*(WORD *)(image),ys	=	*(WORD *)(image + 2);
	BYTE	*img=	&image[4];

	width	=	xs;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	addr	=	x+y*WIDTH;

	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width)
		for (j=0;j<xs;j++)
			p2SCREEN[addr+j]	=	plt[img[offset+j]];
}

void
cDRAW::PutSpriteByReferenceImage(int _iX,int _iY,BYTE *_lpImage,WORD *_lpPlt,BYTE *_lpSprite)
{
	int		X1, Y1, X2, Y2 ,Xs ,Ys;
	int		XPoint=0,i,j,beginX,endX;
	int		count,bufferPoint=8;
	int		size;
	CPos	pos;

	int	iImageWidth		=	*(WORD *)_lpImage;
	int	iImageHeight	=	*(WORD *)(_lpImage+2);

	pos.x	=	0;
	pos.y	=	0;

	_x	=	_iX;
	_y	=	_iY;

	Xs	=	*(short *)(_lpSprite	);
	Ys	=	*(short *)(_lpSprite+2	);
	_x	-=	*(short *)(_lpSprite+4	);	//	위치 교정..
	_y	-=	*(short *)(_lpSprite+6	);	//	위치 교정..

	pos.x-=	*(short *)(_lpSprite+4	);
	pos.y-=	*(short *)(_lpSprite+6	);

	if (_x >= WIDTH)
		return;

	X1	=	0;
	Y1	=	0;

	if ( _x	> 0 )
		X1 = _x;
	if ( _y	> 0 )
		Y1 = _y;

	if ( _x < 0 )
		pos.x	=	-_x;
	if ( _y < 0 )
		pos.y	=	-_y;

	X2 = Xs+_x;
	Y2 = Ys+_y;

	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{
		count=_lpSprite[bufferPoint++];

		for(i=0;i<count;i++)
		{
			bufferPoint++;
			size		=	_lpSprite[bufferPoint++];
			bufferPoint	+=	size;
		}
	}

	CPos	curPos;

	curPos.x	=	pos.x;
	curPos.y	=	pos.y;

	if (_x<0 || _x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	
		count	=	_lpSprite[bufferPoint++];
		XPoint	=	_x;
		curPos.x=	pos.x;

		for(i=0;i<count;i++)
		{
			curPos.x+=	_lpSprite[bufferPoint];
			XPoint	+=	_lpSprite[bufferPoint++];
			size	=	_lpSprite[bufferPoint++];

			beginX	=	0;
			endX	=	(int) size;

			if (XPoint		<	0		)	beginX	=	-XPoint;			//	왼쪽 클리핑..
			if (XPoint+ size>	WIDTH	)	endX	=	WIDTH-XPoint;		//	오른쪽 클리핑..

			for (j=beginX;j<endX;j++)
			{
				*(p2SCREEN+addr+XPoint+j)	=	_lpPlt[_lpImage[curPos.x+curPos.y*iImageWidth+4]];
				curPos.x++;
			}

			bufferPoint	+=	size;
			XPoint		+=	size;
		}

		addr	+=	WIDTH;
		curPos.y++;
	}
	else
	for(;Y1<Y2;Y1++)
	{
		count	=	_lpSprite[bufferPoint++];
		XPoint	=	_x;
		curPos.x=	pos.x;

		for(i=0;i<count;i++)
		{	
			curPos.x+=	_lpSprite[bufferPoint];
			XPoint	+=	_lpSprite[bufferPoint++];
			size	=	_lpSprite[bufferPoint++];

			for (j=0;j<size;j++)
			{
				*(p2SCREEN+addr+XPoint+j)	=	_lpPlt[_lpImage[curPos.x+curPos.y*iImageWidth+4]];

				curPos.x++;
			}

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr	+=	WIDTH;
		curPos.y++;
	}
}

void
cDRAW::PutSpriteByReferenceImage(int _iX,int _iY,BYTE *_lpImage,WORD *_lpPlt,BYTE *_lpReferenceShape,int _iScale)
{
	if (_iScale == 100)
	{
		PutSpriteByReferenceImage(_iX,_iY,_lpImage,_lpPlt,_lpReferenceShape);
		return;
	}
}

void
cDRAW::PutSpriteByReferenceImage(int _iX,int _iY,WORD *_lpImage,BYTE *_lpReferenceShape)
{
}

void
cDRAW::PutSpriteByReferenceImage(int _iX,int _iY,WORD *_lpImage,BYTE *_lpReferenceShape,int _iScale)
{
	if (_iScale == 100)
	{
		PutSpriteByReferenceImage(_iX,_iY,_lpImage,_lpReferenceShape);
		return;
	}
}


void
cDRAW::PutImageScale(int x,int y,BYTE *image,WORD *plt,int _iScale)
{
	TEST(!image,ERRMSG("cDRAW::PutImageScale(int x,int y,WORD *image)"));

	int		addr,bufferPoint;
	int		xs	=	*(WORD *)image,ys	=	*(WORD *)(image+2);
	BYTE	*img=	image+4;

	int	xLen=xs*_iScale/100				;
	int	yLen=ys*_iScale/100				;	//	확대/축소된 이미즈 사이즈

	MakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	bufferPoint	=	0;
	addr		=	x+y*WIDTH;

	for (y=0;y<ys;y++)
	{
		for (int yp=0;yp<zoomYTable[y].size;yp++)
		{
			for (x=0;x<xs;x++)
				for (int xp=0;xp<zoomXTable[x].size;xp++)
				{
					int	pos			=	addr+zoomXTable[x].pos+xp;

					p2SCREEN[pos]	=	plt[img[bufferPoint+x]];
				}

			addr		+=	WIDTH;
		}

		bufferPoint		+=	xs;
	}
}
void
cDRAW::PutImageHB(int x,int y,WORD *image)
{
	TEST(!image,ERRMSG("cDRAW::PutImageHB(int x,int y,WORD *image)"));

	int width,offset=0,addr=0,i,j;
	int		xs	=	image[0],ys	=	image[1];
	WORD	*img=	&image[2];

	width	=	xs;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	if(g_iPixelFormat == dRGB555)
	{	if (xs%2)
		{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
			int mica=x+xs-1+y*WIDTH;
			for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH,offset+=width)
			{
				UINT *p4_img=(UINT *)&img[offset];

				for (j=0;j<xs/2;j++) 
					p4_scr[addr+j]=(((p4_scr[addr+j]&d4HM_MASK_555)>>1)+((p4_img[j]&d4HM_MASK_555)>>1));
				p2SCREEN[mica]=((p2SCREEN[mica]&d2HM_MASK_555)+(img[offset+xs]&d2HM_MASK_555))>>1;
			}
		}
		else
		{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
			for (i=0;i<ys;i++,addr+=halfWidth,offset+=width)
			{	UINT *p4_img=(UINT *)&img[offset];
				for (j=0;j<xs/2;j++)
					p4_scr[addr+j]=(((p4_scr[addr+j]&d4HM_MASK_555)>>1)+((p4_img[j]&d4HM_MASK_555)>>1));
			}
		}
	}
	else
	{	if (xs%2)
		{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
			int mica=x+xs-1+y*WIDTH;
			for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH,offset+=width)
			{
				UINT *p4_img=(UINT *)&img[offset];

				for (j=0;j<xs/2;j++) 
					p4_scr[addr+j]=(((p4_scr[addr+j]&d4HM_MASK_565)>>1)+((p4_img[j]&d4HM_MASK_565)>>1));
				p2SCREEN[mica]=((p2SCREEN[mica]&d2HM_MASK_565)+(img[offset+xs]&d2HM_MASK_565))>>1;
			}
		}
		else
		{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
			for (i=0;i<ys;i++,addr+=halfWidth,offset+=width)
			{	UINT *p4_img=(UINT *)&img[offset];
				for (j=0;j<xs/2;j++)
					p4_scr[addr+j]=(((p4_scr[addr+j]&d4HM_MASK_565)>>1)+((p4_img[j]&d4HM_MASK_565)>>1));
			}
		}
	}
}

void
cDRAW::PutImageAB(int x,int y,WORD *image,int alpha)
{	if	(alpha>=32)	{PutImage(x,y,image);return;}

	TEST(!image,ERRMSG("cDRAW::PutImageAB(int x,int y,WORD *image,int alpha)"));

	int		width,offset=0,addr=0,i,j;
	int		xs	=	image[0],ys	=	image[1];
	WORD	*img=	&image[2];
	int		dest_alpha=32-alpha;

	width	=	xs;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	if(g_iPixelFormat == dRGB555)
	{	if (xs%2)
		{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
			int mica=x+xs-1+y*WIDTH;

			for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH,offset+=width)
			{	UINT *p4_img=(UINT *)&img[offset];
				for (j=0;j<xs/2;j++)
					p4_scr[addr+j]	=	MIX4BYTE555(p4_img[j],p4_scr[addr+j],alpha,dest_alpha);

				p2SCREEN[mica]	=	MIX2BYTE555(img[offset+xs],p2SCREEN[mica],alpha,dest_alpha);
			}
		}
		else
		{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
			for (i=0;i<ys;i++,addr+=halfWidth,offset+=width)
			{	UINT *p4_img=(UINT *)&img[offset];
				for (j=0;j<xs/2;j++)
					p4_scr[addr+j]	=	MIX4BYTE555(p4_img[j],p4_scr[addr+j],alpha,dest_alpha);
			}
		}
	}
	else
	{	if (xs%2)
		{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
			int mica=x+xs-1+y*WIDTH;

			for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH,offset+=width)
			{	UINT *p4_img=(UINT *)&img[offset];
				for (j=0;j<xs/2;j++)
					p4_scr[addr+j]	=	MIX4BYTE565(p4_img[j],p4_scr[addr+j],alpha,dest_alpha);

				p2SCREEN[mica]	=	MIX2BYTE565(img[offset+xs],p2SCREEN[mica],alpha,dest_alpha);
			}
		}
		else
		{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
			for (i=0;i<ys;i++,addr+=halfWidth,offset+=width)
			{	UINT *p4_img=(UINT *)&img[offset];
				for (j=0;j<xs/2;j++)
					p4_scr[addr+j]	=	MIX4BYTE565(p4_img[j],p4_scr[addr+j],alpha,dest_alpha);
			}
		}
	}
}

/*****************************************************************************************

*****************************************************************************************/

// cSHADOW_INFO	cSHADOW::m_aShadowInfo[800][dMAX_SHADOW_THUMB];
// BYTE			cSHADOW::m_ucShadowCount[800];
// 
// void
// cSHADOW::ResetShadow()
// {	memset(m_ucShadowCount,0,800);
// }
// // 
// void
// cSHADOW::AddShadowMask(int x1,int y,int x2)
// {	TEST(m_ucShadowCount[y] >= dMAX_SHADOW_THUMB,ERRMSG("m_ucShadowCount Over flow!!"));
// 
// 	if (m_ucShadowCount[y]	==	0)
// 	{	m_aShadowInfo[y][0].begin	=	x1;
// 		m_aShadowInfo[y][0].end		=	x2;
// 		m_ucShadowCount[y]++;
// 		return;
// 	}
// 
// 	for (int i=0;i<m_ucShadowCount[y];i++)
// 	{	if (x1 <= m_aShadowInfo[y][i].begin)
// 		{	if (x2 >= m_aShadowInfo[y][i].begin)
// 			{	m_aShadowInfo[y][i].begin	=	x1;
// 
// 				if (x2 >= m_aShadowInfo[y][i].end)	m_aShadowInfo[y][i].end	=	x2;
// 				return;
// 			}
// 
// 			//	앞에 끼워 넣는다.
// 			for (int j=m_ucShadowCount[y];j>i;j--)
// 			{	m_aShadowInfo[y][j].begin	=	m_aShadowInfo[y][j-1].begin;
// 				m_aShadowInfo[y][j].end		=	m_aShadowInfo[y][j-1].end;
// 			}
// 
// 			m_aShadowInfo[y][i].begin		=	x1;
// 			m_aShadowInfo[y][i].end			=	x2;
// 
// 			m_ucShadowCount[y]++;
// 			return;
// 		}
// 
// 		if (x1 > m_aShadowInfo[y][i].begin)
// 		{	if (x1 <= m_aShadowInfo[y][i].end)
// 			{	if (x2 >= m_aShadowInfo[y][i].end)	m_aShadowInfo[y][i].end	=	x2;
// 				return;
// 			}
// 		}
// 	}
// 
// 	m_aShadowInfo[y][m_ucShadowCount[y]].begin	=	x1;
// 	m_aShadowInfo[y][m_ucShadowCount[y]].end	=	x2;
// 
// 	m_ucShadowCount[y]++;
// }
// 
// void
// cSHADOW::ShadowMask(int x,int y,BYTE *shadow)
// {	int   y2;
// 	int   xPoint=0,i;
// 	int   count,bufferPoint=8;
// 	int   size;
// 
// 	y2	=	y+*(WORD  *)(shadow+2);
// 	x	-=	*(short *)(shadow+4);	//	위치 교정..
// 	y	-=	*(short *)(shadow+6);	//	위치 교정..
// 
// 	for(y;y<y2;y++)
// 	{	count	=	shadow[bufferPoint++];
// 		xPoint	=	x;
// 
// 		for(i=0;i<count;i++)
// 		{	xPoint			+=	shadow[bufferPoint++];
// 			size			=	shadow[bufferPoint++];
// 
// 			if (y >= 0 && y < HEIGHT)
// 			{	int	x1,x2;
// 				x1	=	max(xPoint,0);
// 				x2	=	min(xPoint+size-1,WIDTH-1);
// 
// 				if (x2	>=	x1)	AddShadowMask(x1,y,x2);
// 			}
// 
// 			xPoint	+=	size;
// 		}
// 	}
// }
// 
// void
// cSHADOW::PutShadowMask()
// {
// }

void
cSHADOW::PutShadow(int x,int y,BYTE *shadow)
{	
	int		x1, y1, x2, y2 ,xs ,ys;
	int		xPoint=0,i,j,beginX,endX,width;
	int		count,bufferPoint=8;
	int		size;
	int		iPos;
	UINT	*lp4Screen;

	xs	=	*(WORD  *)(shadow  );
	ys	=	*(WORD  *)(shadow+2);
	x	-=	*(short *)(shadow+4);	//	위치 교정..
	y	-=	*(short *)(shadow+6);	//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=shadow[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint+=2;
	}

	if(g_iPixelFormat == dRGB555)
	{	
		if (x<0 || x+xs > WIDTH)
		for(;y1<y2;y1++)
		{	
			count	=	shadow[bufferPoint++];
			xPoint	=	x;

			for(i=0;i<count;i++)
			{	
				xPoint		+=	shadow[bufferPoint++];
				size		=	shadow[bufferPoint++];

				beginX		=	0;
				endX		=	(int) size;

				if (xPoint		<	0	) 
					beginX	= -xPoint;			//	왼쪽 클리핑..
				if (xPoint+ size>  WIDTH) 
					endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

				width		=	endX-beginX;

				if (width	>	0	)
				{
					lp4Screen=	(UINT *)&p2SCREEN[addr+xPoint+beginX];

					for (j=0;j<width/2;j++)
						lp4Screen[j]=(lp4Screen[j]&d4HM_MASK_555)>>1;

					if (width&1)
					{	
						iPos			=	addr+xPoint+beginX+width-1;
						p2SCREEN[iPos]	=	(p2SCREEN[iPos]&d2HM_MASK_555)>>1;
					}
				}

				xPoint	+=	size;
			}
			addr+=WIDTH;
		}
		else
		for(;y1<y2;y1++)
		{	count	=	shadow[bufferPoint++];
			xPoint	=	x;

			for(i=0;i<count;i++)
			{	xPoint	+=	shadow[bufferPoint++];
				size	=	shadow[bufferPoint++];

				lp4Screen=(UINT *)&p2SCREEN[addr+xPoint];

				for (j=0;j<size/2;j++)
					lp4Screen[j]=(lp4Screen[j]&d4HM_MASK_555)>>1;

				if ( size&1	)
				{	
					iPos	=	addr+xPoint+size-1;
					p2SCREEN[iPos]	=	(p2SCREEN[iPos]&d2HM_MASK_555)>>1;
				}

				xPoint	+=	size;
			}
			addr+=WIDTH;
		}
	}
	else
	{
		if (x<0 || x+xs > WIDTH)
		for(;y1<y2;y1++)
		{	
			count	=	shadow[bufferPoint++];
			xPoint	=	x;

			for(i=0;i<count;i++)
			{	
				xPoint		+=	shadow[bufferPoint++];
				size		=	shadow[bufferPoint++];

				beginX		=	0;
				endX		=	(int) size;

				if (xPoint		<	0	) 
					beginX	= -xPoint;			//	왼쪽 클리핑..
				if (xPoint+ size>  WIDTH) 
					endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

				width		=	endX-beginX;

				if (width	>	0	)
				{
					lp4Screen=	(UINT *)&p2SCREEN[addr+xPoint+beginX];

					for (j=0;j<width/2;j++)
						lp4Screen[j]=(lp4Screen[j]&d4HM_MASK_565)>>1;

					if (width&1)
					{	
						iPos			=	addr+xPoint+width-1;
						p2SCREEN[iPos]	=	(p2SCREEN[iPos]&d2HM_MASK_565)>>1;
					}
				}

				xPoint	+=	size;
			}
			addr+=WIDTH;
		}
		else
		for(;y1<y2;y1++)
		{	
			count	=	shadow[bufferPoint++];
			xPoint	=	x;

			for(i=0;i<count;i++)
			{
				xPoint	+=	shadow[bufferPoint++];
				size	=	shadow[bufferPoint++];

				lp4Screen=(UINT *)&p2SCREEN[addr+xPoint];

				for (j=0;j<size/2;j++)
					lp4Screen[j]=(lp4Screen[j]&d4HM_MASK_565)>>1;

				if	(size&1)
				{
					iPos			=	addr+xPoint+size-1;
					p2SCREEN[iPos]	=	(p2SCREEN[iPos]&d2HM_MASK_565)>>1;
				}

				xPoint	+=	size;
			}
			addr+=WIDTH;
		}
	}
}

void
cSHADOW::PutShadow(int x,int y,BYTE *shadow,int iAlpha)
{	
	int		x1, y1, x2, y2 ,xs ,ys;
	int		xPoint=0,i,j,beginX,endX,width;
	int		count,bufferPoint=8;
	int		size;
	int		iPos;
	UINT	*lp4Screen;

	xs	=*	(WORD  *)(shadow  );
	ys	=*	(WORD  *)(shadow+2);
	x	-=*	(short *)(shadow+4);	//	위치 교정..
	y	-=*	(short *)(shadow+6);	//	위치 교정..

	if (x >= WIDTH) return;

	x1	=	0;	if ( x > 0 ) x1 = x;
	y1	=	0;	if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=shadow[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint+=2;
	}

	iAlpha	=	min(16,iAlpha);
	iAlpha	=	32-iAlpha;

	if(g_iPixelFormat == dRGB555)
	{
		if (x<0	||	x+xs > WIDTH)
		for(;y1<y2;y1++)
		{
			count	=	shadow[bufferPoint++];
			xPoint	=	x;

			for(i=0;i<count;i++)
			{
				xPoint		+=	shadow[bufferPoint++];
				size		=	shadow[bufferPoint++];

				beginX		=	0;
				endX		=	(int) size;

				if (xPoint		<	0	) 
					beginX	= -xPoint;			//	왼쪽 클리핑..
				if (xPoint+ size>  WIDTH) 
					endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

				width		=	endX-beginX;

				if (width	>	0	)
				{
					lp4Screen=	(UINT *)&p2SCREEN[addr+xPoint+beginX];

					for (j=0;j<width/2;j++)
						lp4Screen[j]=GET4BYTEALPHAVALUE555(lp4Screen[j],iAlpha);

					if (width&1)
					{	
						iPos			=	addr+xPoint+beginX+width-1;
						p2SCREEN[iPos]	=	GET2BYTEALPHAVALUE555(p2SCREEN[iPos],iAlpha);
					}
				}

				xPoint	+=	size;
			}
			addr+=WIDTH;
		}
		else
		for(;y1<y2;y1++)
		{	count	=	shadow[bufferPoint++];
			xPoint	=	x;

			for(i=0;i<count;i++)
			{	xPoint	+=	shadow[bufferPoint++];
				size	=	shadow[bufferPoint++];

				lp4Screen=(UINT *)&p2SCREEN[addr+xPoint];

				for (j=0;j<size/2;j++)	lp4Screen[j]=GET4BYTEALPHAVALUE555(lp4Screen[j],iAlpha);
				if ( size&1	)
				{	iPos			=	addr+xPoint+size-1;
					p2SCREEN[iPos]	=	GET2BYTEALPHAVALUE555(p2SCREEN[iPos],iAlpha);
				}

				xPoint	+=	size;
			}
			addr+=WIDTH;
		}
	}
	else
	{	
		if (x<0	||	x+xs > WIDTH)
		for(;y1<y2;y1++)
		{
			count	=	shadow[bufferPoint++];
			xPoint	=	x;

			for(i=0;i<count;i++)
			{
				xPoint		+=	shadow[bufferPoint++];
				size		=	shadow[bufferPoint++];

				beginX		=	0;
				endX		=	(int) size;

				if (xPoint		<	0	) 
					beginX	= -xPoint;			//	왼쪽 클리핑..
				if (xPoint+ size>  WIDTH) 
					endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

				width		=	endX-beginX;

				if (width	>	0	)
				{
					lp4Screen=	(UINT *)&p2SCREEN[addr+xPoint+beginX];

					for (j=0;j<width/2;j++)
						lp4Screen[j]=GET4BYTEALPHAVALUE565(lp4Screen[j],iAlpha);

					if (width&1)
					{	
						iPos			=	addr+xPoint+width-1;
						p2SCREEN[iPos]	=	GET2BYTEALPHAVALUE565(p2SCREEN[iPos],iAlpha);
					}
				}

				xPoint	+=	size;
			}
			addr+=WIDTH;
		}
		else
		for(;y1<y2;y1++)
		{	count	=	shadow[bufferPoint++];
			xPoint	=	x;

			for(i=0;i<count;i++)
			{	xPoint	+=	shadow[bufferPoint++];
				size	=	shadow[bufferPoint++];

				lp4Screen=(UINT *)&p2SCREEN[addr+xPoint];

				for (j=0;j<size/2;j++)	lp4Screen[j]=GET4BYTEALPHAVALUE565(lp4Screen[j],iAlpha);
				if ( size&1			 )
				{	iPos			=	addr+xPoint+size-1;
					p2SCREEN[iPos]	=	GET2BYTEALPHAVALUE565(p2SCREEN[iPos],iAlpha);
				}

				xPoint	+=	size;
			}
			addr+=WIDTH;
		}
	}
}

void
cSHADOW::PutShadow(int x,int y,BYTE *sprite,int xRate,int yRate,int _iMX,int _iMY)
{
	if (xRate==100 && yRate==100)
	{
		PutShadow(x,y,sprite);
		return;
	}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..
	int	k,pos;
	int	iXG	=	0;
	int	iYG	=	0;
	int	iGY	=	0;

	TEST(!sprite,ERRMSG("cDRAW::PutShadow(int x,int y,BYTE *sprite,int xRate,int yRate)"));

	xp	=	*(short *)(sprite+4);
	yp	=	*(short *)(sprite+6);
	xs	=	*(WORD *)(sprite)	;
	ys	=	*(WORD *)(sprite+2)	;	//	스프라이트 사이즈

	if	(_iMX)
	{
		iXG		=	_iMX/2-xp;
		iYG		=	_iMY/2-yp;
		iGY		=	max(_iMY-ys-iYG,0);

		xs		=	_iMX;
		ys		=	_iMY;
		xp		=	_iMX/2;
		yp		=	_iMY/2;
	}

	xLen	=	xs*xRate/100				;
	yLen	=	ys*yRate/100				;	//	확대/축소된 이미즈 사이즈
	x		-=	xp*xRate/100;
	y		-=	yp*yRate/100;	//	위치 교정..

	if	(x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT ) 
		return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	ys		-=	iGY;

	if (g_iPixelFormat == dRGB555)
		for (y=0;y<ys;y++)
		{
			tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 

			for (yp=0;yp<zoomYTable[y].size;yp++)
			{
				if	(iYG	>	0)
				{
					addr		+=	WIDTH;
					continue;
				}

				count	=	sprite[bufferPoint++];	//	이미지 카운트
				xp		=	iXG;
				for(i=0;i<count;i++)
				{	xp	+=	sprite[bufferPoint++];
					size=	sprite[bufferPoint++];
					for (j=0;j<size;j++)
						for (k=0;k<zoomXTable[j+xp].size;k++)
						{
							pos	=	addr+zoomXTable[j+xp].pos+k;	
							p2SCREEN[pos]	=	(p2SCREEN[pos]&d2HM_MASK_555)>>1;
						}

					xp	+=	size;
				}
				addr		+=	WIDTH,	bufferPoint	=	tempBP;
			}
			if	(iYG	>	0)
			{
				--iYG;
				continue;
			}

			count	=	sprite[bufferPoint++];
			for(i=0;i<count;i++)	bufferPoint	+=	2;
		}
	else
		for (y=0;y<ys;y++)
		{
			tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
			for (yp=0;yp<zoomYTable[y].size;yp++)
			{
				if	(iYG	>	0)
				{
					addr		+=	WIDTH;
					continue;
				}

				count	=	sprite[bufferPoint++];	//	이미지 카운트
				xp		=	iXG;
				for(i=0;i<count;i++)
				{	xp	+=	sprite[bufferPoint++];
					size=	sprite[bufferPoint++];
					for (j=0;j<size;j++)
						for (k=0;k<zoomXTable[j+xp].size;k++)
						{	pos	=	addr+zoomXTable[j+xp].pos+k;	
							p2SCREEN[pos]	=	(p2SCREEN[pos]&d2HM_MASK_565)>>1;
						}

					xp	+=	size;
				}
				addr		+=	WIDTH,	bufferPoint	=	tempBP;
			}

			if	(iYG	>	0)
			{
				--iYG;
				continue;
			}

			count	=	sprite[bufferPoint++];

			for(i=0;i<count;i++)
				bufferPoint	+=	2;
		}
}

void
cSHADOW::PutShadow(int x,int y,BYTE *sprite,int xRate,int yRate,int iAlpha,int _iMX,int _iMY)
{
	if	(xRate==100 && yRate==100)
	{
		PutShadow(x,y,sprite,iAlpha);
		return;
	}

	if	(iAlpha	>=	32)
	{
		PutShadow(x,y,sprite,xRate,yRate,_iMX,_iMY);

		return;
	}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..
	int	k,pos;
	int	iXG	=	0;
	int	iYG	=	0;
	int	iGY	=	0;

	TEST(!sprite,ERRMSG("cDRAW::PutShadow(int x,int y,BYTE *sprite,int xRate,int yRate)"));

	xp		=	*(short *)(sprite+4);
	yp		=	*(short *)(sprite+6);
	xs		=	*(WORD *)(sprite)	;
	ys		=	*(WORD *)(sprite+2)	;	//	스프라이트 사이즈

	if	(_iMX)
	{
		iXG		=	_iMX/2-xp;
		iYG		=	_iMY/2-yp;
		iGY		=	max(_iMY-ys-iYG,0);

		xs		=	_iMX;
		ys		=	_iMY;
		xp		=	_iMX/2;
		yp		=	_iMY/2;
	}

	xLen	=	xs*xRate/100;
	yLen	=	ys*yRate/100;	//	확대/축소된 이미즈 사이즈
	x		-=	xp*xRate/100;
	y		-=	yp*yRate/100;	//	위치 교정..

	if	(x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )
		return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	iAlpha	=	min(16,iAlpha);
	iAlpha	=	32-iAlpha;

	ys		-=	iGY;

	if (g_iPixelFormat == dRGB555)
		for (y=0;y<ys;y++)
		{	
			tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
			for (yp=0;yp<zoomYTable[y].size;yp++)
			{	
				if	(iYG	>	0)
				{
					addr		+=	WIDTH;
					continue;
				}

				count	=	sprite[bufferPoint++];	//	이미지 카운트
				xp		=	iXG;
				for(i=0;i<count;i++)
				{	
					xp	+=	sprite[bufferPoint++];
					size=	sprite[bufferPoint++];
					for (j=0;j<size;j++)
						for (k=0;k<zoomXTable[j+xp].size;k++)
						{
							pos				=	addr+zoomXTable[j+xp].pos+k;	
							p2SCREEN[pos]	=	GET2BYTEALPHAVALUE555(p2SCREEN[pos],iAlpha);
						}

					xp	+=	size;
				}
				addr		+=	WIDTH,	bufferPoint	=	tempBP;
			}

			if	(iYG	>	0)
			{
				--iYG;
				continue;
			}

			count	=	sprite[bufferPoint++];
			for(i=0;i<count;i++)
				bufferPoint	+=	2;
		}
	else
		for (y=0;y<ys;y++)
		{	
			tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
			for (yp=0;yp<zoomYTable[y].size;yp++)
			{	
				if	(iYG	>	0)
				{
					addr		+=	WIDTH;
					continue;
				}

				count	=	sprite[bufferPoint++];	//	이미지 카운트
				xp		=	iXG;
				for(i=0;i<count;i++)
				{	
					xp	+=	sprite[bufferPoint++];
					size=	sprite[bufferPoint++];

					for (j=0;j<size;j++)
						for (k=0;k<zoomXTable[j+xp].size;k++)
						{	
							pos	=	addr+zoomXTable[j+xp].pos+k;	
							p2SCREEN[pos]	=	GET2BYTEALPHAVALUE565(p2SCREEN[pos],iAlpha);
						}

					xp	+=	size;
				}
				addr		+=	WIDTH,	bufferPoint	=	tempBP;
			}

			if	(iYG	>	0)
			{
				--iYG;
				continue;
			}

			count	=	sprite[bufferPoint++];
			for(i=0;i<count;i++)
				bufferPoint	+=	2;
		}
}

void
cDRAW::PutFont(int x,int y,WORD color,BYTE *font)
{
	int   x1, y1, x2, y2 ,xs ,ys,yp=0;
	int   xPoint=0,i,beginX,endX;
	int   count,bufferPoint=2;
	int   size;
	WORD  put_color=color;	

  	TEST(!font,ERRMSG("cDRAW::PutFontNormal(int x,int y,WORD color,BYTE *font,BOOL bMC)"));

	xs=font[0];
	ys=font[1];

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	if (x >= WIDTH) return;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(yp=0;yp<-y;yp++)		//	y축 상단 클리핑..
	{	count=font[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=font[bufferPoint++];
		}
	}

	if (x<0 || x+xs > WIDTH)
	for(;y1<y2;y1++,yp++)
	{	count	=	font[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];

			beginX	=	0;
			endX	=	size;

			if (xPoint			< 0		)	beginX	= -xPoint;			//	왼쪽 클리핑..
			if (xPoint+ size	> WIDTH	)	endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			if (endX			> beginX)	memset16(p2SCREEN+addr+beginX+xPoint,put_color,endX - beginX);
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];
			memset16(p2SCREEN+addr+xPoint,put_color,size);
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutFontHB(int x,int y,WORD color,BYTE *font)
{
	int   x1, y1, x2, y2 ,xs ,ys,yp=0;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=2;
	int   size;

  	TEST(!font,ERRMSG("cDRAW::PutFontHB(int x,int y,WORD color,BYTE *font,BOOL bMC)"));

	xs=font[0];
	ys=font[1];

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	if (x >= WIDTH) return;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(yp=0;yp<-y;yp++)		//	y축 상단 클리핑..
	{	count=font[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=font[bufferPoint++];
		}
	}

	if(g_iPixelFormat == dRGB555)
	{
		WORD  put_color	=	(color&d2HM_MASK_555)>>1;

		if (x<0 || x+xs > WIDTH)
		for(;y1<y2;y1++,yp++)
		{	count	=	font[bufferPoint++];

			for(i=0;i<count;i++)
			{	xPoint	=	x+font[bufferPoint++];
				size	=	font[bufferPoint++];

				beginX	=	0;
				endX	=	size;

				if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
				if (xPoint+ size	> WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

				for (j=beginX;j<endX;j++)
					p2SCREEN[+addr+xPoint+j]=((p2SCREEN[+addr+xPoint+j]&d2HM_MASK_555)>>1)+put_color;
			}

			addr	+=	WIDTH;
		}
		else
		for(;y1<y2;y1++,yp++)
		{	count=font[bufferPoint++];

			for(i=0;i<count;i++)
			{	xPoint	=	x+font[bufferPoint++];
				size	=	font[bufferPoint++];
				for (j=0;j<size;j++)
					p2SCREEN[+addr+xPoint+j]=((p2SCREEN[+addr+xPoint+j]&d2HM_MASK_555)>>1)+put_color;
			}
			addr+=WIDTH;
		}

		return;
	}

	WORD  put_color	=	(color&d2HM_MASK_565)>>1;

	if (x<0 || x+xs > WIDTH)
	for(;y1<y2;y1++,yp++)
	{	count	=	font[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];

			beginX	=	0;
			endX	=	size;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (xPoint+ size	> WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			for (j=beginX;j<endX;j++)
				p2SCREEN[+addr+xPoint+j]=((p2SCREEN[+addr+xPoint+j]&d2HM_MASK_565)>>1)+put_color;
		}

		addr	+=	WIDTH;
	}
	else
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];
			for (j=0;j<size;j++)
				p2SCREEN[+addr+xPoint+j]=((p2SCREEN[+addr+xPoint+j]&d2HM_MASK_565)>>1)+put_color;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutFontAB(int x,int y,WORD color,BYTE *font,int alpha)
{
	int   x1, y1, x2, y2 ,xs ,ys,yp=0;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=2;
	int   size,dest_alpha=32-alpha;
	WORD  put_color;
	UINT  first,second;

	TEST(!font,ERRMSG("cDRAW::PutFont(int x,int y,WORD color,BYTE *font,int alpha,BOOL bMC)"));

	xs=font[0];
	ys=font[1];

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	if (x >= WIDTH) return;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(yp=0;yp<-y;yp++)		//	y축 상단 클리핑..
	{	count=font[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=font[bufferPoint++];
		}
	}

	if(g_iPixelFormat == dRGB555)
	{
		first	=	color&d4AM_MASK_555_1;
		first	>>=	5;
		first	*=	alpha;
		first	&=	d4AM_MASK_555_1;

		second	=	color&d4AM_MASK_555_2;
		second	*=	alpha;
		second	>>=	5;
		second	&=	d4AM_MASK_555_2;

		put_color=	(WORD)(first | second);

		if (x<0 || x+xs > WIDTH)
		for(;y1<y2;y1++,yp++)
		{	
			count=font[bufferPoint++];

			for(i=0;i<count;i++)
			{	xPoint	=	x+font[bufferPoint++];
				size	=	font[bufferPoint++];

				beginX	=	0;
				endX	=	(int) size;

				if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
				if (xPoint+ size	> WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

				for (j=beginX;j<endX;j++)
				{	WORD	sc2		=	p2SCREEN[addr+xPoint+j];

					first			=	sc2&d4AM_MASK_555_1;
					first			>>=	5;
					first			*=	dest_alpha;
					first			&=	d4AM_MASK_555_1;

					second			=	sc2&d4AM_MASK_555_2;
					second			*=	dest_alpha;
					second			>>=	5;
					second			&=	d4AM_MASK_555_2;

					p2SCREEN[addr+xPoint+j]=	put_color + first + second;
				}
			}
			addr+=WIDTH;
		}
		else
		for(;y1<y2;y1++,yp++)
		{	count=font[bufferPoint++];

			for(i=0;i<count;i++)
			{	xPoint	=	x+font[bufferPoint++];
				size	=	font[bufferPoint++];
				for (j=0;j<size;j++)
				{	WORD	sc2		=	p2SCREEN[addr+xPoint+j];

					first			=	sc2&d4AM_MASK_555_1;
					first			>>=	5;
					first			*=	dest_alpha;
					first			&=	d4AM_MASK_555_1;

					second			=	sc2&d4AM_MASK_555_2;
					second			*=	dest_alpha;
					second			>>=	5;
					second			&=	d4AM_MASK_555_2;

					p2SCREEN[addr+xPoint+j]=	put_color + first + second;
				}
			}
			addr+=WIDTH;
		}

		return;
	}

	first	=	color&d4AM_MASK_565_1;
	first	>>=	5;
	first	*=	alpha;
	first	&=	d4AM_MASK_565_1;

	second	=	color&d4AM_MASK_565_2;
	second	*=	alpha;
	second	>>=	5;
	second	&=	d4AM_MASK_565_2;

	put_color=	(WORD)(first | second);

	if (x<0 || x+xs > WIDTH)
	for(;y1<y2;y1++,yp++)
	{	
		count=font[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];

			beginX	=	0;
			endX	=	(int) size;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (xPoint+ size	> WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			for (j=beginX;j<endX;j++)
			{	WORD	sc2		=	p2SCREEN[addr+xPoint+j];

				first			=	sc2&d4AM_MASK_565_1;
				first			>>=	5;
				first			*=	dest_alpha;
				first			&=	d4AM_MASK_565_1;

				second			=	sc2&d4AM_MASK_565_2;
				second			*=	dest_alpha;
				second			>>=	5;
				second			&=	d4AM_MASK_565_2;

				p2SCREEN[addr+xPoint+j]=	put_color + first + second;
			}
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];
			for (j=0;j<size;j++)
			{	WORD	sc2		=	p2SCREEN[addr+xPoint+j];

				first			=	sc2&d4AM_MASK_565_1;
				first			>>=	5;
				first			*=	dest_alpha;
				first			&=	d4AM_MASK_565_1;

				second			=	sc2&d4AM_MASK_565_2;
				second			*=	dest_alpha;
				second			>>=	5;
				second			&=	d4AM_MASK_565_2;

				p2SCREEN[addr+xPoint+j]=	put_color + first + second;
			}
		}
		addr+=WIDTH;
	}
}


void
cDRAW::PutReferenceSprite(int _iX,int _iY,WORD *_lpSprite,WORD *_lpImage)
{
	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=4;
	int	  size;

	Xs=(short)_lpSprite[0];
	Ys=(short)_lpSprite[1];
	_iX-=(short)_lpSprite[2];	//	위치 교정..
	_iY-=(short)_lpSprite[3];	//	위치 교정..

	if (_iX >= s_rectClippingArea.x2) return;
//	if (s_rectClippingArea.x1 >= s_rectClippingArea.x2) return;
//	if (s_rectClippingArea.y1 >= s_rectClippingArea.y2) return;

	X1= s_rectClippingArea.x1;if ( _iX > s_rectClippingArea.x1 ) X1 = _iX;
	Y1= s_rectClippingArea.y1;if ( _iY > s_rectClippingArea.y1 ) Y1 = _iY;

	X2 = Xs+_iX;
	Y2 = Ys+_iY;

	if (s_rectClippingArea.y2 < _iY+Ys) Y2=s_rectClippingArea.y2;	//	_iY축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_iY;j++)		//	_iY축 상단 클리핑..
	{	count=_lpSprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_lpSprite[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (_iX<s_rectClippingArea.x1)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpSprite[bufferPoint++];

		XPoint	=	_iX;

		for(i=0;i<count;i++)
		{	XPoint	+=	_lpSprite[bufferPoint++];
			size	=	_lpSprite[bufferPoint++];

			beginX	=	0;
			endX	=	(int) size;

			if (XPoint			< s_rectClippingArea.x1) beginX	= s_rectClippingArea.x1-XPoint;	//	왼쪽 클리핑..
			if (XPoint+ size	> s_rectClippingArea.x2) endX	= s_rectClippingArea.x2-XPoint;	//	오른쪽 클리핑..

			for (int iX=beginX;iX<endX;iX++)
			{
				if (g_iPixelFormat	==	dRGB565)
				{
					int	iAlpha		=	min(_lpSprite[bufferPoint+iX]*31/0xffff,31);

					p2SCREEN[addr+XPoint+iX]	=	CONVERT2BYTE565(_lpImage[addr+XPoint+iX],iAlpha);
				}
				else
				{
					int	iAlpha		=	min(_lpSprite[bufferPoint+iX]*31/0x7fff,31);

					p2SCREEN[addr+XPoint+iX]	=	CONVERT2BYTE555(_lpImage[addr+XPoint+iX],iAlpha);
				}
			}

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (_iX+Xs > s_rectClippingArea.x2)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpSprite[bufferPoint++];
		XPoint	=	_iX;
		for(i=0;i<count;i++)
		{	XPoint	+=	_lpSprite[bufferPoint++];
			size	=	_lpSprite[bufferPoint++];

			endX	=	(int) size;

			if (XPoint+ size	>  s_rectClippingArea.x2) endX	= s_rectClippingArea.x2-XPoint;		//	오른쪽 클리핑..

			for (int iX=0;iX<endX;iX++)
			{
				if (g_iPixelFormat	==	dRGB565)
				{
					int	iAlpha		=	min(_lpSprite[bufferPoint+iX]*31/0xffff,31);

					p2SCREEN[addr+XPoint+iX]	=	CONVERT2BYTE565(_lpImage[addr+XPoint+iX],iAlpha);
				}
				else
				{
					int	iAlpha		=	min(_lpSprite[bufferPoint+iX]*31/0x7fff,31);

					p2SCREEN[addr+XPoint+iX]	=	CONVERT2BYTE555(_lpImage[addr+XPoint+iX],iAlpha);
				}
			}

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_lpSprite[bufferPoint++];
		XPoint	=	_iX;

		for(i=0;i<count;i++)
		{	XPoint	+=	_lpSprite[bufferPoint++];
			size	=	_lpSprite[bufferPoint++];

			for (int iX=0;iX<size;iX++)
			{
				if (g_iPixelFormat	==	dRGB565)
				{
					int	iAlpha		=	min(_lpSprite[bufferPoint+iX]*31/0xffff,31);

					p2SCREEN[addr+XPoint+iX]	=	CONVERT2BYTE565(_lpImage[addr+XPoint+iX],iAlpha);
				}
				else
				{
					int	iAlpha		=	min(_lpSprite[bufferPoint+iX]*31/0x7fff,31);

					p2SCREEN[addr+XPoint+iX]	=	CONVERT2BYTE555(_lpImage[addr+XPoint+iX],iAlpha);
				}
			}

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
cDRAW::PutScreenReferenceBlackEffect()
{
	for (int i = 0 ; i < WIDTH * HEIGHT ; i++)
	{	
		WORD wColor = p2SCREEN[i] & 0x1f;
		p2SCREEN[i] = wColor | wColor << 5 | wColor << 10;
	}
}

void 
cDRAW::FixGaroPixel(int _iX,int _iY , int _iRadius)
{
	DWORD dwAddr	= 0;
	DWORD dwColor	= 0;
	int TempiX = _iX;
	int iCount = 0;

	for (int k = 0 ; k < _iRadius ; k++)
	{
		for (int i = 0 ; i < _iRadius; i--)		//왼쪽 5픽셀 썩기
		{
			dwAddr	= _iY * g_iScreenHeight + TempiX;
			
			if(TempiX < 0)
				break;
			
			dwColor += p2SCREEN[dwAddr];
			iCount++;
			TempiX--;
		}
		
		TempiX = _iX;
		
		for (i = 0 ; i < _iRadius; i--)		//오른쪽 5픽셀 썩기
		{
			dwAddr	= _iY * g_iScreenHeight + TempiX;
			
			if(TempiX > g_iScreenWidth)
				break;
			
			dwColor += p2SCREEN[dwAddr];
			iCount++;
			TempiX++;
		}
		
		if(iCount)
			p2SCREEN[_iY * g_iScreenHeight + _iX + k] = WORD(dwColor / iCount);
	}
}


void
cDRAW::putMagniFinder(int _iX,int _iY)
{
	//if(_iX > 700 || _iY > 500 || _iX < 100 || _iY < 100)
	//	return;

	WORD	wOrigin[50 * 50];
	int		i , j;
	DWORD	dwAddr;
	int		HalfWidth = dMAG_FILTER_WIDTH / 2 ;
	int		HalfHight = dMAG_FILTER_HIGHT / 2 ;

	for(i = 0 ; i < HalfHight ; i++)
	{
		dwAddr = (_iY + i) * g_iScreenWidth + _iX;
		memcpy(&wOrigin[i * HalfHight] , &p2SCREEN[dwAddr] , 50*2);
	}

	for(i = 0 ; i < dMAG_FILTER_HIGHT; i++)
	{
		for(j = 0 ; j < dMAG_FILTER_WIDTH; j++)
		{
			dwAddr = (_iY + i) * g_iScreenWidth + (_iX + j);

			p2SCREEN[dwAddr] = wOrigin[ _pdwMag[i * dMAG_FILTER_HIGHT + j] ];
		}
	}

};

void 
cDRAW::FixSeroPixel(int _iX,int _iY , int _iRadius)
{
	DWORD dwAddr	= 0;
	DWORD dwColor	= 0;
	int TempiY = _iY;
	int iCount = 0;
	
	for (int i = 0 ; i < _iRadius; i--)		//왼쪽 5픽셀 썩기
	{
		dwAddr	= TempiY * g_iScreenHeight + _iX;
		
		if(TempiY < 0)
			break;
		
		dwColor += p2SCREEN[dwAddr];
		iCount++;
		TempiY--;
	}
	
	TempiY = _iY;
	
	for (i = 0 ; i < _iRadius; i--)		//오른쪽 5픽셀 썩기
	{
		dwAddr	= TempiY * g_iScreenHeight + _iX;
		
		if(TempiY > g_iScreenWidth)
			break;
		
		dwColor += p2SCREEN[dwAddr];
		iCount++;
		TempiY++;
	}
	
	if(iCount)
		p2SCREEN[_iY * g_iScreenHeight + _iX] = WORD(dwColor / iCount);
}


void
cDRAW::PutScreenReferenceTvEffect(int interVal)
{
	int RandomInterVal = rand() % interVal;
	
	for (int i = 0 ; i < WIDTH * HEIGHT ; i++)
	{	
		RandomInterVal--;
		if(RandomInterVal <= 0)
		{
			p2SCREEN[i] = 0xffff;
			RandomInterVal = rand() % interVal;
		}	
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL
cDRAW::IsIn(BYTE *_lpData,int _iX,int _iY)
{
	int   XPoint=0,i,j;
	int   count,bufferPoint=8;
	int   size;

	_iX		+=	*(short *)(_lpData+4);	//	위치 교정..
	_iY		+=	*(short *)(_lpData+6);	//	위치 교정..

	if (_iX	<	0	||	_iX	>=	*(short *)(_lpData	))
		return	FALSE;
	if (_iY	<	0	||	_iY	>=	*(short *)(_lpData+2))
		return	FALSE;

	for(j=0;j<_iY;j++)		//	_y축 상단 클리핑..
	{
		count	=	_lpData[bufferPoint++];

		for(i=0;i<count;i++)
		{
			bufferPoint++;
			size		=	_lpData[bufferPoint++];
			bufferPoint	+=	size;
		}
	}

	count	=	_lpData[bufferPoint++];
	XPoint	=	0;

	for(i=0;i<count;i++)
	{
		XPoint	+=	_lpData[bufferPoint++];
		size	=	_lpData[bufferPoint++];

		if (_iX	>=	XPoint	&&	_iX	<=	XPoint+size)
			return	TRUE;

		bufferPoint	+=	size;
		XPoint		+=	size;
	}

	return	FALSE;
}

BOOL
cDRAW::IsIn(WORD	*_lpData,int _iX,int _iY)
{
	int XPoint=0,i,j;
	int count,bufferPoint=4;
	int	size;

	_iX		+=	(short)_lpData[2];	//	위치 교정..
	_iY		+=	(short)_lpData[3];	//	위치 교정..
	
	if (_iX	<	0	||	_iX	>=	_lpData[0])
		return	FALSE;
	if (_iY	<	0	||	_iY	>=	_lpData[1])
		return	FALSE;

	for(j=0;j<_iY;j++)		//	_y축 상단 클리핑..
	{
		count	=	_lpData[bufferPoint++];

		for(i=0;i<count;i++)
		{
			bufferPoint++;
			size		=	_lpData[bufferPoint++];
			bufferPoint	+=	size;
		}
	}

	count	=	_lpData[bufferPoint++];
	XPoint	=	0;

	for(i=0;i<count;i++)
	{
		XPoint	+=	_lpData[bufferPoint++];
		size	=	_lpData[bufferPoint++];

		if (_iX	>=	XPoint	&&	_iX	<=	XPoint+size)
			return	TRUE;

		bufferPoint	+=	size;
		XPoint		+=	size;
	}

	return	FALSE;
}

void
cDRAW::initMagFilter()
{
	int	i , j;
	DWORD	dwAddr;

	_pdwMag = new DWORD [dMAG_FILTER_WIDTH * dMAG_FILTER_HIGHT]; 

	//memset(_pdwMag , 0 , sizeof())

	int RoofX = dMAG_FILTER_WIDTH / 2;
	int RoofY = dMAG_FILTER_HIGHT / 2;

 	HANDLE		hFile;
 	DWORD		dwRead;
	
 	hFile = CreateFile("JBC Test.txt", GENERIC_WRITE, 0, NULL,
 		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	for(i = 0 ; i < dMAG_FILTER_HIGHT ;i++)	//세로
	{
		for(j = 0 ; j < dMAG_FILTER_WIDTH ; j++)	//가로
		{
			dwAddr = i * dMAG_FILTER_WIDTH + j;			//원점의 주소

			for (int k = 0 ; k < 2 ; k++) //작은 세로
			{
				for (int l = 0 ; l < 2 ; l++) //작은 가로
				{
					if((i * 2 + k) * 2 * dMAG_FILTER_WIDTH + (j * 2) + l < dMAG_FILTER_WIDTH * dMAG_FILTER_HIGHT)
						_pdwMag[(i * 2 + k)* 2 * dMAG_FILTER_WIDTH + (j * 2) + l] = dwAddr; //색이 찍힐 곳.
					else 
						goto LABEL_END_ROOF;
				}
			}
		}
	}

LABEL_END_ROOF:

	char strTemp[1024];
	ZeroMemory(strTemp , 1024);

	for (i = 0 ; i < dMAG_FILTER_WIDTH * dMAG_FILTER_HIGHT ; i++)
	{
		if(i % 200 == 0)
			WriteFile(hFile, "\n" , sizeof(DWORD),&dwRead, NULL);

		sprintf(strTemp, "%d ", _pdwMag[i]);
		WriteFile(hFile, strTemp , strlen(strTemp),&dwRead, NULL);
	}

	CloseHandle(hFile);

}
