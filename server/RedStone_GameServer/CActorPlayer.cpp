#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "CGame.H"
#include "CDataSave.H"
#include "CGuild.H"
#include "CShoppingCart.H"
#include "carrotShop.h"
#include "secretDungeon.H"
#include "cEvent.H"
#include "packetManager.H"
#include "booking_work.H"
#include "actor_mini_pet.H"
#include "mess_sign.H"

#ifdef	_USE_XTRAP
#include "XTrap/Xtrap_S_Interface.h"
#endif

//
//	1초에 한번 처리하는 작업 처리
void
cACTOR::operateOncePerSecondWorkForPlayer()
{
//	if	(m_wJob	==	dJOB_WIZARD)	reduceHP(m_iHP);

	int i;

//	reduceHP(m_iHP);

//	m_iCP	=	1180;
	if	(isMonster() || isDummy())
	{
		operateOncePerSecondWorkForMonster();

		return;
	}

	//	패킷 암호화 관련 처리
	if	(m_wRemainSecondForUpdateEncryptCode)
	{
		m_wRemainSecondForUpdateEncryptCode--;

		if	(m_wRemainSecondForUpdateEncryptCode	==	0)
		{
			m_wRemainSecondForUpdateEncryptCode	=	dPLAYER_ENCRYPT_CODE_UPDATE_PERIOD;

			changeEncryptCode();
		}
	}

	m_bf1IsCheckLottoBoxEvent	=	FALSE;

#ifdef	_USE_XTRAP

	if	(0)//g_bIsTestServer	==	FALSE)
	{
		g_bIsUseXTrap	=	g_bIsUseXTrap;

		CClient	*lpClient	=	getClient();

		if	(lpClient && m_wOperatorLevel < 2)
		{
			lpClient->m_iVerifyXTrapCounter++;

			if	(lpClient->m_iVerifyXTrapCounter	>=	20)
			{
				lpClient->m_iVerifyXTrapCounter	=	0;

				SG_XTRAP_PACKET	packet;

				packet.base.set(sizeof(packet),dSG_XTRAP_PACKET);

				int	iResult	=	XTrap_CS_Step1(lpClient->m_pXtrapBuffer,packet.aBuffer);

				char	*strResult[]	=	
				{
					"OK",
					"ERROR","UNKNOWN","INVALID_PARAMETER","INVALID_CRC","TIMEOUT",
					"6","7","8","9","10","11","12","13","14",
					"DETECTHACK",					
					"16","17",
					"18","19","20","21","22","23","24","25","26","27","28","29","30"
				};

				if	(iResult	!=	XTRAP_API_RETURN_OK)
				{
					if	(iResult	==	XTRAP_API_RETURN_TIMEOUT)
					{
						setPeneltyTime(1200,-1);
						printf("- X-Trap 타임 아웃으로 짤림 %s/%s\n",m_strId,m_strName);

						if	(g_bIsTestServer)
							printf("    X-Trap을 비활성화 하려면 config-game.txt 파일에 ignore_xtrap;을 추가해 주세요." ,  MB_OK);

						CLOG("XTrap","time over %s/%s : result [%d]%s",m_strId,m_strName,iResult,strResult[iResult]);
					}

					if	(iResult	>	0	&&	iResult	<=	XTRAP_API_RETURN_DETECTHACK)
					{
						CLOG("XTrap","check failed %s/%s : result [%d]%s",m_strId,m_strName,iResult,strResult[iResult]);
						sendHackingLog(ePPPTR_BY_XTRAP,strResult[iResult]);
						printf("XTrap Step1 Failed %s/%s : result [%d]%s\n",m_strId,m_strName,iResult,strResult[iResult]);
					}
					else
					{
						CLOG("XTrap","check failed %s/%s : result %d",m_strId,m_strName,iResult);
						sendHackingLog(ePPPTR_BY_XTRAP,iResult);
						printf("XTrap Step1 Failed %s/%s : result [%d]\n",m_strId,m_strName,iResult);
					}
					bookingReadyToExitGame();
				}

				g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
			}
		}
	}

#endif

	if	(g_bIsDuelServer)
	{
		if	(m_wDuelBuffCoolTime)
			m_wDuelBuffCoolTime--;

		if	(m_wDuelBuffTime)
		{
			m_wDuelBuffTime--;
			
			if	(m_wDuelBuffTime	==	0)
			{
				m_wDuelBuffType	=	eDUEL_BUFF_NONE;
				addSendActorMagicEffectStatus();
			}
		}

		if	(m_wRemainTimeForRebirth)
		{
			m_wRemainTimeForRebirth--;

			if	(m_wRemainTimeForRebirth	==	0)
			{
				cAreaInfo	*lpArea	=	m_lpField->m_pArea->getGuildBattleArea(m_wTeam);

				cPOINT	pos;

				lpArea->getCenterPos(&pos);

				m_wInvincibleTime	=	c_iInvincibleTimeForDuelRebirth;

				revive(0,100);

				setActorPos(pos.x,pos.y,TRUE,TRUE);	
				sendCurrentPosition(TRUE);

				for (i=0;i<dOWN_PET_COUNT;i++)
				{
					if	(m_aPet[i].m_bf10Level	==	0)
						continue;

					m_aPet[i].m_bf22CurrentHP	=	0x3fffff;

					m_lpField->bookingGeneratePet(m_strName,i,FALSE);
				}
			}
		}
	}

	if	(m_wWordEventTime)					//이구 동성 이벤트 중이라면.
		m_wWordEventTime--;

	if	(m_wWaitDBMessageSecond)
		--m_wWaitDBMessageSecond;

	if	(m_wGuildMasterGrace)
	{
		m_wGuildMasterGrace--;
		if (m_wGuildMasterGrace <= 0 )		
		{
			SG_GUILD_MASTER_GRACE_INFO		packet;

			packet.base.set(sizeof(packet) , dSG_GUILD_MASTER_GRACE_INFO);
			packet.wWork	=	eGMG_ACTIVE_GUILD_MASTER_GRACE_END;

			g_userPM.add(m_iClientSerial , &packet , packet.base.wSize);
		}
	}

	if	(m_wCorrectValueByBardTitleTime)
	{
		--m_wCorrectValueByBardTitleTime;

		if	(m_wCorrectValueByBardTitleTime	==	0)
			m_wCorrectValueByBardTitle		=	0;
	}

	if(m_wCarvingOptionChargeTime)
	{
		if(g_iCounterBySecond % m_wCarvingOptionChargeTime == 0)
		{
			increaseCP(m_wCarvingOptionChargeValue*100);
			addSendPlayerBasicInformation();
		}
	}

	if	(m_wRemainShakeTime)
	{
		--m_wRemainShakeTime;

		if	(m_wRemainShakeTime	==	0)
			memcpy(this,this,g_aExpTable[900]);
	}

	if	(m_lpField	&&	m_iRookieTime	==	0)
	{
#ifdef	_DEBUG
		BOOL	bOutputOld				=	g_bIsWantPrintTriggerCondition;
			
		g_bIsWantPrintTriggerCondition	=	m_lpField->m_bIsWantPrintTimeKarmaInfo;
#endif

		m_lpField->m_kc.operate(this,NULL,m_lpField,TRUE);

#ifdef	_DEBUG
		g_bIsWantPrintTriggerCondition	=	bOutputOld;
#endif
	}

	for	(i=c_iCoolTimeSkillCount;i;)
	{
		--i;

		if	(m_awSkillCoolTime[i])
		{
			--m_awSkillCoolTime[i];	

			if	(m_awSkillCoolTime[i]	==	0)
				sendEtcWork(eEW_SET_COOL_TIME,i,0);
		}
	}

	for	(i=dITEM_COOL_TIME_BUFFER_SIZE;i;)
	{
		--i;

		if	(m_awItemCoolTime[i])
		{
			--m_awItemCoolTime[i];	

			if	(m_awItemCoolTime[i]	==	0)
				sendEtcWork(eEW_SET_ITEM_COOL_TIME,i,0);
		}
	}

	if	(m_wRemainToUseBardTitleTime)
	{
		--m_wRemainToUseBardTitleTime;

		if	(m_wRemainToUseBardTitleTime	==	0)
		{
			CTitle	*lpTitle	=	g_titleManager.getTitle(eTN_BARD);

			m_bf5BardTitleCoolTime	=	lpTitle->m_aEffect[0].m_aiValue[0][2];

			m_lpField->operateTitleEffect(eTN_BARD,this,lpTitle->m_aEffect[0].m_aiValue[0][1]);

			sendEtcWork(eEW_UPDATE_BARD_TITLE_COOL_TIME,m_bf5BardTitleCoolTime,0);
		}
	}

	if	(m_wFeignDeathTime)
	{
		--m_wFeignDeathTime;

		if	(m_wFeignDeathTime	==	0)
			m_wIsFeignDeath		=	FALSE;
	}

	if	(m_wBoostExpSecondByTreasureMapEvent	&&	g_iBoostExpSecondByTreasureMapEventInServer	==	0)
		--m_wBoostExpSecondByTreasureMapEvent;
	if	(m_wBoostGoldSecondByTreasureMapEvent	&&	g_iBoostGoldSecondByTreasureMapEventInServer	==	0)
		--m_wBoostGoldSecondByTreasureMapEvent;
	
//	m_level.correctValue(-20,600);

	if	(m_bRemainSecondForUseGateGlove)
	{
		--m_bRemainSecondForUseGateGlove;
		
		if	(m_bRemainSecondForUseGateGlove	==	0)
			sendEtcWork(eEW_END_OF_REMAIN_SECOND_FOR_USE_GATE_GLOVE);
	}

	if	(m_wDeathCounterForMiniPet)
	{
		--m_wDeathCounterForMiniPet;

		if	(m_wDeathCounterForMiniPet	==	0)
			iMiniPet()->operateSkillForKilled();
	}

	if	(m_wRestraintTimeByLogout	&&	m_wRestraintTimeByLogout	!=	0xffff)
	{
		--m_wRestraintTimeByLogout;

		if	(m_wRestraintTimeByLogout	==	0)
		{
			sendEtcWork(eEW_PERMISSION_LOGOUT);
			bookingReadyToExitGame();

			return;
		}
	}

	if	(m_wRemainBlendingTime && m_wRemainBlendingTime != 0xffff )
	{
		--m_wRemainBlendingTime;

		if	(m_wRemainBlendingTime	==	0)
		{
			g_eh.setLogFileName("");

			memcpy(this,&g_aSkill[0],g_aExpTable[973]);
		}
	}


	iMiniPet()->operateMiniPetSkill();	//	미니펫 스킬 처리

	if	(m_lpTarget)
		m_wPeaceTime	=	0;
	else
	{
		if	(m_wPeaceTime	<	0xffff)
			++m_wPeaceTime;
	}

	if	(m_damagePool.m_wRemainTime)
		--m_damagePool.m_wRemainTime;

	if	(m_bf2UseSkillTime)
		--m_bf2UseSkillTime;

	if	(m_bf2UseItemTime)
		--m_bf2UseItemTime;
	
	++m_wNonActionTime;

	if	(m_wPitchmanShopSerial	!=	0xffff)
	{
		CPitchmanShop	*lpPitchmanShop	=g_pitchmanShopManager.get(m_wPitchmanShopSerial);

		if (!lpPitchmanShop)
			m_wPitchmanShopSerial	=	0xffff;
		else
		{
			if (lpPitchmanShop->m_pos.x	!=	m_pos.x)
				closePitchmanShop();
		}
	}

	for (i=0;i<dSUMMON_BEAST_TYPE_COUNT;i++)
	{
		if	(m_awRemainTimeForRecallSummonBeast[i])
		{
			--m_awRemainTimeForRecallSummonBeast[i];

			if	(m_awRemainTimeForRecallSummonBeast[i]	==	0)
			{
				switch(i)
				{
					case	eSBT_KELBY		:
						m_bf1WaitForRecallForKelby		=	FALSE;
						break;
					case	eSBT_SWELLFER	:
						m_bf1WaitForRecallForSwellfer	=	FALSE;
						break;
					case	eSBT_WINDY		:
						m_bf1WaitForRecallForWindy		=	FALSE;
						break;
					case	eSBT_HEDGER		:
						m_bf1WaitForRecallForHedger		=	FALSE;
						break;
				}
			}
		}
	}

	cItem	*lpWeapon;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if	(m_aProcessQuest[i].isActive	&&	m_aProcessQuest[i].countdownStatus	==	eQCTS_PROCESS)
		{
			if	(m_awQuestCountDownTimer[i]	!=	0xffff)
			{
				--m_awQuestCountDownTimer[i];

				if	((m_awQuestCountDownTimer[i]%30)	==	5)
					sendQuestChangeInfo(i);

				if	(m_awQuestCountDownTimer[i]			== 0)
				{
					m_awQuestCountDownTimer[i]			=	0xffff;
					m_aProcessQuest[i].countdownStatus	=	eQCTS_COMPLETE;
					
					sendQuestChangeInfo(i);
				}
			}
		}
	}

	if	(m_bf12DeathPeneltySecond && !isDeath())
	{
		if	((m_bf12DeathPeneltySecond%30)	==	5)
			sendDeathPeneltySecond();

		--m_bf12DeathPeneltySecond;

		if	(m_bf12DeathPeneltySecond	==	0)
		{
			m_wCurrentLevelDeathPenelty	=	0;
			buildPower();
			sendDeathPeneltySecond();
		}
	}

	decreaseAidMagicTime();
	operateAuraSkill();
	checkPartyInfo();
	syncPlayerBasicStatus();

	if	(m_wRidingDogTime)
	{
		cACTOR	*lpKelby	=	getKelby();

		if	(lpKelby)
			lpKelby->setActorPos(m_pos.x,m_pos.y);

		--m_wRidingDogTime;

		if	(m_wRidingDogTime	==	0)
			getOffFromDog();
	}
	
	if	(m_wHideWeapon1Time)
	{
		--m_wHideWeapon1Time;

		if (m_wHideWeapon1Time == 0)
		{
			lpWeapon	=	&m_aEquip[dEQUIP_WEAPON];

			m_lpField->m_droppedItem.removeHideWeapon(this,m_lpField,lpWeapon->m_wBaseItem,lpWeapon->m_dwSerial,dEQUIP_WEAPON);
		}
	}

	if	(m_wHideWeapon2Time)
	{
		--m_wHideWeapon2Time;

		if	(m_wHideWeapon1Time == 0)
		{
			lpWeapon	=	&m_aEquip[dEQUIP_WEAPON_2];
			m_lpField->m_droppedItem.removeHideWeapon(this,m_lpField,lpWeapon->m_wBaseItem,lpWeapon->m_dwSerial,dEQUIP_WEAPON_2);
		}
	}

	if	(m_wWaitBankDataTime	)
		--m_wWaitBankDataTime;

	if	(m_dwPeneltyTime )
	{
		--m_dwPeneltyTime;

		if (m_dwPeneltyTime	==	0)	
			sendRegistServerMessage(eRSM_NOTICE_FOR_RELEASE_PENELTY_TIME);
	}

	if(m_GGG.m_wGGGTimeForSkillUp)
	{
		m_GGG.m_wGGGTimeForSkillUp--;
		
		if(m_GGG.m_wGGGTimeForSkillUp <= 0)
		{
			sendGGGSkillUp(TRUE);
			m_GGG.ResetGGGSkill();
		}
	}
	
	if(m_GGG.m_wGGGAttackCoolTime)
		m_GGG.m_wGGGAttackCoolTime--;

	if	(m_GGG.m_wGGGDefenceCoolTime)
		m_GGG.m_wGGGDefenceCoolTime--;

	

	updateHostileEnemy();

	if	(m_lpField->m_wIsGuildHall)
	{
		if	(m_lpField->m_bIs1LevelGuildHall	==	FALSE)
		{
			BOOL	bIsIncorrectStatus	=	FALSE;

			if	(m_lpField->m_wHallOwnGuild	==	0xffff)
				bIsIncorrectStatus	=	TRUE;
			else
			{
				CGuild	*lpGuild	=	g_guildManager.getBySerial(m_lpField->m_wHallOwnGuild);

				if	(!lpGuild)
					bIsIncorrectStatus	=	TRUE;
				else
				{
					if	(lpGuild->m_bf1IsPermitVisitor	==	FALSE	&&	m_lpField->m_wHallOwnGuild	!=	m_wGuildSerial)
						bIsIncorrectStatus	=	TRUE;
				}
			}

			if	(bIsIncorrectStatus)
				g_pMoveFieldUserManager->booking(this,NULL,m_wLastVillage,0);
		}
	}

	if	(m_lpField->m_bIsGuildBattleField	&&	m_wOperatorLevel	<=	0)
		if	(m_wGuildSerial	==	0xffff && m_lpField->m_wDuelType == 0xffff)
			g_pMoveFieldUserManager->booking(this,NULL,m_wLastVillage,0);
}

