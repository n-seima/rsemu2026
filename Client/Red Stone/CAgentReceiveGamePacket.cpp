#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CSkill.H"
#include "cMESSAGE.H"
#include "CSound.H"
#include "CEvent.H"
#include "CGuardianPost.H"
#include "CPortal.H"
#include "arca.H"
#include "secretDungeonDefine.H"
#include "cParty.H"
#include "cGuild.H"
#include "hero_mini_Pet.H"
#include "duel.H"
#include "CParty.h"
#include "BattleLog.h"
#include "mess_sign.h"
#ifdef	_USE_HS
#include "HShield.h"
#endif

#ifdef _USE_XTRAP
#include "XTrap/Xtrap_C_Interface.h"
#endif

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃게임 서버
//
//	디스컨넥트 메시지
void
cPACKET_HANDLER::Disconnect(SERVER_PACKETS *_lpPacket)
{
	SG_OUT	*lpPacket	=	&_lpPacket->mSG_OUT;

	s_agent.m_isEventDisconnect	=	TRUE;

	g_eh.addStaticLog("SG_OUT");

	if	(strlen(s_agent.m_strDisconnectText))
		strcpy(s_agent.m_strDisconnectText,lpPacket->strCause);
}

//
//	게임 참가 결과
void
cPACKET_HANDLER::ResultGameConnect(SERVER_PACKETS *_lpPacket)
{
	SetConnectResult(_lpPacket->mSG_RESULT_CONNECT.wResult);

	if	(_lpPacket->mSG_RESULT_CONNECT.wResult	==	dRESULT_CONNECT_FAIL)
		return;

	CGamePlay::s_serverTime.m_wYear		=	_lpPacket->mSG_RESULT_CONNECT.serverTime.year+2000;
	CGamePlay::s_serverTime.m_wMonth	=	_lpPacket->mSG_RESULT_CONNECT.serverTime.month;
	CGamePlay::s_serverTime.m_wDay		=	_lpPacket->mSG_RESULT_CONNECT.serverTime.day;
	CGamePlay::s_serverTime.m_wHour		=	_lpPacket->mSG_RESULT_CONNECT.serverTime.hour;
	CGamePlay::s_serverTime.m_wMinute	=	_lpPacket->mSG_RESULT_CONNECT.serverTime.minute;
	CGamePlay::s_serverTime.m_wSecond	=	_lpPacket->mSG_RESULT_CONNECT.serverTime.second;
	CGamePlay::s_dwReceiveServerTime	=	timeGetTime();

	{
		s_agent.m_iPacketSerial			=	_lpPacket->mSG_RESULT_CONNECT.wPacketSerial;
		s_agent.m_wXorCode				=	_lpPacket->mSG_RESULT_CONNECT.wXorCode;
		s_agent.m_iGameServerEncTable	=	0;
		memcpy(s_agent.m_abEncTable,_lpPacket->mSG_RESULT_CONNECT.abEncryptTable,sizeof(s_agent.m_abEncTable));
	}

	g_iSeasonVariable					=	_lpPacket->mSG_RESULT_CONNECT.dwSeasonVariable;

	s_iFieldSerial						=	_lpPacket->mSG_RESULT_CONNECT.wFieldSerial;

	strcpy(s_strBookingToMoveFieldFileName,_lpPacket->mSG_RESULT_CONNECT.strMapName);

	if	(s_strBookingToMoveFieldFileName[0] == 'G' && s_strBookingToMoveFieldFileName[1] == 'H')
	{
		s_bIsGuildHall					=	TRUE;
		strcpy(s_strGuildHallOwnerGuildName,_lpPacket->mSG_RESULT_CONNECT.strGuildName);
	}
	else
		s_bIsGuildHall					=	FALSE;
}

//브로드 캐스트 서버 정보
void
cPACKET_HANDLER::ReceiveBCSInfo(SERVER_PACKETS *_lpPacket)
{
	SG_BCS_INFO	*lpPacket	=	(SG_BCS_INFO	*)_lpPacket;

#ifdef	_USE_BROADCAST
	s_agent.connectToBroadCastServer(lpPacket->strBCSIp,dBROADCAST_PORT,lpPacket->wIndexForBCS);
#endif
}

void
cPACKET_HANDLER::ReceiveFailedToJoin(SERVER_PACKETS *_lpPacket)
{
	SG_FAILED_TO_JOIN	*lpPacket	=	(SG_FAILED_TO_JOIN	*)_lpPacket;

	SetJoinResult(lpPacket->wResult);
	
	g_duelManager.reset();

	switch(lpPacket->wResult)
	{
		case	dRESULT_JOIN_FIELD_FULL	:
			CGamePlay::FailedToJoinGame(dMSG_JOIN_FAIELD_BY_FIELD_FULL);
			return;
			
		case	dRESULT_JOIN_FIELD_FAILED	:
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME);
			return;
			
		case	dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA	:
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME);
			return;
			
		case	dRESULT_JOIN_FIELD_FAILED_BY_EXIST_PLAYER	:
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME_BY_EXIST_SAME_PLAYER);
			return;
			
		case	dRESULT_JOIN_FIELD_FAILED_BY_NOT_SECRET_DUNGEON_MEMBER		:	//	비밀던젼이 사라지거나 파티에서 쫓겨났다.
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME_BY_NOT_SECRET_DUNGEON_MEMBER);
			return;
			
		case	dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_CONNECT				:	//	데이터에 문제가 있어 실패했다.
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME_BY_INCORRECT_CONNECT);
			return;
			
		case	dRESULT_JOIN_FIELD_FAILED_BY_TOO_MANY_CONNECTOR				:	//	접속 시도가 너무 많다.
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME_BY_TOO_MANY_CONNECTOR);
			return;
			
		case	dRESULT_JOIN_FIELD_FAILED_BY_TRY_CONNECT_TO_INCORRECT_FIELD	:	//	엉뚱한 필드로 접속을 시도했다.
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME_BY_INCORRECT_FIELD);
			return;

		case	dRESULT_JOIN_FIELD_FAILED_BY_PROCESS_OFFLINE_TOURNAMENT		:	//	엉뚱한 필드로 접속을 시도했다.
			CGamePlay::FailedToJoinGame(lpPacket->strText);
			return;
	}
}

//
//	게임 참가 결과
void
cPACKET_HANDLER::ResultJoin(SERVER_PACKETS *_lpPacket)
{	
	SG_RESULT_JOIN	*lpPacket	=	&_lpPacket->mSG_RESULT_JOIN;

	SetJoinResult(lpPacket->wResult);

	g_duelManager.reset();

	switch(lpPacket->wResult)
	{
		case	dRESULT_JOIN_FIELD_FULL	:
			CGamePlay::FailedToJoinGame(dMSG_JOIN_FAIELD_BY_FIELD_FULL);
			return;

		case	dRESULT_JOIN_FIELD_FAILED	:
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME);
			return;

		case	dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA	:
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME);
			return;

		case	dRESULT_JOIN_FIELD_FAILED_BY_EXIST_PLAYER	:
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME_BY_EXIST_SAME_PLAYER);
			return;

		case	dRESULT_JOIN_FIELD_FAILED_BY_NOT_SECRET_DUNGEON_MEMBER		:	//	비밀던젼이 사라지거나 파티에서 쫓겨났다.
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME_BY_NOT_SECRET_DUNGEON_MEMBER);
			return;

		case	dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_CONNECT				:	//	데이터에 문제가 있어 실패했다.
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME_BY_INCORRECT_CONNECT);
			return;

		case	dRESULT_JOIN_FIELD_FAILED_BY_TOO_MANY_CONNECTOR				:	//	접속 시도가 너무 많다.
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME_BY_TOO_MANY_CONNECTOR);
			return;

		case	dRESULT_JOIN_FIELD_FAILED_BY_TRY_CONNECT_TO_INCORRECT_FIELD	:	//	엉뚱한 필드로 접속을 시도했다.
			CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME_BY_INCORRECT_FIELD);
			return;
	}

	//	길드 홀도 아니고 결투 필드도 아니고 비던도 아닌데 맵 번호가 일치하지 않는다.
	if	(lpPacket->m_bf4GuildHallLevel == 0 && lpPacket->bf1IsDuelField == FALSE)
	{
		if	(lpPacket->m_wCurrentField	<	dINSTANCE_FIELD_FLAG)
		{
			if	(lpPacket->m_wCurrentField	!=	atoi(g_map.m_strFileName+1))
			{
				if	(g_map.m_strFileName[0]	!=	'G')
				{
					CGamePlay::FailedToJoinGame(dMSG_FAILED_TO_JOIN_GAME_BY_INCORRECT_FIELD);
					return;
				}
			}
		}
	}
	
	g_map.m_wWeather = lpPacket->m_bf4Weather;		// 기후변화.
	
	if	(lpPacket->bf1IsDuelField)
		g_hero.resetTemporaryMagicEffect();

	if	(lpPacket->base.wSize	==	sizeof(SG_RESULT_JOIN))
		memcpy(g_hero.m_awSkillCoolTime,lpPacket->m_awSkillCoolTime,sizeof(g_hero.m_awSkillCoolTime));
	else
		memset(g_hero.m_awSkillCoolTime,0,sizeof(g_hero.m_awSkillCoolTime));

	memcpy(g_hero.getUpkeepPlayerDataPointer(),&lpPacket->upkeepPlayerData,sizeof(CUpkeepPlayerData));

	g_hero.m_iSerial							=	lpPacket->wSerialInField;	//	플레이어 시리얼
	g_hero.m_wOperatorLevel						=	lpPacket->bf3OperatorLevel;
	g_hero.m_wTeam								=	lpPacket->bf3Team;
	g_hero.m_wDeathPeneltySecond				=	lpPacket->bf12DeathPeneltySecond;
	g_hero.m_wDeathPenelty						=	lpPacket->bf7DeathPenelty;

	g_hero.m_wCurrentField						=	lpPacket->m_wCurrentField;	//	현재 필드
	g_hero.m_iXPos								=	lpPacket->m_iXPos;
	g_hero.m_iYPos								=	lpPacket->m_iYPos;
	g_hero.m_wLastVillage						=	lpPacket->m_wLastVillage;	//	마지막으로 떠나온 마을
	g_hero.m_wRemainSecondForUseGateGlove		=	lpPacket->m_wRemainSecondForUseGateGlove;
	g_hero.m_bf1IsReadyToSelectGuildMark		=	lpPacket->bf1IsReadyToSelectGuildMark;

	g_iBoostExpSecondByTreasureMapEventInServer	=	lpPacket->m_wBoostExpSecondByTreasureMapEventInServer;
	g_iBoostGoldSecondByTreasureMapEventInServer=	lpPacket->m_wBoostGoldSecondByTreasureMapEventInServer;
	
	g_hero.m_wRedStonePoint						=	lpPacket->m_wRedStonePoint;
	g_hero.m_wBoostExpSecondByTreasureMapEvent	=	lpPacket->m_wPlayerBoostExpSecondByTreasureMapEvent;
	g_hero.m_wBoostGoldSecondByTreasureMapEvent	=	lpPacket->m_wPlayerBoostGoldSecondByTreasureMapEvent;
	g_hero.m_wServerSign						=	lpPacket->m_wServerSign;
	g_hero.m_wReversalPowerTime					=	lpPacket->m_wReversalPowerTime;
#ifdef _FOR_JAPAN				//JBC	08-08-12
	g_hero.m_bf1IsNetCafeUser					=	lpPacket->bf1IsNetCafeUser;
