#if !defined(AFX_PAGE_SUM_H__A159D921_D282_4775_9A60_258291DF4A8A__INCLUDED_)
#define AFX_PAGE_SUM_H__A159D921_D282_4775_9A60_258291DF4A8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PAGE_SUM.h : header file
//
#include "cADO_ADMIN.h"
/////////////////////////////////////////////////////////////////////////////
// CPAGE_SUM dialog

class CPAGE_SUM : public CPropertyPage
{
	DECLARE_DYNCREATE(CPAGE_SUM)
// Construction
public:
//	CPAGE_SUM(CWnd* pParent = NULL);   // standard constructor
	CPAGE_SUM();
	~CPAGE_SUM();

// Dialog Data
	//{{AFX_DATA(CPAGE_SUM)
	enum { IDD = IDD_SUM_PAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPAGE_SUM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void	GetSumPein();
	void	SetAvaList();
	void	SetAvaSave();
// Implementation
protected:
	cADO_ADMIN		DB;
	// Generated message map functions
	//{{AFX_MSG(CPAGE_SUM)
	afx_msg void OnForLevel();
	afx_msg void OnPein();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE_SUM_H__A159D921_D282_4775_9A60_258291DF4A8A__INCLUDED_)
