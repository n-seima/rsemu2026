#include "cAGENT.H"
#include "main.H"


cAGENT::cAGENT()
{
	m_iGameServerPort	=	-1;
}

cAGENT::~cAGENT()
{

}

void
cAGENT::disconnect()
{
	m_socket.close();
}

//
//	초기화
void
cAGENT::reset()
{
	m_isConnected				=	FALSE;
	m_isEventDisconnect			=	FALSE;
	m_isActive					=	FALSE;
	m_strDisconnectText[0]		=	NULL;

	CPacketStatus::reset();	//	초기화
	m_quePacket.reset();		//	패킷 큐 초기화
	disconnect();				//	연결을 끊고..
}	//	cAGENT::reset()

//
//	게임 서버에 연걸(게임 서버 주소를 받았어야 한다.
BOOL
cAGENT::connectToGameServer()
{
	if (m_iGameServerPort	==	-1)	return	FALSE;

	return	connectToServer(m_strGameServerIp,m_iGameServerPort);
}	//	cAGENT::connectToGameServer()

//
//	서버 연결까지 남은 시간.
int
cAGENT::getRemianConnectServerTime()
{
	return	(m_iLimitConnectTime - (timeGetTime()-m_iTryConnectTime))/1000;
}

//
//	연결 결과를 리턴한다.
int	
cAGENT::getConnectServerResult()
{
	if	(timeGetTime() - m_iTryConnectTime > (DWORD)m_iLimitConnectTime)	//	20초가 넘으면 연결을 끊는다.
		return	dCONNECT_STATUS_TIME_OVER;

	if	(m_socket.isConnected())
	{
		m_isConnected	=	TRUE;

		return	dCONNECT_STATUS_CONNECT_OK;
	}

	return	dCONNECT_STATUS_TRYING;
}

//
//	서버에 연결한다.
BOOL
cAGENT::connectToServer(char *_strIp,int _iPortNumber,int _iLimitTime)
{
	reset();

	if (_iPortNumber	==	0xffff)	_iPortNumber	=	dPORT_DUMMY_SERVER;

	if(!m_socket.connect(_strIp,_iPortNumber))	return	FALSE;

	m_isEventDisconnect		=	FALSE;	//	디스컨넥트 이벤트 초기화
	m_isActive				=	TRUE;
	m_iLimitConnectTime		=	_iLimitTime*1000;
	m_iTryConnectTime		=	timeGetTime();

	return	TRUE;
}

//
//	FD_CONNECT 메시지를 받아서 처리
BOOL
cAGENT::connect(HWND _hWnd,SOCKET _socket)
{
	if (m_socket.getSocket()	==	_socket)	return	m_socket.connect();

	return	FALSE;
}

//
//	FD_CLOSE 메시지를 받아서 처리
BOOL
cAGENT::close(SOCKET _socket)
{
	m_isEventDisconnect	=	TRUE;

	if (m_socket.getSocket()	==	_socket)	return	m_socket.close();

	return	FALSE;
}

//
//	FD_READ 메시지를 받아서 처리
BOOL
cAGENT::receive(SOCKET _socket)
{
	if (m_socket.getSocket() !=	_socket)	return	FALSE;

	if (!m_socket.receive())	//	제대로 왔다. 처리해 주자.
	{
		disconnect();	//	연결을 끊고
		m_isEventDisconnect	=	TRUE;

		return	FALSE;
	}

	char	*lpBuffer	=	m_socket.getReceiveBuffer();
	DWORD	dwMsgSize	=	m_socket.getReceivedMessageSize();
	DWORD	dwMsgPos	=	0;

	while(dwMsgPos	<	dwMsgSize)		//	한번에 여러개 뭉쳐서 올수도 있다.
	{
		SERVER_PACKETS*	_lpPacket=	(SERVER_PACKETS*)(lpBuffer+dwMsgPos);

		int	iRemainBufferSize	=	dwMsgSize-dwMsgPos;	//	남은 메시지 버퍼 사이즈

		if (iRemainBufferSize	<	_lpPacket->base.wSize)	//	남은 메시지 버퍼 사이즈가 지금 처리 할 패킷 사이즈 보다 작다.
		{
			m_socket.setReceiveBufferPoint(dwMsgPos,iRemainBufferSize);	//	리시브 버퍼 비우기
			return	TRUE;
		}

		dwMsgPos		+=	_lpPacket->base.wSize;

		m_quePacket.addPacket(_lpPacket);
	}

	m_socket.emptyReceiveBuffer();	//	리시브 버퍼 비우기

	return	TRUE;
}

//
//	패킷 핸들러
BOOL
cAGENT::packetHandler()
{
	SERVER_PACKETS *lpPacket;

	while(1)
	{
		lpPacket	=	popPacket();

		if	(!lpPacket)
			return	TRUE;

		switch(lpPacket->base.wType)
		{
			case	dSL_NOTICE_FOR_LAUNCHER	:
				g_main.receiveNotice((void *)lpPacket);
				break;
		}
	}

	return	FALSE;
}

//
//	이벤트 핸들러..
BOOL
cAGENT::handleWindowMessage(HWND _hWnd,int _iEvent,SOCKET _socket)
{
	switch(_iEvent)
	{	
		case FD_CONNECT		:	// --------------------------- 연결요청 메시지
			connect(_hWnd,_socket);
			break;

		case FD_CLOSE		:	// --------------------------- 소켓 종료 메시지
			close(_socket);
			break;

		case FD_READ		:	// --------------------------- 데이터 수신 메시지
			if	(receive(_socket))	
				return	packetHandler();
			break;

		default:
			break;
	}

	return	_iEvent;
}

//
//	에이전트 초기화
BOOL
cAGENT::init(HWND _hWnd,DWORD _dwLifecodeCycle)
{
	if	(!cSOCKET::Init(_hWnd))
		return	FALSE;		//	윈속 초기화

	reset();

	m_quePacket.reset();							//	패킷 큐 초기화

	return	TRUE;
}

//
//	닫기
void
cAGENT::close()
{
	reset();
	
	cSOCKET::Close();
}
