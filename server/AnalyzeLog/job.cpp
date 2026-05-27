#include "stdafx.h"
#include <winsock2.h>
#include <atlbase.h>
#include "job.h"

cJOB		g_aJob[dMAX_JOB_COUNT];

BOOL
cJOB::Init()
{
	CRegKey		reg;

	char		strLogFolder[512],strDataFolder[512];

	if (reg.Open( g_hRegKey, g_lpszRegistry) == ERROR_SUCCESS)
	{
		DWORD	count	=	sizeof(strLogFolder);

		if	(reg.QueryValue((LPTSTR )strLogFolder,"Path",&count) == ERROR_SUCCESS)
		{
			if	(count	<=	0)
				return FALSE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;

	sprintf(strDataFolder,"%s/data/scenario/red stone",strLogFolder);

	if	(!SetCurrentDirectory(strDataFolder))
		return	FALSE;

	cFILE	dataFile;

#ifdef	_FOR_KOREA		
	if (dataFile.Open("job.dat","rb"))
#else
	if (dataFile.Open("job2.dat","rb"))
#endif
	{
		int	iCount,iVersion=-1,iVersionCheck,iVersionCheckCode=0x12345678;
		dataFile.Read(&iVersionCheck,4);
		
		if	(iVersionCheck	!=	iVersionCheckCode)
		{
			dataFile.Rewind();
			dataFile.Read(&iCount,4);
		}
		else
		{
			dataFile.Read(&iVersion,4);
			dataFile.Read(&iCount,4);
		}

		dataFile.Read(g_aJob,sizeof(cJOB)*iCount);	//	시리얼과 어빌러티

		dataFile.Close();
	}	//	직업 데이터 로딩
	else
	{
		SetLogFolder();
		return	FALSE;
	}

	SetLogFolder();

	return	TRUE;
}
