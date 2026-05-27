// Server.cpp : Defines the entry point for the application.
//
unsigned	int	l_dwSign	=	2010040701;

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <iostream>
#include <io.h>
#include <ctype.h> 

#include "cSOCKET.h"
#include "Client.h"
#include "ExScript.h"
#include "cSRVUTIL.h"
#include "cPACKET_LOGINSERVER.h"
#include "cPACKET_WORLDSERVER.h"
#include "cPACKET_USERDBSERVER.h"
#include "cPACKET_DBSERVER.h"
#include "Hardwareinfo.h"
#include "operate_packet.h"
#include "life_code.h"
#include "reg_common.h"
#include "inputConsoleCommand.h"
#include "playerJobCommon.h"
#include "cExceptionHandler.h"
#include "event.h"
#include "mess_sign.h"
#ifdef _FOR_USA
#include "../tinyxml2.6.1/tinyxml.h"
#include "../OGP_Lib/ogpapi.h"
#endif
#include "../buildVersion.h"	//	¿∞ ∞¿Ø æœ∞Eæ.
							//	æ¿∏∏E¿œ¿ª ª˝º∫.(∞¿Ø œ¡E∏ª∞!!)
							//	æø°
							//	#pragma	once
							//	#define	dBUILDER_NAME	"young"
							//	ø ∞∞¿ º±æEæE¿÷. ø° ¿∏ß¿∫ ¡∑Œ±∑°∏” ¿∏ß¿ª ¿˚¿ª∞
int			g_bIsMainTestServer	=	FALSE;	//	∫ªº≠π∂Eº≥¡§¿ ∂∞∞¿∫ º∑. ∞·¿≥™ ∏˚°¡E∞‘¿” ø¿˚¿Œ∞∏∏ Ω∫Æ.

//#define dUSE_SERVERSTATUS_REPORT

#ifdef _FOR_THAI
#undef dUSE_SERVERSTATUS_REPORT
#endif

#ifdef dUSE_SERVERSTATUS_REPORT
	#include "cServerStatus.h"
#endif

#define _PACKET_OPTION_CRYPT		//	æœ≠º≠ ∂¿ª ≥Ø∏∞.
#define _PACKET_OPTION_SEND_AT_ONE	//	‡ø ¿∏∏¶ πø° ∫∏≥Ω.
#define dDB_CONNECT_TIME			100000
#define dCONFIGFILENAME				"config-login.txt"
#define dNOTICEFILENAME				"notice/notice.txt"
#define MSG(a,b)					MessageBox(NULL,a,b,MB_OK);
#define	dMAX_BUFFER					4096

static DWORD acceptfail = 0;
static DWORD acceptingfail = 0;
static int socketcount=0;
static int version = 0;

CRITICAL_SECTION csSENDPacket;
CRITICAL_SECTION csRECVPacket;
CRITICAL_SECTION csRECORD;
CRITICAL_SECTION csRemoveUser;

BOOL	l_bIsIgnoreSS		=	FALSE;
int		g_bIsIgnoreSMS_Alarm=	FALSE;

static BOOL		isLocalOpen =	FALSE;
static BOOL		isMainLoop	=	TRUE;
static BOOL		isFirst		=	TRUE;
static BOOL		isReady		=	FALSE;
static BOOL		g_bThreadLoop = TRUE;
static BOOL		g_bUsingL4Switch = FALSE;//USINGL4SWITCH
static char		strIDDBIp[dID_LENGTH];
static DWORD	dwIDDBPort;
char			path[1024];
char			pathForDump[1024];
char			strNoticePath[2048];
//static DWORD	g_dwLogoutCount = 0;
//static DWORD	g_dwConnectedCount = 0;
//static DWORD	g_dwSendData = 0;

#define			dDUMMY_CLIENT_COUNT	5000

DummyClient		dcClient[dDUMMY_CLIENT_COUNT];
int				iDCCount		=	0;
int				maxuserCount	=	5000;
char			l_strServerIp[32];

//	≥ª∫Œ Ω∫Æ∏¶ ¿ß 
BOOL		l_bIsTestDB		=	FALSE;

SOCKET		lisSock;					//	listen º“œ

HANDLE		g_hCompletionPort;		//	IOCPø° ªøE?Æ
HANDLE		hWorkerThread[dMAX_SERVER_COUNT],hAdditionThread,hWorldThread[dWORLD_MAX_USER_COUNT],hUSERDBThread,hAcceptThread;

int			nTimer;
CWWScript	configScp,noticeScp;
cSOCKET		l_aWorldSocket[dWORLD_MAX_USER_COUNT];
cSOCKET		userdbsock;

CClientManager	UserManager;
cLoginInfoList	l_Ulist;

BOOL	InitSocket();
BOOL	InitCompletionPort();
void	WorkerThread(LPVOID lpParameter); 
void	AdditionThread(LPVOID lpParameter); 
void	AcceptLoop(LPVOID lpParameter);
void	WorldCtlThread(LPVOID lpParameter); 
void	USERDBCtlThread(LPVOID lpParameter); 

int		GetWorldSocketIndex(char *name);	//	Worldº≠π¿ ¿ŒΩ∫∏¶ ææ˚€.
BOOL	RecvPacket(int serial);
BOOL	SendPacket(int serial);
void	SendPacket_Add(int serial);

void	ProcessRecvPacket(int serial);
void	ProcessDBDisconnect();
BOOL	ProcessCommand(char command);

//	¨∂¿æÆ ¿Øøº∫ ∞ª∏¶ . ∏¡“≥¿ ∑ ¿Ø¿E!
BOOL	CheckGameUser(CClient *_lpUser,WORD _wSecurityCode,UINT _uiSecurityRet,BOOL _bIsDis,BOOL _bIsCheckSecurityRet);

//	method for packet send
void	DisUser(DWORD serial,char * _why,BOOL	isCut = FALSE);
//	end define

UL_SERVER_LIST_ALL		srvList;
SL_NOTICE_FOR_LAUNCHER	sLauncher;
static int	noticeSize = 0;
//////////////////////////////////////
//	±∏ ±…¿ª ¿ß √∑°± 
BOOL	isSend_UserCount	=	FALSE;
//////////////////////////////////////
//	for Profile
DebugTracer		* cLOG;

cSOCKET*
GetGVGServerSocket()
{
	//	0π¿ æ¡∞E 1~4±˚?E¡Æ ¿÷∏E. ¿Eº≥¡§ 4π¿∫ ∏√. π∫∏. ~~
	for(int i=0;i<dMAX_SERVER_COUNT;i++)
	{
		if	(l_aWorldSocket[i].sock == INVALID_SOCKET || strcmp(l_aWorldSocket[i].name,"") ==0)
			continue;

		if	(srvList.aServerList[i].m_wType	!=	eSERVER_TYPE_GVG)
			continue;

		return	&l_aWorldSocket[i];
	}

	return	NULL;
}

BOOL
Is2ByteChar(char _char)
{
#ifdef	_FOR_JAPAN
	if ((BYTE)_char	>=	0xa1 && (BYTE)_char <=0xdf)
		return	FALSE;
#endif

#ifdef	_FOR_THAI
	return	FALSE;
#endif

	if	(_char	<	0)
		return	TRUE;

	return	FALSE;
}

DWORD
DecodeBuffer(int _iSeedValue,BYTE *_lpBuffer,int _iBufferSize)
{
	srand(_iSeedValue);

	int		iRandValue	=	_iSeedValue%GetLoginServerEncodeCode1()+GetLoginServerEncodeCode2();
	DWORD	dwCheckSum	=	0;

	for (int i=0;i<_iBufferSize;i++)
	{
		_lpBuffer[i]	-=	random(iRandValue);
		dwCheckSum		+=	_lpBuffer[i];
	}

	return	dwCheckSum;
}

BOOL
CheckGameUser(CClient *_lpUser,WORD _wSecurityCode,UINT _uiSecurityRet,BOOL _bIsDis,BOOL _bIsCheckSecurityRet)
{
	BOOL	bIsOK	=	TRUE;

	if	(_lpUser->m_wClientType	!=	eCT_GAME	||	_lpUser->m_wSecurityCode	!=	_wSecurityCode)
		bIsOK	=	FALSE;

	if	(bIsOK && _bIsCheckSecurityRet)
	{
		DecodeBuffer(_wSecurityCode,(BYTE*)&_uiSecurityRet,sizeof(UINT));

		if	(_uiSecurityRet	!=	dLOGINSERVER_SECURITY_RET)
			bIsOK	=	FALSE;
	}

	if	(bIsOK == FALSE && _bIsDis)
		DisUser(_lpUser->GetSerial(),"incorrect user",TRUE);

	_lpUser->m_wSecurityCode	=	random(0x6fff)+1;	//	∂ ¿Øøº∫ ∞ª∏¶ ¿ß ∫π E

	return	bIsOK;
}

class	cLastWorkPacket
{
public:
	int		m_iClientPacket;
	int		m_iWorldPacket;
	int		m_iDbcPacket;
};

cLastWorkPacket	l_packetLog;

void
EmergencyWork(void *lpInfo)
{
	g_eh.addStaticLog("client packet 0x%x",l_packetLog.m_iClientPacket);
	g_eh.addStaticLog("world packet 0x%x",l_packetLog.m_iWorldPacket);
	g_eh.addStaticLog("db packet 0x%x",l_packetLog.m_iDbcPacket);
}

