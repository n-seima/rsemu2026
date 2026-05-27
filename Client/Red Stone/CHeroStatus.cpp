#include "CHero.H"
#include "CActor.H"
#include "CMessage.H"
#include "cEvent.H"
#include "hero_mini_pet.H"

CPlayerDeathPeneltyInfo	g_aPlayerDeathPeneltyInfo[20]	=	
{
	{1		,10		,0		,0		,0		},
	{11		,20		,30		,10		,5		},
	{21		,40		,60		,10		,6		},
	{41		,80		,120	,15		,8		},
	{81		,120	,240	,15		,15		},
	{121	,160	,360	,20		,30		},
	{161	,240	,480	,20		,50		},
	{241	,320	,600	,25		,80		},
	{321	,400	,900	,30		,120	},
	{401	,560	,1200	,35		,170	},
	{561	,720	,1800	,40		,250	},
	{721	,880	,2400	,45		,500	},
	{881	,1000	,3600	,50		,1000	},
	{0xffff	,1000	,3600	,50		,1000	}
};


void
CHero::operateOncePerSecondWork()
{
	int	i;

	decreaseAidMagicTime();

	if	(m_wBardTitleValueTime)
	{
		--m_wBardTitleValueTime;

		if	(m_wBardTitleValueTime	==	0)
			m_wBardTitleValue	=	0;
	}

	for(i = 0 ; i < dMAX_PROCESS_QUEST_COUNT ; i++)
		if(g_hero.m_awQuestCountDownTimer[i] != 0xffff)
			g_hero.m_awQuestCountDownTimer[i]--;

	if	(m_wRestraintTimeByAttackAction	)
		m_wRestraintTimeByAttackAction--;

	if	(m_wRestraintTimeByBattle		)
		m_wRestraintTimeByBattle--;

	if	(m_wRemainSecondForUseGateGlove)
		m_wRemainSecondForUseGateGlove--;

	if	(m_wRestraintTimeByLogout	&&	m_wRestraintTimeByLogout	!=	0xffff)
		m_wRestraintTimeByLogout--;

	if	(g_iBoostExpSecondByTreasureMapEventInServer	==	0)
		if	(m_wBoostExpSecondByTreasureMapEvent)
			m_wBoostExpSecondByTreasureMapEvent--;

	if	(g_iBoostGoldSecondByTreasureMapEventInServer	==	0)
		if	(m_wBoostGoldSecondByTreasureMapEvent)
			m_wBoostGoldSecondByTreasureMapEvent--;

	if	(m_GGG.m_wGGGTimeForSkillUp)
		m_GGG.m_wGGGTimeForSkillUp--;

	for	(i=c_iCoolTimeSkillCount;i;)
	{
		--i;

		if	(m_awSkillCoolTime[i])
			--m_awSkillCoolTime[i];	
	}

	for	(i=dITEM_COOL_TIME_BUFFER_SIZE;i;)
	{
		--i;
		
		if	(m_awItemCoolTime[i])
			--m_awItemCoolTime[i];	
	}

	if	(g_am.m_iObserberWaitTime && g_am.m_iObserberTime == 0)
		g_am.m_iObserberWaitTime--;

	if	(g_am.m_iObserberTime)
		g_am.m_iObserberTime--;

	++s_dwLoginTime;
	
	if(s_dwRank12AgeTime)
		--s_dwRank12AgeTime;
	
	if(s_dwLoginTime%3600 == 0)	// 1시간 마다 ..!
		s_dwRank12AgeTime = 3;

	updateKarmaDrawImage();
}

//
//	부가 마법 타임 줄어듬(1초마다..)

