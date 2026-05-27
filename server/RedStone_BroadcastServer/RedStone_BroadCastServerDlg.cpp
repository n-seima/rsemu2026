// RedStone_BroadCastServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RedStone_BroadCastServer.h"
#include "RedStone_BroadCastServerDlg.h"
#include "../buildversion.h"
#include "RSComm.h"
#include "cSRVUTIL.h"
#include "cPACKET_GAMESERVER.h"
#include "cPACKET_WORLDSERVER.h"
#include "cPACKET_DBSERVER.h"
#include "cPACKET_BCSERVER.h"
#include "Client.h"
#include "TemporaryListForEntry.h"
#include "operate_packet.h"
#include "FriendsListUpdater.h"
#include "util.h"
#include "ChattingDataBase.h"
#include "backup.h"
#include "Hardwareinfo.h"
#include "guild.h"
#include "life_code.h"
#include "reg_common.h"

#define dUSE_SERVERSTATUS_REPORT

#ifdef _FOR_THAI
#undef dUSE_SERVERSTATUS_REPORT
#endif

#ifdef dUSE_SERVERSTATUS_REPORT
	#include "cServerStatus.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	서버 전역 변수 및 함수
//	extern
#define dWORKERTHREAD_COUNT		6
#define	dGUILD_LIST_UPDATE_PERIOD	3000		// 길드리스트 업데이트 주기..

BOOL	g_bIsEndDlg	=	FALSE;
BOOL	g_isLoopWT	=	TRUE;


char	CRedStone_BroadCastServerDlg::m_strWorldName[dWORLD_NAME_LENGTH];
char *	CRedStone_BroadCastServerDlg::getWorldName()	{	return m_strWorldName;	}
void	CRedStone_BroadCastServerDlg::setWorldName(char * _strWName)	{	strcpy(m_strWorldName,_strWName); }

DebugTracer		* cLOG;
DebugTracer		* cLOGForLogout;

cSOCKET		l_cSockWorld;
cSOCKET		l_cSockAvaDbc;

CTemporaryListForEntry cTempList;
//CChattingDataBase	g_ChattingDataBase;

HANDLE	l_hWorkerThread[dWORKERTHREAD_COUNT];
HANDLE	l_hAcceptThread;
HANDLE	l_hAdditionThread;
HANDLE	l_hFrientsListUpdateThread;
HANDLE	l_hWorldThread;
HANDLE	l_hAvaDBThread;
HANDLE	l_hChattingDBThread;
HANDLE	l_hLifeCodeThread	=	NULL;
HANDLE	l_hGuildMemberListUpdateThread;		// 길드원 멤버 쓰레드..

static int isLocalOpen;

void WorkerThread(void * _pParam); 
void AcceptThread(void * _pParam); 
void AdditionThread(void * _pParam); 
void ChattingDBThread(void * _pParam);
void LifeCodeUpdateThread(LPVOID lpParameter);
void FrientsListUpdateThread(void * _pParam);
void GuildListUpdate();
void DisUser(WORD _wIdx, char * _strWhy);
void ProcessRecvPacket(WORD _wSerial);
void ProcessRegist(CClient * _cpUser, GB_REG_CLIENT * _rPacket);
void GuildMemberListUpdateThread(void * _pParam);		// 길드원 멤버 쓰레드 함수,.,

void WorldThread(void * _pParam);
void AvaDBThread(void * _pParam);

CRITICAL_SECTION	csRemoveUser;
CRITICAL_SECTION	csConnectionInfo;

CRedStone_BroadCastServerDlg * g_pcMain = NULL;
int	g_iCheckUserConnectionInfo;
void __cdecl extWriteConnectionInfo(char * _strId, char * _strArg,...);

//
/////////////////////////////////////////////////////////////////////////////

void __cdecl extWriteConnectionInfo(char * _strId, char * _strArg,...)
{
	return;

	if	(!g_iCheckUserConnectionInfo)
		return;
	if	(_strId[0] == 0 || strlen(_strId)> dID_LENGTH)
		return ;

	CCritical	CS(&csConnectionInfo);

	TCHAR buffer[2048];
	va_list args;

	va_start( args, _strArg );
	_vstprintf( buffer, _strArg, args );
	va_end( args );

	char fn[1024];
	sprintf(fn,"%s\\CF\\cf_[%s].txt",g_pcMain->m_strMainPath,_strId);

	FILE * fp = fopen(fn,"w+");

	if	(!fp)
		return ;

	fprintf(fp,"%d %d : ",cSRVUTIL::GetTime(),cSRVUTIL::GetTimes());
	fprintf(fp,buffer);

	fclose(fp);	fp = NULL;

	return;
}

/////////////////////////////////////////////////////////////////////////////
// CRedStone_BroadCastServerDlg dialog

BOOL
SendPacketToWS(void *_lpPacket)
{
	if	(!l_cSockWorld.sock)
		return	FALSE;

	return	l_cSockWorld.SendPacket((char *)_lpPacket,((cMSG_BASE_TYPE *)_lpPacket)->wSize);
}

CRedStone_BroadCastServerDlg::CRedStone_BroadCastServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRedStone_BroadCastServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRedStone_BroadCastServerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//	
	m_cpDebuggingDlg=	NULL;
}

