#include <stdio.h>
#include "cANM.H"
#include "cBMPWND.H"
#include "cTRANS.H"
#include "cMAIN.H"
#include "cTEXT.H"
#include "cIMGWND.H"
#include "cWORKWND.H"
#include "cMAIN.H"
#include "cDRAW.H"

char	*regANISET			=	"하늘 에니메이션 셋팅 데이터";
char	*regSDHEADER		=	"하늘 스프라이트 데이터";
char	*regSDHEADER2		=	"하늘 스프라이트 데이터 2";
char	*regSADHEADER		=	"하늘 스프라이트 에니메이션 데이터";
char	*regSADHEADER2		=	"하늘 스프라이트 에니메이션 데이터 2";

int		l_iVersion			=	101;

BYTE	g_bTransColorIndex;


tsSDHEADER	SDHEADER;
tsSADHEADER	SADHEADER;

cRECT	CRASHBOX;
cRECT	g_rectSelect;

cIMG	*cIMG::pHEAD		=	NULL;
cIMG	*cIMG::pTAIL		=	NULL;
cIMG	*cIMG::pCURRENT		=	NULL;	//	링크드 리스트..

int		cIMG::Count			=	0;
int		cIMG::BigXS			=	0;
int		cIMG::BigYS			=	0;
int		cIMG::Current		=	0;
int		cIMG::shadowXRate	=	120;
int		cIMG::shadowYRate	=	80;

int		cIMG::s_iMaxSpriteWidth	=	0;
int		cIMG::s_iMaxSpriteHeight=	0;
int		cIMG::s_iMaxShadowWidth	=	0;
int		cIMG::s_iMaxShadowHeight=	0;

BYTE	cANM::bSAVEOUTLINE	=	TRUE;
BYTE	cANM::bSAVESHADOW	=	FALSE;
BYTE	cANM::bHALF			=	FALSE;

WORD	cANM::PLT[256];

int 	cANM::BPP			=	16;
int 	cANM::AnmType		=	dANI_CUSTOM;
int		cANM::AnmCount		=	9;
int		cANM::CurAnm		=	0;
int		cANM::CurFrame		=	0;
int		cANM::CurDirect		=	0;
char	cANM::FName[256]	=	"";
char	cANM::ANMName[50]	=	"통상 에니메이션";
BYTE	cANM::PALETTE[256*3];

int		cANM::s_iDefaultAttack				=	0xffff;
int		cANM::s_iDefaultMagic				=	0xffff;
WORD	cANM::s_wIsOccasionallyRestAction	=	FALSE;


cANM	ANM[dMAX_ANM];
int		MoveOval			=	2;									//	2:1 좌표계
WORD	g_wCrashSize		=	20;

char	strAnmType[10][40]		=	{	"Normal", "Left and Right 2 directions", "Up and Down 2 directions", "4 directions", "8 directions", "16 directions", "32 directions", "64 directions"};
char	strDirect[10][64][40]	=
{ {"one way"},
	{"left","right"},
	{"up","down"},
	{"north","east","south","west"},
	{"North","Northeast","East","Southeast","South","Southwest","West","Northwest"},
	{"North","North-North-East","North-East","East-North-East",
	"East","South-South-east","South-east","South-South-east",
	"South","South-Southwest","Southwest","West-Southwest",
	"west","west-northwest","northwest","north-northwest"},
	{ "Direction 01", "Direction 02", "Direction 03", "Direction 04", "Direction 05", "Direction 06", "Direction 07", "Direction 08", "Direction 09", "Direction 10", "Direction 11", "Direction 12",
		"Direction 13", "Direction 14", "Direction 15", "Direction 16", "Direction 17", "Direction 18", "Direction 19", "Direction 20", "Direction 21", "Direction 22", "Direction 23", "Direction 24",
		"direction 25", "direction 26", "direction 27", "direction 28", "direction 29", "direction 30", "direction 31", "direction 32"},

	{ "direction 01", "direction 02", "direction 03", "direction 04", "direction 05", "direction 06", "direction 07", "direction 08", "direction 09", "direction 10", "direction 11", "direction 12",
		"Direction 13", "Direction 14", "Direction 15", "Direction 16", "Direction 17", "Direction 18", "Direction 19", "Direction 20", "Direction 21", "Direction 22", "Direction 23", "Direction 24",
		"direction 25", "direction 26", "direction 27", "direction 28", "direction 29", "direction 30", "direction 31", "direction 32", "direction 33", "direction 34", "direction 35", "direction 36",
		"direction 37", "direction 38", "direction 39", "direction 40", "direction 41", "direction 42", "direction 43", "direction 44", "direction 45", "direction 46", "direction 47", "direction 48",
		"direction 49", "direction 50", "direction 51", "direction 52", "direction 53", "direction 54", "direction 55", "direction 56", "direction 57", "direction 58", "direction 59", "direction 60",
		"direction 61","direction 62","direction 63","direction 64"}
};

char	valueDirect[10][16]	=
{	{0},
	{d16DIRECT_W,d16DIRECT_E},
	{d16DIRECT_N,d16DIRECT_S},
	{d16DIRECT_N,d16DIRECT_E,d16DIRECT_S,d16DIRECT_W},
	{d16DIRECT_N,d16DIRECT_NE,d16DIRECT_E,d16DIRECT_SE,d16DIRECT_S,d16DIRECT_SW,d16DIRECT_W,d16DIRECT_NW},
	{d16DIRECT_N,d16DIRECT_NNE,d16DIRECT_NE,d16DIRECT_ENE,
	 d16DIRECT_E,d16DIRECT_ESE,d16DIRECT_SE,d16DIRECT_SSE,
	 d16DIRECT_S,d16DIRECT_SSW,d16DIRECT_SW,d16DIRECT_WSW,
	 d16DIRECT_W,d16DIRECT_WNW,d16DIRECT_NW,d16DIRECT_NNW}
};

char	strAnmName[dMAX_ANM][40]	=
{ "walk",
"jump",
"defense",
"damage",
"Chopping the bell",
"Side cutting",
"Magic",
"Special Move 1",
"Special Move 2",
};

char strCrashType[5][20]	=
{ "none",
	"one",
	"one per animation",
	"one per orientation",
	"one per image"
};


int		_directCount[10]	=	{   1,2,2,4,8,16,32,64};
int		FlipDirect[10]	=	{	1,d2DIRECT_W,1,d4DIRECT_S,d8DIRECT_S,d16DIRECT_S,d32DIRECT_17,d64DIRECT_33};

void
cIMG::Put(int x,int y,int rate,BOOL bflip,int effect)
{
	if (pSPRITE)
		PutSpriteR16[effect+bflip*11](x,y,pSPRITE,rate,rate,effect);

	if (p8SPRITE)
		PutSpriteR8[effect +bflip*11](x,y,p8SPRITE,cANM::PLT,rate,rate,effect);
}

void
cIMG::PutShadow(int x,int y,int rate,BOOL bflip)
{	if (pSHADOW)	cDRAW::PutShadowR(x,y,pSHADOW,rate,rate);
}

void
cIMG::PutLayer(int xp,int yp,WORD color,int rate,BOOL bflip)
{	if (pLAYER)
	{	if (bflip)	cDRAW::fPutLayerR(xp,yp,color,pLAYER,rate,rate);
		else		cDRAW::PutLayerR(xp,yp,color,pLAYER,rate,rate);
	}
}

BOOL
cIMG::MakeShadow()
{	return TRUE;
}

void
cIMG::DelShadow()
{	sSize	=	0;
	pKILL(pSHADOW);

	bVIRGIN	=	FALSE;
}

void
cIMG::Copy(cIMG *img)
{
	Pos.x	=	img->Pos.x	;
	Pos.y	=	img->Pos.y	;
	sPos.x	=	img->sPos.x	;
	sPos.y	=	img->sPos.y	;
	Size	=	img->Size	;
	sSize	=	img->sSize	;
	lSize	=	img->lSize	;
	serial	=	img->serial	;
	xs		=	img->xs		;
	ys		=	img->ys		;
	xp		=	img->xp		;
	yp		=	img->yp		;

	if (img->pSPRITE)
	{
		pSPRITE	=	new WORD	[Size];
		memcpy(pSPRITE,img->pSPRITE,Size*2);
	}

	if (img->p8SPRITE)
	{
		p8SPRITE=	new BYTE	[Size];
		memcpy(p8SPRITE,img->p8SPRITE,Size);
	}

	if (img->pSHADOW)
	{
		pSHADOW	=	new BYTE	[sSize];
		memcpy(pSHADOW,img->pSHADOW,sSize);
	}

	if (img->pLAYER)
	{
		pLAYER	=	new	BYTE	[lSize];
		memcpy(pLAYER,img->pLAYER,lSize);
	}
}

BOOL
cIMG::isIN(int x,int y)
{
	char	*checkField;

	if (pSPRITE)
	{
		int	width	=	pSPRITE[0];
		int	height	=	pSPRITE[1];

		if (x<0||y<0||x>=width||y>=height)
			return FALSE;

		checkField	=	new char [width*height];

		int		xPoint=0,i,index;
		int		count,bufferPoint=4;
		int		size;

		int		addr=0;

		memset(checkField,0,width*height);

		for(index=0;index<height;index++)
		{	
			count	=	pSPRITE[bufferPoint++];
			xPoint	=	0;

			for(i=0;i<count;i++)
			{
				xPoint	+=	pSPRITE[bufferPoint++];
				size	=	pSPRITE[bufferPoint++];

				memset(checkField+addr+xPoint,1,size);

				bufferPoint	+=	size;
				xPoint		+=	size;
			}

			addr+=width;
		}

		BOOL	result;

		if (checkField[x+y*width]==1)
			result=TRUE;
		else
			result=FALSE;

		pKILL(checkField);

		return result;
	}

	if (p8SPRITE)
	{	
		int	width	=	*(WORD *) (p8SPRITE);
		int	height	=	*(WORD *) (p8SPRITE+2);

		if (x<0||y<0||x>=width||y>=height)
			return FALSE;

		checkField	=	new char [width*height];

		int		xPoint=0,i,index;
		int		count,bufferPoint=8;
		int		size;

		int		addr=0;

		memset(checkField,0,width*height);

		for(index=0;index<height;index++)
		{
			count	=	p8SPRITE[bufferPoint++];
			xPoint	=	0;

			for(i=0;i<count;i++)
			{
				xPoint	+=	p8SPRITE[bufferPoint++];
				size	=	p8SPRITE[bufferPoint++];

				memset(checkField+addr+xPoint,1,size);

				bufferPoint	+=	size;
				xPoint		+=	size;
			}

			addr+=width;
		}

		BOOL	result;

		if (checkField[x+y*width]==1)
			result=TRUE;
		else
			result=FALSE;

		pKILL(checkField);

		return result;
	}

	return FALSE;
}


void
cIMG::reform(int dx,int dy)
{
	bVIRGIN	=	FALSE;

	if (_MAIN.Lock)
	{	ANM[0].RefitPos[0].x	+=	dx;
		ANM[0].RefitPos[0].y	+=	dy;
		return;
	}

	xp		+=	dx;
	yp		+=	dy;

	if (pSPRITE)
		pSPRITE[2]	+=	dx,pSPRITE[3]	+=	dy;
	if (p8SPRITE)
	{	*(short *)(p8SPRITE	+4)	+=	dx;
		*(short *)(p8SPRITE	+6)	+=	dy;
	}
	if (pLAYER )
	{	*(short *)(pLAYER	+4)	+=	dx;
		*(short *)(pLAYER	+6)	+=	dy;
	}

	if (pSHADOW)
	{	*(short *)(pSHADOW	+4)	+=	dx;
		*(short *)(pSHADOW	+6)	+=	dy;
	}

}

void
cIMG::reformShadow(int dx,int dy)
{
	if (_MAIN.Lock) return;

	if (pSHADOW)
	{	*(short *)(pSHADOW	+4)	+=	dx;
		*(short *)(pSHADOW	+6)	+=	dy;
	}

	bVIRGIN		=	FALSE;
}

void
cIMG::rePos(int x,int y)
{	bVIRGIN		=	FALSE;
	if (_MAIN.Lock)	return;

	xp		=	x;
	yp		=	y;
	int	dx,dy;

	if (pSPRITE)
	{	dx	=	pSPRITE[2]-x;
		dy	=	pSPRITE[3]-y;
		pSPRITE[2]	=	x,pSPRITE[3]	=	y;
	}
	if (p8SPRITE )
	{	dx	=	*(short *)(p8SPRITE	+4)-x;
		dy	=	*(short *)(p8SPRITE	+6)-y;
		*(short *)(p8SPRITE	+4)	=	x;
		*(short *)(p8SPRITE	+6)	=	y;
	}
	if (pSHADOW)
	{	*(short *)(pSHADOW	+4)	-=	dx;
		*(short *)(pSHADOW	+6)	-=	dy;
	}
	if (pLAYER )
	{	*(short *)(pLAYER	+4)	=	x+1;
		*(short *)(pLAYER	+6)	=	y+1;
	}

}

