#include <math.h>
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include "rebirth_define.H"

//
//	적에게 회오리-_-형 공격의 타격을 입힌다.
BOOL
cACTOR::strikeWhirlWind(cAbility *_lpAbility)
{
	return	m_lpField->strikeWhirlWind((cACTOR*)this,_lpAbility);
}

//
//	적에게 직선형연쇄폭발 먹임
BOOL
cACTOR::strikeLinerChainExplosion(int _iX,int _iY,cAbility *_lpAbility,WORD *_lpDamagedActorList)
{
	return	m_lpField->strikeLinerChainExplosion((cACTOR*)this,_iX,_iY,_lpAbility,_lpDamagedActorList);
}

//
//	적에게 피어싱 미사일형 공격 타격을 입힌다.
BOOL
cACTOR::strikePiercingMissile(cAbility *_lpAbility,cACTOR *_lpTarget,WORD *_lpDamagedActor)
{
	return	m_lpField->strikePiercingMissile((cACTOR*)this,_lpAbility,_lpTarget,_lpDamagedActor);
}

//
//레이져 광선!! -o-!!
BOOL
cACTOR::strikeLaser(cAbility *_lpAbility,cPOINT *_lpX1Y1,cPOINT *_lpX2Y1,cPOINT *_lpX1Y2,cPOINT *_lpX2Y2)
{
	return	m_lpField->strikeLaser((cACTOR*)this,_lpAbility,_lpX1Y1,_lpX2Y1,_lpX1Y2,_lpX2Y2);
}

void
cACTOR::setCoolTime(cAbility *_lpUseAbility)
{
	int	iCoolTimeIndex	=	_lpUseAbility->getCoolTimeIndex();

	if	(iCoolTimeIndex	>=	c_iCoolTimeSkillCount)
		return;

	m_awSkillCoolTime[iCoolTimeIndex]	=	_lpUseAbility->getCoolTime();

	if(m_fightEnergy.isDecreaseCoolTime())
	{
		int iCoolTime = m_awSkillCoolTime[iCoolTimeIndex] * m_fightEnergy.m_iValue;
		if(iCoolTime % 100 >= 50)
			iCoolTime	+= 50;
		m_awSkillCoolTime[iCoolTimeIndex] = max(0,m_awSkillCoolTime[iCoolTimeIndex] - (iCoolTime/100.0));
	}
	sendEtcWork(eEW_SET_COOL_TIME,iCoolTimeIndex,m_awSkillCoolTime[iCoolTimeIndex]);
}

int
cACTOR::getCoolTime(cAbility *_lpUseAbility)
{
	CSkill	*lpSkill	=	_lpUseAbility->getSkill();

	if	(!lpSkill)
		return	0;

	if	(lpSkill->m_wCoolTimeIndex	>=	c_iCoolTimeSkillCount)
		return	0;
	
	int	iHighCoolTime	=	lpSkill->m_wCoolTime+(short)lpSkill->m_wCoolTimePerLevel*1;
	int	iLowCoolTime	=	lpSkill->m_wCoolTime+(short)lpSkill->m_wCoolTimePerLevel*100;
	int	iMaxCoolTime	=	max(iHighCoolTime,iLowCoolTime);

	iMaxCoolTime		=	max(iMaxCoolTime,lpSkill->m_wMinimumCoolTime);

	if	(m_awSkillCoolTime[lpSkill->m_wCoolTimeIndex]	>	iMaxCoolTime/100)
		m_awSkillCoolTime[lpSkill->m_wCoolTimeIndex]	=	0;

	return	m_awSkillCoolTime[lpSkill->m_wCoolTimeIndex];
}

BOOL
cACTOR::checkStatusForUseSkill(CSkill::uCheckStatus	*_lpStatus)
{
	if	(_lpStatus->m_dwValue1)
	{
		if	(_lpStatus->m_bf1IsPoison)
		{
			if	(m_wPoisonTime	==	0)
				return	FALSE;
		}
		if	(_lpStatus->m_bf1IsContract)
		{
			if	(m_wContractTime	==	0)
				return	FALSE;
		}
		if	(_lpStatus->m_bf1IsMinusCP)
		{
			if	(m_iCP	>=	0)
				return	FALSE;
		}
	}

	return	TRUE;
}

BOOL
cACTOR::isUseSkillState(CSkill* _lpSkill)
{

	if(m_wIgnoreNeedStateTime)
		return TRUE;
	switch(_lpSkill->m_bf6NeedState)
	{
	case dNEED_STATE_EMPTY :

		return TRUE;

	case eNEED_STATE_CONSENSUS :
		
		if(m_wConsensusTime)
			return TRUE;
		break;
	case eNEED_STATE_FIGHTING_SPIRIT :
		
		if(m_wFightingSpiritTime)
			return TRUE;
		break;		
	}
	
	return FALSE;
}

void
cACTOR::cancelNeedState(CSkill* _lpSkill)
{	// 필요상태해제
	if(!_lpSkill->m_bf1IsCancelNeedState)
		return;
	switch(_lpSkill->m_bf6NeedState)
	{
	case 	dNEED_STATE_EMPTY :
		break;
	case eNEED_STATE_CONSENSUS :
					
		if(m_wConsensusTime)
		{
				m_wConsensusTime = 0;
				sendEtcWork(eEW_CHANGE_CONSENSUS, 0);
		}
		break;
	case eNEED_STATE_FIGHTING_SPIRIT :
					
		if(m_wFightingSpiritTime)
		{
				m_wFightingSpiritTime = 0;
				sendEtcWork(eEW_CHANGE_FIGHTING_SPIRIT, 0);
		}
		break;		
	}

}

//
//	사용가능한 스킬이냐?

