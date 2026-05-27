// DlgRestTime.cpp : implementation file
//

#include "stdafx.h"
#include "SSViewer.h"
#include "DlgRestTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRestTime dialog


CDlgRestTime::CDlgRestTime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRestTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRestTime)
	m_iRestTime = 0;
	//}}AFX_DATA_INIT
}


void CDlgRestTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRestTime)
	DDX_Control(pDX, IDC_REST_TIME, m_ctlRestTime);
	DDX_Text(pDX, IDC_REST_TIME, m_iRestTime);
	DDV_MinMaxInt(pDX, m_iRestTime, 5, 300);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRestTime, CDialog)
	//{{AFX_MSG_MAP(CDlgRestTime)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRestTime message handlers
