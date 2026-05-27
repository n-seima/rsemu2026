// PAGE_NOTICE.cpp : implementation file
//

#include "stdafx.h"
#include "AdminTool.h"
#include "PAGE_NOTICE.h"
#include "cPACKET_GAMESERVER.h"
#include "cPACKET_WORLDSERVER.h"
#include "cPACKET_CONTROLSERVER.h"
#include "cPACKET_TOOLLOGSERVER.h"
#include "AdminLogCode.h"
#include "CMessage.h"
#include "cADO_ADMIN.h"
#include "AdminCommand.h"
#include "cSRVUTIL.h"

#include <io.h>
#include "cScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAdminCommand	g_adCommand;
cADO_BASE		cADO;

/////////////////////////////////////////////////////////////////////////////
// CPAGE_NOTICE property page

IMPLEMENT_DYNCREATE(CPAGE_NOTICE, CPropertyPage)

CPAGE_NOTICE::CPAGE_NOTICE() : CPropertyPage(CPAGE_NOTICE::IDD)
{
	//{{AFX_DATA_INIT(CPAGE_NOTICE)
	//}}AFX_DATA_INIT
}

CPAGE_NOTICE::~CPAGE_NOTICE()
{
}

void CPAGE_NOTICE::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPAGE_NOTICE)
	DDX_Control(pDX, IDC_ERASEMT, m_ctlEraseMt);
	DDX_Control(pDX, IDC_SENDDIRECT, m_ctlISSendDirectly);
	DDX_Control(pDX, IDC_WHO, m_ctlWho);
	DDX_Control(pDX, IDC_MSG_FORRESERVE, m_ctlMsg);
	DDX_Control(pDX, IDC_ISSEND_ALLSERVER, m_ctlIsSendtoAllServer);
	DDX_Control(pDX, IDC_ENDTIME_TIME, m_ctlFinishTime);
	DDX_Control(pDX, IDC_ENDTIME_DATE, m_ctlFinishDate);
	DDX_Control(pDX, IDC_PARAM1_FORRESERVE, m_ctlParam1);
	DDX_Control(pDX, IDC_RELEASE_TIME, m_ctlReleaseTime);
	DDX_Control(pDX, IDC_RELEASE_DATE, m_ctlReleaseDate);
	DDX_Control(pDX, IDC_WORK_FORRESERVE, m_ctlReserveCommand);
	DDX_Control(pDX, IDC_REGCOMMAND_LIST, m_ctlCommandList);
	DDX_Control(pDX, IDC_ISUSE_ENDTIME, m_ctlIsUseEndTime);
	DDX_Control(pDX, IDC_DBINFOTYPE, m_ctlDbInfoType);
	DDX_Control(pDX, IDC_MAKEDBC, m_ctlMakeDBCFile);
	DDX_Control(pDX, IDC_DBPASSWORDINPUT, m_ctlDBPassInput);
	DDX_Control(pDX, IDC_DBPASSWORD, m_ctlDBPassText);
	DDX_Control(pDX, IDC_DBNAMEINPUT, m_ctlDBNameInput);
	DDX_Control(pDX, IDC_DBNAME, m_ctlDBNameText);
	DDX_Control(pDX, IDC_DBACCOUNTINPUT, m_ctlDBAccountInput);
	DDX_Control(pDX, IDC_DBACCOUNT, m_ctlDBAccountText);
	DDX_Control(pDX, IDC_DBIPVALUE, m_ctlDBIp);
	DDX_Control(pDX, IDC_EDIT_FIXC_NOTICE, m_ctlFixcNotice);
	DDX_Control(pDX, IDC_FIXC_MIN, m_ctlFixc_Min);
	DDX_Control(pDX, IDC_FIXC_HOUR, m_ctlFixc_Hour);
	DDX_Control(pDX, IDC_FIXC_WEEKDAY, m_ctlFixc_WeekDay);
	DDX_Control(pDX, IDC_EDIT2, m_ctlNewPass);
	DDX_Control(pDX, IDC_EDIT1, m_ctlOldPass);
	DDX_Control(pDX, IDC_COMBO2, m_ctlLEV_LIST);
	DDX_Control(pDX, IDC_COMBO1, m_ctlID_LIST);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPAGE_NOTICE, CPropertyPage)
	//{{AFX_MSG_MAP(CPAGE_NOTICE)
	ON_BN_CLICKED(IDC_ADMIN_LOAD, OnAdminLoad)
	ON_BN_CLICKED(IDC_ADMIN_PASSCHANGE, OnAdminPasschange)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_ADMIN_CHANGELEVEL, OnAdminChangelevel)
	ON_BN_CLICKED(IDC_CHANGE_FIXCHECK, OnChangeFixcheck)
	ON_BN_CLICKED(IDC_CHECK_FIXCHECKTIME, OnCheckFixchecktime)
	ON_BN_CLICKED(IDC_KILLUSERS, OnKillusers)
	ON_BN_CLICKED(IDC_CHANGE_DELETE, OnChangeDelete)
	ON_BN_CLICKED(IDC_CREATEACCOUNT, OnCreateaccount)
	ON_BN_CLICKED(IDC_MAKEDBC, OnMakedbc)
	ON_BN_CLICKED(IDC_ISUSE_ENDTIME, OnIsuseEndtime)
	ON_BN_CLICKED(IDC_GETCOMMANDLIST_BTN, OnGetcommandlistBtn)
	ON_CBN_SELCHANGE(IDC_WORK_FORRESERVE, OnSelchangeWorkForreserve)
	ON_BN_CLICKED(IDC_RESERVE_BTN, OnReserveBtn)
	ON_CBN_SELCHANGE(IDC_REGCOMMAND_LIST, OnSelchangeRegcommandList)
	ON_BN_CLICKED(IDC_MODIFY_BTN, OnModifyBtn)
	ON_BN_CLICKED(IDC_CANCEL_BTN, OnCancelBtn)
	ON_BN_CLICKED(IDC_SENDDIRECT, OnSenddirect)
	ON_BN_CLICKED(IDC_ERASEMT, OnErasemt)
	ON_BN_CLICKED(IDC_ADMIN_DELETE, OnAdminDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPAGE_NOTICE message handlers

BOOL CPAGE_NOTICE::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

#ifndef _FOR_KOREA
	SetDlgItemText(IDC_RESERVE_COMMAND,g_strDlgText[176]);
	SetDlgItemText(IDC_REGCOMMAND_LIST_STATIC,g_strDlgText[177]);
	SetDlgItemText(IDC_RELEASE_TIME_STATIC,g_strDlgText[178]);
	SetDlgItemText(IDC_ENDTIME,g_strDlgText[179]);
	SetDlgItemText(IDC_COMMAND_TARGET_STATIC,g_strDlgText[180]);
	SetDlgItemText(IDC_MSG_STATIC,g_strDlgText[181]);
	SetDlgItemText(IDC_COMMAND_TYPE_STATIC,g_strDlgText[182]);
	SetDlgItemText(IDC_RESERVE_BTN,g_strDlgText[183]);
	SetDlgItemText(IDC_MODIFY_BTN,g_strDlgText[184]);
	SetDlgItemText(IDC_CANCEL_BTN,g_strDlgText[185]);
	SetDlgItemText(IDC_GETCOMMANDLIST_BTN,g_strDlgText[186]);
	SetDlgItemText(IDC_RESERVATION_INSPECTION,g_strDlgText[187]);

	SetDlgItemText(IDC_SEND_ALL_MSG,g_strDlgText[88]);
	SetDlgItemText(IDC_SEND_MSG,g_strDlgText[89]);
	SetDlgItemText(IDC_KILLUSERS,g_strDlgText[90]);
	SetDlgItemText(IDC_KILL_WORLD,g_strDlgText[91]);
	SetDlgItemText(IDC_SEND_NOTICE,g_strDlgText[92]);
	SetDlgItemText(IDC_SAVE_NOTICE,g_strDlgText[93]);
	SetDlgItemText(IDC_SYNC_NOTICE,g_strDlgText[94]);
	SetDlgItemText(IDC_STATIC1,g_strDlgText[95]);
	//	중간에 빈건 아직 안 만들어진 기능들이다.
	SetDlgItemText(IDC_STATIC100,g_strDlgText[109]);
	SetDlgItemText(IDC_ADMIN_LOAD,g_strDlgText[110]);
	SetDlgItemText(IDC_ADMIN_PASSCHANGE,g_strDlgText[111]);
	SetDlgItemText(IDC_ADMIN_DELETE,g_strDlgText[112]);
	SetDlgItemText(IDC_ADMIN_CHANGELEVEL,g_strDlgText[113]);
//	SetDlgItemText(IDC_RESERVATION_INSPECTION,g_strDlgText[150]);
	SetDlgItemText(IDC_CHECK_FIXCHECKTIME,g_strDlgText[151]);
	SetDlgItemText(IDC_CHANGE_FIXCHECK,g_strDlgText[152]);
	SetDlgItemText(IDC_CHANGE_DELETE,g_strDlgText[153]);
	SetDlgItemText(IDC_STATIC101,g_strDlgText[154]);
	SetDlgItemText(IDC_STATIC102,g_strDlgText[155]);
	SetDlgItemText(IDC_STATIC105,g_strDlgText[156]);
	SetDlgItemText(IDC_SENDDIRECT,"Send Directly");
	SetDlgItemText(IDC_ERASEMT,"No Maintenance");

	if(extLevel<eAL_SYSTEM_OPERATER)
	{
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RESERVE_COMMAND),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_DATE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISUSE_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_TARGET_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISSEND_ALLSERVER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_TYPE_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC1),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM1_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC2),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM2_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RESERVE_BTN),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MODIFY_BTN),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_CANCEL_BTN),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_REGCOMMAND_LIST_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_REGCOMMAND_LIST),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_WHO),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_WHO),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ERASEMT),SW_HIDE);
	}
