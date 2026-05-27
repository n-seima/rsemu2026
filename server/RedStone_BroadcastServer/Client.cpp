// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Client.h"
#include "cSRVUTIL.h"
#include "party.h"
#include "guild.h"
#include "field.h"

BOOL			g_bIsTestServer	=	FALSE;
CClientManager & UserManager	=	CClientManager::GetInstance();

CClient::CClient(int _iBuffersize)
{
	MAX_LENGTH = _iBuffersize;
	InitializeCriticalSection(&csUSER);
	InitializeCriticalSection(&csDebug);

	m_wSerial			=	0xffff;
	memset(m_strIp,0,dIP_SIZE);
	m_hSock			=	INVALID_SOCKET;
	memset(m_strId,0,dID_LENGTH);
	memset(m_strName,0,dNAME_LENGTH);
	m_isGameServer		=	0;
	m_dwHashIdCode		=	0;
	m_dwHashNameCode	=	0;
	m_wGuildSerial		=	0xffff;			
	m_wGuildRank		=	0xffff;					
	m_wOper				=	0;
	m_wGSSerial			=	0xffff;
	m_wPartySerial		=	0xffff;
	m_dwFriendsListUpdateGap = 0;
	m_wCurrentField		=	0xffff;

	memset(&m_stRecvOL.OL,0,sizeof(OVERLAPPED));
	m_stRecvOL.buf.buf = m_stRecvOL.Buffer;
	m_stRecvOL.buf.len = MAX_LENGTH;

	memset(&m_stSendOL.OL,0,sizeof(OVERLAPPED));
	m_stSendOL.buf.buf = m_stSendOL.Buffer;
	m_stSendOL.buf.len = MAX_LENGTH;
	m_stSendOL.IOState = 1;

	m_pclRecvBuffer = new CPacketBuffer(MAX_LENGTH);
	m_pclSendBuffer = new CPacketBuffer(MAX_LENGTH);
	m_pcSendpacket = new char[MAX_LENGTH];
	memset(m_pcSendpacket,0,MAX_LENGTH);
	m_wSendpacket_size	=	0;
	m_dwRecvSizePerSec = 0;
	m_dwSendSizePerSec = 0;
	m_dwLastRecvUpdateTime = 0;
	m_dwLastSendUpdateTime = 0;
	m_dwRecvTmpSizePerSec = 0;
	m_dwSendTmpSizePerSec = 0;
}

CClient::~CClient()
{	
	Kill();	
	delete [] m_pcSendpacket;
	delete m_pclRecvBuffer;
	delete m_pclSendBuffer;
	DeleteCriticalSection(&csDebug);
	DeleteCriticalSection(&csUSER);
}

void CClient::Open(WORD _wSerial,SOCKET _hSock,char * _strIp)
{
	CCritical cs(&csUSER);

	m_wSerial			=	_wSerial;
	_strIp[dIP_SIZE-1] = 0;
	strcpy(m_strIp,_strIp);
	m_hSock				=	_hSock;

	memset(m_strId,0,dID_LENGTH);
	memset(m_strName,0,dNAME_LENGTH);
	m_isGameServer		=	0;
	m_dwHashIdCode		=	0;
	m_dwHashNameCode	=	0;
	m_wGuildSerial		=	0xffff;			
	m_wGuildRank		=	0xffff;					
	m_wOper				=	0;
	m_wGSSerial			=	0xffff;
	m_wIsRequireAddToParty=	FALSE;

	memset(&m_stRecvOL.OL,0,sizeof(OVERLAPPED));
	m_stRecvOL.buf.buf = m_stRecvOL.Buffer;
	m_stRecvOL.buf.len = MAX_LENGTH;

	memset(&m_stSendOL.OL,0,sizeof(OVERLAPPED));
	m_stSendOL.buf.buf = m_stSendOL.Buffer;
	m_stSendOL.buf.len = MAX_LENGTH;
	m_stSendOL.IOState = 1;

	m_pclRecvBuffer->Reset();
	m_pclSendBuffer->Reset();
	memset(m_pcSendpacket,0,MAX_LENGTH);
	m_wSendpacket_size	=	0;
	m_dwSaveGap = 0;
	m_dwFriendsListUpdateGap = 0;

	m_dwRecvSizePerSec = 0;
	m_dwSendSizePerSec = 0;
	m_dwLastRecvUpdateTime = 0;
	m_dwLastSendUpdateTime = 0;
	m_dwRecvTmpSizePerSec = 0;
	m_dwSendTmpSizePerSec = 0;
}

