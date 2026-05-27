#include	"cACTOR.H"
#include	"CField.H"
#include	"debugCode.H"
#include	"actor_mini_pet.H"
#include	"cGAME.H"
#include	"packetManager.H"

//
//	상태 이상 상태
BOOL
cACTOR::isStrangeStatus()
{
	if (m_wBlindTime)		return	TRUE;	//	장님 상태 남은 시간
	if (m_wPoisonTime)		return	TRUE;	//	초당 데미지/10 , 중독 상태 남은 시간
	if (m_wSleepTime)		return	TRUE;	//	잠이 깰때까지 남은 시간
	if (m_wFreezeTime)		return	TRUE;	//	프리즈 상태 남은 시간
	if (m_wColdTime)		return	TRUE;	//	콜드 상태 남은 시간
	if (m_wStunTime)		return	TRUE;	//	스턴 상태 남은 시간
	if (m_wStoneTime)		return	TRUE;	//	석화 남은 시간
	if (m_wConfuseTime)		return	TRUE;	//	혼란 상태 남은 시간
	if (m_wCharmedTime)		return	TRUE;	//	차밍 상태 남은 시간
	if (m_wNightMareTime)	return	TRUE;	//	악몽
	if (m_wParalysisTime)	return	TRUE;	//	전신 마비

	return	FALSE;
}

//
//	능력치 저하 상태
BOOL
cACTOR::isDeclinePowerStatus()
{
	if (m_attackPower.isWrong()			)	return	TRUE;
	if (m_defensePower.isWrong()		)	return	TRUE;
	if (m_hitChance.isWrong()			)	return	TRUE;
	if (m_dodgeChance.isWrong()			)	return	TRUE;
	if (m_actionSpeed.isWrong()			)	return	TRUE;
	if (m_moveSpeed.isWrong()			)	return	TRUE;
	if (m_concentration.isWrong()		)	return	TRUE;
	if (m_fireResistance.isWrong()		)	return	TRUE;
	if (m_waterResistance.isWrong()		)	return	TRUE;
	if (m_windResistance.isWrong()		)	return	TRUE;
	if (m_earthResistance.isWrong()		)	return	TRUE;
	if (m_lightResistance.isWrong()		)	return	TRUE;
	if (m_darkResistance.isWrong()		)	return	TRUE;
	if (m_allMagicResistance.isWrong()	)	return	TRUE;
	if (m_strength.isWrong()			)	return	TRUE;						//	힘 
	if (m_agility.isWrong()				)	return	TRUE;						//	민첩성
	if (m_constitution.isWrong()		)	return	TRUE;					//	건강
	if (m_intelligence.isWrong()		)	return	TRUE;					//	지식
	if (m_wisdom.isWrong()				)	return	TRUE;						//	지혜
	if (m_charisma.isWrong()			)	return	TRUE;						//	카리스마
	if (m_luck.isWrong()				)	return	TRUE;							//	운

	return	FALSE;
}

//
//	저주 상태
BOOL
cACTOR::isCurseStatus()
{
	if	(m_wFalloffMoraleTime)	return	TRUE;	//	사기 저하 상태 남은 시간
	if	(m_wTargetedTime	)	return	TRUE;	//	타겟 설정 상태 남은 시간
	if	(m_wBreakedArmorTime)	return	TRUE;	//	갑옷 파괴 상태 남은 시간
	if	(m_wBreakedWeaponTime)	return	TRUE;	//	무기 파괴 상태 남은 시간
	if	(m_wSimpletonTime	)	return	TRUE;	//	바보-_- 상태 남은 시간
	if	(m_wBerserkTime		)	return	TRUE;	//	버서크 상태 남은 시간
	if	(m_wTortureTime		)	return	TRUE;	//	고문률-_-??,고문 시간
	if	(m_wBloodDonorTime	)	return	TRUE;	//	피흡혈률,피흡혈 당하는 시간
	if	(m_wContractTime	)	return	TRUE;	//	계약
	if	(m_level.isWrong()	)	return	TRUE;	//	계약
	if	(m_wReversalPowerTime)	return	TRUE;	//	계약
	if  (m_maxHP.isWrong()	)	return	TRUE;
	if	(m_wInterruptingArmorTime)
		return	TRUE;
	if(	m_wInterruptingWeaponTime)
		return TRUE;	// 장비해제..
	if	(m_wDeclientRecoverHPEfficientTime)
		return	TRUE;
	if(m_wNotUseActivityReactionSkillTime)		// 발동형리액션사용금지.
		return TRUE;
	if(m_weaponPower.isWrong())
		return TRUE;
	if(m_armorPower.isWrong())
		return TRUE;


	return	FALSE;
}


