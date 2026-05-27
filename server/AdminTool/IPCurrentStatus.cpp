// IPCurrentStatus.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "IPCurrentStatus.h"
#include "cITEM.h"
#include "cPACKET_WORLDSERVER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cIPCurrentStatus dialog


cIPCurrentStatus::cIPCurrentStatus(CWnd* pParent /*=NULL*/)
	: CDialog(cIPCurrentStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(cIPCurrentStatus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cIPCurrentStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cIPCurrentStatus)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(cIPCurrentStatus, CDialog)
	//{{AFX_MSG_MAP(cIPCurrentStatus)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_RESET_COUNTER, OnResetCounter)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_RESET_CURRENT_COUNTER, OnResetCurrentCounter)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cIPCurrentStatus message handlers

BOOL cIPCurrentStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_listItem.init(GetDlgItem(IDC_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);

	::SendMessage(GetDlgItem(IDC_LIST)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

	m_listItem.addColumn(0,30,"NO");
	m_listItem.addColumn(1,300,"Name");
	m_listItem.addColumn(2,75,"Frequency");
	m_listItem.addColumn(3,120,"Current Count");
	m_listItem.addColumn(4,50,"Limit");

	updateItemList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void cIPCurrentStatus::updateItemList()
{
	m_listItem.reset();

	for (int i=0;i<m_editPack.m_wCount;i++)
	{
		char		strItemName[128];

		cItemInPack	*lpItem	=	&m_editPack.m_aItems[i];

		lpItem->m_dwSerial	=	max(lpItem->m_wCurrentCount,lpItem->m_dwSerial);

		strcpy(strItemName,((cITEM*)lpItem)->getName());

		m_listItem.addItem(i,0,_ms("%.3d",i),i);
		m_listItem.setItem(i,1,strItemName);
		m_listItem.setItem(i,2,_ms("1/%d",lpItem->m_dwFrequency));
		m_listItem.setItem(i,3,_ms("%8d/%d",lpItem->m_wCurrentCount,lpItem->m_dwSerial));
		m_listItem.setItem(i,4,_ms("%d",lpItem->m_wLimitCountOfServer));
	}
}

void cIPCurrentStatus::OnExport() 
{
	// TODO: Add your control notification handler code here
	OPENFILENAME	ofn;

	char	dir[512];
	char	strName[256]	=	"";

	strcpy(dir,g_strLogFolder);

	SetCurrentDirectory(dir);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow()->m_hWnd;
	ofn.lpstrFilter		=	"*.txt";
	ofn.lpstrTitle		=	"please select file";
	ofn.lpstrFile		=	strName;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.lpstrDefExt		=	"txt";
	ofn.lpstrInitialDir	=	dir;

	if (!GetSaveFileName(&ofn))
	{
		SetCurrentDirectory(g_strLogFolder);

		return;
	}

	cFILE	file;

	file.Open(strName,"wb");

	for (int i=0;i<m_editPack.m_wCount;i++)
	{
		char		strItemName[128];

		cItemInPack	*lpItem	=	&m_editPack.m_aItems[i];

		strcpy(strItemName,((cITEM*)lpItem)->getName());

		file.writeStreamCR("%s [current %d] [total %d] [limit %d]",strItemName,lpItem->m_wCurrentCount,lpItem->m_dwSerial,lpItem->m_wLimitCountOfServer);
	}

	file.Close();

	SetCurrentDirectory(g_strLogFolder);
}

void cIPCurrentStatus::OnResetCounter() 
{
}

void cIPCurrentStatus::OnRefresh() 
{
	// TODO: Add your control notification handler code here

	SERVERStoWORLD_ASK_ITEM_PACK_LIST	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ASK_ITEM_PACK_LIST);
	packet.wPackSerial	=	m_editPack.m_wSerial;

	sList[m_iSelectServer].worldsock.SendPacket((char *)&packet,packet.base.wSize);	
}

void cIPCurrentStatus::OnResetCurrentCounter() 
{
	if (CheckOperatorLevel(eAL_MASTER_OPERATER)==FALSE)
		return;

	if (cMSG::YESNO("Reset current item pack drop item count","Do you want continue this work?")	==	IDNO)
		return;

	// TODO: Add your control notification handler code here
	SERVERStoWORLD_RESET_DROP_PACK_ITEM_COUNT	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_RESET_DROP_PACK_ITEM_COUNT);
	packet.wPackSerial	=	m_editPack.m_wSerial;

	sList[m_iSelectServer].worldsock.SendPacket((char *)&packet,packet.base.wSize);	
	
}

void cIPCurrentStatus::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}