int
cACTOR::isUseAbleSkill(cAbility *_lpUseAbility)
{
	BOOL	isExistAbility	=	FALSE;
	int		iCP				=	m_iCP;

	if	(_lpUseAbility->m_wSkill	==	0xffff		)
		return	eAR_NOT_LEARNED_SKILL;
	if	(_lpUseAbility->m_wLevel	<=	0			)
		return	eAR_NOT_LEARNED_SKILL;

	CSkill	*lpSkill	=	_lpUseAbility->getSkill();

	if	(!lpSkill	)
		return	eAR_NOT_LEARNED_SKILL;
	if(!isUseSkillState(lpSkill))		// 스킬이 사용가능한 상태인가..
		return eAR_FAILED;
	if(m_wFleeTime)
		return eAR_FAILED;
	if(m_wRushTime)
		return eAR_FAILED;
	if(m_wBindingWordsTime)
	{	// 언어구속시간이면...
		if(lpSkill->m_wDifficultyLevel >= m_wBindingWordsOverLimitLevel)
			return eAR_FAILED;
	}

	if	(!isPlayer())
		return	eAR_OK;
	if(isPlayer() && m_lpField->m_bIsNotUseSkillBeforeTrans)
	{
		if(!m_wTransformationTime)
			return eAR_FAILED;
	}
	if(m_wTransToWeaponTime)
	{
		if(!lpSkill->bf1IsAbleSkillWhenTransToWeapon)
			return eAR_FAILED;
	}
	if	(!checkStatusForUseSkill(&lpSkill->m_checkCasterStatus))
		return	eAR_CAN_NOT_APT_SKILL_TO_THIS_STATUS;

	if	(getCoolTime(_lpUseAbility))
	{
		sendEtcWork(eEW_SET_COOL_TIME,lpSkill->m_wCoolTimeIndex,m_awSkillCoolTime[lpSkill->m_wCoolTimeIndex]);

		return	eAR_IS_COOL_TIME;
	}

	if	(m_wConfuseTime)
		if	(lpSkill->m_wDifficultyLevel	>	1)
			return	eAR_CAN_NOT_USE_HIGH_LEVEL_SKILL_BY_CONFUSE;

	if	(m_wBerserkTime)
	{
		if	(lpSkill->m_wDifficultyLevel	>	2)
			return	eAR_CAN_NOT_USE_HIGH_LEVEL_SKILL_BY_BERSERK;

		if	(!lpSkill->isAgressiveSkill())
			return	eAR_CAN_NOT_USE_HIGH_LEVEL_SKILL_BY_BERSERK;
	}

	if	(m_wHillSkill			!=	0xffff		)
	{
		if	(_lpUseAbility->isCloseRangeDamageAttack()			)
			return	eAR_CAN_NOT_CLOSE_ATTACK_ON_HILL;	//	언덕 위에서는 근접 공격 못해요
		if	(lpSkill->m_wType	==	eSKILL_TYPE_ICY_STALAGMITE	)
			iCP	=	1000000;
	}

	
	if	(!m_wTransformationTime && !lpSkill->isEnableJob(m_wJob))
		return	eAR_FAILED;
	
	if	(lpSkill->m_wType==	eSKILL_TYPE_SHIMMERING_SHIELD)
		if	(isShimmeringShieldCaster())	
			return	eAR_OK;
		
	BOOL	isEquipWeapon=FALSE,isEquipOtherWeapon=FALSE,isEquipShield=FALSE;
	int		iBulletCount	=	0;

	cItem	*lpWeapon		=	getWeapon();
	cItem	*lpShield		=	getShield();
	cItem	*lpBullet		=	getBullet();
	cItem	*lpOtherWeapon	=	getOtherJobWeapon();

	if	(lpOtherWeapon && isUseAbleItem(lpOtherWeapon,TRUE))
		isEquipOtherWeapon=TRUE;
	if	(lpWeapon && isUseAbleItem(lpWeapon))
		isEquipWeapon=TRUE;
	if	(lpShield && isUseAbleItem(lpShield))
		isEquipShield=TRUE;
	if	(lpBullet && isUseAbleItem(lpBullet))
		if (getBullet()->m_bCount	>	0)
		{
			iBulletCount	=	getBullet()->m_bCount;

			if (getBullet()->isInfinityBullet())
				iBulletCount	=	255;
		}

	if	(m_wIsHideShield || m_wCastBlocker	!=	0xffff	)
		isEquipShield=FALSE;


	if(m_wTransformationTime)
	{
		isEquipOtherWeapon = TRUE;
		isEquipWeapon	=	TRUE;
		isEquipShield	=	FALSE;
		iBulletCount =0;
	}
	if	(lpSkill->m_wType==	eSKILL_TYPE_COMPLETE_PROTECTION)
	{
		if	(isActiveCompleteProtection())
		{
			return	eAR_OK;
		}
		if(!isEquipShield || isShimmeringShieldCaster())
			return eAR_REQUIRE_SHIELD;
	}

	if	(_lpUseAbility->isRequirePotion())
	{
		if	(!getFirstPotion(_lpUseAbility))
			return	eAR_REQUIRE_POTION;
	}
	if	(_lpUseAbility->isRequireDrug())
	{
		if	(!getFirstDrug())
			return	eAR_REQUIRE_DURG;
	}
	if	(_lpUseAbility->isRequireFlower())
	{
		if	(!getFirstFlower())
			return	eAR_REQUIRE_FLOWER;
	}
	if	(_lpUseAbility->isRequireCandy())
	{
		if	(!getFirstCandy())
			return	eAR_REQUIRE_CANDY;
	}
	if(m_isMagicBullet)
		iBulletCount	=	255;
	int iResult = _lpUseAbility->isUseAble(iCP,isEquipWeapon,isEquipOtherWeapon,isEquipShield,iBulletCount,getPetCount(),getSummonBeastCount());
	if(iResult == eAR_LOW_CP && m_wMadnessTime)
	{		// 광란상태.
		iResult	= eAR_OK;
	}
	return	iResult;
}



void
cACTOR::operateAttackDoppelganger()
{

	if	(g_dwCurrentTime	<	m_dwActionTimeAttackDoppelganger)
		return;

	m_dwActionTimeAttackDoppelganger	=	 g_dwCurrentTime+m_wActionPerionAttackDoppelganger - (g_dwCurrentTime- m_dwActionTimeAttackDoppelganger);

	int	iDamage = m_iAttackDoppelgangerDamage;
	
	iDamage		=	max(iDamage,1);
	
	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iDamage	=	(DWORD)llDamage;
	}
	cACTOR* lpAttackDoppelgangerActor = m_lpField->getActor(m_wAttackDoppelgangerCaster);
	
	if(lpAttackDoppelgangerActor && lpAttackDoppelgangerActor->m_dwNameHashCode == m_dwAttackDoppelgangerCasterNameHashCode)
	{
		reduceHP(iDamage,lpAttackDoppelgangerActor);
	}
	else
	{
		reduceHP(iDamage,NULL);
	}
	--m_wAttackDoppelgangerCount;
	operateHitNpcReaction(lpAttackDoppelgangerActor, iDamage);	
	return;

}


