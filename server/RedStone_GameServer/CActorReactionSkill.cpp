#include "cACTOR.H"
#include "cFIELD.H"
#include "CSkill.H"

//
//	데미지 리턴
BOOL
cACTOR::getMonsterActivateReaction(cAbility *_lpAbility,int _iActivateTrigger,int _iIndex,cACTOR *_lpAttacker)
{
	int	i,iCount=0;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if	(lpAbility->getSkill() == NULL || lpAbility->getSkill()->m_wActivateTrigger != _iActivateTrigger	)
			continue;

		if	(_iIndex	==	iCount)	
		{
			_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
			_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);

			return	checkIsSuitableSkill(lpAbility,_lpAttacker,FALSE);
		}

		iCount++;
	}

	return	FALSE;
}

//
//	데미지 리턴
BOOL
cACTOR::getActivateReaction(cAbility *_lpAbility,int _iActivateTrigger,int _iIndex,cACTOR *_lpAttacker)
{
	if	(isPureMonster())
		return	getMonsterActivateReaction(_lpAbility,_iActivateTrigger,_iIndex,_lpAttacker);

	if	(isIdlePlayer() || m_wIsFeignDeath)
		return	FALSE;

	int	i,iCount=0;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(!isMonster()	&&	!lpAbility->isActivateReactionSkill(GetCPLevel(m_iCP))	)
			continue;
		if	(lpSkill->m_wActivateTrigger != _iActivateTrigger	)
			continue;
		if	(lpSkill->m_bf2RequireSummonBeastGrade)
		{
			cACTOR	*lpBeast	=	getSummonBeastByType(lpSkill->m_bf2OperateSummonBeast);

			if	(!lpBeast)
				continue;

			if	(lpBeast->m_wSummonBeastGrade+1	<	(WORD)lpSkill->m_bf2RequireSummonBeastGrade)
				continue;
			
			if	(lpBeast->isDeath())
				continue;
		}

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
//	테일 체이서가 켜져 있냐?
BOOL
cACTOR::getActiveTailChaser(cAbility *_lpAbility)
{
	if	(isIdlePlayer() || m_wIsFeignDeath)
		return	FALSE;

	int	i,iBlockingChance=0;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int			iAbility	=	m_aActiveReaction[i];

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(lpSkill->m_wType	!=	eSKILL_TYPE_TAIL_CHASER			)
			continue;
		if	(!lpAbility->isActivateReactionSkill(GetCPLevel(m_iCP))	)
			return	FALSE;

		_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
		_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);
		
		return	TRUE;
	}

	return	FALSE;
}

//
//	활성화된 블럭킹 스킬을 찾아라!!
BOOL
cACTOR::getActiveBlockingSkill(cAbility *_lpAbility)
{
	if	(isIdlePlayer() || m_wIsFeignDeath)
		return	FALSE;

	int	i,iBlockingChance=0;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(lpSkill->m_wType	!=	eSKILL_TYPE_BLOCKING			)
			continue;
		if	(!lpAbility->isActivateReactionSkill(GetCPLevel(m_iCP))	)
			return	FALSE;

		_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
		_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);
		
		return	TRUE;
	}

	return	FALSE;
}

//
//	활성화된 닫지 스킬
BOOL
cACTOR::getActiveDodgeSkill(cAbility *_lpAbility,int _iCount)
{
	if	(isIdlePlayer() || m_wIsFeignDeath)
		return	FALSE;

	int	i,iBlockingChance=0,iDodgeSkillCount=0;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int			iAbility	=	m_aActiveReaction[i];

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(lpSkill->m_wType	!=	eSKILL_TYPE_DODGE				)
			continue;

		if	(iDodgeSkillCount	>=	_iCount)
		{
			if	(!lpAbility->isActivateReactionSkill(GetCPLevel(m_iCP))	)
				return	FALSE;

			_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
			_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);
			
			return	TRUE;
		}

		iDodgeSkillCount++;
	}

	return	FALSE;
}

