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
#include "cTRANS.H"

char	*regANISET			=	"하늘 에니메이션 셋팅 데이터";
char	*regSDHEADER		=	"하늘 스프라이트 데이터";
char	*regSADHEADER		=	"하늘 스프라이트 에니메이션 데이터";
char	*regSMIHEADER		=	"이미지 데이터";

int		l_iVersion			=	101;

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

WORD	cANM::s_iBPP		=	0;
WORD	cANM::s_awPlt[256];
cANM	ANM[dMAX_ANM];
int		MoveOval			=	2;									//	2:1 좌표계
int		g_iCrashSize		=	20;

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


//	정적 멤버
BOOL
cIMG::AddImage(char *_strName,WORD *data,int xs1,int ys1)
{
	if (Count && cANM::s_iBPP != 16)
		return	ERRMSG("이미지 추가 에러!","기존 이미지가 %dbit 입니다.",cANM::s_iBPP);

	cANM::s_iBPP=	16;

	bVIRGIN		=	FALSE;

	cIMG		*img;

	img			=	new cIMG();
	img->xs		=	xs1;
	img->ys		=	ys1;

	strncpy(img->m_strName,_strName,sizeof(img->m_strName)-1);

	BigXS	=	max(img->xs,BigXS);
	BigYS	=	max(img->ys,BigYS);

	img->m_p16Image	=	new WORD [xs1*ys1+2];
	img->Size		=	xs1*ys1+2;

	memcpy(img->m_p16Image,data,(xs1*ys1+2)*2);

	bVIRGIN	=	FALSE;

	return Add(img);
}

//	정적 멤버
BOOL
cIMG::AddImage(char *_strName,BYTE *data,int xs1,int ys1)
{
	if (Count && cANM::s_iBPP != 8)
		return	ERRMSG("이미지 추가 에러!","기존 이미지가 %dbit 입니다.",cANM::s_iBPP);

	cANM::s_iBPP=	8;

	bVIRGIN		=	FALSE;

	cIMG		*img;

	img			=	new cIMG();
	img->xs		=	xs1;
	img->ys		=	ys1;
	strncpy(img->m_strName,_strName,sizeof(img->m_strName)-1);

	BigXS	=	max(img->xs,BigXS);
	BigYS	=	max(img->ys,BigYS);

	img->m_p8Image	=	new BYTE [xs1*ys1+4];
	img->Size		=	xs1*ys1+4;

	memcpy(img->m_p8Image,data,xs1*ys1+4);

	bVIRGIN	=	FALSE;

	return Add(img);
}

BOOL
cIMG::Add(cIMG *img)
{
	if (!pHEAD)
	{
		pHEAD		=	new cIMG();
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

	bVIRGIN	=	FALSE;

	return TRUE;
}

BOOL
cIMG::Del(cIMG *img)
{
	if (Count <= 0) return FALSE;
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

	bVIRGIN	=	FALSE;

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

	bVIRGIN	=	FALSE;

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
{	if (Count <= 0) return;

	BigXS	=	0,BigYS	=	0;

	cIMG *img=pHEAD->pNEXT;

	for (int i=0;i<Count;i++)
	{	if (img->xs > BigXS) BigXS = img->xs;
		if (img->ys > BigYS) BigYS = img->ys;

		img=img->pNEXT;
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
	g_iCrashSize=	20;
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


BOOL
cANM::SaveSmi(char *fn)
{
	cFILE	file;

	file.Open(fn,"wb");

	file.Write(regSMIHEADER,60);
	file.Write(&s_iBPP,4);
	file.Write(&cIMG::Count,4);

	cIMG	*lpImg	=	cIMG::Get(0);
	int		i,iSize	=	0;

	file.Write(&iSize,sizeof(int));

	if (s_iBPP	==	16)
	{
		for (i=0;i<cIMG::Count;i++)
		{
			iSize	+=	(lpImg->Size + 16);	//	데이터,사이즈,이름
			file.Write(&iSize,sizeof(int));

			lpImg	=	lpImg->pNEXT;
		}
	}
	else
	{
		for (i=0;i<cIMG::Count;i++)
		{
			iSize	+=	(lpImg->Size + 32);	//	데이터,사이즈,이름
			file.Write(&iSize,sizeof(int));

			lpImg	=	lpImg->pNEXT;
		}
	}

	lpImg	=	cIMG::Get(0);

	for (i=0;i<cIMG::Count;i++)
	{
		if (s_iBPP	==	16)
			file.Write(lpImg->m_p16Image,lpImg->Size,2);
		else
			file.Write(lpImg->m_p8Image,lpImg->Size);

		file.Write(lpImg->m_strName,32);
		lpImg	=	lpImg->pNEXT;
	}

	if (s_iBPP	==	8)
	{
		for (i=0;i<256;i++)
			s_awPlt[i]	=	cTRANS::RGB555ToRGB565(s_awPlt[i]);

		file.Write(s_awPlt,sizeof(s_awPlt));

		for (i=0;i<256;i++)
			s_awPlt[i]	=	cTRANS::RGB565ToRGB555(s_awPlt[i]);
	}

	file.Close();

	bVIRGIN	=	TRUE;
	
	return TRUE;
}

BOOL
cANM::LoadSmi(char *fn)
{
	cFILE	file;
	int		iCount;
	int		i,iSize	=	0;

	if (!file.Open(fn,"rb"))	
		return cMSG::Error("이미지 화일 로딩 에러!!","'%s' 화일을 찾을 수 없어요.-o-",fn);

	file.Seek(60,SEEK_SET);
	file.Read(&s_iBPP,4);
	file.Read(&iCount,4);
	file.Seek((iCount+1)*4,SEEK_CUR);

	if (s_iBPP	!=	16 && s_iBPP	!=	8)
		s_iBPP	=	16;


	cIMG::Reset();

	if (s_iBPP	==	16)
	{
		for (i=0;i<iCount;i++)
		{
			char	strName[32];
			WORD	xs,ys,*pData;

			file.Read(&xs,2);
			file.Read(&ys,2);
			pData	=	new WORD [xs*ys+2];

			pData[0]=	xs;
			pData[1]=	ys;

			file.Read(pData+2,xs*ys*2);
			file.Read(strName,32);

			cIMG::AddImage(strName,pData,xs,ys);

			pKILL(pData);
		}
	}
	else
	{
		for (i=0;i<iCount;i++)
		{
			char	strName[32];
			WORD	xs,ys;
			BYTE	*pData;

			file.Read(&xs,2);
			file.Read(&ys,2);
			pData	=	new BYTE [xs*ys+4];

			*(WORD *) pData		=	xs;
			*(WORD *)(pData+2)	=	ys;

			file.Read(pData+4,xs*ys);
			file.Read(strName,32);

			cIMG::AddImage(strName,pData,xs,ys);

			pKILL(pData);
		}	

		file.Read(s_awPlt,sizeof(s_awPlt));

		for (i=0;i<256;i++)
			s_awPlt[i]	=	cTRANS::RGB565ToRGB555(s_awPlt[i]);
	}

	file.Close();

	bVIRGIN	=	TRUE;

	return TRUE;
}