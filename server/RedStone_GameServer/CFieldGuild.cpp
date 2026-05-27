#include	<time.H>
#include	"cFIELD.H"
#include	"cGAME.H"
#include	"instance_guild_battle_manager.H"
#include	"booking_work.H"
#include	"packetManager.H"

//
//	길드전 설정
void
cFIELD::setBattleGuild(int _iGuild1Serial,int _iGuild2Serial,int _iFieldOrder)
{
	m_aBattleGuild[0].m_wSerial			=	_iGuild1Serial;
	m_aBattleGuild[1].m_wSerial			=	_iGuild2Serial;
	m_aBattleGuild[0].m_iKillPoint		=	0;
	m_aBattleGuild[1].m_iKillPoint		=	0;
	m_wGuildBattleFieldOrder			=	_iFieldOrder;

	m_wGuildBattleFinished				=	FALSE;
	m_wIsIncorrectGuildBattle			=	FALSE;
	m_wWaitToExterminatedGuildStatusTime=	0;

	CGuild	*lpGuild1	=	g_guildManager.getBySerial(m_aBattleGuild[0].m_wSerial);
	CGuild	*lpGuild2	=	g_guildManager.getBySerial(m_aBattleGuild[1].m_wSerial);
	
	if	(lpGuild1	&&	lpGuild2)
		m_bIsOperateGuildBattle		=	TRUE;
	else
		m_bIsOperateGuildBattle		=	FALSE;
}


//
//	길드전 관련 업데이트
void
cFIELD::operateGuildBattleWorkPerSecond()
{
	if	(m_bIsGuildBattleField+m_wIsGuildBattleLobby == 0	||	m_bIsSiegeWarfareField)	//	일반 필드다.
		return;
	
	if	(m_wDuelType	!=	0xffff)
		return;

	if	(m_iRemainGuildBattleTime	>	-1)
	{
		m_iRemainGuildBattleTime--;

		SG_ETC_WORK	packet;

		packet.base.set(sizeof(packet),dSG_ETC_WORK);
		packet.wWork	=	eEW_INSTANCE_GUILD_BATTLE;
		packet.aiValue[0]=	0;
		packet.aiValue[1]=	m_iRemainGuildBattleTime;

		sendPacketToAllOperator((ALL_MSG*)&packet,eAL_BOSS);

		if	(m_iRemainGuildBattleTime	==	60)
		{
			SG_GUILD_BATTLE_NOTICE	packet;

			packet.base.set(sizeof(packet),dSG_GUILD_BATTLE_NOTICE);
			packet.bRemainTimeToEndBattle	=	1;
			packet.bRemainTimeToBeginBattle	=	0xff;

			addSendPacket((ALL_MSG*)&packet,0);
		}

		if	(m_iRemainGuildBattleTime	==	0)
			finishGuildBattle();
	}

	if	(g_bIsTestGuildBattle)
	{
		if	(m_wGuildBattleStatus	==	eGB_END	&&	m_bIsGuildBattleField)
			endGuildBattle();

		return;
	}

	int	iBattleOrder			=	g_gbsToday.getCurrentBattleOrder(dGUILD_BATTLE_LAST_SERIAL_FIELD-m_iUniqueSerial);
	m_wGuildBattleFieldOrder	=	g_gbsToday.getFieldOrderByFieldSerial(m_iUniqueSerial,iBattleOrder);

	if	(iBattleOrder	!=	-1	&&	iBattleOrder	<	dGUILD_BATTLE_COUNT)
	{
		if	(m_bIsGuildBattleField)
		{
			if	(m_wGuildBattleFieldOrder	==	0xffff)
			{
				m_wGuildBattleStatus	=	eGB_SLEEP_TIME;
				return;
			}

			int	iBattleStatus	=	g_gbsToday.getCurrentBattleStatus(m_wGuildBattleFieldOrder);

			if	(iBattleStatus	!=	m_wGuildBattleStatus)
				changeGuildBattleStatus(iBattleStatus,iBattleOrder);

			if	(iBattleStatus	==	eGB_END)
				endGuildBattle();
		}
	}

	if	(m_wIsGuildBattleLobby)
		warpGuildBattleLobbyPlayerToBattleField(iBattleOrder);

	return;
}