void
cIMG::SetLayer(BYTE *layer,int size)
{	pLAYER	=	layer;
	lSize	=	size;
}

void
cIMG::SetShadow(BYTE *shadow,int size)
{	pSHADOW	=	shadow;
	sSize	=	size;
}

//	정적 멤버

BOOL
cIMG::Add(WORD *data,int xs1,int ys1,int xp1,int yp1)
{	bVIRGIN	=	FALSE;

	if (xp1==0xffff)	xp1	=0;//xs1/2;
	if (yp1==0xffff)	yp1	=0;//ys1/2;

	cIMG		*img;
	WORD		*image	=	NULL;

	img			=	new cIMG();
	img->xp		=	xp1;
	img->yp		=	yp1;
	img->xs		=	xs1;
	img->ys		=	ys1;

	int x,y;

	BigXS	=	max(img->xs,BigXS);
	BigYS	=	max(img->ys,BigYS);

	image	=	new WORD[(xs1+3)*(ys1+3)];
	memset(image,(WORD)0,((xs1+3)*(ys1+3)));

	for (y=0;y<ys1;y++)	
		for (x=0;x<xs1;x++)	
			if (data[x+y*xs1]!=_BMPWND.transColor)
			{	image[x+1+(y+1)*(xs1+2)	 +1]=1,image[x-1+(y+1  )*(xs1+2)+1]=1;
				image[x+1+(y+1-1)*(xs1+2)+1]=1,image[x  +(y+1-1)*(xs1+2)+1]=1,image[x-1+(y+1-1)*(xs1+2)+1]=1;
				image[x+1+(y+1+1)*(xs1+2)+1]=1,image[x  +(y+1+1)*(xs1+2)+1]=1,image[x-1+(y+1+1)*(xs1+2)+1]=1;
			}

	for (y=0;y<ys1;y++)	
		for (x=0;x<xs1;x++)	
			if (data[x+y*xs1]!=_BMPWND.transColor)
				image[x+(y+1)*(xs1+2)+1]	=	0;


	int	size;
	img->pLAYER		=	cTRANS::ImageToShadow(xs1+2,ys1+2,(short )xp1+1,(short )yp1+1,0,image,size);
	img->lSize		=	size;

	pKILL(image);

	img->pSPRITE	=	cTRANS::ImageToSprite(xs1,ys1,(short )xp1,(short )yp1,_BMPWND.transColor,data,size);
	img->Size		=	size;

	return Add(img);
}

BOOL
cIMG::Add(BYTE *data,int xs1,int ys1,int xp1,int yp1)
{
	bVIRGIN	=	FALSE;

	if	(xp1==0xffff)
		xp1	=0;//xs1/2;
	if	(yp1==0xffff)
		yp1	=0;//ys1/2;

	cIMG	*img;
	BYTE	*image	=	NULL;

	img			=	new cIMG();
	img->xp		=	xp1;
	img->yp		=	yp1;
	img->xs		=	xs1;
	img->ys		=	ys1;

	int x,y;

	BigXS	=	max(img->xs,BigXS);
	BigYS	=	max(img->ys,BigYS);

	image	=	new BYTE [(xs1+3)*(ys1+3)];

	memset(image,0,((xs1+3)*(ys1+3)));

	for (y=0;y<ys1;y++)	
		for (x=0;x<xs1;x++)	
			if (data[x+y*xs1]!=_BMPWND.transColor)
			{
				image[x+1+(y+1)*(xs1+2)	 +1]=1,image[x-1+(y+1  )*(xs1+2)+1]=1;
				image[x+1+(y+1-1)*(xs1+2)+1]=1,image[x  +(y+1-1)*(xs1+2)+1]=1,image[x-1+(y+1-1)*(xs1+2)+1]=1;
				image[x+1+(y+1+1)*(xs1+2)+1]=1,image[x  +(y+1+1)*(xs1+2)+1]=1,image[x-1+(y+1+1)*(xs1+2)+1]=1;
			}

	for (y=0;y<ys1;y++)	
		for (x=0;x<xs1;x++)	
			if (data[x+y*xs1]!=_BMPWND.transColor)
				image[x+(y+1)*(xs1+2)+1]	=	0;

	int	size;
	img->pLAYER		=	cTRANS::ImageToShadow(xs1+2,ys1+2,(short )xp1+1,(short )yp1+1,0,image,size);
	img->lSize		=	size;

	pKILL(image);

	img->p8SPRITE	=	cTRANS::ImageToSprite(xs1,ys1,(short )xp1,(short )yp1,(BYTE)_BMPWND.transColor,data,size);
	img->Size		=	size;

	return Add(img);
}

BOOL
cIMG::AddShadow(WORD *data,int xs1,int ys1,int xp1,int yp1)
{	bVIRGIN	=	FALSE;

	cIMG		*img	=	Get(0);

	if (!img)	return FALSE;

	while(1)
	{	if (!img->pSHADOW)	break;

		img	=	img->pNEXT;

		if (img == pTAIL)	return FALSE;
	}

	int	size;
	img->pSHADOW	=	cTRANS::ImageToShadow(xs1,ys1,(short )xp1,(short )yp1,_BMPWND.transColor,data,size);
	img->sSize		=	size;

	return TRUE;
}

BOOL
cIMG::AddShadow(BYTE *data,int xs1,int ys1,int xp1,int yp1)
{	bVIRGIN	=	FALSE;

	cIMG		*img	=	Get(0);

	if (!img)	return FALSE;

	while(1)
	{	if (!img->pSHADOW)	break;

		img	=	img->pNEXT;

		if (img == pTAIL)	return FALSE;
	}

	int	size;
	img->pSHADOW	=	cTRANS::ImageToShadow(xs1,ys1,(short )xp1,(short )yp1,(BYTE)_BMPWND.transColor,data,size);
	img->sSize		=	size;

	return TRUE;
}

BOOL
cIMG::Add(WORD *sprite,int size)
{	cIMG		*img;

	img			=	new cIMG();
	img->xs		=	(short )sprite[0];
	img->ys		=	(short )sprite[1];
	img->xp		=	(short )sprite[2];
	img->yp		=	(short )sprite[3];

	BigXS		=	max(img->xs,BigXS);
	BigYS		=	max(img->ys,BigYS);

	img->pSPRITE=	sprite;
	img->Size	=	size;

	return Add(img);
}

BOOL
cIMG::Add(BYTE *sprite,int size)
{	cIMG		*img;

	img			=	new cIMG();
	img->xs		=	*(WORD *)(sprite  );
	img->ys		=	*(WORD *)(sprite+2);
	img->xp		=	*(short *)(sprite+4);
	img->yp		=	*(short *)(sprite+6);

	BigXS		=	max(img->xs,BigXS);
	BigYS		=	max(img->ys,BigYS);

	img->p8SPRITE=	sprite;
	img->Size	=	size;

	
	return Add(img);
}

BOOL
cIMG::Add(cIMG *img)
{	if (!pHEAD)
	{	pHEAD		=	new cIMG();
		pTAIL		=	new cIMG();
		pTAIL->pPRE	=	pHEAD;
		pTAIL->pNEXT=	NULL;
		pHEAD->pPRE	=	NULL;
		pHEAD->pNEXT=	pTAIL;
	}

	img->pPRE			=	pTAIL->pPRE;
	img->pNEXT			=	pTAIL;
	pTAIL->pPRE->pNEXT	=	img;
	pTAIL->pPRE			=	img;

	Count++;

	BigXS	=	max(img->xs,BigXS);
	BigYS	=	max(img->ys,BigYS);

	return TRUE;
}

BOOL
cIMG::Del(cIMG *img)
{	if (Count <= 0) return FALSE;
	if (!img)	return FALSE;

	img->pPRE->pNEXT	=	img->pNEXT;
	img->pNEXT->pPRE	=	img->pPRE;
	KILL(img);

	GetMaxImage();

	Count--;
	bVIRGIN	=	FALSE;

	return TRUE;
}

BOOL
cIMG::Del(int index)
{	if (Count <= 0) return FALSE;

	cIMG *img=Get(index);

	return Del(img);
}

BOOL
cIMG::Move(int srcN,int destN)
{	if (Count <= 0) return FALSE;

	cIMG *src=Get(srcN);
	cIMG *dest=Get(destN);

	if (destN == Count)	dest	=	pTAIL;

	if (!src || !dest) return FALSE;

	src->pPRE->pNEXT	=	src->pNEXT;
	src->pNEXT->pPRE	=	src->pPRE;
	src->pPRE			=	dest->pPRE;
	src->pNEXT			=	dest;
	dest->pPRE->pNEXT	=	src;
	dest->pPRE			=	src;
	
	if (srcN>destN)	Current	=	destN;
	else			Current	=	destN-1;

	return TRUE;
}

cIMG *
cIMG::Get(int index)
{
	if (Count <= 0 || index >= Count)
		return NULL;
	
	cIMG *img=pHEAD->pNEXT;

	for (int i=0;i<Count;i++)
	{
		if (i==index)
			return img;
		img=img->pNEXT;
	}
	
	return NULL;
}

int
cIMG::Get(cIMG *cmp)
{	if (Count <= 0 || !cmp) return 0xffff;

	cIMG *img=pHEAD->pNEXT;

	for (int i=0;i<Count;i++)
	{	if (img==cmp) return i;
		img=img->pNEXT;
	}

	return 0xffff;
}

void
cIMG::GetMaxImage()
{
	if	(Count <= 0)
		return;

	s_iMaxSpriteWidth	=	0;
	s_iMaxSpriteHeight	=	0;
	s_iMaxShadowWidth	=	0;
	s_iMaxShadowHeight	=	0;

	{
		cRECT	rectFrame,rectShadowFrame;

		rectFrame.Set(0xffff,0xffff,-0xffff,-0xffff);
		rectShadowFrame.Set(0xffff,0xffff,-0xffff,-0xffff);

		BigXS		=	0,BigYS		=	0;

		cIMG *img	=	pHEAD;

		int	iMaxDX=0,iMaxDY=0;
		int	iShadowMaxDX=0,iShadowMaxDY=0;

		int	iImageWidth		=	0;
		int	iImageHeight	=	0;
		int	iShadowWidth	=	0;
		int	iShadowHeight	=	0;

		BOOL	bImageSizeSame	=	TRUE;
		BOOL	bShadowSizeSame	=	TRUE;

		for (int i=0;i<Count;i++)
		{	
			if	(img->xs > BigXS)
				BigXS = img->xs;

			if	(img->ys > BigYS)
				BigYS = img->ys;

			img	=	img->pNEXT;

			//	이미지
			{
				int	iXS		=	img->xs;				//	스프라이트 사이즈
				int	iYS		=	img->ys;
				int	iDX		=	img->xp;
				int	iDY		=	img->yp;

				if	(iImageWidth	==	0)
				{
					iImageWidth		=	iXS;
					iImageHeight	=	iYS;
				}
				else
				if	(iImageWidth	!=	iXS	||	iImageHeight	!=	iYS)
					bImageSizeSame	=	FALSE;

				cRECT	rect;

				rect.x1		=	-iDX;
				rect.y1		=	-iDY;
				rect.x2		=	iDX+iXS;
				rect.y2		=	iDY+iYS;

				iMaxDX		=	max(iMaxDX,iXS*2-iDX*2);
				iMaxDY		=	max(iMaxDY,iYS*2-iDY*2);
				iMaxDX		=	max(iMaxDX,iDX*2);
				iMaxDY		=	max(iMaxDY,iDY*2);

				rectFrame.x1=	min(rectFrame.x1,rect.x1);
				rectFrame.y1=	min(rectFrame.y1,rect.y1);
				rectFrame.x2=	max(rectFrame.x2,rect.x2);
				rectFrame.y2=	max(rectFrame.y2,rect.y2);	
			}

			if	(img->pSHADOW)	//	그림자
			{
				int	iSXS	=	*(WORD *)img->pSHADOW;				//	스프라이트 사이즈
				int	iSYS	=	*(WORD *)(img->pSHADOW+2);
				int	iSDX	=	*(short *)(img->pSHADOW+4);
				int	iSDY	=	*(short *)(img->pSHADOW+6);

				if	(iShadowWidth	==	0)
				{
					iShadowWidth	=	iSXS;
					iShadowHeight	=	iSYS;
				}
				else
				if	(iShadowWidth	!=	iSXS	||	iShadowHeight	!=	iSYS)
					bShadowSizeSame	=	FALSE;

				cRECT	rect;

				rect.x1		=	-iSDX;
				rect.y1		=	-iSDY;
				rect.x2		=	iSDX+iSXS;
				rect.y2		=	iSDY+iSYS;

				iShadowMaxDX=	max(iShadowMaxDX,iSXS*2-iSDX*2);
				iShadowMaxDY=	max(iShadowMaxDY,iSYS*2-iSDY*2);
				iShadowMaxDX=	max(iShadowMaxDX,iSDX*2);
				iShadowMaxDY=	max(iShadowMaxDY,iSDY*2);

				rectShadowFrame.x1=	min(rectShadowFrame.x1,rect.x1);
				rectShadowFrame.y1=	min(rectShadowFrame.y1,rect.y1);
				rectShadowFrame.x2=	max(rectShadowFrame.x2,rect.x2);
				rectShadowFrame.y2=	max(rectShadowFrame.y2,rect.y2);
			}
		}

		if	(bImageSizeSame)
		{
			s_iMaxSpriteWidth	=	iImageWidth;
			s_iMaxSpriteHeight	=	iImageHeight;
		}
		else
		{
			s_iMaxSpriteWidth	=	rectFrame.width();
			s_iMaxSpriteHeight	=	rectFrame.height();

			s_iMaxSpriteWidth	=	max(s_iMaxSpriteWidth,iMaxDX);
			s_iMaxSpriteHeight	=	max(s_iMaxSpriteHeight,iMaxDY);
		}

		if	(rectShadowFrame.x1	!=	0xffff)
		{
			if	(bShadowSizeSame)
			{
				s_iMaxShadowWidth	=	iShadowWidth;
				s_iMaxShadowHeight	=	iShadowHeight;
			}
			else
			{
				s_iMaxShadowWidth	=	rectShadowFrame.width();
				s_iMaxShadowHeight	=	rectShadowFrame.height();

				s_iMaxShadowWidth	=	max(s_iMaxShadowWidth,iShadowMaxDX);
				s_iMaxShadowHeight	=	max(s_iMaxShadowHeight,iShadowMaxDY);
			}
		}
	}
}

