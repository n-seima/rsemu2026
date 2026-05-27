#include "cFIELD.H"
#include "cGAME.H"
#include "Client.H"
#include "CGuild.H"
#include "arca.H"
#include "secretDungeon.H"
#include "cEvent.H"
#include "packetManager.H"
#include "ast_fire_wall.H"

//
//	첫만남 데이터 전송.. *-_-*
BOOL
cFIELD::sendFirstMeetingData(cACTOR *_lpActor)
{	
	if	(_lpActor->m_iRookieTime	==	1)
	{
		CClient	*lpClient							=	_lpActor->getClient();

		if	(lpClient)
			_lpActor->m_wOperatorLevel				=	lpClient->GetIsOper();
		else
			return	TRUE;
		
//	정상적인 OX퀴즈 필드 참가인지 체크
		if	(m_wIsOXQuizField && !_lpActor->isOperator())
		{
			if	(m_oxQuizStatus.m_wStatus	!=	eOIPS_READY || !_lpActor->isOwnedOXQuizTicket())
			{
				_lpActor->m_wIsIncorrectFieldRequireReturnToVillage	=	TRUE;

				if	(m_oxQuizStatus.m_wStatus==	eOIPS_SLEEP)
					_lpActor->m_bf1IsMoveByNotOXQuizTime	=	TRUE;
				else
					_lpActor->m_bf1IsMoveByOXQuizProcess	=	TRUE;
			}
		}

		if	(m_wIsWordQuizField && !_lpActor->isOperator())
		{
			if	(m_wordQuizStatus.m_wStatus	!=	eWQPS_READY)
			{
				_lpActor->m_wIsIncorrectFieldRequireReturnToVillage	=	TRUE;

				if	(m_oxQuizStatus.m_wStatus==	eWQPS_SLEEP)
					_lpActor->m_bf1IsMoveByNotOXQuizTime	=	TRUE;
				else
					_lpActor->m_bf1IsMoveByOXQuizProcess	=	TRUE;
			}
		}

//	정상적인 길드전 참가인지 체크
		if	(_lpActor->m_wIsIncorrectGuildBattleFieldJoin)
		{
			_lpActor->m_wIsIncorrectFieldRequireReturnToVillage	=	TRUE;
			_lpActor->m_bf1IsMoveByNotGuildBattleTime			=	TRUE;
		}

//	게임 참가 결과
		{
			SG_RESULT_JOIN	packetJoinResult;

			packetJoinResult.base.set(sizeof(SG_RESULT_JOIN),dSG_RESULT_JOIN);
			packetJoinResult.wResult				=	dRESULT_JOIN_FIELD_SUCCESS;
			packetJoinResult.wSerialInField			=	_lpActor->m_wSerialInField;
			packetJoinResult.dwEnchantedMask		=	_lpActor->m_dwEnchantedMask;
			packetJoinResult.bf1IsGuildBattleField	=	m_bIsGuildBattleField;
			packetJoinResult.bf1IsSiegeWarfareField=	m_bIsSiegeWarfareField;
			packetJoinResult.bf1IsGuildPointBattleField	=	m_bIsGuildPointBattleField;
			packetJoinResult.bf1IsBossZone			=	m_wIsBossZone;
			packetJoinResult.bf1IsOXQuizField		=	m_wIsOXQuizField;
			packetJoinResult.bf1IsVillage			=	m_wIsVillage;
			packetJoinResult.bf3Team				=	(BYTE)_lpActor->m_wTeam;
			packetJoinResult.bf4MinimapDisplayLevel	=	m_wMinimapDisplayLevel;
			packetJoinResult.bf3OperatorLevel		=	_lpActor->m_wOperatorLevel;
			packetJoinResult.bf12DeathPeneltySecond	=	_lpActor->m_bf12DeathPeneltySecond;
			packetJoinResult.bf7DeathPenelty		=	_lpActor->getCurrentLevelDeathPeneltyValue();
			packetJoinResult.bf1IsReadyToSelectGuildMark	=	_lpActor->m_bf1IsReadyToSelectGuildMark;
			packetJoinResult.bf1IsWaitOrgAvatarDataResult	=	_lpActor->m_bIsWaitOrgAvatarData;

			packetJoinResult.m_wCurrentField		=	_lpActor->m_wCurrentField;	//	현재 필드
			packetJoinResult.m_iXPos				=	_lpActor->m_iXPos;
			packetJoinResult.m_iYPos				=	_lpActor->m_iYPos;
			packetJoinResult.m_wLastVillage			=	_lpActor->m_wLastVillage;	//	마지막으로 떠나온 마을
			packetJoinResult.bf1IsProgressEvent		=	m_bIsProcessEvent;
			packetJoinResult.m_wRemainSecondForUseGateGlove					=	_lpActor->m_bRemainSecondForUseGateGlove;
			packetJoinResult.m_wBoostExpSecondByTreasureMapEventInServer	=	g_iBoostExpSecondByTreasureMapEventInServer;
			packetJoinResult.m_wBoostGoldSecondByTreasureMapEventInServer	=	g_iBoostGoldSecondByTreasureMapEventInServer;
			packetJoinResult.m_wPlayerBoostExpSecondByTreasureMapEvent		=	_lpActor->m_wBoostExpSecondByTreasureMapEvent;
			packetJoinResult.m_wPlayerBoostGoldSecondByTreasureMapEvent		=	_lpActor->m_wBoostGoldSecondByTreasureMapEvent;
			packetJoinResult.m_wRedStonePoint								=	_lpActor->m_wRedStonePoint;
			packetJoinResult.m_wReversalPowerTime							=	_lpActor->m_wReversalPowerTime;
			packetJoinResult.bf1IsNetCafeUser								=	lpClient->GetIsNetCafeUser();//JBC	08-08-12 넷카페 게이트 이동 사용.
			packetJoinResult.m_bf4GuildHallLevel							=	0;			
			packetJoinResult.m_bf4Weather									=	m_wWeather;			// 기후~~>..

			packetJoinResult.m_bGuildHallShape								=	0;
			packetJoinResult.bf4RemainGuildStategyPointCount				=	m_iRemainStategyPoint;
			packetJoinResult.m_iHeavenRedStoneCount			=	g_iHeavenRedStoneCount;
			packetJoinResult.m_iHellRedStoneCount			=	g_iHellRedStoneCount;
			packetJoinResult.m_iRedDevilRedStoneCount		=	g_iRedDevilRedStoneCount;

			memset(packetJoinResult.m_awRelatedPlaceOfGuild,0xff,sizeof(packetJoinResult.m_awRelatedPlaceOfGuild));
			if(_lpActor->m_wJob == dJOB_SOUL_BRINGER)
			{	// 영술사이고 .. 투사 스킬이 없으면 넣어준다..~!
				if(_lpActor->m_aAbility[25].m_wSkill == 0xffff)
				{
					_lpActor->correctSkill();
				}	
			}
			{
				CGuild	*lpGuild	=	_lpActor->getGuild();

				if	(lpGuild)
				{
					memcpy(packetJoinResult.m_awRelatedPlaceOfGuild,lpGuild->m_awGuildVillage,sizeof(lpGuild->m_awGuildVillage));
					lpGuild->sendGuildMemberList(_lpActor);
				}
			}

			if	(m_wIsGuildHall)
			{
				CGuild	*lpGuild	=	g_guildManager.getBySerial(m_wHallOwnGuild);

				if	(lpGuild)
				{
					packetJoinResult.m_bf4GuildHallLevel	=	(BYTE)lpGuild->m_wHallLevel;
					packetJoinResult.m_bGuildHallShape	=	(BYTE)lpGuild->m_wGuildHallShape;

					if	(m_bIs1LevelGuildHall	||	lpGuild->m_wGuildHallShape	==	0xffff)
						packetJoinResult.m_bGuildHallShape	=	0;
				}
			}

			_lpActor->m_wServerSign											=	largeRandom(65535);
			packetJoinResult.m_wServerSign									=	_lpActor->m_wServerSign;

			if(m_bIsNotUseSkillBeforeTrans)
			{
				if(!_lpActor->m_wTransformationTime)
					_lpActor->reactionAllOff();
			}

			if	(m_wDuelType	!=	0xffff)
				packetJoinResult.bf1IsDuelField	=	TRUE;
			else
				packetJoinResult.bf1IsDuelField	=	FALSE;

			memcpy(&packetJoinResult.upkeepPlayerData	,_lpActor->getUpkeepPlayerDataPointer()	,sizeof(CUpkeepPlayerData));


			packetJoinResult.base.wSize				=	sizeof(packetJoinResult)-sizeof(packetJoinResult.m_awSkillCoolTime);

			for	(int i=0;i<c_iCoolTimeSkillCount;i++)
			{
				if	(_lpActor->m_awSkillCoolTime[i])
				{
					memcpy(packetJoinResult.m_awSkillCoolTime,_lpActor->m_awSkillCoolTime,sizeof(packetJoinResult.m_awSkillCoolTime));
					packetJoinResult.base.wSize				=	sizeof(packetJoinResult);
					break;
				}
			}

			g_userPM.add(_lpActor->m_iClientSerial,&packetJoinResult,packetJoinResult.base.wSize);
		}

		{
			// 이벤트 정보 업데이트
			SG_EVENT_TIME_UPDATE	packet;
			
			packet.base.set(sizeof(SG_EVENT_TIME_UPDATE),dSG_EVENT_TIME_UPDATE);
			
			memcpy(packet.adwEventTimeActiveBuffer,g_eventTimeManager.m_adwEventTimeActiveBuffer, sizeof(packet.adwEventTimeActiveBuffer));
			packet.iCount	=	g_eventTimeManager.m_iCount;
			g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
		}

		{
			cInstanceField	*lpIF	=	getMasterInstanceField();

			if	(lpIF)
				lpIF->sendMessageForChangeFloorActor(_lpActor);
		}

		if	(isVillage())
		{
			_lpActor->m_iHP					=	max(_lpActor->m_iHP,1);
			_lpActor->m_wLastVillage		=	m_iUniqueSerial;	//	마을이다.
		}

		if	(_lpActor->m_dwPeneltyTime)
		{
			int	iRemainTime	=	max(_lpActor->m_dwPeneltyTime/60,1);

			_lpActor->sendRegistServerMessage(eRSM_NOTICE_FOR_INCORRECT_PLAYER_USER);
		}

		checkFieldTrap(_lpActor);

		_lpActor->updateLastActionTime();

		_lpActor->m_iRookieTime								=	2;

		_lpActor->m_bf1IsMoveByEndOfGuildBattle				=	FALSE;
		_lpActor->m_bf1IsMoveByNotGuildBattleTime			=	FALSE;
		_lpActor->m_bf1IsMoveByOXQuizProcess				=	FALSE;
		_lpActor->m_bf1IsMoveByNotOXQuizTime				=	FALSE;
		_lpActor->m_bf1IsMoveByEndOfOXQuiz					=	FALSE;
		_lpActor->m_bf1IsBannedSecretDungeon				=	FALSE;
		_lpActor->m_bf1IsMoveByNotGuildBattleTime			=	FALSE;

		g_game.sendBPUpdatePlayerInfo(_lpActor);

		if	(_lpActor->getPartySerial()	!=	0xffff)
		{
			sendPartyInfo(_lpActor,FALSE);

			cParty	*lpParty	=	g_pPartyManager->get(_lpActor->getPartySerial());

			if (lpParty)
			{
				if (_lpActor->isPartyLeader())
					lpParty->m_wLeaderPlayMinuteOnThisFieldTime	=	0;
				
				g_game.sendWPMoveFieldPartyMember(lpParty->m_strPartyName,_lpActor->m_strName,_lpActor->m_wCurrentField,_lpActor->m_iZoneSerial,FALSE,_lpActor->m_bf8InstanceField,_lpActor->m_bf4InstanceFieldFloor);
			}
		}

		if	(_lpActor->m_iHP	<=	0)
		{
			if	(isVillage())
				_lpActor->m_iHP	=	_lpActor->getMaxHP();
			else
				_lpActor->bookingDeath(NULL);
		}

		_lpActor->checkIncorrectItem(eDIL_LOGIN);
		
		g_game.sendWPConfirmEnterField(_lpActor);

		return	TRUE;
	}

	if	(_lpActor->m_iRookieTime	==	2)
	{
		if	(g_bIsDuelServer)
			g_game.sendWPAskDuelRecord(_lpActor);

		int	i;
		
		if	(m_wIsOXQuizField	+	m_wIsWordQuizField	==	0)
		{
			for (i=0;i<dOWN_PET_COUNT;i++)
			{
				if (_lpActor->m_aPet[i].m_bf10Level	==	0)
					continue;

				bookingGeneratePet(_lpActor->m_strName,i,FALSE);
			}

			for (i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
			{
				cSummonBeastDefine	*lpSummonBeast	=	&_lpActor->m_aSummonBeast[i];

				if	(lpSummonBeast->m_bf10Level	==	0)
					continue;

				lpSummonBeast->m_bf2Grade	=	min(lpSummonBeast->m_bf2Grade,dSUMMON_BEAST_GRADE_COUNT-1);

				bookingGenerateSummonBeast(0,0,_lpActor->m_strName,lpSummonBeast->m_bf2Type,lpSummonBeast->m_bf2Grade,
											lpSummonBeast->m_bf10Level,lpSummonBeast->m_bf22CurrentHP*100,FALSE);
			}
		}

		_lpActor->sendSimpleGuildInfo();	//	아주 간단한 길드 정보 전송

		if	(m_bIsGuildBattleField	&&	(m_wGuildBattleStatus	==	eGB_BATTLE || m_wDuelType != 0xffff))
			addSendGuildBattleInfo(_lpActor->m_wSerialInField);

		_lpActor->m_iRookieTime++;

		_lpActor->addSendVerySimpleInfo(_lpActor);

		return	FALSE;
	}

//	필드에 있는 모든 액터의 시리얼 받기
	if	(_lpActor->m_wReceiveFieldActorData	!=	0xffff		)
	{
		sendMinimumActorInfoList(_lpActor);

		switch(m_wDuelMatchStep)
		{
			case	eDUEL_MATCH_STEP_READY	:
			{
				addSendEtcWork(_lpActor->m_wSerialInField,eEW_OPERATE_DUEL_MATCH,4,m_wReadyForDuelCounter);
				break;
			}
			case	eDUEL_MATCH_STEP_BATTLE	:
			{
				addSendEtcWork(_lpActor->m_wSerialInField,eEW_OPERATE_DUEL_MATCH,0,m_wDuelBattleTimeCounter);
				break;
			}
		}

		return	FALSE;
	}

//	필드에 떨어진 아이템 데이터 받기
	if	(_lpActor->m_wReceiveFieldItemData	!=	0xffff		)
	{
		sendExistFieldItem(_lpActor);

		return	FALSE;
	}

	if	(_lpActor->m_wIsReceivedPitchmanShopList	==	FALSE)
	{
		sendExistPitchmanShopList(_lpActor);

		if	(g_iBoostExpByLeadersSoundofBell	>	100)
		{
			SG_SOUND_OF_LEADERS_BELL	packet;

			packet.base.set(sizeof(packet),dSG_SOUND_OF_LEADERS_BELL);

			packet.iUpkeepSecond			=	g_eventManager.m_iUpkeepSecondForSoundOfLeadersBell;
			packet.wWork					=	eSoundOfLeadersBellWork_move_field;
			packet.iBoostExp				=	g_iBoostExpByLeadersSoundofBell;	//	
			packet.iBoostItemDropChance		=	g_iBoostItemDropChanceByLeadersSoundofBell;
			packet.wIsDisplayNameWithSoundOfLeadersBell	=	g_eventManager.m_wIsDisplayNameWithSoundOfLeadersBell;

			strcpy(packet.strMessage,g_eventManager.m_strSoundOfLeadersBellMessage);
			strcpy(packet.strCaster,g_eventManager.m_strSoundOfLeadersBellOwner);

			g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
		}

		return	FALSE;
	}

	if	(_lpActor->m_wIsReceivedPortalList	==	FALSE)
	{
		sendExistPortalList(_lpActor);
		sendExistTrapList(_lpActor);
		sendDoorsStatus(_lpActor);

		return	FALSE;
	}

	if (_lpActor->m_iRookieTime	==	3)
	{
		_lpActor->m_iRookieTime++;

		if (_lpActor->m_wGuildSerial	!=	0xffff)
		{
			CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpActor->m_wGuildSerial);

			if (!lpGuild)
			{
				_lpActor->m_wGuildSerial=	0xffff;
				_lpActor->m_wGuildRank	=	0;

				_lpActor->sendDissolutionGuildMessage();
			}
			else
			{
				CGuildMan	*lpMember	=	lpGuild->getMember(_lpActor->m_strName);

				if	(!lpMember)
				{
					_lpActor->m_wGuildSerial	=	0xffff;
					_lpActor->m_wGuildRank		=	0;

					SG_EXIT_GUILD	packet;

					packet.base.set(sizeof(packet),dSG_EXIT_GUILD);
					packet.wGuildSerial			=	0xffff;

					g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
				}
				else
				{
					if (_lpActor->m_wGuildRank	!=	lpMember->m_wRank)
					{
						_lpActor->m_wGuildRank	=	lpMember->m_wRank;
						_lpActor->sendChangeRankInGuild(_lpActor->m_wGuildRank);
					}
				}
			}

			return	FALSE;
		}
	}

	if	(_lpActor->m_iRookieTime	==	4)
	{
		sendExistFireWallList(_lpActor);
		sendExistArcaList(_lpActor);
		sendExistBannerList(_lpActor);

		if	(m_wDuelType	!=	0xffff)
			_lpActor->sendItemData();

		_lpActor->checkRedStonePoint();

		_lpActor->m_iRookieTime++;

		return	FALSE;
	}

#ifdef _USE_BROADCAST
	{
		CClient	*lpClient	=	_lpActor->getClient();

		if	(lpClient	&&	lpClient->GetBCSerial()	==	0xffff)
		{
			g_game.sendBPActorInfo(_lpActor);
			_lpActor->sendLogPlayerLogInOut();
		}
	}
#endif

//	다 끝났다.
	_lpActor->m_iRookieTime		=	0;

	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG && _lpActor->m_bIsWaitOrgAvatarData)
	{
		G2W_ASK_GVG_AVATAR_ORG_DATA	packet;

		packet.base.set(sizeof(packet),dG2W_ASK_GVG_AVATAR_ORG_DATA);

		strcpy(packet.strName,_lpActor->m_strName);
		strcpy(packet.strId,_lpActor->m_strId);

		int	iNameLength		=	strlen(packet.strName);

		packet.wOrgServer			=	packet.strName[iNameLength-1]-'A';
		packet.wNotUseUnderBarName	=	_lpActor->m_wProcessChapterIndex;

//		printf("[GVG] %d/%d\n",packet.wOrgServer,packet.wNotUseUnderBarName);

		SEND_WOLRD_PACKET(&packet);
	}

	return	FALSE;
}

