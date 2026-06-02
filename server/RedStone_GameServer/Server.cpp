// Server.cpp : Defines the entry point for the application.
//
/**********************************************************************************
	게임(필드) 관리 서버 
	 - 할당받은 필드의 관리를 담당한다.

**********************************************************************************/
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <iostream>
#include <io.h>
#include <conio.h>

#include "Client.h"
#include "cScript.h"
#include "cSRVUTIL.h"
#include "cServer.h"
#include "gameCodeHeader.h"
#include "debugCode.h"
#include "CDataSave.h"
#include "instance_guild_battle_manager.h"
#include "cPACKET_DBSERVER.h"
#include "cPACKET_LOGSERVER.h"
#include "cPACKET_WORLDSERVER.h"
#include "cPACKET_BCSERVER.h"
#include "CEvent.h"
#include "CShoppingCart.H"
#include "CGuild.h"
#include "secretDungeon.h"
#include "userCustomDataDefine.h"
#include "stressTest.h"
#include "processThread.h"
#include "cGuild.h"
#include "Hardwareinfo.h"
#include "life_code.h"
#include "reg_common.h"
#include "inputConsoleCommand.h"
#include "assert.h"
#include "crtdbg.h"
#include "CGuild.h"
#include "mess_sign.h"
#include "booking_work.h"
#include "atlbase.h"
#include "../buildVersion.h"	//	이건 공유 안하고 쓴다.
							//	없으면 파일을 생성한다.(공유 하지 말것!!)
							//	안에는
							//	#pragma	once
							//	#define	dBUILDER_NAME	"young"
							//	와 같이 선언 되어 있다. 뒤에 이름은 프로그래머 이름을 적을것

#ifdef _FOR_USA
#include "../OGP_Lib/ogpapi.h"
#include "../tinyxml2.6.1/tinyxml.h"
#endif

#ifdef	_USE_XTRAP
#include "XTrap/Xtrap_S_Interface.h"
#endif

#ifndef	_FOR_INTER
#define dUSE_SERVERSTATUS_REPORT
#endif

#ifdef _FOR_THAI
#undef dUSE_SERVERSTATUS_REPORT
#endif

#ifdef dUSE_SERVERSTATUS_REPORT
	#include "cServerStatus.h"

#endif

char	g_strServerFolder[512];
char	g_strBuildVersion[512];

#define _PACKET_OPTION_CRYPT		//	암호화해서 패킷을 날린다.
#define _PACKET_OPTION_SEND_AT_ONE	//	행동 데이타를 한번에 보낸다.
#define dDB_CONNECT_TIME			100000
#define dCONFIGFILENAME				"config-game.txt"
#define dCONFIGFILENAME_FOR_DEV		"config-game_dev.txt"
//#define dMAPINFOFILENAME			"server info.txt"
#define	dMAX_BUFFER					4096
#define dMAX_ACCEPT_COUNT			2

static BOOL isLocalOpen = FALSE;
static BOOL isDataReset = FALSE;
static int socketcount=0;
CRITICAL_SECTION csRECORD;
CRITICAL_SECTION csRemoveUser;
static DWORD dwWorkerThreadNum =3;

//	ProvisionRanking 요청을 위한
#ifdef _USE_PROVISIONRANK
	BOOL	l_bIsRecvProvisionRanking	=	FALSE;
#else
	BOOL	l_bIsRecvProvisionRanking	=	TRUE;
#endif

//	내부 테스트를 위한 
BOOL	l_bIsTestDB		=	FALSE;
char	l_serverAddr[32];


char	g_strGameServerName[dWORLD_NAME_LENGTH];
int		g_aiWorldValue[dMAX_WORLD_VALUE_MAX];

struct stAdmin
{
	BOOL	isUsed;
	int		iMyIndex;
	int		iAdminSerial;
	WORD	wVersion;
};

void CreateToolWidow();
int	SendPacketToBCS(char * _cData,WORD _wSize);
//void		SaveGuild(WORLDtoSERVERS_USER_LISTGUILD * _list);

static BOOL		isAdmin = FALSE;
stAdmin			_Admin[dMAX_ADMIN_COUNT];

static int		acceptCount = 0;
static int		acceptUserCount[dMAX_ACCEPT_COUNT];	//	접속과 삭제되는 수를 기억해서 균형을 맞추는데 사용해라.
BOOL			l_bIsWaitForUrgentSaveDataUpdate	=	TRUE;
static BOOL		isReady =FALSE;
static BOOL		g_bThreadLoop = TRUE;
static BOOL		g_bIsConnectLogServer	=	TRUE;
static int		gTick	=0;
static char		strIp[dMAX_ACCEPT_COUNT][dIP_SIZE];
static char		strDBCIp[dIP_SIZE],strLOGIp[dIP_SIZE],strBCIp[dIP_SIZE];
static DWORD	dwDBCPort,dwLOGPort,dwBCPort;
static WORD		g_wMaxUserCount=0; 
static char		maxTxtPath[2048];

HANDLE			g_hCompletionPort[dMAX_ACCEPT_COUNT];	//	IOCP에 사용되는 포트
SOCKET			lisSock[dMAX_ACCEPT_COUNT];				//	listen 소켓
HANDLE			hBCControlThread,hDBThread,hCommandThread,hAcceptThread[dMAX_ACCEPT_COUNT],hAdditionThread,hWorldThread,l_hthreadDSM,hWorkerThread[dMAX_WORKER_COUNT],hBCThread;
HANDLE			l_hGameUpdateThread	=	NULL;
HANDLE			l_hLifeCodeThread	=	NULL;
HANDLE			hRecvThread;

int			nTimer;
cScript		scp;
cSOCKET		g_socketWorld,g_socketDBC,g_socketLog,g_socketBC;
cLOG_QUE	g_LogQue;

BOOL InitSocket();
BOOL InitCompletionPort(int ac);		//	IOCP 초기화

void AcceptLoop(LPVOID lpParameter);
void WorkerThread(LPVOID lpParameter);
void AdditionThread(LPVOID lpParameter);
void DBThread(LPVOID lpParameter);
void BCThread(LPVOID lpParameter);
void WorldCtlThread(LPVOID lpParameter);
void GameUpdateThread(LPVOID lpParameter);
void LifeCodeUpdateThread(LPVOID lpParameter);
void DataSaveManagerThread(LPVOID lpParameter);
void BCControlThread(LPVOID lpParameter);
void CommandThread(LPVOID lpParameter);

void	ProcessRecvPacket(int serial);
void	ProcessSendPacket(int serial);
void	ProcessDBDisconnect();
BOOL	ProcessCommand(char *_lpstrCommand);

BOOL	g_isHaveToResetBCInfo;
void	resetUserBCInfo();

//	method for packet send
void	DisUser(int serial,char *_lpstrLog);
void	DisUser(char * _strId, char *_lpstrLog);
void	removeUser(int serial);
//void	UnsetUserData(CClient *user);
//	end define
/*
static DWORD	dwhi,dwlo;
void	StartClockCheck()
{
	__asm
	{
	rdtsc			; 클럭카운트를 edx:eax레지스터에 저장
	mov	dwhi,edx	;상위 dword 메모리에 백업
	mov	dwlo,eax	;하위 dword 메모리에 백업	
	}
}

void	EndClockCheck(char *name)
{
	__asm
	{
	rdtsc			; 클럭카운트를 edx:eax레지스터에 저장
	sub edx,dwhi
	cmp edx,0
	je  ZERO

	sub edx,1
	mov dwhi,0xffffffff
	sub dwhi,eax
	mov eax,dwhi
	add dwlo,eax
	jmp END

ZERO:
	sub	eax,dwlo
	mov	dwlo,eax	; 하위 dword 값 저장
END:
	mov	dwhi,edx	; 상위 dword값 저장

	}
	_log( "[%u][%u] \n",dwhi,dwlo);		// 결과 출력
}
*/
//////////////////////////////////////
//	기타 기능을 위한 플래그 
BOOL	isSend_UserCount	=	FALSE;

DebugTracer		* cLOG = NULL;

#include	"CGuilddefine.h"

#ifdef	_USE_NPGL
BOOL
InitGG()
{
	DWORD	dwGGErrCode	=	ERROR_SUCCESS;
	BOOL	bGGAuthOn	=	TRUE;
	// 서버 초기화 루틴에서
	dwGGErrCode			=	InitGameguardAuth(".", 2000);

	if( dwGGErrCode != ERROR_SUCCESS )
	{
		MessageBox(NULL,_ms("Error Code [#%.3d]",dwGGErrCode),"error in InitGameguardAuth",MB_ICONERROR|MB_TOPMOST);

		return	FALSE;
	// 에러 처리
	}

	return	TRUE;
}
#endif

void	SendLoginInfoToWorldServer(BOOL _isReconnect)
{
	int i;
	//	여기 
	//	world에 접속하고 이 서버에서 관리하게될 필드 정보와 기타 정보를 보내준다.
	g_game.sendWPQuizVillage(g_game.m_iOxQuizVillage,g_game.m_iWordQuizVillage);

	g_game.sendWPAskGuildBattleSchedule();

	SERVERStoWORLD_LOGIN	cwlogin;

	cwlogin.wMapCount	=	g_game.m_iFieldCount;
	cwlogin.isReconnect =	(WORD)_isReconnect;

	strcpy(cwlogin.strServerName,g_game.m_aServerList[g_iServerIndex].m_strName);
	_log("[%s] map count = %d \n",cwlogin.strServerName,cwlogin.wMapCount);
	//	맵 정보를 넣어서 보내준다.
	if	(g_game.m_iFieldCount > dGAME_MAX_FIELD_COUNT)
	{
		ERRMSG("game initialize error!!","too many field %d",g_game.m_iFieldCount);

		return;
	}

	for(i=0;i<g_game.m_iFieldCount;i++)
	{
		cwlogin.cGSInfo[i].wIndex = g_game.m_pField[i].m_iUniqueSerial;

		if (g_game.m_pField[i].m_wIsPremiumZone)
			cwlogin.cGSInfo[i].wType  = eFT_PREMIUM_ZONE;
		else
		if (g_game.m_pField[i].m_bIsGuildBattleField)
			cwlogin.cGSInfo[i].wType  = eFT_GUILD_BATTLE_FIELD;
		else
			cwlogin.cGSInfo[i].wType  = eFT_NORMAL_FIELD;

		strcpy(cwlogin.cGSInfo[i].strMapName,g_game.m_pField[i].m_strFileName);
	}

	cwlogin.wNICount = 0;

	for(int nic = 0;nic<acceptCount && nic<dMAX_ACCEPT_COUNT;nic++)
	{
		if(g_bIsOperatorServer == FALSE)			// 개발자 서버가 아니면.. 아이피 검사를 한다.
		{
			if	(isLocalOpen)
			{
				if	(strncmp(strIp[nic],"19",2)!=0 && strncmp(strIp[nic],"17",2)!=0 && strncmp(strIp[nic],"10",2)!=0)
					continue;
			}
			else
			{
				if	(strncmp(strIp[nic],"19",2)==0 || strncmp(strIp[nic],"17",2)==0 || strncmp(strIp[nic],"10",2)==0)
					continue;
			}
		}
		_log("Send Ip = %s",strIp[nic]);
		const char *lpstrConnectIP = cConfigData::Inst().m_strToWorldIP.c_str();
		if	(lpstrConnectIP[0] == 0)
			lpstrConnectIP = strIp[nic];
		strcpy(cwlogin.strIp[cwlogin.wNICount],lpstrConnectIP);
		cwlogin.wNICount++;
	}

	cwlogin.base.set((WORD)sizeof(SERVERStoWORLD_LOGIN),dSERVERStoWORLD_LOGIN);
	cwlogin.base.wSize	=	sizeof(cwlogin)-sizeof(cwlogin.cGSInfo)+sizeof(cGameServerType)*cwlogin.wMapCount;
	cwlogin.wServerIndex	=	g_iServerIndex;

	BOOL	bResult			=	g_socketWorld.SendPacket((char *)&cwlogin, cwlogin.base.wSize);
}


BOOL
FindRegKey()
{
	CRegKey		reg;

	char*	lpstrRegistry	=	(char*)lpszRegistry;

	char	strCommandLine[512],strFileName[512];

	strcpy(strCommandLine,GetCommandLine());
	strcpy(strFileName,_exportFileName(strCommandLine));

	if	(strstr(strFileName,"기획팀"))
		lpstrRegistry	=	(char*)lpszBalanceRegistry;
	else
	if	(strstr(strFileName,"결투용"))
		lpstrRegistry	=	(char*)lpszBattleRegistry;

	LPCTSTR		lpszRegistryDev	=	"software\\L&K Logic Korea\\Red Stone for Dev";

	int	iResult	=	reg.Open( HKEY_CURRENT_USER , lpszRegistryDev);

	if	(iResult	==	ERROR_SUCCESS)
	{
		strcpy(g_strRegAddress,lpszRegistryDev);
		g_hRegKey	=	HKEY_CURRENT_USER;

		return	TRUE;
	}

	iResult	=	reg.Open( HKEY_LOCAL_MACHINE , lpszRegistryDev);

	if	(iResult	==	ERROR_SUCCESS)
	{
		strcpy(g_strRegAddress,lpszRegistryDev);
		g_hRegKey	=	HKEY_LOCAL_MACHINE;

		return	TRUE;
	}

	iResult	=	reg.Open( HKEY_CURRENT_USER , lpstrRegistry);

	if	(iResult	==	ERROR_SUCCESS)
	{
		strcpy(g_strRegAddress,lpstrRegistry);
		g_hRegKey	=	HKEY_CURRENT_USER;

		return	TRUE;
	}

	iResult	=	reg.Open( HKEY_LOCAL_MACHINE , lpstrRegistry);

	if	(iResult	==	ERROR_SUCCESS)
	{
		strcpy(g_strRegAddress,lpstrRegistry);
		g_hRegKey	=	HKEY_LOCAL_MACHINE;

		return	TRUE;
	}

#ifdef	_FOR_JAPAN
	iResult		=	reg.Open( HKEY_CURRENT_USER , lpszRegistryOld);

	if	(iResult	==	ERROR_SUCCESS)
	{
		strcpy(g_strRegAddress,lpszRegistryOld);
		g_hRegKey	=	HKEY_CURRENT_USER;

		return	TRUE;
	}

	iResult	=	reg.Open( HKEY_LOCAL_MACHINE , lpszRegistryOld);

	if	(iResult	==	ERROR_SUCCESS)
	{
		strcpy(g_strRegAddress,lpszRegistryOld);
		g_hRegKey	=	HKEY_LOCAL_MACHINE;

		return	TRUE;
	}

#endif

	return	FALSE;
}

