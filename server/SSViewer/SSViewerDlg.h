// SSViewerDlg.h : header file
//

#if !defined(AFX_SSVIEWERDLG_H__9493DBA7_F585_42EF_8869_A1E17D2C9657__INCLUDED_)
#define AFX_SSVIEWERDLG_H__9493DBA7_F585_42EF_8869_A1E17D2C9657__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrayIcon.h"
#include "DlgHistory.h"
#include "TaskbarNotifier.h"
#include "cServerStatus.h"

/////////////////////////////////////////////////////////////////////////////
// CSSViewerDlg dialog

class CSSViewerDlg : public CDialog
{
private:
	cTrayIcon m_trayIcon;
	CTaskbarNotifier m_wndTaskbarNotifier;
	UINT m_wDeviceID;							// 사운드 재생관련
	stMESSAGEINFO *m_pstMessaeInfo;
	int m_iMessageCnt;

	BOOL	m_isStopAlarm;//TRUE만 일정 시간동안 알람을 사용하지 않는다. 중간의 내용은 모두 무시
	UTime	m_timeCheckHour;//시간을 기록해 뒀다가, 일정 시간 이상이 지나면.. 복귀하자.
	int		m_iRecoverMinute;//알람 중지를 해둔 후 몇분 후에 재복귀 할지를 지정하자. 
	
public:
	cDlgHistory *m_pcDlgHistory;

private:
//	void showNotice(char *_pszDate, char *_pszName, char *_pszIP, int _iErrorType, int _iParam1, char *_pszParam2);
	void showNotice(cSSInfoTable _info);
	BOOL playWaveFile(char *_pszFileName);
	void showDlgHistory();


// Construction
public:
	CSSViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSSViewerDlg)
	enum { IDD = IDD_SSVIEWER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSViewerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support	
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSSViewerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMenuitemMainExit();
	afx_msg void OnMenuitemMainHistory();
	afx_msg void OnMenuitemStop();
	afx_msg void OnMenuItemRestart();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnTrayReshow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTaskbarNotifierClicked(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSVIEWERDLG_H__9493DBA7_F585_42EF_8869_A1E17D2C9657__INCLUDED_)
