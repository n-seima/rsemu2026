// cSOCKET.cpp: implementation of the cSOCKET class.
//
//////////////////////////////////////////////////////////////////////

//	쓰기전에 패킷 기본 사이즈 항상 확인을 ..
#include <process.h>
#include "cSOCKET.h"
#include "stdio.h"
#include "cPACKET_BASE.h"

void	IOThread(void *tmp);
void	SendThread(void *tmp);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


cSOCKET::cSOCKET()
{
	isReconnect			=	FALSE;
	sock				=	NULL;
	isLoopRecv			=	TRUE;
	isSendLoopRecv		=	TRUE;
	hThread				=	FALSE;
	isSync				=	FALSE;
#ifdef _ADMIN_TOOL
	rPacketBuf			=	new CPacketBuffer(8192);
	sPacketBuf			=	new CPacketBuffer(8192);
#else
	rPacketBuf			=	new CPacketBuffer(8192000);
	sPacketBuf			=	new CPacketBuffer(8192000);
#endif

#ifdef	_DEBUG
	m_dwTotalSendSize	=	0;
	m_dwTotalSendSizeInPacket = 0;
	m_dwTotalSendCount	=	0;

	m_dwTotalRecvSize	=	0;
	m_dwTotalRecvCount	=	0;

	m_dwSendCheckTime	=	0;
	m_dwRecvCheckTime	=	0;
#endif
}

cSOCKET::~cSOCKET()
{
	delete			rPacketBuf;
	delete			sPacketBuf;
}

//	public
BOOL	cSOCKET::init(int port, char *addr, char *_strName, WORD basepacketsize,BOOL _isSync)
{
	strcpy(m_strIp,addr);
	strcpy(name,_strName);
	m_iPort = port;
	if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
	{	_log("ERROR : WSAStartUp \n");
		kill();
		return	FALSE;
	}

	if((sock = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,NULL)) == INVALID_SOCKET)
	{
		_log("ERROR : WSASocket \n");
		kill();
		return	FALSE;
	}
	int timeout =1;	//	중첩된 입출력 소켓에서 블럭시간 설정 가능
	setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(int));
	int buf=dSOCK_MAXBUFFER_SIZE;
	if(SOCKET_ERROR ==setsockopt(sock,SOL_SOCKET ,SO_SNDBUF,(const char*)&buf,sizeof(buf)))
	{
		_log(" set socket option error 1 ");
		kill();
		return FALSE;
	}
	if(SOCKET_ERROR ==setsockopt(sock,SOL_SOCKET ,SO_RCVBUF,(const char*)&buf,sizeof(buf)))
	{
		_log(" set socket option error 2 ");
		kill();
		return FALSE;
	}
	LINGER	lingerStruct; lingerStruct.l_onoff = 1; lingerStruct.l_linger = 0; 
	if(SOCKET_ERROR ==setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct)))
	{
		_log(" set socket option error 3 ");
		kill();
		return FALSE;
	}

	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = inet_addr(addr);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);

	if(connect(sock,(LPSOCKADDR)&serverAddress,sizeof(serverAddress)) == SOCKET_ERROR)
	{
#ifndef	_FOR_INNER
		_log("ERROR : connect [%d,%s] Error %d\n",port,addr,GetLastError());
#endif
		kill();
		return FALSE;
	}
	isLoopRecv			=	TRUE;
	hThread = (HANDLE)_beginthread(IOThread,NULL,(void *)this);
	if(hThread==NULL)
	{	_log("ERROR : thread create \n");
		isLoopRecv			=	FALSE;
		kill();
		return FALSE;
	}
	Sleep(1);
	isSync = _isSync;
	if(!isSync)
	{
		isSendLoopRecv		=	TRUE;
		hSendThread = (HANDLE)_beginthread(SendThread,NULL,(void *)this);
		if(hSendThread==NULL)
		{	_log("ERROR : thread create \n");
			isSendLoopRecv		=	FALSE;
			kill();
			return FALSE;
		}
	}else
	{
		_log(" send thread 안돌림.");
	}
	wBaseSize = basepacketsize;
#ifdef	_DEBUG
	m_dwTotalSendSize	=	0;
	m_dwTotalSendSizeInPacket = 0;
	m_dwTotalSendCount	=	0;

	m_dwTotalRecvSize	=	0;
	m_dwTotalRecvCount	=	0;

	m_dwSendCheckTime	=	timeGetTime();
	m_dwRecvCheckTime	=	timeGetTime();
#endif

	return TRUE;
}

int		cSOCKET::reConnect()
{
	isSendLoopRecv = FALSE;
	this->sPacketBuf->Reset();
	::WaitForSingleObject(hSendThread , 0);
	if(sock)
	{	
		shutdown(sock,1);
		closesocket(sock); sock = NULL;
	}

	Sleep(100);

	if(!init(m_iPort,m_strIp,name,wBaseSize))
	{
		kill();
		return 0;
	}
	isReconnect			=	TRUE;
	return 1;
}

void	cSOCKET::kill()
{
	isLoopRecv = FALSE;
	isSendLoopRecv = FALSE;
	this->rPacketBuf->Reset();
	this->sPacketBuf->Reset();

	::WaitForSingleObject(hThread , 0);
	if(!isSync)
		::WaitForSingleObject(hSendThread , 0);
	if(sock)
	{	
		shutdown(sock,1);
		closesocket(sock); sock = NULL;
	}
	strcpy(name,"");
}