//
//	길드전 필드에서의 팀
int
cFIELD::getTeamInGuildBattleField(int _iGuildSerial,char *_lpstrName)
{
	if	(m_wDuelType	!=	0xffff	&& _lpstrName)
		return	getTeamInDuelField(_lpstrName);
	
	if	(m_bIsSiegeWarfareField)
	{
		cGuildHallInfo*	lpHallInfo	=	g_guildManager.getHallInfoByBattleFieldSerial(m_iUniqueSerial);

		if	(!lpHallInfo)
			return	0xffff;

		if	(lpHallInfo->m_wInvadeGuild	==	0xffff)
			return	0xffff;

		if	(lpHallInfo->m_wInvadeGuild	==	_iGuildSerial)
		{
			m_aBattleGuild[1].m_wSerial	=	_iGuildSerial;
			return	1;
		}
		if	(lpHallInfo->m_wOwnGuild	==	_iGuildSerial)
		{
			m_aBattleGuild[0].m_wSerial	=	_iGuildSerial;
			return	0;
		}

		return	0xffff;
	}

	if	(m_aBattleGuild[0].m_wSerial	== _iGuildSerial)	
		return	0;
	if	(m_aBattleGuild[1].m_wSerial	== _iGuildSerial)	
		return	1;

	if	(g_bIsTestServer)
	{
		if	(m_aBattleGuild[0].m_wSerial == 0xffff)
		{
			m_aBattleGuild[0].m_wSerial = _iGuildSerial;

			return	0;
		}
		if	(m_aBattleGuild[1].m_wSerial == 0xffff)
		{
			m_aBattleGuild[1].m_wSerial = _iGuildSerial;

			return	1;
		}
	}

	if	(g_bIsTestGuildBattle)
	{
		int	iGuild1MemberCount	=	getGuildMemberCount(m_aBattleGuild[0].m_wSerial);
		int	iGuild2MemberCount	=	getGuildMemberCount(m_aBattleGuild[1].m_wSerial);

		if	(iGuild1MemberCount	==	0)
			m_aBattleGuild[0].m_wSerial	=	0xffff;
		if	(iGuild2MemberCount	==	0)
			m_aBattleGuild[1].m_wSerial	=	0xffff;

		if	(m_aBattleGuild[0].m_wSerial==	0xffff)
		{
			m_aBattleGuild[0].m_wSerial	=	_iGuildSerial;

			return 0;
		}
		else
		if (m_aBattleGuild[1].m_wSerial	==	0xffff)
		{
			m_aBattleGuild[1].m_wSerial	=	_iGuildSerial;

			return	1;
		}
	}

	return	0xffff;
}

//
//	특정 길드의 멤버 수를 리터언~~
int
cFIELD::getGuildMemberCount(int _iGuildSerial,BOOL _bIsWantAliveMember)
{
	int	iMemberCount	=	0;
	int	iTeam			=	getTeamInGuildBattleField(_iGuildSerial);

	for (int i=0;i<m_aiActorCount[iTeam];i++)
	{
		cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[iTeam][i]];

		if	(!lpPlayer	||	!lpPlayer->isPlayer())
			continue;

		if	(lpPlayer->m_wGuildSerial	!=	_iGuildSerial)
			continue;

		iMemberCount++;

		if	(_bIsWantAliveMember)
			if	(lpPlayer->isDeath())
				iMemberCount--;
	}

	return	iMemberCount;
}

cACTOR*
cFIELD::getTopLevelGuildMember(int _iGuildSerial,BOOL _bIsWantAliveMember)
{
	int		iTeam		=	getTeamInGuildBattleField(_iGuildSerial);
	int		iTopLevel	=	0;
	cACTOR	*lpActor	=	NULL;

	for (int i=0;i<m_aiActorCount[iTeam];i++)
	{
		cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[iTeam][i]];

		if	(!lpPlayer	||	!lpPlayer->isPlayer())
			continue;

		if	(_bIsWantAliveMember && lpPlayer->isDeath())
			continue;

		if	(lpPlayer->m_wGuildSerial!=	_iGuildSerial)
			continue;

		if (lpPlayer->m_iLevel	>	iTopLevel)
		{
			iTopLevel	=	lpPlayer->m_iLevel;
			lpActor		=	lpPlayer;
		}
	}

	return	lpActor;
}

