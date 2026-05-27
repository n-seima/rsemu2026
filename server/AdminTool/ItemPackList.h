#if !defined(AFX_ITEMPACKLIST_H__E818C18C_155F_4522_9AAD_9439D81A50FF__INCLUDED_)
#define AFX_ITEMPACKLIST_H__E818C18C_155F_4522_9AAD_9439D81A50FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// itemPackList.h : header file
//
#include	"windowExtendAPI.h"
#include	"CItemDefine.h"
#include	"cMakeItem.h"
#include	"IPCurrentStatus.h"

/////////////////////////////////////////////////////////////////////////////
// cItemPackList dialog

class cItemPackList : public CPropertyPage
{
// Construction
public:
	cItemPackList(CWnd* pParent = NULL);   // standard constructor

	cIPCurrentStatus	m_currentInfo;

	int				m_iPackCount,m_iSelectedPack,m_iWantPack,m_iWorldOfAskItemPackList;
	cListView		m_listItemPack,m_listItem;
	cItemPackInfo	m_aPacks[dMAX_ITEM_PACK_COUNT];
	cItemPackInfo	m_editPack;
	cMakeItem		m_dlgMakeItem;
	BOOL			m_bIsWantCurrentInfo,m_bIsReceiveData,m_bIsWantAllItemPackList;
	BOOL			m_bIsLostVirgin;
	

	void			lostVirgin()	{m_bIsLostVirgin	=	TRUE;}
	void			setVirgin()		{m_bIsLostVirgin	=	FALSE;}
	BOOL			isVirgin()		{return	1-m_bIsLostVirgin;}

	BOOL			check(int _iLimitLevel,BOOL _bIsCheckReceivedList=TRUE);
	BOOL			isConnected();
	void			reset();
	void			receiveResetDropPackItemCount(int _iPackSerial);
	void			sendItemPackInfo(int _iWorldIndex);
	void			updateList(int _iSelect=-1,BOOL _bIsWantSelect=TRUE);
	void			updateItemList();
	void			savePackList();
	void			fillLinkItemCombo();
	void			askItemPackList(int _iPackIndex);
// Dialog Data
	//{{AFX_DATA(cItemPackList)
	enum { IDD = IDD_ITEM_PACK_LIST };
	CComboBox	m_cbLinkItem;
	CListCtrl	m_itemPackLlist;
	CEdit	m_ebPackName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cItemPackList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cItemPackList)
	virtual BOOL OnInitDialog();
	afx_msg void OnCreate();
	afx_msg void OnAskPackList();
	afx_msg void OnRemovePack();
	afx_msg void OnChangeName();
	afx_msg void OnItemchangedPackList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddItem();
	afx_msg void OnApplyList();
	afx_msg void OnRemoveItem();
	afx_msg void OnModifyData();
	afx_msg void OnResetContents();
	afx_msg void OnAskSelectServerPackInfo();
	afx_msg void OnEditItem();
	afx_msg void OnExport();
	afx_msg void OnImport();
	afx_msg void OnSelchangeLinkItem();
	afx_msg void OnSaveToFile();
	afx_msg void OnLoadFromFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMPACKLIST_H__E818C18C_155F_4522_9AAD_9439D81A50FF__INCLUDED_)
