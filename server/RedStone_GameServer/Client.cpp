// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////
#include "Client.h"
#include "cPACKET_BASE.h"
#include "cSRVUTIL.h"
#include "cGAME.h"
#include "debugCode.h"
#include "GameDLL.H"

#define dGAPLIMIT		150000

DWORD				g_giveSocketCount	=	0;
DWORD				g_receiveSocketCount=	0;
CRITICAL_SECTION	csKILLSOCKET;
void				KillSocket(SOCKET * sock);

#ifdef	_USE_NPGL
BOOL				g_bIsUseNPGL					=	TRUE;
BOOL				g_bIsDisconnectGGAuthFailedUser	=	TRUE;
#endif

int		GetPacketSize(WORD	pType);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void	KillSocket(SOCKET * sock)
{
	CCritical	CS(&csKILLSOCKET);
	if(*sock == 0)	return;
	if(*sock)
	{
		if(*sock>65535)
			*sock = INVALID_SOCKET;
		else
		{
			shutdown(*sock,1);
			closesocket(*sock);
			*sock = INVALID_SOCKET;
			g_receiveSocketCount++;
		}
	}
}


CClient::CClient(int	buffersize)	:	m_state(eSTATE_CLOSE)
{
	srand(timeGetTime());

	seedkey = rand();
	seedkey = seedkey*0x10000+rand();
	orkey	= (rand()%255);

#ifdef	_FOR_KOREA
	m_pXtrapBuffer	=	NULL;
#endif

#ifdef	_USE_HS
	m_ppulCrcInfo	=	NULL;
#endif
	
	MAX_LENGTH		=	buffersize;
//	isUsed			=	eISUSED_NOTYET;
//	isSaveClose		=	FALSE;
//	isClose			=	TRUE;
//	isMoveMap		=	FALSE;		//	맵을 이동할때 TRUE로 세팅한다.
//	m_wIsCorrectLogout		=	FALSE;

	sock			=	INVALID_SOCKET;
	badtime			=	0;
	timegap			=	timeGetTime();
	timegap_count	=	0;
	loginTime		=	0;
	m_wPartySerial	=	0xffff;
	m_wReceivePacketSerial	=	0xffff;
	m_wCurrentEncTableIndex	=	0;

	wSerial			=	0xffff;
#ifdef _USE_BROADCAST
	m_wBCSerial		=	0xffff;
#endif
	m_dwActorSerialInServer	=	-1;
	m_wActorSerialInField	=	-1;
	field_serial			=	-1;
	isOper					=	0;
	m_wTestClientSerial		=	0xffff;
	hashIdCode = hashNameCode = 0;
	m_iAskChangeEncTableTime	=	0;

	memset(sendpacket,0,MAX_LENGTH);
	sendpacket_size = 0;
	memset(ip,0,dIP_SIZE);
	
	rBuffer = new CBufferForPacket(dMAX_PACKET_LENGTH);
	sBuffer = new CBufferForPacket(dMAX_PACKET_LENGTH);

	FPInitCS(&csUSER);
}

CClient::~CClient()
{	
	Close();

#ifdef	_USE_HS
	delete [] m_ppulCrcInfo;
#endif

	DeleteCriticalSection(&csUSER);
}

