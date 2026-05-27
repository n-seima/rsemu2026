// SEARCHPAGE.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "SEARCHPAGE.h"
#include "cSRVUTIL.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void	RecvThread(LPVOID	lpParam);
int		iList = 0;
BOOL	isLoopThread = FALSE;
DWORD	l_dwSearchTime;

/////////////////////////////////////////////////////////////////////////////
// CSEARCHPAGE property page

IMPLEMENT_DYNCREATE(CSEARCHPAGE, CPropertyPage)

CSEARCHPAGE::CSEARCHPAGE() : CPropertyPage(CSEARCHPAGE::IDD)
{
	//{{AFX_DATA_INIT(CSEARCHPAGE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSEARCHPAGE::~CSEARCHPAGE()
{
}

void CSEARCHPAGE::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSEARCHPAGE)
	DDX_Control(pDX, IDC_SEARCH, m_ctlSearch);
	DDX_Control(pDX, IDC_SEARCH_ID, m_editId);
	DDX_Control(pDX, IDC_SEARCH_RESULT, m_listResult);
	DDX_Control(pDX, IDC_COMBO_YY1, m_cbY1);
	DDX_Control(pDX, IDC_COMBO_YY0, m_cbY0);
	DDX_Control(pDX, IDC_COMBO_MM1, m_cbM1);
	DDX_Control(pDX, IDC_COMBO_MM0, m_cbM0);
	DDX_Control(pDX, IDC_COMBO_HH1, m_cbH1);
	DDX_Control(pDX, IDC_COMBO_HH0, m_cbH0);
	DDX_Control(pDX, IDC_COMBO_DD1, m_cbD1);
	DDX_Control(pDX, IDC_COMBO_DD0, m_cbD0);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSEARCHPAGE, CPropertyPage)
	//{{AFX_MSG_MAP(CSEARCHPAGE)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEARCHPAGE message handlers
BOOL CSEARCHPAGE::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	l_dwSearchTime = timeGetTime();
	// TODO: Add extra initialization here
	int tc = 0;	char	tchar[10];
	int yy = cSRVUTIL::GetYear()+1900;
	int mm = cSRVUTIL::GetMonth()+1;
	int dd = cSRVUTIL::GetDay();
	int	hh = cSRVUTIL::GetHour();

	for(tc=2004 ; tc<= yy ; tc ++)
	{
		sprintf(tchar,"%d",tc);
		m_cbY0.AddString(tchar);m_cbY1.AddString(tchar);
	}

	m_cbY0.SetCurSel(yy-2004);
	m_cbY1.SetCurSel(yy-2004);
	
	for(tc = 1;tc <= 12 ; tc++)
	{
		sprintf(tchar,"%02d",tc);
		m_cbM0.AddString(tchar);m_cbM1.AddString(tchar);
	}
	m_cbM0.SetCurSel(mm-1);m_cbM1.SetCurSel(mm-1);

	int monthend[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if( (yy%4 == 0) && (yy%100 !=0) || yy%400 == 0)	monthend[2] = 29;

	for(tc = 1;tc<=monthend[mm];tc++)
	{
		sprintf(tchar,"%02d",tc);
		m_cbD0.AddString(tchar);
		m_cbD1.AddString(tchar);
	}
	m_cbD0.SetCurSel(dd-1);m_cbD1.SetCurSel(dd-1);

	for(tc = 0;tc<=23;tc++)
	{
		sprintf(tchar,"%02d",tc);
		m_cbH0.AddString(tchar);
		m_cbH1.AddString(tchar);
	}
	m_cbH0.SetCurSel(hh);m_cbH1.SetCurSel(hh);

	findList.Reset();

	isLoopThread = TRUE;
	m_listResult.EnableScrollBar(SB_BOTH);
	HANDLE hThread = (HANDLE)_beginthread(RecvThread, 0, this); 
	if(hThread==NULL)
	{
		isLoopThread = FALSE;
		return FALSE;
	}

	return TRUE;
}


void CSEARCHPAGE::OnSearch() 
{
	if(timeGetTime() - l_dwSearchTime < 3000)
	{
		char title[32];
		GetDlgItemText(IDC_SEARCH,title,32);
		if(stricmp(title,"SEARCH")!=0)	return;
	}
	this->OnClear();

	// TODO: Add your control notification handler code here
	PACKET_SEARCH		sPacket;
	sPacket.base.set(sizeof(PACKET_SEARCH),ePACKET_SEARCH);

	char	tmp[20];
	m_cbY0.GetWindowText(tmp,10);	sPacket._yy = atoi(tmp);
	m_cbY1.GetWindowText(tmp,10);	sPacket.yy = atoi(tmp);
	m_cbM0.GetWindowText(tmp,10);	sPacket._mm = atoi(tmp);
	m_cbM1.GetWindowText(tmp,10);	sPacket.mm = atoi(tmp);
	m_cbD0.GetWindowText(tmp,10);	sPacket._dd = atoi(tmp);
	m_cbD1.GetWindowText(tmp,10);	sPacket.dd = atoi(tmp);
	m_cbH0.GetWindowText(tmp,10);	sPacket._hh = atoi(tmp);
	m_cbH1.GetWindowText(tmp,10);	sPacket.hh = atoi(tmp);

	m_editId.GetWindowText(sPacket.strId,dID_LENGTH);	
	strcpy(sPacket.strIp,"");

	logSock.SendPacket((char *)&sPacket,sPacket.base.wSize);
	SetDlgItemText(IDC_SEARCH,"Wait a sec!");
	l_dwSearchTime = timeGetTime();
}

void	RecvThread(LPVOID	lpParam)
{
	CSEARCHPAGE	* page = (CSEARCHPAGE * )lpParam;
	PACKET_COMMON		rPacket;
	while(isLoopThread)
	{
		Sleep(1);
		memset(&rPacket,0,sizeof(PACKET_COMMON));
		if(!logSock.GetRecvPacket((char *)&rPacket))	continue;
		if(rPacket.base.wType != ePACKET_COMMON)	continue;

		char	strList[512];
		if(rPacket.dwtime< 0xfffffffe)
		{
			//	list에 추가한다.
			page->findList.AddNode(rPacket);
			//	List 컨트롤에 넣어준다.
			rPacket.strMsg[strlen(rPacket.strMsg)+1] = 0;
			sprintf(strList,"%d >> %d | %s | %s",iList,rPacket.dwtime,rPacket.strId,rPacket.strMsg);
			page->m_listResult.InsertString(iList,strList);
			iList++;
		}else
		{
			sprintf(strList,"%d >> %s",iList,rPacket.strMsg);
			page->m_listResult.InsertString(iList,strList);
			iList = 0;
			page->SetDlgItemText(IDC_SEARCH,"SEARCH");
		}
	}
}

void CSEARCHPAGE::OnClear() 
{
	// TODO: Add your control notification handler code here
	int delCount = m_listResult.GetCount();for(int c=0;c<delCount;c++)	m_listResult.DeleteString(0);
	findList.Reset();
	iList = 0;
}

void CSEARCHPAGE::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	isLoopThread = FALSE;
	Sleep(10);
	findList.Reset();		
}
