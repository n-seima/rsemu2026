#include "secretDungeon.h"
#include "cFIELD.h"
#include "cProject.h"
#include "cGame.h"
#include "CEvent.h"

cSecretDungeonManager	*g_pSdManager;
cInstanceFieldManager	*g_pIfManager;

void
cSecretDungeon::reset()
{
	m_wActiveKeyCount		=	0;
}

cSecretDungeonManager::cSecretDungeonManager()
{
}

cSecretDungeonManager::~cSecretDungeonManager()
{
}

void
cSecretDungeonManager::rebuild()
{
	m_wGuildPointBattleSecretDungeon	=	0xffff;

	m_den.rebuild();

	for (int i=0;i<dMAX_GUILD_DUNGEON_COUNT;i++)
	{
		m_wGuildDungeon[i] = NULL;
	}

	for (i=0;i<m_den.getMaxCount();i++)
	{
		cSecretDungeon	*lpDungeon	=	m_den.get(i);

		if	(!lpDungeon)
			continue;

		if	(lpDungeon->m_bf1IsTowerOfOrdeal)
			m_wGuildPointBattleSecretDungeon	=	lpDungeon->m_wSerial;

		if	(lpDungeon->m_bf1IsGuildDungen)
		{
			if(!m_wGuildDungeon[lpDungeon->m_bf5GuildDungenSerial - 1])
				m_wGuildDungeon[lpDungeon->m_bf5GuildDungenSerial - 1] = lpDungeon->m_wSerial;
			else
				MessageBox(NULL , "Guild Dungeon Init Error" , "Check Guild Dungeon Serial..." , MB_OK);
		}

		lpDungeon->reset();
	}
}

cInstanceFieldManager::cInstanceFieldManager()
{
	m_iCount						=	1;
	m_iRookie						=	1;

	m_aInstanceField[0].m_wSerial	=	0;
}

cInstanceFieldManager::~cInstanceFieldManager()
{
	close();
}

cInstanceField*
cInstanceFieldManager::getRookieField()
{
	if (m_iCount	>=	dMAX_IF_COUNT)
		return	NULL;

	return	&m_aInstanceField[m_iRookie];
}

cInstanceField*
cInstanceFieldManager::get(int _iIndex)
{
	if (_iIndex<1 || _iIndex>=dMAX_IF_COUNT)
		return	NULL;

	cInstanceField*	lpIF	=	&m_aInstanceField[_iIndex];

	if (lpIF->m_wSerial	==	0xffff)
		return	NULL;

	return	lpIF;
}

cFIELD*
cInstanceFieldManager::getFloor(int _iIndex,int _iFloor,DWORD _dwPartyUniqueSerial,int _iCreateTimeValue)
{
	if (_iFloor	<	0	||	_iIndex<1 || _iIndex>=dMAX_IF_COUNT)
		return	NULL;

	cInstanceField*	lpIF	=	&m_aInstanceField[_iIndex];

	if	(lpIF->m_wSerial	==	0xffff)
		return	NULL;

	if	(_iFloor	>=	lpIF->m_wFloorCount)
		return	NULL;

	if	(_dwPartyUniqueSerial	!=	0xffffffff	&&	lpIF->m_dwPartyUniqueSerial	!=	_dwPartyUniqueSerial)
		return	NULL;

	if	(_iCreateTimeValue	!=	-1)
		if	(_iCreateTimeValue	!=	lpIF->m_wCreateTimeValue)
			return	NULL;

	return	&lpIF->m_pFloor[_iFloor];
}

