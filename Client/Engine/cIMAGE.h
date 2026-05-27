#ifndef _classIMAGE_H
#define	_classIMAGE_H

#include "cDRAW.H"

#define	REGSDHEADER		"ENGINE_TEXT_6"
#define	REGSDHEADER2	"ENGINE_TEXT_7"
#define	REGSADHEADER	"ENGINE_TEXT_8"
#define	REGSADHEADER2	"ENGINE_TEXT_9"
#define	REGRBDHEADER	"ENGINE_TEXT_10"
#define	REGRODHEADER	"ENGINE_TEXT_11"
#define	REGRBDHEADER2	"ENGINE_TEXT_12"
#define	REGRODHEADER2	"ENGINE_TEXT_13"

#define	dEVENT_DAMAGE		1
#define	dEVENT_TRIGGER		2
#define	dEVENT_LOOP_START	4
#define	dEVENT_LOOP_END		8
#define	dEVENT_STEP			16

#define	dLOOP_NONE			0
#define	dLOOP_INCREASE		1
#define	dLOOP_DECREASE		2
#define	dLOOP_INFINITY		3	//	무한 루프.. -o-

typedef struct
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

typedef struct
{
	int			Size;
	char		Reg[40];
	WORD		m_wMaxSpriteWidth,m_wMaxSpriteHeight;
	WORD		m_wMaxShadowWidth,m_wMaxShadowHeight;
	char		buffer[2];
	BYTE		AnmCount;
	BYTE		byAlpha;
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


const	int	c_aFlipedDirect[2][10][64]	=
{	{	{0},		//	플립 아님
		{d2DIRECT_E,d2DIRECT_W},
		{d2DIRECT_N,d2DIRECT_S},
		{d4DIRECT_N,d4DIRECT_E,d4DIRECT_S,d4DIRECT_W},
		{d8DIRECT_N,d8DIRECT_NE,d8DIRECT_E,d8DIRECT_SE,d8DIRECT_S,d8DIRECT_SW,d8DIRECT_W,d8DIRECT_NW},
		{d16DIRECT_N,d16DIRECT_NNE,d16DIRECT_NE,d16DIRECT_ENE,d16DIRECT_E,d16DIRECT_ESE,d16DIRECT_SE,d16DIRECT_SSE,
		 d16DIRECT_S,d16DIRECT_SSW,d16DIRECT_SW,d16DIRECT_WSW,d16DIRECT_W,d16DIRECT_WNW,d16DIRECT_NW,d16DIRECT_NNW},
		{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31},
		{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
		32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63},
	},
	{	{0},		//	플립
		{d2DIRECT_E,d2DIRECT_E},
		{d2DIRECT_N,d2DIRECT_S},
		{d4DIRECT_N,d4DIRECT_E,d4DIRECT_S,d4DIRECT_E},
		{d8DIRECT_N,d8DIRECT_NE,d8DIRECT_E,d8DIRECT_SE,d8DIRECT_S,d8DIRECT_SE,d8DIRECT_E,d8DIRECT_NE},
		{d16DIRECT_N,d16DIRECT_NNE,d16DIRECT_NE,d16DIRECT_ENE,d16DIRECT_E,d16DIRECT_ESE,d16DIRECT_SE,d16DIRECT_SSE,
		 d16DIRECT_S,d16DIRECT_SSE,d16DIRECT_SE,d16DIRECT_ESE,d16DIRECT_E,d16DIRECT_ENE,d16DIRECT_NE,d16DIRECT_NNE},
		{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31},
		{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
		32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63},
	}
};

const	int	c_aIsFlip[2][10][64]	=
{	{	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		//	플립 아님
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	},
	{	{0},		//	플립
		{0,44},
		{0,0},
		{0,0,0,44},
		{0,0,0,0,0,44,44,44},
		{0,0,0,0,0,0,0,0,0,44,44,44,44,44,44,44},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	}
};


class	cINSTANT_INFO
{
public:
	cFILE			*m_lpFILE;
	int				m_iSpriteLocate,m_iLayerLocate,m_iShadowLocate;

					cINSTANT_INFO()
					{	m_lpFILE	=	NULL;
					}


	void			Reset()
					{
						KILL(m_lpFILE);
					}
};


class cSPRITE 
{
public:
	int				m_iSerial;
	WORD			*m_p16Sprite,*m_pPlt,*m_lpPlt;
	BYTE			*m_p8Sprite,*m_pLayer,*m_pShadow;
	int				*m_pSpriteOffset,*m_pLayerOffset,*m_pShadowOffset;
	int				m_iCount,m_iBpp,m_iHeight,m_iWidth;
	int				m_iMaxSpriteWidth,m_iMaxSpriteHeight;
	int				m_iMaxShadowWidth,m_iMaxShadowHeight;
	BYTE			m_isShadow,m_isLayer,m_isLoadedPlt,m_isControledByMcu;

					cSPRITE();
					~cSPRITE();

	BOOL			isIn(int _iIndex,int _iX,int _iY,int _iPutX,int _iPutY);
	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL,cMCU *mcu=NULL);
	BOOL			Load(cFILE *file,BOOL loadpalette,cMCU *mcu);
	BOOL			InstantLoad(cFILE *file,BOOL loadpalette,cMCU *mcu,cINSTANT_INFO *info);

	BOOL			IsFocus(int x,int y);
	void			reset();
	void			close();
	BOOL			getRect(int index,cRECT *rect,int _iScale=100);
	void			setPalette(WORD *plt);	//	팔레트 설정
	void			restorePalette();	//	자체 보유? 한 팔레트가 있으면 복구

	void			put3PieceBar(int _iX,int _iY,int _iFirstImage,int _iSize,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			linerPut(int _iX,int _iY,int _iIndex,int _iAngle,int _iRange,int _iGap,int _iTimes);//	직선으로 _iGap간격으로 _iRange까지 _iTimes만큼 출력
	void			Put(int x,int y,int index,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	void			putClipedImage(int x,int y,int index,int xs,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	void			putReferenceImage(int _iX,int _iY,int _iIndex,WORD *_lpReferenceImage);
	void			clipPut(int x,int y,int index,int xs,int ys);

	void			PutLayer(int x,int y,int index,WORD color,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	void			PutShadow(int x,int y,int index,int xrate=100,int yrate=100,int _iAlpha=16);
// 	void			ShadowMask(int x,int y,int index);

	inline	WORD*	getOrgPalette()	//	데이터에 포함된 팔레트 리턴~
	{
		return	m_pPlt;
	}

	static	BOOL	LoadPlt(WORD *plt,char *fn,cNUX *nux=NULL);
	static	BOOL	LoadPlt(WORD *plt,cFILE *file);

	inline	void	SetSerial(int serial)	{m_iSerial	=	serial;}

	inline	WORD*	Get16(int index)
					{
						if (!m_p16Sprite)		return NULL;
						if (index>=m_iCount)	return NULL;

						return (m_p16Sprite+m_pSpriteOffset[index]);
					}

	inline	BYTE*	Get8(int index)
					{
						if (!m_p8Sprite)		return NULL;
						if (index>=m_iCount)	return NULL;

						return (m_p8Sprite+m_pSpriteOffset[index]);
					}

	inline	int		getSpriteWidth(int index)
					{
						WORD	*lpwData	=	Get16(index);

						if (lpwData)	return	lpwData[0];

						BYTE	*lpbData	=	Get8(index);

						if (lpbData)	return	*(WORD *)lpbData;

						return 0;
					}

	inline	int		getSpriteHeight(int index)
					{
						WORD	*lpwData	=	Get16(index);

						if (lpwData)	return	lpwData[1];

						BYTE	*lpbData	=	Get8(index);

						if (lpbData)	return	*(WORD *)(lpbData+2);

						return 0;
					}

	inline	BOOL	isLoaded()	{if (m_pSpriteOffset)	return	TRUE;return	FALSE;}

	void			getMaxSize();
	void			getSpriteMaxSize(int *_lpiMaxWidth,int *_lpiMaxHeight);
	void			getShadowMaxSize(int *_lpiMaxWidth,int *_lpiMaxHeight);
	static	void	GetSpriteMaxSize(BYTE *_lpSprite,int *_lpiOffset,int _iSpriteCount,int *_lpiMaxWidth,int *_lpiMaxHeight);
	static	void	GetSpriteMaxSize(WORD *_lpSprite,int *_lpiOffset,int _iSpriteCount,int *_lpiMaxWidth,int *_lpiMaxHeight);
};

class cSPRITEQ	:	public cSPRITE
{
public:
	char			m_strFName[40];
	cFILE			*m_lpFILE,*m_pFile;
	int				m_iSpriteLocate,m_iLayerLocate,m_iShadowLocate;

					cSPRITEQ();
					~cSPRITEQ();

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL,cMCU *mcu=NULL);
	BOOL			Load(cFILE *fp,BOOL loadpalette,cMCU *mcu);

	BOOL			Put(int x,int y,int index,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	BOOL			PutLayer(int x,int y,int index,WORD color,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	BOOL			PutShadow(int x,int y,int index,int xrate=100,int yrate=100,int _iAlpha=16);
	BOOL			getRect(int index,cRECT *rect,int _iScale=100);

	WORD*			Get16(int index);
	BYTE*			Get8(int index);
	int				getSpriteWidth(int index);
	int				getSpriteHeight(int index);
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
	WORD			*m_pSprite;						//	에니메이션의 방향별 스프라이트
	BYTE			*m_pIsEvent,*m_pBackFrame;		//	이벤트

	int				m_iFrameCount;					//	프레임 카운트
	int				m_iLoopBegin,m_iLoopEnd;

					cANMDATA();
					~cANMDATA();

	BOOL			Init(cFILE *file,cMCU *mcu);
	void			Reset();

	inline	BOOL	IsTrigger(int frame)	{	return	m_pIsEvent[frame]&dEVENT_TRIGGER;}
	inline	BOOL	IsDamage(int frame)		{	return	m_pIsEvent[frame]&dEVENT_DAMAGE;}
	inline	BOOL	IsStep(int frame)		{	return	m_pIsEvent[frame]&dEVENT_STEP;}

	inline	int		GetSprite(int direct,int frame,BOOL bflip)
					{	return	m_pSprite[c_aFlipedDirect[bflip][m_iAnmType][direct]*m_iFrameCount+frame];
					}
	inline	BOOL	IsBack(int direct,int frame,BOOL bflip)
					{	return	m_pBackFrame[c_aFlipedDirect[bflip][m_iAnmType][direct]*m_iFrameCount+frame];
					}
};

class	cANM_BASE
{
public:
	//	에니메이션 데이터
	int				m_iAnmCount;			//	에니메이션 카운트와 충돌 박스 타입
	cANMDATA		*m_pAnmData;
	BOOL			m_isFlip;				//	플립 에니메이션 이다.
	int				m_iMoveOval;
	cRECT			m_rectCrash;
	cRECT			m_rectSelect;
	int				m_iKind;
	CPos			m_posRefit;
	int				m_iCrashSize;			//	충돌박스 사이즈
	int				m_iDefaultAttack;		//	기본 공격
	int				m_iDefaultMagic;		//	기본 방어
	WORD			m_wIsOccasionallyRestAction;

					cANM_BASE()
					{
						m_pAnmData	=	NULL;
						m_iAnmCount	=	0;
						m_isFlip	=	FALSE;
					}

	virtual			~cANM_BASE()
					{
						_Free(m_pAnmData);
					}

	inline	BOOL	IsBack(int anm,int direct,int frame)
	{
		if (frame>=m_pAnmData[anm].m_iFrameCount)	return	FALSE;
		return m_pAnmData[anm].IsBack(direct,frame,m_isFlip);
	}
	inline	BOOL	IsFront(int anm,int direct,int frame)
	{
		if (frame>=m_pAnmData[anm].m_iFrameCount)	return	FALSE;
		return (1-m_pAnmData[anm].IsBack(direct,frame,m_isFlip));
	}
	inline	BOOL	IsTrigger(int anm,int frame)
	{
		if (frame>=m_pAnmData[anm].m_iFrameCount)	return	FALSE;
		return m_pAnmData[anm].m_pIsEvent[frame]&dEVENT_TRIGGER;
	}
	inline	BOOL	IsDamage(int anm,int frame)
	{
		if (frame>=m_pAnmData[anm].m_iFrameCount)	return	FALSE;
		return m_pAnmData[anm].m_pIsEvent[frame]&dEVENT_DAMAGE;
	}
	inline	BOOL	IsStep(int anm,int frame)
	{
		if (frame>=m_pAnmData[anm].m_iFrameCount)	return	FALSE;
		return m_pAnmData[anm].m_pIsEvent[frame]&dEVENT_STEP;
	}

	inline	int		getTriggerFrame(int _iAnm)
	{
		for (int i=0;i<m_pAnmData[_iAnm].m_iFrameCount;i++)
			if (m_pAnmData[_iAnm].m_pIsEvent[i]&dEVENT_TRIGGER)	
				return	i;

		return	0xffff;
	}
	inline	int		getDamageFrame(int _iAnm)
	{
		for (int i=0;i<m_pAnmData[_iAnm].m_iFrameCount;i++)
			if (m_pAnmData[_iAnm].m_pIsEvent[i]&dEVENT_DAMAGE)	
				return	i;

		return	0xffff;
	}

	inline	BOOL	getReleasePoint(int _iAnm,int _iDirect,CPos *_lpPos)
	{
		if (!m_pAnmData[_iAnm].m_isRELEASE)	
			return	FALSE;

		_lpPos->x	=	m_pAnmData[_iAnm].m_pReleasePos[_iDirect].x;
		_lpPos->y	=	m_pAnmData[_iAnm].m_pReleasePos[_iDirect].y;

		return	TRUE;
	}

	inline	int		GetLinkAnm(int anm)								{return m_pAnmData[anm].m_iLinkAnm;}
	inline	int		GetFrameCount(int anm)							{return m_pAnmData[anm].m_iFrameCount;}
	inline	int		GetLinkFrame(int anm)							{return m_pAnmData[anm].m_iLinkAnm;}
	inline	int		GetFPS(int anm)									{return m_pAnmData[anm].m_iFPS;}
	inline	int		GetPPS(int anm)									{return m_pAnmData[anm].m_iPPS;}
	inline	int		GetDirectCount(int anm)							{return m_pAnmData[anm].m_iDirectCount;}
	inline	int		GetSpriteIndex(int anm,int direct,int frame)	{return	m_pAnmData[anm].GetSprite(direct,frame,FALSE);}

	inline	int		GetLoopBegin(int anm)							{return	m_pAnmData[anm].m_iLoopBegin;	}
	inline	int		GetLoopEnd(int anm)								{return	m_pAnmData[anm].m_iLoopEnd;}
	inline	BOOL	IsLoopBegin(int anm,int frame)					{if (m_pAnmData[anm].m_iLoopBegin==frame) return TRUE;return FALSE;}
	inline	BOOL	IsLoopEnd(int anm,int frame)					{if (m_pAnmData[anm].m_iLoopEnd	 ==frame) return TRUE;return FALSE;}


	virtual	void	Reset(){};
	virtual	void	Destroy(){};
	virtual	BOOL	Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL,cMCU *mcu=NULL){return TRUE;}
	virtual	BOOL	Load(cFILE *fp,BOOL loadpalette,cMCU *mcu){return TRUE;}

	virtual	void	Put(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32){}
	virtual	void	PutSprite(int x,int y,int sprite,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32){}
	virtual	void	PutAlpha(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int alpha){}
	virtual	void	PutShadow(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int _iAlpha=16){}
// 	virtual	void	ShadowMask(int x,int y,int anm,int direct,int frame){}
	virtual	void	PutLayer(int x,int y,int anm,int direct,int frame,WORD color,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32){}
	virtual	void	PutReg(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100){}
	virtual	void	GetRect(int anm,int direct,int frame,cRECT *rect){}
	virtual	void	linerPut(int _iX,int _iY,int _iAnm,int _iDirect,int _iFrame,int _iAngle,int _iRange,int _iGap,int _iTimes){}

	virtual	int		getSpriteHeight(int anm,int direct,int frame){return 0;}
	virtual	int		getSpriteWidth(int anm,int direct,int frame){return 0;}
	virtual	int		getSpriteDX(int anm,int direct,int frame){return 0;}
	virtual	int		getSpriteDY(int anm,int direct,int frame){return 0;}
};

class cANM		:	public cANM_BASE
{
public:
	cSPRITE			m_sprite;

					cANM();
					~cANM();

	void			Destroy();
	void			Reset();
	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL,cMCU *mcu=NULL);
	BOOL			Load(cFILE *fp,BOOL loadpalette,cMCU *mcu);

	void			Put(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutSprite(int x,int y,int sprite,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutAlpha(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int alpha);
	void			PutShadow(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int _iAlpha=16);
// 	void			ShadowMask(int x,int y,int anm,int direct,int frame);
	void			PutLayer(int x,int y,int anm,int direct,int frame,WORD color,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutReg(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100);
	BOOL			getRect(int anm,int direct,int frame,cRECT *rect,int _iScale=100);
	void			linerPut(int _iX,int _iY,int _iAnm,int _iDirect,int _iFrame,int _iAngle,int _iRange,int _iGap,int _iTimes,int _iSecondImageAlphaDepth=32,
							int _iDecreaseAlphaDepth=0,int _iScale=100,BOOL _bIsIncreaseFrame=FALSE);
	void			linerPutByDestPos(int _iX,int _iY,int _iDestX,int _iDestY,int _iAnm,int _iDirect,int _iFrame,int _iRange,int _iGap,int _iScale);
	int				getSpriteHeight(int anm,int direct,int frame);
	int				getSpriteWidth(int anm,int direct,int frame);
	int				getSpriteDX(int anm,int direct,int frame);
	int				getSpriteDY(int anm,int direct,int frame);

	BOOL			convertSpriteData(cFILE *_fp,cFILE *_lpDestFile);
	BOOL			convert051115(char *fn);

	static	BOOL	LoadPlt(WORD *plt,char *fn,cNUX *nux=NULL);
	inline	void	restorePalette()
	{
		m_sprite.restorePalette();
	}
	inline	void	setPalette(WORD *_lpPlt)	//	데이터에 포함된 팔레트 리턴~
	{
		m_sprite.setPalette(_lpPlt);
	}

	inline	WORD*	getOrgPalette()	//	데이터에 포함된 팔레트 리턴~
	{
		return	m_sprite.getOrgPalette();
	}
};

class	cANMQ	:	public cANM_BASE
{
public:
	cSPRITEQ		m_sprite;
	cFILE			*m_pFile;

					cANMQ();
					~cANMQ();

	void			Destroy();
	void			Reset();

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL,cMCU *mcu=NULL);
	BOOL			Load(cFILE *fp,BOOL loadpalette,cMCU *mcu);

	void			Put(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutSprite(int x,int y,int sprite,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutAlpha(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int alpha);
	void			PutReg(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100);
	void			PutShadow(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int _iAlpha=16);

	void			linerPut(int _iX,int _iY,int _iAnm,int _iDirect,int _iFrame,int _iAngle,int _iRange,int _iGap,int _iTimes);
	BOOL			getRect(int anm,int direct,int frame,cRECT *rect,int _iScale=100);

	int				getSpriteHeight(int anm,int direct,int frame);
	int				getSpriteWidth(int anm,int direct,int frame);
	int				getSpriteDX(int anm,int direct,int frame);
	int				getSpriteDY(int anm,int direct,int frame);
};

#define	dOBJECT_TYPE_NORMAL	0
#define	dOBJECT_TYPE_ANM	1
#define	dOBJECT_TYPE_DOOR	2
#define	dOBJECT_TYPE_BRIDGE	3

//	오브젝트 데이터 클래스
class cOBJ_ANM_INFO
{
public:
	BOOL			m_isControledByMcu;
	POINTS			*m_pPos;				//	프레임의 위치
	WORD			*m_pSprite;				//	에니메이션의 방향별 스프라이트
	BYTE			m_bAlpha;				//	알파블렌딩
	WORD			m_wFrameCount;			//	프레임 카운트와 에니메이션 카운트

					cOBJ_ANM_INFO();
					~cOBJ_ANM_INFO();

	BOOL			Init(cFILE *file,cMCU *mcu);
	void			Init(int frameCount);
	int				GetSprite(int frame);
	void			Reset();

};

enum
{
	eOPM_RAPE,
	eOPM_STAND,
	eOPM_FLOAT,
};

class cOBJ_ANM			:	public cSPRITE	//	건물/오브젝트 데이터 클래스
{
	WORD			m_wAnmCount;			//	에니메이션 카운트
	cOBJ_ANM_INFO	*m_pAnmData;				//	에니메이션 정보
	BYTE			*m_pBLOCK,*m_pCRASH;	//	블럭,충돌정보
	int				m_iPutMethod,m_iAttr,m_iFPS,m_iAlpha;
	WORD			m_wFrameCounter;
	WORD			m_wFrame,m_wFPS;
	WORD			m_wIsLoaded;
public:
	WORD			m_wScale;

					cOBJ_ANM();
					~cOBJ_ANM();

	int				getPutMethod()	{return	m_iPutMethod;}
	void			Reset();

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL,cMCU *mcu=NULL);
	BOOL			Load(cFILE *fp,BOOL loadpalette,cMCU *mcu);
	void			Put(int x,int y);
	void			Put(int index,int x,int y);
	void			PutLayer(int x,int y);
	void			PutShadow(int x,int y);
	inline	void	PutShadow(int index,int x,int y)
	{
		if (m_wIsLoaded	==	FALSE)
			return;

		cSHADOW::PutShadow(x,y,m_pShadow+m_pShadowOffset[index],m_wScale,m_wScale,m_iMaxShadowWidth,m_iMaxShadowHeight);
	}
// 	void			ShadowMask(int x,int y);
	void			FitPut(int x,int y,int xs,int ys);
	void			setScale(int _iScale=100)
	{
		m_wScale	=	_iScale;
	}
	inline	BOOL	isDoor()
	{
		if (m_iAttr	==	dOBJECT_TYPE_DOOR)
			return	TRUE;

		return	FALSE;
	}
	inline	BYTE*	getCrashMap()
	{
		return	m_pCRASH;
	}

	static	int		GetDoorCrashMap(char *_lpstrFn,BYTE *_lpBuffer,cNUX *nux);

};

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃이미지 클래스... 그런데... 왜 SMI지.. -_-;;
class cSMI
{
public:
	int					*m_pOffset,m_iCount,m_iImageLocate;	//	오프셋,이미지 수
	WORD				*m_pwImage,*m_lpPlt,*m_pPlt;	//	이미지
	BYTE				*m_pbImage;
	cFILE				*m_pFile;
	cMCU				*m_lpMcu;//	Mcu
	int					m_iBPP;

						cSMI();
						~cSMI();

	void				close();//닫기
	BOOL				load(char *fn,cNUX *_lpNux=NULL,cMCU *_lpMcu=NULL);
	BOOL				iLoad(char *fn,cNUX *_lpNux=NULL,cMCU *_lpMcu=NULL);//인스턴트 로드

	void				put(int _iIndex,int _iX=0,int _iY=0,int _iAlpha=32);
	void				putScale(int _iIndex,int _iX=0,int _iY=0,int _iScale=100);

	WORD*				get16Image(int _iIndex);
	BYTE*				get8Image(int _iIndex);

	void				setPalette(WORD *_lpPalette);
	void				restorePalette();
	void				putBySpriteFilter(int _iX,int _iY,int _iIndex,BYTE *_lpBaseShape,int _iScale=100);
};
//┃이미지 클래스...
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//--------------------------------------------------------------------------------
//	IMAGE 클래스
//--------------------------------------------------------------------------------

class cIMAGE
{
public:
	WORD				*m_p16IMAGE,*m_pPlt;
	BYTE				*m_p8IMAGE;
	int					m_iWidth,m_iHeight,m_iBpp;

						cIMAGE()
						{	m_p16IMAGE	=	NULL;
							m_pPlt		=	NULL;
							m_p8IMAGE	=	NULL;
						}

						~cIMAGE()
						{
							_Free(m_p16IMAGE);
							_Free(m_pPlt);
							_Free(m_p8IMAGE);
						}

	void				close()
	{
		_Free(m_p16IMAGE);
		_Free(m_pPlt);
		_Free(m_p8IMAGE);
	}
	BOOL				Put(int x,int y);
	virtual	BOOL		Load(char *fn,BOOL change16bit=FALSE){return TRUE;};
};

//--------------------------------------------------------------------------------
//	BMP 화일
//--------------------------------------------------------------------------------
class cBMP	:	public cIMAGE
{
public:
	int					m_iRWidth,m_iBytePerLine,m_iColors;

	UINT				m_dwFileLength,m_dwDibSize;
	LPBITMAPINFO		m_lpInfo; 
	LPBITMAPINFOHEADER	m_lpInfoHd;
	BITMAPFILEHEADER	m_stFileHeader;
	BYTE				*m_lpData,*m_pPtr;
	RGBQUAD				*m_lpRGBTable;

						cBMP();
						~cBMP();

	BOOL				Load(char *fn,BOOL change16bit=FALSE);
	BOOL				Load(FILE *fp);

	static	BOOL		Save(char *fn,WORD *image,BOOL overWrite=TRUE);
	static	BOOL		Save(char *fn,WORD *image,int width,int height,BOOL _bIsOverWrite=TRUE);
	static	BOOL		Save(char *fn,BYTE *_lpData,int _iWidth,int _iHeight);
};

//--------------------------------------------------------------------------------
//	TGA 화일
//--------------------------------------------------------------------------------
class cTGA	:	public cIMAGE
{
public:
	BOOL			Load(char *fn,BOOL change16bit=FALSE);
	BOOL			Load(FILE *fp);

	static	BOOL	Save(char *fn,WORD *image,BOOL overWrite=TRUE);
	static	BOOL	Save(char *fn,WORD *image,int width,int height,BOOL overWrite=TRUE);
};


//--------------------------------------------------------------------------------
//	PCX 화일
//--------------------------------------------------------------------------------
class cPCX	:	public cIMAGE
{
public:

	BOOL			Load(char *fn,BOOL change16bit=FALSE);
	BOOL			Load(FILE *fp);

	static	BOOL	Save(char *fn,WORD *image,BOOL overWrite=TRUE);
};

extern	int		g_iImageFrameCounter;	//	오브젝트 에니메이션을 위한 프레임 카운터


#endif
