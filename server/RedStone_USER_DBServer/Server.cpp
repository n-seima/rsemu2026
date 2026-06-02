// Server.cpp : Defines the entry point for the application.
//
/**********************************************************************************
	DB Cache 서버 
**********************************************************************************/

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <iostream>

#include "cSOCKET.h"
#include "Client.h"
#include "cScript.h"
#include "cSRVUTIL.h"
#include "cPACKET_USERDBSERVER.h"
#include "cADO_USER.h"
#include "cADO_COMMAND.h"
#include "Hardwareinfo.h"
#include "event.h"
#include "life_code.h"
#include "../buildVersion.h"
#include "cPACKET_WORLDSERVER.h"
#include "inputConsoleCommand.h"
#include "reg_common.h"

#define dUSE_SERVERSTATUS_REPORT

#ifdef _FOR_THAI
#undef dUSE_SERVERSTATUS_REPORT
#endif

#ifdef dUSE_SERVERSTATUS_REPORT
	#include "cServerStatus.h"
#endif


#define	dLOGIN_SERVER_ID	"#login"
#define	dADMIN_TOOL_ID		"#admin"

#define _PACKET_OPTION_CRYPT		//	암호화해서 패킷을 날린다.
#define _PACKET_OPTION_SEND_AT_ONE	//	행동 데이타를 한번에 보낸다.
#define dDB_CONNECT_TIME			100000
#define dCONFIGFILENAME				"config-userdb.txt"
#define MSG(a,b)					MessageBox(NULL,a,b,MB_OK);

#define	dMAX_BUFFER			4096

static int socketcount=0;

CRITICAL_SECTION csRECVPacket;
CRITICAL_SECTION csSENDPacket;
CRITICAL_SECTION csRemoveUser;

static BOOL		isFirst	=TRUE;
static BOOL		isReady =FALSE;
static BOOL		g_bThreadLoop = TRUE;
static char		strUserDBServerIp[16];
static char		strUserDBServerId[64];
static char		strUserDBServerPw[64];
static char		strUserDBServerSource[128];

char	l_strServerIp[32]=	"";

//	내부 테스트를 위한 
BOOL	l_bIsTestDB		=	FALSE;

HANDLE			g_hCompletionPort;		//	IOCP에 사용되는 포트
SOCKET			lisSock;		//	listen 소켓
HANDLE			hEventMngThread,hWorkerThread[dMAX_WORKER_COUNT],hAdditionThread,hCommandThread;
HANDLE			l_hLifeCodeThread	=	NULL;	//	생존 코드 쓰레드
HANDLE			l_hConsoleCommandThread	=	NULL;	//	생존 코드 쓰레드
HANDLE			l_hAcceptThread			=	NULL;

int			nTimer;
cScript		scp;
cADO_USER		USERDB;
cADO_COMMAND	COMMANDDB;
cCarrotShopDefine	g_cCarrotShop;
void SendCarrotShop(CClient * _cpUser,BOOL _bIsUpdateNow);

CClientManager & UserManager  = CClientManager::GetInstance();

BOOL	InitSocket();
BOOL	InitCompletionPort();		//	IOCP 초기화
//static DWORD WINAPI WorkerThread(LPVOID lpParameter); 
//static DWORD WINAPI AdditionThread(LPVOID lpParameter); 
void	WorkerThread(void * lpParameter); 
void	AdditionThread(void * lpParameter); 
void	CommandThread(void * lpParameter);
void	ConsoleCommandThread(void * lpParameter);
void	EventMngThread(void * lpParameter);
void	LifeCodeUpdateThread(LPVOID lpParameter);
void	AcceptThread(LPVOID lpParameter);

void	SendNotice(int target, char * msg, int isUserMsg=0, char * who = NULL);
void	SendToLogin(char * _packet,int size);

BOOL RecvPacket(int serial);
BOOL SendPacket(int serial);
void SendPacket_Add(int serial);

void	ProcessRecvPacket(int serial);
BOOL	ProcessCommand(char command);
void	ProcessDBDisconnect();

//	method for packet send
void	DisUser(int serial);

//	end define

//////////////////////////////////////
//	기타 기능을 위한 플래그 
BOOL	isSend_UserCount	=	FALSE;

DebugTracer		* cLOG;