//
//	길드전 초기화
void
cFIELD::initGuildBattle(int _iBattleOrder)
{
	CGuildBattleInfo	*lpInfo		=	g_gbsToday.getBattleInfoByFieldSerial(m_iUniqueSerial,_iBattleOrder);
	int					iFieldOrder	=	g_gbsToday.getFieldOrderByFieldSerial(m_iUniqueSerial,_iBattleOrder);

	endGuildBattle();
	killTeam(2);

	if	(lpInfo)
		setBattleGuild(lpInfo->m_awBattleGuild[0],lpInfo->m_awBattleGuild[1],iFieldOrder);
	else
		setBattleGuild(0xffff,0xffff);
}

//
//	킬포인트 ㄷ허줘~
void
cFIELD::addGuildBattleKillPoint(cACTOR *_lpKiller,cACTOR *_lpTarget)
{
	if	(_lpKiller	==	NULL)
		return;
	
	m_aBattleGuild[_lpKiller->m_wTeam].m_iKillPoint	+=	_lpTarget->m_iLevel;

	CGuild	*lpGuild1	=	g_guildManager.getBySerial(m_aBattleGuild[0].m_wSerial);
	CGuild	*lpGuild2	=	g_guildManager.getBySerial(m_aBattleGuild[1].m_wSerial);

	if	(lpGuild1 == NULL ||  lpGuild2 == NULL)
	{
		g_game.sendErrorOfGuildBattleLog(eGBW_ERROR_IN_KILL);

		return;
	}

	SG_GUILD_BATTLE_KILL_NOTICE	packet;

	packet.base.set(sizeof(packet),dSG_GUILD_BATTLE_KILL_NOTICE);

	packet.wKillerTeam		=	_lpKiller->m_wTeam;
	packet.wGetPoint		=	_lpTarget->m_iLevel;
	packet.iGuild1KillPoint	=	m_aBattleGuild[0].m_iKillPoint;
	packet.iGuild2KillPoint	=	m_aBattleGuild[1].m_iKillPoint;
	packet.wIsDuel			=	FALSE;

	strcpy(packet.strKiller,_lpKiller->m_strName);
	strcpy(packet.strTarget,_lpTarget->m_strName);

	addSendPacket((ALL_MSG*)&packet,0xffff);

	g_game.sendKillPlayerGuildBattleLog(lpGuild1->m_strName,lpGuild2->m_strName,_lpKiller->m_strName,_lpTarget->m_strName,_lpTarget->m_iLevel,m_aBattleGuild[0].m_iKillPoint,m_aBattleGuild[1].m_iKillPoint,_lpKiller->m_wTeam);
}

void
cFIELD::readyToGuildBattle()
{
	if	(m_bIsOperateGuildBattle	==	FALSE)	//	길드전 중이 아니다.
	{
		m_wIsIncorrectGuildBattle	=	TRUE;

		return;
	}

	m_wGuildBattleFinished	=	FALSE;

	CGuild	*lpGuild1		=	g_guildManager.getBySerial(m_aBattleGuild[0].m_wSerial);
	CGuild	*lpGuild2		=	g_guildManager.getBySerial(m_aBattleGuild[1].m_wSerial);

	if	(lpGuild1 == NULL ||  lpGuild2 == NULL)
	{
		m_wIsIncorrectGuildBattle	=	TRUE;

		return;
	}

	generateAllNPC();

	int	iGuild1MemberCount	=	getGuildMemberCount(m_aBattleGuild[0].m_wSerial);
	int	iGuild2MemberCount	=	getGuildMemberCount(m_aBattleGuild[1].m_wSerial);

	if	(iGuild1MemberCount	==	0	||	iGuild2MemberCount	==	0)
		m_wIsIncorrectGuildBattle	=	TRUE;
	else
		m_wIsIncorrectGuildBattle	=	FALSE;

	if	(iGuild1MemberCount + iGuild2MemberCount == 0)
	{
		g_game.sendWPGuildBattleResult(m_aBattleGuild[0].m_wSerial,m_aBattleGuild[1].m_wSerial,FALSE,FALSE);
		return;
	}

	if	(iGuild1MemberCount ==	0)
	{
		g_game.sendWPGuildBattleResult(m_aBattleGuild[0].m_wSerial,m_aBattleGuild[1].m_wSerial,FALSE,TRUE);
		return;
	}

	if	(iGuild2MemberCount ==	0)
	{
		g_game.sendWPGuildBattleResult(m_aBattleGuild[0].m_wSerial,m_aBattleGuild[1].m_wSerial,TRUE,FALSE);
		return;
	}

	m_aBattleGuild[0].m_iKillPoint	=	0;
	m_aBattleGuild[1].m_iKillPoint	=	0;

	{
		SG_GUILD_BATTLE_NOTICE	packet;

		packet.base.set(sizeof(packet),dSG_GUILD_BATTLE_NOTICE);
		packet.bRemainTimeToEndBattle	=	0xff;
		packet.bRemainTimeToBeginBattle	=	g_iGuildBattleReadyTime-g_iGuildBattleInitEnterenceTime;

		addSendPacket((ALL_MSG*)&packet,0xffff);
	}

	addSendGuildBattleInfo();
}