void
main()
{
	memset(&l_packetLog,0,sizeof(l_packetLog));

	g_eh.setLauncher(EmergencyWork);
	g_eh.initLog(1024*1024);	//	1∏∞°.. -o-

	CreateDirectory("data",NULL);

	g_eventTimeManager.load();

	{
		char	strTitle[512];

#ifdef	_FOR_KOREA
		sprintf(strTitle,"RS Login server for korea. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_JAPAN
		sprintf(strTitle,"RS Login server for japan. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_THAI
		sprintf(strTitle,"RS Login server for thai. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_USA
		sprintf(strTitle,"RS Login server for USA. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#else
		sprintf(strTitle,"RS Login server. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#endif

		SetConsoleTitle(strTitle);
		WriteServerInfoToRegistry(eST_LOGIN,"Login Server");
	}

	//	dump Data Check Code
	//	ø‰æ¿ª ∂ ¡÷ºÆ√≥∏Æ“ ∞!!
/*	{
		FILE * _dFP = fopen("c:/DumpData_473720749_size7760.dat","rb");
		if(!_dFP)
		{
			printf(" ¿œ¿ª ø≠¡E∏‘.\n");
			return;
		}
		int fl = filelength(fileno(_dFP));
		int cp = 0;
		char data[102400];
		while(1)
		{
			if(cp>=fl)	break;
			WORD size=0;
			WORD type=0;
			fread(&size,2,1,_dFP);
			fread(&type,2,1,_dFP);
			cp+=4;
			printf("packet info : size %d, type 0x%x\n",size,type);
			if(size)
			{
				fread(data,size-4,1,_dFP);
				cp+=size-4;
			}
		}

	}
*/
	srand( timeGetTime()%0xffffffff );

#ifdef dUSE_SERVERSTATUS_REPORT
// 	if(!g_SS.init("Login", "Server"))	//	∑Œ±¿Œ º≠π∞° ø∑Ø∞≥∏E
// 	{
// 		MessageBox(NULL,"ERROR IN SS","WARNING!!",MB_OK);
// 		return ;
// 	}
#endif

	GetCurrentDirectory(1024,path);
	sprintf(pathForDump,"%s/Dump",path);
	FILE * testsfp	=	fopen("isInnerTestServer","rb");
	if	(testsfp)
	{
		l_bIsTestDB	=	TRUE;

		MessageBox(NULL,"for inner test server","WARNING!!",MB_OK);

		fclose(testsfp);
		testsfp = NULL;
	}
	
	printf(" Reading config ... [%s]\n",dCONFIGFILENAME);
	//	read config file
	if	(!configScp.Read(dCONFIGFILENAME))
	{
		return ;
	}

	{
		CWWScript::unWW_DataType	stValue;

		configScp.GetData(CWWScript::eWWS_DT_STRING,&stValue,"IDDBIP");
		strcpy(strIDDBIp,stValue.dtString);
		configScp.GetData(CWWScript::eWWS_DT_UINT,&stValue,"IDDBPORT");
		dwIDDBPort = stValue.dtUInt;
		configScp.GetData(CWWScript::eWWS_DT_UINT,&stValue,"MAXUSER");
		maxuserCount = stValue.dtUInt;
		configScp.GetData(CWWScript::eWWS_DT_UINT,&stValue,"USINGL4SWITCH");
		g_bUsingL4Switch = stValue.dtUInt;
		if	(configScp.GetData(CWWScript::eWWS_DT_UINT,&stValue,"IGNORE_LIFESIGN"))
			g_bIsIgnoreSMS_Alarm	= stValue.dtUInt;
		printf(" g_bIsIgnoreSMS_Alarm = %d \n",g_bIsIgnoreSMS_Alarm);

		if	(configScp.GetData(CWWScript::eWWS_DT_UINT,&stValue,"IGNORE_SS"))
			l_bIsIgnoreSS	=	stValue.dtUInt;

		if	(configScp.GetData(CWWScript::eWWS_DT_UINT,&stValue,"MAIN_TEST"))
			g_bIsMainTestServer	=	stValue.dtUInt;

		if	(maxuserCount<=0)
			maxuserCount = 5000;

		if	(maxuserCount>5000)
			maxuserCount = 5000;

		printf("maxuser count = %d \n",maxuserCount);
	}

// #ifndef	_FOR_KOREA
// 	if	(strncmp(strIDDBIp,"211.189",7)==0)
// 		return ;
// #endif

// #ifdef	_OGP_SERVICE
// 	if	(g_bIsMainTestServer)
// 		_InitOGPLibrary(dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, "http://api.ogplanet.com");
// 	else
// 		_InitOGPLibrary(dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, "https://billing.ogplanet.com/api");
// #endif


	printf(" Reading Notice ... \n");
	//	read notice file
	sprintf(strNoticePath,"%s\\%s",path,dNOTICEFILENAME);

	if	(!noticeScp.Read(strNoticePath))
	{
		cMSG::Error("error!!","can not read [%s] file",strNoticePath);
		return ;
	}


	{
		CWWScript::unWW_DataType	stValue;

		noticeScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"VER");
		version = stValue.dtInt;
		memset(&sLauncher,0,sizeof(SL_NOTICE_FOR_LAUNCHER));
		noticeScp.GetData(CWWScript::eWWS_DT_USHORT,&stValue,"STATUS");
		sLauncher.wServerStatus = stValue.dtUShort;
		noticeScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"OPENFORLOCAL");
		isLocalOpen = stValue.dtInt;

		noticeScp.GetData(CWWScript::eWWS_DT_STRING,&stValue,"PATCH_ADDR");
		strcpy(sLauncher.strDownloadAddress , stValue.dtString);
		sLauncher.wLastestVersion = version;
		noticeScp.GetData(CWWScript::eWWS_DT_STRING,&stValue,"NOTICE");
		strcpy(sLauncher.strMessage,stValue.dtString);
		sLauncher.base.wType = dSL_NOTICE_FOR_LAUNCHER;
		sLauncher.base.wSize = sizeof(cMSG_BASE_TYPE)+sizeof(WORD)*2+dNOTICE_FOR_LAUNCHER_DOWNLOAD_ADDRESS_SIZE+strlen(sLauncher.strMessage);
		printf(" %d %d [%s] \n[%s] \n",version,sLauncher.wServerStatus,sLauncher.strDownloadAddress,sLauncher.strMessage);
	}

	cLOG	=	new DebugTracer(DTType_ALL,"RedStone_LoginServer");
	//	¿øE≥°≥™∏E≥™∏”¡E√±≠
	//	Ω∫√ “œ∞E..

	for(int s=0;s<dWORLD_MAX_USER_COUNT;s++)
	{
		l_aWorldSocket[s].kill();
		hWorkerThread[s] = NULL;
	}

	hAdditionThread = NULL;
	hUSERDBThread = NULL;

	int threadcount=0;

	for	(threadcount=0;threadcount<dMAX_SERVER_COUNT;threadcount++)
		hWorkerThread[threadcount] = NULL;

	for	(threadcount=0;threadcount<dWORLD_MAX_USER_COUNT;threadcount++)
		hWorldThread[threadcount] = NULL;

	memset(&srvList,0,sizeof(UL_SERVER_LIST_ALL));
	srvList.base.set(sizeof(cMSG_BASE_TYPE_FORUSERDB)+2,dUL_SERVER_LIST_ALL,0);
	srvList.wCount = 0;
/*	
	srvList.wCount = atoi(configScp.getData("STATICCOUNT"));
	char	tmpStaticName[32];
	for(int i = 0; i<srvList.wCount; i++)
	{
		sprintf(tmpStaticName,"STATIC%02d",i);
		strcpy(srvList.aServerList[i].m_strName,scp.getData(tmpStaticName));
		strcpy(srvList.aServerList[i].m_strIP,"");
		srvList.aServerList[i].m_wServerIndex = i;
		srvList.aServerList[i].m_wCount = 0;
		srvList.aServerList[i].m_wMessage = 0;
		srvList.aServerList[i].m_wType =eSERVER_TYPE_OFF;
	}
*/
	UserManager.SetBaseSize(sizeof(cMSG_BASE_TYPE));
	InitializeCriticalSection(&csSENDPacket);
	InitializeCriticalSection(&csRECVPacket);
	InitializeCriticalSection(&csRECORD);
	InitializeCriticalSection(&csRemoveUser);

	if	(!InitSocket())
	{
		_log("Error InitSocket");
		return;
	}

	//	USERDBø° πE¡º”.
	SU_LOGIN	suLogin;

	suLogin.base.set(sizeof(SU_LOGIN),dSU_LOGIN,0);
	strcpy(suLogin.id,"#login");
	strcpy(suLogin.ip,l_strServerIp);
	suLogin.wType = 0;
	suLogin.wUserCount = 0;
	suLogin.wServerIndex = 0xffff;
	userdbsock.SendPacket((char *)&suLogin,suLogin.base.wSize);

	_log("Start Login Server ... day : %d - time : %d \n",cSRVUTIL::GetTime(),cSRVUTIL::GetTimes() );

	isReady	=	TRUE;

	InitLifeCode(l_strServerIp);

	cInputConsoleCommand	consoleCommand;

	while(isMainLoop)
	{
		char	*lpstrCommand	=	consoleCommand.getCommand();

		if	(lpstrCommand && !ProcessCommand(lpstrCommand[0]))
		{
			//	º≠π¿ ±…¿ª ¡æ∑·œ∞E
			g_bThreadLoop	=	FALSE;
			g_lifeCodeLogServerMgr.end();
			//	¡æ∑·.
			break;
		}

		Sleep(1);
	}

//	shutdown(lisSock,1);
//	closesocket(lisSock); 
//	lisSock	=	NULL;

	for (int i=10;i;)
	{
		printf("wait for end all thread.. %d\n",i);

		Sleep(1000);

		--i;
	}

// #ifdef	_OGP_SERVICE
// 	_FreeOGPLibrary();
// #endif

	DeleteCriticalSection(&csSENDPacket);
	DeleteCriticalSection(&csRECVPacket);
	DeleteCriticalSection(&csRECORD);
	DeleteCriticalSection(&csRemoveUser);

	WriteServerStatusToRegistry(eST_LOGIN,eSERVER_STATUS_OFF);

	_log("Login Server is Exit!!");
	return;
}

BOOL InitSocket()
{
	SOCKADDR_IN serverAddress;
	WSADATA wsadata;

	if	(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
	{
		_log("Fail WSAStartup !!");
		return FALSE;
	}

	if	((lisSock = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{	
		_log("Fail listen !!");
		return FALSE;
	}

	char name[32];
	char addr[20];

	gethostname(name,sizeof(name));
	struct hostent* he = gethostbyname(name);
	struct in_addr iaddr;
	memset(&iaddr,0,sizeof(iaddr));

	for(int i =0; he->h_addr_list[i] !=0; i++)
	{
		if	(	he->h_addrtype!=AF_INET)
			continue;

		memcpy(&iaddr,he->h_addr_list[i],sizeof(iaddr));
		strcpy(addr,inet_ntoa(iaddr));

		if	(isLocalOpen)
		{
			if (strncmp(addr,"19",2)==0 || strncmp(addr,"17",2)==0)
			{
				break;
			}
		}
		else
		{
			if (strncmp(addr,"19",2)!=0 && strncmp(addr,"17",2)!=0)
			{
				break;
			}
		}
	}

	_log("Open addr  [%d] = %s,%d\n",i,addr,dLOGINSERVER_PORT);

	DWORD	dwAddr = inet_addr(addr);

	if	(dwAddr ==INADDR_NONE)
		return FALSE;

	memset(&serverAddress,0,sizeof(serverAddress));

	if	(g_bUsingL4Switch)
		serverAddress.sin_addr.s_addr = INADDR_ANY;
	else
		serverAddress.sin_addr.s_addr = dwAddr;

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(dLOGINSERVER_PORT);

	if	(bind(lisSock,(LPSOCKADDR)&serverAddress,sizeof(serverAddress)) == SOCKET_ERROR)
	{
		_log("Fail bind !!");
		return FALSE;
	}

	listen(lisSock,5);

	if	(!InitCompletionPort())
	{
		_log("Fail Init IOCP Port !!");
		return FALSE;
	}

	if	(!userdbsock.init(dwIDDBPort,strIDDBIp,"USER DBC",sizeof(cMSG_BASE_TYPE_FORUSERDB)))
	{	
		_log("Fail Connect db !!");
		return FALSE;
	}

	hAcceptThread = (HANDLE)_beginthread(AcceptLoop, 0, NULL); 
	if	(hAcceptThread==NULL)
	{
		_log("Fail Create AcceptLoop");	
		return FALSE;
	}

	hAdditionThread = (HANDLE)_beginthread(AdditionThread, 0, NULL); 
	if	(hAdditionThread==NULL)
	{
		_log("Fail Connect AdditionThread !!");
		return FALSE;
	}

	hUSERDBThread = (HANDLE)_beginthread(USERDBCtlThread,  0, NULL); 
	if	(hUSERDBThread==NULL)
	{
		_log("Fail Connect USERDBCtlThread !!");
		return FALSE;
	}

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
				
				break;
			}
		}
	}

	return TRUE;
}

BOOL
InitCompletionPort()
{
	SYSTEM_INFO sys_info;
	DWORD dwThreadNum;

	g_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,0);

	if	(g_hCompletionPort==NULL)
	{
		_log("Fail IOCP Port Create !!");
		return FALSE;
	}

	GetSystemInfo(&sys_info);
//	dwThreadNum = sys_info.dwNumberOfProcessors * 2 + 1;
	dwThreadNum = dMAX_WORKER_COUNT;

	for	(int i=0;i<(int)dwThreadNum;i++)
	{
		hWorkerThread[i] =(HANDLE)_beginthread(WorkerThread, 0, (void *)i);

		if	(hWorkerThread[i]==NULL)
		{
			_log("Fail Thread begin !!");
			return FALSE;
		}
	}

	return TRUE;
}

void AcceptLoop(LPVOID lpParameter)
{
	DWORD Flags = 0;
	SOCKET		clientsock;

	DWORD index=0;
	_log("Start AcceptLoop ...");
	DWORD checkTime = timeGetTime();
	DWORD checkCount = 0;
	DWORD allconnectCount = 0;
	
	g_lifeCodeMgr.beginCheck(eLIFE_CODE_ACCEPT_LOOP);

	while(g_bThreadLoop)
	{
		Sleep(1);

		if	(UserManager.Size()>maxuserCount)
			continue;

		if	(!isReady)
			continue;

		clientsock	=	0;

		g_lifeCodeMgr.endCheck(eLIFE_CODE_ACCEPT_LOOP);

		clientsock	=	accept(lisSock,NULL,NULL);

		if	(!g_bThreadLoop)
			break;
		
		if	(clientsock == INVALID_SOCKET)
		{
			acceptfail++;
			_log(" ERROR accept %d socket %d ERROR %d",acceptingfail,lisSock,GetLastError());

			continue;
		}

		int buf	=	dMAX_PACKET_LENGTH;

		if	(SOCKET_ERROR ==setsockopt(clientsock,SOL_SOCKET ,SO_SNDBUF,(const char*)&buf,sizeof(buf)))
		{
			shutdown(clientsock,1);
			closesocket(clientsock);
			clientsock = NULL;
			continue;
		}
		if	(SOCKET_ERROR ==setsockopt(clientsock,SOL_SOCKET ,SO_RCVBUF,(const char*)&buf,sizeof(buf)))
		{
			shutdown(clientsock,1);
			closesocket(clientsock);
			clientsock = NULL;
			continue;
		}

		LINGER	lingerStruct;
		
		lingerStruct.l_onoff = 1;
		lingerStruct.l_linger = 0; 

		if	(SOCKET_ERROR ==setsockopt(clientsock, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct)))
		{
			shutdown(clientsock,1);
			closesocket(clientsock);
			clientsock = NULL;
			continue;
		}

		g_lifeCodeMgr.beginCheck(eLIFE_CODE_ACCEPT_LOOP);

		char		_ip[16];

		SOCKADDR_IN	saddr;

		memset(&saddr,0,sizeof(saddr));
		int tmp = sizeof(sockaddr);

		getpeername(clientsock,(SOCKADDR *)&saddr,&tmp);
		strcpy(_ip,inet_ntoa(saddr.sin_addr));

//		if	(sLauncher.wServerStatus==2 && strncmp(_ip,"172.31.6",8)!=0  && strncmp(_ip,"58.150.56",8)!=0  )
//		{
//			shutdown(clientsock,1);
//			closesocket(clientsock);
//			clientsock = NULL;
//			continue;
//		}

		index = UserManager.Add();

		if	(index>(DWORD)maxuserCount)
		{
//					_log("Fail Add User!!!");	
			if	(clientsock)
			{
				shutdown(clientsock,1);
				closesocket(clientsock);
				clientsock = NULL;
			}

			continue;
		}

		CClient *newUser = NULL;
		newUser = UserManager.GetClientPnt(index);

		if	(!newUser)
		{
			if	(clientsock)
			{
				shutdown(clientsock,1);
				closesocket(clientsock);
				clientsock = NULL;
				UserManager.Del(index);
			}
			continue;
		}

		newUser->Open();
		newUser->SetSocket(clientsock);
		newUser->SetSerial(index);
		strcpy(newUser->ip,_ip);

		if	(!CreateIoCompletionPort((HANDLE)clientsock,g_hCompletionPort,(DWORD)index,0))
		{
			_log("Error CreateIoCompletionPort !! - %d, %d",GetLastError(),clientsock);
			shutdown(clientsock,1);
			closesocket(clientsock);
			clientsock = NULL;
			DisUser(index, " Error CreateIoCompletionPort");
			continue;
		}

		RecvPacket(index);
	}

	_log("END AcceptLoop ... ");

	hAcceptThread	=	NULL;

	g_lifeCodeMgr.endCheck(eLIFE_CODE_ACCEPT_LOOP);

	return ;
}

void
WorkerThread(LPVOID lpParameter)
{
	LP_IO_DATA	lpOV;
	DWORD dwRead,dwIOCPKey;
	int	threadSerial = (int)lpParameter;

	_log("Start WorkerThread[%d]...",threadSerial);
	BOOL retVal = TRUE;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_WORKER);

	while(1)
	{
		g_lifeCodeMgr.endCheck(eLIFE_CODE_WORKER);

		dwIOCPKey	=	0xffffffff;
		retVal		=	GetQueuedCompletionStatus(g_hCompletionPort,&dwRead,&dwIOCPKey,(LPOVERLAPPED *)&lpOV,dwIOCPKey);

		if	(g_bThreadLoop	==	FALSE)	//	¿ßø°∞° ∫ø±¿∂º≠..
			break;

		g_lifeCodeMgr.beginCheck(eLIFE_CODE_WORKER);

		if	(retVal == TRUE && dwIOCPKey < (DWORD)maxuserCount && dwRead != 0 && lpOV != 0 )
		{
			CClient	* user = UserManager.GetClientPnt(dwIOCPKey);

			if	(!user)
				continue;	

			if	(lpOV->IOState == 0)
			{
				if	(!user->AddRecvPacket(dwRead))
				{
					DisUser(user->serial,"∑¿Œ∞ ∞∞¿∫..",TRUE);
					continue;
				}

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
			if	(dwIOCPKey >= (DWORD)maxuserCount)
				continue;

			if	(!lpOV)
				_log("SERROR : getqueue... lpOV = NULL");

			if	(dwRead == 0)
			{
				CClient	* user = UserManager.GetClientPnt(dwIOCPKey);

				if(!user)
				{
					_log("SERROR : getqueue... user==NULL || !user->GetIsUsed() , dwIOCPKey = %d",dwIOCPKey);
					continue;	
				}

				DisUser(dwIOCPKey,"Read = 0",TRUE);
			}
			else
			{
				DWORD dwError = GetLastError();
				_log("SERROR GetQueue Error : %d ",dwError);

				if	(dwError != ERROR_IO_PENDING)
				{
					if	(dwError==ERROR_NETNAME_DELETED)
						_log("dwError ==ERROR_NETNAME_DELETED");

					//	ERROR_IO_PENDING , ERROR_SUCCESS ≥™ø¿ ;;
					if	(dwError!=ERROR_OPERATION_ABORTED && dwIOCPKey < (DWORD)maxuserCount)
					{
						_log("%d Worker thread error GetQueueCompletionStatus... error [%d]",threadSerial,dwError);
						DisUser(dwIOCPKey,"Error in WorkerThread",TRUE );
					}
				}
			}
		}
	}

	hWorkerThread[threadSerial]	=	NULL;

	g_lifeCodeMgr.endCheck(eLIFE_CODE_WORKER);

	_log("End WorkerThread[%d]...",threadSerial);
	return;
}

BOOL RecvPacket(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);

	if	(!user || user->GetIsClose())
		return FALSE;

	DWORD	lpRecv;
	DWORD	Flags = 0;
	user->rOL.IOState = 0;

	if	(WSARecv(user->GetSocket(), &user->rOL.buf ,1,&lpRecv,&Flags,&user->rOL.OL ,NULL) ==SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();

		if	(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			DisUser(serial," Error RecvPacket",TRUE);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL
SendPacket(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);

	if	(!user || user->GetIsClose())
		return FALSE;

	WORD iSendSize = (WORD)user->sBuffer->GetData(user->sendpacket,0);

	if	(!iSendSize)
	{
		return TRUE;
	}

	user->sOL.IOState = 1;
	DWORD	lpSend = 0;	
	WSABUF	buf;

	user->sendpacket_size = iSendSize;

	buf.len	=	user->sendpacket_size;
	buf.buf	=	user->sendpacket;

	if	(WSASend(user->GetSocket(), &buf,1,&lpSend,0,&user->sOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();

		if	(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			_log("SEND ERROR ... ERROR %d",dwError);
			user->sBuffer->Next();
			DisUser(serial,"Error SendPacket");
			return FALSE;
		}
	}

	return TRUE;
}

void
SendPacket_Add(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);
	if(!user || user->GetIsClose())	return;
	user->sOL.IOState = 1;
	DWORD	lpSend = 0;	WSABUF	buf;
	buf.len = user->sendpacket_size;
	buf.buf = user->sendpacket;

//	_log("Send_ADD Data Size : %d", buf.len);
	if	(WSASend(user->GetSocket(), &buf,1,&lpSend,0,&user->sOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();

		if	(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			_log("SEND_ADD ERROR ... ERROR %d",dwError);
			user->sBuffer->Next();
			DisUser(serial,"Error Send Packet_Add");
			return;
		}
	}
}

void
OperateClientTypePacket(CClient *user,CL_CLIENT_TYPE *rPacket)
{
	user->m_wClientType		=	rPacket->wType;		//	¨∂¿æÆ ∏¿‘(∑±√,ıπ,∞‘¿” ¿Ø¿E
	user->m_wSecurityCode	=	0;

	switch(rPacket->wType)
	{
		case eCT_LAUNCHER:
			user->AddSendPacket((char *)&sLauncher,sLauncher.base.wSize);
			break;

		case eCT_DUMMY_GAME:
		case eCT_GAME:
		{
			user->m_wClientType		=	rPacket->wType;		//	¨∂¿æÆ ∏¿‘(∑±√,ıπ,∞‘¿” ¿Ø¿E
			user->m_wSecurityCode	=	random(0x6fff)+1;	//	∂ ¿Øøº∫ ∞ª∏¶ ¿ß ∫π E

			SL_SERVER_LIST sPacket;

			sPacket.wCount = 0;
			sPacket.wSecurityCode	=	user->m_wSecurityCode;

			//	º≠πE∏ÆΩ∫Æ∏¶ π∑Œ ∫∏≥ª¡.
			if	( sLauncher.wServerStatus	==	0 /*|| strstr(user->ip,"172.31") || strstr(user->ip,"58.150.56")*/)
			{
				for(int i=0;i<dMAX_SERVER_COUNT;i++)
				{
					if	(strcmp(srvList.aServerList[i].m_strName,"")==0)
						continue;

					strcpy(sPacket.aServerList[sPacket.wCount].m_strName,srvList.aServerList[i].m_strName);
					sPacket.aServerList[sPacket.wCount].m_wServerPlaceInList=	srvList.aServerList[i].m_bf8ServerPlaceInList;
					sPacket.aServerList[sPacket.wCount].m_wType				=	srvList.aServerList[i].m_wType;
					sPacket.aServerList[sPacket.wCount].m_bf1IsInnerOpen	=	srvList.aServerList[i].m_bf1IsInnerOpen;
					sPacket.aServerList[sPacket.wCount].m_wMessage			=	srvList.aServerList[i].m_wMessage;
					++sPacket.wCount;
				}

				sPacket.base.set(sizeof(cMSG_BASE_TYPE)+2+sizeof(cSERVER_INFO)*sPacket.wCount,dSL_SERVER_LIST);
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
			}
			else
			{
				sPacket.base.set(sizeof(cMSG_BASE_TYPE)+2+sizeof(cSERVER_INFO)*sPacket.wCount,dSL_SERVER_LIST);
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
			}
			break;
		}
	}
}

void
OperateLoginClientPacket(CClient *user,CL_LOGIN *rPacket,int serial)
{
	if	(!CheckGameUser(user,rPacket->wSecurityCode1,0,TRUE,FALSE))
		return;

	//	æœ≠ EE
	{
		DecodeBuffer(rPacket->wSecurityCode1,(BYTE *)rPacket->strId,sizeof(rPacket->strId));
		DecodeBuffer(rPacket->wSecurityCode2,(BYTE *)rPacket->strPassword,sizeof(rPacket->strPassword));
	}

	if	(rPacket->strId[0]	==	'#' &&
		rPacket->strId[1]	==	'#' &&
		rPacket->strId[2]	==	'!' &&
		rPacket->strId[3]	==	'#' &&
		rPacket->strId[4]	==	NULL )
	{
		SL_RESULT_LOGIN		slogin;
		memset(&slogin,0,sizeof(SL_RESULT_LOGIN));
		slogin.base.set(sizeof(SL_RESULT_LOGIN),dSL_RESULT_LOGIN);
		slogin.dwResult			=	l_dwSign;
		slogin.wSecurityCode	=	0x1212;

		user->AddSendPacket((char *)&slogin,slogin.base.wSize);
		return;	
	}

	rPacket->strId[dID_LENGTH-1]		=	NULL;
	rPacket->strPassword[dPASS_LENGTH-1]=	NULL;
	rPacket->strMacAddress[63]			=	NULL;
	rPacket->strSelectServer[dNAME_LENGTH-1]	=	NULL;

	if	(rPacket->b1IsYahoo)
	{
		CTimeInfo	currentTime,generateTime;

		currentTime.update();

		char		strMonth[3],strDay[3],strHour[3];

		strncpy(strMonth,rPacket->strPassword,2);
		strncpy(strDay,rPacket->strPassword+2,2);
		strncpy(strHour,rPacket->strPassword+4,2);

		strMonth[2]	=	NULL;
		strDay[2]	=	NULL;
		strHour[2]	=	NULL;

		generateTime.m_wMonth	=	atoi(strMonth);
		generateTime.m_wDay		=	atoi(strDay);
		generateTime.m_wHour	=	atoi(strHour);

		DWORD	dwCurrentTime	=	currentTime.m_wMonth *24*31+currentTime.m_wDay *24+currentTime.m_wHour;
		DWORD	dwGenerateTime	=	generateTime.m_wMonth*24*31+generateTime.m_wDay*24+generateTime.m_wHour;
	}

	strcpy(user->tmpid,rPacket->strId);
	strcpy(user->id,rPacket->strId);

	user->dwHashIdCode = GetHashCode((BYTE *)user->id);
	l_Ulist.add( user->serial, user->id );

	if	(strcmp(rPacket->strSelectServer,"")==0)
		user->SetSelectServer("CloseBeta");
	else
	{
		user->SetSelectServer(rPacket->strSelectServer);

		for(int i = 0;i<dMAX_SERVER_COUNT ;i++)
		{
			if	(l_aWorldSocket[i].sock == INVALID_SOCKET || strcmp(l_aWorldSocket[i].name,"") ==0)
				continue;

			if	(strcmp(rPacket->strSelectServer,srvList.aServerList[i].m_strName)==0)
			{
				user->wSelectedServerIndex  = i;
				break;
			}
		}
	}

	if	(rPacket->wVersion<version)
	{
		SL_RESULT_LOGIN		slogin;
		memset(&slogin,0,sizeof(SL_RESULT_LOGIN));
		slogin.base.set(sizeof(SL_RESULT_LOGIN),dSL_RESULT_LOGIN);
		slogin.dwResult			=	dRESULT_LOGIN_NOT_SAME_VERSION;
		slogin.wSecurityCode	=	user->m_wSecurityCode;

		user->AddSendPacket((char *)&slogin,slogin.base.wSize);
	}
	else
	{
		int testDummy = 0;

		for(int dc = 0 ;dc<dDUMMY_CLIENT_COUNT ; dc++)
		{
			if	(strcmp(dcClient[dc].id,rPacket->strId)==0)
			{
				testDummy = 1;
				break;
			}
		}

		if	(testDummy)
		{
			SL_RESULT_LOGIN		slogin;
			memset(&slogin,0,sizeof(SL_RESULT_LOGIN));
			slogin.base.set(sizeof(SL_RESULT_LOGIN),dSL_RESULT_LOGIN);
			slogin.dwResult			=	dRESULT_LOGIN_USEDID;
			slogin.wSecurityCode	=	user->m_wSecurityCode;

			user->AddSendPacket((char *)&slogin,slogin.base.wSize);
		}
		else
		{
#ifdef _OGP_SERVICE
			char query[1024];memset(query, 0,1024);
			char ret[2048];memset(ret, 0,2048);
			//	https://billing.ogplanet.com/api/checkValidToken.og

			if	(g_bIsMainTestServer)
				sprintf(query,"http://api.ogplanet.com/checkValidToken.og?ServiceID=%d&ServiceKey=%s&Token=%s&UserID=%s&UserIP=%s&RequestType=3",	// RequestType 3-∞‘¿”º≠πE.
					dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, rPacket->strToken, rPacket->strId, user->ip
					);
			else
				sprintf(query,"http://billing.ogplanet.com/api/checkValidToken.og?ServiceID=%d&ServiceKey=%s&Token=%s&UserID=%s&UserIP=%s&RequestType=3",	// RequestType 3-∞‘¿”º≠πE.
					dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, rPacket->strToken, rPacket->strId, user->ip
					);
			
			_SendDirectHTTP(query, ret);
			_log(query);
			_log(ret);

			int retCode = 0;
			{
				TiXmlDocument doc;
				doc.Parse( ret );
				if ( doc.Error() )
				{
					_log( "UseCarrot_OGP Error in %s: %s\n", doc.Value(), doc.ErrorDesc() );
					retCode = 10001;
				}else
				{
					TiXmlNode* node = 0;
					TiXmlElement* ItemInfoEl = 0;
					
					node = doc.FirstChild( "XMLResponseData" );
					//	ø°∑Ø∞° πª˝≥™?
					ItemInfoEl = node->FirstChildElement("ActionInfo");
					TiXmlElement* valueEl = ItemInfoEl->FirstChildElement("ReturnCode");
					retCode = atoi((char *)valueEl->GetText());

					switch(retCode)
					{
					case 10001:	//	∑Œ±¿Œ¿ ¡Eæ¿Ω.
					case 10002: //	∫˙”∞ ªøE?
					case 10003: //	∫˙”∞ IP
					case 10004: //	¡º” ∫“∞° ¡ø™
					case 10005: //	¡∏¿œ¡Eæ ªøE?
					case 10006: //	¡∏¿œ¡Eæ º≠∫Ω∫
					case 10007: //	¿Øøœ¡Eæ¿∫ ‰´
					case 10008: //	¡∏¿œ¡Eæ ‰´¿‘œ.
					case 10009:	//	ServiceIDø ‰´¿ ¿œ°œ¡EæΩ¿œ.
					case 10010:	//	UserIDø ‰´¿ ¿œ°œ¡EæΩ¿œ.
					case 10011:	//	ServiceKey∞° ¡§ª¿˚¿¡EæΩ¿œ.
					case 10012:	//	¡± ±¿ª ∞°¡∞E¿÷¡EæΩ¿œ.
					case 10013:	//	æœ∞° ¿œ°œ¡EæΩ¿œ. ∑Œ±¿Œ Ω
					case 10014:	//	Xπ¿ªEæœ∏¶ ΩœøΩ¿œ. - ∞∞E
					case 10015:	//	Xπ¿ªEΩ∑Œ æ¿∞° ∫˙”∞æ˙Ω¿œ. 
					case 10016:	//	∞º∫≠¡Eæ¿∫ ªøE¿‘œ?
					case 10017:	//	180¿œ ¿ªE∑Œ±¿Œœ¡Eææ ± æ¿¿‘œ. 
						break ;
					case 10000:	//	º∫∞E
						break;
					}
				}
			}

			if(retCode != 10000)
			{
				SL_RESULT_LOGIN		slogin;
				memset(&slogin,0,sizeof(SL_RESULT_LOGIN));
				slogin.base.set(sizeof(SL_RESULT_LOGIN),dSL_RESULT_LOGIN);
				slogin.dwResult = dRESULT_LOGIN_FAILED;
				user->AddSendPacket((char *)&slogin,slogin.base.wSize);
			}else
#endif
			{
				LU_LOGIN			dologin;
				memset(&dologin,0,sizeof(LU_LOGIN));
				dologin.base.set(sizeof(LU_LOGIN),dLU_LOGIN,serial);
				memcpy(dologin.strId,rPacket->strId,dID_LENGTH);
				memcpy(dologin.strPassword,rPacket->strPassword,dPASS_LENGTH);
				memcpy(dologin.strLoginIp,user->ip,dPASS_LENGTH);
				memcpy(dologin.strMacAddress,rPacket->strMacAddress,64);
				memcpy(dologin.strClientCode,rPacket->strClientCode,12);
				dologin.strClientCode[11] = 0;
				dologin.strPassword[dPASS_LENGTH-1] = 0;

				if	(strcmp(dologin.strPassword,"")!=0)	
				{
					memset(dologin.strSelectServer,0,dNAME_LENGTH);
					strcpy(dologin.strSelectServer,"#login");
					dologin.disconnectExistID = rPacket->b1IsDisconnectExistID;
					userdbsock.SendPacket((char *)&dologin,dologin.base.wSize);
				}

				l_Ulist.setData(user->dwHashIdCode, eSEND_DATA , dLU_LOGIN );
				user->loginTime = timeGetTime();
			}
		}
	}
}

void
OperateCreateAvatarClientPacket(CClient *user,CL_CREATE_AVATAR *rPacket,int serial)
{
	int	i;

	if	(!CheckGameUser(user,rPacket->wSecurityCode,rPacket->uiSecurityCodeRet,TRUE,TRUE))
		return;

	BOOL	bIsIncorrectName	=	FALSE;

	if	(!CheckName(rPacket->strName))
		bIsIncorrectName		=	TRUE;

	if	(STRICMP(rPacket->strName,"EMPTY")==0 || STRICMP(rPacket->strName,"dummy!!")==0)
		bIsIncorrectName		=	TRUE;

#if defined (_FOR_INTER)
	for(int z=0;z<strlen(rPacket->strName);z++)
	{
		if( !isascii(rPacket->strName[z]))
			return;
	}

	if	(z < strlen(rPacket->strName))
	{
		bIsIncorrectName = TRUE;
	}
#endif

	int	iPoint		=	0;
	rPacket->strName[dNAME_LENGTH-2]	=	NULL;
	int	iNameLength	=	strlen(rPacket->strName);

	while(iPoint < dNAME_LENGTH-2 && rPacket->strName[iPoint])
	{
		char	ch	=	rPacket->strName[iPoint];

		if	(Is2ByteChar(ch))
		{
#ifdef	_FOR_ENG
			bIsIncorrectName		=	TRUE;
			break;
#endif
			iPoint	+=	2;
			continue;
		}

		if	(ch == '\\' || ch == '%' || ch == '\"' || ch	==	'\'' || ch == ' ')
		{
			bIsIncorrectName		=	TRUE;
			break;
		}

		iPoint++;
	}

#ifdef	_FOR_KOREA
	if	(strstr(rPacket->strName,"GM"))
		bIsIncorrectName		=	TRUE;
	if	(strstr(rPacket->strName,"gm"))
		bIsIncorrectName		=	TRUE;
	if	(strstr(rPacket->strName,"Gm"))
		bIsIncorrectName		=	TRUE;
	if	(strstr(rPacket->strName,"gM"))
		bIsIncorrectName		=	TRUE;
	if	(strstr(rPacket->strName," "))
		bIsIncorrectName		=	TRUE;

	if	(STRICMP(rPacket->strName,"EMPTY")==0	)
		bIsIncorrectName		=	TRUE;
	if	(STRICMP(rPacket->strName,"dummy!!")==0	)
		bIsIncorrectName		=	TRUE;

	if	(strstr(rPacket->strName,"°°") || strstr(rPacket->strName,"œ") || strstr(rPacket->strName,"Œ"))
		bIsIncorrectName		=	TRUE;

	for (i=0;i<iNameLength;i++)
	{
		if ((BYTE)rPacket->strName[i] >= 0x80	)
		{
			WORD	wValue1	=	(BYTE)rPacket->strName[i+1];
			WORD	wValue2	=	(BYTE)rPacket->strName[i];
			WORD	wValue	=	(wValue1)+(wValue2<<8);

			if	(wValue < 0xb0a1 || wValue > 0xcaa0)
			{
				bIsIncorrectName		=	TRUE;
				break;
			}
			i++;
		}
	}
#endif
	if	(bIsIncorrectName)
	{	//	ªøE“∞°…?πÆ¿∞° æ˚¨.
		SL_RESULT_CREATE_AVATAR		cCreateAva;
		memset(&cCreateAva,0,sizeof(SL_RESULT_CREATE_AVATAR));
		cCreateAva.base.set(sizeof(SL_RESULT_CREATE_AVATAR),dSL_RESULT_CREATE_AVATAR);
		cCreateAva.wResult = dRESULT_CREATE_FAILED_BY_INCORRECT_CHAR;
		cCreateAva.wSecurityCode	=	user->m_wSecurityCode;
		user->AddSendPacket((char *)&cCreateAva,cCreateAva.base.wSize);
		return;
	}

	SERVERStoWORLD_CREATEAVA	sPacket;
	memset(&sPacket,0,sizeof(SERVERStoWORLD_CREATEAVA));
	sPacket.base.set(sizeof(SERVERStoWORLD_CREATEAVA),dSERVERStoWORLD_CREATEAVA);
	sPacket.loginSerial = serial;
	strcpy(sPacket.strID,user->GetID());
	strcpy(sPacket.strName,rPacket->strName);
	strcpy(sPacket.strIP,user->ip);
//				printf(" Create Avatar : [%s] [%s] [%s] \n",sPacket.strID,sPacket.strName,sPacket.strIP);

	if	(rPacket->wJob	>=	dPLAYER_JOB_COUNT)
	{
		_log("ERROR job data : %d [%s]",rPacket->wJob,sPacket.strID);
		return;
	}

	sPacket.wJob = rPacket->wJob;

	i = GetWorldSocketIndex(user->GetSelectServer());

	if	(i==-1)
	{	//	º±√ ¿∏ß¿ º≠π ø¿¡∂¿Œ¿. 
#ifdef _FOR_KOREA
		_log("¿∏ º≠πEº±√¿. -0- [%s]",user->GetSelectServer());
#endif
	}
	else
	{
		if	(GetGVGServerSocket()	==	&l_aWorldSocket[i])	//	GVG º≠πE≥∏Ø ª˝º∫ ∫“∞°
			return;

		l_aWorldSocket[i].SendPacket((char *)&sPacket,sPacket.base.wSize);
	}
}

void
OperateRemoveAvatarClientPacket(CClient *user,CL_REMOVE_AVATAR *rPacket,int serial)
{
	if	(!CheckGameUser(user,rPacket->wSecurityCode,rPacket->uiSecurityCodeRet,TRUE,TRUE))
		return;

	SERVERStoWORLD_DELETEAVA	ava_s;
	memset(&ava_s,0,sizeof(SERVERStoWORLD_DELETEAVA));
	ava_s.base.set(sizeof(SERVERStoWORLD_DELETEAVA),dSERVERStoWORLD_DELETEAVA);
	ava_s.loginSerial = serial;
	strcpy(ava_s.strID,user->GetID());
	strcpy(ava_s.strNAME,rPacket->strName);
	strcpy(ava_s.strIP,user->ip);

#ifdef _FOR_KOREA
	_log("ª¡¶Ω√ª [%s] [%s] [%s]",ava_s.strID,ava_s.strNAME,ava_s.strIP);
#endif
	int  i = GetWorldSocketIndex(user->GetSelectServer());

	if	(i	!=	-1)
	{
		l_aWorldSocket[i].SendPacket((char *)&ava_s,ava_s.base.wSize);
	}
}

void
OperateSelectAvatarClientPacket(CClient *user,CL_REMOVE_AVATAR *rPacket,int serial)
{
	if	(!CheckGameUser(user,rPacket->wSecurityCode,rPacket->uiSecurityCodeRet,TRUE,TRUE))
		return;

	SERVERStoWORLD_LOADAVA	sPacket;

	memset(&sPacket,0,sizeof(SERVERStoWORLD_LOADAVA));

	sPacket.base.set(sizeof(SERVERStoWORLD_LOADAVA),dSERVERStoWORLD_LOADAVA);
	sPacket.loginSerial = serial;

	strcpy(sPacket.strID,user->GetID());
	strcpy(sPacket.strNAME,rPacket->strName);

	int  i	=	GetWorldSocketIndex(user->GetSelectServer());

	if	(i	!=	-1)
	{
//#ifdef _FOR_KOREA
		user->loginTime = timeGetTime();
//#endif
		l_aWorldSocket[i].SendPacket((char *)&sPacket,sPacket.base.wSize);
//		_logc("[%d] Avatar Selected...[%s] \n",cSRVUTIL::GetTimes(),sPacket.strNAME);

		l_Ulist.setData(user->dwHashIdCode , eSEND_DATA , dSERVERStoWORLD_LOADAVA);
	}
	else
	{
		_log("[%s] Data Sending is Fail to Game Server !!",sPacket.strNAME);
	}
}

cSOCKET*
GetWorldServerSocket(int _iWorldServerIndex)
{
	for(int i=0;i<dMAX_SERVER_COUNT;i++)
	{
		if	(l_aWorldSocket[i].sock == INVALID_SOCKET || strcmp(l_aWorldSocket[i].name,"") ==0)
			continue;
		if	(srvList.aServerList[i].m_bf8SettingServerIndex	==	_iWorldServerIndex)
			return	&l_aWorldSocket[i];
	}

	return	NULL;
}

void
OperateCopyDataClientPacket(CClient *user,CL_COPY_DATA *rPacket,int serial)
{
	if	(!CheckGameUser(user,rPacket->wSecurityCode,rPacket->uiSecurityCodeRet,TRUE,TRUE))
		return;

	//	0π¿ æ¡∞E 1~4±˚?E¡Æ ¿÷∏E. ¿Eº≥¡§ 4π¿∫ ∏√. π∫∏. ~~
	for(int i=0;i<dMAX_SERVER_COUNT;i++)
	{
		if	(l_aWorldSocket[i].sock == INVALID_SOCKET || strcmp(l_aWorldSocket[i].name,"") ==0)
			continue;
		if	(strcmp(srvList.aServerList[i].m_strName,"")==0)
			continue;

		int	iType	=	rPacket->wType;

		if	(iType	!=	eSERVER_TYPE_TEST	&&	iType	!=	eSERVER_TYPE_DUEL)
			continue;

		if	(iType	!=	srvList.aServerList[i].m_wType)
			continue;

#ifndef _FOR_KOREA
		if	(iType	==	eSERVER_TYPE_TEST)
			continue;
#endif
		printf(" [%s] [%d]",srvList.aServerList[i].m_strName,srvList.aServerList[i].m_wType);

		SERVERStoWORLD_COPY_DATA	cDATA;
		cDATA.base.set(sizeof(SERVERStoWORLD_COPY_DATA),dSERVERStoWORLD_COPY_DATA);

		int	iNameLen	=	strlen(rPacket->info.m_strName);

		if	(iNameLen	>	dNAME_LENGTH-2	||	iNameLen	<	4)
			break;

		cDATA.wType		=	iType;
		cDATA.wOrgWorld	=	rPacket->wWorld;
		strcpy(cDATA.strOrgName,rPacket->info.m_strName);

		strcpy(cDATA.strId,user->GetID());
		memcpy(&cDATA.info,&rPacket->info,sizeof(cAVATAR_INFO));

		if	(strcmp(user->list[ cDATA.info.m_wAvatarIndex ].name, cDATA.info.m_strName) !=0 ||
			user->list[ cDATA.info.m_wAvatarIndex ].job != cDATA.info.m_wJob ||
			user->list[ cDATA.info.m_wAvatarIndex ].lastfield != cDATA.info.m_wLastField ||
			user->list[ cDATA.info.m_wAvatarIndex ].lev != cDATA.info.m_wLevel
		  )
		{
			_log(" DATA COPY ERROR : [%s,%d,%d,%d] != [%s,%d,%d,%d]",user->list[ cDATA.info.m_wAvatarIndex ].name,user->list[ cDATA.info.m_wAvatarIndex ].job,user->list[ cDATA.info.m_wAvatarIndex ].lastfield,user->list[ cDATA.info.m_wAvatarIndex ].lev,cDATA.info.m_strName,cDATA.info.m_wJob,cDATA.info.m_wLastField,cDATA.info.m_wLevel);
			break;
		}

		{	//	¿∏ßø° º≠π∫∞∑Œ π∏¶ ∫¿Œ.
			char	strSIndex[4];
			sprintf(strSIndex,"_%.2d",user->wSelectedServerIndex+1);
			int len = strlen(cDATA.info.m_strName);
			int check = 0;

			if	(len	<=	dNAME_LENGTH-6)
				strcpy(cDATA.info.m_strName+len,strSIndex);
			else
			{
				for(int k = 0;k<len;k++)
				{
					if	(cDATA.info.m_strName[k]<0)
					{
						check++;
						if	(check>=2)
							check = 0;
					}

					if	(check==0 && k>=11)
						break;
				}

				strcpy(cDATA.info.m_strName+k+1,strSIndex);
			}
		}

		l_aWorldSocket[i].SendPacket((char *)&cDATA,cDATA.base.wSize);
		break;
	}
}

void
ProcessRecvPacket(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);

	if	(!user || user->GetIsClose())
		return;

	WORD	type = 0,recvsize = 0,wProtectCode=0;

	char	recvpacket[dMAX_PACKET_LENGTH];
	int		resultGetRecvPacket=0;

	while(TRUE)
	{
		memset(recvpacket,0,dMAX_PACKET_LENGTH);
		resultGetRecvPacket = user->GetRecvPacket(recvpacket);

		if	(resultGetRecvPacket==0)
			break;

		if	(resultGetRecvPacket==-1)
		{	
			_log("¿∏ ¿∏∑Œ ¿Œ ¿∏. %d ",serial);
			DisUser(serial,"incorrect data");
			break;	
		}

		recvsize	=	*(WORD *)recvpacket;
		type		=	*(WORD *)(recvpacket+2);
		wProtectCode=	*(WORD *)(recvpacket+4);

		if	(type == dCM_COMPRESS)
		{
			CM_COMPRESS	comp;
			memcpy(&comp,recvpacket,recvsize);
			cSRVUTIL::Uncompress((unsigned char *)comp.data,(unsigned char *)recvpacket,comp.wSize - sizeof(WORD)*3,comp.wSourceSize);

			recvsize	=	*(WORD *)recvpacket;
			type		=	*(WORD *)(recvpacket+2);
			wProtectCode=	*(WORD *)(recvpacket+4);
		}

		switch(type)	//	æ‡Ω ∂ æœ≠ √≥∏Æ
		{
			case dCL_LOGIN:
			case dCL_CREATE_AVATAR:
			case dCL_REMOVE_AVATAR:
			case dCL_SELECT_AVATAR:
			case dCL_LOGIN_OTP:
			case dCL_COPY_DATA:
			{
				DecryptData((void*)(recvpacket+6),recvsize-6,type);		//	∂æœ≠

// 
// 				DWORD	dwCrc32;
// 
// 				cCRC32::BufferToCrc32(recvpacket+6,recvsize-6,dwCrc32);
// 
// 				WORD	wCheckCode				=	dwCrc32&0xfff;
// 				WORD	wCheckCodeHead			=	(wCheckCode&0xfc0)<<6;
// 				WORD	wCheckCodeTail			=	wCheckCode&0x3f;
// 
//  				WORD	wReceiveCheckCodeHead	=	wProtectCode&0xfc00;
//  				WORD	wReceiveCheckCodeTail	=	wProtectCode&0x3f;
//  				WORD	wReceiveCheckCodeBody	=	wProtectCode&0x3c0;
// 
// 				if	(wCheckCodeHead	!=	wReceiveCheckCodeHead	||	wCheckCodeTail	!=	wReceiveCheckCodeTail)
// 				{
// 					printf("packet crc check failed!! [%s]\n",user->GetID());
// 
// 					_logf("packet crc check failed!![%s]",user->GetID());
// 
// 					DisUser(serial,"packet crc check failed!!");
// 					type	=	0xffff;
// 
// 					break;
// 				}
// 
// 				if	(user->m_wCheckBit	==	wReceiveCheckCodeBody)
// 				{
// 					printf("packet serial check failed!! [%s]\n",user->GetID());
// 
// 					_logf("packet serial check failed!![%s]",user->GetID());
// 
// 					DisUser(serial,"packet serial check failed!!");
// 
// 					type	=	0xffff;
// 
// 					break;
// 				}
// 				user->m_wCheckBit	=	wReceiveCheckCodeBody;
				break;
			}
		}

		switch(type)
		{
			case dCL_CLIENT_TYPE:
			{
				OperateClientTypePacket(user,(CL_CLIENT_TYPE *)recvpacket);
				break;
			}

			case dCL_LOGIN:
			{
				if	(!user->m_bOtp)
				{
					OperateLoginClientPacket(user,(CL_LOGIN *)recvpacket,serial);
				}else
				{
					DisUser(user->GetSerial(), "This is an OTP USER but this sent the login packet.",TRUE);
				}
				break;
			}

			case dCL_LOGIN_OTP:
			{
				CL_LOGIN_OTP	* rPacket = (CL_LOGIN_OTP	*)recvpacket;

				LU_LOGIN_OTP	sPacket;
				sPacket.base.set( sizeof(LU_LOGIN_OTP), dLU_LOGIN_OTP,serial);
				memcpy(sPacket.strId, user->GetID(),dID_LENGTH);
				memcpy(sPacket.strOTP, rPacket->strOTPCode,12);
				memcpy(sPacket.strIp, user->GetIP(),16);
				userdbsock.SendPacket((char *)&sPacket, sPacket.base.wSize);
			}
			break;

			case dCL_CREATE_AVATAR:
			{
				OperateCreateAvatarClientPacket(user,(CL_CREATE_AVATAR *)recvpacket,serial);

				break;
			}

			case dCL_REMOVE_AVATAR:
			{
				OperateRemoveAvatarClientPacket(user,(CL_REMOVE_AVATAR *)recvpacket,serial);
				break;
			}

			case dCL_SELECT_AVATAR:
			{
				OperateSelectAvatarClientPacket(user,(CL_REMOVE_AVATAR *)recvpacket,serial);
				break;
			}

			case dCL_COPY_DATA:
			{	//	º∑¿ª √æº≠ ¿∏ ∫πªœ∂∞E≥∞¡.
				OperateCopyDataClientPacket(user,(CL_COPY_DATA *)recvpacket,serial);
				break;
			}

			case dCM_DUMMY:
				break;

			default:
			{
				DisUser(user->GetSerial(),"unknwon packet",TRUE);
				//	æ¡E∏œ ∂¿∫ π∏∞.
				break;
			}
		}
		SendPacket(serial);
	}
}

void
AdditionThread(LPVOID lpParameter)
{
	BOOL	isSrvInfo = FALSE;
	int		noticeCount				=	0;
	DWORD	checkTime				=	timeGetTime();
	DWORD	dwSSCheckTime			=	timeGetTime();
	DWORD	dwLastSendDummPacketTime=	0;
	cHardwareInfo	hwInfo;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_ADDITIONAL);

	_log("START AdditionThread ...");
	while(g_bThreadLoop)
	{
		Sleep(1);

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_ADDITIONAL);

		for(int z = 0;z<maxuserCount;z++)
		{
			CClient * user = UserManager.GetClientPnt(z);

			if	(!user || user->GetSerial()==0xffffffff)
				continue;

			if	(user->isWaste())
			{	
				DisUser(user->GetSerial(),"it's waste",TRUE);	
				continue;	
			}

			SendPacket(user->GetSerial());
		}

		if	(userdbsock.sock == 0)
		{
			if	(!userdbsock.init(dwIDDBPort,strIDDBIp,"USER DBC",sizeof(cMSG_BASE_TYPE_FORUSERDB)))
			{
				userdbsock.isLoopRecv = FALSE;
				userdbsock.kill();
				_log("RECONNECT : Fail Reconnect User db !!\n");
			}
			else
			{
				hUSERDBThread = (HANDLE)_beginthread(USERDBCtlThread, 0, NULL); 

				if	(hUSERDBThread==NULL)
				{
					_log("RECONNECT : Fail Create USERDBCtlThread");
				}
				else
				{
					_log("RECONNECT : [%s] [%d]",strIDDBIp,dwIDDBPort);

					SU_LOGIN	sLogin;

					sLogin.base.set(sizeof(SU_LOGIN),dSU_LOGIN,0);
					strcpy(sLogin.id,"#login");
					strcpy(sLogin.ip,l_strServerIp);

					sLogin.wType		= 0;
					sLogin.wUserCount	= 0;
					sLogin.wMessage		= 0;
					sLogin.wServerIndex = 0xffff;

					userdbsock.SendPacket((char *)&sLogin,sLogin.base.wSize);
				}
			}
		}

		for(int dc = 0 ;dc<dDUMMY_CLIENT_COUNT ; dc++)
		{
			if	(strcmp(dcClient[dc].id,"") == 0)
				continue;

			int iTime = timeGetTime() - dcClient[dc].time;
			if	(abs(iTime) <= 5000 )
				continue;

			strcpy(dcClient[dc].id,"");
			dcClient[dc].time = 0;
		}

#ifdef dUSE_SERVERSTATUS_REPORT
		if	(l_bIsIgnoreSS	==	FALSE && timeGetTime() - dwSSCheckTime >= 30000)
		{	//	30√ø° πæø DBø° ¿E¡§∫∏∏¶ ∞ªΩ¡.
			if	(g_SS.isReady() )
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
#endif
		DWORD	dwCurrentTime	=	timeGetTime();

		//	√Eπæø
		if	(dwCurrentTime - checkTime > 5000)
		{
			g_lifeCodeLogServerMgr.setUserCount(UserManager.Size());

			if	(g_bIsDisplayLifeSign	==	FALSE)
				printf("- Current User Count : %d\n- [H]+Enter : Help\n\n",UserManager.Size());

			checkTime = timeGetTime();
		}

		int	iPastSecond	=	dwCurrentTime/1000-dwLastSendDummPacketTime/1000;
		//	30√ø° πæø ıπ ∂ ¿E?
		if	(iPastSecond	>=	30	||	iPastSecond	<	0)
		{
			CM_DUMMY	dummy;
			memset(&dummy,0,sizeof(CM_DUMMY));
			dummy.base.set(sizeof(CM_DUMMY),dCM_DUMMY);
			userdbsock.SendPacket((char *)&dummy,dummy.base.wSize);

			dwLastSendDummPacketTime	=	dwCurrentTime;
		}
	}

	hAdditionThread	=	NULL;

	g_lifeCodeMgr.endCheck(eLIFE_CODE_ADDITIONAL);

	_log("END AdditionThread ...");
	return;
}

int
SendToAllWorld(void *_lpPacket)
{
	cMSG_BASE_TYPE_FORWORLD*	lpBasePacket	=	(cMSG_BASE_TYPE_FORWORLD*)_lpPacket;

	for (int i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		if	(l_aWorldSocket[i].isLoopRecv	==	FALSE)
			continue;

		l_aWorldSocket[i].SendPacket((char *)_lpPacket,lpBasePacket->wSize);
	}

	return	TRUE;
}

int
SendToWorld(void *_lpPacket,cSOCKET *_lpSocket)
{
	if	(!_lpSocket)
		return	SendToAllWorld(_lpPacket);

	cMSG_BASE_TYPE_FORWORLD*	lpBasePacket	=	(cMSG_BASE_TYPE_FORWORLD*)_lpPacket;

	if	(_lpSocket->isLoopRecv	==	FALSE)
		return	FALSE;

	_lpSocket->SendPacket((char *)_lpPacket,lpBasePacket->wSize);

	return	TRUE;
}

void
WorldCtlThread(LPVOID lpParameter)
{
	int worldidx = (int)lpParameter;

	if	(!l_aWorldSocket[worldidx].init(dWORLDSERVER_PORT,srvList.aServerList[worldidx].m_strIP,srvList.aServerList[worldidx].m_strName,sizeof(cMSG_BASE_TYPE_FORWORLD)))
	{	
		_log("[%d] Connecting Fail to [%s]WORLD Server",worldidx,srvList.aServerList[worldidx].m_strIP);
		return;
	}

	strcpy(l_aWorldSocket[worldidx].name,srvList.aServerList[worldidx].m_strName);
	_log("START connect world sock name = [%s(%d)]",l_aWorldSocket[worldidx].name,worldidx);

	Sleep(100);

	SERVERStoWORLD_LOGIN	cLogin;

	memset(&cLogin,0,sizeof(SERVERStoWORLD_LOGIN));
	cLogin.base.set(sizeof(cMSG_BASE_TYPE_FORWORLD)+dMAPNAME_LENGTH+2,dSERVERStoWORLD_LOGIN);

	strcpy(cLogin.strServerName,"#login");

	cLogin.wMapCount	=	0;
	cLogin.wServerIndex	=	0xffff;

	l_aWorldSocket[worldidx].SendPacket((char *)&cLogin,cLogin.base.wSize);

	g_eventTimeManager.sendEventData(&l_aWorldSocket[worldidx]);

	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);

	WORD	type=0,recvsize=0;

	int		iLifeCodeIndex	=	eLIFE_CODE_WORLD0+worldidx;

	g_lifeCodeMgr.beginCheck(iLifeCodeIndex);

	l_aWorldSocket[worldidx].isLoopRecv	=	TRUE;

	while(TRUE)
	{
		Sleep(1);

		if	(!l_aWorldSocket[worldidx].isLoopRecv)
			break;

		g_lifeCodeMgr.putLifeSign(iLifeCodeIndex);

		while(l_aWorldSocket[worldidx].GetRecvPacket(recvpacket))
		{
			recvsize = *(WORD *)recvpacket;
			type = *(WORD *)(recvpacket+2);

			if	(type == dCM_COMPRESS)
			{
				CM_COMPRESS	comp;
				memcpy(&comp,recvpacket,recvsize);
				cSRVUTIL::Uncompress((unsigned char *)comp.data,(unsigned char *)recvpacket,comp.wSize - sizeof(WORD)*3,comp.wSourceSize);
				recvsize = *(WORD *)recvpacket;
				type = *(WORD *)(recvpacket+2);
			}

			switch(type)
			{
				case dDLMSG_COPY_DATA_RESULT	:
				{
					DLMSG_COPY_DATA_RESULT	*lpPacket	=	(DLMSG_COPY_DATA_RESULT *)recvpacket;

					CClient	* user = UserManager.GetClientPnt(lpPacket->strId);

					if	(user)
					{
						SL_RESULT_OF_COPY_AVATAR	packet;

						packet.base.set(sizeof(packet),dSL_RESULT_OF_COPY_AVATAR);
						packet.wResult		=	lpPacket->wResult;
						packet.wServerType	=	lpPacket->wServerType;
						user->m_wSecurityCode=	random(0x6fff)+1;	//	∂ ¿Øøº∫ ∞ª∏¶ ¿ß ∫π EπEEπ∏≤
						packet.wSecurityCode=	user->m_wSecurityCode;

						user->AddSendPacket((char*)&packet,packet.base.wSize);
					}
					break;
				}

				case	dW2L_GVG_AVATAR_ORG_DATA	:
				{
					OperateW2L_GVG_AVATAR_ORG_DATA((W2L_GVG_AVATAR_ORG_DATA*)recvpacket);
					break;
				}

				case	dW2L_ASK_GVG_AVATAR_ORG_DATA_RESULT	:
				{
					OperateW2L_ASK_GVG_AVATAR_ORG_DATA_RESULT((W2L_ASK_GVG_AVATAR_ORG_DATA_RESULT*)recvpacket);
					break;
				}

				case	dW2L_TOSS_ASK_GVG_AVATAR_ORG_DATA	:
				{
					OperateW2L_TOSS_ASK_GVG_AVATAR_ORG_DATA(&l_aWorldSocket[worldidx],(W2L_TOSS_ASK_GVG_AVATAR_ORG_DATA*)recvpacket);
					break;
				}

				case	dWORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT	:
				{
					OperateTransAvatarToGVGResultPacket((WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT*)recvpacket);
					break;
				}

				case	dWORLDtoSERVERS_CHANGE_GVG_SERVER_GUILD_MASTER	:
				{
					OperateChangeGVGServerGuildMaster((WORLDtoSERVERS_CHANGE_GVG_SERVER_GUILD_MASTER*)recvpacket);
					break;
				}

				case	dWORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT	:
				{
					OperateTransGuildToGVGResultPacket((WORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT*)recvpacket);
					break;
				}

				case	dWORLDtoSERVERS_TRANS_AVATAR_TO_GVG_SERVER	:
				{
					OperateTransAvatarToGVGServerPacket(&l_aWorldSocket[worldidx],(WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_SERVER*)recvpacket);
					break;
				}
				case	dWORLDtoSERVERS_TRANS_GUILD_TO_GVG_SERVER	:
				{
					OperateTransGuildToGVGServerPacket(&l_aWorldSocket[worldidx],(WORLDtoSERVERS_TRANS_GUILD_TO_GVG_SERVER*)recvpacket);
					break;
				}

				case dWORLDtoSERVERS_LIFE_SIGN		:
				{
					OperateWorldLifeSignPacket(&l_aWorldSocket[worldidx],(WORLDtoSERVERS_LIFE_SIGN*)recvpacket);
					break;
				}

				case dWORLDtoSERVERS_SIMPLE_WORK	:
				{
					OperateSimpleWorldWork(&l_aWorldSocket[worldidx],(WORLDtoSERVERS_SIMPLE_WORK*)recvpacket);
					break;
				}

				case	dWORLDtoSERVERS_EVENT_TIME_DATA	:
				{
					SERVERStoWORLD_MODIFY_EVENT_TIME	*lpPacket	=	(SERVERStoWORLD_MODIFY_EVENT_TIME	*)recvpacket;

					g_eventTimeManager.updateData(lpPacket->wCount,lpPacket->aEventList);
					break;
				}

				case dWORLDtoSERVERS_GETAVAS:
				{
					WORLDtoSERVERS_GETAVAS * rPacket;

					rPacket = (WORLDtoSERVERS_GETAVAS *)recvpacket;
					l_Ulist.setData(rPacket->strID , eRECV_DATA , dWORLDtoSERVERS_GETAVAS);

					CClient	* user = UserManager.GetClientPnt(rPacket->loginSerial);

					if	(!user)
					{
						_log("Do not send Avatar List \n");
						break;
					}

					if	(STRICMP(rPacket->strID,user->GetID())!=0)
					{
						_log("Do not send Avatar List [%s,%d] [%s,%d]\n",rPacket->strID,rPacket->loginSerial,user->GetID(),user->serial);
						break;
					}

					SL_AVATAR_LIST		sAvaList;
					memset(&sAvaList,0,sizeof(SL_AVATAR_LIST));
					sAvaList.base.set(sizeof(SL_AVATAR_LIST),dSL_AVATAR_LIST);
					memcpy(sAvaList.aAvatarList,rPacket->aAvatarList,sizeof(cAVATAR_INFO)*dMAX_AVATAR_COUNT);

					user->m_wSecurityCode	=	random(0x6fff)+1;	//	∂ ¿Øøº∫ ∞ª∏¶ ¿ß ∫π EπEEπ∏≤
					sAvaList.wSecurityCode	=	user->m_wSecurityCode;

					if	(STRICMP(rPacket->strID,"")!=0 && STRICMP(rPacket->strID,user->GetID())==0 && rPacket->loginSerial == user->serial)
					{
						user->AddSendPacket((char *)&sAvaList,sAvaList.base.wSize);
						SendPacket(user->GetSerial());
					}
					else
					{
						_log("Send List Error : recv[%s/%d] user[%s/%d]",rPacket->strID,rPacket->loginSerial,user->GetID(),user->serial);
					}

					for(int z = 0;z<dMAX_AVATAR_COUNT;z++)
					{	
						if(sAvaList.aAvatarList[z].m_wAvatarIndex==0xffff)	continue;
						strcpy(user->list[ sAvaList.aAvatarList[z].m_wAvatarIndex ].name,sAvaList.aAvatarList[z].m_strName);
						user->list[ sAvaList.aAvatarList[z].m_wAvatarIndex ].lev = sAvaList.aAvatarList[z].m_wLevel;
						user->list[ sAvaList.aAvatarList[z].m_wAvatarIndex ].job = sAvaList.aAvatarList[z].m_wJob;
						user->list[ sAvaList.aAvatarList[z].m_wAvatarIndex ].lastfield = sAvaList.aAvatarList[z].m_wLastField;
					}
					break;
				}

				case dWORLDtoSERVERS_CREATEAVA:
				{
					WORLDtoSERVERS_CREATEAVA * rPacket;
					rPacket = (WORLDtoSERVERS_CREATEAVA *)recvpacket;

					CClient	* user = UserManager.GetClientPnt(rPacket->loginSerial);

					if	(!user || STRICMP(rPacket->strID,user->GetID())!=0)
						break;

					SL_RESULT_CREATE_AVATAR		cCreateAva;

					memset(&cCreateAva,0,sizeof(SL_RESULT_CREATE_AVATAR));
					cCreateAva.base.set(sizeof(SL_RESULT_CREATE_AVATAR),dSL_RESULT_CREATE_AVATAR);
					user->m_wSecurityCode		=	random(0x6fff)+1;	//	∂ ¿Øøº∫ ∞ª∏¶ ¿ß ∫π EπEEπ∏≤
					cCreateAva.wSecurityCode	=	user->m_wSecurityCode;

					switch(rPacket->wResult)
					{
						case 0:
							cCreateAva.wResult = dRESULT_CREATE_AVATAR_SUCESS;
							break;
						case -3:
							cCreateAva.wResult = dRESULT_CREATE_AVATAR_EXIST_SAME_NAME;
							break;
						case -4:
							cCreateAva.wResult = dRESULT_CREATE_AVATAR_AVATAR_FULL;
							break;
						case -1:
						case -2:	//	SP≥ª∫Œø°º≠¿ ø°∑Ø.. 
							cCreateAva.wResult = dRESULT_CREATE_AVATAR_FAILED;
					}
					cCreateAva.wIndex = rPacket->wIndex;
					memcpy(&cCreateAva.avatarInfo,&rPacket->aCharacter,sizeof(cAVATAR_INFO));
					user->AddSendPacket((char *)&cCreateAva,cCreateAva.base.wSize);
					SendPacket(user->GetSerial());
					break;
				}

				case dWORLDtoSERVERS_DELETEAVA:
				{
					WORLDtoSERVERS_DELETEAVA * rPacket;
					rPacket = (WORLDtoSERVERS_DELETEAVA *)recvpacket;

					CClient	* user = UserManager.GetClientPnt(rPacket->loginSerial);
					if	(!user)
						break;

					SL_RESULT_REMOVE_AVATAR		cDelAva;
					memset(&cDelAva,0,sizeof(SL_RESULT_REMOVE_AVATAR));
					cDelAva.base.set(sizeof(SL_RESULT_REMOVE_AVATAR),dSL_RESULT_REMOVE_AVATAR);
					user->m_wSecurityCode	=	random(0x6fff)+1;	//	∂ ¿Øøº∫ ∞ª∏¶ ¿ß ∫π EπEEπ∏≤
					cDelAva.wSecurityCode	=	user->m_wSecurityCode;

					if(rPacket->wResult>=0 && rPacket->wResult< dMAX_AVATAR_COUNT)
						cDelAva.wResult = dRESULT_REMOVE_AVATAR_SUCESS;
					else
						cDelAva.wResult = dRESULT_REMOVE_AVATAR_FAILED;

					user->AddSendPacket((char *)&cDelAva,cDelAva.base.wSize);
					SendPacket(user->GetSerial());
					break;
				}

				case dWORLDtoSERVERS_LOADAVA:
				{
					WORLDtoSERVERS_LOADAVA * rPacket;
					rPacket = (WORLDtoSERVERS_LOADAVA *)recvpacket;
					l_Ulist.setData(rPacket->strID , eRECV_DATA , dWORLDtoSERVERS_LOADAVA);

					CClient	* user = UserManager.GetClientPnt(rPacket->loginSerial);

					if	(!user || STRICMP(user->GetID(),rPacket->strID)!=0)
					{
						if	(user)
							_log("ERROR WORLDtoSERVERS_LOADAVA : [%s] [%s]",user->GetID(),rPacket->strID);
						break;
					}
					
					user->loginTime = timeGetTime() - user->loginTime;

					if	(user->loginTime	>	1000)
						_log("SELECT AVATAR Time.. = %s, %d.%03d√",user->GetID(),user->loginTime/1000,user->loginTime%1000);

					user->loginTime = 0;

					SL_RESULT_SELECT_AVATAR		sSelectAva;
					memset(&sSelectAva,0,sizeof(SL_RESULT_SELECT_AVATAR));
					sSelectAva.base.set(sizeof(SL_RESULT_SELECT_AVATAR),dSL_RESULT_SELECT_AVATAR);

					sSelectAva.wResult		=	rPacket->wResult;
					user->m_wSecurityCode	=	random(0x6fff)+1;	//	∂ ¿Øøº∫ ∞ª∏¶ ¿ß ∫π EπEEπ∏≤
					sSelectAva.wSecurityCode=	user->m_wSecurityCode;

					if	(sSelectAva.wResult  == eLOADAVA_SUCCESS)
						user->isConnectGame = TRUE;

					user->AddSendPacket((char *)&sSelectAva,sSelectAva.base.wSize);
					SendPacket(user->GetSerial());
					
					strcpy(dcClient[iDCCount].id,rPacket->strID);	
					dcClient[iDCCount].time = timeGetTime();
					iDCCount++;	if(iDCCount>=4999)	iDCCount = 0;
//#ifdef _FOR_KOREA

//#endif
					break;
				}

				case dWORLDtoSERVERS_DIS:
				{
					WORLDtoSERVERS_DIS * rPacket;
					rPacket = (WORLDtoSERVERS_DIS *)recvpacket;

					CClient	* user = UserManager.GetClientPnt(rPacket->strId);

					if(user!=NULL)	//	¿≥¿ª ∂∂E. 
					{	
						SL_OUT		out;
						memset(&out,0,sizeof(SL_OUT));
						out.base.set(sizeof(SL_OUT),dSL_OUT);
						strcpy(out.strCause,"∞∞¿∫ æ¿¿ ¿Ø¿˙∞° ∑Œ±¿Œ¿ª Ω√Ω¿œ. \n");
						user->AddSendPacket((char *)&out,out.base.wSize);
						SendPacket(user->GetSerial());
					}

					LU_LOGOUT	logout;

					memset(&logout,0,sizeof(LU_LOGOUT));
					logout.base.set(sizeof(LU_LOGOUT),dLU_LOGOUT,0);
					memcpy(logout.strId,rPacket->strId,dID_LENGTH);

					if	(user)
						memcpy(logout.strIp,user->ip,dIP_SIZE);
					else
						strcpy(logout.strIp,"NULL");

					userdbsock.SendPacket((char *)&logout,logout.base.wSize);
					break;
				}

				default:
				{
					break;
				}
			}
		}
	}

	g_lifeCodeMgr.endCheck(iLifeCodeIndex);

	hWorldThread[worldidx] = NULL;
	_log("END world sock name = [%s(%d)]",l_aWorldSocket[worldidx].name,worldidx);
	return;
}

