// EventTimeManager.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "EventTimeManager.h"
#include "AdminToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEventTimeManager	*g_lpEventTimeManagerDlg	=	NULL;

/////////////////////////////////////////////////////////////////////////////
// CEventTimeManager dialog


CEventTimeManager::CEventTimeManager(CWnd* pParent /*=NULL*/)
	: CDialog(CEventTimeManager::IDD, pParent)
{
	g_lpEventTimeManagerDlg	=	this;
	//{{AFX_DATA_INIT(CEventTimeManager)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CEventTimeManager::~CEventTimeManager()   // standard destructor
{
	g_lpEventTimeManagerDlg	=	NULL;
}


void CEventTimeManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventTimeManager)
	DDX_Control(pDX, IDC_WORLD_LIST, m_cbWorldList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEventTimeManager, CDialog)
	//{{AFX_MSG_MAP(CEventTimeManager)
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_MODIFY_DATA, OnModifyData)
	ON_BN_CLICKED(IDC_GET_SERVER_DATA, OnGetServerData)
	ON_BN_CLICKED(IDC_SAVE_DATA, OnSaveData)
	ON_BN_CLICKED(IDC_LOAD_DATA, OnLoadData)
	ON_BN_CLICKED(IDC_FILL_TIME, OnFillTime)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventTimeManager message handlers

