#if !defined(AFX_CMAKEITEM_H__94FC90C4_3ED7_431F_BB76_CAB01193DC24__INCLUDED_)
#define AFX_CMAKEITEM_H__94FC90C4_3ED7_431F_BB76_CAB01193DC24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cMakeItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cMakeItem dialog

#include	"CItemDefine.h"

class cMakeItem : public CDialog
{
// Construction
public:
	cMakeItem(CWnd* pParent = NULL);   // standard constructor

	cItemInPack	m_makeItem;
	int			m_iPrefix1Kind,m_iPrefix2Kind,m_iPrefix3Kind,m_iItemKind;

	void		fillPrefixCombo(int _iIndex);
	void		fillItemCombo();
	void		fillItemData();
	void		getItemInfo(cItemInPack *_lpItem);
// Dialog Data
	//{{AFX_DATA(cMakeItem)
	enum { IDD = IDD_MAKE_ITEM };
	CEdit	m_editCount;
	CEdit	m_editPrefix3_2;
	CEdit	m_editPrefix2_2;
	CEdit	m_editPrefix1_2;
	CStatic	m_stPrefix3Value2;
	CStatic	m_stPrefix2Value2;
	CStatic	m_stPrefix1Value2;
	CEdit	m_editItemCode;
	CEdit	m_editFrequency;
	CEdit	m_editLimitCount;
	CEdit	m_editPrefix3;
	CEdit	m_editPrefix2;
	CEdit	m_editPrefix1;
	CEdit	m_editOption2;
	CEdit	m_editOption1;
	CStatic	m_stCount;
	CStatic	m_stPrefix3Value;
	CStatic	m_stPrefix2Value;
	CStatic	m_stPrefix1Value;
	CStatic	m_stOption2Value;
	CStatic	m_stOption1Value;
	CComboBox	m_cbItemKind;
	CComboBox	m_cbPrefix3Kind;
	CComboBox	m_cbPrefix2Kind;
	CComboBox	m_cbPrefix1Kind;
	CButton	m_boxIsIncludePremiumItem;
	CButton	m_boxIsIncludeQuestItem;
	CComboBox	m_cbItemList;
	CComboBox	m_cbPrefix3;
	CComboBox	m_cbPrefix2;
	CComboBox	m_cbPrefix1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cMakeItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cMakeItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangePrefix1Kind();
	afx_msg void OnSelchangePrefix2Kind();
	afx_msg void OnSelchangePrefix3Kind();
	afx_msg void OnSelchangeItemKind();
	afx_msg void OnSelchangeItemList();
	afx_msg void OnSelchangePrefix1();
	afx_msg void OnSelchangePrefix2();
	afx_msg void OnSelchangePrefix3();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
	afx_msg void OnSetCategory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMAKEITEM_H__94FC90C4_3ED7_431F_BB76_CAB01193DC24__INCLUDED_)
