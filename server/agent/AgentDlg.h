// AgentDlg.h : header file
//

#if !defined(AFX_AGENTDLG_H__FD754A5A_4C05_4CA4_A89A_CCC877E2D2FC__INCLUDED_)
#define AFX_AGENTDLG_H__FD754A5A_4C05_4CA4_A89A_CCC877E2D2FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"resource.h"
#include	"cSOCKET.h"
#include	"server_type_define.h"
#include	"cPACKET_AGENT.h"

class CAgentDlgAutoProxy;
class cServerInfo;

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg dialog

#define	dWORKERTHREAD_COUNT	4

class CAgentDlg : public CDialog
{
	DECLARE_DYNAMIC(CAgentDlg);
	friend class CAgentDlgAutoProxy;

	SOCKET		m_hListenSocket;
	HANDLE		m_hCompletionPort;
	char		m_strMyIp[64];
	char		m_strExeFile[512];
	int			m_iServerType;

	BOOL		initCompletionPort();
	BOOL		beginThread();

	cSOCKET		m_sockLogInServer;

	HANDLE		m_hLiveThread;
	HANDLE		m_hLogInServerThread,m_hReconnectThread;
	HANDLE		m_ahWorkerThread[dWORKERTHREAD_COUNT];

	CString		m_strLogInServerIP;

	void		loadConfig();
	void		saveConfig();

// Construction
public:
	CAgentDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CAgentDlg();

	BOOL				m_isLoopWT;

	void	__cdecl		PrintMainMessage(char * _strMsg, ...);
	inline	HANDLE		getCompletionPort()
	{
		return	m_hCompletionPort;
	}
	inline	SOCKET		getListenSocket()
	{
		return	m_hListenSocket;
	}
	inline	cSOCKET*	getLoginServerSocket()
	{
		return	&m_sockLogInServer;
	}
	void				DisUser(int	_iIndex,char *_lpStrReason);
	BOOL				RecvPacket(DWORD _dwIdx);
	BOOL				ProcessRecvPacket(DWORD _dwIdx);
	BOOL				sendPacket(DWORD _dwIdx);
	BOOL				sendPacketAdd(DWORD _dwIdx);
	BOOL				connectWithLoginServer();	//	로그인 서버와 연결
	
	BOOL				getInfo(cAgentServerInfo	*_lpInfo);
	void				operateLoginClient(CR_STATUS *_lpPacket);
	BOOL				sendCurrentStatus();
	void				fillTree();
	void				updateTreeItem(cServerInfo *_lpInfo);

	void				killServer();
	void				beginServer();
	void				endServer();
// Dialog Data
	//{{AFX_DATA(CAgentDlg)
	enum { IDD = IDD_AGENT_DIALOG };
	CTreeCtrl	m_treeServerList;
	CComboBox	m_cbServerType;
	CListBox	m_ctlMainMsg;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	CRITICAL_SECTION	m_stMainMsgCS;
	CRITICAL_SECTION	m_csRemoveUser;

// Implementation
protected:
	CAgentDlgAutoProxy* m_pAutoProxy;
	HICON				m_hIcon;
	HANDLE				m_hAcceptThread;

	BOOL				initNetwork();		//	기본적인 초기화를 진행한다. (가동시 한번만..)

	BOOL				CanExit();

	// Generated message map functions
	//{{AFX_MSG(CAgentDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnStart();
	afx_msg void OnSelchangeServerType();
	afx_msg void OnChangeLoginServerIp();
	afx_msg void OnKill();
	afx_msg void OnBeginServer();
	afx_msg void OnEndServer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AGENTDLG_H__FD754A5A_4C05_4CA4_A89A_CCC877E2D2FC__INCLUDED_)
