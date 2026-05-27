#include	"stdafx.h"
#include	"WorkerThread.h"
#include	"cSOCKET.h"
#include	"AgentDlg.h"
#include	"Client.h"
#include	"cPACKET_BASE.h"

void
ReconnectThread(void * _pParam)
{
	CAgentDlg	*lpMainDlg			=	(CAgentDlg *)_pParam;
	cSOCKET		*lpRootAgentSocket	=	lpMainDlg->getLoginServerSocket();

	while(g_bIsMainLoop)
	{
		Sleep(5000);

		if	(lpRootAgentSocket->sock	==	0)
		{
			if	(!lpMainDlg->connectWithLoginServer())
			{
				lpRootAgentSocket->isLoopRecv = FALSE;
				lpRootAgentSocket->kill();

				lpMainDlg->PrintMainMessage("RECONNECT : Fail Connect with Root agent server!!");
			}
		}

		if	(lpRootAgentSocket->sock	!=	0)
			lpMainDlg->sendCurrentStatus();
	}

	lpMainDlg->PrintMainMessage("End Reconnect Thread");
}