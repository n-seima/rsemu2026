// DlgTimeSetting.cpp : implementation file
//

#include "stdafx.h"
#include "System Management Tool.h"
#include "DlgTimeSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgTimeSetting property page

IMPLEMENT_DYNCREATE(DlgTimeSetting, CPropertyPage)

DlgTimeSetting::DlgTimeSetting() : CPropertyPage(DlgTimeSetting::IDD)
{
	//{{AFX_DATA_INIT(DlgTimeSetting)
	m_isUseTimeSetting = FALSE;
	m_tiTime = 0;
	m_tiDate = 0;
	m_strTimeInterval = _T("");
	//}}AFX_DATA_INIT
}

DlgTimeSetting::~DlgTimeSetting()
{
}

void DlgTimeSetting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgTimeSetting)
	DDX_Check(pDX, IDC_CHK_USE_TIME, m_isUseTimeSetting);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_tiTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_tiDate);
	DDX_CBString(pDX, IDC_CMB_CLOCK, m_strTimeInterval);
	DDV_MaxChars(pDX, m_strTimeInterval, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgTimeSetting, CPropertyPage)
	//{{AFX_MSG_MAP(DlgTimeSetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgTimeSetting message handlers

BOOL DlgTimeSetting::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