#endif

	g_iHeavenRedStoneCount		=	lpPacket->m_iHeavenRedStoneCount;
	g_iHellRedStoneCount		=	lpPacket->m_iHellRedStoneCount;
	g_iRedDevilRedStoneCount	=	lpPacket->m_iRedDevilRedStoneCount;

	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
	{
		g_bIsWaitGetGVGAvatarOrgData=	lpPacket->bf1IsWaitOrgAvatarDataResult;

		if	(g_bIsWaitGetGVGAvatarOrgData)
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_SYNC_GVG_AVATAR_DATA);
	}
	else
		g_bIsWaitGetGVGAvatarOrgData=	FALSE;

	if	(lpPacket->m_wCurrentField	>=	dINSTANCE_FIELD_FLAG)
		CGamePlay::s_bIsSecretDungeon=	TRUE;
	else
		CGamePlay::s_bIsSecretDungeon=	FALSE;

	s_bIsBossZone				=	lpPacket->bf1IsBossZone;
	s_bIsGuildBattleField		=	lpPacket->bf1IsGuildBattleField;
	s_bIsSiegeWarfareField		=	lpPacket->bf1IsSiegeWarfareField;
	s_bIsVillage				=	lpPacket->bf1IsVillage;
	s_bIsGuildPointBattleField	=	lpPacket->bf1IsGuildPointBattleField;
	s_iMinimapDisplayLevel		=	lpPacket->bf4MinimapDisplayLevel;
	s_iRemainGuildStategyPoint	=	lpPacket->bf4RemainGuildStategyPointCount;

	if	(s_bIsGuildPointBattleField	==	FALSE)
		g_guild.m_iGuildHonorPoint	=	0;

	CGamePlay::s_bIsProgressEvent=	lpPacket->bf1IsProgressEvent;

	if	(s_bIsOXQuizField		!=	(BOOL)lpPacket->bf1IsOXQuizField)
	{
		s_bIsOXQuizField		=	lpPacket->bf1IsOXQuizField;

		g_eventOxQuiz.m_bIsWaitBegin	=	TRUE;
		g_eventOxQuiz.m_bIsEndEvent		=	FALSE;
	}

	if	(s_bIsWordQuizField		!=	g_map.m_bf1IsWordQuizField)
	{
		s_bIsWordQuizField		=	g_map.m_bf1IsWordQuizField;

		g_eventWordQuiz.m_bIsWaitBegin	=	TRUE;
		g_eventWordQuiz.m_bIsEndEvent	=	FALSE;

		CGamePlay::ResetChattingBoxFrame(TRUE);
	}

	{
		BOOL	bIsHideInterfaceOld	=	s_bIsHideInterface;

		s_bIsObserverMode			=	FALSE;
		g_bIsObserverModeForDuel	=	FALSE;

		if	(s_bIsOXQuizField	||	s_bIsWordQuizField)
		{
			s_bIsEventField		=	TRUE;
			s_bIsHideInterface	=	TRUE;
			s_bIsBlockUseSkill	=	TRUE;
		}
		else
		{
			s_bIsEventField		=	FALSE;
			s_bIsHideInterface	=	FALSE;
			s_bIsBlockUseSkill	=	FALSE;
		}

		if	(s_bIsGuildBattleField	&&	g_hero.m_wOperatorLevel	>=	4)
		{
			s_bIsHideInterface	=	TRUE;
			s_bIsBlockUseSkill	=	TRUE;
			s_bIsObserverMode	=	TRUE;

			CGamePlay::ResetChattingBoxFrame(TRUE);
		}

		if	(s_bIsHideInterface	!=	bIsHideInterfaceOld)
			CGamePlay::ResetChattingBoxFrame(TRUE);
	}

	if	(s_bIsGuildBattleField)
		CGamePlay::s_iPlayerMovePacketPeriod	=	dSYNC_FPS/2;
	else	
	if	(s_bIsVillage)
		CGamePlay::s_iPlayerMovePacketPeriod	=	dSYNC_FPS*2;
	else
		CGamePlay::s_iPlayerMovePacketPeriod	=	dSYNC_FPS;

	memcpy(g_guild.m_awGuildVillage,lpPacket->m_awRelatedPlaceOfGuild,sizeof(g_guild.m_awGuildVillage));
	g_am.setHero(&g_hero);

	CGamePlay::s_lpInventoryWindowEquipmentList	=	g_hero.m_aEquip;
	CGamePlay::s_lpInventoryWindowItemList		=	g_hero.m_aItems;
	CGamePlay::s_lpGold							=	&g_hero.m_iGold;

//	g_lpHero는 g_am.setHero 에서 설정된다.
	g_lpHero->m_dwEnchantedMask		=	lpPacket->dwEnchantedMask;
	g_lpHero->m_wMagicCarpetType	=	lpPacket->upkeepPlayerData.m_bf5MagicCarpetShape;

	if	(g_bIsGuildBattleField && !g_map.m_bf1IsGuildHall)		
		g_hero.resetTemporaryMagicEffect();

	g_hero.m_iRecoverHP				=	0;
	
	g_hero.init();
}
void
cPACKET_HANDLER::ReceivePlayerInfo(SERVER_PACKETS *_lpPacket)
{
	SG_PLAYER_INFO	*lpPacket		=	(SG_PLAYER_INFO *)_lpPacket;

	memcpy(&g_hero,&lpPacket->playerData,sizeof(CPlayerSaveData));

	g_hero.m_bf6ExtraInventorySize		=	lpPacket->m_wExtraInventorySize;
	g_hero.m_wBonusStatePoint			=	lpPacket->m_wBonusStatePoint;
	g_hero.m_wActiveMiniPet				=	lpPacket->m_bf5ActiveMiniPet;
	g_hero.m_wActiveMiniPet2			=	lpPacket->m_bf5ActiveMiniPet2;

	g_hero.m_wReversalFrameYear			=	lpPacket->m_reversalFrameYear;
	g_hero.m_wReversalFrameMonth		=	lpPacket->m_reversalFrameMonth;
	g_hero.m_wReversalFrameDay			=	lpPacket->m_reversalFrameDay;
	g_hero.m_wReversalFrameHour			=	lpPacket->m_reversalFrameHour;
	g_hero.m_wReversalFrameMinute		=	lpPacket->m_reversalFrameMinute;

	g_hero.m_bf4CH5StateBonusCount			=	lpPacket->m_bf4CH5StateBonusCount;
	g_hero.m_bf4CH5SkillBonusCount			=	lpPacket->m_bf4CH5SkillBonusCount;
	g_hero.m_bf4CH5HPBonusCount				=	lpPacket->m_bf4CH5HPBonusCount;
	g_hero.m_bf4CH5CPBonusCount				=	lpPacket->m_bf4CH5CPBonusCount;

	g_hero.m_bf4CH5PeneltyHPBonusCount		=	lpPacket->m_bf4CH5PeneltyHPBonusCount;
	g_hero.m_bf4CH5PeneltyCPBonusCount		=	lpPacket->m_bf4CH5PeneltyCPBonusCount;
	g_hero.m_bf3GreateGodGrace				=	lpPacket->m_bf3GreateGodGrace;


	g_hero.m_bf4EatFireMiniPetCount		=	lpPacket->m_bf4EatFireMiniPetCount;
	g_hero.m_bf4EatWaterMiniPetCount	=	lpPacket->m_bf4EatWaterMiniPetCount;
	g_hero.m_bf4EatWindMiniPetCount		=	lpPacket->m_bf4EatWindMiniPetCount;
	g_hero.m_bf4EatEarthMiniPetCount	=	lpPacket->m_bf4EatEarthMiniPetCount;
	g_hero.m_bf4EatLightMiniPetCount	=	lpPacket->m_bf4EatLightMiniPetCount;
	g_hero.m_bf4EatDarkMiniPetCount		=	lpPacket->m_bf4EatDarkMiniPetCount;
	g_hero.m_bf4RebirthCount			=	lpPacket->m_bf4RebirthCount;	//	전생 횟수
	g_hero.m_bf4UseSealSphereOfDawnCount=	lpPacket->m_bf4UseSealSphereOfDawnCount;	//	새벽의 봉인구 사용 횟수

	memcpy(g_hero.m_awItemCoolTime,lpPacket->m_awItemCoolTime,sizeof(lpPacket->m_awItemCoolTime));
	memcpy(g_hero.m_awQuestCountDownTimer,lpPacket->m_awQuestCountDownTimer,sizeof(lpPacket->m_awQuestCountDownTimer));
	memcpy(g_hero.m_aCompleteQuest,lpPacket->m_aCompleteQuest,sizeof(lpPacket->m_aCompleteQuest));
	memcpy(g_hero.m_aCompleteQuest2,lpPacket->m_aCompleteQuest2,sizeof(lpPacket->m_aCompleteQuest2));
	memcpy(g_hero.m_aCompleteArbeit,lpPacket->m_aCompleteArbeit,sizeof(lpPacket->m_aCompleteArbeit));
	memcpy(g_hero.m_aCompleteArbeit2,lpPacket->m_aCompleteArbeit2,sizeof(lpPacket->m_aCompleteArbeit2));
	memcpy(g_hero.m_aExtraInventory,lpPacket->m_aExtraInventory,sizeof(lpPacket->m_aExtraInventory));
	memcpy(g_hero.m_aMiniPet,lpPacket->m_aMiniPet,sizeof(lpPacket->m_aMiniPet));

	g_hero.m_wProcessChapterIndex		=	lpPacket->m_wProcessChapterIndex;
	g_hero.m_wProcessHighQuestIndex		=	lpPacket->m_wProcessHighQuestIndex;

	g_hero.setIsRequireFullInfo(FALSE);

	g_am.setHero(&g_hero);

	g_hero.syncHeroApprearance();
	g_hero.init();
}

void
cPACKET_HANDLER::ReceiveSuccessToJoinGame(SERVER_PACKETS *_lpPacket)
{
}

//비밀 던젼 입장
void
cPACKET_HANDLER::ReceiveEnterToTherSecretDungeon(SERVER_PACKETS *_lpPacket)
{
	SG_ENTER_TO_THE_SECRET_DUNGEON	*lpPacket	=	(SG_ENTER_TO_THE_SECRET_DUNGEON	*)_lpPacket;

	g_map.m_area.m_wInstanceFieldGate	=	lpPacket->wGateArea;

	CGamePlay::s_partyInfo.m_wGateField	=	lpPacket->wGateField;
	CGamePlay::s_partyInfo.m_bf10GateArea=	lpPacket->wGateArea;
}

//
//	누군가 게임에 참가 했다.
void
cPACKET_HANDLER::ReceiveJoinPlayerMessage(SERVER_PACKETS *_lpPacket)
{
	g_am.joinActor(&_lpPacket->mSG_JOIN_PLAYER.actorInfo);
}

//
//	플레이어가 나갔다.
void
cPACKET_HANDLER::ReceivePlayerOutMessage(SERVER_PACKETS *_lpPacket)
{
	CActor	*lpActor	=	g_am.getTestedActor(_lpPacket->mSG_PLAYER_OUT.wSerial);

	if (!lpActor)
		return;

	g_am.removeActor(_lpPacket->mSG_PLAYER_OUT.wSerial);
}

//
//	기존에 액터들
void
cPACKET_HANDLER::ReceiveSimpleActorInfoList(SERVER_PACKETS *_lpPacket)
{
	g_am.receiveSimpleActorInfoList((SG_SIMPLE_ACTOR_INFO_LIST*)_lpPacket);
}

void
cPACKET_HANDLER::ReceiveSimpleActorInfo(SERVER_PACKETS *_lpPacket)
{
	g_am.receiveSimpleActorInfo((SG_SIMPLE_ACTOR_INFO*)_lpPacket);
}

void
cPACKET_HANDLER::ReceiveVerySimpleActorInfoList(SERVER_PACKETS *_lpPacket)
{
	g_am.receiveVerySimpleActorInfoList((SG_VERY_SIMPLE_ACTOR_INFO_LIST	*)_lpPacket);
}

//
//	기존에 액터들
void
cPACKET_HANDLER::ReceiveMinimumActorInfoList(SERVER_PACKETS *_lpPacket)
{
	g_am.receiveMinimumActorInfoList(&_lpPacket->mSG_MINIMUM_ACTOR_INFO_LIST);
}

//	운영자 리스트 받았다.
void
cPACKET_HANDLER::ReceiveOperatorList(SERVER_PACKETS *_lpPacket)
{
	g_am.receiveOperatorList(&_lpPacket->mSG_OPERATOR_LIST);
}

