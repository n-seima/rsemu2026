#ifndef _classANM_H_
#define	_classANM_H_

#include <windows.h>
#include "SFC.H"
#include "cDRAW.H"
#include "cIMAGE.H"

#define	dMAX_ANM			100
#define dMAX_DIRECT			64
#define dMAX_FRAME			1024
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
#define	dANI_DIRECT32		6		//	16 방향
#define	dANI_DIRECT64		7		//	16 방향

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

//	32방향
#define	d32DIRECT_01		0
#define	d32DIRECT_02		1
#define	d32DIRECT_03		2
#define	d32DIRECT_04		3
#define	d32DIRECT_05		4
#define	d32DIRECT_06		5
#define	d32DIRECT_07		6
#define	d32DIRECT_08		7
#define	d32DIRECT_09		8
#define	d32DIRECT_10		9
#define	d32DIRECT_11		10
#define	d32DIRECT_12		11
#define	d32DIRECT_13		12
#define	d32DIRECT_14		13
#define	d32DIRECT_15		14
#define	d32DIRECT_16		15
#define	d32DIRECT_17		16
#define	d32DIRECT_18		17
#define	d32DIRECT_19		18
#define	d32DIRECT_20		19
#define	d32DIRECT_21		20
#define	d32DIRECT_22		21
#define	d32DIRECT_23		22
#define	d32DIRECT_24		23
#define	d32DIRECT_25		24
#define	d32DIRECT_26		25
#define	d32DIRECT_27		26
#define	d32DIRECT_28		27
#define	d32DIRECT_29		28
#define	d32DIRECT_30		29
#define	d32DIRECT_31		30
#define	d32DIRECT_32		31

//	64방향
#define	d64DIRECT_01		0
#define	d64DIRECT_02		1
#define	d64DIRECT_03		2
#define	d64DIRECT_04		3
#define	d64DIRECT_05		4
#define	d64DIRECT_06		5
#define	d64DIRECT_07		6
#define	d64DIRECT_08		7
#define	d64DIRECT_09		8
#define	d64DIRECT_10		9
#define	d64DIRECT_11		10
#define	d64DIRECT_12		11
#define	d64DIRECT_13		12
#define	d64DIRECT_14		13
#define	d64DIRECT_15		14
#define	d64DIRECT_16		15
#define	d64DIRECT_17		16
#define	d64DIRECT_18		17
#define	d64DIRECT_19		18
#define	d64DIRECT_20		19
#define	d64DIRECT_21		20
#define	d64DIRECT_22		21
#define	d64DIRECT_23		22
#define	d64DIRECT_24		23
#define	d64DIRECT_25		24
#define	d64DIRECT_26		25
#define	d64DIRECT_27		26
#define	d64DIRECT_28		27
#define	d64DIRECT_29		28
#define	d64DIRECT_30		29
#define	d64DIRECT_31		30
#define	d64DIRECT_32		31
#define	d64DIRECT_33		32
#define	d64DIRECT_34		33
#define	d64DIRECT_35		34
#define	d64DIRECT_36		35
#define	d64DIRECT_37		36
#define	d64DIRECT_38		37
#define	d64DIRECT_39		38
#define	d64DIRECT_40		39
#define	d64DIRECT_41		40
#define	d64DIRECT_42		41
#define	d64DIRECT_43		42
#define	d64DIRECT_44		43
#define	d64DIRECT_45		44
#define	d64DIRECT_46		45
#define	d64DIRECT_47		46
#define	d64DIRECT_48		47
#define	d64DIRECT_49		48
#define	d64DIRECT_50		49
#define	d64DIRECT_51		50
#define	d64DIRECT_52		51
#define	d64DIRECT_53		52
#define	d64DIRECT_54		53
#define	d64DIRECT_55		54
#define	d64DIRECT_56		55
#define	d64DIRECT_57		56
#define	d64DIRECT_58		57
#define	d64DIRECT_59		58
#define	d64DIRECT_60		59
#define	d64DIRECT_61		60
#define	d64DIRECT_62		61
#define	d64DIRECT_63		62
#define	d64DIRECT_64		63


//	이벤트 타입
#define	dEVENT_DAMAGE		1
#define	dEVENT_TRIGGER		2
#define	dEVENT_LOOP_START	4
#define	dEVENT_LOOP_END		8
#define	dEVENT_STEP			16

