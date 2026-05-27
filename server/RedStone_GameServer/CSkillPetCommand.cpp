#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"

//
//	
int
CActiveSkill::firePetCommand()
{
	int	i;

	if	(m_lpSkill->m_wPetCommand	==	ePC_USE_SKILL)	//	스킬 사용 명령이라면 아래 작업은 필요 없다.
	{
		BOOL	bExistButBusy	=	FALSE;

		for (i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
		{
			cACTOR	*lpSummonBeast	=	m_lpCaster->getSummonBeastActor(i);

			if	(!lpSummonBeast || lpSummonBeast->isDeath() || lpSummonBeast->m_wRidingDogTime)
				continue;

			if	(lpSummonBeast->m_wSummonBeastType	!=	m_lpSkill->m_bf2OperateSummonBeast)
				continue;

			if	(lpSummonBeast->m_wSummonBeastGrade	<	m_lpSkill->m_bf2RequireSummonBeastGrade-1)
				continue;

			if	(lpSummonBeast->m_wProhibitActionByEnchantedMagic	!=	0xffff)
				continue;

			if	(lpSummonBeast->m_wUseSpecialAbilityStep	!=	ePUSS_NONE)
			{
				bExistButBusy	=	TRUE;
				continue;
			}

			m_lpCaster->action(m_lpSkill->m_wAction);

			getActionTime();
			setWaitCommandTime(FALSE);

			return	eSKILL_FIRE_RESULT_OK;
		}

		if	(bExistButBusy	==	FALSE)
			m_lpCaster->sendRegistServerMessage(eRSM_NOT_EXIST_SUMMON_BEAST_USE_THAT_SKILL,eSM_SYSTEM);

		return	eSKILL_FIRE_RESULT_BY_BUSY;
	}

	if (m_lpSkill->m_wPetCommand	==	ePC_SPECIAL_ACTION)
	{
		BOOL	bExistSpecialAttackPet	=	FALSE;

		if	(m_lpTarget && (m_lpTarget->isOwnForce(m_lpCaster) || m_lpTarget->isNpc()) )
			return	eSKILL_FIRE_RESULT_FAILED;

		for (int i=0;i<dOWN_PET_COUNT;i++)
		{
			cACTOR	*lpPet	=	m_lpCaster->getPetActor(i);

			if	(!lpPet)
				continue;

			if	(lpPet->m_specialAbility.m_wSkill	!=	0xffff)
				bExistSpecialAttackPet	=	TRUE;
		}
	}

	int	iReceiveCommandActor	=	0;

	for (i=0;i<dOWN_PET_COUNT;i++)
	{
		cACTOR	*lpPet	=	m_lpCaster->getPetActor(i);

		if	(!lpPet || lpPet->isDeath())
		{
			setPetCommandWaitTime(i,0xffff);
			continue;
		}

		iReceiveCommandActor++;

		int	iWaitTime	=	dSYNC_FPS*2-lpPet->m_wPetLoyaltyValue*dSYNC_FPS*150/100/1000;

		iWaitTime		=	max(iWaitTime,0);

		setPetCommandWaitTime(i,iWaitTime);
	}

	for (i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		cACTOR	*lpSummonBeast	=	m_lpCaster->getSummonBeastActor(i);

		if	(!lpSummonBeast || lpSummonBeast->m_wRidingDogTime || lpSummonBeast->isDeath() || lpSummonBeast->m_wRidingDogTime)
		{
			setPetCommandWaitTime(i+dOWN_PET_COUNT,0xffff);
			continue;
		}

		iReceiveCommandActor++;

		int	iWaitTime	=	1;

		iWaitTime		=	max(iWaitTime,0);

		setPetCommandWaitTime(i,iWaitTime);
	}

	if (iReceiveCommandActor	<=	0)
	{
		m_lpCaster->sendRegistServerMessage(eRSM_NOT_EXIST_SUMMON_BEAST_USE_THAT_SKILL,eSM_SYSTEM);

		return	eSKILL_FIRE_RESULT_FAILED;
	}

	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();
	setWaitCommandTime(FALSE);

	return	eSKILL_FIRE_RESULT_OK;
}

void
CActiveSkill::operatePetCommand(cACTOR *_lpPet)
{
	if	(_lpPet->isDeath())
		return;

	switch(m_lpSkill->m_wPetCommand)
	{
		case	ePC_ATTACK			:
			if (m_lpTarget)
				_lpPet->commandTamedMonster(eTMD_ATTACK_ONE,m_lpTarget);
			else
				_lpPet->commandTamedMonster(eTMD_ATTACK_ALL,m_lpTarget);
			break;
		case	ePC_DEFENSE			:
			_lpPet->commandTamedMonster(eTMD_DEFENSE_POSITION,NULL,m_posTarget.x,m_posTarget.y);
			break;
		case	ePC_PATROL			:
			_lpPet->commandTamedMonster(eTMD_PATROL,m_lpTarget);
			break;
		case	ePC_SPECIAL_ACTION	:
			_lpPet->commandTamedMonster(eTMD_ATTACK_ONE,m_lpTarget);
			if	(_lpPet->m_specialAbility.m_wSkill	==	0xffff)
				break;
			_lpPet->setPetUseSpecialAttack(m_ability.getUpkeepTime());
			break;
	}

	if (m_lpTarget)
		m_lpTarget->applySkillExtraEffects(m_lpCaster,&m_ability);

	_lpPet->applySkillExtraEffects(m_lpCaster,&m_ability);
}

//
//	캐스팅
BOOL
CActiveSkill::castPetCommand()
{
	int	iWaitCommandPetCount	=	0,i;

	if (m_lpSkill->m_wPetCommand	==	ePC_USE_SKILL)	//	스킬 사용 명령이라면 아래 작업은 필요 없다.
	{
		for (i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
		{
			cACTOR	*lpSummonBeast	=	m_lpCaster->getSummonBeastActor(i);

			if	(!lpSummonBeast || lpSummonBeast->isDeath() || lpSummonBeast->m_wRidingDogTime)
				continue;

			if	(lpSummonBeast->m_wSummonBeastType	!=	m_lpSkill->m_bf2OperateSummonBeast)
				continue;

			if	(lpSummonBeast->m_wSummonBeastGrade	<	m_lpSkill->m_bf2RequireSummonBeastGrade-1)
				continue;

			if	(lpSummonBeast->m_wProhibitActionByEnchantedMagic	!=	0xffff)
				continue;

			lpSummonBeast->operateCommandSkill(m_lpSkill->m_wCommandSkill,m_ability.m_wLevel,m_lpTarget,m_posTarget.x,m_posTarget.y);

			m_lpCaster->m_wLastCommandSummonedBeast	=	lpSummonBeast->m_wSerialInField;
			m_lpCaster->increaseCP(m_ability.getGetCP());
			m_lpCaster->sendPlayerBasicInformation();

			return	TRUE;
		}

		m_lpCaster->sendRegistServerMessage(eRSM_NOT_EXIST_SUMMON_BEAST_USE_THAT_SKILL,eSM_SYSTEM);

		return	TRUE;
	}

	for (i=0;i<dOWN_PET_COUNT;i++)
	{
		cACTOR	*lpPet	=	m_lpCaster->getPetActor(i);

		if (!lpPet || lpPet->isDeath())
		{
			setPetCommandWaitTime(i,0xffff);
			continue;
		}

		if (isCommandTime(i))
		{
			operatePetCommand(lpPet);

			continue;
		}

		m_lpCaster->sendPetWaitCommand(i);
		iWaitCommandPetCount++;
	}

	for (i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		cACTOR	*lpSummonBeast	=	m_lpCaster->getSummonBeastActor(i);

		if (!lpSummonBeast || lpSummonBeast->isDeath() || lpSummonBeast->m_wRidingDogTime)
		{
			setPetCommandWaitTime(i+dOWN_PET_COUNT,0xffff);
			continue;
		}

		operatePetCommand(lpSummonBeast);
	}

	if (m_lpCaster->getFollowMonsterCount())
	{
		m_lpCaster->increaseCP(m_ability.getGetCP());
		m_lpCaster->sendPlayerBasicInformation();
	}
	else
		m_lpCaster->sendRegistServerMessage(eRSM_NOT_EXIST_CONTROL_PET,eSM_SYSTEM);

	if (iWaitCommandPetCount)
	{
		setWaitCommandTime(TRUE);

		return	FALSE;
	}

	return	TRUE;
}

//
//	
BOOL
CActiveSkill::updatePetCommand()
{
	if (!isActionTime())
		return	FALSE;

	if (isWaitCommandTime())
	{
		BOOL	bIsRemainWaitCommandPet	=	FALSE;
		int		i;

		for (i=0;i<dOWN_PET_COUNT;i++)
		{
			if (getPetCommandWaitTime(i)	==	0xffff)
				continue;

			cACTOR	*lpPet	=	m_lpCaster->getPetActor(i);

			if (!lpPet)
				continue;

			if (isCommandTime(i))
			{
				operatePetCommand(lpPet);
				continue;
			}

			bIsRemainWaitCommandPet	=	TRUE;
		}

		return	1-bIsRemainWaitCommandPet;
	}
	else
		return	castPetCommand();

	return	FALSE;
}