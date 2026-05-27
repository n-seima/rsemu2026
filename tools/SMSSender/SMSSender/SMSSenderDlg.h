#pragma once
#include "TrayIcon.h"
#include "afxcmn.h"
#include "afxwin.h"


// CSMSSenderDlg 대화 상자
class CSMSSenderDlg : public CDialog
{
private:
	cTrayIcon m_trayIcon;
	BOOL	sendSMS(TCHAR *_lpstrMessage,TCHAR *_lpstrNumber=NULL);
	DWORD	m_dwLastSendSec;
	TCHAR	m_strLogFolder[512];
	char	m_strOldText[128];

	DWORD	m_dwLastLoginServerErrorSec;

// 생성입니다.
public:
	CSMSSenderDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SMSSENDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnTrayReshow(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedTest();
	afx_msg void OnBnClickedCheckNow();
	afx_msg void OnBnClickedRepairProblem();
	afx_msg void OnBnClickedNewMember();
	afx_msg void OnBnClickedRemoveMember();

	void	fillList();
	void	saveData();
	void	loadData();

	CListCtrl m_lcMember;
	CComboBox m_cbType;
	CComboBox m_cbSender;
	afx_msg void OnEnChangeCustomMsg();
	afx_msg void OnBnClickedSendCustomMsg();
};