//
//	기존에 액터들
void
cPACKET_HANDLER::ReceiveRefitActorsPostion(SERVER_PACKETS *_lpPacket)
{
	g_am.operateReceivedRefitActorPostionPacket(&_lpPacket->mSG_REFIT_ACTORS_POSITION);
}

//
//주기적으로 외곽에 액터들 좌표를 업데이트 한다... 라지만, 이름이 좀.. -_-a
void
cPACKET_HANDLER::ReceiveOuterActorsPosUpdate(SERVER_PACKETS *_lpPacket)
{
	g_am.operateReceivedOuterActorPostionPacket(&_lpPacket->mSG_OUTER_ACTORS_POS_UPDATE);
}

//
//이벤트 NPC정보 받기
void
cPACKET_HANDLER::ReceiveEventNpcList(SERVER_PACKETS *_lpPacket)
{
	SG_EVENT_NPC_LIST	*lpPacket	=	&_lpPacket->mSG_EVENT_NPC_LIST;

	for (int i=0;i<lpPacket->wCount;i++)
	{
		CActor	*lpActor	=	&g_aActor[lpPacket->aInfoList[i].bf11Serial];

		if	(lpActor->m_wSerial	==	0xffff)
			continue;

		lpActor->m_wIsFixedActor	=	TRUE;
		lpActor->setPos(lpPacket->aInfoList[i].wX,lpPacket->aInfoList[i].wY);

		lpActor->m_wActorKind		=	lpPacket->aInfoList[i].bf7Type;
	}
}

//
//	이동
void
cPACKET_HANDLER::Move(SERVER_PACKETS *_lpPacket)
{
	CActor	*lpActor	=	g_am.getTestedActor(_lpPacket->mSG_MOVE.wSerial);

	if (!g_am.checkDeathStatus(lpActor))
		return;

	lpActor->operateMovePacket(&_lpPacket->mSG_MOVE);
}

//
//	이동
void
cPACKET_HANDLER::MoveResult(SERVER_PACKETS *_lpPacket)
{
	s_agent.receiveCheckPacketSpeedPacket();
	g_lpHero->operateMoveResult(&_lpPacket->mSG_MOVE_RESULT);
}

//
//	이동
void
cPACKET_HANDLER::ReceiveMoveFailedMessage(SERVER_PACKETS *_lpPacket)
{
	SG_MOVE_FAILED	*lpPacket	=	&_lpPacket->mSG_MOVE_FAILED;

	if	(lpPacket->bf5Result	!=	eMOVE_RESULT_SUCCESS)
		lpPacket->bf5Result	=	lpPacket->bf5Result;

	switch(lpPacket->bf5Result)
	{
		case	eMOVE_RESULT_SUCCESS				:
			break;
		case	eMOVE_RESULT_FAILED_BY_ACTION		:	//	행동 중인디유.. -o-
			break;
		case	eMOVE_RESULT_FAILED_BY_BLOCKED_POS	:	//	막힌 지역이다.
			break;
		case	eMOVE_RESULT_FAILED_BY_INCORRECT_POS:
			break;
		case	eMOVE_RESULT_FAILED_BY_ICY_STALAGMITE:
			g_lpHero->castIcyStalagmite(NULL);
			break;
		case	eMOVE_RESULT_FAILED_BY_STATUS		:
			CGamePlay::WarningMessage(dMSG_CAN_NOT_MOVE_BY_BAD_STATUS);
			break;
	}

	if (lpPacket->bf5Result != eMOVE_RESULT_MOVE_SLAVE)
	{
		g_lpHero->stop(lpPacket->wX,lpPacket->wY);

		g_hero.m_iWaitMoveResultTime	=	0;

		g_lpHero->m_iPPS				=	lpPacket->bf10PPS;
		g_hero.m_iPPS					=	lpPacket->bf10PPS;
		g_lpHero->m_isRunning			=	lpPacket->bf1IsRun;
		g_hero.m_bf1IsRunning			=	lpPacket->bf1IsRun;
	}
}

//
//이동 능력 변경(걷기/뛰기-o-)
void
cPACKET_HANDLER::SetMoveAbility(SERVER_PACKETS *_lpPacket)
{
	SG_SET_MOVE_ABILITY *lpPacket	=	&_lpPacket->mSG_SET_MOVE_ABILITY;

	g_lpHero->m_iPPS		=	lpPacket->wPPS;
	g_hero.m_iPPS			=	lpPacket->wPPS;
	g_lpHero->m_isRunning	=	lpPacket->isRun;
	g_hero.m_bf1IsRunning	=	g_lpHero->m_isRunning;

	if	(g_lpHero->m_isMove)
		g_lpHero->moveDestPos();
}

//
//변쉰
void
cPACKET_HANDLER::Transformation(SERVER_PACKETS *_lpPacket)
{
	SG_TRANSFORMATION	*lpPacket	=	&_lpPacket->mSG_TRANSFORMATION;
	CActor				*lpActor	=	g_am.getTestedActor(_lpPacket->mSG_TRANSFORMATION.wSerial);

	if	(!g_am.checkDeathStatus(lpActor))
		return;

	if(lpActor == NULL)
		return;

	if	(lpActor->isHero())
		if	(lpPacket->bf4Reason	==	eTR_LOW_CP)
			g_lpHero->warning(dMSG_RELEASE_TRANSFORMATION_BY_LOW_CP);

	lpActor->readyTransformation(lpPacket);
}

//
//변쉰
void
cPACKET_HANDLER::TransformationFailed(SERVER_PACKETS *_lpPacket)
{
	SG_TRANSFORMATION_FAILED	*lpPacket	=	&_lpPacket->mSG_TRANSFORMATION_FAILED;

	g_hero.m_iCP	=	lpPacket->sCP*100;

	switch(lpPacket->wReason)
	{
		case	eTRANSFORMATION_FAILED_BY_LOW_CP_LEVEL				:
			g_lpHero->warning(dMSG_CAN_NOT_TRANSFORMATION_BY_LOW_CP);
			break;

		case	eTRANSFORMATION_FAILED_BY_YET_NOT_COMPLETE_ACTION	:
			g_lpHero->warning(dMSG_CAN_NOT_TRANSFORMATION_BY_YET_NOT_COMPLETE_ACTION);
			break;
		case	eTRANSFORMATION_FAILED_BY_SECOND_JOB_SKILL_NOT_FOUND:	//	-_-;;
			g_lpHero->warning(dMSG_CAN_NOT_TRANSFORMATION_BY_SECOND_JOB_SKILL_NOT_FOUND);
			break;
		case	eTRANSFORMATION_FAILED_BY_NOT_AVAIL_JOB				:	//	-_-;;
			g_lpHero->warning(dMSG_CAN_NOT_TRANSFORMATION_BY_NOT_AVAIL_JOB);
			break;
		case	eTRANSFORMATION_FAILED_BY_ETC						:	//	-_-;;
			g_lpHero->warning(dMSG_CAN_NOT_TRANSFORMATION_BY_ETC);
			break;
	}
}

//
//앉기 토글
void
cPACKET_HANDLER::ToggleSitdown(SERVER_PACKETS *_lpPacket)
{
	SG_TOGGLE_SITDOWN	*lpPacket	=	&_lpPacket->mSG_TOGGLE_SITDOWN;
	CActor				*lpActor	=	g_am.getTestedActor(_lpPacket->mSG_TOGGLE_SITDOWN.bf15Serial);

	if (!g_am.checkDeathStatus(lpActor))
		return;

	lpActor->sitDown(lpPacket->wPosX,lpPacket->wPosY,lpPacket->bf1IsSitDown);
}

//
//앉기 결과
void
cPACKET_HANDLER::SitdownResult(SERVER_PACKETS *_lpPacket)
{
	SG_SITDOWN_RESULT	*lpPacket	=	&_lpPacket->mSG_SITDOWN_RESULT;

	switch(lpPacket->wResult)
	{
		case	eSITDOWN_RESULT_OK					:	//	성공
		{
			LONGLONG	llHP=	g_lpHero->m_iMaxHP;
			llHP			*=	min(lpPacket->wHP,255);
			llHP			/=	255;

			g_lpHero->m_iHP	=	(int)llHP;
			g_hero.m_iCP	=	lpPacket->sCP*100;		//	HP/CP
			g_hero.m_iHP	=	g_lpHero->m_iHP;

			g_lpHero->sitDown(g_lpHero->m_pos.x,g_lpHero->m_pos.y,lpPacket->wValue);
			break;
		}

		case	eSITDOWN_RESULT_FAILED_BY_BATTLE	:	//	전투중이라 실패
		{
			CGamePlay::AddSystemMessage(WHITE,dMSG_CAN_NOT_SITDOWN_IN_BATTLE);
			g_lpHero->sitDown(g_lpHero->m_pos.x,g_lpHero->m_pos.y,0);
			break;
		}
	}
}

//죽음의 시간
void
cPACKET_HANDLER::ReceiveDeathPeneltySecond(SERVER_PACKETS *_lpPacket)
{
	SG_DEATH_PENELTY_SECOND	*lpPacket	=	(SG_DEATH_PENELTY_SECOND	*)_lpPacket;

	g_hero.m_wDeathPeneltySecond	=	lpPacket->bf14PeneltySecond;
	g_hero.m_wDeathPenelty			=	lpPacket->wDeathPenelty;

	if	(lpPacket->bf14PeneltySecond==	0)
		g_hero.m_wDeathPenelty		=	0;

	g_hero.buildPower();

	if	(lpPacket->bf1IsDeath && g_hero.m_wDeathPeneltySecond)
		CGamePlay::AddSystemMessage(WHITE,dMSG_DEATH_PENELTY_FORM,g_hero.m_wDeathPeneltySecond/60,g_hero.m_wDeathPeneltySecond%60,g_hero.m_wDeathPenelty);

	if	(lpPacket->bf1IsRecover)
		CGamePlay::AddSystemMessage(LTGREEN,dMSG_RECOVER_SOME_DETH_PENELTY);
}

void
cPACKET_HANDLER::ReceiveQuestionForRecoverDeathPenelty(SERVER_PACKETS *_lpPacket)
{
	SG_QUESTION_FOR_RECOVER_DEATH_PENELTY	*lpPacket	=	(SG_QUESTION_FOR_RECOVER_DEATH_PENELTY	*)_lpPacket;

	if (g_hero.m_wDeathPeneltySecond	==	0)
		CGamePlay::AddSystemMessage(LTGREEN,dMSG_YOU_ARE_NOT_EXIST_DEATH_PENELTY);
	else
	{
		char	strText[256];
		char	strPrice[40],strOwnGold[40];

		strcpy(strPrice,CGamePlay::GetMoneyString(lpPacket->dwPrice));
		strcpy(strOwnGold,CGamePlay::GetMoneyString(g_hero.m_iGold));

		if (g_hero.m_iGold	<	(int)lpPacket->dwPrice)
		{
			sprintf(strText,dMSG_LACK_GOLD_FOR_RECOVER_DEATH_PENELTY_FORM,lpPacket->wRecoverTime/60,lpPacket->wRecoverTime%60,strPrice,strOwnGold);
			g_msgBox.cPopup("",strText,dMSG_OK);
		}
		else
		{
			sprintf(strText,dMSG_QUESTION_FOR_RECOVER_DEATH_PENELTY_FORM,lpPacket->wRecoverTime/60,lpPacket->wRecoverTime%60,strPrice,strOwnGold);

			g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_RECOVER_DEATH_PENELTY);
		}
	}
}

void
cPACKET_HANDLER::ReceiveAddPortal(SERVER_PACKETS *_lpPacket)
{
	SG_ADD_PORTAL	*lpPacket	=	(SG_ADD_PORTAL	*)_lpPacket;

	g_portal.add(lpPacket->wPortalSerial,lpPacket->wField,lpPacket->bf12Owner,lpPacket->wX,lpPacket->wY,lpPacket->bf1IsTownPortal);
}

