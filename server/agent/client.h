// Client.h: interface for the CClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)
#define AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include "RSComm.h"
#include "PacketBuffer.h"
#include "userCustomDataDefine.h"

#define dMAX_CLIENT_SERVER_COUNT	256
#define dMAX_PACKET_LENGTH_FORCS	12288

typedef struct _IO_DATA{
	OVERLAPPED	OL;
	WSABUF		buf;
	DWORD		sendlen;
	char		Buffer[dMAX_PACKET_LENGTH_FORCS];
	int			IOState;	//	0 - recv , 1 - send
}IO_DATA,*LP_IO_DATA;

class CClient
{
public:
	CRITICAL_SECTION	csUSER;
	CRITICAL_SECTION	csDebug;
	int					MAX_LENGTH;

	WORD				m_wSerial;			//	서버에게서 받은 시리얼
	char				m_strIp[dIP_SIZE];
	SOCKET				m_hSock;			//	소켓 번호 
	char				m_strId[dID_LENGTH];
	char				m_strName[dNAME_LENGTH];

	WORD				m_wLevel;
	WORD				m_wAvatarIndex;
	WORD				m_wPartySerial;
	WORD				m_wGuildSerial;
	//WORD				m_wGuildSerial;
	WORD				m_wGuildRank;					
	WORD				m_wCurrentField;			//	운영자 레벨
	WORD				m_wOper;					//	운영자 레벨
	WORD				m_wMyGSSerial;
	WORD				m_wGSSerial;				//	게임서버에 돌려줄 Serial
	BOOL				m_isGameServer;
	WORD				m_wIsRequireAddToParty;		//	파티에 추가 실패 했다. 다시 추가 시도 해라
	DWORD				m_dwHashIdCode,m_dwHashNameCode;
	IO_DATA				m_stRecvOL,m_stSendOL;
	int					m_iDBIndex;
	DWORD				m_dwSaveGap;
	DWORD				m_dwFriendsListUpdateGap;	//	10000ms초마다 갱신한다.

	CPacketBuffer	*	m_pclRecvBuffer;
	CPacketBuffer	*	m_pclSendBuffer;
	char			*	m_pcSendpacket;
	WORD				m_wSendpacket_size;

//	for debugging
	DWORD				m_dwLastRecvUpdateTime;
	DWORD				m_dwLastSendUpdateTime;
	DWORD				m_dwRecvTmpSizePerSec;			//	초당 받은 패킷 사이즈
	DWORD				m_dwSendTmpSizePerSec;			//	초당 보낸 패킷 사이즈
	DWORD				m_dwRecvSizePerSec;			//	초당 받은 패킷 사이즈
	DWORD				m_dwSendSizePerSec;			//	초당 보낸 패킷 사이즈

public:
						CClient(int	_iBuffersize=dMAX_PACKET_LENGTH_FORCS);	//	_SERVER
	virtual				~CClient();

	void				Open(WORD _wSerial,SOCKET _hSock,char * _strIp);
	void				Kill();

	//	일반 Get & Set Method
	
	//	return : 0 - 여유공간이 없어서 추가불가능
	//			 1 - 정상 
	int					AddRecvPacket(char *_cData,int _iSize);	//	packet을 더한다.
	int					AddRecvPacket(int _iSize);			//	packet을 더한다.
	int					AddSendPacket(char * _cData,int _iSize);	//	packet을 더한다.  필요 없어서 삭제한다.

	//	return : NULL - 추가되어 있는 패킷이 없다.
	int					GetRecvPacket(char * _cRecvpacket);	//	받은 패킷을 빼내온다.
	int					CompleteSendPacket(int _iSize);
};

class	CClientManager{
private:
	CRITICAL_SECTION	csDeque;
	CClientManager();

public:
	virtual				 ~CClientManager();
	static CClientManager & GetInstance();	//	항상 동일한 인스턴스를 준다.
	
	CClient				User[dMAX_CLIENT_SERVER_COUNT];
	WORD				m_wNext,m_wCount;

	inline int			Size(){	return m_wCount;	}

	WORD				NextNext();
	void				Init();				//	클라이언트 정보 초기화
	WORD				Add();				//	유저를 추가하고 시리얼을 돌려준다.
	void				Del(WORD _wSerial);	//	해당 유저를 삭제한다.

	CClient			*	GetClientPnt(char * _strId, BOOL _isSearchByName=0);	//	id로 CClient 포인터 얻기 flag=1-> Name으로  
	CClient			*	GetClientPnt(WORD _wSerial);							//	idx로 CClient 포인터 얻기
};

extern	CClientManager &UserManager;

#endif // !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)
