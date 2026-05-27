#include <math.h>
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include "cGuild.H"
#include "packetManager.H"

void
cACTOR::operateInstanceApplyAura()
{
	cACTOR	*lpOwner;

	for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		cAbility	aura;

		if (!getActiveAura(&aura,i))
			continue;

		lpOwner			=	this;

		CSkill	*lpSkill=	aura.getSkill();

		if (!lpSkill->m_bf1IsInstanceApplyAura)
			continue;

		aura.m_wLevel	+=	getCorrectSkillLevel(&aura);

		if (lpSkill->m_bf2RequireSummonBeastGrade)
		{
			lpOwner	=	getSummonBeastByType(lpSkill->m_bf2OperateSummonBeast);

			if (!lpOwner)
				continue;

			if (lpOwner->m_wSummonBeastGrade+1	<	(WORD)lpSkill->m_bf2RequireSummonBeastGrade)
				continue;
			
			if (lpOwner->isDeath())
				continue;
		}

		m_aAuraSkillTimer[i]=	0;

		m_lpField->operateAuraSkill(&aura,lpOwner->m_pos.x,lpOwner->m_pos.y,aura.getHitRange(NULL),lpOwner);
	}
}

//
//	오러 스킬 처리
void
cACTOR::operateMonsterAuraSkill()
{
	if	(isDeath())
		return;

	if	(m_wAiState	==	eAS_READY	&&	m_iGuildGuardianType	==	0)
		return;

	cAbility	ability;

	for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if	(!lpAbility->isAuraSkill())
			continue;

		memcpy(&ability,lpAbility,sizeof(cAbility));

		lpAbility			=	&ability;
		lpAbility->m_wLevel	+=	getCorrectSkillLevel(lpAbility);

		CSkill	*lpSkill	=	lpAbility->getSkill();

		if	(!lpSkill)
			continue;

		m_aAuraSkillTimer[i]++;

		if	(m_aAuraSkillTimer[i]	>=	lpAbility->getApplyAuraPeriod())
		{
			m_aAuraSkillTimer[i]	=	0;

			int	iRange				=	lpAbility->getHitRange(NULL);

			if	(checkIsSuitableSkill(lpAbility,m_lpTarget,FALSE))
			{
				m_lpField->operateAuraSkill(lpAbility,m_pos.x,m_pos.y,iRange,this);

				if	(lpSkill->isExitEnchantEffect())
				{
					mDS.m_aiExValue[3]	=	1;
					addEnchantedMagic(lpAbility,dSYNC_FPS*lpAbility->getApplyAuraPeriod()+dSYNC_FPS/2);
				}
			}
		}
	}
}

//
//	오러 스킬 처리
void
cACTOR::operateAuraSkill()
{
	if	(m_lpField->m_bIsGuildBattleField	&&	m_lpField->m_bIsSiegeWarfareField	==	FALSE)
		if	(m_lpField->m_wGuildBattleStatus != eGB_BATTLE && m_lpField->m_wGuildBattleStatus != eGB_READY_TO_BATTLE_FINISH)
			return;
	
	if	(isIdlePlayer() || isDeath())// || m_wIsFeignDeath)
		return;

	BOOL		bIsCorrectSkill	=	FALSE;

	cACTOR		*lpOwner		=	NULL;
	cAbility	ability;

	for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		lpOwner			=	this;

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if	(!lpAbility->isEnableJob(m_wJob))
		{
			m_aActiveReaction[i]	=	0xffff;
			bIsCorrectSkill			=	TRUE;
			continue;
		}

		if	(!lpAbility->isAuraSkill())
			continue;

		memcpy(&ability,lpAbility,sizeof(cAbility));

		lpAbility			=	&ability;
		lpAbility->m_wLevel	+=	getCorrectSkillLevel(lpAbility);

		CSkill	*lpSkill	=	lpAbility->getSkill();

		if	(!lpSkill)
			continue;
		
		if	(m_wFeignDeathTime	||	m_wIsFeignDeath)
		{
			if	(lpSkill->m_bf1IsRequireFeignDeathStatus	==	FALSE)
				continue;
		}
		else
		{
			if	(lpSkill->m_bf1IsRequireFeignDeathStatus)
				continue;
		}

		if	(m_wRabbitTime || m_wJumppingSkippingTime || m_wTransToWeaponTime || m_wTransToFatGirlTime || m_wTransToFrogTime	||	m_wHwabyungTime	||	m_wZombieBombPower)
			continue;

		if	(lpSkill->m_bf2RequireSummonBeastGrade)
		{
			lpOwner	=	getSummonBeastByType(lpSkill->m_bf2OperateSummonBeast);

			if (!lpOwner)
				continue;

			if (lpOwner->m_wSummonBeastGrade+1	<	(WORD)lpSkill->m_bf2RequireSummonBeastGrade)
				continue;
			
			if (lpOwner->isDeath())
				continue;
		}

		if	(lpSkill->isExitEnchantEffect())
		{
			mDS.m_aiExValue[3]	=	2;
			lpOwner->addEnchantedMagic(lpAbility,0);
		}

		if	(!lpAbility->isActivatePassiveSkill(m_iCP))
			continue;

		if	(lpSkill->isExitEnchantEffect())
		{
			mDS.m_aiExValue[3]	=	3;
			lpOwner->addEnchantedMagic(lpAbility,dSYNC_FPS+2);
		}

		m_aAuraSkillTimer[i]++;

		if (m_aAuraSkillTimer[i]	>=	lpAbility->getApplyAuraPeriod())
		{
			m_aAuraSkillTimer[i]	=	0;

			int	iRange		=	lpAbility->getHitRange(NULL);

			m_lpField->operateAuraSkill(lpAbility,lpOwner->m_pos.x,lpOwner->m_pos.y,iRange,lpOwner);
		}
	}

	if (bIsCorrectSkill)
	{
		SG_TURN_ON_PASSIVE_SKILL	packet;
		int	iCount	=	0;

		packet.base.set(sizeof(SG_TURN_ON_PASSIVE_SKILL),dSG_TURN_ON_PASSIVE_SKILL);
		memset(packet.aPassiveSkill,0xff,sizeof(packet.aPassiveSkill));

		for (i=0;i<dACTIVE_REACTION_COUNT;i++)
		{
			int	iAbility	=	m_aActiveReaction[i];

			if (iAbility	==	0xffff	)	continue;

			packet.aPassiveSkill[iCount++]	=	iAbility;
		}

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
	}
}

//
//	활성화된 오러 찾기
BOOL
cACTOR::getActiveAura(cAbility *_lpAbility,int _iIndex)
{
	int		iAuraCount		=	0;

	for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if (iAbility	==	0xffff	||	iAbility	>=	dABILITY_COUNT)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if	(isPureMonster()	==	FALSE)
		{
			if	(!lpAbility->isEnableJob(m_wJob))
			{
				m_aActiveReaction[i]	=	0xffff;
				continue;
			}

			if	(!lpAbility->isActivatePassiveSkill(m_iCP))
				continue;
		}

		if	(!lpAbility->isAuraSkill()				)
			continue;

		if (iAuraCount	==	_iIndex)
		{
			_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
			_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);

			return	TRUE;
		}

		iAuraCount++;
	}

	return	FALSE;
}