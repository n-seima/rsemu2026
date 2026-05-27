#ifndef _classANM_H_
#define	_classANM_H_

#include <windows.h>
#include "SFC.H"
#include "cDRAW.H"
#include "cIMAGE.H"

#define	dMAX_ANM			100
#define dMAX_DIRECT			16
#define dMAX_FRAME			256
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


//	이벤트 타입
#define	dEVENT_DAMAGE		1
#define	dEVENT_TRIGGER		2
#define	dEVENT_LOOP_START	4
#define	dEVENT_LOOP_END		8

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
	WORD			*m_p16Image;
	WORD			*pSPRITE;
	BYTE			*pSHADOW,*pLAYER,*p8SPRITE,*m_p8Image;
	int				Size,sSize,lSize,serial,xp,yp;
	WORD			xs,ys,BPP;
	char			m_strName[64];

	BOOL			isSelect;

					cIMG()
					{	pNEXT=NULL	,	pPRE=NULL	,	pSPRITE=NULL,	pSHADOW=NULL,pLAYER	=	NULL,p8SPRITE	=	NULL;
						Pos.x=0		,	Pos.y=0		,	sPos.x=0	,	sPos.y=0	;
						xs=0		,	ys=0		,	Size=0		,	sSize=0		,	lSize=0		,	serial=0xffff;
						xp=0		,	yp=0		;
						isSelect	=	FALSE;
						m_p8Image	=	NULL;
						m_p16Image	=	NULL;
					}

					~cIMG()
					{	pKILL(pSPRITE);
						pKILL(p8SPRITE);
						pKILL(pSHADOW);
						pKILL(pLAYER);
						pKILL(m_p8Image);
						pKILL(m_p16Image);
					}

	BOOL			MakeShadow();

	void			reform(int dx,int dy);
	void			reformShadow(int dx,int dy);
	void			rePos(int x,int y);
	BOOL			isIN(int x,int y);
	void			Put(int x,int y,int rate,BOOL bflip,int effect);
	void			PutShadow(int x,int y,int rate,BOOL bflip);
	void			PutLayer(int xp,int yp,WORD color,int rate,BOOL bflip);
	void			Copy(cIMG *img);

	static	cIMG	*pHEAD,*pTAIL,*pCURRENT;	//	링크드 리스트..
	static	int		Count,BigXS,BigYS,Current,shadowXRate,shadowYRate;
					//	이미지 카운트,이미지 최대 사이즈,현재 이미지,그림자 렌더링
	static	BOOL	AddImage(char *_strName,WORD *data,int xs,int ys);
	static	BOOL	AddImage(char *_strName,BYTE *data,int xs,int ys);
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
	static	WORD	s_awPlt[256];
	static	WORD	s_iBPP;
	static	BOOL	SaveSmi(char *fn);
	static	BOOL	LoadSmi(char *fn);
};

extern	int	GetDirectImage(int type,int direct,int moveoval);

extern char	strAnmType[7][40];
extern char	strDirect[7][16][40];
extern char strAnmName[dMAX_ANM][40];
extern char strCrashType[5][20];
extern int	_directCount[7];
extern cANM	ANM[dMAX_ANM];
extern char	valueDirect[7][16];

extern int	MoveOval;									//	2:1 좌표계
extern int	g_iCrashSize;								//	2:1 좌표계

#endif