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
		{0,0,0,0,11,11,11,11},
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
{	FREE(p16DATA);
	FREE(p8DATA);
	FREE(pLAYER);
	FREE(pSHADOW);
	FREE(spriteOFFSET);
	FREE(layerOFFSET);
	FREE(shadowOFFSET);
	if (bLOADEDPLT)	FREE(pPLT);
}

BOOL
cSPRITE::Load(char *fn,BOOL loadpalette,cNUX *nux)
{	cFILE *file;

	if (nux)	file=	nux->Open(fn);
	else
	{	file=	new cFILE();
		if (!file->Open(fn,"rb"))	KILL(file);
	}

	if (!file)	return MSGOUT("Error in cSPRITE::Load - '%s' file not found",fn);

	file->Read(&SDHEADER,sizeof(SDHEADER));

#ifdef dDEBUG_MODE
	if (strcmp(SDHEADER.Reg,REGSDHEADER)!=0)	return MSGOUT("Error in cSPRITE::Load - header not match");
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
		{	pPLT			=	NEW(256,WORD);
			file->Read(pPLT,512);
		}
		else	file->Seek(512,SEEK_CUR);
	}
	//	스프라이트 데이터..
	spriteOFFSET	=	NEW(Count+1,int);

	file->Read(spriteOFFSET,(Count+1),4);

	if (BPP==16)
	{	p16DATA			=	NEW(spriteOFFSET[Count],WORD);
		file->Read(p16DATA	,spriteOFFSET[Count],2);
	}
	else
	{	p8DATA			=	NEW(spriteOFFSET[Count],BYTE);
		file->Read(p8DATA	,spriteOFFSET[Count]);
	}

	//	그림자 불러오기
	if (bSHADOW)
	{	shadowOFFSET	=	NEW(Count+1,int);
		file->Read(shadowOFFSET,Count+1,4);
		if(shadowOFFSET[Count]>0)
		{	pSHADOW			=	NEW(shadowOFFSET[Count],BYTE);
			file->Read(pSHADOW,shadowOFFSET[Count]);
		}
	}

	//	외곽선 불러오기
	if (bLAYER)
	{	layerOFFSET		=	NEW(Count+1,int);
		file->Read(layerOFFSET,Count+1,4);
		pLAYER			=	NEW(layerOFFSET[Count],BYTE);
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
	{	FREE(pPLT);
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

	if (p8DATA)	PutSpriteR8[effect	+flip](x,y,p8DATA+spriteOFFSET[index],pPLT,xrate,yrate,alpha);
	if (p16DATA)PutSpriteR16[effect	+flip](x,y,p16DATA+spriteOFFSET[index],xrate,yrate,alpha);
}

void
cSPRITE::PutLayer(int x,int y,int index,WORD color,int xrate,int yrate,int effect,int alpha,int flip)
{	if (effect	<=	dPUT_NORMAL)	alpha	=	effect;

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
#ifdef dDEBUG_MODE
	if (!pSHADOW)
	{	MSGOUT("Error in cSPRITE::PutShadow - pointer pSHADOW are valid");
		return;
	}
#endif
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
	if (strcmp(SDHEADER.Reg,REGSDHEADER)!=0)	return MSGOUT("Error in cSPRITEQ::Load - '%s' file header not match",FName);
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

	bLOADEDPLT=	loadpalette;


	if (BPP==8)
	{	if (loadpalette)
		{	pPLT		=	NEW(256,WORD);
			file->Read(pPLT,512);
		}
		else	file->Seek(512,SEEK_CUR);
	}

	//	여기부터 스프라이트 데이터..
	spriteOFFSET	=	NEW(Count+1,int);

	file->Read(spriteOFFSET,sizeof(int)*(Count+1));
	SpriteLocate	=	file->Locate();

	if (BPP==8)	file->Seek(spriteOFFSET[Count],SEEK_CUR);
	else		file->Seek(spriteOFFSET[Count]*2,SEEK_CUR);
	
	//	그림자 불러오기
	if (bSHADOW)
	{	ShadowLocate	=	file->Locate();

		shadowOFFSET	=	NEW(Count+1,int);
		file->Read(shadowOFFSET,sizeof(int),Count+1);
	}

	//	외곽선 불러오기
	if (bLAYER)
	{	LayerLocate		=	file->Locate();

		layerOFFSET		=	NEW(Count+1,int);
		file->Read(layerOFFSET,sizeof(int)*(Count+1));
	}

	return TRUE;
}

BOOL
cSPRITEQ::Put(int x,int y,int index,int xrate,int yrate,int effect,int alpha,int flip)
{	if (effect	<	dPUT_NORMAL)	alpha	=	effect;

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
{	Pos			=	NULL;
	Sprite		=	NULL;
	IsEvent		=	NULL;
	BackFrame	=	NULL;

	FrameCount	=	0;
	DirectCount	=	0;
}

cANMDATA::~cANMDATA()
{	Reset();
}

void
cANMDATA::Reset()
{	FREE(Pos);
	FREE(Sprite);
	FREE(BackFrame);
	FREE(IsEvent);
}

BOOL
cANMDATA::Init(cFILE *file)
{	Reset();

	file->Read(&FrameCount	,4);

	AnmType	=	0;

	if (FrameCount > 0)
	{	file->Read(&AnmType		,8*4);
//		RefitPos	=	NEW(DirectCount,POINTS);						//	방향별 보정 위치
//		file->Read(RefitPos,sizeof(POINTS),DirectCount);

		//	AnmType	,DirectCount	,ALPHA			,FPS
		//	PPS		,bRELEASE		,bREFITFRAME	,LinkAnm

		IsEvent		=	NEW(FrameCount*DirectCount,BYTE);
		Sprite		=	NEW(FrameCount*DirectCount,WORD);
		BackFrame	=	NEW(FrameCount*DirectCount,BYTE);

		file->Read(IsEvent	,FrameCount);					//	이벤트 정보
		file->Read(Sprite	,FrameCount*DirectCount,2);		//	스프라이트 정보
		file->Read(BackFrame,FrameCount*DirectCount,1);		//	백 프레임

		if (bRELEASE)	//	릴리즈 포인트
		{	ReleasePos	=	NEW(DirectCount,POINTS);
			if (!ReleasePos)	return FALSE;

			file->Read(ReleasePos	,DirectCount,4);
		}

		if (bREFITFRAME)//	프레임 보정 위치
		{	Pos			=	NEW(FrameCount*DirectCount,POINTS);
			if (!Pos)	return FALSE;

			file->Read(Pos	,FrameCount*DirectCount,4);
		}
	}

	return TRUE;
}

BOOL
cANMDATA::IsBack(int direct,int frame,BOOL bflip)
{	if (frame	>= FrameCount	)	return FALSE;
	if (direct	>= DirectCount	)	return FALSE;

	return	BackFrame[FlipDirect[bflip][AnmType][direct]*FrameCount+frame];
}

int
cANMDATA::GetSprite(int direct,int frame,BOOL bflip)
{	if (frame	>= FrameCount	)	return 0xffff;
	if (direct	>= DirectCount	)	return 0xffff;

	return	Sprite[FlipDirect[bflip][AnmType][direct]*FrameCount+frame];
}

/********************************************************************
	에니메이션 스프라이트 데이터
********************************************************************/

cANIMATION::cANIMATION()
	:cSPRITE()
{	pANM		=	NULL;
	AnmCount	=	0	;
	bFLIP		=	FALSE;
}

cANIMATION::~cANIMATION()
{	Reset();
}

void
cANIMATION::Reset()
{	cSPRITE::Reset();

	pKILL(pANM);
}

BOOL
cANIMATION::isBack(int anm,int direct,int frame)
{	if (!pANM) return FALSE;
	return pANM[anm].IsBack(direct,frame,bFLIP);
}

BOOL
cANIMATION::Load(char *fn,BOOL loadpalette,cNUX *nux)
{	cFILE	*file;

	if (nux)	file	=	nux->Open(fn);
	else
	{	file	=	new cFILE();
		if (!file->Open(fn,"rb"))	KILL(file);
	}

	if (!file)	return MSGOUT("Error in cANIMATION::Load - '%s' file not found",fn);

	file->Read(&SADHEADER,sizeof(SADHEADER));
#ifdef dDEBUG_MODE
	if (strcmp(SADHEADER.Reg,REGSADHEADER)!=0)
		return MSGOUT("Error in cANIMATION::Load - header not match!!");
#endif

	AnmCount		=	SADHEADER.AnmCount;
	CrashType		=	SADHEADER.CrashType;
	bFLIP			=	SADHEADER.bHalf;
	BPP				=	SADHEADER.BPP;
	bSHADOW			=	SADHEADER.bShadow;
	bLAYER			=	SADHEADER.bOutline;
	cSPRITE::Count	=	SADHEADER.ImageCount;

	BOOL result		=	Load(file,loadpalette);

	KILL(file);

	strcpy(Name,fn);

	return result;
}

BOOL
cANIMATION::Load(cFILE *file,BOOL loadpalette)
{	int i;

	Reset();	//	몽땅 초기화
	//	스프라이트 데이터 로딩

	cSPRITE::Load(file,loadpalette);

	//	에니메이션 데이터 로딩
	pANM	=	new	cANMDATA [AnmCount];

	file->Read(&MoveOval,4);
	file->Read(&CrashBox,16);
	file->Read(&SelectBox,16);
	file->Read(&Kind,4);
	file->Read(&pANM[0].RefitPos,sizeof(cPOINT));

	file->Read(&CrashSize,4);
	file->Read(&m_iDefaultAttack,4);
	file->Read(&m_iDefaultMagic,4);

//		RefitPos	=	NEW(DirectCount,POINTS);						//	방향별 보정 위치
//		file->Read(RefitPos,sizeof(POINTS),DirectCount);
	for (i=0;i<AnmCount;i++)	pANM[i].Init(file);

	//	충돌 박스..

	switch(CrashType)
	{	case	dCRASH_NONE			:	break;
		case	dCRASH_ONE			:	break;
		case	dCRASH_PER_ANM		:	break;
		case	dCRASH_PER_IMAGE	:	break;
	}

	return TRUE;
}

void
cANIMATION::Put(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int effect,int alpha)
{
	if (anm >= AnmCount)	return;
	int		fliped	=	IsFlip[bFLIP][pANM[anm].DirectCount][direct];
	int		index	=	pANM[anm].GetSprite(direct,frame,bFLIP);

	if (index >= cSPRITE::Count) return;

	cSPRITE::Put(x,y,index,xrate,yrate,effect,alpha,fliped);
}

void
cANIMATION::PutShadow(int x,int y,int anm,int direct,int frame,int xrate,int yrate)
{	if (!pSHADOW) return;

	if (anm >= AnmCount)	return;

	int		fliped	=	IsFlip[bFLIP][pANM[anm].DirectCount][direct];
	int		index	=	pANM[anm].GetSprite(direct,frame,bFLIP);

	if (index >= cSPRITE::Count) return;

	cSPRITE::PutShadow(x,y,index,xrate,yrate,fliped);
}

void
cANIMATION::PutLayer(int x,int y,int anm,int direct,int frame,WORD color,int xrate,int yrate,int effect,int alpha)
{
	if (anm >= AnmCount)	return;
	int		fliped	=	IsFlip[bFLIP][pANM[anm].DirectCount][direct];
	int		index	=	pANM[anm].GetSprite(direct,frame,bFLIP);

	cSPRITE::PutLayer(x,y,index,color,xrate,yrate,effect,alpha,fliped);
}

void
cANIMATION::PutReg(int x,int y,int anm,int direct,int frame,int xrate,int yrate)
{	if (!p8DATA && !p16DATA)	return;
	if (anm >= AnmCount)	return;

	if (anm		>= AnmCount				)	return;
	if (direct	>= pANM[anm].DirectCount)	return;
	if (frame	>= pANM[anm].FrameCount	)	return;


	int		fliped	=	IsFlip[bFLIP][pANM[anm].DirectCount][direct];
	int		index	=	pANM[anm].GetSprite(direct,frame,bFLIP);
	int		alpha	=	pANM[anm].ALPHA;

	if (index >= cSPRITE::Count) return;
	cSPRITE::Put(x,y,index,xrate,yrate,alpha,alpha,fliped);
}

void
cANIMATION::GetRect(int anm,int direct,int frame,cRECT *rect)
{	int	index	=	pANM[anm].GetSprite(direct,frame,bFLIP);
	if (anm >= AnmCount)	return;

	if (index >= cSPRITE::Count) return;
	cSPRITE::GetRect(index,rect);
}


/********************************************************************
	퀵 로드 에니메이션 스프라이트(리얼타임으로 불러서 찍는다.)
********************************************************************/

cANMQ::cANMQ()
	:cSPRITEQ()
{	pANM		=	NULL;
	AnmCount	=	0	;
}

cANMQ::~cANMQ()
{	Reset();
}

void
cANMQ::Reset()
{	cSPRITE::Reset();

	pKILL(pANM);
}

BOOL
cANMQ::Load(char *fn,BOOL loadpalette,cNUX *nux)
{	cFILE	*file;

	if (nux)	file	=	nux->Open(fn);
	else
	{	file	=	new cFILE();
		if (!file->Open(fn,"rb"))	KILL(file);
	}

	if (!file)	return MSGOUT("Error in cANMQ::Load - '%s' file not found",fn);

	file->Read(&SADHEADER,sizeof(SADHEADER));
#ifdef dDEBUG_MODE
	if (strcmp(SADHEADER.Reg,REGSADHEADER)!=0)
		return MSGOUT("Error in cANMQ::Load - header not match!!");
	if (SADHEADER.bHalf)
		return MSGOUT("Error in cANMQ::Load -'%s' file is flip data!!",FName);
#endif

	AnmCount		=	SADHEADER.AnmCount;
	CrashType		=	SADHEADER.CrashType;
	BPP				=	SADHEADER.BPP;
	bSHADOW			=	SADHEADER.bShadow;
	bLAYER			=	SADHEADER.bOutline;
	cSPRITE::Count	=	SADHEADER.ImageCount;

	BOOL result		=	Load(file,loadpalette);

	if (!nux)	pFILE	=	file;
	else
	{	KILL(file);
		strcpy(FName,fn);
	}

	return result;
}

BOOL
cANMQ::Load(cFILE *file,BOOL loadpalette)
{	int i;

	//	여기부터 스프라이트 데이터..
	Reset();

	cSPRITEQ::Load(file,loadpalette);

	//	에니메이션 데이터..

	pANM	=	new	cANMDATA [AnmCount];

	for (i=0;i<AnmCount;i++)	pANM[i].Init(file);

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
{	int index	=	pANM[anm].GetSprite(direct,frame,0);

	cSPRITEQ::Put(x,y,index,xrate,yrate,effect,alpha);

	return TRUE;
}

void
cANMQ::PutReg(int x,int y,int anm,int direct,int frame,int xrate,int yrate)
{	int		index	=	pANM[anm].GetSprite(direct,frame,0);
	int		alpha	=	pANM[anm].ALPHA;

	cSPRITEQ::Put(x,y,index,xrate,yrate,alpha,alpha,0);
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
{	FREE(Pos);
	FREE(Sprite);
}

BOOL
cOBJ_ANM_INFO::Init(cFILE *file)
{	Reset();

	file->Read(&FrameCount	,2);

	if (FrameCount > 0)
	{	Sprite	=	new WORD	[FrameCount];
		Pos		=	new POINTS	[FrameCount];

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
{	pANM	=	NULL;
	pBLOCK	=	NULL;
	pCRASH	=	NULL;
}


cOBJ_ANM::~cOBJ_ANM()
{	Reset();
}

void
cOBJ_ANM::Reset()
{	cSPRITE::Reset();
	FREE(pBLOCK);
	FREE(pCRASH);

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
	if (strcmp(SADHEADER.Reg,REGRODHEADER)!=0)
		if (strcmp(SADHEADER.Reg,REGRBDHEADER)!=0)
			return MSGOUT("Error in cOBJ_ANM::Load - header not match!!");
#endif

	AnmCount		=	SADHEADER.AnmCount;
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
	pCRASH	= NEW(size,BYTE);

	file->Read(pCRASH,8);
	file->Read(pCRASH,size);

	//	블럭
	file->Read(&size,4);
	size	-=	8;
	pBLOCK	= NEW(size,BYTE);

	file->Read(pBLOCK,8);
	file->Read(pBLOCK,size);

	file->Read(&Crash,sizeof(Crash));

	return TRUE;
}

void
cOBJ_ANM::Put(int x,int y)
{	for (int i=0;i<AnmCount;i++)
	{	if (pANM[i].FrameCount>0)
		{	int		index	=	pANM[i].Sprite[0];

			cSPRITE::Put(x,y,index,100,100,pANM[i].Alpha);
		}
	}
}

void
cOBJ_ANM::PutLayer(int x,int y)
{	if (!pLAYER)	return;

	cDRAW::PutLayer(x,y,_LTGREEN,pLAYER);
}

void
cOBJ_ANM::PutShadow(int x,int y)
{	if (!pSHADOW)	return;

	cSPRITE::PutShadow(x,y,0);
}

void
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
}
