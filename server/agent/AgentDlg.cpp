// AgentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Agent.h"
#include "AgentDlg.h"
#include "DlgProxy.h"
#include "RSComm.h"
#include "cSRVUTIL.h"
#include "client.h"
#include "cPACKET_AGENT.h"
//#include "GameServerThread.h"
//#include "WorldServerThread.h"
//#include "LogInServerThread.h"
//#include "LogServerThread.h"
//#include "CommunityServerThread.h"
//#include "AvatarDBCServerThread.h"
//#include "UserDBCServerThread.h"
#include "WorkerThread.h"
#include "accept.h"
#include "ReconnectThread.h"
#include "reg_common.h"
#include "server_info.h"
#include "KillProcess.h"
#include "LiveThread.h"


// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

DebugTracer		* cLOG;

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg dialog

IMPLEMENT_DYNAMIC(CAgentDlg, CDialog);

CAgentDlg::CAgentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAgentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAgentDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon			=	AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy	=	NULL;
	m_hListenSocket	=	INVALID_SOCKET;

	m_hCompletionPort=	NULL;
	m_hLogInServerThread=	0;
	m_hListenSocket	=	0;
	m_iServerType	=	-1;
	cLOG			=	new	DebugTracer(DTType_File,"RS_Server_Agent");
}

