#include	"life_code.h"
#include	"process.h"
#include	"cPACKET_WORLDSERVER.h"
#include	"server.h"
#include	"reg_common.h"

cLifeCode_LoginServerMgr	g_lifeCodeLogServerMgr;
cLifeCode_Mgr				g_lifeCodeMgr;

BOOL						g_bIsUpkeepLifeThread	=	TRUE;
BOOL						g_bIsDisplayLifeSign	=	TRUE;

#define	dRS_SMS_ALARM_LIFE_SIGN	"c:\\_rsLog\\RS_SMS_AlarmAlive.dat"

char	*strServerTypeName[]	=	
{
	"WorldServer",
	"AvatarDBCServer",
	"CommunityServer",
	"LoginServer",
	"UserDBCServer",
	"GameServer00",
	"GameServer01",
	"GameServer02",
	"GameServer03",
	"GameServer04",
	"GameServer05",
	"GameServer06",
	"GameServer07",
	"GameServer08",
	"GameServer09",
};

#define	dDEATH_SIGN_TEXT_FILE	"c:/_rsLog/deathSign.txt"
#define	dCHECKING_MARK_FILE		"c:/_rsLog/nowChecking.dat"

void
InitLifeCode(char *_lpstrIP)
{
	g_lifeCodeMgr.init(eLIFE_CODE_COUNT);

	g_lifeCodeMgr.set(eLIFE_CODE_ACCEPT_LOOP,"eLIFE_CODE_ACCEPT_LOOP");
	g_lifeCodeMgr.set(eLIFE_CODE_ADDITIONAL,"eLIFE_CODE_ADDITIONAL");
	g_lifeCodeMgr.set(eLIFE_CODE_USER_DBC,"eLIFE_CODE_USER_DBC");
	g_lifeCodeMgr.set(eLIFE_CODE_RECEIVE,"eLIFE_CODE_RECEIVE");
	g_lifeCodeMgr.set(eLIFE_CODE_WORKER,"eLIFE_CODE_WORKER");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD0,"eLIFE_CODE_WORLD0");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD1,"eLIFE_CODE_WORLD1");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD2,"eLIFE_CODE_WORLD2");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD3,"eLIFE_CODE_WORLD3");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD4,"eLIFE_CODE_WORLD4");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD5,"eLIFE_CODE_WORLD5");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD6,"eLIFE_CODE_WORLD6");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD7,"eLIFE_CODE_WORLD7");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD8,"eLIFE_CODE_WORLD8");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD9,"eLIFE_CODE_WORLD9");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD10,"eLIFE_CODE_WORLD10");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD11,"eLIFE_CODE_WORLD11");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD12,"eLIFE_CODE_WORLD12");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD13,"eLIFE_CODE_WORLD13");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD14,"eLIFE_CODE_WORLD14");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD15,"eLIFE_CODE_WORLD15");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD16,"eLIFE_CODE_WORLD16");

	strcpy(g_lifeCodeLogServerMgr.m_strIP,_lpstrIP);

	g_lifeCodeLogServerMgr.beginThread();
}

void
LifeSignThread(LPVOID lpParameter)
{
	g_lifeCodeLogServerMgr.runThread();
}

BOOL
cLifeCode_ServerInfo::isLife(int _iCurrentSecod)
{
	if	(m_iStatus	==	eLCS_IncorrectStatusSign)
		return	FALSE;
	
	if	(m_iStatus	!=	eLCS_Life)
		return	TRUE;

	if	(_iCurrentSecod	>=	m_iLastReceiveCodeSecond+60*2)	//	2분 동안 라이프 사인이 안 날아 왔다.	사인은 10초에 한번 받는다.
		return	FALSE;

	return	TRUE;
}