typedef struct _tsSDHEADER
{
	int			Size;
	char		Reg[40];
	WORD		m_wMaxSpriteWidth,m_wMaxSpriteHeight;
	WORD		m_wMaxShadowWidth,m_wMaxShadowHeight;
	char		buffer[2];
	BYTE		BPP;
	WORD		ImageCount;
	BYTE		Alpha,bOutline,bShadow;
} tsSDHEADER;

typedef struct _tsSADHEADER
{
	int			Size;
	char		Reg[40];
	WORD		m_wMaxSpriteWidth,m_wMaxSpriteHeight;
	WORD		m_wMaxShadowWidth,m_wMaxShadowHeight;
	char		buffer[2];
	WORD		AnmCount;
	WORD		ImageCount;
	BYTE		AnmType,Version;
	BYTE		bOutline,bShadow,bHalf;
	BYTE		BPP;

} tsSADHEADER;


extern	cRECT	CRASHBOX;
extern	cRECT	g_rectSelect;

class cIMG
{
public:

	POINTS			Pos,sPos;
	cIMG			*pNEXT,*pPRE;
	WORD			*pSPRITE;
	BYTE			*pSHADOW,*pLAYER,*p8SPRITE;
	int				Size,sSize,lSize,serial,xs,ys,xp,yp;

	BOOL			isSelect;
	BOOL			bIsMoved;

					cIMG()
					{	
						pNEXT=NULL	,	pPRE=NULL	,	pSPRITE=NULL,	pSHADOW=NULL,pLAYER	=	NULL,p8SPRITE	=	NULL;
						Pos.x=0		,	Pos.y=0		,	sPos.x=0	,	sPos.y=0	;
						xs=0		,	ys=0		,	Size=0		,	sSize=0		,	lSize=0		,	serial=0xffff;
						xp=0		,	yp=0		;
						isSelect	=	FALSE;
						bIsMoved	=	FALSE;
					}

					~cIMG()
					{	pKILL(pSPRITE);
						pKILL(p8SPRITE);
						pKILL(pSHADOW);
						pKILL(pLAYER);
					}

	BOOL			MakeShadow();

	void			reform(int dx,int dy);
	void			reformShadow(int dx,int dy);
	void			rePos(int x,int y);
	BOOL			isIN(int x,int y);
	void			DelShadow();
	void			Put(int x,int y,int rate,BOOL bflip,int effect);
	void			PutShadow(int x,int y,int rate,BOOL bflip);
	void			PutLayer(int xp,int yp,WORD color,int rate,BOOL bflip);
	void			Copy(cIMG *img);

	void			SetLayer(BYTE *layer,int size);
	void			SetShadow(BYTE *shadow,int size);

	static	cIMG	*pHEAD,*pTAIL,*pCURRENT;	//	링크드 리스트..
	static	int		Count,BigXS,BigYS,Current,shadowXRate,shadowYRate;
	static	int		s_iMaxSpriteWidth,s_iMaxSpriteHeight;
	static	int		s_iMaxShadowWidth,s_iMaxShadowHeight;

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
	static	void	Reform(int dx,int dy,int value,int control,BOOL _bIsPressedAlt=FALSE);

	static	cIMG	*Get(int index);
	static	int 	Get(cIMG *img);

	static	void	GetMaxImage();

	static	void	ShadowAll();
	static	void	ShadowAnm();
	static	void	ShadowCurrent();

	static	void	Reset();

