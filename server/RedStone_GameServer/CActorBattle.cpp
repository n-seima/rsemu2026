#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "cGAME.H"
#include <math.h>
#include "arca.H"
#include "CGuild.H"
#include "actor_mini_pet.H"
#include "duel.H"

#define	dCHECK_CONTINUOUS_ATTACKED_TIME			8

int	GetP2PCorrectDamage(int _iDamage,int _iCount , cACTOR* _lpAttacker)
{

	double		dbDamage1=	_iDamage/100;
	double		dbDamage2=	_iDamage%100;

	dbDamage1	/=	_iCount;
	dbDamage2	/=	_iCount;

	dbDamage1	=	sqrt(dbDamage1/_iCount);
	dbDamage2	=	sqrt(dbDamage2/_iCount);

	float iDamageFactor = 2.00f;//_lpAttacker->GetDamageFactorByJob();

	if	(g_bIsDuelServer)
	{
		if(g_iSeasonVariable >= dSEASON_VARIABLE_PVP_JOB_FACTOR)
		{
			if(_lpAttacker == NULL)
				iDamageFactor = 2.00f;
			else
			{
				if (!_lpAttacker->isPlayer())
					iDamageFactor =  g_afDuelDamageFactor[dPLAYER_JOB_COUNT];
				else
					iDamageFactor = _lpAttacker->GetDamageFactorByJob();
			}
		}

		dbDamage1	=	((dbDamage1*_iCount)*100+dbDamage2*_iCount)*iDamageFactor;

		return	(int)dbDamage1;
	}

	return	(int)((dbDamage1*_iCount)*100+dbDamage2*_iCount);
}


CCorrectIntelligenceFactor	l_aCorrectIntelligenceFactor[]	=	
{
	0		,0.005f,
	100		,0.0051f,
	132		,0.0052f,
	174		,0.0053f,
	230		,0.0054f,
	304		,0.0055f,
	402		,0.0056f,
	531		,0.0057f,
	702		,0.0058f,
	928		,0.0059f,
	1227	,0.006f,
	1577	,0.0061f,
	1927	,0.0062f,
	2277	,0.0063f,
	2627	,0.0064f,
	2977	,0.0065f,
	3327	,0.0066f,
	3677	,0.0067f,
	4027	,0.0068f,
	4377	,0.0069f,
	4727	,0.007f,
	5127	,0.0071f,
	5527	,0.0072f,
	5927	,0.0073f,
	6327	,0.0074f,
	6727	,0.0075f,
	7127	,0.0076f,
	7527	,0.0077f,
	7927	,0.0078f,
	8327	,0.0079f,
	8727	,0.008f,
	100000	,0.008f,
	-1,
};							

CContinuousAttackedPeneltyInfo	l_aContinuousAttackedPeneltyInfo[]	=
{
	2	,	3		,300	,150	,250,
	4	,	5		,400	,165	,300,
	6	,	7		,500	,180	,350,
	8	,	9		,750	,195	,450,
	10	,	11		,1050	,210	,600,
	12	,	14		,1300	,225	,800,
	15	,	17		,1550	,240	,900,
	18	,	20		,1800	,255	,1000,
	21	,	23		,2050	,270	,1100,
	24	,	27		,2300	,285	,1200,
	28	,	31		,2550	,300	,1300,
	32	,	35		,2800	,315	,1400,
	36	,	40		,3050	,330	,1500,
	41	,	45		,3300	,345	,1600,
	46	,	51		,3550	,360	,1700,
	52	,	57		,3800	,375	,1800,
	58	,	63		,4350	,390	,1900,
	64	,	10000	,4900	,405	,2000,
	0xffff
};

//	int		m_iBeginLevel,m_iEndLevel;
//	int		m_iCorrectPhysicalDamage;
//	int		m_iCorrectHitChance;
//	int		m_iCorrectMagicAttackPower;

CContinuousAttackedPeneltyInfo*
cACTOR::getContinuousAttackedPenelty(int _iCount)
{
	if (_iCount	==	0)
		return	NULL;

	int	iIndex;

	for (iIndex=0;;iIndex++)
	{
		CContinuousAttackedPeneltyInfo	*lpInfo	=	&l_aContinuousAttackedPeneltyInfo[iIndex];

		if (_iCount >= lpInfo->m_iBeginLevel && _iCount <= lpInfo->m_iEndLevel )
			return	lpInfo;
		
		if (lpInfo->m_iBeginLevel	==	0xffff)
			return NULL;
	}

	return	NULL;
}

float 
cACTOR::GetDamageFactorByJob()
{
	return g_afDuelDamageFactor[m_wJob];
}

void
cACTOR::setUpkeepTargetTime(int _iTime)
{
	m_wUpkeepTargetTime			=	(WORD)_iTime;
}

void
cACTOR::setCurrentTarget(cACTOR *_lpCurrentTarget)
{
	cACTOR	*lpTarget	=	m_lpTarget;

	setTarget(_lpCurrentTarget);

	m_lpCurrentTarget	=	lpTarget;
}

//
//	타겟 설정
BOOL
cACTOR::setTarget(cACTOR *_lpTarget,CSkill *_lpSkill)
{
	if	(!_lpTarget)
	{
		m_lpTarget				=	NULL;
		m_iTargetZoneSerial		=	0xffffffff;
		m_wAttackToObjectType	=	0xffff;
		m_wAttackToObject		=	0xffff;

		return TRUE;
	}

	if	(_lpTarget->isNpc())
		return FALSE;

	if	(_lpTarget->m_wSerialInField	==	m_wSerialInField)
		m_lpTarget			=	_lpTarget;

	m_lpTarget			=	_lpTarget;
	m_iTargetZoneSerial	=	_lpTarget->m_iZoneSerial;

	return	TRUE;
}

//
//	사거리 구하기
int
cACTOR::getAttackRange(cAbility *_lpAbility)
{
	int		iRange	=	_lpAbility->getAttackRange(getWeapon(),m_wAttackRange) + getValuePassiveSkill(eSKILL_PEE_INCREASE_RANGE)/100;

	return	iRange;
}


//
//	공격 어빌러티를 설정
void
cACTOR::setEngageAbility(cAbility *_lpAbility,cAbility *_lpSubAbility)
{
	if	(!_lpAbility)
		m_engageAbility.reset();

	if	(!_lpSubAbility)
		m_subAbility.reset();

	if	(!_lpAbility)
		return;

	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(lpSkill)
		if	(lpSkill->m_wType==	eSKILL_TYPE_COMPLETE_PROTECTION)
			return;

	m_engageAbility.copy(_lpAbility);
	m_subAbility.copy(_lpSubAbility);

	m_engageAbility.m_wLevel+=	getCorrectSkillLevel(&m_engageAbility);
	m_subAbility.m_wLevel	+=	getCorrectSkillLevel(&m_subAbility);

	m_wEngageRange			=	getAttackRange(&m_engageAbility);

	if	(m_wEngageRange		==	0)
		m_wEngageRange		=	m_engageAbility.getHitRange(getWeapon());
}

//
//	
BOOL
cACTOR::setFirstBeater(int _iSerial)
{
	cNPC	*lpNpc	=	getNpc();

	if	(lpNpc)
		return	lpNpc->setFirstBeater(_iSerial);

	return	TRUE;
}

//
//	fps와 현재 액션이 끝나는데 필요한 프레임을 얻어서 설정한다.
void
cACTOR::setActionSpeed(cAbility *_lpAbility)
{
	if	(m_wRemainActionTime	)
		return;

	int	iAttackSpeed	=	_lpAbility->getAttackSpeed();

	if	(iAttackSpeed	==	0)
	{
		int	iCorrectSpeed=	_lpAbility->getCorrectAttackSpeed();

		if	(m_iGuildGuardianType	==	eGUILD_GUARDIAN_HOLLOW_KNIGHT	&&	m_iLevel	>=	100)
			iCorrectSpeed+=	25;

		setFpsByWeaponSpeed(iCorrectSpeed);
	}
	else
		m_wActionSpeed	=	iAttackSpeed;

	int iDelayAfterAttack = dCORRECT_ACTION_SPEED;

	CSkill* lpSkill = _lpAbility->getSkill();
	if(lpSkill && lpSkill->m_bf1IsIgnoreDelayAfterAttack)
		iDelayAfterAttack = 100;

	m_wRemainActionTime	=	m_wActionSpeed*iDelayAfterAttack/100/dREVISE_ACTION_SPEED_FOR_TEST;//max(m_wActionSpeed,m_iFrameCount);
	m_wRemainActionTime	=	max(m_wRemainActionTime,1);

	m_iFPS				=	m_iFPS*_lpAbility->getCorrectAttackSpeed()/100;

	if(!_lpAbility->isIgnoreOptionAttackSpeed())	// 공격속도옵션 무시?
		m_iFPS				+=	m_iFPS*getActionSpeed()/100;

	m_iFPS				=	max(m_iFPS,1);

	int	iRequireFrameForThisAction	=	m_iFrameCount*dSYNC_FPS/m_iFPS;

	if	(iRequireFrameForThisAction	>	m_wRemainActionTime)
		m_iFPS	=	m_iFrameCount*dSYNC_FPS/m_wRemainActionTime;

	if	(m_wColdTime)
	{
		m_iFPS				=	m_iFPS*dCOLD_FACTOR/100;
		m_wRemainActionTime	=	m_wRemainActionTime+m_wRemainActionTime*(100-dCOLD_FACTOR)/100;
	}

	m_iFPS		=	min(m_iFPS,dSYNC_FPS*2);
}

//
//	예약된 작업들 실행
inline	BOOL
cACTOR::operateBookedWork()
{
	if	(m_bookedBattle.m_wTarget	!=	0xffff)
	{
		cACTOR	*lpActor	=	m_lpField->getActor(m_bookedBattle.m_wTarget);

		if	(lpActor	==	NULL)
		{
			emptyBookedBattle();
		}
		else
		{
			if	(lpActor->isDeath())		
			{
				emptyBookedBattle();
			}
		}
	}

	if	(m_bookedAction.m_wType	==	eBOOKED_ACTION_TO_ACTOR	&&	m_bookedAction.m_wTargetSerial	!=	0xffff)	
	{
		cACTOR	*lpActor	=	m_lpField->getActor(m_bookedAction.m_wTargetSerial);

		if	(lpActor == NULL)
		{
			emptyBookedAction();
		}
		else
		{
			if	(lpActor->isDeath())
			{
				emptyBookedAction();
			}
		}
	}

	if	(m_isExclusiveAction	)
		return	FALSE;
	if	(m_wChargeTime		)
		return	FALSE;

	if	(m_bookedAction.m_wType	!=	0xffff)
	{
		switch(m_bookedAction.m_wType)
		{
			case	eBOOKED_ACTION_TO_ACTOR		:
			{
				CG_ACTION_TO_ACTOR	packet;

				packet.base.set(sizeof(CG_ACTION_TO_ACTOR),dCG_ACTION_TO_ACTOR);

				packet.bAbility			=	(BYTE)m_bookedAction.m_wAttackAbility;
				packet.bEngageAbility	=	(BYTE)m_bookedAction.m_wEngageAbility;
				packet.bSubAbility		=	(BYTE)m_bookedAction.m_wSubAbility;
				packet.wTargetSerial	=	m_bookedAction.m_wTargetSerial;

				m_lpField->actionToActor(&packet,m_wSerialInField,FALSE);
				break;
			}

			case	eBOOKED_ACTION_QUICK		:
			{
				CG_ACTION_QUICK	packet;

				packet.base.set(sizeof(CG_ACTION_QUICK),dCG_ACTION_QUICK);

				packet.wAbility			=	m_bookedAction.m_wAttackAbility;
				packet.wPosX			=	m_pos.x;
				packet.wPosY			=	m_pos.y;

				m_lpField->actionQuick(&packet,m_wSerialInField,FALSE);
				break;
			}

			case	eBOOKED_ACTION_TO_GROUND	:
			{
				CG_ACTION_TO_GROUND	packet;

				packet.base.set(sizeof(CG_ACTION_TO_GROUND),dCG_ACTION_TO_GROUND);

				packet.wAbility			=	m_bookedAction.m_wAttackAbility;
				packet.wDestPosX		=	m_bookedAction.m_posTarget.x;
				packet.wDestPosY		=	m_bookedAction.m_posTarget.y;

				m_lpField->actionToGround(&packet,m_wSerialInField,FALSE);
				break;
			}
		}
	}

	if	(m_bookedBattle.m_wTarget	!=	0xffff)
	{
		cACTOR	*lpActor	=	m_lpField->getActor(m_bookedBattle.m_wTarget);

		if	(!lpActor	||	lpActor->isDeath())		
			emptyBookedBattle();
		else
			engageBattle(lpActor,&m_bookedBattle.m_ability);
	}

	return	FALSE;
}

//
//	교전 상대와 전투
void
cACTOR::operatePlayerBattle()
{
	m_bIsDamageFrame	=	FALSE;
	m_isTriggerFrame	=	FALSE;	
	
	if	(operateBookedWork())
		return;

	if	(m_isExclusiveAction	)
		return;

	if	(m_wSimpletonTime)
		return;
	
	if	(m_wAttackToObject	!=	0xffff)
	{
		attackToObject(m_wAttackToObjectType,m_wAttackToObject);

		return;
	}

	if	(!m_lpTarget	)
		return;
	
	if	(m_engageAbility.m_wSkill	==	0xffff	)
	{
		setTarget(NULL);
		return;
	}

	if	(m_bf5MagicCarpetShape	!=	eMCS_GEAR)
		m_bf5MagicCarpetShape	=	0;

	cAbility	attackAbility;
	int			iEngageRange	=	m_wEngageRange;

	attackAbility.copy(&m_engageAbility);

	if	(m_subAbility.m_wSkill		!=	0xffff)
	{

		if( m_wTransToWeaponTime	||	m_wTransToFatGirlTime || m_wTransToFrogTime || m_wJumppingSkippingTime ||m_wFeignDeathTime || m_wRabbitTime )
			return;
		if	(isUseAbleSkill(&m_subAbility)	==	eAR_OK && isCastAbleTarget(m_lpTarget,&m_subAbility))
		{
			int	iRequireCP	=	0;

			if	(m_wJob==dJOB_WEREWOLF||m_wJob==dJOB_FALLEN_ANGEL||m_wJob==dJOB_MAGICAL_GIRL||m_wJob==dJOB_DEVIL	)
				iRequireCP	=	m_subAbility.getSpentCP();

			if	(m_iCP	>=	iRequireCP+getReleaseTrasformationCp() || m_wMadnessTime)
			{	// 폭주상태일때도...공격가능..
				iEngageRange	=	getAttackRange(&m_subAbility);
				attackAbility.copy(&m_subAbility);
			}
		}
	}

	
	if(m_wTransformationTime)
	{
		attackAbility.m_wSkill = m_wTransformationSkill;
		attackAbility.m_wLevel = m_wTransformationLevel;
		iEngageRange		=	getAttackRange(&attackAbility);
	}

	if	(!isTargetInAttackRange(m_lpTarget,iEngageRange+getBodySize()))
		return;

	int	iAttackResult	=	attackToActor(m_lpTarget,&attackAbility);

	//JBC	스킬쓰면  미니펫 카운팅 버그 변경.	08-10-31
// 	if	(iAttackResult	==	eAR_OK)
// 		iMiniPet()->updateChargeCount();

	updateLastActionTime();

	m_lpField->sendAttackToActorResult(iAttackResult,this,m_lpTarget,&attackAbility);
}



//
//	교전
BOOL
cACTOR::engageBattle(cACTOR *_lpTarget,cAbility *_lpAbility)
{
	if	(!_lpAbility->isAvail())
		return	eAR_NOT_EXIST_SKILL;

	if	(_lpAbility->getSkill()->m_wType	==	eSKILL_TYPE_COMPLETE_PROTECTION)
		return	eAR_NOT_APT_USED_SKILL_TO_TARGET;

	int	iResult		=	attackToActor(_lpTarget,_lpAbility);

	if	(iResult	!=	eAR_OK)
		return	iResult;

	setTarget(m_lpTarget);
	setEngageAbility(_lpAbility);

	emptyBookedBattle();
//	updatePlayerBattle();
	switch(m_wUseSkill)
	{
		case	dSKILL_BASIC_ATTACK				:
		{
			int iAttackCount =0;
			iAttackCount = _lpAbility->getSkill()->m_wReiterationDamageCountSyncWithOverlapAction;
			if(m_fightEnergy.isIncreaseAttackCount())
				iAttackCount += m_fightEnergy.m_iValue ;
			iAttackCount = min(dMAX_CONTINUOUS_HIT_INFO_COUNT, iAttackCount);

			if	(iAttackCount> 1)
				m_lpField->sendContinuousHitAttack(this,m_lpTarget,&m_engageAbility);
			else
				m_lpField->sendBasicAttack(this,m_lpTarget,&m_engageAbility);
			break;
		}

		case	dSKILL_CONTINUOUS_HIT_ATTACK	:
		case	dSKILL_ILLUSION_ATTACK			:
			m_lpField->sendContinuousHitAttack(this,m_lpTarget,&m_engageAbility);
			break;

		default	:
			m_lpField->sendActionToActor(this,m_lpTarget,&m_engageAbility);
			break;
	}
//	if (m_engageAbility.isBasicAttackSkill())	m_lpField->sendBasicAttack(this,m_lpTarget,&m_engageAbility);
//	else										m_lpField->sendActionToActor(this,m_lpTarget,&m_engageAbility);
	
	return iResult;
}


//복수-o-!!
BOOL
cACTOR::removeTargeting()
{
	m_lpTarget = NULL;
	m_lpCurrentTarget = NULL;
	m_lpLastAttacker = NULL;

	return TRUE;
}
//
//복수-o-!!
BOOL
cACTOR::revenge(cACTOR *_lpAttacker)
{
	if	(m_wBlockToActionTime)
		return	TRUE;

	if	(isPureMonster())
		operateMonsterCounterSkill(_lpAttacker);

	if	(m_wUpkeepTargetTime)
		return	FALSE;

	if	(m_wFleeTime)
	{
		flee(_lpAttacker,m_wFleeTime);

		return	FALSE;
	}

	if	(m_wLordActor	!=	0xffff		)
		return	FALSE;

	if	(isPureMonster() && _lpAttacker->isPlayer())
	{
		if (!_lpAttacker->addHostileEnemy(m_wSerialInField))
			return	FALSE;
	}

	if (isMonster() == FALSE || m_wConfuseTime == 0)
	{
		if	(m_lpTarget		==	_lpAttacker	)
			return	FALSE;	//	이미 전투중!!
	}

	if	(_lpAttacker	==	NULL		)
		return	FALSE;	//	없는디?

	if(_lpAttacker->m_wTransToWeaponTime)
	{
		_lpAttacker	=	m_lpField->getActor(_lpAttacker->m_wPrincessWeaponReceiver);
		if(!_lpAttacker)
			return FALSE;
	}
	setAiFight(_lpAttacker);

	//이 밑은 링크 몬스터의 Ai처리 
	operateDoubleLink(_lpAttacker);	

	return TRUE;
}
BOOL					
cACTOR::operateDoubleLink(cACTOR *_lpAttacker)
{
	cNPC	*lpNpc		=	getNpc();	
	
	if(!lpNpc)
		return FALSE;
	
	if(m_lpField->s_iFileVersion < 43)	//43 == eVVI_44_ADD_TOKKEN_SHOP 예전에 세이브 해놓은 맵테이터의 링크는 작동하지 않는다.
		return FALSE;

	if(!lpNpc->m_wLinkType)
		return FALSE;
	
	for (int i = 0 ; i < lpNpc->m_wLinkMonsterCount; i++)
	{
		int	iNpc	=	lpNpc->m_awLinkMonster[i];
		
		if(iNpc == 0xffff)
			break;
		
		cNPC	*lpTargetNpc	=	m_lpField->m_apNpcGenerateData[iNpc];
		
		if	(!lpTargetNpc || lpTargetNpc->m_wActorSerial	==	0xffff)
			continue;
		
		cACTOR	*lpTargetActor	=	m_lpField->getTestedActor(lpTargetNpc->m_wActorSerial);
		
		if	(!lpTargetActor	||	lpTargetActor->isDeath())
			continue;
		
		lpTargetActor->setAiFight(_lpAttacker);
	}
	
	
	return	TRUE;
}

//
//전투중지
void
cACTOR::stopBattle()
{
	if	(!isPlayer())
		return;

	setTarget(NULL);
	setCurrentTarget(NULL);

	m_engageAbility.reset();
}

int
cACTOR::actionResult(CSkill	*_lpSkill)
{
	if (_lpSkill->m_wType	==	eSKILL_TYPE_DOUBLE_TARGET			)	return	eAR_DOUBLE_TARGET;
	if (_lpSkill->m_wType	==	eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT	)	return	eAR_GROUP_CONTINUOUS_BY_BIT;
	if (_lpSkill->m_wType	==	eSKILL_TYPE_ATTACK_BY_BUNSHIN		)	return	eAR_BUNSHINE_ATTACK;
	if (_lpSkill->m_wType	==	eSKILL_TYPE_SCIMITAR_CUTTING		)	return	eAR_SCIMITAR_CUTTING;
	if (_lpSkill->m_wType	==	eSKILL_TYPE_BOUNCING_LINEAR			)	return	eAR_BOUNCING_LINEAR;

	if (_lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_VALUE	||
		_lpSkill->m_wType	==	eSKILL_TYPE_ALTERING_HITTER			||
		_lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_SHOOT		||
		_lpSkill->m_wType	==	eSKILL_TYPE_BIT_GLIDER				)
		return	eAR_CONTINUOUS_ATTACK_BY_VALUE;

	if (_lpSkill->m_wType	==	eSKILL_TYPE_LAND_MARKER				)	return	eAR_LAND_MARKER;
	if (_lpSkill->m_wType	==	eSKILL_TYPE_INTERVAL_SHOOTER		)	return	eAR_INTERVAL_SHOOTER;

	if (_lpSkill->m_wType	==	eSKILL_TYPE_ICY_STALAGMITE			)	return	eAR_ICY_STALAGMITE;

	if (_lpSkill->m_wType	==	eSKILL_TYPE_SHIMMERING_SHIELD		)	m_wUseSkill	=	0xffff;
	if (_lpSkill->m_wType	==	eSKILL_TYPE_ICY_STALAGMITE			)	m_wUseSkill	=	0xffff;
	if (_lpSkill->m_wType	==	eSKILL_TYPE_BOUNCING_MISSILE			)	return	eAR_BOUNCING_MISSILE;
	if (_lpSkill->m_wType	==	eSKILL_TYPE_WIDE_MISSILE				)	return	eAR_WIDE_MISSILE;
	return	eAR_OK;
}

int
cACTOR::getDamageFrame(int _iTimes)
{
	cBODY	*lpBody	=	getBody();

	if	(_iTimes	==	-1)
	{
		for (int i=m_iFrameCount;i;--i)
		{
			if	(lpBody->isDamage(m_iAnm,i-1))
				return	i-1;
		}
	}
	else
	{
		int		iCount	=	-1;

		for (int i=0;i<m_iFrameCount;i++)
		{
			if	(lpBody->isDamage(m_iAnm,i))
			{
				iCount++;
				
				if	(_iTimes	==	iCount)
					return	i;
			}
		}
	}

	return	0xffff;
}

int
cACTOR::getTriggerFrame()
{
	cBODY	*lpBody	=	getBody();

	for (int i=m_iFrameCount;i;--i)
		if	(lpBody->isTrigger(m_iAnm,i-1))
			return	i-1;

	return	0xffff;
}

int
cACTOR::getFirstTriggerFrame()
{
	cBODY	*lpBody	=	getBody();

	for (int i=0;i<m_iFrameCount;i++)
		if	(lpBody->isTrigger(m_iAnm,i-1))
			return	i;

	return	0xffff;
}
int
cACTOR::getFirstTriggerFrame(int _iAnm)
{
	cBODY	*lpBody	=	getBody();
	if(!lpBody)
		return 0xffff;
	int iFrameCount = lpBody->getFrameCount(_iAnm);
	for (int i=0;i<iFrameCount;i++)
		if	(lpBody->isTrigger(_iAnm,i-1))
			return	i;

	return	0xffff;
}
int
cACTOR::getDamageFrameInAnm(int _iAnm, int _iTimes)
{
	cBODY	*lpBody	=	getBody();
	if(!lpBody)
		return 0xffff;
	int iFrameCount = lpBody->getFrameCount(_iAnm);
	if	(_iTimes	==	-1)
	{
		for (int i=iFrameCount;i;--i)
		{
			if	(lpBody->isDamage(_iAnm,i-1))
				return	i-1;
		}
	}
	else
	{
		int		iCount	=	-1;

		for (int i=0;i<iFrameCount;i++)
		{
			if	(lpBody->isDamage(_iAnm,i))
			{
				iCount++;
				
				if	(_iTimes	==	iCount)
					return	i;
			}
		}
	}

	return	0xffff;
}
DWORD
cACTOR::getFirstTriggerTime()
{
	if	(m_iFPS	==	0)
		m_iFPS	=	max(getBody()->getFPS(m_iAnm),1);

	int		iFrame	=	getFirstTriggerFrame();
	DWORD	dwTime	=	g_dwCurrentTime+(iFrame*1000/m_iFPS/dREVISE_ACTION_SPEED_FOR_TEST);

	if (dwTime	<	g_dwCurrentTime)
		dwTime	=	g_dwCurrentTime;

	return	dwTime;
}

DWORD
cACTOR::getTriggerTime()
{
	if	(m_iFPS	==	0)
		m_iFPS	=	max(getBody()->getFPS(m_iAnm),1);

	int		iFrame	=	getTriggerFrame();
	DWORD	dwTime	=	g_dwCurrentTime+(iFrame*1000/m_iFPS/dREVISE_ACTION_SPEED_FOR_TEST);

	if (dwTime	<	g_dwCurrentTime)
		dwTime	=	g_dwCurrentTime;

	return	dwTime;
}

DWORD
cACTOR::getDamageFrameTime(int _iTimes)
{
	if	(m_iFPS	==	0)
		m_iFPS	=	max(getBody()->getFPS(m_iAnm),1);

	int		iFrame	=	getDamageFrame(_iTimes);

	if	(iFrame	==	0xffff)
		return	0xffffffff;

	DWORD	dwTime	=	g_dwCurrentTime+(iFrame*1000/m_iFPS/dREVISE_ACTION_SPEED_FOR_TEST);

	if (dwTime	<	g_dwCurrentTime)
		dwTime	=	g_dwCurrentTime;

	return	dwTime;
}

DWORD
cACTOR::getFrameTime(int _iFrame)
{
	cBODY	*lpBody	=	getBody();

	if	(m_iFPS	==	0)
		m_iFPS	=	max(lpBody->getFPS(m_iAnm),1);

	if	(_iFrame	==	-1)
		_iFrame	=	lpBody->getFrameCount(m_iAnm);

	_iFrame	=	min(_iFrame,lpBody->getFrameCount(m_iAnm));

	DWORD	dwTime	=	g_dwCurrentTime+(_iFrame*1000/m_iFPS/dREVISE_ACTION_SPEED_FOR_TEST);

	if	(dwTime	<	g_dwCurrentTime)
		dwTime	=	g_dwCurrentTime;

	return	dwTime;
}


//
//	기본공격 스킬로 공격한다.


int
cACTOR::attackToActorByBasicAttackSkill(cACTOR *_lpTarget,cAbility *_lpAbility)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	m_wIsDontHitAction	=	lpSkill->isNotInterrupt();

	action(lpSkill->m_wAction);
	setExclusiveAction(TRUE);	//	기술 사용하는건 에니메이션 하나로 끝난다. 일단은.. --;;
	m_attackInfo.setBasicData(_lpAbility,_lpTarget->m_wSerialInField);
	
	int iAttackCount =0;
	iAttackCount = lpSkill->m_wReiterationDamageCountSyncWithOverlapAction;
	if(m_fightEnergy.isIncreaseAttackCount())
		iAttackCount += m_fightEnergy.m_iValue;
	iAttackCount = min(dMAX_CONTINUOUS_HIT_INFO_COUNT, iAttackCount);

	if(iAttackCount > 1)
	{
		for (int i=0;i<iAttackCount;i++)
		{
			CHitInfo	*lpHitInfo		=	m_attackInfo.getInfo(i);

			lpHitInfo->reset(TRUE);
			_lpTarget->getDamage(this,_lpAbility,0,0,lpHitInfo,TRUE,TRUE,TRUE,iAttackCount,i);
		}
	}
	else
	{
		_lpTarget->getDamage(this,_lpAbility,0,0,m_attackInfo.getInfo(0));
	}

	m_wUseSkill	=	dSKILL_BASIC_ATTACK;//기본공격

	reduceCP(_lpAbility->getSpentCP(),_lpAbility->m_wSkill);	//CP 감소

	setActionSpeed(_lpAbility);	//fps와 현재 액션이 끝나는데 필요한 프레임을 얻어서 설정한다.
	decreaseBullet(_lpAbility);

	int	iDamageFrame	=	getDamageFrame();

	if	(iDamageFrame	!=	0xffff)
	{
		if	(m_iFPS	==	0)
			m_iFPS	=	getBody()->getFPS(m_iAnm);

		int	iRequireFrame			=	iDamageFrame*dSYNC_FPS/m_iFPS;

		m_dwBookedBasicAttackTime	=	g_dwCurrentTime	+	iRequireFrame*1000/dSYNC_FPS/dREVISE_ACTION_SPEED_FOR_TEST;
		m_dwBookedActionFinishTime	=	g_dwCurrentTime	+	m_wRemainActionTime*1000/dSYNC_FPS/dREVISE_ACTION_SPEED_FOR_TEST;

		if	(m_dwBookedActionFinishTime	<	m_dwBookedBasicAttackTime)
			m_dwBookedActionFinishTime	=	m_dwBookedBasicAttackTime;
	}
	
	setCoolTime(_lpAbility);
	setLastUseSkill(_lpAbility->m_wSkill);

	return eAR_OK;
}


