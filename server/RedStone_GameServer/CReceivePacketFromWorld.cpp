#include "cGAME.H"
#include "cFIELD.H"
#include "cPROJECT.H"
#include "STDAFX.H"
#include "cDECODER.H"
#include "time.H"
#include "cPACKET_GAMESERVER.h"
#include "cPACKET_USERDBSERVER.h"
#include "CImageData.h"
#include "Client.h"
#include "CGuild.h"
#include "instance_guild_battle_manager.h"
#include "debugCode.h"
#include "CEvent.h"
#include "itemPackList.h"
#include "secretDungeon.h"
#include "carrotShop.h"
#include "treasureMapDefine.h"
#include "packetManager.H"
#include "booking_work.H"
#include "cITEM.H"
#include "GameDLL.H"

void
cGAME::operateChangeDay()
{
	g_iBoostExpByTreasureMapEventCount	=	0;
	g_iBoostGoldByTreasureMapEventCount	=	0;
}

void
cGAME::operateSimpleWork(WORLDtoSERVERS_SIMPLE_WORK *_lpPacket)
{
	switch(_lpPacket->wWork)
	{
		case	eSWWG_RESET_GUILD_SIEGEWARFARE_INFO	:
			g_guildManager.resetGuildSiegewarfareInfo();
			break;

		case	eSWWG_RESET_1DAY_GUILD_INFO			:
			g_guildManager.reset1DayGuildInfo();
			break;
			
		case	eSWWG_RESET_1WEEK_GUILD_INFO		:
			g_guildManager.reset1WeekGuildInfo();
			break;
			


		case	eSWWG_UPDATE_GUILD_BATTLE_SCORE	:
		{
			CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->aiValue[0]);

			if	(lpGuild)
			{
				lpGuild->m_bf6LastGVGBattleYear		=	g_currentTime.m_wYear-2000;	//	마지막 길드전 한 날짜
				lpGuild->m_bf4LastGVGBattleMonth	=	g_currentTime.m_wMonth;
				lpGuild->m_bf5LastGVGBattleDay		=	g_currentTime.m_wDay;

				lpGuild->m_iVictoryPoint			=	_lpPacket->aiValue[1];
				lpGuild->m_wVictoryContinue			=	_lpPacket->aiValue[2];
				lpGuild->m_wVictoryCount			=	_lpPacket->aiValue[3];
				lpGuild->m_wDefeatCount				=	_lpPacket->aiValue[4];
				lpGuild->m_wDrawCount				=	_lpPacket->aiValue[5];
				lpGuild->m_wCancelCount				=	_lpPacket->aiValue[6];
				lpGuild->m_bf3ThisDayGuildBattleCount=	_lpPacket->aiValue[7];
				lpGuild->m_bf5ThisWeekGuildBattleCount=	_lpPacket->aiValue[8];
			}
		
			break;
		}

		case	eSWWG_SMS_ALARM_ERROR	:
		{
			SG_MESSAGE	packet;

			packet.base.set(sizeof(packet),dSG_MESSAGE);
#ifdef	_FOR_KOREA
			strcpy(packet.strMessage,"SMS 알람이 꺼져 있습니다.");
#else
			strcpy(packet.strMessage,"SMS Alarm off!!");
#endif
			packet.base.wSize	=	sizeof(packet) - dSERVER_MESSAGE_SIZE + strlen(packet.strMessage)+1;
			packet.wType		=	eSM_SYSTEM;

			sendPacketToAllOperator((char*)&packet);
			break;
		}

		case	eSWWG_RECOVER_BONUS_GUILD_POINT_PENELTY		:
		{
			CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->aiValue[0]);

			if	(lpGuild)
			{
				lpGuild->m_bf6BonusGuildPoint		=	_lpPacket->aiValue[1];
				lpGuild->m_bf6PeneltyBonusGuildPoint=	0;
				lpGuild->m_wGuildPoint				=	_lpPacket->aiValue[2];
				lpGuild->sendEtcWork(eEW_RECOVER_BONUS_GUILD_POINT_PENELTY,_lpPacket->aiValue[1]);
			}

			break;
		}
		case	eSWWG_INCREASE_BONUS_GUILD_POINT			:
		{
			CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->aiValue[0]);

			if	(lpGuild)
			{
				lpGuild->m_bf6BonusGuildPoint	=	_lpPacket->aiValue[1];
				lpGuild->m_wGuildPoint			=	_lpPacket->aiValue[2];
				
				lpGuild->sendEtcWork(eEW_INCREASE_BONUS_GUILD_POINT,_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
			}

			break;
		}
		case	eSWWG_CHANGE_TRY_GUILD_POINT_BATTLE_COUNT	:
		{
			CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->aiValue[0]);

			if	(lpGuild)
				lpGuild->m_bf2TryGuildPointBattleCountPerWeek	=	_lpPacket->aiValue[1];

			break;
		}
		case	eSWWG_CHANGE_GUILD_MARK_TWINKLE_STATUS		:
		{
			g_guildManager.changeGuildMarkTwinkleStatus(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			break;
		}
		case	eSWWG_RESET_GUILD_HONOR_POINT		:
		{
			g_guildManager.resetGuildHonorPoint();
			break;
		}
		case	eSWWG_RESET_GUILD_PROLOG_POINT		:
		{
			g_guildManager.resetGuildPrologPoint();
			break;
		}
		case	eSWWG_CHANGE_GUILD_HALL_OPEN_STATUS	:
		{
			g_guildManager.changeGuildHallOpenStatus(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			break;
		}
		case	eSWGW_ACTIVE_GUILD_MASTER_GRACE		:
		{
			g_guildManager.activeGuildMasterGrace(_lpPacket->aiValue[0] , _lpPacket->aiValue[1]);
			break;
		}
		case	eSWWG_UPDATE_GUILD_PET_EXP			:
		{
			DWORD	dwExp	=	_lpPacket->aiValue[2];
			dwExp			<<=	16;
			dwExp			+=	_lpPacket->aiValue[3];

			g_guildManager.updateGuildPetExp(_lpPacket->aiValue[0],_lpPacket->aiValue[1],dwExp,_lpPacket->aiValue[4]);
			break;
		}
		case	eSWWG_RENT_GUILD_PET				:
			g_guildManager.rentGuildPet(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
			break;

		case	eSWWG_RETRIEVAL_RENTED_GUILD_PET	:
			g_guildManager.retrievalGuildPet(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			break;

		case	eSWWG_NOTICE_BEGIN_SIEGE_WARFARE	:
			g_guildManager.noticeBeginSiegeWarfare(_lpPacket->aiValue[0]);
			//	길드 공성전 시작전 10분 공지. 이때부터 길드전 공성 필드 입장이 가능하다.
			break;
		case	eSWWG_NOTICE_END_SIEGE_WARFARE		:
			g_guildManager.noticeEndSiegeWarfare(_lpPacket->aiValue[0]);
			//	길드 공성전 끝나기 5분 공지.
			break;
		case	eSWWG_BEGIN_SIEGE_WARFARE			:
			g_guildManager.beginSiegeWarfare(_lpPacket->aiValue[0]);
			//	길드 공성전 시작. 잠긴 문을 연다.
			break;
		case	eSWWG_END_SIEGE_WARFARE			:
			g_guildManager.endSiegeWarfare(_lpPacket->aiValue[0]);
			//	길드 공성전 종료
			//	문을 잠그고 길드 문장을 리젠 시킨다.
			break;

		case	eSWWG_SIEGE_WARFARE_RESULT_SUCCESS_TO_DEFENSE	:
			g_guildManager.operateSiegeWarfareResultDefenseHall(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			//	
			break;

		case	eSWWG_SIEGE_WARFARE_RESULT_SUCCESS_TO_INVADE	:
			g_guildManager.operateSiegeWarfareResultCaptureHall(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			break;

		case	eSWWG_CONFIRM_USE_SOUND_OF_LEADERS_BELL	:
		{
			cACTOR	*lpActor	=	getActor(_lpPacket->aiValue[0]);

			if	(!lpActor)
				break;

			lpActor->sendEtcWork(eEW_CONFIRM_USE_SOUND_OF_LEADERS_BELL);

			break;
		}

		case	eSWWG_REQUIRE_MORE_TIME_FOR_USE_SOUND_OF_LEADERS_BELL	:
		{
			cACTOR	*lpActor	=	getActor(_lpPacket->aiValue[1]);

			if	(!lpActor)
				break;

			int		iRemainMinute=	_lpPacket->aiValue[0];
			int		iUpkeepMinute=	_lpPacket->aiValue[1];

			lpActor->sendEtcWork(eEW_REQUIRE_MORE_TIME_FOR_USE_SOUND_OF_LEADERS_BELL,iRemainMinute,iUpkeepMinute);

			break;
		}

		case	eSWWG_UPDATE_RED_STONE_COUNT	:
		{
			int	iIncreasePart	=	g_eventManager.setRedStonCount(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
			g_game.sendEtcWorkPacketToAllPlayer(eEW_UPDATE_RED_STONE_COUNT,iIncreasePart,_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2],1);
			break;
		}

		case	eSWWG_RESET_WEEKLY_DUEL_RECORD	:
		{
			g_duelRankerManager.setDailyRankerCount(0);	//	일간 랭킹 정보 초기화 하고

			SG_ETC_WORK	packet;

			packet.base.set(sizeof(packet),dSG_ETC_WORK);

			packet.wWork	=	eEW_RESET_WEEKLY_DUEL_RECORD;

			for (int i=0;i<m_iMaxActorCount;i++)	//	모든 플레이어의 주간 전적을 0으로 초기화 한다.
			{
				cACTOR	*lpActor	=	getActor(i);

				if	(!lpActor	||	lpActor->m_duelRecord.m_iWeeklyWinPoint	==	0)
					continue;

				g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
				
				lpActor->m_duelRecord.m_iWeeklyBattleCount	=	0;
				lpActor->m_duelRecord.m_iWeeklyWinPoint		=	0;		//	주간 결투 승점
				lpActor->m_duelRecord.m_iWeeklyBattleCount	=	0;	//	주간 결투 횟수
				lpActor->m_duelRecord.m_iWeeklyWinCount		=	0;		//	주간 결투 승리 횟수
				lpActor->m_duelRecord.m_iWeeklyDefeatCount	=	0;	//	주간 결투 패배 횟수
				lpActor->m_duelRecord.m_iWeeklyDrawCount	=	0;		//	주간 결투 무승부 횟수
			}

			break;
		}

		case	eSWWG_CHANGE_SYSTEM_TIME	:
		{
			SYSTEMTIME	sysTime;

			GetLocalTime(&sysTime);

			CTimeInfo	curTime;

			curTime.m_wYear		=	_lpPacket->aiValue[0];
			curTime.m_wMonth	=	_lpPacket->aiValue[1];
			curTime.m_wDay		=	_lpPacket->aiValue[2];

			sysTime.wYear		=	_lpPacket->aiValue[0];
			sysTime.wMonth		=	_lpPacket->aiValue[1];
			sysTime.wDay		=	_lpPacket->aiValue[2];
			sysTime.wHour		=	_lpPacket->aiValue[3];
			sysTime.wMinute		=	_lpPacket->aiValue[4];

			sysTime.wDayOfWeek	=	curTime.getDayOfWeek();

			SetLocalTime(&sysTime);

			{
				curTime.update();

				SG_MESSAGE	packet;

				packet.base.set(sizeof(packet),dSG_MESSAGE);

#ifdef	_FOR_KOREA
				sprintf(packet.strMessage,"시간 변경 : %d년 %.2d월 %.2d일 %.2d시 %.2d분",curTime.m_wYear,curTime.m_wMonth,curTime.m_wDay,curTime.m_wHour,curTime.m_wMinute);
#else
				sprintf(packet.strMessage,"change time : %d/%.2d/%.2d %.2d:%.2d",curTime.m_wYear,curTime.m_wMonth,curTime.m_wDay,curTime.m_wHour,curTime.m_wMinute);
#endif

				packet.base.wSize	=	sizeof(packet) - dSERVER_MESSAGE_SIZE + strlen(packet.strMessage)+1;
				packet.wType		=	eSM_NOTICE;

				g_game.sendPacketToAllOperator((char*)&packet);
			}
			break;
		}
	}
}

void
cGAME::operateDuelMatchInfo(WORLDtoSERVERS_DUEL_MATCH_INFO *_lpPacket)
{
	g_duelTrialGameManager.setTrialGameSchedule(_lpPacket->strTrialGameName,
												_lpPacket->wTrialGameBeginYear,_lpPacket->wTrialGameBeginMonth,_lpPacket->wTrialGameBeginDay,
												_lpPacket->wTrialGameBeginHour,_lpPacket->wTrialGameBeginMinute,
												_lpPacket->wTrialGameEndYear,_lpPacket->wTrialGameEndMonth,_lpPacket->wTrialGameEndDay,
												_lpPacket->wTrialGameEndHour,_lpPacket->wTrialGameEndMinute);

	if	(g_bIsDuelServer)
		if	(g_iTrialGameTeamSize	!=	_lpPacket->wTrialGameTeamSize)
			cMSG::Error("error!!","in change duel match info\n\nmistmatch team size info");

	g_iTrialGameTeamSize								=	_lpPacket->wTrialGameTeamSize;
	g_duelTrialGameManager.m_wIsSendTrialGameResultMail	=	_lpPacket->wIsSendTrialGameResultMail;

	g_duelTrialGameManager.setOfflineTournamentSchedule(_lpPacket->strOfflineTournamentComment,
		_lpPacket->wOfflineTournamentBeginYear,_lpPacket->wOfflineTournamentBeginMonth,_lpPacket->wOfflineTournamentBeginDay,_lpPacket->wOfflineTournamentBeginHour,_lpPacket->wOfflineTournamentBeginMinute,
		_lpPacket->wOfflineTournamentEndYear,_lpPacket->wOfflineTournamentEndMonth,_lpPacket->wOfflineTournamentEndDay,_lpPacket->wOfflineTournamentEndHour,_lpPacket->wOfflineTournamentEndMinute);
}

void
cGAME::operateDuelRecord(WORLDtoSERVERS_DUEL_RECORD *_lpPacket)
{
	cACTOR	*lpActor	=	getPlayerByID(_lpPacket->strId);

	if	(!lpActor)
		return;

	memcpy(&lpActor->m_duelRecord,&_lpPacket->record,sizeof(cDuelRecord));

	lpActor->sendDuelRecord();
}

void
cGAME::operateTransAvatarToGVGResult(WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT *_lpPacket)
{
	cACTOR	*lpPlayer	=	g_game.getPlayer(_lpPacket->strName);

	if	(!lpPlayer)	//	아 뭐 이런경우가.. ;;
		return;

	lpPlayer->operateTransToGVGResult(_lpPacket);
}

void
cGAME::operateW2G_GVG_AVATAR_ORG_DATA(W2G_GVG_AVATAR_ORG_DATA *_lpPacket)
{
//	printf("[GVG] receive W2G_GVG_AVATAR_ORG_DATA %s\n",_lpPacket->strName);

	cACTOR	*lpPlayer	=	getPlayer(_lpPacket->strName);

	if	(!lpPlayer)
		return;

	char				strName[dNAME_LENGTH];//이름
	WORD				wCurrentField;	//현재 필드
	int					iXPos,iYPos;
	WORD				wLastVillage;		//마지막으로 떠나온 마을
	WORD				wGuildSerial;
	WORD				wGuildRank;
	DWORD				dwSaveCount;
	UTime				timeGuildJoinTime;		//	길드 가입한 시간
	WORD				wAvatarIndex;				//	아바타 인덱스
	CPlayerWayPointInfo	aWayPoint[dMAX_PLAYER_WAY_POINT_COUNT];
	DWORD				dwPlayTime;		//	플레이 타임 분 단위
	WORD				wProcessChapterIndex;	//	진행 중인 메인 퀘스트 챕터 인덱스. GVG 서버 캐릭터는 이 정보가 1이면 이름에 '_' 가 안붙고 바로 알파벳이 붙은 경우다.
	WORD				wIsReadyToMakeGuild;

	//	덮어씌우면 안되는 데이터들 보존
	{
		memcpy(strName,lpPlayer->m_strName,sizeof(strName));
		memcpy(&timeGuildJoinTime,&lpPlayer->m_timeGuildJoinTime,sizeof(timeGuildJoinTime));		//	길드 가입한 시간
		memcpy(aWayPoint,lpPlayer->m_aWayPoint,sizeof(aWayPoint));

		wIsReadyToMakeGuild	=	lpPlayer->m_bf1IsReadyToMakeGuild;
		wCurrentField		=	lpPlayer->m_wCurrentField;	//현재 필드
		iXPos				=	lpPlayer->m_iXPos;
		iYPos				=	lpPlayer->m_iYPos;
		wLastVillage		=	lpPlayer->m_wLastVillage;		//마지막으로 떠나온 마을
		wGuildSerial		=	lpPlayer->m_wGuildSerial;
		wGuildRank			=	lpPlayer->m_wGuildRank;
		dwSaveCount			=	lpPlayer->m_dwSaveCount;
		wAvatarIndex		=	lpPlayer->m_wAvatarIndex;				//	아바타 인덱스
		dwPlayTime			=	lpPlayer->m_dwPlayTime;		//	플레이 타임 분 단위
		wProcessChapterIndex=	lpPlayer->m_wProcessChapterIndex;	//	진행 중인 메인 퀘스트 챕터 인덱스. GVG 서버 캐릭터는 이 정보가 1이면 이름에 '_' 가 안붙고 바로 알파벳이 붙은 경우다.
	}

	//	덥어 쓴다!
	memcpy(lpPlayer,&_lpPacket->avatarData,sizeof(_lpPacket->avatarData));

	//	유지 될 필요 없는 데이터들 초기화
	
	memset(lpPlayer->m_aWayPoint,0,sizeof(lpPlayer->m_aWayPoint));
	memset(&lpPlayer->m_itemFailedPremiumItem,0,sizeof(lpPlayer->m_itemFailedPremiumItem));
	memset(lpPlayer->m_aProcessQuest,0,sizeof(lpPlayer->m_aProcessQuest));
	memset(lpPlayer->m_awMainQuestValue,0,sizeof(lpPlayer->m_awMainQuestValue));
	memset(lpPlayer->m_abMainQuestSwitchStatus,0,sizeof(lpPlayer->m_abMainQuestSwitchStatus));
	memset(lpPlayer->m_awSkillCoolTime,0,sizeof(lpPlayer->m_awSkillCoolTime));
	memset(lpPlayer->m_aCompleteQuest2,0,sizeof(lpPlayer->m_aCompleteQuest2));
	memset(lpPlayer->m_aCompleteArbeit2,0,sizeof(lpPlayer->m_aCompleteArbeit2));
	memset(lpPlayer->m_aCompleteArbeit,0,sizeof(lpPlayer->m_aCompleteArbeit));
	memset(lpPlayer->m_aCompleteQuest,0,sizeof(lpPlayer->m_aCompleteQuest));
	memset(lpPlayer->m_awQuestCountDownTimer,0,sizeof(lpPlayer->m_awQuestCountDownTimer));

	lpPlayer->m_dwPeneltyTime					=	0;
	lpPlayer->m_bf1IsPlayOnGuildBattle			=	0;
	lpPlayer->m_bf1IsTeleportToCouple			=	0;
	lpPlayer->m_bf7UseCoupleRingSlot			=	0;
	lpPlayer->m_bf10ESD_Time					=	0;
	lpPlayer->m_bf3UseBoostCount				=	0;
	lpPlayer->m_bf12DeathPeneltySecond			=	0;
	lpPlayer->m_bf1IsPlayOnGuildBattle			=	0;
	lpPlayer->m_bf5GuildBattleDay				=	0;
	lpPlayer->m_bf1IsTeleportToCouple			=	0;
	lpPlayer->m_bf7UseCoupleRingSlot			=	0;
	lpPlayer->m_bf10ESD_Time					=	0;
	lpPlayer->m_bf1IsMoveFieldByDeath			=	0;
	lpPlayer->m_bf1IsMoveFieldByTeleport		=	0;
	lpPlayer->m_bf8InstanceField				=	0;
	lpPlayer->m_bf4InstanceFieldFloor			=	0;
	lpPlayer->m_bf4SecretDungeonPlayCount		=	0;
	lpPlayer->m_wRedStonePoint					=	0;
	lpPlayer->m_wBoostExpSecondByTreasureMapEvent	=	0;
	lpPlayer->m_wBoostGoldSecondByTreasureMapEvent	=	0;

	//	덮어씌우면 안되는 데이터들 복구
	{
		memcpy(lpPlayer->m_strName,strName,sizeof(strName));
		memcpy(&lpPlayer->m_timeGuildJoinTime,&timeGuildJoinTime,sizeof(timeGuildJoinTime));		//	길드 가입한 시간
		memcpy(lpPlayer->m_aWayPoint,aWayPoint,sizeof(aWayPoint));

		lpPlayer->m_wCurrentField		=	wCurrentField			;	//현재 필드
		lpPlayer->m_iXPos				=	iXPos					;
		lpPlayer->m_iYPos				=	iYPos					;
		lpPlayer->m_wLastVillage		=	wLastVillage			;		//마지막으로 떠나온 마을
		lpPlayer->m_wGuildSerial		=	wGuildSerial			;
		lpPlayer->m_wGuildRank			=	wGuildRank			;
		lpPlayer->m_dwSaveCount			=	dwSaveCount			;
		lpPlayer->m_wAvatarIndex		=	wAvatarIndex			;				//	아바타 인덱스
		lpPlayer->m_dwPlayTime			=	dwPlayTime			;		//	플레이 타임 분 단위
		lpPlayer->m_wProcessChapterIndex=	wProcessChapterIndex	;	//	진행 중인 메인 퀘스트 챕터 인덱스. GVG 서버 캐릭터는 이 정보가 1이면 이름에 '_' 가 안붙고 바로 알파벳이 붙은 경우다.
		lpPlayer->m_bf1IsReadyToMakeGuild=	wIsReadyToMakeGuild;
	}

	lpPlayer->m_bIsWaitOrgAvatarData	=	FALSE;

	lpPlayer->sendPlayerInfo(lpPlayer);
	lpPlayer->sendEtcWork(eEW_ASK_GVG_AVATAR_ORG_DATA_RESULT,eGET_GVG_AVATAR_ORG_DATA_RESULT_SUCCESS);
}

void
cGAME::operateW2G_ASK_GVG_AVATAR_ORG_DATA_RESULT(W2G_ASK_GVG_AVATAR_ORG_DATA_RESULT *_lpPacket)
{
//	printf("[GVG] receive W2G_ASK_GVG_AVATAR_ORG_DATA_RESULT %s\n",_lpPacket->strName);

	cACTOR	*lpPlayer	=	getPlayer(_lpPacket->strName);

	if	(!lpPlayer)
		return;

	switch(_lpPacket->wResult)
	{
		case	eGET_GVG_AVATAR_ORG_DATA_RESULT_SUCCESS					:
		case	eGET_GVG_AVATAR_ORG_DATA_RESULT_SUCCESS_BY_IS_MOVEFIELD	:
			lpPlayer->m_bIsWaitOrgAvatarData	=	FALSE;
			lpPlayer->sendEtcWork(eEW_ASK_GVG_AVATAR_ORG_DATA_RESULT,eGET_GVG_AVATAR_ORG_DATA_RESULT_SUCCESS);
			break;

		case	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED							:
		case	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_ORG_AVATAR	:
		case	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_AVATAR		:
		case	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_LOGINSERVER	:
		case	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_WORLDSERVER	:
		case	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_DBCSERVER	:
			lpPlayer->sendEtcWork(eEW_ASK_GVG_AVATAR_ORG_DATA_RESULT,_lpPacket->wResult);
			break;
	}
}

void
cGAME::operateTransGuildToGVGResult(WORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT *_lpPacket)
{
	cACTOR	*lpPlayer	=	g_game.getPlayer(_lpPacket->strMasterName);

	if	(!lpPlayer)	//	아 뭐 이런경우가.. ;;
		return;

	lpPlayer->operateTransGuildToGVGResult(_lpPacket);
}

void
cGAME::operateDuelTeamInfo(WORLDtoSERVERS_DUEL_TEAM *_lpPacket)
{
	g_duelTrialGameManager.init(_lpPacket->wTotalTeamCount);

	for (int i=0;i<_lpPacket->wTeamCount;i++)
		g_duelTrialGameManager.addTeam(&_lpPacket->aTeams[i]);
}

void
cGAME::operateDuelRankerInfo(WORLDtoSERVERS_DUEL_RANKER_INFO *_lpPacket)
{
	if	(_lpPacket->wIsDailyRanking)
	{
		g_duelRankerManager.setDailyRankerCount(_lpPacket->wWeeklyRankerCount);
		g_duelRankerManager.setTrialGameRankerCount(_lpPacket->wTrialGameRankerCount);

		int	i=0,iSlot=0;

		for (i=0;i<_lpPacket->wWeeklyRankerCount;i++,iSlot++)
			g_duelRankerManager.updateDailyRanker(&_lpPacket->aList[iSlot]);
		for (i=0;i<_lpPacket->wTrialGameRankerCount;i++,iSlot++)
			g_duelRankerManager.updateTrialGameRanker(&_lpPacket->aList[iSlot]);
	}
	else
	{
		g_duelRankerManager.setWeeklyRankerCount(_lpPacket->wWeeklyRankerCount);

		int	i=0;

		for (;i<_lpPacket->wWeeklyRankerCount;i++)
			g_duelRankerManager.updateWeeklyRanker(&_lpPacket->aList[i]);
	}

	for (int i=0;i<m_iMaxActorCount;i++)
	{
		cACTOR	*lpActor	=	getActor(i);
		
		if	(!lpActor	||	!lpActor->isPlayer())
			continue;

		int	iWRank	=	lpActor->m_wWeeklyDuelRank;
		int	iDRank	=	lpActor->m_wDailyDuelRank;
		
		lpActor->m_wDailyDuelRank	=	g_duelRankerManager.getRanking(eDailyRanking,lpActor->m_strId);
		lpActor->m_wWeeklyDuelRank	=	g_duelRankerManager.getRanking(eWeeklyRanking,lpActor->m_strId);
		
		if	(iWRank	!=	lpActor->m_wWeeklyDuelRank	||	iDRank	!=	lpActor->m_wDailyDuelRank)
			lpActor->sendDuelRecord();
	}
}

void
cGAME::operateSoundOfLeadersBell(WORLDtoSERVERS_SOUND_OF_LEADERS_BELL *_lpPacket)
{
	if	(_lpPacket->wWork	==	3)	//	3 메시지 변경
	{
		strcpy(g_eventManager.m_strSoundOfLeadersBellMessage,_lpPacket->strMessage);
		return;
	}

	g_iBoostExpByLeadersSoundofBell				=	_lpPacket->iBoostExp;
	g_iBoostItemDropChanceByLeadersSoundofBell	=	_lpPacket->iBoostItemDropChance;

	g_eventManager.m_wIsDisplayNameWithSoundOfLeadersBell		=	_lpPacket->wDisplayName;
	g_eventManager.m_iUpkeepSecondForSoundOfLeadersBell			=	_lpPacket->iUpkeepTime;

	strcpy(g_eventManager.m_strSoundOfLeadersBellMessage,_lpPacket->strMessage);
	strcpy(g_eventManager.m_strSoundOfLeadersBellOwner,_lpPacket->strCaster);

	{
		SG_SOUND_OF_LEADERS_BELL	packet;

		packet.base.set(sizeof(packet),dSG_SOUND_OF_LEADERS_BELL);

		packet.iUpkeepSecond		=	_lpPacket->iUpkeepTime;
		packet.iBoostExp			=	_lpPacket->iBoostExp;
		packet.iBoostItemDropChance	=	_lpPacket->iBoostItemDropChance;
		packet.wIsDisplayNameWithSoundOfLeadersBell	=	_lpPacket->wDisplayName;

		strcpy(packet.strMessage,_lpPacket->strMessage);
		strcpy(packet.strCaster,_lpPacket->strCaster);

		switch(_lpPacket->wWork)
		{
			case	0	:	//	이벤트 시작,
			{
				packet.wWork	=	eSoundOfLeadersBellWork_begin;

				cACTOR	*lpPlayer			=	g_game.getPlayer(_lpPacket->strCaster);

				if	(lpPlayer)
				{
					LOG_S_LEDERS_BELL	logPacket;

					logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);

					logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
					logPacket.logTime.mm		=	g_currentTime.m_wMonth;
					logPacket.logTime.dd		=	g_currentTime.m_wDay;
					logPacket.logTime.hour		=	g_currentTime.m_wHour;
					logPacket.logTime.min		=	g_currentTime.m_wMinute;
					logPacket.wFieldUniqueIndex	=	lpPlayer->m_wCurrentField;
					logPacket.wWork				=	eSLT_USE_LEADERS_BELL	;

					strcpy(logPacket.strMessage,_lpPacket->strMessage);

					g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
				}
				break;
			}

			case	1	:	//	이벤트 종료,
				packet.wWork	=	eSoundOfLeadersBellWork_end;
				break;
		}

		sendPacketToAllPlayer((char*)&packet);
	}

	if	(_lpPacket->wWork	==	1)
	{
		cACTOR	*lpPlayer			=	g_game.getPlayer(_lpPacket->strCaster);

		if	(lpPlayer)
		{
			lpPlayer->m_bf1IsSoundOfLedersBellsLeader	=	FALSE;
			lpPlayer->addSendVerySimpleInfo();
		}
	}

	if	(_lpPacket->wWork	==	eSoundOfLeadersBellWork_begin)
	{
		cACTOR	*lpPlayer			=	g_game.getPlayer(_lpPacket->strCaster);

		if	(g_eventManager.m_iUpkeepSecondForSoundOfLeadersBell && lpPlayer)
		{
			lpPlayer->m_bf1IsSoundOfLedersBellsLeader	=	TRUE;

			cItem	*lpItem	=	lpPlayer->getInventoryItem(_lpPacket->wItemIndex);

			if	(lpItem)
			{
				lpItem->decreaseCount(1);
				lpPlayer->sendUpdateItemData(_lpPacket->wItemIndex,TRUE);
			}

			lpPlayer->addSendVerySimpleInfo();
		}
	}
}

void
cGAME::operateBoostPowerByEvent(WORLDtoSERVERS_BOOST_POWER_BY_EVENT *_lpPacket)
{
	SG_BOOST_POWER_BY_EVENT	packet;

	packet.base.set(sizeof(packet),dSG_BOOST_POWER_BY_EVENT);
	strcpy(packet.strName,_lpPacket->strName);

	packet.wWork	=	_lpPacket->wWork;

	switch(_lpPacket->wWork)
	{
		case	eBPBE_TM_GOLD	:	//	보물 지도 이벤트를 인한 골드 4배
		{
			g_iBoostGoldByTreasureMapEventCount				=	_lpPacket->wBoostGoldByTreasureMapEventCount;
			g_iBoostGoldSecondByTreasureMapEventInServer	+=	dBOOST_PLAYER_GOLD_SECOND_BY_TREASURE_MAP;
			packet.wSecond									=	g_iBoostGoldSecondByTreasureMapEventInServer;
			g_game.sendPacketToAllPlayer((char*)&packet);
			break;
		}

		case	eBPBE_TM_EXP	:	//	보물 지도 이벤트로 인한 경험치 2배
		{
			g_iBoostExpByTreasureMapEventCount				=	_lpPacket->wBoostExpByTreasureMapEventCount;
			g_iBoostExpSecondByTreasureMapEventInServer		+=	dBOOST_PLAYER_EXP_SECOND_BY_TREASURE_MAP;
			packet.wSecond									=	g_iBoostExpSecondByTreasureMapEventInServer;
			g_game.sendPacketToAllPlayer((char*)&packet);
			break;
		}
	}
}

void
cGAME::operateAccountInfo(WORLDtoSERVERS_ACCOUNT_INFO *_lpPacket)
{
	m_iTotalPlayerCountInWorld	=	0;

	memcpy(m_aiPlayerCountInfoInWorld,_lpPacket->aiPlayerCount,sizeof(_lpPacket->aiPlayerCount));

	for (int i=0;i<10;i++)
		m_iTotalPlayerCountInWorld	+=	m_aiPlayerCountInfoInWorld[i];
}

void
cGAME::operateDisconnectServerPacket(WORLDtoSERVERS_DISCONNECT_SERVER *_lpPacket)
{
	int				aDisconnectFieldList[1000];
	cSERVER_DATA	*lpServerInfo	=	&g_game.m_aServerList[_lpPacket->wServerIndex];

	for (int i=0;i<lpServerInfo->m_iCount;i++)
		aDisconnectFieldList[i]	=	lpServerInfo->m_aMap[i].m_iSerial;

	g_pPartyManager->disconnectField(aDisconnectFieldList,lpServerInfo->m_iCount);
}

void
cGAME::operateGRInfo(WORLDtoSERVERS_GR_INFO *_lpPacket)
{
	g_eventManager.updateGoldRushInfo(&_lpPacket->begin,&_lpPacket->end,_lpPacket->wBoostExp,_lpPacket->wDropChance,_lpPacket->strBeginComment,_lpPacket->strEndComment);
}

void
cGAME::operateItemPackInfo(WORLDtoSERVERS_ITEM_PACK_INFO *_lpPacket)
{
	g_game.receiveInitDataFromWorld();
	g_itemPackList.receivePack(&_lpPacket->info);
}

void
cGAME::operateItemPackListSerial(WORLDtoSERVERS_ITEM_PACK_LIST_SERIAL *_lpPacket)
{
	for	(int i=0;i<_lpPacket->wCount;i++)
	{
		int	iSerial	=	_lpPacket->awList[i];

		if	(g_itemPackList.m_aPacks[iSerial].m_wSerial	!=	iSerial)
		{
			ERRMSG("Error!!","Receive Item pack list Error!!\n\nplease restart game server");

			return;
		}
	}

	g_itemPackList.m_bIsReceiveAllInfo	=	TRUE;
}

void
cGAME::operateResetDropPackItemCount(WORLDtoSERVERS_RESET_DROP_PACK_ITEM_COUNT *_lpPacket)
{
	g_itemPackList.resetCurrentItemDropCount(_lpPacket->wPackSerial);
}

void
cGAME::operateUpdatePackItemCount(WORLDtoSERVERS_UPDATE_PACK_ITEM_COUNT *_lpPacket)
{
	g_itemPackList.updateItemDropCount(_lpPacket->wPackSerial,_lpPacket->wItemIndex,_lpPacket->wCount);
}

void
cGAME::operateCarrotShopInfo(WORLDtoSERVERS_CARROT_SHOP_INFO *_lpPacket)
{
	if	(_lpPacket->wStatus	==	2)
	{
		g_carrotShopInfo.m_wCategoryCount					=	min(_lpPacket->wCount,dMAX_CARROT_SHOP_CATEGORY_COUNT);
		g_carrotShopInfo.m_bIsCompleteReceiveCarrotShopInfo	=	TRUE;

		g_carrotShopInfo.buildCheckSum();

		printf("- complete receive carrot shop info[0x%x]\n",g_carrotShopInfo.m_dwCheckSum);
		return;
	}

	if	(_lpPacket->wStatus	==	0)
		g_carrotShopInfo.clearBuffer();

	g_game.receiveInitDataFromWorld();
	g_carrotShopInfo.updateInfo(_lpPacket->wIndex,&_lpPacket->data,&_lpPacket->categoryInfo);
}

void
cGAME::operateConfirmEnterField(WORLDtoSERVERS_CONFIRM_ENTER_FIELD *_lpPacket)
{
	cACTOR	*lpActor	=	getPlayerByID(_lpPacket->strId);

	if	(!lpActor)
		return;

	if	(lpActor->m_wCurrentField	!=	_lpPacket->wField	||	STRICMP(lpActor->m_strName,_lpPacket->strName)	!=	0)
		lpActor->bookingToExitGame();
}

//
//	아이템 생성 결과
void
cGAME::operateCreateItemResultPacket(WORLDtoSERVERS_CREATE_ITEM_RESULT *_lpPacket)
{
	cFIELD	*lpField	=	getField(_lpPacket->wFieldSerial);

	if (!lpField)
		return;
	
	int					i,iCorrectAngle	=	random(360);
	SG_DROPPING_ITEMS	packet;

	packet.base.set(sizeof(packet),dSG_DROPPING_ITEMS);
	packet.wCount		=	0;

	for (i=0;i<_lpPacket->wCount;i++)
	{
		CDroppedItem	*lpItem		=	lpField->m_droppedItem.get(_lpPacket->aCreateItemResult[i].m_wSerial);

		if	(!lpItem)
		{
			CLOG("error","in cGAME::operateCreateItemResultPacket - !lpItem");
			continue;
		}

		if	(lpItem->m_item.m_wBaseItem	!=	dITEM_MONEY)
			lpItem->m_item.m_dwSerial	=	_lpPacket->aCreateItemResult[i].m_dwRegisteredSerial;

		if (_lpPacket->wCount	==	1)
			g_protractor.getAnglePos(&lpItem->m_pos,random(360),10);
		else
			g_protractor.getAnglePos(&lpItem->m_pos,i*(360/_lpPacket->wCount)+iCorrectAngle,60+random(50));

		cP_DROPPED_ITEM	*lpDropItemInfo	=	&packet.aDroppedItem[packet.wCount];

		lpDropItemInfo->m_bf10Serial		=	lpItem->m_wSerial;//시리얼
		lpDropItemInfo->m_bf14BaseItem		=	lpItem->m_item.m_wBaseItem;
		lpDropItemInfo->m_bf4Type			=	lpItem->m_item.getItemMagicalType();

		if	(lpDropItemInfo->m_bf14BaseItem	==	dITEM_MONEY)	
			lpDropItemInfo->m_wCount	=	(WORD)min(lpItem->m_item.m_dwSerial,0xffff);
		else
			lpDropItemInfo->m_wCount	=	lpItem->m_item.m_bCount;

		lpDropItemInfo->wX				=	lpItem->m_pos.x;//어디에 떨어져 있냐?
		lpDropItemInfo->wY				=	lpItem->m_pos.y;//어디에 떨어져 있냐?

		packet.wCount++;
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aDroppedItem)+packet.wCount*sizeof(cP_DROPPED_ITEM);

	lpField->addSendPacket((ALL_MSG*)&packet,0);
}

//
//	아이템 추가 결과
void
cGAME::operateAddItemResultPacket(WORLDtoSERVERS_ADD_ITEM_RESULT *_lpPacket)
{
}

//
//	유저 찾기 결과
void
cGAME::operateUserFindResultPacket(WORLDtoSERVERS_USER_FIND *_lpPacket)
{
	cACTOR	*lpRequester=	getPlayer(_lpPacket->strName);
	
	if	(!lpRequester)
		return;

	if	(_lpPacket->Result	==	dFIND_RESULT_NOTFIND)
	{
		lpRequester->sendFindUserResult(_lpPacket->strFindName,eFUR_NOT_FIND,_lpPacket->wMapUniqueIndex,_lpPacket->wJob);
		return;
	}

	switch(_lpPacket->wWork)
	{
		case	eFUW_FIND					:	//	검색
		{
			lpRequester->sendFindUserResult(_lpPacket->strFindName,eFUR_FIND,_lpPacket->wMapUniqueIndex,_lpPacket->wJob);
			break;
		}

		case	eFUW_RECALL					:	//	소환 - 누군가를 내 옆으로 소환
		{
			SERVERStoWORLD_USER_RECALL	packet;

			packet.base.set(sizeof(SERVERStoWORLD_USER_RECALL),dSERVERStoWORLD_USER_RECALL);
			
			if (lpRequester->m_wCurrentField	>=	990)
				break;

			packet.wFieldIndex	=	lpRequester->m_wCurrentField;
			packet.wX			=	lpRequester->m_pos.x;
			packet.wY			=	lpRequester->m_pos.y;
			strcpy(packet.strName,_lpPacket->strFindName);

			SEND_WOLRD_PACKET(&packet);
			break;
		}

		case	eFUW_CHECK_COUPLE_RING		:
		{
			int		iItemSlot						=	lpRequester->getCoupleRingIndex(_lpPacket->strFindName);

			if	(iItemSlot	==	0xffff)
			{
				g_game.sendWPFindUser(_lpPacket->strName,_lpPacket->strFindName,lpRequester->m_wCurrentField,eFUW_CHECK_COUPLE_RING_RESULT_FAILED);

				break;
			}

			if	(lpRequester->m_lpField->isBlockedToTeleportField() || lpRequester->m_lpField->isCanNotUseTeleportEffectField())
				g_game.sendWPFindUser(_lpPacket->strFindName,_lpPacket->strName,lpRequester->m_wCurrentField,eFUW_CHECK_COUPLE_RING_RESULT_TARGET_EXIST_CAN_NOT_TELEPORT_FIELD);
			else
				g_game.sendWPFindUser(_lpPacket->strFindName,_lpPacket->strName,lpRequester->m_wCurrentField,eFUW_CHECK_COUPLE_RING_RESULT_OK);
			break;
		}

		case	eFUW_CHECK_COUPLE_RING_RESULT_FAILED	:
		{
			lpRequester->sendFindUserResult(_lpPacket->strFindName,eFUR_TARGET_USER_NOT_OWNED_COUPLE_RING,_lpPacket->wMapUniqueIndex,_lpPacket->wJob);
			break;
		}

		case	eFUW_CHECK_COUPLE_RING_RESULT_TARGET_EXIST_CAN_NOT_TELEPORT_FIELD	:
		{
			lpRequester->sendRegistServerMessage(eRSM_TARGET_IS_AT_BLOCKED_FIELD_TO_USE_THAT_ITEM);
			break;
		}

		case	eFUW_USE_COUPLE_RING		:	//	워프 - 누군가의 옆으로 이동
		{
			if	(_lpPacket->wMapUniqueIndex	>=	990)
				break;

			if	(_lpPacket->bf1IsPremiumZone	&&	lpRequester->m_wPremiumServiceLevel	==	0 && lpRequester->m_bf1IsExistExceptExpPortal == FALSE)
			{
				lpRequester->sendEtcWork(eEW_TARGET_IS_IN_PREMIUM_ZONE);

				break;
			}

			int		iItemSlot						=	lpRequester->getCoupleRingIndex(_lpPacket->strFindName);

			if	(iItemSlot	==	0xffff)
				break;

			if	(_lpPacket->wMapUniqueIndex	==	27)
			{
				lpRequester->sendEtcWork(eEW_CAN_NOT_RECALL_ACTOR_IN_JAIL);
				break;
			}

			lpRequester->m_bf1IsTeleportToCouple	=	TRUE;	//	커플링을 사용해 텔레포트중이다.
			lpRequester->m_bf1IsMoveFieldByTeleport	=	TRUE;
			lpRequester->m_bf7UseCoupleRingSlot		=	iItemSlot;

			g_pMoveFieldUserManager->booking(lpRequester,NULL,_lpPacket->wMapUniqueIndex,0x7fff);

			break;
		}

		case	eFUW_WARP		:	//	워프 - 누군가의 옆으로 이동
		{
			if	(_lpPacket->wMapUniqueIndex	>=	990)
				break;

			g_pMoveFieldUserManager->booking(lpRequester,NULL,_lpPacket->wMapUniqueIndex,0xffff);

			break;
		}

		case	eFUW_JAIL			:
		{
			SERVERStoWORLD_USER_RECALL	packet;

			packet.base.set(sizeof(SERVERStoWORLD_USER_RECALL),dSERVERStoWORLD_USER_RECALL);

			packet.wFieldIndex	=	27;
			packet.wX			=	-1;
			packet.wY			=	-1;
			strcpy(packet.strName,_lpPacket->strFindName);

			SEND_WOLRD_PACKET(&packet);
			break;
		}

		case	eFUW_DRAWING_ROOM	:
		{
			SERVERStoWORLD_USER_RECALL	packet;

			packet.base.set(sizeof(SERVERStoWORLD_USER_RECALL),dSERVERStoWORLD_USER_RECALL);

			packet.wFieldIndex	=	28;
			packet.wX			=	-1;
			packet.wY			=	-1;
			strcpy(packet.strName,_lpPacket->strFindName);

			SEND_WOLRD_PACKET(&packet);
			break;
		}

		case	eFUW_BANISH			:
		{
			cACTOR	*lpTarget	=	getPlayer(_lpPacket->strFindName);
			cACTOR	*lpOperator	=	getPlayer(_lpPacket->strName);

			if	(lpTarget)
				lpTarget->bookingReadyToExitGame();

			if	(lpOperator)
			{
				if	(_lpPacket->Result	==	dFIND_RESULT_NOTFIND)
					lpOperator->sendServerMessage(_ms("can not find follow player : %s",_lpPacket->strFindName),eSM_MSG_BOX);
				else
					lpOperator->sendServerMessage(_ms("success!! banish to follow player : %s",_lpPacket->strFindName),eSM_MSG_BOX);
			}

			break;
		}
	}
}

//
//	리콜 패킷 처리
void
cGAME::operateUserRecallPacket(WORLDtoSERVERS_USER_RECALL* _lpPacket)
{
	cACTOR	*lpActor	=	getPlayer(_lpPacket->strName);

	if (!lpActor)
		return;

	g_pMoveFieldUserManager->booking(lpActor,NULL,_lpPacket->wFieldIndex,0x7ffff,_lpPacket->wX,_lpPacket->wY);
}

//
//	아이템의 유니크 시리얼 요구
void
cGAME::operateReceiveItemUniqueSerial(WORLDtoSERVERS_ASK_ITEM_UNIQUE_SERIAL* _lpPacket)
{
	cACTOR	*lpActor	=	getActor(_lpPacket->iSerialInServer);

	if	(!lpActor || !lpActor->isPlayer())
		return;

	lpActor->setItemUniqueSerial(_lpPacket->dwSerial);
}

//
//	이벤트 메시지
void
cGAME::operateEventMessage(WORLDtoSERVERS_EVENT_MESSAGE* _lpPacket)
{
	int	i;

	for (i=0;i<m_iFieldCount;i++)
		m_pField[i].addSendEventMessage(_lpPacket->strMessage,_lpPacket->wType);

	g_pIfManager->addSendEventMessage(_lpPacket->strMessage,_lpPacket->wType);
}

void
cGAME::operateResultIncreaseDBEventValue(WORLDtoSERVERS_RESULT_INCREASE_DB_EVENT_VALUE* _lpPacket)
{
	cACTOR	*lpActor	=	getPlayer(_lpPacket->strUserName);

	if	(lpActor	&&	lpActor->m_wOperatorLevel	<	2)
		lpActor	=	NULL;

	switch(_lpPacket->wResult)
	{
		case	eROIDEV_SUCCESS				:
		{
			if	(lpActor)
				lpActor->sendServerMessage(_ms("Increase Event Value : [Number %d Event] [Value %d]",_lpPacket->wEventIndex,_lpPacket->iValue));
			
			CLOG("yepp","[%s/%s]",_lpPacket->strUserID,_lpPacket->strUserName);

			g_eventManager.changeDBControlledEventValue(_lpPacket->wEventIndex,_lpPacket->iValue);
			break;
		}
		case	eROIDEV_EVENT_COUNT_FULL	:	//	이벤트가 풀이다.
		{
			if	(lpActor)
				lpActor->sendServerMessage("Failed To Increase Event Value : It's Full");
			break;
		}
		case	eROIDEV_ALREADY_EXIST_USER	:	//	이미 존재하는 계정이다.
		{
			if	(lpActor)
				lpActor->sendServerMessage("Failed To Increase Event Value : Already tried");
			break;
		}
		case	eROIDEV_RESET	:	//	초기화
		{
			if	(lpActor)
				lpActor->sendServerMessage(_ms("Reset Event Value : [Number %d Event]",_lpPacket->wEventIndex));

			g_eventManager.changeDBControlledEventValue(_lpPacket->wEventIndex,0);
			break;
		}
	}
}

void
cGAME::operateCheckUserMoveMapResult(SERVERStoWORLD_USER_MOVEMAP* _lpPacket)
{
	cACTOR	*lpActor	=	getActor(_lpPacket->iSerialInServer);

	if	(!lpActor	||	!lpActor->isPlayer())
		return;

	if	(_lpPacket->wCheckResult	!=	dMOVEMAP_RESULT_SUCCESS)
	{
		lpActor->sendMoveFieldResult(_lpPacket->wCheckResult);
		CLOG("moveField","실패 - [%s/%s] %d",lpActor->m_strId,lpActor->m_strName,_lpPacket->wCheckResult);
		return;
	}

	lpActor->readyToRemoveActorWork();

	sendWPUpkeepPlayerData(lpActor);		//	필드를 이동해도 기억해야 할 데이터 전송

	{
		SERVERStoWORLD_USER_MOVEMAP	packetMove;

		memcpy(&packetMove,_lpPacket,sizeof(SERVERStoWORLD_USER_MOVEMAP));

		packetMove.base.set(sizeof(SERVERStoWORLD_USER_MOVEMAP),dSERVERStoWORLD_USER_MOVEMAP);

		SEND_WOLRD_PACKET(&packetMove);
	}
}

void
cGAME::operateMoveFieldResult(WORLDtoSERVERS_USER_MOVEMAP* _lpPacket)
{
	cACTOR	*lpActor	=	getActor(_lpPacket->iSerialInServer);

	if	(!lpActor || !lpActor->isPlayer())
		return;

	CClient	*lpClient	=	lpActor->getClient();

	if	(!lpClient)
		return;

	if	(STRICMP(_lpPacket->strServerAddr,"")!=0)	//	다른 서버로 이동할 경우
	{
		lpClient->SetStateFlag(CClient::eSTATE_MOVING);
//		_log( "HYUN!!! : id[%s] eSTATE_MOVING \n", lpClient->id);
	}

//	lpClient->SetIsUsed(eISUSED_WORLDtoSERVERS_USER_MOVEMAP);

	cFIELD	*lpField	=	lpActor->m_lpField;

	if	(lpActor->m_wWaitLogoutResultTime)
	{
		lpActor->m_bIsReadyToExit	=	FALSE;
//		printf("로그아웃중 필드 이동 결과 날라옴 [%s/%s]\n",lpActor->m_strId,lpActor->m_strName);
		CLOG("moveField","실패 - [%s/%s] 로그아웃 결과를 기다리는데 필드이동 결과 날아옴",lpActor->m_strId,lpActor->m_strName);
		return;
	}

	if	(lpActor->m_wWaitMoveFieldResultTime)
	{
		lpActor->m_bIsReadyToExit	=	FALSE;
//		printf("필드이동중 필드 이동 결과 다시 날라옴 [%s/%s]\n",lpActor->m_strId,lpActor->m_strName);
		CLOG("moveField","실패 - [%s/%s] 필드 이동 결과 받고 대기중인데 다시 필드이동 결과 날아옴",lpActor->m_strId,lpActor->m_strName);
		return;
	}

	if	(lpActor->m_wBookedRemoveTime)
	{
		lpActor->m_bIsReadyToExit	=	FALSE;
//		printf("제거 예약 상태에서 필드 이동 결과 날라옴 [%s/%s]\n",lpActor->m_strId,lpActor->m_strName);
		CLOG("moveField","실패 - [%s/%s] 제거 예약 상태인데 필드이동 결과 날아옴",lpActor->m_strId,lpActor->m_strName);
		return;
	}

	if	(_lpPacket->Result != dMOVEMAP_RESULT_SUCCESS)
	{
		lpActor->m_bIsReadyToExit	=	FALSE;

		lpActor->sendMoveFieldResult(_lpPacket->Result);
//		printf("필드 이동 실패 [%s/%s] [Result %d]\n",lpActor->m_strId,lpActor->m_strName,_lpPacket->Result);
		CLOG("moveField","실패 - [%s/%s] %d",lpActor->m_strId,lpActor->m_strName,_lpPacket->Result);
		return;
	}

	cFIELD	*lpDestField		=	g_game.getField(_lpPacket->strMapName);

	if	(lpDestField	==	NULL)
		lpDestField		=	g_game.getFieldByUniqueSerial(_lpPacket->wUniqueFieldSerial);

	if	(lpDestField	==	NULL)
		lpActor->m_wIsMoveToDifferentServer	=	TRUE;
	else
		lpActor->m_wIsMoveToDifferentServer	=	FALSE;

	lpClient->SetFieldSerial(_lpPacket->wUniqueFieldSerial);	//	시리얼 미리 세팅

	lpActor->m_wCurrentField	=	_lpPacket->wUniqueFieldSerial;
	lpActor->m_posLastMoved.x	=	_lpPacket->wXPos;
	lpActor->m_posLastMoved.y	=	_lpPacket->wYPos;
	lpActor->m_pos.x			=	_lpPacket->wXPos;
	lpActor->m_pos.y			=	_lpPacket->wYPos;

	if	(_lpPacket->wGateIndex	==	0x7ffe)
	{
		lpActor->m_iXPos		=	-2;	//	길드홀 입구로 이동해라
	}
	else
	if	(_lpPacket->wGateIndex	==	0x7fff)
	{
		lpActor->m_iXPos		=	_lpPacket->wXPos;
		lpActor->m_iYPos		=	_lpPacket->wYPos;
	}
	else
	{
		lpActor->m_iXPos		=	-1;
		lpActor->m_iYPos		=	_lpPacket->wGateIndex;
	}
	//CLOG("MoveMap",_ms("operateMoveFieldResult - [%s] field[%d : %s]",lpActor->m_strName,_lpPacket->wUniqueFieldSerial, _lpPacket->strMapName));
	if	(!lpActor->readyToMoveField(_lpPacket->wUniqueFieldSerial,0,0))
		return;	//	필드 이동 준비를 한다.

	if	(lpDestField)
		strcpy(_lpPacket->strServerAddr,"");

	int	iGuild					=	g_guildManager.getGuildHallOwnerGuildByFieldSerial(_lpPacket->wUniqueFieldSerial);

	lpActor->sendMoveFieldResult(_lpPacket->Result,_lpPacket->strServerAddr,_lpPacket->strMapName,0xffff,_lpPacket->wUniqueFieldSerial,iGuild);
}

//
//	
void
cGAME::operateWorldLogoutForMoveServerResult(WORLDtoSERVERS_USER_LOGOUT* _lpPacket)
{
	cACTOR	*lpActor	=	getActor(_lpPacket->iSerialInServer);

	if	(!lpActor)
	{
//		printf("필드이동 버그 : 월드 로그아웃 결과 시리얼로 이름 찾으니까 없음 [%s]\n",_lpPacket->strName);
		CLOG("moveFieldBug","시리얼로 이름 찾으니까 엉뚱한 녀석이 나옴 [%s]",_lpPacket->strName);
		
		lpActor	=	getPlayer(_lpPacket->strName);

		if	(!lpActor)
			return;

//		printf("필드이동 버그 : 캐릭터 이름으로 검색하니까 있어서 자른다. -o-\n",lpActor->m_strId,lpActor->m_strName);

		lpActor->readyToExitGame();

		return;
	}

	if (STRCMP(lpActor->m_strName,_lpPacket->strName) != 0)
	{
//		printf("필드이동 버그 : 월드 로그아웃 결과 시리얼로 이름 찾으니까 엉뚱한 녀석이 나옴 %s ->%s/%s\n",_lpPacket->strName,lpActor->m_strId,lpActor->m_strName);
		CLOG("moveFieldBug","시리얼로 이름 찾으니까 엉뚱한 녀석이 나옴 %s ->[%s/%s]",_lpPacket->strName,lpActor->m_strId,lpActor->m_strName);

		lpActor	=	getPlayer(_lpPacket->strName);

		if (!lpActor)
			return;

		lpActor->readyToExitGame();

		return;
	}

	lpActor->bookingToExitGameForMoveField();
}

//
//	길드 생성
void
cGAME::operateCreateGuildResult(WORLDtoSERVERS_USER_CREATEGUILD *_lpPacket)
{
	cACTOR	*lpMaster	=	getPlayer(_lpPacket->strMasterName);

	if	(!lpMaster)
		return;

	SG_MAKE_GUILD_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_MAKE_GUILD_RESULT);

	switch(_lpPacket->Result)
	{
		case	dCREATEGUILD_RESULT_SUCCESS		:		//	오케바리~
			if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
				memset(lpMaster->m_aWayPoint,0xff,sizeof(lpMaster->m_aWayPoint));

			packet.wResult	=	eMGR_SUCCESS;
			lpMaster->createGuildFirstWork(_lpPacket->wGuildSerial,_lpPacket->strGuildName,_lpPacket->strSubMasterName);
			break;

		case	dCREATEGUILD_RESULT_FULL		:	//	더이상 만들수 없습니다.
			packet.wResult	=	eMGR_FULL;
			break;

		case	dCREATEGUILD_RESULT_SAMENAME	:		//	같은 이름의 길드가 있어
			packet.wResult	=	eMGR_EXIST_SAME_NAME_GUILD;
			break;

		case	dCREATEGUILD_RESULT_HAVEGUILD	:		//	길드 만들 준비가 안됐어
			packet.wResult	=	eMGR_ALREADY_HAVEAGUILD;
			break;

		default									:
			packet.wResult	=	eMGR_FAILED;
			break;
	}

	strcpy(packet.strGuildName,_lpPacket->strGuildName);

	g_userPM.add(lpMaster->m_iClientSerial,&packet,packet.base.wSize);

	if	(packet.wResult	==	eMGR_SUCCESS && g_iWorldServerType	!=	eSERVER_TYPE_GVG)
	{
		cParty	party;

		if	(g_pPartyManager->getParty(&party,lpMaster->getPartySerial()))
		{
			for (int i=1;i<(int)party.m_wMemberCount;i++)
			{
				int		iRank		=	dGUILD_CLASS_ROOKIE;
				cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

				if	(!lpMember)
					continue;

				if	(STRCMP(lpMember->m_strName,_lpPacket->strSubMasterName)==0)	
					iRank	=	dGUILD_CLASS_SUBMASTER;

				sendWPJoinGuild(_lpPacket->strGuildName,_lpPacket->strMasterName,lpMember,iRank);
			}
		}
	}
}

void
cGAME::operateChangeGuildInventoryPermitWithdrawGold(WORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_GOLD *_lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->iGuild);

	if	(!lpGuild)
		return;

	lpGuild->m_aiWithdrawGoldLimitPerDay[_lpPacket->wRank]		=	_lpPacket->iGold;

	if	(IsAvailBCConnect()	==	FALSE)
	{
		SG_ETC_WORK	packet;

		packet.base.set(sizeof(packet),dSG_ETC_WORK);
		packet.wWork		=	eEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_GOLD;
		packet.aiValue[0]	=	_lpPacket->wRank;
		packet.aiValue[1]	=	(_lpPacket->iGold>>16);
		packet.aiValue[2]	=	_lpPacket->iGold&0xffff;

		lpGuild->addSendPacketToAllMember(&packet);
	}
}
void
cGAME::operateChangeGuildInventoryPermitWithdrawItem(WORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_ITEM *_lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->iGuild);

	if	(!lpGuild)
		return;

	lpGuild->m_aWithdrawItemCountPerDay[_lpPacket->wStorage][_lpPacket->wRank]		=	(BYTE)_lpPacket->wItemCount;

	if	(IsAvailBCConnect()	==	FALSE)
	{
		SG_ETC_WORK	packet;

		packet.base.set(sizeof(packet),dSG_ETC_WORK);
		packet.wWork		=	eEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_ITEM_COUNT;
		packet.aiValue[0]	=	_lpPacket->wStorage;
		packet.aiValue[1]	=	_lpPacket->wRank;
		packet.aiValue[2]	=	_lpPacket->wItemCount;

		lpGuild->addSendPacketToAllMember(&packet);
	}
}
void
cGAME::operateChangeGuildInventoryStorageSize(WORLDtoSERVERS_CHANGE_GI_STORAGE_SIZE *_lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->iGuild);

	if	(!lpGuild)
		return;

	memcpy(lpGuild->m_abInventorySize,_lpPacket->abStorageSize,sizeof(_lpPacket->abStorageSize));

	if	(IsAvailBCConnect()	==	FALSE)
	{
		SG_ETC_WORK	packet;

		packet.base.set(sizeof(packet),dSG_ETC_WORK);
		packet.wWork		=	eEW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE;
		packet.aiValue[0]	=	_lpPacket->abStorageSize[0];
		packet.aiValue[1]	=	_lpPacket->abStorageSize[1];
		packet.aiValue[2]	=	_lpPacket->abStorageSize[2];
		packet.aiValue[3]	=	_lpPacket->abStorageSize[3];

		lpGuild->addSendPacketToAllMember(&packet);
	}
}