void
cFIELD::beginGuildBattle(int _iBattleTime)
{
	if	(m_wIsIncorrectGuildBattle	||	m_bIsOperateGuildBattle	==	FALSE)
	{
		endGuildBattle();

		return;
	}

	m_iRemainGuildBattleTime	=	_iBattleTime*60;

	{
		CGuild	*lpGuild1	=	g_guildManager.getBySerial(m_aBattleGuild[0].m_wSerial);
		CGuild	*lpGuild2	=	g_guildManager.getBySerial(m_aBattleGuild[1].m_wSerial);

		if	(lpGuild1 && lpGuild2)
		{
			int		iCount1	=	getGuildMemberCount(m_aBattleGuild[0].m_wSerial,TRUE);
			int		iCount2	=	getGuildMemberCount(m_aBattleGuild[1].m_wSerial,TRUE);

			if	(iCount1	==	0	||	iCount2	==	0)
			{
				m_wIsIncorrectGuildBattle	=	TRUE;
				m_bIsOperateGuildBattle		=	FALSE;

				return;
			}

			g_game.sendBeginGuildBattleLog(lpGuild1,lpGuild2,iCount1,iCount2);
		}
	}

	SG_GUILD_BATTLE_NOTICE	packet;

	packet.base.set(sizeof(packet),dSG_GUILD_BATTLE_NOTICE);
	packet.bRemainTimeToEndBattle	=	0xff;
	packet.bRemainTimeToBeginBattle	=	0xff;

	m_wGuildBattleFinished			=	FALSE;

	addSendPacket((ALL_MSG*)&packet,0);
}

