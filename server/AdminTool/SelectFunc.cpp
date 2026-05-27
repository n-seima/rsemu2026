#include "stdafx.h"
#include <algorithm>
#include "selectFunc.h"

void	cSelectFunc::init()
{
	m_unusedList.clear();
	m_usedList.clear();

	for(int i=0;i<eColumn_MainCount;i++)
	{
		m_unusedList.push_back(i);
	}
}

void	cSelectFunc::left(int _value)
{
	std::deque<int>::iterator it = find( m_usedList.begin(), m_usedList.end(), _value);
	if(it == m_usedList.end())	return;

	m_unusedList.push_back( (*it));
	m_usedList.erase( it );
}

void	cSelectFunc::right(int _value)
{
	std::deque<int>::iterator it = find( m_unusedList.begin(), m_unusedList.end(), _value);
	if(it == m_unusedList.end())	return;

	m_usedList.push_back( (*it));
	m_unusedList.erase( it );
}

void	cSelectFunc::addCond(int _value)
{
}

void	cSelectFunc::delCond(int _value)
{
}


void	cSelectFunc::select()
{
	char query[1024];

	sprintf( query, "SELECT ");
	//	검색할 내용..
//	strcat( " FROM AVATAR_CURRENT WHERE" );
	//	검색할 조건..

	//	쿼리 실행해서 결과를 ..
}
