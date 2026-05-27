#if !defined(AFX_RENEWITEMEXPIREDATE_H__881B78D7_6F85_44F9_B2A2_B1AB39CBF8BF__INCLUDED_)
#define AFX_RENEWITEMEXPIREDATE_H__881B78D7_6F85_44F9_B2A2_B1AB39CBF8BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenewItemExpireDate.h : header file
//

#include "stdafx.H"
#include "definePacketData.H"
/////////////////////////////////////////////////////////////////////////////
// RenewItemExpireDate dialog

class RenewItemExpireDate : public CDialog
{
// Construction
public:
	UTime	m_time;
	DWORD	m_dwRenewMunute;
	WORD	m_wIsForOnlyYahoo;
	WORD	m_wIsForOnlyRedgem;

	RenewItemExpireDate(CWnd* pParent = NULL);   // standard constructor
	int		open(char *_lpstrServerName);

// Dialog Data
	//{{AFX_DATA(RenewItemExpireDate)
	enum { IDD = IDD_RENEW_ITEM_EXPIRE_TIME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RenewItemExpireDate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	char	m_strServerName[128];
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RenewItemExpireDate)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENEWITEMEXPIREDATE_H__881B78D7_6F85_44F9_B2A2_B1AB39CBF8BF__INCLUDED_)
