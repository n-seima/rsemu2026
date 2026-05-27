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
#include <deque>
#include <algorithm>
#include "cPACKET_TOOLLOGSERVER.h"
#include "PacketBuffer.h"

#define dLOG_MAX_USER_COUNT		128

#include "RSComm.h"

using namespace std;

typedef struct _IO_DATA{
	OVERLAPPED	OL;
	WSABUF		buf;
	DWORD		sendlen;
	char		Buffer[dMAX_PACKET_LENGTH];
	int			IOState;	//	0 - recv , 1 - send
}IO_DATA,*LP_IO_DATA;


class	CClient  
{
//	추가될 내용.. 자신의 채널, 게임(서버 정보 및 방 정보) 시리얼..
private:
	SOCKET				sock;		//	소켓 번호 
	BOOL				isClose;	//	소켓이 닫혀 있나? preclose하면 TRUE 된다.
	DWORD				serial;
	char				id[dID_LENGTH];
	
	CRITICAL_SECTION	csUSER;

public:
	//	about Packet Buffer
	IO_DATA				rOL,sOL;
	int					MAX_LENGTH;
	DWORD				timegap;
	CPacketBuffer	*	rBuffer;
	CPacketBuffer	*	sBuffer;
	char				sendpacket[dMAX_PACKET_LENGTH];
	WORD				sendpacket_size;
	BOOL				isUsed;		//	사용중인가?


						CClient(int	buffersize=dMAX_PACKET_LENGTH);
	virtual				~CClient();
	void				Open();
	void				Close();
	void				Kill();
	//	일반 Get & Set Method 
	inline BOOL			GetIsClose()		{		return isClose;			}
	inline SOCKET		GetSocket()			{		return sock;			}
	inline BOOL			GetIsUsed()			{		return isUsed;			}
	inline DWORD		GetSerial()			{		return serial;			}
	inline char		*	GetID()				{		return id;				}
	inline void			SetSocket(SOCKET soc){ if(sock){ closesocket(sock); sock=NULL;}	sock = soc;	}
	inline void			SetSerial(DWORD	ser){		serial = ser;			}
	inline int			GetTimeGap()		{		return timegap;			}
	
	void				SetID(char *nid);

	//	return : 0 - 여유공간이 없어서 추가불가능
	//			 1 - 정상 
	int					AddRecvPacket(char *data,int size);
	int					AddRecvPacket(int size);
	int					AddSendPacket(char *data,int size);

	//	return : NULL - 추가되어 있는 패킷이 없다.
	int					GetRecvPacket(char *);	//	받은 패킷을 빼내온다.
	int					CompleteSendPacket(int size);
};

class	CClientManager{
private:
	CRITICAL_SECTION	csDeque;

public:
						CClientManager();
	virtual				 ~CClientManager();

	CClient				User[dLOG_MAX_USER_COUNT];
	WORD				m_wNext,m_wCount;

	inline int			Size(){	return m_wCount;	}
	WORD				NextNext();

	void				SetBaseSize(int bs);
	void				Init();				//	클라이언트 정보 초기화
	void				End();				//	모든 내용을 정리한다.
	
	int					Add();				//	유저를 추가하고 시리얼을 돌려준다.
	void				Del(int idx);			//	해당 유저를 삭제한다.
	CClient			*	GetClientPnt(char *);	//	ID로 CClient 포인터 얻기
	CClient			*	GetClientPnt(WORD idx);	//	idx로 CClient 포인터 얻기
};

#endif // !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)