void
cFIELD::sendEventTimeUpdate()
{
	SG_EVENT_TIME_UPDATE	packet;
	
	packet.base.set(sizeof(SG_EVENT_TIME_UPDATE),dSG_EVENT_TIME_UPDATE);
	
	memcpy(packet.adwEventTimeActiveBuffer,g_eventTimeManager.m_adwEventTimeActiveBuffer, sizeof(packet.adwEventTimeActiveBuffer));
	packet.iCount	=	g_eventTimeManager.m_iCount;
	sendPacketToAllPlayer(&packet);
}

void
cFIELD::sendDoorsStatus(cACTOR *_lpActor)
{
	SG_DOORS_STATUS_IN_FIELD	packet;

	packet.base.set(sizeof(packet),dSG_DOORS_STATUS_IN_FIELD);
	packet.wDoorCount		=	0;

	for (int i=0;i<m_pArea->m_wDoorCount;i++)
	{
		cAreaDoorInfo		*lpDoor	=	(cAreaDoorInfo	*)m_pArea->getArea(m_pArea->m_awDoorList[i]);
		cFieldDoorStatus	*lpSlot	=	&packet.aDoors[packet.wDoorCount];

		lpSlot->bf10Area	=	lpDoor->m_wSerial;
		lpSlot->bf1IsSleep	=	1-lpDoor->m_bf1IsActive;
		lpSlot->bf1IsHide	=	lpDoor->m_bf1IsHide;
		lpSlot->bf1IsLocked	=	1-lpDoor->m_bf1IsUnlocked;
		lpSlot->bf1IsTrap	=	1-lpDoor->m_bf1IsDisarmed;
		lpSlot->bf1IsClose	=	1-lpDoor->m_bf1IsOpened;
		lpSlot->bf1IsActiveByKarma=	lpDoor->m_bf1IsActiveByKarma;

		packet.wDoorCount++;
	}

	if	(packet.wDoorCount)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aDoors)+sizeof(cFieldDoorStatus)*packet.wDoorCount;

		g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}
}

