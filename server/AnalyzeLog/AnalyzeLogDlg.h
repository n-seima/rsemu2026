// AnalyzeLogDlg.h : header file
//

#if !defined(AFX_ANALYZELOGDLG_H__DA2E6960_E09A_40F1_8460_40E111CF287E__INCLUDED_)
#define AFX_ANALYZELOGDLG_H__DA2E6960_E09A_40F1_8460_40E111CF287E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAnalyzeLogDlg dialog
#include "StdAfx.h"
#include "cPACKET_LOGSERVER.h"
#include "cScript.h"
#include "LOGLISTBOX.h"
#include "Tracer.h"
#include "resource.h"

#define	dMAX_READ_LOG_COUNT	100000

class CLinkedList;
class CProcessInfoDlg;
struct	CNode;

class	cFilterData
{
public:
	int			m_iFilter;
	CTimeInfo	m_dateBegin,m_dateEnd;
	int			m_iServer;
	char		m_strIP[dIP_LENGTH];
	char		m_strValue[64];
	char		m_strID[dID_LENGTH];
	char		m_strName[dNAME_LENGTH];
	DWORD		m_dwItem;
	DWORD		m_dwItemSerial;
	BOOL		m_bIsAndCondition;
	BOOL		m_bIsIgnoreSaveLog;
	BOOL		m_bIsAddSaveLog;

	BOOL		m_bIsOnlyUniqueItem;
	BOOL		m_bIsOnlyDXItem;
	BOOL		m_bIsOnlySetItem;
	BOOL		m_bIsOnlyEternalItem;
	int			m_iEternalGrade;
};

class CLogNode
{
public:
	DWORD		m_dwTimeValue;	//	배열에서의 위치를 나타내며 -1 이면 빈 슬롯이다.
	CNode		*m_lpNode;
};

class CAnalyzeLogDlg : public CDialog
{
// Construction
public:
	CAnalyzeLogDlg(CWnd* pParent = NULL);	// standard constructor

	~CAnalyzeLogDlg();

	int			m_iLogNodeCountForSort;
	CLogNode	*m_aLogNodeForSort;

	void	__cdecl	MSGBOX(char * title,char * txt, ...);
	void		LoadLogFile(char *fn);
	CLOGLISTBOX	m_ctlLogData;
	UINT		m_uWaveDeviceId;
	int			m_iSelectedDate;

	cFilterData	m_filter;

	cScript		scp;

	char		m_strEditFilterCode[512];

	char		ipLogAddr[16];
	char		strMainDir[512];
	char		m_strServerLogDir[1024];
	char		strSaveLogDir[1024];
	char		m_strLocalLogDir[1024];
	char		strSaveDir[1024];
	char		m_strExportLogFolder[1024];

	char		strFtpId[64];
	char		strFtpPw[64];
	DWORD		FtpPort;
	DWORD		startdate;
	BOOL		m_bIsSerchSpecialCondition050720;

	CLinkedList	* allData;
	CLinkedList	* searchData;

	//	상태값  
	DWORD		m_dwCurListCount,m_dwSearchListCount;
	BOOL		isSearchList;		//	리스트에서 검색했다면 , 두번 검색은 불가능하다.

	//	Log
	DebugTracer	* _Tracer;

	BOOL		m_bIsCancelSearch;
	CProcessInfoDlg	*m_pProgressDlg;

	BOOL		searchSettingDuelLog();
	void		searchLog(BOOL _bIsExportFilter=FALSE);
	void		applyFilter();

	void		loadCofig();
	void		saveCofig();
	char*		getEditFilterCode();

// Dialog Data
	//{{AFX_DATA(CAnalyzeLogDlg)
	enum { IDD = IDD_ANALYZELOG_DIALOG };
	CStatic	m_stComment;
	CButton	m_ctlIsServer;
	CEdit	m_editValue;
	CButton	m_ctlDnButton;
	CButton	m_checkAnd;
	CComboBox	m_comboItemList;
	CEdit	m_ctlIP;
	CComboBox	m_ctlMin2;
	CComboBox	m_ctlMin1;
	CButton	m_ctlBack;
	CComboBox	m_ctlCompare;
	CEdit	m_ctlName;
	CEdit	m_ctlSerial;
	CEdit	m_ctlId;
	CComboBox	m_ctlServerList;
	CMonthCalCtrl	m_ctlTime;
	CComboBox	m_ctlCB_Job;
	CComboBox	m_ctlHour2;
	CComboBox	m_ctlHour1;
	CComboBox	m_ctlDD2;
	CComboBox	m_ctlDD1;
	CComboBox	m_ctlMM2;
	CComboBox	m_ctlMM1;
	CComboBox	m_ctlYY2;
	CComboBox	m_ctlYY1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalyzeLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAnalyzeLogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeMm1();
	afx_msg void OnSelchangeMm2();
	afx_msg void OnLogSearch();
	afx_msg void OnLogDown();
	afx_msg void OnLogLoad();
	afx_msg void OnLogSearchFile();
	afx_msg void OnLogBack();
	afx_msg void OnLogSave();
	afx_msg void OnLogItemSave();
	afx_msg void OnDestroy();
	afx_msg void OnLogAddFile();
	afx_msg void OnLogopen();
	afx_msg void OnLogDown2();
	afx_msg void OnFixfile();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeYy1();
	afx_msg void OnSelchangeYy2();
	afx_msg void OnSpLogSearch();
	afx_msg void OnDatafolder();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnGetdaystateLogCalendar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeLogCalendar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectLogCalendar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnisDownLoadServer();
	afx_msg void OnNUXtoLOG();
	afx_msg void OnFullDay();

	void		setLastestDate(int _iDay);

	afx_msg void On1Day();
	afx_msg void On1Week();
	afx_msg void On2Week();
	afx_msg void On1Month();
	afx_msg void On6Month();

	afx_msg void OnSaveFilter();
	afx_msg void OnLoadFilter();
	afx_msg void OnApplyEditFilter();
	afx_msg void OnSelchangeCompare();
	afx_msg void OnGetEditFilter();
	afx_msg void OnIsNotCopyLogData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
extern	CAnalyzeLogDlg	*g_lpMainDlg;

#endif // !defined(AFX_ANALYZELOGDLG_H__DA2E6960_E09A_40F1_8460_40E111CF287E__INCLUDED_)
