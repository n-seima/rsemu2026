#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CSkill.H"
#include "cMESSAGE.H"
#include "CTitle.H"
#include "CWindowInterface.H"
#include "CGuild.H"
#include "CGuildSkill.H"
#include "CCompress.H"

//
//	길드 기본 정보
void
cPACKET_HANDLER::ReceiveBaseGuildInfo(SERVER_PACKETS *_lpPacket)
{
	SG_BASE_GUILD_INFO		*lpPacket	=	&_lpPacket->mSG_BASE_GUILD_INFO;

	g_hero.m_bf1IsWaitGuildInfo	=	FALSE;

	if	(lpPacket->wGuildSerial	==	0xffff)
	{
		g_hero.m_wGuildSerial	=	0xffff;

		return;
	}

	if (lpPacket->base.wSize	==	sizeof(SG_BASE_GUILD_INFO))
		memcpy(&g_guild,&lpPacket->guildInfo,sizeof(CGuildBaseInfo));
	else
		memcpy(&g_guild,&lpPacket->guildInfo,sizeof(CGuildBaseInfo)-sizeof(g_guild.m_abGuildSkill));

	strcpy(g_hero.m_strGuildName,g_guild.m_strName);

	g_guildManager.setGuildMark(g_guild.m_wSerial,&g_guild.m_guildMarkInfo,g_guild.m_bf1IsTwinkle,g_guild.m_wLevel,g_guild.m_wHallLevel);

	if (g_hero.m_wGuildRank	==	0xffff	||	g_hero.m_wGuildRank	==	0	)
	{
		if (STRICMP(g_guild.m_strMasterName,g_hero.m_strName) == 0)
			g_hero.m_wGuildRank	=	dGUILD_CLASS_MASTER;
		else
			g_hero.m_wGuildRank	=	dGUILD_CLASS_ROOKIE;
	}

	g_gwGuild.resetSkillPlusButtonsPos();

	g_hero.buildPower();
}

void
cPACKET_HANDLER::ReceiveSimpleGuildInfo(SERVER_PACKETS *_lpPacket)
{
	SG_SIMPLE_GUILD_INFO	*lpPacket	=	(SG_SIMPLE_GUILD_INFO	*)_lpPacket;

	g_hero.m_bf1IsWaitSimpleGuildInfo	=	FALSE;

	if	(lpPacket->wGuildSerial	==	0xffff)
	{
		g_hero.m_wGuildSerial	=	0xffff;

		g_hero.buildPower();

		return;
	}

	{
		memset(g_map.m_object.m_abGuildStatueStatus,0,sizeof(g_map.m_object.m_abGuildStatueStatus));

		for	(int i=0;i<6;i++)
		{
			for	(int iParts=0;iParts<5;iParts++)
			{
				int	iMask	=	1<<iParts;

				if	((lpPacket->abGuildStatueStatus[i]	&	iMask)	==	iMask)
					g_map.m_object.m_abGuildStatueStatus[i][iParts]	=	TRUE;
			}
		}
	}

	g_guild.m_wSerial	=	lpPacket->wGuildSerial;
	memcpy(&g_guild.m_guildMarkInfo,&lpPacket->guildMarkInfo,sizeof(CGuildMarkInfo));
	strcpy(g_guild.m_strName,lpPacket->strGuildName);

	g_guild.m_wHallLevel	=	lpPacket->bf3HallLevel;
	g_guild.m_wLevel		=	lpPacket->bf8Level;
	g_guild.m_bf1IsTwinkle	=	lpPacket->bf1IsTwinkle;

	memset(g_guild.m_abGuildSkill,0,sizeof(g_guild.m_abGuildSkill));

	{
		for (int i=0;i<lpPacket->wGuildSkillCount;i++)
			g_guild.m_abGuildSkill[i]	=	lpPacket->abGuildSkill[i];
	}

	strcpy(g_hero.m_strGuildName,g_guild.m_strName);

	g_guildManager.setGuildMark(g_guild.m_wSerial,&g_guild.m_guildMarkInfo,g_guild.m_bf1IsTwinkle,g_guild.m_wLevel,g_guild.m_wHallLevel);
	g_guild.operateGuildSkillData();

	g_gwGuild.resetSkillPlusButtonsPos();

	g_hero.buildPower();
}

//
//	멤버를 위한 길드전 정보
void
cPACKET_HANDLER::ReceiveGuildBattleInfoForMember(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_GUILD_BATTLE_INFO_FOR_MEMBER		*lpPacket	=	(SG_UPDATE_GUILD_BATTLE_INFO_FOR_MEMBER		*)_lpPacket;

	memcpy(&g_guild.m_battleInfo,&lpPacket->data,sizeof(CGuildBattleInfoForMember));
}

void
cPACKET_HANDLER::ReceiveGuildMarkInfo(SERVER_PACKETS *_lpPacket)
{
	SG_GUILD_MARK_INFO	*lpPacket	=	(SG_GUILD_MARK_INFO *)_lpPacket;

	g_guildManager.setGuildMark(lpPacket->wGuildSerial,&lpPacket->markInfo,lpPacket->bf1IsTwinkle,lpPacket->bf8Level,lpPacket->bf3HallLevel);
}

void
cPACKET_HANDLER::ReceiveOpenGuildHallList(SERVER_PACKETS *_lpPacket)
{
	SG_OPEN_GUILD_HALL_LIST	*lpPacket	=	(SG_OPEN_GUILD_HALL_LIST*)_lpPacket;

	g_gwGuildHallList.open(lpPacket->abBuffer,lpPacket->wCount);
}

void
cPACKET_HANDLER::ReceiveOpenRelatedPlaceList(SERVER_PACKETS *_lpPacket)
{
	SG_OPEN_RELATED_PLACE_LIST	*lpPacket	=	(SG_OPEN_RELATED_PLACE_LIST*)_lpPacket;

	g_gwGuildRelatedPlaceList.open(lpPacket->awPlaceList,lpPacket->wCount);
}

