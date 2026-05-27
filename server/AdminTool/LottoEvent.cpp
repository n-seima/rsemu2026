// LottoEvent.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "LottoEvent.h"
#include "cPACKET_WORLDSERVER.h"
#include "AdminToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLottoEvent	*g_lpLottoEventDlg	=	NULL;

/////////////////////////////////////////////////////////////////////////////
// CLottoEvent dialog


CLottoEvent::CLottoEvent(CWnd* pParent /*=NULL*/)
	: CDialog(CLottoEvent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLottoEvent)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	g_lpLottoEventDlg	=	this;

}

CLottoEvent::~CLottoEvent()
{
	g_lpLottoEventDlg	=	NULL;
}


void CLottoEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLottoEvent)
	DDX_Control(pDX, IDC_WORLD_LIST, m_cbWorldList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLottoEvent, CDialog)
	//{{AFX_MSG_MAP(CLottoEvent)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_FILL_TIME, OnFillTime)
	ON_BN_CLICKED(IDC_SAVE_DATA, OnSaveData)
	ON_BN_CLICKED(IDC_LOAD_DATA, OnLoadData)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
	ON_BN_CLICKED(IDC_MODIFY_DATA, OnModifyData)
	ON_BN_CLICKED(IDC_GET_SERVER_DATA, OnGetServerData)
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_CONNECT_ALL, OnConnectAll)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_CBN_SELCHANGE(IDC_WORLD_LIST, OnSelchangeWorldList)
	ON_BN_CLICKED(IDC_REMOVE_EVENT, OnRemoveEvent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLottoEvent message handlers

void CLottoEvent::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CLottoEvent::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_iCount	=	0;

	// TODO: Add extra initialization here
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
		m_list.addColumn(1,160,"Name");
		m_list.addColumn(2,230,"Period",LVCFMT_CENTER);
		m_list.addColumn(3,74,"Lotto Item",LVCFMT_CENTER);
		m_list.addColumn(4,76,"Chance",LVCFMT_CENTER);
		m_list.addColumn(5,76,"Mob Level",LVCFMT_CENTER);
		m_list.addColumn(6,76,"Level Gab",LVCFMT_CENTER);
		m_list.addColumn(7,74,"C. Chance",LVCFMT_CENTER);
		m_list.addColumn(8,74,"C. Count",LVCFMT_CENTER);
	}

	fillList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void
CLottoEvent::fillLine(cLottoItemDropEvent *_lpInfo,int _iLine)
{
	m_list.setItem(_iLine,1,_lpInfo->m_strName);
	m_list.setItem(_iLine,2,_ms("%.2d/%.2d/%.2d %.2d:%.2d ~ %.2d/%.2d/%.2d %.2d:%.2d",
								_lpInfo->m_utBegin.year+2000,_lpInfo->m_utBegin.month,_lpInfo->m_utBegin.day,_lpInfo->m_utBegin.hour,_lpInfo->m_utBegin.minute,
								_lpInfo->m_utEnd.year+2000,_lpInfo->m_utEnd.month,_lpInfo->m_utEnd.day,_lpInfo->m_utEnd.hour,_lpInfo->m_utEnd.minute));
	m_list.setItem(_iLine,3,_ms("%d",_lpInfo->m_iLottoBoxSerial));
	m_list.setItem(_iLine,4,_ms("%d.%.4d%%",_lpInfo->m_iDropChance/10000,_lpInfo->m_iDropChance%10000));
	m_list.setItem(_iLine,5,_ms("%d~%d",_lpInfo->m_iMinMobLevel,_lpInfo->m_iMaxMobLevel));
	m_list.setItem(_iLine,6,_ms("%d",_lpInfo->m_iLevelRange));
	m_list.setItem(_iLine,7,_ms("%d.%.4d%%",_lpInfo->m_iCriticalChance/10000,_lpInfo->m_iCriticalChance%10000));
	m_list.setItem(_iLine,8,_ms("%d",_lpInfo->m_iCriticalCount));
}

void
CLottoEvent::fillList()
{
	m_list.reset();

	for (int i=0;i<m_iCount;i++)
	{
		m_list.addItem(i,0,_ms("#%.2d",i),i);
		fillLine(&m_aEventList[i],i);
	}

	fillDetailInfo();
}