void
cFIELD::sendExistFireWallList(cACTOR *_lpActor)
{
	SG_EXIST_FIRE_WALL_LIST	packet;

	packet.base.set(sizeof(packet),dSG_EXIST_FIRE_WALL_LIST);

	packet.wWallCount	=	0;

	for (int i=0,iCount=0;i<dMAX_ACTIVE_SKILL && iCount<m_skillManager.m_iActiveSkillCount;i++)
	{
		CActiveSkill	*lpAS		=	&m_skillManager.m_aActiveSkill[i];

		if	(lpAS->m_wSerial	==	0xffff	||	lpAS->m_wSkillType	!=	eSKILL_TYPE_FIRE_WALL)
			continue;

		cAS_FireWall	*lpFireWall	=	(cAS_FireWall	*)lpAS;

		if	(lpFireWall->m_wGrade	!=	cAS_FireWall::eWall)
			continue;

		cSimpleFireWallInfo	*lpInfo	=	&packet.aWallList[packet.wWallCount];

		lpInfo->m_wSkill	=	lpFireWall->m_ability.m_wSkill;
		lpInfo->m_wX1		=	lpFireWall->m_wX1;
		lpInfo->m_wY1		=	lpFireWall->m_wY1;
		lpInfo->m_wX2		=	lpFireWall->m_wX2;
		lpInfo->m_wY2		=	lpFireWall->m_wY2;

		packet.wWallCount++;

		if	(packet.wWallCount	>=	c_iMaxFireWallCountForPacket)
		{
			g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
			packet.wWallCount	=	0;
		}

		iCount++;
	}

	if	(packet.wWallCount)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aWallList)+sizeof(cSimpleFireWallInfo)*packet.wWallCount;

		g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}
}