void	CClient::Open(WORD _serial)
{
//	isUsed					=	eISUSED_FIRSTOPEN;
//	isClose					=	FALSE;
//	isSaveClose				=	FALSE;
//	isMoveMap				=	FALSE;		//	맵을 이동할때 TRUE로 세팅한다.
//	m_wIsCorrectLogout		=	FALSE;

	UsedUser();
	rBuffer->Reset();
	sBuffer->Reset();
	oldRecvSize = oldSendSize = 0;
	wAcceptIndex			=	0;
	isOper					=	0;
	m_dwActorSerialInServer	=	-1;
	m_wActorSerialInField	=	-1;
	timer					=	timeGetTime();
	timegap					=	timeGetTime();
	timegap_count			=	0;
	loginTime				=	0;
	badtime					=	0;
	m_wPartySerial			=	0xffff;
	tmpcount				=	0;
	wSerial					=	_serial;
	m_wTestClientSerial		=	0xffff;
#ifdef _USE_BROADCAST
	m_wBCSerial		=	0xffff;
#endif

#ifdef	_USE_HS
	m_iLastSendHS_ReqMsgTime		=	0;
	m_iSendHS_AckMsgCount			=	0;
	m_wWaitHS_GuidResultTime		=	0;
	m_wAppointmentDisconnectTime	=	0;
	m_wWaitHS_ReqMsgResult			=	nsHackShield::eWR_None;

	if	(m_ppulCrcInfo)	//	핵쉴드 서버 연동 크랙 방지를 위한 버퍼
		delete [] m_ppulCrcInfo;

	m_ppulCrcInfo			=	NULL;	
#endif

	memset(&rOL.OL,0,sizeof(OVERLAPPED));
	rOL.buf.buf = rOL.Buffer;
	rOL.buf.len = dMAX_PACKET_LENGTH;

	memset(&sOL.OL,0,sizeof(OVERLAPPED));
	sOL.buf.buf = sOL.Buffer;
	sOL.buf.len = dMAX_PACKET_LENGTH;
	sOL.IOState = 1;

	memset(id,0,dID_LENGTH);
	memset(name,0,dNAME_LENGTH);
	memset(sendpacket,0,MAX_LENGTH);
	memset(ip,0,dIP_SIZE);
	hashIdCode = hashNameCode = 0;

	sendpacket_size = 0;
	dwSendPacketCount = 0;
	dwRecvPacketCount = 0;
	dwReallySendPacketCount = 0;
	dwGetRecvPacketCount = 0;
	m_iAskChangeEncTableTime	=	0;

	m_state = eSTATE_CONNECTING;
	NotUsedUser();
}

// void	CClient::PreClose()
// {
// 	if (INVALID_SOCKET != sock) 
// 	{ 
// 		KillSocket(&sock);
// 	} 
// 
// 	m_iAskChangeEncTableTime	=	0;
// //	isClose			=	TRUE;
// //	isSaveClose		=	FALSE;
// 	m_wReceivePacketSerial	=	0xffff;
// }

void
CClient::buildEncTable(int _iTable)
{
	m_wCurrentEncTableIndex	=	0;

	int	iSeedValue	=	random(73)+69;

	for (int iSlot=0;iSlot<dENCRYPT_TABLE_SIZE;iSlot++)
		m_abEncPacketTable[_iTable][iSlot]	=	random(iSeedValue)-iSeedValue/2;
}

void
CClient::buildNextEncTable()
{
	buildEncTable(1-m_wCurrentEncTableIndex);
}


BYTE*
CClient::getEncTable(int _iTable)
{
	return	m_abEncPacketTable[_iTable];
}

