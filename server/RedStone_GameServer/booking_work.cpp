#include "cGAME.H"
#include "debugCode.H"
#include "cServer.H"
#include "secretDungeon.H"
#include "CGuild.H"
#include "booking_work.H"
#include "packetManager.H"
#include "GameDLL.H"

cJoinPlayerManager*		g_pJoinPlayerManager;
cMoveFieldUserManager*	g_pMoveFieldUserManager;

cJoinPlayerManager::cJoinPlayerManager()
{
	m_iCount	=	0;
	FPInitCS(&m_cs);
}

cJoinPlayerManager::~cJoinPlayerManager()
{
	m_iCount	=	0;

	DeleteCriticalSection(&m_cs);
}

int
cJoinPlayerManager::booking(CPlayerSaveData *_lpPlayerData,CUpkeepPlayerData *_lpUpkeepData,
							cActorStatusUpkeep *_lpUpkeepStatus,int _iClientSerial,BOOL _bIsFirstConnect)
{
	mCS(m_cs);

	if	(m_iCount	>=	dMAX_BOOKING_JOIN_PLAYER_COUNT)
		return	dRESULT_JOIN_FIELD_FAILED_BY_TOO_MANY_CONNECTOR;

	cACTOR					*lpActor			=	(cACTOR	*)_lpPlayerData;
	CClient					*lpClient			=	CM.mGETCLIENT(_iClientSerial);
	cBookedJoinPlayerData	*lpJoinPlayerData	=	&m_aData[m_iCount];

	if (!lpClient)
	{
		CLOG("bookingExit","cJoinPlayerManager::booking - invalid client serial!! [%d][%s/%s]",_iClientSerial,lpActor->m_strId,lpActor->m_strName);

		return	dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_CONNECT;
	}
	else
	{
		int	iResult	=	lpActor->checkData(lpClient,"cJoinPlayerManager::booking",FALSE);

		if	(iResult!=	eCADR_ISOK)	
		{
			_lpPlayerData->m_wJob	=	0xffff;

			CLOG("m_wJob","booking %s/%s",_lpPlayerData->m_strId,_lpPlayerData->m_strName);

			return	dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA;
		}
	}

	if	(_lpPlayerData->m_iHP	<=	0)
		_lpPlayerData->m_iHP	=	0;

	memcpy(&lpJoinPlayerData->data,_lpPlayerData,sizeof(CPlayerSaveData));
	memcpy(lpJoinPlayerData->data.getUpkeepPlayerDataPointer(),_lpUpkeepData,sizeof(CUpkeepPlayerData));
	memcpy(&lpJoinPlayerData->m_upkeepStatus,_lpUpkeepStatus,sizeof(cActorStatusUpkeep));

	lpJoinPlayerData->m_iClientSerial	=	_iClientSerial;
	lpJoinPlayerData->m_iFieldSerial	=	_lpPlayerData->m_wCurrentField;
	lpJoinPlayerData->m_bIsFirstConnect	=	_bIsFirstConnect;

	m_iCount++;

	return	dRESULT_JOIN_FIELD_SUCCESS;
}

void
cJoinPlayerManager::operate()
{
	if (m_iCount	<=	0)
		return;

	mCS(m_cs);

	while(m_iCount)
	{
		--m_iCount;

		cBookedJoinPlayerData	*lpData		=	&m_aData[m_iCount];
		CClient					*lpClient	=	CM.mGETCLIENT(lpData->m_iClientSerial);
		cACTOR					*lpActor	=	(cACTOR *)&lpData->data;
		DWORD					dwUniquePartySerial	=	0xfffffff7;

		if (lpData->m_iFieldSerial	>=	dINSTANCE_FIELD_FLAG)
		{
			cParty	*lpParty	=	lpActor->getParty();

			if (lpParty)
				dwUniquePartySerial	=	lpParty->m_dwUniqueSerial;
		}

		cFIELD					*lpField	=	g_game.getFieldByUniqueSerial(lpData->m_iFieldSerial,dwUniquePartySerial,lpActor->m_bf8InstanceField,lpActor->m_bf4InstanceFieldFloor);

		if	(!lpField)
		{
			CLOG("joinPlayer","cJoinPlayerManager::operate - can not find filed!![%d][%s/%s]",lpData->m_iFieldSerial,lpActor->m_strId,lpActor->m_strName);
			continue;
		}
		
		if	(!lpClient)
		{
			CLOG("joinPlayer","[%s]cJoinPlayerManager::operate - 클라이언트 시리얼 무효!![%d][%s/%s]",lpField->m_strFileName,lpData->m_iClientSerial,lpActor->m_strId,lpActor->m_strName);
			continue;
		}

		lpActor		=	lpField->joinPlayer(&lpData->data,lpData->m_iClientSerial,FALSE,&lpData->m_upkeepStatus);

		if	(!lpActor)
		{
			if	(lpField->isEventField() || g_bIsDuelServer)
			{
				cACTOR	*lpPlayer			=	(cACTOR *)&lpData->data;

				lpPlayer->m_wCurrentField	=	lpPlayer->m_wLastVillage;
				lpPlayer->m_iXPos			=	-1;
				lpPlayer->m_iYPos			=	-1;
				lpPlayer->m_dwSaveCount++;

				lpPlayer->sendUrgentSaveDataToDBCache(TRUE);
			}
			
			CLOG("joinPlayer","cJoinPlayerManager::operate - 참가 실패!![%d][%s/%s]",lpData->m_iClientSerial,lpData->data.m_strId,lpData->data.m_strName);
			continue;	//	처리 -_-a
		}

		lpClient->SetState(CClient::eSTATE_INGAME);
		lpClient->SetActorSerialInGame(lpActor->m_iZoneSerial);
		lpClient->SetActorSerialInField(lpActor->m_wSerialInField);
		lpClient->SetFieldSerial(lpField->m_iUniqueSerial);	//	로컬 필드 인덱스
		lpClient->SetFieldIndex(lpField->m_iSerial);	//	로컬 필드 인덱스

//		lpClient->SetIsUsed(eISUSED_JOINSERVER);

		if	(g_iWorldServerType	==	eSERVER_TYPE_GVG && lpData->m_bIsFirstConnect)
			lpActor->m_bIsWaitOrgAvatarData	=	TRUE;
		else
			lpActor->m_bIsWaitOrgAvatarData	=	FALSE;
		
		int	iResult	=	lpActor->checkData(lpClient,"cFIELD::operateBookedPlayerToJoin",TRUE);

		if	(lpActor->getPartySerial()	==	0xffff)
			continue;

		int	iPartySerial	=	g_pPartyManager->checkDisconnectPlayer(lpActor,FALSE);

		g_pPartyManager->setMemberZoneSerial(lpActor->getPartySerial(),lpActor->m_strName,lpActor->m_iZoneSerial);
	}
}