void
cPACKET_HANDLER::ReceiveGuildHonorPointList(SERVER_PACKETS *_lpPacket)
{
	g_gwHonorPointList.open((void *)_lpPacket);
}

void
cPACKET_HANDLER::ReceiveGVGWorkSuccess(SERVER_PACKETS *_lpPacket)
{
	SG_GVG_WORK_SUCCESS	*lpPacket	=	(SG_GVG_WORK_SUCCESS	*)_lpPacket;

	if	(lpPacket->wIsTransAvatar)
		g_msgBox.cPopup("",_ms(dMSG_COMPLETE_TRANS_AVATAR_TO_GVG_SERVER,lpPacket->strName),dMSG_OK);
	else
		g_msgBox.cPopup("",_ms(dMSG_COMPLETE_TRANS_GUILD_TO_GVG_SERVER,lpPacket->strName),dMSG_OK);
}

void
cPACKET_HANDLER::ReceiveGuildMasterGraceInfo(SERVER_PACKETS *_lpPacket)
{
	SG_GUILD_MASTER_GRACE_INFO	*lpPacket	=	(SG_GUILD_MASTER_GRACE_INFO*)_lpPacket;

	switch(lpPacket->wWork)
	{
	case eGMG_ACTIVE_GUILD_MASTER_GRACE			:			//길드 마스터 은총 발동.
		CGamePlay::Notice(dMSG_GUILD_MASTER_GRACE_ACTIVE_2HOUR);
		break;
	case eGMG_ACTIVE_GUILD_MASTER_GRACE_END		:			//길드 마스터 은총 종료
		CGamePlay::Notice(dMSG_GUILD_MASTER_GRACE_END);
		break;
	case eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR1	:
		CGamePlay::Notice(dMSG_GUILD_MASTER_GRACE_ERROR1_ONLY_MASTER_USE);
		break;
	case eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR2	:
		CGamePlay::Notice(dMSG_GUILD_MASTER_GRACE_ERROR2_ONLY_USE_ONCE_IN_WEEK);
		break;
	case eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR3	:
		CGamePlay::Notice(dMSG_GUILD_MASTER_GRACE_ERROR3_LOW_GUILD_HOLL_LEVEL);
		break;
	case eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR4	:
		CGamePlay::Notice(dMSG_GUILD_MASTER_GRACE_ERROR4_UNKNOWEN);
		break;

	}
}

void
cPACKET_HANDLER::ReceiveOpenGuildInventory(SERVER_PACKETS *_lpPacket)
{
	SG_OPEN_GUILD_INVENTORY	*lpPacket	=	(SG_OPEN_GUILD_INVENTORY*)_lpPacket;

	memcpy(g_guild.m_aiWithdrawGoldLimitPerDay	,lpPacket->aiWithdrawGoldLimitPerDay	,sizeof(lpPacket->aiWithdrawGoldLimitPerDay));
	memcpy(g_guild.m_aWithdrawItemCountPerDay	,lpPacket->aWithdrawItemCountPerDay		,sizeof(lpPacket->aWithdrawItemCountPerDay));
	memcpy(g_guild.m_abInventorySize			,lpPacket->abInventorySize				,sizeof(lpPacket->abInventorySize));

	g_guild.m_llGuildGold	=	lpPacket->llGuildGold;

	g_gwGuildInventory.open(lpPacket->aItems,lpPacket->wCount);
}

void
cPACKET_HANDLER::ReceiveGuildGuardianAndPetExp(SERVER_PACKETS *_lpPacket)
{
	SG_GUILD_PET_AND_GUARDIAN_EXP	*lpPacket	=	(SG_GUILD_PET_AND_GUARDIAN_EXP*)_lpPacket;
	
	memcpy(g_guild.m_aGuildPet,lpPacket->aPetInfo,sizeof(g_guild.m_aGuildPet));
	memcpy(g_guild.m_aGuildGuardian,lpPacket->aGuardianInfo,sizeof(g_guild.m_aGuildGuardian));
}

void
cPACKET_HANDLER::ReceiveSiegeWarfareNotice(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperateSiegeWarfareNotice((SG_SIEGE_WARFARE_NOTICE	*)_lpPacket);
}

void
cPACKET_HANDLER::ReceiveStoreGuildInventoryItem(SERVER_PACKETS *_lpPacket)
{
	SG_STORE_GUILD_INVENTORY_ITEM			*lpPacket	=	(SG_STORE_GUILD_INVENTORY_ITEM			*)_lpPacket;

	g_guild.storeItem(lpPacket->wGuildInventorySlot,&lpPacket->item);
}

void
cPACKET_HANDLER::ReceiveRemoveGuildInventoryItem(SERVER_PACKETS *_lpPacket)
{
	SG_REMOVE_GUILD_INVENTORY_ITEM			*lpPacket	=	(SG_REMOVE_GUILD_INVENTORY_ITEM			*)_lpPacket;

	g_guild.removeItem(lpPacket->wGuildInventorySlot,&lpPacket->item);
}

void
cPACKET_HANDLER::ReceiveStoreGuildInventoryGold(SERVER_PACKETS *_lpPacket)
{
	SG_STORE_GUILD_INVENTORY_GOLD			*lpPacket	=	(SG_STORE_GUILD_INVENTORY_GOLD			*)_lpPacket;

	g_guild.storeGold(lpPacket->iGold);
}

void
cPACKET_HANDLER::ReceiveRemoveGuildInventoryGold(SERVER_PACKETS *_lpPacket)
{
	SG_REMOVE_GUILD_INVENTORY_GOLD			*lpPacket	=	(SG_REMOVE_GUILD_INVENTORY_GOLD			*)_lpPacket;

	g_guild.removeGold(lpPacket->iGold);
}

void
cPACKET_HANDLER::ReceiveChangeGuildInventoryItemPlace(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_GUILD_INVENTORY_ITEM_PLACE	*lpPacket	=	(SG_CHANGE_GUILD_INVENTORY_ITEM_PLACE	*)_lpPacket;

	g_guild.changeItemPlace(lpPacket->wSlot1,lpPacket->wSlot2);
}