#endif
	if(extLevel<eAL_MASTER_OPERATER)
	{	//	DB CONNECTION INFO 파일을 만들수 있다.
		m_ctlDBPassInput.ShowWindow(SW_HIDE);
		m_ctlDBPassText.ShowWindow(SW_HIDE);
		m_ctlDBNameInput.ShowWindow(SW_HIDE);
		m_ctlDBNameText.ShowWindow(SW_HIDE);
		m_ctlDBAccountInput.ShowWindow(SW_HIDE);
		m_ctlDBAccountText.ShowWindow(SW_HIDE);
		m_ctlDBIp.ShowWindow(SW_HIDE);
		m_ctlMakeDBCFile.ShowWindow(SW_HIDE);
		m_ctlDbInfoType.ShowWindow(SW_HIDE);
	}else{
		m_ctlDbInfoType.InsertString(0,"Account DB Info");
		m_ctlDbInfoType.InsertString(1,"Avatar DB Info");
		m_ctlDbInfoType.InsertString(2,"Purchase DB Info");
		m_ctlDbInfoType.InsertString(3,"Chatting DB Info");
		m_ctlDbInfoType.InsertString(4,"SS DB Info");
		m_ctlDbInfoType.SetCurSel(0);
	}

	m_ctlFixc_WeekDay.InsertString(0,"Sun");
	m_ctlFixc_WeekDay.InsertString(1,"Mon");
	m_ctlFixc_WeekDay.InsertString(2,"Tues");
	m_ctlFixc_WeekDay.InsertString(3,"Wen");
	m_ctlFixc_WeekDay.InsertString(4,"Thur");
	m_ctlFixc_WeekDay.InsertString(5,"Fri");
	m_ctlFixc_WeekDay.InsertString(6,"Sat");
	m_ctlFixc_WeekDay.SetCurSel(0);

	for(int h=0; h<24;h++)
	{
		char strhour[10];
		sprintf(strhour,"%02d",h);
		m_ctlFixc_Hour.InsertString(h,strhour);
	}
	m_ctlFixc_Hour.SetCurSel(0);

	for(h=0; h<60;h++)
	{
		char strhour[10];
		sprintf(strhour,"%02d",h);
		m_ctlFixc_Min.InsertString(h,strhour);
	}
	m_ctlFixc_Min.SetCurSel(0);

	for(int i =0; i<dMAXADMINCOUNT;i++)
		m_ctAdminList[i].iLev = 0xffff;

	m_ctlLEV_LIST.AddString("0");
	m_ctlLEV_LIST.AddString("1");
	m_ctlLEV_LIST.AddString("2");
	m_ctlLEV_LIST.AddString("3");
	m_ctlLEV_LIST.AddString("4");
	if(extLevel==eAL_MASTER_OPERATER)
		m_ctlLEV_LIST.AddString("5");
	//	기본 명령을 추가한다.
	for(int j=0;j<dCOMMAND_COUNT;j++)
		m_ctlReserveCommand.InsertString(j,g_CT_Text[j]);

	g_adCommand.init(g_strUserDBAddress,"REDSTONE_INFO");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPAGE_NOTICE::SetList()
{
	int icount = m_ctlID_LIST.GetCount();
	for(int i = 0;i<icount;i++)
		m_ctlID_LIST.DeleteString(0);

	for(int z =0; z<dMAXADMINCOUNT;z++)
	{
		if(m_ctAdminList[z].iLev == 0xffff)	continue;
		m_ctlID_LIST.AddString(m_ctAdminList[z].strName);
	}
	m_ctlID_LIST.SetCurSel(0);
	OnSelchangeCombo1();

	m_ctlOldPass.SetWindowText("");
	m_ctlNewPass.SetWindowText("");

}

void CPAGE_NOTICE::OnAdminLoad() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#else
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#endif

	for(int z =0; z<dMAXADMINCOUNT;z++)
		m_ctAdminList[z].iLev = 0xffff;
	//	작업하고..
	{
		_RecordsetPtr  pRs = NULL;
		_variant_t     v_bufferdata;
		char	       str_Query[1024];
		int i_RecCount;
		int i;
		char *pBuf = NULL;
		HRESULT hr = CoInitialize(0);

		sprintf(str_Query,"SELECT * FROM AdminTool_login");
		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)g_DBAdmin.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		i_RecCount = pRs->RecordCount;

		int rcount = 0;
		if(!FAILED(hr))
		{ 
			for(i = 0 ;i < i_RecCount ;i++)
			{
				v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;	//	lev
				m_ctAdminList[rcount].iLev = v_bufferdata.intVal;
				if(m_ctAdminList[rcount].iLev>=6 && extLevel<eAL_SYSTEM_OPERATER)
				{
					pRs->MoveNext();
					continue;
				}
				v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;	//	name
				strcpy(m_ctAdminList[rcount].strName,(_bstr_t)v_bufferdata);
				v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;	//	lastdate
				m_ctAdminList[rcount].iLastConnectDate = v_bufferdata.intVal;
				rcount++;
				pRs->MoveNext();
			}
		}
	}
	SetList();
}

void CPAGE_NOTICE::OnAdminPasschange() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#else
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#endif
	if(MessageBox(g_strToolMessage[13],"Check!! Change the password!!",MB_YESNO)==IDNO)	return;

	int		iSelected = 0;
	char	strOldPass[20],strNewPass[20];
	m_ctlOldPass.GetWindowText(strOldPass,20);
	m_ctlNewPass.GetWindowText(strNewPass,20);

	iSelected = m_ctlID_LIST.GetCurSel();
	if(m_ctAdminList[iSelected].iLev == 0xffff)
	{
		MessageBox(g_strToolMessage[14]);
		return;
	}

	//	5레벨이 다른 5레벨의 비번을 바꿀수 있다. 수정해야 되지 않을까?? - 중국의 경우 문제가 될 수 있다.

	if(stricmp(strOldPass,"")==0 && stricmp(strNewPass,"")==0)
	{
		_RecordsetPtr  pRs = NULL;
		_variant_t     v_bufferdata;
		char	       str_Query[1024];		int i_RecCount;
		char *pBuf = NULL;		HRESULT hr = CoInitialize(0);

		sprintf(str_Query,"SELECT passwd FROM AdminTool_login WHERE name = '%s'",m_ctAdminList[iSelected].strName);		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)g_DBAdmin.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		i_RecCount = pRs->RecordCount;
		char txt[100];

		if(m_ctAdminList[iSelected].iLev < 6 )
		{
			if(!FAILED(hr))
			{ 
				v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
				char	pass[32];
				strcpy(pass,(_bstr_t)v_bufferdata);
				sprintf(txt,g_strToolMessage[15],m_ctAdminList[iSelected].strName,pass);
			}else
			{
				sprintf(txt,g_strToolMessage[16],m_ctAdminList[iSelected].strName);
			}
		}
		pRs->Close();
		MessageBox(txt);
		return;
	}
	
	if(stricmp(strOldPass,"")==0)
	{
		MessageBox(g_strToolMessage[17]);
		return;
	}
	
	if(stricmp(strNewPass,"")==0)
	{
		MessageBox(g_strToolMessage[18]);
		return;
	}

	//	작업하고..
	{
		_RecordsetPtr  pRs = NULL;
		_variant_t     v_bufferdata;
		char	       str_Query[1024];
		char *pBuf = NULL;
		HRESULT hr = CoInitialize(0);

		sprintf(str_Query,"UPDATE AdminTool_login SET passwd='%s' WHERE name='%s' and passwd='%s'",strNewPass,m_ctAdminList[iSelected].strName,strOldPass);
		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)g_DBAdmin.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

		if(!FAILED(hr))	MessageBox("SUCCESS!!");
		else			MessageBox("FAIL!!");
	}
	OnAdminLoad();
}


void CPAGE_NOTICE::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	int iSelected = m_ctlID_LIST.GetCurSel();
	m_ctlID_LIST.SetCurSel(iSelected);
	m_ctlLEV_LIST.SetCurSel(m_ctAdminList[iSelected].iLev);
}

void CPAGE_NOTICE::OnAdminDelete() 
{
	// TODO: Add your control notification handler code here
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#else
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#endif

	if(MessageBox("Do you wanna remove this account?","WARNING!!",MB_YESNO)==IDNO)	return;
	char	strNewId[20];

	m_ctlID_LIST.GetWindowText(strNewId,20);
	if(stricmp(strNewId,"")==0)		{	MessageBox("account name is empty!!");	return;	}

	//	작업하고..
	{
		_RecordsetPtr  pRs = NULL;
		_variant_t     v_bufferdata;
		char	       str_Query[1024];
		char *pBuf = NULL;
		HRESULT hr = CoInitialize(0);

		sprintf(str_Query,"DELETE FROM AdminTool_login WHERE name='%s'",strNewId);
		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)g_DBAdmin.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

		if(!FAILED(hr))
			MessageBox("Complete!!");
		else
			MessageBox("ERROR!!");
	}
	OnAdminLoad();
}


