#include "stdafx.h"
#include "AdminTool.h"
#include "CTitle.h"
#include "SFC.h"
#include "cITEM.H"
#include <atlbase.h>
#include <stdio.h>
#include "cITEM_PREFIX.h"
#include "CMessage.h"

CTitleManager	g_titleManager;

//
//	타이틀 데이터 로딩
BOOL
CTitleManager::init()
{
	if	(m_iTitleCount)
		return	TRUE;
	
	CRegKey		reg;

	char		strLogFolder[512],strDataFolder[512];

	int iResult	=	reg.Open( g_hRegKey, g_lpstrRegistry);

	if (iResult == ERROR_SUCCESS)
	{
		DWORD	count	=	sizeof(strLogFolder);
		if (reg.QueryValue((LPTSTR )strLogFolder,"Path",&count) == ERROR_SUCCESS)
		{	if (count	<=	0)
				return FALSE;
		}
		else	return FALSE;
	}
	else	return FALSE;

	cFILE	file;

	sprintf(strDataFolder,"%s/data",strLogFolder);

	if (!SetCurrentDirectory(strDataFolder))	return	FALSE;

	if (!file.Open("title.dat","rb"))
		return	ERRMSG("error in CTitleManager::init()","can not find 'title.dat' file");

	file.Read(&m_iTitleCount,4);
	file.Read(m_aTitles,sizeof(m_aTitles));

	return	TRUE;
}

CTitle*
CTitleManager::getTitleByName(char *_lpstrTitleName)
{
	for (int i=dMAX_TITLE_COUNT;i;)
	{
		--i;

		if	(m_aTitles[i].m_wSerial	==	0xffff)
			continue;

		if	(STRICMP(m_aTitles[i].m_strName,_lpstrTitleName)	==	0)
			return	&m_aTitles[i];
	}

	return	NULL;
}
