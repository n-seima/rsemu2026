// DebuggingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RedStone_BroadCastServer.h"
#include "DebuggingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDebuggingDialog dialog


CDebuggingDialog::CDebuggingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDebuggingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDebuggingDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDebuggingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebuggingDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDebuggingDialog, CDialog)
	//{{AFX_MSG_MAP(CDebuggingDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebuggingDialog message handlers

BOOL CDebuggingDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Sheet.AddPage(&m_pageUserInfo);

	m_Sheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	m_Sheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_Sheet.ModifyStyle( 0, WS_TABSTOP | WS_THICKFRAME );

	CRect rcSheet;
	GetDlgItem( IDC_MAINTAB_INDEBUGGING )->GetWindowRect( &rcSheet );
	ScreenToClient( &rcSheet );

	m_Sheet.SetWindowPos( NULL, rcSheet.left, rcSheet.top, rcSheet.right, rcSheet.bottom, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