void
cIMG::ShadowAll()
{
	if (Count <= 0)
		return;
}

void
cIMG::ShadowAnm()
{	if (Count <= 0) return;
}

void
cIMG::ShadowCurrent()
{	if (Count <= 0) return;
}

void
cIMG::Reset()
{	int	count	=	0;

	if (pHEAD)
	{	cIMG *img=pHEAD->pNEXT;

		for (int i=0;i<cIMG::Count;i++)
		{	img=img->pNEXT;
			KILL(img->pPRE);
		}
		KILL(pTAIL);
		KILL(pHEAD);
	}

	Count		=	0,BigXS	=	0,BigYS	=	0,Current	=	0;
	g_wCrashSize=	20;
	pCURRENT	=	NULL;
	CRASHBOX.x1	=	0;
	CRASHBOX.y1	=	0;
	CRASHBOX.x2	=	0;
	CRASHBOX.y2	=	0;
	g_rectSelect.x1	=	0;
	g_rectSelect.y1	=	0;
	g_rectSelect.x2	=	0;
	g_rectSelect.y2	=	0;
}

void
cIMG::ClearSelect()
{	if (Count <= 0) return;
	
	cIMG *img=pHEAD->pNEXT;
	
	while(img!=pTAIL)
	{	if (img!=pCURRENT) img->isSelect	=	FALSE;
		img=img->pNEXT;
	}
}

void
cIMG::Select(int num)
{	if (Count <= 0) return;
	if (Current == num ) return;

	ClearSelect();
	
	cIMG *img=pCURRENT;

	if (Current > num )
		for (int i=0;i<Current-num;i++)
		{	img				=img->pPRE;
			img->isSelect	=TRUE;
		}
	
	if (Current < num )
		for (int i=0;i<num-Current;i++)
		{	img				=img->pNEXT;
			img->isSelect	=TRUE;
		}
}


void
cIMG::Reform(int dx,int dy,int value,int control,BOOL _bIsPressedAlt)
{
	if	(Count <= 0)
		return;

	int		i,j=0;

	cIMG *img=pHEAD->pNEXT;

	switch(value)
	{
		case	0	:
		{
			while(img!=pTAIL)
			{
				if (img->isSelect)
				{
					if	(control)
						img->reformShadow(dx,dy);
					else
						img->reform(dx,dy);
				}

				img		=	img->pNEXT;
			}
			break;
		}

		case	1	:
			while(img	!=	pTAIL)
			{
				if	(control)
					img->reformShadow(dx,dy);
				else
					img->reform(dx,dy);

				img	=	img->pNEXT;
				if (_MAIN.Lock) break;
			}
			break;

		case	2	:
			if (_MAIN.Lock)
			{	
				ANM[0].RefitPos[0].x	+=	dx;
				ANM[0].RefitPos[0].y	+=	dy;
			}
			else
			{
				if	(_bIsPressedAlt)
				{
					for (i=0;i<cANM::GetMaxFrame();i++)
					{
						cIMG *frm	=	Get(ANM[cANM::CurAnm].Sprite[cANM::CurDirect][i]);

						if	(frm->bIsMoved)
							continue;

						if	(frm && frm!=pTAIL)
						{	
							if	(control)
								frm->reformShadow(dx,dy);
							else
								frm->reform(dx,dy);

							frm->bIsMoved	=	TRUE;
						}
					}

					for (i=0;i<cANM::GetMaxFrame();i++)
					{
						cIMG *frm	=	Get(ANM[cANM::CurAnm].Sprite[cANM::CurDirect][i]);

						if	(frm)
							frm->bIsMoved	=	FALSE;
					}

				}
				else
				{
					int iDirect;
					for (iDirect=0;iDirect<cANM::GetMaxDirect();iDirect++)
						for (i=0;i<cANM::GetMaxFrame();i++)
						{
							cIMG *frm	=	Get(ANM[cANM::CurAnm].Sprite[iDirect][i]);

							if	(frm->bIsMoved)
								continue;

							if	(frm && frm!=pTAIL)
							{	
								if (control)
									frm->reformShadow(dx,dy);
								else
									frm->reform(dx,dy);
							}

							frm->bIsMoved	=	TRUE;
						}

					for (iDirect=0;iDirect<cANM::GetMaxDirect();iDirect++)
						for (i=0;i<cANM::GetMaxFrame();i++)
						{
							cIMG *frm	=	Get(ANM[cANM::CurAnm].Sprite[iDirect][i]);

							if	(frm)
								frm->bIsMoved	=	FALSE;
						}
				}
			}
			break;
	}
}

//	일반 멤버..

void
cANM::ClearData(int type)
{	int i;

	memset(isEvent,0,dMAX_FRAME);
	memset(BackFrame,0,dMAX_DIRECT*dMAX_FRAME);

	if (type	!=	0xffff)
	{	directCount	=	_directCount[type];
		anmtype		=	type;
		FrameCount	=	10;
		strcpy(Name,"명칭 없음");
		bSaveReleasePoint	=	FALSE;			//	릴리즈 포인트 저장?
		bSaveRefitPos		=	FALSE;			//	프레임 보정 위치 저장?
	}

	LinkAnm		=	0xffff;
	ALPHA		=	dPUT_NORMAL;
	FPS			=	16;

	RefitPos[0].x	=	0;
	RefitPos[0].y	=	0;

	for (i=0;i<dMAX_DIRECT;i++)
	{	for (int j=0;j<dMAX_FRAME;j++)	Pos[i][j].x=0,Pos[i][j].y=0,Sprite[i][j]=0;
		Release[i].x	=	0;
		Release[i].y	=	0;
	}
}

void
cANM::Set(cANM *anm)
{	int i,j;

	strcpy(Name,anm->Name);

	RefitPos[0].x		=	anm->RefitPos[0].x;
	RefitPos[0].y		=	anm->RefitPos[0].y;

	for (i=0;i<dMAX_DIRECT;i++)	
	{	for (j=0;j<dMAX_FRAME;j++)	
		{	Pos[i][j].x	=	anm->Pos[i][j].x;
			Pos[i][j].y	=	anm->Pos[i][j].y;
			Sprite[i][j]=	anm->Sprite[i][j];
		}
		Release[i].x	=	anm->Release[i].x;							//	마법이나 공격시 릴리즈 포인트
		Release[i].y	=	anm->Release[i].y;							//	마법이나 공격시 릴리즈 포인트
	}

	for (i=0;i<dMAX_FRAME;i++)	isEvent[i]	=	anm->isEvent[i];

	FrameCount	=	anm->FrameCount	,	directCount	=	anm->directCount;
	ALPHA		=	anm->ALPHA		,	anmtype		=	anm->anmtype	;

	bSaveReleasePoint	=	anm->bSaveReleasePoint;			//	릴리지 포인트 저장?
	bSaveRefitPos		=	anm->bSaveRefitPos;				//	프레임 보정 위치 저장?
}

void
cANM::Sync(cANMDATA *anm)
{	int i,j;

	anmtype			=	anm->AnmType;
	FrameCount		=	anm->FrameCount;
	directCount		=	anm->DirectCount;
	FPS				=	anm->FPS;
	PPS				=	anm->PPS;


	RefitPos[0].x	=	anm->RefitPos.x;
	RefitPos[0].y	=	anm->RefitPos.y;

	for (i=0;i<directCount;i++)
	{
		for (j=0;j<FrameCount;j++)
			Sprite[i][j]	=	anm->Sprite[i*FrameCount+j];
	}

	bVIRGIN				=	FALSE;
}


void
cANM::Set(cANMDATA *anm)
{	int i,j;

	anmtype				=	anm->AnmType;
	FrameCount			=	anm->FrameCount;
	directCount			=	anm->DirectCount;
	FPS					=	anm->FPS;
	PPS					=	anm->PPS;

	for (i=0;i<directCount;i++)
		for (j=0;j<FrameCount;j++)	
			Sprite[i][j]	=	anm->Sprite[i*FrameCount+j];

	bVIRGIN				=	FALSE;
}

void
cANM::Put(int xp,int yp,int rate,BOOL flip)
{	int frame	=	CurFrame  % FrameCount;
	int direct	=	CurDirect % directCount;

	int index	=	Sprite[direct][frame];

	cIMG *img	=	cIMG::Get(index);

	if (!img)	return;

	img->Put(xp-Pos[direct][frame].x,yp-Pos[direct][frame].y,rate,flip,ALPHA);
}

void
cANM::PutShadow(int xp,int yp,int rate,BOOL flip)
{	int frame	=	CurFrame  % FrameCount;
	int direct	=	CurDirect % directCount;

	int index	=	Sprite[direct][frame];

	cIMG *img	=	cIMG::Get(index);

	if (!img)	return;

	img->PutShadow(xp-Pos[direct][frame].x,yp-Pos[direct][frame].y,rate,flip);
}

BOOL
cANM::isBack(int direct,int frame)
{	if (direct	== 0xffff) direct	= CurDirect;
	if (frame	== 0xffff) frame	= CurFrame;

	frame	=	frame  % FrameCount;
	direct	=	direct % directCount;

	return BackFrame[direct][frame];
}

//	정적 멤버..
int
cANM::getanm(int anm)		
{	if (anm	 ==0xffff) return CurAnm;
	anm		=	min(anm,AnmCount-1);
	anm		=	max(anm,0);	
	return anm;
}

int	
cANM::getframe(int frame)	
{	if (frame==0xffff) return CurFrame;
	frame	=	min(frame,dMAX_FRAME-1);
	frame	=	max(frame,0);	
	return frame;
}

int
cANM::getdirect(int direct)
{	if (direct==0xffff)return CurDirect;
	direct	=	min(direct,dMAX_DIRECT-1);
	direct	=	max(direct,0);	
	return direct;
}

void
cANM::SetMaxAnm(int num)
{	AnmCount=num;
	AnmCount=max(0,AnmCount);
	AnmCount=min(dMAX_ANM,AnmCount);
	if (cIMG::Count > 0 ) bVIRGIN	=FALSE;

	int i,j,k;

	for (i=AnmCount;i<dMAX_ANM;i++)
	{	for (j=0;j<dMAX_FRAME;j++) ANM[i].isEvent[j]=FALSE;
		ANM[i].FrameCount	=	10;
		ANM[i].ALPHA		=	dPUT_NORMAL;

		for (k=0;k<dMAX_DIRECT;i++)	for (int j=0;j<dMAX_FRAME;j++)
			ANM[i].Pos[k][j].x=0,ANM[i].Pos[k][j].y=0,ANM[i].Sprite[k][j]=0;
	}
}

int
cANM::GetMaxFrame(int anm)
{	anm=getanm(anm);
	return ANM[anm].FrameCount;
}

int
cANM::GetMaxDirect(int anm)
{	anm=getanm(anm);
	return ANM[anm].directCount;
}

BOOL
cANM::isEventFrame(int frame,int anm)
{	anm=getanm(anm);
	frame=getframe(frame);

	if (anm		>=	AnmCount		) return FALSE;
	if (frame   >=	GetMaxFrame(anm)) return FALSE;

	return ANM[anm].isEvent[frame];
}

void
cANM::SetEventFrame(int frame,int anm)
{
	frame	=	getframe(frame);
	anm		=	getanm(anm);

	ANM[CurAnm].isEvent[frame]=TRUE;
	bVIRGIN=FALSE;
}

