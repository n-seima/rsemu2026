#include	"../RedStone_DBServer/Client.h"

CClientDataManager*	g_lpCdm	=	NULL;	//	Client Data Manager

BOOL
CClientDataManager::Init()
{
	int		i;

	m_iCount			=	0;
	m_iRookie			=	0;
	m_iFailedDataCount	=	0;
	m_iFailedDataRookie	=	0;
	m_dwUpdateCounter	=	0;

	for (i=0;i<dDATA_MAX_COUNT;i++)	m_aStack[i].close();
	for (i=0;i<dMAX_FAILED_DATA_COUNT_TO_SAVE_DB;i++)	m_aFailedStack[i].close();

	return TRUE;
}

void
CClientDataManager::End()
{	
	m_iCount			=	0;
	m_iRookie			=	0;
	m_iFailedDataCount	=	0;
	m_iFailedDataRookie	=	0;
	m_dwUpdateCounter	=	0;
}

CClientData*
CClientDataManager::getClient(char *_lpstrName,BOOL _bIsByName)
{
	if (_bIsByName)
	{
		for (int i=0,iCount=0;i<dDATA_MAX_COUNT && iCount < m_iCount;i++)
		{
			if (m_aStack[i].wSerial == 0xffff) continue;

			if (stricmp(m_aStack[i].pData.m_strName,_lpstrName)==0)	return	&m_aStack[i];

			iCount++;
		}
	}
	else
	{
		for (int i=0,iCount=0;i<dDATA_MAX_COUNT && iCount < m_iCount;i++)
		{
			if (m_aStack[i].wSerial == 0xffff) continue;

			if (stricmp(m_aStack[i].pData.m_strId,_lpstrName)==0)	return	&m_aStack[i];

			iCount++;
		}
	}

	return	NULL;
}