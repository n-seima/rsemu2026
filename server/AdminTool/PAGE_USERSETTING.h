#if !defined(AFX_PAGE_USERSETTING_H__2B763789_C517_41FC_9564_2A6FA6648459__INCLUDED_)
#define AFX_PAGE_USERSETTING_H__2B763789_C517_41FC_9564_2A6FA6648459__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PAGE_USERSETTING.h : header file
//
#include "cADO_ADMIN.h"
#include "CFTPclient.h"

/////////////////////////////////////////////////////////////////////////////
// CPAGE_USERSETTING dialog

class CPAGE_USERSETTING : public CPropertyPage
{
	DECLARE_DYNCREATE(CPAGE_USERSETTING)

// Construction
public:
	CPAGE_USERSETTING();
	~CPAGE_USERSETTING();

// Dialog Data
	//{{AFX_DATA(CPAGE_USERSETTING)
	enum { IDD = IDD_USERSETTING };
	CButton	m_ctlIsCheckUsingAccount;
	CButton	m_ctlSetTester;
	CEdit	m_ctlSearchId;
	CEdit	m_ctlAvaRecNewName;
	CEdit	m_ctlAvaRecName;
	CEdit	m_ctlAvaRecId;
	CEdit	m_ctlChgNewName;
	CEdit	m_ctlChgOldName;
	CEdit	m_ctlChgNameID;
	CEdit	m_ctlBadCharName;
	CEdit	m_ctlBadID;
	CEdit	m_ctlRecIDNew;
	CEdit	m_ctlBadWhy;
	CComboBox	m_ctlOperLevel;
	CEdit	m_ctlRecID;
	CEdit	m_ctlBadDate;
	CEdit	m_ctlBadName;
	CEdit	m_ctlAdminName;
	CString	m_strWhy;
	CString	m_strBlockDateLengthCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPAGE_USERSETTING)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	cADO_ADMIN		DB;
	nsFTP::CFTPClient		ftp;	//	211.189.122.123
	void			OnChangenameSyc(int serverindex,char *strId, char *newName);
	void			DeleteList(int iSelectedServerCur, char * strId);
	// Generated message map functions
	//{{AFX_MSG(CPAGE_USERSETTING)
	afx_msg void OnSetAdmin();
	afx_msg void OnAdminCount();
	afx_msg void OnBaduser();
	afx_msg void OnRecover();
	virtual BOOL OnInitDialog();
	afx_msg void OnDelBadid();
	afx_msg void OnDelBadname();
	afx_msg void OnChangeName();
	afx_msg void OnChangenameSyc();
	afx_msg void OnAvaRecover();
	afx_msg void OnSearchId();
	afx_msg void OnListDelete();
	afx_msg void OnSaveUpdate();
	afx_msg void OnSaveUpdateAll();
	afx_msg void OnCheckBaduser();
	afx_msg void OnGuildCheck();
	afx_msg void OnChangeAvarecoverName();
	afx_msg void OnChangeCnameName();
	afx_msg void OnUpdatelist();
	afx_msg void OnIdCheck();
	afx_msg void OnEmer();
	afx_msg void OnCheckOperlevel();
	afx_msg void OnSetTester();
	afx_msg void OnResetOper();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE_USERSETTING_H__2B763789_C517_41FC_9564_2A6FA6648459__INCLUDED_)
