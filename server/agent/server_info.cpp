#include	"stdafx.h"
#include	"server_info.h"
#include	"AgentDlg.h"

cServerList		g_serverList;

cWorldServerInfo*
cServerList::getWorldByIP(char *_lpstrIP)
{
	for (int i=0;i<dMAX_WORLD_SERVER_COUNT;i++)
	{
		cWorldServerInfo*	lpWorld	=	&m_aWorldList[i];

		if	(lpWorld->m_iIndex	==	-1)
			continue;

		if	(stricmp(lpWorld->m_aServerList[eSTIW_WORLD].m_strIP,_lpstrIP)==0)
			return	lpWorld;
	}

	return	NULL;
}

void
cServerList::updateServerInfo(cAgentServerInfo *_lpInfo,CAgentDlg *_lpMainDlg)
{
	cWorldServerInfo	*lpWorld	=	NULL;

	if	(_lpInfo->m_wServerType	<	eST_SINGLE_SERVER_BEGIN)
	{
		if	(_lpInfo->m_wServerType	==	eST_WORLD)
		{
			lpWorld				=	&m_aWorldList[_lpInfo->m_iWorldIndex];
			lpWorld->m_iIndex	=	_lpInfo->m_iWorldIndex;
		}
		else
			lpWorld	=	getWorldByIP(_lpInfo->m_strWorldIP);

		if	(!lpWorld)
			return;

		int	iServerIndex	=	-1;

		if	(_lpInfo->m_wServerType		!=	eST_GAME)
			iServerIndex	=	_lpInfo->m_wServerType;
		if	(_lpInfo->m_iGameServerIndex	!=	-1)
		{
			iServerIndex				=	eSTIW_GAME0+_lpInfo->m_iGameServerIndex;
			lpWorld->m_iGameServerCount	=	max(lpWorld->m_iGameServerCount,_lpInfo->m_iGameServerIndex+1);
		}

		if	(iServerIndex	==	-1)
			return;

		cServerInfo	*lpServer	=	&lpWorld->m_aServerList[iServerIndex];

		lpServer->m_iServerType	=	_lpInfo->m_wServerType;
		lpServer->m_iGameIndex	=	_lpInfo->m_iGameServerIndex;
		lpServer->m_iWorldIndex	=	_lpInfo->m_iWorldIndex;
		lpServer->m_iStatus		=	_lpInfo->m_wServerStatus;
		lpServer->m_iVersion	=	_lpInfo->m_iServerVersion;

		strcpy(lpServer->m_strIP,_lpInfo->m_strIP);
		strcpy(lpServer->m_strName,_lpInfo->m_strName);

		_lpMainDlg->updateTreeItem(lpServer);

		return;
	}

	{
		int	iSlot	=	_lpInfo->m_wServerType-eST_SINGLE_SERVER_BEGIN;

		cServerInfo	*lpServer	=	&m_aSingleServerList[iSlot];

		lpServer->m_iServerType	=	_lpInfo->m_wServerType;
		lpServer->m_iGameIndex	=	_lpInfo->m_iGameServerIndex;
		lpServer->m_iWorldIndex	=	_lpInfo->m_iWorldIndex;
		lpServer->m_iStatus		=	_lpInfo->m_wServerStatus;
		lpServer->m_iVersion	=	_lpInfo->m_iServerVersion;

		strcpy(lpServer->m_strIP,_lpInfo->m_strIP);
		strcpy(lpServer->m_strName,_lpInfo->m_strName);

		_lpMainDlg->updateTreeItem(lpServer);
	}
}