int
cACTOR::attackToObject(int _iObject,int _iSerial)
{
	if (m_wUseSkill	!=	0xffff	&&	!m_isExclusiveAction)
	{
		CLOG("skillLog","얼래? 스킬 사용 중인데 독점 모드가 아니네.. -o-");
	}

	if (m_wRemainActionTime	)	
		return	eAR_BUSY;
	if (m_isExclusiveAction)	
		return	eAR_BUSY;

	int	iAbility	=	getDefaultAttackAbilityIndex();

	if	(iAbility	==	-1)
		return	eAR_BUSY;

	cAbility	*lpAbility	=	&m_aAbility[iAbility];

	if	(m_wSealActionTime)
		if	(lpAbility->m_wSkill	!=	m_wUnsealAction)
			return	eAR_BUSY;

	int	iResult	=	isUseAbleSkill(lpAbility);

	if (iResult	!=	eAR_OK)
		return	iResult;

	CSkill	*lpSkill			=	lpAbility->getSkill();

	int		iRemainDurability	=	0;

	m_wAttackToObjectType		=	0xffff;
	m_wAttackToObject			=	0xffff;

	switch(	_iObject)
	{
		case	eATTACK_OBJECT_DOOR	:
		{
			cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)m_lpField->getArea(_iSerial);
			cAreaInfo		*lpFocusedArea = m_lpField->getArea(_iSerial);

			if(!lpFocusedArea || !lpFocusedArea->isIn(m_pos.x,m_pos.y))
				return eAR_FAILED;

			if (!lpDoor || lpDoor->m_wKind	!=	dAREA_DOOR)
				return	eAR_IS_CAN_NOT_ATTACK_TARGET;

			if	(lpDoor->m_bf1IsUnlocked)
				return	eAR_ALREADY_OPEN_ARCA;


			break;
		}

		case	eATTACK_OBJECT_ARCA	:
		{
			cArca	*lpArca		=	g_arcaManager.get(_iSerial);
			cAreaInfo		*lpFocusedArea = m_lpField->getArea(_iSerial);

			if(!lpFocusedArea || !lpFocusedArea->isIn(m_pos.x,m_pos.y))
				return eAR_FAILED;

			if (!lpArca)
				return	eAR_IS_CAN_NOT_ATTACK_TARGET;

			if	(lpArca->m_bLockLevel	==	0)
				return	eAR_ALREADY_OPEN_ARCA;
			break;
		}
	}
	stopAction();		//	정지!!

	setLastUseSkill(lpAbility->m_wSkill);
	m_attackInfo.setBasicData(lpAbility,_iSerial);

	switch(	_iObject)
	{
		case	eATTACK_OBJECT_DOOR	:
		{
			cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)m_lpField->getArea(_iSerial);

			lpDoor->getDamage(this,lpAbility,m_attackInfo.getInfo(0));

			iRemainDurability		=	max(lpDoor->m_iCurrentDurability-m_attackInfo.m_aInfo[0].getDamage(),0);
			LONGLONG	llValue		=	iRemainDurability*100;
			int	iDur				=	max(lpDoor->m_iDurability,1);
			iRemainDurability		=	(int)(llValue/iDur);
			m_wUseSkill				=	dSKILL_BASIC_ATTACK_TO_DOOR;//기본공격
			break;
		}

		case	eATTACK_OBJECT_ARCA	:
		{
			cArca	*lpArca			=	g_arcaManager.get(_iSerial);

			lpArca->getDamage(this,lpAbility,m_attackInfo.getInfo(0));
			iRemainDurability		=	max(lpArca->m_iDurability-m_attackInfo.m_aInfo[0].getDamage(),0);
			LONGLONG	llValue		=	iRemainDurability*100;
			int	iDur				=	max(lpArca->m_iMaxDurability,1);
			iRemainDurability		=	(int)(llValue/iDur);
			m_wUseSkill				=	dSKILL_BASIC_ATTACK_TO_ARCA;//기본공격
			break;
		}
	}

	m_wIsDontHitAction		=	lpSkill->isNotInterrupt();

	action(lpSkill->m_wAction);
	setExclusiveAction(TRUE);	//	기술 사용하는건 에니메이션 하나로 끝난다. 일단은.. --;;

	reduceCP(lpAbility->getSpentCP(),lpAbility->m_wSkill);	//CP 감소

	m_wAttackToObjectType	=	_iObject;
	m_wAttackToObject		=	_iSerial;

	setActionSpeed(lpAbility);	//	fps와 현재 액션이 끝나는데 필요한 프레임을 얻어서 설정한다.
	decreaseBullet(lpAbility);

	int	iDamageFrame	=	getDamageFrame();

	if (iDamageFrame	!=	0xffff)
	{
		if	(m_iFPS	==	0)
			m_iFPS	=	max(getBody()->getFPS(m_iAnm),1);

		int	iRequireFrame			=	iDamageFrame*dSYNC_FPS/m_iFPS;

		m_dwBookedBasicAttackTime	=	g_dwCurrentTime	+	iRequireFrame*1000/dSYNC_FPS/dREVISE_ACTION_SPEED_FOR_TEST;
		m_dwBookedActionFinishTime	=	g_dwCurrentTime	+	m_wRemainActionTime*1000/dSYNC_FPS/dREVISE_ACTION_SPEED_FOR_TEST;

		if	(m_dwBookedActionFinishTime	<	m_dwBookedBasicAttackTime)
			m_dwBookedActionFinishTime	=	m_dwBookedBasicAttackTime;
	}

	m_lpField->addSendAttackToObject(this,_iSerial,_iObject,iRemainDurability);

	setCoolTime(lpAbility);

	return eAR_OK;
}

//
//	연속공격 스킬로 공격한다.
int
cACTOR::attackToActorByContinuousHitSkill(cACTOR *_lpTarget,cAbility *_lpAbility)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	m_wIsDontHitAction	=	lpSkill->isNotInterrupt();

	action(lpSkill->m_wAction);
	setExclusiveAction(TRUE);	//	기술 사용하는건 에니메이션 하나로 끝난다. 일단은.. --;;

	m_attackInfo.setBasicData(_lpAbility,_lpTarget->m_wSerialInField);

	int	iDamageFrameCount	=	0;

	for (int iFrame=0;iFrame<m_iFrameCount;iFrame++)
	{
		if	(getBody()->isDamage(m_iAnm,iFrame))
		{
			CHitInfo	*lpInfo	=	m_attackInfo.getInfo(iDamageFrameCount);

			_lpTarget->getDamage(this,_lpAbility,0,0,lpInfo);

			if	(lpInfo->isInstanceKill() && iDamageFrameCount!=0)
				lpInfo->m_dwResultField	-=	dATTACK_RESULT_FIELD_INSTANCE_KILL;
			else
			if	(lpInfo->isDodge())
				lpInfo->reset(TRUE);

			iDamageFrameCount++;
		}
	}

	m_wUseSkill							=	dSKILL_CONTINUOUS_HIT_ATTACK;//연타
	m_wContinuousHitCount				=	0;
	m_isGetCPByContinuousHitFirstAttack	=	FALSE;

	reduceCP(_lpAbility->getSpentCP(),_lpAbility->m_wSkill);	//CP 감소

	setActionSpeed(_lpAbility);	//fps와 현재 액션이 끝나는데 필요한 프레임을 얻어서 설정한다.
	decreaseBullet(_lpAbility);

	setCoolTime(_lpAbility);
	setLastUseSkill(_lpAbility->m_wSkill);

	return eAR_OK;
}

//
//	연속공격 스킬로 공격한다.
int
cACTOR::attackToActorByIllusionAttackSkill(cACTOR *_lpTarget,cAbility *_lpAbility)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	m_wIsDontHitAction	=	lpSkill->isNotInterrupt();

	action(lpSkill->m_wAction);
	setExclusiveAction(TRUE);	//	기술 사용하는건 에니메이션 하나로 끝난다. 일단은.. --;;

	m_attackInfo.setBasicData(_lpAbility,_lpTarget->m_wSerialInField);

	int	iShotCount	=	min(_lpAbility->getContinuousShotCount(),dMAX_CONTINUOUS_HIT_INFO_COUNT);

	if (m_lpField->m_bIsGuildBattleField)
	{
		int	iPhysicalDamage	=	0,iMagicDamage	=	0,iMissCount	=	0,iCriticalCount=0,iDoubleCriticalCount=0;
		int	iMinPhysicalDamage,iMinMagicDamage,iBlockingCount,iInstanceKillCount,iNoActionBlockCount,iDancingBlockCount;
		int	iHardBlowCount = 0;
		getContinuoueAttackDamage(_lpAbility,_lpTarget,iShotCount,&iPhysicalDamage,&iMagicDamage,&iMissCount,&iCriticalCount,&iDoubleCriticalCount,
			&iMinPhysicalDamage,&iMinMagicDamage,&iBlockingCount,&iInstanceKillCount,&iNoActionBlockCount,&iDancingBlockCount, &iHardBlowCount);
	}
	else
	for (int i=0;i<iShotCount;i++)
	{
		CHitInfo	*lpHitInfo	=	m_attackInfo.getInfo(i);

		_lpTarget->getDamage(this,_lpAbility,0,0,lpHitInfo,TRUE,TRUE,TRUE,iShotCount,i);

		if	(lpHitInfo->isInstanceKill() && i!=0)
			lpHitInfo->m_dwResultField	-=	dATTACK_RESULT_FIELD_INSTANCE_KILL;
	}

	m_wUseSkill							=	dSKILL_ILLUSION_ATTACK;//연타
	m_wContinuousHitCount				=	0;
	m_isGetCPByContinuousHitFirstAttack	=	FALSE;
	m_bIllusionAttackFrame				=	0;

	reduceCP(_lpAbility->getSpentCP(),_lpAbility->m_wSkill);	//CP 감소
	decreaseBullet(_lpAbility);

	setActionSpeed(_lpAbility);	//fps와 현재 액션이 끝나는데 필요한 프레임을 얻어서 설정한다.

	int	iDamageFrame	=	getDamageFrame();

	if	(iDamageFrame	!=	0xffff)
	{
		if	(m_iFPS	==	0)
			m_iFPS	=	getBody()->getFPS(m_iAnm);

		int	iRequireFrame			=	iDamageFrame*dSYNC_FPS/m_iFPS;
		int	iFrameCount				=	(m_iFrameCount+_lpAbility->getContinuousShotPeriod()*(iShotCount-1))*dSYNC_FPS/m_iFPS;

		m_dwBookedBasicAttackTime	=	g_dwCurrentTime	+	iRequireFrame*1000/dSYNC_FPS/dREVISE_ACTION_SPEED_FOR_TEST;
		m_dwBookedActionFinishTime	=	0;
	}

	m_wRemainActionTime	=	0;

	setCoolTime(_lpAbility);
	setLastUseSkill(_lpAbility->m_wSkill);

	return eAR_OK;
}

//
//	_iSerial을 공격
BOOL
cACTOR::attackToActor(cACTOR *_lpTarget,cAbility *_lpAbility)
{
	if	(m_wUseSkill	!=	0xffff	&&	!m_isExclusiveAction)
	{
		CLOG("skillLog","얼래? 스킬 사용 중인데 독점 모드가 아니네.. -o-");
	}

	if	(m_wRemainActionTime	)	
		return	eAR_BUSY;
	if	(m_isExclusiveAction)	
		return	eAR_BUSY;

	if	(m_wSealActionTime)
		if	(_lpAbility->m_wSkill	!=	m_wUnsealAction)
			return	eAR_BUSY;
	
	if	(!_lpTarget)
		return	eAR_FAILED;

	if	(_lpTarget->m_wUntimateBarrierTime)
	{
		int	iCheckRange	=	_lpTarget->m_wUntimateBarrierRange;
		iCheckRange		*=	iCheckRange;

		int	iCurRange	=	GetOvalRange(m_pos.x,m_pos.y,_lpTarget->m_pos.x,_lpTarget->m_pos.y);

		if	(iCurRange	<=	iCheckRange	)
			return	eAR_IS_UNDER_TARGET_ULTIMATE_BARRIER;
	}

	int	iResult	=	isUseAbleSkill(_lpAbility);

	if	(iResult	!=	eAR_OK)
	{
//		if (m_lpTarget	&&	m_lpTarget->m_wSerialInField == _lpTarget->m_wSerialInField)	stopBattle();

		return	iResult;
	}

	if	(m_lpField->isMissileBlockedLine(m_pos.x,m_pos.y,_lpTarget->m_pos.x,_lpTarget->m_pos.y))
		return	eAR_BLOCKED_OBJECT_EXIST;

	releaseCompleteProtection();

	stopAction();		//	정지!!

	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(isPureMonster())
		if	(lpSkill->isAgressiveSkill() && _lpTarget && (_lpTarget->isPlayer() || _lpTarget->isPet() || _lpTarget->isSummonBeast()))
		{
			if	(_lpTarget->m_dwLastAttackedTime + dCHECK_CONTINUOUS_ATTACKED_TIME >= (DWORD)m_lpField->m_iFrameCounter)	//	타겟 입장에서 공격 당하고 5프레임 안에 또 공격 당했다.
				_lpTarget->m_wContinuousAttackedCounter++;
			else
				_lpTarget->m_wContinuousAttackedCounter	=	0;

			_lpTarget->m_dwLastAttackedTime	=	m_lpField->m_iFrameCounter;
		}

	if	(isPlayer())
	{
		cItem	*lpWeapon	=	getWeapon();
		int		iAttackRange=	getAttackRange(_lpAbility) + getBodySize();	//	공격범위 체크

		if	(_lpTarget->m_bIsMoving)
			iAttackRange	+=	_lpTarget->m_wPPS;

		if	(!lpSkill->isQuickCastSkill())
			if	(!isTargetInAttackRange(_lpTarget,iAttackRange))
				return	eAR_FAILED;
	}

	//업데이트 차징스킬.
 	iMiniPet()->updateChargeCount();
	if(iResult == eAR_OK)
	{	//필요 상태 해제.
		cancelNeedState(lpSkill);	
	}

	if (lpSkill->m_wType==	eSKILL_TYPE_NORMAL_HIT					)
		return	attackToActorByBasicAttackSkill(_lpTarget,_lpAbility);
	if (lpSkill->m_wType==	eSKILL_TYPE_CONTINUOUS_HIT_BY_CHARACTER	)
		return	attackToActorByContinuousHitSkill(_lpTarget,_lpAbility);
	if (lpSkill->m_wType==	eSKILL_TYPE_CONTINUOUS_HIT_BY_ILLUSION	)
		return	attackToActorByIllusionAttackSkill(_lpTarget,_lpAbility);

	m_wUseSkill		=	m_lpField->m_skillManager.castAtActor(this,_lpTarget,_lpAbility,getAttackRange(_lpAbility));

	if	(m_wUseSkill	==	0xffff)		//	스킬 사용 실패
	{
		stopAction();		//	정지!!

		return eAR_FAILED;
	}

	{
		CActiveSkill	*lpActiveSkill	=	getActiveSkill();

		if	(lpActiveSkill && lpActiveSkill->m_isNotSpentCP	==	FALSE)	
			reduceCP(_lpAbility->getSpentCP(),_lpAbility->m_wSkill);	//	CP 감소
	}

	setActionSpeed(_lpAbility);	//fps와 현재 액션이 끝나는데 필요한 프레임을 얻어서 설정한다.

	decreaseBullet(_lpAbility);

	setCoolTime(_lpAbility);

	setLastUseSkill(_lpAbility->m_wSkill);

	return	actionResult(lpSkill);
}

//
//땅에다 대고 공격!! -o-!!
int
cACTOR::actionToGround(int _iX,int _iY,cAbility *_lpAbility)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(lpSkill->isRequireTargetSkill()	)
		return	eAR_FAILED;
	if	(m_wRemainActionTime				)
		return	eAR_BUSY;
	if	(m_isExclusiveAction				)
		return	eAR_BUSY;
	if	(m_wSealActionTime)
	{
		if	(_lpAbility->m_wSkill	!=	m_wUnsealAction)
			return	eAR_BUSY;
	}

	if (isPlayer())
	{
		int	iResult	=	isUseAbleSkill(_lpAbility);

		if (iResult	!=	eAR_OK)
			return	iResult;

		int		iAttackRange=	getAttackRange(_lpAbility) + getBodySize();	//	공격범위 체크

		iAttackRange		*=	iAttackRange;

		if (GetOvalRange(m_pos.x,m_pos.y,_iX,_iY)	>	iAttackRange)
			return	eAR_FAILED;

		if	(m_lpField->isMissileBlockedLine(m_pos.x,m_pos.y,_iX,_iY))
			return	eAR_BLOCKED_OBJECT_EXIST;
	}

	releaseCompleteProtection();

	stopAction();	//	정지!!

	setLastUseSkill(_lpAbility->m_wSkill);
	m_wUseSkill		=	m_lpField->m_skillManager.castAtGround(this,_iX,_iY,_lpAbility,getAttackRange(_lpAbility));

	if (m_wUseSkill	==	0xffff)		//	스킬 사용 실패
	{
		//	여기서 일단 실패 원인도 파악해야 한다. 너무 멀다거나..

		stopAction();		//	정지!!

		return eAR_FAILED;
	}

	{
		CActiveSkill	*lpActiveSkill	=	getActiveSkill();

		if (lpActiveSkill && lpActiveSkill->m_isNotSpentCP	==	FALSE)
			reduceCP(_lpAbility->getSpentCP(),_lpAbility->m_wSkill);	//	CP 감소
	}

	setActionSpeed(_lpAbility);	//fps와 현재 액션이 끝나는데 필요한 프레임을 얻어서 설정한다.
	decreaseBullet(_lpAbility);

	setCoolTime(_lpAbility);

	return actionResult(lpSkill);
}

//
//	땅에다 대고 공격!! -o-!!
int
cACTOR::actionQuick(cAbility *_lpAbility)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if (lpSkill->isRequireTargetSkill()	)
		return	eAR_REQUIRE_TARGET_SKILL;
	if (m_wRemainActionTime				)
		return	eAR_BUSY;
	if (m_isExclusiveAction				)
		return	eAR_BUSY;

	if	(m_wSealActionTime)
		if (_lpAbility->m_wSkill	!=	m_wUnsealAction)
			return	eAR_BUSY;

	if	(isPlayer())
	{
		int	iResult	=	isUseAbleSkill(_lpAbility);

		if (iResult	!=	eAR_OK)
		{
//			if (m_lpTarget)	stopBattle();
			
			return	iResult;
		}
	}

	stopAction();	//	정지!!
//
//	컴플리트 프로텍션이다!!
	if (lpSkill->m_wType	==	eSKILL_TYPE_COMPLETE_PROTECTION)
	{
		if	(!isActiveCompleteProtection())
		{
			reduceCP(_lpAbility->getSpentCP(),_lpAbility->m_wSkill);
		}

		toggleSpecialSkillField(dSSF_COMPLETE_PROTECTION);

		return eAR_SET_SPECIAL_SKILL_FIELD;
	}

	releaseCompleteProtection();

	setLastUseSkill(_lpAbility->m_wSkill);

	int	iResult;

	RemoveInvincibleStateException(_lpAbility);

	m_wUseSkill		=	m_lpField->m_skillManager.castQuick(this,_lpAbility,getAttackRange(_lpAbility),&iResult);

	if	(m_wUseSkill	==	0xffff)		//	스킬 사용 실패
	{
		stopAction();		//	정지!!

		if	(iResult	==	eSKILL_FIRE_RESULT_CAN_NOT_FIND_TARGET_IN_ATTACK_RANGE)
			return	eAR_CAN_NOT_FIND_TARGET_IN_ATTACK_RANGE;

		if	(iResult	==	eSKILL_FIRE_RESULT_NOT_APT_SKILL_TO_THIS_STATUS)
			return	eAR_CAN_NOT_APT_SKILL_TO_THIS_STATUS;

		return eAR_FAILED;
	}

	{
		CActiveSkill	*lpActiveSkill	=	getActiveSkill();

		if (lpActiveSkill && lpActiveSkill->m_isNotSpentCP	==	FALSE)
			reduceCP(_lpAbility->getSpentCP(),_lpAbility->m_wSkill);	//	CP 감소
	}

	setActionSpeed(_lpAbility);	//fps와 현재 액션이 끝나는데 필요한 프레임을 얻어서 설정한다.
	decreaseBullet(_lpAbility);

	setCoolTime(_lpAbility);

	return actionResult(lpSkill);
}

//
//	땅에다 대고 공격!! -o-!!
int
cACTOR::actionToObject(cAbility *_lpAbility,int _iObjectType,int _iObjectIndex)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(m_wRemainActionTime)
		return	eAR_BUSY;
	if	(m_isExclusiveAction)
		return	eAR_BUSY;
	if	(m_wSealActionTime)
		return	eAR_BUSY;

	if	(isPlayer())
	{
		int	iResult		=	isUseAbleSkill(_lpAbility);

		if	(iResult	!=	eAR_OK)
			return	iResult;
	}

	stopAction();	//	정지!!

	setLastUseSkill(_lpAbility->m_wSkill);

	cAbility	ability;

	ability.copy(_lpAbility);
	ability.m_wLevel	=	_lpAbility->m_wLevel + getCorrectSkillLevel(_lpAbility);

	m_attackInfo.setBasicData(&ability,0xffff);

	cPOINT	posTarget;
	int		iDirect	=	8;

	switch(_iObjectType)
	{
		case	eTARGET_OBJECT_TRAP			:
		{
			cTrap	*lpTrap	=	m_lpField->getTrap(_iObjectIndex);

			if	(!lpTrap)
				return	eAR_NOT_APT_USED_SKILL_TO_TARGET;

			iDirect	=	getDirect(lpTrap->m_pos.x,lpTrap->m_pos.y,FALSE);
			
			break;
		}
		case	eTARGET_OBJECT_FIELD_TRAP	:
		{
			cAreaInfo	*lpArea	=	m_lpField->getArea(_iObjectIndex);

			if	(!lpArea)
				return	eAR_NOT_APT_USED_SKILL_TO_TARGET;

			lpArea->getCenterPos(&posTarget);

			iDirect	=	getDirect(posTarget.x,posTarget.y,FALSE);
			break;
		}
		case	eTARGET_OBJECT_ARCA			:
		{
			cArca		*lpArca	=	g_arcaManager.get(_iObjectIndex);

			if	(!lpArca)
				return	eAR_NOT_APT_USED_SKILL_TO_TARGET;

			iDirect	=	getDirect(lpArca->m_pos.x,lpArca->m_pos.y,FALSE);
			break;
		}
		case	eTARGET_OBJECT_DOOR			:
		{
			cAreaInfo	*lpArea	=	m_lpField->getArea(_iObjectIndex);

			if	(!lpArea)
				return	eAR_NOT_APT_USED_SKILL_TO_TARGET;

			lpArea->getCenterPos(&posTarget);

			iDirect	=	getDirect(posTarget.x,posTarget.y,FALSE);
			break;
		}
	}

	m_bf3TargetObjectType	=	_iObjectType;
	m_bf13TargetObject		=	_iObjectIndex;
	m_wUseSkill				=	dSKILL_ACTION_TO_OBJECT;//기본공격

	m_wIsDontHitAction		=	lpSkill->isNotInterrupt();

	action(lpSkill->m_wAction);
	setExclusiveAction(TRUE);	//	기술 사용하는건 에니메이션 하나로 끝난다. 일단은.. --;;

	reduceCP(_lpAbility->getSpentCP(),_lpAbility->m_wSkill);		//	CP 감소

	setActionSpeed(_lpAbility);	//fps와 현재 액션이 끝나는데 필요한 프레임을 얻어서 설정한다.

	m_lpField->sendActionQuick(this,_lpAbility,iDirect);

	int	iDamageFrame	=	getTriggerFrame();

	if	(iDamageFrame	!=	0xffff)
	{
		if	(m_iFPS	==	0)
			m_iFPS	=	getBody()->getFPS(m_iAnm);

		int	iRequireFrame			=	iDamageFrame*dSYNC_FPS/m_iFPS;

		m_dwBookedBasicAttackTime	=	g_dwCurrentTime	+	iRequireFrame*1000/dSYNC_FPS/dREVISE_ACTION_SPEED_FOR_TEST;
		m_dwBookedActionFinishTime	=	g_dwCurrentTime	+	m_wRemainActionTime*1000/dSYNC_FPS/dREVISE_ACTION_SPEED_FOR_TEST;

		if	(m_dwBookedActionFinishTime	<	m_dwBookedBasicAttackTime)
			m_dwBookedActionFinishTime	=	m_dwBookedBasicAttackTime;
	}

	setCoolTime(_lpAbility);
	
	return	eAR_OK;
}

//
//	방어력을 구한다.
int
cACTOR::getDefensivePower()
{
	
	LONGLONG	llDefensivePower				=	m_defensePower.m_iValue+m_armorPower.getCorrectedValue();
	int			iCorrectDefenseValue			=	(m_defensePower.m_sCorrectValueByPrefix+m_defensePower.m_sCorrectValue)*100;
	int			iCorrectBySkillDefense			=	getDefensePowerByPassiveSkill();
	int			iCorrectPercentageBySkillDefense=	getDefensePercentageByPassiveSkill();
	
	llDefensivePower	=	llDefensivePower*(100 +m_defensePower.getCorrectPercentageValue()+iCorrectPercentageBySkillDefense)/100 + iCorrectDefenseValue+iCorrectBySkillDefense;
	llDefensivePower	=	llDefensivePower*(100 +getConstitution())/100;
	
	llDefensivePower	+=	llDefensivePower*m_sCorrectFinalDefensivePowerPercentage/100;
	
	if	(m_wBreakedArmorTime)
		llDefensivePower /=	dBREAK_EQUIPMENT_FACTOR;
	
	return	max((int)llDefensivePower,1);
	
}

//
//	공격력을 구한다.

int
cACTOR::getPhysicalAttackPower(cAbility *_lpAbility,int _iTargetLevel,BOOL _bIsPVP,BOOL _bIsBossBattle,CHitInfo *_lpHitInfo,BOOL _bIsWantMinDamage,cACTOR *_lpTarget)
{
	if	(m_iGuildGuardianType	==	eGUILD_GUARDIAN_HOLLOW_KNIGHT)
	{
		int	iAP	=	2000+1800*m_iLevel;
		int	iRV	=	random(2*200*m_iLevel)-200*m_iLevel;

		return	iAP+iRV;
	}

	int		iSkillDamage			=	0;
	int		iSkillDamagePercentage	=	100;
//	공격력 = (무기의 기본 공격력 * 아이템, 기술에 의한 보정율 총합 + 아이템, 기술에 의한 보정치 총합) * (1 + 힘 * 0.01)
//	공격력 = ((무기의 기본 공격력) * (아이템, 기술에 의한 보정율 총합 + 아이템, 기술에 의한 보정치 총합 + 레벨 보정)) * (1 + (힘 * 0.005))  * (스킬의 거리 보정율)

//	소수점 2자리 까지 공격한다. 눈에 보이는 데미지는 /100을 해야 한다.
	int		iDamage			=	0;
	int		iMinDamage		=	0;
	int		iMaxDamage		=	0;
	int		iWeaponMinDamage=	m_iWeaponMinDamage+m_weaponPower.getCorrectValue();
	int		iWeaponMaxDamage=	m_iWeaponMaxDamage+m_weaponPower.getCorrectValue();
	int		iDamageGab;
	int		iAddDamagePercentage	=	0;

	CSkill	*lpSkill;

	if	(_lpAbility)
		lpSkill	=	_lpAbility->getSkill();

	if(m_wShadowHideAddPhysicalPowerPercent)
	{

		if(lpSkill && lpSkill->m_bf1IsApplyShadowHideEffect)
			iAddDamagePercentage = m_wShadowHideAddPhysicalPowerPercent;

		m_wShadowHideAddPhysicalPowerPercent = 0;
	}

	if	(lpSkill	&&	lpSkill->m_bf1AttackByDamagedDamage)
	{
		LONGLONG	llDamage	=	m_iLastDamagePoint;
		llDamage				=	llDamage*_lpAbility->getAttackPowerPercentage(NULL)/100;
		iDamage					=	(int)llDamage;
		int	iFightSpiritValue	=	0;
		if(lpSkill->m_bf1IsApplyFightingSpirit)
		{
			if(m_wFightingSpiritTime)
				iFightSpiritValue	=	m_iFightingSpiritValue;
		}
		if	(lpSkill->m_bf1RootAttackPower)
			iDamage	=	(int)GetRootDamage(iDamage);
		iDamage	=	iDamage+iDamage*(m_wIncreaseFinalPhysicalAttackPower - m_sCorrectFinalAttackPowerPercentage + iFightSpiritValue)/100.0;		//최종공격력감소 퍼센티지...적용.
		
		if(m_fightEnergy.isAddAbsoluteAttackPower())
			iDamage += m_fightEnergy.m_iValue;
		
		return	iDamage;
	}

	int		iCorrectWeaponDamagePercentage	=	100+m_weaponPower.getCorrectPercentageValue();

	iCorrectWeaponDamagePercentage	=	max(0,iCorrectWeaponDamagePercentage);

	iWeaponMinDamage				=	iWeaponMinDamage*iCorrectWeaponDamagePercentage/100;
	iWeaponMaxDamage				=	iWeaponMaxDamage*iCorrectWeaponDamagePercentage/100;

	if	(lpSkill)
	{
		if(_lpTarget && !_lpTarget->isLightAdditionalEffect() && lpSkill->m_bf1IsApplyMinDamageWhenBeNotLightEffect )
			_bIsWantMinDamage	=	TRUE;

		if	(lpSkill->m_bf1IsIgnoreWeaponDamage)
		{
			iWeaponMinDamage=	0;
			iWeaponMaxDamage=	0;
			iDamageGab		=	0;
		}

		iSkillDamage			=	_lpAbility->getAttackPower() + lpSkill->m_sAttackPointPerActorLevel*getLevel();
		iSkillDamagePercentage	=	_lpAbility->getAttackPowerPercentage(_lpTarget);
		iSkillDamagePercentage	+=	iAddDamagePercentage;
		if	(lpSkill->m_bf1IsAttackDamageBasedAttackerRemainHPRate)
			iSkillDamagePercentage	+=	getRemainHPRate();

		if	(_lpAbility->isRequireBullet())
		{
			cItem	*lpBullet	=	getBullet();

			if	(lpBullet)
			{
				iWeaponMinDamage	+=	lpBullet->getBasicItem()->m_wMinDamage*100;
				iWeaponMaxDamage	+=	lpBullet->getBasicItem()->m_wMaxDamage*100;
			}
		}
		if	(_lpAbility->isRequireOtherJobWeapon())
		{
			cItem	*lpWeapon	=	getOtherJobWeapon();

			if (lpWeapon)
			{
				iWeaponMinDamage	+=	lpWeapon->getBasicItem(TRUE)->m_wMinDamage*100;
				iWeaponMaxDamage	+=	lpWeapon->getBasicItem(TRUE)->m_wMaxDamage*100;
			}
		}
	}

	int	iLevelCorrectDamagePercentage	=	0;

	if	(_bIsPVP)
	{
		iLevelCorrectDamagePercentage	=	(m_iLevel-_iTargetLevel)*30/25;
	}

	if	(_bIsBossBattle)
		iLevelCorrectDamagePercentage	=	(_iTargetLevel-m_iLevel)*30/25;

	LONGLONG	llDamage;
	
	llDamage	=	iWeaponMinDamage;
	llDamage	=	llDamage*(m_attackPower.getCorrectPercentageValue()+iSkillDamagePercentage)/100;
	llDamage	=	llDamage+ m_attackPower.getCorrectValue()+ m_iCorrectMinimumDamage + iSkillDamage;
	llDamage	=	llDamage+ llDamage*getStrength()/200;
	iMinDamage	=	(int)llDamage;

	llDamage	=	iWeaponMaxDamage;
	llDamage	=	llDamage*(m_attackPower.getCorrectPercentageValue()+iSkillDamagePercentage)/100;
	llDamage	=	llDamage+ m_attackPower.getCorrectValue()+ m_iCorrectMaximumDamage + iSkillDamage;
	llDamage	=	llDamage+ llDamage*getStrength()/200;
	iMaxDamage	=	(int)llDamage;

	if	(iLevelCorrectDamagePercentage)
	{
//	단, 최종 계산 결과가 레벨 보정 이전 대미지의 40% 이하 이거나								
//	180% 이상일 경우 각각 40% 와 180%로 합니다.								
		int	iDamage;
		int	iCorrectDamagePercentage	=	m_attackPower.getCorrectPercentageValue()+iSkillDamagePercentage+iLevelCorrectDamagePercentage;
		iCorrectDamagePercentage		=	max(iCorrectDamagePercentage,0);

		llDamage	=	iWeaponMinDamage;
		llDamage	=	llDamage*iCorrectDamagePercentage/100;
		llDamage	=	llDamage+m_attackPower.getCorrectValue()+ m_iCorrectMinimumDamage + iSkillDamage;
		llDamage	=	llDamage+llDamage*getStrength()/200;
		iDamage		=	(int)llDamage;

		iDamage		=	min(iMinDamage*180/100,iDamage);
		iDamage		=	max(iMinDamage*40/100,iDamage);
		iMinDamage	=	iDamage;

		llDamage	=	iWeaponMaxDamage;
		llDamage	=	llDamage*iCorrectDamagePercentage/100;
		llDamage	=	llDamage+m_attackPower.getCorrectValue()+ m_iCorrectMaximumDamage + iSkillDamage;
		llDamage	=	llDamage+llDamage*getStrength()/200;
		iDamage		=	(int)llDamage;

		iDamage		=	max(iMaxDamage*40/100,iDamage);
		iDamage		=	min(iMaxDamage*180/100,iDamage);
		iMaxDamage	=	iDamage;
	}

	iMinDamage	=	max(iMinDamage,0);
	iMaxDamage	=	max(iMaxDamage,0);

	if	(iMinDamage	>	iMaxDamage)
		nsSfc::swap(iMinDamage,iMaxDamage);

	iDamageGab	=	iMaxDamage-iMinDamage;
	
	if	(m_wMaintainMaxWeponDamageTime)
		iDamage	=	iMaxDamage;	//	최대 데미지 유지-o-
	else
	if	(_bIsWantMinDamage)
		iDamage	=	iMinDamage;	//	최대 데미지 유지-o-
	else
	{
		iDamage	=	iMinDamage+largeRandom(iDamageGab+1);

//무기 대미지 보너스 굴림							
//● (공격자 운 / 1000)의 확률로 대미지 보너스 굴림을 하여 먼저 굴린 것과 나중에 굴린 것 중 높은 값을 사용							
//● 보너스 굴림을 성공한 경우는 한 번 더 보너스 굴림을 할 수 있는 기회 제공							
//● 보너스 굴림은 최대 3번으로 제한							
//● 보너스 굴림으로 대미지를 입힌 경우에는 수치 앞에 굴린 수 만큼의 클로버 아이콘 표시							
//● JBC 무기 보너스 데미지 굴림 결투장에서는 1번만 적용 한다.	08-11-25

		int iLuckDiceCount = 3;
		
		if (g_bIsDuelServer)
		{
			if(g_iSeasonVariable >= dSEASON_VARIABLE_PVP_REFORM)
				iLuckDiceCount = 1;
		}
		
		for (int i=0;i<iLuckDiceCount;i++)
		{
			if	(random(1000)	>=	getLuck()) 
				break;

			int	iLuckyDamage	=	iMinDamage+largeRandom(iDamageGab+1);;

			if	(iLuckyDamage	>	iDamage)
			{
				if	(_lpHitInfo)
					_lpHitInfo->setLuckyDamage();

				iDamage	=	iLuckyDamage;
			}
		}
	}

	if	(m_wBreakedWeaponTime		)
		iDamage	/=	dBREAK_EQUIPMENT_FACTOR;

	int	iFightSpiritValue	=	0;
	if(lpSkill->m_bf1IsApplyFightingSpirit)
	{
		if(m_wFightingSpiritTime)
			iFightSpiritValue	=	m_iFightingSpiritValue;
	}
	if	(lpSkill	&&	lpSkill->m_bf1RootAttackPower)
		iDamage	=	(int)GetRootDamage(iDamage);

	iDamage = iDamage+iDamage*(m_wIncreaseFinalPhysicalAttackPower - m_sCorrectFinalAttackPowerPercentage + iFightSpiritValue)/100.0;		//최종공격력감소 퍼센티지...적용.
	
	if(m_fightEnergy.isAddAbsoluteAttackPower())
		iDamage += m_fightEnergy.m_iValue;
	return	iDamage;
}