void
cGAME::operateRecordGuildHonorPoint(WORLDtoSERVERS_RECORD_GUILD_HONOR_POINT *_lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->iGuild);

	if	(!lpGuild)
		return;

	lpGuild->m_bf2TryGuildPointBattleCountPerWeek	=	_lpPacket->bf2TryGuildPointBattleCountPerWeek;

	if	(_lpPacket->bf1IsVictoryPoint	)
		lpGuild->m_iVictoryPoint=	_lpPacket->iGuildHonorPoint;
	else
	{
		lpGuild->m_iHonorPoint	=	_lpPacket->iGuildHonorPoint;

		if	(g_iServerIndex	==	0)
		{
			LOG_S_RECORD_GUILD_HONOR_POINT	logPacket;

			logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);

			logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
			logPacket.logTime.mm		=	g_currentTime.m_wMonth;
			logPacket.logTime.dd		=	g_currentTime.m_wDay;
			logPacket.logTime.hour		=	g_currentTime.m_wHour;
			logPacket.logTime.min		=	g_currentTime.m_wMinute;
			logPacket.wFieldUniqueIndex	=	0;
			logPacket.wWork				=	eSLT_RECORD_GUILD_HONOR_POINT;

			strcpy(logPacket.strGuildName,lpGuild->m_strName);
			logPacket.iHonorPoint		=	_lpPacket->iGuildHonorPoint;

			g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
		}
	}
}

