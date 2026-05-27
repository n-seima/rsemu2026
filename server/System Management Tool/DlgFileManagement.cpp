// DlgFileManagement.cpp : implementation file
//

#include "stdafx.h"
#include "System Management Tool.h"
#include "DlgFileManagement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgFileManagement property page

IMPLEMENT_DYNCREATE(DlgFileManagement, CPropertyPage)

DlgFileManagement::DlgFileManagement() : CPropertyPage(DlgFileManagement::IDD)
{
	//{{AFX_DATA_INIT(DlgFileManagement)
	m_isDeleteSource = FALSE;
	m_isUsingScriptFile = FALSE;
	m_isControlFolder = FALSE;
	m_strScriptFile = _T("");
	m_strSourcePath = _T("");
	m_strTargetPath = _T("");
	//}}AFX_DATA_INIT
}

DlgFileManagement::~DlgFileManagement()
{
}

void DlgFileManagement::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgFileManagement)
	DDX_Check(pDX, IDC_CHK_DEL_SOURCE, m_isDeleteSource);
	DDX_Check(pDX, IDC_CHK_USE_SCRIPT, m_isUsingScriptFile);
	DDX_Check(pDX, IDC_CHK_WITH_FOLDER, m_isControlFolder);
	DDX_Text(pDX, IDC_EDIT_SCRIPTFILE, m_strScriptFile);
	DDV_MaxChars(pDX, m_strScriptFile, 512);
	DDX_Text(pDX, IDC_EDIT_SOURCE, m_strSourcePath);
	DDV_MaxChars(pDX, m_strSourcePath, 512);
	DDX_Text(pDX, IDC_EDIT_TARGET, m_strTargetPath);
	DDV_MaxChars(pDX, m_strTargetPath, 512);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgFileManagement, CPropertyPage)
	//{{AFX_MSG_MAP(DlgFileManagement)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgFileManagement message handlers