void CRedStone_BroadCastServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRedStone_BroadCastServerDlg)
	DDX_Control(pDX, IDC_INWORLDNAME, m_ctlInWorldName);
	DDX_Control(pDX, IDC_USERDATATEXT, m_ctlUserDebugInfo);
	DDX_Control(pDX, ID_START_SERVER, m_btnStartServer);
	DDX_Control(pDX, ID_RELOAD_CONFIG, m_btnReadConfig);
	DDX_Control(pDX, IDC_MAINMESSAGE, m_ctlMainMsg);
	DDX_Control(pDX, IDC_GUILD_MEMBER_LIST_UPDATE_PERIOD, m_ctlGuildListUpdatePeriod);
	DDX_Control(pDX, IDC_IMMEDIATELY_UPDATE, m_ctlGuildListImmediatelyUpdate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRedStone_BroadCastServerDlg, CDialog)
	//{{AFX_MSG_MAP(CRedStone_BroadCastServerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_RELOAD_CONFIG, OnReloadConfig)
	ON_BN_CLICKED(ID_START_SERVER, OnStartServer)
	ON_BN_CLICKED(IDC_DEBUGGING, OnDebugging)
	ON_BN_CLICKED(IDC_GUILD_MEMBER_LIST_UPDATE_APPLY, OnGuildListUpdatePeriod)
	ON_BN_CLICKED(IDC_IMMEDIATELY_UPDATE_CHANGE, OnGuildListImmediatelyUpdateChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRedStone_BroadCastServerDlg message handlers

BOOL CRedStone_BroadCastServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	WriteServerInfoToRegistry(eST_COMMUNITY,"community server");

#ifdef dUSE_SERVERSTATUS_REPORT
// 	if(!g_SS.init("WORLDNAME", "CSServer"))
// 	{
// 		MessageBox("ERROR IN SS","WARNING!!",MB_OK);
// 		return FALSE;
// 	}
#endif

	{
		char	strBL[512];
		CreateDirectory("binary_log",NULL);

		sprintf(strBL,"%s/binary_log/bl",g_strLogFolder);

		g_binaryLog.init(strBL);
	}

#ifndef _FOR_KOREA
	::ShowWindow( ::GetDlgItem(this->m_hWnd,IDC_DEBUGGING),SW_HIDE);
#endif

	char	strTitle[512];
	sprintf(strTitle,"RS Community Server.build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
	SetWindowText(strTitle);
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
/*	FILE * dbcfp = fopen("ChattingLogDBInfo.bin","rb");
	if(!dbcfp)
	{
		MessageBox("Can not read avatar db connection info");
		return FALSE;
	}
	fread(m_strChatLogDBSource,128,1,dbcfp);
	fread(m_strChatLogDBId,64,1,dbcfp);
	fread(m_strChatLogDBPw,64,1,dbcfp);
	fclose(dbcfp);
*/
	memset(m_strMyIp,0,dIP_SIZE);
	memset(m_strWorldIp,0,dIP_SIZE);
	memset(m_strAvaDbcIp,0,dIP_SIZE);
	GetCurrentDirectory(dPATH_LENGTH,m_strMainPath);
	sprintf(m_strConfigFile,"%s\\%s",m_strMainPath,dCONFIGFILENAME);

	m_hListenSocket = INVALID_SOCKET;
	
	cLOG = new DebugTracer(DTType_File,"RS_BroadCast_MainMsg");
	SetCurrentDirectory(m_strMainPath);
	cLOGForLogout = new DebugTracer(DTType_File,"RS_BroadCast_MainMsgForLogOut");
	SetCurrentDirectory(m_strMainPath);

	InitializeCriticalSection(&m_stMainMsgCS);
	InitializeCriticalSection(&csRemoveUser);
	InitializeCriticalSection(&csConnectionInfo);

	SetCurrentDirectory(m_strMainPath);
	char CFDirName[1024];
	sprintf(CFDirName,"%s/CF",m_strMainPath);
	CreateDirectory(CFDirName,NULL);
	SetCurrentDirectory(m_strMainPath);

	if(!readConfigFile())
	{
		PrintMainMessage("Fail to initialize BC Server !!");
		return FALSE;
	}

	WSADATA wsadata;
	if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
	{	MessageBox("Fail WSAStartup !!");
		return FALSE;
	}

	isLocalOpen = atoi(m_cScp.getData("OPENFORLOCAL"));
	//	Get My IP
	char strName[32];
	gethostname(strName,sizeof(strName));
	struct hostent* he = gethostbyname(strName);
	struct in_addr iaddr;
	memset(&iaddr,0,sizeof(iaddr));

	for(int i =0; he->h_addr_list[i] !=0; i++)
	{
		if	(	he->h_addrtype!=AF_INET)
			continue;

		memcpy(&iaddr,he->h_addr_list[i],sizeof(iaddr));
		strcpy(m_strMyIp,inet_ntoa(iaddr));

		if	(isLocalOpen)
		{
			if (strncmp(m_strMyIp,"19",2)!=0 && strncmp(m_strMyIp,"17",2)!=0 && strncmp(m_strMyIp,"10",2)!=0)
				continue;
		}
		else
		{
			if (strncmp(m_strMyIp,"19",2)==0 || strncmp(m_strMyIp,"17",2)==0 || strncmp(m_strMyIp,"10",2)==0)
				continue;
		}

		_log("Open addr  = %s\n",m_strMyIp);
		break;
	}

	BOOL ret = TRUE;

	//	Create CompletionPort
	if	(!initCompletionPort())
	{	
		PrintMainMessage("Fail to initialize IOCP Port !!");
		ret = FALSE;
	}

	m_iWorkerThreadCount = 0;
	g_isLoopWT = FALSE;

	m_isRunEndFunc = TRUE;

	g_pcMain = this;
	m_iGuildListUpdatePeriod	=	dGUILD_LIST_UPDATE_PERIOD;
	m_bIsGuildListImmediatelyUpdate	=	TRUE;
	SetDlgItemInt(IDC_IMMEDIATELY_UPDATE,m_bIsGuildListImmediatelyUpdate);
	SetDlgItemInt(IDC_GUILD_MEMBER_LIST_UPDATE_PERIOD,m_iGuildListUpdatePeriod);
	OnStartServer();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRedStone_BroadCastServerDlg::OnPaint() 
{
	if	(IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRedStone_BroadCastServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CRedStone_BroadCastServerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

void CRedStone_BroadCastServerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	DeleteCriticalSection(&m_stMainMsgCS);
	DeleteCriticalSection(&csRemoveUser);
	DeleteCriticalSection(&csConnectionInfo);

	if(m_hListenSocket)
	{
		shutdown(m_hListenSocket,1);
		closesocket(m_hListenSocket);
	}

	cLOG->close();
	cLOGForLogout->close();
}

void __cdecl CRedStone_BroadCastServerDlg::PrintMainMessage(char * _strMsg, ...)
{
	if	(g_bIsEndDlg)
		return;

	CCritical	cs(&m_stMainMsgCS);

	try
	{
		char strBuffer[2048];
		char strBuffer2[2048];

		va_list args;
		va_start( args, _strMsg );
		_vstprintf( strBuffer, _strMsg, args );
		va_end( args );

		sprintf(strBuffer2,"[%d,%d]%s",cSRVUTIL::GetTime(),cSRVUTIL::GetTimes(),strBuffer);

		int	iIndex	=	m_ctlMainMsg.AddString(strBuffer2);	//	Insert String이 너무 느려서 변경함

		m_ctlMainMsg.SetTopIndex(iIndex);

	}
	catch(...)
	{
		int	iIndex	=	m_ctlMainMsg.InsertString(0,"error in PrintMainMessage");
		m_ctlMainMsg.SetTopIndex(iIndex);
	}
}

BOOL CRedStone_BroadCastServerDlg::readConfigFile()	//	config-bc.txt를 읽는다.
{
	if(m_cScp.Read(m_strConfigFile)<0)
	{
		PrintMainMessage("Can't find this file [%s]",m_strConfigFile);
		return FALSE;
	}

	char	*lpstrWorldIP	=	m_cScp.getData(dWORLDINFO);

	if	(lpstrWorldIP)
		WriteWorldServerIPToRegistry(eST_COMMUNITY,lpstrWorldIP);

	return TRUE;
}

BOOL
CRedStone_BroadCastServerDlg::initCompletionPort()
{
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,0);

	if	(m_hCompletionPort==NULL)
		return FALSE;
	return TRUE;
}

BOOL
CRedStone_BroadCastServerDlg::initServer()		//	기본적인 초기화를 진행한다. (가동시 한번만..)
{
	if	(m_hListenSocket)
	{	
		shutdown(m_hListenSocket,1);
		closesocket(m_hListenSocket);
		m_hListenSocket = INVALID_SOCKET;
	}

	if	((m_hListenSocket = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		PrintMainMessage("Fail to create listen socket");
		return FALSE;
	}

	SOCKADDR_IN strSrvAddr;
	memset(&strSrvAddr,0,sizeof(strSrvAddr));
	strSrvAddr.sin_addr.s_addr	=	inet_addr(m_strMyIp);
	strSrvAddr.sin_family		=	AF_INET;
	strSrvAddr.sin_port			=	htons(dBROADCAST_PORT);

	if	(bind(m_hListenSocket,(LPSOCKADDR)&strSrvAddr,sizeof(strSrvAddr)) == SOCKET_ERROR)
	{	
		PrintMainMessage("Fail to bind");
		return FALSE;
	}

	listen(m_hListenSocket,5);

	return TRUE;
}

BOOL
CRedStone_BroadCastServerDlg::Start()		//	사용할 소켓들을 모두 가동한다.
{
	if	(!initServer())
	{	
		PrintMainMessage("Fail to initialize BC Server !!");
		return FALSE;
	}

	g_iCheckUserConnectionInfo = atoi(m_cScp.getData("isWriteUserConnectionInfo"));

	//	AVATAR DB CACHE 연결 
	if	(!l_cSockAvaDbc.init(dDBCACHE_PORT,m_cScp.getData(dAVATARDBCINFO),"AVATAR DBC",sizeof(cMSG_BASE_TYPE_FORDB)))
	{
		PrintMainMessage("Fail to connect AVATAR DB CACHE SERVER!!");
		return FALSE;
	}
	//	Avatar DB Cache에 연결되면 자신을 알린다.
	{
		DDMSG_IAM sPacket;
		sPacket.base.set(sizeof(DDMSG_IAM),dDDMSG_IAM);
		sPacket.iIam = 2;
		l_cSockAvaDbc.SendPacket((char *)&sPacket,sPacket.base.wSize);
	}

	g_cpAvatarDBC	=	&l_cSockAvaDbc;

	{
		char	*lpstrIsTestServer	=	m_cScp.getData("isTestServer");

		if	(stricmp(lpstrIsTestServer,"1")	==	0)
			g_bIsTestServer	=	TRUE;			
	}

	//	WORLD 연결
	if	(!l_cSockWorld.init(dWORLDSERVER_PORT,m_cScp.getData(dWORLDINFO),"WORLD",sizeof(cMSG_BASE_TYPE_FORWORLD)))
	{
		l_cSockAvaDbc.kill();
		PrintMainMessage("Fail to connect WORLD SERVER!!");
		return FALSE;
	}

	//	Worker Thread 를 가동한다.
	g_isLoopWT = TRUE;
	m_iWorkerThreadCount = dWORKERTHREAD_COUNT;

	for(int i=0;i<(int)m_iWorkerThreadCount;i++)
	{
		l_hWorkerThread[i] = (HANDLE)_beginthread(WorkerThread, 0,(LPVOID)this); 
		if(!l_hWorkerThread[i])
		{
			l_cSockAvaDbc.kill();
			l_cSockWorld.kill();
			g_isLoopWT = FALSE;
			PrintMainMessage("Fail to start Worker Thread [%dnd]!!",i+1);
			return FALSE;
		}
	}

	//	AcceptThread 를 가동한다.
	{
		l_hAcceptThread=(HANDLE)_beginthread(AcceptThread, 0,(LPVOID)this); 
		if(!l_hAcceptThread)
		{
			l_cSockAvaDbc.kill();
			l_cSockWorld.kill();
			g_isLoopWT = FALSE;
			PrintMainMessage("Fail to start AcceptThread !!");
			return FALSE;
		}
	}

	{
		l_hLifeCodeThread	=	(HANDLE)_beginthread( LifeCodeUpdateThread, 0,(LPVOID)this); 

		if	(!l_hLifeCodeThread)
		{
			l_cSockAvaDbc.kill();
			l_cSockWorld.kill();
			g_isLoopWT	=	FALSE;
			PrintMainMessage("Fail to start LifeCodeThread !!");
			return FALSE;
		}
	}

	{
		l_hAdditionThread = (HANDLE)_beginthread(AdditionThread, 0,(LPVOID)this); 
		if	(!l_hAdditionThread)
		{
			l_cSockAvaDbc.kill();
			l_cSockWorld.kill();
			g_isLoopWT = FALSE;
			PrintMainMessage("Fail to start AdditionThread !!");
			return FALSE;
		}
	}

	{
		l_hFrientsListUpdateThread = (HANDLE)_beginthread(FrientsListUpdateThread, 0,(LPVOID)this); 
		if(!l_hFrientsListUpdateThread)
		{
			l_cSockAvaDbc.kill();
			l_cSockWorld.kill();
			g_isLoopWT = FALSE;
			PrintMainMessage("Fail to start FrientsListUpdateThread !!");
			return FALSE;
		}
	}
	{	// 길드 멤버 리스트 업데이트
		l_hGuildMemberListUpdateThread = (HANDLE)_beginthread(GuildMemberListUpdateThread, 0,(LPVOID)this); 
		if(!l_hGuildMemberListUpdateThread)
		{
			l_cSockAvaDbc.kill();
			l_cSockWorld.kill();
			g_isLoopWT = FALSE;
			PrintMainMessage("Fail to start GuildMemberListUpdateThread !!");
			return FALSE;
		}
	}
	//	World 서버의 패킷 처리 Thread를 가동한다.
	{
		l_hWorldThread=(HANDLE)_beginthread(WorldThread, 0,(LPVOID)this); 
		if(!l_hWorldThread)
		{
			l_cSockAvaDbc.kill();
			l_cSockWorld.kill();
			g_isLoopWT = FALSE;
			PrintMainMessage("Fail to start WorldThread!!");
			return FALSE;
		}
	}

	//	Avatar 서버의 패킷 처리 Thread를 가동한다.
	{
		l_hAvaDBThread=(HANDLE)_beginthread(AvaDBThread, 0,(LPVOID)this); 
		if(!l_hAvaDBThread)
		{
			l_cSockAvaDbc.kill();
			l_cSockWorld.kill();
			g_isLoopWT = FALSE;
			PrintMainMessage("Fail to start AvaDBThread!!");
			return FALSE;
		}
	}
	
	//	Chatting 로그를 저장하기 위한 Thread를 가동한다.
	{
/*		if(!g_ChattingDataBase.Connect(m_cScp.getData(dCHATDBINFO),m_strChatLogDBId,m_strChatLogDBPw,m_strChatLogDBSource,1))
		{
			l_cSockAvaDbc.kill();
			l_cSockWorld.kill();
			g_isLoopWT = FALSE;
			PrintMainMessage("Fail to start ChattingDBThread!!");
			return FALSE;
		}

		l_hChattingDBThread = (HANDLE)_beginthread(ChattingDBThread, 0,(LPVOID)this); 
		if(!l_hChattingDBThread)
		{
			l_cSockAvaDbc.kill();
			l_cSockWorld.kill();
			g_ChattingDataBase.Close();
			g_isLoopWT = FALSE;
			PrintMainMessage("Fail to start ChattingDBThread!!");
			return FALSE;
		}
*/
	}
	
	PrintMainMessage("Complete to Connect to another server!");

	WriteServerStatusToRegistry(eST_COMMUNITY,eSERVER_STATUS_ON);

	return TRUE;
}

void CRedStone_BroadCastServerDlg::OnReloadConfig() 
{
	// TODO: Add your control notification handler code here
	if	(!readConfigFile())
	{
		MessageBox(" Fail to read the config file. ");
		return;
	}
}

void WorkerThread(void * _pParam)
{
	CRedStone_BroadCastServerDlg * pcMain = (CRedStone_BroadCastServerDlg *)_pParam;

	LP_IO_DATA	lpOV;
	DWORD dwRead,dwIOCPKey;

	BOOL retVal = TRUE;

	while(g_isLoopWT)
	{
		dwIOCPKey = 0xffffffff;

		g_lifeCodeMgr.endCheck(eLIFE_CODE_WORKER);

		retVal	=	GetQueuedCompletionStatus(pcMain->m_hCompletionPort,&dwRead,&dwIOCPKey,(LPOVERLAPPED *)&lpOV,5000);

		while(!g_isLoopWT)
			break;

		g_lifeCodeMgr.beginCheck(eLIFE_CODE_WORKER);

		if	( retVal && dwRead != 0)
		{
			CClient	* user = UserManager.GetClientPnt((WORD)dwIOCPKey);

			if	(!user)
				continue;	

			if	(lpOV->IOState == 0)
			{
				user->AddRecvPacket(lpOV->buf.buf,dwRead);
				RecvPacket(dwIOCPKey);
				ProcessRecvPacket((WORD)dwIOCPKey);
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
		}else
		{
			if	(dwRead == 0 )
			{
				if	(dwIOCPKey==0xffffffff)
					continue;

				DisUser((WORD)dwIOCPKey,"dwRead = 0");
			}
			else
			{
				DWORD dwError = GetLastError();

				if	(dwError != ERROR_IO_PENDING && dwError != 258/*대기시간 초과 , GetQueuedCompletionStatus의 마지막 옵션.*/)
				{
					pcMain->PrintMainMessage("Worker thread error GetQueueCompletionStatus... error [%d]",dwError);
					DisUser((WORD)dwIOCPKey,"GetQueuedCompletionStatus ERROR");
				}
			}
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_WORKER);
}

void AcceptThread(void * _pParam)
{
	CRedStone_BroadCastServerDlg * pcMain = (CRedStone_BroadCastServerDlg *)_pParam;
	
	DWORD	dwFlags = 0;
	SOCKET	hClientsock;
	WORD	wIndex=0;

	while(g_isLoopWT)
	{
		Sleep(1);

		g_lifeCodeMgr.endCheck(eLIFE_CODE_ACCEPT_LOOP);

		hClientsock = accept(pcMain->m_hListenSocket,NULL,NULL);

		if	(hClientsock ==	 INVALID_SOCKET)
			continue;

		if	( !g_isLoopWT )
			continue;

		int buf	=	dMAX_PACKET_LENGTH_SERVER;

		if	(SOCKET_ERROR ==setsockopt(hClientsock,SOL_SOCKET ,SO_SNDBUF,(const char*)&buf,sizeof(buf)))
		{
			shutdown(hClientsock,1);
			closesocket(hClientsock);
			hClientsock = 0;
			continue;
		}
		if	(SOCKET_ERROR ==setsockopt(hClientsock,SOL_SOCKET ,SO_RCVBUF,(const char*)&buf,sizeof(buf)))
		{
			shutdown(hClientsock,1);
			closesocket(hClientsock);
			hClientsock = 0;
			continue;
		}

		LINGER	lingerStruct;
		
		lingerStruct.l_onoff = 1;
		lingerStruct.l_linger = 0; 

		if	(SOCKET_ERROR ==setsockopt(hClientsock, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct)))
		{
			shutdown(hClientsock,1);
			closesocket(hClientsock);
			hClientsock = 0;
			continue;
		}

		g_lifeCodeMgr.beginCheck(eLIFE_CODE_ACCEPT_LOOP);

		wIndex = UserManager.Add();

		if	(wIndex==0xffff)
		{
			shutdown(hClientsock,1);
			closesocket(hClientsock); 
			_log(" ERROR IN CLIENT ADD !!");
			continue;
		}

		CClient *newUser = UserManager.GetClientPnt(wIndex);

		SOCKADDR_IN	saddr;		
		memset(&saddr,0,sizeof(saddr));
		int tmp = sizeof(sockaddr);
		getpeername(hClientsock,(SOCKADDR *)&saddr,&tmp);
		newUser->Open(wIndex,hClientsock,inet_ntoa(saddr.sin_addr));

		if	(!CreateIoCompletionPort((HANDLE)newUser->m_hSock,pcMain->m_hCompletionPort,(DWORD)wIndex,0))
		{
			DWORD error = WSAGetLastError();
			DisUser(wIndex,"Fail to CreateIoCompletionPort");
			continue;
		}
		RecvPacket(wIndex);
	}

	return;
}

void
LifeCodeUpdateThread(LPVOID lpParameter)
{
	CRedStone_BroadCastServerDlg * pcMain = (CRedStone_BroadCastServerDlg *)lpParameter;

	//	로그인 서버 들어 올때까지 루프 돈다~
	while(g_isLoopWT)
	{
		Sleep(10);

		if	(SendWPLifeCode(eBeginLifeSign,pcMain->getMyIp()))
			break;
	}

	while(1)
	{
		for (int iLoop=50;iLoop ;--iLoop)	//	5 초간 기다리는데 그 와중에 쓰레드가 끝나면..
		{
			Sleep(100);

			if	(g_isLoopWT	==	FALSE)
			{
				SendWPLifeCode(eEndLifeSign,pcMain->getMyIp());

				return;
			}
		}

		WriteLifeSignToRegistry(eST_COMMUNITY,1);

		if	(!g_lifeCodeMgr.checkAll(timeGetTime()))
			SendWPLifeCode(eIncorrectStatusSign,pcMain->getMyIp(),g_lifeCodeMgr.m_strBadStatusLog);
		else
			SendWPLifeCode(eUpdateLifeSign,pcMain->getMyIp());
	}
}

void AdditionThread(void * _pParam)
{
	CRedStone_BroadCastServerDlg * pcMain = (CRedStone_BroadCastServerDlg *)_pParam;
	DWORD	dwCheckTime = timeGetTime();
	DWORD	dwSSCheckTime = timeGetTime();
	cHardwareInfo	hwInfo;
	
	g_lifeCodeMgr.beginCheck(eLIFE_CODE_ADDITIONAL);

	while(g_isLoopWT)
	{
		Sleep(1);

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_ADDITIONAL);

		for(int z=dBC_MAX_USER_COUNT; z;)
		{
			--z;

			CClient * user = UserManager.GetClientPnt(z);

			if	(!user || user->m_wSerial == 0xffff)
				continue;

			if	(user->m_wIsRequireAddToParty)
				user->setPartySerial();

			SendPacket(user->m_wSerial);
		}

		if	(!l_cSockAvaDbc.sock)
		{
			if	(!l_cSockAvaDbc.init(dDBCACHE_PORT,pcMain->m_cScp.getData(dAVATARDBCINFO),"AVATAR DBC",sizeof(cMSG_BASE_TYPE_FORDB)))
			{
				Sleep(1000);
				continue;
			}
			//	Avatar 서버의 패킷 처리 Thread를 가동한다.
			{
				l_hAvaDBThread=(HANDLE)_beginthread(AvaDBThread, 0,(LPVOID)pcMain); 

				if	(!l_hAvaDBThread)
				{
					l_cSockAvaDbc.kill();
					pcMain->PrintMainMessage("Fail to Restart AvaDBThread !!");
				}
				else
				{
					pcMain->PrintMainMessage("Reconnected to Avatar DB Cache Server!!");
				}
			}
		}

		if	(!l_cSockWorld.sock)
		{
			if	(!l_cSockWorld.init(dWORLDSERVER_PORT,pcMain->m_cScp.getData(dWORLDINFO),"WORLD",sizeof(cMSG_BASE_TYPE_FORWORLD)))
			{
				Sleep(1000);
				continue;
			}
			//	Avatar 서버의 패킷 처리 Thread를 가동한다.
			{
				l_hWorldThread=(HANDLE)_beginthread(WorldThread, 0,(LPVOID)pcMain); 
				if(!l_hWorldThread)
				{
					l_cSockWorld.kill();
					pcMain->PrintMainMessage("Fail to start WorldThread!!");
				}else
				{
					pcMain->PrintMainMessage("Reconnected to World Server!!");
				}
			}
		}

		int iTime = timeGetTime() - dwCheckTime;
		if	(abs(iTime) > 5000 )
		{
			cMSG_BASE_TYPE	sPacket;
			sPacket.set(sizeof(cMSG_BASE_TYPE),dCM_DUMMY);

			if	(l_cSockAvaDbc.sock)
				l_cSockAvaDbc.SendPacket((char *)&sPacket,sPacket.wSize);
			if	(l_cSockWorld.sock)
				l_cSockWorld.SendPacket((char *)&sPacket,sPacket.wSize);

			{	//	화면의 processing data 갱신하기
				char strText[256];
//				sprintf(strText,"All Count : %d / Count In Memory %d / ",g_ChattingDataBase.getAllCount(),g_ChattingDataBase.getListCount());
//				pcMain->m_ctlChatDebugInfo.SetWindowText(strText);

				sprintf(strText,"List Count : %d / Temp List Count %d / ",UserManager.Size(),cTempList.Size());
				pcMain->m_ctlUserDebugInfo.SetWindowText(strText);
			}
		}

#ifdef dUSE_SERVERSTATUS_REPORT
		if	(timeGetTime() - dwSSCheckTime >= 30000)
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
					break;
				}
	
				case eHW_WARNING_OVER_MEMORY_USAGE:
				{
					sprintf(strText,"eHW_WARNING_OVER_MEMORY_USAGE %d/%d ",hwInfo.m_cMemoryInfo.m_lFreeSize,hwInfo.m_cMemoryInfo.m_lFreeSize);
					g_SS.update( eHW_WARNING_OVER_MEMORY_USAGE , 0 , strText);
					break;
				}

				case eHW_WARNING_HAVENOT_FREESIZE:
				{
					sprintf(strText,"eHW_WARNING_HAVENOT_FREESIZE [%s] %u/%u  ",hwInfo.m_dqHDDList.at(hwInfo.m_iErrorHddIndex).m_strHddName , hwInfo.m_dqHDDList.at(hwInfo.m_iErrorHddIndex).m_iFreeMB, hwInfo.m_dqHDDList.at(hwInfo.m_iErrorHddIndex).m_iTotalMB);
					g_SS.update( eHW_WARNING_HAVENOT_FREESIZE , 0 , strText);
					break;
				}
			}
		}
#endif

	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_ADDITIONAL);
	return;
}