void main()
{
	{
		char	strTitle[512];

#ifdef	_FOR_KOREA
		sprintf(strTitle,"RS UserDB server for korea. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_JAPAN
		sprintf(strTitle,"RS UserDB server for japan. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_THAI
		sprintf(strTitle,"RS UserDB server for thai. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_USA
		sprintf(strTitle,"RS UserDB server for USA. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#else
		sprintf(strTitle,"RS UserDB server. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#endif

		SetConsoleTitle(strTitle);
		
		WriteServerInfoToRegistry(eST_USER_DBC,"User DBC Server");
	}

	srand(timeGetTime());
	scp.Read(dCONFIGFILENAME);

	g_pEventManager	=	new cEventManager();

	{
		char	*lpstrText	=	scp.getData("IS_TEST_SERVER");

		if	(lpstrText && strlen(lpstrText) && stricmp(lpstrText,"true") == 0)
			l_bIsTestDB	=	TRUE;
	}

	if	(l_bIsTestDB)
		printf("for inner test server\n");

#ifdef dUSE_SERVERSTATUS_REPORT
// 	if	(l_bIsTestDB	==	FALSE)
// 	{
// 		if	(!g_SS.init("ACCOUNT", "DBC"))
// 		{	
// 			MessageBox(NULL,"ERROR IN SS","WARNING!!",MB_OK);
// 			return ;
// 		}
// 	}
#endif
	printf(" SS Init End\n");
	char path[1024];
	GetCurrentDirectory(1024,path);

	g_log.setLogFolderName("gameLog");

	FILE * dbcfp = fopen("AccountDbInfo.bin","rb");

	if	(!dbcfp)
	{
		ERRMSG("Can not read Account db connection info");
		return;
	}

	fread(strUserDBServerSource,128,1,dbcfp);
	fread(strUserDBServerId,64,1,dbcfp);
	fread(strUserDBServerPw,64,1,dbcfp);
	fclose(dbcfp);

	//	시작전에 log파일 검사해서 수정할 내용들을 먼저 수정해라.
	hAdditionThread	=	NULL;
	hCommandThread	=	NULL;
	hEventMngThread	=	NULL;

	int threadcouant=0;

	for	(threadcouant=0;threadcouant<dMAX_WORKER_COUNT;threadcouant++)
		hWorkerThread[threadcouant] = NULL;
	//	윈도우 끝나면 나머지 초기화 
	//	스택 할당하고...

	UserManager.SetBaseSize(sizeof(cMSG_BASE_TYPE_FORUSERDB));
	InitializeCriticalSection(&csSENDPacket);
	InitializeCriticalSection(&csRECVPacket);
	InitializeCriticalSection(&csRemoveUser);

	strcpy(strUserDBServerIp,scp.getData("IP"));

	{
		char	*lpstrDay	=	scp.getData("UPDATE_WEEKLY_RANKIG_WEEK_DAY");

		if	(lpstrDay && strlen(lpstrDay))
			g_pEventManager->m_iUpdateWeeklyRankingDay	=	atoi(lpstrDay);
	}

// #ifndef _FOR_KOREA
// 	if	(strncmp(strUserDBServerIp,"211.189",7)==0)
// 		return;
// #endif

	cLOG = new DebugTracer(DTType_ALL,"RedStone_USERDBServer");

	if	(!InitSocket())
	{
		_log("Error InitSocket \n");
		return;
	}

#ifdef _USE_CARROTSHOP
	Sleep(1000);

	USERDB.GetCarrotShopInfo(&g_cCarrotShop,1);

	_log(" Carrot Shop Item Count at First = %d",g_cCarrotShop.m_wCategoryCount);
#endif
	_log("Start USER DB Cache Server ... day : %d - time : %d \n",cSRVUTIL::GetTime(),cSRVUTIL::GetTimes() );

	while(g_bThreadLoop)
	{
		Sleep(1);
	}

	Sleep(1000);

	while(1)
	{
		Sleep(1000);

		_log("Wait for end all worker thread");
		int	 iDeathWorkerCount	=	0;

		for (int i=0;i<dMAX_WORKER_COUNT;i++)
		{
			if	(hWorkerThread[i]	==	NULL)
				iDeathWorkerCount++;
		}

		if	(iDeathWorkerCount	==	dMAX_WORKER_COUNT)
			break;
	}

	DeleteCriticalSection(&csSENDPacket);
	DeleteCriticalSection(&csRECVPacket);
	DeleteCriticalSection(&csRemoveUser);

	WriteServerStatusToRegistry(eST_USER_DBC,eSERVER_STATUS_OFF);

	_log(" DB Server is Exit. \n");
}

BOOL InitSocket()
{
	SOCKADDR_IN serverAddress;
	WSADATA wsadata;

	if	(!USERDB.Connect(strUserDBServerIp,strUserDBServerId,strUserDBServerPw,strUserDBServerSource))
	{	
		_log("USER DB Connect Error !!\n");
		return FALSE;
	}
	if	(!COMMANDDB.Connect(strUserDBServerIp,strUserDBServerId,strUserDBServerPw,"REDSTONE_INFO",4))
	{	
		_log("Command DB Connect Error !!\n");
		USERDB.Close();
		return FALSE;
	}
	_log("Success DB Connecting ... \n");

	if	(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
	{	
		_log("Fail WSAStartup !! \n");
		return FALSE;
	}

	if	((lisSock = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{	
		_log("Fail listen !!");
		return FALSE;
	}

	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(dUSERDBCACHE_PORT);

	if	(bind(lisSock,(LPSOCKADDR)&serverAddress,sizeof(serverAddress)) == SOCKET_ERROR)
	{	
		_log("Fail bind !!\n");
		return FALSE;	
	}

	listen(lisSock,5);

	{
		char name[32];
		gethostname(name,sizeof(name));
		struct hostent* he = gethostbyname(name);
		struct in_addr iaddr;
		memset(&iaddr,0,sizeof(iaddr));
		
		if	(l_strServerIp[0]	==	NULL)
		{
			for(int i =0; he->h_addr_list[i] !=0; i++)
			{
				if	(he->h_addrtype!=AF_INET)
					continue;

				memcpy(&iaddr,he->h_addr_list[i],sizeof(iaddr));
				strcpy(l_strServerIp,inet_ntoa(iaddr));

				_log("Open Server addr = %s\n",l_strServerIp);
				break;
			}
		}

	}

	if(!InitCompletionPort())
	{	_log("Fail Init IOCP Port !!");
		return FALSE;
	}
	hAdditionThread = (HANDLE)_beginthread(AdditionThread, 0, NULL); 
	if	(!hAdditionThread)
		return FALSE;

	hCommandThread = (HANDLE)_beginthread(CommandThread, 0, NULL); 
	if	(!hCommandThread)	
		return FALSE;

	hEventMngThread = (HANDLE)_beginthread(EventMngThread, 0, NULL); 
	if	(!hEventMngThread)
		return FALSE;

	l_hLifeCodeThread	=	(HANDLE)_beginthread(LifeCodeUpdateThread, 0, NULL); 
	if	(!l_hLifeCodeThread)
		return FALSE;

	l_hConsoleCommandThread	=	(HANDLE)_beginthread(ConsoleCommandThread, 0, NULL); 
	if	(!l_hConsoleCommandThread)
		return FALSE;

	l_hAcceptThread			=	(HANDLE)_beginthread(AcceptThread, 0, NULL); 
	if	(!l_hAcceptThread	)
		return FALSE;

	return TRUE;
}

BOOL InitCompletionPort()
{
	SYSTEM_INFO sys_info;
	DWORD dwThreadNum;

	g_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,0);
	if(g_hCompletionPort==NULL)
	{
		_log("Fail IOCP Port Create !!");
		return FALSE;
	}

	GetSystemInfo(&sys_info);
	dwThreadNum = sys_info.dwNumberOfProcessors * 2 + 1;
	if (dwThreadNum > dMAX_WORKER_COUNT)
		dwThreadNum = dMAX_WORKER_COUNT;

	for(int i=0;i<(int)dwThreadNum;i++)
	{
		hWorkerThread[i] = (HANDLE)_beginthread(WorkerThread, 0, (void *)i); 

		if	(hWorkerThread[i]==NULL)
		{
			_log("Fail Thread begin !!");
			return FALSE;
		}
	}
	return TRUE;
}

void
AcceptThread(LPVOID lpParameter)
{
	DWORD	Flags		=	0;
	SOCKET	clientsock	=	INVALID_SOCKET;
	int		index		=	0;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_ACCEPT);

	while(g_bThreadLoop)
	{
		Sleep(1);

		if	( UserManager.Size()>=dUSERDB_MAX_USER_COUNT)
		{	
			_log("UserManager.Size()>=dUSERDB_MAX_USER_COUNT");	
			Sleep(10);	
			continue;
		}

		g_lifeCodeMgr.endCheck(eLIFE_CODE_ACCEPT);

		clientsock	=	accept(lisSock,NULL,NULL);

		if	(clientsock ==	INVALID_SOCKET)
			continue;

		if	(g_bThreadLoop	==	FALSE)
			break;

		g_lifeCodeMgr.beginCheck(eLIFE_CODE_ACCEPT);

		int buf=dMAX_PACKET_LENGTH;

		if	(SOCKET_ERROR	==	setsockopt(clientsock,SOL_SOCKET ,SO_SNDBUF,(const char*)&buf,sizeof(buf)))
		{
			closesocket(clientsock);
			clientsock = 0;
			continue;
		}

		if	(SOCKET_ERROR	==	setsockopt(clientsock,SOL_SOCKET ,SO_RCVBUF,(const char*)&buf,sizeof(buf)))
		{
			closesocket(clientsock);
			clientsock = 0;
			continue;
		}

		LINGER	lingerStruct; 
		lingerStruct.l_onoff = 1;
		lingerStruct.l_linger = 0; 

		if	(SOCKET_ERROR ==setsockopt(clientsock, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct)))
		{
			closesocket(clientsock);
			clientsock = 0;
			continue;
		}

		char	_ip[16];
		SOCKADDR_IN	saddr;		
		memset(&saddr,0,sizeof(saddr));
		int tmp = sizeof(sockaddr);
		getpeername(clientsock,(SOCKADDR *)&saddr,&tmp);
		strcpy(_ip,inet_ntoa(saddr.sin_addr));

// #ifdef _FOR_KOREA
// 		if	(l_bIsTestDB == FALSE && strncmp(_ip,"211.189",7)!=0 && strncmp(_ip,"211.235",7)!=0 && strncmp(_ip,"172.31.6",8)!=0 && strncmp(_ip,"58.150.56",8)!=0)
// 		{
// 			_log("엉뚱한 SERVER 출현 : %s",_ip);
// 			closesocket(clientsock);	
// 			clientsock = 0;	
// 			continue;
// 		}
// #endif
		index = UserManager.Add();

		if	(index==-1)
		{
			_log("Fail Add User!!!");
			continue;
		}

		CClient *newUser = UserManager.GetClientPnt(index);
		newUser->Open();
		newUser->SetSocket(clientsock);
		newUser->SetSerial(index);
		newUser->SetServerIndex(0xffff);
		_log("Add User  = %d , %s \n",UserManager.Size(),_ip);

		if	(!CreateIoCompletionPort((HANDLE)newUser->GetSocket(),g_hCompletionPort,(DWORD)index,0))
		{
			UserManager.Del(newUser->GetSerial());
			_log("Error CreateIoCompletionPort !! - %d",GetLastError());
			continue;
		}

		RecvPacket(index);
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_ACCEPT);

	_log("END AcceptLoop ...");

	return ;
}

void
LifeCodeUpdateThread(LPVOID lpParameter)
{
	//	로그인 서버 들어 올때까지 루프 돈다~
	while(g_bThreadLoop)
	{
		Sleep(10);

		if	(SendLPLifeCode(eBeginLifeSign,UserManager.GetClientPnt(dLOGIN_SERVER_ID),l_strServerIp))
			break;
	}

	WriteServerStatusToRegistry(eST_USER_DBC,eSERVER_STATUS_ON);

	while(1)
	{
		for (int iLoop=50;iLoop ;--iLoop)	//	5초간 기다리는데 그 와중에 쓰레드가 끝나면..
		{
			Sleep(100);

			if	(!g_bThreadLoop)
			{
				SendLPLifeCode(eEndLifeSign,UserManager.GetClientPnt(dLOGIN_SERVER_ID),l_strServerIp);

				_log("End LifeCodeUpdateThread");

				return;
			}
		}

		WriteLifeSignToRegistry(eST_USER_DBC,1);

		if	(!g_lifeCodeMgr.checkAll(timeGetTime()))
			SendLPLifeCode(eIncorrectStatusSign,UserManager.GetClientPnt(dLOGIN_SERVER_ID),l_strServerIp);
		else
			SendLPLifeCode(eUpdateLifeSign,UserManager.GetClientPnt(dLOGIN_SERVER_ID),l_strServerIp);
	}

	_log("End LifeCodeUpdateThread");
}

void
WorkerThread(void * lpParameter)
{
	LP_IO_DATA	lpOV;
	DWORD		dwRead,dwIOCPKey;
	int			threadSerial = (int)lpParameter;
	_log("Start WorkerThread[%d] ...",threadSerial);
	BOOL retVal = TRUE;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_WORKER);

	while(g_bThreadLoop)
	{
		dwIOCPKey = 0xffffffff;

		g_lifeCodeMgr.endCheck(eLIFE_CODE_WORKER);

		retVal = GetQueuedCompletionStatus(g_hCompletionPort,&dwRead,&dwIOCPKey,(LPOVERLAPPED *)&lpOV,5000);

		g_lifeCodeMgr.beginCheck(eLIFE_CODE_WORKER);

		if	( retVal == TRUE && dwIOCPKey < dUSERDB_MAX_USER_COUNT && dwRead != 0 && lpOV != 0 )
		{
			CClient	* user = UserManager.GetClientPnt(dwIOCPKey);

			if	(!user)
				continue;
			
			if	(lpOV->IOState == 0)
			{
				user->AddRecvPacket(dwRead);
				RecvPacket(dwIOCPKey);
				ProcessRecvPacket(dwIOCPKey);
			}
			else
			if	(lpOV->IOState == 1)
			{
				int	result_send = user->CompleteSendPacket(dwRead);

				if	(result_send>0)
					SendPacket_Add(dwIOCPKey);
				else
					SendPacket(dwIOCPKey);
			}
		}
		else
		{
			if	(dwIOCPKey >= dUSERDB_MAX_USER_COUNT)
				continue;

			if	(!lpOV)
				_log("SERROR : getqueue... lpOV = NULL");

			if	(dwRead == 0 )
			{
				CClient	* user = UserManager.GetClientPnt(dwIOCPKey);

				if	(!user)
				{
					_log("SERROR : getqueue... user==NULL || !user->GetIsUsed()");
					continue;	
				}

				user->PreClose();
				DisUser(dwIOCPKey);
			}
			else
			{
				DWORD dwError = GetLastError();

				_log("SERROR GetQueue Error : %d ",dwError);

				if	(dwError==ERROR_NETNAME_DELETED)
					_log("dwError ==ERROR_NETNAME_DELETED");

				if	(dwError !=ERROR_IO_PENDING && dwError!=ERROR_OPERATION_ABORTED && dwIOCPKey < dUSERDB_MAX_USER_COUNT)
				{
					_log("%d Worker thread error GetQueueCompletionStatus... error [%d]",threadSerial,dwError);
					DisUser(dwIOCPKey);
				}
			}
		}
	}

	hWorkerThread[threadSerial]	=	NULL;

	g_lifeCodeMgr.endCheck(eLIFE_CODE_WORKER);

	_log("END WorkerThread[%d] ...",threadSerial);
	return;
}

BOOL RecvPacket(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);
	if(!user || user->GetIsClose())	return FALSE;
//	user->UsedBuffer();
	DWORD	lpRecv;
	DWORD	Flags = 0;
	user->rOL.IOState = 0;

	if(WSARecv(user->GetSocket(), &user->rOL.buf ,1,&lpRecv,&Flags,&user->rOL.OL ,NULL) ==SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();
		if(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
//			user->NotUsedBuffer();
			DisUser(serial);
			return FALSE;
		}
	}
//	user->NotUsedBuffer();
	return TRUE;
}

BOOL SendPacket(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);
	if(!user || user->GetIsClose())	return FALSE;
	user->UsedBuffer();
	if(user->GetSendBufLength()>=512000 && user->isSending)
	{
		_logf("[%s] ERROR isSending = %d %d" ,user->GetID(),user->isSending,user->GetSendBufLength());
		user->NotUsedBuffer();
		DisUser(serial);
		return TRUE;
	}
	if(user->isSending || user->GetSendBufLength()<sizeof(cMSG_BASE_TYPE_FORUSERDB)){	user->NotUsedBuffer();	return TRUE;	}
	user->sOL.IOState = 1;
	DWORD	lpSend;	
	WSABUF	buf;
	buf.buf = user->lpSendBuf;

	WORD	s = 0,t = 0;
	memcpy(&s,user->lpSendBuf,sizeof(WORD));
	memcpy(&t,user->lpSendBuf+sizeof(WORD),sizeof(WORD));
	if(s>user->GetSendBufLength())
	{
		user->NotUsedBuffer();
		return TRUE;
	}
	user->sendpacket_size = s;
	buf.len = user->sendpacket_size;

	if(t<0x10 || t>=0x9500 || s>=41960 || s==0)
	{	//	잘못된 패킷 .. 내용 초기화 한다. 
		memset(user->lpSendBuf,0,dMAX_PACKET_LENGTH_SERVER);
		user->sbuf_length = 0;
		user->sendpacket_size = 0;
		user->NotUsedBuffer();
		return TRUE;
	}
	user->isSending = TRUE;
	if(WSASend(user->GetSocket(), &buf,1,&lpSend,0,&user->sOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();
		if(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{	
			user->isSending = FALSE;
			user->NotUsedBuffer();
			DisUser(serial);
			return FALSE;
		}
	}
	user->NotUsedBuffer();
	return TRUE;
}

void	SendPacket_Add(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);
	if(!user || user->GetIsClose())	return;
	user->UsedBuffer();
	user->sOL.IOState = 1;
	DWORD	lpSend;	WSABUF	buf;
	buf.buf = user->lpSendBuf;
	buf.len = user->sendpacket_size;

	if(WSASend(user->GetSocket(), &buf,1,&lpSend,0,&user->sOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();
		if(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			user->isSending = FALSE;
			user->NotUsedBuffer();
			DisUser(serial);
			return;
		}
	}
	user->NotUsedBuffer();
}

void	ProcessRecvPacket(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);
	if(!user || user->GetIsClose())		return;
	WORD		type,recvsize;
	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	int resultGetRecvPacket=0;
	while(TRUE)
	{
		resultGetRecvPacket = user->GetRecvPacket(recvpacket);

		if(resultGetRecvPacket==0)	break;
		memcpy(&recvsize,recvpacket,sizeof(WORD));
		memcpy(&type,recvpacket+sizeof(WORD),sizeof(WORD));
		switch(type)
		{
		case dWU_ASK_DB_CONTROLLED_EVENT_INFO:
			{	//	월드 서버가 가동하거나 혹은 중간에 이벤트 정보를 얻어오기 위한 요청한 패킷
				WU_ASK_DB_CONTROLLED_EVENT_INFO * rPacket = (WU_ASK_DB_CONTROLLED_EVENT_INFO *)recvpacket;
				UW_DB_CONTROLLED_EVENT_INFO	sPacket;
				sPacket.base.set( sizeof(UW_DB_CONTROLLED_EVENT_INFO), dUW_DB_CONTROLLED_EVENT_INFO, rPacket->base.wSerial);
				USERDB.GetList_EventInGame(sPacket.aiValues);
				_log("CUSTOM EVENT DATA : 0 = %d, 1 = %d ",sPacket.aiValues[0],sPacket.aiValues[1]);
				user->AddSendPacket( (char *)&sPacket, sPacket.base.wSize);
				_log("SEND EVENT INFO TO %s , type 0x%x , size %d , wSerial %d",user->GetIP(),sPacket.base.wType, sPacket.base.wSize,sPacket.base.wSerial);
			}
			break;

		case	dWU_UPDATE_DUEL_TEAM_RECORD	:
		{
			WU_UPDATE_DUEL_TEAM_RECORD	*rPacket	=	(WU_UPDATE_DUEL_TEAM_RECORD	*)recvpacket;

//			rPacket->strId의 정보를 rPacket->record의 정보로 업데이트 시킨다.
			USERDB.SetPvPTeamInfo( rPacket->strTeamName, &rPacket->record);

			break;
		}
		case	dWU_UPDATE_DUEL_RECORD	:
		{
			WU_UPDATE_DUEL_RECORD	*rPacket	=	(WU_UPDATE_DUEL_RECORD *)recvpacket;

			CLOG("duel_record","id [%s] point [%d] total [%d/%d/%d/%d] weekly [%d:%d/%d/%d/%d]",
				rPacket->record.m_strID,
				rPacket->record.m_iDuelPoint,
				rPacket->record.m_iBattleCount,	//	전투 횟수
				rPacket->record.m_iWinCount,	//	승리 횟수
				rPacket->record.m_iDefeatCount,	//	패배 횟수
				rPacket->record.m_iDrawCount,	//	무승부 횟수

				rPacket->record.m_iWeeklyWinPoint,		//	주간 결투 승점
				rPacket->record.m_iWeeklyBattleCount,	//	주간 결투 횟수
				rPacket->record.m_iWeeklyWinCount,		//	주간 결투 승리 횟수
				rPacket->record.m_iWeeklyDefeatCount,	//	주간 결투 패배 횟수
				rPacket->record.m_iWeeklyDrawCount);		//	주간 결투 무승부 횟수

			//	DB에서 rPacket->strTeamName의 정보의 팀에 전적을 rPacket->record의 정보로 업데이트 시킨다.
			USERDB.SetPvPData(rPacket->strId, &rPacket->record);
			break;
		}

		case	dWU_RESET_DUEL_TEAM_INFO	:
		{
			//	결투 팀 순위나 결투팀 정보등 모두 초기화
			USERDB.SetPvPTeamInfoInit();
			break;
		}
		case	dWU_ASK_DUEL_TEAM_INFO	:
		{
			UW_DUEL_TEAM_INFO	sPacket;

			sPacket.base.set(sizeof(sPacket),dUW_DUEL_TEAM_INFO);

			cDuelTeamInfo	* _pList = new cDuelTeamInfo[c_iDuelTeamMaxCount];
			int		iSendCount = 0;
			std::map<int, int>		m_teamIndexInfo;
			
			int	iRegistedTeamCount	=	USERDB.GetPvPTeamList(_pList, m_teamIndexInfo);	//	등록된 팀의 수
			USERDB.GetPvPTeamUserList(_pList,m_teamIndexInfo);	//	각 팀에 등록된 유저 정보를 읽어온다.

			//	등록된 팀의 수를 iRegistedTeamCount에 얻어 오자

			sPacket.wTeamCount		=	0;
			sPacket.wTotalTeamCount	=	iRegistedTeamCount;

			for (int iTeam=0;iTeam<iRegistedTeamCount;iTeam++)
			{
				cDuelTeamInfo	*lpTeam	=	&sPacket.aTeams[sPacket.wTeamCount];

				//	lpTeam에 결투 팀 정보 세팅하자
				memcpy( lpTeam, &_pList[iSendCount++], sizeof(cDuelTeamInfo));

				sPacket.wTeamCount++;

				if	(sPacket.wTeamCount	>=	100)	//	패킷에 버퍼를 최대 100개로 제한했다.
				{
					user->AddSendPacket( (char *)&sPacket, sPacket.base.wSize);

					sPacket.wTeamCount	=	0;
				}
			}

			if	(sPacket.wTeamCount)
			{
				sPacket.base.wSize			=	sizeof(sPacket)-sizeof(sPacket.aTeams)+(sPacket.wTeamCount)*sizeof(cDuelTeamInfo);
				user->AddSendPacket( (char *)&sPacket, sPacket.base.wSize);
			}

			delete [] _pList;
			_pList = NULL;
			break;
		}

		case dWU_ASK_DUEL_RANKING_INFO	:
		{
			WU_ASK_DUEL_RANKING_INFO	*rPacket	=	(WU_ASK_DUEL_RANKING_INFO *)recvpacket;

			UW_DUEL_RANKING_INFO	sPacket;

			sPacket.base.set(sizeof(sPacket),dUW_DUEL_RANKING_INFO,-1);

			//	일간 랭킹 전송
			{
				sPacket.wWeeklyRankerCount	=	0;
				sPacket.wTrialGameRankerCount		=	0;
				sPacket.wIsDailyRanking		=	TRUE;

				//	sPacket.wWeeklyRankerCount에 주간 전적의 일간 랭킹 랭커 수를 담자
				//	sPacket.wTrialGameRankerCount TrialGame 랭킹 랭커 수를 담자
				//	sPacket.aList에 일간 랭킹 정보를 담는다.
				//	200개의 랭킹을 담을 수 있다.
				//	여기에 앞에 100개에 일단 랭킹을 담고 뒤에 100개에 TrialGame 일간 랭킹을 담는다.
				sPacket.wWeeklyRankerCount = USERDB.GetPvPDailyRanking(sPacket.aList);
				sPacket.wTrialGameRankerCount = USERDB.GetPvPTrialGameRanking(sPacket.aList+sPacket.wWeeklyRankerCount);

				sPacket.base.wSize			=	sizeof(sPacket)-sizeof(sPacket.aList)+(sPacket.wWeeklyRankerCount+sPacket.wTrialGameRankerCount)*sizeof(cDuelRankingInfo);
				user->AddSendPacket( (char *)&sPacket, sPacket.base.wSize);
			}

			//	주간 랭킹 전송
			{
				sPacket.wWeeklyRankerCount	=	0;
				sPacket.wTrialGameRankerCount		=	0;
				sPacket.wIsDailyRanking		=	FALSE;

				//	sPacket.wWeeklyRankerCount에 주간 전적의 주간 랭킹 랭커 수를 담자
				//	sPacket.aList에 랭킹 정보를 담자
				//	200개의 랭킹을 담을 수 있다.
				//	앞에 100개에 주간 랭킹정보를 담는다. TrialGame 랭킹 정보는 없다.
				sPacket.wWeeklyRankerCount = USERDB.GetPvPWeeklyRanking(sPacket.aList);

				sPacket.base.wSize			=	sizeof(sPacket)-sizeof(sPacket.aList)+(sPacket.wWeeklyRankerCount)*sizeof(cDuelRankingInfo);
				user->AddSendPacket( (char *)&sPacket, sPacket.base.wSize);
			}
			break;
		}

		case dWU_ASK_DUEL_RECORD	:
		{
			WU_ASK_DUEL_RECORD	*rPacket	=	(WU_ASK_DUEL_RECORD *)recvpacket;
			UW_DUEL_RECORD		sPacket;

			sPacket.base.set(sizeof(sPacket),dUW_DUEL_RECORD,-1);
			memset(&sPacket.record,0,sizeof(sPacket.record));
			strcpy(sPacket.strId,rPacket->strId);
			sPacket.iGSSerialInWorld	=	rPacket->iGSSerialInWorld;

			//	packet.strId의 cDuelRecord 정보를 패킷에 담는다.
			//	DB에서 rPacket->strId의 정보를 찾아 packet.record에 담는다.

			USERDB.GetPvPData(sPacket.strId,&sPacket.record);

			if	(sPacket.record.m_iTrialGameWinPoint == -1)
			{
				memset(&sPacket.record, 0, sizeof(cDuelRecord));
			}

			user->AddSendPacket( (char *)&sPacket, sPacket.base.wSize);

			break;
		}

		case dWU_ASK_INCREASE_DB_EVENT_VALUE:
			{
				WU_ASK_INCREASE_DB_EVENT_VALUE * rPacket = (WU_ASK_INCREASE_DB_EVENT_VALUE *) recvpacket;

				UW_RESULT_OF_INCREASE_DB_EVENT_VALUE	sPacket;
				sPacket.base.set( sizeof(UW_RESULT_OF_INCREASE_DB_EVENT_VALUE),dUW_RESULT_OF_INCREASE_DB_EVENT_VALUE,rPacket->base.wSerial);
				switch( USERDB.InsertInfo_EventInGame( rPacket->bReset, rPacket->wEventIndex, rPacket->strUserID, rPacket->strUserName, rPacket->iMaxCount, rPacket->bIsAllowMulipleID , sPacket.iValue) )
				{
				case -1:	sPacket.wResult = eROIDEV_ALREADY_EXIST_USER;	break;
				case 0:		sPacket.wResult = eROIDEV_EVENT_COUNT_FULL;		break;
				case 1:		sPacket.wResult = eROIDEV_SUCCESS;				break;
				}
				if(rPacket->bReset)
				{
					sPacket.wResult = eROIDEV_RESET;
					strcpy( sPacket.strUserID , rPacket->strUserID);
					strcpy( sPacket.strUserName , rPacket->strUserName);
					sPacket.wEventIndex = rPacket->wEventIndex;
				}else
				{
					strcpy( sPacket.strUserID , rPacket->strUserID);
					strcpy( sPacket.strUserName , rPacket->strUserName);
					sPacket.wEventIndex = rPacket->wEventIndex;
				}
				user->AddSendPacket( (char *)&sPacket, sPacket.base.wSize);
			}
			break;
		case dWU_ASK_CARROTSHOPINFO:
			{
				WU_ASK_CARROTSHOPINFO * rPacket = (WU_ASK_CARROTSHOPINFO *) recvpacket;
				_log(" SendCarrotShop to user [%s]",user->GetIP());
				SendCarrotShop(user,rPacket->wIsUpdateNow);
				break;
			}
		case dWU_WORLDINFO:	//	월드 서버가 보내준 정보를 갱신합시다. 각 월드별로 분당 한번씩 날아옵니다.
			{
#ifdef _FOR_JAPAN
				break;	//	일본은 이 정보 사용 안함.
#endif

				WU_WORLDINFO	* rPacket = (WU_WORLDINFO *)recvpacket;

				for(int i=0;i<dSERVERINFO_COUNT;i++)
				{
					if	(rPacket->info[i].idx == 0 )
						continue;
#ifdef _FOR_KOREA
					COMMANDDB.UpdateServerInfo(&rPacket->info[i],rPacket->curusercountReserve);
#else
					COMMANDDB.UpdateServerInfo(&rPacket->info[i]);
#endif
				}
			}
			break;

			case dLU_LOGIN:
			{
				LU_LOGIN	cLogin;
				memset(&cLogin,0,sizeof(LU_LOGIN));
				memcpy(&cLogin,recvpacket,recvsize);
				
				UL_RESULT_LOGIN	sLogin;
				memset(&sLogin,0,sizeof(UL_RESULT_LOGIN));
				std::string		strResult;
				WORD isOperInAccount = 0;
				strResult = USERDB.Login(cLogin.strId,cLogin.strPassword,cLogin.strMacAddress,cLogin.strClientCode,cLogin.strSelectServer,cLogin.strLoginIp, sLogin.strWhy,isOperInAccount,sLogin.isOTPUser,FALSE);
				sLogin.base.set(sizeof(UL_RESULT_LOGIN),dUL_RESULT_LOGIN,cLogin.base.wSerial);
				sLogin.dwResult = dRESULT_LOGIN_FAILED;
				strcpy(sLogin.strID,cLogin.strId);

				if	(strResult.compare("1")==0 || strResult.compare("3")==0)
				{	//	정상
					sLogin.dwResult = dRESULT_LOGIN_SUCCESS;

#ifdef	_FOR_JAPAN
					switch(sLogin.isOTPUser)
					{
					case 0:
						sLogin.dwResult	=	dRESULT_LOGIN_SUCCESS_BUT_NOT_USE_OTP;
						break;
					case 2:
						sLogin.dwResult =	dRESULT_LOGIN_FAIL_PERSONALCOM;
						break;
					case -1:
						sLogin.dwResult =	dRESULT_LOGIN_SUCCESS_PERSONALCOM;
						break;

					default	:
						if	(strResult.compare("3")==0)
							sLogin.dwResult =	dRESULT_LOGIN_OTP;
						break;
					}
#endif
					sLogin.dwCode	=	CertManager.add(cLogin.strId,cLogin.strPassword);

					if	(sLogin.dwCode==0xffffffff)
					{
						USERDB.Logout(cLogin.strId,cLogin.strLoginIp);
						sLogin.dwResult = dRESULT_LOGIN_FAILED;
					}
					else
					{
						for(int i=0;i<dUSERDB_MAX_USER_COUNT;i++)
						{
							CClient * cli = UserManager.GetClientPnt(i,FALSE);

							if	(cli==NULL)
								continue;

							if	(stricmp(cli->GetID(),dLOGIN_SERVER_ID)!=0 && stricmp(cli->GetID(),dADMIN_TOOL_ID)!=0 && cli->GetServerIndex()!=0xffff)
							{
								US_DIS_USER	dis;
								dis.base.set(sizeof(US_DIS_USER),dUS_DIS_USER,-1);
								strcpy(dis.strId,cLogin.strId);
								dis.wWhy = 1;		//	접속중이니까 잘라라.. 라라라.. 
								cli->AddSendPacket((char *)&dis,dis.base.wSize);
								SendPacket(cli->GetSerial());
//								_logc("전 서버에 해당 아이디를 가진 사람을 자르자. %s->%s\n",dis.strId,cli->GetIP());
							}
						}
					}

					printf("dwCode = %d\n",sLogin.dwCode);
				}
				else 
				if	(strResult.compare("-5")==0 || strResult.compare("-1")==0 || strResult.compare(0,2,"-2")==0 || strResult.compare("-4")==0 || strResult.compare("-3")==0)
				{	//	찾을수  없다.  or ADO 에러 
					_logf(" Login Error : [%s] ",strResult.c_str());
					sLogin.dwResult = dRESULT_LOGIN_FAILED;
				}
				else
				if	(strResult.compare(0,1,"2") == 0)
				{
					//	나쁜 유저 why에서 이유를.. 
					sLogin.dwResult = atoi(strResult.c_str());
				}
				else
				{
					//	strResult 에 현재 접속중인 월드의 이름이 넘어 온다.
					if	(cLogin.disconnectExistID)
					{
//#ifdef _FOR_KOREA
//						_log("-----------기존에  접속중인 유저를 잘라버립니다-----------\n");
//						_log("1. 접속중인 서버를 찾아봅니다. [%s] \n",strResult);
//#endif
						CClient	* usr = UserManager.GetClientPnt((char*)strResult.c_str());	//	const지만 변경할 일 없다.

						strcpy(sLogin.strWhy,strResult.c_str());

						if	(usr==NULL)
						{
							for(int i=0;i<dUSERDB_MAX_USER_COUNT;i++)
							{
								CClient * cli = UserManager.GetClientPnt(i,FALSE);

								if	(cli==NULL)
									continue;

  								if	(stricmp(cli->GetID(),dLOGIN_SERVER_ID)!=0 && stricmp(cli->GetID(),dADMIN_TOOL_ID)!=0 && cli->GetServerIndex()!=0xffff)
  								{
  									US_DIS_USER	dis;
  									dis.base.set(sizeof(US_DIS_USER),dUS_DIS_USER,-1);
  									strcpy(dis.strId,cLogin.strId);
  									dis.wWhy = 1;		//	접속중이니까 잘라라.. 라라라.. 
  									cli->AddSendPacket((char *)&dis,dis.base.wSize);
  									SendPacket(cli->GetSerial());
  									sLogin.dwResult = dRESULT_LOGIN_RETRY;
  									USERDB.Logout(cLogin.strId,cLogin.strLoginIp);
  								}
							}
//#ifdef _FOR_KOREA
//							_log("헉.. 접속중인 서버이름이 이상하군요 ㅠ_ㅜ\n");
//#endif
							sLogin.dwResult = dRESULT_LOGIN_USEDID;
						}
						else
						{
//#ifdef _FOR_KOREA
//							_log("음.. [%s]에 접속중이군요. \n",usr->GetID());
//							_log("2. [%s]에게 [%s]를 자르라고 시킵니다.  \n",usr->GetID(),cLogin.strId);
//#endif
							US_DIS_USER	dis;
							dis.base.set(sizeof(US_DIS_USER),dUS_DIS_USER,-1);
							strcpy(dis.strId,cLogin.strId);
							dis.wWhy = 1;		//	접속중이니까 잘라라.. 라라라.. 
							usr->AddSendPacket((char *)&dis,dis.base.wSize);
							SendPacket(usr->GetSerial());
							sLogin.dwResult = dRESULT_LOGIN_RETRY;
							USERDB.Logout(cLogin.strId,cLogin.strLoginIp);
						}
					}
					else
					{
						sLogin.dwResult = dRESULT_LOGIN_USEDID;
					}
				}

				user->AddSendPacket((char *)&sLogin,sLogin.base.wSize);

				if	(sLogin.dwResult)
					_log("login strResult = %d \n",sLogin.dwResult);
				break;
			}
		case dLU_WLOGIN:
			{
				LU_WLOGIN	cLogin;
				memset(&cLogin,0,sizeof(LU_WLOGIN));
				memcpy(&cLogin,recvpacket,recvsize);
				
				BOOL	isRegistOTP = FALSE;
				std::string result;
				char why[64];
				UL_RESULT_WLOGIN	sLogin;
				memset(&sLogin,0,sizeof(UL_RESULT_WLOGIN));
				sLogin.base.set(sizeof(UL_RESULT_WLOGIN),dUL_RESULT_WLOGIN,cLogin.base.wSerial);
				sLogin.serial = cLogin.serial;
				sLogin.dwResult = dRESULT_LOGIN_FAILED;
				strcpy(sLogin.strId,cLogin.strId);
				strcpy(sLogin.strName,cLogin.strName);
				strcpy(sLogin.strIP,cLogin.strIP);
				sLogin.isOper = 0;
				sLogin.isNetCafeUser = 0;

				_CERT_DATA	_data;

				CertManager.get(&_data,cLogin.strId,cLogin.dwCode);
				if(_data.dwCode!=0xffffffff)
				{
					result = USERDB.WLogin(_data.strId,_data.strPw,cLogin.strMacAddress, cLogin.strClientCode, user->GetID(),cLogin.strIP, why, sLogin.isOper, sLogin.isNetCafeUser, isRegistOTP);
				}else{
					result = "-1";
				}

				if	(result.compare("1")==0 || result.compare("3")==0)
				{	//	정상
					sLogin.dwResult = dRESULT_LOGIN_SUCCESS;
					//	전 서버에 해당 아이디를 가진 사람을 자르자.
#ifdef _FOR_KOREA
					_logc("2전 서버에 해당 아이디를 가진 사람을 자르자. %s\n",sLogin.strId);
#endif
					for(int i=0;i<dUSERDB_MAX_USER_COUNT;i++)
					{
						CClient * cli = UserManager.GetClientPnt(i,FALSE);
						if	(cli==NULL)
							continue;
						if	(cli->GetSerial() == user->GetSerial())
							continue;

						if	(stricmp(cli->GetID(),dLOGIN_SERVER_ID)!=0 && stricmp(cli->GetID(),dADMIN_TOOL_ID)!=0 && cli->GetServerIndex()!=0xffff)
						{
							US_DIS_USER	dis;
							dis.base.set(sizeof(US_DIS_USER),dUS_DIS_USER,-1);
							strcpy(dis.strId,sLogin.strId);
							dis.wWhy = 1;		//	접속중이니까 잘라라.. 라라라.. 
							cli->AddSendPacket((char *)&dis,dis.base.wSize);
							SendPacket(cli->GetSerial());
#ifdef _FOR_KOREA
							_logc("2전 서버에 해당 아이디를 가진 사람을 자르자. %s->%s\n",dis.strId,cli->GetIP());
#endif
							USERDB.Logout(sLogin.strId,sLogin.strIP);
						}
					}
				}
				else
				if(result.compare("-5")==0 || result.compare("-1")==0 || result.compare(0,2,"-2")==0 || result.compare("-4")==0)
				{	//	찾을수  없다.  or ADO 에러 
					sLogin.dwResult = dRESULT_LOGIN_FAILED;
				}
				else
				if	(result.compare(0,1,"0") == 0)
				{
					//	나쁜 유저 why에서 이유를.. 
					sLogin.dwResult = atoi(result.c_str());
				}
				else
				{
					if( result.compare(1, result.size(),dLOGIN_SERVER_ID) == 0) 
						sLogin.dwResult = dRESULT_LOGIN_SUCCESS;
					else
						sLogin.dwResult = dRESULT_LOGIN_USEDID;
				}
				user->AddSendPacket((char *)&sLogin,sLogin.base.wSize);
//				_log("[%s][%s]를 돌려줍니다. \n",sLogin.strId,sLogin.strName);
				break;
			}
		case dLU_LOGIN_OTP:
			{
				LU_LOGIN_OTP * rPacket = (LU_LOGIN_OTP * )recvpacket;
				
				UL_RESULT_LOGIN_OTP sPacket;
				sPacket.base.set(sizeof(UL_RESULT_LOGIN_OTP),dUL_RESULT_LOGIN_OTP,rPacket->base.wSerial);
				strcpy(sPacket.strID, rPacket->strId);
				sPacket.dwResult = USERDB.CheckOTP(rPacket->strId, rPacket->strOTP, rPacket->strIp);

				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				SendPacket(user->GetSerial());
			}
			break;
		case dLU_LOGOUT:
			{
				LU_LOGOUT	cLogout;
				memset(&cLogout,0,sizeof(LU_LOGOUT));
				memcpy(&cLogout,recvpacket,recvsize);
				USERDB.Logout(cLogout.strId,cLogout.strIp);
				
/*				US_DIS_USER	dis;
				memset(&dis,0,sizeof(US_DIS_USER));
				dis.base.set(sizeof(US_DIS_USER),dUS_DIS_USER,-1);
				strcpy(dis.strId,cLogout.strId);
				dis.wWhy = 1;		//	확인을 위해 , 중복 로그인을 막기 위해 
				user->AddSendPacket((char *)&dis,dis.base.wSize);
				SendPacket(user->GetSerial());
*/
//				_log("Logout [%s] [%d-%d] \n",cLogout.strId,cSRVUTIL::GetTime(),cSRVUTIL::GetTimes());
				break;
			}

			case dSU_LOGIN:
			{
				SU_LOGIN	sLogin;
				memset(&sLogin,0,sizeof(SU_LOGIN));
				memcpy(&sLogin,recvpacket,recvsize);

				user->SetID(sLogin.id);
				user->SetIP(sLogin.ip);
				user->SetServerIndex(sLogin.wServerIndex);
				user->SetServerListIndex(sLogin.wServerListIndex);

				user->SetType(sLogin.wType);
				user->SetUserCount(sLogin.wUserCount);
				user->SetMessage(sLogin.wMessage);
				user->SetInnerOpen(sLogin.m_bf1IsInnerOpen);

				_log("Connect Server : [%s] [%s] %d %d idx %d\n",sLogin.id,sLogin.ip,sLogin.wType,sLogin.wUserCount,sLogin.wServerIndex);

				if	(strcmp(user->GetID(),dLOGIN_SERVER_ID)==0)
				{
					UL_SERVER_LIST_ALL		sList;

					memset(&sList,0,sizeof(UL_SERVER_LIST_ALL));
					sList.wCount = 0;	//	로그인 서버 제외하고 

					for(int i=0;i<dUSERDB_MAX_USER_COUNT;i++)
					{
						CClient * cli = UserManager.GetClientPnt(i,FALSE);

						if	(cli	==	NULL)
							continue;

						if	(stricmp(cli->GetID(),dLOGIN_SERVER_ID)!=0 && stricmp(cli->GetID(),dADMIN_TOOL_ID)!=0 && cli->GetServerIndex()!=0xffff)
						{
							strcpy(sList.aServerList[sList.wCount].m_strName,cli->GetID());
							strcpy(sList.aServerList[sList.wCount].m_strIP,cli->GetIP());
							sList.aServerList[sList.wCount].m_wCount = cli->GetUserCount();
							sList.aServerList[sList.wCount].m_bf8ServerPlaceInList	= cli->GetServerListIndex();//	로그인 서버로는 리스트 인덱스를 보낸다.
							sList.aServerList[sList.wCount].m_bf8SettingServerIndex = cli->GetServerIndex();	//	고유 인덱스
							_log("%s,%d, idx %d,disp idx %d",
										sList.aServerList[sList.wCount].m_strName,	sList.wCount,
										sList.aServerList[sList.wCount].m_bf8SettingServerIndex,
										sList.aServerList[sList.wCount].m_bf8ServerPlaceInList);
							sList.aServerList[sList.wCount].m_wType		= cli->GetType();
							sList.aServerList[sList.wCount].m_wMessage	= cli->GetMessage();
							sList.aServerList[sList.wCount].m_bf1IsInnerOpen= cli->IsInnerOpen();

							_log("List ... [%s] [%s] \n",sList.aServerList[sList.wCount].m_strName,sList.aServerList[sList.wCount].m_strIP);
							++sList.wCount;
						}
					}
					sList.base.set(sizeof(cMSG_BASE_TYPE_FORUSERDB)+2+sizeof(cSERVER_SINFO)*sList.wCount,dUL_SERVER_LIST_ALL,0);
					user->AddSendPacket((char *)&sList,sList.base.wSize);
//					_log("Send 0x%x %d(%d) \n",sList.base.wType,user->GetNextSendBufPacketSize(),user->GetSendBufLength());
				}
				else
				{	//	#login서버에서 추가된 내용을 보내준다.
					UL_SERVER_ADD		sAdd;

					memset(&sAdd,0,sizeof(UL_SERVER_ADD));
					sAdd.base.set(sizeof(UL_SERVER_ADD),dUL_SERVER_ADD,0);
					strcpy(sAdd.ssInfo.m_strName,sLogin.id);
					strcpy(sAdd.ssInfo.m_strIP,sLogin.ip);

					sAdd.ssInfo.m_wCount				=	sLogin.wUserCount;
					sAdd.ssInfo.m_wType					=	sLogin.wType;
					sAdd.ssInfo.m_bf1IsInnerOpen		=	sLogin.m_bf1IsInnerOpen;
					sAdd.ssInfo.m_wMessage				=	sLogin.wMessage;
					sAdd.ssInfo.m_bf8ServerPlaceInList	=	sLogin.wServerListIndex;
					sAdd.ssInfo.m_bf8SettingServerIndex	=	sLogin.wServerIndex;

					_log(" Add Server : %s , %s, %d ",sAdd.ssInfo.m_strName,sAdd.ssInfo.m_strIP,user->GetSerial());
					SendToLogin((char *)&sAdd,sAdd.base.wSize);
				}
				break;
			}
		case dSU_INFO:
			{
				SU_INFO	sInfo;
				memset(&sInfo,0,sizeof(SU_INFO));
				memcpy(&sInfo,recvpacket,recvsize);

				user->SetType(sInfo.wType);
				user->SetUserCount(sInfo.wUserCount);
				user->SetMessage(sInfo.wMessage);
				user->SetInnerOpen(sInfo.m_bf1IsInnerOpen);

				break;
			}
		case dSU_NOT_DIS:
			{
				SU_NOT_DIS	dis;
				memset(&dis,0,sizeof(SU_NOT_DIS));
				memcpy(&dis,recvpacket,recvsize);

				USERDB.Logout(dis.strId,dis.strIp);
				break;
			}
//////////////////////////////////////////////////////////////////////////
//	추가된 로그 작업
		case dWU_LOG_INOUT:
			{
				WU_LOG_INOUT* rPacket = (WU_LOG_INOUT* )recvpacket;

				COMMANDDB.logInOut(rPacket);
			}
			break;
		case dWU_LOG_QUEST:
			{
				WU_LOG_QUEST* rPacket = (WU_LOG_QUEST* )recvpacket;

				COMMANDDB.logQuest(rPacket);
			}
			break;
//	추가된 로그 작업
//////////////////////////////////////////////////////////////////////////
		case dCM_DUMMY:
		default:
			{	//	알지 못하는 패킷은 버린다.
//				_log("이상한 나라의 패킷 : [%d] [%d]\n",user->GetSerial(),user->GetRecvBufLength());
				break;
			}
		}
		SendPacket(serial);
	}
}

void	SendNotice(int target, char * msg, int isUserMsg, char * who)
{
	_log("SendNotice : target[%d], msg[%s], isUserMsg[%d], who[%s]",target,msg,isUserMsg,who);
	if(!target)	//	전 서버에게
	{
		if(isUserMsg)	return;//	유저가 전 서버에게?? 일어나서는 안된다.
		UW_NOTICE		sPacket;
		sPacket.base.set(sizeof(UW_NOTICE),dUW_NOTICE,-1);
		sPacket.isUser = 0;
		strcpy(sPacket.msg,msg);
		memset(&sPacket.who,0,dNAME_LENGTH);

		for(int i =0;i<dUSERDB_MAX_USER_COUNT;i++)
		{	
			CClient * pu = UserManager.GetClientPnt(i);

			if	(!pu || pu->serial>=0xffff || stricmp(pu->GetID(),dLOGIN_SERVER_ID)==0 || stricmp(pu->GetID(),dADMIN_TOOL_ID)==0)
				continue;

			pu->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
		}
	}
	else
	{
		_log(" 특정 서버로 메세지 보내기 : target [%d]",target);
		UW_NOTICE		sPacket;
		sPacket.base.set(sizeof(UW_NOTICE),dUW_NOTICE,-1);
		sPacket.isUser = isUserMsg;
		strcpy(sPacket.msg,msg);
		if(isUserMsg)	strcpy(sPacket.who,who);
		for(int i =0;i<dUSERDB_MAX_USER_COUNT;i++)
		{	
			CClient * pu = UserManager.GetClientPnt(i);

			if	(!pu || pu->serial>=0xffff || stricmp(pu->GetID(),dLOGIN_SERVER_ID)==0 || stricmp(pu->GetID(),dADMIN_TOOL_ID)==0)
				continue;

			if	(pu->wServerIndex == target-1)
			{
				_log("Send Notice [%d == %d]",pu->wServerIndex,target-1);
				_log(" send msg to [%s],[%s]",pu->GetID(),pu->GetIP());
				pu->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				break;
			}
		}
	}
}

void
EventMngThread(void * lpParameter)
{
#ifndef _USE_CARROTSHOP
	return ;
#endif
	_log("Start EventMngThread ...");

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_EVENT_MGR);

	while(g_bThreadLoop)
	{	
		for (int iSleep=0;iSleep<600 && g_bThreadLoop;iSleep++)
		{
			Sleep(100);

			if	(!g_bThreadLoop)
				break;

			g_lifeCodeMgr.putLifeSign(eLIFE_CODE_EVENT_MGR);
		}

		if	(USERDB.GetCarrotShopInfo(&g_cCarrotShop))
		{	//	send to world servers
			_logc(" NEW GetCarrotShopInfo : count %d",g_cCarrotShop.m_wCategoryCount);
/*			for(int i =0;i<dUSERDB_MAX_USER_COUNT;i++)
			{	_logc(" count %d ",i);
				CClient * cpUser = UserManager.GetClientPnt(i);
				if(cpUser)
					SendCarrotShop(cpUser);
			}
*/
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_EVENT_MGR);

	_log("END EventMngThread ...");
}

void
CommandThread(void * lpParameter)
{
	if	(l_bIsTestDB)
		return;

	_log("Start CommandThread ...");

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_COMMAND);

	while(g_bThreadLoop)
	{
		Sleep(500);

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_COMMAND);

		{
			cAdCommandData		_command;

			if	(!COMMANDDB.GetCommand(&_command))
				continue;

			//	등록된 명령을 처리한다.
			switch(_command.commandtype)
			{
				case eCOMMANDTYPE_LOGINSERVER:	//	로그인 서버 닫기/열기
				{
#ifdef _FOR_KOREA
					_log("Send Command LOGIN SERVER ONOFF : [%d] [%s]",_command.param1,_command.msg);
#endif
					UL_ONOFF		sPacket;
					sPacket.base.set(sizeof(UL_ONOFF),dUL_ONOFF,-1);
					strcpy(sPacket.msg,_command.msg);
					sPacket.onoff = _command.param1;

					SendToLogin((char *)&sPacket,sPacket.base.wSize);
				}
				break;
				case eCOMMONDTYPE_CHANGE_VERSION://	버젼 수정하기
				{
#ifdef _FOR_KOREA
					_log("Send Command CHANGE VERSION : [%d]",_command.param1);
#endif
					UL_CHANGEVER	sPacket;
					sPacket.base.set(sizeof(UL_CHANGEVER),dUL_CHANGEVER,-1);
					sPacket.ver = _command.param1;
					SendToLogin((char *)&sPacket,sPacket.base.wSize);
				}
				break;
				case eCOMMANDTYPE_CHANGE_MAXUSER://	최대 수용 인원수를 변경한다.
				{
#ifdef _FOR_KOREA
					_log("Send Command CHANGE MAXUSER : [%d]",_command.param1);
#endif
					UL_CHANGEMAXUSER	sPacket;
					sPacket.base.set(sizeof(UL_CHANGEMAXUSER),dUL_CHANGEMAXUSER,-1);
					sPacket.maxuser = _command.param1;
					SendToLogin((char *)&sPacket,sPacket.base.wSize);
				}
				break;
				case eCOMMANDTYPE_NOTICE:		//	공지하기
				{
#ifdef _FOR_KOREA
					_log("Send Command NOTICE : [%d] [%s]",_command.commandtarget,_command.msg);
#endif
					SendNotice(_command.commandtarget,_command.msg);
				}
				break;

				case eCOMMANDTYPE_BANISH:		//	추방하기 , 1회성이다. 다시 못들어오게 하기 위해서는 다른 조취가 필요하다.
				{
#ifdef _FOR_KOREA
					_log("Send Command BANISH : [%d] [%s] [%s]",_command.commandtarget,_command.param3,_command.msg);
#endif
					UW_BANISH		sPacket;
					sPacket.base.set(sizeof(UW_BANISH),dUW_BANISH,-1);
					strcpy(sPacket.msg,_command.msg);
					strcpy(sPacket.who,_command.param3);

					for(int i =0;i<dUSERDB_MAX_USER_COUNT;i++)
					{	
						CClient * pu = UserManager.GetClientPnt(i);
						if	(!pu || pu->serial>=0xffff || stricmp(pu->GetID(),dLOGIN_SERVER_ID)==0 || stricmp(pu->GetID(),dADMIN_TOOL_ID)==0)
							continue;

						if	(pu->wServerIndex == _command.commandtarget-1)
						{
							pu->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
							break;
						}
					}
				}
				break;
				case eCOMMANDTYPE_BROADCAST_MSG:	//	유저들이 올리는 방송용 메세지
				{
#ifdef _FOR_KOREA
					_log("Send Command BROADCAST [%s],[%s] to [%s]",_command.who,_command.msg,_command.param3);
#endif
					SendNotice(_command.commandtarget,_command.msg,1,_command.param3);
				}
				break;
				case eCOMMONTYPE_CLOSEWORLD:		//	월드 서버를 닫을 시간을 정한다.
				{
					UW_WORLD_CLOSE	sPacket;
					sPacket.base.set(sizeof(UW_WORLD_CLOSE),dUW_WORLD_CLOSE,-1);
					sPacket.finTime.m_dwValue = _command.finishtime.m_dwValue;
					sPacket.relTime.m_dwValue = _command.releasetime.m_dwValue;
					strcpy(sPacket.msg,_command.msg);

					for(int i =0;i<dUSERDB_MAX_USER_COUNT;i++)
					{	
						CClient * pu = UserManager.GetClientPnt(i);

						if(!pu || stricmp(pu->GetID(),dLOGIN_SERVER_ID)==0 || stricmp(pu->GetID(),dADMIN_TOOL_ID)==0)
						{
							if(!pu)
								_log(" pu is NULL [%d]",i);
							if(pu && stricmp(pu->GetID(),dLOGIN_SERVER_ID)==0)
								_log(" this server is login server [%d]",i);
							if(pu && stricmp(pu->GetID(),dADMIN_TOOL_ID)==0)
								_log(" this server is admin tool [%d]",i);
							continue;
						}

						_log("Send Command CLOSE WORLD : [COMMAND TARGET %d] , [pu->wServerIndex %d]",_command.commandtarget, pu->wServerIndex);
						if(_command.commandtarget==0 || pu->wServerIndex == _command.commandtarget-1)
						{	
#ifdef _FOR_KOREA
							_log("Send Command CLOSE WORLD : [%s]",pu->GetID());
#endif
							pu->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
						}
					}
				}
				break;
				case eCOMMONTYPE_RENAME_AVATAR:
				{
					UW_RENAME_AVATAR	sPacket;
					sPacket.base.set(sizeof(UW_RENAME_AVATAR),dUW_RENAME_AVATAR,-1);
					strcpy(sPacket.strOldName,_command.param3);
					strcpy(sPacket.strNewName,_command.msg);
					
					for(int i =0;i<dUSERDB_MAX_USER_COUNT;i++)
					{	
						CClient * pu = UserManager.GetClientPnt(i);

						if	(!pu || pu->serial>0xffff || stricmp(pu->GetID(),dLOGIN_SERVER_ID)==0 || stricmp(pu->GetID(),dADMIN_TOOL_ID)==0)
							continue;

						if	(pu->wServerIndex == _command.commandtarget)
						{	
#ifdef _FOR_KOREA
							_log("Send Command RENAME AVATAR : [%s]",pu->GetID());
#endif
							pu->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
							break;
						}
					}
				}
				break;
			}
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_COMMAND);

	_log("END CommandThread ...");
	return;
}

void AdditionThread(void * lpParameter)
{
	BOOL	isSrvList = FALSE;
	BOOL	isDum = FALSE;
	int		count = 0;
	DWORD	dwSSCheckTime = timeGetTime();
	cHardwareInfo	hwInfo;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_ADDITIONAL);

	_log("Start AdditionThread ...");

	while(g_bThreadLoop)
	{	
		Sleep(1);

		int a = UserManager.Size();

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_ADDITIONAL);

		for(int z = 0;z<a;z++)
		{
			CClient * user = UserManager.GetClientPnt(z,FALSE);

			if	(!user || user->GetIsClose())
				continue;

			SendPacket(user->GetSerial());
			ProcessRecvPacket(user->GetSerial());
			a = UserManager.Size();
		}
		Sleep(1);

		g_pEventManager->update();	//	PVP 관련 정보 업데이트 체크

		for(int cCount = 0;cCount<dLOGIN_MAX_USER_COUNT;cCount++)
		{
			if(CertManager.User[cCount].dwCode==0xffffffff)	continue;
			if(CertManager.User[cCount].isWaste())
			{
				EnterCriticalSection(&CertManager.csDeque);
				CertManager.User[cCount].init();
				CertManager.m_wNext = cCount;
				CertManager.m_wCount--;
				LeaveCriticalSection(&CertManager.csDeque);
			}
		}

#ifdef dUSE_SERVERSTATUS_REPORT
/*		if	(l_bIsTestDB == FALSE && timeGetTime() - dwSSCheckTime >= 30000)
		{	//	30초에 한번씩 DB에 현재 정보를 갱신해준다.
			if	( g_SS.isReady() )
			{
				g_SS.update(cServerStatus::eERROR_TYPE_SERVERSTATUS,1,"No problem");
				dwSSCheckTime = timeGetTime();
			}

			int error = hwInfo.checkHW();
			char strText[512];
			switch(error)
			{
			case eHW_WARNING_OVER_CPU_USAGE:
				{
					sprintf(strText,"eHW_WARNING_OVER_CPU_USAGE %d%% ",hwInfo.m_cCPUInfo.m_iTotalUsage);
					g_SS.update( eHW_WARNING_OVER_CPU_USAGE , hwInfo.m_cCPUInfo.m_iTotalUsage , strText);
				}
				break;
			case eHW_WARNING_OVER_MEMORY_USAGE:
				{
					sprintf(strText,"eHW_WARNING_OVER_MEMORY_USAGE %d/%d ",hwInfo.m_cMemoryInfo.m_lFreeSize,hwInfo.m_cMemoryInfo.m_lFreeSize);
					g_SS.update( eHW_WARNING_OVER_MEMORY_USAGE , 0 , strText);
				}
				break;
			case eHW_WARNING_HAVENOT_FREESIZE:
				{
					sprintf(strText,"eHW_WARNING_HAVENOT_FREESIZE [%s] %u/%u  ",hwInfo.m_dqHDDList.at(hwInfo.m_iErrorHddIndex).m_strHddName , hwInfo.m_dqHDDList.at(hwInfo.m_iErrorHddIndex).m_iFreeMB, hwInfo.m_dqHDDList.at(hwInfo.m_iErrorHddIndex).m_iTotalMB);
					g_SS.update( eHW_WARNING_HAVENOT_FREESIZE , 0 , strText);
				}
				break;
			}
		}
*/
#endif
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_ADDITIONAL);

	_log("END AdditionThread ...");
	return;
}

