// Client.h: interface for the CClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)
#define AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include <windows.h>
#include <mmsystem.h>
#include "definePacketData.h"
#include "RSComm.h"
#include "cPLAYER_DATA.h"
#include "STACK.H"
#include "PacketBuffer.h"
#include "CSharedMemory.h"

class CINDEX_TABLE{

public:
	int		iIndex;
	DWORD	hashIdCode,hashNameCode;
	char	strName[dNAME_LENGTH];
	char	strId[dID_LENGTH];
	WORD	wAvaIndex;
	CINDEX_TABLE()
	{	
		reset();
	}
	void	reset()
	{
		hashIdCode = hashNameCode = 0;
		iIndex = 0x7fffffff;
		memset(strName,0,dNAME_LENGTH);
		memset(strId,0,dID_LENGTH);
		wAvaIndex = 0xffff;
	}
};

typedef struct _IO_DATA{
	OVERLAPPED	OL;
	WSABUF		buf;
	DWORD		sendlen;
	char		Buffer[dMAX_PACKET_LENGTH_SERVER];
	int			IOState;	//	0 - recv , 1 - send
}IO_DATA,*LP_IO_DATA;

#define	dMAX_INDEX_COUNT		1000000
#define dINDEX_FILENAME			"indexinfo.dat"

class	CIndexManager
{
public:
	char				strDirectory[512];
	CINDEX_TABLE		idxTable[dMAX_INDEX_COUNT];
	DWORD				dwCount;
	CRITICAL_SECTION	cs;

	CIndexManager();
	virtual ~CIndexManager();

	DWORD	add(int _iIndex, char * _strName, char * _strId, WORD _avaIdx);
	BOOL	addFirst(int _iIndex, char * _strName, char * _strId, WORD _avaIdx);
	BOOL	remove(char * _strName, char * _strId);
	BOOL	remove(int _iIndex);
	void	removeAll(char * _strId);

	int		getIndex(char * _strName, char * _strId="");
	CINDEX_TABLE * getInfo(char * _strName, char * _strId="");
	int		getBlankPos(char * _strId);
	void	getIndexById(char * _strId, int * _iIndex);	//	4개 까지만 처리할것 !! 넘는거 에러 메세지 남기고..
	DWORD	getTableIndexByAvaIndex(char * _strId,int _avaIdx);
	BOOL	changeIndex(int _iIndex, char * _strName, char * _strId="");
};

class	CClientData		//	USER DATA & Select Avatar Data
{
public:
	WORD				wSerial;		//	자신의 시리얼
	WORD				wWorldSerial;	//	world server serial
	DWORD				dwTimer;		//	2분안에 가져가라.. - 200000
	BOOL				isSend;			//	가져갔나??
	WORD				wOper;
	WORD				m_wResaveCount;
	DWORD				hashIdCode,hashNameCode;
	cPLAYER_DATA		pData;

	CClientData()
	{
		wSerial			=	0xffff;
		m_wResaveCount	=	0;
	}

	inline void			copy(CClientData * _pSource)
	{
		memcpy(this,_pSource,sizeof(CClientData));
		makeHash();
	}

	void				close()
	{
		memset(this,0xff,sizeof(CClientData));

		wOper			=	0;
		m_wResaveCount	=	0;
		hashIdCode		=	0;
		hashNameCode	=	0;
	}

	void	makeHash()
	{
		hashIdCode = GetHashCode((BYTE *)pData.m_strId);
		hashNameCode = GetHashCode((BYTE *)pData.m_strName);
	}
};

class	CClient  
{
public:
	typedef enum
	{
		eSRVTYPE_GAME = 0,
		eSRVTYPE_WORLD,
		eSRVTYPE_BC,
	}eSRVTYPE;

//	추가될 내용.. 자신의 채널, 게임(서버 정보 및 방 정보) 시리얼..
private:
	SOCKET				sock;			//	소켓 번호 
	BOOL				isUsed;			//	사용중인가?
	BOOL				isClose;		//	소켓이 닫혀 있나? preclose하면 TRUE 된다.
	BOOL				isSave;			//	세이브했냐?
	eSRVTYPE			enServerType;	//	어떤 서버냐?
	char				id[dID_LENGTH];	//	연결된 서버의 이름들
	WORD				wUserCount;
	
	CRITICAL_SECTION	csUSER;

public:
	//	about Packet Buffer
	DWORD				serial;			//	시리얼
	IO_DATA				rOL,sOL;
	BOOL				isSending;
	int					MAX_LENGTH;
	DWORD				timegap;
	char				m_strIP[32];	//	연결된 서버의 이름들

	CPacketBuffer	*	rBuffer;
	CPacketBuffer	*	sBuffer;
	char				sendpacket[dMAX_PACKET_LENGTH_SERVER];
	WORD				sendpacket_size;

						CClient(int	buffersize=dMAX_PACKET_LENGTH_SERVER);
	virtual				~CClient();

	inline eSRVTYPE		GetSrvType(){	return enServerType; }
	void				SetSrvType(eSRVTYPE _eType){ enServerType = _eType; }
	void				SetIP(char *_lpstrIP);
	void				Open();
	void				PreClose();
	void				Close();
	void				Kill();
	void				CSReset();
	//	일반 Get & Set Method 
	inline BOOL			GetIsClose(){		return isClose;							}
	inline void			UsedUser(){			EnterCriticalSection(&csUSER);			}
	inline void			NotUsedUser(){		LeaveCriticalSection(&csUSER);			}

