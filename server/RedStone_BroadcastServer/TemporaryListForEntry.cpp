// TemporaryListForEntry.cpp: implementation of the CTemporaryListForEntry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RedStone_BroadCastServer.h"
#include "TemporaryListForEntry.h"
#include "cSRVUTIL.h"
#include <mmsystem.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTemporaryListForEntry::CTemporaryListForEntry()
{
	InitializeCriticalSection(&csTL);
	Init();
}

CTemporaryListForEntry::~CTemporaryListForEntry()
{
	DeleteCriticalSection(&csTL);
}

WORD CTemporaryListForEntry::NextNext()
{
	for(int _next = m_wNext;_next<dBC_MAX_USER_COUNT;_next++)
	{	if(stData[_next].wSerial== 0xffff)
		{
			m_wNext = _next;
			return m_wNext;
		}
	}
	int end = m_wNext;
	for(_next = 0;_next<end;_next++)
	{	if(stData[_next].wSerial == 0xffff)
		{	m_wNext = _next;
			return m_wNext;
		}
	}
	m_wNext = 0xffff;
	return 0xffff;
}

void CTemporaryListForEntry::Init()	//	DATA 초기화
{
	m_wNext = m_wCount = 0;
	for(int i=0;i<dBC_MAX_USER_COUNT;i++)
	{
		memset(&stData[i],0xff,sizeof(stCLIENTDATA));
		stData[i].wOper = 0;
		stData[i].dwRegistTime = 0;
		stData[i].timeRegistedTime.m_dwValue = 0;
		stData[i].cData.reset();
	}
}

WORD 
CTemporaryListForEntry::Add(stCLIENTDATA _stData)	//	유저를 추가하고 시리얼을 돌려준다.
{	
	CCritical	CS(&csTL);

	WORD	wResult = 0xffff;

	if	(m_wCount >= dBC_MAX_USER_COUNT)
		return wResult;
	if	(m_wNext == 0xffff)
		return wResult;

	wResult = m_wNext;

	memcpy(&stData[wResult],&_stData,sizeof(stCLIENTDATA));
	stData[wResult].wSerial = wResult;
	stData[wResult].dwHashNameCode = cSRVUTIL::GetHashCode((BYTE *)stData[wResult].strName);
	stData[wResult].dwHashIdCode = cSRVUTIL::GetHashCode((BYTE *)stData[wResult].strId);
	stData[wResult].dwRegistTime = timeGetTime();
	cSRVUTIL::GetUTime(&stData[wResult].timeRegistedTime);
	m_wCount++;
	NextNext();

	return wResult;
}

void CTemporaryListForEntry::Del(WORD _wSerial)	//	해당 유저를 삭제한다.
{
	CCritical	CS(&csTL);

	if(_wSerial>=dBC_MAX_USER_COUNT)		return;
	if(stData[_wSerial].wSerial == 0xffff)		return;

	memset(&stData[_wSerial],0xff,sizeof(stCLIENTDATA));
	if(m_wCount>0)
		m_wCount--;
	
	if(m_wNext == 0xffff)	m_wNext = _wSerial;
}

stCLIENTDATA * CTemporaryListForEntry::GetDataPnt(char * _strId, BOOL _isSearchByName)	//	id로 CClient 포인터 얻기 flag=1-> Name으로  
{
	DWORD	dwHashCode = cSRVUTIL::GetHashCode((BYTE *)_strId);
	for(int i=0;i<dBC_MAX_USER_COUNT;i++)
	{
		if(stData[i].wSerial>=0xffff)
			continue;

		if(!_isSearchByName)
		{
			if(stData[i].dwHashIdCode == dwHashCode && STRICMP(stData[i].strId,_strId)==0)
				return &stData[i];
		}else{
			if(stData[i].dwHashNameCode == dwHashCode && STRICMP(stData[i].strName,_strId)==0)
				return &stData[i];
		}
	}

	return NULL;
}

stCLIENTDATA * CTemporaryListForEntry::GetDataPnt(WORD _wSerial)	//	idx로 CClient 포인터 얻기
{
	if(_wSerial>=dBC_MAX_USER_COUNT)		return NULL;

	return &stData[_wSerial];
}