void GuildListUpdate()
{
	for(int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		cGuildInfo	*lpGuild	=	g_guildManager.get(i);
		
		if(!lpGuild)
			continue;
		if(lpGuild->m_wActiveMemberCount == 0)
			continue;
		if(lpGuild->m_bIsChangeConnetState)
		{
			lpGuild->sendGuildMemberConnectionList();
			lpGuild->m_bIsChangeConnetState = FALSE;
		}	
	}

}




void GuildMemberListUpdateThread(void * _pParam)
{
	CRedStone_BroadCastServerDlg * pcMain = (CRedStone_BroadCastServerDlg *)_pParam;
	
	g_lifeCodeMgr.beginCheck(eLIFE_CODE_GUILD_MEMBER_LIST_UPDATE);
	
	while(g_isLoopWT)
	{	
		Sleep(1);
		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_GUILD_MEMBER_LIST_UPDATE);
	
		int iIdx;

		BOOL isEmergency = g_cGuildMemberListUpdater.GetNextProcIndex(iIdx);

		if	(iIdx<0)
			continue;

		cGuildInfo	*lpGuild	=	g_guildManager.get(iIdx);
		if(!lpGuild || lpGuild->m_wActiveMemberCount == 0)
			continue;

		if	(timeGetTime() - lpGuild->m_dwGuildMemberListUpdateGap < pcMain->m_iGuildListUpdatePeriod) 
			continue;

		lpGuild->m_dwGuildMemberListUpdateGap	=	timeGetTime();

		GuildListUpdate();

	}
	g_lifeCodeMgr.endCheck(eLIFE_CODE_GUILD_MEMBER_LIST_UPDATE);
}

