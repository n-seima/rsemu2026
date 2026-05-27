// PAGE_PVPTOOL.cpp: implementation of the CPAGE_PVPTOOL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PAGE_PVPTOOL.h"
#include "cDECODER.h"
#include "admintool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPAGE_PVPTOOL, CPropertyPage)

CPAGE_PVPTOOL::CPAGE_PVPTOOL() : CPropertyPage(CPAGE_PVPTOOL::IDD)
{
	//{{AFX_DATA_INIT(CPAGE_PVPTOOL)
	//}}AFX_DATA_INIT
}

CPAGE_PVPTOOL::~CPAGE_PVPTOOL()
{

}

void CPAGE_PVPTOOL::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPAGE_PVPTOOL)
//	DDX_Control(pDX, IDC_CLEAR, m_ctlClearWCGList);
//	DDX_Control(pDX, IDC_UPDATE, m_ctlUpdateWCGList);
//	DDX_Control(pDX, IDC_GETLIST, m_ctlGetWCGList);

//	DDX_Control(pDX, IDC_CLEAR_WCG_RANK, m_ctlClearWCGRank);
//	DDX_Control(pDX, IDC_GET_WCG_RANK, m_ctlGetWCGRank);
//	DDX_Control(pDX, IDC_MODIFY_WCG_SCORE, m_ctlModifyWCGScore);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPAGE_PVPTOOL, CPropertyPage)
	//{{AFX_MSG_MAP(CPAGE_PVPTOOL)
	ON_BN_CLICKED(IDC_CLEAR, OnClearWCGList)
	ON_BN_CLICKED(IDC_ADD_TEAM_LIST, OnAddTeamList)
	ON_BN_CLICKED(IDC_GETLIST, OnGetWCGList)

	ON_BN_CLICKED(IDC_CLEAR_WCG_RANK, OnClearWCGRank)
	ON_BN_CLICKED(IDC_GET_WCG_RANK, OnGetWCGRank)
	ON_BN_CLICKED(IDC_MODIFY_WCG_SCORE, OnModifyWCGScore)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPAGE_NOTICE message handlers

BOOL CPAGE_PVPTOOL::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_iMemberCount	=	2;

	SetDlgItemInt(IDC_MEMBER_COUNT,m_iMemberCount);

	clearListControl(eCTL_INIT_WCGTEAMLIST);
//	OnGetWCGList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPAGE_PVPTOOL::OnClearWCGList(BOOL _bIsByAdd)
{
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

	if	(MessageBox("Do you want to clear all of the WCG Team Infomations?","Caution!!",MB_YESNO)==IDNO)
		return;

	if	(_bIsByAdd	==	FALSE)
		if	(MessageBox("Really??","Caution!!",MB_YESNO)==IDNO)
			return;

	g_DBAccount.spClearWCGTeamList();
	clearListControl(eCTL_INIT_WCGTEAMLIST);
}

void CPAGE_PVPTOOL::OnAddTeamList()
{
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

	char	dir[512];
	char	strName[256]	=	"";

	m_iMemberCount	=	GetDlgItemInt(IDC_MEMBER_COUNT);

	if	(m_iMemberCount	<	1	||	m_iMemberCount	>	5)
	{
		cMSG::Error("error!!","member count failed!!\n\n1~5");
		return;
	}

	strcpy(dir,g_strLogFolder);

	if	(!cFILE::LoadBox(strName,"team list {*.txt}\0*.txt\0\0","txt",dir,"load team list file"))
	{
		SetCurrentDirectory(g_strLogFolder);
		
		return;
	}

	OnClearWCGList(TRUE);

	cDECODER	decoder;

	if	(!decoder.Upload(strName,NULL))
		return;

	int	iAddTeamCount	=	0;

	while(1)
	{
		char	*lpReadText;
		char	strTeamName[64],strMemberID[5][dID_LENGTH];

		lpReadText	=	decoder.GetStream();

		if	(!lpReadText)
			break;

		strcpy(strTeamName,lpReadText);

		for (int i=0;i<m_iMemberCount;i++)
		{
			lpReadText	=	decoder.GetWord();
			strcpy(strMemberID[i],lpReadText);
		}

		int	iTeamIndex	=	g_DBAccount.spInsertWCGTeamList(strTeamName);

		if	(iTeamIndex	==	-1)
		{
			cMSG::Error("Failed to add team","team [%s]",strTeamName);
			continue;
		}

		for (int iMember=0;iMember<m_iMemberCount;iMember++)
		{
			int	iResult	=	g_DBAccount.spInsertWCGTeamUserList(iTeamIndex,strMemberID[iMember]," ",0);//_idx WCG팀의 팀원을 삽입한다.

			if	(iResult	==	-1)
				cMSG::Error("Failed to add member","team [%s] member id [%s]",strTeamName,strMemberID[iMember]);
		}

		iAddTeamCount++;
	}

	cMSG::Put("result of add team list","%d numbers team entried",iAddTeamCount);
//	g_DBAccount.spModifyPvPTeamScore("team name", cDuelTeamRecord);	팀 정보 수정
//	g_DBAccount.spInsertWCGTeamList("team name");	팀 추가

	clearListControl(eCTL_INIT_WCGTEAMLIST);
	OnGetWCGList();
}