void CPAGE_NOTICE::OnAdminChangelevel() 
{
	// TODO: Add your control notification handler code here
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#else
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#endif
	if(MessageBox(g_strToolMessage[19],"WARNING",MB_YESNO)==IDNO)	return;
	char	strNewId[20],strLev[2];
	int		iLev;

	m_ctlID_LIST.GetWindowText(strNewId,20);
	m_ctlLEV_LIST.GetWindowText(strLev,2);
	iLev = atoi(strLev);

	if(stricmp(strNewId,"")==0)		{	MessageBox(g_strToolMessage[18]);	return;	}
	if(iLev<0 || iLev>5)			{	MessageBox(g_strToolMessage[20]);	return;	}

	//	작업하고..
	{
		_RecordsetPtr  pRs = NULL;
		_variant_t     v_bufferdata;
		char	       str_Query[1024];
		char *pBuf = NULL;
		HRESULT hr = CoInitialize(0);

		switch(iLev)	// 제곱 공식이 뭐더라.. -_-;
		{
		case 2:	iLev = 4;	break;
		case 3:	iLev = 8;	break;
		case 5:	
#ifdef _FOR_KOREA
			iLev = 16;	
#else
			iLev = 32;	//	태국만 이런가. -_-
#endif
			break;
		case 6:	iLev = 32;	break;
		}
		sprintf(str_Query,"UPDATE AdminTool_login SET lev=%d WHERE name='%s'",iLev,strNewId);
		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)g_DBAdmin.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

		if(!FAILED(hr))
			MessageBox(g_strToolMessage[21]);
		else
			MessageBox(g_strToolMessage[22]);
	}
	OnAdminLoad();
}

void CPAGE_NOTICE::OnChangeFixcheck() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if(g_iSelectedServer!=-1 && !sList[g_iSelectedServer].isConnect)
	{
		MessageBox(g_strToolMessage[10]);
		return;
	}

	char	strWeek[2],strHour[3],strMin[3];
	WORD	wWeek,wHour,wMin;
	char	strMsg[98];

	m_ctlFixc_WeekDay.GetWindowText(strWeek,4);
	m_ctlFixc_Hour.GetWindowText(strHour,3);
	m_ctlFixc_Min.GetWindowText(strMin,3);
	m_ctlFixcNotice.GetWindowText(strMsg,98);

	wWeek = m_ctlFixc_WeekDay.GetCurSel();	//	0 ~ 6
	wHour = atoi(strHour);	//	0 ~ 23
	wMin  = atoi(strMin);	//	0 ~ 59

	if(wWeek>6)	{	MessageBox(g_strToolMessage[23]);	return;	}
	if(wHour>23){	MessageBox(g_strToolMessage[24]);	return;	}
	if(wMin>59) {	MessageBox(g_strToolMessage[25]);	return;	}
	if(strcmp(strMsg,"")==0){	MessageBox(g_strToolMessage[26]);	return;	}

	char	tmp[1024];
	sprintf(tmp, g_strToolMessage[27],sList[g_iSelectedServer].strName,strWeek,wHour,wMin,strMsg);

	if(MessageBox(tmp,"Do you change the maintenance time?",MB_YESNO)==IDNO)	return;

	SERVERStoWORLD_CHANGE_FIXCHECK		sPacket;
	sPacket.base.set(sizeof(SERVERStoWORLD_CHANGE_FIXCHECK),dSERVERStoWORLD_CHANGE_FIXCHECK);
	sPacket.wWeek = wWeek;
	sPacket.wHour = wHour;
	sPacket.wMin  = wMin;
	strcpy(sPacket.strNoticeMsg,strMsg);

	if(g_iSelectedServer!=-1 && sList[g_iSelectedServer].isConnect)
		sList[g_iSelectedServer].worldsock.SendPacket((char *)&sPacket,sPacket.base.wSize);	
	else
		MessageBox(g_strToolMessage[10]);
}

void CPAGE_NOTICE::OnCheckFixchecktime() 
{
	// TODO: Add your control notification handler code here
	if(g_iSelectedServer==-1)
	{	MessageBox("Select a server!");
		return;
	}
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	SERVERStoWORLD_GET_FIXCHECK		sPacket;
	sPacket.base.set(sizeof(SERVERStoWORLD_GET_FIXCHECK),dSERVERStoWORLD_GET_FIXCHECK);
	if(g_iSelectedServer!=-1 && sList[g_iSelectedServer].isConnect)
		sList[g_iSelectedServer].worldsock.SendPacket((char *)&sPacket,sPacket.base.wSize);	
	else
		MessageBox(g_strToolMessage[10]);
}

void CPAGE_NOTICE::OnKillusers() 
{
	// TODO: Add your control notification handler code here
	if(g_iSelectedServer==-1)
	{	MessageBox("Select a server!");
		return;
	}
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	g_DBAccount.spDeleteLoginInfo(sList[g_iSelectedServer].strName);
}

void CPAGE_NOTICE::OnChangeDelete() 
{
	// TODO: Add your control notification handler code here
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if(g_iSelectedServer!=-1 && !sList[g_iSelectedServer].isConnect)
	{
		MessageBox(g_strToolMessage[10]);
		return;
	}
	char	tmp[512];
	sprintf(tmp,"selected Server Name [%s]",sList[g_iSelectedServer].strName);
	if(MessageBox(tmp,"Do you delete The maintenance time?",MB_YESNO)==IDNO)	return;

	SERVERStoWORLD_CHANGE_FIXCHECK		sPacket;
	sPacket.base.set(sizeof(SERVERStoWORLD_CHANGE_FIXCHECK),dSERVERStoWORLD_CHANGE_FIXCHECK);
	sPacket.wWeek = 0;
	sPacket.wHour = -1;
	sPacket.wMin  = 0;
	strcpy(sPacket.strNoticeMsg,"");

	if(g_iSelectedServer!=-1 && sList[g_iSelectedServer].isConnect)
		sList[g_iSelectedServer].worldsock.SendPacket((char *)&sPacket,sPacket.base.wSize);	
	else
		MessageBox(g_strToolMessage[10]);
}

void CPAGE_NOTICE::OnCreateaccount() 
{
	// TODO: Add your control notification handler code here
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#else
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#endif

	if(MessageBox("CREATE ADMINTOOL ACCOUNT","Do you wanna create an account infomation?",MB_YESNO)==IDNO)	return;
	
	char	strNewId[20],strNewPass1[20],strNewPass2[20],strLev[2];
	int		iLev;

	m_ctlID_LIST.GetWindowText(strNewId,20);
	m_ctlOldPass.GetWindowText(strNewPass1,20);
	m_ctlNewPass.GetWindowText(strNewPass2,20);
	m_ctlLEV_LIST.GetWindowText(strLev,2);
	iLev = atoi(strLev);
	if(extLevel==eAL_MASTER_OPERATER && (iLev==5 || iLev==6))
	{
		MessageBox("FAIL","Impossible!!",MB_OK);	return;
	}

	if(stricmp(strNewId,"")==0)		{	MessageBox("Input Account");	return;	}
	//	이름이 중복되었는지 확인해본다.
	for(int z =0; z<dMAXADMINCOUNT;z++)
	{
		if(stricmp(m_ctAdminList[z].strName,strNewId)==0)
		{
			MessageBox("This name is exist.");
			return;
		}
	}

	if(stricmp(strNewPass1,"")==0)	{	MessageBox("Input Password.");	return;	}
	if(stricmp(strNewPass2,"")==0)	{	MessageBox("Input Confime Password.");	return;	}
	if(strcmp(strNewPass1,strNewPass2)!=0){MessageBox("Wrong Password");	return;	}
	if(extLevel==eAL_MASTER_OPERATER)
	{
		if(iLev<0 || iLev>5)			{	MessageBox("Wrong Level Setting");		return;	}
	}

	//	작업하고..
	{
		_RecordsetPtr  pRs = NULL;
		_variant_t     v_bufferdata;
		char	       str_Query[1024];
		char *pBuf = NULL;
		HRESULT hr = CoInitialize(0);

		sprintf(str_Query,"INSERT INTO AdminTool_login VALUES('%s','%s',%d,20040101,0)",strNewId,strNewPass1,iLev);
		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)g_DBAdmin.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

		if(!FAILED(hr))
			MessageBox("Success!!");
		else
			MessageBox("FAIL!! Contact your sql db administrator!!");
	}
	OnAdminLoad();
}

