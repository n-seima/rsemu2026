// StressTestClientDlg.h : header file
//

#if !defined(AFX_STRESSTESTCLIENTDLG_H__21DD5271_E5F5_4597_8B48_232060EBFEAC__INCLUDED_)
#define AFX_STRESSTESTCLIENTDLG_H__21DD5271_E5F5_4597_8B48_232060EBFEAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStressTestClientDlg dialog

//	━━stress test client
#define dCG_CONNECT_STRESS_CLIENT				0x2000
#define dCG_STRESSDATA							0x2001
#define dCG_MODIFY_STRESS_CLIENT				0x2002
#define dCG_REGIST_STRESS_CLIENT				0x2003

#define dSG_STRESSCLIENT						0x2200
#define dSG_STRESS_SERVER_INFO					0x2201

class	cMSG_BASE_TYPE
{
public:
	WORD	wSize,				//	패킷 사이즈
			wType,				//	패킷 타입
			wProtectType;		//	보안 타입

	inline	void	set(WORD size,WORD type,WORD protectType=0)	{wSize	=	size,wType	=	type,wProtectType=protectType;}
};

struct	CG_CONNECT_STRESS_CLIENT
{
	cMSG_BASE_TYPE		base;

	int					iUniqueSerial;
};

struct	CG_REGIST_STRESS_CLIENT
{
	cMSG_BASE_TYPE		base;

	int					iUniqueSerial;
};

struct	CG_MODIFY_STRESS_CLIENT
{
	cMSG_BASE_TYPE		base;
};

struct	CG_STRESSCLIENT
{
	cMSG_BASE_TYPE		base;

	WORD				wUserCount;
	WORD				wSendSize;
};

struct	CG_STRESSDATA
{
	cMSG_BASE_TYPE		base;
};

struct	SG_STRESSCLIENT
{
	cMSG_BASE_TYPE		base;
};

struct	SG_STRESS_SERVER_INFO
{
	cMSG_BASE_TYPE		base;

	int					iConnectUserCount;
	int					iUpdateTime;
};


class CStressTestClientDlg : public CDialog
{
// Construction
public:
	CStressTestClientDlg(CWnd* pParent = NULL);	// standard constructor

	int		m_iSendCount,m_iSendByte;
	int		m_iOldUserCount,m_iUserCount;
	int		m_iConnectUserCount;
	int		m_iAverageUpdateTime,m_iUpdateTimeCounter,m_iUpdateTimeSum;

	int		m_iSendCountPer10Second;
	int		m_iSendSizePer10Second;

	int		m_iTickCounter;
	BOOL	m_bIsRegistClinet;
	

// Dialog Data
	//{{AFX_DATA(CStressTestClientDlg)
	enum { IDD = IDD_STRESSTESTCLIENT_DIALOG };
	CStatic	m_staticUpdateSpeed;
	CStatic	m_editConnectUserCount;
	CEdit	m_ctlPort;
	CEdit	m_ctlIP;
	CEdit	m_ctlDataAmount;
	CEdit	m_ctlUserCount;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStressTestClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CStressTestClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL		m_isConnecting;
	DWORD		m_dwSendDataAmount;
	DWORD		m_dwRecvDataAmount;
	DWORD		m_dwSendDataAverage_Sec;
	DWORD		m_dwRecvDataAverage_Sec;

	BOOL		ConnectServer();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRESSTESTCLIENTDLG_H__21DD5271_E5F5_4597_8B48_232060EBFEAC__INCLUDED_)
