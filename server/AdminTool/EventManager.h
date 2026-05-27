#if !defined(AFX_EVENTMANAGER_H__CAB01AD2_65AB_4B30_8561_61F5522C7CEF__INCLUDED_)
#define AFX_EVENTMANAGER_H__CAB01AD2_65AB_4B30_8561_61F5522C7CEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventManager.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEventManager dialog

#define	dMAX_LIST_COUNT					300
enum
{

	eEVENT_UNANIMOUS,		// 이벤트 이구동성
	eEVENT_CHRISTMAS,		// 이벤트 크리스마스.

};
class	cEventListDefine
{
public:
	char	m_ServerName[dMAX_LIST_COUNT][20];
	char	m_strName[dMAX_LIST_COUNT][20];
	int		m_iListCount;
};

class	cCheckEventList	:public cEventListDefine
{
public:
	void	load(char* _strServerName, char* _strFileName);
};


class CEventManager : public CPropertyPage
{
// Construction
public:
	int					m_iSelectOXQuizSolution;
	BOOL				m_bIsAppointmentGoldRushEvent;
	BOOL				m_bIsCancelGoldRushEvent;
	cCheckEventList		m_EventList;

	CEventManager();   // standard constructor

	void	SendWorldToWordEvent(BOOL isStartNow);


// Dialog Data
	//{{AFX_DATA(CEventManager)
	enum { IDD = IDD_EVENT };
	CEdit	m_ebGREndComment;
	CEdit	m_ebGRBeginComment;
	CEdit	m_ebDropChance;
	CEdit	m_ebExp;
	CEdit	m_ebMinute2;
	CEdit	m_ebMinute1;
	CEdit	m_ebHour2;
	CEdit	m_ebHour1;
	CEdit	m_ebDay2;
	CEdit	m_ebDay1;
	CEdit	m_ebMonth2;
	CEdit	m_ebMonth1;
	CEdit	m_ebYear2;
	CEdit	m_ebYear1;
	CEdit	m_editOxQuizQuestion;
	CEdit	m_editYear;
	CEdit	m_editMonth;
	CEdit	m_editHour;
	CEdit	m_editDay;
	CComboBox	m_cbOX;
	CComboBox	m_cbEventKind;		// 이벤트종류... 09.10.09
	CComboBox	m_cbEventBuff;		// 이벤트버프종류... 09.10.09
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventManager)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEventManager)
	virtual BOOL OnInitDialog();
	afx_msg void OnBookingOxQuizEvent();
	afx_msg void OnOxQuiz();
	afx_msg void OnCompleteOxQuiz();
	afx_msg void OnCancelOxQuizEvent();
	afx_msg void OnBookingOxQuizEventForTest();
	afx_msg void OnReviveOxQuizLoserBattle();
	afx_msg void OnGrApply();
	afx_msg void OnGrCancel();
	afx_msg void OnGetSchedule();
	afx_msg void OnFillTime();
	afx_msg void OnAppointmentWordQuiz();
	afx_msg void OnCancelWordQuiz();
	afx_msg void OnSettingWqQuestion();
	afx_msg void OnChangeWordQuizRound();
	afx_msg void OnEndWordQuiz();
	afx_msg void OnTestWordQuiz();
	afx_msg void OnEventListButtonSerch();
	afx_msg void OnEventListButtonOk();
	afx_msg void OnChattingEventButtonSet();
	afx_msg void OnChattingEventButtonRelease();
	afx_msg void OnChangeMessageOfSoundOfLedersBell();
	afx_msg void OnResetSoundOfLedersBell();
	afx_msg void OnCancelSoundOfLedersBell();
	afx_msg void OnCheckEventInfo();
	afx_msg void OnLottoEvent();
	afx_msg void OnEventTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTMANAGER_H__CAB01AD2_65AB_4B30_8561_61F5522C7CEF__INCLUDED_)