//
//	뭔가 이상한 상태
BOOL
cACTOR::isWrongStatus()
{
	if (isStrangeStatus())		return	TRUE;	//	상태 이상 상태
	if (isDeclinePowerStatus())	return	TRUE;	//	능력치 저하 상태
	if (isCurseStatus())		return	TRUE;	//	저주 상태

	return	FALSE;
}

void
cACTOR::reduceStrangeStatusTime(int _iTime)
{
	int	iTime;

	iTime			=	m_wBlindTime-_iTime;	//	장님 상태 남은 시간
	m_wBlindTime	=	max(iTime,0);

	iTime			=	m_wPoisonTime-_iTime;	//	장님 상태 남은 시간
	m_wPoisonTime	=	max(iTime,0);

	iTime			=	m_wBlindTime-_iTime;	//	장님 상태 남은 시간
	m_wBlindTime	=	max(iTime,0);

	iTime			=	m_wSleepTime-_iTime;	//	장님 상태 남은 시간
	m_wSleepTime	=	max(iTime,0);

	iTime			=	m_wFreezeTime-_iTime*dSYNC_FPS;	//	장님 상태 남은 시간
	m_wFreezeTime	=	max(iTime,0);

	iTime			=	m_wColdTime-_iTime;	//	장님 상태 남은 시간
	m_wColdTime		=	max(iTime,0);

	iTime			=	m_wStunTime-_iTime*dSYNC_FPS;	//	장님 상태 남은 시간
	m_wStunTime		=	max(iTime,0);

	iTime			=	m_wStoneTime-_iTime;	//	장님 상태 남은 시간
	m_wStoneTime		=	max(iTime,0);

	iTime			=	m_wNightMareTime-_iTime;	//	장님 상태 남은 시간
	m_wNightMareTime=	max(iTime,0);

	iTime			=	m_wConfuseTime-_iTime;	//	악몽
	m_wConfuseTime	=	max(iTime,0);

	iTime			=	m_wParalysisTime-_iTime;	//	전신 마비
	m_wParalysisTime=	max(iTime,0);

	if	(m_wCharmedTime)
	{
		iTime			=	m_wCharmedTime-_iTime;	//	장님 상태 남은 시간
		m_wCharmedTime	=	max(iTime,0);

		if	(m_wCharmedTime	==	0)
			releaseCharm();
	}
}

void
cACTOR::reduceCurseTime(int _iTime)
{
	int	iTime;

	iTime				=	m_wFalloffMoraleTime	-	_iTime;
	m_wFalloffMoraleTime=	max(iTime,0);

	iTime				=	m_wTargetedTime	-	_iTime;
	m_wTargetedTime		=	max(iTime,0);

	iTime				=	m_wBreakedArmorTime	-	_iTime;
	m_wBreakedArmorTime	=	max(iTime,0);

	iTime				=	m_wBreakedWeaponTime	-	_iTime;
	m_wBreakedWeaponTime=	max(iTime,0);

	iTime				=	m_wSimpletonTime	-	_iTime;
	m_wSimpletonTime	=	max(iTime,0);

	iTime				=	m_wBerserkTime	-	_iTime;
	m_wBerserkTime		=	max(iTime,0);

	iTime				=	m_wTortureTime	-	_iTime;
	m_wTortureTime		=	max(iTime,0);

	iTime				=	m_wBloodDonorTime	-	_iTime;
	m_wBloodDonorTime	=	max(iTime,0);

	m_maxHP.reduceWrongTime(_iTime);
}