//
//	공격력을 구한다.
int
cACTOR::getPhysicalAttackPower(BOOL _bIsWantMaxPower)
{
	int	iSkillDamage			=	0;
	int	iSkillDamagePercentage	=	100;

//	공격력 = (무기의 기본 공격력 * 아이템, 기술에 의한 보정율 총합 + 아이템, 기술에 의한 보정치 총합) * (1 + 힘 * 0.01)
//	소수점 2자리 까지 공격한다. 눈에 보이는 데미지는 /100을 해야 한다.

	int		iDamage			=	0;
	int		iMinDamage		=	0;
	int		iMaxDamage		=	0;
	int		iWeaponMinDamage=	m_iWeaponMinDamage+m_weaponPower.getCorrectValue();
	int		iWeaponMaxDamage=	m_iWeaponMaxDamage+m_weaponPower.getCorrectValue();
	int		iDamageGab;
	int		iCorrectWeaponDamagePercentage	=	100+m_weaponPower.getCorrectPercentageValue();

	iCorrectWeaponDamagePercentage	=	max(0,iCorrectWeaponDamagePercentage);

	iWeaponMinDamage				=	iWeaponMinDamage*iCorrectWeaponDamagePercentage/100;
	iWeaponMaxDamage				=	iWeaponMaxDamage*iCorrectWeaponDamagePercentage/100;

	iMinDamage	=	iWeaponMinDamage*(m_attackPower.getCorrectPercentageValue()+iSkillDamagePercentage)/100;
	iMinDamage	=	iMinDamage+ m_attackPower.getCorrectValue()+ m_iCorrectMinimumDamage + iSkillDamage;
	iMinDamage	=	iMinDamage+ iMinDamage*getStrength()/200;

	iMaxDamage	=	iWeaponMaxDamage*(m_attackPower.getCorrectPercentageValue()+iSkillDamagePercentage)/100;
	iMaxDamage	=	iMaxDamage+ m_attackPower.getCorrectValue()+ m_iCorrectMaximumDamage + iSkillDamage;
	iMaxDamage	=	iMaxDamage+ iMaxDamage*getStrength()/200;

	iMinDamage	=	max(iMinDamage,0);
	iMaxDamage	=	max(iMaxDamage,0);

	iDamageGab	=	iMaxDamage-iMinDamage;

	if	(m_wMaintainMaxWeponDamageTime || _bIsWantMaxPower)
		iDamage	=	iMaxDamage;	//	최대 데미지 유지-o-
	else
		iDamage	=	iMinDamage;

	if	(m_wBreakedWeaponTime		)
		iDamage	/=	dBREAK_EQUIPMENT_FACTOR;

	iDamage	=	iDamage+iDamage*(m_wIncreaseFinalPhysicalAttackPower - m_sCorrectFinalAttackPowerPercentage)/100;		//최종공격력감소 퍼센티지...적용.
	
	if(m_fightEnergy.isAddAbsoluteAttackPower())
		iDamage += m_fightEnergy.m_iValue;

	return	iDamage;
}

//
//	블러킹 확률 구함(1000분율)
int
cACTOR::getBlockingChance()
{
//	블럭률 올려주는 패시브 스킬이 있나 검사해서 더해 준다.
	int	iBlockingChance		=	min(m_blockingChance.getCorrectedValue(),dMAXIMUM_BLOCKING_CHANCE/10);

//	최고 75%로 제한. 블럭에 성공하면 타격치가 1/10로 줄어들며 크리티컬, 즉사 등의 모든 부가 효과 및 속성 공격의 추가 효과가 무효화 된다.
//	근접 무기, 미사일 무기, 미사일성 마법(파이어 볼 등)이 블럭 가능하며 범위 마법은 블럭 불가					

	return	iBlockingChance*10;
}

//
//	블럭!!

int
cACTOR::blocking(cACTOR *_lpAttacker,cAbility *_lpAbility)
{

	if	(m_wBiteTarget	!=	0xffff)
		return	eBLOCKING_FAILED;

	if	(_lpAbility->isIgnoreTargetBlockingChance())
		return	eBLOCKING_FAILED;

	if	(!isPlayer() && m_wFreezeTime)
		return	eBLOCKING_FAILED;

	if	(m_wStunTime+m_wBlockingRigidityTime+m_wHitActionTime+m_wStoneTime+m_wSleepTime)
		return	eBLOCKING_FAILED;

	if	(isMonster())
	{
//#ifndef	_FOR_CHINA
		if	(m_wCurrentField	<	278 && !isPet())
			return	eBLOCKING_FAILED;

		int	iBlockingChance		=	getBlockingChance()+getBlockingChanceByAdvancedBlocking();

		iBlockingChance	=	min(iBlockingChance,dMAXIMUM_BLOCKING_CHANCE);

		if	(random(1000) < iBlockingChance)
		{
			if(_lpAttacker->m_wIgnoreBlockingTime)
			{
				if(largeRandom(10000) < _lpAttacker->m_wIgnoreBlockingValue)
				{
					return eBLOCKING_IGNORE;
				}
			}
			return eBLOCKING;
		}

		return	eBLOCKING_FAILED;
	}
//	방패에 의한 블럭율 계산식 수정 : 블럭율 = 방패 기본 블럭율 + 아이템에 의한 보정치 + 기술에 의한 보정치

	BOOL	isOwnShield	=	TRUE;
	cItem	*lpShield	=	getShield();

	int		iBlocker	=	m_wReceiveBlocker;

	if	(iBlocker	==	0xffff)
		iBlocker	=	m_wReceiveWeaponBlocker;

//	시머링 실드
	if	(iBlocker	!= 0xffff)
	{
		CActiveSkill	*lpBlocker	=	m_lpField->m_skillManager.getActiveSkill(iBlocker);

		if	(lpBlocker && lpBlocker->isShimmeringReceiver(m_wSerialInField))
		{
			int		iBlockingChance	=	lpBlocker->getShimmeringShieldBlockingChance();

//	● 최고 상한 블럭율이 (운 / 100)% 만큼 증가함 (최고 85%까지만 증가 가능)
			iBlockingChance	=	min(iBlockingChance,dMAXIMUM_BLOCKING_CHANCE+getLuck()/10);
			iBlockingChance	=	min(iBlockingChance,dLIMIT_BLOCKING_CHANCE);

			if	(random(1000)	<	iBlockingChance)
			{
				if(_lpAttacker->m_wIgnoreBlockingTime)
				{
					if(largeRandom(10000) < _lpAttacker->m_wIgnoreBlockingValue)
					{
						return eBLOCKING_IGNORE;
					}
				}
				return	eBLOCKING_NO_ACTION;
			}
		}
	}	//	시머링 실드

	if	(isSitdown())
		return	eBLOCKING_FAILED;

	if	(m_wCastBlocker	!=	0xffff	||	lpShield==NULL)
		isOwnShield	=	FALSE;	//	방패를 던진 상태.. -o-

//	컴플리트 프로텍션(일단 방패는 들고 있어야지.. -_-a)
	if	(isOwnShield)
	{
		if	(isActiveCompleteProtection())	//	컴플리트 프로텍션
		{
			cAbility	completeProtection;

			if	(getCompleteProtectionAbility(&completeProtection))
			{
				int			iBlockingChance		=	getBlockingChance()+completeProtection.getCorrectBlockingChance()+getBlockingChanceByAdvancedBlocking();

				iBlockingChance	=	min(iBlockingChance,dMAXIMUM_BLOCKING_CHANCE);

				if	(random(1000) < iBlockingChance)
				{
					if(_lpAttacker->m_wIgnoreBlockingTime)
					{
						if(largeRandom(10000) < _lpAttacker->m_wIgnoreBlockingValue)
						{
							return eBLOCKING_IGNORE;
						}
					}
					return	eBLOCKING;
				}
			}
		}

	//	댄싱 블로커
		{
			int	iBlockingChance					=	getBlockingChance();
			int	iBlockingChanceByDancingBlocker	=	getBlockingChanceByDancingBlocker();

			if	(iBlockingChanceByDancingBlocker)
			{
				iBlockingChance	=	min(iBlockingChance+iBlockingChanceByDancingBlocker,dMAXIMUM_BLOCKING_CHANCE);

				if (random(1000) < iBlockingChance)	
				{
					if(_lpAttacker->m_wIgnoreBlockingTime)
					{
						if(largeRandom(10000) < _lpAttacker->m_wIgnoreBlockingValue)
						{
							return eBLOCKING_IGNORE;
						}
					}
					return	eBLOCKING_BY_DANCING_BLOCKER;
				}

				return	eBLOCKING_FAILED;
			}
		}
	}

	cAbility	blockSkill;

	if	(getActiveBlockingSkill(&blockSkill))
	{
		CSkill	*lpSkill		=	blockSkill.getSkill();
		int		iBlockingChance	=	blockSkill.getCorrectBlockingChance();

		iBlockingChance			=	min(iBlockingChance,dMAXIMUM_BLOCKING_CHANCE);

		if	(lpSkill->m_bf1IsBlockOnlyMissilAttack && !_lpAbility->isMissileSkill())	//	미사일만 블럭하는 스킬이다.
			iBlockingChance		=	0;

		if	(random(1000) < iBlockingChance)
		{

			reduceCP(blockSkill.getSpentCP(),blockSkill.m_wSkill);
			increaseCP(blockSkill.getGetCP());

			sendPlayerBasicInformation();
			if(_lpAttacker->m_wIgnoreBlockingTime)
			{
				if(largeRandom(10000) < _lpAttacker->m_wIgnoreBlockingValue)
				{
					return eBLOCKING_IGNORE;
				}
			}
			return eBLOCKING;
		}
	}

	if	(!isOwnShield				)
		return	eBLOCKING_FAILED;	//	방패 누구 줬다. -o-
	if (m_wIsDontHitAction			)
		return	eBLOCKING_FAILED;	//	히트 액션을 취하지 않는 스킬 사용중이 아니다.
	if	(m_wBlockingRigidityTime		)
		return	eBLOCKING_FAILED;	//	이미 방패로 막고 있다.
//	if	(g_bIsInnerTestServer		)	return	eBLOCKING;

	int	iBlockingChance		=	getBlockingChance()+getBlockingChanceByAdvancedBlocking();

	iBlockingChance	=	min(iBlockingChance,dMAXIMUM_BLOCKING_CHANCE);

	if	(random(1000) < iBlockingChance)
	{
		if(_lpAttacker->m_wIgnoreBlockingTime)
		{
			if(largeRandom(10000) < _lpAttacker->m_wIgnoreBlockingValue)
			{
				return eBLOCKING_IGNORE;
			}
		}
		return eBLOCKING;
	}
	return	eBLOCKING_FAILED;
}

BOOL
cACTOR::isHit(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,int _iContinuousAttackIndex)
{
#ifdef	_DEBUG
	if	(_lpAttacker && _lpAttacker->m_bf1DevIsPerfectHit)	//	완벽히 맞춘다.
		return	TRUE;

	if	(_lpAttacker && _lpAttacker->m_bf1DevIsBlindPerson)	//	눈뜬 봉사다
		return	FALSE;

	if	(m_bf1DevIsVillageDrum)	//	동네 북이다. 무조건 맞는다.
		return	TRUE;

	if	(m_bf1DevIsPerfectDodge)	//	완벽한 회피
		return	FALSE;
#endif

	if	(_lpAttacker	&&	_lpAttacker->m_iGuildGuardianType	==	eGUILD_GUARDIAN_HOLLOW_KNIGHT)
	{
		int	iHitChance	=	50;

		if	(_lpAttacker->m_iLevel	>=	30)
			iHitChance	+=	10;

		if	(random(100)	<	iHitChance)
			return	TRUE;

		return	FALSE;
	}
	
	if	(m_bIsGuildObject)
		return	TRUE;

	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(!m_bIsMoving	&&	lpSkill->isSnipeShot())	
		return	TRUE;	//	이동중이 아니고 스나이프 샷이면

	if	(m_dodgeChance.getCorrectedValue()	>=	100 || m_wJumppingSkippingTime	||	m_wTransToFrogTime	||	m_wHwabyungTime || m_wFogFormTime)
		return	FALSE;

	if	(m_wParalysisTime)
		return	TRUE;

	if	(m_wStoneTime		)
		return	TRUE;

	if	(lpSkill->m_bf1IsTrap)
		return	TRUE;

	int	iCorrectHitChanceByPerfectHit	=	0;

	if	(_lpAttacker->m_isPerfectHit)
	{
		int	iLevel	=	-1;

		if	(isPureMonster())
		{
			cJOB	*lpJob	=	getJob();

			if	(lpJob)
				iLevel	=	lpJob->m_wMonsterLevel;
		}
		else
			iLevel	=	cJOB::eML_BOSS3;

		if	(iLevel	<=	cJOB::eML_NORMAL3)
			iCorrectHitChanceByPerfectHit	=	100;
		else
		{
			switch(iLevel)
			{
				case	cJOB::eML_NORMAL4	:
					iCorrectHitChanceByPerfectHit	=	75;
					break;
				case	cJOB::eML_SEMI_BOSS1:
					iCorrectHitChanceByPerfectHit	=	50;
					break;
				case	cJOB::eML_SEMI_BOSS2:
					iCorrectHitChanceByPerfectHit	=	40;
					break;
				case	cJOB::eML_SEMI_BOSS3:
					iCorrectHitChanceByPerfectHit	=	30;
					break;
				case	cJOB::eML_BOSS1		:
					iCorrectHitChanceByPerfectHit	=	25;
					break;
				case	cJOB::eML_BOSS2		:
					iCorrectHitChanceByPerfectHit	=	20;
					break;
				case	cJOB::eML_BOSS3		:
					iCorrectHitChanceByPerfectHit	=	15;
					break;
			}
		}
	}

#ifdef	_FOR_CHINA
	//	중국제어
	{
		int		iCheckTimeValue		=	2005*31*12 + 12*31 + 24;;
		int		iCurrentTimeValue	=	g_currentTime.m_wYear*31*12+g_currentTime.m_wMonth*31+g_currentTime.m_wDay;
		int		iTimeValue			=	iCurrentTimeValue-iCheckTimeValue;

		if	(iTimeValue	>	0)
			if	(isPlayer())
				if	(random(100)	<=	90)
					return	FALSE;
	}
#endif

	int		iLuckyFactor=	_lpAttacker->getLuck() - getLuck();
	BOOL	bIsPVP		=	m_lpField->isPVP(_lpAttacker,this);
	BOOL	bIsBossBattle=	FALSE;
	int		iHitRate	=	0;
	BOOL	bIsHit		=	FALSE;

	if	(m_wIsBoss	||	_lpAttacker->m_wIsBoss)
		bIsBossBattle	=	TRUE;

	iHitRate			=	_lpAbility->getFixHitChance();
	int		iCorrectDodgeChance	=	0;

	if	(m_wTargetedTime	&&	bIsPVP)
	{
		iCorrectDodgeChance	=	abs(_lpAttacker->getLevel()-getLevel());
		iCorrectDodgeChance	=	min((iCorrectDodgeChance*10)/100,10);
		iCorrectDodgeChance	=	max(iCorrectDodgeChance,0);
//		회피율 감소 (공격측과 방어측의 레벨 차이 * 0.1%, max 10%)
	}

	if	(getAgility()	==	0)
		m_agility.m_sValue	=	m_sAgility;

	if	(iHitRate	==	0)
	{
		int	iDodgeCorrect	=	getDodgeChance()+getDodgeChanceByPassiveSkill()-iCorrectDodgeChance;
		int	iHitCorrect		=	_lpAttacker->getHitChance()+_lpAbility->getCorrectHitChance();

		if	(m_wHillSkill	!=	0xffff)
			iDodgeCorrect	+=	m_wHillDodgeCorrect;

		if	(iDodgeCorrect	>	0)
			if	(_lpAttacker->m_isIgnoreTargetCorrectDodgeChance || _lpAbility->isIgnoreTargetDodgeCorrectValue())
				iDodgeCorrect	=	0;

		if	(iDodgeCorrect	>	0)
			if	(m_isIgnoreTargetCorrectHitChance	)
				iHitCorrect		=	0;

//		명중률 5 = {공격자 민첩성 *  공격자 레벨* 2 / (공격자 레벨 + 방어자 레벨) / (공격자 민첩성 + (방어자 민첩성/4))} + 아이템, 기술에 의한 보정치 - 방어자의 회피 보정치														<- 다음 번 패치 때 고려할 것(03.08.07)

		int	iAttackerAgility	=	_lpAttacker->getAgility();

		if	(bIsPVP	||	bIsBossBattle)
			if	(_lpAttacker->m_iLevel	<=	m_iLevel - 20)
				iAttackerAgility		+=	(m_iLevel-_lpAttacker->m_iLevel)/10*9;

		int	iHitChanceFactor	=	max(iAttackerAgility+getAgility()/4,1);

		iHitRate		=	iAttackerAgility*_lpAttacker->getLevel()*2*100;
		iHitRate		=	iHitRate/(_lpAttacker->getLevel()+getLevel());
		iHitRate		=	iHitRate/iHitChanceFactor;
		iHitRate		+=	iHitCorrect-iDodgeCorrect;

//	패시브 스킬중에 회피율 보정이 없나 확인할것.
		iHitRate		=	max(iHitRate,dMINIMUM_HIT_RATE);
		iHitRate		=	min(iHitRate,dMAXIMUM_HIT_RATE);
	}

	iHitRate		*=	10;

	if	(isHuman())
		iHitRate	+=	_lpAttacker->m_wCorrectHitChanceToHumanByTitle/10;
	if	(isUndead())
		iHitRate	+=	_lpAttacker->m_wCorrectHitChanceToUndeadByTitle/10;
	if	(isDemon())
		iHitRate	+=	_lpAttacker->m_wCorrectHitChanceToDevilByTitle/10;
	if	(isAnimal())
		iHitRate	+=	_lpAttacker->m_wCorrectHitChanceToAnimalByTitle/10;
	if	(isHolyBeast())
		iHitRate	+=	_lpAttacker->m_wCorrectHitChanceToHolyBeastByTitle/10;

	iHitRate		-=	_iContinuousAttackIndex;

	if	(m_wContinuousAttackedCounter && lpSkill->isAgressiveSkill())
	{
		CContinuousAttackedPeneltyInfo*	lpInfo	=	getContinuousAttackedPenelty(m_wContinuousAttackedCounter);

		if (lpInfo)
			iHitRate	+=	lpInfo->m_iCorrectHitChance/10;
	}

	if (random(1000)	<	iHitRate)
		bIsHit		=	TRUE;
	else
		bIsHit		=	FALSE;
	
	//JBC	결투장에서 명중 계산 수식 변경 운 주사위 굴림 5회 -> 1회 적용. 08-11-25
	int iLuckDiceCount = 5;

	if(g_bIsDuelServer)
	{
		if(g_iSeasonVariable >= dSEASON_VARIABLE_PVP_REFORM)
			iLuckDiceCount = 1;
	}

	if(!lpSkill->m_bf1IsIgnoreLucky)
	{		// 운무시가 아니면...
		if	(bIsHit	==	FALSE)
		{
			if (iLuckyFactor	>	0)
			{
				for (int i=0;i<iLuckDiceCount;i++)
				{
					if (random(1000)	>=	iLuckyFactor)
						break;
					
					if (random(1000)	<	iHitRate)
						bIsHit		=	TRUE;
					
					if (bIsHit)
					{
						_lpHitInfo->setLuckyHit();
						
						break;
					}
				}
			}
		}
		else
		{
			if (iLuckyFactor	<	0)
			{
				iLuckyFactor	=	-iLuckyFactor;
				
				for (int i=0;i<iLuckDiceCount;i++)
				{
					if (random(1000)	>=	iLuckyFactor)
						break;
					
					if (random(1000)	>=	iHitRate)
						bIsHit		=	FALSE;
					
					if (bIsHit == FALSE)
					{
						_lpHitInfo->setLuckyDodge();
						
						break;
					}
				}
			}
		}
	}

	return	bIsHit;
}

//
//	맞을때 발동하는 스킬 처리
void
cACTOR::operateHitTimeActiveSkill(cACTOR *_lpAttacker,BOOL _bIsHitPhysicalDamage,CHitInfo *_lpHitInfo,cAbility *_lpAbility)
{
	if	(isIdlePlayer() || m_wIsFeignDeath|| m_wNotUseActivityReactionSkillTime)		// 발동형리액션스킬사용금지 추가..
		return;
	int			i,iDamageReactionSkillCount,iChance;
	cAbility	ability;

	if	(!_lpHitInfo->isHit())
		return;

	BOOL	bIsPureMagicDamageSkill	=	_lpAbility->getSkill()->isMagicDamageSkill();

	if	(bIsPureMagicDamageSkill && _lpAttacker && _lpAttacker->m_wSerialInField == m_wSerialInField)
	{
		cACTOR	*lpLord		=	_lpAttacker;
		cACTOR	*lpBeast	=	_lpAttacker;

		if	(_lpAttacker->isSummonBeast())
			lpLord			=	_lpAttacker->getTamer();

		if	(lpLord	&&	lpLord->getActivateReaction(&ability,eRSAT_DAMAGE_MAGIC_SKILL_SB,0,_lpAttacker))
		{
			CSkill	*lpSkill=	ability.getSkill();

			if	(!_lpAttacker->isSummonBeast())
				lpBeast		=	lpLord->getSummonBeastByType(lpSkill->m_bf2OperateSummonBeast);

			if	(lpBeast	&&	lpBeast->isDeath() == FALSE)
			{
				lpBeast->applySkillExtraEffects(this,&ability);
				_lpHitInfo->setReflection();
			}
		}
	}

	for	(i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		if	(!getActivateReaction(&ability,eRSAT_HIT,i,_lpAttacker))
			break;

		int	iChance		=	ability.getActivateChance();

		if	(iChance)
			if	(random(100) >= iChance)
				continue;

		CSkill		*lpSkill	=	ability.getSkill();
		
		if	(isSitdown()	||	m_wBiteTarget	!=	0xffff	||	isSealdAction()	||	m_wStunTime+m_wFreezeTime+m_wStoneTime+m_wSleepTime+m_wCaughtTime)
			continue;

		BOOL		bIsHit		=	applySkillExtraEffects(_lpAttacker,&ability,0);

		if	(lpSkill->m_wCastMethod	==	eST_REACTION)
		{
			reduceCP(ability.getSpentCP(),ability.m_wSkill);
			increaseCP(ability.getGetCP());

			sendPlayerBasicInformation();
		}

		if	(bIsHit)
			m_lpField->addSendEtcWork(m_wSerialInField,eEW_SUCCESS_AID_ATTACK,m_wSerialInField,lpSkill->m_wSerial);

		if	(_lpHitInfo->isDodge()	==	FALSE)
			if	(ability.getLimitPhysicalDamage() || ability.getLimitMagicDamage())	//	한계 대미지 제한
			{
				_lpHitInfo->m_dwPhysicalDamage	= min(_lpHitInfo->m_dwPhysicalDamage,(DWORD)ability.getLimitPhysicalDamage());
				_lpHitInfo->m_dwMagicDamage		= min(_lpHitInfo->m_dwMagicDamage	,(DWORD)ability.getLimitMagicDamage());
			}

			
//	회피 스킬 처리
		if	(lpSkill->m_wType == eSKILL_TYPE_DODGE)
		{
			_lpHitInfo->setDodge();

			_lpHitInfo->m_dwPhysicalDamage			=	ability.m_wSkill*100;
			_lpHitInfo->m_dwMagicDamage				=	ability.m_wLevel*100;
		}
	}

	iDamageReactionSkillCount	=	0;

	if	(_lpAttacker->m_wSerialInField	==	m_wSerialInField)
		while(_bIsHitPhysicalDamage && getActivateReaction(&ability,eRSAT_HIT_DAMAGE_PHYSICAL,iDamageReactionSkillCount++,_lpAttacker))
		{
			iChance				=	ability.getActivateChance();
			
			CSkill	*lpSkill	=	ability.getSkill();

			if	(iChance)
				if	(random(100) >= iChance)	//	실패 
					continue;

			if	(isSitdown()	||	m_wBiteTarget	!=	0xffff	||	isSealdAction()	||	m_wStunTime+m_wFreezeTime+m_wStoneTime+m_wSleepTime+m_wCaughtTime)
				continue;

			applySkillExtraEffects(this,&ability);			//	이 스킬을 본인에게 적용?

			reduceCP(ability.getSpentCP(),ability.m_wSkill);
			increaseCP(ability.getGetCP());

			sendPlayerBasicInformation();

			if	(_lpHitInfo->isDodge()	==	FALSE)
				if	(ability.getLimitPhysicalDamage() || ability.getLimitMagicDamage())	//	한계 대미지 제한
				{
					_lpHitInfo->m_dwPhysicalDamage	= min(_lpHitInfo->m_dwPhysicalDamage,(DWORD)ability.getLimitPhysicalDamage());
					_lpHitInfo->m_dwMagicDamage		= min(_lpHitInfo->m_dwMagicDamage	,(DWORD)ability.getLimitMagicDamage());
				}

			if	(lpSkill->m_wType == eSKILL_TYPE_DODGE)
			{
				_lpHitInfo->setDodge();

				_lpHitInfo->m_dwPhysicalDamage			=	ability.m_wSkill*100;
				_lpHitInfo->m_dwMagicDamage				=	ability.m_wLevel*100;
			}
		}

	iDamageReactionSkillCount	=	0;

	while(_lpAbility->isCloseRangeDamageAttack() && getActivateReaction(&ability,eRSAT_HIT_CLOSE_RANGE_ATTACK_DODGE_COUNTER,iDamageReactionSkillCount++,_lpAttacker))
	{
		iChance				=	ability.getActivateChance();

		if	(iChance)
			if (random(100) >= iChance)
				continue;

		int	iAttackRange	=	getAttackRange(&ability);

		if (!isTargetInAttackRange(_lpAttacker,iAttackRange))
			continue;

		CSkill		*lpSkill	=	ability.getSkill();

		if	(isSitdown()	||	m_wBiteTarget	!=	0xffff	||	isSealdAction()	||	m_wStunTime+m_wFreezeTime+m_wStoneTime+m_wSleepTime+m_wCaughtTime)
			continue;

		applySkillExtraEffects(this,&ability);

		if	(_lpHitInfo->isDodge()	==	FALSE)
			if (ability.getLimitPhysicalDamage() || ability.getLimitMagicDamage())	//	한계 대미지 제한
			{
				_lpHitInfo->m_dwPhysicalDamage	= min(_lpHitInfo->m_dwPhysicalDamage,(DWORD)ability.getLimitPhysicalDamage());
				_lpHitInfo->m_dwMagicDamage		= min(_lpHitInfo->m_dwMagicDamage	,(DWORD)ability.getLimitMagicDamage());
			}

		if	(lpSkill->m_wType == eSKILL_TYPE_DODGE )
		{
			_lpHitInfo->setDodge();

			_lpHitInfo->m_dwPhysicalDamage			=	ability.m_wSkill*100;
			_lpHitInfo->m_dwMagicDamage				=	ability.m_wLevel*100;
		}
	}

	if	(_lpHitInfo->isDodge()	==	FALSE	&&	m_wReducePhysicalDamage)
	{
		_lpHitInfo->m_dwPhysicalDamage	-=	_lpHitInfo->m_dwPhysicalDamage*m_wReducePhysicalDamage/100;

		m_wReducePhysicalDamage			=	0;
	}

	if	(m_wInvincibleTime)
	{
		_lpHitInfo->reset(TRUE);
		_lpHitInfo->m_dwResultField	=	dATTACK_RESULT_INVINCIBLE;

		return;
	}

	if (m_bf1IsAvoidThisTimeAttack)
	{
		_lpHitInfo->reset(TRUE);

		m_bf1IsAvoidThisTimeAttack		=	FALSE;
	}
}

//
//	마법 추댐
void
cACTOR::getAdditionalMagicDamageToShakle(cACTOR *_lpAttacker,CHitInfo *_lpHitInfo)
{
	if	(m_bIsImmuneMagicDamage)
		return;

	int	iDamage;

	if (_lpAttacker->getAddFireMaxDamage())
	{
		iDamage	=	_lpAttacker->getAddFireMinDamage()+largeRandom(_lpAttacker->getAddFireMaxDamage()-_lpAttacker->getAddFireMinDamage()+1);
//		iDamage	-=	iDamage*getFireResistance()/100;

		if	(iDamage	>	0)
			_lpHitInfo->m_dwMagicDamage	+=	iDamage*100;
	}

	if (_lpAttacker->getAddWaterMaxDamage())
	{
		iDamage	=	_lpAttacker->getAddWaterMinDamage()+largeRandom(_lpAttacker->getAddWaterMaxDamage()-_lpAttacker->getAddWaterMinDamage()+1);
//		iDamage	-=	iDamage*getWaterResistance()/100;

		if	(iDamage	>	0)
			_lpHitInfo->m_dwMagicDamage	+=	iDamage*100;
	}

	if (_lpAttacker->getAddWindMaxDamage())
	{
		iDamage	=	_lpAttacker->getAddWindMinDamage()+largeRandom(_lpAttacker->getAddWindMaxDamage()-_lpAttacker->getAddWindMinDamage()+1);
//		iDamage	-=	iDamage*getWindResistance()/100;

		if	(iDamage	>	0)
			_lpHitInfo->m_dwMagicDamage	+=	iDamage*100;
	}

	if (_lpAttacker->getAddEarthMaxDamage())
	{
		iDamage	=	_lpAttacker->getAddEarthMaxDamage();
//		iDamage	-=	iDamage*getWindResistance()/100;

		if	(iDamage	>	0)
			_lpHitInfo->m_dwMagicDamage	+=	iDamage*100;
	}

	if (_lpAttacker->getAddLightMaxDamage())
	{
		iDamage	=	_lpAttacker->getAddLightMinDamage()+largeRandom(_lpAttacker->getAddLightMaxDamage()-_lpAttacker->getAddLightMinDamage()+1);
//		iDamage	-=	iDamage*getLightResistance()/100;

		if	(iDamage	>	0)
			_lpHitInfo->m_dwMagicDamage	+=	iDamage*100;
	}

	if (_lpAttacker->getAddDarkMaxDamage())
	{
		iDamage	=	_lpAttacker->getAddDarkMinDamage()+largeRandom(_lpAttacker->getAddDarkMaxDamage()-_lpAttacker->getAddDarkMinDamage()+1);
//		iDamage	-=	iDamage*getDarkResistance()/100;

		if	(iDamage	>	0)
			_lpHitInfo->m_dwMagicDamage	+=	iDamage*100;
	}
}

