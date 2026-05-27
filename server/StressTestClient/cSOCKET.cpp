// cSOCKET.cpp: implementation of the cSOCKET class.
//
//////////////////////////////////////////////////////////////////////

//	쓰기전에 패킷 기본 사이즈 항상 확인을 ..
#include <process.h>
#include "cSOCKET.h"
#include "stdio.h"

void	IOThread(void *tmp);
void	SendThread(void *tmp);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


cSOCKET::cSOCKET(BOOL	isUsessl)
{
	isUseSSL			=	isUsessl;
	lastsendtime		=	0;
	lastrecvtime		=	0;
	sock				=	NULL;
	isLoopRecv			=	TRUE;
	hThread				=	FALSE;
	InitializeCriticalSection(&csSOCKET);
	InitializeCriticalSection(&csSEND);
}

cSOCKET::~cSOCKET()
{
	if(&csSOCKET)	DeleteCriticalSection(&csSOCKET);
	if(&csSEND)		DeleteCriticalSection(&csSEND);
}

//	public
BOOL	cSOCKET::init(int	port,	char	*addr , WORD basepacketsize)
{
	memset(sendbuf.buf,0,sizeof(sendbuf.buf));
	sendbuf.size = 0;
	memset(recvbuf.buf,0,sizeof(recvbuf.buf));
	recvbuf.size = 0;

	strcpy(name,"");

	if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
	{	_log("ERROR : WSAStartUp \n");
		return	FALSE;
	}

	if((sock = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,NULL)) == INVALID_SOCKET)
	{
		_log("ERROR : WSASocket \n");
		return	FALSE;
	}
	int timeout =1;	//	중첩된 입출력 소켓에서 블럭시간 설정 가능
	setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(int));
	int buf=dSOCK_MAXBUFFER_SIZE;
	if(SOCKET_ERROR ==setsockopt(sock,SOL_SOCKET ,SO_SNDBUF,(const char*)&buf,sizeof(buf)))
	{
		_log(" set socket option error 1 ");
		return FALSE;
	}
	if(SOCKET_ERROR ==setsockopt(sock,SOL_SOCKET ,SO_RCVBUF,(const char*)&buf,sizeof(buf)))
	{
		_log(" set socket option error 2 ");
		return FALSE;
	}
	LINGER	lingerStruct; lingerStruct.l_onoff = 1; lingerStruct.l_linger = 0; 
	if(SOCKET_ERROR ==setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct)))
	{
		_log(" set socket option error 3 ");
		return FALSE;
	}

	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = inet_addr(addr);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);

	if(connect(sock,(LPSOCKADDR)&serverAddress,sizeof(serverAddress)) < 0)
	{
		_log("ERROR : connect [%d,%s]\n",port,addr);
		return FALSE;
	}

	//	SSL 처리 할 것!! 
	char tmpBuff[1024],tmpBuff2[1024];
	int ssize;
	int rsize = recv(sock,	tmpBuff, 1024,0);
	if(rsize < 36)
	{
		_log("rsize < 36 ...");
		shutdown(sock,1);
		closesocket(sock);
		return FALSE;
	}
	ssize = gen_handshake(&ssl,tmpBuff2,1024);
	send(sock,tmpBuff2,ssize,0);
	rsize = process_handshake(&ssl,tmpBuff,rsize);
	if(rsize!=0)
	{
		_log("handShake Error");
		shutdown(sock,1);
		closesocket(sock);
		return FALSE;
	}


	isLoopRecv			=	TRUE;
	hThread = (HANDLE)_beginthread(IOThread,NULL,(void *)this);
	if(hThread==NULL)
	{	_log("ERROR : thread create \n");
		isLoopRecv			=	FALSE;
		return FALSE;
	}
	Sleep(1);
	hThread = (HANDLE)_beginthread(SendThread,NULL,(void *)this);
	if(hThread==NULL)
	{	_log("ERROR : thread create \n");
		isLoopRecv			=	FALSE;
		return FALSE;
	}

	lastsendtime		=	timeGetTime();
	lastrecvtime		=	timeGetTime();

	wBaseSize = basepacketsize;

	return TRUE;
}

