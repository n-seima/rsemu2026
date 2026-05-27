#include	"CActorStatus.H"
#include	"cACTOR.H"
#include	"cFIELD.H"
#include	"debugCode.H"
#include	"CGame.H"
#include	"CGuild.H"
#include	"secretDungeon.H"
#include	"packetManager.H"
#include	"actor_mini_pet.H"
#include	"cEvent.H"
#include	"rebirth_define.H"

//
//	중독 대미지 설정
void
cACTOR::setPoisonDamage(int _iTime,int _iDamage,int _iCaster)
{
	if	(m_wPoisonTime * m_iPoisonDamagePerSec >= _iTime*_iDamage)
		return;

	m_wPoisonTime			=	_iTime;
	m_iPoisonDamagePerSec	=	_iDamage;
	m_wPoisonDamageCaster	=	_iCaster;
}

void
cACTOR::setFreezeTime(int _iTime)
{
	if	(m_wFreezeTime >= _iTime)
		return;

	m_wFreezeTime	=	_iTime;

	stopAction();
	setActorPos(m_pos.x,m_pos.y,TRUE,TRUE);
	sendCurrentPosition();
}

void
cACTOR::setNightmareTime(int _iTime)
{
	if (m_wNightMareTime	>= _iTime)
		return;

	m_wNightMareTime	=	_iTime;

	stopAction();
	setActorPos(m_pos.x,m_pos.y,TRUE,TRUE);
	sendCurrentPosition();
}

void
cACTOR::setParalysisTime(int _iTime)
{
	if (m_wParalysisTime	>= _iTime)
		return;

	m_wParalysisTime	=	_iTime;

	stopAction();
	setActorPos(m_pos.x,m_pos.y,TRUE,TRUE);
	sendCurrentPosition();
}

//
//	대미지 흡수
void
cACTOR::absorbDamage(int _iTarget,int _iDamage)
{
	if	(m_iHP == getMaxHP()	||	_iDamage	<=	0)
		return;

	if	(instanceHealHP(_iDamage))
		m_lpField->sendAbsorbDamage(_iTarget,_iDamage);
}

//
//	스킬 부가 효과로 체력 회복
void
cACTOR::regenHPByAidSkill(int _iHealPoint)
{
	if	(m_iHP == getMaxHP())
		return;

	if	(!instanceHealHP(_iHealPoint))
		return;

	SG_REGEN_HP_BY_AID_SKILL	packet;

	packet.base.set(sizeof(SG_REGEN_HP_BY_AID_SKILL),dSG_REGEN_HP_BY_AID_SKILL);

	packet.wTarget	=	m_wSerialInField;
	
	LONGLONG	llHP=	m_iHP;
	llHP			=	llHP*10000/getMaxHP();
	packet.wRemainHP=	(WORD)llHP;
	packet.wRegenHP	=	_iHealPoint/100;

	m_lpField->addSendPacket((ALL_MSG *)&packet,m_wSerialInField);
}

//
//	스킬 부가 효과로 체력 회복
BOOL
cACTOR::recoverHPByAidSkill(int _iSkill,int _iHealPoint)
{
	if	(m_iHP == getMaxHP() || isDeath())
		return	FALSE;

	if	(isCopyActor())
		return	FALSE;

	SG_RECOVER_HP_BY_AID_SKILL	packet;

	packet.base.set(sizeof(SG_RECOVER_HP_BY_AID_SKILL),dSG_RECOVER_HP_BY_AID_SKILL);
	packet.wTarget	=	m_wSerialInField;
	packet.wSkill	=	_iSkill;
	packet.iRemainHP=	m_iHP;
	packet.wRecoverHP=	_iHealPoint/100;

	m_lpField->addSendPacket((ALL_MSG *)&packet,m_wSerialInField);

	if	(g_aSkill[_iSkill].m_bf1IsInstanceHeal)
	{
		if	(!instanceHealHP(_iHealPoint))
			return	FALSE;
	}
	else
		healHP(_iHealPoint);

	return	TRUE;
}

//
//

void
cACTOR::resetTemporaryMagicEffect()
{
	m_wSealActionTime	=	0;
	m_wUnsealAction		=	0xffff;
	m_wSealRunningTime	=	0;
	m_wInvincibleTime	=	0;				//	무적시간
	m_sInvisivilityTime	=	0;			//	인비저빌러티시간(-이면 장비에 붙어 있는거다.)
	m_bIsHideByShadowHideSkill = FALSE;
	m_sShadowHideAddCriticalChance	=	0;
	m_wShadowHideAddPhysicalPowerPercent	=	0;

	if (m_sFloatTime	>	0)
		m_sFloatTime	=	0;
		//	공중부양 시간(-이면 장비에 붙어 있는거다.)
	if (m_sBlurTime		>	0)
		m_sBlurTime		=	0;//	흐릿한 상태 시간(-이면 장비에 붙어 있는거다.)

	if (m_sNonTargetTime>	0)				//	타겟에서 제외됨(-1이면 장비에 붙어 있는 것임)
		m_sNonTargetTime=	0;

	m_wMaintainMaxCPTime		=	0;			//	이 시간동안 CP가 max로 유지된다.
	m_wMaintainMaxWeponDamageTime=	0;	//	이 시간동안 무기의 데미지가 항상 max가 나온다.
	m_iFirstAidHP				=	0;					//	응급처치한 HP
	m_wFirstAidTime				=	0;				//	응급처치한 시간

	m_wBlindTime				=	0;						//	장님 상태 남은 시간
	m_iPoisonDamagePerSec		=	0;
	m_wPoisonTime				=	0;	//	초당 데미지/10 , 중독 상태 남은 시간
	m_wSleepTime				=	0;	//	잠이 깰때까지 남은 시간
	m_wFreezeTime				=	0;	//	프리즈 상태 남은 시간
	m_wColdTime					=	0;	//	콜드 상태 남은 시간
	m_wStunTime					=	0;	//	스턴 상태 남은 시간
	m_wStoneTime				=	0;	//	석화 남은 시간
	m_wConfuseTime				=	0;	//	혼란 상태 남은 시간

	m_wFalloffMoraleTime		=	0;			//	사기 저하 상태 남은 시간
	m_wTargetedTime				=	0;				//	타겟 설정 상태 남은 시간
	m_wBreakedArmorTime			=	0;			//	갑옷 파괴 상태 남은 시간
	m_wBreakedWeaponTime		=	0;			//	무기 파괴 상태 남은 시간
	m_wSimpletonTime			=	0;				//	바보-_- 상태 남은 시간
	m_wBerserkTime				=	0;					//	버서크 상태 남은 시간
	m_wTortureDarkDamage		=	0;
	m_wTortureTime				=	0;	//	고문률-_-??,고문 시간
	m_wBloodDonorRate			=	0,
	m_wBloodDonorTime			=	0;	//	피흡혈률,피흡혈 당하는 시간
	m_wCurseAmplificationTime	=	0;		//	저주 증폭 상태

	m_wHideWeapon1Time			=	0;
	m_wHideWeapon2Time			=	0;
	m_dwEnchantedMask			=	0;
	m_wIsHideShield				=	FALSE;

	m_wBlockingRigidityTime		=	0;
	m_wHitActionTime			=	0;

	m_wRemainActionTime			=	0;
	m_bf1IsReadyToWarpToNearestVillage	=	FALSE;
	m_bf1IsReadyToCreateTownPortal		=	FALSE;
	m_bf1IsAnimatePartnerStatus			=	FALSE;

	m_wTauntedToTargetTime		=	0;
	m_wTauntedToTargetDamagePercent		=	0;
	m_dwTauntedToTargetNameHashCode		=	0;
	m_wTauntedToTargetSerial			=	0xffff;

	m_wMirrorTowerUpkeepTime	=	0;
	m_wMirrorTowerCaster		=	0xffff;
	m_wJewelTime				=	0;					//	보석 유지 시간

	m_wUpdateOuterActorInfoPeriod=	0;
	m_wChargeTime				=	0;
	m_iRecoverHP				=	0;
	m_wFeignDeathTime			=	0;
	m_wIsFeignDeath				=	FALSE;
	
	m_wSelfReproofTime			=	0;	//	자책감 남은 시간.
	m_wRemainImpulseTime		=	0;
	m_wDegenerateTime			=	0;
	m_wTerribleDreamTime		=	0;
	m_wHwabyungTime				=	0;
	m_wPOS_Time					=	0;
	m_wDeclientRecoverHPEfficientTime	=	0;
	m_wDeclientRecoverHPEfficient		=	0;
//	m_wInterruptingReviveTime			=	0;	//	부활 방해 인데 죽었을때 초기화 시키면 안되지..;;
	m_wContractTime						=	0;
//	m_wSoulOathTime						=	0;
	if	(m_wInterruptingArmorTime)
	{
		m_wInterruptingArmorTime	=	0;
		m_wInterruptingArmorLevel	=	0;
		
		sendEtcWork(eEW_INTERRUPTING_ARMOR,0,0);
		buildPower();
	}


	m_wImmuneStrageStausTime			=	0;
	m_wImmunePhysicalAttackTime			=	0;
	m_wImmuneMagicAttackTime			=	0;
	m_wNotUseActivityReactionSkillTime	=	0;		// 발동형리액션사용금지 시간.
	m_wConsensusTime					=	0;		// 교감상태.
	if	(m_wInterruptingWeaponTime)
	{
		m_wInterruptingWeaponTime	=	0;
		m_wInterruptingWeaponDeclineDamagePer	=	0;
		
		sendEtcWork(eEW_INTERRUPTING_ARMOR,0,0);
		buildPower();
	}

	m_wIncreaseAllStatusTime			= 0; // 모든 스텟증가.
	
	m_wMadnessTime						= 0; //  광란..
	
	m_wSoulBurnTime						= 0 ;// 소울 번

	m_wRageSoulTime						= 0;// 타오르는 영혼
	
	m_wBindingWordsTime					= 0; // 언어 구속..

	m_strengtheningCriticalDamage.resetAidData();	// 치명타 대미지 증가..

	setExclusiveAction(FALSE);

	m_wBoostPower			=	100;
	m_wBoostTime			=	0;

	if	(m_wCharmedTime	)
		releaseCharm();

	m_detecting.resetAidData();
	m_regenHPPerSecond.resetAidData();				//	매초 체력을 회복
	m_bonusExp.resetAidData();					//	경험치 보너스
	m_attackPower.resetAidData();					//	공격력
	m_defensePower.resetAidData();					//	방어력

	m_level.resetAidData();
	m_maxHP.resetAidData();						//	최대 HP
	m_maxCP.resetAidData();						//	최대 CP

	m_strength.resetAidData();						//	힘 
	m_agility.resetAidData();						//	민첩성
	m_constitution.resetAidData();					//	건강
	m_intelligence.resetAidData();					//	지식
	m_wisdom.resetAidData();						//	지혜
	m_charisma.resetAidData();						//	카리스마
	m_luck.resetAidData();							//	운

	m_actionSpeed.resetAidData();					//	액션 속도 보정
	m_moveSpeed.resetAidData();					//	이동 속도 보정

	m_hitChance.resetAidData();					//	명중률 보정
	m_dodgeChance.resetAidData();					//	회피 보정
	m_concentration.resetAidData();
	m_concentrationByStellarPeer.resetAidData();
	m_allignment.resetAidData();
	m_addFireDamage.resetAidData();
	m_addWaterDamage.resetAidData();
	m_addWindDamage.resetAidData();
	m_addLightDamage.resetAidData();
	m_addDarkDamage.resetAidData();
	m_addEarthDamage.resetAidData();
	m_blockingChance.resetAidData();

	m_weakningTargetFireResistance.resetAidData();
	m_weakningTargetWaterResistance.resetAidData();
	m_weakningTargetWindResistance.resetAidData();
	m_weakningTargetEarthResistance.resetAidData();
	m_weakningTargetLightResistance.resetAidData();
	m_weakningTargetDarkResistance.resetAidData();
	m_weakningTargetMagicResistance.resetAidData();

	m_strengtheningFireDamage.resetAidData();
	m_strengtheningWaterDamage.resetAidData();
	m_strengtheningWindDamage.resetAidData();
	m_strengtheningEarthDamage.resetAidData();
	m_strengtheningDarkDamage.resetAidData();
	m_strengtheningLightDamage.resetAidData();
	m_strengtheningMagicDamage.resetAidData();
	m_charmingResistance.resetAidData();
	m_strangeStatusResistance.resetAidData();
	m_curseResistance.resetAidData();
	m_declinePowerResistance.resetAidData();

	m_fireResistance.resetAidData();				//	불꽃 저항
	m_waterResistance.resetAidData();				//	물 저항
	m_windResistance.resetAidData();				//	바람 저항
	m_earthResistance.resetAidData();				//	대지 저항
	m_lightResistance.resetAidData();				//	빛 저항
	m_darkResistance.resetAidData();				//	어둠 저항
	m_allMagicResistance.resetAidData();			//	모든 마법 저항

	releaseShakle();

	m_iHP	=	min(m_iHP,getMaxHP());	//	최대 HP
	m_iCP	=	min(m_iCP,getMaxCP());	//	최대 CP

	memset(m_aEnchantedMagic,0xff,sizeof(m_aEnchantedMagic));
	m_fightEnergy.reset();					// 투기 상태..
	m_wRushTime							=	0;	//돌진
	m_wFightingSpiritTime				=	0;		// 투지.
	m_wIgnoreNeedStateTime				=	0;		// 필요상태무시.

	m_wIgnoreBlockingTime				=	0;
	m_wIgnoreBlockingValue				=	0;		// 블럭킹무시

	m_wReturnMissileDamageTime			=	0;
	m_wReturnMissileDamageValue			=	0;		// 미사일대미지리턴


	m_wIncreaseHardBlowTime				=	0;		
	m_wHardBlowValue					=	0;		// 강타확률증가

	m_wTransformationTime				=	0;
	m_wTransformationLevel				=	0;
	m_wTransformationSkin				=	0xffff;
	m_wTransformationSkill				=	0xffff;

		// 출혈
	m_wBleedingPeriod					=	0;
	m_wBleedingTime						=	0;
	m_wBleedingCaster					=	0xffff;				//	출혈 건놈		
	m_iBleedingDamage					=	0;				//	출혈대미지

	// 감전
	
	m_wElectricShockTime				=	0;				//	지속시간			
	m_wElectricShockDamagePercent		=	0;			//	감전효과
	

	// 즉시힐효과
	m_wInstanceHealEffectTime			=	0;				//	지속시간			
	m_wInstanceHealEffectPercent		=	0;					//	즉시힐효과

	//	언데드화
	m_wMakeTypeUndeadTime				=	0;					//	지속시간

	// 헌혈
	m_wBloodDonationTime				=	0;
	m_wBloodDonationPercent				=	0;
}

void
cACTOR::renewPremiumItem(BOOL _bIsNow)
{
	if	(!isPlayer())
		return;

	int		iCount,i;
	BOOL	bIsOvertimePremiumItem	=	FALSE;

	for (iCount=0;iCount<g_game.m_iRenewPremiumItemCount;iCount++)
	{
		int		iIndex			=	g_game.m_aRenewPremiumItemInfo[iCount].m_wSerial;
		BOOL	bIsForOnlyMiniPet=	g_game.m_aRenewPremiumItemInfo[iCount].m_wIsForOnlyYahoo;
		BOOL	bIsForOnlyRedgem=	g_game.m_aRenewPremiumItemInfo[iCount].m_wIsForOnlyRedGem;
		int		iTerm			=	g_game.m_aRenewPremiumItemInfo[iCount].m_wExtendTerm;
		
		if	(m_wRenewPremiumItemCounter	<	iIndex)
		{
			for (int iMP=0;iMP<c_iMiniPetCount;iMP++)
				m_aMiniPet[iMP].renewExpireTime(iTerm);

			if	(m_reversalFrameYear)
				iMiniPet()->extendReversalFrameTime(iTerm,FALSE,FALSE);

			if	(bIsForOnlyMiniPet)
				continue;

			for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
			{
				if	(!m_aEquip[i].isPremiumItem() || m_aEquip[i].isExtraItem())
					continue;

				if	(m_aEquip[i].m_year	==	0)
					continue;

				m_aEquip[i].increaseMinute(iTerm);	//	하루 증가

				sendRenewItemLog(&m_aEquip[i],iTerm);

				bIsOvertimePremiumItem	=	TRUE;

				if	(_bIsNow)
					sendUpdateItemData(i+dBORDER_OF_ITEM_AND_EQUIPMENT,TRUE);
			}

			for (i=0;i<dOWN_ITEM_COUNT;i++)
			{
				if (!m_aItems[i].isPremiumItem()|| m_aItems[i].isExtraItem())
					continue;

				if (m_aItems[i].m_year	==	0)
					continue;

				m_aItems[i].increaseMinute(iTerm);	//	하루 증가

				sendRenewItemLog(&m_aItems[i],iTerm);

				bIsOvertimePremiumItem	=	TRUE;

				if (_bIsNow)
					sendUpdateItemData(i,TRUE);
			}

			for (i=0;i<m_bf6ExtraInventorySize;i++)
			{
				cItem	*lpItem	=	&m_aExtraInventory[i];

				if (!lpItem->isPremiumItem()|| lpItem->isExtraItem())
					continue;

				if (lpItem->m_year	==	0)
					continue;

				lpItem->increaseMinute(iTerm);	//	하루 증가

				sendRenewItemLog(lpItem,iTerm);

				bIsOvertimePremiumItem	=	TRUE;

				if (_bIsNow)
					sendUpdateItemData(i+dOWN_ITEM_COUNT,TRUE);
			}
		}
	}

	m_wRenewPremiumItemCounter	=	max(g_game.m_iLastRenewPremiumItemCounter,m_wRenewPremiumItemCounter);

	if (bIsOvertimePremiumItem)
		sendRegistServerMessage(eRSM_OVER_TIME_ITEM_TERM_OF_VALIDATE_IN_INVENTORY);
}

void
cACTOR::initDataForPlayer()
{
	if	(isPlayer() == FALSE)
		return;

	m_llLastGold		=	getOwnGoldSum();
	m_llGettingGold		=	0;
	m_llSpentGold		=	0;
	m_llTradeGettingGold=	0;
	m_llTradeSpentGold	=	0;

	m_bf1IsLevelDown	=	FALSE;

	if	(m_dwPeneltyTime)
	{
		SG_PENELTY_TIME		packet;
		
		packet.base.set(sizeof(packet),dSG_PENELTY_TIME);
		packet.dwPeneltyTime	=	m_dwPeneltyTime;

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
	}

	if	(g_eventManager.m_iUpkeepSecondForSoundOfLeadersBell)
		if	(STRICMP(m_strName,g_eventManager.m_strSoundOfLeadersBellOwner)	==	0)
			m_bf1IsSoundOfLedersBellsLeader	=	TRUE;

	if	(m_bf8ModifyDataCounter	<	nsUpdatePlayerData::ADD_ARBEIT_BUFFER_SIZE	)	//	추가된 아르바이트 버퍼 초기화
		memset(m_aCompleteArbeit2,0,sizeof(m_aCompleteArbeit2));

	if	(m_bf8ModifyDataCounter	<	nsUpdatePlayerData::REVISE_JAPAN_QUEST_DATA)
	{
#ifdef	_FOR_JAPAN
#endif
	}

	if	(m_bf8ModifyDataCounter	<	nsUpdatePlayerData::ADD_EAT_MINIPET_COUNT	)	//	미니펫 2마리 활성되게 수정
	{
		m_bf4EatFireMiniPetCount	=	0;
		m_bf4EatWaterMiniPetCount	=	0;
		m_bf4EatWindMiniPetCount	=	0;
		m_bf4EatEarthMiniPetCount	=	0;
		m_bf4EatLightMiniPetCount	=	0;
		m_bf4EatDarkMiniPetCount	=	0;

		m_bf4RebirthCount			=	0;	//	전생 횟수
		m_bf4UseSealSphereOfDawnCount=	0;	//	새벽의 봉인구 사용 횟수
	}

	if	(m_bf8ModifyDataCounter	<	nsUpdatePlayerData::ADD_ITEM_COOL_TIME	)	//	미니펫 2마리 활성되게 수정
		memset(m_awItemCoolTime,0,sizeof(m_awItemCoolTime));
	
	if	(m_bf8ModifyDataCounter	<	nsUpdatePlayerData::ADD_ACTIVE_MINI_PET2)	//	미니펫 2마리 활성되게 수정
	{
		if	(m_bf1IsAddMiniPetSystem	==	FALSE)
		{
			m_bf5ActiveMiniPet2		=	c_iMiniPetCount;
			m_bf3GreateGodGrace		=	0;

			m_reversalFrameYear		=	0;
			m_reversalFrameMonth	=	0;
			m_reversalFrameDay		=	0;
			m_reversalFrameHour		=	0;
			m_reversalFrameMinute	=	0;	//	25
		}
	}

	if	(m_bf8ModifyDataCounter	<	nsUpdatePlayerData::ADD_CHAPTER5)	//	미니펫 2마리 활성되게 수정
	{
		m_bf4CH5StateBonusCount		=	0;
		m_bf4CH5SkillBonusCount		=	0;
		m_bf4CH5HPBonusCount		=	0;
		m_bf4CH5CPBonusCount		=	0;

		m_bf4CH5PeneltyHPBonusCount	=	0;
		m_bf4CH5PeneltyCPBonusCount	=	0;
		m_bf5BardTitleCoolTime		=	0;
		m_bf4LevelOfLatestReceivedCH5EquipLevelBonus	=	0;
	}

	if	(m_bf8ModifyDataCounter	<	nsUpdatePlayerData::ADD_GREATE_GOD_GRACE)	//	거신의 은총용 데이터 추가 + 이벤트 변수 추가.
	{
		m_bf3GreateGodGrace			=	0;
	}

	if	(m_bf8ModifyDataCounter	<	nsUpdatePlayerData::ADD_WANT_EVENT_COUNT_RESET)	//	이벤트 변수 리셋.
	{
		m_wWordEventTime			=	0;
		m_bf3EventCounter			=	0;
	}

	if	(m_bf8ModifyDataCounter	<	nsUpdatePlayerData::ADD_GUILD_MASTER_GRACE)	//	이벤트 변수 리셋.
	{
		m_wGuildMasterGrace			=	0;
		m_bf3GuildDungeonPlayTime	=	0;
	}

	m_bf8ModifyDataCounter		=	c_iUpdatePlayerDataCounter;

	m_wWeeklyDuelRank			=	g_duelRankerManager.getRanking(eWeeklyRanking,m_strId);
	m_wDailyDuelRank			=	g_duelRankerManager.getRanking(eDailyRanking,m_strId);

	if	(m_lpField->m_bIsGuildBattleField && m_wOperatorLevel	>=	4)
		m_bf1IsHide	=	TRUE;

	m_iExpPeneltyForRebirthPlayer=	0;

	if	(m_bf4RebirthCount && m_iLevel <= dGAIN_EXP_PENELTY_LEVEL_FOR_REBIRTH)
		m_iExpPeneltyForRebirthPlayer	=	c_aiGainExpPeneltyForRebirth[m_bf4RebirthCount];

	if	(g_bIsDuelServer)
	{
		memset(m_awSkillCoolTime,0,sizeof(m_awSkillCoolTime));

		if	(m_wOperatorLevel	>=	4)
			m_bf1IsHide	=	TRUE;

		if	(m_lpField->m_wDuelType	==	0xffff)
		{
			int	i;

			for (i=dEQUIP_BELT_SLOT1;i<=dEQUIP_BELT_SLOT5;i++)
			{
				cItem	*lpItem	=	&m_aEquip[i];

				if	(lpItem->m_wBaseItem	==	0xffff)
					continue;

				if	(lpItem->checkFirstEffect(eIE_DUEL_BUFF_FIRE)	||
					lpItem->checkFirstEffect(eIE_DUEL_BUFF_WATER)	||
					lpItem->checkFirstEffect(eIE_DUEL_BUFF_WIND)	||
					lpItem->checkFirstEffect(eIE_DUEL_BUFF_EARTH))
					lpItem->reset();
			}

			for (i=0;i<dOWN_ITEM_COUNT;i++)
			{
				cItem	*lpItem	=	&m_aItems[i];

				if	(lpItem->m_wBaseItem	==	0xffff)
					continue;

				if	(lpItem->checkFirstEffect(eIE_DUEL_BUFF_FIRE)	||
					lpItem->checkFirstEffect(eIE_DUEL_BUFF_WATER)	||
					lpItem->checkFirstEffect(eIE_DUEL_BUFF_WIND)	||
					lpItem->checkFirstEffect(eIE_DUEL_BUFF_EARTH))
					lpItem->reset();
			}

			for (i=0;i<m_bf6ExtraInventorySize;i++)
			{
				cItem	*lpItem	=	&m_aExtraInventory[i];

				if	(lpItem->m_wBaseItem	==	0xffff)
					continue;

				if	(lpItem->checkFirstEffect(eIE_DUEL_BUFF_FIRE)	||
					lpItem->checkFirstEffect(eIE_DUEL_BUFF_WATER)	||
					lpItem->checkFirstEffect(eIE_DUEL_BUFF_WIND)	||
					lpItem->checkFirstEffect(eIE_DUEL_BUFF_EARTH))
					lpItem->reset();
			}

			for (i=0;i<dOWN_PET_COUNT;i++)
			{
				if	(m_aPet[i].m_bf10Level	==	0)
					continue;

				m_aPet[i].m_bf22CurrentHP	=	4000000;

				int	iaOldPet[]	=	{1271,1272,1273,1274,1275,1277,1278,1280,1281,1282,1283,1284,1285,-1};

				for (int iIndex=0;iaOldPet[iIndex]!=-1;iIndex++)
				{
					if	(m_aPet[i].getJobIndex()	==	iaOldPet[iIndex])
					{
						removePet(i);
						sendRemovePet(i);
						break;
					}
				}
			}
		}
	}
}

