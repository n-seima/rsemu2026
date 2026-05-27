
// RSS_AgentDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "RSS_Agent.h"
#include "RSS_AgentDlg.h"
#include "FTPClient.h"
#include "enviroment_value.h"
#include "lnk_define.h"
#include "Timer.h"
#include "sharedmemory.h"
#include "file.h"
#include "MessSign.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef	_DEBUG
#pragma comment(lib, "./common/messSign_debug.lib")
#else
#pragma comment(lib, "./common/messSign_release.lib")

#endif
cEnviromentValue	l_envValue;

#define WM_TRAY_ICON_NOTIFY_MESSAGE (WM_USER + 1)

TCHAR	*l_strServerType[]	=	
{
	L"Game",
	L"World",
	L"Login",
	L"UserDBC",
	L"AvatarDBC",
	L"Log",
	L"Community",
	L"SMManager",
};

//
//	이후 설정된 수치들은 그냥 사용한다.
//	
//	어플리케이션 종료 직전 혹은 필요 할때마다 저장을 해 준다.

// CRSS_AgentDlg 대화 상자

CRSS_AgentDlg::CRSS_AgentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRSS_AgentDlg::IDD, pParent)
{
	m_hIcon			=	AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pFTPClient	=	NULL;
	m_pMemoryDLL	=	new CSharedMemory;
}

void CRSS_AgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LOG, m_ctrlLog);
}

BEGIN_MESSAGE_MAP(CRSS_AgentDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CRSS_AgentDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BEGIN_SERVER, &CRSS_AgentDlg::OnBnClickedBeginServer)
	ON_BN_CLICKED(IDC_FOR_DEBUG, &CRSS_AgentDlg::OnBnClickedForDebug)
	ON_MESSAGE(WM_TRAY_ICON_NOTIFY_MESSAGE, &CRSS_AgentDlg::TrayIconMsg)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CRSS_AgentDlg 메시지 처리기

BOOL CRSS_AgentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HANDLE	m_hMutex	=	::CreateMutex(NULL, TRUE, _T("___RSSAGENT")); // 같은 뮤택스가 존재하면 이미 다른 프로그램이 실행중임

	if	(m_hMutex)
	{
		if	(ERROR_ALREADY_EXISTS==::GetLastError())
		{
			::ReleaseMutex(m_hMutex);

			MessageBox(L"Is already running.");

			OnCancel();

			return FALSE;
		}
	}

	InitializeCriticalSection(&m_stMainMsgCS);

	m_pFTPClient	=	new cFTPClient();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_bIsTrayIcon = FALSE;
	RegistTrayIcon();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

class	CCritical
{
public:
	CRITICAL_SECTION	* cs;

	CCritical(CRITICAL_SECTION * _cs)
	{
		cs = _cs;
		EnterCriticalSection(cs);
	}

	~CCritical()
	{
		LeaveCriticalSection(cs);
	}
};

void __cdecl CRSS_AgentDlg::PrintMainMessage(TCHAR * _strMsg, ...)
{
	CCritical	cs(&m_stMainMsgCS);

	TCHAR	s_tempStr[1024],strBuffer2[1024];

	va_list argptr;

	va_start(argptr, _strMsg);

	_vstprintf_s(s_tempStr,_strMsg,argptr);

	va_end(argptr);

 	cClock	curTime;
 
 	curTime.getTime();
 
	SPRINTF(strBuffer2,L"[%.2d/%.2d/%.2d %.2d:%.2d] %s",curTime.m_u2Year-2000,curTime.m_u2Month,curTime.m_u2Day,curTime.m_u2Hour,curTime.m_u2Minute,s_tempStr);

 	int	iIndex	=	m_ctrlLog.AddString(s_tempStr);
 
 	m_ctrlLog.SetTopIndex(iIndex);

	m_ctrlLog.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
}

void CRSS_AgentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CRSS_AgentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRSS_AgentDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