void USERDBCtlThread(LPVOID lpParameter)
{
	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);
	WORD	type=0,recvsize=0;

	_log("START USERDBCtlThread");

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_USER_DBC);

	while(userdbsock.isLoopRecv)
	{
		Sleep(1);

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_USER_DBC);

		while(userdbsock.GetRecvPacket(recvpacket))
		{
			recvsize	=	*(WORD *)recvpacket;
			type		=	*(WORD *)(recvpacket+2);

			if	(type == dCM_COMPRESS)
			{
				CM_COMPRESS	comp;
				memcpy(&comp,recvpacket,recvsize);
				cSRVUTIL::Uncompress((unsigned char *)comp.data,(unsigned char *)recvpacket,comp.wSize - sizeof(WORD)*3,comp.wSourceSize);
				recvsize = *(WORD *)recvpacket;
				type = *(WORD *)(recvpacket+2);
			}

			switch(type)
			{
				case dWORLDtoSERVERS_LIFE_SIGN		:
				{
					OperateWorldLifeSignPacket(&userdbsock,(WORLDtoSERVERS_LIFE_SIGN*)recvpacket);
					break;
				}

				case	dUL_CHANGEVER:
				{
					UL_CHANGEVER * rPacket;
					rPacket = (UL_CHANGEVER	*)recvpacket;
					sLauncher.wLastestVersion = version = rPacket->ver;
					_log(" Version = %d",version);
					//	noticeScp º¡§
					{
						char tmp[32];
						sprintf(tmp,"%d",version);
						noticeScp.SetData( tmp,"VER");
						noticeScp.Save();
					}
					break;
				}

				case	dUL_CHANGEMAXUSER:
				{
					UL_CHANGEMAXUSER * rPacket;
					rPacket = (UL_CHANGEMAXUSER	*)recvpacket;
					maxuserCount = rPacket->maxuser;
					_log("Max User Count = %d",maxuserCount);
					//	configScp º¡§
					{
						char tmp[32];
						sprintf(tmp,"%d",maxuserCount);
						configScp.SetData( tmp,"MAXUSER");
						configScp.Save();
					}
					break;
				}

				case	dUL_ONOFF:
				{
#ifdef _FOR_KOREA
					_log("Test For Admin Tool Message : [%s]",sLauncher.strMessage);
#endif
					UL_ONOFF	* rPacket;
					rPacket = (UL_ONOFF	*)recvpacket;
					sLauncher.wServerStatus = (WORD)rPacket->onoff;
					strcpy(sLauncher.strMessage,rPacket->msg);
					sLauncher.base.wSize = sizeof(cMSG_BASE_TYPE)+sizeof(WORD)*2+dNOTICE_FOR_LAUNCHER_DOWNLOAD_ADDRESS_SIZE+strlen(sLauncher.strMessage);
#ifdef _FOR_KOREA
					_log("Change OnOff Command : OnOff %d, msg [%s]",rPacket->onoff,rPacket->msg);
					_log("Test For Admin Tool Message : [%s]",sLauncher.strMessage);
#endif
					//	noticeScp º¡§
					{
						char tmp[32];
						sprintf(tmp,"%d",rPacket->onoff);
						noticeScp.SetData( tmp,"STATUS");
						if( stricmp( rPacket->msg, "" ) !=0 )
							noticeScp.SetData( rPacket->msg,"NOTICE");
						noticeScp.Save();
					}
				}
				break;

				case dUL_RESULT_LOGIN_OTP:
				{
					UL_RESULT_LOGIN_OTP *rPacket=	(UL_RESULT_LOGIN_OTP *)recvpacket;
					CClient				*user	=	UserManager.GetClientPnt(rPacket->base.wSerial);

					if	(!user)
						break;

					{
						SL_LOGIN_RESULT_OTP sPacket;
						sPacket.base.set(sizeof(SL_LOGIN_RESULT_OTP),dSL_LOGIN_RESULT_OTP);

						sPacket.iResult = rPacket->dwResult;

						switch(rPacket->dwResult)
						{
						case dRESULT_LOGIN_OTP_SUCCESS:		
							sPacket.iResult = dRESULT_LOGIN_OTP_SUCCESS;
							break;
						case dRESULT_LOGIN_OTP_INCORRECT:	
							sPacket.iResult = dOTP_RESULT_INCORRECT;	
							break;
						case dRESULT_LOGIN_OTP_EXPIRE:		
							sPacket.iResult = dOTP_RESULT_EXPIRE;		
							break;
						case dRESULT_LOGIN_OTP_ERROR:		
							sPacket.iResult = dOTP_RESULT_FAIL;			
							break;
						}

						user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
						SendPacket(user->GetSerial());

						if	(rPacket->dwResult	!=	0)
							break;
					}

					//	∏ÆΩ∫Æ∏¶ ø‰√ª. æ∑° login resultø ∞. E¡§∏Æœ¿. ¡?E20091022
					{
						int widx =-1;

						for(int i=0;i<dMAX_SERVER_COUNT;i++)
						{	
							if	(strcmp(srvList.aServerList[i].m_strName,"") != 0 && strcmp(srvList.aServerList[i].m_strName,user->GetSelectServer()) == 0)
							{	
								if	(strcmp(l_aWorldSocket[i].name,"")==0)
									widx = -1;
								else
									widx = i;
								break;
							}
						}

						if	(widx!=-1)
						{

							if	(l_aWorldSocket[widx].sock!=NULL)
							{
								SERVERStoWORLD_GETAVAS	cGetAvas;
								memset(&cGetAvas,0,sizeof(SERVERStoWORLD_GETAVAS));
								cGetAvas.base.set(sizeof(SERVERStoWORLD_GETAVAS),dSERVERStoWORLD_GETAVAS);
								cGetAvas.loginSerial = user->GetSerial();
								strcpy(cGetAvas.strId,user->GetID());
								if(!l_aWorldSocket[widx].SendPacket((char *)&cGetAvas,cGetAvas.base.wSize))
									DisUser(user->GetSerial(),"Error in dUL_RESULT_LOGIN");
								else
									l_Ulist.setData(user->dwHashIdCode , eSEND_DATA , dSERVERStoWORLD_GETAVAS);
							}
						}
						else
						{
							_logf("[%s] world [%s]∏¶ √¿ª ºEæΩ¿œ.\n",user->tmpid,user->GetSelectServer());
							DisUser(user->GetSerial(),"Error in dUL_RESULT_LOGIN2");
						}
					}
				}
				break;
				case	dUL_RESULT_LOGIN:
				{
					UL_RESULT_LOGIN	* rPacket;
					rPacket = (UL_RESULT_LOGIN	*)recvpacket;
					l_Ulist.setData(rPacket->strID , eRECV_DATA , dUL_RESULT_LOGIN);
					CClient	* user = UserManager.GetClientPnt(rPacket->base.wSerial);

					if	(!user)
					{
						_log("ERROR dUL_RESULT_LOGIN : USER = NULL (%d)\n",rPacket->base.wSerial);
						break;
					}

					if	(STRICMP(rPacket->strID,user->tmpid)!=0)
					{
						_log("ERROR dUL_RESULT_LOGIN : [%s] != [%s]\n",rPacket->strID,user->tmpid);
						break;
					}

//					_logc("[%s]'s Login Result [%d,%d]\n",user->tmpid,rPacket->dwResult,rPacket->dwCode);
					SL_RESULT_LOGIN		rLogin;

					memset(&rLogin,0,sizeof(SL_RESULT_LOGIN));
					rLogin.base.set(sizeof(SL_RESULT_LOGIN),dSL_RESULT_LOGIN);
					rLogin.dwResult		=	rPacket->dwResult;
					rLogin.dwCode		=	rPacket->dwCode;
					user->m_bOtp		=	rPacket->isOTPUser;
					user->m_wSecurityCode=	random(0x6fff)+1;	//	∂ ¿Øøº∫ ∞ª∏¶ ¿ß ∫π EπEEπ∏≤
					rLogin.wSecurityCode=	user->m_wSecurityCode;

					strcpy(rLogin.strWhy,rPacket->strWhy);

					user->AddSendPacket((char *)&rLogin,rLogin.base.wSize);
					SendPacket(user->GetSerial());

					if	(rPacket->dwResult == dRESULT_LOGIN_OTP)
					{
						user->SetID(user->tmpid);
						//	OTP∞° ø¿±E±∑∞° ≥™¡ø° æπ∏ ∏ÆΩ∫Æ∏¶ ±∏∞.
					}else
					if	(rPacket->dwResult == dRESULT_LOGIN_FAIL_PERSONALCOM)
					{
						//	¿Ø¿˙ø°∞‘ personalcom guard πÆ¡¶∂∞E∞∞˙.

					}else
					if	(rPacket->dwResult == dRESULT_LOGIN_SUCCESS || rPacket->dwResult == dRESULT_LOGIN_SUCCESS_BUT_NOT_USE_OTP || 
						rPacket->dwResult == dRESULT_LOGIN_SUCCESS_PERSONALCOM || rPacket->dwResult >=10 )
					{
						user->SetID(user->tmpid);

						int widx =-1;

						for(int i=0;i<dMAX_SERVER_COUNT;i++)
						{	
							if	(strcmp(srvList.aServerList[i].m_strName,"") != 0 && strcmp(srvList.aServerList[i].m_strName,user->GetSelectServer()) == 0)
							{	
								if	(strcmp(l_aWorldSocket[i].name,"")==0)
									widx = -1;
								else
									widx = i;
								break;
							}
						}

						if	(widx!=-1)
						{
							if	(l_aWorldSocket[widx].sock!=NULL)
							{
								SERVERStoWORLD_GETAVAS	cGetAvas;
								memset(&cGetAvas,0,sizeof(SERVERStoWORLD_GETAVAS));
								cGetAvas.base.set(sizeof(SERVERStoWORLD_GETAVAS),dSERVERStoWORLD_GETAVAS);
								cGetAvas.loginSerial = user->GetSerial();
								strcpy(cGetAvas.strId,user->GetID());
								if(!l_aWorldSocket[widx].SendPacket((char *)&cGetAvas,cGetAvas.base.wSize))
									DisUser(user->GetSerial(),"Error in dUL_RESULT_LOGIN");
								else
									l_Ulist.setData(user->dwHashIdCode , eSEND_DATA , dSERVERStoWORLD_GETAVAS);
							}
						}
						else
						{
							_logf("[%s] world [%s]∏¶ √¿ª ºEæΩ¿œ.\n",user->tmpid,user->GetSelectServer());
							DisUser(user->GetSerial(),"Error in dUL_RESULT_LOGIN2");
						}
					}
					else
					{
						CClient	* user = UserManager.GetClientPnt(rPacket->base.wSerial);

						if	(!user)
							break;

						if	(STRICMP(rPacket->strWhy,"#Login")==0)
						{	//	userdbø°∞‘ ª¡¶Ω√∞∞E. ¿E¿Ø¿˙∏¶ ≥÷æ˚’∏∞.
							LU_LOGOUT	logout;
							memset(&logout,0,sizeof(LU_LOGOUT));
							logout.base.set(sizeof(LU_LOGOUT),dLU_LOGOUT,0);
							memcpy(logout.strId,rPacket->strID,dID_LENGTH);
							memcpy(logout.strIp,user->ip,dIP_SIZE);

							userdbsock.SendPacket((char *)&logout,logout.base.wSize);
						}
					}
//#ifdef _FOR_KOREA
//					user->loginTime = timeGetTime() - user->loginTime;
//					_logf("Login Time.. = %s, %d.%03d Sec",user->GetID(),user->loginTime/1000,user->loginTime%1000);
//					user->loginTime = 0;
//#endif
					break;
				}
				case dUL_SERVER_ADD:
				{
					UL_SERVER_ADD	*rPacket=	(UL_SERVER_ADD *)recvpacket;
					int				idx		=	rPacket->ssInfo.m_bf8ServerPlaceInList; // rPacket->ssInfo.m_m_bf8ServerPlaceInList;

					if	(hWorldThread[idx])
					{
#ifdef _FOR_KOREA
						_log("[%s]º≠π∞° ¡∫πæ˙.*************************",srvList.aServerList[idx].m_strName);
#endif
						break;
					}

					strcpy(srvList.aServerList[idx].m_strIP,rPacket->ssInfo.m_strIP);
					hWorldThread[idx] = (HANDLE)_beginthread(WorldCtlThread, 0,(LPVOID)idx); 

					if	(!hWorldThread[idx])
					{
						srvList.aServerList[idx].m_wType = eSERVER_TYPE_OFF;
					}
					else
					{
						strcpy(srvList.aServerList[idx].m_strName,rPacket->ssInfo.m_strName);
						strcpy(srvList.aServerList[idx].m_strIP,rPacket->ssInfo.m_strIP);
						srvList.aServerList[idx].m_wCount = 0;
						srvList.aServerList[idx].m_wType = rPacket->ssInfo.m_wType;
						srvList.aServerList[idx].m_bf1IsInnerOpen	= rPacket->ssInfo.m_bf1IsInnerOpen;
						srvList.aServerList[idx].m_wMessage	= rPacket->ssInfo.m_wMessage;
						srvList.aServerList[idx].m_bf8ServerPlaceInList = rPacket->ssInfo.m_bf8ServerPlaceInList;
						srvList.aServerList[idx].m_bf8SettingServerIndex= rPacket->ssInfo.m_bf8SettingServerIndex;
						srvList.wCount++;
					}
					break;
				}

				case dUL_SERVER_DELETE:
				{
					UL_SERVER_DELETE * rPacket;
					rPacket = (UL_SERVER_DELETE *)recvpacket;
					
					//	Eº≠π∏¶ ª¡¶.
					l_aWorldSocket[rPacket->idx].isLoopRecv=FALSE;
					shutdown(l_aWorldSocket[rPacket->idx].sock,1);

					Sleep(1000);

					if	(hWorldThread[rPacket->idx])
						hWorldThread[rPacket->idx] = NULL;

					l_aWorldSocket[rPacket->idx].kill();
					srvList.aServerList[rPacket->idx].m_wType = eSERVER_TYPE_OFF;
					break;
				}

				case dUL_SERVER_LIST_ALL:
				{
					int j=0,i=0;
					UL_SERVER_LIST_ALL * rPacket;
					rPacket = (UL_SERVER_LIST_ALL *)recvpacket;

					for(j=0;j<rPacket->wCount;j++)
					{
						int sIdx = rPacket->aServerList[j].m_bf8ServerPlaceInList;
//						_log(" Server List ; name [%s,%s] type [%d]",srvList.aServerList[j].m_strName,srvList.aServerList[j].m_strIP,srvList.aServerList[j].m_wType);
						//	æ≤∑πE¿ ¡§ª¿ æœ∂∏E∞ªΩ.
						if	(hWorldThread[sIdx])
						{
							_log("[%s] this server is running!",rPacket->aServerList[j].m_strName);
							break;
						}

						//	∞ªΩ!!
						strcpy(srvList.aServerList[sIdx].m_strIP,rPacket->aServerList[j].m_strIP);
						strcpy(srvList.aServerList[sIdx].m_strName,rPacket->aServerList[j].m_strName);

						srvList.aServerList[sIdx].m_wCount			=	0;
						srvList.aServerList[sIdx].m_wMessage		=	rPacket->aServerList[j].m_wMessage;
						srvList.aServerList[sIdx].m_wType			=	rPacket->aServerList[j].m_wType;
						srvList.aServerList[sIdx].m_bf1IsInnerOpen	=	rPacket->aServerList[j].m_bf1IsInnerOpen;
						srvList.aServerList[sIdx].m_bf8ServerPlaceInList	=	rPacket->aServerList[j].m_bf8ServerPlaceInList;
						srvList.aServerList[sIdx].m_bf8SettingServerIndex	=	rPacket->aServerList[j].m_bf8SettingServerIndex;

						hWorldThread[sIdx]	=	(HANDLE)_beginthread(WorldCtlThread, 0,(LPVOID)sIdx); 

						if	(!hWorldThread[sIdx])
						{
							srvList.aServerList[sIdx].m_wType	=	eSERVER_TYPE_OFF;
							_log("ERROR CREATE WORLD THREAD [%s]",rPacket->aServerList[j].m_strIP);
						}
						else
						{
							srvList.wCount++;
							_log("[%s,%s] Connected Server !!\n",srvList.aServerList[sIdx].m_strName,srvList.aServerList[sIdx].m_strIP);
						}
					}
					break;
				}

				case dUS_DIS_USER:
				{
					US_DIS_USER	* rPacket;
					rPacket = (US_DIS_USER	*)recvpacket;

					CClient	* user = UserManager.GetClientPnt(rPacket->strId);

					if	(user!=NULL)	//	¿≥¿ª ∂∂E. 
					{	
						SL_OUT		out;
						memset(&out,0,sizeof(SL_OUT));
						out.base.set(sizeof(SL_OUT),dSL_OUT);
						strcpy(out.strCause,"∞∞¿∫ æ¿¿ ¿Ø¿˙∞° ∑Œ±¿Œ¿ª Ω√Ω¿œ. \n");
						user->AddSendPacket((char *)&out,out.base.wSize);
						SendPacket(user->GetSerial());
					}
					else
					{
						LU_LOGOUT	logout;
						logout.base.set(sizeof(LU_LOGOUT),dLU_LOGOUT,0);
						memcpy(logout.strId,rPacket->strId,dID_LENGTH);
						strcpy(logout.strIp,"NULL");

						userdbsock.SendPacket((char *)&logout,logout.base.wSize);
					}
					break;
				}

				case dCM_DUMMY:
				default:
				{
					break;
				}
			}
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_USER_DBC);

	hUSERDBThread	=	NULL;

	_log("END USERDBCtlThread");
	isMainLoop = FALSE;
	return ;
}