void
cFIELD::sendExistBannerList(cACTOR *_lpActor)
{
	if	(isSecretDungeon())
		return;

	SG_BANNER_LIST		packet;

	packet.base.set(sizeof(packet),dSG_BANNER_LIST);

	int	iBP	=	1,iCount	=	0;
	int	iBC	=	0;	//	banner count

	for	(int	i=0;i<dMAX_BANNER_COUNT_IN_FIELD && iCount < m_wBannerCount; i++)
	{
		cBannerSimpleInfo	*lpBI		=	(cBannerSimpleInfo	*)&packet.abBuffer[iBP];

		if	(m_awBannerList[i]	==	0xffff)
			continue;

		cBanner				*lpBanner	=	g_bannerManager.get(m_awBannerList[i]);

		if	(!lpBanner	||	lpBanner->m_lpField->m_iUniqueSerial	!=	m_iUniqueSerial)
		{
			m_awBannerList[i]	=	0xffff;
			m_wBannerCount--;
			continue;
		}

		lpBI->m_wBannerSerial	=	lpBanner->m_wSerial;
		lpBI->m_wX				=	lpBanner->m_wX;
		lpBI->m_wY				=	lpBanner->m_wY;
		lpBI->m_bf15Shape		=	lpBanner->m_bf15Shape;
		lpBI->m_bf1IsVertical	=	lpBanner->m_bf1IsVertical;
		strcpy(lpBI->m_strTitle	,lpBanner->m_strTitle);

		iBP						+=	sizeof(cBannerSimpleInfo)-sizeof(lpBanner->m_strTitle)+strlen(lpBanner->m_strTitle)+1;
		iBC++;

		if	(iBP+sizeof(cBannerSimpleInfo)	>=	sizeof(packet.abBuffer))
		{
			packet.abBuffer[0]	=	iBC;
			packet.base.wSize	=	sizeof(packet)-sizeof(packet.abBuffer)+iBP;

			g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
			iBC	=	0;
		}

		iCount++;
	}

	packet.abBuffer[0]	=	iBC;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.abBuffer)+iBP;

	g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

void
cFIELD::sendExistArcaList(cACTOR *_lpActor)
{
	SG_ARCA_LIST		packet;

	packet.base.set(sizeof(packet),dSG_ARCA_LIST);
	packet.bf8Count		=	0;
	packet.bf1IsFirst	=	TRUE;

	int	iArcaCount		=	m_wArcaCount;

	for (int i=0,iCount=0;i<dMAX_ARCA_COUNT_IN_FIELD && iCount<iArcaCount;i++)
	{
		int		iArcaSerial	=	m_awArcaList[i];

		if	(iArcaSerial	==	0xffff)
			continue;

		iCount++;

		cArca	*lpArca	=	g_arcaManager.get(iArcaSerial);

		if	(!lpArca	||	lpArca->getField()	!=	this)
		{
			g_arcaManager.remove(iArcaSerial);
			m_awArcaList[i]	=	0xffff;
			m_wArcaCount--;
			continue;
		}

		packet.aArcaList[packet.bf8Count].m_wSerial			=	iArcaSerial;
		packet.aArcaList[packet.bf8Count].m_wX				=	lpArca->m_pos.x;
		packet.aArcaList[packet.bf8Count].m_wY				=	lpArca->m_pos.y;
		packet.aArcaList[packet.bf8Count].m_bf8SecretLevel	=	lpArca->m_bSecretLevel;
		packet.aArcaList[packet.bf8Count].m_bf1IsDestroyAble	=	lpArca->m_bf1IsDestroyAble;
		packet.aArcaList[packet.bf8Count].m_bf1IsLocked		=	FALSE;

		if	(lpArca->m_bLockLevel)
			packet.aArcaList[packet.bf8Count].m_bf1IsLocked	=	TRUE;

		packet.bf8Count++;
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aArcaList)+sizeof(cArcaSimpleInfo)*packet.bf8Count;

	g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

void
cFIELD::sendExistTrapList(cACTOR *_lpActor)
{
	if	(m_bIs1LevelGuildHall)
		return;

	{
		SG_EXIST_TRAP_LIST	packet;

		packet.base.set(sizeof(packet),dSG_EXIST_TRAP_LIST);
		packet.wTrapCount	=	0;
		
		for (int i=0;i<m_wTrapCount;i++)
		{
			int		iTrapSerial	=	m_awTrapList[i];

			cTrap	*lpTrap		=	g_pTrapManager->get(iTrapSerial);

			if	(!lpTrap)
				continue;

			if	(lpTrap->m_wLocalPart	!=	_lpActor->m_iLocalPart)
				continue;

			int	iRange	=	lpTrap->m_wRange;

			if	(iRange	==	0)
				iRange	=	lpTrap->getAbility()->getHitRange();

			packet.aTrapInfo[packet.wTrapCount].m_wSerial		=	iTrapSerial;
			packet.aTrapInfo[packet.wTrapCount].m_wRange		=	iRange;
			packet.aTrapInfo[packet.wTrapCount].m_wSkill		=	lpTrap->getAbility()->m_wSkill;
			packet.aTrapInfo[packet.wTrapCount].m_wX			=	lpTrap->m_pos.x;
			packet.aTrapInfo[packet.wTrapCount].m_wY			=	lpTrap->m_pos.y;
			packet.aTrapInfo[packet.wTrapCount].m_bf3Team		=	lpTrap->m_wTeam;
			packet.aTrapInfo[packet.wTrapCount].m_bf8DetectLevel=	lpTrap->m_wDetectLevel;

			packet.wTrapCount++;

			if (packet.wTrapCount	>=	dMAX_TRAP_COUNT_IN_FIELD_FOR_PACKET)
			{
				g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);

				packet.wTrapCount	=	0;
			}
		}

		if (packet.wTrapCount)
		{
			packet.base.wSize	=	sizeof(packet) - sizeof(packet.aTrapInfo)+sizeof(cSimpleTrapInfo)*packet.wTrapCount;
			g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
		}
	}

	{
		SG_EXIST_ACTIVE_FIELD_TRAP_LIST	packet;

		packet.base.set(sizeof(packet),dSG_EXIST_ACTIVE_FIELD_TRAP_LIST);
		packet.wTrapCount	=	0;

		for (int i=0;i<m_pArea->m_wTrapCount;i++)
		{
			cAreaTrapInfo	*lpTrap	=	m_pArea->getFieldTrap(i);

			if	(!lpTrap->m_wIsActive)
				continue;

			packet.awTrapList[packet.wTrapCount]	=	lpTrap->m_wSerial;	
			packet.wTrapCount++;

			if (packet.wTrapCount	>=	dMAX_ACTIVE_TRAP_COUNT_IN_PACKET)
			{
				g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
				packet.wTrapCount	=	0;
			}
		}

		if (packet.wTrapCount)
		{
			packet.base.wSize	=	sizeof(packet) - sizeof(packet.awTrapList)+sizeof(WORD)*packet.wTrapCount;
			g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
		}
	}

}