BOOL
cACTOR::isCastAbleTarget(cACTOR *_lpTarget,cAbility *_lpAbility)
{
	if	(_lpAbility	==	NULL)
		return	FALSE;

	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(!lpSkill)
		return	FALSE;
	if	(isNpc())
		return	FALSE;

	int		iCheckLevel	=	getLevel()+_lpAbility->m_wLevel+getCorrectSkillLevel(_lpAbility);
	int		iLimitLevel	=	0;

	if	(!_lpTarget->checkStatusForUseSkill(&lpSkill->m_checkTargetStatus))
		return	FALSE;

	if	(lpSkill->m_sTargetLevelLimitType1	!=	0x7fff)
		iLimitLevel		=	iCheckLevel+lpSkill->m_sTargetLevelLimitType1;
	if	(lpSkill->m_sHumanTargetLevelLimitType1	!=	0x7fff && isHuman())
		iLimitLevel		=	iCheckLevel+lpSkill->m_sHumanTargetLevelLimitType1;
	if	(lpSkill->m_sUndeadTargetLevelLimitType1	!=	0x7fff && isUndead())
		iLimitLevel		=	iCheckLevel+lpSkill->m_sUndeadTargetLevelLimitType1;
	if	(lpSkill->m_sAnimalTargetLevelLimitType1	!=	0x7fff && isAnimal())
		iLimitLevel		=	iCheckLevel+lpSkill->m_sAnimalTargetLevelLimitType1;
	if	(lpSkill->m_sHolyBeastTargetLevelLimitType1	!=	0x7fff && isHolyBeast())
		iLimitLevel		=	iCheckLevel+lpSkill->m_sHolyBeastTargetLevelLimitType1;
	if	(lpSkill->m_sDemonTargetLevelLimitType1	!=	0x7fff && isDemon())
		iLimitLevel		=	iCheckLevel+lpSkill->m_sDemonTargetLevelLimitType1;

	if	(iLimitLevel	&&	_lpTarget->getLevel()	>	max(iLimitLevel,1))
		return	FALSE;

	if	(lpSkill->isCastOnDeathPlayerSkill())
	{
		if	(isOwnForce(_lpTarget) && _lpTarget->isDeath())
			return	TRUE;
	}

	if	(lpSkill->isCastOnDeathPetSkill())
	{
		if	(isOwnForce(_lpTarget) && _lpTarget->isPet() && _lpTarget->isDeath())
			return	TRUE;
	}

	if	(lpSkill->isCastOnDeathSummonBeastSkill())
	{
		if	(isOwnForce(_lpTarget) && _lpTarget->isSummonBeast() && _lpTarget->isDeath() && !_lpTarget->m_wIsReleaseSummonBeast)
			return	TRUE;
	}

	if	(_lpTarget->isDeath())
		return	FALSE;

	if	(lpSkill->isCastOnGroundSkill()	||	lpSkill->isQuickCastSkill())
		return	TRUE;

	if	(lpSkill->isCastOnPetSkill())
	{
		if	(isOwnForce(_lpTarget) && _lpTarget->isPet() && _lpTarget->m_wTamer == m_wSerialInField)
			return	TRUE;
	}
	if	(lpSkill->isCastOnCasterSkill())
	{
		if	(_lpTarget->m_wSerialInField	==	m_wSerialInField)
			return	TRUE;
	}
	if (lpSkill->isCastOnSummonBeastSkill())
	{
		if	(isOwnForce(_lpTarget) && _lpTarget->isSummonBeast() && _lpTarget->m_wTamer == m_wSerialInField)
			return	TRUE;
	}
	if (lpSkill->isCastOnKelby())
	{
		if	(isOwnForce(_lpTarget) && _lpTarget->isSummonBeast() && _lpTarget->m_wTamer == m_wSerialInField && _lpTarget->m_wSummonBeastType	==	eSBT_KELBY)
			return	TRUE;
	}
	if (lpSkill->isCastOnEnemyPetSkill())
	{
		if	(_lpTarget->isPet() && !isOwnForce(_lpTarget))
			return	TRUE;
	}
	if	(lpSkill->isCastOnPartyMemberSkill())
	{
		if	(isPartyMember(_lpTarget) && _lpTarget->m_wSerialInField	!=	m_wSerialInField)
			return	TRUE;
	}
	if	(lpSkill->isCastOnUserPartyMemberSkill())
	{
		if	(isPartyMember(_lpTarget) && _lpTarget->m_wSerialInField	!=	m_wSerialInField	&&	_lpTarget->isPlayer())
			return	TRUE;
	}
	if (lpSkill->isCastOnPlayerSkill())
	{
		if	(isOwnForce(_lpTarget))
			return	TRUE;
	}

	if (lpSkill->isCastOnEnemySkill())
	{
		if	(_lpTarget->m_wSerialInField	==	m_wSerialInField	&&	m_iShakleHP)
			return	TRUE;
		
		if	(_lpTarget->m_iShakleHP	&&	_lpTarget->m_bf1IsFreezeShakle)
			return	TRUE;

		if	(!isOwnForce(_lpTarget))
			return	TRUE;
	}

	return	FALSE;
}

//
//_iSkill인 어빌러티를 찾는다.
cAbility*
cACTOR::getAbilityBySkill(int _iSkill)
{
	if (_iSkill	==	0xffff)	
		return	NULL;

	for	(int i=0;i<dABILITY_COUNT;i++)
		if	(m_aAbility[i].m_wSkill	==	_iSkill)
			return	&m_aAbility[i];

	return	NULL;
}

//
//_iSkill인 어빌러티를 찾는다.
int
cACTOR::getAbilityIndexBySkill(int _iSkill)
{
	if (_iSkill	==	0xffff)
		return	0xffff;

	for (int i=0;i<dABILITY_COUNT;i++)
		if (m_aAbility[i].m_wSkill	==	_iSkill)
			return	i;

	return	0xffff;
}

//
//	어빌러티 레벨 증가
BOOL
cACTOR::increaseAbilityLevel(int _iAbility)
{
	if	(_iAbility	>=	dABILITY_COUNT || _iAbility	<	0)
	{
		CLOG("packetHacking","[%.3d] %s/%s increaseAbilityLevel",m_wCurrentField,m_strId,m_strName);

		setPeneltyTime(dPENELTY_TIME_FOR_SPEEDHACKUSER,ePPPTR_CG_INCREASE_ABILITY_LEVEL);

		return	FALSE;
	}

	cAbility	*lpAbility	=	&m_aAbility[_iAbility];
	CSkill		*lpSkill	=	lpAbility->getSkill();

	if	(!lpSkill											)
		return	FALSE;
	if	(lpSkill->m_bf1IsTestSkill	&& !g_bIsOpenedTestServer	)
		return	FALSE;

	int	iRemainSkillPoint	=	lpAbility->getRemainExpForLevelUp();

	if	(m_dwSkillExperience	<	(DWORD)iRemainSkillPoint)
		return	FALSE;

	for (int i=0;i<dMAX_REQUIRE_SKILL_COUNT;i++)
	{
		if (lpSkill->m_aRequireSkill[i].m_wSkill	==	0xffff)
			break;

		int	iSkill	=	lpSkill->m_aRequireSkill[i].m_wSkill;

		cAbility	*lpRequireAbility	=	getAbilityBySkill(iSkill);

		if (!lpRequireAbility)
			return	FALSE;
		if (lpRequireAbility->m_wLevel	<	lpSkill->m_aRequireSkill[i].m_wLevel)
			return	FALSE;
	}

	if (lpAbility->m_wLevel	>=	dLIMIT_SKILL_LEVEL)
		return	FALSE;

	m_dwSkillExperience	-=	iRemainSkillPoint;

	lpAbility->m_wLevel++;

	if (lpAbility->m_wLevel	>=	dLIMIT_SKILL_LEVEL)
		buildPower();

	return	TRUE;
}