//
//	길드 스케쥴러 오픈
void
cPACKET_HANDLER::ReceiveOpenGuildScheduler(SERVER_PACKETS *_lpPacket)
{
	SG_OPEN_GUILD_BATTLE_SCHEDULER	*lpPacket	=	&_lpPacket->mSG_OPEN_GUILD_BATTLE_SCHEDULER;

	g_hero.m_bf1IsWaitGuildCommandResult	=	FALSE;

	g_gwGuildBattleScheduler.open(lpPacket->wBeginHour,lpPacket->wBeginMinute,lpPacket->wBattlePeriod,lpPacket->wBattleCount);
}

//	길드 마크 조합 인터페이스 오픈
void
cPACKET_HANDLER::ReceiveOpenGuildMarkComposer(SERVER_PACKETS *_lpPacket)
{
	SG_OPEN_GUILD_MARK_COMPOSER		*lpPacket	=	&_lpPacket->mSG_OPEN_GUILD_MARK_COMPOSER;

	g_gwGuildMarkComposer.open();
}

//
//	길드 스케쥴
void
cPACKET_HANDLER::ReceiveGuildBattleScheduleInSameTime(SERVER_PACKETS *_lpPacket)
{
	SG_GUILD_BATTLE_SCHEDULE_IN_SAME_TIME	*lpPacket	=	&_lpPacket->mSG_GUILD_BATTLE_SCHEDULE_IN_SAME_TIME;

	g_hero.m_bf1IsWaitGuildCommandResult	=	FALSE;

	g_gwGuildBattleScheduler.updateGuildBattleSchedule(lpPacket->wTimeOrder,&lpPacket->data,lpPacket->wYear,lpPacket->wMonth,lpPacket->wDay);
}


//
//	길드 스케쥴 변경됨
void
cPACKET_HANDLER::ReceiveChangeGuildSchedule(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_GUILD_BATTLE_SCHEDULE	*lpPacket	=	&_lpPacket->mSG_CHANGE_GUILD_BATTLE_SCHEDULE;

	g_hero.m_bf1IsWaitGuildCommandResult	=	FALSE;
	g_gwGuildBattleScheduler.updateGuildBattleSchedule(lpPacket->wTimeOrder,lpPacket->wFieldOrder,&lpPacket->info);
}

//
//	한주동안 싸운 횟수 업데이트
void
cPACKET_HANDLER::ReceiveUpdateGuildBattleCountAtThisWeek(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_GUILD_BATTLE_COUNT_AT_THIS_WEEK	*lpPacket	=	&_lpPacket->mSG_UPDATE_GUILD_BATTLE_COUNT_AT_THIS_WEEK;

	if (g_guild.m_wSerial	!=	lpPacket->wGuildSerial)
		return;
}

//
//	다음 길드전 준비 메시지 받음
void
cPACKET_HANDLER::ReceiveReadyToNextGuildBattle(SERVER_PACKETS *_lpPacket)
{
	SG_READY_TO_NEXT_GUILD_BATTLE	*lpPacket	=	&_lpPacket->mSG_READY_TO_NEXT_GUILD_BATTLE;

	char	strText[256];
	char	*lpstrTimeStr	=	cGAME_UTIL::GetTimeStringHM(lpPacket->bHour,lpPacket->bMinute);

	sprintf(strText,dMSG_READY_TO_NEXT_GUILD_BATTLE_FORM,lpstrTimeStr,lpPacket->strTargetGuildName);

	CGamePlay::Notice(strText);
}

//	길드전 관련 공지
void
cPACKET_HANDLER::ReceiveGuildBattleNotice(SERVER_PACKETS *_lpPacket)
{
	SG_GUILD_BATTLE_NOTICE			*lpPacket	=	(SG_GUILD_BATTLE_NOTICE	*)_lpPacket;
	char	strText[256];

	if	(lpPacket->bRemainTimeToBeginBattle	!=	0xff)
	{
		sprintf(strText,dMSG_REMAIN_TIME_TO_BEGIN_GUILD_BATTLE_FORM,lpPacket->bRemainTimeToBeginBattle);

		CGamePlay::Notice(strText);
	}
	else
	if	(lpPacket->bRemainTimeToEndBattle		!=	0xff)
	{
		sprintf(strText,dMSG_REMAIN_TIME_TO_END_GUILD_BATTLE_FORM,lpPacket->bRemainTimeToEndBattle);

		CGamePlay::Notice(strText);
	}
	else
		CGamePlay::Notice(dMSG_BEGUN_GUILD_BATTLE);
}

//	길드전 관련 공지
void
cPACKET_HANDLER::ReceiveGuildBattleKillNotice(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperateGuildBattleKillNotice((SG_GUILD_BATTLE_KILL_NOTICE*)_lpPacket);
}
//	길드전 관련 공지
void
cPACKET_HANDLER::ReceiveGuildBattleInfo(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperateGuildBattleInfo((SG_GUILD_BATTLE_INFO*)_lpPacket);
}

