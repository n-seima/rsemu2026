#include "CActor.H"
#include "CMessage.H"
#include "cDebugWindow.H"

//
//	이 스킬이 이녀석한테 사용 가능한 스킬이냐?
BOOL
CActor::isCastAbleSkill(cAbility *_lpAbility,BOOL _bIsCheck)
{
	if (_lpAbility	==	NULL)
		return	FALSE;

	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if (!lpSkill)
		return	FALSE;
	if (isNpc())
		return	FALSE;

	int	iLevel	=	g_hero.getLevel()+_lpAbility->m_wLevel+g_hero.getCorrectSkillLevel(_lpAbility);

	if (lpSkill->m_sTargetLevelLimitType1	!=	0x7fff)
	{
		int		iLimitLevel		=	iLevel+lpSkill->m_sTargetLevelLimitType1;
		
		if (m_wLevel	>	max(iLimitLevel,1))
		{
			if (_bIsCheck	==	FALSE)
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_USE_SKILL_TO_TARGET_FAIELD_BY_LOW_LEVEL);
			return	FALSE;
		}
	}

	if (lpSkill->m_sHumanTargetLevelLimitType1	!=	0x7fff && isHuman())
	{
		int		iLimitLevel		=	iLevel+lpSkill->m_sHumanTargetLevelLimitType1;
		
		if (m_wLevel	>	max(iLimitLevel,1))
		{
			if (_bIsCheck	==	FALSE)
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_USE_SKILL_TO_TARGET_FAIELD_BY_LOW_LEVEL);
			return	FALSE;
		}
	}
	if (lpSkill->m_sUndeadTargetLevelLimitType1	!=	0x7fff && isUndead())
	{
		int		iLimitLevel		=	iLevel+lpSkill->m_sUndeadTargetLevelLimitType1;
		
		if (m_wLevel	>	max(iLimitLevel,1))
		{
			if (_bIsCheck	==	FALSE)
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_USE_SKILL_TO_TARGET_FAIELD_BY_LOW_LEVEL);
			return	FALSE;
		}
	}
	if (lpSkill->m_sAnimalTargetLevelLimitType1	!=	0x7fff && isAnimal())
	{
		int		iLimitLevel		=	iLevel+lpSkill->m_sAnimalTargetLevelLimitType1;
		
		if (m_wLevel	>	max(iLimitLevel,1))
		{
			if (_bIsCheck	==	FALSE)
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_USE_SKILL_TO_TARGET_FAIELD_BY_LOW_LEVEL);
			return	FALSE;
		}
	}
	if (lpSkill->m_sHolyBeastTargetLevelLimitType1	!=	0x7fff && isHolyBeast())
	{
		int		iLimitLevel		=	iLevel+lpSkill->m_sHolyBeastTargetLevelLimitType1;
		
		if (m_wLevel	>	max(iLimitLevel,1))
		{
			if (_bIsCheck	==	FALSE)
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_USE_SKILL_TO_TARGET_FAIELD_BY_LOW_LEVEL);
			return	FALSE;
		}
	}
	if (lpSkill->m_sDemonTargetLevelLimitType1	!=	0x7fff && isDemon())
	{
		int		iLimitLevel		=	iLevel+lpSkill->m_sDemonTargetLevelLimitType1;
		
		if (m_wLevel	>	max(iLimitLevel,1))
		{
			if (_bIsCheck	==	FALSE)
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_USE_SKILL_TO_TARGET_FAIELD_BY_LOW_LEVEL);
			return	FALSE;
		}
	}

	if (lpSkill->isCastOnDeathPlayerSkill())
	{
		if (isOwnTeam() && isDeath())
			return	TRUE;
	}

	if (lpSkill->isCastOnDeathPetSkill())
	{
		if (isOwnTeam() && isPet() && isDeath())
			return	TRUE;
	}

	if (lpSkill->isCastOnDeathSummonBeastSkill())
	{
		if (isOwnTeam() && isSummonBeast() && isDeath() && !m_wIsReleaseSummonBeast)
			return	TRUE;
	}

	if	(isDeath())
		return	FALSE;

	if (lpSkill->isCastOnGroundSkill()	||	lpSkill->isQuickCastSkill())
		return	TRUE;

	if (lpSkill->isCastOnPetSkill())
	{
		if	(isOwnTeam() && g_hero.isPet(m_wSerial))
			return	TRUE;
	}
	if	(lpSkill->isCastOnCasterSkill())
	{
		if	(isHero())
			return	TRUE;
	}
	if (lpSkill->isCastOnSummonBeastSkill())
	{
		if	(isOwnTeam() && g_hero.isSummonBeastActor(m_wSerial))
			return	TRUE;
	}
	if (lpSkill->isCastOnKelby())
	{
		if	(isOwnTeam() && g_hero.isSummonBeastActor(m_wSerial,dSBT_KELBY))
			return	TRUE;
	}
	if (lpSkill->isCastOnEnemyPetSkill())
	{
		if	(isPet() && !isOwnTeam())
			return	TRUE;
	}
	if	(lpSkill->isCastOnUserPartyMemberSkill())
	{
		if	(isPartyMember() && isHero() == FALSE && isPlayer())
			return	TRUE;
	}
	if	(lpSkill->isCastOnPartyMemberSkill())
	{
		if	(isPartyMember() && isHero() == FALSE)
			return	TRUE;
	}

	if (lpSkill->isCastOnPlayerSkill())
	{
		if	(isOwnTeam())
			return	TRUE;
	}

	if (lpSkill->isCastOnEnemySkill())
	{
		if	(isHero()	&&	m_wShakleSkill	!=	0xffff)
			return	TRUE;

		if	(m_bf1IsFreezeShakle)
			return	TRUE;

		if	(!isOwnTeam())
			return	TRUE;
	}

	return	FALSE;
}