void FrientsListUpdateThread(void * _pParam)
{
	CRedStone_BroadCastServerDlg * pcMain = (CRedStone_BroadCastServerDlg *)_pParam;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_FRIEND_LIST_UPDATE);

	while(g_isLoopWT)
	{	
		Sleep(1);	

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_FRIEND_LIST_UPDATE);
		
		//GuildListUpdate();

		int iIdx;

		BOOL isEmergency = g_cFriendsListUpdater.GetNextProcIndex(iIdx);

		if	(iIdx<0)
			continue;

		CClient * user = UserManager.GetClientPnt(iIdx);

		if	(!user || user->m_wSerial == 0xffff || user->m_isGameServer)
			continue;

		if	(timeGetTime() - user->m_dwFriendsListUpdateGap < 10000) 
			continue;

		user->m_dwFriendsListUpdateGap	=	timeGetTime();

		if	(user->m_clPersonalData.isChangedFriendListOnlineStatus())
			nsSendToClient::ChangeFriendOnlineStatus(user);

		{
			for(int i=0;i<dMAX_FRIEND_COUNT;i++)
			{
				if	(!user->m_clPersonalData.m_aFriendList[i].m_strName[0])
					break;

				CClient * pFriend = UserManager.GetClientPnt(user->m_clPersonalData.m_aFriendList[i].m_strName,1);

				if	(!pFriend)
					user->m_clPersonalData.updateFriendOnlineStatus(i,false);
				else
					user->m_clPersonalData.updateFriendOnlineStatus(i,true);
			}

			if	(user->m_clPersonalData.isChangedFriendListOnlineStatus())
				nsSendToClient::ChangeFriendOnlineStatus(user);
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_FRIEND_LIST_UPDATE);
}