//	다음 길드전 준비 메시지 받음
void
cPACKET_HANDLER::ReceiveGuildBattleResult(SERVER_PACKETS *_lpPacket)
{
	SG_GUILD_BATTLE_RESULT	*lpPacket	=	&_lpPacket->mSG_GUILD_BATTLE_RESULT;

	if	(g_hero.m_wOperatorLevel == 4)
	{
		if	(lpPacket->strTargetGuild[0])
			g_msgBox.cPopup("",_ms(dMSG_WINNING_GUILD_FORM,lpPacket->strTargetGuild),dMSG_OK);
		else
			g_msgBox.cPopup("",dMSG_DRAW_GAME,dMSG_OK);

		return;
	}

	if	(g_hero.m_wGuildSerial	!=	lpPacket->battleResult.m_wGuildSerial)
		return;

	char	*lpstrResultStr		=	dMSG_DRAW;

	if	(lpPacket->battleResult.m_wResult	==	eGBR_WIN)
		lpstrResultStr	=	dMSG_WIN;
	if	(lpPacket->battleResult.m_wResult	==	eGBR_LOSE)
		lpstrResultStr	=	dMSG_LOSE;
	if	(lpPacket->battleResult.m_wResult	== eGBR_DRAW)
		lpstrResultStr	=	dMSG_DRAW;

	g_guild.m_iVictoryPoint		=	lpPacket->battleResult.m_iVictoryPoint;
	g_guild.m_wVictoryContinue	=	lpPacket->battleResult.m_wVictoryContinue;
	g_guild.m_wVictoryCount		=	lpPacket->battleResult.m_wVictoryCount;
	g_guild.m_wDefeatCount		=	lpPacket->battleResult.m_wDefeatCount;
	g_guild.m_wDrawCount		=	lpPacket->battleResult.m_wDrawCount;
	g_guild.m_wCancelCount		=	lpPacket->battleResult.m_wCancelCount;

	if	(s_bIsGuildBattleField	==	FALSE)
		CGamePlay::Notice(_ms(dMSG_GUILD_BATTLE_RESULT_BRIEFLY_TEXT_FORM,lpPacket->strTargetGuild,lpstrResultStr));
	else
	{
		cSTRING	string;

		string.Add(dMSG_GUILD_BATTLE_RESULT_TEXT_FORM,lpPacket->strTargetGuild,lpstrResultStr);
		string.Add("\n\n");

		if	(lpPacket->battleResult.m_wIncreaseVictoryPoint > 0)
			string.Add(dMSG_GUILD_BATTLE_GET_VICTORY_POINT_FORM,lpPacket->battleResult.m_wIncreaseVictoryPoint);

		if	(lpPacket->battleResult.m_wVictoryContinue >= 2)
			string.Add(dMSG_GUILD_BATTLE_CONTINUE_WIN_FORM,g_guild.m_wVictoryContinue);

		string.Add("\n");
		string.Add(dMSG_GUILD_BATTLE_RESULT_FORM,g_guild.m_wVictoryCount,g_guild.m_wDefeatCount,g_guild.m_wDrawCount,g_guild.m_iVictoryPoint);

		g_msgBox.cPopup(dMSG_GUILD_BATTLE_RESULT,string.String,dMSG_OK);
		g_msgBox.setButtonId(eMBI_RETURN_TO_THE_VILLAGE_BY_END_OF_GUILD_BATTLE);	//	길드전이 끝나서 마을로 돌아가기
	}
}

//
//	길드 맹글기 준비!!
void
cPACKET_HANDLER::ReceiveReadyToMakeGuild(SERVER_PACKETS *_lpPacket)
{
	SG_READY_TO_MAKE_GUILD	*lpPacket	=	&_lpPacket->mSG_READY_TO_MAKE_GUILD;

	g_hero.m_bf1IsWaitGuildCommandResult=	FALSE;

	g_gwMakeGuild.open((char *)lpPacket->strMemberList);
}

//
//	길드 만들기 결과 받기
void
cPACKET_HANDLER::ReceiveCreateGuildResult(SERVER_PACKETS *_lpPacket)
{
	SG_MAKE_GUILD_RESULT	*lpPacket	=	&_lpPacket->mSG_MAKE_GUILD_RESULT;

	g_hero.m_bf1IsWaitGuildCommandResult=	FALSE;

	switch(lpPacket->wResult)
	{
		case	eMGR_SUCCESS				:	//	오케바리~
			if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
				memset(g_hero.m_aWayPoint,0xff,sizeof(g_hero.m_aWayPoint));

			g_hero.m_bf1IsReadyToMakeGuild	=	FALSE;
			CGamePlay::AddSystemMessage(WHITE,dMSG_GUILD_CREATE_FORM,lpPacket->strGuildName);
			g_gwMakeGuild.close();
			break;
		case	eMGR_EXIST_SAME_NAME_GUILD	:	//	같은 이름의 길드가 있어
			g_hero.m_bf1IsReadyToMakeGuild	=	TRUE;
			CGamePlay::AddSystemMessage(WHITE,dMSG_GUILD_CREATE_ERROR_EXIST_SAME_NAME_GUILD);
			break;
		case	eMGR_NOT_READY_TO_MAKE_GUILD:	//	길드 만들 준비가 안됐어
			g_hero.m_bf1IsReadyToMakeGuild	=	TRUE;
			CGamePlay::AddSystemMessage(WHITE,dMSG_NOT_READY_TO_MAKE_GUILD);
			g_gwMakeGuild.close();
			break;
		case	eMGR_ALREADY_HAVEAGUILD		:	//	이미 길드에 속해 있다.
			g_hero.m_bf1IsReadyToMakeGuild	=	TRUE;
			CGamePlay::AddSystemMessage(WHITE,dMSG_GUILD_CREATE_ERROR_ALREADY_GUILD_MEMBER);
			g_gwMakeGuild.close();
			break;
		case	eMGR_LOW_POWER				:	//	길드 만들 능력이 안된다.
			g_hero.m_bf1IsReadyToMakeGuild	=	TRUE;
			CGamePlay::AddSystemMessage(WHITE,dMSG_GUILD_CREATE_ERROR_LOW_POW);
			g_gwMakeGuild.close();
			break;
		case	eMGR_FULL					:	//	더 이상 길드를 만들 수 없다.
			g_hero.m_bf1IsReadyToMakeGuild	=	TRUE;
			CGamePlay::AddSystemMessage(WHITE,dMSG_CAN_NOT_CREATE_MORE_GUILD);
			g_gwMakeGuild.close();
			break;
			
		case	eMGR_FAILED					:	//	걍 실패
			g_hero.m_bf1IsReadyToMakeGuild	=	TRUE;
			CGamePlay::AddSystemMessage(WHITE,dMSG_GUILD_CREATE_FAILED);
			break;
	}
}	//	cPACKET_HANDLER::ReceiveCreateGuildResult(SERVER_PACKETS *_lpPacket)

