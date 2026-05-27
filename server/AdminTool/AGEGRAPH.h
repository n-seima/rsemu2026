#if !defined(AFX_AGEGRAPH_H__087357F3_666E_49A8_9549_BEC342235571__INCLUDED_)
#define AFX_AGEGRAPH_H__087357F3_666E_49A8_9549_BEC342235571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AGEGRAPH.h : header file
//
#pragma warning( push )
#pragma warning( disable : 4786 )

#include <vector>
#include <algorithm>
#include "cADO_ADMIN.h"

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CAGEGRAPH dialog

class CAGEGRAPH : public CDialog
{
// Construction
public:
	CAGEGRAPH(CWnd* pParent = NULL);   // standard constructor

	int		iConnectGraph[8][24];
	int		iPlayGraph[8][24];
	int		iDL[dMAX_WORLD_SERVER_COUNT][200];
	void	Set(int **c,int **p,int t,vector<cADO_ADMIN::stUserStatsPack> * _pVector);
	DWORD	time;
	int		iServer;
	int		iSetType;
	int		iSetAge;
	int		iSelectGraph;
	vector<cADO_ADMIN::stUserStatsPack> * m_pVector;
	vector<int> m_VectorAll;
	int			m_allMaxIndex;	//최대 동접이 된 시간을 기억한다.
// Dialog Data
	//{{AFX_DATA(CAGEGRAPH)
	enum { IDD = IDD_AGE_COUNT };
	CComboBox	m_ctlServerList;
	CComboBox	m_ctlTypeList;
	CComboBox	m_ctlSelectList;
	CComboBox	m_ctlAgeList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAGEGRAPH)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAGEGRAPH)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeAgeList();
	afx_msg void OnSelchangeSelectList();
	afx_msg void OnSelchangeType();
	afx_msg void OnSelchangeServerList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#pragma warning( pop )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AGEGRAPH_H__087357F3_666E_49A8_9549_BEC342235571__INCLUDED_)
