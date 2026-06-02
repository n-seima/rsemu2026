// LOGIN.cpp : implementation file
//

#include "stdafx.h"
#include "AdminTool.h"
#include "LOGIN.h"
#include "AdminLogCode.h"

#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int		extLevel;
char	strUserName[32];
/////////////////////////////////////////////////////////////////////////////
// CLOGIN dialog

CLOGIN::CLOGIN(CWnd* pParent /*=NULL*/)
	: CDialog(CLOGIN::IDD, pParent)
{

	//{{AFX_DATA_INIT(CLOGIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	extLevel = 0;
}

void CLOGIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLOGIN)
	DDX_Control(pDX, IDC_EDIT_PW, m_ctlPw);
	DDX_Control(pDX, IDC_EDIT_ID, m_ctlName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLOGIN, CDialog)
	//{{AFX_MSG_MAP(CLOGIN)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLOGIN message handlers

void CLOGIN::OnOK() 
{
//	CDialog::OnOK();	return;
	// TODO: Add extra validation here
	if (IsDlgButtonChecked(IDC_INNER_SERVER))
	{
		g_lWhereisServer = eCOUNTRY_INNERTEST;
//		strcpy(g_strUserDBAddress,dINNERDBADDR);

		strcpy(g_strUserDBAddress,g_strInnerUserDBAddress);
		strcpy(g_strUserDBName,g_strInnerUserDBName);

		g_bIsRequireSendToolLog	=	FALSE;

		strcpy(strAvatarDBCId,"RedMaster");
		strcpy(strAccountDBCId,"RedMaster");
		strcpy(strLoginDBId,"RedMaster");
	}
	else
	{
		g_bIsRequireSendToolLog	=	(g_iToolLogPort > 0 && g_strToolLogIP[0] && stricmp(g_strToolLogIP,"0") != 0 && stricmp(g_strToolLogIP,"none") != 0);
		
#ifdef _FOR_CHINA		
		g_lWhereisServer = eCOUNTRY_CHINA;
#elif _FOR_JAPAN
		g_lWhereisServer = eCOUNTRY_JAPAN;
#elif _FOR_USA
		g_lWhereisServer = eCOUNTRY_USA;
#elif _FOR_INTER
		g_lWhereisServer = eCOUNTRY_INTER;
#elif _FOR_KOREA
		g_lWhereisServer = eCOUNTRY_KOREA;
#elif _FOR_THAI
		g_lWhereisServer = eCOUNTRY_THAI;
#endif
	}

	cLOG->outputf("g_DBAccount [CHECK!!] DB [%s] , id[%s], pw[%s] ,NAME[%s] \n", g_strUserDBAddress , strAccountDBCId,strAccountDBCPw,g_strUserDBName);
	if(!g_DBAccount.Connect(g_strUserDBAddress , strAccountDBCId,strAccountDBCPw,g_strUserDBName,1))
	{	CDialog::OnCancel();
		return;
	}

	cLOG->outputf("g_DBSrvInfo [CHECK!!] DB [%s] , id[%s], pw[%s] ,NAME[REDSTONE_INFO] \n", g_strUserDBAddress , strAccountDBCId,strAccountDBCPw);
	if(!g_DBSrvInfo.Connect(g_strUserDBAddress , strAccountDBCId,strAccountDBCPw,"REDSTONE_INFO",1))
	{
		g_DBAccount.Close();
		CDialog::OnCancel();
		return;
	}

	cLOG->outputf("g_DBAdmin [CHECK!!] DB [%s] , id[%s], pw[%s] ,NAME[ADMINTOOL_LOGIN] \n", g_strUserDBAddress , strAccountDBCId,strAccountDBCPw);
	if(!g_DBAdmin.Connect(g_strUserDBAddress , strAccountDBCId,strAccountDBCPw,"ADMINTOOL_LOGIN",1))
	{
		g_DBAccount.Close();
		g_DBSrvInfo.Close();
		CDialog::OnCancel();
		return;
	}
	char name[20],pw[20];
	m_ctlName.GetWindowText(name,18);
	m_ctlPw.GetWindowText(pw,18);

	if	(strcmp(name,"")==0 || strcmp(pw,"")==0)
	{
		MessageBox("Input your ID and PW, please !!");
		return;
	}

	CRegKey	reg;

	int iResult	=	reg.Open( g_hRegKey, g_lpstrRegistry);

	if (iResult == ERROR_SUCCESS)
	{
		reg.SetValue(name,"admin id");
	}
	m_ctlName.GetWindowText(strAdminId,dID_LENGTH);

	extLevel = g_DBAdmin.spAdminLogin(name,pw,12);	//	¸¶¸·.. Version

	if	(extLevel == -2)
	{
		CDialog::OnCancel();
		return;	
	}
	
	if	(extLevel == -1)
	{
		MessageBox("This version is incorrect!");
		return;	
	}

	switch(extLevel)
	{
	case 1:
		extLevel = eAL_NORMAL;
		break;
	case 2:
		extLevel = eAL_TESTER;
		break;
	case 3:
	case 4:
		extLevel = eAL_OPERATOR;
		break;
	case 5:
	case 8:
		extLevel = eAL_MASTER_OPERATER;
		break;
	case 6:
	case 16:
	case 32:
		extLevel = eAL_SYSTEM_OPERATER;
		break;
	}

	strcpy(strUserName,name);


	CDialog::OnOK();
}

void CLOGIN::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}


BOOL CLOGIN::OnInitDialog() 
{
	CDialog::OnInitDialog();

 	char	strCommandLine[1024];

	strcpy(strCommandLine,GetCommandLine());

	if	(strstr(strCommandLine,"__inner") || g_iToolType ==	eTOOL_FOR_BALANCE)
		CheckDlgButton(IDC_INNER_SERVER,TRUE);

#ifndef	_FOR_KOREA
	::ShowWindow(::GetDlgItem(m_hWnd,IDC_INNER_SERVER),SW_HIDE);
#endif

	CRegKey		reg;
	m_strLogId[0]	=	NULL;

	int iResult	=	reg.Open( g_hRegKey, g_lpstrRegistry);

	if (iResult == ERROR_SUCCESS)
	{	
		DWORD	count	=	sizeof(m_strLogId);
		reg.QueryValue((LPTSTR )m_strLogId,"admin id",&count);
	}

	if (strlen(m_strLogId) >= 4)	m_ctlName.SetWindowText(m_strLogId);
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
