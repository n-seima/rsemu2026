#if !defined(AFX_PROCESSINFODLG_H__44DE131C_349F_4282_8746_BB0B6BE4D49B__INCLUDED_)
#define AFX_PROCESSINFODLG_H__44DE131C_349F_4282_8746_BB0B6BE4D49B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProcessInfoDlg dialog

class CProcessInfoDlg : public CDialog
{
// Construction
public:
	CProcessInfoDlg(CWnd* pParent = NULL);   // standard constructor

	void			endDlg();
	void			hide();
	void			destroy();
	void			show(CWnd *_lpParents);

	BOOL			m_bIsCreated;
	BOOL			m_bIsEndDlg;
	int				m_iFileCount,m_iCurrentFileIndex;
	char			m_strCurrentFileName[512];
	int				m_iFileSize,m_iCurrendReadSize;
	int				m_iReadLogCount,m_iAnalLogSize;
	int				m_iAnalLogRate,m_iOldAnalLogRate;
	int				m_iReadLogRate,m_iOldReadLogRate;

	void			setFileCount(int _iFileCount);
	void			setAnalLogSize(int _iSize);
	void			setCurrentFileIndex(int _iFile);
	void			setCurrentFile(char *_lpstrFile,int _iFullSize);
	void			setUploadFileSize(int _iSize);
	void			setReadLogCount(int  _iCount,BOOL _bIsForce);

	BOOL			OnInitDialog();
// Dialog Data
	//{{AFX_DATA(CProcessInfoDlg)
	enum { IDD = PROCESS_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProcessInfoDlg)
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSINFODLG_H__44DE131C_349F_4282_8746_BB0B6BE4D49B__INCLUDED_)
