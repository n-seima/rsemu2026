#include "cACTOR.H"
#include "cFIELD.H"

//
//	활성화된 향상된 막기 스킬 구함
BOOL
cACTOR::getActiveAdvancedBlockingSkill(cAbility *_lpAbility)
{
	int	i,iBlockingChance=0;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int			iAbility	=	m_aActiveReaction[i];

		if (iAbility	==	0xffff)	continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if (lpSkill->m_wType	!=	eSKILL_TYPE_ADVANCED_BLOCKING)	continue;
		if (!lpAbility->isActivatePassiveSkill(m_iCP))	continue;

		_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
		_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(_lpAbility);

		return	TRUE;
	}

	return	FALSE;
}

//
//	댄싱 블로커에 의한 블럭률을 구한다.
int
cACTOR::getBlockingChanceByDancingBlocker()
{
	cAbility	*lpAbility	=	getDancingBlockerSkill();

	if (!lpAbility)
		return	0;

	if (!lpAbility->isActivatePassiveSkill(m_iCP))
		return	0;

	return	lpAbility->getCorrectBlockingChance();
}

void
cACTOR::operatePassiveSkill()		// 패시브스킬들저장..
{
	int	iSlot = (m_wJob%2)*25;
	
	if(!isPlayer())
		return ;
	memset(m_aPassiveSkill,0xffff,sizeof(m_aPassiveSkill));		// 패시브스킬들.
	int iCount = 0;
	for(int i=iSlot;i<iSlot+25;++i)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];
		if(!lpAbility)
			continue;
		CSkill* lpSkill = lpAbility->getSkill();
		if(!lpSkill)
			continue;
		if(lpSkill->m_wType == eSKILL_TYPE_PASSIVE)
		{
			m_aPassiveSkill[iCount++] = i;
		}

		if(iCount>=dPASSIVE_SKILL_COUNT)
			break;
	}	
}

int
cACTOR::getValuePassiveSkill(int _iKind, int _iValueIndex)
{
	int	iValue = 0;

	if(!isPlayer())
		return iValue;
	for(int i=0;i<dPASSIVE_SKILL_COUNT;++i)
	{
		if(m_aPassiveSkill[i] == 0xffff)
			break;
		if(m_aPassiveSkill[i] >= dABILITY_COUNT)
			continue;
		cAbility	*lpAbility	=	&m_aAbility[m_aPassiveSkill[i]];
		CSkill		*lpSkill	=	lpAbility->getSkill();
		if(!lpSkill)
			continue;
		if (lpSkill->m_aValue[0]	!=	_iKind)
			continue;
		if(lpAbility->m_wLevel == 0)
			break;
		int iLevel	=	lpAbility->m_wLevel + getCorrectSkillLevel(lpAbility);
		if(_iValueIndex == 1)
		{
			iValue += lpSkill->m_aValue[1] + lpSkill->m_aValue[2] * iLevel;
			if(lpSkill->m_aValue[3] != 0)
				iValue = min(iValue, lpSkill->m_aValue[3]);
		}
		else if(_iValueIndex == 2)
		{
			iValue += lpSkill->m_aValue[4] + lpSkill->m_aValue[5] * iLevel;
			if(lpSkill->m_aValue[6] != 0)
				iValue = min(iValue, lpSkill->m_aValue[6]);
		}

		break;
	}

	return	iValue;
}



//
//	댄싱 블로커를 구한다.
cAbility*
cACTOR::getDancingBlockerSkill()
{
	int	i,iBlockingChance=0;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if (iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if (lpSkill->m_wType	!=	eSKILL_TYPE_DANCING_BLOCKER)
			continue;

		return	lpAbility;
	}

	return	0;
}

//
//	녹아웃 방어하는 스킬 찾기
BOOL
cACTOR::getDefenseNockoutPassiveSkill(cAbility *_lpAbility)
{
	int	i;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if (iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if	(lpAbility->getSkill()->m_wCastMethod	!=	eST_PASSIVE)
			continue;
		if	(!lpAbility->isDefenseKnockout()				)
			continue;
		if	(!lpAbility->isActivatePassiveSkill(m_iCP)	)
			return	FALSE;

		_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
		_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(_lpAbility);

		return	TRUE;
	}

	return	FALSE;
}