void DisUser(WORD _wIdx, char * _strWhy)
{
	CCritical	CS(&csRemoveUser);

	cLOGForLogout->output("[%d] -> [%s]",_wIdx,_strWhy);

	CClient	 *user  = UserManager.GetClientPnt(_wIdx);
	if(!user || user->m_wSerial == 0xffff)
		return;

	if	(!user->m_isGameServer)//	유저의 정보를 갱신하자.
	{
		if	(user->m_strId[0]!=0	&&	strcmp(user->m_strId,"")!=0)
		{
			extWriteConnectionInfo(user->m_strId," Disconnect ... \n");
			nsSendToDB::SavePersonalData(user , 1);
			nsSendToClient::SendConnectionInfoToMyFriends(user, FALSE);	// 접속을 종료했다고 친구들에게 보낸다.
		}
	}
	else
	{	//게임서버가 접속을 끊은 거라면..
		g_pcMain->PrintMainMessage("Game Server Logout : [%s,%s]",user->m_strIp,_strWhy);
	}
	cGuildInfo* lpGuild = g_guildManager.get(user->m_wGuildSerial);

	UserManager.Del(_wIdx);
	
	if(lpGuild)
	{	
		if(g_pcMain->m_bIsGuildListImmediatelyUpdate)
			lpGuild->sendGuildMemberConnectionList();
		lpGuild->m_bIsChangeConnetState = TRUE;
	}
}