//
//	현재 사용중인 스킬
CActiveSkill*
CActor::getActiveSkill()
{
	return	g_sm.get(m_wUseSkill);
}

//
//	현재 위치에서 타격범위 안에 드는가?
BOOL
CActor::isInAttackRange(int _iX,int _iY,int _iRange)
{
	_iRange	+=	getBodySize();
	_iRange	*=	_iRange;

	if	(GetOvalRange(m_pos.x,m_pos.y,_iX,_iY)	<=	_iRange)
		return TRUE;

	return	FALSE;
}

//
//	이녀석 울 파티 멤버냐?
BOOL
CActor::isPartyMember()
{
	return	s_partyInfo.isPartyMember(m_wSerial);
}

//
//	얘 파티 리더냐?
BOOL
CActor::isPartyLeader()
{
	return	s_partyInfo.isPartyLeader(m_wSerial);
}

//
//	얘 파티 리더냐?
BOOL
CActor::isUnionPartyLeader()
{
	return	s_partyInfo.isUnionPartyLeader(m_wSerial);
}

//
//	몬스터를 제어하고 있다.
BOOL
CActor::isControlMonster()
{
	if (isHero() && g_hero.m_wSlaveMonster != 0xffff)
		return	TRUE;

	return	FALSE;
}

BOOL
CActor::isFocusActor()
{
	if (m_wSerial	==	s_iFocusActor)
		return	TRUE;

	return	FALSE;
}

BOOL
CActor::isEngageTarget()
{
	return	g_hero.isEngageTarget(m_wSerial);
}

BOOL
CActor::isPetEngageTarget()
{
	return	g_hero.isPetEngageTarget(m_wSerial);
}


int
CActor::getHillHeight()
{
	if	(m_wHillSkill	==	0xffff)
		return	0;

	CActiveSkill	*lpSkill	=	g_sm.get(m_wHillSkill);

	if	(!lpSkill)
		return	0;

	return	lpSkill->m_sHeight*m_wVerticalScale/100;
}

int
CActor::getHeight(BOOL _bIsIncludeLevitateHeight)
{
	int	iHeight	=	getHillHeight()+(m_wJumpHeight + m_iHeightJumpRush + m_iHeightEffectJump)*m_wVerticalScale/100;

	if	(_bIsIncludeLevitateHeight)
		iHeight	+=	m_sLevitateHeight*m_wVerticalScale/100;

	return	iHeight;
}
int
CActor::getBodyWidth(BOOL _bIsWantApplyScale)
{
	if	(_bIsWantApplyScale)
		return	getBody()->m_sprite.m_iWidth*m_wHorizonScale/100;

	return	getBody()->m_sprite.m_iWidth;
}

int
CActor::getBodyHeight(BOOL _bIsWantApplyScale)
{
	if	(_bIsWantApplyScale)
		return	getBody()->m_sprite.m_iHeight*m_wVerticalScale/100;

	return	getBody()->m_sprite.m_iHeight;
}