void
cGAME::operateAddRelatedPlaceOfGuild(WORLDtoSERVERS_ADD_RELATED_PLACE_OF_GUILD *_lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->iGuild);

	if	(!lpGuild)
		return;

	if	(_lpPacket->wIsSuccess)
	{
		if	(_lpPacket->wIsAdd)
		{
			lpGuild->addRelatedPlace(_lpPacket->iFieldSerial,_lpPacket->iGold);

			WORD	wGold1	=	_lpPacket->iGold>>16;
			WORD	wGold2	=	_lpPacket->iGold&0xffff;

			lpGuild->sendEtcWork(eEW_ADD_RELATED_PLACE_OF_GUILD,_lpPacket->iFieldSerial,wGold1,wGold2);
		}
		else
		{
			lpGuild->removeRelatedPlace(_lpPacket->iFieldSerial);

			lpGuild->sendEtcWork(eEW_REMOVE_RELATED_PLACE_OF_GUILD,_lpPacket->iFieldSerial);
		}

	}
	else
	{
		cACTOR	*lpActor	=	g_game.getPlayer(_lpPacket->strMemberName);

		if	(lpActor)
			lpActor->sendRegistServerMessage(eRSM_FAILED_COMMAND_WORK);
	}
}

void
cGAME::operateStoreGuildInventoryItem(WORLDtoSERVERS_STORE_GUILD_INVENTORY_ITEM *_lpPacket)
{
	cACTOR	*lpActor	=	g_game.getPlayer(_lpPacket->strMemberName);

	if	(_lpPacket->wIsFailed)
	{
		if	(lpActor)
		{
			if	(_lpPacket->item.isMoneyItem())
				CLOG("goldIOError","operateStoreGuildInventoryItem");

			lpActor->addItemToInventory((cItem *)&_lpPacket->item);
			lpActor->sendRegistServerMessage(eRSM_FAILED_COMMAND_WORK);
		}

		return;
	}

	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->iGuild);

	if	(!lpGuild)
		return;

	lpGuild->storeItem(_lpPacket->iGuildInventorySlot,&_lpPacket->item);

	if	(lpActor)
		lpActor->sendRemoveItem(_lpPacket->wInventorySlot,_lpPacket->item.m_bCount,TRUE,eRIR_STORE_TO_GUILD_INVENTORY);

	if	(IsAvailBCConnect()	==	FALSE)
	{
		SG_STORE_GUILD_INVENTORY_ITEM	packet;

		packet.base.set(sizeof(packet),dSG_STORE_GUILD_INVENTORY_ITEM);
		memcpy(&packet.item,&_lpPacket->item,sizeof(_lpPacket->item));
		packet.wGuildInventorySlot	=	_lpPacket->iGuildInventorySlot;

		lpGuild->addSendPacketToAllMember(&packet);
	}
}