//
//	길드 해체
void
cPACKET_HANDLER::ReceiveGuildDissolution(SERVER_PACKETS *_lpPacket)
{
	SG_DISSOLUTION_GUILD	*lpPacket	=	&_lpPacket->mSG_DISSOLUTION_GUILD;

	CGamePlay::AddSystemMessage(LTYELLOW,dMSG_DISSOLUTION_GUILD);

	g_hero.m_bf1IsWaitGuildCommandResult=	FALSE;
	g_hero.m_wGuildSerial				=	0xffff;
	g_hero.m_wGuildRank					=	0;
	g_guild.reset();
}	//	cPACKET_HANDLER::ReceiveGuildDissolution(SERVER_PACKETS *_lpPacket)

//
//	길드 참가신청 결과
void
cPACKET_HANDLER::ReceiveAskJoinGuildResult(SERVER_PACKETS *_lpPacket)
{
	SG_ASK_JOIN_GUILD_RESULT	*lpPacket	=	&_lpPacket->mSG_ASK_JOIN_GUILD_RESULT;

	switch(lpPacket->wResult)
	{
		case	eAJGR_REQUIRE_CITIZEN_TITLE		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_AJGR_REQUIRE_CITIZEN_TITLE);
			break;

		case	eAJGR_ALREADY_GUILD_MEMBER		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_ALREADY_GUILD_MEMBER);
			break;

		case	eAJGR_TARGET_NOT_EXIST			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TARGET_NOT_EXIST);
			break;

		case	eAJGR_TARGET_NOT_GUILD_MASTER	:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TARGET_NOT_GUILD_MASTER);
			break;

		case	eAJGR_GUILD_NOT_EXIST			:
		{
			char	strGuildName[dGUILD_NAME_LENGTH];
			memcpy(strGuildName,g_hero.m_aWayPoint,dGUILD_NAME_LENGTH);

			g_msgBox.cPopup("",_ms(dMSG_CAN_NOT_FOUND_TRANS_GUILD_FOR_GVG,strGuildName),dMSG_OK);
			break;
		}

		case	eAJGR_TARGET_DENY_COMMUNITY		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TARGET_DENY_COMMUNITY_STATUS);
			break;

		case	eAJGR_TARGET_BUSY				:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TARGET_DENY_COMMUNITY_STATUS);
			break;

		case	eAJGR_GUILD_MEMBER_FULL			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FULL_MEMBER_GUILD);
			break;

		case	eAJGR_FAILED					:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FAILED_TO_JOIN_GUILD);
			break;

	}
}


//
//	길드 참가신청 결과
void
cPACKET_HANDLER::ReceiveAskJoinGuild(SERVER_PACKETS *_lpPacket)
{
	SG_ASK_JOIN_GUILD	*lpPacket	=	&_lpPacket->mSG_ASK_JOIN_GUILD;

	g_gwGuild.askJoinGuild(lpPacket->strAskerName,lpPacket->wAskerSerial,lpPacket->wLevel,lpPacket->wJob);
}

//
//	길드 가입
void
cPACKET_HANDLER::ReceiveJoinGuild(SERVER_PACKETS *_lpPacket)
{
	SG_JOIN_GUILD	*lpPacket	=	&_lpPacket->mSG_JOIN_GUILD;

	g_guild.reset();

	g_hero.m_wGuildSerial		=	lpPacket->wGuildSerial;	//	길드 시리얼
	g_hero.m_bf1IsWaitGuildInfo	=	FALSE;

	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
		memset(g_hero.m_aWayPoint,0xff,sizeof(g_hero.m_aWayPoint));

	s_agent.sendAskGuildInfo(TRUE);

	CGamePlay::AddSystemMessage(WHITE,dMSG_JOIN_GUILD_FORM,lpPacket->strGuildName);
}

//
//	길드 탈퇴
void
cPACKET_HANDLER::ReceiveLeaveGuild(SERVER_PACKETS *_lpPacket)
{
	SG_EXIT_GUILD	*lpPacket			=	&_lpPacket->mSG_EXIT_GUILD;

	g_hero.m_wGuildSerial				=	0xffff;	//	길드 시리얼
	g_hero.m_wGuildRank					=	0;
	g_hero.m_bf1IsWaitGuildCommandResult=	FALSE;
	g_lpHero->m_wGuildSerial			=	0xffff;
	g_guild.m_wSerial					=	0xffff;

	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
		memset(g_hero.m_aWayPoint,0xffffffff,sizeof(g_hero.m_aWayPoint));

	if	(lpPacket->wGuildSerial	==	0xffff)
		return;

	CGamePlay::AddSystemMessage(WHITE,dMSG_LEAVE_GUILD_FORM,lpPacket->strGuildName);
}


//
//	길드 레벨 업데이트
void
cPACKET_HANDLER::ReceiveUpdateGuildLevel(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_GUILD_LEVEL	*lpPacket	=	&_lpPacket->mSG_UPDATE_GUILD_LEVEL;

	int	iIncreaseGuildPoint	=	lpPacket->wGuildPoint-g_guild.m_wGuildPoint;

	g_guild.m_wLevel		=	lpPacket->wGuildLevel;
	g_guild.m_wExpTaxRate	=	lpPacket->wExpTaxRate;
	g_guild.m_wGuildPoint	=	lpPacket->wGuildPoint;

	CGamePlay::AddSystemMessage(WHITE,dMSG_GUILD_LEVEL_UP_FORM,iIncreaseGuildPoint);
}	//	cPACKET_HANDLER::ReceiveUpdateGuildLevel(SERVER_PACKETS *_lpPacket)

