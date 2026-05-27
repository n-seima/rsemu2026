// TemporaryListForEntry.h: interface for the CTemporaryListForEntry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMPORARYLISTFORENTRY_H__4470FD59_90F2_4793_A48B_6472C79C9C25__INCLUDED_)
#define AFX_TEMPORARYLISTFORENTRY_H__4470FD59_90F2_4793_A48B_6472C79C9C25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RSComm.h"
#include "userCustomDataDefine.h"
#include "cSRVUTIL.h"


typedef struct 
{
	char	strId[dID_LENGTH];		//	이하는 정상유저인지 확인을 위해
	char	strName[dNAME_LENGTH];		
	WORD	wGuildSerial;			
	WORD	wGuildRank;	
	WORD	wCurrentLevel;
	WORD	wCurrentField;			
	WORD	wAvatarIndex;			//	아바타 인덱스	
	WORD	wPartySerial;			//	파티 시리얼
	WORD	wOper;					//	운영자 레벨, 만약 0xffff일 경우, 재접속한 유저 정보라고 생각하고, Oper를 나중에 0으로 초기화 시킨다.
	WORD	wMyGSSerial;			//	이사람이 현재 속한 게임 서버.
	WORD	wGSSerial;				//	게임서버에 돌려줄 Serial
	WORD	wSerial;				//	자신에게 할당된 시리얼
	DWORD	dwHashIdCode;
	DWORD	dwHashNameCode;
	DWORD	dwRegistTime;			//	게임서버로부터 등록된 시간.
	cpUTime	timeRegistedTime;		//	게임서버로부터 등록된 시간.

	int		iDBIndex;
	cUserCustomDataDefine	cData;
}stCLIENTDATA;


class CTemporaryListForEntry  
{
protected:
	CRITICAL_SECTION	csTL;
	stCLIENTDATA		stData[dBC_MAX_USER_COUNT];
	WORD				m_wNext,m_wCount;

public:
	CTemporaryListForEntry();
	virtual ~CTemporaryListForEntry();

	inline int			Size(){	return m_wCount;	}

	WORD				NextNext();
	void				Init();				//	클라이언트 정보 초기화
	WORD				Add(stCLIENTDATA _stData);	//	유저를 추가하고 시리얼을 돌려준다.
	void				Del(WORD _wSerial);	//	해당 유저를 삭제한다.

	stCLIENTDATA	*	GetDataPnt(char * _strId, BOOL _isSearchByName=0);	//	id로 CClient 포인터 얻기 flag=1-> Name으로  
	stCLIENTDATA	*	GetDataPnt(WORD _wSerial);							//	idx로 CClient 포인터 얻기
};

extern	CTemporaryListForEntry cTempList;

#endif // !defined(AFX_TEMPORARYLISTFORENTRY_H__4470FD59_90F2_4793_A48B_6472C79C9C25__INCLUDED_)
