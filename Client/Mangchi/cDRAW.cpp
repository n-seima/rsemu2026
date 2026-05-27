#include "cDRAW.H"

WORD		_WHITE		,_BLACK;
WORD		_LTGRAY		,_GRAY		,_DEEPGRAY;
WORD		_LTCYAN		,_CYAN		,_DEEPCYAN;
WORD		_LTBLUE		,_BLUE		,_DEEPBLUE;
WORD		_LTRED		,_RED		,_DEEPRED;
WORD		_LTGREEN	,_GREEN		,_DEEPGREEN;
WORD		_LTYELLOW	,_YELLOW	,_DEEPYELLOW;
WORD		_LTPURPLE	,_PURPLE	,_DEEPPURPLE;
WORD		_SKYBLUE;


WORD		_gCOL[7][16];

DWORD	__first,__second;

inline DWORD AB4(DWORD color,int alpha)
{	__first		=	color&d4AM_MASK_1;
	__first		>>=	5;
	__first		*=	alpha;
	__first		&=	d4AM_MASK_1;

	__second	=	color&d4AM_MASK_2;
	__second	*=	alpha;
	__second	>>=	5;
	__second	&=	d4AM_MASK_2;

	return	__first + __second;
}

//	스프라이트 확대 축소 출력을 위한 테이블
zoomTable zoomXTable[3096];
zoomTable zoomYTable[3096];

/*********************************************************************************************
//	스프라이트 확대 축소 출력을 위한 테이블	생성 -	배율과 사이즈에 따른 테이블값이 생성된다.

	x,y		  위치
	xs,ys     스프라이트 실제 사이즈..
	xLen,yLen 확대/축소된 스프라이트 사이즈..

*********************************************************************************************/

WORD	redEFFECT[7][32][32];
WORD	blueEFFECT[7][32][32];
WORD	greenEFFECT[7][64][64];

inline	DWORD	
MIX4BYTE(DWORD spriteColor,DWORD screenColor,int alpha,int dest_alpha)
{	DWORD	mixFirst,mixSecond,mixColor;
	
	mixFirst	=	screenColor&d4AM_MASK_1;
	mixFirst	>>=	5;
	mixFirst	*=	dest_alpha;
	mixFirst	&=	d4AM_MASK_1;

	mixSecond	=	screenColor&d4AM_MASK_2;
	mixSecond	*=	dest_alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_2;

	mixColor	=	mixFirst + mixSecond;

	mixFirst	=	spriteColor&d4AM_MASK_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_1;

	mixSecond	=	spriteColor&d4AM_MASK_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_2;

	return	mixColor+	mixFirst + mixSecond;
}

inline	DWORD	
fMIX4BYTE(DWORD spriteColor,DWORD screenColor,int alpha,int dest_alpha)
{	DWORD	mixFirst,mixSecond,mixColor,backWord;

	mixFirst	=	screenColor&d4AM_MASK_1;
	mixFirst	>>=	5;
	mixFirst	*=	dest_alpha;
	mixFirst	&=	d4AM_MASK_1;

	mixSecond	=	screenColor&d4AM_MASK_2;
	mixSecond	*=	dest_alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_2;

	mixColor	=	mixFirst + mixSecond;

	backWord	=	((spriteColor<<16)+(spriteColor>>16));

	mixFirst	=	backWord&d4AM_MASK_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_1;

	mixSecond	=	backWord&d4AM_MASK_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_2;

	return	mixColor+	mixFirst + mixSecond;
}

inline	WORD	
MIX2BYTE(WORD spriteColor,WORD screenColor,int alpha,int dest_alpha)
{	DWORD	mixFirst,mixSecond,mixColor;

	mixFirst	=	screenColor&d4AM_MASK_1;
	mixFirst	>>=	5;
	mixFirst	*=	dest_alpha;
	mixFirst	&=	d4AM_MASK_1;

	mixSecond	=	screenColor&d4AM_MASK_2;
	mixSecond	*=	dest_alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_2;

	mixColor	=	mixFirst + mixSecond;

	mixFirst	=	spriteColor&d4AM_MASK_1;
	mixFirst	>>=	5;
	mixFirst	*=	alpha;
	mixFirst	&=	d4AM_MASK_1;

	mixSecond	=	spriteColor&d4AM_MASK_2;
	mixSecond	*=	alpha;
	mixSecond	>>=	5;
	mixSecond	&=	d4AM_MASK_2;

	return	(WORD)(mixColor	+ mixFirst + mixSecond);
}

inline WORD
FXMIX(WORD spriteColor,WORD screenColor,int effect)
{	WORD	fxRed,fxGreen,fxBlue;

	fxRed	=redEFFECT	[effect][ spriteColor>>dGET_RED_SHIFT		][ screenColor>>dGET_RED_SHIFT		];
	fxGreen	=greenEFFECT[effect][(spriteColor>>5)&dGET_GREEN_MASK	][(screenColor>>5)&dGET_GREEN_MASK	];
	fxBlue	=blueEFFECT	[effect][ spriteColor&31					][ screenColor&31					];

	return fxRed+fxGreen+fxBlue;
}