//////////////////////////////////////////////
//	method for packet method
//////////////////////////////////////////////
void
DisUser(DWORD serial,char * _why,BOOL isCut)
{
	//	∏∏æE¿Ø¿˙∞° 
	CCritical	CS(&csRemoveUser);

	if	(serial>=(DWORD)maxuserCount)
		return;

	CClient * user  = UserManager.GetClientPnt(serial);

	if	(!user)
		return;

	if	(user->loginTime)
	{
		l_Ulist.remove(user->dwHashIdCode , TRUE);
		_log("Uncompotable User ... %s, why[%s]",user->GetID(),_why);
	}
	else
		l_Ulist.remove(user->dwHashIdCode , FALSE);

	if	(user->GetIsUsed() && strcmp(user->GetID(),"")!=0 )
	{
		if	(!user->isConnectGame)
		{
			LU_LOGOUT	logout;
			memset(&logout,0,sizeof(LU_LOGOUT));
			logout.base.set(sizeof(LU_LOGOUT),dLU_LOGOUT,serial);
			memcpy(logout.strId,user->GetID(),dID_LENGTH);
			memcpy(logout.strIp,user->ip,dIP_SIZE);
			userdbsock.SendPacket((char *)&logout,logout.base.wSize);
		}
	}

	UserManager.Del(user->GetSerial());	

}

