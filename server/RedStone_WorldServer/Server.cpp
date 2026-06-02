// Server.cpp : Defines the entry point for the application.
//
/**********************************************************************************
WORLD 관리 서버 
**********************************************************************************/

#include <iostream>

#include "cSOCKET.h"
#include "Client.h"
#include "cGuildManager.h"
#include "ExScript.h"
#include "cScript.h"
#include "cSRVUTIL.h"
#include "cPACKET_WORLDSERVER.h"
#include "cPACKET_USERDBSERVER.h"
#include "cPACKET_BCSERVER.h"
#include "cPACKET_DBSERVER.h"
#include "Msjexhnd.h"
#include "CUtil.h"
#include "CEvent.h"
#include "CGuildBattleManager.h"
#include "party.h"
//#include "buildVersion.h"
#include "carrotShopDefine.h"
#include "operate_packet.h"
#include "guild.h"
#include "hardwareinfo.h"
#include "cDECODER.h"
#include "server.h"
#include "life_code.h"
#include "reg_common.h"
#include "inputConsoleCommand.h"
#include "playerJobCommon.h"
#include "crtdbg.h"
#include "../buildVersion.h"	//	이건 공유 안하고 쓴다.
							//	없으면 파일을 생성한다.(공유 하지 말것!!)
							//	안에는
							//	#pragma	once
							//	#define	dBUILDER_NAME	"young"
							//	와 같이 선언 되어 있다. 뒤에 이름은 프로그래머 이름을 적을것


#define	dLOGIN_SERVER_ID	"#login"
#define	dADMIN_TOOL_ID		"#admin"

BOOL	g_bIsDevServer	=	FALSE;

#define BUILDDATE_YY	8
#define BUILDDATE_MM	8
#define BUILDDATE_DD	6
#define BUILDDATE_HH	9
#define BUILDDATE_MIN	57
#define BUILDDATE_SS	46
#define BUILDVERSION	2000
#define dUSE_SERVERSTATUS_REPORT

#ifdef _FOR_THAI
#undef dUSE_SERVERSTATUS_REPORT
#endif

#ifdef dUSE_SERVERSTATUS_REPORT
	#include "cServerStatus.h"

#endif

#define _PACKET_OPTION_CRYPT		//	암호화해서 패킷을 날린다.
#define _PACKET_OPTION_SEND_AT_ONE	//	행동 데이타를 한번에 보낸다.
#define dDB_CONNECT_TIME			100000
#define dCONFIGFILENAME				"config-world.txt"
#define dCONFIGFILENAME_FOR_DEV		"config-world_dev.txt"

#define dMAX_WORLD_WORKER_COUNT		8

WORD	l_wWorldPort	=	dWORLDSERVER_PORT;
WORD	g_MaxUserCount;	//	MAXUSERCOUNT
int		g_iGameServerCount;//GAMESERVER_COUNT
char	g_MainConfig[512];
static int testSizeCount=0;
static int l_iCountGuildSaveGap = 0;
BOOL	g_bIsIgnoreSSInfo = FALSE;
BOOL	g_bIsInnerTestServer	= FALSE;
cFILE	l_fileSaveItemCount;

int		g_aiWorldValue[dMAX_WORLD_VALUE_MAX] = {0};

struct stAdmin
{
	BOOL	isUsed;
	int		iMyIndex;
	int		iAdminSerial;
	WORD	wVersion;
};

#define	dMAX_BUFFER		4096

//	DIS_USER 횟수 확인을 위한 카운터
static int				l_iCountForDisUser = 0;


static int				socketcount=0;
static unsigned int		l_uiItemCounter = 0;
static BOOL				isAdmin = FALSE;
stAdmin			_Admin[dMAX_ADMIN_COUNT];
static DWORD	g_DAY=0,g_dwUserCount=0,g_dwMaxTimeUserCount[24],g_dwMaxDayUserCount=0,g_dwMaxUserCount=0;
static DWORD	g_dwYahooUserCount = 0,g_dwMaxTimeYahooUserCount[24],g_dwMaxDayYahooUserCount = 0,g_dwMaxYahooUserCount = 0;
static DWORD	g_dwParanUserCount = 0,g_dwMaxTimeParanUserCount[24],g_dwMaxDayParanUserCount = 0,g_dwMaxParanUserCount = 0;
CRITICAL_SECTION csRemoveUser;
CRITICAL_SECTION csItemCount;
CRITICAL_SECTION csExitParty;
CRITICAL_SECTION csYahooCount;
CRITICAL_SECTION csParanCount;

CTimeInfo			g_currentTime;
stMainterenceTime	g_stMainterenceTime;

enum 
{
	eServerCheckType_NORMAL		= 0,	//	wServerCheckType
	eServerCheckType_STOPREADY	= 1,
	eServerCheckType_STOP		= 2,
};		

enum
{
	eIsReady_Nothing = 0,
		eIsReady_EventInfo,
		eIsReady_GuildInfo,
		eIsReady_Complete,
};
static WORD		wServerCheckType= 0;	//	일반적인 상태
static BOOL		isFirst			= TRUE;
static int		isReady			= eIsReady_Nothing;
static int		iRecvGuildCount = 0;		//	필요한 길드 정보를 모두 받으면 isReady = 3;
static BOOL		isMainLoop		= TRUE;
static BOOL		g_bThreadLoop	= TRUE;
static BOOL		g_bOffServer	= FALSE;
static char		strMainPath[1024];
char			strWorldName[dWORLD_NAME_LENGTH];
static char		l_strWorldIp[dIP_SIZE]	=	"";
static WORD		wServerListIndex;
static char		strAvatarDBIp[dIP_SIZE];
static DWORD	dwAvatarDBPort;
static char		strIDDBIp[dIP_SIZE];
static DWORD	dwIDDBPort;
static char		strScorePath[2048];
static int		g_iSeasonVariable;

int			g_iWorldType;
int			g_iWorldServerIndex;

static int		wWeekFixCheck,wHourFixCheck,wMinFixCheck,wWeekTestFixCheck;
static int		wWeekFixEndCheck,wHourFixEndCheck,wMinFixEndCheck;
WORD			l_wMessage	=	0;
static char		strMsgFixCheck[dCHAT_LENGTH];

HANDLE		g_hCompletionPort;		//	IOCP에 사용되는 포트
SOCKET		lisSock;		//	listen 소켓
HANDLE		hAcceptThread,hWorkerThread[dMAX_WORLD_WORKER_COUNT],hAdditionThread,hDBThread,hUSERDBThread,hCommandThread;
HANDLE		l_hLifeCodeThread	=	NULL;	//	생존 코드 쓰레드

int			nTimer;
CWWScript	configScp;
cScript		scp_score;
cSOCKET		l_avatarDBCSocket,l_userDBCSocket;

CClientManager & UserManager	=	CClientManager::GetInstance();
//cGuildManager  & GuildManager = cGuildManager::GetInstance();

BOOL	InitSocket();
BOOL	InitCompletionPort();		//	IOCP 초기화
void	AcceptLoop(LPVOID lpParameter); 
void	WorkerThread(LPVOID lpParameter); 
void	AdditionThread(LPVOID lpParameter); 
void	DBCtlThread(LPVOID lpParameter); 
void	USERDBCtlThread(LPVOID lpParameter); 
void	CommandThread(LPVOID lpParameter);
void	SendGameServerInitDataThread(LPVOID lpParameter);
void	LifeCodeUpdateThread(LPVOID lpParameter);

BOOL	RecvPacket(int serial);
BOOL	SendPacket(int serial);
void	SendPacket_Add(int serial);
void	ProcessRecvPacket(int serial);
void	ProcessDBDisconnect();

void	SaveData(CClient *user,SERVERStoWORLD_SAVEAVA	* sAva);
void	opeateWorldValue(int _iSortSet , WORD _wSerial , int _iValue);
void	LoadData(CClient *user,DGMSG_LOGINAVATAR * sLoginAva);
BOOL	DeleteUser(CClient *user, char	* _strId, char	* _strIp, BOOL	_isSkipMoveMap = 1);
//	결과값이 돌아왔을 경우 길드 정보가 월드에 남아 있는지 검사하고 
//	길드정보가 남아 있다면 해당 모든 길드원에게 길드 삭제의 정보를 보내준다.
// void	DeleteGuildPacket(DWORD _dwGameServerSerial,WORD _wGuildSerial,char * _strGuildName, char * _strRequestMan, WORD _wReason = eDGR_NORMAL);
// void	DeleteGuild(WORD _wGuildSerial, char * _strGuildName,char * _strWhy);
void	DeleteGuilder(WORD _guildSerial,char * _strId,char * _strName);
BOOL	GetGuildLevel(WORD *_lpiLevel,DWORD *_lpdwExp);	//	CUtil.h
// void	UpdateGuildData(cGuild * guild,DWORD	_dwSerial = 0xffffffff);
void	LoadGuildDataInFile(cGuild * _lpGuild);
void	SendToAllGameNCommunityServers(void * pData, int size);


void	SendGameServerInitData(WORD  _dwGameServerSerial);

//	method for packet send
void	DisUser(int serial,char * _why);
//	end define


static DWORD	dwhi,dwlo;

cSOCKET*
GetADBSocket()
{
	return	&l_avatarDBCSocket;
}

cSOCKET*
GetIDDBSocket()
{
	return	&l_userDBCSocket;
}

void
SendPacketToBCS(void *_lpPacket)
{
	cMSG_BASE_TYPE	*lpPacket	=	(cMSG_BASE_TYPE	*)_lpPacket;

	UserManager.SendToBCS((char *)lpPacket,lpPacket->wSize);
}

BOOL
SendGuildPacketToBCS(void *_lpPacket,int _iGuild)
{
	return	TRUE;
#ifdef	_FOR_JAPAN
	return	TRUE;
#endif
	GB_GUILD_WORK_BUS	packet;

	cMSG_BASE_TYPE	*lpPacket	=	(cMSG_BASE_TYPE	*)_lpPacket;

	if	(lpPacket->wSize	>	c_iBCS_TransPacketBufferSize)
		return	FALSE;

	packet.base.set(sizeof(packet),dWB_GUILD_WORK_BUS);
	packet.wGuildSerial	=	_iGuild;
	memcpy(packet.aBuffer,lpPacket,lpPacket->wSize);

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aBuffer)+lpPacket->wSize;

	UserManager.SendToBCS((char *)&packet,packet.base.wSize);

	return	TRUE;
}


Avatar_Data*
GetAvatar(char *_lpstrUserName)
{
	return	UserManager.GetGClientPnt(_lpstrUserName,1);
}

CClient*
GetClientServer(char *_lpstrUserName)
{
	WORD	iFieldSerial	=	UserManager.GetClientOfGClientSerial(_lpstrUserName,1);

	return	UserManager.GetClientPnt(iFieldSerial);
}

inline	void	increaseItemCounter(DWORD *_lpCounter)
{
	EnterCriticalSection(&csItemCount);
	l_uiItemCounter++;
	
	//	0은 기본 아이템 (예전)
	//	1은 시리얼을 요구하는 녀석
	
	if	(l_uiItemCounter == 0			)
		l_uiItemCounter	=	1;
	if	(l_uiItemCounter >= 0xffffffff )
		l_uiItemCounter	=	1;

	*_lpCounter		=	l_uiItemCounter;

	l_fileSaveItemCount.Rewind();
	l_fileSaveItemCount.Write(&l_uiItemCounter,4);

	LeaveCriticalSection(&csItemCount);
}

void
StartClockCheck()
{
	__asm
	{
		rdtsc			; 클럭카운트를 edx:eax레지스터에 저장
			mov	dwhi,edx	;상위 dword 메모리에 백업
			mov	dwlo,eax	;하위 dword 메모리에 백업	
	}
	
}

void
EndClockCheck()
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
	printf( "[%u][%u] (clock count) \n",dwhi,dwlo);		// 결과 출력
	
}
//////////////////////////////////////
//	기타 기능을 위한 플래그
BOOL	isSend_UserCount	=	FALSE;

void
OperateSpecialWork()
{
	if	(g_worldUtil.isSendAccountInfoTime())
	{
		WORLDtoSERVERS_ACCOUNT_INFO	packet;
		
		packet.base.set(sizeof(packet),dWORLDtoSERVERS_ACCOUNT_INFO);
		memcpy(packet.aiPlayerCount,g_worldUtil.m_aPlayerCount,sizeof(packet.aiPlayerCount));
		
		CClient	*lpServer	=	NULL;
		
		mSEND_GSP(lpServer,packet);
	}
}

int		readDigit(FILE *f)
{
	char	_readline[20];
	int		tmp;
	int		count=0;
	memset(_readline,0,2);
	
	tmp = fgetc(f);
	while(tmp!=EOF && tmp!=10)
	{
		_readline[count++] = (char)tmp;
		tmp = fgetc(f);
	}
	_readline[count]=0;
	return atoi(_readline);
}


//////////////////////////////////////
//	for Profile
DebugTracer		* cLOG;

void
reloadTournamentInfo(BOOL _bIsReload)
{
	if	(_bIsReload)
		if	(!configScp.Read(g_MainConfig))
			return;

	CWWScript::unWW_DataType	stValue;

	if	(configScp.GetData(CWWScript::eWWS_DT_STRING,&stValue,"OFFLINE_TOURNAMENT_COMMENT"))
		strcpy(g_eventManager.m_strOfflineTournamentComment,stValue.dtString);

	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"OFFLINE_TOURNAMENT_BEGIN"))
		g_eventManager.m_timeOfflineTournamentBegin.m_wYear		=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"OFFLINE_TOURNAMENT_BEGIN",2))
		g_eventManager.m_timeOfflineTournamentBegin.m_wMonth	=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"OFFLINE_TOURNAMENT_BEGIN",3))
		g_eventManager.m_timeOfflineTournamentBegin.m_wDay		=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"OFFLINE_TOURNAMENT_BEGIN",4))
		g_eventManager.m_timeOfflineTournamentBegin.m_wHour		=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"OFFLINE_TOURNAMENT_BEGIN",5))
		g_eventManager.m_timeOfflineTournamentBegin.m_wMinute	=	stValue.dtInt;

	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"OFFLINE_TOURNAMENT_END"))
		g_eventManager.m_timeOfflineTournamentEnd.m_wYear		=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"OFFLINE_TOURNAMENT_END",2))
		g_eventManager.m_timeOfflineTournamentEnd.m_wMonth		=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"OFFLINE_TOURNAMENT_END",3))
		g_eventManager.m_timeOfflineTournamentEnd.m_wDay		=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"OFFLINE_TOURNAMENT_END",4))
		g_eventManager.m_timeOfflineTournamentEnd.m_wHour		=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"OFFLINE_TOURNAMENT_END",5))
		g_eventManager.m_timeOfflineTournamentEnd.m_wMinute		=	stValue.dtInt;

	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"TRIAL_GAME_BEGIN"))
		g_eventManager.m_timeTrialGameBegin.m_wYear		=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"TRIAL_GAME_BEGIN",2))
		g_eventManager.m_timeTrialGameBegin.m_wMonth	=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"TRIAL_GAME_BEGIN",3))
		g_eventManager.m_timeTrialGameBegin.m_wDay		=	stValue.dtInt;

	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"TRIAL_GAME_END"))
		g_eventManager.m_timeTrialGameEnd.m_wYear		=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"TRIAL_GAME_END",2))
		g_eventManager.m_timeTrialGameEnd.m_wMonth		=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"TRIAL_GAME_END",3))
		g_eventManager.m_timeTrialGameEnd.m_wDay		=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_STRING,&stValue,"TRIAL_GAME_NAME"))
		strcpy(g_eventManager.m_strTrialGameName,stValue.dtString);

	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"TRIAL_GAME_TEAM_SIZE"))
		g_eventManager.m_iTrialGameTeamSize				=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"IS_SEND_TRIAL_GAME_RESULT_MAIL"))
		g_eventManager.m_iIsSendTrialGameResultMail		=	stValue.dtInt;

	g_eventManager.m_timeTrialGameBegin.m_wHour		=	0;
	g_eventManager.m_timeTrialGameBegin.m_wMinute	=	0;
	g_eventManager.m_timeTrialGameEnd.m_wHour		=	24;
	g_eventManager.m_timeTrialGameEnd.m_wMinute		=	0;

	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"TRIAL_GAME_PLAY_TIME"))
		g_eventManager.m_timeTrialGameBegin.m_wHour	=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"TRIAL_GAME_PLAY_TIME",2))
		g_eventManager.m_timeTrialGameBegin.m_wMinute=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"TRIAL_GAME_PLAY_TIME",3))
		g_eventManager.m_timeTrialGameEnd.m_wHour	=	stValue.dtInt;
	if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"TRIAL_GAME_PLAY_TIME",4))
		g_eventManager.m_timeTrialGameEnd.m_wMinute	=	stValue.dtInt;

	//if	(configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"IGNORE_SS_INFO"))	
		//g_bIsIgnoreSSInfo	=	stValue.dtInt;
	

	CTimeInfo	*lpBT_Time	=	&g_eventManager.m_timeOfflineTournamentBegin;
	CTimeInfo	*lpET_Time	=	&g_eventManager.m_timeOfflineTournamentEnd;

	printf("\noffline tournament comment\n  %s\n",g_eventManager.m_strOfflineTournamentComment);
	printf("offline tournament %d/%.2d/%.2d %.2d:%.2d ~ %d/%.2d/%.2d %.2d:%.2d\n",
			lpBT_Time->m_wYear,lpBT_Time->m_wMonth,lpBT_Time->m_wDay,lpBT_Time->m_wHour,lpBT_Time->m_wMinute,
			lpET_Time->m_wYear,lpET_Time->m_wMonth,lpET_Time->m_wDay,lpET_Time->m_wHour,lpET_Time->m_wMinute);

	CTimeInfo	*lpTGBT_Time=	&g_eventManager.m_timeTrialGameBegin;
	CTimeInfo	*lpTGET_Time=	&g_eventManager.m_timeTrialGameEnd;

	printf("\ntrial game name : %s\n",g_eventManager.m_strTrialGameName,
			lpTGBT_Time->m_wYear,lpTGBT_Time->m_wMonth,lpTGBT_Time->m_wDay,
			lpTGET_Time->m_wYear,lpTGET_Time->m_wMonth,lpTGET_Time->m_wDay);

	printf("trial game period : %d/%.2d/%.2d~%d/%.2d/%.2d\n",
			lpTGBT_Time->m_wYear,lpTGBT_Time->m_wMonth,lpTGBT_Time->m_wDay,
			lpTGET_Time->m_wYear,lpTGET_Time->m_wMonth,lpTGET_Time->m_wDay);

	printf("trial game play time : %.2d:%.2d ~ %.2d:%.2d\n",
			lpTGBT_Time->m_wHour,lpTGBT_Time->m_wMinute,
			lpTGET_Time->m_wHour,lpTGET_Time->m_wMinute);

	if	(!_bIsReload)
		return;

	WORLDtoSERVERS_DUEL_MATCH_INFO	packet;

	packet.wTrialGameBeginYear				=	g_eventManager.m_timeTrialGameBegin.m_wYear;
	packet.wTrialGameBeginMonth				=	g_eventManager.m_timeTrialGameBegin.m_wMonth;
	packet.wTrialGameBeginDay				=	g_eventManager.m_timeTrialGameBegin.m_wDay;
	packet.wTrialGameBeginHour				=	g_eventManager.m_timeTrialGameBegin.m_wHour;
	packet.wTrialGameBeginMinute			=	g_eventManager.m_timeTrialGameBegin.m_wMinute;
	packet.wTrialGameEndYear				=	g_eventManager.m_timeTrialGameEnd.m_wYear;
	packet.wTrialGameEndMonth				=	g_eventManager.m_timeTrialGameEnd.m_wMonth;
	packet.wTrialGameEndDay					=	g_eventManager.m_timeTrialGameEnd.m_wDay;
	packet.wTrialGameEndHour				=	g_eventManager.m_timeTrialGameEnd.m_wHour;
	packet.wTrialGameEndMinute				=	g_eventManager.m_timeTrialGameEnd.m_wMinute;
	packet.wIsSendTrialGameResultMail		=	g_eventManager.m_iIsSendTrialGameResultMail;
	packet.wTrialGameTeamSize				=	g_eventManager.m_iTrialGameTeamSize;

	packet.wOfflineTournamentBeginYear		=	g_eventManager.m_timeOfflineTournamentBegin.m_wYear;
	packet.wOfflineTournamentBeginMonth		=	g_eventManager.m_timeOfflineTournamentBegin.m_wMonth;
	packet.wOfflineTournamentBeginDay		=	g_eventManager.m_timeOfflineTournamentBegin.m_wDay;
	packet.wOfflineTournamentBeginHour		=	g_eventManager.m_timeOfflineTournamentBegin.m_wHour;
	packet.wOfflineTournamentBeginMinute	=	g_eventManager.m_timeOfflineTournamentBegin.m_wMinute;

	packet.wOfflineTournamentEndYear		=	g_eventManager.m_timeOfflineTournamentEnd.m_wYear;
	packet.wOfflineTournamentEndMonth		=	g_eventManager.m_timeOfflineTournamentEnd.m_wMonth;
	packet.wOfflineTournamentEndDay			=	g_eventManager.m_timeOfflineTournamentEnd.m_wDay;
	packet.wOfflineTournamentEndHour		=	g_eventManager.m_timeOfflineTournamentEnd.m_wHour;
	packet.wOfflineTournamentEndMinute		=	g_eventManager.m_timeOfflineTournamentEnd.m_wMinute;


	strcpy(packet.strTrialGameName,g_eventManager.m_strTrialGameName);
	strcpy(packet.strOfflineTournamentComment,g_eventManager.m_strOfflineTournamentComment);

	packet.base.set(sizeof(WORLDtoSERVERS_DUEL_MATCH_INFO),dWORLDtoSERVERS_DUEL_MATCH_INFO);

	SendDataAllGameServer((char *)&packet,packet.base.wSize);
}

void main()
{
	const	int	c_iCheckLeakBlock	=	-1;

	if	(c_iCheckLeakBlock	!=	-1)
	{
		cMSG::Put("체크!!","메모리 릭 체크 변수가 설정되어 있습니다");
//		assert(0 && "메모리 릭 체크 변수가 세팅되어 있습니다!!");

		_CrtSetBreakAlloc( c_iCheckLeakBlock );
	}

  	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 
	
	{
		char	strTitle[512];

#ifdef	_FOR_KOREA
		sprintf(strTitle,"RS World server for korea. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_JAPAN
		sprintf(strTitle,"RS World server for japan. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_THAI
		sprintf(strTitle,"RS World server for thai. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_USA
		sprintf(strTitle,"RS World server for USA. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#else
		sprintf(strTitle,"RS World server. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#endif

		SetConsoleTitle(strTitle);
	}

	InitLifeCode();


	g_guildBattleManager.init();
	g_eventManager.init();
	
	CreateDirectory("data",NULL);
	SetCurrentDirectory("data");
	CreateDirectory("playerCountLog",NULL);
	CreateDirectory("guild",NULL);
	SetCurrentDirectory("..");

	{
		if	(l_fileSaveItemCount.Open("data/itemCount.dat","rb"))
		{
			l_fileSaveItemCount.Read(&l_uiItemCounter,4);
			l_fileSaveItemCount.Close();
		}
		else
		{
			FILE *itemFP = fopen("itemcount.txt","r");

			if	(itemFP)
			{
				char tmptxt[10];

				fread(tmptxt,sizeof(tmptxt),1,itemFP);

				l_uiItemCounter = atoi(tmptxt);

				fclose(itemFP);
			}
		}

		l_fileSaveItemCount.Open("data/itemCount.dat","wb",TRUE);
	}
	//	아이템 카운트 저장한 파일
	
	GetCurrentDirectory(1024,strMainPath);
	g_DAY = cSRVUTIL::GetTime();

	sprintf(g_MainConfig,"%s\\%s",strMainPath,dCONFIGFILENAME_FOR_DEV);

	if	(!configScp.Read(g_MainConfig))
	{
		sprintf(g_MainConfig,"%s\\%s",strMainPath,dCONFIGFILENAME);

		if	(!configScp.Read(g_MainConfig))
		{
			cMSG::Error("error!!","can not find config-world.txt file");
			return;
		}
	}
	else
		cMSG::Put("check!!","using config-world_dev.txt file");

	{
		CWWScript::unWW_DataType	stValue;
		configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"GAMESERVER_COUNT");
		g_iGameServerCount=	stValue.dtInt;
		configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"MAXUSERCOUNT");
		g_MaxUserCount	=	stValue.dtInt;
		configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"WEEKFIXCHECK");
		wWeekFixCheck	=	wWeekFixEndCheck = stValue.dtInt;
		configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"TESTWEEKFIXCHECK");
		wWeekTestFixCheck=	stValue.dtInt;
		configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"HOURFIXCHECK");
		wHourFixCheck	=	stValue.dtInt;
		configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"MINFIXCHECK");
		wMinFixCheck	=	wMinFixEndCheck = stValue.dtInt;
#ifdef _FOR_JAPAN
		configScp.GetData(CWWScript::eWWS_DT_USHORT,&stValue,"message");
		l_wMessage		=	stValue.dtUShort;
#endif

		reloadTournamentInfo(FALSE);

		//	EXCEPT_GUILDBATTLETIME
		configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"EXCEPT_GUILDBATTLETIME");
		g_stMainterenceTime.bf12MTWeekday = stValue.dtInt;
		configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"EXCEPT_GUILDBATTLETIME",2);
		g_stMainterenceTime.bf4MTStartHour = stValue.dtInt;
		configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"EXCEPT_GUILDBATTLETIME",3);
		g_stMainterenceTime.bf6MTStartMin = stValue.dtInt;
		configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"EXCEPT_GUILDBATTLETIME",4);
		g_stMainterenceTime.bf4MTEndHour = stValue.dtInt;
		configScp.GetData(CWWScript::eWWS_DT_INT,&stValue,"EXCEPT_GUILDBATTLETIME",5);
		g_stMainterenceTime.bf6MTEndMin = stValue.dtInt;
		wHourFixEndCheck=	cSRVUTIL::CalcHour(wHourFixCheck,1,1,&wWeekFixEndCheck);
		printf("l_wMessage %d\n",l_wMessage);

		if	(configScp.GetData(CWWScript::eWWS_DT_STRING,&stValue,"MSGFIXCHECK"))
			strcpy(strMsgFixCheck,stValue.dtString);
		else
			strMsgFixCheck[0] = 0;
	}
	sprintf(strScorePath,"%s\\Score",strMainPath);
	CreateDirectory(strScorePath,NULL);
	
	memset(g_dwMaxTimeUserCount,0,sizeof(DWORD)*24);
	memset(g_dwMaxTimeYahooUserCount,0,sizeof(DWORD)*24);
	char filename[2048];
	sprintf(filename,"%s\\%d_Score.txt",strScorePath,cSRVUTIL::GetTime());
	FILE *fpScore = fopen(filename,"r");
	if	(fpScore)
	{
		fclose(fpScore);
	}
	else
	{
		fpScore = fopen(filename,"w");
		if	(fpScore)
		{
#ifdef _FOR_KOREA
			fprintf(fpScore,"MAXUSER = 0 | 0 | 0 | 0 | 0 | 0;\n");
#else
			fprintf(fpScore,"MAXUSER = 0 | 0 ;\n");
#endif
			for	(int t=0;t<24;t++)
			{
#ifdef _FOR_KOREA
				fprintf(fpScore,"T%d = 0 | 0 | 0;\n",t);
#else
				fprintf(fpScore,"T%d = 0;\n",t);
#endif
			}
			fclose(fpScore);
		}
	}
	if(scp_score.Read(filename)==dNOTFILE)
	{
		g_dwMaxUserCount = 0;
		g_dwMaxDayUserCount = 0;
		g_dwMaxDayYahooUserCount = 0;
		g_dwMaxYahooUserCount = 0;
		g_dwMaxDayParanUserCount = 0;
		g_dwMaxParanUserCount = 0;
	}else
	{
		g_dwMaxUserCount = atoi(scp_score.getData("MAXUSER",1));
		g_dwMaxDayUserCount = atoi(scp_score.getData("MAXUSER",2));
#ifdef _FOR_KOREA
		g_dwMaxYahooUserCount = atoi(scp_score.getData("MAXUSER",3));
		g_dwMaxDayYahooUserCount = atoi(scp_score.getData("MAXUSER",4));
		g_dwMaxParanUserCount = atoi(scp_score.getData("MAXUSER",5));
		g_dwMaxDayParanUserCount = atoi(scp_score.getData("MAXUSER",6));
#endif
		printf("%d/ %d\n",g_dwMaxUserCount,g_dwMaxDayUserCount);
		char	strHour[4];
		for(int h=0;h<24;h++)
		{
			sprintf(strHour,"T%d",h);
			g_dwMaxTimeUserCount[h] = atoi(scp_score.getData(strHour));
			g_dwMaxTimeYahooUserCount[h] = atoi(scp_score.getData(strHour,2));
			g_dwMaxTimeParanUserCount[h] = atoi(scp_score.getData(strHour,3));
		}
	}
	
	hAcceptThread = NULL;
	hAdditionThread = NULL;
	hDBThread = NULL;
	hUSERDBThread = NULL;
	int threadcouant=0;
	for(threadcouant=0;threadcouant<dMAX_WORLD_WORKER_COUNT;threadcouant++)
		hWorkerThread[threadcouant] = NULL;
	
	//	윈도우 끝나면 나머지 초기화
	//	스택 할당하고...
	InitializeCriticalSection(&csRemoveUser);
	InitializeCriticalSection(&csItemCount);
	InitializeCriticalSection(&csExitParty);
	InitializeCriticalSection(&csYahooCount);
	InitializeCriticalSection(&csParanCount);
	{
		CWWScript::unWW_DataType	stValue;
		configScp.GetData(CWWScript::eWWS_DT_STRING,&stValue,"WORLDNAME");
		strcpy(strWorldName,stValue.dtString);

		WriteServerInfoToRegistry(eST_WORLD,strWorldName);

		//	밖으로 나가는 월드 서버 아이피를 고정시키고 싶은 경우
		if	(configScp.GetData(CWWScript::eWWS_DT_STRING,&stValue,"WORLD_SERVER_IP"))
			strcpy(l_strWorldIp,stValue.dtString);

		configScp.GetData(CWWScript::eWWS_DT_STRING,&stValue,"DBCACHEIP");
		strcpy(strAvatarDBIp,stValue.dtString);
		configScp.GetData(CWWScript::eWWS_DT_STRING,&stValue,"IDDBCACHEIP");
		strcpy(strIDDBIp,stValue.dtString);
		configScp.GetData(CWWScript::eWWS_DT_SHORT,&stValue,"TYPE");
		g_iWorldType = stValue.dtShort;
		configScp.GetData(CWWScript::eWWS_DT_SHORT,&stValue,"SERVERINDEX");
		g_iWorldServerIndex = stValue.dtShort;
		configScp.GetData(CWWScript::eWWS_DT_SHORT,&stValue,"SERVERLISTINDEX");
		wServerListIndex = stValue.dtShort;
		configScp.GetData(CWWScript::eWWS_DT_UINT,&stValue,"DBCACHEPORT");
		dwAvatarDBPort = stValue.dtUInt;
		configScp.GetData(CWWScript::eWWS_DT_UINT,&stValue,"IDDBCACHEPORT");
		dwIDDBPort = stValue.dtUInt;

		WriteWorldServerIndexToRegistry(eST_WORLD,g_iWorldServerIndex);

		if	(configScp.GetData(CWWScript::eWWS_DT_USHORT,&stValue,"WORLDPORT"))
			l_wWorldPort	=	stValue.dtUShort;
		if	(configScp.GetData(CWWScript::eWWS_DT_USHORT,&stValue,"SEASONVARIABLE"))
			g_iSeasonVariable = stValue.dtUShort;
		if	(configScp.GetData(CWWScript::eWWS_DT_USHORT,&stValue,"DEV_SERVER"))
			g_bIsDevServer	=	stValue.dtUShort;

 		if	(configScp.GetData(CWWScript::eWWS_DT_SHORT,&stValue,"IGNORE_SS_INFO"))
 			g_bIsIgnoreSSInfo	= stValue.dtShort;

 		if	(configScp.GetData(CWWScript::eWWS_DT_SHORT,&stValue,"IS_INNER_TEST"))
 			g_bIsInnerTestServer	= stValue.dtShort;
	}	

#ifdef dUSE_SERVERSTATUS_REPORT
	if(g_bIsIgnoreSSInfo == FALSE && !g_SS.init(strWorldName, "Server"))
	{
		MessageBox(NULL,"ERROR IN SS","WARNING!!",MB_OK);
		return ;
	}
#endif

	UserManager.SetBaseSize(sizeof(cMSG_BASE_TYPE_FORWORLD));
	
// #ifndef _FOR_KOREA
// 	if(strncmp(strAvatarDBIp,"211.189",7)==0 || strncmp(strIDDBIp,"211.189",7)==0)	return;
// #endif
	
	cLOG = new DebugTracer(DTType_ALL,"RedStone_WorldServer");
	
	if	(!InitSocket())
	{
		_log("Error InitSocket \n");
		return;
	}
	
	_log("g_MaxUserCount = %d",g_MaxUserCount);
	
	//	이벤트 관련 정보를 요청한다.
	{
		WDMSG_ASK_EVENTS_INFO sPacket;
		sPacket.base.set(sizeof(WDMSG_ASK_EVENTS_INFO),dWDMSG_ASK_EVENTS_INFO);
		sPacket.isReally = 1;
		GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
	}

	//	기존 로그인 정보를 싹 삭제한다.
	{
		LDMSG_LOGINRESET	sPacketLogin;
		memset(&sPacketLogin,0,sizeof(LDMSG_LOGINRESET));
		sPacketLogin.base.set(sizeof(LDMSG_LOGINRESET),dLDMSG_LOGINRESET);
		GetADBSocket()->SendPacket((char *)&sPacketLogin,sPacketLogin.base.wSize);
	}
	_log("Start WORLD Server ... day  - Not Ready ");

	while(isMainLoop)
	{
		Sleep(1);
		
		OperateSpecialWork();
		
		for(int z = 0;z<dWORLD_MAX_USER_COUNT;z++)
		{
			CClient * user = UserManager.GetClientPnt(z,FALSE);
			
			if	(!user || user->GetSerial()==0xffff || user->GetIsClose())
				continue;
			
			SendPacket(user->GetSerial());
		}
		
		if	(g_bOffServer)
		{
			if	(UserManager.GameServerCount())
			{
				printf("The end of server is in the process of advancing. Some game server are connecting : count [%d]\n",UserManager.GameServerCount());
				Sleep(1000);
			}
			else
				break;
		}
	}

	g_bThreadLoop = FALSE;

	g_eventManager.rollbackChangeTimeInfo();

	g_guildBattleManager.close();

	printf("Wait a minute. Saving the data of all guild.\n");
	//	길드 정보를 저장한다.
	for(int g=0;g<dGUILD_MAX_COUNT;g++)
	{
		cGuild	* guild = g_guildManager.GetGuildPnt(g);
		
		if	(!guild || guild->wSerial==0xffff)
			continue;
		
		UpdateGuildData(guild);
	}
	
	printf("Wait a sec. Empty out the send buffer \n");

	while(GetADBSocket()->rPacketBuf->GetSize())
	{
		printf(" avatar dbc socket size = %d",GetADBSocket()->rPacketBuf->GetSize());
		GetADBSocket()->ReallySend();
		Sleep(1);
	}

	l_fileSaveItemCount.Close();

	Sleep(1000);

	UserManager.End();

	for (int i=10;i;i--)
	{
		printf("Wait a moment to end DBThread. %d\n",i);

		Sleep(1000);
	}

	GetADBSocket()->isLoopRecv = FALSE;

	GetIDDBSocket()->kill();
	GetADBSocket()->kill();

	printf("Ok. These works are finished.\n");

	DeleteCriticalSection(&csRemoveUser);
	DeleteCriticalSection(&csItemCount);
	DeleteCriticalSection(&csExitParty);
	DeleteCriticalSection(&csYahooCount);
	DeleteCriticalSection(&csParanCount);

	WriteServerStatusToRegistry(eST_WORLD,eSERVER_STATUS_OFF);

	KILL(cLOG);

	exit(0);

	return ;
}

BOOL
InitSocket()
{
	SOCKADDR_IN serverAddress;
	WSADATA wsadata;
	
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
	serverAddress.sin_addr.s_addr	=	htonl(INADDR_ANY);
	serverAddress.sin_family		=	AF_INET;
	serverAddress.sin_port			=	htons(l_wWorldPort);
	
	char name[32];
	gethostname(name,sizeof(name));
	struct hostent* he = gethostbyname(name);
	struct in_addr iaddr;
	memset(&iaddr,0,sizeof(iaddr));
	
	if	(l_strWorldIp[0]	==	NULL)
	{
		for(int i =0; he->h_addr_list[i] !=0; i++)
		{
			if	(he->h_addrtype!=AF_INET)
				continue;

			memcpy(&iaddr,he->h_addr_list[i],sizeof(iaddr));
			strcpy(l_strWorldIp,inet_ntoa(iaddr));
			
			if	(g_iWorldType==2)
			{
				if	(strncmp(l_strWorldIp,"19",2)!=0 && strncmp(l_strWorldIp,"17",2)!=0)
					continue;
			}
			else
			{
				if	(strncmp(l_strWorldIp,"19",2)==0 || strncmp(l_strWorldIp,"17",2)==0)
					continue;
			}

			_log("Open Server addr = %s\n",l_strWorldIp);
			break;
		}
	}
	
	if	(bind(lisSock,(LPSOCKADDR)&serverAddress,sizeof(serverAddress)) == SOCKET_ERROR)
	{
		cMSG::Error("bind error!!","perhaps... already world server running");
		_log("Fail bind !!\n");
		return FALSE;
	}
	
	listen(lisSock,5);
	
	if	(!InitCompletionPort())
	{	_log("Fail Init IOCP Port !!");
		return FALSE;
	}
	
	//	USERDB에 접속
	if(!GetIDDBSocket()->init(dwIDDBPort,strIDDBIp,"USER DBC",sizeof(cMSG_BASE_TYPE_FORWORLD)))
	{
		_log("Fail Connect User db !! Can't start world server yet! \n");
		return FALSE;
	}
	HANDLE hUSERDBThread = (HANDLE)_beginthread(USERDBCtlThread, 0, NULL); 
	if(hUSERDBThread==NULL)	{	GetIDDBSocket()->kill();	_log("Fail Create USERDBCtlThread. Can't start world server yet!");	return FALSE;	}
	
	//	DB서버 연결하기 
	if(!GetADBSocket()->init(dwAvatarDBPort,strAvatarDBIp,"AVATAR DBC",sizeof(cMSG_BASE_TYPE_FORWORLD)))
	{
		_log("Fail Connect db !!\n");
		return FALSE;
	}else
	{
		WORLDtoSERVERS_WORLDNAME	rPacket;
		rPacket.base.set( sizeof(WORLDtoSERVERS_WORLDNAME), dWORLDtoSERVERS_WORLDNAME);
		strcpy(rPacket.strWorldName , strWorldName);
		rPacket.wServerType	=	g_iWorldType;

		GetADBSocket()->SendPacket(( char  *)&rPacket,rPacket.base.wSize);
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
		_log("Fail Create AdditionThread");
		return FALSE;	
	}

	hDBThread = (HANDLE)_beginthread(DBCtlThread, 0, NULL); 
	if	(hDBThread==NULL)
	{
		_log("Fail Create DBCtlThread");
		return FALSE;
	}

	hCommandThread = (HANDLE)_beginthread(CommandThread, 0, NULL); 
	if	(hCommandThread==NULL)
	{	
		_log("Fail Create CommandThread");
		return FALSE;
	}

	l_hLifeCodeThread	=	(HANDLE)_beginthread( LifeCodeUpdateThread, 0,NULL); 
	if	(l_hLifeCodeThread==NULL)
	{
		_log("Fail Create LifeConeThread");
		return FALSE;
	}
	
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
	//	dwThreadNum = sys_info.dwNumberOfProcessors * 2 + 1;
	dwThreadNum = dMAX_WORLD_WORKER_COUNT;
	for(int i=0;i<(int)dwThreadNum;i++)
	{
		hWorkerThread[i]=(HANDLE)_beginthread(WorkerThread, 0,(LPVOID)i); 
		if(hWorkerThread[i]==NULL)
		{
			_log("Fail Worker [%d] Thread begin !!",i);
			return FALSE;
		}
	}
	return TRUE;
}

cGuild	*
GetGuild(int _iIndex)
{
	return	g_guildManager.GetGuildPnt(_iIndex);
}

void
LifeCodeUpdateThread(LPVOID lpParameter)
{
	//	로그인 서버 들어 올때까지 루프 돈다~
	while(g_bThreadLoop)
	{
		Sleep(10);

		if	(SendLPLifeCode(eBeginLifeSign,UserManager.GetClientPnt(dLOGIN_SERVER_ID),l_strWorldIp))
			break;
	}

	WriteServerStatusToRegistry(eST_WORLD,eSERVER_STATUS_ON);

	while(1)
	{
		for (int iLoop=50;iLoop ;--iLoop)	//	5 초간 기다린다.
		{
			Sleep(100);

			if	(g_bIsLifeCodeLoop	==	FALSE)	//	쓰레드가 종료 되었네?
			{
				SendLPLifeCode(eEndLifeSign,UserManager.GetClientPnt(dLOGIN_SERVER_ID),l_strWorldIp);

				return;
			}
		}

		WriteLifeSignToRegistry(eST_WORLD,1);

		if	(!g_lifeCodeMgr.checkAll(timeGetTime()))
			SendLPLifeCode(eIncorrectStatusSign,UserManager.GetClientPnt(dLOGIN_SERVER_ID),l_strWorldIp,g_lifeCodeMgr.m_strBadStatusLog);
		else
			SendLPLifeCode(eUpdateLifeSign,UserManager.GetClientPnt(dLOGIN_SERVER_ID),l_strWorldIp);
	}
}

void AcceptLoop(LPVOID lpParameter)
{
	DWORD Flags = 0;
	SOCKET		clientsock;
	
	WORD index=0;
	_log("Start AcceptLoop ...");

	while(g_bThreadLoop)
	{
		Sleep(1);

		g_lifeCodeMgr.endCheck(eLIFE_CODE_ACCEPT_LOOP);

		if	(isReady &&   ( (clientsock = accept(lisSock,NULL,NULL)) != INVALID_SOCKET))
		{
			if	(!g_bThreadLoop)
				continue;

			int buf=dMAX_PACKET_LENGTH_SERVER;

			if	(SOCKET_ERROR ==setsockopt(clientsock,SOL_SOCKET ,SO_SNDBUF,(const char*)&buf,sizeof(buf)))
			{
				shutdown(clientsock,1);
				closesocket(clientsock);
				clientsock = 0;
				continue;
			}

			if	(SOCKET_ERROR ==setsockopt(clientsock,SOL_SOCKET ,SO_RCVBUF,(const char*)&buf,sizeof(buf)))
			{
				shutdown(clientsock,1);
				closesocket(clientsock);
				clientsock = 0;
				continue;
			}

			LINGER	lingerStruct; lingerStruct.l_onoff = 1; lingerStruct.l_linger = 0; 
			if	(SOCKET_ERROR ==setsockopt(clientsock, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct)))
			{
				shutdown(clientsock,1);closesocket(clientsock);	clientsock = 0;	continue;
			}

			g_lifeCodeMgr.beginCheck(eLIFE_CODE_ACCEPT_LOOP);
			
#ifdef _FOR_KOREA
			char		_ip[16];
			SOCKADDR_IN	saddr;

			memset(&saddr,0,sizeof(saddr));
			int tmp = sizeof(sockaddr);
			getpeername(clientsock,(SOCKADDR *)&saddr,&tmp);

			strcpy(_ip,inet_ntoa(saddr.sin_addr));

// 			if	(strncmp(_ip,"211.189",7)!=0 &&  strncmp(_ip,"172.31.6",8)!=0 &&  strncmp(_ip,"58.150.56",9)!=0)
// 			{
// 				_log("엉뚱한 SERVER 출현 : %s",_ip);
// 				closesocket(clientsock);	clientsock = 0;	continue;
// 			}
#endif
			index = UserManager.Add();

			if	(index==0xffff)
			{
				_log("Fail Add User!!!");
				shutdown(clientsock,1);
				closesocket(clientsock); 
				continue;
			}

			CClient *newUser = UserManager.GetClientPnt(index);

			newUser->Open();
			newUser->SetSocket(clientsock);
			newUser->SetSerial(index);
#ifndef _FOR_KOREA
			SOCKADDR_IN	saddr;		
			memset(&saddr,0,sizeof(saddr));
			int tmp = sizeof(sockaddr);
			getpeername(clientsock,(SOCKADDR *)&saddr,&tmp);
			newUser->SetAddr(inet_ntoa(saddr.sin_addr));
#else
			newUser->SetAddr(_ip);
#endif
			_log("Connected Server's IP Address is [%s] \n",newUser->GetAddr());
			
			if(!CreateIoCompletionPort((HANDLE)newUser->GetSocket(),g_hCompletionPort,(DWORD)index,0))
			{
				UserManager.Del(newUser->GetSerial(),"");
				_log("Error CreateIoCompletionPort !! - %d",GetLastError());
				continue;
			}

			RecvPacket(index);
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_ACCEPT_LOOP);

	hAcceptThread	=	NULL;

	_log("END AcceptLoop ...");
	return ;
}

void WorkerThread(LPVOID lpParameter)
{
	LP_IO_DATA	lpOV;
	DWORD dwRead,dwIOCPKey;
	int	threadSerial = (int)lpParameter;
	
	_log("Start WorkerThread[%d]...",threadSerial);
	BOOL retVal = TRUE;

	while(g_bThreadLoop)
	{
		dwIOCPKey = 0xffffffff;

		g_lifeCodeMgr.endCheck(eLIFE_CODE_WORKER);

		retVal = GetQueuedCompletionStatus(g_hCompletionPort,&dwRead,&dwIOCPKey,(LPOVERLAPPED *)&lpOV,dwIOCPKey);

		g_lifeCodeMgr.beginCheck(eLIFE_CODE_WORKER);

		if	( retVal == TRUE && dwIOCPKey < dGAME_MAX_USER_COUNT && dwRead != 0 && lpOV != 0 )
		{
			CClient	* user = UserManager.GetClientPnt((WORD)dwIOCPKey);

			if	(!user)
				continue;	
			
			if	(lpOV->IOState == 0)
			{
				user->AddRecvPacket(lpOV->buf.buf,dwRead);
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
			if	(dwIOCPKey >= dGAME_MAX_USER_COUNT)	
				_log("SERROR : getqueue... dwIOCPKey < dGAME_MAX_USER_COUNT");
			if	(!lpOV)	
				_log("SERROR : getqueue... lpOV = NULL");
			
			if	(dwRead == 0 )
			{
				CClient	* user = UserManager.GetClientPnt((WORD)dwIOCPKey);

				if	(!user)
					continue;
				//				_log("Dis User [%s]",user->GetID());
				if	(user->m_wIsGameServer)	
					OperateDisconnectGameServer(user);
				user->PreClose();
				DisUser(dwIOCPKey,"read byte = 0");
			}
			else
			{
				DWORD dwError = GetLastError();
				_log("SERROR GetQueue Error : %d ",dwError);
				
				if	(dwError==ERROR_NETNAME_DELETED)
					_log("dwError ==ERROR_NETNAME_DELETED");
				
				if	(dwError != ERROR_IO_PENDING && dwError!=ERROR_OPERATION_ABORTED && dwIOCPKey < dGAME_MAX_USER_COUNT)
				{
					_log("%d Worker thread error GetQueueCompletionStatus... error [%d]",threadSerial,dwError);
					DisUser(dwIOCPKey,"GetQueuedCompletionStatus ERROR");
				}
			}
		}
	}

	hWorkerThread[threadSerial]	=	NULL;


	g_lifeCodeMgr.endCheck(eLIFE_CODE_WORKER);

	_log("END WorkerThread[%d]...",threadSerial);
	return ;
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
			_log("RECV ERROR ... ERROR %d",dwError);
			DisUser(serial,"RECV ERROR");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL SendPacket(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);
	
	if	(!user || user->GetIsClose())
		return FALSE;
	
	WORD iSendSize = (WORD)user->sBuffer->GetData(user->sendpacket,0);
	if(!iSendSize)
	{
		return TRUE;
	}
	
	user->sOL.IOState = 1;
	DWORD	lpSend = 0;	
	WSABUF	buf;
	
	user->sendpacket_size = iSendSize;
	
	buf.len	=	user->sendpacket_size;
	buf.buf	=	user->sendpacket;
	
	if(WSASend(user->GetSocket(), &buf,1,&lpSend,0,&user->sOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();
		if	(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			_log("SEND ERROR ... ERROR %d",dwError);
			user->sBuffer->Next();
			DisUser(serial,"SEND ERROR");
			return FALSE;
		}
	}
	
	return TRUE;
}

void	SendPacket_Add(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);
	if(!user || user->GetIsClose())	return;
	user->sOL.IOState = 1;
	DWORD	lpSend = 0;	WSABUF	buf;
	buf.len = user->sendpacket_size;
	buf.buf = user->sendpacket;
	
	//	_log("Send_ADD Data Size : %d", buf.len);
	if(WSASend(user->GetSocket(), &buf,1,&lpSend,0,&user->sOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();
		if(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			_log("SEND_ADD ERROR ... ERROR %d",dwError);
			user->sBuffer->Next();
			DisUser(serial,"SEND_ADD ERROR");
			return;
		}
	}
}

void
ProcessRecvPacket(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);
	
	if	(!user || user->GetIsClose())
		return;
	
	int		continuepacketCountPerGameServer=0;
	WORD	type = 0,recvsize = 0;
	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];

	memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);

	int		resultGetRecvPacket=0;

	while(TRUE)
	{
		continuepacketCountPerGameServer++;

		resultGetRecvPacket	=	user->GetRecvPacket(recvpacket);

		if	(resultGetRecvPacket==0)
			break;

		if	(continuepacketCountPerGameServer>20)
		{
//			_log("latency Count : %d\n",continuepacketCountPerGameServer);
			break;
		}

		recvsize=	*(WORD *)recvpacket;
		type	=	*(WORD *)(recvpacket+2);

		if	(type == dCM_COMPRESS)
		{
			CM_COMPRESS	comp;
			memcpy(&comp,recvpacket,recvsize);
			cSRVUTIL::Uncompress((unsigned char *)comp.data,(unsigned char *)recvpacket,comp.wSize - sizeof(WORD)*3,comp.wSourceSize);
			recvsize = *(WORD *)recvpacket;
			type = *(WORD *)(recvpacket+2);
		}
		//		if(recvsize>8)	
		//			printf(" recv packet type : 0x%x, %d\n",type,recvsize);
		switch(type)
		{
			case dBW_ASK_PARTY_LIST:
				g_partyManager.sendAllPartyInfoToBCS();
				break;

			case dBW_ASK_PARTY_INFO:
			{
				BW_ASK_PARTY_INFO	*lpPacket	=	(BW_ASK_PARTY_INFO	*)recvpacket;

				g_partyManager.sendUpdatePartyInfoToBCS(lpPacket->wPartySerial);
				break;
			}

			case dWORLDtoSERVERS_CHEKCPACKET:
				testSizeCount++;
				printf("testSizeCount = %d\n",testSizeCount);
				break;


			case dSERVERStoWORLD_OFFSERVER:
			{
				SERVERStoWORLD_OFFSERVER * rPacket;
				rPacket = (SERVERStoWORLD_OFFSERVER *)recvpacket;
				
				if	(strcmp(rPacket->strMsg,"cjfvksrndl")==0)
				{
					g_bIsLifeCodeLoop	=	FALSE;
					Sleep(100);	//	라이프 사인 쓰레드 때문에..
					//	종료 라이프 사인 보내기
					SendLPLifeCode(eEndLifeSign,UserManager.GetClientPnt(dLOGIN_SERVER_ID),l_strWorldIp);

					//	로그인 서버와 어드민 툴을 잘라 버린다.
					for(int i=0;i<4;i++)
					{
						CClient * loginusr = UserManager.GetClientPnt(dLOGIN_SERVER_ID);

						if	(loginusr)
							DisUser(loginusr->GetSerial(),"For exit Server!");
						else
							break;
					}

					CClient * adminusr = UserManager.GetClientPnt(dADMIN_TOOL_ID);

					if	(adminusr)
					{	
						DisUser(adminusr->GetSerial(),"For exit Server!");
					}

					g_bOffServer = TRUE;
				}
				break;
			}

			case dSERVERStoWORLD_INFO:
			{
				SERVERStoWORLD_INFO	* rPacket;
				rPacket = (SERVERStoWORLD_INFO *)recvpacket;

//	기존의 유저수 업데이트하는 것은 dSERVERStoWORLD_ACCOUNT_INFO 에서 처리함				
				if	(rPacket->wSocketIndex != 0xffff) // 게임 서버에서 보내온 유저 수 
				{
					user->wSendCount = rPacket->wCount;
					user->wCurIndex = rPacket->wSocketIndex;
				}
				break;
			}

			case dSERVERStoWORLD_LOGOUT:	//	send만 안돼게 막아둔다.
			{
				user->isSendPacket = FALSE;
				break;
			}

			case dSERVERStoWORLD_LOGIN:
			{
				SERVERStoWORLD_LOGIN * rPacket;

				rPacket = (SERVERStoWORLD_LOGIN *)recvpacket;

				if	(g_bOffServer)
				{
#ifdef _FOR_KOREA
					_log("서버 종료중에 어떤 서버도 받지 않습니다.",rPacket->strServerName);
#endif
					DisUser(serial,"Not allow any server!!");
					break;
				}

				user->SetID(rPacket->strServerName);
				user->wConnectCount	= rPacket->wNICount;

				if	(user->wConnectCount == 0 && STRICMP(rPacket->strServerName,"CS_SERVER")==0)
				{
					user->wMapCount	 = 0;
					user->m_wServerIndex = 0xffff;
					UserManager.m_wBCSIndex = user->GetSerial();
					//	월드서버의 이름을 보내준다.
					WB_WORLDNAME	sPacket;
					sPacket.base.set(sizeof(WB_WORLDNAME),dWB_WORLDNAME);
					strcpy(sPacket.strWorldName,strWorldName);
					user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
					SendPacket(user->GetSerial());
					break;
				}
				
				user->wMapCount	=	rPacket->wMapCount;
				memset(&user->cGSInfo,0,sizeof(cGameServerType)*dGAME_MAX_FIELD_COUNT);
				memcpy(&user->cGSInfo,rPacket->cGSInfo,sizeof(cGameServerType)*dGAME_MAX_FIELD_COUNT);
				user->m_wServerIndex	=	rPacket->wServerIndex;
				
				if	(rPacket->wNICount<0xcccc && strcmp(dADMIN_TOOL_ID,rPacket->strServerName)!=0)
				{
					for(int i=0;i<user->wConnectCount;i++)
					{
						//						_log("recv ip [%s]",rPacket->strIp[i]);
						strcpy(user->strConnectaddr[i],rPacket->strIp[i]);
					}
					++UserManager.m_wGSCount;	//	동시에 들어오면 문제가 발생할 수도 있지만.. 게임서버가 그리 많지 않으니 신경쓰지 말자.
				}

				user->wSendIndex=	2;
				user->wSendCount=	0;
				user->wCurIndex	=	0;
				
				//				_log("[%s] Server is connected ",user->GetID());
				if	(STRICMP(rPacket->strServerName,dADMIN_TOOL_ID)==0)
				{
					user->wSendIndex	=	0xffff;
					
					for(int i=0;i<dMAX_ADMIN_COUNT;i++)
					{
						if	(!_Admin[i].isUsed)
						{
							_Admin[i].iMyIndex		=	i; 
							_Admin[i].iAdminSerial	=	serial;
							_Admin[i].isUsed		=	TRUE;
							_Admin[i].wVersion		=	rPacket->wMapCount;	//	mapcount에 admin툴의 버젼을 보낸다.
							break;	
						}
					}
					
					if(i==dMAX_ADMIN_COUNT)	
					{	
						DisUser(serial,"Can't not allow an admin any more!");
					}	//	더이상 admin을 받을 수 없다.
					else 
					{
						if(_Admin[i].wVersion<8)
						{
							DisUser(serial,"Your Version is older.");	//	버젼으로 접속 여부를 수용해준다.
						}
						else
						{	
							isAdmin = TRUE;	
						}
					}
				}

				//	게임 서버다
				if	(STRICMP(rPacket->strServerName,dADMIN_TOOL_ID)!=0 && STRICMP(rPacket->strServerName,dLOGIN_SERVER_ID)!=0 && strcmp(rPacket->strServerName,"CS_SERVER")!=0)
				{
					_log(" Send sendAppointmentQuizEventInfo... ");
					sendAppointmentQuizEventInfo(user);
					sendWorldValueInfo(user);

					user->setGameServer();
					user->m_wIsGameServer	=	TRUE;
					//	게임서버일 경우만...
					//	길드 정보를 해당 게임서버에게 보내준다.
					_log(" This user is game server. isReconnect[%d] \n",rPacket->isReconnect);
					if	(!rPacket->isReconnect)
						SendGameServerInitData(serial);

					//	게임 서버에게 월드 서버의 이름을 보내준다.
					WORLDtoSERVERS_WORLDNAME	rPacket;
					rPacket.base.set( sizeof(WORLDtoSERVERS_WORLDNAME), dWORLDtoSERVERS_WORLDNAME);
					rPacket.wServerType	=	g_iWorldType;

					strcpy(rPacket.strWorldName , strWorldName);
					user->AddSendPacket(( char  *)&rPacket,rPacket.base.wSize);
				}
				else
				{
					_log(" This user is not game server");
					user->m_wIsGameServer	=	FALSE;
					user->m_wServerIndex	=	0xffff;

					if	(strcmp(rPacket->strServerName,"CS_SERVER")==0)
					{
						//	커뮤니티 서버에게 월드 서버의 이름을 보내준다.
						WORLDtoSERVERS_WORLDNAME	rPacket;
						rPacket.base.set( sizeof(WORLDtoSERVERS_WORLDNAME), dWORLDtoSERVERS_WORLDNAME);
						rPacket.wServerType	=	g_iWorldType;

						strcpy(rPacket.strWorldName , strWorldName);
						user->AddSendPacket(( char  *)&rPacket,rPacket.base.wSize);
					}
				}

				break;
			}
		case dSERVERStoWORLD_CONFIRM_ENTER_FIELD	:
			{
				SERVERStoWORLD_CONFIRM_ENTER_FIELD * rPacket;
				rPacket = (SERVERStoWORLD_CONFIRM_ENTER_FIELD *)recvpacket;

				WORLDtoSERVERS_CONFIRM_ENTER_FIELD	packet;

				packet.base.set(sizeof(packet),dWORLDtoSERVERS_CONFIRM_ENTER_FIELD);

				packet.wField	=	rPacket->wField;
				strcpy(packet.strId,rPacket->strId);
				strcpy(packet.strName,rPacket->strName);
				SendDataAllGameServer((char *)&packet,packet.base.wSize);
				break;
			}

		case dSERVERStoWORLD_GETAVAS:	//	해당하는 아이디의 아바타 정보를 찾아서 보내준다.
			{
				if	(wServerCheckType == eServerCheckType_STOP)
					break;
				
				SERVERStoWORLD_GETAVAS * rPacket;
				rPacket = (SERVERStoWORLD_GETAVAS *)recvpacket;
				
				LDMSG_GETAVATARS		sPacket;
				sPacket.base.set(sizeof(LDMSG_GETAVATARS),dLDMSG_GETAVATARS);
				sPacket.u_serial = rPacket->loginSerial;
				sPacket.worldSerial = serial;
				strcpy(sPacket.strID,rPacket->strId);
				GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
				break;
			}
		case dSERVERStoWORLD_CREATEAVA:
			{
				SERVERStoWORLD_CREATEAVA * rPacket;
				rPacket = (SERVERStoWORLD_CREATEAVA *)recvpacket;
				
				LDMSG_CREATEAVATAR		sPacket;
				sPacket.base.set(sizeof(LDMSG_CREATEAVATAR),dLDMSG_CREATEAVATAR);
				sPacket.u_serial = rPacket->loginSerial;
				sPacket.worldSerial = serial;
				sPacket.wServerType	=	g_iWorldType;

				strcpy(sPacket.strID,rPacket->strID);
				strcpy(sPacket.strName,rPacket->strName);
				strcpy(sPacket.strIP,rPacket->strIP);
				
				if	(rPacket->wJob	>	dPLAYER_JOB_COUNT)
				{
					_log("ERROR job data : %d [%s]",rPacket->wJob,sPacket.strID);
					break;
				}
				sPacket.wJob = rPacket->wJob;
				GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
				break;
			}
		case dSERVERStoWORLD_DELETEAVA:
			{
				SERVERStoWORLD_DELETEAVA * rPacket;
				rPacket = (SERVERStoWORLD_DELETEAVA *)recvpacket;
				
				LDMSG_DELETEAVATAR			sDelAva;
				sDelAva.base.set(sizeof(LDMSG_DELETEAVATAR),dLDMSG_DELETEAVATAR);
				sDelAva.u_serial = rPacket->loginSerial;
				sDelAva.worldSerial = serial;
				strcpy(sDelAva.strID,rPacket->strID);
				strcpy(sDelAva.strNAME,rPacket->strNAME);
				strcpy(sDelAva.strIP,rPacket->strIP);
				GetADBSocket()->SendPacket((char *)&sDelAva,sDelAva.base.wSize);
				g_partyManager.removePartyMember(sDelAva.strNAME);
				break;
			}
		case dSERVERStoWORLD_LOADAVA:
			{
				SERVERStoWORLD_LOADAVA * rPacket;
				rPacket = (SERVERStoWORLD_LOADAVA *)recvpacket;
				
				LDMSG_SELECTAVATAR		sLoadAva;
				sLoadAva.base.set(sizeof(LDMSG_SELECTAVATAR),dLDMSG_SELECTAVATAR);
				sLoadAva.loginSerial = rPacket->loginSerial;
				sLoadAva.worldSerial = serial;
				strcpy(sLoadAva.strID,rPacket->strID);
				strcpy(sLoadAva.strNAME,rPacket->strNAME);
				GetADBSocket()->SendPacket((char *)&sLoadAva,sLoadAva.base.wSize);
				break;
			}
		case dSERVERStoWORLD_ISCONNECT_USER:
			{
				SERVERStoWORLD_ISCONNECT_USER * rPacket;
				rPacket = (SERVERStoWORLD_ISCONNECT_USER *)recvpacket;
				
				WORLDtoSERVERS_ISCONNECT_USER		UserInfo;
				UserInfo.base.set(sizeof(WORLDtoSERVERS_ISCONNECT_USER),dWORLDtoSERVERS_ISCONNECT_USER);
				strcpy(UserInfo.strId,rPacket->strId);
				Avatar_Data * aData = UserManager.GetGClientPnt(rPacket->strId);
				if(aData && aData->wSerial!=0xffff)
				{
					strcpy(UserInfo.strName,aData->data.m_strName);
					UserInfo.wfieldUniqueIndex = aData->data.m_wCurrentField;
				}else{
					strcpy(UserInfo.strName,"EMPTY");
				}
				user->AddSendPacket((char *)&UserInfo,UserInfo.base.wSize);
				break;
			}
			
		case dSERVERStoWORLD_EVENT_MESSAGE	:
			{
				SERVERStoWORLD_EVENT_MESSAGE * rPacket;
				rPacket = (SERVERStoWORLD_EVENT_MESSAGE *)recvpacket;
				
				WORLDtoSERVERS_EVENT_MESSAGE	sendPacket;
				sendPacket.base.set(sizeof(sendPacket),dWORLDtoSERVERS_EVENT_MESSAGE);
				sendPacket.wType	=	rPacket->wType;
				strcpy(sendPacket.strMessage,rPacket->strMessage);
				SendDataAllGameServer((char *)&sendPacket,sendPacket.base.wSize);
				break;
			}
			
		case dSERVERStoWORLD_ALL_MSG:
			{
				SERVERStoWORLD_ALL_MSG * rPacket;
				rPacket = (SERVERStoWORLD_ALL_MSG *)recvpacket;
				
				rPacket->base.wType = dWORLDtoSERVERS_ALL_MSG;
				SendDataAllGameServer((char *)rPacket,rPacket->base.wSize);
				break;
			}
			
		case dSERVERStoWORLD_CALLING	:	//	파티원 소환
			{
				SERVERStoWORLD_CALLING * rPacket;
				rPacket = (SERVERStoWORLD_CALLING *)recvpacket;
				
				sendGSPCalling(NULL,rPacket);
				break;
			}
			
		case dSERVERStoWORLD_CHANGE_GUILD_BATTLE_REPORT	:
			{
				break;
			}
		case dWORLDtoSERVERS_CHAGEGUILDBATTLE:
			{
				WORLDtoSERVERS_CHAGEGUILDBATTLE * rPacket;
				rPacket = (WORLDtoSERVERS_CHAGEGUILDBATTLE *)recvpacket;
				
				cGuild * tmpGuild;
				WORD	wGuildSerial1,wGuildSerial2;
				
				tmpGuild	=	g_guildManager.GetGuildPnt(rPacket->strGuildName1);
				
				if(tmpGuild)	wGuildSerial1 = tmpGuild->m_wSerial;
				else			wGuildSerial1 = 0xffff;
				
				tmpGuild = g_guildManager.GetGuildPnt(rPacket->strGuildName2);
				if(tmpGuild)	wGuildSerial2 = tmpGuild->m_wSerial;
				else			wGuildSerial2 = 0xffff;
				
				
				CGuildBattleInfo	returnData,cancelData;
				int iResult = g_guildBattleManager.changeBattle(wGuildSerial1,wGuildSerial2,rPacket->isTime,rPacket->wField,&returnData,&cancelData);
				
				if(!iResult)	break;
				
				WORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO		infoBattle;
				infoBattle.base.set(sizeof(WORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO),dWORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO);
				infoBattle.wTimeOrder	=	rPacket->isTime;
				infoBattle.wFieldOrder	=	rPacket->wField;
				//	기존의 길드는 취소해준다.
				{
					if(cancelData.m_awBattleGuild[0] != 0xffff)
					{
						cGuild	*lpBattleCancelGuild = g_guildManager.GetGuildPnt(cancelData.m_awBattleGuild[0]);
						if(lpBattleCancelGuild && lpBattleCancelGuild->wSerial!=0xffff)
						{
							infoBattle.wGuildSerial = lpBattleCancelGuild->m_wSerial;
							infoBattle.awBattleGuild[0] = 0xffff;
							infoBattle.awBattleGuild[1] = cancelData.m_awBattleGuild[1];
							SendDataAllGameServer((char *)&infoBattle,infoBattle.base.wSize);
							SaveGuildBaseData(lpBattleCancelGuild);
						}
					}
					if(cancelData.m_awBattleGuild[1] != 0xffff)
					{
						cGuild	*lpBattleCancelGuild = g_guildManager.GetGuildPnt(cancelData.m_awBattleGuild[1]);
						if(lpBattleCancelGuild && lpBattleCancelGuild->wSerial!=0xffff)
						{
							infoBattle.wGuildSerial = lpBattleCancelGuild->m_wSerial;
							infoBattle.awBattleGuild[0] = 0xffff;
							infoBattle.awBattleGuild[1] = 0xffff;
							SendDataAllGameServer((char *)&infoBattle,infoBattle.base.wSize);
							SaveGuildBaseData(lpBattleCancelGuild);
						}
					}
				}
				//	새롭게 등록된 내용을 보내준다.
				infoBattle.wGuildSerial		=	0xffff;
				
				if (wGuildSerial1	!=	0xffff)
				{
					cGuild	*lpGuild	= g_guildManager.GetGuildPnt(wGuildSerial1);
					
					if(lpGuild	==	NULL)
						wGuildSerial1	=	0xffff;
				}
				if (wGuildSerial2	!=	0xffff)
				{
					cGuild	*lpGuild	= g_guildManager.GetGuildPnt(wGuildSerial2);
					
					if(lpGuild	==	NULL)
						wGuildSerial2	=	0xffff;
				}
				
				infoBattle.awBattleGuild[0] =	wGuildSerial1;
				infoBattle.awBattleGuild[1] =	wGuildSerial2;
				
				//				sendGSPGuildBattleInfo(NULL,rPacket->isToday,rPacket->isTime,rPacket->wField);
				
				SendDataAllGameServer((char *)&infoBattle,infoBattle.base.wSize);
				break;
			}
			//	GAME SERVER PACKET 
		case dSERVERStoWORLD_USER_LOGIN:
			{
				SERVERStoWORLD_USER_LOGIN * rPacket;
				rPacket = (SERVERStoWORLD_USER_LOGIN *)recvpacket;
				
				if(wServerCheckType > eServerCheckType_NORMAL)
					break;

				if(rPacket->type)
				{
					WORLDtoSERVERS_USER_LOGIN		sUserLogin;
					
					sUserLogin.base.set(sizeof(WORLDtoSERVERS_USER_LOGIN),dWORLDtoSERVERS_USER_LOGIN);
					sUserLogin.serial		=	(WORD)rPacket->serial;
					//	현재 자신의 정보에 이 유저의 데이타가 있는지 확인한다.
					//					_log("In [%s], find [%s]\n",user->GetID(),rPacket->strID);
					Avatar_Data * ava = user->GameClientMagager.GetAvatarPnt(rPacket->strID);
					
					if(ava !=NULL && ava->wSerial!=0xffff)
					{
						//						if(g_iWorldType>0)	_log("[%s(%s) P : [%d]] came from another map to here \n",rPacket->strID,rPacket->strName,ava->GetPartySerial());
						//						_log("in [%s], [%s] is success login!!\n",user->GetID(),ava->data.m_strId);
						
						ava->wState				=	2;
						ava->SetTime();
						
						sUserLogin.wFieldidx	=	ava->data.m_wCurrentField;
						sUserLogin.bf10IFSerial	=	ava->data.m_bf8InstanceField;
						sUserLogin.bf4IFFloor	=	ava->data.m_bf4InstanceFieldFloor;
						sUserLogin.wPartyIndex	=	ava->keepData.getPartySerial();
						
						//						_log("--------------wanted field  = %d \n",sUserLogin.wFieldidx);
						
						sUserLogin.iIndex		=	ava->wSerial;
						sUserLogin.serial		=	rPacket->serial;
						sUserLogin.oper			=	ava->isOper;
						sUserLogin.isNetCafeUser=	ava->isNetCafeUser;
						if(sUserLogin.oper>6)
						{
							_log("ERROR OPER (in dSERVERStoWORLD_USER_LOGIN)is = %d ,[%s][%s]",sUserLogin.oper,ava->data.m_strId,ava->data.m_strName);
						}
						
						g_guildManager.updateLastConnectTime(ava->data.m_wGuildSerial,ava->data.m_wGuildRank);	//	멤버/마스터 마지막 접속일 기록

						//	cGuildManagerForWS::updateLastConnectTime 로 옮김. 아래 코드는 백업
// 						if	(ava->data.m_wGuildSerial!=0xffff)
// 						{	//	해당 길드의 최근 접속한 시간을 갱신한다.
// 							cGuild * tmpGuild =	g_guildManager.GetGuildPnt(ava->data.m_wGuildSerial);
// 
// 							if	(tmpGuild)
// 							{
// 								//	길드원 접속 일자 갱신
// 								{
// 									int	iOldDay	=	tmpGuild->m_timeLastConnectTime.year*372+tmpGuild->m_timeLastConnectTime.month*31+tmpGuild->m_timeLastConnectTime.day;
// 
// 									tmpGuild->m_timeLastConnectTime.year	=	cSRVUTIL::GetYear()-100;
// 									tmpGuild->m_timeLastConnectTime.month	=	cSRVUTIL::GetMonth()+1;
// 									tmpGuild->m_timeLastConnectTime.day		=	cSRVUTIL::GetDay();
// 
// 									int	iCurDay	=	tmpGuild->m_timeLastConnectTime.year*372+tmpGuild->m_timeLastConnectTime.month*31+tmpGuild->m_timeLastConnectTime.day;
// 
// 									if	(iOldDay	!=	iCurDay)
// 										SaveGuildBaseData(tmpGuild);
// 								}
// 
// 								if	(ava->data.m_wGuildRank==6)	//	길마 접속 일자 갱신
// 								{
// 									int	iOldDay	=	tmpGuild->m_timeLastConnectedMasterTime.year*372+tmpGuild->m_timeLastConnectedMasterTime.month*31+tmpGuild->m_timeLastConnectedMasterTime.day;
// 
// 									tmpGuild->m_timeLastConnectedMasterTime.year	=	cSRVUTIL::GetYear()-100;
// 									tmpGuild->m_timeLastConnectedMasterTime.month	=	cSRVUTIL::GetMonth()+1;
// 									tmpGuild->m_timeLastConnectedMasterTime.day		=	cSRVUTIL::GetDay();
// 
// 									int	iCurDay	=	tmpGuild->m_timeLastConnectedMasterTime.year*372+tmpGuild->m_timeLastConnectedMasterTime.month*31+tmpGuild->m_timeLastConnectedMasterTime.day;
// 
// 									if	(iOldDay!=	iCurDay)
// 										SaveGuildBaseData(tmpGuild);
// 								}
// 							}
// 						}
					}
					else
					{
						WORD gsSerial = UserManager.GetClientOfGClientSerial(rPacket->strID);
						if(gsSerial<0xffff)
						{
							_log("login result error 1 : the client info is in [%d] Server. (I'm [%d] Server) [%s]",gsSerial,user->GetSerial(),rPacket->strID);
							Avatar_Data * _clientptr = UserManager.GetGClientPnt(rPacket->strID);
							if(_clientptr)
							{
								_log("login result error 2 : client info [%s (%s)] field[%d] boostTime %d , action speed %d",_clientptr->data.m_strId,_clientptr->data.m_strName,_clientptr->data.m_wCurrentField,_clientptr->keepData.m_wBoostTime,_clientptr->upkeepStatus.m_actionSpeed);
							}else
							{
								_log("Can not find the user data [%s]",rPacket->strID);
							}
						}else
						{
							_log("can not find [%s] in this world in dSERVERStoWORLD_USER_LOGIN\n",rPacket->strID);
						}
						sUserLogin.iIndex  = -1;
					}
					user->AddSendPacket((char *)&sUserLogin,sUserLogin.base.wSize);
				}else
				{
					if(g_dwUserCount>=g_MaxUserCount)
					{
						//	여기 테스트 해볼것!! 
						//						_log("g_dwUserCount(%d), g_MaxUserCount(%d)",g_dwUserCount,g_MaxUserCount);
						LU_LOGOUT	sPacket;
						sPacket.base.set(sizeof(LU_LOGOUT),dLU_LOGOUT,serial);
						strcpy(sPacket.strId,rPacket->strID);
						strcpy(sPacket.strIp,rPacket->strIP);
						GetIDDBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
						
						WDMSG_KILL_AVATAR	ssPacket;
						ssPacket.base.set(sizeof(WDMSG_KILL_AVATAR),dWDMSG_KILL_AVATAR);
						GetADBSocket()->SendPacket((char *)&ssPacket,ssPacket.base.wSize);
						
						WORLDtoSERVERS_USER_LOGIN		sUserLogin;
						sUserLogin.base.set(sizeof(WORLDtoSERVERS_USER_LOGIN),dWORLDtoSERVERS_USER_LOGIN);
						sUserLogin.iIndex = -2;
						sUserLogin.serial = rPacket->serial;
						user->AddSendPacket((char *)&sUserLogin,sUserLogin.base.wSize);
						//						_log("인원 초과로 로그인 실패 [%d/%d]\n",g_dwUserCount,g_MaxUserCount);
					}else{
						LU_WLOGIN	sPacket;
						sPacket.base.set(sizeof(LU_WLOGIN),dLU_WLOGIN,serial);
						sPacket.serial = rPacket->serial;
						strcpy(sPacket.strId,rPacket->strID);
						strcpy(sPacket.strName,rPacket->strName);
						sPacket.dwCode = rPacket->dwCode;
						strcpy(sPacket.strIP,rPacket->strIP);
						strcpy(sPacket.strMacAddress, rPacket->strMacAddress);
						strcpy(sPacket.strClientCode, rPacket->strClientCode);
						//_log("request log in [%s]",sPacket.strId,sPacket.strIP);
						GetIDDBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
					}
				}
				break;
			}
		case dSERVERStoWORLD_COPY_DATA:
			{	//	테스트 서버라면 데이타를 복사한다.
				SERVERStoWORLD_COPY_DATA * rPacket;

				rPacket = (SERVERStoWORLD_COPY_DATA *)recvpacket;

				if	(rPacket->wType	==	eSERVER_TYPE_TEST && g_iWorldType != eSERVER_TYPE_TEST)
					break;
				if	(rPacket->wType	==	eSERVER_TYPE_DUEL && g_iWorldType != eSERVER_TYPE_DUEL)
					break;

				LDMSG_COPY_DATA				sCopyData;
				sCopyData.base.set(sizeof(LDMSG_COPY_DATA),dLDMSG_COPY_DATA);
				strcpy(sCopyData.strOrgName,rPacket->strOrgName);

				sCopyData.wType				=	rPacket->wType;
				sCopyData.wWorld			=	rPacket->wOrgWorld;

				memcpy(&sCopyData.info,&rPacket->info,sizeof(cAVATAR_INFO));
				strcpy(sCopyData.strId,rPacket->strId);

				if	(g_iWorldType == eSERVER_TYPE_DUEL)	//	결투 서버의 캐릭터 레벨은 모두 300
					sCopyData.info.m_wLevel	=	c_iLevelForDuelServer;

				GetADBSocket()->SendPacket((char *)&sCopyData,sCopyData.base.wSize);
				break;
			}
			
		case dSERVERStoWORLD_ACCOUNT_INFO	:
			{
				SERVERStoWORLD_ACCOUNT_INFO	*lpPacket	=	(SERVERStoWORLD_ACCOUNT_INFO *)recvpacket;

				user->m_wCurrentPlayerCount	=	lpPacket->iPlayerCount;
				g_worldUtil.setPlayerCount(lpPacket);
			
				break;
			}

		case dSERVERStoWORLD_PLAYER_COUNT	:
			{
				SERVERStoWORLD_PLAYER_COUNT	*lpPacket	=	(SERVERStoWORLD_PLAYER_COUNT *)recvpacket;

				g_worldUtil.updatePlayerCountInField(lpPacket->wHour,lpPacket->wCount,lpPacket->awPlayerCount);
				
				break;
			}
			
		case dSERVERStoWORLD_GET_AVADATA	:
			{
				SERVERStoWORLD_GET_AVADATA * rPacket;
				rPacket			=	(SERVERStoWORLD_GET_AVADATA *)recvpacket;
				
				WORLDtoSERVERS_GET_AVADATA	sAvaData;
				sAvaData.base.set(sizeof(WORLDtoSERVERS_GET_AVADATA),dWORLDtoSERVERS_GET_AVADATA);
				sAvaData.serial	=	rPacket->serial;
				sAvaData.type	=	0;

				memset(&sAvaData.aInfo,0,sizeof(CPlayerSaveDataForServerDefine));
				
				Avatar_Data * ava = user->GameClientMagager.GetAvatarPnt(rPacket->strId);
				
				if(!ava || ava->wState==0xffff)
				{
					_log("ERROR SERVERStoWORLD_GET_AVADATA : ava==NULL || ava->wState==0xffff : type[%d] [%s] \n",rPacket->type,rPacket->strId);
					sAvaData.type = 0xffff;
					user->AddSendPacket((char *)&sAvaData,sAvaData.base.wSize);
					break;
				}

#ifdef	_OGP_SERVICE
				strcpy(ava->m_strToken,rPacket->strToken);
#endif

				/////////////////////////////////
				//첨 들어올때 유저의 필드이동시에 유지해야 하는 데이터를 초기 로그인시 초기화를 위해 세팅한다.
				if	(ava->wState == 1) 
					sAvaData.type = 1;
				/////////////////////////////////
				
				if(STRICMP(ava->data.m_strId,rPacket->strId)!=0)
				{
#ifdef _FOR_KOREA
					_log("[%s]가 [%s]를 요청하다니...\n",rPacket->strId,ava->data.m_strId);
#endif
					sAvaData.type = 0xffff;
					user->AddSendPacket((char *)&sAvaData,sAvaData.base.wSize);
					break;
				}
				
				memcpy(&sAvaData.aInfo,&ava->data,sizeof(CPlayerSaveDataForServerDefine));
				memcpy(&sAvaData.upkeepPlayerData,&ava->keepData,sizeof(CUpkeepPlayerData));
				memcpy(&sAvaData.upkeepStatus,&ava->upkeepStatus,sizeof(cActorStatusUpkeep));
				
				if(ava->data.m_iLevel==0 || sAvaData.aInfo.m_iLevel==0)
				{
					_log("ERROR SERVERStoWORLD_GET_AVADATA : ava->data.m_iLevel==0 || sAvaData.aInfo.m_iLevel==0  name[%s]  \n",ava->data.m_strName);
					sAvaData.type = 0xffff;
					user->AddSendPacket((char *)&sAvaData,sAvaData.base.wSize);
					break;
				}
				
				WORD	_wRESULT;
				CClient	*cli	=	UserManager.GetClientOfField( sAvaData.aInfo.m_wCurrentField ,1,_wRESULT);
				
				if	(!cli)
				{
					_log("ERROR GETAVATARDATA : getAva - current  field serial = %d\n",sAvaData.aInfo.m_wCurrentField);
					sAvaData.type = 0xffff;
					user->AddSendPacket((char *)&sAvaData,sAvaData.base.wSize);
					break;
				}
				
				ava->wState = 2;
				ava->bMovingMap = FALSE;
				user->AddSendPacket((char *)&sAvaData,sAvaData.base.wSize);
				//				_logf("GET_AVA_DATA : [%s] [%s] lev[%d] exp[%d] savecount[%d]",sAvaData.aInfo.m_strId,sAvaData.aInfo.m_strName,sAvaData.aInfo.m_iLevel,sAvaData.aInfo.m_iExperience,sAvaData.aInfo.m_dwSaveCount);
				break;
			}
		case dSERVERStoWORLD_JOIN_COMPLETE:
			{
				SERVERStoWORLD_JOIN_COMPLETE * rPacket;
				rPacket=(SERVERStoWORLD_JOIN_COMPLETE *)recvpacket;
				
				Avatar_Data * ava = user->GameClientMagager.GetAvatarPnt(rPacket->strId);
				if(ava && ava->wSerial!=0xffff)
				{
					ava->wState = 2;
					//	길드가 있다면
					if(ava->data.m_wGuildSerial!=0xffff)
					{
						cGuild * guild = g_guildManager.GetGuildPntBySerial(ava->data.m_wGuildSerial);
						if(guild && guild->wSerial!=0xffff)
						{
							CGuildMan * guilder = guild->GetGuilderPnt(ava->data.m_strName);
							if(guilder && guilder->wSerial!=0xffff)
							{
								guilder->m_bf1IsConnected	=	TRUE;
								guilder->m_wJob				=	ava->data.m_wJob;
								guilder->m_wLevel			=	ava->data.m_iLevel;
							}
						}
					}
				}else
				{
					_logc("ERROR FAIL JOIN. NOT FOUND THIS USER OF ACCOUNT [%s]",rPacket->strId);
				}
				break;
			}
		case dSERVERStoWORLD_USER_MOVEMAP_INFO:
			{
				SERVERStoWORLD_USER_MOVEMAP_INFO * rPacket;
				rPacket = (SERVERStoWORLD_USER_MOVEMAP_INFO *)recvpacket;
				
				Avatar_Data *ava = user->GameClientMagager.GetAvatarPnt(rPacket->dwSerial);
				if(!ava || ava->wSerial==0xffff)
				{
					_log("ERROR SERVERStoWORLD_USER_MOVEMAP_INFO : NOT FOUND USER - %d\n",rPacket->dwSerial);
				}
				else
				{
					ava->isNetCafeUser = rPacket->wIsNetCafeUser;
					memcpy(&ava->keepData,&rPacket->upkeepPlayerData,sizeof(CUpkeepPlayerData));
					memcpy(&ava->upkeepStatus,&rPacket->upkeepStatus,sizeof(cActorStatusUpkeep));
					//					if(g_iWorldType>0 && ava->keepData.getPartySerial()<65535)
					//					{
					//						_log("USER MOVEMAP INFO : [%s(%s)] p[%d]",ava->strMaster,ava->data.m_strName,ava->keepData.getPartySerial());
					//					}
				}
				break;
			}

			case dL2W_GVG_AVATAR_ORG_DATA		:
			{
				L2W_GVG_AVATAR_ORG_DATA	*lpPacket	=	(L2W_GVG_AVATAR_ORG_DATA	*)recvpacket;
				
				operateL2W_GVG_AVATAR_ORG_DATA(UserManager.GetClientOfGClientPnt(lpPacket->strName,1),lpPacket);

				break;
			}

			case dL2W_ASK_GVG_AVATAR_ORG_DATA_RESULT	:
			{
				L2W_ASK_GVG_AVATAR_ORG_DATA_RESULT	*lpPacket	=	(L2W_ASK_GVG_AVATAR_ORG_DATA_RESULT	*)recvpacket;

				operateL2W_ASK_GVG_AVATAR_ORG_DATA_RESULT(UserManager.GetClientOfGClientPnt(lpPacket->strName,1),lpPacket);

				break;
			}
			
			case dL2W_TOSS_ASK_GVG_AVATAR_ORG_DATA	:
			{
				L2W_TOSS_ASK_GVG_AVATAR_ORG_DATA	*lpPacket	=	(L2W_TOSS_ASK_GVG_AVATAR_ORG_DATA	*)recvpacket;

				operateL2W_TOSS_ASK_GVG_AVATAR_ORG_DATA(UserManager.GetClientPnt(dLOGIN_SERVER_ID),lpPacket);

				break;
			}

			case dG2W_ASK_GVG_AVATAR_ORG_DATA		:
			{
				operateG2W_ASK_GVG_AVATAR_ORG_DATA(UserManager.GetClientPnt(dLOGIN_SERVER_ID),user,(G2W_ASK_GVG_AVATAR_ORG_DATA *)recvpacket);

				break;
			}

			case dSERVERStoWORLD_TRANS_AVATAR_TO_GVG_SERVER	:
			{
				SERVERStoWORLD_TRANS_AVATAR_TO_GVG_SERVER	*lpPacket	=	(SERVERStoWORLD_TRANS_AVATAR_TO_GVG_SERVER *)recvpacket;

				operateTransAvatarToGVGServerPacket(UserManager.GetClientPnt(dLOGIN_SERVER_ID),&lpPacket->avatarData,user,lpPacket->iWorldIndex,GetADBSocket());

				break;
			}


			case dSERVERStoWORLD_CHANGE_GVG_SERVER_GUILD_MASTER	:
			{
				SERVERStoWORLD_CHANGE_GVG_SERVER_GUILD_MASTER	*lpPacket	=	(SERVERStoWORLD_CHANGE_GVG_SERVER_GUILD_MASTER *)recvpacket;

				operateChangeGVGServerGuildMasterPacket(lpPacket,GetADBSocket());

				break;
			}

			case dSERVERStoWORLD_TRANS_GUILD_TO_GVG_SERVER	:
			{
				SERVERStoWORLD_TRANS_GUILD_TO_GVG_SERVER	*lpPacket	=	(SERVERStoWORLD_TRANS_GUILD_TO_GVG_SERVER *)recvpacket;

				operateTransGuildToGVGServerPacket(UserManager.GetClientPnt(dLOGIN_SERVER_ID),lpPacket,user,GetADBSocket());

				break;
			}

			case dSERVERStoWORLD_LIFE_SIGN	:
			{
				OperateLifeSignPacket(UserManager.GetClientPnt(dLOGIN_SERVER_ID),(SERVERStoWORLD_LIFE_SIGN *)recvpacket);
				break;
			}

			case dSERVERStoWORLD_USER_LOGOUT:
			{
				SERVERStoWORLD_USER_LOGOUT * rPacket;
				rPacket = (SERVERStoWORLD_USER_LOGOUT *)recvpacket;
				
				if(rPacket->type==0)
				{
					Avatar_Data	*lpPlayer	=	UserManager.GetGClientPnt(rPacket->strID);
					
					if	(lpPlayer)
						g_partyManager.disconnectMember(lpPlayer->data.m_strName);
					
					DeleteUser(user, rPacket->strID,rPacket->strIP);
				}
				else
				{
					WORLDtoSERVERS_USER_LOGOUT		sPacket;
					sPacket.base.set(sizeof(WORLDtoSERVERS_USER_LOGOUT),dWORLDtoSERVERS_USER_LOGOUT);
					strcpy(sPacket.strName,rPacket->strName);
					sPacket.iSerialInServer = rPacket->iSerialInServer;
					//					_log("필드 이동 -_-a [%s]를 [%s]서버에서 제외한다. \n",rPacket->strID,user->GetID());
					sPacket.wResult = user->GameClientMagager.Del(rPacket->strID,"dSERVERStoWORLD_USER_LOGOUT 필드이동");
					g_dwUserCount = UserManager.GetAllUserCount();
					user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				}
				break;
			}
		case dSERVERStoWORLD_CHECK_USER_MOVEMAP://이동 가능 여부만 확인
			{	//	SERVERStoWORLD_USER_MOVEMAP 패킷과 형태는 동일하다.
				SERVERStoWORLD_USER_MOVEMAP * rPacket = (SERVERStoWORLD_USER_MOVEMAP *)recvpacket;

				SERVERStoWORLD_USER_MOVEMAP sPacket;// 받은 패킷에 결과만 처리해서 보낸다.
				sPacket.base.set(sizeof(SERVERStoWORLD_USER_MOVEMAP),dWORLDtoSERVERS_CHECK_USER_MOVEMAP);
				sPacket.iSerialInServer = rPacket->iSerialInServer;
				sPacket.iWorldSerial = rPacket->iWorldSerial;
				strcpy(sPacket.strMapName, rPacket->strMapName);
				sPacket.wFieldSerial = rPacket->wFieldSerial;
				sPacket.wGateIndex = rPacket->wGateIndex;
				sPacket.wIsPremiumMember = rPacket->wIsPremiumMember;
				sPacket.wXPos = rPacket->wXPos;
				sPacket.wYPos = rPacket->wYPos;
				sPacket.wCheckResult = dMOVEMAP_RESULT_NOTFIND;

				int iProSerial = 0;//맵의 고유 인덱스
				CClient * pTargetGameServer = NULL;//이동할 게임서버
				if (rPacket->wFieldSerial == 0xffff)
				{
					pTargetGameServer = UserManager.GetClientOfField(rPacket->strMapName,iProSerial,rPacket->wIsPremiumMember,sPacket.wCheckResult);
				}else{
					iProSerial	= rPacket->wFieldSerial;//맵의 고유 인덱스
					pTargetGameServer = UserManager.GetClientOfField(iProSerial,rPacket->wIsPremiumMember,sPacket.wCheckResult);
				}
				if(pTargetGameServer)//이동할 맵이 있다면..
				{
					if(pTargetGameServer->GetUserCount()>=(dGAME_MAX_USER_COUNT-10))//풀인원이라 더 이상 이 서버에 들어갈 수 없다.
						sPacket.wCheckResult = dMOVEMAP_RESULT_FULL;
					else
						sPacket.wCheckResult = dMOVEMAP_RESULT_SUCCESS;
				}
				else
				{
					printf("move map [%d] not found!!\n",sPacket.wFieldSerial);
				}
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				break;
			}
		case dSERVERStoWORLD_USER_MOVEMAP://실제 유저의 이동.
			{
				SERVERStoWORLD_USER_MOVEMAP * rPacket;
				rPacket = (SERVERStoWORLD_USER_MOVEMAP *)recvpacket;
				
				WORLDtoSERVERS_USER_MOVEMAP		sMoveMap;
				memset(&sMoveMap,0,sizeof(WORLDtoSERVERS_USER_MOVEMAP));
				sMoveMap.base.set(sizeof(WORLDtoSERVERS_USER_MOVEMAP),dWORLDtoSERVERS_USER_MOVEMAP);
				sMoveMap.Result			=	dMOVEMAP_RESULT_NOTFIND;
				sMoveMap.iSerialInServer=	rPacket->iSerialInServer;
				sMoveMap.wXPos			=	rPacket->wXPos;
				sMoveMap.wYPos			=	rPacket->wYPos;
				
				int iProSerial = 0;
				CClient * cli = NULL;
				if (rPacket->wFieldSerial == 0xffff)
				{
					cli = UserManager.GetClientOfField(rPacket->strMapName,iProSerial,rPacket->wIsPremiumMember,sMoveMap.Result);
				}else{
					iProSerial	= rPacket->wFieldSerial;	//	맵의 고유 인덱스
					cli = UserManager.GetClientOfField(iProSerial,rPacket->wIsPremiumMember,sMoveMap.Result);
				}
				
				if(cli)
				{
					Avatar_Data * aData = user->GameClientMagager.GetAvatarPnt(rPacket->iWorldSerial);
					if(!aData || aData->wSerial==0xffff)
					{
						_log("[%s]필드는 [%s]의 서버에서 동작중.. \n",rPacket->strMapName,cli->GetAddr());
						_log("ERROR MOVE MAP: this info is not exact. ");
						break;
					}
					if(STRICMP(aData->data.m_strId,"")==0)
					{
						_log("ERROR : id = [%s] [%s] to Map : [%s] \n",aData->data.m_strId,aData->data.m_strName,rPacket->strMapName);
						break;
					}
					aData->isPrimium = rPacket->wIsPremiumMember;
					//	만약 같은 서버에 있지 않다면...
					int	guserial = -3;
					if	(cli->GetSerial()!=serial)
					{
						guserial=cli->GameClientMagager.Add(aData->data.m_strId);
						if(guserial==0xffff)	guserial = -1;
						else if(guserial==0xfffe)
						{
#ifdef _FOR_KOREA
							_log("SSERROR : [%s(%s)]가 양쪽 서버에 다 남아 있다. 확인해봐라..",aData->data.m_strId,aData->data.m_strName);
#endif
							guserial = -2;
						}
					}
					
					switch(guserial)
					{
					case -1:	//	더이상 못들어간다.
						_log("FULL!! Can't get any user!!\n");
						sMoveMap.Result = dMOVEMAP_RESULT_FULL;
						break;
					case -2:	//	같은 이름의 유저가 있다 -_-;
#ifdef _FOR_KOREA
						_log("SERROR : 같은 이름의 유저가 있다 -_- [%s]\n",aData->data.m_strId);
#endif
						cli->GameClientMagager.Del(aData->data.m_strId,"Same Id is exist");
						g_dwUserCount = UserManager.GetAllUserCount();
						sMoveMap.Result = dMOVEMAP_RESULT_USEDNAME;
						break;
					default:
						{
							if(guserial==-3)	strcpy(sMoveMap.strServerAddr,"");
							else				strcpy(sMoveMap.strServerAddr,cli->strConnectaddr[0]);
							
							int	mIdx	=	cli->GetMapIndex(iProSerial);
							if	(mIdx	==	-1)
							{
								_log("this map is not exist - [%d]",iProSerial);
								sMoveMap.Result = dMOVEMAP_RESULT_NOTFIND;
							}
							else
							{
								strcpy(sMoveMap.strMapName, cli->cGSInfo[mIdx].strMapName);
								sMoveMap.wUniqueFieldSerial =	cli->cGSInfo[mIdx].wIndex;
								sMoveMap.Result				=	dMOVEMAP_RESULT_SUCCESS;
								sMoveMap.iSerialInServer	=	rPacket->iSerialInServer;
								sMoveMap.wGateIndex			=	rPacket->wGateIndex;
								aData->data.m_wCurrentField	=	iProSerial;
								//								_log("--------------가려는 필드의 위치  = %d (serial = %d)\n",aData->data.m_wCurrentField,guserial);
								
								if	(rPacket->wGateIndex	!=	0x7fff)
								{
									aData->data.m_iXPos			=	-1;
									aData->data.m_iYPos			=	rPacket->wGateIndex;
								}
//								memcpy(&aData->keepData,rPacket->keepData,sizeof(CUpkeepPlayerData));
//								memcpy(&aData->upkeepStatus,rPacket->upkeepStatus,sizeof(cActorStatusUpkeep));

								if(guserial!=-3)
								{
									if(aData->isOper>6)
									{
										_log("ERROR OPER : [%s(%s)] oper %d",aData->strMaster,aData->data.m_strName,aData->isOper);
										aData->isOper = 0;
									}
									cli->GameClientMagager.writeData((WORD)guserial,&aData->data,0,aData->isOper,aData->isNetCafeUser);
									cli->GameClientMagager.writeKeepData((WORD)guserial,&aData->keepData,TRUE);
									cli->GameClientMagager.writeKeepStatus((WORD)guserial,&aData->upkeepStatus);
									
									aData->bMovingMap = TRUE;
									aData->time = timeGetTime();
								}
								//	파티의 정보를 수정한다.
							}
							break;
						}
					}
				}
				user->AddSendPacket((char *)&sMoveMap,sMoveMap.base.wSize);
				break;
			}
		case dSERVERStoWORLD_SAVEAVA:
			{
				SERVERStoWORLD_SAVEAVA * rPacket;
				rPacket = (SERVERStoWORLD_SAVEAVA *)recvpacket;
				
				SaveData(user,rPacket);
				break;
			}
		case dSERVERStoWORLD_UPDATE_WORLD_VALUE_INFO:
			{
				SERVERStoWORLD_WORLD_VALUE_INFO * rPacket;
				rPacket = (SERVERStoWORLD_WORLD_VALUE_INFO *)recvpacket;
				opeateWorldValue(rPacket->iAddRemoveSet , rPacket->wWorldValueSerial , rPacket->iValue );
			}
			break;
			//┏━START About Party━━━━━━━━━━━━━━━━━━━━
		case dSERVERStoWORLD_JOIN_PARTY_APPLICATION			:
			g_partyManager.operateJoinPartyApplication((void*)user,recvpacket);
			break;

		case dSERVERStoWORLD_JOIN_PARTY_APPLICATION_ANSWER	:
			g_partyManager.operateJoinPartyApplicationAnswer((void*)user,recvpacket);
			break;
		case dSERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY		:
			g_partyManager.operateGuildMemberJoinParty((void*)user,recvpacket);
			break;
		case dSERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY_ANSWER		:
			g_partyManager.operateGuildMemberJoinPartyAnswer((void*)user,recvpacket);
			break;			
		case dSERVERStoWORLD_CREATE_PARTY				:
			g_partyManager.operateCreatePartyPacket((void*)user,recvpacket);
			break;
		case dSERVERStoWORLD_JOIN_PARTY				:
			g_partyManager.operateJoinPartyPacket((void*)user,recvpacket);
			break;
		case dSERVERStoWORLD_PARTY_WORK				:
			g_partyManager.operatePartyWorkPacket((void*)user,recvpacket);
			break;
// 		case SERVERStoWORLD_UNION_PARTY_WORK		:
// 			g_UnionpartyManager.operateUnionPartyWorkPacket((void*)user,recvpacket);
// 			break;
		case dSERVERStoWORLD_RENAME_PARTYNAME			:	//	파티 이름 변경을 요청했다.
			g_partyManager.operateRenamePartyPacket((void*)user,recvpacket);
			break;
		case dSERVERStoWORLD_ASK_PARTY_LIST				:	//	파티 리스트를 요청했다.
			g_partyManager.operateAskPartyListPacket((void*)user,recvpacket);
			break;
		case dSERVERStoWORLD_ASK_PARTYINFO				:	//	파티 정보를 요청했다.
			g_partyManager.operateAskPartyInfoPacket((void*)user,recvpacket);
			break;
		case dSERVERStoWORLD_CHANGE_PARTY_STATUS		:	//	파티가 해체 됐다.
			g_partyManager.operateChangePartyStatusPacket((void*)user,recvpacket);
			break;
		case dSERVERStoWORLD_CHANGE_PARTY_MEMBER_INFO	:	//	파티원 레벨이 올랐다.
			g_partyManager.operateUpdatePartyMemberInfoPacket((void*)user,recvpacket);
			break;
		case dSERVERStoWORLD_CHECK_PARTY_INFO_BY_ACTOR	:	//	캐릭터가 파티 정보를 체크했다.
			g_partyManager.operateCheckPartyInfoByActorPacket((void*)user,recvpacket);
			break;
		case dSERVERStoWORLD_MOVE_FIELD_PARTY_MEMBER	:	//	파티 멤버가 필드를 이동했다.
			g_partyManager.operateMoveFieldPartyMemberPacket((void*)user,recvpacket);
			break;
		case dSERVERStoWORLD_PARTY_CHAT:
			g_partyManager.operatePartyChatPacket((void*)user,recvpacket);
			break;
		case dSERVERStoWORLD_PARTY_ENTER_IF:
			g_partyManager.operateEnterIFPartyPacket((void*)user,recvpacket);
			break;
			//┗━END About Party━━━━━━━━━━━━━━━━━━━━
		case dSERVERStoWORLD_CREATE_UNION_PARTY:
			g_UnionpartyManager.operateCreateUnionPartyPacket((void*)user,recvpacket);
			break;
			//┏━START ABOUT GUILD━━━━━━━━━━━━━━━━━━━━

		case dSERVERStoWORLD_TRANS_AVATAR_TO_GVG_RESULT	:
		{
			SERVERStoWORLD_TRANS_AVATAR_TO_GVG_RESULT	*lpPacket	=	(SERVERStoWORLD_TRANS_AVATAR_TO_GVG_RESULT	*)recvpacket;

			CClient	*lpGameServer	= UserManager.GetClientOfGClientPnt(lpPacket->strName,1);

			if	(!lpGameServer)
				break;

			operateTransAvatarToGVGResultPacket(lpGameServer,lpPacket);

			break;
		}

		case dSERVERStoWORLD_TRANS_GUILD_TO_GVG_RESULT	:
		{
			SERVERStoWORLD_TRANS_GUILD_TO_GVG_RESULT	*lpPacket	=	(SERVERStoWORLD_TRANS_GUILD_TO_GVG_RESULT	*)recvpacket;

			CClient	*lpGameServer	= UserManager.GetClientOfGClientPnt(lpPacket->strMasterName,1);

			operateTransGuildToGVGResultPacket(lpGameServer,lpPacket);

			break;
		}

		case dSERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_GOLD		:
		{
			SERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_GOLD	*lpPacket	=	(SERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_GOLD *)recvpacket;
			cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpPacket->iGuild);

			if	(lpGuild	==	NULL)
				break;

			lpGuild->m_aiWithdrawGoldLimitPerDay[lpPacket->wRank]		=	lpPacket->iGold;

			WORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_GOLD	packet;

			packet.base.set(sizeof(packet),dWORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_GOLD);

			memcpy(&packet,lpPacket,sizeof(packet));
			packet.base.wType	=	dWORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_GOLD;

			SendDataAllGameServer((char *)&packet,packet.base.wSize);

			break;
		}

		case dSERVERStoWORLD_RECORD_GUILD_HONOR_POINT			:
		{
			SERVERStoWORLD_RECORD_GUILD_HONOR_POINT	*lpPacket	=	(SERVERStoWORLD_RECORD_GUILD_HONOR_POINT	*)recvpacket;

			cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpPacket->iGuild);

			if	(lpGuild	==	NULL)
				break;

			lpGuild->m_iHonorPoint	=	max(lpPacket->iGuildHonorPoint,lpGuild->m_iHonorPoint);

			{
				SG_ETC_WORK	packet;

				packet.base.set(sizeof(packet),dSG_ETC_WORK);

				packet.wWork		=	eEW_RECORD_GUILD_HONOR_POINT;
				packet.aiValue[0]	=	lpPacket->iGuildHonorPoint>>16;
				packet.aiValue[1]	=	lpPacket->iGuildHonorPoint&0xffff;
				packet.aiValue[2]	=	lpGuild->m_bf2TryGuildPointBattleCountPerWeek;

				SendGuildPacketToBCS(&packet,lpGuild->m_wSerial);
			}
			
			SaveGuildBaseData(lpGuild);

			WORLDtoSERVERS_RECORD_GUILD_HONOR_POINT	packet;

			packet.base.set(sizeof(packet),dWORLDtoSERVERS_RECORD_GUILD_HONOR_POINT);

			packet.iGuild			=	lpPacket->iGuild;
			packet.iGuildHonorPoint	=	lpPacket->iGuildHonorPoint;
			packet.bf1IsVictoryPoint=	FALSE;
			packet.bf2TryGuildPointBattleCountPerWeek	=	lpGuild->m_bf2TryGuildPointBattleCountPerWeek;

			SendDataAllGameServer((char *)&packet,packet.base.wSize);

			break;
		}

		case dSERVERStoWORLD_ADD_RELATED_PLACE_OF_GUILD			:
		{
			SERVERStoWORLD_ADD_RELATED_PLACE_OF_GUILD	*lpPacket	=	(SERVERStoWORLD_ADD_RELATED_PLACE_OF_GUILD *)recvpacket;
			cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpPacket->iGuild);

			if	(lpGuild	==	NULL)
				break;

			WORLDtoSERVERS_ADD_RELATED_PLACE_OF_GUILD	packet;
			
			packet.base.set(sizeof(packet),dWORLDtoSERVERS_ADD_RELATED_PLACE_OF_GUILD);

			packet.iGuild		=	lpPacket->iGuild;
			packet.iFieldSerial	=	lpPacket->iFieldSerial;
			packet.iGold		=	lpPacket->iGold;
			packet.wIsAdd		=	lpPacket->wIsAdd;
			packet.wIsSuccess	=	TRUE;
			strcpy(packet.strMemberName,lpPacket->strMemberName);

			if	(lpPacket->wIsAdd)
			{
				if	(!lpGuild->addRelatedPlace(lpPacket->iFieldSerial,lpPacket->iGold))
				{
					packet.wIsSuccess	=	FALSE;
					user->AddSendPacket((char *)&packet,packet.base.wSize);
					break;
				}
			}
			else
			{
				if	(!lpGuild->removeRelatedPlace(lpPacket->iFieldSerial))
				{
					packet.wIsSuccess	=	FALSE;
					user->AddSendPacket((char *)&packet,packet.base.wSize);
					break;
				}
			}

			SendDataAllGameServer((char *)&packet,packet.base.wSize);
			break;
		}

		case dSERVERStoWORLD_STORE_GUILD_INVENTORY_ITEM			:
		{
			SERVERStoWORLD_STORE_GUILD_INVENTORY_ITEM	*lpPacket	=	(SERVERStoWORLD_STORE_GUILD_INVENTORY_ITEM *)recvpacket;
			cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpPacket->iGuild);

			WORLDtoSERVERS_STORE_GUILD_INVENTORY_ITEM	packet;

			packet.base.set(sizeof(packet),dWORLDtoSERVERS_STORE_GUILD_INVENTORY_ITEM);
			memcpy(&packet.item,&lpPacket->item,sizeof(packet.item));

			packet.wIsFailed			=	FALSE;
			packet.iGuild				=	lpPacket->iGuild;
			packet.iGuildInventorySlot	=	lpPacket->iGuildInventorySlot;
			packet.wInventorySlot		=	lpPacket->wInventorySlot;
			strcpy(packet.strMemberName,lpPacket->strMemberName);

			if	(lpGuild	==	NULL)
			{
				packet.wIsFailed		=	TRUE;
				user->AddSendPacket((char *)&packet,packet.base.wSize);
				break;
			}

			if	(!lpGuild->storeItem(lpPacket->iGuildInventorySlot,&lpPacket->item))
			{
				packet.wIsFailed		=	TRUE;
				user->AddSendPacket((char *)&packet,packet.base.wSize);
				break;
			}

			{
				SG_STORE_GUILD_INVENTORY_ITEM	packet;

				packet.base.set(sizeof(packet),dSG_STORE_GUILD_INVENTORY_ITEM);
				memcpy(&packet.item,&lpPacket->item,sizeof(lpPacket->item));
				packet.wGuildInventorySlot	=	lpPacket->iGuildInventorySlot;

				SendGuildPacketToBCS(&packet,lpGuild->m_wSerial);
			}

			SendDataAllGameServer((char *)&packet,packet.base.wSize);

			break;
		}

		case dSERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_ITEM		:
		{
			SERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_ITEM	*lpPacket	=	(SERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_ITEM *)recvpacket;
			cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpPacket->iGuild);

			WORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_ITEM	packet;

			packet.base.set(sizeof(packet),dWORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_ITEM);
			memcpy(&packet.item,&lpPacket->item,sizeof(packet.item));

			packet.wIsFailed			=	FALSE;
			packet.iGuild				=	lpPacket->iGuild;
			packet.iGuildInventorySlot	=	lpPacket->iGuildInventorySlot;
			strcpy(packet.strMemberName,lpPacket->strMemberName);

			if	(lpGuild	==	NULL)
			{
				packet.wIsFailed		=	TRUE;
				user->AddSendPacket((char *)&packet,packet.base.wSize);
				break;
			}

			if	(!lpGuild->removeItem(lpPacket->iGuildInventorySlot,&lpPacket->item))
			{
				packet.wIsFailed		=	TRUE;
				user->AddSendPacket((char *)&packet,packet.base.wSize);
				break;
			}

			{
				SG_REMOVE_GUILD_INVENTORY_ITEM	packet;

				packet.base.set(sizeof(packet),dSG_REMOVE_GUILD_INVENTORY_ITEM);
				packet.wGuildInventorySlot	=	lpPacket->iGuildInventorySlot;
				memcpy(&packet.item,&lpPacket->item,sizeof(lpPacket->item));

				SendGuildPacketToBCS(&packet,lpGuild->m_wSerial);
			}

			SendDataAllGameServer((char *)&packet,packet.base.wSize);
			break;
		}

		case dSERVERStoWORLD_STORE_GUILD_INVENTORY_GOLD			:
		{
			SERVERStoWORLD_STORE_GUILD_INVENTORY_GOLD	*lpPacket	=	(SERVERStoWORLD_STORE_GUILD_INVENTORY_GOLD *)recvpacket;
			cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpPacket->iGuild);

			WORLDtoSERVERS_STORE_GUILD_INVENTORY_GOLD	packet;

			packet.base.set(sizeof(packet),dWORLDtoSERVERS_STORE_GUILD_INVENTORY_GOLD);
			packet.wIsFailed			=	FALSE;
			packet.iGuild				=	lpPacket->iGuild;
			packet.iGold				=	lpPacket->iGold;
			strcpy(packet.strMemberName,lpPacket->strMemberName);

			if	(lpGuild	==	NULL)
			{
				packet.wIsFailed		=	TRUE;
				user->AddSendPacket((char *)&packet,packet.base.wSize);
				break;
			}

			if	(!lpGuild->storeGold(lpPacket->iGold))
			{
				packet.wIsFailed		=	TRUE;
				user->AddSendPacket((char *)&packet,packet.base.wSize);
				break;
			}

			{
				SG_STORE_GUILD_INVENTORY_GOLD	packet;

				packet.base.set(sizeof(packet),dSG_STORE_GUILD_INVENTORY_GOLD);
				packet.iGold	=	lpPacket->iGold;

				SendGuildPacketToBCS(&packet,lpPacket->iGuild);
			}

			SendDataAllGameServer((char *)&packet,packet.base.wSize);
			break;
		}

		case dSERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_GOLD		:
		{
			SERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_GOLD	*lpPacket	=	(SERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_GOLD *)recvpacket;
			cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpPacket->iGuild);

			WORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_GOLD	packet;

			packet.base.set(sizeof(packet),dWORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_GOLD);
			packet.wIsFailed			=	FALSE;
			packet.iGuild				=	lpPacket->iGuild;
			packet.iGold				=	lpPacket->iGold;
			strcpy(packet.strMemberName,lpPacket->strMemberName);

			if	(lpGuild	==	NULL)
			{
				packet.wIsFailed		=	TRUE;
				user->AddSendPacket((char *)&packet,packet.base.wSize);
				break;
			}

			if	(!lpGuild->removeGold(lpPacket->iGold))
			{
				packet.wIsFailed		=	TRUE;
				user->AddSendPacket((char *)&packet,packet.base.wSize);
				break;
			}

			{
				SG_REMOVE_GUILD_INVENTORY_GOLD	packet;

				packet.base.set(sizeof(packet),dSG_REMOVE_GUILD_INVENTORY_GOLD);
				packet.iGold	=	lpPacket->iGold;

				SendGuildPacketToBCS(&packet,lpPacket->iGuild);
			}

			SendDataAllGameServer((char *)&packet,packet.base.wSize);
			break;
		}

		case dSERVERStoWORLD_CHANGE_GUILD_INVENTORY_ITEM_PLACE	:
		{
			SERVERStoWORLD_CHANGE_GUILD_INVENTORY_ITEM_PLACE	*lpPacket	=	(SERVERStoWORLD_CHANGE_GUILD_INVENTORY_ITEM_PLACE *)recvpacket;
			cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpPacket->iGuild);

			WORLDtoSERVERS_CHANGE_GUILD_INVENTORY_ITEM_PLACE	packet;

			packet.base.set(sizeof(packet),dWORLDtoSERVERS_CHANGE_GUILD_INVENTORY_ITEM_PLACE);
			packet.wIsFailed	=	FALSE;
			packet.iGuild		=	lpPacket->iGuild;
			packet.iSlot1		=	lpPacket->iSlot1;
			packet.iSlot2		=	lpPacket->iSlot2;
			strcpy(packet.strMemberName,lpPacket->strMemberName);

			if	(lpGuild	==	NULL)
			{
				packet.wIsFailed		=	TRUE;
				user->AddSendPacket((char *)&packet,packet.base.wSize);
				break;
			}

			if	(!lpGuild->changeItemPlace(lpPacket->iSlot1,lpPacket->iSlot2))
			{
				packet.wIsFailed		=	TRUE;
				user->AddSendPacket((char *)&packet,packet.base.wSize);
				break;
			}

			{
				SG_CHANGE_GUILD_INVENTORY_ITEM_PLACE	packet;

				packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_INVENTORY_ITEM_PLACE);
				packet.wSlot1	=	lpPacket->iSlot1;
				packet.wSlot2	=	lpPacket->iSlot2;

				SendGuildPacketToBCS(&packet,lpPacket->iGuild);
			}

			SendDataAllGameServer((char *)&packet,packet.base.wSize);
			break;
		}
		
		case dSERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_ITEM		:
		{
			SERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_ITEM	*lpPacket	=	(SERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_ITEM *)recvpacket;
			cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpPacket->iGuild);

			if	(lpGuild	==	NULL)
				break;

			lpGuild->m_aWithdrawItemCountPerDay[lpPacket->wStorage][lpPacket->wRank]		=	(BYTE)lpPacket->wItemCount;

			WORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_ITEM	packet;

			memcpy(&packet,lpPacket,sizeof(packet));
			packet.base.wType	=	dWORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_ITEM;

			SendDataAllGameServer((char *)&packet,packet.base.wSize);

			break;
		}

		case dSERVERStoWORLD_CHANGE_GI_STORAGE_SIZE				:
		{
			SERVERStoWORLD_CHANGE_GI_STORAGE_SIZE			*lpPacket	=	(SERVERStoWORLD_CHANGE_GI_STORAGE_SIZE *)recvpacket;
			cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpPacket->iGuild);

			if	(lpGuild	==	NULL)
				break;

			memcpy(lpGuild->m_abInventorySize,lpPacket->abStorageSize,sizeof(lpPacket->abStorageSize));


			{
				SG_ETC_WORK	packet;

				packet.base.set(sizeof(packet),dSG_ETC_WORK);
				packet.wWork		=	eEW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE;
				packet.aiValue[0]	=	lpPacket->abStorageSize[0];
				packet.aiValue[1]	=	lpPacket->abStorageSize[1];
				packet.aiValue[2]	=	lpPacket->abStorageSize[2];
				packet.aiValue[3]	=	lpPacket->abStorageSize[3];

				SendGuildPacketToBCS(&packet,lpPacket->iGuild);
			}


			WORLDtoSERVERS_CHANGE_GI_STORAGE_SIZE	packet;

			memcpy(&packet,lpPacket,sizeof(packet));
			packet.base.wType	=	dWORLDtoSERVERS_CHANGE_GI_STORAGE_SIZE;

			SendDataAllGameServer((char *)&packet,packet.base.wSize);

			break;
		}
		
		case dSERVERStoWORLD_SET_GUILD_HALL_LEVEL	:
			{
				SERVERStoWORLD_SET_GUILD_HALL_LEVEL	*lpPacket	=	(SERVERStoWORLD_SET_GUILD_HALL_LEVEL *)recvpacket;

				cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpPacket->strName);

				if	(lpGuild	==	NULL	||	lpGuild->m_wHallLevel)
					break;

				lpGuild->m_wHallLevel		=	lpPacket->wLevel;
				lpGuild->m_wGuildHallShape	=	0;

				WORLDtoSERVERS_SET_GUILD_HALL_LEVEL	packet;

				packet.base.set(sizeof(packet),dWORLDtoSERVERS_SET_GUILD_HALL_LEVEL);
				packet.iHallLevel		=	lpPacket->wLevel;
				packet.iGuildSerial		=	lpGuild->m_wSerial;

				SendDataAllGameServer((char *)&packet,packet.base.wSize);

				break;
			}
		case dSERVERStoWORLD_SET_GUILD_HALL_ROEN_GOOD_WILL:
			{
				SERVERStoWORLD_SET_GUILD_HALL_ROEN_GOOD_WILL	*lpPacket	=	(SERVERStoWORLD_SET_GUILD_HALL_ROEN_GOOD_WILL *)recvpacket;
				
				cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpPacket->strName);
				
				if	(lpGuild	==	NULL)
					break;
				
				lpGuild->m_bf3GuildGoodwillForRoen		=	lpPacket->wGoodWill;
				
				WORLDtoSERVERS_SET_GUILD_HALL_ROEN_GOOD_WILL	packet;
				
				packet.base.set(sizeof(packet),dWORLDtoSERVERS_SET_GUILD_HALL_ROEN_GOOD_WILL);
				packet.iGoodWill		=	lpPacket->wGoodWill;
				packet.iGuildSerial		=	lpGuild->m_wSerial;
				
				SendDataAllGameServer((char *)&packet,packet.base.wSize);
				
				break;
			}
		case dSERVERStoWORLD_USER_CREATEGUILD:
			{
				SERVERStoWORLD_USER_CREATEGUILD * rPacket;
				rPacket = (SERVERStoWORLD_USER_CREATEGUILD *)recvpacket;
				
				WORD wResult = 0xffff;
				cGuild	* isUsedNameGuild;
				WORLDtoSERVERS_USER_CREATEGUILD	rGuild;	//	실패할 경우
				rGuild.base.set(sizeof(WORLDtoSERVERS_USER_CREATEGUILD),dWORLDtoSERVERS_USER_CREATEGUILD);
				cGuild * tmpGuild = g_guildManager.GetGuildPnt(rPacket->strName);
				if(tmpGuild && tmpGuild->wSerial!=0xffff)
				{
					rGuild.Result = dCREATEGUILD_RESULT_SAMENAME;
					_log(" ERROR GUILD SAMENAME : [%s] [%s]",rPacket->strName,tmpGuild->m_strName);
					goto GOTO_DONOTCREATEGUILD;
				}
				WDMSG_CREATEGUILD					creGuild;
				creGuild.base.set(sizeof(WDMSG_CREATEGUILD),dWDMSG_CREATEGUILD);
				strcpy(creGuild.strName,rPacket->strName);
				strcpy(creGuild.strMasterId,rPacket->strMasterId);
				strcpy(creGuild.strMasterName,rPacket->strMasterName);
				strcpy(creGuild.strSubMasterName,rPacket->strSubMasterName);
				creGuild.wMasterJob = rPacket->wMasterJob;
				creGuild.wMasterLevel = rPacket->wMasterLevel;
				creGuild.dwGameServerSerial = user->GetSerial();
				//	혹시 같은 이름이 있나?
				isUsedNameGuild = g_guildManager.GetGuildPnt(rPacket->strName);
				if(isUsedNameGuild && isUsedNameGuild->wSerial!=0xffff)
					goto GOTO_DONOTCREATEGUILD;
				//	빈 길드의 시리얼을 얻어온다.
				creGuild.wIndex = g_guildManager.GetBlankGuildSerial();
				
				if(creGuild.wIndex == 0xffff)
					goto GOTO_DONOTCREATEGUILD;
				//	메니져에 임시 등록하고 DB에 패킷을 보내준다.
				g_guildForCreate.reset();
				g_guildForCreate.m_wSerial	=	creGuild.wIndex;
				g_guildForCreate.wPre		=	1;
				
				strcpy(g_guildForCreate.m_strName,creGuild.strName);
				strcpy(g_guildForCreate.m_strMasterName,creGuild.strMasterName);
				wResult = g_guildManager.Add(&g_guildForCreate);
				if(wResult == 0xffff)
				{
					rGuild.Result = dCREATEGUILD_RESULT_FULL;
GOTO_DONOTCREATEGUILD:
					strcpy(rGuild.strGuildName,rPacket->strName);
					strcpy(rGuild.strMasterName,rPacket->strMasterName);
					rGuild.wGuildSerial = 0xffff;
					user->AddSendPacket((char *)&rGuild,rGuild.base.wSize);
					break;
				}
				//				_logf("[GUILD] [%s(serial %d)]  [%s] is request for CREATE GUILD",g_guildForCreate.m_strName,g_guildForCreate.m_wSerial,g_guildForCreate.m_strMasterName);
				//	패킷을 DB에 보낸다.
				GetADBSocket()->SendPacket((char *)&creGuild,creGuild.base.wSize);
				break;
			}
		case dSERVERStoWORLD_USER_DELETEGUILD:
			{
				SERVERStoWORLD_USER_DELETEGUILD * rPacket;
				rPacket = (SERVERStoWORLD_USER_DELETEGUILD *)recvpacket;
				
				cGuild * delGuild = g_guildManager.GetGuildPntBySerial(rPacket->wGuildSerial);
				
				if (delGuild == NULL || delGuild->wSerial==0xffff)
				{
					sendGSPValidGuild(NULL,rPacket->wGuildSerial);
					break;
				}
				
				if(delGuild && delGuild->wSerial!=0xffff)	delGuild->wPre = 1;
				//	DBC에 삭제 요청을 한다. 
				//				_logf("[GUILD] [%s(serial %d)]  [%s] is requet for DELETE GUILD",rPacket->strName,rPacket->wGuildSerial,rPacket->strRequestMan);
				DeleteGuildPacket(user->GetSerial(),rPacket->wGuildSerial,rPacket->strName,rPacket->strRequestMan);
				break;
			}
		case dSERVERStoWORLD_USER_JOINGUILD:
			{
				SERVERStoWORLD_USER_JOINGUILD * rPacket;
				rPacket = (SERVERStoWORLD_USER_JOINGUILD *)recvpacket;
				
				cGuild * guild = g_guildManager.GetGuildPnt(rPacket->strName);
				WORLDtoSERVERS_USER_JOINGUILD	failJoinGuild;
				failJoinGuild.base.set(sizeof(WORLDtoSERVERS_USER_JOINGUILD),dWORLDtoSERVERS_USER_JOINGUILD);
				strcpy(failJoinGuild.strMasterName,rPacket->strMasterName);
				strcpy(failJoinGuild.strName,rPacket->strName);
				strcpy(failJoinGuild.strTargetName,rPacket->strTargetName);
				
				if(!guild || guild->wSerial==0xffff)
				{
					failJoinGuild.wResult = dJOINGUILD_RESULT_NOTFIND;
					user->AddSendPacket((char *)&failJoinGuild,failJoinGuild.base.wSize);
					break;
				}
				WORD result = guild->CheckJoinMember(rPacket->strMasterName,rPacket->strTargetName);
				if(result>0)
				{
					failJoinGuild.wResult = result;
					user->AddSendPacket((char *)&failJoinGuild,failJoinGuild.base.wSize);
					break;
				}
				//	DB에 넘겨준다.
				WDMSG_JOINGUILD		jGuild;
				jGuild.base.set(sizeof(WDMSG_JOINGUILD),dWDMSG_JOINGUILD);
				jGuild.dwGameServerSerial = user->GetSerial();
				jGuild.wIndex = guild->m_wSerial;
				strcpy(jGuild.strName,guild->m_strName);
				strcpy(jGuild.strMaster,rPacket->strMasterName);
				strcpy(jGuild.strTargetId,rPacket->strTargetId);
				strcpy(jGuild.strTargetName,rPacket->strTargetName);
				jGuild.wRank = rPacket->wRank;
				jGuild.wJob = rPacket->wJob;
				jGuild.wLevel = rPacket->wLevel;
				GetADBSocket()->SendPacket((char *)&jGuild,jGuild.base.wSize);
				//				_logf("[GUILD] [%s(serial %d)] [%s] is request for [%s]'s joining",jGuild.strName,jGuild.wIndex,jGuild.strMaster,jGuild.strTargetName);
				break;
			}
		case dSERVERStoWORLD_USER_EXITGUILD:
			{
				SERVERStoWORLD_USER_EXITGUILD * rPacket;
				rPacket = (SERVERStoWORLD_USER_EXITGUILD *)recvpacket;
				
				WORLDtoSERVERS_USER_EXITGUILD	failExitGuild;
				failExitGuild.base.set(sizeof(WORLDtoSERVERS_USER_EXITGUILD),dWORLDtoSERVERS_USER_EXITGUILD);
				strcpy(failExitGuild.strMasterName,rPacket->strMasterName);
				failExitGuild.wGuildSerial = rPacket->wGuildSerial;
				strcpy(failExitGuild.strTargetName,rPacket->strTargetName);
				
				cGuild * guild = g_guildManager.GetGuildPntBySerial(rPacket->wGuildSerial);
				
				if	(!guild || guild->wSerial==0xffff)
				{
					sendGSPValidGuild(NULL,rPacket->wGuildSerial);

					break;
				}
				
				WORD result = guild->CheckExitMember(rPacket->strMasterName,rPacket->strTargetName);

				if	(result	>	0)
				{
					failExitGuild.wResult = result;
					user->AddSendPacket((char *)&failExitGuild,failExitGuild.base.wSize);
					break;
				}
				//	DB에 넘겨준다.
				WDMSG_EXITGUILD		eGuild;
				eGuild.base.set(sizeof(WDMSG_EXITGUILD),dWDMSG_EXITGUILD);
				eGuild.dwGameServerSerial = user->GetSerial();
				eGuild.wIndex = guild->m_wSerial;
				strcpy(eGuild.strName,guild->m_strName);
				strcpy(eGuild.strMaster,rPacket->strMasterName);
				strcpy(eGuild.strTargetName,rPacket->strTargetName);
				GetADBSocket()->SendPacket((char *)&eGuild,eGuild.base.wSize);
				//_log("[GUILD] [%s(serial %d)]  [%s] is request for [%s]'s guild leaving",eGuild.strName,eGuild.wIndex,eGuild.strMaster,eGuild.strTargetName);
				break;
			}
		case dSERVERStoWORLD_UPDATE_NOTICE:
			{
				SERVERStoWORLD_UPDATE_NOTICE * rPacket;
				rPacket = (SERVERStoWORLD_UPDATE_NOTICE *)recvpacket;
				
				cGuild * guild = g_guildManager.GetGuildPntBySerial(rPacket->wGuildSerial);

				if	(!guild || guild->wSerial==0xffff)
				{
					sendGSPValidGuild(NULL,rPacket->wGuildSerial);
					break;
				}
				if	(!guild->isSubOrMaster(rPacket->strMaster))
					break;

				guild->writeNotice(rPacket->wType,rPacket->strNotice);

				{
					WORLDtoSERVERS_GUILD_NOTICE			sPacket;
					sPacket.base.set(sizeof(WORLDtoSERVERS_GUILD_NOTICE),dWORLDtoSERVERS_GUILD_NOTICE);
					sPacket.wGuildSerial = guild->m_wSerial;
					strcpy(sPacket.strNotice,guild->m_strNotice);
					SendDataAllGameServer((char *)&sPacket,sPacket.base.wSize);
				}

				{
					SG_CHANGE_GUILD_NOTICE		packet;	

					packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_NOTICE);
					strcpy(packet.strGuildNotice,guild->m_strNotice);
					packet.wGuildSerial	=	rPacket->wGuildSerial;

					SendGuildPacketToBCS(&packet,rPacket->wGuildSerial);
				}

				break;
			}
		case dSERVERStoWORLD_UPDATE_EXPRATE:
			{
				SERVERStoWORLD_UPDATE_EXPRATE * rPacket;
				rPacket = (SERVERStoWORLD_UPDATE_EXPRATE *)recvpacket;
				
				cGuild * guild = g_guildManager.GetGuildPntBySerial(rPacket->wGuildSerial);
				
				if	(!guild || guild->wSerial==0xffff)
				{
					sendGSPValidGuild(NULL,rPacket->wGuildSerial);
					break;
				}

				if	(!guild->isMaster(rPacket->strMaster))
					break;

				guild->m_wExpTaxRate	=	rPacket->wRateExp;
				
				{
					WORLDtoSERVERS_GUILD_EXPRATE	sPacket;
					sPacket.base.set(sizeof(WORLDtoSERVERS_GUILD_EXPRATE),dWORLDtoSERVERS_GUILD_EXPRATE);
					
					sPacket.wGuildSerial	=	guild->m_wSerial;
					sPacket.wRateExp		=	guild->m_wExpTaxRate;
					
					SendDataAllGameServer((char *)&sPacket,sPacket.base.wSize);
				}

				{
					SG_CHANGE_GUILD_TAX_RATE	packet;	

					packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_TAX_RATE);
					packet.wTaxRate		=	rPacket->wRateExp;
					packet.wGuildSerial	=	rPacket->wGuildSerial;

					SendGuildPacketToBCS(&packet,rPacket->wGuildSerial);
				}
				break;
			}

		case dSERVERStoWORLD_MAKE_GUILD_LEVEL	:
			{
				operateMakeGuildLevelPacket((SERVERStoWORLD_MAKE_GUILD_LEVEL *)recvpacket);
				break;
			}

		case dSERVERStoWORLD_SIMPLE_WORK	:
			{
				CClient	*loginServer	=	UserManager.GetClientPnt(dLOGIN_SERVER_ID);

				OperateSimpleWork((SERVERStoWORLD_SIMPLE_WORK *)recvpacket,user,loginServer,GetIDDBSocket());
				break;
			}

		case dSERVERStoWORLD_USER_CHANGERANK:
			{
				SERVERStoWORLD_USER_CHANGERANK * rPacket;
				rPacket = (SERVERStoWORLD_USER_CHANGERANK *)recvpacket;
				
				cGuild * guild = g_guildManager.GetGuildPntBySerial(rPacket->wGuildSerial);
				
				if	(!guild || guild->wSerial==0xffff)
				{
					sendGSPValidGuild(NULL,rPacket->wGuildSerial);
					break;
				}
				
				int		 result =	1;
				WORD	_wRank	=	rPacket->wRank;
				WORD	_wRank2	=	dGUILD_CLASS_MEMBER;

				switch(rPacket->wType)
				{
				case dGUILD_APPONT:
					{
						if	(rPacket->wRank==dGUILD_CLASS_MASTER)
						{
							if	(g_iWorldType	==	2 || strcmp(rPacket->strMaster,"#admin") == 0)				//테섭이면 바로 원로가 된다.-_-;
								_wRank2	=	dGUILD_CLASS_ELDER;	//	원로.
							else
							{
								if	(cSRVUTIL::CalcDayOnToday(guild->m_timeBeginGuildMaster.year+2000,guild->m_timeBeginGuildMaster.month,guild->m_timeBeginGuildMaster.day) < dGUILD_MINLIMITPERIOD_FORCHANGINGELDER)
									_wRank2 = dGUILD_CLASS_MEMBER;
								else
									_wRank2 = dGUILD_CLASS_ELDER;	//	원로.
							}
						}
					}
					break;
				case dGUILD_DISMISSAL:
					{
						if	(rPacket->wRank==dGUILD_CLASS_SUBMASTER)
						{
							if	(guild->isMaster(rPacket->strMaster) && guild->isSubMaster(rPacket->strTarget))
								result = 1;
						}
						if	(rPacket->wRank==dGUILD_CLASS_PARLIAMENT)
						{
							if	(guild->isMaster(rPacket->strMaster))
								result = 1;
						}
						break;
					}
				default:
					{
						_logf("ERROR incorrected guild rank changing -> type : %d, guild : %s, master : %s",rPacket->wType,guild->m_strName,rPacket->strMaster);
						break;
					}
				}

				strcpy(rPacket->strMaster,guild->m_strMasterName);
				
				if	(result)
				{
					//					_logf("[GUILD] [%s] is change [%s], [rank %d]",rPacket->strMaster,rPacket->strTarget,rPacket->wRank);
					WDMSG_CHANGERANK	cRank;
					cRank.base.set(sizeof(WDMSG_CHANGERANK),dWDMSG_CHANGERANK);
					cRank.dwGameServerSerial = user->GetSerial();
					cRank.wIndex = guild->m_wSerial;
					cRank.wRank  = _wRank;
					strcpy(cRank.strName,guild->m_strName);
					strcpy(cRank.strTarget,rPacket->strTarget);
					cRank.wType = rPacket->wType;
					strcpy(cRank.strMaster,rPacket->strMaster);
					cRank.wRank2		=	_wRank2;
					cRank.wReason		=	rPacket->wType;	
					//					cRank.wRank3 = _wRank3;
					GetADBSocket()->SendPacket((char *)&cRank,cRank.base.wSize);

					if	(rPacket->wRank==dGUILD_CLASS_MASTER)
					{
						int tomorrow = cSRVUTIL::GetNextTime((guild->m_timeLastConnectedMasterTime.year+2000)*10000 + guild->m_timeLastConnectedMasterTime.month*100 + guild->m_timeLastConnectedMasterTime.day);
						guild->m_timeLastConnectedMasterTime.year = (tomorrow /10000)%100;
						guild->m_timeLastConnectedMasterTime.month = (tomorrow %10000)/100;
						guild->m_timeLastConnectedMasterTime.day = tomorrow %100;
						SaveGuildBaseData(guild);
					}
				}
				else
				{
					_logf("[GUILD] [%s]is fail changing [%s] [rank %d]!!",rPacket->strMaster,rPacket->strTarget,rPacket->wRank);
					WORLDtoSERVERS_USER_CHANGERANK	sPacket;
					sPacket.base.set(sizeof(WORLDtoSERVERS_USER_CHANGERANK),dWORLDtoSERVERS_USER_CHANGERANK);
					strcpy(sPacket.strMaster,rPacket->strMaster);
					strcpy(sPacket.strTarget,rPacket->strTarget);
					sPacket.wGuildSerial = rPacket->wGuildSerial;
					sPacket.wRank = rPacket->wRank;
					sPacket.wResult = dCHANGERANK_RESULT_NOTACOUNT;
					sPacket.wType = rPacket->wType;
					user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				}
				break;
			}
		case dSERVERStoWORLD_CHECK_GUILD_CHECKSUM	:
			{
				CheckGuildCheckSum(user,(SERVERStoWORLD_CHECK_GUILD_CHECKSUM *)recvpacket);
				break;
			}
		case dSERVERStoWORLD_UPDATE_SERVER_EXP:
			{
				SERVERStoWORLD_UPDATE_SERVER_EXP * rPacket;
				rPacket = (SERVERStoWORLD_UPDATE_SERVER_EXP *)recvpacket;
				
				WORLDtoSERVERS_GUILD_UPDATE_EXP		rExp;
				memset(&rExp,0,sizeof(WORLDtoSERVERS_GUILD_UPDATE_EXP));
				rExp.base.set(sizeof(WORLDtoSERVERS_GUILD_UPDATE_EXP),dWORLDtoSERVERS_GUILD_UPDATE_EXP);
				rExp.wCount	=	0;
				
				for(int i=0;i<rPacket->wCount;i++)
				{
					cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(rPacket->aExpData[i].m_wGuildSerial);
					
					if	(!lpGuild || lpGuild->wSerial==0xffff)	//	그런 길드가 없다.
					{
						sendGSPValidGuild(NULL,rPacket->aExpData[i].m_wGuildSerial);
#ifdef _FOR_KOREA
						_log("ERROR : 경험치 무산됨.. [%d] [%d]",rPacket->aExpData[i].m_wGuildSerial,rPacket->aExpData[i].m_dwUpdateExp);
#endif
						//	해당 게임 서버에 정보 보내줘야함.
						continue;
					}

					//					_log("[%s] 길드 경험치 + [%d + %d]",lpGuild->m_strName,lpGuild->m_dwExp,rPacket->aExpData[i].m_dwUpdateExp);

					int	iIncreaseLevel	=	lpGuild->increaseGuildExp(rPacket->aExpData[i].m_dwUpdateExp);

					if	(iIncreaseLevel)
					{
						_logf("-------> [%s]GUILD lev UP+%d [%d(+%d)]",lpGuild->m_strName,iIncreaseLevel,lpGuild->m_wLevel,rPacket->aExpData[i].m_dwUpdateExp);
						SaveGuildBaseData(lpGuild);
					}

					rExp.aExpData[rExp.wCount].m_dwUpdateExp	=	lpGuild->m_dwExp;
					rExp.aExpData[rExp.wCount].m_wGuildSerial	=	lpGuild->m_wSerial;
					rExp.aExpData[rExp.wCount].m_wLevel			=	lpGuild->m_wLevel;
					rExp.aExpData[rExp.wCount].m_wGuildPoint	=	lpGuild->m_wGuildPoint;
					rExp.aExpData[rExp.wCount].m_wExpTaxRate	=	lpGuild->m_wExpTaxRate;
					rExp.wCount++;
				}

				rExp.base.wSize = sizeof(cMSG_BASE_TYPE_FORWORLD)+sizeof(WORD)+sizeof(CGuildExpInfoIncludeLevel)*rExp.wCount;

				SendDataAllGameServer((char *)&rExp,rExp.base.wSize);
				break;
			}

		case dSERVERStoWORLD_INCREASE_GUILD_SKILL_LEVEL	:
			{
				SERVERStoWORLD_INCREASE_GUILD_SKILL_LEVEL * rPacket;
				rPacket = (SERVERStoWORLD_INCREASE_GUILD_SKILL_LEVEL *)recvpacket;

				cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(rPacket->wGuildSerial);

				if	(!lpGuild || lpGuild->wSerial==0xffff)	//	그런 길드가 없다.
				{
					sendGSPValidGuild(NULL,rPacket->wGuildSerial);
					break;
				}

				CGuildSkillInfo	skillInfo;
				
				int	iResult	=	lpGuild->increaseGuildSkillLevel(rPacket->wSkill,&skillInfo);

				if (iResult	==	eRIGSL_OK)	//	길드 스킬 레벨 증가 성공
				{
					WORLDtoSERVERS_UPDATE_GUILD_SKILL_LEVEL	packetSuccess;
					
					packetSuccess.base.set(sizeof(packetSuccess),dWORLDtoSERVERS_UPDATE_GUILD_SKILL_LEVEL);
					
					packetSuccess.wGuildSerial		=	lpGuild->m_wSerial;
					packetSuccess.wSkill			=	skillInfo.m_wSkill;
					packetSuccess.wLevel			=	skillInfo.m_wLevel;
					packetSuccess.wGuildPoint		=	lpGuild->m_wGuildPoint;
					
					SendDataAllGameServer((char *)&packetSuccess,packetSuccess.base.wSize);
					SaveGuildBaseData(lpGuild);
					SaveGuildSkill(lpGuild);

					{
						SG_UPDATE_GUILD_SKILL_LEVEL	packet;

						packet.base.set(sizeof(packet),dSG_UPDATE_GUILD_SKILL_LEVEL);

						packet.wGuildSerial		=	lpGuild->m_wSerial;
						packet.wSkill			=	skillInfo.m_wSkill;
						packet.wLevel			=	skillInfo.m_wLevel;
						packet.wRemainSkillPoint=	lpGuild->m_wGuildPoint;

						SendGuildPacketToBCS(&packet,lpGuild->m_wSerial);
					}

					break;
				}
				
				//	길드 스킬 레벨 증가 실패
				WORLDtoSERVERS_GUILD_SKILL_LEVEL_INCREASE_FAILURE	packetFailed;
				
				packetFailed.base.set(sizeof(packetFailed),dWORLDtoSERVERS_GUILD_SKILL_LEVEL_INCREASE_FAILURE);
				packetFailed.wResult		=	iResult;
				packetFailed.wGuildSerial	=	lpGuild->m_wSerial;
				
				user->AddSendPacket((char *)&packetFailed,packetFailed.base.wSize);
				break;
			}
			
		case dSERVERStoWORLD_CHANGE_GUILD_SKILL_STATUS	:
			{
				SERVERStoWORLD_CHANGE_GUILD_SKILL_STATUS * rPacket;
				rPacket = (SERVERStoWORLD_CHANGE_GUILD_SKILL_STATUS *)recvpacket;
				
				cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(rPacket->wGuildSerial);
				
				if	(!lpGuild || lpGuild->wSerial==0xffff)	//	그런 길드가 없다.
				{
					sendGSPValidGuild(NULL,rPacket->wGuildSerial);
					break;
				}
				
				if (lpGuild->m_abGuildSkill[rPacket->wSkill]	==	0)
					break;
				
				if	(rPacket->wIsDisable)
				{
					if(lpGuild->m_abGuildSkill[rPacket->wSkill]<	0x80)
						lpGuild->m_abGuildSkill[rPacket->wSkill]	+=	0x80;
				}
				else
				{
					if(lpGuild->m_abGuildSkill[rPacket->wSkill]>=	0x80)
						lpGuild->m_abGuildSkill[rPacket->wSkill]-=	0x80;
				}
				
				WORLDtoSERVERS_UPDATE_GUILD_SKILL_LEVEL	packetSuccess;
				
				packetSuccess.base.set(sizeof(packetSuccess),dWORLDtoSERVERS_UPDATE_GUILD_SKILL_LEVEL);
				
				packetSuccess.wGuildSerial		=	lpGuild->m_wSerial;
				packetSuccess.wSkill			=	rPacket->wSkill;
				packetSuccess.wLevel			=	lpGuild->m_abGuildSkill[rPacket->wSkill];
				packetSuccess.wGuildPoint		=	lpGuild->m_wGuildPoint;
				
				SendDataAllGameServer((char *)&packetSuccess,packetSuccess.base.wSize);
				SaveGuildSkill(lpGuild);
				break;
			}
			
		case dSERVERStoWORLD_BOOKING_GUILD_BATTLE	:
			{
				SERVERStoWORLD_BOOKING_GUILD_BATTLE	 *lpReceivePacket	=	(SERVERStoWORLD_BOOKING_GUILD_BATTLE *)recvpacket;
				
				cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(lpReceivePacket->wGuildSerial);
				
				if	(!lpGuild || lpGuild->wSerial==0xffff)	//	그런 길드가 없다.
				{
					sendGSPValidGuild(NULL,lpReceivePacket->wGuildSerial);
					break;
				}

				CGuildBattleInfo	returnData;

				int	iResult	=	g_guildBattleManager.bookingBattle(lpReceivePacket->wGuildSerial,lpReceivePacket->wTimeOrder,
																	lpReceivePacket->wFieldOrder,lpReceivePacket->wCurrentBattleOrder,&returnData);
				
				if (iResult == eRBGB_SUCCESS || iResult == eRBGB_ALREADY_EXIST_SCHEDULE || iResult == eRBGB_ALREADY_BOOKED || iResult == eRBGB_INCORRECT_DATA)
				{
					WORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO	packetSuccess;
					
					packetSuccess.base.set(sizeof(packetSuccess),dWORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO);
					
					packetSuccess.wTimeOrder		=	lpReceivePacket->wTimeOrder;
					packetSuccess.wFieldOrder		=	lpReceivePacket->wFieldOrder;
					packetSuccess.wGuildSerial		=	lpReceivePacket->wGuildSerial;
					packetSuccess.awBattleGuild[0]	=	returnData.m_awBattleGuild[0];
					packetSuccess.awBattleGuild[1]	=	returnData.m_awBattleGuild[1];
					
					SendDataAllGameServer((char *)&packetSuccess,packetSuccess.base.wSize);
					SaveGuildBaseData(lpGuild);
				}
				
				if (iResult == eRBGB_SUCCESS)
					break;
				
				break;
			}
			
		case dSERVERStoWORLD_CANCEL_GUILD_BATTLE	:
			{
				SERVERStoWORLD_CANCEL_GUILD_BATTLE	 *lpReceivePacket	=	(SERVERStoWORLD_CANCEL_GUILD_BATTLE *)recvpacket;
				
				if	(lpReceivePacket->wGuildSerial	==	0xffff)
				{
					int	iResult	=	g_guildBattleManager.cancelBattles(lpReceivePacket->wTimeOrder);

					WORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO	packetSuccess;
					
					packetSuccess.base.set(sizeof(packetSuccess),dWORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO);
					
					packetSuccess.wTimeOrder		=	lpReceivePacket->wTimeOrder;
					packetSuccess.wFieldOrder		=	0xffff;
					packetSuccess.wGuildSerial		=	0xffff;
					packetSuccess.awBattleGuild[0]	=	0xffff;
					packetSuccess.awBattleGuild[1]	=	0xffff;
					
					SendDataAllGameServer((char *)&packetSuccess,packetSuccess.base.wSize);
					break;
				}
				
				cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(lpReceivePacket->wGuildSerial);
				
				if	(!lpGuild || lpGuild->wSerial==0xffff)	//	그런 길드가 없다.
				{
					sendGSPValidGuild(NULL,lpReceivePacket->wGuildSerial);
					break;
				}

				CGuildBattleInfo	returnData;
				
				int	iResult	=	g_guildBattleManager.cancelBookedBattle(lpReceivePacket->wGuildSerial,&returnData);
				
				WORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO	packetSuccess;
				
				packetSuccess.base.set(sizeof(packetSuccess),dWORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO);

				packetSuccess.wTimeOrder		=	lpReceivePacket->wTimeOrder;
				packetSuccess.wFieldOrder		=	lpReceivePacket->wFieldOrder;
				packetSuccess.wGuildSerial		=	lpReceivePacket->wGuildSerial;
				packetSuccess.awBattleGuild[0]	=	returnData.m_awBattleGuild[0];
				packetSuccess.awBattleGuild[1]	=	returnData.m_awBattleGuild[1];
				
				SendDataAllGameServer((char *)&packetSuccess,packetSuccess.base.wSize);
				
				break;
			}

		case dSERVERStoWORLD_ADD_GUILD_GOLD				:
			{
				nsReceiveFromGS::addGuildGold((SERVERStoWORLD_ADD_GUILD_GOLD*)recvpacket);

				break;
			}

		case dSERVERStoWORLD_ASK_GUILD_BATTLE_SCHEDULE		:	//	길드전 스케쥴을 요청해 왔다.
			{
				SERVERStoWORLD_ASK_GUILD_BATTLE_SCHEDULE	 *lpReceivePacket	=	(SERVERStoWORLD_ASK_GUILD_BATTLE_SCHEDULE *)recvpacket;
				sendGSPGuildBattleSchedule(user);
				break;
			}
			
		case dSERVERStoWORLD_ASK_GUILD_INFO					:
			{
				SERVERStoWORLD_ASK_GUILD_INFO *lpReceivePacket	=	(SERVERStoWORLD_ASK_GUILD_INFO*)recvpacket;
				
				sendGSPUpdateGuildInfo(user,lpReceivePacket->wGuildSerial,FALSE);
				break;
			}
			
		case dSERVERStoWORLD_ASK_GUILD_BATTLE_INFO			:
			{
				SERVERStoWORLD_ASK_GUILD_BATTLE_INFO *lpReceivePacket	=	(SERVERStoWORLD_ASK_GUILD_BATTLE_INFO*)recvpacket;
				
				sendGSPGuildBattleInfo(user,lpReceivePacket->wTimeOrder,lpReceivePacket->wFieldOrder);
				break;
			}
			
		case dSERVERStoWORLD_GUILD_BATTLE_RESULT			:
			{
				SERVERStoWORLD_GUILD_BATTLE_RESULT	*lpReceivePacket	=	(SERVERStoWORLD_GUILD_BATTLE_RESULT*)recvpacket;
				
				operateGuildBattleResult(NULL,lpReceivePacket);
				break;
			}
			
		case dSERVERStoWORLD_SELECT_GUILD_MARK				:
			{
				SERVERStoWORLD_SELECT_GUILD_MARK	*lpReceivePacket	=	(SERVERStoWORLD_SELECT_GUILD_MARK*)recvpacket;
				
				operateSelectGuildMark(NULL,lpReceivePacket);
				break;
			}
			
		case dSERVERStoWORLD_UPDATE_GUILD_MEMBER_LEVEL		:
			{
				SERVERStoWORLD_UPDATE_GUILD_MEMBER_LEVEL	*lpReceivePacket	=	(SERVERStoWORLD_UPDATE_GUILD_MEMBER_LEVEL*)recvpacket;
				
				operateUpdateGuildMemberLevel(NULL,lpReceivePacket);
				break;
			}
		case dSERVERStoWORLD_ASK_GUILD_NAME_LIST		:
			{
				SERVERStoWORLD_ASK_GUILD_NAME_LIST	*lpReceivePacket	=	(SERVERStoWORLD_ASK_GUILD_NAME_LIST*)recvpacket;
				sendGSPGuildNameList(user,lpReceivePacket->iFirstGuild,lpReceivePacket->iServerIndex);
				break;
			}
		case dSERVERStoWORLD_ASK_GUILD_NAME	:
			{
				SERVERStoWORLD_ASK_GUILD_NAME*	lpPacket	=	(SERVERStoWORLD_ASK_GUILD_NAME*)recvpacket;
				sendGSPGuildName(user,lpPacket->wGuildSerial);
				break;
			}
			
		case dSERVERStoWORLD_BEGIN_TEST_GUILD_BATTLE	:
			{
				SERVERStoWORLD_BEGIN_TEST_GUILD_BATTLE*	lpPacket	=	(SERVERStoWORLD_BEGIN_TEST_GUILD_BATTLE*)recvpacket;
				operateBeginTestGuildBattle(user,lpPacket);
				break;
			}
			
			
			//┗━END   ABOUT GUILD━━━━━━━━━━━━━━━━━━━━
			//	ITEM
			
		case dSERVERStoWORLD_RESET_DROP_PACK_ITEM_COUNT	:
			{
				SERVERStoWORLD_RESET_DROP_PACK_ITEM_COUNT	*lpPacket	= (SERVERStoWORLD_RESET_DROP_PACK_ITEM_COUNT*)recvpacket;
				
				g_itemPackManager.resetCounter(lpPacket->wPackSerial);
				
				WORLDtoSERVERS_RESET_DROP_PACK_ITEM_COUNT	packet;
				
				packet.base.set(sizeof(packet),dWORLDtoSERVERS_RESET_DROP_PACK_ITEM_COUNT);
				packet.wPackSerial	=	lpPacket->wPackSerial;
				
				CClient		*lpServer=	NULL;
				
				mSEND_GSP(lpServer,packet);
				
				break;
			}
			
		case dSERVERStoWORLD_INCREASE_PACK_ITEM_COUNT	:
			{
				SERVERStoWORLD_INCREASE_PACK_ITEM_COUNT	*lpPacket	= (SERVERStoWORLD_INCREASE_PACK_ITEM_COUNT*)recvpacket;

				int	iCount	=	g_itemPackManager.increaseItemCounter(lpPacket->wPackSerial,lpPacket->wItemIndex);

				if	(iCount)
				{
					WORLDtoSERVERS_UPDATE_PACK_ITEM_COUNT	packet;

					packet.base.set(sizeof(packet),dWORLDtoSERVERS_UPDATE_PACK_ITEM_COUNT);

					packet.wPackSerial	=	lpPacket->wPackSerial;
					packet.wItemIndex	=	lpPacket->wItemIndex;
					packet.wCount		=	iCount;

					CClient		*lpServer=	NULL;

					mSEND_GSP(lpServer,packet);
				}
				break;
			}
			
		case dSERVERStoWORLD_CREATE_ITEM:
			{
				SERVERStoWORLD_CREATE_ITEM * rPacket;
				rPacket = (SERVERStoWORLD_CREATE_ITEM *)recvpacket;
				
				WORLDtoSERVERS_CREATE_ITEM_RESULT	sCreateItem;
				memset(&sCreateItem,0,sizeof(WORLDtoSERVERS_CREATE_ITEM_RESULT));
				sCreateItem.base.set(sizeof(WORLDtoSERVERS_CREATE_ITEM_RESULT),dWORLDtoSERVERS_CREATE_ITEM_RESULT);
				sCreateItem.wFieldSerial = rPacket->wFieldSerial;
				sCreateItem.wCount = rPacket->wCount; 
				for(int i=0;i<sCreateItem.wCount;i++)
				{
					if(rPacket->aCreateItem[i].m_wItem!=0xfffe)	//	돈
					{	//	해당 아이템의 카운터만 증가
						//	시리얼 할당
						increaseItemCounter(&sCreateItem.aCreateItemResult[i].m_dwRegisteredSerial);
					}
					//					_log("[%d]receive  item serial %d (0x%x )\n",i,rPacket->aCreateItem[i].m_wSerial,rPacket->aCreateItem[i].m_wItem);
					sCreateItem.aCreateItemResult[i].m_wSerial = rPacket->aCreateItem[i].m_wSerial;
				}
				user->AddSendPacket((char *)&sCreateItem,sCreateItem.base.wSize);
				break;
			}
			
		case dSERVERStoWORLD_ASK_ITEM_UNIQUE_SERIAL	:
			{
				SERVERStoWORLD_ASK_ITEM_UNIQUE_SERIAL * rPacket;
				WORLDtoSERVERS_ASK_ITEM_UNIQUE_SERIAL	sendPacket;
				rPacket = (SERVERStoWORLD_ASK_ITEM_UNIQUE_SERIAL *)recvpacket;
				
				sendPacket.base.set(sizeof(WORLDtoSERVERS_ASK_ITEM_UNIQUE_SERIAL),dWORLDtoSERVERS_ASK_ITEM_UNIQUE_SERIAL);
				sendPacket.iSerialInServer	=	rPacket->iSerialInServer;
				
				increaseItemCounter(&sendPacket.dwSerial);
				
				user->AddSendPacket((char *)&sendPacket,sendPacket.base.wSize);
				break;
			}
		case dSERVERStoWORLD_CREATE_SELL_ITEM	:
			{
				SERVERStoWORLD_CREATE_SELL_ITEM * rPacket;
				rPacket = (SERVERStoWORLD_CREATE_SELL_ITEM *)recvpacket;

				WORLDtoSERVERS_CREATE_SELL_ITEM_RESULT	sCreateSellItem;
				memset(&sCreateSellItem,0,sizeof(WORLDtoSERVERS_CREATE_SELL_ITEM_RESULT));
				sCreateSellItem.base.set(sizeof(WORLDtoSERVERS_CREATE_SELL_ITEM_RESULT),dWORLDtoSERVERS_CREATE_SELL_ITEM_RESULT);
				
				sCreateSellItem.iSerialInServer	=	rPacket->iSerialInServer;
				sCreateSellItem.dwPrice			=	rPacket->dwPrice;
				sCreateSellItem.wCount			=	rPacket->wCount;
				sCreateSellItem.wWhere			=	rPacket->wWhere;
				sCreateSellItem.wIsDuelPointShop=	rPacket->wIsDuelPointShop;
				sCreateSellItem.wIsTokenShop	=	rPacket->wIsTokenShop;

				strcpy(sCreateSellItem.strName,rPacket->strName);
				memcpy(&sCreateSellItem.item,&rPacket->item,sizeof(CItemDefine));
				
				increaseItemCounter(&sCreateSellItem.item.m_dwSerial);

				sCreateSellItem.dwSerial		=	sCreateSellItem.item.m_dwSerial;

				for(int i=0;i<rPacket->wCount-1;i++)
					increaseItemCounter(&sCreateSellItem.item.m_dwSerial);
					
				
				user->AddSendPacket((char *)&sCreateSellItem,sCreateSellItem.base.wSize);
				break;
			}
			
		case dSERVERStoWORLD_ADD_ITEM:
			{
				SERVERStoWORLD_ADD_ITEM			* rPacket;
				WORLDtoSERVERS_ADD_ITEM_RESULT	sendPacket;
				
				rPacket = (SERVERStoWORLD_ADD_ITEM *)recvpacket;
				
				sendPacket.base.set(sizeof(WORLDtoSERVERS_ADD_ITEM_RESULT),dWORLDtoSERVERS_ADD_ITEM_RESULT);
				sendPacket.wClient	=	rPacket->wClient;
				memcpy(&sendPacket.item,&rPacket->item,sizeof(CItemDefine));
				
				increaseItemCounter(&sendPacket.item.m_dwSerial);
				
				user->AddSendPacket((char *)&sendPacket,sendPacket.base.wSize);
				break;
			}
			
		case dSERVERStoWORLD_IS_DENY_COMMUNITY_TARGET:
			{
				operateIsDenyCommunityTargetPacket((SERVERStoWORLD_IS_DENY_COMMUNITY_TARGET *)recvpacket);
				break;
			}
			
		case dSERVERStoWORLD_USER_SAY:
			{
				SERVERStoWORLD_USER_SAY * rPacket;
				rPacket = (SERVERStoWORLD_USER_SAY *)recvpacket;
				
				Avatar_Data *ava			=	UserManager.GetGClientPnt(rPacket->strRecvName,1);
				WORD		l_iFieldSerial	=	UserManager.GetClientOfGClientSerial(rPacket->strRecvName,1);
				CClient		*usr			=	UserManager.GetClientPnt(l_iFieldSerial);
				
				WORLDtoSERVERS_USER_SAY_RESULT	sSayFail;
				memset(&sSayFail,0,sizeof(WORLDtoSERVERS_USER_SAY_RESULT));
				sSayFail.base.set(sizeof(WORLDtoSERVERS_USER_SAY_RESULT),dWORLDtoSERVERS_USER_SAY_RESULT);
				sSayFail.dwSenderSerial		=	rPacket->dwSenderSerial;
				
				if	(!ava || ava->wSerial==0xffff || !usr || l_iFieldSerial==0xffff)
					sSayFail.wReason = eROFS_CAN_NOT_FIND_TARGET;
				else
				{
					if	(ava->isOper>=3)
						sSayFail.wReason = eROFS_TARGET_IS_OPERATOR;
					else
						sSayFail.wReason = eROFS_SUCCESS;
				}

				strcpy(sSayFail.strDestName,rPacket->strRecvName);
				strcpy(sSayFail.strMsg,rPacket->strMsg);
				user->AddSendPacket((char *)&sSayFail,sSayFail.base.wSize);
				SendPacket(user->GetSerial());
				
				if	(usr && sSayFail.wReason == eROFS_SUCCESS)
				{
					WORLDtoSERVERS_USER_SAY		sSay;
					
					memset(&sSay,0,sizeof(WORLDtoSERVERS_USER_SAY));
					sSay.base.set(sizeof(WORLDtoSERVERS_USER_SAY),dWORLDtoSERVERS_USER_SAY);
					sSay.wIsByOperator	=	rPacket->wIsByOperator;
					strcpy(sSay.strMsg,rPacket->strMsg);
					strcpy(sSay.strRecvName,rPacket->strRecvName);
					strcpy(sSay.strSendName,rPacket->strSendName);
					usr->AddSendPacket((char *)&sSay,sSay.base.wSize);
					SendPacket(usr->GetSerial());
				}
				break;
			}
		case dSERVERStoWORLD_USER_RECALL:
			{
				SERVERStoWORLD_USER_RECALL * rPacket;
				rPacket = (SERVERStoWORLD_USER_RECALL *)recvpacket;
				
				WORLDtoSERVERS_USER_RECALL	sRecall;
				memcpy(&sRecall,rPacket,sizeof(WORLDtoSERVERS_USER_RECALL));
				sRecall.base.set(sizeof(WORLDtoSERVERS_USER_RECALL),dWORLDtoSERVERS_USER_RECALL);
				
				CClient * target = UserManager.GetClientOfGClientPnt(rPacket->strName,1);
				if(target)	target->AddSendPacket((char *)&sRecall,sRecall.base.wSize);
				break;
			}

		case dSERVERStoWORLD_INCREASE_DB_EVENT_VALUE	:
			{
#ifdef	_FOR_KOREA	
				SERVERStoWORLD_INCREASE_DB_EVENT_VALUE	*lpPacket	=	(SERVERStoWORLD_INCREASE_DB_EVENT_VALUE	*)recvpacket;

				WU_ASK_INCREASE_DB_EVENT_VALUE	packet;

				packet.base.set(sizeof(packet),dWU_ASK_INCREASE_DB_EVENT_VALUE,-1);

				strcpy(packet.strUserID,lpPacket->strUserID);
				strcpy(packet.strUserName,lpPacket->strUserName);
				packet.wEventIndex		=	lpPacket->wEventIndex;
				packet.bReset			=	lpPacket->bReset;
				packet.iMaxCount		=	lpPacket->iMaxCount;
				packet.bIsAllowMulipleID=	lpPacket->bIsAllowMulipleID;

				GetIDDBSocket()->SendPacket((char *)&packet,packet.base.wSize);
#endif
				break;
			}

		case dSERVERStoWORLD_ASK_DUEL_RECORD	:
		{
			SERVERStoWORLD_ASK_DUEL_RECORD	*lpPacket	=	(SERVERStoWORLD_ASK_DUEL_RECORD *)recvpacket;
			WU_ASK_DUEL_RECORD	packet;

			packet.base.set(sizeof(packet),dWU_ASK_DUEL_RECORD,-1);

			strcpy(packet.strId,lpPacket->strId);
			packet.iGSSerialInWorld	=	user->GetSerial();

			GetIDDBSocket()->SendPacket((char *)&packet,packet.base.wSize);
			break;
		}

		case	dSERVERStoWORLD_UPDATE_DUEL_TEAM_RECORD	:
		{
			SERVERStoWORLD_UPDATE_DUEL_TEAM_RECORD	*rPacket	=	(SERVERStoWORLD_UPDATE_DUEL_TEAM_RECORD*)recvpacket;
			WU_UPDATE_DUEL_TEAM_RECORD	packet;

			packet.base.set(sizeof(packet),dWU_UPDATE_DUEL_TEAM_RECORD,-1);

			strcpy(packet.strTeamName,rPacket->strTeamName);
			memcpy(&packet.record,&rPacket->record,sizeof(cDuelTeamRecord));
			packet.iGSSerialInWorld	=	user->GetSerial();

			cDuelTeamInfo*	lpTeam	=	g_duelTeamManager.getTeam(rPacket->strTeamName);

			if	(lpTeam)
				memcpy(&lpTeam->m_record,&rPacket->record,sizeof(cDuelTeamRecord));

			GetIDDBSocket()->SendPacket((char *)&packet,packet.base.wSize);
			break;
		}

		case dSERVERStoWORLD_UPDATE_DUEL_RECORD	:
		{
			SERVERStoWORLD_UPDATE_DUEL_RECORD	*rPacket	=	(SERVERStoWORLD_UPDATE_DUEL_RECORD*)recvpacket;
			WU_UPDATE_DUEL_RECORD	packet;

			packet.base.set(sizeof(packet),dWU_UPDATE_DUEL_RECORD,-1);

			strcpy(packet.strId,rPacket->strId);
			memcpy(&packet.record,&rPacket->record,sizeof(cDuelRecord));
			packet.iGSSerialInWorld	=	user->GetSerial();

			GetIDDBSocket()->SendPacket((char *)&packet,packet.base.wSize);
			break;
		}

		case dSERVERStoWORLD_USER_FIND:
			{
				SERVERStoWORLD_USER_FIND * rPacket;
				rPacket = (SERVERStoWORLD_USER_FIND *)recvpacket;
				
				WORLDtoSERVERS_USER_FIND		sFind;
				memset(&sFind,0,sizeof(WORLDtoSERVERS_USER_FIND));
				sFind.base.set(sizeof(WORLDtoSERVERS_USER_FIND),dWORLDtoSERVERS_USER_FIND);
				
				sFind.wWork	=	rPacket->wWork;
				sFind.wField=	rPacket->wField;
				
				strcpy(sFind.strFindName,rPacket->strFindName);
				strcpy(sFind.strName,rPacket->strName);
				
				Avatar_Data *lpAvatar	=	UserManager.GetGClientPnt(rPacket->strFindName,1);
				
				if	(lpAvatar	!=	NULL)
				{
					sFind.Result			=	dFIND_RESULT_SUCCESS;
					sFind.wMapUniqueIndex	=	lpAvatar->data.m_wCurrentField;
					sFind.wJob				=	lpAvatar->data.m_wJob;
					sFind.bf1IsPremiumZone	=	FALSE;
					
					CClient		*lpTargetServer	=	UserManager.GetClientOfGClientPnt(rPacket->strFindName,1);
					if(!lpTargetServer)
					{
						sFind.Result = dFIND_RESULT_NOTFIND;
						goto NOTFOUND;
					}
					int			iMapIndex		=	lpTargetServer->GetMapIndex(lpAvatar->data.m_wCurrentField);
					
					if	(lpTargetServer	&&	lpTargetServer->cGSInfo[iMapIndex].wType	==	eFT_PREMIUM_ZONE)
						sFind.bf1IsPremiumZone	=	TRUE;
					
					if	(lpTargetServer)
					{
						if	(sFind.wWork	==	eFUW_BANISH)	//	추방하려고 찾는거다..
						{
							if	(lpTargetServer->m_wServerIndex==0xffff || lpTargetServer->m_wServerIndex	!=	user->m_wServerIndex)
								lpTargetServer->AddSendPacket((char *)&sFind,sFind.base.wSize);
						}
						else
						if	(sFind.wWork	==	eFUW_USE_COUPLE_RING)	//	커플링을 사용해 텔레포트 하려고
						{
							sFind.wWork		=	eFUW_CHECK_COUPLE_RING;
							sFind.wField	=	0xffff;
							
							strcpy(sFind.strName,rPacket->strFindName);
							strcpy(sFind.strFindName,rPacket->strName);
							
							lpTargetServer->AddSendPacket((char *)&sFind,sFind.base.wSize);
							
							break;
						}
						else
						if	(sFind.wWork	==	eFUW_CHECK_COUPLE_RING_RESULT_FAILED)	//	커플링을 사용했는데, 상대방은 커플링이 없다.
						{
							sFind.Result	=	dFIND_NOT_EXIST_COUPLE_RING;
							
							strcpy(sFind.strName,rPacket->strFindName);
							strcpy(sFind.strFindName,rPacket->strName);
							
							lpTargetServer->AddSendPacket((char *)&sFind,sFind.base.wSize);
							
							break;
						}
						else
						if	(sFind.wWork	==	eFUW_CHECK_COUPLE_RING_RESULT_OK)		//	커플링 사용 성공
						{
							sFind.wWork		=	eFUW_USE_COUPLE_RING;
							
							lpTargetServer	=	UserManager.GetClientOfGClientPnt(rPacket->strName,1);
							
							if	(lpTargetServer)
								lpTargetServer->AddSendPacket((char *)&sFind,sFind.base.wSize);
							
							break;
						}
						else
						if	(sFind.wWork	==	eFUW_CHECK_COUPLE_RING_RESULT_TARGET_EXIST_CAN_NOT_TELEPORT_FIELD)	//	상대방이 커플링을 사용해서 갈 수 없는 필드에 있다.
						{
							sFind.wWork		=	eFUW_CHECK_COUPLE_RING_RESULT_TARGET_EXIST_CAN_NOT_TELEPORT_FIELD;
							
							lpTargetServer	=	UserManager.GetClientOfGClientPnt(rPacket->strName,1);

							if	(lpTargetServer)
								lpTargetServer->AddSendPacket((char *)&sFind,sFind.base.wSize);
							
							break;
						}
					}
				}
				else
					sFind.Result = dFIND_RESULT_NOTFIND;
NOTFOUND:				
				user->AddSendPacket((char *)&sFind,sFind.base.wSize);
				break;
			}
			
		case	dSERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST	:
			{
				SERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST	*lpPacket	=	(SERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST	*)recvpacket;
				
				operateRemoveFriendInTargetList(lpPacket->strClientName,lpPacket->strTargetName);
				break;
			}
		case	dSERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST_RESULT	:
			{
				SERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST_RESULT	*lpPacket	=	(SERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST_RESULT	*)recvpacket;
				
				operateRemoveFriendInTargetListResult(lpPacket->strClientName,lpPacket->strTargetName,lpPacket->wResult);
				break;
			}
			
			//┏━START ABOUT EVENT━━━━━━━━━━━━━━━━━━━━
		case dSERVERStoWORLD_MODIFY_LOTTO_EVENT_DATA				:
			{
				SERVERStoWORLD_MODIFY_LOTTO_EVENT_DATA	*lpPacket	=	(SERVERStoWORLD_MODIFY_LOTTO_EVENT_DATA*)recvpacket;

				g_lottoEventManager.updateData(lpPacket->wCount,lpPacket->aEventList);
				break;
			}
			
		case	dSERVERStoWORLD_MODIFY_EVENT_TIME	:
			{
				SERVERStoWORLD_MODIFY_EVENT_TIME	*lpPacket	=	(SERVERStoWORLD_MODIFY_EVENT_TIME*)recvpacket;

				if	(lpPacket->wIsByLoginServer)	//	로그인 서버에서 받은거다.
					g_eventTimeManager.updateData(lpPacket->wCount,lpPacket->aEventList);
				else
				{
					CClient	*lpLoginServer	=	UserManager.GetClientPnt(dLOGIN_SERVER_ID);

					if	(!lpLoginServer)
					{
						SendSimpleWork(user,eSWWA_CAN_NOT_FOUND_LOGIN_SERVER);

						break;
					}

					SERVERStoWORLD_MODIFY_EVENT_TIME	packet;

					memcpy(&packet,lpPacket,lpPacket->base.wSize);

					packet.base.wType	=	dWORLDtoSERVERS_EVENT_TIME_DATA;

					mSEND_GSP(lpLoginServer,packet);
				}
					
				break;
			}

		case dSERVERStoWORLD_BANISH_USER				:
			{
				operateBanishUser(user,(SERVERStoWORLD_BANISH_USER*)recvpacket);
				break;
			}
			
		case dSERVERStoWORLD_UPDATE_GR_INFO				:
			{
				SERVERStoWORLD_UPDATE_GR_INFO*	lpPacket	=	(SERVERStoWORLD_UPDATE_GR_INFO*)recvpacket;
				
				g_goldRush.updateInfo(&lpPacket->begin,&lpPacket->end,lpPacket->wBoostExp,lpPacket->wDropChance,lpPacket->strBeginComment,lpPacket->strEndComment);
				sendGRInfo(NULL);
				break;
			}
			
		case dSERVERStoWORLD_BOOST_POWER_BY_EVENT		:
			{
				operateBoostPowerByEvent(NULL,(SERVERStoWORLD_BOOST_POWER_BY_EVENT*)recvpacket);
				break;
			}

		case dSERVERStoWORLD_ITEM_PACK_INFO_CHECKSUM	:
			{
				operateItemPackInfoCheckSum(user,(SERVERStoWORLD_ITEM_PACK_INFO_CHECKSUM*)recvpacket);
				break;
			}
			
		case dSERVERStoWORLD_ADD_RENEW_ITEM_EXPIRE_TIME			:
			{
				operateAddRenewItemExpireTime(user,(SERVERStoWORLD_ADD_RENEW_ITEM_EXPIRE_TIME*)recvpacket);
				break;
			}
			
		case dSERVERStoWORLD_ASK_RENEW_ITEM_EXPIRE_TIME_LIST	:
			{
				operateAskRenewItemExpireTimeList(user);
				break;
			}
			
		case dSERVERStoWORLD_APPOINTMENT_QUIZ_EVENT	:
			{
				operateAppointmentQuizEvent(NULL,(SERVERStoWORLD_APPOINTMENT_QUIZ_EVENT *)recvpacket);
				break;
			}
		case dSERVERStoWORLD_PRODUCE_WORD_QUIZ		:
			{
				operateProduceWordQuiz(NULL,(SERVERStoWORLD_PRODUCE_WORD_QUIZ *)recvpacket);
				break;
			}

		case dSERVERStoWORLD_CHANGE_EVENT_WORD	:
			{
				operateChangeEventWord(NULL,(SERVERStoWORLD_SET_EVENT_WORD *)recvpacket);
				break;
			}

		case dSERVERStoWORLD_SOUND_OF_LEADERS_BELL	:
			{
				g_eventManager.useSoundOfLeadersBell(user,(SERVERStoWORLD_SOUND_OF_LEADERS_BELL *)recvpacket);
				break;
			}

		case dSERVERStoWORLD_CHANGE_MSG_OF_LEDERS_BELL	:
			{
				g_eventManager.changeMsgSoundOfLeadersBell(((SERVERStoWORLD_CHANGE_MSG_OF_LEDERS_BELL *)recvpacket)->strMsg);
				break;
			}

		case dSERVERStoWORLD_WORD_EVENT_END		:
			{
				operateChangeEventWord(NULL,(SERVERStoWORLD_SET_EVENT_WORD *)recvpacket , FALSE);
				break;
			}
		case dSERVERStoWORLD_WORD_EVENT_START	:
			{
				break;
			}

		case dSERVERStoWORLD_BUILD_RESULT_OF_GUILD_BATTLE_PROLOG	:
			{
				OperateBuildGuildBattleProlog(((SERVERStoWORLD_BUILD_RESULT_OF_GUILD_BATTLE_PROLOG *)recvpacket)->wIsWantOnlyTest);
				break;
			}

		case dSERVERStoWORLD_CHANGE_WORD_QUIZ_ROUND	:
			{
				operateChangeWordQuizRound(NULL,(SERVERStoWORLD_CHANGE_WORD_QUIZ_ROUND *)recvpacket);
				break;
			}
			
		case dSERVERStoWORLD_PRODUCE_OX_QUIZ	:
			{
				operateProduceOxQuiz(NULL,(SERVERStoWORLD_PRODUCE_OX_QUIZ *)recvpacket);
				break;
			}
		case dSERVERStoWORLD_END_QUIZ_EVENT		:
			{
				operateEndQuizEvent(NULL,(SERVERStoWORLD_END_QUIZ_EVENT *)recvpacket);
				break;
			}
			
		case dSERVERStoWORLD_CANCEL_QUIZ_EVENT		:
			{
				operateCancelQuizEvent(NULL,(SERVERStoWORLD_CANCEL_QUIZ_EVENT *)recvpacket);
				break;
			}
			
		case dSERVERStoWORLD_EVENT_QUIZ_GATE_VILLAGE	:
			{
				operateSetQuizEventVillage(NULL,(SERVERStoWORLD_EVENT_QUIZ_GATE_VILLAGE *)recvpacket);
				break;
			}
			
		case dSERVERStoWORLD_OX_QUIZ_LOSER_REVIVE_BATTLE	:
			{
				operateOXQuizLoserReviveBattle(NULL);
				break;
			}

			//┗━END   ABOUT EVENT━━━━━━━━━━━━━━━━━━━━
		case dSERVERStoWORLD_CHANGE_FIXCHECK:
			{
				SERVERStoWORLD_CHANGE_FIXCHECK	* rPacket;
				rPacket = (SERVERStoWORLD_CHANGE_FIXCHECK *)recvpacket;
				
				//	이미 진행되었다면 수정 불가능..
				if	(wServerCheckType==eServerCheckType_NORMAL)
				{
					wWeekFixCheck = rPacket->wWeek;
					wHourFixCheck = rPacket->wHour;
					wMinFixCheck  = rPacket->wMin;
					strcpy(strMsgFixCheck,rPacket->strNoticeMsg);
				}
				else
				{
					wServerCheckType=eServerCheckType_NORMAL;
					
					WORLDtoSERVERS_CHECKTIME		checkTime;
					checkTime.base.set(sizeof(WORLDtoSERVERS_CHECKTIME),dWORLDtoSERVERS_CHECKTIME);
					checkTime.wType = 0;
					SendDataAllGameServer((char *)&checkTime,checkTime.base.wSize);
					
					WORLDtoSERVERS_ALL_MSG		allMsg;
					allMsg.base.set(sizeof(WORLDtoSERVERS_ALL_MSG),dWORLDtoSERVERS_ALL_MSG);
					sprintf(allMsg.strMessage,"%d시 %d에 계획된 서버 점검이 취소되었습니다.",wHourFixCheck,wMinFixCheck);
#ifdef _USE_BROADCAST
					if(!UserManager.SendToBCS((char *)&allMsg,allMsg.base.wSize))
						SendDataAllGameServer((char *)&allMsg,allMsg.base.wSize);
#else
					SendDataAllGameServer((char *)&allMsg,allMsg.base.wSize);
#endif
					
					wWeekFixCheck = rPacket->wWeek;
					wHourFixCheck = rPacket->wHour;
					wMinFixCheck  = rPacket->wMin;
					strcpy(strMsgFixCheck,rPacket->strNoticeMsg);
				}
				break;
			}
		case dSERVERStoWORLD_GET_FIXCHECK:
			{
				WORLDtoSERVERS_GET_FIXCHECK		sPacket;
				sPacket.base.set(sizeof(WORLDtoSERVERS_GET_FIXCHECK),dWORLDtoSERVERS_GET_FIXCHECK);
				
				sPacket.wWeek = wWeekFixCheck;
				sPacket.wHour = wHourFixCheck;
				sPacket.wMin  = wMinFixCheck;
				strcpy(sPacket.strNoticeMsg,strMsgFixCheck);
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				break;
			}
/*		case dSERVERStoWORLD_GET_GUILDLIST:
			{
				WORLDtoSERVERS_GET_GUILDLIST	sPacket;
				sPacket.base.set(sizeof(WORLDtoSERVERS_GET_GUILDLIST),dWORLDtoSERVERS_GET_GUILDLIST);
				int listCount = g_guildManager.Size();
				sPacket.wCount = 0;
				for(int i = 0;i < listCount; i++)
				{
					cGuild	* guild = g_guildManager.GetGuildPnt(i);
					if(!guild || guild->wSerial==0xffff)	continue;
					
					sPacket.wCount++;
					sPacket.list[i].wGuildSerial = guild->m_wSerial;
					strcpy(sPacket.list[i].strGuildName,guild->m_strName);
				}
				sPacket.base.wSize = sizeof(cMSG_BASE_TYPE_FORWORLD)+sizeof(WORD) + sizeof(stGuildList)*sPacket.wCount;
				CM_COMPRESS		comp;
				WORD	wCompSize = cSRVUTIL::Compress((unsigned char *)&sPacket,(unsigned char *)comp.data,sPacket.base.wSize);
				comp.wSourceSize = sPacket.base.wSize;
				comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);
				
				if(wCompSize ==0)
				{
					user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				}else{
					user->AddSendPacket((char *)&comp,comp.wSize);
				}
				break;
			}
*/
		case dSERVERStoWORLD_SYNC_CHANGEDNAME:
			{
				SERVERStoWORLD_SYNC_CHANGEDNAME * rPacket = (SERVERStoWORLD_SYNC_CHANGEDNAME *)recvpacket;

				if	(rPacket->guildSerial == 0xffff)
					break;

				cGuild	* guild = g_guildManager.GetGuildPnt(rPacket->guildSerial);

				if	(!guild)
					break;

				CGuildMan * pMan = guild->GetGuilderPnt(rPacket->oldName);

				if	(!pMan)
					break;

				strcpy(pMan->m_strName, rPacket->newName);

				//	이 내용은 모든 게임 서버와 커뮤니티 서버에 배포할 것!
				WORLDtoSERVERS_SYNC_CHANGEDNAME sPacket;
				memcpy(&sPacket, rPacket,sizeof(WORLDtoSERVERS_SYNC_CHANGEDNAME));
				sPacket.base.wType = dWORLDtoSERVERS_SYNC_CHANGEDNAME;

				SendDataAllGameServer((char *)&sPacket, sPacket.base.wSize, TRUE);
			}
			break;

		case dSERVERStoWORLD_GET_GUILDINFO:
			{
				SERVERStoWORLD_GET_GUILDINFO * rPacket;
				
				rPacket = (SERVERStoWORLD_GET_GUILDINFO *)recvpacket;
				
				cGuild	* guild = g_guildManager.GetGuildPnt(rPacket->strGuildName);
				
				if	(!guild || guild->wSerial==0xffff)
				{
					WORLDtoSERVERS_GET_GUILDINFO	sPacket;
					
					sPacket.base.set(sizeof(WORLDtoSERVERS_GET_GUILDINFO),dWORLDtoSERVERS_GET_GUILDINFO);
					sPacket.base.wSize		=	sizeof(WORLDtoSERVERS_GET_GUILDINFO)-sizeof(sPacket.info);
					sPacket.wGuildSerial	=	0xffff;
					
					user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
					break;
				}
				
				WORLDtoSERVERS_GET_GUILDINFO	sPacket;
				
				sPacket.base.set(sizeof(WORLDtoSERVERS_GET_GUILDINFO),dWORLDtoSERVERS_GET_GUILDINFO);
				sPacket.wGuildSerial		=	guild->wSerial;
				strcpy(sPacket.strGuildName,rPacket->strGuildName);
				
				if(strcmp(guild->m_strName,sPacket.strGuildName)==0)
				{
					memcpy(&sPacket.info,(CGuildAdvanceInfo *)guild,sizeof(CGuildAdvanceInfo));
				}
				else
				{
					sPacket.info.m_wSerial = 0xffff;
				}
				
				CM_COMPRESS		comp;
				
				WORD	wCompSize	=	cSRVUTIL::Compress((unsigned char *)&sPacket,(unsigned char *)comp.data,sPacket.base.wSize);
				
				comp.wSourceSize	=	sPacket.base.wSize;
				comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);
				
				if(wCompSize ==0)
				{
					user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				}
				else
				{
					user->AddSendPacket((char *)&comp,comp.wSize);
				}
				sendGSPUpdateGuildMemberInfo(user,guild->m_wSerial);
				
				break;
			}
		case dSERVERStoWORLD_GET_GUILDMEMBERLIST:
			{
				SERVERStoWORLD_GET_GUILDMEMBERLIST	* rPacket;
				
				rPacket = (SERVERStoWORLD_GET_GUILDMEMBERLIST *)recvpacket;
				
				cGuild	* guild = g_guildManager.GetGuildPnt(rPacket->wGuildSerial);
				
				if(!guild || guild->wSerial==0xffff)
					continue;
				
				//	해당 길드의 유저 정보를 보내준다.
				WORLDtoSERVERS_USER_GUILDINFO	guilderList;
				memset(&guilderList,0,sizeof(WORLDtoSERVERS_USER_GUILDINFO));
				guilderList.base.set(sizeof(WORLDtoSERVERS_USER_GUILDINFO),dWORLDtoSERVERS_USER_GUILDINFO);
				guilderList.wGuildSerial= guild->wSerial;
				guilderList.wAllCount	= guild->GetCount();
				guilderList.wListCount	= 0;
				for(int j = 0;j<dGUILD_MAX_PLAYER || guilderList.wListCount<guild->GetCount(); j++)
				{
					CGuildMan * guilder = guild->GetGuilderPnt(j);
					
					if(!guilder || guilder->wSerial==0xffff)
						continue;
					
					memcpy(&guilderList.List[guilderList.wListCount],guilder,sizeof(CGuildMan));
					guilderList.wListCount++;
				}

				guilderList.base.wSize = sizeof(cMSG_BASE_TYPE_FORWORLD)+sizeof(WORD)*3+sizeof(CGuildMan)*guilderList.wListCount;

				if	(guilderList.wListCount>0)
				{
					CM_COMPRESS		comp;
					WORD	wCompSize = cSRVUTIL::Compress((unsigned char *)&guilderList,(unsigned char *)comp.data,guilderList.base.wSize);
					comp.wSourceSize = guilderList.base.wSize;
					comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);
					
					if(wCompSize ==0)
					{
						user->AddSendPacket((char *)&guilderList,guilderList.base.wSize);
					}else
					{
						user->AddSendPacket((char *)&comp,comp.wSize);
					}
				}else{
					user->AddSendPacket((char *)&guilderList,guilderList.base.wSize);
				}
				break;
			}
//////////////////////////////////////////////////////////////////////////
//	LOG 처리 - DB와의 컨넥션이 없으므로 TOSS 역할만 한다. 
		case dSERVERStoWORLD_LOG_INOUT:
			{
				WU_LOG_INOUT	sPacket;
				memcpy(&sPacket, (SERVERStoWORLD_LOG_INOUT *)recvpacket, sizeof(WU_LOG_INOUT));
				sPacket.base.wType = dWU_LOG_INOUT;

				GetIDDBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
			}
			break;
		case dSERVERStoWORLD_LOG_QUEST:
			{
				WU_LOG_QUEST	sPacket;
				memcpy(&sPacket, (SERVERStoWORLD_LOG_QUEST *)recvpacket, sizeof(WU_LOG_QUEST));
				sPacket.base.wType = dWU_LOG_QUEST;
				
				GetIDDBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
			}
			break;
//	LOG 처리
//////////////////////////////////////////////////////////////////////////
		case dCM_DUMMY:
			{
				break;
			}
		default:
			{	
				_logc("ERROR recv TYPE : [%s] type = 0x%x , size= %d \n",user->GetID(),type,recvsize);
				break;
			}
		}
		SendPacket(serial);
	}
}

void AdditionThread(LPVOID lpParameter)
{
	BOOL	isSrvInfo = FALSE;
	BOOL	isGuildSet = FALSE;
	WORD	cfgResetCount = 0;
	cHardwareInfo	hwInfo;
	_log("Start AdditionThread ... ");
	DWORD	dwSSCheckTime = timeGetTime();

	g_currentTime.update();

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_ADDITIONAL);

	while(g_bThreadLoop)
	{
		Sleep(1);

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_ADDITIONAL);

		if	(isReady<eIsReady_GuildInfo)
			continue;
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	게임서버들이 모두 정상적으로 가동하기 시작하면 USERDB에 등록한다.
		if	(isReady<eIsReady_Complete)
		{	//	로그인 서버와 어드민툴을 제외하고 게임서버가 g_iGameServerCount 만큼 정상적으로 접속했는지 확인해본다.
			int iConnectGameServerCount	=	0;

			for	(int count=0;count<dWORLD_MAX_USER_COUNT;count++)
			{	
				CClient * user = UserManager.GetClientPnt(count,FALSE);

				if	(!user || user->GetSerial()==0xffff)
					continue;

				if	(user->m_wIsGameServer && user->isReadyToPlay)
					iConnectGameServerCount++;
			}

			if	(iConnectGameServerCount	>=	g_iGameServerCount	||	g_bIsDevServer)
			{	
				SU_LOGIN	sLogin;

				sLogin.base.set(sizeof(SU_LOGIN),dSU_LOGIN,0);
				strcpy(sLogin.id,strWorldName);
				strcpy(sLogin.ip,l_strWorldIp);

				sLogin.wServerIndex = g_iWorldServerIndex;
				sLogin.wServerListIndex = wServerListIndex;

				_log(" Try CONNECT to ACCOUNT DB : %s , %s , idx %d",sLogin.id,sLogin.ip,sLogin.wServerIndex);
				sLogin.wType		=	g_iWorldType;	//	test
				sLogin.m_bf1IsInnerOpen=	g_bIsInnerTestServer;	//	test
				sLogin.wUserCount	=	UserManager.GetAllUserCount();
				sLogin.wMessage		=	l_wMessage;
				GetIDDBSocket()->SendPacket((char *)&sLogin,sLogin.base.wSize);

				isReady				=	eIsReady_Complete;
				_log(" isReady = %d ",isReady);
				_log(" Let Start World Server...............!! ");
			}
			continue;
		}
//┃	
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	현재의 각 게임 서버, 동접자 정보를 DB로 전송한다.
		{
			int	iOldDay		=	g_currentTime.m_wDay;
			int	iOldMinute	=	g_currentTime.m_wMinute;
			
			g_currentTime.update();
			
			if	(g_currentTime.m_wDay	!=	iOldDay)
			{
				g_iBoostGoldByTreasureMapEventCount	=	0;
				g_iBoostExpByTreasureMapEventCount	=	0;
				
				sendGSPChangeDay(NULL);
			}

			if	(iOldMinute	!=	g_currentTime.m_wMinute)
			{
				printf("- discount %d-\n",l_iCountForDisUser);

				l_iCountForDisUser = 0;
				
				//	지현추가 : USER DB에 서버 정보를 갱신한다. 분단위로 갱신하자.
				WU_WORLDINFO	sSInfo;
				memset(&sSInfo,0,sizeof(WU_WORLDINFO));
				sSInfo.base.set(sizeof(WU_WORLDINFO),dWU_WORLDINFO,-1);
				sSInfo.info[0].idx = (g_iWorldServerIndex+1) * 100;
				sSInfo.info[0].worldidx = (g_iWorldServerIndex+1);
				strcpy(sSInfo.info[0].ip,l_strWorldIp);
				sSInfo.info[0].servertype = 0;
				sSInfo.info[0].curusercount = 0;
				sSInfo.info[0].maxusercounttoday = 0;

				for(int count=0;count<dWORLD_MAX_USER_COUNT;count++)
				{	
					CClient * user = UserManager.GetClientPnt(count,FALSE);

					if	(!user || user->GetSerial()==0xffff || user->m_wServerIndex==0xffff)
						continue;

					if	(STRICMP(user->GetID(),dLOGIN_SERVER_ID)==0 || STRICMP(user->GetID(),dADMIN_TOOL_ID)==0)
						continue;

					sSInfo.info[user->m_wServerIndex+1].idx = (g_iWorldServerIndex+1) * 100 + user->m_wServerIndex+1;
					sSInfo.info[user->m_wServerIndex+1].worldidx = (g_iWorldServerIndex+1);
					sSInfo.info[user->m_wServerIndex+1].servertype = 1;
					strcpy(sSInfo.info[user->m_wServerIndex+1].ip,user->GetAddr());
					sSInfo.info[user->m_wServerIndex+1].curusercount = user->m_wCurrentPlayerCount;
					sSInfo.info[user->m_wServerIndex+1].maxusercounttoday =0;	//	기억하게 해봅시다.
					sSInfo.info[0].maxusercounttoday += sSInfo.info[user->m_wServerIndex+1].curusercount;
					sSInfo.info[0].curusercount++;
				}
				GetIDDBSocket()->SendPacket((char *)&sSInfo,sSInfo.base.wSize);
			}
		}
//┃	
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	길드전 메니져의 시간 업데이트
		if	(g_guildBattleManager.update())
		{
			sendGSPGuildBattleSchedule(NULL);
			sendGSPResetWeekFightGuildInfo(NULL);
		}
//┃	
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	이벤트 정보 업데이트
		g_eventManager.update();
//┃	
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	파티 정보 업데이트
		g_partyManager.update();
//┃	
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		
		//		for (int i=0;i<dGUILD_BATTLE_COUNT;i++)
		//			forceSetTodayGuildBattleSchedule(NULL,i,0,2,3,TRUE);
		
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	불량 유저 처리
		for(int count=0;count<dWORLD_MAX_USER_COUNT;count++)
		{	
			CClient * user = UserManager.GetClientPnt(count,FALSE);
			
			if	(!user || user->GetSerial()==0xffff)
				continue;
			if	(STRICMP(user->GetID(),dLOGIN_SERVER_ID)==0)
				continue;
			
			if	(user->GetIsUsed())
			{
				for(int c=0;c<dGAME_MAX_USER_COUNT;c++)
				{	
					Avatar_Data	* ava = user->GameClientMagager.GetAvatarPnt(c,FALSE);
					
					if	(ava!=NULL && ava->wSerial!=0xffff && ava->isWaste())
					{	
#ifdef _FOR_KOREA
						_log("일정시간 데이타가 날아오지 않은 유저는 자른다. [%d][%s][%s]",c,ava->strMaster,ava->data.m_strId);
#endif
						DeleteUser(user, ava->data.m_strId,"EMPTY",0);
					}
				}
			}
		}
//┃	
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	기타 작업들
		if	(cSRVUTIL::GetSec()%15==0 && !isSrvInfo && !g_bOffServer)	//	1분에 한번씩 
		{	

#ifdef dUSE_SERVERSTATUS_REPORT
			if	(!g_bIsIgnoreSSInfo)
			{	//	30초에 한번씩 DB에 현재 정보를 갱신해준다.
				if( g_SS.isReady() )
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

			//	USERDB에 일단 이름,IP등의 기본정보를 넘긴다.
			//	user db와의 컨텍트 검사 ..
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃		Account DB와 연결이 정상적인가?
			if(GetIDDBSocket()->sock == 0)
			{
				if(!GetIDDBSocket()->init(dwIDDBPort,strIDDBIp,"USER DBC",sizeof(cMSG_BASE_TYPE_FORUSERDB)))
				{
					GetIDDBSocket()->isLoopRecv = FALSE;
					GetIDDBSocket()->kill();
					_log("RECONNECT : Fail Connect User db !!\n");
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
						strcpy(sLogin.id,strWorldName);
						strcpy(sLogin.ip,l_strWorldIp);
						sLogin.wType		= g_iWorldType;	//	test
						sLogin.wUserCount	= UserManager.GetAllUserCount();
						sLogin.wMessage		=	l_wMessage;
						sLogin.wServerIndex = g_iWorldServerIndex;
						sLogin.wServerListIndex = wServerListIndex;
						
						GetIDDBSocket()->SendPacket((char *)&sLogin,sLogin.base.wSize);
					}
				}
			}

			if	(GetIDDBSocket()->sock!=0)
			{
				SU_INFO	sPacket;
				sPacket.base.set(sizeof(SU_INFO),dSU_INFO,0);
				sPacket.wType			=	g_iWorldType;
				sPacket.wMessage		=	l_wMessage;
				sPacket.m_bf1IsInnerOpen=	g_bIsInnerTestServer;
				sPacket.wUserCount		=	UserManager.GetAllUserCount();

				GetIDDBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
				isSrvInfo = TRUE;
				
				CM_DUMMY	dummy;
				dummy.base.set(sizeof(CM_DUMMY),dCM_DUMMY);
				GetIDDBSocket()->SendPacket((char *)&dummy,dummy.base.wSize);
			}
//┃		Account DB와 연결이 정상적인가?
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃		Avatar DB와 연결이 정상적인가?
			if	(GetADBSocket()->sock == 0)
			{
				if	(!GetADBSocket()->init(dwAvatarDBPort,strAvatarDBIp,"AVATAR DBC",sizeof(cMSG_BASE_TYPE_FORDB)))
				{
					GetADBSocket()->isLoopRecv = FALSE;
					GetADBSocket()->kill();
					_log("RECONNECT : Fail Connect AVATAR db !!\n");
				}
				else
				{
					hDBThread = (HANDLE)_beginthread(DBCtlThread, 0, NULL); 

					if	(hDBThread==NULL)
					{	
						_log("RECONNECT : Fail Create DBCtlThread");
					}else
					{
						_log("RECONNECT : [%s] [%d]",strAvatarDBIp,dwAvatarDBPort);
					}
				}
			}
			if	(GetADBSocket()->sock	!=	0)
			{
				CM_DUMMY	dummy;
				dummy.base.set(sizeof(CM_DUMMY),dCM_DUMMY);
				GetADBSocket()->SendPacket((char *)&dummy,dummy.base.wSize);
				
				WDMSG_ASK_EVENTS_INFO sPacket;
				sPacket.base.set(sizeof(WDMSG_ASK_EVENTS_INFO),dWDMSG_ASK_EVENTS_INFO);
				sPacket.isReally = 0;

				GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
			}
//┃		Avatar DB와 연결이 정상적인가?
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
			
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃		Admin이 접속해 있으면 그놈에게 현재 접속된 게임 리스트를 보낸다.
			if(isAdmin)	//	admin이 한명이라도 있다면...
			{
				//					_log("Admin info를 보낸다. %d \n",cSRVUTIL::GetTimes());
				WORLDtoSERVERS_SENDINFOTOADMIN		sendInfo;
				memset(&sendInfo,0,sizeof(WORLDtoSERVERS_SENDINFOTOADMIN));
				sendInfo.base.set(sizeof(WORLDtoSERVERS_SENDINFOTOADMIN),dWORLDtoSERVERS_SENDINFOTOADMIN);
				//					_log("총 접속한 서버들 : %d \n",UserManager.Size());
				sendInfo.wGameSrvCount = UserManager.Size()-2;	//	login, admin은 빼고.. 
				sendInfo.wUserCount = g_dwUserCount;
				sendInfo.wMaxUserCount = g_dwUserCount;
				
				for(int i=0;i<dMAX_ADMIN_COUNT;i++)
				{
					if(!_Admin[i].isUsed)	continue;
					CClient * user = UserManager.GetClientPnt(_Admin[i].iAdminSerial);
					if(user && user->GetSerial()!=0xffff)
					{
						user->AddSendPacket((char *)&sendInfo,sendInfo.base.wSize);
						SendPacket(_Admin[i].iAdminSerial);
					}
				}
			}
//┃		Admin이 접속해 있으면 그놈에게 현재 접속된 게임 리스트를 보낸다.			
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃		동접자 현황을 파일에 기록한다.
			//	기록들을 저장한다.
			if(g_DAY != (DWORD)cSRVUTIL::GetTime())
			{
				g_DAY = cSRVUTIL::GetTime();
				memset(g_dwMaxTimeUserCount,0,sizeof(DWORD)*24);
				memset(g_dwMaxTimeYahooUserCount,0,sizeof(DWORD)*24);
				g_dwMaxTimeUserCount[cSRVUTIL::GetHour()] = g_dwUserCount;
				g_dwMaxTimeYahooUserCount[cSRVUTIL::GetHour()] = g_dwYahooUserCount;
				g_dwMaxTimeParanUserCount[cSRVUTIL::GetHour()] = g_dwParanUserCount;
				g_dwMaxDayUserCount = g_dwUserCount;
				g_dwMaxDayYahooUserCount = g_dwYahooUserCount;
				g_dwMaxDayParanUserCount = g_dwParanUserCount;
			}

			FILE *fp;
			char filename[1024];
			sprintf(filename,"%s\\%d_Score.txt",strScorePath,cSRVUTIL::GetTime(),cSRVUTIL::GetHour());

			if	((fp=fopen(filename,"w"))!=NULL)
			{
#ifdef _FOR_KOREA
				fprintf(fp,"MAXUSER = %d | %d | %d | %d | %d | %d;\n",g_dwMaxUserCount,g_dwMaxDayUserCount,g_dwMaxYahooUserCount,g_dwMaxDayYahooUserCount,g_dwMaxParanUserCount,g_dwMaxDayParanUserCount);	//	최대 동접 , 오늘 하루의 최대 동접
#else
				fprintf(fp,"MAXUSER = %d | %d ;\n",g_dwMaxUserCount,g_dwMaxDayUserCount);	//	최대 동접 , 오늘 하루의 최대 동접
#endif
				
				for(int t=0;t<24;t++)
				{
#ifdef _FOR_KOREA
					if(t>cSRVUTIL::GetHour())	fprintf(fp,"T%d = %d | %d | %d;\n",t,0xffff,0xffff,0xffff);
					else						fprintf(fp,"T%d = %d | %d | %d;\n",t,g_dwMaxTimeUserCount[t], g_dwMaxTimeYahooUserCount[t], g_dwMaxTimeParanUserCount[t]);
#else
					if(t>cSRVUTIL::GetHour())	fprintf(fp,"T%d = %d;\n",t,0xffff);
					else						fprintf(fp,"T%d = %d;\n",t,g_dwMaxTimeUserCount[t]);
#endif
				}
				fclose(fp);
			}
//┃		동접자 현황을 파일에 기록한다.
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃		정기점검인지 확인한다.
			if(wServerCheckType==eServerCheckType_NORMAL && wHourFixCheck > -1)
			{	//	Test Server : 	화,토요일 새벽 3시 30분 
				if(g_iWorldType && (cSRVUTIL::GetWeekDay()==wWeekFixCheck || cSRVUTIL::GetWeekDay()==wWeekTestFixCheck))
				{
					int cHour,cMin;
					cHour = wHourFixCheck;
					cMin = wMinFixCheck;//cSRVUTIL::CalcMin(,2,0,&cHour,NULL);
					if(cSRVUTIL::GetHour()==cHour && cSRVUTIL::GetMin()>=cMin && cSRVUTIL::GetMin()< (cMin+4))
					{
						wServerCheckType = eServerCheckType_STOPREADY;
					}
				}else
				{
					if(!g_iWorldType && cSRVUTIL::GetWeekDay() ==wWeekFixCheck)
					{
						int cHour,cMin;
						cHour = wHourFixCheck;
						cMin = wMinFixCheck;//cSRVUTIL::CalcMin(wMinFixCheck,2,0,&cHour,NULL);
						if(cSRVUTIL::GetHour()==cHour && cSRVUTIL::GetMin()>=cMin && cSRVUTIL::GetMin()< (cMin+4))
						{
							wServerCheckType = eServerCheckType_STOPREADY;
						}
					}
				}
			}else
			{
				if(wServerCheckType == eServerCheckType_STOPREADY)
				{
					WORLDtoSERVERS_ALL_MSG		allMsg;
					allMsg.base.set(sizeof(WORLDtoSERVERS_ALL_MSG),dWORLDtoSERVERS_ALL_MSG);
					int spareTime = wMinFixCheck - cSRVUTIL::GetMin();
					if(spareTime<0)	spareTime += 60;
#ifdef _FOR_KOREA
					if(g_iWorldType)		//	Test Server .... 
					{
						strcpy(allMsg.strMessage,strMsgFixCheck);
					}else{
						strcpy(allMsg.strMessage,strMsgFixCheck);
					}
#else
					if(g_iWorldType)		//	Test Server .... 
					{
						strcpy(allMsg.strMessage,strMsgFixCheck);
					}else{
						strcpy(allMsg.strMessage,strMsgFixCheck);
					}
#endif
					SendDataAllGameServer((char *)&allMsg,allMsg.base.wSize);
					wServerCheckType = eServerCheckType_STOP;
					WORLDtoSERVERS_CHECKTIME		checkTime;
					checkTime.base.set(sizeof(WORLDtoSERVERS_CHECKTIME),dWORLDtoSERVERS_CHECKTIME);
					checkTime.wType = 1;
					SendDataAllGameServer((char *)&checkTime,checkTime.base.wSize);

					WDMSG_MT_TIME_START	sPacket;
					GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
				}else
				{
					if(wServerCheckType == eServerCheckType_STOP)
					{
						if(cSRVUTIL::GetHour()==wHourFixEndCheck && cSRVUTIL::GetMin()>=wMinFixEndCheck)
						{
							wServerCheckType = eServerCheckType_NORMAL;
							WORLDtoSERVERS_CHECKTIME		checkTime;
							checkTime.base.set(sizeof(WORLDtoSERVERS_CHECKTIME),dWORLDtoSERVERS_CHECKTIME);
							checkTime.wType = 0;
							SendDataAllGameServer((char *)&checkTime,checkTime.base.wSize);
							
							WDMSG_MT_TIME_END	sPacket;
							GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
						}
					}
					
					if(g_iWorldType && cSRVUTIL::GetWeekDay() == wWeekFixCheck)		//	Test Server ....
					{
						if(wServerCheckType && cSRVUTIL::GetHour()>=wHourFixEndCheck && cSRVUTIL::GetMin()>=wMinFixEndCheck)
						{
							WORLDtoSERVERS_CHECKTIME		checkTime;
							checkTime.base.set(sizeof(WORLDtoSERVERS_CHECKTIME),dWORLDtoSERVERS_CHECKTIME);
							checkTime.wType = 0;
							wServerCheckType = eServerCheckType_NORMAL;
							SendDataAllGameServer((char *)&checkTime,checkTime.base.wSize);
							
							WDMSG_MT_TIME_END	sPacket;
							GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
						}
					}
				}
			}
//┃		정기점검인지 확인한다.
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃		길드 관련 점검을 실시한다.
#ifndef _OGP_SERVICE
			{
				g_guildManager.operate1MinuteWork();	//	주기적으로 처리해야 하는 길드 작업 처리
//				아래 코드들은 모두
//				cGuildManagerForWS::operate1MinuteWork 으로 옮겨갔다. 만약을 위해 백업 코드로 남겨둠
// 				int iGuildSaveMaxCount = 0;

// 		if	(lpGuild->m_bf6LastGVGBattleYear == 0)
// 		{
// 			lpGuild->m_bf6LastGVGBattleYear		:	6;	//	마지막으로 GVG 서버에서 전투한 시간
// 			lpGuild->m_bf4LastGVGBattleMonth		:	4;
// 			lpGuild->m_bf5LastGVGBattleDay		:	5;
// 		}

// 				for(WORD i=dGUILD_MAX_COUNT;i;)
// 				{	
// 					--i;
// 
// 					cGuild * _pGuild = g_guildManager.GetGuildPnt(i);
// 
// 					if	(!_pGuild || _pGuild->m_wSerial==0xffff || !_pGuild->m_timeLastConnectedMasterTime.m_dwValue || !_pGuild->m_timeLastConnectTime.m_dwValue)
// 						continue;
// 
// 					//	만약 일정기간동안 해당 길드의 어떤 길드원도 들어오지 않는다면 이 길드는 사라진다.
// 					int iCheckday = cSRVUTIL::CalcDayOnToday(_pGuild->m_timeLastConnectTime.year+2000,_pGuild->m_timeLastConnectTime.month,_pGuild->m_timeLastConnectTime.day);
// 
// 					if	(iCheckday > dGUILD_DORMANCYLIMITPERIOD_FORGUILD)
// 					{
// 						_log("DORMANCY GUILD DELETE : [%d:%s, master (%s) , lastconnected date : %d/%d/%d - %d] ", _pGuild->m_wSerial,_pGuild->m_strName,_pGuild->m_strMasterName,_pGuild->m_timeLastConnectTime.year,_pGuild->m_timeLastConnectTime.month,_pGuild->m_timeLastConnectTime.day,iCheckday);
// 						DeleteGuildPacket(0xffff,_pGuild->m_wSerial,_pGuild->m_strName,"", eDGR_TIME_OVER);
// 						DeleteGuild(_pGuild->m_wSerial,_pGuild->m_strName,"Can not found Next Master.");
// 						continue;
// 					}
// 
// 					//	만약 한달동안 길드 마스터가 접속하지 않았다면, 마스터의 권한은 다른 유저에게 넘어간다.
// 			
// 					iCheckday = cSRVUTIL::CalcDayOnToday(_pGuild->m_timeLastConnectedMasterTime.year+2000,_pGuild->m_timeLastConnectedMasterTime.month,_pGuild->m_timeLastConnectedMasterTime.day);
// 
// 					if	(iCheckday > dGUILD_DORMANCYLIMITPERIOD_FORGUILDMASTER)
// 					{
// 						if	(_pGuild->m_timeBeginGuildMaster.year>20)
// 							continue;
// 
// 						_log("DORMANCY GUILD MASTER DELETE : [%d:%s, master (%s) , lastconnected date : %d/%d/%d - %d] ", _pGuild->m_wSerial,_pGuild->m_strName,_pGuild->m_strMasterName,_pGuild->m_timeLastConnectedMasterTime.year,_pGuild->m_timeLastConnectedMasterTime.month,_pGuild->m_timeLastConnectedMasterTime.day,iCheckday);
// 
// 						CGuildMan * _pNextMaster = NULL;
// 
// 						if	(_pGuild->GetCount()==1 || !(_pNextMaster = _pGuild->GetNextMaster()) )	//	이 길드는 삭제한다.
// 						{
// 							_log("CHECK POINT : Can not found Next Master. And this guild is delete!!");
// 							DeleteGuildPacket(0xffff,_pGuild->m_wSerial,_pGuild->m_strName,"", eDGR_TIME_OVER);
// 							DeleteGuild(_pGuild->m_wSerial,_pGuild->m_strName,"Can not found Next Master.");
// 						}
// 						else
// 						{
// 							//	DB에 랭크 변경을 요청한다.
// 							WDMSG_CHANGERANK	sPacket;
// 							sPacket.base.set(sizeof(WDMSG_CHANGERANK),dWDMSG_CHANGERANK);
// 							sPacket.dwGameServerSerial = 0xffff;
// 							sPacket.wIndex = _pGuild->m_wSerial;
// 							strcpy(sPacket.strName,_pGuild->m_strName);
// 							strcpy(sPacket.strMaster,_pGuild->m_strMasterName);
// 							strcpy(sPacket.strTarget,_pNextMaster->m_strName);
// 							sPacket.wRank = dGUILD_CLASS_MASTER;
// 							sPacket.wReason = eCGR_TIME_OVER;
// 							//	현재 마스터의 랭크를 아래와 같이 수정하자.
// 
// 							if	(cSRVUTIL::CalcDayOnToday(_pGuild->m_timeBeginGuildMaster.year+2000,_pGuild->m_timeBeginGuildMaster.month,_pGuild->m_timeBeginGuildMaster.day) < dGUILD_MINLIMITPERIOD_FORCHANGINGELDER)
// 								sPacket.wRank2 = dGUILD_CLASS_MEMBER;	//	일반 맴버
// 							else
// 								sPacket.wRank2 = dGUILD_CLASS_ELDER;	//	원로.
// 
// 							sPacket.wType = dGUILD_APPONT;
// 							GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
// 
// 							//	변경 내용은 중간중간 갱신되는 패킷을 통해 해결하자.
// 							int tomorrow = cSRVUTIL::GetNextTime((_pGuild->m_timeLastConnectedMasterTime.year+2000)*10000 + _pGuild->m_timeLastConnectedMasterTime.month*100 + _pGuild->m_timeLastConnectedMasterTime.day);
// 
// 							_pGuild->m_timeLastConnectedMasterTime.year = (tomorrow /10000)%100;
// 							_pGuild->m_timeLastConnectedMasterTime.month = (tomorrow %10000)/100;
// 							_pGuild->m_timeLastConnectedMasterTime.day = tomorrow %100;
// 
// 							SaveGuildBaseData(_pGuild);
// 						}
// 					}
// 
// 					if	(abs(timeGetTime()-_pGuild->dwUpdateTime) > 600000)	//	10분에 한번
// 					{
// 						if	((abs(timeGetTime()-_pGuild->dwUpdateTime) < 1200000) && iGuildSaveMaxCount>=10)
// 							continue;
// 
// 						_pGuild->dwUpdateTime = timeGetTime();
// 						UpdateGuildData(_pGuild);
// 						++iGuildSaveMaxCount;
// 					}
// 				}
			}
#endif
			isSrvInfo = TRUE;
//┃		길드 관련 점검을 실시한다.
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━			
		}
		else 
		if	(cSRVUTIL::GetSec()%15!=0)
			isSrvInfo = FALSE;	
//┃	
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	}

	hAdditionThread	=	NULL;
	
	g_lifeCodeMgr.endCheck(eLIFE_CODE_ADDITIONAL);

	_log("End AdditionThread ... ");
	return ;
}

void DBCtlThread(LPVOID lpParameter)
{
	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	WORD	type,recvsize;
	_log("Start DBCtlThread ... ");


	WORLDtoSERVERS_WORLDNAME	rPacket;
	rPacket.base.set( sizeof(WORLDtoSERVERS_WORLDNAME), dWORLDtoSERVERS_WORLDNAME);
	rPacket.wServerType	=	g_iWorldType;

	strcpy(rPacket.strWorldName , strWorldName);
	GetADBSocket()->SendPacket(( char  *)&rPacket,rPacket.base.wSize);

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_DBC);

	while(GetADBSocket()->isLoopRecv)
	{
		Sleep(1);

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_DBC);

		while(GetADBSocket()->GetRecvPacket(recvpacket))
		{
			recvsize = *(WORD *)recvpacket;
			type = *(WORD *)(recvpacket+2);

			if	(type == dCM_COMPRESS)
			{
				CM_COMPRESS		comp;

				memcpy(&comp,recvpacket,recvsize);
				cSRVUTIL::Uncompress((unsigned char *)comp.data,(unsigned char *)recvpacket,comp.wSize - sizeof(WORD)*3,comp.wSourceSize);
				recvsize = *(WORD *)recvpacket;
				type = *(WORD *)(recvpacket+2);
			}

			switch(type)
			{
				case dDLMSG_COPY_DATA_RESULT	:
				{
					CClient * loginusr = UserManager.GetClientPnt(dLOGIN_SERVER_ID);

					if	(loginusr)
						loginusr->AddSendPacket(recvpacket,recvsize);
					break;
				}

				case dSERVERStoWORLD_LIFE_SIGN	:
				{
					OperateLifeSignPacket(UserManager.GetClientPnt(dLOGIN_SERVER_ID),(SERVERStoWORLD_LIFE_SIGN *)recvpacket);
					break;
				}

				case dDLMSG_GETAVATARS:
				{
					DLMSG_GETAVATARS * rPacket;
					rPacket = (DLMSG_GETAVATARS *)recvpacket;
					
					CClient	* user = UserManager.GetClientPnt((WORD)rPacket->worldSerial);
					
					if(!user)
						break;
					//#ifdef  _FOR_CHINA
					//					printf("[%s] send avalist \n=========================\n",rPacket->strID);
					//					for(int te = 0;te<4;te++)
					//						printf("%d name [%s], lev [%d] \n",rPacket->aCharacter[te].m_wAvatarIndex,rPacket->aCharacter[te].m_strName,rPacket->aCharacter[te].m_wLevel);
					//#endif
					WORLDtoSERVERS_GETAVAS	sGetAvas;
					memset(&sGetAvas,0,sizeof(WORLDtoSERVERS_GETAVAS));
					sGetAvas.base.set(sizeof(WORLDtoSERVERS_GETAVAS),dWORLDtoSERVERS_GETAVAS);
					sGetAvas.loginSerial = rPacket->u_serial;
					strcpy(sGetAvas.strID,rPacket->strID);
					memcpy(sGetAvas.aAvatarList,rPacket->aCharacter,sizeof(cAVATAR_INFO)*dMAX_AVATAR_COUNT);
					//					printf(" [%s]'s AvatarList \n",rPacket->strID);
					//	각 아바타의 마지막 필드를 확인해서 주소를 삽입해서 준다.
					for(int i =0; i<dMAX_AVATAR_COUNT; i++)
					{	
						if(strcmp(sGetAvas.aAvatarList[i].m_strName,"EMPTY")==0 || sGetAvas.aAvatarList[i].m_wAvatarIndex==0xffff)
						{
							memset(sGetAvas.aAvatarList[i].m_strIP,0,dIP_SIZE);
#ifdef  _FOR_CHINA
							printf("ERROR %d name [%s], lev [%d] \n",rPacket->aCharacter[i].m_wAvatarIndex,rPacket->aCharacter[i].m_strName,rPacket->aCharacter[i].m_wLevel);
#endif
							continue;
						}
						
						WORD	_wIsPremium = 0;
						CClient * cli = UserManager.GetClientOfField(rPacket->aCharacter[i].m_wLastField,1,_wIsPremium);
						if(!cli)
						{	
#ifdef _FOR_KOREA
							printf("해당 서버를 찾을수 없다. -0- field =%d\n",rPacket->aCharacter[i].m_wLastField);
#else
							printf("Not found the game server with this field number =%d\n",rPacket->aCharacter[i].m_wLastField);
#endif
							strcpy(sGetAvas.aAvatarList[i].m_strIP,"");
						}
						else
						{
							switch(cli->wSendIndex)
							{
							case 0:
							case 1:
								//								printf("1. idx %d, ip address of target server : %s\n",i,cli->strConnectaddr[cli->wCurIndex]);
								if(cli->wConnectCount>1)
								{
									strcpy(sGetAvas.aAvatarList[i].m_strIP,cli->strConnectaddr[cli->wCurIndex]);
									//									printf("ip address : %s \n",sGetAvas.aAvatarList[i].m_strIP);
									cli->wConnectCount--;
									if(cli->wConnectCount==0)	cli->wSendIndex = 2;
								}else{
									strcpy(sGetAvas.aAvatarList[i].m_strIP,cli->strConnectaddr[0]);
								}
								break;
							case 2:
								//								printf("2. idx %d, ip address of target server : %s\n",i,cli->strConnectaddr[cli->wCurIndex]);
								strcpy(sGetAvas.aAvatarList[i].m_strIP,cli->strConnectaddr[cli->wCurIndex]);
								if(cli->wConnectCount>1)
								{
									cli->wCurIndex++;
									if(cli->wCurIndex>1)	cli->wCurIndex = 0;
								}
								break;
							}
						}
					}
					//					printf(" field %d : goto [%s] \n",sGetAvas.aAvatarList[0].m_wLastField,sGetAvas.aAvatarList[0].m_strIP);
					//					printf(" field %d : goto [%s] \n",sGetAvas.aAvatarList[1].m_wLastField,sGetAvas.aAvatarList[1].m_strIP);
					//					printf(" field %d : goto [%s] \n",sGetAvas.aAvatarList[2].m_wLastField,sGetAvas.aAvatarList[2].m_strIP);
					//					printf(" field %d : goto [%s] \n",sGetAvas.aAvatarList[3].m_wLastField,sGetAvas.aAvatarList[3].m_strIP);
					
					user->AddSendPacket((char *)&sGetAvas,sGetAvas.base.wSize);
					SendPacket(user->GetSerial());
					break;
				}

				case dDLMSG_CREATEAVATAR:
				{
					DLMSG_CREATEAVATAR * rPacket;
					rPacket = (DLMSG_CREATEAVATAR *)recvpacket;
					
					CClient	* user = UserManager.GetClientPnt((WORD)rPacket->worldSerial);
					if(!user)	break;

					//if(strlen(rPacket->aCharacter->m_strName) < 4)	break;
					
					WORLDtoSERVERS_CREATEAVA	sPacket;
					memset(&sPacket,0,sizeof(WORLDtoSERVERS_CREATEAVA));
					sPacket.base.set(sizeof(WORLDtoSERVERS_CREATEAVA),dWORLDtoSERVERS_CREATEAVA);
					strcpy(sPacket.strID,rPacket->strID);
					sPacket.loginSerial = rPacket->u_serial;
					sPacket.wResult = rPacket->wResult;
					sPacket.wIndex = rPacket->wIndex;
					memcpy(&sPacket.aCharacter,&rPacket->aCharacter,sizeof(cAVATAR_INFO));
					//최초 생성된 각 캐릭터의 필드번호는 DB에서 임시설정되어 넘어온다.
					WORD _wResult = 0;
					CClient * cli = UserManager.GetClientOfField(rPacket->aCharacter.m_wLastField,1,_wResult);
					if(!cli)		strcpy(sPacket.aCharacter.m_strIP,"");	
					else{
						switch(cli->wSendIndex)
						{
						case 0:
						case 1:
							if(cli->wConnectCount>1)
							{
								strcpy(sPacket.aCharacter.m_strIP,cli->strConnectaddr[cli->wCurIndex]);
								cli->wConnectCount--;
								if(cli->wConnectCount==0)	cli->wSendIndex = 2;
							}else{
								strcpy(sPacket.aCharacter.m_strIP,cli->strConnectaddr[0]);
							}
							break;
						case 2:
							strcpy(sPacket.aCharacter.m_strIP,cli->strConnectaddr[cli->wCurIndex]);
							if(cli->wConnectCount>1)
							{
								cli->wCurIndex++;	if(cli->wCurIndex>1)	cli->wCurIndex = 0;
							}
							
							break;
						}
					}
					user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
					SendPacket(user->GetSerial());
					break;
				}

				case dDLMSG_SELECTAVATAR:
				{
					DLMSG_SELECTAVATAR * rPacket;
					rPacket = (DLMSG_SELECTAVATAR *)recvpacket;
					
					CClient	* user = UserManager.GetClientPnt((WORD)rPacket->worldSerial);
					if(!user)
					{
						_log("dDLMSG_SELECTAVATAR : Not Found User ~ %s %s",rPacket->strID,rPacket->strNAME);
						break;
					}
					
					WORLDtoSERVERS_LOADAVA	sPacket;
					memset(&sPacket,0,sizeof(WORLDtoSERVERS_LOADAVA));
					sPacket.base.set(sizeof(WORLDtoSERVERS_LOADAVA),dWORLDtoSERVERS_LOADAVA);
					
					strcpy(sPacket.strID,rPacket->strID);
					strcpy(sPacket.strNAME,rPacket->strNAME);
					
					sPacket.wResult		=	rPacket->wResult;
					sPacket.loginSerial	=	rPacket->loginSerial;
					
					user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
					SendPacket(user->GetSerial());
					break;
				}
				case dDLMSG_DELETEAVATAR:
				{
					DLMSG_DELETEAVATAR * rPacket;
					rPacket = (DLMSG_DELETEAVATAR *)recvpacket;
					
					CClient	* user = UserManager.GetClientPnt((WORD)rPacket->worldSerial);
					if(!user)	break;
					
					WORLDtoSERVERS_DELETEAVA	sPacket;
					memset(&sPacket,0,sizeof(WORLDtoSERVERS_DELETEAVA));
					sPacket.base.set(sizeof(WORLDtoSERVERS_DELETEAVA),dWORLDtoSERVERS_DELETEAVA);
					strcpy(sPacket.strID,rPacket->strID);
					sPacket.loginSerial = rPacket->u_serial;
					sPacket.wResult = rPacket->wResult;
					user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
					SendPacket(user->GetSerial());
					if(sPacket.wResult == -1)
					{
						_log("ERROR : Can't delete Avatar [%s,%s]",rPacket->strID,rPacket->strName);
					}
					else
					{
						DeleteGuilder(rPacket->guildSerial,rPacket->strID,rPacket->strName);
						g_partyManager.removePartyMember(rPacket->strName);
					}
					break;
				}
				case dDGMSG_LOGINAVATAR:
				{
					DGMSG_LOGINAVATAR * rPacket;
					rPacket = (DGMSG_LOGINAVATAR *)recvpacket;
					
					CClient	* user = UserManager.GetClientPnt((WORD)rPacket->worldSerial);
					if(!user)
					{
						_log("ERROR dDGMSG_LOGINAVATAR: user= NULL");
						break;
					}
					LoadData(user,rPacket);
					break;
				}

				case dDWMSG_TRANS_AVATAR_TO_GVG_RESULT	:
				{
					DWMSG_TRANS_AVATAR_TO_GVG_RESULT	*lpPacket	=	(DWMSG_TRANS_AVATAR_TO_GVG_RESULT	*)recvpacket;

					operateTransAvatarToGVGResultDBPacket(UserManager.GetClientPnt(dLOGIN_SERVER_ID),lpPacket);
					break;
				}

				case	dD2W_ASK_GVG_AVATAR_ORG_DATA_RESULT	:
				{
					D2W_ASK_GVG_AVATAR_ORG_DATA_RESULT	*lpPacket	=	(D2W_ASK_GVG_AVATAR_ORG_DATA_RESULT	*)recvpacket;

					operateD2W_ASK_GVG_AVATAR_ORG_DATA_RESULT(UserManager.GetClientPnt(dLOGIN_SERVER_ID),lpPacket);
					break;					
				}

				case	dD2W_SIMPLE_WORK				:
				{
					D2W_SIMPLE_WORK	*lpPacket	=	(D2W_SIMPLE_WORK	*)recvpacket;

					operateD2W_SIMPLE_WORK(GetADBSocket(),UserManager.GetClientPnt(dLOGIN_SERVER_ID),lpPacket);
					break;
				}

				case	dD2W_GVG_AVATAR_ORG_DATA		:
				{
					operateD2W_GVG_AVATAR_ORG_DATA(UserManager.GetClientPnt(dLOGIN_SERVER_ID),(D2W_GVG_AVATAR_ORG_DATA	*)recvpacket);
					break;
				}
				//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
				//		길드 정보 교환 
				case dDWMSG_GETGUILD:
				{
					DWMSG_GETGUILD		sGuild;
					memset(&sGuild,0,sizeof(DWMSG_GETGUILD));
					memcpy(&sGuild,recvpacket,recvsize);
					
					iRecvGuildCount = sGuild.wCount;

					//	들어온 기본정보를 가지고 길드 메니져를 구성한다.
					for(int i=0;i<iRecvGuildCount;i++)
					{
						//	길드 기본 세팅
						cGuild	tmpGuild;	tmpGuild.close();

						tmpGuild.setSaveData_Column( &sGuild.List[i].dataColumn);
// 						if(IsFile(strLogFile))
// 						{
// 							sGuild.List[i].dataBinary.m_bf1IsUseGuildMasterGrace	= 0;
// 							sGuild.List[i].dataBinary.m_bf3GuildGoodwillForRoen		= 0;
//  						}
						tmpGuild.setSaveData_Binary( &sGuild.List[i].dataBinary);						
						//tmpGuild.setSaveData_BinaryForInitData( &sGuild.List[i].dataBinary);
						tmpGuild.setSaveData_Skill( &sGuild.List[i].dataSkill);
						tmpGuild.setSaveData_Mark( &sGuild.List[i].dataMark);
						tmpGuild.setSaveData_Bank( &sGuild.List[i].dataBank);
						tmpGuild.setSaveData_RelShip( &sGuild.List[i].dataRelShip);
						tmpGuild.setSaveData_Pet( &sGuild.List[i].dataPet);
						tmpGuild.setSaveData_Guardian( &sGuild.List[i].dataGuardian);
						tmpGuild.setSaveData_Inventory( &sGuild.List[i].dataInventory);
						tmpGuild.setSaveData_Structure( &sGuild.List[i].dataStructure);

						int	iMarkInfoSum	=	tmpGuild.m_guildMarkInfo.m_wIcon+
							tmpGuild.m_guildMarkInfo.m_bIconColor+
							tmpGuild.m_guildMarkInfo.m_bBaseShape+
							tmpGuild.m_guildMarkInfo.m_bBaseColorPartition+
							tmpGuild.m_guildMarkInfo.m_bBaseColor1+
							tmpGuild.m_guildMarkInfo.m_bBaseColor2+
							tmpGuild.m_guildMarkInfo.m_bBaseLineColor;
						
						if	(iMarkInfoSum	==	0)
							tmpGuild.m_guildMarkInfo.reset();

						if	(tmpGuild.m_bf6PeneltyBonusGuildPoint	>	c_iMaxBonusGuildPoint	||	
							tmpGuild.m_bf6BonusGuildPoint			>	c_iMaxBonusGuildPoint)
						{
							tmpGuild.m_bf6PeneltyBonusGuildPoint	=	0;
							tmpGuild.m_bf6BonusGuildPoint			=	0;
						}

						if	(tmpGuild.m_bf3ThisDayGuildBattleCount	>	g_iGuildBattleCountAtDay	||	
							tmpGuild.m_bf5ThisWeekGuildBattleCount	>	g_iGuildBattleCountAtWeek)
						{
							tmpGuild.m_bf3ThisDayGuildBattleCount	=	0;
							tmpGuild.m_bf5ThisWeekGuildBattleCount	=	0;
						}

						LoadGuildDataInFile(&tmpGuild);
						
						int	iCurrentGuildPoint	=	tmpGuild.getCurrentGuildPointSum();
						int	iCorrectGuildPoint	=	tmpGuild.getFullSkillPoint();
						
						if	(iCurrentGuildPoint	!=	iCorrectGuildPoint)
						{
							int	iDifferenceValue=	iCorrectGuildPoint-iCurrentGuildPoint;

							if	(iDifferenceValue	>	0)
								tmpGuild.m_wGuildPoint	+=	iDifferenceValue;
							else
							{
								if	(tmpGuild.m_wGuildPoint	>=	-iDifferenceValue)
									tmpGuild.m_wGuildPoint	+=	iDifferenceValue;
								else
								{
									tmpGuild.m_wGuildPoint			=	tmpGuild.getFullSkillPoint();
									
									memset(tmpGuild.m_abGuildSkill,0,100);

									_log("guild [%s] incorrect guild skill info!. reset the guild point.",tmpGuild.m_strName);

								}
							}
						}
						
						//	길드 매니져에 등록한다.
						tmpGuild.dwUpdateTime = timeGetTime() + ( l_iCountGuildSaveGap%(dGUILD_MAX_COUNT/40) )*15000;	//	세이브 주기를 변경한다.
						++l_iCountGuildSaveGap;
						WORD wResult = g_guildManager.Add(&tmpGuild);
						
						if(wResult == 0xffff)	//	등록실패
						{
							_log("ERROR Guild Regist Fail: Stop Server , please !!");	Sleep(10000);break;
						}
						else
						{
							printf("[%.4d][%s] ready to Guild\n",tmpGuild.m_wSerial,tmpGuild.m_strName);
						}
					}

					//					iRecvGuildCount = 0;
					if	(iRecvGuildCount == 0 )	//	길드가 없으니 맴버도 안오겠지..
					{
						_log(" Recv Guild Info ....  Complete ............");
						isReady = eIsReady_GuildInfo;
						_log(" isReady = %d ",isReady);
					}
					break;
				}
				case dDWMSG_GUILD_MEMBER:
				{
					DWMSG_GUILD_MEMBER		sGetMember;
					memset(&sGetMember,0,sizeof(DWMSG_GUILD_MEMBER));
					memcpy(&sGetMember,recvpacket,recvsize);
					
					//	등록된 길드에 넣어두고...
					cGuild	* guild = g_guildManager.GetGuildPntBySerial(sGetMember.wSerial);
					
					if	(!guild || guild->wSerial==0xffff)
					{
						if	(sGetMember.wCount>0)
							_log(" ERROR MEMBER INFO : Guild [%d] , Count [%d] , 1st [%s(%s)]",sGetMember.wSerial,sGetMember.wCount,sGetMember.members[0].strId,sGetMember.members[0].strName);
						else
							_log(" ERROR MEMBER INFO : Guild [%d] , Count [%d]",sGetMember.wSerial,sGetMember.wCount);
					}
					else
					{
						guild->m_wCongressManCount = 0;
						guild->m_wSubMasterCount = 0;
						guild->m_wMemberCount = 0;

						for(int i=0;i<sGetMember.wCount;i++)
						{
							CGuildMan	member;
							strcpy(member.m_strId	,	sGetMember.members[i].strId);
							strcpy(member.m_strName	,	sGetMember.members[i].strName);
							member.m_wJob			=	sGetMember.members[i].wJob;
							member.m_dwBestowValue	=	sGetMember.members[i].dwBestowValue;
							member.m_bf1IsConnected	=	FALSE;
							member.m_wLevel			=	sGetMember.members[i].wLevel;
							member.m_wRank			=	sGetMember.members[i].wRank;
							member.wSerial			=	0;
							member.m_bf3ReservePet	=	0;
							
							//	if(guild->Add(&member)!=0xffff)	guild->m_wMemberCount++;
							guild->Add(&member);
							
							if	(member.m_wRank == dGUILD_CLASS_PARLIAMENT)
								guild->m_wCongressManCount++;
							
							if	(member.m_wRank == dGUILD_CLASS_SUBMASTER	)
							{
								guild->m_wSubMasterCount++;
								//								_logf("submastercount plus: [%s] [%d] ",guild->m_strName,guild->m_wSubMasterCount);
							}
							
							if	(member.m_wRank == dGUILD_CLASS_MASTER	)
								strcpy(guild->m_strMasterName,member.m_strName);
						}

						printf("[%.4d][%s] Receive Guild member info [real %d][record %d] persons\n",guild->m_wSerial,guild->m_strName,guild->GetCount(),guild->m_wMemberCount);

						_logf(" [%d : %s]GUILD  %d(%d)persons .. [%d/%d] ",guild->m_wSerial,guild->m_strName,guild->GetCount(),guild->m_wMemberCount,guild->m_wSubMasterCount,guild->m_wMaxSubMaster);

						if	(guild->GetCount()	==	0)
						{
							char	strGuildName[dGUILD_NAME_LENGTH];
							int		iSerial	=	guild->m_wSerial;

							memcpy(strGuildName,guild->m_strName,dGUILD_NAME_LENGTH);
							DeleteGuild(guild->m_wSerial,guild->m_strName,"- Delete Guild by Member Count Zero");
							DeleteGuildPacket(0xffff,iSerial,strGuildName, "");
						}
						//						guild->updateGuildInfo();
					}
					
					if	(sGetMember.wIsEnd)
					{
						_log(" Recv Guild Member Info ....  Complete ............");

						if	(g_guildManager.m_bIsLoadGuildHallInfo	==	FALSE)	//	길드홀 정보를 파일에서 로딩 했다
						{
							g_guildManager.buildHallInfoByGuild();
							g_guildBattleManager.update(TRUE);
						}
						else
						{
							g_guildManager.checkHallInfoByGuild();				//	저장된 길드홀 정보가 없다.
						}

						isReady = eIsReady_GuildInfo;
						_log(" isReady = %d ",isReady);
					}
					break;
				}
				case dDWMSG_CREATEGUILD:
				{
					DWMSG_CREATEGUILD * rPacket;
					rPacket = (DWMSG_CREATEGUILD *)recvpacket;

					WORLDtoSERVERS_USER_CREATEGUILD		creGuild;
					creGuild.base.set(sizeof(WORLDtoSERVERS_USER_CREATEGUILD),dWORLDtoSERVERS_USER_CREATEGUILD);
					creGuild.Result = rPacket->wResult;
					creGuild.wGuildSerial = rPacket->wGuildSerial;
					strcpy(creGuild.strGuildName,rPacket->strGuildName);
					strcpy(creGuild.strMasterName,rPacket->strMasterName);
					strcpy(creGuild.strSubMasterName,rPacket->strSubMasterName);
					Avatar_Data	* ava = UserManager.GetGClientPnt(creGuild.strMasterName,1);
					if(ava==NULL || ava->wSerial == 0xffff)
					{
						if(creGuild.Result == dCREATEGUILD_RESULT_SUCCESS)
						{
#ifdef _FOR_KOREA
							_log("SSERROR 대에러 : DB는 등록되었는데, MASTER의 정보가 없다. [%d] [%s] [%s]",creGuild.wGuildSerial,creGuild.strGuildName,creGuild.strMasterName);
#endif
							creGuild.Result = dCREATEGUILD_RESULT_FAIL;
							//	DB에 DELETE 패킷 보낸다.
							DeleteGuild(creGuild.wGuildSerial,creGuild.strGuildName,"DB등록중 마스터의 정보가 없다.");
							DeleteGuildPacket(0xffff,creGuild.wGuildSerial,creGuild.strGuildName,"", eDGR_CREATE_FAIL);
							CClient * targetGameServer = UserManager.GetClientPnt((WORD)rPacket->dwGameServerSerial);
							if(targetGameServer && targetGameServer->GetSerial()!=0xffff)
							{
								targetGameServer->AddSendPacket((char *)&creGuild,creGuild.base.wSize);
								SendPacket(targetGameServer->GetSerial());
							}
						}else{
#ifdef _FOR_KOREA
							_log("SSERROR 대에러 : DB는 등록실패, MASTER의 정보가 없다. [%d] [%s] [%s]",creGuild.wGuildSerial,creGuild.strGuildName,creGuild.strMasterName);
#endif
						}
						break;
					}
					//					_logf("GUILD CREATE RESULT = %d, guild[%s] master[%s]",creGuild.Result,creGuild.strGuildName,creGuild.strMasterName);
					if(creGuild.Result == dCREATEGUILD_RESULT_SUCCESS)
					{	//	임시 등록된 정보를 갱신한다.
						cGuild * guild = g_guildManager.GetGuildPntBySerial(creGuild.wGuildSerial);
						if(guild == NULL || guild->wSerial==0xffff)
						{
#ifdef _FOR_KOREA
							_log("SSERROR 대에러 : DB는 등록되었는데, 임시 등록된 정보가 없다. [%d] [%s] [%s]",creGuild.wGuildSerial,creGuild.strGuildName,creGuild.strMasterName);
#endif
							creGuild.Result = dCREATEGUILD_RESULT_FAIL;
							//	DB에 DELETE 패킷 보낸다.
							DeleteGuild(creGuild.wGuildSerial,creGuild.strGuildName,"임시 등록된 정보가 없다.");
							DeleteGuildPacket(0xffff,creGuild.wGuildSerial,creGuild.strGuildName,"", eDGR_CREATE_FAIL);
							CClient * targetGameServer = UserManager.GetClientPnt((WORD)rPacket->dwGameServerSerial);
							if(targetGameServer && targetGameServer->GetSerial()!=0xffff)
							{
								targetGameServer->AddSendPacket((char *)&creGuild,creGuild.base.wSize);
								SendPacket(targetGameServer->GetSerial());
							}
							break;
						}
						else
						{
							CGuildMan		master;
							strcpy(master.m_strId,ava->data.m_strId);
							strcpy(master.m_strName,ava->data.m_strName);

							master.m_bf1IsConnected =	TRUE;
							master.m_wJob			=	ava->data.m_wJob;
							master.m_wLevel			=	ava->data.m_iLevel;
							master.m_wRank			=	dGUILD_CLASS_MASTER;
							
							int	l_iMasterGuildSerial = guild->Add(&master);
							
							if(l_iMasterGuildSerial==0xffff)	//	등록 실패.
							{
#ifdef _FOR_KOREA
								_log("SSERROR 대에러 : MASTER등록 실패 -_-; [%d] [%s] [%s]",creGuild.wGuildSerial,creGuild.strGuildName,creGuild.strMasterName);
#endif
								creGuild.Result = dCREATEGUILD_RESULT_FAIL;
								//	DB에 DELETE 패킷 보낸다.
								DeleteGuild(creGuild.wGuildSerial,creGuild.strGuildName,"마스터 등록 실패");
								DeleteGuildPacket(0xffff,creGuild.wGuildSerial,creGuild.strGuildName,"", eDGR_CREATE_FAIL);
								CClient * targetGameServer = UserManager.GetClientPnt((WORD)rPacket->dwGameServerSerial);
								if(targetGameServer && targetGameServer->GetSerial()!=0xffff)
								{
									targetGameServer->AddSendPacket((char *)&creGuild,creGuild.base.wSize);
									SendPacket(targetGameServer->GetSerial());
								}
								break;
							}
							else
							{							//	등록 성공
								CClient * targetGameServer = UserManager.GetClientPnt((WORD)rPacket->dwGameServerSerial);

								if(!targetGameServer || targetGameServer->GetSerial()==0xffff)
								{
									creGuild.Result = dCREATEGUILD_RESULT_FAIL;
#ifdef _FOR_KOREA
									_log("SSERROR 대에러 : 해당 게임 서버가 없어짐 -_-; [%d] [%s] [%s]",creGuild.wGuildSerial,creGuild.strGuildName,creGuild.strMasterName);
#endif
									//	등록된 길드를 삭제한다.
									//	DB에 DELETE 패킷 보낸다.
									DeleteGuild(creGuild.wGuildSerial,creGuild.strGuildName,"생성을 요청한 게임 서버가 없어짐 ;;");
									DeleteGuildPacket(rPacket->dwGameServerSerial,creGuild.wGuildSerial,creGuild.strGuildName,"", eDGR_CREATE_FAIL);
									CClient * targetGameServer = UserManager.GetClientPnt((WORD)rPacket->dwGameServerSerial);
									if(targetGameServer && targetGameServer->GetSerial()!=0xffff)
									{
										targetGameServer->AddSendPacket((char *)&creGuild,creGuild.base.wSize);
										SendPacket(targetGameServer->GetSerial());
									}
									break;
								}
								else
								{
									//	게임 서버에 정상적으로 등록된 길드 정보를 보내준다.
									targetGameServer->AddSendPacket((char *)&creGuild,creGuild.base.wSize);
									SendPacket(targetGameServer->GetSerial());
									if(ava && ava->wSerial!=0xffff)
									{
										ava->data.m_wGuildRank = dGUILD_CLASS_MASTER;
										ava->data.m_wGuildSerial = guild->m_wSerial;
									}
									//	각 게임 서버에게 ..
									WORLDtoSERVERS_GUILD_CREATE	createG;
									createG.base.set(sizeof(WORLDtoSERVERS_GUILD_CREATE),dWORLDtoSERVERS_GUILD_CREATE);
									memcpy(&createG.info,(CGuildAdvanceInfo *)guild,sizeof(CGuildAdvanceInfo));
									createG.info.m_wMemberCount = 0;
									
									SendDataAllGameServer((char *)&createG,createG.base.wSize);
									//									_logf("[GUILD] Create Guild [%s]",createG.info.m_strName);
									
									//	master의 변경시간
									cSRVUTIL::GetUTime((cpUTime * )&guild->m_timeBeginGuildMaster);
									cSRVUTIL::GetUTime((cpUTime * )&guild->m_timeLastConnectTime);
									cSRVUTIL::GetUTime((cpUTime * )&guild->m_timeLastConnectedMasterTime);

									if	(ava && ava->wSerial!=0xffff)
									{
										WORLDtoSERVERS_GUILD_JOIN	joinG;
										joinG.base.set(sizeof(WORLDtoSERVERS_GUILD_JOIN),dWORLDtoSERVERS_GUILD_JOIN);
										joinG.member.wSerial= l_iMasterGuildSerial;
										joinG.wGuildSerial	= guild->m_wSerial;
										strcpy(joinG.strGuildName,guild->m_strName);
										joinG.member.m_dwBestowValue = 0;
										strcpy(joinG.member.m_strId,ava->data.m_strId);
										strcpy(joinG.member.m_strName,ava->data.m_strName);
										joinG.member.m_bf1IsConnected	=	1;
										joinG.member.m_wJob				=	ava->data.m_wJob;
										joinG.member.m_wLevel			=	ava->data.m_iLevel;
										joinG.member.m_wRank			=	ava->data.m_wGuildRank;

										SendDataAllGameServer((char *)&joinG,joinG.base.wSize);

										SendGuildMemberUpdateInfoToBCS(guild,&joinG.member);
										//										_logf("[GUILD] Create Guild [%s] Add member! [%s]",createG.info.m_strName,ava->data.m_strId);
									}else{
										_logf("[GUILD] Create Guild [%s] I'm fail to add member!! [%s]",createG.info.m_strName);
									}
								}
							}
						}
					}
					else
					{	//	임시 등록된 정보를 삭제한다.
						DeleteGuild(creGuild.wGuildSerial,creGuild.strGuildName,"DB에서 등록 실패래..");

						CClient * targetGameServer = UserManager.GetClientPnt((WORD)rPacket->dwGameServerSerial);

						if	(targetGameServer && targetGameServer->GetSerial()!=0xffff)
						{
							targetGameServer->AddSendPacket((char *)&creGuild,creGuild.base.wSize);
							SendPacket(targetGameServer->GetSerial());
						}
					}
					break;
				}
				case dDWMSG_DELETEGUILD:
				{
					DWMSG_DELETEGUILD * rPacket;
					rPacket = (DWMSG_DELETEGUILD *)recvpacket;
					
					CClient	* usr = UserManager.GetClientPnt((WORD)rPacket->dwGameServerSerial);

					if	(!usr || usr->GetSerial()==0xffff)
					{
#ifdef _FOR_KOREA
						_log(" [%s]길드 삭제를 [%s]에게 돌려줄수가 없다.",rPacket->strName,rPacket->strRequestMan);
#endif
						break;
					}

					//	신청자에게 삭제결과 돌려주구...
					WORLDtoSERVERS_USER_DELETEGUILD		packetDelGuild;

					packetDelGuild.base.set(sizeof(WORLDtoSERVERS_USER_DELETEGUILD),dWORLDtoSERVERS_USER_DELETEGUILD);
					packetDelGuild.wResult = rPacket->wResult;

					strcpy(packetDelGuild.strGuildName,rPacket->strName);
					strcpy(packetDelGuild.strRequestMan,rPacket->strRequestMan);
					packetDelGuild.wGuildSerial = rPacket->wGuildSerial;

					usr->AddSendPacket((char *)&packetDelGuild,packetDelGuild.base.wSize);

					SendPacket(usr->GetSerial());

					if	(rPacket->wResult != dDELETEGUILD_RESULT_SUCCESS)
					{
#ifdef _FOR_KOREA
						_log(" [%s]길드 삭제실패 : [%d] ",rPacket->strName,rPacket->wResult);
#endif
					}
					else
					{
						DeleteGuild(rPacket->wGuildSerial,rPacket->strName,"REQUESTING FOR DELETE GUILD");
					}
					break;
				}
				case dDWMSG_JOINGUILD:
				{
					DWMSG_JOINGUILD * rPacket;
					rPacket = (DWMSG_JOINGUILD *)recvpacket;
					
					cGuild * guild = g_guildManager.GetGuildPnt(rPacket->strName);

					if	(!guild || guild->wSerial==0xffff)
					{
						_logf("SERROR : FAIL JOIN GUILDER  [%d] NOT FOUND GUILD ..",rPacket->wIndex);
						break;
					}
					
					CClient * client = UserManager.GetClientPnt((WORD)rPacket->dwGameServerSerial);
					
					if	(!client || client->GetSerial()==0xffff)
					{
						_logf("SERROR [%s]FAIL JOIN GUILDER  , NOT FOUND GAMESERVER",guild->m_strName);
						break;
					}
					
					WORLDtoSERVERS_USER_JOINGUILD	jGuild;

					jGuild.base.set(sizeof(WORLDtoSERVERS_USER_JOINGUILD),dWORLDtoSERVERS_USER_JOINGUILD);
					strcpy(jGuild.strMasterName,rPacket->strMaster);
					strcpy(jGuild.strName,rPacket->strName);
					strcpy(jGuild.strTargetName,rPacket->strTargetName);

					jGuild.wRank	=	rPacket->wRank;
					jGuild.wResult	=	rPacket->wResult;
					
					Avatar_Data * ava = client->GameClientMagager.GetAvatarPnt(rPacket->strTargetName,1);
					
					if	(!ava || ava->wSerial==0xffff)
					{
						_logf("SERROR [%s]FAIL JOIN GUILDER  , [%s] is not found",guild->m_strName,rPacket->strTargetName);
					}
					
					if	(jGuild.wResult != dJOINGUILD_RESULT_SUCCESS)
					{
						client->AddSendPacket((char *)&jGuild,jGuild.base.wSize);
						break;
					}
					//	길드에 더해주고
					CGuildMan	p;
					strcpy(p.m_strName,rPacket->strTargetName);
					p.m_wRank = rPacket->wRank;
					
					int _iIndex = 0xffff;
					
					if	(!ava || ava->wSerial==0xffff)
					{
						strcpy(p.m_strId,"");

						p.m_bf1IsConnected	=	FALSE;
						p.m_wJob			=	0xffff;
						p.m_wLevel			=	0xffff;
					}
					else
					{
						strcpy(p.m_strId,ava->data.m_strId);
						p.m_bf1IsConnected	=	TRUE;
						p.m_wJob			=	ava->data.m_wJob;
						p.m_wLevel			=	ava->data.m_iLevel;
						ava->data.m_wGuildSerial = guild->m_wSerial;
						ava->data.m_wGuildRank   = p.m_wRank;
						_iIndex = guild->Add(&p);
					}
					
					if	(_iIndex==0xffff)
					{
						_log(" [%s]FAIL JOIN GUILDER [%s], Register Error in WORLD SERVER",guild->m_strName,rPacket->strTargetName);
					}
					
					if	(rPacket->wRank == dGUILD_CLASS_SUBMASTER)
					{
						guild->m_wSubMasterCount++;
						//						_logf("submastercount plus: [%s] [%d] ",guild->m_strName,guild->m_wSubMasterCount);
					}
					
					guild->updateGuildInfo();
					
					//					guild->m_wMemberCount++;
					//	해당 게임서버에 답변 
					client->AddSendPacket((char *)&jGuild,jGuild.base.wSize);
					SendPacket(client->GetSerial());
					//	모든 서버에게 길드 변경정보 전달
					WORLDtoSERVERS_GUILD_JOIN	joinG;
					joinG.base.set(sizeof(WORLDtoSERVERS_GUILD_JOIN),dWORLDtoSERVERS_GUILD_JOIN);
					joinG.member.wSerial	=	_iIndex;
					joinG.wGuildSerial		=	guild->m_wSerial;

					strcpy(joinG.strGuildName,guild->m_strName);
					joinG.member.m_dwBestowValue = 0;

					if	(!ava || ava->wSerial==0xffff)
					{
						strcpy(joinG.member.m_strId,"");
						strcpy(joinG.member.m_strName,rPacket->strTargetName);
						joinG.member.m_bf1IsConnected	=	FALSE;
						joinG.member.m_wJob				=	0;
						joinG.member.m_wLevel			=	0;
						joinG.member.m_wRank			=	rPacket->wRank;
					}
					else
					{
						strcpy(joinG.member.m_strId,ava->data.m_strId);
						strcpy(joinG.member.m_strName,ava->data.m_strName);
						joinG.member.m_bf1IsConnected	=	TRUE;
						joinG.member.m_wJob				=	ava->data.m_wJob;
						joinG.member.m_wLevel			=	ava->data.m_iLevel;
						joinG.member.m_wRank			=	ava->data.m_wGuildRank;
						//						_logf("[GUILD] Join Member [%s] [%s]",guild->m_strName,joinG.member.m_strId);
					}

					SendDataAllGameServer((char *)&joinG,joinG.base.wSize);
					SendGuildMemberUpdateInfoToBCS(guild,&joinG.member);
					
					break;
				}
				case dDWMSG_EXITGUILD:
				{
					DWMSG_EXITGUILD * rPacket;
					rPacket = (DWMSG_EXITGUILD *)recvpacket;
					
					//	길드에서 빼고
					cGuild * guild = g_guildManager.GetGuildPnt(rPacket->strName);
					
					if	(!guild || guild->wSerial==0xffff)
					{
						_log("SERROR : FAIL DELETE GUILDER [%d] NOT FOUND GUILD ..",rPacket->wIndex);
						break;
					}
					
					CClient * client = UserManager.GetClientPnt((WORD)rPacket->dwGameServerSerial);
					
					if	(!client || client->GetSerial()==0xffff)
					{
						_log("SERROR [%s]FAIL DELETE GUILDER  , NOT FOUND GAME SERVER",guild->m_strName);
						break;
					}
					
					Avatar_Data * ava = client->GameClientMagager.GetAvatarPnt(rPacket->strTargetName,1);
					
					if	(!ava || ava->wSerial==0xffff)
					{
#ifdef _FOR_KOREA
						_log("[%s]길드원 삭제 , [%s]가 없다. 상관은 없지만.. ;;",guild->m_strName,rPacket->strTargetName);
#endif
					}
					
					WORLDtoSERVERS_USER_EXITGUILD	eGuild;
					eGuild.base.set(sizeof(WORLDtoSERVERS_USER_EXITGUILD),dWORLDtoSERVERS_USER_EXITGUILD);
					strcpy(eGuild.strMasterName,rPacket->strMaster);
					eGuild.wGuildSerial	=	guild->m_wSerial;
					strcpy(eGuild.strTargetName,rPacket->strTargetName);
					eGuild.wResult		=	rPacket->wResult;
					
					if	(eGuild.wResult != dEXITGUILD_RESULT_SUCCESS)
					{
						client->AddSendPacket((char *)&eGuild,eGuild.base.wSize);
						break;
					}
					//	할일 : 삭제된 길드원의 랭크에 따른 작업
					//					guild->m_wMemberCount--;
					int _iRank = 0;

					CGuildMan * man = guild->GetGuilderPnt(eGuild.strTargetName);

					if	(man && man->wSerial!=0xffff)
					{
						_iRank = man->m_wRank;

						switch(_iRank)
						{
						case dGUILD_CLASS_PARLIAMENT:
							if	(guild->m_wCongressManCount	>	0)
								guild->m_wCongressManCount--;
							else
								_logf("ERROR m_wCongressManCount minus: [%s] [%d] ",guild->m_strName,guild->m_wCongressManCount);
							break;
						case dGUILD_CLASS_SUBMASTER:
							if	(guild->m_wSubMasterCount	>	0)
								guild->m_wSubMasterCount--;
							else
								_logf("ERROR submastercount minus: [%s] [%d] (dDWMSG_EXITGUILD)",guild->m_strName,guild->m_wSubMasterCount);
							break;
						case dGUILD_CLASS_ELDER:
							break;
						}
					}
					
					if	(ava && ava->wSerial!=0xffff)
					{
						ava->data.m_wGuildSerial = 0xffff;
						ava->data.m_wGuildRank   = 0xffff;
					}
					
					guild->Del(eGuild.strTargetName);
					guild->updateGuildInfo();
					
					//	해당 게임서버에 답변
					client->AddSendPacket((char *)&eGuild,eGuild.base.wSize);
					//	모든 서버에게 길드 변경정보 전달
					WORLDtoSERVERS_GUILD_EXIT		gExit;
					gExit.base.set(sizeof(WORLDtoSERVERS_GUILD_EXIT),dWORLDtoSERVERS_GUILD_EXIT);
					strcpy(gExit.strGuildName,guild->m_strName);
					strcpy(gExit.strMemberName,eGuild.strTargetName);
					gExit.wGuildSerial = guild->m_wSerial;
					SendDataAllGameServer((char *)&gExit,gExit.base.wSize);

					{
						SG_REMOVE_GUILD_MEMBER	packet;

						packet.base.set(sizeof(packet),dSG_REMOVE_GUILD_MEMBER);

						packet.wGuildSerial			=	guild->m_wSerial;
						packet.bMemberCount			=	(BYTE)guild->m_wMemberCount;
						packet.bSubMasterCount		=	(BYTE)guild->m_wSubMasterCount;
						packet.bCongressManCount	=	(BYTE)guild->m_wCongressManCount;

						strcpy(packet.strMemberName,eGuild.strTargetName);

						SendGuildPacketToBCS(&packet,guild->m_wSerial);
					}

					break;
				}
				case dDWMSG_CHANGERANK:
				{
					DWMSG_CHANGERANK * rPacket;
					rPacket = (DWMSG_CHANGERANK *)recvpacket;
					
					CClient * client = UserManager.GetClientPnt((WORD)rPacket->dwGameServerSerial);
					//					if(!client || client->GetSerial()==0xffff)
					//						break;
					
					cGuild * guild = g_guildManager.GetGuildPntBySerial(rPacket->wIndex);

					if	(!guild || guild->wSerial==0xffff)
						break;

					WORLDtoSERVERS_USER_CHANGERANK	sPacket;
					sPacket.base.set(sizeof(WORLDtoSERVERS_USER_CHANGERANK),dWORLDtoSERVERS_USER_CHANGERANK);
					sPacket.wResult = rPacket->wResult;
					sPacket.wGuildSerial = rPacket->wIndex;
					strcpy(sPacket.strMaster,rPacket->strMaster);
					strcpy(sPacket.strTarget,rPacket->strTarget);
					sPacket.wRank = rPacket->wRank;
					sPacket.wType = rPacket->wType;
					
					if	(rPacket->wResult == dCHANGERANK_RESULT_SUCCESS)
					{
						WORLDtoSERVERS_USER_CHANGERANK_RESULT	crResult;
						crResult.base.set(sizeof(WORLDtoSERVERS_USER_CHANGERANK_RESULT),dWORLDtoSERVERS_USER_CHANGERANK_RESULT);
						crResult.wGuildSerial = guild->m_wSerial;

						CGuildMan	*lpMember	= guild->GetGuilderPnt(rPacket->strTarget);
						
						switch(rPacket->wType)
						{
							case dGUILD_CHANGERANK_MASTER_APPONT_SPECIAL:
							{
								CGuildMan * newMaster = guild->GetGuilderPnt(rPacket->strTarget);
								
								if	(newMaster && newMaster->wSerial!=0xffff)
								{
									switch(newMaster->m_wRank)
									{
									case dGUILD_CLASS_ROOKIE	:
										break;

									case dGUILD_CLASS_MEMBER	:
										break;

									case dGUILD_CLASS_PARLIAMENT:
										if	(guild->m_wCongressManCount>0)
											guild->m_wCongressManCount--;
										break;

									case dGUILD_CLASS_SUBMASTER:
										if	(guild->m_wSubMasterCount	>	0)
											guild->m_wSubMasterCount--;
										break;

									case dGUILD_CLASS_ELDER:
										if	(guild->m_wElderCount	>	0)
											guild->m_wElderCount--;

										break;
									}
									
									newMaster->m_wRank = dGUILD_CLASS_MASTER;
									strcpy(guild->m_strMasterName,newMaster->m_strName);
									guild->m_wGuildMasterLevel = newMaster->m_wLevel;
									//	전 서버에게 변경된 결과를 알려준다.
									strcpy(crResult.strTarget,newMaster->m_strName);
									crResult.wRank = newMaster->m_wRank;

									SendChangeGVGServerGuildMasterPacketToLoginServer(guild->m_strName,newMaster->m_strId,UserManager.GetClientPnt(dLOGIN_SERVER_ID));

									//	master의 변경시간
									cSRVUTIL::GetUTime((cpUTime * )&guild->m_timeBeginGuildMaster);
									cSRVUTIL::GetUTime((cpUTime * )&guild->m_timeLastConnectTime);
									cSRVUTIL::GetUTime((cpUTime * )&guild->m_timeLastConnectedMasterTime);
								}
								break;
							}
							case dGUILD_APPONT:
							{	
								if	(rPacket->wRank == dGUILD_CLASS_MASTER)
								{
									//	마스터를 바꾸고 기존의 마스터는 원로길드로 수정
									CGuildMan * oldMaster = guild->GetGuilderPnt(rPacket->strMaster);
									cSRVUTIL::GetUTime((cpUTime *)&guild->m_timeLastConnectedMasterTime);
									
									if	(oldMaster && oldMaster->wSerial<0xcccc)
									{
										oldMaster->m_wRank = rPacket->wRank2;
										
										if	(oldMaster->m_wRank == dGUILD_CLASS_ELDER)
											guild->m_wElderCount++;
									}
									else
									{
										_log("ERROR dDWMSG_CHANGERANK 1 : [%s,%s,%s]",guild->m_strName,rPacket->strMaster,rPacket->strTarget);
									}
									
									CGuildMan * newMaster = guild->GetGuilderPnt(rPacket->strTarget);
									
									if	(newMaster && newMaster->wSerial!=0xffff)
									{
										switch(newMaster->m_wRank)
										{
										case dGUILD_CLASS_ROOKIE:
											break;
											
										case dGUILD_CLASS_MEMBER:
											break;
											
										case dGUILD_CLASS_PARLIAMENT:
											if	(guild->m_wCongressManCount>0)
												guild->m_wCongressManCount--;
											break;
											
										case dGUILD_CLASS_SUBMASTER:
											if	(guild->m_wSubMasterCount>0)
												guild->m_wSubMasterCount--;
											break;
											
										case dGUILD_CLASS_ELDER:
											if	(guild->m_wElderCount>0)
												guild->m_wElderCount--;
											break;
										}
										newMaster->m_wRank = dGUILD_CLASS_MASTER;
										strcpy(guild->m_strMasterName,newMaster->m_strName);
										guild->m_wGuildMasterLevel = newMaster->m_wLevel;

										SendChangeGVGServerGuildMasterPacketToLoginServer(guild->m_strName,newMaster->m_strId,UserManager.GetClientPnt(dLOGIN_SERVER_ID));
									}
									else
									{
										_log("ERROR dDWMSG_CHANGERANK 2 : [%s,%s,%s]",guild->m_strName,rPacket->strMaster,rPacket->strTarget);
									}
									
									strcpy(crResult.strTarget,rPacket->strMaster);
									crResult.wRank = rPacket->wRank2;
									//	기존의 마스터는 뭘로 바뀌었는지 전 서버에 날리고..
									SendDataAllGameServer((char *)&crResult,crResult.base.wSize);
									
									//	새로운 마스터의 변경은 저 아래에서 날린다.
									strcpy(crResult.strTarget,rPacket->strTarget);
									crResult.wRank = rPacket->wRank;
									//	master의 변경시간
									cSRVUTIL::GetUTime((cpUTime * )&guild->m_timeBeginGuildMaster);
									cSRVUTIL::GetUTime((cpUTime * )&guild->m_timeLastConnectTime);
									cSRVUTIL::GetUTime((cpUTime * )&guild->m_timeLastConnectedMasterTime);
								}
								else
								{	//	마스터가 아닌 다른 계급에서 임명이 된경우
									CGuildMan * member = guild->GetGuilderPnt(rPacket->strTarget);
									
									if	(member && member->wSerial!=0xffff)
									{
										switch(member->m_wRank)
										{
										case dGUILD_CLASS_ROOKIE:
											break;
										case dGUILD_CLASS_MEMBER:
											break;
										case dGUILD_CLASS_PARLIAMENT:
											guild->m_wCongressManCount++;
											break;
										case dGUILD_CLASS_SUBMASTER:
											guild->m_wSubMasterCount++;
											break;
										case dGUILD_CLASS_ELDER:
											guild->m_wElderCount++;
											break;
										}
										member->m_wRank = rPacket->wRank;
									}
									strcpy(crResult.strTarget,rPacket->strTarget);
									crResult.wRank = rPacket->wRank;
								}
								break;
							}
							case dGUILD_DISMISSAL:
							{
								CGuildMan * member = guild->GetGuilderPnt(rPacket->strTarget);
								if(member && member->wSerial!=0xffff)
								{
									switch(member->m_wRank)
									{
									case dGUILD_CLASS_ROOKIE:
										break;
									case dGUILD_CLASS_MEMBER:
										break;
									case dGUILD_CLASS_PARLIAMENT:
										if	(guild->m_wCongressManCount>0)
											guild->m_wCongressManCount--;
										break;
										
									case dGUILD_CLASS_SUBMASTER:
										if	(guild->m_wSubMasterCount>0)	
											guild->m_wSubMasterCount--;	
										break;
									case dGUILD_CLASS_ELDER:
										if	(guild->m_wElderCount>0)		
											guild->m_wElderCount--;
										break;
									}
									member->m_wRank = dGUILD_CLASS_MEMBER;
									strcpy(crResult.strTarget,member->m_strName);
									crResult.wRank = member->m_wRank;
								}
								break;
							}
						}
						
						if	(guild)
							guild->updateGuildInfo();
						
						if	(client)
						{
							client->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
							SendPacket(client->GetSerial());
						}
						//	변경된 내용은 전 서버에 전달한다.
						SendGuildMemberUpdateInfoToBCS(guild,lpMember);
						SendDataAllGameServer((char *)&crResult,crResult.base.wSize);
					}
					else
					{
						if	(client)
						{
							client->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
							SendPacket(client->GetSerial());
						}
					}
					break;
				}
				//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
				//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
				//┃ Event 관련 
				case dDWMSG_GR_INFO_RESULT:
				{
					DWMSG_GR_INFO_RESULT * rPacket  = (DWMSG_GR_INFO_RESULT * )recvpacket;
					
					g_goldRush.updateInfo(&rPacket->begin,&rPacket->end,rPacket->wBoostExp,rPacket->wDropChance,rPacket->strBeginComment,rPacket->strEndComment);
					sendGRInfo(NULL);
					break;
				}

				case dDWMSG_ITEMPACK_RESULT:
				{
					DWMSG_ITEMPACK_RESULT * rPacket  = (DWMSG_ITEMPACK_RESULT * )recvpacket;
					
					operateUpdateItemPackInfo(NULL,(DWMSG_ITEMPACK_RESULT*)rPacket);
					
					if(rPacket->wSerial == dMAX_ITEM_PACK_COUNT-1 && isReady == eIsReady_Nothing)
					{
						isReady = eIsReady_EventInfo;
						_log(" isReady = %d ",isReady);
						_log(" Request Guild Infomation of Sql DB ... ");
						
						WDMSG_GETGUILD sPacket;
						sPacket.base.set(sizeof(WDMSG_GETGUILD),dWDMSG_GETGUILD);
						GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
					}
					break;
				}
				//┃
				//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
				case dCM_DUMMY:
				default:
				{
					break;
				}
			}
		}
	}

	hDBThread	=	NULL;

	g_lifeCodeMgr.endCheck(eLIFE_CODE_DBC);

	_log("END DBCtlThread ... ");
	//	isMainLoop = FALSE;
	return ;
}

void USERDBCtlThread(LPVOID lpParameter)
{
	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);
	WORD	type = 0,recvsize = 0;
	_log("Start USERDBCtlThread ... ");
	
	{
		WU_ASK_CARROTSHOPINFO sPacket;
		sPacket.base.set(sizeof(WU_ASK_CARROTSHOPINFO),dWU_ASK_CARROTSHOPINFO,0xffff);
		sPacket.wIsUpdateNow	=	FALSE;
		GetIDDBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);


		SendAskDuelRankingInfoMsg();	//	결투 랭킹 정보 요청
		SendAskDuelTeamInfoMsg();	//	결투 팀 정보 요청
	}

#ifdef	_FOR_KOREA	
	{
		WU_ASK_DB_CONTROLLED_EVENT_INFO	packet;

		packet.base.set(sizeof(WU_ASK_DB_CONTROLLED_EVENT_INFO),dWU_ASK_DB_CONTROLLED_EVENT_INFO,0xffff);
		GetIDDBSocket()->SendPacket((char *)&packet,packet.base.wSize);
	}
#endif
	
	g_lifeCodeMgr.beginCheck(eLIFE_CODE_USER_DBC);

	while(GetIDDBSocket()->isLoopRecv)
	{
		Sleep(1);

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_USER_DBC);

		while(GetIDDBSocket()->GetRecvPacket(recvpacket))
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
			switch(type)
			{
				case	dUW_RESET_WEEKLY_DUEL_INFO	:
				{
					g_duelRankerManager.setDailyRankerCount(0);
					SendSimpleWork(NULL,eSWWG_RESET_WEEKLY_DUEL_RECORD);
					break;
				}

				case	dUW_DUEL_TEAM_INFO		:
				{
					UW_DUEL_TEAM_INFO * rPacket  = (UW_DUEL_TEAM_INFO * )recvpacket;

					g_duelTeamManager.init(rPacket->wTotalTeamCount);

					for (int i=0;i<rPacket->wTeamCount;i++)
						g_duelTeamManager.addTeam(&rPacket->aTeams[i]);

					break;
				}

				case	dUW_DUEL_RANKING_INFO	:
				{
					UW_DUEL_RANKING_INFO * rPacket  = (UW_DUEL_RANKING_INFO * )recvpacket;

					if	(rPacket->wIsDailyRanking)
					{
						g_duelRankerManager.setDailyRankerCount(rPacket->wWeeklyRankerCount);
						g_duelRankerManager.setTrialGameRankerCount(rPacket->wTrialGameRankerCount);

						int	i=0,iSlot=0;

						for (i=0;i<rPacket->wWeeklyRankerCount;i++,iSlot++)
							g_duelRankerManager.updateDailyRanker(&rPacket->aList[iSlot]);
						for (i=0;i<rPacket->wTrialGameRankerCount;i++,iSlot++)
							g_duelRankerManager.updateTrialGameRanker(&rPacket->aList[iSlot]);
					}
					else
					{
						g_duelRankerManager.setWeeklyRankerCount(rPacket->wWeeklyRankerCount);

						int	i=0;

						for (;i<rPacket->wWeeklyRankerCount;i++)
							g_duelRankerManager.updateWeeklyRanker(&rPacket->aList[i]);
					}

					sendGSPDuelRankerInfo(NULL,rPacket->wIsDailyRanking);

					break;
				}

				case	dUW_DUEL_RECORD	:
				{
					UW_DUEL_RECORD	*rPacket	=	(UW_DUEL_RECORD	*)recvpacket;
					CClient			*lpGS		=	UserManager.GetClientPnt(rPacket->iGSSerialInWorld);

					if	(!lpGS)
						break;

					WORLDtoSERVERS_DUEL_RECORD	packet;

					packet.base.set(sizeof(packet),dWORLDtoSERVERS_DUEL_RECORD);
					strcpy(packet.strId,rPacket->strId);
					memcpy(&packet.record,&rPacket->record,sizeof(cDuelRecord));

					mSEND_GSP(lpGS,packet);

					break;
				}
			case dUW_DB_CONTROLLED_EVENT_INFO	:
				{
					UW_DB_CONTROLLED_EVENT_INFO	*lpPacket	=	(UW_DB_CONTROLLED_EVENT_INFO	*)recvpacket;

					g_eventManager.updateDBControlledEventValue(lpPacket->aiValues);
					
					break;
				}
			case dUW_RESULT_OF_INCREASE_DB_EVENT_VALUE	:
				{
					UW_RESULT_OF_INCREASE_DB_EVENT_VALUE	*lpPacket	=	(UW_RESULT_OF_INCREASE_DB_EVENT_VALUE	*)recvpacket;

					WORLDtoSERVERS_RESULT_INCREASE_DB_EVENT_VALUE	packet;

					packet.base.set(sizeof(packet),dWORLDtoSERVERS_RESULT_INCREASE_DB_EVENT_VALUE);
					packet.wResult		=	lpPacket->wResult;
					packet.wEventIndex	=	lpPacket->wEventIndex;
					packet.iValue		=	lpPacket->iValue;

					if	(lpPacket->wResult	==	eROIDEV_SUCCESS)
						g_eventManager.updateDBControlledEventValue(lpPacket->wEventIndex,lpPacket->iValue);
					if	(lpPacket->wResult	==	eROIDEV_RESET)
						g_eventManager.resetDBControlledEventValue(lpPacket->wEventIndex);

					strcpy(packet.strUserID		,	lpPacket->strUserID);
					strcpy(packet.strUserName	,	lpPacket->strUserName);

					CClient *_lpUser	=	NULL;

					mSEND_GSP(_lpUser,packet);
					break;
				}
				break;

			case dUW_RENAME_AVATAR:
				{	//	길드에 가입된 유저라면.. 찾아라.
					UW_RENAME_AVATAR * rPacket;
					rPacket = (UW_RENAME_AVATAR *)recvpacket;

					CGuildMan * _man = g_guildManager.GetGuildManPnt(rPacket->strOldName);
					if(_man)
					{
						strcpy(_man->m_strName,rPacket->strNewName);
						_log(" RENAME_AVATAR : [%s] -> [%s]",rPacket->strOldName,rPacket->strNewName);

					}
				}
				break;
			case dUW_WORLD_CLOSE:
				{
					UW_WORLD_CLOSE * rPacket;
					rPacket = (UW_WORLD_CLOSE *)recvpacket;

					//점검시간 수정하기
					if(rPacket->relTime.year == 99)
					{
						wWeekFixCheck = wWeekFixEndCheck = wWeekFixEndCheck = wHourFixEndCheck = wMinFixCheck = wMinFixEndCheck = -1;
					}else
					{
						wWeekFixCheck = cSRVUTIL::GetWeekDayOfaday(rPacket->relTime.year+2000,rPacket->relTime.month,rPacket->relTime.day);
						wWeekFixEndCheck = cSRVUTIL::GetWeekDayOfaday(rPacket->finTime.year+2000,rPacket->finTime.month,rPacket->finTime.day);;
						wHourFixCheck = rPacket->relTime.hour;
						wHourFixEndCheck = rPacket->finTime.hour;
						wMinFixCheck = rPacket->relTime.minute;
						wMinFixEndCheck = rPacket->finTime.minute;
					}
					strcpy(strMsgFixCheck,rPacket->msg);
					{	//	저장하기
						char txt[32];
						sprintf(txt,"%d",wWeekFixCheck);	configScp.SetData(txt,"WEEKFIXCHECK");
						sprintf(txt,"%d",wWeekFixEndCheck);	configScp.SetData(txt,"WEEKFIXENDCHECK");
						sprintf(txt,"%d",wHourFixCheck);	configScp.SetData(txt,"HOURFIXCHECK");
						sprintf(txt,"%d",wHourFixEndCheck);	configScp.SetData(txt,"HOURFIXENDCHECK");
						sprintf(txt,"%d",wMinFixCheck);		configScp.SetData(txt,"MINFIXCHECK");
						sprintf(txt,"%d",wMinFixEndCheck);	configScp.SetData(txt,"MINFIXENDCHECK");
						configScp.SetData(strMsgFixCheck,"MSGFIXCHECK");
						configScp.Save();
					}					
					_log("New Maintenance Time : Weekday[%d] , Hour [%d], Day [%d]\n->Maintenance End Time : Weekday[%d] , Hour [%d], Day [%d] ",wWeekFixCheck,wHourFixCheck,wMinFixCheck,wWeekFixEndCheck,wHourFixEndCheck,wMinFixEndCheck);
				}
				break;
			case dUW_BANISH:
				{
					UW_BANISH * rPacket;
					rPacket = (UW_BANISH *)recvpacket;
					
					SERVERStoWORLD_BANISH_USER		sPacket;
					memset(&sPacket,0,sizeof(SERVERStoWORLD_BANISH_USER));
					sPacket.base.set(sizeof(SERVERStoWORLD_BANISH_USER),dSERVERStoWORLD_BANISH_USER);
					strcpy(sPacket.strName,rPacket->who);
					strcpy(sPacket.strReason,rPacket->msg);
					sPacket.wIsForced = 1;
#ifdef _FOR_KOREA
					_log(" Command BANISH : [%s] [%s]",sPacket.strName,sPacket.strReason);
#endif
					operateBanishUser(NULL,(SERVERStoWORLD_BANISH_USER*)&sPacket);
				}
				break;
			case dUW_NOTICE:
				{
					UW_NOTICE * rPacket;
					rPacket = (UW_NOTICE *)recvpacket;
					
					WORLDtoSERVERS_ALL_MSG	sPacket;
					sPacket.base.set(sizeof(WORLDtoSERVERS_ALL_MSG),dWORLDtoSERVERS_ALL_MSG);
					if(rPacket->isUser)
						sprintf(sPacket.strMessage,"%s -[%s]님- ",rPacket->msg,rPacket->who);
					else
						sprintf(sPacket.strMessage,"%s",rPacket->msg);
					sPacket.strMessage[dCHAT_LENGTH-1] = 0;
#ifdef _FOR_KOREA
					_log("NOTICE [%s]",sPacket.strMessage);
#endif
					SendDataAllGameServer((char *)&sPacket,sPacket.base.wSize);
				}
				break;
			case dUL_RESULT_WLOGIN:
				{
					UL_RESULT_WLOGIN * rPacket;
					rPacket = (UL_RESULT_WLOGIN *)recvpacket;
					
					if(rPacket->dwResult==0 || rPacket->dwResult>=10)
					{
						//	유저가 로그인 인증을 성공했다면, BC에 등록되어 있을지도 모를 유저의 정보를 날리자.
						if	(UserManager.m_wBCSIndex<0xffff)
						{
							WB_REMOVE_USERINFO	sPacket;
							sPacket.base.set(sizeof(WB_REMOVE_USERINFO),dWB_REMOVE_USERINFO,0);
							strcpy(sPacket.strId,rPacket->strId);

							CClient * _pBCServer = UserManager.GetClientPnt(UserManager.m_wBCSIndex);
							if	(_pBCServer)
								_pBCServer->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
						}
						GDMSG_LOGINAVATAR	sUserLogin;
						memset(&sUserLogin,0,sizeof(GDMSG_LOGINAVATAR));
						sUserLogin.base.set(sizeof(GDMSG_LOGINAVATAR),dGDMSG_LOGINAVATAR);
						sUserLogin.u_serial = rPacket->serial;
						sUserLogin.worldSerial = rPacket->base.wSerial;
						if(rPacket->dwResult>=10)	sUserLogin.badtime = (WORD)rPacket->dwResult;
						else						sUserLogin.badtime = 0;
						sUserLogin.isNetCafeUser = rPacket->isNetCafeUser;
						sUserLogin.isOper = rPacket->isOper;
						strcpy(sUserLogin.strId,rPacket->strId);
						strcpy(sUserLogin.strName,rPacket->strName);
						strcpy(sUserLogin.strIP,rPacket->strIP);
						GetADBSocket()->SendPacket((char *)&sUserLogin,sUserLogin.base.wSize);

						//	AVATAR DBC에 로그인 메세지 보내자.
						LDMSG_LOGIN	sPacketLogin;
						memset(&sPacketLogin,0,sizeof(LDMSG_LOGIN));
						sPacketLogin.base.set(sizeof(LDMSG_LOGIN),dLDMSG_LOGIN);
						strcpy(sPacketLogin.strId, rPacket->strId);
						strcpy(sPacketLogin.strName, rPacket->strName);
						sPacketLogin.externalType = 0;	//	외부 연동 관련된 값을 넣자. 
						GetADBSocket()->SendPacket((char *)&sPacketLogin,sPacketLogin.base.wSize);
					}
					else
					{	//	게임 서버로 로그인 실패 메세지 확인해서 보낼것!!
						WORLDtoSERVERS_USER_LOGIN		sUserLogin;
						memset(&sUserLogin,0,sizeof(WORLDtoSERVERS_USER_LOGIN));
						sUserLogin.base.set(sizeof(WORLDtoSERVERS_USER_LOGIN),dWORLDtoSERVERS_USER_LOGIN);
						sUserLogin.iIndex = -1;
						sUserLogin.serial = rPacket->serial;
						
						CClient	* user = UserManager.GetClientPnt(rPacket->base.wSerial);
						
						if(user)
						{
							user->AddSendPacket((char *)&sUserLogin,sUserLogin.base.wSize);
							SendPacket(user->GetSerial());
						}
						printf("login Failed [%s] by dUL_RESULT_WLOGIN\n",rPacket->strId);
					}
					break;
				}
			case dUS_DIS_USER:
				{
					US_DIS_USER * rPacket;
					rPacket = (US_DIS_USER *)recvpacket;
#ifdef _FOR_KOREA
					//					_log("DB가 자르라고 시켰다. [%s]를 찾아보자. \n",rPacket->strId);
#endif
					//	이름으로 어느 게임서버의 유저인지 찾아내고 
					WORD ss = UserManager.GetClientOfGClientSerial(rPacket->strId);

					if	(ss == 0xffff)
					{
						WORLDtoSERVERS_DIS		out;

						memset(&out,0,sizeof(WORLDtoSERVERS_DIS));
						out.base.set(sizeof(WORLDtoSERVERS_DIS),dWORLDtoSERVERS_DIS);
						strcpy(out.strId,rPacket->strId);
						strcpy(out.strCause,"Invalid user");

						++l_iCountForDisUser;

						for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)
						{
							CClient	* usr = UserManager.GetClientPnt(i,FALSE);

							if	(!usr || usr->GetSerial()==0xffff)
								continue;
							if	(stricmp(usr->GetID(),dLOGIN_SERVER_ID) ==0)
								continue;
							
							usr->AddSendPacket((char *)&out,out.base.wSize);
							SendPacket(usr->GetSerial());
						}
					}
					else
					{
						//	게임서버에게 나가라고 시켜라
						WORLDtoSERVERS_DIS		out;
						memset(&out,0,sizeof(WORLDtoSERVERS_DIS));
						out.base.set(sizeof(WORLDtoSERVERS_DIS),dWORLDtoSERVERS_DIS);
						strcpy(out.strId,rPacket->strId);
						strcpy(out.strCause,"Same ID users try to login\n");
						++l_iCountForDisUser;
						
						CClient * user = UserManager.GetClientOfGClientPnt(rPacket->strId);

						if	(user)
						{
							user->AddSendPacket((char *)&out,out.base.wSize);
							SendPacket(user->GetSerial());
							DeleteUser(user, rPacket->strId,"EMPTY",0);
						}
					}
					WDMSG_KILL_AVATAR		killAvatar;
					killAvatar.base.set(sizeof(WDMSG_KILL_AVATAR),dWDMSG_KILL_AVATAR);
					strcpy(killAvatar.strId,rPacket->strId);
					GetADBSocket()->SendPacket((char *)&killAvatar,killAvatar.base.wSize);
					break;
				}
			case dUW_ASK_CARROTSHOPINFO_RESULT:
				{
					operateUpdateCarrotShopInfo(NULL,(UW_ASK_CARROTSHOPINFO_RESULT *)recvpacket);
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

	hUSERDBThread	=	NULL;
	
	g_lifeCodeMgr.endCheck(eLIFE_CODE_USER_DBC);

	_log("END USERDBCtlThread ... ");
	//	isMainLoop = FALSE;
	return ;
}

//////////////////////////////////////////////
//	method for packet method
//////////////////////////////////////////////
void	DisUser(int serial,char * _why)
{
	_log("===========>>>>>>>>>>		Dis User : %d [%s]",serial,_why);

	if	(serial<0)
		return;

	EnterCriticalSection(&csRemoveUser);

	CClient * user  = UserManager.GetClientPnt(serial);

	if	(!user)
	{
		LeaveCriticalSection(&csRemoveUser);
		return ;
	}
	_log("GAME SERVER QUIT [%s]!!",user->GetID());

	if	(user->GetIsUsed())
	{	//	게임 서버가 죽을 경우.. 그 게임서버의 유저들을 몽땅 디스 처리 하고 죽인다.
		if(STRICMP(user->GetID(),"CS_SERVER")==0)
		{
			UserManager.m_wBCSIndex = 0xffff;
		}
		else
		if(STRICMP(user->GetID(),dADMIN_TOOL_ID)==0)
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
			for(int j=0;j<dMAX_ADMIN_COUNT;j++)
			{
				if(_Admin[j].isUsed)
					break;
			}
			if(j==dMAX_ADMIN_COUNT)	isAdmin = FALSE;
		}else
		{
			int i=0;
#ifdef _FOR_KOREA
			_log("[%s]서버의 %d명을 정리한다.",user->GetID(),user->GameClientMagager.Size());
#endif
			for(int gccount=0;gccount<dGAME_MAX_USER_COUNT;gccount++ )
			{
				Avatar_Data	* ava = user->GameClientMagager.GetAvatarPnt(gccount,FALSE);
				
				if	(ava==NULL || ava->wSerial==0xffff)
					continue;
				
				if(STRICMP(ava->data.m_strId,"")==0)
				{
#ifdef _FOR_KOREA
					_log("ERROR 게임 서버가 종료됨.. 유저 삭제 ID=NULL : %d",gccount);
#endif
					continue;
				}
				DeleteUser(user,ava->data.m_strId,"EMPTY");
			}

			g_dwUserCount = UserManager.GetAllUserCount();

			if	(UserManager.m_wGSCount>0)
				--UserManager.m_wGSCount;
		}

		user->SetIsUsed(FALSE);
	}

	_log("(%s)%d is exit\n",user->GetID(),serial);
	UserManager.Del(serial,user->GetID());
#ifdef _FOR_KOREA
	_logc("남은 게임 서버 갯수 : [%d] \n",UserManager.Size());
#endif
	LeaveCriticalSection(&csRemoveUser);
}

void
SaveData(CClient * user,SERVERStoWORLD_SAVEAVA	* sAva)
{
	//	해당 게임서버에 속해 있는 유저 포인터를 찾는다.
	Avatar_Data * ava = user->GameClientMagager.GetAvatarPnt(sAva->aInfo.m_strId);
	//	만약 유저 포인터가 유효하다면 , 
	if	(!ava || ava->wSerial==0xffff)
		return;
	//		_logf("AVA SAVE : [%s][%s]lev[%d]exp[%d]savecount[%d]",sAva->aInfo.m_strId,sAva->aInfo.m_strName,sAva->aInfo.m_iLevel,sAva->aInfo.m_iExperience,sAva->aInfo.m_dwSaveCount);
	//	데이타를 복사해주고.. 
	memcpy(&ava->data,&sAva->aInfo,sizeof(CPlayerSaveDataForServerDefine));
	//	세이브된 시간을 기록한다.
	ava->SetTime();
	//	해당 유저가 맵을 이동중인 상태가 아니면 
	if	(ava->bMovingMap	==	FALSE)
		return;
	//	새로 들어간 서버를 찾아서 

	WORD	_wResult;
	CClient * tserver = UserManager.GetClientOfField(sAva->aInfo.m_wCurrentField,1,_wResult);

	if	(!tserver)
	{
		_logc("ERROR SAVE_DATA : correct Field index.. ");
	}
	else
	{
		//	해당 유저를 찾고, 있으면 복사해준다.
		Avatar_Data * tava = tserver->GameClientMagager.GetAvatarPnt(sAva->aInfo.m_strId);

		if	(tava && tava->wSerial!=0xffff)
			memcpy(&tava->data,&sAva->aInfo,sizeof(CPlayerSaveDataForServerDefine));
	}
}

void	SaveWorldValue()
{
	cFILE	file;
	
	if	(file.Open(_ms("%s/data/WorldValueList.dat",g_strLogFolder),"wb"))
	{
		file.Write(&g_aiWorldValue,sizeof(int)*dMAX_WORLD_VALUE_MAX);
		file.Close();
	}
}


void	opeateWorldValue(int _iSortSet , WORD _wSerial , int _iValue)
{
	if(_wSerial > dMAX_WORLD_VALUE_MAX)
		return;

	switch(_iSortSet)
	{
	case 0:			//더한다.
		g_aiWorldValue[_wSerial] += _iValue;
		break;
	case 1:			//뺀다
		g_aiWorldValue[_wSerial] -= _iValue;
		break;
	case 2:			//로 한다.(대입)
		g_aiWorldValue[_wSerial] = _iValue;
		break;
	}

	SaveWorldValue();

	WORLDtoSERVERS_WORLD_VALUE_INFO	packet;
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_UPDATE_WORLD_VALUE_INFO);
	packet.wIndex = _wSerial;
	packet.iValue = g_aiWorldValue[_wSerial];

	SendDataAllGameServer((char *)&packet,packet.base.wSize);

}



void	LoadData(CClient *user,DGMSG_LOGINAVATAR *sLoginAva)
{
	WORLDtoSERVERS_USER_LOGIN	rPacketAva;
	
	memset(&rPacketAva,0,sizeof(WORLDtoSERVERS_USER_LOGIN));
	rPacketAva.base.set(sizeof(WORLDtoSERVERS_USER_LOGIN),dWORLDtoSERVERS_USER_LOGIN);
	rPacketAva.serial			=	sLoginAva->u_serial;
	
	CPartyInfo	*lpParty		=	g_partyManager.getByPlayer(sLoginAva->pData.m_strName);
	
	if (lpParty)
		rPacketAva.wPartyIndex	=	lpParty->m_wSerial;
	else
		rPacketAva.wPartyIndex	=	0xffff;
	
	WORD	_wResult	=	0;
	
	if(sLoginAva->wResult && UserManager.GetClientOfField(sLoginAva->pData.m_wCurrentField,1,_wResult)!=NULL)
	{
		WORD guserial = user->GameClientMagager.Add(sLoginAva->pData.m_strId);
		
		rPacketAva.iIndex = guserial;
		
		if (guserial!=0xffff && guserial!=0xffff && guserial!=0xfffe)
		{
			if(strstr(sLoginAva->pData.m_strId,"@y"))
			{
				EnterCriticalSection(&csYahooCount);
				g_dwYahooUserCount++;
				LeaveCriticalSection(&csYahooCount);
			}else
			if(strstr(sLoginAva->pData.m_strId,"@s"))
			{
				EnterCriticalSection(&csParanCount);
				g_dwParanUserCount++;
				LeaveCriticalSection(&csParanCount);
			}
			user->GameClientMagager.writeData(guserial,&sLoginAva->pData,(WORD)sLoginAva->u_serial,sLoginAva->Oper,sLoginAva->isNetCafeUser);
			rPacketAva.wFieldidx	=	sLoginAva->pData.m_wCurrentField;
			rPacketAva.bf10IFSerial	=	sLoginAva->pData.m_bf8InstanceField;
			rPacketAva.bf4IFFloor	=	sLoginAva->pData.m_bf4InstanceFieldFloor;
			rPacketAva.badtime		=	sLoginAva->badtime;
			rPacketAva.oper			=	sLoginAva->Oper;
			rPacketAva.isNetCafeUser=	sLoginAva->isNetCafeUser;
			rPacketAva.isOperInAccount = sLoginAva->isOper;
			
			if(sLoginAva->Oper>6 || sLoginAva->Oper<0)
			{
				_log("ERROR OPER %d",sLoginAva->Oper);
			}
			
			g_dwUserCount = UserManager.GetAllUserCount();
			if(g_dwMaxTimeUserCount[cSRVUTIL::GetHour()]==0xffff)	g_dwMaxTimeUserCount[cSRVUTIL::GetHour()] = 0;
			if(g_dwMaxTimeYahooUserCount[cSRVUTIL::GetHour()]==0xffff)	g_dwMaxTimeYahooUserCount[cSRVUTIL::GetHour()] = 0;
			if(g_dwMaxTimeParanUserCount[cSRVUTIL::GetHour()]==0xffff)	g_dwMaxTimeParanUserCount[cSRVUTIL::GetHour()] = 0;
			
			if(g_dwUserCount>g_dwMaxTimeUserCount[cSRVUTIL::GetHour()])	g_dwMaxTimeUserCount[cSRVUTIL::GetHour()] = g_dwUserCount;
			if(g_dwYahooUserCount>g_dwMaxTimeYahooUserCount[cSRVUTIL::GetHour()])	g_dwMaxTimeYahooUserCount[cSRVUTIL::GetHour()] = g_dwYahooUserCount;
			if(g_dwParanUserCount>g_dwMaxTimeParanUserCount[cSRVUTIL::GetHour()])	g_dwMaxTimeParanUserCount[cSRVUTIL::GetHour()] = g_dwParanUserCount;
			
			if(g_dwMaxDayUserCount<g_dwUserCount)	g_dwMaxDayUserCount = g_dwUserCount;
			if(g_dwMaxDayYahooUserCount<g_dwYahooUserCount)	g_dwMaxDayYahooUserCount = g_dwYahooUserCount;
			if(g_dwMaxDayParanUserCount<g_dwParanUserCount)	g_dwMaxDayParanUserCount = g_dwParanUserCount;
			
			if(g_dwMaxUserCount<g_dwUserCount)	g_dwMaxUserCount = g_dwUserCount;
			if(g_dwMaxYahooUserCount<g_dwYahooUserCount)	g_dwMaxYahooUserCount = g_dwYahooUserCount;
			if(g_dwMaxParanUserCount<g_dwParanUserCount)	g_dwMaxParanUserCount = g_dwParanUserCount;
		}
		else
		{
			if(guserial==0xfffe)
			{
				DeleteUser(user,sLoginAva->pData.m_strId,"EMPTY",0);
#ifdef _FOR_KOREA
				_log("SERROR : 로그인 하려는데 기존 데이타가 있어서 자른다. [%s]",sLoginAva->pData.m_strId);
#endif
			}
			rPacketAva.iIndex = -1;
			
			printf("load data Failed [%s] add fail\n",sLoginAva->pData.m_strId);
#ifdef _FOR_KOREA
			_log("ERROR :  새로운 캐릭터를 추가하는데 실패하다.[%s]",sLoginAva->pData.m_strId);
#endif
		}
	}
	else
	{
		rPacketAva.iIndex = -1;
		printf("load data Failed by Incorrect Data. result [%d]\n",sLoginAva->wResult);
#ifdef _FOR_KOREA
		_log("ERROR :  새로운 캐릭터를 추가하는데 실패하다.wResult = %d, field = %d",sLoginAva->wResult,sLoginAva->pData.m_wCurrentField);
#endif
	}
	
	user->AddSendPacket((char *)&rPacketAva,rPacketAva.base.wSize);
	SendPacket(user->GetSerial());
}

BOOL	DeleteUser(CClient	* user, char	* _strId, char	* _strIp, BOOL	_isSkipMoveMap)
{
	//	전 게임 서버의 모든 해당 아이디 캐릭터를 찾아서 지운다.
	Avatar_Data * ava = NULL;
	CClient * checkUser = NULL;

	while(TRUE)
	{
		if(_isSkipMoveMap)
			checkUser = UserManager.GetClientOfGClientPntForDelete(_strId);
		else
			checkUser = UserManager.GetClientOfGClientPnt(_strId);
		
		if	(!checkUser)
		{
			break;
		}
		
		ava = checkUser->GameClientMagager.GetAvatarPnt(_strId);

		if	(!ava)
		{
#ifdef _FOR_KOREA
			_log("SSSSSSSERROR : 장난치나.. -_-;");
#endif
			break;
		}

		if	(_isSkipMoveMap && ava->bMovingMap)
		{
			//			printf(" 맵 이동할 곳의 데이타 이므로 삭제하지 않는다.");
			break;
		}

		int ptime = ava->playtime;	//	플레이 타임

		if	(ava->data.m_wGuildSerial!=0xffff)
		{
			cGuild * guild = g_guildManager.GetGuildPntBySerial(ava->data.m_wGuildSerial);

			if	(guild && guild->wSerial!=0xffff)
			{
				CGuildMan * guilder = guild->GetGuilderPnt(ava->data.m_strName);

				if	(guilder && guilder->wSerial!=0xffff)
				{
					guilder->m_bf1IsConnected	=	FALSE;
					guilder->m_wJob				=	ava->data.m_wJob;
					guilder->m_wLevel			=	ava->data.m_iLevel;
				}
			}
		}

		LDMSG_LOGOUT	sPacket;
		sPacket.base.set(sizeof(LDMSG_LOGOUT),dLDMSG_LOGOUT);
		strcpy(sPacket.strId,_strId);
		GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);

		LU_LOGOUT		sLogout;
		sLogout.base.set(sizeof(LU_LOGOUT),dLU_LOGOUT,checkUser->GetSerial());
		strcpy(sLogout.strId,_strId);
		strcpy(sLogout.strIp,_strIp);
		GetIDDBSocket()->SendPacket((char *)&sLogout,sLogout.base.wSize);

		checkUser->GameClientMagager.Del(_strId,"dSERVERStoWORLD_USER_LOGOUT  USER IS QUIT !!");
		g_dwUserCount = UserManager.GetAllUserCount();
	}
	
	return TRUE;
}

void	SendGameServerInitDataThread(LPVOID lpParameter)
{
	WORD	gsSerial = (WORD)lpParameter;
	
	CClient * client = UserManager.GetClientPnt(gsSerial);

	if	(!client || client->GetSerial()==0xffff)
	{
#ifdef _FOR_KOREA
		printf(" 없는 클라이언트가 길드 리스트를 요청한다.\n");
#endif
		return;
	}
	
	g_lifeCodeMgr.beginCheck(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);

	int listCount = g_guildManager.Size();
	
	if(listCount == 0)
	{
		printf("아직 길드 리스트가 없어요.\n");
	}
	else
	{
		WORLDtoSERVERS_USER_LISTGUILD	listGuild;
		
		memset(&listGuild,0,sizeof(WORLDtoSERVERS_USER_LISTGUILD));
		listGuild.base.set(sizeof(WORLDtoSERVERS_USER_LISTGUILD),dWORLDtoSERVERS_USER_LISTGUILD);
		
		listGuild.wAllCount	=	listCount;
		listGuild.wCount	=	0;
		
		memset(&listGuild.info,0xff,sizeof(CGuildAdvanceInfo));
		
		CM_COMPRESS		comp;
//	길드 정보 전송		
		for(int i=0;i<dGUILD_MAX_COUNT || listGuild.wCount<listCount;i++)
		{
			cGuild	* guild = g_guildManager.GetGuildPnt(i);

			g_lifeCodeMgr.putLifeSign(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);

			if	(!guild || guild->wSerial==0xffff)
				continue;

			listGuild.wCount++;
			memcpy(&listGuild.info,(CGuildAdvanceInfo *)guild,sizeof(CGuildAdvanceInfo));
			
			WORD	wCompSize = cSRVUTIL::Compress((unsigned char *)&listGuild,(unsigned char *)comp.data,listGuild.base.wSize);
			comp.wSourceSize = listGuild.base.wSize;
			comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);

			if	(wCompSize ==0)
			{
				_log("Send Guild Info (not comp) : [%d] [%s] [%d]",guild->wSerial,guild->m_strName,guild->m_wMemberCount);
				client->AddSendPacket((char *)&listGuild,listGuild.base.wSize);
			}
			else
			{	
				_log("Send Guild Info (    comp) : [%d] [%s] [%d]",guild->wSerial,guild->m_strName,guild->m_wMemberCount);
				client->AddSendPacket((char *)&comp,comp.wSize);
			}
			
			SendPacket(client->GetSerial());
			Sleep(50);
		}
		
//	길드 멤버 정보 전송		
		for(i=0;i<dGUILD_MAX_COUNT || listGuild.wCount<listCount;i++)
		{
			cGuild	* guild = g_guildManager.GetGuildPnt(i);

			if	(!guild || guild->wSerial==0xffff)
				continue;
			
			g_lifeCodeMgr.putLifeSign(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);

			//	해당 길드의 유저 정보를 보내준다.
			WORLDtoSERVERS_USER_GUILDINFO	guilderList;
			memset(&guilderList,0,sizeof(WORLDtoSERVERS_USER_GUILDINFO));
			guilderList.base.set(sizeof(WORLDtoSERVERS_USER_GUILDINFO),dWORLDtoSERVERS_USER_GUILDINFO);
			guilderList.wGuildSerial=	guild->wSerial;
			guilderList.wAllCount	=	guild->GetCount();
			guilderList.wListCount	=	0;
			
			for	(int j = 0;j<dGUILD_MAX_PLAYER || guilderList.wListCount<guild->GetCount(); j++)
			{
				CGuildMan * guilder = guild->GetGuilderPnt(j);
				
				if	(!guilder || guilder->wSerial==0xffff)
					continue;
				
				if	(STRICMP(guilder->m_strId,"")==0)
				{	
					guild->Del(j);
					MessageBox(NULL,"경고 !! 지현이 부르세요. !! 해당 게임 서버를 다시 시작해 주시기 바랍니다.","WARNING",MB_OK);
					continue;
				}
				
				memcpy(&guilderList.List[guilderList.wListCount],guilder,sizeof(CGuildMan));
				guilderList.wListCount++;
			}
			
			guild->m_wMemberCount = guilderList.wListCount;
			guilderList.base.wSize = sizeof(cMSG_BASE_TYPE_FORWORLD)+sizeof(WORD)*3+sizeof(CGuildMan)*guilderList.wListCount;

			if	(guilderList.wListCount>0)
			{
				WORD	wCompSize = cSRVUTIL::Compress((unsigned char *)&guilderList,(unsigned char *)comp.data,guilderList.base.wSize);
				comp.wSourceSize = guilderList.base.wSize;
				comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);
				
				if(wCompSize ==0)
				{	
					_log("Send Guilder Info (not comp) : [%d] [%s] [%d] [%d]",guild->wSerial,guild->m_strName,guilderList.wListCount,guilderList.wAllCount);
					client->AddSendPacket((char *)&guilderList,guilderList.base.wSize);
				}
				else
				{
					_log("Send Guilder Info (    comp) : [%d] [%s] [%d] [%d]",guild->wSerial,guild->m_strName,guilderList.wListCount,guilderList.wAllCount);
					client->AddSendPacket((char *)&comp,comp.wSize);
				}

				SendPacket(client->GetSerial());	
				Sleep(50);
			}
		}

		SendUpdateGuildHallInfo(client,TRUE);
	}
	
	{
		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);

		WORLDtoSERVERS_LOGIN	login;

		login.base.set(sizeof(WORLDtoSERVERS_LOGIN),dWORLDtoSERVERS_LOGIN);

		login.wBoostGoldByTreasureMapEventCount	=	g_iBoostGoldByTreasureMapEventCount;
		login.wBoostExpByTreasureMapEventCount	=	g_iBoostExpByTreasureMapEventCount;
		login.iHeavenRedStoneCount				=	g_iHeavenRedStoneCount;
		login.iHellRedStoneCount				=	g_iHellRedStoneCount;
		login.iRedDevilRedStoneCount			=	g_iRedDevilRedStoneCount;
		login.iServerType						=	g_iWorldType;

		//	인도자의 종소리
		{
			login.iBoostExpByLeadersSoundofBell		=	g_eventManager.m_iBoostExpByLeadersSoundofBell;
			login.iBoostItemDropChanceByLeadersSoundofBell	=	g_eventManager.m_iBoostItemDropChanceByLeadersSoundofBell;
			login.iRemainSoundofBellTime			=	g_eventManager.m_iRemainSoundofBellTime;
			login.wDisplayNameWithSoundOfLeadersBell=	g_eventManager.m_bIsDisplayNameWithSoundofLeadersBell;

			strcpy(login.strMessageOfSoundOfLeadersBell,g_eventManager.m_strSoundOfLeadersBellMessage);
			strcpy(login.strOwnerOfSoundOfLeadersBell,g_eventManager.m_strSoundOfLeadersBellOwner);
		}

		login.wTrialGameBeginYear				=	g_eventManager.m_timeTrialGameBegin.m_wYear;
		login.wTrialGameBeginMonth				=	g_eventManager.m_timeTrialGameBegin.m_wMonth;
		login.wTrialGameBeginDay				=	g_eventManager.m_timeTrialGameBegin.m_wDay;
		login.wTrialGameBeginHour				=	g_eventManager.m_timeTrialGameBegin.m_wHour;
		login.wTrialGameBeginMinute				=	g_eventManager.m_timeTrialGameBegin.m_wMinute;
		login.wTrialGameEndYear					=	g_eventManager.m_timeTrialGameEnd.m_wYear;
		login.wTrialGameEndMonth				=	g_eventManager.m_timeTrialGameEnd.m_wMonth;
		login.wTrialGameEndDay					=	g_eventManager.m_timeTrialGameEnd.m_wDay;
		login.wTrialGameEndHour					=	g_eventManager.m_timeTrialGameEnd.m_wHour;
		login.wTrialGameEndMinute				=	g_eventManager.m_timeTrialGameEnd.m_wMinute;

		login.wOfflineTournamentBeginYear		=	g_eventManager.m_timeOfflineTournamentBegin.m_wYear;
		login.wOfflineTournamentBeginMonth		=	g_eventManager.m_timeOfflineTournamentBegin.m_wMonth;
		login.wOfflineTournamentBeginDay		=	g_eventManager.m_timeOfflineTournamentBegin.m_wDay;
		login.wOfflineTournamentBeginHour		=	g_eventManager.m_timeOfflineTournamentBegin.m_wHour;
		login.wOfflineTournamentBeginMinute		=	g_eventManager.m_timeOfflineTournamentBegin.m_wMinute;
		login.wIsSendTrialGameResultMail		=	g_eventManager.m_iIsSendTrialGameResultMail;
		login.wTrialGameTeamSize				=	g_eventManager.m_iTrialGameTeamSize;

		login.wOfflineTournamentEndYear			=	g_eventManager.m_timeOfflineTournamentEnd.m_wYear;
		login.wOfflineTournamentEndMonth		=	g_eventManager.m_timeOfflineTournamentEnd.m_wMonth;
		login.wOfflineTournamentEndDay			=	g_eventManager.m_timeOfflineTournamentEnd.m_wDay;
		login.wOfflineTournamentEndHour			=	g_eventManager.m_timeOfflineTournamentEnd.m_wHour;
		login.wOfflineTournamentEndMinute		=	g_eventManager.m_timeOfflineTournamentEnd.m_wMinute;
		login.wSeasonVariable					=	g_iSeasonVariable;
		login.wWorldServerIndex					=	g_iWorldServerIndex;

		strcpy(login.strTrialGameName,g_eventManager.m_strTrialGameName);
		strcpy(login.strOfflineTournamentComment,g_eventManager.m_strOfflineTournamentComment);

		GetSystemTime(&login.systemTime);

		strcpy(login.strServerName,strWorldName);
		memcpy(&login.stMtTime,&g_stMainterenceTime,sizeof(DWORD));
		memcpy(&login.aiEventValue,g_eventManager.getDBControlledEventValue(),sizeof(login.aiEventValue));
		client->AddSendPacket((char *)&login,login.base.wSize);
		client->isReadyToPlay = 1;
	}

	g_lifeCodeMgr.putLifeSign(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);

	Sleep(1000);

//	게임 서버는 
//	WORLDtoSERVERS_LOGIN 패킷을 받기 전에는 월드에서 오는 패킷은 모두 무시함
//	상황에 따라 아래 패킷이 더 늦게 가는 경우가 있으니 반드시 예외 처리 할것
//	게임서버 server.cpp 파일에 "월드 패킷 isActiveGameServer() 예외처리" 로 검색해 볼 것
	sendRenewPremiumItemInfo(client,FALSE);
	Sleep(10);
	g_lifeCodeMgr.putLifeSign(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);
	sendItemPackList(client);
	Sleep(10);
	g_lifeCodeMgr.putLifeSign(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);
	sendGRInfo(client);
	Sleep(10);
	g_lifeCodeMgr.putLifeSign(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);
	sendCarrotShopInfo(client);
	Sleep(10);
	g_lifeCodeMgr.putLifeSign(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);
	sendGSPDuelRankerInfo(client,TRUE);
	Sleep(10);
	g_lifeCodeMgr.putLifeSign(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);
	sendGSPDuelRankerInfo(client,FALSE);
	Sleep(10);
	g_lifeCodeMgr.putLifeSign(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);
	sendGSPDuelTeamInfo(client);
	Sleep(10);
	g_lifeCodeMgr.putLifeSign(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);
	
	Sleep(10);
	g_lottoEventManager.sendEventData(client);

	Sleep(10);
	g_eventTimeManager.sendEventData(client);

	Sleep(10);
	g_partyManager.updateAllPartyInfo((void *)client);

	g_lifeCodeMgr.endCheck(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA);
}

void	SendGameServerInitData(WORD  _dwGameServerSerial)
{
	HANDLE hThread = (HANDLE)_beginthread(SendGameServerInitDataThread, 0, (LPVOID)_dwGameServerSerial); 

	if	(hThread==NULL)
	{	
		_log("Fail SendGameServerInitDataThread");	
		return ;	
	}else
	{
		_log("Success SendGameServerInitDataThread");	
	}
}

void	DeleteGuildPacket(DWORD _dwGameServerSerial,WORD _wGuildSerial,char * _strGuildName, char * _strRequestMan, WORD _wReason)
{
	WDMSG_DELETEGUILD		deleteGuild;
	deleteGuild.base.set(sizeof(WDMSG_DELETEGUILD),dWDMSG_DELETEGUILD);
	deleteGuild.dwGameServerSerial = _dwGameServerSerial;
	strcpy(deleteGuild.strName,_strGuildName);
	strcpy(deleteGuild.strRequestMan,_strRequestMan);
	deleteGuild.wIndex = _wGuildSerial;
	deleteGuild.wReason	=	_wReason;
	GetADBSocket()->SendPacket((char *)&deleteGuild,deleteGuild.base.wSize);
}

void	DeleteGuild(WORD _wGuildSerial, char * _strGuildName,char * _strWhy)
{
	_log("[DELETE GUILD] %s : %s",_strGuildName, _strWhy);
	//	해당 길드 포인터를 찾아서
	cGuild	* delGuild = g_guildManager.GetGuildPntBySerial(_wGuildSerial);

	if	(delGuild && delGuild->wSerial!=0xffff && STRICMP(delGuild->m_strName,_strGuildName)==0)
	{
		//	길드원이 존재한다면 각 게임서버로 길드 삭제 정보를 보낸다.
		WORLDtoSERVERS_DELETEGUILD		delGuildforServer;
		delGuildforServer.base.set(sizeof(WORLDtoSERVERS_DELETEGUILD),dWORLDtoSERVERS_DELETEGUILD);
		strcpy(delGuildforServer.strGuildName,_strGuildName);
		delGuildforServer.wGuildSerial = _wGuildSerial;
		SendDataAllGameServer((char *)&delGuildforServer,delGuildforServer.base.wSize);
		//	길드 메니져에서 삭제할 것 !!

		g_guildManager.Del(_strGuildName);
	}
	else
	{
		if(delGuild && delGuild->wSerial!=0xffff)
			_log("ERROR DELETE GUILD : GUILD NAME [%s] , REQUEST GUILD NAME FOR DELETE [%s]",delGuild->m_strName,_strGuildName);
		else
			_log("ERROR DELETE GUILD : GUILD NAME [%s] , why [%s]SERIAL [%d] ",_strGuildName,_strWhy,_wGuildSerial);
	}
}

void	DeleteGuilder(WORD _guildSerial,char * _strId,char * _strName)
{
	BOOL	isDeletedGuild = FALSE;
	
	if(_guildSerial==0xffff)	return;
	
	cGuild * guild = g_guildManager.GetGuildPntBySerial(_guildSerial);
	if	(!guild || guild->wSerial==0xffff)
		return;

	CGuildMan * member = guild->GetGuilderPnt(_strName);

	if	(member && member->wSerial!=0xffff && STRICMP(member->m_strId,_strId)==0)
	{
		WDMSG_DELETEGUILDER		delGuilder;
		delGuilder.base.set(sizeof(WDMSG_DELETEGUILDER),dWDMSG_DELETEGUILDER);
		strcpy(delGuilder.strTarget,_strName);
		delGuilder.wIndex = guild->m_wSerial;
		strcpy(delGuilder.strName,guild->m_strName);
		GetADBSocket()->SendPacket((char *)&delGuilder,delGuilder.base.wSize);
		
		WORLDtoSERVERS_GUILD_EXIT	gExit;
		gExit.base.set(sizeof(WORLDtoSERVERS_GUILD_EXIT),dWORLDtoSERVERS_GUILD_EXIT);
		strcpy(gExit.strGuildName,guild->m_strName);
		strcpy(gExit.strMemberName,_strName);
		gExit.wGuildSerial = guild->m_wSerial;
		
		//	마지막 한명일경우.. , 길드를 삭제한다.
		if	(guild->GetCount()==1)
		{
			DeleteGuildPacket(0xffff,guild->m_wSerial,guild->m_strName,"");
			DeleteGuild(guild->m_wSerial,guild->m_strName,"Leave Guild All member");
			isDeletedGuild = TRUE;
		}
		else
		{
			//	새롭게 길드 마스터를 정해서 DB로 보내준다.
			switch(member->m_wRank)
			{
			case dGUILD_CLASS_PARLIAMENT:
				guild->m_wCongressManCount--;
				break;
			case dGUILD_CLASS_SUBMASTER:
				if	(guild->m_wSubMasterCount>0)
					guild->m_wSubMasterCount--;
				else
					_log("ERROR submastercount minus: [%s] [%d] (DeleteGuilder)",guild->m_strName,guild->m_wSubMasterCount);
				break;
			case dGUILD_CLASS_ELDER:
				if	(guild->m_wElderCount>0)
					guild->m_wElderCount--;
				else
					_log("ERROR ElderCount minus: [%s] [%d] (DeleteGuilder)",guild->m_strName,guild->m_wElderCount);
				break;
			case dGUILD_CLASS_MASTER:
				{	//	마스터가 나간경우, DB에서는 이미 잘려 있는 상태임
					//	Sub를 찾아본다.
					CGuildMan * nextMaster = guild->GetNextMaster();
					if(!nextMaster)
					{	//	다음 마스터가 없으므로 삭제된다.
						_log("ERROR : Can not found Next Master. And this guild is delete!!");
						DeleteGuildPacket(0xffff,guild->m_wSerial,guild->m_strName,"");
						DeleteGuild(guild->m_wSerial,guild->m_strName,"다음을 이어받을 마스터가 없다.");
						isDeletedGuild = TRUE;
					}
					else
					{
						//	DB로 랭크 변경 패킷만을 보내준다.
						WDMSG_CHANGERANK	sPacket;
						sPacket.base.set(sizeof(WDMSG_CHANGERANK),dWDMSG_CHANGERANK);
						sPacket.wIndex = guild->m_wSerial;
						strcpy(sPacket.strName,guild->m_strName);
						sPacket.dwGameServerSerial = 0xffff;
						strcpy(sPacket.strTarget,nextMaster->m_strName);
						sPacket.wRank		=	dGUILD_CLASS_MASTER;
						sPacket.wReason		=	eCGR_CHANGERANK_MASTER_APPOINT_SPECIAL;
						sPacket.wType = dGUILD_CHANGERANK_MASTER_APPONT_SPECIAL;
						memset(sPacket.strMaster,0,dNAME_LENGTH);
						GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
						int tomorrow = cSRVUTIL::GetNextTime((guild->m_timeLastConnectedMasterTime.year+2000)*10000 + guild->m_timeLastConnectedMasterTime.month*100 + guild->m_timeLastConnectedMasterTime.day);
						guild->m_timeLastConnectedMasterTime.year = (tomorrow /10000)%100;
						guild->m_timeLastConnectedMasterTime.month = (tomorrow %10000)/100;
						guild->m_timeLastConnectedMasterTime.day = tomorrow %100;
						SaveGuildBaseData(guild);
					}
					break;
				}
			}
			
			if	(!isDeletedGuild)
			{
				//	해당 캐릭터는 리스트에서 삭제하고..
				guild->Del(_strName);
				guild->updateGuildInfo();
				SendDataAllGameServer((char *)&gExit,gExit.base.wSize);

				{
					SG_REMOVE_GUILD_MEMBER	packet;

					packet.base.set(sizeof(packet),dSG_REMOVE_GUILD_MEMBER);

					packet.wGuildSerial			=	guild->m_wSerial;
					packet.bMemberCount			=	(BYTE)guild->m_wMemberCount;
					packet.bSubMasterCount		=	(BYTE)guild->m_wSubMasterCount;
					packet.bCongressManCount	=	(BYTE)guild->m_wCongressManCount;

					strcpy(packet.strMemberName,_strName);

					SendGuildPacketToBCS(&packet,guild->m_wSerial);
				}
			}
		}
	}
}

void	SendDataAllGameServer(char * _data,WORD _size,BOOL _bIsWantOnlyGS)
{
	for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		CClient	* usr = UserManager.GetClientPnt(i,FALSE);
		
		if	(!usr || usr->GetSerial()==0xffff)
			continue;

		if	(_bIsWantOnlyGS	&&	usr->m_wIsGameServer	==	FALSE)
			continue;
		
		usr->AddSendPacket(_data,_size);
		SendPacket(usr->GetSerial());
	}
}

void	UpdateGuildData(cGuild * guild,DWORD	_dwSerial)
{
	SaveGuildBaseData(guild);
//	SaveGuildSkill(guild,_dwSerial);
//	SaveGuildMarkData(guild,_dwSerial);
//	SaveGuildPet(guild,_dwSerial);
//	SaveGuildGuardian(guild,_dwSerial);
//	SaveGuildStructure(guild,_dwSerial);
	SaveGuildInven(guild,_dwSerial);
}

void	LoadGuildDataInFile(cGuild * _lpGuild)
{
	CGuildPetInfo		aGuildPet[dGUILD_PET_TYPE_COUNT],aGuildGuardian[dGUILD_GUARDIAN_TYPE_COUNT];

	char	strCurrentFolder[1024];
	
	GetCurrentDirectory(1024,strCurrentFolder);
	SetCurrentDirectory(g_strLogFolder);
	
	cFILE	file;
	
	if	(file.Open(_ms("%s/data/guild/%s.gp",g_strLogFolder,_lpGuild->m_strName),"rb"))
	{
		file.Read(aGuildPet,sizeof(aGuildPet));
		file.Read(aGuildGuardian,sizeof(aGuildGuardian));
		file.Close();

		DWORD	dwPetFileCRC,dwGuardianFileCRC,dwPetCRC,dwGuardianCRC;

		cCRC32::BufferToCrc32(aGuildPet,sizeof(aGuildPet),dwPetFileCRC);
		cCRC32::BufferToCrc32(aGuildGuardian,sizeof(aGuildGuardian),dwGuardianFileCRC);
		cCRC32::BufferToCrc32(_lpGuild->m_aGuildPet,sizeof(aGuildPet),dwPetCRC);
		cCRC32::BufferToCrc32(_lpGuild->m_aGuildGuardian,sizeof(aGuildGuardian),dwGuardianCRC);

		if	(dwPetFileCRC	!=	dwPetCRC)
		{
			_log("%s - incorrect guild pet info",_lpGuild->m_strName);

			memcpy(_lpGuild->m_aGuildPet,aGuildPet,sizeof(aGuildPet));
		}
		if	(dwGuardianFileCRC	!=	dwGuardianCRC)
		{
			_log("%s - incorrect guild guardian info",_lpGuild->m_strName);

			memcpy(_lpGuild->m_aGuildGuardian,aGuildGuardian,sizeof(aGuildGuardian));
		}
	}
		
	SetCurrentDirectory(strCurrentFolder);
}

void	SaveGuildMarkData(cGuild * guild,DWORD	_dwSerial)
{
/*	WDMSG_SAVEGUILD_MARK	packet;
	
	packet.base.set(sizeof(WDMSG_SAVEGUILD_MARK),dWDMSG_SAVEGUILD_MARK);
	packet.dwGameServerSerial	=	_dwSerial;
	packet.wIndex				=	guild->m_wSerial;
	strcpy(packet.strName,guild->m_strName);
	memcpy(&packet.m_guildMarkInfo,&guild->m_guildMarkInfo,sizeof(CGuildMarkInfo));
	
	CM_COMPRESS		comp;
	
	WORD	wCompSize = cSRVUTIL::Compress((unsigned char *)&packet,(unsigned char *)comp.data,packet.base.wSize);
	comp.wSourceSize = packet.base.wSize;
	comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);
	
	if(wCompSize ==0)
		GetADBSocket()->SendPacket((char *)&packet,packet.base.wSize);
	else
		GetADBSocket()->SendPacket((char *)&comp,comp.wSize);

	{
		char	strCurrentFolder[1024];

		GetCurrentDirectory(1024,strCurrentFolder);
		SetCurrentDirectory(g_strLogFolder);

		cFILE	file;

		file.Open(_ms("%s/data/%s.gm",g_strLogFolder,guild->m_strName),"wb");
		file.Write(&guild->m_guildMarkInfo,sizeof(guild->m_guildMarkInfo));
		file.Close();

		SetCurrentDirectory(strCurrentFolder);
	}
*/
}

void	SaveGuildBaseData(cGuild * guild)
{
	WDMSG_SAVEGUILDBASE	sPacket;
	sPacket.base.set(sizeof(WDMSG_SAVEGUILDBASE),dWDMSG_SAVEGUILDBASE);
	guild->getSaveData_Column(&sPacket.dataColumn);
	guild->getSaveData_Binary(&sPacket.dataBinary);
	guild->getSaveData_Mark(&sPacket.dataMark);
	guild->getSaveData_Bank(&sPacket.dataBank);
	guild->getSaveData_RelShip(&sPacket.dataRelShip);
	
	CM_COMPRESS		comp;
	WORD	wCompSize = cSRVUTIL::Compress((unsigned char *)&sPacket,(unsigned char *)comp.data,sPacket.base.wSize);
	comp.wSourceSize = sPacket.base.wSize;
	comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);
	
	if(wCompSize == 0)	GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
	else				GetADBSocket()->SendPacket((char *)&comp,comp.wSize);
}

void	SaveGuildSkill(cGuild * guild,DWORD	_dwSerial)
{
	WDMSG_SAVEGUILD_SKILL	sPacket;
	sPacket.base.set(sizeof(WDMSG_SAVEGUILD_SKILL),dWDMSG_SAVEGUILD_SKILL);
	sPacket.dwGameServerSerial = _dwSerial;
	sPacket.wIndex = guild->m_wSerial;
	sPacket.wGuildPoint = guild->m_wGuildPoint;

	for	(int i=dMAX_GUILD_SKILL_COUNT;i;)
	{
		--i;
		sPacket.m_aGuildSkill[i]	=	guild->m_abGuildSkill[i];
	}
	strcpy(sPacket.strName,guild->m_strName);
	
	CM_COMPRESS		comp;
	WORD	wCompSize = cSRVUTIL::Compress((unsigned char *)&sPacket,(unsigned char *)comp.data,sPacket.base.wSize);
	comp.wSourceSize = sPacket.base.wSize;
	comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);
	
	if(wCompSize == 0)	GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
	else				GetADBSocket()->SendPacket((char *)&comp,comp.wSize);
}

void	SaveGuildPet(cGuild * guild,DWORD	_dwSerial)
{
	WDMSG_SAVEGUILD_PET	sPacket;
	sPacket.base.set(sizeof(WDMSG_SAVEGUILD_PET),dWDMSG_SAVEGUILD_PET);
	sPacket.dwGameServerSerial = _dwSerial;
	sPacket.wIndex = guild->m_wSerial;
	memcpy(sPacket.m_aGuildPet,guild->m_aGuildPet,sizeof(sPacket.m_aGuildPet));
	strcpy(sPacket.strName,guild->m_strName);
	
	GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);

	{
		char	strCurrentFolder[1024];

		GetCurrentDirectory(1024,strCurrentFolder);
		SetCurrentDirectory(g_strLogFolder);

		cFILE	file;

		if	(file.Open(_ms("%s/data/guild/%s.gp",g_strLogFolder,guild->m_strName),"wb"))
		{
			file.Write(&guild->m_aGuildPet,sizeof(guild->m_aGuildPet));
			file.Write(&guild->m_aGuildGuardian,sizeof(guild->m_aGuildGuardian));
			file.Close();
		}

		SetCurrentDirectory(strCurrentFolder);
	}
}

void	SaveGuildGuardian(cGuild * guild,DWORD	_dwSerial)
{
	WDMSG_SAVEGUILD_GUARDIAN	sPacket;
	sPacket.base.set(sizeof(WDMSG_SAVEGUILD_GUARDIAN),dWDMSG_SAVEGUILD_GUARDIAN);
	sPacket.dwGameServerSerial = _dwSerial;
	sPacket.wIndex = guild->m_wSerial;
	memcpy(sPacket.m_aGuildGuardian,guild->m_aGuildGuardian,sizeof(sPacket.m_aGuildGuardian));
	strcpy(sPacket.strName,guild->m_strName);
	
	GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);

	{
		char	strCurrentFolder[1024];

		GetCurrentDirectory(1024,strCurrentFolder);
		SetCurrentDirectory(g_strLogFolder);

		cFILE	file;

		if	(file.Open(_ms("%s/data/guild/%s.gp",g_strLogFolder,guild->m_strName),"wb"))
		{
			file.Write(&guild->m_aGuildPet,sizeof(guild->m_aGuildPet));
			file.Write(&guild->m_aGuildGuardian,sizeof(guild->m_aGuildGuardian));
			file.Close();
		}

		SetCurrentDirectory(strCurrentFolder);
	}
}

void	SaveGuildStructure(cGuild * guild,DWORD	_dwSerial)
{
	WDMSG_SAVEGUILD_STRUCTURE	sPacket;
	sPacket.base.set(sizeof(WDMSG_SAVEGUILD_STRUCTURE),dWDMSG_SAVEGUILD_STRUCTURE);
	sPacket.dwGameServerSerial = _dwSerial;
	sPacket.wIndex = guild->m_wSerial;
	memcpy(sPacket.m_aGuildStructure,guild->m_aGuildStructure,sizeof(sPacket.m_aGuildStructure));
	strcpy(sPacket.strName,guild->m_strName);
	
	GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
}

void	SaveGuildInven(cGuild * guild,DWORD	_dwSerial)
{
	WDMSG_SAVEGUILD_INVENTORY	sPacket;
	sPacket.base.set(sizeof(WDMSG_SAVEGUILD_INVENTORY),dWDMSG_SAVEGUILD_INVENTORY);
	sPacket.dwGameServerSerial = _dwSerial;
	sPacket.wIndex = guild->m_wSerial;
	memcpy(sPacket.m_aInventory,guild->m_aInventory,sizeof(sPacket.m_aInventory));
	strcpy(sPacket.strName,guild->m_strName);
	
	GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
}

void
sendRenewPremiumItemInfo(CClient *_lpUser,BOOL _bIsNow)
{
	WORLDtoSERVERS_RENEW_PREMIUM_ITEM	renew;
	
	for (int i=0;i<g_renewPremiumItemExpireTime.m_iCount;i++)
	{
		renew.aRenewInfo[i].bf14Index			=	g_renewPremiumItemExpireTime.m_aList[i].m_bf14Index;
		renew.aRenewInfo[i].bf1IsForOnlyYahoo	=	g_renewPremiumItemExpireTime.m_aList[i].m_bf1IsForOnlyYahoo;
		renew.aRenewInfo[i].bf1IsForOnlyRedGem	=	g_renewPremiumItemExpireTime.m_aList[i].m_bf1IsForOnlyRedGem;
		renew.aRenewInfo[i].wTerm				=	(WORD)g_renewPremiumItemExpireTime.m_aList[i].m_dwRenewMinuite;
	}
	
	renew.wRenewCount	=	g_renewPremiumItemExpireTime.m_iCount;
	renew.wIsNow		=	_bIsNow;
	renew.base.wType	=	dWORLDtoSERVERS_RENEW_PREMIUM_ITEM;
	renew.base.wSize	=	sizeof(renew)-sizeof(renew.aRenewInfo)+renew.wRenewCount*sizeof(cSimpleRenewInfo);
	
	mSEND_GSP(_lpUser,renew);
}

void
sendGSPCalling(CClient *_lpUser,SERVERStoWORLD_CALLING *_lpPacket)
{
	WORLDtoSERVERS_CALLING	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_CALLING);
	
	packet.wCasterField		=	_lpPacket->wCasterField;
	packet.wParty			=	_lpPacket->wParty;
	packet.wCaterXPos		=	_lpPacket->wCaterXPos;
	packet.wCaterYPos		=	_lpPacket->wCaterYPos;
	packet.wLimitRange		=	_lpPacket->wLimitRange;
	packet.wSkill			=	_lpPacket->wSkill;
	packet.dwNameHashCode	=	_lpPacket->dwNameHashCode;
	
	mSEND_GSP(_lpUser,packet);
}

//
//	게임 서버에 패킷 전송
void
sendGSPValidGuild(CClient *_lpUser,int _iGuildSerial)
{
	WORLDtoSERVERS_VALID_GUILD	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_VALID_GUILD);
	packet.wGuildSerial	=	_iGuildSerial;
	
	mSEND_GSP(_lpUser,packet);
}

void
sendGSPChangeDay(CClient *_lpUser)
{
	WORLDtoSERVERS_CHANGE_DAY		packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_CHANGE_DAY);
	
	mSEND_GSP(_lpUser,packet);
}

//
//	길드전 스케쥴 전송
void
sendGSPGuildBattleSchedule(CClient *_lpUser)
{
	WORLDtoSERVERS_GUILD_BATTLE_SCHEDULE	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_GUILD_BATTLE_SCHEDULE);
	
	memcpy(&packet.todaySchedule	,&g_guildBattleManager.m_gbsToday		,sizeof(CGuildBattleSchedule));

	mSEND_GSP(_lpUser,packet);
}

void
sendGSPResetWeekFightGuildInfo(CClient *_lpUser)
{
	WORLDtoSERVERS_RESET_WEEK_FIGHT_GUILD_INFO	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_RESET_WEEK_FIGHT_GUILD_INFO);
	
	mSEND_GSP(_lpUser,packet);
}

//
//	게임 서버에 패킷 전송
void
sendGSPUpdateGuildInfo(CClient *_lpUser,int _iGuildSerial,BOOL _bIsRequireResetMemberInfo)
{
	WORLDtoSERVERS_UPDATE_GUILD_INFO	packet;

	cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(_iGuildSerial);

	if (!lpGuild)
	{
		sendGSPValidGuild(_lpUser,_iGuildSerial);
		
		return;
	}

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_UPDATE_GUILD_INFO);
	packet.wGuildSerial					=	lpGuild->wSerial;
	packet.wIsRequireResetMemberInfo	=	_bIsRequireResetMemberInfo;

	memcpy(&packet.info,(CGuildAdvanceInfo *)lpGuild,sizeof(CGuildAdvanceInfo));
	
	CM_COMPRESS		comp;
	
	WORD	wCompSize	=	cSRVUTIL::Compress((unsigned char *)&packet,(unsigned char *)comp.data,packet.base.wSize);
	comp.wSourceSize	=	packet.base.wSize;
	comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);
	
	if	(wCompSize ==0)
	{
		mSEND_GSP(_lpUser,packet);
	}
	else
	{
		if	(_lpUser)
			_lpUser->AddSendPacket((char *)&comp,comp.wSize);
		else
			SendDataAllGameServer((char *)&comp,comp.wSize);
	}
}

void
operateBeginTestGuildBattle(CClient *_lpUser,SERVERStoWORLD_BEGIN_TEST_GUILD_BATTLE *_lpPacket)
{
	WORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE_RESULT	packetResult;
	
	packetResult.base.set(sizeof(packetResult),dWORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE_RESULT);
	packetResult.wResult	=	0;
	
	cGuild	*lpGuild1	=	g_guildManager.GetGuildPnt(_lpPacket->strGuild1);
	cGuild	*lpGuild2	=	g_guildManager.GetGuildPnt(_lpPacket->strGuild2);
	
	if (!lpGuild1 || !lpGuild2)
	{
		packetResult.wResult	=	1;
		
		mSEND_GSP(_lpUser,packetResult);
		
		return;
	}
	
	mSEND_GSP(_lpUser,packetResult);
	
	WORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE);
	
	strcpy(packet.strGuild1,_lpPacket->strGuild1);
	strcpy(packet.strGuild2,_lpPacket->strGuild2);
	packet.wField		=	_lpPacket->wField;
	packet.wTime		=	_lpPacket->wTime;
	
	_lpUser				=	NULL;
	
	mSEND_GSP(_lpUser,packet);
}

void
sendGSPGuildNameList(CClient *_lpUser,int _iFirstGuild,int _iServerIndex)
{
	WORLDtoSERVERS_GUILD_NAME_LIST	packet;
	
	packet.base.set(sizeof(WORLDtoSERVERS_GUILD_NAME_LIST),dWORLDtoSERVERS_GUILD_NAME_LIST);

	if	(_iFirstGuild	==	0)
		packet.bf1IsBegin	=	TRUE;

	packet.bf1IsLast	=	TRUE;
	packet.bf14Count	=	0;
	packet.iServerIndex	=	_iServerIndex;
	packet.iLastGuild	=	0;
	
	int			i;
	CM_COMPRESS	comp;
	WORD		wCompSize;
	
	for(i=_iFirstGuild;i<dGUILD_MAX_COUNT;i++)
	{
		cGuild	*lpGuild = g_guildManager.GetGuildPnt(i);
		
		if	(!lpGuild || lpGuild->wSerial==0xffff)
			continue;
		
		strcpy(packet.strGuildNameList[packet.bf14Count],lpGuild->m_strName);
		
		packet.bf14Count++;
		packet.iLastGuild	=	lpGuild->wSerial;
		
		if	(packet.bf14Count	>=	100)
		{
			packet.bf1IsLast	=	FALSE;	//	중간에 끊겼으므로 마지막이 아니다.
			break;
		}
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strGuildNameList)+dGUILD_NAME_LENGTH*packet.bf14Count;
	
	wCompSize			=	cSRVUTIL::Compress((unsigned char *)&packet,(unsigned char *)comp.data,packet.base.wSize);
	comp.wSourceSize	=	packet.base.wSize;
	comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);
	
	if	(wCompSize ==0)
	{
		mSEND_GSP(_lpUser,packet);
	}
	else
		_lpUser->AddSendPacket((char *)&comp,comp.wSize);
}

void
sendGSPGuildName(CClient *_lpUser,int _iGuildSerial)
{
	WORLDtoSERVERS_GUILD_NAME	packet;
	
	packet.base.set(sizeof(WORLDtoSERVERS_GUILD_NAME),dWORLDtoSERVERS_GUILD_NAME);
	
	cGuild	*lpGuild = g_guildManager.GetGuildPnt(_iGuildSerial);
	
	if(!lpGuild || lpGuild->wSerial==0xffff)
		packet.wGuildSerial	=	0xffff;
	else
	{
		packet.wGuildSerial	=	_iGuildSerial;
		strcpy(packet.strGuildName,lpGuild->m_strName);
	}
	
	mSEND_GSP(_lpUser,packet);
}

//	길드 멤버 정보 전송
void
sendGSPUpdateGuildMemberInfo(CClient *_lpUser,int _iGuildSerial)
{
	cGuild	* lpGuild = g_guildManager.GetGuildPnt(_iGuildSerial);
	
	if	(!lpGuild || lpGuild->wSerial==0xffff)
	{
		sendGSPValidGuild(_lpUser,_iGuildSerial);
		return;
	}
	
	WORLDtoSERVERS_USER_GUILDINFO	packet;
	
	packet.base.set(sizeof(WORLDtoSERVERS_USER_GUILDINFO),dWORLDtoSERVERS_USER_GUILDINFO);
	
	packet.wGuildSerial	=	lpGuild->wSerial;
	packet.wAllCount	=	lpGuild->GetCount();
	packet.wListCount	=	0;
	
	for(int j = 0;j<dGUILD_MAX_PLAYER || packet.wListCount<lpGuild->GetCount(); j++)
	{
		CGuildMan * lpMember = lpGuild->GetGuilderPnt(j);
		
		if(!lpMember || lpMember->wSerial==0xffff)
			continue;
		
		if(STRICMP(lpMember->m_strId,"")==0)
		{	
			lpGuild->Del(j);
			_log("ERROR %s guild member id valid. index %d",lpGuild->m_strName,j);
			continue;
		}
		
		memcpy(&packet.List[packet.wListCount],lpMember,sizeof(CGuildMan));
		packet.wListCount++;
	}
	
	CM_COMPRESS	comp;
	
	lpGuild->m_wMemberCount	=	packet.wListCount;
	packet.base.wSize		=	sizeof(cMSG_BASE_TYPE_FORWORLD)+sizeof(WORD)*3+sizeof(CGuildMan)*packet.wListCount;
	
	if(packet.wListCount	>	0)
	{
		WORD	wCompSize	=	cSRVUTIL::Compress((unsigned char *)&packet,(unsigned char *)comp.data,packet.base.wSize);
		
		comp.wSourceSize	=	packet.base.wSize;
		
		comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);
		
		if	(wCompSize ==0)
		{
			mSEND_GSP(_lpUser,packet);
		}
		else
			_lpUser->AddSendPacket((char *)&comp,comp.wSize);
	}
}

//
//	특정시간대의 길드전정보 전송
void
sendGSPGuildBattleInfo(CClient *_lpUser,int _iTimeOrder,int _iFieldOrder)
{
	WORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO	packet;
	
	CGuildBattleSchedule	*lpGBS;
	
	lpGBS	=	&g_guildBattleManager.m_gbsToday;
	
	CGuildBattleInfo		*lpInfo	=	&lpGBS->m_aBattleGuildSchedule[_iTimeOrder].m_aBattleGuildInfo[_iFieldOrder];
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO);
	
	packet.wTimeOrder		=	_iTimeOrder;
	packet.wFieldOrder		=	_iFieldOrder;
	packet.wGuildSerial		=	0xffff;
	
	cGuild	*lpGuild1		=	g_guildManager.GetGuildPntBySerial(lpInfo->m_awBattleGuild[0]);
	cGuild	*lpGuild2		=	g_guildManager.GetGuildPntBySerial(lpInfo->m_awBattleGuild[1]);
	
	if (!lpGuild1)
		lpInfo->m_awBattleGuild[0]	=	0xffff;
	if (!lpGuild2)
		lpInfo->m_awBattleGuild[1]	=	0xffff;
	
	packet.awBattleGuild[0]	=	lpInfo->m_awBattleGuild[0];
	packet.awBattleGuild[1]	=	lpInfo->m_awBattleGuild[1];
	
	mSEND_GSP(_lpUser,packet);
}

//
//	길드전 결과 처리
void
operateGuildBattleResult(CClient *_lpUser,SERVERStoWORLD_GUILD_BATTLE_RESULT *_lpPacket)
{
	int	i;
	
	WORLDtoSERVERS_GUILD_BATTLE_RESULT	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_GUILD_BATTLE_RESULT);
	
	//	전투한 길드를 얻어온다.
	cGuild	*lpGuild1	=	g_guildManager.GetGuildPntBySerial(_lpPacket->awGuildSerial[0]);
	cGuild	*lpGuild2	=	g_guildManager.GetGuildPntBySerial(_lpPacket->awGuildSerial[1]);
	
	if (!lpGuild1 && !lpGuild2)
		return;
	
	int	aiGuild1VP[2]	=	{-1,-1};
	
	//	길드전 했던 녀석들의 스택을 앞으로 밀어버린다.
	for (i=dMAX_FIGHT_ATWEEK-2;i>=0;i--)
	{
		if (lpGuild1)
			lpGuild1->m_wFightGuild[i+1]=	lpGuild1->m_wFightGuild[i];
		if (lpGuild2)
			lpGuild2->m_wFightGuild[i+1]=	lpGuild2->m_wFightGuild[i];
	}
	//	오늘 길드전 했던 녀석들을 가장 앞에 둔다.
	if (lpGuild1)
	{
		lpGuild1->m_wFightGuild[0]	=	_lpPacket->awGuildSerial[1];
		aiGuild1VP[0]	=	lpGuild1->m_iVictoryPoint;
	}
	if (lpGuild2)
	{
		lpGuild2->m_wFightGuild[0]	=	_lpPacket->awGuildSerial[0];
		aiGuild1VP[1]	=	lpGuild2->m_iVictoryPoint;
	}
	
	packet.aBattleGuild[0].m_wGuildSerial	=	_lpPacket->awGuildSerial[0];
	packet.aBattleGuild[1].m_wGuildSerial	=	_lpPacket->awGuildSerial[1];

//	1. 싸워서 이긴 경우 (상대방의 랭킹 포인트 - 자신의 랭킹 포인트)/2만큼 자신의 랭킹 포인트 상승
//  2. 이긴 경우 증가 랭킹 포인트의 최소값은 1이다. (음수나 0일때도 1은 증가)
//  3. 연승을 할 경우에는 연승 하나당 2점의 보너스 포인트를 준다. (5연승까지만 적용. 이후는 5연승과 동일한 보너스 포인트)
//	4. 랭킹 포인트 증가치는 최대 100을 넘을 수 없다

// 	for (i=0;i<2;i++)
// 	{
// 		cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(_lpPacket->awGuildSerial[i]);
// 		int		iAddPoint	=	0;
// 		
// 		packet.aBattleGuild[i].m_wGuildSerial		=	0xffff;
// 
//  		if	(!lpGuild)
// 			continue;
// 
// 		if	(_lpPacket->abWin[0] == TRUE && _lpPacket->abWin[1] == TRUE )	//	무승부
// 		{
// 			packet.aBattleGuild[i].m_wResult	=	eGBR_DRAW;
// 			lpGuild->m_wDrawCount++;
// 			iAddPoint	=	1;
// 		}
// 		else
// 		if	(_lpPacket->abWin[i] == TRUE)	//	1번 길드 윈~
// 		{
// 			packet.aBattleGuild[i].m_wResult	=	eGBR_WIN;
// 
// 			if	(aiGuild1VP[1-i]	!=	-1)
// 			{
// 				iAddPoint	=	(aiGuild1VP[1-i]-lpGuild->m_iVictoryPoint)/2;
// 				iAddPoint	=	max(iAddPoint,1);
// 				iAddPoint	+=	min(lpGuild->m_wVictoryContinue,5)*2;
// 			}
// 
// 			iAddPoint	=	max(iAddPoint,1);
// 			iAddPoint	=	min(iAddPoint,100);
// 
// 			lpGuild->m_wVictoryCount++;
// 			lpGuild->m_wVictoryContinue++;
// 
// 			lpGuild->m_iVictoryPoint	+=	iAddPoint;
// 		}
// 		else
// 		{
// 			packet.aBattleGuild[i].m_wResult	=	eGBR_LOSE;
// 			lpGuild->m_wDefeatCount++;
// 			lpGuild->m_wVictoryContinue	=	0;
// 		}
// 
// 		packet.aBattleGuild[i].m_wGuildSerial			=	_lpPacket->awGuildSerial[i];
// 		packet.aBattleGuild[i].m_iVictoryPoint			=	lpGuild->m_iVictoryPoint;	//	길드 승점
// 		packet.aBattleGuild[i].m_wVictoryContinue		=	lpGuild->m_wVictoryContinue;
// 		packet.aBattleGuild[i].m_wVictoryCount			=	lpGuild->m_wVictoryCount;
// 		packet.aBattleGuild[i].m_wDefeatCount			=	lpGuild->m_wDefeatCount;
// 		packet.aBattleGuild[i].m_wDrawCount				=	lpGuild->m_wDrawCount;
// 		packet.aBattleGuild[i].m_wCancelCount			=	lpGuild->m_wCancelCount;
// 		packet.aBattleGuild[i].m_wIncreaseVictoryPoint	=	iAddPoint;
// 		
// 		SaveGuildBaseData(lpGuild);
// 	}
	
	for (i=0;i<2;i++)
	{
		cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(_lpPacket->awGuildSerial[i]);
		int		iAddPoint	=	0;
		
		packet.aBattleGuild[i].m_wGuildSerial		=	0xffff;

		if	(!lpGuild)
			continue;

		lpGuild->m_bf3ThisDayGuildBattleCount++;	//	오늘 길드전 횟수
		lpGuild->m_bf5ThisWeekGuildBattleCount++;	//	주중 길드전 횟수

		if	(_lpPacket->abWin[0] == TRUE && _lpPacket->abWin[1] == TRUE )	//	무승부
		{
			packet.aBattleGuild[i].m_wResult	=	eGBR_DRAW;
			lpGuild->m_wDrawCount++;
		}
		else
		if	(_lpPacket->abWin[i] == TRUE)	//	윈~
		{
			packet.aBattleGuild[i].m_wResult	=	eGBR_WIN;

			if	(lpGuild->m_iVictoryPoint > aiGuild1VP[1-i])
				iAddPoint	=	1;
			else
			if	(lpGuild->m_iVictoryPoint == aiGuild1VP[1-i])
				iAddPoint	=	3;
			else
			if	(lpGuild->m_iVictoryPoint < aiGuild1VP[1-i])
				iAddPoint	=	5;

// 			printf("길드 [%s] 현재 승점 [%d],상대 승점 [%d],획득 승점 [%d]\n",lpGuild->m_strName,aiGuild1VP[1-i],lpGuild->m_iVictoryPoint,iAddPoint);

			lpGuild->m_wVictoryCount++;
			lpGuild->m_wVictoryContinue++;

			lpGuild->m_iVictoryPoint	+=	iAddPoint;
		}
		else
		{
			packet.aBattleGuild[i].m_wResult	=	eGBR_LOSE;

			lpGuild->m_wDefeatCount++;
			lpGuild->m_wVictoryContinue	=	0;
		}

		packet.aBattleGuild[i].m_wGuildSerial			=	_lpPacket->awGuildSerial[i];
		packet.aBattleGuild[i].m_iVictoryPoint			=	lpGuild->m_iVictoryPoint;	//	길드 승점
		packet.aBattleGuild[i].m_wVictoryContinue		=	lpGuild->m_wVictoryContinue;
		packet.aBattleGuild[i].m_wVictoryCount			=	lpGuild->m_wVictoryCount;
		packet.aBattleGuild[i].m_wDefeatCount			=	lpGuild->m_wDefeatCount;
		packet.aBattleGuild[i].m_wDrawCount				=	lpGuild->m_wDrawCount;
		packet.aBattleGuild[i].m_wCancelCount			=	lpGuild->m_wCancelCount;
		packet.aBattleGuild[i].m_wIncreaseVictoryPoint	=	iAddPoint;
		
		lpGuild->m_bf6LastGVGBattleYear		=	g_currentTime.m_wYear-2000;	//	마지막으로 길드전 한 연도
		lpGuild->m_bf4LastGVGBattleMonth	=	g_currentTime.m_wMonth;
		lpGuild->m_bf5LastGVGBattleDay		=	g_currentTime.m_wDay;

		((cGuildForWS *)lpGuild)->sendGuildBattleScore();

		SaveGuildBaseData(lpGuild);
		CGuildBattleInfo	*lpInfo	=	g_guildBattleManager.getBookedScheduleWithAll(_lpPacket->awGuildSerial[i]);
		if(lpInfo)
		{
			lpInfo->m_awBattleGuild[0]	=	0xffff;
			lpInfo->m_awBattleGuild[1]	=	0xffff;
		}
	}
	
	mSEND_GSP(_lpUser,packet);
}


void
operateSelectGuildMark(CClient *_lpUser,SERVERStoWORLD_SELECT_GUILD_MARK *_lpPacket)
{
	cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(_lpPacket->wGuildSerial);
	
	if (!lpGuild)
	{
		sendGSPValidGuild(_lpUser,_lpPacket->wGuildSerial);
		
		return;
	}
	
	memcpy(&lpGuild->m_guildMarkInfo,&_lpPacket->guildMarkInfo,sizeof(CGuildMarkInfo));
	
	SaveGuildMarkData(lpGuild);

	{
		WORLDtoSERVERS_SELECT_GUILD_MARK	packet;
		
		packet.base.set(sizeof(packet),dWORLDtoSERVERS_SELECT_GUILD_MARK);
		memcpy(&packet.guildMarkInfo,&_lpPacket->guildMarkInfo,sizeof(CGuildMarkInfo));
		packet.wGuildSerial				=	_lpPacket->wGuildSerial;
		packet.bf1IsUseGuildMarkPack	=	_lpPacket->bf1IsUseGuildMarkPack;
		
		mSEND_GSP(_lpUser,packet);
	}

	{
		SG_CHANGE_GUILD_MARK	packet;

		packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_MARK);
		packet.bf1IsUseGuildMarkPack	=	_lpPacket->bf1IsUseGuildMarkPack;
		packet.bf4Result				=	0;
		packet.wGuildSerial				=	_lpPacket->wGuildSerial;
		memcpy(&packet.guildMarkInfo,&_lpPacket->guildMarkInfo,sizeof(CGuildMarkInfo));

		SendGuildPacketToBCS(&packet,_lpPacket->wGuildSerial);
	}
}

void
SendGuildMemberUpdateInfoToBCS(cGuild *_lpGuild,CGuildMan *_lpMember)
{
	SG_UPDATE_GUILD_MEMBER	packet;

	packet.base.set(sizeof(packet),dSG_UPDATE_GUILD_MEMBER);

	packet.wGuildSerial			=	_lpGuild->m_wSerial;
	packet.bMemberCount			=	(BYTE)_lpGuild->m_wMemberCount;
	packet.bSubMasterCount		=	(BYTE)_lpGuild->m_wSubMasterCount;
	packet.bCongressManCount	=	(BYTE)_lpGuild->m_wCongressManCount;

	packet.m_bf11Level			=	_lpMember->m_wLevel;
	packet.m_bf4Rank			=	_lpMember->m_wRank;
	packet.m_bf8Job				=	(BYTE)_lpMember->m_wJob;
	packet.m_bf4ReservePet		=	(BYTE)_lpMember->m_bf3ReservePet;
	packet.m_bf1IsConnected		=	FALSE;
	strcpy(packet.strMemberName,_lpMember->m_strName);

	SendGuildPacketToBCS(&packet,_lpGuild->m_wSerial);
}

void
operateUpdateGuildMemberLevel(CClient *_lpUser,SERVERStoWORLD_UPDATE_GUILD_MEMBER_LEVEL *_lpPacket)
{
	cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(_lpPacket->wGuildSerial);
	
	if	(!lpGuild)
	{
		sendGSPValidGuild(_lpUser,_lpPacket->wGuildSerial);
		
		return;
	}
	
	CGuildMan	*lpMember	=	lpGuild->GetGuilderPnt(_lpPacket->strName);
	
	if	(!lpMember)
		return;
	
	lpMember->m_wLevel		=	_lpPacket->wLevel;

	{
		WORLDtoSERVERS_UPDATE_GUILD_MEMBER_LEVEL	packet;
		
		packet.base.set(sizeof(packet),dWORLDtoSERVERS_UPDATE_GUILD_MEMBER_LEVEL);
		packet.wGuildSerial	=	_lpPacket->wGuildSerial;
		packet.wLevel		=	_lpPacket->wLevel;
		strcpy(packet.strName,_lpPacket->strName);
		
		mSEND_GSP(_lpUser,packet);
	}

	SendGuildMemberUpdateInfoToBCS(lpGuild,lpMember);
}


void
forceSetTodayGuildBattleSchedule(CClient *_lpUser,int _iTimeOrder,int _iFieldOrder,int _iGuild1,int _iGuild2,BOOL _bIsSendData)
{
	CGuildBattleInfo		*lpInfo	=	&g_guildBattleManager.m_gbsToday.m_aBattleGuildSchedule[_iTimeOrder].m_aBattleGuildInfo[_iFieldOrder];
	
	lpInfo->m_awBattleGuild[0]	=	_iGuild1;
	lpInfo->m_awBattleGuild[1]	=	_iGuild2;
	
	if	(_bIsSendData)
		sendGSPGuildBattleInfo(NULL,_iTimeOrder,_iFieldOrder);
}

void
operateBanishUser(CClient *_lpUser,SERVERStoWORLD_BANISH_USER* _lpPacket)
{
	WORLDtoSERVERS_BANISH_USER	packet;
	
	memcpy(&packet,_lpPacket,sizeof(WORLDtoSERVERS_BANISH_USER));
	
	packet.base.set(sizeof(WORLDtoSERVERS_BANISH_USER),dWORLDtoSERVERS_BANISH_USER);
	
	Avatar_Data *lpAva			=	UserManager.GetGClientPnt(_lpPacket->strName,1);
	int			iFieldSerial	=	UserManager.GetClientOfGClientSerial(_lpPacket->strName,1);
	CClient		*lpServer		=	UserManager.GetClientPnt(iFieldSerial);
	
	if(!lpAva || lpAva->wSerial==0xffff || !lpServer || iFieldSerial==0xffff)
	{
		if(_lpUser)
		{
			WORLDtoSERVERS_BANISH_USER_RESULT	resultPacket;
			resultPacket.base.set(sizeof(resultPacket),dWORLDtoSERVERS_BANISH_USER_RESULT);
			
			strcpy(resultPacket.strName,_lpPacket->strName);
			_lpUser->AddSendPacket((char *)&resultPacket,resultPacket.base.wSize);
		}
		return;
	}
	
	lpServer->AddSendPacket((char *)&packet,packet.base.wSize);
	SendPacket(lpServer->GetSerial());
}

void
operateAddRenewItemExpireTime(CClient *_lpServer,SERVERStoWORLD_ADD_RENEW_ITEM_EXPIRE_TIME *_lpPacket)
{
	g_renewPremiumItemExpireTime.addRenewItemExpireTime(&_lpPacket->time,_lpPacket->dwRenewMinute,_lpPacket->wIsForOnlyYahoo,_lpPacket->wIsForOnlyRedgem);
	operateAskRenewItemExpireTimeList(_lpServer);
	sendRenewPremiumItemInfo(NULL,TRUE);
}

void
operateAskRenewItemExpireTimeList(CClient *_lpServer)
{
	WORLDtoSERVERS_RENEW_ITEM_EXPIRE_TIME_LIST	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_RENEW_ITEM_EXPIRE_TIME_LIST);
	packet.wCount		=	0;
	packet.wFullCount	=	g_renewPremiumItemExpireTime.m_iCount;
	packet.wIsLast		=	FALSE;
	
	for (int i=0;i<g_renewPremiumItemExpireTime.m_iCount;i++)
	{
		memcpy(&packet.aList[packet.wCount],&g_renewPremiumItemExpireTime.m_aList[i],sizeof(cRenewItemExpireTimeInfo));
		
		packet.wCount++;
		
		if (packet.wCount	>=	dRENEW_ITEM_EXPIRE_TIME_INFO_BUFFER_SIZE)
		{
			mSEND_GSP(_lpServer,packet);
			packet.wCount	=	0;
		}
	}
	
	packet.wIsLast		=	TRUE;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aList)+sizeof(cRenewItemExpireTimeInfo)*packet.wCount;
	mSEND_GSP(_lpServer,packet);
}



void
operateAppointmentQuizEvent(CClient *_lpUser,SERVERStoWORLD_APPOINTMENT_QUIZ_EVENT *_lpPacket)
{
	int	iGateVillage	=	0xffff;
	
	if	(_lpPacket->wType==	eET_OX_QUIZ)
		iGateVillage	=	g_eventManager.m_oxQuiz.m_wGateVillage;
	if	(_lpPacket->wType==	eET_WORD_QUIZ)
		iGateVillage	=	g_eventManager.m_wordQuiz.m_wGateVillage;
	
	g_eventManager.setQuizEventBeginTime(_lpPacket->wType,_lpPacket->wBeginYear,_lpPacket->wBeginMonth,_lpPacket->wBeginDay,_lpPacket->wBeginHour,_lpPacket->wBeginMinute,_lpPacket->wLimitPlayerCount,_lpPacket->wIsTest);
	
	WORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT	packet;
	ZeroMemory(&packet , sizeof(packet));
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT);
	
	packet.wBeginYear		=	_lpPacket->wBeginYear;
	packet.wBeginMonth		=	_lpPacket->wBeginMonth;
	packet.wBeginDay		=	_lpPacket->wBeginDay;
	packet.wBeginHour		=	_lpPacket->wBeginHour;
	packet.wBeginMinute		=	_lpPacket->wBeginMinute;
	packet.wIsTest			=	_lpPacket->wIsTest;
	packet.wGateVillage		=	iGateVillage;
	packet.wLimitPlayerCount=	_lpPacket->wLimitPlayerCount;
	packet.wType			=	_lpPacket->wType;
	strcpy(packet.strEventName,_lpPacket->strEventName);
	
	mSEND_GSP(_lpUser,packet);
}

void
sendWorldValueInfo(CClient *_user)
{
	WORLDtoSERVERS_WORLD_VALUE_INFO_ALL		packet;
	ZeroMemory(&packet , sizeof(packet));

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_UPDATE_WORLD_VALUE_INFO_ALL);

	memcpy(packet.aiWouldValue , g_aiWorldValue , sizeof(int) * dMAX_WORLD_VALUE_MAX);

	mSEND_GSP(_user,packet);
}

void
sendAppointmentQuizEventInfo(CClient *_lpUser)				//이름은 이렇지만 OX퀴즈 ,단어 이벤트 ,이구동성 이벤트 전부를 전송한다.
{// 마지막 수정일 : 09.10.09
	WORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT	packet;
	ZeroMemory(&packet , sizeof(packet));

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT);
	
	packet.wBeginYear	=	g_eventManager.m_oxQuiz.m_wYear;
	packet.wBeginMonth	=	g_eventManager.m_oxQuiz.m_wMonth;
	packet.wBeginDay	=	g_eventManager.m_oxQuiz.m_wDay;
	packet.wBeginHour	=	g_eventManager.m_oxQuiz.m_wHour;
	packet.wBeginMinute	=	g_eventManager.m_oxQuiz.m_wMinute;
	packet.wIsTest		=	g_eventManager.m_oxQuiz.m_wIsTest;
	packet.wLimitPlayerCount	=	g_eventManager.m_oxQuiz.m_wLimitPlayerCount;
	packet.wType		=	eET_OX_QUIZ;
	
	mSEND_GSP(_lpUser,packet);
	
	packet.wBeginYear	=	g_eventManager.m_wordQuiz.m_wYear;
	packet.wBeginMonth	=	g_eventManager.m_wordQuiz.m_wMonth;
	packet.wBeginDay	=	g_eventManager.m_wordQuiz.m_wDay;
	packet.wBeginHour	=	g_eventManager.m_wordQuiz.m_wHour;
	packet.wBeginMinute	=	g_eventManager.m_wordQuiz.m_wMinute;
	packet.wLimitPlayerCount	=	g_eventManager.m_wordQuiz.m_wLimitPlayerCount;
	packet.wIsTest		=	g_eventManager.m_wordQuiz.m_wIsTest;
	packet.wType		=	eET_WORD_QUIZ;
	strcpy(packet.strEventName , "");
	
	mSEND_GSP(_lpUser,packet);

	WORLDtoSERVER_EVENT_WORD_INFO	WordEventpacket;
	ZeroMemory(&WordEventpacket , sizeof(WordEventpacket));

	WordEventpacket.base.set(sizeof(WordEventpacket),dWORLDtoSERVERS_UPDATE_WORD_COMPARE);
	WordEventpacket.bIsActiveEvent		= FALSE;
	WordEventpacket.dwTimeInfo			= g_eventManager.m_wordCompareEvent.m_dwTimeMinite;

	
	WordEventpacket.bf16EventKind		= g_eventManager.m_wordCompareEvent.m_bf16EventKind;		// 09.10.09
	WordEventpacket.bf16BuffSerial		= g_eventManager.m_wordCompareEvent.m_bf16BuffSerial;// 09.10.09


	WordEventpacket.wBeginTimeYear		= g_eventManager.m_wordCompareEvent.m_wBeginTimeYear;
	WordEventpacket.wBeginTimeMonth		= g_eventManager.m_wordCompareEvent.m_wBeginTimeMonth;
	WordEventpacket.wBeginTimeDay		= g_eventManager.m_wordCompareEvent.m_wBeginTimeDay;
	WordEventpacket.wBeginTimeHour		= g_eventManager.m_wordCompareEvent.m_wBeginTimeHour;
	WordEventpacket.wBeginTimeMinite	= g_eventManager.m_wordCompareEvent.m_wBeginTimeMinite;
	WordEventpacket.wEndTimeYear		= g_eventManager.m_wordCompareEvent.m_wEndTimeYear;
	WordEventpacket.wEndTimeMonth		= g_eventManager.m_wordCompareEvent.m_wEndTimeMonth;
	WordEventpacket.wEndTimeDay			= g_eventManager.m_wordCompareEvent.m_wEndTimeDay;
	WordEventpacket.wEndTimeHour		= g_eventManager.m_wordCompareEvent.m_wEndTimeHour;
	WordEventpacket.wEndTimeMinite		= g_eventManager.m_wordCompareEvent.m_wEndTimeMinite;
	
	strcpy(WordEventpacket.strBeginMent	, g_eventManager.m_wordCompareEvent.m_strBeginComment);
	strcpy(WordEventpacket.strEndMent	, g_eventManager.m_wordCompareEvent.m_strEndComment);
	strcpy(WordEventpacket.strEventWord , g_eventManager.m_wordCompareEvent.m_strEventWord);
	strcpy(WordEventpacket.strActiveMent, g_eventManager.m_wordCompareEvent.m_strActiveComment);

	mSEND_GSP(_lpUser,WordEventpacket);
}


void
operateSetQuizEventVillage(CClient *_lpUser,SERVERStoWORLD_EVENT_QUIZ_GATE_VILLAGE *_lpPacket)
{
	g_eventManager.setOXQuizVillage(_lpPacket->wOXQuizVillage);
	g_eventManager.setWordQuizVillage(_lpPacket->wWordQuizVillage);
	
	//	operateAppointmentQuizEvent(NULL,NULL);
}

//	OX 퀴즈
void
operateOXQuizLoserReviveBattle(CClient *_lpUser)
{
	WORLDtoSERVERS_OX_QUIZ_LOSER_REVIVE_BATTLE	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_OX_QUIZ_LOSER_REVIVE_BATTLE);
	
	mSEND_GSP(_lpUser,packet);
}

void
operateProduceOxQuiz(CClient *_lpUser,SERVERStoWORLD_PRODUCE_OX_QUIZ *_lpPacket)
{
	WORLDtoSERVERS_PRODUCE_OX_QUIZ	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_PRODUCE_OX_QUIZ);
	
	strcpy(packet.strQuestion,_lpPacket->strQuestion);
	packet.wSolution	=	_lpPacket->wSolution;
	packet.wCount		=	_lpPacket->wCount;
	
	mSEND_GSP(_lpUser,packet);
}

//
//단어 비교 이벤트의 단어를 세팅하고 이벤트를 시작한다.
void			
operateChangeEventWord(CClient *_lpUser,SERVERStoWORLD_SET_EVENT_WORD *_lpPacket , BOOL _isStart)
{ // 마지막 수정일 : 09.10.09
	WORLDtoSERVER_EVENT_WORD_INFO	packet;

// 	g_eventManager.m_WordCompareEvent.m_bIsActiveWordCompareEvent = TRUE;
// 	strcpy(g_eventManager.m_WordCompareEvent.m_strEventWord , _lpPacket->strWordOrString);

	//페킷 세팅.
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_UPDATE_WORD_COMPARE);
	packet.bIsActiveEvent		= _isStart;
	packet.dwTimeInfo			= _lpPacket->dwTimeInfo;
	packet.bf16EventKind		= _lpPacket->bf16EventKind;
	packet.bf16BuffSerial		= _lpPacket->bf16BuffSerial;
	packet.wBeginTimeYear		= _lpPacket->wBeginTimeYear;
	packet.wBeginTimeMonth		= _lpPacket->wBeginTimeMonth;
	packet.wBeginTimeDay		= _lpPacket->wBeginTimeDay;
	packet.wBeginTimeHour		= _lpPacket->wBeginTimeHour;
	packet.wBeginTimeMinite		= _lpPacket->wBeginTimeMinite;
	packet.wEndTimeYear			= _lpPacket->wEndTimeYear;
	packet.wEndTimeMonth		= _lpPacket->wEndTimeMonth;
	packet.wEndTimeDay			= _lpPacket->wEndTimeDay;
	packet.wEndTimeHour			= _lpPacket->wEndTimeHour;
	packet.wEndTimeMinite		= _lpPacket->wEndTimeMinite;

	strcpy(packet.strBeginMent	, _lpPacket->strBeginMent);
	strcpy(packet.strEndMent	, _lpPacket->strEndMent);
	strcpy(packet.strEventWord , _lpPacket->strWordOrString);
	strcpy(packet.strActiveMent , _lpPacket->strActiveMent);

	//이벤트 메니져에 값 세팅.
	g_eventManager.m_wordCompareEvent.m_bIsActiveWordCompareEvent	= _isStart;
	g_eventManager.m_wordCompareEvent.m_bf16EventKind				= _lpPacket->bf16EventKind;
	g_eventManager.m_wordCompareEvent.m_bf16BuffSerial				= _lpPacket->bf16BuffSerial;
	g_eventManager.m_wordCompareEvent.m_dwTimeMinite				= _lpPacket->dwTimeInfo;

	g_eventManager.m_wordCompareEvent.m_wBeginTimeYear				= _lpPacket->wBeginTimeYear;
	g_eventManager.m_wordCompareEvent.m_wBeginTimeMonth				= _lpPacket->wBeginTimeMonth;
	g_eventManager.m_wordCompareEvent.m_wBeginTimeDay				= _lpPacket->wBeginTimeDay;
	g_eventManager.m_wordCompareEvent.m_wBeginTimeHour				= _lpPacket->wBeginTimeHour;
	g_eventManager.m_wordCompareEvent.m_wBeginTimeMinite			= _lpPacket->wBeginTimeMinite;
	g_eventManager.m_wordCompareEvent.m_wEndTimeYear				= _lpPacket->wEndTimeYear;
	g_eventManager.m_wordCompareEvent.m_wEndTimeMonth				= _lpPacket->wEndTimeMonth;
	g_eventManager.m_wordCompareEvent.m_wEndTimeDay					= _lpPacket->wEndTimeDay;
	g_eventManager.m_wordCompareEvent.m_wEndTimeHour				= _lpPacket->wEndTimeHour;
	g_eventManager.m_wordCompareEvent.m_wEndTimeMinite				= _lpPacket->wEndTimeMinite;

	strcpy(g_eventManager.m_wordCompareEvent.m_strBeginComment	, _lpPacket->strBeginMent);
	strcpy(g_eventManager.m_wordCompareEvent.m_strEndComment	, _lpPacket->strEndMent);
	strcpy(g_eventManager.m_wordCompareEvent.m_strEventWord		, _lpPacket->strWordOrString);
	strcpy(g_eventManager.m_wordCompareEvent.m_strActiveComment	, _lpPacket->strActiveMent);

	g_eventManager.m_wordCompareEvent.save();

	SendDataAllGameServer((char *)&packet,packet.base.wSize);
}


void
operateProduceWordQuiz(CClient *_lpUser,SERVERStoWORLD_PRODUCE_WORD_QUIZ *_lpPacket)
{
	WORLDtoSERVERS_PRODUCE_WORD_QUIZ	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_PRODUCE_WORD_QUIZ);
	
	strcpy(packet.strQuestion,_lpPacket->strQuestion);
	strcpy(packet.strSolution,_lpPacket->strSolution);
	packet.wCount		=	_lpPacket->wCount;
	
	mSEND_GSP(_lpUser,packet);
}

void
operateChangeWordQuizRound(CClient *_lpUser,SERVERStoWORLD_CHANGE_WORD_QUIZ_ROUND *_lpPacket)
{
	WORLDtoSERVERS_CHANGE_WORD_QUIZ_ROUND	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_CHANGE_WORD_QUIZ_ROUND);
	
	mSEND_GSP(_lpUser,packet);
}

void
operateEndQuizEvent(CClient *_lpUser,SERVERStoWORLD_END_QUIZ_EVENT *_lpPacket)
{
	WORLDtoSERVERS_END_QUIZ_EVENT	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_END_QUIZ_EVENT);
	packet.wType	=	_lpPacket->wType;
	
	mSEND_GSP(_lpUser,packet);
}

void
operateCancelQuizEvent(CClient *_lpUser,SERVERStoWORLD_CANCEL_QUIZ_EVENT *_lpPacket)
{
	WORLDtoSERVERS_CANCEL_QUIZ_EVENT	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_CANCEL_QUIZ_EVENT);
	packet.wType	=	_lpPacket->wType;
	
	mSEND_GSP(_lpUser,packet);
}


void
operateIsDenyCommunityTargetPacket(SERVERStoWORLD_IS_DENY_COMMUNITY_TARGET *_lpPacket)
{
	Avatar_Data *lpAvatar		=	UserManager.GetGClientPnt(_lpPacket->strSenderName,1);
	WORD		iFieldSerial	=	UserManager.GetClientOfGClientSerial(_lpPacket->strSenderName,1);
	CClient		*lpClient		=	UserManager.GetClientPnt(iFieldSerial);
	
	if (!lpAvatar || !lpClient || iFieldSerial==0xffff)
		return;
	
	WORLDtoSERVERS_IS_DENY_COMMUNITY_TARGET	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_IS_DENY_COMMUNITY_TARGET);
	
	strcpy(packet.strSenderName,_lpPacket->strSenderName);
	strcpy(packet.strReceiverName,_lpPacket->strReceiverName);
	
	mSEND_GSP(lpClient,packet);
}


void	operateMakeGuildLevelPacket(SERVERStoWORLD_MAKE_GUILD_LEVEL *_lpPacket)
{
	WORLDtoSERVERS_GUILD_UPDATE_EXP	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_GUILD_UPDATE_EXP);
	
	cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(_lpPacket->wGuildSerial);
	
	if (!lpGuild)
		return;
	
	lpGuild->makeLevel(_lpPacket->wLevel);
	
	//	_logf(" GuildExp : [%s] lev[%d] exp[%d]",lpGuild->m_strName, lpGuild->m_wLevel, lpGuild->m_dwExp);
	
	packet.aExpData[0].m_dwUpdateExp	=	lpGuild->m_dwExp;
	packet.aExpData[0].m_wGuildSerial	=	lpGuild->m_wSerial;
	packet.aExpData[0].m_wLevel			=	lpGuild->m_wLevel;
	packet.aExpData[0].m_wGuildPoint	=	lpGuild->m_wGuildPoint;
	packet.aExpData[0].m_wExpTaxRate	=	lpGuild->m_wExpTaxRate;
	packet.wCount						=	1;
	packet.base.wSize					=	sizeof(packet)-sizeof(packet.aExpData)+sizeof(CGuildExpInfoIncludeLevel)*packet.wCount;
	
	SendDataAllGameServer((char *)&packet,packet.base.wSize);
}

void CommandThread(LPVOID lpParameter)
{
	cInputConsoleCommand	inputCommand;

	int iReallyQuit = 0;
	
	while(g_bThreadLoop)
	{
		Sleep(1);

		g_lifeCodeMgr.endCheck(eLIFE_CODE_COMMAND);

		char	*lpstrCommand	=	inputCommand.getCommand(TRUE);

		g_lifeCodeMgr.beginCheck(eLIFE_CODE_COMMAND);

		if	(!lpstrCommand)
			continue;

		if	(iReallyQuit && STRICMP(lpstrCommand,"y")==0)
		{
			g_bIsLifeCodeLoop	=	FALSE;

			WriteServerStatusToRegistry(eST_WORLD,eSERVER_STATUS_QUIT);

			Sleep(100);	//	라이프 사인 쓰레드 때문에..

			//	종료 라이프 사인 보내기
			SendLPLifeCode(eEndLifeSign,UserManager.GetClientPnt(dLOGIN_SERVER_ID),l_strWorldIp);

			for(int i=0;i<4;i++)
			{
				CClient * loginusr = UserManager.GetClientPnt(dLOGIN_SERVER_ID);

				if(loginusr)
				{
#ifdef _FOR_KOREA
					_log("서버 종료를 위해 로그인 서버를 자른다.");
#endif
					DisUser(loginusr->GetSerial(),"For destroy Server!");
				}else
					break;
			}
			
			CClient * adminusr = UserManager.GetClientPnt(dADMIN_TOOL_ID);
			
			if	(adminusr)
			{
#ifdef _FOR_KOREA
				_log("서버 종료를 위해 어드민 서버를 자른다.");
#endif
				DisUser(adminusr->GetSerial(),"For destroy Server!");
			}

			g_bOffServer = TRUE;

			break;
		}
		else
			iReallyQuit	=	FALSE;

		if	(STRICMP(lpstrCommand,"q")==0)
		{
			printf(" Quit? (Y/N) ...");

			iReallyQuit = 1;

			continue;
		}

		if	(STRICMP(lpstrCommand,"help")==0)
		{
			printf("===============================\n");
			printf(" Command list \n\n");
			printf(" [guildlist] : Print the current guild list in file\n");
			printf(" [gameserverlist] : Print the current gameserver list on screen and in log file\n");
			printf(" [debuginfo] : Print debugging info in log file\n");
			printf(" [q] : Close this server\n");
			printf("===============================\n");

			continue;
		}

		if	(STRICMP(lpstrCommand,"debuginfo")==0)
		{
			_log("======================================\n");
			_log(" Debug Info \n");
			_log(" g_iWorldServerIndex = %d, g_iWorldType = %d \n",g_iWorldServerIndex,g_iWorldType);
			_log(" isFirst = %d , isReady = %d , wServerCheckType = %d\n",isFirst,isReady,wServerCheckType);
			_log(" isMainLoop = %d , g_bThreadLoop = %d , g_bOffServer = %d\n",isMainLoop,g_bThreadLoop,g_bOffServer);
			_log(" wWeekFixCheck = %d, wHourFixCheck = %d, wMinFixCheck = %d, wWeekTestFixCheck = %d\n",wWeekFixCheck,wHourFixCheck,wMinFixCheck,wWeekTestFixCheck);
			_log(" wWeekFixEndCheck = %d, wHourFixEndCheck = %d, wMinFixEndCheck = %d\n",wWeekFixEndCheck ,wHourFixEndCheck ,wMinFixEndCheck );

			for(int z = 0;z<dWORLD_MAX_USER_COUNT;z++)
			{
				CClient * user = UserManager.GetClientPnt(z,FALSE);
				
				if(!user || user->GetSerial()==0xffff || user->GetIsClose())
					continue;
				_log(" CLIENT : name [%s,%s] isClose %d , Socket %d\n",user->GetID(),user->GetAddr(),user->GetIsClose(),user->GetSocket());
			}
			_log("======================================\n");
			continue;
		}

		if	(STRICMP(lpstrCommand,"gameserverlist")==0)
		{
			_log("GAME SERVER LIST... ");
			_log("==============================================");
			for(int z = 0;z<dWORLD_MAX_USER_COUNT;z++)
			{
				CClient * user = UserManager.GetClientPnt(z,FALSE);
				
				if(!user || user->GetSerial()==0xffff || user->GetIsClose())
					continue;
				_log(" [%s(%s) - mapcount %d",user->GetID(),user->GetAddr(),user->wMapCount);
			}
			_log("==============================================");

			continue;
		}

		if	(STRICMP(lpstrCommand,"gi")==0)
		{
			printf("gold rush info\n");
			printf("  begin time : %d/%d/%d %d:%d~%d/%d/%d %d:%d\n",g_goldRush.m_begin.year,g_goldRush.m_begin.month,g_goldRush.m_begin.day,g_goldRush.m_begin.hour,g_goldRush.m_begin.minute,
				g_goldRush.m_end.year,g_goldRush.m_end.month,g_goldRush.m_end.day,g_goldRush.m_end.hour,g_goldRush.m_end.minute);
			printf("  begin message : %s\n",g_goldRush.m_strBeginComment);
			printf("  end message : %s\n",g_goldRush.m_strEndComment);
			printf("  exp : %d%%\n",g_goldRush.m_wBoostExperience);
			printf("  item drop rate : %d%%\n\n",g_goldRush.m_wCorrectItemDropChance);

			continue;
		}

		if	(STRICMP(lpstrCommand,"rti")==0)	//	reload tournament info
		{
			reloadTournamentInfo(TRUE);

			continue;
		}

		if	(STRICMP(lpstrCommand,"rgbs")==0)	//	reset guild battle schedule
		{
			g_guildBattleManager.resetTodaySchedule();
			sendGSPGuildBattleSchedule(NULL);

			printf("reset guild battle schedule\n");

			continue;
		}


		if	(g_iWorldType	==	2)
		{
			if	(STRICMP(lpstrCommand,"ready_tgb")==0)
			{
				SendNoticeBeginSiegeWarfare(NULL,0);	//	공성전 준비
				continue;
			}

			if	(STRICMP(lpstrCommand,"begin_tgb")==0)
			{
				SendBeginSiegeWarfare(NULL,0);	//	공성전 시작
				continue;
			}

			if	(STRICMP(lpstrCommand,"end_tgb")==0)
			{
				SendEndSiegeWarfare(NULL,0);		//	공성전 종료
				continue;
			}

			if	(STRICMP(lpstrCommand,"ready_end_tgb")==0)
			{
				SendNoticeEndSiegeWarfare(NULL,0);	//	공성전 종료 x분전
				continue;
			}
		}
	
	}

	hCommandThread	=	NULL;

	g_lifeCodeMgr.endCheck(eLIFE_CODE_COMMAND);

	return;
}


void	OperateDisconnectGameServer(CClient *_lpServer,int _iCause)
{
	int	aField[1000];
	
	for (int i=0;i<_lpServer->wMapCount;i++)
		aField[i]	=	_lpServer->cGSInfo[i].wIndex;
	
	g_partyManager.disconnectField(aField,_lpServer->wMapCount);
	
	WORLDtoSERVERS_DISCONNECT_SERVER	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_DISCONNECT_SERVER);
	
	if	(_lpServer->m_wServerIndex==0xffff)
	{
		_log("ERROR OperateDisconnectGameServer  _lpServer->m_wServerIndex = %d",_lpServer->m_wServerIndex);
		return;
	}
	packet.wServerIndex	=	_lpServer->m_wServerIndex;
	packet.wCause		=	_iCause;
	strcpy(packet.strWorldName,strWorldName);
	
	SendDataAllGameServer((char *)&packet,packet.base.wSize);
}

void
CheckGuildCheckSum(CClient *_lpServer,SERVERStoWORLD_CHECK_GUILD_CHECKSUM *_lpPacket)
{
//	_logf("check guild count %d\n",_lpPacket->wGuildCount);

	for (int i=0;i<_lpPacket->wGuildCount;i++)
	{
//		_logf("check guild serial %d\n",_lpPacket->awCheckGuild[i]);
		cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(_lpPacket->awCheckGuild[i]);
		
		if (!lpGuild)
		{
			if (_lpPacket->aiCheckSum[i])
			{
				sendGSPValidGuild(_lpServer,_lpPacket->awCheckGuild[i]);
				_logf("ERROR CheckGuildCheckSum - valid guild data");
			}
			continue;
		}

		if (_lpPacket->aiCheckSum[i]	!=	lpGuild->getCheckSum())
		{
			sendGSPUpdateGuildInfo(_lpServer,lpGuild->wSerial,TRUE);
			sendGSPUpdateGuildMemberInfo(_lpServer,lpGuild->wSerial);
			
			_logf("ERROR CheckGuildCheckSum - mismatch guild data '%s'",lpGuild->m_strName);
		}
	}
	
	if	(_lpPacket->dwTodayGBS_CheckSum		!=	g_guildBattleManager.m_gbsToday.getCheckSum())
		sendGSPGuildBattleSchedule(_lpServer);

	if	(_lpPacket->dwGuildHallCheckSum		!=	g_guildManager.getGuildHallCheckSum())
		SendUpdateGuildHallInfo(_lpServer);
}

void
sendCarrotShopInfo(CClient *_lpServer)
{
	WORLDtoSERVERS_CARROT_SHOP_INFO	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_CARROT_SHOP_INFO);
	
	packet.wCount	=	g_carrotShopInfo.m_wCategoryCount;
	packet.wIndex	=	0xffff;
	packet.wStatus	=	0;
	
	for (int i=0;i<g_carrotShopInfo.m_wCategoryCount;i++)
	{
		printf("send carrot shop info #%.3d\n",i);

		packet.wIndex		=	i;

		cCarrotShopCategoryInfoDefine	*lpData				=	&g_carrotShopInfo.m_aData[i];
		char*							lpCategoryInfoAddr	=	((char*)lpData)+sizeof(cCarrotShopCategoryInfo1);

		packet.base.wSize	=	sizeof(packet)-sizeof(packet.data.m_aItemList)+sizeof(cCarrotShopItemDefine)*lpData->m_wItemCount;

		memcpy(&packet.data,lpData,sizeof(cCarrotShopCategoryInfo1));
		memcpy(&packet.categoryInfo,lpCategoryInfoAddr,sizeof(cCarrotShopCategoryInfo2));

// 		packet.categoryInfo.m_bf1IsTestCategory		=	1;
// 		packet.categoryInfo.m_bf1IsRecommandCategory=	1;
// 		packet.categoryInfo.m_awSpareBuffer[0]		=	37;

		mSEND_GSP(_lpServer,packet);

		packet.wStatus		=	1;

		Sleep(10);
	}
	
	packet.wStatus		=	2;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.data)-sizeof(packet.categoryInfo);

	mSEND_GSP(_lpServer,packet);
}

void
sendGRInfo(CClient *_lpServer)
{
	WORLDtoSERVERS_GR_INFO	packet;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_GR_INFO);
	
	memcpy(&packet.begin,&g_goldRush.m_begin,sizeof(UTime));
	memcpy(&packet.end,&g_goldRush.m_end,sizeof(UTime));
	
	packet.wDropChance	=	g_goldRush.m_wCorrectItemDropChance;
	packet.wBoostExp	=	g_goldRush.m_wBoostExperience;
	
	strncpy(packet.strBeginComment,g_goldRush.m_strBeginComment,sizeof(packet.strBeginComment)-2);
	strncpy(packet.strEndComment,g_goldRush.m_strEndComment,sizeof(packet.strBeginComment)-2);
	
	mSEND_GSP(_lpServer,packet);
}

void
sendItemPackList(CClient *_lpServer,int _iPackSerial)
{
	WORLDtoSERVERS_ITEM_PACK_INFO			packet;
	WORLDtoSERVERS_ITEM_PACK_LIST_SERIAL	packetList;
	
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_ITEM_PACK_INFO);
	
	if	(!_lpServer)
		return;

	int	iBeginPack	=	0,iEndPack=dMAX_ITEM_PACK_COUNT;

	if	(_iPackSerial	!=	0xffff)
	{
		iBeginPack		=	_iPackSerial;
		iEndPack		=	_iPackSerial+1;
	}

	packetList.base.set(sizeof(packetList),dWORLDtoSERVERS_ITEM_PACK_LIST_SERIAL);
	packetList.wCount	=	0;

	for (int i=iBeginPack;i<iEndPack;i++)
	{
		cItemPackInfo	*lpInfo	=	&g_itemPackManager.m_aPacks[i];

		if	(lpInfo->m_wCount	==	0)
			continue;

		printf("send item pack info [%d] to game server\n",i);
		
		memcpy(&packet.info,lpInfo,sizeof(cItemPackInfo));

		packet.base.wSize	=	sizeof(packet) - sizeof(packet.info.m_aItems) + sizeof(cItemInPack)*packet.info.m_wCount;
		packetList.awList[packetList.wCount++]	=	packet.info.m_wSerial;

		CM_COMPRESS		comp;

		WORD	wCompSize	=	cSRVUTIL::Compress((unsigned char *)&packet,(unsigned char *)comp.data,packet.base.wSize);
		comp.wSourceSize	=	packet.base.wSize;
		
		comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);

		if	(wCompSize ==0)
		{
			mSEND_GSP(_lpServer,packet);
		}
		else
		{
			_lpServer->AddSendPacket((char *)&comp,comp.wSize);
		}

		Sleep(1);
	}

	packetList.base.wSize	=	sizeof(packetList)-sizeof(packetList.awList)+2*packetList.wCount;

	_lpServer->AddSendPacket((char *)&packetList,packetList.base.wSize);
}

void
operateBoostPowerByEvent(CClient *_lpServer,SERVERStoWORLD_BOOST_POWER_BY_EVENT* _lpPacket)
{
	switch(_lpPacket->wWork)
	{
		case	eBPBE_TM_GOLD	:	//	보물 지도 이벤트를 인한 골드 4배
			g_iBoostGoldByTreasureMapEventCount++;
			break;
		case	eBPBE_TM_EXP	:	//	보물 지도 이벤트로 인한 경험치 2배
			g_iBoostExpByTreasureMapEventCount++;
			break;
	}

	WORLDtoSERVERS_BOOST_POWER_BY_EVENT	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_BOOST_POWER_BY_EVENT);
	packet.wWork							=	_lpPacket->wWork;
	packet.wBoostGoldByTreasureMapEventCount=	g_iBoostGoldByTreasureMapEventCount;
	packet.wBoostExpByTreasureMapEventCount	=	g_iBoostExpByTreasureMapEventCount;

	strcpy(packet.strName,_lpPacket->strName);

	mSEND_GSP(_lpServer,packet);
}

void
operateUpdateCarrotShopInfo(CClient *_lpServer,UW_ASK_CARROTSHOPINFO_RESULT* _lpPacket)
{	//수정 : 이지현 / 20060111 ; USER DB에서 업데이트 된 내용을 메모리에 저장하고, 모든 게임서버에게 보낸다.

	printf("receive carrot shop info\n");

	g_carrotShopInfo.updateInfo(&_lpPacket->cShopInfo);

	if	(_lpPacket->wIsUpdateNow)
		sendCarrotShopInfo(NULL);
}

void
operateUpdateItemPackInfo(CClient *_lpServer,DWMSG_ITEMPACK_RESULT* _lpPacket)
{
	printf("receive item pack info [%d]\n",_lpPacket->wSerial);
	g_itemPackManager.modifyData(_lpPacket->wSerial,&_lpPacket->info);
}

void
operateItemPackInfoCheckSum(CClient *_lpServer,SERVERStoWORLD_ITEM_PACK_INFO_CHECKSUM* _lpPacket)
{
	if (g_itemPackManager.getCheckSum()	!=	_lpPacket->dwCheckSum)
	{
		g_itemPackManager.reset();

		WORLDtoSERVERS_ASK_ALL_ITEM_PACK_INFO	packet;

		packet.base.set(sizeof(packet),dWORLDtoSERVERS_ASK_ALL_ITEM_PACK_INFO);

		mSEND_GSP(_lpServer,packet);
	}
}

void
operateRemoveFriendInTargetList(char *_lpstrClientName,char *_lpstrTargetName)
{
	WORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST);

	strcpy(packet.strClientName,_lpstrClientName);
	strcpy(packet.strTargetName,_lpstrTargetName);

	CClient	*lpServer	=	UserManager.GetClientOfGClientPnt(packet.strTargetName,1);	//	한 유저가 속한 게임서버

	if (!lpServer)
	{
		operateRemoveFriendInTargetListResult(_lpstrClientName,_lpstrTargetName,eRFITLR_NOT_CONNECTED);
		return;
	}

	mSEND_GSP(lpServer,packet);
}

void
operateRemoveFriendInTargetListResult(char *_lpstrClientName,char *_lpstrTargetName,int _iResult)
{
	WORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST_RESULT	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST_RESULT);

	strcpy(packet.strClientName,_lpstrClientName);
	strcpy(packet.strTargetName,_lpstrTargetName);
	packet.wResult	=	_iResult;

	CClient	*lpServer	=	UserManager.GetClientOfGClientPnt(packet.strClientName,1);	//	한 유저가 속한 게임서버

	if (!lpServer)
		return;

	mSEND_GSP(lpServer,packet);
}
