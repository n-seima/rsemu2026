#include "stdafx.h"
#include "SMSSender.h"
#include "SMSSenderDlg.h"
#include "DBDataSMS.h"
#include "member.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	dMSG_SEND_PERIOD_SECOND	60*5	//	5분
#define	dCHECK_TIMER_PERIOD		5000	//	5초

bool
isFile(WIN32_FIND_DATA *fd)
{	
	if	(fd->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		return false;

	return true;
}

bool
IsFile(const TCHAR *fn)
{
	HANDLE				hFind;
	BOOL				bRet	=	TRUE;
	WIN32_FIND_DATA		fd;
	int					fc		=	0;

	hFind	=	FindFirstFile(fn,&fd);

	if	(hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	FindClose(hFind);

	return isFile(&fd);
}

inline const TCHAR*
IA2U(const char *_pszMulti,int _iOpt=CP_ACP)
{	
	static	TCHAR	strInstanceAsciiToUnicodeBuffer[4096];
#if defined _UNICODE  ||  defined UNICODE                      
	if	(MultiByteToWideChar(_iOpt, 0, _pszMulti, -1, strInstanceAsciiToUnicodeBuffer, 4096)	!=	0)
		return	strInstanceAsciiToUnicodeBuffer;

	return	NULL;
#else
	if	(4096 > (int)_tcslen(_pszMulti))
	{
		_tcscpy_s(strInstanceAsciiToUnicodeBuffer, 4096, _pszMulti);

		return (int)_tcslen(strInstanceAsciiToUnicodeBuffer);
	}

	return NULL;
#endif
}

inline int 
A2U(const char *_pszMulti, TCHAR *_pszWide, int _iWideBufferSize,int _iOpt=CP_ACP)
{	
#if defined _UNICODE  ||  defined UNICODE                      
	return	MultiByteToWideChar(_iOpt, 0, _pszMulti, -1, _pszWide, _iWideBufferSize);
#else
	if	(_iWideBufferSize > (int)_tcslen(_pszMulti))
	{
		_tcscpy_s(_pszWide, _iWideBufferSize, _pszMulti);

		return (int)_tcslen(_pszWide);
	}

	return 0;		
#endif
}

inline int 
U2A(const TCHAR *_pszUnicode,char *_pszMulti,int _iMultiBufferSize,int _iOpt=CP_ACP)
{	
#if defined _UNICODE  ||  defined UNICODE
	int iResult = WideCharToMultiByte(_iOpt, 0, _pszUnicode, -1, _pszMulti, _iMultiBufferSize, NULL, NULL);

	if	(iResult == 0)
	{
		DWORD dwRe = GetLastError();

		if	(dwRe == ERROR_INSUFFICIENT_BUFFER)
		{
			return 0;
		}
	}

	return iResult;
#else
	if	(_iMultiBufferSize >= (int)_tcslen(_pszUnicode))
	{
		_tcscpy_s(_pszMulti, _iMultiBufferSize, _pszUnicode);
		return (int)_tcslen(_pszMulti);
	}
	return 0;
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Extern
UINT g_iShellRestart;	// 트레이 아이콘이 사라지는 버그 때문에
// Extern
////////////////////////////////////////////////////////////////////////////////////////////////

TCHAR	l_strLoginServerLifeSign[256];	
TCHAR	l_strDeathSignTextFile[256];	
TCHAR	l_strCheckingMarkFile[256];	

#define	dRS_SMS_ALARM_LIFE_SIGN	"c:\\_rsLog\\RS_SMS_AlarmAlive.dat"
#define	dLOGINSERVER_LIFE_SIGN	"c:\\_rsLog\\loginServerAlive.dat"
#define	dDEATH_SIGN_TEXT_FILE	"c:\\_rsLog\\deathSign.txt"
#define	dCHECKING_MARK_FILE		"c:\\_rsLog\\nowChecking.dat"


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSMSSenderDlg 대화 상자

CSMSSenderDlg::CSMSSenderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMSSenderDlg::IDD, pParent)
{
	m_hIcon			=	AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dwLastSendSec	=	0;
	m_dwLastLoginServerErrorSec	=	0;

	A2U(dLOGINSERVER_LIFE_SIGN,l_strLoginServerLifeSign,_countof(l_strLoginServerLifeSign));
	A2U(dDEATH_SIGN_TEXT_FILE,l_strDeathSignTextFile,_countof(l_strDeathSignTextFile));
	A2U(dCHECKING_MARK_FILE,l_strCheckingMarkFile,_countof(l_strCheckingMarkFile));

	memset(m_strOldText,0,sizeof(m_strOldText));
}

void CSMSSenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lcMember);
	DDX_Control(pDX, IDC_TYPE, m_cbType);
	DDX_Control(pDX, IDC_SENDER, m_cbSender);
}