void CClient::Kill()
{
	if	(INVALID_SOCKET != m_hSock) 
	{ 
		shutdown(m_hSock,SD_SEND);
		closesocket(m_hSock);
		m_hSock = INVALID_SOCKET;
	} 

	setPartySerial(0xffff);
	setGuildSerial(0xffff , 0xff);
	setFieldSerial(0xffff);
	setAvatarIndex(0xffff);

	m_wSerial			=	0xffff;
	memset(m_strId,0,dID_LENGTH);
	memset(m_strName,0,dNAME_LENGTH);
	m_isGameServer		=	0;
	m_dwHashIdCode		=	0;
	m_dwHashNameCode	=	0;
	m_wIsRequireAddToParty=	FALSE;

	m_pclRecvBuffer->Reset();
	m_pclSendBuffer->Reset();
}

void
CClient::setPartySerial(int _iSerial)
{
	m_wIsRequireAddToParty	=	FALSE;

	if	(_iSerial	==	-1)
		_iSerial	=	m_wPartySerial;
	else
	if	(m_wPartySerial	!=	0xffff)
		g_partyManager.removePartyMember(m_wPartySerial,m_wSerial);

	if	(_iSerial		!=	0xffff)	
		if	(!g_partyManager.addPartyMember(_iSerial,this))
			m_wIsRequireAddToParty	=	TRUE;

	m_wPartySerial		=	_iSerial;
}

void
CClient::setGuildSerial(int _iSerial , int _GuildDisplayIndex)
{
	if	(m_wGuildSerial	!=	0xffff)
		g_guildManager.removeGuildMember(m_wGuildSerial,m_wSerial);

	if	(_iSerial		!=	0xffff)	
		g_guildManager.addGuildMember(_iSerial,m_wSerial , _GuildDisplayIndex);

	m_wGuildSerial		=	_iSerial;
}

void
CClient::setFieldSerial(int _iSerial)
{
	if	(m_wCurrentField!=	0xffff)
		g_fieldManager.removeUser(m_wCurrentField,m_wSerial);

	if	(_iSerial		!=	0xffff)	
		g_fieldManager.addUser(_iSerial,m_wSerial);

	m_wCurrentField		=	_iSerial;
}

void
CClient::setAvatarIndex(int _iIndex)
{
	m_wAvatarIndex	=	_iIndex;
}

int
CClient::AddRecvPacket(char * _cData,int _iSize)	//	packet을 더한다.
{
#ifdef _DEBUG
	EnterCriticalSection(&csDebug);
	//	쪼개지지 않았다고 가정한다.
	if(timeGetTime() - m_dwLastRecvUpdateTime >= 1000)
	{
		m_dwRecvSizePerSec = m_dwRecvTmpSizePerSec;
		m_dwRecvTmpSizePerSec = _iSize;
		m_dwLastRecvUpdateTime = timeGetTime();
	}else
	{
		m_dwRecvTmpSizePerSec += _iSize;
	}
	LeaveCriticalSection(&csDebug);
#endif
	return m_pclRecvBuffer->AddData(_cData,_iSize);
}

int CClient::AddRecvPacket(int _iSize)			//	packet을 더한다.
{
	return AddRecvPacket(m_stRecvOL.buf.buf ,_iSize);
}

int CClient::GetRecvPacket(char * _cRecvpacket)	//	받은 패킷을 빼내온다.
{
	return m_pclRecvBuffer->GetData(_cRecvpacket,1);
}

int CClient::AddSendPacket(char * _cData,int _iSize)	//	packet을 더한다.  필요 없어서 삭제한다.
{
	return m_pclSendBuffer->AddData(_cData,_iSize);
}