void
CHero::resetTemporaryMagicEffect()
{
	m_sFloatTime					=	0;
	m_sInvisivilityTime				=	0;
	m_sNonTargetTime				=	0;
	m_wMaintainMaxCPTime			=	0;
	m_wMaintainMaxWeponDamageTime	=	0;

	m_wBlindTime					=	0;
	m_wPoisonTime					=	0;
	m_wSleepTime					=	0;
	m_wColdTime						=	0;
	m_wStoneTime					=	0;
	m_wConfuseTime					=	0;
	m_wCharmedTime					=	0;

	m_wFalloffMoraleTime			=	0;
	m_wTargetedTime					=	0;
	m_wBreakedArmorTime				=	0;
	m_wBreakedWeaponTime			=	0;
	m_wSimpletonTime				=	0;
	m_wBerserkTime					=	0;
	m_wTortureTime					=	0;
	m_wBloodDonorTime				=	0;
	m_wCurseAmplificationTime		=	0;
	m_wNotificationOfDeathCounter	=	0;

	m_wJewelTime					=	0;
	m_wFirstAidTime					=	0;
	m_wCorrectFinalDefensivePowerPercentageTime = 0;
	m_sCorrectFinalDefensivePowerPercentage = 0;

	m_wConsensusTime	=0;		// 교감상태.
	m_wMadnessTime	=0;			// 광란.
	// 언어구속
	m_wBindingWordsTime	=0;
	
	//도망
	m_wFleeTime	=0; 
	//  리액션스킬사용금지..
	m_wNotUseReactionSkillTime	=0;	

	m_attackPower.resetAidData();					//	공격력
	m_defensePower.resetAidData();					//	방어력

	m_maxHP.resetAidData();							//	최대 HP
	m_maxCP.resetAidData();							//	최대 CP
	m_regenHPPerSecond.resetAidData();
	m_strangeStatusResistance.resetAidData();
	m_curseResistance.resetAidData();
	m_declinePowerResistance.resetAidData();

	m_detectSecretObject.resetAidData();
	m_detectTrap.resetAidData();

	m_level.resetAidData()			;			
	m_strength.resetAidData()		;			//	힘 
	m_agility.resetAidData()		;			//	민첩성
	m_constitution.resetAidData()	;			//	건강
	m_intelligence.resetAidData()	;			//	지식
	m_wisdom.resetAidData()			;			//	지혜
	m_charisma.resetAidData()		;			//	카리스마

	m_luck.resetAidData();							//	운

	m_actionSpeed.resetAidData();					//	액션 속도 보정
	m_moveSpeed.resetAidData();						//	이동 속도 보정

	m_hitChance.resetAidData();						//	명중률 보정
	m_dodgeChance.resetAidData();					//	회피 보정
	m_concentration.resetAidData();
	m_concentrationByStellarPeer.resetAidData();

	m_fireResistance.resetAidData();				//	불꽃 저항
	m_waterResistance.resetAidData();				//	물 저항
	m_windResistance.resetAidData();				//	바람 저항
	m_earthResistance.resetAidData();				//	대지 저항
	m_lightResistance.resetAidData();				//	빛 저항
	m_darkResistance.resetAidData();				//	어둠 저항
	m_allMagicResistance.resetAidData();			//	모든 마법 저항
	m_correctAllSkillLevel.resetAidData();
	m_correctRightPunchSkillLevel.resetAidData();
	m_correctLeftPunchSkillLevel.resetAidData();
	m_correctRightKickSkillLevel.resetAidData();
	m_correctLeftKickSkillLevel.resetAidData();
	m_correctDownKickSkillLevel.resetAidData();

	m_wFightingSpiritTime			=	0;		// 투지
	m_wIgnoreNeedStateTime			=	0;				// 필요상태무시

	m_wTransformationTime				=	0;
	m_wTransformationLevel				=	0;
	m_wTransformationSkin				=	0xffff;
	m_wTransformationSkill				=	0xffff;

}

