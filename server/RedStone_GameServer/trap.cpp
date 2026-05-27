#include "trap.H"
#include "cActor.H"
#include "CSkill.H"
#include "cFIELD.H"
#include "GameDLL.H"

cTrapManager		*g_pTrapManager	=	NULL;

BOOL
cTrap::isCaughtInTrap(int _iX,int _iY,int _iBodySize)
{
	if	(m_wRange	==	0)
		return	FALSE;

	DWORD	dwRange		=	GetOvalRange(_iX,_iY,m_pos.x,m_pos.y);
	DWORD	dwTrapRange	=	(m_wRange+_iBodySize)*(m_wRange+_iBodySize);

	if	(dwRange	<=	dwTrapRange)
		return	TRUE;

	return	FALSE;
}

void
cTrap::build(cACTOR *_lpOwner,cAbility *_lpAbility,int _iX,int _iY,int _iRange,int _iDiscoverLevel,int _iDisarmLevel,int _iUpkeepTime)
{
	reset();

	cAbility *lpAbility	=	(cAbility *)m_aAbilityBuffer;

	m_wDisarmLevel		=	_iDisarmLevel;			//	해체레벨(해체 가능 최소 레벨)
	m_wDetectLevel		=	_iDiscoverLevel;		//	안보이는 레벨(발견 가능 최소 레벨)
	m_wUpkeepTime		=	max(_iUpkeepTime,1);	//	생존 시간(플레이어가 만든 경우)

	m_wOwner			=	_lpOwner->m_wSerialInField;	//	함정 만든애
	m_dwOwnerNameHashCode=	_lpOwner->m_dwNameHashCode;
	m_wOwnerStrength	=	_lpOwner->getStrength();
	m_wOwnerIntelligence=	_lpOwner->getIntelligence();
	m_wTeam				=	_lpOwner->m_wTeam;
	m_wLocalPart		=	_lpOwner->m_iLocalPart;

	lpAbility->copy(_lpAbility);

	m_pos.x				=	_iX;
	m_pos.y				=	_iY;
	m_wRange			=	_iRange;
	m_wFieldSerial		=	_lpOwner->m_lpField->m_iUniqueSerial;

	if	(_iRange	==	0)
		m_wExplosionTimeCountDown	=	1;
}

void
cTrap::copy(cTrap *_lpTrap)
{
	memcpy(this,_lpTrap,sizeof(cTrap));
}

//	업데이트 하고 지속 시간이 끝나면 TRUE 값을 리턴한다.
int
cTrap::update()
{
	m_wUpkeepTime--;

	if	(m_wUpkeepTime	==	0)
		return	eRUT_END;

	if	(m_wRange	==	0)
	{
		m_wExplosionTimeCountDown--;

		if	(m_wExplosionTimeCountDown	==	0)
		{
			m_wExplosionTimeCountDown	=	getAbility()->getStrikePeriod()/100;

			return	eRUT_EXPLOSION;
		}
	}

	return	eRUT_LIVE;
}

//
//	공격력을 구한다.
int
cTrap::getPhysicalAttackPower(BOOL _bIsPVP,int _iLevel,int _iTargetLevel)
{
	int			iDamage					=	0;
	cAbility	*lpAbility				=	getAbility();
	int			iSkillDamage			=	lpAbility->getAttackPower();
	int			iSkillDamagePercentage	=	lpAbility->getAttackPowerPercentage(NULL);
	int			iLevelCorrectDamagePercentage	=	0;

	if	(_bIsPVP)
		iLevelCorrectDamagePercentage	=	(_iLevel-_iTargetLevel)/25*30;

	iDamage		=	iSkillDamage*iSkillDamagePercentage/100;
	iDamage		=	iDamage+iDamage*m_wOwnerStrength/200;

	if (iLevelCorrectDamagePercentage)
	{
		int	iCorrectDamage;

		iCorrectDamage	=	iSkillDamage*(iSkillDamagePercentage+iLevelCorrectDamagePercentage)/100;
		iCorrectDamage	=	iDamage+iDamage*m_wOwnerStrength/200;
		iDamage			=	max(iCorrectDamage*40/100,iDamage);
		iDamage			=	min(iCorrectDamage*180/100,iDamage);
	}

	return	iDamage;
}

cTrapManager::cTrapManager()
{
	FPInitCS(&m_cs);
}

cTrapManager::~cTrapManager()
{
	m_den.close();

	DeleteCriticalSection(&m_cs);
}

void
cTrapManager::reset()
{
	m_den.reset();
}

int
cTrapManager::addTrap(cTrap *_lpTrap)
{
	mCS(m_cs);

	int	iTrapSerial		=	add(_lpTrap);

	_lpTrap->m_wSerial	=	iTrapSerial;

	return	iTrapSerial;
}

BOOL
cTrapManager::disarmTrap(int _iSerial)
{
	mCS(m_cs);
	
	return	remove(_iSerial);
}