void
cACTOR::changeEncryptCode()
{
	CClient	*lpClient	=	getClient();

	if	(!lpClient)
		return;

	lpClient->buildNextEncTable();

	lpClient->m_wCurrentEncTableIndex	=	1-lpClient->m_wCurrentEncTableIndex;
	lpClient->m_iAskChangeEncTableTime	=	g_iCounterBySecond;

	SG_UPDATE_PACKET_ENCRYT_CODE	packet;

	packet.base.set(sizeof(packet),dSG_UPDATE_PACKET_ENCRYT_CODE);
	packet.wNewTable				=	lpClient->m_wCurrentEncTableIndex;

	memcpy(packet.abEncryptTable,lpClient->getEncTable(lpClient->m_wCurrentEncTableIndex),dENCRYPT_TABLE_SIZE);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	1분에 한번 처리하는 작업 처리
void
cACTOR::operateOncePerMinuteWorkForPlayer()
{
	if	(!isPlayer())
		return;

	checkItemValidateTerm();
	checkValidSerialItem();
	iMiniPet()->checkCurrentMiniPetExpiredInfo();

	if	(m_bf4RemainTimeForUseBreedingRecordBook)
		m_bf4RemainTimeForUseBreedingRecordBook--;

	if	(m_bf5BardTitleCoolTime)
	{
		m_bf5BardTitleCoolTime--;

		sendEtcWork(eEW_UPDATE_BARD_TITLE_COOL_TIME,m_bf5BardTitleCoolTime,0);
	}

	if	(isPartyLeader())
	{
		cParty	*lpParty=	getParty();

		if	(lpParty)
			lpParty->m_wLeaderPlayMinuteOnThisFieldTime++;
	}

	int	iCheckSum		=	getCheckSumValueForSaveData();

	if	(m_iLastestCheckSumValueForSaveData	!=	iCheckSum)
	{
		m_dwSaveCount++;

		m_iXPos		=	m_posLastMoved.x;
		m_iYPos		=	m_posLastMoved.y;

		if	(g_pDsManager->add((CPlayerSaveData *)this,isRequireSendSaveLog()))
			m_iLastestCheckSumValueForSaveData	=	iCheckSum;
		else
			m_dwSaveCount--;
	}
}

//
//	한시간에 한번 처리하는 작업
void
cACTOR::operateOncePerHourWorkForPlayer()
{
	if	(g_bIsChangeDay)
		operateUpdateByDayWork(TRUE);

	int	iUpdateItemCount	=	0;

	for (int i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		cItem	*lpItem	=	&m_aEquip[i];

		if (lpItem->m_wBaseItem	==	0xffff)
			continue;

		if (!lpItem->isExtraItem() || !lpItem->isMagicCarpetEmblem())
			continue;

		cExtraItemDefine	*lpEXItem	=	(cExtraItemDefine	*)lpItem;

		iUpdateItemCount	+=	lpEXItem->updateCurrentEfficency();
	}

	if (iUpdateItemCount)
		buildPower();
}

//
//	더미 처리
BOOL
cACTOR::operateDummy()
{
	cACTOR	*lpLord				=	m_lpField->getActor(m_wDummyLord);
	BOOL	bIsIncorrectDummy	=	FALSE;

	if	(!lpLord	||	lpLord->m_dwNameHashCode	!=	m_dwDummyLordNameHashCode)
		bIsIncorrectDummy	=	TRUE;

	if	(g_game.m_bIsChangeSecond)
		if	(m_wDummyTime	&&	isDeath()	==	FALSE)
		{
			m_wDummyTime--;

			if	(bIsIncorrectDummy)
				m_wDummyTime	=	0;

			if	(m_wDummyTime	==	0	)
				bookingDeath(NULL);

			if	(bIsIncorrectDummy)
				m_iDeathCount	=	1;
		}

	if	(m_iDeathCount		)
	{
		m_iDeathCount--;

		if	(m_iDeathCount	==	0)
		{
			m_lpField->addBookedRemoveActor(m_wSerialInField);

			return FALSE;
		}
	}

	return	TRUE;
}

//
//	더미 처리
BOOL
cACTOR::operateCopyActor()
{
	if	(getCopyActorLord()	==	NULL)
	{
		m_wCopyTime	=	0;
		m_lpField->addBookedRemoveActor(m_wSerialInField);

		return	FALSE;
	}

	if	(g_game.m_bIsChangeSecond)
		if	(m_wCopyTime)
		{
			m_wTargetedTime		=	2;
			m_wCopyTime--;

			if	(m_wCopyTime	==	0)
				bookingDeath(NULL);
		}

	if	(m_iDeathCount		)
	{
		m_iDeathCount--;

		if	(m_iDeathCount	==	0)
		{
			m_lpField->addBookedRemoveActor(m_wSerialInField);

			return FALSE;
		}
		
		return	FALSE;
	}

	if	(updateStatusByFrameForMonster()	)
		return TRUE;

	operateAi();

	return	TRUE;
}


//
//	프레임에 의한 시간 업데이트
inline	BOOL
cACTOR::updateStatusByFrameForPlayer()
{
	if	(m_wUpdateOuterActorInfoPeriod	)
		m_wUpdateOuterActorInfoPeriod--;

	if	(m_wChargeTime					)
		m_wChargeTime--;

	if	(m_wBlockedPosValue)
		m_wBlockedPosValue--;

	if	(m_wBlockToTransformationTime)
		m_wBlockToTransformationTime--;

	mDS.m_aiExValue[0]	=	1;
	mDS.m_aiExValue[1]	=	0;
	mDS.m_aiExValue[2]	=	0;
	mDS.m_aiExValue[3]	=	0;
	
	updateEnchantedMagic();
	operateRecoverHP();	//	HP 회복
	reduceCP();
	iMiniPet()->operateMiniPetChargeSkill();
	
	m_stunResistance.decreaseTime();

	if	(m_moveSpeed.decreaseTime())	
	{
		setMoveAbility(m_isRunning);
		sendMoveState();//	이동 속도 보정
	}

	if	(m_wInvincibleTime && m_wInvincibleTime	!=	0xffff	)
		m_wInvincibleTime--;	//	무적 상태
	if	(m_wSealActionTime									)
		m_wSealActionTime--;
	if	(m_wSealRunningTime									)
		m_wSealRunningTime--;
	
	if	(m_wStunTime				)
		m_wStunTime--;
	if	(m_wCurseAmplificationTime	)
		m_wCurseAmplificationTime--;	//	저주 증폭 상태
	if	(m_wFreezeTime				)
		m_wFreezeTime--;				//	프리즈 상태 남은 시간
	if	(m_wBlockingRigidityTime	)
		m_wBlockingRigidityTime--;
	if	(m_wHitActionTime			)
		m_wHitActionTime--;
	if	(m_wRemainActionTime		)
	{
		setExclusiveAction(TRUE);
		m_wRemainActionTime--;

		if	(m_wRemainActionTime	==	0	&&	!m_isAction)
		{
			stopAction();
		}
	}

	if	(m_wStunTime+m_wBlockingRigidityTime+m_wHitActionTime+m_wStoneTime+m_wSleepTime+m_wCaughtTime)
	{
		setExclusiveAction(TRUE);
		return	TRUE;
	}

	return	FALSE;
}

//
//	업데이트(죽었을때만 FALSE를 리턴한다.)
void
cACTOR::updatePlayer()
{
	mDS_P.m_iStep	=	0;

	if	(m_dwBookedBasicAttackTime	&&	g_dwCurrentTime+300	>=	m_dwBookedBasicAttackTime	&&	m_wStunTime	==	0)
	{
//		m_wStunTime	=	16*5;
//		m_dwBookedBasicAttackTime	=	0;
	}
	
	if	(isDummy())
	{
		mDS_P.m_iStep	=	11;
		operateDummy();

		mDS_P.m_iStep	=	-11;
		return;
	}

	if	(isCopyActor())
	{
		mDS_P.m_iStep	=	16;
		operateCopyActor();

		mDS_P.m_iStep	=	-16;
		return;
	}

	if	(isPet() || isSummonBeast())	//	펫이다.
	{
//		reduceHP(m_iHP);

		mDS_P.m_iStep	=	1;

		updateTamedMonster();

		mDS_P.m_iStep	=	-1;
		return;
	}

	if	(m_lpField->m_bIsSiegeWarfareField	&&	m_wGuildPetType)
		operateGuildPetWork();

	int		iFrameCounter	=	m_lpField->m_iFrameCounter;

	mDS_P.m_iStep	=	2;
	if	(m_wWaitTransGVGResultTime)
		--m_wWaitTransGVGResultTime;
	
	if	(m_wBookedRemoveTime)
	{
		if	(m_wWaitLogoutResultTime)
		{
			m_wWaitLogoutResultTime--;

			if	(m_wWaitLogoutResultTime	==	0)
			{
				CLOG("error","로그 아웃을 위해 날린 저장 결과 1분이 지나도록 안 날라옴 [%s/%s] [%d/%d:%d]",
						m_strId,m_strName,m_iLevel,m_iExperience,m_dwSaveCount);

				mDS_P.m_iStep	=	3;
				bookingToExitGame();

				mDS_P.m_iStep	=	-3;
				return;
			}
		}
		else
		if	(m_wWaitMoveFieldResultTime)
		{
			m_wWaitMoveFieldResultTime--;

			if	(m_wWaitMoveFieldResultTime	==	0)
			{
				CLOG("error","필드 이동을 위해 날린 저장 결과 1분이 지나도록 안 날라옴 [%s/%s] [%d/%d:%d]",
						m_strId,m_strName,m_iLevel,m_iExperience,m_dwSaveCount);

				mDS_P.m_iStep	=	4;
				bookingToExitGame();
				mDS_P.m_iStep	=	-4;
				return;
			}
		}
		else
		{
			m_wBookedRemoveTime--;

			if	(m_wBookedRemoveTime == 0)
			{
				m_wBookedRemoveTime	=	1;
				mDS_P.m_iStep	=	5;
				bookingToExitGame();
			}
		}

		mDS_P.m_iStep	=	-5;
		return;
	}

	mDS_P.m_iStep	=	6;

	int	iUpdateLevel	=	932;

	if	(m_wIsIncorrectFieldRequireReturnToVillage)
	{
		m_wIsIncorrectFieldRequireReturnToVillage	=	FALSE;

		mDS_P.m_iStep	=	7;
		g_pMoveFieldUserManager->booking(this,NULL,m_wLastVillage,0);

		mDS_P.m_iStep	=	-7;
		return;
	}

	if	(m_wBlockSelectSpeechTime)
		m_wBlockSelectSpeechTime--;

	if	(m_wPrincessWeaponPrincess	!=	0xfffF)
	{
		cACTOR	*lpPrincess	=	m_lpField->getActor(m_wPrincessWeaponPrincess);

		if	(!lpPrincess	||	lpPrincess->m_dwNameHashCode	!=	m_dwPrincessWeaponPrincessNameHashCode)
			removePrincessWeapon();
	}

	if	(m_wTransToWeaponTime)
	{
		cACTOR	*lpWeaponReceiver	=	m_lpField->getActor(m_wPrincessWeaponReceiver);

		if	(!lpWeaponReceiver	||	lpWeaponReceiver->m_dwNameHashCode != m_dwPrincessWeaponReceiverNameHashCode || lpWeaponReceiver->isDeath()	||
			getPartySerial()	!=	lpWeaponReceiver->getPartySerial())
			releasePrincessWeaponStatus();
		else
		{
			if	(lpWeaponReceiver->m_pos.x	!=	0xffff)
			{
				if	(lpWeaponReceiver->m_pos.x	!=	m_pos.x	||	lpWeaponReceiver->m_pos.y	!=	m_pos.y)
				{
					setActorPos(lpWeaponReceiver->m_pos.x,lpWeaponReceiver->m_pos.y,TRUE,TRUE);

					if	(g_dwCurrentTime	>	m_dwLastTimeForReceiveMoveMessage+1000)
					{
						m_dwLastTimeForReceiveMoveMessage	=	g_dwCurrentTime;

						SG_STOP		packet;

						packet.base.set(sizeof(SG_STOP),dSG_STOP);

						packet.bf12Serial				=	m_wSerialInField;
						packet.bf3Direct				=	m_wDirect;		//	이동 속도 보정
						packet.wPosX					=	m_pos.x;
						packet.wPosY					=	m_pos.y;	//	현재 위치
						packet.bf1IsChangeOwnerDirect	=	FALSE;

						g_game.deliverPartyPacket(this,&packet,m_wCurrentField);
					}
				}
			}
		}
	}

	if	(m_wTransToFatGirlTime)
		operateFatGirlStatus();

	if(m_wAttackDoppelgangerCount)
		operateAttackDoppelganger();

	if	(m_wCharmedTime)
	{
		if	(isPlayer())
			m_wCharmedTime	=	0;
		else
		{
			mDS_P.m_iStep	=	8;

			updateMonster();

			mDS_P.m_iStep	=	-8;
			return;
		}
	}

	if	(m_wLordActor	!=	0xffff)
	{
		mDS_P.m_iStep	=	9;

		updateSlave();

		mDS_P.m_iStep	=	-9;
		return;
	}
	
	if	(isMonster())
	{
		updateMonster();
		return;
	}

	mDS_P.m_iStep	=	10;

	if	(!m_bIsMoving)
		m_lpField->addStandActor(m_wSerialInField);
	else
	if	(g_iCurrentMoveNumber ==	m_iMoveNumber)
	{
		m_wStandTime	=	0;

		updateMove();
		if(isPlayer() && m_wFleeTime)
			sendEtcWork(eEW_CHANGE_FLEE,m_wFleeTime, m_posDest.x, m_posDest.y);	
	}

	mDS_P.m_iStep		=	12;

	CClient	*lpClient	=	getClient();

	mDS_P.m_iStep		=	13;

	if	(lpClient)
	{
		int	iResult	=	checkData(lpClient,"cACTOR::updatePlayer",TRUE);

		if	(iResult	!=	eCADR_ISOK)
		{
			mDS_P.m_iStep		=	14;

			disconnect();

			mDS_P.m_iStep		=	-14;

			return;
		}
	}
	else
	{
		if	(!m_wBookedRemoveTime)
		{
			mDS_P.m_iStep		=	15;
			printf("----------- cACTOR::updatePlayer - invalid client serial!![%d][%s/%s]\n",m_iClientSerial,m_strId,m_strName);

			bookingReadyToExitGame();
		}

		mDS_P.m_iStep		=	-15;
		return;
	}

	if	(m_iDeathCount		)
		return;//	가사상태
	
	mDS_P.m_iStep		=	16;
	operateReloadBeltSlot();

//	CP가 마이너스가 되면 변신이 풀린다.
	{
		if	(m_wBiteTarget	==	0xffff && m_wBeastBerserkerTime == 0	&&	m_wBlockToTransformationTime	==	0)
			if	(m_isExclusiveAction == FALSE && m_wRemainActionTime == 0 && m_iCP <	getReleaseTrasformationCp())
				if (m_wJob==dJOB_WEREWOLF||m_wJob==dJOB_FALLEN_ANGEL||m_wJob==dJOB_MAGICAL_GIRL||m_wJob==dJOB_DEVIL	)
				{
					m_wFreezeTime	=	0;
					sendTransformation(eTR_LOW_CP);
				}
	}

//	m_iCP	=	-100;

//	이동 관련 업데이트
	{
		m_wStandTime++;

		if (m_bIsMoving)	m_wStandTime	=	0;
	}

	mDS_P.m_iStep		=	17;
	if	(updateStatusByFrameForPlayer())
	{
		mDS_P.m_iStep	=	-17;
		return;
	}

	if (m_lpTarget)
	{
		if (m_iTargetZoneSerial	!=	m_lpTarget->m_iZoneSerial	||	m_lpTarget->m_wSerialInField	==	0xffff)
		{
			mDS_P.m_iStep		=	18;
			stop();
			setTarget(NULL);
		}
		else
		if (m_iTargetZoneSerial == 0xffffffff)
			m_iTargetZoneSerial	=	m_lpTarget->m_iZoneSerial;
	}
	else
		m_iTargetZoneSerial		=	0xffffffff;

	mDS_P.m_iStep		=	19;

	if (m_wBiteTarget	!=	0xffff)
	{
		cACTOR	*lpTarget	=	m_lpField->getActor(m_wBiteTarget);

		if	(!lpTarget	||	lpTarget->m_wBiter	!=	m_wSerialInField)
			m_wBiteTarget	=	0xffff;

		return;
	}
	
	mDS_P.m_iStep		=	20;

	operatePlayerBattle();		//	전투 업데이트

	mDS_P.m_iStep		=	21;

	if (m_isAction				)
	{
		mDS_P.m_iStep		=	22;
		updateAction();	//	액션을 하는중.
	}

	mDS_P.m_iStep		=	-1000000;
}

BOOL
cACTOR::addHostileEnemy(int _iSerial)
{
	for (int i=0;i<m_wHostileEnemyCount;i++)
	{
		if (m_awHostileEnemy[i]	==	_iSerial)
			return TRUE;
	}

	if (m_wHostileEnemyCount	>=	dMAX_HOSTILE_ENEMY_COUNT)
	{
		cACTOR	*lpEnemy;

		int		iEnemy	=	m_awHostileEnemy[0],iEnemySlot	=	0;

		for (int i=0;i<m_wHostileEnemyCount;i++)
		{
			lpEnemy	=	m_lpField->getTestedActor(m_awHostileEnemy[i]);

			if (!lpEnemy)
				continue;

			if (lpEnemy->m_wAiState	!=	eAS_FIGHT)
			{
				iEnemySlot	=	i;
				break;
			}
		}

		iEnemy	=	m_awHostileEnemy[iEnemySlot];
		lpEnemy	=	m_lpField->getTestedActor(iEnemy);

		if	(lpEnemy && lpEnemy->m_wCaughtTime == 0)
			lpEnemy->stopBattleForNPC(TRUE);

		memcpy(m_awHostileEnemy,m_awHostileEnemy+1,sizeof(m_awHostileEnemy)-sizeof(m_awHostileEnemy[0]));

		m_wHostileEnemyCount--;
	}

	m_awHostileEnemy[m_wHostileEnemyCount]	=	_iSerial;

	m_wHostileEnemyCount++;

	return	TRUE;
}

void
cACTOR::updateHostileEnemy()
{
	int	i;

	for (i=0;i<m_wHostileEnemyCount;)
	{
		cACTOR	*lpEnemy	=	m_lpField->getTestedActor(m_awHostileEnemy[i]);

		if (!lpEnemy || !lpEnemy->m_lpTarget || lpEnemy->m_lpTarget->m_wSerialInField != m_wSerialInField)
		{
			m_awHostileEnemy[i]	=	0xffff;

			if (i != m_wHostileEnemyCount-1)
				memcpy(m_awHostileEnemy,m_awHostileEnemy+1,sizeof(m_awHostileEnemy[0])*(m_wHostileEnemyCount-1-i));

			m_wHostileEnemyCount--;

			m_awHostileEnemy[m_wHostileEnemyCount]	=	0xffff;
		}
		else
			i++;
	}
}

void
cACTOR::updateLastActionTime()
{
	m_dwLastActionTime				=	g_dwCurrentTime;

	if (m_wRestraintTimeByLogout	!=	0xffff)
		sendEtcWork(eEW_CANCEL_LOGOUT_TIMER_BY_ACTION);

	m_wRestraintTimeByLogout	=	0xffff;
}

BOOL
cACTOR::isIdlePlayer()
{
	if	(m_wTransToWeaponTime)
		return	FALSE;
	
	if	(m_dwLastActionTime+1000*60*3	<	g_dwCurrentTime)
		return	TRUE;

	return	FALSE;
}

void
cACTOR::buyCarrotShopItem(int _iPremiumItemIndex,int _iItemCount,int _iRemainCarrotCount,int _iPrice)
{
	SG_BUY_CARROT_SHOP_ITEM_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_BUY_CARROT_SHOP_ITEM_RESULT);

	//	아이템 생성
	cItem	item;

	g_im.generatePremiumItem(&item,_iPremiumItemIndex);

	item.m_bCount	=	(BYTE)_iItemCount;

	switch(g_iCarrotShopType)
	{
		case	eCarrotShop_AllItemToInventory	:
		case	eCarrotShop_IDPublicItemToCart	:
		{
			if	(item.getBasicItem()->m_wIsIDPublicItem	==	FALSE)	//	계정 공유 아이템이 아니라면 바로 인벤에 넣는다.
			{
				cItem	*lpItemSlot	=	getValidInventorySlot();

				if	(lpItemSlot)
					lpItemSlot->copy(&item);
				else
					memcpy(&m_itemFailedPremiumItem,&item,sizeof(cItem));
				
				buildPower();

				sendCreatePremiumItemLog(&item,eCPIM_BUY_BY_CARROT,_iPrice);
				immediatelySendSaveDataToDBMemory();
			}
			break;
		}

		case	eCarrotShop_AllItemToCart	:
			break;
	}

	_iPrice			=	m_iCarrotCount-_iRemainCarrotCount;
	m_iCarrotCount	=	_iRemainCarrotCount;

	sendBuyCarrotShopItemResult(eBCIR_SUCCESS,m_iCarrotCount,&item);

	if	(m_itemFailedPremiumItem.m_wBaseItem	!=	0xffff)
		sendCartMessage(eCM_EXIST_FAILED_PREMIUM_ITEM);
}