void
cPACKET_HANDLER::ReceiveRemovePortal(SERVER_PACKETS *_lpPacket)
{
	SG_REMOVE_PORTAL	*lpPacket=	(SG_REMOVE_PORTAL *)_lpPacket;

	g_portal.remove(lpPacket->wPortalSerial);
}
	
void
cPACKET_HANDLER::ReceivePortalList(SERVER_PACKETS *_lpPacket)
{
	SG_PORTAL_LIST	*lpPacket	=	(SG_PORTAL_LIST *)_lpPacket;

	g_portal.reset();

	for (int i=0;i<lpPacket->wPortalCount;i++)
	{
		CSimplePortalInfo	*lpPortal	=	&lpPacket->aPortalList[i];

		g_portal.add(lpPortal->m_bf7Serial,lpPortal->m_bf12Village,lpPortal->m_bf10Owner,lpPortal->m_wX,lpPortal->m_wY,lpPortal->m_bf1IsTownPortal);
	}
}

void
cPACKET_HANDLER::ReceiveChangeDoorStatus(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_DOOR_STATUS	*lpPacket	=	(SG_CHANGE_DOOR_STATUS	*)_lpPacket;

	g_hero.m_bf1IsWaitDoorWorkResult	=	FALSE;

	g_map.changeDoorStatus(lpPacket->bf10Area,lpPacket->bf1IsSleep,lpPacket->bf1IsHide,lpPacket->bf1IsLocked,lpPacket->bf1IsTrap,lpPacket->bf1IsClose,lpPacket->bf1IsActiveByKarma);
}

void
cPACKET_HANDLER::ReceiveDoorMessage(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperateDoorWork((SG_DOOR_MESSAGE *)_lpPacket);
}

void
cPACKET_HANDLER::ReceiveDoorsStatusInField(SERVER_PACKETS *_lpPacket)
{
	SG_DOORS_STATUS_IN_FIELD	*lpPacket	=	(SG_DOORS_STATUS_IN_FIELD	*)_lpPacket;

	for (int i=0;i<lpPacket->wDoorCount;i++)
	{
		cFieldDoorStatus	*lpStatus	=	&lpPacket->aDoors[i];

		g_map.changeDoorStatus(lpStatus->bf10Area,lpStatus->bf1IsSleep,lpStatus->bf1IsHide,lpStatus->bf1IsLocked,lpStatus->bf1IsTrap,lpStatus->bf1IsClose,lpStatus->bf1IsActiveByKarma);
	}
}

void
cPACKET_HANDLER::ReceiveRedistribute(SERVER_PACKETS *_lpPacket)
{
	SG_REDISTRIBUTE	*lpPacket		=	(SG_REDISTRIBUTE	*)_lpPacket;

	switch(lpPacket->wWork)
	{
		case	eREDISTRIBUTE_SKILL	:
			g_hero.resetSkillPoint();
			break;

		case	eREDISTRIBUTE_STATE	:
			g_hero.resetStatePoint();
			break;
		case	eREDISTRIBUTE_SKILL_BY_CH5	:
			g_hero.resetSkillPoint();
			g_hero.m_bf4CH5PeneltyHPBonusCount		=	g_hero.m_bf4CH5HPBonusCount;
			g_hero.m_bf4CH5PeneltyCPBonusCount		=	g_hero.m_bf4CH5CPBonusCount;
			break;

		case	eREDISTRIBUTE_STATE_BY_CH5	:
			g_hero.resetStatePoint();
			g_hero.m_bf4CH5PeneltyHPBonusCount		=	g_hero.m_bf4CH5HPBonusCount;
			g_hero.m_bf4CH5PeneltyCPBonusCount		=	g_hero.m_bf4CH5CPBonusCount;
			break;

		case	eREDISTRIBUTE_FOR_DUEL_SERVER	:
			break;
	}
}

void
cPACKET_HANDLER::ReceiveStopBattle(SERVER_PACKETS *_lpPacket)
{
	SG_STOP_BATTLE	*lpPacket	=	(SG_STOP_BATTLE	*)_lpPacket;

	g_hero.stopBattle();

	if	(lpPacket->bf1IsReleaseFocus)
		g_hero.lockOff();
}

//
//	이동 실패
void
cPACKET_HANDLER::CorrectPlayerPosition(SERVER_PACKETS *_lpPacket)
{
	CActor	*lpActor	=	g_am.getTestedActor(_lpPacket->mSG_CORRECT_PLAYER_POSITION.bf15Serial);

	if	(!lpActor)
		return;

	lpActor->setPos(_lpPacket->mSG_CORRECT_PLAYER_POSITION.wPosX,_lpPacket->mSG_CORRECT_PLAYER_POSITION.wPosY);

	if	(lpActor->isHero())
		g_hero.m_iLastMovedTime	=	0;

	if	(_lpPacket->mSG_CORRECT_PLAYER_POSITION.bf1IsInnerTeleport)
		lpActor->addHitEffect(0,0,g_im.m_wInnerTeleport);

}

//
//	정지
void
cPACKET_HANDLER::Stop(SERVER_PACKETS *_lpPacket)
{
	CActor	*lpActor	=	g_am.getTestedActor(_lpPacket->mSG_STOP.bf12Serial);

	if (!g_am.checkDeathStatus(lpActor))
		return;

	lpActor->operateStopPacket(&_lpPacket->mSG_STOP);
}


void
cPACKET_HANDLER::ReceiveHideEquipment(SERVER_PACKETS *_lpPacket)
{
	SG_HIDE_EQUIPMENT	*lpPacket	=	&_lpPacket->mSG_HIDE_EQUIPMENT;

	if (lpPacket->wPart == dEQUIP_WEAPON	)	g_hero.m_bf1IsHideWeapon1 = 1-lpPacket->wMethod;
	if (lpPacket->wPart == dEQUIP_WEAPON_2	)	g_hero.m_bf1IsHideWeapon2 = 1-lpPacket->wMethod;

	if (lpPacket->wMethod					)	CGamePlay::AddSystemMessage(LTGREEN,dMSG_RETURN_THROW_WEAPON);

	g_hero.syncHeroApprearance();
}

//
//	몬스터 생성
void
cPACKET_HANDLER::RegenMonster(SERVER_PACKETS *_lpPacket)
{
	SG_REGEN_MONSTER	*lpPacket	=	(SG_REGEN_MONSTER	*)_lpPacket;

	g_am.joinMonster(&lpPacket->monsterInfo);
}

//
//	죽... 었어.. -_;;
void
cPACKET_HANDLER::Kill(SERVER_PACKETS *_lpPacket)
{
	g_am.killActor(_lpPacket->mSG_KILL.wSerial,_lpPacket->mSG_KILL.bf1IsInstanceKill,_lpPacket->mSG_KILL.bf12DeathPeneltySecond,_lpPacket->mSG_KILL.bf1IsReleaseSummon);
}

//
//경험치 업데이트
void
cPACKET_HANDLER::UpdateExp(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_EXP	*lpPacket	=	&_lpPacket->mSG_UPDATE_EXP;
	int	iGetExp					=	lpPacket->iExp-g_hero.m_iExperience;
	int	iGetSkillExp			=	lpPacket->wSkillExp-g_hero.m_dwSkillExperience;

	//이전 경험치를 세이브 해논다.
	g_hero.m_iSaveExperience	=	g_hero.m_iExperience;
	g_hero.m_iExperience		=	lpPacket->iExp;
	g_hero.m_dwSkillExperience	=	lpPacket->wSkillExp;

	if	(lpPacket->bf1IsRestore)
		CGamePlay::AddSystemMessage(WHITE,dMSG_RESTORE_EXP_FORM,iGetExp);
	else
	{
		if	(iGetExp	>	0)
		{
			CGamePlay::AddSystemMessage(WHITE,dMSG_GET_EXP_FORM,iGetExp);
#ifdef _DEBUG
			g_BattleLog.WriteExpLog(iGetExp);
#endif
			
		}
//		else
//			CGamePlay::AddSystemMessage(WHITE,dMSG_LOSE_EXP_FORM,iGetExp);
	}

	if	(iGetSkillExp	>	0)
	{
		g_lpHero->attatchSkillLevelUpEffect();
		CGamePlay::AddSystemMessage(LTGREEN,dMSG_GET_SKILL_EXP_FORM,iGetSkillExp);
	}

	if	(g_hero.m_iLevel	!=	lpPacket->bf10Level)
	{
		if	(lpPacket->bf10Level	>	g_hero.m_iLevel)
		{
			int	iIndex	=	g_hero.m_wActiveMiniPet;

			if	(g_hero.m_wActiveMiniPet2	!=	c_iMiniPetCount)
				if	(random(2)	==	0)
					iIndex	=	g_hero.m_wActiveMiniPet2;

			g_hero.iMiniPet()->playFX(eMPFX_PLAYER_LEVEL_UP,iIndex);
		}

//		SG_LEVEL_UP 패킷에서 처리한다.
//		g_hero.setLevel(lpPacket->bf10Level);
	}
}

//
//경험치 업데이트
void
cPACKET_HANDLER::UpdatePetExp(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_PET_EXP	*lpPacket	=	(SG_UPDATE_PET_EXP *)_lpPacket;

	g_hero.m_aPet[lpPacket->bf4PetIndex].m_bf10Level	=	lpPacket->bf10Level;
	g_hero.m_aPet[lpPacket->bf4PetIndex].m_dwExperience	=	lpPacket->iExp;
}

//
//체력 업데이트
void
cPACKET_HANDLER::UpdateBasicInformation(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_BASIC_INFORMATION	*lpPacket	=	&_lpPacket->mSG_UPDATE_BASIC_INFORMATION;
	CActor	*lpActor	=	g_am.getTestedActor(lpPacket->wSerial);

	if	(lpActor == NULL)
		return;

	LONGLONG	llHP=	lpActor->m_iMaxHP;

	llHP			*=	min(lpPacket->wHP,60000);
	llHP			/=	60000;

	lpActor->m_iHP	=	(int)llHP;

	if	(lpActor->isHero())
	{
		if	(g_hero.m_iRecoverHP==	0)
			g_hero.m_iHP	=	(int)llHP;

		g_hero.m_iCP	=	lpPacket->sCP*100;
	}
}

//
//액터 제거
void
cPACKET_HANDLER::RemoveActor(SERVER_PACKETS *_lpPacket)
{
	SG_REMOVE_ACTOR	*lpPacket	=	&_lpPacket->mSG_REMOVE_ACTOR;

	g_am.removeActor(lpPacket->bf15Actor,lpPacket->bf1IsHide);
}

//
//몬스터 정보
void
cPACKET_HANDLER::ReceiveVerySimpleActorInfo(SERVER_PACKETS *_lpPacket)
{
	g_am.receiveVerySimpleActorInfo((SG_VERY_SIMPLE_ACTOR_INFO *)_lpPacket);
}

