#include "cSOCKET.H"

void			(*cSOCKET::pfReceiveProc)(HWND,char*)	=	NULL;	//	받은 메시지를 처리하는 함수 포인터
HWND			cSOCKET::s_hWnd			=	NULL;
char			cSOCKET::s_strIp[128];

cSOCKET::cSOCKET()
{	m_socket				=	INVALID_SOCKET;			//	소켓 인덱스
	m_isConnected			=	FALSE;					//	서버와 연경된 상태이다.
	m_dwBufferSize			=	0xffff;					//	현재 버퍼 사이즈
	m_pBuffer				=	NULL;	//	메시지 버퍼
	m_pTemplateBuffer		=	NULL;
}

cSOCKET::~cSOCKET()
{
	close();
}

//
//	소켓을 생성하고 연결한다.
//	소켓이 초기화 되어 있어야 한다.
//
BOOL
cSOCKET::connect(char *ip,WORD portnumber)
{
	m_wPortNumber	=	portnumber;
	strcpy_s(m_strServerAddress,ip);
/////////////////////////////////////////////////////////////////////////////////
//	소켓을 생성한다.
/////////////////////////////////////////////////////////////////////////////////

	close();	//	기존에 소켓이 있음 날린다.

	m_pBuffer			=	new char [m_dwBufferSize];	//	메시지 버퍼
	m_pTemplateBuffer	=	new char [m_dwBufferSize];	//	메시지 버퍼
	m_socket			=	socket(AF_INET, SOCK_STREAM, 0);		//	소켓 생성

	if(m_socket == INVALID_SOCKET)		return MSGOUT(WSError(WSAGetLastError()));

	if(WSAAsyncSelect(m_socket,s_hWnd, WM_ASYNC,FD_CONNECT) == SOCKET_ERROR)
		return MSGOUT("error in cAGENT::connect - %s",cSOCKET::WSError(WSAGetLastError()));

//////////////////////////////////////////////////////////////////////////////
//	서버에 연결한다.
//////////////////////////////////////////////////////////////////////////////

	m_socketAddress.sin_family		=	AF_INET;
	m_socketAddress.sin_port		=	htons(m_wPortNumber);
	m_socketAddress.sin_addr.s_addr	=	inet_addr(ip);

	if(::connect(m_socket, (LPSOCKADDR)&m_socketAddress,sizeof(m_socketAddress)) == SOCKET_ERROR) 
	{
		int	iError	=	WSAGetLastError();

		if (iError	!=	WSAEWOULDBLOCK)	
			return MSGOUT("error in cSOCKET::connect - %s",WSError(iError));
	}

	return TRUE;
}	//	cSOCKET::connect(char *ip,WORD portnumber)

//
//	연결이 성공했다.
BOOL
cSOCKET::connect()
{
	if(WSAAsyncSelect(m_socket,s_hWnd, WM_ASYNC,FD_READ|FD_WRITE|FD_CLOSE) == SOCKET_ERROR)
		return MSGOUT("error in cSOCKET::connect - %s",WSError(WSAGetLastError()));

	m_isDisconnected=	FALSE;
	m_isConnected	=	TRUE;

	//	자신의 IP주소 알아내기
	{
		SOCKADDR_IN sockAddr;

		memset(&sockAddr, 0, sizeof(sockAddr));

		int nSockAddrLen=	sizeof(sockAddr);

		BOOL bcbc		=	getsockname(m_socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);

		char *myip		=	inet_ntoa(sockAddr.sin_addr);

		if (myip)	strcpy_s(s_strIp,myip);
		else		strcpy_s(s_strIp,"valid");

		MSGOUT("my ip address : %s",s_strIp);
	}

	return	TRUE;
}	//	cSOCKET::connect()

