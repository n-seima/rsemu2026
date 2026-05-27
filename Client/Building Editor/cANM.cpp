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
char	*regSADHEADER		=	"붉은 보석 건물 데이터";
char	*regSADHEADER2		=	"붉은 보석 건물 데이터 2";

tsSDHEADER	SDHEADER;
tsSADHEADER	SADHEADER;

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
int 	cANM::CrashType		=	dCRASH_NONE;
int		cANM::AnmCount		=	9;
int		cANM::CurAnm		=	0;
int		cANM::CurFrame		=	0;
int		cANM::CurDirect		=	0;
char	cANM::FName[256]	=	"";
char	cANM::ANMName[50]	=	"통상 에니메이션";
BYTE	cANM::PALETTE[256*3];

cANM	ANM[dMAX_ANM];

char	strAnmType[7][40]		=	{	"일반",	"좌우 2 방향","상하 2 방향","4 방향","8 방향","16 방향"};
char	strDirect[7][16][40]	=
{	{"한방향"},
	{"좌","우"},
	{"상","하"},
	{"북","동","남","서"},
	{"북","북동","동","남동","남","남서","서","북서"},
	{"북","북북동","북동","동북동",
	 "동","동남동","남동","남남동",
	 "남","남남서","남서","서남서",
	 "서","서북서","북서","북북서"}
};

char	valueDirect[7][16]	=
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
{	"걷기",
	"뛰기",
	"방어",
	"타격",
	"종베기",
	"횡베기",
	"마법",
	"필살기 1",
	"필살기 2",
};

char strCrashType[5][20]	=
{	"없음",
	"하나",
	"에니메이션당 하나씩",
	"방향에 하나씩",
	"이미지당 하나씩"
};


int		_directCount[7]	=	{   1,2,2,4,8,16};
int		FlipDirect[10]	=	{	1,d2DIRECT_W,1,d4DIRECT_S,d8DIRECT_S,d16DIRECT_S};

BOOL
cCRASHBOX::isIN(int x,int y)
{	if (x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom) return TRUE;

	return FALSE;
}

void
cIMG::Put(int x,int y,int rate,BOOL bflip,int effect)
{	if (pSPRITE)	PutSpriteR16[effect+bflip*11](x,y,pSPRITE,rate,rate,effect);
	if (p8SPRITE)	PutSpriteR8[effect +bflip*11](x,y,p8SPRITE,cANM::PLT,rate,rate,effect);
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
}

void
cIMG::addCrash(RECT rect)
{	if (crashCount >= dMAX_CRASH-1) return;

	if (rect.left > rect.right ) swap(rect.left,rect.right );
	if (rect.top  > rect.bottom) swap(rect.top ,rect.bottom);

	if (rect.right - rect.left < 2) return;
	if (rect.bottom- rect.top  < 2) return;


	CRASH[newCrash].serial	=	newCrash;
	SetRect(&CRASH[newCrash].rect,rect.left,rect.top,rect.right,rect.bottom);

	curCrash	=	newCrash;

	crashCount++;

	for (int i=newCrash+1;i<dMAX_CRASH;i++)
	{	if (CRASH[i].serial==0xffff)
		{	newCrash	=	i;
			return;
		}
	}
}

void
cIMG::delCrash(int index)
{	if (CRASH[index].serial==0xffff) return;
	if (crashCount <= 0) return;
	CRASH[index].serial	=	0xffff;

	crashCount--;

	if (index < newCrash) newCrash=index;
}

int
cIMG::GetCrash(int x,int y)
{	for (int i=0,index=0;i<crashCount && index < dMAX_CRASH;index++)
	{	if (CRASH[index].serial!=0xffff)
		{	i++;
			if (CRASH[index].isIN(x,y))
			{	curCrash=index;
				return index;
			}
		}
	}

	return -1;
}

void
cIMG::PreCrush()
{	for (int i=curCrash-1;i>=0;i--) if (CRASH[i].serial!=0xffff) {curCrash=i;return;}
}