void	CClient::Close()
{
	rBuffer->Reset();
	sBuffer->Reset();

	memset(&rOL.OL,0,sizeof(OVERLAPPED));
	rOL.buf.buf = rOL.Buffer;
	rOL.buf.len = dMAX_PACKET_LENGTH;

	memset(&sOL.OL,0,sizeof(OVERLAPPED));
	sOL.buf.buf = sOL.Buffer;
	sOL.buf.len = dMAX_PACKET_LENGTH;
	sOL.IOState = 1;
	if (INVALID_SOCKET != sock) 
	{ 
		KillSocket(&sock);
	} 

	m_state = eSTATE_CLOSE;
// 	isUsed					=	FALSE;
// 	isSaveClose				=	eISUSED_CLOSE;
// 	isClose					=	TRUE;
// 	isMoveMap				=	FALSE;		//	맵을 이동할때 TRUE로 세팅한다.
//	m_wIsCorrectLogout		=	FALSE;
	timegap					=	0xffffffff;
	timegap_count			=	0;
	loginTime				=	0;
	isOper					=	0;
	wSerial					=	0xffff;
	m_wReceivePacketSerial	=	0xffff;
	m_iAskChangeEncTableTime=	0;
	m_dwActorSerialInServer	=	-1;
	m_wActorSerialInField	=	-1;
	field_serial			=	-1;
	m_wTestClientSerial		=	0xffff;
	m_wPartySerial			=	0xffff;
#ifdef _USE_BROADCAST
	m_wBCSerial				=	0xffff;
#endif

	hashIdCode = hashNameCode = 0;

	m_dwSumOfSendPacketSize	=	0;		//	보낸 총 패킷 사이즈
	m_dwCountOfSendPacket	=	0;			//	보낸 패킷 수

	m_dwCountOfCheckSendPacketSize	=	0;	//	최초로 패킷을 보내고 난후 지난 분

	m_dwSumOfSendPacketSizePerMinute	=	0;	//	분당 보낸 패킷 사이즈 합
	m_dwCountOfSendPacketSizePerMinute	=	0;	//	분당 보낸 패킷 수

	m_dwLastCheckSendPacketSizeTime	=	0;	//	마지막으로 체크 한 시간

	m_dwMaxSendPacketSizePerMinute	=	0;		//	분당 보낸 최고 패킷 사이즈
	m_wMaxSendPacketCountPerMinute	=	0;		//	분당 보낸 최고 패킷 수

	m_dwThisTimeSendPacketSizeSum	=	0;		//	이번에 보낸 패킷 사이즈
	m_wThisTimeSendPacketCount		=	0;			//	이번에 보낸 패킷 수

	m_dwSumOfReceivePacketSize		=	0;		//	받은 총 패킷 사이즈
	m_dwCountOfReceivePacket		=	0;			//	받은 패킷 수

	m_dwCountOfCheckReceivePacketSize	=	0;	//	최초로 패킷을 벋고 난후 지난 분

	m_dwSumOfReceivePacketSizePerMinute	=	0;	//	분당 받은 패킷 사이즈 합
	m_dwCountOfReceivePacketSizePerMinute=	0;	//	분당 받은 패킷 수

	m_dwLastCheckReceivePacketSizeTime	=	0;	//	마지막으로 체크 한 시간

	m_dwMaxReceivePacketSizePerMinute	=	0;		//	분당 받은 최고 패킷 사이즈
	m_wMaxReceivePacketCountPerMinute	=	0;		//	분당 받은 최고 패킷 수

	m_dwThisTimeReceivePacketSizeSum	=	0;		//	이번에 받은 패킷 사이즈
	m_wThisTimeReceivePacketCount		=	0;			//	이번에 받은 패킷 수

	memset(id,0,dID_LENGTH);
	memset(ip,0,dIP_SIZE);
}

void
CClient::operateReceivePacketDataStaticsWork(int _iSize)
{
	m_dwSumOfReceivePacketSize			+=	_iSize;
	m_dwThisTimeReceivePacketSizeSum	+=	_iSize;
	m_wThisTimeReceivePacketCount++;
	m_dwCountOfReceivePacket++;

	if	(m_dwLastCheckReceivePacketSizeTime	&&	g_dwCurrentTime	>=	m_dwLastCheckReceivePacketSizeTime+60*1000)
	{
		if	(m_dwThisTimeReceivePacketSizeSum	>	m_dwMaxReceivePacketSizePerMinute)
		{
			m_dwMaxReceivePacketSizePerMinute	=	max(m_dwMaxReceivePacketSizePerMinute,m_dwSumOfReceivePacketSize);
			m_wMaxReceivePacketCountPerMinute	=	m_wThisTimeReceivePacketCount;
		}

		m_dwCountOfCheckReceivePacketSize++;
		
		m_dwSumOfReceivePacketSizePerMinute	+=	m_dwThisTimeReceivePacketSizeSum;	//	
		m_dwCountOfReceivePacketSizePerMinute	+=	m_wThisTimeReceivePacketCount;

		m_dwThisTimeReceivePacketSizeSum		=	0;
		m_wThisTimeReceivePacketCount			=	0;
		m_dwLastCheckReceivePacketSizeTime		=	g_dwCurrentTime;
	}

	m_dwCountOfReceivePacket++;

	if	(m_dwLastCheckReceivePacketSizeTime	==	0)
		m_dwLastCheckReceivePacketSizeTime		=	g_dwCurrentTime;
}