BOOL	cSOCKET::SendPacket(char *buf,int size)
{
	if(!isSync)
	{
		if(!sPacketBuf->AddData(buf,size))
			return FALSE;
	}else
	{
		CoreSend(buf, size);
	}

	return TRUE;
}

void	cSOCKET::ReallySend()
{
	char	buf[dSOCK_MAXBUFFER_SIZE];
	WORD	wSize = 0;

	DWORD	startTime = timeGetTime();
	DWORD	count = 0;
	while(TRUE)
	{
		wSize = sPacketBuf->GetData(buf,1);
		if(wSize ==0)
			break;

		++count;
		if(CoreSend(buf,wSize)==FALSE)
		{
			isLoopRecv = FALSE;
			isSendLoopRecv = FALSE;
			return;
		}
		wLastPacket = ((cMSG_BASE_TYPE *)buf)->wType;
		if(count >100)	break;
	}
//	if(count >20)
//		_log(" [%s] send time %dms , count %d, cur Bufsize %d\n",name,timeGetTime() - startTime,count,sPacketBuf->GetSize());
	return ;
}

BOOL	cSOCKET::CoreSend(char * data, int size)
{
	WORD	_wSize = 0,_wSendAllSize = 0;
	int		iSendSize = 0;
	WORD	_loop=0;

	_wSendAllSize = _wSize = size;

	if(*(WORD *)data ==0 || *(WORD *)data>=65535)
	{
		_log("SOCKET ERROR : [%s] send size error -> size %d, data size %d",name,size,*(WORD *)data);
		return 2;
	}

	if(_wSendAllSize>=41960)
	{
		_log("SOCKET ERROR : Size Over [%d]",_wSendAllSize);
		return 2;
	}

SEND_RETURN:
	iSendSize = send(sock,data+iSendSize,_wSize,0);
	if(iSendSize==SOCKET_ERROR || iSendSize==0 || iSendSize==0xffff)
	{
		if(WSAEWOULDBLOCK==GetLastError())
		{
			_log(" Core Send Warning : %d", GetLastError());
			iSendSize = 0;
			_loop++;
			if(_loop>10)
			{
				_log("CORE SEND _loop [%d] \n",_loop);
				return FALSE;
			}
		}else{
			WORD	_wType=0;
			_wType = *(WORD *)(data+2);
			_log("ERROR ReallySend :[%s]  size %d, type %x(LT %x) ,ERROR %d",name,_wSendAllSize,_wType,wLastPacket,GetLastError());
			isLoopRecv = FALSE;
			isSendLoopRecv = FALSE;
			return FALSE;
		}
		goto SEND_RETURN;
	}
	if(iSendSize<_wSize)
	{
		if(!sock)
		{
			_log("CORE SEND _loop [%d] \n",_loop);
			return FALSE;
		}
		_wSize -= iSendSize;
		_log("◇◇◇◇◇ socket loop  ◇◇◇◇◇(sendsize : %d, size : %d - %d)\n",_wSendAllSize,_wSize,iSendSize);
		goto SEND_RETURN;
	}
	return TRUE;
}

BOOL	cSOCKET::GetRecvPacket(char *recvpacket)
{
	BOOL ret = rPacketBuf->GetData(recvpacket,1);
	return ret;
}

void SendThread(void *tmp)
{
	cSOCKET	*soc = (cSOCKET *)tmp;

	while(soc->isSendLoopRecv == TRUE)
	{	Sleep(1);
		soc->ReallySend();	
	}
	return;
}

void IOThread(void *tmp)
{
	cSOCKET	*soc = (cSOCKET *)tmp;
	int		len=0,allLen = 0;
	DWORD	msgSize=0;
	char buf[dSOCK_MAXBUFFER_SIZE];
	memset(buf,0,dSOCK_MAXBUFFER_SIZE);
	soc->isLoopRecv = TRUE;
	
//	_log("Make IOThread in Socket Class ..sock %d",soc->sock);
	while(soc->isLoopRecv)
	{
		Sleep(1);
		if(!soc->sock)	continue;
		if(ioctlsocket(soc->sock,FIONREAD,&msgSize)==0 && msgSize>0)
		{
			int isLoopContinuously = 1;
			allLen = 0, len = 0;
			for(allLen = 0; allLen<(int)msgSize;	allLen += len)
			{
				len = recv(soc->sock,buf,dSOCK_MAXBUFFER_SIZE,0);
				if(len ==0 || len==SOCKET_ERROR)
				{	
					soc->isLoopRecv = FALSE;
					soc->isSendLoopRecv = FALSE;
//					_log("◇◇◇◇◇ ㅡㅡ+ in socket thread ◇◇◇◇\n");
					break;
				}
				if(isLoopContinuously && !soc->rPacketBuf->AddData(buf,len))
				{
//					_log("ERROR in cSOCKET : [%s] Recv Buffer Cur length %d, len = %d",soc->name,soc->rPacketBuf->GetSize(),len);
					isLoopContinuously = 0;
				}
			}
		}
	}
	if(soc->sock)
	{
		closesocket(soc->sock); soc->sock = NULL;
	}
	_log("------------------QUIT SOCKET THREAD ---------------------\n");
	return;
}

void	cSOCKET::EmergencyLog()
{
	FILE * fp = fopen("c:\\redstone_emergencyLog_lastpacket.pac","wb+");

	if	(!fp)
	{
		_log(" Can not make the emergencyLog file");
		return;
	}

	fclose(fp);
}