void
cACTOR::reduceDeclinePowerStatusTime(int _iTime)
{
	m_attackPower.reduceWrongTime(_iTime);
	m_defensePower.reduceWrongTime(_iTime);
	m_hitChance.reduceWrongTime(_iTime);
	m_dodgeChance.reduceWrongTime(_iTime);
	m_actionSpeed.reduceWrongTime(_iTime);
	m_moveSpeed.reduceWrongTime(_iTime*dSYNC_FPS);
	m_concentration.reduceWrongTime(_iTime);
	m_fireResistance.reduceWrongTime(_iTime);
	m_waterResistance.reduceWrongTime(_iTime);
	m_windResistance.reduceWrongTime(_iTime);
	m_earthResistance.reduceWrongTime(_iTime);
	m_lightResistance.reduceWrongTime(_iTime);
	m_darkResistance.reduceWrongTime(_iTime);
	m_allMagicResistance.reduceWrongTime(_iTime);
	m_strength.reduceWrongTime(_iTime);					//	힘 
	m_agility.reduceWrongTime(_iTime);						//	민첩성
	m_constitution.reduceWrongTime(_iTime);			//	건강
	m_intelligence.reduceWrongTime(_iTime);			//	지식
	m_wisdom.reduceWrongTime(_iTime);						//	지혜
	m_charisma.reduceWrongTime(_iTime);					//	카리스마
	m_luck.reduceWrongTime(_iTime);							//	운

}
void
cACTOR::reduceWrongStatusTime(int _iTime)//	상태이상 치료
{
	reduceStrangeStatusTime(_iTime);
	reduceCurseTime(_iTime);
	reduceDeclinePowerStatusTime(_iTime);
}

//
//	이상상태 치료
void
cACTOR::cureStrangeStatus(int _iEfficient)
{
	m_wBlindTime	-=	m_wBlindTime*_iEfficient/100;	//	장님 상태 남은 시간
	m_wPoisonTime	-=	m_wPoisonTime*_iEfficient/100;	//	초당 데미지/10 , 중독 상태 남은 시간
	m_wSleepTime	-=	m_wSleepTime*_iEfficient/100;	//	잠이 깰때까지 남은 시간
	m_wFreezeTime	-=	m_wFreezeTime*_iEfficient/100;	//	프리즈 상태 남은 시간
	m_wColdTime		-=	m_wColdTime	*_iEfficient/100;	//	콜드 상태 남은 시간
	m_wStunTime		-=	m_wStunTime	*_iEfficient/100;	//	스턴 상태 남은 시간
	m_wStoneTime	-=	m_wStoneTime*_iEfficient/100;	//	석화 남은 시간
	m_wConfuseTime	-=	m_wConfuseTime*_iEfficient/100;	//	혼란 상태 남은 시간
	m_wCharmedTime	-=	m_wCharmedTime*_iEfficient/100;
	m_wNightMareTime-=	m_wNightMareTime*_iEfficient/100;					//	악몽
	m_wParalysisTime-=	m_wParalysisTime*_iEfficient/100;					//	전신 마비

	m_wTerribleDreamTime-=	m_wTerribleDreamTime*_iEfficient/100;	//	가위

	if	(m_wCharmedTime)
	{
		m_wCharmedTime		-=	m_wCharmedTime*_iEfficient/100;	//	가위
		m_wCharmedTime		=	max(m_wCharmedTime,1);
	}

	releaseCharm();
}

//
//	저하계 치료
void
cACTOR::cureDeclinePowerStatus()
{
	m_attackPower.cure();
	m_defensePower.cure();
	m_hitChance.cure();
	m_dodgeChance.cure();
	m_actionSpeed.cure();
	m_moveSpeed.cure();
	m_concentration.cure();
	m_fireResistance.cure();
	m_waterResistance.cure();
	m_windResistance.cure();
	m_earthResistance.cure();
	m_lightResistance.cure();
	m_darkResistance.cure();
	m_allMagicResistance.cure();
	m_strength.cure();					//	힘 
	m_agility.cure();						//	민첩성
	m_constitution.cure();			//	건강
	m_intelligence.cure();			//	지식
	m_wisdom.cure();						//	지혜
	m_charisma.cure();					//	카리스마
	m_luck.cure();							//	운
}

//
//	저주 치료

