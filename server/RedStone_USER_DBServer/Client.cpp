// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////
#include <winsock2.h>
#include "Client.h"

#define dGAPLIMIT		180000

cCertificationList		CertManager;

static	int		basesize = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient(int	buffersize)
{	isSending = FALSE;
	MAX_LENGTH		=	buffersize;
	isUsed			=	FALSE;
	isClose			=	TRUE;
	sock			=	INVALID_SOCKET;
	sbuf_length		=	0;
	rbuf_length		=	0;
	timegap			=	0;
	sendpacket_size =	0;
	wIsSendList		=	0;
	InitializeCriticalSection(&csSendBuffer);
	InitializeCriticalSection(&csUSER);
	InitializeCriticalSection(&csRECVBUFFER);
	InitializeCriticalSection(&csSENDBUFFER);
}

CClient::~CClient()
{
	Kill();
}

void	CClient::CSReset()
{
	isSending = FALSE;
	DeleteCriticalSection(&csUSER);
	DeleteCriticalSection(&csSendBuffer);
	DeleteCriticalSection(&csSENDBUFFER);
	DeleteCriticalSection(&csRECVBUFFER);

	InitializeCriticalSection(&csSendBuffer);
	InitializeCriticalSection(&csUSER);
	InitializeCriticalSection(&csSENDBUFFER);
	InitializeCriticalSection(&csRECVBUFFER);
}

void	CClient::Open()
{	
	UsedUser();
	isSending = FALSE;
	isUsed			=	TRUE;
	isClose			=	FALSE;
	timegap			=	timeGetTime();
	wUserCount		=	0;
	sbuf_length		=	0;
	rbuf_length		=	0;
	wIsSendList		=	0;

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
	UsedBuffer();
	if (INVALID_SOCKET != sock) 
	{ 
		closesocket(sock); sock = NULL;
	} 
	isClose	= TRUE;
	sbuf_length = 0;
	NotUsedBuffer();

	EnterCriticalSection(&csRECVBUFFER);
	rbuf_length = 0;
	LeaveCriticalSection(&csRECVBUFFER);

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
}

void	CClient::Kill()
{
	if (INVALID_SOCKET != sock) 
	{ 
		closesocket(sock); sock = NULL;
	} 
	isUsed			=	FALSE;
	isClose			=	TRUE;
	DeleteCriticalSection(&csUSER);
	DeleteCriticalSection(&csSendBuffer);
	DeleteCriticalSection(&csRECVBUFFER);
	DeleteCriticalSection(&csSENDBUFFER);
}

void		CClient::SetIsUsed(BOOL isused)
{	UsedUser();
	isUsed = isused;
	isClose = isused ^ 1;
	NotUsedUser();
}

void		CClient::SetID(char *nid){	memcpy(&id,nid,dWORLD_NAME_LENGTH);	}
void		CClient::SetIP(char *nip){	memcpy(&ip,nip,dIP_SIZE);	}
void		CClient::SetServerIndex(WORD _idx){	wServerIndex = _idx;}
void		CClient::SetServerListIndex(WORD _idx){	wServerListIndex = _idx;}

void		CClient::SetUserCount(WORD count)
{	UsedUser();
	wUserCount = count;
	NotUsedUser();
}

BOOL	CClient::isWaste()
{
	int iTime = timeGetTime() - GetTimeGap();
	if	( (abs(iTime) > 180000) && ((DWORD)timeGetTime() > (DWORD)GetTimeGap()))
	{
		return TRUE;
	}
	return FALSE;
}


