#ifndef _classAGENT_H
#define _classAGENT_H

#include "cPACKET.H"
#include "cSOCKET.H"

//	POPS - Period Of Packet Send
#define	dPOPS_DEFAULT				5
#define	dPOPS_BUY_ITEM				dPOPS_DEFAULT	//	아이템 구입 메시지 전송 주기 5초
#define	dPOPS_SET_REACTION_STATUS	dPOPS_DEFAULT


#define	dPORT_DUMMY_SERVER		7045

#define	dMAX_PACKET_QUE_COUNT	100

#define	dCONNECT_STATUS_TRYING			0
#define	dCONNECT_STATUS_CONNECT_OK		1
#define	dCONNECT_STATUS_CONNECT_FAILED	2
#define	dCONNECT_STATUS_TIME_OVER		3
#define	dDEFAULT_LIFE_CODE_CYCLE		4*60

//
//	패킷 큐
class cQUE_PACKET
{
	SERVER_PACKETS		m_aPacket[dMAX_PACKET_QUE_COUNT];		//	패킷들..
	int					m_iFirstQue,m_iLastQue,m_iCount;		//	첫번째,마지막 패킷, 쌓여 있는패킷 수
	CRITICAL_SECTION	m_csOperateQue;

public:
						cQUE_PACKET();
						~cQUE_PACKET();

	void				reset();								//	초기화
	BOOL				addPacket(SERVER_PACKETS *_lpPacket);			//	패킷 추가
	BOOL				isReceivedPacket(int _iPacket);			//	받은 패킷이냐?
	SERVER_PACKETS*		popPacket();							//	큐에서 패킷을 빼낸다.
};	//	class cQUE_PACKET

//
//	에이전트에서 관리하는 영웅 데이터
class	CPacketStatus
{
public:
	BOOL				m_isOperateItemData;
	int					m_iSendTradeItemPacketTime;
	int					m_iSendSetReactionPacketTime;	//	리액션 설정 패킷 보낸 시간

	inline	void		reset()
	{
		m_iSendTradeItemPacketTime	=	0;
		m_isOperateItemData			=	FALSE;
		m_iSendSetReactionPacketTime=	0;	//	
	}

};	//	class	CPacketStatus

//
//	게임과 소켓 메시지를 연결해 주는 에이전트
class cAGENT	:	public CPacketStatus
{
	int					m_iGameServerPort;
	char				m_strGameServerIp[20];
	DWORD				m_dwLifecodeCycle;								//	생존-_-코드를 보내는 주기
	DWORD				m_dwLastLifecodeTime;								//	생존-_-코드를 보내는 주기
	cSOCKET				m_socket;
	cQUE_PACKET			m_quePacket;									//	패킷 큐
	BOOL				m_isActive;
	BOOL				m_isConnected;

	BOOL				connect(HWND _hWnd,SOCKET _socket);
	BOOL				close(SOCKET _socket);
	BOOL				receive(SOCKET _socket);				//	메시지를 받는다.

public:
	int					m_iLimitConnectTime,m_iTryConnectTime;			//	서버와 연결 연결 제한 시간,연결 시도를 한 시간.
	BOOL				m_isEventDisconnect;			//	서버에서 짤렸다.
	char				m_strDisconnectText[128];		//	왜 짤렸냐.. -_-a

						cAGENT();
						~cAGENT();

	void				reducePacketTime(int *_iTime,int _iPeriod=dPOPS_DEFAULT);	//	패킷 타임 줄이기
	BOOL				init(HWND _hWnd,DWORD _dwLifecodeCycle=dDEFAULT_LIFE_CODE_CYCLE);//	윈속 초기화하고 각종 패킷 정의
	void				sendLifeCode();							//	생존 코드 전송
	void				close();								//	윈속을 없에고.. 기타등등..
	void				reset();								//	초기화
	BOOL				connectToServer(char *_strIp,int _iPortNumber=0xffff,int _iLimitTime=20);	//	서버에 연결한다.
	BOOL				connectToGameServer();					//	게임 서버에 연결한다.
	inline	void		blocking(){m_socket.blocking();}
	inline	void		releaseBlock(){m_socket.releaseBlock();}

	inline	BOOL		isActive(){return m_isActive;}			//	살아있냐?
	inline	BOOL		isConnected()
	{
		if (!m_socket.isConnected())	return	FALSE;

		return	m_isConnected;
	}	//	연결 되어 있냐?

	inline	BOOL		isDisconnected()
	{	
		if	(m_isEventDisconnect)
		{
			return	TRUE;
		}
		
		if (m_socket.isDisconnected())	//	디스 됐냐?
		{
			return	TRUE;
		}

		return	FALSE;
	}

	void				disconnect();							//	연결을 종료한다.

	int					getRemianConnectServerTime();			//	서버 연결까지 남은 시간.
	int					getConnectServerResult();						//	서버 연결 결과

	int					handleWindowMessage(HWND _hWnd,int _iEvent,SOCKET _socket);	//	패킷 받는다.

	inline	SERVER_PACKETS*	popPacket(){return	m_quePacket.popPacket();}	//	받은 패킷을 리턴~~
	BOOL				packetHandler();							//	패킷 핸들러

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃클라이언트->서버 메시지 정의
//┃
	inline	BOOL	sendPacket(char *_lpPacketData,int _iSize)
	{
		if (m_isActive)	return	m_socket.send(_lpPacketData,_iSize);

		return	FALSE;
	}
//┃
//┃클라이언트->서버 메시지 정의
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};	//	class cAGENT

extern	cAGENT	g_agent;

#endif