BEGIN_MESSAGE_MAP(CSMSSenderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSMSSenderDlg::OnBnClickedButton1)

	ON_MESSAGE(dUM_TRAYICON_NOTIFY, OnTrayNotification)	
	ON_REGISTERED_MESSAGE(g_iShellRestart, OnTrayReshow) 
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TEST, &CSMSSenderDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_CHECK_NOW, &CSMSSenderDlg::OnBnClickedCheckNow)
	ON_BN_CLICKED(IDC_REPAIR_PROBLEM, &CSMSSenderDlg::OnBnClickedRepairProblem)
	ON_BN_CLICKED(IDC_NEW_MEMBER, &CSMSSenderDlg::OnBnClickedNewMember)
	ON_BN_CLICKED(IDC_REMOVE_MEMBER, &CSMSSenderDlg::OnBnClickedRemoveMember)
	ON_EN_CHANGE(IDC_CUSTOM_MSG, &CSMSSenderDlg::OnEnChangeCustomMsg)
	ON_BN_CLICKED(IDC_SEND_CUSTOM_MSG, &CSMSSenderDlg::OnBnClickedSendCustomMsg)
END_MESSAGE_MAP()


// CSMSSenderDlg 메시지 처리기

BOOL CSMSSenderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetCurrentDirectory(512,m_strLogFolder);

	memset(g_aMember,0,sizeof(g_aMember));
	g_iMemberCount	=	0;

	loadData();

	m_lcMember.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lcMember.InsertColumn(0,_T(""),LVCFMT_RIGHT		,0);
	m_lcMember.InsertColumn(1,_T("구분"),LVCFMT_LEFT	,40);
	m_lcMember.InsertColumn(2,_T("이름"),LVCFMT_RIGHT	,80);
	m_lcMember.InsertColumn(3,_T("번호"),LVCFMT_LEFT	,200);

	fillList();

	{
		m_cbType.AddString(_T("운영"));
		m_cbType.AddString(_T("개발"));
		m_cbType.SelectString(0,_T("개발"));
	}

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);

	if	(pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);

		if	(!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	if(!m_trayIcon.create(this, dUM_TRAYICON_NOTIFY, _T("SMSSender"), NULL, IDR_MAINFRAME)){
		AfxMessageBox(_T("Error!cTrayIcon::create()"));
		PostQuitMessage(0);
		return FALSE;
	}
	m_trayIcon.setIcon(IDR_MAINFRAME);
	m_trayIcon.setTooltipText(_T("SMSSender"));

	SetTimer(1, 1, NULL);
	SetTimer(2, dCHECK_TIMER_PERIOD, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSMSSenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if	((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSMSSenderDlg::OnPaint()
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
HCURSOR CSMSSenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSMSSenderDlg::OnBnClickedButton1()
{
	if	(AfxMessageBox(_T("Exit ?"), MB_OKCANCEL) == IDOK)
	{
		PostQuitMessage(0);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 트레이 아이콘 사라지는 버그때문에
///////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSMSSenderDlg::OnTrayReshow(WPARAM wParam, LPARAM lParam) 
{ 
    // TrayIcon을 다시 보여줍니다. ShowTray는 Tray를 보여주는 함수입니다. 
	if	(!m_trayIcon.create(this, dUM_TRAYICON_NOTIFY, _T("SMSSender"), NULL, IDR_MAINFRAME))
	{
		return -1;
	}
	m_trayIcon.setIcon(IDR_MAINFRAME);

	return TRUE;
} 


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : TrayIcon Notify
///////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSMSSenderDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if	(LOWORD(lParam) == WM_RBUTTONUP)
	{				// 오른쪽 마우스 버튼이 눌렸을 경우
		OnBnClickedButton1();
	}
	else
	if	(LOWORD(lParam) == WM_LBUTTONDBLCLK)
	{	// 왼쪽 마우스 버튼이 더블 클릭되었을 경우
		ShowWindow(SW_RESTORE);
		SetForegroundWindow();		   
	}
	return 1;
}


BOOL CSMSSenderDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// Esc, Enter 키 막기(다이얼로그가 죽음)
	if	(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
		return 0;

	return CDialog::PreTranslateMessage(pMsg);
}

void CSMSSenderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if	(nIDEvent != 2)
	{
		CDialog::OnTimer(nIDEvent);

		return;
	}

	{
		FILE	*fp	=	NULL;

		fopen_s(&fp,dRS_SMS_ALARM_LIFE_SIGN,"wb");

		fclose(fp);
	}

	BOOL	bIsOccurError	=	FALSE;
	DWORD	dwCurSecond	=	GetTickCount()/1000;

	if	(IsFile(l_strLoginServerLifeSign)	==	FALSE)
	{
		if	(dwCurSecond	>=	m_dwLastLoginServerErrorSec+60000)	//	로그인 서버 라이프 사인이 1분이상 사라지면 문제 발생한걸로 판단.
			bIsOccurError	=	TRUE;
	}
	else
		m_dwLastLoginServerErrorSec	=	0;

	if	(IsFile(l_strDeathSignTextFile))
		bIsOccurError	=	TRUE;

	if	(bIsOccurError)
	{
		Sleep(100);	//	파일 생성중일 수도 있으니... 여유시간? - -
		
		FILE	*fp	=	NULL;
		
		fopen_s(&fp,dDEATH_SIGN_TEXT_FILE,"rb");

		if	(fp)
		{
			if	(dwCurSecond	>=	m_dwLastSendSec+dMSG_SEND_PERIOD_SECOND)	//	문제 생기면 처리 할때까지 5분 마다 문자를 보낸다.
			{
				char	strText[256];
				TCHAR	strMsg[256];

				memset(strText,0,sizeof(strText));

				UINT	uiSize	=	_filelength(_fileno(fp));

				fread(strText,min(uiSize,sizeof(strText)),1,fp);

				A2U(strText,strMsg,256);

				sendSMS(strMsg);

				m_dwLastSendSec	=	GetTickCount()/1000;
			}

			fclose(fp);
		}
	}

	if	(bIsOccurError	==	FALSE)
		m_dwLastSendSec	=	0;

	CDialog::OnTimer(nIDEvent);
}

BOOL
CSMSSenderDlg::sendSMS(TCHAR *_lpstrMessage,TCHAR *_lpstrNumber)
{
	TCHAR	strSenderNumber[64];

	if	(_lpstrNumber	==	NULL)
		memcpy(strSenderNumber,_T("02-598-9293"),sizeof(strSenderNumber));
	else
		memcpy(strSenderNumber,_lpstrNumber,sizeof(strSenderNumber));

	cDBDataSMS sms;

	if	(!sms.connect())
		return FALSE;

	for (int i=0;i<dMAX_MEMBER_COUNT;i++)
	{
		cSMS_Member	*lpMember	=	&g_aMember[i];

		if	(lpMember->m_strName[0]	==	0)
			continue;

		sms.sendSMS(lpMember->m_strNumber, strSenderNumber,_lpstrMessage);
	}

	sms.disconnect();

	return TRUE;
}

void CSMSSenderDlg::OnBnClickedTest()
{
	sendSMS(_T("테스트 문자입니다."));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CSMSSenderDlg::OnBnClickedCheckNow()
{
	int	iSelect	=	m_cbSender.GetCurSel();
	int	iIndex	=	m_cbSender.GetItemData(iSelect);

	sendSMS(_T("LNK/RS/[터미널 접속 했습니다. 문제 확인 시작합니다.]"),g_aMember[iIndex].m_strNumber);

	{
		FILE	*fp	=	NULL;
		
		fopen_s(&fp,dCHECKING_MARK_FILE,"wb");
		fclose(fp);
	}
	
	if	(IsFile(l_strDeathSignTextFile))
	{
		DeleteFile(l_strDeathSignTextFile);
		MessageBox(_T("수정 하는 동안에는 더 이상 체크를 하지 않습니다. 수정 완료후 반드시 아래 버튼을 이용해 완료 처리를 해 주시기 바랍니다."),_T("데쓰 사인을 없애고 SMS 문자를 보냈습니다"));
	}
	else
		MessageBox(_T("수정 하는 동안에는 더 이상 체크를 하지 않습니다. 수정 완료후 반드시 아래 버튼을 이용해 완료 처리를 해 주시기 바랍니다."),_T("SMS 문자를 보냈습니다"));
}

void CSMSSenderDlg::OnBnClickedRepairProblem()
{
	DeleteFile(l_strDeathSignTextFile);
	DeleteFile(l_strCheckingMarkFile);

	int	iSelect	=	m_cbSender.GetCurSel();
	int	iIndex	=	m_cbSender.GetItemData(iSelect);

	sendSMS(_T("LNK/RS/[문제 해결했습니다.]"),g_aMember[iIndex].m_strNumber);

	if	(IsFile(l_strDeathSignTextFile))
	{
		DeleteFile(l_strDeathSignTextFile);
		MessageBox(_T("데쓰 사인을 없애고 SMS 문자를 보냈습니다"));
	}
	else
		MessageBox(_T("SMS 문자를 보냈습니다"));
}

void CSMSSenderDlg::OnBnClickedNewMember()
{
	TCHAR	strName[32],strNumber[32];

	GetDlgItemText(IDC_NEW_PHONE_NUMBER,strNumber,30);
	GetDlgItemText(IDC_NEW_NAME,strName,30);

	int		iSel	=	m_cbType.GetCurSel();

	g_aMember[g_iMemberCount].m_wType	=	iSel;

	memcpy(g_aMember[g_iMemberCount].m_strName,strName,sizeof(g_aMember[g_iMemberCount].m_strName));
	memcpy(g_aMember[g_iMemberCount].m_strNumber,strNumber,sizeof(g_aMember[g_iMemberCount].m_strNumber));

	g_iMemberCount++;

	saveData();

	fillList();
}

void CSMSSenderDlg::OnBnClickedRemoveMember()
{
	int	iSelectCount	=	m_lcMember.GetSelectedCount();

	if	(iSelectCount	<=	0)
		return;

	if	(g_iMemberCount	==	0)
		return;

	POSITION	pos		=	m_lcMember.GetFirstSelectedItemPosition();

	if	(pos == NULL)
		return;

	while(pos)
	{
		int iSel	=	m_lcMember.GetNextSelectedItem(pos);
		int iIndex	=	(int)m_lcMember.GetItemData(iSel);

		memcpy(&g_aMember[iIndex],&g_aMember[g_iMemberCount-1],sizeof(cSMS_Member));
		memset(&g_aMember[g_iMemberCount-1],0,sizeof(cSMS_Member));

		g_iMemberCount--;
	}

	saveData();

	fillList();
}

void
CSMSSenderDlg::loadData()
{
	SetCurrentDirectory(m_strLogFolder);

	FILE	*fp	=	NULL;
	
	fopen_s(&fp,"member.dat","rb");

	if	(fp)
	{
		fread(&g_iMemberCount,sizeof(g_iMemberCount),1,fp);
		fread(g_aMember,sizeof(g_aMember),1,fp);
		fclose(fp);
	}
}

void
CSMSSenderDlg::saveData()
{
	SetCurrentDirectory(m_strLogFolder);

	FILE	*fp	=	NULL;
	
	fopen_s(&fp,"member.dat","wb");

	if	(fp)
	{
		fwrite(&g_iMemberCount,sizeof(g_iMemberCount),1,fp);
		fwrite(g_aMember,sizeof(g_aMember),1,fp);
		fclose(fp);
	}
}

void
CSMSSenderDlg::fillList()
{
	m_cbSender.ResetContent();

	m_lcMember.DeleteAllItems();

	TCHAR	*strType[]	=	{_T("운영"),_T("개발")};

	for (int i=0;i<dMAX_MEMBER_COUNT;i++)
	{
		cSMS_Member	*lpMember	=	&g_aMember[i];

		if	(lpMember->m_strName[0]	==	NULL)
			continue;

		if	(lpMember->m_wType	==	1)
		{
			CString	string;

			string	=	lpMember->m_strName;
			string	+=	_T("/");
			string	+=	lpMember->m_strNumber;

			int	iIndex=	m_cbSender.AddString(string);

			m_cbSender.SetItemData(iIndex,i);
		}

		int	iIndex	=	m_lcMember.InsertItem(0,_T(" "));
		m_lcMember.SetItemData(iIndex,i);

		m_lcMember.SetItemText(iIndex,1,strType[lpMember->m_wType]);
		m_lcMember.SetItemText(iIndex,2,lpMember->m_strName);
		m_lcMember.SetItemText(iIndex,3,lpMember->m_strNumber);
	}

	m_cbSender.SetCurSel(0);
}
void CSMSSenderDlg::OnEnChangeCustomMsg()
{
	TCHAR	strText[512];
	char	strTrans[512];

	GetDlgItemText(IDC_CUSTOM_MSG,strText,512);

	U2A(strText,strTrans,512);

	int	iLength		=	strlen(strTrans);

	if	(iLength	>=	80)
	{
		A2U(m_strOldText,strText,512);
		strcpy_s(strTrans,m_strOldText);

		SetDlgItemText(IDC_CUSTOM_MSG,strText);

		MessageBox(_T("80바이트를 넘을 수 없습니다."));
	}

	iLength	=	strlen(strTrans);

	strcpy_s(m_strOldText,strTrans);

	_stprintf_s(strText,_T("- %d byte"),iLength);

	SetDlgItemText(IDC_CUSTOM_MSG_LENGTH,strText);
}

void CSMSSenderDlg::OnBnClickedSendCustomMsg()
{
	TCHAR	strText[512];

	GetDlgItemText(IDC_CUSTOM_MSG,strText,512);

	int	iSelect	=	m_cbSender.GetCurSel();
	int	iIndex	=	m_cbSender.GetItemData(iSelect);

	if	(sendSMS(strText,g_aMember[iIndex].m_strNumber))
		MessageBox(_T("메시지를 성공적으로 전송 했습니다."));
	else
		MessageBox(_T("메시지 전송에 실패 했습니다!!"));
}