//
//	스턴을 방어하는 스킬 찾기
BOOL
cACTOR::getDefenseStunPassiveSkill(cAbility *_lpAbility)
{
	int	i;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if (iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if (lpAbility->getSkill()->m_wCastMethod	!=	eST_PASSIVE	)
			continue;
		if (lpAbility->getStunResistance()	==	0	)
			continue;
		if (!lpAbility->isActivatePassiveSkill(m_iCP)	)
			return	FALSE;

		_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
		_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(_lpAbility);
		
		return	TRUE;
	}

	return	FALSE;
}

//
//	스턴을 방어하는 스킬 찾기
BOOL
cACTOR::getDefenseStrangeStatusPassiveSkill(cAbility *_lpAbility)
{
	int	i;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if (iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if (lpAbility->getSkill()->m_wCastMethod	!=	eST_PASSIVE	)
			continue;
		if (lpAbility->getStrangeStatusResistance()	==	0			)
			continue;
		if (!lpAbility->isActivatePassiveSkill(m_iCP)				)
			return	FALSE;

		_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
		_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);

		return	TRUE;
	}

	return	FALSE;
}

BOOL
cACTOR::getMonsterActivatePassiveSkill(cAbility *_lpAbility,int _iActivateTrigger,int _iIndex,cACTOR *_lpAttacker)
{
	int	i,iCount=0;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if (iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if	(lpAbility->getSkill()->m_wActivateTrigger != _iActivateTrigger	)
			continue;

		if	(_iIndex	==	iCount)
		{
			_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
			_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);

			if	(checkIsSuitableSkill(lpAbility,_lpAttacker,FALSE))
				return	TRUE;

			return	FALSE;
		}

		iCount++;
	}

	return	FALSE;
}

//
//	맞는 순간 발동하는 패시브 스킬 찾기
BOOL
cACTOR::getActivatePassiveSkill(cAbility *_lpAbility,int _iActivateTrigger,int _iIndex,BOOL _bIsIgnoreCP,cACTOR *_lpAttacker)
{
	if	(isPureMonster())
		return	getMonsterActivatePassiveSkill(_lpAbility,_iActivateTrigger,_iIndex,_lpAttacker);

	int	i,iCount=0;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if	(iAbility	>=	dABILITY_COUNT)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if	(!lpAbility->getSkill() || lpAbility->getSkill()->m_wActivateTrigger != _iActivateTrigger	)
			continue;

		if	(!lpAbility->isActivatePassiveSkill(m_iCP)	&& _bIsIgnoreCP==	FALSE)
			continue;

		if	(_iIndex	==	iCount)
		{
			_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
			_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);
			
			return	TRUE;
		}

		iCount++;
	}

	return	FALSE;
}

//
//	상태이상 공격에 의한 저항
int
cACTOR::getStrangeResistanceByPassiveSkill()
{
	cAbility ability;

	if (!getDefenseStrangeStatusPassiveSkill(&ability))	return	0;

	int	iActivateChance	=	ability.getActivateChance();

	if (iActivateChance)
		if (random(100) > iActivateChance)
			return	0;

	return	ability.getStrangeStatusResistance();
}

//
//	패시브 스킬에 의한 저항
int
cACTOR::getStunResistanceByPassiveSkill()
{
	cAbility ability;

	if (!getDefenseStunPassiveSkill(&ability))
		return	0;

	int	iActivateChance	=	ability.getActivateChance();

	if (iActivateChance)
		if (random(100) > iActivateChance)
			return	0;

	return	ability.getStunResistance();
}

//
//	패시브 스킬에 의한 저항
int
cACTOR::getNockOutResistanceByPassiveSkill()
{
	cAbility ability;

	if	(!getDefenseNockoutPassiveSkill(&ability))
		return	0;

	return	ability.getActivateChance();
}

//	패시브 스킬에 의한 방어력 퍼센티지 증가 수치
int
cACTOR::getDefensePercentageByPassiveSkill()
{
	int	iDefensivePower=0;

	for (int i=0;;i++)
	{
		int			j,iCount=0;
		cAbility	*lpAbility	=	NULL;

		for (j=0;j<dACTIVE_REACTION_COUNT;j++)
		{
			if	(m_aActiveReaction[j] !=	0xffff)
			{
				lpAbility	=	&m_aAbility[m_aActiveReaction[j]];

				if	(lpAbility->m_wSkill	==	0xffff)
					continue;

				if	(lpAbility->getSkill()->m_wCastMethod ==	eST_PASSIVE	&&	lpAbility->getDefensivePowerPercentage() && lpAbility->isActivatePassiveSkill(m_iCP))
				{
					if	(i == iCount)
						break;
					else
						lpAbility = NULL;
				}
				else
					lpAbility = NULL;
			}
		}

		if	(lpAbility	==	NULL)
			break;

		int	iActivateChance	=	lpAbility->getActivateChance();

		if	(random(100) > iActivateChance)
			continue;

		iDefensivePower	+=	lpAbility->getDefensivePowerPercentage()/100;
	}

	if	(isActiveCompleteProtection())
	{
		cAbility	completeProtection;

		if	(getCompleteProtectionAbility(&completeProtection))
			iDefensivePower	+=	completeProtection.getDefensivePowerPercentage()/100;
	}

	return	iDefensivePower;
}