void
cGAME::opreateWithdrawGuildInventoryItem(WORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_ITEM *_lpPacket)
{
	cACTOR	*lpActor	=	g_game.getPlayer(_lpPacket->strMemberName);

	if	(_lpPacket->wIsFailed)
	{
		if	(lpActor)
			lpActor->sendRegistServerMessage(eRSM_FAILED_COMMAND_WORK);

		return;
	}

	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->iGuild);

	if	(!lpGuild)
		return;

	lpGuild->removeItem(_lpPacket->iGuildInventorySlot,&_lpPacket->item);

	if	(IsAvailBCConnect()	==	FALSE)
	{
		SG_REMOVE_GUILD_INVENTORY_ITEM	packet;

		packet.base.set(sizeof(packet),dSG_REMOVE_GUILD_INVENTORY_ITEM);
		packet.wGuildInventorySlot	=	_lpPacket->iGuildInventorySlot;
		memcpy(&packet.item,&_lpPacket->item,sizeof(_lpPacket->item));

		lpGuild->addSendPacketToAllMember(&packet);
	}

	if	(lpActor)
	{
		if	(lpActor->isGuildMaster()	==	FALSE)
		{
			int	iStorage	=	lpGuild->getStorageIndexBySlot(_lpPacket->iGuildInventorySlot);

			lpActor->m_abTodayWithdrawItemCount[iStorage]++;

			lpActor->sendEtcWork(eEW_CHANGE_TODAY_WITHDRAW_GUILD_INVENTORY_ITEM_COUNT,iStorage,lpActor->m_abTodayWithdrawItemCount[iStorage]);
		}

		((cItem *)(&_lpPacket->item))->reviseIncorrectData();

		lpActor->sendAddItem((cItem *)&_lpPacket->item,eIAR_WITHDRAW_FROM_GUILD_INVENTORY);
		lpActor->sendCreateItemLog((cItem*)&_lpPacket->item,eCIM_WITHDRAW_FROM_GUILD_INVENTORY2,lpGuild->m_strName);

		lpActor->addItemToInventory((cItem *)&_lpPacket->item,eGET_GOLD_IGNORE);	//	길드 창고에 넣는 것이니 증가 필요 없다.
		lpActor->immediatelySendSaveDataToDBMemory(TRUE);
	}
}

void
cGAME::opreateStoreGuildInventoryGold(WORLDtoSERVERS_STORE_GUILD_INVENTORY_GOLD *_lpPacket)
{
	cACTOR	*lpActor	=	g_game.getPlayer(_lpPacket->strMemberName);

	if	(_lpPacket->wIsFailed)
	{
		if	(lpActor)
		{
			lpActor->m_iGold	+=	_lpPacket->iGold;

			lpActor->sendRegistServerMessage(eRSM_FAILED_COMMAND_WORK);
		}

		return;
	}

	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->iGuild);

	if	(!lpGuild)
		return;

	if	(lpActor)
		lpActor->sendAddGold(-_lpPacket->iGold,eAGR_STORE_TO_GUILD_INVENTORY);

	lpGuild->storeGold(_lpPacket->iGold);

	if	(IsAvailBCConnect()	==	FALSE)
	{
		SG_STORE_GUILD_INVENTORY_GOLD	packet;

		packet.base.set(sizeof(packet),dSG_STORE_GUILD_INVENTORY_GOLD);
		packet.iGold	=	_lpPacket->iGold;

		lpGuild->addSendPacketToAllMember(&packet);
	}
}

void
cGAME::opreateWithdrawGuildInventoryGold(WORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_GOLD *_lpPacket)
{
	cACTOR	*lpActor	=	g_game.getPlayer(_lpPacket->strMemberName);

	if	(_lpPacket->wIsFailed)
	{
		if	(lpActor)
			lpActor->sendRegistServerMessage(eRSM_FAILED_COMMAND_WORK);

		return;
	}

	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->iGuild);

	if	(!lpGuild)
		return;

	if	(lpActor)
	{
		lpActor->m_iGold	+=	_lpPacket->iGold;
		lpActor->sendAddGold(_lpPacket->iGold,eAGR_WITHDRAW_FROM_GUILD_INVENTORY);

		lpActor->immediatelySendSaveDataToDBMemory(TRUE);

		if	(lpActor->isGuildMaster()	==	FALSE)
		{
			lpActor->m_iTodayWithdrawGoldInGuild	+=	_lpPacket->iGold;

			WORD	wGold1	=	lpActor->m_iTodayWithdrawGoldInGuild>>16;
			WORD	wGold2	=	lpActor->m_iTodayWithdrawGoldInGuild&0xffff;

			lpActor->sendEtcWork(eEW_CHANGE_TODAY_WITHDRAW_GUILD_INVENTORY_GOLD,wGold1,wGold2);
		}

		{
			cItem	item;
			item.reset();
			item.m_wBaseItem	=	dITEM_MONEY;
			item.m_dwSerial		=	_lpPacket->iGold;
			lpActor->sendCreateItemLog(&item,eCIM_WITHDRAW_FROM_GUILD_INVENTORY2,lpGuild->m_strName);
		}
	}

	lpGuild->removeGold(_lpPacket->iGold);

	if	(IsAvailBCConnect()	==	FALSE)
	{
		SG_REMOVE_GUILD_INVENTORY_GOLD	packet;

		packet.base.set(sizeof(packet),dSG_REMOVE_GUILD_INVENTORY_GOLD);
		packet.iGold	=	_lpPacket->iGold;

		lpGuild->addSendPacketToAllMember(&packet);
	}
}

void
cGAME::opreateChangeGuildInventoryItemPlace(WORLDtoSERVERS_CHANGE_GUILD_INVENTORY_ITEM_PLACE *_lpPacket)
{
	cACTOR	*lpActor	=	g_game.getPlayer(_lpPacket->strMemberName);

	if	(_lpPacket->wIsFailed)
	{
		if	(lpActor)
			lpActor->sendRegistServerMessage(eRSM_FAILED_COMMAND_WORK);

		return;
	}

	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->iGuild);

	if	(!lpGuild)
		return;

	lpGuild->changeItemPlace(_lpPacket->iSlot1,_lpPacket->iSlot2);

	if	(IsAvailBCConnect()	==	FALSE)
	{
		SG_CHANGE_GUILD_INVENTORY_ITEM_PLACE	packet;

		packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_INVENTORY_ITEM_PLACE);
		packet.wSlot1	=	_lpPacket->iSlot1;
		packet.wSlot2	=	_lpPacket->iSlot2;

		lpGuild->addSendPacketToAllMember(&packet);
	}
}

void
cGAME::operateSetGuildHallLevel(WORLDtoSERVERS_SET_GUILD_HALL_LEVEL *_lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->iGuildSerial);

	if	(!lpGuild)
		return;

	lpGuild->setGuildHallLevel(_lpPacket->iHallLevel);
}

void
cGAME::operateSetGuildHallRoenGoodWill(WORLDtoSERVERS_SET_GUILD_HALL_ROEN_GOOD_WILL *_lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->iGuildSerial);
	
	if	(!lpGuild)
		return;

	
	
	lpGuild->m_bf3GuildGoodwillForRoen		=	_lpPacket->iGoodWill;
}

//
//	길드 리스트 받음
void
cGAME::operateReceiveGuildList(WORLDtoSERVERS_USER_LISTGUILD *_lpPacket)
{
//	CLOG("길드","[%d] %s",_lpPacket->info.m_wSerial,_lpPacket->info.m_strName);
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->info.m_wSerial);

	if	(!g_guildManager.setGuild(_lpPacket->info.m_wSerial,&_lpPacket->info) &&	g_bIsCompleteReceiveGuildDataWork == FALSE)
	{
		ERRMSG("error","receive guild list\n\nindex %d guild is already exist",_lpPacket->wCount);
		return;
	}
	
	g_game.receiveInitDataFromWorld();

	if	(g_guildManager.m_iGuildCount	==	0)
		printf("begin receive guild list\n");

	g_guildManager.m_iReceiveGuildCount++;

	printf("  [%4d/%4d] [%s:%.4d] receive guild info \n",g_guildManager.m_iReceiveGuildCount,g_guildManager.m_iGuildCount,_lpPacket->info.m_strName,_lpPacket->info.m_wSerial);

	g_guildManager.m_iGuildCount	=	_lpPacket->wAllCount;
}

//
//
void
cGAME::operateReceiveGuildMemberList(WORLDtoSERVERS_USER_GUILDINFO* _lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);
	int		i;

	if (!lpGuild && g_bIsCompleteReceiveGuildDataWork == FALSE)
	{
		ERRMSG("error","receive guild member info\n\ncan not find index %d guild",_lpPacket->wGuildSerial);
		return;
	}

	int	iBufferSize	=	sizeof(_lpPacket->List)/sizeof(CGuildMan);

	if	(_lpPacket->wListCount > iBufferSize	&&	g_bIsCompleteReceiveGuildDataWork == FALSE)
		ERRMSG("error","receive guild member info\n\ncount over[%d/%d]",lpGuild->m_strName,iBufferSize,_lpPacket->wListCount);

	for (i=0;i<_lpPacket->wListCount;i++)
		if	(!lpGuild->setMember(_lpPacket->List[i].wSerial,&_lpPacket->List[i]) &&	g_bIsCompleteReceiveGuildDataWork == FALSE)
			ERRMSG("error","receive guild member info!!\n\nalready exist serial %d member",_lpPacket->List[i].wSerial);

	if (_lpPacket->wListCount	!=	lpGuild->m_wMemberCount)
		printf("error Guild member Count mistmatch!! [%s:%d/%d",lpGuild->m_strName,lpGuild->m_wMemberCount,_lpPacket->wListCount);

	g_game.receiveInitDataFromWorld();

	if	(g_guildManager.m_iReceiveMemberCount	==	0)
		printf("\n\nbegin receive guild member info\n");

	g_guildManager.m_iReceiveMemberCount++;
	printf("  [%4d/%4d] [%s:%.4d] receive member info\n",g_guildManager.m_iReceiveMemberCount,g_guildManager.m_iGuildCount,lpGuild->m_strName,lpGuild->m_wSerial);
}

