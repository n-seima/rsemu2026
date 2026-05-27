// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////
#include <winsock2.h>
#include "Client.h"
#include "cSRVUTIL.h"


#define dGAPLIMIT		180000

static	int		basesize = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient(int	buffersize)
{
	MAX_LENGTH		=	buffersize;
	isUsed			=	FALSE;
	isClose			=	TRUE;
	sock			=	INVALID_SOCKET;
	timegap			=	0;
	sendpacket_size =	0;
	serial			=	0xffff;

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

void	CClient::Open()
{
	CCritical	cs(&csUSER);
	isUsed			=	TRUE;
	isClose			=	FALSE;
	timegap			=	timeGetTime();

	memset(&rOL.OL,0,sizeof(OVERLAPPED));
	rOL.buf.buf = rOL.Buffer;
	rOL.buf.len = dMAX_PACKET_LENGTH;

	memset(&sOL.OL,0,sizeof(OVERLAPPED));
	sOL.buf.buf = sOL.Buffer;
	sOL.buf.len = dMAX_PACKET_LENGTH;
	sOL.IOState = 0;
}

void	CClient::Close()
{
	CCritical	cs(&csUSER);
	if (INVALID_SOCKET != sock) 
	{ 
		closesocket(sock); sock = NULL;
	} 
	serial			=	0xffff;
	isUsed			=	FALSE;
	isClose			=	TRUE;
}

void	CClient::Kill()
{
	CCritical	cs(&csUSER);
	if (INVALID_SOCKET != sock) 
	{ 
		closesocket(sock); sock = NULL;
	} 
	serial			=	0xffff;
	isUsed			=	FALSE;
	isClose			=	TRUE;
}

int		CClient::AddRecvPacket(char *data,int size)
{	
	timegap = timeGetTime();
	return rBuffer->AddData(data,size);
}
int		CClient::AddRecvPacket(int size){	return AddRecvPacket(rOL.buf.buf,size);}

int		CClient::GetRecvPacket(char * recvpacket)
{	
	int	ret = rBuffer->GetData(recvpacket,1);
	if(ret && *(WORD *)recvpacket==0 || *(WORD *)recvpacket>=65535)
	{
		_log("Error...2");
	}
	return ret;
}

int		CClient::AddSendPacket(char *data,int size)
{	
	if(*(WORD *)data==0 || *(WORD *)data>=65535)
	{
		_log("Error...3");
	}
	return sBuffer->AddData(data,size);
}

int		CClient::CompleteSendPacket(int size)
{
	if(sendpacket_size<size)	//	하나의 데이터가 완전하게 보내지지 않았다면..
	{
		char	tmpPacket[dMAX_PACKET_LENGTH];memset(tmpPacket,0,dMAX_PACKET_LENGTH);
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
{	InitializeCriticalSection(&csDeque);	
	Init();
}

CClientManager::~CClientManager()
{	DeleteCriticalSection(&csDeque);
}

WORD	CClientManager::NextNext()
{	for(int _next = m_wNext;_next<dWORLD_MAX_USER_COUNT;_next++)
	{	if(User[_next].GetSerial()== 0xffff && !User[_next].GetIsUsed())
		{
			m_wNext = _next;
			return m_wNext;
		}
	}
	int end = m_wNext;
	for(_next = 0;_next<end;_next++)
	{	if(User[_next].GetSerial() == 0xffff && !User[_next].GetIsUsed())
		{	m_wNext = _next;
			return m_wNext;
		}
	}
	_log("NEXTNEXT ERROR ERROR ERROR ERROR");

	//유저 정보를 초기화 하자.
	for(_next = 0;_next<dWORLD_MAX_USER_COUNT;_next++)
	{
		_log("idx [%d] , id[%s] serial [%d] socket [%d] timegap[%d] isUsed [%d]" ,User[_next].GetID(),User[_next].GetSerial(),User[_next].GetSocket(),User[_next].GetTimeGap(),User[_next].isUsed);
	}
	
	m_wNext = 0xffff;
	return 0xffff;
}
void	CClientManager::SetBaseSize(int bs){	basesize = bs;	}

void	CClientManager::Init()
{	m_wNext = 0;		m_wCount = 0;
	for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		User[i].Close();
	}
}
void	CClientManager::End(){	Init();	}

int		CClientManager::Add()
{	CCritical	CS(&csDeque);

	WORD	wResult = 0xffff;
	if(m_wCount == dWORLD_MAX_USER_COUNT)	return wResult;
	if(m_wNext == 0xffff)					return wResult;
	wResult = m_wNext;
	User[m_wNext].isUsed = TRUE;
	m_wCount++;
	NextNext();
	return wResult;
}

void	CClientManager::Del(int idx)
{	CCritical	CS(&csDeque);

	if(idx>=dWORLD_MAX_USER_COUNT)	return;
	if(User[idx].GetSerial() == 0xffff)	return;

	User[idx].Close();
	if(m_wCount==0)	_log("ERROR CClientManager::Del m_wCount = 0");
	else			m_wCount--;
	if(m_wNext == 0xffff)	m_wNext = idx;
}

CClient		*CClientManager::GetClientPnt(char *id)	//	ID로 CClient 포인터 얻기
{	
	for(int i = 0; i < dWORLD_MAX_USER_COUNT; i++)
	{
		if(!User[i].GetIsUsed())	continue;
		if(stricmp(id,User[i].GetID()) == 0  && User[i].GetSocket()>0)
			return &User[i];
	}
	return NULL;
}

CClient		*CClientManager::GetClientPnt(WORD idx)	//	serial로 CClient 포인터 얻기
{	
	if(idx >=dWORLD_MAX_USER_COUNT)
		return NULL;

	if(User[idx].GetIsUsed())
		return &User[idx];

	return NULL;
}