BOOL
CRSS_AgentDlg::launchServer(TCHAR *_lpstrServerFile)
{
	TCHAR	strCurFolder[MAX_PATH];

	GetCurrentDirectory(MAX_PATH,strCurFolder);

	PrintMainMessage(L"> Server File Update");

	if	(IsFile(_lpstrServerFile))
	{
		SetFileAttributes(_lpstrServerFile,FILE_ATTRIBUTE_ARCHIVE);

		DeleteFile(_lpstrServerFile);
	}

	if	(IsFile(_lpstrServerFile))
	{
		PrintMainMessage(L"    Failed to delete exist server");
		return	FALSE;
	}

	if	(!m_pFTPClient->downloadFile(L"", _lpstrServerFile, strCurFolder))
	{
		PrintMainMessage(L"    Failed to update server");
		return	FALSE;
	}

	ShellExecute(NULL,L"open",_lpstrServerFile,NULL,NULL,SW_SHOWNORMAL);

	PrintMainMessage(L"    Complete");

	return	TRUE;
}

void CRSS_AgentDlg::OnBnClickedBeginServer()
{
	UpdateData(TRUE);

	PrintMainMessage(L"> Started working for launch server");

	if	(!connectWithFTP())
		return;

	if	(!getIniInfo())
		return;

	if	(!mountDLL(m_strDllFile))
		return;

	launchServer(m_strServerFile);
}

BOOL CRSS_AgentDlg::DestroyWindow()
{
	UpdateData(TRUE);

	DeleteCriticalSection(&m_stMainMsgCS);

	if	(m_bIsTrayIcon)
	{
		NOTIFYICONDATA  nid;
		nid.cbSize	= sizeof(nid);
		nid.hWnd	= m_hWnd;
		nid.uID		= IDR_MAINFRAME;
		
		Shell_NotifyIcon(NIM_DELETE, &nid);
	}	

	delete m_pMemoryDLL;

	if	(m_pFTPClient)
	{
		delete m_pFTPClient;

		m_pFTPClient	=	NULL;
	}

	return CDialog::DestroyWindow();
}

void
CRSS_AgentDlg::getTempFileName(TCHAR* _lpstrTempFileName,int _iBuffSize)
{
	TCHAR	strTempPath[MAX_PATH];
	TCHAR	strTempFile[MAX_PATH];

	GetTempPath(MAX_PATH,strTempPath);

	GetTempFileName(strTempPath,L"",GetTickCount()%1274,strTempFile);

	STRCPY(_lpstrTempFileName,_iBuffSize,strTempFile);
}

BOOL
CRSS_AgentDlg::mountDLL(TCHAR *_lpstrDLLFile)
{
	TCHAR	strCurFolder[MAX_PATH],strLocalFileName[MAX_PATH];

 	getTempFileName(strLocalFileName,MAX_PATH);

	GetCurrentDirectory(MAX_PATH,strCurFolder);

	if	(!m_pFTPClient->downloadFile(L"", _lpstrDLLFile, strCurFolder,strLocalFileName))
	{
		PrintMainMessage(L"    Failed to Mount data.");
		return	FALSE;
	}

	int	iFileSize	=	cFile::GetSize(strLocalFileName);

	cFile	file;

	if	(!file.open(strLocalFileName,L"rb"))
	{
		PrintMainMessage(L"    Failed to local data open");
		return	FALSE;
	}

	m_pMemoryDLL->close();
	m_pMemoryDLL->create("rsgsdllmemory",file.m_uiLength+4);
	m_pMemoryDLL->beginProcess();

	*(int *)m_pMemoryDLL->getBuffer()	=	file.m_uiLength;
	file.read(m_pMemoryDLL->getBuffer()+4,file.m_uiLength);

	file.close();
	m_pMemoryDLL->endProcess();

	DeleteFile(strLocalFileName);

	PrintMainMessage(L"    Complete to Data Mount");

	return	TRUE;
}

BOOL
CRSS_AgentDlg::getIniInfo()
{
	TCHAR	strCurFolder[MAX_PATH],strLocalIniFileName[MAX_PATH];

	getTempFileName(strLocalIniFileName,MAX_PATH);

	GetCurrentDirectory(MAX_PATH,strCurFolder);

	PrintMainMessage(L"> Read Update Information");

	if	(!m_pFTPClient->downloadFile(L"", L"RS_Server.ini", strCurFolder,strLocalIniFileName))
	{
		PrintMainMessage(L"    Failed to download");
		return	FALSE;
	}

	{
		if	(!GetPrivateProfileString(_T("server"),_T("data"),_T(""),m_strDllFile,512,strLocalIniFileName))
		{
			PrintMainMessage(L"    Failed to parsing data");

			return	FALSE;
		}

		if	(!GetPrivateProfileString(_T("server"),_T("gameserver"),_T(""),m_strServerFile,512,strLocalIniFileName))
		{
			PrintMainMessage(L"    Failed to parsing server data");

			return	FALSE;
		}

		DeleteFile(strLocalIniFileName);
	}

	return	TRUE;
}