void
cACTOR::cureCurseStatus()
{
	m_wFalloffMoraleTime=	0;	//	사기 저하 상태 남은 시간
	m_wTargetedTime		=	0;	//	타겟 설정 상태 남은 시간
	m_wBreakedArmorTime	=	0;	//	갑옷 파괴 상태 남은 시간
	m_wBreakedWeaponTime=	0;	//	무기 파괴 상태 남은 시간
	m_wSimpletonTime	=	0;	//	바보-_- 상태 남은 시간
	m_wBerserkTime		=	0;	//	버서크 상태 남은 시간
	m_wTortureTime		=	0;	//	고문률-_-??,고문 시간
	m_wBloodDonorTime	=	0;	//	피흡혈률,피흡혈 당하는 시간
	m_wContractTime		=	0;

	m_wDeclientRecoverHPEfficientTime	=	0;
	
	if	(m_wInterruptingArmorTime)
	{
		m_wInterruptingArmorTime	=	0;
		m_wInterruptingArmorLevel	=	0;
		
		sendEtcWork(eEW_INTERRUPTING_ARMOR,0,0);
		buildPower();
	}

	if	(m_wReversalPowerTime)
	{
		m_wReversalPowerTime	=	0;

		if	(isPureMonster()	==	FALSE)
			buildPower();

		if	(isPlayer())
			sendEtcWork(eEW_SET_REVERSAL_POWER_TIME,0);
	}

	if	(m_wPOS_Time)
	{
		int	iSpentTime	=	m_wPOS_FullTime-m_wPOS_Time;

		operatePlotOfShadow(iSpentTime);

		m_wPOS_Time		=	0;	//	가위
	}

	m_maxHP.cure();

	if	(m_level.isWrong())
	{
		m_level.cure();
		m_strength.cure();						//	힘 
		m_agility.cure();						//	민첩성
		m_constitution.cure();					//	건강
		m_intelligence.cure();					//	지식
		m_wisdom.cure();						//	지혜
		m_charisma.cure();						//	카리스마

		sendEtcWork(eEW_LEVEL_DRAIN,m_wSerialInField,m_level.m_sCorrectValue,m_level.m_wTime,TRUE,getLevel(),0);

		if	(isPlayer())
			buildPower();
		else
			rebuildForUpdateLevel();
		
		addSendSimpleInfo();

		if	(getPartySerial()	!=	0xffff)
			g_pPartyManager->sendWPChangeMemberInfo(getPartySerial(),m_strName,m_iLevel,m_bf4SecretDungeonPlayCount,m_wJob,m_bf3GuildDungeonPlayTime);
	}
	m_weaponPower.cure();
	m_armorPower.cure();

}

//
//	상태이상 치료
void
cACTOR::cureWrongStatus()
{
	cureStrangeStatus();
	cureDeclinePowerStatus();
	cureCurseStatus();
}

//
//	좋은 상태 해제
void
cACTOR::dispell()
{		// 마지막 수정일 : 09.10.05 
	m_addFireDamage.dispell();
	m_addWaterDamage.dispell();
	m_addWindDamage.dispell();
	m_addLightDamage.dispell();
	m_addDarkDamage.dispell();
	m_addEarthDamage.dispell();

	m_attackPower.dispell();
	m_defensePower.dispell();
	m_hitChance.dispell();
	m_dodgeChance.dispell();
	m_actionSpeed.dispell();
	m_moveSpeed.dispell();
	m_concentration.dispell();
	m_concentrationByStellarPeer.dispell();
	m_fireResistance.dispell();
	m_waterResistance.dispell();
	m_windResistance.dispell();
	m_earthResistance.dispell();
	m_lightResistance.dispell();
	m_darkResistance.dispell();
	m_allMagicResistance.dispell();

	m_strength.dispell();						//	힘 
	m_agility.dispell();						//	민첩성
	m_constitution.dispell();					//	건강
	m_intelligence.dispell();					//	지식
	m_wisdom.dispell();							//	지혜
	m_charisma.dispell();						//	카리스마
	m_luck.dispell();							//	운
	m_maxCP.dispell();
	m_maxHP.dispell();
	m_level.dispell();
	m_detecting.dispell();
	
	m_wSOD_Time	=	0;

	m_wUpkeepTheOtherSelfTime	=	0;
	m_wTheOtherSelfCount		=	0;

	if	(m_sInvisivilityTime	>	0)
	{
		m_sInvisivilityTime	=	0;
		m_bIsHideByShadowHideSkill = FALSE;
		m_sShadowHideAddCriticalChance	=	0;
		m_wShadowHideAddPhysicalPowerPercent	= 0;
	}
	if	(m_sFloatTime		>	0)
		m_sFloatTime		=	0;
	if	(m_sBlurTime			>	0)
		m_sBlurTime			=	0;
	if	(m_sNonTargetTime	>	0)
		m_sNonTargetTime	=	0;
	
	if	(m_wInvincibleTime	!=	0xffff)
		m_wInvincibleTime	=	0;	//	무적 상태
	
	m_wDuelBuffType	=	eDUEL_BUFF_NONE;

	if	(m_wDuelBuffTime)
		m_wDuelBuffTime	=	1;

	for (int  i = 0;i<dMAX_ENCHANTED_MAGIC_COUNT_FOR_CHARACTER;i++)
	{
		if	(m_aEnchantedMagic[i].m_wSkill	==	0xffff)
			continue;

		CSkill	*lpSkill	=	&g_aSkill[m_aEnchantedMagic[i].m_wSkill];

		for (int iEEIndex=0;iEEIndex<dEXTRA_EFFECT_COUNT;iEEIndex++)
		{
			CSkillExtraEffect		*lpExtraEffect	=	&lpSkill->m_aExtraEffect[iEEIndex];

			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_OWN_FORCE		||
				lpExtraEffect->m_bf4TargetMethod	==	eEEAT_PARTY_MEMBER	||
				lpExtraEffect->m_bf4TargetMethod	==	eEEAT_SELF		)
			{
				m_aEnchantedMagic[i].m_wSkill		=	0xffff;
			}
		}
	}

	mDS.m_aiExValue[0]	=	7;
	updateEnchantedMagic(TRUE);
}