inline void
MakeSpriteRTable(int x,int y,int xs,int ys,int xLen,int yLen)
{	int xp,yp,length,i,num;

	xp		=0	,	length	=0;
	for (i=0;i<xs;i++)
	{	length				+=	xLen;		//	length를 체크해서 xs보다 크면은.. 점 사이즈를 더해준다.
		num					=	length/xs;

		zoomXTable[i].pos	=	xp;			//	위치.. 이점의 위치.. xp
		length				-=	num*xs;		//	길이에서 빼준다..
		zoomXTable[i].size	=	num;		//	이점의 실제 사이즈..
		xp					+=	num;		//	다음점의 위치..

		if (x+zoomXTable[i].pos + zoomXTable[i].size >= cDRAW::WIDTH	)
			zoomXTable[i].size = cDRAW::WIDTH - x-zoomXTable[i].pos;

		if (x+zoomXTable[i].pos < 0	)
		{	if (x+zoomXTable[i].pos + zoomXTable[i].size <= 0	)	zoomXTable[i].size = 0;
			else
			{	zoomXTable[i].size += (x+zoomXTable[i].pos);
				zoomXTable[i].pos = -x;
			}
		}
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

//		while(length>=xs)		length-=xs	,zoomXTable[i].size++,	xp++;

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

/*		함수 포인터			*/
void	(*PutSprite16[22])	(int x,int y,WORD *sprite,int alpha);
void	(*PutSpriteR16[22])	(int x,int y,WORD *sprite,int xRate,int yRate,int alpha);
void	(*PutSprite8[22])	(int x,int y,BYTE *sprite,WORD *plt,int alpha);
void	(*PutSpriteR8[22])	(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha);

int		cDRAW::WIDTH		=	0;
int		cDRAW::HEIGHT		=	0;
WORD	*cDRAW::p2SCREEN	=	NULL;
UINT	*cDRAW::p4SCREEN	=	NULL;
int		cDRAW::halfScreen	=	0;
int		cDRAW::halfWidth	=	0;
int		cDRAW::byteWidth	=	0;
int		cDRAW::dotPerScreen	=	0;
int		cDRAW::bytePerScreen=	0;

BOOL
cDRAW::Init()
{
	//	16비트 스프라이트
	PutSprite16[dPUT_NORMAL				]=	c16BITSPRITE::PutSprite;
	PutSprite16[dPUT_HALF_BLENDING		]=	c16BITSPRITE::PutSpriteHB;
	PutSprite16[dPUT_ALPHA_BLENDING		]=	c16BITSPRITE::PutSpriteAB;
	PutSprite16[dPUT_COLOR				]=	c16BITSPRITE::PutSpriteColor;
	PutSprite16[dPUT_SOFT_DODGE			]=	c16BITSPRITE::PutSpriteFX;
	PutSprite16[dPUT_DIFFERENT			]=	c16BITSPRITE::PutSpriteFX;
	PutSprite16[dPUT_DODGE_BURN			]=	c16BITSPRITE::PutSpriteFX;
	PutSprite16[dPUT_SOFT				]=	c16BITSPRITE::PutSpriteFX;
	PutSprite16[dPUT_DODGE				]=	c16BITSPRITE::PutSpriteFX;
	PutSprite16[dPUT_LIGHT				]=	c16BITSPRITE::PutSpriteFX;
	PutSprite16[dPUT_EXTREAM_LIGHT		]=	c16BITSPRITE::PutSpriteFX;

	PutSprite16[dPUT_NORMAL			+11	]=	c16BITSPRITE::fPutSprite;
	PutSprite16[dPUT_HALF_BLENDING	+11	]=	c16BITSPRITE::fPutSpriteHB;
	PutSprite16[dPUT_ALPHA_BLENDING	+11	]=	c16BITSPRITE::fPutSpriteAB;
	PutSprite16[dPUT_COLOR			+11	]=	c16BITSPRITE::fPutSpriteColor;
	PutSprite16[dPUT_SOFT_DODGE		+11	]=	c16BITSPRITE::fPutSpriteFX;
	PutSprite16[dPUT_DIFFERENT		+11	]=	c16BITSPRITE::fPutSpriteFX;
	PutSprite16[dPUT_DODGE_BURN		+11	]=	c16BITSPRITE::fPutSpriteFX;
	PutSprite16[dPUT_SOFT			+11	]=	c16BITSPRITE::fPutSpriteFX;
	PutSprite16[dPUT_DODGE			+11	]=	c16BITSPRITE::fPutSpriteFX;
	PutSprite16[dPUT_LIGHT			+11	]=	c16BITSPRITE::fPutSpriteFX;
	PutSprite16[dPUT_EXTREAM_LIGHT	+11	]=	c16BITSPRITE::fPutSpriteFX;

	PutSpriteR16[dPUT_NORMAL			]=	c16BITSPRITE::PutSpriteR;
	PutSpriteR16[dPUT_HALF_BLENDING		]=	c16BITSPRITE::PutSpriteRHB;
	PutSpriteR16[dPUT_ALPHA_BLENDING	]=	c16BITSPRITE::PutSpriteRAB;
	PutSpriteR16[dPUT_COLOR				]=	c16BITSPRITE::PutSpriteRColor;
	PutSpriteR16[dPUT_SOFT_DODGE		]=	c16BITSPRITE::PutSpriteRFX;
	PutSpriteR16[dPUT_DIFFERENT			]=	c16BITSPRITE::PutSpriteRFX;
	PutSpriteR16[dPUT_DODGE_BURN		]=	c16BITSPRITE::PutSpriteRFX;
	PutSpriteR16[dPUT_SOFT				]=	c16BITSPRITE::PutSpriteRFX;
	PutSpriteR16[dPUT_DODGE				]=	c16BITSPRITE::PutSpriteRFX;
	PutSpriteR16[dPUT_LIGHT				]=	c16BITSPRITE::PutSpriteRFX;
	PutSpriteR16[dPUT_EXTREAM_LIGHT		]=	c16BITSPRITE::PutSpriteRFX;

	PutSpriteR16[dPUT_NORMAL		+11	]=	c16BITSPRITE::fPutSpriteR;
	PutSpriteR16[dPUT_HALF_BLENDING	+11	]=	c16BITSPRITE::fPutSpriteRHB;
	PutSpriteR16[dPUT_ALPHA_BLENDING+11	]=	c16BITSPRITE::fPutSpriteRAB;
	PutSpriteR16[dPUT_COLOR			+11	]=	c16BITSPRITE::fPutSpriteRColor;
	PutSpriteR16[dPUT_SOFT_DODGE	+11	]=	c16BITSPRITE::fPutSpriteRFX;
	PutSpriteR16[dPUT_DIFFERENT		+11	]=	c16BITSPRITE::fPutSpriteRFX;
	PutSpriteR16[dPUT_DODGE_BURN	+11	]=	c16BITSPRITE::fPutSpriteRFX;
	PutSpriteR16[dPUT_SOFT			+11	]=	c16BITSPRITE::fPutSpriteRFX;
	PutSpriteR16[dPUT_DODGE			+11	]=	c16BITSPRITE::fPutSpriteRFX;
	PutSpriteR16[dPUT_LIGHT			+11	]=	c16BITSPRITE::fPutSpriteRFX;
	PutSpriteR16[dPUT_EXTREAM_LIGHT	+11	]=	c16BITSPRITE::fPutSpriteRFX;


	//	8트 스프라이트
	PutSprite8[dPUT_NORMAL				]=	c8BITSPRITE::PutSprite;
	PutSprite8[dPUT_HALF_BLENDING		]=	c8BITSPRITE::PutSpriteHB;
	PutSprite8[dPUT_ALPHA_BLENDING		]=	c8BITSPRITE::PutSpriteAB;
	PutSprite8[dPUT_COLOR				]=	c8BITSPRITE::PutSpriteColor;
	PutSprite8[dPUT_SOFT_DODGE			]=	c8BITSPRITE::PutSpriteFX;
	PutSprite8[dPUT_DIFFERENT			]=	c8BITSPRITE::PutSpriteFX;
	PutSprite8[dPUT_DODGE_BURN			]=	c8BITSPRITE::PutSpriteFX;
	PutSprite8[dPUT_SOFT				]=	c8BITSPRITE::PutSpriteFX;
	PutSprite8[dPUT_DODGE				]=	c8BITSPRITE::PutSpriteFX;
	PutSprite8[dPUT_LIGHT				]=	c8BITSPRITE::PutSpriteFX;
	PutSprite8[dPUT_EXTREAM_LIGHT		]=	c8BITSPRITE::PutSpriteFX;

	PutSprite8[dPUT_NORMAL			+11	]=	c8BITSPRITE::fPutSprite;
	PutSprite8[dPUT_HALF_BLENDING	+11	]=	c8BITSPRITE::fPutSpriteHB;
	PutSprite8[dPUT_ALPHA_BLENDING	+11	]=	c8BITSPRITE::fPutSpriteAB;
	PutSprite8[dPUT_COLOR			+11	]=	c8BITSPRITE::fPutSpriteColor;
	PutSprite8[dPUT_SOFT_DODGE		+11	]=	c8BITSPRITE::fPutSpriteFX;
	PutSprite8[dPUT_DIFFERENT		+11	]=	c8BITSPRITE::fPutSpriteFX;
	PutSprite8[dPUT_DODGE_BURN		+11	]=	c8BITSPRITE::fPutSpriteFX;
	PutSprite8[dPUT_SOFT			+11	]=	c8BITSPRITE::fPutSpriteFX;
	PutSprite8[dPUT_DODGE			+11	]=	c8BITSPRITE::fPutSpriteFX;
	PutSprite8[dPUT_LIGHT			+11	]=	c8BITSPRITE::fPutSpriteFX;
	PutSprite8[dPUT_EXTREAM_LIGHT	+11	]=	c8BITSPRITE::fPutSpriteFX;

	PutSpriteR8[dPUT_NORMAL				]=	c8BITSPRITE::PutSpriteR;
	PutSpriteR8[dPUT_HALF_BLENDING		]=	c8BITSPRITE::PutSpriteRHB;
	PutSpriteR8[dPUT_ALPHA_BLENDING		]=	c8BITSPRITE::PutSpriteRAB;
	PutSpriteR8[dPUT_COLOR				]=	c8BITSPRITE::PutSpriteRColor;
	PutSpriteR8[dPUT_SOFT_DODGE			]=	c8BITSPRITE::PutSpriteRFX;
	PutSpriteR8[dPUT_DIFFERENT			]=	c8BITSPRITE::PutSpriteRFX;
	PutSpriteR8[dPUT_DODGE_BURN			]=	c8BITSPRITE::PutSpriteRFX;
	PutSpriteR8[dPUT_SOFT				]=	c8BITSPRITE::PutSpriteRFX;
	PutSpriteR8[dPUT_DODGE				]=	c8BITSPRITE::PutSpriteRFX;
	PutSpriteR8[dPUT_LIGHT				]=	c8BITSPRITE::PutSpriteRFX;
	PutSpriteR8[dPUT_EXTREAM_LIGHT		]=	c8BITSPRITE::PutSpriteRFX;

	PutSpriteR8[dPUT_NORMAL			+11	]=	c8BITSPRITE::fPutSpriteR;
	PutSpriteR8[dPUT_HALF_BLENDING	+11	]=	c8BITSPRITE::fPutSpriteRHB;
	PutSpriteR8[dPUT_ALPHA_BLENDING	+11	]=	c8BITSPRITE::fPutSpriteRAB;
	PutSpriteR8[dPUT_COLOR			+11	]=	c8BITSPRITE::fPutSpriteRColor;
	PutSpriteR8[dPUT_SOFT_DODGE		+11	]=	c8BITSPRITE::fPutSpriteRFX;
	PutSpriteR8[dPUT_DIFFERENT		+11	]=	c8BITSPRITE::fPutSpriteRFX;
	PutSpriteR8[dPUT_DODGE_BURN		+11	]=	c8BITSPRITE::fPutSpriteRFX;
	PutSpriteR8[dPUT_SOFT			+11	]=	c8BITSPRITE::fPutSpriteRFX;
	PutSpriteR8[dPUT_DODGE			+11	]=	c8BITSPRITE::fPutSpriteRFX;
	PutSpriteR8[dPUT_LIGHT			+11	]=	c8BITSPRITE::fPutSpriteRFX;
	PutSpriteR8[dPUT_EXTREAM_LIGHT	+11	]=	c8BITSPRITE::fPutSpriteRFX;


	BYTE yellow[8][3] = {{10,21, 0},{15,31, 2},{17,35, 4},{20,48, 6},{22,45, 9},{25,50,11},{27,55,14},{29,59,16}};
	BYTE cyan  [8][3] = {{ 0,27,13},{ 4,36,18},{ 6,40,20},{ 9,45,22},{11,50,25},{14,55,27},{16,60,30},{18,63,31}};
	BYTE red   [8][3] = {{31, 8, 0},{31,15, 0},{31,23, 0},{31,31, 0},{31,39, 0},{31,47, 0},{31,55, 0},{31,63, 0}};
	BYTE green [8][3] = {{ 0,35, 8},{ 1,41,12},{ 3,46,15},{ 5,51,17},{ 8,56,20},{10,60,22},{14,63,24},{20,63,27}};
	BYTE gray  [8][3] = {{ 9,18, 9},{14,28,14},{16,32,16},{18,37,18},{21,42,21},{23,46,23},{25,51,25},{27,55,27}};
	BYTE blue  [8][3] = {{ 6,13,31},{11,23,31},{14,28,31},{17,34,31},{19,39,31},{22,44,31},{25,50,30},{29,54,31}};
	BYTE purple[8][3] = {{17,13,17},{22,17,22},{24,21,24},{26,26,26},{29,31,29},{31,35,31},{31,43,31},{31,50,31}};

	// 그라데이션 칼라 셋팅!!! 그래픽 카드의 픽셸 포맷에 관계 없음.. 발진 준비!!! ^^
	for (int i=0;i<8;i++) 
	{	_gCOL[0][i	 ]=RGBmix(yellow[i][0],yellow[i][1],yellow[i][2]);
		_gCOL[0][15-i]=RGBmix(yellow[i][0],yellow[i][1],yellow[i][2]);
		_gCOL[1][i	 ]=RGBmix(cyan  [i][0],cyan  [i][1],cyan  [i][2]);
		_gCOL[1][15-i]=RGBmix(cyan  [i][0],cyan  [i][1],cyan  [i][2]);
		_gCOL[2][i	 ]=RGBmix(red   [i][0],red   [i][1],red   [i][2]);
		_gCOL[2][15-i]=RGBmix(red   [i][0],red   [i][1],red   [i][2]);
		_gCOL[3][i	 ]=RGBmix(green [i][0],green [i][1],green [i][2]);
		_gCOL[3][15-i]=RGBmix(green [i][0],green [i][1],green [i][2]);
		_gCOL[4][i	 ]=RGBmix(gray  [i][0],gray  [i][1],gray  [i][2]);
		_gCOL[4][15-i]=RGBmix(gray  [i][0],gray  [i][1],gray  [i][2]);
		_gCOL[5][i	 ]=RGBmix(blue  [i][0],blue  [i][1],blue  [i][2]);
		_gCOL[5][15-i]=RGBmix(blue  [i][0],blue  [i][1],blue  [i][2]);
		_gCOL[6][i	 ]=RGBmix(purple[i][0],purple[i][1],purple[i][2]);
		_gCOL[6][15-i]=RGBmix(purple[i][0],purple[i][1],purple[i][2]);
	}

	_WHITE		=RGBmix(31,63,31);
	_BLACK		=RGBmix( 0, 0, 0);

	_LTGRAY		=RGBmix(24,48,24);
	_GRAY 		=RGBmix(16,32,16);
	_DEEPGRAY	=RGBmix( 8,16, 8);

	_LTCYAN 	=RGBmix( 0,63,31);
	_CYAN		=RGBmix( 0,32,16);
	_DEEPCYAN	=RGBmix( 0,16, 8);

	_LTBLUE 	=RGBmix( 0, 0,31);
	_BLUE		=RGBmix( 0, 0,16);
	_DEEPBLUE	=RGBmix( 0, 0, 8);

	_LTRED  	=RGBmix(31, 0, 0);
	_RED	 	=RGBmix(16, 0, 0);
	_DEEPRED 	=RGBmix( 8, 0, 0);

	_LTGREEN	=RGBmix( 0,63, 0);
	_GREEN		=RGBmix( 0,32, 0);
	_DEEPGREEN	=RGBmix( 0,16, 0);

	_LTYELLOW	=RGBmix(31,63, 0);
	_YELLOW		=RGBmix(16,32, 0);
	_DEEPYELLOW	=RGBmix( 8,16, 0);

	_LTPURPLE	=RGBmix(31, 0,31);
	_PURPLE		=RGBmix(16, 0,16);
	_DEEPPURPLE	=RGBmix( 8, 0, 8);
	_SKYBLUE	=RGB24To16(165,203,247);

	int	spr,bg;

	for (spr=0;spr<32;spr++)
		for (bg=0;bg<32;bg++)
		{	int soft	=	(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));

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
		{	int soft	=	(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));

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
		{	int soft	=	(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));

			blueEFFECT[dPUT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr));
			blueEFFECT[dPUT_DODGE_BURN]		[spr][bg]=(min(((bg*(32-spr)))/32 , 31));
			blueEFFECT[dPUT_SOFT]			[spr][bg]=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));
			blueEFFECT[dPUT_DODGE]			[spr][bg]=(min((bg*32)/(32-spr),31));
			blueEFFECT[dPUT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,31));
			blueEFFECT[dPUT_LIGHT]			[spr][bg]=(max(bg,spr));
			blueEFFECT[dPUT_SOFT_DODGE]		[spr][bg]=(min((soft*32)/(32-spr),31));
		}

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
cDRAW::Line(WORD color,int x1,int y1,int x2,int y2)
{	int	slant,address,len=0,x=0;

	if (x2==x1)	{YLine(color,y1,y2,x1);return;}
	if (y2==y1)	{XLine(color,x1,x2,y1);return;}

	slant	=	(x2-x1)*(y2-y1);

	int width	=	max(x2,x1)-min(x2,x1)+1;
	int height	=	max(y2,y1)-min(y2,y1)+1;

	if (x1>x2)
	{	swap(x1,x2);
		swap(y1,y2);
	}

	address	=	x1+y1*WIDTH;

	if (slant > 0)	//	＼ 요런 모양
	{
		if (width< height)	//	세로가 가로의 변화보다 크다.
			for (int i=0;i<height;i++)
			{	if (x1 >=	WIDTH	) return;
				if (y1 >=	HEIGHT	) return;

				if (address	>=	0	&&	address	<	WIDTH*HEIGHT)
					if (x1 >= 0 && y1 >= 0)	p2SCREEN[address]	=	color;

				len					+=	width;
				address				+=	WIDTH;
				y1++;
				if (len >= height)	len-=height,address++,x1++;
			}
		else				//	가로가 세로의 변화보다 크다.
			for (int i=0;i<width;i++)
			{	if (x1 >=	WIDTH	) return;
				if (y1 >=	HEIGHT	) return;

				if (address	>=	0	&&	address	<	WIDTH*HEIGHT)
					if (x1 >=0 && y1 >= 0)	p2SCREEN[address]	=	color;

				len		+=	height;
				address++;
				x1++;
				if (len >= width ) len-=width,address+=WIDTH,y1++;
			}
	}
	else		//	／ 요런 모양
	{	if (width< height)	//	세로가 가로의 변화보다 크다.
			for (int i=0;i<height;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 <  0		) return;

				if (address	>=	0	&&	address	<	WIDTH*HEIGHT)
					if (x1 >= 0 && y1 < HEIGHT) p2SCREEN[address]	=	color;

				len		+=	width;
				address	-=	WIDTH;
				y1--;
				if (len >= height)	len-=height,address++,x1++;
			}
		else//	가로가 세로의 변화보다 크다.
			for (int i=0;i<width;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 <  0		) return;

				if (address	>=	0	&&	address	<	WIDTH*HEIGHT)
					if (x1 >= 0 && y1 < HEIGHT) p2SCREEN[address]	=	color;

				len		+=	height;
				address++;
				x1++;
				if (len >= width ) len-=width,address-=WIDTH,y1--;
			}
	}
}

