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
	WORD		AnmCount;
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
		{d8DIRECT_NE,d8DIRECT_E,d8DIRECT_SE,d8DIRECT_S,d8DIRECT_SW,d8DIRECT_W,d8DIRECT_NW,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{d16DIRECT_N,d16DIRECT_NNE,d16DIRECT_NE,d16DIRECT_ENE,d16DIRECT_E,d16DIRECT_ESE,d16DIRECT_SE,d16DIRECT_SSE,
		 d16DIRECT_S,d16DIRECT_SSW,d16DIRECT_SW,d16DIRECT_WSW,d16DIRECT_W,d16DIRECT_WNW,d16DIRECT_NW,d16DIRECT_NNW},
	},
	{	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		//	플립
		{d2DIRECT_E,d2DIRECT_E		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{d2DIRECT_N,d2DIRECT_S		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{d4DIRECT_N,d4DIRECT_E,d4DIRECT_S,d4DIRECT_E		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{d8DIRECT_NE,d8DIRECT_E,d8DIRECT_SE,d8DIRECT_S,d8DIRECT_SE,d8DIRECT_E,d8DIRECT_NE		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
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
	pPLT	=	NULL;
	Count	=	0;
}

cSPRITE::~cSPRITE()
{	pKILL(p16DATA);
	pKILL(p8DATA);
	pKILL(pLAYER);
	pKILL(spriteOFFSET);
	pKILL(layerOFFSET);
	pKILL(pPLT);
}

BOOL
cSPRITE::Load(char *fn,BOOL loadpalette,cNUX *file)
{	FILE *fp;

	if (file)	fp	=	file->Open(fn);
	else		fp	=	fopen(fn,"rb");

	if (!fp)	return MSGOUT("Error in cSPRITE::Load - '%s' file not found",fn);

	BOOL result =	Load(fp,loadpalette);

	if (!file)	fclose(fp);

	return result;
}

BOOL
cSPRITE::Load(FILE *fp,BOOL loadpalette)
{	fread(&SDHEADER,sizeof(SDHEADER),1,fp);

#ifdef dDEBUG_MODE
//	if (strcmp(SDHEADER.Reg,REGSDHEADER)!=0)	return MSGOUT("Error in cSPRITE::Load - header not match");
#endif

	pKILL(spriteOFFSET);
	pKILL(layerOFFSET);
	pKILL(p16DATA);
	pKILL(p8DATA);
	pKILL(pLAYER);
	pKILL(pPLT);

	if (SDHEADER.BPP==8)
	{	if (loadpalette)
		{	pPLT			=	new	WORD [256];
			fread(pPLT,256,2,fp);
		}
		else	fseek(fp,768,SEEK_CUR);

		BPP				=	8;
	}	else BPP		=	16;

	//	여기부터 스프라이트 데이터..
	Count			=	SDHEADER.ImageCount;
	spriteOFFSET	=	new int [Count+1];

	fread(spriteOFFSET,sizeof(int),Count+1,fp);

	if (SDHEADER.BPP==16)
	{	p16DATA			=	new WORD [spriteOFFSET[Count]];
		fread(p16DATA,spriteOFFSET[Count],sizeof(WORD),fp);
	}
	else
	{	p8DATA			=	new BYTE [spriteOFFSET[Count]];
		fread(p8DATA,spriteOFFSET[Count],sizeof(BYTE),fp);
	}

//	외곽선 불러오기
	if (SDHEADER.bOutline)
	{	return TRUE;
		layerOFFSET		=	new int [Count+1];
		fread(layerOFFSET,sizeof(int),Count+1,fp);
		pLAYER			=	new BYTE [layerOFFSET[Count]];
		fread(pLAYER,layerOFFSET[Count],sizeof(BYTE),fp);
	}

	return TRUE;
}

void
cSPRITE::Put(int x,int y,int index,int xrate,int yrate,int effect,int alpha)
{	if (effect	<=	dPUT_NORMAL)	alpha	=	effect;

	if (p16DATA)PutSpriteR16[effect	](x,y,p16DATA+spriteOFFSET[index],xrate,yrate,alpha);
	if (p8DATA)	PutSpriteR8[effect	](x,y,p8DATA+spriteOFFSET[index],pPLT,xrate,yrate,alpha);
}

void
cSPRITE::PutFx(int x,int y,int index,int effect)
{	if (p8DATA)	PutSpriteR8[effect	](x,y,p8DATA+spriteOFFSET[index],pPLT,100,100,effect);
	if (p16DATA)PutSpriteR16[effect	](x,y,p16DATA+spriteOFFSET[index],100,100,effect);	
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

/********************************************************************
	퀵 로드 스프라이트(리얼타임으로 불러서 찍는다.)
********************************************************************/

BOOL
cSPRITEQ::Load(char *fn,BOOL loadpalette,cNUX *file)
{	FILE *fp;

	if (file)	fp	=	file->Open(fn);
	else		fp	=	fopen(fn,"rb");

	if (!fp)	return MSGOUT("Error in cSPRITEQ::Load - '%s' file not found",fn);

	BOOL result =	Load(fp,loadpalette);

	if (!file)	fclose(fp);

	pNUX	=	file;
	strcpy(FName,fn);
	return result;
}

BOOL
cSPRITEQ::Load(FILE *fp,BOOL loadpalette)
{	fread(&SDHEADER,sizeof(SDHEADER),1,fp);

#ifdef dDEBUG_MODE
	if (strcmp(SDHEADER.Reg,REGSDHEADER)!=0)	return MSGOUT("Error in cSPRITEQ::Load - '%s' file header not match",FName);
#endif

	pKILL(spriteOFFSET);
	pKILL(layerOFFSET);
	pKILL(p16DATA);
	pKILL(p8DATA);
	pKILL(pLAYER);
	pKILL(pPLT);

	if (SDHEADER.BPP==8)
	{	if (loadpalette)
		{	pPLT			=	new	WORD [256];
			fread(pPLT,256,2,fp);
		}
		else	fseek(fp,768,SEEK_CUR);

		BPP				=	8;
	}	else BPP		=	16;

	//	여기부터 스프라이트 데이터..
	Count			=	SDHEADER.ImageCount;
	spriteOFFSET	=	new int [Count+1];

	fread(spriteOFFSET,sizeof(int),Count+1,fp);
	SpriteLocate	=	ftell(fp);

//	외곽선 불러오기
	if (SDHEADER.bOutline)
	{	if (SDHEADER.BPP==8)	fseek(fp,spriteOFFSET[Count]  ,SEEK_CUR);
		else					fseek(fp,spriteOFFSET[Count]*2,SEEK_CUR);

		LayerLocate		=	ftell(fp);

		layerOFFSET		=	new int [Count+1];
		fread(layerOFFSET,sizeof(int),Count+1,fp);
	}

	return TRUE;
}

BOOL
cSPRITEQ::Put(int x,int y,int index,int xrate,int yrate,int effect,int alpha)
{	if (effect	<	dPUT_NORMAL)	alpha	=	effect;

#ifdef dDEBUG_MODE
	if (!spriteOFFSET)	return MSGOUT("Error in cSPRITE::Put - data's are not initialized");
#endif
	FILE	*fp;

	if (pNUX)	fp	=	pNUX->Open(FName);
	else		fp	=	fopen(FName,"rb");

#ifdef dDEBUG_MODE
	if (!fp) return MSGOUT("Error in cSPRITEQ::Put - '%s' file not found",FName);
#endif

	DWORD size	=	spriteOFFSET[index+1]-spriteOFFSET[index];

	fseek(fp,SpriteLocate+spriteOFFSET[index],SEEK_CUR);

#ifdef dDEBUG_MODE
	if (size > dMAX_IMAGE_SIZE)
	{	if (!pNUX) fclose(fp);
		return MSGOUT("Error in cSPRITEQ::Put - '%s' file index Image size is over[%dbyte]",FName,size);
	}
#endif

	fread(tempIMAGE,size,1,fp);

	if (BPP	==	8)	PutSpriteR8[effect](x,y,tempIMAGE,pPLT,xrate,yrate,alpha);
	else			PutSpriteR16[effect](x,y,(WORD *)tempIMAGE,xrate,yrate,alpha);

	if (!pNUX) fclose(fp);

	return TRUE;
}

/********************************************************************
	에니메이션 스프라이트 데이터
********************************************************************/
cANMDATA::cANMDATA()
{	Pos			=	NULL;
	Sprite		=	NULL;
	IsEvent		=	NULL;
	BackFrame		=	NULL;

	FrameCount	=	0;
	DirectCount	=	0;
}

cANMDATA::~cANMDATA()
{	int i;

	for (i=0;i<DirectCount;i++)
	{	pKILL(Pos[i]);
		pKILL(Sprite[i]);
		pKILL(BackFrame[i]);
	}

	pKILL(Pos);
	pKILL(Sprite);
	pKILL(IsEvent);
	pKILL(BackFrame);
}

void
cANMDATA::Init(int directCount,int frameCount)
{	int	i;

	for (i=0;i<DirectCount;i++)
	{	if (Pos		) pKILL(Pos[i]);
		if (Sprite	) pKILL(Sprite[i]);
		if (BackFrame	) pKILL(BackFrame[i]);
		
	}

	pKILL(Pos);
	pKILL(Sprite);
	pKILL(IsEvent);
	pKILL(BackFrame);

	DirectCount	=	directCount;
	FrameCount	=	frameCount;

	if (!bSTATIC)	Pos		=	new POINTS *[DirectCount];

	Sprite	=	new WORD   *[DirectCount];
	BackFrame	=	new BYTE   *[DirectCount];
	IsEvent	=	new BYTE	[FrameCount];

	for (i=0;i<directCount;i++)
	{	if (!bSTATIC)	Pos[i]		=	new POINTS	[FrameCount];
		Sprite[i]	=	new WORD	[FrameCount];
		BackFrame[i]	=	new BYTE	[FrameCount];
	}
}


/********************************************************************
	에니메이션 스프라이트 데이터
********************************************************************/

cANIMATION::cANIMATION()
{	p16DATA		=	NULL,	p8DATA		=	NULL,	spriteOFFSET	=	NULL;
	SpriteCount	=	0	;
	pLAYER		=	NULL,	layerOFFSET	=	NULL;
	pPLT		=	NULL,	pANM		=	NULL;
	pSHADOW		=	NULL,	shadowOFFSET=	NULL;
	AnmCount	=	0	,	bFLIP		=	FALSE;
}

cANIMATION::~cANIMATION()
{	pKILL(p16DATA);
	pKILL(p8DATA);
	pKILL(spriteOFFSET);

	pKILL(pLAYER);
	pKILL(layerOFFSET);

	pKILL(pSHADOW);
	pKILL(shadowOFFSET);

	pKILL(pPLT);
	pKILL(pANM);

	AnmCount	=	0;
	SpriteCount	=	0;
}

void
cANIMATION::Reset()
{	pKILL(p16DATA);
	pKILL(p8DATA);
	pKILL(spriteOFFSET);

	pKILL(pLAYER);
	pKILL(layerOFFSET);

	pKILL(pSHADOW);
	pKILL(shadowOFFSET);

	pKILL(pPLT);
	pKILL(pANM);

	AnmCount	=	0;
	SpriteCount	=	0;
}

BOOL
cANIMATION::isBack(int anm,int direct,int frame)
{	if (anm >= AnmCount) return FALSE;
	if (direct >= pANM[anm].DirectCount ) return FALSE;
	if (frame  >= pANM[anm].FrameCount ) return FALSE;
	
	return pANM[anm].BackFrame[direct][frame];
}

BOOL
cANIMATION::Load(char *fn,BOOL loadpalette,cNUX *file)
{	FILE *fp;

	fp	=	fopen(fn,"rb");

	if (!fp)	return MSGOUT("Error in cANIMATION::Load - '%s' file not found",fn);

	BOOL result = Load(fp,loadpalette);

	if (!file) fclose(fp);

	return result;
}

BOOL
cANIMATION::Load(FILE *fp,BOOL loadpalette)
{	int i,j;

	fread(&SADHEADER,sizeof(SADHEADER),1,fp);
#ifdef dDEBUG_MODE
	if (strcmp(SADHEADER.Reg,REGSADHEADER)!=0)
		return MSGOUT("Error in cANIMATION::Load - header not match!!");
#endif

	AnmCount	=	SADHEADER.AnmCount;
	CrashType	=	SADHEADER.CrashType;
	bFLIP		=	SADHEADER.bHalf;

	if (SADHEADER.BPP==8)	BPP	=	8;
	else					BPP	=	16;

	//	여기부터 스프라이트 데이터..
	pKILL(p16DATA);
	pKILL(p8DATA);
	pKILL(spriteOFFSET);

	pKILL(pSHADOW);
	pKILL(shadowOFFSET);

	pKILL(pLAYER);
	pKILL(layerOFFSET);

	pKILL(pPLT);
	pKILL(pANM);

	if (BPP	==	8)
	{	if (loadpalette)
		{	pPLT			=	new	WORD [256];
			fread(pPLT,256,2,fp);
		}
		else	fseek(fp,768,SEEK_CUR);

		BPP				=	8;
	}	else	BPP		=	16;

	SpriteCount		=	SADHEADER.ImageCount;
	spriteOFFSET	=	new int [SpriteCount+1];

	fread(spriteOFFSET,sizeof(int),SpriteCount+1,fp);

	if (BPP==16)
	{	p16DATA			=	new WORD [spriteOFFSET[SpriteCount]];
		fread(p16DATA,spriteOFFSET[SpriteCount],sizeof(WORD),fp);
	}
	else
	{	p8DATA			=	new BYTE [spriteOFFSET[SpriteCount]];
		fread(p8DATA,spriteOFFSET[SpriteCount],sizeof(BYTE),fp);
	}

//	그림자 불러오기
	if (SADHEADER.bShadow)
	{	shadowOFFSET	=	new int [SpriteCount+1];
		fread(shadowOFFSET,sizeof(int),SpriteCount+1,fp);
		pSHADOW			=	new BYTE [shadowOFFSET[SpriteCount]];
		fread(pLAYER,shadowOFFSET[SpriteCount],sizeof(BYTE),fp);
	}

//	외곽선 불러오기
	if (SADHEADER.bOutline)
	{	layerOFFSET		=	new int [SpriteCount+1];
		fread(layerOFFSET,sizeof(int),SpriteCount+1,fp);
		pLAYER			=	new BYTE [layerOFFSET[SpriteCount]];
		fread(pLAYER,layerOFFSET[SpriteCount],sizeof(BYTE),fp);
	}
	else
	{	DWORD	full;
		fseek(fp,4*SpriteCount,SEEK_CUR);
		fread(&full,1,4,fp);
		fseek(fp,full,SEEK_CUR);
	}

	//	에니메이션 데이터..

	pANM	=	new	cANMDATA	[AnmCount];

	for (i=0;i<AnmCount;i++)	//	에니
	{	fread(&pANM[i].AnmType		,sizeof(int),1,fp);
		fread(&pANM[i].DirectCount	,sizeof(int),1,fp);
		fread(&pANM[i].FrameCount	,sizeof(int),1,fp);

		if (pANM[i].FrameCount > 0)
		{	fread(&pANM[i].bSTATIC	,sizeof(BYTE),1,fp);
			
			pANM[i].Init(pANM[i].DirectCount,pANM[i].FrameCount);

			fread(&pANM[i].IsEvent	,sizeof(BYTE),pANM[i].FrameCount,fp);
			fread(&pANM[i].Alpha	,sizeof(BYTE),1,fp);

			for (j=0;j<pANM[i].DirectCount;j++)		//	방향..
			{	if (!pANM[i].bSTATIC)
					fread(&pANM[i].Pos[j][0],sizeof(cPOINT),pANM[i].FrameCount,fp);
				fread(&pANM[i].Sprite[j][0],sizeof(WORD),pANM[i].FrameCount,fp);
			}
		}
	}

	//	충돌 박스..

	switch(CrashType)
	{	case	dCRASH_NONE			:	break;
		case	dCRASH_ONE			:	break;
		case	dCRASH_PER_ANM		:	break;
		case	dCRASH_PER_IMAGE	:	break;
	}

	return TRUE;
}

int
cANIMATION::GetSprite(int anm,int direct,int frame)
{	return pANM[anm].Sprite[FlipDirect[bFLIP][pANM[anm].DirectCount][direct]][frame];
}

void
cANIMATION::PutReg(int x,int y,int anm,int direct,int frame,int rate)
{	if (!p8DATA && !p16DATA)	return;

	if (anm		>= AnmCount)	return;
	if (direct	>= pANM[anm].DirectCount || frame >= pANM[anm].FrameCount)	return;

	int		fliped	=	IsFlip[bFLIP][pANM[anm].DirectCount][direct];
	int		index	=	pANM[anm].Sprite[FlipDirect[bFLIP][pANM[anm].DirectCount][direct]][frame];
	int		alpha	=	pANM[anm].Alpha;

	if (p8DATA)	PutSpriteR8[alpha +fliped](x,y,p8DATA+spriteOFFSET[index],pPLT,rate,rate,alpha);
	if (p16DATA)PutSpriteR16[alpha+fliped](x,y,p16DATA+spriteOFFSET[index],rate,rate,alpha);
}

void
cANIMATION::Put(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int effect,int alpha)
{	int		fliped	=	IsFlip[bFLIP][pANM[anm].DirectCount][direct];
	int		index	=	pANM[anm].Sprite[FlipDirect[bFLIP][pANM[anm].DirectCount][direct]][frame];
	if (effect	<	dPUT_NORMAL)	alpha	=	effect;

	if (p8DATA)	PutSpriteR8[effect +fliped](x,y,p8DATA+spriteOFFSET[index],pPLT,xrate,yrate,alpha);
	if (p16DATA)PutSpriteR16[effect+fliped](x,y,p16DATA+spriteOFFSET[index],xrate,yrate,alpha);
}

void
cANIMATION::PutFx(int x,int y,int anm,int direct,int frame,int effect)
{	int		fliped	=	IsFlip[bFLIP][pANM[anm].DirectCount][direct];
	int		index	=	pANM[anm].Sprite[FlipDirect[bFLIP][pANM[anm].DirectCount][direct]][frame];

	if (p8DATA)	PutSpriteR8[effect +fliped](x,y,p8DATA+spriteOFFSET[index],pPLT,100,100,effect);
	if (p16DATA)PutSpriteR16[effect+fliped](x,y,p16DATA+spriteOFFSET[index],100,100,effect);
}

void
cANIMATION::Put(WORD *plt,int x,int y,int anm,int direct,int frame,int xrate,int yrate,int effect,int alpha)
{	int		fliped	=	IsFlip[bFLIP][pANM[anm].DirectCount][direct];
	int		index	=	pANM[anm].Sprite[FlipDirect[bFLIP][pANM[anm].DirectCount][direct]][frame];
	if (effect	<	dPUT_NORMAL)	alpha	=	effect;

	PutSpriteR8[effect+fliped](x,y,p8DATA+spriteOFFSET[index],plt,xrate,yrate,alpha);
}

void
cANIMATION::PutFx(WORD *plt,int x,int y,int anm,int direct,int frame,int effect)
{	int		fliped	=	IsFlip[bFLIP][pANM[anm].DirectCount][direct];
	int		index	=	pANM[anm].Sprite[FlipDirect[bFLIP][pANM[anm].DirectCount][direct]][frame];

	PutSpriteR8[effect+fliped](x,y,p8DATA+spriteOFFSET[index],plt,100,100,effect);
}

void
cANIMATION::GetRect(int anm,int direct,int frame,cRECT *rect)
{	int	index	=	pANM[anm].Sprite[FlipDirect[bFLIP][pANM[anm].DirectCount][direct]][frame];

	if (BPP==16)
	{	WORD	*pdata	=	p16DATA+spriteOFFSET[index];

		rect->x1	=	pdata[2];
		rect->y1	=	pdata[3];
		rect->x2	=	(short )pdata[0];
		rect->y2	=	(short )pdata[1];
	}
	else
	{	BYTE	*pdata	=	p8DATA+spriteOFFSET[index];

		rect->x1	=	*(short *)(pdata+4);
		rect->y1	=	*(short *)(pdata+6);
		rect->x2	=	*(WORD  *)(pdata  );
		rect->y2	=	*(WORD  *)(pdata+2);
	}
}


/********************************************************************
	퀵 로드 에니메이션 스프라이트(리얼타임으로 불러서 찍는다.)
********************************************************************/

BOOL
cANMQ::Load(char *fn,BOOL loadpalette,cNUX *file)
{	FILE *fp;

	fp	=	fopen(fn,"rb");

	if (!fp)	return MSGOUT("Error in cANIMATION::Load - '%s' file not found",fn);

	BOOL result = Load(fp,loadpalette);

	if (!file) fclose(fp);

	strcpy(FName,fn);
	pNUX	=	file;

	return result;
}

BOOL
cANMQ::Load(FILE *fp,BOOL loadpalette)
{	int i,j;

	fread(&SADHEADER,sizeof(SADHEADER),1,fp);
#ifdef dDEBUG_MODE
	if (strcmp(SADHEADER.Reg,REGSADHEADER)!=0)
		return MSGOUT("Error in cANMQ::Load -'%s' file header not match!!",FName);
#endif

	AnmCount	=	SADHEADER.AnmCount;
	CrashType	=	SADHEADER.CrashType;
	bFLIP		=	SADHEADER.bHalf;

	BPP			=	SADHEADER.BPP;

	if (bFLIP)	return MSGOUT("Error in cANMQ::Load -'%s' file is flip data!!",FName);

	//	여기부터 스프라이트 데이터..
	pKILL(p16DATA);
	pKILL(p8DATA);
	pKILL(spriteOFFSET);

	pKILL(pSHADOW);
	pKILL(shadowOFFSET);

	pKILL(pLAYER);
	pKILL(layerOFFSET);

	pKILL(pPLT);
	pKILL(pANM);

	if (BPP==8)
	{	if (loadpalette)
		{	pPLT			=	new	WORD [256];
			fread(pPLT,256,2,fp);
		}
		else	fseek(fp,512,SEEK_CUR);
	}

	SpriteCount		=	SADHEADER.ImageCount;
	spriteOFFSET	=	new int [SpriteCount+1];

	fread(spriteOFFSET,sizeof(int),SpriteCount+1,fp);

	SpriteLocate	=	ftell(fp);

	if (BPP==16)	fseek(fp,spriteOFFSET[SpriteCount]*2,SEEK_CUR);
	else			fseek(fp,spriteOFFSET[SpriteCount]  ,SEEK_CUR);

//	그림자 불러오기
	if (SADHEADER.bShadow)
	{	shadowOFFSET	=	new int [SpriteCount+1];
		fread(shadowOFFSET,sizeof(int),SpriteCount+1,fp);
		ShadowLocate	=	ftell(fp);
		fseek(fp,shadowOFFSET[SpriteCount],SEEK_CUR);
	}

//	외곽선 불러오기
	if (SADHEADER.bOutline)
	{	layerOFFSET		=	new int [SpriteCount+1];
		fread(layerOFFSET,sizeof(int),SpriteCount+1,fp);
		LayerLocate		=	ftell(fp);
		fseek(fp,layerOFFSET[SpriteCount],SEEK_CUR);
	}
	else
	{	DWORD	full;
		fseek(fp,4*SpriteCount,SEEK_CUR);
		fread(&full,1,4,fp);
		fseek(fp,full,SEEK_CUR);
	}

	//	에니메이션 데이터..

	pANM	=	new	cANMDATA	[AnmCount];

	for (i=0;i<AnmCount;i++)	//	에니
	{	fread(&pANM[i].AnmType		,sizeof(int),1,fp);
		fread(&pANM[i].DirectCount	,sizeof(int),1,fp);
		fread(&pANM[i].FrameCount	,sizeof(int),1,fp);

		if (pANM[i].FrameCount > 0)
		{	pANM[i].Init(pANM[i].DirectCount,pANM[i].FrameCount);

			fread(&pANM[i].IsEvent	,sizeof(BYTE),pANM[i].FrameCount,fp);
			fread(&pANM[i].bSTATIC	,sizeof(BYTE),1,fp);
			fread(&pANM[i].Alpha	,sizeof(BYTE),1,fp);

			for (j=0;j<pANM[i].DirectCount;j++)		//	방향..
			{	if (!pANM[i].bSTATIC)
					fread(&pANM[i].Pos[j][0],sizeof(cPOINT),pANM[i].FrameCount,fp);
				fread(&pANM[i].Sprite[j][0],sizeof(WORD),pANM[i].FrameCount,fp);
			}
		}
	}

	//	충돌 박스..

	switch(CrashType)
	{	case	dCRASH_NONE			:	break;
		case	dCRASH_ONE			:	break;
		case	dCRASH_PER_ANM		:	break;
		case	dCRASH_PER_IMAGE	:	break;
	}

	return TRUE;
}

BOOL
cANMQ::Put(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int effect,int alpha)
{	if (effect	<	dPUT_NORMAL)	alpha	=	effect;

#ifdef dDEBUG_MODE
	if (!spriteOFFSET)
		return MSGOUT("Error in cANMQ::Put - data's are not initialized");
#endif
	FILE *fp;

	if (pNUX)	fp	=	pNUX->Open(FName);
	else		fp	=	fopen(FName,"rb");

	if (!fp)	return MSGOUT("Error in cANMQ::Put - '%s' File not Found",FName);

	int index	=	pANM[anm].Sprite[direct][frame];

	fseek(fp,SpriteLocate+spriteOFFSET[index],SEEK_CUR);

	DWORD	size	=	spriteOFFSET[index+1]-spriteOFFSET[index];

#ifdef dDEBUG_MODE
	if (size > dMAX_IMAGE_SIZE)
		return MSGOUT("Error in cANMQ::Put - '%s' file index Image size is over[%dbyte]",FName,size);
#endif

	fread(tempIMAGE,size,1,fp);

	if (BPP	==	8)	PutSpriteR8[effect](x,y,tempIMAGE,pPLT,xrate,yrate,alpha);
	else			PutSpriteR16[effect](x,y,(WORD *)tempIMAGE,xrate,yrate,alpha);

	if (!pNUX)	fclose(fp);

	return FALSE;
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

	return FALSE;
}
