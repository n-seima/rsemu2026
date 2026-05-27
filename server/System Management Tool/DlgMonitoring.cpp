// DlgMonitoring.cpp : implementation file
//

#include "stdafx.h"
#include "System Management Tool.h"
#include "DlgMonitoring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgMonitoring property page

IMPLEMENT_DYNCREATE(DlgMonitoring, CPropertyPage)

DlgMonitoring::DlgMonitoring() : CPropertyPage(DlgMonitoring::IDD)
{
	//{{AFX_DATA_INIT(DlgMonitoring)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

DlgMonitoring::~DlgMonitoring()
{
}

void DlgMonitoring::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgMonitoring)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgMonitoring, CPropertyPage)
	//{{AFX_MSG_MAP(DlgMonitoring)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgMonitoring message handlers