void
cIMG::NextCrush()
{	for (int i=0,index=0;i<crashCount && index < dMAX_CRASH;index++)
		if (CRASH[index].serial!=0xffff)
		{	i++;
			if (index > curCrash)
			{	curCrash=index;
				return;
			}
		}
}

BOOL
cIMG::isIN(int x,int y)
{	char	*checkField;

	if (pSPRITE)
	{	int	width	=	pSPRITE[0];
		int	height	=	pSPRITE[1];

		if (x<0||y<0||x>=width||y>=height) return FALSE;

		checkField	=	new char [width*height];

		int		xPoint=0,i,index;
		int		count,bufferPoint=4;
		int		size;

		int		addr=0;

		memset(checkField,0,width*height);

		for(index=0;index<height;index++)
		{	count	=	pSPRITE[bufferPoint++];
			xPoint	=	0;

			for(i=0;i<count;i++)
			{	xPoint	+=	pSPRITE[bufferPoint++];
				size	=	pSPRITE[bufferPoint++];

				memset(checkField+addr+xPoint,1,size);

				bufferPoint	+=	size;
				xPoint		+=	size;
			}

			addr+=width;
		}

		BOOL	result;

		if (checkField[x+y*width]==1)	result=TRUE;
		else							result=FALSE;

		pKILL(checkField);

		return result;
	}

	if (p8SPRITE)
	{	int	width	=	*(WORD *) (p8SPRITE);
		int	height	=	*(WORD *) (p8SPRITE+2);

		if (x<0||y<0||x>=width||y>=height) return FALSE;

		checkField	=	new char [width*height];

		int		xPoint=0,i,index;
		int		count,bufferPoint=8;
		int		size;

		int		addr=0;

		memset(checkField,0,width*height);

		for(index=0;index<height;index++)
		{	count	=	p8SPRITE[bufferPoint++];
			xPoint	=	0;

			for(i=0;i<count;i++)
			{	xPoint	+=	p8SPRITE[bufferPoint++];
				size	=	p8SPRITE[bufferPoint++];

				memset(checkField+addr+xPoint,1,size);

				bufferPoint	+=	size;
				xPoint		+=	size;
			}

			addr+=width;
		}

		BOOL	result;

		if (checkField[x+y*width]==1)	result=TRUE;
		else							result=FALSE;

		pKILL(checkField);

		return result;
	}

	return FALSE;
}


void
cIMG::reform(int dx,int dy)
{	if (_MAIN.Lock) return;

	xp		+=	dx;
	yp		+=	dy;

	if (pSPRITE)	pSPRITE[2]	+=	dx,pSPRITE[3]	+=	dy;
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

	bVIRGIN		=	FALSE;
}

void
cIMG::reformShadow(int dx,int dy)
{	if (_MAIN.Lock) return;

	if (pSHADOW)
	{	*(short *)(pSHADOW	+4)	+=	dx;
		*(short *)(pSHADOW	+6)	+=	dy;
	}

	bVIRGIN		=	FALSE;
}