int
cInstanceFieldManager::create(int _iDungeonSerial,cParty *_lpParty,int _iLinkField,int _iLinkGate)
{
	cInstanceField	*lpIF		=	getRookieField();

	if	(!lpIF)
		return	eCSDR_FULL;

	if	(isFullSecretDungeon(_iDungeonSerial))
		return	eCSDR_FULL;

	if	(_lpParty->m_wGateField	!=	0xffff)
	{
		if	(getByUniquePartySerial(_lpParty->m_dwUniqueSerial))
			return	eCSDR_ALREADY_USE_SECRET_DUNGEON;
	}

	BOOL	bIsSuccess	=	lpIF->init(_iDungeonSerial,m_iRookie,_lpParty,_iLinkField,_iLinkGate);

	if	(!bIsSuccess)
		return	eCSDR_FAILED;

	lpIF->m_wSerial			=	m_iRookie;
	lpIF->m_wCreateTimeValue=	g_currentTime.m_wMinute+(g_currentTime.m_wHour%12)*60;

	m_iCount++;

	for	(int i=m_iRookie+1;i<dMAX_IF_COUNT;i++)
	{
		if (m_aInstanceField[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;

			break;
		}
	}

	return	eCSDR_SUCCESS;
}

void
cInstanceFieldManager::close()
{
	for (int i=dMAX_IF_COUNT;i;)
	{
		--i;

		if	(m_aInstanceField[i].m_wSerial	==	0xffff)
			continue;

		m_aInstanceField[i].close();
	}

	m_iCount	=	0;
	m_iRookie	=	0;
}

void
cInstanceFieldManager::close(int _iSerial)
{
	if	(m_aInstanceField[_iSerial].m_wSerial	==	0xffff)
		return;

	m_aInstanceField[_iSerial].close();
	m_iCount--;

	if	(_iSerial	<	m_iRookie)
		m_iRookie	=	_iSerial;
}

int
cInstanceFieldManager::getAvailSecretDungeonCount(int _iDungeonSerial)
{
	int	iCount	=	0;

	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		if (m_aInstanceField[i].m_wSerial	==	0xffff)
			continue;

		if (m_aInstanceField[i].m_wSecretDungeonSerial	==	_iDungeonSerial)
			iCount++;
	}

	return	iCount;
}

BOOL
cInstanceFieldManager::isFullSecretDungeon(int _iDungeonSerial,BOOL _bIsIncludeActiveKey)
{
	cSecretDungeon	*lpDungeon	=	g_pSdManager->get(_iDungeonSerial);

	if (!lpDungeon)
		return	TRUE;

	int	iCurrentCount	=	getAvailSecretDungeonCount(_iDungeonSerial);

	if	(_bIsIncludeActiveKey)
		iCurrentCount	+=	lpDungeon->m_wActiveKeyCount;

	if (iCurrentCount	>=	lpDungeon->m_wLimitCount)
		return	TRUE;

	return	FALSE;
}

cInstanceField*
cInstanceFieldManager::getByUniquePartySerial(DWORD _dwUniqueSerial)
{
	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		if (m_aInstanceField[i].m_wSerial	==	0xffff)
			continue;

		if (m_aInstanceField[i].m_dwPartyUniqueSerial	==	_dwUniqueSerial)
			return	&m_aInstanceField[i];
	}

	return	NULL;
}

cACTOR*
cInstanceFieldManager::getPlayerByName(char *_lpstrName)
{
	cACTOR	*lpActor	=	NULL;

	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if	(lpIF->m_wSerial	==	0xffff)
			continue;

		if	(lpIF->m_pFloor	==	NULL)
			break;

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
		{
			lpActor	=	lpIF->m_pFloor[iFloor].getPlayerByName(_lpstrName);

			if	(lpActor)
				return	lpActor;
		}
	}

	return	NULL;
}

cACTOR*
cInstanceFieldManager::getPlayerByID(char *_lpstrID,DWORD _dwIDHashCode)
{
	cACTOR	*lpActor	=	NULL;

	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if	(lpIF->m_wSerial	==	0xffff)
			continue;

		if	(lpIF->m_pFloor	==	NULL)
			break;

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
		{
			lpActor	=	lpIF->m_pFloor[iFloor].getPlayerByID(_lpstrID,_dwIDHashCode);

			if	(lpActor)
				return	lpActor;
		}
	}

	return	NULL;
}

