#ifndef _classIMAGE_H
#define	_classIMAGE_H

#include "SFC.H"

#define	REGSDHEADER		"하늘 스프라이트 데이터"
#define	REGSADHEADER	"하늘 스프라이트 에니메이션 데이터"
#define	REGSADHEADER2	"하늘 스프라이트 에니메이션 데이터 2"
#define	REGRBDHEADER	"붉은 보석 건물 데이터"
#define	REGRODHEADER	"붉은 보석 오브젝트 데이터"

#define	dEVENT_DAMAGE		1
#define	dEVENT_TRIGGER		2
#define	dEVENT_LOOP_START	4
#define	dEVENT_LOOP_END		8

#define	dLOOP_NONE			0
#define	dLOOP_INCREASE		1
#define	dLOOP_DECREASE		2

typedef struct
{	int			Size;
	char		Reg[50];
	BYTE		BPP;
	WORD		ImageCount;
	BYTE		Alpha,bOutline,bShadow;
} tsSDHEADER;

typedef struct
{	int			Size;
	char		Reg[50];
	WORD		AnmCount;
	WORD		ImageCount;
	BYTE		AnmType,CrashType;
	BYTE		bOutline,bShadow,bHalf;
	BYTE		BPP;

} tsSADHEADER;

extern	tsSDHEADER	SDHEADER;
extern	tsSADHEADER SADHEADER;

//	충돌 박스 타입
#define	dCRASH_NONE			0		//	없음
#define	dCRASH_ONE			1		//	하나가 있다.
#define	dCRASH_PER_ANM		2		//	에니메이션당 하나
#define	dCRASH_PER_DIRECT	3		//	방향 하나.
#define	dCRASH_PER_IMAGE	4		//	이미지당 하나.

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


const	int	c_aFlipedDirect[2][10][16]	=
{	{	{0},		//	플립 아님
		{d2DIRECT_E,d2DIRECT_W},
		{d2DIRECT_N,d2DIRECT_S},
		{d4DIRECT_N,d4DIRECT_E,d4DIRECT_S,d4DIRECT_W},
		{d8DIRECT_N,d8DIRECT_NE,d8DIRECT_E,d8DIRECT_SE,d8DIRECT_S,d8DIRECT_SW,d8DIRECT_W,d8DIRECT_NW},
		{d16DIRECT_N,d16DIRECT_NNE,d16DIRECT_NE,d16DIRECT_ENE,d16DIRECT_E,d16DIRECT_ESE,d16DIRECT_SE,d16DIRECT_SSE,
		 d16DIRECT_S,d16DIRECT_SSW,d16DIRECT_SW,d16DIRECT_WSW,d16DIRECT_W,d16DIRECT_WNW,d16DIRECT_NW,d16DIRECT_NNW},
	},
	{	{0},		//	플립
		{d2DIRECT_E,d2DIRECT_E},
		{d2DIRECT_N,d2DIRECT_S},
		{d4DIRECT_N,d4DIRECT_E,d4DIRECT_S,d4DIRECT_E},
		{d8DIRECT_N,d8DIRECT_NE,d8DIRECT_E,d8DIRECT_SE,d8DIRECT_S,d8DIRECT_SE,d8DIRECT_E,d8DIRECT_NE},
		{d16DIRECT_N,d16DIRECT_NNE,d16DIRECT_NE,d16DIRECT_ENE,d16DIRECT_E,d16DIRECT_ESE,d16DIRECT_SE,d16DIRECT_SSE,
		 d16DIRECT_S,d16DIRECT_SSE,d16DIRECT_SE,d16DIRECT_ESE,d16DIRECT_E,d16DIRECT_ENE,d16DIRECT_NE,d16DIRECT_NNE},
	}
};

const	int	c_aIsFlip[2][10][16]	=
{	{	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		//	플립 아님
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	},
	{	{0},		//	플립
		{0,44},
		{0,0},
		{0,0,0,44},
		{0,0,0,0,0,44,44,44},
		{0,0,0,0,0,0,0,0,0,44,44,44,44,44,44,44},
	}
};


class	cANM_INFO
{
public:
	int  			m_iAnmType;
	int  			m_iDirectCount;		//	에니메이션 타입,방향 수
	int				m_iALPHA;
	int				m_iFPS;
	int				m_iPPS;				//	알파 효과,Frame per Sec,Pixel per Sec
	BOOL			m_isRELEASE;
	BOOL			m_isREFITFRAME;		//	릴리즈 포인트,프레임 보정
	int				m_iLinkAnm;			//	연계된 에니메이션
};


class cANMDATA	:	public cANM_INFO
{
	BOOL			m_isControledByMcu;
public:
	POINTS			*m_pPos;						//	프레임의 위치
	POINTS			*m_pReleasePos;					//	릴리즈 포인트
	BYTE			*m_pIsEvent;					//	이벤트

