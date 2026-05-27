// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////
#include <winsock2.h>
#include "Client.h"
#include "cPACKET_DBSERVER.h"
#include "cSRVUTIL.h"


#define dGAPLIMIT		180000

static	int		basesize = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient(int	buffersize)
{
	wServerIndex	=	0;
	MAX_LENGTH		=	buffersize;
	isUsed			=	FALSE;
	isClose			=	TRUE;
	sock			=	INVALID_SOCKET;
	timegap			=	0;
	sendpacket_size =	0;
	serial			=	0xffffffff;

	rBuffer = new CPacketBuffer(buffersize);
	sBuffer = new CPacketBuffer(buffersize);

	InitializeCriticalSection(&csUSER);
}

CClient::~CClient()
{
	Kill();
	DeleteCriticalSection(&csUSER);
}

void	CClient::Open()
{
	CCritical	CS(&csUSER);
	wServerIndex	=	0;
	isUsed			=	TRUE;
	isClose			=	FALSE;
	timegap			=	timeGetTime();

	memset(&rOL.OL,0,sizeof(OVERLAPPED));
	rOL.buf.buf = rOL.Buffer;
	rOL.buf.len = dMAX_PACKET_LENGTH_SERVER;

	memset(&sOL.OL,0,sizeof(OVERLAPPED));
	sOL.buf.buf = sOL.Buffer;
	sOL.buf.len = dMAX_PACKET_LENGTH_SERVER;
	sOL.IOState = 0;
}

void	CClient::PreClose()
{
	if (INVALID_SOCKET != sock) 
	{ 
		closesocket(sock); sock = NULL;
	} 
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
}

void		CClient::SetIsUsed(BOOL isused)
{	CCritical	CS(&csUSER);
	isUsed = isused;
	isClose = isused ^ 1;
}

void		CClient::SetID(char *nid){	memcpy(&id,nid,dID_LENGTH);	}

BOOL	CClient::isWaste()
{
	int iTime = timeGetTime() - GetTimeGap();
	if( (abs(iTime) > dGAPLIMIT+1000) && (abs(iTime) < dGAPLIMIT*2) )
		return TRUE;

	return FALSE;
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
{	
	InitializeCriticalSection(&csDeque);
	Init();
	basesize = sizeof(cMSG_BASE_TYPE_FORLOG);
}

CClientManager::~CClientManager()
{
	DeleteCriticalSection(&csDeque);
}

void	CClientManager::SetBaseSize(int bs){	basesize = bs;}

CClientManager & CClientManager::GetInstance()
{	static CClientManager cmanager;
	return cmanager;
}

void	CClientManager::Init()
{	
	CCritical CS(&csDeque);
	wNext = 0; wCount = 0;
	for(int i=0;i<dLOG_MAX_USER_COUNT;i++)
		User[i].serial = 0xffffffff;
}

void	CClientManager::End()
{	
	CCritical CS(&csDeque);
	wNext = 0; wCount = 0;
	for(int i=0;i<dLOG_MAX_USER_COUNT;i++)
		User[i].serial = 0xffffffff;
}

WORD	CClientManager::Next()
{
	for(int i = 0;i<dLOG_MAX_USER_COUNT;i++)
		if(User[i].GetIsClose() || User[i].serial == 0xffffffff)	break;

	if(i >= dLOG_MAX_USER_COUNT)	wNext = 0xffff;
	else						wNext = i;
	return wNext;
}


int		CClientManager::Add()
{
	CCritical CS(&csDeque);
	int	iResult = 0xffff;
	if(wNext == 0xffff){	Next();	return -1;	}
	User[wNext].SetIsUsed(TRUE);		//	사용을 위해 일단 이것만 설정해둔다.
	User[wNext].SetSerial(wNext);
	iResult = wNext;
	wCount++;
	Next();
	return iResult;						//	마지막에 있는 값을 돌려준다.
}

void	CClientManager::Del(int idx)
{	
	CCritical CS(&csDeque);
	User[idx].Close();
	wNext = idx;
	wCount--;
	if(wCount>=dLOG_MAX_USER_COUNT)	
	{
		_logf(" ERROR ClientManager Delete wCount = %d",wCount);
		wCount = 0;
	}
}

CClient		*CClientManager::GetClientPnt(char *id)	//	ID로 CClient 포인터 얻기
{
	CCritical CS(&csDeque);
	
	CClient * pClient = NULL;
	for(int i=0;i<dLOG_MAX_USER_COUNT;i++)
	{	
		if(stricmp(id,User[i].GetID())==0 && User[i].GetSocket()>0)
		{	
			pClient = &User[i];	
			break;	
		}
	}
	return pClient;
}

CClient		*CClientManager::GetClientPnt(int idx,BOOL	isSerial)	//	serial로 CClient 포인터 얻기
{	
	CCritical CS(&csDeque);

	CClient * pClient = NULL;

	if(User[idx].GetIsUsed() && User[idx].serial<0xffffffff)	pClient = &User[idx];				

	return pClient;
}
