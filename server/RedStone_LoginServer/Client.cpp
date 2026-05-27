 // Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////

#include "Client.h"
#include "cPACKET_BASE.h"
#include "cPACKET_LOGINSERVER.h"

#define dGAPLIMIT		150000

static	int		basesize;
int		GetPacketSize(WORD	pType);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient(int	buffersize)	:	m_bOtp(FALSE)
{
	wSelectedServerIndex = 0;
	m_wCheckBit		=	0xffff;
	m_wClientType	=	0xffff;
	isConnectGame = FALSE;
	MAX_LENGTH		=	buffersize;
	isUsed			=	FALSE;
	isClose			=	TRUE;
	sock			=	INVALID_SOCKET;
	timegap			=	0;
	gapcheckcount	=	0;
	loginTime		=	0;
	serial			=	0xffffffff;
	sendpacket_size =	0;
	InitializeCriticalSection(&csUSER);

	rBuffer			=	new CPacketBuffer(buffersize);
	sBuffer			=	new CPacketBuffer(buffersize);
}

CClient::~CClient()
{
	Kill();
	DeleteCriticalSection(&csUSER);
	m_bOtp = FALSE;
}

void	CClient::CSReset()
{
	isConnectGame = FALSE;
}

void	CClient::Open()
{
	CCritical cs(&csUSER);

	m_bOtp = FALSE;
	strcpy(id,"");
	strcpy(tmpid,"");
	isConnectGame = FALSE;
	isUsed			=	TRUE;
	isClose			=	FALSE;
	timegap			=	timeGetTime();
	gapcheckcount	=	0;
	m_wCheckBit		=	0xffff;

	memset(&rOL.OL,0,sizeof(OVERLAPPED));
	rOL.buf.buf = rOL.Buffer;
	rOL.buf.len = dMAX_PACKET_LENGTH;

	memset(&sOL.OL,0,sizeof(OVERLAPPED));
	sOL.buf.buf = sOL.Buffer;
	sOL.buf.len = dMAX_PACKET_LENGTH;
	sOL.IOState = 0;
	
	dwHashIdCode = 0;

	memset(list,0,sizeof(stAvatarDummyList)*dMAX_AVATAR_COUNT);

	rBuffer->Reset();
	sBuffer->Reset();
}

void	CClient::PreClose()
{
	if(!isUsed)	return ;

	if (INVALID_SOCKET != sock) 
	{
		shutdown(sock,1);
		closesocket(sock);
		sock = NULL;
	}
	isClose	= TRUE;
}

void	CClient::Close()
{
	strcpy(id,"");
	strcpy(tmpid,"");

	if (INVALID_SOCKET != sock) 
	{ 
		shutdown(sock,1);
		closesocket(sock);
		sock = NULL;
	} 
	isClose	= TRUE;
	serial = 0xffffffff;		//	서버에게서 받은 시리얼

	isUsed			=	FALSE;
	timegap			=	0;
	gapcheckcount	=	0;
	loginTime		=	0;
	m_wClientType	=	0xffff;
}

void	CClient::Kill()
{
	if (INVALID_SOCKET != sock) 
	{ 
		shutdown(sock,1);
		closesocket(sock);
		sock = NULL;
	} 

	isUsed			=	FALSE;
	isClose			=	TRUE;
}

BOOL	CClient::isWaste()
{
	DWORD	checkTime = timeGetTime();
	if(checkTime < timegap)
	{
		checkTime += 0xffffffff-timegap;
	}else
		checkTime -= timegap;
	if( ( checkTime> 300000 && checkTime<500000) )
	{
		_log(" disconnect[cause too long idle] .. id[%s] serial [%d] time[%d]",id,serial,checkTime);
		return TRUE;
	}
	return FALSE;
}


int		CClient::AddRecvPacket(char *data,int size)
{	
	int iTIme = timeGetTime() - timegap;
	if(abs(iTIme) < 100)
	{
		++gapcheckcount;
		if(gapcheckcount>500)
		{
			_log(" 잡아내 이 유저.. ip[%s] id[%s] count %d",ip,id,gapcheckcount);
			return 0;
		}
	}
	 timegap = timeGetTime();
	
	return rBuffer->AddData(data,size);
}

int		CClient::AddRecvPacket(int size){	return AddRecvPacket(rOL.buf.buf,size);	}
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


/*****************************************************************************************
	Class	Client Manager
*****************************************************************************************/
CClientManager::CClientManager()
{	
	InitializeCriticalSection(&csDeque);
	InitializeCriticalSection(&csDequeList);

	ListSendData = new WORD[dLISTSENDDATA_MAXCOUNT];
	memset(ListSendData,0xff,dLISTSENDDATA_MAXCOUNT);
	wNextList=0;wCountList=0;wCurrent = 0;
	InitializeCriticalSection(&csDequeList);

	Init();	
}