#include	"GameDLL.h"
#include	"MemoryModule.h"
#include	"CSharedMemory.h"

GetHeroFileNameProc		FPGetHeroFileName;
GetMonsterFileNameProc	FPGetMonsterFileName;
GetNpcFileNameProc		FPGetNpcFileName;
IsActionTimeProc		FPIsActionTime;
SetCurrentFolderProc	FPSetCurrentFolder;
GetCurrentFolderProc	FPGetCurrentFolder;
InitCSProc				FPInitCS;
RemoveASProc			FPRemoveAS;

static int
LocalSetCurrentFolder(char *_lpstrDestFolder)
{
	return	SetCurrentDirectory(_lpstrDestFolder);
}

static int
LocalGetCurrentFolder(int _iSize,char *_lpstrResult)
{
	return	GetCurrentDirectory(_iSize,_lpstrResult);
}

static char*
LocalGetHeroFileName(int _iIndex)
{
	static char	s_strEnd[]	=	"end";
	static char	s_astrHeroFileName[][64]	=
	{
		"Knight01.sad",			"Knight02.sad",			"Knight03.sad",
		"Warrior01.sad",		"Warrior02.sad",		"Warrior03.sad",
		"Wizard01.sad",			"Wizard02.sad",			"Wizard03.sad",
		"Werewolf01.sad",		"Werewolf02.sad",		"Werewolf03.sad",
		"Priest01.sad",			"Priest02.sad",			"Priest03.sad",
		"FallenAngel01.sad",	"FallenAngel02.sad",	"FallenAngel03.sad",
		"Rogue01.sad",			"Rogue02.sad",			"Rogue03.sad",
		"Champion01.sad",		"Champion02.sad",		"Champion03.sad",
		"Lancer01.sad",			"Lancer02.sad",			"Lancer03.sad",
		"Archer01.sad",			"Archer02.sad",			"Archer03.sad",
		"BeastTamer01.sad",		"BeastTamer01.sad",		"BeastTamer03.sad",
		"BeastTamer01.sad",		"BeastTamer01.sad",		"BeastTamer03.sad",
		"Princess01.sad",		"Princess02.sad",		"Princess03.sad",
		"MagicalGirl01.sad",	"MagicalGirl02.sad",	"MagicalGirl03.sad",
		"NecroMancer01.sad",	"NecroMancer02.sad",	"NecroMancer03.sad",
		"Devil01.sad",			"Devil02.sad",			"Devil03.sad",
		"SoulBringer01.sad",	"SoulBringer02.sad",	"SoulBringer03.sad",
		"Champion01.sad",		"Champion02.sad",		"Champion03.sad",
		"Opticalist01.sad",		"Opticalist02.sad",		"Opticalist03.sad",
		"BeastMan01.sad",		"BeastMan02.sad",		"BeastMan03.sad",
	};

	if	(_iIndex	<	0)
		return	s_strEnd;

	if	(_iIndex	>=	sizeof(s_astrHeroFileName)/sizeof(s_astrHeroFileName[0]))
		return	s_strEnd;

	return	s_astrHeroFileName[_iIndex];
}

static char*
LocalGetIndexedSadFileName(int _iIndex,int _iMaxCount)
{
	static char	s_strEnd[]		=	"end";
	static char	s_strResult[MAX_PATH];
	char		astrFileName[128][MAX_PATH];
	int			iCount			=	0;

	if	(_iIndex	<	0	||	_iIndex	>=	_iMaxCount)
		return	s_strEnd;

	WIN32_FIND_DATA	findData;
	HANDLE	hFind	=	FindFirstFile("*.sad",&findData);

	if	(hFind	==	INVALID_HANDLE_VALUE)
		return	s_strEnd;

	do
	{
		if	(findData.dwFileAttributes	&	FILE_ATTRIBUTE_DIRECTORY)
			continue;

		if	(iCount	>=	_iMaxCount)
			break;

		strncpy(astrFileName[iCount],findData.cFileName,MAX_PATH-1);
		astrFileName[iCount][MAX_PATH-1]	=	0;
		iCount++;
	}
	while	(FindNextFile(hFind,&findData));

	FindClose(hFind);

	for	(int i=0;i<iCount-1;i++)
	{
		for	(int j=i+1;j<iCount;j++)
		{
			if	(_stricmp(astrFileName[i],astrFileName[j])	>	0)
			{
				char	strTemp[MAX_PATH];
				strcpy(strTemp,astrFileName[i]);
				strcpy(astrFileName[i],astrFileName[j]);
				strcpy(astrFileName[j],strTemp);
			}
		}
	}

	if	(_iIndex	>=	iCount)
		return	s_strEnd;

	strcpy(s_strResult,astrFileName[_iIndex]);

	return	s_strResult;
}

static char*
LocalGetMonsterFileName(int _iIndex)
{
	return	LocalGetIndexedSadFileName(_iIndex,99);
}

static char*
LocalGetNpcFileName(int _iIndex)
{
	return	LocalGetIndexedSadFileName(_iIndex,99);
}

static int
LocalIsActionTime(unsigned int _dwBookedTime,unsigned int _dwCurrentTime)
{
	if	(_dwBookedTime	==	0xffffffff)
		return	FALSE;

	return	((int)(_dwCurrentTime-_dwBookedTime)	>=	0);
}

static void
LocalInitCS(void *_lpData)
{
	if	(_lpData)
		InitializeCriticalSection((LPCRITICAL_SECTION)_lpData);
}

static int
LocalRemoveAS(int _iSerial,int *_lpiRookie,unsigned short *_lpwSerial,int *_lpiASCount)
{
	return	FALSE;
}

static void
InitLocalGameDLLProc()
{
	FPGetHeroFileName	=	LocalGetHeroFileName;
	FPGetMonsterFileName=	LocalGetMonsterFileName;
	FPGetNpcFileName	=	LocalGetNpcFileName;
	FPIsActionTime		=	LocalIsActionTime;
	FPSetCurrentFolder	=	LocalSetCurrentFolder;
	FPGetCurrentFolder	=	LocalGetCurrentFolder;
	FPInitCS			=	LocalInitCS;
	FPRemoveAS			=	LocalRemoveAS;
}

class	cSmartMemory
{
	char*	m_pBuffer;
public:

	cSmartMemory()
	{
		m_pBuffer	=	NULL;
	}
	~cSmartMemory()
	{
		pKILL(m_pBuffer);
	}
	
	char*	alloc(int _iSize)
	{
		pKILL(m_pBuffer);
		m_pBuffer	=	new char [_iSize];

		return	m_pBuffer;
	}

	char*	getBuffer()
	{
		return	m_pBuffer;
	}
};