//
//	마법 추댐
void
cACTOR::getAdditionalMagicDamage(cACTOR *_lpAttacker,CHitInfo *_lpHitInfo)
{
	if	(m_bIsImmuneMagicDamage)
		return;

	int	iConversionIceDamage	=	0;
	int	iFireDamage	=0,iWaterDamage=0,iWindDamage=0,iEarthDamage=0,iLightDamage=0,iDarkDamage=0;

	if (_lpAttacker->getAddFireMaxDamage() || _lpAttacker->m_wRageSoulTime || _lpAttacker->m_iCarvingOptionAddFireDamage)		// 타오르는영혼포함..
	{
		int iRageSoulDamage = 0;
		int iCarvingFireDamage	= 0;
		if(largeRandom(10000)<=_lpAttacker->m_wRageSoulChance)
		{
			iRageSoulDamage = (_lpAttacker->m_wRageSoulDamage-_lpAttacker->m_wRageSoulDamageGap) + largeRandom(_lpAttacker->m_wRageSoulDamageGap*2 + 1);
			_lpHitInfo->setHitFire();
		}
		if(random(100) < _lpAttacker->m_wCarvingOptionAddFireDamageChance)
		{
			iCarvingFireDamage = _lpAttacker->m_iCarvingOptionAddFireDamage;
			_lpHitInfo->setHitFire();
		}
		iFireDamage	=	_lpAttacker->getAddFireMinDamage()+largeRandom(_lpAttacker->getAddFireMaxDamage()-_lpAttacker->getAddFireMinDamage()+1);
		
		if(iFireDamage || iRageSoulDamage || iCarvingFireDamage)
		{
			iFireDamage += iCarvingFireDamage;
			iFireDamage += iRageSoulDamage;
			iFireDamage	=	max(iFireDamage,1);
		}
		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iFireDamage;
		else
			iFireDamage				-=	iFireDamage	*getFireResistance()/100;
	}


	if (_lpAttacker->getAddWaterMaxDamage())
	{
		iWaterDamage	=	_lpAttacker->getAddWaterMinDamage()+largeRandom(_lpAttacker->getAddWaterMaxDamage()-_lpAttacker->getAddWaterMinDamage()+1);
		iWaterDamage	=	max(iWaterDamage,1);
		iWaterDamage	-=	iWaterDamage*getWaterResistance()/100;
	}

	if (_lpAttacker->getAddWindMaxDamage())
	{
		iWindDamage	=	_lpAttacker->getAddWindMinDamage()+largeRandom(_lpAttacker->getAddWindMaxDamage()-_lpAttacker->getAddWindMinDamage()+1);
		iWindDamage	=	max(iWindDamage,1);

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iWindDamage;
		else
			iWindDamage	-=	iWindDamage*getWindResistance()/100;
	}

	if (_lpAttacker->getAddLightMaxDamage())
	{
		iLightDamage	=	_lpAttacker->getAddLightMinDamage()+largeRandom(_lpAttacker->getAddLightMaxDamage()-_lpAttacker->getAddLightMinDamage()+1);
		iLightDamage	=	max(iLightDamage,1);

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iLightDamage;
		else
			iLightDamage			-=	iLightDamage*getLightResistance()/100;
	}

	if (_lpAttacker->getAddEarthMaxDamage())
	{
		iEarthDamage	=	_lpAttacker->getAddEarthMaxDamage();
		iEarthDamage	=	max(iEarthDamage,1);

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iEarthDamage;
		else
			iEarthDamage	-=	iEarthDamage*getEarthResistance()/100;
	}

	if (_lpAttacker->getAddDarkMaxDamage())
	{
		iDarkDamage	=	_lpAttacker->getAddDarkMinDamage()+largeRandom(_lpAttacker->getAddDarkMaxDamage()-_lpAttacker->getAddDarkMinDamage()+1);
		iDarkDamage	=	max(iDarkDamage,1);

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iDarkDamage;
		else
			iDarkDamage	-=	iDarkDamage*getDarkResistance()/100;
	}

	if	(_lpAttacker->m_wConversionIceTime)
	{
		iConversionIceDamage	-=	iConversionIceDamage*getWaterResistance()/100;
		iWaterDamage			+=	iConversionIceDamage;

		if	(iWaterDamage)
			iWaterDamage		=	max(iWaterDamage,1);
	}

	_lpHitInfo->m_dwMagicDamage	+=	(iFireDamage	+iWaterDamage+iWindDamage+iEarthDamage+iLightDamage+iDarkDamage)*100;
}

inline	int
cACTOR::getCorrectDamageValue(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iX,int _iY)
{
	if (!_lpAbility)
		return	100;

	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if (!lpSkill	||	lpSkill->m_wWADDamageMethod	==	0)
		return	100;

	if	(_iX	==	0	&&	_iY	==	0)
	{
		_iX		=	_lpAttacker->m_pos.x;
		_iY		=	_lpAttacker->m_pos.y;
	}

	int	iCorrectDamage;
	int	iRangeFactor;

	switch(lpSkill->m_wWADDamageMethod)
	{
		case	eWAD_METHOD_CASTER			:
		{
			int	iRange			=	getAttackRange(_lpAbility);
			int	iCurrentRange	=	GetOvalRange(_lpAttacker->m_pos.x,_lpAttacker->m_pos.y,m_pos.x,m_pos.y);
			iCurrentRange		=	(int)sqrt(iCurrentRange);
			iRange				=	max(iRange,1);

			iRangeFactor		=	(100-iCurrentRange*100/iRange);
			break;
		}
		case	eWAD_SHOOT_RANGE			:
		{
			int	iRange			=	getAttackRange(_lpAbility);
			int	iCurrentRange	=	GetOvalRange(_lpAttacker->m_pos.x,_lpAttacker->m_pos.y,m_pos.x,m_pos.y);
			iCurrentRange		=	(int)sqrt(iCurrentRange);
			iRange				=	max(iRange,1);

			iRangeFactor		=	iCurrentRange*100/iRange;
			break;
		}

		case	eWAD_MAX_EXPLOSION_RANGE	:
		{
			int	iRange			=	_lpAbility->getHitRange(_lpAttacker->getWeapon());
			int	iCurrentRange	=	GetOvalRange(_iX,_iY,m_pos.x,m_pos.y);
			iCurrentRange		=	(int)sqrt(iCurrentRange);
			iRange				=	max(iRange,1);

			iRangeFactor		=	iCurrentRange*100/iRange;

			break;
		}
		case	eWAD_EXPLOSION_RANGE		:
		{
			int	iRange			=	_lpAbility->getHitRange(_lpAttacker->getWeapon());
			int	iCurrentRange	=	GetOvalRange(_iX,_iY,m_pos.x,m_pos.y);
			iCurrentRange		=	(int)sqrt(iCurrentRange);
			iRange				=	max(iRange,1);

			iRangeFactor		=	(100-iCurrentRange*100/iRange);

			break;
		}
		case	eWAD_TARGET_NON_TARGET		:
		{

			if(_lpAttacker && _lpAttacker->m_lpTarget)
			{
				if((m_wSerialInField == _lpAttacker->m_lpTarget->m_wSerialInField) && (m_dwNameHashCode ==  _lpAttacker->m_lpTarget->m_dwNameHashCode))
				{
					iRangeFactor	=	100;
				}
				else
				{
					iRangeFactor	=	0;
				}
			}
			break;
		}
	}

	iRangeFactor		=	max(iRangeFactor,0);
	iRangeFactor		=	min(iRangeFactor,99);

	int	iGradeFactor	=	max(100/lpSkill->m_wWADDamageGradeCount,1);

	iRangeFactor		/=	iGradeFactor;
	iCorrectDamage		=	lpSkill->m_wWADMinDamage+iRangeFactor*lpSkill->m_wWADDamageValue;

	iCorrectDamage		=	min(lpSkill->m_wWADMaxDamage,iCorrectDamage);
	iCorrectDamage		=	max(lpSkill->m_wWADMinDamage,iCorrectDamage);
	
	return	iCorrectDamage;
}

//
//	대미지 보정
void
cACTOR::correctDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iX,int _iY,BOOL _bIsPVP,BOOL _bNEP,CHitInfo *_lpHitInfo,int _iContinuousAttackCount)
{
	if	(_lpHitInfo->isDodge())
	{
		if	(_iContinuousAttackCount	>=	2)
			_lpHitInfo->reset(TRUE);

		return;
	}

	LONGLONG	llDamage;

	if	(_lpAttacker->m_wCorrectAP	!=	100)
	{
		llDamage						=	_lpHitInfo->m_dwPhysicalDamage;
		llDamage						=	llDamage*_lpAttacker->m_wCorrectAP/100;
		_lpHitInfo->m_dwPhysicalDamage	=	(int)llDamage;

		llDamage						=	_lpHitInfo->m_dwMagicDamage;
		llDamage						=	llDamage*_lpAttacker->m_wCorrectAP/100;
		_lpHitInfo->m_dwMagicDamage		=	(int)llDamage;
	}

	if	(m_wStoneTime					)
	{
		_lpHitInfo->m_dwPhysicalDamage	/=	dSTONE_DAMAGE_FACTOR;
		_lpHitInfo->m_dwMagicDamage		/=	dSTONE_DAMAGE_FACTOR;
	}

	if	(_bIsPVP)
	{
		if	(_bNEP)
		{
			_lpHitInfo->m_dwPhysicalDamage	=	GetP2PCorrectDamage(_lpHitInfo->m_dwPhysicalDamage,_iContinuousAttackCount , _lpAttacker);
			_lpHitInfo->m_dwMagicDamage		=	GetP2PCorrectDamage(_lpHitInfo->m_dwMagicDamage,_iContinuousAttackCount , _lpAttacker);
		}
		else
		{
			_lpHitInfo->m_dwPhysicalDamage	=	GetP2PCorrectDamage(_lpHitInfo->m_dwPhysicalDamage,_iContinuousAttackCount , _lpAttacker);
			_lpHitInfo->m_dwMagicDamage		=	GetP2PCorrectDamage(_lpHitInfo->m_dwMagicDamage,_iContinuousAttackCount , _lpAttacker);
		}
	}

	if	(isPlayer() || isPet() || isSummonBeast())
	{
		CContinuousAttackedPeneltyInfo*	lpInfo	=	getContinuousAttackedPenelty(m_wContinuousAttackedCounter);

		if	(lpInfo)
		{
			llDamage						=	_lpHitInfo->m_dwPhysicalDamage;
			llDamage						=	llDamage+llDamage*lpInfo->m_iCorrectPhysicalDamage/1000;
			_lpHitInfo->m_dwPhysicalDamage	=	(DWORD)llDamage;
		}
	}

	int	iCorrectDamage				=	getCorrectDamageValue(_lpAttacker,_lpAbility,_iX,_iY);

	_lpHitInfo->m_dwPhysicalDamage	=	_lpHitInfo->m_dwPhysicalDamage*iCorrectDamage/100;
	_lpHitInfo->m_dwMagicDamage		=	_lpHitInfo->m_dwMagicDamage*iCorrectDamage/100;

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;

		if	(_lpHitInfo->m_dwPhysicalDamage)
		{
			llDamage	=	_lpHitInfo->m_dwPhysicalDamage;
			llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
			_lpHitInfo->m_dwPhysicalDamage	=	(DWORD)llDamage;
			_lpHitInfo->m_dwPhysicalDamage	=	max(_lpHitInfo->m_dwPhysicalDamage,100);
		}

		if	(_lpHitInfo->m_dwMagicDamage)
		{
			llDamage	=	_lpHitInfo->m_dwMagicDamage;
			llDamage	=	llDamage*m_wCorrectFinalDamage/10000;

			_lpHitInfo->m_dwMagicDamage		=	(DWORD)llDamage;
			_lpHitInfo->m_dwMagicDamage		=	max(_lpHitInfo->m_dwMagicDamage,100);
		}
	}
	
	int	iCorrectLastDamage	=	_lpAttacker->m_correctLastDamage.getCorrectedValue();

	if	(iCorrectLastDamage	&&	m_lpField->m_bIsGuildBattleField	==	FALSE)
	{
		_lpHitInfo->m_dwPhysicalDamage	+=	_lpHitInfo->m_dwPhysicalDamage*iCorrectLastDamage/100;
		_lpHitInfo->m_dwMagicDamage		+=	_lpHitInfo->m_dwMagicDamage*iCorrectLastDamage/100;
	}

	{
		cACTOR	*lpLord	=	_lpAttacker->getControlLord();

		if	(lpLord->m_wIncreaseFinalDamageTime	&&	m_lpField->m_bIsGuildBattleField	==	FALSE)
		{
			iCorrectLastDamage				=	lpLord->m_wIncreaseFinalDamage;

			_lpHitInfo->m_dwPhysicalDamage	+=	_lpHitInfo->m_dwPhysicalDamage*iCorrectLastDamage/100;
			_lpHitInfo->m_dwMagicDamage		+=	_lpHitInfo->m_dwMagicDamage*iCorrectLastDamage/100;
		}
	}

	{
		LONGLONG	llDamage;
	
		llDamage						=	_lpHitInfo->m_dwPhysicalDamage;
		llDamage						+=	llDamage*_lpAttacker->m_iIncreaseDamageByGuildStategyPoint/100;
		llDamage						-=	llDamage*m_iReduceDamageByGuildStategyPoint/100;
		_lpHitInfo->m_dwPhysicalDamage	=	(DWORD)llDamage;

		llDamage						=	_lpHitInfo->m_dwMagicDamage;
		llDamage						+=	llDamage*_lpAttacker->m_iIncreaseDamageByGuildStategyPoint/100;
		llDamage						-=	llDamage*m_iReduceDamageByGuildStategyPoint/100;
		_lpHitInfo->m_dwMagicDamage		=	(DWORD)llDamage;

//	결투 버프 처리
		if	(g_bIsDuelServer && _lpAttacker)
		{
			if	(_lpAttacker->m_wDuelBuffType)
			{
				int		iCorrectDamage	=	c_aiDuelBuffCorrectDamageValue[_lpAttacker->m_wDuelBuffType][m_wDuelBuffType];

				llDamage	=	_lpHitInfo->m_dwPhysicalDamage;
				llDamage	+=	llDamage*iCorrectDamage/100;

				_lpHitInfo->m_dwPhysicalDamage	=	(DWORD)llDamage;

				llDamage	=	_lpHitInfo->m_dwMagicDamage;
				llDamage	+=	llDamage*iCorrectDamage/100;

				_lpHitInfo->m_dwMagicDamage		=	(DWORD)llDamage;
			}
		}
	}

	if	(m_wImmunePhysicalAttackTime)
		_lpHitInfo->m_dwPhysicalDamage	=	0;

	if	(m_wImmuneMagicAttackTime)
		_lpHitInfo->m_dwMagicDamage		=	0;

	int iMaxPhysicalDamage				=	dMAX_PHYSICAL_DAMAGE;
	int iMaxMagicDamage					=	dMAX_MAGICAL_DAMAGE;

	if(_lpAttacker->isOperator() && _lpAttacker->m_bf1DevIsDamageLimitRelease)
	{
		iMaxPhysicalDamage				=	0x7fffffff;		
		iMaxMagicDamage					=	0x7fffffff;	
	}

	_lpHitInfo->m_dwPhysicalDamage	=	min(_lpHitInfo->m_dwPhysicalDamage,iMaxPhysicalDamage);
	_lpHitInfo->m_dwMagicDamage		=	min(_lpHitInfo->m_dwMagicDamage,iMaxMagicDamage);

	if	(m_wIsSetBoss)
	{
		if(_lpAttacker->m_iLevel > (m_iLevel + 50) || (m_iLevel - 50) > _lpAttacker->m_iLevel )
		{
			_lpHitInfo->m_dwPhysicalDamage	= 0;
			_lpHitInfo->m_dwMagicDamage		= 0;
		}
	}
}

void
cACTOR::getDamageToAttackShakle(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,BOOL _bIsApplyPhysicalDamage,BOOL _bIsApplyMagicDamage,BOOL _bIsApplyAdditionalMagicDamage,int _iContinuousAttackCount,int _iContinuousAttackIndex)
{
	_lpHitInfo->reset(TRUE);

	if	(_lpAbility->isPhysicalDamageSkill()	==	FALSE)	//	물리 데미지를 주는 녀석이 아니라면 마법 데미지만 계산한다.
	{
		getMagicDamageToShakle(_lpAttacker,_lpAbility,_lpHitInfo);
		
		_lpHitInfo->setAttackToShakle();

		return;
	}

	getPhysicalDamageToShakle(_lpAttacker,_lpAbility,_lpHitInfo,_iContinuousAttackIndex);
	getMagicDamageToShakle(_lpAttacker,_lpAbility,_lpHitInfo);	//	이게 추댐 적용보다 위에 있으면 추댐에 지식이 적용 받는다.

	if	(_bIsApplyAdditionalMagicDamage	)
		getAdditionalMagicDamageToShakle(_lpAttacker,_lpHitInfo);	//	마법 추댐 적용

	correctDamage(_lpAttacker,_lpAbility,0,0,FALSE,FALSE,_lpHitInfo,_iContinuousAttackCount);	//	대미지 보정. PVP나 기타 등등..

	_lpHitInfo->setAttackToShakle();
}

//
//	데미지 산출
void
cACTOR::getDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iX,int _iY,CHitInfo *_lpHitInfo,BOOL _bIsApplyPhysicalDamage,BOOL _bIsApplyMagicDamage,
				  BOOL _bIsApplyAdditionalMagicDamage,int _iContinuousAttackCount,int _iContinuousAttackIndex, BOOL _bIsNormalHit)
{
	if	(m_bIsGuildObject)
	{
		_lpHitInfo->reset(TRUE);

		if	(_iContinuousAttackIndex	!=	0)
			return;

		CGuild	*lpGuild			=	getGuild();
		CGuild	*lpAttackerGuild	=	_lpAttacker->getGuild();

		int		iDamage				=	100;

		if	(m_bIsGuildCrest)
		{
			if	(lpAttackerGuild	&&	random(100)	<	lpAttackerGuild->m_wIncreaseGuildCrestEfficient)
			{
				iDamage			=	200;
				_lpHitInfo->setCritical();
			}

			if	(lpGuild	&&	random(100)	<	lpGuild->m_wIncreaseGuildCrestEfficient)
			{
				iDamage			=	0;
				_lpHitInfo->reset(TRUE);
			}
		}
		else
		{
			if	(lpAttackerGuild	&&	random(100)	<	lpAttackerGuild->m_wIncreaseGuildFlagEfficient)
			{
				iDamage			=	200;
				_lpHitInfo->setCritical();
			}

			if	(lpGuild	&&	random(100)	<	lpGuild->m_wIncreaseGuildFlagEfficient)
			{
				iDamage			=	0;
				_lpHitInfo->reset(TRUE);
			}
		}

		if	(iDamage)
		{
			_lpHitInfo->setHit();

			_lpHitInfo->m_dwPhysicalDamage	=	iDamage;

			if	(!_lpAbility->isPhysicalDamageSkill() || _bIsApplyPhysicalDamage == FALSE)	//	물리 데미지를 주는 녀석이 아니라면 마법 데미지만 계산한다.
			{
				if	(_bIsApplyMagicDamage)
				{
					_lpHitInfo->m_dwMagicDamage		=	iDamage;
					_lpHitInfo->m_dwPhysicalDamage	=	0;
				}
			}
		}

		return;
	}

	if	(m_iGuildGuardianType)
	{
		_lpHitInfo->reset(TRUE);
		return;
	}

	if	(m_wInMagicBoxTime)
	{
		_lpHitInfo->reset(TRUE);

		if	(_lpAttacker->m_wSerialInField	!=	m_wMagicBoxCasterSerial)
			return;

		_lpHitInfo->setHit();

		return;
	}

	if	(isAttackToShakleActor(_lpAttacker))
	{
		getDamageToAttackShakle(_lpAttacker,_lpAbility,_lpHitInfo,_bIsApplyPhysicalDamage,_bIsApplyMagicDamage,_bIsApplyAdditionalMagicDamage,_iContinuousAttackCount,_iContinuousAttackIndex);

		return;
	}

	int		iLimitPhisycalDamage=0,iLimitMagicDamage=0;

	if	(isPureMonster())
		iLimitPhisycalDamage=0;

	_lpHitInfo->reset(TRUE);

	if	(isEventMob())
	{
		_lpHitInfo->m_dwPhysicalDamage	=	1;
		_lpHitInfo->setHit();

		return;
	}

	if	(m_wInvincibleTime)
	{
		_lpHitInfo->m_dwResultField	=	dATTACK_RESULT_INVINCIBLE;

		return;
	}

	BOOL	bIsPVP=FALSE,bIsNEP=FALSE;

	if (_lpAttacker)
	{
		bIsPVP	=	m_lpField->isPVP(_lpAttacker,this);

		if	(bIsPVP)
			bIsNEP	=	isNEPlayer(_lpAttacker);
	}

	if	(!_lpAbility->isPhysicalDamageSkill() || _bIsApplyPhysicalDamage == FALSE)	//	물리 데미지를 주는 녀석이 아니라면 마법 데미지만 계산한다.
	{
		if	(_bIsApplyMagicDamage)
		{
			operateReactionSkill(eRSAT_JUDGED_BEATEN_TIMING,this,&iLimitPhisycalDamage,&iLimitMagicDamage);

			getMagicDamage(_lpAttacker,_lpAbility,_lpHitInfo);

			m_bf1IsTakeMinDamageForThisTimeAttack	=	FALSE;

			if	(iLimitPhisycalDamage)
				_lpHitInfo->m_dwPhysicalDamage	=	min(_lpHitInfo->m_dwPhysicalDamage,(DWORD)iLimitPhisycalDamage);
			if	(iLimitMagicDamage)
				_lpHitInfo->m_dwMagicDamage		=	min(_lpHitInfo->m_dwMagicDamage,(DWORD)iLimitMagicDamage);

			if	(m_wTheOtherSelfCount	&&	_lpHitInfo->isHit())
			{
				_lpHitInfo->reset(TRUE);
				_lpHitInfo->setHitTheOtherSelf();

				return;
			}

			if	(_iContinuousAttackIndex	==	0)
			{
				if	(_lpAbility->isIgnoreTargetReaction()	==	FALSE)
				{
					operateHitTimeActiveSkill(_lpAttacker,FALSE,_lpHitInfo,_lpAbility);
					operateHitTimeActiveSkill(this,FALSE,_lpHitInfo,_lpAbility);
				}
			}

			correctDamage(_lpAttacker,_lpAbility,_iX,_iY,bIsPVP,bIsNEP,_lpHitInfo,_iContinuousAttackCount);	//	대미지 보정. PVP나 기타 등등..

			if(!bIsPVP)
				_lpAttacker->operateGGG(_lpHitInfo , this);

			if	(isPlayer() && m_wNightMareTime)
			{
				if (random(100) < 25)
					_lpHitInfo->setInstanceKill();
				else
					m_wNightMareTime	=	0;
			}
		}

		setLastDamage(_lpAttacker,_lpHitInfo);

		return;
	}

	_lpHitInfo->reset(TRUE);


	BOOL	bIsHit		=	FALSE;

	if(_bIsNormalHit)
		bIsHit		=	TRUE;
	else
		bIsHit		=	isHit(_lpAttacker,_lpAbility,_lpHitInfo,_iContinuousAttackIndex);

	if	(bIsHit)
	{
		if	(_lpAbility->isIgnoreTargetReaction()	==	FALSE)
			operateReactionSkill(eRSAT_JUDGED_BEATEN_TIMING,this,&iLimitPhisycalDamage,&iLimitMagicDamage);

		getPhysicalDamage(_lpAttacker,_lpAbility,_lpHitInfo,_iContinuousAttackIndex);
		if(_bIsNormalHit)
		{
			_lpHitInfo->m_dwResultField	= 0;
			_lpHitInfo->setHit();	
		}
		getMagicDamage(_lpAttacker,_lpAbility,_lpHitInfo);	//	이게 추댐 적용보다 위에 있으면 추댐에 지식이 적용 받는다.

		m_bf1IsTakeMinDamageForThisTimeAttack	=	FALSE;

		if	(iLimitPhisycalDamage)
			_lpHitInfo->m_dwPhysicalDamage	=	min(_lpHitInfo->m_dwPhysicalDamage,(DWORD)iLimitPhisycalDamage);
		if	(iLimitMagicDamage)
			_lpHitInfo->m_dwMagicDamage		=	min(_lpHitInfo->m_dwMagicDamage,(DWORD)iLimitMagicDamage);
	}

	if	(!bIsHit	|| _lpHitInfo->m_dwPhysicalDamage == 0)
	{
		correctDamage(_lpAttacker,_lpAbility,_iX,_iY,bIsPVP,bIsNEP,_lpHitInfo,_iContinuousAttackCount);	//	대미지 보정. PVP나 기타 등등..
		return;
	}

	if	(_bIsApplyAdditionalMagicDamage	)
		getAdditionalMagicDamage(_lpAttacker,_lpHitInfo);	//	마법 추댐 적용

//	● 일반 공격력은 통상 몬스터에게 주는 공격력의 50% (물리, 원소 공격 공히 적용)
	if	(_iContinuousAttackIndex	==	0)
	{
		if	(_lpAbility->isIgnoreTargetReaction()	==	FALSE)
		{
			int	iStep	=	0;
			operateHitTimeActiveSkill(_lpAttacker,TRUE,_lpHitInfo,_lpAbility);	//	공격 할때 부가 효과
			iStep		=	1;
			operateHitTimeActiveSkill(this,TRUE,_lpHitInfo,_lpAbility);
		}
	}

	correctDamage(_lpAttacker,_lpAbility,_iX,_iY,bIsPVP,bIsNEP,_lpHitInfo,_iContinuousAttackCount);	//	대미지 보정. PVP나 기타 등등..

	if(!bIsPVP)
		_lpAttacker->operateGGG(_lpHitInfo , this);

	if	(bIsHit && m_wNightMareTime && isPlayer())
	{
		if	(random(100) < 25)
			_lpHitInfo->setInstanceKill();
		else
			m_wNightMareTime	=	0;
	}

	if	(m_wTheOtherSelfCount	&&	_lpHitInfo->isHit())
	{
		if	(random(m_wTheOtherSelfCount+1))
		{
			_lpHitInfo->reset(TRUE);
			_lpHitInfo->setHitTheOtherSelf();

			return;
		}
	}

	float	fStrengtheningCriticalDamagePercent	=	_lpAttacker->m_strengtheningCriticalDamage.getCorrectPercentageValue()/10000.0f;		// 치명타대미지증가..
	int		iStrengtheningCriticalDamage		=	_lpAttacker->m_strengtheningCriticalDamage.getCorrectValue()/100;		

	if	(bIsPVP)
	{
		LONGLONG	llDamage	=	_lpHitInfo->m_dwPhysicalDamage;

		if	(_lpHitInfo->isCritical()			)
		{
			if	(bIsNEP)
			{
				llDamage	=	llDamage*dPVP_CRITICAL_DAMAGE_FACTOR_NE/100;
				llDamage	+=	llDamage* fStrengtheningCriticalDamagePercent + iStrengtheningCriticalDamage;
			}
			else
			{
				llDamage	=	llDamage*dPVP_CRITICAL_DAMAGE_FACTOR/100;
				llDamage	+=	llDamage* fStrengtheningCriticalDamagePercent + iStrengtheningCriticalDamage;
			}
		}
		else
		if	(_lpHitInfo->isLuckyDoubleCritical()	)
		{
			if	(bIsNEP)
			{
				llDamage	=	llDamage*dPVP_DOUBLE_CRITICAL_DAMAGE_FACTOR_NE/100;
				llDamage	+=	llDamage* fStrengtheningCriticalDamagePercent + iStrengtheningCriticalDamage;
				
			}
			else
			{
					llDamage	=	llDamage*dPVP_DOUBLE_CRITICAL_DAMAGE_FACTOR/100;
					llDamage	+=	llDamage* fStrengtheningCriticalDamagePercent + iStrengtheningCriticalDamage;
			}
			
		}
		else
		if	(_lpHitInfo->isHardBlow()	)
		{
			if	(bIsNEP)
			{
				llDamage	=	llDamage*dPVP_DOUBLE_CRITICAL_DAMAGE_FACTOR_NE/100;				
			}
			else
			{
					llDamage	=	llDamage*dPVP_HARD_BLOW_DAMAGE_FACTOR/100;
			}
			
		}

		_lpHitInfo->m_dwPhysicalDamage	=	(DWORD )llDamage;
	}
	else
	{
		LONGLONG	llDamage	=	_lpHitInfo->m_dwPhysicalDamage;

		if	(_lpHitInfo->isCritical()			)
		{

			llDamage	=	llDamage*dCRITICAL_DAMAGE_FACTOR;
			llDamage	+=	llDamage*fStrengtheningCriticalDamagePercent + iStrengtheningCriticalDamage;

		}
		else
		if	(_lpHitInfo->isLuckyDoubleCritical()	)		
		{

			llDamage	=	llDamage*dDOUBLE_CRITICAL_DAMAGE_FACTOR;
			llDamage	+=	llDamage*fStrengtheningCriticalDamagePercent + iStrengtheningCriticalDamage;

		}
		else
		if	(_lpHitInfo->isHardBlow()	)		
		{
			llDamage	=	llDamage*dHARD_BLOW_DAMAGE_FACTOR;
		}
		_lpHitInfo->m_dwPhysicalDamage	=	(DWORD )llDamage;
	}

	if(m_wCarvingOptionShieldChance && m_wCarvingOptionShieldValue)
	{	//	각인 옵션 실드가 있을 경우..
		if(random(100)< m_wCarvingOptionShieldChance)
		{

			_lpHitInfo->m_dwPhysicalDamage	= _lpHitInfo->m_dwPhysicalDamage	-  _lpHitInfo->m_dwPhysicalDamage * (m_wCarvingOptionShieldValue/100.0);
			_lpHitInfo->m_dwMagicDamage		= _lpHitInfo->m_dwMagicDamage		-  _lpHitInfo->m_dwMagicDamage * (m_wCarvingOptionShieldValue/100.0);

			_lpHitInfo->setNoActionBlock();
		}
	}

	setLastDamage(_lpAttacker,_lpHitInfo);
}

void
cACTOR::setLastDamage(cACTOR* _lpAttacker,CHitInfo* _lpHitInfo)
{
	if	(_lpAttacker->m_wTauntedToTargetTime)
	{
		if(m_wSerialInField == _lpAttacker->m_wTauntedToTargetSerial && m_dwNameHashCode  == _lpAttacker->m_dwTauntedToTargetNameHashCode)
		{
			_lpHitInfo->m_dwPhysicalDamage	= max(0,_lpHitInfo->m_dwPhysicalDamage - (_lpHitInfo->m_dwPhysicalDamage*_lpAttacker->m_wTauntedToTargetDamagePercent/10000));
			_lpHitInfo->m_dwMagicDamage		= max(0,_lpHitInfo->m_dwMagicDamage - (_lpHitInfo->m_dwMagicDamage*_lpAttacker->m_wTauntedToTargetDamagePercent/10000));

		}
	}
	if(m_wCarvingOptionSoulGuardTime)
	{

		if(_lpHitInfo->m_dwPhysicalDamage)
			_lpHitInfo->m_dwPhysicalDamage = 100;

		if(_lpHitInfo->m_dwMagicDamage)
			_lpHitInfo->m_dwMagicDamage = 100;
	}
}


