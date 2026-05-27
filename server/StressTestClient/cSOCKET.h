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
#include "ssl.h"

#define	dSOCK_MAXBUFFER_SIZE			4096
#define	dPACKET_BUFFER_SIZE				100*1024

class	cPacketBuff2
{
public:
	char	binData[dPACKET_BUFFER_SIZE];
	WORD	wPacketCount;
	DWORD	dwLoadPos,dwSavePos;
	CRITICAL_SECTION	cs;

				cPacketBuff2()	{	InitializeCriticalSection(&cs);	reset();	}
				~cPacketBuff2()	{	DeleteCriticalSection(&cs);					}
	inline void	reset()			
	{	
		memset(binData,0,sizeof(cPacketBuff2));	dwLoadPos = 0;	dwSavePos = 0;	wPacketCount = 0;	
	}
	BOOL	getPacket(void * data, WORD wSize)
	{	CCritical	CS(&cs);
		if(wPacketCount<=0)		return FALSE;
		memcpy(&wSize,binData+dwLoadPos,sizeof(WORD));
		memcpy(data,binData+dwLoadPos+sizeof(WORD),wSize);
		dwLoadPos += sizeof(WORD)+wSize;
		if(wSize == 0)	reset();
		else
		{
			wPacketCount--;
			if(wPacketCount == 0 )	reset();
		}
		return TRUE;
	}

	BOOL	addPacket(void * data, WORD	wSize)
	{	
		CCritical	CS(&cs);

		if(wSize + dwSavePos >= dPACKET_BUFFER_SIZE)
			return FALSE;

		memcpy(binData+dwSavePos,&wSize,sizeof(WORD));
		memcpy(binData+dwSavePos+sizeof(WORD),data,wSize);

		dwSavePos += wSize+sizeof(WORD);
		wPacketCount++;

		return TRUE;
	}
};


typedef struct __sendbuf{
	int		size;					//	버퍼에 대기중인 사이즈
	char	buf[dSOCK_MAXBUFFER_SIZE];	//	보낼 데이타 넣어둘 버퍼 
}_sendbuf;

typedef struct __recvbuf{
	int		size;					//	버퍼에 대기중인 사이즈
	char	buf[dSOCK_MAXBUFFER_SIZE];	//	받을 데이타 넣어둘 버퍼
}_recvbuf;

class cSOCKET
{
public:
	cSOCKET(BOOL	isUsessl = FALSE);
	virtual ~cSOCKET();

	BOOL				isUseSSL;
	struct	SSLSESSION	ssl;
	SOCKET				sock;
	WSADATA				wsadata;
	HANDLE				hThread;
	SOCKADDR_IN			serverAddress;
	CRITICAL_SECTION	csSOCKET;
	CRITICAL_SECTION	csSEND;
	WORD				wBaseSize;

	_sendbuf			sendbuf;
	_recvbuf			recvbuf;
	cPacketBuff2		packetBuf;

	char	name[dNAME_LENGTH];			//	서버 이름으로 구분할거 있으며.. 구분해야쥐..
	int		lastsendtime;
	int		lastrecvtime;
	BOOL	isLinkConnect;
	BOOL	isChannelConnect;
	BOOL	isLoopRecv;

	BOOL	init(int	port,	char	*addr, WORD basepacketsize);
	void	kill();

	inline int		GetRecvBufLength(){	return recvbuf.size;	}
	inline int		GetSendBufLength(){	return sendbuf.size;	}

	BOOL	SendPacket(char *,int size);
	void	ReallySend();
	BOOL	CoreSend(char * , int size);
	BOOL	GetRecvPacket(char *);
};

#endif // !defined(AFX_aCSOCKET_H__FD139771_508A_4B9B_8CCA_C34E2B8DAF98__INCLUDED_)