//
//
void
cPACKET_HANDLER::ReceiveGuildWorkMessage(SERVER_PACKETS *_lpPacket)
{
	SG_GUILD_WORK_MESSAGE	*lpPacket	=	&_lpPacket->mSG_GUILD_WORK_MESSAGE;

	g_hero.m_bf1IsWaitGuildCommandResult=	FALSE;

	switch(lpPacket->wMessage)
	{
		case	eGWM_LACK_INCREASE_MEMBER_COUNT_SKILL_LEVEL			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GWM_LACK_INCREASE_MEMBER_COUNT_SKILL_LEVEL);
			break;

		case	eGWM_ONLY_GUILD_MASTER_CAN_SELECT_GUILD_MARK		:
			g_msgBox.cPopup("",dMSG_GWM_ONLY_GUILD_MASTER_CAN_SELECT_GUILD_MARK,dMSG_OK);
			break;
		
		case	eGWM_YOU_ARE_ALREADY_GUILD_MEMBER		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_ALREADY_GUILD_MEMBER);
			break;

		case	eGWM_GUILD_BATTLE_SLEEP_TIME			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GUILD_BATTLE_SCHEDULE_CHANGE_RULE);
			break;
		
		case	eGWM_YOU_ARE_NOT_GUILD_LEADER			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_YOU_ARE_NOT_GUILD_LEADER);
			break;

		case	eGWM_TOO_LONG_GUILD_NOTICE_LENGTH		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TOO_LONG_GUILD_NOTICE_LENTH);
			break;

		case	eGWM_YOU_ARE_NOT_GUILD_MASTER			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_YOU_ARE_NOT_GUILD_MASTER);
			break;

		case	eGWM_INCORRECT_MEMBER_NAME_LENGTH		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_INCORRECT_GUILD_MEMBER_NAME_LENGTH);
			break;

		case	eGWM_USER_NOT_GUILD_MEMBER				:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_USER_NOT_GUILD_MEMBER);
			break;

		case	eGWM_USER_NOT_SUB_GUILD_MASTER			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_USER_NOT_SUB_GUILD_MASTER);
			break;

		case	eGWM_GUILD_MASTER_CAN_NOT_LEAVE_GUILD	:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GUILD_MASTER_CAN_NOT_LEAVE_GUILD);
			break;
			
		case	eGWM_USER_ALREADY_SUB_GUILD_MASTER		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_USER_ALREADY_SUB_GUILD_MASTER);
			break;

		case	eGWM_COMPLETE_ASK_WORK					:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_COMPLETE_ASK_GUILD_WORK);
			break;

		case	eGWM_FAILED_ASK_WORK					:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FAILED_ASK_GUILD_WORK);
			break;

		case	eGWM_YOU_ARE_NOT_READY_TO_MAKE_GUILD	:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_YOU_ARE_NOT_READY_TO_MAKE_GUILD);
			break;

		case	eGWM_INCORRECT_GUILD_NAME				:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_INCORRECT_GUILD_NAME);
			break;

		case	eGWM_YOU_ARE_CAN_NOT_CHANGE_SKILL_LEVEL	:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GWM_YOU_ARE_CAN_NOT_CHANGE_SKILL_LEVEL);
			break;

		case	eGWM_INCORRECT_SKILL_INDEX				:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GWM_INCORRECT_SKILL_INDEX);
			break;

		case	eGWM_CAN_NOT_INCREASE_LEVEL_SKILL		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GWM_CAN_NOT_INCREASE_LEVEL_SKILL);
			break;

		case	eGWM_NOT_REMAIN_GUILD_SKILL_POINT		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GWM_NOT_REMAIN_GUILD_SKILL_POINT);
			break;

		case	eGWM_USER_ALREADY_GUILD_CONGRESS		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GWM_USER_ALREADY_GUILD_CONGRESS);
			break;

		case	eGWM_USER_NOT_GUILD_CONGRESS			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GWM_USER_NOT_GUILD_CONGRESS);
			break;

		case	eGWM_TOO_MANY_SUB_GUILD_MASTER			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GWM_TOO_MANY_SUB_GUILD_MASTER);
			break;

		case	eGWM_MEMBER_COUNT_NOT_ONE_AT_DISSOLUTION_GUILD	:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_DISSOLUTION_GUILD_CONDITION_COMMENT);
			break;

		case	eGWM_TOO_MANY_GUILD_CONGRESS			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GWM_TOO_MANY_GUILD_CONGRESS);
			break;
			
		case	eGWM_LEAVE_GUILD_LIMIT_BY_JOIN_DATE		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GWM_LEAVE_GUILD_LIMIT_BY_JOIN_DATE);
			break;

		case	eGWM_JOIN_GUILD_LIMIT_BY_JOIN_DATE		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_GWM_JOIN_GUILD_LIMIT_BY_JOIN_DATE);
			break;

		case	eGWM_INCORRECT_TAX_RATE					:
		{
			char	strText[256];
			sprintf(strText,dMSG_INCORRECT_TAX_RATE_FORM,lpPacket->wValue1/100,lpPacket->wValue1%100,lpPacket->wValue2/100,lpPacket->wValue2%100);
			g_msgBox.cPopup("",strText,dMSG_OK);
			break;
		}

		case	eGWM_CHANGE_RANK_IN_GUILD				:
		{
			CGamePlay::AddSystemMessage(WHITE,dMSG_GWM_CHANGE_GUILD_RANK_FORM,lpPacket->strName,g_pstrGuildRank[lpPacket->wValue1]);
			break;
		}
		
		case	eGWM_CAN_NOT_BANISH_GUILD_MASTER		:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_BANISH_GUILD_MASTER,lpPacket->strName,g_pstrGuildRank[lpPacket->wValue1]);
			break;
		}
		case	eGWM_CAN_NOT_BANISH_GUILD_ELDER			:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_BANISH_GUILD_ELDER,lpPacket->strName,g_pstrGuildRank[lpPacket->wValue1]);
			break;
		}
		case	eGWM_CAN_NOT_BANISH_GUILD_CONGRESS		:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_BANISH_GUILD_MEMBER_BY_SUB_MASTER,lpPacket->strName,g_pstrGuildRank[lpPacket->wValue1]);
			break;
		}
	}
}