void CPAGE_NOTICE::OnMakedbc() 
{
	// TODO: Add your control notification handler code here
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#else
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#endif

	char dbname[128];
	char id[64];
	char pw[64];
	char ip[32];

	m_ctlDBNameInput.GetWindowText(dbname,128);
	m_ctlDBAccountInput.GetWindowText(id,64);
	m_ctlDBPassInput.GetWindowText(pw,64);
	m_ctlDBIp.GetWindowText(ip,64);

	if(strcmp(dbname,"")==0)	{	MessageBox("ERROR","Input DB Name");	return;	}
	if(strcmp(id,"")==0)	{	MessageBox("ERROR","Input Account");	return;	}
	if(strcmp(pw,"")==0)	{	MessageBox("ERROR","Input Password Name");	return;	}

	char filename[128];
	switch(m_ctlDbInfoType.GetCurSel())
	{
	case 1:	//	avatar
		sprintf(filename,"AvatarDbInfo.bin");
		break;
	case 2:	//	purchase
		sprintf(filename,"PurchaseDbInfo.bin");
		break;
	case 3:	//	chatting db
		sprintf(filename,"ChattingLogDBInfo.bin");
		break;
	case 4:	//	server status db
		if(strcmp(ip,"")==0)	{	MessageBox("ERROR","Input DB IP");	return;	}

		sprintf(filename,"SSDBInfo.bin");
		break;
	default:	//	0 - account
		sprintf(filename,"AccountDbInfo.bin");
		break;
	}

	//	Make bin file
	char	backPath[1024];

	GetCurrentDirectory(1024,backPath);
	SetCurrentDirectory(strCurPath);

	FILE * fp  = fopen(filename,"wb");
	if(!fp)	{	MessageBox("ERROR","Can not make a file!!",MB_OK);	return;	}
	fwrite(dbname,128,1,fp);
	fwrite(id,64,1,fp);
	fwrite(pw,64,1,fp);
	if(m_ctlDbInfoType.GetCurSel() == 4)
		fwrite(ip,32,1,fp);
	fclose(fp);
	
	SetCurrentDirectory(backPath);
}

void CPAGE_NOTICE::OnIsuseEndtime() 
{
	// TODO: Add your control notification handler code here
	if(m_ctlIsUseEndTime.GetCheck())
	{
		HWND hwnd = ::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE);
		::ShowWindow(hwnd,SW_SHOW);
		hwnd = ::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME);
		::ShowWindow(hwnd,SW_SHOW);
	}else
	{
		HWND hwnd = ::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE);
		::ShowWindow(hwnd,SW_HIDE);
		hwnd = ::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME);
		::ShowWindow(hwnd,SW_HIDE);
	}
}

void CPAGE_NOTICE::OnGetcommandlistBtn() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	// 예약된 명령어들을 가져온다.
	// 백업 명령들을 가져올지를 확인한다.

	if(!g_adCommand.connectCommandDB())
	{
		MessageBox("Can not connect to DATABASE [REDSTONE_INFO]","ERROR");
		return;
	}
	//	기존의 리스트를 지운다. 
	while(m_ctlCommandList.GetCount())
	{	m_ctlCommandList.DeleteString(0);
	}
	g_adCommand.GetCommandList(0);
	//	얻어온 리스트를 넣는다.
	char	strCommand[256];
	for(int i=0;i<g_adCommand.GetListCount();i++)
	{
		int txttype = g_adCommand.cAdCommandList[i].commandtype;
		if(txttype>=eCOMMANDTYPE_NOTICE && txttype<eCOMMONTYPE_CLOSEWORLD)
		{
			txttype -= eCOMMANDTYPE_NOTICE+3;
		}else
		if(txttype>=eCOMMONTYPE_CLOSEWORLD)
		{
			txttype -= eCOMMONTYPE_CLOSEWORLD+6;
		}

		switch(g_adCommand.cAdCommandList[i].commandtype)
		{
		case eCOMMANDTYPE_LOGINSERVER:
			sprintf(strCommand,"[%d] [LOGIN SERVER] [%s] release[%d:%d:%d %d:%d] On/Off %d",
				g_adCommand.cAdCommandList[i].idx,
				g_CT_Text[txttype],
				g_adCommand.cAdCommandList[i].releasetime.year+2000,g_adCommand.cAdCommandList[i].releasetime.month,g_adCommand.cAdCommandList[i].releasetime.day,g_adCommand.cAdCommandList[i].releasetime.hour,g_adCommand.cAdCommandList[i].releasetime.minute,
				g_adCommand.cAdCommandList[i].param1
			);
			break;
		case eCOMMONDTYPE_CHANGE_VERSION:	//	버젼 수정하기
			{
				sprintf(strCommand,"[%d] [LOGIN SERVER] [%s] Changing Version %d",
					g_adCommand.cAdCommandList[i].idx,
					g_CT_Text[txttype],
					g_adCommand.cAdCommandList[i].param1
				);
			}
			break;
		case eCOMMANDTYPE_CHANGE_MAXUSER:	//	최대 수용 인원수를 변경한다.
			{
				sprintf(strCommand,"[%d] [LOGIN SERVER] [%s] Changing Max User Count %d",
					g_adCommand.cAdCommandList[i].idx,
					g_CT_Text[txttype],
					g_adCommand.cAdCommandList[i].param1
				);
			}
			break;

		case eCOMMANDTYPE_NOTICE:
			sprintf(strCommand,"[%d] [%s] [%s] release[%d:%d:%d %d:%d]  finish[%d:%d:%d %d:%d]",
				g_adCommand.cAdCommandList[i].idx,
				((g_adCommand.cAdCommandList[i].commandtarget==0)?("ALL SERVER"):(sList[g_adCommand.cAdCommandList[i].commandtarget-1].strName)),
				g_CT_Text[txttype],
				g_adCommand.cAdCommandList[i].releasetime.year+2000,g_adCommand.cAdCommandList[i].releasetime.month,g_adCommand.cAdCommandList[i].releasetime.day,g_adCommand.cAdCommandList[i].releasetime.hour,g_adCommand.cAdCommandList[i].releasetime.minute,
				g_adCommand.cAdCommandList[i].finishtime.year+2000,g_adCommand.cAdCommandList[i].finishtime.month,g_adCommand.cAdCommandList[i].finishtime.day,g_adCommand.cAdCommandList[i].finishtime.hour,g_adCommand.cAdCommandList[i].finishtime.minute
			);
			break;
		case eCOMMANDTYPE_BANISH:
			sprintf(strCommand,"[%d] [%s] [%s] to [%s] release[%d:%d:%d %d:%d] Msg[%s] ",
				g_adCommand.cAdCommandList[i].idx,
				sList[g_adCommand.cAdCommandList[i].commandtarget].strName,
				g_CT_Text[txttype],
				g_adCommand.cAdCommandList[i].param3,
				g_adCommand.cAdCommandList[i].releasetime.year+2000,g_adCommand.cAdCommandList[i].releasetime.month,g_adCommand.cAdCommandList[i].releasetime.day,g_adCommand.cAdCommandList[i].releasetime.hour,g_adCommand.cAdCommandList[i].releasetime.minute,
				g_adCommand.cAdCommandList[i].msg
			);
			break;
		case eCOMMANDTYPE_BROADCAST_MSG:
			sprintf(strCommand,"[%d] [%s] [%s] release[%d:%d:%d %d:%d] to [%s]",
				g_adCommand.cAdCommandList[i].idx,
				sList[g_adCommand.cAdCommandList[i].commandtarget].strName,
				g_CT_Text[txttype],
				g_adCommand.cAdCommandList[i].releasetime.year+2000,g_adCommand.cAdCommandList[i].releasetime.month,g_adCommand.cAdCommandList[i].releasetime.day,g_adCommand.cAdCommandList[i].releasetime.hour,g_adCommand.cAdCommandList[i].releasetime.minute,
				g_adCommand.cAdCommandList[i].param3
			);
			break;

		case eCOMMONTYPE_CLOSEWORLD:	//	월드 서버를 닫을 시간을 정한다.
			{
				sprintf(strCommand,"[%d] [%s] Maintenance: release[%d:%d:%d %d:%d] finish[%d:%d:%d %d:%d]",
					g_adCommand.cAdCommandList[i].idx,
					((g_adCommand.cAdCommandList[i].commandtarget==0)?("ALL SERVER"):(sList[g_adCommand.cAdCommandList[i].commandtarget-1].strName)),
					g_adCommand.cAdCommandList[i].releasetime.year+2000,g_adCommand.cAdCommandList[i].releasetime.month,g_adCommand.cAdCommandList[i].releasetime.day,g_adCommand.cAdCommandList[i].releasetime.hour,g_adCommand.cAdCommandList[i].releasetime.minute,
					g_adCommand.cAdCommandList[i].finishtime.year+2000,g_adCommand.cAdCommandList[i].finishtime.month,g_adCommand.cAdCommandList[i].finishtime.day,g_adCommand.cAdCommandList[i].finishtime.hour,g_adCommand.cAdCommandList[i].finishtime.minute
				);
			}
			break;
		case eCOMMONTYPE_RENAME_AVATAR:
			{
				sprintf(strCommand,"[%d] [%s] Changing Avatar Name: release[%d:%d:%d %d:%d]",
					g_adCommand.cAdCommandList[i].idx,
					((g_adCommand.cAdCommandList[i].commandtarget==0)?("ALL SERVER"):(sList[g_adCommand.cAdCommandList[i].commandtarget-1].strName)),
					g_adCommand.cAdCommandList[i].releasetime.year+2000,g_adCommand.cAdCommandList[i].releasetime.month,g_adCommand.cAdCommandList[i].releasetime.day,g_adCommand.cAdCommandList[i].releasetime.hour,g_adCommand.cAdCommandList[i].releasetime.minute
				);
			}
			break;
		}
		m_ctlCommandList.AddString(strCommand);
	}
	g_adCommand.disCommandDB();
}