int
CActor::getBodySize(BOOL _bIsWantApplyScale)
{
	if	(_bIsWantApplyScale)
		return	getBody()->m_iCrashSize*m_wHorizonScale/100;

	return	getBody()->m_iCrashSize;
}


BOOL
CActor::isBlockToMove()
{
	if	(m_enchantedImage.m_bf1IsBindByShakle)
	{
		if	(m_wShakleSkill	==	0xffff)
			return	TRUE;

		CSkillExtraEffect	*lpSEE	=	g_aSkill[m_wShakleSkill].getExtraEffectByEffect(eSKILL_EE_MAKE_SHAKLE);

		if	(!lpSEE)
			return	TRUE;

		if	(lpSEE->m_aiValue[7])
			return	FALSE;
	}	

	if	(isSleep()+isStoned() + isStuned() + isFreeze() + isParalysis() + isNightMare() + isRockBounding() + m_enchantedImage.m_bf1IsBlockToMove+m_enchantedImage.m_bf1IsShutInMagicBox)
		return TRUE;

	return	FALSE;
}

BOOL
CActor::isStunedStatus(BOOL _bIsIgnoreNightmare)
{
	if (_bIsIgnoreNightmare)
	{
		if (isStoned() + isStuned() + isFreeze() + isParalysis())
			return TRUE;
	}
	else
	{
		if (isStoned() + isStuned() + isFreeze() + isParalysis() + isNightMare())
			return TRUE;
	}

	if	(m_wShakleSkill	!=	0xffff)
	{
		CSkillExtraEffect	*lpSEE	=	g_aSkill[m_wShakleSkill].getExtraEffectByEffect(eSKILL_EE_MAKE_SHAKLE);

		if	(lpSEE	&&	lpSEE->m_aiValue[4])
			return	TRUE;
	}


	return FALSE;
}

CJobInfo*
CActor::getJob()
{
	return	g_jm.getJob(m_wTrueJob);
}

BOOL
CActor::isVisibleName()
{
	int	iOperatorLevel	=	g_hero.m_wOperatorLevel;

#ifdef	_DEBUG
	if	(g_bIsHideDamageAndHideName)
		iOperatorLevel	=	0;
#endif

	if	(s_bIsGuildBattleField)
	{
		if	(isOwnTeam()	==	FALSE)
		{
			if	(isFocusActor() || isEngageTarget())
				return	TRUE;
			if	(isInvisivle()	||	isBlur())
				return	FALSE;
		}

		return	TRUE;
	}
	else
	{
		if	(isHero()||isPartyMember()||isNpc()||isHighOperator()||	iOperatorLevel	>=	2||g_hero.isPet(m_wSerial)||g_hero.isSummonBeastActor(m_wSerial) ||isFocusActor()|| isEngageTarget())
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CActor::isRequireAddPaint()
{
	BOOL	bIsHideTile	=	g_map.m_aMap[m_pos.mx+m_pos.my*g_map.m_iWidth] & 0x4000;

	if	(isControlMonster())
		return	FALSE;

	if	(isHero() && isBlind())
		return	FALSE;

	if	(bIsHideTile || isVisibleName() || m_wTalkTime)
		return	TRUE;

	return	FALSE;
}


BOOL
CActor::isInvisivle()
{
	if	(m_enchantedImage.m_bf1IsDisableInvisibleByAttack	&&	m_wRevealTime)
		return	FALSE;

	if	(isDeath())
		return	FALSE;

	if (m_dwEnchantedMask & dAMEM_INVISIBLE)
		return	TRUE;

	return	FALSE;
}

BOOL
CActor::isBlur()
{
	if	(isDeath())
		return	FALSE;

	if	(m_enchantedImage.m_bf1IsHwabyung)
		return	TRUE;
	if	(m_enchantedImage.m_bf1IsMadness)
		return	TRUE;
	if	(m_wRevealTime)
		return	FALSE;

	if	(m_dwEnchantedMask & dAMEM_BLUR)
		return	TRUE;

	return	FALSE;
}

char*
CActor::getName()
{
	if	(m_strName[0])
		return	m_strName;

	if	(m_wNpcActorSerial	>=	512)
		return	NULL;

	return	g_map.m_aSaveActor[m_wNpcActorSerial].m_strName;
}