//
//	소켓과 연결을 끊는다.
BOOL
cSOCKET::close()
{
	m_isConnected				=	FALSE;
	m_isDisconnected			=	FALSE;
	m_dwReceiveBufferPoint		=	0;		//	받은 메시지 길이

	pKILL(m_pBuffer);	//	메시지 버퍼
	pKILL(m_pTemplateBuffer);	//	메시지 버퍼

	if (m_socket	==	INVALID_SOCKET)
		return	FALSE;

	shutdown(m_socket,SD_SEND);

	closesocket(m_socket);

	m_socket					=	INVALID_SOCKET;

	return TRUE;
}	//	cSOCKET::close()

//
//	메시지를 보낸다.
BOOL
cSOCKET::send(char *_lpStream,int _iSize)
{
	if (m_isBlockedSend)	return	TRUE;

	int	iSendSize	=	::send(m_socket,_lpStream,_iSize, 0);

	if (iSendSize	==	SOCKET_ERROR)
	{
		m_isDisconnected	=	TRUE;
		return	MSGOUT("error in cSOCKET::send - %s",cSOCKET::WSError(WSAGetLastError()));
	}

#ifdef _DEBUG
	int	iSize	=	*(WORD *)_lpStream;
	int	iType	=	*(WORD *)(_lpStream+2);

	if (_iSize	!=	iSendSize	||	iSize	!=	iSendSize)
	{
		ERRMSG("센드 사이즈가 이상해효!!","전송 %d,패킷 %d,실제 %d",_iSize,iSize,iSendSize);
	}
	MSGOUT("[0x%x %d=%dbyte]  - %d.%.3d",iType,iSize,iSendSize,timeGetTime()/1000,timeGetTime()%1000);
#endif

	return TRUE;
}	//	cSOCKET::send(char *msg,int size)

//
//	리시브 버퍼 포인트 설정(뒷부분을 앞으로 땡긴다.)
void
cSOCKET::setReceiveBufferPoint(int _iPoint,int _iRemainSize)
{
	memcpy(m_pTemplateBuffer,m_pBuffer+_iPoint,_iRemainSize);
	memcpy(m_pBuffer,m_pTemplateBuffer,_iRemainSize);

	m_dwReceiveBufferPoint	=	_iRemainSize;
}	//	cSOCKET::setReceiveBufferPoint(int _iPoint,int _iRemainSize)

//
//	메시지 받음
BOOL
cSOCKET::receive()
{	
	int		iErr;					//	에러
	DWORD	dwReceiveMsgSize;		//	이번에 받을 메시지 길이
	DWORD	dwSumOfReceiveMsgSize;	//	받은 메시지 길이
	DWORD	dwReceiveBufferPoint	=	m_dwReceiveBufferPoint;

	if (ioctlsocket(m_socket, FIONREAD, &m_dwReceivedMessageSize) != 0	)	
		return	FALSE;
	if (m_dwReceivedMessageSize <= 0									)
		return	FALSE;

	if (m_dwReceivedMessageSize > m_dwBufferSize)	//	받은 버퍼가 리시브 버퍼의 크기보다 크다.
	{
		pKILL(m_pBuffer);
		pKILL(m_pTemplateBuffer);

		m_pBuffer			=	new char [m_dwReceivedMessageSize];
		m_pTemplateBuffer	=	new char [m_dwReceivedMessageSize];
		m_dwBufferSize		=	m_dwReceivedMessageSize;
	}

	for(dwSumOfReceiveMsgSize=0;dwSumOfReceiveMsgSize < m_dwReceivedMessageSize;)
	{
		dwReceiveMsgSize	=	recv(m_socket,m_pBuffer+dwReceiveBufferPoint,m_dwReceivedMessageSize - dwSumOfReceiveMsgSize,0);

		if (dwReceiveMsgSize==SOCKET_ERROR)	//	에러 났다. -_-
		{
			if((iErr = WSAGetLastError()) != WSAEWOULDBLOCK)
			{
				MSGOUT(WSError(iErr));				//	기타 등등의 에러가 났다.

				return	dRECEIVE_DATA_FAILED;
			}
		}
		else
		{
			dwSumOfReceiveMsgSize	+=	dwReceiveMsgSize;	//	이번에 받은 메시지의 총 사이즈
			dwReceiveBufferPoint	+=	dwReceiveMsgSize;	//	이전에 받은 메시지까지 해서 총 사이즈
		}
	}

	m_dwReceivedMessageSize	+=	m_dwReceiveBufferPoint;

	return TRUE;
}	//	cSOCKET::receive()