//
//	레벨 업
void
cPACKET_HANDLER::LevelUp(SERVER_PACKETS *_lpPacket)
{
	SG_LEVEL_UP		*lpPacket		=	&_lpPacket->mSG_LEVEL_UP;
	int				iGetSkillExp	=	lpPacket->dwSkillExp-g_hero.m_dwSkillExperience;

	g_hero.m_iExperience			=	lpPacket->iExp;
	g_hero.m_dwSkillExperience		=	lpPacket->dwSkillExp;

	g_hero.m_sStrength				=	lpPacket->sStrength		;//	힘		
	g_hero.m_sAgility				=	lpPacket->sAgility		;//	민첩성	
	g_hero.m_sConstitution			=	lpPacket->sConstitution	;//	건강	
	g_hero.m_sIntelligence			=	lpPacket->sIntelligence	;//	지혜	
	g_hero.m_sWisdom				=	lpPacket->sWisdom		;//	지식	
	g_hero.m_sCharisma				=	lpPacket->sCharisma		;//	카리스마
	g_hero.m_sLuck					=	lpPacket->sLuck			;//	운		
	g_hero.m_strength.m_sValue		=	lpPacket->sStrength		;
	g_hero.m_agility.m_sValue		=	lpPacket->sAgility		;
	g_hero.m_constitution.m_sValue	=	lpPacket->sConstitution	;
	g_hero.m_intelligence.m_sValue	=	lpPacket->sIntelligence	;
	g_hero.m_wisdom.m_sValue		=	lpPacket->sWisdom		;
	g_hero.m_charisma.m_sValue		=	lpPacket->sCharisma		;
	g_hero.m_luck.m_sValue			=	lpPacket->sLuck			;
	g_hero.m_wLevelPoint			=	lpPacket->wLevelPoint	;

	if	(iGetSkillExp > 0)
		CGamePlay::AddSystemMessage(LTGREEN,dMSG_GET_SKILL_EXP_FORM,iGetSkillExp);

	//JBC	경험치가 있고 1레벨업 했을때는 경험치 획득 메세지가 출력 된다.	08-09-10
	if	((lpPacket->iExp > 0 ) && (g_hero.m_iLevel + 1 == lpPacket->wLevel ))
	{
		CGamePlay::AddSystemMessage(WHITE,dMSG_GET_EXP_FORM,lpPacket->iExp + g_aExpTable[g_hero.m_iLevel] - g_hero.m_iSaveExperience);
#ifdef _DEBUG
		g_BattleLog.WriteExpLog(lpPacket->iExp + g_aExpTable[g_hero.m_iLevel] - g_hero.m_iSaveExperience);
#endif
	}

	if	(g_hero.m_iLevel			!=	lpPacket->wLevel)
	{
		g_hero.setLevel(lpPacket->wLevel);
		g_hero.buildPower();
		g_hero.m_iHP					=	g_hero.getMaxHP();
		g_hero.increaseCP(g_hero.getMaxCP());
		g_lpHero->m_iMaxHP				=	g_hero.getMaxHP();
		g_lpHero->m_iHP					=	g_lpHero->m_iMaxHP;
	}
}

//
//	레벨 업
void
cPACKET_HANDLER::LevelUpOther(SERVER_PACKETS *_lpPacket)
{
	SG_LEVEL_UP_OTHER	*lpPacket	=	&_lpPacket->mSG_LEVEL_UP_OTHER;

	CActor	*lpActor	=	g_am.getTestedActor(lpPacket->wWho);

	if (!lpActor)
		return;

	lpActor->m_wLevel				=	lpPacket->wLevel;
	lpActor->attatchLevelUpEffect();
	lpActor->m_iHP					=	lpActor->m_iMaxHP;
}

//
//	레벨 업
void
cPACKET_HANDLER::ChangeStatePower(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_STATE_RESULT	*lpPacket=	&_lpPacket->mSG_CHANGE_STATE_RESULT;

	switch(lpPacket->wType)
	{
		case	eCHARACTER_BASIC_STATE_STRENGTH		:	
			g_hero.m_sStrength				=	lpPacket->sValue;//	힘
			break;
		case	eCHARACTER_BASIC_STATE_CONSTITUTION	:
			g_hero.m_sConstitution			=	lpPacket->sValue;//	건강
			break;
		case	eCHARACTER_BASIC_STATE_AGILITY		:
			g_hero.m_sAgility				=	lpPacket->sValue;//	민첩성
			break;
		case	eCHARACTER_BASIC_STATE_INTELLIGENCE	:
			g_hero.m_sIntelligence			=	lpPacket->sValue;//	지식
			break;
		case	eCHARACTER_BASIC_STATE_WISDOM		:
			g_hero.m_sWisdom				=	lpPacket->sValue;//	지혜
			break;
		case	eCHARACTER_BASIC_STATE_CHARISMA		:
			g_hero.m_sCharisma				=	lpPacket->sValue;//	카리스마
			break;
		case	eCHARACTER_BASIC_STATE_LUCK			:
			g_hero.m_sLuck					=	lpPacket->sValue;//	운
			break;
			
		case	eCHARACTER_BASIC_STATE_FAILED		:
			break;

	}

	g_hero.m_wLevelPoint			=	lpPacket->wLevelPoint;

	g_hero.buildPower();
}

//
//	어빌러티 레벨 증가
void
cPACKET_HANDLER::IncreaseAbilityLevel(SERVER_PACKETS *_lpPacket)
{
	SG_INCREASE_ABILITY_LEVEL	*lpPacket=	&_lpPacket->mSG_INCREASE_ABILITY_LEVEL;

	g_hero.increaseSkillLevel(lpPacket->wAbility,lpPacket->wLevel,lpPacket->dwSkillExperience);
}

//
//	
void
cPACKET_HANDLER::ReceiveFindUserResult(SERVER_PACKETS *_lpPacket)
{
	SG_FIND_USER_RESULT			*lpPacket	=	&_lpPacket->mSG_FIND_USER_RESULT;

	switch(lpPacket->wResult)
	{
		case	eFUR_NOT_FIND	:
		{
			g_hero.disconnectFriend(lpPacket->strTargetName);
			CGamePlay::AddSystemMessage(WHITE,dMSG_CAN_NOT_FIND_USER_FORM,lpPacket->strTargetName);
			break;
		}

		case	eFUR_FIND	:
		{
			g_hero.connectFriend(lpPacket->strTargetName,lpPacket->wJob);

			char	*lpstrFieldName	=	cMAP::GetFieldName(lpPacket->wField);

			if	(lpstrFieldName)
				CGamePlay::AddSystemMessage(WHITE,dMSG_FIND_USER_RESULT_FORM,lpPacket->strTargetName,lpstrFieldName);
			break;
		}

		case	eFUR_TARGET_USER_NOT_OWNED_COUPLE_RING	:
		{
			CGamePlay::AddSystemMessage(WHITE,dMSG_FAILED_TO_USE_COUPLE_RING_BY_TARGET_IS_NOT_OWNED_COUPLE_RING_FROM,lpPacket->strTargetName);
			break;
		}
	}
}

void
cPACKET_HANDLER::ReceiveExitGameSuccess(SERVER_PACKETS *_lpPacket)
{
//	SG_EXIT_GAME_SUCCESS	*lpPacket	=	&_lpPacket->mSG_EXIT_GAME_SUCCESS;

//	CGamePlay::ExitGame(lpPacket->wValue);
}

