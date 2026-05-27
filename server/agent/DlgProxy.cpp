// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "Agent.h"
#include "DlgProxy.h"
#include "AgentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAgentDlgAutoProxy

IMPLEMENT_DYNCREATE(CAgentDlgAutoProxy, CCmdTarget)

CAgentDlgAutoProxy::CAgentDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CAgentDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CAgentDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CAgentDlgAutoProxy::~CAgentDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CAgentDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CAgentDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CAgentDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAgentDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CAgentDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAgent to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D439BFFB-26FE-44BB-B034-5122ACF63353}
static const IID IID_IAgent =
{ 0xd439bffb, 0x26fe, 0x44bb, { 0xb0, 0x34, 0x51, 0x22, 0xac, 0xf6, 0x33, 0x53 } };

BEGIN_INTERFACE_MAP(CAgentDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CAgentDlgAutoProxy, IID_IAgent, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {E4FF91DC-E699-4724-9258-BA0790341034}
IMPLEMENT_OLECREATE2(CAgentDlgAutoProxy, "Agent.Application", 0xe4ff91dc, 0xe699, 0x4724, 0x92, 0x58, 0xba, 0x7, 0x90, 0x34, 0x10, 0x34)

/////////////////////////////////////////////////////////////////////////////
// CAgentDlgAutoProxy message handlers