void
cGAME::operateFinishReceiveGuildInfo(WORLDtoSERVERS_LOGIN* _lpPacket)
{
	strcpy(g_strGameServerName,_lpPacket->strServerName);
	g_guildManager.checkGuildData();

	CGuildBattleManager::SetMaintenanceTime(_lpPacket->stMtTime.bf12MTWeekday,
				_lpPacket->stMtTime.bf4MTStartHour,_lpPacket->stMtTime.bf6MTStartMin,
				_lpPacket->stMtTime.bf4MTEndHour,_lpPacket->stMtTime.bf6MTEndMin);

	SetSystemTime(&_lpPacket->systemTime);

	g_iHeavenRedStoneCount		=	_lpPacket->iHeavenRedStoneCount;
	g_iHellRedStoneCount		=	_lpPacket->iHellRedStoneCount;
	g_iRedDevilRedStoneCount	=	_lpPacket->iRedDevilRedStoneCount;

	if	(g_iSeasonVariable == 0)
		g_iSeasonVariable			=	_lpPacket->wSeasonVariable;

	g_iWorldServerIndex	=	_lpPacket->wWorldServerIndex;
	g_iWorldServerType	=	_lpPacket->iServerType;
	
	//	인도자의 종소리 정보
	{
		g_iBoostExpByLeadersSoundofBell							=	_lpPacket->iBoostExpByLeadersSoundofBell;
		g_iBoostItemDropChanceByLeadersSoundofBell				=	_lpPacket->iBoostItemDropChanceByLeadersSoundofBell;
		g_eventManager.m_wIsDisplayNameWithSoundOfLeadersBell	=	_lpPacket->wDisplayNameWithSoundOfLeadersBell;
		g_eventManager.m_iUpkeepSecondForSoundOfLeadersBell		=	_lpPacket->iRemainSoundofBellTime;

		strcpy(g_eventManager.m_strSoundOfLeadersBellMessage,_lpPacket->strMessageOfSoundOfLeadersBell);
		strcpy(g_eventManager.m_strSoundOfLeadersBellOwner,_lpPacket->strOwnerOfSoundOfLeadersBell);
	}

	g_bIsCompleteReceiveGuildDataWork = TRUE;
	g_guildManager.buildHallInfo();
	g_eventManager.setDBControlledEventValue(_lpPacket->aiEventValue);

	if	(_lpPacket->iServerType	==	eSERVER_TYPE_DUEL)
	{
		if	(g_bIsDuelServer	==	FALSE)
			cMSG::Error("error!!","server type mismatch!!\n\ncheck duel server setting");
		g_bIsDuelServer			=	TRUE;
	}
	else
 	if	(!g_bIsDuelTestPC)
 	{
  		if	(g_bIsDuelServer)
  			cMSG::Error("error!!","server type mismatch!!\n\ncheck duel server setting");

  		g_bIsDuelServer			=	FALSE;
 	}

	if	(g_bIsDuelServer)
	{
		g_duelTrialGameManager.setTrialGameSchedule(_lpPacket->strTrialGameName,
												_lpPacket->wTrialGameBeginYear,_lpPacket->wTrialGameBeginMonth,_lpPacket->wTrialGameBeginDay,
												_lpPacket->wTrialGameBeginHour,_lpPacket->wTrialGameBeginMinute,
												_lpPacket->wTrialGameEndYear,_lpPacket->wTrialGameEndMonth,_lpPacket->wTrialGameEndDay,
												_lpPacket->wTrialGameEndHour,_lpPacket->wTrialGameEndMinute);
		
		if	(g_iTrialGameTeamSize	!=	_lpPacket->wTrialGameTeamSize)
			cMSG::Error("error!!","in change duel match info\n\nmistmatch team size info");
		
		g_iTrialGameTeamSize								=	_lpPacket->wTrialGameTeamSize;
		g_duelTrialGameManager.m_wIsSendTrialGameResultMail	=	_lpPacket->wIsSendTrialGameResultMail;
		
		g_duelTrialGameManager.setOfflineTournamentSchedule(_lpPacket->strOfflineTournamentComment,
			_lpPacket->wOfflineTournamentBeginYear,_lpPacket->wOfflineTournamentBeginMonth,_lpPacket->wOfflineTournamentBeginDay,_lpPacket->wOfflineTournamentBeginHour,_lpPacket->wOfflineTournamentBeginMinute,
			_lpPacket->wOfflineTournamentEndYear,_lpPacket->wOfflineTournamentEndMonth,_lpPacket->wOfflineTournamentEndDay,_lpPacket->wOfflineTournamentEndHour,_lpPacket->wOfflineTournamentEndMinute);
	}

	printf("receive guild data complete\n");
}

void
cGAME::operateReceiveRenewPremiumItemInfo(WORLDtoSERVERS_RENEW_PREMIUM_ITEM* _lpPacket)
{
	m_iRenewPremiumItemCount		=	_lpPacket->wRenewCount;
	m_iLastRenewPremiumItemCounter	=	0;

	for (int i=0;i<m_iRenewPremiumItemCount;i++)
	{
		m_aRenewPremiumItemInfo[i].m_wSerial			=	_lpPacket->aRenewInfo[i].bf14Index;
		m_aRenewPremiumItemInfo[i].m_wIsForOnlyYahoo	=	_lpPacket->aRenewInfo[i].bf1IsForOnlyYahoo;
		m_aRenewPremiumItemInfo[i].m_wIsForOnlyRedGem	=	_lpPacket->aRenewInfo[i].bf1IsForOnlyRedGem;
		m_aRenewPremiumItemInfo[i].m_wExtendTerm		=	_lpPacket->aRenewInfo[i].wTerm;

		m_iLastRenewPremiumItemCounter					=	max(m_iLastRenewPremiumItemCounter,m_aRenewPremiumItemInfo[i].m_wSerial);
	}

	if (_lpPacket->wIsNow)
	{
		for (int iActor=0;iActor<m_iActorCount;iActor++)
		{
			cACTOR	*lpActor	=	&m_pActor[iActor];

			if (lpActor->m_iZoneSerial	==	0xffffffff	||	!lpActor->isPlayer())
				continue;

			lpActor->renewPremiumItem();
		}
	}
}

void
cGAME::operateCreateGuild(WORLDtoSERVERS_GUILD_CREATE* _lpPacket)
{
	if (g_guildManager.setGuild(_lpPacket->info.m_wSerial,&_lpPacket->info)	==	FALSE)
		CLOG("aboutGuild","얼래..;;; %s 길드 이미 있는데 또 만든다는 정보 날라왔어요.. -o-",_lpPacket->info.m_strName);
	else
		g_guildManager.m_iGuildCount++;
}

void
cGAME::operateRemoveGuild(WORLDtoSERVERS_DELETEGUILD* _lpPacket)
{
	if (g_guildManager.removeGuild(_lpPacket->strGuildName)	==	FALSE)
		CLOG("aboutGuild","얼래..;;; %s 길드 없는데 없앤데요.",_lpPacket->strGuildName);
}

//
//	길드 멤버 추가
void
cGAME::operateAddGuildMember(WORLDtoSERVERS_GUILD_JOIN* _lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

	if (!lpGuild)	
	{
		CLOG("aboutGuild","얼래..;;; 멤버를 가입 시키려는데 %s 길드 없는디요.. -l-",_lpPacket->strGuildName);
		return;
	}

	lpGuild->m_wIsWaitGuildWorkResult	=	FALSE;

	if	(lpGuild->addMember(_lpPacket->member.wSerial,&_lpPacket->member))
		CLOG("aboutGuild","얼래..;;; %s 길드에 %s 멤버 이미 있는데 또 추가한다는 정보 날라왔어요.. -o-",lpGuild->m_strName,_lpPacket->member.m_strName);
}

//
//
void
cGAME::operateRemoveGuildMember(WORLDtoSERVERS_GUILD_EXIT* _lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

	if (!lpGuild)
	{
		CLOG("aboutGuild","얼래..;;; 멤버를 탈퇴 시키려는데 %s 길드 없는디요.. -l-",_lpPacket->strGuildName);
		return;
	}

	lpGuild->m_wIsWaitGuildWorkResult	=	FALSE;

	if	(!lpGuild->removeMember(_lpPacket->strMemberName))
		CLOG("aboutGuild","얼래..;;; %s 길드에 %s 멤버 없는데 또 없앤다는 정보 날라왔어요.. -o-",lpGuild->m_strName,_lpPacket->strMemberName);
}

//
//
void
cGAME::operateLeaveGuild(WORLDtoSERVERS_USER_EXITGUILD* _lpPacket)
{
	switch(_lpPacket->wResult)
	{
		case	dEXITGUILD_RESULT_SUCCESS		:
			break;

		case	dEXITGUILD_RESULT_NOTFIND		://	그런 길드는 없는데 .. ;
		case	dEXITGUILD_RESULT_NOTJOIN		://	넌 길드 없스 ~~
		case	dEXITGUILD_RESULT_NOTACOUNT		://	정보가 일치하지 않습니다.
		case	dEXITGUILD_RESULT_FAIL			:	//	뭘까~~~
		case	dEXITGUILD_RESULT_NEEDRANK		:	//	뭘까~~~
		case	dEXITGUILD_RESULT_FAILED_BY_TARGET_IS_GUILD_MASTER	:	//	뭘까~~~
		case	dEXITGUILD_RESULT_FAILED_BY_TARGET_IS_ELDER			:
		case	dEXITGUILD_RESULT_FAILED_BY_IT_IS_ABLE_ONLY_MASTER	:
			sendWPAskGuildInfo(_lpPacket->wGuildSerial);
			return;
	}

	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

	if	(!lpGuild)
	{
		sendWPAskGuildInfo(_lpPacket->wGuildSerial);

		return;
	}

	lpGuild->m_wIsWaitGuildWorkResult	=	FALSE;

	g_game.sendLeaveGuildLog(lpGuild->m_strName,_lpPacket->strTargetName);
	lpGuild->removeMember(_lpPacket->strTargetName);
}

//
//	길드 참가 결과
void
cGAME::operateJoinGuildResult(WORLDtoSERVERS_USER_JOINGUILD* _lpPacket)
{
	cACTOR	*lpActor	=	getPlayer(_lpPacket->strTargetName);

	if	(lpActor)
	{
		switch(_lpPacket->wResult)
		{
			case	dJOINGUILD_RESULT_SUCCESS	:
			{
				if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
				{
					memset(lpActor->m_aWayPoint,0xff,sizeof(lpActor->m_aWayPoint));
					lpActor->sendSaveDataToDBCache();
					break;
				}
				break;
			}

			case	dJOINGUILD_RESULT_NOTFIND	:	//	그런 길드는 없는데 .. ;
				lpActor->sendJoinGuildResult(eAJGR_TARGET_NOT_GUILD_MASTER);
				break;
			case	dJOINGUILD_RESULT_FULL		:	//	푸 ~~ 울
				lpActor->sendJoinGuildResult(eAJGR_GUILD_MEMBER_FULL);
				break;
			case	dJOINGUILD_RESULT_JOINED	:	//	이미 다른 길드에 조인되어 있다.
				lpActor->sendJoinGuildResult(eAJGR_ALREADY_GUILD_MEMBER);
				break;
			case	dJOINGUILD_RESULT_FAIL		:	//	뭘까~~~
				lpActor->sendJoinGuildResult(eAJGR_FAILED);
				break;
		}
	}

	CGuild	*lpGuild	=	g_guildManager.getByName(_lpPacket->strName);

	if	(lpGuild)
		lpGuild->m_wIsWaitGuildWorkResult	=	FALSE;

	g_game.sendJoinGuildLog(_lpPacket->strName,_lpPacket->strTargetName);

	lpActor	=	getPlayer(_lpPacket->strMasterName);

	if	(lpActor)
	{
		if	(_lpPacket->wResult	==	dJOINGUILD_RESULT_SUCCESS)
			lpActor->sendGuildWorkMessage(eGWM_COMPLETE_ASK_WORK);
		else
			lpActor->sendGuildWorkMessage(eGWM_FAILED_ASK_WORK);
	}
}

//
//	길드 경험치 업데이트
void
cGAME::operateUpdateGuildExp(WORLDtoSERVERS_GUILD_UPDATE_EXP* _lpPacket)
{
	for (int i=0;i<_lpPacket->wCount;i++)
	{
		CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->aExpData[i].m_wGuildSerial);

		if (!lpGuild)
			continue;

		lpGuild->updateExp(_lpPacket->aExpData[i].m_wLevel,_lpPacket->aExpData[i].m_dwUpdateExp,_lpPacket->aExpData[i].m_wGuildPoint,_lpPacket->aExpData[i].m_wExpTaxRate);
	}
}

void
cGAME::operateUpdateGuildNotice(WORLDtoSERVERS_GUILD_NOTICE* _lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

	if	(!lpGuild)
	{
		sendWPAskGuildInfo(_lpPacket->wGuildSerial);

		return;
	}

	lpGuild->changeNotice(_lpPacket->strNotice);
}


void
cGAME::operateChangeGuildExpTaxRate(WORLDtoSERVERS_GUILD_EXPRATE* _lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

	if	(!lpGuild)
	{
		sendWPAskGuildInfo(_lpPacket->wGuildSerial);

		return;
	}

	lpGuild->changeExpTaxRate(_lpPacket->wRateExp);
}

void
cGAME::operateUpdateGuildHallInfo(WORLDtoSERVERS_UPDATE_GUILD_HALL_INFO *_lpPacket)
{
	g_guildManager.updateGuildHallInfo(_lpPacket->aHallList,_lpPacket->wCount,_lpPacket->wIsIsBuildSiegeWarfareSchedule);
}

void
cGAME::operateChangeRankInGuild(WORLDtoSERVERS_USER_CHANGERANK* _lpPacket)
{
	switch(_lpPacket->wResult)
	{
		case dCHANGERANK_RESULT_SUCCESS		:
			break;

		case dCHANGERANK_RESULT_NOTFIND		:	//	그런 길드는 없는데 .. ;
		{
			sendWPAskGuildInfo(_lpPacket->wGuildSerial);
			return;
		}
		case dCHANGERANK_RESULT_NOTACOUNT	:	//	정보가 일치하지 않습니다.
		{
			sendWPAskGuildInfo(_lpPacket->wGuildSerial);
			return;
		}
		case dCHANGERANK_RESULT_FAIL		:	//	뭘까~~~
		{
			sendWPAskGuildInfo(_lpPacket->wGuildSerial);
			return;
		}

		default		:
			return;
		
	}

	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);
	
	if	(!lpGuild)
	{
		sendWPAskGuildInfo(_lpPacket->wGuildSerial);

		return;
	}

	cACTOR	*lpActor	=	getPlayer(_lpPacket->strMaster);

	if	(lpActor)
		lpActor->sendGuildWorkMessage(eGWM_COMPLETE_ASK_WORK);
}

void
cGAME::operateChangeRankResult(WORLDtoSERVERS_USER_CHANGERANK_RESULT* _lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

	if	(!lpGuild)
	{
		sendWPAskGuildInfo(_lpPacket->wGuildSerial);

		return;
	}

	lpGuild->m_wIsWaitGuildWorkResult	=	FALSE;

	lpGuild->changeRank(_lpPacket->strTarget,_lpPacket->wRank);
}

void
cGAME::operateUpdateGuildSkillLevel(WORLDtoSERVERS_UPDATE_GUILD_SKILL_LEVEL * _lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

	if	(!lpGuild)
	{
		sendWPAskGuildInfo(_lpPacket->wGuildSerial);

		return;
	}

	lpGuild->m_wIsWaitGuildWorkResult	=	FALSE;
	lpGuild->updateSkillLevel(_lpPacket->wSkill,_lpPacket->wLevel,_lpPacket->wGuildPoint);
}

//
//	길드전 스케쥴을 받았다.
void
cGAME::receiveGuildBattleSchedule(WORLDtoSERVERS_GUILD_BATTLE_SCHEDULE* _lpPacket)
{
	if	(g_bIsTestGuildBattle)
		return;

	memcpy(&g_gbsToday,&_lpPacket->todaySchedule,sizeof(g_gbsToday));

	g_gbsToday.setByToday();
	g_gbsToday.resetFieldSerial();

	if	(g_bIsReceivedGuildBattleSchedule	==	FALSE)
		g_gbsToday.init();	//	초기화

	g_bIsReceivedGuildBattleSchedule	=	TRUE;
	g_bIsTestGuildBattle				=	FALSE;
}

//
//	길드전 스케쥴을 받았다.
void
cGAME::receiveGuildBattleInfo(WORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO* _lpPacket)
{
	if	(_lpPacket->wFieldOrder	==	0xffff)
	{
		g_gbsToday.resetGuildBattle(_lpPacket->wTimeOrder);

		return;
	}

	g_game.receiveInitDataFromWorld();

	g_gbsToday.updateBattleInfo(_lpPacket->wTimeOrder,_lpPacket->wFieldOrder,_lpPacket->awBattleGuild[0],_lpPacket->awBattleGuild[1]);

	if	(_lpPacket->wGuildSerial	!=	0xffff)
	{
		CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

		if	(!lpGuild)
			sendWPAskGuildInfo(_lpPacket->wGuildSerial);
		else
		{
			cACTOR	*lpMaster	=	getPlayer(lpGuild->m_strMasterName);

			if	(lpMaster)
			{
				lpMaster->sendGuildBattleInfoForMember();

				CGuildBattleInfo	*lpGuildBattleInfo	=	g_gbsToday.getBattleInfoByGuild(_lpPacket->wGuildSerial);

				if	(lpGuildBattleInfo)
					lpMaster->sendChangeGuildBattleSchedule(_lpPacket->wTimeOrder,_lpPacket->wFieldOrder,lpGuildBattleInfo);
				else
				{
					lpGuildBattleInfo	=	g_gbsToday.getBattleInfo(_lpPacket->wTimeOrder,_lpPacket->wFieldOrder);	//	길드전 정보 얻기

					if	(lpGuildBattleInfo)
						lpMaster->sendChangeGuildBattleSchedule(_lpPacket->wTimeOrder,_lpPacket->wFieldOrder,lpGuildBattleInfo);
				}
			}
		}
	}

	CGuild	*lpGuild1	=	g_guildManager.getBySerial(_lpPacket->awBattleGuild[0]);
	CGuild	*lpGuild2	=	g_guildManager.getBySerial(_lpPacket->awBattleGuild[1]);

	if	(lpGuild1 &&	lpGuild2 && getFieldByUniqueSerial(0))
		sendEngageGuildBattleLog(lpGuild1->m_strName,lpGuild2->m_strName,_lpPacket->wTimeOrder,_lpPacket->wFieldOrder);
}