//
//	길드전 종료
void
cFIELD::finishGuildBattle()
{
	if	(m_wGuildBattleFinished	||	m_bIsOperateGuildBattle	==	FALSE)
		return;

	if	(m_wIsIncorrectGuildBattle)
	{
		endGuildBattle();

		return;
	}

	CGuild	*lpGuild1	=	NULL;
	CGuild	*lpGuild2	=	NULL;

	{
		lpGuild1		=	g_guildManager.getBySerial(m_aBattleGuild[0].m_wSerial);
		lpGuild2		=	g_guildManager.getBySerial(m_aBattleGuild[1].m_wSerial);

		if	(!lpGuild1 || !lpGuild2)
		{
			g_game.sendErrorOfGuildBattleLog(eGBW_ERROR_IN_CHECK_RESULT);
		}
		else
		{
			int		iCount1		=	getGuildMemberCount(m_aBattleGuild[0].m_wSerial,TRUE);
			int		iCount2		=	getGuildMemberCount(m_aBattleGuild[1].m_wSerial,TRUE);

			cACTOR	*lpActor1	=	getTopLevelGuildMember(m_aBattleGuild[0].m_wSerial,TRUE);
			cACTOR	*lpActor2	=	getTopLevelGuildMember(m_aBattleGuild[1].m_wSerial,TRUE);

			char	*lpstrName1=NULL,*lpstrName2=NULL;

			if	(lpActor1)
				lpstrName1	=	lpActor1->m_strName;
			if	(lpActor2)
				lpstrName2	=	lpActor2->m_strName;

			if	(iCount1	==	0)
				m_aBattleGuild[0].m_iKillPoint	=	0;

			if	(iCount2	==	0)
				m_aBattleGuild[1].m_iKillPoint	=	0;

			g_game.sendPreResultOfGuildBattleLog(lpGuild1->m_strName,lpGuild2->m_strName,m_aBattleGuild[0].m_iKillPoint,m_aBattleGuild[1].m_iKillPoint,iCount1,iCount2,lpstrName1,lpstrName2);
		}
	}

	BOOL	bIsWinGuild1=FALSE,bIsWinGuild2=FALSE;

	if	(m_aBattleGuild[0].m_iKillPoint	>	m_aBattleGuild[1].m_iKillPoint)
		bIsWinGuild1	=	TRUE;

	if	(m_aBattleGuild[1].m_iKillPoint	>	m_aBattleGuild[0].m_iKillPoint)
		bIsWinGuild2	=	TRUE;

	if	(m_aBattleGuild[1].m_iKillPoint	==	m_aBattleGuild[0].m_iKillPoint)
	{
		bIsWinGuild1	=	TRUE;
		bIsWinGuild2	=	TRUE;
	}

	g_game.sendWPGuildBattleResult(m_aBattleGuild[0].m_wSerial,m_aBattleGuild[1].m_wSerial,bIsWinGuild1,bIsWinGuild2);


	{
		SG_GUILD_BATTLE_RESULT	packet;

		packet.wGuildSerial				=	0xffff;
		packet.base.set(sizeof(packet),dSG_GUILD_BATTLE_RESULT);

		if	(bIsWinGuild1 && bIsWinGuild2)
			strcpy(packet.strTargetGuild,"");
		else
		if	(bIsWinGuild1)
			strcpy(packet.strTargetGuild,lpGuild1->m_strName);
		else
		if	(bIsWinGuild2)
			strcpy(packet.strTargetGuild,lpGuild2->m_strName);

		for (int iPlayer=m_aiActorCount[0];iPlayer;iPlayer--)
		{
			cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[0][iPlayer-1]];

			if	(lpPlayer	==	NULL	||	lpPlayer->m_wOperatorLevel < 4)
				continue;

			g_userPM.add(lpPlayer->m_iClientSerial,&packet,sizeof(packet));
		}

	}

	if	(g_bIsTestGuildBattle)
	{
		m_wGuildBattleStatus	=	eGB_SLEEP_TIME;

		setBattleGuild(0xffff,0xffff);
	}

	m_wGuildBattleFinished		=	TRUE;
	m_bIsOperateGuildBattle		=	FALSE;
}

//
//	애들 전부 마을로 쫒아낸다.
void
cFIELD::endGuildBattle()
{
	for (int iTeam=0;iTeam<2;iTeam++)
		for (int iPlayer=m_aiActorCount[iTeam];iPlayer;iPlayer--)
		{
			cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[iTeam][iPlayer-1]];

			if	(lpPlayer	==	NULL	||	lpPlayer->isPlayer()	==	FALSE	||	lpPlayer->m_bf1IsMoveByEndOfGuildBattle || lpPlayer->m_wOperatorLevel >= 4)
				continue;

			lpPlayer->m_bf1IsMoveByEndOfGuildBattle	=	TRUE;
			lpPlayer->m_bf1IsPlayOnGuildBattle		=	FALSE;

			g_pMoveFieldUserManager->booking(lpPlayer,NULL,lpPlayer->m_wLastVillage,0xffff);
		}

	m_bIsOperateGuildBattle		=	FALSE;
	m_iRemainGuildBattleTime	=	-1;
}