void
cDRAW::XLine(WORD color,int x1,int x2,int y,int thick)
{	int	xs,i;

	x1=max(x1,0);
	x2=min(x2,WIDTH-1);

	if (y+thick > HEIGHT) thick = HEIGHT-y;
	if (y < 0 )
	{	if (y+thick < 0) return;
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

	y1=max(y1,0);
	y2=min(y2,HEIGHT-1);

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
{	FillAB(color,x1,y,x2,y+thick-1,alpha);
}

void
cDRAW::YLineAB(WORD color,int alpha,int y1,int y2,int x,int thick)
{	FillAB(color,x,y1,x+thick-1,y2,alpha);
}

void
cDRAW::XLineHB(WORD color,int x1,int x2,int y,int thick)
{	FillHB(color,x1,y,x2,y+thick-1);
}

void
cDRAW::YLineHB(WORD color,int y1,int y2,int x,int thick)
{	FillHB(color,x,y1,x+thick-1,y2);
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
{	if (x1 > x2) swap(x1,x2);
	if (y1 > y2) swap(y1,y2);
	XLine(color,x1,x2,y1		,thick);
	XLine(color,x1,x2,y2-thick+1,thick);
	YLine(color,y1,y2,x1		,thick);
	YLine(color,y1,y2,x2-thick+1,thick);
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
	hc4	=(fc4&d4HM_MASK)>>1;
	for (int i=0;i<halfScreen;i++)	p4SCREEN[i]=((p4SCREEN[i]&d4HM_MASK)>>1)+hc4;
}

void
cDRAW::FillAB(WORD color,int alpha)
{	int		dest_alpha=32-alpha;
	UINT	ac4,sc4,fc4,first,second;

	fc4	=	(UINT) (color+(color<<16));			//	4바이트로 만듦..

	first	=	fc4&d4AM_MASK_1;
	first	>>=	5;
	first	*=	alpha;
	first	&=	d4AM_MASK_1;

	second	=	fc4&d4AM_MASK_2;
	second	*=	alpha;
	second	>>=	5;
	second	&=	d4AM_MASK_2;

	ac4		=	first + second;

	for (int i=0;i<halfScreen;i++)
	{	sc4			=	p4SCREEN[i];

		first		=	sc4&d4AM_MASK_1;
		first		>>=	5;
		first		*=	dest_alpha;
		first		&=	d4AM_MASK_1;

		second		=	sc4&d4AM_MASK_2;
		second		*=	dest_alpha;
		second		>>=	5;
		second		&=	d4AM_MASK_2;

		p4SCREEN[i]	=	ac4 + (first + second);
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
{	int	xs,ys,i,j;

	x1=max(x1,0);
	y1=max(y1,0);
	x2=min(x2,WIDTH-1);
	y2=min(y2,HEIGHT-1);

	xs=x2-x1+1;
	ys=y2-y1+1;

	if (xs <= 0  || ys <= 0) return;

	UINT	fc4,*p4_scr,addr=0;

	fc4=(UINT) (color+(color<<16));			//	4바이트로 만듦..
	p4_scr=(UINT *)&p2SCREEN[x1+y1*WIDTH];	//	4바이트 주소..

	if (xs%2)							//	4바이트로 나누어지는 경우..
	{	int mica=x2+y1*WIDTH;

		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
		{	for (j=0;j<xs/2;j++) 	p4_scr[addr+j]=fc4;
			p2SCREEN[mica]=color;			//	안 나눠 지니까.. 쪼가리가 남는다..
		}
	}
	else
	{	for (i=0;i<ys;i++,addr+=halfWidth)
			for (j=0;j<xs/2;j++)	p4_scr[addr+j]=fc4;
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

	fc4=(UINT) (color+(color<<16));			//	4바이트로 만듦..
	hc4=(fc4&d4HM_MASK)>>1;
	hc2=(color&d2HM_MASK)>>1;
	p4_scr=(UINT *)&p2SCREEN[x1+y1*WIDTH];	//	4바이트 주소..

	if (xs%2)							//	4바이트로 나누어지는 경우..
	{	int	mica=x2+y1*WIDTH;

		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
		{	for (j=0;j<xs/2;j++) p4_scr[addr+j]=((p4_scr[addr+j]&d4HM_MASK)>>1)+hc4;
			p2SCREEN[mica]=((p2SCREEN[mica]&d2HM_MASK)>>1)+hc2;			//	안 나눠 지니까.. 쪼가리가 남는다..
		}
	}
	else	
	{	for (i=0;i<ys;i++,addr+=halfWidth)
			for (j=0;j<xs/2;j++)
				p4_scr[addr+j]=((p4_scr[addr+j]&d4HM_MASK)>>1)+hc4;
	}
}

void
cDRAW::FillAB(WORD color,int x1,int y1,int x2,int y2,int alpha)
{	int	xs,ys,i,j;

	x1=max(x1,0);
	y1=max(y1,0);
	x2=min(x2,WIDTH-1);
	y2=min(y2,HEIGHT-1);

	xs=x2-x1+1;
	ys=y2-y1+1;

	if (xs <= 0  || ys <= 0) return;

	int		dest_alpha=32-alpha;
	UINT	*p4_scr,addr=0,ac4,sc4,fc4,first,second;
	WORD	ac2,sc2;

	fc4=(UINT) (color+(color<<16));			//	4바이트로 만듦..

	first	=	fc4&d4AM_MASK_1;
	first	>>=	5;
	first	*=	alpha;
	first	&=	d4AM_MASK_1;

	second	=	fc4&d4AM_MASK_2;
	second	*=	alpha;
	second	>>=	5;
	second	&=	d4AM_MASK_2;

	ac4		=	first + second;

	first	=	color&d4AM_MASK_1;
	first	>>=	5;
	first	*=	alpha;
	first	&=	d4AM_MASK_1;

	second	=	color&d4AM_MASK_2;
	second	*=	alpha;
	second	>>=	5;
	second	&=	d4AM_MASK_2;

	ac2		=	(WORD)(first + second);

	p4_scr=(UINT *)&p2SCREEN[x1+y1*WIDTH];	//	4바이트 주소..

	if (xs%2==0)							//	4바이트로 나누어지는 경우..
	{	for (i=0;i<ys;i++,addr+=halfWidth)
			for (j=0;j<xs/2;j++)
			{	sc4				=	p4_scr[addr+j];

				first			=	sc4&d4AM_MASK_1;
				first			>>=	5;
				first			*=	dest_alpha;
				first			&=	d4AM_MASK_1;

				second			=	sc4&d4AM_MASK_2;
				second			*=	dest_alpha;
				second			>>=	5;
				second			&=	d4AM_MASK_2;

				p4_scr[addr+j]	=	ac4 + (first + second);
			}
	}
	else									//	안나눠 지는 경우.. ==;
	{	int mica=x2+y1*WIDTH;
		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
		{	for (j=0;j<xs/2;j++)
			{	sc4				=	p4_scr[addr+j];

				first			=	sc4&d4AM_MASK_1;
				first			>>=	5;
				first			*=	dest_alpha;
				first			&=	d4AM_MASK_1;

				second			=	sc4&d4AM_MASK_2;
				second			*=	dest_alpha;
				second			>>=	5;
				second			&=	d4AM_MASK_2;

				p4_scr[addr+j]	=	ac4 + (first + second);
			}
			sc2				=	p2SCREEN[mica];

			first			=	sc2&d4AM_MASK_1;
			first			>>=	5;
			first			*=	dest_alpha;
			first			&=	d4AM_MASK_1;

			second			=	sc2&d4AM_MASK_2;
			second			*=	dest_alpha;
			second			>>=	5;
			second			&=	d4AM_MASK_2;

			p2SCREEN[mica]	=	ac2 + (WORD)(first + second);
		}
	}
}

/****************************************************************************
void	GetImage(int x,int y,int xs,int ys,WORD *img)
WORD*	GetImage(int x,int y,int xs,int ys)
	임의의 지점의 이미지를 읽어 온다.
****************************************************************************/

void
cDRAW::GetImage(int x,int y,int xs,int ys,WORD *img)
{
#ifdef dDEBUG_MODE
	if (!img)
	{	cMSG::Error("Error!! in cDRAW::GetImage[code0001]","Valid Image Buffer");
		return;
	}
#endif

	int width=xs,offset=0,addr=0,i;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	img[0]	=	(WORD )xs;
	img[1]	=	(WORD )ys;

	addr	=	x+y*WIDTH;

	xs*=2;
	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width) memcpy(img+offset,p2SCREEN+addr,xs);
}

WORD *
cDRAW::GetImage(int x,int y,int xs,int ys)
{	int offset=2,addr=0,i,width=xs;

	WORD *img	=	new WORD [xs*ys+2];
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
cDRAW::PutImage(int x,int y,int xs,int ys,WORD *image)
{
#ifdef dDEBUG_MODE
	if (!image)
	{	MSGOUT("Error!! in cDRAW::PutImage - Valid Image data");
		return;
	}
#endif

	int width,offset=0,addr=0,i;
	width	=	xs;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	addr	=	x+y*WIDTH;

	xs*=2;

	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width) memcpy(p2SCREEN+addr,image+offset,xs);
}

void
cDRAW::PutImageScale(int x,int y,int xs,int ys,WORD *image,int _iXScale,int _iYScale)
{
	int		addr,bufferPoint;

	int	xLen=xs*_iXScale/100+1;
	int	yLen=ys*_iYScale/100+1;	//	확대/축소된 이미즈 사이즈

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

					p2SCREEN[pos]	=	image[bufferPoint+x];
				}

			addr		+=	WIDTH;
		}

		bufferPoint		+=	xs;
	}
}

void
cDRAW::PutImage(int x,int y,int xs,int ys,BYTE *image,WORD *plt)
{
#ifdef dDEBUG_MODE
	if (!image)
	{	MSGOUT("Error!! in cDRAW::PutImage - Valid pointer 'image'");
		return;
	}
	if (!plt)
	{	MSGOUT("Error!! in cDRAW::PutImage - Valid pointer 'plt'");
		return;
	}
#endif

	int		width,offset=0,addr=0,i,j;

	width	=	xs;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	addr	=	x+y*WIDTH;

	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width)
		for (j=0;j<xs;j++)
			p2SCREEN[addr+j]	=	plt[image[offset+j]];
}

void
cDRAW::PutImage(int x,int y,WORD *image)
{
//	for (int k=0;k<WIDTH*HEIGHT;k++)	p2SCREEN[k]=image[800*600-k+2];return;
#ifdef dDEBUG_MODE
	if (!image)
	{	MSGOUT("Error!! in cDRAW::PutImage - Valid Image data");
		return;
	}
#endif

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
cDRAW::PutImage(int x,int y,BYTE *image,WORD *plt)
{
#ifdef dDEBUG_MODE
	if (!image)
	{	MSGOUT("Error!! in cDRAW::PutImage - Valid pointer 'image'");
		return;
	}
	if (!plt)
	{	MSGOUT("Error!! in cDRAW::PutImage - Valid pointer 'plt'");
		return;
	}
#endif

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
cDRAW::PutImageHB(int x,int y,WORD *image)
{
#ifdef dDEBUG_MODE
	if (!image)
	{	cMSG::Error("Error!! in cDRAW::PutImageHM[code0033]  Valid Image data");
		return;
	}
#endif

	int width,offset=0,addr=0,i,j;
	int		xs	=	image[0],ys	=	image[1];
	WORD	*img=	&image[2];

	width	=	xs;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	if (xs%2)
	{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
		int mica=x+xs-1+y*WIDTH;
		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH,offset+=width)
		{
			UINT *p4_img=(UINT *)&img[offset];
			for (j=0;j<xs/2;j++) 
				p4_scr[addr+j]=(((p4_scr[addr+j]&d4HM_MASK)>>1)+((p4_img[j]&d4HM_MASK)>>1));
			p2SCREEN[mica]=((p2SCREEN[mica]&d2HM_MASK)+(img[offset+xs]&d2HM_MASK))>>1;
		}
	}
	else
	{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
		for (i=0;i<ys;i++,addr+=halfWidth,offset+=width)
		{	UINT *p4_img=(UINT *)&img[offset];
			for (j=0;j<xs/2;j++)
				p4_scr[addr+j]=(((p4_scr[addr+j]&d4HM_MASK)>>1)+((p4_img[j]&d4HM_MASK)>>1));
		}
	}
}

void
cDRAW::PutImageAB(int x,int y,WORD *image,int alpha)
{	if	(alpha>=32)	{PutImage(x,y,image);return;}

#ifdef dDEBUG_MODE
	if (!image)
	{	cMSG::Error("Error!! in cDRAW::PutImageAM[code0035]  Valid Image data");
		return;
	}
#endif

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

	if (xs%2)
	{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH],first,second;
		int mica=x+xs-1+y*WIDTH;
		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH,offset+=width)
		{	UINT *p4_img=(UINT *)&img[offset];
			for (j=0;j<xs/2;j++)
			{	first	=	p4_img[j]&d4AM_MASK_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_MASK_1;

				second	=	p4_img[j]&d4AM_MASK_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_MASK_2;

				p4_scr[addr+j]=first+second;
			}

			first	=	img[offset+xs]&d4AM_MASK_1;
			first	>>=	5;
			first	*=	alpha;
			first	&=	d4AM_MASK_1;

			second	=	img[offset+xs]&d4AM_MASK_2;
			second	*=	alpha;
			second	>>=	5;
			second	&=	d4AM_MASK_2;

			p2SCREEN[mica]=(WORD)first+second;
		}
	}
	else
	{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH],first,second;
		for (i=0;i<ys;i++,addr+=halfWidth,offset+=width)
		{	UINT *p4_img=(UINT *)&img[offset];
			for (j=0;j<xs/2;j++)
			{	first	=	p4_img[j]&d4AM_MASK_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_MASK_1;

				second	=	p4_img[j]&d4AM_MASK_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_MASK_2;

				p4_scr[addr+j]=first+second;
			}
		}
	}
}

