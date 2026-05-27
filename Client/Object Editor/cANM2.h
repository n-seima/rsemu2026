#ifndef _classANM_H_
#define	_classANM_H_

#include <windows.h>
#include "SFC.H"
#include "cDRAW.H"

#define	dMAX_ANM			100
#define dMAX_DIRECT			16
#define dMAX_FRAME			100
#define dMAX_IMAGE			1000
#define	dMAX_EVENT			100
#define	dMAX_CRASH			50


//	에니메이션 타입
#define	dANI_CUSTOM			0		//	일반 에니메이션 방향이 없다.
#define	dANI_DIRECT2X		1		//	2 방향 좌우만 있다.
#define	dANI_DIRECT2Y		2		//	2 방향 상하만 있다.
#define	dANI_DIRECT4		3		//	4 방향
#define	dANI_DIRECT8		4		//	8 방향
#define	dANI_DIRECT16		5		//	16 방향

#define	dCRASH_NONE			0
#define	dCRASH_ONE			1
#define	dCRASH_PER_ANM		2
#define	dCRASH_PER_DIRECT	3
#define	dCRASH_PER_IMAGE	4

//	2 방향	좌우
#define	d2DIRECT_W			0
#define	d2DIRECT_E			1

//	2 방향	상하
#define	d2DIRECT_N			0
#define	d2DIRECT_S			1

//	4방향
#define	d4DIRECT_N			0
#define	d4DIRECT_E			1
#define	d4DIRECT_S			2
#define	d4DIRECT_W			3

//	8방향
#define	d8DIRECT_N			0
#define	d8DIRECT_NE			1
#define	d8DIRECT_E			2
#define	d8DIRECT_SE			3
#define	d8DIRECT_S			4
#define	d8DIRECT_SW			5
#define	d8DIRECT_W			6
#define	d8DIRECT_NW			7

//	16방향
#define	d16DIRECT_N			0
#define	d16DIRECT_NNE		1
#define	d16DIRECT_NE		2
#define	d16DIRECT_ENE		3
#define	d16DIRECT_E			4
#define	d16DIRECT_ESE		5
#define	d16DIRECT_SE		6
#define	d16DIRECT_SSE		7
#define	d16DIRECT_S			8
#define	d16DIRECT_SSW		9
#define	d16DIRECT_SW		10
#define	d16DIRECT_WSW		11
#define	d16DIRECT_W			12
#define	d16DIRECT_WNW		13
#define	d16DIRECT_NW		14
#define	d16DIRECT_NNW		15

typedef struct _tsSDHEADER
{	int			Size;
	char		Reg[50];
	BYTE		BPP;
	WORD		ImageCount;
	BYTE		Alpha,bOutline,bShadow;
} tsSDHEADER;

typedef struct _tsSADHEADER
{	int			Size;
	char		Reg[50];
	WORD		AnmCount;
	WORD		ImageCount;
	BYTE		AnmType,CrashType;
	BYTE		bOutline,bShadow,bHalf;
	BYTE		BPP;

} tsSADHEADER;

class cCRASHBOX
{
public:
	RECT			rect;
	WORD			serial;

					cCRASHBOX()	{serial=0xffff;}

	BOOL			isIN(int x,int y);
};

class cIMG
{
public:

	POINTS			Pos,sPos;
	cIMG			*pNEXT,*pPRE;
	WORD			*pSPRITE;
	BYTE			*pSHADOW,*pLAYER,*p8SPRITE;
	int				Size,sSize,lSize,serial,xs,ys,xp,yp;

	cCRASHBOX		CRASH[dMAX_CRASH];
	int				newCrash,crashCount,curCrash;
	BOOL			isSelect;

					cIMG()
					{	pNEXT=NULL	,	pPRE=NULL	,	pSPRITE=NULL,	pSHADOW=NULL,pLAYER	=	NULL,p8SPRITE	=	NULL;
						Pos.x=0		,	Pos.y=0		,	sPos.x=0	,	sPos.y=0	;
						xs=0		,	ys=0		,	Size=0		,	sSize=0		,	lSize=0		,	serial=0xffff;
						xp=0		,	yp=0		;
						crashCount	=	0			,	newCrash	=	0	,	curCrash	=	0;
						isSelect	=	FALSE;
					}