//
//	등록된 서버 메시지
void
cPACKET_HANDLER::ReceiveRegistMessage(SERVER_PACKETS *_lpPacket)
{
	char	*lpMessage;

	SG_REGIST_MESSAGE		*lpPacket	=	&_lpPacket->mSG_REGIST_MESSAGE;

	g_portal.m_iWaitEnterPortalTime		=	0;

	if	(lpPacket->wMessage	<	eRSM_OUTPUT_MESSAGE_BORDER)
	{
		if	(lpPacket->wMessage	>=	eRSM_COMMON_MESSAGE_BORDER)
		{
			switch(lpPacket->wMessage)
			{
				case	eRSM_DUEL_EXCEPTION_TIME_RULE	:
					lpMessage	=	dMSG_RSM_DUEL_EXCEPTION_TIME_RULE;
					break;

				case	eRSM_MISSILE_BLOCKED_LINE			:
					lpMessage	=	dMSG_BLOCKED_OBJECT_EXIST;
					break;

				case	eRSM_EXPIRED_REVERSAL_FRAME_TIME	:
					lpMessage	=	dMSG_EXPIRED_REVERSAL_FRAME_TIME;
					g_hero.m_wActiveMiniPet2	=	c_iMiniPetCount;
					g_hero.buildPower();
					break;

				case	eRSM_CAN_NOT_SUMMON_MINI_PET_BY_FULL_COUNT	:
					lpMessage	=	dMSG_CAN_NOT_SUMMON_MORE_MINIPET;
					break;
				case	eRSM_TEMP_LIMIT_GOLD_IN_GUILD_INVENTORY		:
					lpMessage	=	dMSG_RSM_TEMP_LIMIT_GOLD_IN_GUILD_INVENTORY;
					break;
					
				case	eRSM_VALID_GUILD_SKILL_LEVEL			:
					lpMessage	=	dMSG_RSM_VALID_GUILD_SKILL_LEVEL;
					break;

				case	eRSM_PERMIT_ONLY_GUILD_MASTER_WORK	:
					lpMessage	=	dMSG_PERMIT_ONLY_GUILD_MASTER_WORK;
					break;

				case	eRSM_CAN_NOT_STORE_ITEM_IN_GUILD_INVENTORY	:
					lpMessage	=	dMSG_RSM_CAN_NOT_STORE_ITEM_IN_GUILD_INVENTORY;
					break;
				case	eRSM_CAN_NOT_WITHDRAW_GUILD_ITEM_BY_CHECK_PERMIT_COUNT	:
					lpMessage	=	dMSG_RSM_CAN_NOT_WITHDRAW_GUILD_ITEM_BY_CHECK_PERMIT_COUNT;
					break;
				case	eRSM_CAN_NOT_WITHDRAW_GUILD_GOLD_BY_CHECK_PERMIT_GOLD	:
					lpMessage	=	dMSG_RSM_CAN_NOT_WITHDRAW_GUILD_GOLD_BY_CHECK_PERMIT_GOLD;
					break;
					
				case	eRSM_TARGET_GUILD_HALL_IS_CLOSE_TO_STRANGER	:
					lpMessage	=	dMSG_RSM_TARGET_GUILD_HALL_IS_CLOSE_TO_STRANGER;
					break;

				case	eRSM_FAILED_TO_ENTER_GUILD_BATTLE_FIELD_BY_NOT_BATTLE_TIME	:
					lpMessage	=	dMSG_RSM_FAILED_TO_ENTER_GUILD_BATTLE_FIELD_BY_NOT_BATTLE_TIME;
					break;

				case	eRSM_ALREADY_EXIST_SAME_VILLAGE_IN_GUILD_RELATED_PLACE_LIST	:
					lpMessage	=	dMSG_ALREADY_EXIST_SAME_VILLAGE_IN_GUILD_RELATED_PLACE_LIST;
					break;

				case	eRSM_FULL_VILLAGE_IN_GUILD_RELATED_PLACE_COUNT				:	//	더 이상 연고지를 추가 할 수 없다.
					lpMessage	=	dMSG_FULL_VILLAGE_IN_GUILD_RELATED_PLACE_COUNT;
					break;

				case	eRSM_NOT_EXIST_VILLAGE_IN_GUILD_RELATED_PLACE_LIST			:	//	제거 하려는 마을이 연고지에 없다.
					lpMessage	=	dMSG_NOT_EXIST_VILLAGE_IN_GUILD_RELATED_PLACE_LIST;
					break;

				case	eRSM_MINIMUM_COUNT_RULE_OF_GUILD_RELATED_PLACE_LIST			:	//	최소 하나 이상의 길드 연고지가 있어야 한다.
					lpMessage	=	dMSG_MINIMUM_COUNT_RULE_OF_GUILD_RELATED_PLACE_LIST;
					break;

				case	eRSM_TARGET_IS_AT_BLOCKED_FIELD_TO_USE_THAT_ITEM	:
					lpMessage	=	dMSG_TARGET_IS_AT_BLOCKED_FIELD_TO_USE_THAT_ITEM;
					break;
				case	eRSM_CAN_NOT_USE_THAT_ITEM_IN_THIS_FIELD	:
					lpMessage	=	dMSG_CAN_NOT_USE_THAT_ITEM_IN_THIS_FIELD;
					break;
				case	eRSM_EXIST_TRAFFIC_OBJECT_TO_OPEN_PORTAL	:
					lpMessage	=	dMSG_EXIST_TRAFFIC_OBJECT_TO_OPEN_PORTAL;
					break;

				case	eRSM_REQUEST_TIME_IS_MAINTENANCE_TIME	:
					lpMessage	=	dMSG_REQUEST_TIME_IS_MAINTENANCE_TIME;
					break;
					
				case	eRSM_LACK_MONEY_FOR_JUDGE_ITEM			:
					lpMessage	=	dMSG_LACK_MONEY_FOR_JUDGE_ITEM;
					break;
				case	eRSM_LACK_MONEY_FOR_MERGE_ITEM		:
					lpMessage	=	dMSG_LACK_MONEY_FOR_MERGE_ITEM;
					break;
				case	eRSM_OWN_GOLD_LIMIT					:
					lpMessage	=	dMSG_OWN_GOLD_LIMIT;
					break;
				case	eRSM_OWN_BANK_GOLD_LIMIT			:
					lpMessage	=	dMSG_OWN_BANK_GOLD_LIMIT;
					break;

				case	eRSM_ALREADY_STEAL_GOLD_MONSTER	:
					lpMessage	=	dMSG_ALREADY_STEAL_GOLD_MONSTER;
					break;

				case	eRSM_ALREADY_STEAL_ITEM_MONSTER	:
					lpMessage	=	dMSG_ALREADY_STEAL_ITEM_MONSTER;
					break;

				case	eRSM_CAN_NOT_ADD_SECRET_DUNGEON_KEY_BY_FULL_INVENTORY	:
//					lpMessage	=	dMSG_CAN_NOT_ADD_SECRET_DUNGEON_KEY_BY_FULL_INVENTORY;
					break;
				case	eRSM_THAT_IS_CLOSE_SECRET_DUNGEON						:
					lpMessage	=	dMSG_THAT_IS_CLOSE_SECRET_DUNGEON;
					break;
				case	eRSM_TOO_MANY_PLAYER_ENTER_SECRET_DUNGEON				:
					lpMessage	=	dMSG_TOO_MANY_PLAYER_ENTER_SECRET_DUNGEON;
					break;
				case	eRSM_OVER_THE_SECRET_DUNGEON_PLAY_COUNT_PER_DAY			:
					lpMessage	=	dMSG_OVER_THE_SECRET_DUNGEON_PLAY_COUNT_PER_DAY;
					break;

				case	eRSM_CAN_NOT_ENTER_TO_PORTAL_BY_IS_OWNED_DIFFERENT_PARTY:
					lpMessage	=	dMSG_CAN_NOT_ENTER_TO_PORTAL_BY_IS_OWNED_DIFFERENT_PARTY;
					break;

				case	eRSM_TO_USE_COUPLERING_SET_REQUIRE_TWO_VALID_INVENTORY_SLOT:
					lpMessage	=	dMSG_TO_USE_COUPLERING_SET_REQUIRE_TWO_VALID_INVENTORY_SLOT;
					break;

				case	eRSM_CAN_NOT_TELEPORT_FIELD								:
					lpMessage	=	dMSG_RSM_CAN_NOT_TELEPORT_FIELD;
					g_hero.m_bf1IsWaitTeleportResult	=	FALSE;
					break;

				case	eRSM_FAILED_COMMAND_WORK								:
					lpMessage	=	dMSG_FAILED_COMMAND_WORK;
					break;

				case	eRSM_COUNT_OVER_TO_TRY_GUILD_POINT_BATTLE				:
					lpMessage	=	dMSG_COUNT_OVER_TO_TRY_GUILD_POINT_BATTLE;
					break;

				case	eRSM_TO_TRY_GUILD_POINT_BATTLE_GATHER_PARTY_MEMBER_IN_SAME_FIELD	:
					lpMessage	=	dMSG_TO_TRY_GUILD_POINT_BATTLE_GATHER_PARTY_MEMBER_IN_SAME_FIELD;
					break;

				case	eRSM_TO_MANY_GUILD_TO_TRY_GUILD_POINT_BATTLE	:
					lpMessage	=	dMSG_TO_MANY_GUILD_TO_TRY_GUILD_POINT_BATTLE;
					break;

				case	eRSM_FAILED_TO_TRY_GUILD_POINT_BATTLE			:
					lpMessage	=	dMSG_FAILED_TO_TRY_GUILD_POINT_BATTLE;
					break;

				case	eRSM_CAN_NOT_WARP_OWN_GUILD_HALL_BY_VALID		:
					lpMessage	=	dMSG_RSM_CAN_NOT_WARP_OWN_GUILD_HALL_BY_VALID;
					break;

				case	eRSM_FAILED_BY_INSTANCE_GUILD_BATTLE_TIME_RULE	:
					lpMessage	=	dMSG_RSM_FAILED_BY_INSTANCE_GUILD_BATTLE_TIME_RULE;
					break;

				case	eRSM_ITS_USE_TO_ENEMY_SKILL	:
					lpMessage	=	dMSG_ITS_TO_USE_ENEMY_SKILL;
					break;

				case	eRSM_TOO_FAR_TO_USE_SKILL	:
					lpMessage	=	dMSG_TOO_FAR_DISTANCE_TARGET	;
					break;

				case	eRSM_CAN_NOT_FIND_ATTACK_ABLE_TARGET		:
					lpMessage	=	dMSG_CAN_NOT_FIND_ATTACK_ABLE_TARGET	;
					break;

				case	eRSM_IS_NOT_ABLE_TO_TRY_GUILD_POINT_BATTLE_TIME	:
					lpMessage	=	dMSG_IS_NOT_ABLE_TO_TRY_GUILD_POINT_BATTLE_TIME;
					break;

				case	eRSM_CAN_NOT_ACTIVATE_MINIPET_BY_IS_EXPIRED_MINIPET	:
					lpMessage	=	dMSG_CAN_NOT_ACTIVATE_MINIPET_BY_IS_EXPIRED_MINIPET	;
					break;

				case	eRSM_PLEASE_SELECT_MINIPET_FOR_CHANGE_NAME	:
					lpMessage	=	dMSG_PLEASE_SELECT_MINIPET_FOR_CHANGE_NAME	;
					break;
				case	eRSM_REQUIRE_MINIPET_FOR_USE_THAT_ITEM	:
					lpMessage	=	dMSG_REQUIRE_MINIPET_FOR_USE_THAT_ITEM	;
					break;
					
				case	eRSM_MINI_PET_CANT_USE_AWAKEN_ITEM_LOWER	:
					lpMessage	=	dMSG_MINI_PET_CANT_USE_AWAKEN_ITEM_LOWER	;
					break;

				case	eRSM_DUEL_CAN_NOT_TRY_BY_BAD_PARTY_STATUS			:
					lpMessage	=	dMSG_RSM_DUEL_CAN_NOT_TRY_BY_BAD_PARTY_STATUS;
					break;
				case	eRSM_DUEL_CAN_NOT_TRY_BY_TO_MANY_BATTLER	:
					lpMessage	=	dMSG_RSM_DUEL_CAN_NOT_TRY_BY_TO_MANY_BATTLER;
					break;
				case	eRSM_DUEL_1VS1_RULE		:
					lpMessage	=	dMSG_RSM_DUEL_1VS1_RULE;
					break;
				case	eRSM_DUEL_FIELD_IS_FULL	:
					lpMessage	=	dMSG_RSM_DUEL_FIELD_IS_FULL;
					break;

				case	eRSM_DUEL_CAN_NOT_TRY_BY_YOU_ARE_NOT_PARTY_LEADER		:
					lpMessage	=	dMSG_RSM_DUEL_CAN_NOT_TRY_BY_YOU_ARE_NOT_PARTY_LEADER;
					break;
				case	eRSM_DUEL_CAN_NOT_TRY_BY_MISMATCH_PARTY_MEMBER_COUNT	:
					lpMessage	=	dMSG_RSM_DUEL_CAN_NOT_TRY_BY_MISMATCH_PARTY_MEMBER_COUNT;
					break;
					
				case	eRSM_GUILD_DUNGEN_ERROR_GATHER_PARTY_MEMBER_IN_SAME_FIELD	:
					lpMessage	=	dMSG_TO_TRY_GUILD_DUNGEN_GATHER_PARTY_MEMBER_IN_SAME_FIELD;
					break;
				case	eRSM_COUNT_OVER_TO_TRY_GUILD_DUNGEN	:
					lpMessage	=	dMSG_COUNT_OVER_TO_TRY_GUILD_DUNGEN;
					break;
				case	eRSM_TO_MANY_GUILD_TO_TRY_GUILD_DUNGEN	:
					lpMessage	=	dMSG_TO_MANY_GUILD_TO_TRY_GUILD_DUNGEN;
					break;
				case	eRSM_FAILED_TO_TRY_GUILD_DUNGEN	:
					lpMessage	=	dMSG_FAILED_TO_TRY_GUILD_DUNGEN	;
					break;

				case	eRSM_EXIST_COUNT_OVER_MEMBER_TO_TRY_SECRET_DUNGEN			:
					lpMessage	=	dMSG_COUNT_OVER_TO_TRY_SECRET_DUNGEON	;
					break;
				case	eRSM_SECRET_DUNGEN_ERROR_GATHER_PARTY_MEMBER_IN_SAME_FIELD	:
					lpMessage	=	dMSG_TO_TRY_SECRET_DUNGEON_GATHER_PARTY_MEMBER_IN_SAME_FIELD	;
					break;

				case	eRSM_TO_MANY_PARTY_TO_TRY_SECRET_DUNGEN	:
					lpMessage	=	dMSG_TOO_MAY_PARTY_TRY_TGUS_SECRET_DUNGEON	;
					break;

				case	eRSM_TOO_MANY_BANK_TRANSACTION_WITH_REBIRTH			:
					lpMessage	=	dMSG_TOO_MANY_BANK_TRANSACTION_WITH_REBIRTH	;
					break;
				case	eRSM_TOO_MANY_GOLD_BAR			:
					lpMessage	=	dMSG_CAN_NOT_POSSESS_NO_MORE_THE_GOLD	;
					break;
				default	:
					lpMessage	=	g_pstrRegistServerMessage[lpPacket->wMessage];
					break;
			}
		}
		else
		switch(lpPacket->wMessage)
		{
			case	eRSM_NOTICE_FOR_RELEASE_PENELTY_TIME	:
				return;

			default	:
				lpMessage	=	g_pstrRegistServerMessage[lpPacket->wMessage];
				break;
		}

		switch(lpPacket->wType)
		{
			case	eSM_SYSTEM		:
				CGamePlay::AddSystemMessage(BTPURPLE,lpMessage);
				break;
			case	eSM_CHATTING	:
				CGamePlay::AddSystemMessage(BTPURPLE,lpMessage);
				break;
			case	eSM_NOTICE		:
				CGamePlay::Notice(lpMessage);
				break;

			case	eSM_WARNING		:
				CGamePlay::WarningMessage(lpMessage);
				break;

			case	eSM_MSG_BOX		:
				g_msgBox.cPopup("",lpMessage,dMSG_OK);
				break;
		}

		return;
	}

	switch(lpPacket->wMessage)
	{
		case	eRSM_TOUCH_EVENT_AREA_SUCCESS	:
			g_hero.m_bf1IsReadyToEventAreaWork	=	TRUE;
			break;
		case	eRSM_TOUCH_EVENT_AREA_FAILED	:
			g_hero.m_bf1IsReadyToEventAreaWork	=	TRUE;
			break;
	}
}

//
//	CP충전
void
cPACKET_HANDLER::ReceiveChargeCP(SERVER_PACKETS *_lpPacket)
{
	SG_CHARGE_CP			*lpPacket	=	&_lpPacket->mSG_CHARGE_CP;

	g_hero.m_iCP	=	lpPacket->sCP*100;
	g_hero.increaseCP(0);
	g_lpHero->stop();

	g_esm.playActionSound(&g_aSkill[lpPacket->wSkill]);
}

//
//	메시지를 받았다.
void
cPACKET_HANDLER::ReceiveMessage(SERVER_PACKETS *_lpPacket)
{
	SG_MESSAGE		*lpPacket	=	&_lpPacket->mSG_MESSAGE;

	switch(lpPacket->wType)
	{
		case	eSM_SYSTEM		:
			CGamePlay::AddSystemMessage(BTPURPLE,lpPacket->strMessage);
			break;
		case	eSM_CHATTING	:
			CGamePlay::AddSystemMessage(BTPURPLE,lpPacket->strMessage);
			break;
		case	eSM_NOTICE		:
			CGamePlay::Notice(lpPacket->strMessage);
			break;
	}
}

