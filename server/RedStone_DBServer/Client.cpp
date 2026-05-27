// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////
#include <winsock2.h>
#include "Client.h"
#include "cPACKET_DBSERVER.h"
#include "cSRVUTIL.h"
#include "CSaveData.h"

#define dGAPLIMIT		180000

static	int		basesize = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient(int	buffersize)
{
	serial			=	0xffffffff;
	isSending		=	FALSE;
	MAX_LENGTH		=	buffersize;
	isUsed			=	FALSE;
	isClose			=	TRUE;
	sock			=	INVALID_SOCKET;
	timegap			=	0;
	sendpacket_size =	0;
	enServerType		=	eSRVTYPE_GAME;

	rBuffer = new CPacketBuffer(buffersize);
	sBuffer = new CPacketBuffer(buffersize);

	InitializeCriticalSection(&csUSER);
}

CClient::~CClient()
{
	delete rBuffer;
	delete sBuffer;
	DeleteCriticalSection(&csUSER);
	Kill();
}

void	CClient::CSReset()
{
	isSending = FALSE;
}

void	CClient::Open()
{
	UsedUser();
	rBuffer->Reset();
	sBuffer->Reset();
	isSending = FALSE;
	isUsed			=	TRUE;
	isClose			=	FALSE;
	timegap			=	timeGetTime();
	wUserCount		=	0;

	memset(&rOL.OL,0,sizeof(OVERLAPPED));
	rOL.buf.buf = rOL.Buffer;
	rOL.buf.len = dMAX_PACKET_LENGTH_SERVER;

	memset(&sOL.OL,0,sizeof(OVERLAPPED));
	sOL.buf.buf = sOL.Buffer;
	sOL.buf.len = dMAX_PACKET_LENGTH_SERVER;
	sOL.IOState = 0;
	NotUsedUser();
}

void	CClient::PreClose()
{
	isClose	= TRUE;
}

void	CClient::Close()
{
	if (INVALID_SOCKET != sock) 
	{ 
		closesocket(sock); sock = NULL;
	} 
	isUsed			=	FALSE;
	isClose			=	TRUE;
	timegap			=	0;
	serial			=	0xffffffff;
	enServerType		=	eSRVTYPE_GAME;
}

void	CClient::Kill()
{
	if (INVALID_SOCKET != sock) 
	{ 
		closesocket(sock); sock = NULL;
	} 

	isUsed			=	FALSE;
	isClose			=	TRUE;
	serial			=	0xffffffff;
	enServerType		=	eSRVTYPE_GAME;
}

void		CClient::SetIsUsed(BOOL isused)
{	UsedUser();
	isUsed = isused;
	isClose = isused^1;
	NotUsedUser();
}

void
CClient::SetID(char *nid)
{	
	memcpy(&id,nid,dID_LENGTH);	
}

void
CClient::SetUserCount(WORD count)
{
	UsedUser();

	wUserCount	= count;

	NotUsedUser();
}

void
CClient::SetIP(char *_lpstrIP)
{
	strcpy(m_strIP,_lpstrIP);
}

BOOL
CClient::isWaste()
{
	int iTimes = timeGetTime() - GetTimeGap();
	if( (abs(iTimes) > dGAPLIMIT+1000) && (abs(iTimes) < dGAPLIMIT*2) )
	{
//		_log("%d> %d &&  %d< %d",abs(timeGetTime() - GetTimeGap()) , dGAPLIMIT+1000, abs(timeGetTime() - GetTimeGap()) , dGAPLIMIT*2);
		return TRUE;
	}
	return FALSE;
}

int
CClient::AddRecvPacket(char *data,int size)
{	
	timegap = timeGetTime();
	return rBuffer->AddData(data,size);
}

int
CClient::AddRecvPacket(int size)
{
	return AddRecvPacket(rOL.buf.buf,size);
}

int
CClient::GetRecvPacket(char * recvpacket)
{	
	int	ret = rBuffer->GetData(recvpacket,1);

	if	(*(WORD *)recvpacket ==0 || *(WORD *)recvpacket>=0xcccc)
	{
//		_log("Error...");
		return 0;
	}

	return ret;
}

int
CClient::AddSendPacket(char *data,int size)
{	
	if	(*(WORD *)data ==0 || *(WORD *)data>=0xcccc)
	{
//		_log("Error...");
		return 0;
	}

	return sBuffer->AddData(data,size);
}

int
CClient::CompleteSendPacket(int size)
{
	if	(sendpacket_size<size)	//	하나의 데이터가 완전하게 보내지지 않았다면..
	{
		char	tmpPacket[dMAX_PACKET_LENGTH];
		
		memset(tmpPacket,0,dMAX_PACKET_LENGTH);
		memcpy(tmpPacket,sendpacket+size,sendpacket_size-size);
		memset(sendpacket,0,dMAX_PACKET_LENGTH);
		sendpacket_size -= size;
		memcpy(sendpacket,tmpPacket,sendpacket_size);
		return sendpacket_size;
	}

	sBuffer->Next();
	return 0;
}

//****************************************************************************************
//	Class	Client Manager
//****************************************************************************************
CClientManager::CClientManager()
{	
	InitializeCriticalSection(&csDeque);
	Init();
	basesize =			sizeof(cMSG_BASE_TYPE_FORDB);
}

CClientManager::~CClientManager()
{
	DeleteCriticalSection(&csDeque);
}

