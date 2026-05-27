/*
	작성일	:	1999.12.19
	개  퓖E:	소켓
	설	툈E:	소켓이다!! ... 그런 눈으로 보햨E말푳E.. 뜨거워지잖아... -_-;;
*/

#ifndef _classSOCKET_H
#define _classSOCKET_H

#include "SFC.H"

#pragma comment(lib,"wsock32.lib")			// Windows Socket Library

#define	dADDRESS_FAMILY		AF_INET
#define WM_ASYNC     		(WM_USER + 1)	//	소켓 메시햨E.
#define WM_DISCONNECTED		(WM_USER + 2)	//	소켓 메시햨E.

#define	dRECEIVE_DATA_FAILED		0
#define	dRECEIVE_DATA_SUCCESS		1
#define	dRECEIVE_DATA_REMAIN_PACKET	2

class cSOCKET
{
protected:
	BOOL						m_isConnected;				//	서버와 연경된 상태이다.
	DWORD						m_dwReceivedMessageSize;	//	이번에 받은 메시햨E사이햨E	
	DWORD						m_dwReceiveBufferPoint;		//	리시틒E버퍼 포인트
	WORD						m_wPortNumber;
	SOCKADDR_IN					m_socketAddress;
	char						m_strServerAddress[255];
	DWORD						m_dwBufferSize;					//	현픸E버퍼 사이햨E	
	char						*m_pBuffer,*m_pTemplateBuffer;						//	메시햨E버퍼
	BOOL						m_isDisconnected;
	BOOL						m_isBlockedSend;

public:
								cSOCKET();
								~cSOCKET();

	SOCKET 						m_socket;					//	소켓 인덱스

	BOOL						connect(char *serveraddress,WORD portnumber);	//	소켓 생성 및 서버에 연컖E	
	BOOL						connect();										//	소켓이 연컖E되었다.
	BOOL						close();										//	연컖E끊콅E	
	BOOL						shutDown();										//	셧 다퓖E	
	BOOL						send(char *msg,int size);						//	메시햨E보내콅E	
	BOOL						receive();										//	리시틒E버퍼
	inline	SOCKET				getSocket()			{return	m_socket;}
	inline	BOOL				isConnected()		{return	m_isConnected;}
	inline	BOOL				isDisconnected()	{return	m_isDisconnected;}
	inline	void				blocking()			{m_isBlockedSend	=	TRUE;}
	inline	void				releaseBlock()		{m_isBlockedSend	=	FALSE;}
	inline	BOOL				isBlockedToSend()	{return	m_isBlockedSend;}

	inline	char*				getReceiveBuffer(){return	m_pBuffer;}
	inline	DWORD				getReceivedMessageSize(){return	m_dwReceivedMessageSize;}
	inline	void				emptyReceiveBuffer(){m_dwReceiveBufferPoint	=	0;}
	void						setReceiveBufferPoint(int _iPoint,int _iRemainSize);	//	리시틒E버퍼 포인트 설정(뒷부분을 앞으로 땡긴다.)

	static	HWND				s_hWnd;								//	소켓 윈도퓖E핸탛E	
	static	char				s_strIp[128];						//	IP 주소

	static	BOOL				Init(HWND hwnd);					//	소켓 초기화
	static	void				Close();							//	소켓 끝냄..
	static	BOOL				Send(SOCKET SocketIndex,char *msg,int size);	//	메시햨E보내콅E	
	static	BOOL				Disconnect(SOCKET SocketIndex);		//	연컖E끊콅E	
	static	BOOL				ReceiveMsg(SOCKET socket);
	static	char				*WSError(UINT nErr);
	static	char *				GetIP(char *domain);


	static	void				(*pfReceiveProc)(HWND,char*);			//	받은 메시지를 처리하는 함펯E포인터
};

#endif