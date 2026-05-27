#include "cGAME.H"
#include "debugCode.H"
#include "cServer.H"
#include "secretDungeon.H"
#include "CGuild.H"
#include "booking_work.h"


//
//	액터 빈 자리 찾기
cACTOR*
cGAME::getFreeActor()
{
	mCS(m_csActorManager);

	int		i,iReturnActorSerial	=	m_iRookieActor;

	if (m_iActorCount	>=	m_iMaxActorCount)
		return	NULL;

	m_pActor[m_iRookieActor].m_iZoneSerial	=	m_iRookieActor;

	for (i=m_iRookieActor+1;i<m_iMaxActorCount;i++)
	{
		if (m_pActor[i].m_iZoneSerial	==	0xffffffff)
		{
			m_iRookieActor	=	i;

			break;
		}
	}

	m_iActorCount++;

	return	&m_pActor[iReturnActorSerial];
}

//
//	액터 제거(필드에서는 따로 제거해야 한다.)
BOOL
cGAME::removeActor(int _iSerial)
{
	mCS(m_csActorManager);

	if (_iSerial<0	||	_iSerial>=m_iMaxActorCount)
	{
		CLOG("removeActor","------------------------- [%8d]reset in cGAME::removeActor - Failed",_iSerial);
		return	FALSE;
	}

	if (m_pActor[_iSerial].m_iZoneSerial	==	0xffffffff)
	{
		CLOG("removeActor","------------------------- [%8d,%8d]reset in cGAME::Failed2",m_pActor[_iSerial].m_wCurrentField,m_pActor[_iSerial].m_wSerialInField);
		return	FALSE;
	}
//	LOG("[%8d,%8d]reset in cGAME::removeActor",m_pActor[_iSerial].m_wCurrentField,m_pActor[_iSerial].m_wSerialInField);
	m_pActor[_iSerial].reset();

	if (_iSerial	<	m_iRookieActor)
		m_iRookieActor	=	_iSerial;

	m_iActorCount--;

	return	TRUE;
}