int
GetSperateString(const TCHAR *_lpstrInputString,TCHAR *_lpstrResultString,TCHAR _seperator,int _iPos)
{
	int	iBP	=	0;

	_lpstrResultString[0]	=	NULL;

	while(_lpstrInputString[_iPos] != _seperator && _lpstrInputString[_iPos] != NULL)
		_lpstrResultString[iBP++]	=	_lpstrInputString[_iPos++];

	_lpstrResultString[iBP]	=	NULL;

	if	(_lpstrInputString[_iPos]	==	NULL)
		return	_iPos;

	return	_iPos+1;
}


BOOL
CRSS_AgentDlg::connectWithFTP()
{
	m_pFTPClient->disconnect();

	PrintMainMessage(L"> Try Connect");

	int		iDataSize;
	TCHAR	strInfo[1024];

	m_pMessData	=	(char*)cMessSign::ReadMessSignFile(L"config.dat",&iDataSize);

	if	(!m_pMessData)
	{
		PrintMainMessage(L"    Can not find config file!");

		return	FALSE;
	}

	cMessSign::DecodeBuffer(m_pMessData,(char*)strInfo,iDataSize);

	strInfo[1024-1]	=	NULL;

	TCHAR	strID[64],strPass[64],strIP[64],strPort[64];
	int		iPos=0;

	iPos	=	GetSperateString(strInfo,strIP,'/',iPos);
	iPos	=	GetSperateString(strInfo,strPort,'/',iPos);
	iPos	=	GetSperateString(strInfo,strID,'/',iPos);
	iPos	=	GetSperateString(strInfo,strPass,'/',iPos);

	int		iPort	=	_wtoi(strPort);

 	BOOL	bIsConnected	=	m_pFTPClient->connect(strIP,iPort,strID,strPass);

	memset(strID,0,sizeof(strID));
	memset(strPass,0,sizeof(strPass));
	memset(strIP,0,sizeof(strIP));
	memset(strPort,0,sizeof(strPort));
	memset(m_pMessData,0,iDataSize);
	memset(strInfo,0,sizeof(strInfo));

	pKILL(m_pMessData);

	iPort	=	0;

	if	(!bIsConnected)
	{
		PrintMainMessage(L"    Connection Failed!");
		return	FALSE;
	}

	PrintMainMessage(L"    Connect OK!");

	return	TRUE;
}

void CRSS_AgentDlg::OnBnClickedForDebug()
{
	UpdateData(TRUE);

	PrintMainMessage(L"> Started working for debugging");

	if	(!connectWithFTP())
		return;

	if	(!getIniInfo())
		return;

	mountDLL(m_strDllFile);
}
void CRSS_AgentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if	(nID == SC_CLOSE)
		ShowWindow(SW_HIDE);	
	else
		CDialog::OnSysCommand(nID, lParam);
}

LRESULT
CRSS_AgentDlg::TrayIconMsg(WPARAM wParam, LPARAM lParam)
{
	UINT uMsg	= (UINT)lParam;  
	
	switch (uMsg ) 
	{
		case WM_LBUTTONDBLCLK	:
		case WM_LBUTTONDOWN		:
		case WM_RBUTTONDOWN		:
			ShowWindow(SW_SHOW);
			break;
	}

	return	0;
}

void CRSS_AgentDlg::RegistTrayIcon()
{
	NOTIFYICONDATA  nid;
	nid.cbSize				= sizeof(nid);
	nid.hWnd				= m_hWnd;
	nid.uID					= IDR_MAINFRAME;  // 아이콘 리소스 ID
	nid.uFlags				= NIF_MESSAGE | NIF_ICON | NIF_TIP; // 플래그 설정
	nid.uCallbackMessage	= WM_TRAY_ICON_NOTIFY_MESSAGE ; // 콜백메시지 설정
	nid.hIcon				= AfxGetApp()->LoadIcon(IDR_MAINFRAME); // 아이콘 로드 
	
	// tooltip
	TCHAR	strTitle[256];
	GetWindowText(strTitle, sizeof(strTitle));
	lstrcpy(nid.szTip, strTitle); 
	Shell_NotifyIcon(NIM_ADD, &nid);
	SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)nid.hIcon);

	m_bIsTrayIcon			= TRUE;
}