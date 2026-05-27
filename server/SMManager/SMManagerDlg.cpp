// SMManagerDlg.cpp : implementation file
//
//#define	dBBECK

#include "stdafx.h"
#include "SMManager.h"
#include "SMManagerDlg.h"
#ifndef dBBECK
#include "CDataManager.h"
#endif

#include "reg_common.h"	//	이건 공유 안하고 쓴다.
#include "../buildVersion.h"	//	이건 공유 안하고 쓴다.
							//	없으면 파일을 생성한다.(공유 하지 말것!!)
							//	안에는
							//	#pragma	once
							//	#define	dBUILDER_NAME	"young"
							//	와 같이 선언 되어 있다. 뒤에 이름은 프로그래머 이름을 적을것


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSMManagerDlg dialog

CSMManagerDlg::CSMManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMManagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSMManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMManagerDlg)
	DDX_Control(pDX, IDC_OUTPUT, m_lbOutput);
	//}}AFX_DATA_MAP
	SetTimer(0,1000,NULL);

#ifndef dBBECK
	m_lbOutput.AddString(" ");
	if (g_dsm.init())
			m_lbOutput.AddString(" Success Shared memory manager Init!!");
	else	m_lbOutput.AddString(" Fail Shared memory manager Init!!");
	m_lbOutput.AddString(" ");
#endif
}

BEGIN_MESSAGE_MAP(CSMManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CSMManagerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMManagerDlg message handlers

BOOL CSMManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	WriteServerInfoToRegistry(eST_SMMANAGER,"Avatar DBC S.M Manager");
	
	{
		char	strTitle[512];

		sprintf(strTitle,"RS Share Memory Manager. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);

		SetWindowText(strTitle);
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	char	strOutputText[512];

	sprintf(strOutputText,"sizeof(cPLAYER_DATA) %d",sizeof(cPLAYER_DATA));

	int	iIndex			=	m_lbOutput.AddString(strOutputText);

	m_lbOutput.SetTopIndex(iIndex);
	m_lbOutput.SetCurSel(iIndex);
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSMManagerDlg::OnPaint() 
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
HCURSOR CSMManagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSMManagerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
#ifndef dBBECK
	CDialog::OnTimer(nIDEvent);

	if	(nIDEvent	==	0)
	{
		char	strOutputText[512];
		int		iActorCount,iFailedActorCount;
		BOOL	bIsActive;

		g_dsm.lockCM();
		iActorCount			=	g_lpCdm->getCount();
		iFailedActorCount	=	g_lpCdm->getFailedDataCount();
		bIsActive			=	g_lpCdm->isActive();
		g_lpCdm->disactive();
		g_dsm.unlockCM();

#ifdef _FOR_KOREA
		if	(bIsActive)
			sprintf(strOutputText," ☎ 서버 정상   : 컨넥트 유저 수 %5d , 데이터 저장 실패후 대기중인 수 %d",iActorCount,iFailedActorCount);
		else
		{
			if	(iActorCount+iFailedActorCount == 0)
				sprintf(strOutputText," ♨ 서버 아직 안 켜짐");
			else
				sprintf(strOutputText," ◈ 서버 이상함 : 컨넥트 유저 수 %5d , 데이터 저장 실패후 대기중인 수 %d",iActorCount,iFailedActorCount);
		}
#else
		if (bIsActive)
					sprintf(strOutputText," Connected User Count %5d , Save Fail Count %d",iActorCount,iFailedActorCount);
		else
		{
			if (iActorCount+iFailedActorCount == 0)
					sprintf(strOutputText," ♨ Not Start Avatar Server");
			else	sprintf(strOutputText," Connected User Count %5d , Save Fail Count %d",iActorCount,iFailedActorCount);
		}
#endif

		int	iIndex			=	m_lbOutput.AddString(strOutputText);

		m_lbOutput.SetTopIndex(iIndex);
		m_lbOutput.SetCurSel(iIndex);
	}
#endif
}
void CSMManagerDlg::OnOK() 
{
#ifndef dBBECK
	int		iActorCount,iFailedActorCount;
	BOOL	bIsActive;

	g_dsm.lockCM();
	iActorCount			=	g_lpCdm->getCount();
	iFailedActorCount	=	g_lpCdm->getFailedDataCount();
	bIsActive			=	g_lpCdm->isActive();
	g_dsm.unlockCM();
	
	if (iActorCount || iFailedActorCount)
	{
		MessageBox("Don't quit this program!! Unsaved data is exist!!","QUIT",MB_ICONINFORMATION);
		return;
	}

	if (MessageBox("Are you really quit?","QUIT",MB_YESNO | MB_ICONQUESTION) == IDNO)	return;

	// TODO: Add extra validation here
	
	CDialog::OnOK();
#endif
}

void CSMManagerDlg::OnClose() 
{
	OnOK();
	// TODO: Add your message handler code here and/or call default
}
