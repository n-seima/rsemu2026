#if !defined(AFX_EXPCALC_H__95925E35_C80E_4762_92C6_EC7EEFE1121E__INCLUDED_)
#define AFX_EXPCALC_H__95925E35_C80E_4762_92C6_EC7EEFE1121E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExpCalc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ExpCalc dialog

class ExpCalc : public CDialog
{
// Construction
public:
	static	int		GetExp(int _iLevel1,int _iExp1,int _iLevel2,int _iExp2);
	ExpCalc(CWnd* pParent = NULL);   // standard constructor

	cPLAYER_DATA	*m_lpPlayer;
	int				m_iOrgLevel;
	int				m_iOrgExp;
	BOOL			open(cPLAYER_DATA	*_lpPlayer);

// Dialog Data
	//{{AFX_DATA(ExpCalc)
	enum { IDD = IDD_CALC_EXP };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ExpCalc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ExpCalc)
	afx_msg void OnCancel2();
	afx_msg void OnCalculate();
	afx_msg void OnCalRebirthExp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_EXPCALC_H__95925E35_C80E_4762_92C6_EC7EEFE1121E__INCLUDED_)
