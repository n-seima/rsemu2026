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
#include "STACK.H"
#include "RSComm.h"
#include "definePacketData.h"
#include "cPACKET_WORLDSERVER.h"
#include "actorDefine.h"
#include "PacketBuffer.h"

typedef struct _IO_DATA{
	OVERLAPPED	OL;
	WSABUF		buf;
	DWORD		sendlen;
	char		Buffer[dMAX_PACKET_LENGTH_SERVER];
	int			IOState;	//	0 - recv , 1 - send
}IO_DATA,*LP_IO_DATA;

class	Avatar_Data{
public:
	WORD				wSerial;
	BOOL				bMovingMap;
	CRITICAL_SECTION	csSerial;
	CPlayerSaveDataForServerDefine		data;
	CUpkeepPlayerData	keepData;
	cActorStatusUpkeep	upkeepStatus;
	char				strMaster[dID_LENGTH];

#ifdef	_OGP_SERVICE
	char				m_strToken[dTOKEN_LENGTH];
#endif
	DWORD				hashIdCode;
	WORD				wState;		//	0:wait-login or mapmove , 1:connect , 2:used
	WORD				wGateIndex;	//	맵 이동시에 사용한다.
	WORD				isOper;
	WORD				isNetCafeUser;
	DWORD				time;
	DWORD				min;
	DWORD				playtime;
	WORD				wGameSerial;	//	Game서버에서 사용하는 serial
	WORD				delCount;
	BOOL				isPrimium;

	void				SetTime();
	BOOL				isWaste();

	Avatar_Data(){	InitializeCriticalSection(&csSerial);	close();	}
	inline void	close()
	{
		bMovingMap = FALSE;
		wSerial = 0xffff;
		delCount = 0;
		memset(&data,0,sizeof(CPlayerSaveDataForServerDefine));
		keepData.reset();
		upkeepStatus.reset();
		strcpy(strMaster,"");
		hashIdCode = 0;
		wState = 0xffff;
		wGateIndex = 0;
		time = 0;
		min = 0;
		playtime = 0;
		wGameSerial = 0xffff;
		isOper = 0;
		isNetCafeUser = 0;
		isPrimium = 0;
	}
};

class	CGClientManager{			//	Field in Client
private:
	CRITICAL_SECTION	csDeque;

public:
						CGClientManager();
	virtual				 ~CGClientManager();

	Avatar_Data			*m_pStack;
	WORD				m_wNext,m_wCount;

	inline int			Size(){	return m_wCount;	}

	WORD				NextNext();
	void				Init();				//	클라이언트 정보 초기화
	void				End();				//	모든 내용을 정리한다.
	int					Add(char *_strId);		//	유저를 추가하고, 데이타를 복사해두구 시리얼을 돌려준다.
	BOOL				Del(WORD _serial,char * str);		//	해당 유저를 삭제한다.
	BOOL				Del(char * id,char * str);

	void				writeData(WORD _wSerial , CPlayerSaveDataForServerDefine * data,WORD  gsserial,WORD oper,WORD _isNetCafeUser, WORD s = 1);
	void				writeKeepData(WORD _wSerial,CUpkeepPlayerData * upkeepdata,BOOL isMoveField);
	void				writeKeepStatus(WORD _wSerial,cActorStatusUpkeep * _lpUpkeepStatus);	
	Avatar_Data		*	GetAvatarPnt(char * , int flag=0);	//	0:ID , 1:name로 CClient 포인터 얻기
	Avatar_Data		*	GetAvatarPnt(int idx,BOOL	isSerial = TRUE);	//	idx로 CClient 포인터 얻기

	void				allocAvatarDataBuffer();
};

class CClient
{
//	추가될 내용.. 자신의 채널, 게임(서버 정보 및 방 정보) 시리얼..
private:
	SOCKET				sock;		//	소켓 번호 
	BOOL				isUsed;		//	사용중인가?
	BOOL				isClose;	//	소켓이 닫혀 있나? preclose하면 TRUE 된다.
	WORD				serial;			//	WORLD 서버에게서 받은 시리얼
	char				id[dMAPNAME_LENGTH];	//	WORLD에 연결된 게임서버의 이름들
	char				addr[dIP_SIZE];	//	연결된 서버의 주소
	CRITICAL_SECTION	csUSER;

public:
	//	about USER DATA
	CGClientManager		GameClientMagager;

	//	about FIELD
	WORD				m_wIsGameServer;
	WORD				wMapCount;
	cGameServerType		cGSInfo[dGAME_MAX_FIELD_COUNT];	//	자신이 관리하는 맵이름을 등록한다.
	BOOL				isReadyToPlay;	//	게임서버에 기본정보를 다 보내주었다면 Check