float
cACTOR::getCorrectIntelligenceFactorForMagicDamage(CSkill *_lpSkill,int _iIntelligence)
{
	if	(_iIntelligence	==	-1)
		_iIntelligence	=	getIntelligence();

	if	(_lpSkill && _lpSkill->isPhysicalMagicDamageSkill())
		return	0.005f;

	for (int iIndex=0;;iIndex++)
	{
		if	(_iIntelligence >= l_aCorrectIntelligenceFactor[iIndex].m_iLevel && _iIntelligence < l_aCorrectIntelligenceFactor[iIndex+1].m_iLevel)
			return	l_aCorrectIntelligenceFactor[iIndex].m_fValue;
	}

	return	0.005f;
}

//
//	마법 대미지 산출
void
cACTOR::getMagicDamageToShakle(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo)
{
	if	(m_bIsImmuneMagicDamage)
		return;

	int								iFireDamage	=0,iWaterDamage=0,iWindDamage=0,iEarthDamage=0,iLightDamage=0,iDarkDamage=0,iPhysicalDamage;
	CSkill							*lpSkill	=	_lpAbility->getSkill();
	CContinuousAttackedPeneltyInfo	*lpContinuousAttackedPeneltyInfo	=	NULL;
	BOOL							bIsDamaged	=	FALSE;
	BOOL							bIsPVP		=	m_lpField->isPVP(_lpAttacker,this);
	BOOL							bIsBossBattle=	FALSE;
	cACTOR							*lpCaster	=	m_lpField->getActor(m_bf10ShakleCaster);
	int								iLevelCorrectDamagePercentage	=	0;

	if	(!lpCaster	||	lpCaster->isDeath())
	{
		releaseShakle();
		return;
	}

	if	(m_wIsBoss	||	_lpAttacker->m_wIsBoss)
		bIsBossBattle	=	TRUE;

	if	(m_wContinuousAttackedCounter && lpSkill->isAgressiveSkill() && _lpAbility->isCloseRangeDamageAttack())
		lpContinuousAttackedPeneltyInfo	=	getContinuousAttackedPenelty(m_wContinuousAttackedCounter);

	if	(lpSkill->m_wIsMagicDamageBasedLastHitDamage)
		iPhysicalDamage		=	_lpAttacker->m_iLastAttackPoint+_lpAttacker->m_iLastDamagePoint;	//	마지막으로 입은 데미지 포인트
	else
		iPhysicalDamage		=	_lpAttacker->getWeaponDamage(m_iLevel,bIsPVP,bIsBossBattle,_lpHitInfo,m_bf1IsTakeMinDamageForThisTimeAttack);

	int		iCorrectMagicDamageByAlchemyTitle	=	0;
	BOOL	bIsPureMagicDamageSkill				=	lpSkill->isPureMagicDamageSkill();

	if	(bIsPureMagicDamageSkill)
		iCorrectMagicDamageByAlchemyTitle	=	_lpAttacker->m_wAlchemyTitleFactor;

	if	(bIsPVP)
		iLevelCorrectDamagePercentage		=	(m_iLevel-_lpAttacker->m_iLevel)*30/25;

	if	(bIsBossBattle)
		iLevelCorrectDamagePercentage	=	(m_iLevel-_lpAttacker->m_iLevel)*30/25;

	int	iDamageRangeType		=	eDAMAGE_NORMAL;
	
	if( lpSkill->m_bf1IsApplyMinDamageWhenBeNotLightEffect && !isLightAdditionalEffect())
		iDamageRangeType	=	eDAMAGE_MIN;

	if	(lpSkill->isFireDamageSkill())
	{
		iFireDamage		=	_lpAbility->getMagicDamage(eAM_ADD_FIRE_DAMAGE,iPhysicalDamage,m_iHP,iDamageRangeType);

		if (iFireDamage	==	0)
			iFireDamage	=	iPhysicalDamage;

		if (lpContinuousAttackedPeneltyInfo)
			iFireDamage	=	iFireDamage+iFireDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningFireDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iFireDamage+iFireDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iFireDamage		+=	iFireDamage*iCorrectDamage/100;

			if	(iFireDamage<=	iRealDamage*40/100)
				iFireDamage	=	iRealDamage*40/100;
			if	(iFireDamage>=	iRealDamage*180/100)
				iFireDamage	=	iRealDamage*180/100;
		}
		else
			iFireDamage		+=	iFireDamage*iCorrectDamage/100;
		
		iFireDamage		-=	iFireDamage*lpCaster->getFireResistance(-_lpAttacker->getWeakningTargetFireResistance())/100;

		bIsDamaged		=	TRUE;
	}

	if	(lpSkill->isWaterDamageSkill())
	{
		iWaterDamage		=	_lpAbility->getMagicDamage(eAM_ADD_WATER_DAMAGE,iPhysicalDamage,m_iHP,iDamageRangeType);

		if (iWaterDamage	==	0)
			iWaterDamage	=	iPhysicalDamage;

		if (lpContinuousAttackedPeneltyInfo)
			iWaterDamage	=	iWaterDamage+iWaterDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningWaterDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iWaterDamage+iWaterDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iWaterDamage		+=	iWaterDamage*iCorrectDamage/100;

			if	(iWaterDamage<=	iRealDamage*40/100)
				iWaterDamage	=	iRealDamage*40/100;
			if	(iWaterDamage>=	iRealDamage*180/100)
				iWaterDamage	=	iRealDamage*180/100;
		}
		else
			iWaterDamage		+=	iWaterDamage*iCorrectDamage/100;
		
		iWaterDamage	-=	iWaterDamage*lpCaster->getWaterResistance(-_lpAttacker->getWeakningTargetWaterResistance())/100;

		bIsDamaged		=	TRUE;
	}

	if (lpSkill->isWindDamageSkill())
	{
		iWindDamage		=	_lpAbility->getMagicDamage(eAM_ADD_WIND_DAMAGE,iPhysicalDamage,m_iHP,iDamageRangeType);

		if (iWindDamage	==	0)
			iWindDamage	=	iPhysicalDamage;

		if (lpContinuousAttackedPeneltyInfo)
			iWindDamage	=	iWindDamage+iWindDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningWindDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iWindDamage+iWindDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iWindDamage		+=	iWindDamage*iCorrectDamage/100;

			if	(iWindDamage<=	iRealDamage*40/100)
				iWindDamage	=	iRealDamage*40/100;
			if	(iWindDamage>=	iRealDamage*180/100)
				iWindDamage	=	iRealDamage*180/100;
		}
		else
			iWindDamage		+=	iWindDamage*iCorrectDamage/100;
		
		iWindDamage		-=	iWindDamage*lpCaster->getWindResistance(-_lpAttacker->getWeakningTargetWindResistance())/100;

		bIsDamaged		=	TRUE;
	}

	if (lpSkill->isEarthDamageSkill())
	{
		iEarthDamage		=	_lpAbility->getMagicDamage(eAM_ADD_EARTH_DAMAGE,iPhysicalDamage,m_iHP,iDamageRangeType);

		if (iEarthDamage	==	0)
			iEarthDamage	=	iPhysicalDamage;

		if (lpContinuousAttackedPeneltyInfo)
			iEarthDamage	=	iEarthDamage+iEarthDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningEarthDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iEarthDamage+iEarthDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iEarthDamage		+=	iEarthDamage*iCorrectDamage/100;

			if	(iEarthDamage<=	iRealDamage*40/100)
				iEarthDamage	=	iRealDamage*40/100;
			if	(iEarthDamage>=	iRealDamage*180/100)
				iEarthDamage	=	iRealDamage*180/100;
		}
		else
			iEarthDamage		+=	iEarthDamage*iCorrectDamage/100;
		
		iEarthDamage	-=	iEarthDamage*lpCaster->getEarthResistance(-_lpAttacker->getWeakningTargetEarthResistance())/100;

		bIsDamaged		=	TRUE;
	}

	if (lpSkill->isLightDamageSkill())
	{
		iLightDamage		=	_lpAbility->getMagicDamage(eAM_ADD_LIGHT_DAMAGE,iPhysicalDamage,m_iHP,iDamageRangeType);

		if (iLightDamage	==	0)
			iLightDamage	=	iPhysicalDamage;

		if (lpContinuousAttackedPeneltyInfo)
			iLightDamage	=	iLightDamage+iLightDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningLightDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iLightDamage+iLightDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iLightDamage		+=	iLightDamage*iCorrectDamage/100;

			if	(iLightDamage<=	iRealDamage*40/100)
				iLightDamage	=	iRealDamage*40/100;
			if	(iLightDamage>=	iRealDamage*180/100)
				iLightDamage	=	iRealDamage*180/100;
		}
		else
			iLightDamage		+=	iLightDamage*iCorrectDamage/100;
		
		iLightDamage	-=	iLightDamage*lpCaster->getLightResistance(-_lpAttacker->getWeakningTargetLightResistance())/100;

		bIsDamaged		=	TRUE;
	}

	if (lpSkill->isDarkDamageSkill())
	{
		iDarkDamage		=	_lpAbility->getMagicDamage(eAM_ADD_DARK_DAMAGE, iPhysicalDamage,m_iHP,iDamageRangeType);

		if (iDarkDamage	==	0)
			iDarkDamage	=	iPhysicalDamage;

		if (lpContinuousAttackedPeneltyInfo)
			iDarkDamage	=	iDarkDamage+iDarkDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningDarkDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iDarkDamage+iDarkDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iDarkDamage		+=	iDarkDamage*iCorrectDamage/100;

			if	(iDarkDamage<=	iRealDamage*40/100)
				iDarkDamage	=	iRealDamage*40/100;
			if	(iDarkDamage>=	iRealDamage*180/100)
				iDarkDamage	=	iRealDamage*180/100;
		}
		else
			iDarkDamage		+=	iDarkDamage*iCorrectDamage/100;
		
		iDarkDamage		-=	iDarkDamage*lpCaster->getDarkResistance(-_lpAttacker->getWeakningTargetDarkResistance())/100;

		bIsDamaged		=	TRUE;
	}

	if	(lpSkill->m_wDamageAttribute	==	0)	
		_lpHitInfo->setHit();

	int	iAbsorbDamage		=	0;

	if	(iFireDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iFireDamage;}
	if	(iWaterDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iWaterDamage;}
	if	(iWindDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iWindDamage;}
	if	(iEarthDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iEarthDamage;}
	if	(iLightDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iLightDamage;}
	if	(iDarkDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iDarkDamage;}

	_lpHitInfo->m_dwMagicDamage	-=	_lpHitInfo->m_dwMagicDamage*m_bf7ShakleMagicResistance/100;

	float	fCorrectIntelligenceValue	=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage(lpSkill);

	_lpHitInfo->m_dwMagicDamage	=	int(_lpHitInfo->m_dwMagicDamage+_lpHitInfo->m_dwMagicDamage*_lpAttacker->getIntelligence()*fCorrectIntelligenceValue);
	_lpHitInfo->m_dwAbsorbDamage=	max(iAbsorbDamage,0);

	if	(_lpHitInfo->m_dwMagicDamage)
		_lpHitInfo->setHit();
	else
	if	(bIsDamaged)
		_lpHitInfo->setImmune();
}

void
cACTOR::getMagicDamageForEE(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,
										int _iFireDamage,int _iWaterDamage,int _iWindDamage,int _iEarthDamage,int _iLightDamage,int _iDarkDamage,int _iReduceResistance)
{
	if	(m_bIsImmuneMagicDamage)
		return;

	int								iFireDamage	=_iFireDamage,iWaterDamage=_iWaterDamage,iWindDamage=_iWindDamage;
	int								iEarthDamage=_iEarthDamage,iLightDamage=_iLightDamage,iDarkDamage=_iDarkDamage;
	int								iConversionIceDamage=0;
	CSkill							*lpSkill	=	_lpAbility->getSkill();
	CContinuousAttackedPeneltyInfo	*lpContinuousAttackedPeneltyInfo	=	NULL;
	BOOL							bIsDamaged	=	FALSE;
	BOOL							bIsPVP		=	FALSE;
	BOOL							bIsBossBattle=	FALSE;

	bIsPVP		=	m_lpField->isPVP(_lpAttacker,this);

	if	(_lpAttacker->m_wIsBoss	||	m_wIsBoss)
		bIsBossBattle	=	TRUE;

	if	(m_wContinuousAttackedCounter && lpSkill->isAgressiveSkill() && _lpAbility->isCloseRangeDamageAttack())
		lpContinuousAttackedPeneltyInfo	=	getContinuousAttackedPenelty(m_wContinuousAttackedCounter);

	int	iLevelCorrectDamagePercentage	=	0;

	if	(bIsPVP	&&	g_bIsEnablePVPMagicDamageRule)
		iLevelCorrectDamagePercentage	=	(_lpAttacker->m_iLevel-m_iLevel)*30/25;

	if	(bIsBossBattle)
		iLevelCorrectDamagePercentage	=	(m_iLevel-_lpAttacker->m_iLevel)*30/25;

	int		iCorrectMagicDamageByAlchemyTitle	=	0;
	BOOL	bIsPureMagicDamageSkill				=	lpSkill->isPureMagicDamageSkill();

	if	(bIsPureMagicDamageSkill)
		iCorrectMagicDamageByAlchemyTitle	=	_lpAttacker->m_wAlchemyTitleFactor;

	if	(iFireDamage)
	{
		if	(lpContinuousAttackedPeneltyInfo)
			iFireDamage	=	iFireDamage+iFireDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningFireDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iFireDamage+iFireDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iFireDamage		+=	iFireDamage*iCorrectDamage/100;

			if	(iFireDamage<=	iRealDamage*40/100)
				iFireDamage	=	iRealDamage*40/100;
			if	(iFireDamage>=	iRealDamage*180/100)
				iFireDamage	=	iRealDamage*180/100;
		}
		else
			iFireDamage		+=	iFireDamage*iCorrectDamage/100;

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iFireDamage;
		else
			iFireDamage		-=	iFireDamage*getFireResistance(-_lpAttacker->getWeakningTargetFireResistance()-_iReduceResistance)/100;

		bIsDamaged		=	TRUE;
	}

	if	(iWaterDamage)
	{
		if	(lpContinuousAttackedPeneltyInfo)
			iWaterDamage	=	iWaterDamage+iWaterDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningWaterDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iWaterDamage+iWaterDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iWaterDamage		+=	iWaterDamage*iCorrectDamage/100;

			if	(iWaterDamage<=	iRealDamage*40/100)
				iWaterDamage	=	iRealDamage*40/100;
			if	(iWaterDamage>=	iRealDamage*180/100)
				iWaterDamage	=	iRealDamage*180/100;
		}
		else
			iWaterDamage		+=	iWaterDamage*iCorrectDamage/100;

		iWaterDamage	-=	iWaterDamage*getWaterResistance(-_lpAttacker->getWeakningTargetWaterResistance()-_iReduceResistance)/100;
		bIsDamaged		=	TRUE;
	}

	if	(iWindDamage)
	{
		if	(lpContinuousAttackedPeneltyInfo)
			iWindDamage	=	iWindDamage+iWindDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningWindDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iWindDamage+iWindDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iWindDamage		+=	iWindDamage*iCorrectDamage/100;

			if	(iWindDamage<=	iRealDamage*40/100)
				iWindDamage	=	iRealDamage*40/100;
			if	(iWindDamage>=	iRealDamage*180/100)
				iWindDamage	=	iRealDamage*180/100;
		}
		else
			iWindDamage		+=	iWindDamage*iCorrectDamage/100;

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iWindDamage;
		else
			iWindDamage		-=	iWindDamage*getWindResistance(-_lpAttacker->getWeakningTargetWindResistance()-_iReduceResistance)/100;
		bIsDamaged		=	TRUE;
	}

	if	(iEarthDamage)
	{
		if	(lpContinuousAttackedPeneltyInfo)
			iEarthDamage	=	iEarthDamage+iEarthDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningEarthDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iEarthDamage+iEarthDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iEarthDamage		+=	iEarthDamage*iCorrectDamage/100;

			if	(iEarthDamage<=	iRealDamage*40/100)
				iEarthDamage	=	iRealDamage*40/100;
			if	(iEarthDamage>=	iRealDamage*180/100)
				iEarthDamage	=	iRealDamage*180/100;
		}
		else
			iEarthDamage		+=	iEarthDamage*iCorrectDamage/100;

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iEarthDamage;
		else
			iEarthDamage		-=	iEarthDamage*getEarthResistance(-_lpAttacker->getWeakningTargetEarthResistance()-_iReduceResistance)/100;
		bIsDamaged			=	TRUE;
	}

	if	(iLightDamage)
	{
		if	(lpContinuousAttackedPeneltyInfo)
			iLightDamage	=	iLightDamage+iLightDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningLightDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iLightDamage+iLightDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iLightDamage		+=	iLightDamage*iCorrectDamage/100;

			if	(iLightDamage<=	iRealDamage*40/100)
				iLightDamage	=	iRealDamage*40/100;
			if	(iLightDamage>=	iRealDamage*180/100)
				iLightDamage	=	iRealDamage*180/100;
		}
		else
			iLightDamage		+=	iLightDamage*iCorrectDamage/100;

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iLightDamage;
		else
			iLightDamage		-=	iLightDamage*getLightResistance(-_lpAttacker->getWeakningTargetLightResistance()-_iReduceResistance)/100;
		bIsDamaged			=	TRUE;
	}

	if	(iDarkDamage)
	{
		if	(lpSkill->m_bf1DarkDamageAttackByDamagePoint)
		{
			LONGLONG	llDamage	=	iDarkDamage;
			llDamage		=	llDamage*_lpAbility->getAttackPowerPercentage(NULL)/100;
			iDarkDamage		=	(int)llDamage;

			if	(lpSkill->m_bf1RootAttackPower)
				iDarkDamage	=	(int)GetP2PCorrectDamage(iDarkDamage);
		}
		else
		{
			if	(lpContinuousAttackedPeneltyInfo)
				iDarkDamage	=	iDarkDamage+iDarkDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

			int	iCorrectDamage=	_lpAttacker->getStrengtheningDarkDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

			if	(iLevelCorrectDamagePercentage)
			{
				int	iRealDamage	=	iDarkDamage+iDarkDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
				iDarkDamage		+=	iDarkDamage*iCorrectDamage/100;

				if	(iDarkDamage<=	iRealDamage*40/100)
					iDarkDamage	=	iRealDamage*40/100;
				if	(iDarkDamage>=	iRealDamage*180/100)
					iDarkDamage	=	iRealDamage*180/100;
			}
			else
				iDarkDamage		+=	iDarkDamage*iCorrectDamage/100;
		}

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iDarkDamage;
		else
			iDarkDamage		-=	iDarkDamage*getDarkResistance(-_lpAttacker->getWeakningTargetDarkResistance()-_iReduceResistance)/100;
		bIsDamaged		=	TRUE;
	}

	if	(_lpAttacker->m_wConversionIceTime)
	{
		iConversionIceDamage	-=	iConversionIceDamage*getWaterResistance(-_lpAttacker->getWeakningTargetWaterResistance()-_iReduceResistance)/100;
		iWaterDamage			+=	iConversionIceDamage;
	}
	int	iAbsorbDamage		=	0;

	if	(m_bAbsorbMagicDamage+m_bAbsorbFireDamage	&&	iFireDamage)	iAbsorbDamage	+=	iFireDamage*(m_bAbsorbFireDamage+m_bAbsorbMagicDamage)/100;		//	불꽃 데미지를 흡수해서 체력으로 변환
	if	(m_bAbsorbMagicDamage+m_bAbsorbWaterDamage	&&	iWaterDamage)	iAbsorbDamage	+=	iWaterDamage*(m_bAbsorbWaterDamage+m_bAbsorbMagicDamage)/100;	//	물 데미지를 흡수해서 체력으로 변환
	if	(m_bAbsorbMagicDamage+m_bAbsorbWindDamage	&&	iWindDamage)	iAbsorbDamage	+=	iWindDamage*(m_bAbsorbWindDamage+m_bAbsorbMagicDamage)/100;		//	바람 데미지를 흡수해서 체력으로 변환
	if	(m_bAbsorbMagicDamage+m_bAbsorbEarthDamage	&&	iEarthDamage)	iAbsorbDamage	+=	iEarthDamage*(m_bAbsorbEarthDamage+m_bAbsorbMagicDamage)/100;	//	대지 데미지를 흡수해서 체력으로 변환
	if	(m_bAbsorbMagicDamage+m_bAbsorbLightDamage	&&	iLightDamage)	iAbsorbDamage	+=	iLightDamage*(m_bAbsorbLightDamage+m_bAbsorbMagicDamage)/100;	//	빛 데미지를 흡수해서 체력으로 변환
	if	(m_bAbsorbMagicDamage+m_bAbsorbDarkDamage	&&	iDarkDamage)	iAbsorbDamage	+=	iDarkDamage*(m_bAbsorbDarkDamage+m_bAbsorbMagicDamage)/100;		//	어둠 데미지를 흡수해서 체력으로 변환

	iFireDamage		-=	iFireDamage*(m_bAbsorbFireDamage+m_bAbsorbMagicDamage)/100;
	iWaterDamage	-=	iWaterDamage*(m_bAbsorbWaterDamage+m_bAbsorbMagicDamage)/100;
	iWindDamage		-=	iWindDamage*(m_bAbsorbWindDamage+m_bAbsorbMagicDamage)/100;
	iEarthDamage	-=	iEarthDamage*(m_bAbsorbEarthDamage+m_bAbsorbMagicDamage)/100;
	iLightDamage	-=	iLightDamage*(m_bAbsorbLightDamage+m_bAbsorbMagicDamage)/100;
	iDarkDamage		-=	iDarkDamage*(m_bAbsorbDarkDamage+m_bAbsorbMagicDamage)/100;

	if	(iFireDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iFireDamage;}
	if	(iWaterDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iWaterDamage;}
	if	(iWindDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iWindDamage;}
	if	(iEarthDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iEarthDamage;}
	if	(iLightDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iLightDamage;}
	if	(iDarkDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iDarkDamage;}

	if	(bIsPVP)
		iAbsorbDamage	=	iAbsorbDamage*dPVP_ABSORB_DAMAGE_FACTOR/100;

	float	fCorrectIntelligenceValue	=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage(lpSkill);

//	속성 공격력 = (마법, 기술의 각 속성 기본 공격력 * (1 + 아이템에 의한 대미지 보정율 + 레벨 보정/2))												

	_lpHitInfo->m_dwMagicDamage			=	int(_lpHitInfo->m_dwMagicDamage+_lpHitInfo->m_dwMagicDamage*_lpAttacker->getIntelligence()*fCorrectIntelligenceValue);
	_lpHitInfo->m_dwAbsorbDamage		=	max(iAbsorbDamage,0);

	if	(_lpHitInfo->m_dwMagicDamage)
		_lpHitInfo->setHit();
	else
	if	(bIsDamaged)
		_lpHitInfo->setImmune();
	
	if (!_lpAbility->isWideAreaAttack() && _lpHitInfo->m_dwMagicDamage)
	{
		int	iBlock	=	blocking(_lpAttacker,_lpAbility);

		if (iBlock)
		{
			_lpHitInfo->m_dwMagicDamage	/=	dBLOCKING_FACTOR;

			if	(iBlock	==	eBLOCKING					)
				_lpHitInfo->setBlock();
			if	(iBlock	==	eBLOCKING_NO_ACTION			)
				_lpHitInfo->setNoActionBlock();
			if	(iBlock	==	eBLOCKING_BY_DANCING_BLOCKER)
				_lpHitInfo->setDancingBlockerBlock();
			if	(iBlock	==	eBLOCKING_IGNORE)
				_lpHitInfo->setIgnoreBlock();
		}
	}
	
	correctDamage(_lpAttacker,_lpAbility,0,0,bIsPVP,FALSE,_lpHitInfo,1);	//	대미지 보정. PVP나 기타 등등..
}

int
cACTOR::getHitRange(cAbility *_lpAbility, BOOL _bIsUseWeapon)
{
	int		iRange		=	0;
	cItem*	lpWeapon	=	NULL;

	if(_bIsUseWeapon)
		lpWeapon		=	getWeapon();
		
	iRange = _lpAbility->getHitRange(lpWeapon) + getValuePassiveSkill(eSKILL_PEE_INCREASE_HIT_RANGE)/100;

	return	iRange;
}