void	cSOCKET::kill()
{
	isLoopRecv = FALSE;
	if(sock)
	{	
		closesocket(sock); sock = NULL;
	}
//	WaitForSingleObject(hThread,5000L);
//	if(hThread)		CloseHandle(hThread);
	strcpy(name,"");
//	WSACleanup();
}

BOOL	cSOCKET::SendPacket(char *buf,int size)
{
	CCritical	CS(&csSEND);
	WORD	_wSize = 0,_wType = 0;
	_wSize = *(WORD *)buf;
	_wType = *(WORD *)(buf+2);
	if(packetBuf.wPacketCount>0)
	{
		return packetBuf.addPacket(buf,size);
	}
	if(sendbuf.size+size>dSOCK_MAXBUFFER_SIZE)
	{	
		_log("MAX Packet BUFFER: size[%d] type[0x%x] , _sendSize %d",_wSize,_wType,size);
		return packetBuf.addPacket(buf,size);
	}

	if(_wType<0x10 || _wType>0x9500 || _wSize>=41960 || size==0)
	{	
		_log(" _wType [0x%x] _wSize [%d] size [%d]",_wType,_wSize,size);
		return FALSE;
	}
	memcpy(sendbuf.buf+sendbuf.size,buf,size);
	sendbuf.size += size;
	return TRUE;
}

void	cSOCKET::ReallySend()
{
	CCritical	CS(&csSEND);

	while(packetBuf.wPacketCount>0)
	{
		char	buf[dSOCK_MAXBUFFER_SIZE];
		WORD	wSize = 0;
		packetBuf.getPacket(buf,wSize);
		WORD	wPacketSize = *(WORD *)buf;
		CoreSend(buf,wPacketSize);
//		if(wPacketSize == wSize)	CoreSend(buf,wSize);
//		else	_log("ERROR ReallySend : packetsize = %d , size = %d",wPacketSize,wSize);
	}

	WORD	_wSendAllSize = *(WORD *)sendbuf.buf;
	if(sendbuf.size<sizeof(WORD) || _wSendAllSize>sendbuf.size)		return;	//	size + type

	if(CoreSend(sendbuf.buf,_wSendAllSize))
	{
		if(sendbuf.size - _wSendAllSize>0)
		{
			memcpy(sendbuf.buf,sendbuf.buf + _wSendAllSize, sendbuf.size - _wSendAllSize);
			sendbuf.size -= _wSendAllSize;
		}else
			sendbuf.size  = 0;
	}
	return ;
}

BOOL	cSOCKET::CoreSend(char * data, int size)
{
	WORD	_wSize = 0,_wSendAllSize = 0;
	WORD	_wSendSize = 0;

	//	encrypt


	if(size>=41960)
	{
		_log("ERROR : Size Over [%d]",size);
		sendbuf.size = 0;
		return FALSE;
	}
	char tmpPacket[41960];
	int bytesSend = size;
	if(isUseSSL)
	{
		bytesSend = encrypt_msg(&ssl,tmpPacket,data,dMAX_PACKET_LENGTH,size,AES_ENCRYPT);
	}
	_wSendAllSize = _wSize = bytesSend;

SEND_RETURN:
	_wSendSize = send(sock,tmpPacket+_wSendSize,_wSize,0);
	if(_wSendSize==SOCKET_ERROR || _wSendSize==0 || _wSendSize==0xffff)
	{
		if(WSAEWOULDBLOCK==GetLastError())
		{
			_wSendSize = 0;
		}else{
			WORD	_wType=0;
			_wType = *(WORD *)(data+2);
			_log("ERROR ReallySend : size %d, type %x ,ERROR %d",_wSendAllSize,_wType,WSAGetLastError());
			isLoopRecv = FALSE;
			return FALSE;
		}
	}
	if(_wSendSize<_wSize)
	{
		_wSize -= _wSendSize;
		_log("◇◇◇◇◇ 소켓 반복  ◇◇◇◇◇(sendsize : %d, size : %d - %d)\n",_wSendAllSize,_wSize,_wSendSize);
		goto SEND_RETURN;
	}
	return TRUE;
}

