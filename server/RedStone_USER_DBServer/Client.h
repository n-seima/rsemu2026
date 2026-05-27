// Client.h: interface for the CClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)
#define AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <mmsystem.h>
#include "cPACKET_USERDBSERVER.h"
#include "definePacketData.h"


typedef struct _IO_DATA{
	OVERLAPPED	OL;
	WSABUF		buf;
	DWORD		sendlen;
	char		Buffer[dMAX_PACKET_LENGTH_SERVER];
	int			IOState;	//	0 - recv , 1 - send
}IO_DATA,*LP_IO_DATA;


class	CClient  
{
//	추가될 내용.. 자신의 채널, 게임(서버 정보 및 방 정보) 시리얼..
private:
	SOCKET				sock;		//	소켓 번호 
	BOOL				isUsed;		//	사용중인가?
	BOOL				isClose;	//	소켓이 닫혀 있나? preclose하면 TRUE 된다.
	char				id[dWORLD_NAME_LENGTH];	//	WORLD서버의 이름
	char				ip[dIP_SIZE];	//	WORLD서버의 IP
	WORD				wUserCount;		//	WORLD서버에 접속한 총 인원
	WORD				wType;			//	WORLD서버의 타입 0 - NORMAL , 1 - TEST
	WORD				m_bf1IsInnerOpen	:	1;
	WORD				wMessage;
	WORD				wIsSendList;	//	한번 리스트를 보냈던넘인가. 0, 1
	
	CRITICAL_SECTION	csUSER;
	CRITICAL_SECTION	csSendBuffer;
	CRITICAL_SECTION	csRECVBUFFER;
	CRITICAL_SECTION	csSENDBUFFER;

public:
	//	about Packet Buffer
	WORD				wServerIndex;	//	서버 인덱스
	WORD				wServerListIndex;//	로그인 서버 리스트 상에서의 인덱스
	DWORD				serial;
	IO_DATA				rOL,sOL;
	BOOL				isSending;

	int					lastSendPacket;
	int					MAX_LENGTH;
	int					sbuf_length;
	int					rbuf_length;
	char				lpSendBuf[dMAX_PACKET_LENGTH_SERVER];
	char				lpRecvBuf[dMAX_PACKET_LENGTH_SERVER];
	DWORD				timegap;
	LPBYTE				lpTmpBuf;

	char				sendpacket[dMAX_PACKET_LENGTH_SERVER];
	WORD				sendpacket_size;

						CClient(int	buffersize=dMAX_PACKET_LENGTH_SERVER);
	virtual				~CClient();
	void				Open();
	void				PreClose();
	void				Close();
	void				Kill();
	void				CSReset();
	//	일반 Get & Set Method 
	inline BOOL			GetIsClose(){		return isClose;					}
	inline void			UsedUser(){		EnterCriticalSection(&csUSER);		}
	inline void			NotUsedUser(){	LeaveCriticalSection(&csUSER);		}
	inline void			UsedBuffer(){EnterCriticalSection(&csSendBuffer);	}
	inline void			NotUsedBuffer(){LeaveCriticalSection(&csSendBuffer);}

	inline SOCKET		GetSocket()				{	return sock;		}
	inline BOOL			GetIsUsed()				{	return isUsed;		}
	inline DWORD		GetSerial()				{	return serial;		}
	inline char		*	GetID()					{	return id;			}
	inline char		*	GetIP()					{	return ip;			}
	inline WORD			GetUserCount()			{	return wUserCount;	}
	inline WORD			GetType()				{	return wType;		}
	inline BOOL			IsInnerOpen()			{	return m_bf1IsInnerOpen;		}
	inline WORD			GetMessage()			{	return wMessage;		}
	inline void			SetSocket(SOCKET	soc){ if(sock){ closesocket(sock); sock=NULL;}	sock = soc;}
	inline void			SetSerial(DWORD	ser)	{	serial = ser;		}
	inline void			SetType(WORD t)			{	wType = t;			}
	inline void			SetInnerOpen(BOOL _bIsInnerOpen)	{	m_bf1IsInnerOpen=_bIsInnerOpen;		}

	inline void			SetMessage(WORD m)		{	wMessage	= m;			}
	inline int			GetSendBufLength()		{	return sbuf_length;	}
	inline int			GetRecvBufLength()		{	return rbuf_length;	}
	inline int			GetTimeGap()			{	return timegap;		}
	inline int			GetIsSendList()			{	if(wIsSendList==0){	wIsSendList=1;	return 0;}else{	return 1;}	}

	void				SetIsUsed(BOOL isused);
	void				SetID(char *nid);
	void				SetIP(char *nip);
	void				SetUserCount(WORD count);
	void				SetServerIndex(WORD _idx);	//	wServerIndex
	void				SetServerListIndex(WORD _idx);	//	wServerIndex
	int					GetServerIndex()		{	return wServerIndex;	}
	int					GetServerListIndex()	{	return wServerListIndex;	}
	BOOL				isWaste();

	//	return : 0 - 여유공간이 없어서 추가불가능
	//			 1 - 정상 
	int					AddRecvPacket(char *data,int size);	//	packet을 더한다.
	int					AddRecvPacket(int size);			//	packet을 더한다.
	int					AddSendPacket(char *data,int size);	//	packet을 더한다.  필요 없어서 삭제한다.

	//	return : NULL - 추가되어 있는 패킷이 없다.
	int					GetRecvPacket(char *);	//	받은 패킷을 빼내온다.
	int					CompleteSendPacket(int size);
};

class	CClientManager{
private:

	CClient				User[dUSERDB_MAX_USER_COUNT];
	WORD				wNext, wCount;
	CRITICAL_SECTION	csDeque;

	CClientManager();

public:
	virtual				 ~CClientManager();
	static CClientManager & GetInstance();	//	항상 동일한 인스턴스를 준다.

	inline	int			Size(){	return wCount;	}

	void				SetBaseSize(int bs);
	void				Init();				//	클라이언트 정보 초기화
	void				End();				//	모든 내용을 정리한다.
	int					Add();				//	유저를 추가하고 시리얼을 돌려준다.
	void				Del(int idx);		//	해당 유저를 삭제한다.
	WORD				Next();				//	다음에 넣을 자리를 찾는다. 없으면 0xffff
	CClient			*	GetClientPnt(char *);	//	ID로 CClient 포인터 얻기
	CClient			*	GetClientPnt(int idx,BOOL	isSerial = TRUE);	//	idx로 CClient 포인터 얻기
};

class _CERT_DATA
{
public:
	DWORD		dwCode;
	DWORD		dwTime;
	char		strId[dID_LENGTH];
	char		strPw[dPASS_LENGTH];

	_CERT_DATA(){}
	~_CERT_DATA(){}
	inline void init()
	{	dwCode = 0xffffffff;	dwTime = 0xffffffff;	memset(strId,0,dID_LENGTH);	memset(strPw,0,dPASS_LENGTH);
	}
	BOOL isWaste();
};

class	cCertificationList
{
public:
	CRITICAL_SECTION	csDeque;

	_CERT_DATA			User[100000];
	DWORD				m_wNext,m_wCount;
	inline int			Size(){	return m_wCount;	}
	DWORD				NextNext();

	cCertificationList();
	~cCertificationList();

	DWORD				add(char * _strId,char * _strPw);
	void				get(_CERT_DATA	* _data,char * _strId,DWORD _code);
};

extern	cCertificationList		CertManager;


#endif // !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)
