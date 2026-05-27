#include "FriendsListUpdater.h"
#include "RSComm.h"

cFriendsListUpdater g_cFriendsListUpdater;
cFriendsListUpdater g_cGuildMemberListUpdater;		// 길드 멤버 ...
cFriendsListUpdater::cFriendsListUpdater()
{
	InitializeCriticalSection(&csEmerIdx);
	
	m_iNextIndex = 0;
	for(int i = dEMER_PROC_COUNT;i;)
	{	--i;
		m_aiEmerProcIndex[i] = -1;
	}
	m_iEmerCurIndex = -1;	//긴급 처리할 지금 인덱스
	m_iEmerNextIndex = 0;	//긴급 처리할 인덱스를 삽입할 위치
	m_iEmerRegCount = 0;	//긴급 처리할 인덱스의 수
}

cFriendsListUpdater::~cFriendsListUpdater()
{
	DeleteCriticalSection(&csEmerIdx);
}

BOOL	cFriendsListUpdater::GetNextProcIndex(int & _iIndex)	//	다음 처리할 인덱스를 가져온다.
{
	_iIndex = -1;
	if(m_iEmerRegCount)
	{
		CCritical CS(&csEmerIdx);
		{
		_iIndex = m_aiEmerProcIndex[m_iEmerCurIndex];
		m_aiEmerProcIndex[m_iEmerCurIndex] = -1;
		m_iEmerCurIndex++;
		if(m_iEmerCurIndex>=dEMER_PROC_COUNT)	m_iEmerCurIndex = 0;
		--m_iEmerRegCount;
		}
		return TRUE;
	}
	_iIndex = m_iNextIndex;
	m_iNextIndex++;
	if(m_iNextIndex>=dBC_MAX_USER_COUNT)	m_iNextIndex = 0;
	return FALSE;
}

void	cFriendsListUpdater::SetLoginUserIndex(int _iIndex)	//로그인을 처음한 유저의 인덱스를 세팅한다.
{
	if(_iIndex<0 || _iIndex>=dEMER_PROC_COUNT)	return;

	CCritical CS(&csEmerIdx);

	m_aiEmerProcIndex[m_iEmerNextIndex] = _iIndex;
	m_iEmerNextIndex++;
	if(m_iEmerCurIndex>=dEMER_PROC_COUNT)	m_iEmerCurIndex = 0;
	m_iEmerRegCount++;
}