void
cIMG::rePos(int x,int y)
{	if (_MAIN.Lock) return;

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

	bVIRGIN		=	FALSE;
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
{	bVIRGIN	=	FALSE;

	if (xp1==0xffff)	xp1	=xs1/2;
	if (yp1==0xffff)	yp1	=ys1/2;

	cIMG		*img;
	BYTE		*image	=	NULL;

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
{	if (Count <= 0 || index >= Count) return NULL;
	
	cIMG *img=pHEAD->pNEXT;

	for (int i=0;i<Count;i++)
	{	if (i==index) return img;
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
	s_iMaxSpriteWidth	=	0;
	s_iMaxSpriteHeight	=	0;
	s_iMaxShadowWidth	=	0;
	s_iMaxShadowHeight	=	0;

	if	(Count <= 0)
		return;

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
{	if (Count <= 0) return;
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
{	if (pHEAD)
	{	cIMG *img	=	pHEAD->pNEXT;
		while(img	!=	pTAIL)
		{	img	=	img->pNEXT;
			KILL(img->pPRE);
		}

		KILL(pHEAD);
		KILL(pTAIL);
	}

	Count	=	0,BigXS	=	0,BigYS	=	0,Current	=	0;
	pCURRENT=	NULL;
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
cIMG::Reform(int dx,int dy,int value,int control)
{	if (Count <= 0) return;

	int		i;

	cIMG *img=pHEAD->pNEXT;

	switch(value)
	{	case	0	:
			if (pCURRENT)
			{	if (control)	pCURRENT->reformShadow(dx,dy);
				else			pCURRENT->reform(dx,dy);
			}
			break;

		case	1	:
			while(img!=pTAIL)
			{	if (control)	img->reformShadow(dx,dy);
				else			img->reform(dx,dy);

				img	=	img->pNEXT;
			}
			break;

		case	2	:
			for (i=0;i<cANM::GetDirCount();i++)
			{	cIMG *frm	=	Get(ANM[cANM::CurAnm].Sprite[cANM::CurDirect][i]);
				if (frm && frm!=pTAIL)
				{	if (control)	frm->reformShadow(dx,dy);
					else			frm->reform(dx,dy);
				}
			}
			break;
	}
}

//	일반 멤버..

void
cANM::ClearData(int type)
{	int i;

	for (i=0;i<dMAX_FRAME;i++) isEvent[i]=FALSE;

	if (type	!=	0xffff)
	{	directCount	=	_directCount[type];
		anmtype		=	type;
		FrameCount	=	10;
		strcpy(Name,"명칭 없음");
		bSTATIC		=	FALSE;
	}

	ALPHA		=	dPUT_NORMAL;

	for (i=0;i<dMAX_DIRECT;i++)	for (int j=0;j<dMAX_FRAME;j++)
		Pos[i][j].x=0,Pos[i][j].y=0,Sprite[i][j]=0;
}

void
cANM::Set(cANM *anm)
{	int i,j;

	strcpy(Name,anm->Name);

	for (i=0;i<dMAX_DIRECT;i++)	
		for (j=0;j<dMAX_FRAME;j++)	
		{	Pos[i][j].x	=	anm->Pos[i][j].x;
			Pos[i][j].y	=	anm->Pos[i][j].y;
			Sprite[i][j]=	anm->Sprite[i][j];
		}

	for (i=0;i<dMAX_FRAME;i++)	isEvent[i]	=	anm->isEvent[i];

	FrameCount	=	anm->FrameCount	,	directCount	=	anm->directCount;
	bSTATIC		=	anm->bSTATIC	,	ALPHA		=	anm->ALPHA;
	anmtype		=	anm->anmtype;
}

void
cANM::Put(int xp,int yp,int rate,BOOL flip)
{	if (FrameCount <= 0) return;

	CurFrame	=	min(CurFrame,FrameCount-1);

	int index	=	Sprite[0][CurFrame];

	cIMG *img	=	cIMG::Get(index);

	img->Put(xp-Pos[0][CurFrame].x*rate/100,yp-Pos[0][CurFrame].y*rate/100,rate,FALSE,ALPHA);
}

void
cANM::PutAll(int xp,int yp,int Rate,int alpha)
{	for (int i=0;i<FrameCount;i++)
	{	int index	=	Sprite[0][i];
		cIMG *img	=	cIMG::Get(index);
		img->Put(xp-Pos[0][i].x*Rate/100,yp-Pos[0][i].y*Rate/100,Rate,FALSE,alpha);
	}
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
{	frame	=	getframe(frame);
	anm		=	getanm(anm);
	ANM[CurAnm].isEvent[frame]=TRUE;
	bVIRGIN=FALSE;
}

int
cANM::GetFrameImage(int frame,int anm,int direct)
{	frame	=	getframe(frame);
	anm		=	getanm(anm);
	direct	=	getdirect(direct);
	if (frame < 0 || frame >= ANM[CurAnm].FrameCount) return 0xffff;
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

	SetCurrentFrame(0,anm,0);

	bVIRGIN	=	FALSE;
}

void
cANM::SetCurrentFrame(int frame,int anm,int direct)
{	if (frame < 0 ) return;

	anm						=	getanm(anm);

	if (ANM[anm].FrameCount	==	0)
	{	cIMG::pCURRENT=NULL;
		return;
	}
	CurFrame=getframe(frame);
	cIMG::SetCURRENT(GetFrameImage(frame,anm,direct));
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
	SetCurrentFrame();
}

void
cANM::PlusCurrentAnm()
{	CurFrame	=	0,CurDirect	=	0,CurAnm	=	min(AnmCount-1,CurAnm+1);
	SetCurrentFrame();
}

void
cANM::MinusCurrentDirect()
{	CurFrame	=	0,CurDirect--;
	if (CurDirect< 0 ) CurDirect= GetMaxDirect()-1;
	SetCurrentFrame();
}

void	
cANM::PlusCurrentDirect()
{	CurFrame	=	0,CurDirect++;
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
	{	if (cMSG::YESNO("에니메이션 셋팅 데이터 저장","'%s'와 같은 이름의 화일이 존재 합니다. 덮어 씌우시겠습니까?",FName)==IDNO) return FALSE;
		fclose(fp);
	}

	fp=fopen(FName,"wb");

	if (!fp)
	{	fclose(fp);
		fp=fopen(FName,"wb");
		if (!fp)	return cMSG::Put("에니메이션 셋팅 데이터 저장 에러!!","'%s' 화일 쓰기 에러입니다.",FName);
	}

	fwrite(regANISET	,40,1,fp);
	fwrite(ANMName		,40,1,fp);
	fwrite(&bHALF		,1,1,fp);
	fwrite(&bSAVEOUTLINE,1,1,fp);
	fwrite(&bSAVESHADOW	,1,1,fp);

	fwrite(&AnmType		,4,1,fp);
	fwrite(&AnmCount	,4,1,fp);

	for (i=0;i<AnmCount;i++)
	{	fwrite(ANM[i].Name			,40	,1,fp);
		fwrite(&ANM[i].anmtype		,4	,1,fp);
		fwrite(&ANM[i].directCount	,4	,1,fp);
		fwrite(&ANM[i].FrameCount	,4	,1,fp);
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
	{	if (cMSG::YESNO("기존의 에니메이션 셋팅이 초기화 됩니다.","계속하시겠습니까?")	!=	IDYES) 
			return FALSE;
	
		OPENFILENAME	ofn;

		memset(&ofn,0,sizeof(OPENFILENAME));
		ofn.lStructSize		=	sizeof(OPENFILENAME);
		ofn.hwndOwner		=	_MAIN.hWND;
		ofn.lpstrFilter		=	"에니메이션 셋팅 데이터 {*.asd}\0*.asd\0\0";
		ofn.lpstrTitle		=	"에니메이션 셋팅 데이터 불러오기.";
		ofn.lpstrFile		=	FName;
		ofn.nMaxFile		=	256;
		ofn.Flags			=	OFN_HIDEREADONLY | OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST;
		ofn.lpstrDefExt		=	"asd";

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
		return cMSG::Put("에니메이션 셋팅 데이터 오픈 에러!!","화일 인식자가 다릅니다.",FName);
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
		ANM[i].ALPHA			=	dPUT_NORMAL;
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
		if (!fp)	return cMSG::Put("폰트 화일 저장 에러!!","'%s' 화일 쓰기 에러입니다.",fname);
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

BOOL
cANM::SaveSD(char *fn)
{	FILE *fp;
	int i;

	fp=fopen(fn,"wb");
	if (!fp)
	{	fclose(fp);
		fp=fopen(fn,"wb");
		if (!fp)	return cMSG::Put("스프라이트 화일 저장 에러!!","'%s' 화일 쓰기 에러입니다.",fn);
	}

	fseek(fp,sizeof(SDHEADER),SEEK_SET);	//	화일 헤더부분.. 화일 사이즈 땜시 건너뜀..

	if (BPP==8)	fwrite(PLT,512,1,fp);

	//	여기부터 스프라이트 데이터..
	cIMG *img=cIMG::pHEAD->pNEXT;

	int Size;

	Size=0;

	fwrite(&Size,sizeof(int),1,fp);

	for (i=0;i<cIMG::Count;i++)
	{	Size+=img->Size;
		fwrite(&Size,sizeof(int),1,fp);
		img=img->pNEXT;
	}

	img=cIMG::pHEAD->pNEXT;

	//	스프라이트 데이타 저장..
	if (BPP==8)
		for (i=0;i<cIMG::Count;i++)
		{	fwrite(img->p8SPRITE,img->Size,1,fp);
			img=img->pNEXT;
		}

	if (BPP==16)
		for (i=0;i<cIMG::Count;i++)
		{	fwrite(img->pSPRITE,img->Size,sizeof(WORD),fp);
			img=img->pNEXT;
		}

	if (bSAVESHADOW)
	{
	}

//	if (bSAVEOUTLINE)
//	{
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
//	}

	SDHEADER.Size		=	ftell(fp);
	strcpy(SDHEADER.Reg,regSDHEADER);
	SDHEADER.BPP		=	BPP;
	SDHEADER.ImageCount	=	cIMG::Count;
	SDHEADER.Alpha		=	_WORKWND.Alpha;
	SDHEADER.bOutline	=	bSAVEOUTLINE;
	SDHEADER.bShadow	=	bSAVESHADOW;

	fseek(fp,0L,SEEK_SET);
	fwrite(&SDHEADER,sizeof(SDHEADER),1,fp);
	fclose(fp);

	bVIRGIN	=	TRUE;

	return TRUE;
}

BOOL
cANM::LoadSD(char *fn)
{	FILE *fp;
	int i;

	fp=fopen(fn,"rb");
	if (!fp)	return cMSG::Error("스프라이트 화일 불러오기 에러!!","'%s' 화일을 찾을 수 없습니다.",fn);

	fread(&SDHEADER,sizeof(SDHEADER),1,fp);
	if (strcmp(SDHEADER.Reg,regSDHEADER)!=0)
		return cMSG::Put("스프라이트 화일 불러오기 에러!!","'%s'의 인식자가 일치하지 않습니다. ",fn);

	cIMG::Reset();

	if (SDHEADER.BPP==8)	fread(&PLT,512,1,fp);

	//	여기부터 스프라이트 데이터..
	int *Size;

	Size	=	new int [SDHEADER.ImageCount+1];
	fread(Size,sizeof(int),SDHEADER.ImageCount+1,fp);

	if (SDHEADER.BPP==8)
		for (i=0;i<SDHEADER.ImageCount;i++)
		{	BYTE *spr;

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

	cIMG *img	=	cIMG::pHEAD->pNEXT;

	fread(Size,sizeof(int),SDHEADER.ImageCount+1,fp);

	for (i=0;i<cIMG::Count;i++)
	{	BYTE	*lspr;

		lspr =	new BYTE [Size[i+1]-Size[i]];
		fread(lspr,sizeof(BYTE),Size[i+1]-Size[i],fp);

		img->SetLayer(lspr,Size[i+1]-Size[i]);
		img=img->pNEXT;
	}

	BPP				=	SDHEADER.BPP;
	_WORKWND.Alpha	=	SDHEADER.Alpha;
	bSAVEOUTLINE	=	SDHEADER.bOutline;
	bSAVESHADOW		=	TRUE;

	fclose(fp);

	bVIRGIN	=	TRUE;

	pKILL(Size);

	cIMG::SetCURRENT(0);

	return TRUE;
}

BOOL
cANM::SaveSAD(char *fn)
{
	FILE *fp;
	int i;

	bSAVESHADOW	=	TRUE;

	fp=fopen(fn,"wb");

	if (!fp)
	{	fclose(fp);
		fp=fopen(fn,"wb");
		if (!fp)	return cMSG::Put("스프라이트 에니메이션 화일 저장 에러!!","'%s' 화일 쓰기 에러입니다.",fn);
	}

	fseek(fp,sizeof(SADHEADER),SEEK_SET);	//	화일 헤더부분.. 화일 사이즈 땜시 건너뜀..

	if (BPP==8)	fwrite(PLT,512,1,fp);

	//	여기부터 스프라이트 데이터..
	cIMG *img=cIMG::pHEAD->pNEXT;

	int Size;

	Size=0;

	fwrite(&Size,sizeof(int),1,fp);

	for (i=0;i<cIMG::Count;i++)
	{	Size+=	img->Size;
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
		for (i=0;i<cIMG::Count;i++)
		{	fwrite(img->pSPRITE,img->Size,sizeof(WORD),fp);
			img=img->pNEXT;
		}

	//	그림자 데이타 저장..
	if	(bSAVESHADOW)
	{
		img	=	cIMG::pHEAD->pNEXT;

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
	}

	img		=	cIMG::pHEAD->pNEXT;
	Size	=	0;

	fwrite(&Size,sizeof(int),1,fp);

	for (i=0;i<cIMG::Count;i++)
	{	Size+=	img->lSize;
		fwrite(&Size,sizeof(int),1,fp);
		img	=	img->pNEXT;
	}

	img=cIMG::pHEAD->pNEXT;

	for (i=0;i<cIMG::Count;i++)
	{	fwrite(img->pLAYER,img->lSize,1,fp);
		img	=	img->pNEXT;
	}

	//	에니메이션 데이터..

	i	=	0;
	fwrite(&i,4,1,fp);

	for (i=0;i<AnmCount;i++)			//	에니
	{	fwrite(&ANM[i].FrameCount		,2,1,fp);

		if (ANM[i].FrameCount > 0)
		{	fwrite(&ANM[i].ALPHA		,1,1,fp);
			fwrite(&ANM[i].Pos[0][0]	,4,ANM[i].FrameCount,fp);
			fwrite(&ANM[i].Sprite[0][0]	,2,ANM[i].FrameCount,fp);
		}
	}

	//	충돌 박스..

	BYTE *data = cTRANS::ImageToSprite(256,256,0,0,0,&CRASHES.DATA[0][0],Size);

	fwrite(&Size,4,1,fp);
	fwrite(data,Size,1,fp);
	pKILL(data);

	//	블럭 박스
	data = cTRANS::ImageToSprite(256,256,0,0,0,&BLOCKS.DATA[0][0],Size);

	fwrite(&Size,4,1,fp);
	fwrite(data,Size,1,fp);
	pKILL(data);

	//	오버랩 영역
	data = cTRANS::ImageToSprite(256,256,0,0,0,&OVERLAPBLOCKS.DATA[0][0],Size);

	fwrite(&Size,4,1,fp);
	fwrite(data,Size,1,fp);
	pKILL(data);

	//	에디터에서 쓸 데이터..

	fwrite(ANMName		,40,1,fp);
	fwrite(&bHALF		,1,1,fp);
	fwrite(&bSAVEOUTLINE,1,1,fp);
	fwrite(&bSAVESHADOW	,1,1,fp);

	fwrite(&AnmType		,4,1,fp);
	fwrite(&AnmCount	,4,1,fp);

	for (i=0;i<AnmCount;i++)
	{	fwrite(ANM[i].Name			,40	,1,fp);
		fwrite(&ANM[i].anmtype		,4	,1,fp);
		fwrite(&ANM[i].directCount	,4	,1,fp);
		fwrite(&ANM[i].FrameCount	,4	,1,fp);
	}

	//	헤더 저장..

	cIMG::GetMaxImage();
	SADHEADER.Size		=	ftell(fp);
	strcpy(SADHEADER.Reg,regSADHEADER2);
	SADHEADER.AnmCount	=	AnmCount;
	SADHEADER.ImageCount=	cIMG::Count;
	SADHEADER.AnmType	=	AnmType;
	SADHEADER.CrashType	=	CrashType;
	SADHEADER.bOutline	=	bSAVEOUTLINE;
	SADHEADER.bShadow	=	bSAVESHADOW;
	SADHEADER.bHalf		=	bHALF;
	SADHEADER.BPP		=	BPP;

	SDHEADER.m_wMaxSpriteWidth	=	cIMG::s_iMaxSpriteWidth;
	SDHEADER.m_wMaxSpriteHeight	=	cIMG::s_iMaxSpriteHeight;
	SDHEADER.m_wMaxShadowWidth	=	cIMG::s_iMaxShadowWidth;
	SDHEADER.m_wMaxShadowHeight	=	cIMG::s_iMaxShadowHeight;

	fseek(fp,0L,SEEK_SET);
	fwrite(&SADHEADER,sizeof(SDHEADER),1,fp);
	fclose(fp);

	bVIRGIN	=	TRUE;

	return TRUE;
}

BOOL
cANM::LoadSAD(char *fn)
{
	FILE *fp;
	int i;

	fp	=	fopen(fn,"rb");

	if	(!fp)
		return cMSG::Put("스프라이트 에니메이션 화일 오픈 에러!!","'%s' 화일을 찾을 수 없습니다.",fn);

	fread(&SADHEADER,sizeof(SADHEADER),1,fp);

	if	(strcmp(SADHEADER.Reg,regSADHEADER)!=0 && strcmp(SADHEADER.Reg,regSADHEADER2)!=0 )
		return cMSG::Put("스프라이트 에니메이션 화일 불러오기 에러!!","'%s'의 인식자가 일치하지 않습니다. ",fn);

	AnmCount	=	SADHEADER.AnmCount;
	AnmType		=	SADHEADER.AnmType;
	CrashType	=	SADHEADER.CrashType;
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

	if (bSAVESHADOW && SADHEADER.ImageCount)
	{	
		img	=	cIMG::pHEAD->pNEXT;

		fread(Size,sizeof(int),SADHEADER.ImageCount+1,fp);

		for (i=0;i<cIMG::Count;i++)
		{
			BYTE *pshadow	=	NULL;

			int size	=	Size[i+1]-Size[i];

			if	(size>0)
			{
				pshadow =	new BYTE [size];
				fread(pshadow,sizeof(BYTE),size,fp);
			}

			img->SetShadow(pshadow,size);
			img	=	img->pNEXT;
		}
	}

	img	=	cIMG::pHEAD->pNEXT;

	fread(Size,sizeof(int),SADHEADER.ImageCount+1,fp);

	for (i=0;i<cIMG::Count;i++)
	{
		BYTE	*lspr	=	NULL;

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

	fread(&i,4,1,fp);

	for (i=0;i<AnmCount;i++)			//	에니
	{	ANM[i].anmtype		=	0;
		ANM[i].directCount	=	1;
		fread(&ANM[i].FrameCount,2,1,fp);

		if (ANM[i].FrameCount > 0)
		{	fread(&ANM[i].ALPHA			,1,1,fp);
			fread(&ANM[i].Pos[0][0]		,4,ANM[i].FrameCount,fp);
			fread(&ANM[i].Sprite[0][0]	,2,ANM[i].FrameCount,fp);
		}
	}

	//	충돌 박스..

	CRASHES.Reset();
	BLOCKS.Reset();
	OVERLAPBLOCKS.Reset();
	int size;

	fread(&size,4,1,fp);

	//	충돌 박스
	BYTE *data = new BYTE [size];

	fread(data,size,1,fp);

	int	bufferPoint	=	8;
	int	addr		=	0;

	for(int y=0;y<256;y++)
	{	int count	=	data[bufferPoint++];
		int	xPoint	=	0;

		for(i=0;i<count;i++)
		{	xPoint	+=	data[bufferPoint++];
			size	=	data[bufferPoint++];

			for (int x=0;x<size;x++)
				*(&CRASHES.DATA[0][0]+addr+xPoint+x)=*(data+bufferPoint+x);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=256;
	}

	pKILL(data);

	//	블럭 박스
	fread(&size,4,1,fp);

	data = new BYTE [size];

	fread(data,size,1,fp);

	bufferPoint	=	8;
	addr		=	0;

	for(y=0;y<256;y++)
	{	int count	=	data[bufferPoint++];
		int	xPoint	=	0;

		for(i=0;i<count;i++)
		{	xPoint	+=	data[bufferPoint++];
			size	=	data[bufferPoint++];

			for (int x=0;x<size;x++)
				*(&BLOCKS.DATA[0][0]+addr+xPoint+x)=*(data+bufferPoint+x);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=256;
	}
	pKILL(data);

	//	오버랩 데이터
	fread(&size,4,1,fp);

	data = new BYTE [size];

	fread(data,size,1,fp);

	bufferPoint	=	8;
	addr		=	0;

	for(y=0;y<256;y++)
	{	int count	=	data[bufferPoint++];
		int	xPoint	=	0;

		for(i=0;i<count;i++)
		{	xPoint	+=	data[bufferPoint++];
			size	=	data[bufferPoint++];

			for (int x=0;x<size;x++)
				*(&OVERLAPBLOCKS.DATA[0][0]+addr+xPoint+x)=*(data+bufferPoint+x);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=256;
	}
	pKILL(data);

	//	에디터에서 쓸 데이터..

	fread(ANMName		,40,1,fp);
	fread(&bHALF		,1,1,fp);
	fread(&bSAVEOUTLINE	,1,1,fp);
	fread(&bSAVESHADOW	,1,1,fp);

	fread(&AnmType		,4,1,fp);
	fread(&AnmCount		,4,1,fp);

	for (i=0;i<AnmCount;i++)
	{	fread(ANM[i].Name			,40	,1,fp);
		fread(&ANM[i].anmtype		,4	,1,fp);
		fread(&ANM[i].directCount	,4	,1,fp);
		fread(&ANM[i].FrameCount	,4	,1,fp);
	}


	fclose(fp);

	bVIRGIN		=	TRUE;

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
}


void
cANM::SetFront(int frame)
{	ANM[CurAnm].BackFrame[CurDirect][frame]	=	0;
	bVIRGIN	=	FALSE;
}

void
cANM::SetBack(int frame)
{	ANM[CurAnm].BackFrame[CurDirect][frame]	=	1;
	bVIRGIN	=	FALSE;
}

BOOL
cANM::IsBack()
{	return ANM[CurAnm].BackFrame[CurDirect][CurFrame];
}


int
cANM::ReformPosX()
{	return	ANM[CurAnm].Pos[0][CurFrame].x;
}

int
cANM::ReformPosY()
{	return	ANM[CurAnm].Pos[0][CurFrame].y;
}

void
cANM::ReformPos(int x,int y)
{	ANM[CurAnm].Pos[0][CurFrame].x	=	x;
	ANM[CurAnm].Pos[0][CurFrame].y	=	y;
	bVIRGIN	=	FALSE;
}

void
cANM::MovePos(int x,int y)
{	ANM[CurAnm].Pos[0][CurFrame].x	+=	x;
	ANM[CurAnm].Pos[0][CurFrame].y	+=	y;
	bVIRGIN	=	FALSE;
}