//
//	남은 체력
void
cPACKET_HANDLER::ReceiveRemainHP(SERVER_PACKETS *_lpPacket)
{
	SG_REMAIN_HP	*lpPacket	=	&_lpPacket->mSG_REMAIN_HP;
	CActor			*lpTarget	=	g_am.getTestedActor(lpPacket->wActor);

	if	(!lpTarget)	
		return;
	LONGLONG		llMaxHp		= lpTarget->m_iMaxHP;
	
	lpTarget->m_iMaxHP			=	lpPacket->iMaxHP;
	if(lpPacket->bf1IsGuildObject)	// 길드오브젝트일경우...
	{
		llMaxHp				= c_iGuildObjectHP; 
		lpTarget->m_iMaxHP	= 	c_iGuildObjectHP;
	}	

	lpTarget->m_iHP				=	(int)(llMaxHp * lpPacket->wRemainHPRate/60000);

	if	(lpTarget->isHero())
		g_hero.m_iHP	=	lpTarget->m_iHP;

	//JBC							09-02-16
// 	if (lpPacket->wIsSyncUnionParty)
// 	{
// 		int	iIsUnionPartyMember	=	s_unionPartyInfo.isUnionPartyMember(lpPacket->wActor);
// 
// 		if(iIsUnionPartyMember != 0xffffffff)
// 		{
// 			int iPlace = iIsUnionPartyMember / 10;
// 			int jPlace = iIsUnionPartyMember % 10;
// 
// 			s_unionPartyInfo.m_wUnionPartyInfo[iPlace][jPlace].m_wHP = lpPacket->wRemainHPRate;
// 		}
// 	}
}

//┃게임 서버
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//
//	몬스터 컨트롤
void
cPACKET_HANDLER::ReceiveControlMonster(SERVER_PACKETS *_lpPacket)
{
	SG_CONTROL_MONSTER	*lpPacket	=	&_lpPacket->mSG_CONTROL_MONSTER;

	if	(lpPacket->wLord	==	0xffff)
	{
		g_hero.m_wSlaveMonster		=	0xffff;
		g_lpControlActor			=	NULL;

		return;
	}

	CActor	*lpTarget	=	g_am.getTestedActor(lpPacket->wSlave);

	if (lpTarget)
	{
		if (lpPacket->wLord	==	g_hero.m_iSerial)
		{
			g_hero.m_wSlaveMonster		=	lpPacket->wSlave;
			g_lpControlActor			=	lpTarget;

			g_lpHero->setPos(g_lpControlActor->m_pos.x,g_lpControlActor->m_pos.y);
		}
		lpTarget->addHitEffect(0,0,g_im.m_wSoulOutEffect);
	}
}

void
cPACKET_HANDLER::ReceiveOccurEffect(SERVER_PACKETS *_lpPacket)
{
	SG_OCCUR_EFFECT	*lpPacket	=	(SG_OCCUR_EFFECT*)_lpPacket;

	switch(g_im.m_abImageOutputMethod[lpPacket->wEffect])
	{
		case	eEOM_RAPE	:
			g_rapeEffect.add(lpPacket->wX,lpPacket->wY,lpPacket->wEffect,0,FALSE,0,0,FALSE,0xffff,lpPacket->wScale);
			break;
		case	eEOM_STAND	:
			g_standEffect.add(lpPacket->wX,lpPacket->wY,lpPacket->wEffect,0,FALSE,0,0,FALSE,0xffff,lpPacket->wScale);
			break;
		case	eEOM_FLOAT	:
			g_floatEffect.add(lpPacket->wX,lpPacket->wY,lpPacket->wEffect,0,FALSE,0,0,FALSE,0xffff,lpPacket->wScale);
			break;
	}
}

//
//	필드 이동
void
cPACKET_HANDLER::MoveFieldResult(SERVER_PACKETS *_lpPacket)
{		// 마지막 수정일 : 09.10.19
	SG_MOVE_FIELD_RESULT	*lpPacket	=	&_lpPacket->mSG_MOVE_FIELD_RESULT;

	switch(lpPacket->wResult)
	{
		case dMOVEMAP_RESULT_NOTFIND		:	//	존재하지 않거나.. 현재 접속 불가능 지역입니다.
			g_msgBox.cPopup("",dMSG_MOVE_FIELD_WRONG_AREA,dMSG_OK);
			g_hero.m_wWaitMoveAreaResultTime	=	0;
			return;
		case dMOVEMAP_RESULT_FULL			:	//	더이상 입장이 불가능합니다.
			g_msgBox.cPopup("",dMSG_FILED_FULL,dMSG_OK);
			g_hero.m_wWaitMoveAreaResultTime	=	0;
			return;
		case dMOVEMAP_RESULT_USEDNAME		:	//	같은 이름의 아이디가 있다 -_-;
			g_msgBox.cPopup("",dMSG_ALREADY_EXIST_SAME_PLAYER,dMSG_OK);
			g_hero.m_wWaitMoveAreaResultTime	=	0;
			return;
		case dMOVEMAP_RESULT_NOTGUILD		:	//	특정길드만이 입장 가능합니다.
			g_msgBox.cPopup("",dMSG_THAT_FIELD_EXCLUSIVE_FOR_GUILD,dMSG_OK);
			g_hero.m_wWaitMoveAreaResultTime	=	0;
			return;

		case dMOVEMAP_RESULT_ONE_WAY_PORTAL	:	//	일방통행 포탈입니다.
			g_msgBox.cPopup("",dMSG_THAT_PORTAL_IS_ONE_WAY_PORTAL,dMSG_OK);
			g_hero.m_wWaitMoveAreaResultTime	=	0;
			return;

		case dMOVEMAP_RESULT_PREMIUM_ZONE	:
		{
			g_hero.m_wWaitMoveAreaResultTime	=	0;
			g_msgBox.cPopup("",dMSG_CAN_NOT_MOVE_BY_PREMIUM_ZONE,dMSG_OK);
			return;
		} 

		case dMOVEMAP_RESULT_TEST_PORTAL	:
		{
			g_hero.m_wWaitMoveAreaResultTime	=	0;
			g_msgBox.cPopup("",dMSG_TEST_PORTAL_FOR_TEST,dMSG_OK);
			return;
		}

		case	dMOVEMAP_RESULT_CAN_NOT_MOVE_FIELD_STATUS	:
		{
			g_hero.m_wWaitMoveAreaResultTime	=	0;
			g_msgBox.cPopup("",dMSG_CAN_NOT_MOVE_BY_BAD_STATUS,dMSG_OK);
			return;
		}

		case dMOVEMAP_RESULT_MOVE_TO_IF		:
		{
			char	strFieldName[256];

			sprintf(strFieldName,"expansion/[%s]%.3d.rmd",lpPacket->strMapName,lpPacket->wFloor);

			CGamePlay::BookingMoveField(lpPacket->strIP,strFieldName);
			
			if	(s_strBookingToMoveFieldFileName[0] == 'G' && s_strBookingToMoveFieldFileName[1] == 'H')
				s_bIsGuildHall					=	TRUE;
			else
				s_bIsGuildHall					=	FALSE;

			break;
		}

		case dMOVEMAP_RESULT_SUCCESS		:
		{
			char	strFieldName[256];

			strcpy(strFieldName,lpPacket->strMapName);

			s_iFieldSerial		=	lpPacket->wFieldSerial;

			if	(lpPacket->bf4GuildHallLevel)
			{
				s_bIsGuildHall					=	TRUE;
				strcpy(s_strGuildHallOwnerGuildName,lpPacket->strGuildName);
			}
			else
				s_bIsGuildHall					=	FALSE;
			
			g_hero.syncHeroApprearance();

			CGamePlay::BookingMoveField(lpPacket->strIP,strFieldName);
			g_map.m_bIsDrawWeather = FALSE;					// 날씨 그리지마
			g_map.m_wWeather = lpPacket->bf4Weather;		// 날씨 변경.
			
			g_map.resetSnow();
			break;
		}

		default								:
			g_hero.m_wWaitMoveAreaResultTime	=	0;
			break;
	}
}

//
//	필드 이동
void
cPACKET_HANDLER::ReceiveMoveFieldNow(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::MoveField();
}

//
//	
void
cPACKET_HANDLER::ReceiveFirstAid(SERVER_PACKETS *_lpPacket)
{
	SG_FIRST_AID	*lpPacket	=	(SG_FIRST_AID *)_lpPacket;

	CActor			*lpActor	=	g_am.getTestedActor(lpPacket->wSerial);

	if (!lpActor)
		return;

	lpActor->m_iFirstAidHP		=	lpPacket->iFirstAidHP;
}

void
cPACKET_HANDLER::ReceiveRevive(SERVER_PACKETS *_lpPacket)
{
	SG_REVIVE	*lpPacket	=	(SG_REVIVE *)_lpPacket;

	CActor		*lpActor	=	g_am.getTestedActor(lpPacket->wSerial);

	if (!lpActor)
		return;

	lpActor->revive();
}

void
cPACKET_HANDLER::ReceiveUpdateBoostCount(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_BOOST_COUNT	*lpPacket	=	(SG_UPDATE_BOOST_COUNT *)_lpPacket;
	
	g_hero.m_bf3UseBoostCount			=	lpPacket->bf3UseBoostCount;
	g_hero.m_aPet[0].m_bf5BoostCount	=	lpPacket->bf5RemainPet1BoostCount;
	g_hero.m_aPet[1].m_bf5BoostCount	=	lpPacket->bf5RemainPet2BoostCount;
}
void
cPACKET_HANDLER::ReceiveXtrapPacket(SERVER_PACKETS *_lpPacket)
{
#ifdef	_USE_XTRAP
	SG_XTRAP_PACKET	*lpPacket	=	(SG_XTRAP_PACKET *)_lpPacket;

	CG_XTRAP_PACKET	packet;

	packet.base.set(sizeof(packet),dCG_XTRAP_PACKET);

	int	iResult	=	XTrap_CS_Step2(lpPacket->aBuffer,packet.aBuffer,XTRAP_PROTECT_PE|XTRAP_PROTECT_CODE|XTRAP_PROTECT_EXCEPT_VIRUS);

	s_agent.sendPacket((char*)&packet,packet.base.wSize);
#endif
}

void
cPACKET_HANDLER::ReceiveUpdatePacketEncryptCode(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_PACKET_ENCRYT_CODE	*lpPacket	=	(SG_UPDATE_PACKET_ENCRYT_CODE	*)_lpPacket;

	s_agent.m_iGameServerEncTable		=	lpPacket->wNewTable;

	memcpy(s_agent.m_abEncTable,lpPacket->abEncryptTable,sizeof(lpPacket->abEncryptTable));
}

void
cPACKET_HANDLER::ReceiveTitleEffect(SERVER_PACKETS *_lpPacket)
{
	SG_TITLE_EFFECT	*lpPacket	=	(SG_TITLE_EFFECT *)_lpPacket;

	CTitle	*lpTitle		=	g_titleManager.getTitle(lpPacket->wTitle);

	int		iSkill			=	lpTitle->m_aEffect[0].m_aiValue[0][0];
	CSkill	*lpSkill		=	&g_aSkill[iSkill];
	int		iPlaySoundCount	=	0;

	for (int i=0;i<lpPacket->wTargetCount;i++)
	{
		int		iActor		=	lpPacket->awTargetList[i];
		CActor	*lpActor	=	g_am.getActor(iActor);

		if	(!lpActor)
			continue;

		switch(lpPacket->wTitle)
		{
			case	eTN_BARD	:
			{
				if	(lpActor->isHero())
				{
					g_hero.m_wBardTitleValue	=	20;
					g_hero.m_wBardTitleValueTime=	60*5;

					g_hero.m_iHP	=	g_hero.getMaxHP();
					g_hero.m_iCP	=	g_hero.getMaxCP();
					lpActor->m_iMaxHP=	g_hero.getMaxHP();
				}

				lpActor->m_iHP	=	lpActor->m_iMaxHP;

				break;
			}
		}

		if	(lpSkill)
		{
			lpActor->addEffect(lpSkill->m_wHitImageOutputPart,lpSkill->m_wHitImage);

			if	(iPlaySoundCount	<	3)
			{
				g_esm.playHitSound(lpSkill);
				iPlaySoundCount++;
			}
		}
	}
}