					~cIMG()
					{	pKILL(pSPRITE);
						pKILL(p8SPRITE);
						pKILL(pSHADOW);
						pKILL(pLAYER);
					}

	BOOL			MakeShadow();

	void			addCrash(RECT rect);
	void			delCrash(int index);
	int 			GetCrash(int x,int y);
	void			PreCrush();
	void			NextCrush();
	void			reform(int dx,int dy);
	void			reformShadow(int dx,int dy);
	void			rePos(int x,int y);
	BOOL			isIN(int x,int y);
	void			DelShadow();
	void			Put(int x,int y,int rate,BOOL bflip,int effect);
	void			PutShadow(int x,int y,int rate,BOOL bflip);
	void			PutLayer(int xp,int yp,WORD color,int rate,BOOL bflip);

	void			SetLayer(BYTE *layer,int size);
	void			SetShadow(BYTE *shadow,int size);

	static	cIMG	*pHEAD,*pTAIL,*pCURRENT;	//	링크드 리스트..
	static	int		Count,BigXS,BigYS,Current,shadowXRate,shadowYRate;
					//	이미지 카운트,이미지 최대 사이즈,현재 이미지,그림자 렌더링
	static	BOOL	Add(BYTE *data,int xs,int ys,int xp=0xffff,int yp=0xffff);
	static	BOOL	AddShadow(BYTE *data,int xs,int ys,int xp,int yp);
	static	BOOL	Add(WORD *data,int xs,int ys,int xp=0xffff,int yp=0xffff);
	static	BOOL	AddShadow(WORD *data,int xs,int ys,int xp,int yp);

	static	BOOL	Add(WORD *sprite,int size);
	static	BOOL	Add(BYTE *sprite,int size);
	static	BOOL	Add(cIMG *img);
	static	BOOL	Del(cIMG *img);
	static	BOOL	Del(int index);
	static	BOOL	Move(int scrN,int targetN);
	static	void	Reform(int dx,int dy,int value,int control);

	static	cIMG	*Get(int index);
	static	int 	Get(cIMG *img);

	static	void	GetMaxImage();

	static	void	ShadowAll();
	static	void	ShadowAnm();
	static	void	ShadowCurrent();

	static	void	Reset();

	static	void	SetCURRENT(int num)
					{	if (num >= Count)
						{	num			=	0;
							pCURRENT	=	NULL;
						}
						if (num <  0    ) num	=	Count-1;
						if (Count <= 0  ) 
						{	pCURRENT	=	NULL;
							return;
						}

						Current				=	num;
						pCURRENT			=	Get(num);
						pCURRENT->isSelect	=	TRUE;

						ClearSelect();
					}
	static	void	ClearSelect();
	static	void	Select(int num);

};

class cANM
{
public:
	char			Name[50];										//	에니메이션 이름..
	POINTS			Pos[dMAX_DIRECT][dMAX_FRAME];					//	프레임의 위치
	WORD			Sprite[dMAX_DIRECT][dMAX_FRAME];
	BYTE			BackFrame[dMAX_DIRECT][dMAX_FRAME];
	BYTE			isEvent[dMAX_FRAME];
	int  			FrameCount,directCount;							//	프레임 카운트와 에니메이션 카운트
	BYTE			bSTATIC,ALPHA;
	int				anmtype;										//	고정된 에니메이션..
																	//	즉 프레임 위치 저장 할 필요가 없음..
																	//	알파 블렌딩 효과..

					cANM()
					{	int i;

						for (i=0;i<dMAX_FRAME;i++)
						{	isEvent[i]	=	FALSE;
						}

						bSTATIC		=	FALSE;
						directCount	=	1;
						FrameCount	=	1;
						strcpy(Name,"명칭 없음");
						ALPHA		=	dPUT_NORMAL;

						for (i=0;i<dMAX_DIRECT;i++)	for (int j=0;j<dMAX_FRAME;j++)
							Pos[i][j].x=0,Pos[i][j].y=0,Sprite[i][j]=0,BackFrame[i][j]	=	0;
					}