CAgentDlg::~CAgentDlg()
{
	saveConfig();

	m_sockLogInServer.kill();

	delete cLOG;
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if	(m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;

	DeleteCriticalSection(&m_stMainMsgCS);
	DeleteCriticalSection(&m_csRemoveUser);
}

void CAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgentDlg)
	DDX_Control(pDX, IDC_SERVER_LIST, m_treeServerList);
	DDX_Control(pDX, IDC_SERVER_TYPE, m_cbServerType);
	DDX_Control(pDX, IDC_MAINMESSAGE, m_ctlMainMsg);
	DDX_Text(pDX, IDC_LOGIN_SERVER_IP, m_strLogInServerIP);
	DDX_CBIndex(pDX, IDC_SERVER_TYPE, m_iServerType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAgentDlg, CDialog)
	//{{AFX_MSG_MAP(CAgentDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_CBN_SELCHANGE(IDC_SERVER_TYPE, OnSelchangeServerType)
	ON_EN_CHANGE(IDC_LOGIN_SERVER_IP, OnChangeLoginServerIp)
	ON_BN_CLICKED(IDC_KILL, OnKill)
	ON_BN_CLICKED(IDC_BEGIN_SERVER, OnBeginServer)
	ON_BN_CLICKED(IDC_END_SERVER, OnEndServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg message handlers

void
CAgentDlg::loadConfig()
{
	cFILE	file;

	m_iServerType	=	0;

	m_strLogInServerIP	=	"127.0.0.1";

	if	(!file.Open(_ms("%s/config.dat",g_strLogFolder),"rb"))
		return;

	char	strLogInServerIP[dIP_LENGTH];

	file.Read(strLogInServerIP,sizeof(strLogInServerIP));
	file.Read(&m_iServerType,sizeof(m_iServerType));

	file.Close();

	m_strLogInServerIP	=	strLogInServerIP;
}

void
CAgentDlg::saveConfig()
{
	cFILE	file;

	if	(!file.Open(_ms("%s/config.dat",g_strLogFolder),"wb"))
		return;

	char	strLogInServerIP[dIP_LENGTH];

	strcpy(strLogInServerIP,m_strLogInServerIP);

	file.Write(strLogInServerIP,sizeof(strLogInServerIP));
	file.Write(&m_iServerType,sizeof(m_iServerType));

	file.Close();
}

BOOL CAgentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	loadConfig();

	InitializeCriticalSection(&m_stMainMsgCS);
	InitializeCriticalSection(&m_csRemoveUser);

	for (int i=0;i<dSERVER_TYPE_COUNT;i++)
		m_cbServerType.AddString(g_strServerName[i]);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	UpdateData(FALSE);

	m_hLiveThread	=	(HANDLE)_beginthread(LiveThread, 0,(LPVOID)this); 

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAgentDlg::OnPaint() 
{
	if (IsIconic())
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

void __cdecl CAgentDlg::PrintMainMessage(char * _strMsg, ...)
{
	CCritical	cs(&m_stMainMsgCS);

	char strBuffer[2048];
	char strBuffer2[2048];

	va_list args;
	va_start( args, _strMsg );
	_vstprintf( strBuffer, _strMsg, args );
	va_end( args );

	CTimeInfo	curTime;

	curTime.update();

	sprintf(strBuffer2,"%s/%s",curTime.getTime(FALSE,TRUE,TRUE,TRUE,TRUE,TRUE),strBuffer);

	int	iIndex	=	m_ctlMainMsg.AddString(strBuffer2);
	m_ctlMainMsg.SetTopIndex(iIndex);
}

void
CAgentDlg::DisUser(int	_iIndex,char *_lpstrReason)
{
	CCritical	CS(&m_csRemoveUser);

	CClient		*lpUser  = UserManager.GetClientPnt(_iIndex);

	if	(!lpUser	||	lpUser->m_wSerial == 0xffff)
		return;

	UserManager.Del(_iIndex);
}

BOOL
CAgentDlg::initCompletionPort()
{
	m_hCompletionPort	=	CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,0);

	if	(m_hCompletionPort==NULL)
		return FALSE;

	return TRUE;
}

BOOL
CAgentDlg::getInfo(cAgentServerInfo	*_lpInfo)
{
	char	strName[64];

	if	(!ReadServerNameWithRegistry(m_iServerType,strName,64))
		strcpy(strName,g_strServerName[m_iServerType]);

	_lpInfo->m_wServerType		=	m_iServerType;
	_lpInfo->m_iWorldIndex		=	-1;
	_lpInfo->m_iGameServerIndex	=	-1;
	_lpInfo->m_wServerStatus	=	ReadServerStatusWithRegistry(m_iServerType);
	_lpInfo->m_iServerVersion	=	ReadServerVersionInRegistry(m_iServerType);

	memset(_lpInfo->m_strWorldIP,0,sizeof(_lpInfo->m_strWorldIP));

	if	(m_iServerType	==	eST_GAME	||	m_iServerType	==	eST_COMMUNITY	||		m_iServerType	==	eST_AVATAR_DBC)
		ReadWorldServerIPWithRegistry(m_iServerType,_lpInfo->m_strWorldIP,sizeof(_lpInfo->m_strWorldIP));

	if	(m_iServerType	==	eST_GAME)
		_lpInfo->m_iGameServerIndex	=	ReadGameServerIndexWithRegistry(m_iServerType);

	if	(m_iServerType	==	eST_WORLD)
		_lpInfo->m_iWorldIndex		=	ReadWorldServerIndexWithRegistry(m_iServerType);

	strcpy(_lpInfo->m_strName,strName);
	strcpy(_lpInfo->m_strIP,m_strMyIp);

	return	TRUE;
}

BOOL
CAgentDlg::sendCurrentStatus()
{
	CR_STATUS	packet;

	packet.base.set(sizeof(packet),dCR_STATUS);

	getInfo(&packet.m_serverInfo);

	m_sockLogInServer.SendPacket((char*)&packet,packet.base.wSize);

	return	TRUE;
}

BOOL
CAgentDlg::connectWithLoginServer()
{
// 	if	(m_sockLogInServer.init(dSERVER_AGENT_PORT,m_strLogInServerIP.GetBuffer(0),g_strServerName[m_iServerType],sizeof(cMSG_BASE_TYPE_INNER)))
// 	{
// 		m_hLogInServerThread	=	(HANDLE)_beginthread(LogInServerThread, 0,(LPVOID)this); 
// 
// 		if	(!m_hLogInServerThread)
// 		{
// 			PrintMainMessage("Failed to Root agent agent packet thread");
// 			return	FALSE;
// 		}
// 
// 		sendCurrentStatus();
// 
// 		PrintMainMessage("Begin Root agent packet thread");
// 
// 		return	TRUE;
// 	}

	return	FALSE;
}

BOOL
CAgentDlg::beginThread()
{
	if	(m_iServerType	==	eST_LOGIN)
	{
		for(int i=0;i<dWORKERTHREAD_COUNT;i++)
		{
			m_ahWorkerThread[i]	=	(HANDLE)_beginthread(WorkerThread, 0,(LPVOID)this); 

			if	(!m_ahWorkerThread[i])
			{
				PrintMainMessage("Fail to start Worker Thread [%d]!!",i+1);
				return FALSE;
			}

			PrintMainMessage("Begin worker thread [%d]!!",i+1);
		}

		m_hAcceptThread	=	(HANDLE)_beginthread(AcceptThread, 0,(LPVOID)this); 

		if	(!m_hAcceptThread)
		{
			PrintMainMessage("Failed to start accept thread");

			return	FALSE;
		}

		PrintMainMessage("Begin accept thread");
	}
	else
	{
		m_hReconnectThread	=	(HANDLE)_beginthread(ReconnectThread, 0,(LPVOID)this); 

		if	(!m_hReconnectThread)
		{
			PrintMainMessage("Failed to start reconnect thread");

			return	FALSE;
		}

		PrintMainMessage("Begin reconnect thread");
	}

	return	TRUE;
}

BOOL
CAgentDlg::initNetwork()		//	기본적인 초기화를 진행한다. (가동시 한번만..)
{
	//	Get My IP
	{
		WSADATA wsadata;

		if	(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
		{
			MessageBox("Fail WSAStartup !!");
			return FALSE;
		}

		char strName[32];

		gethostname(strName,sizeof(strName));
		struct hostent* he = gethostbyname(strName);
		struct in_addr iaddr;
		memset(&iaddr,0,sizeof(iaddr));

		for(int i =0; he->h_addr_list[i] !=0; i++)
		{
			if	(he->h_addrtype!=AF_INET)
				continue;

			memcpy(&iaddr,he->h_addr_list[i],sizeof(iaddr));
			strcpy(m_strMyIp,inet_ntoa(iaddr));

			if	(strncmp(m_strMyIp,"19",2)==0 || strncmp(m_strMyIp,"17",2)==0 || strncmp(m_strMyIp,"10",2)==0)
				continue;

			break;
		}
	}

	if	(m_iServerType	==	eST_LOGIN)
	{
		//	호스트 서버에서만 설정 하는 것들
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
		strSrvAddr.sin_port			=	htons(dSERVER_AGENT_PORT);

		if	(bind(m_hListenSocket,(LPSOCKADDR)&strSrvAddr,sizeof(strSrvAddr)) == SOCKET_ERROR)
		{	
			PrintMainMessage("Fail to bind");
			return FALSE;
		}

		listen(m_hListenSocket,5);

		//	Create CompletionPort
		if	(!initCompletionPort())
		{	
			PrintMainMessage("Fail to initialize IOCP Port !!");

			return	FALSE;
		}
	}
	else
	if	(m_iServerType	!=	eST_LOGIN)
	{
		//	나머지는 다 클라이언트다
		if	(!connectWithLoginServer())
			PrintMainMessage("Fail to connect with Root agent server");
	}

	beginThread();

	return TRUE;
}

// The system calls this to obtain the cursor to display while the lpUser drags
//  the minimized window.
HCURSOR CAgentDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a lpUser closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CAgentDlg::OnClose() 
{
	if	(CanExit())
		CDialog::OnClose();
}

void CAgentDlg::OnOK() 
{
	if	(CanExit())
		CDialog::OnOK();
}

void CAgentDlg::OnCancel() 
{
	g_bIsMainLoop	=	FALSE;

	if	(CanExit())
		CDialog::OnCancel();
}

BOOL CAgentDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if	(m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);

		return FALSE;
	}

	return TRUE;
}