int		CClient::AddRecvPacket(char *data,int size)
{
	EnterCriticalSection(&csRECVBUFFER);
	if( (rbuf_length>=MAX_LENGTH) || ((rbuf_length+size)>=MAX_LENGTH) || data==NULL){	LeaveCriticalSection(&csRECVBUFFER);return 0;	}
	memcpy(lpRecvBuf+rbuf_length,data,size);
	rbuf_length+=size;
	timegap = timeGetTime();
	LeaveCriticalSection(&csRECVBUFFER);
	return 1;
}
int		CClient::AddRecvPacket(int size){	return AddRecvPacket(rOL.buf.buf,size);}
int		CClient::GetRecvPacket(char * recvpacket)
{	
	EnterCriticalSection(&csRECVBUFFER);
	memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);

	if( rbuf_length<basesize )
	{
		LeaveCriticalSection(&csRECVBUFFER);return 0;	
	}
	WORD	size=0; memcpy(&size,lpRecvBuf,sizeof(WORD));
	WORD	type=0; memcpy(&type,lpRecvBuf+sizeof(WORD),sizeof(WORD));

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃	받은 메세지 유효검사 
//	if(type!=0x1000 && (type<0x1020 || type>=0x1060)){_log("GetRecvPacket : type 0x%x",type);memset(recvresult,0,dMAX_PACKET_LENGTH);LeaveCriticalSection(&csRECVBUFFER);return 1;	}
//	if(size<6 || size>GetPacketSize(type) || GetPacketSize(type)==0){_log("GetRecvPacket : size %d (real : %d)",size,GetPacketSize(type));memset(recvresult,0,dMAX_PACKET_LENGTH);LeaveCriticalSection(&csRECVBUFFER);return 1;	}
	if(type<0x10 || type>0x9500 || size>=41960 || size==0)
	{
		_log("SERROR : GetRecvPacket type[0x%x][%d][%d]",type,size,rbuf_length);
		rbuf_length = 0;
		memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);
		memset(lpRecvBuf,0,dMAX_PACKET_LENGTH_SERVER);
		LeaveCriticalSection(&csRECVBUFFER);
		return 0;
	}
	//┃	받은 메세지 유효검사 
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━

	if(rbuf_length<size ){	LeaveCriticalSection(&csRECVBUFFER);	return 0;	}
	memcpy(recvpacket,lpRecvBuf,size);
	rbuf_length -=size;
	if(rbuf_length>0){
		char tmp[dMAX_PACKET_LENGTH_SERVER];
		memcpy(tmp,lpRecvBuf+size,rbuf_length);
		memset(lpRecvBuf,0,dMAX_PACKET_LENGTH_SERVER);
		memcpy(lpRecvBuf,tmp,rbuf_length);
	}else if(sbuf_length<0)
	{	
//		_log("CClient::GetRecvPacket : rbuf_length < getrecvsize(%d) : %d",size,rbuf_length);
		rbuf_length = 0;
		memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);
		memset(lpRecvBuf,0,dMAX_PACKET_LENGTH_SERVER);
		LeaveCriticalSection(&csRECVBUFFER);
		return 0;	//	잘못된 유저이다 바로 잘라버려!! , 일단 보류 
	}
	LeaveCriticalSection(&csRECVBUFFER);
	return 2;
}

int		CClient::AddSendPacket(char *data,int size)
{
	UsedBuffer();
	WORD	s = 0,t = 0;
	memcpy(&s,data,sizeof(WORD));
	memcpy(&t,data+sizeof(WORD),sizeof(WORD));

	if(t<0x10 || t>=0x9500 || s>41960 || size==0)
	{
		_log("[%s]'s Add Send Packet [%d/%d] [0x%x]",GetID(),s,size,t);
		NotUsedBuffer();
		return 0;
	}

	if( (sbuf_length+size)>=MAX_LENGTH )
	{	NotUsedBuffer();
		return 0;
	}
	memcpy(lpSendBuf+sbuf_length,data,size);
	sbuf_length+=size;
	NotUsedBuffer();

	return 1;
}
int		CClient::CompleteSendPacket(int size)
{
	UsedBuffer();
	int		restSize =0;
	sbuf_length -=size;
	if(sbuf_length>0)
	{
		char tmp[dMAX_PACKET_LENGTH_SERVER];
		memcpy(tmp,lpSendBuf+size,sbuf_length);
		memset(lpSendBuf,0,dMAX_PACKET_LENGTH_SERVER);
		memcpy(lpSendBuf,tmp,sbuf_length);	
		//	다음 패킷의 모든 데이타가 0이 될때가 있다.. 왜지 -_-; 아래인가. ~~
	}else if(sbuf_length==0){
		memset(lpSendBuf,0,dMAX_PACKET_LENGTH_SERVER);
	}else
	{
		memset(lpSendBuf,0,dMAX_PACKET_LENGTH_SERVER);
		sbuf_length = 0;	sendpacket_size = 0;	isSending = FALSE;
		NotUsedBuffer();	return 0;	
	}

	sendpacket_size-=size;
	if(sendpacket_size>0)
	{	restSize = sendpacket_size;	isSending = TRUE;
	}
	else
	{	restSize = 0;	isSending = FALSE;
	}
	WORD wResult = restSize;
	NotUsedBuffer();
	return wResult;
}

