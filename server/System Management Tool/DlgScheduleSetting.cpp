// DlgScheduleSetting.cpp : implementation file
//

#include "stdafx.h"
#include "System Management Tool.h"
#include "DlgScheduleSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgScheduleSetting dialog


DlgScheduleSetting::DlgScheduleSetting(CWnd* pParent /*=NULL*/)
	: CDialog(DlgScheduleSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgScheduleSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


	m_iDlgType = eST_Backup;
}


void DlgScheduleSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgScheduleSetting)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgScheduleSetting, CDialog)
	//{{AFX_MSG_MAP(DlgScheduleSetting)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgScheduleSetting message handlers

BOOL DlgScheduleSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Sheet.AddPage(&m_pageTimeSetting);
	switch(m_iDlgType)
	{
	case eST_Backup:	
		m_Sheet.AddPage(&m_pageFileManagement);
		break;
	case eST_File:
		m_Sheet.AddPage(&m_pageFileManagement);
		break;
	case eST_Process:
		m_Sheet.AddPage(&m_pageCheckProcess);
		break;
	case eST_Monitoring:
		m_Sheet.AddPage(&m_pageMonitoring);
		break;
	}
	m_Sheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	m_Sheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_Sheet.ModifyStyle( 0, WS_TABSTOP | WS_THICKFRAME );

	CRect rcSheet;

	GetDlgItem( IDC_SHEET )->GetWindowRect( &rcSheet );
	ScreenToClient( &rcSheet );

	m_Sheet.SetWindowPos( NULL, rcSheet.left-7, rcSheet.top-7, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgScheduleSetting::OnCancel() 
{
	// TODO: Add your control notification handler code here
	
	CDialog::OnCancel();
}

void DlgScheduleSetting::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