void
CHero::decreaseAidMagicTime()
{
	if	(m_sFloatTime					)	m_sFloatTime--;					//	공중부양 시간(-이면 장비에 붙어 있는거다.)
	if	(m_sInvisivilityTime			)	m_sInvisivilityTime--;			//	인비저빌러티시간(-이면 장비에 붙어 있는거다.)
	if	(m_sNonTargetTime				)	m_sNonTargetTime--;				//	타겟에서 제외됨(-1이면 장비에 붙어 있는 것임)
	if	(m_wMaintainMaxCPTime			)	m_wMaintainMaxCPTime--;			//	이 시간동안 CP가 max로 유지된다.
	if	(m_wMaintainMaxWeponDamageTime	)	m_wMaintainMaxWeponDamageTime--;//	이 시간동안 무기의 대미지가 항상 max가 나온다.

	if	(m_wBlindTime					)	m_wBlindTime--;					//	장님 상태 남은 시간
	if	(m_wPoisonTime					)	m_wPoisonTime--;				//	초당 대미지/10 , 중독 상태 남은 시간
	if	(m_wSleepTime					)	m_wSleepTime--;					//	잠이 깰때까지 남은 시간
	if	(m_wColdTime					)	m_wColdTime--;					//	콜드 상태 남은 시간
//	if	(m_wFreezeTime					)	m_wFreezeTime--;				//	프리즈 상태 남은 시간
//	if	(m_wStunTime					)	m_wStunTime--;					//	스턴 상태 남은 시간
	if	(m_wStoneTime					)	m_wStoneTime--;					//	석화 남은 시간
	if	(m_wConfuseTime					)	m_wConfuseTime--;				//	혼란 상태 남은 시간
	if	(m_wCharmedTime					)	m_wCharmedTime--;				//	차밍 상태 남은 시간

	if	(m_wFalloffMoraleTime			)	m_wFalloffMoraleTime--;			//	사기 저하 상태 남은 시간
	if	(m_wTargetedTime				)	m_wTargetedTime--;				//	타겟 설정 상태 남은 시간
	if	(m_wBreakedArmorTime			)	m_wBreakedArmorTime--;			//	갑옷 파괴 상태 남은 시간
	if	(m_wBreakedWeaponTime			)	m_wBreakedWeaponTime--;			//	무기 파괴 상태 남은 시간
	if	(m_wSimpletonTime				)	m_wSimpletonTime--;				//	바보-_- 상태 남은 시간
	if	(m_wBerserkTime					)	m_wBerserkTime--;				//	버서크 상태 남은 시간
	if	(m_wTortureTime					)	m_wTortureTime--;				//	고문률-_-??,고문 시간
	if	(m_wBloodDonorTime				)	m_wBloodDonorTime--;			//	피흡혈률,피흡혈 당하는 시간
	if	(m_wCurseAmplificationTime		)	m_wCurseAmplificationTime--;	//	저주 증폭 상태
	if	(m_wNotificationOfDeathCounter	)	m_wNotificationOfDeathCounter--;

	//AidMagic은 아니지만 여기서 처리한다.	화면 효과 시간
	if	(m_wBlackScreenCount			)	m_wBlackScreenCount--;
	if	(m_wTVScreenCount				)	m_wTVScreenCount--;

	
	if(m_wCorrectFinalDefensivePowerPercentageTime)
	{
		--m_wCorrectFinalDefensivePowerPercentageTime;
		if(m_wCorrectFinalDefensivePowerPercentageTime == 0)
			m_sCorrectFinalDefensivePowerPercentage = 0;;
	}
	if	(m_wDeathPeneltySecond && !g_lpHero->isDeath()	)
	{
		m_wDeathPeneltySecond--;

		if	(m_wDeathPeneltySecond	==	0)
		{
			CGamePlay::AddSystemMessage(LTGREEN,dMSG_RECOVER_ALL_DETH_PENELTY);
			buildPower();
		}
	}

	if	(m_wJewelTime					)
	{	
		m_wJewelTime--;					//	보석 유지 시간

		if	(m_wJewelTime	==	0)
			buildPower();
	}

	if	(m_wFirstAidTime)	//	응급처치 업데이트
	{
		m_wFirstAidTime--;

		if	(m_wFirstAidTime	==	0)
			m_iFirstAidHP	=	0;
	}

	if	(m_wInterruptingArmorTime)
	{
		--m_wInterruptingArmorTime;

		if	(m_wInterruptingArmorTime	==	0)
			buildPower();
	}

	if	(m_wInterruptingWeaponTime)
	{	// 무기해제
		--m_wInterruptingWeaponTime;

		if	(m_wInterruptingWeaponTime	==	0)
			buildPower();
	}
	if(m_wConsensusTime)		// 교감상태.
		--m_wConsensusTime;
	if(m_wMadnessTime)		// 광란 상태.
		--m_wMadnessTime;
	if(m_wBindingWordsTime)	// 언어구속상태.
		--m_wBindingWordsTime;
	if(m_wFleeTime)		// 도망.
		--m_wFleeTime;
	if	(m_wReversalPowerTime)
		--m_wReversalPowerTime;
	if	(m_wDeclientRecoverHPEfficientTime)
		--m_wDeclientRecoverHPEfficientTime;
	if	(m_wHwabyungTime)
		--m_wHwabyungTime;
	if	(m_wRemainImpulseTime)
		--m_wRemainImpulseTime;
	if	(m_wAnimatePartnerTime)
		--m_wAnimatePartnerTime;
	if(m_wNotUseReactionSkillTime)
		--m_wNotUseReactionSkillTime;
	m_attackPower.decreaseTime();					//	공격력
	m_defensePower.decreaseTime();					//	방어력

	m_maxHP.decreaseTime();							//	최대 HP
	m_maxCP.decreaseTime();							//	최대 CP
	m_regenHPPerSecond.decreaseTime();
	m_strangeStatusResistance.decreaseTime();
	m_curseResistance.decreaseTime();
	m_declinePowerResistance.decreaseTime();

	m_detectSecretObject.decreaseTime();
	m_detectTrap.decreaseTime();

	if	(
		m_level.decreaseTime()			||			
		m_strength.decreaseTime()		||			//	힘 
		m_agility.decreaseTime()		||			//	민첩성
		m_constitution.decreaseTime()	||			//	건강
		m_intelligence.decreaseTime()	||			//	지식
		m_wisdom.decreaseTime()			||			//	지혜
		m_charisma.decreaseTime())					//	카리스마
		buildPower();

	m_luck.decreaseTime();							//	운

	m_actionSpeed.decreaseTime();					//	액션 속도 보정
	m_moveSpeed.decreaseTime();						//	이동 속도 보정
	m_bonusItemDropChance.decreaseTime();
	m_bonusExp.decreaseTime();

	m_hitChance.decreaseTime();						//	명중률 보정
	m_dodgeChance.decreaseTime();					//	회피 보정
	m_concentration.decreaseTime();
	m_concentrationByStellarPeer.decreaseTime();

	m_fireResistance.decreaseTime();				//	불꽃 저항
	m_waterResistance.decreaseTime();				//	물 저항
	m_windResistance.decreaseTime();				//	바람 저항
	m_earthResistance.decreaseTime();				//	대지 저항
	m_lightResistance.decreaseTime();				//	빛 저항
	m_darkResistance.decreaseTime();				//	어둠 저항
	m_allMagicResistance.decreaseTime();			//	모든 마법 저항
	m_correctAllSkillLevel.decreaseTime();
	m_correctRightPunchSkillLevel.decreaseTime();
	m_correctLeftPunchSkillLevel.decreaseTime();
	m_correctRightKickSkillLevel.decreaseTime();
	m_correctLeftKickSkillLevel.decreaseTime();
	m_correctDownKickSkillLevel.decreaseTime();

	m_strengtheningFireDamage.decreaseTime();
	m_strengtheningWaterDamage.decreaseTime();
	m_strengtheningWindDamage.decreaseTime();
	m_strengtheningEarthDamage.decreaseTime();
	m_strengtheningDarkDamage.decreaseTime();
	m_strengtheningLightDamage.decreaseTime();
	m_strengtheningMagicDamage.decreaseTime();
	m_fightEnergy.decreaseTime();

	if(m_wRushTime)
		--m_wRushTime;
	if(m_wFightingSpiritTime)
		--m_wFightingSpiritTime;
	if(m_wIgnoreNeedStateTime)
		--m_wIgnoreNeedStateTime;

	if(m_wInstanceHealEffectTime)
	{
		--m_wInstanceHealEffectTime;
		if(!m_wInstanceHealEffectTime)
			m_wInstanceHealEffectPercent = 0;
	}
	if(m_wTransformationTime)
	{
		--m_wTransformationTime;
		if(m_wTransformationTime == 0)
		{	
			m_wTransformationLevel	=	0;
			m_wTransformationSkin	=	0xffff;
			m_wTransformationSkill	=	0xffff;
		}
	}


}