CClientManager &
CClientManager::GetInstance()
{
	static CClientManager cmanager;

	return cmanager;
}

void
CClientManager::Init()
{	
	CCritical CS(&csDeque);

	wNext = 0;
	wCount = 0;

	for(int i=0;i<dDB_MAX_USER_COUNT;i++)
		User[i].serial = 0xffffffff;
}

void
CClientManager::End()
{
	CCritical CS(&csDeque);

	wNext = 0;
	wCount = 0;

	for(int i=0;i<dDB_MAX_USER_COUNT;i++)
		User[i].serial = 0xffffffff;
}

WORD
CClientManager::Next()
{
	for(int i = 0;i<dDB_MAX_USER_COUNT;i++)
		if(User[i].GetIsClose() || User[i].serial == 0xffffffff)
			break;

	if	(i >= dDB_MAX_USER_COUNT)
		wNext = 0xffff;
	else
		wNext = i;

	return wNext;
}

int		CClientManager::Add()
{	
	CCritical CS(&csDeque);

	int	iResult = 0xffff;

	if	(wNext == 0xffff)
	{
		return -1;
	}

//	User[wNext].Open();
	User[wNext].SetIsUsed(TRUE);		//	사용을 위해 일단 이것만 설정해둔다.
	User[wNext].SetSerial(wNext);

	iResult = wNext;

	wCount++;

	Next();

	return iResult;						//	마지막에 있는 값을 돌려준다.
}

void
CClientManager::Del(int idx)
{	
	CCritical CS(&csDeque);

	User[idx].Close();
	wNext = idx;
	wCount--;

	if	(wCount>=dDB_MAX_USER_COUNT)	
	{
		_logf(" ERROR ClientManager Delete wCount = %d",wCount);
		wCount = 0;
	}
}

CClient*
CClientManager::GetWorldSrvPnt()
{
	CCritical CS(&csDeque);
	
	CClient * pClient = NULL;

	for(int i=0;i<dDB_MAX_USER_COUNT;i++)
	{	
		if	(User[i].GetSrvType() == CClient::eSRVTYPE_WORLD)
		{	
			pClient = &User[i];	
			break;	
		}
	}

	return pClient;
}

CClient
*CClientManager::GetClientPnt(char *id)	//	ID로 CClient 포인터 얻기
{
	CCritical CS(&csDeque);
	
	CClient * pClient = NULL;

	for(int i=0;i<dDB_MAX_USER_COUNT;i++)
	{	
		if	(STRICMP(id,User[i].GetID())==0 && User[i].GetSocket()>0)
		{	
			pClient = &User[i];	
			break;	
		}
	}

	return pClient;
}

CClient*
CClientManager::GetClientPnt(int idx,BOOL	isSerial)	//	serial로 CClient 포인터 얻기
{	
	CCritical CS(&csDeque);

	CClient * pClient = NULL;

	if	(User[idx].GetIsUsed() && User[idx].serial<0xffffffff)
		pClient = &User[idx];				

	return pClient;
}

//****************************************************************************************
//	Class	Client Data Manager
//****************************************************************************************

//CClientDataManager	g_cdm;	//	Client Data Manager

//BOOL	CClientDataManager::s_bIsExist	=	FALSE;
CClientDataManager*	g_lpCdm	=	NULL;	//	Client Data Manager

BOOL
CClientDataManager::Init()
{
	InitializeCriticalSection(&m_csDeque);

	return TRUE;
}

void
CClientDataManager::End()
{	
	DeleteCriticalSection(&m_csDeque);
}

CClientData*
CClientDataManager::getClient(char *_lpstrString,BOOL _bIsByName)
{
	DWORD	hash = GetHashCode((BYTE *)_lpstrString);
	if (_bIsByName)
	{
		for (int i=0,iCount=0;i<dDATA_MAX_COUNT && iCount < m_iCount;i++)
		{
			if (m_aStack[i].wSerial == 0xffff) continue;

			//	STRICMP
			if (hash ==m_aStack[i].hashNameCode &&  STRICMP(m_aStack[i].pData.m_strName,_lpstrString)==0)	return	&m_aStack[i];	

			iCount++;
		}
	}else
	{
		for (int i=0,iCount=0;i<dDATA_MAX_COUNT && iCount < m_iCount;i++)
		{
			if (m_aStack[i].wSerial == 0xffff) continue;

			if (hash ==m_aStack[i].hashIdCode && STRICMP(m_aStack[i].pData.m_strId,_lpstrString)==0)		return	&m_aStack[i];

			iCount++;
		}
	}

	return	NULL;
}	//	CClientDataManager::getClient(char *_lpstrString,BOOL _bIsByName)

