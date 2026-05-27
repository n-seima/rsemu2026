// AnalyzeLog.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AnalyzeLog.h"
#include "AnalyzeLogDlg.h"
#include "cMessage.h"
#include "assert.h"
#include <ATLBASE.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char szProgName[]="ANALYZELOG";
const char szProgTitle[]="AnalyzeLog - RedStone";
HANDLE		szMutexHandle = NULL;

/////////////////////////////////////////////////////////////////////////////
// CAnalyzeLogApp

BEGIN_MESSAGE_MAP(CAnalyzeLogApp, CWinApp)
	//{{AFX_MSG_MAP(CAnalyzeLogApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnalyzeLogApp construction

CAnalyzeLogApp::CAnalyzeLogApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CAnalyzeLogApp::~CAnalyzeLogApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	if(szMutexHandle)
		ReleaseMutex(szMutexHandle); 
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAnalyzeLogApp object

CAnalyzeLogApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAnalyzeLogApp initialization

BOOL CAnalyzeLogApp::InitInstance()
{
	const	int	c_iCheckLeakBlock	=	-1;

	if	(c_iCheckLeakBlock	!=	-1)
	{
		assert(0 && "메모리 릭 체크 변수가 세팅되어 있습니다!!");

		_CrtSetBreakAlloc( c_iCheckLeakBlock );
	}

    szMutexHandle = CreateMutex(NULL, TRUE, szProgName);

    if	(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND hCurWnd = ::FindWindow(NULL,szProgTitle);
		if(hCurWnd)		::ShowWindow(hCurWnd,SW_RESTORE);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CAnalyzeLogDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