//
//	초기화
void
cACTOR::initData()
{
	int	i;

	int	iOperatorShape	=	getOperatorShapeByBook();

	if	(iOperatorShape	!=	-1)
	{
		int	aiShape[]	=	{251,63,64};

		m_bf10Costume	=	aiShape[iOperatorShape];

		if	(iOperatorShape	==	0)
			m_bf1IsEvilSpritOfChristmas	=	TRUE;
		else
			m_bf1IsEvilSpritOfChristmas	=	FALSE;

		if	(m_aAbility[0].m_wSkill	!=	910)
		{
			for (int i=0;i<dABILITY_COUNT;i++)	
			{
				m_aAbility[i].m_wSkill	=	910+i;
				m_aAbility[i].m_wLevel	=	0;

				if	(g_aSkill[m_aAbility[i].m_wSkill].m_wSerial	==	0xffff)
					m_aAbility[i].m_wSkill	=	0xffff;
			}

			m_dwSkillExperience	=	getSkillPointSumByLevel()+1;
		}
	}

	if	(m_wRedStonePoint	==	0xffff)
		m_wRedStonePoint	=	0;
	
	m_iCarrotCount	=	-1;

	CQuestManager	*lpQM;
	
	m_wNonActionTime	=	1000;

	m_bf1IsIncorrectConnectByGuildBattle	=	FALSE;
	m_iTodayWithdrawGoldInGuild				=	max(m_iTodayWithdrawGoldInGuild,0);

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if	(m_aProcessQuest[i].isActive		==	0	)
			continue;

		lpQM	=	NULL;

		if	(m_aProcessQuest[i].questKind	==	eQT_NORMAL)
			lpQM	=	&g_quest;
		if	(m_aProcessQuest[i].questKind	==	eQT_ARBEIT)
			lpQM	=	&g_questArbeit;

		if	(lpQM==	NULL || m_aProcessQuest[i].questIndex	>=	(DWORD)lpQM->m_iCount)
		{
			memset(&m_aProcessQuest[i],0,sizeof(m_aProcessQuest[i]));
			continue;
		}
		if	(m_aProcessQuest[i].questProcessLevel	>=	dQUEST_STATUS_COUNT)
		{
			memset(&m_aProcessQuest[i],0,sizeof(m_aProcessQuest[i]));
			continue;
		}
	}

	buildPower();

	if	(m_bf1IsAnimatePartnerStatus)
	{
		m_iHP						=	0;
		m_bf1IsAnimatePartnerStatus	=	FALSE;
	}

	m_iHP				=	min(m_iHP,getMaxHP());

	m_pos.x				=	m_iXPos;
	m_pos.y				=	m_iYPos;
	m_pos.mx			=	m_pos.x >>	dTILE_XSIZE_SHIFT;
	m_pos.my			=	m_pos.y >>	dTILE_YSIZE_SHIFT;

	m_posLastMoved.x	=	m_pos.x;
	m_posLastMoved.y	=	m_pos.y;

	if	(m_lpField->isVillage())
		m_iCP	=	max(m_iCP,-20000);

	CClient	*lpClient	=	CM.mGETCLIENT(m_iClientSerial);

	if	(lpClient)
	{
		m_wOperatorLevel			=	lpClient->GetIsOper();

#ifdef	_USE_PROVISIONRANK
		m_wOperatorLevelInAccount	=	lpClient->GetIsOperInAccount();
#else
		m_wOperatorLevelInAccount	=	0;
#endif
		if	(m_wOperatorLevelInAccount	>	1)
			m_wOperatorLevelInAccount	=	0;

		if	(m_wOperatorLevel	>=	2)
			m_dwPeneltyTime	=	0;
	}

	if	(m_wJob	<	dJOB_PRINCESS)
		memset(m_awSkillCoolTime,0,sizeof(m_awSkillCoolTime));

	if	(FALSE	==	m_bf1CancelQuestWork)
	{
		m_bf1CancelQuestWork	=	TRUE;

		if	(m_bf1CancelQuestWork2	==	FALSE)
		{
			for	(i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
			{
				m_aProcessQuest[i].isActive			=	FALSE;
				m_aProcessQuest[i].questKind		=	0;
				m_aProcessQuest[i].questProcessLevel=	0;
				m_aProcessQuest[i].questValue		=	0;
				m_aProcessQuest[i].questIndex		=	0;
			}
		}
	}

	if	(FALSE == m_bf1IsAddFailedPremiumItemInfoAndResetWayPoint)
	{
		memset(m_aWayPoint,0xff,sizeof(m_aWayPoint));
		m_bf1IsAddFailedPremiumItemInfoAndResetWayPoint	=	TRUE;
	}

	if	(FALSE == m_bf1IsResetPremiumFieldInItem)
	{
		for (i=0;i<dOWN_ITEM_COUNT;i++)
			m_aItems[i].m_isCanNotAttachPrefixItem	=	FALSE;

		for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
			m_aEquip[i].m_isCanNotAttachPrefixItem	=	FALSE;

		for (i=0;i<dEXTRA_INVENTORY_SIZE;i++)
			m_aExtraInventory[i].m_isCanNotAttachPrefixItem	=	FALSE;

		m_bf1IsResetPremiumFieldInItem	=	TRUE;
		m_itemFailedPremiumItem.reset();
	}

	if	(FALSE	==	m_bf1IsAddAboutMainQuestInfo)
	{
		m_bf1IsAddAboutMainQuestInfo	=	TRUE;

		m_wProcessChapterIndex			=	0xffff;
		m_wProcessHighQuestIndex		=	0xffff;
		memset(m_awMainQuestValue,0,sizeof(m_awMainQuestValue));
		memset(m_abMainQuestSwitchStatus,0,sizeof(m_abMainQuestSwitchStatus));
	}

	if	(FALSE	==	m_bf1IsChangePetJobData)
	{
		m_bf1IsChangePetJobData	=	TRUE;

		for(int i=0;i<dOWN_PET_COUNT;i++)
			m_aPet[i].m_bf1Job	=	FALSE;

		for (i=0;i<dOWN_ITEM_COUNT;i++)
		{
			if	(m_aItems[i].isBreedingRecordBook())
			{
				cPetDefine	*lpPetInfo	=	(cPetDefine	*)&m_aItems[i].m_bCount;

				lpPetInfo->m_bf1Job	=	FALSE;
			}
		}
		for (i=0;i<dEXTRA_INVENTORY_SIZE;i++)
		{
			if	(m_aExtraInventory[i].isBreedingRecordBook())
			{
				cPetDefine	*lpPetInfo	=	(cPetDefine	*)&m_aExtraInventory[i].m_bCount;

				lpPetInfo->m_bf1Job	=	FALSE;
			}
		}
	}

//	펫과 사육 기록서 무결성 검사
	{
		revisePetJobIndex();
	}


	if	(m_bf1AddData2005_03_03	==	FALSE)
	{
		m_bf1AddData2005_03_03					=	TRUE;

		m_bf6ExtraInventorySize					=	0;
		m_timeLastNomannerPointTime.m_dwValue	=	0;
		m_wBonusSkillPoint						=	0;
		m_wBonusStatePoint						=	0;
		m_bf5GiveANoMannerPointDay				=	0;

		memset(m_aCompleteArbeit,0,sizeof(m_aCompleteArbeit));

		for (i=0;i<dEXTRA_INVENTORY_SIZE;i++)
			m_aExtraInventory[i].reset();
	}

	if	(FALSE == m_bf1AddResetSkillStateScroll)
	{
		int	iValidSlotCount	=	0;

		for (int i=0;i<getItemSlotCount();i++)
		{
			if (m_aItems[i].getBasicItem()	==	NULL)
				iValidSlotCount++;
		}

		if	(iValidSlotCount	>=	2)
		{
			cItem	*lpItem;

			lpItem				=	getValidInventorySlot();
			lpItem->reset();
			lpItem->m_wBaseItem	=	1449;
			lpItem->m_bCount	=	1;
			lpItem->m_bDurability=	10;
			lpItem->m_dwSerial	=	largeRandom(0x7fffffff);

			lpItem				=	getValidInventorySlot();
			lpItem->reset();
			lpItem->m_wBaseItem	=	1450;
			lpItem->m_bCount	=	1;
			lpItem->m_bDurability=	10;
			lpItem->m_dwSerial	=	largeRandom(0x7fffffff);

			m_bf1AddResetSkillStateScroll	=	TRUE;

			arrangeInventory();
		}
	}

	if	(FALSE == m_bf1IsBeginPremiumService)
	{
		for (i=0;i<dOWN_ITEM_COUNT;i++)
		{
			cBasicItem	*lpBasicItem	=	m_aItems[i].getBasicItem();

			if	(lpBasicItem	==	NULL)
				continue;

			if	(lpBasicItem->m_wKind	==	eIK_PREMIUM_ITEM)
				m_aItems[i].reset();
		}

		for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
		{
			cBasicItem	*lpBasicItem	=	m_aEquip[i].getBasicItem();

			if	(lpBasicItem	==	NULL)
				continue;

			if	(lpBasicItem->m_wKind	==	eIK_PREMIUM_ITEM)
				m_aEquip[i].reset();
		}

		m_bf1IsBeginPremiumService		=	TRUE;

		m_itemFailedPremiumItem.reset();
	}
	
	if	(FALSE	==	m_bf1IsAddIncreaseLastDamageBy20070508	||	m_wIncreaseFinalDamage	>	400)
	{
		m_wIncreaseFinalDamageTime	=	0;
		m_wIncreaseFinalDamage		=	0;
		m_bf1IsAddIncreaseLastDamageBy20070508	=	TRUE;
	}

	if	(FALSE == m_bf1IsAddPetSystemBy20040916)
	{
		m_bf1IsResetHidePitchmanShop20041102	=	FALSE;
		m_bf1IsAddQuestCountDownTimer20055027	=	FALSE;
		m_bf1IsRenewalRenewItemPeriodSystem		=	FALSE;

		m_bf1IsAddPetSystemBy20040916			=	TRUE;

		for (i=0;i<dOWN_PET_COUNT;i++)
			m_aPet[i].reset();
	}

	if	(FALSE	==	m_bf1IsAddMiniPetSystem)
	{
		m_bf1IsAddMiniPetSystem	=	TRUE;
		m_bf8ModifyDataCounter	=	0;
		m_bf5ActiveMiniPet2		=	c_iMiniPetCount;
		m_bf3GreateGodGrace		=	0;

		m_reversalFrameYear		=	0;
		m_reversalFrameMonth	=	0;
		m_reversalFrameDay		=	0;
		m_reversalFrameHour		=	0;
		m_reversalFrameMinute	=	0;	//	25

		for (i=0;i<c_iMiniPetCount;i++)
			m_aMiniPet[i].reset();
	}

	if	(FALSE	==	m_bf1IsAddQuestCountDownTimer20055027)
	{
		m_bf1IsAddQuestCountDownTimer20055027	=	TRUE;
		memset(m_awQuestCountDownTimer,0xff,sizeof(m_awQuestCountDownTimer));
	}

	if	(FALSE	==	m_bf1IsResetHidePitchmanShop20041102)
	{
		m_bf1IsResetHidePitchmanShop20041102=	TRUE;
		m_bf1IsHidePitchmanShop				=	FALSE;
	}

	if	(FALSE	==	m_bf1IsRenewalRenewItemPeriodSystem)
	{
		m_bf1IsRenewalRenewItemPeriodSystem	=	TRUE;
		m_wRenewPremiumItemCounter			=	0;
	}
	if	(FALSE	==	m_bf1IsResetRedStonePoint)
	{
		m_bf1IsResetRedStonePoint			=	TRUE;
		m_wRedStonePoint					=	0;
		m_wBoostExpSecondByTreasureMapEvent	=	0;
		m_wBoostGoldSecondByTreasureMapEvent=	0;
		m_bf1IsPlayOnGuildBattle			=	FALSE;	//	길드전 중이다
		m_bf1IsTeleportToCouple				=	FALSE;	//	커플링을 사용해 텔레포트중이다.
		m_bf7UseCoupleRingSlot				=	0;	//	사용한 커플링 아이템 슬롯
		m_bf1IsMoveFieldByDeath				=	FALSE;	//	죽어서 마을로 돌아 왔다.
		m_bf1IsMoveFieldByTeleport			=	FALSE;	//	죽어서 마을로 돌아 왔다.
	}

	initDataForPlayer();

	if	(g_bIsDuelServer)
	{
		m_iHP	=	getMaxHP();
		dispell();
		resetTemporaryMagicEffect();
		cureWrongStatus();
	}

	if	(m_wBoostExpSecondByTreasureMapEvent>=	10000)
		m_wBoostExpSecondByTreasureMapEvent	=	0;

	if	(m_wBoostGoldSecondByTreasureMapEvent>=	10000)
		m_wBoostGoldSecondByTreasureMapEvent=	0;
	
	setMoveSpeed();
	renewPremiumItem();

	if	(m_wJob/2	==	dJOB_WEREWOLF/2)	//	마법사나 늑인 스킬 모자라는 넘들
	{
		BOOL	bIsRequireSkillReset	=	FALSE;

		for (int i=0;i<dABILITY_COUNT-2;i++)
		{
			if	(m_aAbility[i].m_wSkill	==	0xffff)
				bIsRequireSkillReset	=	TRUE;
		}

		if	(bIsRequireSkillReset)
			resetSkillPoint();
	}

	if	(m_wGuildSerial	!=	0xffff && m_timeGuildJoinTime.m_dwValue	==	0)
	{
		m_timeGuildJoinTime.year	=	g_currentTime.m_wYear	-	2000;
		m_timeGuildJoinTime.month	=	g_currentTime.m_wMonth;
		m_timeGuildJoinTime.day		=	g_currentTime.m_wDay;
	}

	if	(!m_lpField->isCanCallMagicCarpetField())
		m_bf5MagicCarpetShape	=	0;

//	if	(isGuildMaster())
//	{
//		CGuild	*lpGuild	=	getGuild();

//		if	(lpGuild)
//			lpGuild->checkRequireMasterGuildSkill(this);
//	}

	if	(m_bf1IsReadyToMakeGuild && m_wGuildSerial != 0xffff)
		m_bf1IsReadyToMakeGuild	=	FALSE;

	m_dwNameHashCode	=	GetHashCode((BYTE *)m_strName);
	m_dwIDHashCode		=	GetHashCode((BYTE *)m_strId);

	if	(m_bf1IsExistExceptExpPortal	==	FALSE	&&	m_wPremiumServiceLevel	==	ePS_FREE && m_lpField->m_wIsPremiumZone && 
		m_wOperatorLevel < eAL_NORMAL && g_bIsTestServer == FALSE)
		m_wIsIncorrectFieldRequireReturnToVillage	=	TRUE;

	if	(m_bf1WaitForRecallForKelby)	
		m_awRemainTimeForRecallSummonBeast[0]	=	dDEFAULT_TIME_FOR_RECALL_SUMMON_BEAST;
	if	(m_bf1WaitForRecallForSwellfer)
		m_awRemainTimeForRecallSummonBeast[1]	=	dDEFAULT_TIME_FOR_RECALL_SUMMON_BEAST;
	if	(m_bf1WaitForRecallForWindy)	
		m_awRemainTimeForRecallSummonBeast[2]	=	dDEFAULT_TIME_FOR_RECALL_SUMMON_BEAST;
	if	(m_bf1WaitForRecallForHedger)
		m_awRemainTimeForRecallSummonBeast[3]	=	dDEFAULT_TIME_FOR_RECALL_SUMMON_BEAST;

	operateUpdateByDayWork(FALSE);

	m_rectActionFrame.Set(0,0,0x7fffffff,0x7fffffff);
	m_rectMoveFrame.Set(0,0,0x7fffffff,0x7fffffff);

	m_bf6ExtraInventorySize		=	min(m_bf6ExtraInventorySize,dEXTRA_INVENTORY_SIZE);

	if	(m_lpField->m_bIsSiegeWarfareField)
	{
		CGuild	*lpGuild	=	getGuild();

		if	(lpGuild)
		{
			CGuildMan	*lpMember	=	lpGuild->getMember(m_strName);

			if	(lpMember)
			{
				m_wGuildPetType					=	lpMember->m_bf3ReservePet;
				m_wGuildPetLevel				=	lpGuild->m_awPetLevel[m_wGuildPetType];
				m_dwLastActionTimeForGuildPet	=	0;
			}
		}
	}

	if	(m_bf5ActiveMiniPet		<	c_iMiniPetCount)
	{
		if	(m_aMiniPet[m_bf5ActiveMiniPet].m_bf7Level	==	0)
			m_bf5ActiveMiniPet	=	c_iMiniPetCount;
	}
	if	(m_bf5ActiveMiniPet2	<	c_iMiniPetCount)
	{
		if	(m_aMiniPet[m_bf5ActiveMiniPet2].m_bf7Level	==	0)
			m_bf5ActiveMiniPet2	=	c_iMiniPetCount;
	}

	iMiniPet()->checkCurrentMiniPetExpiredInfo(FALSE);

	if	(m_bf12DeathPeneltySecond)
	{
		m_wCurrentLevelDeathPenelty	=	getCurrentLevelDeathPeneltyValue();
		

	}

	buildByGuildStragety();
}

void
cACTOR::buildByGuildStragety(BOOL _bIsRecoverHP)
{
	if	(!m_lpField->m_bIsSiegeWarfareField)
		return;

	m_iCorrectHPByGuildStategyPoint			=	0;
	m_iIncreaseDamageByGuildStategyPoint	=	0;
	m_iReduceDamageByGuildStategyPoint		=	0;

	CGuild	*lpGuild	=	getGuild();

	if	(lpGuild)
	{
		m_iIncreaseDamageByGuildStategyPoint	=	lpGuild->m_wIncreaseDamageUnderSiegeWarfare;
		m_iReduceDamageByGuildStategyPoint		=	lpGuild->m_wReduceDamageUnderSiegeWarfare;
	}

	int	iCorrectHPByGuildStategyPointPerFlag=	5;

	if	(m_wTeam	==	0)
		m_iCorrectHPByGuildStategyPoint		=	m_lpField->m_iRemainStategyPoint*iCorrectHPByGuildStategyPointPerFlag;
	else
	if	(m_wTeam	==	1)
	{
		int	iDestroyFlageCount				=	c_iGuildStategyPointCount-m_lpField->m_iRemainStategyPoint;

		m_iIncreaseDamageByGuildStategyPoint+=	iDestroyFlageCount*10;
		m_iReduceDamageByGuildStategyPoint	+=	iDestroyFlageCount*10;
	}

	if	(_bIsRecoverHP)
		m_iHP	=	getMaxHP();
}

//
//	앉아있냐?
BOOL
cACTOR::isSitdown()
{
	if	(m_iAnm	==	dACT_SITDOWN)
		return	TRUE;

	return	FALSE;
}

//
//	모든 보정 데이터를 초기화 시킨다.

void
cACTOR::rebuildBasicData()
{
	m_wAttackRange					=	0;
	m_wAttackSpeed					=	0;

	//	마법 속성 공격에 대한 저항력
	m_fireResistance.m_sValue		=	m_sFireResistance;	//	불꽃 저항
	m_waterResistance.m_sValue		=	m_sWaterResistance;	//	물 저항
	m_windResistance.m_sValue		=	m_sWindResistance;	//	바람 저항
	m_earthResistance.m_sValue		=	m_sEarthResistance;	//	대지 저항
	m_lightResistance.m_sValue		=	m_sLightResistance;	//	빛 저항
	m_darkResistance.m_sValue		=	m_sDarkResistance;	//	어둠 저항

	m_bFreezeResistance				=	(BYTE)m_sFreezeResistance;	//	프리즈 저항

	m_bColdResistance				=	(BYTE)m_sColdResistance;		//	콜드 저항

	m_stunResistance.m_sValue		=	(BYTE)m_sStunResistance;		//	경직 저항

	m_bConfuseResistance			=	(BYTE)m_sConfuseResistance;	//	혼란 저항
	m_charmingResistance.m_sValue	=	m_sCharmingResistance;//	챠밍 저항
	m_bStoneResistance				=	(BYTE)m_sStoneResistance;		//	석화 저항
	m_bPoisonResistance				=	(BYTE)m_sPoisonResistance;	//	중독 저항
	m_bSleepResistance				=	(BYTE)m_sSleepResistance;		//	잠 저항
	m_bBlindResistance				=	(BYTE)m_sBlindResistance;		//	장님 저항

	m_strangeStatusResistance.m_sValue	=	m_sBadStatusResistance;	//	상태 이상 저항
	m_declinePowerResistance.m_sValue	=	m_sDeclinePowerResistance;//	능력치 저하 저항
	m_curseResistance.m_sValue			=	m_sCurseResistance;		//	저주 저항

	m_maxHP.m_iValue				=	m_iMaxHP;
	m_maxCP.m_iValue				=	m_iMaxCP;
	m_level.m_sValue				=	m_iLevel;

//	플레이어
	if	(isPlayer())
	{
		m_defensePower.resetPrefixData();				//	더하거나 빼야 하는 수치
		
		m_iWeaponMinDamage			=	m_sMinDamage*100;		//	최소 데미지
		m_iWeaponMaxDamage			=	m_sMaxDamage*100;		//	최대 데미지
		m_defensePower.m_iValue		=	m_sDefensivePower*100;	//	최소 방어력

		m_strength.m_sValue			=	m_sStrength;		//	힘
		m_agility.m_sValue			=	m_sAgility;			//	민첩성
		m_constitution.m_sValue		=	m_sConstitution;	//	건강
		m_intelligence.m_sValue		=	m_sIntelligence;	//	지식
		m_wisdom.m_sValue			=	m_sWisdom;			//	지혜
		m_charisma.m_sValue			=	m_sCharisma;		//	카리스마
		m_luck.m_sValue				=	m_sLuck;			//	운

		m_wItemSlotCount			=	dDEFAULT_ITEM_SLOT_COUNT;
		m_wPartyExpBonus			=	0;
		m_wBankSize					=	0;
		m_sSight					=	380;


		if	(!m_wTransformationTime && m_wJob	==	dJOB_FIGHTER)
		{
			m_wAttackRange			=	g_fighterAttackInfo.getAttackRange(m_iLevel);


			m_wAttackSpeed			=	g_fighterAttackInfo.getAttackSpeed(m_iLevel)*dSYNC_FPS/100;
			m_iWeaponMinDamage		=	m_sMinDamage*100+g_fighterAttackInfo.getMinAttackPower(m_iLevel);		//	최소 데미지
			m_iWeaponMaxDamage		=	m_sMaxDamage*100+g_fighterAttackInfo.getMaxAttackPower(m_iLevel);		//	최대 데미지
		}

		return;
	}
	
	if	(isSummonBeast())
	{
		m_sSight					=	380;

		cACTOR	*lpTamer	=	getTamer();

		if	(lpTamer)
			buildSummonBeastBasicPower(lpTamer);

		return;
	}

//	몬스터
	cJOB	*lpJob	=	getJob();

//	직업에 의한 데이터 보정
	{
		m_strength.m_sValue			=	(int)m_sStrength*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		m_agility.m_sValue			=	(int)m_sAgility*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		m_constitution.m_sValue		=	(int)m_sConstitution*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;

		if	(isPet())
			m_bIsRevisIntCalc	=	lpJob->m_bf1IsReviseIntCalc;

		if	(m_bIsRevisIntCalc)
		{
			m_intelligence.m_sValue		=	(int)m_sIntelligence*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
			m_wisdom.m_sValue			=	(int)m_sWisdom*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		}
		else
		{
			m_intelligence.m_sValue		=	(int)m_sWisdom*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
			m_wisdom.m_sValue			=	(int)m_sIntelligence*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		}

		m_charisma.m_sValue			=	(int)m_sCharisma*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		m_luck.m_sValue				=	(int)m_sLuck*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;

		m_iWeaponMinDamage			=	(int)m_sMinDamage*100+(getLevel()-1)*lpJob->m_wMinDamageLevelFactor;
		m_iWeaponMaxDamage			=	(int)m_sMaxDamage*100+(getLevel()-1)*lpJob->m_wMaxDamageLevelFactor;
		m_defensePower.m_iValue		=	(int)m_sDefensivePower*100+(getLevel()-1)*lpJob->m_wDefensePowerLevelFactor;
		m_iWeaponMaxDamage			=	max(m_iWeaponMaxDamage,m_iWeaponMinDamage);

		m_correctCriticalHitChance.m_sValue	=	(BYTE)lpJob->m_wCriticalShot;
		m_bCorrectCrushBlowChance	=	(BYTE)lpJob->m_wCrushBlow;
		m_bCriticalHitResistance	=	(BYTE)lpJob->m_sCriticalResistance;			//	크리티컬 저항
		m_bCrushBlowResistance		=	(BYTE)lpJob->m_sCrushResistance;				//	크러쉬 블로우 저항

		m_hitChance.m_sValue		=	lpJob->m_sCorrectHit;
		m_dodgeChance.m_sValue		=	lpJob->m_sCorrectDodge;
		m_blockingChance.m_sValue	=	lpJob->m_wBlocking;
		m_wActionSpeed				=	dSYNC_FPS*lpJob->m_wAttackSpeed/100;
		m_wNpcMoveSpeed				=	lpJob->m_wMoveSpeed;
	}

	buildAddPetPower();
}

void
cACTOR::rebuildForUpdateLevel()
{
	if	(!isMonster())
		return;

	cACTOR	*lpTamer	=	getTamer();

	if	(isSummonBeast())
	{
		if	(!lpTamer)
			return;

		buildSummonBeastBasicPower(lpTamer);
	}

	if	(isPet())
	{
		cJOB	*lpJob				=	getJob();

		m_bIsRevisIntCalc			=	lpJob->m_bf1IsReviseIntCalc;

		m_strength.m_sValue			=	(int)m_sStrength*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		m_agility.m_sValue			=	(int)m_sAgility*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		m_constitution.m_sValue		=	(int)m_sConstitution*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;

		if	(m_bIsRevisIntCalc)
		{
			m_intelligence.m_sValue		=	(int)m_sIntelligence*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
			m_wisdom.m_sValue			=	(int)m_sWisdom*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		}
		else
		{
			m_intelligence.m_sValue		=	(int)m_sWisdom*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
			m_wisdom.m_sValue			=	(int)m_sIntelligence*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		}

		m_charisma.m_sValue			=	(int)m_sCharisma*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		m_luck.m_sValue				=	(int)m_sLuck*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;

		m_iWeaponMinDamage			=	(int)m_sMinDamage*100+(getLevel()-1)*lpJob->m_wMinDamageLevelFactor;
		m_iWeaponMaxDamage			=	(int)m_sMaxDamage*100+(getLevel()-1)*lpJob->m_wMaxDamageLevelFactor;
		m_defensePower.m_iValue		=	(int)m_sDefensivePower*100+(getLevel()-1)*lpJob->m_wDefensePowerLevelFactor;

		buildAddPetPower();
	}

	addSendSimpleInfo();
}

//
//	죽었을때 페널티
void
cACTOR::operateDeathPenelty()
{
	if	(m_lpField->m_bIsGuildBattleField	&&	!m_lpField->m_bIsSiegeWarfareField)
		return;

	if	(m_lpField->m_wIsVillage)
		return;
	
	CClient	*lpClient	=	getClient();

	if	(lpClient	&&	isPlayer())
		if	(lpClient->GetIsNetCafeUser())
			return;


	if	(isPlayer())
	{
		m_bf12DeathPeneltySecond	=	getCurrentLevelDeathPeneltySecond();
		m_wCurrentLevelDeathPenelty	=	getCurrentLevelDeathPeneltyValue();

		if	(m_lpField->m_bIsSiegeWarfareField)
			m_bf12DeathPeneltySecond	=	60;

		sendDeathPeneltySecond(TRUE);
	}

	if	(m_lpField->m_bIsGuildBattleField)
		return;

	if	(isPet())
	{
		m_iLostExperience	=	min(g_aExpTable[m_iLevel]/20,m_iExperience);
		m_iExperience		-=	g_aExpTable[m_iLevel]/20;
		m_iExperience		=	max(m_iExperience,0);

		cACTOR	*lpTamer	=	getTamer();

		if (lpTamer)
		{
			lpTamer->m_aPet[m_wPetIndex].m_dwExperience	=	m_iExperience;
			lpTamer->sendUpdatePetExp(m_wPetIndex);
		}
	}
}

void
cACTOR::recoverDeathPenelty()
{
	int			iPrice	=	getCurrentLevelDeathPeneltyRecoverPrice();

	LONGLONG	llPrice	=	iPrice;
	llPrice				-=	llPrice*m_wCourtesyTitleValue/10000;
	iPrice				=	(int)llPrice;

	if	(m_iGold	<	iPrice)
	{
		sendRegistServerMessage(eRSM_LACK_MONEY_FOR_RECOVER_DEATH_PENELTY);
		return;
	}

	int	iRecoverTime=	getCurrentLevelDeathPeneltySecond()/2;

	iRecoverTime = getFixedLevelDeathPeneltySecondByGuild(iRecoverTime);

	if (iRecoverTime>=	(int)m_bf12DeathPeneltySecond)
		m_bf12DeathPeneltySecond	=	1;
	else
		m_bf12DeathPeneltySecond	-=	iRecoverTime;

	m_bf12DeathPeneltySecond		=	max(m_bf12DeathPeneltySecond,1);

	sendDeathPeneltySecond(FALSE,TRUE);

	if	(iPrice)
	{
		m_iGold				-=	iPrice;
		updateGoldLog(-iPrice,eSPENT_GOLD_BY_RECOVER_DEATH_PENELTY);

		sendAddGold(-iPrice);

		cItem	item;

		item.reset();
		item.m_wBaseItem	=	dITEM_MONEY;
		item.m_dwSerial		=	iPrice;

		sendRemoveItemLog(&item,eRI_RECOVER_DEATH_PENELTY);
	}
}

