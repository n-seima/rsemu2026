// Client.h: interface for the CClient class.
//
//////////////////////////////////////////////////////////////////////

/////////////////////////////
//	9.22일 packet처리 부분 잘못되면 되돌릴 버젼 - 지현


#if !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)
#define AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include <windows.h>
#include <mmsystem.h>
#include "RSComm.h"
#include "BufferForPacket.h"
#include "mess_sign.h"

#ifdef	_FOR_JAPAN
//#define	_USE_NPGL	1
#endif

#ifdef	_FOR_KOREA
#define	_USE_XTRAP	1
#endif

#ifdef	_USE_NPGL
#include	"ggsrv.h"
#pragma comment(lib, "ggsrvlib.lib")
extern	BOOL	g_bIsDisconnectGGAuthFailedUser;
extern	BOOL	g_bIsUseNPGL;
#endif

extern DWORD	g_giveSocketCount;
extern DWORD	g_receiveSocketCount;
extern CRITICAL_SECTION	csKILLSOCKET;
extern void		KillSocket(SOCKET * sock);

typedef struct _IO_DATA{
	OVERLAPPED	OL;
	WSABUF		buf;
	DWORD		sendlen;
	char		Buffer[dMAX_PACKET_LENGTH];
	int			IOState;	//	0 - recv , 1 - send
}IO_DATA,*LP_IO_DATA;

#define dACTION_TYPE_MOVE			0x01
#define dACTION_TYPE_USER_SKILL		0x02

// typedef enum
// {
// // 	eISUSED_NOTYET		=	0,
// // 	eISUSED_DISUSER		=	1,
// // 	eISUSED_CLOSE		=	2,
// // 	eISUSED_KILL		=	3,
// // 	eISUSED_FIRSTOPEN	=	4,
// // 	eISUSED_ADD			=	5,
// // 	eISUSED_GETSSL		=	6,
// // 	eISUSED_ENDSSL		=	7,
// 
// 	eISUSED_SERVERStoWORLD_USER_MOVEMAP	=	100,
// 	eISUSED_WORLDtoSERVERS_USER_MOVEMAP	=	101,
// 	eISUSED_WDMSG_SAVEAVA_TO_FILE		=	102,
// 	eISUSED_DWMSG_SAVEAVA_TO_FILE		=	103,
// 	eISUSED_SERVERStoWORLD_USER_LOGOUT	=	104,
// 	eISUSED_MOVE_FIELD_IN_SAME_SERVER	=	105,
// 	
// //	eISUSED_SENDAVATARDATA				=	200,
// 	eISUSED_JOINSERVER					=	201,
// //	eISUSED_JOINCOMPLETE				=	202,
// 	eISUSED_READYTOEXITGAME				=	203,
// //	eISUSED_DISCONNECT					=	204,
// }eISUSED;

class CClient
{
public:
	enum	//	혹시 몰라 bit flag 형태로.. 중첩될 일은 없을 듯..
	{
		eSTATE_CLOSE		= 0x00,	//	사용중이 아닌 경우
		eSTATE_WASTE		= 0x01,	//	쓰레기 상태
		eSTATE_LOGOUT		= 0x02,	//	유저가 나간 상태 , 로그 아웃 처리 중 일 수도 있다. bit flag로 로그 아웃인데.. 필드 이동 중일 수도 있다.
		eSTATE_CONNECTING	= 0x04,	//	소켓만 연결된 상태
		eSTATE_RECEIVE_INFO = 0x08,	//	컨텐츠 관련 정보를 받는 중
		eSTATE_INGAME		= 0x10,	//	게임 컨텐츠와 연동된 상태
		eSTATE_MOVING		= 0x20,	//	필드 이동 중
		eSTATE_READYTOEXIT	= 0x40,	//	빠져나오려고 대기중
	};
//	추가될 내용.. 자신의 채널, 게임(서버 정보 및 방 정보) 시리얼..
private:
//	BOOL				isMoveMap;		//	맵을 이동할때 TRUE로 세팅한다.
//	BOOL				isClose;		//	소켓이 닫혀 있나? preclose하면 TRUE 된다.
//	WORD				m_wIsCorrectLogout;
//	DWORD				isUsed;			//	사용중인가?
//	BOOL				isSaveClose;	//	세이브를 잠시 하지말어.. ㅠ_ㅜ 변수 정말 많다. ;;
	DWORD				m_state;		//	클라이언트 상태를 체크한다. bit flag로 운영.