//
//	길드 정보 업데이트
void
cGAME::receiveGuildInfo(WORLDtoSERVERS_UPDATE_GUILD_INFO* _lpPacket)
{
	if	(_lpPacket->wGuildSerial	>=	dGUILD_MAX_COUNT)	
		return;

	g_game.receiveInitDataFromWorld();

	CGuild	*lpGuild	=	&g_guildManager.m_aGuild[_lpPacket->wGuildSerial];

	g_guildManager.setGuild(_lpPacket->wGuildSerial,&_lpPacket->info);

	if	(_lpPacket->wIsRequireResetMemberInfo)
		memset(g_guildManager.m_aGuild[_lpPacket->wGuildSerial].m_aMembers,0xff,sizeof(g_guildManager.m_aGuild[_lpPacket->wGuildSerial].m_aMembers));
}

//	벨리드 길드
void
cGAME::receiveValidGuild(WORLDtoSERVERS_VALID_GUILD* _lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

	if	(lpGuild)
		g_guildManager.removeGuild(lpGuild->m_strName);
}

//
//	길드전 결과 받음
void
cGAME::receiveGuildBattleResult(WORLDtoSERVERS_GUILD_BATTLE_RESULT *_lpPacket)
{
	int	aGuildVP[2];

	for (int i=0;i<2;i++)
	{
		aGuildVP[i]						=	0;
		CGuildBattleResultInfo	*lpInfo	=	&_lpPacket->aBattleGuild[i];

		int		iTargetGuildSerial		=	_lpPacket->aBattleGuild[1-i].m_wGuildSerial;
		CGuild	*lpGuild				=	g_guildManager.getBySerial(lpInfo->m_wGuildSerial);
		CGuild	*lpTargetGuild			=	g_guildManager.getBySerial(iTargetGuildSerial);

		if	(!lpGuild)
		{	
			if	(lpInfo->m_wGuildSerial != 0xffff)
				sendWPAskGuildInfo(lpInfo->m_wGuildSerial);

			continue;
		}

		aGuildVP[i]						=	lpInfo->m_iVictoryPoint-lpGuild->m_iVictoryPoint;
		lpGuild->m_iVictoryPoint		=	lpInfo->m_iVictoryPoint;
		lpGuild->m_wVictoryContinue		=	lpInfo->m_wVictoryContinue;
		lpGuild->m_wVictoryCount		=	lpInfo->m_wVictoryCount;
		lpGuild->m_wDefeatCount			=	lpInfo->m_wDefeatCount;
		lpGuild->m_wDrawCount			=	lpInfo->m_wDrawCount;
		lpGuild->m_wCancelCount			=	lpInfo->m_wCancelCount;

		SG_GUILD_BATTLE_RESULT	packet;

		packet.wGuildSerial				=	lpGuild->m_wSerial;
		packet.base.set(sizeof(packet),dSG_GUILD_BATTLE_RESULT);
		memcpy(&packet.battleResult,lpInfo,sizeof(CGuildBattleResultInfo));

		if	(lpTargetGuild)
			strcpy(packet.strTargetGuild,lpTargetGuild->m_strName);
		else
			strcpy(packet.strTargetGuild,"unknown guild");

		lpGuild->addSendPacketToAllMember((cMSG_BASE_TYPE *)&packet);			
	}

	CGuild	*lpGuild1	=	g_guildManager.getBySerial(_lpPacket->aBattleGuild[0].m_wGuildSerial);
	CGuild	*lpGuild2	=	g_guildManager.getBySerial(_lpPacket->aBattleGuild[1].m_wGuildSerial);

	if	(lpGuild1 && lpGuild2 && getFieldByUniqueSerial(0))
		sendResultOfGuildBattleLog(lpGuild1->m_strName,lpGuild2->m_strName,aGuildVP[0],aGuildVP[1]);

}

//
//	길드마크 선택결과 받음
void
cGAME::receiveSelectGuildMark(WORLDtoSERVERS_SELECT_GUILD_MARK *_lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

	if	(!lpGuild)
	{
		sendWPAskGuildInfo(_lpPacket->wGuildSerial);

		return;
	}

	lpGuild->selectGuildMark(&_lpPacket->guildMarkInfo,_lpPacket->bf1IsUseGuildMarkPack);
}

void
cGAME::receiveUpdateGuildMemberLevel(WORLDtoSERVERS_UPDATE_GUILD_MEMBER_LEVEL* _lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

	if	(!lpGuild)
	{
		sendWPAskGuildInfo(_lpPacket->wGuildSerial);

		return;
	}

	lpGuild->updateGuildMemberLevel(_lpPacket->strName,_lpPacket->wLevel,FALSE);
}


void
cGAME::receiveWorldValueInfoAll(WORLDtoSERVERS_WORLD_VALUE_INFO_ALL	*_lpPacket)
{
	memcpy(g_aiWorldValue  , _lpPacket->aiWouldValue , sizeof(int) * dMAX_WORLD_VALUE_MAX);
}

void
cGAME::receiveWorldValueInfo(WORLDtoSERVERS_WORLD_VALUE_INFO	*_lpPacket)
{
	g_aiWorldValue[_lpPacket->wIndex] = _lpPacket->iValue;
}

//
//	OX 퀴즈 예약 
void
cGAME::receiveAppointmentQuizEvent(WORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT	*_lpPacket)
{
	if	(_lpPacket->wType	==	eET_OX_QUIZ)
		g_eventManager.setOXQuizData(_lpPacket->wBeginYear,_lpPacket->wBeginMonth,_lpPacket->wBeginDay,_lpPacket->wBeginHour,_lpPacket->wBeginMinute,_lpPacket->wGateVillage,_lpPacket->wLimitPlayerCount,_lpPacket->wIsTest);

	if	(_lpPacket->wType	==	eET_WORD_QUIZ)
		g_eventManager.getWordQuiz()->setData(_lpPacket->wBeginYear,_lpPacket->wBeginMonth,_lpPacket->wBeginDay,_lpPacket->wBeginHour,_lpPacket->wBeginMinute,_lpPacket->wGateVillage,_lpPacket->wLimitPlayerCount,_lpPacket->wIsTest, _lpPacket->strEventName);
}

//
//	OX 퀴즈 종료
void
cGAME::receiveEndQuizEvent(WORLDtoSERVERS_END_QUIZ_EVENT *_lpPacket)
{
	if	(_lpPacket->wType	==	eET_OX_QUIZ)
		g_eventManager.endOXQuizEvent();	//	OX 퀴즈 종료

	if	(_lpPacket->wType	==	eET_WORD_QUIZ)
		g_eventManager.getWordQuiz()->endEvent();
}

void
cGAME::receiveEventWordInfo(WORLDtoSERVER_EVENT_WORD_INFO *_lpPacket)
{// 마지막 수정일 : 09.10.09
	g_eventManager.m_wordCompareEvent.m_dwTimeMinite				= _lpPacket->dwTimeInfo;		//시간 정보 세팅
	g_eventManager.m_wordCompareEvent.m_bf16EventKind				= _lpPacket->bf16EventKind;		// 이벤트 종류
	g_eventManager.m_wordCompareEvent.m_bf16BuffSerial				= _lpPacket->bf16BuffSerial;		//	이벤트 버프 시리얼
	g_eventManager.m_wordCompareEvent.m_bIsActiveWordCompareEvent	= _lpPacket->bIsActiveEvent;	//지금 실행해라.후훗.

	g_eventManager.m_wordCompareEvent.m_wBeginTimeYear			= _lpPacket->wBeginTimeYear;
	g_eventManager.m_wordCompareEvent.m_wBeginTimeMonth			= _lpPacket->wBeginTimeMonth;
	g_eventManager.m_wordCompareEvent.m_wBeginTimeDay			= _lpPacket->wBeginTimeDay;
	g_eventManager.m_wordCompareEvent.m_wBeginTimeHour			= _lpPacket->wBeginTimeHour;
	g_eventManager.m_wordCompareEvent.m_wBeginTimeMinite		= _lpPacket->wBeginTimeMinite;
	g_eventManager.m_wordCompareEvent.m_wEndTimeYear			= _lpPacket->wEndTimeYear;
	g_eventManager.m_wordCompareEvent.m_wEndTimeMonth			= _lpPacket->wEndTimeMonth;
	g_eventManager.m_wordCompareEvent.m_wEndTimeDay				= _lpPacket->wEndTimeDay;
	g_eventManager.m_wordCompareEvent.m_wEndTimeHour			= _lpPacket->wEndTimeHour;
	g_eventManager.m_wordCompareEvent.m_wEndTimeMinite			= _lpPacket->wEndTimeMinite;

	
	strcpy(g_eventManager.m_wordCompareEvent.m_strBeginComment,_lpPacket->strBeginMent);		//시작 멘트.
	strcpy(g_eventManager.m_wordCompareEvent.m_strEndComment,_lpPacket->strEndMent);			//엔딩 멘트.
	strcpy(g_eventManager.m_wordCompareEvent.m_strActiveComment,_lpPacket->strActiveMent);			//엑티브 멘트.
	strcpy(g_eventManager.m_wordCompareEvent.m_strEventWord,_lpPacket->strEventWord);			//이벤트 키워드.
}
//
//	OX 퀴즈 취소
void
cGAME::receiveCancelQuizEvent(WORLDtoSERVERS_CANCEL_QUIZ_EVENT *_lpPacket)
{
	if	(_lpPacket->wType	==	eET_OX_QUIZ)
		g_eventManager.cancelOXQuiz();

	if	(_lpPacket->wType	==	eET_WORD_QUIZ)
		g_eventManager.getWordQuiz()->cancelQuiz();
}

void
cGAME::receiveEventTimeData(SERVERStoWORLD_MODIFY_EVENT_TIME *_lpPacket)
{
	g_eventTimeManager.updateData(_lpPacket->wCount,_lpPacket->aEventList);

	printf("- receive %d numbers event time data\n",_lpPacket->wCount);

	g_bIsCompleteReceiveEventTimeData	=	TRUE;
}

void
cGAME::receiveLottoEventData(WORLDtoSERVERS_LOTTO_EVENT_DATA *_lpPacket)
{
	g_lottoEventManager.updateData(_lpPacket->wCount,_lpPacket->aEventList);

	g_bIsCompleteReceiveLottoEventData	=	TRUE;

	printf("- receive %d numbers lotto event data\n",_lpPacket->wCount);
}

void
cGAME::receiveProduceOxQuiz(WORLDtoSERVERS_PRODUCE_OX_QUIZ *_lpPacket)
{
	cFIELD	*lpField	=	getOXQuizField();

	if	(!lpField)
		return;

	g_eventManager.setOXQuizQuestion(_lpPacket->strQuestion,_lpPacket->wSolution,_lpPacket->wCount+dOX_QUIZ_SPARE_COUNT);
	lpField->producedOxQuizQuestion();	//	문제 제출
}

//
//	OX 퀴즈 패자 부활전
void
cGAME::receiveOXQuizLoserReviveBattle()
{
	cFIELD	*lpField	=	getOXQuizField();

	if	(!lpField)
		return;

	lpField->launchOXQuizLoserReviveBattle();	//	OX 퀴즈 패자 부활전 실시!
}

void
cGAME::receiveChangeWordQuizRound(WORLDtoSERVERS_CHANGE_WORD_QUIZ_ROUND *_lpPacket)
{
	cFIELD	*lpField	=	getWordQuizField();

	if	(!lpField)
		return;

	lpField->changeWordQuizRound();	//	정답 추가
}

void
cGAME::receiveProduceWordQuiz(WORLDtoSERVERS_PRODUCE_WORD_QUIZ *_lpPacket)
{
	cFIELD	*lpField	=	getWordQuizField();

	if	(!lpField)
		return;

	g_eventManager.getWordQuiz()->setQuestion(_lpPacket->strQuestion,_lpPacket->strSolution,_lpPacket->wCount+dOX_QUIZ_SPARE_COUNT);
	lpField->producedWordQuizQuestion();	//	문제 제출
}

void
cGAME::receiveBeginTestGuildBattle(WORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE* _lpPacket)
{
	if	(g_bIsTestGuildBattle)
		return;

	if	(g_bIsTestServer	==	FALSE)
		return;
	
	g_bIsTestGuildBattle	=	FALSE;

	g_gbsToday.initForTest(_lpPacket->strGuild1,_lpPacket->strGuild2,_lpPacket->wTime,_lpPacket->wField);
}

void
cGAME::receiveRemoveFriendInTargetList(WORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST *_lpPacket)
{
	cACTOR	*lpTarget	=	getPlayer(_lpPacket->strTargetName);

	if	(!lpTarget)
	{
		sendWPRemoveFriendInTargetListResult(_lpPacket->strClientName,_lpPacket->strTargetName,eRFITLR_NOT_CONNECTED);

		return;
	}

	g_game.sendBPRemoveFriend(_lpPacket->strTargetName,_lpPacket->strClientName);

	lpTarget->sendAskRemoveFriend(_lpPacket->strClientName);
}

void
cGAME::receiveRemoveFriendInTargetListResult(WORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST_RESULT* _lpPacket)
{
	cACTOR	*lpClient	=	getPlayer(_lpPacket->strClientName);

	if	(!lpClient)
		return;

	lpClient->sendAskRemoveFriendResult(_lpPacket->strTargetName,_lpPacket->wResult);
}

void
cGAME::receiveCalling(WORLDtoSERVERS_CALLING* _lpPacket)
{
	cParty	party;

	if	(!g_pPartyManager->getParty(&party,_lpPacket->wParty))
		return;
	cACTOR* lpCallingCaster	=	party.getActorByMemberNameHashCode(_lpPacket->dwNameHashCode);

	for (int i=0;i<(int)party.m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	party.getActorByMemberIndex(i);

		if	(!lpActor || lpActor->m_dwNameHashCode == _lpPacket->dwNameHashCode)
			continue;

		if	(lpActor->m_bf1IsProcessEvent	||	lpActor->m_wIsWaitCartMessage	||	lpActor->m_lpField->isCanNotUseTeleportEffectField())
			continue;

		if	(lpActor->m_wCurrentField	>=	dINSTANCE_FIELD_FLAG)
			continue;



		if	(_lpPacket->wCasterField	!=	lpActor->m_wCurrentField)
		{
			int	iRange	=	g_fieldInfo.getDistance(_lpPacket->wCasterField,lpActor->m_wCurrentField);

			if	(iRange	>	_lpPacket->wLimitRange)
				continue;
			lpActor->m_bf1IsTeleportToPointAround	=	TRUE;
			lpActor->stop();
			g_pMoveFieldUserManager->booking(lpActor,NULL,_lpPacket->wCasterField,0x7fff,_lpPacket->wCaterXPos,_lpPacket->wCaterYPos);

			continue;
		}

		if	(lpActor->m_wBiter	!=	0xffff)
		{
			cACTOR	*lpCaster	=	lpActor->m_lpField->getActor(lpActor->m_wBiter);

			if	(lpCaster)
				lpCaster->stop();
		}

		cPOINT	posCalling = {_lpPacket->wCaterXPos,_lpPacket->wCaterYPos};

		lpActor->m_lpField->findEnterancePosToPointAroundTile(&posCalling,1);

		lpActor->stop();
		lpActor->changePos(posCalling.x,posCalling.y);	//	위치 바꾸고 본인과 다른이들한테 메시지 전송
		lpActor->releaseIcyStalagmite(eRUIS_RELEASE);
	}
}

//
//	길드 스킬레벨 올리기 실패
void
cGAME::operateGuildSkillLevelIncreaseFailure(WORLDtoSERVERS_GUILD_SKILL_LEVEL_INCREASE_FAILURE * _lpPacket)
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

	if(!lpGuild)
	{
		sendWPAskGuildInfo(_lpPacket->wGuildSerial);

		return;
	}

	cACTOR	*lpMaster	=	getPlayer(lpGuild->m_strMasterName);

	if (!lpMaster)	return;

	if (_lpPacket->wResult	==	eRIGSL_LEVEL_SKILL_IS_MAX)
		lpMaster->sendGuildWorkMessage(eGWM_CAN_NOT_INCREASE_LEVEL_SKILL);

	if (_lpPacket->wResult	==	eRIGSL_SKILL_POINT_LACKS)
		lpMaster->sendGuildWorkMessage(eGWM_NOT_REMAIN_GUILD_SKILL_POINT);
}

//
//	귓말 처리 결과
void
cGAME::operateSayResult(WORLDtoSERVERS_USER_SAY_RESULT *_lpPacket)
{
	cACTOR	*lpActor	=	getActor(_lpPacket->dwSenderSerial);

	if	(!lpActor || !lpActor->isPlayer())
		return;

	switch(_lpPacket->wReason)
	{
		case	eROFS_SUCCESS					:
		{
			SG_SAY_RESULT	packet;

			packet.base.set(sizeof(SG_SAY_RESULT),dSG_SAY_RESULT);

			packet.wResult	=	_lpPacket->wReason;
			
			if	(lpActor->isTester())
				lpActor->sendOperatorChatLog(eCT_SAY,FALSE,_lpPacket->strDestName,_lpPacket->strMsg);

			strncpy(packet.strName,_lpPacket->strDestName,sizeof(packet.strName));
			strncpy(packet.strMessage,_lpPacket->strMsg,sizeof(packet.strMessage));

			g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
			break;
		}

		case	eROFS_TARGET_DENIED_SAY_MESSAGE	:	//	수신 거부 상태
		case	eROFS_CAN_NOT_FIND_TARGET		:	//	타겟을 못찾겠는디유.. -o-;;
		{
			SG_SAY_RESULT	packet;

			packet.base.set(sizeof(SG_SAY_RESULT),dSG_SAY_RESULT);
			packet.wResult		=	_lpPacket->wReason;
			strncpy(packet.strName,_lpPacket->strDestName,sizeof(packet.strName));
			packet.base.wSize	=	sizeof(SG_SAY_RESULT)-sizeof(packet.strMessage);

			g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
			break;
		}

		case	eROFS_TARGET_IS_OPERATOR		:
		{
			SG_SAY_RESULT	packet;

			packet.base.set(sizeof(SG_SAY_RESULT),dSG_SAY_RESULT);
			packet.wResult		=	_lpPacket->wReason;

			packet.base.wSize	=	sizeof(cMSG_BASE_TYPE)+2;

			g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
			break;
		}
	}
}

//
//	대상이 커뮤니티 거부 상태여
void
cGAME::operateIsDenyCommunityTarget(WORLDtoSERVERS_IS_DENY_COMMUNITY_TARGET *_lpPacket)
{
	cACTOR	*lpActor		=	getPlayer(_lpPacket->strSenderName);

	if (!lpActor)
		return;

	SG_SAY_RESULT	packet;

	packet.base.set(sizeof(SG_SAY_RESULT),dSG_SAY_RESULT);
	packet.wResult		=	eROFS_TARGET_DENIED_SAY_MESSAGE;

	strncpy(packet.strName,_lpPacket->strReceiverName,sizeof(packet.strName)-1);
	packet.base.wSize	=	sizeof(SG_SAY_RESULT)-sizeof(packet.strMessage);

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

}