void CPAGE_PVPTOOL::OnGetWCGList()
{
	//	리스트 컨트롤을 초기화 한다.
	clearListControl(eCTL_INIT_WCGTEAMLIST);
	g_DBAccount.spGetWCGTeamInfo();

	char strTxt[64];
	for(int i=0;i<g_DBAccount.m_cWCGInfo.m_iTeamCount;i++)
	{
		sprintf(strTxt,"%d",g_DBAccount.m_cWCGInfo.m_pcInfo[i].m_wIndex);
		m_ctlListResult.addItem(i,0,strTxt);
		m_ctlListResult.setItem(i,1,g_DBAccount.m_cWCGInfo.m_pcInfo[i].m_strTeamName);
		sprintf(strTxt,"%d",g_DBAccount.m_cWCGInfo.m_pcInfo[i].m_record.m_iWinPoint);
		m_ctlListResult.setItem(i,2,strTxt);
		sprintf(strTxt,"%d",g_DBAccount.m_cWCGInfo.m_pcInfo[i].m_record.m_iBattleCount);
		m_ctlListResult.setItem(i,3,strTxt);
		sprintf(strTxt,"%d",g_DBAccount.m_cWCGInfo.m_pcInfo[i].m_record.m_iWinCount);
		m_ctlListResult.setItem(i,4,strTxt);
		sprintf(strTxt,"%d",g_DBAccount.m_cWCGInfo.m_pcInfo[i].m_record.m_iDefeatCount);
		m_ctlListResult.setItem(i,5,strTxt);
		sprintf(strTxt,"%d",g_DBAccount.m_cWCGInfo.m_pcInfo[i].m_record.m_iDrawCount);
		m_ctlListResult.setItem(i,6,strTxt);
	}
}

void CPAGE_PVPTOOL::OnClearWCGRank()
{
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;

	if	(MessageBox("Do you want to clear all of the WCG Ranking?","Caution!!",MB_YESNO)==IDNO)
		return;
	if	(MessageBox("Really??","Caution!!",MB_YESNO)==IDNO)
		return;

	g_DBAccount.m_cWCGInfo.m_iTeamCount	=	0;
	g_DBAccount.spClearWCGRank();

	clearListControl(eCTL_INIT_WCGRANKING);
}

void CPAGE_PVPTOOL::OnGetWCGRank()
{
	clearListControl(eCTL_INIT_WCGRANKING);
	g_DBAccount.spGetWCGRank();

	char strTxt[64];

	for(int i=0;i<g_DBAccount.m_cWCGInfo.m_iTeamCount;i++)
	{
		m_ctlListResult.addItem(i,0,g_DBAccount.m_cWCGInfo.m_pcRank[i].m_strName);
		sprintf(strTxt,"%d",g_DBAccount.m_cWCGInfo.m_pcRank[i].m_iWinPoint);
		m_ctlListResult.addItem(i,1,strTxt);
		sprintf(strTxt,"%d",g_DBAccount.m_cWCGInfo.m_pcRank[i].m_iBattleCount);
		m_ctlListResult.addItem(i,2,strTxt);
		sprintf(strTxt,"%d",g_DBAccount.m_cWCGInfo.m_pcRank[i].m_iWinCount);
		m_ctlListResult.addItem(i,3,strTxt);
		sprintf(strTxt,"%d",g_DBAccount.m_cWCGInfo.m_pcRank[i].m_iDefeatCount);
		m_ctlListResult.addItem(i,4,strTxt);
		sprintf(strTxt,"%d",g_DBAccount.m_cWCGInfo.m_pcRank[i].m_iDrawCount);
		m_ctlListResult.addItem(i,5,strTxt);
	}
}

void CPAGE_PVPTOOL::OnModifyWCGScore()
{
//	g_DBAccount.spModifyPvPScore("char name", cDuelRecord);
}

void CPAGE_PVPTOOL::clearListControl(eCTL_INIT _what)
{
	m_ctlListResult.resetColumn();

	if(_what == eCTL_INIT_WCGTEAMLIST)
	{
		m_ctlListResult.init(GetDlgItem(IDC_LIST_RESULT)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
		m_ctlListResult.addColumn(0,80,"INDEX");
		m_ctlListResult.addColumn(1,100,"TEAM NAME");
		m_ctlListResult.addColumn(2,100,"Win Point");
		m_ctlListResult.addColumn(3,100,"Battle Count");
		m_ctlListResult.addColumn(4,100,"Win Count");
		m_ctlListResult.addColumn(5,100,"Defeat Count");
		m_ctlListResult.addColumn(6,100,"Draw Count");
	}else
	{
		m_ctlListResult.init(GetDlgItem(IDC_LIST_RESULT)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
		m_ctlListResult.addColumn(0,80,"NAME");
		m_ctlListResult.addColumn(1,100,"Win Point");
		m_ctlListResult.addColumn(2,100,"Battle Count");
		m_ctlListResult.addColumn(3,100,"Win Count");
		m_ctlListResult.addColumn(4,100,"Defeat Count");
		m_ctlListResult.addColumn(5,100,"Draw Count");
	}
}