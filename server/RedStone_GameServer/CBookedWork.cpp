#include "CBookedWork.h"

CBW_Manager::CBW_Manager()
{
	reset();
}	//	CBW_Manager::CBW_Manager()

CBW_Manager::~CBW_Manager()
{
	reset();
}	//	CBW_Manager::~CBW_Manager()

BOOL
CBW_Manager::init()
{
	m_wBookedWorkCount	=	0;

	return	TRUE;
}	//	CBW_Manager::init()

void
CBW_Manager::reset()
{
	m_wBookedWorkCount	=	0;
}	//	CBW_Manager::reset()

void
CBW_Manager::close()
{
	m_wBookedWorkCount	=	0;
}	//	CBW_Manager::close()

//
//	예약
BOOL
CBW_Manager::bookingWork(UAllBookedWork *_lpWork)
{
	return	TRUE;
}	//	CBW_Manager::bookingWork(UAllBookedWork *_lpWork)
//	UAllBookedWork*		popBookedWork();	//	예약된 작업 하나씩 꺼냄
