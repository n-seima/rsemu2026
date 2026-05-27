#include <stdio.h>
#include "cIMAGE.H"
#include "cDRAW.H"

#define	dMAX_IMAGE_SIZE	204800

static	BYTE	tempIMAGE[dMAX_IMAGE_SIZE];

struct 
{	int			Size;
	char		Reg[50];
	BYTE		BPP;
	WORD		ImageCount;
	BYTE		Alpha,bOutline,bShadow;
}	SDHEADER;

struct
{	int			Size;
	char		Reg[50];
	BYTE		AnmCount;
	BYTE		byAlpha;
	WORD		ImageCount;
	BYTE		AnmType,CrashType;
	BYTE		bOutline,bShadow,bHalf;
	BYTE		BPP;

}	SADHEADER;

static	int		FlipDirectCount[2][10]		=	
{	{100,100		,100,100		,100		,100		},
	{1	,d2DIRECT_W	,1	,d4DIRECT_S	,d8DIRECT_S	,d16DIRECT_S}
};

static	int	FlipDirect[2][10][30]	=
{	{	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		//	플립 아님
		{d2DIRECT_E,d2DIRECT_W,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{d2DIRECT_N,d2DIRECT_S,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{d4DIRECT_N,d4DIRECT_E,d4DIRECT_S,d4DIRECT_W,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{d8DIRECT_N,d8DIRECT_NE,d8DIRECT_E,d8DIRECT_SE,d8DIRECT_S,d8DIRECT_SW,d8DIRECT_W,d8DIRECT_NW,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{d16DIRECT_N,d16DIRECT_NNE,d16DIRECT_NE,d16DIRECT_ENE,d16DIRECT_E,d16DIRECT_ESE,d16DIRECT_SE,d16DIRECT_SSE,
		 d16DIRECT_S,d16DIRECT_SSW,d16DIRECT_SW,d16DIRECT_WSW,d16DIRECT_W,d16DIRECT_WNW,d16DIRECT_NW,d16DIRECT_NNW},
	},
	{	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		//	플립
		{d2DIRECT_E,d2DIRECT_E		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{d2DIRECT_N,d2DIRECT_S		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{d4DIRECT_N,d4DIRECT_E,d4DIRECT_S,d4DIRECT_E		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{d8DIRECT_N,d8DIRECT_NE,d8DIRECT_E,d8DIRECT_SE,d8DIRECT_S,d8DIRECT_SE,d8DIRECT_E,d8DIRECT_NE		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{d16DIRECT_N,d16DIRECT_NNE,d16DIRECT_NE,d16DIRECT_ENE,d16DIRECT_E,d16DIRECT_ESE,d16DIRECT_SE,d16DIRECT_SSE,
		 d16DIRECT_S,d16DIRECT_SSE,d16DIRECT_SE,d16DIRECT_ESE,d16DIRECT_E,d16DIRECT_ENE,d16DIRECT_NE,d16DIRECT_NNE},
	}
};

static	int	IsFlip[2][10][30]	=
{	{	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		//	플립 아님
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	},
	{	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		//	플립
		{0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,11,11,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,11},
	}
};

/********************************************************************
	스프라이트 데이터
********************************************************************/

cSPRITE::cSPRITE()
{	p16DATA	=	NULL,	p8DATA		=	NULL,	spriteOFFSET	=	NULL;
	pLAYER	=	NULL,	layerOFFSET	=	NULL;
	pSHADOW	=	NULL,	shadowOFFSET=	NULL;
	pPLT	=	NULL;
	Count	=	0	,	BPP			=	16;
}

cSPRITE::~cSPRITE()
{	Reset();
}

void
cSPRITE::Reset()
{	_free(p16DATA);
	_free(p8DATA);
	_free(pLAYER);
	_free(pSHADOW);
	_free(spriteOFFSET);
	_free(layerOFFSET);
	_free(shadowOFFSET);
	if (bLOADEDPLT)	_free(pPLT);
}

BOOL
cSPRITE::Load(char *fn,BOOL loadpalette,cNUX *nux)
{	cFILE *file;

	strcpy(FName,fn);
	if (nux)	file=	nux->Open(fn);
	else
	{	file=	new cFILE();
		if (!file->Open(fn,"rb"))	KILL(file);
	}

	if (!file)	return MSGOUT("Error in cSPRITE::Load - '%s' file not found",fn);

	file->Read(&SDHEADER,sizeof(SDHEADER));

#ifdef dDEBUG_MODE
	if	(strcmp(SDHEADER.Reg,REGSDHEADER)!=0 && strcmp(SDHEADER.Reg,REGSDHEADER2)!=0)
		return MSGOUT("Error in cSPRITE::Load - header not match");
#endif
	BPP		=	SDHEADER.BPP;
	bSHADOW	=	SDHEADER.bShadow;
	bLAYER	=	SDHEADER.bOutline;
	Count	=	SDHEADER.ImageCount;
		
	BOOL result =	Load(file,loadpalette);

	KILL(file);

	return result;
}

BOOL
cSPRITE::Load(cFILE *file,BOOL loadpalette)
{	Reset();

	bLOADEDPLT	=	loadpalette;

	if (BPP==8)
	{	if (loadpalette)
		{	pPLT			=	_new(WORD,256,"cSPRITE::pPLT");
			file->Read(pPLT,512);
		}
		else	file->Seek(512,SEEK_CUR);
	}
	//	스프라이트 데이터..
	spriteOFFSET	=	_new(int,Count+1,"cSPRITE::spriteOFFSET");

	file->Read(spriteOFFSET,(Count+1),4);

	if (BPP==16)
	{	p16DATA			=	_new(WORD,spriteOFFSET[Count],"cSPRITE::p16DATA");
		file->Read(p16DATA	,spriteOFFSET[Count],2);
	}
	else
	{	p8DATA			=	_new(BYTE,spriteOFFSET[Count],"cSPRITE::p8DATA");
		file->Read(p8DATA	,spriteOFFSET[Count]);
	}

	//	그림자 불러오기
	if (bSHADOW)
	{	shadowOFFSET	=	_new(int,Count+1,"cSPRITE::shadowOFFSET");
		file->Read(shadowOFFSET,Count+1,4);
		if(shadowOFFSET[Count]>0)
		{	pSHADOW			=	_new(BYTE,shadowOFFSET[Count],"cSPRITE::shadowOFFSET");
			file->Read(pSHADOW,shadowOFFSET[Count]);
		}
	}

	//	외곽선 불러오기
	if (bLAYER)
	{	layerOFFSET		=	_new(int,Count+1,"cSPRITE::layerOFFSET");
		file->Read(layerOFFSET,Count+1,4);
		pLAYER			=	_new(BYTE,layerOFFSET[Count],"cSPRITE::pLAYER");
		file->Read(pLAYER,layerOFFSET[Count]);
	}
	else
	{	DWORD	size;

		file->Seek(Count*4,SEEK_CUR);
		file->Read(&size,4);
		file->Seek(size,SEEK_CUR);
	}

	return TRUE;
}

WORD*
cSPRITE::Get16(int index)
{
#ifdef dDEBUG_MODE
	if (!p16DATA) return NULL;
#endif

	return (p16DATA+spriteOFFSET[index]);
}

BYTE*
cSPRITE::Get8(int index)
{
#ifdef dDEBUG_MODE
	if (!p8DATA) return NULL;
#endif

	return (p8DATA+spriteOFFSET[index]);
}

void
cSPRITE::GetRect(int index,cRECT *rect)
{	if (p16DATA)
	{	WORD	*pdata	=	p16DATA + spriteOFFSET[index];

		rect->x1	=	-(short )pdata[2];
		rect->y1	=	-(short )pdata[3];
		rect->x2	=	(WORD )pdata[0];
		rect->y2	=	(WORD )pdata[1];
	}
	if (p8DATA)
	{	BYTE	*pdata	=	p8DATA + spriteOFFSET[index];

		rect->x1	=	-*(short *)(pdata+4);
		rect->y1	=	-*(short *)(pdata+6);
		rect->x2	=	*(WORD  *)(pdata  );
		rect->y2	=	*(WORD  *)(pdata+2);
	}
}

void
cSPRITE::SetPalette(WORD *plt)
{	if (bLOADEDPLT)
	{	_free(pPLT);
		bLOADEDPLT	=	FALSE;
#ifdef dDEBUG_MODE
		MSGOUT("Warning!! in cSPRITE::SetPalette - Palette is already Loaded");
#endif
	}
	pPLT	=	plt;
}

void
cSPRITE::Put(int x,int y,int index,int xrate,int yrate,int effect,int alpha,int flip)
{	if (effect	<=	dPUT_NORMAL)	alpha	=	effect;

	if (index	>=	Count)	return;

	if (p8DATA)	PutSpriteR8[effect	+flip](x,y,p8DATA+spriteOFFSET[index],pPLT,xrate,yrate,alpha);
	if (p16DATA)PutSpriteR16[effect	+flip](x,y,p16DATA+spriteOFFSET[index],xrate,yrate,alpha);
}

void
cSPRITE::PutColor(int x,int y,int index,WORD color,int xrate,int yrate)
{
	if (index	>=	Count)	return;
	if (p8DATA)	PutSpriteR8[dPUT_COLOR](x,y,p8DATA+spriteOFFSET[index],pPLT,xrate,yrate,color);
	if (p16DATA)PutSpriteR16[dPUT_COLOR](x,y,p16DATA+spriteOFFSET[index],xrate,yrate,color);
}

int
cSPRITE::FitPut(int x,int y,int dx,int dy,int index,BOOL putshadow)
{
	if (index	>=	Count)	return 0;
	int xs=0,ys=0,rate=100;

	if (p8DATA)
	{
		BYTE *pDATA	=	p8DATA+spriteOFFSET[index];

		xs	=	*(WORD *)(pDATA  );
		ys	=	*(WORD *)(pDATA+2);

		if (xs > dx)	rate	=	dx*100/xs;
		if (ys > dy)	rate	=	min(rate,dy*100/ys);

		int	xp	=	*(short *)(pDATA+4  );//(limitX-xs*rate/100)/2;
		int	yp	=	*(short *)(pDATA+6  );//(limitY-xs*rate/100)/2;

		xp	=	xp*rate/100;
		yp	=	yp*rate/100;

		xp	=	xp + (dx-xs*rate/100)/2;
		yp	=	yp + (dy-ys*rate/100)/2;

		PutSpriteR8[dPUT_NORMAL](x+xp,y+yp,pDATA,pPLT,rate,rate,32);

		if (putshadow && pSHADOW)	PutShadow(x+xp,y+yp,index,rate,rate);

		return rate;
	}

	WORD *pDATA	=	p16DATA+spriteOFFSET[index];

	xs	=	pDATA[0];
	ys	=	pDATA[1];

	if (xs > dx)	rate	=	dx*100/xs;
	if (ys > dy)	rate	=	min(rate,dy*100/ys);

	int	xp	=	(short)pDATA[2];
	int	yp	=	(short)pDATA[3];

	xp	=	xp*rate/100;
	yp	=	yp*rate/100;

	xp	=	xp + (dx-xs*rate/100)/2;
	yp	=	yp + (dy-ys*rate/100)/2;

	PutSpriteR16[dPUT_NORMAL](x+xp,y+yp,pDATA,rate,rate,32);

	if (putshadow)	PutShadow(x+xp,y+yp,index,rate,rate);

	return	rate;
}

int
cSPRITE::FitPutColor(int x,int y,int dx,int dy,int index,WORD color)
{
	if (index	>=	Count)	return 0;
	int xs=0,ys=0,rate=100;

	if (p8DATA)
	{
		BYTE *pDATA	=	p8DATA+spriteOFFSET[index];

		xs	=	*(WORD *)(pDATA  );
		ys	=	*(WORD *)(pDATA+2);

		if (xs > dx)	rate	=	dx*100/xs;
		if (ys > dy)	rate	=	min(rate,dy*100/ys);

		int	xp	=	*(short *)(pDATA+4  );//(limitX-xs*rate/100)/2;
		int	yp	=	*(short *)(pDATA+6  );//(limitY-xs*rate/100)/2;

		xp	=	xp*rate/100;
		yp	=	yp*rate/100;

		xp	=	xp + (dx-xs*rate/100)/2;
		yp	=	yp + (dy-ys*rate/100)/2;

		PutColor(x+xp,y+yp,index,color,rate,rate);
		return	rate;
	}

	WORD *pDATA	=	p16DATA+spriteOFFSET[index];

	xs	=	pDATA[0];
	ys	=	pDATA[1];

	if (xs > dx)	rate	=	dx*100/xs;
	if (ys > dy)	rate	=	min(rate,dy*100/ys);

	int	xp	=	(short)pDATA[2];
	int	yp	=	(short)pDATA[3];

	xp	=	xp*rate/100;
	yp	=	yp*rate/100;

	xp	=	xp + (dx-xs*rate/100)/2;
	yp	=	yp + (dy-ys*rate/100)/2;

	PutColor(x+xp,y+yp,index,color,rate,rate);

	return	rate;
}


void
cSPRITE::PutLayer(int x,int y,int index,WORD color,int xrate,int yrate,int effect,int alpha,int flip)
{	if (effect	<=	dPUT_NORMAL)	alpha	=	effect;
	if (index	>=	Count)	return;

#ifdef dDEBUG_MODE
	if (!pLAYER)
	{	MSGOUT("Error in cSPRITE::PutLayer - pointer pLAYER are valid");
		return;
	}
#endif
	if (flip)	cDRAW::fPutLayerR(x,y,color,pLAYER+layerOFFSET[index],xrate,yrate);
	else		cDRAW::PutLayerR(x,y,color,pLAYER+layerOFFSET[index],xrate,yrate);
}

void
cSPRITE::PutShadow(int x,int y,int index,int xrate,int yrate,int flip)
{
	if (index	>=	Count)	return;
#ifdef dDEBUG_MODE
	if (!pSHADOW)
	{	MSGOUT("Error in cSPRITE::PutShadow - pointer pSHADOW are valid");
		return;
	}
#endif

	if (shadowOFFSET[index]	>=	shadowOFFSET[Count])	return;

	if(flip)	cDRAW::fPutShadowR(x,y,pSHADOW+shadowOFFSET[index],xrate,yrate);
	else		cDRAW::PutShadowR(x,y,pSHADOW+shadowOFFSET[index],xrate,yrate);
}

/********************************************************************
	퀵 로드 스프라이트(리얼타임으로 불러서 찍는다.)
********************************************************************/

cSPRITEQ::cSPRITEQ()
		:cSPRITE()
{	pFILE	=	NULL;
}

cSPRITEQ::~cSPRITEQ()
{	KILL(pFILE);
	Reset();
}

BOOL
cSPRITEQ::Load(char *fn,BOOL loadpalette,cNUX *nux)
{	cFILE *file;

	if (nux)	file=	nux->Open(fn);
	else
	{	file	=	new cFILE();
		if (!file->Open(fn,"rb"))	KILL(file);
	}

	if (!file)	return MSGOUT("Error in cSPRITEQ::Load - '%s' file not found",fn);

	file->Read(&SDHEADER,sizeof(SDHEADER));
#ifdef dDEBUG_MODE
	if (strcmp(SDHEADER.Reg,REGSDHEADER)!=0 && strcmp(SDHEADER.Reg,REGSDHEADER2)!=0)
		return MSGOUT("Error in cSPRITEQ::Load - '%s' file header not match",FName);
#endif

	BPP		=	SDHEADER.BPP;
	bSHADOW	=	SDHEADER.bShadow;
	bLAYER	=	SDHEADER.bOutline;
	Count	=	SDHEADER.ImageCount;

	BOOL result =	Load(file,loadpalette);

	if (nux)	pFILE	=	file;
	else
	{	KILL(file);
		strcpy(FName,fn);
	}

	return result;
}

BOOL
cSPRITEQ::Load(cFILE *file,BOOL loadpalette)
{	Reset();

	bLOADEDPLT	=	loadpalette;
	m_iDataSize	=	0;

	if (BPP==8)
	{	if (loadpalette)
		{	pPLT		=	_new(WORD,256,"cSPRITEQ::pPLT");
			file->Read(pPLT,512);
		}
		else	file->Seek(512,SEEK_CUR);
	}

	//	여기부터 스프라이트 데이터..
	spriteOFFSET	=	_new(int,Count+1,"cSPRITEQ::spriteOFFSET");

	file->Read(spriteOFFSET,sizeof(int)*(Count+1));
	SpriteLocate	=	file->Locate();

	if (BPP==8)
	{	file->Seek(spriteOFFSET[Count],SEEK_CUR);
		m_iDataSize	+=	spriteOFFSET[Count];

	}
	else
	{	file->Seek(spriteOFFSET[Count]*2,SEEK_CUR);
		m_iDataSize	+=	spriteOFFSET[Count]*2;
	}


	//	그림자 불러오기
	if (bSHADOW)
	{
		shadowOFFSET	=	_new(int,Count+1,"cSPRITEQ::shadowOFFSET");
		file->Read(shadowOFFSET,sizeof(int),Count+1);

		ShadowLocate	=	file->Locate();
		file->Seek(shadowOFFSET[Count],SEEK_CUR);
		m_iDataSize		+=	shadowOFFSET[Count];
	}

	//	외곽선 불러오기
	layerOFFSET		=	_new(int,Count+1,"cSPRITEQ::layerOFFEST");
	file->Read(layerOFFSET,sizeof(int)*(Count+1));

	LayerLocate		=	file->Locate();
	file->Seek(layerOFFSET[Count],SEEK_CUR);

	return TRUE;
}

BOOL
cSPRITEQ::Put(int x,int y,int index,int xrate,int yrate,int effect,int alpha,int flip)
{	if (effect	<	dPUT_NORMAL)	alpha	=	effect;
	if (index	>=	Count)	return FALSE;

#ifdef dDEBUG_MODE
	if (!spriteOFFSET)	return MSGOUT("Error in cSPRITEQ::Put - data's are not initialized");
#endif
	cFILE	*file;

	if (pFILE)	file	=	pFILE;
	else
	{	file	=	new cFILE();
		file->Open(FName,"rb");
	}

#ifdef dDEBUG_MODE
	if (!file) return MSGOUT("Error in cSPRITEQ::Put - '%s' file not found",FName);
#endif

	DWORD size	=	spriteOFFSET[index+1]-spriteOFFSET[index];

	file->Seek(SpriteLocate+spriteOFFSET[index],SEEK_SET);

#ifdef dDEBUG_MODE
	if (size > dMAX_IMAGE_SIZE)
	{	if (!pFILE) KILL(file);
		return MSGOUT("Error in cSPRITEQ::Put - '%s' file index Image size is over[%dbyte]",FName,size);
	}
#endif

	if (BPP	==	8)
	{	file->Read(tempIMAGE,size);
		PutSpriteR8[effect+flip](x,y,tempIMAGE,pPLT,xrate,yrate,alpha);
	}
	else
	{	file->Read(tempIMAGE,size*2);
		PutSpriteR16[effect+flip](x,y,(WORD *)tempIMAGE,xrate,yrate,alpha);
	}

	if (!pFILE) KILL(file);

	return TRUE;
}

BOOL
cSPRITEQ::PutLayer(int x,int y,int index,WORD color,int xrate,int yrate,int effect,int alpha,int flip)
{	if (effect	<	dPUT_NORMAL)	alpha	=	effect;
	if (index	>=	Count)	return FALSE;

#ifdef dDEBUG_MODE
	if (!layerOFFSET)	return MSGOUT("Error in cSPRITEQ::Put - data's are not initialized");
#endif
	cFILE	*file;

	if (pFILE)	file	=	pFILE;
	else
	{	file	=	new cFILE();
		file->Open(FName,"rb");
	}

#ifdef dDEBUG_MODE
	if (!file) return MSGOUT("Error in cSPRITEQ::PutLayer - '%s' file not found",FName);
#endif

	DWORD size	=	layerOFFSET[index+1]-layerOFFSET[index];

	file->Seek(LayerLocate+layerOFFSET[index],SEEK_SET);

#ifdef dDEBUG_MODE
	if (size > dMAX_IMAGE_SIZE)
	{	if (!pFILE) KILL(file);
		return MSGOUT("Error in cSPRITEQ::PutLayer - '%s' file index Image size is over[%dbyte]",FName,size);
	}
#endif

	file->Read(tempIMAGE,size);
	if (flip)	cDRAW::PutLayerR(x,y,color,tempIMAGE,xrate,yrate);
	else		cDRAW::fPutLayerR(x,y,color,tempIMAGE,xrate,yrate);

	if (!pFILE) KILL(file);

	return TRUE;
}

BOOL
cSPRITEQ::PutShadow(int x,int y,int index,int xrate,int yrate,int flip)
{
	if (index	>=	Count)	return FALSE;

#ifdef dDEBUG_MODE
	if (!shadowOFFSET)	return MSGOUT("Error in cSPRITEQ::Put - data's are not initialized");
#endif
	cFILE	*file;

	if (pFILE)	file	=	pFILE;
	else
	{	file	=	new cFILE();
		file->Open(FName,"rb");
	}

#ifdef dDEBUG_MODE
	if (!file) return MSGOUT("Error in cSPRITEQ::PutShadow - '%s' file not found",FName);
#endif

	DWORD size	=	shadowOFFSET[index+1]-shadowOFFSET[index];

	file->Seek(ShadowLocate+shadowOFFSET[index],SEEK_SET);

#ifdef dDEBUG_MODE
	if (size > dMAX_IMAGE_SIZE)
	{	if (!pFILE) KILL(file);
		return MSGOUT("Error in cSPRITEQ::PutShadow - '%s' file index Image size is over[%dbyte]",FName,size);
	}
#endif

	file->Read(tempIMAGE,size);

	if (flip)	cDRAW::PutShadowR(x,y,tempIMAGE,xrate,yrate);
	else		cDRAW::fPutShadowR(x,y,tempIMAGE,xrate,yrate);

	if (!pFILE) KILL(file);

	return TRUE;
}
/********************************************************************
	에니메이션 스프라이트 데이터
********************************************************************/
cANMDATA::cANMDATA()
{	m_pReleasePos	=	NULL;
	m_pPos			=	NULL;
	m_pSprite		=	NULL;
	m_pIsEvent		=	NULL;
	m_pBackFrame	=	NULL;

	m_iFrameCount	=	0;
	m_iDirectCount	=	0;
	m_isMCU		=	FALSE;
}

cANMDATA::~cANMDATA()
{	Reset();
}

void
cANMDATA::Reset()
{	if (!m_isMCU)
	{	_free(m_pPos);
		_free(m_pSprite);
		_free(m_pBackFrame);
		_free(m_pIsEvent);
		_free(m_pReleasePos);
	}
	m_pPos			=	NULL;
	m_pSprite		=	NULL;
	m_pBackFrame	=	NULL;
	m_pIsEvent		=	NULL;
	m_pReleasePos	=	NULL;

	m_isMCU		=	FALSE;
}

BOOL
cANMDATA::Init(cFILE *file)
{	Reset();

	file->Read(&m_iFrameCount	,4);

	if (m_iFrameCount > 0)
	{
		file->Read(&m_iAnmType		,8*4);

		m_pIsEvent		=	_new(BYTE,m_iFrameCount*m_iDirectCount,"cANMDATA::m_pIsEvent");
		m_pSprite		=	_new(WORD,m_iFrameCount*m_iDirectCount,"cANMDATA::m_pSprite");
		m_pBackFrame	=	_new(BYTE,m_iFrameCount*m_iDirectCount,"cANMDATA::m_pBackFrame");

		file->Read(m_pIsEvent	,m_iFrameCount);					//	이벤트 정보
		file->Read(m_pSprite	,m_iFrameCount*m_iDirectCount,2);		//	스프라이트 정보
		file->Read(m_pBackFrame,m_iFrameCount*m_iDirectCount,1);		//	백 프레임

		if (m_isRELEASE)	//	릴리즈 포인트
		{	m_pReleasePos	=	_new(POINTS,m_iDirectCount,"cANMDATA::m_pReleasePos");
			if (!m_pReleasePos)	return FALSE;

			file->Read(m_pReleasePos	,m_iDirectCount,4);
		}

		if (m_isREFITFRAME)//	프레임 보정 위치
		{	m_pPos	=	_new(POINTS,m_iFrameCount*m_iDirectCount,"cANMDATA::m_pPos");
			if (!m_pPos)	return FALSE;

			file->Read(m_pPos	,m_iFrameCount*m_iDirectCount,4);
		}
	}

	return TRUE;
}

BOOL
cANMDATA::IsBack(int direct,int frame,BOOL bflip)
{	return	m_pBackFrame[FlipDirect[bflip][m_iAnmType][direct]*m_iFrameCount+frame];
}

int
cANMDATA::GetSprite(int direct,int frame,BOOL bflip)
{	return	m_pSprite[FlipDirect[bflip][m_iAnmType][direct]*m_iFrameCount+frame];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////	에니메이션 스프라이트 데이터
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cANM::cANM()
	:cSPRITE()
{	m_pANM		=	NULL;
	m_iAnmCount	=	0	;
	m_isFLIP		=	FALSE;
}

cANM::~cANM()
{	Reset();
}

void
cANM::Reset()
{	cSPRITE::Reset();
	
	pKILL(m_pANM);
}

BOOL
cANM::Load(char *fn,BOOL loadpalette,cNUX *nux)
{	cFILE	*file;

	if (nux)	file	=	nux->Open(fn);
	else
	{	file	=	new cFILE();
		if (!file->Open(fn,"rb"))	KILL(file);
	}

	if (!file)	return ERRMSG("Error in cANM::Load - '%s' file not found",fn);

	file->Read(&SADHEADER,sizeof(SADHEADER));

	if	(strcmp(SADHEADER.Reg,REGSADHEADER)!=0 &&strcmp(SADHEADER.Reg,REGSADHEADER2)!=0)
		return ERRMSG("Error in cANM::Load - header not match!!");

	m_iAnmCount		=	SADHEADER.AnmCount;
	m_iAlpha		=	SADHEADER.byAlpha;
	if (m_iAlpha	==	0)	m_iAlpha	=	dPUT_NORMAL;
	else					m_iAlpha--;

	m_isFLIP		=	SADHEADER.bHalf;
	BPP				=	SADHEADER.BPP;
	bSHADOW			=	SADHEADER.bShadow;
	bLAYER			=	SADHEADER.bOutline;
	cSPRITE::Count	=	SADHEADER.ImageCount;

	BOOL result		=	Load(file,loadpalette);

	KILL(file);

	return result;
}

BOOL
cANM::Load(cFILE *file,BOOL loadpalette)
{	int i;

	Reset();	//	몽땅 초기화

	//	스프라이트 데이터 로딩

	cSPRITE::Load(file,loadpalette);

	//	에니메이션 데이터 로딩
	m_pANM	=	new cANMDATA [m_iAnmCount];

	file->Read(&m_iMoveOval		,4);
	file->Read(&m_CrashBox		,16);
	file->Read(&m_SelectBox		,16);
	file->Read(&m_iKind			,4);		//	데이터 타입
	file->Read(&m_RefitPos		,8);
	file->Read(&m_iCrashSize	,4);
	file->Read(&m_iDefaultAttack,4);
	file->Read(&m_iDefaultMagic	,4);

	for (i=0;i<m_iAnmCount;i++)	m_pANM[i].Init(file);

	return TRUE;
}

void
cANM::Put(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int effect,int alpha)
{	int		fliped	=	IsFlip[m_isFLIP][m_pANM[anm].m_iDirectCount][direct];
	int		index	=	m_pANM[anm].GetSprite(direct,frame,m_isFLIP);

	y	-=	m_RefitPos.y;
	x	-=	m_RefitPos.x;

	cSPRITE::Put(x,y,index,xrate,yrate,effect,alpha,fliped);
}

void
cANM::PutShadow(int x,int y,int anm,int direct,int frame,int xrate,int yrate)
{
	TEST(!pSHADOW,ERRMSG("Error in cANM::PutShadow - pointer pSHADOW are valid"));

	int		index	=	m_pANM[anm].GetSprite(direct,frame,FALSE);

	y	-=	m_RefitPos.y;
	x	-=	m_RefitPos.x;

	cDRAW::PutShadow(x,y,pSHADOW+shadowOFFSET[index]);
}

void
cANM::PutLayer(int x,int y,int anm,int direct,int frame,WORD color,int xrate,int yrate,int effect,int alpha)
{
	TEST(!pLAYER,ERRMSG("Error in cANM::PutLayer - pointer m_pLAYER are valid"));

	int		fliped	=	IsFlip[m_isFLIP][m_pANM[anm].m_iDirectCount][direct];
	int		index	=	m_pANM[anm].GetSprite(direct,frame,m_isFLIP);

	y	-=	m_RefitPos.y;
	x	-=	m_RefitPos.x;

	if (effect	<	dPUT_NORMAL)	alpha	=	effect;

	cSPRITE::PutLayer(x,y,index,color,xrate,yrate,effect,alpha,fliped);
}

void
cANM::PutReg(int x,int y,int anm,int direct,int frame,int xrate,int yrate)
{
	TEST(!p8DATA && !p16DATA,ERRMSG("Error in cANM::PutReg - pointer m_pLAYER are valid"));
	TEST(anm	>= m_iAnmCount,ERRMSG("Error in cANM::PutReg - valid anm"));
	TEST(direct	>= m_pANM[anm].m_iDirectCount || frame >= m_pANM[anm].m_iFrameCount,ERRMSG("Error in cANM::PutReg - valid direct or frame"));

	int		fliped	=	IsFlip[m_isFLIP][m_pANM[anm].m_iDirectCount][direct];
	int		index	=	m_pANM[anm].GetSprite(direct,frame,m_isFLIP);
	int		alpha	=	m_pANM[anm].m_iALPHA;

	y	-=	m_RefitPos.y;
	x	-=	m_RefitPos.x;

	if (m_pANM[anm].m_isREFITFRAME)
	{	x	-=	m_pANM[anm].m_pPos[direct*m_pANM[anm].m_iFrameCount + frame].x;
		y	-=	m_pANM[anm].m_pPos[direct*m_pANM[anm].m_iFrameCount + frame].y;
	}

	cSPRITE::Put(x,y,index,xrate,yrate,alpha,alpha,fliped);
}

void
cANM::GetRect(int anm,int direct,int frame,cRECT *rect)
{	int	index	=	m_pANM[anm].GetSprite(direct,frame,m_isFLIP);

	rect->x1	-=	m_RefitPos.x;
	rect->y1	-=	m_RefitPos.y;

	cSPRITE::GetRect(index,rect);
}

BOOL
cANM::IsBack(int anm,int direct,int frame)
{	return m_pANM[anm].IsBack(direct,frame,m_isFLIP);
}

BOOL
cANM::IsFront(int anm,int direct,int frame)
{	return (1-m_pANM[anm].IsBack(direct,frame,m_isFLIP));
}

int
cANM::GetLinkAnm(int anm)
{	return m_pANM[anm].m_iLinkAnm;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////	에니메이션 스프라이트 데이터
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cANMQ::cANMQ()
	:cSPRITEQ()
{	m_pANM		=	NULL;
	m_iAnmCount	=	0	;
	m_isFLIP		=	FALSE;
}

cANMQ::~cANMQ()
{	Reset();
}

void
cANMQ::Reset()
{	cSPRITE::Reset();

	_free(layerOFFSET);
	pKILL(m_pANM);
}

BOOL
cANMQ::isGetTriggerFrame()
{
	for (int i=0;i<m_iAnmCount;i++)
		for (int frame=0;frame<m_pANM[i].m_iFrameCount;frame++)
			if (m_pANM[i].m_pIsEvent[frame])	return	TRUE;
	
	return	FALSE;
}

BOOL
cANMQ::Load(char *fn,BOOL loadpalette,cNUX *nux)
{	
	cFILE	*file	=	NULL;

	if	(nux)
		file	=	nux->Open(fn);
	else
	{	
		file	=	new cFILE();

		if	(!file->Open(fn,"rb"))
			KILL(file);
	}

	if	(!file)
		return FALSE;//ERRMSG("Error in cANM::Load - '%s' file not found",fn);

	strncpy(FName,fn,sizeof(FName));

	file->Read(&SADHEADER,sizeof(SADHEADER));

	if	(strcmp(SADHEADER.Reg,REGSADHEADER)!=0 && strcmp(SADHEADER.Reg,REGSADHEADER2)!=0)
		return ERRMSG("Error in cANM::Load - header not match!!");

	m_iAnmCount			=	SADHEADER.AnmCount;
	m_iAlpha			=	SADHEADER.byAlpha;

	if	(m_iAlpha	==	0)
		m_iAlpha	=	dPUT_NORMAL;
	else
		m_iAlpha--;

	cSPRITEQ::Count		=	SADHEADER.ImageCount;
	BPP					=	SADHEADER.BPP;
	bSHADOW				=	SADHEADER.bShadow;

	BOOL result			=	Load(file,loadpalette);

	pFILE				=	file;

	return result;
}

BOOL
cANMQ::Load(cFILE *file,BOOL loadpalette)
{
	int i;

	Reset();	//	몽땅 초기화

	//	스프라이트 데이터 로딩

	cSPRITEQ::Load(file,loadpalette);

	//	에니메이션 데이터 로딩
	m_pANM	=	new cANMDATA [m_iAnmCount];

	file->Read(&m_iMoveOval	,4);
	file->Read(&m_CrashBox	,16);
	file->Read(&m_SelectBox	,16);
	file->Read(&m_iKind		,4);		//	데이터 타입
	file->Read(&m_RefitPos	,8);
	file->Read(&m_iCrashSize,4);
	file->Read(&m_iDefaultAttack,4);
	file->Read(&m_iDefaultMagic,4);

	for (i=0;i<m_iAnmCount;i++)	m_pANM[i].Init(file);

	return TRUE;
}

BOOL
cANMQ::Read(cANM_INDEX *lpAnmInfo,int iReadCount)
{
	cFILE	file;

	if (!file.Open(FName,"rb")) return FALSE;

	int		*pSpriteOffset,*pShadowOffset,*pLayerOffset;
	int		iTotalSpriteSize	=	0,iTotalShadowSize	=	0,iTotalLayerSize	=	0;
	int		iSpriteSize		,iShadowSize	,iLayerSize;
	int		iSpriteLocate	,iShadowLocate	,iLayerLocate;
	int		i,iImageIndex;
	
	pSpriteOffset	=	new int [iReadCount];
	if (shadowOFFSET)
		pShadowOffset	=	new int [iReadCount];
	pLayerOffset	=	new int [iReadCount];

	for (i=0;i<iReadCount;i++)
	{
		iImageIndex		=	GetSprite(lpAnmInfo[i].m_iAnm,lpAnmInfo[i].m_iDirect,lpAnmInfo[i].m_iFrame);

		iSpriteSize		=	spriteOFFSET[iImageIndex+1]	-	spriteOFFSET[iImageIndex];
		iTotalSpriteSize+=	iSpriteSize;

		if (shadowOFFSET)
		{
			iShadowSize		=	shadowOFFSET[iImageIndex+1]	-	shadowOFFSET[iImageIndex];
			iTotalShadowSize+=	iShadowSize;
		}

		iLayerSize		=	layerOFFSET[iImageIndex+1]	-	layerOFFSET[iImageIndex];
		iTotalLayerSize+=	iLayerSize;
	}

	_free(p8DATA);
	_free(pSHADOW);
	_free(pLAYER);

	p8DATA	=	_new(BYTE,iTotalSpriteSize,_ms("cANMQ::%s's p8DATA",FName));
	if (shadowOFFSET)
		pSHADOW	=	_new(BYTE,iTotalShadowSize,_ms("cANMQ::%s's pSHADOW",FName));
	pLAYER	=	_new(BYTE,iTotalLayerSize,_ms("cANMQ::%s's pLAYER",FName));

	iSpriteLocate	=	0;
	iShadowLocate	=	0;
	iLayerLocate	=	0;

	for (i=0;i<iReadCount;i++)
	{
		iImageIndex		=	GetSprite(lpAnmInfo[i].m_iAnm,lpAnmInfo[i].m_iDirect,lpAnmInfo[i].m_iFrame);

		iSpriteSize		=	spriteOFFSET[iImageIndex+1]	-	spriteOFFSET[iImageIndex];
		file.Seek(spriteOFFSET[iImageIndex] + SpriteLocate,SEEK_SET);
		file.Read(p8DATA+iSpriteLocate,iSpriteSize);
		pSpriteOffset[i]=	iSpriteLocate;
		iSpriteLocate	+=	iSpriteSize;

		if (shadowOFFSET)
		{
			iShadowSize		=	shadowOFFSET[iImageIndex+1]	-	shadowOFFSET[iImageIndex];
			file.Seek(shadowOFFSET[iImageIndex] + ShadowLocate,SEEK_SET);
			file.Read(pSHADOW+iShadowLocate,iShadowSize);
			pShadowOffset[i]=	iShadowLocate;
			iShadowLocate	+=	iShadowSize;
		}

		iLayerSize		=	layerOFFSET[iImageIndex+1]	-	layerOFFSET[iImageIndex];
		file.Seek(layerOFFSET[iImageIndex] + LayerLocate,SEEK_SET);
		file.Read(pLAYER+iLayerLocate,iLayerSize);
		pLayerOffset[i]	=	iLayerLocate;
		iLayerLocate	+=	iLayerSize;

	}

	_free(spriteOFFSET);
	_free(shadowOFFSET);
	_free(layerOFFSET);


	spriteOFFSET	=	_new(int,iReadCount+1,_ms("cANMQ::%s's spriteOFFSET",FName));
	if (pShadowOffset)	shadowOFFSET	=	_new(int,iReadCount+1,_ms("cANMQ::%s's shadowOFFSET",FName));
	layerOFFSET		=	_new(int,iReadCount+1,_ms("cANMQ::%s's shadowOFFSET",FName));

	Count	=	iReadCount;
	memcpy(spriteOFFSET	,pSpriteOffset,4*Count);
	if (pShadowOffset)	memcpy(shadowOFFSET	,pShadowOffset,4*Count);
	memcpy(layerOFFSET	,pLayerOffset,4*Count);

	pKILL(pSpriteOffset);
	pKILL(pShadowOffset);
	pKILL(pLayerOffset);

	spriteOFFSET[Count]	=	iTotalSpriteSize;
	shadowOFFSET[Count]	=	iTotalShadowSize;
	layerOFFSET[Count]	=	iTotalLayerSize;

	return	TRUE;
}

void
cANMQ::Put(int x,int y,int index,int _sx,int _sy)
{
	y	-=	m_RefitPos.y;
	x	-=	m_RefitPos.x;

	cSPRITE::PutShadow(x,y,index,_sx,_sy,FALSE);
	cSPRITE::Put(x,y,index,_sx,_sy,dPUT_NORMAL,32,FALSE);
}

void
cANMQ::PutHalf(int x,int y,int index,int _sx,int _sy)
{
	y	-=	m_RefitPos.y;
	x	-=	m_RefitPos.x;

	cSPRITE::Put(x,y,index,_sx,_sy,dPUT_HALF_BLENDING,32,FALSE);
}

void
cANMQ::PutRed(int x,int y,int index,int _sx,int _sy)
{
	y	-=	m_RefitPos.y;
	x	-=	m_RefitPos.x;

	int		i;
	WORD	plt[256];

	memcpy(plt,pPLT,512);

	for (i=0;i<256;i++)
	{
		int	red		=	pPLT[i]>>10;
		int	green	=	(pPLT[i]>>5)&31;
		int	blue	=	pPLT[i]&31;

		red			=	red+(31	- red	)*70/100;
		green		=	green*70/100;
		blue		=	blue*70/100;
		pPLT[i]		=	(red << 10) + (green << 5) + blue;
	}

	cSPRITE::PutShadow(x,y,index,_sx,_sy,FALSE);
	cSPRITE::Put(x,y,index,_sx,_sy,dPUT_NORMAL,32,FALSE);

	memcpy(pPLT,plt,512);
}

void
cANMQ::PutLayer(int x,int y,int index,WORD color,int _sx,int _sy)
{
	y	-=	m_RefitPos.y;
	x	-=	m_RefitPos.x;

	cSPRITE::PutLayer(x,y,index,color,_sx,_sy);
}

int
cANMQ::FitPut(int x,int y,int dx,int dy,int index,BOOL putshadow)
{
	return	cSPRITE::FitPut(x,y,dx,dy,index,putshadow);
}


/********************************************************************
	cIMAGE
********************************************************************/

BOOL
cIMAGE::Put(int x,int y)
{	if (p16IMAGE)
	{	cDRAW::PutImage(x,y,p16IMAGE);
		return TRUE;
	}

	if (p8IMAGE)
	{	cDRAW::PutImage(x,y,p8IMAGE,pPLT);
		return TRUE;
	}

	return TRUE;
}

cIMAGE*
cIMAGE::LOAD(char *fn,BOOL change)
{
	void	*temp;

	if (stricmp(_exportExt(fn),"tga")==0)	temp	=	new cTGA();
	else
	if (stricmp(_exportExt(fn),"bmp")==0)	temp	=	new cBMP();
	else
	if (stricmp(_exportExt(fn),"pcx")==0)	temp	=	new cPCX();
	else	return NULL;

	if (!((cIMAGE *)temp)->Load(fn,change))	return FALSE;

	return	(cIMAGE *)temp;
}

/********************************************************************
	오브젝트/건물 이미지 데이터
********************************************************************/

cOBJ_ANM_INFO::cOBJ_ANM_INFO()
{	Pos			=	NULL;
	Sprite		=	NULL;
	FrameCount	=	0;
	Alpha		=	dPUT_NORMAL;
}

cOBJ_ANM_INFO::~cOBJ_ANM_INFO()
{	Reset();
}

void
cOBJ_ANM_INFO::Reset()
{	_free(Pos);
	_free(Sprite);
}

BOOL
cOBJ_ANM_INFO::Init(cFILE *file)
{	Reset();

	file->Read(&FrameCount	,2);

	if (FrameCount > 0)
	{
		Sprite	=	_new(WORD,FrameCount,"cOBJ_ANM_INFO::Sprite");
		Pos		=	_new(POINTS,FrameCount,"cOBJ_ANM_INFO::Pos");

		file->Read(&Alpha	,1);
		file->Read(Pos		,4,FrameCount);
		file->Read(Sprite	,2,FrameCount);
	}

	return TRUE;
}

int
cOBJ_ANM_INFO::GetSprite(int frame)
{	return Sprite[frame];
}


cOBJ_ANM::cOBJ_ANM()
{	
	pANM	=	NULL;
	pBLOCK	=	NULL;
	pCRASH	=	NULL;
	pOVERLAPBLOCK	=	NULL;
}


cOBJ_ANM::~cOBJ_ANM()
{	
	Reset();
}

void
cOBJ_ANM::Reset()
{	
	cSPRITE::Reset();
	_free(pBLOCK);
	_free(pCRASH);
	_free(pOVERLAPBLOCK);

	pKILL(pANM);
}

BOOL
cOBJ_ANM::Load(char *fn,BOOL loadpalette,cNUX *nux)
{	cFILE *file;

	strncpy(Name,_exportName(fn),sizeof(Name)-2);

	if (nux)	file=	nux->Open(fn);
	else
	{	file	=	new cFILE();
		if (!file->Open(fn,"rb"))	KILL(file);
	}

	if (!file)	return MSGOUT("Error in cOBJ_ANM::Load - '%s' file not found",fn);

	file->Read(&SADHEADER,sizeof(SADHEADER));

#ifdef dDEBUG_MODE
	if	(strcmp(SADHEADER.Reg,REGRODHEADER)	!=	0	&&	strcmp(SADHEADER.Reg,REGRBDHEADER)	!=	0 &&
		strcmp(SADHEADER.Reg,REGRODHEADER2)	!=	0	&&	strcmp(SADHEADER.Reg,REGRBDHEADER2)	!=	0)
		return MSGOUT("Error in cOBJ_ANM::Load - header not match!!");
#endif

	AnmCount		=	SADHEADER.AnmCount;
	m_iAlpha		=	SADHEADER.byAlpha;
	if (m_iAlpha	==	0)	m_iAlpha	=	dPUT_NORMAL;
	else					m_iAlpha--;
	bSTATIC			=	1-SADHEADER.CrashType;
	BPP				=	SADHEADER.BPP;
	bSHADOW			=	SADHEADER.bShadow;
	bLAYER			=	TRUE;
	cSPRITE::Count	=	SADHEADER.ImageCount;

	if (SADHEADER.BPP!=8)	return MSGOUT("Error in cOBJ_ANM::Load - object \"%s\"'s bpp not 8bit",fn);

	BOOL result		= Load(file,loadpalette);

	KILL(file);

	return result;
}

BOOL
cOBJ_ANM::Load(cFILE *file,BOOL loadpalette)
{	int i;

	Reset();							//	초기화

	cSPRITE::Load(file,loadpalette);	//	스프라이트 데이터 로딩

	//	에니메이션 데이터..

	pANM	=	new	cOBJ_ANM_INFO [AnmCount];

	file->Read(&Attr,4);

	for (i=0;i<AnmCount;i++)
	{	pANM[i].Init(file);

		if (i==0 && pANM[i].FrameCount == 0)	cMSG::Test(file->Locate());
	}

	int	 size;

	file->Read(&size,4);
	size	-=	8;

	//	충돌 박스
	pCRASH	= _new(BYTE,size,"cOBJ_ANM::pCRASH");

	file->Read(pCRASH,8);
	file->Read(pCRASH,size);

	//	블럭
	file->Read(&size,4);
	size	-=	8;
	pBLOCK	= _new(BYTE,size,"cOBJ_ANM::pBLOCK");

	file->Read(pBLOCK,8);
	file->Read(pBLOCK,size);

	//	오버랩 블럭
	file->Read(&size,4);
	size	-=	8;
	
	_free(pOVERLAPBLOCK);

	pOVERLAPBLOCK	= _new(BYTE,size,"cOBJ_ANM::pOVERLAPBLOCK");

	file->Read(pOVERLAPBLOCK,8);
	file->Read(pOVERLAPBLOCK,size);

	file->Read(&Crash,sizeof(Crash));
	file->Read(&m_iObjectType,4);


	
	return TRUE;
}

void
cOBJ_ANM::Put(int index,int x,int y)
{
	cSPRITE::Put(x,y,index,100,100,m_iAlpha);
}

void
cOBJ_ANM::PutScale(int index,int x,int y,int _iScale)
{
	cSPRITE::Put(x,y,index,_iScale,_iScale,m_iAlpha);
}

void
cOBJ_ANM::PutLayer(int index,int x,int y)
{
	if (!pLAYER)	return;

	cDRAW::PutLayer(x,y,_LTGREEN,pLAYER+layerOFFSET[index]);
}

void
cOBJ_ANM::PutLayerScale(int index,int x,int y,int _iScale)
{
	if (!pLAYER)	return;

	cDRAW::PutLayerR(x,y,_LTGREEN,pLAYER+layerOFFSET[index],_iScale,_iScale);
}

void
cOBJ_ANM::PutLayer(int index,int x,int y,WORD color)
{
	if (!pLAYER)	return;

	cDRAW::PutLayer(x,y,color,pLAYER+layerOFFSET[index]);
}

void
cOBJ_ANM::PutRed(int index,int x,int y)
{
	int		i;
	WORD	plt[256];

	memcpy(plt,pPLT,512);

	for (i=0;i<256;i++)
	{
		int	red		=	pPLT[i]>>10;
		int	green	=	(pPLT[i]>>5)&31;
		int	blue	=	pPLT[i]&31;

		red			+=	(31	- red	)*70/100;
		pPLT[i]	=		(red << 10) + (green << 5) + blue;
	}

	cSPRITE::Put(x,y,index,100,100);

	memcpy(pPLT,plt,512);
}

void
cOBJ_ANM::PutShadow(int index,int x,int y)
{	if (!pSHADOW)	return;

	cSPRITE::PutShadow(x,y,index);
}

void
cOBJ_ANM::PutShadowScale(int index,int x,int y,int _iScale)
{	if (!pSHADOW)	return;

	cSPRITE::PutShadow(x,y,index,_iScale,_iScale);
}


void
cOBJ_ANM::Put(int x,int y)
{
	cSPRITE::Put(x,y,0,100,100,m_iAlpha);

	if (m_iObjectType	==	dOBJECT_TYPE_ANM)
		cSPRITE::Put(x,y,1,100,100,m_iAlpha);
}

void
cOBJ_ANM::PutScale(int x,int y,int _iScale)
{
	cSPRITE::Put(x,y,0,_iScale,_iScale,m_iAlpha);

	if (m_iObjectType	==	dOBJECT_TYPE_ANM)	cSPRITE::Put(x,y,1,_iScale,_iScale,m_iAlpha);
}

void
cOBJ_ANM::PutRed(int x,int y)
{
	int		i;
	WORD	plt[256];

	memcpy(plt,pPLT,512);

	for (i=0;i<256;i++)
	{
		int	red		=	pPLT[i]>>10;
		int	green	=	(pPLT[i]>>5)&31;
		int	blue	=	pPLT[i]&31;

		red			+=	(31	- red	)*70/100;
		pPLT[i]	=		(red << 10) + (green << 5) + blue;
	}

	cSPRITE::Put(x,y,0);

	if (m_iObjectType	==	dOBJECT_TYPE_ANM)	cSPRITE::Put(x,y,1);
/*
	for (i=0;i<AnmCount;i++)
	{	if (pANM[i].FrameCount>0)
		{	int		index	=	pANM[i].Sprite[0];

			cSPRITE::Put(x,y,index,100,100,pANM[i].Alpha);
		}
	}
*/
	memcpy(pPLT,plt,512);
}

void
cOBJ_ANM::PutLayer(int x,int y)
{	if (!pLAYER)	return;

	cDRAW::PutLayer(x,y,_LTGREEN,pLAYER);
}

void
cOBJ_ANM::PutLayerScale(int x,int y,int _iScale)
{	if (!pLAYER)	return;

	cDRAW::PutLayerR(x,y,_LTGREEN,pLAYER,_iScale,_iScale);
}

void
cOBJ_ANM::PutShadow(int x,int y)
{	if (!pSHADOW)	return;

	cSPRITE::PutShadow(x,y,0);
}

void
cOBJ_ANM::PutShadowScale(int x,int y,int _iScale)
{	if (!pSHADOW)	return;

	cSPRITE::PutShadow(x,y,0,_iScale,_iScale);
}

int
cOBJ_ANM::FitPut(int x,int y,int limitX,int limitY)
{	int xs=0,ys=0,i,rate=100;

	BYTE *pDATA	=	Get8(0);

	xs	=	*(WORD *)(pDATA  );
	ys	=	*(WORD *)(pDATA+2);

	if (xs > limitX)	rate	=	limitX*100/xs;
	if (ys > limitY)	rate	=	min(rate,limitY*100/ys);

	int	xp	=	*(short *)(pDATA+4  );//(limitX-xs*rate/100)/2;
	int	yp	=	*(short *)(pDATA+6  );//(limitY-xs*rate/100)/2;

	xp	=	xp*rate/100;
	yp	=	yp*rate/100;

	xp	=	xp + (limitX-xs*rate/100)/2;
	yp	=	yp + (limitY-ys*rate/100)/2;

	for (i=0;i<AnmCount;i++)
	{	if (pANM[i].FrameCount>0)
		{	int		index	=	pANM[i].Sprite[0];

			cSPRITE::Put(x+xp,y+yp,index,rate,rate,pANM[i].Alpha);
		}
	}

	return	rate;
}

int
cOBJ_ANM::FitPut(int index,int x,int y,int limitX,int limitY)
{
	int xs=0,ys=0,rate=100;

	BYTE *pDATA	=	Get8(index);

	xs	=	*(WORD *)(pDATA  );
	ys	=	*(WORD *)(pDATA+2);

	if (xs > limitX)	rate	=	limitX*100/xs;
	if (ys > limitY)	rate	=	min(rate,limitY*100/ys);

	int	xp	=	*(short *)(pDATA+4  );//(limitX-xs*rate/100)/2;
	int	yp	=	*(short *)(pDATA+6  );//(limitY-xs*rate/100)/2;

	xp	=	xp*rate/100;
	yp	=	yp*rate/100;

	xp	=	xp + (limitX-xs*rate/100)/2;
	yp	=	yp + (limitY-ys*rate/100)/2;

	cSPRITE::Put(x+xp,y+yp,index,rate,rate);

	return	rate;
}





//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃이미지 클래스... 그런데... 왜 SMI지.. -_-;;
cSMI::cSMI()
{
	m_iCount	=	0;	//	오프셋,이미지 수
	m_pOffset	=	NULL;
	m_pImage	=	NULL;	//	이미지
	m_pFile		=	NULL;
	m_lpMcu		=	NULL;//	Mcu
}

//
//	디스트럭터
cSMI::~cSMI()
{
	close();
}	//	cSMI::~cSMI()

//
//	닫기
void
cSMI::close()
{
	KILL(m_pFile);

	pKILL(m_pImage);
	pKILL(m_pOffset);

	m_iCount	=	0;	//	오프셋,이미지 수
	m_lpMcu		=	NULL;//	Mcu
	m_pOffset	=	NULL;
	m_pImage	=	NULL;	//	이미지
}	//	cSMI::close()

//
//	걍 로드
BOOL
cSMI::load(char *fn)
{
	cFILE	*pFile;

	pFile	=	new cFILE();
	if (!pFile->Open(fn,"rb"))	KILL(pFile);

	if (!pFile)	return ERRMSG("cSMI::load","'%s' file not found",fn);

	close();

	pFile->Seek(64,SEEK_SET);
	pFile->Read(&m_iCount,4);

	m_pOffset	=	new int [m_iCount+1];

	pFile->Read(m_pOffset,(m_iCount+1)*sizeof(int));

	m_pImage	=	new WORD [m_pOffset[m_iCount]];
	pFile->Read(m_pImage,m_pOffset[m_iCount],2);

//	for (int i=0;i<m_iCount;i++)	CONVERTIMAGE(m_pImage+m_pOffset[i]);

	KILL(pFile);

	return	TRUE;
}	//	load(char *fn,cNUX *_lpNux=NULL,cMCU *_lpMcu=NULL)

void
cSMI::put(int _iIndex,int _iX,int _iY,int _iAlpha)
{
	if (_iIndex >= m_iCount)	return;
//	CRY(_iIndex >= m_iCount,"Error in cSMI::put - _iIndex >= m_iCount");

	cDRAW::PutImage(_iX,_iY,m_pImage+m_pOffset[_iIndex]);
}	//	cSMI::put(int _iIndex,int _iX=0,int _iY=0,int _iAlpha=32)


WORD*
cSMI::getImage(int _iIndex)
{
	if (_iIndex >= m_iCount)	return NULL;

	return	m_pImage+m_pOffset[_iIndex];
}