cMoveFieldUserManager::cMoveFieldUserManager()
{
	m_iTail	=	0;
	m_iHead		=	0;

	FPInitCS(&m_cs);
}

cMoveFieldUserManager::~cMoveFieldUserManager()
{
	m_iTail	=	0;

	DeleteCriticalSection(&m_cs);
}

BOOL
cMoveFieldUserManager::booking(cACTOR *_lpActor,char *_lpstrFieldFileName,int _iFieldIndex,int _iGate,int _iX,int _iY)
{
	if	(_lpActor->isPlayer()	==	FALSE)
	{
		CLOG("moveFieldFailed","플레이어가 아닌데 필드 이동한다고 했다. [%s/%s]",_lpActor->m_strId,_lpActor->m_strName);

		return	FALSE;
	}

	if	(_lpActor->m_wWaitBuyCarrotShopItemResultTime)
		return	FALSE;

	mCS(m_cs);

	//	이미 예약된것이 있는지 확인
	{
		int	iHead	=	m_iHead;

		while(iHead	!=	m_iTail)
		{
			cBookedMoveFieldUserData	*lpData	=	&m_aData[iHead];

			if	(lpData->m_iUserSerial	==	_lpActor->m_iZoneSerial	&&	STRICMP(lpData->m_strName,_lpActor->m_strName)==0)	//	있다!!
			{
				if	(_lpstrFieldFileName	==	NULL)
					lpData->m_strFieldFileName[0]	=	NULL;
				else
					strcpy(lpData->m_strFieldFileName,_lpstrFieldFileName);

				lpData->m_iFieldSerial		=	_iFieldIndex;
				lpData->m_iGateIndex		=	_iGate;
				lpData->m_iX				=	_iX;
				lpData->m_iY				=	_iY;

				return	TRUE;
			}

			iHead++;

			if	(iHead	>=	dMAX_BOOKING_MOVE_FIELD_USER_COUNT)
				iHead	=	0;
		}
	}

	if	(((m_iTail+1)%dMAX_BOOKING_MOVE_FIELD_USER_COUNT)	==	m_iHead)	//	더이상 예약 할 수 없다.
		return	FALSE;

	cBookedMoveFieldUserData	*lpData	=	&m_aData[m_iTail];

	strcpy(lpData->m_strName,_lpActor->m_strName);

	lpData->m_iUserSerial		=	_lpActor->m_iZoneSerial;

	if	(_lpstrFieldFileName	==	NULL)
		lpData->m_strFieldFileName[0]	=	NULL;
	else
		strcpy(lpData->m_strFieldFileName,_lpstrFieldFileName);

	lpData->m_iFieldSerial		=	_iFieldIndex;
	lpData->m_iGateIndex		=	_iGate;
	lpData->m_iX				=	_iX;
	lpData->m_iY				=	_iY;

	m_iTail++;

	if	(m_iTail	>=	dMAX_BOOKING_MOVE_FIELD_USER_COUNT)
		m_iTail		=	0;

	return	TRUE;
}

void
cMoveFieldUserManager::operate()
{
	mCS(m_cs);

	if	(m_iHead	==	m_iTail)
		return;

	int	i;

	for (i=0;i<1 && m_iHead	!=	m_iTail;)
	{
		cBookedMoveFieldUserData	*lpData		=	&m_aData[m_iHead];

		m_iHead++;

		if	(m_iHead	>=	dMAX_BOOKING_MOVE_FIELD_USER_COUNT)
			m_iHead		=	0;

		cACTOR*	lpActor		=	g_game.getActor(lpData->m_iUserSerial);

		if	(!lpActor	||	lpActor->isPlayer() == FALSE || STRICMP(lpData->m_strName,lpActor->m_strName) != 0)
			continue;

		i++;

		g_game.sendWPMoveField(lpActor,lpData->m_strFieldFileName,lpData->m_iFieldSerial,lpData->m_iGateIndex,lpData->m_iX,lpData->m_iY);
	}
}