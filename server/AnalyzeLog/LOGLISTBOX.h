#if !defined(AFX_LOGLISTBOX_H__E595622A_DEBA_4BE9_B0A6_242D6F8BE75C__INCLUDED_)
#define AFX_LOGLISTBOX_H__E595622A_DEBA_4BE9_B0A6_242D6F8BE75C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LOGLISTBOX.h : header file
//
#include "CBMPMENU.h"

/////////////////////////////////////////////////////////////////////////////
// CLOGLISTBOX window

class CLOGLISTBOX : public CListBox
{
// Construction
public:
	CLOGLISTBOX();

// Attributes
public:
//	CBmpMenu	menuList;
	CMenu		menu;
	HACCEL		accel;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLOGLISTBOX)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLOGLISTBOX();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLOGLISTBOX)
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGLISTBOX_H__E595622A_DEBA_4BE9_B0A6_242D6F8BE75C__INCLUDED_)
