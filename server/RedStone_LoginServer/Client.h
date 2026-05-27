// Client.h: interface for the CClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)
#define AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable :4786)

#include <winsock2.h>
#include <windows.h>
#include <mmsystem.h>
#include "RSComm.h"
#include "cPACKET_BASE.h"
#include "assert.h"
#include "PacketBuffer.h"
#include "cSRVUTIL.h"


#include <map>

extern DWORD	g_receiveSocketCount;
extern CRITICAL_SECTION	csKILLSOCKET;
extern SOCKET	lisSock;

typedef struct _IO_DATA{
	OVERLAPPED	OL;
	WSABUF		buf;
	DWORD		sendlen;
	char		Buffer[dMAX_PACKET_LENGTH];
	int			IOState;	//	0 - recv , 1 - send
}IO_DATA,*LP_IO_DATA;

typedef struct
{
	char	name[dNAME_LENGTH];
	int		lev;
	WORD	job,lastfield;
}stAvatarDummyList;

class CClient  
{
//	추가될 내용.. 자신의 채널, 게임(서버 정보 및 방 정보) 시리얼..
public:
	CRITICAL_SECTION	csUSER;
	SOCKET				sock;		//	소켓 번호 
	BOOL				isUsed;		//	사용중인가?
	BOOL				isClose;	//	소켓이 닫혀 있나? preclose하면 TRUE 된다.
	DWORD				serial;		//	서버에게서 받은 시리얼
	char				id[dID_LENGTH];
	DWORD				dwHashIdCode;
	char				SelectServerName[dNAME_LENGTH];

	char				tmpid[dID_LENGTH];
	char				ip[dIP_SIZE];
	IO_DATA				rOL,sOL;
	BOOL				isConnectGame;
	int					MAX_LENGTH;
	DWORD				timegap,gapcheckcount;
	DWORD				loginTime;
	WORD				wSelectedServerIndex;
	stAvatarDummyList	list[dMAX_AVATAR_COUNT];

	CPacketBuffer	*	rBuffer;
	CPacketBuffer	*	sBuffer;
	char				sendpacket[dMAX_PACKET_LENGTH];
	WORD				sendpacket_size;

	WORD				m_wClientType;		//	클라이언트 타입(런쳐,더미,게임 유저)
	WORD				m_wSecurityCode;	//	패킷 유효성 검사를 위한 비밀 코드
	WORD				m_wCheckBit;
	BOOL				m_bOtp;				//	OTP인증을 사용하는 유저인가?

						CClient(int	buffersize=dMAX_PACKET_LENGTH);
	virtual				~CClient();
	void				Open();
	void				PreClose();
	void				Close();
	void				Kill();
	void				CSReset();
	//	일반 Get & Set Method 
	inline BOOL			GetIsClose(){		return isClose;					}

	inline SOCKET		GetSocket()	{	return sock;	}
	inline BOOL			GetIsUsed()	{	return isUsed;	}
	inline DWORD		GetSerial()	{	return serial;	}
	inline char		*	GetID()		{		return id;	}
	inline char		*	GetIP()		{		return ip;	}
	inline char		*	GetSelectServer(){	return SelectServerName;	}
	inline void			SetSocket(SOCKET	soc){	if(sock){ shutdown(sock,1);	closesocket(sock);sock = NULL;} sock = soc;}
	inline void			SetSerial(DWORD	ser){	serial =ser;	}
	inline void			SetID(char *nid){	memcpy(id,nid,dID_LENGTH);	}
	inline void			SetSelectServer(char *wn){	strcpy(SelectServerName,wn);}
	inline int			GetTimeGap(){	return timegap;	}
	inline void			SetIsUsed(BOOL isused){	isUsed = isused;	}

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

class DummyClient
{
//	추가될 내용.. 자신의 채널, 게임(서버 정보 및 방 정보) 시리얼..
public:
	char				id[dID_LENGTH];
	DWORD				time;