CClientManager::~CClientManager()
{
	DeleteCriticalSection(&csDeque);
	DeleteCriticalSection(&csDequeList);

	delete [] ListSendData;
	ListSendData = NULL;
}

void	CClientManager::Init()
{
	CCritical CS(&csDeque);

	wNext = 0; wCount = 0;
	for(int i=0;i<dLOGIN_MAX_USER_COUNT;i++)
	{
		User[i].serial = 0xffffffff;
	}
}

void	CClientManager::ListUp(WORD _serial)
{
	CCritical CS(&csDequeList);

	if(wNextList<1  || ListSendData[wNextList-1] != _serial)
	{
		ListSendData[wNextList] = _serial;
		wNextList++;
		wCountList++;
		if(wNextList>=dLISTSENDDATA_MAXCOUNT)	wNextList = 0;
//		_log("List Up : wNextList %d,wCountList %d, wCurrent %d",wNextList,wCountList, wCurrent);
	}
}

WORD	CClientManager::GetList()
{
	CCritical CS(&csDequeList);
	
	WORD ret = ListSendData[wCurrent];

	if(ret == 0xffff)
	ListSendData[wCurrent]=0xffff;
	wCurrent++;
	wCountList--;
	if(wCurrent>=dLISTSENDDATA_MAXCOUNT)	wCurrent = 0;

	_log("GetList : wNextList %d,wCountList %d, wCurrent %d",wNextList,wCountList, wCurrent);

	return ret;
}

void	CClientManager::End()
{
	CCritical CS(&csDeque);
	wNext = 0; wCount = 0;
	for(int i=0;i<dLOGIN_MAX_USER_COUNT;i++)
		User[i].serial = 0xffffffff;
}

DWORD	CClientManager::Next()
{
	for(int i = 0;i<dLOGIN_MAX_USER_COUNT;i++)
		if(User[i].serial > 0xcccccccc)	break;
	if(i >= dLOGIN_MAX_USER_COUNT)	wNext = 0xffffffff;
	else							wNext = i;
	return wNext;
}

DWORD		CClientManager::Add()
{
	CCritical CS(&csDeque);
	DWORD	iResult = 0xffffffff;
	if(wNext == 0xffffffff)
	{	
		printf("in Add() , wNext == 0xffffffff\n");	
		Next();
		return -1;	
	}
	if(User[wNext].serial == 0xfffffffe)
	{
		return -1;
	}
//	User[wNext].Open();
	User[wNext].isUsed = TRUE;
	strcpy(User[wNext].id,"");
	strcpy(User[wNext].tmpid,"");

//	User[wNext].SetIsUsed(TRUE);		//	사용을 위해 일단 이것만 설정해둔다.
	User[wNext].serial = wNext;
//	User[wNext].SetSerial(wNext);
	iResult = wNext;
	wCount++;
	Next();
	return iResult;						//	마지막에 있는 값을 돌려준다.
}

void	CClientManager::Del(DWORD idx)
{
	CCritical CS(&csDeque);
	if(idx>dLOGIN_MAX_USER_COUNT){	printf(" in Del, idx==0xffffffff\n");	return;	}
	User[idx].Close();
	if(wNext==0xffffffff)	wNext = idx;
	wCount--;
	if(wCount>=dLOGIN_MAX_USER_COUNT)	
	{
		_logf(" ERROR ClientManager Delete wCount = %d",wCount);
		wCount = 0;
	}
}

CClient		*CClientManager::GetClientPnt(char *id)	//	ID로 CClient 포인터 얻기
{
	CCritical CS(&csDeque);
	
	DWORD	hashid = GetHashCode((BYTE *)id);
	CClient * pClient = NULL;
	for(int i=0;i<dLOGIN_MAX_USER_COUNT;i++)
	{
		if(User[i].serial > 0xccccccccc)	continue;
		if(User[i].dwHashIdCode==hashid && (STRICMP(id,User[i].id)==0 && User[i].GetSocket()>0) )
		{
			return &User[i];
			break;
		}
	}
	return pClient;
}

CClient		*CClientManager::GetClientPnt(DWORD idx,BOOL	isSerial)	//	serial로 CClient 포인터 얻기
{
	CCritical CS(&csDeque);

	CClient * pClient = NULL;
	
	if(idx>dLOGIN_MAX_USER_COUNT)
	{
//		_logf("ERROR GetClientPnt idx [%d]",idx);
		return NULL;
	}
	if(User[idx].serial<0xcccccccc)	pClient = &User[idx];

	return pClient;

}

void		CClientManager::SetBaseSize(int bs){	basesize = bs;	}

void		CClientManager::test(char *name)
{
	CCritical CS(&csDeque);
	
	CClient * pClient = NULL;
	_logf("Start [%s]---------------------------------",name);
	for(int i=0;i<dLOGIN_MAX_USER_COUNT;i++)
	{
		if(User[i].sock==0xcccccccc)
		{
			_logf("incorrected index =%d",i);
			continue;
		}
	}
	_logf("end ---------------------------------");
}

