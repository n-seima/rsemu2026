#include "cACTOR.H"
#include "cServer.H"
#include "CGame.H"
#include "CDataSave.H"
#include "debugCode.H"
#include "cPACKET_DBSERVER.h"

//
//	클라이언트에 문제가 없나 체크합니다.
BOOL
cACTOR::checkClient(CClient *_lpClient,char *_lpstrLog)
{
	if	(isDummy())
		return	FALSE;

	SYSTEMTIME	time;

	if (!_lpClient)
	{
		GetLocalTime(&time);

		_lpClient	=	getClient();

		if (!_lpClient)
		{
			CLOG("saveFailed","실패1 -%s-  [%20s/%20s] [%2d/%2d/%2d] [%2d:%2d:%2d]",_lpstrLog,m_strId,m_strName,
								time.wYear,time.wMonth,time.wDay,
								time.wHour,time.wMinute,time.wSecond);
			return	FALSE;
		}
	}

	if	(checkData(_lpClient,_lpstrLog,TRUE)!=eCADR_ISOK)
	{
		GetLocalTime(&time);

		CLOG("saveFailed","실패2 -%s- [%20s/%20s] [%2d/%2d/%2d] [%2d:%2d:%2d]",_lpstrLog,m_strId,m_strName,
							time.wYear,time.wMonth,time.wDay,
							time.wHour,time.wMinute,time.wSecond);
		return	FALSE;
	}

	return	TRUE;
}

//
//	세이브 데이터를 월드로 전송한다.
BOOL
cACTOR::sendSaveDataToDBFile(BOOL _bIsTemporaryBackup,BOOL _bIsRequireSendLog)
{
	if	(checkData(NULL,"cACTOR::sendSaveDataToDBFile",FALSE)!=eCADR_ISOK)
	{
		CLOG("saveFailed","cACTOR::sendSaveDataToDBFile- [%20s/%20s]",m_strId,m_strName);
		return	FALSE;
	}

	int	iSerial	=	0xffffffff;

	if	(!_bIsTemporaryBackup)
	{
		if	(m_iXPos	>	0)
		{
			m_iXPos	=	m_posLastMoved.x;
			m_iYPos	=	m_posLastMoved.y;
		}

		m_dwSaveCount++;

		SERVERStoWORLD_SAVEAVA	packet;

		packet.base.set(sizeof(SERVERStoWORLD_SAVEAVA),dSERVERStoWORLD_SAVEAVA);
		memcpy(&packet.aInfo,this,sizeof(CPlayerSaveData));

		if	(m_lpField && m_lpField->isSpecialField()	&&	(m_lpField->m_bIsGuildBattleField	==	FALSE	&&	m_bf1IsPlayOnGuildBattle))
		{
			packet.aInfo.m_wCurrentField	=	m_wLastVillage;
			packet.aInfo.m_iXPos			=	-1;
			packet.aInfo.m_iYPos			=	-1;
		}

		g_socketWorld.SendPacket((char *)&packet,packet.base.wSize);

		m_wWaitMoveFieldResultTime	=	dWAIT_MOVE_FIELD_RESULT_TIME;
		iSerial						=	m_iZoneSerial;
	}

//	DB File에 저장
	{
		if	(!_bIsTemporaryBackup)
		{
			CClient	*lpClient		=	getClient();

//			if	(lpClient)
//				lpClient->SetIsUsed(eISUSED_WDMSG_SAVEAVA_TO_FILE);
		}

		WDMSG_SAVEAVA_TO_FILE	packet;

		packet.wIsWantResult	=	1-_bIsTemporaryBackup;
		packet.iSerialInServer	=	iSerial;
		packet.base.set(sizeof(WDMSG_SAVEAVA_TO_FILE),dWDMSG_SAVEAVA_TO_FILE);

		memcpy(&packet.pData,this,sizeof(CPlayerSaveDataForServerDefine));

		if	(!g_socketDBC.SendPacket((char *)&packet,((cMSG_BASE_TYPE *)&packet)->wSize))
		{
			printf("critical fault!! save failed!!\n");
			CLOG("saveError","%s/%s",m_strId,m_strName);
		}

		if	(_bIsRequireSendLog)
		{
			saveToFile();
			sendSaveLog();
		}
	}

	return TRUE;
}

void
cACTOR::immediatelySendSaveDataToDBMemory(BOOL _bIsRequireLog)
{
	g_pDsManager->remove(m_strName);

	m_dwSaveCount	+=	2;

	sendSaveDataToDBFile(TRUE,_bIsRequireLog+isRequireSendSaveLog());	//	일단 저장해!!

	m_iLastestCheckSumValueForSaveData	=	getCheckSumValueForSaveData();
}