//
//	플레이어 참가 예약
int
cGAME::bookingJoinPlayer(CPlayerSaveData *_lpPlayerData,CUpkeepPlayerData *_lpUpkeepData,cActorStatusUpkeep *_lpUpkeepStatus,int _iClientSerial,BOOL _bIsFirstConnect)
{
	cACTOR	*lpActor	=	(cACTOR *)_lpPlayerData;

	if	(g_bIsDuelServer)
	{
		if	(g_duelTrialGameManager.isOfflineTournamentSeason())
			if	(g_duelTrialGameManager.checkEntry(lpActor)	==	FALSE)
				return	dRESULT_JOIN_FIELD_FAILED_BY_PROCESS_OFFLINE_TOURNAMENT;
	}

	if	(_lpUpkeepData->getPartySerial()	==	0xffff)
		_lpUpkeepData->setPartySerial(g_pPartyManager->checkDisconnectPlayer(lpActor,TRUE));

	DWORD	dwUniquePartySerial			=	0xfffffff7;

	if	(lpActor->m_wCurrentField		>=	dINSTANCE_FIELD_FLAG	&&	_lpUpkeepData->getPartySerial()	!=	0xffff)
	{
		cParty	*lpParty	=	g_pPartyManager->get(_lpUpkeepData->getPartySerial());

		if	(lpParty)
			dwUniquePartySerial	=	lpParty->m_dwUniqueSerial;
	}

	cFIELD		*lpField	=	getFieldByUniqueSerial(lpActor->m_wCurrentField,dwUniquePartySerial,lpActor->m_bf8InstanceField,lpActor->m_bf4InstanceFieldFloor,FALSE,lpActor->m_bf10ESD_Time);	//	고유시리얼로 필드 얻기!!

	if	(!lpField)
	{
		if	(lpActor->m_wCurrentField	>=	dINSTANCE_FIELD_FLAG)
		{
			lpActor->m_wCurrentField		=	lpActor->m_wCurrentField-dINSTANCE_FIELD_FLAG;
			lpActor->m_iXPos				=	-1;
			lpActor->m_iYPos				=	-1;
			lpActor->m_bf8InstanceField		=	0;
			lpActor->m_bf4InstanceFieldFloor=	0;
			lpActor->m_dwSaveCount++;
			lpActor->immediatelySendSaveDataToDBMemory();

			lpField	=	getFieldByUniqueSerial(lpActor->m_wCurrentField,dwUniquePartySerial);	//	고유시리얼로 필드 얻기!!

			if	(!lpField)
				return	dRESULT_JOIN_FIELD_FAILED_BY_NOT_SECRET_DUNGEON_MEMBER;

			lpActor->m_bf1IsBannedSecretDungeon	=	TRUE;
		}
		else
		{
			lpActor->m_wCurrentField		=	lpActor->m_wLastVillage;
			lpActor->m_iXPos				=	-1;
			lpActor->m_iYPos				=	-1;
			lpActor->m_dwSaveCount++;
			lpActor->sendUrgentSaveDataToDBCache(TRUE);

			return	dRESULT_JOIN_FIELD_FAILED_BY_TRY_CONNECT_TO_INCORRECT_FIELD;
		}
	}
	else
	{
		if	(lpField->m_bIsSiegeWarfareField)
		{
			int		iTeam				=	lpField->getTeamInGuildBattleField(lpActor->m_wGuildSerial,lpActor->m_strName);

			if	(iTeam	==	0xffff)
			{
				lpActor->m_bf1IsPlayOnGuildBattle=	FALSE;
				lpActor->m_wCurrentField		=	lpActor->m_wLastVillage;
				lpActor->m_iXPos				=	-1;
				lpActor->m_iYPos				=	-1;
				lpActor->m_dwSaveCount++;
				lpActor->sendUrgentSaveDataToDBCache(TRUE);

				return	dRESULT_JOIN_FIELD_FAILED_BY_TRY_CONNECT_TO_INCORRECT_FIELD;
			}
		}
		else
		if	(lpField->m_bIsGuildBattleField)
		{
			int		iOperatorLevel	=	0;

			CClient	*lpClient	=	CM.GetClientPnt((WORD)_iClientSerial);

			if	(lpClient)
				iOperatorLevel	=	lpClient->GetIsOper();

			int		iTeam				=	lpField->getTeamInGuildBattleField(lpActor->m_wGuildSerial,lpActor->m_strName);

			BOOL	bIsIncorrectTime	=	FALSE;

			if	(iTeam	==	0xffff)
			{
				if	(iOperatorLevel	>=	4)
					iTeam	=	0;
				else
					bIsIncorrectTime		=	TRUE;
			}
			else
			if	(lpActor->m_bf1IsPlayOnGuildBattle	==	FALSE && g_bIsDuelServer == FALSE)
				bIsIncorrectTime		=	TRUE;
			else	//lpField->m_wDuelType	==	0xffff	JBC 듀얼 필드 개방을 위해...08-08-25
			if	(lpField->m_wDuelType	==	0xffff	&&	lpActor->m_bf5GuildBattleDay		!=	g_currentTime.m_wDay && !g_bIsDuelTestPC)
				bIsIncorrectTime		=	TRUE;
			else
			if	(g_bIsTestGuildBattle			==	FALSE		&&
				lpField->m_wGuildBattleStatus	!=	eGB_ENTERENCE&&
				lpField->m_wGuildBattleStatus	!=	eGB_READY	&&
				lpField->m_wGuildBattleStatus	!=	eGB_BATTLE	&&
				lpField->m_wGuildBattleStatus	!=	eGB_INIT	&&
				lpField->m_wGuildBattleStatus	!=	eGB_READY_TO_BATTLE_FINISH)
				bIsIncorrectTime		=	TRUE;

//			CLOG("MoveMap","bookingJoinPlayer [%s] field[%d] iteam[%d] IsPlayOnGuildBattle[%d] guildBattleDay[%d] currentDay[%d] IsIncorrectTime[%d] guildbattlestatus[%d]",
//				_lpPlayerData->m_strName, lpActor->m_wCurrentField,lpField->getTeamInGuildBattleField(lpActor->m_wGuildSerial,lpActor->m_strName),lpActor->m_bf1IsPlayOnGuildBattle,lpActor->m_bf5GuildBattleDay,
//				g_currentTime.m_wDay,bIsIncorrectTime,lpField->m_wGuildBattleStatus);

			if	(bIsIncorrectTime && iOperatorLevel < 4)
			{
				lpActor->m_bf1IsPlayOnGuildBattle=	FALSE;
				lpActor->m_wCurrentField		=	lpActor->m_wLastVillage;
				lpActor->m_iXPos				=	-1;
				lpActor->m_iYPos				=	-1;
				lpActor->m_dwSaveCount++;
				lpActor->sendUrgentSaveDataToDBCache(TRUE);

// 				if	(lpClient)
// 					g_game.sendWPUserLogoutById(lpActor->m_strId,lpClient->ip);
// 				else
// 					g_game.sendWPUserLogoutById(lpActor->m_strId,"EMPTY");
				DisUser(lpActor->m_strId,"field move");

				return	dRESULT_JOIN_FIELD_FAILED_BY_TRY_CONNECT_TO_INCORRECT_FIELD;
			}
		}

		if	(lpActor->m_wCurrentField	>=	dINSTANCE_FIELD_FLAG)
		{
			BOOL	bIsIncorrectTime=	FALSE;

			cInstanceField	*lpIF	=	g_pIfManager->get(lpActor->m_bf8InstanceField);

			if	(lpIF	&&	lpIF->m_wIsReadyToClose)
				bIsIncorrectTime	=	TRUE;

			if	(lpField->m_wGateField			!=	lpActor->m_wCurrentField-dINSTANCE_FIELD_FLAG)
				bIsIncorrectTime	=	TRUE;

			if	(bIsIncorrectTime	==	TRUE)
			{
				lpActor->m_wCurrentField		=	lpActor->m_wLastVillage;
				lpActor->m_iXPos				=	-1;
				lpActor->m_iYPos				=	-1;
				lpActor->m_bf8InstanceField		=	0;
				lpActor->m_bf4InstanceFieldFloor=	0;
				lpActor->m_dwSaveCount++;

				lpActor->sendUrgentSaveDataToDBCache(TRUE);

				return	dRESULT_JOIN_FIELD_FAILED_BY_NOT_SECRET_DUNGEON_MEMBER;
			}
		}
	}

	return	g_pJoinPlayerManager->booking(_lpPlayerData,_lpUpkeepData,_lpUpkeepStatus,_iClientSerial,_bIsFirstConnect);
}

//
//	플래이어 얻기
cACTOR*
cGAME::getPlayer(char *_lpstrName)
{
	DWORD	dwNameHashCode	=	GetHashCode(_lpstrName);

	for (int iField=m_iFieldCount;iField;)
	{
		--iField;

		cACTOR	*lpActor	=	m_pField[iField].getPlayerByName(_lpstrName,dwNameHashCode);

		if	(lpActor)
			return	lpActor;
	}

	return	g_pIfManager->getPlayerByName(_lpstrName);
}

//
//	플래이어 얻기
cACTOR*
cGAME::getPlayerByID(char *_lpstrID)
{
	DWORD	dwIDHashCode	=	GetHashCode(_lpstrID);

	for (int iField=m_iFieldCount;iField;)
	{
		--iField;

		cACTOR	*lpActor	=	m_pField[iField].getPlayerByID(_lpstrID,dwIDHashCode);

		if	(lpActor)
			return	lpActor;
	}

	return	g_pIfManager->getPlayerByID(_lpstrID,dwIDHashCode);
}