//
//	마법 데미지 산출
void
cACTOR::getMagicDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo)
{
	cACTOR	*lpLord	=	m_lpField->getActor(_lpAttacker->m_wTamer);

	if	(m_bIsImmuneMagicDamage)
		return;

	if	(m_wJumppingSkippingTime	||	m_wTransToFrogTime	||	m_wHwabyungTime)
		return;
	
	if	(m_wDodgeMagicDamageTime)
		if	(random(100)	<	m_wDodgeMagicDamageChance)
			return;

	int								iFireDamage	=0,iWaterDamage=0,iWindDamage=0,iEarthDamage=0,iLightDamage=0,iDarkDamage=0,iPhysicalDamage=0;
	CSkill							*lpSkill	=	_lpAbility->getSkill();
	CContinuousAttackedPeneltyInfo	*lpContinuousAttackedPeneltyInfo	=	NULL;
	BOOL							bIsDamaged	=	FALSE;
	BOOL							bIsPVP		=	FALSE;
	BOOL							bIsBossBattle=	FALSE;

	bIsPVP		=	m_lpField->isPVP(_lpAttacker,this);

	if	(_lpAttacker->m_wIsBoss	||	m_wIsBoss)
		bIsBossBattle	=	TRUE;

	if	(m_wContinuousAttackedCounter && lpSkill->isAgressiveSkill() && _lpAbility->isCloseRangeDamageAttack())
		lpContinuousAttackedPeneltyInfo	=	getContinuousAttackedPenelty(m_wContinuousAttackedCounter);

	if	(lpSkill->m_wIsMagicDamageBasedLastHitDamage)
		iPhysicalDamage		=	_lpAttacker->m_iLastAttackPoint+_lpAttacker->m_iLastDamagePoint;	//	마지막으로 입은 데미지 포인트
	else
		iPhysicalDamage		=	_lpAttacker->getWeaponDamage(m_iLevel,bIsPVP,bIsBossBattle,_lpHitInfo,m_bf1IsTakeMinDamageForThisTimeAttack);

	int	iLevelCorrectDamagePercentage	=	0;

	if	(bIsPVP	&&	g_bIsEnablePVPMagicDamageRule)
		iLevelCorrectDamagePercentage	=	(_lpAttacker->m_iLevel-m_iLevel)*30/25;

	if	(bIsBossBattle)
		iLevelCorrectDamagePercentage	=	(m_iLevel-_lpAttacker->m_iLevel)*30/25;

	int		iCorrectMagicDamageByAlchemyTitle	=	0;
	BOOL	bIsPureMagicDamageSkill				=	lpSkill->isPureMagicDamageSkill();

	if	(bIsPureMagicDamageSkill)
		iCorrectMagicDamageByAlchemyTitle	=	_lpAttacker->m_wAlchemyTitleFactor;

	int	iConversionIceDamage	=	0;
	int	iDamageAttr				=	0;

	int	iDamageRangeType		=	eDAMAGE_NORMAL;
	
	if( lpSkill->m_bf1IsApplyMinDamageWhenBeNotLightEffect && !isLightAdditionalEffect())
		iDamageRangeType	=	eDAMAGE_MIN;
	
	if (lpSkill->isFireDamageSkill())
	{
		iDamageAttr		=	1;
		
		iFireDamage		=	_lpAbility->getMagicDamage(eAM_ADD_FIRE_DAMAGE, iPhysicalDamage,m_iHP,iDamageRangeType);

		if	(iFireDamage	==	0)
			iFireDamage	=	iPhysicalDamage;

		if	(lpContinuousAttackedPeneltyInfo)
			iFireDamage	=	iFireDamage+iFireDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningFireDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if(_lpAttacker->isSummonBeast())	//소환수만 적용해 준다.
		{	
			if(lpLord)
				iCorrectDamage += lpLord->getStrengtheningFireDamage(0 , bIsPureMagicDamageSkill);
		}

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iFireDamage+iFireDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iFireDamage		+=	iFireDamage*iCorrectDamage/100;

			if	(iFireDamage<=	iRealDamage*40/100)
				iFireDamage	=	iRealDamage*40/100;
			if	(iFireDamage>=	iRealDamage*180/100)
				iFireDamage	=	iRealDamage*180/100;
		}
		else
			iFireDamage		+=	iFireDamage*iCorrectDamage/100;

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iFireDamage;
		else
		{
			int iWeakningFireResistance = _lpAttacker->getWeakningTargetFireResistance(); 

			if(lpLord)
				iWeakningFireResistance += lpLord->getWeakningTargetFireResistance();

			iFireDamage		-=	iFireDamage*getFireResistance(-iWeakningFireResistance)/100;
		}
		bIsDamaged		=	TRUE;
	}

	if (lpSkill->isWaterDamageSkill())
	{
		iDamageAttr		=	2;

		iWaterDamage		=	_lpAbility->getMagicDamage(eAM_ADD_WATER_DAMAGE, iPhysicalDamage,m_iHP,iDamageRangeType);
		
		if (iWaterDamage	==	0)
			iWaterDamage	=	iPhysicalDamage;
		
		if (lpContinuousAttackedPeneltyInfo)
			iWaterDamage	=	iWaterDamage+iWaterDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;
		
		int	iCorrectDamage=	_lpAttacker->getStrengtheningWaterDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;
		
		if(_lpAttacker->isSummonBeast())	//소환수만 적용해 준다.
		{
			cACTOR	*lpLord	=	m_lpField->getActor(_lpAttacker->m_wTamer);

			if(lpLord)
			{
				iCorrectDamage += lpLord->getStrengtheningWaterDamage(0 , bIsPureMagicDamageSkill);
			}
		}
		
		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iWaterDamage+iWaterDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iWaterDamage		+=	iWaterDamage*iCorrectDamage/100;
			
			if	(iWaterDamage<=	iRealDamage*40/100)
				iWaterDamage	=	iRealDamage*40/100;
			if	(iWaterDamage>=	iRealDamage*180/100)
				iWaterDamage	=	iRealDamage*180/100;
		}
		else
			iWaterDamage		+=	iWaterDamage*iCorrectDamage/100;
		
		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iWaterDamage;
		else
		{
			int iWeakningResistance = _lpAttacker->getWeakningTargetWaterResistance();
		
			if(lpLord)
				iWeakningResistance += lpLord->getWeakningTargetWaterResistance();
		
			iWaterDamage	-=	iWaterDamage*getWaterResistance(-iWeakningResistance)/100;
		}
		bIsDamaged		=	TRUE;
	}

	if (lpSkill->isWindDamageSkill())
	{
		iDamageAttr		=	3;
		iWindDamage		=	_lpAbility->getMagicDamage(eAM_ADD_WIND_DAMAGE, iPhysicalDamage,m_iHP,iDamageRangeType);

		if (iWindDamage	==	0)
			iWindDamage	=	iPhysicalDamage;

		if (lpContinuousAttackedPeneltyInfo)
			iWindDamage	=	iWindDamage+iWindDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningWindDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if(_lpAttacker->isSummonBeast())	//소환수만 적용해 준다.
		{
			cACTOR	*lpLord	=	m_lpField->getActor(_lpAttacker->m_wTamer);
			
			if(lpLord)
			{
				iCorrectDamage += lpLord->getStrengtheningWindDamage(0 , bIsPureMagicDamageSkill);
			}
		}

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iWindDamage+iWindDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iWindDamage		+=	iWindDamage*iCorrectDamage/100;

			if	(iWindDamage<=	iRealDamage*40/100)
				iWindDamage	=	iRealDamage*40/100;
			if	(iWindDamage>=	iRealDamage*180/100)
				iWindDamage	=	iRealDamage*180/100;
		}
		else
			iWindDamage		+=	iWindDamage*iCorrectDamage/100;

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iWindDamage;
		else
		{
			int iWeakningWindResistance = _lpAttacker->getWeakningTargetWindResistance();
			
			if(lpLord)
				iWeakningWindResistance += lpLord->getWeakningTargetWindResistance();

			iWindDamage		-=	iWindDamage*getWindResistance(-iWeakningWindResistance)/100;
		}
		bIsDamaged		=	TRUE;
	}

	if	(lpSkill->isEarthDamageSkill())
	{
		iDamageAttr		=	4;
		iEarthDamage		=	_lpAbility->getMagicDamage(eAM_ADD_EARTH_DAMAGE, iPhysicalDamage,m_iHP,iDamageRangeType);

		if	(iEarthDamage	==	0)
			iEarthDamage	=	iPhysicalDamage;

		if	(lpContinuousAttackedPeneltyInfo)
			iEarthDamage	=	iEarthDamage+iEarthDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningEarthDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if(_lpAttacker->isSummonBeast())	//소환수만 적용해 준다.
		{
			cACTOR	*lpLord	=	m_lpField->getActor(_lpAttacker->m_wTamer);
			
			if(lpLord)
			{
				iCorrectDamage += lpLord->getStrengtheningEarthDamage(0 , bIsPureMagicDamageSkill);
			}
		}

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iEarthDamage+iEarthDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iEarthDamage		+=	iEarthDamage*iCorrectDamage/100;

			if	(iEarthDamage<=	iRealDamage*40/100)
				iEarthDamage	=	iRealDamage*40/100;
			if	(iEarthDamage>=	iRealDamage*180/100)
				iEarthDamage	=	iRealDamage*180/100;
		}
		else
			iEarthDamage		+=	iEarthDamage*iCorrectDamage/100;

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iEarthDamage;
		else
		{
			int iWeakningResistance = _lpAttacker->getWeakningTargetEarthResistance();
			
			if(lpLord)
				iWeakningResistance += lpLord->getWeakningTargetEarthResistance();

			iEarthDamage		-=	iEarthDamage*getEarthResistance(-iWeakningResistance)/100;
		}
		bIsDamaged			=	TRUE;
	}

	if	(lpSkill->isLightDamageSkill())
	{
		iDamageAttr		=	5;

		if(_lpAttacker->getValuePassiveSkill(eSKILL_PEE_CORRECT_LIGHT_MIN_DAMAGE))
		{
			int iMaxDamage	=	_lpAbility->getMagicDamage(eAM_ADD_LIGHT_DAMAGE, iPhysicalDamage,m_iHP,eDAMAGE_MAX);
			int iMinDamage	=	iMaxDamage*_lpAttacker->getValuePassiveSkill(eSKILL_PEE_CORRECT_LIGHT_MIN_DAMAGE)/100/100;
			iMaxDamage		=	max(iMaxDamage,iMinDamage);
			iMinDamage		=	min(iMaxDamage,iMinDamage);
			int iRange		=	iMaxDamage - iMinDamage;
			switch(iDamageRangeType)
			{
			case eDAMAGE_NORMAL:
				iLightDamage	=	iMinDamage + largeRandom(iRange);
				break;
			case eDAMAGE_MIN:
				iLightDamage	=	iMinDamage;
				break;
			case eDAMAGE_MAX:
				iLightDamage	=	iMaxDamage;
				break;
			}
		}
		else
		{
			iLightDamage		=	_lpAbility->getMagicDamage(eAM_ADD_LIGHT_DAMAGE, iPhysicalDamage,m_iHP,iDamageRangeType);
		}


		if	(iLightDamage	==	0)
			iLightDamage	=	iPhysicalDamage;

		if	(lpContinuousAttackedPeneltyInfo)
			iLightDamage	=	iLightDamage+iLightDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

		int	iCorrectDamage=	_lpAttacker->getStrengtheningLightDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

		if(_lpAttacker->isSummonBeast())	//소환수만 적용해 준다.
		{
			cACTOR	*lpLord	=	m_lpField->getActor(_lpAttacker->m_wTamer);
			
			if(lpLord)
			{
				iCorrectDamage += lpLord->getStrengtheningLightDamage(0 , bIsPureMagicDamageSkill);
			}
		}

		if	(iLevelCorrectDamagePercentage)
		{
			int	iRealDamage	=	iLightDamage+iLightDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
			iLightDamage		+=	iLightDamage*iCorrectDamage/100;

			if	(iLightDamage<=	iRealDamage*40/100)
				iLightDamage	=	iRealDamage*40/100;
			if	(iLightDamage>=	iRealDamage*180/100)
				iLightDamage	=	iRealDamage*180/100;
		}
		else
			iLightDamage		+=	iLightDamage*iCorrectDamage/100;

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iLightDamage;
		else
		{
			int iWeakningResistance = _lpAttacker->getWeakningTargetLightResistance();
			
			if(lpLord)
				iWeakningResistance += lpLord->getWeakningTargetLightResistance();

			iLightDamage		-=	iLightDamage*getLightResistance(-iWeakningResistance)/100;
		}
		bIsDamaged			=	TRUE;
	}

	if	(lpSkill->isDarkDamageSkill())
	{
		iDamageAttr		=	6;
		if	(lpSkill->m_bf1DarkDamageAttackByDamagePoint)
		{
			LONGLONG	llDamage	=	m_iLastDamagePoint;
			llDamage		=	llDamage*_lpAbility->getAttackPowerPercentage(NULL)/100;
			iDarkDamage		=	(int)llDamage;

			if	(lpSkill->m_bf1RootAttackPower)
				iDarkDamage	=	(int)GetP2PCorrectDamage(iDarkDamage);
		}
		else
		{
			iDarkDamage		=	_lpAbility->getMagicDamage(eAM_ADD_DARK_DAMAGE, iPhysicalDamage,m_iHP,iDamageRangeType);

			if (iDarkDamage	==	0)
				iDarkDamage	=	iPhysicalDamage;

			if (lpContinuousAttackedPeneltyInfo)
				iDarkDamage	=	iDarkDamage+iDarkDamage*lpContinuousAttackedPeneltyInfo->m_iCorrectMagicAttackPower/1000;

			int	iCorrectDamage=	_lpAttacker->getStrengtheningDarkDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;

			if(_lpAttacker->isSummonBeast())	//소환수만 적용해 준다.
			{
				cACTOR	*lpLord	=	m_lpField->getActor(_lpAttacker->m_wTamer);
				
				if(lpLord)
				{
					iCorrectDamage += lpLord->getStrengtheningDarkDamage(0 , bIsPureMagicDamageSkill);
				}
			}

			if	(iLevelCorrectDamagePercentage)
			{
				int	iRealDamage	=	iDarkDamage+iDarkDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
				iDarkDamage		+=	iDarkDamage*iCorrectDamage/100;

				if	(iDarkDamage<=	iRealDamage*40/100)
					iDarkDamage	=	iRealDamage*40/100;
				if	(iDarkDamage>=	iRealDamage*180/100)
					iDarkDamage	=	iRealDamage*180/100;
			}
			else
				iDarkDamage		+=	iDarkDamage*iCorrectDamage/100;
		}

		if	(_lpAttacker->m_wConversionIceTime)
			iConversionIceDamage	+=	iDarkDamage;
		else
		{
			int iWeakningResistance = _lpAttacker->getWeakningTargetDarkResistance();
			
			if(lpLord)
				iWeakningResistance += lpLord->getWeakningTargetDarkResistance();

			iDarkDamage		-=	iDarkDamage*getDarkResistance(-iWeakningResistance)/100;
		}
		bIsDamaged		=	TRUE;
	}

	if	(_lpAttacker->m_wConversionIceTime)
	{
		iConversionIceDamage	-=	iConversionIceDamage*getWaterResistance(-_lpAttacker->getWeakningTargetWaterResistance())/100;;
		iWaterDamage			+=	iConversionIceDamage;
	}

	if	(lpSkill->m_wDamageAttribute	==	0)	
		_lpHitInfo->setHit();

	int	iAbsorbDamage		=	0;

	int	aiAbsorbDamageRate[7];

	memset(aiAbsorbDamageRate,0,sizeof(aiAbsorbDamageRate));

	aiAbsorbDamageRate[1]	=	m_bAbsorbMagicDamage+m_bAbsorbFireDamage;
	aiAbsorbDamageRate[2]	=	m_bAbsorbMagicDamage+m_bAbsorbWaterDamage;
	aiAbsorbDamageRate[3]	=	m_bAbsorbMagicDamage+m_bAbsorbWindDamage;
	aiAbsorbDamageRate[4]	=	m_bAbsorbMagicDamage+m_bAbsorbEarthDamage;
	aiAbsorbDamageRate[5]	=	m_bAbsorbMagicDamage+m_bAbsorbLightDamage;
	aiAbsorbDamageRate[6]	=	m_bAbsorbMagicDamage+m_bAbsorbDarkDamage;

	if	(m_lpCharacter && iDamageAttr)
	{
		int	iChance	=	m_lpCharacter->m_asAbsorbMagicDamage[iDamageAttr][0];

		if	(iChance && random(100) < iChance)
			aiAbsorbDamageRate[iDamageAttr]	=	m_lpCharacter->m_asAbsorbMagicDamage[iDamageAttr][1];
	}

	if	(aiAbsorbDamageRate[1]	&&	iFireDamage)	iAbsorbDamage	+=	iFireDamage*aiAbsorbDamageRate[1]/100;		//	불꽃 데미지를 흡수해서 체력으로 변환
	if	(aiAbsorbDamageRate[2]	&&	iWaterDamage)	iAbsorbDamage	+=	iWaterDamage*aiAbsorbDamageRate[2]/100;	//	물 데미지를 흡수해서 체력으로 변환
	if	(aiAbsorbDamageRate[3]	&&	iWindDamage)	iAbsorbDamage	+=	iWindDamage*aiAbsorbDamageRate[3]/100;		//	바람 데미지를 흡수해서 체력으로 변환
	if	(aiAbsorbDamageRate[4]	&&	iEarthDamage)	iAbsorbDamage	+=	iEarthDamage*aiAbsorbDamageRate[4]/100;	//	대지 데미지를 흡수해서 체력으로 변환
	if	(aiAbsorbDamageRate[5]	&&	iLightDamage)	iAbsorbDamage	+=	iLightDamage*aiAbsorbDamageRate[5]/100;	//	빛 데미지를 흡수해서 체력으로 변환
	if	(aiAbsorbDamageRate[6]	&&	iDarkDamage)	iAbsorbDamage	+=	iDarkDamage*aiAbsorbDamageRate[6]/100;		//	어둠 데미지를 흡수해서 체력으로 변환

	iFireDamage		-=	iFireDamage*(aiAbsorbDamageRate[1])/100;
	iWaterDamage	-=	iWaterDamage*(aiAbsorbDamageRate[2])/100;
	iWindDamage		-=	iWindDamage*(aiAbsorbDamageRate[3])/100;
	iEarthDamage	-=	iEarthDamage*(aiAbsorbDamageRate[4])/100;
	iLightDamage	-=	iLightDamage*(aiAbsorbDamageRate[5])/100;
	iDarkDamage		-=	iDarkDamage*(aiAbsorbDamageRate[6])/100;

	if	(iFireDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iFireDamage;}
	if	(iWaterDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iWaterDamage;}
	if	(iWindDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iWindDamage;}
	if	(iEarthDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iEarthDamage;}
	if	(iLightDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iLightDamage;}
	if	(iDarkDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iDarkDamage;}

	if	(bIsPVP)
		iAbsorbDamage	=	iAbsorbDamage*dPVP_ABSORB_DAMAGE_FACTOR/100;

	iAbsorbDamage		=	max(iAbsorbDamage,0);

	float	fCorrectIntelligenceValue	=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage(lpSkill);

//	속성 공격력 = (마법, 기술의 각 속성 기본 공격력 * (1 + 아이템에 의한 대미지 보정율 + 레벨 보정/2))												

	_lpHitInfo->m_dwMagicDamage			=	int(_lpHitInfo->m_dwMagicDamage+_lpHitInfo->m_dwMagicDamage*_lpAttacker->getIntelligence()*fCorrectIntelligenceValue);
	_lpHitInfo->m_dwAbsorbDamage		+=	iAbsorbDamage;

	if	(_lpHitInfo->m_dwMagicDamage)
		_lpHitInfo->setHit();
	else
	if	(bIsDamaged)
		_lpHitInfo->setImmune();
	
	if (!_lpAbility->isWideAreaAttack() && _lpHitInfo->m_dwMagicDamage)
	{
		int	iBlock	=	blocking(_lpAttacker,_lpAbility);

		if (iBlock)
		{
			_lpHitInfo->m_dwMagicDamage	/=	dBLOCKING_FACTOR;

			if	(iBlock	==	eBLOCKING					)
				_lpHitInfo->setBlock();
			if	(iBlock	==	eBLOCKING_NO_ACTION			)
				_lpHitInfo->setNoActionBlock();
			if	(iBlock	==	eBLOCKING_BY_DANCING_BLOCKER)
				_lpHitInfo->setDancingBlockerBlock();
			if	(iBlock	==	eBLOCKING_IGNORE)
				_lpHitInfo->setIgnoreBlock();
		}
	}
}

//
//	마법 데미지 산출
int
cACTOR::getMagicAttackPower(cAbility *_lpAbility)
{
	int		iFireDamage	=0,iWaterDamage=0,iWindDamage=0,iEarthDamage=0,iLightDamage=0,iDarkDamage=0,iPhysicalDamage;
	CSkill	*lpSkill	=	_lpAbility->getSkill();
	BOOL	bIsDamaged	=	FALSE;
	BOOL	bIsPVP		=	FALSE;

	if	(lpSkill->m_wIsMagicDamageBasedLastHitDamage)
		iPhysicalDamage	=	m_iLastAttackPoint+m_iLastDamagePoint;	//	마지막으로 입은 데미지 포인트
	else
		iPhysicalDamage	=	getWeaponDamage(0,FALSE,FALSE,NULL,FALSE);

	int		iCorrectMagicDamageByAlchemyTitle	=	0;
	BOOL	bIsPureMagicDamageSkill				=	lpSkill->isPureMagicDamageSkill();

	if	(bIsPureMagicDamageSkill)
		iCorrectMagicDamageByAlchemyTitle	=	m_wAlchemyTitleFactor;

	if (lpSkill->isFireDamageSkill())
	{
		iFireDamage		=	_lpAbility->getMagicDamage(eAM_ADD_FIRE_DAMAGE, iPhysicalDamage,m_iHP);

		if (iFireDamage	==	0)
			iFireDamage	=	iPhysicalDamage;

		iFireDamage		+=	iFireDamage*getStrengtheningFireDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)/100;
		bIsDamaged		=	TRUE;
	}

	if (lpSkill->isWaterDamageSkill())
	{
		iWaterDamage		=	_lpAbility->getMagicDamage(eAM_ADD_WATER_DAMAGE, iPhysicalDamage,m_iHP);

		if (iWaterDamage	==	0)
			iWaterDamage	=	iPhysicalDamage;

		iWaterDamage	+=	iWaterDamage*getStrengtheningWaterDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)/100;
		bIsDamaged		=	TRUE;
	}

	if (lpSkill->isWindDamageSkill())
	{
		iWindDamage		=	_lpAbility->getMagicDamage(eAM_ADD_WIND_DAMAGE, iPhysicalDamage,m_iHP);

		if (iWindDamage	==	0)
			iWindDamage	=	iPhysicalDamage;

		iWindDamage		+=	iWindDamage*getStrengtheningWindDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)/100;
		bIsDamaged		=	TRUE;
	}

	if (lpSkill->isEarthDamageSkill())
	{
		iEarthDamage		=	_lpAbility->getMagicDamage(eAM_ADD_EARTH_DAMAGE, iPhysicalDamage,m_iHP);

		if (iEarthDamage	==	0)
			iEarthDamage	=	iPhysicalDamage;

		iEarthDamage	+=	iEarthDamage*getStrengtheningEarthDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)/100;
		bIsDamaged		=	TRUE;
	}

	if (lpSkill->isLightDamageSkill())
	{
		iLightDamage		=	_lpAbility->getMagicDamage(eAM_ADD_LIGHT_DAMAGE, iPhysicalDamage,m_iHP);

		if (iLightDamage	==	0)
			iLightDamage	=	iPhysicalDamage;

		iLightDamage	+=	iLightDamage*getStrengtheningLightDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)/100;
		bIsDamaged		=	TRUE;
	}

	if (lpSkill->isDarkDamageSkill())
	{
		iDarkDamage		=	_lpAbility->getMagicDamage(eAM_ADD_DARK_DAMAGE, iPhysicalDamage,m_iHP);

		if (iDarkDamage	==	0)
			iDarkDamage	=	iPhysicalDamage;

		iDarkDamage		+=	iDarkDamage*getStrengtheningDarkDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)/100;
		bIsDamaged		=	TRUE;
	}

	int	iDamage		=	max(iFireDamage+iWaterDamage+iWindDamage+iEarthDamage+iLightDamage+iDarkDamage,0);

	return	iDamage;
}

//
//	물리적인 데미지를 구한다.
BOOL
cACTOR::getPhysicalDamageToShakle(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,int _iContinuousAttackIndex)
{
	if	(m_bIsImmunePhysicalDamage)
		return	FALSE;

	BOOL		bIsBossBattle	=	FALSE;
	CSkill		*lpSkill		=	_lpAbility->getSkill();

	if	(m_wIsBoss	||	_lpAttacker->m_wIsBoss)
		bIsBossBattle	=	TRUE;

	int			iAttackPower	=	_lpAttacker->getPhysicalAttackPower(_lpAbility,m_iLevel,FALSE,bIsBossBattle,_lpHitInfo,m_bf1IsTakeMinDamageForThisTimeAttack,NULL);
	int			iDefensivePower	=	m_wShakleDefensivePower*100;

	LONGLONG	llDamage		=	iAttackPower;
	LONGLONG	llDivideDamage	=	max(llDamage+iDefensivePower,1);
	int			iDamage			=	(int)(llDamage*llDamage/llDivideDamage);

	_lpHitInfo->setHit();
	_lpHitInfo->m_dwPhysicalDamage	=	max(iDamage,100);
	_lpHitInfo->setPhysicalDamage();	//	물리 데미지가 있다고 체크

	return	TRUE;
}

//
//	물리적인 데미지를 구한다.
BOOL
cACTOR::getPhysicalDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,int _iContinuousAttackIndex)
{
	if	(m_bIsImmunePhysicalDamage)
		return	FALSE;

	int			iBlocking;
	CSkill		*lpSkill	=	_lpAbility->getSkill();
	int			iLuckyFactor=	_lpAttacker->getLuck() - getLuck();
	BOOL		bIsPVP		=	FALSE,bIsNEP;
	BOOL		bIsBossBattle=	FALSE;

	bIsNEP		=	isNEPlayer(_lpAttacker);
	bIsPVP		=	m_lpField->isPVP(_lpAttacker,this);

	if	(m_wIsBoss	||	_lpAttacker->m_wIsBoss)
		bIsBossBattle	=	TRUE;

	iBlocking	=	blocking(_lpAttacker,_lpAbility);

	if	(bIsPVP	==	FALSE)
		bIsNEP	=	FALSE;

	int			iAttackPower	=	_lpAttacker->getPhysicalAttackPower(_lpAbility,m_iLevel,bIsPVP,bIsBossBattle,_lpHitInfo,m_bf1IsTakeMinDamageForThisTimeAttack,this);
	int			iDefensivePower	=	getDefensivePower();
	int			iAddCriticalChance	=	0;

	if(_lpAttacker->m_sShadowHideAddCriticalChance)
	{
		if(lpSkill && lpSkill->m_bf1IsApplyShadowHideEffect)
			iAddCriticalChance += _lpAttacker->m_sShadowHideAddCriticalChance;
		_lpAttacker->m_sShadowHideAddCriticalChance = 0;
	}
	if	(m_wHillSkill	!=	0xffff)
		iDefensivePower	=	max(m_wHillDefensivePower,iDefensivePower);

	if	(lpSkill->m_wCorrectTargetDefensivePower)
		iDefensivePower	=	iDefensivePower*lpSkill->m_wCorrectTargetDefensivePower/100;

	LONGLONG	llDamage		=	iAttackPower;
	LONGLONG	llDivideDamage	=	max(llDamage+iDefensivePower,1);
	int			iDamage			=	(int)(llDamage*llDamage/llDivideDamage);

	if	(lpSkill->m_wBlackBluesFactor)
	{
		int	iBlackBluesFactor	=	(lpSkill->m_wBlackBluesFactor+lpSkill->m_wBlackBluesFactorPerLevel*_lpAbility->m_wLevel)/100;

		iBlackBluesFactor		=	min(iBlackBluesFactor,lpSkill->m_wBlackBluesFactorLimit/100);

		int	iAddDamage			=	(iAttackPower-iDamage)*iBlackBluesFactor/100;

		iDamage					+=	iAddDamage;
	}

	_lpHitInfo->setHit();

	if	(iDamage	>=	-1	&&	iDamage	<=	0)		//	데미지가 -1~0 사이이다.
	{
//	타격치가 (-1~0)일 경우에는 (1, 또는 공격자 공격력/1000 중 큰 값)의 타격치를 입힘
		iDamage		=	iAttackPower/1000;

		if (iDamage	<	1)
			iDamage	=	1;
	}
	else
	if	(iDamage	<	-1)		//	데미지가 -1 보다 작다.
	{
//	타격치가 (-1) 이하일 경우에는 1/(타격치의 절대값)의 확률로 (1, 또는 공격자 공격력/1000 중 큰 값)의 타격치를 입힘
		iDamage		=	-iDamage;

		if	(largeRandom(iDamage)	==	0)
		{
			iDamage		=	iAttackPower/1000;

			if (iDamage	<	1)	iDamage	=	1;

		}	else	iDamage	=	0;
	}

	if	(iDamage	<=	0)				//	데미지가 0 이면 Miss 처리 한다.
		goto	label_hit;

	if	(iBlocking)
	{
		iDamage		/=	dBLOCKING_FACTOR;

		if	(iBlocking	==	eBLOCKING					)
			_lpHitInfo->setBlock();
		if	(iBlocking	==	eBLOCKING_NO_ACTION			)
			_lpHitInfo->setNoActionBlock();
		if	(iBlocking	==	eBLOCKING_BY_DANCING_BLOCKER)
			_lpHitInfo->setDancingBlockerBlock();
		if	(iBlocking	==	eBLOCKING_IGNORE)
			_lpHitInfo->setIgnoreBlock();
	}
	else
	{
	//	즉사
		if	(_iContinuousAttackIndex	==	0)
		{
			BOOL	bIsApply	=	TRUE;

			if	(lpSkill->m_wInstanceKillMethod	==	1)	//"캐릭Lv+기술Lv-수치 0Lv 이하 Lv 적"
				if	(_lpAttacker->getLevel() + _lpAbility->m_wLevel - getLevel() < lpSkill->m_aValue[0])
					bIsApply	=	FALSE;

			if	(bIsApply)
			{
				int	iInstanceKillChance	=	_lpAttacker->m_bInstanceKillChance;

				if	(lpSkill->isSkullShot())
				{
					if	(getEquipment(dEQUIP_HELM)	==	NULL)
						if	(m_bIsMoving	==	FALSE)
							iInstanceKillChance		+=	_lpAbility->getInstanceKillChance();
				}
				else
					iInstanceKillChance		+=	_lpAbility->getInstanceKillChance();

				if	(_lpAbility->getInstanceKillToUndead())
					if	(isUndead(TRUE))
					{
						iInstanceKillChance	+=	_lpAbility->getInstanceKillToUndead();
						iInstanceKillChance	-=	getLightResistance();
					}

				if	(_lpAbility->getInstanceKillToAnimal())		
					if	(isAnimal()		)	
						iInstanceKillChance	+=	_lpAbility->getInstanceKillToAnimal();
				if	(_lpAbility->getInstanceKillToHuman())
					if	(isHuman()		)	
						iInstanceKillChance	+=	_lpAbility->getInstanceKillToHuman();
				if	(_lpAbility->getInstanceKillToDemon())
					if	(isDemon()		)	
						iInstanceKillChance	+=	_lpAbility->getInstanceKillToDemon();
				if	(_lpAbility->getInstanceKillToHolyAnimal())	
					if	(isHolyBeast()	)
						iInstanceKillChance	+=	_lpAbility->getInstanceKillToHolyAnimal();

//즉사 공격 발생 확률 (아이템에 의한 즉사만 적용)	
//	공격자 레벨 - 방어자 레벨이 0 ~ 50 일 경우 : 최종치 변동 없음
//	공격자 레벨 - 방어자 레벨이 0 이하일  경우 : 최종치 = 즉사 공격 확률의 총합 * (공격자 레벨) / 방어자 레벨 / 4
//	공격자 레벨 - 방어자 레벨이 50 이상일 경우 : 최종치 = 즉사 공격 확률의 총합 * (공격자 레벨 - 50) / 방어자 레벨
/*
				if (_lpAttacker->m_bInstanceKillChance	==	iInstanceKillChance)
				{
					if (_lpAttacker->getLevel() - getLevel() < -50)
					{
						iInstanceKillChance	=	iInstanceKillChance*(_lpAttacker->getLevel()-50)/getLevel();
//						iInstanceKillChance*100/201;
					}
					else
					if (_lpAttacker->getLevel() - getLevel() < 0)
					{
						iInstanceKillChance	=	iInstanceKillChance*_lpAttacker->getLevel()/getLevel()/4;
//						iInstanceKillChance*150/201/4;
					}
				}
*/
				//	즉사 확률 = 공격자의 즉사 확률 - (타겟의 즉사 저항 + 저주 저항)
				iInstanceKillChance	=	iInstanceKillChance-(m_bInstanceKillResistance+getCurseResistance());

				if	(bIsPVP)
				{
					if	(bIsNEP)
						iInstanceKillChance	=	iInstanceKillChance*dPVP_CORRECT_INSTANCE_KILL_CAHNCE_NE/100;
					else
						iInstanceKillChance	=	iInstanceKillChance*dPVP_CORRECT_INSTANCE_KILL_CAHNCE/100;
				}
				
				cJOB	*lpJob		=	getJob();

				if	(lpJob)
					if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_SEMI_BOSS1)
						iInstanceKillChance	=	0;

				if	(random(100)	<	iInstanceKillChance)
				{
					if	(bIsPVP)
					{
						if	(bIsNEP)	//	천적이다.
							iDamage		=	m_iHP*dPVP_INSTANCE_KILL_FACTOR_NE/100;
						else
							iDamage		=	m_iHP*dPVP_INSTANCE_KILL_FACTOR/100;
					}
					else
						_lpHitInfo->setInstanceKill();

					goto	label_hit;
				}
			}
		}	//	즉사

//	크러쉬 블로우~
//	치명타, 결정타 저항력 = 모든 장비, 아이템, 기술의 치명타,결정타 저항력 합 * (1 +  (지혜 + 카리스마) * 0.1)
		{
			int	iCrushHitChance			=	_lpAbility->getFixCrushChance();

			if	(iCrushHitChance	==	0)
			{
				int	iCrushBlowResistance=	m_bCrushBlowResistance;
				int	iAddResistance		=	iCrushBlowResistance*(getWisdom()+getCharisma())/1000;
				iCrushBlowResistance	+=	min(iAddResistance,50);
				iCrushBlowResistance	=	min(iCrushBlowResistance,100);

				iCrushHitChance			=	(_lpAttacker->m_bCorrectCrushBlowChance+_lpAbility->getCrushChance())*(100-iCrushBlowResistance)/100;

				if	(bIsPVP)
				{
//● 결정타 대미지는 체력의 1/3이 아닌 1/24로 처리										
//	또한 결정타 발생확률 계산시는 최종 확률(공격자 확률*방어자 보정치)에 50%의 보정치를 곱해서 적용									
					if	(bIsNEP)
						iCrushHitChance		=	iCrushHitChance*dPVP_CORRECT_CRUSH_BLOW_CAHNCE_NE/100;
					else
						iCrushHitChance		=	iCrushHitChance*dPVP_CORRECT_CRUSH_BLOW_CAHNCE/100;
				}
			}

			if	(random(100)	<	iCrushHitChance)
				_lpHitInfo->setCrush();
		}

		//	크리티컬 샷~ 데미지 2배? 공격력 2배?
		{
			BOOL bIsApplyAbilityCritical	=	TRUE;

			if(lpSkill->m_bf1IsApplyCriticalEffectWhenBeLightEffect && !isLightAdditionalEffect())
			{
				bIsApplyAbilityCritical		=	FALSE;
			}

			int	iCriticalHitChance		=	0;
			
			if	( _lpAbility->getFixCriticalChance())
			{
				if(bIsApplyAbilityCritical)
					iCriticalHitChance		= _lpAbility->getFixCriticalChance()*10;	//	천분률로 만들기 위해 *10을 했다.				
			}
			else			
			{
				int	iCriticalHitResistance	=	m_bCriticalHitResistance;
				int	iAddResistance			=	iCriticalHitResistance*(getWisdom()+getCharisma())/1000;

				iCriticalHitResistance		+=	min(iAddResistance,50);
				iCriticalHitResistance		=	min(iCriticalHitResistance,100);

				iCriticalHitChance			=	_lpAttacker->m_correctCriticalHitChance.getCorrectedValue();

				if(bIsApplyAbilityCritical)
				{
					iCriticalHitChance		+=	_lpAbility->getCriticalChance()/100;
					if	(_lpAbility->getCriticalToUndead())
					{
						if	(isUndead())
						{
							iCriticalHitChance	+=	_lpAbility->getCriticalToUndead();
							//						iCriticalHitChance	-=	getLightResistance();
						}
					}	
					if	(_lpAbility->getCriticalToDemon())
					{
						if	(isDemon())
						{
							iCriticalHitChance	+=	_lpAbility->getCriticalToDemon();
							//						iCriticalHitChance	-=	getLightResistance();
						}
					}	
					if	(_lpAbility->getCriticalToAnimal())		
					{
						if	(isAnimal()		)
							iCriticalHitChance	+=	_lpAbility->getCriticalToAnimal();
					}
					if	(_lpAbility->getCriticalToHuman())
					{
						if	(isHuman()		)
							iCriticalHitChance	+=	_lpAbility->getCriticalToHuman();
					}
					if	(_lpAbility->getCriticalToHolyAnimal())	
					{
						if	(isHolyBeast()	)
							iCriticalHitChance	+=	_lpAbility->getCriticalToHolyAnimal();
					}
					iCriticalHitChance += iAddCriticalChance;
				}
//				크리티컬 발생 확률 = (민첩성 * 0.001 + 아이템, 기술에 의한 보정치 총합) * (1 - 방어자의 크리티컬 저항력)
				iCriticalHitChance	=	(_lpAttacker->getAgility()/10+iCriticalHitChance*10)*(100-iCriticalHitResistance)/100;
			}

			if	(_lpAttacker->isActiveSurvivalInstinctTitle()&& lpSkill->isPurePhysicalDamageSkill() && 
				_lpAbility->getHitRange() == 0 && isTargetInAttackRange(_lpAttacker,200) && isAttackToManyPersonSkill(_lpAbility) )
				iCriticalHitChance	=	max(_lpAttacker->m_wSurvivalInstinctsTitleCriticalChance*10,iCriticalHitChance);
			
			if(_lpAttacker->m_fightEnergy.isIncreaseAbsoluteCriticalPercent())
			{// 절대 크리티컬 % 상승 투기상태일때 다른 크리티컬은 모두 무시한다!
				iCriticalHitChance = _lpAttacker->m_fightEnergy.m_iValue;
			}
			
			if	(random(1000)	<	iCriticalHitChance)
			{
				//더블 크리티컬 발생 확률							
				//● 크리티컬 히트가 발생한 경우 (공격자의 운 - 방어자의 운) / 400의 확률로 더블 크리티컬이 발생됨							
				//● 더블 크리티컬 = 통상 대미지 * 4							
				//● 더블 크리티컬 발생 시 Critical 표시 옆에 클로버 아이콘 생김
				
				if	(iLuckyFactor > 0	&& iCriticalHitChance <	1000  &&  !lpSkill->m_bf1IsIgnoreLucky && !lpSkill->isSkullShot() && !lpSkill->isSnipeShot() )		// 운무시 추가....
				{
					if	(random(400) < iLuckyFactor)
						_lpHitInfo->setLuckyDoubleCritical();
					else
						_lpHitInfo->setCritical();
				}
				else
				{
					//크리티컬 저항 확률							
					//● 크리티컬 히트가 발생한 경우 (방어자의 운 - 공격자의 운) / 300의 확률로 크리티컬 회피							
					//● 운으로 크리티컬 히트를 회피한 경우 Critical Block 표시와 클로버 아이콘							
					if	(random(300) < -iLuckyFactor && iCriticalHitChance < 1000 && !lpSkill->m_bf1IsIgnoreLucky && !lpSkill->isSkullShot() && !lpSkill->isSnipeShot())	// 운무시추가..
						_lpHitInfo->setLuckyDodgeCritical();
					else
						_lpHitInfo->setCritical();
				}
			}
		}	//	크리티컬 샷~

		{	// 강타!
			
			if(_lpHitInfo->isCritical() == FALSE && _lpHitInfo->isLuckyDoubleCritical() == FALSE)
			{		// 크리티컬류가 아니면 강타확률을 체크한다.				
				int iHardBlowChance =	 _lpAttacker->getValuePassiveSkill(eSKILL_PEE_INCREASE_HARD_BLOW) + _lpAttacker->m_correcthardBlowHitChance.getCorrectedValue()*100;
				if(_lpAttacker->m_wIncreaseHardBlowTime)
					iHardBlowChance += _lpAttacker->m_wHardBlowValue;
				int iRand = largeRandom(10000);
				if(iRand < iHardBlowChance)
				{
					_lpHitInfo->setHardBlow();
				}
			}
		}

		if	(!m_wIsDontHitAction	&&	m_wHitActionTime	==	0)//	히트 액션을 취하지 않는 스킬 사용중이 아니다.
		{
			if	(m_bf1IsCancelKnockOut)
				m_bf1IsCancelKnockOut	=	FALSE;
			else
			{
//			녹아웃 확률 = {공격자 레벨 / (공격자 레벨 + 방어자 레벨)} /2  * (1 + 아이템 보정치) [%]	
				int	iProbabilityOfHitAction	=	_lpAttacker->getLevel()*50/(_lpAttacker->getLevel()+getLevel())*(100+_lpAttacker->m_bCorrectKnockoutAttackChance)/100;
				iProbabilityOfHitAction		-=	getNockOutResistanceByPassiveSkill();

				if	(m_wBiteTarget	==	0xffff)
					if	(random(100)	<	iProbabilityOfHitAction)
						_lpHitInfo->setHitAction();
			}
		}
	}