void SendToLogin(char * _packet,int size)
{
	for(int i =0;i<dUSERDB_MAX_USER_COUNT;i++)
	{
		CClient * pu = UserManager.GetClientPnt(i);

		if	(!pu || pu->serial>=0xffff)
			continue;

		if	(stricmp(pu->GetID(),dLOGIN_SERVER_ID)==0)	//	모든 로그인 서버에게 보낸다.
		{
			pu->AddSendPacket(_packet,size);
		}
	}
}

//////////////////////////////////////////////
//	method for Command
//////////////////////////////////////////////
void
ConsoleCommandThread(void * lpParameter)
{
	int iReallyQuit = 0;
	
	cInputConsoleCommand	inputCommand;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_COMMAND);

	while(g_bThreadLoop)
	{
		Sleep(1);

		char	*lpstrCommand	=	inputCommand.getCommand(TRUE);

		if	(!lpstrCommand)
			continue;

		if	(STRICMP(lpstrCommand,"q")==0)
		{
			printf(" Quit? (Y/N) ...");

			iReallyQuit = 1;

			continue;
		}
		
		if	(iReallyQuit && STRICMP(lpstrCommand,"y")==0)
		{
			WriteServerStatusToRegistry(eST_USER_DBC,eSERVER_STATUS_QUIT);

			g_bThreadLoop	=	FALSE;

			break;
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_COMMAND);
}