cLifeCode_LoginServerMgr::cLifeCode_LoginServerMgr()
{
	memset(m_strMessage,0,sizeof(m_strMessage));

	char	strLogFolder[512];

	GetCurrentDirectory(512,strLogFolder);

	m_iWorldCount	=	0;

	SetCurrentDirectory("c:\\");
	CreateDirectory("_rsLog",NULL);
	SetCurrentDirectory(strLogFolder);

	DeleteFile(dDEATH_SIGN_TEXT_FILE);
	DeleteFile(dCHECKING_MARK_FILE);

	g_log.setLogFolderName("c:\\_rsLog",TRUE);
}

cLifeCode_LoginServerMgr::~cLifeCode_LoginServerMgr()
{
	m_fileLog.Close();
}

void
cLifeCode_LoginServerMgr::beginThread()
{
	m_bIsUpkeep	=	TRUE;
	m_hLifeSign	=	(HANDLE)_beginthread(LifeSignThread, 0, NULL); 
}

void
cLifeCode_LoginServerMgr::registWorld(char *_lpstrName,int _iGameServerCount)
{
	if	(getWorld(_lpstrName))
		return;

	cLifeCode_WorldInfo	*lpWorld	=	&m_aWorldList[m_iWorldCount];

	strcpy(lpWorld->m_strWorldName,_lpstrName);

	lpWorld->m_iStatus			=	eLCS_Life;
	lpWorld->m_iGameServerCount	=	_iGameServerCount;

	lpWorld->m_aServers[eLC_LoginServer].m_iStatus		=	eLCS_IsNotUse;	//	관리 편하려고 넣어 둔거다.. 로그인 서버는 월드에 상관 없이 하나다
	lpWorld->m_aServers[eLC_UserDBCServer].m_iStatus	=	eLCS_IsNotUse;	//	관리 편하려고 넣어 둔거다.. 계정 DB는 월드에 상관 없이 하나다

	m_iWorldCount++;
}

void
cLifeCode_LoginServerMgr::lifeSign(int _iSign,cLifeCode_ServerInfo *_lpServerInfo,char *_lpstrWorldName,int _iServerType,char *_lpstrIP,char *_lpstrMessage)
{
	if	(!m_bIsUpkeep)
		return;

	if	(_lpstrMessage)
	{
		strcpy(m_strMessage,_lpstrMessage);

		if	(_lpstrWorldName)
		{
			CLOG("deathLog","#receiveSign:%s/%s/%s",_lpServerInfo->m_lpstrWorldName,strServerTypeName[_iServerType],_lpServerInfo->m_strIP,_lpstrMessage);
		}
		else
		{
			CLOG("deathLog","#receiveSign:%s/%s",strServerTypeName[_iServerType],_lpServerInfo->m_strIP,_lpstrMessage);
		}
	}

	if	(_iServerType	==	eLC_World)
	{
		cLifeCode_WorldInfo	*lpWorld	=	getWorld(_lpstrWorldName);

		if	(lpWorld)
		{
			lpWorld->m_iStatus			=	_iSign;

			if	(_iSign	==	eLCS_Begin)
				lpWorld->m_iStatus			=	eLCS_Life;
		}
	}

	strcpy(_lpServerInfo->m_strIP,_lpstrIP);
	_lpServerInfo->m_lpstrWorldName			=	_lpstrWorldName;
	_lpServerInfo->m_iStatus				=	_iSign;
	_lpServerInfo->m_iLastReceiveCodeSecond	=	timeGetTime()/1000;
}

cLifeCode_WorldInfo*
cLifeCode_LoginServerMgr::getWorld(char *_lpstrName)
{
	for (int i=0;i<eLC_ServerCount;i++)
	{
		if	(m_aWorldList[i].m_iStatus	==	eLCS_Sleep)
			continue;

		if	(STRICMP(m_aWorldList[i].m_strWorldName,_lpstrName)==0)
			return	&m_aWorldList[i];
	}

	return	NULL;
}

void
cLifeCode_LoginServerMgr::runThread()
{
	WriteServerStatusToRegistry(eST_LOGIN,eSERVER_STATUS_ON);
	WriteLifeSignToRegistry(eST_LOGIN,1);

	while(m_bIsUpkeep)
	{
		for (int i=0;i<50 && m_bIsUpkeep;i++)
			Sleep(100);

		WriteLifeSignToRegistry(eST_LOGIN,1);

		update();
	}
}