//
//
void
cACTOR::boostPower(int _iPercentage)
{
	//	마법 속성 공격에 대한 저항력
	m_fireResistance.m_sValue	=	min(m_sFireResistance*_iPercentage/100,100);	//	불꽃 저항
	m_waterResistance.m_sValue	=	min(m_sWaterResistance*_iPercentage/100,100);	//	물 저항
	m_windResistance.m_sValue	=	min(m_sWindResistance*_iPercentage/100,100);	//	바람 저항
	m_earthResistance.m_sValue	=	min(m_sEarthResistance*_iPercentage/100,100);	//	대지 저항
	m_lightResistance.m_sValue	=	min(m_sLightResistance*_iPercentage/100,100);	//	빛 저항
	m_darkResistance.m_sValue	=	min(m_sDarkResistance*_iPercentage/100,100);	//	어둠 저항

	m_bFreezeResistance			=	min(m_sFreezeResistance*_iPercentage/100,100);	//	프리즈 저항
	m_bColdResistance			=	min(m_sColdResistance*_iPercentage/100,100);		//	콜드 저항
	m_stunResistance.m_sValue	=	min(m_sStunResistance*_iPercentage/100,100);		//	경직 저항
	
	m_bConfuseResistance		=	min(m_sConfuseResistance*_iPercentage/100,100);	//	혼란 저항
	m_charmingResistance.m_sValue=	min(m_sCharmingResistance*_iPercentage/100,100);	//	챠밍 저항

	m_bStoneResistance			=	min(m_sStoneResistance*_iPercentage/100,100);		//	석화 저항
	m_bPoisonResistance			=	min(m_sPoisonResistance*_iPercentage/100,100);	//	중독 저항
	m_bSleepResistance			=	min(m_sSleepResistance*_iPercentage/100,100);		//	잠 저항
	m_bBlindResistance			=	min(m_sBlindResistance*_iPercentage/100,100);		//	장님 저항

	m_strangeStatusResistance.m_sValue	=	(BYTE)min(m_sBadStatusResistance*_iPercentage/100,100);	//	상태 이상 저항
	m_declinePowerResistance.m_sValue	=	(BYTE)min(m_sDeclinePowerResistance*_iPercentage/100,100);//	능력치 저하 저항
	m_curseResistance.m_sValue			=	(BYTE)min(m_sCurseResistance*_iPercentage/100,100);		//	저주 저항

	m_maxHP.m_iValue			=	m_iMaxHP*_iPercentage/100;
	m_maxCP.m_iValue			=	m_iMaxCP*_iPercentage/100;
	m_level.m_sValue			=	m_iLevel;


//	몬스터
	cJOB	*lpJob	=	getJob();

//	직업에 의한 데이터 보정
	{
		m_strength.m_sValue			=	(int)m_sStrength*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000*_iPercentage/100;
		m_agility.m_sValue			=	(int)m_sAgility*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000*_iPercentage/100;
		m_constitution.m_sValue		=	(int)m_sConstitution*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000*_iPercentage/100;

		if	(m_bIsRevisIntCalc)
		{
			m_intelligence.m_sValue		=	(int)m_sIntelligence*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000*_iPercentage/100;
			m_wisdom.m_sValue			=	(int)m_sWisdom*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000*_iPercentage/100;
		}
		else
		{
			m_intelligence.m_sValue		=	(int)m_sWisdom*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000*_iPercentage/100;
			m_wisdom.m_sValue			=	(int)m_sIntelligence*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000*_iPercentage/100;
		}

		m_charisma.m_sValue			=	(int)m_sCharisma*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000*_iPercentage/100;
		m_luck.m_sValue				=	(int)m_sLuck*((getLevel()-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000*_iPercentage/100;

		m_iWeaponMinDamage			=	(int)m_sMinDamage*100+(getLevel()-1)*lpJob->m_wMinDamageLevelFactor*_iPercentage/100;
		m_iWeaponMaxDamage			=	(int)m_sMaxDamage*100+(getLevel()-1)*lpJob->m_wMaxDamageLevelFactor*_iPercentage/100;
		m_defensePower.m_iValue		=	(int)m_sDefensivePower*100+(getLevel()-1)*lpJob->m_wDefensePowerLevelFactor*_iPercentage/100;
		m_iWeaponMaxDamage			=	max(m_iWeaponMaxDamage,m_iWeaponMinDamage)*_iPercentage/100;

		m_correctCriticalHitChance.m_sValue	=	lpJob->m_wCriticalShot*_iPercentage/100;
		m_bCorrectCrushBlowChance	=	lpJob->m_wCrushBlow*_iPercentage/100;
		m_bCriticalHitResistance	=	min(lpJob->m_sCriticalResistance*_iPercentage/100,100);	//	크리티컬 저항
		m_bCrushBlowResistance		=	min(lpJob->m_sCrushResistance*_iPercentage/100,100);	//	크러쉬 블로우 저항

		m_hitChance.m_sValue		=	lpJob->m_sCorrectHit*_iPercentage/100;
		m_dodgeChance.m_sValue		=	lpJob->m_sCorrectDodge*_iPercentage/100;
		m_blockingChance.m_sValue	=	lpJob->m_wBlocking*_iPercentage/100;
		m_wActionSpeed				=	dSYNC_FPS*lpJob->m_wAttackSpeed/100;
		m_wNpcMoveSpeed				=	lpJob->m_wMoveSpeed*_iPercentage/100;
	}

	m_iHP	=	getMaxHP();

	cNPC	*lpNpc	=	getNpc();

	if	(lpNpc)
		lpNpc->m_wBoostPower	=	_iPercentage;
}

//
//	체력 회복
void
cACTOR::operateRecoverHP()
{
	if	(m_iHP	<=	0)
	{
		m_iHP	=	0;
		return;
	}

	int		iRecoverHP;

	regenHP();

	if	(isSitdown() || (m_bf5MagicCarpetShape && m_bf1IsExistTurttleCarpet) || (m_bf5MagicCarpetShape ==	eMCS_TURTLE))
	{
//	HP Regen 속도 (앉았을 시) = 1 * (1 + 건강 /100) * (1 + 칭호에 의한 보정치) [포인트/초]
		iRecoverHP	=	(100 + getConstitution())/dSYNC_FPS;
		iRecoverHP	=	iRecoverHP*(m_wCampingMasterTitleFactor+m_wIncreaseSitdownHPRecoverSpeed)/100;

		m_iHP		+=	iRecoverHP;
		m_iHP		=	min(m_iHP,getMaxHP());
	}

	if	(m_iRecoverHP<=	0)
		return;

//	HP Regen 속도 (약을 먹었을 시) = 10 * (1 + 건강 /50) * (1 + 아이템에 의한 보정치) [포인트/초]
	iRecoverHP	=	10*(100+getConstitution()*2)*(100+m_bCorrectHPRecoverSpeed)/100/dSYNC_FPS;
	iRecoverHP	=	min(m_iRecoverHP,iRecoverHP);
	m_iRecoverHP-=	iRecoverHP;

	if	(m_wDeclientRecoverHPEfficientTime)
		iRecoverHP	-=	iRecoverHP*m_wDeclientRecoverHPEfficient/100;

	m_iHP		+=	iRecoverHP;

	m_iHP		=	min(m_iHP,getMaxHP());
	
	if	(m_iRecoverHP<=	0)
	{
		syncTamedMonsterHP(TRUE);
		syncPlayerBasicStatus(TRUE);
	}

	cParty	*lpParty	=	getParty();
	
	if(lpParty)
	{
		cUnionParty*	lpUnionParty	=	g_pUnionpartyManager->getDenIndex(lpParty->m_wUnionPartyserial);
		if(!lpUnionParty)
			return;
		
		lpUnionParty->syncUnionPartyMemberHP(this);
	}	
}

void
cACTOR::healHP(int _iHealPoint)	//	체력 회복
{
	if	(g_bIsDuelServer)
		_iHealPoint	=	_iHealPoint	*g_iCorrectHealEfficientUnderDuelBattle/100;

	operateDegenerateStatus(_iHealPoint);

	m_iRecoverHP	+=	_iHealPoint;
}

//	즉시 체력 회복
BOOL
cACTOR::instanceHealHP(int _iHealPoint)
{
	if(m_iHP<=0)
		return FALSE;

	if	(m_wDeclientRecoverHPEfficientTime)
		return	FALSE;

	if	(g_bIsDuelServer)
		_iHealPoint	=	_iHealPoint	*g_iCorrectHealEfficientUnderDuelBattle/100;

	increaseHP(_iHealPoint);

	operateDegenerateStatus(_iHealPoint);

	return	TRUE;
}

void
cACTOR::increaseHP(int _iHP)
{
	m_iHP			+=	_iHP;
	m_iHP			=	min(m_iHP,getMaxHP());

	if	(m_lpField->m_wDuelType	!=	0xffff)
	{
		SG_REMAIN_HP	packet;

		packet.base.set(sizeof(packet),dSG_REMAIN_HP);

		LONGLONG	llMaxHP		=	getMaxHP();
		LONGLONG	llHP		=	m_iHP;

		llHP					=	llHP*60000/llMaxHP;
		packet.iMaxHP			=	getMaxHP();
		packet.wRemainHPRate	=	(WORD)llHP;
		packet.wActor			=	m_wSerialInField;
		packet.bf1IsSyncUnionParty=	isUnionPartyMember(); // 수정
		packet.bf1IsGuildObject	=	m_bIsGuildObject;		// 길드오브젝트.
		if	(m_iHP > 0)
			packet.wRemainHPRate	=	max(packet.wRemainHPRate,1);

		m_lpField->sendPacketToAllOperator((ALL_MSG*)&packet,4);
	}

	cParty	*lpParty	=	getParty();
	
	if(lpParty)
	{
		cUnionParty*	lpUnionParty	=	g_pUnionpartyManager->getDenIndex(lpParty->m_wUnionPartyserial);
		if(!lpUnionParty)
			return;

		lpUnionParty->syncUnionPartyMemberHP(this);
	}
}
void
cACTOR::reduceCP()
{
	if	(g_bIsDuelServer)
		return;
	if(m_wMadnessTime)		// 광란상태.
		return;
	if	(m_wMaintainMaxCPTime)
	{
		m_iCP	=	getMaxCP();
		return;
	}

	if	(m_wCPFloatTime)
	{
		m_wCPFloatTime--;
		return;
	}

	if	(m_bf5MagicCarpetShape ==	eMCS_SQUIRREL)
		return;

	if	(m_bf5MagicCarpetShape && m_bf1IsExistSquirrelCarpet)
		return;

	if	(m_iCP	<	m_wBottomCP*100)
		return;
	
	if	((m_bf5MagicCarpetShape || m_isRunning) && m_bIsMoving)
	{
		int	iCorrectCP	=	max(10,100-getConcentration());

		int	iReduceCP	=	(45* getCharisma()/100+500)*iCorrectCP/100/dSYNC_FPS;
//● CP 감소 속도 = (-0.0045 * 카리스마 + 5) * 아이템, 기술에 의한 CP 감소율 보정치 [포인트/초]
//			iReduceCP	*=	dREDUCE_CP_RUNNING_FACTOR;

		m_iCP	=	max(m_iCP-iReduceCP,m_wBottomCP*100);
	}
}
//
//	버서커 상태로 만드는 효과 추가
void
cACTOR_STATUS::addMakeBerserk(int _iChance,int _iTime)
{
	if (m_bTargetBerserkChance	<	_iChance)
	{
		m_bTargetBerserkChance	=	_iChance;
		m_wTargetBerserkTime	=	_iTime;
	}
	else
	if (m_bTargetBerserkChance	==	_iChance)
	{
		if (m_wTargetBerserkTime	<	_iTime)
		{
			m_bTargetBerserkChance		=	_iChance;
			m_wTargetBerserkTime		=	_iTime;
		}
	}

	m_bTargetBerserkChance	=	min(m_bTargetBerserkChance,dMAXIMUM_MAKE_BERSERK_CHANCE);
}

void
cACTOR_STATUS::addMakeStone(int _iChance,int _iTime)
{
	if (m_bTargetStoneChance	<	_iChance)
	{
		m_bTargetStoneChance	=	_iChance;
		m_wTargetStoneTime	=	_iTime;
	}
	else
	if (m_bTargetStoneChance	==	_iChance)
	{
		if (m_wTargetStoneTime	<	_iTime)
		{
			m_bTargetStoneChance		=	_iChance;
			m_wTargetStoneTime		=	_iTime;
		}
	}

	m_bTargetStoneChance	=	min(m_bTargetStoneChance,dMAXIMUM_MAKE_STONE_CHANCE);
}
void
cACTOR_STATUS::addMakeSleep(int _iChance,int _iTime)
{
	if (m_bTargetSleepChance	<	_iChance)
	{
		m_bTargetSleepChance	=	_iChance;
		m_wTargetSleepTime	=	_iTime;
	}
	else
	if (m_bTargetSleepChance	==	_iChance)
	{
		if (m_wTargetSleepTime	<	_iTime)
		{
			m_bTargetSleepChance		=	_iChance;
			m_wTargetSleepTime		=	_iTime;
		}
	}

	m_bTargetSleepChance	=	min(m_bTargetSleepChance,dMAXIMUM_MAKE_SLEEP_CHANCE);
}
void
cACTOR_STATUS::addMakeFirstTarget(int _iChance,int _iTime)
{
	if (m_bTargetFirstTargetChance	<	_iChance)
	{
		m_bTargetFirstTargetChance	=	_iChance;
		m_wTargetFirstTargetTime	=	_iTime;
	}
	else
	if (m_bTargetFirstTargetChance	==	_iChance)
	{
		if (m_wTargetFirstTargetTime	<	_iTime)
		{
			m_bTargetFirstTargetChance		=	_iChance;
			m_wTargetFirstTargetTime		=	_iTime;
		}
	}

	m_bTargetFirstTargetChance	=	min(m_bTargetFirstTargetChance,dMAXIMUM_MAKE_FIRSTTARGET_CHANCE);
}
void
cACTOR_STATUS::addMakeSimpleton(int _iChance,int _iTime)
{
	if (m_bTargetSimpltoneChance	<	_iChance)
	{
		m_bTargetSimpltoneChance	=	_iChance;
		m_wTargetSimpltoneTime	=	_iTime;
	}
	else
	if (m_bTargetSimpltoneChance	==	_iChance)
	{
		if (m_wTargetSimpltoneTime	<	_iTime)
		{
			m_bTargetSimpltoneChance		=	_iChance;
			m_wTargetSimpltoneTime		=	_iTime;
		}
	}

	m_bTargetSimpltoneChance	=	min(m_bTargetSimpltoneChance,dMAXIMUM_MAKE_SIMPLTONE_CHANCE);
}

//
//	
void
cACTOR_STATUS::addMakeArmorBreak(int _iChance,int _iTime)
{
	if (m_bTargetArmorBreakChance	<	_iChance)
	{
		m_bTargetArmorBreakChance	=	_iChance;
		m_wTargetArmorBreakTime		=	_iTime;
	}
	else
	if (m_bTargetArmorBreakChance	==	_iChance)
	{
		if (m_wTargetArmorBreakTime	<	_iTime)
		{
			m_bTargetArmorBreakChance	=	_iChance;
			m_wTargetArmorBreakTime		=	_iTime;
		}
	}

	m_bTargetArmorBreakChance	=	min(m_bTargetArmorBreakChance,dMAXIMUM_MAKE_BREAK_ARMOR_CHANCE);
}

void
cACTOR_STATUS::addMakeWeaponBreak(int _iChance,int _iTime)
{
	if (m_bTargetWeaponBreakChance	<	_iChance)
	{
		m_bTargetWeaponBreakChance	=	_iChance;
		m_wTargetWeaponBreakTime	=	_iTime;
	}
	else
	if (m_bTargetWeaponBreakChance	==	_iChance)
	{
		if (m_wTargetWeaponBreakTime	<	_iTime)
		{
			m_bTargetWeaponBreakChance	=	_iChance;
			m_wTargetWeaponBreakTime	=	_iTime;
		}
	}

	m_bTargetWeaponBreakChance	=	min(m_bTargetWeaponBreakChance,dMAXIMUM_MAKE_BREAK_WEAPON_CHANCE);
}

void
cACTOR_STATUS::addMakeConfuse(int _iChance,int _iTime)
{
	if (m_bTargetConfuseChance	<	_iChance)
	{
		m_bTargetConfuseChance	=	_iChance;
		m_wTargetConfuseTime	=	_iTime;
	}
	else
	if (m_bTargetConfuseChance	==	_iChance)
	{
		if (m_wTargetConfuseTime	<	_iTime)
		{
			m_bTargetConfuseChance	=	_iChance;
			m_wTargetConfuseTime	=	_iTime;
		}
	}

	m_bTargetConfuseChance	=	min(m_bTargetConfuseChance,dMAXIMUM_MAKE_CONFUSE_CHANCE);
}

void
cACTOR_STATUS::addMakeCharmed(int _iChance,int _iTime)
{
	if (m_bTargetCharmingChance	<	_iChance)
	{
		m_bTargetCharmingChance	=	_iChance;
		m_wTargetCharmingTime	=	_iTime;
	}
	else
	if (m_bTargetCharmingChance	==	_iChance)
	{
		if (m_wTargetCharmingTime	<	_iTime)
		{
			m_bTargetCharmingChance	=	_iChance;
			m_wTargetCharmingTime	=	_iTime;
		}
	}

	m_bTargetCharmingChance	=	min(m_bTargetCharmingChance,dMAXIMUM_MAKE_CHARM_CHANCE);
}

void
cACTOR_STATUS::addMakeStun(int _iChance,int _iTime)
{
	if (m_bTargetStunChance	<	_iChance)
	{
		m_bTargetStunChance	=	_iChance;
		m_wTargetStunTime	=	_iTime;
	}
	else
	if (m_bTargetStunChance	==	_iChance)
	{
		if (m_wTargetStunTime	<	_iTime)
		{
			m_bTargetStunChance	=	_iChance;
			m_wTargetStunTime	=	_iTime;
		}
	}

	m_bTargetStunChance	=	min(m_bTargetStunChance,dMAXIMUM_MAKE_STUN_CHANCE);
}

void
cACTOR_STATUS::addMakeCold(int _iChance,int _iTime)
{
	if (m_bTargetColdChance	<	_iChance)
	{
		m_bTargetColdChance	=	_iChance;
		m_wTargetColdTime	=	_iTime;
	}
	else
	if (m_bTargetColdChance	==	_iChance)
	{
		if (m_wTargetColdTime	<	_iTime)
		{
			m_bTargetColdChance	=	_iChance;
			m_wTargetColdTime	=	_iTime;
		}
	}

	m_bTargetColdChance	=	min(m_bTargetColdChance,dMAXIMUM_MAKE_COLD_CHANCE);
}

void
cACTOR_STATUS::addMakeFreeze(int _iChance,int _iTime)
{
	if (m_bTargetFreezeChance	<	_iChance)
	{
		m_bTargetFreezeChance	=	_iChance;
		m_wTargetFreezeTime	=	_iTime;
	}
	else
	if (m_bTargetFreezeChance	==	_iChance)
	{
		if (m_wTargetFreezeTime	<	_iTime)
		{
			m_bTargetFreezeChance	=	_iChance;
			m_wTargetFreezeTime	=	_iTime;
		}
	}

	m_bTargetFreezeChance	=	min(m_bTargetFreezeChance,dMAXIMUM_MAKE_FREEZE_CHANCE);
}

void
cACTOR_STATUS::addMakeBlind(int _iChance,int _iTime)
{
	if (m_bTargetBlindChance	<	_iChance)
	{
		m_bTargetBlindChance	=	_iChance;
		m_wTargetBlindTime	=	_iTime;
	}
	else
	if (m_bTargetBlindChance	==	_iChance)
	{
		if (m_wTargetBlindTime	<	_iTime)
		{
			m_bTargetBlindChance	=	_iChance;
			m_wTargetBlindTime	=	_iTime;
		}
	}

	m_bTargetBlindChance	=	min(m_bTargetBlindChance,dMAXIMUM_MAKE_BLIND_CHANCE);
}

void
cACTOR_STATUS::addAddPoisonDamage(int _iDamage,int _iTime)
{
	addAddPoisonDamage(_iDamage,_iDamage,_iTime);
}

void
cACTOR_STATUS::addAddPoisonDamage(int _iMinDamage,int _iMaxDamage,int _iTime)
{
	m_wMinAddPoisonDamage	+=	_iMinDamage*_iTime;
	m_wMaxAddPoisonDamage	+=	_iMaxDamage*_iTime;
	m_wTargetPoisonedTime	+=	_iTime;
	m_wAddPoisonDamageOptionCount++;
}

void
cACTOR_STATUS::calcAddPoisonDamageOption()
{
	if	(m_wAddPoisonDamageOptionCount	==	0	||	m_wTargetPoisonedTime	==	0)
		return;

	m_wTargetPoisonedTime	/=	m_wAddPoisonDamageOptionCount;
	m_wMinAddPoisonDamage	=	m_wMinAddPoisonDamage/m_wTargetPoisonedTime;
	m_wMaxAddPoisonDamage	=	m_wMaxAddPoisonDamage/m_wTargetPoisonedTime;

	m_bAddPoisonMaxDamage	=	min(m_wMinAddPoisonDamage,dMAXIMUM_POISON_DAMAGE);
	m_bAddPoisonMinDamage	=	min(m_wMaxAddPoisonDamage,dMAXIMUM_POISON_DAMAGE);
}

void
cACTOR_STATUS::addAddFireDamage(int _iDamage)
{
	m_wAddFireMinDamage	+=	_iDamage;
	m_wAddFireMaxDamage	+=	_iDamage;

	m_wAddFireMinDamage	=	min(m_wAddFireMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddFireMaxDamage	=	min(m_wAddFireMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
cACTOR_STATUS::addAddWaterDamage(int _iDamage)
{
	m_wAddWaterMinDamage+=	_iDamage;
	m_wAddWaterMaxDamage+=	_iDamage;
	m_wAddWaterMinDamage=	min(m_wAddWaterMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddWaterMaxDamage=	min(m_wAddWaterMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
cACTOR_STATUS::addAddWindDamage(int _iDamage)
{
	m_wAddWindMinDamage	+=	_iDamage;
	m_wAddWindMaxDamage	+=	_iDamage;
	m_wAddWindMinDamage	=	min(m_wAddWindMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddWindMaxDamage	=	min(m_wAddWindMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
cACTOR_STATUS::addAddLightDamage(int _iDamage)
{
	m_wAddLightMinDamage+=	_iDamage;
	m_wAddLightMaxDamage+=	_iDamage;
	m_wAddLightMinDamage=	min(m_wAddLightMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddLightMaxDamage=	min(m_wAddLightMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
cACTOR_STATUS::addAddEarthDamage(int _iDamage)
{
	m_wAddEarthDamage	+=	_iDamage;
	m_wAddEarthDamage	=	min(m_wAddEarthDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
cACTOR_STATUS::addAddDarkDamage(int _iDamage)
{
	m_wAddDarkMinDamage	+=	_iDamage;
	m_wAddDarkMaxDamage	+=	_iDamage;
	m_wAddDarkMinDamage	=	min(m_wAddDarkMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddDarkMaxDamage	=	min(m_wAddDarkMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
cACTOR_STATUS::addAddFireDamage(int _iMinDamage,int _iMaxDamage)
{
	m_wAddFireMinDamage	+=	_iMinDamage;
	m_wAddFireMaxDamage	+=	_iMaxDamage;
	m_wAddFireMinDamage	=	min(m_wAddFireMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddFireMaxDamage	=	min(m_wAddFireMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
cACTOR_STATUS::addAddWaterDamage(int _iMinDamage,int _iMaxDamage)
{
	m_wAddWaterMinDamage+=	_iMinDamage;
	m_wAddWaterMaxDamage+=	_iMaxDamage;
	m_wAddWaterMinDamage=	min(m_wAddWaterMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddWaterMaxDamage=	min(m_wAddWaterMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
cACTOR_STATUS::addAddWindDamage(int _iMinDamage,int _iMaxDamage)
{
	m_wAddWindMinDamage	+=	_iMinDamage;
	m_wAddWindMaxDamage	+=	_iMaxDamage;
	m_wAddWindMinDamage	=	min(m_wAddWindMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddWindMaxDamage	=	min(m_wAddWindMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
cACTOR_STATUS::addAddLightDamage(int _iMinDamage,int _iMaxDamage)
{
	m_wAddLightMinDamage+=	_iMinDamage;
	m_wAddLightMaxDamage+=	_iMaxDamage;
	m_wAddLightMinDamage=	min(m_wAddLightMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddLightMaxDamage=	min(m_wAddLightMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
cACTOR_STATUS::addAddDarkDamage(int _iMinDamage,int _iMaxDamage)
{
	m_wAddDarkMinDamage	+=	_iMinDamage;
	m_wAddDarkMaxDamage	+=	_iMaxDamage;
	m_wAddDarkMinDamage	=	min(m_wAddDarkMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddDarkMaxDamage	=	min(m_wAddDarkMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

//VV     09.08.07			마법추가데미지DX 추가 ~ㄷㄷㄷ 
void
cACTOR_STATUS::addAddNaturalDamageDX(int _iClass, int _iDamage)
{
	m_sAddNaturalDamageDX[_iClass].wMin += _iDamage;
	m_sAddNaturalDamageDX[_iClass].wMax += _iDamage;

	m_sAddNaturalDamageDX[_iClass].wMin	=	min(m_sAddNaturalDamageDX[_iClass].wMin,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE_DX);
	m_sAddNaturalDamageDX[_iClass].wMax =	min(m_sAddNaturalDamageDX[_iClass].wMax ,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE_DX);
}
void
cACTOR_STATUS::addAddNaturalDamageDX(int _iClass, int _iMinDamage,int _iMaxDamage)
{
	m_sAddNaturalDamageDX[_iClass].wMin += _iMinDamage;
	m_sAddNaturalDamageDX[_iClass].wMax += _iMaxDamage;

	m_sAddNaturalDamageDX[_iClass].wMin	=	min(m_sAddNaturalDamageDX[_iClass].wMin,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE_DX);
	m_sAddNaturalDamageDX[_iClass].wMax =	min(m_sAddNaturalDamageDX[_iClass].wMax ,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE_DX);
}

//AA     09.08.07			마법추가데미지DX 추가 ~ㄷㄷㄷ 

void
cACTOR_STATUS::addAddDamageToUndead(int _iDamage)
{
	_iDamage			+=	m_wAddDamageToUndead;
	m_wAddDamageToUndead=	min(_iDamage,dMAXIMUM_CORRECT_VS_ENEMY_VALUE);
}

void
cACTOR_STATUS::addAddDamageToAnimal(int _iDamage)
{
	_iDamage			+=	m_wAddDamageToAnimal;
	m_wAddDamageToAnimal=	min(_iDamage,dMAXIMUM_CORRECT_VS_ENEMY_VALUE);
}

void
cACTOR_STATUS::addAddDamageToHuman(int _iDamage)
{
	_iDamage				+=	m_wAddDamageToHuman;
	m_wAddDamageToHuman		=	min(_iDamage,dMAXIMUM_CORRECT_VS_ENEMY_VALUE);
}

void
cACTOR_STATUS::addAddDamageToHolyAnimal(int _iDamage)
{
	_iDamage				+=	m_wAddDamageToHolyAnimal;
	m_wAddDamageToHolyAnimal=	min(_iDamage,dMAXIMUM_CORRECT_VS_ENEMY_VALUE);
}

void
cACTOR_STATUS::addAddDamageToDemon(int _iDamage)
{
	_iDamage				+=	m_wAddDamageToDemon;
	m_wAddDamageToDemon		=	min(_iDamage,dMAXIMUM_CORRECT_VS_ENEMY_VALUE);
}

int
cACTOR::getFreezeResistance()
{
	int	iFreezeResistance	=	max(m_bFreezeResistance,getStrangeStatusResistance(TRUE));
	int	iBonusResistance	=	min(getStrangeStatusResistance(FALSE)*(getWisdom()+getCharisma())/1000,50);

	iFreezeResistance		+=	iBonusResistance;

	if	(m_wReduceAllResistanceToHalfTime)
		iFreezeResistance	/=	2;

	if	(m_wTeam	==	dTEAM_PLAYER)
		iFreezeResistance	=	min(iFreezeResistance,m_lpField->m_iBossZoneResistenceLimit);

	iFreezeResistance		=	max(iFreezeResistance,0);

	return	min(iFreezeResistance,100);
}

int
cACTOR::getColdResistance()
{
	int	iColdResistance		=	max(m_bColdResistance,getStrangeStatusResistance(TRUE));
	int	iBonusResistance	=	min(getStrangeStatusResistance(FALSE)*(getWisdom()+getCharisma())/1000,50);

	iColdResistance			+=	iBonusResistance;

	if (m_wReduceAllResistanceToHalfTime)
		iColdResistance		/=	2;

	if	(m_wTeam	==	dTEAM_PLAYER)
		iColdResistance		=	min(iColdResistance,m_lpField->m_iBossZoneResistenceLimit);

	iColdResistance			=	max(iColdResistance,0);

	return	min(iColdResistance,100);
}

int
cACTOR::getBlindResistance()
{
	int	iBlindResistance	=	max(m_bBlindResistance,getStrangeStatusResistance(TRUE));
	int	iBonusResistance	=	min(getStrangeStatusResistance(FALSE)*(getWisdom()+getCharisma())/1000,50);

	iBlindResistance		+=	iBonusResistance;

	if	(m_wReduceAllResistanceToHalfTime)
		iBlindResistance	/=	2;

	if	(m_wTeam	==	dTEAM_PLAYER)
		iBlindResistance	=	min(iBlindResistance,m_lpField->m_iBossZoneResistenceLimit);

	iBlindResistance		=	max(iBlindResistance,0);

	return	min(iBlindResistance,100);
}

int
cACTOR::getSleepResistance()
{
	int	iSleepResistance	=	max(m_bSleepResistance,getStrangeStatusResistance(TRUE));
	int	iBonusResistance	=	min(getStrangeStatusResistance(FALSE)*(getWisdom()+getCharisma())/1000,50);

	iSleepResistance		+=	iBonusResistance;

	if	(m_wReduceAllResistanceToHalfTime)
		iSleepResistance	/=	2;

	if	(m_wTeam	==	dTEAM_PLAYER)
		iSleepResistance	=	min(iSleepResistance,m_lpField->m_iBossZoneResistenceLimit);

	iSleepResistance		=	max(iSleepResistance,0);

	return	min(iSleepResistance,100);
}

int
cACTOR::getStunResistance()
{
	int	iStunResistance		=	max(m_stunResistance.getCorrectedValue()+getStunResistanceByPassiveSkill(),getStrangeStatusResistance(TRUE));
	int	iBonusResistance	=	min(getStrangeStatusResistance(FALSE)*(getWisdom()+getCharisma())/1000,50);

	iStunResistance			+=	iBonusResistance;

	if	(m_wReduceAllResistanceToHalfTime)
		iStunResistance		/=	2;

	if	(m_wTeam	==	dTEAM_PLAYER)
		iStunResistance		=	min(iStunResistance,m_lpField->m_iBossZoneResistenceLimit);

	iStunResistance			=	max(iStunResistance,0);

	return	min(iStunResistance,100);
}

int
cACTOR::getConfuseResistance()
{
	int	iConfuseResistance	=	max(m_bConfuseResistance,getStrangeStatusResistance(TRUE));
	int	iBonusResistance	=	min(getStrangeStatusResistance(FALSE)*(getWisdom()+getCharisma())/1000,50);

	iConfuseResistance		+=	iBonusResistance;

	if	(m_wReduceAllResistanceToHalfTime)
		iConfuseResistance	/=	2;

	if	(m_wTeam	==	dTEAM_PLAYER)
		iConfuseResistance	=	min(iConfuseResistance,m_lpField->m_iBossZoneResistenceLimit);

	iConfuseResistance		=	max(iConfuseResistance,0);

	return	min(iConfuseResistance,100);
}

int
cACTOR::getCharmingResistance()
{
	int	iCharmingResistance	=	max(m_charmingResistance.getCorrectedValue(),getStrangeStatusResistance(TRUE));
	int	iBonusResistance	=	min(getStrangeStatusResistance(FALSE)*(getWisdom()+getCharisma())/1000,50);

	iCharmingResistance		+=	iBonusResistance;

	if	(m_wReduceAllResistanceToHalfTime)
		iCharmingResistance	/=	2;

	if	(m_wTeam	==	dTEAM_PLAYER)
		iCharmingResistance	=	min(iCharmingResistance,m_lpField->m_iBossZoneResistenceLimit);

	iCharmingResistance		=	max(iCharmingResistance,0);

	return	min(iCharmingResistance,100);
}

int
cACTOR::getStoneResistance()
{
	int	iStoneResistance	=	max(m_bStoneResistance,getStrangeStatusResistance(TRUE));
	int	iBonusResistance	=	min(getStrangeStatusResistance(FALSE)*(getWisdom()+getCharisma())/1000,50);

	iStoneResistance		+=	iBonusResistance;

	if	(m_wReduceAllResistanceToHalfTime)
		iStoneResistance	/=	2;

	if	(m_wTeam	==	dTEAM_PLAYER)
		iStoneResistance	=	min(iStoneResistance,m_lpField->m_iBossZoneResistenceLimit);

	iStoneResistance		=	max(iStoneResistance,0);

	return	min(iStoneResistance,100);
}

int
cACTOR::getPoisonResistance()
{
	int	iPoisonResistance	=	max(m_bPoisonResistance,getStrangeStatusResistance(TRUE));
	int	iBonusResistance	=	min(getStrangeStatusResistance(TRUE)*(getWisdom()+getCharisma())/1000,50);

	iPoisonResistance		+=	iBonusResistance;

	if	(m_wReduceAllResistanceToHalfTime)
		iPoisonResistance	/=	2;

	if	(m_wTeam	==	dTEAM_PLAYER)
		iPoisonResistance	=	min(iPoisonResistance,m_lpField->m_iBossZoneResistenceLimit);

	iPoisonResistance		=	max(iPoisonResistance,0);

	return	min(iPoisonResistance,100);
}

//
//	상태 이상 저항
int
cACTOR::getStrangeStatusResistance(BOOL _bIsRequireBonus)
{
	if	(m_wImmuneStrageStausTime)
		return	100;

	int	iStrangeStatusResistance	=	m_strangeStatusResistance.getCorrectedValue();
	iStrangeStatusResistance		+=	getStrangeResistanceByPassiveSkill();
	iStrangeStatusResistance		+=	getValuePassiveSkill(eSKILL_PEE_INCREASE_STRANGE_STATUS_RESISTANCE)/100;
	int	iAddValue					=	iStrangeStatusResistance*(getWisdom()+getCharisma())/1000;

	if	(_bIsRequireBonus	==	FALSE)
		iAddValue			=	0;

	iStrangeStatusResistance		+=	min(iAddValue,50);

	iStrangeStatusResistance		=	max(iStrangeStatusResistance,0);

	if	(m_wReduceAllResistanceToHalfTime)
		iStrangeStatusResistance	/=	2;

	if	(m_wTeam	==	dTEAM_PLAYER)
		iStrangeStatusResistance	=	min(iStrangeStatusResistance,m_lpField->m_iBossZoneResistenceLimit);

	return	min(iStrangeStatusResistance,100);
}

int
cACTOR::getCurseResistance()
{
	if	(m_wImmuneStrageStausTime)
		return	100;

	int	iCurseResistance	=	m_curseResistance.getCorrectedValue();
	int	iAddValue			=	iCurseResistance*(getWisdom()+getCharisma())/1000;

	if	(m_wJob	==	dJOB_DEVIL)
		iCurseResistance	+=	20;

	iCurseResistance		+=	min(iAddValue,50);

	if	(m_wReduceAllResistanceToHalfTime)
		iCurseResistance	/=	2;

	if	(m_wTeam	==	dTEAM_PLAYER)
		iCurseResistance	=	min(iCurseResistance,m_lpField->m_iBossZoneResistenceLimit);
	
	iCurseResistance		=	max(iCurseResistance,0);

	cJOB	*lpJob	=	getJob();

	if	(lpJob)
		if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_SEMI_BOSS1)
			iCurseResistance	=	max(iCurseResistance,60);

	return	min(iCurseResistance,100);
}

int
cACTOR::getDeclinePowerResistance()	
{
	if	(m_wImmuneStrageStausTime)
		return	100;

	int	iDeclinePowerResistance	=	m_declinePowerResistance.getCorrectedValue();
	int	iAddValue				=	iDeclinePowerResistance*(getWisdom()+getCharisma())/1000;

	iDeclinePowerResistance		+=	min(iAddValue,50);

	iDeclinePowerResistance		=	max(iDeclinePowerResistance,0);
	
	if	(m_wReduceAllResistanceToHalfTime)
		iDeclinePowerResistance	/=	2;

	if	(m_wTeam	==	dTEAM_PLAYER)
		iDeclinePowerResistance	=	min(iDeclinePowerResistance,m_lpField->m_iBossZoneResistenceLimit);

	return	min(iDeclinePowerResistance,100);
}

//
//	보조 마법 시간 감소
void
cACTOR::decreaseAidMagicTime()
{
	int	iRequireDataRebuildCount	=	0;

	if	(m_wBlockToActionTime)
		--m_wBlockToActionTime;
	if	(m_wImmuneStrageStausTime)
		--m_wImmuneStrageStausTime;
	if	(m_wImmunePhysicalAttackTime)
		--m_wImmunePhysicalAttackTime;
	if	(m_wImmuneMagicAttackTime)
		--m_wImmuneMagicAttackTime;
	if	(m_wUnderBattleTime)
		--m_wUnderBattleTime;
	if	(m_wInhaleTime)
		--m_wInhaleTime;
	if	(m_wFlyTime)
		--m_wFlyTime;
	
	if	(m_sFloatTime			>	0	)
		--m_sFloatTime;					//	공중부양 시간(-이면 장비에 붙어 있는거다.)
	if	(m_sInvisivilityTime	>	0	)
		--m_sInvisivilityTime;			//	인비저빌러티시간(-이면 장비에 붙어 있는거다.)
	if	(m_sNonTargetTime		>	0	)
		--m_sNonTargetTime;				//	타겟에서 제외됨(-1이면 장비에 붙어 있는 것임)
	if	(m_wMaintainMaxCPTime			)
		--m_wMaintainMaxCPTime;			//	이 시간동안 CP가 max로 유지된다.
	if	(m_wMaintainMaxWeponDamageTime	)
		--m_wMaintainMaxWeponDamageTime;//	이 시간동안 무기의 데미지가 항상 max가 나온다.
	if	(m_wDeclientRecoverHPEfficientTime)
		--m_wDeclientRecoverHPEfficientTime;
	if	(m_wInterruptingReviveTime)
		--m_wInterruptingReviveTime;
	if	(m_wBlindTime					)
		--m_wBlindTime;					//	장님 상태 남은 시간

	if	(m_wSoulOathTime)
		--m_wSoulOathTime;				//	혼의 서약
	if	(m_wBloodCompactTime)
		--m_wBloodCompactTime;
	if	(m_wInterruptingArmorTime)
	{
		--m_wInterruptingArmorTime;

		if	(m_wInterruptingArmorTime	==	0)
		{
			m_wInterruptingArmorLevel = 0;
			iRequireDataRebuildCount++;
			buildPower();
		}
	}
	if	(m_wInterruptingWeaponTime)
	{		// 무기해제 사용불가.
		--m_wInterruptingWeaponTime;

		if	(m_wInterruptingWeaponTime	==	0)
		{
			iRequireDataRebuildCount++;
			buildPower();
		}
	}
	if	(m_wNotUseActivityReactionSkillTime)		// 발동형리액션사용금지.
		--m_wNotUseActivityReactionSkillTime;
	if(m_wCorrectFinalAttackPowerPercentageTime)		// 최종공격력퍼센트감소.
		--m_wCorrectFinalAttackPowerPercentageTime;		
	if(m_wConsensusTime)		// 교감상태.
		--m_wConsensusTime;

	if(m_wIncreaseAllStatusTime)	// 모든능력치증가.
	{
		--m_wIncreaseAllStatusTime;
		m_wIncreaseAllStatus	= 0;
	}
	if(m_wMadnessTime)		// 광란상태
		--m_wMadnessTime;
	if(m_wSoulBurnTime)		// 소울번
	{
		--m_wSoulBurnTime;	
		if(m_wSoulBurnTime%m_wSoulBurnDamageSecond == 0)
		{
			operateSoulBurn(m_wSoulBurnDamage,m_wSoulBurnSkill);
			m_wSoulBurnDamage += m_wIncreaseSoulBurnDamage;	
		}
		if(!m_wSoulBurnTime)
		{
			m_wSoulBurnDamage = 0;
			m_wSoulBurnCaster = 0xffff;
			m_dwSoulBurnCasterNameHashCode = 0xffffffff;
			dispell();
			cureCurseStatus();
			cureDeclinePowerStatus();
			cureStrangeStatus();
			cureWrongStatus();
			buildPower();

			if	(isPlayer())
				sendEtcWork(eEW_DISPELL);

			
		}
	}
	if(m_wRageSoulTime)		// 타오르는영혼.
	{
		--m_wRageSoulTime;	
		if(!m_wRageSoulTime)
		{
			m_wRageSoulDamage		=0;
			m_wRageSoulDamageGap	=0;
			m_wRageSoulChance		=0;
		}
	}
	if(m_wBindingWordsTime)
	{
		--m_wBindingWordsTime;
	}

	if	(m_wUseSpecialAbilityTime)
	{
		--m_wUseSpecialAbilityTime;

		if	(m_wUseSpecialAbilityTime	==	0)
			setMonsterReaction(FALSE);
	}

	if	(m_wCorrectFinalDefensivePowerPercentageTime)
	{
		--m_wCorrectFinalDefensivePowerPercentageTime;

		if	(m_wCorrectFinalDefensivePowerPercentageTime	<=	0)
			m_sCorrectFinalDefensivePowerPercentage	=	0;
	}

	if	(m_wMarionetteTime				)
	{
		--m_wMarionetteTime;

		if	(m_wMarionetteTime	==	0	||	getMarionetteLord() == NULL)
			releaseMarionette();
	}

	if	(m_wContractTime)
	{
		--m_wContractTime;

		if	(((g_iCounterBySecond+m_wSerialInField)%m_wContractOccurPeriod)==0)
			operateContract();
	}

	if	(m_wSOD_Time)
		--m_wSOD_Time;

	if	(m_wPoisonTime					)
	{
		--m_wPoisonTime;				//	초당 데미지/10 , 중독 상태 남은 시간

		if	(m_iHP > 100)
		{
			cACTOR	*lpAttacker	=	NULL;

			if	(m_wPoisonDamageCaster	!=	0xffff)
			{
				lpAttacker	=	m_lpField->getActor(m_wPoisonDamageCaster);

				if	(!lpAttacker)
					m_wPoisonDamageCaster	=	0xffff;
			}

			reduceHP(m_iPoisonDamagePerSec,lpAttacker,100);
			addSendSimpleHit(0,m_iPoisonDamagePerSec/100,eAHT_POISON);
		}
	}

	if	(m_wBleedingTime					)
	{
		--m_wBleedingTime;				//

		if	(m_wBleedingTime%m_wBleedingPeriod == 0)
		{
			cACTOR	*lpAttacker	=	NULL;

			if	(m_wBleedingCaster	!=	0xffff)
			{
				lpAttacker	=	m_lpField->getActor(m_wBleedingCaster);

				if	(!lpAttacker)
					m_wBleedingCaster	=	0xffff;
			}

			reduceHP(m_iBleedingDamage,lpAttacker);
			addSendSimpleHit(m_iBleedingDamage/100,0,eAHT_BLEEDING);
		}
	}

	if	(m_wCheckBookingDeathCount)
		--m_wCheckBookingDeathCount;
	if	(m_wRabbitTime)
	{
		--m_wRabbitTime;

		if	(m_wRabbitTime	==	0)
			m_lpField->addSendEtcWork(m_wSerialInField,eEW_TRANS,m_wSerialInField);
	}
	if	(m_wTransToFatGirlTime)
	{
		--m_wTransToFatGirlTime;

		if	(m_wTransToFatGirlTime	==	0)
			m_lpField->addSendEtcWork(m_wSerialInField,eEW_TRANS,m_wSerialInField);
	}

	if	(m_wTransToFrogTime)
	{
		--m_wTransToFrogTime;

		if	(m_wTransToFrogTime	==	0)
			m_lpField->addSendEtcWork(m_wSerialInField,eEW_TRANS,m_wSerialInField);
	}

	if	(m_wTransToWeaponTime)
	{
		--m_wTransToWeaponTime;

		if	(m_wTransToWeaponTime	==	0)
			releasePrincessWeaponStatus(TRUE);
	}

	if	(m_wCaughtTime)
		--m_wCaughtTime;

	if	(m_wRestraintTimeByBattle)
		--m_wRestraintTimeByBattle;

	if	(m_wJumppingSkippingTime)
		--m_wJumppingSkippingTime;

	if	(m_wAnimatePartnerTime)
	{
		--m_wAnimatePartnerTime;

		if	(m_wAnimatePartnerTime	==	0)
		{
			damagedBy(NULL,NULL,m_iHP,m_iHP);
			m_bf1IsInstanceKill			=	TRUE;
			m_bf1IsDeathByAnimateUndead	=	TRUE;
			sendEtcWork(eEW_ANIMATE_PARTNER_TIME,m_wAnimatePartnerTime);
		}
	}
	
	if	(m_wReversalPowerTime)
	{
		--m_wReversalPowerTime;

		if	(m_wReversalPowerTime	==	0)
		{
			if	(isPureMonster()	==	FALSE)
				buildPower();

			if	(isPlayer())
				sendEtcWork(eEW_SET_REVERSAL_POWER_TIME,0);
		}
	}

	if	(m_wShakleUpkeepTime)
	{
		--m_wShakleUpkeepTime;

		if	(m_wShakleUpkeepTime	==	0)
			releaseShakle();
	}

	if	(m_wRemainImpulseTime)
	{
		--m_wRemainImpulseTime;

		if	(m_wRemainImpulseTime	==	0)
			operateImpulseStatus();
	}

	if	(m_wSelfReproofTime)
		--m_wSelfReproofTime;

	if	(m_wSleepTime					)
	{
		if	(m_wSleepTime	>=	0x8000)
			m_wSleepTime	-=	0x8000;

		if	(m_wDarkDamageBySleep)
		{
			cACTOR	*lpAttacker	=	NULL;

			if	(m_wDarkDamageBySleepCaster	!=	0xffff)
			{
				lpAttacker	=	m_lpField->getActor(m_wDarkDamageBySleepCaster);

				if	(!lpAttacker)
					m_wDarkDamageBySleepCaster	=	0xffff;
			}

			int	iDamage		=	m_wDarkDamageBySleep*100;
			iDamage			-=	iDamage*getDarkResistance()/100;

			if	(m_lpField->isPVP(this,lpAttacker)	||	isPlayer())
				iDamage		=	GetP2PCorrectDamage(iDamage);

			if	(m_wCorrectFinalDamage	!=	10000)
			{
				LONGLONG	llDamage;
				llDamage	=	iDamage;
				llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
				iDamage		=	(DWORD)llDamage;
			}

			reduceHP(iDamage,lpAttacker);
			addSendSimpleHit(0,iDamage/100,eAHT_DARK);
		}

		--m_wSleepTime;					//	잠이 깰때까지 남은 시간

		if	(m_wSleepTime	==	0)
			m_wDarkDamageBySleep	=	0;
	}

	if	(m_wElectricShockTime)
	{
		--m_wElectricShockTime;

		if(m_wElectricShockTime == 0)
			m_wElectricShockDamagePercent = 0;

	}
	if	(m_wInstanceHealEffectTime)
	{
		--m_wInstanceHealEffectTime;

		if(m_wInstanceHealEffectTime == 0)
			m_wInstanceHealEffectPercent = 0;

	}
	if	(m_wMakeTypeUndeadTime)
	{
		--m_wMakeTypeUndeadTime;
	}
	if	(m_wColdTime						)
	{
		--m_wColdTime;					//	콜드 상태 남은 시간
		if	(m_wColdTime==0)
			moveDestPos();
	}
	if	(m_wStoneTime					)
		--m_wStoneTime;					//	석화 남은 시간
	if	(m_wFleeTime					)
		--m_wFleeTime;					//	석화 남은 시간
	if	(m_wUpkeepTheOtherSelfTime		)
	{
		--m_wUpkeepTheOtherSelfTime;

		if	(m_wUpkeepTheOtherSelfTime	==	0)
			m_wTheOtherSelfCount	=	0;
	}

	if	(m_wNightMareTime)
		--m_wNightMareTime;

	if	(m_wParalysisTime)
		--m_wParalysisTime;

	if	(m_wConfuseTime					)	
	{
		--m_wConfuseTime;				//	혼란 상태 남은 시간

		operateConfuseAi();
	}
	if	(m_wCharmedTime					)
	{
		--m_wCharmedTime;				//	차밍 상태 남은 시간

		if	(m_wCharmedTime	==	0)
		{
			m_wCharmedTime	=	1;
			releaseCharm();
		}
	}
	if	(m_wFalloffMoraleTime			)
		--m_wFalloffMoraleTime;			//	사기 저하 상태 남은 시간
	if	(m_wTargetedTime				)
		--m_wTargetedTime;				//	타겟 설정 상태 남은 시간
	if	(m_wBreakedArmorTime			)
		--m_wBreakedArmorTime;			//	갑옷 파괴 상태 남은 시간
	if	(m_wBreakedWeaponTime			)
		--m_wBreakedWeaponTime;			//	무기 파괴 상태 남은 시간
	if	(m_wSimpletonTime				)
		--m_wSimpletonTime;				//	바보-_- 상태 남은 시간
	if	(m_wBerserkTime					)
		--m_wBerserkTime;				//	버서크 상태 남은 시간
	if	(m_wTortureTime					)
		--m_wTortureTime;				//	고문률-_-??,고문 시간
	if	(m_wBloodDonorTime				)
		--m_wBloodDonorTime;			//	피흡혈률,피흡혈 당하는 시간
	if	(m_wTauntedToTargetTime			)
	{
		--m_wTauntedToTargetTime;
		if(m_wTauntedToTargetTime	==	0)
		{
			m_wTauntedToTargetSerial		= 0xffff;
			m_wTauntedToTargetDamagePercent	=	0;
			m_dwTauntedToTargetNameHashCode	=	0;
		}
	}
	if	(m_wDegenerateTime				)
		--m_wDegenerateTime;

	if	(m_wBloodDonationTime				)
	{
		--m_wBloodDonationTime;

		if(!m_wBloodDonationTime)
			m_wBloodDonationPercent = 0;
	}

	if	(m_wPOS_Time)
	{
		--m_wPOS_Time;

		cACTOR	*lpCaster	=	m_lpField->getActor(m_wPOS_Caster);

		if	(!lpCaster	||	lpCaster->m_dwNameHashCode	!=	m_dwPOS_CasterNameHashCode)
		{
			m_wPOS_Time		=	0xffff;
			m_wPOS_Caster	=	0xfff;
			lpCaster	=	NULL;

			operatePlotOfShadow(m_wPOS_FullTime-m_wPOS_Time);

			m_wPOS_Time	=	0;
		}
		else
		{
			lpCaster->operatePlotOfShadowCaster(m_wPOS_CasterDamage,m_wPOS_Skill);

			if	(m_wPOS_Time	==	0)
				operatePlotOfShadow(m_wPOS_FullTime);
		}
	}

	if	(m_wMirrorTowerUpkeepTime		)
	{
		--m_wMirrorTowerUpkeepTime;

		if	(m_wMirrorTowerUpkeepTime	==	0)
			m_wMirrorTowerCaster	=	0xffff;
	}

	if	(m_wTerribleDreamTime)
	{
		--m_wTerribleDreamTime;

		if	((g_iCounterBySecond	% m_wTerribleDreamActivePeriod)	==	0)
			operateTerribleDream();
	}

	if	(m_wHwabyungTime)
	{
		--m_wHwabyungTime;

		if	(m_wHwabyungTime	==	0)
			operateHwabyung();
	}

	if	(m_wJewelTime					)
	{	
		--m_wJewelTime;					//	보석 유지 시간
		
		if	(m_wJewelTime	==	0)
			++iRequireDataRebuildCount;
	}

	if	(m_wFirstAidTime)	//	응급처치 업데이트
	{
		m_iFirstAidHP	=	min(m_iFirstAidHP,getMaxHP()-m_iHP);
		--m_wFirstAidTime;

		if	(m_wFirstAidTime	==	0)
		{
			m_iFirstAidHP	=	0;
			addSendFirstAidHP();
		}
	}

	if	(m_wUntimateBarrierTime)
		--m_wUntimateBarrierTime;

	if	(m_iShakleHP	&&	m_bf4RemainTimeForDamageByShakleSkill)
	{
		--m_bf4RemainTimeForDamageByShakleSkill;

		if	(m_bf4RemainTimeForDamageByShakleSkill	==	0)
			strikeShakleSkillDamage();
	}

	if	(m_wNotificationOfDeathCounter)
	{
		--m_wNotificationOfDeathCounter;

		operateNotificationOfDeathCounter();
	}
	
	m_attackPower.decreaseTime();	//	공격력
	m_defensePower.decreaseTime();	//	방어력
	m_correctLastDamage.decreaseTime();

	if	(m_wIncreaseFinalDamageTime	&&	m_lpField->m_bIsGuildBattleField	==	FALSE)
		--m_wIncreaseFinalDamageTime;
	
	m_weakningTargetFireResistance.decreaseTime();			//	모든 마법 저항;
	m_weakningTargetWaterResistance.decreaseTime();			//	모든 마법 저항;
	m_weakningTargetWindResistance.decreaseTime();			//	모든 마법 저항;
	m_weakningTargetEarthResistance.decreaseTime();			//	모든 마법 저항;
	m_weakningTargetLightResistance.decreaseTime();			//	모든 마법 저항;
	m_weakningTargetDarkResistance.decreaseTime();			//	모든 마법 저항;
	m_weakningTargetMagicResistance.decreaseTime();			//	모든 마법 저항;

	m_strengtheningFireDamage.decreaseTime();			//	모든 마법 저항;
	m_strengtheningWaterDamage.decreaseTime();			//	모든 마법 저항;
	m_strengtheningWindDamage.decreaseTime();			//	모든 마법 저항;
	m_strengtheningEarthDamage.decreaseTime();			//	모든 마법 저항;
	m_strengtheningDarkDamage.decreaseTime();			//	모든 마법 저항;
	m_strengtheningLightDamage.decreaseTime();			//	모든 마법 저항;
	m_strengtheningMagicDamage.decreaseTime();			//	모든 마법 저항;
	m_correctCriticalHitChance.decreaseTime();
	m_correctAllSkillLevel.decreaseTime();
	m_strengtheningCriticalDamage.decreaseTime();			// 치명타대미지시간 감소.
	m_correctRightPunchSkillLevel.decreaseTime();
	m_correctLeftPunchSkillLevel.decreaseTime();
	m_correctRightKickSkillLevel.decreaseTime();
	m_correctLeftKickSkillLevel.decreaseTime();
	m_correctDownKickSkillLevel.decreaseTime();
	m_bonusExp.decreaseTime();
	m_bonusPetExp.decreaseTime();

	if	(m_maxHP.decreaseTime())
	{
		m_iHP	=	min(m_iHP,getMaxHP());	//	최대 HP

		addSendSimpleInfo();
	}
	
	if	(m_maxCP.decreaseTime())
		m_iCP	=	min(m_iCP,getMaxCP());	//	최대 CP
	
	if	(m_level.decreaseTime())
	{
		if	(isPlayer())
			buildPower();
		else
			rebuildForUpdateLevel();
		
		addSendSimpleInfo();

		if	(getPartySerial()	!=	0xffff)
		{
			g_pPartyManager->sendWPChangeMemberInfo(getPartySerial(),m_strName,m_iLevel,m_bf4SecretDungeonPlayCount,m_wJob,m_bf3GuildDungeonPlayTime);
			sendEtcWorkToPartyMember(TRUE,eEW_LEVEL_DRAIN,m_wSerialInField,0,0,FALSE,getLevel(),1);
		}
	}

	m_blockingChance.decreaseTime();
	m_regenHPPerSecond.decreaseTime();
	m_strangeStatusResistance.decreaseTime();
	m_curseResistance.decreaseTime();
	m_declinePowerResistance.decreaseTime();

	m_addFireDamage.decreaseTime();
	m_addWaterDamage.decreaseTime();
	m_addWindDamage.decreaseTime();
	m_addLightDamage.decreaseTime();
	m_addDarkDamage.decreaseTime();
	m_addEarthDamage.decreaseTime();

	iRequireDataRebuildCount	+=	m_strength.decreaseTime();						//	힘 
	iRequireDataRebuildCount	+=	m_agility.decreaseTime();						//	민첩성
	iRequireDataRebuildCount	+=	m_constitution.decreaseTime();					//	건강
	iRequireDataRebuildCount	+=	m_intelligence.decreaseTime();					//	지식
	iRequireDataRebuildCount	+=	m_wisdom.decreaseTime();						//	지혜
	iRequireDataRebuildCount	+=	m_charisma.decreaseTime();						//	카리스마
	iRequireDataRebuildCount	+=	m_luck.decreaseTime();							//	운

	m_actionSpeed.decreaseTime();					//	액션 속도 보정

	if	(m_wReduceAllResistanceToHalfTime)
		m_wReduceAllResistanceToHalfTime--;
	
	if	(m_wDodgeMagicDamageTime)
	{
		if	(m_wDodgeMagicDamageTime	!=	0xffff)
			m_wDodgeMagicDamageTime--;
	}

	m_hitChance.decreaseTime();						//	명중률 보정
	m_dodgeChance.decreaseTime();					//	회피 보정
	m_concentration.decreaseTime();
	m_concentrationByStellarPeer.decreaseTime();
	m_allignment.decreaseTime();					//	성향
	m_charmingResistance.decreaseTime();			//	유혹 저항

	m_fireResistance.decreaseTime();				//	불꽃 저항
	m_waterResistance.decreaseTime();				//	물 저항
	m_windResistance.decreaseTime();				//	바람 저항
	m_earthResistance.decreaseTime();				//	대지 저항
	m_lightResistance.decreaseTime();				//	빛 저항
	m_darkResistance.decreaseTime();				//	어둠 저항
	m_allMagicResistance.decreaseTime();			//	모든 마법 저항
	m_detecting.decreaseTime();
	m_returnDamageRate.decreaseTime();
	m_returnDamageRateIDEF.decreaseTime();
	m_returnDamageByRootRate.decreaseTime();

	m_detectTrap.decreaseTime();
	m_detectSecretObject.decreaseTime();

	m_armorPower.decreaseTime();			//	기본 방어력 증가
	m_weaponPower.decreaseTime();			//	기본 공격력 증가

	if	(m_wConversionIceTime)
		m_wConversionIceTime--;
	if	(m_wFogFormTime)
		m_wFogFormTime--;
	if	(m_wNoMoreFightTime)
		m_wNoMoreFightTime--;

	if	(m_wDanceTime)
	{
		m_wDanceTime--;

		if	(m_wDanceTime	==	0)
			releaseDance();
	}

	if	(m_wInMagicBoxTime)
	{
		--m_wInMagicBoxTime;

		cACTOR	*lpCaster	=	m_lpField->getActor(m_wMagicBoxCasterSerial);

		if	(!lpCaster	||	lpCaster->m_dwNameHashCode	!=	m_dwMagicBoxCasterNameHashCode)
			m_wInMagicBoxTime	=	0;

		if	(m_wInMagicBoxTime	==	0)
		{
			m_wMagicBoxCasterSerial			=	0xffff;
			m_dwMagicBoxCasterNameHashCode	=	0xffffffff;
		}
	}

	if	(m_wRallyTime)
	{
		m_wRallyTime--;

		if	(m_wRallyTime	==	0)
			m_wBattleInclination	=	m_wOrgBattleInclination;
	}

	if	(m_wBoostTime)
	{
		m_wBoostTime--;
		
		if	(m_wBoostTime	==	0)
		{
			m_wBoostPower			=	100;
			iRequireDataRebuildCount	=	TRUE;

			if	(isPlayer())
				sendEtcWork(eEW_TIME_OVER_BERSERK_TIME);
		}
	}
	m_fightEnergy.decreaseTime();
	if(m_wRushTime)		// 돌진시간.
		--m_wRushTime;
	if(m_wFightingSpiritTime)		// 투지.
		--m_wFightingSpiritTime;
	if(m_wIgnoreNeedStateTime)		// 필요상태무시
		--m_wIgnoreNeedStateTime;
	if(m_wIgnoreBlockingTime)		// 블럭킹무시
	{
		--m_wIgnoreBlockingTime;
		if(m_wIgnoreBlockingTime == 0)
			m_wIgnoreBlockingValue	= 0;
	}
	if(m_wReturnMissileDamageTime)		// 미사일대미지리턴
	{
		--m_wReturnMissileDamageTime;
		if(m_wReturnMissileDamageTime == 0)
			m_wReturnMissileDamageValue	= 0;
	}
	if(m_wIncreaseHardBlowTime)		// 강타확률증가
	{
		--m_wIncreaseHardBlowTime;
		if(m_wIncreaseHardBlowTime == 0)
			m_wHardBlowValue	= 0;
	}

	if(m_wTransformationTime)
	{
		--m_wTransformationTime;
		if(m_wTransformationTime == 0)
		{
			releaseTransformationMonster();
			addSendSimpleInfo();
			if	(isPlayer())
				sendEtcWork(eEW_END_TRANSFORMATION_MONSTER);
		}
	}
	if(m_wCarvingOptionSoulGuardTime)
		--m_wCarvingOptionSoulGuardTime;

	decreaseBitTime();

	if	(iRequireDataRebuildCount)
		buildPower();
}

void
cACTOR::dispellAllBuff()
{
	dispell();
	cureWrongStatus();
	buildPower();	
}

void
cACTOR::releaseTransformationMonster()
{

	m_wTransformationTime = 0;
	m_wTransformationLevel = 0;
	m_wTransformationSkin	=	0xffff;
	m_wTransformationSkill	=	0xffff;

}

void			
cACTOR::decreaseBitTime()
{

	for(int i=0;i<m_bitInfoManager.m_iCount;++i)
	{
		WORD wSerialInField = m_bitInfoManager.m_bitInfo[i].m_wTargetSerial;
		DWORD dwNameHashCode = m_bitInfoManager.m_bitInfo[i].m_dwNameHashCode;
		if(m_bitInfoManager.decreaseTime(i))
		{
			m_lpField->addSendEtcWork(0xffff,eEW_REMOVE_BIT,m_wSerialInField, wSerialInField , dwNameHashCode, 1);
			--i;
		}
	}
}



void
cACTOR::buildPower()
{
	int		i;

	m_bf1IsUseAbleBulletSubstituteArrow	=	FALSE;
	m_wCorrectMiniPetGainExpByFeedItem	=	1;
	m_wCorrectSecretDungeonPlayCount	=	0;
	m_wCorrectGuildDungeonPlayCount		=	0;
	m_bf1AddEntryGuildDungeon			=	FALSE;	//	
	m_bf1IsAddGetMysticStone			=	FALSE;	//	신비석 추가 획득 
	m_bf1IsAddGetTantalissRelic			=	FALSE;	//	탄탈리스 유물 추가 획득 
	m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile	=	FALSE;	//	길드홀 레벨 제한 없이 탄탈리스 유적 입장 
	m_bf1IsInfinitySeriousUpgradeChanceUp				=	FALSE;	//	

	resetEquipmentData();
	rebuildBasicData();

	operateTitleData();
	operateSpecialItemEffectInInventory();
	iMiniPet()->operatePowerUpSkill();
	operateGuildSkillData();
	operateGGG();				//GGG = GreateGodGrace; 
	operateEquipmentData();
	operatePassiveSkill();		// 패시브스킬들저장..
	if	(m_wSurvivalInstinctsTitleTriggerHP)
	{
		LONGLONG	llLimitHPByPercentage		=	getMaxHP();

		llLimitHPByPercentage					=	llLimitHPByPercentage*m_wSurvivalInstinctsTitleTriggerPropertion/100/100;

		m_wSurvivalInstinctsTitleTriggerHP		=	min((int)llLimitHPByPercentage,m_wSurvivalInstinctsTitleTriggerHP);
	}

	m_iHP	=	min(m_iHP,getMaxHP());

	if	(m_sInvisivilityTime)
	{
		m_sBlurTime		=	0;
		m_sNonTargetTime=	0;
	}

	if	(m_sBlurTime)
		m_sNonTargetTime=	0;

	if	(m_sInvisivilityTime	<	0)
	{
		m_wHidingLevel							=	dINVISIBLE_HIDNIG_LEVEL;
		m_wFoundOutChanceInInvisible			=	1000;
		m_wFoundOutChanceInInvisibleForStand	=	1000;
	}

	if	(m_sBlurTime)
		m_wHidingLevel	=	dBLUR_HIDNIG_LEVEL;
	if	(m_sNonTargetTime)
		m_wHidingLevel	=	dNON_TARGET_HIDNIG_LEVEL;

	for (i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		cACTOR	*lpSummonBeast	=	getSummonBeastActor(i);

		if (!lpSummonBeast || lpSummonBeast->isDeath())
			continue;

		lpSummonBeast->rebuildForUpdateLevel();
	}

	for (i=0;i<dOWN_PET_COUNT;i++)
	{
		cACTOR	*lpPet	=	getPetActor(i);

		if	(!lpPet	|| lpPet->isDeath())
			continue;

		lpPet->rebuildForUpdateLevel();
	}
}

//
//	체력 줄어듦
void
cACTOR::reduceHP(int _iDamage,cACTOR *_lpAttacker,int _iLimitHP,BOOL _bIsByDamagePool,BOOL _bIsByMiniPet)
{
	m_wUnderBattleTime	=	5;

	if	(_lpAttacker)
	{
		_lpAttacker->m_wUnderBattleTime	=	5;

		if	(_lpAttacker	!=	this)
			m_lpLastAttacker=	_lpAttacker;
	}

	if	(m_iGuildGuardianType)
		return;

	if	(m_wTransToWeaponTime)
		return;

	if	((m_iHP <= 0 && isDeath()) || _iDamage == 0	||	m_bf1IsHide)
		return;
	
#ifdef	_DEBUG
	if	(_lpAttacker && _lpAttacker->m_bf1DevIsOneShotOneKill)
		_iDamage	=	m_iHP;
	else
	if	(m_bf1DevIsOneShotOneDeath)
		_iDamage	=	m_iHP;
	else
	if	(_lpAttacker && _lpAttacker->m_bf7DevTargetReduceHPRate)
		_iDamage	=	getMaxHP()*_lpAttacker->m_bf7DevTargetReduceHPRate/100;
	else
	if	(m_bf7DevMyReduceHPRate)
		_iDamage	=	getMaxHP()*m_bf7DevMyReduceHPRate/100;
#endif

	if	(isPlayer() && _bIsByMiniPet == FALSE)
		iMiniPet()->operateMiniPetDamagedTriggerSkill();

	int	iOldGuildObjectHP		=	0;
	int	iCurrentGuildObjectHP	=	0;

	if	(m_bIsGuildObject)
	{
		_iDamage			=	min(_iDamage,200);
		iOldGuildObjectHP	=	m_iHP*100/getMaxHP();
	}

	operateHitNpcReaction(_lpAttacker,_iDamage);

	if	(m_wInvincibleTime)
		_iDamage	=	0;

#ifdef	_FOR_CHINA
	//	중국제어
	{
		int		iCheckTimeValue		=	2005*31*12 + 12*31 + 14;;
		int		iCurrentTimeValue	=	g_currentTime.m_wYear*31*12+g_currentTime.m_wMonth*31+g_currentTime.m_wDay;
		int		iTimeValue			=	iCurrentTimeValue-iCheckTimeValue;

		if	(iTimeValue	>	0)
			if	(isPlayer())
				if	(random(100)	==	0)
					_iDamage		=	max(m_iHP-100,10);
	}
#endif

	m_iLastDamagePoint	=	_iDamage;

	//	충동 제거
	if	(_lpAttacker)
		_lpAttacker->m_wRemainImpulseTime	=	0;

	if	(m_iFirstAidHP	!=	0)
	{
		if	(_iDamage	<=	m_iFirstAidHP)
		{
			m_iFirstAidHP-=	_iDamage;
			_iDamage	=	0;
		}
		else
		{
			_iDamage	-=	m_iFirstAidHP;
			m_iFirstAidHP=	0;
		}

		if	(m_iFirstAidHP	<=	0)
		{
			m_iFirstAidHP	=	0,m_wFirstAidTime	=	0;
			addSendFirstAidHP();
		}
	}

	if	(_bIsByDamagePool	==	FALSE && _bIsByMiniPet == FALSE)
		_iDamage	=	operateDamagePool(_iDamage,_lpAttacker);

	m_iHP	-=	_iDamage;

	int	iOverDamage	=	-m_iHP;

	m_iHP	=	max(m_iHP,_iLimitHP);

	if	(m_wCopyActorLord	!=	0xffff)
	{
		cACTOR	*lpLord	=	m_lpField->getActor(m_wCopyActorLord);

		if	(lpLord)
			sendRemainHP(lpLord);
	}

	if	(m_bIsGuildObject)
	{
		SG_REMAIN_HP	packet;

		packet.base.set(sizeof(packet),dSG_REMAIN_HP);

		LONGLONG	llMaxHP		=	getMaxHP();
		LONGLONG	llHP		=	m_iHP;

		llHP					=	llHP*60000/llMaxHP;
		packet.wRemainHPRate	=	(WORD)llHP;
		packet.wActor			=	m_wSerialInField;
		packet.bf1IsSyncUnionParty=	isUnionPartyMember();		// 수정.
		packet.bf1IsGuildObject		= m_bIsGuildObject;			// 추가. 길드오브젝트.

		if	(m_iHP > 0)
			packet.wRemainHPRate	=	max(packet.wRemainHPRate,1);

		m_lpField->sendPacketToAllPlayer(&packet);
	}
	else
	if	(_lpAttacker)
	{
		cACTOR	*lpActor	=	_lpAttacker;

		if	(_lpAttacker->m_wCopyActorLord	!=	0xffff)
			lpActor	=	m_lpField->getActor(_lpAttacker->m_wCopyActorLord);

		if	(lpActor	&&	lpActor->isPlayer())
		{
			cACTOR	*lpAttackerTarget	=	lpActor->m_lpTarget;

			if	(lpAttackerTarget == NULL)
				sendRemainHP(lpActor);
			else
			{
				if	(lpAttackerTarget->m_wSerialInField == m_wSerialInField)
					sendRemainHP(lpActor);
			}
		}
	}

	cParty	*lpParty	=	getParty();

	if	(m_iHP	<=	0	&&	m_wAnimatePartnerTime)
	{
		if	(isTiamath())
		{
			cACTOR	*lpFeeler;

			lpFeeler	=	m_lpField->getActorByBody(dJOB_MONSTER_TIAMATH_FEELER);

			if	(lpFeeler)
			{
				lpFeeler->reduceHP(iOverDamage);

				if	(lpFeeler->m_iHP	>	0)
				{
					lpFeeler->m_iHP	/=	2;

					instanceHealHP(lpFeeler->m_iHP);
					m_lpField->sendAbsorbDamage(m_wSerialInField,lpFeeler->m_iHP);
				}
			}
			else
			{
				lpFeeler	=	m_lpField->getActorByBody(dJOB_MONSTER_TIAMATH_BOTTOM_FEELER);

				if	(lpFeeler)
				{
					lpFeeler->reduceHP(iOverDamage);
					
					if	(lpFeeler->m_iHP	>	0)
					{
						lpFeeler->m_iHP	/=	2;
						
						instanceHealHP(lpFeeler->m_iHP);
						m_lpField->sendAbsorbDamage(m_wSerialInField,lpFeeler->m_iHP);
					}
				}
			}
		}

		if	(m_iHP	<=	0)
		{
			if	(m_wAnimatePartnerReviveTime)
			{
				if	(m_wAnimatePartnerTime	>	m_wAnimatePartnerReviveTime)
				{
					m_wAnimatePartnerTime	-=	m_wAnimatePartnerReviveTime;

					m_iHP					=	getMaxHP();

					addSendRevive();
				}
				else
					m_wAnimatePartnerTime	=	0;

				sendEtcWork(eEW_ANIMATE_PARTNER_TIME,m_wAnimatePartnerTime);
			}
			else
			{
				int	iDecreaseTime	=	-(m_iHP/100)+1;

				if	(iDecreaseTime	>	m_wAnimatePartnerTime)
				{
					m_wAnimatePartnerTime		=	0;
					m_bf1IsAnimatePartnerStatus	=	FALSE;
				}
				else
				{
					m_wAnimatePartnerTime		-=	iDecreaseTime;
					m_iHP						=	100;
				}
			}
		}
	}

	if	(m_lpField->m_wDuelType	!=	0xffff)
	{
		SG_REMAIN_HP	packet;

		packet.base.set(sizeof(packet),dSG_REMAIN_HP);

		LONGLONG	llMaxHP		=	getMaxHP();
		LONGLONG	llHP		=	m_iHP;

		llHP					=	llHP*60000/llMaxHP;
		packet.iMaxHP			=	getMaxHP();
		packet.wRemainHPRate	=	(WORD)llHP;
		packet.wActor			=	m_wSerialInField;
		packet.bf1IsSyncUnionParty=	isUnionPartyMember();		// 수정.
		packet.bf1IsGuildObject =	m_bIsGuildObject;			// 길드오브젝트.. 추가.
		if	(m_iHP > 0)
			packet.wRemainHPRate	=	max(packet.wRemainHPRate,1);

		m_lpField->sendPacketToAllOperator((ALL_MSG*)&packet,4);
	}

	if	(lpParty)
		lpParty->syncMemberHP(this);

	if	(m_wOperatorLevel >= eAL_OPERATOR && m_bf1DevIsOneShotOneDeath == FALSE)
		m_iHP	=	max(m_iHP,1);

	if	(m_iHP	<=	0)
	{
		bookingDeath(_lpAttacker);	//	_lpAttacker가 죽였어요!! -o-;;
		operateDeathPenelty();
		m_GGG.ResetGGG();
		sendGGGSkillUp(TRUE);
	}
	else
	{
	//	피 흡혈 상태다.
		if	(_lpAttacker	&&	_lpAttacker->m_wBloodDonorTime)
			_lpAttacker->operateBloodDrainStatus(this,_iDamage);

		if	(m_bFirstAidPercentage)
			if	(random(100) < 10)
				firstAid(_iDamage*m_bFirstAidPercentage/100);
	}
}

//
//	게임을 종료 할 수 없는 상황
BOOL
cACTOR::isCanNotCloseGameStatus()
{
	if	(m_wWaitBankDataSaveResultTime || m_wRestraintTimeByBattle	||	m_wWaitBuyCarrotShopItemResultTime || m_wWaitTransGVGResultTime)
		return	TRUE;

	return	FALSE;
}

int
cACTOR::getConcentration()
{
	int iPassiveConcentration = getValuePassiveSkill(eSKILL_PEE_INCREASE_CONCENTRATION)/100;

	if	(m_sFloatTime != 0)
		return	max(m_concentration.getCorrectedValue()+m_concentrationByStellarPeer.getCorrectedValue() + iPassiveConcentration,dCONCENTRATION_FOR_LEVITATE);

	return	m_concentration.getCorrectedValue()+m_concentrationByStellarPeer.getCorrectedValue() + iPassiveConcentration;
}

int				
cACTOR::getFireResistance(int _iCorrectValue)
{
	int	iResistance	=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAGIC_RESISTANCE)/100 + m_allMagicResistance.getCorrectedValue()+m_fireResistance.getCorrectedValue()+getWisdom()*5/100;
	iResistance		+=	getFireResistanceBonusByA();

	if	(m_wJob	==	dJOB_DEVIL)
		iResistance	+=	15;

	if	(m_wRidingDogTime)
	{
		cAbility	*lpRidingDogSkill	=	getRidingDogSkill();

		if	(lpRidingDogSkill)
			iResistance	+=	10+lpRidingDogSkill->m_wLevel+getCorrectSkillLevel(lpRidingDogSkill);
	}

	if	(m_wParalysisTime)
		return	0;

	iResistance						+=	_iCorrectValue;

	int	iFieldElementalResistance	=	m_lpField->m_sCorrectFireResistance;
	iFieldElementalResistance		-=	m_wCorrectFieldElementalResistanceByTitle;
	iFieldElementalResistance		-=	m_bCorrectFieldFirePenelty;


	CGuild	*lpGuild				=	getGuild();

	if	(lpGuild)
		iFieldElementalResistance	-=	(lpGuild->m_wDecreasePeneltyOfFireResistance+lpGuild->m_wDecreaseDungeonMagicPenelty);

	iFieldElementalResistance		=	max(iFieldElementalResistance,0);

	if	(isPlayer() || isPet() || isSummonBeast())
	{
		iResistance	=	min(iResistance-iFieldElementalResistance,dMAXIMUM_PLAYER_MAGIC_RESISTANCE_VALUE);

		if	(m_wTeam	==	dTEAM_PLAYER)
			iResistance	=	min(iResistance,m_lpField->m_iBossZoneResistenceLimit);
	}
	else
	if	(isPureMonster())
		iResistance	+=	m_lpField->m_sCorrectMonsterFireResistance;

	return	min(iResistance,dMAXIMUM_MONSTER_MAGIC_RESISTANCE_VALUE);
}

int				
cACTOR::getWaterResistance(int _iCorrectValue)
{
	int	iResistance	=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAGIC_RESISTANCE)/100 + m_allMagicResistance.getCorrectedValue()+m_waterResistance.getCorrectedValue()+getWisdom()*5/100;
	iResistance		+=	getWaterResistanceBonusByA();

	if (m_wParalysisTime)
		return	0;

	iResistance		+=	_iCorrectValue;

	int	iFieldElementalResistance	=	m_lpField->m_sCorrectWaterResistance;
	iFieldElementalResistance		-=	m_wCorrectFieldElementalResistanceByTitle;
	iFieldElementalResistance		-=	m_bCorrectFieldWaterPenelty;

	CGuild	*lpGuild				=	getGuild();

	if	(lpGuild)
		iFieldElementalResistance	-=	(lpGuild->m_wDecreasePeneltyOfWaterResistance+lpGuild->m_wDecreaseDungeonMagicPenelty);

	iFieldElementalResistance		=	max(iFieldElementalResistance,0);

	if	(isPlayer() || isPet() || isSummonBeast())
	{
		iResistance	=	min(iResistance-iFieldElementalResistance,dMAXIMUM_PLAYER_MAGIC_RESISTANCE_VALUE);
		if	(m_wTeam	==	dTEAM_PLAYER)
			iResistance	=	min(iResistance,m_lpField->m_iBossZoneResistenceLimit);
	}
	else
	if	(isPureMonster())
		iResistance	+=	m_lpField->m_sCorrectMonsterWaterResistance;

	return	min(iResistance,dMAXIMUM_MONSTER_MAGIC_RESISTANCE_VALUE);
}

int				
cACTOR::getWindResistance(int _iCorrectValue)
{
	int	iResistance	=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAGIC_RESISTANCE)/100 + m_allMagicResistance.getCorrectedValue()+m_windResistance.getCorrectedValue()+getWisdom()*5/100;
	iResistance		+=	getWindResistanceBonusByA();

	if (m_wParalysisTime)
		iResistance	=	0;

	iResistance		+=	_iCorrectValue;

	int	iFieldElementalResistance	=	m_lpField->m_sCorrectWindResistance;
	iFieldElementalResistance		-=	m_wCorrectFieldElementalResistanceByTitle;
	iFieldElementalResistance		-=	m_bCorrectFieldWindPenelty;

	CGuild	*lpGuild				=	getGuild();

	if	(lpGuild)
		iFieldElementalResistance	-=	(lpGuild->m_wDecreasePeneltyOfWindResistance+lpGuild->m_wDecreaseDungeonMagicPenelty);

	iFieldElementalResistance		=	max(iFieldElementalResistance,0);

	if	(isPlayer() || isPet() || isSummonBeast())
	{
		iResistance	=	min(iResistance-iFieldElementalResistance,dMAXIMUM_PLAYER_MAGIC_RESISTANCE_VALUE);
		if	(m_wTeam	==	dTEAM_PLAYER)
			iResistance	=	min(iResistance,m_lpField->m_iBossZoneResistenceLimit);
	}
	else
	if	(isPureMonster())
		iResistance	+=	m_lpField->m_sCorrectMonsterWindResistance;

	return	min(iResistance,dMAXIMUM_MONSTER_MAGIC_RESISTANCE_VALUE);
}

int				
cACTOR::getEarthResistance(int _iCorrectValue)
{
	int	iResistance	=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAGIC_RESISTANCE)/100 + m_allMagicResistance.getCorrectedValue()+m_earthResistance.getCorrectedValue()+getWisdom()*5/100;
	iResistance		+=	getEarthResistanceBonusByA();

	if	(m_sFloatTime)
		iResistance	+=	dEARTH_RESISTANCE_FOR_LEVITATE;

	if	(m_wParalysisTime)
		iResistance	=	0;

	iResistance		+=	_iCorrectValue;

	int	iFieldElementalResistance	=	m_lpField->m_sCorrectEarthResistance;
	iFieldElementalResistance		-=	m_wCorrectFieldElementalResistanceByTitle;
	iFieldElementalResistance		-=	m_bCorrectFieldEarthPenelty;

	CGuild	*lpGuild				=	getGuild();

	if	(lpGuild)
		iFieldElementalResistance	-=	(lpGuild->m_wDecreasePeneltyOfEarthResistance+lpGuild->m_wDecreaseDungeonMagicPenelty);

	iFieldElementalResistance		=	max(iFieldElementalResistance,0);

	if	(isPlayer() || isPet() || isSummonBeast())
	{
		iResistance	=	min(iResistance-iFieldElementalResistance,dMAXIMUM_PLAYER_MAGIC_RESISTANCE_VALUE);
		if	(m_wTeam	==	dTEAM_PLAYER)
			iResistance	=	min(iResistance,m_lpField->m_iBossZoneResistenceLimit);
	}
	else
	if	(isPureMonster())
		iResistance	+=	m_lpField->m_sCorrectMonsterEarthResistance;

	return	min(iResistance,dMAXIMUM_MONSTER_MAGIC_RESISTANCE_VALUE);
}

int				
cACTOR::getLightResistance(int _iCorrectValue)
{
	int	iResistance	=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAGIC_RESISTANCE)/100 + m_allMagicResistance.getCorrectedValue()+m_lightResistance.getCorrectedValue()+getWisdom()*5/100;
	iResistance		+=	getLightResistanceBonusByA();

	if	(m_wParalysisTime)
		iResistance	=	0;

	iResistance		+=	_iCorrectValue;

	int	iFieldElementalResistance	=	m_lpField->m_sCorrectLightResistance;
	iFieldElementalResistance		-=	m_wCorrectFieldElementalResistanceByTitle;
	iFieldElementalResistance		-=	m_bCorrectFieldLightPenelty;

	CGuild	*lpGuild				=	getGuild();

	if	(lpGuild)
		iFieldElementalResistance	-=	(lpGuild->m_wDecreasePeneltyOfLightResistance+lpGuild->m_wDecreaseDungeonMagicPenelty);

	iFieldElementalResistance		=	max(iFieldElementalResistance,0);

	if	(isPlayer() || isPet() || isSummonBeast())
	{
		iResistance	=	min(iResistance-iFieldElementalResistance,dMAXIMUM_PLAYER_MAGIC_RESISTANCE_VALUE);
		if	(m_wTeam	==	dTEAM_PLAYER)
			iResistance	=	min(iResistance,m_lpField->m_iBossZoneResistenceLimit);
	}
	else
	if	(isPureMonster())
		iResistance	+=	m_lpField->m_sCorrectMonsterLightResistance;

	return	min(iResistance,dMAXIMUM_MONSTER_MAGIC_RESISTANCE_VALUE);
}
int				
cACTOR::getDarkResistance(int _iCorrectValue)
{
	int	iResistance	=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAGIC_RESISTANCE)/100 + m_allMagicResistance.getCorrectedValue()+m_darkResistance.getCorrectedValue()+getWisdom()*5/100;
	iResistance		+=	getDarkResistanceBonusByA();

	if (m_wParalysisTime)
		iResistance	=	0;

	iResistance		+=	_iCorrectValue;

	int	iFieldElementalResistance	=	m_lpField->m_sCorrectDarkResistance;
	iFieldElementalResistance		-=	m_wCorrectFieldElementalResistanceByTitle;
	iFieldElementalResistance		-=	m_bCorrectFieldDarkPenelty;

	CGuild	*lpGuild				=	getGuild();

	if	(lpGuild)
		iFieldElementalResistance	-=	(lpGuild->m_wDecreasePeneltyOfDarkResistance+lpGuild->m_wDecreaseDungeonMagicPenelty);

	iFieldElementalResistance		=	max(iFieldElementalResistance,0);

	if	(isPlayer() || isPet() || isSummonBeast())
	{
		iResistance	=	min(iResistance-iFieldElementalResistance,dMAXIMUM_PLAYER_MAGIC_RESISTANCE_VALUE);
		if	(m_wTeam	==	dTEAM_PLAYER)
			iResistance	=	min(iResistance,m_lpField->m_iBossZoneResistenceLimit);
	}
	else
	if	(isPureMonster())
		iResistance	+=	m_lpField->m_sCorrectMonsterDarkResistance;

	return	min(iResistance,dMAXIMUM_MONSTER_MAGIC_RESISTANCE_VALUE);
}

int
cACTOR::getStrengtheningFireDamage(int _iCorrectByTitle,BOOL _bIsPureMagicDamageSkill)
{
	return	_iCorrectByTitle+m_strengtheningFireDamage.getCorrectedValue()+m_strengtheningMagicDamage.getCorrectedValue()*_bIsPureMagicDamageSkill;
}

int
cACTOR::getStrengtheningWaterDamage(int _iCorrectByTitle,BOOL _bIsPureMagicDamageSkill)
{
	return	_iCorrectByTitle+m_strengtheningWaterDamage.getCorrectedValue()+m_strengtheningMagicDamage.getCorrectedValue()*_bIsPureMagicDamageSkill;
}

int
cACTOR::getStrengtheningWindDamage(int _iCorrectByTitle,BOOL _bIsPureMagicDamageSkill)
{
	return	_iCorrectByTitle+m_strengtheningWindDamage.getCorrectedValue()+m_strengtheningMagicDamage.getCorrectedValue()*_bIsPureMagicDamageSkill;
}

int
cACTOR::getStrengtheningEarthDamage(int _iCorrectByTitle,BOOL _bIsPureMagicDamageSkill)
{
	return	_iCorrectByTitle+m_strengtheningEarthDamage.getCorrectedValue()+m_strengtheningMagicDamage.getCorrectedValue()*_bIsPureMagicDamageSkill;
}

int
cACTOR::getStrengtheningLightDamage(int _iCorrectByTitle,BOOL _bIsPureMagicDamageSkill)
{
	return	_iCorrectByTitle+m_strengtheningLightDamage.getCorrectedValue()+m_strengtheningMagicDamage.getCorrectedValue()*_bIsPureMagicDamageSkill;
}

int
cACTOR::getStrengtheningDarkDamage(int _iCorrectByTitle,BOOL _bIsPureMagicDamageSkill)
{
	return	_iCorrectByTitle+m_strengtheningDarkDamage.getCorrectedValue()+m_strengtheningMagicDamage.getCorrectedValue()*_bIsPureMagicDamageSkill;
}

int
cACTOR::getWeakningTargetFireResistance()
{
	return	m_weakningTargetFireResistance.getCorrectedValue()+m_weakningTargetMagicResistance.getCorrectedValue();
}

int
cACTOR::getWeakningTargetWaterResistance()
{
	return	m_weakningTargetWaterResistance.getCorrectedValue()+m_weakningTargetMagicResistance.getCorrectedValue();
}

int
cACTOR::getWeakningTargetWindResistance()
{
	return	m_weakningTargetWindResistance.getCorrectedValue()+m_weakningTargetMagicResistance.getCorrectedValue();
}

int
cACTOR::getWeakningTargetEarthResistance()
{
	return	m_weakningTargetEarthResistance.getCorrectedValue()+m_weakningTargetMagicResistance.getCorrectedValue();
}

int
cACTOR::getWeakningTargetLightResistance()
{
	return	m_weakningTargetLightResistance.getCorrectedValue()+m_weakningTargetMagicResistance.getCorrectedValue();
}

int
cACTOR::getWeakningTargetDarkResistance()
{
	return	m_weakningTargetDarkResistance.getCorrectedValue()+m_weakningTargetMagicResistance.getCorrectedValue();
}

int
cACTOR::getAllMagicResistance()
{
	int	iResistance	=	m_allMagicResistance.getCorrectedValue()+getWisdom()*5/100;

	return	iResistance;
}

//
//	점프 중인 시간..
void
cACTOR::setJumpFlyTime(int _iTime)
{
	m_wJumpFlyCounter	=	max(_iTime,m_wJumpFlyCounter);
}

//
//	점프 중인 시간..
void
cACTOR::resetJumpFlyTime()
{
	m_wJumpFlyCounter	=	0;
}

void
cACTOR::charm(cACTOR *_lpCaster,int _iTime,BOOL _bIBooking)
{
	if	(isPlayer())
		return;

	if	(m_wLordActor!=	0xffff)
		return;

	if	(m_wCharmedTime	)
	{
		if (_lpCaster->m_wTeam	==	m_wTeam)
			m_wCharmedTime	=	max(m_wCharmedTime,_iTime);

		return;
	}

	if	(_bIBooking)
	{
		m_lpField->bookingCharming(_lpCaster,this,FALSE,_iTime);

		return;
	}

	m_wCharmedTime			=	_iTime;
	m_wCharmingActor		=	_lpCaster->m_wSerialInField;
	m_dwCharmingActorHashCode=	_lpCaster->m_dwNameHashCode;

	if	(m_wCurseAmplificationTime)
		m_wCharmedTime	=	m_wCharmedTime*dCURSE_AMPLIFICATION_FACTOR/100;

	m_lpField->changeTeam(this,_lpCaster->m_wTeam);

	addSendVerySimpleInfo();

	stop();
	setTarget(NULL);
	setAiReady();
	addPacketStop();
}

void
cACTOR::releaseCharm(BOOL _bIBooking)
{
	if (_bIBooking)
	{
		m_lpField->bookingCharming(NULL,this,TRUE,0);

		return;
	}

	m_wCharmedTime	=	0;

	if (m_wTeam	==	m_wOldTeam)
		return;

	m_lpField->changeTeam(this,m_wOldTeam);

	setAiReady();
	resetTemporaryMagicEffect();

	addSendVerySimpleInfo();
}

void
cACTOR::tamed(cACTOR *_lpTamer)
{
	m_wTamer				=	_lpTamer->m_wSerialInField;
	m_dwTamerNameHashCode	=	_lpTamer->m_dwNameHashCode;
	m_wTamedMonsterCommand	=	eTMD_READY;

	cNPC	*lpNpc			=	getNpc();

	if (lpNpc)
	{
		lpNpc->reset();
		lpNpc->m_iLastTime	=	g_iCounterBySecond;
	}

	m_lpField->changeTeam(this,_lpTamer->m_wTeam);
	m_wOldTeam	=	m_wTeam;
	setAiType(eAT_TAMED_MONSTER);
	setAiReady();
}

BOOL
cACTOR::isTamed()
{
	if (m_wTamer	==	0xffff)
		return	FALSE;

	cACTOR	*lpTamer	=	m_lpField->getTestedActor(m_wTamer);

	if	(!lpTamer)
	{
		reduceHP(m_iHP);

		return	FALSE;
	}

	if (!lpTamer->isTamedMonster(m_wSerialInField))
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::firstAid(int _iFirstAidHP,int _iUpkeepTime)	//	응급처치
{
	m_iFirstAidHP	=	min(_iFirstAidHP,getMaxHP()-m_iHP);

	if	(m_iFirstAidHP	==	0)
		return	FALSE;

	int	iTime		=	30*100/(max(m_iFirstAidHP*100/getMaxHP(),1));
	iTime			=	min(iTime,dMAXIMUM_FIRST_AID_TIME);
	iTime			=	max(iTime,1);
	m_wFirstAidTime	=	iTime;

	if	(_iUpkeepTime)
		m_wFirstAidTime	=	_iUpkeepTime;
//	응급처치 지속시간 = 30 / (응급처치한 총 HP / MAX HP) 초	

	addSendFirstAidHP();

	return TRUE;
}

int
cACTOR::getMaxHP()	//	최대 체력
{
	if	(isCopyActor()	||	m_bIsGuildObject)
		return	m_maxHP.m_iValue;

	if	(isPureMonster() && m_level.isWrong())
		return	m_iMaxHPForDrainLevel;

	if	(m_maxHP.m_iValue	==	0)
		m_maxHP.m_iValue	=	m_iMaxHP;

	int	iAddHPByCH5	=	max(m_bf4CH5HPBonusCount-m_bf4CH5PeneltyHPBonusCount,0);
	iAddHPByCH5		=	iAddHPByCH5*c_iCH5BonusHPPerLevel;

	LONGLONG	llMaxHP;

	if	(m_wJob	<=	100)	//	소환수 플레이어
		llMaxHP	=	m_maxHP.m_iValue+getConstitution()*m_wCorrectMaxHPConstitutionFactor+getLevel()*m_wCorrectMaxHPFactor;
	else
		llMaxHP	=	m_maxHP.m_iValue+getConstitution()*m_wCorrectMaxHPConstitutionFactor*10+getLevel()*m_wCorrectMaxHPFactor*10;

	llMaxHP		=	llMaxHP*(m_maxHP.getCorrectPercentageValue()+iAddHPByCH5+100)/100;
	llMaxHP		+=	m_maxHP.getCorrectValue()*100;
	llMaxHP		+=	llMaxHP*m_iCorrectHPByGuildStategyPoint/100;
	llMaxHP		=	llMaxHP*m_wCorrectHP/100;
	llMaxHP		+=	llMaxHP*m_wCorrectValueByBardTitle/100;

	llMaxHP		+=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAX_HP);
	
	llMaxHP		+=	m_iCarvingOptionMaxHP;

	return	(int)max(llMaxHP,100);
}


int
cACTOR::getMaxCP()	//	최대 CP
{
	int	iAddCPByCH5	=	max(m_bf4CH5CPBonusCount-m_bf4CH5PeneltyCPBonusCount,0);
	iAddCPByCH5		=	iAddCPByCH5*c_iCH5BonusCPPerLevel;

	int	iMaxCP	=	m_maxCP.m_iValue+getCharisma()*100+getLevel()*100;
	iMaxCP		=	iMaxCP*(m_maxCP.getCorrectPercentageValue()+iAddCPByCH5+100)/100;
	iMaxCP		+=	m_maxCP.getCorrectValue()*100;

	iMaxCP		+=	iMaxCP*m_wCorrectValueByBardTitle/100;

	return	iMaxCP;
}

void
cACTOR::reduceCP(int _iCP,int _iSkill)	//	CP 빼기
{
	if(m_wMadnessTime)		// 광란상태.
		return;
	if	(m_wMaintainMaxCPTime)
	{
		m_iCP	=	getMaxCP();	
		return;
	}

	m_iCP	=	m_iCP-_iCP;
	iMiniPet()->operateReduceCPWork(_iSkill,_iCP);

	m_iCP	=	min(m_iCP,getMaxCP());
}

int
cACTOR::getRemainHPRate()
{
	LONGLONG	llHP	=	m_iHP;

	llHP	=	llHP*100/getMaxHP();

	return	(int)llHP;
}

//
//	인챈트 마법 추가
int
cACTOR::addEnchantedMagic(cAbility *_lpAbility,WORD _wTime)
{
	int	i;

	mDS.m_aiExValue[2]	=	_lpAbility->m_wSkill;

	CSkill*	lpSkill		=	_lpAbility->getSkill();

	for (i=0;i<dMAX_ENCHANTED_MAGIC_COUNT_FOR_CHARACTER;i++)
	{
		if	(m_aEnchantedMagic[i].m_wSkill	!=	_lpAbility->m_wSkill)
			continue;

		if(lpSkill && lpSkill->m_bf1IsCanNotApplySameEffect)
			return 0xffff;

		if (_wTime	==	0)
		{
			m_aEnchantedMagic[i].m_wSkill		=	0xffff;
			m_aEnchantedMagic[i].m_wLevel		=	0;
			m_aEnchantedMagic[i].m_wRemainTime	=	0;

			mDS.m_aiExValue[0]	=	5;
	 		updateEnchantedMagic();
		}
		else
		{
			m_aEnchantedMagic[i].m_wSkill		=	_lpAbility->m_wSkill;
			m_aEnchantedMagic[i].m_wLevel		=	_lpAbility->m_wLevel;
			m_aEnchantedMagic[i].m_wRemainTime	=	_wTime;

			mDS.m_aiExValue[0]	=	51;
			if	(m_aEnchantedMagic[i].m_wLevel	!=	_lpAbility->m_wLevel || m_aEnchantedMagic[i].m_wRemainTime	!=	_wTime)
				updateEnchantedMagic();
		}
		
		return	i;
	}
	
	for (i=0;i<dMAX_ENCHANTED_MAGIC_COUNT_FOR_CHARACTER;i++)
	{
		if (m_aEnchantedMagic[i].m_wSkill	!=	0xffff)
			continue;

		m_aEnchantedMagic[i].m_wSkill		=	_lpAbility->m_wSkill;
		m_aEnchantedMagic[i].m_wLevel		=	_lpAbility->m_wLevel;
		m_aEnchantedMagic[i].m_wRemainTime	=	_wTime;

		mDS.m_aiExValue[0]	=	6;
		updateEnchantedMagic();

		return	i;
	}

	return	0xffff;
}

//
//	인챈트 마법 추가
void
cACTOR::removeEnchantedMagic(int _iSkill)
{
	int	i;

	for (i=0;i<dMAX_ENCHANTED_MAGIC_COUNT_FOR_CHARACTER;i++)
	{
		if	(m_aEnchantedMagic[i].m_wSkill		==	_iSkill)
		{
			m_aEnchantedMagic[i].m_wSkill		=	0xffff;
			m_aEnchantedMagic[i].m_wRemainTime	=	0;

			break;
		}
	}
}

//
//
void
cACTOR::updateEnchantedMagic(BOOL _bIsIgnoreDeathStatus)
{
	mDS_P.m_iStep		=	2701;
	mDS.m_aiExValue[1]++;

	if	(_bIsIgnoreDeathStatus == FALSE && isDeath())
		return;

	cAbility	aAuraSkill[dACTIVE_REACTION_COUNT];
	int			i;

	LONGLONG		dwCurrentMagicEffectStatus	=	0;
	uEnchantedImage	currentEnchantedImage;

	mDS_P.m_iStep		=	2701;
	currentEnchantedImage.reset();

//	시머링 실드 체크
	if	(m_wReceiveBlocker		!=	0xffff)	
		dwCurrentMagicEffectStatus	|=	dAMEM_SHIMMERING_SHIELD;

	if	(m_wReceiveWeaponBlocker	!=	0xffff	||	m_wDodgeMagicDamageTime)
		dwCurrentMagicEffectStatus	|=	dAMEM_SPEAR_BLOCKER;

	if	(m_wCharmedTime)
		dwCurrentMagicEffectStatus	|=	dAMEM_CHARMING;

	mDS_P.m_iStep		=	2702;
	if	(isSitdown())
		dwCurrentMagicEffectStatus	|=	dAMEM_IS_SITDOWN;

	if	(m_wInvincibleTime)
		dwCurrentMagicEffectStatus	|=	dAMEM_INVINCIBLE;

	mDS_P.m_iStep		=	2703;

	for (i=0;i<dMAX_ENCHANTED_MAGIC_COUNT_FOR_CHARACTER;i++)
	{
		int			iEnchantedSkill	=	m_aEnchantedMagic[i].m_wSkill;

		if (iEnchantedSkill	==	0xffff )//||  iEnchantedSkill< dMAX_SKILL)
			continue;

		CSkill		*lpSkill		=	&g_aSkill[iEnchantedSkill];
		cAbility	ability;

		m_aEnchantedMagic[i].m_wRemainTime--;

		if (lpSkill->m_bf1BeastBerserker && m_wBeastBerserkerTime	==	0)
			m_aEnchantedMagic[i].m_wRemainTime	=	0;

		mDS_P.m_iStep		=	2704;

		if	((lpSkill->m_dwEnchantedEffectMask&dAMEM_MISTIC_FOG)	&&	m_dodgeChance.isGood()	==	FALSE)
			m_aEnchantedMagic[i].m_wRemainTime	=	0;


		if (m_aEnchantedMagic[i].m_wRemainTime	==	0)
		{
			m_aEnchantedMagic[i].m_wSkill	=	0xffff;
			continue;
		}

		mDS_P.m_iStep		=	2705;

		if (lpSkill->m_dwEnchantedEffectMask)
			dwCurrentMagicEffectStatus	|=	lpSkill->m_dwEnchantedEffectMask;
		else
			currentEnchantedImage.orEffect(&lpSkill->m_enchantedImage);

		ability.set(m_aEnchantedMagic[i].m_wSkill,m_aEnchantedMagic[i].m_wLevel);

		mDS_P.m_iStep		=	2706;
		if (!ability.isAttackMagicShield())
			continue;

		mDS_P.m_iStep		=	2707;
		operateAttackMagicShield(&ability);
		mDS_P.m_iStep		=	27070;
	}

	if	(m_sFloatTime	||	m_wFlyTime		)
		dwCurrentMagicEffectStatus	|=	dAMEM_LEVITATE;		//	떠 있다.
	if	(m_wStoneTime				)
		dwCurrentMagicEffectStatus	|=	dAMEM_STONED;		//	돌됐다.
	if	(m_wFreezeTime				)
		dwCurrentMagicEffectStatus	|=	dAMEM_FREEZE;		//	프리즈
	if	(m_wColdTime					)
		dwCurrentMagicEffectStatus	|=	dAMEM_COLD;			//	콜드
	if	(m_wStunTime					)
		dwCurrentMagicEffectStatus	|=	dAMEM_STUN;			//	스턴
	if	(m_wConfuseTime				)
		dwCurrentMagicEffectStatus	|=	dAMEM_CONFUSE;		//	혼란
	if	(m_wSleepTime				)
		dwCurrentMagicEffectStatus	|=	dAMEM_SLEEP;		//	잠
	if	(m_wBlindTime				)
		dwCurrentMagicEffectStatus	|=	dAMEM_BLIND;		//	장님

	mDS_P.m_iStep		=	2708;

	if	(m_wBlindByEvent)
	{
		mDS_P.m_iStep		=	2709;

		switch(m_wBlindByEvent)
		{
			case eBLIND_RANGE_NULL		:			//0x0000000200000000 ~~ 0x0000002000000000
				dwCurrentMagicEffectStatus &=	0xffffff41ffffffff;	
				break;
			case eBLIND_RANGE_VERY_SMALL:
				dwCurrentMagicEffectStatus |=	dAMEM_NEW_BLIND_VERY_SMALL;
				break;
			case eBLIND_RANGE_SMALL:
				dwCurrentMagicEffectStatus |=	dAMEM_NEW_BLIND_SMALL;
				break;
			case eBLIND_RANGE_MIDDLE:
				dwCurrentMagicEffectStatus |=	dAMEM_NEW_BLIND_MIDDLE;
				break;
			case eBLIND_RANGE_LARGE:
				dwCurrentMagicEffectStatus |=	dAMEM_NEW_BLIND_LARGE;
				break;
			case eBLIND_RANGE_VERY_LARGE:
				dwCurrentMagicEffectStatus |=	dAMEM_NEW_BLIND_VERY_LARGE;
				break;
		}
	}
	
	if (m_wBerserkTime				)
		dwCurrentMagicEffectStatus	|=	dAMEM_BERSERK;		//	버서크
	if (m_wLordActor	!=	0xffff	)
		dwCurrentMagicEffectStatus	|=	dAMEM_DISPLACEMENT;	//	빙의됨
	if (m_wBoostTime	||	isActiveSurvivalInstinctTitle())
		dwCurrentMagicEffectStatus	|=	dAMEM_BOOST;		//	부스트 상태
	if (m_ImmuneOneTime)
		dwCurrentMagicEffectStatus	|=	dAMEM_IGNORE_ATTACK_BY_GGG;		//	한대 면역 상태
	if	(m_sInvisivilityTime)
	{
		dwCurrentMagicEffectStatus	|=	dAMEM_INVISIBLE;	//	인비져블

		currentEnchantedImage.m_bf1IsDisableInvisibleByAttack	=	m_bf1IsDisableInvisibleByAttack;
	}
	if	(m_sBlurTime)
		dwCurrentMagicEffectStatus	|=	dAMEM_BLUR;			//	블러
	if	(m_wParalysisTime)
		dwCurrentMagicEffectStatus	|=	dAMEM_PARALYSIS;	//	전신마비
	if	(m_wNightMareTime)
		dwCurrentMagicEffectStatus	|=	dAMEM_NIGHTMARE;	//	악몽

	mDS_P.m_iStep		=	2710;

	if	(m_attackPower.isGood() || m_weaponPower.isGood()	||	m_correctLastDamage.isGood())
		currentEnchantedImage.m_bf1InclineAttackPower	=	TRUE;

	if	(m_wIncreaseFinalDamageTime	&&	m_lpField->m_bIsGuildBattleField	==	FALSE)
		currentEnchantedImage.m_bf1InclineAttackPower	=	TRUE;

	mDS_P.m_iStep		=	2711;

	if	(m_attackPower.isWrong() || m_weaponPower.isWrong())
		currentEnchantedImage.m_bf1DeclineAttackPower	=	TRUE;

	if	(m_wTransToWeaponTime)
		currentEnchantedImage.m_bf1IsTransToWeapon		=	TRUE;

	mDS_P.m_iStep		=	2712;

	if	(m_defensePower.isGood() || m_armorPower.isGood()	||	m_sCorrectFinalDefensivePowerPercentage	>	0)
		currentEnchantedImage.m_bf1InclineDefensePower	=	TRUE;

	if	(m_defensePower.isWrong() || m_armorPower.isWrong() || m_sCorrectFinalDefensivePowerPercentage	<	0)
		currentEnchantedImage.m_bf1DeclineDefensePower	=	TRUE;

	if	(m_hitChance.isGood())
		currentEnchantedImage.m_bf1InclineHitChance		=	TRUE;
	if	(m_hitChance.isWrong())
		currentEnchantedImage.m_bf1DeclineHitChance		=	TRUE;

	if	(m_wUntimateBarrierTime)
		currentEnchantedImage.m_bf1UltimateBarrier		=	TRUE;

	mDS_P.m_iStep		=	2713;

	if	(m_level.isWrong())
		currentEnchantedImage.m_bf1IsLevelDown			=	TRUE;

	if	(m_dodgeChance.isGood()	||	(m_wHillSkill	!=	0xffff	&&	m_wHillDodgeCorrect))
		currentEnchantedImage.m_bf1InclineDodgeChance	=	TRUE;
	if	(m_dodgeChance.isWrong())
		currentEnchantedImage.m_bf1DeclineDodgeChance	=	TRUE;

	mDS_P.m_iStep		=	2714;

	if	(m_actionSpeed.isGood())
		currentEnchantedImage.m_bf1InclineAttackSpeed	=	TRUE;
	if	(m_actionSpeed.isWrong())
		currentEnchantedImage.m_bf1DeclineAttackSpeed	=	TRUE;

	if	(m_moveSpeed.m_sCorrectValue	<=	-100)
		currentEnchantedImage.m_bf1IsBlockToMove		=	TRUE;

	mDS_P.m_iStep		=	2715;

	if	(m_moveSpeed.isGood()	||	m_wCorrectMoveSpeedByRidingDog)
		currentEnchantedImage.m_bf1InclineMoveSpeed		=	TRUE;

	if	(m_moveSpeed.isWrong())
		currentEnchantedImage.m_bf1DeclineMoveSpeed		=	TRUE;

	if	(m_wNoMoreFightTime)
		currentEnchantedImage.m_bf1IsNoMoreFight		=	TRUE;
	if	(m_wFogFormTime)
		currentEnchantedImage.m_bf1IsFogForm			=	TRUE;
	if	(m_wConversionIceTime)
		currentEnchantedImage.m_bf1IsConversionIce		=	TRUE;
	if	(m_wTauntedToTargetTime	)
		currentEnchantedImage.m_bf1IsTaunted			=	TRUE;
	if	(m_wFeignDeathTime		)
		currentEnchantedImage.m_bf1IsFeignDeath			=	TRUE;
	if	(m_wTransToFatGirlTime	)
		currentEnchantedImage.m_bf1IsPlump				=	TRUE;
	if	(m_wRabbitTime			)
		currentEnchantedImage.m_bf1IsRabbit				=	TRUE;
	if	(m_wTransToFrogTime)
		currentEnchantedImage.m_bf1IsFrog				=	TRUE;
	if	(m_wJumppingSkippingTime)
		currentEnchantedImage.m_bf1IsJumppingSkipping	=	TRUE;
	if	(m_wNotificationOfDeathCounter)
	{
		if	(m_wZombieBombPower	==	0)
			currentEnchantedImage.m_bf1IsNotificationOfDeath=	TRUE;
		else
			currentEnchantedImage.m_bf1IsZombieBomb			=	TRUE;
	}

	if	(m_wAnimatePartnerTime)
		currentEnchantedImage.m_bf1IsAnimatePartner		=	TRUE;
	if	(m_wReversalPowerTime)
		currentEnchantedImage.m_bf1IsReversalPower		=	TRUE;
	if	(m_wUseSpecialAbilityTime)
		currentEnchantedImage.m_bf1IsUseSpecialAttack	=	TRUE;

	mDS_P.m_iStep		=	2716;

	if	(isPlayer() && isIdlePlayer())
		currentEnchantedImage.m_bf1IsLaziness			=	TRUE;

	if	(m_wTheOtherSelfCount)
		currentEnchantedImage.m_bf1IsExistTheOtherSelf	=	TRUE;

	if	(m_iShakleHP)
		currentEnchantedImage.m_bf1IsBindByShakle		=	TRUE;

	if	(m_wBiter	!=	0xffff	)
		currentEnchantedImage.m_bf1IsExistBiter			=	TRUE;

	if	(m_wTortureTime			)
		currentEnchantedImage.m_bf1IsTorture			=	TRUE;

	mDS_P.m_iStep		=	2717;
	if	(m_strengtheningFireDamage.isGood()+m_strengtheningWaterDamage.isGood()+m_strengtheningWindDamage.isGood()+m_strengtheningEarthDamage.isGood()+m_strengtheningDarkDamage.isGood()+
		m_strengtheningLightDamage.isGood()+m_strengtheningMagicDamage.isGood())
		currentEnchantedImage.m_bf1StrengthenMagicDamage	=	TRUE;
	mDS_P.m_iStep		=	2718;
	if	(m_fireResistance.isWrong()+m_waterResistance.isWrong()+m_windResistance.isWrong()+m_earthResistance.isWrong()+m_lightResistance.isWrong()+m_darkResistance.isWrong()+m_allMagicResistance.isWrong())
		currentEnchantedImage.m_bf1WeaknessMagicRegistence	=	TRUE;

	if	(m_wReduceAllResistanceToHalfTime)
		currentEnchantedImage.m_bf1WeaknessResistance		=	TRUE;
	if	(m_wDanceTime	)
		currentEnchantedImage.m_bf1IsDance					=	TRUE;
	if	(m_wInMagicBoxTime	)
		currentEnchantedImage.m_bf1IsShutInMagicBox			=	TRUE;
	if	(m_wRemainImpulseTime)
		currentEnchantedImage.m_bf1IsImpulse				=	TRUE;
	if	(m_wBloodDonorTime || m_wBloodDonationTime)
		currentEnchantedImage.m_bf1IsBloodDrain				=	TRUE;
	if	(m_wHwabyungTime)
		currentEnchantedImage.m_bf1IsHwabyung				=	TRUE;
	if	(m_wPOS_Time	)
		currentEnchantedImage.m_bf1IsPlotOfShadow			=	TRUE;
	if	(m_wSOD_Time)
		currentEnchantedImage.m_bf1IsSmellOfDeath			=	TRUE;

	if	(m_wBreakedArmorTime)
		currentEnchantedImage.m_bf1IsBreakArmor				=	TRUE;
	if	(m_wBreakedWeaponTime)
		currentEnchantedImage.m_bf1IsBreakWeapon			=	TRUE;

	if	(m_wInterruptingArmorTime)
		currentEnchantedImage.m_bf1IsInterruptingArmor		=	TRUE;
	if	(m_wContractTime)
		currentEnchantedImage.m_bf1IsContract				=	TRUE;
	if	(m_wBloodCompactTime)
		currentEnchantedImage.m_bf1IsBloodCompact			=	TRUE;
	if	(m_wSoulOathTime)
		currentEnchantedImage.m_bf1IsSoulOath				=	TRUE;

	if	(m_wInterruptingWeaponTime)	// 무기해제
		currentEnchantedImage.m_bf1IsInterruptingWeapon		=	TRUE;
	if(m_wCorrectFinalAttackPowerPercentageTime)	// 최종공격력감소퍼센티지
		currentEnchantedImage.m_bf1IsInterruptingWeapon		=	TRUE;
	if	(m_wNotUseActivityReactionSkillTime)			// 발동형리액션사용금지 
		currentEnchantedImage.m_bf1IsNotUseActivityReactionSkill 	=	TRUE;
	if	(m_wConsensusTime)		// 교감상태.
		currentEnchantedImage.m_bf1IsConsensus				=	 TRUE;
	if	(m_wMadnessTime)			// 광란.
	{
		dwCurrentMagicEffectStatus	|=	dAMEM_BLACK;		//	광란 상태
		currentEnchantedImage.m_bf1IsMadness				=	TRUE;
	}
	mDS_P.m_iStep		=	2719;
	if	(m_strengtheningCriticalDamage.isGood())			// 치명타 대미지증가.
		currentEnchantedImage.m_bf1IsStrengtheningCriticalDamage				=	TRUE;
	if	(m_wSoulBurnTime)	// 소울번..
		currentEnchantedImage.m_bf1IsSoulBurn				=	TRUE;
	if	(m_wRageSoulTime)	// 타오르는영혼
		currentEnchantedImage.m_bf1IsRageSoul				=	TRUE;
	if	(m_wBindingWordsTime)	// 언어구속.
		currentEnchantedImage.m_bf1IsBindingWords				=	TRUE;
	if	(m_wFleeTime)	// 공포
		currentEnchantedImage.m_bf1IsFlee				=	TRUE;
	{	// 투기 효과..
		if(m_fightEnergy.isAddAbsoluteAttackPower())
		{
			currentEnchantedImage.m_bf1IsAddAbsoluteAttackPower				=	TRUE;
		}
		else if(m_fightEnergy.isIncreaseAttackCount())
		{
			currentEnchantedImage.m_bf1IsIncreaseAttackCount				=	TRUE;
		}
		else if(m_fightEnergy.isIncreaseGetCP())
		{
			currentEnchantedImage.m_bf1IsIncreaseGetCP						=	TRUE;
		}
		else if(m_fightEnergy.isIncreaseAbsoluteCriticalPercent())
		{
			currentEnchantedImage.m_bf1IsIncreaseAbsoluteCriticalPercent		=	TRUE;
		}
		else if(m_fightEnergy.isDecreaseCoolTime())
		{
			currentEnchantedImage.m_bf1IsDecreaseCoolTime					=	TRUE;
		}
	}

	if(m_wRushTime)		// 돌진시간.
		currentEnchantedImage.m_bf1IsRushTime					=	TRUE;
	if	(m_wFightingSpiritTime)		// 투지.
		currentEnchantedImage.m_bf1IsFightingSpirit				=	 TRUE;
	if	(m_wIgnoreNeedStateTime)		// 필요상태무시
		currentEnchantedImage.m_bf1IsFightingSpirit				=	 TRUE;

	if	(m_wIgnoreBlockingTime)		//	블럭킹무시
		currentEnchantedImage.m_bf1IsIgnoreBlocking				=	 TRUE;

	if	(m_wReturnMissileDamageTime)		//	미사일대미지리턴
		currentEnchantedImage.m_bf1IsReturnMissileDamage		=	 TRUE;

	if	(m_wIncreaseHardBlowTime)		//	강타확률증가
		currentEnchantedImage.m_bf1IsIncreaseHardBlow		=	 TRUE;

	if	(m_wMakeTypeUndeadTime)		//	언데드화상태
		currentEnchantedImage.m_bf1IsMakeTypeUndead		=	 TRUE;

	if	(m_wElectricShockTime)		// 감전 상태
		currentEnchantedImage.m_bf1IsElectricShock				=	TRUE;

	if(m_wTransformationTime)
	{	
		switch(m_wTransformationSkin)
		{
		case dJOB_MONSTER_DARK_ELF:
			currentEnchantedImage.m_bf1IsTransformationHunter = TRUE;
			break;
			
		case dJOB_MONSTER_ASSASSIN:
			currentEnchantedImage.m_bf1IsTransformationThief = TRUE;
			break;
		case dJOB_MONSTER_FALLEN_WIZARD:
			currentEnchantedImage.m_bf1IsTransformationMagician = TRUE;
			break;
		case	dJOB_MONSTER_REPTILE:
			currentEnchantedImage.m_bf1IsTransformationReptile = TRUE;
			break;
		}
	}

	if(m_wCarvingOptionSoulGuardTime)
		dwCurrentMagicEffectStatus = dwCurrentMagicEffectStatus | dAMEM_SANCTUARY;

	if	(m_wImmunePhysicalAttackTime)
		currentEnchantedImage.m_bf1IsImmunePhysicalDamage		=	TRUE;

	mDS_P.m_iStep		=	2720;
	if	(m_dwEnchantedMask	!=	dwCurrentMagicEffectStatus || !currentEnchantedImage.isSame(&m_enchantedImage) || m_bf5MagicCarpetShape	!=	m_wActiveMagicCarpetShape)
	{
		m_wActiveMagicCarpetShape	=	m_bf5MagicCarpetShape;
		m_dwEnchantedMask			=	dwCurrentMagicEffectStatus;
		m_enchantedImage.copy(&currentEnchantedImage);

		mDS_P.m_iStep		=	2721;
		addSendActorMagicEffectStatus();
	}

	if	(m_wProhibitActionByEnchantedMagic	!=	0xffff)
	{
		if	(m_aEnchantedMagic[m_wProhibitActionByEnchantedMagic].m_wRemainTime	==	0)
			m_wProhibitActionByEnchantedMagic	=	0xffff;
	}

	mDS_P.m_iStep		=	-2701;
}

void
cACTOR::correctInvisibleStatus(LONGLONG *_lpdwMask,cACTOR *_lpTarget)
{
	if	(isOwnForce(_lpTarget))
		return;

	if	(*_lpdwMask & dAMEM_INVISIBLE)
	{
		if	(getDetectingLevel() >= _lpTarget->getHidingLevel())
			*_lpdwMask -=	dAMEM_INVISIBLE;
	}
	if	(*_lpdwMask & dAMEM_BLUR)
	{
		if	(getDetectingLevel() >= _lpTarget->getHidingLevel())
			*_lpdwMask	-=	dAMEM_BLUR;
	}
}

void
cACTOR::correctInvisibleStatus(CSimplePlayerInfo *_lpInfo,cACTOR *_lpTarget)
{
	if (_lpInfo->bf2Kind	==	eAK_PLAYER)
		correctInvisibleStatus(&_lpInfo->dwEnchantedMask,_lpTarget);
	else
	if (_lpInfo->bf2Kind	==	eAK_MONSTER)
	{
		CSimpleMonsterInfo	*lpMonsterInfo	=	(CSimpleMonsterInfo	*)_lpInfo;
		correctInvisibleStatus(&lpMonsterInfo->dwEnchantedMask,_lpTarget);
	}
}

void
cACTOR::correctInvisibleStatus(CVerySimplePlayerInfo *_lpInfo,cACTOR *_lpTarget)
{
	if (_lpInfo->bf2Kind	==	eAK_PLAYER)
		correctInvisibleStatus(&_lpInfo->dwEnchantedMask,_lpTarget);
	else
	if (_lpInfo->bf2Kind	==	eAK_MONSTER)
	{
		CVerySimpleMonsterInfo	*lpMonsterInfo	=	(CVerySimpleMonsterInfo	*)_lpInfo;
		correctInvisibleStatus(&lpMonsterInfo->dwEnchantedMask,_lpTarget);
	}
}

BOOL
cACTOR::isCharmed()
{
	if (m_wCharmedTime)
		return	TRUE;

	return	FALSE;
}

void
cACTOR::getUpkeepStatus(cActorStatusUpkeep *_lpInfo)
{

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃특수 능력
	_lpInfo->m_wBeastBerserkerTime			=	m_wBeastBerserkerTime;
	_lpInfo->m_sInvisivilityTime			=	m_sInvisivilityTime;			//	인비저빌러티시간(-이면 장비에 붙어 있는거다.)
	_lpInfo->m_wInvincibleTime				=	m_wInvincibleTime;
	_lpInfo->m_sFloatTime					=	m_sFloatTime;					//	공중부양 시간(-이면 장비에 붙어 있는거다.)
	_lpInfo->m_sBlurTime					=	m_sBlurTime;					//	흐릿한 상태 시간(-이면 장비에 붙어 있는거다.)
	_lpInfo->m_sNonTargetTime				=	m_sNonTargetTime;				//	타겟에서 제외됨(-1이면 장비에 붙어 있는 것임)
	_lpInfo->m_wMaintainMaxCPTime			=	m_wMaintainMaxCPTime;			//	이 시간동안 CP가 max로 유지된다.
	_lpInfo->m_wReversalPowerTime			=	m_wReversalPowerTime;			//	지식 <-> 힘, 지혜 <-> 민첩성 변환 시간
	_lpInfo->m_wMaintainMaxWeponDamageTime	=	m_wMaintainMaxWeponDamageTime;	//	이 시간동안 무기의 데미지가 항상 max가 나온다.
	_lpInfo->m_iFirstAidHP					=	m_iFirstAidHP;					//	응급처치한 HP
	_lpInfo->m_wFirstAidTime				=	m_wFirstAidTime;				//	응급처치한 시간
	_lpInfo->m_wSealRunningTime				=	m_wSealRunningTime;
	_lpInfo->m_wSealActionTime				=	m_wSealActionTime;
	_lpInfo->m_wUnsealAction				=	m_wUnsealAction;
	_lpInfo->m_wJumppingSkippingTime		=	m_wJumppingSkippingTime;
	_lpInfo->m_wRabbitTime					=	m_wRabbitTime;

//┃특수 능력
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃능력치 저하
	_lpInfo->m_wReduceAllResistanceToHalfTime=	m_wReduceAllResistanceToHalfTime;
//┃능력치 저하
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃상태 이상
	_lpInfo->m_wBlindTime					=	m_wBlindTime;						//	장님 상태 남은 시간
	_lpInfo->m_wPoisonTime					=	m_wPoisonTime;						//	초당 데미지/10 , 중독 상태 남은 시간
	_lpInfo->m_iPoisonDamagePerSec			=	m_iPoisonDamagePerSec;
	_lpInfo->m_wColdTime					=	m_wColdTime;						//	콜드 상태 남은 시간
	_lpInfo->m_wConfuseTime					=	m_wConfuseTime;						//	혼란 상태 남은 시간
//┃상태 이상
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃저주
	_lpInfo->m_wFalloffMoraleTime			=	m_wFalloffMoraleTime;			//	사기 저하 상태 남은 시간
	_lpInfo->m_wTargetedTime				=	m_wTargetedTime;				//	타겟 설정 상태 남은 시간
	_lpInfo->m_wBreakedArmorTime			=	m_wBreakedArmorTime;			//	갑옷 파괴 상태 남은 시간
	_lpInfo->m_wBreakedWeaponTime			=	m_wBreakedWeaponTime;			//	무기 파괴 상태 남은 시간
	_lpInfo->m_wSimpletonTime				=	m_wSimpletonTime;				//	바보-_- 상태 남은 시간
	_lpInfo->m_wBerserkTime					=	m_wBerserkTime;					//	버서크 상태 남은 시간
//┃저주
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기타
	_lpInfo->m_wCurseAmplificationTime		=	m_wCurseAmplificationTime;		//	저주 증폭 상태

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃능력치
	m_attackPower.getUpkeepInfo(&_lpInfo->m_attackPower);					//	공격력
	m_defensePower.getUpkeepInfo(&_lpInfo->m_defensePower);					//	방어력
	m_regenHPPerSecond.getUpkeepInfo(&_lpInfo->m_regenHPPerSecond);			//	체력 회복
	m_maxHP.getUpkeepInfo(&_lpInfo->m_maxHP);						//	최대 HP
	m_maxCP.getUpkeepInfo(&_lpInfo->m_maxCP);						//	최대 CP
	m_weaponPower.getUpkeepInfo(&_lpInfo->m_weaponPower);
	m_armorPower.getUpkeepInfo(&_lpInfo->m_armorPower);

	m_level.getUpkeepInfo(&_lpInfo->m_level);					//	레벨
	m_strength.getUpkeepInfo(&_lpInfo->m_strength);						//	힘 
	m_agility.getUpkeepInfo(&_lpInfo->m_agility);						//	민첩성
	m_constitution.getUpkeepInfo(&_lpInfo->m_constitution);					//	건강
	m_intelligence.getUpkeepInfo(&_lpInfo->m_intelligence);					//	지식
	m_wisdom.getUpkeepInfo(&_lpInfo->m_wisdom);						//	지혜
	m_charisma.getUpkeepInfo(&_lpInfo->m_charisma);						//	카리스마
	m_luck.getUpkeepInfo(&_lpInfo->m_luck);							//	운
	m_actionSpeed.getUpkeepInfo(&_lpInfo->m_actionSpeed);					//	액션 속도 보정
	m_moveSpeed.getUpkeepInfo(&_lpInfo->m_moveSpeed);					//	이동 속도 보정
	m_hitChance.getUpkeepInfo(&_lpInfo->m_hitChance);					//	명중률 보정
	m_dodgeChance.getUpkeepInfo(&_lpInfo->m_dodgeChance);					//	회피 보정
	m_concentration.getUpkeepInfo(&_lpInfo->m_concentration);
	m_concentrationByStellarPeer.getUpkeepInfo(&_lpInfo->m_concentrationByStellarPeer);


//┃능력치
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	m_addFireDamage.getUpkeepInfo(&_lpInfo->m_addFireDamage);
	m_addWaterDamage.getUpkeepInfo(&_lpInfo->m_addWaterDamage);
	m_addWindDamage.getUpkeepInfo(&_lpInfo->m_addWindDamage);
	m_addLightDamage.getUpkeepInfo(&_lpInfo->m_addLightDamage);
	m_addDarkDamage.getUpkeepInfo(&_lpInfo->m_addDarkDamage);
	m_addEarthDamage.getUpkeepInfo(&_lpInfo->m_addEarthDamage);

//┃공격이 성공하면 부가적으로 들어가는 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 저항
	m_stunResistance.getUpkeepInfo(&_lpInfo->m_stunResistance);
	m_strangeStatusResistance.getUpkeepInfo(&_lpInfo->m_strangeStatusResistance);
	m_curseResistance.getUpkeepInfo(&_lpInfo->m_curseResistance);
	m_declinePowerResistance.getUpkeepInfo(&_lpInfo->m_declinePowerResistance);
	m_fireResistance.getUpkeepInfo(&_lpInfo->m_fireResistance);				//	불꽃 저항
	m_waterResistance.getUpkeepInfo(&_lpInfo->m_waterResistance);				//	물 저항
	m_windResistance.getUpkeepInfo(&_lpInfo->m_windResistance);				//	바람 저항
	m_earthResistance.getUpkeepInfo(&_lpInfo->m_earthResistance);				//	대지 저항
	m_lightResistance.getUpkeepInfo(&_lpInfo->m_lightResistance);				//	빛 저항
	m_darkResistance.getUpkeepInfo(&_lpInfo->m_darkResistance);				//	어둠 저항
	m_allMagicResistance.getUpkeepInfo(&_lpInfo->m_allMagicResistance);			//	모든 마법 저항

	int	iCount	=	0;

	memset(_lpInfo->m_aEnchantMagic,0xff,sizeof(_lpInfo->m_aEnchantMagic));

	for (int i=0;i<dMAX_ENCHANTED_MAGIC_COUNT_FOR_CHARACTER;i++)
	{
		if	(m_aEnchantedMagic[i].m_wSkill	==	0xffff)
			continue;

		memcpy(&_lpInfo->m_aEnchantMagic[iCount],&m_aEnchantedMagic[i],sizeof(_lpInfo->m_aEnchantMagic[iCount]));

		iCount++;

		if	(iCount	>=	dMAX_ENCHANTED_MAGIC_COUNT_FOR_UPKEEP)
			break;
	}
//┃보정된 마법 저항
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
}

void
cACTOR::setUpkeepStatus(cActorStatusUpkeep *_lpInfo)
{
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃특수 능력
	m_wBeastBerserkerTime			=	_lpInfo->m_wBeastBerserkerTime;
	m_sInvisivilityTime				=	_lpInfo->m_sInvisivilityTime;			//	인비저빌러티시간(-이면 장비에 붙어 있는거다.)
	m_wInvincibleTime				=	_lpInfo->m_wInvincibleTime;
	m_sFloatTime					=	_lpInfo->m_sFloatTime;					//	공중부양 시간(-이면 장비에 붙어 있는거다.)
	m_sBlurTime						=	_lpInfo->m_sBlurTime;					//	흐릿한 상태 시간(-이면 장비에 붙어 있는거다.)
	m_sNonTargetTime				=	_lpInfo->m_sNonTargetTime;				//	타겟에서 제외됨(-1이면 장비에 붙어 있는 것임)
	m_wMaintainMaxCPTime			=	_lpInfo->m_wMaintainMaxCPTime;			//	이 시간동안 CP가 max로 유지된다.
	m_wReversalPowerTime			=	_lpInfo->m_wReversalPowerTime;			//	지식 <-> 힘, 지혜 <-> 민첩성 변환 시간

	m_wMaintainMaxWeponDamageTime	=	_lpInfo->m_wMaintainMaxWeponDamageTime;	//	이 시간동안 무기의 데미지가 항상 max가 나온다.
	m_iFirstAidHP					=	_lpInfo->m_iFirstAidHP;					//	응급처치한 HP
	m_wFirstAidTime					=	_lpInfo->m_wFirstAidTime;				//	응급처치한 시간
	m_wSealRunningTime				=	_lpInfo->m_wSealRunningTime;
	m_wSealActionTime				=	_lpInfo->m_wSealActionTime;
	m_wUnsealAction					=	_lpInfo->m_wUnsealAction;
	m_wJumppingSkippingTime			=	_lpInfo->m_wJumppingSkippingTime;
	m_wRabbitTime					=	_lpInfo->m_wRabbitTime;

//┃특수 능력
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃능력치 저하
	m_wReduceAllResistanceToHalfTime=	_lpInfo->m_wReduceAllResistanceToHalfTime;
//┃능력치 저하
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃상태 이상
	m_wBlindTime					=	_lpInfo->m_wBlindTime;						//	장님 상태 남은 시간
	m_wPoisonTime					=	_lpInfo->m_wPoisonTime;						//	초당 데미지/10 , 중독 상태 남은 시간
	m_iPoisonDamagePerSec			=	_lpInfo->m_iPoisonDamagePerSec;
	m_wColdTime						=	_lpInfo->m_wColdTime;						//	콜드 상태 남은 시간
	m_wConfuseTime					=	_lpInfo->m_wConfuseTime;						//	혼란 상태 남은 시간
//┃상태 이상
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃저주
	m_wFalloffMoraleTime			=	_lpInfo->m_wFalloffMoraleTime;			//	사기 저하 상태 남은 시간
	m_wTargetedTime					=	_lpInfo->m_wTargetedTime;				//	타겟 설정 상태 남은 시간
	m_wBreakedArmorTime				=	_lpInfo->m_wBreakedArmorTime;			//	갑옷 파괴 상태 남은 시간
	m_wBreakedWeaponTime			=	_lpInfo->m_wBreakedWeaponTime;			//	무기 파괴 상태 남은 시간
	m_wSimpletonTime				=	_lpInfo->m_wSimpletonTime;				//	바보-_- 상태 남은 시간
	m_wBerserkTime					=	_lpInfo->m_wBerserkTime;					//	버서크 상태 남은 시간
//┃저주
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기타
	m_wCurseAmplificationTime		=	_lpInfo->m_wCurseAmplificationTime;		//	저주 증폭 상태

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃능력치
	m_attackPower.setUpkeepInfo(&_lpInfo->m_attackPower);					//	공격력
	m_defensePower.setUpkeepInfo(&_lpInfo->m_defensePower);					//	방어력
	m_regenHPPerSecond.setUpkeepInfo(&_lpInfo->m_regenHPPerSecond);			//	체력 회복
	m_maxHP.setUpkeepInfo(&_lpInfo->m_maxHP);						//	최대 HP
	m_maxCP.setUpkeepInfo(&_lpInfo->m_maxCP);						//	최대 CP
	m_weaponPower.setUpkeepInfo(&_lpInfo->m_weaponPower);
	m_armorPower.setUpkeepInfo(&_lpInfo->m_armorPower);

	m_level.setUpkeepInfo(&_lpInfo->m_level);					//	레벨
	m_strength.setUpkeepInfo(&_lpInfo->m_strength);						//	힘 
	m_agility.setUpkeepInfo(&_lpInfo->m_agility);						//	민첩성
	m_constitution.setUpkeepInfo(&_lpInfo->m_constitution);					//	건강
	m_intelligence.setUpkeepInfo(&_lpInfo->m_intelligence);					//	지식
	m_wisdom.setUpkeepInfo(&_lpInfo->m_wisdom);						//	지혜
	m_charisma.setUpkeepInfo(&_lpInfo->m_charisma);						//	카리스마
	m_luck.setUpkeepInfo(&_lpInfo->m_luck);							//	운
	m_actionSpeed.setUpkeepInfo(&_lpInfo->m_actionSpeed);					//	액션 속도 보정
	m_moveSpeed.setUpkeepInfo(&_lpInfo->m_moveSpeed);					//	이동 속도 보정
	m_hitChance.setUpkeepInfo(&_lpInfo->m_hitChance);					//	명중률 보정
	m_dodgeChance.setUpkeepInfo(&_lpInfo->m_dodgeChance);					//	회피 보정
	m_concentration.setUpkeepInfo(&_lpInfo->m_concentration);
	m_concentrationByStellarPeer.setUpkeepInfo(&_lpInfo->m_concentrationByStellarPeer);
//┃능력치
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	m_addFireDamage.setUpkeepInfo(&_lpInfo->m_addFireDamage);
	m_addWaterDamage.setUpkeepInfo(&_lpInfo->m_addWaterDamage);
	m_addWindDamage.setUpkeepInfo(&_lpInfo->m_addWindDamage);
	m_addLightDamage.setUpkeepInfo(&_lpInfo->m_addLightDamage);
	m_addDarkDamage.setUpkeepInfo(&_lpInfo->m_addDarkDamage);
	m_addEarthDamage.setUpkeepInfo(&_lpInfo->m_addEarthDamage);
//┃공격이 성공하면 부가적으로 들어가는 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 저항
	m_stunResistance.setUpkeepInfo(&_lpInfo->m_stunResistance);
	m_strangeStatusResistance.setUpkeepInfo(&_lpInfo->m_strangeStatusResistance);
	m_curseResistance.setUpkeepInfo(&_lpInfo->m_curseResistance);
	m_declinePowerResistance.setUpkeepInfo(&_lpInfo->m_declinePowerResistance);
	m_fireResistance.setUpkeepInfo(&_lpInfo->m_fireResistance);				//	불꽃 저항
	m_waterResistance.setUpkeepInfo(&_lpInfo->m_waterResistance);				//	물 저항
	m_windResistance.setUpkeepInfo(&_lpInfo->m_windResistance);				//	바람 저항
	m_earthResistance.setUpkeepInfo(&_lpInfo->m_earthResistance);				//	대지 저항
	m_lightResistance.setUpkeepInfo(&_lpInfo->m_lightResistance);				//	빛 저항
	m_darkResistance.setUpkeepInfo(&_lpInfo->m_darkResistance);				//	어둠 저항
	m_allMagicResistance.setUpkeepInfo(&_lpInfo->m_allMagicResistance);			//	모든 마법 저항
//┃보정된 마법 저항
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


	for (int i=0;i<dMAX_ENCHANTED_MAGIC_COUNT_FOR_UPKEEP;i++)
	{
		if	(_lpInfo->m_aEnchantMagic[i].m_wSkill	==	0xffff)
			break;

		memcpy(&m_aEnchantedMagic[i],&_lpInfo->m_aEnchantMagic[i],sizeof(_lpInfo->m_aEnchantMagic[i]));
	}

}


void
cACTOR_FACTOR_TYPE1::setUpkeepInfo(cACTOR_FACTOR_TYPE1_FOR_UPKEEP *_lpInfo)
{
	m_sCorrectValue					=	_lpInfo->m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	m_wTime							=	_lpInfo->m_wTime;		//	유지 시간
	m_bf1IsCanNotApplySameEffect	=	_lpInfo->m_bf1IsCanNotApplySameEffect;
}

void
cACTOR_FACTOR_TYPE1::getUpkeepInfo(cACTOR_FACTOR_TYPE1_FOR_UPKEEP *_lpInfo)
{
	_lpInfo->m_sCorrectValue				=	m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	_lpInfo->m_wTime						=	m_wTime;		//	유지 시간
	_lpInfo->m_bf1IsCanNotApplySameEffect	=	m_bf1IsCanNotApplySameEffect;
}

void
cACTOR_FACTOR_TYPE2::setUpkeepInfo(cACTOR_FACTOR_TYPE2_FOR_UPKEEP *_lpInfo)
{
	m_sCorrectValue	=	_lpInfo->m_sCorrectValue;					//	더하거나 빼야 하는 수치
	m_sCorrectPercentageValue	=	_lpInfo->m_sCorrectPercentageValue;			//	더하거나 빼야 하는 수치(퍼센티지)
	m_wTimeForCorrectValue	=	_lpInfo->m_wTimeForCorrectValue;				//	초
	m_wTimeForCorrectPercentageValue	=	_lpInfo->m_wTimeForCorrectPercentageValue;	//	초
	m_bf1IsCanNotApplySameEffect					=	_lpInfo->m_bf1IsCanNotApplySameEffect;
	m_bf1IsCanNotApplySameEffectPercent				=	_lpInfo->m_bf1IsCanNotApplySameEffectPercent;
}
void
cACTOR_FACTOR_TYPE2::getUpkeepInfo(cACTOR_FACTOR_TYPE2_FOR_UPKEEP *_lpInfo)
{
	_lpInfo->m_sCorrectValue	=	m_sCorrectValue;					//	더하거나 빼야 하는 수치
	_lpInfo->m_sCorrectPercentageValue	=	m_sCorrectPercentageValue;			//	더하거나 빼야 하는 수치(퍼센티지)
	_lpInfo->m_wTimeForCorrectValue	=	m_wTimeForCorrectValue;				//	초
	_lpInfo->m_wTimeForCorrectPercentageValue	=	m_wTimeForCorrectPercentageValue;	//	초
	_lpInfo->m_bf1IsCanNotApplySameEffect			=	m_bf1IsCanNotApplySameEffect;
	_lpInfo->m_bf1IsCanNotApplySameEffectPercent	=	m_bf1IsCanNotApplySameEffectPercent;

}

void
cACTOR_FACTOR_TYPE3::setUpkeepInfo(cACTOR_FACTOR_TYPE3_FOR_UPKEEP *_lpInfo)
{
	m_sCorrectValue	=	_lpInfo->m_sCorrectValue;					//	더하거나 빼야 하는 수치
	m_sCorrectPercentageValue	=	_lpInfo->m_sCorrectPercentageValue;			//	더하거나 빼야 하는 수치(퍼센티지)
	m_wTimeForCorrectValue	=	_lpInfo->m_wTimeForCorrectValue;				//	초
	m_wTimeForCorrectPercentageValue	=	_lpInfo->m_wTimeForCorrectPercentageValue;	//	초
	m_sCorrectPercentageValueForPetCommand	=	_lpInfo->m_sCorrectPercentageValueForPetCommand;
	m_wTimeForCorrectPercentageValueForPetCommand	=	_lpInfo->m_wTimeForCorrectPercentageValueForPetCommand;	//	초
	m_bf1IsCanNotApplySameEffect					=	_lpInfo->m_bf1IsCanNotApplySameEffect;
	m_bf1IsCanNotApplySameEffectPercent				=	_lpInfo->m_bf1IsCanNotApplySameEffectPercent;
	m_bf1IsCanNotApplySameEffectPetCommand			=	_lpInfo->m_bf1IsCanNotApplySameEffectPetCommand;
}

void
cACTOR_FACTOR_TYPE3::getUpkeepInfo(cACTOR_FACTOR_TYPE3_FOR_UPKEEP *_lpInfo)
{
	_lpInfo->m_sCorrectValue	=	m_sCorrectValue;					//	더하거나 빼야 하는 수치
	_lpInfo->m_sCorrectPercentageValue	=	m_sCorrectPercentageValue;			//	더하거나 빼야 하는 수치(퍼센티지)
	_lpInfo->m_wTimeForCorrectValue	=	m_wTimeForCorrectValue;				//	초
	_lpInfo->m_wTimeForCorrectPercentageValue	=	m_wTimeForCorrectPercentageValue;	//	초
	_lpInfo->m_sCorrectPercentageValueForPetCommand	=	m_sCorrectPercentageValueForPetCommand;
	_lpInfo->m_wTimeForCorrectPercentageValueForPetCommand	=	m_wTimeForCorrectPercentageValueForPetCommand;	//	초
	_lpInfo->m_bf1IsCanNotApplySameEffect			=	m_bf1IsCanNotApplySameEffect;
	_lpInfo->m_bf1IsCanNotApplySameEffectPercent	=	m_bf1IsCanNotApplySameEffectPercent;
	_lpInfo->m_bf1IsCanNotApplySameEffectPetCommand	=	m_bf1IsCanNotApplySameEffectPetCommand;
}

void
cACTOR_FACTOR_TYPE4::setUpkeepInfo(cACTOR_FACTOR_TYPE4_FOR_UPKEEP *_lpInfo)
{
	m_sCorrectValue	=	_lpInfo->m_sCorrectValue;					//	더하거나 빼야 하는 수치
	m_sCorrectPercentageValue	=	_lpInfo->m_sCorrectPercentageValue;			//	더하거나 빼야 하는 수치(퍼센티지)
	m_wTimeForCorrectValue	=	_lpInfo->m_wTimeForCorrectValue;				//	초
	m_wTimeForCorrectPercentageValue	=	_lpInfo->m_wTimeForCorrectPercentageValue;	//	초
	m_sCorrectPercentageValueForPetCommand	=	_lpInfo->m_sCorrectPercentageValueForPetCommand;
	m_wTimeForCorrectPercentageValueForPetCommand	=	_lpInfo->m_wTimeForCorrectPercentageValueForPetCommand;	//	초
	m_bf1IsCanNotApplySameEffect					=	_lpInfo->m_bf1IsCanNotApplySameEffect;
	m_bf1IsCanNotApplySameEffectPercent				=	_lpInfo->m_bf1IsCanNotApplySameEffectPercent;
	m_bf1IsCanNotApplySameEffectPetCommand			=	_lpInfo->m_bf1IsCanNotApplySameEffectPetCommand;
}

void
cACTOR_FACTOR_TYPE4::getUpkeepInfo(cACTOR_FACTOR_TYPE4_FOR_UPKEEP *_lpInfo)
{
	_lpInfo->m_sCorrectValue	=	m_sCorrectValue;					//	더하거나 빼야 하는 수치
	_lpInfo->m_sCorrectPercentageValue	=	m_sCorrectPercentageValue;			//	더하거나 빼야 하는 수치(퍼센티지)
	_lpInfo->m_wTimeForCorrectValue	=	m_wTimeForCorrectValue;				//	초
	_lpInfo->m_wTimeForCorrectPercentageValue	=	m_wTimeForCorrectPercentageValue;	//	초
	_lpInfo->m_sCorrectPercentageValueForPetCommand	=	m_sCorrectPercentageValueForPetCommand;
	_lpInfo->m_wTimeForCorrectPercentageValueForPetCommand	=	m_wTimeForCorrectPercentageValueForPetCommand;	//	초
	
	_lpInfo->m_bf1IsCanNotApplySameEffect			=	m_bf1IsCanNotApplySameEffect;
	_lpInfo->m_bf1IsCanNotApplySameEffectPercent	=	m_bf1IsCanNotApplySameEffectPercent;
	_lpInfo->m_bf1IsCanNotApplySameEffectPetCommand	=	m_bf1IsCanNotApplySameEffectPetCommand;
}

void
cACTOR_FACTOR_TYPE5::setUpkeepInfo(cACTOR_FACTOR_TYPE5_FOR_UPKEEP *_lpInfo)
{
	m_sCorrectValue					=	_lpInfo->m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	m_sRange						=	_lpInfo->m_sRange;
	m_wTime							=	_lpInfo->m_wTime;		//	유지 시간
	m_bf1IsCanNotApplySameEffect	=	_lpInfo->m_bf1IsCanNotApplySameEffect;
}

void
cACTOR_FACTOR_TYPE5::getUpkeepInfo(cACTOR_FACTOR_TYPE5_FOR_UPKEEP *_lpInfo)
{
	_lpInfo->m_sCorrectValue				=	m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	_lpInfo->m_sRange						=	m_sRange;
	_lpInfo->m_wTime						=	m_wTime;		//	유지 시간
	_lpInfo->m_bf1IsCanNotApplySameEffect	=	m_bf1IsCanNotApplySameEffect;
}

void
cACTOR_FACTOR_TYPE6::setUpkeepInfo(cACTOR_FACTOR_TYPE6_FOR_UPKEEP *_lpInfo)
{
	m_sCorrectValue					=	_lpInfo->m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	m_wTime							=	_lpInfo->m_wTime;		//	유지 시간
	m_bf1IsCanNotApplySameEffect	=	_lpInfo->m_bf1IsCanNotApplySameEffect;
}

void
cACTOR_FACTOR_TYPE6::getUpkeepInfo(cACTOR_FACTOR_TYPE6_FOR_UPKEEP *_lpInfo)
{
	_lpInfo->m_sCorrectValue				=	m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	_lpInfo->m_wTime						=	m_wTime;		//	유지 시간
	_lpInfo->m_bf1IsCanNotApplySameEffect	=	m_bf1IsCanNotApplySameEffect;
}




BOOL
cACTOR::setReversalPowerTime(int _iTime)
{
	if	(m_wReversalPowerTime	>=	_iTime)
		return	FALSE;

	m_wReversalPowerTime	=	_iTime;

	if	(isPlayer())
		sendEtcWork(eEW_SET_REVERSAL_POWER_TIME,_iTime);

	buildPower();

	return	TRUE;
}

BOOL
cACTOR::setCofuseTime(int _iTime,cACTOR *_lpCaster)
{
	if	(m_wConfuseTime	>	_iTime)
		return	FALSE;

	m_wConfuseTime					=	max(_iTime,1);

	if	(_lpCaster)
	{
		m_wConfuseCaster				=	_lpCaster->m_wSerialInField;
		m_dwConfuseCasterNameHashCode	=	_lpCaster->m_dwNameHashCode;
	}
	else
		m_wConfuseCaster				=	0xffff;

	return	TRUE;
}

void
cACTOR::resetStatusForDeath()
{
	if	(m_wLordOperator	!=	0xffff)
	{
		cACTOR	*lpLord		=	m_lpField->getActor(m_wLordOperator);

		if	(lpLord)
		{
			if	(isMonster())
			{
				SG_CONTROL_MONSTER	packet;

				packet.base.set(sizeof(SG_CONTROL_MONSTER),dSG_CONTROL_MONSTER);

				packet.wLord			=	0xffff;
				packet.wSlave			=	0xffff;

				g_userPM.add(lpLord->m_iClientSerial,&packet,packet.base.wSize);
			}

			lpLord->m_wLordOperator	=	0xffff;
		}
	}

	if	(m_wSlaveActor	!=	0xffff)
		m_lpField->bookingDisplacement(this,m_lpField->getActor(m_wSlaveActor),TRUE);

	if	(m_wLordActor	!=	0xffff)
		m_lpField->bookingDisplacement(m_lpField->getActor(m_wLordActor),this,TRUE);

	if	(m_wCharmedTime)
		releaseCharm(FALSE);

	if	(m_wReceiveBlocker		!=	0xffff	||	m_wReceiveWeaponBlocker	!=	0xffff	||	isShimmeringShieldCaster())
	{
		releaseCastShimmeringShield();
		releaseReceiveShimmeringShield();
	}

	if	(m_wTransToWeaponTime)
		releasePrincessWeaponStatus();

	if	(m_wRidingDogTime)
		getOffFromDog(FALSE);

	if	(isSummonBeast())
		returnBeastItem();

	if	(isDummy())
		m_lpField->releaseDummy(this);

	if	(m_wMarionetteTime)
		releaseMarionette();

	m_wSleepTime	=	0;
	m_wNightMareTime=	0;
	m_wParalysisTime=	0;
}

void
cACTOR::resetStatusForExitField()
{
	resetStatusForDeath();

	if	(!isPlayer())
		return;

	for (int i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		cACTOR	*lpBeast	=	getSummonBeastActor(i);

		if	(lpBeast == NULL || lpBeast->isDeath())
		{
			if	(lpBeast && lpBeast->m_wIsReleaseSummonBeast	==	FALSE)
				deathSummonBeast(i,m_iLevel);

			m_aSummonBeast[i].m_bf10Level		=	0;
			m_aSummonBeast[i].m_bf2Type			=	0;
			m_aSummonBeast[i].m_bf2Grade		=	0;
			m_aSummonBeast[i].m_bf22CurrentHP	=	0;
		}
		else
			m_aSummonBeast[i].m_bf10Level		=	lpBeast->m_iLevel;
	}

	if	(m_wPrincessWeaponPrincess	!=	0xffff)
		removePrincessWeapon();

	closePitchmanShop();
	closeBank();
	closeCart();
	closeTrade();
	checkChangeGold();

}

void
cACTOR::operateBloodDrainStatus(cACTOR *_lpTarget,int _iDamage)
{
	int	iDrainBlood	=	min(_iDamage*m_wBloodDonorRate/100,m_iHP);

	cACTOR	*lpCaster=	NULL;

	if	(m_wBloodDrainCaster	!=	0xffff)
	{
		lpCaster	=	m_lpField->getActor(m_wBloodDrainCaster);

		if	(!lpCaster	||	lpCaster->m_dwNameHashCode	!=	m_dwBloodDrainCasterNameHashCode)
		{
			lpCaster			=	NULL;
			m_wBloodDrainCaster	=	0xffff;
		}
	}

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iDrainBlood;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iDrainBlood	=	(DWORD)llDamage;
	}

	reduceHP(iDrainBlood,lpCaster);
	_lpTarget->increaseHP(iDrainBlood);

	LONGLONG	llHP		=	m_iHP;
	llHP					=	llHP*10000/getMaxHP();
	int			iRemainHP	=	(WORD)llHP;

	m_lpField->addSendEtcWork(m_wSerialInField,eEW_DRAIN_BLOOD,m_wSerialInField,_lpTarget->m_wSerialInField,iDrainBlood/100,m_wBloodDrainSkill,iRemainHP);
}

void
cACTOR::operateSelfReproofStatus()
{
	int	iDarkDamage	=	m_wSelfReproofDamage*100;
	iDarkDamage		-=	iDarkDamage*getDarkResistance()/100;

	cACTOR	*lpCaster=	NULL;

	if	(m_wSelfReproofCaster	!=	0xffff)
	{
		lpCaster	=	m_lpField->getActor(m_wSelfReproofCaster);

		if	(!lpCaster	||	lpCaster->m_dwNameHashCode	!=	m_dwSelfReproofCasterNameHashCode)
		{
			lpCaster			=	NULL;
			m_wSelfReproofCaster=	0xffff;
		}
	}

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iDarkDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iDarkDamage	=	(DWORD)llDamage;
	}

	reduceHP(iDarkDamage,lpCaster);
	addSendHitDamage(lpCaster,0,iDarkDamage,dATTACK_RESULT_FIELD_HIT,m_wSelfReproofSkill);
}


void
cACTOR::operateImpulseStatus()
{
	int	iDarkDamage	=	m_wImpulseDamage*100;
	iDarkDamage		-=	iDarkDamage*getDarkResistance()/100;

	cACTOR	*lpCaster=	NULL;

	if	(m_wImpulseCaster	!=	0xffff)
	{
		lpCaster	=	m_lpField->getActor(m_wImpulseCaster);

		if	(!lpCaster	||	lpCaster->m_dwNameHashCode	!=	m_dwImpulseCasterNameHashCode)
		{
			lpCaster			=	NULL;
			m_wImpulseCaster	=	0xffff;
		}
	}

	if	(isPlayer())
		iDarkDamage	=	GetP2PCorrectDamage(iDarkDamage,1);

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iDarkDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iDarkDamage	=	(DWORD)llDamage;
	}

	reduceHP(iDarkDamage,lpCaster);
	addSendHitDamage(lpCaster,0,iDarkDamage,dATTACK_RESULT_FIELD_HIT,m_wImpulseSkill);
}

//	고문
void
cACTOR::operateTortureStatus()
{
	LONGLONG	llTortureDamage	=	m_wTortureDarkDamage*100;
	llTortureDamage				-=	llTortureDamage*getDarkResistance()/100;

	int			iDamage			=	(int)llTortureDamage;

	cACTOR		*lpCaster		=	NULL;

	if	(m_wTortureCaster	!=	0xffff)
	{
		lpCaster	=	m_lpField->getActor(m_wTortureCaster);

		if	(!lpCaster	||	lpCaster->m_dwNameHashCode	!=	m_dwTortureCasterNameHashCode)
		{
			lpCaster			=	NULL;
			m_wTortureCaster	=	0xffff;
		}
	}

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iDamage	=	(DWORD)llDamage;
	}

	reduceHP(iDamage,lpCaster);

	m_lpField->addSendEtcWork(m_wSerialInField,eEW_TORTURE_DAMAGE,m_wSerialInField,iDamage/100);
}

//	악화
void
cACTOR::operateDegenerateStatus(int _iHealHp)
{
	if	(m_wDegenerateTime	==	0)
		return;

	int			iDamage		=	_iHealHp*m_wDegeneratePropertion/100;
	LONGLONG	llDamage	=	iDamage;

	llDamage				-=	llDamage*getCurseResistance()/100;
	iDamage					=	(int)llDamage;

	cACTOR		*lpCaster		=	NULL;

	if	(m_wDegenerateCaster	!=	0xffff)
	{
		lpCaster	=	m_lpField->getActor(m_wDegenerateCaster);

		if	(!lpCaster	||	lpCaster->m_dwNameHashCode	!=	m_dwDegenerateCasterNameHashCode)
		{
			lpCaster			=	NULL;
			m_wDegenerateCaster	=	0xffff;
		}
	}

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iDamage	=	(DWORD)llDamage;
	}

	reduceHP(iDamage,lpCaster);
	addSendHitDamage(lpCaster,0,iDamage,dATTACK_RESULT_FIELD_HIT,m_wDegenerateSkill);
}

//	가위
void
cACTOR::operateTerribleDream()
{
	int	iDarkDamage	=	m_wTerribleDreamDamage*100;
	iDarkDamage		-=	iDarkDamage*getDarkResistance()/100;

	cACTOR	*lpCaster=	NULL;

	if	(m_wTerribleDreamCaster	!=	0xffff)
	{
		lpCaster	=	m_lpField->getActor(m_wTerribleDreamCaster);

		if	(!lpCaster	||	lpCaster->m_dwNameHashCode	!=	m_dwTerribleDreamCasterNameHashCode)
		{
			lpCaster				=	NULL;
			m_wTerribleDreamCaster	=	0xffff;
		}
	}

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iDarkDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iDarkDamage	=	(DWORD)llDamage;
	}

	reduceHP(iDarkDamage,lpCaster);
	addSendHitDamage(lpCaster,0,iDarkDamage,dATTACK_RESULT_FIELD_HIT,m_wTerribleDreamSkill);	
}

//	가위
void
cACTOR::operateHwabyung()
{
	int	iDarkDamage	=	m_wHwabyungDarkDamage*100;
	iDarkDamage		-=	iDarkDamage*getDarkResistance()/100;

	int	iFireDamage	=	m_wHwabyungFireDamage*100;
	iFireDamage		-=	iFireDamage*getFireResistance()/100;

	int	iDamage		=	iDarkDamage+iFireDamage;

	cACTOR	*lpCaster=	NULL;

	if	(m_wHwabyungCaster	!=	0xffff)
	{
		lpCaster	=	m_lpField->getActor(m_wHwabyungCaster);

		if	(!lpCaster	||	lpCaster->m_dwNameHashCode	!=	m_dwHwabyungCasterNameHashCode)
		{
			lpCaster			=	NULL;
			m_wHwabyungCaster	=	0xffff;
		}
		
		if	(lpCaster)
			revenge(lpCaster);
	}

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iDamage		=	(DWORD)llDamage;
	}
	
	iDamage	=	min(dMAX_MAGICAL_DAMAGE,iDamage);

	m_wSleepTime			=	0;
	m_wTerribleDreamTime	=	0;

	reduceHP(iDamage,lpCaster);
	addSendHitDamage(lpCaster,0,iDamage,dATTACK_RESULT_FIELD_HIT,m_wHwabyungSkill);	
}

//	음모의 그림자
void
cACTOR::operatePlotOfShadow(int _iSpentTime)
{
	if	(_iSpentTime	<=	0)
		return;

	int	iDarkDamage	=	m_wPOS_Damage*100;
	iDarkDamage		-=	iDarkDamage*getDarkResistance()/100;

	cACTOR	*lpCaster=	NULL;

	if	(m_wPOS_Caster	!=	0xffff)
	{
		lpCaster	=	m_lpField->getActor(m_wPOS_Caster);

		if	(!lpCaster	||	lpCaster->m_dwNameHashCode	!=	m_dwPOS_CasterNameHashCode)
		{
			lpCaster		=	NULL;
			m_wPOS_Caster	=	0xffff;
		}
		else
			lpCaster->m_wPOS_Target	=	0xffff;
	}

	BOOL	bIsDamageSum	=	FALSE;

	iDarkDamage		=	iDarkDamage*_iSpentTime;

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iDarkDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iDarkDamage	=	(DWORD)llDamage;
	}

	reduceHP(iDarkDamage,lpCaster);
	m_lpField->addSendEtcWork(m_wSerialInField,eEW_MULTIPLE_DAMAGE,m_wSerialInField,0,iDarkDamage/100,_iSpentTime,m_wPOS_Skill,bIsDamageSum);
}

void
cACTOR::operatePlotOfShadowCaster(int _iDamage,int _iSkill)
{
	int	iDarkDamage	=	_iDamage*100;
	iDarkDamage		-=	iDarkDamage*getDarkResistance()/100;

	iDarkDamage		=	max(iDarkDamage,1);

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iDarkDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iDarkDamage	=	(DWORD)llDamage;
	}

	reduceHP(iDarkDamage,NULL);
	addSendHitDamage(this,0,iDarkDamage,dATTACK_RESULT_FIELD_HIT,_iSkill);	
}
void
cACTOR::operateSoulBurn(int _iDamage,int _iSkill)
{	// 소울번
	int	iFireDamage	=	_iDamage*100;

	iFireDamage		=	max(iFireDamage,1);

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iFireDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iFireDamage	=	(DWORD)llDamage;
	}
	cACTOR* lpSoulBurnActor = m_lpField->getActor(m_wSoulBurnCaster);

	if(lpSoulBurnActor && lpSoulBurnActor->m_dwNameHashCode == m_dwSoulBurnCasterNameHashCode)
	{
		reduceHP(iFireDamage,lpSoulBurnActor);
	}
	else
	{
		reduceHP(iFireDamage,NULL);
	}
	
	operateHitNpcReaction(lpSoulBurnActor, iFireDamage);	

	addSendHitDamage(this,0,iFireDamage,dATTACK_RESULT_FIELD_HIT,_iSkill);	
}


void
cACTOR::operateSmellOfDeath(cACTOR *_lpCaster)
{
	int	iDarkDamage	=	_lpCaster->m_wSOD_Damage*100;
	iDarkDamage		-=	iDarkDamage*getDarkResistance()/100;

	iDarkDamage		=	max(iDarkDamage,1);

	if	(m_lpField->isPVP(this,_lpCaster))
		iDarkDamage		=	GetP2PCorrectDamage(iDarkDamage		);

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iDarkDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iDarkDamage	=	(DWORD)llDamage;
	}

	reduceHP(iDarkDamage,_lpCaster);
	addSendHitDamage(this,0,iDarkDamage,dATTACK_RESULT_FIELD_HIT,m_wSOD_Skill);	

	m_declinePowerResistance.correctValue(-_lpCaster->m_wSOD_ReduceResistance,_lpCaster->m_wSOD_ReduceResistanceTime);
	m_strangeStatusResistance.correctValue(-_lpCaster->m_wSOD_ReduceResistance,_lpCaster->m_wSOD_ReduceResistanceTime);
	m_curseResistance.correctValue(-_lpCaster->m_wSOD_ReduceResistance,_lpCaster->m_wSOD_ReduceResistanceTime);
}

void
cACTOR::operateNotificationOfDeathCounter()
{
	if	(m_wZombieBombPower)
	{
		if	(m_wNotificationOfDeathCounter	==	0)
		{
			cACTOR	*lpCaster	=	m_lpField->getActor(m_wNotificationOfDeathCounterCaster);

			int	iDamage	=	m_wZombieBombPower*m_iHP/100;
			int	iTeam	=	1-m_wTeam;

			if	(lpCaster)
				iTeam	=	lpCaster->m_wTeam;

			m_wZombieBombPower	=	0;
			m_bf1IsInstanceKill	=	TRUE;

			reduceHP(m_iHP);

			cAbility	ability;

			ability.set(m_wZombieBombSkill,1);

			m_lpField->strikeWideAreaMagicDamage(this,-1,-1,FALSE,&ability,m_wNotificationOfDeathCounterRange,0,0,0,0,0,iDamage,iTeam);
			m_lpField->addSendOccurEffectBySkill(m_pos.x,m_pos.y,m_wZombieBombSkill,eSE_EXPLOSION);
		}
	}
	else
	{
		cACTOR	*lpCaster	=	m_lpField->getActor(m_wNotificationOfDeathCounterCaster);
		
		if	(!lpCaster	||	lpCaster->isDeath())
		{
			m_wNotificationOfDeathCounter		=	0;
			m_wNotificationOfDeathCounterCaster	=	0xffff;
		}
		else
		{
			if	(isTargetInAttackRange(lpCaster,m_wNotificationOfDeathCounterRange)	==	FALSE)
			{
				m_wNotificationOfDeathCounter		=	0;
				m_wNotificationOfDeathCounterCaster	=	0xffff;
				sendEtcWork(eEW_NOTIFICATION_OF_DEATH_COUNTER,0);
			}
			else
				if	(m_wNotificationOfDeathCounter	==	0)
				{
					int	iInstanceKillChance	=	m_wNotificationOfDeathChance-(m_bInstanceKillResistance+getCurseResistance());
					
					if	(m_wNotificationOfDeathChance	==	0)
						iInstanceKillChance	=	100;
					
					if	(random(100)	<	iInstanceKillChance)
					{
						int		iDamage	=	m_iHP;
						BOOL	bIsPVP	=	m_lpField->isPVP(this,lpCaster);
						BOOL	bIsNEP	=	isNEPlayer(lpCaster);
						
						m_bf1IsInstanceKill					=	TRUE;
						
						if	(bIsPVP)
						{
							if	(bIsNEP)	//	천적이다.
								iDamage		=	m_iHP*dPVP_INSTANCE_KILL_FACTOR_NE/100;
							else
								iDamage		=	m_iHP*dPVP_INSTANCE_KILL_FACTOR/100;
							
							m_bf1IsInstanceKill	=	FALSE;
						}
						
						damagedBy(lpCaster,NULL,iDamage,0);
					}
					
					m_wNotificationOfDeathCounterCaster	=	0xffff;
				}
		}
	}
}

void
cACTOR::operateContract()
{
	cACTOR	*lpCaster	=	m_lpField->getActor(m_wContractCaster);

	if	(!lpCaster	||	lpCaster->m_dwNameHashCode	!=	m_dwContractCasterNameHashCode)
	{
		m_wContractTime	=	0;

		return;
	}

	recoverHPByAidSkill(m_wContractSkill,m_wContractRecoverHP*100);

	cAbility	ability;

	ability.set(m_wContractDamageSkill,m_wContractDamageSkillLevel);

	WORD		awDamagedActorList[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];

	memset(awDamagedActorList,0xff,sizeof(awDamagedActorList));

	BOOL		bIsGotedCP	=	TRUE;

	awDamagedActorList[0]	=	m_wSerialInField;

	m_lpField->strikeWideAreaDamageSkill(lpCaster,&ability,&m_pos,ability.getHitRange(),awDamagedActorList,dMAX_TARGET_FOR_WIDE_AREA_ATTACK,&bIsGotedCP);

	if	(awDamagedActorList[1]	==	0xffff)
		return;

	m_lpField->addSendOccurEffectBySkill(m_pos.x,m_pos.y,ability.m_wSkill,eSE_EXPLOSION);
}

void
cACTOR::operateSoulOath(cACTOR *_lpKiller)
{
	if	(!_lpKiller	||	m_wSoulOathTime	==	0)
		return;

	m_wSoulOathTime	=	0;

	_lpKiller->increaseCP(m_wSoulOathCP*100);
	_lpKiller->syncPlayerBasicStatus(TRUE);

	cAbility	ability;

	ability.set(m_wSoulOathSkill,1);

	_lpKiller->addSendSkillEffect(&ability,eSE_SKILL);
}

void
cACTOR::operateBloodCompact(cACTOR *_lpKiller)
{
	if	(!_lpKiller	||	m_wBloodCompactTime	==	0)
		return;

	m_wBloodCompactTime	=	0;

	_lpKiller->healHP(m_wBloodCompactHP*100);

	cAbility	ability;

	ability.set(m_wSoulOathSkill,1);

	_lpKiller->addSendSkillEffect(&ability,eSE_SKILL);
}

BOOL
cACTOR::releaseContractTime()
{
	if	(m_wContractTime	==	0)
		return	FALSE;

	m_wContractTime	=	0;

	return	TRUE;
}