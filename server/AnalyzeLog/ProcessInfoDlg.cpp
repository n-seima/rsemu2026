// ProcessInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzelog.h"
#include "ProcessInfoDlg.h"
#include	"sfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessInfoDlg dialog


CProcessInfoDlg::CProcessInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessInfoDlg::IDD, pParent)
{
	m_iFileCount		=	0;
	m_iCurrentFileIndex	=	0;
	m_strCurrentFileName[0]	=	NULL;
	m_iFileSize			=	0;
	m_iCurrendReadSize	=	0;
	m_iReadLogCount		=	0;
	m_bIsCreated		=	FALSE;
	m_iAnalLogRate		=	0;
	m_bIsEndDlg			=	FALSE;
	//{{AFX_DATA_INIT(CProcessInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProcessInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcessInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessInfoDlg message handlers

BOOL
CProcessInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bIsEndDlg	=	FALSE;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProcessInfoDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_bIsEndDlg	=	TRUE;

	CDialog::OnCancel();
}

void
CProcessInfoDlg::show(CWnd *_lpParents)
{
	if	(m_bIsCreated)
		return;

	Create(CProcessInfoDlg::IDD,_lpParents);
	ShowWindow(SW_SHOW);

	m_bIsCreated	=	TRUE;
}

void
CProcessInfoDlg::hide()
{
	if	(!m_bIsCreated)
		return;

	ShowWindow(SW_HIDE);
}

void
CProcessInfoDlg::destroy()
{
	if	(m_bIsCreated	==	FALSE)
		return;

	m_bIsCreated	=	FALSE;

	DestroyWindow();
}

void
CProcessInfoDlg::endDlg()
{
	if	(m_bIsEndDlg)
		return;

	SetDlgItemText(IDCANCEL,"Complete");

	m_bIsEndDlg	=	TRUE;

//	CDialog::OnCancel();
}

void
CProcessInfoDlg::setFileCount(int _iFileCount)
{
	m_iFileCount	=	_iFileCount;

	if	(m_bIsEndDlg)
		return;

	SetDlgItemText(IDC_ST_FILE_COUNT,_ms(" %d/%d",m_iCurrentFileIndex,m_iFileCount));
}

void
CProcessInfoDlg::setCurrentFileIndex(int _iFile)
{
	m_iCurrentFileIndex	=	_iFile;

	if	(m_bIsEndDlg)
		return;

	SetDlgItemText(IDC_ST_FILE_COUNT,_ms(" %d/%d",m_iCurrentFileIndex,m_iFileCount));
}

void
CProcessInfoDlg::setCurrentFile(char *_lpstrFile,int _iFullSize)
{
	if	(m_bIsEndDlg)
		return;

	strcpy(m_strCurrentFileName,_exportFileName(_lpstrFile));
	m_iFileSize	=	_iFullSize;

	SetDlgItemText(IDC_ST_CURRENT_FILE,_ms(" %s read [%d kb / %d kb ] 0%%",m_strCurrentFileName,m_iCurrendReadSize/1024,m_iFileSize/1024));

	setAnalLogSize(-1);
}

void
CProcessInfoDlg::setUploadFileSize(int _iSize)
{
	if	(m_bIsEndDlg)
		return;

	m_iOldReadLogRate	=	m_iReadLogRate;
	m_iCurrendReadSize	=	_iSize;
	m_iReadLogRate		=	0;
	
	if	(m_iFileSize)
	{
		LONGLONG	llSize	=	m_iCurrendReadSize;
		m_iReadLogRate		=	(int)(llSize*100/m_iFileSize);
	}

	m_iReadLogRate		=	min(100,m_iReadLogRate+1);

	if	(m_iOldReadLogRate	!=	m_iReadLogRate)
	{
		if	(m_iReadLogRate	==	100)
			SetDlgItemText(IDC_ST_CURRENT_FILE,_ms(" %s read complete",m_strCurrentFileName));
		else
			SetDlgItemText(IDC_ST_CURRENT_FILE,_ms(" %s read %d%%",m_strCurrentFileName,m_iReadLogRate));
	}
}

void
CProcessInfoDlg::setAnalLogSize(int _iSize)
{
	if	(m_bIsEndDlg)
		return;

	if	(_iSize	==	-1)
	{
		m_iOldAnalLogRate	=	-1;
		m_iAnalLogSize		=	0;
		m_iAnalLogRate		=	0;

		SetDlgItemText(IDC_ST_ANAL_LOG," ready...");

		return;
	}

	m_iOldAnalLogRate	=	m_iAnalLogRate;
	m_iAnalLogSize		=	_iSize;
	m_iAnalLogRate		=	0;
	
	if	(m_iFileSize)
	{
		LONGLONG	llSize	=	m_iAnalLogSize;
		m_iAnalLogRate		=	(int)(llSize*100/m_iFileSize);
	}

	m_iAnalLogRate		=	min(100,m_iAnalLogRate+1);

	if	(m_iOldAnalLogRate	!=	m_iAnalLogRate)
	{
		if	(m_iAnalLogRate	==	100)
			SetDlgItemText(IDC_ST_ANAL_LOG," read complete");
		else
			SetDlgItemText(IDC_ST_ANAL_LOG,_ms(" %d%%",m_iAnalLogRate));
	}
}

void
CProcessInfoDlg::setReadLogCount(int  _iCount,BOOL _bIsForce)
{
	if	(m_bIsEndDlg)
		return;

	m_iReadLogCount		=	_iCount;

	static	DWORD	s_dwLastUpdateTick	=	0;

	if	(!_bIsForce)
	{
		DWORD	dwCurrentTick	=	GetTickCount();

		if	(s_dwLastUpdateTick	+	100	>	dwCurrentTick)
			return;

		s_dwLastUpdateTick		=	dwCurrentTick;
	}

	SetDlgItemText(IDC_ST_READ_COUNT,_ms(" %d",m_iReadLogCount));
}