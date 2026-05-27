#ifndef	_boss_monster_event_sage_h
#define	_boss_monster_event_sage_h

#include	"bm_event.h"

class	cBM_SageEvent	: public CGamePlay,public cBM_EventCommon
{
public:

	cBM_SageEvent();
	~cBM_SageEvent();
	
	BOOL					open();
	void					close();

	void					draw();

	BOOL					update();

	BOOL					isOpened();
};

extern	cBM_SageEvent	g_bmEventSage;

#endif