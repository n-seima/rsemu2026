#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CGame.H"

//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
//	CSkill
//	스킬 관련
//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

CSkill				g_aSkill[dMAX_SKILL];
cFighterAttackInfo	g_fighterAttackInfo;

int
CSkill::getExtraEffectIndexByEffect(int _iEffect)
{
	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		if (m_aExtraEffect[i].m_wEffect	!=	_iEffect)
			continue;

		return	i;
	}

	return	0xffff;
}

CSkillExtraEffect*
CSkill::getExtraEffectByEffect(int _iEffect)
{
	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		if (m_aExtraEffect[i].m_wEffect	!=	_iEffect)
			continue;

		return	&m_aExtraEffect[i];
	}

	return	NULL;	
}

int
CSkill::getOrderingInSkillAi()
{
	for (int i=0;i<dMAX_SKILL_AI_PATTERN;i++)
	{
		if (m_aAi[i].m_wTrigger	==	eSACS_ORDERING)
			return	m_aAi[i].m_asValue[0];
	}
	
	return	0;
}

BOOL
CSkill::isAgressiveSkill()	//	공격적인 스킬이다.
{
	if	(m_bf1IsNonAggressiveSkill)
		return	FALSE;
	
	if	(m_wType	<	eSKILL_TYPE_AGRESSIVE_SKILL_BORDER)
		return	TRUE;
	
	if( m_wType == eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER)
		return TRUE;
	return	FALSE;
}


BOOL
CSkill::isCastOnCorpseSkill()
{
	if	(isResurrection())
		return	TRUE;

	if	(getExtraEffectByEffect(eSKILL_EE_ANIMATE_PARTNER))
		return	TRUE;

	return	FALSE;	
}

//
//	대상이 반드시 필요한 스킬
BOOL
CSkill::isRequireTargetSkill()
{
	BOOL	bIsUseToEmemy			=	isCastOnEnemySkill();
	BOOL	bIsUseToOwnForce		=	isCastOnPlayerSkill();
	BOOL	bIsUseToGround			=	isCastOnGroundSkill();
	BOOL	bIsUseQuick				=	isQuickCastSkill();
	BOOL	bIsUseToPet				=	isCastOnPetSkill()+isCastOnDeathPetSkill()+isCastOnEnemyPetSkill();
	BOOL	bIsUseToSummonBeast		=	isCastOnSummonBeastSkill();
	BOOL	bIsUseToParty			=	isCastOnPartyMemberSkill();
	BOOL	bIsUseToDeathPet		=	isCastOnDeathPetSkill();
	BOOL	bIsUseToDeathSummonBeast=	isCastOnDeathSummonBeastSkill();
//	BOOL	bIsUseToCaster			=	isCastOnCasterSkill();
	BOOL	bIsUseToUserPartyMember	=	isCastOnUserPartyMemberSkill();

	if	(bIsUseToGround	+bIsUseQuick	)
		return	FALSE;

	if	(bIsUseToOwnForce+bIsUseToEmemy+bIsUseToPet+bIsUseToParty+bIsUseToSummonBeast+bIsUseToDeathPet+bIsUseToDeathSummonBeast+bIsUseToUserPartyMember==	0	)
		return	FALSE;

	
	return	TRUE;
}

//
//	_iSerial번째거 제거
BOOL
CActiveSkillManager::remove(int _iSerial)
{
// 	if	(RemoveAS(_iSerial,&m_iRookie,&m_aActiveSkill[_iSerial].m_wSerial,&m_iActiveSkillCount))
// 	{
// 		m_aActiveSkill[_iSerial].reset();
// 
// 		return	TRUE;
// 	}
	if	(_iSerial	<	m_iRookie)
		m_iRookie	=	_iSerial;

	if	(m_aActiveSkill[_iSerial].m_wSerial	!= 0xffff)
	{
		m_aActiveSkill[_iSerial].reset();

		m_iActiveSkillCount	--;	
	}

	return	TRUE;
}

//
//	업데이트
void
CActiveSkillManager::update()
{
	mDS_AS.m_iActiveSkillManagerStep	=	0;

	for (int i=0,iCount=0;i<dMAX_ACTIVE_SKILL && iCount<m_iActiveSkillCount;i++)
	{
		if	(m_aActiveSkill[i].m_wSerial		==	0xffff)
			continue;

		mDS_AS.m_iCurrentSkill		=	m_aActiveSkill[i].m_ability.m_wSkill;
		mDS_AS.m_iCurrentSkillLevel	=	m_aActiveSkill[i].m_ability.m_wLevel;
		
		mDS_AS.m_iActiveSkillManagerStep	=	1;

		if	(m_aActiveSkill[i].update())
		{
			mDS_AS.m_iActiveSkillManagerStep	=	2;

			remove(i);

			mDS_AS.m_iActiveSkillManagerStep	=	3;
			continue;
		}

		mDS_AS.m_iActiveSkillManagerStep	=	4;
		iCount++;
	}

	mDS_AS.m_iActiveSkillManagerStep	=	-1;
	mDS_AS.m_iCurrentSkill				=	-1;
}

//
//	캐스터가 정지했다.
BOOL
CActiveSkillManager::casterStop(int _iSerial)
{
	if (m_aActiveSkill[_iSerial].casterStop())
	{
		if (_iSerial	<	m_iRookie)	m_iRookie	=	_iSerial;

		m_iActiveSkillCount--;	
	}

	return	TRUE;
}

//
//	캐스터가 죽었어.. -o-
void
CActiveSkillManager::actorDeath(int _iSerial)
{
	int	i,iCount;

	for (i=0,iCount=0;i<dMAX_ACTIVE_SKILL && iCount < m_iActiveSkillCount;i++)
	{
		CActiveSkill	*lpAciveSkill	=	&m_aActiveSkill[i];

		if (lpAciveSkill->m_wSerial	==	0xffff)	continue;

		if (lpAciveSkill->m_lpCaster)
			if (lpAciveSkill->m_lpCaster->m_wSerialInField	==	_iSerial)
				if (lpAciveSkill->casterDeath())
				{
					if (i < m_iRookie)	m_iRookie	=	i;

					m_iActiveSkillCount--;
					continue;
				}

		if (lpAciveSkill->m_lpTarget)
			if (lpAciveSkill->m_lpTarget->m_wSerialInField	==	_iSerial)
				if (lpAciveSkill->targetDeath())
				{
					if (i < m_iRookie)	m_iRookie	=	i;
					
					m_iActiveSkillCount--;
					continue;
				}

		iCount++;
	}
}	//	CActiveSkillManager::actorDeath(int _iSerial)