/////////////////////////////////////////////////////////////////////
//								정적 멤버
/////////////////////////////////////////////////////////////////////

//
//	WinSock 초기화
BOOL
cSOCKET::Init(HWND hwnd)
{	if (!IsWindow(hwnd))	return TRUE;

	s_hWnd	=	hwnd;

	WORD	wVersionRequested = MAKEWORD(2,2);
	WSADATA wsaData;	//	
	int		nError;		//	에러코드
	
	if ((nError = WSAStartup(0x202, &wsaData))!=0)		//	소켓을 초기화한다
	{
		WSACleanup();
		return MSGOUT("error in cSOCKET::Init - %s",WSError(nError));
	}

	MSGOUT("winsock initialized");

	return TRUE;
}	//	cSOCKET::Init(HWND hwnd)

//
//	WinSock 종료
void
cSOCKET::Close()
{
	if (!IsWindow(s_hWnd))	return;
	
	s_hWnd	=	NULL;

	MSGOUT("winsock closed");

	WSACleanup();					//	소켓을 끝장 낸다.	
}	//	cSOCKET::Close()

//
//	특정 도메인의 IP 주소를 얻어 온다. NULL을 넣으면 자기 자신의 IP 주소이다.
char *
cSOCKET::GetIP(char *domain)
{	struct hostent * phost;
	static char IP[128];

	phost	=	gethostbyname (domain);

	if (phost==NULL)
	{	MSGOUT("호스트 연결 에러!!","호스트를 찾을 수 없습니다.");
		return NULL;
	}

	wsprintf(IP,"%d.%d.%d.%d",
				(BYTE)phost->h_addr_list[0][0],
				(BYTE)phost->h_addr_list[0][1],
				(BYTE)phost->h_addr_list[0][2],
				(BYTE)phost->h_addr_list[0][3]);

	return IP;
}	//	cSOCKET::GetIP(char *domain)

//
//	특정 소켓 종료
//
int
cSOCKET::Disconnect(SOCKET _socketIndex)
{	
	return closesocket(_socketIndex);
}	//	cSOCKET::Disconnect(SOCKET SocketIndex)

//
//	소켓에 메시지 보냄
BOOL
cSOCKET::Send(SOCKET _socketIndex,char *_lpStream,int _iSize)
{
	if (::send(_socketIndex,_lpStream,_iSize, 0)==SOCKET_ERROR )
		return	MSGOUT("error in cSOCKET::Send - %s",cSOCKET::WSError(WSAGetLastError()));
				
	return TRUE;
}	//	cSOCKET::Send(SOCKET _socketIndex,char *_lpStream,int _iSize)