//
//	길드 전투 상태를 변경
void
cFIELD::changeGuildBattleStatus(int _iStatus,int _iBattleOrder)
{
	m_wGuildBattleStatus	=	_iStatus;

	if	(m_bIsGuildBattleField	==	FALSE)
		return;

	if	(m_wDuelType	!=	0xffff)
		return;

	switch(m_wGuildBattleStatus)
	{
		case	eGB_INIT					:
			initGuildBattle(_iBattleOrder);
			break;

		case	eGB_ENTERENCE				:
			break;

		case	eGB_READY					:
		{
			readyToGuildBattle();
			break;
		}

		case	eGB_BATTLE					:
			beginGuildBattle();
			break;

		case	eGB_READY_TO_BATTLE_FINISH	:
		{
			SG_GUILD_BATTLE_NOTICE	packet;

			packet.base.set(sizeof(packet),dSG_GUILD_BATTLE_NOTICE);
			packet.bRemainTimeToEndBattle	=	1;
			packet.bRemainTimeToBeginBattle	=	0xff;

			addSendPacket((ALL_MSG*)&packet,0);
			break;
		}

		case	eGB_FINISH			:
			finishGuildBattle();
			break;

		case	eGB_END				:
//			endGuildBattle();
			break;
	}

	if	(_iStatus	==	eGB_BATTLE || _iStatus	==	eGB_FINISH	||	_iStatus	==	eGB_INIT)
	{
		int	iBattleOrder	=	_iBattleOrder;
		int	iLastOrder		=	dGUILD_BATTLE_COUNT;

		if	(_iStatus	!=	eGB_INIT)	//	다음 길드전의 길드 멤버들에게 공지를 보낸다.
			iBattleOrder++;

		if	(iBattleOrder < dGUILD_BATTLE_COUNT)	//	다음 길드전 할 멤버들에게 메시지 전송
		{
			CGuildBattleInfo	*lpBattleInfo	=	g_gbsToday.getBattleInfo(iBattleOrder,m_wGuildBattleFieldOrder);

			if	(lpBattleInfo)
			{
				CGuild	*lpGuild1		=	g_guildManager.getBySerial(lpBattleInfo->m_awBattleGuild[0]);
				CGuild	*lpGuild2		=	g_guildManager.getBySerial(lpBattleInfo->m_awBattleGuild[1]);

				if	(lpGuild1 && lpGuild2)
				{
					CTimeInfo	beginTime;
					g_gbsToday.getBeginTime(iBattleOrder,&beginTime,m_wGuildBattleFieldOrder);

					lpGuild1->sendReadyToNextGuildBattleMessageToMember(lpGuild2->m_strName,beginTime.m_wHour,beginTime.m_wMinute);
					lpGuild2->sendReadyToNextGuildBattleMessageToMember(lpGuild1->m_strName,beginTime.m_wHour,beginTime.m_wMinute);
				}
			}
		}
	}
}