void
CLottoEvent::fillDetailInfo()
{
	SetDlgItemText(IDC_NAME,m_editEvent.m_strName);
	SetDlgItemInt(IDC_YY1,m_editEvent.m_utBegin.year+2000);
	SetDlgItemInt(IDC_MM1,m_editEvent.m_utBegin.month);
	SetDlgItemInt(IDC_DD1,m_editEvent.m_utBegin.day);
	SetDlgItemInt(IDC_HH1,m_editEvent.m_utBegin.hour);
	SetDlgItemInt(IDC_MIN1,m_editEvent.m_utBegin.minute);

	SetDlgItemInt(IDC_YY2,m_editEvent.m_utEnd.year+2000);
	SetDlgItemInt(IDC_MM2,m_editEvent.m_utEnd.month);
	SetDlgItemInt(IDC_DD2,m_editEvent.m_utEnd.day);
	SetDlgItemInt(IDC_HH2,m_editEvent.m_utEnd.hour);
	SetDlgItemInt(IDC_MIN2,m_editEvent.m_utEnd.minute);

	SetDlgItemInt(IDC_MONSTER_MIN_LEVEL,m_editEvent.m_iMinMobLevel);
	SetDlgItemInt(IDC_MONSTER_MAX_LEVEL,m_editEvent.m_iMaxMobLevel);
		
	SetDlgItemInt(IDC_LEVEL_RANGE,m_editEvent.m_iLevelRange);
		
	SetDlgItemInt(IDC_DROP_ITEM,m_editEvent.m_iLottoBoxSerial);

	SetDlgItemText(IDC_DROP_CHANCE,_ms("%d.%.4d",m_editEvent.m_iDropChance/10000,m_editEvent.m_iDropChance%10000));
	SetDlgItemText(IDC_CRITICAL_CHANCE,_ms("%d.%.4d",m_editEvent.m_iCriticalChance/10000,m_editEvent.m_iCriticalChance%10000));
	SetDlgItemInt(IDC_CRITICAL_COUNT,m_editEvent.m_iCriticalCount);
}

void CLottoEvent::OnAdd() 
{
	CString	strText;

	GetDlgItemText(IDC_NAME,strText);
	strncpy(m_editEvent.m_strName,strText,dLOTTO_EVENT_NAME_LENGTH-2);

	m_editEvent.m_utBegin.year	=	GetDlgItemInt(IDC_YY1)-2000;
	m_editEvent.m_utBegin.month	=	GetDlgItemInt(IDC_MM1);
	m_editEvent.m_utBegin.day	=	GetDlgItemInt(IDC_DD1);
	m_editEvent.m_utBegin.hour	=	GetDlgItemInt(IDC_HH1);
	m_editEvent.m_utBegin.minute=	GetDlgItemInt(IDC_MIN1);

	m_editEvent.m_utEnd.year	=	GetDlgItemInt(IDC_YY2)-2000;
	m_editEvent.m_utEnd.month	=	GetDlgItemInt(IDC_MM2);
	m_editEvent.m_utEnd.day		=	GetDlgItemInt(IDC_DD2);
	m_editEvent.m_utEnd.hour	=	GetDlgItemInt(IDC_HH2);
	m_editEvent.m_utEnd.minute	=	GetDlgItemInt(IDC_MIN2);
 
 	m_editEvent.m_iMinMobLevel	=	GetDlgItemInt(IDC_MONSTER_MIN_LEVEL);
 	m_editEvent.m_iMaxMobLevel	=	GetDlgItemInt(IDC_MONSTER_MAX_LEVEL);

 	m_editEvent.m_iLevelRange	=	GetDlgItemInt(IDC_LEVEL_RANGE);
 	m_editEvent.m_iLottoBoxSerial=	GetDlgItemInt(IDC_DROP_ITEM);

	{
 		GetDlgItemText(IDC_DROP_CHANCE,strText);

		float	fValue	=	atof(strText);

		m_editEvent.m_iDropChance	=	fValue*10000;
	}

	{
 		GetDlgItemText(IDC_CRITICAL_CHANCE,strText);

		float	fValue	=	atof(strText);

		m_editEvent.m_iCriticalChance	=	fValue*10000;
	 	m_editEvent.m_iCriticalCount=	GetDlgItemInt(IDC_CRITICAL_COUNT);
	}

	memcpy(&m_aEventList[m_iCount],&m_editEvent,sizeof(cLottoItemDropEvent));

	m_list.addItem(m_iCount,0,_ms("#%.2d",m_iCount),m_iCount);
	fillLine(&m_editEvent,m_iCount);

	m_list.selectItem(m_iCount);

	m_iCount++;
}