//	귓말 처리
void
cGAME::operateSayMessage(WORLDtoSERVERS_USER_SAY *_lpPacket)
{
	if	(strlen(_lpPacket->strSendName)	>=	sizeof(_lpPacket->strSendName)	)
		return;
	if	(strlen(_lpPacket->strRecvName)	>=	sizeof(_lpPacket->strRecvName)	)
		return;
	if	(strlen(_lpPacket->strMsg)		>=	sizeof(_lpPacket->strMsg)		)
		return;

	cACTOR	*lpReceiver	=	getPlayer(_lpPacket->strRecvName);
	cACTOR	*lpTalker	=	getPlayer(_lpPacket->strSendName);

	if	(!lpReceiver)
		return;

	if	(lpReceiver->isDenyCommunity()	&&	_lpPacket->wIsByOperator	==	FALSE)
	{
		g_game.sendWPIsCommunityDenyStatus(_lpPacket->strSendName,_lpPacket->strRecvName);

		return;
	}

	if	(lpReceiver->isTester())
		lpReceiver->sendOperatorChatLog(eCT_SAY,TRUE,_lpPacket->strSendName,_lpPacket->strMsg);

	SG_CHAT		packet;

	packet.bf6ChatType	=	eCT_SAY;
	packet.bf11Serial	=	lpReceiver->m_wSerialInField;

	FillChattingBuffer(&packet,_lpPacket->strSendName,_lpPacket->strMsg);

	if	(lpTalker)
		if	(lpTalker->m_wOperatorLevel >= eAL_BOSS)
			packet.bf6ChatType	+=	0x20;

	if	(IsAvailBCConnect())
		g_game.deliverPacket(lpTalker,&packet,lpReceiver->getBCSerial());
	else
		g_userPM.add(lpReceiver->m_iClientSerial,&packet,packet.base.wSize);
}

void
cGAME::operateGuildMemberJoinParty(WORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY *_lpPacket)
{
	cACTOR	*lpPlayer	=	getPlayer(_lpPacket->strTargetName);

	int		iResult		=	eGMJPA_OK;

	if	(!lpPlayer)
		iResult	=	eGMJPA_FAILED;
	else
	{
		cParty	*lpParty	=	lpPlayer->getParty();

		if	(lpParty)
			iResult	=	eGMJPA_FAILED;
		if	(lpPlayer->isDeath())
			iResult	=	eGMJPA_FAILED;
		
		if	(lpPlayer->m_wPitchmanShopSerial	!=	0xffff)
		{
			iResult = eGMJPA_FAILED;
			return;
		}
		
		if	(lpPlayer->m_wOperatorLevel >= eAL_BOSS)
		{
			iResult = eGMJPA_FAILED;
			return;
		}
		
		if	(lpPlayer->isDenyCommunity()	==	TRUE)
		{
			iResult = eGMJPA_FAILED;
			return;
		}
	}

	if	(iResult	==	eGMJPA_FAILED)
	{
		sendWPGuildMemberJoinPartyAnswer(_lpPacket->strTargetName,_lpPacket->strAskerName,iResult);
		return;
	}

	lpPlayer->sendJoinParty(_lpPacket->wAskerLevel,_lpPacket->wAskerJob,_lpPacket->strAskerName,FALSE, TRUE);
}

void
cGAME::operateGuildMemberJoinPartyAnswer(WORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY_ANSWER *_lpPacket)
{
	cACTOR	*lpPlayer	=	getPlayer(_lpPacket->strAskerName);

	if	(!lpPlayer)	
		return;

	switch(_lpPacket->wAnswer)
	{
	case	eGMJPA_DENY:
		lpPlayer->sendPartyMessage(ePM_DENY_INVITE_MESSAGE);
		break;
	case	eGMJPA_HAVE_NO_TARGET:
		lpPlayer->sendPartyMessage(ePM_TARGET_NOT_EXIST);
		break;
	case	eGMJPA_TARGET_IS_IN_PARTY:
		lpPlayer->sendPartyMessage(ePM_TARGET_BELONG_TO_ALREADY_AT_DIFFERENT_PARTY);
		break;
	case	eGMJPA_PARTY_IS_FULL:
		lpPlayer->sendPartyMessage(ePM_ALREADY_FULL_PARTY);
		break;
	case	eGMJPA_FAILED:
		lpPlayer->sendPartyMessage(ePM_FAILED_TO_JOIN_PARTY);
		break;
	}
}
//
//	판매할 아이템 생성 결과
void
cGAME::operateCreateShopItemResult(WORLDtoSERVERS_CREATE_SELL_ITEM_RESULT *_lpPacket)
{ 

	cACTOR	*lpClient	=	getActor(_lpPacket->iSerialInServer);

	if (!lpClient || STRCMP(lpClient->m_strName,_lpPacket->strName) !=0)
		return;

	SG_BUY_ITEM		packet;

	packet.base.set(sizeof(SG_BUY_ITEM),dSG_BUY_ITEM);

	LONGLONG	llGold			=	_lpPacket->dwPrice*_lpPacket->wCount;

	packet.dwPrice				=	(int)_lpPacket->dwPrice;		//	가격
	packet.bf1IsPigeonPostItem	=	FALSE;
	packet.wTokenShop			=	_lpPacket->wIsTokenShop;

	int iItemCount = lpClient->getItemCount( _lpPacket->wIsTokenShop,FALSE);

	if	(_lpPacket->wIsDuelPointShop)
	{
		if	(lpClient->m_duelRecord.m_iDuelPoint	<	llGold)
		{
			lpClient->sendFailedBuyItem(eFRBI_LACK_OF_DUEL_POINT);
			return;
		}
	}
	else 
	if	(_lpPacket->wIsTokenShop != 0xffff &&  iItemCount < packet.dwPrice)
	{
		lpClient->sendFailedBuyItem(eFRBI_LACK_OF_TOKEN);
		return;
	}
	else
	if	(_lpPacket->wIsTokenShop == 0xffff && lpClient->m_iGold	<	llGold)
	{
		lpClient->sendFailedBuyItem(eFRBI_LACK_OF_GOLD);
		return;
	}
	
	int	iBuyCount	=	0;
	int	iAddResult	=	0;

	for	(int i=0;i<_lpPacket->wCount;i++)
	{
		cItem	itemAdd;

		itemAdd.copy((cItem *)&_lpPacket->item);

		itemAdd.m_dwSerial	=	_lpPacket->dwSerial;

		_lpPacket->dwSerial++;

		if	(_lpPacket->dwSerial>= 0xffffffff )
			_lpPacket->dwSerial	=	1;

		memcpy(&packet.aBuyItem[iBuyCount],&itemAdd,sizeof(cItem));	//	아이템

		if	(itemAdd.isMoneyItem())
			CLOG("goldIOError","operateCreateShopItemResult");

		iAddResult	=	lpClient->addItemToInventory(&itemAdd);

		if	(iAddResult	!=	eUIR_OK)
		{
			switch(iAddResult)
			{
				case	eUPR_INVENTORY_FULL				:
					iAddResult	=	eFRBI_INVENTORY_FULL;
					break;

				case	eUAIR_CAN_NOT_OWN_SAME_ITEM		:
					iAddResult	=	eFRBI_CAN_NOT_OWN_SAME_ITEM;
					break;

				case	eUAIR_TOO_MANY_BADGE_ITEM		:
					iAddResult	=	eFRBI_TOO_MANY_BADGE_ITEM;
					break;

				default									:
					iAddResult	=	eFRBI_UNKONWN;
					break;
			}
			break;
		}

		iBuyCount++;
	}

	if	(iBuyCount	==	0)
	{
		lpClient->sendFailedBuyItem(iAddResult);
		return;
	}

	packet.bf1IsDuelPointItem	=	_lpPacket->wIsDuelPointShop;

	if	(_lpPacket->wIsDuelPointShop)
	{
		lpClient->m_duelRecord.m_iDuelPoint	-=	(packet.dwPrice*iBuyCount);
		packet.iRemainGold	=	lpClient->m_duelRecord.m_iDuelPoint;//	남은 돈

		//	월드에 lpRecord 값 전송
		{
			SERVERStoWORLD_UPDATE_DUEL_RECORD	packet;

			packet.base.set(sizeof(packet),dSERVERStoWORLD_UPDATE_DUEL_RECORD);

			strcpy(packet.strId,lpClient->m_strId);
			memcpy(&packet.record,&lpClient->m_duelRecord,sizeof(cDuelRecord));

			SEND_WOLRD_PACKET(&packet);
		}
	}
	else
	if	(_lpPacket->wIsTokenShop == 0xffff)
	{
		LONGLONG	llSpentGold	=	packet.dwPrice;
		llSpentGold			*=	iBuyCount;

		lpClient->m_iGold	-=	llSpentGold;

// 		_log("buy item log %I64d", -llSpentGold);

		lpClient->updateGoldLog(-llSpentGold,eSPENT_GOLD_BY_BUY_ITEM);
		packet.iRemainGold	=	lpClient->m_iGold;//	남은 돈
	}

	packet.wCount		=	iBuyCount;

	g_userPM.add(lpClient->m_iClientSerial,&packet,packet.base.wSize);

	if	(_lpPacket->wIsTokenShop != 0xffff)
	{
		lpClient->removeItem(_lpPacket->wIsTokenShop , packet.dwPrice * iBuyCount , FALSE,eRI_BY_TOKEN,eSPENT_GOLD_BY_BUY_TOKENSHOP_ITEM);
	}

	if	(packet.dwPrice	>=	1000 || _lpPacket->wIsTokenShop != 0xffff)
	{		// 가격이 1000원 이상이거나 토큰 상점일경우 로그를 남긴다.
		_lpPacket->item.setBitFieldValue(60);
		lpClient->sendBuyItemLog((cItem *)&_lpPacket->item,packet.dwPrice,iBuyCount,_lpPacket->wWhere, _lpPacket->wIsTokenShop);//	로그 전송
	}

}

