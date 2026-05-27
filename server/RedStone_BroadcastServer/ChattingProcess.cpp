#include "stdafx.h"
#include "RedStone_BroadCastServer.h"
#include "RedStone_BroadCastServerDlg.h"
#include "cSRVUTIL.h"
#include "cPACKET_BCSERVER.h"
#include "ChattingDataBase.h"

void ChattingDBThread(void * _pParam)
{
	CRedStone_BroadCastServerDlg * pcMain = (CRedStone_BroadCastServerDlg *)_pParam;
	
	CChattingLog	cLogData;

	while(1)
	{
		Sleep(1);

		if	( strcmp(CRedStone_BroadCastServerDlg::getWorldName(),""))
			break;

	}

	while(g_isLoopWT)
	{
		Sleep(1);
		
		//	send this data to SQL DB
//		g_ChattingDataBase.sendData();
	}
}
