#pragma once

#include "SFC.H"
#include "cPACKET_WORLDSERVER.H"
#include "eventDefine.h"

class	cSOCKET;

class	CEventTimeManager
{
public:
	int					m_iCount;
	cEventTimeDefine	m_aEventTimeList[dMAX_EVENT_TIME_COUNT];

	CEventTimeManager();

	void	updateData(int _iCount,cEventTimeDefine *_lpData);
	void	sendEventData(cSOCKET *_lpTargetServer);

	void	save();
	void	load();
};

extern	CEventTimeManager	g_eventTimeManager;