BOOL	cSOCKET::GetRecvPacket(char *recvpacket)
{
	EnterCriticalSection(&csSOCKET);
	if(recvbuf.size<wBaseSize)
	{
		LeaveCriticalSection(&csSOCKET);
		return FALSE;
	}
	
	BOOL	ret = FALSE;
	WORD nextsize=0,nexttype=0;	recvpacket[0] = 0;

	if(isUseSSL)
	{
		nextsize = *(int *)(recvbuf.buf+6) + 10;
	}else
	{
		nextsize = *(WORD *)recvbuf.buf;
		nexttype = *(WORD *)(recvbuf.buf+2);
	}
	int result=0;
 
	if(nextsize>=wBaseSize && recvbuf.size>=nextsize)
	{
		if(isUseSSL)
		{
			decrypt_msg(&ssl,recvpacket,recvbuf.buf,dMAX_PACKET_LENGTH,nextsize,FALSE);
		}else
		{
			memcpy(recvpacket,recvbuf.buf,nextsize);
		}
		if(recvbuf.size-nextsize == 0)
		{
			recvbuf.size = 0;
		}else if(recvbuf.size-nextsize > 0){
			memcpy(recvbuf.buf,recvbuf.buf+nextsize,recvbuf.size-nextsize);
			recvbuf.size-=nextsize;
		}else{
			_log("SERRRO 헉뜨 ~~~ recvbuf.size(%d)-nextsize(%d) = %d",recvbuf.size,nextsize,recvbuf.size-nextsize);
			memset(recvbuf.buf,0,dSOCK_MAXBUFFER_SIZE);
			recvbuf.size = 0;
		}
		ret = TRUE;
	}else
//	if(nexttype<0x10 || nexttype>0x9500 || nextsize>=41960)
	{
		_log("이상한 패킷 .. [0x%x][%d]",nexttype,nextsize);
//		memset(recvbuf.buf,0,dSOCK_MAXBUFFER_SIZE);
//		recvbuf.size = 0;
		LeaveCriticalSection(&csSOCKET);
		return FALSE;
	}

	LeaveCriticalSection(&csSOCKET);
	return ret;
}

void SendThread(void *tmp)
{
	cSOCKET	*soc = (cSOCKET *)tmp;

	while(soc->isLoopRecv)
	{	Sleep(1);
		if(soc->GetSendBufLength()>0)
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
	
	while(soc->isLoopRecv)
	{
		Sleep(1);
		if(ioctlsocket(soc->sock,FIONREAD,&msgSize)==0 && msgSize>0)
		{
			allLen = 0, len = 0;
			EnterCriticalSection(&soc->csSOCKET);
			for(allLen = 0; allLen<(int)msgSize;	allLen += len)
			{
				len = recv(soc->sock,buf,dSOCK_MAXBUFFER_SIZE,0);
				if(len ==0 || len==SOCKET_ERROR)
				{	
					soc->isLoopRecv = FALSE;
					LeaveCriticalSection(&soc->csSOCKET);
					soc->kill();
					_log("◇◇◇◇◇ ㅡㅡ+ in socket thread ◇◇◇◇\n");
					break;
				}
				if( (soc->recvbuf.size+len) > dSOCK_MAXBUFFER_SIZE)
				{	
					_log("IOTHREAD %d + %d > %d ",soc->recvbuf.size,len,dSOCK_MAXBUFFER_SIZE);
					memset(soc->recvbuf.buf,0,dSOCK_MAXBUFFER_SIZE);
					soc->recvbuf.size = 0;
					LeaveCriticalSection(&soc->csSOCKET);
					continue;
				}
				soc->lastrecvtime = timeGetTime();
				memcpy(soc->recvbuf.buf+soc->recvbuf.size,buf,len);
				soc->recvbuf.size+=len;
			}			
//			_log("◆◆◆◆받은 사이즈.. %d \n",soc->recvbuf.size);
			LeaveCriticalSection(&soc->csSOCKET);
		}
	}
	if(soc->sock)
	{
		closesocket(soc->sock); soc->sock = NULL;
	}
	_log("◆◆◆◆◆◆◆◆◆◆◆SOCKET THREAD 종료 ◆◆◆◆◆◆◆◆◆◆◆\n");
	return;
}