void CPAGE_NOTICE::OnSelchangeWorkForreserve() 
{
	// TODO: Add your control notification handler code here
	int idx = m_ctlReserveCommand.GetCurSel();
	if(idx>=3 && idx<6)
	{
		idx += eCOMMANDTYPE_NOTICE-3;
	}else
	if(idx>=6)
	{
		idx += eCOMMONTYPE_CLOSEWORLD - 6;
	}
	switch(idx)
	{
	case eCOMMANDTYPE_LOGINSERVER:
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_DATE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISUSE_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_TARGET_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISSEND_ALLSERVER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC1),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM1_FORRESERVE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC2),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM2_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_FORRESERVE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_WHO),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_WHO),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ERASEMT),SW_HIDE);
		break;
	case eCOMMONDTYPE_CHANGE_VERSION:
	case eCOMMANDTYPE_CHANGE_MAXUSER:
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_DATE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISUSE_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_TARGET_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISSEND_ALLSERVER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC1),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM1_FORRESERVE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC2),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM2_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_WHO),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_WHO),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ERASEMT),SW_HIDE);
		break;
	case eCOMMANDTYPE_NOTICE:
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_DATE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISUSE_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_TARGET_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISSEND_ALLSERVER),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC1),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM1_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC2),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM2_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_FORRESERVE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_WHO),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_WHO),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ERASEMT),SW_HIDE);
		break;
	case eCOMMANDTYPE_BANISH:
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_DATE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISUSE_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_TARGET_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISSEND_ALLSERVER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC1),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM1_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC2),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM2_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_FORRESERVE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_WHO),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_WHO),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ERASEMT),SW_HIDE);
		break;
	case eCOMMANDTYPE_BROADCAST_MSG:
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_DATE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISUSE_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_TARGET_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISSEND_ALLSERVER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC1),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM1_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC2),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM2_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_FORRESERVE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_WHO),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_WHO),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ERASEMT),SW_HIDE);
		break;
	case  eCOMMONTYPE_CLOSEWORLD:
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_DATE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISUSE_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_TARGET_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISSEND_ALLSERVER),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC1),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM1_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC2),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM2_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_WHO),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_WHO),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_SENDDIRECT),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ERASEMT),SW_SHOW);
		break;
	case eCOMMONTYPE_RENAME_AVATAR:
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_DATE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISUSE_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_TARGET_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISSEND_ALLSERVER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC1),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM1_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_PARAM_STATIC2),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_PARAM2_FORRESERVE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_MSG_FORRESERVE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_LIST_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_COMMAND_WHO),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_WHO),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_SENDDIRECT),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ERASEMT),SW_SHOW);
		break;
	}
}