int 
GetWorldSocketIndex(char *name)
{
	int i;

	for	(i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		if	(l_aWorldSocket[i].sock!=INVALID_SOCKET && strcmp(l_aWorldSocket[i].name,"")!=0 && strcmp(l_aWorldSocket[i].name,name)==0)
			break;
	}

	if	(i>=dWORLD_MAX_USER_COUNT)
		i= -1;

	return i;
}

//////////////////////////////////////////////
//	method for Command
//////////////////////////////////////////////
BOOL
ProcessCommand(char command)
{
	static	BOOL	bIsQuitConfirm	=	FALSE;

	switch(command)
	{
		case 'h':	//	help
		case 'H':	//	help
		{
			printf("¶¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶§\n");
			printf("¶q : [Q]uit Server                         ¶\n");
			printf("¶v : toggle [V]iew server life sign        ¶\n");
			printf("¶¶¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶°¶\n");
			printf("\n\n\n");
			break;
		}

		case 'v':	//	help
		case 'V':	//	help
		{
			g_bIsDisplayLifeSign	=	1-g_bIsDisplayLifeSign;

			if	(g_bIsDisplayLifeSign)
				printf("- begin display server life sign\n\n");
			else
				printf("- hide server life sign\n\n");
			break;
		}

		case 'y':
		case 'Y':
		{
			if	(bIsQuitConfirm)
			{
				WriteServerStatusToRegistry(eST_LOGIN,eSERVER_STATUS_QUIT);

				return FALSE;//	º≠π∏¶ ¡æ∑·.
			}

			break;
		}

		case 'q':
		case 'Q':
		{
			printf(" Quit ? (y/n) ");

			bIsQuitConfirm	=	TRUE;

			return	TRUE;
		}
	}

	bIsQuitConfirm	=	FALSE;

	return TRUE;
}