	void			ClearData(int type);
	void			Set(cANM *anm);
	void			Put(int xp,int yp,int Rate,BOOL flip);
	void			PutAll(int xp,int yp,int Rate,int alpha);
	BOOL			isBack(int direct,int frame)
					{	return BackFrame[direct][frame];
					}

	static	BYTE	bSAVEOUTLINE,bSAVESHADOW,bHALF;

	static	WORD	PLT[256];
	static	int		BPP,AnmType,AnmCount,CurAnm,CurFrame,CurDirect,CrashType;
	static	char	FName[256],ANMName[50];
	static	BYTE	PALETTE[256*3];

	static	void	Reset();

	static	BOOL	SaveSAD	(char *fn);
	static	BOOL	SaveSD	(char *fn);
	static	BOOL	SaveID	(char *fn);
	static	BOOL	SaveLayer(char *fn);
	static	BOOL	LoadSAD	(char *fn);
	static	BOOL	LoadSD	(char *fn);

	static	BOOL	isFLIP	();
	static	void	SetCurrentAnm(int num);
	static	void	SetCurrentDirect(int num);
	static	void	ResetPoint();
	static	int		GetFlipDirect(int anm=0xffff,int direct=0xffff);
	static	int		GetDirect(int anm=0xffff,int direct=0xffff);
	static	BOOL	isFlip(int anm=0xffff,int direct=0xffff);

	static	int		getanm(int anm);
	static	int		getframe(int frame);
	static	int		getdirect(int direct);
	static	void	SetMaxAnm(int num);
	static	int 	GetMaxFrame(int anm=0xffff);
	static	int 	GetMaxDirect(int anm=0xffff);
	static	BOOL	isEventFrame(int frame=0xffff,int anm=0xffff);
	static	void	SetEventFrame(int frame=0xffff,int anm=0xffff);
	static	int 	GetFrameImage(int frame=0xffff,int anm=0xffff,int direct=0xffff);
	static	void	SetMaxFrame(int frame,int anm=0xffff);
	static	void	SetCurrentFrame(int frame=0xffff,int anm=0xffff,int direct=0xffff);
	static	void	SetFrameImage(int image,int frame=0xffff,int anm=0xffff,int direct=0xffff);
	static	void	MinusMaxAnm();
	static	void	PlusMaxAnm();
	static	void	MinusMaxFrame(int anm=0xffff);
	static	void	PlusMaxFrame(int anm=0xffff);
	static	void	MinusCurrentImage();
	static	void	PlusCurrentImage();
	static	void	MinusCurrentFrame();
	static	void	PlusCurrentFrame();
	static	void	MinusCurrentAnm();
	static	void	PlusCurrentAnm();
	static	void	MinusCurrentDirect();
	static	void	PlusCurrentDirect();

	static	int		GetAnmType(int anm=0xffff);
	static	int		GetDirCount(int anm=0xffff);

	static	void	SetAnmType(int type);
	static	void	SetAnmType(int anitype,int anm);

	static	void	SetFront(int frame);
	static	void	SetBack(int frame);

	static	BOOL	SaveAsFont(char *fname,int what);	//	이미지를 폰트로 저장한다.
	static	BOOL	LoadAniSet(char *fn=NULL);			//	에니메이션 셋팅 데이터 오픈
	static	BOOL	SaveAniSet(char *fn=NULL);			//	에니메이션 셋팅 데이터 저장
	static	BOOL	IsBack();

	static	int		ReformPosX();
	static	int		ReformPosY();
	static	void	ReformPos(int x,int y);
	static	void	MovePos(int x,int y);
};

extern char	strAnmType[7][40];
extern char	strDirect[7][16][40];
extern char strAnmName[dMAX_ANM][40];
extern char strCrashType[5][20];
extern int	_directCount[7];
extern cANM	ANM[dMAX_ANM];
extern char	valueDirect[7][16];

#endif