void CPAGE_NOTICE::OnReserveBtn()
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	// TODO: Add your control notification handler code here
	if(!g_adCommand.connectCommandDB())
	{
		MessageBox("Can not connect to DATABASE [REDSTONE_INFO]","ERROR");
		return;
	}
	int idx = m_ctlReserveCommand.GetCurSel();
	if(idx>=3 && idx<6)
	{
		idx += eCOMMANDTYPE_NOTICE-3;
	}else
	if(idx>=6)
	{
		idx += eCOMMONTYPE_CLOSEWORLD - 6;
	}

	switch(idx)
	{
	case eCOMMANDTYPE_LOGINSERVER:
		{
			CTime	time;
			UTime	regTime,finTime;
			char	tmp[8];
			char	msg[dNOTICE_MAX_LENGTH];
			int		OnOff;

			if(m_ctlISSendDirectly.GetCheck())
			{
				cSRVUTIL::GetUTime((cpUTime *)&regTime,20);	//	20초 이후 정보..
			}else{
				m_ctlReleaseDate.GetTime(time);		regTime.year = ((time.GetYear()>=2000)?(time.GetYear()%100):00);			regTime.month = time.GetMonth();			regTime.day = time.GetDay();
				m_ctlReleaseTime.GetTime(time);		regTime.hour = time.GetHour();			regTime.minute = time.GetMinute();			regTime.second = time.GetSecond();
			}

			m_ctlParam1.GetWindowText(tmp,2);
			cADO.getTimeToUTime(dDEFAULT_TIME,&finTime);
			tmp[1] = 0;
			m_ctlMsg.GetWindowText(msg,dNOTICE_MAX_LENGTH);
			msg[dNOTICE_MAX_LENGTH-1] = 0;
			OnOff = atoi(tmp);

			if(strcmp(msg,"")==0)
			{
				if(MessageBox("Notice text is blank. is it OK?","Check!!",MB_YESNO)==IDNO)
				{
					g_adCommand.disCommandDB();	return;
				}
				sprintf(msg,"NOTHING");
			}

			char	txt[128];
			if(OnOff)
				sprintf(txt,"Login Server Off ");
			else
				sprintf(txt,"Login Server On ");

			if(MessageBox(txt,"Check!!",MB_YESNO)==IDNO)
			{
				MessageBox("Cancel.");	g_adCommand.disCommandDB();	return;
			}

			if(g_adCommand.ReserveCommand(regTime, finTime, eCOMMANDTYPE_LOGINSERVER ,0,OnOff,0,dDEFAULT_TEXT,msg,strUserName))
				MessageBox("Reserved!!");
			else
				MessageBox("Fail!!");
		}
		break;
	case eCOMMONDTYPE_CHANGE_VERSION:
		{
			CTime	time;
			UTime	regTime,finTime;
			char	tmp[8];
			int		Ver;

			if(m_ctlISSendDirectly.GetCheck())
			{
				cSRVUTIL::GetUTime((cpUTime *)&regTime,20);	//	20초 이후 정보..
			}else{
				m_ctlReleaseDate.GetTime(time);		regTime.year = ((time.GetYear()>=2000)?(time.GetYear()%100):00);			regTime.month = time.GetMonth();			regTime.day = time.GetDay();
				m_ctlReleaseTime.GetTime(time);		regTime.hour = time.GetHour();			regTime.minute = time.GetMinute();			regTime.second = time.GetSecond();
			}
			m_ctlParam1.GetWindowText(tmp,7);	tmp[6] = 0;
			Ver = atoi(tmp);
			cADO.getTimeToUTime(dDEFAULT_TIME,&finTime);

			char	txt[128];
			sprintf(txt,"Change Game Version [%d]",Ver);
			if(MessageBox(txt,"Check!!",MB_YESNO)==IDNO)
			{
				MessageBox("Cancel.");	g_adCommand.disCommandDB();	return;
			}

			if(g_adCommand.ReserveCommand(regTime, finTime, eCOMMONDTYPE_CHANGE_VERSION ,0,Ver,0,dDEFAULT_TEXT,dDEFAULT_TEXT,strUserName))
				MessageBox("Reserved!!");
			else
				MessageBox("Fail!!");
		}
		break;
	case eCOMMANDTYPE_CHANGE_MAXUSER:
		{
			CTime	time;
			UTime	regTime,finTime;
			char	tmp[8];
			int		MaxUserCount;

			if(m_ctlISSendDirectly.GetCheck())
			{
				cSRVUTIL::GetUTime((cpUTime *)&regTime,20);	//	20초 이후 정보..
			}else{
				m_ctlReleaseDate.GetTime(time);		regTime.year = ((time.GetYear()>=2000)?(time.GetYear()%100):00);			regTime.month = time.GetMonth();			regTime.day = time.GetDay();
				m_ctlReleaseTime.GetTime(time);		regTime.hour = time.GetHour();			regTime.minute = time.GetMinute();			regTime.second = time.GetSecond();
			}
			m_ctlParam1.GetWindowText(tmp,7);	tmp[6] = 0;
			MaxUserCount = atoi(tmp);
			cADO.getTimeToUTime(dDEFAULT_TIME,&finTime);

			char	txt[128];
			sprintf(txt,"Change max user count in Login Server [%d]",MaxUserCount);
			if(MessageBox(txt,"Check!!",MB_YESNO)==IDNO)
			{
				MessageBox("Cancel.");	g_adCommand.disCommandDB();	return;
			}

			if(g_adCommand.ReserveCommand(regTime, finTime, eCOMMANDTYPE_CHANGE_MAXUSER ,0,MaxUserCount,0,dDEFAULT_TEXT,dDEFAULT_TEXT,strUserName))
				MessageBox("Reserved!!");
			else
				MessageBox("Fail!!");
		}
		break;
	case eCOMMANDTYPE_NOTICE:
		{
			CTime	time;
			UTime	regTime,finTime;
			int		target = 0;
			char	msg[dNOTICE_MAX_LENGTH];

			if(m_ctlISSendDirectly.GetCheck())
			{
				cSRVUTIL::GetUTime((cpUTime *)&regTime,20);	//	20초 이후 정보..
			}else{
				m_ctlReleaseDate.GetTime(time);		regTime.year = time.GetYear()%100;			regTime.month = time.GetMonth();			regTime.day = time.GetDay();
				m_ctlReleaseTime.GetTime(time);		regTime.hour = time.GetHour();				regTime.minute = time.GetMinute();			regTime.second = time.GetSecond();
			}
			finTime.m_dwValue = regTime.m_dwValue;
			
			if(!m_ctlIsSendtoAllServer.GetCheck())
			{
				if(g_iSelectedServer == -1)
				{	MessageBox("Not Select World Server !" ," ERROR ");	g_adCommand.disCommandDB();	return;
				}
				target = g_iSelectedServer+1;
			}
			m_ctlMsg.GetWindowText(msg,dNOTICE_MAX_LENGTH);	msg[dNOTICE_MAX_LENGTH-1]=0;
			if(strcmp(msg,"")==0)
			{	MessageBox("Message is blank.","ERROR");	g_adCommand.disCommandDB();	return;
			}
			if(strlen(msg) > 127)
			{	MessageBox("Message length is too long. You can write under 128 characters.","ERROR");	g_adCommand.disCommandDB();	return;
			}

			char tmp[128];
			if(m_ctlIsSendtoAllServer.GetCheck())
				sprintf(tmp,"Are you really send the notice to All Server?");
			else
				sprintf(tmp,"Are you really send the notice to [%s] Server?",sList[g_iSelectedServer].strName);

			if(MessageBox(tmp,"Check!!",MB_YESNO)==IDNO)
			{
				MessageBox("Cancel.");	g_adCommand.disCommandDB();	return;
			}

			if(g_adCommand.ReserveCommand(regTime, finTime, eCOMMANDTYPE_NOTICE ,target,0,0,dDEFAULT_TEXT,msg,strUserName))
				MessageBox("Reserved!!");
			else
				MessageBox("Fail!!");
		}
		break;
	case eCOMMANDTYPE_BANISH:
		{
			UTime	regTime,finTime;
			CTime	time;
			int		target = 0;
			char	msg[dNOTICE_MAX_LENGTH];
			char	who[32];

			if(g_iSelectedServer == -1)
			{	MessageBox("Not Select World Server !" ," ERROR ");	g_adCommand.disCommandDB();	return;
			}
			target = g_iSelectedServer+1;

			m_ctlWho.GetWindowText(who,22);
			if(strcmp(who,"")==0)
			{	MessageBox("Target is blank.","ERROR");	g_adCommand.disCommandDB();	return;
			}

			m_ctlMsg.GetWindowText(msg,dNOTICE_MAX_LENGTH);	msg[dNOTICE_MAX_LENGTH-1]=0;
			if(strcmp(msg,"")==0)
			{	MessageBox("Message is blank.","ERROR");	g_adCommand.disCommandDB();	return;
			}

			if(m_ctlISSendDirectly.GetCheck())
			{
				cSRVUTIL::GetUTime((cpUTime *)&regTime,20);	//	20초 이후 정보..
			}else{
				m_ctlReleaseDate.GetTime(time);		regTime.year = time.GetYear()%100;			regTime.month = time.GetMonth();			regTime.day = time.GetDay();
				m_ctlReleaseTime.GetTime(time);		regTime.hour = time.GetHour();				regTime.minute = time.GetMinute();			regTime.second = time.GetSecond();
			}
			cADO.getTimeToUTime(dDEFAULT_TIME,&finTime);
			
			if(g_adCommand.ReserveCommand(regTime, finTime, eCOMMANDTYPE_BANISH ,target,0,0,who,msg,strUserName))
				MessageBox("Reserved!!");
			else
				MessageBox("Fail!!");
		}
		break;
	case eCOMMANDTYPE_BROADCAST_MSG:	//	유저들이 올리는 방송용 메세지
		{
			char 	msg[dNOTICE_MAX_LENGTH],who[22];
			CTime	time;
			UTime	regTime,finTime;
			if(m_ctlISSendDirectly.GetCheck())
			{
				cSRVUTIL::GetUTime((cpUTime *)&regTime,20);	//	20초 이후 정보..
			}else{
				m_ctlReleaseDate.GetTime(time);
				regTime.year = ((time.GetYear()>=2000)?(time.GetYear()%100):00);			regTime.month = time.GetMonth();			regTime.day = time.GetDay();
				m_ctlReleaseTime.GetTime(time);
				regTime.hour = time.GetHour();			regTime.minute = time.GetMinute();	regTime.second = time.GetSecond();
			}
			cADO.getTimeToUTime(dDEFAULT_TIME,&finTime);

			if(g_iSelectedServer == -1)
			{	MessageBox("Not Select World Server!" ,"ERROR");	g_adCommand.disCommandDB();	return;
			}
			int target = g_iSelectedServer+1;

			m_ctlWho.GetWindowText(who,20);
			if(strcmp(who,"")==0)
			{	MessageBox("Param3 is blank.","ERROR");	g_adCommand.disCommandDB();	return;
			}

			m_ctlMsg.GetWindowText(msg,dNOTICE_MAX_LENGTH);	msg[dNOTICE_MAX_LENGTH-1]=0;
			if(strcmp(msg,"")==0)
			{	MessageBox("Message is blank.","ERROR");	g_adCommand.disCommandDB();	return;
			}

			char tmp[128];
			sprintf(tmp,"Are you really send the message to [%s] in [%s] Server?",who,sList[g_iSelectedServer].strName);
			if(MessageBox(tmp,"Check!!",MB_YESNO)==IDNO)
			{	MessageBox("Cancel.");	g_adCommand.disCommandDB();	return;
			}

			if(g_adCommand.ReserveCommand(regTime, finTime, eCOMMANDTYPE_BROADCAST_MSG ,target,0,0,who,msg,strUserName))
				MessageBox("Reserved!!");
			else
				MessageBox("Fail!!");
		}
		break;
	case eCOMMONTYPE_CLOSEWORLD:
		{
			CTime	time;
			UTime	regTime,finTime;
			char msg[dNOTICE_MAX_LENGTH];
			if(m_ctlEraseMt.GetCheck())
			{
				regTime.year = 99; regTime.month = 01; regTime.day = 01;
				regTime.hour = 01; regTime.minute = 01; regTime.second = 01;
				finTime.m_dwValue = regTime.m_dwValue;
			}else{
				m_ctlReleaseDate.GetTime(time);	regTime.year = time.GetYear()%100;	regTime.month = time.GetMonth();			regTime.day = time.GetDay();
				m_ctlReleaseTime.GetTime(time);	regTime.hour = time.GetHour();		regTime.minute = time.GetMinute();			regTime.second = time.GetSecond();
				m_ctlFinishDate.GetTime(time);	finTime.year = time.GetYear()%100;	finTime.month = time.GetMonth();			finTime.day = time.GetDay();
				m_ctlFinishTime.GetTime(time);	finTime.hour = time.GetHour();		finTime.minute = time.GetMinute();			finTime.second = time.GetSecond();
			}

			int target = 0;
			if(!m_ctlIsSendtoAllServer.GetCheck())
			{
				if(g_iSelectedServer == -1)
				{	MessageBox("Not Select World Server!" ,"ERROR");	g_adCommand.disCommandDB();	return;
				}
				target = g_iSelectedServer+1;
			}

			char tmp[128];
			if(m_ctlEraseMt.GetCheck())
			{
				if(!target)
					sprintf(tmp,"Are you really erase the maintenance time of all servers?");
				else
					sprintf(tmp,"Are you really erase the maintenance time of [%s] Server?",sList[g_iSelectedServer].strName);
			}else
			{
				if(!target)
					sprintf(tmp,"Are you really change the maintenance time of all servers?");
				else
					sprintf(tmp,"Are you really change the maintenance time of [%s] Server?",sList[g_iSelectedServer].strName);
			}
			if(MessageBox(tmp,"Check!!",MB_YESNO)==IDNO)
			{
				MessageBox("Cancel.");	g_adCommand.disCommandDB();	return;
			}

			msg[0] = 0;

			if(g_adCommand.ReserveCommand(regTime, finTime, eCOMMONTYPE_CLOSEWORLD ,target,0,0,dDEFAULT_TEXT,msg,strUserName))
				MessageBox("Reserved!!");
			else
				MessageBox("Fail!!");
		}
		break;
	case eCOMMONTYPE_RENAME_AVATAR:
		{
			MessageBox(" It's impossable. This job is just regist automatically");
			m_ctlCommandList.SetCurSel(0);
		}
		break;
	}
	g_adCommand.disCommandDB();
}