void ProcessRecvPacket(WORD _wSerial)
{
	CClient	* user = UserManager.GetClientPnt(_wSerial);

	if	(!user || user->m_wSerial == 0xffff)
		return;

	int  iContinuePacketCountPerGameServer=0;
	WORD	type = 0,recvsize = 0;
	char	cRecvpacket[dMAX_PACKET_LENGTH];
	int		iResultGetRecvPacket=0;
	memset(cRecvpacket,0,dMAX_PACKET_LENGTH);

	while(TRUE)
	{
		iContinuePacketCountPerGameServer++;

		if	(iContinuePacketCountPerGameServer>20)
			break;

		iResultGetRecvPacket = user->GetRecvPacket(cRecvpacket);

		if	(iResultGetRecvPacket==0)
			break;

		recvsize = *(WORD *)cRecvpacket;
		type = *(WORD *)(cRecvpacket+2);

		g_binaryLog.add(cRecvpacket,recvsize);
//		_log(">type 0x%x",type);
		switch(type)
		{
		case dCB_ASK_PARTY_PROSPECTUS		:
			nsReceiveFromClient::AskPartyProstpectus(user,(CB_ASK_PARTY_PROSPECTUS *)cRecvpacket);
			break;
		case dCB_ASK_OPEN_PARTY_LIST		:
			nsReceiveFromClient::AskOpenPartyList(user,(CB_ASK_OPEN_PARTY_LIST *)cRecvpacket);
			break;
		case dCB_CHANGE_PARTY_PROSPECTUS	:
			nsReceiveFromClient::ChangePartyProstpectus(user,(CB_CHANGE_PARTY_PROSPECTUS *)cRecvpacket);
			break;
			
		case dCB_UPDATE_USER_ALLINFO	:
		{
			CB_UPDATE_USER_ALLINFO	*lpPacket	=	(CB_UPDATE_USER_ALLINFO *)cRecvpacket;

			memset(lpPacket->strId+dID_LENGTH-2,0,2);

			if	(stricmp(user->m_strId,"")	==	0	||	stricmp(lpPacket->strId,user->m_strId)	!=	0	)
				break;

			nsReceiveFromClient::UpdateUserAllInfoPacket(user,lpPacket);
			break;
		}
		case dCB_CHANGE_USER_SKILL_SETTING		:
			nsReceiveFromClient::ChangeUserSkillSetting(user,(CB_CHANGE_USER_SKILL_SETTING *)cRecvpacket);
			break;

		case dCB_CHANGE_FRIEND_GROUP_NAME	:
			nsReceiveFromClient::ChangeFriendGroupName(user,(CB_CHANGE_FRIEND_GROUP_NAME *)cRecvpacket);
			break;

		case dCB_CHANGE_FRIEND_GROUP		:
			nsReceiveFromClient::ChangeFriendGroup(user,(CB_CHANGE_FRIEND_GROUP *)cRecvpacket);
			break;

		case dGB_PARTY_PACKET_DELIVER	:
			nsReceiveFromClient::PartyPacketDeliver(user,(GB_PARTY_PACKET_DELIVER *)cRecvpacket);
			break;
		case dGB_PACKET_DELIVER	:
			nsReceiveFromClient::PacketDeliver(user,(GB_PACKET_DELIVER *)cRecvpacket);
			break;

		case dGB_ADD_FRIEND:
			nsReceiveFromClient::AddFriend(user,(GB_ADD_FRIEND *)cRecvpacket);
			break;

		case dCB_REMOVE_FRIEND:
			nsReceiveFromClient::RemoveFriend(user,(CB_REMOVE_FRIEND *)cRecvpacket);
			break;

		case dCB_LOGIN		:	//	원래 코드 operate_packet.cpp 파일로 옮김
			nsReceiveFromClient::LogInClient(user,(CB_LOGIN *)cRecvpacket);
			break;
			
		case dGB_GUILD_CHAT	:	//	길드 채팅
			nsReceiveFromClient::GuildChat(user,(GB_GUILD_CHAT *)cRecvpacket);
			break;

		case dGB_PARTY_CHAT	:	//	길드 채팅
			nsReceiveFromClient::PartyChat(user,(GB_PARTY_CHAT *)cRecvpacket);
			break;

		case dGB_FORCED_REMOVE_FRIEND	:
			nsReceiveFromClient::ForcedRemoveFriend(user,(GB_FORCED_REMOVE_FRIEND *)cRecvpacket);
			break;

		case dGB_ASK_ITEM_INFO			:
			nsReceiveFromClient::AskItemInfo(user,(GB_ASK_ITEM_INFO *)cRecvpacket);
			break;

		case dGB_UPDATE_INFO			:
			nsReceiveFromClient::UpdateInfo(user,(GB_UPDATE_INFO *)cRecvpacket);
			break;

		case dGB_GUILD_WORK_BUS			:
			nsReceiveFromClient::OperateGuildWorkBus(user,(GB_GUILD_WORK_BUS *)cRecvpacket);
			break;

		case dGB_LOGIN:
			{
				GB_LOGIN * rPacket;
				rPacket = (GB_LOGIN *)cRecvpacket;

				rPacket->strName[dNAME_LENGTH-1]	=	NULL;
				mRCI();
				strcpy(user->m_strName,rPacket->strName);
				mCRCI();
				user->m_isGameServer = 1;

//				if(!user->m_pclRecvBuffer->ReSize(1048576))
//					g_pcMain->PrintMainMessage("Fail to Resize Recv Buffer");
//				if(!user->m_pclSendBuffer->ReSize(1048576))
//					g_pcMain->PrintMainMessage("Fail to Resize Send Buffer");
				g_pcMain->PrintMainMessage("Game Server Login : ip (%s) name (%s) ",user->m_strIp,user->m_strName);
			}
			break;
		case dGB_REG_CLIENT:
			{
				ProcessRegist(user,(GB_REG_CLIENT *)cRecvpacket);
				break;
			}
			break;
		}

//		_log("type 0x%x <",type);
	}
}

void AvaDBThread(void * _pParam)
{
	CRedStone_BroadCastServerDlg * pcMain = (CRedStone_BroadCastServerDlg *)_pParam;

	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	WORD	type,recvsize;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_AVATAR_DB);

	while(l_cSockAvaDbc.isLoopRecv)
	{
		Sleep(1);

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_AVATAR_DB);

		while(l_cSockAvaDbc.GetRecvPacket(recvpacket))
		{
			recvsize = *(WORD *)recvpacket;
			type = *(WORD *)(recvpacket+2);

			switch(type)
			{
			case dDGMSG_PERSONALDATA:
				{
					DGMSG_PERSONALDATA * rPacket;
					rPacket = (DGMSG_PERSONALDATA *)recvpacket;

					stCLIENTDATA * _stData = cTempList.GetDataPnt(rPacket->wIndex);

					if	(_stData && STRICMP(_stData->strName,rPacket->strName)==0 && rPacket->iDBIndex != 0x7fffffff)
					{
						mRCI();
//						g_bud.checkData(&rPacket->data,_stData->strId,_stData->strName,_stData->wAvatarIndex);
						mRCI();
						_stData->cData.copy(&rPacket->data);
						mCRCI();
						_stData->cData.resetCurrentFriendListOnlineStatus();	//	친구목록의 on/off표시 초기화
						_stData->iDBIndex = rPacket->iDBIndex;
#ifdef _FOR_KOREA
						CheckString(_stData->strId,sizeof(_stData->strId),"dDGMSG_PERSONALDATA",__LINE__);
						CheckString(_stData->strName,sizeof(_stData->strName),"dDGMSG_PERSONALDATA",__LINE__);
#endif
						//	게임서버에게 정상적으로 등록된 것을 보내준다.
						CClient * user  = UserManager.GetClientPnt(rPacket->wSerial);
						if	(user)
						{
							_stData->wMyGSSerial = user->m_wSerial;
							BG_REG_CLIENT_RESULT	sPacket;
							sPacket.base.set(sizeof(BG_REG_CLIENT_RESULT),dBG_REG_CLIENT_RESULT);
							sPacket.wIndex = _stData->wSerial;
							sPacket.wSerial = _stData->wGSSerial;
							user->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
							extWriteConnectionInfo(rPacket->strId,"dDGMSG_PERSONALDATA SendTo %d, idx %d, serial %d \n",_stData->wMyGSSerial, sPacket.wIndex,sPacket.wSerial);
						}else
						{
							cTempList.Del(rPacket->wIndex);	//	게임서버가 죽었다면 이 캐릭터의 정보는 삭제한다.
							extWriteConnectionInfo(rPacket->strId,"dDGMSG_PERSONALDATA Delete %d\n",rPacket->wIndex);
						}
					}else
					{
						extWriteConnectionInfo(rPacket->strId,"dDGMSG_PERSONALDATA Not Found %d %s %s\n",rPacket->wIndex,rPacket->strId,rPacket->strName);
					}
				}
				break;
			}
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_AVATAR_DB);

//	pcMain->PrintMainMessage(" Finish.. Avatar DB Cache Thread ..");
	return;
}