void CLottoEvent::OnFillTime() 
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

void CLottoEvent::OnSaveData() 
{
	char	dir[512];
	char	strName[256]	=	"";

	sprintf(dir,"%s\\LED",g_strLogFolder);

	cFOLDER::Create(dir,TRUE);

	if	(!cFILE::SaveBox(strName,"Lotto Event data {*.led}\0*.led\0\0","led",dir,"Save Lotto Event data"))
	{
		SetCurrentDirectory(g_strLogFolder);

		return;
	}

	cFILE	file;

	file.Open(strName,"wb");

	file.Write(&m_iCount,4);
	file.Write(m_aEventList,sizeof(cLottoItemDropEvent)*m_iCount);
	file.Close();
}

void CLottoEvent::OnLoadData() 
{
	char	dir[512];
	char	strName[256]	=	"";

	sprintf(dir,"%s\\LED",g_strLogFolder);

	cFOLDER::Create(dir,TRUE);

	if	(!cFILE::LoadBox(strName,"Lotto Event data {*.led}\0*.led\0\0","led",dir,"Load Lotto Event data"))
	{
		SetCurrentDirectory(g_strLogFolder);

		return;
	}

	cFILE	file;

	file.Open(strName,"rb");

	file.Read(&m_iCount,4);
	file.Read(m_aEventList,sizeof(cLottoItemDropEvent)*m_iCount);
	file.Close();

	fillList();
}

void CLottoEvent::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	if	(pNMListView->uOldState	!=	0)
		return;

	int		iSelectEvent	=	pNMListView->iItem;

	if	(iSelectEvent	==	-1)
		return;

	memcpy(&m_editEvent,&m_aEventList[iSelectEvent],sizeof(m_editEvent));

	fillDetailInfo();
}

void CLottoEvent::OnModifyData() 
{
#ifdef	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(m_iSelectServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	char*	lpstrSelectWorldName	=	sList[m_iSelectServer].strName;

	if	(cMSG::YESNO("Modify current data","update current data to server [%s]\n\nAre you OK?",lpstrSelectWorldName)==IDNO)
		return;

	SetDlgItemText(IDC_STATUS,_ms(" Work Status : send current data to server [%s] ... ",lpstrSelectWorldName));

 	SERVERStoWORLD_MODIFY_LOTTO_EVENT_DATA	packet;

	packet.wCount	=	m_iCount;
	memcpy(packet.aEventList,m_aEventList,sizeof(cLottoItemDropEvent)*m_iCount);

	packet.base.set(sizeof(packet),dSERVERStoWORLD_MODIFY_LOTTO_EVENT_DATA);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aEventList)+sizeof(cLottoItemDropEvent)*m_iCount;

 	SendPacketToWorld(m_iSelectServer,(char *)&packet);

	SetDlgItemText(IDC_STATUS,_ms(" Work Status : send current data to server [%s]. Complete. ",lpstrSelectWorldName));
}