//
//	패시브 스킬에 의한 저항
int
cACTOR::getDefensePowerByPassiveSkill()
{
	int	iDefensivePower=0;

	for (int i=0;;i++)
	{
		int			j,iCount=0;
		cAbility	*lpAbility	=	NULL;

		for (j=0;j<dACTIVE_REACTION_COUNT;j++)
		{
			if	(m_aActiveReaction[j] !=	0xffff)
			{
				lpAbility	=	&m_aAbility[m_aActiveReaction[j]];

				if	(lpAbility->m_wSkill	==	0xffff)
					continue;

				if	(lpAbility->getSkill()->m_wCastMethod ==	eST_PASSIVE	&&	lpAbility->getDefensivePower() && lpAbility->isActivatePassiveSkill(m_iCP))
				{
					if (i == iCount)
						break;
					else
						lpAbility = NULL;
				}
				else
					lpAbility = NULL;
			}
		}

		if	(lpAbility	==	NULL)
			break;

		int	iActivateChance	=	lpAbility->getActivateChance();

		if	(random(100) > iActivateChance)
			continue;

		iDefensivePower	+=	lpAbility->getDefensivePower();
	}

	if	(isActiveCompleteProtection())
	{
		cAbility	completeProtection;

		if	(getCompleteProtectionAbility(&completeProtection))
			iDefensivePower	+=	completeProtection.getDefensivePower();
	}

	return	iDefensivePower;
}

//
//	패시브 스킬에 의한 회피율
int
cACTOR::getDodgeChanceByPassiveSkill()
{
	int	iDodgeChance=0;

	for (int i=0;;i++)
	{
		int			j,iCount=0;
		cAbility	*lpAbility	=	NULL;

		for (j=0;j<dACTIVE_REACTION_COUNT;j++)
		{
			if (m_aActiveReaction[j] !=	0xffff)
			{
				lpAbility	=	&m_aAbility[m_aActiveReaction[j]];
				
				if	(lpAbility->m_wSkill	==	0xffff)
					continue;

				if (lpAbility->getSkill()->m_wCastMethod ==	eST_PASSIVE	&&	lpAbility->getDodgeChance() && lpAbility->isActivatePassiveSkill(m_iCP))
				{
					if (i == iCount)
						break;
					else
						lpAbility = NULL;
				}	
				else
					lpAbility = NULL;
			}
		}

		if (lpAbility	==	NULL)
			break;

		int	iActivateChance	=	lpAbility->getActivateChance();

		if (random(100) > iActivateChance)
			continue;

		iDodgeChance	+=	lpAbility->getDodgeChance();
	}

	return	iDodgeChance;
}

//
//	패시브 스킬에 의한 블럭률을 구한다.
int
cACTOR::getBlockingChanceByAdvancedBlocking()
{
	int	iBlockingChance=0;

	for (int i=0;;i++)
	{
		int			j,iCount=0;
		cAbility	*lpAbility	=	NULL;

		for (j=0;j<dACTIVE_REACTION_COUNT;j++)
		{
			if (m_aActiveReaction[j] !=	0xffff)
			{
				lpAbility	=	&m_aAbility[m_aActiveReaction[j]];

				if	(lpAbility->m_wSkill	==	0xffff)
					continue;

				if	(lpAbility->getSkill()->m_wType ==	eSKILL_TYPE_ADVANCED_BLOCKING && lpAbility->isActivatePassiveSkill(m_iCP))
				{
					if (i == iCount)
						break;
					else
						lpAbility = NULL;
				}	
				else
					lpAbility = NULL;
			}
		}

		if (lpAbility	==	NULL)
			break;

		int	iActivateChance	=	lpAbility->getActivateChance();

		if (random(100) > iActivateChance)
			continue;

		iBlockingChance	=	max(iBlockingChance,lpAbility->getCorrectBlockingChance());
	}

	return	iBlockingChance;
}