int
cANM::GetFrameImage(int frame,int anm,int direct)
{
	frame	=	getframe(frame);
	anm		=	getanm(anm);
	direct	=	getdirect(direct);

	if (frame < 0 || frame >= ANM[CurAnm].FrameCount)
		return 0xffff;

	return ANM[anm].Sprite[direct][frame];
}

void
cANM::SetMaxFrame(int frame,int anm)
{	anm						=	getanm(anm);
	frame					=	min(dMAX_FRAME,frame);
	frame					=	max(0,frame);
	ANM[CurAnm].FrameCount	=	frame;
	if (cIMG::Count > 0 ) bVIRGIN=FALSE;
	for (int i=ANM[CurAnm].FrameCount;i<dMAX_FRAME;i++)
		for (int j=0;j<dMAX_DIRECT;j++)	ANM[CurAnm].Sprite[j][i]	=	0;
}

void
cANM::SetCurrentFrame(int frame,int anm,int direct)
{	if (frame < 0 ) return;
	CurFrame					=	getframe(frame);
	cIMG::SetCURRENT(GetFrameImage(frame,anm,direct));

	memset(IsSelectedFrame,0,dMAX_FRAME);
	IsSelectedFrame[CurFrame]	=	1;
}

void	
cANM::SetFrameImage(int image,int frame,int anm,int direct)
{	frame	=	getframe(frame);
	anm		=	getanm(anm);
	direct	=	getdirect(direct);
	ANM[anm].Sprite[direct][frame]	=	image;

	if (bHALF)	ANM[anm].Sprite[GetFlipDirect(direct)][frame]	=	image;
	bVIRGIN	=	FALSE;
}

void	
cANM::MinusMaxAnm()
{	SetMaxAnm(AnmCount-1);			
}
void	
cANM::PlusMaxAnm()
{	SetMaxAnm(AnmCount+1);				
}
void	
cANM::MinusMaxFrame(int anm)	
{	SetMaxFrame(GetMaxFrame(anm)-1,anm);
}
void
cANM::PlusMaxFrame(int anm)	
{	SetMaxFrame(GetMaxFrame(anm)+1,anm);
}
void
cANM::MinusCurrentImage()
{	cIMG::SetCURRENT(cIMG::Current-1);	
}
void
cANM::PlusCurrentImage()
{	cIMG::SetCURRENT(cIMG::Current+1);	
}

void	
cANM::MinusCurrentFrame()
{	CurFrame--;
	if (CurFrame < 0			) CurFrame=GetMaxFrame()-1;
	SetCurrentFrame();
}

void	
cANM::PlusCurrentFrame()
{	CurFrame++;
	if (CurFrame >= GetMaxFrame()) CurFrame=0;
	SetCurrentFrame();
}

void	
cANM::MinusCurrentAnm()
{	CurFrame	=	0,CurDirect	=	0,CurAnm	=	max(0,CurAnm-1);

	if (AnmCount == ImageFolderCount)	CurrentImageFolder	=	CurAnm;

	SetCurrentFrame();
}

void
cANM::PlusCurrentAnm()
{	CurFrame	=	0,CurDirect	=	0,CurAnm	=	min(AnmCount-1,CurAnm+1);

	if (AnmCount == ImageFolderCount)	CurrentImageFolder	=	CurAnm;

	SetCurrentFrame();
}

void
cANM::MinusCurrentDirect()
{	CurDirect--;
	if (CurDirect< 0 ) CurDirect= GetMaxDirect()-1;
	SetCurrentFrame();
}

void	
cANM::PlusCurrentDirect()
{	CurDirect++;
	if (CurDirect>= GetMaxDirect()) CurDirect= 0;
	SetCurrentFrame();
}

int
cANM::GetAnmType(int anm)
{	if (anm>=AnmCount) anm=CurAnm;

	return ANM[anm].anmtype;
}

int
cANM::GetDirCount(int anm)
{	if (anm>=AnmCount) anm=CurAnm;

	return _directCount[ANM[anm].anmtype];
}

int		
cANM::GetFlipDirect(int anm,int direct)
{	anm		=getanm(anm);
	direct	=getdirect(direct);

	if (_MAIN.workMode==dWORK_SET_SPRITE) return direct;
	if (!bHALF)	return direct;

	switch(ANM[anm].anmtype)
	{	case	dANI_CUSTOM			:	return direct;
		case	dANI_DIRECT2X		:	return 1-direct;
		case	dANI_DIRECT2Y		:	return direct;
		case	dANI_DIRECT4		:
					if (direct <  d4DIRECT_S && direct > 0	) direct=direct + (d4DIRECT_S- direct ) * 2;
			else	if (direct >  d4DIRECT_S				) direct=direct - (direct - d4DIRECT_S) * 2;
			break;

		case	dANI_DIRECT8		:
					if (direct <  d8DIRECT_S && direct > 0	) direct=direct + (d8DIRECT_S- direct ) * 2;
			else	if (direct >  d8DIRECT_S				) direct=direct - (direct - d8DIRECT_S) * 2;
			break;
				
		case	dANI_DIRECT16		:
					if (direct <  d16DIRECT_S && direct > 0 ) direct=direct + (d16DIRECT_S- direct ) * 2;
			else	if (direct >  d16DIRECT_S				) direct=direct - (direct - d16DIRECT_S) * 2;
			break;
	}

	return direct;
}

int
cANM::GetDirect(int anm,int direct)
{	anm		=	getanm(anm);
	direct	=	getdirect(direct);

	if (_MAIN.workMode==dWORK_SET_SPRITE) return direct;
	if (!bHALF)	return direct;

	switch(ANM[anm].anmtype)
	{	case	dANI_CUSTOM			:	return direct;
		case	dANI_DIRECT2X		:	if (direct == d2DIRECT_E) return d2DIRECT_W;
		case	dANI_DIRECT2Y		:	return direct;
		case	dANI_DIRECT4		:	if (direct > d4DIRECT_S ) return d4DIRECT_W;
		case	dANI_DIRECT8		:	if (direct > d8DIRECT_S ) return direct-(direct-d8DIRECT_S)*2;
		case	dANI_DIRECT16		:	if (direct > d16DIRECT_S) return direct-(direct-d16DIRECT_S)*2;
	}

	return direct;
}

BOOL
cANM::isFlip(int anm,int direct)
{	if (_MAIN.workMode==dWORK_SET_SPRITE) return FALSE;
	if (!bHALF)	return FALSE;

	anm		=	getanm(anm);
	direct	=	getdirect(direct);

	if (direct > FlipDirect[ANM[anm].anmtype]) return TRUE;

	return FALSE;
}

void
cANM::SetCurrentAnm(int num)
{	CurFrame	=	0,CurDirect	=	0,CurAnm	=	num;
	if (AnmCount == ImageFolderCount)	CurrentImageFolder	=	CurAnm;

	SetCurrentFrame();
}

void
cANM::SetCurrentDirect(int num)
{	CurFrame	=	0,CurDirect	=	num;
	if (CurDirect>= GetMaxDirect()) CurDirect= 0;
	SetCurrentFrame();
}

void
cANM::SetAnmType(int type)
{	AnmType	=	type;

	for (int i=0;i<dMAX_ANM;i++) ANM[i].ClearData(type);

	bVIRGIN=FALSE;
}

void
cANM::SetAnmType(int type,int anm)
{	if (type != ANM[anm].anmtype) ANM[anm].ClearData(type);
}

//	데이터 저장..
BOOL
cANM::SaveAniSet(char *fn)
{	FILE	*fp;
	int		i;

	SetCurrentDirectory(_MAIN.curDir);
	if (!SetCurrentDirectory("anmset"))
	{	CreateDirectory("anmset",NULL);
		SetCurrentDirectory("anmset");
	}

	if (fn==NULL)	strcpy(FName,_ms("%s.asd",ANMName));
	else			strcpy(FName,fn);

	fp=fopen(FName,"rb");
	if (fp)
	{	if (cMSG::YESNO("Save animation settings data","A file with the same name as '%s' exists, do you want to overwrite it?",FName)==IDNO) return FALSE;
		fclose(fp);
	}

	fp=fopen(FName,"wb");

	if (!fp)
	{	fclose(fp);
		fp=fopen(FName,"wb");
		if (!fp)	return cMSG::Put("Error saving animation settings data!!!","'%s' is a foreign write error.",FName);
	}

	fwrite(regANISET	,40,1,fp);
	fwrite(ANMName		,40,1,fp);
	fwrite(&bHALF		,1,1,fp);
	fwrite(&bSAVEOUTLINE,1,1,fp);
	fwrite(&bSAVESHADOW	,1,1,fp);

	fwrite(&AnmType		,4,1,fp);
	fwrite(&AnmCount	,4,1,fp);

	for (i=0;i<AnmCount;i++)
	{	fwrite(ANM[i].Name				,40	,1,fp);
		fwrite(&ANM[i].anmtype			,4	,1,fp);
		fwrite(&ANM[i].directCount		,4	,1,fp);
		fwrite(&ANM[i].FrameCount		,4	,1,fp);

		fwrite(&ANM[i].bSaveReleasePoint,4	,1,fp);	//	릴리즈 포인트
		fwrite(&ANM[i].bSaveRefitPos	,4	,1,fp);	//	프레임 보정
	}

	fclose(fp);

	return TRUE;
}

BOOL
cANM::LoadAniSet(char *fn)
{	FILE	*fp;
	int		i;

	SetCurrentDirectory(_MAIN.curDir);
	if (!SetCurrentDirectory("anmset"))
	{	SetCurrentDirectory(_MAIN.curDir);
		return FALSE;
	}

	if (fn==NULL)
	{	if (cMSG::YESNO("Existing animation settings will be reset.","Are you sure you want to continue?")	!=	IDYES) 
			return FALSE;
	
		OPENFILENAME	ofn;

		memset(&ofn,0,sizeof(OPENFILENAME));
		ofn.lStructSize		=	sizeof(OPENFILENAME);
		ofn.hwndOwner		=	_MAIN.hWND;
		ofn.lpstrFilter = "Animation settings data {*.asd}\0*.asd\0\0";
		ofn.lpstrTitle = "Loading animation settings data.";
		ofn.lpstrFile		=	FName;
		ofn.nMaxFile		=	256;
		ofn.Flags			=	OFN_HIDEREADONLY | OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST;
		ofn.lpstrDefExt		=	"asd";
		ofn.lpstrDefExt		=	"asd";
		ofn.lpstrInitialDir	=	_ms("%s\\anmset",_MAIN.curDir);

		if (!GetOpenFileName(&ofn)) return FALSE;

	}	else strcpy(FName,fn);


	fp	=	fopen(FName,"rb");

	if (!fp)
	{	SetCurrentDirectory(_MAIN.curDir);
		return FALSE;
	}

	char reg[50];

	fread(reg,40,1,fp);

	if (strcmp(reg,regANISET)!=0)
	{	fclose(fp);
		return cMSG::Put("Animation settings data open error!!!","Japanese recognizer is different.",FName);
	}
	fread(ANMName		,40,1,fp);
	fread(&bHALF		,1,1,fp);
	fread(&bSAVEOUTLINE	,1,1,fp);
	fread(&bSAVESHADOW	,1,1,fp);

	fread(&AnmType		,4,1,fp);	//	에니메이션 타입
	SetAnmType(AnmType);			//	설정..
	fread(&AnmCount		,4,1,fp);

	for (i=0;i<AnmCount;i++)
	{	fread(ANM[i].Name			,40	,1,fp);
		fread(&ANM[i].anmtype		,4	,1,fp);
		fread(&ANM[i].directCount	,4	,1,fp);
		fread(&ANM[i].FrameCount	,4	,1,fp);

		fread(&ANM[i].bSaveReleasePoint	,4	,1,fp);	//	릴리즈 포인트
		fread(&ANM[i].bSaveRefitPos		,4	,1,fp);	//	프레임 보정
	}

	fclose(fp);

	SetCurrentDirectory(_MAIN.curDir);
	return TRUE;
}


BOOL
cANM::SaveAsFont(char *fname,int kind)
{	FILE *fp;

	int Size=0,i;

	fp=fopen(fname,"wb");

	if (!fp)
	{	fclose(fp);
		fp=fopen(fname,"wb");
		if (!fp)	return cMSG::Put("Error saving font color!!","Error writing color '%s'.",fname);
	}

	fwrite(&Size,4,1,fp);			//	화일 사이즈 넣을 자리..
	fwrite(&cIMG::Count,4,1,fp);	//	폰트 수..
	fwrite(&Size,4,1,fp);			//	첫번째 오프셋.. 0이다.

	cIMG *img=cIMG::pHEAD->pNEXT;

	for (i=0;i<cIMG::Count;i++)
	{	Size		+=	cTRANS::GetSTFSize(img->pSPRITE);
		img			=	img->pNEXT;
		fwrite(&Size,4,1,fp);		//	마지막 오프셋은 데이터 사이즈가 된다.
	}

	img=cIMG::pHEAD->pNEXT;

	for (i=0;i<cIMG::Count;i++)
	{
		Size		=	cTRANS::GetSTFSize(img->pSPRITE);
		BYTE *font	=	cTRANS::SpriteToFont(img->pSPRITE);
		img			=	img->pNEXT;

		fwrite(font,Size,1,fp);
		pKILL(font);
	}

	fclose(fp);

	return TRUE;
}