//
//	필드에 존재하는 캐릭터들 시리얼 리스트 전송
void
cFIELD::sendMinimumActorInfoList(cACTOR *_lpActor)
{
	SG_MINIMUM_ACTOR_INFO_LIST	packet;
	SG_OPERATOR_LIST			packetOperatorList;
	SG_EVENT_NPC_LIST			packetEventNpcList;

	int		i	=	_lpActor->m_wReceiveFieldActorData;

	packet.base.set(0,dSG_MINIMUM_ACTOR_INFO_LIST);
	packet.wCount				=	0;
	packetEventNpcList.base.set(0,dSG_EVENT_NPC_LIST);
	packetEventNpcList.wCount	=	0;
	packetOperatorList.base.set(0,dSG_OPERATOR_LIST);
	packetOperatorList.wCount	=	0;

	_lpActor->m_wReceiveFieldActorData	=	0xffff;

	for (;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR	*lpExistActor	=	getActor(i);

		if	(!lpExistActor)
			continue;
		if	(lpExistActor->m_bIsWaitOrgAvatarData)
			continue;
		if	(lpExistActor->m_wSerialInField	==	_lpActor->m_wSerialInField	)
			continue;	//	자기 자신의 정보는 안 보낸다.
		if	(lpExistActor->isDeath()	&&	!lpExistActor->isPlayer()			)
			continue;	//	죽은 몬스터다.

		if	(m_bIs1LevelGuildHall	&&	lpExistActor->isPlayer())
			if	(lpExistActor->m_wGuildSerial	!=	_lpActor->m_wGuildSerial)
				continue;	//	죽은 몬스터다.

		if	(lpExistActor->isPet() || lpExistActor->isSummonBeast())
		{
			cACTOR	*lpTamer	=	lpExistActor->getTamer();

			if	(!lpTamer || lpTamer->m_wGuildSerial	!=	_lpActor->m_wGuildSerial)
				continue;
		}

		lpExistActor->getMinumumActorInfo(&packet.aInfoList[packet.wCount++]);
		
		if (lpExistActor->m_wActorKind >	eAK_MONSTER)
		{
			cNPC	*lpNpc	=	lpExistActor->getNpc();

			if	(lpNpc->isNotMoveNpc())
				lpExistActor->getEventNpcInfo(&packetEventNpcList.aInfoList[packetEventNpcList.wCount++]);
		}

		if (lpExistActor->m_wOperatorLevel	>=	eAL_TESTER)
		{
			packetOperatorList.aOperatorList[packetOperatorList.wCount].wSerialInField	=	i;
			packetOperatorList.aOperatorList[packetOperatorList.wCount].wOperatorLevel	=	lpExistActor->m_wOperatorLevel;
			strcpy(packetOperatorList.aOperatorList[packetOperatorList.wCount].strName,lpExistActor->m_strName);

			packetOperatorList.wCount++;
			
			if (packetOperatorList.wCount	>=	dEXIST_OPERATOR_COUNT_IN_FIELD)
			{
				_lpActor->m_wReceiveFieldActorData	=	i+1;	//	여기까지 받았다~~
				break;
			}
		}

		if	(packet.wCount	>=	dEXIST_ACTOR_COUNT_FOR_EASL)	//	당신.. 오바야. -o-
		{
			_lpActor->m_wReceiveFieldActorData	=	i+1;	//	여기까지 받았다~~
			break;
		}
	}

	if	(packet.wCount	>	0)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aInfoList)+packet.wCount*sizeof(CMinimumActorInfo);
		g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}

	if	(packetEventNpcList.wCount	>	0)
	{
		packetEventNpcList.base.wSize	=	sizeof(packetEventNpcList)-sizeof(packetEventNpcList.aInfoList)+packetEventNpcList.wCount*sizeof(CMinimumActorInfo);
		g_userPM.add(_lpActor->m_iClientSerial,&packetEventNpcList,packetEventNpcList.base.wSize);
	}

	if	(packetOperatorList.wCount	>	0)
	{
		packetOperatorList.base.wSize	=	sizeof(packetOperatorList)-sizeof(packetOperatorList.aOperatorList)+packetOperatorList.wCount*sizeof(COperatorInfo);
		g_userPM.add(_lpActor->m_iClientSerial,&packetOperatorList,packetOperatorList.base.wSize);
	}
}

//	필드에 존재하는 노점상 정보 전송
void
cFIELD::sendExistPitchmanShopList(cACTOR *_lpActor)
{
	SG_PITCHMAN_SHOP_LIST	packet;

	packet.base.set(sizeof(packet),dSG_PITCHMAN_SHOP_LIST);
	packet.bf15Count	=	0;
	packet.bf1IsFirst	=	TRUE;

	for (int i=0;i<g_pitchmanShopManager.getMaxCount();i++)
	{
		CPitchmanShop	*lpShop	=	g_pitchmanShopManager.get(i);

		if	(!lpShop)
			continue;

		if	(lpShop->m_wFieldSerial	!=	m_iUniqueSerial)
			continue;

		if	(m_bIs1LevelGuildHall)
		{
			cACTOR	*lpOwner	=	lpShop->getOwner();

			if	(!lpOwner	||	lpOwner->m_wGuildSerial	!=	_lpActor->m_wGuildSerial)
				continue;
		}

		packet.aList[packet.bf15Count].wSerialInField	=	lpShop->m_wSerial;
		packet.aList[packet.bf15Count].wX				=	lpShop->m_pos.x;
		packet.aList[packet.bf15Count].wY				=	lpShop->m_pos.y;

		packet.bf15Count++;

		if (packet.bf15Count	>=	dPITCHMAN_SHOP_COUNT_FOR_LIST_PACKET)
		{
			g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);

			packet.bf15Count	=	0;
			packet.bf1IsFirst	=	FALSE;
		}
	}

	if (packet.bf15Count || packet.bf1IsFirst)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aList) + sizeof(CMinimumPitchmanShopInfo)*packet.bf15Count;

		g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}

	_lpActor->m_wIsReceivedPitchmanShopList	=	TRUE;
}

void
cFIELD::sendExistPortalList(cACTOR *_lpActor)
{
	SG_PORTAL_LIST	packet;

	packet.base.set(sizeof(packet),dSG_PORTAL_LIST);
	packet.wPortalCount	=	0;

	for (int i=m_portal.m_iPortalCount;i;)
	{
		--i;

		CSimplePortalInfo	*lpInfo	=	&packet.aPortalList[packet.wPortalCount];

		if	(m_portal.m_aPortal[i].m_wOwner	==	0xffff)
			continue;

		lpInfo->m_wX			=	m_portal.m_aPortal[i].m_wX;
		lpInfo->m_wY			=	m_portal.m_aPortal[i].m_wY;
		lpInfo->m_bf12Village	=	m_portal.m_aPortal[i].m_wField;
		lpInfo->m_bf10Owner		=	m_portal.m_aPortal[i].m_wOwner;
		lpInfo->m_bf7Serial		=	i;
		lpInfo->m_bf1IsTownPortal=	FALSE;

		if	(m_portal.m_aPortal[i].m_wTargetX	==	0xffff)
			lpInfo->m_bf1IsTownPortal=	TRUE;

		packet.wPortalCount++;
	}

	if	(packet.wPortalCount)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aPortalList) + sizeof(CSimplePortalInfo)*packet.wPortalCount;

		g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}

	_lpActor->m_wIsReceivedPortalList	=	TRUE;
}

