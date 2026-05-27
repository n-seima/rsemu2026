#if !defined(AFX_DLGTIMESETTING_H__54EE3FC9_DAC5_4067_AADC_E9A028DFBE5C__INCLUDED_)
#define AFX_DLGTIMESETTING_H__54EE3FC9_DAC5_4067_AADC_E9A028DFBE5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTimeSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgTimeSetting dialog

class DlgTimeSetting : public CPropertyPage
{
	DECLARE_DYNCREATE(DlgTimeSetting)

// Construction
public:
	DlgTimeSetting();
	~DlgTimeSetting();

// Dialog Data
	//{{AFX_DATA(DlgTimeSetting)
	enum { IDD = IDD_DLG_TIME };
	BOOL	m_isUseTimeSetting;
	CTime	m_tiTime;
	CTime	m_tiDate;
	CString	m_strTimeInterval;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DlgTimeSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DlgTimeSetting)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTIMESETTING_H__54EE3FC9_DAC5_4067_AADC_E9A028DFBE5C__INCLUDED_)
