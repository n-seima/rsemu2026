/*
	작성일	:	1999.12.19
	개  요	:	소켓
	설	명	:	소켓이다!! ... 그런 눈으로 보지 말어... 뜨거워지잖아... -_-;;
*/

#ifndef _classSOCKET_H
#define _classSOCKET_H

#include "SFC.H"

#pragma comment(lib,"wsock32.lib")			// Windows Socket Library

#define	dADDRESS_FAMILY		AF_INET
#define WM_ASYNC     		(WM_USER + 1)	//	소켓 메시지..
#define WM_DISCONNECTED		(WM_USER + 2)	//	소켓 메시지..

#define	dRECEIVE_DATA_FAILED		0
#define	dRECEIVE_DATA_SUCCESS		1
#define	dRECEIVE_DATA_REMAIN_PACKET	2

class cSOCKET
{
protected:
	BOOL						m_isConnected;				//	서버와 연경된 상태이다.
	DWORD						m_dwReceivedMessageSize;	//	이번에 받은 메시지 사이즈
	DWORD						m_dwReceiveBufferPoint;		//	리시브 버퍼 포인트
	WORD						m_wPortNumber;
	SOCKADDR_IN					m_socketAddress;
	char						m_strServerAddress[255];
	DWORD						m_dwBufferSize;					//	현재 버퍼 사이즈
	char						*m_pBuffer,*m_pTemplateBuffer;						//	메시지 버퍼
	BOOL						m_isDisconnected;
	BOOL						m_isBlockedSend;

public:
								cSOCKET();
								~cSOCKET();

	SOCKET 						m_socket;					//	소켓 인덱스

	BOOL						connect(char *serveraddress,WORD portnumber);	//	소켓 생성 및 서버에 연결
	BOOL						connect();										//	소켓이 연결 되었다.
	BOOL						close();										//	연결 끊기
	BOOL						send(char *msg,int size);						//	메시지 보내기
	BOOL						receive();										//	리시브 버퍼
	inline	SOCKET				getSocket()		{return	m_socket;}
	inline	BOOL				isConnected()	{return	m_isConnected;}
	inline	BOOL				isDisconnected()	{return	m_isDisconnected;}
	inline	void				blocking()		{m_isBlockedSend	=	TRUE;}
	inline	void				releaseBlock()	{m_isBlockedSend	=	FALSE;}

	inline	char*				getReceiveBuffer(){return	m_pBuffer;}
	inline	DWORD				getReceivedMessageSize(){return	m_dwReceivedMessageSize;}
	inline	void				emptyReceiveBuffer(){m_dwReceiveBufferPoint	=	0;}
	void						setReceiveBufferPoint(int _iPoint,int _iRemainSize);	//	리시브 버퍼 포인트 설정(뒷부분을 앞으로 땡긴다.)

	static	HWND				s_hWnd;								//	소켓 윈도우 핸들
	static	char				s_strIp[128];						//	IP 주소

	static	BOOL				Init(HWND hwnd);					//	소켓 초기화
	static	void				Close();							//	소켓 끝냄..
	static	BOOL				Send(SOCKET SocketIndex,char *msg,int size);	//	메시지 보내기
	static	BOOL				Disconnect(SOCKET SocketIndex);		//	연결 끊기
	static	BOOL				ReceiveMsg(SOCKET socket);
	static	char				*WSError(UINT nErr);
	static	char *				GetIP(char *domain);


	static	void				(*pfReceiveProc)(HWND,char*);			//	받은 메시지를 처리하는 함수 포인터
};

#endif