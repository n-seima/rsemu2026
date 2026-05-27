#if !defined(AFX_CMINIPETLIST_H__926AAEF5_2578_4AA2_967C_6E93A7C58D97__INCLUDED_)
#define AFX_CMINIPETLIST_H__926AAEF5_2578_4AA2_967C_6E93A7C58D97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cMiniPetList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cMiniPetList dialog

#include	"miniPetCommon.h"
#include	"windowExtendAPI.h"

class	CAvatarData;

class cMiniPetList : public CDialog
{
// Construction
	cMiniPetCommon	m_aMiniPet[c_iMiniPetCount];
	cMiniPetCommon	m_miniPet;
	CAvatarData		*m_lpPlayer;
public:
	CTimeInfo		m_reversalFramevalidate,m_editReversalFrameValidate;
	cMiniPetList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cMiniPetList)
	enum { IDD = IDD_MINIPET_LIST };
	CComboBox	m_cbMiniPetType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cMiniPetList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	cListView	m_listMiniPet;
	BOOL		m_bIsVirgin;

	void		updateList();
	void		resetEditMiniPet();
	void		fillEditMiniPetInfo();
	BOOL		getEditMiniPetInfo();
	void		reviseMiniPetInfo();

public:
	cMiniPetCommon*	getMiniPet()
	{
		return	m_aMiniPet;
	}
	void		setMiniPet(CAvatarData *_lpPlayer,cMiniPetCommon* _lpMiniPet,int _iYear,int _iMonth,int _iDay,int _iHour,int _iMinute);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cMiniPetList)
	afx_msg void OnRemove();
	afx_msg void OnAdd();
	afx_msg void OnAddByCode();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnReset();
	afx_msg void OnUpdate();
	afx_msg void OnGetSelectedPetInfo();
	afx_msg void OnFillDateByCurrentTime();
	afx_msg void OnGetCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMINIPETLIST_H__926AAEF5_2578_4AA2_967C_6E93A7C58D97__INCLUDED_)
