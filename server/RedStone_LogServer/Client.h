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
#include "cPACKET_LOGSERVER.h"
#include "PacketBuffer.h"

#define dLOG_MAX_USER_COUNT		128

#include "RSComm.h"

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
	BOOL				isUsed;		//	사용중인가?
	BOOL				isClose;	//	소켓이 닫혀 있나? preclose하면 TRUE 된다.
	char				id[dID_LENGTH];
	
	CRITICAL_SECTION	csUSER;

public:
	//	about Packet Buffer
	SOCKET				sock;		//	소켓 번호 
	DWORD				serial;
	WORD				wServerIndex;
	IO_DATA				rOL,sOL;
	int					MAX_LENGTH;
	DWORD				timegap;
	LPBYTE				lpTmpBuf;
	CPacketBuffer	*	rBuffer;
	CPacketBuffer	*	sBuffer;

	char				sendpacket[dMAX_PACKET_LENGTH_SERVER];
	WORD				sendpacket_size;

						CClient(int	buffersize=dMAX_PACKET_LENGTH_SERVER);
	virtual				~CClient();
	void				Open();
	void				PreClose();
	void				Close();
	void				Kill();
	//	일반 Get & Set Method 
	inline BOOL			GetIsClose(){		return isClose;							}

	inline SOCKET		GetSocket()			{		return sock;			}
	inline BOOL			GetIsUsed()			{		return isUsed;			}
	inline DWORD		GetSerial()			{		return serial;			}
	inline char		*	GetID()				{		return id;				}
	inline void			SetSocket(SOCKET soc){ if(sock){ closesocket(sock); sock=NULL;}	sock = soc;	}
	inline void			SetSerial(DWORD	ser){		serial = ser;			}
	inline int			GetTimeGap()		{		return timegap;			}
	
	void				SetIsUsed(BOOL isused);
	void				SetID(char *nid);
	BOOL				isWaste();

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
	CClient				User[dLOG_MAX_USER_COUNT];
	CRITICAL_SECTION	csDeque;	//	used, unused를 같이 가둬라.	좀 느려도 별수없다. 안정 안정.. 환  .. 꺄 >.<
	WORD				wNext, wCount;

	CClientManager();

public:
	virtual				 ~CClientManager();
	static CClientManager & GetInstance();	//	항상 동일한 인스턴스를 준다.

	inline	int			Size(){	return wCount;	}

	WORD				Next();				//	다음에 넣을 자리를 찾는다. 없으면 0xffff
	void				SetBaseSize(int bs);
	void				Init();				//	클라이언트 정보 초기화
	void				End();				//	모든 내용을 정리한다.
	int					Add();				//	유저를 추가하고 시리얼을 돌려준다.
	void				Del(int idx);			//	해당 유저를 삭제한다.
	CClient			*	GetClientPnt(char *);	//	ID로 CClient 포인터 얻기
	CClient			*	GetClientPnt(int idx,BOOL	isSerial = TRUE);	//	idx로 CClient 포인터 얻기
};

#endif // !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)
