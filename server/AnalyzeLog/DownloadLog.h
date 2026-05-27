#pragma once

#include "sfc.h"

extern	void	DownLoadThread(void *tmp);
extern	BOOL	g_bIsDownloadLogTime;
extern	BOOL	g_bIsWaitingDownloadLog;

extern	char	g_strAccountDBIP[dIP_LENGTH];//	ACCOUNTDBINFO , 1
extern	char	g_strAccountTableName[128];	//	ACCOUNTDBINFO , 2
extern	char	g_strAccountDBID[32];
extern	char	g_strAccountDBPW[32];
