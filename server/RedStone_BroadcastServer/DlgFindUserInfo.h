#if !defined(AFX_DLGFINDUSERINFO_H__894617B8_C8B9_44CD_B9CD_C5BFD8854621__INCLUDED_)
#define AFX_DLGFINDUSERINFO_H__894617B8_C8B9_44CD_B9CD_C5BFD8854621__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFindUserInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFindUserInfo dialog

//	: 특정 유저의 정보 검색
//
/////////////////////////////////////////////////////////////////////////////

#include <deque>

class cGSvrInfo
{
public:
	int iSerial;
	char ip[16];
	cGSvrInfo(int _iSerial, char * _ip)
	{
		iSerial = _iSerial;
		strcpy(ip,_ip);
	}
};

class CDlgFindUserInfo : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgFindUserInfo)

// Construction
public:
	CDlgFindUserInfo();
	~CDlgFindUserInfo();

	std::deque<cGSvrInfo>		m_dequeServerList;
// Dialog Data
	//{{AFX_DATA(CDlgFindUserInfo)
	enum { IDD = IDD_FIND_USERINFO };
	CEdit	m_ctlSendEdit;
	CEdit	m_ctlRecvEdit;
	CComboBox	m_ctlServerList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgFindUserInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgFindUserInfo)
	afx_msg void OnOutputAllInfo();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnKillThisServer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFINDUSERINFO_H__894617B8_C8B9_44CD_B9CD_C5BFD8854621__INCLUDED_)
