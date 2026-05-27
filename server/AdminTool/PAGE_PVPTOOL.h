// PAGE_PVPTOOL.h: interface for the CPAGE_PVPTOOL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE_PVPTOOL_H__13DE7099_2AD1_4A0E_B261_27B7A137658E__INCLUDED_)
#define AFX_PAGE_PVPTOOL_H__13DE7099_2AD1_4A0E_B261_27B7A137658E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "windowExtendAPI.h"
#include "resource.h"

class CPAGE_PVPTOOL : public CPropertyPage
{
	DECLARE_DYNCREATE(CPAGE_PVPTOOL)
		
	int	m_iMemberCount;

	typedef enum
	{
		eCTL_INIT_WCGTEAMLIST,
		eCTL_INIT_WCGRANKING,
	}eCTL_INIT;
public:
	CPAGE_PVPTOOL();
	virtual ~CPAGE_PVPTOOL();

	void clearListControl(eCTL_INIT _what);//eCTL_INIT¶ 
// Dialog Data
	//{{AFX_DATA(CPAGE_NOTICE)
	enum { IDD = IDD_PVP_TOOL };
//	CButton	m_ctlClearWCGList;
//	CButton	m_ctlUpdateWCGList;
//	CButton	m_ctlGetWCGList;

//	CButton	m_ctlClearWCGRank;
//	CButton	m_ctlGetWCGRank;
//	CButton	m_ctlModifyWCGScore;

	cListView m_ctlListResult;	//	IDC_LIST_RESULT
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPAGE_PVPTOOL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPAGE_PVPTOOL)
	virtual BOOL OnInitDialog();

	afx_msg void OnClearWCGList(BOOL _bIsByAdd=FALSE);
	afx_msg void OnAddTeamList();
	afx_msg void OnGetWCGList();

	afx_msg void OnClearWCGRank();
	afx_msg void OnGetWCGRank();
	afx_msg void OnModifyWCGScore();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_PAGE_PVPTOOL_H__13DE7099_2AD1_4A0E_B261_27B7A137658E__INCLUDED_)