void
WorldThread(void * _pParam)
{
	CRedStone_BroadCastServerDlg * pcMain = (CRedStone_BroadCastServerDlg *)_pParam;

	SERVERStoWORLD_LOGIN	sendLoginPacket;

	mRCI();
	strcpy(sendLoginPacket.strServerName,"CS_SERVER");
	mCRCI();

	sendLoginPacket.wNICount = 0;
	sendLoginPacket.base.set( sizeof(cMSG_BASE_TYPE_FORWORLD) + sizeof(char)*dMAPNAME_LENGTH + sizeof(WORD),dSERVERStoWORLD_LOGIN);
	l_cSockWorld.SendPacket((char *)&sendLoginPacket,sendLoginPacket.base.wSize);

	nsSendToWorld::AskPartyList();

	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	WORD	type,recvsize;

	g_lifeCodeMgr.beginCheck(eLIFE_CODE_WORLD);

	while(l_cSockWorld.isLoopRecv)
	{
		Sleep(1);

		g_lifeCodeMgr.putLifeSign(eLIFE_CODE_WORLD);

		while(l_cSockWorld.GetRecvPacket(recvpacket))
		{
			recvsize = *(WORD *)recvpacket;
			type = *(WORD *)(recvpacket+2);

			g_binaryLog.add(recvpacket,recvsize);

			switch(type)
			{
			case dWORLDtoSERVERS_ALL_MSG:
				{
					WORLDtoSERVERS_ALL_MSG * rPacket;
					rPacket = (WORLDtoSERVERS_ALL_MSG *)recvpacket;

					SG_CHAT		sPacket;
					FillChattingBuffer(&sPacket,"◐관리자◑",rPacket->strMessage);
					sPacket.bf6ChatType	=	eCT_ADMIN;

					for(int i=dBC_MAX_USER_COUNT;i;)
					{
						--i;

						if	(UserManager.User[i].m_wSerial == 0xffff)
							continue;
						if	(UserManager.User[i].m_isGameServer)
							continue;

						UserManager.User[i].AddSendPacket((char *)&sPacket,sPacket.base.wSize);
					}
				}
				break;

			case dWB_GUILD_WORK_BUS			:
				nsReceiveFromClient::OperateGuildWorkBus(NULL,(GB_GUILD_WORK_BUS *)recvpacket);
				break;

			case dWB_REMOVE_USERINFO:
				{
					WB_REMOVE_USERINFO * rPacket;
					rPacket = (WB_REMOVE_USERINFO *)recvpacket;

					if	(strcmp(rPacket->strId,"")==0)
						break;

					CClient * pUser = UserManager.GetClientPnt(rPacket->strId);
					if(pUser && pUser->m_wSerial<0xffff)
					{
//						cLOG->output("중복 유저 탄생 -_- : id[%s] name[%s] sock[%d]",pUser->m_strId,pUser->m_strName,pUser->m_hSock);
						UserManager.Del(pUser->m_wSerial);
					}else
					{
						stCLIENTDATA * pData = cTempList.GetDataPnt(rPacket->strId);
						if(pData && pData->wSerial<0xffff && ((timeGetTime() - pData->dwRegistTime)/1000 > 30) )
						{
//							_log(" 중복  TempList 유저 탄생 -_- : id[%s] name[%s] time [%d]",pData->strId,pData->strName,(timeGetTime() - pData->dwRegistTime)/1000);
							cTempList.Del(pData->wSerial);
						}
					}
				}
				break;

			case	dWB_PARTY_LIST	:
				nsReceiveFromWorld::UpdatePartyList((WB_PARTY_LIST *)recvpacket);
				break;
			case	dWB_UPDATE_PARTY_INFO	:
				nsReceiveFromWorld::UpdatePartyInfo((WB_UPDATE_PARTY_INFO *)recvpacket);
				break;

			case dWB_WORLDNAME:
				{
					WB_WORLDNAME * rPacket = (WB_WORLDNAME *)recvpacket;
					pcMain->setWorldName(rPacket->strWorldName);
					char strWName[128];
					sprintf(strWName," (In \"%s\")",rPacket->strWorldName);
					pcMain->m_ctlInWorldName.SetWindowText(strWName);
				}
				break;
			case dWORLDtoSERVERS_WORLDNAME:
				{
#ifdef dUSE_SERVERSTATUS_REPORT
					WORLDtoSERVERS_WORLDNAME * rPacket = (WORLDtoSERVERS_WORLDNAME *)recvpacket;
					sprintf(g_SS.m_strServerName,"%s_CS", rPacket->strWorldName);
#endif
				}
				break;
			}
		}
	}

	g_lifeCodeMgr.endCheck(eLIFE_CODE_WORLD);

//	pcMain->PrintMainMessage(" Finish.. World Thread ..");
	return ;
}

void CRedStone_BroadCastServerDlg::OnStartServer() 
{
	// TODO: Add your control notification handler code here
	if	(m_isRunEndFunc)
	{
		if	(!Start())
		{
			PrintMainMessage("Fix the problem and  restart.");
		}
		else
		{
			m_btnStartServer.SetWindowText("Stop !!");
			m_isRunEndFunc = FALSE;
		}
	}
	else
	{
		m_btnStartServer.SetWindowText("Start !!");
		End();
	}
}

void CRedStone_BroadCastServerDlg::End()
{
	if	(m_isRunEndFunc)
		return;

	m_isRunEndFunc = TRUE;

	WriteServerStatusToRegistry(eST_COMMUNITY,eSERVER_STATUS_QUIT);

	//	서버를 끄기전에 전 유저의 데이터 잘 저장해주자.
	PrintMainMessage("Saving User's Personal Data.. Wait a minute.");

	for(int i = dBC_MAX_USER_COUNT; i;)
	{
		--i;

		CClient	 *user  = UserManager.GetClientPnt(i);

		if	(!user || user->m_wSerial == 0xffff)
			continue;

		nsSendToDB::SavePersonalData(user , 1);
	}

	PrintMainMessage("Sending Packets.. Wait a sec.");

	//	패킷 전송이 완료되었는지...
	if	(l_cSockAvaDbc.isLoopRecv)
	{
		int count = 0;
		int size = 0;
		while( (l_cSockAvaDbc.sPacketBuf)->GetSize())
		{
			if(size == (l_cSockAvaDbc.sPacketBuf)->GetSize())
			{
				++count;
				if(count==100)	break;
			}else
				count = 0;
			size = (l_cSockAvaDbc.sPacketBuf)->GetSize();
			Sleep(1);
		}
	}

	if	(l_cSockWorld.isLoopRecv)
	{
		int count = 0;
		int size = 0;

		while( (l_cSockWorld.sPacketBuf)->GetSize())
		{
			if(size == (l_cSockAvaDbc.sPacketBuf)->GetSize())
			{
				++count;
				if	(count==100)
					break;
			}
			else
				count = 0;
			size = (l_cSockAvaDbc.sPacketBuf)->GetSize();
			Sleep(1);
		}
	}

	g_isLoopWT	=	0;

	Sleep(1000);

//	::WaitForSingleObject(l_hAcceptThread,INFINITE);
//	::WaitForSingleObject(l_hFrientsListUpdateThread,INFINITE);
//	::WaitForSingleObject(l_hAdditionThread,INFINITE);
//	::WaitForSingleObject(l_hWorldThread,INFINITE);
//	::WaitForSingleObject(l_hAvaDBThread,INFINITE);
//	for(int j=dWORKERTHREAD_COUNT;j;)
//	{	--j;
//		::WaitForSingleObject(l_hWorkerThread[j],INFINITE);
//	}

//	PrintMainMessage("Close all work...");

	CClientManager::GetInstance().Init();

//	PrintMainMessage("Complete");

	l_cSockWorld.kill();
	l_cSockAvaDbc.kill();

	cLOG->outputf("Completed... ");

	g_bIsEndDlg	=	TRUE;

	WriteServerStatusToRegistry(eST_COMMUNITY,eSERVER_STATUS_OFF);

	CDialog::OnOK();
}