void CEventTimeManager::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CEventTimeManager::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	{
		for (int i=0;i<g_iWorldCount;i++)
		{
			if	(g_lWhereisServer== eCOUNTRY_INNERTEST)
			{
				if (sList[i].isTestServer	!=	2)
					continue;
			}
			else
			{
				if (sList[i].isTestServer	==	2)
					continue;
			}

			char	strName[50];

			sprintf(strName,"%d] %s",i,sList[i].strName);

			int	iIndex	=	m_cbWorldList.AddString(strName);
			m_cbWorldList.SetItemData(iIndex,i);
		}

		m_cbWorldList.SetCurSel(0);
		m_iSelectServer	=	m_cbWorldList.GetItemData(0);
	}
	
	{
		m_list.init(GetDlgItem(IDC_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
		::SendMessage(GetDlgItem(IDC_LIST)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

		m_list.addColumn(0,30,"ID",LVCFMT_CENTER);
		m_list.addColumn(1,220,"Name ",LVCFMT_RIGHT);
		m_list.addColumn(2,230," Period",LVCFMT_LEFT);
	}


	m_iEventCount	=	0;

	memset(m_aEventTimeList,0,sizeof(m_aEventTimeList));

	for (int i=0;;i++)
	{
		if	(strcmp(g_strEventTimeName[i],"end")==0)
		{
			m_iEventCount	=	i;
			break;
		}
	}

	m_editEvent.m_timeBegin.update();
	m_editEvent.m_timeEnd.update();

	fillList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEventTimeManager::OnConnect() 
{
	int	iSelectServer	=	m_cbWorldList.GetCurSel();

	if	(iSelectServer	==	-1)
		return;

	m_iSelectServer		=	m_cbWorldList.GetItemData(iSelectServer);

	g_lpAdminDlg->connectWorld(m_iSelectServer);
}

void CEventTimeManager::OnModifyData() 
{
#ifdef	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#else
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#endif

	if	(m_iSelectServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	if	(cMSG::YESNO("Modify current data","update current data to all server \n\nAre you OK?")==IDNO)
		return;

	SetDlgItemText(IDC_STATUS,_ms(" Work Status : send current data to server ... "));

 	SERVERStoWORLD_MODIFY_EVENT_TIME	packet;

	packet.wCount			=	m_iEventCount;
	packet.wIsByLoginServer	=	FALSE;

	memcpy(packet.aEventList,m_aEventTimeList,sizeof(cEventTimeDefine)*m_iEventCount);

	packet.base.set(sizeof(packet),dSERVERStoWORLD_MODIFY_EVENT_TIME);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aEventList)+sizeof(cEventTimeDefine)*m_iEventCount;

 	SendPacketToWorld(m_iSelectServer,(char *)&packet);

	SetDlgItemText(IDC_STATUS,_ms(" Work Status : send current data to server. Complete. "));
}

void CEventTimeManager::OnGetServerData() 
{
	if	(m_iSelectServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	char*	lpstrSelectWorldName	=	sList[m_iSelectServer].strName;

	m_iAskDataServer	=	m_iSelectServer;

	SetDlgItemText(IDC_STATUS,_ms(" Work Status : Receive data from server [%s]",lpstrSelectWorldName));

	if	(!SendWP_SimpleWork(m_iSelectServer,eSWAW_ASK_EVENT_TIME_DATA,0))
	{
		ERRMSG("Sending Packet Error Retry!!");
		return;
	}
}

void CEventTimeManager::OnSaveData() 
{
	// TODO: Add your control notification handler code here
	
}

void CEventTimeManager::OnLoadData() 
{
	// TODO: Add your control notification handler code here
	
}

void CEventTimeManager::OnFillTime() 
{
	CTimeInfo	currentTime;

	currentTime.update();

	SetDlgItemInt(IDC_YY1,currentTime.m_wYear);
	SetDlgItemInt(IDC_MM1,currentTime.m_wMonth);
	SetDlgItemInt(IDC_DD1,currentTime.m_wDay);
	SetDlgItemInt(IDC_HH1,currentTime.m_wHour);
	SetDlgItemInt(IDC_MIN1,00);

	SetDlgItemInt(IDC_YY2,currentTime.m_wYear);
	SetDlgItemInt(IDC_MM2,currentTime.m_wMonth);
	SetDlgItemInt(IDC_DD2,currentTime.m_wDay);
	SetDlgItemInt(IDC_HH2,currentTime.m_wHour);
	SetDlgItemInt(IDC_MIN2,00);
}

void CEventTimeManager::OnModify() 
{
	int	iIndex	=	m_list.getSelectedItem();

	if	(iIndex	==	-1)
	{
		cMSG::Put("warning!!","please select event");
		return;
	}

	m_editEvent.m_timeBegin.m_wYear		=	GetDlgItemInt(IDC_YY1);
	m_editEvent.m_timeBegin.m_wMonth	=	GetDlgItemInt(IDC_MM1);
	m_editEvent.m_timeBegin.m_wDay		=	GetDlgItemInt(IDC_DD1);
	m_editEvent.m_timeBegin.m_wHour		=	GetDlgItemInt(IDC_HH1);
	m_editEvent.m_timeBegin.m_wMinute	=	GetDlgItemInt(IDC_MIN1);

	m_editEvent.m_timeEnd.m_wYear		=	GetDlgItemInt(IDC_YY2);
	m_editEvent.m_timeEnd.m_wMonth		=	GetDlgItemInt(IDC_MM2);
	m_editEvent.m_timeEnd.m_wDay		=	GetDlgItemInt(IDC_DD2);
	m_editEvent.m_timeEnd.m_wHour		=	GetDlgItemInt(IDC_HH2);
	m_editEvent.m_timeEnd.m_wMinute		=	GetDlgItemInt(IDC_MIN2);

	memcpy(&m_aEventTimeList[iIndex].m_timeBegin,&m_editEvent.m_timeBegin,sizeof(CTimeInfo));
	memcpy(&m_aEventTimeList[iIndex].m_timeEnd,&m_editEvent.m_timeEnd,sizeof(CTimeInfo));

	fillLine(iIndex);
}

void CEventTimeManager::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	if	(pNMListView->uOldState	!=	0)
		return;

	int		iSelectEvent	=	pNMListView->iItem;

	if	(iSelectEvent	==	-1)
		return;

	memcpy(&m_editEvent,&m_aEventTimeList[iSelectEvent],sizeof(m_editEvent));

	fillDetailInfo();
}


void
CEventTimeManager::fillLine(int _iLine)
{
	cEventTimeDefine	*lpInfo	=	&m_aEventTimeList[_iLine];

	m_list.setItem(_iLine,1,_ms("%s ",g_strEventTimeName[_iLine]));
	m_list.setItem(_iLine,2,_ms(" %.4d/%.2d/%.2d %.2d:%.2d ~ %.4d/%.2d/%.2d %.2d:%.2d",
								lpInfo->m_timeBegin.m_wYear,lpInfo->m_timeBegin.m_wMonth,lpInfo->m_timeBegin.m_wDay,lpInfo->m_timeBegin.m_wHour,lpInfo->m_timeBegin.m_wMinute,
								lpInfo->m_timeEnd.m_wYear,lpInfo->m_timeEnd.m_wMonth,lpInfo->m_timeEnd.m_wDay,lpInfo->m_timeEnd.m_wHour,lpInfo->m_timeEnd.m_wMinute));
}

void
CEventTimeManager::fillList()
{
	m_list.reset();

	for (int i=0;i<m_iEventCount;i++)
	{
		m_list.addItem(i,0,_ms("#%.2d",i),i);
		fillLine(i);
	}

	fillDetailInfo();
}

void
CEventTimeManager::fillDetailInfo()
{
	SetDlgItemInt(IDC_YY1,m_editEvent.m_timeBegin.m_wYear);
	SetDlgItemInt(IDC_MM1,m_editEvent.m_timeBegin.m_wMonth);
	SetDlgItemInt(IDC_DD1,m_editEvent.m_timeBegin.m_wDay);
	SetDlgItemInt(IDC_HH1,m_editEvent.m_timeBegin.m_wHour);
	SetDlgItemInt(IDC_MIN1,m_editEvent.m_timeBegin.m_wMinute);

	SetDlgItemInt(IDC_YY2,m_editEvent.m_timeEnd.m_wYear);
	SetDlgItemInt(IDC_MM2,m_editEvent.m_timeEnd.m_wMonth);
	SetDlgItemInt(IDC_DD2,m_editEvent.m_timeEnd.m_wDay);
	SetDlgItemInt(IDC_HH2,m_editEvent.m_timeEnd.m_wHour);
	SetDlgItemInt(IDC_MIN2,m_editEvent.m_timeEnd.m_wMinute);
}

void
CEventTimeManager::receiveEventTimeData(SERVERStoWORLD_MODIFY_EVENT_TIME *_lpPacket)
{

	m_iEventCount		=	max(_lpPacket->wCount,m_iEventCount);
	memcpy(m_aEventTimeList, _lpPacket->aEventList, sizeof(cEventTimeDefine)*_lpPacket->wCount);
	memcpy(&m_editEvent,&m_aEventTimeList[m_list.getSelectedItem()],sizeof(m_editEvent));
	fillList();
}
