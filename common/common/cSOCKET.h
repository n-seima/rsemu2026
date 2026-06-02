// cSOCKET.h: interface for the cSOCKET class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_aCSOCKET_H__FD139771_508A_4B9B_8CCA_C34E2B8DAF98__INCLUDED_)
#define AFX_aCSOCKET_H__FD139771_508A_4B9B_8CCA_C34E2B8DAF98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////
//		IOCP를 사용하지 않은 socket class
/////////////////////////////////////////////////////////////////

#include <winsock2.h>
#include <process.h>
#include "RSComm.h"
#include "PacketBuffer.h"

#define	dSOCK_MAXBUFFER_SIZE			dMAX_PACKET_LENGTH_SERVER

class cSOCKET
{
public:
	cSOCKET();
	virtual ~cSOCKET();

	SOCKET				sock;
	WSADATA				wsadata;
	HANDLE				hThread,hSendThread;
	SOCKADDR_IN			serverAddress;
	WORD				wBaseSize;
	WORD				wLastPacket;

	char				name[dNAME_LENGTH];	//	서버 이름으로 구분할거 있으며.. 구분해야쥐..
	char				m_strIp[dIP_SIZE];
	int					m_iPort;
	BOOL	isLinkConnect;
	BOOL	isChannelConnect;
	BOOL	isLoopRecv,isSendLoopRecv;
	BOOL	isReconnect;
	BOOL	isSync;

#ifdef _DEBUG
	DWORD				m_dwTotalSendSize;			//	기준 시간동안 보낸 사이즈
	DWORD				m_dwTotalSendSizeInPacket;	//	기준 시간동안 보낸 사이즈(패킷데이터를 기준)
	DWORD				m_dwTotalSendCount;			//	기준 시간동안 보낸 횟수
	
	DWORD				m_dwTotalRecvSize;			//	기준 시간동안 받은 사이즈
	DWORD				m_dwTotalRecvCount;			//	기준 시간동안 받은 횟수

	DWORD				m_dwSendCheckTime;			//	1분에 한번씩 로그 저장하고 리셋한다.
	DWORD				m_dwRecvCheckTime;			//	1분에 한번씩 로그 저장하고 리셋한다.
#endif

	CPacketBuffer	*	rPacketBuf;
	CPacketBuffer	*	sPacketBuf;

public:
	BOOL	init(int port, char *addr, char *_strName,WORD basepacketsize,BOOL _isSync=FALSE);
	void	kill();

	BOOL	SendPacket(char *,int size);
	void	ReallySend();
	BOOL	CoreSend(char * , int size);
	BOOL	GetRecvPacket(char *);
	void	EmergencyLog();

	int		reConnect();
};

#endif // !defined(AFX_aCSOCKET_H__FD139771_508A_4B9B_8CCA_C34E2B8DAF98__INCLUDED_)
