// cDlgChangeGMaster.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "cDlgChangeGMaster.h"
#include "cADO_ADMIN.h"
#include "cMessage.h"

cADO_ADMIN		commandDB;
int				Step = 0;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cDlgChangeGMaster dialog


cDlgChangeGMaster::cDlgChangeGMaster(CWnd* pParent /*=NULL*/)
	: CDialog(cDlgChangeGMaster::IDD, pParent)
{
	//{{AFX_DATA_INIT(cDlgChangeGMaster)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cDlgChangeGMaster::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cDlgChangeGMaster)
	DDX_Control(pDX, IDC_cbServerList, m_ctlServerList);
	DDX_Control(pDX, IDC_CGM_EXPLAIN, m_ctlExplain);
	DDX_Control(pDX, IDC_NEWGUILDMASTER, m_ctlNewGuildMaster);
	DDX_Control(pDX, IDC_GUILDNAME, m_ctlGuildName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cDlgChangeGMaster, CDialog)
	//{{AFX_MSG_MAP(cDlgChangeGMaster)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_CBN_SELCHANGE(IDC_cbServerList, OnSelchangecbServerList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cDlgChangeGMaster message handlers

void cDlgChangeGMaster::OnCancel() 
{
	// TODO: Add your control notification handler code here
	commandDB.Close();
	OnOK();
}

BOOL cDlgChangeGMaster::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Step = 0;
	m_ctlExplain.SetWindowText(g_strToolMessage[154]);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_GUILDNAME),SW_HIDE);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_NEWGUILDMASTER),SW_HIDE);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_GUILDNAME),SW_HIDE);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEWGUILDMASTER),SW_HIDE);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_SERVERLIST),SW_HIDE);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_cbServerList),SW_HIDE);

	for(int i=0;i<dMAX_WORLD_SERVER_COUNT;i++)
	{
		if(strcmp(sList[i].strName,"")==0)	break;
		m_ctlServerList.InsertString(i,sList[i].strName);
		m_ctlServerList.SetCurSel(0);
	}
	selectedSrvIndex = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void cDlgChangeGMaster::FailProcess()
{
	m_ctlExplain.SetWindowText(g_strToolMessage[156]);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_GUILDNAME),SW_HIDE);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_NEWGUILDMASTER),SW_HIDE);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_GUILDNAME),SW_HIDE);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEWGUILDMASTER),SW_HIDE);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEXT),SW_HIDE);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_SERVERLIST),SW_HIDE);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_cbServerList),SW_HIDE);
}

void cDlgChangeGMaster::GetGuildInfo()
{
	char gname[32];
	char gmname[32];
	char backgmname[32];

	m_ctlGuildName.GetWindowText(gname,32);
	m_ctlNewGuildMaster.GetWindowText(gmname,20);
	m_ctlNewGuildMaster.GetWindowText(backgmname,20);

	_log("Get Guild Info [%s] [%s]",gname,gmname);
	int ret = commandDB.spGetGuildInfo(gname,gmname);
	//	결과 Step는 넘어가려는 값보다 1작게. ~ OnNext에 ++가 있다.
	if(ret  == 1)
	{
		Step = 4;		//	성공
		//	기존 마스터와 새로운 마스터의 데이타 정보를 수정하자.
		if(stricmp(backgmname,gmname)!=0)
		{
			_log("last master = [%s] , current master = [%s]",backgmname,gmname);
			//	돌아온 gmname은 길드 랭킹을 2로 만들어야 한다.
			cPLAYER_DATA	pData;
			_log("Load data of [%s]",gmname);
			commandDB.spReadAva(gmname,&pData);
			pData.m_wGuildRank = 2;
			_log("Modify data of [%s]",gmname);
			commandDB.spSaveAva(&pData);
		}
		cPLAYER_DATA	pData;
		_log("Load data of [%s]",backgmname);
		commandDB.spReadAva(backgmname,&pData);
		pData.m_wGuildRank = 6;
		_log("Modify data of [%s]",backgmname);
		commandDB.spSaveAva(&pData);
	}
	else if (ret == -1)	Step = 5;	//	입력한 길드는 이 서버에 없습니다.
	else if (ret == -2) Step = 6;	//	길마의 정보를 찾을 수 없습니다.
	else Step = 7;					//	SP 관련에러로 Try Catch에 걸린거야.
	OnNext();
}

void cDlgChangeGMaster::OnNext() 
{
	// TODO: Add your control notification handler code here
	Step++;

	m_ctlExplain.SetWindowText("Wait a minute!!");
	switch(Step)
	{
	case 1:
		m_ctlExplain.SetWindowText(g_strToolMessage[158]);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_SERVERLIST),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_cbServerList),SW_SHOW);
		break;
	case 2:
		{
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_SERVERLIST),SW_HIDE);
			::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_cbServerList),SW_HIDE);

			if(!commandDB.Connect(sList[selectedSrvIndex].strDBIP,strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
			{	
				MessageBox(g_strToolMessage[38]);	
				FailProcess();
				return;	
			}
			m_ctlExplain.SetWindowText("Success to connect DB! Press Button \"NEXT\"");
		}
		break;
	case 3:
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_GUILDNAME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_NEWGUILDMASTER),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_GUILDNAME),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEWGUILDMASTER),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEXT),SW_SHOW);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_SERVERLIST),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_cbServerList),SW_HIDE);
		m_ctlExplain.SetWindowText(g_strToolMessage[155]);
		break;
	case 4:
		m_ctlExplain.SetWindowText(g_strToolMessage[157]);
		GetGuildInfo();
		commandDB.Close();
		break;
	case 5:
		m_ctlExplain.SetWindowText("Success !!");
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_GUILDNAME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_NEWGUILDMASTER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_GUILDNAME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEWGUILDMASTER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEXT),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_SERVERLIST),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_cbServerList),SW_HIDE);
		break;
	case 6:
		m_ctlExplain.SetWindowText("Cann't find the guild");
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_GUILDNAME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_NEWGUILDMASTER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_GUILDNAME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEWGUILDMASTER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEXT),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_SERVERLIST),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_cbServerList),SW_HIDE);
		break;
	case 7:
		m_ctlExplain.SetWindowText("Cann't find the new guild master");
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_GUILDNAME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_NEWGUILDMASTER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_GUILDNAME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEWGUILDMASTER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEXT),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_SERVERLIST),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_cbServerList),SW_HIDE);
		break;
	case 8:
		m_ctlExplain.SetWindowText("ERROR !! Contect DB Administrator");
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_GUILDNAME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_STATIC_NEWGUILDMASTER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_GUILDNAME),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEWGUILDMASTER),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_NEXT),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_SERVERLIST),SW_HIDE);
		::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_cbServerList),SW_HIDE);
		break;
	}
}

void cDlgChangeGMaster::OnSelchangecbServerList() 
{
	// TODO: Add your control notification handler code here
	selectedSrvIndex = m_ctlServerList.GetCurSel();
}