//
//	활성화된 돌려 일어나기
BOOL
cACTOR::getActiveSwingArising(cAbility *_lpAbility)
{
	if	(isIdlePlayer() || m_wIsFeignDeath)
		return	FALSE;

	int	i;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int			iAbility	=	m_aActiveReaction[i];

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(lpSkill->m_wType	!=	eSKILL_TYPE_SPIN_ARISING		)
			continue;
		if	(!lpAbility->isActivateReactionSkill(GetCPLevel(m_iCP))	)
			return	FALSE;

		_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
		_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);

		return	TRUE;
	}

	return	FALSE;
}

//
//	활성화된 아머 디스어셈블러
BOOL
cACTOR::getActiveArmorDisassemler(cAbility *_lpAbility)
{
	if	(isIdlePlayer() || m_wIsFeignDeath)
		return	FALSE;

	int	i;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int			iAbility	=	m_aActiveReaction[i];

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(lpSkill->m_wType	!=	eSKILL_TYPE_ARMOR_DISASSEMBLER	)
			continue;
		if	(!lpAbility->isActivateReactionSkill(GetCPLevel(m_iCP))	)
			return	FALSE;

		_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
		_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);

		return	TRUE;
	}

	return	FALSE;
}

//
//	활성화된 카운터 스킬
BOOL
cACTOR::getActiveCounterSkill(cAbility *_lpAbility,int _iCount)
{
	if	(isIdlePlayer() || m_wIsFeignDeath)
		return	FALSE;

	int	i,iCount = 0;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(lpSkill->m_wType	!=	eSKILL_TYPE_COUNTER				)
			continue;
		if	(!lpAbility->isActivateReactionSkill(GetCPLevel(m_iCP))	)
			return	FALSE;

		if	(iCount	==	_iCount)
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
//	녹아웃 방어하는 스킬 찾기
BOOL
cACTOR::getDefenseNockoutReactionSkill(cAbility *_lpAbility)
{
	if	(isIdlePlayer() || m_wIsFeignDeath)
		return	FALSE;

	int	i;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int			iAbility	=	m_aActiveReaction[i];

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if	(lpAbility->getSkill()->m_wCastMethod	!=	eST_REACTION)
			continue;
		if	(!lpAbility->isDefenseKnockout())
			continue;
		if	(!lpAbility->isActivateReactionSkill(GetCPLevel(m_iCP))	)
			return	FALSE;

		_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
		_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);

		return	TRUE;
	}

	return	FALSE;
}

//
//	스턴을 방어하는 스킬 찾기
BOOL
cACTOR::getDefenseStunReactionSkill(cAbility *_lpAbility)
{
	if	(isIdlePlayer() || m_wIsFeignDeath)
		return	FALSE;

	int	i;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int			iAbility	=	m_aActiveReaction[i];

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if	(lpAbility->getSkill()->m_wCastMethod	!=	eST_REACTION)
			continue;
		if	(lpAbility->getStunResistance()	==	0					)
			continue;
		if	(!lpAbility->isActivateReactionSkill(GetCPLevel(m_iCP))	)
			return	FALSE;

		_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
		_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);

		return	TRUE;
	}

	return	FALSE;
}

//
//	타운트 하는 패시브 스킬 찾기
BOOL
cACTOR::getTauntReactionSkill(cAbility *_lpAbility)
{
	if	(isIdlePlayer() || m_wIsFeignDeath)
		return	FALSE;

	int	i;

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	m_aActiveReaction[i];

		if	(iAbility	==	0xffff)
			continue;

		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if	(lpAbility->getSkill()->m_wCastMethod	!=	eST_REACTION)
			continue;
		if	(!lpAbility->isTaunt()									)
			continue;
		if	(!lpAbility->isActivateReactionSkill(GetCPLevel(m_iCP))	)
			return	FALSE;

		_lpAbility->m_wSkill	=	lpAbility->m_wSkill;
		_lpAbility->m_wLevel	=	lpAbility->m_wLevel+getCorrectSkillLevel(lpAbility);

		return	TRUE;
	}

	return	FALSE;
}

