#if !defined(AFX_IPCURRENTSTATUS_H__AAE65B07_D352_456E_8A96_204C3983EFD1__INCLUDED_)
#define AFX_IPCURRENTSTATUS_H__AAE65B07_D352_456E_8A96_204C3983EFD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IPCurrentStatus.h : header file
//
#include	"CItemDefine.h"
#include	"windowExtendAPI.h"

/////////////////////////////////////////////////////////////////////////////
// cIPCurrentStatus dialog

class cIPCurrentStatus : public CDialog
{
// Construction
public:
	cItemPackInfo	m_editPack;
	int				m_iSelectServer;
	cListView		m_listItem;

	cIPCurrentStatus(CWnd* pParent = NULL);   // standard constructor
	
	void			updateItemList();
// Dialog Data
	//{{AFX_DATA(cIPCurrentStatus)
	enum { IDD = IDD_CURRENT_ITEM_PACK_INFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cIPCurrentStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cIPCurrentStatus)
	virtual BOOL OnInitDialog();
	afx_msg void OnExport();
	afx_msg void OnResetCounter();
	afx_msg void OnRefresh();
	afx_msg void OnResetCurrentCounter();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPCURRENTSTATUS_H__AAE65B07_D352_456E_8A96_204C3983EFD1__INCLUDED_)
