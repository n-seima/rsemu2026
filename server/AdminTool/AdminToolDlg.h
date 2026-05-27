// AdminToolDlg.h : header file
//

#if !defined(AFX_ADMINTOOLDLG_H__B97352C2_3953_4B25_8B43_E7D6A743268A__INCLUDED_)
#define AFX_ADMINTOOLDLG_H__B97352C2_3953_4B25_8B43_E7D6A743268A__INCLUDED_

#include "cSOCKET.h"
#include "cScript.h"
#include "StdAfx.h"
#include "PAGE_NOTICE.h"
#include "PAGE_USERSETTING.h"
#include "CharacterInfo.h"
#include "WorldScore.h"
#include "SEARCHPAGE.h"
#include "TentativeGuildManager.h"
#include "EventManager.h"
#include "ServerTool.h"
#include "itemPackList.h"
#include "guildInfo.h"
#include "carrotShop.h"
#include "PAGE_PVPTOOL.h"
#include "PollDlg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAdminToolDlg dialog

class CAdminToolDlg : public CDialog
{
public:
// Construction
public:
	CAdminToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAdminToolDlg)
	enum { IDD = IDD_ADMINTOOL_DIALOG };
	CButton	m_ctlConnect;
	CEdit	m_ctlInputMsg;
	CTreeCtrl	m_ctlServerTree;
	CImageList	*m_pCtlImageList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminToolDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CPropertySheet		m_Sheet;

	CPAGE_NOTICE			m_PageNotice;
	CPAGE_USERSETTING		m_PageUserSetting;
	CPAGE_PVPTOOL			m_PagePvPTool;
	CWorldScore				m_PageWorldScore;
	CCharacterInfo			m_pageCharacterInfo;
	CServerTool				m_pageServerTool;
	cItemPackList			m_pageItemPackList;
	cCarrotShop				m_pageCarrotShop;
	CSEARCHPAGE				m_pageSearch;
	CTentativeGuildManager	m_pageTentativeGuild;
	CEventManager			m_pageEventManager;
	cGuildInfo				m_pageGuildInfo;
	cPollDlg				m_pagePollDlg;

	BOOL					tryConnectServer(int _serverIndex);
	BOOL					disConnectServer(int _serverIndex);
public:
	void					connectWorld(int _iWorld);

	// Generated message map functions
	//{{AFX_MSG(CAdminToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT, CWnd*, BOOL);
	afx_msg void OnSelchangedServerTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnConnect();
	afx_msg void OnInfoUpdate();
	afx_msg void OnRclickServerTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickServerTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern	CAdminToolDlg	*g_lpAdminDlg;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINTOOLDLG_H__B97352C2_3953_4B25_8B43_E7D6A743268A__INCLUDED_)