BOOL
CAgentDlg::ProcessRecvPacket(DWORD _dwIdx)
{
	CClient	*lpUser = UserManager.GetClientPnt((int)_dwIdx);

	if	(!lpUser || lpUser->m_wSerial == 0xffff)
		return	FALSE;

	WORD	wType = 0,wRecvsize = 0;
	char	aRecvBuffer[dMAX_PACKET_LENGTH];
	int		iResultGetRecvPacket=0;

	while(TRUE)
	{
		iResultGetRecvPacket	=	lpUser->GetRecvPacket(aRecvBuffer);

		if	(iResultGetRecvPacket==0)
			break;

		wRecvsize	=	*(WORD *)aRecvBuffer;
		wType		=	*(WORD *)(aRecvBuffer+2);

		switch(wType)
		{
			case	dCR_STATUS	:
				PrintMainMessage("recv 0x%x",wType);
				g_serverList.updateServerInfo(&((CR_STATUS *)aRecvBuffer)->m_serverInfo,this);

				break;
		}
	}

	return	TRUE;
}

BOOL
CAgentDlg::RecvPacket(DWORD _dwIdx)
{
	CClient	* lpUser = UserManager.GetClientPnt((WORD)_dwIdx);

	if	(!lpUser || lpUser->m_wSerial == 0xffff)
		return FALSE;

	DWORD	lpRecv;
	DWORD	Flags = 0;

	lpUser->m_stRecvOL.IOState = 0;

	if	(WSARecv(lpUser->m_hSock, &lpUser->m_stRecvOL.buf ,1,&lpRecv,&Flags,&lpUser->m_stRecvOL.OL ,NULL) ==SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();

		if	(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			DisUser((WORD)_dwIdx,"recev error");

			return FALSE;
		}
	}

	return TRUE;
}

