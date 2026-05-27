
// RSS_AgentDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class	cFTPClient;
class	CSharedMemory;

// CRSS_AgentDlg 대화 상자
class CRSS_AgentDlg : public CDialog
{
// 생성입니다.
public:
	cFTPClient		*m_pFTPClient;
	CSharedMemory	*m_pMemoryDLL;
	char			*m_pMessData;
	BOOL			m_bIsTrayIcon;

	TCHAR			m_strDllFile[MAX_PATH],m_strServerFile[MAX_PATH];
	CRITICAL_SECTION	m_stMainMsgCS;

	CRSS_AgentDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
// 	virtual	~CRSS_AgentDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RSS_AGENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	void __cdecl PrintMainMessage(TCHAR * _strMsg, ...);

// 구현입니다.
protected:
	HICON		m_hIcon;
	CListBox	m_ctrlLog;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBeginServer();
	virtual BOOL DestroyWindow();

	BOOL	launchServer(TCHAR *_lpstrServerFile);
	void	UploadDLL();
	void	getTempFileName(TCHAR* _lpstrTempFileName,int _iBuffSize);

	afx_msg void OnBnClickedForDebug();
	BOOL	mountDLL(TCHAR *_lpstrDLLFile);
	BOOL	connectWithFTP();
	BOOL	getIniInfo();
	void	RegistTrayIcon();
	HANDLE	m_hMutex;

	afx_msg LRESULT	TrayIconMsg(WPARAM wParam, LPARAM lParam);

	afx_msg void	OnSysCommand(UINT nID, LPARAM lParam);
};