#define	dEXPORT_TEXT	1

BOOL
cANM::SaveSD(char *fn,BOOL _bIsSaveCodeSprite)
{
	FILE *fp,*tfp=NULL;
	int i;

#ifdef	dEXPORT_TEXT
	int		iBufferPoint=0,iBlockCount=0,iBufferSize=0;
	char	strFileName[512];

	if	(_bIsSaveCodeSprite)
	{
		char	strFolderName[512];

		strcpy(strFolderName,_exportFolderName(fn));
		sprintf(strFileName,"%s/_%s.cpp",strFolderName,_exportFileName(fn));

		tfp	=	fopen(strFileName,"wt");

		if	(!tfp)
		{
			cMSG::Error("에러!!","%s 파일이 잠겨 있습니다.(체크 아웃 안되어 있을 확률이 높습니다.",strFileName);

			return	FALSE;
		}

		fprintf(tfp,"#include	\"_%s.h\"\n",_exportFileName(fn));
		fprintf(tfp,"#include	<string.h>\n\n");

		fprintf(tfp,"int __g_aIRS_Offset[] = {\n");
	}
#endif

	fp=fopen(fn,"wb");

	if	(!fp)
	{
		fclose(fp);
		fp=fopen(fn,"wb");
		if (!fp)	return cMSG::Put("Sprite fade save error!!","'%s' fade write error.",fn);
	}

	fseek(fp,sizeof(SDHEADER),SEEK_SET);	//	화일 헤더부분.. 화일 사이즈 땜시 건너뜀..

	if (BPP==8)
		fwrite(PLT,512,1,fp);

	//	여기부터 스프라이트 데이터..
	cIMG *img=cIMG::pHEAD->pNEXT;

	int Size;

	Size=0;

	fwrite(&Size,sizeof(int),1,fp);
#ifdef	dEXPORT_TEXT
	if	(_bIsSaveCodeSprite)
		fprintf(tfp,_ms("%d,",Size));
#endif

	for (i=0;i<cIMG::Count;i++)
	{
		Size+=img->Size;
		fwrite(&Size,sizeof(int),1,fp);
#ifdef	dEXPORT_TEXT
		if	(_bIsSaveCodeSprite)
		{
			fprintf(tfp,_ms("%d,",Size));
			
			if (i%10 == 0)
				fprintf(tfp,"\n");
		}
#endif
		img=img->pNEXT;
	}

#ifdef	dEXPORT_TEXT
	if	(_bIsSaveCodeSprite)
		fprintf(tfp,_ms("};\n\nunsigned short __l_aIRSData001[] = {\n"));
#endif
	img=cIMG::pHEAD->pNEXT;

	//	스프라이트 데이타 저장..
	if (BPP==8)
		for (i=0;i<cIMG::Count;i++)
		{	fwrite(img->p8SPRITE,img->Size,1,fp);
			img=img->pNEXT;
		}

	if (BPP==16)
		for (i=0;i<cIMG::Count;i++)
		{	
			fwrite(img->pSPRITE,img->Size,sizeof(WORD),fp);

#ifdef	dEXPORT_TEXT
			if	(_bIsSaveCodeSprite)
				for (int s=0;s<img->Size;s++)
				{
					if (s%10 == 0)
						fprintf(tfp,"\n");

					fprintf(tfp,_ms("%d,",img->pSPRITE[s]));

					iBufferSize++;

					if	(iBufferSize	>=	1024*128)
					{
						iBlockCount++;
						iBufferSize	=	0;
						fprintf(tfp,_ms("};\n\nunsigned short __l_aIRSData%.3d[] = {\n",iBlockCount+1));
					}
				}
#endif

			img=img->pNEXT;
		}

#ifdef	dEXPORT_TEXT

	if	(_bIsSaveCodeSprite)
	{
		fprintf(tfp,"};\n\n");

		for	(i=iBlockCount+1;i<10;i++)
			fprintf(tfp,_ms("unsigned short __l_aIRSData%.3d[] = {0,};\n\n",i+1));

		fprintf(tfp,"\nint __l_iIRS_BlockCount = %d;",iBlockCount+1);
		fprintf(tfp,"\nint __l_iIRS_TotalSize	= %d;",Size);
		fprintf(tfp,"\nint __l_iIRS_BlockSize	= %d;",1024*128);
		fprintf(tfp,"\nint __g_iIRS_Count		= %d;",cIMG::Count);
		fprintf(tfp,"\n\nunsigned	short	*__g_pIRS_Data	=	NULL;");

		fprintf(tfp,"\n");
		fprintf(tfp,"\n");

		fprintf(tfp,"void\n");
		fprintf(tfp,"__UploadIRSData()\n");
		fprintf(tfp,"{\n");
		fprintf(tfp,"	__g_pIRS_Data		=	new unsigned short [__l_iIRS_TotalSize];\n");
		fprintf(tfp,"\n");
		fprintf(tfp,"	int	iBufferPoint	=	0;\n");
		fprintf(tfp,"	int	iCopyBlockSize	=	0;\n");
		fprintf(tfp,"\n");

		for	(i=0;i<10;i++)
		{
			fprintf(tfp,"	if	(__l_iIRS_TotalSize)\n");
			fprintf(tfp,"	{\n");
			fprintf(tfp,"		if	(__l_iIRS_TotalSize	>=	__l_iIRS_BlockSize)\n");
			fprintf(tfp,"			iCopyBlockSize	=	__l_iIRS_BlockSize;\n");
			fprintf(tfp,"		else\n");
			fprintf(tfp,"			iCopyBlockSize	=	__l_iIRS_TotalSize%%__l_iIRS_BlockSize;\n");
			fprintf(tfp,"\n");
			fprintf(tfp,"		memcpy(__g_pIRS_Data+iBufferPoint,__l_aIRSData%.3d,iCopyBlockSize*2);\n",i+1);
			fprintf(tfp,"		__l_iIRS_TotalSize		-=	iCopyBlockSize;\n");
			fprintf(tfp,"		iBufferPoint		+=	iCopyBlockSize;\n");

			fprintf(tfp,"	}\n");
		}

		fprintf(tfp,"}\n");
		fprintf(tfp,"\n");
		
		fprintf(tfp,"void\n");
		fprintf(tfp,"__DestroyIRS()\n");
		fprintf(tfp,"{\n");
		fprintf(tfp,"	if	(__g_pIRS_Data)\n");
		fprintf(tfp,"	{\n");
		fprintf(tfp,"		delete [] __g_pIRS_Data;\n");
		fprintf(tfp,"\n");
		fprintf(tfp,"		__g_pIRS_Data	=	NULL;\n");
		fprintf(tfp,"	}\n");
		fprintf(tfp,"}\n");

		fclose(tfp);

		sprintf(strFileName,"_%s.h",_exportFileName(fn));

		tfp	=	fopen(strFileName,"wt");

		fprintf(tfp,"#ifndef	_incode_resource_sprite_h\n");
		fprintf(tfp,"#define	_incode_resource_sprite_h\n");
		fprintf(tfp,"\n");
		fprintf(tfp,"extern	int					__g_aIRS_Offset[];\n");
		fprintf(tfp,"extern	unsigned	short	*__g_pIRS_Data;\n");
		fprintf(tfp,"extern	int					__g_iIRS_Count;\n");
		fprintf(tfp,"\n");
		fprintf(tfp,"extern	void				__UploadIRSData();\n");
		fprintf(tfp,"extern	void				__DestroyIRS();\n");

		fprintf(tfp,"\n");
		fprintf(tfp,"#endif\n");
		fclose(tfp);

		cMSG::Put("Done","Saved to file %s.",strFileName);
	}

#endif

//	그림자 데이타 저장..
	img	=	cIMG::pHEAD->pNEXT;

	if (bSAVESHADOW)
	{
		img	=	cIMG::pHEAD->pNEXT;

		Size	=	0;
		fwrite(&Size,sizeof(int),1,fp);

		for (i=0;i<cIMG::Count;i++)
		{
			Size	+=	img->sSize;
			fwrite(&Size,sizeof(int),1,fp);
			img		=	img->pNEXT;
		}

		img	=	cIMG::pHEAD->pNEXT;

		for (i=0;i<cIMG::Count;i++)
		{
			fwrite(img->pSHADOW,img->sSize,1,fp);
			img		=	img->pNEXT;
		}
	}

	img=cIMG::pHEAD->pNEXT;

	Size	=	0;
	fwrite(&Size,sizeof(int),1,fp);

	for (i=0;i<cIMG::Count;i++)
	{	Size+=img->lSize;
		fwrite(&Size,sizeof(int),1,fp);
		img=img->pNEXT;
	}

	img=cIMG::pHEAD->pNEXT;

	for (i=0;i<cIMG::Count;i++)
	{	fwrite(img->pLAYER,img->lSize,sizeof(BYTE),fp);
		img=img->pNEXT;
	}

	cIMG::GetMaxImage();

	strcpy(SDHEADER.Reg,regSDHEADER2);

	SDHEADER.Size				=	ftell(fp);
	SDHEADER.BPP				=	BPP;
	SDHEADER.ImageCount			=	cIMG::Count;
	SDHEADER.Alpha				=	_WORKWND.Alpha;
	SDHEADER.bOutline			=	bSAVEOUTLINE;
	SDHEADER.bShadow			=	bSAVESHADOW;

	SDHEADER.m_wMaxSpriteWidth	=	cIMG::s_iMaxSpriteWidth;
	SDHEADER.m_wMaxSpriteHeight	=	cIMG::s_iMaxSpriteHeight;
	SDHEADER.m_wMaxShadowWidth	=	cIMG::s_iMaxShadowWidth;
	SDHEADER.m_wMaxShadowHeight	=	cIMG::s_iMaxShadowHeight;

	fseek(fp,0L,SEEK_SET);
	fwrite(&SDHEADER,sizeof(SDHEADER),1,fp);
	fclose(fp);

	bVIRGIN	=	TRUE;

	return TRUE;
}

BOOL
cANM::LoadSD(char *fn)
{
	FILE *fp;
	int i;

	fp=fopen(fn,"rb");

	if	(!fp)
		return cMSG::Error("Error loading sprite file!!","'%s' file not found.",fn);

	fread(&SDHEADER,sizeof(SDHEADER),1,fp);

	BPP				=	SDHEADER.BPP;
	_WORKWND.Alpha	=	SDHEADER.Alpha;
	bSAVEOUTLINE	=	SDHEADER.bOutline;
	bSAVESHADOW		=	SDHEADER.bShadow;

	if	(strcmp(SDHEADER.Reg,regSDHEADER)	!=0	&&	strcmp(SDHEADER.Reg,regSDHEADER2))
		return cMSG::Put("Sprite firing error!!","The recognizer for '%s' does not match.",fn);

	cIMG::Reset();

	if	(SDHEADER.BPP==8)
		fread(&PLT,512,1,fp);

	//	여기부터 스프라이트 데이터..
	int *Size;

	Size	=	new int [SDHEADER.ImageCount+1];
	fread(Size,sizeof(int),SDHEADER.ImageCount+1,fp);

	KILL(imageFOLDERS);

	imageFOLDERS	=	new cSTRINGS();

	imageFOLDERS->Add("추가된 이미지");

	ImageFolder[0]		=	0;
	ImageFolder[1]		=	SDHEADER.ImageCount;
	ImageFolder[2]		=	0;
	ImageFolder[3]		=	SDHEADER.ImageCount;
	ImageFolderCount	=	1;
	CurrentImageFolder	=	0;

	if	(SDHEADER.BPP==8)
		for (i=0;i<SDHEADER.ImageCount;i++)
		{	
			BYTE *spr;

			spr	=	new BYTE[Size[i+1]-Size[i]];

			fread(spr,Size[i+1]-Size[i],1,fp);

			cIMG::Add(spr,Size[i+1]-Size[i]);
		}

	if (SDHEADER.BPP==16)
		for (i=0;i<SDHEADER.ImageCount;i++)
		{	WORD	*spr;

			spr	=	new WORD[Size[i+1]-Size[i]];

			fread(spr,Size[i+1]-Size[i],sizeof(WORD),fp);

			cIMG::Add(spr,Size[i+1]-Size[i]);
		}


	cIMG *img;

	if (bSAVESHADOW)
	{	img	=	cIMG::pHEAD->pNEXT;

		fread(Size,sizeof(int),cIMG::Count+1,fp);

		for (i=0;i<cIMG::Count;i++)
		{	BYTE *pshadow	=	NULL;

			int size	=	Size[i+1]-Size[i];

			if (size>0)
			{	pshadow =	new BYTE [size];
				fread(pshadow,sizeof(BYTE),size,fp);
			}

			img->SetShadow(pshadow,size);
			img	=	img->pNEXT;
		}
	}

	img	=	cIMG::pHEAD->pNEXT;

	fread(Size,sizeof(int),cIMG::Count+1,fp);

	for (i=0;i<cIMG::Count;i++)
	{	BYTE	*lspr;

		lspr =	new BYTE [Size[i+1]-Size[i]];
		fread(lspr,sizeof(BYTE),Size[i+1]-Size[i],fp);

		img->SetLayer(lspr,Size[i+1]-Size[i]);
		img=img->pNEXT;
	}

	fclose(fp);

	bVIRGIN	=	TRUE;

	pKILL(Size);

	cIMG::SetCURRENT(0);

	return TRUE;
}