int	mainLoop()
{
	BuildMessSign();


// 	{
// 		HINSTANCE	hGameDLL	= LoadLibrary("GameDLL.dll");
// 
// 	}

// 	cFIELD	field;
// 
// 	field.load("D:/work/RS/Red Stone dev Client/Data/Scenario/Red Stone/Map/[407]T05_Bank01.rmd");

	const	int	c_iCheckLeakBlock	=	-1;

	if	(!FindRegKey())
		return ERRMSG("Error in main()","can not find registry data of Red Stone");

	if	(c_iCheckLeakBlock	!=	-1)
	{
		cMSG::Put("체크!!","메모리 릭 체크 변수가 설정되어 있습니다");
//		assert(0 && "메모리 릭 체크 변수가 세팅되어 있습니다!!");

		_CrtSetBreakAlloc( c_iCheckLeakBlock );
	}

	{
		char	strTitle[512];

#ifdef	_FOR_KOREA
		sprintf(strTitle,"RS Game server for korea. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_JAPAN
		sprintf(strTitle,"RS Game server for japan. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_THAI
		sprintf(strTitle,"RS Game server for thai. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_USA
		sprintf(strTitle,"RS Game server for USA. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#else
		sprintf(strTitle,"RS Game server. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#endif

		strcpy(g_strBuildVersion,strTitle);

		SetConsoleTitle(strTitle);

		WriteServerInfoToRegistry(eST_GAME,"game server");

		if	(ReadAgentSignWithRegistry(eST_GAME)	==	eAGENT_SIGN_BEGIN_BY_AGENT)
		{
			WriteAgentSignToRegistry(eST_GAME,eAGENT_SIGN_EMPTY);

			g_bIsBeginByAgent	=	TRUE;

			cMSG::Disable();
		}
	}

	g_isHaveToResetBCInfo = FALSE;

	for(int z=0;z<dMAX_ADMIN_COUNT;z++)
		_Admin[z].isUsed = FALSE;

	char path[1024];
	FPGetCurrentFolder(1024,path);
	FPGetCurrentFolder(512,g_strServerFolder);
	FILE * testsfp	=	fopen("isInnerTestServer2","rb");

	if	(testsfp)
	{
		l_bIsTestDB	=	TRUE;

		MessageBox(NULL,"for inner test server","WARNING!!",MB_OK);

		fclose(testsfp);testsfp = NULL;
	}

	sprintf(maxTxtPath,"%s/maxusercount.txt",path);

	FILE	*fp;

	fp	=	fopen(maxTxtPath,"r");

	if	(fp)
	{
		char tmptxt[10];

		fread(tmptxt,sizeof(DWORD),1,fp);

		g_wMaxUserCount = atoi(tmptxt);

		fclose(fp);
	}

	printf("Current Max User Count = %d \n",g_wMaxUserCount);

	printf("* load server data\n");

	if	(!g_game.loadConfigData())
		printf("- Failed\n");	//	서버 정보 로딩

	if	(g_bIsDuelServer)
		if	(!g_game.loadConfigForBattleData())
			printf("- Failed\n");	//	서버 정보 로딩

	printf("- ok\n");

#ifdef	_OGP_SERVICE
// 	if	(g_bIsMainTestServer)
// 		_InitOGPLibrary(dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, "http://api.ogplanet.com");
// 	else
// 		_InitOGPLibrary(dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, "https://billing.ogplanet.com/api");
#endif

#ifdef	_USE_NPGL
	if	(g_bIsOpenedTestServer)
		g_bIsUseNPGL	=	FALSE;

	if	(g_bIsUseNPGL)
		if	(!InitGG())
			return 0;
#endif

	if	(IsFile(dCONFIGFILENAME_FOR_DEV))
	{
		scp.Read(dCONFIGFILENAME_FOR_DEV);
	}
	else
	if	(scp.Read(dCONFIGFILENAME) == -1)
	{
		cMSG::Error("error!!","can not find config-game.txt file");
		return 0;
	}

	hAdditionThread =	NULL;
	hBCControlThread=	NULL;
	hWorldThread	=	NULL;
	l_hthreadDSM	=	NULL;

	int threadcount=0;

	for	(threadcount=0;threadcount<dMAX_ACCEPT_COUNT;threadcount++)
		hAcceptThread[threadcount]	=	NULL;
	for	(threadcount=0;threadcount<dMAX_WORKER_COUNT;threadcount++)
		hWorkerThread[threadcount] = NULL;

	//	윈도우 끝나면 나머지 초기화 
	//	스택 할당하고...
	int i	=	0;
	FPInitCS(&csRECORD);
	FPInitCS(&csRemoveUser);

	cConfigData::Inst().m_strGameName = scp.getData("SERVER_NAME");
	cConfigData::Inst().m_strWorldIp = scp.getData("WORLDIP");
	cConfigData::Inst().m_dwWorldPort = atoi(scp.getData("WORLDPORT"));
	cConfigData::Inst().m_strToWorldIP = scp.getData("SEND_TO_WORLDSRVIP");

	WriteWorldServerIPToRegistry(eST_GAME,(char*)(cConfigData::Inst().m_strWorldIp.c_str()));

	strcpy(strDBCIp,scp.getData("DBCACHEIP"));
	dwDBCPort = atoi(scp.getData("DBCACHEPORT"));
	strcpy(strLOGIp,scp.getData("LOGIP"));
	dwLOGPort = atoi(scp.getData("LOGPORT"));
	isLocalOpen = atoi(scp.getData("OPENFORLOCAL"));
	strcpy(strBCIp,scp.getData("BROADCASTIP"));
	dwBCPort = dBROADCAST_PORT;
	g_game.setBCSIP(strBCIp);

	cConfigData::Inst().m_strMailServerIP = scp.getData("WCG_MAIL_SERVERIP");
	cConfigData::Inst().m_strWCGMailTitle = scp.getData("WCG_MAIL_TITLE");
	cConfigData::Inst().m_strWCGMailSender = scp.getData("WCG_MAIL_SENDER");
	cConfigData::Inst().m_strWCGMailSubject = scp.getData("WCG_MAIL_SUBJECT");
	cConfigData::Inst().m_strWCGMailRecipient = scp.getData("WCG_RECIPIENT");

	int count = scp.getDataCount("WCG_CCRECIPIENT");

	for(int c=0;c<count;c++)
	{
		cConfigData::Inst().m_listWCGMailCCRecipient.push_back( std::string( scp.getData("CCWCG_RECIPIENT",c+1) ) );
	}

#ifdef dUSE_SERVERSTATUS_REPORT
	if	(stricmp(scp.getData("ignore_ss"),"true")	==	0)
		g_bIsIgnoreSSInfo	=	TRUE;
	else
	{
		if	(!g_SS.init("WORLDNAME", (char *)cConfigData::Inst().m_strGameName.c_str()))
		{
			MessageBox(NULL,"ERROR IN SS","WARNING!!",MB_OK);
			return 0;
		}
	}
#endif

	//??
	//printf("Connect world %c [%s] [%s(%s)]\n",cConfigData::Inst().m_dwWorldPort,cConfigData::Inst().m_strWorldIp,cConfigData::Inst().m_strGameName,scp.getData("GAMENAME"));

	cLOG = new DebugTracer(DTType_ALL,"RedStone_GameServer");
/*
	//	BUFFER TEST 
	{
		HANDLE hTestThread;
		for(int i =0;i< 100;i++)
			hTestThread = (HANDLE)_beginthread(TestThread, 0, (LPVOID)i); 
		while(1)
		{
			Sleep(1000);
		}
	}
*/
	if	(!g_game.init(0))
		return	0;

	if	(!InitSocket())
	{
		_log("Error InitSocket \n");
		return 0;
	}

	InitLifeCode(l_serverAddr);

	SendLoginInfoToWorldServer(0);

	_log("\n Start Game Server ... day : %d - time : %d \n",cSRVUTIL::GetTime(),cSRVUTIL::GetTimes() );

	isReady =	TRUE;

	hCommandThread	=	(HANDLE)_beginthread(CommandThread, 0, (LPVOID)0); 

	if	(hCommandThread==NULL)
	{
		_log("Fail Create CommandThread");

		ERRMSG("Fail Create CommandThread");
		return 0;
	}

	while(1)
	{
		if	(g_bGameClose || !g_socketDBC.isLoopRecv)
			break;

		Sleep(1);
	}

	Sleep(1000);

	while(g_game.isActive())
		Sleep(1);

	SendWPLifeCode(eEndLifeSign,l_serverAddr);

	//	남은 로그 데이타를 전송한다.
	_log(" Send log Data ... Wait a sec.. !!");
	if (g_bIsInnerTestServer == FALSE  && g_bIsConnectLogServer)
	{
		LOG_PACKAGE		package;

		package.base.wType	=	dLOG_PACKAGE;
		package.base.wSize	=	4;
		int	curpos			=	0;

		while(g_LogQue.getCount())
		{
			ALL_LOGMSG	logMsg;

			memcpy(&logMsg,g_LogQue.popPacket(),sizeof(ALL_LOGMSG));
			memcpy(package.package+curpos,&logMsg,logMsg.base.wSize);

			package.base.wSize	+=	logMsg.base.wSize;
			curpos				+=	logMsg.base.wSize;

			if(package.base.wSize>=(dPACKAGE_MAXSIZE - sizeof(LOG_ITEM_TRADE)))
				break;
		}

		g_socketLog.SendPacket((char *)&package,package.base.wSize);

		Sleep(1000);
	}

	_log(" Close Game Wait a sec.. !!");
	g_game.close();	//	강제 종료

	g_bThreadLoop	=	FALSE;

	Sleep(1000);

//	데이터 메니저가 종료되길 기다린다.
	{
		printf("wait for close data manager.\n");

		g_pDsManager->readyForEndManager();

		while(g_pDsManager->isActiveManager())
			Sleep(1);	//	데이터 메니저가 완전히 종료될때까지 기다린다.

		while(g_socketWorld.sPacketBuf->GetSize()>0)
		{
			printf("g_socketWorld.sPacketBuf->GetSize() = %d\n",g_socketWorld.sPacketBuf->GetSize());
			g_socketWorld.ReallySend();
			Sleep(1);
		}

		printf("work complete. -o- \n");
	}	//	데이터 메니저가 종료되길 기다린다.

	_logc("유저 정리 완료 . \n");
	DeleteCriticalSection(&csRECORD);
	DeleteCriticalSection(&csRemoveUser);
	_logc("Critical 정리 완료 . \n");

	Sleep(100);

	while(g_socketDBC.sPacketBuf->GetSize())
	{
		printf("g_socketDBC.sPacketBuf->GetSize() = %d\n",g_socketDBC.sPacketBuf->GetSize());
		g_socketDBC.ReallySend();
		Sleep(1);
	}
	printf("DBC SOCKET end !!\n");

	while(g_socketLog.sPacketBuf->GetSize())
	{
		printf("g_socketLog.sPacketBuf->GetSize() = %d\n",g_socketLog.sPacketBuf->GetSize());
		g_socketLog.ReallySend();
		Sleep(1);
	}

// #ifdef	_OGP_SERVICE
// 	_FreeOGPLibrary();
// #endif

	printf("LOG SOCKET end !!\n");
	g_socketWorld.kill();
	g_socketDBC.kill();
	g_socketLog.kill();
	g_socketBC.kill();

	Sleep(1000);
#ifdef	_USE_NPGL
	if (g_bIsUseNPGL)
		CleanupGameguardAuth(); 
#endif

	_log("End Game Server ... ");

	WriteServerStatusToRegistry(eST_GAME,eSERVER_STATUS_OFF);

	KILL(cLOG);

	return 0;
}

int main()
{
	{
		const	int	c_iCheckLeakBlock	=	-1;

		if	(c_iCheckLeakBlock	!=	-1)
		{
			cMSG::Put("체크!!","메모리 릭 체크 변수가 설정되어 있습니다");
	//		assert(0 && "메모리 릭 체크 변수가 세팅되어 있습니다!!");

			_CrtSetBreakAlloc( c_iCheckLeakBlock );
		}

  		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 
	}

	cSmartMemory	dllBuffer;
	HMEMORYMODULE	hGameDLL	=	NULL;

	CSharedMemory	shareMemory;

	if	(shareMemory.open("rsgsdllmemory"))
	{
		int	iSize	=	*(int*)shareMemory.getBuffer();

		if	(iSize	>	0	&&	iSize	<	64*1024*1024)
		{
			dllBuffer.alloc(iSize);

			memcpy(dllBuffer.getBuffer(),shareMemory.getBuffer()+4,iSize);

			hGameDLL	=  MemoryLoadLibrary(dllBuffer.getBuffer());
		}
	}

	if	(hGameDLL)
	{
		FPGetHeroFileName	=	(GetHeroFileNameProc)MemoryGetProcAddress(hGameDLL, "GetHeroFileName");
		FPGetMonsterFileName=	(GetMonsterFileNameProc)MemoryGetProcAddress(hGameDLL, "GetMonsterFileName");
		FPGetNpcFileName	=	(GetNpcFileNameProc)MemoryGetProcAddress(hGameDLL, "GetNpcFileName");
		FPIsActionTime		=	(IsActionTimeProc)MemoryGetProcAddress(hGameDLL, "IsActionTime");
		FPSetCurrentFolder	=	(SetCurrentFolderProc)MemoryGetProcAddress(hGameDLL, "SetCurrentFolder");
		FPGetCurrentFolder	=	(GetCurrentFolderProc)MemoryGetProcAddress(hGameDLL, "GetCurrentFolder");
		FPInitCS			=	(InitCSProc)MemoryGetProcAddress(hGameDLL, "InitCS");
		FPRemoveAS			=	(RemoveASProc)MemoryGetProcAddress(hGameDLL, "RemoveAS");

		FPSetCurrentFolder	=	LocalSetCurrentFolder;
		FPGetCurrentFolder	=	LocalGetCurrentFolder;
	}

	if	(!FPGetHeroFileName		||	!FPGetMonsterFileName	||	!FPGetNpcFileName	||
		 !FPIsActionTime		||	!FPSetCurrentFolder		||	!FPGetCurrentFolder	||
		 !FPInitCS				||	!FPRemoveAS)
	{
		if	(hGameDLL)
		{
			MemoryFreeLibrary(hGameDLL);
			hGameDLL	=	NULL;
		}

		InitLocalGameDLLProc();
	}

	g_pJoinPlayerManager	=	new cJoinPlayerManager();
	g_pMoveFieldUserManager	=	new cMoveFieldUserManager();
	g_pTrapManager			=	new cTrapManager();
	g_pUnionpartyManager	=	new cUnionPartyManager();
	g_pPartyManager			=	new cPartyManager();
	g_pDsManager			=	new CDataSaveManager();
	g_pSdManager			=	new cSecretDungeonManager();
	g_pIfManager			=	new cInstanceFieldManager();

	mainLoop();

	g_game.close();

	KILL(g_pSdManager);
	KILL(g_pIfManager);

	KILL(g_pJoinPlayerManager);
	KILL(g_pTrapManager);
	KILL(g_pMoveFieldUserManager);
	KILL(g_pUnionpartyManager);
	KILL(g_pPartyManager);
	KILL(g_pDsManager);
	KILL(cLOG);

	if	(hGameDLL)
		MemoryFreeLibrary(hGameDLL);

	return	0;
}

void
EndServer()
{
	g_bGameClose = TRUE;

	WriteServerStatusToRegistry(eST_GAME,eSERVER_STATUS_QUIT);
}

void
CommandThread(LPVOID lpParameter)
{
	cInputConsoleCommand	consoleInput;

	while(g_bThreadLoop)
	{
		Sleep(1);

		char	*lpstrCommand	=	consoleInput.getCommand();

		if	(lpstrCommand && !ProcessCommand(lpstrCommand))
		{
			//	서버의 기능을 종료하고 
//			SERVERStoWORLD_LOGOUT	sLogout;
//			sLogout.base.set(sizeof(SERVERStoWORLD_LOGOUT),dSERVERStoWORLD_LOGOUT);
//			g_socketWorld.SendPacket((char *)&sLogout,sLogout.base.wSize);
			Sleep(10);

			EndServer();

			WriteServerStatusToRegistry(eST_GAME,eSERVER_STATUS_QUIT);

			//	종료한다.
			break;
		}
	}

	hCommandThread	=	NULL;

	return ;
}

BOOL
InitSocket()
{
	int			i	=	0;

	SOCKADDR_IN serverAddress;
	WSADATA		wsadata;

	SetLogFolder();

	if	(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
	{
		_log("Fail WSAStartup !! \n");
		return FALSE;
	}

//	게임 초기화
	char name[32];

	gethostname(name,sizeof(name));

	struct hostent* he = gethostbyname(name);
	struct in_addr iaddr;
	memset(&iaddr,0,sizeof(iaddr));
	BOOL bForceLocalListen = (strcmp(cConfigData::Inst().m_strWorldIp.c_str(),"127.0.0.1") == 0);

	for(i =0; he->h_addr_list[i] !=0 && i<dMAX_ACCEPT_COUNT; i++)
	{
		if	(he->h_addrtype!=AF_INET)
			continue;

		memcpy(&iaddr,he->h_addr_list[i],sizeof(iaddr));
		strcpy(l_serverAddr,inet_ntoa(iaddr));
		if (bForceLocalListen)
			strcpy(l_serverAddr,"127.0.0.1");

		if(!bForceLocalListen && g_bIsOperatorServer == FALSE)		// 개발자 서버가 아니면.. 아이피 검사를 한다.
		{
			if	(isLocalOpen)
			{
				if (strncmp(l_serverAddr,"19",2)!=0 && strncmp(l_serverAddr,"17",2)!=0 && strncmp(l_serverAddr,"10",2)!=0)
					continue;
			}else
			{
				if (strncmp(l_serverAddr,"19",2)==0 || strncmp(l_serverAddr,"17",2)==0 || strncmp(l_serverAddr,"10",2)==0)
					continue;
			}
		}
		_log("Open addr  [%d] = %s\n",acceptCount,l_serverAddr);

		strcpy(strIp[acceptCount],l_serverAddr);

		DWORD	dwAddr = inet_addr(l_serverAddr);
		
		if	(dwAddr ==INADDR_NONE)
			continue;	

		if	((lisSock[acceptCount] = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
		{	
			_log("Fail listen !!");
			return ERRMSG("Fail listen !!");
		}

		memset(&serverAddress,0,sizeof(serverAddress));
		serverAddress.sin_addr.s_addr = dwAddr;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(dGAMESERVER_PORT);

		if	(bind(lisSock[acceptCount],(LPSOCKADDR)&serverAddress,sizeof(serverAddress)) == SOCKET_ERROR)
		{
			_log("Fail bind !! - %d",GetLastError());

			return ERRMSG("Fail bind","%d",GetLastError());
		}

		listen(lisSock[acceptCount],5);

		if	(!InitCompletionPort(acceptCount))
		{
			_log("Fail Init IOCP Port !! - %d",GetLastError());

			return ERRMSG("Fail Init IOCP Port","%d",GetLastError());
		}

		++acceptCount;

		if (bForceLocalListen)
			break;
	}
	
	for(int a = 0; a<acceptCount;a++)
	{
		hAcceptThread[a] = (HANDLE)_beginthread(AcceptLoop, 0, (LPVOID)a); 

		if	(hAcceptThread[a]==NULL)
		{	
			_log("Fail Create AcceptLoop");

			return ERRMSG("Fail Create AcceptLoop");
		}
	}

	//	여기 
	char* lpstrWorldIP	= (char *)cConfigData::Inst().m_strWorldIp.c_str();

	_logc("World Server [%s] [%d]",lpstrWorldIP,cConfigData::Inst().m_dwWorldPort);

	if	(!g_socketWorld.init(cConfigData::Inst().m_dwWorldPort, lpstrWorldIP ,"WORLD SERVER",sizeof(cMSG_BASE_TYPE_FORWORLD)))
	{
		_log("Fail Connect World !!\n");

		return ERRMSG("Fail Connect World !!");
	}

	_logc("DBC Server [%s] [%d]",strDBCIp,dwDBCPort);

	if	(!g_socketDBC.init(dwDBCPort,strDBCIp,"AVATAR DBC",sizeof(cMSG_BASE_TYPE_FORDB)))
	{
		_log("Fail Connect DBC !!\n");

		return ERRMSG("Fail Connect DBC !!");
	}

	if	(g_bIsInnerTestServer == FALSE && g_bIsConnectLogServer)
	{
		_logc("Log Server [%s] [%d]",strLOGIp,dwLOGPort);

		if	(!g_socketLog.init(dwLOGPort,strLOGIp,"LOG SERVER",sizeof(cMSG_BASE_TYPE_FORLOG)))
		{
			_log("Fail Connect Log Server !!");

			return ERRMSG("Fail Connect Log Server !!");
		}
	}

#ifdef _USE_BROADCAST
	if	(!g_socketBC.sock)
	{
		if	(g_bIsDisplayTick)
		{
			if	(!g_socketBC.init(dwBCPort,strBCIp,"BC SERVER",sizeof(cMSG_BASE_TYPE)) )
				_logc(" Can't Connect to BC SERVER");
			else
			{
				hBCThread = (HANDLE)_beginthread( BCThread, 0, NULL); 

				if	(hBCThread==NULL)
				{
					_log("ERROR BCThread thread create");
					g_socketBC.kill();
				}
				else
				{
					_log("BCTHREAD START : [%s] [%d]",strBCIp,dwBCPort);
				}
			}
		}
	}
#endif

	hDBThread = (HANDLE)_beginthread( DBThread, 0, NULL); 
	if	(DBThread==NULL)
	{	
		_log("ERROR : DBThread thread create");
		return FALSE;
	}

	hAdditionThread = (HANDLE)_beginthread( AdditionThread, 0, NULL); 
	if	(AdditionThread==NULL)
	{	
		_log("ERROR : Addition thread create");	
		return FALSE;
	}

	hWorldThread	= (HANDLE)_beginthread( WorldCtlThread, 0, NULL); 
	if	(WorldCtlThread==NULL)
	{
		_log("ERROR : World thread create");	
		return FALSE;
	}

	hBCControlThread= (HANDLE)_beginthread(BCControlThread,0,NULL);
	if	(hBCControlThread == NULL)
	{
		_log("ERROR : hBCControlThread create \n");	
		return FALSE;
	}

	l_hthreadDSM	= (HANDLE)_beginthread( DataSaveManagerThread, 0, NULL); 
	if	(l_hthreadDSM==NULL)
	{
		_log("ERROR : Data Save Manager Thread Create");
		return FALSE;
	}

	l_hLifeCodeThread	=	(HANDLE)_beginthread( LifeCodeUpdateThread, 0,NULL); 
//	Game Init
	l_hGameUpdateThread	=	(HANDLE)_beginthread( GameUpdateThread, 0,NULL); 

	if	(l_hGameUpdateThread	==	NULL)
	{
		_log("Failed to start game update thread!!");

		return FALSE;
	}

	return TRUE;
}

BOOL InitCompletionPort(int ac)
{
	SYSTEM_INFO sys_info;

	g_hCompletionPort[ac] = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,0);

	if	(g_hCompletionPort[ac]==NULL)
	{
		_log("Fail IOCP Port  %d Create !!",ac);
		return FALSE;
	}

	GetSystemInfo(&sys_info);

	int start_count	=	ac * dwWorkerThreadNum;
	int	end_count	=	dwWorkerThreadNum*(ac+1);

//	if(start_count>0)	return TRUE;
	for	(int i=start_count;i<end_count;i++)
	{
		hWorkerThread[i] =	(HANDLE)_beginthread(WorkerThread, 0, (void *)i);

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
	SOCKET	clientsock;
	DWORD	Flags		=	0;
	int		ac			=	(int )lpParameter;

	acceptUserCount[ac]	=	0;

	_log("START AcceptLoop ...");

#ifdef _USE_PROVISIONRANK
	while(l_bIsWaitForUrgentSaveDataUpdate || !isReady || !g_bIsCompleteReceiveGuildDataWork || !l_bIsRecvProvisionRanking)	
#else
	while(l_bIsWaitForUrgentSaveDataUpdate || !isReady || !g_bIsCompleteReceiveGuildDataWork)	
#endif
	{
		//if	(strstr(cConfigData::Inst().m_strWorldIp.c_str(),"172.31") || strstr(strBCIp,"172.31") )
			l_bIsRecvProvisionRanking	=	TRUE;

		Sleep(1000);

#ifdef _USE_PROVISIONRANK
		if	(l_bIsRecvProvisionRanking	==	FALSE)
		{
			printf("- wait for provision ranking\n");
		}
#endif
	}

	while(g_bThreadLoop)
	{
		Sleep(1);

		g_lifeCodeMgr.endCheck(eLIFE_CODE_ACCEPT_LOOP);

		if	(isDataReset || (clientsock = accept(lisSock[ac],NULL,NULL)) == INVALID_SOCKET)
			continue;

		int buf=dMAX_PACKET_LENGTH;

		if	(SOCKET_ERROR ==setsockopt(clientsock,SOL_SOCKET ,SO_SNDBUF,(const char*)&buf,sizeof(buf)))
		{
			_log("SERROR : setsockop SO_SNDBUF");
			KillSocket(&clientsock);
			continue;
		}

		if	(SOCKET_ERROR ==setsockopt(clientsock,SOL_SOCKET ,SO_RCVBUF,(const char*)&buf,sizeof(buf)))
		{
			_log("SERROR : setsockop SO_RCVBUF");
			KillSocket(&clientsock);
			continue;
		}

		LINGER	lingerStruct; 
		
		lingerStruct.l_onoff	=	1; 
		lingerStruct.l_linger	=	0; 

		if	(SOCKET_ERROR ==setsockopt(clientsock, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct)))
		{
			_log("SERROR : setsockop SO_LINGER");
			KillSocket(&clientsock);
			continue;
		}

		g_lifeCodeMgr.beginCheck(eLIFE_CODE_ACCEPT_LOOP);

		if	(CM.Size()>=dGAME_MAX_USER_COUNT)
		{	
//			SG_RESULT_CONNECT	sConnect;
//			memset(&sConnect,0,sizeof(SG_RESULT_CONNECT));
//			sConnect.base.set(sizeof(SG_RESULT_CONNECT),dSG_RESULT_CONNECT);
//			sConnect.wResult = dRESULT_CONNECT_FULL;
//			send(clientsock,(char *)&sConnect,sConnect.base.wSize,0);
//			Sleep(10);	
			_log(" What the HELL!! CM.Size() %d , dGAME_MAX_USER_COUNT %d", CM.Size(), dGAME_MAX_USER_COUNT);
			KillSocket(&clientsock);
			continue;
		}

		CClient *newUser = CM.Add();

		if(!newUser)
		{
			_log("CAUTUION!! failed to add a new user");	
			KillSocket(&clientsock);
			continue;
		}

		acceptUserCount[ac]++;

		newUser->wAcceptIndex = ac;
		newUser->SetSocket(clientsock);

		SOCKADDR_IN	saddr;

		memset(&saddr,0,sizeof(saddr));
		int tmp = sizeof(sockaddr);
		getpeername(clientsock,(SOCKADDR *)&saddr,&tmp);
		strcpy(newUser->ip,inet_ntoa(saddr.sin_addr));

#ifdef	_USE_NPGL
		if	(g_bIsUseNPGL)
			newUser->m_csa.Init();
#endif

#ifdef	_USE_XTRAP
		if	(g_bIsUseXTrap)
		{
			newUser->initBufferForXtrap(g_iXtrapMapFileCount);

			int	iResult	=	XTrap_S_SessionInit(600,g_iXtrapMapFileCount,g_pXtrapMapFile,newUser->m_pXtrapBuffer);

			if	(iResult	!=	0)
				cMSG::Error("XTrap - error!!","in session init %d : code XTrap_S_SessionInit(600,g_iXtrapMapFileCount,g_pXtrapMapFile,newUser->m_pXtrapBuffer)",iResult);
		}
#endif

#ifdef	_USE_HS
		if	(g_bIsUseHackShield)
			if	(!nsHackShield::makeGuid(newUser))
			{
				DisUser(newUser->wSerial,"nsHackShield::makeGuid ERROR"); // DisUser: 인증과정이므로 바로 자른다.
				continue;
			}
#endif
		if	(!CreateIoCompletionPort((HANDLE)newUser->GetSocket(),g_hCompletionPort[ac],(DWORD)newUser->wSerial,0))
		{
			DisUser(newUser->wSerial,"CreateIoCompletionPort ERROR");	// DisUser: 인증과정이므로 바로 자른다.
			_log("Error CreateIoCompletionPort !! - %d,[%s]",GetLastError(),newUser->ip);
			continue;
		}

		RecvPacket(newUser->wSerial);

		if	(g_wMaxUserCount<CM.Size())
			g_wMaxUserCount = CM.Size();
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_ACCEPT_LOOP);

	if	(lisSock[ac])
	{
		KillSocket(&lisSock[ac]);
	}

	printf("END AcceptLoop ...\n");

	return ;
}

void WorkerThread(LPVOID lpParameter)
{
	LP_IO_DATA	lpOV;
	DWORD dwRead,dwIOCPKey;
	int	threadSerial = (int)lpParameter;
	int	acceptIndex = threadSerial/dwWorkerThreadNum;

	_log("START WorkerThread[%d] ...",threadSerial);
	BOOL retVal = TRUE;

	while(g_bThreadLoop)
	{
		dwIOCPKey	=	0xffffffff;

		g_lifeCodeMgr.endCheck(eLIFE_CODE_WORKER);

		retVal		=	GetQueuedCompletionStatus(g_hCompletionPort[acceptIndex],&dwRead,&dwIOCPKey,(LPOVERLAPPED *)&lpOV,5000);

		g_lifeCodeMgr.beginCheck(eLIFE_CODE_WORKER);

		if	( retVal == TRUE && dwIOCPKey < dGAME_MAX_USER_COUNT && dwRead != 0 && lpOV != 0 )
		{
			if	(g_bGameClose)
				continue;

			CClient	* user = CM.GetClientPnt((WORD)dwIOCPKey);

			if	(!user)
				continue;	
			if	(lpOV->IOState == 0)
			{
				if	(!user->AddRecvPacket(dwRead))
				{
					DisUser(dwIOCPKey,"AddRecvPacket = 0");
					continue;
				}

				RecvPacket(dwIOCPKey);
				ProcessRecvPacket(dwIOCPKey);
			}
			else
			if(lpOV->IOState == 1)
			{
				int	result_send = user->CompleteSendPacket(dwRead);

				if	(result_send>0)
					SendPacket_Add(dwIOCPKey);
				else
					SendPacket(dwIOCPKey);
// 				else
// 				{
// 					if(!user->GetIsUsed() && !user->sBuffer->GetSize())
// 						removeUser(user->wSerial);
// 					else
// 						SendPacket(dwIOCPKey);
// 				}
			}
		}
		else
		{
			if	(dwIOCPKey >= dGAME_MAX_USER_COUNT)
				continue;
			CClient	* user = CM.GetClientPnt((WORD)dwIOCPKey);
			if	(!user)
				continue;	

			if	(!lpOV)
				_log("SERROR : getqueue... lpOV = NULL");

			if	(dwRead == 0 )
			{
				if	(g_bGameClose)
					continue;

				CClient	* user = CM.GetClientPnt((WORD)dwIOCPKey);
				if	(!user)		
					continue;

				DisUser(dwIOCPKey,"dwRead == 0");
			}
			else
			{
				DWORD dwError = GetLastError();

				if	(dwError!= ERROR_IO_PENDING && dwError!=ERROR_OPERATION_ABORTED && dwIOCPKey < dGAME_MAX_USER_COUNT)
				{
					_log("%d Worker thread error GetQueueCompletionStatus... error [%d]",threadSerial,dwError);

					DisUser(dwIOCPKey,"IOCP 에러!!");
				}
			}
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_WORKER);

	printf("END WorkerThread[%d] ...\n",threadSerial);
	return ;
}
 
BOOL RecvPacket(int serial)
{
	CClient	* user = CM.GetClientPnt(serial);

	if	(!user || !user->isCanSendRecv())
		return FALSE;

	DWORD	lpRecv;
	DWORD	Flags = 0;

	user->rOL.IOState = 0;

	if	(WSARecv(user->GetSocket(), &user->rOL.buf ,1,&lpRecv,&Flags,&user->rOL.OL ,NULL) ==SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();

		if	(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			DisUser(serial,"접속이 끊겼다. 그런데 받다니..");
			return FALSE;
		}
	}

	return TRUE;
}

BOOL SendPacket(int serial)
{
	CClient	* user = CM.GetClientPnt(serial);

	if	(!user || !user->GetIsUsed())
		return FALSE;

	WORD iSendSize = (WORD)user->sBuffer->GetData(user->sendpacket,0);
	if(!iSendSize)
		return TRUE;

	user->sOL.IOState = 1;
	DWORD	lpSend = 0;	
	WSABUF	buf;

	user->sendpacket_size = user->oldSendSize = iSendSize;

	buf.len	=	user->sendpacket_size;
	buf.buf	=	user->sendpacket;

	//	lpSend의 값을 iSendSize값과 비교해보면 어떤 결과가 나올까?
	if(WSASend(user->GetSocket(), &buf,1,&lpSend,0,&user->sOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();

		if	(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{	
			user->sBuffer->Next();
			DisUser(serial,"Send Error");
			return FALSE;
		}
	}

	return TRUE;
}

void
SendPacket_Add(int serial)
{
	CClient	* user = CM.GetClientPnt(serial);
	if(!user || !user->GetIsUsed())	
		return;
	user->sOL.IOState = 1;
	DWORD	lpSend = 0;	WSABUF	buf;
	buf.len = user->sendpacket_size;
	buf.buf = user->sendpacket;

//	_log("Send_ADD Data Size : %d", buf.len);
	if	(WSASend(user->GetSocket(), &buf,1,&lpSend,0,&user->sOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();
		if(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			user->sBuffer->Next();
			DisUser(serial,"센드 에러SendPacket_Add");
			return;
		}
	}
}

void
ProcessRecvPacket(int serial)
{
	if	(serial<0)
		return;

	CClient	* user = CM.GetClientPnt(serial);

	if	(!user || !user->GetIsUsed())
		return;

	WORD	type=0,recvsize=0,wProtectCode;
	char	recvpacket[dMAX_PACKET_LENGTH];
	int		resultGetRecvPacket=0;
	int		loopcount = 0;

	while(TRUE)
	{
		if	(loopcount++>10)		
			break;

		memset(recvpacket,0,dMAX_PACKET_LENGTH);
		resultGetRecvPacket = user->GetRecvPacket(recvpacket);

		if	(resultGetRecvPacket==0)		
			break;
		if	(resultGetRecvPacket==1)
		{	
			DisUser(serial,"패킷이 꼬였다. ");
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

	// 패킷 암호화 디코딩
		
		if	(type	>	dCG_CONNECT_GAME)
		{
			int	iEncTableIndex,iProtectCodeSerial;

			g_gsEncTable.decodeProtectCode(wProtectCode,&iEncTableIndex,&iProtectCodeSerial,user->m_wXorCode);

			if	(type	!=	dCG_JOIN_GAME)
			{
				if	(user->m_wReceivePacketSerial && user->m_wReceivePacketSerial	!=	0xffff	&&	user->m_wReceivePacketSerial	!=	iProtectCodeSerial)
				{
					char	strReason[256];

					sprintf(strReason,"mismatch packet serial code : packet 0x%x[%d/%d]",type,user->m_wReceivePacketSerial,iProtectCodeSerial);

					DisUser(serial,strReason);

					cACTOR	*lpActor	=	g_game.getActor(user->GetActorSerialInServer());

					if	(lpActor && lpActor->isPlayer())
 						lpActor->sendHackingLog(ePPPTR_PACKET_HACKING,"packet serial hacking");
 
 					continue;
 				}
			}

			user->m_wReceivePacketSerial	=	g_gsEncTable.getNextPacketSerial(iProtectCodeSerial);

			if	(user->m_wCurrentEncTableIndex	!=	iEncTableIndex)
			{
				if	(user->m_iAskChangeEncTableTime+30	<	g_iCounterBySecond)	//	인코드 테이블 변경한지 30초가 넘었는데도 그대로다
				{
					DisUser(serial,"mismatch packet encode table");

					cACTOR	*lpActor	=	g_game.getActor(user->GetActorSerialInServer());

					if	(lpActor && lpActor->isPlayer())
						lpActor->sendHackingLog(ePPPTR_PACKET_HACKING,"packet encode table hacking");

					continue;
				}
			}

			g_gsEncTable.decrypt(recvpacket+6,recvsize-6,user->getEncTable(iEncTableIndex));
		}

		switch(type)
		{
			case dCG_CONNECT_STRESS_CLIENT	:
			{
				if	(g_bIsStressTestServer	==	FALSE)
					break;

				g_pStressTestManager->addClient(user->wSerial);

				break;
			}

			case dCG_MODIFY_STRESS_CLIENT	:
			{
				if	(g_bIsStressTestServer	==	FALSE)
					break;
				CG_MODIFY_STRESS_CLIENT	*lpPacket	=	(CG_MODIFY_STRESS_CLIENT *)recvpacket;

				g_pStressTestManager->addReporter(user->wSerial);
				break;
			}

			case dCG_STRESSDATA		:
			{
				if	(g_bIsStressTestServer	==	FALSE)
					break;

				break;
			}

			case dSG_CLOSESERVER:
			{
				SG_CLOSESERVER	* cDisServer;
				cDisServer = (SG_CLOSESERVER *)recvpacket;

				SG_CHAT		all_chat;

				FillChattingBuffer(&all_chat,"◐관리자◑","서버를 종료하겠습니다.");

				all_chat.bf6ChatType	=	eCT_ADMIN;

				int count;
				for(count=0;count<dGAME_MAX_USER_COUNT;count++)
				{
					CClient * usr = CM.GetClientPnt(count);
					if(!usr || usr->wSerial==0xffff)
						continue;
					usr->AddSendPacket((char *)&all_chat,all_chat.base.wSize);
					SendPacket(usr->GetSerial());
				}

				for (int i=10;i;--i)
				{
					Sleep(1000);	//	10초 정도 대기하다가..

					_log("Wait for end game. %d",i);
				}

				EndServer();

				break;
			}

			case dCG_CONNECT_GAME	:
			{
				CG_CONNECT_GAME	* cConnect = (CG_CONNECT_GAME * )recvpacket;

				cConnect->strName[dNAME_LENGTH-1]	=	NULL;
				cConnect->strId[dID_LENGTH-1]		=	NULL;
				cConnect->strMacAddress[63]			=	NULL;
				cConnect->strClientCode[11]			=	NULL;

#ifdef _OGP_SERVICE
				{
					cConnect->strToken[dTOKEN_LENGTH-1]	=	NULL;
					char query[1024];
					char ret[1024];

					if	(g_bIsMainTestServer)
						sprintf(query,"http://api.ogplanet.com/checkValidToken.og?ServiceID=%d&ServiceKey=%s&Token=%s&UserID=%s&UserIP=%s&RequestType=3",	// RequestType 3-게임서버..
							dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, cConnect->strToken, cConnect->strId, user->ip
							);
					else
						sprintf(query,"https://billing.ogplanet.com/checkValidToken.og?ServiceID=%d&ServiceKey=%s&Token=%s&UserID=%s&UserIP=%s&RequestType=3",	// RequestType 3-게임서버..
							dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, cConnect->strToken, cConnect->strId, user->ip
							);

					_SendDirectHTTP(query, ret);
					
					//					_log(query);
					//					_log(ret);
					
					//	실패면 여기서 잘라.
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
							//	에러가 발생했나?
							ItemInfoEl = node->FirstChildElement("ActionInfo");
							TiXmlElement* valueEl = ItemInfoEl->FirstChildElement("ReturnCode");
							retCode = atoi(valueEl->GetText());
							
							switch(retCode)
							{
							case 10001:	//	로그인이 되지 않음.
							case 10002: //	블럭된 사용자
							case 10003: //	블럭된 IP
							case 10004: //	접속 불가 지역
							case 10005: //	존재하지 않는 사용자
							case 10006: //	존재하지 않는 서비스
							case 10007: //	유효하지 않은 토큰
							case 10008: //	존재하지 않는 토큰입니다.
							case 10009:	//	ServiceID와 토큰이 일치하지 않습니다.
							case 10010:	//	UserID와 토큰이 일치하지 않습니다.
							case 10011:	//	ServiceKey가 정상적이지 않습니다.
							case 10012:	//	접근 권한을 가지고 있지 않습니다.
							case 10013:	//	암호가 일치하지 않습니다. 로그인 실패
							case 10014:	//	X번이상 암호를 실패하였습니다. - 경고
							case 10015:	//	X번이상 실패로 아이디가 블럭되었습니다. 
							case 10016:	//	활성화되지 않은 사용자입니다.
							case 10017:	//	180일 이상 로그인하지 않아 폐기된 아이디입니다. 
								break ;
							case 10000:	//	성공
								break;
							}
						}
					}
					if(retCode != 10000)
					{
						DisUser(serial,"OGP TOKEN AUTH ERROR");	
						break;
					}
				}
#endif
				g_game.operateReceivePacketDataStaticsWork(cConnect->base.wSize);

				SERVERStoWORLD_USER_LOGIN	sUserLogin;

				sUserLogin.base.set(sizeof(SERVERStoWORLD_USER_LOGIN),dSERVERStoWORLD_USER_LOGIN);

				sUserLogin.type		=	cConnect->wType;
				sUserLogin.serial	=	serial;

#ifdef	_OGP_SERVICE
				user->SetID(cConnect->strId,cConnect->strToken);
#else
				user->SetID(cConnect->strId,NULL);
#endif
//				user->isSaveClose	=	TRUE;

				if	(STRICMP("#admin",cConnect->strId)==0)
				{
					int i;
					for(i=0;i<dMAX_ADMIN_COUNT;i++)
					{
						if(!_Admin[i].isUsed)
						{
							_Admin[i].iMyIndex = i; 
							_Admin[i].iAdminSerial= serial;
							_Admin[i].isUsed = TRUE;
							_Admin[i].wVersion = cConnect->wType;	//	type에 admin툴의 버젼을 보낸다.
							break;	
						}
					}
					if(i==dMAX_ADMIN_COUNT)	
					{
						DisUser(serial,"over Admintool user count");	
					}	//	더이상 admin을 받을 수 없다.
					else if(_Admin[i].wVersion<8)
					{
						DisUser(serial,"Incorrect Admintool version");	//	버젼으로 접속 여부를 수용해준다.
					}
					else
					{
						isAdmin = TRUE;
					}
				}
				else
				{
					strcpy(sUserLogin.strID,cConnect->strId);
					sUserLogin.dwCode = cConnect->dwCode;
					strcpy(sUserLogin.strName,cConnect->strName);
					user->hashIdCode = GetHashCode((BYTE *)sUserLogin.strID);
					user->hashNameCode = GetHashCode((BYTE *)sUserLogin.strName);

					strcpy(sUserLogin.strIP,user->ip);
					cConnect->strMacAddress[63]	=	NULL;
					strcpy(sUserLogin.strMacAddress, cConnect->strMacAddress);
					cConnect->strClientCode[11] = 0;
					strcpy(sUserLogin.strClientCode, cConnect->strClientCode);
					g_socketWorld.SendPacket((char *)&sUserLogin,sUserLogin.base.wSize);
					user->loginTime = timeGetTime();
					user->SetState(CClient::eSTATE_RECEIVE_INFO);
//					_log( "HYUN!!! : id[%s] eSTATE_RECEIVE_INFO \n", sUserLogin.strID);
				}
				break;
			}

			case dCG_JOIN_GAME:
			{
				CG_JOIN_GAME	*	joinGame;

				joinGame = (CG_JOIN_GAME *)recvpacket;

				g_game.operateReceivePacketDataStaticsWork(joinGame->base.wSize);

//				_log("[%d]join user (%s) [%d] ",cSRVUTIL::GetTimes(),user->GetID(),serial);
				SERVERStoWORLD_GET_AVADATA	cGetAvaData;

				cGetAvaData.base.set(sizeof(SERVERStoWORLD_GET_AVADATA),dSERVERStoWORLD_GET_AVADATA);
				cGetAvaData.serial	=	serial;
				cGetAvaData.type	=	1;
				cGetAvaData.dwIndex =	user->GetWorldSerial();
				strcpy(cGetAvaData.strId,user->GetID());

#ifdef	_OGP_SERVICE
				strcpy(cGetAvaData.strToken,user->m_strToken);
#endif
				cGetAvaData.badtime =	user->badtime;
//				user->isSaveClose	=	FALSE;
				g_socketWorld.SendPacket((char *)&cGetAvaData,cGetAvaData.base.wSize);

				break;
			}

			default	:
			{
				ALL_MSG	*lpPacket	=	(ALL_MSG *)recvpacket;

				if	(STRICMP(user->GetID(),"")	!=	0)
				{
					int	iFieldIndex		=	user->GetFieldIndex();
					int	iActorSerial	=	user->GetActorSerialInField();

					if	(recvsize < 6)
					{
						CLOG("checkPacketCRC","size hacked [%s]",user->GetID());
						break;
					}

					g_game.operateReceivePacketDataStaticsWork(lpPacket->base.wSize);
					g_game.addReceivePacket(iFieldIndex,lpPacket,iActorSerial);
				}
				break;
			}
		}
		SendPacket(serial);
	}
}

void
ProcessSendPacket(int serial)
{	
	SendPacket(serial);
}

void
BCControlThread(LPVOID lpParameter)
{
	_log("START BCControlThread ...");
	
	g_lifeCodeMgr.beginCheck(eLIFE_CODE_BC_CONTROL);

	while(g_bThreadLoop)
	{	
		Sleep(1);

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_BC_CONTROL);
		
		for(int z = dGAME_MAX_USER_COUNT;z;)
		{	
			--z;
			if	(!g_isHaveToResetBCInfo)
				break;
			
			if	(g_bGameClose)
				break;

			CClient * user = CM.GetClientPnt(z);

			if	(!user || user->wSerial == 0xffff || user->isCanSendRecv())
				continue;
			//	필드에 들어가 있는 상태인데.. BC Serial 이 0xffff이면, BC 접속이 
			if	(user->GetFieldIndex()!=0xffff && user->GetBCSerial() == 0xffff)
			{
				cACTOR * _lpActor = g_game.getActor(user->GetActorSerialInServer());

				if	(!_lpActor || _lpActor->m_iRookieTime)
					continue;

				GB_REG_CLIENT spacket;
				spacket.base.set(sizeof(GB_REG_CLIENT),dGB_REG_CLIENT);
				strcpy(spacket.strId,user->GetID());
				strcpy(spacket.strName,user->GetName());

				spacket.wLevel		=	_lpActor->m_iLevel;
				spacket.wGuildSerial=	_lpActor->m_wGuildSerial;
				spacket.wGuildRank	=	_lpActor->m_wGuildRank;
				spacket.wOper		=	_lpActor->m_wOperatorLevel;					//	운영자 레벨
				spacket.wSerial		=	_lpActor->m_iClientSerial;	//	돌려받을때 사용할 게임서버내의 client serial
				spacket.wCurrentField=	_lpActor->m_wCurrentField;
				spacket.wAvatarIndex=	_lpActor->m_wAvatarIndex;
				spacket.wPartySerial=	_lpActor->getPartySerial();
				
				if	(g_socketBC.SendPacket((char *)&spacket,spacket.base.wSize))
					user->SetBCSerial(0xfffe);
			}
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_BC_CONTROL);

	printf("END BCControlThread ...\n");
}

void
resetUserBCInfo()
{
	g_isHaveToResetBCInfo = FALSE;

	for(int z = dGAME_MAX_USER_COUNT;z;)
	{
		--z;
		CClient * user = CM.GetClientPnt(z);

		if	(!user || user->wSerial == 0xffff)
			continue;

		user->SetBCSerial(0xffff);
	}
}

void AdditionThread(LPVOID lpParameter)
{
	int	update_tick=0;

	_log("START AdditionThread ...");

	DWORD			dwSSCheckTime = timeGetTime();
	cHardwareInfo	hwInfo;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_ADDITIONAL);

	while(g_bThreadLoop)
	{
		Sleep(1);

		update_tick++;

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_ADDITIONAL);

		for(int z = dGAME_MAX_USER_COUNT;z;)
		{
			--z;
			CClient * user = CM.GetClientPnt(z);

			if	(!user || user->wSerial == 0xffff)
				continue;

			if	(!user->GetIsUsed())
			{
				printf("is close client %s/%s\n",user->id,user->name);
				continue;
			}

			if	(user->isWaste())
			{
				_log(" [%s (%s)] is waste. state[%d] " ,user->GetID(),user->GetName(), user->GetIsUsed());
				user->SetState(CClient::eSTATE_WASTE);
//				_log( "HYUN!!! : id[%s] eSTATE_WASTE \n", user->GetID());
				DisUser(user->wSerial,"Waste!!");
				continue;
			}

			SendPacket(user->GetSerial());
		}

		if	(g_bIsInnerTestServer == FALSE  && g_bIsConnectLogServer)
		{
			if	(g_socketLog.sock == NULL)
			{
				if	(!g_socketLog.init(dwLOGPort,strLOGIp,"LOG SERVER",sizeof(cMSG_BASE_TYPE_FORLOG)))
				{
					g_socketLog.isLoopRecv = FALSE;
					g_socketLog.kill();
					_log("Fail Connect g_socketLog !!\n");
				}
				Sleep(10);
			}

			if	(g_LogQue.m_dwTotalPacketSize>1024)
			{
				LOG_PACKAGE		package;
				package.base.wType = dLOG_PACKAGE;
				package.base.wSize = 4;
				int curpos = 0;

				while(g_LogQue.getCount())
				{
					if	(package.base.wSize	>=	(dPACKAGE_MAXSIZE - sizeof(ALL_LOGMSG)))
						break;

					ALL_LOGMSG	logMsg;

					memcpy(&logMsg,g_LogQue.popPacket(),sizeof(ALL_LOGMSG));
					memcpy(package.package+curpos,&logMsg,logMsg.base.wSize);
					package.base.wSize	+=	logMsg.base.wSize;

					curpos	+=	logMsg.base.wSize;
				}
				//	압축해서 보낼것!!
				g_socketLog.SendPacket((char *)&package,package.base.wSize);
			}
		}

		if	(update_tick<500 || g_bGameClose)
			continue;

#ifdef dUSE_SERVERSTATUS_REPORT
		if  (timeGetTime() - dwSSCheckTime >= 30000 && g_bIsIgnoreSSInfo == FALSE)
		{	//	30초에 한번씩 DB에 현재 정보를 갱신해준다.
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

		if	(!g_socketDBC.sock)
		{
			if	(!g_socketDBC.init(dwDBCPort,strDBCIp,"DBC SERVER",sizeof(cMSG_BASE_TYPE_FORDB)))
			{
				g_socketDBC.isLoopRecv = FALSE;
				g_socketDBC.kill();
				_log("RECONNECT : Fail Connect AVATAR db !!\n");
			}
			else
			{
				hDBThread = (HANDLE)_beginthread( DBThread, 0, NULL); 
				if	(DBThread==NULL)
				{
					g_socketDBC.kill();
					_log("RECONNECT : ERROR DBThread thread create");
				}else
				{
					_log("RECONNECT : [%s] [%d]",strDBCIp,dwDBCPort);
				}
			}
		}

		if	(g_socketDBC.sock!=0)
		{
			CM_DUMMY	dummy;
			dummy.base.set(sizeof(CM_DUMMY),dCM_DUMMY);
			g_socketDBC.SendPacket((char *)&dummy,dummy.base.wSize);
		}

#ifdef _USE_BROADCAST
		if	(!g_socketBC.sock)
		{
			if	(g_bIsDisplayTick)
			{
				if	(!g_socketBC.init(dwBCPort,strBCIp,"BC SERVER",sizeof(cMSG_BASE_TYPE)) )
				{
#ifndef	_FOR_INNER
					_logc(" Can't Connect to BC SERVER");
#endif
				}
				else
				{
					if	(g_isHaveToResetBCInfo)
						resetUserBCInfo();

					hBCThread = (HANDLE)_beginthread( BCThread, 0, NULL); 

					if	(BCThread==NULL)
					{
						_log("RECONNECT : ERROR BCThread thread create");
						g_socketBC.kill();
					}
					else
					{
						_log("RECONNECT : [%s] [%d]",strBCIp,dwBCPort);
					}
				}
			}
		}
		else
		{
			CM_DUMMY	dummy;
			dummy.base.set(sizeof(CM_DUMMY),dCM_DUMMY);
			g_socketBC.SendPacket((char *)&dummy,dummy.base.wSize);
		}
#endif
		update_tick = 0;
		//	월드에게 어느 소켓으로 받을지 알려준다. 0번과 1번만 비교해보자. ;;
		if	(abs(acceptUserCount[0]-acceptUserCount[1])>100)	//	소켓과 100명이상 차이가 난다면.
		{
			SERVERStoWORLD_INFO	sInfo;
			sInfo.base.set(sizeof(SERVERStoWORLD_INFO),dSERVERStoWORLD_INFO);
			sInfo.wCount = abs(acceptUserCount[0]-acceptUserCount[1]);

			if	(acceptUserCount[0]-acceptUserCount[1]>0)
				sInfo.wSocketIndex = 0;
			else
				sInfo.wSocketIndex = 1;

			g_socketWorld.SendPacket((char *)&sInfo,sInfo.base.wSize);
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_ADDITIONAL);

	printf("END AdditionThread ...\n");
	return ;
}

void
DataSaveManagerThread(LPVOID lpParameter)
{
	g_pDsManager->update();
}

void
LifeCodeUpdateThread(LPVOID lpParameter)
{
	while(g_bThreadLoop)
	{
		Sleep(10);

		if	(SendWPLifeCode(eBeginLifeSign,l_serverAddr))
			break;
	}

	WriteServerStatusToRegistry(eST_GAME,eSERVER_STATUS_ON);

	while(1)
	{
		for (int i=0;i<1000 ;i++)
		{
			Sleep(10);

			if	(g_bThreadLoop == FALSE || g_bGameClose)
			{
				SendWPLifeCode(eEndLifeSign,l_serverAddr);

				return;
			}
		}

		WriteLifeSignToRegistry(eST_GAME,1);

		int	iSign	=	ReadAgentSignWithRegistry(eST_GAME);

		if	(iSign	==	eAGENT_SIGN_END_SERVER)
		{
			WriteAgentSignToRegistry(eST_GAME,eAGENT_SIGN_EMPTY);

			EndServer();

			continue;
		}

		DWORD	dwCurTime	=	timeGetTime();

		if	(!g_lifeCodeMgr.checkAll(dwCurTime))
			SendWPLifeCode(eIncorrectStatusSign,l_serverAddr,g_lifeCodeMgr.m_strBadStatusLog);
		else
			SendWPLifeCode(eUpdateLifeSign,l_serverAddr);
	}
}

void
GameUpdateThread(LPVOID lpParameter)
{
	g_lifeCodeMgr.beginCheck(eLIFE_CODE_GAME);

	while(!g_bGameClose)
	{
		g_game.update();

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_GAME);

#ifdef dUSE_SERVERSTATUS_REPORT
		if	(g_bIsChangeMinute && g_bIsIgnoreSSInfo == FALSE)	//	1분에 한번씩 전송
			g_SS.update(cServerStatus::eERROR_TYPE_GAME_UPDATE_STATUS,1,"No problem");
#endif

		Sleep(1);
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_GAME);

	g_game.off();

	return ;
}

void
WorldCtlThread(LPVOID lpParameter)
{
	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);
	WORD	type=0,recvsize=0;

	_log("START WorldCtlThread ...");

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_WORLD_CTRL);

	while(g_bThreadLoop)
	{
		int loopcount = 0;

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_WORLD_CTRL);

		if	(!g_socketWorld.isLoopRecv && !g_socketWorld.isReconnect)	//	월드랑 연결이 끊겼다규!!
		{
			_log("ERROR : error occured!! in connet with WORLD");

			EndServer();
			break;
		}

		if	(g_socketWorld.isReconnect)
		{
			SendLoginInfoToWorldServer(1);
			g_socketWorld.isReconnect = FALSE;
		}

		while(isReady &&  g_socketWorld.GetRecvPacket(recvpacket))
		{
			loopcount++;

			if	(loopcount	>	100)
				break;

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
				case dWORLDtoSERVERS_CHECKTIME:
				{
					WORLDtoSERVERS_CHECKTIME	*	checkTime;
					checkTime = (WORLDtoSERVERS_CHECKTIME *)recvpacket;

					if(checkTime->wType==1)	//	close
					{
						isDataReset = TRUE;
						g_game.bookingBanishAllUser();
					}
					else
					{
						isDataReset = FALSE;
					}
					break;
				}

				case dWORLDtoSERVERS_WORLDNAME:
					if	(g_bIsIgnoreSSInfo == FALSE)
					{
	#ifdef dUSE_SERVERSTATUS_REPORT

						WORLDtoSERVERS_WORLDNAME * rPacket = (WORLDtoSERVERS_WORLDNAME *)recvpacket;
						sprintf(g_SS.m_strServerName,"%s_G[%s]", rPacket->strWorldName,strIp[0]);
	#endif
					}
					break;

				case dWORLDtoSERVERS_DELETE_AVATAR:
				{
					WORLDtoSERVERS_DELETE_AVATAR	*	dAvatar;
					dAvatar = (WORLDtoSERVERS_DELETE_AVATAR *)recvpacket;

					g_game.removeSaveData(dAvatar->strID,dAvatar->wAvatarIndex,2);
					break;
				}

				case dWORLDtoSERVERS_ALL_MSG:
				{
					WORLDtoSERVERS_ALL_MSG	*	all_msg;
					all_msg = (WORLDtoSERVERS_ALL_MSG *)recvpacket;

					SG_CHAT		all_chat;

					FillChattingBuffer(&all_chat,"◐관리자◑",all_msg->strMessage);

					all_chat.bf6ChatType = eCT_ADMIN;
	
					int count;
					for(count=0;count<dGAME_MAX_USER_COUNT;count++)
					{
						CClient * user = CM.GetClientPnt(count);
						if(!user || user->wSerial==0xffff)	continue;
						user->AddSendPacket((char *)&all_chat,all_chat.base.wSize);
					}
					break;
				}

				case dWORLDtoSERVERS_USER_LOGIN:
				{
//					_log("recv dWORLDtoSERVERS_USER_LOGIN\n");
					WORLDtoSERVERS_USER_LOGIN	 * cuLogin;

					cuLogin			=	(WORLDtoSERVERS_USER_LOGIN *)recvpacket;
					CClient	* user	=	CM.GetClientPnt((WORD)cuLogin->serial);

					if	(!user)
					{
//						_log("ERROR dWORLDtoSERVERS_USER_LOGIN : user==NULL");
						break;
					}

					SG_RESULT_CONNECT		sConnect;
					memset(&sConnect,0,sizeof(sConnect));

					sConnect.base.set(sizeof(SG_RESULT_CONNECT),dSG_RESULT_CONNECT);
					sConnect.base.wSize			=	sizeof(sConnect)-sizeof(sConnect.strGuildName);
					sConnect.wFieldSerial		=	cuLogin->wFieldidx;
					{
						CTimeInfo	connectTime;
						connectTime.update();

						int	iServerYear	=	connectTime.m_wYear-2000;
						if	(iServerYear	<	0)
							iServerYear	=	0;
						if	(iServerYear	>	63)
							iServerYear	=	63;

						sConnect.serverTime.year	=	iServerYear;
						sConnect.serverTime.month	=	connectTime.m_wMonth;
						sConnect.serverTime.day		=	connectTime.m_wDay;
						sConnect.serverTime.hour	=	connectTime.m_wHour;
						sConnect.serverTime.minute	=	connectTime.m_wMinute;
						sConnect.serverTime.second	=	connectTime.m_wSecond;
					}
					sConnect.dwSeasonVariable	=	g_iSeasonVariable;

					{
						user->m_wCurrentEncTableIndex=	0;
						user->m_wXorCode			=	g_gsEncTable.makeXorCode();
						user->m_wReceivePacketSerial=	random(30000)+1;
						user->buildEncTable(0);

						sConnect.wPacketSerial		=	user->m_wReceivePacketSerial;
						sConnect.wXorCode			=	user->m_wXorCode;

						user->m_wReceivePacketSerial=	g_gsEncTable.getNextPacketSerial(user->m_wReceivePacketSerial);

						memcpy(sConnect.abEncryptTable,user->getEncTable(0),dENCRYPT_TABLE_SIZE);
					}

					if	(cuLogin->iIndex	==	-1)	//	FAIL
					{
						sConnect.wResult = dRESULT_CONNECT_FAIL;
						DisUser(cuLogin->serial,"Login Failed in World");

						printf("Login Failed by World\n");
//	여기서 강제로 끊어버릴까? 아님 기다릴까?
//	_log("로그인 실패.. 여기서 강제로 끊어버릴까? 아님 기다릴까? [%s]",user->GetID());
						strcpy(sConnect.strMapName,"");
					}
					else
					if	(cuLogin->iIndex==-2)
					{
						sConnect.wResult = dRESULT_CONNECT_FULL;
						_log("ERROR login failed... over user count [%s]",user->GetID());
						strcpy(sConnect.strMapName,"");

						printf("Connect Failed by World\n");
					}
					else
					{					//	SUCCESS
						cFIELD	*lpField;

						if	(cuLogin->wFieldidx	>=	dINSTANCE_FIELD_FLAG)
						{
							lpField			=	g_game.getFieldByUniqueSerial(cuLogin->wFieldidx-dINSTANCE_FIELD_FLAG);
							cParty	*lpParty=	g_pPartyManager->get(cuLogin->wPartyIndex);

							if	(lpParty)
							{
								cInstanceField	*lpIF		=	g_pIfManager->getByUniquePartySerial(lpParty->m_dwUniqueSerial);

								if (lpIF && lpIF->m_wSerial	==	cuLogin->bf10IFSerial)
								{
									if (lpIF->m_wFloorCount	>	cuLogin->bf4IFFloor)
										lpField	=	&lpIF->m_pFloor[cuLogin->bf4IFFloor];
								}
							}
						}
						else
							lpField	=	g_game.getFieldByUniqueSerial(cuLogin->wFieldidx);

						if	(lpField)
						{
							user->SetWorldSerial(cuLogin->iIndex);
							user->badtime		=	cuLogin->badtime;
							user->SetOper(cuLogin->oper);
							user->SetNetCafeUser(cuLogin->isNetCafeUser);
#ifdef _USE_ACCOUNT_LEVEL
							user->SetOperInAccount(cuLogin->isOperInAccount);
#else
							user->SetOperInAccount(0);
#endif
							if	(cuLogin->oper	>	eAL_SYSTEM_OPERATOR)
							{
								FILE * fp = fopen("c:\\OPER_ERROR.txt","w+");

								if	(fp)
								{
									fprintf(fp,"[%d %d] OPER = %d ,[%s][%s]",cSRVUTIL::GetTime(),cSRVUTIL::GetTimes(), cuLogin->oper,user->GetID(),user->GetName());
									fclose(fp);
								}
							}
							//	일단 게임 참여는 뒤로 미루고.. 맵 데이타부터 전송해준다.
	//						_log("일단 게임 참여는 뒤로 미루고.. 맵 데이타부터 전송해준다. [%s]",user->GetID());
							sConnect.wResult	=	dRESULT_CONNECT_SUCCESS;
	//						_log("[%s] -> [%s]",user->GetID(),lpField->m_strName);
	//						if (!lpField)	lpField	=	g_game.getFieldByUniqueSerial(0);
							if	(lpField->m_wIsGuildHall	&&	lpField->m_bIs1LevelGuildHall	==	FALSE)
							{
								strcpy(sConnect.strMapName,lpField->m_strFileName);

								sConnect.base.wSize	=	sizeof(sConnect);

								CGuild	*lpGuild	=	g_guildManager.getBySerial(lpField->m_wHallOwnGuild);

								if	(lpGuild)
									strcpy(sConnect.strGuildName,lpGuild->m_strName);
								else
									strcpy(sConnect.strGuildName,"???");
							}
							else
								strcpy(sConnect.strMapName, lpField->m_strFileName);
						}
						else
						{
							sConnect.wResult = dRESULT_CONNECT_FAIL;
							strcpy(sConnect.strMapName,"");

							LDMSG_DELETELIST	delList;
							delList.base.set(sizeof(LDMSG_DELETELIST),dLDMSG_DELETELIST);
							strcpy(delList.strId,user->GetID());

							g_socketDBC.SendPacket((char *)&delList,delList.base.wSize);

							printf("Connect Failed by Field\n");
						}
					}
					user->loginTime = timeGetTime() - user->loginTime;
					user->AddSendPacket((char *)&sConnect,sConnect.base.wSize);
					ProcessSendPacket(user->GetSerial());
					break;
				}

				case dWORLDtoSERVERS_DIS:
				{
					WORLDtoSERVERS_DIS	*	dis;
					dis = (WORLDtoSERVERS_DIS *)recvpacket;

					CClient	* user = CM.GetClientPntByIgnoreUsedFlagById(dis->strId);

					if	(!user || user->wSerial == 0xffff)
					{
						if	(STRICMP(dis->strCause,"Invalid user")	!=	0)
							DisUser(dis->strId,"EMPTY");
//							g_game.sendWPUserLogoutById(dis->strId,"EMPTY");
						break;
					}

					int		iSerial	= user->GetSerial();
					{//	유저한테 짤렸다고 메시지 보내준다.
						SG_OUT	out;
						out.base.set(sizeof(SG_OUT),dSG_OUT);
						strcpy(out.strCause,dis->strCause);
						user->AddSendPacket((char *)&out,out.base.wSize);
						ProcessSendPacket(user->GetSerial());
					}
					printf("Disconnect By World [%s]\n",dis->strId);

					cACTOR	*lpPlayer	=	g_game.getActor(user->GetActorSerialInServer());

					if	(lpPlayer)
						lpPlayer->immediatelySendSaveDataToDBMemory();

					DisUser(iSerial,"Disconnect By World ");
					break;
				}

				case dWORLDtoSERVERS_GET_AVADATA:
				{
					WORLDtoSERVERS_GET_AVADATA	*	aData;
					aData = (WORLDtoSERVERS_GET_AVADATA *)recvpacket;

					CPlayerSaveData		*lpSaveData		=	(CPlayerSaveData *)&aData->aInfo;
					CUpkeepPlayerData	*lpUpkeepData	=	&aData->upkeepPlayerData;
					cActorStatusUpkeep	*lpUpkeepStatus	=	&aData->upkeepStatus;
					BOOL				bIsFirstConnect	=	FALSE;
				//	CLOG("MoveMap","WORLDtoSERVERS_GET_AVADATA - [%s]serial[%d] field[%d] type[%d] job[%d] level[%d] GVGData[%d %d %d]",lpSaveData->m_strName,aData->serial,lpSaveData->m_wCurrentField,aData->type,lpSaveData->m_wJob,lpSaveData->m_iLevel,lpSaveData->m_bf6LastTransAvatarToGVGYear,lpSaveData->m_bf4LastTransAvatarToGVGMonth,lpSaveData->m_bf5LastTransAvatarToGVGDay);

					if	(aData->type	==	1)	//	처음 접속하는거다.
					{
						bIsFirstConnect	=	TRUE;
						lpUpkeepData->reset();
						lpUpkeepStatus->reset();
					}

					if	(lpSaveData->m_wJob	>= dPLAYER_JOB_COUNT)
					{
						CLOG("incorrectJob","WorldCtlThread - [%s/%s/%d][%d/%d] %d Gold직업이 이상하다.",lpSaveData->m_strId,lpSaveData->m_strName,lpSaveData->m_wJob,
								lpSaveData->m_aEquip[dEQUIP_WEAPON].m_wBaseItem,lpSaveData->m_aEquip[dEQUIP_WEAPON_2].m_wBaseItem,
								lpSaveData->m_iGold);
					}

					if	(lpSaveData->m_iLevel	<=	0)
					{
						CClient	*lpClient	=	CM.mGETCLIENT(aData->serial);
						if(!lpClient){
							_log("1. ERROR dWORLDtoSERVERS_GET_AVADATA : user==NULL");
							break;
						}
						_log("[%d]ERROR dWORLDtoSERVERS_GET_AVADATA : [%s] Incorrect Data",cSRVUTIL::GetTimes(),lpClient->GetID());
						SG_RESULT_JOIN	sJoinResult;
						sJoinResult.base.set(sizeof(cMSG_BASE_TYPE)+2,dSG_RESULT_JOIN);
						sJoinResult.wResult	= dRESULT_JOIN_FIELD_FAILED;

						lpClient->AddSendPacket((char*)&sJoinResult,sJoinResult.base.wSize);
						ProcessSendPacket(aData->serial);
						break;
					}

					//CClient	*lpClient	=	CM.mGETCLIENT(aData->serial);
					CClient	*lpClient	=	CM.GetClientPnt((WORD)aData->serial);

					if	(!lpClient)
					{
						_log("4. ERROR dWORLDtoSERVERS_GET_AVADATA : user==NULL");
						break;
					}
					if	(lpClient->GetSerial()!=aData->serial)
					{
						_log("5. ERROR dWORLDtoSERVERS_GET_AVADATA : [%s] serial -> %d != %d",aData->aInfo.m_strId,aData->serial,lpClient->GetSerial());
						DisUser(lpClient->GetSerial(),"Incorrect serial actor ");
						break;
					}

					lpClient->SetName(aData->aInfo.m_strName);

					if	(lpSaveData->m_wJob	>= dPLAYER_JOB_COUNT)
					{
						CLOG("incorrectJob","WorldCtlThread - [%s/%s/%d][%d/%d] %d Gold직업이 이상하다.",lpSaveData->m_strId,lpSaveData->m_strName,lpSaveData->m_wJob,
								lpSaveData->m_aEquip[dEQUIP_WEAPON].m_wBaseItem,lpSaveData->m_aEquip[dEQUIP_WEAPON_2].m_wBaseItem,
								lpSaveData->m_iGold);
					}

					if	(aData->type	==	0xffff)
					{
						_log("[%d]ERROR dWORLDtoSERVERS_GET_AVADATA : [%s]Failed to Join [aData->type == 0xffff]\n",cSRVUTIL::GetTimes(),lpClient->GetID());

						SG_RESULT_JOIN	sJoinResult;

						sJoinResult.base.set(sizeof(cMSG_BASE_TYPE)+2,dSG_RESULT_JOIN);

						sJoinResult.wResult	=	dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_CONNECT;

						lpClient->AddSendPacket((char*)&sJoinResult,sJoinResult.base.wSize);
						ProcessSendPacket(aData->serial);

						break;
					}

					int	iJoinResult	=	g_game.bookingJoinPlayer(lpSaveData,lpUpkeepData,lpUpkeepStatus,aData->serial,bIsFirstConnect);

				//	CLOG("MoveMap","WORLDtoSERVERS_GET_AVADATA - [%s]serial[%d]joinResult[%d]",lpSaveData->m_strName,aData->serial,iJoinResult);


					if	(iJoinResult	!=	dRESULT_JOIN_FIELD_SUCCESS)
					{
						_log("[%d]ERROR dWORLDtoSERVERS_GET_AVADATA : [%s]Failed to Join [%d]\n",cSRVUTIL::GetTimes(),lpClient->GetID(),iJoinResult);

						SG_FAILED_TO_JOIN	sJoinResult;

						sJoinResult.base.set(sizeof(SG_FAILED_TO_JOIN)-sizeof(sJoinResult.strText),dSG_FAILED_TO_JOIN);

						sJoinResult.wResult	=	iJoinResult;

						if	(iJoinResult	==	dRESULT_JOIN_FIELD_FAILED_BY_PROCESS_OFFLINE_TOURNAMENT)
						{
							strcpy(sJoinResult.strText,g_duelTrialGameManager.m_strOfflineTournamentComment);
							sJoinResult.base.wSize	+=	(strlen(sJoinResult.strText)+1);
						}

						lpClient->AddSendPacket((char*)&sJoinResult,sJoinResult.base.wSize);
						ProcessSendPacket(aData->serial);
					}
					else
					{
						SERVERStoWORLD_JOIN_COMPLETE	cJoinComplete;

						cJoinComplete.base.set(sizeof(SERVERStoWORLD_JOIN_COMPLETE),dSERVERStoWORLD_JOIN_COMPLETE);
						strcpy(cJoinComplete.strId,lpClient->GetID());
						g_socketWorld.SendPacket((char *)&cJoinComplete,cJoinComplete.base.wSize);
					}
					break;
				}
			case	dWORLDtoSERVERS_SYNC_CHANGEDNAME:
				{
					WORLDtoSERVERS_SYNC_CHANGEDNAME	*	rPacket = (WORLDtoSERVERS_SYNC_CHANGEDNAME *)recvpacket;
					if(rPacket->guildSerial != 0xffff)
					{
						CGuild * pGuild = g_guildManager.getBySerial(rPacket->guildSerial);
						if(pGuild)
						{
							CGuildMan * pMan = pGuild->getMember(rPacket->oldName);
							if(pMan)
								strcpy(pMan->m_strName,rPacket->newName);
						}
					}
				}
				break;

//	월드 패킷 isActiveGameServer() 예외처리
				case	dWORLDtoSERVERS_LOGIN					:
				case	dWORLDtoSERVERS_USER_LISTGUILD			:
				case	dWORLDtoSERVERS_USER_GUILDINFO			:
				case	dWORLDtoSERVERS_GUILD_BATTLE_SCHEDULE	:
				case	dWORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT	:
				case	dWORLDtoSERVERS_RENEW_PREMIUM_ITEM		:
				case	dWORLDtoSERVERS_ITEM_PACK_INFO			:
				case	dWORLDtoSERVERS_ITEM_PACK_LIST_SERIAL	:
				case	dWORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE	:
				case	dWORLDtoSERVERS_GR_INFO					:
				case	dWORLDtoSERVERS_CARROT_SHOP_INFO		:
				case	dWORLDtoSERVERS_UPDATE_GUILD_HALL_INFO	:
				case	dWORLDtoSERVERS_DUEL_RANKER_INFO		:
				case	dWORLDtoSERVERS_DUEL_TEAM				:
				case	dWORLDtoSERVERS_UPDATE_WORD_COMPARE		:
				case	dWORLDtoSERVERS_UPDATE_WORLD_VALUE_INFO_ALL	:
				case	dWORLDtoSERVERS_LOTTO_EVENT_DATA		:
				case	dWORLDtoSERVERS_EVENT_TIME_DATA			:
					if	(!g_bGameClose)
						g_game.addWorldPacket(recvpacket);
					break;

				case	dWORLDtoSERVERS_BANISH_USER				:
				{
					WORLDtoSERVERS_BANISH_USER * rPacket = (WORLDtoSERVERS_BANISH_USER *)recvpacket;

					CClient * pUser = CM.GetClientPnt(rPacket->strName,1);

					if	(!pUser)
						break;

					SG_NOTICE	sPacket;
					sPacket.base.set(sizeof(SG_NOTICE),dSG_NOTICE);
					strcpy(sPacket.strMessage,rPacket->strReason);
					pUser->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
					Sleep(1);

					DisUser(pUser->GetSerial(),"BANISHED USER..");
				}
				break;

				default:
				{
					if	(g_game.isActiveGameServer())
						g_game.addWorldPacket(recvpacket);
					break;	
				}
			}
		}
		Sleep(1);
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_WORLD_CTRL);

	printf("END WorldCtlThread ...\n");
	return ;
}

void BCThread(LPVOID lpParameter)
{
	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);
	WORD	type=0,recvsize=0;

	GB_LOGIN sPacket;
	sPacket.base.set(sizeof(GB_LOGIN),dGB_LOGIN);
	strcpy(sPacket.strName, cConfigData::Inst().m_strGameName.c_str());

	g_socketBC.sPacketBuf->Reset();
	g_socketBC.rPacketBuf->Reset();
	int	iSendGameServerInfoCount = 0;

BCThread_LOOP:
	if	(!g_socketBC.SendPacket((char *)&sPacket,sPacket.base.wSize))
	{	
		if	(iSendGameServerInfoCount<10)
		{
			_log("iSendGameServerInfoCount -> %d",iSendGameServerInfoCount);
			++iSendGameServerInfoCount;
			Sleep(1);
			goto BCThread_LOOP;
		}
	}

	g_socketBC.ReallySend();

	if	(!g_socketBC.isLoopRecv)
	{
		_log(" Failed to start BC Thread .. because can not send first data.");
		g_socketBC.kill();
		return;
	}

	g_isHaveToResetBCInfo = TRUE;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_BC_PACKET);

	while(g_bThreadLoop)
	{
		Sleep(1);

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_BC_PACKET);

		if	(!g_socketBC.isLoopRecv)
		{
			_log("BC SOCKET is disconnected!");
			break;
		}

		while(g_socketBC.GetRecvPacket(recvpacket))
			g_game.addBCSPacket(recvpacket);
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_BC_PACKET);
}

void DBThread(LPVOID lpParameter)
{
	BOOL isUpdateTime = FALSE;

	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);
	WORD	type=0,recvsize=0;
	g_game.recoveryUrgentSave();	//	이전에 긴급 저장 되었던것들 복구

	l_bIsWaitForUrgentSaveDataUpdate	=	FALSE;

#ifdef _USE_PROVISIONRANK
	GDMSG_PROVISION_RANK_REQ	sPacket;
	sPacket.base.set(sizeof(GDMSG_PROVISION_RANK_REQ),dGDMSG_PROVISION_RANK_REQ);
	g_socketDBC.SendPacket((char *)&sPacket,sPacket.base.wSize);
	l_bIsRecvProvisionRanking = FALSE;
	_log(" Request the list of provision ranking");
#else
	l_bIsRecvProvisionRanking = TRUE;
#endif

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_AVATAR_DB);

	while(g_bThreadLoop)
	{
		int	loopcount = 0;

		if	(!g_socketDBC.isLoopRecv)
		{
			_log("DB connect error!");
			break;
		}

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_AVATAR_DB);

		while(g_socketDBC.GetRecvPacket(recvpacket))
		{
			recvsize = *(WORD *)recvpacket;
			type = *(WORD *)(recvpacket+2);

			if(type == dCM_COMPRESS)
			{
				CM_COMPRESS	comp;
				memcpy(&comp,recvpacket,recvsize);
				cSRVUTIL::Uncompress((unsigned char *)comp.data,(unsigned char *)recvpacket,comp.wSize - sizeof(WORD)*3,comp.wSourceSize);
				recvsize = *(WORD *)recvpacket;
				type = *(WORD *)(recvpacket+2);
			}

#ifdef _USE_PROVISIONRANK
			if	(!l_bIsRecvProvisionRanking && type == dDGMSG_PROVISION_RANK_RET)
			{
				_log(" Received provision ranking");
				l_bIsRecvProvisionRanking = TRUE;
			}
#endif
			g_game.addDBPacket(recvpacket);

#ifdef _USE_PROVISIONRANK
			if(cSRVUTIL::GetHour() == 7 && isUpdateTime == FALSE)
			{
				GDMSG_PROVISION_RANK_REQ	sPacket;
				sPacket.base.set(sizeof(GDMSG_PROVISION_RANK_REQ),dGDMSG_PROVISION_RANK_REQ);
				g_socketDBC.SendPacket((char *)&sPacket,sPacket.base.wSize);
				l_bIsRecvProvisionRanking = FALSE;
				_log(" Request the list of provision ranking");

				isUpdateTime = TRUE;
			}else if(cSRVUTIL::GetHour() != 7)
			{
				isUpdateTime = FALSE;
			}
#endif
		}
		Sleep(1);
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_AVATAR_DB);
}