int CClient::CompleteSendPacket(int _iSize)
{
	if	(_iSize	<	0)
	{
		__asm
		{
			int	3;
		}
	}

	if	(m_wSendpacket_size>_iSize	&&	_iSize	>=	0)	//	하나의 데이터가 완전하게 보내지지 않았다면..
	{
		char	tmpPacket[dMAX_PACKET_LENGTH];
		
		memset(tmpPacket,0,dMAX_PACKET_LENGTH);
		memcpy(tmpPacket,m_pcSendpacket+_iSize,m_wSendpacket_size-_iSize);
		memset(m_pcSendpacket,0,dMAX_PACKET_LENGTH);
		m_wSendpacket_size	-=	_iSize;

		if	(m_wSendpacket_size	>	MAX_LENGTH)
		{
			__asm
			{
				int	3;
			}
		}

		memcpy(m_pcSendpacket,tmpPacket,m_wSendpacket_size);

		return m_wSendpacket_size;
	}
	
	m_pclSendBuffer->Next();

#ifdef _DEBUG
	CCritical CS(&csDebug);
	//	쪼개지지 않았다고 가정한다.
	if(timeGetTime() - m_dwLastSendUpdateTime >= 1000)
	{
		m_dwSendSizePerSec = m_dwSendTmpSizePerSec;
		m_dwSendTmpSizePerSec = _iSize;
		m_dwLastSendUpdateTime = timeGetTime();
	}else
	{
		m_dwSendTmpSizePerSec += _iSize;
	}
#endif
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//	Class CClientManager

CClientManager::CClientManager()
{	InitializeCriticalSection(&csDeque);
	Init();
}

CClientManager::~CClientManager()
{	Init();
	DeleteCriticalSection(&csDeque);
}

CClientManager & CClientManager::GetInstance()
{	static CClientManager cmanager;
	return cmanager;
}

WORD CClientManager::NextNext()
{	for(int _next = m_wNext;_next<dBC_MAX_USER_COUNT;_next++)
	{	if(User[_next].m_wSerial== 0xffff)
		{
			m_wNext = _next;
			return m_wNext;
		}
	}
	int end = m_wNext;
	for(_next = 0;_next<end;_next++)
	{	if(User[_next].m_wSerial == 0xffff)
		{	m_wNext = _next;
			return m_wNext;
		}
	}
	m_wNext = 0xffff;
	return 0xffff;
}

void CClientManager::Init()
{	m_wNext = 0;		m_wCount = 0;
	for(int i=0;i<dBC_MAX_USER_COUNT;i++)
	{
		User[i].Kill();
	}
}

void CClientManager::Del(WORD _wSerial)
{	CCritical	CS(&csDeque);

	if(_wSerial>=dBC_MAX_USER_COUNT)		return;
	if(User[_wSerial].m_wSerial == 0xffff)		return;

	User[_wSerial].Kill();
	if(m_wCount>0)
		m_wCount--;
	
	if(m_wNext == 0xffff)	m_wNext = _wSerial;
}

WORD CClientManager::Add()
{	CCritical	CS(&csDeque);

	WORD	wResult = 0xffff;
	if(m_wCount >= dBC_MAX_USER_COUNT)	{ NextNext(); return wResult; }
	if(m_wNext == 0xffff)				{ NextNext(); return wResult; }
	wResult = m_wNext;
	User[wResult].m_wSerial = wResult+1;	//	임시
	m_wCount++;
	NextNext();
	return wResult;
}

CClient		*CClientManager::GetClientPnt(WORD _wSerial)
{	
	if(_wSerial >=dBC_MAX_USER_COUNT)
		return NULL;

	if(User[_wSerial].m_wSerial<0xffff)
		return &User[_wSerial];

	return NULL;
}

CClient	*CClientManager::GetClientPnt(char * _strId, BOOL _isSearchByName)
{
	DWORD	dwHashCode = cSRVUTIL::GetHashCode((BYTE *)_strId);

	if(!_isSearchByName)
	{
		for(int i=0;i<dBC_MAX_USER_COUNT;i++)
		{
			if(User[i].m_wSerial>=0xffff)
				continue;

			if(User[i].m_dwHashIdCode == dwHashCode && STRICMP(User[i].m_strId,_strId)==0)
				return &User[i];
		}
	}else
	{
		for(int i=0;i<dBC_MAX_USER_COUNT;i++)
		{
			if(User[i].m_wSerial>=0xffff)
				continue;
			if(User[i].m_dwHashNameCode == dwHashCode && STRICMP(User[i].m_strName,_strId)==0)
				return &User[i];
		}
	}

	return NULL;
}