void
cPACKET_HANDLER::ReceiveChangeGuildExpTax(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_GUILD_TAX_RATE	*lpPacket	=	&_lpPacket->mSG_CHANGE_GUILD_TAX_RATE;

	CGamePlay::AddSystemMessage(WHITE,dMSG_CHANGE_GUILD_EXP_TAX_FORM,lpPacket->wTaxRate/100,lpPacket->wTaxRate%100);

	g_guild.m_wExpTaxRate				=	lpPacket->wTaxRate;
	g_hero.m_bf1IsWaitGuildCommandResult=	FALSE;
}	//	cPACKET_HANDLER::ReceiveChangeGuildExpTax(SERVER_PACKETS *_lpPacket)

void
cPACKET_HANDLER::ReceiveChangeGuildNotice(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_GUILD_NOTICE		*lpPacket	=	&_lpPacket->mSG_CHANGE_GUILD_NOTICE;

	CGamePlay::AddSystemMessage(WHITE,dMSG_CHANGED_GUILD_NOTICE);

	strcpy(g_guild.m_strNotice,lpPacket->strGuildNotice);
}	//	cPACKET_HANDLER::ReceiveChangeGuildNotice(SERVER_PACKETS *_lpPacket)

void
cPACKET_HANDLER::ReceiveChangeRankInGuild(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_RANK_IN_GUILD			*lpPacket	=	&_lpPacket->mSG_CHANGE_RANK_IN_GUILD;

	g_hero.m_wGuildRank		=	lpPacket->wRank;

	int	iMemberIndex		=	g_guild.getMemberIndex(g_hero.m_strName);

	if	(iMemberIndex	!=	0xffff)
	{
		CSimpleGuildMemberInfo		*lpMember	=	g_guild.getMember(iMemberIndex);

		if	(lpMember)
			lpMember->m_bf4Rank	=	lpPacket->wRank;
	}
	g_hero.operateGuildSkillData();
	CGamePlay::AddSystemMessage(WHITE,dMSG_CHANGED_GUILD_RANK_FORM,g_pstrGuildRank[lpPacket->wRank]);
}	//	cPACKET_HANDLER::ReceiveChangeRankInGuild(SERVER_PACKETS *_lpPacket)

void
cPACKET_HANDLER::ReceiveGuildMemberList(SERVER_PACKETS *_lpPacket)
{
	SG_GUILD_MEMBER_LIST	*lpPacket	=	(SG_GUILD_MEMBER_LIST*)_lpPacket;

	if	(lpPacket->bf1IsAppend == FALSE)
	{
		g_guild.m_memberList.m_wCount		=	0;
		g_guild.m_wIsNotReceivedMemberList	=	FALSE;
		g_hero.m_bf1IsWaitGuildInfo			=	FALSE;
	}

	if	(lpPacket->bf15Count	==	0)
		return;

	memcpy(&g_guild.m_memberList.m_aMembers[g_guild.m_memberList.m_wCount],lpPacket->m_aMember,sizeof(CSimpleGuildMemberInfo)*lpPacket->bf15Count);

	memcpy(&g_guild.m_OriginmemberList.m_aMembers[g_guild.m_memberList.m_wCount],lpPacket->m_aMember,sizeof(CSimpleGuildMemberInfo)*lpPacket->bf15Count);

	g_guild.m_memberList.m_wCount	+=	lpPacket->bf15Count;

	g_guild.sortMemberList();

	g_gwGuild.changeTab();
}

//	길드 멤버 정보 업데이트
void
cPACKET_HANDLER::ReceiveUpdateGuildMemberInfo(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_GUILD_MEMBER	*lpPacket	=	(SG_UPDATE_GUILD_MEMBER *)_lpPacket;

	g_guild.updateMember(lpPacket->strMemberName,lpPacket->m_bf11Level,lpPacket->m_bf4Rank,lpPacket->m_bf8Job,lpPacket->m_bf1IsConnected,lpPacket->m_bf4ReservePet);

	g_guild.m_wMemberCount		=	lpPacket->bMemberCount;
	g_guild.m_wSubMasterCount	=	lpPacket->bSubMasterCount;
	g_guild.m_wCongressManCount	=	lpPacket->bCongressManCount;
	g_guild.m_memberList.m_wCount=	lpPacket->bMemberCount;
	g_hero.buildPower();
}
	
//	길드 멤버 제거
void
cPACKET_HANDLER::ReceiveRemoveGuildMember(SERVER_PACKETS *_lpPacket)
{
	SG_REMOVE_GUILD_MEMBER	*lpPacket	=	(SG_REMOVE_GUILD_MEMBER *)_lpPacket;

	g_guild.removeMember(lpPacket->strMemberName);

	g_guild.m_wMemberCount		=	lpPacket->bMemberCount;
	g_guild.m_wSubMasterCount	=	lpPacket->bSubMasterCount;
	g_guild.m_wCongressManCount	=	lpPacket->bCongressManCount;
	g_guild.m_memberList.m_wCount=	lpPacket->bMemberCount;

	if	(g_hero.isGuildLeader())
		CGamePlay::AddSystemMessage(LTYELLOW,dMSG_LEAVE_GUILD_MEMBER_FORM,lpPacket->strMemberName);
}