BOOL
cANM::SaveSAD(char *fn,BOOL bInfoOut)
{
	FILE *fp;
	int i,j;

	cANM::CorrectEventFrame();

	fp	=	fopen(fn,"wb");

	if (!fp)
	{
		fclose(fp);
		fp=fopen(fn,"wb");

		if	(!fp)
			return cMSG::Put("Sprite animation fade save error!!","'%s' fade write error.",fn);
	}

	fseek(fp,sizeof(SADHEADER),SEEK_SET);	//	화일 헤더부분.. 화일 사이즈 땜시 건너뜀..

	if	(BPP==8)
		fwrite(PLT,512,1,fp);

	//	여기부터 스프라이트 데이터..
	cIMG *img=cIMG::pHEAD->pNEXT;

	int Size;

	Size=0;

	fwrite(&Size,sizeof(int),1,fp);

	for (i=0;i<cIMG::Count;i++)
	{
		Size+=	img->Size;
		fwrite(&Size,sizeof(int),1,fp);
		img	=	img->pNEXT;
	}

	img=cIMG::pHEAD->pNEXT;

	//	스프라이트 데이타 저장..
	if (BPP==8)
		for (i=0;i<cIMG::Count;i++)
		{	fwrite(img->p8SPRITE,img->Size,1,fp);
			img	=	img->pNEXT;
		}

	if (BPP==16)
	{
		for (i=0;i<cIMG::Count;i++)
		{	fwrite(img->pSPRITE,img->Size,sizeof(WORD),fp);
			img=img->pNEXT;
		}
		Size*=2;
	}

	int	imageSize	=	ftell(fp);

	if	(bInfoOut)
		MSGOUT("%s File's Image  Size : %-6.4f kilo byte",fn,(double)((double)Size/1024.0));

	DWORD TotalSize	=	Size;

	if (BPP==16)	TotalSize*=2;

	//	그림자 데이타 저장..

	if (bSAVESHADOW)
	{	img	=	cIMG::pHEAD->pNEXT;

		Size	=	0;
		fwrite(&Size,sizeof(int),1,fp);

		for (i=0;i<cIMG::Count;i++)
		{	Size	+=	img->sSize;
			fwrite(&Size,sizeof(int),1,fp);
			img		=	img->pNEXT;
		}

		img	=	cIMG::pHEAD->pNEXT;

		for (i=0;i<cIMG::Count;i++)
		{	fwrite(img->pSHADOW,img->sSize,1,fp);
			img		=	img->pNEXT;
		}

		TotalSize	+=	Size;
		if (bInfoOut)	MSGOUT("%s File's Shadow Size : %-6.4f kilo byte",fn,(double)((double)Size/1024.0));
	}

	img		=	cIMG::pHEAD->pNEXT;
	Size	=	0;

	fwrite(&Size,sizeof(int),1,fp);

	for (i=0;i<cIMG::Count;i++)
	{
		Size+=	img->lSize;
		fwrite(&Size,sizeof(int),1,fp);
		img	=	img->pNEXT;
	}

	img=cIMG::pHEAD->pNEXT;

	for (i=0;i<cIMG::Count;i++)
	{
		fwrite(img->pLAYER,img->lSize,1,fp);
		img	=	img->pNEXT;
	}

	TotalSize	+=	Size;

	if	(bInfoOut)
		MSGOUT("%s File's Layer  Size : %-6.4f kilo byte",fn,(double)((double)Size/1024.0));
	if	(bInfoOut)
		MSGOUT("%s File's Total  Size : %-6.4f kilo byte",fn,(double)((double)TotalSize/1024.0));
	//	에니메이션 데이터..

	fwrite(&MoveOval,4	,1,fp);
	fwrite(&CRASHBOX,16	,1,fp);
	fwrite(&g_rectSelect,16	,1,fp);
	fwrite(&g_wAnmDataType	,2,1,fp);		//	ANM 파일 타입
	fwrite(&g_wPartsType	,2,1,fp);		//	파츠 타입
	
	cPOINT	tempPos;
	tempPos.x	=	ANM[0].RefitPos[0].x;
	tempPos.y	=	ANM[0].RefitPos[0].y;
	fwrite(&tempPos		,1	,sizeof(cPOINT)	,fp);	//	전체 보정

	fwrite(&g_wCrashSize,2	,1				,fp);		//	데이터 타입
	fwrite(&g_wPlayerJob,2	,1				,fp);		//	캐릭터 직업

//	추가됨
	fwrite(&s_iDefaultAttack,4	,1	,fp);			//	기본 공격
//	fwrite(&s_iDefaultMagic	,4	,1	,fp);			//	기본 마법
	{
		WORD	wDefaultMagic	=	s_iDefaultMagic;
		fwrite(&wDefaultMagic				,2	,1	,fp);//	기본 마법
		fwrite(&s_wIsOccasionallyRestAction	,2	,1	,fp);//	계속 쉬는 액션을 취하는 에니메이션
	}

//	fwrite(&ANM[0].RefitPos[0]	,1	,sizeof(POINTS),fp);	//	전체 보정

	for (i=0;i<AnmCount;i++)			//	에니
	{
		fwrite(&ANM[i].FrameCount		,sizeof(int),1,fp);
	
		if (ANM[i].FrameCount > 0)
		{	int alpha	=	ANM[i].ALPHA;

			fwrite(&ANM[i].anmtype			,4	,1,fp);
			fwrite(&ANM[i].directCount		,4	,1,fp);
	
			fwrite(&alpha					,4	,1,fp);
			fwrite(&ANM[i].FPS				,4	,1,fp);
			fwrite(&ANM[i].PPS				,4	,1,fp);

			fwrite(&ANM[i].bSaveReleasePoint,4	,1,fp);	//	릴리즈 포인트
			fwrite(&ANM[i].bSaveRefitPos	,4	,1,fp);	//	프레임 보정
			fwrite(&ANM[i].LinkAnm			,4	,1,fp);	//	링크 에니메이션

//			fwrite(ANM[i].RefitPos			,ANM[i].directCount	,sizeof(POINTS),fp);	//	전체 보정

			fwrite(&ANM[i].isEvent,ANM[i].FrameCount,1,fp);

			for (j=0;j<ANM[i].directCount;j++)
			{
				for (int iFrame=0;iFrame<ANM[i].FrameCount;iFrame++)
				{
					if (ANM[i].Sprite[j][iFrame]	>=	cIMG::Count)
						ANM[i].Sprite[j][iFrame]	=	0;
				}

				fwrite(&ANM[i].Sprite[j][0]		,ANM[i].FrameCount*2,1,fp);
			}
			for (j=0;j<ANM[i].directCount;j++)	fwrite(&ANM[i].BackFrame[j][0]	,ANM[i].FrameCount,1,fp);

			if (ANM[i].bSaveReleasePoint)
				for (j=0;j<ANM[i].directCount;j++)
					fwrite(&ANM[i].Release[j]	,4,1,fp);

			if (ANM[i].bSaveRefitPos)
				for (j=0;j<ANM[i].directCount;j++)
					fwrite(&ANM[i].Pos[j][0]	,4*ANM[i].FrameCount,1,fp);
		}
	}

	//	에디터에서 쓸 데이터..

	fwrite(ANMName		,40,1,fp);
	fwrite(&bHALF		,1,1,fp);
	fwrite(&bSAVEOUTLINE,1,1,fp);
	fwrite(&bSAVESHADOW	,1,1,fp);

	fwrite(&AnmType		,4,1,fp);
	fwrite(&AnmCount	,4,1,fp);

	for (i=0;i<AnmCount;i++)
	{
		fwrite(ANM[i].Name			,40	,1,fp);
		fwrite(&ANM[i].anmtype		,4	,1,fp);
		fwrite(&ANM[i].directCount	,4	,1,fp);
		fwrite(&ANM[i].FrameCount	,4	,1,fp);
		fwrite(&ANM[i].bSaveReleasePoint,4	,1,fp);	//	릴리즈 포인트
		fwrite(&ANM[i].bSaveRefitPos	,4	,1,fp);	//	프레임 보정
	}

	fwrite(&ImageFolderCount,4,1,fp);
	fwrite(&ImageFolder,4*(ImageFolderCount+2),1,fp);

	for (i=0;i<ImageFolderCount;i++)
	{	char folderName[64];
		if (strlen(imageFOLDERS->GetStr(i))>=63)
		{	memcpy(folderName,imageFOLDERS->GetStr(i),63);
			folderName[63]	=	0;
		}
		else	strcpy(folderName,imageFOLDERS->GetStr(i));

		fwrite(folderName,64,1,fp);
	}

	//	헤더 저장..
	cIMG::GetMaxImage();

	SADHEADER.Size		=	ftell(fp);
	strcpy(SADHEADER.Reg,regSADHEADER2);
	SADHEADER.AnmCount	=	AnmCount;
	SADHEADER.ImageCount=	cIMG::Count;
	SADHEADER.AnmType	=	AnmType;
	SADHEADER.Version	=	l_iVersion;
	SADHEADER.bOutline	=	bSAVEOUTLINE;
	SADHEADER.bShadow	=	bSAVESHADOW;
	SADHEADER.bHalf		=	bHALF;
	SADHEADER.BPP		=	BPP;

	SADHEADER.m_wMaxSpriteWidth		=	cIMG::s_iMaxSpriteWidth;
	SADHEADER.m_wMaxSpriteHeight	=	cIMG::s_iMaxSpriteHeight;
	SADHEADER.m_wMaxShadowWidth		=	cIMG::s_iMaxShadowWidth;
	SADHEADER.m_wMaxShadowHeight	=	cIMG::s_iMaxShadowHeight;

	fseek(fp,0L,SEEK_SET);
	fwrite(&SADHEADER,sizeof(SDHEADER),1,fp);
	fclose(fp);

	bVIRGIN	=	TRUE;

	return TRUE;
}

