#ifndef _classTRAP_H
#define _classTRAP_H

#include	"SFC.H"
#include	"trapDefine.H"

#define	dTRAP_RANGE					64
#define	dTRAP_DISARM_RANGE			70

#define	dDEFAULT_BROKEN_BOX_CHANCE	75	//	운에 따라서 25%까지 떨어짐
#define	dMAX_TRAP_COUNT				10000
#define	dMAX_TRAP_COUNT_IN_FIELD	256

//	함정 사정거리 64
//	함정 해체하는 거리 70
//	함정/플레이어 함정
//	던젼 안보이는 보물상자(리젠 타임)/몬스터 드랍 보물 상자
//	비밀문/잠긴문	-	이동 포탈과 연계가되어야 함

//	퀼리티 낮은 배지류/퀼리티 높은 보석
//	유니크 조각/레어 조각
//	몬스터가 레어나 유니크를 드롭할때 특정 확률로 보물 상자로 떨어짐
//	해체레벨(해체 가능 최소 레벨)
//	안보이는 레벨(발견 가능 최소 레벨)
//	생존 시간(플레이어가 만든 경우)
//	체력
//	물리 저항
//	상자를 부술수 있는 최소 레벨
//	해체된 시간(숨어 있는 포탈이 노출되는 시간)
//	함정의 효과는 스킬정보를 이용

class	cAbility;
class	cACTOR;
class	CHitInfo;

//
//	Result of Update Trap
enum
{
	eRUT_LIVE,
	eRUT_END,
	eRUT_EXPLOSION,
};

//
//	Explosion Trap Type
enum
{
	eETT_NORMAL,
	eETT_DEFEND_ON_IMAGE_FIRST_STEP,
	eETT_DEFEND_ON_IMAGE,
};

class	cTrap
{
public:
	WORD				m_wSerial;				//	함정 시리얼

	WORD				m_wDisarmLevel;			//	해체레벨(해체 가능 최소 레벨)
	WORD				m_wDetectLevel;			//	안보이는 레벨(발견 가능 최소 레벨)
	WORD				m_wUpkeepTime;			//	생존 시간(플레이어가 만든 경우)

	WORD				m_wOwner;				//	함정 만든애
	WORD				m_wTeam;
	DWORD				m_dwOwnerNameHashCode;
	WORD				m_wOwnerStrength,m_wOwnerIntelligence;
	WORD				m_wExplosionTimeCountDown;

	char				m_aAbilityBuffer[4];

	cPOINT				m_pos;
	WORD				m_wRange;

	WORD				m_wLocalPart;			//	필드 안에서 로컬 파트
	WORD				m_wFieldSerial;

						cTrap()	
						{
							reset();
						}

	inline	void		reset()
	{
		m_wSerial		=	0xffff;
	}
	inline	cAbility*	getAbility()
	{
		return	(cAbility*)m_aAbilityBuffer;
	}
	void				copy(cTrap *_lpTrap);
	int					getPhysicalAttackPower(BOOL _bIsPVP,int _iLevel,int _iTargetLevel);
	BOOL				isCaughtInTrap(int _iX,int _iY,int _iBodySize);
	void				build(cACTOR *_lpOwner,cAbility *_lpAbility,int _iX,int _iY,int _iRange,int _iDiscoverLevel,int _iDisarmLevel,int _iUpkeepTime);
	int					update();	//	업데이트 하고 지속 시간이 끝나면 TRUE 값을 리턴한다.
};

class	cTrapManager
{
public:
	cDen<cTrap>				m_den;
	CRITICAL_SECTION		m_cs;

							cTrapManager();
							~cTrapManager();

	inline	void			init()
	{
		m_den.init(dMAX_TRAP_COUNT);
		reset();
	}
	void					reset();	//초기화
	inline	void			close()	{m_den.close();}//초기화
	inline	int				add(cTrap *_lpTrap)			{return m_den.add(_lpTrap);}//추가
	inline	BOOL			remove(int _iSerial)		{return m_den.remove(_iSerial);}//제거
	inline	int				getCount()					{return m_den.getCount();}//카운트
	inline	int				getMaxCount()				{return m_den.getMaxCount();}//카운트
	inline	void			rebuild()					{m_den.rebuild();}
	inline	cTrap*			get(int _iIndex)			{return	m_den.get(_iIndex);}

	int						addTrap(cTrap *_lpTrap);
	BOOL					disarmTrap(int _iSerial);
};

extern	cTrapManager*	g_pTrapManager;