	WORD				isOper;			//	운영자인가.. 0 - 일반 유저  , 1 - 운영자
	WORD				isNetCafeUser;
	WORD				isOperInAccount;//계정에 운영자 설정이 되어 있나?
	DWORD				m_dwActorSerialInServer;//	서버 안에서 cACTOR의 시리얼
	WORD				m_wActorSerialInField;	//	필드 안에서 cACTOR의 시리얼
	WORD				field_serial;	//	현재 속한 필드의 번호 
	WORD				field_index;
	DWORD				world_serial;	//	world서버에서의 자신의 번호  : 게임서버를 이동하면 
	WORD				m_wPartySerial;	//	게임 서버내에서의 파티 시리얼.
#ifdef _USE_BROADCAST
	WORD				m_wBCSerial;
#endif
	DWORD				timer;

public:
	CRITICAL_SECTION	csUSER;

	DWORD				dwSendPacketCount;
	DWORD				dwRecvPacketCount;
	DWORD				dwReallySendPacketCount;
	DWORD				dwGetRecvPacketCount;

	DWORD				m_dwSumOfSendPacketSize;		//	보낸 총 패킷 사이즈
	DWORD				m_dwCountOfSendPacket;			//	보낸 패킷 수

	DWORD				m_dwCountOfCheckSendPacketSize;	//	최초로 패킷을 보내고 난후 지난 분

	DWORD				m_dwSumOfSendPacketSizePerMinute;	//	분당 보낸 패킷 사이즈 합
	DWORD				m_dwCountOfSendPacketSizePerMinute;	//	분당 보낸 패킷 수

	DWORD				m_dwLastCheckSendPacketSizeTime;	//	마지막으로 체크 한 시간

	DWORD				m_dwMaxSendPacketSizePerMinute;		//	분당 보낸 최고 패킷 사이즈
	WORD				m_wMaxSendPacketCountPerMinute;		//	분당 보낸 최고 패킷 수

	DWORD				m_dwThisTimeSendPacketSizeSum;		//	이번에 보낸 패킷 사이즈
	WORD				m_wThisTimeSendPacketCount;			//	이번에 보낸 패킷 수

	DWORD				m_dwSumOfReceivePacketSize;		//	받은 총 패킷 사이즈
	DWORD				m_dwCountOfReceivePacket;			//	받은 패킷 수

	DWORD				m_dwCountOfCheckReceivePacketSize;	//	최초로 패킷을 벋고 난후 지난 분

	DWORD				m_dwSumOfReceivePacketSizePerMinute;	//	분당 받은 패킷 사이즈 합
	DWORD				m_dwCountOfReceivePacketSizePerMinute;	//	분당 받은 패킷 수

	DWORD				m_dwLastCheckReceivePacketSizeTime;	//	마지막으로 체크 한 시간

	DWORD				m_dwMaxReceivePacketSizePerMinute;		//	분당 받은 최고 패킷 사이즈
	WORD				m_wMaxReceivePacketCountPerMinute;		//	분당 받은 최고 패킷 수

	DWORD				m_dwThisTimeReceivePacketSizeSum;		//	이번에 받은 패킷 사이즈
	WORD				m_wThisTimeReceivePacketCount;			//	이번에 받은 패킷 수

	int					m_iAskChangeEncTableTime;
	WORD				m_wReceivePacketSerial;	
	WORD				m_wCurrentEncTableIndex;
	WORD				m_wXorCode;
	BYTE				m_abEncPacketTable[2][dENCRYPT_TABLE_SIZE];

	void				buildEncTable(int _iTable);
	void				buildNextEncTable();
	BYTE*				getEncTable(int _iTable);

#ifdef	_USE_NPGL
	CCSAuth2			m_csa;
#endif

#ifdef	_USE_XTRAP
	char				*m_pXtrapBuffer;
	int					m_iVerifyXTrapCounter;

	void				initBufferForXtrap(int _iMapCount)
	{
		if	(m_pXtrapBuffer)
			return;

		m_pXtrapBuffer	=	new char [320+80*_iMapCount];
	}
#endif

#ifdef	_USE_HS
	WORD				m_wWaitHS_GuidResultTime;
	WORD				m_wWaitHS_ReqMsgResult;	//	0 기다리는거 없음,1 최초,2 이후
	WORD				m_wAppointmentDisconnectTime;
	int					m_iLastSendHS_ReqMsgTime,m_iSendHS_AckMsgCount;
	BYTE				m_abHS_GuidReqInfo[SIZEOF_GUIDREQINFO];
	BYTE				m_abHS_ReqInfo[SIZEOF_REQINFO];