//////////////////////////////////////////////
//	method for packet method
//////////////////////////////////////////////
void	DisUser(int serial)
{
	if	(serial<0)
		return;

	EnterCriticalSection(&csRemoveUser);

	CClient * user  = UserManager.GetClientPnt(serial);

	if	(!user)
	{	
		LeaveCriticalSection(&csRemoveUser);
		return;
	}

	if	(user->GetIsUsed())
	{
		user->SetIsUsed(FALSE);

		_log("(%s)%d is Exit!!\n",user->GetID(),serial);
	}

	//	Login 서버들에게 알려준다.
	if	(strcmp(user->GetID(),dLOGIN_SERVER_ID)!=0 && user->GetServerIndex()!=0xffff)
	{
		UL_SERVER_DELETE		sDelete;
		memset(&sDelete,0,sizeof(UL_SERVER_DELETE));
		sDelete.base.set(sizeof(UL_SERVER_DELETE),dUL_SERVER_DELETE,0);
		sDelete.idx = user->GetServerIndex();
		SendToLogin((char *)&sDelete,sDelete.base.wSize);
	}

	UserManager.Del(serial);
	printf("DELETE USER  =%d \n",UserManager.Size());
	user->CSReset();
	LeaveCriticalSection(&csRemoveUser);
}

