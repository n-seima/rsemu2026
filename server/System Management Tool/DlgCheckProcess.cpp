// DlgCheckProcess.cpp : implementation file
//

#include "stdafx.h"
#include "System Management Tool.h"
#include "DlgCheckProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgCheckProcess property page

IMPLEMENT_DYNCREATE(DlgCheckProcess, CPropertyPage)

DlgCheckProcess::DlgCheckProcess() : CPropertyPage(DlgCheckProcess::IDD)
{
	//{{AFX_DATA_INIT(DlgCheckProcess)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

DlgCheckProcess::~DlgCheckProcess()
{
}

void DlgCheckProcess::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgCheckProcess)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgCheckProcess, CPropertyPage)
	//{{AFX_MSG_MAP(DlgCheckProcess)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgCheckProcess message handlers