//
//	길드전 로비에 있는 플레이어를 길드전 필드로 워프 시킨다.
void
cFIELD::warpGuildBattleLobbyPlayerToBattleField(int _iTimeOrder,int _iGuild1,int _iGuild2)
{
	int	i;

	if	(g_bIsTestGuildBattle)
	{
		for (i=m_pArea->m_wGuildBattleAreaCount;i;)
		{
			--i;

			int			iArea	=	m_pArea->m_awGuildBattleArea[i];
			cAreaInfo	*lpArea	=	m_pArea->getArea(iArea);
			cPOINT		posCenter;

			lpArea->getCenterPos(&posCenter);

			for (int iPlayer=m_aiActorCount[0];iPlayer;)
			{
				--iPlayer;

				cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[0][iPlayer]];

				if	(_iGuild1	!=	-1)
				{
					if	(lpPlayer->m_wGuildSerial	!=	_iGuild1	&&	lpPlayer->m_wGuildSerial	!=	_iGuild2)
						continue;
				}

				int	iRange	=	GetOvalRange(lpPlayer->m_pos.x,lpPlayer->m_pos.y,posCenter.x,posCenter.y);

				if	(iRange	>	dGUILD_BATTLE_LOBBY_WARP_RANGE*dGUILD_BATTLE_LOBBY_WARP_RANGE)
					continue;
				if	(!lpPlayer->isPlayer())
					continue;
				if	(lpPlayer->m_bf1IsPlayOnGuildBattle			)
					continue;
				if	(lpPlayer->m_wGuildSerial	==	0xffff)
					continue;
				if	(lpPlayer->getPartySerial()	!=	0xffff			)
				{
					lpPlayer->sendRegistServerMessage(eRSM_PLEASE_RELEASE_PARTY_FOR_GUILD_BATTLE,eSM_SYSTEM);

					continue;
				}

				cFIELD	*lpField	=	g_game.getFieldByUniqueSerial(_iTimeOrder);

				if	(!lpField	||	lpField->m_bIsGuildBattleField	==	FALSE)
					continue;

				lpPlayer->m_bf1IsPlayOnGuildBattle			=	TRUE;	//	길드전 중이다
				lpPlayer->m_bf5GuildBattleDay				=	g_currentTime.m_wDay;	//	길드전 입장한 날
				lpField->m_wGuildBattleStatus				=	eGB_ENTERENCE;
				g_pMoveFieldUserManager->booking(lpPlayer,NULL,_iTimeOrder,0);
			}
		}

		return;
	}

	for (int iField=0;iField<dGUILD_BATTLE_COUNT_IN_SAME_TIME;iField++)
	{
		if	(g_gbsToday.getCurrentBattleStatus(iField)	!=	eGB_ENTERENCE	)
			continue;

		int	iTimeOrder	=	g_gbsToday.getCurrentBattleOrder(iField);

		CGuildBattleInfo*	lpInfo	=	&g_gbsToday.m_aBattleGuildSchedule[iTimeOrder].m_aBattleGuildInfo[iField];

		for (i=m_pArea->m_wGuildBattleAreaCount;i;)
		{
			--i;

			int			iArea	=	m_pArea->m_awGuildBattleArea[i];
			cAreaInfo	*lpArea	=	m_pArea->getArea(iArea);
			cPOINT		posCenter;

			lpArea->getCenterPos(&posCenter);

			for (int iPlayer=m_aiActorCount[0];iPlayer;)
			{
				--iPlayer;

				cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[0][iPlayer]];

				if	(!lpPlayer->isPlayer())
					continue;
				if	(lpPlayer->m_bf1IsPlayOnGuildBattle			)
					continue;
				if	(!lpInfo->isBattleGuild(lpPlayer->m_wGuildSerial)				)
					continue;

				int	iRange	=	GetOvalRange(lpPlayer->m_pos.x,lpPlayer->m_pos.y,posCenter.x,posCenter.y);

				if	(iRange	>	dGUILD_BATTLE_LOBBY_WARP_RANGE*dGUILD_BATTLE_LOBBY_WARP_RANGE)
					continue;

				if	(lpPlayer->getPartySerial()	!=	0xffff)
				{
					lpPlayer->sendRegistServerMessage(eRSM_PLEASE_RELEASE_PARTY_FOR_GUILD_BATTLE);

					continue;
				}

				lpPlayer->m_bf1IsPlayOnGuildBattle			=	TRUE;	//	길드전 중이다
				lpPlayer->m_bf5GuildBattleDay				=	g_currentTime.m_wDay;	//	길드전 입장한 날

				g_pMoveFieldUserManager->booking(lpPlayer,NULL,lpInfo->m_wFieldSerial,0);
			}
		}
	}
}

//
//	전멸한 길드가 없나 체크
void
cFIELD::checkExterminatedGuild()
{
	if	(m_bIsGuildBattleField	==	FALSE)
		return;

	if	(m_wGuildBattleStatus	!=	eGB_BATTLE)
		return;

	if	(m_wGuildBattleFinished)
		return;

	if	(m_wDuelType	!=	0xffff)
		return;

	CGuild	*lpGuild1	=	g_guildManager.getBySerial(m_aBattleGuild[0].m_wSerial);
	CGuild	*lpGuild2	=	g_guildManager.getBySerial(m_aBattleGuild[1].m_wSerial);

	if	(!lpGuild1 || !lpGuild2)
		return;

	int	iGuild1MemberCount	=	getGuildMemberCount(m_aBattleGuild[0].m_wSerial,TRUE);
	int	iGuild2MemberCount	=	getGuildMemberCount(m_aBattleGuild[1].m_wSerial,TRUE);

	if	(iGuild1MemberCount	==	0	||	iGuild2MemberCount	==	0)
	{
/*		if	(m_wWaitToExterminatedGuildStatusTime)
		{
			m_wWaitToExterminatedGuildStatusTime--;

			if	(m_wWaitToExterminatedGuildStatusTime)
				return;
		}
		else
		{
			m_wWaitToExterminatedGuildStatusTime	=	60;

			return;
		}
*/
		int	iWinner;

		if	(iGuild1MemberCount	!=	iGuild2MemberCount)
		{
			if	(iGuild1MemberCount	==	0)
				iWinner			=	1;
			else
				iWinner			=	0;

			m_aBattleGuild[iWinner].m_iKillPoint	=	10000;
			m_aBattleGuild[1-iWinner].m_iKillPoint	=	0;
		}
		else
		{
			if	(m_aBattleGuild[0].m_iKillPoint	>	m_aBattleGuild[1].m_iKillPoint)
				iWinner	=	0;
			else
			if	(m_aBattleGuild[0].m_iKillPoint	<	m_aBattleGuild[1].m_iKillPoint)
				iWinner	=	1;
			else
				iWinner	=	2;
		}

		g_game.sendExterminateGuildBattleLog(lpGuild1->m_strName,lpGuild2->m_strName,iWinner);

		finishGuildBattle();

		m_wGuildBattleFinished	=	TRUE;

		return;
	}

	m_wWaitToExterminatedGuildStatusTime	=	0;
}