//////////////////////////////
//	About Carrot Shop 
void SendCarrotShop(CClient * _cpUser,BOOL _bIsUpdateNow)
{
	UW_ASK_CARROTSHOPINFO_RESULT sPacket;
	sPacket.base.set(sizeof(UW_ASK_CARROTSHOPINFO_RESULT),dUW_ASK_CARROTSHOPINFO_RESULT,0xffff);
	memcpy(&sPacket.cShopInfo,&g_cCarrotShop,sizeof(cCarrotShopDefine));
	sPacket.wIsUpdateNow	=	_bIsUpdateNow;
#ifndef _USE_CARROTSHOP
	g_cCarrotShop.m_wCategoryCount = 0;
#endif
	_log(" Send CarrotShop Info : count %d (%d)",sPacket.cShopInfo.m_wCategoryCount ,g_cCarrotShop.m_wCategoryCount);

	_cpUser->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
}

//////////////////////////////
//	About Carrot Shop 
void SendPacket(CClient * _cpUser,void* _lpPacket)
{
	int	iSize	=	((cMSG_BASE_TYPE*)_lpPacket)->wSize;

	if	(_cpUser	==	NULL)
	{
		for(int i =0;i<dUSERDB_MAX_USER_COUNT;i++)
		{	
			CClient * lpWS = UserManager.GetClientPnt(i);

			if	(!lpWS || lpWS->serial>0xffff || stricmp(lpWS->GetID(),dLOGIN_SERVER_ID)==0 || stricmp(lpWS->GetID(),dADMIN_TOOL_ID)==0)
				continue;

			lpWS->AddSendPacket((char*)_lpPacket,iSize);
		}

		return;
	}
	
	_cpUser->AddSendPacket((char *)_lpPacket,iSize);
}