class	cTrapSkill	:	public cTrapSkillDefine
{
public:
	BOOL	explosion(cACTOR *_lpActor,CHitInfo *_lpHitInfo,int _iTrapLevel);

	BOOL	hitFireDamage(cACTOR *_lpActor,CHitInfo *_lpHitInfo,int _iTrapLevel);
	BOOL	hitWaterDamage(cACTOR *_lpActor,CHitInfo *_lpHitInfo,int _iTrapLevel);
	BOOL	hitEarthDamage(cACTOR *_lpActor,CHitInfo *_lpHitInfo,int _iTrapLevel);
	BOOL	hitWindDamage(cACTOR *_lpActor,CHitInfo *_lpHitInfo,int _iTrapLevel);
	BOOL	hitLightDamage(cACTOR *_lpActor,CHitInfo *_lpHitInfo,int _iTrapLevel);
	BOOL	hitDarkDamage(cACTOR *_lpActor,CHitInfo *_lpHitInfo,int _iTrapLevel);
	BOOL	hitPhysicalDamage(cACTOR *_lpActor,CHitInfo *_lpHitInfo,int _iTrapLevel);
	BOOL	hitBlind(cACTOR *_lpActor,int _iTrapLevel);
	BOOL	hitCold(cACTOR *_lpActor,int _iTrapLevel,int _iTime=0);
	BOOL	hitFreeze(cACTOR *_lpActor,int _iTrapLevel);
	BOOL	hitNightmare(cACTOR *_lpActor,int _iTrapLevel);
	BOOL	hitParalysis(cACTOR *_lpActor,int _iTrapLevel);
	BOOL	hitConfuse(cACTOR *_lpActor,int _iTrapLevel);
	BOOL	hitDeclineAttackPower(cACTOR *_lpActor,int _iTrapLevel);
	BOOL	hitDeclineDefensivePower(cACTOR *_lpActor,int _iTrapLevel);
	BOOL	hitDeclineHitChance(cACTOR *_lpActor,int _iTrapLevel,int _iTime=0);
	BOOL	hitDeclineDodgeChance(cACTOR *_lpActor,int _iTrapLevel,int _iTime=0);
	BOOL	hitDeclineAttackSpeed(cACTOR *_lpActor,int _iTrapLevel);
	BOOL	hitDeclineMoveSpeed(cACTOR *_lpActor,int _iTrapLevel);
	BOOL	hitDeclineConcentration(cACTOR *_lpActor,int _iTrapLevel);
	BOOL	hitDeclineMagicResistance(cACTOR *_lpActor,int _iTrapLevel);
	BOOL	hitWeakningResistance(cACTOR *_lpActor,int _iTrapLevel,int _iTime=0);
};

class	cTrapSkillManager
{
public:
	int			m_iCount;
	int			m_iDamageSkillCount;
	int			m_iBadStatusSkillCount;
	cTrapSkill	m_aTrapSkill[dMAX_TRAP_SKILL];

	BOOL		init();
	cTrapSkill*	getSkill(int _iIndex);
	cTrapSkill*	getRandomSkill(int _iType);
};

extern	cTrapSkillManager	g_trapSkillManager;

/*
class	cTrapInfo
{
public:
	WORD			m_wSerial;				//	함정 시리얼
	WORD			m_wType;				//	함정 타입

	WORD			m_wExpireLevel;			//	해체레벨(해체 가능 최소 레벨)
	WORD			m_wStealthLevel;		//	안보이는 레벨(발견 가능 최소 레벨)
	WORD			m_wLifeTime;			//	생존 시간(플레이어가 만든 경우)

	int				m_iHP;					//	체력
	WORD			m_wPhysicalResistance;	//	물리 저항
	WORD			m_wBrokenLevel;			//	상자를 부술수 있는 최소 레벨

	WORD			m_wOwner;				//	주인.. -_-a
	DWORD			m_dwOwnerNameHashCode;
	WORD			m_wDisarmedTime;		//	해체된 시간(숨어 있는 포탈이 노출되는 시간)
	WORD			m_wDisarmPlayer;		//	해체한 플레이어

	WORD			m_wOwnerJob,m_wOwnerLevel;
	WORD			m_wLinkPortalArea;		//	링크되어 있는 필드 이동 포탈

	cAbility		m_ability;

	cPOINT			m_pos;

					cTrapInfo()	
					{
						reset();
					}

	inline	void	reset()
	{
		m_wSerial		=	0xffff;
	}
};
*/

#endif