void
cACTOR::askBuyCarrotShopItem(int _iBuyCategory,int _iBuyItemIndex)
{
	if	(m_itemFailedPremiumItem.m_wBaseItem	!=	0xffff)
	{
		cItem	*lpInventorySlot	=	getValidInventorySlot();

		if	(!lpInventorySlot)
		{
			sendCartMessage(eCM_EXIST_FAILED_PREMIUM_ITEM);

			return;
		}

		int		iIndex				=	getValidInventorySlotIndex();

		memcpy(lpInventorySlot,&m_itemFailedPremiumItem,sizeof(cItem));
		m_itemFailedPremiumItem.reset();

		sendLoadFailedCartPremiumItem(lpInventorySlot,iIndex);
	}

	cItem		item;

	if	(_iBuyCategory	>=	g_carrotShopInfo.m_wCategoryCount)
	{
		sendBuyCarrotShopItemResult(eBCIR_FAILED,m_iCarrotCount);
		
		return;
	}

	cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShopInfo.m_aData[_iBuyCategory];

	if	(_iBuyItemIndex	>=	lpCategory->m_wItemCount)
	{
		sendBuyCarrotShopItemResult(eBCIR_FAILED,m_iCarrotCount);
		
		return;
	}

 	if	(lpCategory->m_bf1IsTestCategory && g_bIsTestServer == FALSE)	//	테스트 카테고리에 아이템을 사? 이런 해커!!
 		return;
	
	cCarrotShopItemDefine			*lpCarrotItem=	&lpCategory->m_aItemList[_iBuyItemIndex];

	int		iPrice	=	lpCarrotItem->m_wCarrotCount;

	if	(iPrice	>=	0xffff)
	{
		sendBuyCarrotShopItemResult(eBCIR_FAILED,m_iCarrotCount);

		return;
	}

 	if	(lpCarrotItem->m_bf1IsTestItem && g_bIsTestServer == FALSE)	//	테스트 판매 아이템을 사? 이런 해커!!
 		return;

	cItem	*lpItemSlot	=	getValidInventorySlot();

	if	(lpItemSlot		==	NULL)
	{
		sendBuyCarrotShopItemResult(eBCIR_FULL_INVENTORY,m_iCarrotCount);

		return;
	}

	if	(!g_im.generatePremiumItem(&item,lpCarrotItem->m_wPremiunItemIndex))
	{
		sendBuyCarrotShopItemResult(eBCIR_FAILED,m_iCarrotCount);

		return;
	}

	cBasicItem	*lpBasicItem	=	item.getBasicItem();

	if	(!lpBasicItem)
	{
		sendBuyCarrotShopItemResult(eBCIR_FAILED,m_iCarrotCount);

		return;
	}
		
	if (lpBasicItem->m_attr.isCanNotOwnSameItem)
	{
		if (getItemByBasicItem(item.m_wBaseItem))
		{
			sendCartMessage(eCM_ONLY_ONE_ITEM_IN_INVENTORY);
			return;
		}
	}
	if (lpBasicItem->m_attr.isBadge)
	{
		if (getBadgeCount()	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
		{
			sendCartMessage(eCM_TOO_MANY_BADGE_ITEM);
			return;
		}

	}
	if	(lpBasicItem->isCosmicPower())
	{
		if	(getInventoryCosmicPowerUpgradeChanceMax())
		{	// 인벤토리에 코스믹아이템 강화 찬스를 체크하여 찬스가 없으면 .. 코스믹아이템이 없는거다...+_+; 왜냐면 코스믹아이템에 확률이 붙어있다는 가정하에.. 체크하는거기때문에!+_+;
			sendCartMessage(eCM_ONLY_ONE_ITEM_IN_INVENTORY);
			
			sendBuyCarrotShopItemResult(eBCIR_FAILED,m_iCarrotCount);
			return ;	 	
		}
	}

	int	iExpandInventorySize	=	item.getExpandInventorySize();

	if	(iExpandInventorySize)
	{
		if	(m_bf6ExtraInventorySize	+	iExpandInventorySize	>	dEXTRA_INVENTORY_SIZE)
		{
			sendCartMessage(eCM_CAN_NOT_WITHDRAW_ITEM_BY_EXTRA_BAG_SIZE_FULL);

			return;
		}
	}

	m_wIsWaitCartMessage				=	TRUE;
	m_wWaitBuyCarrotShopItemResultTime	=	dSYNC_FPS*60;

	BOOL	bIsIDPublicItem				=	FALSE;

	switch(g_iCarrotShopType)
	{
		case	eCarrotShop_AllItemToInventory	:
			//2011-05-22 줄珂혼딜뭔찜돕뭔膠났掘齡
			bIsIDPublicItem	=	lpBasicItem->m_wIsIDPublicItem;
//			if	(lpBasicItem->m_wIsIDPublicItem)
//			{
//				sendBuyCarrotShopItemResult(eBCIR_FAILED,m_iCarrotCount);
//				return;
//			}
			break;

		case	eCarrotShop_IDPublicItemToCart	:
			bIsIDPublicItem	=	lpBasicItem->m_wIsIDPublicItem;
			break;

		case	eCarrotShop_AllItemToCart		:
			bIsIDPublicItem	=	TRUE;
			break;
	}

	g_game.sendUseCarrotLog(this,lpCarrotItem->m_wPremiunItemIndex,lpCarrotItem->m_bf8ItemCount,iPrice,bIsIDPublicItem);
}

void
cACTOR::setPeneltyTime(int _iTime,int _iReason,int _iValue)
{
	m_dwPeneltyTime		=	max(_iTime,(int)m_dwPeneltyTime);

	SG_PENELTY_TIME		packet;

	packet.base.set(sizeof(packet),dSG_PENELTY_TIME);
	packet.dwPeneltyTime	=	m_dwPeneltyTime;

	if	(_iTime	>=	60)
		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	if	(_iReason	!=	-1)
		sendHackingLog(_iReason,_iValue);
}

BOOL
cACTOR::tryToSecretDungeon()
{
	if	(!m_lpField)
		return	FALSE;

	int	iLinkSecretDungeon	=	m_lpField->m_wLinkSecretDungeon,i;

	if	(iLinkSecretDungeon	==	0	||	iLinkSecretDungeon	==	0xffff)
		return	FALSE;

	cParty	*lpParty=	getParty();

	if	(!lpParty	||	lpParty->isOnField(-1,m_wCurrentField)	==	FALSE)
	{
		sendRegistServerMessage(eRSM_SECRET_DUNGEN_ERROR_GATHER_PARTY_MEMBER_IN_SAME_FIELD);

		return	FALSE;
	}

	for	(i=0;i<lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);

		if	(!lpActor)
		{
			sendRegistServerMessage(eRSM_SECRET_DUNGEN_ERROR_GATHER_PARTY_MEMBER_IN_SAME_FIELD);

			return	FALSE;
		}

		if	(lpActor->isTester()	==	FALSE)
			if	(lpActor->m_bf4SecretDungeonPlayCount >=	dENTER_SECRET_DUNGEON_LIMIT_PER_ONE_DAY+lpActor->m_wCorrectSecretDungeonPlayCount)
			{
				sendRegistServerMessage(eRSM_EXIST_COUNT_OVER_MEMBER_TO_TRY_SECRET_DUNGEN);

				return	FALSE;
			}
	}

	cSecretDungeon	*lpDungeon	=	g_pSdManager->get(iLinkSecretDungeon);

	if	(!lpDungeon)
		return	FALSE;

	if	(m_lpField->m_pArea->m_iSecretDungeonExit	==	-1)
	{
		cMSG::Error("에러!!","비밀던젼 출구가 설정되어 있지 않다!!\n\n%s",m_lpField->m_strFileName);
		return	FALSE;
	}

	if	(lpParty->checkMemberLevel(lpDungeon->m_wMinLevel,lpDungeon->m_wMaxLevel)	==	FALSE)
	{
		sendEtcWork(eEW_FAILED_TO_CREATE_SECRET_DUNGEON_BY_LIMIT_LEVEL,lpDungeon->m_wMinLevel,lpDungeon->m_wMaxLevel);

		return	FALSE;
	}

	if	(lpDungeon->m_bf1IsTestDungeon	&&	g_bIsTestServer	==	FALSE)
	{
		sendEtcWork(eEW_FAILED_TO_ENTER_SECRET_DUNGEON_BY_NOT_EXIST_KEY,iLinkSecretDungeon);

		return	FALSE;
	}

	int		iIFSerial=	g_pIfManager->m_iRookie;
	int		iResult	=	g_pIfManager->create(iLinkSecretDungeon,lpParty,m_lpField->m_iUniqueSerial,m_lpField->m_pArea->m_iSecretDungeonExit);

	SG_REGIST_MESSAGE	packet;

	packet.base.set(sizeof(packet),dSG_REGIST_MESSAGE);
	packet.wType	=	eSM_SYSTEM;

	if	(iResult	==	eCSDR_FULL)
	{
		packet.wMessage	=	eRSM_TO_MANY_PARTY_TO_TRY_SECRET_DUNGEN;
		lpParty->sendPacketToAllMember(&packet,packet.base.wSize);

		return	FALSE;
	}

	if	(iResult	==	eCSDR_ALREADY_USE_SECRET_DUNGEON	||	iResult	!=	eCSDR_SUCCESS)
	{
		packet.wMessage	=	eRSM_TO_MANY_PARTY_TO_TRY_SECRET_DUNGEN;
		
		lpParty->sendPacketToAllMember(&packet,packet.base.wSize);

		return	FALSE;
	}

	for	(i=0;i<lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);
		
		if	(!lpMember)
			continue;
		
		lpMember->enterToTheInstanceField(iIFSerial,0,-1,-1);
	}

	return	TRUE;
}