BOOL
cANM::LoadSAD(char *fn,BOOL _is , HWND _hWnd)
{	
	FILE *fp;

	int i,j;

	fp	=	fopen(fn,"rb");

	if	(!fp)
		return cMSG::Put("Sprite animation file open error!!","File '%s' could not be found.",fn);

	DWORD	check;

	fseek(fp,50L,SEEK_SET);
	fread(&check,4,1,fp);
	fseek(fp,0L,SEEK_SET);

	fread(&SADHEADER,sizeof(SADHEADER),1,fp);
	if	(strcmp(SADHEADER.Reg,regSADHEADER)	!=	0 && strcmp(SADHEADER.Reg,regSADHEADER2)!=0)
		return cMSG::Put("Error loading sprite animation file!!","The recognizer for '%s' does not match.",fn);

	AnmCount	=	SADHEADER.AnmCount;
	AnmType		=	SADHEADER.AnmType;
	bSAVEOUTLINE=	SADHEADER.bOutline;
	bSAVESHADOW	=	SADHEADER.bShadow;
	bHALF		=	SADHEADER.bHalf;
	BPP			=	SADHEADER.BPP;

	if	(SADHEADER.BPP==8)
		fread(PLT,256,2,fp);

	//	여기부터 스프라이트 데이터..
	cIMG::Reset();
	int *Size;

	Size	=	new int [SADHEADER.ImageCount+1];
	fread(Size,sizeof(int),SADHEADER.ImageCount+1,fp);

	if (SADHEADER.BPP==8)
		for (i=0;i<SADHEADER.ImageCount;i++)
		{	BYTE *spr;

			spr	=	new BYTE[Size[i+1]-Size[i]];

			fread(spr,Size[i+1]-Size[i],1,fp);
			cIMG::Add(spr,Size[i+1]-Size[i]);
		}

	if (SADHEADER.BPP==16)
		for (i=0;i<SADHEADER.ImageCount;i++)
		{	WORD	*spr;

			spr	=	new WORD[Size[i+1]-Size[i]];

			fread(spr,Size[i+1]-Size[i],sizeof(WORD),fp);
			cIMG::Add(spr,Size[i+1]-Size[i]);
		}

	cIMG *img;

	if (bSAVESHADOW)
	{	img	=	cIMG::pHEAD->pNEXT;

		fread(Size,sizeof(int),SADHEADER.ImageCount+1,fp);

		for (i=0;i<cIMG::Count;i++)
		{	BYTE *pshadow	=	NULL;

			int size	=	Size[i+1]-Size[i];

			if (size>0)
			{	pshadow =	new BYTE [size];
				fread(pshadow,sizeof(BYTE),size,fp);
			}

			img->SetShadow(pshadow,size);
			img	=	img->pNEXT;
		}
	}

	img	=	cIMG::pHEAD->pNEXT;

	fread(Size,sizeof(int),SADHEADER.ImageCount+1,fp);

	for (i=0;i<cIMG::Count;i++)
	{	BYTE	*lspr	=	NULL;

		int size	=	Size[i+1]-Size[i];

		if (size>0)
		{	lspr =	new BYTE	[size];
			fread(lspr,sizeof(BYTE),size,fp);
		}

		img->SetLayer(lspr,Size[i+1]-Size[i]);
		img=img->pNEXT;
	}

	//	에니메이션 데이터..

	SetAnmType(AnmType);

	fread(&MoveOval,4	,1,fp);
	fread(&CRASHBOX,16	,1,fp);

	if (SADHEADER.Version	>=	101		)	fread(&g_rectSelect,16	,1,fp);

	fread(&g_wAnmDataType	,2,1,fp);		//	ANM 파일 타입
	fread(&g_wPartsType		,2,1,fp);		//	파츠 타입

//	fread(&ANM[0].RefitPos[0]	,1	,sizeof(POINTS),fp);	//	전체 보정
	cPOINT	tempPos;
	fread(&tempPos	,1	,sizeof(cPOINT),fp);	//	전체 보정
	ANM[0].RefitPos[0].x	=	tempPos.x;
	ANM[0].RefitPos[0].y	=	tempPos.y;

	fread(&g_wCrashSize,2,1,fp);		//	데이터 타입
	fread(&g_wPlayerJob,2,1,fp);		//	데이터 타입

	if	(g_wPlayerJob)	//	코드 개판이다.. - -
		_MAIN.loadPlayerLayerData();

//	추가됨
	if (SADHEADER.Version	>=	100	&&	SADHEADER.Version	<=	l_iVersion)
	{
		WORD	wDefaultMagic;

		fread(&s_iDefaultAttack	,4	,1	,fp);			//	기본 공격
//		fread(&s_iDefaultMagic	,4	,1	,fp);			//	기본 마법
		fread(&wDefaultMagic	,2	,1	,fp);			//	기본 마법
		fread(&s_wIsOccasionallyRestAction,2	,1	,fp);//	계속 쉬는 액션을 취하는 에니메이션

		s_iDefaultMagic	=	wDefaultMagic;
//		fread(&wDefaultMagic	,2	,1	,fp);			//	기본 마법
	}
	else
	{
		s_iDefaultAttack	=	0xffff;					//	기본 공격
		s_iDefaultMagic		=	0xffff;					//	기본 마법
	}

	for (i=0;i<AnmCount;i++)			//	에니
	{	fread(&ANM[i].FrameCount		,sizeof(int),1,fp);
		ANM[i].anmtype		=	0;

		if (ANM[i].FrameCount > 0)
		{	int alpha	=	ANM[i].ALPHA;

			fread(&ANM[i].anmtype			,4	,1,fp);
			fread(&ANM[i].directCount		,4	,1,fp);
	
			fread(&ANM[i].ALPHA				,4	,1,fp);
			fread(&ANM[i].FPS				,4	,1,fp);
			fread(&ANM[i].PPS				,4	,1,fp);

			fread(&ANM[i].bSaveReleasePoint	,4	,1,fp);	//	릴리즈 포인트
			fread(&ANM[i].bSaveRefitPos		,4	,1,fp);	//	프레임 보정
			fread(&ANM[i].LinkAnm			,4	,1,fp);	//	링크 에니메이션
			if (ANM[i].LinkAnm >= AnmCount)	ANM[i].LinkAnm = 0xffff;

//			fread(ANM[i].RefitPos			,ANM[i].directCount	*sizeof(POINTS),1,fp);	//	전체 보정
			fread(&ANM[i].isEvent			,ANM[i].FrameCount,1,fp);

			for (j=0;j<ANM[i].directCount;j++)	fread(&ANM[i].Sprite[j][0]		,ANM[i].FrameCount*2,1,fp);
			for (j=0;j<ANM[i].directCount;j++)	fread(&ANM[i].BackFrame[j][0]	,ANM[i].FrameCount	,1,fp);

			if (ANM[i].bSaveReleasePoint)	for (j=0;j<ANM[i].directCount;j++)	fread(&ANM[i].Release[j],4	,1,fp);
			if (ANM[i].bSaveRefitPos)		for (j=0;j<ANM[i].directCount;j++)	fread(&ANM[i].Pos[j][0]	,4*ANM[i].FrameCount,1,fp);
		}

	}


	//	충돌 박스..


	//	에디터에서 쓸 데이터..

	fread(ANMName		,40,1,fp);
	fread(&bHALF		,1,1,fp);
	fread(&bSAVEOUTLINE	,1,1,fp);
	fread(&bSAVESHADOW	,1,1,fp);

	fread(&AnmType		,4,1,fp);
	fread(&AnmCount		,4,1,fp);

	for (i=0;i<AnmCount;i++)
	{
		fread(ANM[i].Name				,40	,1,fp);
		fread(&ANM[i].anmtype			,4	,1,fp);
		fread(&ANM[i].directCount		,4	,1,fp);
		fread(&ANM[i].FrameCount		,4	,1,fp);
		fread(&ANM[i].bSaveReleasePoint	,4	,1,fp);	//	릴리즈 포인트
		fread(&ANM[i].bSaveRefitPos		,4	,1,fp);	//	프레임 보정
	}

	fread(&ImageFolderCount,4,1,fp);
	fread(&ImageFolder,4*(ImageFolderCount+2),1,fp);

	KILL(imageFOLDERS);

	imageFOLDERS	=	new cSTRINGS();

	for (i=0;i<ImageFolderCount;i++)
	{
		char folderName[64];
		fread(folderName,64,1,fp);
		imageFOLDERS->Add(folderName);
	}

	fclose(fp);

	bVIRGIN		=	TRUE;

	CurAnm		=	0;
	CurFrame	=	0;
	CurDirect	=	0;

	if	(_MAIN.workMode==dWORK_SET_FRAME)
		SetCurrentFrame();
	else
		cIMG::SetCURRENT(0);

	cANM::CorrectEventFrame();
	
	SetWindowText( _hWnd, fn);

	return TRUE;
}

BOOL
cANM::Import(char *fn,BOOL _isOperateSitDown)
{	
	FILE *fp;
	int i,j;

	fp	=	fopen(fn,"rb");

	if (!fp)	return cMSG::Put("Sprite animation file open error!!","File '%s' could not be found.",fn);

	DWORD	check;

	fseek(fp,50L,SEEK_SET);
	fread(&check,4,1,fp);
	fseek(fp,0L,SEEK_SET);

	fread(&SADHEADER,sizeof(SADHEADER),1,fp);
	if (strcmp(SADHEADER.Reg,regSADHEADER)!=0 && strcmp(SADHEADER.Reg,regSADHEADER2)!=0)
		return cMSG::Put("Error loading sprite animation file!!","The recognizer for '%s' does not match.",fn);

	AnmCount	=	SADHEADER.AnmCount;
	AnmType		=	SADHEADER.AnmType;
	bSAVEOUTLINE=	SADHEADER.bOutline;
	bSAVESHADOW	=	SADHEADER.bShadow;
	bHALF		=	SADHEADER.bHalf;
	BPP			=	SADHEADER.BPP;

	if (SADHEADER.BPP==8)	fseek(fp,256*2,SEEK_CUR);

	//	여기부터 스프라이트 데이터..
	int *Size;

	Size	=	new int [SADHEADER.ImageCount+1];
	fread(Size,sizeof(int),SADHEADER.ImageCount+1,fp);

	if (SADHEADER.BPP==8)
		fseek(fp,Size[SADHEADER.ImageCount],SEEK_CUR);

	if (SADHEADER.BPP==16)
		fseek(fp,Size[SADHEADER.ImageCount]*2,SEEK_CUR);

	if (bSAVESHADOW)
	{	fread(Size,sizeof(int),SADHEADER.ImageCount+1,fp);

		fseek(fp,Size[SADHEADER.ImageCount],SEEK_CUR);
	}

	fread(Size,sizeof(int),SADHEADER.ImageCount+1,fp);
	fseek(fp,Size[SADHEADER.ImageCount],SEEK_CUR);


	//	에니메이션 데이터..

	if (!_isOperateSitDown)	SetAnmType(AnmType);

	fread(&MoveOval,4	,1,fp);
	fread(&CRASHBOX,16	,1,fp);

	if (SADHEADER.Version	>=	101		)
		fread(&g_rectSelect,16	,1,fp);

	fread(&g_wAnmDataType	,2,1,fp);		//	ANM 파일 타입
	fread(&g_wPartsType		,2,1,fp);		//	파츠 타입

//	fread(&ANM[0].RefitPos[0]	,sizeof(POINTS),1,fp);	//	전체 보정
	cPOINT	tempPos;
	fread(&tempPos	,1	,sizeof(cPOINT),fp);	//	전체 보정
	ANM[0].RefitPos[0].x	=	tempPos.x;
	ANM[0].RefitPos[0].y	=	tempPos.y;
	fread(&g_wCrashSize,2,1,fp);		//	데이터 타입
	fread(&g_wPlayerJob,2,1,fp);		//	데이터 타입

	fread(&s_iDefaultAttack	,4,1,fp);		//	데이터 타입
//	fread(&s_iDefaultMagic	,4	,1	,fp);			//	기본 마법

	{
		WORD	wDefaultMagic;
		fread(&wDefaultMagic	,2	,1	,fp);			//	기본 마법
		fread(&s_wIsOccasionallyRestAction,2	,1	,fp);//	계속 쉬는 액션을 취하는 에니메이션
		s_iDefaultMagic	=	wDefaultMagic;
	}

	AnmCount	+=	_isOperateSitDown;

	for (i=0;i<AnmCount;i++)			//	에니
	{
		if (_isOperateSitDown)	
			if (i==7)
			{
				ANM[i].anmtype			=	dANI_DIRECT8;
				ANM[i].directCount		=	8;
				ANM[i].FPS				=	16;
				ANM[i].PPS				=	0;
				ANM[i].bSaveReleasePoint=	FALSE;	//	릴리즈 포인트
				ANM[i].bSaveRefitPos	=	FALSE;	//	프레임 보정
				ANM[i].LinkAnm			=	0xffff;	//	링크 에니메이션
				memset(ANM[i].isEvent,0,sizeof(ANM[i].isEvent));

				for (j=0;j<ANM[i].directCount;j++)	ANM[i].Sprite[j][0]		=	0;
				for (j=0;j<ANM[i].directCount;j++)	ANM[i].BackFrame[j][0]	=	FALSE;

				i++;
			}
		
		fread(&ANM[i].FrameCount		,sizeof(int),1,fp);

		if (ANM[i].FrameCount > 0)
		{	int alpha	=	ANM[i].ALPHA;

			fread(&ANM[i].anmtype			,4	,1,fp);
			fread(&ANM[i].directCount		,4	,1,fp);
	
			fread(&ANM[i].ALPHA				,4	,1,fp);
			fread(&ANM[i].FPS				,4	,1,fp);
			fread(&ANM[i].PPS				,4	,1,fp);

			fread(&ANM[i].bSaveReleasePoint	,4	,1,fp);	//	릴리즈 포인트
			fread(&ANM[i].bSaveRefitPos		,4	,1,fp);	//	프레임 보정
			fread(&ANM[i].LinkAnm			,4	,1,fp);	//	링크 에니메이션

			if (ANM[i].LinkAnm >= AnmCount)	ANM[i].LinkAnm = 0xffff;

			fread(&ANM[i].isEvent			,ANM[i].FrameCount,1,fp);

			for (j=0;j<ANM[i].directCount;j++)	
			{	fread(&ANM[i].Sprite[j][0]		,ANM[i].FrameCount*2,1,fp);

				if (_isOperateSitDown	&&	i>7)	
					for (int k=0;k<ANM[i].FrameCount;k++)	ANM[i].Sprite[j][k]	+=	8;
			}
			for (j=0;j<ANM[i].directCount;j++)	fread(&ANM[i].BackFrame[j][0]	,ANM[i].FrameCount	,1,fp);

			if (ANM[i].bSaveReleasePoint)	for (j=0;j<ANM[i].directCount;j++)	fread(&ANM[i].Release[j],4	,1,fp);
			if (ANM[i].bSaveRefitPos	)	for (j=0;j<ANM[i].directCount;j++)	fread(&ANM[i].Pos[j][0]	,4*ANM[i].FrameCount,1,fp);
		}
	}

	//	충돌 박스..

	//	에디터에서 쓸 데이터..

	if (!_isOperateSitDown)
	{
		fread(ANMName		,40,1,fp);
		fread(&bHALF		,1,1,fp);
		fread(&bSAVEOUTLINE	,1,1,fp);
		fread(&bSAVESHADOW	,1,1,fp);

		fread(&AnmType		,4,1,fp);
		fread(&AnmCount		,4,1,fp);

		for (i=0;i<AnmCount;i++)
		{	fread(ANM[i].Name				,40	,1,fp);
			fread(&ANM[i].anmtype			,4	,1,fp);
			fread(&ANM[i].directCount		,4	,1,fp);
			fread(&ANM[i].FrameCount		,4	,1,fp);
			fread(&ANM[i].bSaveReleasePoint	,4	,1,fp);	//	릴리즈 포인트
			fread(&ANM[i].bSaveRefitPos		,4	,1,fp);	//	프레임 보정
		}

		if (check == 0xfefefefe)	//	이미지 폴더 정보
		{	fread(&ImageFolderCount,4,1,fp);
			fread(&ImageFolder,4*(ImageFolderCount+2),1,fp);
			KILL(imageFOLDERS);

			imageFOLDERS	=	new cSTRINGS();

			for (i=0;i<ImageFolderCount;i++)
			{	char folderName[64];
				fread(folderName,64,1,fp);
				imageFOLDERS->Add(folderName);
			}

			fread(&g_wAnmDataType	,2,1,fp);		//	ANM 파일 타입
			fread(&g_wPartsType		,2,1,fp);		//	파츠 타입
		}
	}

	fclose(fp);

	bVIRGIN		=	FALSE;

	CurAnm		=	0;
	CurFrame	=	0;
	CurDirect	=	0;

	if (_MAIN.workMode==dWORK_SET_FRAME)	SetCurrentFrame();
	else									cIMG::SetCURRENT(0);

	return TRUE;
}