void
CClient::operateSendPacketDataStaticsWork(int _iSize)
{
	m_dwSumOfSendPacketSize			+=	_iSize;
	m_dwThisTimeSendPacketSizeSum	+=	_iSize;
	m_wThisTimeSendPacketCount++;
	m_dwCountOfSendPacket++;

	if	(m_dwLastCheckSendPacketSizeTime	&&	g_dwCurrentTime	>=	m_dwLastCheckSendPacketSizeTime+60*1000)
	{
		if	(m_dwThisTimeSendPacketSizeSum	>	m_dwMaxSendPacketSizePerMinute)
		{
			m_dwMaxSendPacketSizePerMinute	=	max(m_dwMaxSendPacketSizePerMinute,m_dwSumOfSendPacketSize);
			m_wMaxSendPacketCountPerMinute	=	m_wThisTimeSendPacketCount;
		}

		m_dwCountOfCheckSendPacketSize++;
		
		m_dwSumOfSendPacketSizePerMinute	+=	m_dwThisTimeSendPacketSizeSum;	//	
		m_dwCountOfSendPacketSizePerMinute	+=	m_wThisTimeSendPacketCount;

		m_dwThisTimeSendPacketSizeSum		=	0;
		m_wThisTimeSendPacketCount			=	0;
		m_dwLastCheckSendPacketSizeTime		=	g_dwCurrentTime;
	}

	m_dwCountOfSendPacket++;

	if	(m_dwLastCheckSendPacketSizeTime	==	0)
		m_dwLastCheckSendPacketSizeTime		=	g_dwCurrentTime;
}

// void	CClient::Kill()
// {
// 	isUsed				=	eISUSED_KILL;
// 	m_wIsCorrectLogout	=	FALSE;
// 	
// 	if (INVALID_SOCKET != sock) 
// 	{ 
// 		KillSocket(&sock);
// 	} 
// }
// 
// void		CClient::SetIsUsed(eISUSED isused)
// {
// 	UsedUser();
// 	isUsed = isused;
// 	if(isused<eISUSED_FIRSTOPEN)	isClose = 1;
// 	else							isClose = 0;
// 	NotUsedUser();
// }

void		CClient::SetID(char *nid,char *_lpstrToken)
{	
#ifdef	_OGP_SERVICE
	if	(_lpstrToken && _lpstrToken[0])
	{
		memcpy(m_strToken,_lpstrToken,dTOKEN_LENGTH);

		m_strToken[dTOKEN_LENGTH-1]	=	NULL;
	}
	else
		m_strToken[0]	=	NULL;
#endif

	memcpy(id,nid,dID_LENGTH);	

	id[dID_LENGTH-1]	=	NULL;
	
	hashIdCode = GetHashCode((BYTE *)id);
}

BOOL	CClient::isWaste()
{
	DWORD	checkTime = timeGetTime();
	if(checkTime < timegap)
	{
		_log(" 이럴 경우도 있을 수 있겠네요. checkTime[%u] timegap[%u] id [%s]",checkTime, timegap, id);
		checkTime  = 0;
		timegap = checkTime;
	}else
		checkTime -= timegap;
	if ( checkTime > 60000 )	return TRUE;

	return FALSE;
}

int		CClient::AddRecvPacket(char *data,int size)
{
	int time = timeGetTime() - timegap;
	if(abs(time) < 100)
	{
		++timegap_count;
		if(timegap_count>500)
		{
			_log(" 잡아내 이 유저.. ip[%s] id[%s] count %d",ip,id,timegap_count);
			return 0;
		}
	}else
	{
		if(timegap_count>0)
			--timegap_count;
	}
	timegap = timeGetTime();

	int ret = rBuffer->AddData(data,size);
#ifdef _DEBUG_FOR_BUFFER_SAFETY
	if(!ret)
	{
		return 0;
	}
	if(rBuffer->GetSize()>5000)
	{
//		_log(" CHECK ADD RECV : id[%s],name[%s], time [recv%d,send%d,sendcheck%d]",GetID(),GetName(),rBuffer->m_dwLastRecvTime,rBuffer->m_dwLastSendTime,rBuffer->m_dwLastSendCheckTime);
	}
#endif
	return ret;
}
int		CClient::AddRecvPacket(int size){	return AddRecvPacket(rOL.buf.buf ,size);	}
int		CClient::GetRecvPacket(char * recvresult)
{
	return rBuffer->GetData(recvresult,1);
}

int		CClient::AddSendPacket(char *data,int size)
{
	//	2(type),4(seq),4(16의 배수)
	int	ret = sBuffer->AddData(data,size);

#ifdef _DEBUG_FOR_BUFFER_SAFETY
	if(!ret)
	{	
//		_log(" ADD SEND ERROR : id[%s],name[%s], time [recv%d,send%d,sendcheck%d]",GetID(),GetName(),sBuffer->m_dwLastRecvTime,sBuffer->m_dwLastSendTime,sBuffer->m_dwLastSendCheckTime);
	}
	if(sBuffer->GetSize()>5000)
	{
//		_log(" CHECK ADD SEND : id[%s],name[%s], time [recv%d,send%d,sendcheck%d]",GetID(),GetName(),sBuffer->m_dwLastRecvTime,sBuffer->m_dwLastSendTime,sBuffer->m_dwLastSendCheckTime);
	}
#endif
	return ret;
}