//
//	스테이터스 업데이트
void
CHero::updateStatus()
{
	updateTwinkleItemFrame();

	if (m_wWaitMoveAreaResultTime)
		m_wWaitMoveAreaResultTime--;

	if	(s_isLeftButtonPressed	==	0)	//	마우스 버튼을 뗐다.
		g_hero.m_bf1IsWaitReleaseLeftButtonForAreaWork	=	TRUE;

	if	(!g_lpHero)
		return;

	m_rectSight.set(g_lpHero->m_pos.x-dFORECAST_VISIBLE_ACTOR_WIDTH*100/g_iGameScale,g_lpHero->m_pos.y-dFORECAST_VISIBLE_ACTOR_HEIGHT*100/g_iGameScale,g_lpHero->m_pos.x+dFORECAST_VISIBLE_ACTOR_WIDTH*100/g_iGameScale,g_lpHero->m_pos.y+dFORECAST_VISIBLE_ACTOR_HEIGHT*100/g_iGameScale);

	if	(g_hero.m_rectSight.x1	<	0					)
	{
		g_hero.m_rectSight.x2	-=	g_hero.m_rectSight.x1;
		g_hero.m_rectSight.x1	=	0;
	}
	if	(g_hero.m_rectSight.y1	<	0					)
	{
		g_hero.m_rectSight.y2	-=	g_hero.m_rectSight.y1;
		g_hero.m_rectSight.y1	=	0;
	}
	if	(g_hero.m_rectSight.x2	>	g_map.m_iPixelWidth	)
	{
		g_hero.m_rectSight.x1	-=	(g_hero.m_rectSight.x2-g_map.m_iPixelWidth);
		g_hero.m_rectSight.x2	=	g_map.m_iPixelWidth;
	}
	if	(g_hero.m_rectSight.y2	>	g_map.m_iPixelHeight)
	{
		g_hero.m_rectSight.y1	-=	(g_hero.m_rectSight.y2-g_map.m_iPixelHeight);
		g_hero.m_rectSight.y2	=	g_map.m_iPixelHeight;
	}

	g_lpHero->m_wLevel	=	getLevel();
	g_lpHero->m_iMaxHP	=	g_hero.getMaxHP();

	CPartyMemberInfo	*lpMember	=	CGamePlay::s_partyInfo.getMemberByActorSerial(g_lpHero->m_wSerial);

	if	(lpMember)
		lpMember->m_wHP	=	g_hero.m_iHP*100/g_hero.getMaxHP();

	if	(m_wEngageTarget	!=	0xffff)
		if	(g_aActor[m_wEngageTarget].isDeath())
			releaseEngagedTarget();

	if	(s_bIs1Second)
		operateOncePerSecondWork();

	if	(m_isWalking && m_bf1IsReloadingBeltItem)	//	리로드 중에 이동하면 리로드가 취소된다.
	{
		m_bf1IsReloadingBeltItem	=	FALSE;
		g_lpHero->warning(dMSG_RELOAD_BELT_ITEM_CANCEL);
	}

	if	(!s_isUpdateTimingByUpdatePeriod)
		return;

	operateRecoverHP();
	reduceCP();
}