	static	void	SetCURRENT(int num)
					{	if (num >= Count) num	=	0;
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
	POINTS			Release[dMAX_DIRECT];							//	마법이나 공격시 릴리즈 포인트
	POINTS			RefitPos[dMAX_DIRECT];

	int				bSaveReleasePoint;			//	릴리지 포인트 저장?
	int				bSaveRefitPos;				//	프레임 보정 위치 저장?
	int				FPS,PPS,ALPHA;				//	Frame Per Second , Second Per Second
	int  			FrameCount,directCount;		//	프레임 카운트와 에니메이션 카운트
	int				LinkAnm;					//	링크된 에니메이션
	int				anmtype;

	WORD			Sprite[dMAX_DIRECT][dMAX_FRAME];
	BYTE			BackFrame[dMAX_DIRECT][dMAX_FRAME];
	BYTE			isEvent[dMAX_FRAME];

					cANM()
					{	int i;

						memset(isEvent,0,dMAX_FRAME);

						directCount	=	1;
						FrameCount	=	10;
						FPS			=	16;
						PPS			=	0;
						strcpy(Name,"명칭 없음");
						ALPHA		=	dPUT_NORMAL;

						bSaveReleasePoint	=	FALSE;			//	릴리지 포인트 저장?
						bSaveRefitPos		=	FALSE;			//	프레임 보정 위치 저장?
						LinkAnm				=	0xffff;

						for (i=0;i<dMAX_DIRECT;i++)	for (int j=0;j<dMAX_FRAME;j++)
							Sprite[i][j]=0,BackFrame[i][j]	=	0;

					}

	void			ClearData(int type);
	void			Set(cANM *anm);

	void			Set(cANMDATA *anm);
	void			Sync(cANMDATA *anm);

	void			Put(int xp,int yp,int Rate,BOOL flip);
	void			PutShadow(int xp,int yp,int Rate,BOOL flip);
	BOOL			isBack(int direct=0xffff,int frame=0xffff);

	static	BYTE	bSAVEOUTLINE,bSAVESHADOW,bHALF;

	static	WORD	PLT[256];
	static	int		BPP,CurAnm,CurFrame,CurDirect;

	static	int		s_iDefaultAttack,s_iDefaultMagic;
	static	WORD	s_wIsOccasionallyRestAction;

	static	int		AnmType,AnmCount,CrashBoxCount;
	static	char	FName[256],ANMName[50];
	static	BYTE	PALETTE[256*3];

	static	void	Reset();

	static	BOOL	SaveSAD	(char *fn,BOOL bInfoOut	=	TRUE);
	static	BOOL	SaveSD	(char *fn,BOOL _bIsSaveCodeSprite=	FALSE);
	static	BOOL	SaveID	(char *fn);
	static	BOOL	SaveLayer(char *fn);
	static	BOOL	LoadSAD	(char *fn,BOOL _isAddSitdown=FALSE , HWND _hWnd = NULL);
	static	BOOL	Import	(char *fn,BOOL _isOperateSitDown=FALSE);

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

	static	void	DamageFrame(int frame);
	static	void	TriggerFrame(int frame);
	static	void	StepFrame(int frame);

	static	void	LoopStartFrame(int frame);
	static	void	LoopEndFrame(int frame);

	static	BOOL	IsDamageFrame(int frame);
	static	BOOL	IsLoopStartFrame(int frame);
	static	BOOL	IsLoopEndFrame(int frame);
	static	BOOL	IsTriggerFrame(int frame);
	static	BOOL	IsStepFrame(int frame);

	static	BOOL	SaveAsFont(char *fname,int what);	//	이미지를 폰트로 저장한다.
	static	BOOL	LoadAniSet(char *fn=NULL);			//	에니메이션 셋팅 데이터 오픈
	static	BOOL	SaveAniSet(char *fn=NULL);			//	에니메이션 셋팅 데이터 저장
	static	BOOL	IsBack(int frame);

	static	int		GetLink(int anm=0xffff);

	static	int		ReformPosX();
	static	int		ReformPosY();
	static	void	ReformPos(int x,int y);
	static	void	MovePos(int x,int y,BOOL _bIsPressedShift);

	static	void	SetReleasePos(int x,int y);

	static	void	Set(cANIMATION *anm);
	static	void	CorrectEventFrame();
};

extern	int	GetDirectImage(int type,int direct,int moveoval);

extern char	strAnmType[10][40];
extern char	strDirect[10][64][40];
extern char strAnmName[dMAX_ANM][40];
extern char strCrashType[5][20];
extern int	_directCount[10];
extern cANM	ANM[dMAX_ANM];
extern char	valueDirect[10][16];

extern int	MoveOval;									//	2:1 좌표계
extern WORD	g_wCrashSize;								//	2:1 좌표계
extern BYTE	g_bTransColorIndex;

extern	tsSDHEADER	SDHEADER;
extern	tsSADHEADER	SADHEADER;

#endif