//	상성 공격
	{
		if	(isUndead())		
			iDamage	=	iDamage*(100+_lpAttacker->m_wAddDamageToUndead)/100;
		if	(isDemon())		
			iDamage	=	iDamage*(100+_lpAttacker->m_wAddDamageToDemon)/100;
		if	(isHuman())		
			iDamage	=	iDamage*(100+_lpAttacker->m_wAddDamageToHuman)/100;
		if	(isAnimal())		
			iDamage	=	iDamage*(100+_lpAttacker->m_wAddDamageToAnimal)/100;
		if	(isHolyBeast())	
			iDamage	=	iDamage*(100+_lpAttacker->m_wAddDamageToHolyAnimal)/100;
	}

//	물리 대미지 흡수
	if	(m_lpCharacter)
	{
		int	iChance	=	m_lpCharacter->m_asAbsorbMagicDamage[0][0];

		if	(iChance && random(100) < iChance)
		{
			int	iAbsorbDamageRate	=	m_lpCharacter->m_asAbsorbMagicDamage[0][1];
			int	iAbsorbDamage		=	iAbsorbDamageRate*iDamage/100;

			iDamage					-=	iAbsorbDamage;

			_lpHitInfo->m_dwAbsorbDamage	=	iAbsorbDamage;

		}
	}

label_hit:
	
	_lpHitInfo->m_dwPhysicalDamage	=	iDamage;
	_lpHitInfo->setPhysicalDamage();	//	물리 데미지가 있다고 체크

	return	TRUE;
}

//
//	근접공격에 의해 약속된-_- 데미지를 입힌다.
BOOL
cACTOR::hitBookedDamage(int _iTimes)
{
	BOOL		bIsApplyAdditionalEffect	=	FALSE;
	cACTOR		*lpTarget	=	m_lpField->getActor(m_attackInfo.m_wTarget);
	CHitInfo	*lpInfo		=	m_attackInfo.getInfo(_iTimes);
	BOOL		bIsHit		=	lpInfo->isHit();

	if	(!lpTarget)
	{
		m_attackInfo.reset();
		m_wUseSkill	=	0xffff;

		return	bIsHit;
	}

	if	(_iTimes	==	0)
		bIsApplyAdditionalEffect	=	TRUE;

	if	(!strike(lpTarget,lpInfo,&m_attackInfo.m_ability,bIsApplyAdditionalEffect))
	{
		if	(!lpTarget	||	lpTarget->isDeath()	||	lpTarget->m_wSerialInField == 0xffff)
		{
			m_attackInfo.reset();
			m_wUseSkill	=	0xffff;

			return	bIsHit;
		}
	}

	return	bIsHit;
}

//
//	반격 효과 처리
void
cACTOR::operateCounterEffect(cACTOR *_lpAttacker)
{
	if	(m_bIsGuildObject)
		return;
	if	(m_wSerialInField	==	0xffff	)
		return;
	if	(isDeath()						)
		return;

	if	(_lpAttacker->m_wSOD_Time)
		operateSmellOfDeath(_lpAttacker);

//	저주 증폭
	if	(_lpAttacker->m_wTargetCurseAmplificationTime)
		if	(random(100)	<	100-getCurseResistance())
		{
			int	iCurseAmplificationTime	=	_lpAttacker->m_wTargetCurseAmplificationTime*(100-getCurseResistance())/100;

			iCurseAmplificationTime		*=	dSYNC_FPS;
			m_wCurseAmplificationTime	=	max(iCurseAmplificationTime,dSYNC_FPS);
		}

	BOOL	bIsPVP	=	FALSE;

	bIsPVP		=	m_lpField->isPVP(_lpAttacker,this);

//	대상을 버서크로 만들 확률

	if	(random(100)	<	_lpAttacker->m_bTargetBerserkChance-getCurseResistance())
	{
		int	iBerserkTime		=	_lpAttacker->m_wTargetBerserkTime*(100-getCurseResistance())/100;

		if	(m_wCurseAmplificationTime)
			iBerserkTime		=	iBerserkTime*dCURSE_AMPLIFICATION_FACTOR/100;

		m_wBerserkTime			=	max(m_wBerserkTime,iBerserkTime);
	}
/*
//	도망가게 만듦
	if (random(100)	<	_lpAttacker->m_bTargetFleeChance)
	{
		if (m_wFleeTime ==	0)	packet.status.isBreakedArmor=	1;
		m_wFleeTime	=	max(m_wFleeTime,_lpAttacker->m_wTargetArmorBreakTime*(100-getCurseResistance())/100);
	}
*/	
//	대상을 갑옷 파괴 상태로 만들 확률
	if	(random(100)	<	_lpAttacker->m_bTargetArmorBreakChance-getCurseResistance())
	{
		if	(!bIsPVP)
		{
			int	iBreakedArmorTime			=	_lpAttacker->m_wTargetArmorBreakTime*(100-getCurseResistance())/100;

			if (m_wCurseAmplificationTime)
				iBreakedArmorTime			=	iBreakedArmorTime*dCURSE_AMPLIFICATION_FACTOR/100;

			m_wBreakedArmorTime		=	max(iBreakedArmorTime,m_wBreakedArmorTime);
		}
	}

//	대상을 무기 파괴 상태로 만들 확률
	if	(random(100)	<	_lpAttacker->m_bTargetWeaponBreakChance-getCurseResistance())
	{
		if	(!bIsPVP)
		{
			int	iBreakedWeaponTime		=	_lpAttacker->m_wTargetWeaponBreakTime*(100-getCurseResistance())/100;

			if (m_wCurseAmplificationTime)
				iBreakedWeaponTime		=	iBreakedWeaponTime*dCURSE_AMPLIFICATION_FACTOR/100;

			m_wBreakedWeaponTime		=	max(m_wBreakedWeaponTime,iBreakedWeaponTime);
		}
	}

//	대상을 퍼스트 타겟으로 만들 확률
	if	(random(100)	<	_lpAttacker->m_bTargetFirstTargetChance-getCurseResistance())
	{
		int	iTargetFirstTargetTime		=	_lpAttacker->m_wTargetFirstTargetTime*(100-getCurseResistance())/100;

		if	(m_wCurseAmplificationTime)
			iTargetFirstTargetTime		=	iTargetFirstTargetTime*dCURSE_AMPLIFICATION_FACTOR/100;

		m_wTargetedTime					=	max(m_wTargetFirstTargetTime,iTargetFirstTargetTime);
	}

//	대상을 AI저하 상태로 만들 확률
	if	(random(100)	<	_lpAttacker->m_bTargetSimpltoneChance-getCurseResistance())
	{
		int	iTargetSimpltoneTime		=	_lpAttacker->m_wTargetSimpltoneTime*(100-getCurseResistance())/100;

		if	(m_wCurseAmplificationTime)
			iTargetSimpltoneTime		=	iTargetSimpltoneTime*dCURSE_AMPLIFICATION_FACTOR/100;

		m_wSimpletonTime				=	max(m_wTargetSimpltoneTime,iTargetSimpltoneTime);
	}

//	대상을 혼란 시킨다.
	if	(random(100)	<	_lpAttacker->m_bTargetConfuseChance-getConfuseResistance())
	{
		int	iConfuseTime		=	_lpAttacker->m_wTargetConfuseTime*(100-getConfuseResistance())/100;

		if	(m_wCurseAmplificationTime)
			iConfuseTime		=	iConfuseTime*dCURSE_AMPLIFICATION_FACTOR/100;

		setCofuseTime(iConfuseTime,_lpAttacker);
	}

//	대상을 유혹한다.
	if (!bIsPVP && random(100)	<	_lpAttacker->m_bTargetCharmingChance-getCharmingResistance())
	{
		charm(_lpAttacker,_lpAttacker->m_wTargetCharmingTime*(100-getCharmingResistance())/100);
	}

//	대상을 경직 시킨다.
	int	iTargetStunChance	=	_lpAttacker->m_bTargetStunChance-getStunResistance();

	if (bIsPVP)
		iTargetStunChance	=	min(iTargetStunChance,dPVP_TOP_STUN_CHANCE);
		
	if (random(100)	<	iTargetStunChance)
	{
		int	iStunTime		=	_lpAttacker->m_wTargetStunTime*(100-getStunResistance())/100;

		if (bIsPVP)
		{
			if (isNEPlayer(_lpAttacker))	//	천적이다.
				iStunTime	=	iStunTime*dPVP_CORRECT_STUN_TIME_NE/100;
			else
				iStunTime	=	iStunTime*dPVP_CORRECT_STUN_TIME/100;
		}

		if (m_wCurseAmplificationTime)
			iStunTime		=	iStunTime*dCURSE_AMPLIFICATION_FACTOR/100;

		m_wStunTime			=	max(m_wStunTime,iStunTime);
	}

//	대상을 콜드 상태로 만든다.
	if (random(100)	<	_lpAttacker->m_bTargetColdChance-getColdResistance())
	{
		int	iColdTime		=	_lpAttacker->m_wTargetColdTime*(100-getColdResistance())/100;

		if (m_wCurseAmplificationTime)
			iColdTime		=	iColdTime*dCURSE_AMPLIFICATION_FACTOR/100;

		m_wColdTime			=	max(m_wColdTime,iColdTime);
	}

//	대상을 얼린다.
	if (random(100)	<	_lpAttacker->m_bTargetFreezeChance-getFreezeResistance())
	{
		int	iFreezeTime		=	_lpAttacker->m_wTargetFreezeTime*(100-getFreezeResistance())/100;

		if (m_wCurseAmplificationTime)
			iFreezeTime		=	iFreezeTime*dCURSE_AMPLIFICATION_FACTOR/100;

		setFreezeTime(iFreezeTime);
	}

//	대상을 장님으로 만들 확률
	if (m_wBlindTime	==	0)
		if (random(100)	<	_lpAttacker->m_bTargetBlindChance-getBlindResistance())
		{
			m_wBlindTime			=	_lpAttacker->m_wTargetBlindTime*(100-getBlindResistance())/100;

			if (m_wCurseAmplificationTime)
			{
				m_wBlindTime	=	m_wBlindTime*dCURSE_AMPLIFICATION_FACTOR/100;
			}
		}

//	대상을 돌로 만들 확률
	int	iTargetStoneChance	=	_lpAttacker->m_bTargetStoneChance-getStoneResistance();

	if (bIsPVP)
		iTargetStoneChance	=	min(iTargetStoneChance,dPVP_TOP_STONE_CHANCE);

	if	(m_wStoneTime	==	0)
		if (random(100)	<	iTargetStoneChance)
		{
			m_wStoneTime		=	_lpAttacker->m_wTargetStoneTime*(100-getStoneResistance())/100;

			if (bIsPVP)
			{
				if (isNEPlayer(_lpAttacker))	//	천적이다.
					m_wStoneTime	=	m_wStoneTime*dPVP_CORRECT_STONE_TIME_NE/100;
				else
					m_wStoneTime	=	m_wStoneTime*dPVP_CORRECT_STONE_TIME/100;
			}

			if (m_wCurseAmplificationTime)
				m_wStoneTime	=	m_wStoneTime*dCURSE_AMPLIFICATION_FACTOR/100;
		}

//	대상을 재울 확률
	if	(m_wSleepTime	==	0)
		if (random(100)	<	_lpAttacker->m_bTargetSleepChance-getSleepResistance())
		{
			m_wSleepTime			=	_lpAttacker->m_wTargetSleepTime*(100-getSleepResistance())/100;
			if (m_wCurseAmplificationTime)
				m_wSleepTime	=	m_wSleepTime*dCURSE_AMPLIFICATION_FACTOR/100;
			
			m_wSleepTime	=	max(m_wSleepTime,1);
			m_wSleepTime	+=	0x8000;
		}

//	중독
	if	(_lpAttacker->m_wTargetPoisonedTime)
	{
		int	iDamage;
		int	iMinDamage		=	_lpAttacker->m_bAddPoisonMinDamage;
		int	iMaxDamage		=	_lpAttacker->m_bAddPoisonMaxDamage;
		int	iTime			=	_lpAttacker->m_wTargetPoisonedTime;
		int	iTimeResistance	=	100-getPoisonResistance();
		int	iDamageResistance=	100-getEarthResistance();

		if	(iTimeResistance	>	0	&&	iDamageResistance	>	0)
		{
			iDamage			=	iMinDamage+largeRandom(iMaxDamage-iMinDamage+1);
			iDamage			=	iDamage*iDamageResistance;	//	대미지는 곱하기 100을 해줘야 한다.
			iTime			=	iTime*iTimeResistance/100;

			if (bIsPVP)
			{
				iDamage		=	GetP2PCorrectDamage(iDamage);	//	중독 대미지
				iTime		=	iTime*dPVP_POISON_TIME_FACTOR/100;//	
			}

			setPoisonDamage(iTime,iDamage,_lpAttacker->m_wSerialInField);
		}
	}

//	명중률 저하
	if	(_lpAttacker->m_wTargetDeclineHitChanceTime)
		if	(random(100)	<	100-getDeclinePowerResistance())
		{
			int	iValue	=	_lpAttacker->m_wTargetDeclineHitChanceTime*(100-getDeclinePowerResistance())/100;

			if (m_wCurseAmplificationTime)
				iValue	=	iValue*dCURSE_AMPLIFICATION_FACTOR/100;

			m_hitChance.correctValue(-dDECLINE_HIT_CHANCE_VALUE,iValue);
		}

//	회피율 저하
	if	(_lpAttacker->m_wTargetDeclineDodgeChanceTime)
		if	(random(100)	<	100-getDeclinePowerResistance())
		{
			int	iValue	=	_lpAttacker->m_wTargetDeclineDodgeChanceTime*(100-getDeclinePowerResistance())/100;

			if (m_wCurseAmplificationTime)
				iValue	=	iValue*dCURSE_AMPLIFICATION_FACTOR/100;

			m_dodgeChance.correctValue(-dDECLINE_DODGE_CHANCE_VALUE,iValue);
		}
	if	(largeRandom(10000) < getValuePassiveSkill(eSKILL_PEE_ADD_BIT_CHANCE_WHEN_BE_HIT))
	{
		_lpAttacker->addBit(m_wSerialInField, m_dwNameHashCode,1, getValuePassiveSkill(eSKILL_PEE_ADD_BIT_CHANCE_WHEN_BE_HIT,2)/100);
	}
}

//
//	추가 공격 효과 처리
void
cACTOR::operateAdditionalAttackEffect(cACTOR *_lpAttacker)
{
	if	(m_bIsGuildObject)
		return;
	if	(m_wSerialInField	==	0xffff	)
		return;
	if	(isDeath()						)
		return;

//	프리즈 반격
	if	(m_bReturnFreezeChance)
	{
		int	iActiveChance	=	m_bReturnFreezeChance;
		
		iActiveChance		=	iActiveChance*(100-_lpAttacker->getFreezeResistance())/100;

		if	(random(100)	<	iActiveChance)
		{
			int	iUpkeepTime		=	m_wReturnFreezeTime*dSYNC_FPS;

			iUpkeepTime			=	iUpkeepTime*(100-_lpAttacker->getFreezeResistance())/100;

			if	(m_wCurseAmplificationTime)
				iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			_lpAttacker->setFreezeTime(iUpkeepTime);
		}
	}

//	공격자 콜드
	if	(m_bReturnColdChance)
	{
		int	iActiveChance	=	m_bReturnColdChance;
		
		iActiveChance		=	iActiveChance*(100-_lpAttacker->getColdResistance())/100;

		if	(random(100)	<	iActiveChance)
		{
			int	iUpkeepTime		=	m_wReturnColdTime;

			iUpkeepTime			=	iUpkeepTime*(100-_lpAttacker->getColdResistance())/100;

			if	(m_wCurseAmplificationTime)
				iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			_lpAttacker->m_wColdTime	=	max(iUpkeepTime,m_wColdTime);
		}
	}

}

//
//	데미지를 입힌다.
BOOL
cACTOR::strike(cACTOR *_lpTarget,CHitInfo *_lpHitInfo,cAbility *_lpAbility,BOOL _bIsApplyAdditionalEffect,BOOL _bIsApplyLifeSteal,int _iX,int _iY,BOOL _bIsCounter)
{		// 마지막 수정일 : 09.09.24
	if	(!_lpTarget	||	_lpTarget->m_wTransToWeaponTime	||	_lpTarget->m_iGuildGuardianType)
		return	FALSE;

	if	(_lpTarget->isAttackToShakleActor(this))
	{
		_lpTarget->damagedBy(this,_lpAbility,_lpHitInfo->m_dwPhysicalDamage,_lpHitInfo->m_dwMagicDamage,FALSE,FALSE,_iX,_iY,_lpHitInfo,FALSE);

		return	TRUE;
	}

	if	(m_wSelfReproofTime	&&	_lpHitInfo->isMiss())
		operateSelfReproofStatus();

	WORD	wIntervalShooter=	m_wIntervalShooter;
	BOOL	bIsPVP			=	FALSE;
	BOOL	bIsNEP			=	isNEPlayer(_lpTarget);
	BOOL	bIsHit			=	FALSE;

	if	(isPlayer())
		bIsHit	=	bIsHit;

	if	(_lpHitInfo->isInvincible())
		return	TRUE;

	if (_lpTarget->m_wSerialInField	==	0xffff || _lpTarget->isDeath())
		return	FALSE;

	bIsPVP					=	m_lpField->isPVP(this,_lpTarget);

	if (_lpTarget->m_lpTarget	&&	m_wSerialInField	!=	_lpTarget->m_lpTarget->m_wSerialInField)
		_lpTarget->m_wAnotherTargetBitMeCount++;

	if	(_lpAbility	==	NULL)
		_lpAbility	=	&m_attackInfo.m_ability;

	CSkill	*lpSkill		=	_lpAbility->getSkill();

	if (lpSkill	&&	lpSkill->isAgressiveSkill())
	{
		if	(_lpTarget && _lpTarget->isPlayer())
			_lpTarget->breakdownArmorDurability();

		if	(isPlayer())
			breakdownWeaponDurability();
		
		if	(_lpTarget)
		{
			if	(_lpTarget	!=	this)
				_lpTarget->m_lpLastAttacker=	this;
		}
	}

	int		iDamage	=	_lpHitInfo->getDamage();

	_lpTarget->operateTamedMonsterReactionForUnderAttackLord(this);

	if	(_lpHitInfo->isDodge())
	{
		cAbility	ability,*lpAbility=NULL;
		int			iSpentCp;

		ability.m_wSkill			=	(WORD)_lpHitInfo->m_dwPhysicalDamage/100;
		ability.m_wLevel			=	(WORD)_lpHitInfo->m_dwMagicDamage/100;
		_lpTarget->m_wDodgeAbility	=	_lpTarget->getAbilityIndexBySkill(ability.m_wSkill);

		iSpentCp					=	ability.getSpentCP()-ability.getGetCP();
		
		_lpTarget->dodgeAction(this,iSpentCp);

		return	FALSE;
	}
	else
	if	(_lpHitInfo->isDancingBlockerBlock())
	{
		cAbility	*lpAbility	=	_lpTarget->getDancingBlockerSkill();

		if (lpAbility)
		{
			_lpTarget->increaseCP(lpAbility->getGetCP());
			_lpTarget->sendPlayerBasicInformation();
		}
	}
	else
	if	(_lpHitInfo->isBlock())
	{
		int iShieldRigidityTime = dDEFAULT_SHIELD_RIGIDITY_TIME/(m_iCorrectRecoverBlocking+100)/100;
		iShieldRigidityTime = max(iShieldRigidityTime,0);

		_lpTarget->blockingAction(iShieldRigidityTime);

		int	iCount	=	0;

		for (int i=0;;i++)
		{
			cAbility	reaciton;

			if (!_lpTarget->getActivateReaction(&reaciton,eRSAT_BLOCKING,i,this))
				break;

			if (random(100) < reaciton.getActivateChance())
				_lpTarget->operateReactionSkill(this,&reaciton);
		}

		cAbility	advancedBlocking;

		if (_lpTarget->getActiveAdvancedBlockingSkill(&advancedBlocking))
		{
			_lpTarget->increaseCP(advancedBlocking.getGetCP());
			_lpTarget->sendPlayerBasicInformation();
		}
	}
	else
	{
		m_wIntervalShooter		=	0xffff;

		int	iRememberDamage		=	iDamage;

		if	(_lpHitInfo->isCrush()			)
		{
			if	(bIsPVP)
			{
/*				if	(g_bIsDuelServer)
				{
					if	(bIsNEP)
						iDamage	+=	_lpTarget->m_iHP/dDUEL_CRUSH_BLOW_FACTOR_NE;
					else
						iDamage	+=	_lpTarget->m_iHP/dDUEL_CRUSH_BLOW_FACTOR;
				}
				else*/
				{
					if	(bIsNEP)
						iDamage	+=	_lpTarget->m_iHP/dPVP_CRUSH_BLOW_FACTOR_NE;
					else
						iDamage	+=	_lpTarget->m_iHP/dPVP_CRUSH_BLOW_FACTOR;
				}
			}
			else
				iDamage	+=	_lpTarget->m_iHP/dCRUSH_BLOW_FACTOR;//	크러쉬 블로우.. -o-
		}
		if	(_lpHitInfo->isInstanceKill()	)
		{
			if	(bIsPVP)
			{
				if	(bIsNEP)	//	천적이다.
					iDamage	+=	_lpTarget->m_iHP*dPVP_INSTANCE_KILL_FACTOR_NE/100;
				else
					iDamage	+=	_lpTarget->m_iHP*dPVP_INSTANCE_KILL_FACTOR/100;
			}
			else
				iDamage	=	_lpTarget->m_iHP;//	즉사
		}
		if	(_lpHitInfo->isHitAction()		)
		{
			_lpTarget->hitAction();
			_lpTarget->sendHitAction();
		}

		if	(iDamage>iRememberDamage)	
			_lpHitInfo->m_dwPhysicalDamage	+=	(iDamage-iRememberDamage);

		if	(_lpHitInfo->m_dwPhysicalDamage && _bIsApplyAdditionalEffect )
		{
			if	(m_wHillSkill	==	0xffff)
			{
				_lpTarget->operateAdditionalAttackEffect(this);	//	공격 부가 효과 처리
				_lpTarget->operateCounterEffect(this);			//	반격 효과 처리
				
				if(lpSkill && lpSkill->m_sAddDamagePercent)
				{
					int iPhysicalDamage	=	int(_lpHitInfo->m_dwPhysicalDamage	* (lpSkill->m_sAddDamagePercent/10000.0));
					if( iPhysicalDamage	)
					{
						_lpTarget->reduceHP(iPhysicalDamage,this);
						_lpTarget->addSendHitDamage(this,iPhysicalDamage,0,dATTACK_RESULT_FIELD_HIT,_lpAbility->m_wSkill);
					}
				}
			}
		}
		if	(_lpHitInfo->getDamage() && _bIsApplyAdditionalEffect)
		{
			if	(m_wHillSkill	==	0xffff)
			{
				if(_lpTarget->m_wElectricShockTime)
				{
					int iPhysicalDamage	=	int(_lpHitInfo->m_dwPhysicalDamage	* (_lpTarget->m_wElectricShockDamagePercent/10000.0));
					int iMagicDamage	=	int(_lpHitInfo->m_dwMagicDamage		* (_lpTarget->m_wElectricShockDamagePercent/10000.0));
					_lpTarget->reduceHP(iPhysicalDamage+iMagicDamage,this);
					_lpTarget->addSendSimpleHit(iPhysicalDamage/100,iMagicDamage/100,eAHT_ELECTRIC_SHOCK);
				}
			}
		}
		if	(wIntervalShooter	!=	0xffff && _bIsApplyAdditionalEffect)
			m_lpField->bookingIntervalShooter(wIntervalShooter,this,_lpTarget);

		int	iReduceHP			=	min(_lpTarget->m_iHP,(int)_lpHitInfo->m_dwPhysicalDamage);
		int	iLifeStealDamage	=	0;
		int	iPhysicalDamageToHP	=	_lpAbility->getDamageToHP();

		if	(_bIsApplyLifeSteal)
		{
			int iLifeStrealPercent = m_wLifeStealPecentage;

			if(_lpTarget->m_wBloodDonationTime)
				iLifeStrealPercent += _lpTarget->m_wBloodDonationPercent;

			if	(iLifeStrealPercent)
				iLifeStealDamage+=	iReduceHP*iLifeStrealPercent/100;

			if	(iPhysicalDamageToHP)
				iLifeStealDamage+=	iReduceHP*iPhysicalDamageToHP/100;

			if	(iLifeStealDamage)
			{
				if	(_lpTarget->isTargetInAttackRange(this,dRETURN_DAMAGE_RANGE))
				{
					int	iCorrectResistance	=	100;

					if (m_wCurrentField		>=	278)
						iCorrectResistance	=	max(100-_lpTarget->getCurseResistance(),0);
					else
						iCorrectResistance	=	max(100-_lpTarget->getCurseResistance(),90);

					iCorrectResistance	=	min(iCorrectResistance,100);

					iLifeStealDamage	=	iLifeStealDamage*iCorrectResistance/100;

					if	(iLifeStealDamage	>	0)
					{
						if	(instanceHealHP(iLifeStealDamage))
							m_lpField->sendAbsorbDamage(m_wSerialInField,iLifeStealDamage);
					}
				}
			}
		}

		bIsHit	=	TRUE;

		if	(lpSkill->isAgressiveSkill()&& lpSkill->m_wSerial != dSKILL_SERIAL_HOLYCROSS&& _lpHitInfo->isHit())		//  히트시 적용
			applySkillExtraEffects(this,_lpAbility,iDamage);	//	때릴때 본인 한테 적용
	}

	if	(m_wSerialInField	==	0xffff)
		g_eh.addStaticLog("cACTOR::strike 1");
	if	(_lpTarget->m_wSerialInField	==	0xffff)
		g_eh.addStaticLog("cACTOR::strike 2");

	BOOL	bResult		=	_lpTarget->damagedBy(this,_lpAbility,_lpHitInfo->m_dwPhysicalDamage,_lpHitInfo->m_dwMagicDamage,_lpHitInfo->isPhysicalDamage(),_bIsApplyAdditionalEffect,_iX,_iY,_lpHitInfo,_bIsCounter);

	if	(m_wSerialInField	==	0xffff)
		g_eh.addStaticLog("cACTOR::strike 3");
	if	(_lpTarget->m_wSerialInField	==	0xffff)
		g_eh.addStaticLog("cACTOR::strike 4");
		
	if	(_lpHitInfo->isReflection())
		return	TRUE;

	if	(_lpTarget->isDeath()		)
		return	FALSE;

	if(!_lpHitInfo->isBlock())
	{
		int	iAbsorbDamage	=	0;
		int	iDamageToCP		=	0;

		if (_lpTarget->m_bDamageToCPRate)
			iDamageToCP		=	iDamage*_lpTarget->m_bDamageToCPRate/100;
		if (_lpHitInfo->m_dwAbsorbDamage)
			iAbsorbDamage	+=	_lpHitInfo->m_dwAbsorbDamage;

		if (iAbsorbDamage)
			_lpTarget->absorbDamage(_lpTarget->m_wSerialInField,iAbsorbDamage);

		if (iDamageToCP)
		{
			_lpTarget->increaseCP(iDamageToCP);
			_lpTarget->sendPlayerBasicInformation();
		}
	}

	if (_lpTarget->isSitdown()					)
		return	bResult;

	if (m_wSerialInField			==	0xffff	)
		return	bResult;
	if (_lpTarget->m_wSerialInField	==	0xffff	)
		return	bResult;

	cAbility	counterAbility;
	int			iChance;

	if  (_lpTarget->getActiveSwingArising(&counterAbility))
	{
		iChance	=	counterAbility.getActivateChance();

		if (random(100) < iChance)
		{
			_lpTarget->fireCounterSwingArising(&counterAbility);

			return	bResult;
		}
	}

	if (_lpTarget->isDeath()					)
		return	bResult;
	if (m_wSerialInField			==	0xffff	)
		return	bResult;
	if (_lpTarget->m_wSerialInField	==	0xffff	)
		return	bResult;

	if (bIsHit)
	{
		int	iDamageReactionSkillCount	=	0;

		if	(_lpTarget->m_wHillSkill	==	0xffff	||	_lpAbility->isCloseRangeDamageAttack()	==	FALSE)
			while(getActivateReaction(&counterAbility,eRSAT_STRIKE,iDamageReactionSkillCount++,_lpTarget))
			{
				iChance				=	counterAbility.getActivateChance();

				if	(iChance)
					if (random(100) >= iChance)
						continue;

				int	iHitCount	=	0;

				iHitCount		+=	_lpTarget->applySkillExtraEffects(this,&counterAbility);//	때릴때 적한테 적용
				iHitCount		+=	applySkillExtraEffects(this,&counterAbility);			//	때릴때 본인한테 적용
				
				if	(counterAbility.getSkill()->m_wType	==	eSKILL_TYPE_ACTIVITY_REACTION)
				{
					int	iSpentCP	=	counterAbility.getSpentCP();
					int	iGetCP		=	counterAbility.getGetCP();

					reduceCP(iSpentCP,counterAbility.m_wSkill);
					increaseCP(iGetCP);

					sendPlayerBasicInformation();
				}
			}

		if	(_lpAbility->isIgnoreTargetReaction()	==	FALSE)
		{
			iDamageReactionSkillCount	=	0;
			
			if	(_lpAbility->isCloseRangeDamageAttack())
				while(_lpTarget->getActivateReaction(&counterAbility,eRSAT_HIT_CLOSE_RANGE_ATTACK_COUNTER,iDamageReactionSkillCount++,this))
				{
					iChance				=	counterAbility.getActivateChance();

					if	(iChance)
						if (random(100) >= iChance)
							continue;

					int	iAttackRange	=	_lpTarget->getAttackRange(&counterAbility);

					if	(iAttackRange	==	0	||	_lpTarget->isTargetInAttackRange(this,iAttackRange))
						_lpTarget->fireCounterReactionSkill(this,&counterAbility);
				}

			iDamageReactionSkillCount		=	0;

			if	(_lpHitInfo->m_dwPhysicalDamage)
			{
				while(_lpTarget->getActivateReaction(&counterAbility,eRSAT_PHYSICAL_DAMAGED,iDamageReactionSkillCount++,this))
				{
					iChance				=	counterAbility.getActivateChance();

					if	(iChance)
						if (random(100) >= iChance)
							continue;

					_lpTarget->reduceCP(counterAbility.getSpentCP(),counterAbility.m_wSkill);

					BOOL	bIsSuccess	=	_lpTarget->applySkillExtraEffects(_lpTarget,&counterAbility);			//	이 스킬을 본인에게 적용?

					if	(bIsSuccess)
					{
						_lpTarget->addSendSkillEffect(&counterAbility,eSE_SKILL);
						_lpTarget->increaseCP(counterAbility.getGetCP());
					}

					_lpTarget->sendPlayerBasicInformation();
				}
				
				iDamageReactionSkillCount	=	0;

				while(_lpTarget->getActivateReaction(&counterAbility,eRSAT_HIT_PHYSICAL_DAMAGE_TIME_COUNTER,iDamageReactionSkillCount++,this))
				{
					iChance				=	counterAbility.getActivateChance();

					if	(iChance)
						if (random(100) >= iChance)
							continue;

					int	iAttackRange	=	_lpTarget->getAttackRange(&counterAbility);

					if	(iAttackRange	==	0	||	_lpTarget->isTargetInAttackRange(this,iAttackRange))
						_lpTarget->fireCounterReactionSkill(this,&counterAbility);
				}
			}
		}
	}
/*
	if (_lpAbility->isCloseRangeDamageAttack())
	{
		int	iCounterSkillCount=0;

		while(_lpTarget->getActiveCounterSkill(&counterAbility,iCounterSkillCount++))
		{
			iChance				=	counterAbility.getActivateChance();

			if (random(100) < iChance)
			{
				int	iAttackRange	=	counterAbility.getAttackRange(_lpTarget->getWeapon());

				if (_lpTarget->isTargetInAttackRange(this,iAttackRange))
					_lpTarget->fireCounterReactionSkill(this,&counterAbility);
			}
		}
	}
*/
	return	bResult;
}

