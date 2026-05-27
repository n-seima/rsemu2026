// SSViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SSViewer.h"
#include "SSViewerDlg.h"
#include "mmsystem.h"
#include "cSRVUTIL.h"
#include "cServerStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////////////////////
// Extern
UINT g_iShellRestart;					// 트레이 아이콘이 사라지는 버그 때문에
CSSViewerDlg *g_pcMainFramePtr;			// 메인 프레임의 포인터
DebugTracer		* cLOG;

// Extern
////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSViewerDlg dialog

CSSViewerDlg::CSSViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSSViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSSViewerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pstMessaeInfo = NULL;
	m_pcDlgHistory = NULL;
}

void CSSViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSSViewerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSSViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CSSViewerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_COMMAND(ID_MENUITEM_MAIN_EXIT, OnMenuitemMainExit)
	ON_COMMAND(ID_MENUITEM_MAIN_HISTORY, OnMenuitemMainHistory)
	ON_COMMAND(ID_MENUITEM_STOP, OnMenuitemStop)
	ON_COMMAND(ID_MENUITEM_RESTART, OnMenuItemRestart)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(dUM_TRAYICON_NOTIFY, OnTrayNotification)	
	ON_REGISTERED_MESSAGE(g_iShellRestart, OnTrayReshow) 
	ON_MESSAGE(WM_TASKBARNOTIFIERCLICKED,OnTaskbarNotifierClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSViewerDlg message handlers

BOOL CSSViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_isStopAlarm = FALSE;
//	m_timeCheckHour;
	m_iRecoverMinute = 0;

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	if(!m_trayIcon.create(this, dUM_TRAYICON_NOTIFY, "RSOnline Server State Viewer", NULL, IDR_MAINFRAME)){
		return -1;
	}
	m_trayIcon.setIcon(IDR_MAINFRAME);

	// MSN Message
	m_wndTaskbarNotifier.Create(this);
	m_wndTaskbarNotifier.SetSkin(IDB_BITMAP_SKIN);
	m_wndTaskbarNotifier.SetTextFont("Arial", 110,TN_TEXT_NORMAL,TN_TEXT_UNDERLINE);
 	m_wndTaskbarNotifier.SetTextColor(RGB(0,0,0),RGB(0,0,200));
	m_wndTaskbarNotifier.SetTextRect(CRect(20, 20,m_wndTaskbarNotifier.m_nSkinWidth-10,m_wndTaskbarNotifier.m_nSkinHeight-25));

	// 메인 프레임 포인터
	g_pcMainFramePtr = this;

	////////////////////////////////////////////////////////////////////////////
	// 메세지 데이타 읽기 (File)
	FILE *pFile;
	pFile = fopen(dDATAFILE, _T("r"));
	if(!pFile){
		AfxMessageBox("Error, Not find file [Notice.dat]!");
		PostQuitMessage(0);
		return FALSE;
	}

	// 라인수구하기
	char szData[dMAXMESSAGESIZE + 40];
	m_iMessageCnt = 0;
	while(fgets(szData, sizeof(szData), pFile)){
		++m_iMessageCnt;
	}

	fseek(pFile, 0, SEEK_SET);			// 파일포인터를 처음으로 이동

	m_pstMessaeInfo = new stMESSAGEINFO[m_iMessageCnt];
	char *p;
	char szIdx[40];
	int iIdxSize;
	int iCount = 0;
	while(fgets(szData, sizeof(szData), pFile)){
		p = strstr(szData, ",");
		iIdxSize = strlen(szData) - strlen(p);
		memcpy(szIdx, szData, iIdxSize);
		szIdx[iIdxSize] = NULL;
		++p;
		m_pstMessaeInfo[iCount].iIdx = atoi(szIdx);
		strcpy(m_pstMessaeInfo[iCount].szMessage, p);
		++iCount;
	}

	fclose(pFile);
	// 메세지 데이타 읽기 (File)
	////////////////////////////////////////////////////////////////////////////
	
	if(!g_SS.init("SS","Viewer"))
	{
		MessageBox("Failed to Initialize of Server Status Viewer");
		return FALSE;
	}

	SetTimer(eTIMER_DLEINITDLG, 1000, NULL);		// 한번만 실행되고 KillTimer()

	g_SS.getError(TRUE);
	SetTimer(eTIMER_CHECKSS, 6000, NULL);	//	분당 한번씩
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSSViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSSViewerDlg::OnPaint() 
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSSViewerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 트레이 아이콘 사라지는 버그때문에
///////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSSViewerDlg::OnTrayReshow(WPARAM wParam, LPARAM lParam) 
{ 

    // TrayIcon을 다시 보여줍니다. ShowTray는 Tray를 보여주는 함수입니다. 
	if(!m_trayIcon.create(this, dUM_TRAYICON_NOTIFY, "RSOnline Server State Viewer", NULL, IDR_MAINFRAME)){
		return -1;
	}
	m_trayIcon.setIcon(IDR_MAINFRAME);

	return TRUE;
} 


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 트레이 아이콘 Notify
///////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSSViewerDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if(LOWORD(lParam) == WM_RBUTTONUP){				// 오른쪽 마우스 버튼이 눌렸을 경우
		// TrackPopupMenu
		CMenu obMenu;
		obMenu.LoadMenu(IDR_MENU_MAIN);
		CMenu *pPopupMenu = obMenu.GetSubMenu(0);
		ASSERT(pPopupMenu);
		CPoint obCursorPoint = (0, 0);
		GetCursorPos(&obCursorPoint);
		pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN ,obCursorPoint.x ,obCursorPoint.y,this );				
	}else if(LOWORD(lParam) == WM_LBUTTONDBLCLK)
	{	// 왼쪽 마우스 버튼이 더블 클릭되었을 경우
		//ShowWindow(SW_RESTORE);
		//SetForegroundWindow();	
	}

	return 1;
}

void CSSViewerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case eTIMER_DLEINITDLG:
		KillTimer(eTIMER_DLEINITDLG);		// 한번만 실행
		ShowWindow(SW_HIDE);
		break;
	// TEST
	case eTIMER_CHECKSS:
		{	//	DB로 접근해서 최근 정보를 받아온다.
			g_SS.getSS();
			g_SS.getError();

			int iloop = 0;
			//	5분 이상 업데이트가 안된 서버가 있는지 확인한다.
			for(iloop=0;iloop<g_SS.m_listSSInfo.size();iloop++)
			{
				cpUTime t;
				t.m_dwValue = g_SS.m_listSSInfo.at(iloop).utTime.m_dwValue;
				if( cSRVUTIL::CalcTermToday( t , cSRVUTIL::eDATECHECK_MIN) > 5 )	//	5분이상 응답이 없으면..
				{
					if(!m_isStopAlarm)
						showNotice( g_SS.m_listSSInfo.at(iloop) );
				}
			}
			
			//	치명적인 에러가 발생했는지 확인한다.
			for(iloop=0;iloop<g_SS.m_listSSError.size();iloop++)
			{
				if(!m_isStopAlarm)
					showNotice( g_SS.m_listSSError.at(iloop) );
			}     

			cpUTime t;
			t.m_dwValue = g_SS.m_listSSInfo.at(iloop).utTime.m_dwValue;
			if(m_isStopAlarm && cSRVUTIL::CalcTermToday(t,cSRVUTIL::eDATECHECK_MIN) > m_iRecoverMinute)
			{
				m_isStopAlarm = FALSE;
			}
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 프로그램 종료
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSSViewerDlg::OnMenuitemMainExit() 
{
	if(AfxMessageBox("Program Exit?", MB_YESNO) == IDYES){
		PostQuitMessage(0);
		return;
	}	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : History
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSSViewerDlg::OnMenuitemMainHistory() 
{
	showDlgHistory();	
}

void CSSViewerDlg::OnMenuItemRestart()
{
	m_isStopAlarm = FALSE;
}

void CSSViewerDlg::OnMenuitemStop()
{
	//	Dlg 뛰워서 쉴 시간 정하고..

	//	설정한다.
	m_isStopAlarm = TRUE;
	cSRVUTIL::GetUTime((cpUTime *)&m_timeCheckHour);
	m_iRecoverMinute = 30;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : Show MSN Message
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CSSViewerDlg::showNotice(char *_pszDate, char *_pszName, char *_pszIP, int _iErrorType, int _iParam1, char *_pszParam2)
void CSSViewerDlg::showNotice(cSSInfoTable _info)
{
	BOOL bFlag = FALSE;
	int iIdx;
	for(iIdx=0; iIdx<m_iMessageCnt; iIdx++){
		if(_info.iErrorType == m_pstMessaeInfo[iIdx].iIdx){
			bFlag = TRUE;
			break;
		}
	}

	CString strNotice;

	// 해당 메세지를 찾을수 없을때 
	if(!bFlag)
	{
		strNotice.Format(m_pstMessaeInfo[0].szMessage, _info.strName);
		m_wndTaskbarNotifier.Show(strNotice, 200, 36000000, 200, 1);
		playWaveFile(dSOUNDFILE);
		return;
	}

	strNotice.Format(m_pstMessaeInfo[iIdx].szMessage, _info.strName);
	m_wndTaskbarNotifier.Show(strNotice, 200, 36000000, 200, 1);
	playWaveFile(dSOUNDFILE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : MSN MessageBox Click
///////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSSViewerDlg::OnTaskbarNotifierClicked(WPARAM wParam,LPARAM lParam)
{
	//MessageBox("A Taskbar Notifier was clicked!!","Hi",MB_OK);
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 사운드 재생
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSSViewerDlg::playWaveFile(char *_pszFileName)
{
	DWORD Result;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_PLAY_PARMS mciPlayParms;

	// 장치를 Open하고 ID를 발급받는다.
	mciOpenParms.lpstrDeviceType = "WaveAudio";
	mciOpenParms.lpstrElementName = _pszFileName;
	Result=mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID) &mciOpenParms);
	if (Result) {
		return Result;
	}
	m_wDeviceID = mciOpenParms.wDeviceID;

	mciPlayParms.dwCallback = (DWORD)m_hWnd;
	Result = mciSendCommand(m_wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParms);
	if (Result) {
		mciSendCommand(m_wDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
		return Result;
	}
	return 0;
}

void CSSViewerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	// 사운드재생파일관련
	if (m_wDeviceID)
		mciSendCommand(m_wDeviceID, MCI_CLOSE, 0, (DWORD)NULL);

	if(m_pstMessaeInfo){
		delete [] m_pstMessaeInfo;
		m_pstMessaeInfo = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 업무일지 (메모)
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSSViewerDlg::showDlgHistory()
{
	if(!m_pcDlgHistory){
		m_pcDlgHistory = new cDlgHistory;
		m_pcDlgHistory->Create(IDD_DIALOG_HISTORY, GetDesktopWindow());
	}
	m_pcDlgHistory->ShowWindow(SW_RESTORE);
	m_pcDlgHistory->SetForegroundWindow();
}

