#include "CHero.H"
#include "Cmessage.H"

//
//	언제나 쓸 수 있는 디폴트 스킬
cAbility*
CHero::getDefaultSkill()
{
	for (int i=0;i<dABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];

		if (lpAbility->m_wSkill	==	0xffff)	continue;

		if (isUseAbleSkill(i)	==	eUSR_SUCCESS)	if (lpAbility->getSpentCP() == 0)	return	lpAbility;
	}

	return	NULL;
}	//	CHero::getDefaultSkill()

//
//	왼쪽 스킬이 비어 있을 경우 디폴트 스킬을 설정해 준다.
void
CHero::setLeftSkill()
{
	if	(m_aLeftAbility[(m_wJob)%2]	!=	0xffff)
		return;

	for (int i=0;i<dABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];

		if	(lpAbility->m_wSkill	==	0xffff)
			continue;

		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(!lpSkill->isEnableJob(m_wJob))
			continue;

		if	(!lpSkill->isEnableLeftSkill())
			continue;

		if (isUseAbleSkill(i) 	==	eUSR_SUCCESS)
		{
			m_wLeftAbility		=	i;
			m_aLeftAbility[(m_wJob)%2]	=	i;

			break;
		}
	}
}
void
CHero::setReactionAllOff()
{	// 모든 리액션끄기..
	int	iSlot	=	m_wJob%2;
	for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		m_aActivePassiveSkill[iSlot][i] = 0xffff;
	}
}

//
//	리액션 켜기
void
CHero::setReactionStatus(int _iAbility,BOOL _isOn)
{
	int	iSlot	=	m_wJob%2;

	if (_isOn)
	{
		for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
		{
			if (m_aActivePassiveSkill[iSlot][i] == 0xffff)
			{
				m_aActivePassiveSkill[iSlot][i]	=	_iAbility;
				return;
			}
		}

		m_aActivePassiveSkill[iSlot][0]	=	_iAbility;
	}
	else
	{
		for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
		{
			if (m_aActivePassiveSkill[iSlot][i] ==	_iAbility)
			{
				m_aActivePassiveSkill[iSlot][i]	=	0xffff;
				return;
			}
		}
	}
}

//
//	켜진 리액션이냐?
BOOL
CHero::isActiveReaction(int _iAbility)
{
	int	iSlot	=	m_wJob%2;

	for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
		if (m_aActivePassiveSkill[iSlot][i] == _iAbility)
			return	TRUE;

	return	FALSE;
}

//
//	패시브 스킬들 전부 해제
void
CHero::releaseAllPassiveSkill()
{
	memset(m_aActivePassiveSkill,0xff,sizeof(m_aActivePassiveSkill));
}	//	cACTOR_DATA::releaseAllReactionSkill()

//
//	패시브 스킬들 켜기
void
CHero::sendTurnOnPassiveSkill()
{
	int	iSlot	=	m_wJob%2,iCount=0;

	CG_TURN_ON_PASSIVE_SKILL	packet;

	packet.base.set(sizeof(CG_TURN_ON_PASSIVE_SKILL),dCG_TURN_ON_PASSIVE_SKILL);

	memset(packet.aPassiveSkill,0xff,sizeof(packet.aPassiveSkill));

	for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActivePassiveSkill[iSlot][i];

		if (iAbility	==	0xffff)	continue;

		if (m_aAbility[iAbility].isEnableJob(m_wJob)	==	FALSE)
		{
			m_aActivePassiveSkill[iSlot][i]	=	0xffff;
			continue;
		}

		packet.aPassiveSkill[iCount++]	=	iAbility;
	}

	if (iCount	==	0)	return;

	s_agent.sendTurnOnPassiveSkill(&packet);

	memset(m_aActivePassiveSkill[iSlot],0xff,sizeof(m_aActivePassiveSkill[iSlot]));
}	//	CHero::sendTurnOnPassiveSkill()

//
//	패시브 스킬들 켜기
void
CHero::turnOnPassiveSkill(BYTE *_lpSkillList)
{
	int	iSlot	=	m_wJob%2,i;

	memset(m_aActivePassiveSkill[iSlot],0xff,sizeof(m_aActivePassiveSkill[iSlot]));

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	_lpSkillList[i];

		if (_lpSkillList[i]==0xff)	continue;

		m_aActivePassiveSkill[iSlot][i]			=	iAbility;
	}
}	//	CHero::turnOnPassiveSkill(BYTE *_lpSkillList)

