#if !defined(AFX_LOTTOEVENT_H__71009421_77F6_4E14_A2DE_DF7E31314EE1__INCLUDED_)
#define AFX_LOTTOEVENT_H__71009421_77F6_4E14_A2DE_DF7E31314EE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LottoEvent.h : header file
//
#include	"windowExtendAPI.h"
#include	"eventDefine.h"

/////////////////////////////////////////////////////////////////////////////
// CLottoEvent dialog

struct	WORLDtoSERVERS_LOTTO_EVENT_DATA;

class CLottoEvent : public CDialog
{
// Construction
public:
	CLottoEvent(CWnd* pParent = NULL);   // standard constructor

	virtual	~CLottoEvent();

// Dialog Data
	//{{AFX_DATA(CLottoEvent)
	enum { IDD = IDD_LOTTO_EVENT };

	cListView			m_list;
	CComboBox			m_cbWorldList;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLottoEvent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLottoEvent)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnFillTime();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModifyData();
	afx_msg void OnGetServerData();
	afx_msg void OnConnect();
	afx_msg void OnConnectAll();
	afx_msg void OnModify();
	afx_msg void OnSelchangeWorldList();
	afx_msg void OnRemoveEvent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

		
protected:
	int					m_iSelectServer;
	int					m_iCount;
	int					m_iAskDataServer;

	cLottoItemDropEvent	m_editEvent;
	cLottoItemDropEvent	m_aEventList[dLOTTO_EVENT_COUNT];

	void		fillList();
	void		fillLine(cLottoItemDropEvent *_lpInfo,int _iLine);
	void		fillDetailInfo();

public:
	void		receiveLottoEventData(WORLDtoSERVERS_LOTTO_EVENT_DATA *_lpPacket);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

extern	CLottoEvent	*g_lpLottoEventDlg;

#endif // !defined(AFX_LOTTOEVENT_H__71009421_77F6_4E14_A2DE_DF7E31314EE1__INCLUDED_)