//
//	월드에서 온 패킷 처리
void
cGAME::operateWorldPacket()
{
	int		iSlot,iPacketType;
	char*	lpPacket;

	beginProfile("월드 패킷 처리");

	mCS(m_csReceiveWorldPacket);	//	패킷 큐를 위한 메니져. -_-a

	for	(iSlot=0;iSlot<m_iReceivedWorldPacketCount;iSlot++)
	{
		lpPacket	=	m_apWorldPacketBuffer[iSlot];
		iPacketType	=	((cMSG_BASE_TYPE_FORWORLD *)lpPacket)->wType;

		mDS_PK.m_iLastUpdateWorldPacket	=	iPacketType;
		
		beginProfileByID(iPacketType);

		switch(iPacketType)
		{
			case	dW2G_GVG_AVATAR_ORG_DATA			:
				operateW2G_GVG_AVATAR_ORG_DATA((W2G_GVG_AVATAR_ORG_DATA *)lpPacket);
				break;

			case	dW2G_ASK_GVG_AVATAR_ORG_DATA_RESULT	:
				operateW2G_ASK_GVG_AVATAR_ORG_DATA_RESULT((W2G_ASK_GVG_AVATAR_ORG_DATA_RESULT *)lpPacket);
				break;

			case	dWORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT	:
				operateTransGuildToGVGResult((WORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT *)lpPacket);
				break;

			case	dWORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT	:
				operateTransAvatarToGVGResult((WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT *)lpPacket);
				break;

			case	dWORLDtoSERVERS_DUEL_MATCH_INFO			:
				operateDuelMatchInfo((WORLDtoSERVERS_DUEL_MATCH_INFO *)lpPacket);
				break;

			case	dWORLDtoSERVERS_DUEL_RECORD			:
				operateDuelRecord((WORLDtoSERVERS_DUEL_RECORD *)lpPacket);
				break;

			case	dWORLDtoSERVERS_SIMPLE_WORK					:
				operateSimpleWork((WORLDtoSERVERS_SIMPLE_WORK *)lpPacket);
				break;

			case	dWORLDtoSERVERS_DUEL_RANKER_INFO			:
				operateDuelRankerInfo((WORLDtoSERVERS_DUEL_RANKER_INFO *)lpPacket);
				break;

			case	dWORLDtoSERVERS_DUEL_TEAM					:
				operateDuelTeamInfo((WORLDtoSERVERS_DUEL_TEAM *)lpPacket);
				break;

			case	dWORLDtoSERVERS_CHANGE_DAY					:
				operateChangeDay();
				break;

			case	dWORLDtoSERVERS_BOOST_POWER_BY_EVENT		:
				operateBoostPowerByEvent((WORLDtoSERVERS_BOOST_POWER_BY_EVENT *)lpPacket);
				break;

			case	dWORLDtoSERVERS_SOUND_OF_LEADERS_BELL		:
				operateSoundOfLeadersBell((WORLDtoSERVERS_SOUND_OF_LEADERS_BELL *)lpPacket);
				break;

			case	dWORLDtoSERVERS_ACCOUNT_INFO				:
				operateAccountInfo((WORLDtoSERVERS_ACCOUNT_INFO *)lpPacket);
				break;
				
			case	dWORLDtoSERVERS_DISCONNECT_SERVER			:
				operateDisconnectServerPacket((WORLDtoSERVERS_DISCONNECT_SERVER *)lpPacket);
				break;

			case	dWORLDtoSERVERS_GR_INFO						:
				operateGRInfo((WORLDtoSERVERS_GR_INFO *)lpPacket);
				break;

			case	dWORLDtoSERVERS_UPDATE_PACK_ITEM_COUNT		:
				operateUpdatePackItemCount((WORLDtoSERVERS_UPDATE_PACK_ITEM_COUNT *)lpPacket);
				break;

			case	dWORLDtoSERVERS_CARROT_SHOP_INFO			:
				operateCarrotShopInfo((WORLDtoSERVERS_CARROT_SHOP_INFO *)lpPacket);
				break;

			case	dWORLDtoSERVERS_CONFIRM_ENTER_FIELD			:
				operateConfirmEnterField((WORLDtoSERVERS_CONFIRM_ENTER_FIELD *)lpPacket);
				break;

			case	dWORLDtoSERVERS_ITEM_PACK_INFO				:
				operateItemPackInfo((WORLDtoSERVERS_ITEM_PACK_INFO *)lpPacket);
				break;
				
			case	dWORLDtoSERVERS_ITEM_PACK_LIST_SERIAL		:
				operateItemPackListSerial((WORLDtoSERVERS_ITEM_PACK_LIST_SERIAL *)lpPacket);
				break;

			case	dWORLDtoSERVERS_RESET_DROP_PACK_ITEM_COUNT	:
				operateResetDropPackItemCount((WORLDtoSERVERS_RESET_DROP_PACK_ITEM_COUNT *)lpPacket);
				break;

			case	dWORLDtoSERVERS_CREATE_SELL_ITEM_RESULT	:
				operateCreateShopItemResult((WORLDtoSERVERS_CREATE_SELL_ITEM_RESULT *)lpPacket);
				break;

			case	dWORLDtoSERVERS_USER_SAY			:
				operateSayMessage((WORLDtoSERVERS_USER_SAY *)lpPacket);
				break;

			case	dWORLDtoSERVERS_IS_DENY_COMMUNITY_TARGET	:
				operateIsDenyCommunityTarget((WORLDtoSERVERS_IS_DENY_COMMUNITY_TARGET *)lpPacket);
				break;

			case	dWORLDtoSERVERS_USER_SAY_RESULT		:
				operateSayResult((WORLDtoSERVERS_USER_SAY_RESULT *)lpPacket);
				break;

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃파티관련 패킷들
			case	dWORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY			:
				operateGuildMemberJoinParty((WORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY *)lpPacket);
				break;

			case	dWORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY_ANSWER			:
				operateGuildMemberJoinPartyAnswer((WORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY_ANSWER *)lpPacket);
				break;
			case	dWORLDtoSERVERS_JOIN_PARTY_APPLICATION			:
				operateJoinPartyApplication((WORLDtoSERVERS_JOIN_PARTY_APPLICATION *)lpPacket);
				break;
			case	dWORLDtoSERVERS_JOIN_PARTY_APPLICATION_ANSWER	:
				operateJoinPartyApplicationAnswer((WORLDtoSERVERS_JOIN_PARTY_APPLICATION_ANSWER *)lpPacket);
				break;

			case	dWORLDtoSERVERS_CHANGE_PARTY_STATUS		:
				operateChangePartyStatus((WORLDtoSERVERS_CHANGE_PARTY_STATUS *)lpPacket);
				break;

			case	dWORLDtoSERVERS_MOVE_FIELD_PARTY_MEMBER		:
				operateMoveFieldPartyMemberPacket((WORLDtoSERVERS_MOVE_FIELD_PARTY_MEMBER *)lpPacket);
				break;
				
			case	dWORLDtoSERVERS_UPDATE_UNION_PARTYINFO		:
				operateUpdateUnionPartyInfoPacket((WORLDtoSERVERS_UPDATE_UNIONPARTYINFO *)lpPacket);
				break;
				
			case	dWORLDtoSERVERS_DISSOLUTION_UNION_PARTY		:
				operateUpdateUnionPartyDissolutionPacket((SG_UNION_PARTY_WORK *)lpPacket);
				break;
			case	dWORLDtoSERVERS_CREATE_UNION_PARTY_RESULT	:
				operateUpdateUnionPartyCreateResultPacket((SG_UNION_PARTY_WORK *)lpPacket);
				break;

			case	dWORLDtoSERVERS_UPDATE_PARTYINFO			:
				operateUpdatePartyInfoPacket((WORLDtoSERVERS_UPDATE_PARTYINFO *)lpPacket);
				break;

			case	dWORLDtoSERVERS_VALID_PARTY					:
				operateValidPartyPacket((WORLDtoSERVERS_VALID_PARTY *)lpPacket);
				break;

			case	dWORLDtoSERVERS_FAILED_TO_CREATE_PARTY		:
				operateFailedToCreatePartyPacket((WORLDtoSERVERS_FAILED_TO_CREATE_PARTY *)lpPacket);
				break;

			case	dWORLDtoSERVERS_DISSOLUTION_PARTY			:
				operateDissolutionPartyPacket((WORLDtoSERVERS_DISSOLUTION_PARTY *)lpPacket);
				break;

			case	dWORLDtoSERVERS_PARTY_LIST		:
				operatePartyListPacket((WORLDtoSERVERS_PARTY_LIST *)lpPacket);
				break;

			case	dWORLDtoSERVERS_JOIN_PARTY		:
				operateJoinPartyPacket((WORLDtoSERVERS_JOIN_PARTY*)lpPacket);
				break;

			case	dWORLDtoSERVERS_PARTY_WORK		:
				operatePartyWorkPacket((WORLDtoSERVERS_PARTY_WORK*)lpPacket);
				break;

			case	dWORLDtoSERVERS_DISCONNECT_PARTY_MEMBER	:
				operateDisconnectPartyMemberPacket((WORLDtoSERVERS_DISCONNECT_PARTY_MEMBER*)lpPacket);
				break;

			case	dWORLDtoSERVERS_RENAME_PARTYNAME	:
				operateChangePartyNamePacket((WORLDtoSERVERS_RENAME_PARTYNAME*) lpPacket);
				break;

			case	dWORLDtoSERVERS_CHANGE_PARTY_MEMBER_INFO	:
				operateChangePartyMemberInfoPacket((WORLDtoSERVERS_CHANGE_PARTY_MEMBER_INFO*)lpPacket);
				break;
			case	dWORLDtoSERVERS_PARTY_CHAT		:
				operatePartyChat((WORLDtoSERVERS_PARTY_CHAT*)lpPacket);
				break;

//┃파티관련 패킷들
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

			case	dWORLDtoSERVERS_CREATE_ITEM_RESULT	:
				operateCreateItemResultPacket((WORLDtoSERVERS_CREATE_ITEM_RESULT*)lpPacket);
				break;
				
			case	dWORLDtoSERVERS_ADD_ITEM_RESULT		:
				operateAddItemResultPacket((WORLDtoSERVERS_ADD_ITEM_RESULT*)lpPacket);
				break;

			case	dWORLDtoSERVERS_USER_FIND			:
				operateUserFindResultPacket((WORLDtoSERVERS_USER_FIND*)lpPacket);
				break;

			case	dWORLDtoSERVERS_USER_RECALL			:
				operateUserRecallPacket((WORLDtoSERVERS_USER_RECALL*)lpPacket);
				break;

			case	dWORLDtoSERVERS_ASK_ITEM_UNIQUE_SERIAL	:
				operateReceiveItemUniqueSerial((WORLDtoSERVERS_ASK_ITEM_UNIQUE_SERIAL*)lpPacket);
				break;

			case	dWORLDtoSERVERS_RESULT_INCREASE_DB_EVENT_VALUE	:
				operateResultIncreaseDBEventValue((WORLDtoSERVERS_RESULT_INCREASE_DB_EVENT_VALUE*)lpPacket);
				break;

			case	dWORLDtoSERVERS_EVENT_MESSAGE		:
				operateEventMessage((WORLDtoSERVERS_EVENT_MESSAGE*)lpPacket);
				break;
				
	//	필드 이동 결과 처리
			case	dWORLDtoSERVERS_USER_MOVEMAP		:
				operateMoveFieldResult((WORLDtoSERVERS_USER_MOVEMAP*)lpPacket);
				break;

//	필드 이동 체크
			case	dWORLDtoSERVERS_CHECK_USER_MOVEMAP	:
				operateCheckUserMoveMapResult((SERVERStoWORLD_USER_MOVEMAP*)lpPacket);
				break;

	//	서버간 이동을 위한 로그아웃 메시지 처리
			case	dWORLDtoSERVERS_USER_LOGOUT			:
				operateWorldLogoutForMoveServerResult((WORLDtoSERVERS_USER_LOGOUT*)lpPacket);
				break;
	////
	////	길드 관련 패킷들
	////
			case	dWORLDtoSERVERS_RECORD_GUILD_HONOR_POINT			:
				operateRecordGuildHonorPoint((WORLDtoSERVERS_RECORD_GUILD_HONOR_POINT*)lpPacket);
				break;
			case	dWORLDtoSERVERS_ADD_RELATED_PLACE_OF_GUILD			:
				operateAddRelatedPlaceOfGuild((WORLDtoSERVERS_ADD_RELATED_PLACE_OF_GUILD*)lpPacket);
				break;
			case	dWORLDtoSERVERS_STORE_GUILD_INVENTORY_ITEM			:
				operateStoreGuildInventoryItem((WORLDtoSERVERS_STORE_GUILD_INVENTORY_ITEM*)lpPacket);
				break;
			case	dWORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_ITEM		:
				opreateWithdrawGuildInventoryItem((WORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_ITEM*)lpPacket);
				break;
			case	dWORLDtoSERVERS_STORE_GUILD_INVENTORY_GOLD			:
				opreateStoreGuildInventoryGold((WORLDtoSERVERS_STORE_GUILD_INVENTORY_GOLD*)lpPacket);
				break;
			case	dWORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_GOLD		:
				opreateWithdrawGuildInventoryGold((WORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_GOLD*)lpPacket);
				break;
			case	dWORLDtoSERVERS_CHANGE_GUILD_INVENTORY_ITEM_PLACE	:
				opreateChangeGuildInventoryItemPlace((WORLDtoSERVERS_CHANGE_GUILD_INVENTORY_ITEM_PLACE*)lpPacket);
				break;

			case	dWORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_GOLD		:
				operateChangeGuildInventoryPermitWithdrawGold((WORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_GOLD*)lpPacket);
				break;
			case	dWORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_ITEM		:
				operateChangeGuildInventoryPermitWithdrawItem((WORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_ITEM*)lpPacket);
				break;
			case	dWORLDtoSERVERS_CHANGE_GI_STORAGE_SIZE				:
				operateChangeGuildInventoryStorageSize((WORLDtoSERVERS_CHANGE_GI_STORAGE_SIZE*)lpPacket);
				break;
				
			case	dWORLDtoSERVERS_SET_GUILD_HALL_LEVEL:
				operateSetGuildHallLevel((WORLDtoSERVERS_SET_GUILD_HALL_LEVEL*)lpPacket);
				break;
			
			case	dWORLDtoSERVERS_SET_GUILD_HALL_ROEN_GOOD_WILL:
				operateSetGuildHallRoenGoodWill((WORLDtoSERVERS_SET_GUILD_HALL_ROEN_GOOD_WILL*)lpPacket);
				break;

			case	dWORLDtoSERVERS_USER_CREATEGUILD	:
				operateCreateGuildResult((WORLDtoSERVERS_USER_CREATEGUILD*)lpPacket);
				break;

			case	dWORLDtoSERVERS_USER_LISTGUILD		:
				operateReceiveGuildList((WORLDtoSERVERS_USER_LISTGUILD*)lpPacket);
				break;

			case	dWORLDtoSERVERS_USER_GUILDINFO		:
				operateReceiveGuildMemberList((WORLDtoSERVERS_USER_GUILDINFO*)lpPacket);
				break;

			case	dWORLDtoSERVERS_LOGIN				:
				operateFinishReceiveGuildInfo((WORLDtoSERVERS_LOGIN*)lpPacket);
				break;

			case	dWORLDtoSERVERS_RENEW_PREMIUM_ITEM	:
				operateReceiveRenewPremiumItemInfo((WORLDtoSERVERS_RENEW_PREMIUM_ITEM*)lpPacket);
				break;

	//	새로운 길드
			case dWORLDtoSERVERS_GUILD_CREATE			:
				operateCreateGuild((WORLDtoSERVERS_GUILD_CREATE*)lpPacket);
				break;

			case dWORLDtoSERVERS_DELETEGUILD			:
				operateRemoveGuild((WORLDtoSERVERS_DELETEGUILD*)lpPacket);
				break;

	//	길드 탈퇴
			case dWORLDtoSERVERS_GUILD_EXIT				:
				operateRemoveGuildMember((WORLDtoSERVERS_GUILD_EXIT*)lpPacket);
				break;

	//	길드탈퇴
			case dWORLDtoSERVERS_USER_EXITGUILD			:
				operateLeaveGuild((WORLDtoSERVERS_USER_EXITGUILD*)lpPacket);
				break;

	//	새로운 길드 멤버
			case dWORLDtoSERVERS_GUILD_JOIN				:
				operateAddGuildMember((WORLDtoSERVERS_GUILD_JOIN*)lpPacket);
				break;

	//	조인 결과
			case dWORLDtoSERVERS_USER_JOINGUILD			:
				operateJoinGuildResult((WORLDtoSERVERS_USER_JOINGUILD*)lpPacket);
				break;

			case dWORLDtoSERVERS_GUILD_UPDATE_EXP		:
				operateUpdateGuildExp((WORLDtoSERVERS_GUILD_UPDATE_EXP*)lpPacket);
				break;

	//	길드 공지 변경
			case dWORLDtoSERVERS_GUILD_NOTICE			:
				operateUpdateGuildNotice((WORLDtoSERVERS_GUILD_NOTICE*)lpPacket);
				break;

	//	길드 경험치 세율 업뎃
			case dWORLDtoSERVERS_GUILD_EXPRATE			:
				operateChangeGuildExpTaxRate((WORLDtoSERVERS_GUILD_EXPRATE*)lpPacket);
				break;

			case dWORLDtoSERVERS_UPDATE_GUILD_HALL_INFO	:
				operateUpdateGuildHallInfo((WORLDtoSERVERS_UPDATE_GUILD_HALL_INFO*)lpPacket);
				break;

	//	길드원 랭크 변경
			case dWORLDtoSERVERS_USER_CHANGERANK		:
				operateChangeRankInGuild((WORLDtoSERVERS_USER_CHANGERANK*)lpPacket);
				break;

			case dWORLDtoSERVERS_USER_CHANGERANK_RESULT	:
				operateChangeRankResult((WORLDtoSERVERS_USER_CHANGERANK_RESULT*)lpPacket);
				break;
				
			case dWORLDtoSERVERS_UPDATE_GUILD_SKILL_LEVEL	:
				operateUpdateGuildSkillLevel((WORLDtoSERVERS_UPDATE_GUILD_SKILL_LEVEL*)lpPacket);
				break;
			case dWORLDtoSERVERS_GUILD_SKILL_LEVEL_INCREASE_FAILURE	:
				operateGuildSkillLevelIncreaseFailure((WORLDtoSERVERS_GUILD_SKILL_LEVEL_INCREASE_FAILURE*)lpPacket);
				break;
				
			case dWORLDtoSERVERS_UPDATE_WORLD_VALUE_INFO_ALL:
				receiveWorldValueInfoAll((WORLDtoSERVERS_WORLD_VALUE_INFO_ALL *)lpPacket);
				break;
			case dWORLDtoSERVERS_UPDATE_WORLD_VALUE_INFO:
				receiveWorldValueInfo((WORLDtoSERVERS_WORLD_VALUE_INFO *)lpPacket);
				break;
			case dWORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT	:
				receiveAppointmentQuizEvent((WORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT *)lpPacket);
				break;

			case dWORLDtoSERVERS_END_QUIZ_EVENT		:
				receiveEndQuizEvent((WORLDtoSERVERS_END_QUIZ_EVENT *)lpPacket);
				break;

			case dWORLDtoSERVERS_CANCEL_QUIZ_EVENT	:
				receiveCancelQuizEvent((WORLDtoSERVERS_CANCEL_QUIZ_EVENT *)lpPacket);
				break;
			case dWORLDtoSERVERS_UPDATE_WORD_COMPARE:
				receiveEventWordInfo((WORLDtoSERVER_EVENT_WORD_INFO*)lpPacket);
				break;
			case dWORLDtoSERVERS_PRODUCE_OX_QUIZ	:
				receiveProduceOxQuiz((WORLDtoSERVERS_PRODUCE_OX_QUIZ *)lpPacket);
				break;
				
			case dWORLDtoSERVERS_LOTTO_EVENT_DATA	:
				receiveLottoEventData((WORLDtoSERVERS_LOTTO_EVENT_DATA *)lpPacket);
				break;

			case dWORLDtoSERVERS_EVENT_TIME_DATA	:
				receiveEventTimeData((SERVERStoWORLD_MODIFY_EVENT_TIME *)lpPacket);
				break;

			case dWORLDtoSERVERS_CHANGE_WORD_QUIZ_ROUND	:
				receiveChangeWordQuizRound((WORLDtoSERVERS_CHANGE_WORD_QUIZ_ROUND *)lpPacket);
				break;

			case dWORLDtoSERVERS_PRODUCE_WORD_QUIZ		:
				receiveProduceWordQuiz((WORLDtoSERVERS_PRODUCE_WORD_QUIZ *)lpPacket);
				break;

			case dWORLDtoSERVERS_OX_QUIZ_LOSER_REVIVE_BATTLE	:
				receiveOXQuizLoserReviveBattle();
				break;

			case dWORLDtoSERVERS_SELECT_GUILD_MARK	:
				receiveSelectGuildMark((WORLDtoSERVERS_SELECT_GUILD_MARK *)lpPacket);
				break;
				
			case dWORLDtoSERVERS_UPDATE_GUILD_MEMBER_LEVEL	:
				receiveUpdateGuildMemberLevel((WORLDtoSERVERS_UPDATE_GUILD_MEMBER_LEVEL*)lpPacket);
				break;

			case dWORLDtoSERVERS_GUILD_BATTLE_SCHEDULE				:
				receiveGuildBattleSchedule((WORLDtoSERVERS_GUILD_BATTLE_SCHEDULE*)lpPacket);
				break;

			case dWORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO			:
				receiveGuildBattleInfo((WORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO*)lpPacket);
				break;

			case dWORLDtoSERVERS_UPDATE_GUILD_INFO					:
				receiveGuildInfo((WORLDtoSERVERS_UPDATE_GUILD_INFO*)lpPacket);
				break;

			case dWORLDtoSERVERS_VALID_GUILD						:
				receiveValidGuild((WORLDtoSERVERS_VALID_GUILD*)lpPacket);
				break;

			case dWORLDtoSERVERS_GUILD_BATTLE_RESULT				:
				receiveGuildBattleResult((WORLDtoSERVERS_GUILD_BATTLE_RESULT*)lpPacket);
				break;
			case dWORLDtoSERVERS_CALLING							:
				receiveCalling((WORLDtoSERVERS_CALLING*)lpPacket);
				break;

			case dWORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE			:
				receiveBeginTestGuildBattle((WORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE*)lpPacket);
				break;
				
			case dWORLDtoSERVERS_RESET_WEEK_FIGHT_GUILD_INFO		:
				g_guildManager.resetWeekFightInfo();
				break;

			case dWORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST		:
				receiveRemoveFriendInTargetList((WORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST*)lpPacket);
				break;

			case dWORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST_RESULT	:
				receiveRemoveFriendInTargetListResult((WORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST_RESULT*)lpPacket);
				break;
		}

		endProfileByID(iPacketType);
	}

	mDS_PK.m_iLastUpdateWorldPacket	=	-1;

	m_iReceivedWorldPacketCount	=	0;

	endProfile("월드 패킷 처리");
}


void
cGAME::addWorldPacket(char *_lpPacketData)
{
	int	iPacketSize	=	((cMSG_BASE_TYPE_FORWORLD*)_lpPacketData)->wSize;

	if (iPacketSize >	(int)m_dwMaxWorldPacketSize)
	{
//		printf("치명정인 오류!!!! in cGAME::addWorldPacket - iPacketSize >= m_dwMaxWorldPacketSize\n");
		CLOG("error","in cGAME::addWorldPacket - iPacketSize >= m_dwMaxWorldPacketSize");

		return;
	}

	if (m_iReceivedWorldPacketCount	>= dWORLD_PACKET_BUFFER_SIZE)
	{
//		printf("치명정인 오류!!!! in cGAME::addWorldPacket - m_iReceivedWorldPacketCount >= dWORLD_PACKET_BUFFER_SIZE\n");
		CLOG("error","in cGAME::addWorldPacket - m_iReceivedWorldPacketCount >= dWORLD_PACKET_BUFFER_SIZE");
		return;
	}

//	_log(" add world packet m_iReceivedWorldPacketCount [%d]",m_iReceivedWorldPacketCount);
	mCS(m_csReceiveWorldPacket);	//	패킷 큐를 위한 메니져. -_-a
	memcpy(m_apWorldPacketBuffer[m_iReceivedWorldPacketCount],_lpPacketData,iPacketSize);

	++m_iReceivedWorldPacketCount;
}

void
cGAME::initWorldPacketReceiver()
{
	FPInitCS(&m_csReceiveWorldPacket);	//	패킷 큐를 위한 메니져. -_-a

	m_iReceivedWorldPacketCount	=	0;
	m_dwMaxWorldPacketSize		=	0;

	
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_USER_SAY_RESULT		));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_PARTY_LIST			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_JOIN_PARTY			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_PARTY_WORK			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_UPDATE_PARTYINFO			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_RENAME_PARTYNAME		));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_CHANGE_PARTY_MEMBER_INFO		));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_CREATE_ITEM_RESULT		));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_ADD_ITEM_RESULT		));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_USER_FIND				));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_USER_RECALL			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_ASK_ITEM_UNIQUE_SERIAL	));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_EVENT_MESSAGE			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_PARTY_CHAT			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_USER_MOVEMAP			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_USER_LOGOUT			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_USER_CREATEGUILD		));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_USER_LISTGUILD			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_USER_GUILDINFO			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_LOGIN					));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_GUILD_CREATE			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_DELETEGUILD			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_GUILD_JOIN				));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_GUILD_EXIT				));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_USER_JOINGUILD			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_GUILD_UPDATE_EXP		));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_GUILD_NOTICE			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_GUILD_EXPRATE			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_USER_CHANGERANK		));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_USER_EXITGUILD			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_USER_CHANGERANK_RESULT	));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_UPDATE_GUILD_SKILL_LEVEL			));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_GUILD_SKILL_LEVEL_INCREASE_FAILURE	));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_GUILD_BATTLE_SCHEDULE	));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_UPDATE_GUILD_INFO));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_GUILD_BATTLE_RESULT));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_SELECT_GUILD_MARK));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_PRODUCE_OX_QUIZ));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_END_QUIZ_EVENT));
	m_dwMaxWorldPacketSize		=	max(m_dwMaxWorldPacketSize,sizeof(WORLDtoSERVERS_OX_QUIZ_LOSER_REVIVE_BATTLE));

	printf("World packet maximum size %dbyte\n",m_dwMaxWorldPacketSize);
	printf("World packet buffer size %dbyte\n",m_dwMaxWorldPacketSize*dWORLD_PACKET_BUFFER_SIZE);

	for (int i=0;i<dWORLD_PACKET_BUFFER_SIZE;i++)
		m_apWorldPacketBuffer[i]	=	new char [m_dwMaxWorldPacketSize];
}

void
cGAME::closeWorldPacketReceiver()
{
	printf("release World packet buffer\n");

	for (int i=0;i<dWORLD_PACKET_BUFFER_SIZE;i++)
		pKILL(m_apWorldPacketBuffer[i]);

	DeleteCriticalSection(&m_csReceiveWorldPacket);	//	패킷 큐를 위한 메니져. -_-a
}
