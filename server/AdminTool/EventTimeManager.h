#if !defined(AFX_EVENTTIMEMANAGER_H__1043E235_30C9_4678_9D77_4B5A545BFE8E__INCLUDED_)
#define AFX_EVENTTIMEMANAGER_H__1043E235_30C9_4678_9D77_4B5A545BFE8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventTimeManager.h : header file
//

#include	"windowExtendAPI.h"
#include	"eventDefine.h"

/////////////////////////////////////////////////////////////////////////////
// CEventTimeManager dialog
struct	SERVERStoWORLD_MODIFY_EVENT_TIME;

class CEventTimeManager : public CDialog
{
// Construction
public:
	CEventTimeManager(CWnd* pParent = NULL);   // standard constructor
	virtual	~CEventTimeManager();   // standard destructor

// Dialog Data
	//{{AFX_DATA(CEventTimeManager)
	enum { IDD = IDD_EVENT_TIME };
	cListView	m_list;
	CComboBox	m_cbWorldList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventTimeManager)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEventTimeManager)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnConnect();
	afx_msg void OnModifyData();
	afx_msg void OnGetServerData();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
	afx_msg void OnFillTime();
	afx_msg void OnModify();
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int					m_iSelectServer,m_iAskDataServer;
	int					m_iEventCount;
	cEventTimeDefine	m_editEvent;
	cEventTimeDefine	m_aEventTimeList[dMAX_EVENT_TIME_COUNT];

	void		fillList();
	void		fillLine(int _iLine);
	void		fillDetailInfo();
public:
	void		receiveEventTimeData(SERVERStoWORLD_MODIFY_EVENT_TIME *_lpPacket);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

extern	CEventTimeManager	*g_lpEventTimeManagerDlg;

#endif // !defined(AFX_EVENTTIMEMANAGER_H__1043E235_30C9_4678_9D77_4B5A545BFE8E__INCLUDED_)