#if !defined(AFX_CINVENTORY_H__74DF5D62_82DE_4074_B203_50028A25CA61__INCLUDED_)
#define AFX_CINVENTORY_H__74DF5D62_82DE_4074_B203_50028A25CA61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cInventory.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cInventory dialog

#include	"windowExtendAPI.h"

enum
{
	eVS_INVENTORY,
	eVS_EQUIPMENT,
	eVS_BANK,
};

class	CAvatarManager;
class	CAvatarData;

class cItem;
class cInventory : public CDialog
{
// Construction
public:
	cListView		m_aList[2];
	int				m_iViewSlot;
	int				m_iSelectAvatar,m_iSelectWorld;

	CAvatarManager	*m_lpAM;
	CAvatarData		*m_lpSelectPlayer;

	void			init(CAvatarManager	*_lpEditAvatar,int _iSelectAvatar,int _iSelectWorld);
	void			updateList();
	void			setItemInfoToList(cListView *_lpListView,int _iIndex,cITEM *_lpItem);
	void			updateInventoryList();
	void			updateEquipmentList();
	void			updateBankList();
	void			exportItemList(int _iBaseItem,DWORD _dwSerial);
	cInventory(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cInventory)
	enum { IDD = IDD_INVENTORY_INFO };
	CComboBox	m_cbPlayerList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cInventory)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cInventory)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnFind();
	afx_msg void OnMake();
	afx_msg void OnExport();
	afx_msg void OnRemoveByCode();
	afx_msg void OnGetItemCode();
	afx_msg void OnAddGold();
	afx_msg void OnRemoveGold();
	afx_msg void OnBank();
	afx_msg void OnInventory();
	afx_msg void OnSelchangePlayerList();
	afx_msg void OnRenewSelectItemPeriod();
	afx_msg void OnClickItemList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickItemList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd2();
	afx_msg void OnGetItemCode2();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL OnAddByEventList(char* _ItemScript ,int _InsertBank);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CINVENTORY_H__74DF5D62_82DE_4074_B203_50028A25CA61__INCLUDED_)