void	InitWeeklyPvPInfo()
{
	USERDB.initWeeklyPvPInfo();	//	주간 전적을 초기화 하고 주간 랭킹 먹이는것
}

void	InitDailyPvPInfo()
{
	USERDB.initDailyPvPRankingInfo();		//	일간 랭킹 정보 초기화
}

void	MakeDailyDeulRankingInfo()
{
	USERDB.updateDailyPvPRankingInfo();
}


void
SendWeeklyDuelRankingInfo(CClient * _cpUser)
{
	UW_DUEL_RANKING_INFO	sPacket;
	
	sPacket.base.set(sizeof(sPacket),dUW_DUEL_RANKING_INFO,-1);
	
	//	주간 랭킹 전송
	sPacket.wWeeklyRankerCount		=	0;
	sPacket.wTrialGameRankerCount	=	0;
	sPacket.wIsDailyRanking			=	FALSE;
		
	//	sPacket.wWeeklyRankerCount에 주간 전적의 주간 랭킹 랭커 수를 담자
	//	sPacket.aList에 랭킹 정보를 담자
	//	200개의 랭킹을 담을 수 있다.
	//	앞에 100개에 주간 랭킹정보를 담는다. TrialGame 랭킹 정보는 없다.
	sPacket.wWeeklyRankerCount		=	USERDB.GetPvPWeeklyRanking(sPacket.aList);
	sPacket.base.wSize				=	sizeof(sPacket)-sizeof(sPacket.aList)+(sPacket.wWeeklyRankerCount)*sizeof(cDuelRankingInfo);

	SendPacket(_cpUser,&sPacket);
}