int		CClient::CompleteSendPacket(int size)
{
	if(sendpacket_size>size)	//	하나의 데이터가 완전하게 보내지지 않았다면..
	{
		_log(" [%d] CHECK CHECK : send data is devided.  allsize %d ,sended size %d ",wSerial,sendpacket_size,size);
		char	tmpPacket[dMAX_PACKET_LENGTH];memset(tmpPacket,0,dMAX_PACKET_LENGTH);
		memcpy(tmpPacket,sendpacket+size,sendpacket_size-size);
		memset(sendpacket,0,dMAX_PACKET_LENGTH);
		sendpacket_size -= size;
		memcpy(sendpacket,tmpPacket,sendpacket_size);
		return sendpacket_size;
	}else if (sendpacket_size<size)
	{
		_log(" [%d] CHECK CHECK : send data is incorreted.  allsize %d ,sended size %d ",wSerial,sendpacket_size,size);
	}
	sBuffer->Next();
	return 0;
}

/*****************************************************************************************
	Class	Client Manager
*****************************************************************************************/
CClientManager::CClientManager()
{	
	FPInitCS(&csKILLSOCKET);
	FPInitCS(&csDeque);
	Init();	
}

CClientManager::~CClientManager()
{	
	End();
	DeleteCriticalSection(&csDeque);
}
CClientManager & CClientManager::GetInstance()
{	static CClientManager cmanager;
	return cmanager;
}

WORD	CClientManager::NextNext()
{
	for(int _next = m_wNext;_next<dGAME_MAX_USER_COUNT;_next++)
	{
		if(User[_next].wSerial == 0xffff)
		{
			m_wNext = _next;
			return m_wNext;
		}
			
	}
	int end = m_wNext;
	for(_next = 0;_next<end;_next++)
	{
		if(User[_next].wSerial == 0xffff)
		{
			m_wNext = _next;
			return m_wNext;
		}
	}
	_log("NEXTNEXT ERROR ERROR ERROR ERROR");
	return 0xffff;
}

void	CClientManager::Init()
{
	m_wNext = 0;		
	m_wCount = 0;

	for(int i=0;i<dGAME_MAX_USER_COUNT;i++)
		User[i].Close();
}

void	CClientManager::End()
{
	Init();
}

int
CClientManager::checkSize()
{
	return	0;
	
	int checkCount = 0;
	for(int i=0;i<dGAME_MAX_USER_COUNT;i++)
	{
		if( User[i].GetIsUsed() )
			++checkCount;
	}

	if( abs( checkCount - m_wCount ) > 10)	//	격차가 크다면.
	{
		printf("!!!!!!!!!!!!!!!!!!!!1\n");
		MessageBox(NULL,"CClientManager::checkSize","",MB_OK);
		_log("ERROR(20110105) : CClientManager의 m_wCount [%d], checkCount [%d]\n CClientManager has a problem what clients count!!", m_wCount, checkCount);
		m_wCount = checkCount;
	}
	return 0;
}

CClient*	CClientManager::Add()
{
	CCritical	CS(&csDeque);

	WORD	wResult = 0xffff;
	if(m_wCount == dGAME_MAX_USER_COUNT)	return NULL;
	if(m_wNext == 0xffff)					return NULL;
	wResult = m_wNext;
	User[m_wNext].Open(m_wNext);
//	User[m_wNext].SetIsUsed(eISUSED_ADD);

	m_wCount++;
	NextNext();
	return &User[wResult];
}

void	CClientManager::Del(WORD	_serial)
{
	CCritical	CS(&csDeque);

	if(_serial>=dGAME_MAX_USER_COUNT)	return;
	if(User[_serial].wSerial == 0xffff)	return;

	User[_serial].Close();
	if(m_wCount==0)	_log("ERROR CClientManager::Del m_wCount = 0");
	else			m_wCount--;
	if(m_wNext == 0xffff)	m_wNext = _serial;
}