void CLottoEvent::OnGetServerData() 
{
	if	(m_iSelectServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	char*	lpstrSelectWorldName	=	sList[m_iSelectServer].strName;

	m_iAskDataServer	=	m_iSelectServer;

	SetDlgItemText(IDC_STATUS,_ms(" Work Status : Receive data from server [%s]",lpstrSelectWorldName));

	if	(!SendWP_SimpleWork(m_iSelectServer,eSWAW_ASK_LOTTO_EVENT_DATA,0))
	{
		ERRMSG("Sending Packet Error Retry!!");
		return;
	}
}

void
CLottoEvent::receiveLottoEventData(WORLDtoSERVERS_LOTTO_EVENT_DATA *_lpPacket)
{
	char*	lpstrSelectWorldName	=	sList[m_iSelectServer].strName;

	m_iAskDataServer	=	m_iSelectServer;

	SetDlgItemText(IDC_STATUS,_ms(" Work Status : Receive data from server [%s]. Complete!",lpstrSelectWorldName));

	m_iCount	=	_lpPacket->wCount;

	memcpy(m_aEventList,_lpPacket->aEventList,sizeof(cLottoItemDropEvent)*m_iCount);

	fillList();
}

void CLottoEvent::OnConnect() 
{
	int	iSelectServer	=	m_cbWorldList.GetCurSel();

	if	(iSelectServer	==	-1)
		return;

	m_iSelectServer		=	m_cbWorldList.GetItemData(iSelectServer);

	g_lpAdminDlg->connectWorld(m_iSelectServer);

}

void CLottoEvent::OnConnectAll() 
{
	g_lpAdminDlg->connectWorld(-1);
}

void CLottoEvent::OnModify() 
{
	int	iIndex	=	m_list.getSelectedItem();

	if	(iIndex	==	-1)
		return;
	
	CString	strText;

	GetDlgItemText(IDC_NAME,strText);
	strncpy(m_editEvent.m_strName,strText,dLOTTO_EVENT_NAME_LENGTH-2);

	m_editEvent.m_utBegin.year	=	GetDlgItemInt(IDC_YY1)-2000;
	m_editEvent.m_utBegin.month	=	GetDlgItemInt(IDC_MM1);
	m_editEvent.m_utBegin.day	=	GetDlgItemInt(IDC_DD1);
	m_editEvent.m_utBegin.hour	=	GetDlgItemInt(IDC_HH1);
	m_editEvent.m_utBegin.minute=	GetDlgItemInt(IDC_MIN1);

	m_editEvent.m_utEnd.year	=	GetDlgItemInt(IDC_YY2)-2000;
	m_editEvent.m_utEnd.month	=	GetDlgItemInt(IDC_MM2);
	m_editEvent.m_utEnd.day		=	GetDlgItemInt(IDC_DD2);
	m_editEvent.m_utEnd.hour	=	GetDlgItemInt(IDC_HH2);
	m_editEvent.m_utEnd.minute	=	GetDlgItemInt(IDC_MIN2);
 
 	m_editEvent.m_iMinMobLevel	=	GetDlgItemInt(IDC_MONSTER_MIN_LEVEL);
 	m_editEvent.m_iMaxMobLevel	=	GetDlgItemInt(IDC_MONSTER_MAX_LEVEL);

 	m_editEvent.m_iLevelRange	=	GetDlgItemInt(IDC_LEVEL_RANGE);
 	m_editEvent.m_iLottoBoxSerial=	GetDlgItemInt(IDC_DROP_ITEM);

	{
 		GetDlgItemText(IDC_DROP_CHANCE,strText);

		float	fValue	=	atof(strText);

		m_editEvent.m_iDropChance	=	fValue*10000;
	}

	{
 		GetDlgItemText(IDC_CRITICAL_CHANCE,strText);

		float	fValue	=	atof(strText);

		m_editEvent.m_iCriticalChance	=	fValue*10000;
	 	m_editEvent.m_iCriticalCount=	GetDlgItemInt(IDC_CRITICAL_COUNT);
	}

	memcpy(&m_aEventList[iIndex],&m_editEvent,sizeof(cLottoItemDropEvent));

	fillLine(&m_editEvent,iIndex);

	m_list.selectItem(iIndex);
}

void CLottoEvent::OnSelchangeWorldList() 
{
	int	iSelectServer	=	m_cbWorldList.GetCurSel();

	if	(iSelectServer	==	-1)
	{
		m_iSelectServer	=	-1;
		return;
	}

	m_iSelectServer		=	m_cbWorldList.GetItemData(iSelectServer);

}

void CLottoEvent::OnRemoveEvent() 
{
	int	iIndex	=	m_list.getSelectedItem();

	if	(iIndex	==	-1)
		return;
	
	CString	strText;

	memset(&m_editEvent,0,sizeof(m_editEvent));
	strcpy(m_editEvent.m_strName,"- REMOVED -");

	memcpy(&m_aEventList[iIndex],&m_editEvent,sizeof(cLottoItemDropEvent));

	fillLine(&m_editEvent,iIndex);

	m_list.selectItem(iIndex);
}
