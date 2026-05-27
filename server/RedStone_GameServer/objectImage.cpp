#include "objectImage.h"

cObjectCrashInfoManager	g_ociManager;

BOOL
cObjectCrashInfoManager::init()
{
	int		iTileSetCount,iDoorCount,i;

	cFILE	file;

	if (!file.Open("data/doorObjectCrashMap.dat","rb"))
		return	ERRMSG("error!!","can not find 'data/doorObjectCrashMap.dat' file");

	file.Read(&iTileSetCount,4);
	file.Read(&iDoorCount,4);

	for (i=0;i<iDoorCount;i++)
	{
		int	iTileSet,iObject,iSize;

		file.Read(&iTileSet,4);
		file.Read(&iObject,4);
		file.Read(&iSize,4);

		m_aData[iTileSet][iObject].m_pCrashInfo	=	new BYTE [iSize];

		file.Read(m_aData[iTileSet][iObject].m_pCrashInfo,iSize);
	}

	return	TRUE;
}
