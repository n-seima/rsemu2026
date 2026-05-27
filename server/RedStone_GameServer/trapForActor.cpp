#include "cActor.h"
#include "trap.h"
#include "cField.h"
#include "packetManager.h"
#include "CGuild.h"
#include "actor_mini_pet.h"

void
cACTOR::checkCaughtInTrap(cTrap *_lpTrap)
{
	if	(_lpTrap	==	NULL)
		_lpTrap	=	m_lpField->getCaughtTrap(this);

	if	(!_lpTrap)
		return;

	cACTOR	*lpOwner	=	m_lpField->getTestedActor(_lpTrap->m_wOwner);

	if	(!lpOwner)
	{
		m_lpField->removeTrap(_lpTrap->m_wSerial);
		return;
	}

	cAbility	*lpAbility	=	_lpTrap->getAbility();

	if (lpAbility->getSkill()->m_bf1IsDefendOnImageTrap)
	{
		m_lpField->m_skillManager.castDefendOnImageTrap(lpOwner,_lpTrap);
		m_lpField->explosionTrap(lpOwner,_lpTrap,this,eETT_DEFEND_ON_IMAGE_FIRST_STEP);
	}
	else
		m_lpField->explosionTrap(lpOwner,_lpTrap,this);
}

//
//	마법 데미지 산출
void
cACTOR::getTrapMagicDamage(cACTOR *_lpOwner,cTrap *_lpTrap,cAbility *_lpAbility,CHitInfo *_lpHitInfo)
{
	int								iFireDamage	=0,iWaterDamage=0,iWindDamage=0,iEarthDamage=0,iLightDamage=0,iDarkDamage=0;
	CSkill							*lpSkill	=	_lpAbility->getSkill();
	CContinuousAttackedPeneltyInfo	*lpContinuousAttackedPeneltyInfo	=	NULL;
	BOOL							bIsDamaged	=	FALSE;
	BOOL							bIsPVP		=	m_lpField->isPVP(_lpOwner,this);

	int		iCorrectMagicDamageByAlchemyTitle	=	0;
	BOOL	bIsPureMagicDamageSkill				=	lpSkill->isPureMagicDamageSkill();
	int		iPhysicalDamage						=	0;

	if	(bIsPureMagicDamageSkill)
		iCorrectMagicDamageByAlchemyTitle	=	_lpOwner->m_wAlchemyTitleFactor;

	if	(lpSkill->isFireDamageSkill())
	{
		iFireDamage		=	_lpAbility->getMagicDamage(eAM_ADD_FIRE_DAMAGE,iPhysicalDamage,m_iHP);
		iFireDamage		+=	iFireDamage*iCorrectMagicDamageByAlchemyTitle/100;

		iFireDamage		-=	iFireDamage*getFireResistance()/100;
		bIsDamaged		=	TRUE;
	}

	if	(lpSkill->isWaterDamageSkill())
	{
		iWaterDamage	=	_lpAbility->getMagicDamage(eAM_ADD_WATER_DAMAGE,iPhysicalDamage,m_iHP);
		iWaterDamage	+=	iWaterDamage*iCorrectMagicDamageByAlchemyTitle/100;

		iWaterDamage	-=	iWaterDamage*getWaterResistance()/100;
		bIsDamaged		=	TRUE;
	}

	if	(lpSkill->isWindDamageSkill())
	{
		iWindDamage		=	_lpAbility->getMagicDamage(eAM_ADD_WIND_DAMAGE,iPhysicalDamage,m_iHP);
		iWindDamage		+=	iWindDamage*iCorrectMagicDamageByAlchemyTitle/100;

		iWindDamage		-=	iWindDamage*getWindResistance()/100;
		bIsDamaged		=	TRUE;
	}

	if	(lpSkill->isEarthDamageSkill())
	{
		iEarthDamage		=	_lpAbility->getMagicDamage(eAM_ADD_EARTH_DAMAGE,iPhysicalDamage,m_iHP);
		iEarthDamage	+=	iEarthDamage*iCorrectMagicDamageByAlchemyTitle/100;

		iEarthDamage	-=	iEarthDamage*getEarthResistance()/100;
		bIsDamaged		=	TRUE;
	}

	if	(lpSkill->isLightDamageSkill())
	{
		iLightDamage		=	_lpAbility->getMagicDamage(eAM_ADD_LIGHT_DAMAGE,iPhysicalDamage,m_iHP);
		iLightDamage	+=	iLightDamage*iCorrectMagicDamageByAlchemyTitle/100;

		iLightDamage		-=	iLightDamage*getLightResistance()/100;
		bIsDamaged		=	TRUE;
	}

	if	(lpSkill->isDarkDamageSkill())
	{
		iDarkDamage		=	_lpAbility->getMagicDamage(eAM_ADD_DARK_DAMAGE,iPhysicalDamage,m_iHP);
		iDarkDamage		+=	iDarkDamage*iCorrectMagicDamageByAlchemyTitle/100;

		iDarkDamage		-=	iDarkDamage*getDarkResistance()/100;
		bIsDamaged		=	TRUE;
	}

	int	iAbsorbDamage		=	0;

	if (m_bAbsorbMagicDamage+m_bAbsorbFireDamage	&&	iFireDamage)
		iAbsorbDamage	+=	iFireDamage*(m_bAbsorbFireDamage+m_bAbsorbMagicDamage)/100;		//	불꽃 데미지를 흡수해서 체력으로 변환
	if (m_bAbsorbMagicDamage+m_bAbsorbWaterDamage	&&	iWaterDamage)
		iAbsorbDamage	+=	iWaterDamage*(m_bAbsorbWaterDamage+m_bAbsorbMagicDamage)/100;	//	물 데미지를 흡수해서 체력으로 변환
	if (m_bAbsorbMagicDamage+m_bAbsorbWindDamage	&&	iWindDamage)
		iAbsorbDamage	+=	iWindDamage*(m_bAbsorbWindDamage+m_bAbsorbMagicDamage)/100;		//	바람 데미지를 흡수해서 체력으로 변환
	if (m_bAbsorbMagicDamage+m_bAbsorbEarthDamage	&&	iEarthDamage)
		iAbsorbDamage	+=	iEarthDamage*(m_bAbsorbEarthDamage+m_bAbsorbMagicDamage)/100;	//	대지 데미지를 흡수해서 체력으로 변환
	if (m_bAbsorbMagicDamage+m_bAbsorbLightDamage	&&	iLightDamage)
		iAbsorbDamage	+=	iLightDamage*(m_bAbsorbLightDamage+m_bAbsorbMagicDamage)/100;	//	빛 데미지를 흡수해서 체력으로 변환
	if (m_bAbsorbMagicDamage+m_bAbsorbDarkDamage	&&	iDarkDamage)
		iAbsorbDamage	+=	iDarkDamage*(m_bAbsorbDarkDamage+m_bAbsorbMagicDamage)/100;		//	어둠 데미지를 흡수해서 체력으로 변환

	iFireDamage		-=	iFireDamage*(m_bAbsorbFireDamage+m_bAbsorbMagicDamage)/100;
	iWaterDamage	-=	iWaterDamage*(m_bAbsorbWaterDamage+m_bAbsorbMagicDamage)/100;
	iWindDamage		-=	iWindDamage*(m_bAbsorbWindDamage+m_bAbsorbMagicDamage)/100;
	iEarthDamage	-=	iEarthDamage*(m_bAbsorbEarthDamage+m_bAbsorbMagicDamage)/100;
	iLightDamage	-=	iLightDamage*(m_bAbsorbLightDamage+m_bAbsorbMagicDamage)/100;
	iDarkDamage		-=	iDarkDamage*(m_bAbsorbDarkDamage+m_bAbsorbMagicDamage)/100;

	if (iFireDamage>	0)
		_lpHitInfo->m_dwMagicDamage	+=	iFireDamage;
	if (iWaterDamage>	0)
		_lpHitInfo->m_dwMagicDamage	+=	iWaterDamage;
	if (iWindDamage>	0)
		_lpHitInfo->m_dwMagicDamage	+=	iWindDamage;
	if (iEarthDamage>	0)
		_lpHitInfo->m_dwMagicDamage	+=	iEarthDamage;
	if (iLightDamage>	0)
		_lpHitInfo->m_dwMagicDamage	+=	iLightDamage;
	if (iDarkDamage>	0)
		_lpHitInfo->m_dwMagicDamage	+=	iDarkDamage;

	if	(bIsPVP)
		iAbsorbDamage	=	iAbsorbDamage*dPVP_ABSORB_DAMAGE_FACTOR/100;

	float	fCorrectIntelligenceValue	=	_lpOwner->getCorrectIntelligenceFactorForMagicDamage(lpSkill,_lpTrap->m_wOwnerIntelligence);

	_lpHitInfo->m_dwMagicDamage			=	int(_lpHitInfo->m_dwMagicDamage+_lpHitInfo->m_dwMagicDamage*_lpTrap->m_wOwnerIntelligence*fCorrectIntelligenceValue);
	_lpHitInfo->m_dwAbsorbDamage		=	max(iAbsorbDamage,0);

	if	(_lpHitInfo->m_dwMagicDamage)
		_lpHitInfo->setHit();
	else
	if	(bIsDamaged)
		_lpHitInfo->setImmune();
}