/*****************************************************************************************

*****************************************************************************************/

void
cDRAW::PutShadow(int x,int y,BYTE *shadow)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=8;
	int   size;

	xs=*(WORD  *)(shadow  );
	ys=*(WORD  *)(shadow+2);
	x-=*(short *)(shadow+4);	//	위치 교정..
	y-=*(short *)(shadow+6);	//	위치 교정..

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
	
	if (x<0)
	for(;y1<y2;y1++)
	{	count	=	shadow[bufferPoint++];
		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	shadow[bufferPoint++];
			size	=	shadow[bufferPoint++];

			beginX	=	0;
			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint+beginX];
			width		=size-beginX;

			for (j=0;j<width/2;j++)		p4_scr[j]=(p4_scr[j]&d4HM_MASK)>>1;
			if (width%2 && width > 0)
			{	int	pos	=	addr+xPoint+size-1;
				p2SCREEN[pos]	=	(p2SCREEN[pos]&d2HM_MASK)>>1;
			}

			xPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count	=	shadow[bufferPoint++];
		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	shadow[bufferPoint++];
			size	=	shadow[bufferPoint++];

			endX	=	(int) size;
			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			UINT *p4_scr=	(UINT *)&p2SCREEN[addr+xPoint];
			width		=	endX;

			for (j=0;j<width/2;j++)	p4_scr[j]	=	(p4_scr[j]&d4HM_MASK)>>1;
			if ((width&1) && endX > 0)
			{	int	pos	=	addr+xPoint+endX-1;
				p2SCREEN[pos]	=	(p2SCREEN[pos]&d2HM_MASK)>>1;
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

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];

			for (j=0;j<size/2;j++)	p4_scr[j]=(p4_scr[j]&d4HM_MASK)>>1;
			if ( size&1			 )
			{	int	pos	=	addr+xPoint+size-1;
				p2SCREEN[pos]	=	(p2SCREEN[pos]&d2HM_MASK)>>1;
			}

			xPoint	+=	size;
		}
		addr+=WIDTH;
	}

}