void
CHero::reduceCP()	//	CP 줄이기
{	
	if(m_wMadnessTime)		// 광란상태.
		return;
	if (m_wMaintainMaxCPTime)
	{	
		m_iCP	=	getMaxCP();
		return;
	}

	if (m_wCPFloatTime)		//	m_wCPFloatTime	=	(1+getCharisma()/100)*16
	{
		m_wCPFloatTime--;
		return;
	}

	if	(g_lpHero->m_wMagicCarpetType && m_bf1IsExistSquirrelCarpet)
		return;

	if	(m_iCP	<	m_wBottomCP*100)
		return;

	if	(m_bf1IsRunning	&&	m_isWalking)
	{
		int iPassiveConcentration = getValuePassiveSkill(eSKILL_PEE_INCREASE_CONCENTRATION)/100;
		int	iConcentration	=	min(m_concentration.getCorrectedValue()+m_concentrationByStellarPeer.getCorrectedValue()  + iPassiveConcentration,dMAXIMUM_CONCENTRATION_VALUE);
		int	iCorrectCP		=	max(10,100-iConcentration);
		int	iReduceCP		=	(45* getCharisma()/100+500)*iCorrectCP/100/dSYNC_FPS_IN_SERVER;
//● CP 감소 속도 = (-0.0045 * 카리스마 + 5) * 아이템, 기술에 의한 CP 감소율 보정치 [포인트/초]
//				iReduceCP	*=	dREDUCE_CP_RUNNING_FACTOR;

		m_iCP				=	max(m_iCP-iReduceCP,m_wBottomCP*100);
	}
}

//
//	체력 회복
void
CHero::operateRecoverHP()
{
	int		iRecoverHP;

	if	(m_iForcedRecoverHP)
	{
		iRecoverHP		=	m_iForcedRecoverHP;
		iRecoverHP		=	min(iRecoverHP,getMaxHP()-m_iHP);

		if	(iRecoverHP	>	0)
		{
			m_iHP				+=	iRecoverHP;
			m_iForcedRecoverHP	-=	iRecoverHP;
		}
	}

	regenHP();

	if	(g_lpHero->isSitdown() || (g_lpHero->m_wMagicCarpetType && m_bf1IsExistTurttleCarpet))
	{
//	HP Regen 속도 (앉았을 시) = 1 * (1 + 건강 /100) * (1 + 칭호에 의한 보정치) [포인트/초]
		iRecoverHP		=	(100 + getConstitution())/dSYNC_FPS_IN_SERVER;
		iRecoverHP		=	iRecoverHP*(m_wCampingMasterTitleFactor+m_wIncreaseSitdownHPRecoverSpeed)/100;
		m_iHP			+=	iRecoverHP;
		m_iHP			=	min(m_iHP,getMaxHP());
		g_lpHero->m_iHP	=	m_iHP;
	}

	if	(m_iRecoverHP	<=	0)
		return;

//	HP Regen 속도 (약을 먹었을 시) = 10 * (1 + 건강 /50) * (1 + 아이템에 의한 보정치) [포인트/초]
	iRecoverHP		=	10*(100+getConstitution()*2)*(100+m_bCorrectHPRecoverSpeed)/100/dSYNC_FPS_IN_SERVER;
	iRecoverHP		=	min(m_iRecoverHP,iRecoverHP);
	m_iRecoverHP	-=	iRecoverHP;

	if	(m_wDeclientRecoverHPEfficientTime)
		iRecoverHP	-=	iRecoverHP*m_wDeclientRecoverHPEfficient/100;

	m_iHP			+=	iRecoverHP;
	m_iHP			=	min(m_iHP,getMaxHP());

	g_lpHero->m_iHP	=	m_iHP;
}

//
//	버서커 상태로 만드는 효과 추가
void
CHero::addMakeBerserk(int _iChance,int _iTime)
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
}	//	CHero::addMakeBerserk(int _iChance,int _iTime)

void
CHero::addMakeStone(int _iChance,int _iTime)
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
CHero::addMakeSleep(int _iChance,int _iTime)
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
CHero::addMakeFirstTarget(int _iChance,int _iTime)
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
CHero::addMakeSimpleton(int _iChance,int _iTime)
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
CHero::addMakeArmorBreak(int _iChance,int _iTime)
{
	if (m_bTargetArmorBreakChance	<	_iChance)
	{
		m_bTargetArmorBreakChance	=	_iChance;
		m_wTargetBerserkTime	=	_iTime;
	}
	else
	if (m_bTargetArmorBreakChance	==	_iChance)
	{
		if (m_wTargetBerserkTime	<	_iTime)
		{
			m_bTargetArmorBreakChance	=	_iChance;
			m_wTargetBerserkTime		=	_iTime;
		}
	}

	m_bTargetArmorBreakChance	=	min(m_bTargetArmorBreakChance,dMAXIMUM_MAKE_BREAK_ARMOR_CHANCE);
}	//	CHero::addMakeArmorBreak(int _iChance,int _iTime)