void
cLifeCode_LoginServerMgr::end()
{
	m_bIsUpkeep	=	FALSE;
}

void
cLifeCode_LoginServerMgr::update()
{
	DWORD	dwCurTime	=	timeGetTime();
	int		iCurSecond	=	dwCurTime/1000;
	int		iErrorCount	=	0;

	if	(g_bIsDisplayLifeSign)
		printf("----------------------------------------------------------\n| Current User Count : %d\n| [H]+Enter : Help\n|---\n",m_iUserCount);

#ifdef	_FOR_KOREA
	if	(g_bIsIgnoreSMS_Alarm	==	FALSE)
	{
		if	(IsFile(dRS_SMS_ALARM_LIFE_SIGN))
		{
			printf("| SMS Alarm Status : OK\n");
			DeleteFile(dRS_SMS_ALARM_LIFE_SIGN);

			m_iLastSMS_AliveSec	=	iCurSecond;
		}
		else
		{
			if	(iCurSecond		>=	m_iLastSMS_AliveSec+30)
			{
				SERVERStoWORLD_SIMPLE_WORK	packet;

				packet.base.set(sizeof(packet),dSERVERStoWORLD_SIMPLE_WORK);
				packet.wWork	=	eSWLW_SMS_ALARM_ERROR;

				SendToAllWorld(&packet);
				printf("| SMS Alarm Status : not active!!\n");
			}
			else
				printf("| SMS Alarm Status : hmm.....!!\n");
		}
	}
#endif

	BOOL	bIsIgnoreError=	FALSE;

	if	(IsFile(dCHECKING_MARK_FILE))
		bIsIgnoreError	=	TRUE;

	if	(g_lifeCodeMgr.checkAll(dwCurTime)	==	FALSE)
	{
		if	(bIsIgnoreError == FALSE && !IsFile(dDEATH_SIGN_TEXT_FILE))	//	더이상 처리 안함
		{
			cFILE		file;

			file.Open(dDEATH_SIGN_TEXT_FILE,"wt");
			file.writeStreamCR("death/LoginServer/%s/%s",m_strIP,g_lifeCodeMgr.m_strBadStatusLog);

			file.Close();
		}

		iErrorCount++;

		CLOG("deathLog","death/LoginServer/%s/%s",m_strIP,g_lifeCodeMgr.m_strBadStatusLog);

		if	(g_bIsDisplayLifeSign)
			printf("| LogInServer Status : ERROR!! %s\n",g_lifeCodeMgr.m_strBadStatusLog);
	}
	else
	{
		if	(g_bIsDisplayLifeSign)
			printf("| LogInServer Status : OK\n");
	}

	if	(m_userDbc.m_iStatus	==	eLCS_Sleep)
	{
		if	(g_bIsDisplayLifeSign)
			printf("| UserDBC[%s] Status : not begin!! \n",m_userDbc.m_strIP);
	}
	else
	if	(m_userDbc.m_iStatus	==	eLCS_End)
	{
		if	(g_bIsDisplayLifeSign)
			printf("| UserDBC[%s] Status : end server!! \n",m_userDbc.m_strIP);
	}
	else
	if	(m_userDbc.isLife(iCurSecond) == FALSE)
	{
		iErrorCount++;

		if	(bIsIgnoreError == FALSE && !IsFile(dDEATH_SIGN_TEXT_FILE))	//	더이상 처리 안함
		{
			cFILE		file;

			file.Open(dDEATH_SIGN_TEXT_FILE,"wt");
			file.writeStreamCR("death/UserDBCServer/%s/%s",m_userDbc.m_strIP,m_strMessage);

			file.Close();
		}

		CLOG("deathLog","death/UserDBCServer/%s/%s",m_userDbc.m_strIP,m_strMessage);

		if	(g_bIsDisplayLifeSign)
			printf("| UserDBC[%s] Status : ERROR!! %s\n",m_userDbc.m_strIP,m_strMessage);
	}
	else
	{
		if	(g_bIsDisplayLifeSign)
			printf("| UserDBC[%s] Status : OK!! \n",m_userDbc.m_strIP);
	}

	for (int i=0;i<dWORLD_COUNT_FOR_LIFE_SIGN;i++)
	{
		cLifeCode_WorldInfo	*lpWorldInfo	=	&m_aWorldList[i];

		if	(lpWorldInfo->m_iStatus	==	eLCS_Sleep)
			continue;

		if	(g_bIsDisplayLifeSign)
			printf("| World [%s] Status",lpWorldInfo->m_strWorldName);

		if	(lpWorldInfo->m_iStatus	==	eLCS_End)
		{
			if	(g_bIsDisplayLifeSign)
				printf(" : End\n");
			continue;
		}

		int		iIncorrectStatusServerCount	=	0;
		int		iGameServerCount			=	lpWorldInfo->m_iGameServerCount;
		BOOL	bIsIncorrectWorld			=	FALSE;

		for (int iServer=0;iServer<eLC_ServerCount;iServer++)
		{
			cLifeCode_ServerInfo	*lpServerInfo	=	&lpWorldInfo->m_aServers[iServer];

			if	(lpServerInfo->m_iStatus	==	eLCS_IsNotUse)
				continue;

			if	(lpServerInfo->m_iStatus	==	eLCS_Sleep)
			{
				if	(iServer	<	eLC_GameServer0	+iGameServerCount)
				{
					if	(g_bIsDisplayLifeSign)
						printf("\n|   %s[%s] not begin!!",strServerTypeName[iServer],lpServerInfo->m_strIP);

					iIncorrectStatusServerCount++;
				}

				continue;
			}

			if	(lpServerInfo->m_iStatus	==	eLCS_End)
			{
				if	(iServer	<	eLC_GameServer0	+iGameServerCount)
				{
					if	(g_bIsDisplayLifeSign)
						printf("\n|   %s[%s] end server!!",strServerTypeName[iServer],lpServerInfo->m_strIP);

					iIncorrectStatusServerCount++;
				}

				continue;
			}

			if	(lpServerInfo->isLife(iCurSecond))
				continue;

			if	(iServer	==	eLC_World)
				bIsIncorrectWorld	=	TRUE;

			iErrorCount++;

			if	(g_bIsDisplayLifeSign)
				printf("\n|   %s[%s] ERROR!!",strServerTypeName[iServer],lpServerInfo->m_strIP);

			iIncorrectStatusServerCount++;

			if	(bIsIgnoreError == FALSE)	//	더이상 처리 안함
			{
				BOOL	bIsRequireRemainLog	=	TRUE;

				if	(bIsIncorrectWorld	&&	i != eLC_World)
					bIsRequireRemainLog	=	FALSE;

				if	(bIsRequireRemainLog)
				{
					cFILE		file;

					file.Open(dDEATH_SIGN_TEXT_FILE,"wt");
					file.writeStreamCR("death/%s/%s/%s/%s",lpWorldInfo->m_strWorldName,strServerTypeName[iServer],lpServerInfo->m_strIP,m_strMessage);
					file.Close();
				}
			}

			CLOG("deathLog","death/%s/%s/%s/%s",lpWorldInfo->m_strWorldName,strServerTypeName[iServer],lpServerInfo->m_strIP,m_strMessage);
		}

		if	(g_bIsDisplayLifeSign)
		{
			if	(iIncorrectStatusServerCount	==	0)
				printf(" : OK!!\n",lpWorldInfo->m_strWorldName);
			else
				printf("\n");
		}
	}

	if	(iErrorCount	==	0)
	{
		DeleteFile(dDEATH_SIGN_TEXT_FILE);
		DeleteFile(dCHECKING_MARK_FILE);
	}

	if	(g_bIsDisplayLifeSign)
		printf("+----------------------------------------------------------\n\n");
}