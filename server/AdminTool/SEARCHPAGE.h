#if !defined(AFX_SEARCHPAGE_H__EB67230B_042D_43CE_95F3_8A13538E8E09__INCLUDED_)
#define AFX_SEARCHPAGE_H__EB67230B_042D_43CE_95F3_8A13538E8E09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SEARCHPAGE.h : header file
//

#include "AdminLogCode.h"
#include "LinkedList.h"
/////////////////////////////////////////////////////////////////////////////
// CSEARCHPAGE dialog

class CSEARCHPAGE : public CPropertyPage
{
	DECLARE_DYNCREATE(CSEARCHPAGE)

// Construction
public:
	CSEARCHPAGE();
	~CSEARCHPAGE();

	CLinkedList		findList;

// Dialog Data
	//{{AFX_DATA(CSEARCHPAGE)
	enum { IDD = IDD_SEARCH };
	CButton	m_ctlSearch;
	CEdit	m_editId;
	CListBox	m_listResult;
	CComboBox	m_cbY1;
	CComboBox	m_cbY0;
	CComboBox	m_cbM1;
	CComboBox	m_cbM0;
	CComboBox	m_cbH1;
	CComboBox	m_cbH0;
	CComboBox	m_cbD1;
	CComboBox	m_cbD0;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSEARCHPAGE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSEARCHPAGE)
	virtual BOOL OnInitDialog();
	afx_msg void OnSearch();
	afx_msg void OnClear();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHPAGE_H__EB67230B_042D_43CE_95F3_8A13538E8E09__INCLUDED_)