void
CHero::addMakeWeaponBreak(int _iChance,int _iTime)
{
	if (m_bTargetWeaponBreakChance	<	_iChance)
	{
		m_bTargetWeaponBreakChance	=	_iChance;
		m_wTargetBerserkTime	=	_iTime;
	}
	else
	if (m_bTargetWeaponBreakChance	==	_iChance)
	{
		if (m_wTargetBerserkTime	<	_iTime)
		{
			m_bTargetWeaponBreakChance	=	_iChance;
			m_wTargetBerserkTime		=	_iTime;
		}
	}

	m_bTargetWeaponBreakChance	=	min(m_bTargetWeaponBreakChance,dMAXIMUM_MAKE_BREAK_WEAPON_CHANCE);
}

void
CHero::addMakeConfuse(int _iChance,int _iTime)
{
	if (m_bTargetConfuseChance	<	_iChance)
	{
		m_bTargetConfuseChance	=	_iChance;
		m_wTargetBerserkTime	=	_iTime;
	}
	else
	if (m_bTargetConfuseChance	==	_iChance)
	{
		if (m_wTargetBerserkTime	<	_iTime)
		{
			m_bTargetConfuseChance	=	_iChance;
			m_wTargetBerserkTime		=	_iTime;
		}
	}

	m_bTargetConfuseChance	=	min(m_bTargetConfuseChance,dMAXIMUM_MAKE_CONFUSE_CHANCE);
}

void
CHero::addMakeCharmed(int _iChance,int _iTime)
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
CHero::addMakeStun(int _iChance,int _iTime)
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
CHero::addMakeCold(int _iChance,int _iTime)
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
CHero::addMakeFreeze(int _iChance,int _iTime)
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
CHero::addMakeBlind(int _iChance,int _iTime)
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
CHero::addAddPoisonDamage(int _iDamage,int _iTime)
{
	m_bAddPoisonMinDamage	+=	_iDamage;
	m_bAddPoisonMaxDamage	+=	_iDamage;

	if (m_wTargetPoisonedTime	>	_iTime	||	m_wTargetPoisonedTime	==	0)	m_wTargetPoisonedTime	=	_iTime;

	m_bAddPoisonMaxDamage	=	min(m_bAddPoisonMaxDamage,dMAXIMUM_POISON_DAMAGE);
	m_bAddPoisonMinDamage	=	min(m_bAddPoisonMinDamage,dMAXIMUM_POISON_DAMAGE);
}

void
CHero::addAddPoisonDamage(int _iMinDamage,int _iMaxDamage,int _iTime)
{
	m_bAddPoisonMinDamage	+=	_iMinDamage;
	m_bAddPoisonMaxDamage	+=	_iMaxDamage;

	if (m_wTargetPoisonedTime	>	_iTime	||	m_wTargetPoisonedTime	==	0)	m_wTargetPoisonedTime	=	_iTime;

	m_bAddPoisonMaxDamage	=	min(m_bAddPoisonMaxDamage,dMAXIMUM_POISON_DAMAGE);
	m_bAddPoisonMinDamage	=	min(m_bAddPoisonMinDamage,dMAXIMUM_POISON_DAMAGE);
}