//
//	스킬 포인트 합 얻기
int
cACTOR::getSkillPointSum()
{
	int i,iSkillPoint=m_dwSkillExperience;

	for (i=0;i<dABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if (!lpSkill || lpAbility->m_wLevel==0)	continue;

		int	iPoint	=	lpAbility->getSkillPoint();

		iSkillPoint	+=	iPoint;
	}

	return	iSkillPoint-1;
}

//
//	레벨에 의한 스킬 포인트의 합
int
cACTOR::getSkillPointSumByLevel()
{
	int	iSkillPoint		=	0;

	for (int i=2;i<=m_iLevel;i++)
		iSkillPoint	+=	min(i,100);

	DWORD		dwLevelupExperience		=	g_aExpTable[m_iLevel];
	DWORD		dwNextSkillPoint		=	min(m_iLevel+1,100);
	LONGLONG	llExp					=	m_iExperience;
	llExp								*=	dwNextSkillPoint;

	if	(dwLevelupExperience	!=	0)
		llExp								/=	dwLevelupExperience;

	DWORD		dwCurrentLevelSkillPoint=	(DWORD)llExp;

	iSkillPoint							+=	dwCurrentLevelSkillPoint;	//	경험치에 의한 보정

	if	(m_wBonusSkillPoint	!=	0xffff)
		iSkillPoint						+=	m_wBonusSkillPoint;

	int		iBonusPoint					=	m_bf4CH5SkillBonusCount;

	iSkillPoint							+=	iBonusPoint*100;
	iSkillPoint							+=	c_aBonusSkillPointForRebirth[m_bf4RebirthCount];

	return	iSkillPoint;
}

//
//	스킬 포인트 합 얻기
void
cACTOR::resetSkillLevel()
{
	for (int i=0;i<52;i++)
		m_aAbility[i].m_wLevel	=	0;

	m_aAbility[0].m_wLevel	=	1;

	if	(m_wJob	== 0)
		m_aAbility[1].m_wLevel	=	1;
}

//
//	리액션 켜기
int
cACTOR::reactionOn(int _iAbility)
{
	cAbility	*lpAbility	=	getAbility(_iAbility);
	if (!lpAbility)
		return	eRRS_FAILED;

	CSkill		*lpSkill	=	lpAbility->getSkill();

	if (!lpSkill)
		return	eRRS_FAILED;
	if (!lpSkill->isPassiveSkill())
		return	eRRS_FAILED;
	if (lpAbility->m_wLevel	<=	0)
		return	eRRS_FAILED;
	if(m_wNotUseActivityReactionSkillTime)
		return eRRS_NOT_USE_REACTION_SKILL_TIME;
	
	if(m_lpField->m_bIsNotUseSkillBeforeTrans)
		if(!m_wTransformationTime)
			return eRRS_FAILED;


	int	i,iCount=0;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		if (m_aActiveReaction[i]	==	_iAbility	)
			return	eRRS_OK;
	}
	int iPartyLimitCount = getPartyReactionLimitCount();
	iCount = getReactionCount();

	if	( iCount	>=	dPLAYER_ACTIVE_REACTION_LIMIT || (lpSkill->m_bf1IsReactionLimitCount && iPartyLimitCount >=dPLAYER_ACTIVE_REACTION_LIMIT) )
		return	eRRS_TOO_MANY_REACTION;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		if (m_aActiveReaction[i]	==	0xffff)
		{
			m_aActiveReaction[i]	=	_iAbility;
			m_aAuraSkillTimer[i]	=	0;
			return	eRRS_OK;
		}
	}

	return	eRRS_FAILED;
}
int 
cACTOR::getReactionCount()
{
	int iCount = 0;
	for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		if (m_aActiveReaction[i]	!=	0xffff	)
			++iCount;
	}
	return iCount;
}
BOOL
cACTOR::isReactionLimit(int _iAbility)
{
	cAbility* lpAbility = getAbility(_iAbility);
	if (!lpAbility)
		return FALSE;
	CSkill		*lpSkill	=	lpAbility->getSkill();
	if(!lpSkill)
		return FALSE;
	if(lpSkill->m_bf1IsReactionLimitCount)
		return TRUE;	
	return FALSE;
}

int
cACTOR::getReactionLimitCount()
{
	int iCount = 0;
	for (int i=0;i<getReactionCount();i++)
	{
		if(isReactionLimit(m_aActiveReaction[i]))
			++iCount;
	}
	return iCount;
}
int
cACTOR::getPartyReactionLimitCount()
{
	
	cParty	*lpParty	=	getParty();
	
	if (!lpParty)
		return	-1;
	int iCount = 0;
	for (int i=0;i<(int)lpParty->getMemberCount();i++)
	{
		cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);
		if(!lpMember)
			continue;
		int iMemberReactionCount = lpMember->getReactionLimitCount();
		iCount+= iMemberReactionCount;
	}
	return iCount;
}


void
cACTOR::reactionAllOff()
{	// 리액션스킬 모두 끄기...

	for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
			m_aActiveReaction[i]	=	0xffff;
			m_aAuraSkillTimer[i]	=	0;
	}
}

//
//	리액션 끄기
BOOL
cACTOR::reactionOff(int _iAbility)
{
	int	i,j;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		if (m_aActiveReaction[i]	==	_iAbility	)
		{
			m_aActiveReaction[i]	=	0xffff;

			for (j=i;j<dACTIVE_REACTION_COUNT-1;j++)
			{
				m_aActiveReaction[j]	=	m_aActiveReaction[j+1];
				m_aAuraSkillTimer[j]	=	m_aAuraSkillTimer[j+1];
			}

			m_aActiveReaction[dACTIVE_REACTION_COUNT-1]	=	0xffff;

			return	eRRS_OK;
		}
	}

	return	eRRS_OK;
}
//
//	리액션 상태
BOOL
cACTOR::isActiveReaction(int _iAbility)
{
	if(m_wNotUseActivityReactionSkillTime)
		return FALSE;

	int	i;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
		if (m_aActiveReaction[i]	==	_iAbility	)	return	TRUE;

	return	FALSE;
}