void
cACTOR::saveToFile()
{
	return;
}

void
cACTOR::loadFromFile()
{
	return;
}

//
//	데이터를 DB캐쉬로 전송한다.
BOOL
cACTOR::sendSaveDataToDBCache()
{
	CClient	*lpClient		=	CM.GetClientPntByIgnoreUsedFlag(m_iClientSerial);

	if	(lpClient == NULL || lpClient->wSerial==0xffff)
	{
		CLOG("logOutError","sendSaveDataToDBCache 실패 - 클라이언트가 없음");
		
		return	FALSE;
	}

	if	(checkClient(lpClient,"cACTOR::sendSaveDataToDBCache"))
	{
		GDMSG_LOGOUTAVATAR	packet;

		packet.base.set(sizeof(GDMSG_LOGOUTAVATAR),dGDMSG_LOGOUTAVATAR);
		packet.iActorSerialInServer	=	m_iZoneSerial;
		m_dwSaveCount++;

		if	(m_iXPos	!=	-1)
		{
			m_iXPos	=	m_posLastMoved.x;
			m_iYPos	=	m_posLastMoved.y;
		}

		memcpy(&packet.pData,this,sizeof(CPlayerSaveDataForServerDefine));

		if	(m_lpField && m_lpField->isSpecialField()	&&	(m_lpField->m_bIsGuildBattleField	==	FALSE	&&	m_bf1IsPlayOnGuildBattle))
		{
			packet.pData.m_wCurrentField	=	m_wLastVillage;
			packet.pData.m_iXPos			=	-1;
			packet.pData.m_iYPos			=	-1;
		}

		g_socketDBC.SendPacket((char *)&packet,((cMSG_BASE_TYPE *)&packet)->wSize);

		m_wWaitLogoutResultTime				=	dWAIT_LOGOUT_RESULT_TIME;

		saveToFile();
		sendSaveLog();

		return TRUE;
	}

	return	FALSE;
}

//
//	데이터를 DB캐쉬로 전송한다.
BOOL
cACTOR::sendUrgentSaveDataToDBCache(BOOL _bIsForJoinFailed)
{
	if	(checkData(NULL,"cACTOR::sendUrgentSaveDataToDBCache",FALSE) != eCADR_ISOK)
		return FALSE;

	GDMSG_LOGOUTAVATAR	packet;

	packet.base.set(sizeof(GDMSG_LOGOUTAVATAR),dGDMSG_LOGOUTAVATAR);
	packet.iActorSerialInServer	=	0xffffffff;

	memcpy(&packet.pData,this,sizeof(CPlayerSaveDataForServerDefine));

	if	(_bIsForJoinFailed == FALSE && m_lpField && m_lpField->isSpecialField())
	{
		packet.pData.m_wCurrentField	=	m_wLastVillage;
		packet.pData.m_iXPos			=	-1;
		packet.pData.m_iYPos			=	-1;
	}

	g_socketDBC.SendPacket((char *)&packet,((cMSG_BASE_TYPE *)&packet)->wSize);

	sendSaveLog();

	return TRUE;
}

//
//	서버 종료할때 데이터 저장
BOOL
cACTOR::saveDataForTerminateServer()
{
	if (checkData(NULL,"cACTOR::saveDataForTerminateServer",FALSE)!=eCADR_ISOK)
		return	FALSE;

	if (m_iXPos	!=	-1)
	{
		m_iXPos	=	m_posLastMoved.x;
		m_iYPos	=	m_posLastMoved.y;
	}
	
	m_dwSaveCount++;	//	DB저장 데이터와 동기화 시키기 위해 카운트를 여기서 증가 시킨다.

	GDMSG_LOGOUTAVATAR	packet;

	packet.base.set(sizeof(GDMSG_LOGOUTAVATAR),dGDMSG_LOGOUTAVATAR);
	packet.iActorSerialInServer	=	0xffffffff;
	memcpy(&packet.pData,this,sizeof(CPlayerSaveDataForServerDefine));

	if(m_wSerialInField	!=	0xffff	&&	m_lpField && m_lpField->isSpecialField())
	{
		packet.pData.m_wCurrentField	=	m_wLastVillage;
		packet.pData.m_iXPos			=	-1;
		packet.pData.m_iYPos			=	-1;
	}

	g_socketDBC.SendPacket((char *)&packet,((cMSG_BASE_TYPE *)&packet)->wSize);

	sendSaveLog();

	return	TRUE;
}