//
//	테일 체이서 사용!!
void
cACTOR::fireTailChaser(cACTOR *_lpTarget,int _iSkill,int _iCount,int _iPeriod,int _iDamage,int _iCorrectDamage)
{
	m_lpField->sendFireTailChaser(m_wSerialInField,_lpTarget->m_wSerialInField,_iSkill,_iCount,_iPeriod,_iDamage,_iCorrectDamage);
}

//
//	카운터로 스윙어라이징 발동?
void
cACTOR::fireCounterSwingArising(cAbility *_lpAbility)
{
	CSkill		*lpSkill	=	_lpAbility->getSkill();

	if	(!lpSkill)
		return;

	int	iKnockBackDistance	=	_lpAbility->getKnockBackDistance();

	if	(iKnockBackDistance	<=	0)
		return;

	action(lpSkill->m_wAction);
	setExclusiveAction(TRUE);

	int	iSpentCP	=	_lpAbility->getSpentCP();
	int	iGetCP		=	_lpAbility->getGetCP();

	reduceCP(iSpentCP-iGetCP,lpSkill->m_wSerial);
	
	int	iAbilityRange	=	_lpAbility->getHitRange(getWeapon());
	int	iThrustRange	=	iAbilityRange+getBodySize();

	if	(m_lpField->thrustActor(m_pos.x,m_pos.y,this,NULL,m_wActorKind,_lpAbility,iThrustRange,iKnockBackDistance))
	{
		CHitInfo	hitInfo;

		hitInfo.m_dwResultField	=	0;
		hitInfo.setHit();

		m_lpField->sendMakeActionForm(m_wSerialInField,0xffff,_lpAbility,&hitInfo);
	}
}

//
//	카운터로 아머 디스어셈블러 발동
void
cACTOR::fireCounterArmorDisassembler(cACTOR *_lpTarget,cAbility *_lpAbility)
{
	CSkill		*lpSkill	=	_lpAbility->getSkill();

	if	(!lpSkill)
		return;

	stop();

	action(lpSkill->m_wAction);
	setExclusiveAction(TRUE);

	int	iSpentCP	=	_lpAbility->getSpentCP();
	int	iGetCP		=	_lpAbility->getGetCP();

	reduceCP(iSpentCP-iGetCP,lpSkill->m_wSerial);

	_lpTarget->damagedBy(this,_lpAbility,0,0);

	CHitInfo	hitInfo;

	hitInfo.reset();

	hitInfo.m_dwResultField		=	0;
	hitInfo.m_dwPhysicalDamage	=	1;
	hitInfo.setHit();

	m_lpField->sendMakeActionForm(m_wSerialInField,_lpTarget->m_wSerialInField,_lpAbility,&hitInfo);
}

//
//	카운터 스킬 발동
void
cACTOR::fireCounterReactionSkill(cACTOR *_lpTarget,cAbility *_lpAbility)
{
	CSkill		*lpSkill	=	_lpAbility->getSkill();

	if	(!lpSkill)
		return;

	if	(lpSkill->m_wAction	!=	0xffff)
	{
		stop();
		action(lpSkill->m_wAction);
		setExclusiveAction(TRUE);
	}

	int	iSpentCP	=	_lpAbility->getSpentCP();
	int	iGetCP		=	_lpAbility->getGetCP();
	int	iHitRange	=	_lpAbility->getHitRange();

	reduceCP(iSpentCP,_lpAbility->m_wSkill);
	increaseCP(iGetCP);

	if	(iHitRange)
	{
		BOOL	bIsGotedCP	=	TRUE;

		if	(lpSkill->m_wAction	!=	0xffff)
			m_lpField->sendMakeActionForm(m_wSerialInField,0xffff,_lpAbility);

		m_bIsOperateCounter	=	TRUE;
		m_lpField->strikeWhirlWind(this,_lpAbility,FALSE);
		m_bIsOperateCounter	=	FALSE;
		m_lpField->addSendOccurEffectBySkill(m_pos.x,m_pos.y,_lpAbility->m_wSkill,eSE_EXPLOSION);

		if	(lpSkill->m_sound.m_strCasting[0])
			sendPlaySkillSound(lpSkill->m_wSerial,eSSD_CASTING);
	}
	else
	{
		CHitInfo	hitInfo;

		_lpTarget->getDamage(this,_lpAbility,0,0,&hitInfo,TRUE,TRUE,FALSE);

		BOOL	bResult	=	strike(_lpTarget,&hitInfo,_lpAbility,TRUE,FALSE,0,0,TRUE);

		if	(lpSkill->m_wAction	!=	0xffff)
			m_lpField->sendMakeActionForm(m_wSerialInField,_lpTarget->m_wSerialInField,_lpAbility,&hitInfo);
		else
			_lpTarget->addSendHitDamage(this,hitInfo.m_dwPhysicalDamage,hitInfo.m_dwMagicDamage,hitInfo.m_dwResultField,_lpAbility->m_wSkill);

		if	(lpSkill->m_sound.m_strCasting[0])
			sendPlaySkillSound(lpSkill->m_wSerial,eSSD_CASTING);
	}
}

