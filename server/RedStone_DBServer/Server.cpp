// Server.cpp : Defines the entry point for the application.
//
/**********************************************************************************
	DB Cache 憮 
**********************************************************************************/
#include <winsock2.h>
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <process.h>
#include <iostream>
#include <io.h>
#include <time.h>

#include "cSOCKET.h"
#include "Client.h"
#include "cScript.h"
#include "cSRVUTIL.h"
#include "cPACKET_DBSERVER.h"
#include "cPACKET_WORLDSERVER.h"
#include "cPACKET_LOGSERVER.h"
#include "cPLAYER_DATA.h"
#include "SFC.h"
#include "CSaveData.h"
#include "cADO_WEB.h"
#include "cADO_STATISTIC.h"
#include "CPlayerBuff.h"
#include "Hardwareinfo.h"
#include "cDECODER.h"
#include "life_code.h"
#include "reg_common.h"
#include "inputConsoleCommand.h"
#include "operatePacket.h"
#ifdef	_FOR_USA
#include "../OGP_Lib/ogpapi.h"
#endif
#include "../buildVersion.h"	//	檜勒 奢嶸 ж堅 .
							//	橈賊 だ擊 и.(奢嶸 ж雖 匙!!)
							//	朝
							//	#pragma	once
							//	#define	dBUILDER_NAME	"young"
							//	諦 檜 摹樹 腎橫 氈. 菴 檜擎 Щ煎斜楚該 檜擊 瞳擊匙
int					g_bIsMainTestServer	=	FALSE;	//	獄憮嫌 撲薑檜 僅擎 撥. 營 賃陛雖  瞳檣匙虜 蝶お.

#ifdef _FOR_THAI
#undef dUSE_SERVERSTATUS_REPORT
#endif

#ifdef dUSE_SERVERSTATUS_REPORT
	#include "cServerStatus.h"
#endif

int		g_isUsePlayerBuffer;	//	global variable - in CPlayerBuff.h
int		g_iServerType	=	-1;	//	憮 顫殮

//	頂睡 蝶お 嬪и 
BOOL	l_bIsTestDB		=	FALSE;

#define _PACKET_OPTION_CRYPT		//	倆 ぬ擊 陳.
#define _PACKET_OPTION_SEND_AT_ONE	//	ч翕 等檜顫 и廓 爾魚.
#define dDB_CONNECT_TIME			100000
#define dCONFIGFILENAME				"config-avatardb.txt"
#define MSG(a,b)					MessageBox(NULL,a,b,MB_OK);
#define dIS_DB_SERVER

#define	dMAX_BUFFER			4096

static char savepath[512];
static char listpath[512];
static char bankpath[512];
static char	guildpath[512];

CRITICAL_SECTION csRECVPacket;
CRITICAL_SECTION csSENDPacket;
CRITICAL_SECTION csRemoveUser;

static BOOL		isFirst	=TRUE;
static BOOL		isReady =FALSE;
static BOOL		g_bThreadLoop = TRUE;
static char		strAvatarDBIp[dIP_SIZE];
static char		strAvatarDBId[64];
static char		strAvatarDBPw[64];
static char		strAvatarDBSource[128];

static char		strWebDBIp[dIP_SIZE];
static char		strWebDBId[64];
static char		strWebDBPw[64];
static char		strWebDBSource[128];

#ifdef _FOR_THAI
static char		strThaiPayLetterServerIP[dIP_SIZE];
static int		strThaiPayLetterServerPort;
#endif

char			l_strServerIp[64]	=	"";

CSharedMemory	g_sharedMemory;

HANDLE			hEventMngThread,hAcceptThread,hAdditionThread,hWorkerThread[dMAX_WORKER_COUNT];
#ifdef _FOR_THAI
HANDLE			hPayLetterThread;
#endif

HANDLE			l_hCommandThread,l_hLifeCodeThread;
HANDLE			l_hAcceptThread			=	NULL;

void			CommandThread(LPVOID lpParameter);
void			LifeCodeUpdateThread(LPVOID lpParameter);

HANDLE			g_hCompletionPort;	//	IOCP 餌辨腎朝 んお
SOCKET			lisSock;			//	listen 模鰍
cADO_AVATAR		g_adoAvatar;

cADO_WEB		cWeb;
#ifndef _FOR_THAI
cADO_CARROT		cCarrot;
#endif

cLOG_QUE		g_LogQue;
cSOCKET			g_socketLog;
#ifdef _FOR_THAI
cSOCKET			g_socketPayLetter;
#endif

BOOL				l_bIsForcedSettingGoldRushEvent	=	FALSE;
cGoldRushDefine		g_cGoldRush_Event;
cItemPackInfo		g_cItemPack_Event[dMAX_ITEM_PACK_COUNT];

char			mainPath[1024];
static char		strLOGIp[dIP_SIZE];
static DWORD	dwLOGPort;
BOOL			l_bIsTestServer	=	FALSE;
WORD			wServerType;		//	0 - 憮綠蝶 憮, 1 - 蝶お 憮
int				g_iSaveGap;			//	檜 蝦衙 憮 п.
int				nTimer;
int				l_iWorldServerSerial	=	0;	//	錯萄 憮 衛葬橡
cScript			scp;
cHardwareInfo	hwInfo;

CClientManager & UserManager  = CClientManager::GetInstance();

BOOL	InitSocket();
BOOL	InitCompletionPort();		//	IOCP 蟾晦
void	WorkerThread(LPVOID lpParameter); 
void	AdditionThread(LPVOID lpParameter); 
void	EventMngThread(LPVOID lpParameter); 
#ifdef _FOR_THAI
void	PayLetterThread(LPVOID lpParameter); 
#endif
void	AcceptThread(LPVOID lpParameter);

BOOL	RecvPacket(int serial);
BOOL	SendPacket(int serial);
void	SendPacket_Add(int serial);

void	ProcessRecvPacket(int serial);

//	method for packet send
void	DisUser(int serial);
void	UpdateSaveUser(CClient *user,LDMSG_UPDATESAVE_USER *cUsUser);
void	UpdateSaveAll(CClient *user,LDMSG_UPDATESAVE *cUpdateSave);
//	end define


void	sendLogDeleteGuild(char* _strGuildName, char* _strRequestName, WORD _wReason);
void	sendLogChangeGuildRank(char* _strGuildName, char* _strMasterName, char* _strTargetName, WORD _wTargetRank, WORD _wMasterRank, WORD _wReason);


//////////////////////////////////////
//	晦顫 晦棟擊 嬪и Ы楚斜 
BOOL	isSend_UserCount	=	FALSE;

DebugTracer			* cLOG;
//CTracerFromSaveLog	* _SLOG;

void	CheckString(char * _str,int _size, char * _where,int _line)
{
	int i;

	for(i=0; i<_size;i++)
	{
		if(_str[i]==NULL)
			break;
	}

	if	(i==_size)
	{
		char * tmpStr = new char[_size+1];
		memcpy(tmpStr,_str,_size);
		tmpStr[_size] = 0;
		_log("Error String : where[%s,%d] value[%s]",_where,_line,tmpStr);
		delete [] tmpStr;
		tmpStr = NULL;
	}
}