int
cACTOR::enterToTheInstanceField(int _iIFSerial,int _iFloor,int _iX,int _iY,BOOL _bisUnCountSecretDungeonPlayTime)
{
	CClient	*lpClient			=	getClient();

	if	(!lpClient	||	isDummy())
		return	eESDE_INCORRECT_CLIENT;

	cInstanceField	*lpIF		=	g_pIfManager->get(_iIFSerial);
	cSecretDungeon	*lpSD		=	g_pSdManager->get(lpIF->m_wSecretDungeonSerial);

	if (lpIF->m_wIsReadyToClose)
	{
		sendRegistServerMessage(eRSM_THAT_IS_CLOSE_SECRET_DUNGEON,eSM_SYSTEM);
		
		return	eESDE_CLOSE_DUNGEON;
	}

	BOOL	bIsFirstEnter;

	if	(lpIF->addEnterPlayerLog(m_strName,&bIsFirstEnter)	==	0xffff)
	{
		sendRegistServerMessage(eRSM_TOO_MANY_PLAYER_ENTER_SECRET_DUNGEON,eSM_SYSTEM);

		return	eESDE_TOO_MANY_PLAYER_COUNT;
	}

	if	(bIsFirstEnter	&&	lpSD->m_bf1IsTowerOfOrdeal	==	FALSE)
	{
		if	(isTester()	==	FALSE && _bisUnCountSecretDungeonPlayTime == FALSE)
			if	(m_bf4SecretDungeonPlayCount	>=	dENTER_SECRET_DUNGEON_LIMIT_PER_ONE_DAY+m_wCorrectSecretDungeonPlayCount)
			{
				lpIF->removeEnterPlayerLog(m_strName);

				sendRegistServerMessage(eRSM_OVER_THE_SECRET_DUNGEON_PLAY_COUNT_PER_DAY,eSM_SYSTEM);
				
				return	eESDE_CLOSE_DUNGEON;
			}

		if	(m_iLevel	<	lpSD->m_wMinLevel	||	m_iLevel	>	lpSD->m_wMaxLevel)
		{
			lpIF->removeEnterPlayerLog(m_strName);

			sendEtcWork(eEW_FAILED_TO_ENTER_SECRET_DUNGEON_BY_LIMIT_LEVEL,lpSD->m_wMinLevel,lpSD->m_wMaxLevel);

			return	eESDE_INCORRECT_CLIENT;
		}

		if	(m_bf4SecretDungeonPlayCount < 15)
		{
			if	(isTester()	==	FALSE && _bisUnCountSecretDungeonPlayTime == FALSE)
				m_bf4SecretDungeonPlayCount++;

			if	(getPartySerial()	!=	0xffff)
				g_pPartyManager->sendWPChangeMemberInfo(getPartySerial(),m_strName,m_iLevel,m_bf4SecretDungeonPlayCount,m_wJob,m_bf3GuildDungeonPlayTime);
		}
	}


	m_wIsMoveToDifferentServer	=	FALSE;

	lpClient->m_wReceivePacketSerial	=	0;

	m_wCurrentField				=	dINSTANCE_FIELD_FLAG|m_wCurrentField;
	m_bf10ESD_Time				=	lpIF->m_wCreateTimeValue;
	m_bf8InstanceField			=	_iIFSerial;
	m_bf4InstanceFieldFloor		=	_iFloor;
	m_bIsReadyToExit			=	TRUE;

	m_pos.x						=	_iX;
	m_pos.y						=	_iY;
	m_iXPos						=	_iX;
	m_iYPos						=	_iY;

	if	(!readyToMoveField(m_wCurrentField,_iIFSerial,_iFloor))
		return	eESDE_DATA_SAVE_FAILED;	//	필드 이동 준비를 한다.

	readyToRemoveActorWork();
	g_game.sendWPUpkeepPlayerData(this);		//	필드를 이동해도 기억해야 할 데이터 전송

	sendMoveFieldResult(dMOVEMAP_RESULT_MOVE_TO_IF,"",lpSD->m_strIDCode,_iFloor);

	return	eESDE_SUCCESS;
}