//
//	길드 스킬 레벨 업데이트
void
cPACKET_HANDLER::ReceiveUpdateGuildSkillLevel(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_GUILD_SKILL_LEVEL	*lpPacket	=	&_lpPacket->mSG_UPDATE_GUILD_SKILL_LEVEL;

	g_hero.m_bf1IsWaitGuildCommandResult	=	FALSE;

	if	(lpPacket->wSkill	==	0xffff)
	{
		memset(g_guild.m_abGuildSkill,0,sizeof(g_guild.m_abGuildSkill));
		memset(g_map.m_object.m_abGuildStatueStatus,0,sizeof(g_map.m_object.m_abGuildStatueStatus));
		CGamePlay::AddSystemMessage(LTYELLOW,"reset all guild skill");
	}
	else
	if	(lpPacket->wSkill	==	0xfffe)
	{
		memset(g_guild.m_abGuildSkill,0,c_iNormalGuildSkillCount);
		CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RESET_GUILD_SKILL_LEVEL);

		for (int i=0;i<g_guild.m_wMemberCount;i++)
		{
			if	(g_guild.m_memberList.m_aMembers[i].m_bf4Rank	==	dGUILD_CLASS_SUBMASTER	||	
				g_guild.m_memberList.m_aMembers[i].m_bf4Rank	==	dGUILD_CLASS_PARLIAMENT)
				g_guild.m_memberList.m_aMembers[i].m_bf4Rank	=	dGUILD_CLASS_MEMBER;
		}

		CSimpleGuildMemberInfo*	lpMember	=	g_guild.getMember(g_hero.m_strName);

		if	(lpMember)
			if	(lpMember->m_bf4Rank	==	dGUILD_CLASS_SUBMASTER	||	lpMember->m_bf4Rank	==	dGUILD_CLASS_PARLIAMENT)
				g_hero.m_wGuildRank	=	dGUILD_CLASS_MEMBER;
	}
	else
	if	(lpPacket->wSkill	==	0xfffd)
	{
		memset(g_guild.m_abGuildSkill,0,c_iNormalGuildSkillCount);
		g_guild.m_bf6PeneltyBonusGuildPoint	=	g_guild.m_bf6BonusGuildPoint;
		CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RESET_GUILD_SKILL_LEVEL);

		for (int i=0;i<g_guild.m_wMemberCount;i++)
		{
			if	(g_guild.m_memberList.m_aMembers[i].m_bf4Rank	==	dGUILD_CLASS_SUBMASTER	||	
				g_guild.m_memberList.m_aMembers[i].m_bf4Rank	==	dGUILD_CLASS_PARLIAMENT)
				g_guild.m_memberList.m_aMembers[i].m_bf4Rank	=	dGUILD_CLASS_MEMBER;
		}

		CSimpleGuildMemberInfo*	lpMember	=	g_guild.getMember(g_hero.m_strName);

		if	(lpMember)
			if	(lpMember->m_bf4Rank	==	dGUILD_CLASS_SUBMASTER	||	lpMember->m_bf4Rank	==	dGUILD_CLASS_PARLIAMENT)
				g_hero.m_wGuildRank	=	dGUILD_CLASS_MEMBER;
	}
	else
	{
		g_guild.m_abGuildSkill[lpPacket->wSkill]=	lpPacket->wLevel;

		if	(g_gsManager.m_aSkills[lpPacket->wSkill].m_bf1IsStatueSkill)
		{
			int	iSlot	=	(lpPacket->wSkill-100)/5;
			int	iParts	=	(lpPacket->wSkill-100)%5;

			if	(lpPacket->wLevel)
				g_map.m_object.m_abGuildStatueStatus[iSlot][iParts]	=	TRUE;
			else
				g_map.m_object.m_abGuildStatueStatus[iSlot][iParts]	=	FALSE;

			CGamePlay::AddSystemMessage(WHITE,dMSG_INCREASED_GUILD_STATUE_SKILL_LEVEL_FORM,g_gsManager.m_aSkills[lpPacket->wSkill].m_strName,lpPacket->wLevel);
		}
		else
			CGamePlay::AddSystemMessage(WHITE,dMSG_INCREASED_GUILD_SKILL_LEVEL_FORM,g_gsManager.m_aSkills[lpPacket->wSkill].m_strName,lpPacket->wLevel);
	}

	g_guild.m_wGuildPoint					=	lpPacket->wRemainSkillPoint;

	g_hero.buildPower();
	g_guild.operateGuildSkillData();
	g_gwGuild.resetSkillPlusButtonsPos();
}

//
//	길드 마크 변경
void
cPACKET_HANDLER::ReceiveChangeGuildMark(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_GUILD_MARK	*lpPacket	=	&_lpPacket->mSG_CHANGE_GUILD_MARK;

	g_hero.m_bf1IsWaitGuildCommandResult=	FALSE;

	CGuildMarkInfo			oldInfo;
	BOOL					bIsChangeGuildMarkShape	=	FALSE;
	BOOL					bIsOwnGuildMarkPack		=	FALSE;

	memcpy(&oldInfo,&g_guild.m_guildMarkInfo,sizeof(CGuildMarkInfo));
	memcpy(&g_guild.m_guildMarkInfo,&lpPacket->guildMarkInfo,sizeof(CGuildMarkInfo));

	if (oldInfo.m_wIcon					!=	g_guild.m_guildMarkInfo.m_wIcon				||
		oldInfo.m_bIconColor			!=	g_guild.m_guildMarkInfo.m_bIconColor		||
		oldInfo.m_bBaseShape			!=	g_guild.m_guildMarkInfo.m_bBaseShape		||
		oldInfo.m_bBaseColorPartition	!=	g_guild.m_guildMarkInfo.m_bBaseColorPartition||
		oldInfo.m_bBaseColor1			!=	g_guild.m_guildMarkInfo.m_bBaseColor1		||
		oldInfo.m_bBaseColor2			!=	g_guild.m_guildMarkInfo.m_bBaseColor2		||
		oldInfo.m_bBaseLineColor		!=	g_guild.m_guildMarkInfo.m_bBaseLineColor	)
		bIsChangeGuildMarkShape	=	TRUE;

	if (oldInfo.m_ownedMarkItem.m_dwValue	!=	g_guild.m_guildMarkInfo.m_ownedMarkItem.m_dwValue)
		bIsOwnGuildMarkPack		=	TRUE;

	if (!g_gwGuildMarkComposer.isOpened())
	{
		if (bIsChangeGuildMarkShape)
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CHANGE_GUILD_MARK);

		if (bIsOwnGuildMarkPack)
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_ADD_NEW_GUILD_MARK_PACK);
	}
	else
	{
		g_hero.m_bf1IsReadyToSelectGuildMark=	FALSE;

		if	(lpPacket->bf1IsUseGuildMarkPack	==	FALSE)
		{
			g_gwGuildMarkComposer.close();
			g_msgBox.cPopup("",dMSG_SELECT_GUILD_MARK_BY_YOU,dMSG_OK);
		}
		else
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_ADD_NEW_GUILD_MARK_PACK);
	}
}
