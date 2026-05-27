// ChattingDataBase.h: interface for the CChattingDataBase class.
#ifndef __CCHATTINGDATABASE_H
#define __CCHATTINGDATABASE_H

#pragma warning( push )
#pragma warning( disable : 4786 )

#include "cSRVUTIL.h"
#include "cPACKET_BCSERVER.h"
#include "cADO_BASE.h"
#include <deque>

using namespace std;

class CChattingDataBase	: public cADO_BASE
{
public:
	CChattingDataBase();
	virtual ~CChattingDataBase();

	inline int		getListCount(){ return m_stdList.size(); }
	inline DWORD	getAllCount() { return m_dwTotalChattingCount; }
	inline void		incTotalCount(){	++m_dwTotalChattingCount;	}

	void	addData(CChattingLog * _data);//채팅 데이터를 리스트에 추가한다.
	BOOL	add(void *_lpSender,void *_lpReceiver,int _iChatType,char *_lpstrMessage);
	int		sendData();	//	리스트에 추가된 정보를 DB로 보낸다.

private:
	CRITICAL_SECTION		m_csList;
	DWORD					m_dwTotalChattingCount;//지금까지 축척된 채팅 총 카운트

	deque<CChattingLog>		m_stdList;//전달된 유저들의 채팅 데이터를 저장할 곳.
};

extern	CChattingDataBase	g_ChattingDataBase;

#pragma warning( pop )

#endif // __CCHATTINGDATABASE_H