//
//	컴플리트 프로텍션이 설정되어 있다면 해제 시킨다.
void
cACTOR::releaseCompleteProtection()
{
	if (isActiveSpecialSkill(dSSF_COMPLETE_PROTECTION))
	{
		turnOffSpecialSkillField(dSSF_COMPLETE_PROTECTION);

		m_lpField->sendChangeSpecialSkillField((cACTOR*)this);
	}
}

//
//	시머링 실드 끝내기
void
cACTOR::releaseShimmeringShield()
{
	releaseCastShimmeringShield();
	releaseReceiveShimmeringShield();
}

//
//	시머링 실드 끝내기
void
cACTOR::releaseCastShimmeringShield()
{
	if	(m_wCastBlocker	!=	0xffff)
	{
		CActiveSkill	*lpSkill	=	m_lpField->m_skillManager.getActiveSkill(m_wCastBlocker);

		if	(lpSkill	==	NULL)
		{
			m_wCastBlocker	=	0xffff;
			return;
		}

		if	(lpSkill->isShimmeringCaster(m_wSerialInField))	
			lpSkill->releaseShimmeringShield();
	}

	if	(m_bf10CastWeaponBlocker	!=	dVALID_BLOCKER_CODE)
	{
		CActiveSkill	*lpSkill	=	m_lpField->m_skillManager.getActiveSkill(m_bf10CastWeaponBlocker);

		if	(lpSkill	==	NULL)
		{
			if	(m_bf1CastWeaponBlockerJob	==	0)
				m_wHideWeapon1Time	=	0;
			else
				m_wHideWeapon2Time	=	0;

			m_bf10CastWeaponBlocker	=	dVALID_BLOCKER_CODE;
			
			return;
		}

		if	(lpSkill->isShimmeringCaster(m_wSerialInField))	
			lpSkill->releaseShimmeringShield();
	}

}

//
//	시머링 실드 끝내기
void
cACTOR::releaseReceiveShimmeringShield()
{
	if	(m_wReceiveBlocker	!=	0xffff)
	{
		CActiveSkill	*lpSkill	=	m_lpField->m_skillManager.getActiveSkill(m_wReceiveBlocker);

		if	(lpSkill	==	NULL)
		{
			m_wReceiveBlocker	=	0xffff;
			return;
		}

		if	(lpSkill->isShimmeringReceiver(m_wSerialInField))
			lpSkill->releaseShimmeringShield();
	}

	if	(m_wReceiveWeaponBlocker	!=	0xffff)
	{
		CActiveSkill	*lpSkill	=	m_lpField->m_skillManager.getActiveSkill(m_wReceiveWeaponBlocker);

		if	(lpSkill	==	NULL)
		{
			m_wReceiveBlocker	=	0xffff;
			return;
		}

		if	(lpSkill->isShimmeringReceiver(m_wSerialInField))
			lpSkill->releaseShimmeringShield();
	}
}

//
//	시머링 실드 캐스터냐?
BOOL
cACTOR::isShimmeringShieldCaster()
{
	if	(m_wCastBlocker	!=	0xffff || m_bf10CastWeaponBlocker	!=	dVALID_BLOCKER_CODE)
		return TRUE;

	return	FALSE;
}

//
//	패시브 스킬들 전부 해제
void
cACTOR::releaseAllPassiveSkill()
{
	releaseShimmeringShield();

	memset(m_aActiveReaction,0xff,sizeof(m_aActiveReaction));
}

//
//	머쉰 띄우기
void
cACTOR::floatMachine(int _iSkillSerial,int _iSkill)
{
	m_wMachine	=	_iSkillSerial;

	m_lpField->sendFloatMachine(m_wSerialInField,_iSkill);
}

//
//머신 미사일 발사
void
cACTOR::shootMachineMissile(int _iTarget,int _iSkill,CHitInfo *_lpHitInfo)
{
	m_lpField->sendMachineMissile(m_wSerialInField,_iTarget,_iSkill,_lpHitInfo);
}

//
//	머신 제거
void
cACTOR::releaseMachine()
{
	if (m_wMachine	==	0xffff)	return;

	m_wMachine	=	0xffff;

	m_lpField->sendReleaseMachine(m_wSerialInField);
}

//
//	컴플리트 프로텍션 얻기
BOOL
cACTOR::getCompleteProtectionAbility(cAbility *_lpAbility)
{
	int	i;

	for (i=0;i<dABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(!lpSkill	||	lpSkill->m_wType	!=	eSKILL_TYPE_COMPLETE_PROTECTION	)
			continue;
		
		_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
		_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);

		return	TRUE;
	}

	return	FALSE;
}

//
//	아이시 스탈라그마이트 캐스팅
void
cACTOR::castIcyStalagmite(cAbility *_lpAbility,int _iStatus)
{
	m_lpField->castIcyStalagmite(this,_lpAbility,_iStatus);
}

//
//	아이시 스탈라그마이트 해제~
void
cACTOR::releaseIcyStalagmite(int _iStatus)
{
	if	(m_wHillSkill==	0xffff)
		return;

	CActiveSkill	*lpHillSkill	=	m_lpField->m_skillManager.getActiveSkill(m_wHillSkill);

	if	(lpHillSkill	==	NULL	||	lpHillSkill->m_wSerial	!=	m_wHillSkill)
	{
		m_wHillSkill=	0xffff;
		return;
	}

	m_lpField->castIcyStalagmite(this,&lpHillSkill->m_ability,eRUIS_DESTROY);

	m_lpField->m_skillManager.remove(m_wHillSkill);

	m_wHillSkill	=	0xffff;
}

//
//	인터벌 슈터 설정
void
cACTOR::setIntervalShooter(int _iSkill)
{
	if (m_wIntervalShooter	!=	0xffff)
	{
		CActiveSkill	*lpIntervalShooter	=	m_lpField->m_skillManager.getActiveSkill(m_wIntervalShooter);

		if (lpIntervalShooter)
			m_lpField->m_skillManager.remove(m_wIntervalShooter);
	}

	m_wIntervalShooter	=	_iSkill;		
}

//
//	인터벌 슈터 해제
void
cACTOR::releaseIntervalShooter()
{
	m_wIntervalShooter	=	0xffff;
}

