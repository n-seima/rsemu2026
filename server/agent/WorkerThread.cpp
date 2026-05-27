#include	"stdafx.h"
#include	"WorkerThread.h"
#include	"cSOCKET.h"
#include	"AgentDlg.h"
#include	"Client.h"

void
WorkerThread(void * _pParam)
{
	CAgentDlg*	lpMainDlg = (CAgentDlg *)_pParam;

	LP_IO_DATA	lpOV;
	DWORD		dwRead,dwIOCPKey;
	BOOL		retVal	=	TRUE;

	while(g_bIsMainLoop)
	{
		dwIOCPKey	=	0xffffffff;
		retVal		=	GetQueuedCompletionStatus(lpMainDlg->getCompletionPort(),&dwRead,&dwIOCPKey,(LPOVERLAPPED *)&lpOV,5000);

		if	(g_bIsMainLoop	==	FALSE)
			break;

		if	(retVal && dwRead)
		{
			CClient	* lpUser = UserManager.GetClientPnt((WORD)dwIOCPKey);

			if	(!lpUser)
				continue;	

			if	(lpOV->IOState == 0)
			{
				lpUser->AddRecvPacket(lpOV->buf.buf,dwRead);

				lpMainDlg->RecvPacket(dwIOCPKey);
				lpMainDlg->ProcessRecvPacket(dwIOCPKey);
			}
			else
			if	(lpOV->IOState == 1)
			{
				int	iResultOfSend	=	lpUser->CompleteSendPacket(dwRead);

				if	(iResultOfSend	>	0)
					lpMainDlg->sendPacketAdd(dwIOCPKey);
				else
					lpMainDlg->sendPacket(dwIOCPKey);
			}

			continue;
		}

		if	(dwRead == 0 )
		{
			if	(dwIOCPKey==0xffffffff)
				continue;

			lpMainDlg->DisUser((WORD)dwIOCPKey,"dwRead = 0");
		}
		else
		{
			DWORD dwError = GetLastError();

			if	(dwError != ERROR_IO_PENDING && dwError != 258/*대기시간 초과 , GetQueuedCompletionStatus의 마지막 옵션.*/)
			{
				lpMainDlg->PrintMainMessage("Worker thread error GetQueueCompletionStatus... error [%d]",dwError);

				lpMainDlg->DisUser((WORD)dwIOCPKey,"GetQueuedCompletionStatus ERROR");
			}
		}
	}



}