CGuild*
cFIELD::getHallOwnGuild()
{
	if	(m_wIsGuildHall	==	FALSE)
		return	NULL;

	return	g_guildManager.getBySerial(m_wHallOwnGuild);
}


//
//	애들 전부 마을로 쫒아낸다.
void
cFIELD::banishGuildHallPlayer()
{
	int	iTeam	=	0;

	for (int iPlayer=m_aiActorCount[iTeam];iPlayer;iPlayer--)
	{
		cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[iTeam][iPlayer-1]];

		if	(lpPlayer	==	NULL	||	lpPlayer->isPlayer()	==	FALSE	||	lpPlayer->m_bf1IsMoveByChangeGuildHallOwner)
			continue;

		lpPlayer->m_bf1IsMoveByChangeGuildHallOwner	=	TRUE;

		g_pMoveFieldUserManager->booking(lpPlayer,NULL,lpPlayer->m_wLastVillage,0x7ffe);	//	마을의 길드홀 입구로 보낸다
	}
}

//
//	길드 가디언 수 얻기
int
cFIELD::getGuildGuardianCount()
{
	int	iTeam	=	0,iCount	=	0;

	for (int iPlayer=m_aiActorCount[iTeam];iPlayer;iPlayer--)
	{
		cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[iTeam][iPlayer-1]];

		if	(lpPlayer	==	NULL)
			continue;

		if	(lpPlayer->m_iGuildGuardianType)
			iCount++;
	}

	return	iCount;
}

//	_iRange안에 길드 가디언 찾기
cACTOR*
cFIELD::findNearestGuildGuardian(int _iX,int _iY,int _iRange)
{
	cACTOR*	lpGuardian	=	NULL;
	int		iTeam		=	0,iCount	=	0;
	int		iMinRange	=	0x7fffffff;
	_iRange				*=	_iRange;

	for (int iPlayer=m_aiActorCount[iTeam];iPlayer;iPlayer--)
	{
		cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[iTeam][iPlayer-1]];

		if	(lpPlayer	==	NULL	||	lpPlayer->m_iGuildGuardianType	==	0)
			continue;

		int	iCurRange	=	GetRange(_iX,_iY,lpPlayer->m_pos.x,lpPlayer->m_pos.y);

		if	(iCurRange	<	_iRange	&&	iCurRange	<	iMinRange)
		{
			lpGuardian	=	lpPlayer;
			iMinRange	=	iCurRange;
		}
	}

	return	lpGuardian;
}

BOOL
cFIELD::checkInsightGuildGuardian(int _iCheckRange,int _iX,int _iY)
{
	int	iTeam		=	0;
	int	iCheckRange	=	_iCheckRange*_iCheckRange;

	for (int iPlayer=m_aiActorCount[iTeam];iPlayer;iPlayer--)
	{
		cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[iTeam][iPlayer-1]];

		if	(lpPlayer	==	NULL)
			continue;

		if	(lpPlayer->m_iGuildGuardianType	==	0)
			continue;

		if	(GetOvalRange(_iX,_iY,lpPlayer->m_posFeignDeath.x,lpPlayer->m_posFeignDeath.y)	<=	iCheckRange)
			return	TRUE;
	}

	return	FALSE;
}