void
cPACKET_HANDLER::ReceivePeneltyTime(SERVER_PACKETS *_lpPacket)
{
	SG_PENELTY_TIME	*lpPacket		=	(SG_PENELTY_TIME	*)_lpPacket;

	CGamePlay::WarningMessage(_ms(dMSG_PENELTY_TIME_FORM,lpPacket->dwPeneltyTime/60,lpPacket->dwPeneltyTime%60));
}

void
cPACKET_HANDLER::ReceiveWinningPrizeLotto(SERVER_PACKETS *_lpPacket)
{
	SG_WINNING_PRIZE_LOTTO	*lpPacket	=	(SG_WINNING_PRIZE_LOTTO	*)_lpPacket;

	char	strText[256];

	sprintf(strText,dMSG_WINNING_PRIZE_LOTTO_FORM,g_aBasicItem[lpPacket->wItem].m_strName);

	CGamePlay::Notice(strText);
}

void
cPACKET_HANDLER::ReceiveAccountInfo(SERVER_PACKETS *_lpPacket)
{
	SG_ACCOUNT_INFO	*lpPacket	=	(SG_ACCOUNT_INFO	*)_lpPacket;

	int	iTotalCount	=	0;

	for (int i=0;i<10;i++)
	{
		int	iCount	=	lpPacket->aPlayerCountPerServer[i];

		iTotalCount	+=	iCount;
		
		if (iCount)
			CGamePlay::AddSystemMessage(WHITE,"server <c:LTYELLOW>#%d<n> : exist <c:LTGREEN>%d<n> numbers Player",i+1,iCount);
	}

	CGamePlay::AddSystemMessage(WHITE,"Total <c:LTGREEN>%d<n> numbers Player Connected",iTotalCount);
}

void
cPACKET_HANDLER::ReceiveGGAuthCode(SERVER_PACKETS *_lpPacket)
{
#ifdef	_USE_NPGL
	SG_GG_AUTH_DATA			*lpPacket	=	(SG_GG_AUTH_DATA	*)_lpPacket;

	g_npgl->Auth2((GG_AUTH_DATA *)lpPacket->aBuffer);
#endif
}

void
cPACKET_HANDLER::ReceiveHSGuid(SERVER_PACKETS *_lpPacket)
{
#ifdef	_USE_HS
	SG_HS_GUID		*lpPacket	=	(SG_HS_GUID	*)_lpPacket;
	BYTE			abGuidAckMsg[SIZEOF_GUIDREQMSG];

	DWORD	dwResult	=	_AhnHS_MakeGuidAckMsg(lpPacket->abGuidReqMsg,abGuidAckMsg);

	if	(ERROR_SUCCESS	==	dwResult)
	{
		CG_HS_GUID_REPLY	packet;

		packet.base.set(sizeof(packet),dCG_HS_GUID_REPLY);
		memcpy(packet.abGuidAckMsg,abGuidAckMsg,SIZEOF_GUIDREQMSG);

		s_agent.sendPacket((char *)&packet,packet.base.wSize);
	}
#endif	
}

void
cPACKET_HANDLER::ReceiveHSReqMsg(SERVER_PACKETS *_lpPacket)
{
#ifdef	_USE_HS
	SG_HS_REQ_MSG	*lpPacket	=	(SG_HS_REQ_MSG	*)_lpPacket;
	BYTE			abAckMsg[SIZEOF_ACKMSG];

	DWORD	dwResult	=	_AhnHS_MakeAckMsg(lpPacket->abReqMsg,abAckMsg);

	if	(ERROR_SUCCESS	==	dwResult)
	{
		CG_HS_ACK_MSG_REPLY	packet;

		packet.base.set(sizeof(packet),dCG_HS_ACK_MSG_REPLY);
		memcpy(packet.abAckMsg,abAckMsg,SIZEOF_ACKMSG);

		s_agent.sendPacket((char *)&packet,packet.base.wSize);
	}
	else
		cMSG::Put("_AhnHS_MakeAckMsg Error!","result = %d",dwResult);

#endif	
}

void
cPACKET_HANDLER::ReceiveArcaList(SERVER_PACKETS *_lpPacket)
{
	SG_ARCA_LIST	*lpPacket	=	(SG_ARCA_LIST	*)_lpPacket;

	g_arcaManager.receiveExistArcaList(lpPacket->aArcaList,lpPacket->bf8Count,lpPacket->bf1IsFirst);
}

void
cPACKET_HANDLER::ReceiveAddArca(SERVER_PACKETS *_lpPacket)
{
	SG_ADD_ARCA		*lpPacket	=	(SG_ADD_ARCA	*)_lpPacket;

	g_arcaManager.addArca(&lpPacket->arcaInfo);

	if	(lpPacket->arcaInfo.m_wSerial	==	0x7fff)
		CGamePlay::AddSystemMessage(LTGREEN,dMSG_SUCCESS_TO_USE_TREASURE_MAP);
}

void
cPACKET_HANDLER::ReceiveArcaWork(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperateArcaWork((SG_CLOSE_RANGE_ARCA_WORK *)_lpPacket);
}

void
cPACKET_HANDLER::ReceiveBrokenArcaItemByAttack(SERVER_PACKETS *_lpPacket)
{
	SG_BROKEN_ARCA_ITEM_BY_ATTACK	*lpPacket	=	(SG_BROKEN_ARCA_ITEM_BY_ATTACK	*)_lpPacket;

	for (int i=0;i<lpPacket->wCount;i++)
	{
		cItem	*lpItem	=	(cItem *)&lpPacket->aItem[i];

		char	strMessage[512];
		DWORD	dwColor;

		dwColor	=	lpItem->getNameColor();

		sprintf(strMessage,dMSG_BROKEN_ARCA_ITEM_BY_ATTACK_FORM,lpItem->getName(FALSE));

		CGamePlay::EventNotice(strMessage,eCT_GET_PARTY_ITEM_MESSAGE,FALSE,dwColor);
	}
}

void
cPACKET_HANDLER::ReceiveGGGInfo(SERVER_PACKETS *_lpPacket)
{
	SG_GGG_INFO *lpPacket = (SG_GGG_INFO *)_lpPacket;		

	g_hero.m_GGG.m_wGGGTimeForSkillUp	= lpPacket->wValue[3];
	g_hero.m_GGG.m_wGGGValue1ForSkillUp = lpPacket->wValue[4];
	g_hero.m_GGG.m_wGGGValue2ForSkillUp = lpPacket->wValue[5];
}	

void 
cPACKET_HANDLER::ReceiveGGGEffectPlay(SERVER_PACKETS *_lpPacket)
{
	SG_GGG_INFO *lpPacket	= (SG_GGG_INFO *)_lpPacket;
	WORD		wType		= lpPacket->wValue[0];

	//lpPacket->wValue[0]	=	타입 정보 <-비트마스크한 값이다.
	//lpPacket->wValue[1]	=	엑터 시리얼.
	//lpPacket->wValue[2]	=	GGG이펙트 EX> 1 = FIRE  , 2 = WATER , 3 = WIND , 4 = EARTH

	CActor	*lpActor	=	g_am.getTestedActor(lpPacket->wValue[1]);	
	CActor	*lpAttacker	=	g_am.getTestedActor(lpPacket->wValue[3]);	

	if(!lpActor)
		return;

	if(!lpAttacker)
		return;

	int iEffect = 0;

	if (wType & dGGG_EFFECT_PLAYTYPE_POINT_UP)
	{
		lpAttacker->m_bf3HitGGGType = lpPacket->wValue[2];
		lpAttacker->m_bf1IsGGGPointHit	= TRUE;
	} 

	if(wType & dGGG_EFFECT_PLAYTYPE_HIT)
	{
 		lpAttacker->m_bf3HitGGGType = lpPacket->wValue[2];
 		lpAttacker->m_bf1IsGGGNormalHit= TRUE;
	}

	if(wType & dGGG_EFFECT_PLAYTYPE_HIT_BY)
	{
 		lpAttacker->m_bf3HitGGGType = lpPacket->wValue[4];
 		lpAttacker->m_bf1IsGGGHitBy = TRUE;
	}

	int	iHitEffect		=	lpActor->getFreeHitIndex();
	CPos	pos;
	
	lpActor->getHitPointCorrectPos(0,&pos);
	
	if(lpPacket->wValue[5])
		lpActor->m_aHitEffect[iHitEffect] = g_hem.addEffectAndInfo(pos.x,pos.y,0xffff,dHIT_INFO_GREEN_NUMBER,lpPacket->wValue[5],0,0,FALSE);	//	타격효과 추가
}

void
cPACKET_HANDLER::ReceiveGGGSkillUpTimeEnd(SERVER_PACKETS *_lpPacket)
{
	SG_GGG_INFO *lpPacket = (SG_GGG_INFO *)_lpPacket;

	g_hero.m_GGG.m_wGGGTimeForSkillUp = 0;
//	g_lpHero->m_wGGGBuffType = 0;
	g_hero.m_GGG.ResetGGGSkill();
	g_hero.buildPower();
}

void
cPACKET_HANDLER::ReceiveGGGSkillUp(SERVER_PACKETS *_lpPacket)
{
	SG_GGG_INFO				*lpPacket	=	(SG_GGG_INFO	*)_lpPacket;

	g_hero.m_GGG.m_wGGGTimeForSkillUp		= lpPacket->wValue[0];//시간.
	g_hero.m_GGG.m_wGGGValue1ForSkillUp	= lpPacket->wValue[1];//
	g_hero.m_GGG.m_wGGGValue2ForSkillUp	= lpPacket->wValue[2];//
}


void
cPACKET_HANDLER::ReceiveGGG(SERVER_PACKETS *_lpPacket)
{
	SG_GGG_INFO				*lpPacket	=	(SG_GGG_INFO	*)_lpPacket;

	g_hero.m_bf3GreateGodGrace = lpPacket->wValue[0];

	char* pstrComment = NULL;

	switch (g_hero.m_bf3GreateGodGrace)
	{
	case 1:			//불
		pstrComment = _ms(dMSG_GGG , dMSG_GGG_NAME_FIRE,dMSG_GET);
		break;
	case 2:			//물
		pstrComment = _ms(dMSG_GGG , dMSG_GGG_NAME_WATER,dMSG_GET);
		break;
	case 3:			//바람
		pstrComment = _ms(dMSG_GGG , dMSG_GGG_NAME_WIND,dMSG_GET);
		break;
	case 4:			//땅
		pstrComment = _ms(dMSG_GGG , dMSG_GGG_NAME_EARTH,dMSG_GET);
		break;
	}

	g_hero.AddSystemMessage(WHITE , pstrComment);
}

void
cPACKET_HANDLER::ReceiveRemoveGGG(SERVER_PACKETS *_lpPacket)
{
	if(!g_hero.m_bf3GreateGodGrace)	return;
	char* pstrComment = NULL;
	
	switch (g_hero.m_bf3GreateGodGrace)
	{
	case 1:			//불
		pstrComment = _ms(dMSG_GGG , dMSG_GGG_NAME_FIRE,dMSG_REMOVE);
		break;
	case 2:			//물
		pstrComment = _ms(dMSG_GGG , dMSG_GGG_NAME_WATER,dMSG_REMOVE);
		break;
	case 3:			//바람
		pstrComment = _ms(dMSG_GGG , dMSG_GGG_NAME_WIND,dMSG_REMOVE);
		break;
	case 4:			//땅
		pstrComment = _ms(dMSG_GGG , dMSG_GGG_NAME_EARTH,dMSG_REMOVE);
		break;
	}

	g_hero.m_GGG.ResetGGG();
	g_hero.m_bf3GreateGodGrace = 0 ;

	g_hero.AddSystemMessage(WHITE , pstrComment);
}