void CPAGE_NOTICE::OnSelchangeRegcommandList() 
{
	// TODO: Add your control notification handler code here
	int idx = m_ctlCommandList.GetCurSel();

	int cmdtype = g_adCommand.cAdCommandList[idx].commandtype;

	if(cmdtype>=eCOMMANDTYPE_NOTICE && cmdtype<eCOMMONTYPE_CLOSEWORLD)
	{
		cmdtype -= (eCOMMANDTYPE_NOTICE - 3);
	}else
	if(cmdtype>=eCOMMONTYPE_CLOSEWORLD)
	{
		cmdtype -= (eCOMMONTYPE_CLOSEWORLD - 6);
	}
	m_ctlReserveCommand.SetCurSel(cmdtype);
	OnSelchangeWorkForreserve();

	switch(g_adCommand.cAdCommandList[idx].commandtype)
	{
	case eCOMMANDTYPE_LOGINSERVER:	//	로그인 서버 닫기/열기
		{
			CTime	rt(g_adCommand.cAdCommandList[idx].releasetime.year+2000,
					   g_adCommand.cAdCommandList[idx].releasetime.month,
					   g_adCommand.cAdCommandList[idx].releasetime.day, 
					   g_adCommand.cAdCommandList[idx].releasetime.hour,
					   g_adCommand.cAdCommandList[idx].releasetime.minute,
					   g_adCommand.cAdCommandList[idx].releasetime.second);
			m_ctlReleaseDate.SetTime(&rt);
			m_ctlReleaseTime.SetTime(&rt);
			char txt[10];
			sprintf(txt,"%d",g_adCommand.cAdCommandList[idx].param1);
			m_ctlParam1.SetWindowText(txt);
		}
		break;
	case eCOMMONDTYPE_CHANGE_VERSION:	//	버젼 수정하기
		{
			CTime	rt(g_adCommand.cAdCommandList[idx].releasetime.year+2000,
					   g_adCommand.cAdCommandList[idx].releasetime.month,
					   g_adCommand.cAdCommandList[idx].releasetime.day, 
					   g_adCommand.cAdCommandList[idx].releasetime.hour,
					   g_adCommand.cAdCommandList[idx].releasetime.minute,
					   g_adCommand.cAdCommandList[idx].releasetime.second);
			m_ctlReleaseDate.SetTime(&rt);
			m_ctlReleaseTime.SetTime(&rt);
			char txt[10];
			sprintf(txt,"%d",g_adCommand.cAdCommandList[idx].param1);
			m_ctlParam1.SetWindowText(txt);
		}
		break;
	case eCOMMANDTYPE_CHANGE_MAXUSER:	//	최대 수용 인원수를 변경한다.
		{
			CTime	rt(g_adCommand.cAdCommandList[idx].releasetime.year+2000,
					   g_adCommand.cAdCommandList[idx].releasetime.month,
					   g_adCommand.cAdCommandList[idx].releasetime.day, 
					   g_adCommand.cAdCommandList[idx].releasetime.hour,
					   g_adCommand.cAdCommandList[idx].releasetime.minute,
					   g_adCommand.cAdCommandList[idx].releasetime.second);
			m_ctlReleaseDate.SetTime(&rt);
			m_ctlReleaseTime.SetTime(&rt);
			char txt[10];
			sprintf(txt,"%d",g_adCommand.cAdCommandList[idx].param1);
			m_ctlParam1.SetWindowText(txt);
		}
		break;

	case eCOMMANDTYPE_NOTICE:		//	공지하기
		{
			CTime	rt(g_adCommand.cAdCommandList[idx].releasetime.year+2000,
					   g_adCommand.cAdCommandList[idx].releasetime.month,
					   g_adCommand.cAdCommandList[idx].releasetime.day, 
					   g_adCommand.cAdCommandList[idx].releasetime.hour,
					   g_adCommand.cAdCommandList[idx].releasetime.minute,
					   g_adCommand.cAdCommandList[idx].releasetime.second);
			m_ctlReleaseDate.SetTime(&rt);
			m_ctlReleaseTime.SetTime(&rt);

			m_ctlMsg.SetWindowText(g_adCommand.cAdCommandList[idx].msg);
			if(g_adCommand.cAdCommandList[idx].commandtarget==0)
			{
				m_ctlIsSendtoAllServer.SetCheck(1);
				g_iSelectedServer = -1;
			}else
				g_iSelectedServer = g_adCommand.cAdCommandList[idx].commandtarget-1;
		}
		break;
	case eCOMMANDTYPE_BANISH:		//	추방하기 , 1회성이다. 다시 못들어오게 하기 위해서는 다른 조취가 필요하다.
		{
			CTime	rt(g_adCommand.cAdCommandList[idx].releasetime.year+2000,
					   g_adCommand.cAdCommandList[idx].releasetime.month,
					   g_adCommand.cAdCommandList[idx].releasetime.day, 
					   g_adCommand.cAdCommandList[idx].releasetime.hour,
					   g_adCommand.cAdCommandList[idx].releasetime.minute,
					   g_adCommand.cAdCommandList[idx].releasetime.second);
			m_ctlReleaseDate.SetTime(&rt);

			if(g_adCommand.cAdCommandList[idx].commandtarget==0)
			{	m_ctlIsSendtoAllServer.SetCheck(1);
				g_iSelectedServer = -1;
			}else
				g_iSelectedServer = g_adCommand.cAdCommandList[idx].commandtarget-1;
			m_ctlMsg.SetWindowText(g_adCommand.cAdCommandList[idx].msg);
			m_ctlWho.SetWindowText(g_adCommand.cAdCommandList[idx].param3);
		}
		break;
	case eCOMMANDTYPE_BROADCAST_MSG:	//	유저들이 올리는 방송용 메세지
		{
			CTime	rt(g_adCommand.cAdCommandList[idx].releasetime.year+2000,
					   g_adCommand.cAdCommandList[idx].releasetime.month,
					   g_adCommand.cAdCommandList[idx].releasetime.day, 
					   g_adCommand.cAdCommandList[idx].releasetime.hour,
					   g_adCommand.cAdCommandList[idx].releasetime.minute,
					   g_adCommand.cAdCommandList[idx].releasetime.second);
			m_ctlReleaseDate.SetTime(&rt);
			m_ctlReleaseTime.SetTime(&rt);

			m_ctlWho.SetWindowText(g_adCommand.cAdCommandList[idx].param3);
			m_ctlMsg.SetWindowText(g_adCommand.cAdCommandList[idx].msg);
			g_iSelectedServer = g_adCommand.cAdCommandList[idx].commandtarget-1;
		}
		break;

	case eCOMMONTYPE_CLOSEWORLD:	//	월드 서버를 닫을 시간을 정한다.
		{
			CTime	rt(g_adCommand.cAdCommandList[idx].releasetime.year+2000,
					   g_adCommand.cAdCommandList[idx].releasetime.month,
					   g_adCommand.cAdCommandList[idx].releasetime.day, 
					   g_adCommand.cAdCommandList[idx].releasetime.hour,
					   g_adCommand.cAdCommandList[idx].releasetime.minute,
					   g_adCommand.cAdCommandList[idx].releasetime.second);
			CTime	ft(g_adCommand.cAdCommandList[idx].finishtime.year+2000,
					   g_adCommand.cAdCommandList[idx].finishtime.month,
					   g_adCommand.cAdCommandList[idx].finishtime.day, 
					   g_adCommand.cAdCommandList[idx].finishtime.hour,
					   g_adCommand.cAdCommandList[idx].finishtime.minute,
					   g_adCommand.cAdCommandList[idx].finishtime.second);
			m_ctlReleaseDate.SetTime(&rt);
			m_ctlReleaseTime.SetTime(&rt);
			m_ctlFinishDate.SetTime(&ft);
			m_ctlFinishTime.SetTime(&ft);

			if(g_adCommand.cAdCommandList[idx].commandtarget==0)
			{	m_ctlIsSendtoAllServer.SetCheck(1);
				g_iSelectedServer = -1;
			}else
				g_iSelectedServer = g_adCommand.cAdCommandList[idx].commandtarget-1;
		}
	case eCOMMONTYPE_RENAME_AVATAR:
		{
			MessageBox(" It's impossable. This job is just regist automatically");
			m_ctlCommandList.SetCurSel(0);
		}
		break;
	}
}