//
//	필드에 존재하는 플레이어 혹은 몬스터 정보를 보내준다.
void
cFIELD::sendExistFieldItem(cACTOR *_lpActor)
{
	SG_DROPPING_ITEMS	sgDroppingItems;
	int					i				=	_lpActor->m_wReceiveFieldItemData;
	BOOL				bIncorrectData	=	FALSE;

	sgDroppingItems.base.set(sizeof(sgDroppingItems),dSG_DROPPING_ITEMS);
	sgDroppingItems.wCount				=	0;

	_lpActor->m_wReceiveFieldItemData	=	0xffff;

	for (;i<dMAX_DROPPED_ITEM;i++)
	{
		CDroppedItem	*lpDroppedItem	=	m_droppedItem.get(i);

		if	(!lpDroppedItem							)
			continue;
		if	(lpDroppedItem->m_wSerial	==	0xffff	)
			continue;

		cP_DROPPED_ITEM	*lpDropItemInfo	=	&sgDroppingItems.aDroppedItem[sgDroppingItems.wCount];

		if	(lpDroppedItem->m_item.m_wBaseItem	==	0xffff)
		{
			CLOG("dropItem","빈 아이템이 들어 있다. %s/%s",m_strFileName,m_strName);

			bIncorrectData				=	TRUE;
			lpDroppedItem->m_wSerial	=	0xffff;
			continue;
		}


		lpDropItemInfo->m_bf10Serial		=	lpDroppedItem->m_wSerial;//시리얼

		if	(lpDroppedItem->m_item.isPrincessWeapon())
		{
			cPrincessWeaponDefine	*lpWeapon			=	lpDroppedItem->m_item.getPrincessWeapon();

			if	(!lpWeapon)
				return;

			lpDropItemInfo->m_bf14BaseItem		=	lpWeapon->m_wBasicItem;
			lpDropItemInfo->m_wCount			=	1;
			lpDropItemInfo->m_bf4Type			=	eMIT_PRINCESS_WEAPON;
		}
		else
		{
			lpDropItemInfo->m_bf14BaseItem		=	lpDroppedItem->m_item.m_wBaseItem;

			if	(lpDropItemInfo->m_bf14BaseItem	==	dITEM_MONEY)
				lpDropItemInfo->m_wCount		=	(WORD)min(lpDroppedItem->m_item.m_dwSerial,0xffff);
			else
				lpDropItemInfo->m_wCount		=	lpDroppedItem->m_item.m_bCount;

			lpDropItemInfo->m_bf4Type			=	lpDroppedItem->m_item.getItemMagicalType();
		}

		lpDropItemInfo->wX					=	lpDroppedItem->m_pos.x;//어디에 떨어져 있냐?
		lpDropItemInfo->wY					=	lpDroppedItem->m_pos.y;//어디에 떨어져 있냐?

		sgDroppingItems.wCount++;

		if	(sgDroppingItems.wCount	>=	dMAX_OPERATE_DROPPED_ITEM)	//	당신.. 오바야. -o-
		{
			_lpActor->m_wReceiveFieldItemData	=	i+1;	//	여기까지 받았다~~
			break;
		}
	}

	if	(bIncorrectData)
		m_droppedItem.rebuild();

	if	(sgDroppingItems.wCount	>	0)
	{
		sgDroppingItems.base.wSize	=	sizeof(sgDroppingItems)-sizeof(sgDroppingItems.aDroppedItem)+sgDroppingItems.wCount*sizeof(cP_DROPPED_ITEM);

		g_userPM.add(_lpActor->m_iClientSerial,&sgDroppingItems,sgDroppingItems.base.wSize);
	}
}