///////////////////////////////////////////////////////////////////////////////////
//						소켓 관련 에러 
///////////////////////////////////////////////////////////////////////////////////
char *
cSOCKET::WSError(UINT nErr)
{	switch(nErr)
	{
		case	WSANOTINITIALISED		:
			return "A successful WSAStartup must occur before using this function.";

		case	WSAENETDOWN				:	
			return "The network subsystem has failed.";

		case	WSAEADDRINUSE			:
			return "The socket's local address is already in use and the socket was not marked to allow address reuse with SO_REUSEADDR. This error usually occurs when executing bind, but could be delayed until this function if the bind was to a partially wild-card address (involving ADDR_ANY) and if a specific address needs to be committed at the time of this function.";

		case	WSAEINTR				:
			return "The (blocking) Windows Socket 1.1 call was canceled through WSACancel Blocking Call.";

		case	WSAEINPROGRESS			:
			return "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.";

		case	WSAEALREADY				:
			return "A nonblocking connect call is in progress on the specified socket.";

		case	WSAEADDRNOTAVAIL		:
			return "The remote address is not a valid address (such as ADDR_ANY).";

		case	WSAEAFNOSUPPORT			:
			return "Addresses in the specified family cannot be used with this socket.";

		case	WSAECONNREFUSED			:
			return "Connection refused. No connection could be made because the target machine actively refused it. This usually results from trying to connect to a service that is inactive on the foreign host - i.e. one with no server application running";

		case	WSAEFAULT				:
			return "The name or the namelen parameter is not a valid part of the user address space, the namelen parameter is too small, or the name parameter contains incorrect address format for the associated address family.";

		case	WSAEINVAL				:
			return "The parameter s is a listening socket, or the destination address specified is not consistent with that of the constrained group the socket belongs to.";

		case	WSAEISCONN				:
			return "The socket is already connected (connection-oriented sockets only).";

		case	WSAENETUNREACH			:
			return "The network cannot be reached from this host at this time.";

		case	WSAENOBUFS				:
			return "No buffer space is available. The socket cannot be connected.";

		case	WSAENOTSOCK				:
			return "The descriptor is not a socket.";

		case	WSAETIMEDOUT			:
			return "The connection has been dropped, because of a network failure or because the system on the other end went down without notice.";

		case	WSAEACCES				:
			return "Attempt to connect datagram socket to broadcast address failed because setsockopt option SO_BROADCAST is not enabled.";

		case	WSAEMFILE				:
			return "No more socket descriptors are available.";
			
		case	WSAEPROTONOSUPPORT		:
			return "The specified protocol is not supported.";
			
		case	WSAEPROTOTYPE			:
			return "The specified protocol is the wrong type for this socket.";
			
		case	WSAESOCKTNOSUPPORT		:
			return "The specified socket type is not supported in this address family.";

		case	WSASYSNOTREADY		:
			return	"Indicates that the underlying network subsystem is not ready for network communication.";

		case	WSAVERNOTSUPPORTED	:
			return	"The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation. ";
				
		case	WSAEPROCLIM			:
			return	"Limit on the number of tasks supported by the Windows Sockets implementation has been reached.";

		case	WSAENETRESET :
			return	"The connection has been broken due to the \"keep-alive\" activity detecting a failure while the operation was in progress. ";

		case	WSAENOTCONN :
			return	"The socket is not connected. ";
			
		case	WSAEOPNOTSUPP :
			return	"MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, out-of-band data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations. ";

		case	WSAESHUTDOWN :
			return	"The socket has been shut down; it is not possible to send on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH. ";

		case	WSAEWOULDBLOCK :	//	무시해도 됨
			return	"The socket is marked as nonblocking and the requested operation would block. ";
			
		case	WSAEMSGSIZE :
			return	"The socket is message oriented, and the message is larger than the maximum supported by the underlying transport. ";
			
		case	WSAEHOSTUNREACH :
			return	"The remote host cannot be reached from this host at this time. ";
			
		case	WSAECONNABORTED :
			return	"The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable. ";
			
		case	WSAECONNRESET :
			return	"The virtual circuit was reset by the remote side executing a \"hard\" or \"abortive\" close. For UPD sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a \"Port Unreachable\" ICMP packet. The application should close the socket as it is no longer usable. ";
			
		case	WSAHOST_NOT_FOUND	:	//	호스트가 없다
			return "Authoritive: Host not found";

		case	WSATRY_AGAIN		:	//	호스트가 없다
			return "Non-authoritive: host not found or server failure";

		case	WSANO_RECOVERY		:
			return "Non-recoverable: refused or not implemented";

		case	WSANO_DATA			:
			return "Valid name, no data record for type";

		case	WSAEHOSTDOWN		:	//	호스트가 죽었음
			return "host down";
  }

  return "Unknown error";
}