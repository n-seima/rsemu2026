#ifndef _classPLAYER_BUFF_
#define _classPLAYER_BUFF_

#include <winsock2.h>
#include <windows.h>
#include <mmsystem.h>
#include <deque>
#include <algorithm>
#include "definePacketData.h"
#include "RSComm.h"
#include "cPLAYER_DATA.h"
#include "STACK.H"

#define	dPLAYER_BUFF_SIZE		300000

extern	int g_isUsePlayerBuffer;

class	CCharacterInfo
{
public:
	DWORD			m_dwIndex;
	cAVATAR_INFO	m_avatarInfo;
};

class	CHashIndex
{
public:
	DWORD		m_dwHashCode;
	DWORD		m_dwIndex;
};

class	CPlayerBuff
{
public:
	DWORD				m_dwSerial;
	DWORD				m_dwIDHashCode;
	int					m_iFriendCount;
	char				m_strId[dID_LENGTH];
	UTime				m_utLastAccessTime;

	CCharacterInfo		m_aAvatarInfo[4];
//	char				m_strFriendList[dMAX_FRIEND_COUNT][dNAME_LENGTH];

	void				reset();
	BOOL				addFriend(char *_lpstrFriend);
	BOOL				removeFriend(char *_lpstrFriend);
	DWORD				getFriendCheckSum();
};

class	CPlayerBuffManager
{
	CHashIndex			m_aHashTable[10000][256];

	CTimeInfo			m_time;
	UTime				m_uTime;
	int					m_iRookie,m_iCount;
	CPlayerBuff			m_aBuffer[dPLAYER_BUFF_SIZE];
	CRITICAL_SECTION	m_csWork;
public:
	void				init();
	void				close();
	void				initForSharedMemoryManager();
	void				closeForSharedMemoryManager();

	CPlayerBuff*		getPlayer(char* _lpstrId);
	CPlayerBuff*		getPlayer(DWORD _dwHashCode,char* _lpstrId);
	BOOL				addPlayer(char* _lpstrId,CCharacterInfo *_lpCharacterInfo);
	void				saveData();
	BOOL				getAvatarInfo(char* _lpstrId,cAVATAR_INFO *_lpAvatarInfo);
	UTime*				getTime();

	void				update();
	inline	void		lock()		{EnterCriticalSection(&m_csWork);}
	inline	void		unlock()	{LeaveCriticalSection(&m_csWork);}
};

extern	CPlayerBuffManager	*g_lpAbm;

#endif