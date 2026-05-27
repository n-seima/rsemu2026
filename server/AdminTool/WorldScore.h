#if !defined(AFX_WORLDSCORE_H__C0E8F944_AC94_42F8_8180_5FDB424A0B43__INCLUDED_)
#define AFX_WORLDSCORE_H__C0E8F944_AC94_42F8_8180_5FDB424A0B43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorldScore.h : header file
//
#pragma warning( push )
#pragma warning( disable : 4786 )

#include "CFTPclient.h"
#include "cScript.h"
#include "cADO_ADMIN.h"
#include <vector>

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CWorldScore dialog

#define dAGE_DEFIEN_COUNT		8

class CWorldScore : public CPropertyPage
{
	DECLARE_DYNCREATE(CWorldScore)


	enum
	{	eREALTIME_TIMER_EVENT_ID = WM_USER+1001,	
		eREALTIME_TIMER_INTERVAL = 6000,
	};
// Construction
public:
	CWorldScore();
	~CWorldScore();

	cADO_ADMIN		DB;
	nsFTP::CFTPClient	ftp;
	cDynamicScript		scp;
	HICON			iconGraph;
	int				AC[dAGE_DEFIEN_COUNT][24], PC[dAGE_DEFIEN_COUNT][24];	//	시간,나이별 접속 | 시간, 나이별 플레이 시간 | 레벨 분포
	//									  ↑서버수에 비례하게 늘려가라.. ;;
	vector<cADO_ADMIN::stUserStatsPack>		m_vcDetailCC[dMAX_WORLD_SERVER_COUNT];
	int				m_iSelectedServerIdx;
	int				m_iCurDay;
	BOOL			m_bRealTime;	//	통계를 실시간으로 볼 것인가?

public:
	void stopRealTime();
	void startRealTime();

// Dialog Data
	//{{AFX_DATA(CWorldScore)
	enum { IDD = IDD_WORLD_SCORE };
	CStatic	m_ctlTotalConnectedUserCount;
	CButton	m_ctlGraph;
	CComboBox	m_ctlAgeList;
	CComboBox	m_ctlServerList;
	CStatic	m_ctlSUM_TEXT;
	CComboBox	m_ctlScoreByTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWorldScore)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWorldScore)
	afx_msg void OnSelchangeMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeServerList();
	afx_msg void OnGraph();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSumText();
	afx_msg void OnExcel();
	afx_msg void OnRealTimeView();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#pragma warning( pop )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORLDSCORE_H__C0E8F944_AC94_42F8_8180_5FDB424A0B43__INCLUDED_)