void
cDRAW::PutShadowR(int x,int y,BYTE *sprite,int xRate,int yRate)
{	if (xRate==100 && yRate==100) {	PutShadow(x,y,sprite);	return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code cDRAW_0009");
		return;
	}
#endif

	xp	=	*(short *)(sprite+4);
	yp	=	*(short *)(sprite+6);

	xs	=*(WORD *)(sprite)			;
	ys	=*(WORD *)(sprite+2)		;	//	스프라이트 사이즈
	xLen=xs*xRate/100				;
	yLen=ys*yRate/100				;	//	확대/축소된 이미즈 사이즈
	x	-=xp*xRate/100;
	y	-=yp*yRate/100;	//	위치 교정..

	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
					{	int	pos	=	addr+zoomXTable[j+xp].pos+k;	
						p2SCREEN[pos]	=	(p2SCREEN[pos]&d2HM_MASK)>>1;
					}

				xp	+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	2;
	}
}

void
cDRAW::fPutShadowR(int x,int y,BYTE *sprite,int xRate,int yRate)
{	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code cDRAW_0013");
		return;
	}
#endif

	xp	=	*(short *)(sprite+4);
	yp	=	*(short *)(sprite+6);

	xs	=*(short *)(sprite)		;
	ys	=*(short *)(sprite+2)	;	//	스프라이트 사이즈
	xLen=xs*xRate/100			;
	yLen=ys*yRate/100			;	//	확대/축소된 이미즈 사이즈
	x	-=(xs-xp)*xRate/100;
	y	-=yp*yRate/100;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
					{	int	pos			=	addr+zoomXTable[xp+j].pos-k-1;
						p2SCREEN[pos]	=	(p2SCREEN[pos]&d2HM_MASK)>>1;
					}

				xp	+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	2;
	}
}


