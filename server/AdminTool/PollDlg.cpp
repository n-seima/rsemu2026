// PollDlg.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "PollDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cPollDlg dialog


cPollDlg::cPollDlg()
		: CPropertyPage(cPollDlg::IDD)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(cPollDlg)
	//}}AFX_DATA_INIT
}

void cPollDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void cPollDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cPollDlg)
	DDX_Control(pDX, IDC_BEGIN_DAY, m_beginDay);
	DDX_Control(pDX, IDC_END_DAY, m_endDay);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(cPollDlg, CDialog)
	//{{AFX_MSG_MAP(cPollDlg)
	ON_BN_CLICKED(IDC_ADD_POLL, OnAddPoll)
	ON_BN_CLICKED(IDC_CHANGE_SELECT_POLL_INFO, OnChangeSelectPollInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(cPollDlg, CDialog)
	//{{AFX_DISPATCH_MAP(cPollDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IcPollDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {CB284235-5CE0-4245-9584-36FB0B48C1FE}
static const IID IID_IcPollDlg =
{ 0xcb284235, 0x5ce0, 0x4245, { 0x95, 0x84, 0x36, 0xfb, 0xb, 0x48, 0xc1, 0xfe } };

BEGIN_INTERFACE_MAP(cPollDlg, CDialog)
	INTERFACE_PART(cPollDlg, IID_IcPollDlg, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cPollDlg message handlers
BOOL
cPollDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void
cPollDlg::OnAddPoll() 
{
	SYSTEMTIME	beginTime,endTime;

	MonthCal_GetCurSel(m_beginDay.m_hWnd,&beginTime);	//	현재 선택한 일자를 얻어온다.
	MonthCal_GetCurSel(m_endDay.m_hWnd,&endTime);	//	현재 선택한 일자를 얻어온다.

	// TODO: Add your control notification handler code here
}

void
cPollDlg::OnChangeSelectPollInfo() 
{
	
}