void CPAGE_NOTICE::OnModifyBtn() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	// TODO: Add your control notification handler code here
	int sidx = m_ctlReserveCommand.GetCurSel();
	if(sidx == -1)	return;
	if(sidx>=3 && sidx<6)
	{
		sidx += eCOMMANDTYPE_NOTICE-3;
	}else
	if(sidx>=6)
	{
		sidx += eCOMMONTYPE_CLOSEWORLD - 6;
	}

	if(g_adCommand.cAdCommandList[sidx].commandtype != sidx)
	{
		MessageBox("Changed command type. Not allow!!");
		return;
	}

	if(!g_adCommand.connectCommandDB())
	{
		MessageBox("Can not connect to DATABASE [REDSTONE_INFO]","ERROR");
		return;
	}
	switch(g_adCommand.cAdCommandList[sidx].commandtype)
	{
	case eCOMMANDTYPE_NOTICE:
		{
			CTime	time;
			UTime	regTime,finTime;
			int		target = 0;
			char	msg[dNOTICE_MAX_LENGTH];

			m_ctlReleaseDate.GetTime(time);
			regTime.year = time.GetYear()%100;			regTime.month = time.GetMonth();			regTime.day = time.GetDay();
			m_ctlReleaseTime.GetTime(time);
			regTime.hour = time.GetHour();				regTime.minute = time.GetMinute();			regTime.second = time.GetSecond();

			finTime.m_dwValue = regTime.m_dwValue;
			
			if(!m_ctlIsSendtoAllServer.GetCheck())
			{
				if(g_iSelectedServer == -1)
				{	MessageBox("Not Select World Server !" ," ERROR ");	g_adCommand.disCommandDB();	return;
				}
				target = g_iSelectedServer+1;
			}
			m_ctlMsg.GetWindowText(msg,dNOTICE_MAX_LENGTH);	msg[dNOTICE_MAX_LENGTH-1]=0;
			if(strcmp(msg,"")==0)
			{	MessageBox("Message is blank.","ERROR");	g_adCommand.disCommandDB();	return;
			}

			char tmp[128];
			if(target)
				sprintf(tmp,"Are you really send the notice to [%s] Server?",sList[g_iSelectedServer].strName);
			else
				sprintf(tmp,"Are you really send the notice to All Server?");

			if(MessageBox(tmp,"Check!!",MB_YESNO)==IDNO)
			{
				MessageBox("Cancel.");	g_adCommand.disCommandDB();	return;
			}

			if(g_adCommand.ModifyCommand(g_adCommand.cAdCommandList[sidx].idx,regTime, finTime, eCOMMANDTYPE_NOTICE ,target,0,0,dDEFAULT_TEXT,msg,strUserName))
				MessageBox("Modified!!");
			else
				MessageBox("Fail!!");
		}
		break;
	case eCOMMANDTYPE_BANISH:
		{
			UTime	regTime,finTime;
			int		target = 0;
			char	msg[dCHAT_LENGTH];
			char	who[32];

			if(g_iSelectedServer == -1)
			{	MessageBox("Not Select World Server !" ," ERROR ");	g_adCommand.disCommandDB();	return;
			}
			target = g_iSelectedServer+1;

			m_ctlWho.GetWindowText(who,22);
			if(strcmp(who,"")==0)
			{	MessageBox("Target is blank.","ERROR");	g_adCommand.disCommandDB();	return;
			}

			m_ctlMsg.GetWindowText(msg,dCHAT_LENGTH);	msg[dCHAT_LENGTH-1]=0;
			if(strcmp(msg,"")==0)
			{	MessageBox("Message is blank.","ERROR");	g_adCommand.disCommandDB();	return;
			}
			regTime.m_dwValue = 1;	//	오늘 날짜로 처리해 주셈
			cADO.getTimeToUTime(dDEFAULT_TIME,&finTime);
			
			if(g_adCommand.ModifyCommand(g_adCommand.cAdCommandList[sidx].idx,regTime, finTime, eCOMMANDTYPE_BANISH ,target,0,0,who,msg,strUserName))
				MessageBox("Modified!!");
			else
				MessageBox("Fail!!");
		}
		break;
	case eCOMMANDTYPE_LOGINSERVER:
		{
			CTime	time;
			UTime	regTime,finTime;
			char	tmp[8];
			int		OnOff;

			m_ctlReleaseDate.GetTime(time);
			regTime.year = ((time.GetYear()>=2000)?(time.GetYear()%100):00);
			regTime.month = time.GetMonth();
			regTime.day = time.GetDay();

			m_ctlReleaseTime.GetTime(time);
			regTime.hour = time.GetHour();
			regTime.minute = time.GetMinute();
			regTime.second = time.GetSecond();

			m_ctlParam1.GetWindowText(tmp,2);

			cADO.getTimeToUTime(dDEFAULT_TIME,&finTime);
			tmp[1] = 0;
			OnOff = atoi(tmp);
			if(g_adCommand.ModifyCommand(g_adCommand.cAdCommandList[sidx].idx,regTime, finTime, eCOMMANDTYPE_LOGINSERVER ,0,OnOff,0,dDEFAULT_TEXT,dDEFAULT_TEXT,strUserName))
				MessageBox("Modified!!");
			else
				MessageBox("Fail!!");
		}
		break;
	case eCOMMONDTYPE_CHANGE_VERSION://	버젼 수정하기
		{
		}
		break;
	case eCOMMANDTYPE_CHANGE_MAXUSER://	최대 수용 인원수를 변경한다.
		{
		}
		break;
	case eCOMMANDTYPE_BROADCAST_MSG:	//	유저들이 올리는 방송용 메세지
		{
			char 	msg[dCHAT_LENGTH],who[22];
			CTime	time;
			UTime	regTime,finTime;
			m_ctlReleaseDate.GetTime(time);
			regTime.year = ((time.GetYear()>=2000)?(time.GetYear()%100):00);			regTime.month = time.GetMonth();			regTime.day = time.GetDay();
			m_ctlReleaseTime.GetTime(time);
			regTime.hour = time.GetHour();			regTime.minute = time.GetMinute();	regTime.second = time.GetSecond();
			cADO.getTimeToUTime(dDEFAULT_TIME,&finTime);

			if(g_iSelectedServer == -1)
			{	MessageBox("Not Select World Server!" ,"ERROR");	g_adCommand.disCommandDB();	return;
			}
			int target = g_iSelectedServer+1;

			m_ctlWho.GetWindowText(who,20);
			if(strcmp(who,"")==0)
			{	MessageBox("Param3 is blank.","ERROR");	g_adCommand.disCommandDB();	return;
			}

			m_ctlMsg.GetWindowText(msg,dCHAT_LENGTH);	msg[dCHAT_LENGTH-1]=0;
			if(strcmp(msg,"")==0)
			{	MessageBox("Message is blank.","ERROR");	g_adCommand.disCommandDB();	return;
			}

			char tmp[128];
			sprintf(tmp,"Are you really send the message to [%s] in [%s] Server?",who,sList[g_iSelectedServer].strName);
			if(MessageBox(tmp,"Check!!",MB_YESNO)==IDNO)
			{	MessageBox("Cancel.");	g_adCommand.disCommandDB();	return;
			}

			if(g_adCommand.ModifyCommand(g_adCommand.cAdCommandList[sidx].idx,regTime, finTime, eCOMMANDTYPE_BROADCAST_MSG ,target,0,0,who,msg,strUserName))
				MessageBox("Modified!!");
			else
				MessageBox("Fail!!");
		}
		break;
	case eCOMMONTYPE_CLOSEWORLD:
		{
		}
		break;
	case eCOMMONTYPE_RENAME_AVATAR:
		{
			MessageBox(" It's impossable. This job is just regist automatically");
			m_ctlCommandList.SetCurSel(0);
		}
		break;
	}
	g_adCommand.disCommandDB();
}

void CPAGE_NOTICE::OnCancelBtn() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	// TODO: Add your control notification handler code here
	int sidx = m_ctlCommandList.GetCurSel();
	if(sidx == -1)	return;
	if(sidx>=3 && sidx<6)
	{
		sidx += eCOMMANDTYPE_NOTICE-3;
	}else
	if(sidx>=6)
	{
		sidx += eCOMMONTYPE_CLOSEWORLD - 6;
	}

	if(MessageBox("Do you delete this command?","Check!!",MB_YESNO)==IDNO)
	{	MessageBox("Cancel.");	g_adCommand.disCommandDB();	return;
	}

	if(g_adCommand.DeleteCommand(g_adCommand.cAdCommandList[sidx].idx,strUserName))
		MessageBox("Deleted!!");
	else
		MessageBox("Fail!!");
}

void CPAGE_NOTICE::OnSenddirect() 
{
	// TODO: Add your control notification handler code here
	int idx = m_ctlReserveCommand.GetCurSel();
	if(idx>=3 && idx<6)
	{
		idx += eCOMMANDTYPE_NOTICE-3;
	}else
	if(idx>=6)
	{
		idx += eCOMMONTYPE_CLOSEWORLD - 6;
	}

	if(m_ctlISSendDirectly.GetCheck())	//	시간 설정 기능을 비활성화 한다.
	{
		if(idx !=eCOMMONTYPE_CLOSEWORLD)
		{
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME_STATIC),SW_HIDE);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_DATE),SW_HIDE);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME),SW_HIDE);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME),SW_HIDE);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISUSE_ENDTIME),SW_HIDE);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE),SW_HIDE);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME),SW_HIDE);
		}
	}else	//	시간 설정 기능을 활성화 한다.
	{
		switch(idx)
		{
		case eCOMMANDTYPE_LOGINSERVER:
		case eCOMMONDTYPE_CHANGE_VERSION:	//	버젼 수정하기
		case eCOMMANDTYPE_CHANGE_MAXUSER:	//	최대 수용 인원수를 변경한다.
		case eCOMMANDTYPE_NOTICE:
		case eCOMMANDTYPE_BANISH:
		case eCOMMANDTYPE_BROADCAST_MSG:
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME_STATIC),SW_SHOW);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_DATE),SW_SHOW);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME),SW_SHOW);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME),SW_HIDE);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISUSE_ENDTIME),SW_HIDE);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE),SW_HIDE);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME),SW_HIDE);
			break;
		}
	}	
}

void CPAGE_NOTICE::OnErasemt() 
{
	// TODO: Add your control notification handler code here
	if(m_ctlEraseMt.GetCheck())
	{
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME_STATIC),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_DATE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISUSE_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME),SW_HIDE);
	}else
	{
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME_STATIC),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_DATE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_RELEASE_TIME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ISUSE_ENDTIME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_DATE),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_ENDTIME_TIME),SW_SHOW);
	}
}

void CPAGE_NOTICE::OnCheckSmsAlarm() 
{
	// TODO: Add your control notification handler code here
	
}