	inline SOCKET		GetSocket()			{		return sock;			}
	inline BOOL			GetIsUsed()			{		return isUsed;			}
	inline DWORD		GetSerial()			{		return serial;			}
	inline char		*	GetID()				{		return id;				}
	inline WORD			GetUserCount()		{		return wUserCount;		}	
	inline void			SetSocket(SOCKET soc){ if(sock){ closesocket(sock); sock=NULL;}	sock = soc;	}
	inline void			SetSerial(DWORD	ser){		serial = ser;			}

	inline int			GetTimeGap()		{		return timegap;			}
	
	void				SetIsUsed(BOOL isused);
	void				SetID(char *nid);
	void				SetUserCount(WORD count);
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
	CClient				User[dDB_MAX_USER_COUNT];
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
	CClient			*	GetWorldSrvPnt();
};

#define dDATA_MAX_COUNT						dWORLD_MAX_ALLUSER_COUNT*2
#define	dMAX_FAILED_DATA_COUNT_TO_SAVE_DB	1024

class	CClientDataManager
{
private:
	int					m_iCount,m_iRookie;
	int					m_iFailedDataCount,m_iFailedDataRookie;
	DWORD				m_dwUpdateCounter;
	BOOL				m_bIsActive;

	CRITICAL_SECTION	m_csDeque;
	CClientData			m_aStack[dDATA_MAX_COUNT];
	CClientData			m_aFailedStack[dMAX_FAILED_DATA_COUNT_TO_SAVE_DB];

public:
	CClientData*		getClient(char *_lpstrString,BOOL _bIsByName);
	inline CClientData*	getClientById(char *_lpstrId)
	{
		return	getClient(_lpstrId,FALSE);
	}
	inline CClientData*	getClientByName(char *_lpstrName)
	{
		return	getClient(_lpstrName,TRUE);
	}

	BOOL				Init();						//	클라이언트 정보 초기화
	void				End();						//	모든 내용을 정리한다.
	WORD				Add(cPLAYER_DATA *_lpData,int _iOperaterLevel);//	유저를 추가하고 시리얼을 돌려준다.
	BOOL				Del(int _iIndex);			//	해당 유저를 삭제한다.
	CClientData*		GetClientPnt(char *_lpstrString,BOOL _bIsByName= FALSE);	//	Id(or Name)로 CClientData 포인터 얻기
	CClientData*		GetClientPnt(int _iIndex);	//	idx로 CClientData 포인터 얻기

	inline	BOOL		removeById(char *_lpstrString)	//	해당 유저를 아이디로 삭제한다.
	{
		return	remove(_lpstrString,FALSE);
	}
	inline	BOOL		removeByName(char *_lpstrString)	//	해당 유저를 이름으로 삭제한다.
	{
		return	remove(_lpstrString,TRUE);
	}
	BOOL				remove(char *_lpstrString,BOOL _bIsByName);	//	해당 유저를 삭제한다.

	BOOL				removeByIdAndSave(char *_lpstrId,cPLAYER_DATA * _pData = NULL);	//	Id와 아바타 인덱스를 참조해서 제거
	void				removeByIdAndIndex(char *_lpstrId,int _iAvatarIndex);		//	Id와 아바타 인덱스를 참조해서 제거

	inline	int			getCount()				{return	m_iCount;}
	inline	int			getFailedDataCount()	{return	m_iFailedDataCount;}
	inline	BOOL		isActive()				{return m_bIsActive;}
	inline	void		active()				{m_bIsActive	=	TRUE;}
	inline	void		disactive()				{m_bIsActive	=	FALSE;}

	int					checkClientStatus(cPLAYER_DATA *_lpData);
	void				saveAllDataToDB();	//	모든 데이터를 DB에 저정한다.

	void				update();
	int					updatePlayerDataByName(char *_lpstrName,cPLAYER_DATA *_lpData);	//	이름으로 유저를 찾아서 데이터를 업데이트 한다.
	void				saveMemoryDataToDBById(char *_lpstrId);	//	아이디로 유저를 찾아서 데이터를 업데이트 한다.

//	DB에 저장 실패한 데이터 관리
	BOOL				addFailedData(cPLAYER_DATA * _lpData,BOOL _bIsAlreadyLocked=FALSE);	//	시스템 문제로 DB 저장에 실패 했다.
	CClientData*		getFailedDataWithNoLock(char *_lpstrId,BOOL _bIsByName);
	void				removeFailedDataWithNoLock(CClientData *_lpClient);

	inline	void		lock()		{EnterCriticalSection(&m_csDeque);}
	inline	void		unlock()	{LeaveCriticalSection(&m_csDeque);}
};

extern	CClientDataManager*	g_lpCdm;	//	Client Data Manager
extern	CIndexManager		g_Im;		//	Index Manager
extern	CSharedMemory		g_sharedMemory;
extern	int					g_bIsMainTestServer;	//	본서버랑 설정이 똑같은 테섭. 결재나 몇가지 게임 외적인것만 테스트다.


#endif // !defined(AFX_CLIENT_H__8C19EDBE_0135_46A5_82E2_E59F3DA25643__INCLUDED_)