//
//	물리적인 데미지를 구한다.
BOOL
cACTOR::getTrapPhysicalDamage(cACTOR *_lpOwner,cTrap *_lpTrap,cAbility *_lpAbility,CHitInfo *_lpHitInfo)
{
	CSkill		*lpSkill	=	_lpAbility->getSkill();
	BOOL		bIsHit		=	TRUE;
	BOOL		bIsNEP		=	isNEPlayer(_lpOwner);
	BOOL		bIsPVP		=	m_lpField->isPVP(_lpOwner,this);
	int			iDisarmLevel=	max(_lpTrap->m_wDisarmLevel,1);
	int			iDodgeChance=	getLuck()*1000/(50*iDisarmLevel);
//● 모든 함정은 (캐릭터의 운 / (500 * 함정의 해체 레벨))*(1+함정 회피 칭호 보정율)의 확률로 회피할 수 있음							

	iMiniPet()->operateDodgeTrapSkill(&iDodgeChance);
	
	if	(random(1000)	<	iDodgeChance)
	{
		_lpHitInfo->reset(TRUE);
		_lpHitInfo->setLuckyDodge();

		return FALSE;	//	안 맞았다. -o-
	}

	int			iAttackPower	=	_lpTrap->getPhysicalAttackPower(bIsPVP,_lpOwner->getLevel(),getLevel());
	int			iDefensivePower	=	getDefensivePower();

	if (m_wHillSkill!=	0xffff)
		iDefensivePower	=	max(m_wHillDefensivePower,iDefensivePower);

	LONGLONG	llDamage		=	iAttackPower;
	int			iDamage			=	(int) (llDamage*llDamage/(llDamage+iDefensivePower));

	_lpHitInfo->setHit();
	_lpHitInfo->m_dwPhysicalDamage	=	iDamage;
	_lpHitInfo->setPhysicalDamage();	//	물리 데미지가 있다고 체크

	return	TRUE;
}