void
cDRAW::PutFontNormal(int x,int y,WORD color,BYTE *font,BOOL bMC)
{	int   x1, y1, x2, y2 ,xs ,ys,yp=0;
	int   xPoint=0,i,beginX,endX;
	int   count,bufferPoint=2;
	int   size;
	WORD  put_color=color;	
#ifdef dDEBUG_MODE
	if (!font)
	{	cMSG::Error("Error!! in cDRAW::PutFont[code0061]");
		return;
	}
#endif

	xs=font[0];
	ys=font[1];

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

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
	{	count=font[bufferPoint++];
		if (bMC) put_color=_gCOL[color][yp];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];

			beginX	=	0;
			endX	=	size;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (xPoint+ size	> WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			if (endX			> beginX)	memset16(p2SCREEN+addr+beginX+xPoint,put_color,endX - beginX);
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];
		if (bMC) put_color=_gCOL[color][yp];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];
			memset16(p2SCREEN+addr+xPoint,put_color,size);
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutFontHB(int x,int y,WORD color,BYTE *font,BOOL bMC)
{	int   x1, y1, x2, y2 ,xs ,ys,yp=0;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=2;
	int   size;
#ifdef dDEBUG_MODE
	if (!font)
	{	cMSG::Error("Error!! in cDRAW::PutFontHM2[code0084]");
		return;
	}
#endif

	xs=font[0];
	ys=font[1];

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

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

	WORD  put_color=(color&d2HM_MASK)>>1;

	if (x<0 || x+xs > WIDTH)
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];
		if (bMC) put_color=(_gCOL[color][yp]&d2HM_MASK)>>1;

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];

			beginX	=	0;
			endX	=	size;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (xPoint+ size	> WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			for (j=beginX;j<endX;j++)
				p2SCREEN[+addr+xPoint+j]=((p2SCREEN[+addr+xPoint+j]&d2HM_MASK)>>1)+put_color;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];
		if (bMC) put_color=(_gCOL[color][yp]&d2HM_MASK)>>1;

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];
			for (j=0;j<size;j++)
				p2SCREEN[+addr+xPoint+j]=((p2SCREEN[+addr+xPoint+j]&d2HM_MASK)>>1)+put_color;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutFont(int x,int y,WORD color,BYTE *font,int alpha,BOOL bMC)
{	if (alpha<=0 ) return;
	if (alpha>=32)
	{	PutFontNormal(x,y,color,font,bMC);
		return;
	}
	if (alpha==16)
	{	PutFontHB(x,y,color,font,bMC);
		return;
	}

	int   x1, y1, x2, y2 ,xs ,ys,yp=0;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=2;
	int   size,dest_alpha=32-alpha;
	WORD  put_color;
	UINT  first,second;
#ifdef dDEBUG_MODE
	if (!font )
	{	cMSG::Error("Error!! in cDRAW::PutFontAM2[code0083]");
		return;
	}
#endif

	xs=font[0];
	ys=font[1];

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

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

	first	=	color&d4AM_MASK_1;
	first	>>=	5;
	first	*=	alpha;
	first	&=	d4AM_MASK_1;

	second	=	color&d4AM_MASK_2;
	second	*=	alpha;
	second	>>=	5;
	second	&=	d4AM_MASK_2;

	put_color=	(WORD)(first | second);

	if (x<0 || x+xs > WIDTH)
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];

		if (bMC)
		{	first	=	_gCOL[color][yp]&d4AM_MASK_1;
			first	>>=	5;
			first	*=	alpha;
			first	&=	d4AM_MASK_1;

			second	=	_gCOL[color][yp]&d4AM_MASK_2;
			second	*=	alpha;
			second	>>=	5;
			second	&=	d4AM_MASK_2;

			put_color=	(WORD)(first | second);
		}

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];

			beginX	=	0;
			endX	=	(int) size;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (xPoint+ size	> WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			for (j=beginX;j<endX;j++)
			{	WORD	sc2		=	p2SCREEN[addr+xPoint+j];

				first			=	sc2&d4AM_MASK_1;
				first			>>=	5;
				first			*=	dest_alpha;
				first			&=	d4AM_MASK_1;

				second			=	sc2&d4AM_MASK_2;
				second			*=	dest_alpha;
				second			>>=	5;
				second			&=	d4AM_MASK_2;

				p2SCREEN[addr+xPoint+j]=	put_color + (WORD)(first | second);
			}
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];
		if (bMC)
		{	first	=	_gCOL[color][yp]&d4AM_MASK_1;
			first	>>=	5;
			first	*=	alpha;
			first	&=	d4AM_MASK_1;

			second	=	_gCOL[color][yp]&d4AM_MASK_2;
			second	*=	alpha;
			second	>>=	5;
			second	&=	d4AM_MASK_2;

			put_color=	(WORD)(first | second);

		}

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];
			for (j=0;j<size;j++)
			{	WORD	sc2		=	p2SCREEN[addr+xPoint+j];

				first			=	sc2&d4AM_MASK_1;
				first			>>=	5;
				first			*=	dest_alpha;
				first			&=	d4AM_MASK_1;

				second			=	sc2&d4AM_MASK_2;
				second			*=	dest_alpha;
				second			>>=	5;
				second			&=	d4AM_MASK_2;

				p2SCREEN[addr+xPoint+j]=	put_color + (WORD)(first | second);
			}
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutLayer(int x,int y,WORD color,BYTE *sprite)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint	=8;
	int	  size;