void
SendDailyDuelRankingInfo(CClient * _cpUser)
{
	UW_DUEL_RANKING_INFO	sPacket;
	
	sPacket.base.set(sizeof(sPacket),dUW_DUEL_RANKING_INFO,-1);
	
	sPacket.wWeeklyRankerCount		=	0;
	sPacket.wTrialGameRankerCount	=	0;
	sPacket.wIsDailyRanking			=	TRUE;
		
	//	sPacket.wWeeklyRankerCount에 주간 전적의 일간 랭킹 랭커 수를 담자
	//	sPacket.wTrialGameRankerCount TrialGame 랭킹 랭커 수를 담자
	//	sPacket.aList에 일간 랭킹 정보를 담는다.
	//	200개의 랭킹을 담을 수 있다.
	//	여기에 앞에 100개에 일단 랭킹을 담고 뒤에 100개에 TrialGame 일간 랭킹을 담는다.
	sPacket.wWeeklyRankerCount		=	USERDB.GetPvPDailyRanking(sPacket.aList);
	sPacket.wTrialGameRankerCount	=	USERDB.GetPvPTrialGameRanking(sPacket.aList+sPacket.wWeeklyRankerCount);
		
	sPacket.base.wSize				=	sizeof(sPacket)-sizeof(sPacket.aList)+(sPacket.wWeeklyRankerCount+sPacket.wTrialGameRankerCount)*sizeof(cDuelRankingInfo);
	
	SendPacket(_cpUser,&sPacket);
}