//
//	데미지 산출
void
cACTOR::getTrapDamage(cACTOR *_lpOwner,cTrap *_lpTrap,cAbility *_lpAbility,CHitInfo *_lpHitInfo)
{
	_lpHitInfo->reset();
	_lpHitInfo->m_dwResultField	=	0;

	if	(m_bIsGuildObject)
	{
		CGuild	*lpGuild			=	getGuild();
		CGuild	*lpAttackerGuild	=	_lpOwner->getGuild();

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

			if	(!_lpAbility->isPhysicalDamageSkill())	//	물리 데미지를 주는 녀석이 아니라면 마법 데미지만 계산한다.
			{
				_lpHitInfo->m_dwMagicDamage		=	iDamage;
				_lpHitInfo->m_dwPhysicalDamage	=	0;
			}
		}

		return;		
	}

	if	(m_wInvincibleTime)
	{
		_lpHitInfo->m_dwResultField	=	dATTACK_RESULT_INVINCIBLE;

		return;
	}

	BOOL	bIsPVP	=	FALSE,bIsNEP	=	FALSE;

	if (_lpOwner)
	{
		bIsPVP	=	m_lpField->isPVP(_lpOwner,this);	
		bIsNEP	=	isNEPlayer(_lpOwner);
	}

	if (!_lpAbility->isPhysicalDamageSkill())	//	물리 데미지를 주는 녀석이 아니라면 마법 데미지만 계산한다.
	{
		getMagicDamage(_lpOwner,_lpAbility,_lpHitInfo);//getTrapMagicDamage(_lpOwner,_lpTrap,_lpAbility,_lpHitInfo);
		correctDamage(_lpOwner,_lpAbility,_lpTrap->m_pos.x,_lpTrap->m_pos.y,bIsPVP,bIsNEP,_lpHitInfo,1);	
		return;
	}

	BOOL	isHit	=	getTrapPhysicalDamage(_lpOwner,_lpTrap,_lpAbility,_lpHitInfo);

	if	(!isHit)
		return;

	getMagicDamage(_lpOwner,_lpAbility,_lpHitInfo);//getTrapMagicDamage(_lpOwner,_lpTrap,_lpAbility,_lpHitInfo);
	correctDamage(_lpOwner,_lpAbility,_lpTrap->m_pos.x,_lpTrap->m_pos.y,bIsPVP,bIsNEP,_lpHitInfo,1);
}