#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code cDRAW_0001");
		return;
	}
#endif

	xs=*(WORD *)(sprite);
	ys=*(WORD *)(sprite+2);
	x-=*(short *)(sprite+4);	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size	=	sprite[bufferPoint++];
		}
	}

	if (x < 0 || x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;
			endX	=	size;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (xPoint+ size	> WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			if (endX			> beginX) memset16(p2SCREEN+addr+beginX+xPoint,color,endX - beginX);

			xPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			memset16(p2SCREEN+addr+xPoint,color,size);

			xPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::fPutLayer(int x,int y,WORD color,BYTE *sprite)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite )
	{	cMSG::Error("Error!! code cDRAW_0005");
		return;
	}
#endif

	xs=*(WORD *)(sprite);
	ys=*(WORD *)(sprite+2);
	x-=(xs - *(short *)(sprite+4));	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint+=2;
	}

	if (x < 0 || x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;
			endX	=	size;

			int	tail	=	xPoint;

			if (xPoint	< size	) beginX= size-xPoint;			//	왼쪽 클리핑..
			if (xPoint	> WIDTH)
			{	endX	= WIDTH-(xPoint-size);		//	오른쪽 클리핑..
				tail	= WIDTH;
			}
			memset16(p2SCREEN+addr+tail-size+1+beginX,color,endX-beginX);

			xPoint	-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			memset16(p2SCREEN+addr+xPoint-size+1,color,size);

			xPoint	-=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutLayerR(int x,int y,WORD color,BYTE *sprite,int xRate,int yRate)
{	if (xRate==100 && yRate==100) {	PutLayer(x,y,color,sprite);	return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code cDRAW_0009");
		return;
	}
#endif

	xs=*(WORD *)(sprite);
	ys=*(WORD *)(sprite+2);
	xp=*(short *)(sprite+4);	//	위치 교정..
	yp=*(short *)(sprite+6);	//	위치 교정..

	xLen=xs*xRate/100	;
	yLen=ys*yRate/100	;	//	확대/축소된 이미즈 사이즈
	x	-=xp*xRate/100	;
	y	-=yp*yRate/100	;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[j+xp].pos+k)=color;

				xp	+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	2;
	}
}

void
cDRAW::fPutLayerR(int x,int y,WORD color,BYTE *sprite,int xRate,int yRate)
{	if (xRate==100 && yRate==100) {	fPutLayer(x,y,color,sprite);	return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code cDRAW_0013");
		return;
	}
#endif

	xs=*(WORD *)(sprite)	;
	ys=*(WORD *)(sprite+2)	;
	xp=*(short *)(sprite+4)	;	//	위치 교정..
	yp=*(short *)(sprite+6)	;	//	위치 교정..
	xLen=xs*xRate/100		;
	yLen=ys*yRate/100		;	//	확대/축소된 이미즈 사이즈
	x	-=(xs-xp)*xRate/100	;
	y	-=yp*yRate/100		;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[xp+j].pos-k-1)=color;

				xp	+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	2;
	}
}