WORD
CClientDataManager::Add(cPLAYER_DATA *_lpData,int _iOperaterLevel)
{	
	if (m_iCount	>=	dDATA_MAX_COUNT)	
		return	0xffff;

	lock();

//	화일로 복구
	{
		cFILE	file;
		char	strFileName[256];

		sprintf(strFileName,"save/%s[%d].sav",_lpData->m_strId,_lpData->m_wAvatarIndex);

		if (file.Open(strFileName,"rb"))
		{
			cPLAYER_DATA	data;

			file.Read(&data,sizeof(cPLAYER_DATA));
			file.Close();

			if	(data.m_dwSaveCount	> _lpData->m_dwSaveCount && STRICMP(data.m_strName,_lpData->m_strName) == 0&& STRICMP(data.m_strId,_lpData->m_strId) == 0)
			{
				memcpy(_lpData,&data,sizeof(cPLAYER_DATA));		//	그럼 복사 -o-
#ifdef _FOR_KOREA
				_log("화일 데이터가 있어서 복구 [%s/%s]",_lpData->m_strId,_lpData->m_strName);
#endif
			}

			DeleteFile(strFileName);
		}
	}

	CClientData	*lpFailedData	=	getFailedDataWithNoLock(_lpData->m_strName,TRUE);

	if	(lpFailedData)	//	저장 실패한 데이터가 있다.
	{
		if	(lpFailedData->pData.m_dwSaveCount	>	_lpData->m_dwSaveCount)	//	카운트가 더 크네?
		{
			memcpy(_lpData,&lpFailedData->pData,sizeof(cPLAYER_DATA));		//	그럼 복사 -o-
#ifdef _FOR_KOREA
			_log("저장 실패 데이터가 있어서 복구  [%s/%s]",_lpData->m_strId,_lpData->m_strName);
#endif
		}

		removeFailedDataWithNoLock(lpFailedData);
	}

	CClientData	*lpClient	=	&m_aStack[m_iRookie];

	lpClient->wSerial		=	m_iRookie;
	lpClient->isSend		=	FALSE;
	lpClient->wWorldSerial	=	0;
	lpClient->wOper			=	_iOperaterLevel;
	lpClient->dwTimer		=	timeGetTime();

	memcpy(&lpClient->pData,_lpData,sizeof(cPLAYER_DATA));
	lpClient->makeHash();

	for (int i=m_iRookie+1;i<dDATA_MAX_COUNT;i++)
		if (m_aStack[i].wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iCount++;

	unlock();

	return lpClient->wSerial;
}	//	CClientDataManager::Add(cPLAYER_DATA *_lpData)

//
//	Id와 아바타 인덱스를 참조해서 제거
void
CClientDataManager::removeByIdAndIndex(char *_lpstrId,int _iAvatarIndex)
{
	lock();

	CClientData	*lpClient		=	getClient(_lpstrId,FALSE);
	CClientData	*lpFailedData	=	getFailedDataWithNoLock(_lpstrId,FALSE);

	if (lpClient)
	{
		if (lpClient->pData.m_wAvatarIndex	==	_iAvatarIndex)
		{
			if (lpClient->wSerial	<	m_iRookie)
				m_iRookie	=	lpClient->wSerial;

			m_iCount--;

			lpClient->close();
		}
	}

	if (lpFailedData)
		if (lpFailedData->pData.m_wAvatarIndex	==	_iAvatarIndex)
			removeFailedDataWithNoLock(lpFailedData);

	unlock();
}	//	CClientDataManager::removeByIdAndIndex(char *_lpstrId,int _iAvatarIndex)

BOOL
CClientDataManager::Del(int _iIndex)
{
	if (_iIndex < 0 || _iIndex >= dDATA_MAX_COUNT)
	{
#ifdef _FOR_KOREA
		_log("CClientDataManager::Del '_iIndex < 0 || _iIndex >= dDATA_MAX_COUNT'");
#endif
		return	FALSE;
	}

	lock();

	CClientData	*lpClient	=	&m_aStack[_iIndex];

	if	(lpClient->wSerial	==	0xffff)
	{
		unlock();

		return FALSE;
	}

	lpClient->close();

	if (_iIndex < m_iRookie)
		m_iRookie	=	_iIndex;

	m_iCount--;

	unlock();

	return	TRUE;
}

//
//	제거
BOOL
CClientDataManager::remove(char *_lpstrString,BOOL _bIsByName)
{
	CClientData *lpClient;

	lock();

	lpClient	=	getClient(_lpstrString,_bIsByName);

	if	(!lpClient)
	{
		unlock();

		return	FALSE;
	}

	if (lpClient->wSerial < m_iRookie)
		m_iRookie	=	lpClient->wSerial;

	lpClient->close();

	m_iCount--;

	unlock();

	return TRUE;
}	//	CClientDataManager::remove(char *_lpstrString,BOOL _bIsByName)

//
//	제거
BOOL
CClientDataManager::removeByIdAndSave(char *_lpstrId,cPLAYER_DATA * _pData)
{
	CClientData *lpClient;

	lock();

	lpClient	=	getClient(_lpstrId,FALSE);

	if	(!lpClient)
	{
		unlock();

		return	FALSE;
	}
#ifdef _FOR_KOREA
	_log("강제 제거 시도 하면서 기존 데이터 저장");
#endif
	
	if( _pData && (_pData->m_dwSaveCount < lpClient->pData.m_dwSaveCount) )
	{
		_log(" removeByIdAndSave : savecount [%d < %d] lev [%d , %d] exp [%d, %d]",_pData->m_dwSaveCount, lpClient->pData.m_dwSaveCount,_pData->m_iLevel, lpClient->pData.m_iLevel, _pData->m_iExperience, lpClient->pData.m_iExperience);

		memcpy(_pData, &lpClient->pData, sizeof(cPLAYER_DATA));
	}
	int	iSaveResult	=	g_dsManager.save(&lpClient->pData);

	if (g_dsManager.isRequireResaveResult(iSaveResult))
		addFailedData(&lpClient->pData,TRUE);

	if (lpClient->wSerial < m_iRookie)
		m_iRookie	=	lpClient->wSerial;

	lpClient->close();

	m_iCount--;

	unlock();

	return TRUE;
}	//	CClientDataManager::removeAndSave(char *_lpstrId)

//
//	이거 위험 이렇게 얻어가면 문제의 여지가 있음
CClientData*
CClientDataManager::GetClientPnt(char *_lpstrString,BOOL _bIsByName)
{
	CClientData *lpClient;

	lock();

	lpClient	=	getClient(_lpstrString,_bIsByName);

	unlock();

	return	lpClient;
}	//	CClientDataManager::GetClientPnt(char *_lpstrString,BOOL _bIsByName)

CClientData*
CClientDataManager::GetClientPnt(int _iIndex)
{	
	if (_iIndex < 0 || _iIndex >= dDATA_MAX_COUNT)
	{
#ifdef _FOR_KOREA
		_log("CClientDataManager::GetClientPnt '_iIndex < 0 || _iIndex >= dDATA_MAX_COUNT'");
#endif
		return	NULL;
	}

	return	&m_aStack[_iIndex];
}	//	CClientDataManager::GetClientPnt(int _iIndex)

#define	dSHARED_VALUE			100
#define	dLIMIT_TIME_TO_REPLY	(2*60*1000)	//	2분

//
//	너무 늦은 클라이언트들 제거
void
CClientDataManager::update()
{
	lock();

	active();

//	DB 세이브 실패한 넘들 처리
	if (m_iFailedDataCount)
	{
		int		i,iCount;

		for (i=0,iCount=0;i<dMAX_FAILED_DATA_COUNT_TO_SAVE_DB && iCount < m_iFailedDataCount ; i++)
		{
			CClientData	*lpClient	=	&m_aFailedStack[i];

			if (lpClient->wSerial	==	0xffff)	continue;

			iCount++;
			lpClient->m_wResaveCount++;
			
#ifdef _FOR_KOREA
			_log("DB 저장 실패 데이터 다시 저장 시도");
#endif
			int	iSaveResult	=	g_dsManager.save(&lpClient->pData);

			if (iSaveResult == eSAVE_SUCCESS_DB || iSaveResult == eSAVE_FAIL_LITTLECOUNT || iSaveResult == eSAVE_DB_INCORRECT_DATA)
				lpClient->close();
			else
			if (lpClient->m_wResaveCount	>=	10)
			{
				char	strFileName[256];

				cFILE	file;

				sprintf(strFileName,"save/%s[%d].sav",lpClient->pData.m_strId,lpClient->pData.m_wAvatarIndex);

				file.Open(strFileName,"wb");
				file.Write(&lpClient->pData,sizeof(cPLAYER_DATA));
				file.Close();
#ifdef _FOR_KOREA
				_log("DB 저장 실패 데이터 10번 이상 저장 실패!!");
				_log("%s에 [%s/%s] 데이터 저장",strFileName,lpClient->pData.m_strId,lpClient->pData.m_strName);
#endif

				lpClient->close();
			}
		}

		m_iFailedDataCount	=	0;
		m_iFailedDataRookie	=	-1;

		for (i=0;i<dMAX_FAILED_DATA_COUNT_TO_SAVE_DB; i++)
		{
			CClientData	*lpClient	=	&m_aFailedStack[i];

			if (lpClient->wSerial	==	0xffff)
			{
				if (m_iFailedDataRookie	==	-1)	m_iFailedDataRookie	=	i;

				continue;
			}

			m_iFailedDataCount++;
		}
	}

//	접속후 아직 응답 안 보낸애들 체크해서 시간이 넘 지났으면 짤라 버린다.
	{
		int	iValue		=	m_dwUpdateCounter%dSHARED_VALUE;
		int	iBeginIndex	=	dDATA_MAX_COUNT/dSHARED_VALUE*iValue;
		int	iEndIndex	=	dDATA_MAX_COUNT/dSHARED_VALUE*(iValue+1)-1;

		iEndIndex		=	min(dDATA_MAX_COUNT-1,iEndIndex);

		for(int	iIndex=iBeginIndex;iIndex <= iEndIndex;iIndex++)
		{
			CClientData	*lpClient	=	&m_aStack[iBeginIndex];

			if (lpClient->wSerial	==	0xffff	)	continue;
			if (lpClient->isSend				)	continue;

			if (lpClient->dwTimer > timeGetTime() || lpClient->dwTimer + dLIMIT_TIME_TO_REPLY < timeGetTime())	//	2분이 지났거나 한바퀴 돌았다.
			{
//	삭제
				lpClient->close();

				if (iBeginIndex	< m_iRookie)
					m_iRookie	=	iBeginIndex;

				m_iCount--;
			}
		}

		m_dwUpdateCounter++;
	}

	unlock();
}	//	CClientDataManager::update()


//
//	시스템 문제로 DB 저장에 실패 했다.
BOOL
CClientDataManager::addFailedData(cPLAYER_DATA * _lpData,BOOL _bIsAlreadyLocked)
{
	if	(m_iFailedDataCount	>=	dMAX_FAILED_DATA_COUNT_TO_SAVE_DB)	return	FALSE;

	if (FALSE	==	_bIsAlreadyLocked)	lock();

	int			i,iCount;
	CClientData	*lpClient;

	for (i=0,iCount=0;i<dMAX_FAILED_DATA_COUNT_TO_SAVE_DB && iCount < m_iFailedDataCount ; i++)
	{
		CClientData	*lpClient	=	&m_aFailedStack[i];

		if (lpClient->wSerial	==	0xffff)	continue;

		iCount++;

		if (STRICMP(lpClient->pData.m_strName,_lpData->m_strName) == 0)
		{
			if (lpClient->pData.m_dwSaveCount <= _lpData->m_dwSaveCount)
			{
				memcpy(&lpClient->pData,_lpData,sizeof(cPLAYER_DATA));
				lpClient->makeHash();

				goto	success_label;
			}
		}
	}

	lpClient				=	&m_aFailedStack[m_iFailedDataRookie];
	lpClient->wSerial		=	m_iFailedDataRookie;
	lpClient->m_wResaveCount=	0;

	memcpy(&lpClient->pData,_lpData,sizeof(cPLAYER_DATA));
	lpClient->makeHash();
	m_iFailedDataCount++;

	for (i=m_iFailedDataRookie+1;i<dMAX_FAILED_DATA_COUNT_TO_SAVE_DB;i++)
		if (m_aFailedStack[i].wSerial	==	0xffff)
		{
			m_iFailedDataRookie	=	i;
			break;
		}

success_label:

	if (FALSE	==	_bIsAlreadyLocked)	unlock();

	return	TRUE;
}	//	CClientDataManager::addFailedData(cPLAYER_DATA * _lpData,BOOL _bIsAlreadyLocked)

//
//	시스템 문제로 DB 저장에 실패 했다.
CClientData*
CClientDataManager::getFailedDataWithNoLock(char *_lpstrString,BOOL _bIsByName)
{
	int		i,iCount;
	DWORD	hash = GetHashCode((BYTE *)_lpstrString);
	if	(m_iFailedDataCount	<=	0)	return	NULL;

	if (_bIsByName)
	{
		for (i=0,iCount=0;i<dMAX_FAILED_DATA_COUNT_TO_SAVE_DB && iCount < m_iFailedDataCount ; i++)
		{
			CClientData	*lpClient	=	&m_aFailedStack[i];
			if (lpClient->wSerial	==	0xffff)	continue;
			iCount++;
			if (hash==lpClient->hashNameCode && STRICMP(lpClient->pData.m_strName,_lpstrString) == 0)
				return	lpClient;
		}
	}
	else
	{
		for (i=0,iCount=0;i<dMAX_FAILED_DATA_COUNT_TO_SAVE_DB && iCount < m_iFailedDataCount ; i++)
		{
			CClientData	*lpClient	=	&m_aFailedStack[i];
			if (lpClient->wSerial	==	0xffff)	continue;
			iCount++;
			if (hash==lpClient->hashIdCode && STRICMP(lpClient->pData.m_strId,_lpstrString) == 0)
				return	lpClient;
		}
	}

	return	NULL;
}	//	CClientDataManager::getFailedDataWithNoLock(char *_lpstrString,BOOL _bIsByName)

//
//	이름으로 유저를 찾아서 데이터를 업데이트 한다.
int
CClientDataManager::updatePlayerDataByName(char *_lpstrName,cPLAYER_DATA *_lpData)
{
	lock();

	int		iResult			=	eSAVE_SUCCESS_SAVEFILE;

	CClientData	*lpClient	=	getClient(_lpstrName,TRUE);

	if (lpClient	==	NULL)
	{
		iResult		=	eSAVE_CAN_NOT_FIND_LOGIN_USER;

		goto	end_label;
	}
	
	if (lpClient->pData.m_dwSaveCount	>=	_lpData->m_dwSaveCount)
	{
		if(lpClient->pData.m_dwSaveCount - _lpData->m_dwSaveCount > 10)
			iResult		=	eSAVE_MEMORY_FAIL_LITTLECOUNT2;
		else
			iResult		=	eSAVE_MEMORY_FAIL_LITTLECOUNT;

		goto	end_label;
	}

	memcpy(&lpClient->pData,_lpData,sizeof(cPLAYER_DATA));
	
end_label:

	unlock();

	return	iResult;
}	//	CClientDataManager::updatePlayerDataByName(char *_lpstrName,cPLAYER_DATA *_lpData)

//
//	실패한 데이터 제거
void
CClientDataManager::removeFailedDataWithNoLock(CClientData *_lpClient)
{
	if (_lpClient->wSerial	<	m_iFailedDataRookie)
		m_iFailedDataRookie	=	_lpClient->wSerial;

	m_iFailedDataCount--;

	_lpClient->close();
}	//	CClientDataManager::removeFailedDataWithNoLock(CClientData *_lpClient)

//
//	아이디로 유저를 찾아서 데이터를 업데이트 한다.
void
CClientDataManager::saveMemoryDataToDBById(char *_lpstrId)
{
	lock();

	CClientData	*lpFailedData	=	getFailedDataWithNoLock(_lpstrId,FALSE);
	CClientData	*lpClient		=	getClient(_lpstrId,FALSE);
	int			iSaveResult;

	if (lpFailedData)
	{
		iSaveResult		=	g_dsManager.save(&lpFailedData->pData);

		if (iSaveResult == eSAVE_SUCCESS_DB || iSaveResult == eSAVE_FAIL_LITTLECOUNT || iSaveResult == eSAVE_DB_INCORRECT_DATA)
			removeFailedDataWithNoLock(lpFailedData);
	}

	if (lpClient)
	{
		iSaveResult		=	g_dsManager.save(&lpClient->pData);

		if (g_dsManager.isRequireResaveResult(iSaveResult))
			addFailedData(&lpClient->pData);
	}

	unlock();
}	//	CClientDataManager::saveMemoryDataToDBById(char *_lpstrId)

//
//
void
CClientDataManager::saveAllDataToDB()
{
	lock();
	int	i,iCount,iResult;

//	화일에 저장해둔 데이터 복구
	{
		cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.sav","save");
		if (lpFiles)
		{
			cSTRING		*lpFile		=	lpFiles->Pick();
			int			iFileCount	=	lpFiles->Count;
			char		strFileName[128];
			cFILE		file;
#ifdef _FOR_KOREA
			_log("시작복구 by 화일 시작 ------------- ");
#endif
			for (i=0;i<iFileCount;i++)
			{
				sprintf(strFileName,"save/%s",lpFile->String);

				lpFile	=	lpFile->pNEXT;

				if (!file.Open(strFileName,"rb"))	continue;

				cPLAYER_DATA	data;

				file.Read(&data,sizeof(cPLAYER_DATA));
				file.Close();

				DeleteFile(strFileName);

				iResult	=	g_dsManager.save(&data);

				if (g_dsManager.isRequireResaveResult(iResult))
					addFailedData(&data,TRUE);

				Sleep(1);
			}
#ifdef _FOR_KOREA
			_log("시작복구 by 화일 종료 ------------- ");
#endif
			KILL(lpFiles);
		}
	}
	
	for (i=0,iCount=0;i<dDATA_MAX_COUNT && iCount < m_iCount ; i++)
	{
		CClientData	*lpClient	=	&m_aStack[i];

		if (lpClient->wSerial	==	0xffff)	continue;

		iCount++;
#ifdef _FOR_KOREA
		_log("시작복구 by 접속자");
#endif
		iResult	=	g_dsManager.save(&lpClient->pData);

		if (g_dsManager.isRequireResaveResult(iResult))
			addFailedData(&lpClient->pData,TRUE);

		lpClient->close();

		Sleep(1);
	}

	m_iCount	=	0;
	m_iRookie	=	0;

////////////////////////	저장 실패했던 데이터들 처리
	for (i=0,iCount=0;i<dMAX_FAILED_DATA_COUNT_TO_SAVE_DB && iCount < m_iFailedDataCount ; i++)
	{
		CClientData	*lpClient	=	&m_aFailedStack[i];

		if (lpClient->wSerial	==	0xffff)	continue;

		iCount++;
#ifdef _FOR_KOREA
		_log("시작복구 by Failed data");
#endif
		iResult	=	g_dsManager.save(&lpClient->pData);

		if (iResult == eSAVE_SUCCESS_DB || iResult == eSAVE_FAIL_LITTLECOUNT || iResult == eSAVE_DB_INCORRECT_DATA)
			lpClient->close();

		Sleep(1);
	}

	m_iFailedDataCount	=	0;
	m_iFailedDataRookie	=	-1;

	for (i=0;i<dMAX_FAILED_DATA_COUNT_TO_SAVE_DB; i++)
	{
		CClientData	*lpClient	=	&m_aFailedStack[i];
		
		if (lpClient->wSerial	==	0xffff)
		{
			if (m_iFailedDataRookie	==	-1)	m_iFailedDataRookie	=	i;

			continue;
		}

		m_iFailedDataCount++;
	}

	unlock();
}	//	CClientDataManager::saveAllDataToDB()


//****************************************************************************************
//	Class	Index Manager
//****************************************************************************************

CIndexManager	g_Im;		//	Index Manager
//	CINDEX_TABLE		idxTable[dMAX_INDEX_COUNT];
//	DWORD				dwCount;

CIndexManager::CIndexManager()
{
	GetCurrentDirectory(512,strDirectory);
	dwCount = 0;
	InitializeCriticalSection(&cs);
}

CIndexManager::~CIndexManager()
{
	DeleteCriticalSection(&cs);
}

/*
BOOL	CIndexManager::reset()
{
	BOOL	bResult = TRUE;
	char	strTmpDir[512];
	GetCurrentDirectory(512,strTmpDir);
	SetCurrentDirectory(strDirectory);
	//	기존에 내용이 있으면 한번 저장할까나... 

	//	파일에서 불러온다.
	dwCount = 0;
	FILE * fp = fopen(dINDEX_FILENAME,"rb");
	if(fp)
	{
		fread(&dwCount,sizeof(DWORD),1,fp);
		if(dwCount<=dMAX_INDEX_COUNT)
			fread(idxTable,sizeof(CINDEX_TABLE)*dwCount,1,fp);
		else{
			_log("Error Index Table Read : 총 [%d]개가 저장되어 있단다.",idxTable);
			bResult = FALSE;
		}
		fclose(fp);
	}
	SetCurrentDirectory(strTmpDir);
	return bResult;
}

void	CIndexManager::close()
{
	save();
}

BOOL	CIndexManager::save()
{
	CCritical		CS(&cs);

	char	strTmpDir[512];
	GetCurrentDirectory(512,strTmpDir);
	SetCurrentDirectory(strDirectory);

	DWORD	dwRealCount = 0;
	int i = 0;
	for(i = 0 ; i<dMAX_INDEX_COUNT ; i++)
		if(idxTable[i].dwIndex!=0x7fffffff)	dwRealCount++;

	//	기존 파일을 백업하고..
	char	strBackupFile[512];
	sprintf(strBackupFile,"%d_%d_indexinfo.dat",cSRVUTIL::GetTime(),cSRVUTIL::GetTimes());
	CopyFile(dINDEX_FILENAME,strBackupFile,FALSE);

	//	새로운 내용을 기록한다.
	FILE * fp = fopen(dINDEX_FILENAME,"wb");
	if(fp)
	{
		fwrite(&dwRealCount,sizeof(DWORD),1,fp);
		for(i = 0 ; i<dMAX_INDEX_COUNT ; i++)
			if(idxTable[i].dwIndex!=0x7fffffff)
				fwrite(&idxTable[i],sizeof(CINDEX_TABLE),1,fp);
		fclose(fp);
	}else{
		_log("Error Index Table Write : Save Error");
		SetCurrentDirectory(strTmpDir);
		return FALSE;
	}
	SetCurrentDirectory(strTmpDir);
	return TRUE;
}
*/
BOOL	CIndexManager::addFirst(int _iIndex, char * _strName, char * _strId,WORD _avaIndex)
{
	CCritical		CS(&cs);
	int i = 0;
	//	중복되어 있나 확인해본다.
/*	for(i = 0 ; i<dMAX_INDEX_COUNT ; i++)
		if(STRICMP(_strName,idxTable[i].strName)==0)
		{
			if(STRICMP(_strId,idxTable[i].strId)==0)	//	같은 경우인데 인덱스가 다른경우
			{
				printf("Caution Add : [%s] [%s] [%d]->[%d]",_strId,_strName,g_Im.idxTable[i].iIndex,_iIndex);
				idxTable[i].iIndex = _iIndex;
				return TRUE;
			}else
				idxTable[i].iIndex = 0x7fffffff;
		}
*/
//	for(i = 0 ; i<dMAX_INDEX_COUNT ; i++)
//		if(idxTable[i].iIndex==0x7fffffff)
		{
			idxTable[dwCount].iIndex = _iIndex;
			idxTable[dwCount].hashIdCode = GetHashCode((BYTE *)_strId);
			idxTable[dwCount].hashNameCode = GetHashCode((BYTE *)_strName);
			strcpy(idxTable[dwCount].strId,_strId);
			strcpy(idxTable[dwCount].strName,_strName);
			idxTable[dwCount].wAvaIndex = _avaIndex;
			++dwCount;
//			_log(" Add First IndexManager Count %d",dwCount);
//			_log("add first 2 [%d] [%s] ,[%s]",_iIndex,_strId,_strName);
			return TRUE;
		}
	return FALSE;
}

DWORD	CIndexManager::add(int _iIndex, char * _strName, char * _strId, WORD _avaIdx)
{
	CCritical		CS(&cs);
	int i = 0;
	unsigned long hashIdCode = GetHashCode((BYTE *)_strId);
	unsigned long hashNameCode = GetHashCode((BYTE *)_strName);
	//	중복되어 있나 확인해본다.
	for(i = 0 ; i<(int)dwCount ; i++)
		if(hashNameCode==idxTable[i].hashNameCode &&  STRICMP(_strName,idxTable[i].strName)==0)
		{
			if(STRICMP(_strId,idxTable[i].strId)==0)	//	같은 경우인데 인덱스가 다른경우
			{
				_log("Caution Add : [%s] [%s] =? [%s] [%d]->[%d]",_strId,_strName,idxTable[i].strName,g_Im.idxTable[i].iIndex,_iIndex);
				idxTable[i].iIndex = _iIndex;
				return i;
			}else{
				idxTable[i].iIndex = 0x7fffffff;
			}
		}

	idxTable[dwCount].iIndex = _iIndex;
	idxTable[dwCount].hashIdCode = hashIdCode;
	idxTable[dwCount].hashNameCode = hashNameCode;
	strcpy(idxTable[dwCount].strId,_strId);
	strcpy(idxTable[dwCount].strName,_strName);
	idxTable[dwCount].wAvaIndex = _avaIdx;
	++dwCount;
	_log("add [%d] [%s] ,[%s], max count %d",_iIndex,_strId,_strName,dwCount);
	return dwCount-1;
}

void	CIndexManager::removeAll(char * _strId)
{
	CCritical		CS(&cs);
	unsigned long hashIdCode = GetHashCode((BYTE *)_strId);
	for(int i = 0 ; i<(int)dwCount ; i++)
		if(hashIdCode==idxTable[i].hashIdCode && STRICMP(_strId,idxTable[i].strId)==0)
		{
			idxTable[i].reset();
		}
	return ;
}

BOOL	CIndexManager::remove(int _iIndex)
{
	CCritical		CS(&cs);
	for(int i = 0 ; i<(int)dwCount ; i++)
		if(_iIndex == idxTable[i].iIndex)
		{
			_logf("_iIndex remove [%d] [%s] [%s] [%d]",idxTable[i].iIndex,idxTable[i].strId,idxTable[i].strName,idxTable[i].wAvaIndex);
			idxTable[i].reset();
			return TRUE;
		}
	return FALSE;
}

BOOL	CIndexManager::remove(char * _strName, char * _strId)
{
	CCritical		CS(&cs);
	unsigned long hashIdCode = GetHashCode((BYTE *)_strId);
	unsigned long hashNameCode = GetHashCode((BYTE *)_strName);

	for(int i = 0 ; i<(int)dwCount ; i++)
		if(hashNameCode==idxTable[i].hashNameCode && STRICMP(_strName,idxTable[i].strName)==0 && STRICMP(_strId,idxTable[i].strId)==0)
		{
			_logf("idx remove [%d] [%s] [%s] [%d]",idxTable[i].iIndex,idxTable[i].strId,idxTable[i].strName,idxTable[i].wAvaIndex);
			idxTable[i].reset();
			return TRUE;
		}
	return FALSE;
}

DWORD	CIndexManager::getTableIndexByAvaIndex(char * _strId,int _avaIdx)
{
	unsigned long hashIdCode = GetHashCode((BYTE *)_strId);
	for(int i = 0 ; i<(int)dwCount ; i++)
		if(hashIdCode==idxTable[i].hashIdCode && STRICMP(_strId,idxTable[i].strId)==0)
		{
			if(idxTable[i].wAvaIndex == _avaIdx)
				return i;
		}
	return 0xffffffff;
}

int		CIndexManager::getBlankPos(char * _strId)
{
	DWORD	sTime = timeGetTime();
	int blankpos[dMAX_AVATAR_COUNT];
	memset(blankpos,0xff, sizeof(int)*dMAX_AVATAR_COUNT);
	
	unsigned long hashIdCode = GetHashCode((BYTE *)_strId);
	for(int i = 0 ; i<(int)dwCount ; i++)
	{
		if(hashIdCode== idxTable[i].hashIdCode && STRICMP(_strId,idxTable[i].strId)==0)
		{
			blankpos[idxTable[i].wAvaIndex] = idxTable[i].wAvaIndex;
		}
	}

	for( int b=0;b<dMAX_AVATAR_COUNT;b++)
	{
		if(blankpos[b] == -1)
		{
			DWORD eTime = timeGetTime() - sTime;
			_log("getBlankPos Succ: %d.%03d",eTime/1000,eTime%1000);
			return b;
		}
	}
	DWORD eTime = timeGetTime() - sTime;
	_log("getBlankPos Fail: %d.%03d",eTime/1000,eTime%1000);
	return -1;
}

void
CIndexManager::getIndexById(char * _strId, int * _iIndex)
{
	int _iCount = 0;
	unsigned long hashIdCode = GetHashCode((BYTE *)_strId);
	for(int i = 0 ; i<(int)dwCount ; i++)
		if(hashIdCode== idxTable[i].hashIdCode && STRICMP(_strId,idxTable[i].strId)==0)
		{
			_iIndex[_iCount] = idxTable[i].iIndex;
			_iCount++;
		}
}

CINDEX_TABLE * 
CIndexManager::getInfo(char * _strName, char * _strId)
{
	unsigned long hashNameCode = GetHashCode((BYTE *)_strName);
	if(strcmp(_strId,"")!=0)
	{
		for(int i = 0 ; i<(int)dwCount ; i++)
		{
			if(hashNameCode==idxTable[i].hashNameCode && STRICMP(_strName,idxTable[i].strName)==0)
			{
				if(STRICMP(_strId,idxTable[i].strId)==0)
					return &idxTable[i];
			}
		}
	}else
	{
		for(int i = 0 ; i<(int)dwCount ; i++)
		{
			if(hashNameCode==idxTable[i].hashNameCode && STRICMP(_strName,idxTable[i].strName)==0)
				return &idxTable[i];
		}
	}
	return NULL;
}

int
CIndexManager::getIndex(char * _strName, char * _strId)
{
	unsigned long hashNameCode = GetHashCode((BYTE *)_strName);
	if(strcmp(_strId,"")!=0)
	{
		for(int i = 0 ; i<(int)dwCount ; i++)
		{
			if(hashNameCode==idxTable[i].hashNameCode && STRICMP(_strName,idxTable[i].strName)==0)
			{
				if(STRICMP(_strId,idxTable[i].strId)==0)
					return idxTable[i].iIndex;
			}
		}
	}else
	{
		for(int i = 0 ; i<(int)dwCount ; i++)
		{
			if(hashNameCode==idxTable[i].hashNameCode && STRICMP(_strName,idxTable[i].strName)==0)
				return idxTable[i].iIndex;
		}
	}
	return 0x7fffffff;
}

BOOL
CIndexManager::changeIndex(int _iIndex, char * _strName, char * _strId)
{
	CCritical		CS(&cs);
	unsigned long hashNameCode = GetHashCode((BYTE *)_strName);
	if(strcmp(_strId,"")!=0)
	{
		for(int i = 0 ; i<(int)dwCount ; i++)
		{
			if(hashNameCode==idxTable[i].hashNameCode && STRICMP(_strName,idxTable[i].strName)==0 && STRICMP(_strId,idxTable[i].strId)==0)
			{
				if(STRICMP(_strId,idxTable[i].strId)==0)
				{
					idxTable[i].iIndex = _iIndex;
					return TRUE;
				}
			}
		}
	}else
	{
		for(int i = 0 ; i<(int)dwCount ; i++)
		{
			if(hashNameCode==idxTable[i].hashNameCode && STRICMP(_strName,idxTable[i].strName)==0 && STRICMP(_strId,idxTable[i].strId)==0)
			{
				idxTable[i].iIndex = _iIndex;
				return TRUE;
			}
		}
	}
	return FALSE;
}
