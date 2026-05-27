#include "stdafx.h"
#include "LiveThread.h"
#include "client.h"
#include "AgentDlg.h"
#include "reg_common.h"
#include "killProcess.h"

#define	dLIMIT_TIME_FOR_ING_SERVER		1000*60*5	//	부팅중.. 끄는중...
#define	dLIMIT_TIME_FOR_UPDATE_LIFESIGN	1000*60		//	라이프 사인 제대로 들어오는거 기디리기

void
LiveThread(void * _pParam)
{
	CAgentDlg	*lpMainDlg		=	(CAgentDlg	*)_pParam;
	int			iDeathSignCount	=	0;
	int			iLoopPeriod		=	5000;
	int			iBootingCount	=	0;

	while(g_bIsMainLoop)
	{
		Sleep(iLoopPeriod);

		if	(lpMainDlg->IsDlgButtonChecked(IDC_UPKEEP_LIVE_SERVER))
		{
			int	iServerType	=	lpMainDlg->m_iServerType;

			if	(iServerType	==	-1)
				continue;

			{
				char	strExeName[128];

				ReadServerEXENameWithRegistry(lpMainDlg->m_iServerType,strExeName,128);

				if	(IsActiveProcess(strExeName)	==	FALSE)
					WriteServerStatusToRegistry(lpMainDlg->m_iServerType,eSERVER_STATUS_OFF);
			}

			int	iStatus		=	ReadServerStatusWithRegistry(iServerType);

			switch(iStatus)
			{
				case	eSERVER_STATUS_BOOTING	:
				case	eSERVER_STATUS_QUIT		:
				{
					iBootingCount++;

					if	(iBootingCount	>=	(dLIMIT_TIME_FOR_ING_SERVER/iLoopPeriod))	//	5분 이상 맛탱이 갔다.
					{
						iDeathSignCount	=	100000;

						WriteLifeSignToRegistry(iServerType,0);
						break;
					}
					continue;
				}

				case	eSERVER_STATUS_OFF		:
					lpMainDlg->beginServer();
					continue;
			}

			iBootingCount	=	0;

			int	iLifeSign	=	ReadLifeSignWithRegistry(iServerType);

			if	(iLifeSign	==	0)
			{
				iDeathSignCount++;

				if	(iDeathSignCount	>=	(dLIMIT_TIME_FOR_UPDATE_LIFESIGN/iLoopPeriod))
				{
					lpMainDlg->killServer();
					Sleep(1000);
					lpMainDlg->beginServer();

					iDeathSignCount	=	0;
				}
			}
			else
			{
				iDeathSignCount	=	0;
				WriteLifeSignToRegistry(iServerType,0);
			}
		}
	}
}