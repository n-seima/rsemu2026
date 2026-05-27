#ifndef	_boss_monster_event_water_dragon_h
#define	_boss_monster_event_water_dragon_h

#include	"bm_event.h"

class	cBM_WaterDragonEvent	: public CGamePlay,public cBM_EventCommon
{
public:

	cBM_WaterDragonEvent();
	~cBM_WaterDragonEvent();
	
	BOOL					open();
	void					close();
	
	void					draw();
	
	BOOL					update();
	
	BOOL					isOpened();

};

extern	cBM_WaterDragonEvent	g_bmEventWaterDragon;

#endif