	DummyClient(){		time = 0;	strcpy(id,"");	}
};

#define dLISTSENDDATA_MAXCOUNT	10000

class cLoginInfo	//	로그인 문제를 해결하기 위해 패킷 처리 과정을 담아보자.
{
public:
	DWORD	m_dwSerial;		//	서버에게서 받은 시리얼
	DWORD	m_dwHashIdCode;
	char	m_strId[dID_LENGTH];
	int		m_dwRegTime;
	int		m_dwSendTime;
	int		m_dwRecvTime;
	WORD	m_wSendPacketType;
	WORD	m_wRecvPacketType;

	cLoginInfo()
	{
	}
	cLoginInfo(DWORD _serial, char * _strId)
	{
		setInfo(_serial,_strId);
	}

	byte operator==(DWORD _hash)
	{
		if( _hash == m_dwHashIdCode )	return TRUE;
		else							return FALSE;
	}

	void operator=(cLoginInfo info)
	{
		memcpy(this,&info,sizeof(cLoginInfo));
	}

	void setInfo(DWORD _serial, char * _strId)
	{
		memset(this,0x00,sizeof(cLoginInfo));
		m_dwSerial = _serial;
		strncpy(m_strId,_strId, dID_LENGTH);
		m_strId[dID_LENGTH - 1] = 0;
		m_dwHashIdCode = cSRVUTIL::GetHashCode((BYTE *)m_strId);

		m_dwRegTime = cSRVUTIL::GetTimes();
	}

	void setSendData(WORD _packetType)
	{
		m_wSendPacketType = _packetType;
		m_wRecvPacketType = 0;
		m_dwSendTime = cSRVUTIL::GetTimes();
		m_dwRecvTime = 0;
	}

	void setRecvData(WORD _packetType)
	{
		m_wRecvPacketType = _packetType;
		m_dwRecvTime = cSRVUTIL::GetTimes();
	}
};

typedef enum
{
	eSEND_DATA,
	eRECV_DATA,
}ePACKETTYPE;



class cLoginInfoList
{
private:
	std::map <DWORD, cLoginInfo > m_mapList;
	
public:
	cLoginInfoList();
	~cLoginInfoList();

	void release();

	BOOL add(DWORD _serial, char * _strId);
	BOOL remove(char * _strId, BOOL isWriteLog = FALSE)
	{
		DWORD hash = cSRVUTIL::GetHashCode((BYTE *)_strId);
		return remove(hash, isWriteLog);

	}
	BOOL remove(DWORD _hash, BOOL isWriteLog = FALSE);

	BOOL setData(char * _strId, ePACKETTYPE _type, WORD _wType)
	{
		DWORD hash = cSRVUTIL::GetHashCode((BYTE *)_strId);
		return setData(hash, _type,_wType);
	}
	BOOL setData(DWORD _hash, ePACKETTYPE _type, WORD _wType);
};

class	CClientManager{
public:
	CClientManager();
	CClient				User[dLOGIN_MAX_USER_COUNT];
	DWORD				wNext, wCount;
	CRITICAL_SECTION	csDeque;

	virtual				 ~CClientManager();

	inline	int			Size(){	return wCount;	}
	void				SetBaseSize(int bs);

	void				Init();				//	클라이언트 정보 초기화
	void				End();				//	모든 내용을 정리한다.
	DWORD				Add();				//	유저를 추가하고 시리얼을 돌려준다.
	void				Del(DWORD idx);		//	해당 유저를 삭제한다.
	DWORD				Next();				//	다음에 넣을 자리를 찾는다. 없으면 0xffff
	CClient			*	GetClientPnt(char *);	//	ID로 CClient 포인터 얻기
	CClient			*	GetClientPnt(DWORD idx,BOOL	isSerial = TRUE);	//	idx로 CClient 포인터 얻기
	void				test(char *name);

	//	소켓 처리를 위해 필요한 리스트 처리.
	WORD			*	ListSendData;	//	dLISTSENDDATA_MAXCOUNT
	WORD				wNextList, wCountList,wCurrent;
	CRITICAL_SECTION	csDequeList;

	void	ListUp(WORD _serial);
	WORD	GetList();
};
//	소켓이 0xcccccccc될때를 찾아라!!
extern CClientManager	UserManager;

#endif // !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)