	unsigned	long	*m_ppulCrcInfo;		//	핵쉴드 서버 연동 크랙 방지를 위한 버퍼
#endif
	WORD				wSerial;			//	서버에게서 받은 시리얼
	WORD				m_wTestClientSerial;
	char				ip[dIP_SIZE];
	WORD				wAcceptIndex;
	SOCKET				sock;			//	소켓 번호 
	DWORD				seedkey;
	BYTE				orkey;
	WORD				oldRecvSize,oldSendSize;
	char				id[dID_LENGTH];
#ifdef	_OGP_SERVICE
	char				m_strToken[dTOKEN_LENGTH];
#endif
	char				name[dNAME_LENGTH];
	DWORD				hashIdCode,hashNameCode;

	IO_DATA				rOL,sOL;
	int					MAX_LENGTH;
	CBufferForPacket	* rBuffer;
	CBufferForPacket	* sBuffer;
	DWORD				timegap;
	WORD				timegap_count;	//	0.3초기준으로 연속으로 10번이상 패킷이 오면 자른다.
	DWORD				loginTime;
	WORD				badtime;

	char				sendpacket[dMAX_PACKET_LENGTH];
	WORD				sendpacket_size;
	int					tmpcount;

						CClient(int	buffersize=dMAX_PACKET_LENGTH);
	virtual				~CClient(); 

	void				operateSendPacketDataStaticsWork(int _iSize);
	void				operateReceivePacketDataStaticsWork(int _iSize);

	void				Open(WORD _serial);
	void				Close();	//	소켓을 종료한다.
//	void				PreClose();
//	void				Kill();
	//	일반 Get & Set Method
//	void				readyToLogout()		{m_wIsCorrectLogout = TRUE;}
//	BOOL				isCorrectLogout()	{return m_wIsCorrectLogout;}
//	inline BOOL			GetIsClose()	{	return isClose;							}
	inline BOOL			isCanSendRecv()	{	return ((m_state >= eSTATE_CONNECTING)?(TRUE):(FALSE));	}	//	패킷을 보낼 수 있나?
	inline BOOL			GetIsUsed()		{	return ((m_state & eSTATE_CLOSE)?(FALSE):(TRUE));	}	//	사용중인건가?
	inline BOOL			GetMoveMap()	{	return ((m_state & eSTATE_MOVING)?(TRUE):(FALSE));	}
	inline BOOL			isInGame()		{	return ((m_state & eSTATE_INGAME)?(TRUE):(FALSE));	}
	inline BOOL			isBeforeInGame(){	return !isInGame();	}
	inline BOOL			isReadyToExit()	{	return ((m_state & eSTATE_READYTOEXIT)?(TRUE):(FALSE));	}

	inline void			UsedUser()		{	EnterCriticalSection(&csUSER);			}
	inline void			NotUsedUser()	{	LeaveCriticalSection(&csUSER);			}
	
	inline WORD			GetIsOper()				{	return isOper;		}
	inline WORD			GetIsNetCafeUser()		{	return isNetCafeUser;}

	inline WORD			GetIsOperInAccount()	{	return isOperInAccount; }
	inline WORD			GetSerial()				{	return wSerial;		}
	inline WORD			GetPartySerial()		{	return m_wPartySerial;	}
	inline	SOCKET		GetSocket()				{	return sock;		}
	inline	char *		GetID()					{	return id;			}
	inline	char *		GetName()				{	return name;		}
	inline	void		SetName(char *n)		{	strcpy(name,n);		}
	inline	int			GetTimer()				
	{	
		int time = timeGetTime() - timer;
		if(abs(time)>180000 && timeGetTime()>timer){	timer = timeGetTime();	return TRUE;}
		else if(timeGetTime()<timer)							  {	timer = timeGetTime();	return FALSE;}
		else					 								  {	return FALSE;						}
	}
	inline	void		SetSocket(SOCKET	soc){	if(sock){	KillSocket(&sock);}sock = soc;	}
	inline	void		SetSerial(WORD	ser)	{	wSerial = ser;		}
	inline	void		SetPartySerial(WORD ps)	{	m_wPartySerial = ps;}
	inline	void		SetOper(WORD op)		{	isOper = op;		}
	inline	void		SetNetCafeUser(WORD nc)	{	isNetCafeUser = nc;	}
	inline	void		SetOperInAccount(WORD op){	isOperInAccount = op;}
	inline	void		SetState(int _flag)		{	m_state = _flag;	}
	inline	void		SetStateFlag(int _flag)	{	m_state |=_flag; 	}
	inline  DWORD		GetTimeGap()			{	return timegap;		}
#ifdef _USE_BROADCAST
	inline	WORD		GetBCSerial()			{	return m_wBCSerial;	}
	inline	void		SetBCSerial(WORD _wSerial){	m_wBCSerial = _wSerial;	}
#endif

//	void				SetIsUsed(eISUSED isused);
	void				SetID(char *nid,char *_lpstrToken);
	BOOL				isWaste();
	