	//	about Packet Buffer
	BOOL				isSendPacket;
	IO_DATA				rOL,sOL;
	int					MAX_LENGTH;
	DWORD				timegap;
	LPBYTE				lpTmpBuf;
	char				strConnectaddr[2][dIP_SIZE];	//	유저가 연결할 소켓의 갯수
	WORD				wConnectCount;					//	유저가 연결할 소켓의 갯수
	WORD				wSendIndex;						//	2 : 반반으로 나눠서 보내준다.
	WORD				wSendCount;						//	한방향으로 보낼때 얼마나 줘야 할까..
	WORD				wCurIndex;						//	보낸 방향 
	WORD				m_wServerIndex;
	WORD				m_wCurrentPlayerCount;	//	게임 서버에서 받아온 수치- 이걸 DB에 기록하라.
	//	해당 시간대에 접속한 유저의 레벨 분포

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
	inline BOOL			GetIsClose(){		return isClose;					}
	inline void			UsedUser(){			EnterCriticalSection(&csUSER);	}
	inline void			NotUsedUser(){		LeaveCriticalSection(&csUSER);	}

	inline SOCKET		GetSocket()				{	return sock;			}
	inline BOOL			GetIsUsed()				{	return isUsed;			}
	inline WORD			GetSerial()				{	return serial;			}
	inline char		*	GetID()					{	return id;				}
	inline char		*	GetAddr()				{	return addr;			}
	inline WORD			GetUserCount()			{	return GameClientMagager.Size();	}
	inline void			SetSerial(WORD	ser)	{	serial = ser;			}
	inline void			SetAddr(char *a)		{	strcpy(addr,a);			}
	inline int			GetTimeGap()			{	return timegap;			}
	int					GetMapIndex(int proserial);

	void				SetSocket(SOCKET	soc);
	void				SetIsUsed(BOOL isused);
	void				SetID(char *nid);

	void				setGameServer();	//	이 클라이언트를 게임서버로 설정
	
	BOOL				isWaste();

	//	return : 0 - 여유공간이 없어서 추가불가능
	//			 1 - 정상 
	int					AddRecvPacket(char *data,int size);	//	packet을 더한다.
	int					AddRecvPacket(int size);			//	packet을 더한다.
	int					AddSendPacket(char *data,int size);	//	packet을 더한다.  필요 없어서 삭제한다.

	//	return : NULL - 추가되어 있는 패킷이 없다.
	int					GetRecvPacket(char	*);	//	받은 패킷을 빼내온다.
	int					CompleteSendPacket(int size);
};

class	CClientManager{
private:
	CRITICAL_SECTION	csDeque;	//	used, unused를 같이 가둬라.	좀 느려도 별수없다. 안정 안정.. 환  .. 꺄 >.<

	CClientManager();

public:
	virtual				 ~CClientManager();
	static CClientManager & GetInstance();	//	항상 동일한 인스턴스를 준다.

	CClient				User[dWORLD_MAX_USER_COUNT];
	WORD				m_wNext,m_wCount,m_wGSCount;
	inline int			Size(){	return m_wCount;	}
	inline int			GameServerCount(){ return m_wGSCount; }
	WORD				NextNext();

	WORD				m_wBCSIndex;
	
	void				SetBaseSize(int bs);
	int					GetAllUserCount();
	void				Init();					//	클라이언트 정보 초기화
	void				End();					//	모든 내용을 정리한다.
	int					Add();					//	유저를 추가하고 시리얼을 돌려준다.
	void				Del(int idx,char *id);	//	해당 유저를 삭제한다.

	BOOL				isGuildBattleField(int _iFieldSerial);	//	이거 길드전 필드냐?

	CClient			*	GetClientPnt(char *);	//	ID로 CClient 포인터 얻기
	CClient			*	GetClientPnt(WORD idx,BOOL	isSerial = TRUE);	//	idx로 CClient 포인터 얻기
	CClient			*	GetClientOfField(int properserial,WORD	_wPrimium,WORD & _wResult);
	CClient			*	GetClientOfField(char *,int & properserial,WORD	_wPrimium,WORD & _wResult);
	char			*	GetFieldName(int fieldindex);
	Avatar_Data		*	GetGClientPnt(char *,int flag = 0);				//	게임서버에서 유저 찾기
	WORD				GetClientOfGClientSerial(char * , int flag =0);	//	한 유저가 속한 게임서버의 시리얼
	CClient			*	GetClientOfGClientPnt(char * , int _iFindByName = FALSE);	//	한 유저가 속한 게임서버
	CClient			*	GetClientOfGClientPntForDelete(char * , int flag =0);	//	삭제를 위한 검색. 맵 이동중인거 안가져 온다.

	int					SendToBCS(char * _cData, WORD _size);
};

#define	mSEND_GSP(user,packet)	if (user) user->AddSendPacket((char *)&packet,packet.base.wSize);else SendDataAllGameServer((char *)&packet,packet.base.wSize);
extern	void			SendDataAllGameServer(char * _data,WORD _size,BOOL _bIsWantOnlyGS=TRUE);
extern	BOOL			SendGuildPacketToBCS(void *_lpPacket,int _iGuild);
extern	BOOL			SendTransPacketToBCS(void *_lpPacket);
extern	void			SendPacketToBCS(void *_lpPacket);

extern	Avatar_Data*	GetAvatar(char *_lpstrUserName);	//	이름으로 아바타 정보 검색
extern	CClient*		GetClientServer(char *_lpstrUserName);


#endif // !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)