int main()
{
	{
		char	strTitle[512];

#ifdef	_FOR_KOREA
		sprintf(strTitle,"RS Avatar DB server for korea. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_JAPAN
		sprintf(strTitle,"RS Avatar DB server for japan. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_THAI
		sprintf(strTitle,"RS Avatar DB server for thai. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_USA
		sprintf(strTitle,"RS Avatar DB server for USA. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#else
		sprintf(strTitle,"RS Avatar DB server. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#endif

		SetConsoleTitle(strTitle);
	}

	WriteServerInfoToRegistry(eST_AVATAR_DBC,"Avatar DBC Server");
	WriteServerStatusToRegistry(eST_AVATAR_DBC,eSERVER_STATUS_BOOTING);

	InitLifeCode();

	printf("CGuildSaveInfoForBinary  =  %d ,sizeof(CGuildSaveInfoMark) =%d ,CGuildSaveInfoBank=%d , CGuildSaveInfoRelShip=%d\n",sizeof(CGuildSaveInfoForBinary),sizeof(CGuildSaveInfoMark),sizeof(CGuildSaveInfoBank), sizeof(CGuildSaveInfoRelShip));
	printf("DWMSG_GETGUILD = %d,%d \n",sizeof(GuildListDB),sizeof(DWMSG_GETGUILD));
	printf("PLAYER DATA = %d,%d,%d \n",sizeof(cPLAYER_DATA),sizeof(cUserCustomDataDefine),sizeof(CBankInfo));

	{
		cDECODER	decoder;
		
		if	(decoder.Upload("goldRush.txt",NULL))
		{
			l_bIsForcedSettingGoldRushEvent	=	TRUE;
			
			decoder.GetWord();
			g_cGoldRush_Event.m_begin.year	=	decoder.GetNumber()-2000;
			g_cGoldRush_Event.m_begin.month	=	decoder.GetNumber();
			g_cGoldRush_Event.m_begin.day	=	decoder.GetNumber();
			g_cGoldRush_Event.m_begin.hour	=	decoder.GetNumber();
			g_cGoldRush_Event.m_begin.minute=	decoder.GetNumber();
			g_cGoldRush_Event.m_wBoostExperience		=	decoder.GetNumber();
			g_cGoldRush_Event.m_wCorrectItemDropChance	=	decoder.GetNumber();
			
			char	*lpstrBeginComment	=	decoder.GetStream();
			strncpy(g_cGoldRush_Event.m_strBeginComment,lpstrBeginComment,254);
			
			g_cGoldRush_Event.m_end.year		=	decoder.GetNumber()-2000;
			g_cGoldRush_Event.m_end.month		=	decoder.GetNumber();
			g_cGoldRush_Event.m_end.day			=	decoder.GetNumber();
			g_cGoldRush_Event.m_end.hour		=	decoder.GetNumber();
			g_cGoldRush_Event.m_end.minute		=	decoder.GetNumber();
			
			char	*lpstrEndComment	=	decoder.GetStream();
			strncpy(g_cGoldRush_Event.m_strEndComment,lpstrEndComment,254);
			
			cMSG::Put	("please check gold rush event info","time %d/%d/%d %d:%d ~ %d/%d/%d %d:%d\n\nbegin comment : %s\nend comment : %s\n\nboost exp : %d%%\nboost item drop chance : %d%%",
				g_cGoldRush_Event.m_begin.year+2000	,g_cGoldRush_Event.m_begin.month	,g_cGoldRush_Event.m_begin.day	,g_cGoldRush_Event.m_begin.hour	,g_cGoldRush_Event.m_begin.minute,
				g_cGoldRush_Event.m_end.year+2000	,g_cGoldRush_Event.m_end.month	,g_cGoldRush_Event.m_end.day		,g_cGoldRush_Event.m_end.hour	,g_cGoldRush_Event.m_end.minute	,
				g_cGoldRush_Event.m_strBeginComment	,
				g_cGoldRush_Event.m_strEndComment	,
				g_cGoldRush_Event.m_wBoostExperience,g_cGoldRush_Event.m_wCorrectItemDropChance);
		}
	}

////////////////////
//	措 TEST  
/*	strcpy(strThaiPayLetterServerIP,"203.238.151.253");
	strThaiPayLetterServerPort = 36011;

	SOCKADDR_IN serverAddress;
	WSADATA		wsadata;
	if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
	{	_log("Fail WSAStartup !! \n");
		g_adoAvatar.Close();
		return FALSE;
	}

	if(!g_socketPayLetter.init(strThaiPayLetterServerPort,strThaiPayLetterServerIP,"payletter server",4))
	{
		_log(" Failed connecting PayLetter Server !!");
		return FALSE;
	}
	//	擎 議蓮高 橫晦
	{
		char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
		memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);
		WORD	type=0,recvsize=0;
		//	test
		{
			BILL_PACK		sPacket;
			memset(&sPacket,0,sizeof(BILL_PACK));
			sPacket.base.set( sizeof(BILL_PACK), dREQ_GETCASHPOINT);	//	ぬ擊 嫡擊陽 顫殮薑爾 渠и 薯и 餌о 挫 匙!!
			sPacket.dwReqKey = 314000001;
			strcpy(sPacket.szUserID , "lnklogic");
			sPacket.dwClientIP = 38;	//	п渡 憮憮曖 檜樹お 衛葬橡
			sPacket.wClientPort = 39;		//	dwClientIP п渡 憮曖 衛葬橡 給溥還 匙!!
			strcpy(sPacket.szRetCode,"0000");
			g_socketPayLetter.SendPacket(( char *)&sPacket,sPacket.base.wSize);
		}
		//	婁 晦
		{
			while(1)
			{
				Sleep(1);
				if(!g_socketPayLetter.GetRecvPacket(recvpacket))	continue;

				recvsize = *(WORD *)recvpacket;
				type = *(WORD *)(recvpacket+2);

				BILL_PACK * rPacket  = (BILL_PACK * )recvpacket;
				printf(" type = %d , 擎 議蓮 = %d\n", rPacket->base.wType, rPacket->dwTotalCash);

				break;
			}
		}
	}
	//	掘衙ж晦
	{
		char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
		memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);
		WORD	type=0,recvsize=0;
		//	test
		{
			BILL_PACK		sPacket;
			memset(&sPacket,0,sizeof(BILL_PACK));
			sPacket.base.set( sizeof(BILL_PACK), dREQ_BUYITEM);	//	ぬ擊 嫡擊陽 顫殮薑爾 渠и 薯и 餌о 挫 匙!!
			sPacket.dwReqKey = 314000002;
			strcpy(sPacket.szUserID , "lnklogic");
			strcpy(sPacket.szActorName , "jihyun");
			sPacket.wPresentFlag = 1;
			sPacket.dwReqChargeCashAmt =100;
			sPacket.dwClientIP = 39;	//	п渡 憮憮曖 檜樹お 衛葬橡
			sPacket.wClientPort = 38;		//	dwClientIP п渡 憮曖 衛葬橡 給溥還 匙!!
			sPacket.dwItemID = 200;
			sPacket.wItemCnt = 1;
			strcpy(sPacket.szRetCode,"0000");
			g_socketPayLetter.SendPacket(( char *)&sPacket,sPacket.base.wSize);	
		}
		//	婁 晦
		{
			while(1)
			{
				Sleep(1);
				if(!g_socketPayLetter.GetRecvPacket(recvpacket))	continue;

				recvsize = *(WORD *)recvpacket;
				type = *(WORD *)(recvpacket+2);

				BILL_PACK * rPacket  = (BILL_PACK * )recvpacket;
				printf(" type = %d , 擎  = %d , 援掘[%s][%s] , 憮 [%d][%d]\n", rPacket->base.wType, rPacket->dwTotalCash,rPacket->szUserID, rPacket->szActorName, rPacket->dwClientIP,rPacket->wClientPort);

				break;
			}
		}
	}
*/

//
////////////////////

/*	FILE * dumpFp = fopen("uncom_error2_zzim224_PrincessJudy_-912362547.dump","rb");
	if(dumpFp)
	{
		int filesize = filelength(fileno(dumpFp));
		char * dump = new char[filesize];
		fread(dump,filesize,1,dumpFp);
		fclose(dumpFp);

		// だ曖 憶蹴擊 ヴ橫獄.
		cPLAYER_DATA pData;
		WORD ww = cSRVUTIL::Uncompress((unsigned char *)dump,(unsigned char *)&pData,filesize,sizeof(cPLAYER_DATA));
		if(ww)
		{
			printf(" -0- ");
		}
	}
*/

#ifdef dUSE_SERVERSTATUS_REPORT
//  	if	(l_bIsTestServer == FALSE)
// 	{
// 		if	(!g_SS.init("WORLDNAME", "DBC"))
//  		{
//  			MessageBox(NULL,"ERROR IN SS","WARNING!!",MB_OK);
//  			return 0;
//  		}
// 	}
#endif

	g_isUsePlayerBuffer = 1;

	if	(!LoadDefaultPlayerData("defaultJob.dat"))
	{
		printf("defaultJob.dat is not found. \n");
		return 0;
	}

	printf("- Try Shared memory open\n");

	if	(!g_sharedMemory.open("UDB ClientData"))
	{
		ERRMSG("Can not found shared memory");
		return 0;
	}

	Sleep(1);

	scp.Read(dCONFIGFILENAME);

	FILE * dbcfp = fopen("AvatarDbInfo.bin","rb");

	if	(!dbcfp)
	{
		ERRMSG("Can not read avatar db connection info");
		return 0;
	}

	fread(strAvatarDBSource,128,1,dbcfp);
	fread(strAvatarDBId,64,1,dbcfp);
	fread(strAvatarDBPw,64,1,dbcfp);
	fclose(dbcfp);
	
	dbcfp	=	fopen("isInnerTestServer","rb");
	
	if (dbcfp)
	{
		l_bIsTestDB	=	TRUE;

		printf("for inner test db server\n");

		fclose(dbcfp);
	}

	{
		FILE * webfp = fopen("PurchaseDbInfo.bin","rb");

		if	(webfp)
		{
			fread(strWebDBSource,128,1,webfp);
			fread(strWebDBId,64,1,webfp);
			fread(strWebDBPw,64,1,webfp);
			fclose(webfp);
			l_bIsTestDB = FALSE;
		}
		else
		{
			char * lpstrPurchaseDB = scp.getData("PURCHASE_DB");
			char strDefaultPurchaseDB[] = "redgem";

			if	(lpstrPurchaseDB[0] == 0)
				lpstrPurchaseDB = strDefaultPurchaseDB;

			strcpy(strWebDBSource,lpstrPurchaseDB);
			strcpy(strWebDBId,strAvatarDBId);
			strcpy(strWebDBPw,strAvatarDBPw);
			l_bIsTestDB = FALSE;
			printf("purchase db fallback [%s]\n",strWebDBSource);
		}
	}

	if	(l_bIsTestDB	==	FALSE && strWebDBSource[0] == 0)
	{
		ERRMSG("Can not read Purchase db connection info");
		return 0;
	}

	g_sharedMemory.beginProcess();

	g_lpCdm	=	(CClientDataManager*)g_sharedMemory.getBuffer();

	g_lpCdm->Init();

	g_sharedMemory.endProcess();

	GetCurrentDirectory(1024,mainPath);

	sprintf(savepath,"%s\\save",mainPath);
	CreateDirectory(savepath, NULL);
	sprintf(listpath,"%s\\list",mainPath);
	CreateDirectory(listpath, NULL);
	sprintf(bankpath,"%s\\bank",mainPath);
	CreateDirectory(bankpath, NULL);

	g_dsManager.init(savepath,listpath,bankpath);
	//	衛 logだ 匐餌п憮 熱薑й 頂辨擊 試 熱薑п.
	hEventMngThread = hAcceptThread = hAdditionThread = NULL;

	for(int jj=0;jj<dMAX_WORKER_COUNT;jj++)
		hWorkerThread[jj] = NULL;

#ifdef _FOR_THAI
	hPayLetterThread = NULL;
#endif

	//	孺紫辦 部賊 該雖 蟾晦 
	//	蝶鷗 й渡ж堅...
	InitializeCriticalSection(&csRECVPacket);
	InitializeCriticalSection(&csSENDPacket);
	InitializeCriticalSection(&csRemoveUser);

#ifdef _FOR_THAI
	strcpy(strThaiPayLetterServerIP,scp.getData("PAYLETTER_IP"));
	strThaiPayLetterServerPort = atoi(scp.getData("PAYLETTER_PORT"));
#endif

	strcpy(strAvatarDBIp,scp.getData("IP"));
	g_iSaveGap = atoi(scp.getData("SAVEINTERVAL"));

	if	(!g_iSaveGap)
		g_iSaveGap = 10;	//	雖薑ж雖 彊賊 5廓 и 廓

	strcpy(strLOGIp,scp.getData("LOGIP"));
	dwLOGPort = atoi(scp.getData("LOGPORT"));


	strcpy(strWebDBIp,scp.getData("WEBIP"));

	strcpy(dSTATISTIC.m_strIp , scp.getData("STAT_INFO",1));
	strcpy(dSTATISTIC.m_strId , scp.getData("STAT_INFO",2));
	strcpy(dSTATISTIC.m_strPw , scp.getData("STAT_INFO",3));
	strcpy(dSTATISTIC.m_strDBSource , scp.getData("STAT_INFO",4));
	
	g_bIsMainTestServer	=	atoi(scp.getData("MAIN_TEST"));

// #ifdef	_OGP_SERVICE
// 	if	(g_bIsMainTestServer)
// 		_InitOGPLibrary(dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, "http://api.ogplanet.com");
// 	else
// 		_InitOGPLibrary(dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, "https://billing.ogplanet.com/api");
// #endif


	{
		char	*lpstrTestServer	=	scp.getData("TESTSERVER");

		if	(lpstrTestServer[0])
			l_bIsTestServer	=	atoi(lpstrTestServer);
	}

//	_SLOG = new CTracerFromSaveLog();
//	_SLOG->initialize(mainPath,"SAVELOGFILE");
	cLOG = new DebugTracer(DTType_ALL,"RedStone_DBServer");

	if	(!InitSocket())
	{
		_log("Error InitSocket \n");
		return 0;
	}

	while(1)
	{
		Sleep(1000);

		_log("Wait for begin all worker thread");
		int	 iBeginWorkerCount	=	0;

		for (int i=0;i<dMAX_WORKER_COUNT;i++)
		{
			if	(hWorkerThread[i])
				iBeginWorkerCount++;
		}

		if	(iBeginWorkerCount	==	dMAX_WORKER_COUNT)
			break;
	}

	_log("Start Avatar DB Cache Server ... day : %d - time : %d \n",cSRVUTIL::GetTime(),cSRVUTIL::GetTimes() );

	while(g_bThreadLoop)
	{
		Sleep(1);
	}

	SendWPLifeCode(eEndLifeSign,UserManager.GetClientPnt(l_iWorldServerSerial),l_strServerIp);

	Sleep(1000);

	shutdown(lisSock,1);
	closesocket(lisSock); 
	lisSock	=	NULL;

	for (int i=10;i;)
	{
		Sleep(1000);

		printf("Wait for end all worker thread. %d\n",i);

		--i;

		int	 iDeathWorkerCount	=	0;

		for (int i=0;i<dMAX_WORKER_COUNT;i++)
		{
			if	(hWorkerThread[i]	==	NULL)
				iDeathWorkerCount++;
		}

		if	(iDeathWorkerCount	==	dMAX_WORKER_COUNT)
			break;
	}

// #ifdef	_OGP_SERVICE
// 	_FreeOGPLibrary();
// #endif

	//	и廓 爾頂朝等陛 氈雖虜 鼻窕  熱 氈晦 陽僥 и廓 渦 爾魚.
	g_socketLog.kill();
#ifdef _FOR_THAI
	g_socketPayLetter.kill();
#endif

	if	(l_bIsTestDB	==	FALSE)
	{
		cWeb.Close();
#ifndef _FOR_THAI
		cCarrot.Close();
#endif
	}

	g_adoAvatar.Close();

	DeleteCriticalSection(&csRECVPacket);
	DeleteCriticalSection(&csSENDPacket);
	DeleteCriticalSection(&csRemoveUser);

	WriteServerStatusToRegistry(eST_AVATAR_DBC,eSERVER_STATUS_OFF);

	_log(" Quit DB SERVER !!\n");

	return 0;
}

BOOL InitSocket()
{
	SOCKADDR_IN serverAddress;
	WSADATA		wsadata;

	_log("DB Connect ip[%s] id[%s] source[%s]",strAvatarDBIp,strAvatarDBId,strAvatarDBSource);

	if(!g_adoAvatar.Connect(strAvatarDBIp,strAvatarDBId,strAvatarDBPw,strAvatarDBSource,8))//	艇 憮爾  棺溜.
	{
		_log("AVATAR DB Connect Error !!\n");
		return FALSE;
	}

	int		beginTime = timeGetTime();
	_log("Start    Make DB Index.....");
	if(!g_adoAvatar.MakeDBIndex())
	{
		_log("AVATAR DB Error Make Index!!\n");
		return FALSE;
	}
	int		iSpentTime = timeGetTime()-beginTime;
	_log("End  Make DB Index.  ListCount %d, %d.%.3d Sec",g_Im.dwCount, iSpentTime/1000,iSpentTime%1000);

	{
		beginTime = timeGetTime();
		_log("Start    Get Event Infomation .....");

		if	(l_bIsForcedSettingGoldRushEvent	==	FALSE)
			if (!g_adoAvatar.spEvent_GoldRush_Get_First( &g_cGoldRush_Event ))
				g_cGoldRush_Event.reset();

		for(int i = dMAX_ITEM_PACK_COUNT;i;)
		{	--i;	Sleep(1);
			if (!g_adoAvatar.spEvent_ItemPack_Get_First(i,&g_cItemPack_Event[i] ))	//	腎歷賊..
			{
				g_cItemPack_Event[i].reset();
			}
			g_cItemPack_Event[i].m_wSerial = i;
		}
		iSpentTime = timeGetTime()-beginTime;
		_log("End    Get Event Infomation %d.%.3d Sec",iSpentTime/1000,iSpentTime%1000);
	}

//	詭葬 擎 等檜攪 睡 
	g_sharedMemory.beginProcess();
	g_lpCdm->saveAllDataToDB();
	g_sharedMemory.endProcess();

	if (l_bIsTestDB	==	FALSE)
	{
		_log("Purchase DB Connect ip[%s] id[%s] source[%s]",strWebDBIp,strWebDBId,strWebDBSource);
#ifdef _FOR_KOREA
		if(!cWeb.Connect(strWebDBIp,strWebDBId,strWebDBPw,strWebDBSource,4))
		{
			_log("Web DB Connect Error !!\n");
			return FALSE;
		}
		if(!cCarrot.Connect(strWebDBIp,strWebDBId,strWebDBPw,strWebDBSource,4))
		{
			_log("Carrot DB Connect Error !!\n");
			return FALSE;
		}

		if(!dSTATISTIC.ConnectDB())
		{
			_log("STATISTIC DB Connect Error !!\n");
			return FALSE;
		}

#else
		if(!cWeb.Connect(strWebDBIp,strWebDBId,strWebDBPw,strWebDBSource,4))
		{
			_log("Web DB Connect Error !!\n");
			return FALSE;
		}
#ifndef _FOR_THAI
		if(!cCarrot.Connect(strWebDBIp,strWebDBId,strWebDBPw,strWebDBSource,2))
		{
			_log("Carrot DB Connect Error !!\n");
			return FALSE;
		}
#endif	//	_for_thai
#endif
	}

	if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
	{	_log("Fail WSAStartup !! \n");
		g_adoAvatar.Close();
		return FALSE;
	}

	if((lisSock = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{	_log("Fail listen !!");
		g_adoAvatar.Close();
		return FALSE;
	}

	//	蝶お 憮陽朝 雖и.
//	if(wServerType == 0)
	{
		_log("Log Server [%s] [%d]",strLOGIp,dwLOGPort);
		if(!g_socketLog.init(dwLOGPort,strLOGIp,"LOG SERVER",sizeof(cMSG_BASE_TYPE_FORLOG)))
		{
			_log("Failed connecting Log Server !!");
			return FALSE;
		}
	}
#ifdef _FOR_THAI
	{
		if(!g_socketPayLetter.init(strThaiPayLetterServerPort,strThaiPayLetterServerIP,"payletter server",4))
		{
			_log(" Failed connecting PayLetter Server !!");
			return FALSE;
		}

	}
#endif

	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(dDBCACHE_PORT);

	if(bind(lisSock,(LPSOCKADDR)&serverAddress,sizeof(serverAddress)) == SOCKET_ERROR)
	{	_log("Fail bind !!\n");
		g_adoAvatar.Close();
		return FALSE;
	}

	listen(lisSock,5);

	if(!InitCompletionPort())
	{	_log("Fail Init IOCP Port !!");
		g_adoAvatar.Close();
		return FALSE;
	}
	hAdditionThread = (HANDLE)_beginthread(AdditionThread, 0, NULL); 
	if(hAdditionThread==NULL)
	{	_log("ERROR : addition thread create \n");
		g_adoAvatar.Close();
		return FALSE;
	}

	hEventMngThread = (HANDLE)_beginthread(EventMngThread, 0, NULL); 
	if(hEventMngThread==NULL)
	{	_log("ERROR : Event Manager thread create \n");
		g_adoAvatar.Close();
		return FALSE;
	}

	l_hCommandThread = (HANDLE)_beginthread(CommandThread, 0, NULL); 
	if	(l_hCommandThread==NULL)
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

	l_hAcceptThread			=	(HANDLE)_beginthread(AcceptThread, 0, NULL); 

	if	(!l_hAcceptThread	)
		return FALSE;

#ifdef _FOR_THAI
	hPayLetterThread = (HANDLE)_beginthread(PayLetterThread, 0, NULL);

	if	(hPayLetterThread == NULL)
	{	_log("ERROR : Payletter thread create \n");
		g_adoAvatar.Close();
		return FALSE;
	}
#endif

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

BOOL InitCompletionPort()
{
	SYSTEM_INFO sys_info;

	g_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,0);

	if	(g_hCompletionPort==NULL)
	{
		_log("Fail IOCP Port Create !!");
		return FALSE;
	}

	GetSystemInfo(&sys_info);

	for(int i=0;i<dMAX_WORKER_COUNT;i++)
	{
		hWorkerThread[i] = (HANDLE)_beginthread(WorkerThread, 0,(LPVOID)i); 

		if	(hWorkerThread[i]==NULL)
		{
			_log("Fail Worker [%d] Thread begin !!",i);
			return FALSE;
		}
	}

	return TRUE;
}

void
CommandThread(LPVOID lpParameter)
{
	cInputConsoleCommand	consoleCommand;

	int iReallyQuit = 0;
	
	while(g_bThreadLoop)
	{
		Sleep(1);

		g_lifeCodeMgr.endCheck(eLIFE_CODE_COMMAND);
		char	*lpstrCommand	=	consoleCommand.getCommand();
		g_lifeCodeMgr.beginCheck(eLIFE_CODE_COMMAND);

		if	(lpstrCommand)
		{
			if	(STRICMP(lpstrCommand,"q")==0)
			{
				printf(" Quit? (Y/N) ...");

				iReallyQuit = 1;
			}
			else
			if	(iReallyQuit && STRICMP(lpstrCommand,"y")==0)
			{
				g_bThreadLoop	=	FALSE;
				WriteServerStatusToRegistry(eST_AVATAR_DBC,eSERVER_STATUS_QUIT);
			}
			else
				iReallyQuit	=	FALSE;
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_COMMAND);
}

void
AcceptThread(LPVOID lpParameter)
{
	DWORD	Flags = 0;
	SOCKET	clientsock = INVALID_SOCKET;
	int		index=0;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_ACCEPT);

	while(g_bThreadLoop)
	{
		Sleep(1);

		g_lifeCodeMgr.endCheck(eLIFE_CODE_ACCEPT);

		if	((clientsock = accept(lisSock,NULL,NULL)) != INVALID_SOCKET)
		{
			if	( UserManager.Size()>=dDB_MAX_USER_COUNT)
			{	
				_log("UserManager.Size()>=dDB_MAX_USER_COUNT");	
				Sleep(10);	
				closesocket(clientsock);
				continue;
			}

			int buf=dMAX_PACKET_LENGTH;

			if	(SOCKET_ERROR ==setsockopt(clientsock,SOL_SOCKET ,SO_SNDBUF,(const char*)&buf,sizeof(buf)))
			{
				closesocket(clientsock);
				clientsock = 0;
				continue;
			}
			if	(SOCKET_ERROR ==setsockopt(clientsock,SOL_SOCKET ,SO_RCVBUF,(const char*)&buf,sizeof(buf)))
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

			g_lifeCodeMgr.beginCheck(eLIFE_CODE_ACCEPT);

			index = UserManager.Add();

			if	(index==-1)
			{
				_log("FAIL ADD USER!!!");
				continue;
			}

			CClient *newUser = UserManager.GetClientPnt(index);

			newUser->Open();
			newUser->SetSocket(clientsock);
			newUser->SetSerial(index);

			{
				char		strIp[20];
				SOCKADDR_IN	saddr;

				memset(&saddr,0,sizeof(saddr));
				int tmp	=	sizeof(sockaddr);
				getpeername(clientsock,(SOCKADDR *)&saddr,&tmp);

				strcpy(strIp,inet_ntoa(saddr.sin_addr));

				newUser->SetIP(strIp);
			}

			_logc("[%d]Add User.. %d \n",cSRVUTIL::GetTimes(),UserManager.Size());

			if	(!CreateIoCompletionPort((HANDLE)newUser->GetSocket(),g_hCompletionPort,(DWORD)index,0))
			{
				UserManager.Del(newUser->GetSerial());
				_log("Error CreateIoCompletionPort !! - %d",GetLastError());
				continue;
			}
			RecvPacket(index);
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_ACCEPT);
}

void
LifeCodeUpdateThread(LPVOID lpParameter)
{
	//	錯萄 憮 橫 螢陽梱雖 瑞Щ 絲~
	while(g_bThreadLoop)
	{
		Sleep(10);

		if	(SendWPLifeCode(eBeginLifeSign,UserManager.GetClientPnt(l_iWorldServerSerial),l_strServerIp))
			break;
	}

	WriteServerStatusToRegistry(eST_AVATAR_DBC,eSERVER_STATUS_ON);

	while(1)
	{
		for (int iLoop=50;iLoop;--iLoop)	//	5蟾 и廓 檜Щ 餌檣 歎
		{
			Sleep(100);

			if	(!g_bThreadLoop)
			{
				SendWPLifeCode(eEndLifeSign,UserManager.GetClientPnt(l_iWorldServerSerial),l_strServerIp);
				return;
			}

		}

		WriteLifeSignToRegistry(eST_AVATAR_DBC,1);

		if	(!g_lifeCodeMgr.checkAll(timeGetTime()))
			SendWPLifeCode(eIncorrectStatusSign,UserManager.GetClientPnt(l_iWorldServerSerial),l_strServerIp,g_lifeCodeMgr.m_strBadStatusLog);
		else
			SendWPLifeCode(eUpdateLifeSign,UserManager.GetClientPnt(l_iWorldServerSerial),l_strServerIp);
	}
}

void
WorkerThread(LPVOID lpParameter)
{
	LP_IO_DATA	lpOV;
	DWORD	dwRead,dwIOCPKey;
	int		threadSerial		=	(int)lpParameter;

	_log("START WorkerThread[%d] ...",threadSerial);
	BOOL	retVal = TRUE;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_WORKER);

	while(g_bThreadLoop)
	{
		dwIOCPKey	=	0xffffffff;
		g_lifeCodeMgr.endCheck(eLIFE_CODE_WORKER);
		retVal		=	GetQueuedCompletionStatus(g_hCompletionPort,&dwRead,&dwIOCPKey,(LPOVERLAPPED *)&lpOV,5000);

		g_lifeCodeMgr.beginCheck(eLIFE_CODE_WORKER);

		if	( retVal == TRUE && dwIOCPKey < dGAME_MAX_USER_COUNT && dwRead != 0 && lpOV != 0 )
		{
			CClient	* user = UserManager.GetClientPnt(dwIOCPKey);

			if	(!user)
			{
				continue;
			}
			
			if	(lpOV->IOState == 0)
			{
				user->AddRecvPacket(dwRead);
				RecvPacket(dwIOCPKey);
				ProcessRecvPacket(dwIOCPKey);	//	if(l_bIsTestDB)
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
				continue;

			if	(!lpOV)
				_log("SERROR : getqueue... lpOV = NULL");

			if	(dwRead == 0 )
			{
				CClient	* user = UserManager.GetClientPnt(dwIOCPKey);

				if	(!user)
				{
					_log("SERROR : getqueue... user==NULL || !user->GetIsUsed() %d",dwIOCPKey);
					continue;	
				}

				DisUser(dwIOCPKey);
			}
			else
			{
				DWORD dwError = GetLastError();
				_log("SERROR GetQueue Error : %d ",dwError);
				if	(dwError	==	ERROR_NETNAME_DELETED)
					_log("dwError ==ERROR_NETNAME_DELETED");

				if	(dwError != ERROR_IO_PENDING && dwError!=ERROR_OPERATION_ABORTED && dwIOCPKey < dGAME_MAX_USER_COUNT)
				{
					_log("%d Worker thread error GetQueueCompletionStatus... error [%d]",threadSerial,dwError);
					DisUser(dwIOCPKey);
				}
			}
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_WORKER);

	hWorkerThread[threadSerial]	=	NULL;

	_log("END WorkerThread[%d] ...",threadSerial);
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
			DisUser(serial);
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

	//	lpSend曖 高擊 iSendSize高婁 綠掖п爾賊 橫雯 婁陛 螢梱?
	if(WSASend(user->GetSocket(), &buf,1,&lpSend,0,&user->sOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();

		if	(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{	
			user->sBuffer->Next();
			DisUser(serial);
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

	if(WSASend(user->GetSocket(), &buf,1,&lpSend,0,&user->sOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();
		if(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			user->sBuffer->Next();
			DisUser(serial);
			return;
		}
	}
}

void	
sendLogDeleteGuild(char* _strGuildName, char* _strRequestName, WORD _wReason)
{
	
	if(!_strGuildName || !_strRequestName)
		return;
	
	time_t tTime;	
	struct tm *tmTime;		
	tTime = time(NULL);	
	tmTime = localtime(&tTime);

	LOG_DELETE_GUILD	logPacket;

	logPacket.base.set(sizeof(LOG_DELETE_GUILD),dLOG_GUILD_WORK);

	logPacket.logTime.yy		=	tmTime->tm_year%100;
	logPacket.logTime.mm		=	tmTime->tm_mon+1;
	logPacket.logTime.dd		=	tmTime->tm_mday;
	logPacket.logTime.hour		=	tmTime->tm_hour;
	logPacket.logTime.min		=	tmTime->tm_min;
	logPacket.wFieldUniqueIndex	=	0xffff;
	strcpy(logPacket.strName	, _strRequestName);
	strcpy(logPacket.strGuildName , _strGuildName);
	strcpy(logPacket.strId,"");
	strcpy(logPacket.strIP,"");
	logPacket.wReason			=	_wReason;
	logPacket.wWork				=	eGW_DELETE_GUILD;
	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);
}

void	
sendLogChangeGuildRank(char* _strGuildName, char* _strMasterName, char* _strTargetName, WORD _wTargetRank, WORD _wMasterRank, WORD _wReason)
{
	
	if(!_strGuildName || !_strMasterName || !_strTargetName)
		return;


	if(_wReason == eCGR_APPOINT && _wTargetRank != dGUILD_CLASS_MASTER || _wReason == eCGR_DISMISSAL)
		return;

	
	time_t tTime;	
	struct tm *tmTime;		
	tTime = time(NULL);	
	tmTime = localtime(&tTime);

	LOG_CHANGE_GUILD_RANK	logPacket;

	logPacket.base.set(sizeof(LOG_CHANGE_GUILD_RANK),dLOG_GUILD_WORK);

	logPacket.logTime.yy		=	tmTime->tm_year%100;
	logPacket.logTime.mm		=	tmTime->tm_mon+1;
	logPacket.logTime.dd		=	tmTime->tm_mday;
	logPacket.logTime.hour		=	tmTime->tm_hour;
	logPacket.logTime.min		=	tmTime->tm_min;
	logPacket.wFieldUniqueIndex	=	0xffff;
	strcpy(logPacket.strName	, _strMasterName);
	strcpy(logPacket.strGuildName , _strGuildName);
	strcpy(logPacket.strId,"");
	strcpy(logPacket.strIP,"");
	strcpy(logPacket.strTargetName,_strTargetName);
	logPacket.wReason			=	_wReason;
	logPacket.wTargetRank		=	_wTargetRank;
	logPacket.wMasterRank		=	_wMasterRank;
	logPacket.wWork				=	eGW_CHANGE_GUILD_RANK;
	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);
}


void	ProcessRecvPacket(int serial)
{
	CClient	*user	=	UserManager.GetClientPnt(serial);

	if	(!user || user->GetIsClose())
		return;

	WORD	type=0,recvsize=0;

	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	int		resultGetRecvPacket=0;

	while(TRUE)
	{
		resultGetRecvPacket	=	user->GetRecvPacket(recvpacket);

		if	(!resultGetRecvPacket)
			break;

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
			case dDDMSG_IAM:
			{
				DDMSG_IAM * rPacket = (DDMSG_IAM *)recvpacket;

				if	(rPacket->iIam==2)
					user->SetSrvType(CClient::eSRVTYPE_BC);
				break;
			}

			case dWORLDtoSERVERS_WORLDNAME:
			{
				WORLDtoSERVERS_WORLDNAME * rPacket = (WORLDtoSERVERS_WORLDNAME *)recvpacket;

				g_iServerType	=	rPacket->wServerType;
#ifdef dUSE_SERVERSTATUS_REPORT
				if	(l_bIsTestServer	==	FALSE)
					sprintf(g_SS.m_strServerName,"%s_DBC", rPacket->strWorldName);
#endif
				break;
			}

			case dW2D_ASK_GVG_AVATAR_ORG_DATA	:
			{
				W2D_ASK_GVG_AVATAR_ORG_DATA * rPacket = (W2D_ASK_GVG_AVATAR_ORG_DATA *)recvpacket;

				OperateW2D_ASK_GVG_AVATAR_ORG_DATA(user,rPacket);

				break;
			}

			case dWDMSG_TRANS_AVATAR_TO_GVG	:
			{
				WDMSG_TRANS_AVATAR_TO_GVG * rPacket = (WDMSG_TRANS_AVATAR_TO_GVG *)recvpacket;

				OperateTransAvatarToGVGPacket(user,rPacket);

				break;
			}

			case dWDMSG_ASK_EVENTS_INFO:	//	錯萄 憮陛  麵韓陽 , 檜漸お 薑爾 蹂ж晦 嬪п 爾頂朝 ぬ. 檜 ぬ煎 錯萄 憮檣雖, 晦顫 憮檣雖 挫.
			{
				WDMSG_ASK_EVENTS_INFO * rPacket = (WDMSG_ASK_EVENTS_INFO * )recvpacket;

				user->SetSrvType(CClient::eSRVTYPE_WORLD);

				WriteWorldServerIPToRegistry(eST_AVATAR_DBC,user->m_strIP);

				l_iWorldServerSerial	=	user->GetSerial();

				if	(!rPacket->isReally)
					break;

				//	萄 蓮 薑爾 爾魚遽.
				{
					DWMSG_GR_INFO_RESULT sPacket;
					sPacket.base.set(sizeof(DWMSG_GR_INFO_RESULT),dDWMSG_GR_INFO_RESULT);
					memcpy(&sPacket.begin,&g_cGoldRush_Event,sizeof(cGoldRushDefine));
					user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				}

				//	檜蠱 ね 睡 爾頂遽.
				{
					DWMSG_ITEMPACK_RESULT sPacket;
					sPacket.base.set(sizeof(DWMSG_ITEMPACK_RESULT),dDWMSG_ITEMPACK_RESULT);

					for(int i = 0;i<dMAX_ITEM_PACK_COUNT;i++)
					{
						Sleep(1);
						sPacket.wSerial = i;

						if	(g_cItemPack_Event[i].m_wLinkItem < 0xffff  || i==dMAX_ITEM_PACK_COUNT-1)
						{
							// 等檜攪陛 綠歷擊陽朝 輿雖 堅, dMAX_ITEM_PACK_COUNT廓簞朝 部擊 憲葬晦 嬪п憮 爾頂.
							memcpy(&sPacket.info,&g_cItemPack_Event[i],sizeof(cItemPackInfo));
							user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
						}
					}
				}
				break;
			}
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早	橫萄團 罐憮 陳朝 萄
			case dLDMSG_DELETE_LIST:		//	か薑 嶸曖 葬蝶お だ 雖辦晦
			{
				//	⑷營  INDEX 脹 睡婁 db 氈朝 睡擊 匐餌п憮 熱薑и.
				LDMSG_DELETE_LIST	*	rPacket;
				rPacket = (LDMSG_DELETE_LIST *)recvpacket;

				CheckString(rPacket->strId,dID_LENGTH,"server.cpp",__LINE__);

				_log("LDMSG_DELETE_LIST %s ",rPacket->strId);
				g_Im.removeAll(rPacket->strId);
 				//	DB憮 id煎 匐儀п憮 陛螳堅.. 
				_DBINDEXTABLE	avaList;
				g_adoAvatar.GetDBIndex(rPacket->strId,avaList);

				for(int i = 0;i<dMAX_AVATAR_COUNT;i++)
				{
					if(avaList.wAvatarIndex[i]==0xffff)	continue;
					int	iIndex = g_Im.getIndex(avaList.strName[i],rPacket->strId);
					if(iIndex==0x7fffffff)
					{
						DWORD ret = g_Im.add(avaList.dwIndex[i],avaList.strName[i],rPacket->strId,avaList.wAvatarIndex[i]);
						_logf(" ADD DBINDEX : [%d] [%s] [%s] [%d],ret %d",avaList.dwIndex[i],avaList.strName[i],rPacket->strId,avaList.wAvatarIndex[i],ret);
					}
				}
				break;
			}

			case dLDMSG_UPDATESAVE_USER:	//	か薑 嶸曖 撮檜 だ 機等檜お ж晦
			{
				break;
			}
			case dLDMSG_UPDATESAVE:			//	 氈朝  議葛攪曖 撮檜 だ 機等檜お ж晦
			{
				break;
			}

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早	AVATAR PACKET
			case dLDMSG_GETAVATARS:
			{
				DWORD sTime = timeGetTime();
				LDMSG_GETAVATARS	*	rPacket;
				rPacket = (LDMSG_GETAVATARS *)recvpacket;

				CheckString(rPacket->strID,dID_LENGTH,"server.cpp",__LINE__);

				DLMSG_GETAVATARS			sGetAvas;
				memset(&sGetAvas,0,sizeof(DLMSG_GETAVATARS));
				sGetAvas.base.set(sizeof(DLMSG_GETAVATARS),dDLMSG_GETAVATARS);
				sGetAvas.u_serial = rPacket->u_serial;
				sGetAvas.worldSerial = rPacket->worldSerial;
				strcpy(sGetAvas.strID,rPacket->strID);
				memset(sGetAvas.aCharacter,0xff,sizeof(cAVATAR_INFO)*dMAX_AVATAR_COUNT);
				g_adoAvatar.GetAvaSInfo(rPacket->strID,sGetAvas);
				BOOL	isReadAgain = FALSE;

				for(int i=0;i<dMAX_AVATAR_COUNT;i++)
				{
					if	(sGetAvas.aCharacter[i].m_wAvatarIndex<dMAX_AVATAR_COUNT && sGetAvas.aCharacter[i].m_wLastField==0xffff)
					{	//	GetAvas and SaveList && Load
						cPLAYER_DATA	pData;	
						WORD			wOper = 0;

						memset(&pData,0xff,sizeof(cPLAYER_DATA));
						g_adoAvatar.spGetAvaInfo(sGetAvas.aCharacter[i].m_strName,sGetAvas.strID, &pData, wOper);

						if(pData.m_wAvatarIndex<dMAX_AVATAR_COUNT)
						{
							pData.m_dwSaveCount++;
							g_adoAvatar.SaveAva(&pData,pData.m_dwPlayTime);
							isReadAgain	=	TRUE;
						}
						else
						{
							sGetAvas.aCharacter[i].m_wAvatarIndex = 0xffff;
							strcpy(sGetAvas.aCharacter[i].m_strName,"EMPTY");
						}
					}
				}

				if	(isReadAgain)
				{
					_log(" read again : %s",rPacket->strID);
					g_adoAvatar.GetAvaSInfo(rPacket->strID,sGetAvas);

					for(i=0;i<dMAX_AVATAR_COUNT;i++)
					{
						if(sGetAvas.aCharacter[i].m_wAvatarIndex<dMAX_AVATAR_COUNT && sGetAvas.aCharacter[i].m_wLastField==0xffff)
						{
							sGetAvas.aCharacter[i].m_wAvatarIndex = 0xffff;
							strcpy(sGetAvas.aCharacter[i].m_strName,"EMPTY");
						}
					}
				}
				user->AddSendPacket((char *)&sGetAvas,sGetAvas.base.wSize);
				DWORD retTime = timeGetTime() - sTime;
				_logc("dLDMSG_GETAVATARS  %d.%03d sec..",retTime/1000,retTime%1000);
				break;
			}

			case dLDMSG_CREATEAVATAR://斐褒伎
			{
				DWORD allTime = timeGetTime();
				LDMSG_CREATEAVATAR	*	rPacket;
				rPacket = (LDMSG_CREATEAVATAR *)recvpacket;

				CheckString(rPacket->strID,dID_LENGTH,"server.cpp",__LINE__);

				cPLAYER_DATA	pData;	//	field廓 world憮 稱罹遽.
				memset(&pData,0,sizeof(cPLAYER_DATA));
				pData.initializeJobData(rPacket->strID,rPacket->strName,rPacket->wJob,-1,g_iServerType);

				if	(rPacket->wServerType	==	eSERVER_TYPE_DUEL)
					g_dsManager.makeDuelPlayer(&pData);

				DLMSG_CREATEAVATAR		sCreateAva;
				memset(&sCreateAva,0,sizeof(DLMSG_CREATEAVATAR));
				sCreateAva.base.set(sizeof(DLMSG_CREATEAVATAR),dDLMSG_CREATEAVATAR);

				sCreateAva.u_serial		=	rPacket->u_serial;
				sCreateAva.worldSerial	=	rPacket->worldSerial;
				strcpy(sCreateAva.strID,rPacket->strID);
				strcpy(sCreateAva.strName,rPacket->strName);

				DWORD	dwDbIndex = 0x7fffffff;
				int	result	=	g_adoAvatar.spCreateAva(dwDbIndex,&pData,g_Im.getBlankPos(sCreateAva.strID));
//				_log("result %d, dwDbIndex %x ",result,dwDbIndex);
				if(result>=0)
				{
					if(dwDbIndex==0x7fffffff)
					{
//						_log("ERROR DBINDEX : [%s][%s]->avaindex(%d) is reqular, dbindex(%d) is irrequlary.",pData.m_strName,pData.m_strId,result,dwDbIndex);
						g_Im.removeAll(pData.m_strId);
						_DBINDEXTABLE	avaList;
						g_adoAvatar.GetDBIndex(pData.m_strId,avaList);
						for(int i = 0;i<dMAX_AVATAR_COUNT;i++)
						{
							if(avaList.wAvatarIndex[i]==0xffff)	continue;
							int	iIndex = g_Im.getIndex(avaList.strName[i],pData.m_strId);
							if(iIndex==0x7fffffff)
							{
								DWORD ret = g_Im.add(avaList.dwIndex[i],avaList.strName[i],pData.m_strId,avaList.wAvatarIndex[i]);
								_log(" RE ADD DBINDEX : [%d] [%s] [%s] [%d],ret %d",avaList.dwIndex[i],avaList.strName[i],pData.m_strId,avaList.wAvatarIndex[i],ret);
							}
						}
					}
					else
					{
						DWORD ret = g_Im.add(dwDbIndex,pData.m_strName,pData.m_strId,pData.m_wAvatarIndex);
						_log(" ADD DBINDEX : [%d] [%s] [%s] [%d],ret %d",dwDbIndex,pData.m_strName,pData.m_strId,pData.m_wAvatarIndex,ret);
					}

					sCreateAva.wResult = 0;
					sCreateAva.wIndex = result;
					//	衛 檜Я 輿模 - в萄 檣策蝶 褻餌п憮 輿模 厥橫醜 и. 錯萄憮 п ... 
					pData.getAvatarData(&sCreateAva.aCharacter);

					g_sharedMemory.beginProcess();
					g_lpCdm->removeByIdAndIndex(pData.m_strId,sCreateAva.wIndex);
					g_sharedMemory.endProcess();
				}else
				{
					sCreateAva.wResult	=	result;
					sCreateAva.wIndex	=	0;

					strcpy(sCreateAva.aCharacter.m_strName,"EMPTY");
					sCreateAva.aCharacter.m_wJob = 0xffff;
				}
/*				
				g_Im.removeAll(pData.m_strId);
 				//	DB憮 id煎 匐儀п憮 陛螳堅.. 
				_DBINDEXTABLE	avaList;
				g_adoAvatar.GetDBIndex(pData.m_strId,avaList);

				for(int i = 0;i<dMAX_AVATAR_COUNT;i++)
				{
					if(avaList.wAvatarIndex[i]==0xffff)	continue;
					int	iIndex = g_Im.getIndex(avaList.strName[i],pData.m_strId);
					if(iIndex==0x7fffffff)
					{
						if(g_Im.add(avaList.dwIndex[i],avaList.strName[i],pData.m_strId,avaList.wAvatarIndex[i]))
							_logc(" ADD DBINDEX : [%d] [%s] [%s] [%d]",avaList.dwIndex[i],avaList.strName[i],pData.m_strId,avaList.wAvatarIndex[i]);
						else
							_logf(" ADD DBINDEX FAIL: [%d] [%s] [%s] [%d]",avaList.dwIndex[i],avaList.strName[i],pData.m_strId,avaList.wAvatarIndex[i]);
					}
				}
*/
				user->AddSendPacket((char *)&sCreateAva,sCreateAva.base.wSize);
//				if(wServerType==0)
				{
					time_t tTime;		struct tm *tmTime;			tTime = time(NULL);			tmTime = localtime(&tTime);
					LOG_AVA_CREATE		logPacket;
					logPacket.base.set(sizeof(logPacket),dLOG_AVA_CREATE);
					logPacket.logTime.yy		=	tmTime->tm_year%100;
					logPacket.logTime.mm		=	tmTime->tm_mon+1;
					logPacket.logTime.dd		=	tmTime->tm_mday;
					logPacket.logTime.hour		=	tmTime->tm_hour;
					logPacket.logTime.min		=	tmTime->tm_min;

					strcpy(logPacket.strId,rPacket->strID);
					strcpy(logPacket.strName,rPacket->strName);
					strcpy(logPacket.strIP,rPacket->strIP);
					g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
				}
				DWORD retTime = timeGetTime() - allTime;
				_logf("FINISH dLDMSG_CREATEAVATAR  %d.%03d sec..",retTime/1000,retTime%1000);
				break;
			}

			case dLDMSG_DELETEAVATAR://壺褒伎
			{
				LDMSG_DELETEAVATAR	* rPacket;
				rPacket = (LDMSG_DELETEAVATAR *)recvpacket;

				CheckString(rPacket->strID,dID_LENGTH,"server.cpp",__LINE__);
				CheckString(rPacket->strNAME,dNAME_LENGTH,"server.cpp",__LINE__);

				DLMSG_DELETEAVATAR		sDelAva;
				memset(&sDelAva,0,sizeof(DLMSG_DELETEAVATAR));
				sDelAva.base.set(sizeof(DLMSG_DELETEAVATAR),dDLMSG_DELETEAVATAR);
				strcpy(sDelAva.strID,rPacket->strID);
				strcpy(sDelAva.strName,rPacket->strNAME);

				sDelAva.u_serial	=	rPacket->u_serial;
				sDelAva.worldSerial	=	rPacket->worldSerial;

				sDelAva.guildSerial	=	0xffff;
				sDelAva.wResult		=	g_adoAvatar.DeleteAva(rPacket->strID,rPacket->strNAME,sDelAva.guildSerial);
				user->AddSendPacket((char *)&sDelAva,sDelAva.base.wSize);

				g_sharedMemory.beginProcess();
				g_lpCdm->removeByIdAndIndex(rPacket->strID,sDelAva.wResult);
				g_sharedMemory.endProcess();

				{
					time_t tTime;		struct tm *tmTime;			tTime = time(NULL);			tmTime = localtime(&tTime);
					LOG_AVA_DELETE		logPacket;
					logPacket.base.set(sizeof(logPacket),dLOG_AVA_DELETE);
					logPacket.logTime.yy		=	tmTime->tm_year%100;
					logPacket.logTime.mm		=	tmTime->tm_mon+1;
					logPacket.logTime.dd		=	tmTime->tm_mday;
					logPacket.logTime.hour		=	tmTime->tm_hour;
					logPacket.logTime.min		=	tmTime->tm_min;

					strcpy(logPacket.strId,rPacket->strID);
					strcpy(logPacket.strName,rPacket->strNAME);
					strcpy(logPacket.strIP,rPacket->strIP);
					g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
				}
				break;
			}

			case dLDMSG_SELECTAVATAR://恁褒伎
			{
				LDMSG_SELECTAVATAR * rPacket;
				rPacket = (LDMSG_SELECTAVATAR *)recvpacket;

				DLMSG_SELECTAVATAR		sLoadAva;
				memset(&sLoadAva,0,sizeof(DLMSG_SELECTAVATAR));

				sLoadAva.base.set(sizeof(DLMSG_SELECTAVATAR),dDLMSG_SELECTAVATAR);
				sLoadAva.wResult		=	eDLMSG_SELECTAVATAR_SUCCESS;
				sLoadAva.loginSerial	=	rPacket->loginSerial;
				sLoadAva.worldSerial	=	rPacket->worldSerial;
				strcpy(sLoadAva.strID,rPacket->strID);
				strcpy(sLoadAva.strNAME,rPacket->strNAME);

				cPLAYER_DATA	pData;
				WORD			wOper	=	0;

				memset(&pData,0,sizeof(cPLAYER_DATA));
	
				CheckString(rPacket->strID,dID_LENGTH,"server.cpp",__LINE__);
				CheckString(rPacket->strNAME,dNAME_LENGTH,"server.cpp",__LINE__);

				g_adoAvatar.spGetAvaInfo(rPacket->strNAME, rPacket->strID, &pData, wOper);
				g_sharedMemory.beginProcess();

				if (g_lpCdm->getClientById(pData.m_strId))
				{
					g_lpCdm->removeByIdAndSave(pData.m_strId,&pData);

					//	綠薑鼻瞳煎 橫霞 辦 檜 憮朝 橈.
					//	斜煎 檜憮 撮檜 等檜攪虜 偵褐ж堅 
					//	嶸擎 煎 樓й 熱 氈紫煙 п輿賊 脹.
					sLoadAva.wResult	=	eDLMSG_SELECTAVATAR_SUCCESS;//eDLMSG_EXIST_CONNECT_USER;
				}else
				if	(g_lpCdm->Add(&pData,wOper)	==	0xffff)
				{
					sLoadAva.wResult	=	eDLMSG_SELECTAVATAR_FAIL;
					_log("CHECK POINT : SELECT_AVATAR : FAILED TO ADD [%s/%s]",pData.m_strId,pData.m_strName);
				}

				g_sharedMemory.endProcess();

				user->AddSendPacket((char *)&sLoadAva,sLoadAva.base.wSize);
				//	公 瑞Щ 氈擊梱?
				break;
			}

			case dGDMSG_LOGINAVATAR:	//	恁綴褒伎腎
			{
				g_sharedMemory.beginProcess();	//	膠橫萄 詭葬 硫!!
				g_lpCdm->lock();				//	葬じ 播暮 衛

				GDMSG_LOGINAVATAR * rPacket;
				rPacket = (GDMSG_LOGINAVATAR *)recvpacket;

				CClientData *lpClient	= g_lpCdm->getClient(rPacket->strName,TRUE);

				DGMSG_LOGINAVATAR			sLoginAva;
				memset(&sLoginAva,0,sizeof(DGMSG_LOGINAVATAR));
				sLoginAva.u_serial		=	rPacket->u_serial;
				sLoginAva.worldSerial	=	rPacket->worldSerial;
				sLoginAva.badtime		=	rPacket->badtime;
				sLoginAva.isOper		=	rPacket->isOper;
				sLoginAva.isNetCafeUser	=	rPacket->isNetCafeUser;

				CheckString(rPacket->strId,dID_LENGTH,"server.cpp",__LINE__);

				if	(lpClient && STRICMP(lpClient->pData.m_strId,rPacket->strId) == 0)
				{
					lpClient->dwTimer	=	timeGetTime();
					lpClient->isSend	=	TRUE;

					sLoginAva.Oper		=	lpClient->wOper;
					sLoginAva.base.set(sizeof(DGMSG_LOGINAVATAR),dDGMSG_LOGINAVATAR);
					sLoginAva.wResult	=	1;
					memcpy(&sLoginAva.pData,&lpClient->pData,sizeof(cPLAYER_DATA));

//					if(wServerType == 0)
					{
						time_t tTime;		struct tm *tmTime;			tTime = time(NULL);			tmTime = localtime(&tTime);
						LOG_AVA_LOGIN		logPacket;
						logPacket.base.set(sizeof(logPacket),dLOG_AVA_LOGIN);
						logPacket.logTime.yy		=	tmTime->tm_year%100;
						logPacket.logTime.mm		=	tmTime->tm_mon+1;
						logPacket.logTime.dd		=	tmTime->tm_mday;
						logPacket.logTime.hour		=	tmTime->tm_hour;
						logPacket.logTime.min		=	tmTime->tm_min;

						strcpy(logPacket.strId,lpClient->pData.m_strId);
						strcpy(logPacket.strName,lpClient->pData.m_strName);
						strcpy(logPacket.strIP,rPacket->strIP);
						g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
					}
				}
				else
				{
					// 檣腎雖 彊擎 嶸陛 樓ц. 斜喫擎 穢溥 -0-
					_logf("[%s(%s)]this user's info was not found.",rPacket->strId,rPacket->strName);
					sLoginAva.wResult	=	0;
					sLoginAva.base.set(sizeof(cMSG_BASE_TYPE_FORDB)+sizeof(DWORD)*2+sizeof(WORD)*3,dDGMSG_LOGINAVATAR);
				}

				user->AddSendPacket((char *)&sLoginAva,sLoginAva.base.wSize);
				g_lpCdm->unlock();				//	葬じ 播暮 ヴ橫醜
				g_sharedMemory.endProcess();	//	膠橫萄 詭葬  質.
				break;
			}

			case dLDMSG_DELETELIST:
			{
				break;
			}

			case dLDMSG_COPY_DATA:
			{
				LDMSG_COPY_DATA * rPacket;
				rPacket = (LDMSG_COPY_DATA *)recvpacket;

				DLMSG_COPY_DATA_RESULT	result;

				result.base.set(sizeof(result),dDLMSG_COPY_DATA_RESULT);

				memcpy(result.strId,rPacket->strId,dID_LENGTH);

				result.wServerType	=	rPacket->wType;
				result.wResult		=	g_dsManager.copyData(&rPacket->info,rPacket->strId);

				user->AddSendPacket((char *)&result,result.base.wSize);
				break;
			}

			case dWDMSG_KILL_AVATAR		://world->db褒伎掩
			{
				g_sharedMemory.beginProcess();

				g_lpCdm->removeByIdAndSave(((WDMSG_KILL_AVATAR *)recvpacket)->strId);
				
				g_sharedMemory.endProcess();

				break;
			}

			case dGDMSG_LOGOUTAVATAR	://GS->DB褒伎堤
			{
				GDMSG_LOGOUTAVATAR		*	rPacket;
				rPacket = (GDMSG_LOGOUTAVATAR *)recvpacket;
				cPLAYER_DATA	*lpData	=	(cPLAYER_DATA *)&rPacket->pData;

				CheckString(lpData->m_strId,dID_LENGTH,"server.cpp",__LINE__);
				CheckString(lpData->m_strName,dNAME_LENGTH,"server.cpp",__LINE__);
//				int	iBeginTime	=	timeGetTime();
				DGMSG_LOGOUTAVATAR			sPacket;
				sPacket.base.set(sizeof(DGMSG_LOGOUTAVATAR),dDGMSG_LOGOUTAVATAR);
				strcpy(sPacket.strName,rPacket->pData.m_strName);
				sPacket.iActorSerialInServer=	rPacket->iActorSerialInServer;
				sPacket.wResult				=	g_dsManager.save((cPLAYER_DATA *)&rPacket->pData);
				sPacket.dwSaveCount			=	rPacket->pData.m_dwSaveCount;

//				int	iSpentTime				=	timeGetTime()-iBeginTime;
//				_log("Spend Time %d.%.3d Sec",iSpentTime/1000,iSpentTime%1000);

				g_sharedMemory.beginProcess();
				if (g_dsManager.isRequireResaveResult(sPacket.wResult))
					g_lpCdm->addFailedData((cPLAYER_DATA *)&rPacket->pData);
				g_lpCdm->removeById(rPacket->pData.m_strId);
				g_sharedMemory.endProcess();

				if(rPacket->iActorSerialInServer	!=	0xffffffff)	//	0xffffffff 檜賊 憮 麵雖賊憮 爾魚 掘 等檜攪
				{
					user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
//					_log("Send DGMSG_LOGOUTAVATAR [%s], Spend Time %d.%.3d Sec",sPacket.strName,iSpentTime/1000,iSpentTime%1000);
				}

//				if(wServerType == 0)
				{
					time_t tTime;		struct tm *tmTime;			tTime = time(NULL);			tmTime = localtime(&tTime);
					LOG_AVA_LOGOUT	logPacket;
					logPacket.base.set(sizeof(logPacket),dLOG_AVA_LOGOUT);
					logPacket.logTime.yy		=	tmTime->tm_year%100;
					logPacket.logTime.mm		=	tmTime->tm_mon+1;
					logPacket.logTime.dd		=	tmTime->tm_mday;
					logPacket.logTime.hour		=	tmTime->tm_hour;
					logPacket.logTime.min		=	tmTime->tm_min;

					strcpy(logPacket.strId,lpData->m_strId);
					strcpy(logPacket.strName,lpData->m_strName);
					strcpy(logPacket.strIP,"");
					g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
				}
				break;
			}

//	だ 撮檜虜 霞чи. - DB擎 ж雖 彊朝.
			case dWDMSG_SAVEAVA_TO_FILE://world->DB 褒伎善
			{
				WDMSG_SAVEAVA_TO_FILE * rPacket;
				rPacket = (WDMSG_SAVEAVA_TO_FILE *)recvpacket;

				cPLAYER_DATA	*lpData	=	(cPLAYER_DATA *)&rPacket->pData;

				CheckString(lpData->m_strId,dID_LENGTH,"server.cpp",__LINE__);
				CheckString(lpData->m_strName,dNAME_LENGTH,"server.cpp",__LINE__);

				int	iResult = eSAVE_SUCCESS_SAVEFILE;

				if	(lpData->m_bf1IsLevelDown)
				{
					g_dsManager.save(lpData);
				}
				else
				if	(!g_isUsePlayerBuffer || lpData->m_dwSaveCount%g_iSaveGap == 0)	
				{
					g_lpCdm->remove(lpData->m_strId,0);
					g_dsManager.save(lpData);
				}

				if	(g_isUsePlayerBuffer)
				{
					g_sharedMemory.beginProcess();
					int	iResult		=	g_lpCdm->updatePlayerDataByName(rPacket->pData.m_strName,lpData);

					if (iResult		==	eSAVE_CAN_NOT_FIND_LOGIN_USER)
					{	//	詭葬憮 瓊雖 ж賊 DB煎 欽 煎 ж堅..
						int	iSaveResult	=	g_dsManager.save(lpData);

						if (g_dsManager.isRequireResaveResult(iSaveResult))
							g_lpCdm->addFailedData((cPLAYER_DATA *)&rPacket->pData);
					}
					else
					if (iResult == eSAVE_MEMORY_FAIL_LITTLECOUNT2)
					{
						CClientData * _backupdata = g_lpCdm->getClientByName(rPacket->pData.m_strName);

						if	(_backupdata)
						{
							_log("Error in dWDMSG_SAVEAVA_TO_FILE:");
							_log("   Save Count : newdata[%d] , backupdata[%d]",rPacket->pData.m_dwSaveCount,_backupdata->pData.m_dwSaveCount);
							_log("   ID/Name : newdata[%s/%s] , backupdata[%s/%s]",rPacket->pData.m_strId,rPacket->pData.m_strName,_backupdata->pData.m_strId,_backupdata->pData.m_strName);
							_log("   Lev/Exp : newdata[%d/%d] , backupdata[%d/%d]",rPacket->pData.m_iLevel,rPacket->pData.m_iExperience,_backupdata->pData.m_iLevel,_backupdata->pData.m_iExperience);
						}
					}
					g_sharedMemory.endProcess();
				}
				if (rPacket->wIsWantResult || iResult == eSAVE_CAN_NOT_FIND_LOGIN_USER)	//	婁 ж剪 樓п 氈朝 嶸 瓊擊 熱 橈.
				{
					DWMSG_SAVEAVA_TO_FILE		sPacket;

					sPacket.base.set(sizeof(DWMSG_SAVEAVA_TO_FILE),dDWMSG_SAVEAVA_TO_FILE);

					sPacket.iSerialInServer	=	rPacket->iSerialInServer;
					sPacket.wResult			=	iResult;
					strcpy(sPacket.strName,rPacket->pData.m_strName);

					user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				}
				break;
			}

			case dWDMSG_BANKOPEN://WORDL->DB湖羲
			{
				WDMSG_BANKOPEN * rPacket;
				rPacket = (WDMSG_BANKOPEN *)recvpacket;

				DWMSG_BANKOPEN			rBank;
				rBank.base.set(sizeof(DWMSG_BANKOPEN),dDWMSG_BANKOPEN);
				rBank.iActorSerial= rPacket->iActorSerial;

				if	(!g_dsManager.openBank(rPacket->strID,&rBank.cBank))
				{
					rBank.cBank.m_dwSaveCounter = 0;
					_logf("ERROR GET BANK : What's it??");
				}
				else
				{
					strcpy(rBank.cBank.m_strId, rPacket->strID);
				}
				user->AddSendPacket((char *)&rBank,rBank.base.wSize);
				break;
			}

			case dWDMSG_SAVEBANK://WORDL->DB
			{
				WDMSG_SAVEBANK * rPacket;
				rPacket = (WDMSG_SAVEBANK *)recvpacket;

				DWMSG_SAVEBANK			rBank;
				rBank.base.set(sizeof(DWMSG_SAVEBANK),dDWMSG_SAVEBANK);
				rBank.iActorSerial = rPacket->iActorSerial;
				strcpy(rBank.strID,rPacket->cBank.m_strId);
				if(STRICMP(rBank.strID,rPacket->cBank.m_strId)!=0)
				{
					_log("ERROR : Name is not same!! [%s] [%s]",rBank.strID,rPacket->cBank.m_strId);
					rBank.wResult = eRBDS_ID_NOT_MISMATCH;
				}else{
					rBank.wResult = g_dsManager.closeBank(&rPacket->cBank);
				}
				user->AddSendPacket((char *)&rBank,rBank.base.wSize);
				break;
			}

			case dWDMSG_UPDATESAVE://WORDL->DB載摯
			{
				WDMSG_UPDATESAVE * rPacket;
				rPacket = (WDMSG_UPDATESAVE *)recvpacket;

				g_dsManager.updateBank(rPacket->strId);
				break;
			}

			case dWDMSG_MT_TIME_START:
			{
				g_isUsePlayerBuffer = 0;
				g_lpCdm->saveAllDataToDB();
				break;
			}

			case dWDMSG_MT_TIME_END:
			{
				g_isUsePlayerBuffer = 1;
				break;
			}

	//早	AVATAR PACKET
	//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//早	GUILD PACKET
			case dWDMSG_GETGUILD:	//	錯萄憮陛  望萄曖 薑爾 蹂и 匙..
			{
				WDMSG_GETGUILD * rPacket;
				rPacket = (WDMSG_GETGUILD *)recvpacket;

				DWMSG_GETGUILD		sendGuildInfo;
				sendGuildInfo.base.set(sizeof(DWMSG_GETGUILD),dDWMSG_GETGUILD);

				GuildListDB		* List = new GuildListDB[dGUILD_MAX_COUNT];
				int l_iListCount = 0;

				g_adoAvatar.queryGetGuild_List(List,l_iListCount);
				g_adoAvatar.queryGetGuild_Data(List);

				int l_iLoopCount = l_iListCount/dGUILD_COUNT_FORPACKET;
				if(l_iListCount%dGUILD_COUNT_FORPACKET>0)	l_iLoopCount++;
				sendGuildInfo.wAllCount = l_iListCount;

				for(int j = 0;j<l_iLoopCount;j++)
				{
					sendGuildInfo.wCount = dGUILD_COUNT_FORPACKET;
					if(l_iListCount%dGUILD_COUNT_FORPACKET>0 && j==(l_iLoopCount-1))
						sendGuildInfo.wCount = l_iListCount%dGUILD_COUNT_FORPACKET;
					sendGuildInfo.base.wSize = sizeof(cMSG_BASE_TYPE_FORDB)+sizeof(WORD)+sizeof(GuildListDB)*sendGuildInfo.wCount;
					memcpy(sendGuildInfo.List,(GuildListDB *)&List[j*dGUILD_COUNT_FORPACKET],sizeof(GuildListDB)*sendGuildInfo.wCount);

					CM_COMPRESS		comp;
					WORD	wCompSize = cSRVUTIL::Compress((unsigned char *)&sendGuildInfo,(unsigned char *)comp.data,sendGuildInfo.base.wSize);
					comp.wSourceSize = sendGuildInfo.base.wSize;
					comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);

					if(wCompSize == 0)
					{
						while(!user->AddSendPacket((char *)&sendGuildInfo,sendGuildInfo.base.wSize))
						{
							Sleep(50);
							if(user->GetIsClose())	break;
						}
					}
					else
					{
						while(!user->AddSendPacket((char *)&comp,comp.wSize))
						{
							Sleep(50);
							if(user->GetIsClose())	break;
						}
					}
					SendPacket(user->GetSerial());
					Sleep(50);
				}

				if	(l_iLoopCount==0)	//	蛔煙脹 望萄陛 橈擊陽...
				{
					sendGuildInfo.wAllCount = 0;
					sendGuildInfo.wCount = 0;
					sendGuildInfo.base.wSize = sizeof(cMSG_BASE_TYPE_FORDB)+sizeof(WORD);

					while(!user->AddSendPacket((char *)&sendGuildInfo,sendGuildInfo.base.wSize))
					{
						Sleep(100);
						if(user->GetIsClose())	break;
					}

					SendPacket(user->GetSerial());
					Sleep(50);
				}

				//	該雖 鼻撮 薑爾 爾頂遽.
				for(int i = 0;i < l_iListCount; i++)
				{
					DWMSG_GUILD_MEMBER		member;
					member.base.set(sizeof(DWMSG_GUILD_MEMBER),dDWMSG_GUILD_MEMBER);
					member.wSerial = List[i].dataColumn.m_wSerial;
					if(i == l_iListCount-1)	member.wIsEnd = 1;
					else					member.wIsEnd = 0;

					g_adoAvatar.queryGetGuild_Member(List[i].dataColumn.m_wSerial,&member);
					CM_COMPRESS		comp;
					WORD	wCompSize = cSRVUTIL::Compress((unsigned char *)&member,(unsigned char *)comp.data,member.base.wSize);
					comp.wSourceSize = member.base.wSize;
					comp.set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);

					if(wCompSize == 0)
					{
						while(!user->AddSendPacket((char *)&member,member.base.wSize))
						{
							Sleep(100);
							if(user->GetIsClose())	
							{
								break;
							}
						}
					}else{
						while(!user->AddSendPacket((char *)&comp,comp.wSize))
						{
							Sleep(100);
							if(user->GetIsClose())	
							{
								break;
							}
						}
					}
//#ifdef _FOR_KOREA
//					printf("SEND INFO : guild [%d] member [%d]",member.wSerial,member.wCount);
//#endif
					SendPacket(user->GetSerial());
					Sleep(50);
				}
				delete [] List;
				List = NULL;
				break;
			}

			case dWDMSG_CREATEGUILD:
			{
				WDMSG_CREATEGUILD * rPacket;
				rPacket = (WDMSG_CREATEGUILD *)recvpacket;

				DWMSG_CREATEGUILD	cGuild;
				cGuild.base.set(sizeof(DWMSG_CREATEGUILD),dDWMSG_CREATEGUILD);
				strcpy(cGuild.strGuildName,rPacket->strName);
				strcpy(cGuild.strMasterName,rPacket->strMasterName);
				strcpy(cGuild.strSubMasterName,rPacket->strSubMasterName);
				cGuild.wGuildSerial = rPacket->wIndex;
				cGuild.dwGameServerSerial = rPacket->dwGameServerSerial;

/*				int i = 0;
				CGuildSkillInfo		_aGuildSkill[dMAX_GUILD_SKILL_COUNT];
				for(i = 0; i < dMAX_GUILD_SKILL_COUNT ; i++)	_aGuildSkill[i].reset();
				CGuildPetInfo		_aGuildPet[dGUILD_PET_TYPE_COUNT];
				for(i = 0; i < dGUILD_PET_TYPE_COUNT ; i++)	_aGuildPet[i].reset();
				CGuildPetInfo		_aGuildGuardian[dGUILD_GUARDIAN_TYPE_COUNT];
				for(i = 0; i < dGUILD_GUARDIAN_TYPE_COUNT ; i++)	_aGuildGuardian[i].reset();
				CGuildObjectInfo	_aGuildObject[dMAX_GUILD_OBJECT_COUNT];
				for(i = 0; i < dMAX_GUILD_OBJECT_COUNT ; i++)	_aGuildObject[i].reset();
				CItemDefine			_aInventory[dGUILD_INVENTORY_SIZE];
				for(i = 0; i < dGUILD_INVENTORY_SIZE ; i++)	_aInventory[i].reset();
				CSellingAgentItem	_aSellingAgentItem[dMAX_GUILD_SELLING_AGENT_ITEM_COUNT];
				for(i = 0; i < dMAX_GUILD_SELLING_AGENT_ITEM_COUNT ; i++)	_aSellingAgentItem[i].m_item.reset();
				CGuildPollInfo		_guildPoll;		_guildPoll.reset();
*/
				cGuild.wResult = g_adoAvatar.CreateGuild(rPacket->wIndex,rPacket->strName,rPacket->strMasterId,rPacket->strMasterName,rPacket->wMasterJob,rPacket->wMasterLevel);
				if(cGuild.wResult == dCREATEGUILD_RESULT_SUCCESS)
				{
					_logf("Create Guild : guild [%s] master[%s] result[%d]",rPacket->strName,rPacket->strMasterName,cGuild.wResult);
				}else{
					_logf("Fail Create Guild : guild [%s] master[%s] result[%d], index = [%d]",rPacket->strName,rPacket->strMasterName,cGuild.wResult,rPacket->wIndex);
				}
				user->AddSendPacket((char *)&cGuild,cGuild.base.wSize);
				//	DB  等檜顫 機等檜おй 匙!!
				break;
			}
			case dWDMSG_DELETEGUILD:
			{
				WDMSG_DELETEGUILD * rPacket;
				rPacket = (WDMSG_DELETEGUILD *)recvpacket;

				DWMSG_DELETEGUILD	delGuild;
				delGuild.base.set(sizeof(DWMSG_DELETEGUILD),dDWMSG_DELETEGUILD);
				delGuild.dwGameServerSerial = rPacket->dwGameServerSerial;
				strcpy(delGuild.strName,rPacket->strName);
				strcpy(delGuild.strRequestMan,rPacket->strRequestMan);
				delGuild.wGuildSerial = rPacket->wIndex;
				delGuild.wResult = g_adoAvatar.DeleteGuild(delGuild.wGuildSerial,delGuild.strName);
				sendLogDeleteGuild(delGuild.strName, delGuild.strRequestMan, rPacket->wReason);
				if(delGuild.dwGameServerSerial != 0xffff)
				{
					_logf("request Delete Guild : [%s]",delGuild.strName);
//					g_dsManager.DeleteGuild(delGuild.strName);
					user->AddSendPacket((char *)&delGuild,delGuild.base.wSize);
				}
				_logf("Delete Guild : guild [%s] master[%s] result[%d]",rPacket->strName,rPacket->strRequestMan,delGuild.dwGameServerSerial);
				break;
			}

			case dWDMSG_JOINGUILD:
			{
				WDMSG_JOINGUILD * rPacket;
				rPacket = (WDMSG_JOINGUILD *)recvpacket;

				DWMSG_JOINGUILD		sPacket;
				sPacket.base.set(sizeof(DWMSG_JOINGUILD),dDWMSG_JOINGUILD);
				sPacket.dwGameServerSerial = rPacket->dwGameServerSerial;
				strcpy(sPacket.strMaster,rPacket->strMaster);
				strcpy(sPacket.strName,rPacket->strName);
				strcpy(sPacket.strTargetName,rPacket->strTargetName);
				sPacket.wIndex = rPacket->wIndex;
				sPacket.wRank = rPacket->wRank;
				sPacket.wResult = g_adoAvatar.JoinGuild(rPacket->wIndex,rPacket->strName,rPacket->strTargetId,rPacket->strTargetName,rPacket->wRank,rPacket->wJob,rPacket->wLevel);
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				_logf("Join Guild : guild[%s] target[%s] result [%d]",rPacket->strName,rPacket->strTargetName,sPacket.wResult);
				break;
			}

			case dWDMSG_EXITGUILD:
			{
				WDMSG_EXITGUILD * rPacket;
				rPacket = (WDMSG_EXITGUILD *)recvpacket;

				DWMSG_EXITGUILD		sPacket;
				sPacket.base.set(sizeof(DWMSG_EXITGUILD),dDWMSG_EXITGUILD);
				sPacket.dwGameServerSerial = rPacket->dwGameServerSerial;
				strcpy(sPacket.strMaster,rPacket->strMaster);
				strcpy(sPacket.strName,rPacket->strName);
				strcpy(sPacket.strTargetName,rPacket->strTargetName);
				sPacket.wIndex = rPacket->wIndex;
				sPacket.wResult = g_adoAvatar.ExitGuild(sPacket.wIndex,sPacket.strName,sPacket.strTargetName);
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				_logf("Exit Guild : guild[%s] target[%s] result [%d]",sPacket.strName,sPacket.strTargetName,sPacket.wResult);
				break;
			}

			case dWDMSG_DELETEGUILDER:
			{
				WDMSG_DELETEGUILDER	* rPacket;
				rPacket = (WDMSG_DELETEGUILDER *)recvpacket;

				WORD ret = g_adoAvatar.ExitGuild(rPacket->wIndex,rPacket->strName,rPacket->strTarget);
/*				switch(ret)
				{
				case 1:	_log("SUCCESS !! Deleted Guild Member [%s]",rPacket->strTarget);	break;
				case 2: _log("FAIL!! Not found Guild [%d,%s]",rPacket->wIndex,rPacket->strName);	break;
				case 3: _log("FAIL!! [%s] is not member of Guild [%d,%s]",rPacket->strTarget,rPacket->wIndex,rPacket->strName);	break;
				}
*/				break;
			}

			case dWDMSG_CHANGERANK:
			{
				WDMSG_CHANGERANK * rPacket;
				rPacket = (WDMSG_CHANGERANK *)recvpacket;

				DWMSG_CHANGERANK		sPacket;
				sPacket.base.set(sizeof(DWMSG_CHANGERANK),dDWMSG_CHANGERANK);
				sPacket.dwGameServerSerial = rPacket->dwGameServerSerial;
				sPacket.wIndex = rPacket->wIndex;
				sPacket.wRank = rPacket->wRank;
				sPacket.wRank2 = rPacket->wRank2;
				sPacket.wType = rPacket->wType;
				strcpy(sPacket.strMaster,rPacket->strMaster);
				strcpy(sPacket.strTarget,rPacket->strTarget);
				switch(rPacket->wType)
				{
				case dGUILD_APPONT:
					if(rPacket->wRank==dGUILD_CLASS_MASTER)
					{
						sPacket.wResult = g_adoAvatar.ChangeRank(rPacket->wIndex,rPacket->strName,rPacket->strMaster,rPacket->wRank2);
						sPacket.wResult = g_adoAvatar.ChangeRank(rPacket->wIndex,rPacket->strName,rPacket->strTarget,rPacket->wRank);
					}else 
						sPacket.wResult = g_adoAvatar.ChangeRank(rPacket->wIndex,rPacket->strName,rPacket->strTarget,rPacket->wRank);
					break;
				case dGUILD_CHANGERANK_MASTER_APPONT_SPECIAL:
					sPacket.wResult = g_adoAvatar.ChangeRank(rPacket->wIndex,rPacket->strName,rPacket->strTarget,rPacket->wRank);
					break;
				case dGUILD_DISMISSAL:
					sPacket.wResult = g_adoAvatar.ChangeRank(rPacket->wIndex,rPacket->strName,rPacket->strTarget,dGUILD_CLASS_MEMBER);
					break;
				default:
					sPacket.wResult = dCHANGERANK_RESULT_FAIL;
					break;
				}
				sendLogChangeGuildRank(rPacket->strName, rPacket->strMaster,rPacket->strTarget,rPacket->wRank,rPacket->wRank2,rPacket->wReason);
//				_logf("Change Guild Rank : guild [%d]  master [%s]  target [%s]  result  %d-> , type %d",sPacket.wIndex,sPacket.strMaster,sPacket.strTarget,sPacket.wResult,sPacket.wType);
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				break;
			}

			case dWDMSG_SAVEGUILDBASE:
			{
				WDMSG_SAVEGUILDBASE		* rPacket;
				rPacket = (WDMSG_SAVEGUILDBASE *)recvpacket;

				g_adoAvatar.SaveGuild_Info(rPacket);
				break;
			}

			case dWDMSG_SAVEGUILD_GUARDIAN:
			{
				WDMSG_SAVEGUILD_GUARDIAN		* rPacket;
				rPacket = (WDMSG_SAVEGUILD_GUARDIAN *)recvpacket;


				g_adoAvatar.SaveGuild_Guardian(rPacket->wIndex,rPacket->strName,(void *)rPacket->m_aGuildGuardian,sizeof(CGuildPetInfo)*dGUILD_GUARDIAN_TYPE_COUNT);
				break;
			}

			case dWDMSG_SAVEGUILD_INVENTORY:
			{
				WDMSG_SAVEGUILD_INVENTORY		* rPacket;
				rPacket = (WDMSG_SAVEGUILD_INVENTORY *)recvpacket;

				g_adoAvatar.SaveGuild_Inventory(rPacket->wIndex,rPacket->strName,(void *)rPacket->m_aInventory,sizeof(CItemDefine)*c_iGuildInventorySize);
				break;
			}
			case dWDMSG_SAVEGUILD_MARK:
			{
//				WDMSG_SAVEGUILD_MARK		* rPacket;
//				rPacket = (WDMSG_SAVEGUILD_MARK *)recvpacket;

//				g_adoAvatar.SaveGuild_Mark(rPacket->wIndex,rPacket->strName,(void *)&rPacket->m_guildMarkInfo,sizeof(CGuildMarkInfo));
				break;
			}
			case dWDMSG_SAVEGUILD_STRUCTURE:
			{
				WDMSG_SAVEGUILD_STRUCTURE		* rPacket;
				rPacket = (WDMSG_SAVEGUILD_STRUCTURE *)recvpacket;

				g_adoAvatar.SaveGuild_Structure(rPacket->wIndex,rPacket->strName,(void *)rPacket->m_aGuildStructure,sizeof(cGuildStructureInfo)*dMAX_GUILD_STRUCTURE_COUNT);
				break;
			}
			case dWDMSG_SAVEGUILD_PET:
			{
				WDMSG_SAVEGUILD_PET		* rPacket;
				rPacket = (WDMSG_SAVEGUILD_PET *)recvpacket;

				g_adoAvatar.SaveGuild_Pet(rPacket->wIndex,rPacket->strName,(void *)rPacket->m_aGuildPet,sizeof(CGuildPetInfo)*dGUILD_PET_TYPE_COUNT);
				break;
			}

			case	dWDMSG_ASK_COMPUTE_DUEL_RANKING	:
			{
				break;
			}
			case	dWDMSG_ASK_DUEL_RANKING_INFO	:
			{
				break;
			}
			case	dWDMSG_ASK_DUEL_RECORD	:
			{
				WDMSG_ASK_DUEL_RECORD		* rPacket	=	(WDMSG_ASK_DUEL_RECORD *)recvpacket;

				DWMSG_DUEL_RECORD	packet;

				packet.base.set(sizeof(packet),dDWMSG_DUEL_RECORD);
				strcpy(packet.strName,rPacket->strName);
				strcpy(packet.strId,rPacket->strId);
				memset(&packet.record,0,sizeof(packet.record));	//	檜薑爾 DB憮 橫 諦 и.

				user->AddSendPacket((char *)&packet,packet.base.wSize);

				break;
			}

			case dWDMSG_SAVEGUILD_SKILL:
			{
				WDMSG_SAVEGUILD_SKILL		* rPacket;
				rPacket = (WDMSG_SAVEGUILD_SKILL *)recvpacket;

				g_adoAvatar.SaveGuild_Skill(rPacket->wIndex,rPacket->strName,rPacket->wGuildPoint,(void *)rPacket->m_aGuildSkill,sizeof(WORD)*dMAX_GUILD_SKILL_COUNT);
				break;
			}
	//早	GUILD PACKET
	//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//早	BUY ITEM PACKET 劃こ祜羲

			case	dG2D_BUY_PREMIUM_ITEM_TO_WEB_DB	:
			{
				if	(l_bIsTestDB)
					break;

				G2D_BUY_PREMIUM_ITEM_TO_WEB_DB	*lpPacket	=	(G2D_BUY_PREMIUM_ITEM_TO_WEB_DB	*)recvpacket;

				break;
			}

			case dGDMSG_BUYLIST://GS->DB尨こ蹈桶
			{
				if	(l_bIsTestDB)
					break;

#ifdef		_FOR_KOREA
				DWORD	startTime = timeGetTime();
				GDMSG_BUYLIST * rPacket;
				rPacket = (GDMSG_BUYLIST *)recvpacket;

				DGMSG_BUYLIST	sPacket;
				sPacket.base.set(sizeof(DGMSG_BUYLIST),dDGMSG_BUYLIST);
				sPacket.dwSerialInServer = rPacket->dwSerialInServer;
				strcpy(sPacket.strId,rPacket->strId);
				cWeb.GetBuyList(sPacket.strId,&sPacket);
				_log("GetBuyList = %d.%03d",(timeGetTime()-startTime)/1000,(timeGetTime()-startTime)%1000);
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
#else
				GDMSG_BUYLIST * rPacket;
				rPacket = (GDMSG_BUYLIST *)recvpacket;

				DGMSG_BUYLIST	sPacket;
				sPacket.base.set(sizeof(DGMSG_BUYLIST),dDGMSG_BUYLIST);
				sPacket.dwSerialInServer = rPacket->dwSerialInServer;
				strcpy(sPacket.strId,rPacket->strId);
				cWeb.GetBuyList(sPacket.strId,&sPacket);
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
#endif
				break;
			}
			case dGDMSG_GET_CARROT_COUNT:
			{
				if (l_bIsTestDB)	break;
#ifdef		_USE_CARROTSHOP
				GDMSG_GET_CARROT_COUNT	* rPacket;
				rPacket = (GDMSG_GET_CARROT_COUNT *)recvpacket;
#ifndef _FOR_THAI
				DGMSG_GET_CARROT_COUNT_RESULT	sPacket;
				sPacket.base.set(sizeof(DGMSG_GET_CARROT_COUNT_RESULT),dDGMSG_GET_CARROT_COUNT_RESULT);
				sPacket.dwSerialInServer = rPacket->dwSerialInServer;
				strcpy(sPacket.strId,rPacket->strId);
#ifdef _OGP_SERVICE
				sPacket.iRemainCarrotCount = cCarrot.GetCarrot_OGP(rPacket->strId, rPacket->strIP, rPacket->ogpServiceKey.m_strToken);
#else
				UINT	uiOldTime	=	timeGetTime();
				sPacket.iRemainCarrotCount = cCarrot.GetCarrot(rPacket->strId);
				int		iDeltaTime	=	timeGetTime()-uiOldTime;
				
				_log("%s GetCarrotCountTime = %d.%03d [count %d]",rPacket->strId,iDeltaTime/1000,iDeltaTime%1000,sPacket.iRemainCarrotCount);
#endif
				if	(sPacket.iRemainCarrotCount	==	-1)
				{	
					sPacket.iRemainCarrotCount	=	 0;
					sPacket.wResult = eGCR_FAILED;
				}
				else
				if	(sPacket.iRemainCarrotCount	==	-10) // 0xff9c == -100
				{	
					sPacket.iRemainCarrotCount	=	0;
					sPacket.wResult				=	eGCR_NOTAGREE;
				}
				else
					sPacket.wResult = eGCR_SUCCESS;
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
#else
				BILL_PACK		sPacket;
				memset(&sPacket,0,sizeof(BILL_PACK));
				sPacket.base.set( sizeof(BILL_PACK), dREQ_GETCASHPOINT);	//	ぬ擊 嫡擊陽 顫殮薑爾 渠и 薯и 餌о 挫 匙!!
				sPacket.dwReqKey = 314000001;
				strcpy(sPacket.szUserID , rPacket->strId);
				sPacket.dwClientIP = rPacket->dwSerialInServer;	//	п渡 憮憮曖 檜樹お 衛葬橡
				sPacket.wClientPort = user->GetSerial();		//	dwClientIP п渡 憮曖 衛葬橡 給溥還 匙!!
				strcpy(sPacket.szRetCode,"0000");
				g_socketPayLetter.SendPacket(( char *)&sPacket,sPacket.base.wSize);
#endif	//	_for_thai
#endif
				break;
			}
			case dGDMSG_USE_CARROT_LOG://GS->DB劃
			{
				if	(l_bIsTestDB)
					break;

#ifndef		_USE_CARROTSHOP
				break;
#endif
				GDMSG_USE_CARROT_LOG	* rPacket;
				rPacket = (GDMSG_USE_CARROT_LOG *)recvpacket;

#ifndef		_FOR_THAI
				DGMSG_USE_CARROT_RESULT	sPacket;

				sPacket.base.set(sizeof(DGMSG_USE_CARROT_RESULT),dDGMSG_USE_CARROT_RESULT);
				sPacket.dwSerialInServer = rPacket->dwSerialInServer;
				strcpy(sPacket.strId,rPacket->strId);
				strcpy(sPacket.strName,rPacket->strName);
				sPacket.itcnt = rPacket->itcnt;
				sPacket.itemno = rPacket->itemno;
				sPacket.wPrice = rPacket->sPrice;

				_log("[CARROT] use request id[%s] name[%s] server[%s] item[%d] count[%d] price[%d] public[%d] zoneSerial[%d]",
					rPacket->strId,rPacket->strName,rPacket->strServerName,rPacket->itemno,rPacket->itcnt,rPacket->sPrice,rPacket->wIsPublicItem,rPacket->dwSerialInServer);

				int iCurrentCarrotCount = cCarrot.GetCarrot(rPacket->strId);

				if	(iCurrentCarrotCount >= 0 && iCurrentCarrotCount < rPacket->sPrice)
				{
					sPacket.crcnt = iCurrentCarrotCount;
					sPacket.wResult = eUCR_LOW_COUNT;
					_log("[CARROT] use low count id[%s] current[%d] price[%d]",rPacket->strId,iCurrentCarrotCount,rPacket->sPrice);
				}
				else
				{
					cWeb.UseCarrot(rPacket,&sPacket);

					if	(sPacket.wResult == eUCR_FAILED && sPacket.crcnt < 0)
						sPacket.crcnt = iCurrentCarrotCount;
				}
				_log("[CARROT] use result id[%s] name[%s] item[%d] count[%d] price[%d] result[%d] remain[%d]",
					sPacket.strId,sPacket.strName,sPacket.itemno,sPacket.itcnt,sPacket.wPrice,sPacket.wResult,sPacket.crcnt);
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
#else
				BILL_PACK		sPacket;
				memset(&sPacket,0,sizeof(BILL_PACK));
				sPacket.base.set( sizeof(BILL_PACK), dREQ_BUYITEM);	//	ぬ擊 嫡擊陽 顫殮薑爾 渠и 薯и 餌о 挫 匙!!
				sPacket.dwReqKey = 314000002;
				strcpy(sPacket.szUserID , rPacket->strId);
				strcpy(sPacket.szActorName , rPacket->strName);
				sPacket.wPresentFlag = 1;
				sPacket.dwReqChargeCashAmt =rPacket->sPrice;
//				sPacket.wItemCategory ;			//	檜匙紫 в蹂 橈.
				sPacket.dwClientIP = rPacket->dwSerialInServer;	//	п渡 憮憮曖 檜樹お 衛葬橡
				sPacket.wClientPort = user->GetSerial();		//	dwClientIP п渡 憮曖 衛葬橡 給溥還 匙!!
				sPacket.dwItemID = rPacket->itemno;
				sPacket.wItemCnt = rPacket->itcnt;
				strcpy(sPacket.szRetCode,"0000");
				g_socketPayLetter.SendPacket(( char *)&sPacket,sPacket.base.wSize);
#endif	//	_for_thai

				break;
			}
			case dGDMSG_USEBUY://GS->DB淏劃
			{
				if	(l_bIsTestDB)
					break;

				GDMSG_USEBUY * rPacket;
				rPacket = (GDMSG_USEBUY *)recvpacket;

				DGMSG_USEBUY	sPacket;

				sPacket.base.set(sizeof(DGMSG_USEBUY),dDGMSG_USEBUY);

				strcpy(sPacket.strId,rPacket->strId);
				strcpy(sPacket.strName,rPacket->strName);

				sPacket.dwAssentSqnList	=	rPacket->dwUseSqnList;
				sPacket.dwSerialInServer=	rPacket->dwSerialInServer;
				sPacket.wCartSlot		=	rPacket->wCartSlot;
				sPacket.wInventorySlot	=	rPacket->wInventorySlot;
				sPacket.wType			=	rPacket->wType;
				
				memcpy(&sPacket.timeEndDate,&rPacket->timeEndDate,sizeof(UTime));

				sPacket.wResult	=	cWeb.UseBuy(rPacket->dwUseSqnList,rPacket->strId,rPacket->strServerName,rPacket->strName,&rPacket->timeEndDate,rPacket->iLev,rPacket->wJob);

				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				//	葬婁 log 望匙!!
				_logf("BUYITEM : result[%d] id[%s] name[%s] sqn[%d]",sPacket.wResult,sPacket.strId,sPacket.strName,sPacket.dwAssentSqnList);

				break;
			}
	//早	BUY ITEM PACKET
	//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

	//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//早	GET PROVISION RANK
#ifdef _USE_PROVISIONRANK
			case dGDMSG_PROVISION_RANK_REQ:
			{
				DGMSG_PROVISION_RANK_RET	sPacket;
				sPacket.base.set(sizeof(DGMSG_PROVISION_RANK_RET),dDGMSG_PROVISION_RANK_RET);
				g_adoAvatar.GetProvisionRank(&sPacket);
				_log(" Provision Rank = %d",sPacket.wCount);
				sPacket.base.wSize = sizeof(cMSG_BASE_TYPE_FORDB)+ sizeof(WORD) + sizeof(stProvRankField)*sPacket.wCount;
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				break;
			}
#endif
	//早	GET PROVISION RANK
	//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

	//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//早	GET PERSONAL DATA
#ifdef _USE_BROADCAST
			case dGDMSG_PERSONALDATA:
			{
				GDMSG_PERSONALDATA * rPacket;
				rPacket = (GDMSG_PERSONALDATA *)recvpacket;

				DGMSG_PERSONALDATA	sPacket;
				sPacket.base.set(sizeof(DGMSG_PERSONALDATA),dDGMSG_PERSONALDATA);
				sPacket.wSerial = rPacket->wSerial;
				sPacket.wIndex = rPacket->wIndex;
				strcpy(sPacket.strId,rPacket->strId);
				strcpy(sPacket.strName,rPacket->strName);
				sPacket.iDBIndex = g_Im.getIndex(rPacket->strName);
				if(sPacket.iDBIndex < 0x7fffffff)
					g_adoAvatar.spPD_GetData(sPacket.iDBIndex,sPacket.strId,sPacket.strName,&sPacket.data);
				else
				{
					_log(" ERROR : Can't find this user [%d][%s][%s]",sPacket.iDBIndex,sPacket.strId,sPacket.strName);
 					//	DB憮 id煎 匐儀п憮 陛螳堅.. 
					_DBINDEXTABLE	avaList;
					g_adoAvatar.GetDBIndex(rPacket->strId,avaList);

					for(int i=0; i<4; i++)
					{
						if(avaList.wAvatarIndex[i]==0xffff)	continue;
						int	iIndex = g_Im.getIndex(avaList.strName[i],rPacket->strId);
						if(iIndex==0x7fffffff)
						{
							DWORD ret = g_Im.add(avaList.dwIndex[i],avaList.strName[i],rPacket->strId,avaList.wAvatarIndex[i]);
							_log(" reindex ADD DBINDEX: [%d] [%s] [%s] [%d], ret %d",avaList.dwIndex[i],avaList.strName[i],rPacket->strId,avaList.wAvatarIndex[i],ret);
						}
					}
					sPacket.iDBIndex = g_Im.getIndex(rPacket->strName);
					if(sPacket.iDBIndex < 0x7fffffff)
					{
						g_adoAvatar.spPD_GetData(sPacket.iDBIndex,sPacket.strId,sPacket.strName,&sPacket.data);
						_logf(" Research [%s] .. Success!!", rPacket->strName);
					}else
					{
						sPacket.iDBIndex = g_adoAvatar.spPD_GetData(-1,sPacket.strId,sPacket.strName,&sPacket.data);
						if(sPacket.iDBIndex > -1)
							_logf(" Research2[%s] .. Success!!", rPacket->strName);
						else
							_logf(" Research [%s] .. Failed!! ret = %d", rPacket->strName,sPacket.iDBIndex);
					}
				}
				user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				break;
			}

			case dGDMSG_SAVE_PERSONALDATA:
			{
				GDMSG_SAVE_PERSONALDATA * rPacket;
				rPacket = (GDMSG_SAVE_PERSONALDATA *)recvpacket;
			
				g_adoAvatar.spPD_SetData(rPacket->iDBIndex,rPacket->strId,rPacket->strName,&rPacket->data);
				break;
			}
#endif
		//早	GET PERSONAL DATA
		//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

		//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		//早	STATISTIC
			case dGDMSG_STAT_GOLD:
			{
				GDMSG_STAT_GOLD * rPacket = (GDMSG_STAT_GOLD *)recvpacket;
				dSTATISTIC.spSend_GOLD(rPacket->strServer , rPacket->strId, rPacket->strName, rPacket->llGoldAll, rPacket->llGettingGold);
				break;
			}

		//早	STATISTIC
		//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

		//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		//早	login/out
			case dLDMSG_LOGINRESET:
				{
					LDMSG_LOGINRESET * rPacket = (LDMSG_LOGINRESET *)recvpacket;
					g_adoAvatar.spLogin_Reset();
				}
				break;
			case dLDMSG_LOGIN:
				{
					LDMSG_LOGIN * rPacket = (LDMSG_LOGIN *)recvpacket;
					g_adoAvatar.spLogin_In(rPacket->strId, rPacket->strName, rPacket->externalType);
				}
				break;
			case dLDMSG_LOGOUT:
				{
					LDMSG_LOGOUT * rPacket = (LDMSG_LOGOUT *)recvpacket;
					g_adoAvatar.spLogin_Out(rPacket->strId);
				}
				break;

		//早	login/out
		//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
			case dCM_DUMMY:
				break;

			default:
			{	//	憲雖 ж朝 ぬ擎 .
//				_log("this packet isn't understand  : [%d] [%d]\n",user->GetSerial(),user->GetRecvBufLength());
				break;
			}
		}

		SendPacket(serial);
	}
}

#ifdef _FOR_THAI
void
PayLetterThread(LPVOID lpParameter)
{
	_log("START PayLetter Thread ...");

	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	memset(recvpacket,0,dMAX_PACKET_LENGTH_SERVER);
	WORD	type=0,recvsize=0;

	while(g_bThreadLoop)
	{
		Sleep(1);

		if	(!g_socketPayLetter.isLoopRecv)
		{
			_log("PayLetter connection error!");

			g_socketPayLetter.reConnect();
			Sleep(3000);
			continue;
		}

		while(g_socketPayLetter.GetRecvPacket(recvpacket))
		{
			recvsize = *(WORD *)recvpacket;
			type = *(WORD *)(recvpacket+2);

			BILL_PACK * rPacket  = (BILL_PACK * )recvpacket;
			CClient * cpUser  = UserManager.GetClientPnt( rPacket->wClientPort);

			if(!cpUser)
			{
				_log(" Pay Letter憮 爾魚 衛葬橡[%d]朝 襄營ж雖 彊朝.",rPacket->wClientPort);
				continue;
			}
			
			switch(type)
			{ 
			case dREQ_GETCASHPOINT:
				{
					DGMSG_GET_CARROT_COUNT_RESULT	sPacket;
					sPacket.base.set(sizeof(DGMSG_GET_CARROT_COUNT_RESULT),dDGMSG_GET_CARROT_COUNT_RESULT);
					sPacket.dwSerialInServer = rPacket->dwClientIP;
					strcpy(sPacket.strId,rPacket->szUserID);
					sPacket.iRemainCarrotCount = rPacket->dwTotalCash;
					sPacket.wResult = eGCR_SUCCESS;
					cpUser->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				}
				break;
			case dREQ_BUYITEM:
				{
					DGMSG_USE_CARROT_RESULT	sPacket;
					sPacket.base.set(sizeof(DGMSG_USE_CARROT_RESULT),dDGMSG_USE_CARROT_RESULT);
					sPacket.dwSerialInServer = rPacket->dwClientIP;
					strcpy(sPacket.strId,rPacket->szUserID);
					strcpy(sPacket.strName,rPacket->szActorName);
					sPacket.itcnt	=	rPacket->wItemCnt;
					sPacket.crcnt	=	rPacket->dwTotalCash;
					sPacket.itemno	=	rPacket->dwItemID;
					sPacket.wPrice	=	rPacket->dwItemUnitPrice;
					
					if	(stricmp(rPacket->szRetCode,"0000")	==	0)
						sPacket.wResult	=	eUCR_SUCCESS;
					else
						sPacket.wResult	=	eUCR_FAILED;

					cpUser->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
				}
				break;
			}
		}

	}
}
#endif

void
EventMngThread(LPVOID lpParameter)
{
	_log("START Event Manager Thread ...");

	cGoldRushDefine		cGoldRush_Event;
	cItemPackInfo		cItemPack_Event;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_EVENT_MGR);

	while(g_bThreadLoop)
	{
		for (int iLoop=6000;iLoop && g_bThreadLoop;--iLoop)	//	1 晦葬濠
		{
			g_lifeCodeMgr.putLifeSign(eLIFE_CODE_EVENT_MGR);
			Sleep(10);
		}

		//	WORLD 憮 瓊.
		CClient * cpWorldClient = NULL;

		cpWorldClient = UserManager.GetWorldSrvPnt();

		//	GoldRush 婦 薑爾 挫.
		if	(l_bIsForcedSettingGoldRushEvent == FALSE && g_adoAvatar.spEvent_GoldRush_Get( &cGoldRush_Event ))	//	腎歷賊..
		{	
			memcpy(&g_cGoldRush_Event,&cGoldRush_Event,sizeof(cGoldRushDefine));
			//	錯萄煎 歎и.
			if(cpWorldClient)
			{
				DWMSG_GR_INFO_RESULT sPacket;
				sPacket.base.set(sizeof(DWMSG_GR_INFO_RESULT),dDWMSG_GR_INFO_RESULT);
				memcpy(&sPacket.begin,&g_cGoldRush_Event,sizeof(cGoldRushDefine));
				cpWorldClient->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
#ifdef _FOR_USA
				_log(" SET GOLDRUSH EVENT : [%d/%d/%d] ~ [%d/%d/%d] end msg [%s]",sPacket.begin.month,sPacket.begin.day,sPacket.begin.year , sPacket.end.month, sPacket.end.day,sPacket.end.year, sPacket.strEndComment);
#endif
			}
		}

		//	ItemPack 薑爾 挫.
		while (g_adoAvatar.spEvent_ItemPack_Get(&cItemPack_Event ))	//	腎歷賊..
		{	
			memcpy(&g_cItemPack_Event[cItemPack_Event.m_wSerial],&cItemPack_Event,sizeof(cItemPackInfo));
			// 錯萄陛 億照 蛔煙ц擊 陽虜 等檜攪 爾魚. 除 爾頂雖 濠.
//			if	(cpWorldClient)
//			{
//				DWMSG_ITEMPACK_RESULT sPacket;
//				sPacket.base.set(sizeof(DWMSG_ITEMPACK_RESULT),dDWMSG_ITEMPACK_RESULT);
//				sPacket.wSerial = cItemPack_Event.m_wSerial;
				// 等檜攪陛 綠歷擊 陽朝 輿雖 堅, dMAX_ITEM_PACK_COUNT廓簞朝 部擊 憲葬晦 嬪п憮 爾頂.
//				memcpy(&sPacket.info,&g_cItemPack_Event[cItemPack_Event.m_wSerial],sizeof(cItemPackInfo));
//				cpWorldClient->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
//			}
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_EVENT_MGR);

	_log("END Event Manager Thread ...");
	return ; 
}

void
AdditionThread(LPVOID lpParameter)
{
	_log("START AdditionThread ...");
	MEMORYSTATUS stat;
	GlobalMemoryStatus (&stat);	
	DWORD	_dwUsingMemSize =	stat.dwTotalPageFile/1024-stat.dwAvailPageFile/1024;
	DWORD	_dwNewMemSize	=	0;
	DWORD	dwSSCheckTime	=	timeGetTime();

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_ADDITIONAL);

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
			a	=	UserManager.Size();
		}

		g_sharedMemory.beginProcess();
		g_lpCdm->update();	//	擬港檜 傘鼠 朝擁 穢
		g_sharedMemory.endProcess();
		
#ifdef dUSE_SERVERSTATUS_REPORT
		if	(l_bIsTestServer == FALSE && timeGetTime() - dwSSCheckTime >= 30000)
		{	//	30蟾 и廓 DB ⑷營 薑爾 偵褐п遽.
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

		if	(g_LogQue.getCount()>10)
		{
			LOG_PACKAGE		package;
			package.base.wType = dLOG_PACKAGE;
			package.base.wSize = 4;
			int curpos = 0;

			while(g_LogQue.getCount())
			{
				ALL_LOGMSG	logMsg;
				memcpy(&logMsg,g_LogQue.popPacket(),sizeof(ALL_LOGMSG));
				memcpy(package.package+curpos,&logMsg,logMsg.base.wSize);
				package.base.wSize+= logMsg.base.wSize;
				curpos += logMsg.base.wSize;
				if(package.base.wSize>=(dPACKAGE_MAXSIZE - sizeof(LOG_ITEM_TRADE)))	break;
			}
//			if(wServerType == 0)
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

				g_socketLog.SendPacket((char *)&package,package.base.wSize);
			}
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_ADDITIONAL);

	_log("END AdditionThread ...");

	return ; 
}

void
UpdateSaveUser(CClient *user,LDMSG_UPDATESAVE_USER *cUsUser)
{
	DLMSG_UPDATESAVE_USER		sUsUser;

	sUsUser.base.set(sizeof(DLMSG_UPDATESAVE_USER),dDLMSG_UPDATESAVE_USER);
	sUsUser.wResult	=	1;

	strcpy(sUsUser.strId,cUsUser->strId);

	g_sharedMemory.beginProcess();
	g_lpCdm->saveMemoryDataToDBById(cUsUser->strId);	//	虜 腎賊 詭葬 氈朝 等檜攪 睡 DB 
	g_sharedMemory.endProcess();

	user->AddSendPacket((char *)&sUsUser,sUsUser.base.wSize);
}

void
UpdateSaveAll(CClient *user,LDMSG_UPDATESAVE *cUpdateSave)
{
}

//////////////////////////////////////////////
//	method for packet method
//////////////////////////////////////////////
void
DisUser(int serial)
{
	if	(serial<0 || serial>=dDB_MAX_USER_COUNT)
		return;

	EnterCriticalSection(&csRemoveUser);

	CClient * user  = UserManager.GetClientPnt(serial);

	if	(!user)
	{
		LeaveCriticalSection(&csRemoveUser);
		return;
	}

	user->PreClose();

	if	(user->GetIsUsed())
	{
		int iCount = 0;

		while(user->rBuffer->GetSize())
		{	
			ProcessRecvPacket(user->GetSerial());
			Sleep(1);	

			iCount++;

			if	(iCount>=100)
				break;
		}

		_log("(%s)%d is exit!!\n",user->GetID(),serial);

		user->SetIsUsed(FALSE);
	}

	UserManager.Del(serial);
	user->CSReset();
	LeaveCriticalSection(&csRemoveUser);
}