//
//	마스터한 스킬의 수
int
cACTOR::getMasterSkillCount()
{
	int	iMasterSkillCount	=	0;

	for (int i=0;i<dABILITY_COUNT;i++)
	{
		CSkill	*lpSkill	=	m_aAbility[i].getSkill();

		if (!lpSkill)
			continue;

		if (!lpSkill->isEnableJob(m_wJob))
			continue;

		if (m_aAbility[i].m_wLevel == dLIMIT_SKILL_LEVEL)
			iMasterSkillCount++;
	}

	return	iMasterSkillCount;
}

cAbility*
cACTOR::getPigeonPostItemSkill()
{
	for (int i=0;i<dABILITY_COUNT;i++)
	{
		CSkill	*lpSkill	=	m_aAbility[i].getSkill();

		if	(!lpSkill)
			continue;

		if	(lpSkill->m_bf1IsPigeonPostItemSkill)
			return	&m_aAbility[i];
	}

	return	NULL;
}

cAbility*
cACTOR::getGlareSkill(BOOL _bIsInReaction)
{
	if	(_bIsInReaction)
	{
		for (int	i=0;i<dACTIVE_REACTION_COUNT;i++)
		{
			int			iAbility	=	m_aActiveReaction[i];

			if	(iAbility	==	0xffff)
				continue;

			cAbility	*lpAbility	=	&m_aAbility[iAbility];
			CSkill		*lpSkill	=	lpAbility->getSkill();

			if (lpSkill	&&	lpSkill->m_bf1IsGlareSkill)
				return	lpAbility;
		}
	}
	else
	for (int i=0;i<dABILITY_COUNT;i++)
	{
		CSkill	*lpSkill	=	m_aAbility[i].getSkill();

		if (!lpSkill)
			continue;

		if (lpSkill->m_bf1IsGlareSkill)
			return	&m_aAbility[i];
	}

	return	NULL;
}

cAbility*
cACTOR::getRidingDogSkill()
{
	for (int i=0;i<dABILITY_COUNT;i++)
	{
		CSkill	*lpSkill	=	m_aAbility[i].getSkill();

		if (!lpSkill)
			continue;

		for (int j=0;j<dEXTRA_EFFECT_COUNT;j++)
			if (lpSkill->m_aExtraEffect[j].m_wEffect	==	eSKILL_EE_RIDING_DOG)
				return	&m_aAbility[i];
	}

	return	NULL;
}

cAbility*
cACTOR::getUnlockDoorSkill()
{
	return	getSpecialFeatureSkill(eSUSF_IS_UNLOCK_DOOR);
}

cAbility*
cACTOR::getUnlockArcaSkill()
{
	return	getSpecialFeatureSkill(eSUSF_IS_UNLOCK_ARCA);
}

