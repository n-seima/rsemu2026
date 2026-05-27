// NEWNAME.cpp : implementation file
//

#include "stdafx.h"
#include "AdminTool.h"
#include "NEWNAME.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNEWNAME dialog


CNEWNAME::CNEWNAME(CWnd* pParent /*=NULL*/)
	: CDialog(CNEWNAME::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNEWNAME)
	m_strNewName = _T("");
	//}}AFX_DATA_INIT
}


void CNEWNAME::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNEWNAME)
	DDX_Control(pDX, IDC_NEW_NAME, m_ctlNewName);
	DDX_Text(pDX, IDC_NEW_NAME, m_strNewName);
	DDV_MaxChars(pDX, m_strNewName, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNEWNAME, CDialog)
	//{{AFX_MSG_MAP(CNEWNAME)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNEWNAME message handlers

void CNEWNAME::OnOK() 
{
	// TODO: Add extra validation here
	char newname[22];
	m_ctlNewName.GetWindowText(newname,22);
	if(strcmp(newname,"")==0){	MessageBox("Input new name");	return;	}
	
	CDialog::OnOK();
}