//////////////////////////////////////////////
//	method for packet method
//////////////////////////////////////////////

void	DisUser(char * _strId, char *_lpstrLog)
{
	CClient	 *user  = CM.GetClientPntByIgnoreUsedFlagById(_strId);

	if(user)
		DisUser(user->wSerial, _lpstrLog);
}

void	DisUser(int serial,char *_lpstrLog)
{
	CCritical	CS(&csRemoveUser);

	CClient	 *user  = CM.GetClientPnt(serial);

	if	(!user)
	{
		if	(stricmp(_lpstrLog,"dwRead == 0")!=0)
			_log("Dis User USer == NULL, [%s]",_lpstrLog);

		return;	
	}

	if(user->isInGame() && !user->isReadyToExit())
	{
		int		iActorSerial=	user->GetActorSerialInServer();
		cACTOR* lpActor		=	g_game.getActor(iActorSerial);
	 
		if (lpActor && lpActor->m_iClientSerial ==	user->GetSerial())	//	정상이다.
		{
			lpActor->bookingReadyToExitGame();
			return;
		}else
		{
			if	(user->GetPartySerial()	!=	0xffff && user->GetMoveMap() == FALSE)
			{
				CLOG("party","leave party by disconnect [%s]",user->GetName());
				g_game.sendWPPartyWork(user->GetPartySerial(),ePW_LEAVE_PARTY,user->GetName());
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//	로그아웃 메세지를 던진다.
	if(!user->GetMoveMap())	//	이동이 아닌 경우는 월드에 종료 메세지를 보낸다.
		g_game.sendWPUserLogoutById(user->id, user->ip);
	//
	//////////////////////////////////////////////////////////////////////////


	user->SetStateFlag(CClient::eSTATE_LOGOUT);

	//////////////////////////////////////////////////////////////////////////
	//	더이상 받지 않도록..
	int ret = shutdown(user->sock,SD_RECEIVE);
	switch(ret)
	{
	case WSANOTINITIALISED:	_log("WSANOTINITIALISED : [%s]",user->GetID());	break;
	case WSAENETDOWN:		_log("WSAENETDOWN : [%s]",user->GetID());		break;
	case WSAEINVAL:			_log("WSAEINVAL : [%s]",user->GetID());			break;
	case WSAEINPROGRESS:	_log("WSAEINPROGRESS : [%s]",user->GetID());	break;
	case WSAENOTCONN:		_log("WSAENOTCONN : [%s]",user->GetID());		break;
	case WSAENOTSOCK:		_log("WSAENOTSOCK : [%s]",user->GetID());		break;
	}
	//
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	어드민 툴!!
	if	(STRICMP(user->GetID(),"#admin")==0)	
	{
		for(int i=0;i<dMAX_ADMIN_COUNT;i++)
		{
			if(serial==_Admin[i].iAdminSerial)
			{
				_Admin[i].isUsed = FALSE;
				_Admin[i].iAdminSerial = -1;
				_Admin[i].iMyIndex = i;
				_Admin[i].wVersion = 0;
				break;	
			}
		}
		int j;
		for(j=0;j<dMAX_ADMIN_COUNT;j++)
			if(_Admin[j].isUsed)	
				break;
		if(j==dMAX_ADMIN_COUNT)	isAdmin = FALSE;
		removeUser(serial);
		return;
	}
	//	어드민 툴!!
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	
	removeUser(serial);
	//
	//////////////////////////////////////////////////////////////////////////
}

void removeUser(int _serial)
{
	CCritical	CS(&csRemoveUser);

	CClient	 *user  = CM.GetClientPnt(_serial);
	if(!user)
	{
//		_log( "HYUN!!! : _serial[%d] removeUser Error \n", _serial);
		return;
	}
	--acceptUserCount[user->wAcceptIndex];
	
	if	(acceptUserCount[user->wAcceptIndex]<0)
	{
		_log("ERROR acceptUserCount[%d] =%d ",user->wAcceptIndex,acceptUserCount[user->wAcceptIndex]);
		acceptUserCount[user->wAcceptIndex] = 0;
	}
	CM.Del(_serial);
//	_log( "HYUN!!! : mCLIENTMANAGER.Del[%d] removeUser !!\n", _serial);
}

// void	UnsetUserData(CClient *user)
// {
// 	if(STRICMP(user->GetID(),"")!=0 && strncmp(user->GetID(),"#",1)!=0)
// 	{
// 		int		iActorSerial=	user->GetActorSerialInServer();
// 		cACTOR* lpActor		=	g_game.getActor(iActorSerial);
// 
// 		if (lpActor && lpActor->m_iClientSerial ==	user->GetSerial())	//	정상이다.
// 			lpActor->bookingReadyToExitGame();
// 		else
// 		{
// 			if	(user->GetPartySerial()	!=	0xffff && user->GetMoveMap() == FALSE)
// 			{
// 				CLOG("party","leave party by disconnect [%s]",user->GetName());
// 				g_game.sendWPPartyWork(user->GetPartySerial(),ePW_LEAVE_PARTY,user->GetName());
// 			}
// 
// 			if	(user->GetMoveMap()	==	FALSE)
// 				g_game.sendWPUserLogoutById(user->GetID(),user->ip);
// 
// 			mCLIENTMANAGER.Del(user->GetSerial());
// 		}
// 	}
// 	else
// 	{
// 		mCLIENTMANAGER.Del(user->GetSerial());
// 
// 		_log("Dis User : Incorrect ID [%s]",user->GetID());
// 	}
// }

//////////////////////////////////////////////
//	method for Command
//////////////////////////////////////////////
BOOL	ProcessCommand(char* _lpstrCommand)
{
	static	BOOL	bIsWaitQuitConfirm	=	FALSE;

	if	(STRICMP(_lpstrCommand,"q")==0)
	{
		printf(" Quit ? (y/n) ");

		bIsWaitQuitConfirm	=	TRUE;

		return	TRUE;
	}

	if	(STRICMP(_lpstrCommand,"egl")==0)	//	export gold log
	{
		int	i;

		char	strLog[512]	=	"";

		for (int i = 0;i<eGET_GOLD_TYPE_COUNT;i++)
			strcat(strLog,_ms("	%I64d",g_allGettingGold[i]));
		for (int i = 0;i<eSPENT_GOLD_TYPE_COUNT;i++)
			strcat(strLog,_ms("	%I64d",g_allSpentGold[i]));

		CLOG("goldIO",strLog);

		printf("\nExport gold log Complete.\n");

		return	TRUE;
	}

	if	(bIsWaitQuitConfirm && (STRICMP(_lpstrCommand,"y")==0 || STRICMP(_lpstrCommand,"Y")==0 ))
	{
		return FALSE;//	서버를 종료한다.
	}

	bIsWaitQuitConfirm	=	FALSE;

	if (STRICMP(_lpstrCommand,"ba")==0)	//	유저 모두 내보내기
	{
		g_game.bookingBanishAllUser();
		return	TRUE;
	}

	if (STRICMP(_lpstrCommand,"rn")==0)	//	reset test guild battle 길드 배틀을 초기화
	{
		g_game.m_bIsBookingRegenNPC	=	TRUE;
		return	TRUE;
	}

	if (STRICMP(_lpstrCommand,"reloadSkill")==0)	//	reset test guild battle 길드 배틀을 초기화
	{
		g_sm.loadData("data\\skill.dat");

		printf("* load skill data complete\n");
		return	TRUE;
	}
#ifdef	_USE_NPGL
	if (g_bIsUseNPGL)
		if (STRICMP(_lpstrCommand,"stopGG")==0)	//	reset test guild battle 길드 배틀을 초기화
		{
			g_bIsDisconnectGGAuthFailedUser	=	1-g_bIsDisconnectGGAuthFailedUser;

			if (g_bIsDisconnectGGAuthFailedUser)
				printf("disconnect user by game guard\n");
			else
				printf("do not disconnect user by game guard\n");

			return	TRUE;
		}
#endif
	if(STRICMP(_lpstrCommand,"gai")==0)	//	getAcceptInfo
	{
		_log("==================================");
		_log("isDataReset = %d",isDataReset);
		_log("acceptCount = %d",acceptCount);
		for(int i = 0;i<acceptCount;i++)
		{
			_log("    lisSock[%d] = %d, acceptUserCount[%d] = %d",i,lisSock[i],i,acceptUserCount[i]);
		}
		_log("isReady = %d",isReady);
		_log("g_bGameClose = %d",g_bGameClose);

		return TRUE;
	}

	if (g_bIsTestServer)
	{
		if (STRICMP(_lpstrCommand,"skill")==0)	//	스킬 관련 정보 출력 시작
		{
			if	(g_iOutputStatus)
			{
				g_iOutputStatus	=	0;
				printf("스킬 정보 출력 종료\n");
			}
			else
			{
				g_iOutputStatus	=	1;
				printf("스킬 정보 출력 시작\n");
			}
			return	TRUE;
		}

		if	(STRICMP(_lpstrCommand,"trigger")==0)	//	reset test guild battle 길드 배틀을 초기화
		{
			g_bIsWantPrintTriggerCondition	=	1-g_bIsWantPrintTriggerCondition;
			if (g_bIsWantPrintTriggerCondition)
				printf("카르마 관련 정보 출력 시작\n");
			else
				printf("카르마 관련 정보 출력 종료\n");
			return	TRUE;
		}

		if	(_lpstrCommand[0] == 't' && strstr(_lpstrCommand,"trigger"))	//	reset test guild battle 길드 배틀을 초기화
		{
			int		iField		=	atoi(_lpstrCommand+strlen("trigger"));

			cFIELD	*lpField	=	g_game.getFieldByUniqueSerial(iField);

			if	(lpField)
			{
				lpField->m_bIsWantPrintTimeKarmaInfo	=	1-lpField->m_bIsWantPrintTimeKarmaInfo;

				if	(lpField->m_bIsWantPrintTimeKarmaInfo)
					printf("필드 %d번 카르마 관련 정보 출력 시작\n",iField);
				else
					printf("필드 %d번 카르마 관련 정보 출력 종료\n",iField);
			}

			return	TRUE;
		}

		if (STRICMP(_lpstrCommand,"hide")==0)	//	reset test guild battle 길드 배틀을 초기화
		{
			g_bIsDisplayTick	=	1-g_bIsDisplayTick;
			return	TRUE;
		}
	}

	if (STRICMP(_lpstrCommand,"readEvent")==0)
	{
		g_eventManager.readEventData();

		return	TRUE;
	}
	
// 	if (STRICMP(_lpstrCommand,"cgu")==0)	//	check garbage User
// 	{
// 		int cguCount = 0;
// 		for(int z = 0;z<dGAME_MAX_USER_COUNT;z++)
// 		{
// 			CClient * user = mCLIENTMANAGER.GetClientPnt(z);
// 			if(!user || user->GetIsClose())		continue;
// 			_log( "[%d] serial %d, [%s,%s] isMoveMap %d, field_index %d",z,user->wSerial,user->GetID(),user->GetName(),user->GetMoveMap(),user->GetFieldIndex());
// 			cguCount++;
// 		}
// 		_log("Client Size : %d",cguCount);
// 		return	TRUE;
// 	}

	if (STRICMP(_lpstrCommand,"h")==0)
	{
		printf("┌─────────────────────┐\n");
		printf("│h : 명령어 리스트 보기                    │\n");
		printf("│q : 서버 종료                             │\n");
		printf("│e : 프로파일 뽑기                         │\n");
		printf("│cgu : Chech Garbage User                  │\n");
		printf("│readEvent:                                │\n");
		printf("│gai: getAcceptInfo                        │\n");
		printf("└─────────────────────┘\n");
		printf("\n\n\n");
		return	TRUE;
	}

	if	(STRICMP(_lpstrCommand,"gi")==0)
	{
		cGoldRush	*lpGoldRush	=	&g_eventManager.m_goldRush;

		printf("gold rush info\n");
		printf("  begin time : %d/%d/%d %d:%d~%d/%d/%d %d:%d\n",lpGoldRush->m_begin.year,lpGoldRush->m_begin.month,lpGoldRush->m_begin.day,lpGoldRush->m_begin.hour,lpGoldRush->m_begin.minute,
															lpGoldRush->m_end.year,lpGoldRush->m_end.month,lpGoldRush->m_end.day,lpGoldRush->m_end.hour,lpGoldRush->m_end.minute);
		printf("  begin message : %s\n",lpGoldRush->m_strBeginComment);
		printf("  end message : %s\n",lpGoldRush->m_strEndComment);
		printf("  exp : %d%%\n",lpGoldRush->m_wBoostExperience);
		printf("  item drop rate : %d%%\n\n",lpGoldRush->m_wCorrectItemDropChance);

		return	TRUE;
	}
	
	if (STRICMP(_lpstrCommand,"u")==0)	//	긴급 저장
	{
		g_game.m_wIsReadyToUrgentSave	=	TRUE;
		return	TRUE;
	}

	if (STRICMP(_lpstrCommand,"t")==0)	//	내부 테스트 서버 모드로 전환
	{
		if	(g_bIsTestServer	==	FALSE)
			return	TRUE;

		g_bIsTestServer			=	FALSE;

		printf("\n\nrelease test server mode\n\n");

		return	TRUE;
	}

	if (STRICMP(_lpstrCommand,"e")==0)	//	여러가지 정보 익스 포트
	{
		g_game.m_bIsExportProfile	=	TRUE;

		return TRUE;
	}

	if (STRICMP(_lpstrCommand,"ed")==0)	//	여러가지 정보 익스 포트
	{
		cFILE	file;

		char	strFileName[512];

		sprintf(strFileName,"%s/exportDebugInfo.dat",g_strLogFolder);

		if	(file.Open(strFileName,"wb"))
		{
			file.Write(&g_debugSign,sizeof(g_debugSign));

			file.Close();
		}
			
		return TRUE;
	}

	if (STRICMP(_lpstrCommand,"resetFieldStatics")==0)	//	여러가지 정보 익스 포트
	{
		g_game.m_bIsResetFieldStatics	=	TRUE;

		return TRUE;
	}

	if	(STRICMP(_lpstrCommand,"resetProfile")==0)	//	여러가지 정보 익스 포트
	{
		g_game.m_bResetProfiler		=	TRUE;

		return TRUE;
	}
	if	(STRICMP(_lpstrCommand,"toggle_gc")==0)	//	길드 채팅 on
	{
		g_bIsEnableGuildChat	=	1-g_bIsEnableGuildChat;

		return TRUE;
	}
	
	if	(STRICMP(_lpstrCommand,"pvp_test")==0)	//	길드 채팅 on
	{
		g_bIsEnablePVPMagicDamageRule	=	1-g_bIsEnablePVPMagicDamageRule;

		if	(g_bIsEnablePVPMagicDamageRule)
			printf("PVP 마법 대미지 보정 적용 함\n");
		else
			printf("PVP 마법 대미지 보정 적용 안함\n");

		return TRUE;
	}

	if	(STRICMP(_lpstrCommand,"s")==0)	//	외곽에 위치한 캐릭터 정보 업데이트
	{
		g_game.m_bIsUpdateSightBorderActorInfo	=	1-g_game.m_bIsUpdateSightBorderActorInfo;

		return TRUE;
	}

	return TRUE;
}

int
SendPacketToBCS(char * _cData,WORD _wSize)
{
	return g_socketBC.SendPacket(_cData,_wSize);
}

BOOL
IsAvailBCConnect()
{
	return	FALSE;
#ifdef	_FOR_JAPAN
	return	FALSE;
#endif

	return	g_socketBC.isLoopRecv;
}
