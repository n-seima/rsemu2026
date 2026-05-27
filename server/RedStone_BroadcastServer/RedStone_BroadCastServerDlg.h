// RedStone_BroadCastServerDlg.h : header file
//

#if !defined(AFX_REDSTONE_BROADCASTSERVERDLG_H__EF21699D_7D70_4CA3_B208_A049AF413DC2__INCLUDED_)
#define AFX_REDSTONE_BROADCASTSERVERDLG_H__EF21699D_7D70_4CA3_B208_A049AF413DC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cSOCKET.h"
#include "cScript.h"
#include "DebuggingDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CRedStone_BroadCastServerDlg dialog
class CRedStone_BroadCastServerDlg : public CDialog
{
// Construction
public:
	CRedStone_BroadCastServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRedStone_BroadCastServerDlg)
	enum { IDD = IDD_REDSTONE_BROADCASTSERVER_DIALOG };
	CStatic	m_ctlInWorldName;
	CEdit	m_ctlUserDebugInfo;
	CButton	m_btnStartServer;
	CButton	m_btnReadConfig;
	CListBox	m_ctlMainMsg;
	CEdit	m_ctlGuildListUpdatePeriod;
	CEdit	m_ctlGuildListImmediatelyUpdate;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRedStone_BroadCastServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRedStone_BroadCastServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnReloadConfig();
	afx_msg void OnStartServer();
	afx_msg void OnDebugging();
	virtual void OnOK();
	afx_msg void OnGuildListUpdatePeriod();
	afx_msg void OnGuildListImmediatelyUpdateChange();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	char		m_strMainPath[dPATH_LENGTH];//	main path of this server
	int			m_iGuildListUpdatePeriod;	
	BOOL		m_bIsGuildListImmediatelyUpdate;	
protected:	//	member variables
	CRITICAL_SECTION	m_stMainMsgCS;

	char		m_strMyIp[dIP_SIZE];//	ip address of Community Server 
	static char		m_strWorldName[dWORLD_NAME_LENGTH];
	char		m_strWorldIp[dIP_SIZE];//	ip address of World Server 
	char		m_strAvaDbcIp[dIP_SIZE];//	ip address of Avatar DB Cache Server 
	char		m_strChatDbIp[dIP_SIZE];//	ip address of Chatting log Sql DB
	char		m_strConfigFile[dPATH_LENGTH];//	file name of config file

	char		m_strChatLogDBId[64],m_strChatLogDBPw[64],m_strChatLogDBSource[64];

	int			m_iWorldPort;	//	port number of world server
	int			m_iAvaDbcPort;	//	port number of avatar dbc server

	int			m_iWorkerThreadCount;	//	worker thread's count

	BOOL		m_isRunEndFunc;	//	if this is set '1' , this server is start yet.

protected:	//	member functions
	BOOL	initCompletionPort();
	BOOL	readConfigFile();		//	config-bc.txt를 읽는다.
	BOOL	initServer();			//	기본적인 초기화를 진행한다. (가동시 한번만..)
	BOOL	Start();				//	서버를 가동한다.
	void	End();					//	서버를 종료한다.

public:		//	pbulic members
	HANDLE		m_hCompletionPort;	//	port for iocp
	SOCKET		m_hListenSocket;	//	socket handle for listen function
	cScript		m_cScp;				//	class for reading of config files
	CDebuggingDialog	* m_cpDebuggingDlg;	//	dialog for debugging

public:		//	public inline functions
	inline char *	getMyIp()		{	return m_strMyIp;		}
	inline char *	getWorldIp()	{	return m_strWorldIp;	}
	inline char *	getAbaDbcIp()	{	return m_strAvaDbcIp;	}
	inline int		getWorldPort()	{	return m_iWorldPort;	}
	inline int		getAvaDbcPort()	{	return m_iAvaDbcPort;	}

	static char *	getWorldName();
	static void		setWorldName(char * _strWName);

public:		//	public functions
	BOOL			m_bIsEndDlg;
	void	__cdecl PrintMainMessage(char * _strMsg, ...);
};

extern	BOOL	g_bIsEndDlg;
extern	BOOL	g_isLoopWT;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REDSTONE_BROADCASTSERVERDLG_H__EF21699D_7D70_4CA3_B208_A049AF413DC2__INCLUDED_)