	int				m_iFrameCount;					//	프레임 카운트
	int				m_iLoopBegin,m_iLoopEnd;
	int				m_iFPS;
	int				m_iPPS;				//	알파 효과,Frame per Sec,Pixel per Sec

					cANMDATA();
					~cANMDATA();

	BOOL			init(cFILE *file);
	void			reset();

	inline	BOOL	isTrigger(int frame)	{	return	getEventValue(frame)&dEVENT_TRIGGER;}
	inline	BOOL	isDamage(int frame)		{	return	getEventValue(frame)&dEVENT_DAMAGE;}
	inline	BYTE	getEventValue(int frame)
	{
		if	(m_pIsEvent == NULL)
			return	0;
		if	(frame < 0 || frame >= m_iFrameCount)
			return	0;

		return	m_pIsEvent[frame];
	}
};

class	cBODY
{
public:
	//	에니메이션 데이터
	int				m_iAnmCount;		//	에니메이션 카운트와 충돌 박스 타입
	int				m_iSpriteCount;
	int				m_iDefaultAttack;		//	기본 공격
	int				m_iDefaultMagic;		//	기본 방어
	int				m_iCrashSize;
	cANMDATA		*m_pANM;


					cBODY();
					~cBODY();

	void			reset();

	BOOL			load(char *fn,cNUX *file=NULL);
	BOOL			load(cFILE *fp);

	inline	cANMDATA*	getAnmData(int anm);

	inline	BOOL	isTrigger(int anm,int frame);
	inline	BOOL	isDamage(int anm,int frame);
	inline	int		getFrameCount(int anm);
	inline	int		getFPS(int anm);
	inline	int		getPPS(int anm);
	inline	int		getFirstTriggerFrame(int _iAnm);
	inline	int		getLoopBegin(int anm);
	inline	int		getLoopEnd(int anm);
	inline	BOOL	isLoopBegin(int anm,int frame);
	inline	BOOL	isLoopEnd(int anm,int frame);
};

inline	cANMDATA*	
cBODY::getAnmData(int anm)
{
	if	(!m_pANM || anm < 0 || anm >= m_iAnmCount)
		return	NULL;

	return	&m_pANM[anm];
}

inline	BOOL
cBODY::isTrigger(int anm,int frame)
{
	cANMDATA	*lpAnmData	=	getAnmData(anm);

	if	(!lpAnmData)
		return	FALSE;

	return	lpAnmData->isTrigger(frame);
}

inline	BOOL
cBODY::isDamage(int anm,int frame)
{
	cANMDATA	*lpAnmData	=	getAnmData(anm);

	if	(!lpAnmData)
		return	FALSE;

	return	lpAnmData->isDamage(frame);
}

inline	int
cBODY::getFrameCount(int anm)
{
	cANMDATA	*lpAnmData	=	getAnmData(anm);

	if	(!lpAnmData)
		return	0;

	return	lpAnmData->m_iFrameCount;
}


inline	int
cBODY::getFPS(int anm)
{
	cANMDATA	*lpAnmData	=	getAnmData(anm);

	if	(!lpAnmData)
		return	0;

	return	lpAnmData->m_iFPS;
}

inline	int
cBODY::getPPS(int anm)
{
	cANMDATA	*lpAnmData	=	getAnmData(anm);

	if	(!lpAnmData)
		return	0;

	return	lpAnmData->m_iPPS;
}

inline	int
cBODY::getFirstTriggerFrame(int _iAnm)
{
	cANMDATA	*lpAnmData	=	getAnmData(_iAnm);

	if	(!lpAnmData)
		return	0xffff;

	for (int i=0;i<lpAnmData->m_iFrameCount;i++)
		if	(lpAnmData->isTrigger(i))
			return	i;

	return	0xffff;
}

inline	int
cBODY::getLoopBegin(int _iAnm)
{
	cANMDATA	*lpAnmData	=	getAnmData(_iAnm);

	if	(!lpAnmData)
		return	0xffff;

	return	lpAnmData->m_iLoopBegin;
}

inline	int
cBODY::getLoopEnd(int _iAnm)
{
	cANMDATA	*lpAnmData	=	getAnmData(_iAnm);

	if	(!lpAnmData)
		return	0xffff;

	return	lpAnmData->m_iLoopEnd;
}

inline	BOOL
cBODY::isLoopBegin(int _iAnm,int _iFrame)
{
	if	(getLoopBegin(_iAnm)	==	_iFrame)
		return	TRUE;

	return	FALSE;
}

inline	BOOL
cBODY::isLoopEnd(int _iAnm,int _iFrame)
{
	if	(getLoopEnd(_iAnm)	==	_iFrame)
		return	TRUE;

	return	FALSE;
}

#endif