void
cACTOR::damagedByTrap(cACTOR *_lpOwner,CHitInfo *_lpHitInfo,cAbility *_lpAbility,int _iX,int _iY)
{
	BOOL	bResult			=	FALSE;
	int		iDamage			=	_lpHitInfo->getDamage(),iReduceHP;
	int		iMagicDamage	=	_lpHitInfo->m_dwMagicDamage;
	int		iPhysicalDamage	=	_lpHitInfo->m_dwPhysicalDamage;

	if	(isNpc()||	isOperator()	)
		return;

	if	(m_iHP	<=	0	&& isDeath())
		return;	//	이미 시체

//	미러 타워 처리
	if (m_wMirrorTowerCaster != 0xffff && iMagicDamage && _lpOwner && _lpAbility)
		iMagicDamage	=	m_lpField->operateMirrorTower(_lpOwner,this,_lpAbility,iMagicDamage);

	iDamage	=	iPhysicalDamage+iMagicDamage;

	if (iDamage	<	0)
		iDamage	=	0;

	iReduceHP	=	min(iDamage,m_iHP);

	if (isMonster())	//	몬스터라묜...
		setRestraintTimeByBattle();

	m_wTerribleDreamTime	=	0;
	m_wSleepTime			=	0;	//	맞으면 깨어난다. -o-

	if	(iDamage)
		bResult		=	TRUE;

	if	(m_wHillSkill!=	0xffff)
		strikeIcyStalagmite(_lpAbility,iDamage,iPhysicalDamage,_lpOwner);
	else
		reduceHP(iDamage,_lpOwner);

	if (m_iHP > 0)
	{
		int	iChance	=	_lpAbility->getActivateChance();

		if (iChance == 0 || random(100 ) < iChance)
			bResult		+=	applySkillExtraEffects(_lpOwner,_lpAbility,iPhysicalDamage,_iX,_iY);	//	스킬 부가 효과 적용
	}

	if (_lpAbility && !_lpAbility->isHpSyncSkill())
	{
		if (_lpOwner && _lpOwner->isPlayer())
		{
			cACTOR	*lpAttackerTarget	=	_lpOwner->m_lpTarget;

			if	(lpAttackerTarget == NULL)
				sendRemainHP(_lpOwner);
			else
			{
				if (lpAttackerTarget->m_wSerialInField == m_wSerialInField)
					sendRemainHP(_lpOwner);
			}
		}
	}

	if	(m_iHP	<=	0)
		return;

	operateRevenge(_lpOwner,_lpAbility);
}

void
cACTOR::tryDisarmTrap(int _iTrapSerial)
{
	cTrap	*lpTrap	=	m_lpField->getTrap(_iTrapSerial);

	if	(!lpTrap)
	{
		SG_REMOVE_TRAP		packet;

		packet.base.set(sizeof(packet),dSG_REMOVE_TRAP);
		packet.wSerial	=	_iTrapSerial;

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

		return;
	}

	int		iRange		=	GetOvalRange(m_pos.x,m_pos.y,lpTrap->m_pos.x,lpTrap->m_pos.y);
	int		iTrapRange	=	lpTrap->m_wRange;

	if	(iTrapRange	==	0)
	{
		cAbility	*lpTrapAbility	=	lpTrap->getAbility();

		if	(!lpTrapAbility)
			return;

		iTrapRange	=	lpTrapAbility->getHitRange();
	}

	iTrapRange	+=	getBodySize();
	iTrapRange	+=	c_iDisarmTrapAbleRange+10;
	
	if	(lpTrap->m_wTeam	==	m_wTeam)
	{
		return;
	}

	if	(iRange	>	iTrapRange*iTrapRange)
	{
		sendEtcWork(eEW_FAILED_DISARM_ACTOR_TRAP_BY_RANGE,_iTrapSerial);

		return;
	}

	if	(!isVisibleTrap(lpTrap->m_wDetectLevel))
	{
		sendEtcWork(eEW_FAILED_DISARM_ACTOR_TRAP_BY_IS_CAN_NOT_DETECT,_iTrapSerial);

		return;
	}

	cAbility	*lpDisarmSkill	=	getDisarmTrapAbility();

	if	(!lpDisarmSkill)
	{
		sendEtcWork(eEW_FAILED_DISARM_ACTOR_TRAP,_iTrapSerial);

		return;
	}

	int	iAR		=	actionToObject(lpDisarmSkill,eTARGET_OBJECT_TRAP,_iTrapSerial);

	if	(iAR	==	eAR_OK)
	{
		sendEtcWork(eEW_TRY_DISARM_ACTOR_TRAP,_iTrapSerial);
	}
	else
	if	(iAR	==	eAR_LOW_CP)
	{
		sendEtcWork(eEW_TRY_DISARM_ACTOR_TRAP_FAILED_BY_LOW_CP,_iTrapSerial);
	}
	else
	{
		sendEtcWork(eEW_TRY_DISARM_ACTOR_TRAP_FAILED_BY_BUSY,_iTrapSerial);
	}	
}