void
cACTOR::operateDamageReturn(int _iDamage,cACTOR *_lpAttacker,BOOL _bIsReturnMissileDamage)
{
	int	iReturnDamageRate		=	m_returnDamageRate.getCorrectedValue();
	if(_bIsReturnMissileDamage)
		iReturnDamageRate		= m_wReturnMissileDamageValue/100;
	int	iReturnDamageByRootRate	=	m_returnDamageByRootRate.getCorrectedValue();

	int iReturnDamageRateIgnoreDefence		=	m_returnDamageRateIDEF.getCorrectedValue() * 100;
	if (iReturnDamageRate+iReturnDamageByRootRate + iReturnDamageRateIgnoreDefence	==	0	)
		return;
	if (_iDamage			<=	0	)
		return;
	if (m_iHP				<=	0	)
		return;
	if (_lpAttacker			==	NULL)
		return;
	if (_lpAttacker->isDeath()		)
		return;	

	if	(!_bIsReturnMissileDamage && !isTargetInAttackRange(_lpAttacker,dRETURN_DAMAGE_RANGE))
		return;

	if	(iReturnDamageRateIgnoreDefence)
	{
		m_lpField->sendHitThornDamage(m_wSerialInField,_lpAttacker->m_wSerialInField,iReturnDamageRateIgnoreDefence,m_iCP,TRUE);
		
		_lpAttacker->reduceHP(iReturnDamageRateIgnoreDefence,this);

		m_returnDamageRateIDEF.reset();
	}
	
	if	(iReturnDamageRate)
	{
		LONGLONG	llDamage		=	_iDamage;
		llDamage					=	llDamage*iReturnDamageRate/100;
		int			iAttackPower	=	(int)llDamage;
		int			iDefensivePower	=	_lpAttacker->getDefensivePower();
		
		LONGLONG	llReturnDamage	=	iAttackPower;
		LONGLONG	llDivedeValue	=	max(llReturnDamage+iDefensivePower,1);
		
		llReturnDamage				=	llReturnDamage*llReturnDamage/llDivedeValue;
		
		int	iReturnDamage			=	(int)llReturnDamage;
		
		if	(iReturnDamage)
		{
			m_lpField->sendHitThornDamage(m_wSerialInField,_lpAttacker->m_wSerialInField,iReturnDamage,m_iCP);
			
			_lpAttacker->reduceHP(iReturnDamage,this);
		}
	}

	if	(iReturnDamageByRootRate)
	{
		LONGLONG	llDamage		=	_iDamage;
		llDamage					=	llDamage*iReturnDamageByRootRate/100;
		int			iAttackPower	=	(int)llDamage;
		int			iDefensivePower	=	_lpAttacker->getDefensivePower();

		iAttackPower				=	GetRootDamage(iAttackPower);

		LONGLONG	llReturnDamage	=	iAttackPower;
		LONGLONG	llDivedeValue	=	max(llReturnDamage+iDefensivePower,1);

		llReturnDamage				=	llReturnDamage*llReturnDamage/llDivedeValue;

		int	iReturnDamage			=	(int)llReturnDamage;

		if	(iReturnDamage)
		{
			m_lpField->sendHitThornDamage(m_wSerialInField,_lpAttacker->m_wSerialInField,iReturnDamage,m_iCP);

			_lpAttacker->reduceHP(iReturnDamage,this);
		}
	}
}
void
cACTOR::strikeIcyStalagmite(cAbility *_lpAbility,int _iDamage,int _iPhysicalDamage,cACTOR *_lpAttacker)
{
	if	(_lpAbility && _lpAbility->isCloseRangeDamageAttack())	
		m_iHillHP	-=	_iPhysicalDamage;	//	근접 공격이면 얼음에만 대미지..
	else
	{
		reduceHP(_iDamage,_lpAttacker);	//	몸에 맞고..

		if (_lpAbility && _lpAbility->getHitRange()			)
			m_iHillHP	-=	_iPhysicalDamage;	//	범위 공격이면 얼음도 맞는다.
	}

	if	(m_iHillHP	<=	0)
		releaseIcyStalagmite(eRUIS_DESTROY);
}

void
cACTOR::operateHitNpcReaction(cACTOR *_lpAttacker,int _iReduceHP)
{
	if	(isPlayer())
		return;

	cNPC	*lpNpc	=	getNpc();

	if	(!lpNpc	||	_iReduceHP <= 0)
		return;

	int	iPartySerial	=	0xffff;

	if	(_lpAttacker)
	{
		iPartySerial	=	_lpAttacker->getPartySerial();

		cACTOR	*lpLord	=	_lpAttacker->getControlLord();

		if	(lpLord)
			iPartySerial=	lpLord->getPartySerial();
	}

	if	(!_lpAttacker	||	_lpAttacker->isDeath())
		lpNpc->addDamageBySolo(0,0xffff,_iReduceHP,m_lpField->m_iFrameCounter);
	else
	{
		if	(iPartySerial	!=	0xffff)
			lpNpc->addDamageByParty(_lpAttacker->m_level.m_sValue,iPartySerial,_lpAttacker->m_wSerialInField,_iReduceHP,m_lpField->m_iFrameCounter);
		else
			lpNpc->addDamageBySolo(_lpAttacker->m_level.m_sValue,_lpAttacker->m_wSerialInField,_iReduceHP,m_lpField->m_iFrameCounter);	//	파티 플레이가 아니다.
	}
}

//
//	타운트 스킬 발동
void
cACTOR::operateTauntSkill(cACTOR *_lpAttacker,int _iDamagePercent, int _iUpkeepTime)
{
	revenge(_lpAttacker);
	m_wTauntedToTargetSerial		=	_lpAttacker->m_wSerialInField;
	m_wTauntedToTargetDamagePercent	=	_iDamagePercent;	// 타운트건놈에게 대미지적용퍼센트..
	m_dwTauntedToTargetNameHashCode	=	_lpAttacker->m_dwNameHashCode;
	m_wTauntedToTargetTime			=	_iUpkeepTime;	//	초
}

//
//	펫 타운트 스킬 발동
void
cACTOR::operatePetTauntSkill(cACTOR *_lpAttacker,int _iUpkeepTime)
{
	DWORD	dwMinRangge	=	0x7fffffff;
	int		iPetIndex	=	0xffff;

	for (int i=0;i<dOWN_PET_COUNT;i++)
	{
		cACTOR	*lpActor	=	_lpAttacker->getPetActor(i);

		if (!lpActor || lpActor->isDeath())
			continue;

		int	iRange	=	GetOvalRange(lpActor->m_pos.x,lpActor->m_pos.y,m_pos.x,m_pos.y);

		if (iRange	<	(int)dwMinRangge)
		{
			iPetIndex	=	i;
			dwMinRangge	=	iRange;
		}
	}

	if (iPetIndex	==	0xffff)
		return;

	cACTOR	*lpPet	=	_lpAttacker->getPetActor(iPetIndex);

	revenge(lpPet);

	m_wTauntedToTargetTime	=	_iUpkeepTime;	//	초
}

//
//	복수 -o-
void
cACTOR::operateRevenge(cACTOR *_lpAttacker,cAbility *_lpAbility,BOOL _bIsForecedRevenge)
{
	if	(m_bIsGuildObject)
		return;

	if	(m_iGuildGuardianType	&&	m_iGuildGuardianType	<	eGUILD_GUARDIAN_HOLLOW_KNIGHT)
		return;

	if	(m_wProcessPatternLevel	!=	0xffff	||	m_bf1IsBlockToRevenge)
		return;
	if	(m_wJob	==	dJOB_MONSTER_SAGE_MASTER)
	{
		if	(m_wInvincibleTime)
			return;
	}
	if	(_bIsForecedRevenge	==	FALSE	&&	_lpAbility		&&	_lpAbility->getSkill()->m_bf1NotRevenge)
		return;
	if	(m_wConfuseTime	!=	0						)
		return;	//	혼란 상태다.
	if	(!_lpAttacker	||	_lpAttacker->isDeath()	)
		return;	//	공격자가 죽거나 없다.

	if	(m_wUseSpecialAbilityStep	!=	ePUSS_NONE)
		return;

	if	(m_wTauntedToTargetTime	==	0)
	{
		cNPC	*lpNpc	=	getNpc();

		if	(lpNpc)
		{
			int	iSerial	=	lpNpc->getSlugger();

			if	(m_lpTarget	==	NULL	||	iSerial	==	0xffff	||	iSerial	==	_lpAttacker->m_wSerialInField)
				revenge(_lpAttacker);	//	슬러거가 없다.
		}
		else
			revenge(_lpAttacker);
		
		return;
	}

	if	(m_lpTarget	==	NULL)
		revenge(_lpAttacker);	//	타겟이 없다.
}

//
//	* 무기 대미지 = (각 무기의 기본 대미지 + 격투 반지의 증가 대미지) * (% 증가 대미지) (+ 증가 대미지는 포함하지 않음)
int
cACTOR::getWeaponDamage(int _iTargetLevel,BOOL _bIsPVP,BOOL _bIsBossBattle,CHitInfo *_lpHitInfo,BOOL _bIsWantMinDamage)
{
	int	iLevelCorrectDamagePercentage	=	0;
	int	iWeaponMinDamage	=	m_iWeaponMinDamage+m_weaponPower.getCorrectValue();
	int	iWeaponMaxDamage	=	m_iWeaponMaxDamage+m_weaponPower.getCorrectValue();

	int	iCorrectWeaponDamagePercentage	=	100+m_weaponPower.getCorrectPercentageValue();
	iCorrectWeaponDamagePercentage		=	max(0,iCorrectWeaponDamagePercentage);

	iWeaponMinDamage		=	iWeaponMinDamage*iCorrectWeaponDamagePercentage/100;
	iWeaponMaxDamage		=	iWeaponMaxDamage*iCorrectWeaponDamagePercentage/100;

	if	(_bIsPVP)
		iLevelCorrectDamagePercentage	=	(m_iLevel-_iTargetLevel)/25*30;

	if	(_bIsBossBattle)
		iLevelCorrectDamagePercentage	=	(_iTargetLevel-m_iLevel)/25*30;

	iWeaponMinDamage		+=	iWeaponMinDamage*m_attackPower.getCorrectPercentageValue()/100;
	iWeaponMaxDamage		+=	iWeaponMaxDamage*m_attackPower.getCorrectPercentageValue()/100;

	int	iDamageGab			=	iWeaponMaxDamage-iWeaponMinDamage,iDamage;

	if	(m_wMaintainMaxWeponDamageTime)
		iDamage	=	iWeaponMaxDamage;	//	최대 데미지 유지-o-
	else
	if	(_bIsWantMinDamage)
		iDamage	=	iWeaponMinDamage;	//	최소 데미지 유지-o-
	else
	{
		iDamage	=	iWeaponMinDamage+largeRandom(iDamageGab+1);

//무기 대미지 보너스 굴림							
//● (공격자 운 / 1000)의 확률로 대미지 보너스 굴림을 하여 먼저 굴린 것과 나중에 굴린 것 중 높은 값을 사용							
//● 보너스 굴림을 성공한 경우는 한 번 더 보너스 굴림을 할 수 있는 기회 제공							
//● 보너스 굴림은 최대 3번으로 제한							
//● 보너스 굴림으로 대미지를 입힌 경우에는 수치 앞에 굴린 수 만큼의 클로버 아이콘 표시		
//● JBC 무기 보너스 데미지 굴림 결투장에서는 1번만 적용 한다.	08-11-25

	int iLuckDiceCount = 3;

	if (g_bIsDuelServer)
	{
		if(g_iSeasonVariable >= dSEASON_VARIABLE_PVP_REFORM)
			iLuckDiceCount = 1;
	}
		
		for (int i=0;i<iLuckDiceCount;i++)
		{
			if	(getLuck()	<=	random(1000))
				break;

			int	iLuckyDamage=	iWeaponMinDamage+largeRandom(iDamageGab+1);

			if	(iLuckyDamage	>	iDamage)
			{
				iDamage		=	iLuckyDamage;

				if	(_lpHitInfo)
					_lpHitInfo->setLuckyDamage();
			}
		}
	}

	if	(iLevelCorrectDamagePercentage)
	{
		int	iBaseValue	=	100;
		int	iCorrectedDamage;

		if	(_bIsBossBattle)
			iBaseValue	=	100;

		iCorrectedDamage	=	iDamage*(iBaseValue+iLevelCorrectDamagePercentage)/100;

		iCorrectedDamage	=	max(iDamage*40/100,iCorrectedDamage);
		iCorrectedDamage	=	min(iDamage*180/100,iCorrectedDamage);
		iDamage				=	iCorrectedDamage;
	//	단, 최종 계산 결과가 레벨 보정 이전 대미지의 40% 이하 이거나
	//	180% 이상일 경우 각각 40% 와 180%로 합니다.
	}

	if (m_wBreakedWeaponTime		)
		iDamage	/=	dBREAK_EQUIPMENT_FACTOR;

	return	iDamage;
}

//
//누가 데미지를 입혔다.
void
cACTOR::attackToShakle(cACTOR *_lpAttacker,int _iPhysicalDamage,int _iMagicDamage,cAbility *_lpAbility)
{
	m_iShakleHP			-=	(_iPhysicalDamage+_iMagicDamage);

	if	(m_bf1IsSyncDamageShakle)
	{
		CHitInfo	hitInfo;

		_iPhysicalDamage		=	GetP2PCorrectDamage(_iPhysicalDamage);
		_iMagicDamage			=	GetP2PCorrectDamage(_iMagicDamage);

		if	(m_bf1IsCriticalShakle)
			_iPhysicalDamage	*=	2;

		hitInfo.reset(TRUE);

		if	(_iPhysicalDamage+_iMagicDamage)
		{
			hitInfo.setPhysicalDamage();
			hitInfo.setHit();
		}

		reduceHP(_iPhysicalDamage+_iMagicDamage);

		if	(_lpAbility)
			addSendHitDamage(NULL,_iPhysicalDamage,_iMagicDamage,hitInfo.m_dwResultField,_lpAbility->m_wSkill);
	}

	if	(m_iShakleHP	<=	0)
		releaseShakle();
}

void
cACTOR::crushMagicBox(cACTOR *_lpAttacker)
{
	BOOL	bIsPVP	=	FALSE;
	BOOL	bIsNEP	=	isNEPlayer(_lpAttacker);

	if	(m_lpField->isPVP(_lpAttacker,this))
		bIsPVP		=	TRUE;
	
	int	iKillChance	=	m_wKillChanceByMagicBox;
	int	iDamage		=	m_iHP;

	if	(bIsPVP)
	{
		if	(bIsNEP)	//	천적이다.
		{
			iDamage		=	m_iHP*dPVP_INSTANCE_KILL_FACTOR_NE/100;
			iKillChance	=	iKillChance*dPVP_CORRECT_INSTANCE_KILL_CAHNCE_NE/100;
		}
		else
		{
			iDamage		=	m_iHP*dPVP_INSTANCE_KILL_FACTOR/100;
			iKillChance	=	iKillChance*dPVP_CORRECT_INSTANCE_KILL_CAHNCE/100;
		}
	}

	int	iResistanceValue	=	max(m_bInstanceKillResistance,getCurseResistance());

	if	(bIsPVP)
		iResistanceValue	=	m_bInstanceKillResistance+getCurseResistance();

	iKillChance				=	iKillChance*(100-iResistanceValue)/100;

	if	(random(100) < iKillChance	)
	{
		reduceHP(iDamage,_lpAttacker);
		m_lpField->addSendEtcWork(m_wSerialInField,eEW_CRUSH_MAGIC_BOX,m_wSerialInField,TRUE);
	}
	else
	{
		m_iHP		=	getMaxHP();

		m_lpField->addSendEtcWork(m_wSerialInField,eEW_CRUSH_MAGIC_BOX,m_wSerialInField,FALSE);

		operateRevenge(_lpAttacker,NULL);
	}

	m_wKillChanceByMagicBox	=	0;
	m_wInMagicBoxTime		=	0;
}

//
//누가 데미지를 입혔다.
BOOL
cACTOR::damagedBy(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iPhysicalDamage,int _iMagicDamage,BOOL _bIsApplyReturnDamage,BOOL _bIsApplyAdditionalEffect,int _iX,int _iY,CHitInfo *_lpHitInfo,BOOL _bIsCounter)
{
	m_wUnderBattleTime	=	5;

	if	(_lpAttacker)
	{
		_lpAttacker->m_wUnderBattleTime	=	5;

		if	(isAttackToShakleActor(_lpAttacker))
		{
			attackToShakle(_lpAttacker,_iPhysicalDamage,_iMagicDamage,_lpAbility);

		
			return	TRUE;
		}

		if	(m_wInMagicBoxTime)
		{
			if	(_lpAttacker->m_wSerialInField	==	m_wMagicBoxCasterSerial)
				crushMagicBox(_lpAttacker);

			return	TRUE;
		}
	}
		
	BOOL	bResult		=	FALSE;
	int		iDamage		=	_iPhysicalDamage+_iMagicDamage,iReduceHP;
	CSkill	*lpSkill	=	NULL;

	if	(m_bf1IsEvilSpritOfChristmas && _lpAttacker	&&	_lpAttacker->isPlayer() && iDamage)
		dropItemForEventMob();

	if	(_bIsApplyAdditionalEffect&& _lpAttacker	&&	_lpAttacker->isPlayer() && iDamage)
	{
		if	(m_bf1IsEventSummonMonster && m_dropInfoOfSummonEventMonster.m_wDropItem  != 0xffff && random(100) < m_dropInfoOfSummonEventMonster.m_wDropChance)
		{
			if	(m_dropInfoOfSummonEventMonster.m_wKind == eSEM_ATTACK_INVENTORY_DROP)
			{				
				for(int i=0;i<m_dropInfoOfSummonEventMonster.m_wDropCount;++i)
				{	
					if	(m_dropInfoOfSummonEventMonster.m_aiOption[0] !=0xffff)
						_lpAttacker->addItemWithOption(m_dropInfoOfSummonEventMonster.m_wDropItem,1, m_dropInfoOfSummonEventMonster.m_aiOption,eGET_GOLD_BY_BEAT_MONSTER_EVENT);
					else
						_lpAttacker->addItemWithOption(m_dropInfoOfSummonEventMonster.m_wDropItem,1,NULL,eGET_GOLD_BY_BEAT_MONSTER_EVENT);
				}
			}
		}
	}

	if	(_lpAbility)
		lpSkill	=	_lpAbility->getSkill();

	if	(m_iHP	<=	0	&& isDeath())
		return FALSE;	//	이미 시체

	if	(iDamage	<	0)
		iDamage	=	0;

	if	(_lpHitInfo && _lpHitInfo->isHitTheOtherSelf())
	{
		if	(m_wTheOtherSelfCount)
			m_wTheOtherSelfCount--;

		addSendSimpleInfo();

		return	TRUE;
	}

//	미러 타워 처리
	if	(m_wMirrorTowerCaster != 0xffff && _iPhysicalDamage != -1 && _iMagicDamage && _lpAttacker && _lpAbility)
		_iMagicDamage	=	m_lpField->operateMirrorTower(_lpAttacker,this,_lpAbility,_iMagicDamage);
	
	if	(_lpHitInfo && _lpHitInfo->isReflection())
	{
		cAbility	ability;
		CHitInfo	hitInfo;

		hitInfo.reset();
		hitInfo.setHit();

		cACTOR		*lpLord		=	this;
		cACTOR		*lpBeast	=	this;

		if	(isSummonBeast())
			lpLord	=	getTamer();

		if	(lpLord	&&	lpLord->getActivateReaction(&ability,eRSAT_DAMAGE_MAGIC_SKILL_SB,0,_lpAttacker))
		{
			CSkill	*lpSkill	=	ability.getSkill();

			if	(isSummonBeast()==	FALSE)
				lpBeast			=	getSummonBeastByType(lpSkill->m_bf2OperateSummonBeast);

			if	(lpBeast && lpBeast->m_wSummonBeastGrade+1	>=	(WORD)lpSkill->m_bf2RequireSummonBeastGrade)
			{
				cACTOR	*lpTarget	=	lpBeast;
				cACTOR	*lpAttacker	=	_lpAttacker;

				if	(isSummonBeast() == FALSE && random(100) <=	ability.getActivateChance())
				{
					lpTarget	=	_lpAttacker;
					lpAttacker	=	lpBeast;

					lpTarget->getDamage(lpAttacker,_lpAbility,0,0,&hitInfo,TRUE,TRUE,FALSE);
					lpBeast->addSendSkillEffect(&ability,eSE_HIT);
					lpLord->addSendSkillEffect(&ability,eSE_HIT);

					reduceCP(ability.getSpentCP(),ability.m_wSkill);
					increaseCP(ability.getGetCP());
					sendPlayerBasicInformation();
				}
				else
					lpTarget->getDamage(lpAttacker,_lpAbility,0,0,&hitInfo,TRUE,TRUE,FALSE);

				if	(hitInfo.isReflection())
					hitInfo.m_dwResultField	-=	dATTACK_RESULT_REFLECTION;

				lpTarget->damagedBy(lpAttacker,_lpAbility,hitInfo.m_dwPhysicalDamage,hitInfo.m_dwMagicDamage,FALSE,TRUE,_iX,_iY);
				lpTarget->addSendHitDamage(lpAttacker,hitInfo.m_dwPhysicalDamage,hitInfo.m_dwMagicDamage,hitInfo.m_dwResultField,_lpAbility->m_wSkill);

				return	TRUE;
			}
		}

		_lpHitInfo->m_dwResultField	-=	dATTACK_RESULT_REFLECTION;

		addSendHitDamage(_lpAttacker,_lpHitInfo->m_dwPhysicalDamage,_lpHitInfo->m_dwMagicDamage,_lpHitInfo->m_dwResultField,_lpAbility->m_wSkill);
		reduceHP(_lpHitInfo->getDamage(),_lpAttacker);

		return	TRUE;
	}

	iDamage		=	_iPhysicalDamage+_iMagicDamage;
	iReduceHP	=	min(iDamage,m_iHP);

	if	(_lpAttacker && !_lpAttacker->isDeath())
	{
		_lpAttacker->setRestraintTimeByBattle();	//	공격자와

		if	(_lpAttacker->m_wSerialInField ==m_wSerialInField)	//	자학 할경우 자살은 안된다.
			if	(iDamage	>=	m_iHP)	
				iDamage	=	m_iHP-1;

		_lpAttacker->m_iLastAttackPoint	=	iDamage;

		if	(_lpAbility)
		{
			CSkill	*lpSkill	=	_lpAbility->getSkill();

			if	(lpSkill->m_wIsMagicDamageBasedLastHitDamage)
				_lpAttacker->m_iLastAttackPoint	=	0;
		}
	}

	if	(isMonster())	//	몬스터라묜...
		setRestraintTimeByBattle();

//	맞으면 깨어난다. -o- 가위에 의한 대미지는 입지 않는다. 이점 주의 할 것
	m_wTerribleDreamTime	=	0;

	if	(m_wSleepTime	<	0x8000)
		m_wSleepTime		=	0;
	
	releaseDance();		//	맞으면 춤 그만 춘다.

	if	(iDamage)
		bResult		=	TRUE;

	if	(m_iHP > 0 && _bIsApplyAdditionalEffect && _lpAttacker && _lpAbility)
	{
		if	(m_wHillSkill	==	0xffff	||	_lpAbility->isCloseRangeDamageAttack()	==	FALSE)
		{
			int	iChance	=	_lpAbility->getActivateChance();

			if	(iDamage	==	0)
				if	(!lpSkill	||	lpSkill->m_wDamageAttribute	!=	0)
					iChance	=	-1;

			if	(iChance	!=	-1)
				if	(iChance == 0 || random(100 ) < iChance || _bIsCounter)
					bResult		+=	applySkillExtraEffects(_lpAttacker,_lpAbility,_iPhysicalDamage,_iX,_iY);	//	스킬 부가 효과 적용
		}
	}

	if	(m_wHillSkill	!=	0xffff)
		strikeIcyStalagmite(_lpAbility,iDamage,_iPhysicalDamage,_lpAttacker);
	else
	{
		reduceHP(iDamage,_lpAttacker);

		if	(_lpHitInfo	&&	_lpHitInfo->isInstanceKill()	&&	m_iHP	<=	0)
			m_bf1IsInstanceKill		=	TRUE;

		if(m_wReturnMissileDamageTime && _lpAbility->isMissileSkill())			// 미사일대미지리턴이면!
		{
			operateDamageReturn(iDamage ,_lpAttacker, TRUE);
		}
		else if(_bIsApplyReturnDamage)
		{
			operateDamageReturn(_iPhysicalDamage,_lpAttacker);
		}
			
	}

	if	(m_iHP	<=	0)
		return bResult;

	operateRevenge(_lpAttacker,_lpAbility);

	for (int i=0;bResult && i<dACTIVE_REACTION_COUNT;i++)
	{
		cAbility	ability;

		if	(!getActivatePassiveSkill(&ability,eRSAT_DAMAGED,i,FALSE,_lpAttacker))
			break;

		int	iChance		=	ability.getActivateChance();

		if	(iChance)
			if (random(100) >= iChance)
				continue;

		if	(isSitdown()	||	m_wBiteTarget	!=	0xffff	||	isSealdAction()	||	m_wStunTime+m_wFreezeTime+m_wStoneTime+m_wSleepTime+m_wCaughtTime)
			continue;
			
		reduceCP(ability.getSpentCP(),ability.m_wSkill);	//CP 감소

		if	(applySkillExtraEffects(this,&ability,0))
		{
			addSendSkillEffect(&ability,eSE_CAST);
			increaseCP(ability.getGetCP());	//CP 감소
		}

		sendPlayerBasicInformation();
	}

	return	bResult;
}

void
cACTOR::getContinuoueAttackDamage(cAbility *_lpAbility,cACTOR *_lpTarget,int _iShotCount,int *_lpiPhisycalDamage,int *_lpiMagicalDamage,int *_lpiMissCount,int *_lpiCriticalCount,int *_lpiDoubleCriticalCount,
								int *_lpiMinPhysicalDamage,int *_lpiMinMagicDamage,int *_lpiBlockingCount,int *_lpiInstanceKillCount,int *_lpiNoActionBlockCount,int *_lpiDancingBlockCount, int* _lpiHardBlowCount)
{
	int		iMissCount			=	0,i;
	DWORD	dwPhysicalDamage	=	0,dwMagicDamage	=	0,iCriticalCount	=	0,iDoubleCriticalCount=0;
	int		iMinPhysicalDamage	=	0x7fffffff,iMinMagicDamage	=	0x7fffffff;
	int		iBlockingCount		=	0;
	int		iInstanceKillCount	=	0;
	int		iNoActionBlockCount	=	0;
	int		iDancingBlockCount	=	0;
	int		iHardBlowCount		=	0;
	for (i=0;i<_iShotCount;i++)
	{
		CHitInfo	hitInfo,*lpInfo	=	&m_attackInfo.m_aInfo[i];

		lpInfo->reset(TRUE);

		_lpTarget->getDamage(this,_lpAbility,0,0,&hitInfo,TRUE,TRUE,TRUE,_iShotCount,i);

		if	(hitInfo.isInstanceKill())
			iInstanceKillCount++;

		if	(hitInfo.isMiss()	||	hitInfo.isDodge())
			iMissCount++;
		else
		{
			if	(hitInfo.isBlock())
				iBlockingCount++;
			else
			if	(hitInfo.isNoActionBlock())
				iNoActionBlockCount++;
			else
			if	( hitInfo.isDancingBlockerBlock())
				iDancingBlockCount++;

			lpInfo->setHit();

			lpInfo->m_dwPhysicalDamage	=	hitInfo.m_dwPhysicalDamage;
			lpInfo->m_dwMagicDamage		=	hitInfo.m_dwMagicDamage;
			dwPhysicalDamage			+=	lpInfo->m_dwPhysicalDamage;
			dwMagicDamage				+=	lpInfo->m_dwMagicDamage;

			iMinPhysicalDamage			=	min(iMinPhysicalDamage,(int)lpInfo->m_dwPhysicalDamage);
			iMinMagicDamage				=	min(iMinMagicDamage,(int)lpInfo->m_dwMagicDamage);

			if	(hitInfo.isCritical())
				iCriticalCount++;
			if	(hitInfo.isLuckyDoubleCritical())
				iDoubleCriticalCount++;
			if	(hitInfo.isHardBlow())
				++iHardBlowCount;
		}
	}

	*_lpiMissCount			=	iMissCount;
	*_lpiPhisycalDamage		=	dwPhysicalDamage;
	*_lpiMagicalDamage		=	dwMagicDamage;
	*_lpiCriticalCount		=	iCriticalCount;
	*_lpiDoubleCriticalCount=	iDoubleCriticalCount;
	*_lpiMinPhysicalDamage	=	iMinPhysicalDamage/100;
	*_lpiMinMagicDamage		=	iMinMagicDamage/100;
	*_lpiBlockingCount		=	iBlockingCount;
	*_lpiInstanceKillCount	=	iInstanceKillCount;
	*_lpiNoActionBlockCount	=	iNoActionBlockCount;
	*_lpiDancingBlockCount	=	iDancingBlockCount;
	*_lpiHardBlowCount		=	iHardBlowCount;
}