void
CAgentDlg::OnStart()
{
	ReadAllServerVersionInRegistry();

	int	iStatus	=	ReadServerStatusWithRegistry(m_iServerType);

	m_treeServerList.DeleteAllItems();

	if	(m_iServerType	==	eST_LOGIN)
	{
		m_treeServerList.EnableWindow(TRUE);
		fillTree();
	}
	else
		m_treeServerList.EnableWindow(FALSE);

	initNetwork();
}

void
CAgentDlg::updateTreeItem(cServerInfo *_lpInfo)
{
	if	(_lpInfo->m_hTreeNode	==	NULL)
	{
		fillTree();
		return;
	}

	m_treeServerList.SetItemText(_lpInfo->m_hTreeNode,_ms("%s[%s]",_lpInfo->m_strName,_lpInfo->m_strIP));
}

void
CAgentDlg::fillTree()
{
	for (int i=eST_SINGLE_SERVER_BEGIN;i<eST_SINGLE_SERVER_END;i++)
	{
		int			iSlot		=	i-eST_SINGLE_SERVER_BEGIN;
		cServerInfo	*lpServer	=	&g_serverList.m_aSingleServerList[iSlot];

		if	(lpServer->m_iServerType	==	-1)
		{
			TV_INSERTSTRUCT	tvInfo;

			tvInfo.hParent		=	NULL;
			tvInfo.hInsertAfter =	TVI_LAST;
			tvInfo.item.pszText	=	_ms("[off]%s",g_strServerName[i]);
			tvInfo.item.mask	=	TVIF_TEXT|TVIF_PARAM;

			lpServer->m_hTreeNode=	m_treeServerList.InsertItem(&tvInfo);

			continue;
		}

		{
			TV_INSERTSTRUCT	tvInfo;

			tvInfo.hParent		=	NULL;
			tvInfo.hInsertAfter =	TVI_LAST;
			tvInfo.item.pszText	=	_ms("%s[%s]",lpServer->m_strName,lpServer->m_strIP);
			tvInfo.item.mask	=	TVIF_TEXT|TVIF_PARAM;

			lpServer->m_hTreeNode=	m_treeServerList.InsertItem(&tvInfo);
		}
	}

	for (int iWorld=0;iWorld<dMAX_WORLD_SERVER_COUNT;iWorld++)
	{
		cWorldServerInfo	*lpWorld	=	&g_serverList.m_aWorldList[iWorld];

		if	(lpWorld->m_iIndex	==	-1)
			continue;

		cServerInfo			*lpWorldServer=	&lpWorld->m_aServerList[0];

		TV_INSERTSTRUCT	tvInfo;

		tvInfo.hParent		=	NULL;
		tvInfo.hInsertAfter =	TVI_LAST;
		tvInfo.item.pszText	=	_ms("%s[%s]",lpWorldServer->m_strName,lpWorldServer->m_strIP);
		tvInfo.item.mask	=	TVIF_TEXT|TVIF_PARAM;

		lpWorldServer->m_hTreeNode	=	m_treeServerList.InsertItem(&tvInfo);

		for (int iServer=eSTIW_AVATAR_DBC;iServer<=eSTIW_COMMUNITY;iServer++)
		{
			cServerInfo	*lpServer	=	&lpWorld->m_aServerList[iServer];

			if	(lpServer->m_iServerType	==	-1)
			{
				tvInfo.hParent		=	lpWorldServer->m_hTreeNode;
				tvInfo.hInsertAfter =	TVI_LAST;
				tvInfo.item.pszText	=	_ms("[off][%s]",g_strServerName[iServer]);
				tvInfo.item.mask	=	TVIF_TEXT|TVIF_PARAM;

				lpServer->m_hTreeNode=	m_treeServerList.InsertItem(&tvInfo);
			}
			else
			{
				tvInfo.hParent		=	lpWorldServer->m_hTreeNode;
				tvInfo.hInsertAfter =	TVI_LAST;
				tvInfo.item.pszText	=	_ms("%s[%s]",lpServer->m_strName,lpServer->m_strIP);
				tvInfo.item.mask	=	TVIF_TEXT|TVIF_PARAM;

				lpServer->m_hTreeNode=	m_treeServerList.InsertItem(&tvInfo);
			}
		}

		for (int iGame=eSTIW_GAME0;iServer<eSTIW_GAME0+lpWorld->m_iGameServerCount;iServer++)
		{
			cServerInfo	*lpServer	=	&lpWorld->m_aServerList[iGame];

			if	(lpServer->m_iServerType	==	-1)
				continue;

			tvInfo.hParent		=	lpWorldServer->m_hTreeNode;
			tvInfo.hInsertAfter =	TVI_LAST;
			tvInfo.item.pszText	=	_ms("%s[%s]",lpServer->m_strName,lpServer->m_strIP);
			tvInfo.item.mask	=	TVIF_TEXT|TVIF_PARAM;

			lpServer->m_hTreeNode=	m_treeServerList.InsertItem(&tvInfo);
		}
	}
}

