#include "CPlayerBuff.h"

CPlayerBuffManager	*g_lpAbm	=	NULL;

//
//	초기화
void
CPlayerBuff::reset()
{
	m_iFriendCount				=	0;
	m_dwSerial					=	0xffffffff;
	m_dwIDHashCode				=	0xffffffff;
	m_utLastAccessTime.m_dwValue=	0;
/*
	for (int i=0;i<dMAX_FRIEND_COUNT;i++)
		memset(m_strFriendList[i],0,dNAME_LENGTH);
		*/
}	//	CPlayerBuff::reset()

//
//	친구 추가
BOOL
CPlayerBuff::addFriend(char *_lpstrFriend)
{
/*	for (int i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		if (m_strFriendList[i][0]	==	NULL)
		{
			memset(m_strFriendList[i],0,dNAME_LENGTH);
			strcpy(m_strFriendList[i],_lpstrFriend);

			return	TRUE;
		}
	}
*/
	return	FALSE;
}	//	CPlayerBuff::addFriend(char *_lpstrFriend)

//
//	친구 제거
BOOL
CPlayerBuff::removeFriend(char *_lpstrFriend)
{/*
	for (int i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		if (STRICMP(m_strFriendList[i],_lpstrFriend) == 0)
		{
			memset(m_strFriendList[i],0,dNAME_LENGTH);

			return	TRUE;
		}
	}
*/
	return	FALSE;
}	//	CPlayerBuff::removeFriend(char *_lpstrFriend)

//
//	친구 첵섬 구하기
DWORD
CPlayerBuff::getFriendCheckSum()
{
	/*
	DWORD	dwCheckSum	=	0;
	BYTE	*lpBuffer	=	(BYTE *)m_strFriendList;

	for (int i=0;i<sizeof(m_strFriendList);i++)
		dwCheckSum	+=	lpBuffer[i];

	return	dwCheckSum;
	*/

	return	0;
}	//	CPlayerBuff::getFriendCheckSum()

void
CPlayerBuffManager::init()
{
	InitializeCriticalSection(&m_csWork);

}	//	CPlayerBuffManager::init()

void
CPlayerBuffManager::close()
{
	DeleteCriticalSection(&m_csWork);
}	//	CPlayerBuffManager::init()

void
CPlayerBuffManager::initForSharedMemoryManager()
{
	m_iRookie	=	0;
	m_iCount	=	0;

	for (int i=0;i<dPLAYER_BUFF_SIZE;i++)
		m_aBuffer[i].reset();

	memset(m_aHashTable,0xff,sizeof(m_aHashTable));
}	//	CPlayerBuffManager::init()

void
CPlayerBuffManager::closeForSharedMemoryManager()
{
}	//	CPlayerBuffManager::init()

//
//
CPlayerBuff*
CPlayerBuffManager::getPlayer(char* _lpstrId)
{
	DWORD	dwHashCode	=	GetHashCode((BYTE *)_lpstrId);

	for (int i=0,iCount = 0;i<dPLAYER_BUFF_SIZE && iCount;i++)
	{
		if (m_aBuffer[i].m_dwIDHashCode	==	dwHashCode)
		{
			if (STRICMP(m_aBuffer[i].m_strId,_lpstrId) == 0)
				return	&m_aBuffer[i];
		}
	}

	return	NULL;
}	//	CPlayerBuffManager::getPlayer(char* _lpstrId)

//
//
CPlayerBuff*
CPlayerBuffManager::getPlayer(DWORD _dwHashCode,char* _lpstrId)
{
	for (int i=0,iCount = 0;i<dPLAYER_BUFF_SIZE && iCount;i++)
	{
		if (m_aBuffer[i].m_dwIDHashCode	==	_dwHashCode)
		{
			if (STRICMP(m_aBuffer[i].m_strId,_lpstrId) == 0)
				return	&m_aBuffer[i];
		}
	}

	return	NULL;
}	//	CPlayerBuffManager::getPlayer(DWORD _dwHashCode,char* _lpstrId)

UTime*				
CPlayerBuffManager::getTime()
{
	return &m_uTime;
}


//
//	아바타 정보 얻어오기
BOOL
CPlayerBuffManager::getAvatarInfo(char* _lpstrId,cAVATAR_INFO *_lpAvatarInfo)
{
	lock();

	CPlayerBuff*	lpPlayer	=	getPlayer(_lpstrId);

	if (!lpPlayer)
	{
		unlock();

		return	FALSE;
	}

	lpPlayer->m_utLastAccessTime.m_dwValue	=	m_uTime.m_dwValue;

	memcpy(_lpAvatarInfo,lpPlayer->m_aAvatarInfo,sizeof(lpPlayer->m_aAvatarInfo));

	unlock();

	return	TRUE;
}	//	CPlayerBuffManager::getAvatarInfo(char* _lpstrId,cAVATAR_INFO *_lpAvatarInfo)

//
//
BOOL
CPlayerBuffManager::addPlayer(char* _lpstrId,CCharacterInfo *_lpAvatarInfo)
{
	lock();

	DWORD		dwIDHachCode=	GetHashCode((BYTE *)_lpstrId);
	CPlayerBuff	*lpPlayer	=	getPlayer(dwIDHachCode,_lpstrId);

	if (lpPlayer)
	{
		lpPlayer->m_utLastAccessTime.m_dwValue	=	m_uTime.m_dwValue;
		memcpy(lpPlayer->m_aAvatarInfo,_lpAvatarInfo,sizeof(lpPlayer->m_aAvatarInfo));

		unlock();

		return	FALSE;
	}

	lpPlayer								=	&m_aBuffer[m_iCount];
	lpPlayer->m_utLastAccessTime.m_dwValue	=	m_uTime.m_dwValue;
	lpPlayer->m_dwIDHashCode				=	dwIDHachCode;

	m_iCount++;

	if (m_iCount	>=	dPLAYER_BUFF_SIZE)	m_iCount	=	0;

	memcpy(lpPlayer->m_aAvatarInfo,_lpAvatarInfo,sizeof(m_aBuffer[m_iRookie].m_aAvatarInfo));

	unlock();

	return	TRUE;
}	//	CPlayerBuffManager::addPlayer(DWORD _dwSerial,char* _lpstrId,CCharacterInfo *_lpCharacterInfo)

//
//
void
CPlayerBuffManager::update()
{
	lock();

	m_time.update();

	m_uTime.year	=	m_time.m_wYear-2000;
	m_uTime.month	=	m_time.m_wMonth;
	m_uTime.day		=	m_time.m_wDay;
	m_uTime.hour	=	m_time.m_wHour;
	m_uTime.minute	=	m_time.m_wMinute;
	m_uTime.second	=	m_time.m_wSecond;

	unlock();
}	//	CPlayerBuffManager::update()