void
cANM::Reset()
{	cIMG::Reset();

	for (int i=0;i<dMAX_ANM;i++)	ANM[i].ClearData(0xffff);
	CurAnm		=	0;
	CurFrame	=	0;
	CurDirect	=	0;

	s_iDefaultAttack			=	0xffff;
	s_iDefaultMagic				=	0xffff;
	s_wIsOccasionallyRestAction	=	FALSE;
}


void
cANM::SetFront(int frame)
{	ANM[CurAnm].BackFrame[CurDirect][frame]	=	0;
	bVIRGIN	=	FALSE;
}

void
cANM::SetBack(int frame)
{	ANM[CurAnm].BackFrame[CurDirect][frame]	=	1-ANM[CurAnm].BackFrame[CurDirect][frame];
	bVIRGIN	=	FALSE;
}

BOOL
cANM::IsBack(int frame)
{	return ANM[CurAnm].BackFrame[CurDirect][frame];
}

//
//	데미지 프레임 설정
void
cANM::DamageFrame(int frame)
{
	if (ANM[CurAnm].isEvent[frame]&dEVENT_DAMAGE)	ANM[CurAnm].isEvent[frame]	-=	dEVENT_DAMAGE;
	else											ANM[CurAnm].isEvent[frame]	+=	dEVENT_DAMAGE;
	bVIRGIN	=	FALSE;

	if (ANM[CurAnm].isEvent[frame]&dEVENT_DAMAGE)
	{
		if (frame	>=	ANM[CurAnm].FrameCount-1)
		{
			ANM[CurAnm].isEvent[frame]	-=	dEVENT_DAMAGE;

			cMSG::Error("Damage frame set error!!","Damage frame cannot be set for the last frame!");
		}
		else
		{
			if (ANM[CurAnm].isEvent[frame+1]&dEVENT_DAMAGE)
			{
				cMSG::Error("Damage frame set error!!!","Damage frame cannot be set consecutively!");

				ANM[CurAnm].isEvent[frame]	-=	dEVENT_DAMAGE;
			}
			if (frame	>	0)
			{
				if (ANM[CurAnm].isEvent[frame-1]&dEVENT_DAMAGE)
				{
					cMSG::Error("Damage frame set error!!","Damage frames cannot be set consecutively!");

					ANM[CurAnm].isEvent[frame]	-=	dEVENT_DAMAGE;
				}
			}
		}
	}
}	//	cANM::DamageFrame(int frame)

//
//	트리거 프레임 설정
void
cANM::TriggerFrame(int frame)
{
	if (ANM[CurAnm].isEvent[frame]&dEVENT_TRIGGER)	ANM[CurAnm].isEvent[frame]	-=	dEVENT_TRIGGER;
	else											ANM[CurAnm].isEvent[frame]	+=	dEVENT_TRIGGER;

	bVIRGIN	=	FALSE;

	if (ANM[CurAnm].isEvent[frame]&dEVENT_TRIGGER)
	{
		if (frame	>=	ANM[CurAnm].FrameCount-1)
		{
			ANM[CurAnm].isEvent[frame]	-=	dEVENT_TRIGGER;

			cMSG::Error("Error setting trigger frame!!","Trigger frame cannot be set for the last frame!");
		}
		else
		{
			if (ANM[CurAnm].isEvent[frame+1]&dEVENT_TRIGGER)
			{
				cMSG::Error("Error setting trigger frame!!!","Trigger frame cannot be set consecutively!");

				ANM[CurAnm].isEvent[frame]	-=	dEVENT_TRIGGER;
			}
			if (frame	>	0)
			{
				if (ANM[CurAnm].isEvent[frame-1]&dEVENT_TRIGGER)
				{
					cMSG::Error("Error setting trigger frame!!","Trigger frame cannot be set consecutively!");

					ANM[CurAnm].isEvent[frame]	-=	dEVENT_TRIGGER;
				}
			}
		}
	}
}	//	cANM::TriggerFrame(int frame)

//
//	스텝 프레임 설정
void
cANM::StepFrame(int frame)
{
	if (ANM[CurAnm].isEvent[frame]&dEVENT_STEP)	ANM[CurAnm].isEvent[frame]	-=	dEVENT_STEP;
	else										ANM[CurAnm].isEvent[frame]	+=	dEVENT_STEP;

	bVIRGIN	=	FALSE;
}	//	cANM::StepFrame(int frame)

//
//	이벤트 프레임들의 잘못된 부분을 교정한다.
void
cANM::CorrectEventFrame()
{
	for (int iAnm=0;iAnm<AnmCount;iAnm++)
	{
		cANM	*lpAnm	=	&ANM[iAnm];

		int	iFrameCount	=	lpAnm->FrameCount;

		if (lpAnm->isEvent[iFrameCount-1]&dEVENT_DAMAGE	)
		{
			lpAnm->isEvent[iFrameCount-1]	-=	dEVENT_DAMAGE;
			bVIRGIN	=	FALSE;
		}
		if (lpAnm->isEvent[iFrameCount-1]&dEVENT_TRIGGER)
		{
			lpAnm->isEvent[iFrameCount-1]	-=	dEVENT_TRIGGER;
			bVIRGIN	=	FALSE;
		}

		for (int iFrame=0;iFrame<iFrameCount-1;iFrame++)
		{
			if (lpAnm->isEvent[iFrame]&dEVENT_DAMAGE	&&	lpAnm->isEvent[iFrame+1]&dEVENT_DAMAGE	)
			{
				lpAnm->isEvent[iFrame]	-=	dEVENT_DAMAGE;
				bVIRGIN	=	FALSE;
			}
			if (lpAnm->isEvent[iFrame]&dEVENT_TRIGGER	&&	lpAnm->isEvent[iFrame+1]&dEVENT_TRIGGER	)
			{
				lpAnm->isEvent[iFrame]	-=	dEVENT_TRIGGER;
				bVIRGIN	=	FALSE;
			}
		}
	}
}	//	cANM::CorrectEventFrame()

void
cANM::LoopStartFrame(int frame)
{
	if (ANM[CurAnm].isEvent[frame] & dEVENT_LOOP_START)
	{
		ANM[CurAnm].isEvent[frame] -= dEVENT_LOOP_START;
		return;
	}

	for (int i=0;i<dMAX_FRAME;i++)
		if (ANM[CurAnm].isEvent[i] & dEVENT_LOOP_START)	ANM[CurAnm].isEvent[i] -= dEVENT_LOOP_START;

	ANM[CurAnm].isEvent[frame]	+=	dEVENT_LOOP_START;

	bVIRGIN	=	FALSE;
}

void
cANM::LoopEndFrame(int frame)
{
	if (ANM[CurAnm].isEvent[frame] & dEVENT_LOOP_END)
	{
		ANM[CurAnm].isEvent[frame] -= dEVENT_LOOP_END;
		return;
	}

	for (int i=0;i<dMAX_FRAME;i++)
		if (ANM[CurAnm].isEvent[i] & dEVENT_LOOP_END)	ANM[CurAnm].isEvent[i] -= dEVENT_LOOP_END;

	ANM[CurAnm].isEvent[frame]	+=	dEVENT_LOOP_END;

	bVIRGIN	=	FALSE;
}

BOOL
cANM::IsDamageFrame(int frame)
{	if (ANM[CurAnm].isEvent[frame]&dEVENT_DAMAGE)	return TRUE;

	return FALSE;
}

BOOL
cANM::IsTriggerFrame(int frame)
{	if (ANM[CurAnm].isEvent[frame]&dEVENT_TRIGGER)	return TRUE;

	return FALSE;
}

BOOL
cANM::IsStepFrame(int frame)
{	if (ANM[CurAnm].isEvent[frame]&dEVENT_STEP)	return TRUE;

	return FALSE;
}

BOOL
cANM::IsLoopStartFrame(int frame)
{	if (ANM[CurAnm].isEvent[frame]&dEVENT_LOOP_START)	return TRUE;

	return FALSE;
}

BOOL
cANM::IsLoopEndFrame(int frame)
{	if (ANM[CurAnm].isEvent[frame]&dEVENT_LOOP_END)	return TRUE;

	return FALSE;
}

int
GetDirectImage(int type,int direct,int moveoval)
{
	if (type>=dANI_DIRECT32) return 0;

	if (type == dANI_DIRECT8 && moveoval == 2)
	{	direct = valueDirect[type][direct];

		return direct;
	}

	return	valueDirect[type][direct];
}

void
cANM::SetReleasePos(int x,int y)
{	if (ANM[CurAnm].bSaveReleasePoint)
	{	ANM[CurAnm].Release[CurDirect].x	=	x;
		ANM[CurAnm].Release[CurDirect].y	=	y;
	}
}


//	프레임 이동
int
cANM::ReformPosX()
{	return	ANM[CurAnm].Pos[CurDirect][CurFrame].x;
}

int
cANM::ReformPosY()
{	return	ANM[CurAnm].Pos[CurDirect][CurFrame].y;
}

void
cANM::ReformPos(int x,int y)
{	bVIRGIN	=	FALSE;

	if (_MAIN.Lock)
	{	
		ANM[0].RefitPos[0].x	=	x;
		ANM[0].RefitPos[0].y	=	y;

		return;
	}

	ANM[CurAnm].Pos[CurDirect][CurFrame].x	=	x;
	ANM[CurAnm].Pos[CurDirect][CurFrame].y	=	y;
}

void
cANM::MovePos(int x,int y,BOOL _bIsPressedShift)
{
	bVIRGIN	=	FALSE;

	if (_MAIN.Lock)
	{	
		ANM[0].RefitPos[0].x	+=	x;
		ANM[0].RefitPos[0].y	+=	y;
		return;
	}

	if (_bIsPressedShift == FALSE)
	{
		ANM[CurAnm].Pos[CurDirect][CurFrame].x	+=	x;
		ANM[CurAnm].Pos[CurDirect][CurFrame].y	+=	y;
	}
	else
	{
		for (int i=0;i<GetMaxFrame();i++)
		{
			cIMG *frm	=	cIMG::Get(ANM[CurAnm].Sprite[CurDirect][i]);

			if (frm && frm!=cIMG::pTAIL)
			{	
				ANM[CurAnm].Pos[CurDirect][i].x	+=	x;
				ANM[CurAnm].Pos[CurDirect][i].y	+=	y;
			}
		}
	}
}


int	
cANM::GetLink(int anm)
{	if (anm == 0xffff) anm = CurAnm;

	return ANM[anm].LinkAnm;
}


void
cANM::Set(cANIMATION *anm)
{	if (!anm->IsEnable())
	{	ERRMSG("Animation synchronization error!!!","No data loaded.");
		return;
	}

	MoveOval			=	anm->MoveOval;
	bVIRGIN				=	FALSE;

	for (int i=0;i<anm->AnmCount;i++)
		ANM[i].Sync(&anm->pANM[i]);
}