	//	return : 0 - 여유공간이 없어서 추가불가능
	//			 1 - 정상 
	int					AddRecvPacket(char *data,int size);	//	packet을 더한다.
	int					AddRecvPacket(int size);			//	packet을 더한다.
	int					AddSendPacket(char *data,int size);	//	packet을 더한다.  필요 없어서 삭제한다.

	//	return : NULL - 추가되어 있는 패킷이 없다.
	int					GetRecvPacket(char *);	//	받은 패킷을 빼내온다.
	int					CompleteSendPacket(int size);


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃필드와 같이 사용
	inline	DWORD		GetActorSerialInServer()	{	return m_dwActorSerialInServer;}
	inline	DWORD		GetActorSerialInField()		{	return m_wActorSerialInField;}
	inline	WORD		GetFieldSerial(){	return field_serial;}
	inline	WORD		GetFieldIndex(){	return field_index;}
	inline	DWORD		GetWorldSerial(){	return world_serial;}

	inline	void		SetActorSerialInGame(DWORD ser)	{	m_dwActorSerialInServer = ser;	}
	inline	void		SetActorSerialInField(WORD ser)	{	m_wActorSerialInField	= ser;	}
	inline	void		SetFieldSerial(WORD ser){	field_serial = ser; }
	inline	void		SetFieldIndex(WORD ser){	field_index	= ser; }
	inline	void		SetWorldSerial(DWORD ser){	world_serial = ser;	}
//┃필드와 같이 사용
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};

class	CClientManager{
private:
	CRITICAL_SECTION	csDeque;
	CClientManager();

public:
	virtual				 ~CClientManager();
	static CClientManager & GetInstance();	//	항상 동일한 인스턴스를 준다.
	
	CClient				User[dGAME_MAX_USER_COUNT];
	WORD				m_wNext,m_wCount;

	inline int			Size(){	return m_wCount;	}
	int					checkSize();

	WORD				NextNext();
	void				Init();				//	클라이언트 정보 초기화
	void				End();				//	모든 내용을 정리한다.
	CClient*			Add();				//	유저를 추가하고 시리얼을 돌려준다.
	void				Del(WORD _serial);	//	해당 유저를 삭제한다.

	BOOL				removeClientWithCheckId(int _iIndex,char *_lpstrId);	//	해당 유저를 삭제한다.

	CClient			*	GetClientPnt(char * , int flag=0);				//	id로 CClient 포인터 얻기 flag=1-> Name으로  
	CClient			*	getClientByIdIgnoreUsedFlag(char *_lpstrId);	//	used flag를 무시하고 계정으로 포인터 얻기
	CClient			*	getClientByNameIgnoreUsedFlag(char *_lpstrName);//	used flag를 무시하고 이름으로 포인터 얻기
	CClient			*	GetClientPnt(WORD _serial);						//	idx로 CClient 포인터 얻기

	CClient			*	GetClientPntByIgnoreUsedFlag(int idx);			//	serial로 CClient 포인터 얻기
	CClient			*	GetClientPntByIgnoreUsedFlagById(char * _strId);//	id로 CClient 포인터 얻기

	inline CClient *	mGETCLIENT(int serial){ return GetClientPnt(serial); }

	inline BOOL			SEND_ISOK(int serial)	//정상적으로 등록 되어 있는 클라이언트냐?
	{
		CClient * lpClient = GetClientPnt(serial);
		if(!lpClient)	return FALSE;
		return lpClient->GetIsUsed();
	}

	inline BOOL			SEND_PACKET(int serial,void * b,WORD c)
	{
		CClient * lpClient = GetClientPnt(serial);

		if	(!lpClient)
			return	FALSE;

		lpClient->AddSendPacket((char *)b,c);

		ProcessSendPacket(serial);

		return	TRUE;
	}
	inline void	SEND_PACKET2(CClient * lpClient,void * b,WORD c)
	{
		if	(!lpClient)
			return;

		lpClient->AddSendPacket((char *)b,c);

		ProcessSendPacket(lpClient->GetSerial());
	}

	inline DWORD GET_WORLDSERIAL(int serial)
	{
		CClient * lpClient = GetClientPnt(serial);
		if(!lpClient)	return 0xffffffff;
		return lpClient->GetWorldSerial();
	}
};

extern	int		SendPacketToBCS(char * _cData,WORD _wSize);
extern	BOOL	IsAvailBCConnect();

#endif // !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)