void
cInstanceFieldManager::updateGame()
{
	m_iRealPlayerCount	=	0;

	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if	(lpIF->m_wSerial	==	0xffff)
			continue;

		lpIF->m_iRealPlayerCount	=	0;
		lpIF->m_iAlivePlayerCount	=	0;

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
		{
			lpIF->m_pFloor[iFloor].updateGame();
			m_iRealPlayerCount			+=	lpIF->m_pFloor[iFloor].m_iRealPlayerCount;
			lpIF->m_iRealPlayerCount	+=	lpIF->m_pFloor[iFloor].m_iRealPlayerCount;
			lpIF->m_iAlivePlayerCount	+=	lpIF->m_pFloor[iFloor].m_iAlivePlayerCount;
		}
	}
}

void
cInstanceFieldManager::operateStoredPacket()
{
	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if (lpIF->m_wSerial	==	0xffff)
			continue;

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
			lpIF->m_pFloor[iFloor].operateStoredPacket();
	}
}

void
cInstanceFieldManager::operateOncePerSecondWork()
{
	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if (lpIF->m_wSerial	==	0xffff)
			continue;

		if (lpIF->update())
		{
			close(i);
			continue;
		}

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
			lpIF->m_pFloor[iFloor].operateOncePerSecondWork();
	}
}

void
cInstanceFieldManager::operateOncePer10SecondWork()
{
	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if (lpIF->m_wSerial	==	0xffff)
			continue;

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
			lpIF->m_pFloor[iFloor].operateOncePer10SecondWork();
	}
}

void
cInstanceFieldManager::operateOncePerMinuteWork()
{
	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if (lpIF->m_wSerial	==	0xffff)
			continue;

		if (lpIF->m_wUpkeepMinuteInValidPlayerStatus)
			lpIF->m_wUpkeepMinuteInValidPlayerStatus--;

		if (lpIF->m_wIsProgressEvent	==	FALSE)
		{
			if (lpIF->m_wUpkeepMinute)
				lpIF->m_wUpkeepMinute--;

			lpIF->m_wGenerateMinute++;
		}

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
			lpIF->m_pFloor[iFloor].operateOncePerMinuteWork();
	}
}

void
cInstanceFieldManager::operateOncePerHourWork()
{	
	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if (lpIF->m_wSerial	==	0xffff)
			continue;

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
			lpIF->m_pFloor[iFloor].operateOncePerHourWork();
	}
}

void
cInstanceFieldManager::sendPacketToAllOperator(char *_lpData,int _iLevel)
{
	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if (lpIF->m_wSerial	==	0xffff)
			continue;

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
			lpIF->m_pFloor[iFloor].sendPacketToAllOperator((ALL_MSG*)_lpData,_iLevel);;
	}
}

void
cInstanceFieldManager::sendPacketToAllPlayer(char *_lpData)
{
	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if (lpIF->m_wSerial	==	0xffff)
			continue;

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
			lpIF->m_pFloor[iFloor].addSendPacket((ALL_MSG*)_lpData,0xffff);
	}
}

void
cInstanceFieldManager::addSendEventMessage(char *_lpstrMessage,int _iType)
{
	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if	(lpIF->m_wSerial	==	0xffff)
			continue;

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
			lpIF->m_pFloor[iFloor].addSendEventMessage(_lpstrMessage,_iType);
	}
}

void
cInstanceFieldManager::resetReceivePacket()
{
	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if (lpIF->m_wSerial	==	0xffff)
			continue;

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
			lpIF->m_pFloor[iFloor].m_queReceivePacket.reset();
	}
}

int
cInstanceFieldManager::getRealPlayerCount()
{
	return	m_iRealPlayerCount;
}

void
cInstanceFieldManager::increasePlayerPlayTime()
{
	for (int i =1;i<dMAX_IF_COUNT;i++)
	{
		cInstanceField*	lpIF	=	&m_aInstanceField[i];

		if (lpIF->m_wSerial	==	0xffff)
			continue;

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
			lpIF->m_pFloor[iFloor].increasePlayerPlayTime();
	}
}