#pragma once

#include <list>

class cSchedule
{
public:	//	const ... 
	enum
	{
		eChkType_Date,
		eChkType_Month,
		eChkType_Week,
		eChkType_Day,
	}eChkType;

public:	//	variable
	BOOL	m_isUsingTime;	//	타이머를 사용하지 않으면 수동으로 작업을 등록하고 그때 그때 실행한다는 이야기..

	int		m_iCheckType;	//	eChkType
	cpUTime	m_tiTime;		//	실행 시각
	short	m_sTimeInteval;	//	시간 간격으로 실행할 때.. 0이면 시간 간격이 아니다. 12를 넘어가면 의미가 없어진다. // 하루 중 특정 시간대만 돌리고 싶어질거 같은데..

public:	//	function
	BOOL	is
};