BOOL
CAgentDlg::sendPacket(DWORD _dwIdx)
{
	CClient*	lpUser = UserManager.GetClientPnt((WORD)_dwIdx);

	if	(!lpUser || lpUser->m_wSerial == 0xffff)
		return FALSE;

	int	iSendSize	=	lpUser->m_pclSendBuffer->GetData(lpUser->m_pcSendpacket,0);

	if	(!iSendSize)
		return TRUE;

	lpUser->m_stSendOL.IOState = 1;

	DWORD	dwSend = 0;	
	WSABUF	buf;

	lpUser->m_wSendpacket_size	=	iSendSize;

	buf.len	=	lpUser->m_wSendpacket_size;
	buf.buf	=	lpUser->m_pcSendpacket;

	if	(WSASend(lpUser->m_hSock, &buf,1,&dwSend,0,&lpUser->m_stSendOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();

		if	(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			lpUser->m_pclSendBuffer->Next();

			DisUser(_dwIdx,"send error");

			return FALSE;
		}
	}

	return TRUE;
}

BOOL
CAgentDlg::sendPacketAdd(DWORD _dwIdx)
{
	CClient	* lpUser = UserManager.GetClientPnt((WORD)_dwIdx);

	if	(!lpUser || lpUser->m_wSerial == 0xffff)	
		return FALSE;

	lpUser->m_stSendOL.IOState = 1;

	DWORD	dwSend = 0;
	WSABUF	buf;

	buf.len = lpUser->m_wSendpacket_size;
	buf.buf = lpUser->m_pcSendpacket;

	if	(WSASend(lpUser->m_hSock, &buf,1,&dwSend,0,&lpUser->m_stSendOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();

		if	(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			lpUser->m_pclSendBuffer->Next();

			DisUser(_dwIdx,"add send error");
			return FALSE;
		}
	}
	return TRUE;
}

void CAgentDlg::OnSelchangeServerType() 
{
	UpdateData(TRUE);
	
}

void CAgentDlg::OnChangeLoginServerIp() 
{
	UpdateData(TRUE);
	
}

void CAgentDlg::OnKill() 
{
	killServer();
}

void CAgentDlg::OnBeginServer() 
{
	beginServer();
}

void CAgentDlg::OnEndServer() 
{
	endServer();
}


void CAgentDlg::killServer() 
{
	char	strName[128];

	if	(m_iServerType	==	-1)
		return;

	int	iStatus	=	ReadServerStatusWithRegistry(m_iServerType);				//	서버 상태 읽기

	if	(!ReadServerEXENameWithRegistry(m_iServerType,strName,128))
	{
		PrintMainMessage("can not find registry info");

		return;
	}

	if	(!KillProcess(strName))
	{
		PrintMainMessage("can not find running server");

		return;
	}

	PrintMainMessage("kill [%s] server",g_strServerName[m_iServerType]);
}

void CAgentDlg::beginServer() 
{
	if	(m_iServerType	==	-1)
		return;

	WriteAgentSignToRegistry(m_iServerType,eAGENT_SIGN_BEGIN_BY_AGENT);

	char	strExeName[128];
	char	strPath[512];

	if	(!ReadServerEXENameWithRegistry(m_iServerType,strExeName,128) || !ReadServerPathWithRegistry(m_iServerType,strPath,512))
	{
		PrintMainMessage("can not find registry! failed to begin server");

		return;
	}

	ShellExecute(NULL,"open",strExeName,NULL,strPath,SW_SHOWNORMAL);

	PrintMainMessage("begin [%s] server",g_strServerName[m_iServerType]);
}

void CAgentDlg::endServer() 
{
	if	(m_iServerType	==	-1)
		return;

	int	iStatus		=	ReadServerStatusWithRegistry(m_iServerType);				//	서버 상태 읽기

	if	(iStatus	==	eSERVER_STATUS_OFF	||	iStatus	==	eSERVER_STATUS_QUIT)
	{
		PrintMainMessage("can not find running server");

		return;
	}

	WriteAgentSignToRegistry(m_iServerType,eAGENT_SIGN_END_SERVER);

	PrintMainMessage("end [%s] server",g_strServerName[m_iServerType]);
}
