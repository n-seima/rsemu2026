#if !defined(AFX_CARROTSHOP_H__607AEA93_BC59_4F54_BB71_0EF8B79EE2F9__INCLUDED_)
#define AFX_CARROTSHOP_H__607AEA93_BC59_4F54_BB71_0EF8B79EE2F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// carrotShop.h : header file
//

#include	"windowExtendAPI.h"
#include	"CItemDefine.h"
#include	"cMakeItem.h"
#include	"IPCurrentStatus.h"
#include	"carrotShopDefine.h"

/////////////////////////////////////////////////////////////////////////////
// cCarrotShop dialog

class cCarrotShopItem	:	public cCarrotShopItemDefine
{
public:
};

class cCarrotShopCategoryInfo	:	public	cCarrotShopCategoryInfoDefine
{
public:
	void						reset();
	cCarrotShopItem	*			getItem(int _iIndex);
};

class cCarrotShopInfo	:	public	cCarrotShopDefine
{
public:
	void						reset();

	cCarrotShopCategoryInfo*	getByName(char *_lpstrName);
	cCarrotShopCategoryInfo*	getCategory(int _iIndex);
	int							getValidCategoryIndex();
	BOOL						isFull();
};

class cCarrotShop : public CPropertyPage
{
	DECLARE_DYNCREATE(cCarrotShop)

// Construction
public:
	cCarrotShop();
	~cCarrotShop();

	BOOL					m_bIsReceiveData,m_bIsWaitToReceiveData;
	int						m_iSelectedCategory,m_iWantCategoty,m_iWorldOfAskCarrotShopInfo,m_iSelectedItem;
	cListView				m_listCategory,m_listCarrotShopItem;
	cCarrotShopInfo			m_shopInfo;
	cCarrotShopCategoryInfo	m_editCategory,m_clipCategory;
	BOOL					m_bIsVirgin;

	void					lostVirgin();
	void					setVirgin();
	BOOL					isVirgin();
	BOOL					isLostVirgin();
	BOOL					check(int _iLimitLevel,BOOL _bIsCheckReceivedList=TRUE);
	void					receiveCarrotShopInfo(int _iIndex,int _iCount,int _iStatus,cCarrotShopCategoryInfo *_lpData);

	void					updateItemList();
	void					updateCategoryList();
// Dialog Data
	//{{AFX_DATA(cCarrotShop)
	enum { IDD = IDD_CARROT_SHOP };
	CEdit	m_editPrice;
	CEdit	m_editCount;
	CComboBox	m_cbPremiumItem;
	CEdit	m_editCategoryName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(cCarrotShop)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(cCarrotShop)
	virtual BOOL OnInitDialog();
	afx_msg void OnCreate();
	afx_msg void OnAddCarrotItem();
	afx_msg void OnRemoveCarrotItem();
	afx_msg void OnEditCarrotItem();
	afx_msg void OnResetCarrotItemContents();
	afx_msg void OnApplyCarrotItem();
	afx_msg void OnItemchangedCategory(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRemovePack();
	afx_msg void OnChangeName();
	afx_msg void OnMoveUpSelectCategory();
	afx_msg void OnMoveDownSelectCategory();
	afx_msg void OnMoveUpSelectItem();
	afx_msg void OnMoveDownSelectItem();
	afx_msg void OnSaveToFile();
	afx_msg void OnLoadFromFile();
	afx_msg void OnPasteCategory();
	afx_msg void OnCopyCategory();
	afx_msg void OnAskCarrotInfo();
	afx_msg void OnModifyCarrotShopData();
	afx_msg void OnItemchangedCarrotItemList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnApplyNow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARROTSHOP_H__607AEA93_BC59_4F54_BB71_0EF8B79EE2F9__INCLUDED_)
