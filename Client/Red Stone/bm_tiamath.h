#ifndef	_boss_monster_event_tiamath_h
#define	_boss_monster_event_tiamath_h

#include	"bm_event.h"

class	cBM_TiamathEvent	: public CGamePlay,public cBM_EventCommon
{
public:

	cBM_TiamathEvent();
	~cBM_TiamathEvent();
	
	BOOL					open();
	void					close();

	void					draw();

	BOOL					update();

	BOOL					isOpened();
};

extern	cBM_TiamathEvent	g_bmEventTiamath;

#endif