BOOL
cACTOR::isVisibleTrap(int _iDetectTrapLevel)
{
	if (m_detectTrap.getCorrectedValue()	<	_iDetectTrapLevel)
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::isVisibleObject(int _iSecretLevel)
{
	if (m_detectSecretObject.getCorrectedValue()	<	_iSecretLevel)
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::isVisibleDoor(cAreaDoorInfo *_lpDoor)
{
	return	isVisibleObject(_lpDoor->m_bSecretLevel);
}

BOOL
cACTOR::isVisibleDoorTrap(cAreaDoorInfo *_lpDoor)
{
	return	isVisibleTrap(_lpDoor->m_bDetectTrapLevel);
}

cAbility*
cACTOR::getSpecialFeatureSkill(int _iFeature)
{
	for (int i=0;i<dABILITY_COUNT;i++)
	{
		cAbility*	lpAbility	=	&m_aAbility[i];
		
		if (lpAbility->m_wLevel	<=	0)
			continue;
		
		CSkill*		lpSkill		=	lpAbility->getSkill();

		if (!lpSkill || !lpSkill->isEnableJob(m_wJob) || lpSkill->m_wSkillUniqueSpecialFeature	!=	_iFeature)
			continue;

		return	lpAbility;
	}

	return	NULL;
}

cAbility*
cACTOR::getDisarmTrapAbility()
{
	return	getSpecialFeatureSkill(eSUSF_IS_DISARM);
}

BOOL
cACTOR::isDisarmAbleTrap(int _iTrapLevel)
{
	cAbility	*lpAbility	=	getDisarmTrapAbility();

	if (!lpAbility)
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::isUnlockAbleArcaBySkill(int _iLockLevel)
{
	int		iCorrectUnlockLevel=	getCorrectUnlockLevelByTitle();

	cAbility	*lpAbility	=	getUnlockArcaSkill();

	if	(!lpAbility)
		return	FALSE;

	int		iCorrectSkillLevel=		getCorrectSkillLevel(lpAbility);
	
	if	(lpAbility->getUnlockArcaLevel()+iCorrectUnlockLevel+iCorrectSkillLevel	>=	_iLockLevel)
		return	TRUE;

	return	FALSE;
}

BOOL
cACTOR::isUnlockAbleDoorBySkill(int _iLockLevel)
{
	cAbility	*lpAbility	=	getUnlockDoorSkill();

	if (!lpAbility)
		return	FALSE;

	CSkill				*lpSkill	=	lpAbility->getSkill();
	CSkillExtraEffect	*lpEffect	=	lpSkill->getExtraEffectByEffect(eSKILL_EE_UNLOCK_DOOR);

	if (!lpEffect)
		return	FALSE;

	int		iUnlockLevel=	(lpEffect->m_aiValue[0]+lpEffect->m_aiValue[1]*lpAbility->m_wLevel)/100;

	if (iUnlockLevel	>=	_iLockLevel)
		return	TRUE;

	return	FALSE;
}

int
cACTOR::getUnlockArcaKeySlot(int _iLockLevel)
{
	int		i,iMinLevel			=	10000;
	int		iKey				=	0xffff;
	
	int		iCorrectUnlockLevel	=	getCorrectUnlockLevel();
	
	for (i=0;i<m_wItemSlotCount;i++)
	{
		int	iLevel	=	m_aItems[i].getUnlockArcaLevel();

		if	(iLevel	<=	0)
			continue;

		iLevel		+=	iCorrectUnlockLevel;

		if	(iLevel	<	_iLockLevel)
			continue;

		if	(iLevel	<	iMinLevel)
		{
			iMinLevel	=	iLevel;
			iKey		=	i;
		}
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		int	iLevel	=	m_aExtraInventory[i].getUnlockArcaLevel();

		if	(iLevel	<=	0)
			continue;

		iLevel		+=	iCorrectUnlockLevel;

		if	(iLevel	<	_iLockLevel)
			continue;

		if	(iLevel	<	iMinLevel)
		{
			iMinLevel	=	iLevel;
			iKey		=	i+dOWN_ITEM_COUNT;
		}
	}

	return	iKey;
}

int
cACTOR::getUnlockDoorKeySlot(int _iLockLevel)
{
	int		i,iMinLevel			=	10000;
	int		iKey				=	0xffff;
	int		iCorrectUnlockLevel	=	getCorrectUnlockLevel();

	for (i=0;i<m_wItemSlotCount;i++)
	{
		int	iLevel	=	m_aItems[i].getUnlockDoorLevel();

		if	(iLevel	<=	0)
			continue;

		iLevel		+=	iCorrectUnlockLevel;

		if (iLevel	<	_iLockLevel)
			continue;

		if (iLevel	<	iMinLevel)
		{
			iMinLevel	=	iLevel;
			iKey		=	i;
		}
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		int	iLevel	=	m_aExtraInventory[i].getUnlockDoorLevel();

		if	(iLevel	<=	0)
			continue;

		iLevel		+=	iCorrectUnlockLevel;

		if	(iLevel	<	_iLockLevel)
			continue;

		if (iLevel	<	iMinLevel)
		{
			iMinLevel	=	iLevel;
			iKey		=	i+dOWN_ITEM_COUNT;
		}
	}

	return	iKey;
}

cItem*
cACTOR::getUnlockArcaKey(int _iLockLevel)
{
	int	iSlot	=	getUnlockArcaKeySlot(_iLockLevel);

	if (iSlot	==	0xffff)
		return	NULL;

	if (iSlot	<	dOWN_ITEM_COUNT)
		return	&m_aItems[iSlot];

	return	&m_aExtraInventory[iSlot-dOWN_ITEM_COUNT];
}

cItem*
cACTOR::getUnlockDoorKey(int _iLockLevel)
{
	int	iSlot	=	getUnlockDoorKeySlot(_iLockLevel);

	if (iSlot	==	0xffff)
		return	NULL;

	if (iSlot	<	dOWN_ITEM_COUNT)
		return	&m_aItems[iSlot];

	return	&m_aExtraInventory[iSlot-dOWN_ITEM_COUNT];
}

BOOL
cACTOR::isUnlockAbleDoorByKey(int _iLockLevel)
{
	int	iKey	=	getUnlockDoorKeySlot(_iLockLevel);

	if (iKey	==	0xffff)
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::isUnlockAbleArcaByKey(int _iLockLevel)
{
	int	iKey	=	getUnlockArcaKeySlot(_iLockLevel);

	if (iKey	==	0xffff)
		return	FALSE;

	return	TRUE;
}

int
cACTOR::getDefaultAttackAbilityIndex()
{
	int	iAbility	=	-1,i;

	if	(isPlayer())
	{
		for (i=0;i<dABILITY_COUNT;i++)
		{
			if (m_aAbility[i].m_wLevel	==	0)
				continue;

			if (m_aAbility[i].m_wSkill	==	g_aPlayer1LvSetting[m_wJob].m_wDefaultSkill)
			{
				iAbility	=	i;
				break;
			}
		}
	}
	else
	{
		for (i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
		{
			CSkill	*lpSkill	=	m_aAbility[i].getSkill();

			if (lpSkill	==	NULL	||	lpSkill->isPassiveSkill())
				continue;

			iAbility	=	i;
		}
	}

	return	iAbility;
}

void
cACTOR::hitThrowDrug(cItem *_lpItem,cAbility *_lpAbility,int _iHealHP)
{
	eatItem(_lpItem);
	addSendSkillEffect(_lpAbility,eSE_HIT);
	
	if	(g_bIsDuelServer)
		_iHealHP	=	_iHealHP*g_iCorrectHealEfficientUnderDuelBattle/100;

	m_lpField->sendCureActorMessage(m_wSerialInField,_lpAbility->m_wSkill,_iHealHP);
	m_iRecoverHP	+=	_iHealHP;
}

int
cACTOR::hitThrowPotion(cItem *_lpItem,cAbility *_lpAbility,int _iEfficient)
{
	int	iOld	=	m_iRecoverHP;

	eatItem(_lpItem,_iEfficient);

	return	m_iRecoverHP-iOld;
}

void
cACTOR::throwDrug(cACTOR *_lpTarget,cAbility *_lpAbility,cItem *_lpItem,int _iHitRange,int _iHealHP)
{
	SG_WIDE_HEAL	packet;

	packet.base.set(sizeof(packet),dSG_WIDE_HEAL);
	packet.wCount	=	0;
	packet.wSkill	=	_lpAbility->m_wSkill;

	_lpTarget->addSendSkillEffect(_lpAbility,eSE_EXPLOSION);

	cParty	party;

	if	(!g_pPartyManager->getParty(&party,getPartySerial()))
	{
		_lpTarget->hitThrowDrug(_lpItem,_lpAbility,_iHealHP);

		packet.wCount				=	1;
		packet.aData[0].wActor		=	_lpTarget->m_wSerialInField;
		packet.aData[0].wHealPoint	=	(WORD)max(_iHealHP/100,1);
		packet.base.wSize			=	sizeof(packet)-sizeof(packet.aData)+sizeof(cWideHealE);

		m_lpField->addSendPacket(&packet,_lpTarget->m_wSerialInField);

		return;
	}

	DWORD				dwRange;

	_iHitRange			*=	_iHitRange;

	for	(int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if	(!lpMember	||	lpMember->m_wCurrentField	!=	m_wCurrentField)
			continue;

		dwRange		=	GetOvalRange(_lpTarget->m_pos.x,_lpTarget->m_pos.y,lpMember->m_pos.x,lpMember->m_pos.y);

		if	(dwRange	<=	(DWORD)_iHitRange)
		{
			lpMember->hitThrowDrug(_lpItem,_lpAbility,_iHealHP);

			packet.aData[packet.wCount].wActor		=	lpMember->m_wSerialInField;
			packet.aData[packet.wCount].wHealPoint	=	(WORD)max(_iHealHP/100,1);
			packet.wCount++;

			for	(int iPet=0;iPet<dOWN_PET_COUNT;iPet++)
			{
				cACTOR	*lpPet	=	lpMember->getPetActor(iPet);

				if	(!lpPet)
					continue;

				dwRange		=	GetOvalRange(_lpTarget->m_pos.x,_lpTarget->m_pos.y,lpPet->m_pos.x,lpPet->m_pos.y);

				if	(dwRange	>	(DWORD)_iHitRange)
					continue;

				lpPet->hitThrowDrug(_lpItem,_lpAbility,_iHealHP);

				packet.aData[packet.wCount].wActor		=	lpPet->m_wSerialInField;
				packet.aData[packet.wCount].wHealPoint	=	(WORD)max(_iHealHP/100,1);
				packet.wCount++;
			}
		}

		for	(int iSummonBeast=0;iSummonBeast<dOWN_SUMMON_BEAST_COUNT;iSummonBeast++)
		{
			cACTOR	*lpSummonBeast	=	lpMember->getSummonBeastActor(iSummonBeast);

			if	(!lpSummonBeast)
				continue;

			dwRange		=	GetOvalRange(_lpTarget->m_pos.x,_lpTarget->m_pos.y,lpSummonBeast->m_pos.x,lpSummonBeast->m_pos.y);

			if	(dwRange	>	(DWORD)_iHitRange)
				continue;

			lpSummonBeast->hitThrowDrug(_lpItem,_lpAbility,_iHealHP);

			packet.aData[packet.wCount].wActor		=	lpSummonBeast->m_wSerialInField;
			packet.aData[packet.wCount].wHealPoint	=	(WORD)max(_iHealHP/100,1);
			packet.wCount++;
		}

		if	(packet.wCount+5	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
		{
			packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+sizeof(cWideHealE)*packet.wCount;

			m_lpField->addSendPacket(&packet,_lpTarget->m_wSerialInField);
			packet.wCount	=	0;
		}
	}

	if	(packet.wCount)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+sizeof(cWideHealE)*packet.wCount;

		m_lpField->addSendPacket(&packet,_lpTarget->m_wSerialInField);
	}
}

void
cACTOR::throwPotion(cACTOR *_lpTarget,cAbility *_lpAbility,cItem *_lpItem,int _iEfficient,int _iHitRange)
{
	SG_WIDE_HEAL	packet;

	packet.base.set(sizeof(packet),dSG_WIDE_HEAL);
	packet.wSkill	=	_lpAbility->m_wSkill;
	packet.wCount	=	0;

	_lpTarget->addSendSkillEffect(_lpAbility,eSE_EXPLOSION);

	cParty	party;

	if	(!g_pPartyManager->getParty(&party,getPartySerial()))
	{
		int	iHealPoint				=	_lpTarget->hitThrowPotion(_lpItem,_lpAbility,_iEfficient);

		packet.wCount				=	1;
		packet.aData[0].wActor		=	_lpTarget->m_wSerialInField;
		packet.aData[0].wHealPoint	=	max(iHealPoint/100,1);
		packet.base.wSize			=	sizeof(packet)-sizeof(packet.aData)+sizeof(cWideHealE);

		m_lpField->addSendPacket(&packet,_lpTarget->m_wSerialInField);

		return;
	}

	DWORD				dwRange;

	_iHitRange			*=	_iHitRange;

	for	(int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if	(!lpMember	||	lpMember->m_wCurrentField	!=	m_wCurrentField)
			continue;

		dwRange		=	GetOvalRange(_lpTarget->m_pos.x,_lpTarget->m_pos.y,lpMember->m_pos.x,lpMember->m_pos.y);

		if	(dwRange	<=	(DWORD)_iHitRange)
		{
			int	iHealPoint	=	lpMember->hitThrowPotion(_lpItem,_lpAbility,_iEfficient);;

			packet.aData[packet.wCount].wActor		=	lpMember->m_wSerialInField;
			packet.aData[packet.wCount].wHealPoint	=	(WORD)max(iHealPoint/100,1);
			packet.wCount++;
		}

		for	(int iPet=0;iPet<dOWN_PET_COUNT;iPet++)
		{
			cACTOR	*lpPet	=	lpMember->getPetActor(iPet);

			if	(!lpPet)
				continue;

			dwRange		=	GetOvalRange(_lpTarget->m_pos.x,_lpTarget->m_pos.y,lpPet->m_pos.x,lpPet->m_pos.y);

			if	(dwRange	>	(DWORD)_iHitRange)
				continue;

			int	iHealPoint	=	lpPet->hitThrowPotion(_lpItem,_lpAbility,_iEfficient);

			packet.aData[packet.wCount].wActor		=	lpPet->m_wSerialInField;
			packet.aData[packet.wCount].wHealPoint	=	(WORD)max(iHealPoint/100,1);
			packet.wCount++;
		}

		for	(int iSummonBeast=0;iSummonBeast<dOWN_SUMMON_BEAST_COUNT;iSummonBeast++)
		{
			cACTOR	*lpSummonBeast	=	lpMember->getSummonBeastActor(iSummonBeast);

			if	(!lpSummonBeast)
				continue;

			dwRange		=	GetOvalRange(_lpTarget->m_pos.x,_lpTarget->m_pos.y,lpSummonBeast->m_pos.x,lpSummonBeast->m_pos.y);

			if	(dwRange	>	(DWORD)_iHitRange)
				continue;

			int	iHealPoint	=	lpSummonBeast->hitThrowPotion(_lpItem,_lpAbility,_iEfficient);;

			packet.aData[packet.wCount].wActor		=	lpMember->m_wSerialInField;
			packet.aData[packet.wCount].wHealPoint	=	(WORD)max(iHealPoint/100,1);
			packet.wCount++;
		}

		if	(packet.wCount+5	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
		{
			packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+sizeof(cWideHealE)*packet.wCount;

			m_lpField->addSendPacket(&packet,_lpTarget->m_wSerialInField);
			packet.wCount	=	0;
		}
	}

	if	(packet.wCount)
	{
		packet.base.wSize		=	sizeof(packet)-sizeof(packet.aData)+sizeof(cWideHealE)*packet.wCount;
		m_lpField->addSendPacket(&packet,_lpTarget->m_wSerialInField);
	}
}

void
cACTOR::throwCandy(cACTOR *_lpTarget,cAbility *_lpAbility,int _iHitRange)
{
	BOOL	bIsGotCP	=	TRUE;
	int		iHitRange	=	_lpAbility->getHitRange(getWeapon());

	m_lpField->strikeExplosionSkill(this,_lpTarget,_lpTarget->m_pos.x,_lpTarget->m_pos.y,_lpAbility,&bIsGotCP);
}

void
cACTOR::operateFatGirlStatus()
{
	if	(g_dwCurrentTime	<	m_dwActionTimeUnderFatGirlStatus)
		return;

	m_dwActionTimeUnderFatGirlStatus	=	g_dwCurrentTime+m_wActionPerionUnderFatGirlStatus;

	cAbility	ability;

	ability.set(m_wFatGirlSkill,m_wFatGirlSkillLevel);

	if	(m_lpField->strikeWhirlWind(this,&ability))
		m_lpField->addSendOccurEffect(m_pos.x,m_pos.y,ability.getSkill()->m_wExplosionImage);
}