//****************************************************************************************
//	Class	Client Manager
//****************************************************************************************
CClientManager::CClientManager()
{
	InitializeCriticalSection(&csDeque);
	Init();
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
	for(int i=0;i<dUSERDB_MAX_USER_COUNT;i++)
		User[i].serial = 0xffffffff;
}

void	CClientManager::End()
{
	CCritical CS(&csDeque);
	wNext = 0; wCount = 0;
	for(int i=0;i<dUSERDB_MAX_USER_COUNT;i++)
		User[i].serial = 0xffffffff;
}

WORD	CClientManager::Next()
{
	for(int i = 0;i<dUSERDB_MAX_USER_COUNT;i++)
		if(User[i].GetIsClose() || User[i].serial == 0xffffffff)	break;

	if(i >= dUSERDB_MAX_USER_COUNT)	wNext = 0xffff;
	else						wNext = i;
	return wNext;
}

int		CClientManager::Add()
{
	CCritical CS(&csDeque);
	int	iResult = 0xffff;
	if(wNext == 0xffff){	return -1;	}
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
	if(wCount>=dUSERDB_MAX_USER_COUNT)	
	{
		_logf(" ERROR ClientManager Delete wCount = %d",wCount);
		wCount = 0;
	}
}

CClient		*CClientManager::GetClientPnt(char *id)	//	ID로 CClient 포인터 얻기
{
	CCritical CS(&csDeque);
	
	CClient * pClient = NULL;
	for(int i=0;i<dUSERDB_MAX_USER_COUNT;i++)
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

//////////////////////////////////////////////
//	For cCertification

BOOL _CERT_DATA::isWaste()
{
	DWORD	checkTime = timeGetTime();
	if(checkTime < dwTime)
		dwTime = checkTime;

	checkTime -= dwTime;
	if( ( checkTime> 300000) )	return TRUE;
	return FALSE;
}

cCertificationList::cCertificationList()
{
	InitializeCriticalSection(&csDeque);
	for(int i = 0;i<100000;i++)	User[i].init();
	m_wNext = m_wCount = 0;

	srand( timeGetTime()%0xffffffff );
}

cCertificationList::~cCertificationList()
{
	DeleteCriticalSection(&csDeque);
}

DWORD		cCertificationList::NextNext()
{
	for(int _next = m_wNext;_next<100000;_next++)
	{
		if(User[_next].dwCode == 0xffffffff)
		{
			m_wNext = _next;
			return m_wNext;
		}
			
	}
	int end = m_wNext;
	for(_next = 0;_next<end;_next++)
	{
		if(User[_next].dwCode == 0xffffffff)
		{
			m_wNext = _next;
			return m_wNext;
		}
	}
	_log("NEXTNEXT ERROR ERROR ERROR ERROR");

	return 0xffffffff;
}

DWORD		cCertificationList::add(char * _strId,char * _strPw)
{
	CCritical	CS(&csDeque);

	DWORD	result = 0xffffffff;

	if(m_wCount == 100000)	return 0xffffffff;
	if(m_wNext == 0xffffffff)				return 0xffffffff;

	result = User[m_wNext].dwCode = rand();
	User[m_wNext].dwTime = timeGetTime();
	strcpy(User[m_wNext].strId,_strId);
	strcpy(User[m_wNext].strPw,_strPw);
	m_wCount++;

	NextNext();
	return result;
}

void		cCertificationList::get(_CERT_DATA	* _data,char * _strId,DWORD _code)
{
	CCritical	CS(&csDeque);

	_data->dwCode = 0xffffffff;
	if(_code == 0xffffffff)				return;

	for(int i = 0;i<100000;i++)
	{
		if(User[i].dwCode == _code)
		{
			if(stricmp(_strId,User[i].strId)==0)
			{
				memcpy(_data,&User[i],sizeof(_CERT_DATA));
				User[i].init();
				m_wCount--;
				m_wNext = i;
				printf(" count %d , next = %d\n",m_wCount,m_wNext);
				return ;
			}
		}
	}
	return ;
}
