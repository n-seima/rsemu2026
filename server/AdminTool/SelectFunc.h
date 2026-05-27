#pragma once 

#include <deque>

typedef enum
{
	eColumn_avaidx,
	eColumn_id,
	eColumn_name,
	eColumn_createDate,
	eColumn_lastConnectionDate,
	eColumn_job,
	eColumn_level,
	eColumn_isOperator,
	eColumn_guildindex,
	eColumn_guildRank,
	eColumn_rebirthCount,

	eColumn_MainCount,	//	DB에서 바로 검색 가능한 컬럼의 수

	//	이후 데이터는 로딩을 위해 바이너리 정보를 처리해야 하므로 검색결과의 개수가 많을 경우 
	//	검색시 많은 시간과 라이브 서버의 속도에 영향을 줄 수 있다. 
	//	검색시 Sleep을 사용할 것이므로 삭제하지 말고, 오래 걸려도 기다려주길..

	//	일단 추가 요청이 없으면 만들지는 않을 예정.
//	eColumn_,
}eColumn;

const char g_constColumnDesc[eColumn_MainCount][32] = { "인덱스", "계정", "이름", "생성일자", "마지막 접속일자", "직업", 
								   "레벨", "운영자여부", "길드인덱스", "길드 계급", "rebirth 횟수"};

const char g_constColumnName[eColumn_MainCount][32] = { "avaidx", "master", "name", "stardate", "lastdate", "job", 
								   "lev", "isOper", "guild", "guildRank", "rebirthCount"};

class cSelectFunc
{
public:
	std::deque<int>		m_unusedList, m_usedList;	//	검색안할 목록과 검색할 목록.
	std::deque<int>		m_conditionListForSearching;//	검색 조건

	inline DWORD	getSelectedCount()	{	return m_usedList.size();	}

	void	init();
	void	left(int _value);
	void	right(int _value);

	void	addCond(int _value);
	void	delCond(int _value);

	void	select();
};