BOOL
cACTOR::perfectReviveAllPartyMember(int _iUpkeepTime,int _iIncreaseRate,cItem *_lpItem,cACTOR *_lpCaster)
{
	cParty*	lpParty	=	getParty();

	if	(!lpParty)
		return	FALSE;

	int		iCount	=	0;

	SG_USE_ITEM	sgUseItem;

	sgUseItem.base.set(sizeof(sgUseItem),dSG_USE_ITEM);
	memcpy(&sgUseItem.item,_lpItem,sizeof(sgUseItem.item));

	sgUseItem.bf1IsUseEffectToHero	=	TRUE;
	sgUseItem.bf1IsDisplayMsg		=	TRUE;

	strcpy(sgUseItem.strCaster,_lpCaster->m_strName);

	for (int i=0;i<lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

		if	(!lpMember	||	!lpMember->isDeath() || !_lpCaster->isOnSameField(lpMember))
			continue;

		lpMember->m_maxHP.correctPercentageValue(_iIncreaseRate,_iUpkeepTime);
		lpMember->m_maxCP.correctPercentageValue(_iIncreaseRate,_iUpkeepTime);

		lpMember->revive(100,100,FALSE);

		sgUseItem.wWho	=	lpMember->m_wSerialInField;
		g_userPM.add(lpMember->m_iClientSerial,&sgUseItem,sgUseItem.base.wSize);

		iCount++;
	}

	if	(iCount	==	0)
		return	FALSE;

	return	TRUE;
}

//
//	부활
BOOL
cACTOR::revive(int _iRestoreTimePercentage,int _iRestoreHP,BOOL _bIsHealByValue)
{
	if	(g_bIsDuelServer)
		m_iCP	=	getMaxCP();

	if	(!isDeath() || m_wInterruptingReviveTime)
		return	FALSE;

	stop();

	m_iDeathCount	=	0;
	m_iHP			=	1;
	//JBC	엑터가 죽으면 리셋시켜주는 부분. 08-09-25
	if(m_wReversalPowerTime)
		m_wReversalPowerTime = 1;

	addSendRevive();

	if	(_iRestoreHP)
	{
		if	(_bIsHealByValue)
			m_iHP		=	_iRestoreHP;
		else
			m_iHP		=	getMaxHP()*_iRestoreHP/100;

		m_iHP		=	min(m_iHP,getMaxHP());

		addSendSimpleInfo();
	}
//	if	(m_lpField->m_bIsGuildBattleField	||	m_lpField->m_bIsGuildPointBattleField)
//		return;
	if	(m_iLevel					<	10	)
		return	TRUE;

	if	(_iRestoreTimePercentage	<=	0	)
		return	TRUE;

	if	(isPlayer())
	{
		int	iRestoreSecond		=	0;
		int	iSecond				=	m_bf12DeathPeneltySecond;

		iRestoreSecond			=	iSecond*_iRestoreTimePercentage/100;
		iSecond					-=	iRestoreSecond;
		m_bf12DeathPeneltySecond=	max(iSecond,1);

		sendDeathPeneltySecond(FALSE,TRUE);
	}

	if	(isPet())
	{
		cACTOR	*lpTamer		=	getTamer();

		if	(lpTamer)
		{
			m_iExperience		+=	m_iLostExperience*_iRestoreTimePercentage/100;
			lpTamer->m_aPet[m_wPetIndex].m_dwExperience	=	m_iExperience;
			lpTamer->sendUpdatePetExp(m_wPetIndex);
		}
	}

	buildPower();

	m_bf1IsOperateReadyToRemoveActorWork	=	FALSE;

	return	TRUE;
}