//
//	저장된 패킷들중에 필요한 패킷을 골라서 _lpActor에게 보낸다.
BOOL
cFIELD::sendStoredPacketsToPlayer(cACTOR *_lpActor)
{
	if	(_lpActor->m_iRookieTime	)	
		if	(sendFirstMeetingData(_lpActor))
			return	TRUE;

	if	(m_queSendPacket.getCount()	==	0)
		return	FALSE;

	m_queSendPacket.setFront();	//	패킷을 젤 앞에 둔다.

	int		iPacketCount	=	0;
	cCFPM	*lpPacketTrailer=	g_userPM.inlineGetClient(_lpActor->m_iClientSerial);

	if	(!lpPacketTrailer)
		return	FALSE;

	while(1)
	{
		cPACKET_INFO	*lpPacketInfo	=	m_queSendPacket.getOne();
		ALL_MSG			*lpPacket		=	&lpPacketInfo->m_packet;
		cACTOR			*lpOwner		=	NULL;

		if	(!lpPacketInfo)
			break;	//	다 꺼냈다.

		if	(lpPacketInfo->m_iOwner		>=	0	&&	lpPacketInfo->m_iOwner	<	dMAX_ACTOR_IN_FIELD)
			lpOwner						=	getActor(lpPacketInfo->m_iOwner);
		else
		if	(lpPacketInfo->m_iOwner		==	cPACKET_INFO::eSendedPacket)
			continue;
		
		if	(m_bIs1LevelGuildHall)
		{
			BOOL	bIsForcedSend	=	FALSE;
			
			if	(lpPacket->base.wType	==	dSG_CHAT)
			{
				SG_CHAT	*lpChatPacket	=	&lpPacket->mSG_CHAT;

				if	(lpChatPacket->bf6ChatType	==	eCT_PARTY)
					bIsForcedSend	=	TRUE;
			}

			if	(lpPacket->base.wType	==	dSG_REMOVE_DROPPED_ITEMS	||	lpPacket->base.wType	==	dSG_REMOVE_DROPPED_ITEM)
				bIsForcedSend	=	TRUE;

			if	(bIsForcedSend	==	FALSE)
			{
				if	(!lpOwner)
					continue;

				cACTOR	*lpMaster	=	lpOwner->getControlLord();
				
				if	(lpMaster->m_wGuildSerial!=	_lpActor->m_wGuildSerial)
					continue;
			}
		}

		switch(lpPacket->base.wType)
		{
			case	dSG_CHAT						:
			{
				SG_CHAT	*lpChatPacket	=	&lpPacket->mSG_CHAT;

				if	(lpChatPacket->bf6ChatType	==	eCT_GUILD)
					break;
				else
				if	(lpChatPacket->bf6ChatType	==	eCT_PARTY)
				{
					if	(lpChatPacket->bf11PGSerial	!=	(DWORD)_lpActor->getPartySerial())
						break;
				}
				else
				{
					if	(lpChatPacket->bf6ChatType	==	eCT_TALK || lpChatPacket->bf6ChatType	==	eCT_TALK +0x20 || lpChatPacket->bf6ChatType == eCT_EVENT_CHAT)
					{
						if	(!lpOwner)
							break;

						if	(!_lpActor->m_rectMoveFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
							break;
					}
				}

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_CHANGE_GUILD_MARK						:
			case	dSG_CHANGE_GUILD_TAX_RATE					:
			case	dSG_CHANGE_GUILD_NOTICE						:
			case	dSG_UPDATE_GUILD_BATTLE_COUNT_AT_THIS_WEEK	:
			case	dSG_UPDATE_GUILD_MEMBER						:
			case	dSG_REMOVE_GUILD_MEMBER						:
			case	dSG_UPDATE_GUILD_BATTLE_INFO_FOR_MEMBER		:
			case	dSG_READY_TO_NEXT_GUILD_BATTLE				:
			case	dSG_GUILD_BATTLE_RESULT						:
			{
				SG_GUILD_PACKET_FORM	*lpGuildPacket	=	(SG_GUILD_PACKET_FORM	*)lpPacket;

				if	(lpGuildPacket->wGuildSerial	!=	_lpActor->m_wGuildSerial)
					break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_MOVE_FOR_PARTY_MEMBER		:
			{
				if	(lpPacketInfo->m_iOwner	==	_lpActor->m_wSerialInField)
					break;	//	자기 자신한테는 안 보낸다.

				if	(!lpOwner)
					break;

				if	(lpOwner->getPartySerial()	!=	_lpActor->getPartySerial())
					break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_MOVE						:	//	이동
			{
				if	(lpPacketInfo->m_iOwner	==	_lpActor->m_wSerialInField)
					break;	//	자기 자신한테는 안 보낸다.

				if	(!lpOwner)
					break;

				if	(m_bIsGuildBattleField == FALSE)
					if	(!_lpActor->m_rectMoveFrame.Isin(lpPacket->mSG_MOVE.wPosX,lpPacket->mSG_MOVE.wPosY))
						if	(lpOwner->getPartySerial() == 0xffff || _lpActor->getPartySerial() != lpOwner->getPartySerial())
							break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_STOP						:	//	정지
			{
				if	(!checkActor(lpOwner))
					break;

				if	(m_bIsGuildBattleField == FALSE)
					if	(!_lpActor->m_rectMoveFrame.Isin(lpPacket->mSG_STOP.wPosX,lpPacket->mSG_STOP.wPosY))
					{
						if	(lpOwner->getPartySerial() == 0xffff || _lpActor->getPartySerial() != lpOwner->getPartySerial())
							break;
					}

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_TOGGLE_SITDOWN				:
			{
				if	(lpPacketInfo->m_iOwner	==	_lpActor->m_wSerialInField)
					break;	//	자기 자신한테는 안 보낸다.
				if	(!checkActor(lpOwner))
					break;

				if	(!_lpActor->m_rectActionFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
					break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_CORRECT_PLAYER_POSITION		:	//	플레이어 위치 보정
			{
				if	(m_bIsGuildBattleField == FALSE)
					if	(!_lpActor->m_rectMoveFrame.Isin(lpPacket->mSG_CORRECT_PLAYER_POSITION.wPosX,lpPacket->mSG_CORRECT_PLAYER_POSITION.wPosY))
						break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_THRUST_ACTORS				:
			{
				if (m_bIsGuildBattleField == FALSE)
					if (_lpActor->m_rectMoveFrame.Isin(lpPacket->mSG_THRUST_ACTORS.wAxisXPos,lpPacket->mSG_THRUST_ACTORS.wAxisYPos))
						lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_USE_ITEM					:
			{
				cACTOR	*lpActor	=	getActor(lpPacket->mSG_USE_ITEM.wWho);
				
				if	(!checkActor(lpActor))
					break;

				if	(_lpActor->m_rectActionFrame.Isin(lpActor->m_pos.x,lpActor->m_pos.y))
					lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_THRUST							:
			case	dSG_RUSH							:
			case	dSG_TELEPORT						:
			{
				if (!checkActor(lpOwner))
					break;

				if (m_bIsGuildBattleField == FALSE)
					if (!_lpActor->m_rectMoveFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
						break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_VERY_SIMPLE_ACTOR_INFO_LIST		:
			case	dSG_SIMPLE_ACTOR_INFO_LIST			:
			case	dSG_REFIT_ACTORS_POSITION			:
			{
				if (lpOwner && !lpOwner->isPlayer())	//	플레이어가 아닌경우는 주변에 얼라들한테 다 보낸다.
				{
					if (!_lpActor->m_rectActionFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
						break;
				}
				else
				if (lpPacketInfo->m_iOwner	!=	_lpActor->m_wSerialInField)	//	플레이어일 경우 자기 자신한테만 보낸다.
					break;	//	자기 자신한테만 보낸다.

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_VERY_SIMPLE_ACTOR_INFO			:
			{
				if (!checkActor(lpOwner))
					break;

				if (!_lpActor->m_rectActionFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
					break;

				SG_VERY_SIMPLE_ACTOR_INFO	*lpInfo	=	(SG_VERY_SIMPLE_ACTOR_INFO	*)lpPacket;

				_lpActor->correctInvisibleStatus((CVerySimplePlayerInfo *)lpInfo->aBuffer,lpOwner);

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);

				break;
			}
			case	dSG_SIMPLE_ACTOR_INFO				:
			{
				if (!checkActor(lpOwner))
					break;

				if (!_lpActor->m_rectActionFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
					break;

				SG_SIMPLE_ACTOR_INFO	*lpInfo	=	(SG_SIMPLE_ACTOR_INFO	*)lpPacket;

				_lpActor->correctInvisibleStatus((CSimplePlayerInfo *)lpInfo->aBuffer,lpOwner);

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_ACTOR_ENCHANTED_STATUS			:
			{
				if (!checkActor(lpOwner))
					break;

				if (!_lpActor->m_rectActionFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
					break;

				SG_ACTOR_ENCHANTED_STATUS	*lpInfo	=	(SG_ACTOR_ENCHANTED_STATUS	*)lpPacket;

				_lpActor->correctInvisibleStatus(&lpInfo->dwMask,lpOwner);

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_UPDATE_BASIC_INFORMATION		:
			{
				if	(!checkActor(lpOwner))
					break;

				if	(m_bIsGuildBattleField)
				{
					if	(!_lpActor->isOwnForce(lpOwner))
						break;
				}
				else
				{
					if	(!_lpActor->isPartyMember(lpOwner))
						break;
				}

				if	(!_lpActor->m_rectActionFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
					break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_ACTION_TO_ACTOR					:
			{
				if	(!checkActor(lpOwner))
					break;

				SG_ACTION_TO_ACTOR	*lpActionPacket	=	(SG_ACTION_TO_ACTOR	*)lpPacket;
				cACTOR				*lpTarget		=	getTestedActor(lpActionPacket->bf11TargetSerial);

				if	(!lpTarget)
					break;

				if	(!_lpActor->m_rectActionFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y)	&&	!lpTarget->m_rectActionFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
					break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_BASIC_ATTACK					:
			case	dSG_QUICK_ACTION					:
			case	dSG_WIDE_AREA_DAMAGE				:
			case	dSG_WIDE_AID_ATTACK_RESULT			:
			case	dSG_HIT_CHANED_DAMAGE				:
			case	dSG_CONTINUOUS_HIT_BY_CHARACTER		:
			case	dSG_STRIKE_INTERVAL_SHOOTER			:
			case	dSG_CAST_INTERVAL_SHOOTER			:
			case	dSG_CONTINUOUS_HIT_BY_VALUE			:
			case	dSG_LAND_MARKER						:
			case	dSG_BUNSHINE_ATTACK					:
			case	dSG_SCIMITAR_CUTTING				:
			case	dSG_ACTION_TO_GROUP					:
			case	dSG_ACTION_TO_GROUP_STICKED_BIT		:
			case	dSG_ACTION_TO_GROUND				:
			case	dSG_HIT_THORN_DAMAGE				:
			case	dSG_ABSORB_DAMAGE					:
			case	dSG_CURE_ACTOR						:
			case	dSG_FLOAT_MACHINE					:
			case	dSG_RELEASE_MACHINE					:
			case	dSG_SHOOT_MACHINE_MISSILE			:
			case	dSG_FIRE_TAIL_CHASER				:
			case	dSG_MAKE_ACTION_FORM				:
			case	dSG_USE_SKILL_TO_ACTOR				:
			case	dSG_USE_SKILL_TO_GROUND				:
			case	dSG_RECEIVE_AID_MAGIC				:
			case	dSG_HIT_AURA_SKILL					:
			case	dSG_LEVEL_UP_OTHER					:
			case	dSG_CONTROL_MONSTER					:
			case	dSG_ACTIVE_REACTION_SKILL			:
			case	dSG_ICY_STALAGMITE					:
			case	dSG_SIMPLE_HIT						:
			case	dSG_REGEN_HP_BY_AID_SKILL			:
			case	dSG_RECOVER_HP_BY_AID_SKILL			:
			case	dSG_HIT_DAMAGE						:
			case	dSG_SKILL_EFFECT					:
			case	dSG_MISS							:
			case	dSG_FIRST_AID						:
			case	dSG_DOOR_MESSAGE					:
			case	dSG_ATTACK_TO_OBJECT				:
			case	dSG_EXPLOSION_TRAP_SKILL			:
			case	dSG_CLOSE_RANGE_ARCA_WORK			:
			case	dSG_CLOSE_RANGE_FIELD_TRAP_WORK		:
			case	dSG_CHECK_RANGE_ETC_WORK			:
			case	dSG_WIDE_HEAL						:
			case	dSG_STRIKE_LIGHTNING_WINDER			:
			case	dSG_STRIKE_MULTI_MISSILE			:		// 멀티미사일.	
			case	dSG_TITLE_EFFECT					:
			case	dSG_PLAY_GGG_EFFECT					:
			{
				if	(!checkActor(lpOwner))
					break;

				if	(!_lpActor->m_rectActionFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
					break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_GUARDIAN_POST					:
			{
				if (!checkActor(lpOwner))
					break;

				if (lpPacket->mSG_GUARDIAN_POST.wTargetCount)	//	타격 입히기 
					if (!_lpActor->m_rectActionFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
						break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}


			case	dSG_HIT_ACTION					:
			{
				if	(!checkActor(lpOwner))
					break;
				if	(!_lpActor->m_rectActionFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
					break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_ACTIVE_SKILL_EFFECT			:
			case	dSG_ACTIVE_MIRROR_TOWER			:
			{
				SG_ACTIVE_SKILL_EFFECT	*lpInfo	=	&lpPacket->mSG_ACTIVE_SKILL_EFFECT;

				cACTOR	*lpCaster	=	getActor(lpInfo->wCaster);
				cACTOR	*lpTarget	=	getActor(lpInfo->wTarget);

				if	(!lpCaster)
					break;

				if (!_lpActor->m_rectActionFrame.Isin(lpCaster->m_pos.x,lpCaster->m_pos.y))
				{
					if (lpTarget == NULL)	break;

					if (!_lpActor->m_rectMoveFrame.Isin(lpCaster->m_pos.x,lpCaster->m_pos.y))	break;
				}

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	이상 패킷은 받는 캐릭터의 위치에 따라서 취사 선택한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			case	dSG_JOIN_PLAYER					:	//	참가
			{
				if(lpPacket->mSG_JOIN_PLAYER.actorInfo.bf11SerialInField	==	_lpActor->m_wSerialInField)
					break;

				_lpActor->correctInvisibleStatus(&lpPacket->mSG_JOIN_PLAYER.actorInfo.dwEnchantedMask,lpOwner);

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_ADD_TRAP					:
			{
				if (!_lpActor->m_rectActionFrame.Isin(lpPacket->mSG_ADD_TRAP.wPosX,lpPacket->mSG_ADD_TRAP.wPosY))
					break;

				if (_lpActor->m_wTeam	!=	lpPacket->mSG_ADD_TRAP.bf3Team)
					if (!_lpActor->isVisibleTrap(lpPacket->mSG_ADD_TRAP.bf8DetectLevel))
						break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}
			case	dSG_EXPLOSION_TRAP				:
			{
				if (!_lpActor->m_rectActionFrame.Isin(lpPacket->mSG_EXPLOSION_TRAP.wPosX,lpPacket->mSG_EXPLOSION_TRAP.wPosY))
					break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_CHANGE_GUILD_BATTLE_SCHEDULE			:
			{
				if	(_lpActor->m_wIsOpendGuildBattleScheduler	!=	1)
					break;

				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}

			case	dSG_SET_SHIMMERING_SHIELD		:	//	시머링 실드
			case	dSG_RELEASE_SHIMMERING_SHIELD	:	//	시머링 실드 해제
			case	dSG_TRANSFORMATION				:	//	변신
			case	dSG_ADD_PITCHMAN_SHOP			:	//	노점상 추가
			case	dSG_CHANGE_PITCHMAN_SHOP_INFO	:	//
			case	dSG_REMOVE_PITCHMAN_SHOP		:	//	노점상 제거
			case	dSG_CHANGE_DROPPED_ITEM_INFO	:	//	필드에 아이템 정보 변경
			case	dSG_REMOVE_DROPPED_ITEM			:	//	필드에 아이템 제거
			case	dSG_REMOVE_DROPPED_ITEMS		:	//	필드에 아이템들 제거
			case	dSG_CHANGE_APPEARANCE			:	//	플레이어 외관 변화 
			case	dSG_PLAYER_OUT					:	//	플레이어 나감
			case	dSG_REMOVE_ACTOR				:
			case	dSG_KILL						:	//	죽었다.
			case	dSG_DROPPING_ITEMS				:	//	아이템 드랍
			case	dSG_REGEN_MONSTER				:	//	몬스터 리젠
			case	dSG_DROPPING_ITEM				:	//	아이템 드롭
			case	dSG_CREATE_DUMMY				:
			case	dSG_COPY_ACTOR					:
			case	dSG_RELEASE_DUMMY				:
			case	dSG_DAMAGE_TO_ACTOR_BY_KARMA	:	//	-_-;;
			case	dSG_EVENT_NOTICE				:
			case	dSG_OCCUR_EFFECT				:
			case	dSG_REGIST_MESSAGE				:
			case	dSG_ANNOUNCE_QUIZ_EVENT			:	//	OX 관련 퀴즈 공지
			case	dSG_START_COUNTDOWN				:	//	카운트 다운 시작
			case	dSG_BEGIN_QUIZ_EVENT			:	//	OX 퀴즈 시작
			case	dSG_QUIZ_EVENT_QUESTION			:	//	OX 퀴즈 질문
			case	dSG_OX_QUIZ_RESULT				:	//	OX 퀴즈 결과 받음
			case	dSG_END_QUIZ_EVENT				:	//	OX 퀴즈 종료
			case	dSG_QUIZ_EVENT_LOSER_LIST		:
			case	dSG_CANCEL_QUIZ_EVENT			:
			case	dSG_FAILED_SELL_ITEM			:
			case	dSG_GUILD_BATTLE_NOTICE			:
			case	dSG_REMOVE_PORTAL				:
			case	dSG_CHANGE_DOOR_STATUS			:
			case	dSG_ADD_PORTAL					:
			case	dSG_REVIVE						:
			case	dSG_REMOVE_TRAP					:
			case	dSG_NOTICE						:
			case	dSG_WINNING_PRIZE_LOTTO			:
			case	dSG_ADD_ARCA					:
			case	dSG_ARCA_WORK					:
			case	dSG_ETC_WORK					:
			case	dSG_FIELD_TRAP_WORK				:
			case	dSG_SECRET_DUNGEON_MESSAGE		:
			case	dSG_ENTER_TO_THE_SECRET_DUNGEON	:
			case	dSG_SET_ANM						:
			case	dSG_OX_QUIZ_LOSER_REVIVE_BATTLE	:
			case	dSG_OX_QUIZ_WINNER_LIST			:
			case	dSG_BANNER_INFO					:
			case	dSG_BOOST_POWER_BY_EVENT		:
			case	dSG_GUILD_BATTLE_KILL_NOTICE	:
			case	dSG_USE_SKILL_RESULT			:
			case	dSG_GUILD_BATTLE_INFO			:
			case	dSG_WORD_QUIZ_RESULT			:
			case	dSG_DUEL_RESULT					:
			case	dSG_SOUND_OF_LEADERS_BELL		:
			case	dSG_EVENT_TIME_UPDATE			:
			{
				lpPacketTrailer->add(lpPacket,lpPacket->base.wSize);
				break;
			}
		}
	}

	return	TRUE;
}