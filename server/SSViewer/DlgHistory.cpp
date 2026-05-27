// DlgHistory.cpp : implementation file
//

#include "stdafx.h"
#include "SSViewer.h"
#include "DlgHistory.h"
#include "SSViewerDlg.h"
#include "cServerStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cDlgHistory dialog


cDlgHistory::cDlgHistory(CWnd* pParent /*=NULL*/)
	: CDialog(cDlgHistory::IDD, pParent)
{
	//{{AFX_DATA_INIT(cDlgHistory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cDlgHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cDlgHistory)
	DDX_Control(pDX, IDC_COMBO_MONTH, m_comboMonth);
	DDX_Control(pDX, IDC_COMBO_YEAR, m_comboYear);
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cDlgHistory, CDialog)
	//{{AFX_MSG_MAP(cDlgHistory)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_CBN_SELCHANGE(IDC_COMBO_MONTH, OnSelchangeComboMonth)
	ON_CBN_SELCHANGE(IDC_COMBO_YEAR, OnSelchangeComboYear)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DATA, OnDblclkListData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cDlgHistory message handlers

void cDlgHistory::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	g_pcMainFramePtr->m_pcDlgHistory = NULL;
	delete this;

	CDialog::PostNcDestroy();
}

void cDlgHistory::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
	//CDialog::OnClose();
}

void cDlgHistory::OnButtonClose() 
{
	OnClose();
}

BOOL cDlgHistory::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	// Esc, Enter 키 막기(다이얼로그가 죽음)
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
		return 0;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL cDlgHistory::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char szTemp[256];

	SYSTEMTIME systimeToday;
	GetLocalTime(&systimeToday);

	// Month
	for(int i=0; i<12; i++){
		sprintf(szTemp, "%d", i + 1);
		m_comboMonth.AddString(szTemp);
		if(systimeToday.wMonth == i + 1){
			m_comboMonth.SetCurSel(i);
		}
	}	

	// Year	
	int iLimitYear = 10;
	for(i=0; i<iLimitYear; i++){
		sprintf(szTemp, "%d", systimeToday.wYear - i);
		m_comboYear.AddString(szTemp);	
	}
	m_comboYear.SetCurSel(0);
	
	m_iYear = systimeToday.wYear;
	m_iMonth = systimeToday.wMonth;

	// 리스트 컨트롤 높이 설정
	CImageList smallImage;
	int aa = smallImage.Create(IDB_BITMAP_DUMMY, 1, 0, RGB(255, 0, 0));
	m_listData.SetImageList(&smallImage, LVSIL_SMALL);		

	// ListCtrl
	m_listData.InsertColumn(0, _T("Index"), LVCFMT_LEFT, 50);
	m_listData.InsertColumn(1, _T("ServerName"), LVCFMT_LEFT, 200);
	m_listData.InsertColumn(2, _T("ErrorCode"), LVCFMT_CENTER, 70);	
	m_listData.InsertColumn(3, _T("ErrorTime"), LVCFMT_CENTER, 120);			
	m_listData.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);	// 밑줄나오게 설정

	getData();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cDlgHistory::addItem(int _iIdx, char *_pszServerName, int _iErrorCode, char *_pszErrorTime)
{
	TCHAR szNum[8];
	itoa(_iIdx, szNum, 10);	
	
	LV_ITEM a;
	a.iItem = m_listData.GetItemCount();	// 삽입 위치
	a.mask = LVIF_TEXT | LVIF_STATE;	// 실직적으로 표현될값
	a.iSubItem = 0;	// 열인덱스
	a.iImage = 0;
	a.stateMask = LVIS_STATEIMAGEMASK;	// 상태변화를 Mask 처리
	a.state = INDEXTOSTATEIMAGEMASK(1);	// 유효한 상태 비트
	a.pszText = szNum;	// 문자열 
	m_listData.InsertItem(&a);		
	
	TCHAR szTemp[512];
	m_listData.SetItemText(a.iItem, 1, _pszServerName);
	sprintf(szTemp, "%d", _iErrorCode);
	m_listData.SetItemText(a.iItem, 2, szTemp);
	m_listData.SetItemText(a.iItem, 3, _pszErrorTime);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cDlgHistory::OnSelchangeComboMonth() 
{
	char szYear[40];
	m_comboMonth.GetLBText(m_comboMonth.GetCurSel(), szYear);
	m_iMonth = atoi(szYear);
	getData();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cDlgHistory::OnSelchangeComboYear() 
{
	char szMonth[40];
	m_comboYear.GetLBText(m_comboYear.GetCurSel(), szMonth);	
	m_iYear = atoi(szMonth);
	getData();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cDlgHistory::OnDblclkListData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	int intNum = pNMListView->iItem;

	// 인덱스 컬럼
	if(m_listData.GetItemText(intNum, 0) == _T("")){					// 데이타가 없는곳을 더블클릭시
		;
	}else{		
		int iDataIdx = atoi(m_listData.GetItemText(intNum, 0));
		detailViewNotice(iDataIdx);
	}

	*pResult = 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 하나의 디테일 데이타 가져오기
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cDlgHistory::detailViewNotice(int _iIdx)
{
	TCHAR szMessage[1024];
	sprintf(szMessage, "이곳에 데이타를 넣어주면 됩니다");

	if(::MessageBox(m_hWnd, szMessage, "\"확인\" 을 누루면 내용이 Copy 됩니다", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK){
		// 클립보드에 내용저장
		int iLen = sizeof(szMessage);
		HGLOBAL hGlobal;
		char* pGlobal;
		hGlobal = GlobalAlloc (GHND | GMEM_SHARE, iLen);
		pGlobal = (char*)GlobalLock(hGlobal);
		memcpy((void*)pGlobal, (const void*)szMessage, iLen);    
		OpenClipboard();
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hGlobal);
		CloseClipboard();
	}

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : DB 에서 데이타 받아오기
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cDlgHistory::getData()
{
	m_listData.DeleteAllItems();

	// TEST Data
	char szTemp[512];
	sprintf(szTemp, "%04d-%02d 12:22", m_iYear, m_iMonth);
	for(int i=0; i<g_SS.m_listSSError.size(); i++){		
		addItem(i+1, g_SS.m_listSSError.at(i).strName,g_SS.m_listSSError.at(i).iErrorType, g_SS.m_listSSError.at(i).Param2_char512);
//		addItem(i + 1, "소룬드", i + 10, szTemp);
	}

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2
	// m_iYear, m_iMonth 2개의 값으로 처리하면 됨
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2
}