//
//	부활
void
cACTOR::animatePartner(int _iTime,int _iReviveTime)
{
	if	(!isDeath())
		return;

	stop();

	m_iDeathCount				=	0;
	m_iHP						=	getMaxHP();
	m_wAnimatePartnerTime		=	_iTime;
	m_wAnimatePartnerReviveTime	=	_iReviveTime;

	m_bf1IsAnimatePartnerStatus	=	TRUE;

	if	(m_bf2AnimatePartnerCount	<	dMONSTER_ANIMATE_PARTNER_LIMIT_COUNT)
		m_bf2AnimatePartnerCount++;

	addSendRevive();
	addSendSimpleInfo();
	buildPower();

	sendEtcWork(eEW_ANIMATE_PARTNER_TIME,_iTime);

	m_bf1IsOperateReadyToRemoveActorWork	=	FALSE;
}


//
//	스킬에 의한 치료
BOOL
cACTOR::cure(cACTOR *_lpCaster,cAbility *_lpAbility,int _iHealHP,int _iRestoreTimePercentage)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(isDeath()	&&	lpSkill->isResurrection()	==	FALSE)
		return	FALSE;
	
	BOOL	bIsEffected	=	FALSE;

	if	(lpSkill->m_bf1IsApplyHealPointByPercentage)
	{
		LONGLONG	llHP	=	_iHealHP/100;
		llHP				=	llHP*getMaxHP()/100;
		_iHealHP			=	(int)llHP;
	}

	if (lpSkill->isCureStrangeStatus())
	{
		bIsEffected		=	TRUE;
		cureStrangeStatus();
	}
	if (lpSkill->isCureDeclinePowerStatus())
	{
		bIsEffected		=	TRUE;
		cureDeclinePowerStatus();
	}
	if (lpSkill->isCureCurseStatus())
	{
		bIsEffected		=	TRUE;
		cureCurseStatus();
	}
	if (lpSkill->isCureAllStrangeStatus())
	{
		bIsEffected		=	TRUE;
		cureWrongStatus();
	}
	if (lpSkill->isResurrection())
	{
		bIsEffected		=	TRUE;
		revive(_iRestoreTimePercentage);
	}

	if (_iHealHP > 0 && m_iHP < getMaxHP()) 
		bIsEffected = TRUE;


	if	(lpSkill->m_bf1IsInstanceHeal)
	{
		instanceHealHP(_iHealHP);
		addSendSimpleInfo();
		if(m_wSoulBurnTime)
		{
			m_wSoulBurnTime = 0;
			m_wSoulBurnDamage = 0;
			dispell();
			cureCurseStatus();
			cureDeclinePowerStatus();
			cureStrangeStatus();
			cureWrongStatus();
			sendEtcWork(eEW_DISPELL);
		}
	}
	else
	{
		if(m_wInstanceHealEffectTime)
		{
			int iInstanceHealHP	=	_iHealHP * m_wInstanceHealEffectPercent/10000;
			iInstanceHealHP	=	min(iInstanceHealHP,_iHealHP);
			if(iInstanceHealHP)
			{
				instanceHealHP(iInstanceHealHP);
				addSendSimpleInfo();
			}
			_iHealHP  = max(_iHealHP - iInstanceHealHP,0);
		}

		healHP(_iHealHP);
		if(m_wSoulBurnTime)
		{
			m_wSoulBurnTime = 0;
			m_wSoulBurnDamage = 0;
			dispell();
			cureCurseStatus();
			cureDeclinePowerStatus();
			cureStrangeStatus();
			cureWrongStatus();
			sendEtcWork(eEW_DISPELL);
		}
	}

	iMiniPet()->operateHealSkillWork(_iHealHP);

	applySkillExtraEffects(_lpCaster,_lpAbility);

	return	bIsEffected;
}