//	리액션 스킬 처리
void
cACTOR::operateReactionSkill(int _iTiming,cACTOR *_lpTarget,int *_lpiLimitPhisycalDamage,int *_lpiLimitMagicDamage)
{
	if	(isIdlePlayer() || m_wIsFeignDeath)
		return;

	for (int i=0;;i++)
	{
		cAbility	reaciton;

		if	(!getActivateReaction(&reaciton,_iTiming,i,_lpTarget))
			break;

		if	(random(100)	<	reaciton.getActivateChance())
		{
			operateReactionSkill(_lpTarget,&reaciton);

			if (reaciton.getLimitPhysicalDamage() || reaciton.getLimitMagicDamage())	//	한계 대미지 제한
			{
				if	((*_lpiLimitPhisycalDamage))
					(*_lpiLimitPhisycalDamage)	=	min(reaciton.getLimitPhysicalDamage(),(*_lpiLimitPhisycalDamage));
				else
					(*_lpiLimitPhisycalDamage)	=	reaciton.getLimitPhysicalDamage();

				if	((*_lpiLimitMagicDamage))
					(*_lpiLimitMagicDamage)		=	min(reaciton.getLimitMagicDamage(),(*_lpiLimitMagicDamage));
				else
					(*_lpiLimitMagicDamage)		=	reaciton.getLimitMagicDamage();
			}
		}
	}
}

//
//	리액션 스킬 처리
void
cACTOR::operateReactionSkill(cACTOR *_lpTarget,cAbility *_lpAbility)
{
	BOOL		bIsDecreaseCP	=	FALSE;
	CSkill		*lpSkill=	_lpAbility->getSkill();
	CHitInfo	hitInfo;

	hitInfo.reset();

//	반격
	if	(lpSkill->isDamageSkill())
	{
		int	iRange		=	getAttackRange(_lpAbility);

		if	(isTargetInAttackRange(_lpTarget,iRange))
		{
			_lpTarget->getDamage(this,_lpAbility,0,0,&hitInfo);

			if	(hitInfo.isHit())
			{
				int	iRange		=	getAttackRange(_lpAbility);

				if	(isTargetInAttackRange(_lpTarget,iRange))
				{
					reduceCP(_lpAbility->getSpentCP(),_lpAbility->m_wSkill);
					increaseCP(_lpAbility->getGetCP());
					sendPlayerBasicInformation();

					_lpTarget->damagedBy(this,_lpAbility,hitInfo.m_dwPhysicalDamage,hitInfo.m_dwMagicDamage);

					bIsDecreaseCP	=	TRUE;
				}
			}
		}
	}

	if	(_lpTarget->applySkillExtraEffects(this,_lpAbility) && bIsDecreaseCP == FALSE)
	{
		reduceCP(_lpAbility->getSpentCP(),_lpAbility->m_wSkill);
		increaseCP(_lpAbility->getGetCP());
		sendPlayerBasicInformation();
	}

	m_lpField->sendActiveSkillEffect(m_wSerialInField,_lpTarget->m_wSerialInField,_lpAbility->m_wSkill,m_iCP,hitInfo.m_dwPhysicalDamage,hitInfo.m_dwMagicDamage);
}