void
CHero::addAddFireDamage(int _iDamage)
{
	m_wAddFireMinDamage	=	max(m_wAddFireMinDamage,_iDamage);
	m_wAddFireMaxDamage	=	max(m_wAddFireMaxDamage,_iDamage);

	m_wAddFireMinDamage	=	min(m_wAddFireMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddFireMaxDamage	=	min(m_wAddFireMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::addAddWaterDamage(int _iDamage)
{
	m_wAddWaterMinDamage+=	_iDamage;
	m_wAddWaterMaxDamage+=	_iDamage;
	m_wAddWaterMinDamage=	min(m_wAddWaterMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddWaterMaxDamage=	min(m_wAddWaterMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::addAddWindDamage(int _iDamage)
{
	m_wAddWindMinDamage	+=	_iDamage;
	m_wAddWindMaxDamage	+=	_iDamage;
	m_wAddWindMinDamage	=	min(m_wAddWindMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddWindMaxDamage	=	min(m_wAddWindMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::addAddLightDamage(int _iDamage)
{
	m_wAddLightMinDamage+=	_iDamage;
	m_wAddLightMaxDamage+=	_iDamage;
	m_wAddLightMinDamage=	min(m_wAddLightMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddLightMaxDamage=	min(m_wAddLightMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::addAddDarkDamage(int _iDamage)
{
	m_wAddDarkMinDamage	+=	_iDamage;
	m_wAddDarkMaxDamage	+=	_iDamage;
	m_wAddDarkMinDamage	=	min(m_wAddDarkMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddDarkMaxDamage	=	min(m_wAddDarkMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::addAddFireDamage(int _iMinDamage,int _iMaxDamage)
{
	m_wAddFireMinDamage	+=	_iMinDamage;
	m_wAddFireMaxDamage	+=	_iMaxDamage;
	m_wAddFireMinDamage	=	min(m_wAddFireMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddFireMaxDamage	=	min(m_wAddFireMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::addAddWaterDamage(int _iMinDamage,int _iMaxDamage)
{
	m_wAddWaterMinDamage+=	_iMinDamage;
	m_wAddWaterMaxDamage+=	_iMaxDamage;
	m_wAddWaterMinDamage=	min(m_wAddWaterMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddWaterMaxDamage=	min(m_wAddWaterMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::addAddWindDamage(int _iMinDamage,int _iMaxDamage)
{
	m_wAddWindMinDamage	+=	_iMinDamage;
	m_wAddWindMaxDamage	+=	_iMaxDamage;
	m_wAddWindMinDamage	=	min(m_wAddWindMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddWindMaxDamage	=	min(m_wAddWindMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::addAddLightDamage(int _iMinDamage,int _iMaxDamage)
{
	m_wAddLightMinDamage+=	_iMinDamage;
	m_wAddLightMaxDamage+=	_iMaxDamage;
	m_wAddLightMinDamage=	min(m_wAddLightMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddLightMaxDamage=	min(m_wAddLightMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::addAddDarkDamage(int _iMinDamage,int _iMaxDamage)
{
	m_wAddDarkMinDamage	+=	_iMinDamage;
	m_wAddDarkMaxDamage	+=	_iMaxDamage;
	m_wAddDarkMinDamage	=	min(m_wAddDarkMinDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
	m_wAddDarkMaxDamage	=	min(m_wAddDarkMaxDamage,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}


void
CHero::addAddDamageToUndead(int _iDamage)
{
	m_bAddDamageToUndead+=	_iDamage;
	m_bAddDamageToUndead=	min(m_bAddDamageToUndead,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}
//VV     09.08.07			마법추가데미지DX 추가 ~ㄷㄷㄷ 
void
CHero::addAddNaturalDamageDX(int _iClass, int _iDamage)
{
	m_sAddNaturalDamageDX[_iClass].wMin += _iDamage;
	m_sAddNaturalDamageDX[_iClass].wMax += _iDamage;

	m_sAddNaturalDamageDX[_iClass].wMin	=	min(m_sAddNaturalDamageDX[_iClass].wMin,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE_DX);
	m_sAddNaturalDamageDX[_iClass].wMax =	min(m_sAddNaturalDamageDX[_iClass].wMax ,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE_DX);
}
void
CHero::addAddNaturalDamageDX(int _iClass, int _iMinDamage,int _iMaxDamage)
{
	m_sAddNaturalDamageDX[_iClass].wMin += _iMinDamage;
	m_sAddNaturalDamageDX[_iClass].wMax += _iMaxDamage;

	m_sAddNaturalDamageDX[_iClass].wMin	=	min(m_sAddNaturalDamageDX[_iClass].wMin,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE_DX);
	m_sAddNaturalDamageDX[_iClass].wMax =	min(m_sAddNaturalDamageDX[_iClass].wMax ,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE_DX);
}

//AA     09.08.07			마법추가데미지DX 추가 ~ㄷㄷㄷ 
void
CHero::addAddDamageToAnimal(int _iDamage)
{
	m_bAddDamageToAnimal+=	_iDamage;
	m_bAddDamageToAnimal=	min(m_bAddDamageToAnimal,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::addAddDamageToHuman(int _iDamage)
{
	m_bAddDamageToHuman	+=	_iDamage;
	m_bAddDamageToHuman	=	min(m_bAddDamageToHuman,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::addAddDamageToHolyAnimal(int _iDamage)
{
	m_bAddDamageToAnimal+=	_iDamage;
	m_bAddDamageToAnimal=	min(m_bAddDamageToAnimal,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::addAddDamageToDemon(int _iDamage)
{
	m_bAddDamageToDemon	+=	_iDamage;
	m_bAddDamageToDemon	=	min(m_bAddDamageToDemon,dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE);
}

void
CHero::declineRecoverHPEfficient(int _iTime,int _iEfficient)
{
	m_wDeclientRecoverHPEfficientTime	=	_iTime;
	m_wDeclientRecoverHPEfficient		=	_iEfficient;
}

void
CHero::buildPower(BOOL _bIsRequireCheckBeltItem, BOOL _bIsBySkillPointReset, BOOL _bIsOperatePassiveSkill)
{
	resetEquipmentData();
	rebuildBasicData();

	m_bf1IsOwnedPitchmanShopSignBoard1	=	FALSE;
	m_bf1IsOwnedPitchmanShopSignBoard2	=	FALSE;
	m_bf1IsOwnedPitchmanShopSignBoard3	=	FALSE;
	m_bf1IsOwnedPitchmanShopSignBoard4	=	FALSE;
	m_bf1IsOwnedPitchmanShopSignBoard5	=	FALSE;
	m_bf1IsOwnedPitchmanShopLoudSpeaker	=	FALSE;
	m_bf1IsOwnedPitchmanColorPen		=	FALSE;
	m_bf1IsOwnedPitchmanShopBoldPen		=	FALSE;
	m_bf1IsOwnedPitchmanShopLongPen		=	FALSE;
	m_bf1IsOwnedPitchmanShopTwinkle		=	FALSE;
	m_bf1IsUseAbleBulletSubstituteArrow	=	FALSE;
	m_wIncreaseFinalPhysicalAttackPower	=	0;

	operateTitleData();
	operateSpecialItemInInventory();
	
	iMiniPet()->operateBuffSkill();
	operateGuildSkillData();

#ifdef _FOR_JAPAN	//JBC	넷카페 유저 특전 40랩 배지.		08-08-12
	if(g_hero.m_bf1IsNetCafeUser)
		m_wLimitEquipmentLevelOverPoint	+=	40;
#endif

	operateEquipmentData(_bIsRequireCheckBeltItem , _bIsBySkillPointReset);

	if(_bIsOperatePassiveSkill)
		operatePassiveSkill();	// 패시브스킬 셋팅

	m_iHP	=	min(m_iHP,getMaxHP());
}



void
CHero::increaseCP(int _iCP)	//	CP 더하기
{
	if	(g_lpHero	&&	g_lpHero->m_enchantedImage.m_bf1IsNoMoreFight)
		return;
	if(m_fightEnergy.isIncreaseGetCP())
	{
		_iCP += m_fightEnergy.m_iValue;
	}
	m_iCP			=	m_iCP+(_iCP*m_bGetBonusCPPercentage)/100;
	m_iCP			=	min(m_iCP+_iCP,getMaxCP());
	m_wCPFloatTime	=	dDEFAULT_CP_FLOAT_TIME+getCharisma()*dSYNC_FPS/100;
}
int
CHero::getDefensivePower(BOOL _bIsWantBaseValue)
{
	if	(_bIsWantBaseValue)
	{
		int		iDefensivePower				=	m_defensePower.m_iValue;

		if	(m_armorPower.m_sFixValue	!=	0x7fff)
			iDefensivePower	+=	m_armorPower.m_sFixValue;
		else
			iDefensivePower	+=	m_armorPower.m_sValue;

		iDefensivePower	=	iDefensivePower*(100 +m_defensePower.m_sCorrectPercentageValueByPrefix)/100 + m_defensePower.m_sCorrectValueByPrefix*100;
		iDefensivePower	=	iDefensivePower*(100 +getConstitution())/100;

		if	(m_wBreakedArmorTime)
			iDefensivePower	/=	dBREAK_EQUIPMENT_FACTOR;

		return	max(iDefensivePower,1);
	}

	int		iDefensivePower					=	m_defensePower.m_iValue+m_armorPower.getCorrectedValue();
	int		iCorrectDefenseValue			=	(m_defensePower.m_sCorrectValueByPrefix+m_defensePower.m_sCorrectValue)*100;
	int		iCorrectBySkillDefense			=	getDefensePowerByPassiveSkill();
	int		iCorrectPercentageBySkillDefense=	getDefensePercentageByPassiveSkill();

	iDefensivePower	=	iDefensivePower*(100 +m_defensePower.getCorrectPercentageValue()+iCorrectPercentageBySkillDefense)/100 + iCorrectDefenseValue+iCorrectBySkillDefense;
	iDefensivePower	=	iDefensivePower*(100 +getConstitution())/100;

	if(m_wCorrectFinalDefensivePowerPercentageTime)
		iDefensivePower	+=	iDefensivePower*m_sCorrectFinalDefensivePowerPercentage/100;

	if	(m_wBreakedArmorTime)
		iDefensivePower	/=	dBREAK_EQUIPMENT_FACTOR;

	return	max(iDefensivePower,1);
}

//	보조 마법같은것의 영향으로 방어력이 올라갔냐?
BOOL
CHero::isPlusDefense()
{
	if	(m_defensePower.m_sCorrectValue			>	0)
		return	TRUE;
	if	(m_defensePower.m_sCorrectPercentageValue>	0)
		return	TRUE;
	if	(m_wCorrectFinalDefensivePowerPercentageTime && m_sCorrectFinalDefensivePowerPercentage>	0)
		return TRUE;
	if	(m_armorPower.getCorrectedValue())
		return	TRUE;
	if	(getDefensePowerByPassiveSkill())
		return	TRUE;

	return	FALSE;
}

//
//	보조 마법같은것의 영향으로 방어력이 떨어졌냐?
BOOL
CHero::isMinusDefense()
{
	if	(m_defensePower.m_sCorrectValue			<	0)
		return	TRUE;
	if	(m_defensePower.m_sCorrectPercentageValue<	0)
		return	TRUE;
	if	(m_wCorrectFinalDefensivePowerPercentageTime && m_sCorrectFinalDefensivePowerPercentage<	0)
		return TRUE;
	return	FALSE;
}

void
CHero::setReversalPowerTime(int _iTime)
{
	m_wReversalPowerTime	=	_iTime;
}

BOOL
CHero::setInterruptingArmor(int _iCurrentLevel,int _iTime)
{
	m_wInterruptingArmorTime	=	_iTime;
	m_wInterruptingArmorLevel	=	_iCurrentLevel;

	buildPower();

	return	TRUE;
}

BOOL
CHero::setInterruptingWeapon(int _iTime)
{
	m_wInterruptingWeaponTime	=	_iTime;

	buildPower();

	return	TRUE;
}