////////////////////////////////////////////////////////////////////////////
//
void ProcessRegist(CClient * _cpUser, GB_REG_CLIENT * _rPacket)
{
	stCLIENTDATA	stData;
	
	if	(strlen(_rPacket->strId)	>=	dID_LENGTH)
		return;
	if	(strlen(_rPacket->strName)	>=	dNAME_LENGTH)
		return;

	mRCI();
	strcpy(stData.strId,_rPacket->strId);
	mRCI();
	strcpy(stData.strName,_rPacket->strName);
	mCRCI();

	stData.dwHashIdCode		= cSRVUTIL::GetHashCode((BYTE *)stData.strId);
	stData.dwHashNameCode	= cSRVUTIL::GetHashCode((BYTE *)stData.strName);

	stData.wCurrentLevel	= _rPacket->wLevel;
	stData.wGSSerial		= _rPacket->wSerial;
	stData.wGuildRank		= _rPacket->wGuildRank;
	stData.wGuildSerial		= _rPacket->wGuildSerial;
	stData.wCurrentField	= _rPacket->wCurrentField;
	stData.wOper			= _rPacket->wOper;
	stData.wPartySerial		= _rPacket->wPartySerial;
	stData.wAvatarIndex		= _rPacket->wAvatarIndex;
	stData.iDBIndex			= 0x7fffffff;	//	나중에 DB에서 받아야 한다.
	stData.wMyGSSerial		= _cpUser->m_wSerial;

	//	기존에 정상 등록되어 있는 유저인가?
	WORD	wSerial = 0xffff;
	CClient	* pCheckUser = UserManager.GetClientPnt(stData.strId);

	if	(pCheckUser)
	{
		if	(STRICMP(stData.strName,pCheckUser->m_strName)==0)
		{
			wSerial = 0xfffe;
		}
		else
		{
			_cpUser->m_strId[dID_LENGTH-1]		=	NULL;
			_cpUser->m_strName[dNAME_LENGTH-1]	=	NULL;

			mRCI();
			strcpy(pCheckUser->m_strId,_cpUser->m_strId);
			mRCI();
			strcpy(pCheckUser->m_strName,_cpUser->m_strName);
			mCRCI();
			pCheckUser->m_dwHashIdCode= cSRVUTIL::GetHashCode((BYTE *)pCheckUser->m_strId);
			pCheckUser->m_dwHashNameCode = cSRVUTIL::GetHashCode((BYTE *)pCheckUser->m_strName);
			pCheckUser->m_wMyGSSerial	= _cpUser->m_wSerial;
			pCheckUser->m_wGSSerial		= _rPacket->wSerial;
			pCheckUser->m_wGuildRank	= _rPacket->wGuildRank;

			pCheckUser->setAvatarIndex(_rPacket->wAvatarIndex);
			pCheckUser->setGuildSerial(_rPacket->wGuildSerial);
			pCheckUser->setFieldSerial(_rPacket->wCurrentField);
			pCheckUser->m_wLevel		= _rPacket->wLevel;
			pCheckUser->setPartySerial(_rPacket->wPartySerial);
			pCheckUser->m_wOper			= _rPacket->wOper;
			pCheckUser->m_iDBIndex		= 0x7fffffff;
			wSerial = 0xfffe;
		}
	}else
	{
		//	혹시 templist에 등록되어 있는 유저인가?
		stCLIENTDATA * _stData = cTempList.GetDataPnt(stData.strId);
		if	(!_stData)
		{
			 wSerial = cTempList.Add(stData);
		}else
		{
			wSerial = _stData->wSerial;
			mRCI();
			strcpy(_stData->strId,stData.strId);
			mRCI();
			strcpy(_stData->strName,stData.strName);
			mCRCI();
			_stData->wMyGSSerial	= stData.wMyGSSerial;
			_stData->dwHashIdCode	= cSRVUTIL::GetHashCode((BYTE *)_stData->strId);
			_stData->dwHashNameCode = cSRVUTIL::GetHashCode((BYTE *)_stData->strName);
			_stData->wGSSerial		= stData.wGSSerial;
			_stData->wGuildRank		= stData.wGuildRank;
			_stData->wGuildSerial	= stData.wGuildSerial;
			_stData->wOper			= stData.wOper;
			_stData->iDBIndex		= stData.iDBIndex;
			_stData->wCurrentField	= stData.wCurrentField;
			_stData->wCurrentLevel	= stData.wCurrentLevel;
			_stData->wPartySerial	= stData.wPartySerial;
			_stData->wAvatarIndex	= stData.wAvatarIndex;
		}
	}

	if	(wSerial == 0xffff)	//	fail
	{
		extWriteConnectionInfo(_rPacket->strId,"(ProcessRegist) 0xffff\n");

		BG_REG_CLIENT_RESULT	sPacket;
		sPacket.base.set(sizeof(BG_REG_CLIENT_RESULT),dBG_REG_CLIENT_RESULT);
		sPacket.wIndex = 0xffff;
		sPacket.wSerial = _rPacket->wSerial;
		_cpUser->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
	}else if(wSerial == 0xfffe)
	{
		extWriteConnectionInfo(_rPacket->strId,"(ProcessRegist) 0xfffe , wRealIndex %d, wSerial %d\n",pCheckUser->m_wSerial,_rPacket->wSerial);

		BG_REG_CLIENT_SERIAL	ssPacket;
		ssPacket.base.set(sizeof(BG_REG_CLIENT_SERIAL),dBG_REG_CLIENT_SERIAL);
		ssPacket.wRealIndex = pCheckUser->m_wSerial;
		ssPacket.wSerial = _rPacket->wSerial;
		_cpUser->AddSendPacket((char *)&ssPacket,ssPacket.base.wSize);

		BG_REG_CLIENT_RESULT	sPacket;
		sPacket.base.set(sizeof(BG_REG_CLIENT_RESULT),dBG_REG_CLIENT_RESULT);
		sPacket.wIndex = 0xfffe;
		sPacket.wSerial = _rPacket->wSerial;
		_cpUser->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
	}
	else
	{
		extWriteConnectionInfo(_rPacket->strId,"(ProcessRegist) name [%s] wIndex %d ,wSerial %d\n", _rPacket->strName, wSerial, _cpUser->m_wSerial);
		GDMSG_PERSONALDATA	sPacket;
		sPacket.base.set(sizeof(GDMSG_PERSONALDATA),dGDMSG_PERSONALDATA);
		mRCI();
		strcpy(sPacket.strId,_rPacket->strId);
		mRCI();
		strcpy(sPacket.strName,_rPacket->strName);
		mCRCI();
		sPacket.wIndex = wSerial;//BC에 등록된 유저의 임시 시리얼
		sPacket.wSerial = _cpUser->m_wSerial;//BC에 등록된 게임서버 시리얼, 돌려줄때 사용
		l_cSockAvaDbc.SendPacket((char *)&sPacket,sPacket.base.wSize);
	}
}

void CRedStone_BroadCastServerDlg::OnDebugging() 
{
	// TODO: Add your control notification handler code here
	if(m_cpDebuggingDlg)
	{
		m_cpDebuggingDlg->EndDialog(0);
		delete m_cpDebuggingDlg;
		m_cpDebuggingDlg = NULL;
	}
	m_cpDebuggingDlg = new CDebuggingDialog;
	m_cpDebuggingDlg->Create(IDD_DEBUGGING_DIALOG, GetDesktopWindow( ));				
	m_cpDebuggingDlg->ShowWindow(SW_SHOW);
	m_cpDebuggingDlg->SetActiveWindow();
}

void CRedStone_BroadCastServerDlg::OnOK() 
{
	// TODO: Add extra validation here
	char temp[32];
	m_btnStartServer.GetWindowText(temp,32);

	End();
}
void CRedStone_BroadCastServerDlg::OnGuildListUpdatePeriod() 
{	 
	m_iGuildListUpdatePeriod = GetDlgItemInt(IDC_GUILD_MEMBER_LIST_UPDATE_PERIOD);	
}
void CRedStone_BroadCastServerDlg::OnGuildListImmediatelyUpdateChange() 
{	 
	m_bIsGuildListImmediatelyUpdate = !m_bIsGuildListImmediatelyUpdate;
	SetDlgItemInt(IDC_IMMEDIATELY_UPDATE,m_bIsGuildListImmediatelyUpdate);

}