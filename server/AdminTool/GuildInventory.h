#if !defined(AFX_GUILDINVENTORY_H__D42F1045_9A19_4A36_AB70_60D0210132F0__INCLUDED_)
#define AFX_GUILDINVENTORY_H__D42F1045_9A19_4A36_AB70_60D0210132F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuildInventory.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuildInventory dialog

#include	"windowExtendAPI.h"

class	CGuildAdvanceInfo;

class CGuildInventory : public CDialog
{
// Construction
public:
	CGuildInventory(CWnd* pParent = NULL);   // standard constructor

	cListView			m_list;
	CGuildAdvanceInfo	*m_lpGuild;

	void				updateList();
	void				exportItemList(int _iBaseItem=0,DWORD _dwSerial=0);
	void				setItemInfoToList(cListView *_lpListView,int _iIndex,cITEM *_lpItem);
	void				setGuild(CGuildAdvanceInfo	*_lpGuild);

// Dialog Data
	//{{AFX_DATA(CGuildInventory)
	enum { IDD = IDD_GUILD_INVENTORY_INFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuildInventory)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGuildInventory)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnRemoveByCode();
	afx_msg void OnFind();
	afx_msg void OnAddGold();
	afx_msg void OnRemoveGold();
	afx_msg void OnExport();
	afx_msg void OnGetItemCode();
	afx_msg void OnMake();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILDINVENTORY_H__D42F1045_9A19_4A36_AB70_60D0210132F0__INCLUDED_)
