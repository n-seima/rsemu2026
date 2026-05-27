#pragma once

#include "userCustomDataDefine.h"

//	우선순위를 관리하는 테이블

//	이 사람의 최근 갱신시간. (class UTime)

#define dEMER_PROC_COUNT		1024

class cFriendsListUpdater
{
private:
	CRITICAL_SECTION	csEmerIdx;
	int		m_iNextIndex;	//	평소의 순차검색을 처리할 다음 인덱스를 가지고 있는다.
							//	client.h의  CClientManager가 관리하는 인덱스. 
							//	0 ~ dBC_MAX_USER_COUNT-1
	int		m_aiEmerProcIndex[dEMER_PROC_COUNT];//긴급 처리할 인덱스를 저장한다.
	int		m_iEmerCurIndex;	//긴급 처리할 지금 인덱스
	int		m_iEmerNextIndex;	//긴급 처리할 인덱스를 삽입할 위치
	int		m_iEmerRegCount;	//긴급 처리할 인덱스의 수

public:
	cFriendsListUpdater();
	~cFriendsListUpdater();


	BOOL	GetNextProcIndex(int & _iIndex);//다음 처리할 인덱스를 가져온다. 긴급일경우 TRUE를 반환
	void	SetLoginUserIndex(int _iIndex);	//로그인을 처음한 유저의 인덱스를 세팅한다.
											//긴급처리에 삽입한다.
};

extern cFriendsListUpdater g_cFriendsListUpdater;
extern cFriendsListUpdater g_cGuildMemberListUpdater;		// 길드 멤버 ... // 추가