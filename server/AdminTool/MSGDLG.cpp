// MSGDLG.cpp : implementation file
//

#include "stdafx.h"
#include "AdminTool.h"
#include "MSGDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMSGDLG dialog


CMSGDLG::CMSGDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CMSGDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMSGDLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMSGDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMSGDLG)
	DDX_Control(pDX, IDC_PROGRESS, m_ctlProgress);
	DDX_Control(pDX, IDC_MSG, m_ctlMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMSGDLG, CDialog)
	//{{AFX_MSG_MAP(CMSGDLG)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSGDLG message handlers
