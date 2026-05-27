#include	"stdafx.h"
#include	"LogInServerThread.h"
#include	"cSOCKET.h"
#include	"AgentDlg.h"

void
LogInServerThread(void * _pParam)
{
	CAgentDlg*	lpMainDlg	=	(CAgentDlg *)_pParam;
	cSOCKET*	lpSocket	=	lpMainDlg->getLoginServerSocket();

	char		aRecvPacketBuffer[dMAX_PACKET_LENGTH_SERVER];
	WORD		wType,wReceiveSize;

	while(lpSocket->isLoopRecv)
	{
		Sleep(1);

		while(lpSocket->GetRecvPacket(aRecvPacketBuffer))
		{
			wReceiveSize=	*(WORD *)aRecvPacketBuffer;
			wType		=	*(WORD *)(aRecvPacketBuffer+2);

// 			switch(wType)
// 			{
// 			}
		}
	}

	lpMainDlg->PrintMainMessage("Finish.. Log In Server Thread ..");
}
