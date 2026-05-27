#pragma once

#include	"SFC.h"


class	cEventManager
{
public:
	int			m_iUpdateWeeklyRankingDay;
	CTimeInfo	m_timeLastUpdate;

	cEventManager();

	void		sendResetWeeklyDuelRecordMessage();

	void		update();
};

extern	cEventManager	*g_pEventManager;