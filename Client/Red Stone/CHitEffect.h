#ifndef	_classHIT_EFFECT_
#define	_classHIT_EFFECT_

#include "cNEMO.H"
#include "CGame.H"

#define	dMAX_HIT_EFFECT				256
#define	dMAX_BLOOD_EFFECT			256

#define	dHIT_INFO_WHITE_NUMBER		0
#define	dHIT_INFO_RED_NUMBER		10
#define	dHIT_INFO_GREEN_NUMBER		20
#define	dHIT_INFO_MAGIC_NUMBER		30

#define	dHIT_INFO_RED_MISS			40
#define	dHIT_INFO_WHITE_MISS		41
#define	dHIT_INFO_BLOCK				42
#define	dHIT_INFO_DODGE				43
#define	dHIT_INFO_SLIGHT_DAMAGE		44
#define	dHIT_INFO_NO_DAMAGE			45

#define	dHIT_BY_MONSTER_BORDER		46


#define	dHIT_INFO_TIME				82

class	CHitEffect		:	public	CGame		//	캐릭터 몸에 나타나는 각종 마법효과
{
public:
	WORD				m_wEffect;
	WORD				m_wAnm;
	WORD				m_wDirect;
	WORD				m_wFrame;
	WORD				m_wMaxFrame;
	WORD				m_wFrameCounter;
	WORD				m_wFPS;
	WORD				m_wPaletteIndex;
	WORD				m_wOutputEffect;
	CPos				m_pos;
	WORD				m_wXScale,m_wYScale;
	WORD				m_wIsHitByMonster;

	int					m_iHitInfoKind;
	int					m_iHitInfoTime;
	WORD				m_wShakeValue;
	char				m_strHitInfo[8];

						CHitEffect()
						{	
							m_wEffect		=	0xffff;
							m_iHitInfoKind	=	0xffff;
							m_wPaletteIndex	=	0xffff;
							m_wShakeValue	=	0;
							strcpy(m_strHitInfo,"");
						}

	BOOL				update();			//	끝났으면 자폭하고  FALSE를 리턴
	void				put(int x,int y);	//	효과를 찍는다.
	int					getRemainFrame()	{	return m_iHitInfoTime;}
};

class	CHitEffectManager
{
public:
	CHitEffect			m_aHitEffect[dMAX_HIT_EFFECT];
	int					m_iHitCount,m_iRookie;

						CHitEffectManager(){reset();}

	int					addEffectAndInfo(int _iX,int _iY,int _iEffect,int _iHitInfo,int _iPhysicalDamage,int _iMagicDamage,int _iDirect,BOOL _bIsOwnTeam,int _iAnm=0xffff);
	int					addEffect(int _iX,int _iY,int _iEffect,int _iDirect,int _iAnm=0xffff,int _iShakeValue=0,int _iXScale=100,int _iYScale=100 , BOOL _isHalfFaram = FALSE);
	int					addAura(int _iEffect,int _iOutputEffect,int _iPalette,int _iXScale=100,int _iYScale=100);
	int					addInfo(int _iX,int _iY,int _iHitInfo,int _iPhysicalDamage,int _iMagicDamage,BOOL _bIsOwnTeam);

	BOOL				remove(int serial);
	void				reset();
	BOOL				update(int _iIndex);
	int					getRemainFrame(int _iIndex);
	CHitEffect*			get(int _iIndex);
	void				put(int _iIndex,int _iX,int _iY);
};

class	cHitInfoManager
{
public:
	WORD			m_wCount;
	WORD			*m_pHitEffect;			//	타격 효과

					cHitInfoManager();
					~cHitInfoManager();
	
	void			init(int _iCount);
	int				getFreeHitIndex();
	void			addHitEffect(int _iEffect);
};

extern	CHitEffectManager	g_hem;

#endif