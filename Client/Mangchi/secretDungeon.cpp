#include "secretDungeon.h"

cSecretDungeonManager	g_sdManager;

void
cSecretDungeon::reset()
{
	m_wSerial			=	0xffff;
	m_wLimitCount		=	5;
	m_bf8FloorCount		=	3;
	m_wUpkeepTime		=	60+30;
	m_bf1IsTestDungeon	=	TRUE;

	strcpy(m_strName,"valid");
	strcpy(m_strIDCode,"valid");
		
	for (int i=0;i<dMAX_FLOOR_COUNT_FOR_SECRET_DUNGEON;i++)
		m_aFloor[i].reset();
}

void
cSecretDungeon::copy(cSecretDungeon *_lpDungeon)
{
	memcpy(this,_lpDungeon,sizeof(cSecretDungeon));
}

cSecretDungeonManager::cSecretDungeonManager()
{
	init();
	reset();
}

int
cSecretDungeonManager::add(cSecretDungeon *_lpDungeon,BOOL _bIsCheckNameAndIDCode)
{
	m_den.setFirst();

	while(1)
	{
		cSecretDungeon	*lpInfo	=	m_den.getNext();

		if (!lpInfo)
			break;

		if (stricmp(lpInfo->m_strName,_lpDungeon->m_strName) == 0)
		{
			ERRMSG("error!!","이미 같은 이름의 비밀 던젼이 있습니다.");

			return	0xffff;
		}
		if (stricmp(lpInfo->m_strIDCode,_lpDungeon->m_strIDCode) == 0)
		{
			ERRMSG("error!!","이미 같은 ID Code를 가진 비밀 던젼이 있습니다.");

			return	0xffff;
		}
	}

	if	(_bIsCheckNameAndIDCode)
		return	0;

	return	m_den.add(_lpDungeon);
}

BOOL
cSecretDungeonManager::replace(int _iSerial,cSecretDungeon *_lpDungeon)
{
	m_den.setFirst();

	while(1)
	{
		cSecretDungeon	*lpInfo	=	m_den.getNext();

		if (!lpInfo || _iSerial == lpInfo->m_wSerial)
			break;

		if (stricmp(lpInfo->m_strName,_lpDungeon->m_strName) == 0)
			return	ERRMSG("error!!","이미 같은 이름의 비밀 던젼이 있습니다.");

		if (stricmp(lpInfo->m_strIDCode,_lpDungeon->m_strIDCode) == 0)
			return	ERRMSG("error!!","이미 같은 ID의 비밀 던젼이 있습니다.");
	}

	return	m_den.replace(_iSerial,_lpDungeon);	
}

void
cSecretDungeonManager::build()
{
	m_den.rebuild();

	if (getCount() == 0)
	{
		cSecretDungeon	dungeon;

		dungeon.reset();

		add(&dungeon);
	}

	{
		cSecretDungeon	*lpDungeon	=	m_den.get(0);

		lpDungeon->reset();
		lpDungeon->m_wSerial	=	0;
	}
}

void
cSecretDungeonManager::copy(cSecretDungeonManager *_lpSDM)
{
	reset();

	_lpSDM->m_den.setFirst();

	while(1)
	{
		cSecretDungeon	*lpInfo	=	_lpSDM->m_den.getNext();

		if	(!lpInfo)
			break;

		replace(lpInfo->m_wSerial,lpInfo);
	}

	memcpy(m_aValueString,_lpSDM->m_aValueString,sizeof(m_aValueString));
	
	rebuild();
}