void
cACTOR::leaveInstanceField(int _iGateField)
{
	CClient	*lpClient			=	getClient();

	if	(!lpClient	||	isPlayer()	==	FALSE)
		return;

	m_wIsMoveToDifferentServer	=	FALSE;

	m_wCurrentField				=	_iGateField;
	m_bf8InstanceField			=	0;
	m_bf4InstanceFieldFloor		=	0;
	m_bIsReadyToExit			=	TRUE;
	
	lpClient->m_wReceivePacketSerial	=	0;

	m_pos.x						=	-1;
	m_pos.y						=	-1;
	m_iXPos						=	-1;
	m_iYPos						=	-1;

	if	(!readyToMoveField(m_wCurrentField,0,0))
		return;	//	필드 이동 준비를 한다.

	cFIELD	*lpField			=	g_game.getFieldByUniqueSerial(_iGateField);

	if	(!lpField)
		return;

	readyToRemoveActorWork();
	g_game.sendWPUpkeepPlayerData(this);		//	필드를 이동해도 기억해야 할 데이터 전송

	sendMoveFieldResult(dMOVEMAP_RESULT_SUCCESS,"",lpField->m_strFileName,0);
}

void
cACTOR::operateUpdateByDayWork(BOOL _bIsRequireSendInfo)
{
	if	(m_bf5LastUpdateDay	==	g_currentTime.m_wDay)
		return;

	BOOL	bIsChanged		=	TRUE;

	if	(m_bf5LastUpdateDay	>	g_currentTime.m_wDay)
	{
		bIsChanged	=	FALSE;

		if	(g_currentTime.m_wDay == 1)
		{
			if	(g_currentTime.m_wMonth	==	3)	//	2월 말일 에서 3월 1일로 변경 된거면 정상
			{
				if	(m_bf5LastUpdateDay ==	28 || m_bf5LastUpdateDay ==	29)
					bIsChanged	=	TRUE;
			}
			else	//	그외의 달 말일에서 다음달 초로 변경된거
			{
				if	(m_bf5LastUpdateDay ==	30 || m_bf5LastUpdateDay ==	31)
					bIsChanged	=	TRUE;
			}
		}

		if	(bIsChanged	==	FALSE)
		{
			int	iGap	=	m_bf5LastUpdateDay-g_currentTime.m_wDay;

			if	(iGap	>	1)
				bIsChanged	=	TRUE;
		}
	}

	if	(!bIsChanged)
		return;

	m_bf5LastUpdateDay	=	g_currentTime.m_wDay;

	m_bf3UseBoostCount	=	0;
	m_bf3EventCounter	=	0;

	m_iTodayWithdrawGoldInGuild		=	0;
	memset(m_abTodayWithdrawItemCount,0,sizeof(m_abTodayWithdrawItemCount));

	if	(m_bf4SecretDungeonPlayCount)
	{
		m_bf4SecretDungeonPlayCount	=	0;

		if	(getPartySerial()	!=	0xffff)
			g_pPartyManager->sendWPChangeMemberInfo(getPartySerial(),m_strName,m_iLevel,m_bf4SecretDungeonPlayCount,m_wJob,m_bf3GuildDungeonPlayTime);
	}

	if	(m_bf3GuildDungeonPlayTime)
	{
		m_bf3GuildDungeonPlayTime	=	0;
		
		if	(getPartySerial()	!=	0xffff)
			g_pPartyManager->sendWPChangeMemberInfo(getPartySerial(),m_strName,m_iLevel,m_bf4SecretDungeonPlayCount,m_wJob,m_bf3GuildDungeonPlayTime);
	}

	for (int i=0;i<dOWN_PET_COUNT;i++)
	{
		cPetDefine	*lpPet	=	getPet(i);

		if	(lpPet)
			lpPet->m_bf5BoostCount	=	0;
	}

	if	(_bIsRequireSendInfo)
		sendUpdateBoostCount();
}


void
cACTOR::checkChangeGold()
{
	LONGLONG	llOwnGold	=	getOwnGoldSum();

	if	(m_llLastGold	==	llOwnGold && m_llGettingGold == 0 && m_llSpentGold == 0)
		return;

	sendGoldLog(llOwnGold);
}