//
//	해당 유저를 삭제한다.
BOOL
CClientManager::removeClientWithCheckId(int _iIndex,char *_lpstrId)
{
	CCritical	CS(&csDeque);

	WORD	_serial = (WORD)_iIndex;
	if	(_serial >= dGAME_MAX_USER_COUNT)
	{
		_log("ERROR removeClientWithCheckId : _iIndex = %d",_iIndex);
	}
	else
	{
		DWORD hash = GetHashCode((BYTE *)_lpstrId);
		if (User[_serial].hashIdCode == hash && STRICMP(_lpstrId,User[_serial].id) == 0)	//	정상적인 경우
		{
			User[_serial].Close();
			if(m_wCount==0)
				_log("ERROR CClientManager::removeClientWithCheckId m_wCount = 0");
			else
				m_wCount--;

			if(m_wNext == 0xffff)
				m_wNext = _serial;

			return TRUE;
		}
		else
		{
			CLOG("error","in CClientManager::removeClient - STRICMP(_lpstrId,User[_iIndex].GetID()) != 0");
		}
	}
	return	FALSE;
}


CClient		*CClientManager::GetClientPnt(char *id,int flag)	//	ID로 CClient 포인터 얻기
{
	CCritical	CS(&csDeque);

	CClient	* pClient = NULL;
	DWORD hash = GetHashCode((BYTE *)id);
	if(flag)
	{
		for(WORD i=0;i<dGAME_MAX_USER_COUNT;i++)
		{
			if(User[i].GetIsUsed() && User[i].hashNameCode == hash && STRICMP(id,User[i].name)==0)
			{
				pClient = &User[i];
				break;
			}
		}
	}
	else
	{
		for(WORD i=0;i<dGAME_MAX_USER_COUNT;i++)
		{
			if(User[i].GetIsUsed() && User[i].hashIdCode == hash && STRICMP(id,User[i].id)==0)
			{	
				pClient = &User[i]; 
				break; 
			}
		}
	}
	return pClient;
}

//	used flag를 무시하고 계정으로 포인터 얻기
CClient*
CClientManager::getClientByIdIgnoreUsedFlag(char *_lpstrId)
{
	CCritical	CS(&csDeque);
	CClient	* pClient	=	NULL;

	DWORD hash = GetHashCode((BYTE *)_lpstrId);

	for(WORD i=0;i<dGAME_MAX_USER_COUNT;i++)
	{
		if(User[i].wSerial!= 0xffff && User[i].hashIdCode == hash && STRICMP(_lpstrId,User[i].id)==0)
		{
			pClient	=	&User[i];
			break;
		}
	}
	return pClient;
}

//	used flag를 무시하고 이름으로 포인터 얻기
CClient*
CClientManager::getClientByNameIgnoreUsedFlag(char *_lpstrName)
{
	CCritical	CS(&csDeque);
	CClient	* pClient	=	NULL;
	DWORD hash = GetHashCode((BYTE *)_lpstrName);

	for(WORD i=0;i<dGAME_MAX_USER_COUNT;i++)
	{
		if(User[i].wSerial!= 0xffff && User[i].hashNameCode == hash && STRICMP(_lpstrName,User[i].name)==0)
		{
			pClient	=	&User[i];
			break;
		}
	}
	return pClient;
}

CClient		*CClientManager::GetClientPnt(WORD _serial)	//	serial로 CClient 포인터 얻기
{
	CClient	* pClient = NULL;

	if(_serial<dGAME_MAX_USER_COUNT)
	{
		if(User[_serial].GetIsUsed())
		{	pClient = &User[_serial];	
		}
	}
	return pClient;
}


CClient		*CClientManager::GetClientPntByIgnoreUsedFlagById(char * _strId)
{
	return getClientByIdIgnoreUsedFlag(_strId);
}

CClient		*CClientManager::GetClientPntByIgnoreUsedFlag(int idx)	//	serial로 CClient 포인터 얻기
{
	CCritical	CS(&csDeque);
	CClient	* pClient = NULL;

	WORD	_serial = (WORD)idx;
	if(_serial<dGAME_MAX_USER_COUNT)
	{
		if(User[_serial].wSerial != 0xffff)
		{	
			pClient = &User[_serial];
		}
	}
	return pClient;
}

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	패킷 사이즈 
int	GetPacketSize(WORD	pType)
{
	switch(pType)
	{
	case dCG_CONNECT_GAME:	return sizeof(CG_CONNECT_GAME);
	}
	return 0;
}