//
//	
cAbility*
CHero::getIcyStalagmiteSkill()
{
	for (int i=0;i<dABILITY_COUNT;i++)
	{
		CSkill	*lpSkill	=	m_aAbility[i].getSkill();

		if (!lpSkill)	continue;

		if (lpSkill->m_wType	==	eSKILL_TYPE_ICY_STALAGMITE)	return	&m_aAbility[i];
	}

	return	NULL;
}

cAbility*
CHero::getRidingDogSkill()
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

int
CHero::getMasterSkillDifficultySum(int _iJob)
{
	int	iAbility,iCorrectLevel=0;

	for (iAbility=0;iAbility<dABILITY_COUNT;iAbility++)
	{
		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if (!lpAbility->isEnableJob(_iJob))
			continue;

		CSkill		*lpSkill	=	lpAbility->getSkill();

		if (lpAbility->m_wLevel	<	dLIMIT_SKILL_LEVEL	||	lpSkill	==	NULL)
			continue;

		iCorrectLevel	+=	lpSkill->m_wDifficultyLevel;
	}

	return	iCorrectLevel;
}

//
//	컴플리트 프로텍션 얻기
BOOL
CHero::getCompleteProtectionAbility(cAbility *_lpAbility)
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


//	패시브 스킬에 의한 방어력 퍼센티지 증가 수치
int
CHero::getDefensePercentageByPassiveSkill()
{
	int	iDefensivePower	=	0;
	int	iSlot			=	m_wJob%2;

	for (int i=0;;i++)
	{
		int			j,iCount=0;
		cAbility	*lpAbility	=	NULL;

		for (j=0;j<dACTIVE_REACTION_COUNT;j++)
		{
			if	(m_aActivePassiveSkill[iSlot][j] !=	0xffff)
			{
				lpAbility	=	&m_aAbility[m_aActivePassiveSkill[iSlot][j]];

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

		CSkill	*lpSkill	=	lpAbility->getSkill();

		if	(!lpSkill	||	lpSkill->m_sActiveChance	<	100)
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
CHero::getDefensePowerByPassiveSkill()
{
	int	iDefensivePower	=	0;
	int	iSlot			=	m_wJob%2;

	for (int i=0;;i++)
	{
		int			j,iCount=0;
		cAbility	*lpAbility	=	NULL;

		for (j=0;j<dACTIVE_REACTION_COUNT;j++)
		{
			if (m_aActivePassiveSkill[iSlot][j] !=	0xffff)
			{
				lpAbility	=	&m_aAbility[m_aActivePassiveSkill[iSlot][j]];

				if (lpAbility->getSkill()->m_wCastMethod ==	eST_PASSIVE	&&	lpAbility->getDefensivePower() && lpAbility->isActivatePassiveSkill(m_iCP))
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

		CSkill	*lpSkill	=	lpAbility->getSkill();

		if	(!lpSkill	||	lpSkill->m_sActiveChance	<	100)
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

cAbility*
CHero::getSpecialFeatureSkill(int _iFeature)
{
	for (int i=0;i<dABILITY_COUNT;i++)
	{
		cAbility*	lpAbility	=	&m_aAbility[i];
		CSkill*		lpSkill		=	lpAbility->getSkill();

		if	(!lpSkill || !lpSkill->isEnableJob(m_wJob) || lpSkill->m_wSkillUniqueSpecialFeature	!=	_iFeature)
			continue;

		return	lpAbility;
	}

	return	NULL;
}


cAbility*
CHero::getDisarmTrapAbility()
{
	return	getSpecialFeatureSkill(eSUSF_IS_DISARM);
}

cAbility*
CHero::getUnlockDoorSkill()
{
	return	getSpecialFeatureSkill(eSUSF_IS_UNLOCK_DOOR);
}

cAbility*
CHero::getUnlockArcaSkill()
{
	return	getSpecialFeatureSkill(eSUSF_IS_UNLOCK_ARCA);
}

BOOL
CHero::isVisibleObject(int _iSecretLevel)
{
	if (_iSecretLevel	>	m_detectSecretObject.getCorrectedValue())
		return	FALSE;

	return	TRUE;
}

BOOL
CHero::isVisibleTrap(int _iTrapLevel)
{
	if (_iTrapLevel	>	m_detectTrap.getCorrectedValue())
		return	FALSE;

	return	TRUE;
}

int
CHero::getCoolTime(cAbility *_lpAbility)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(!lpSkill)
		return	0;

	if	(lpSkill->m_wCoolTimeIndex	>=	c_iCoolTimeSkillCount)
		return	0;

	return	m_awSkillCoolTime[lpSkill->m_wCoolTimeIndex];
}