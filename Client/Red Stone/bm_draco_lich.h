#ifndef	_boss_monster_event_draco_lich_h
#define	_boss_monster_event_draco_lich_h

#include	"bm_event.h"

class	cBM_DracoLichEvent	: public CGamePlay,public cBM_EventCommon
{
public:
	
	cBM_DracoLichEvent();
	~cBM_DracoLichEvent();
	
	BOOL					open();
	void					close();

	void					draw();

	BOOL					update();

	BOOL					isOpened();
};

extern	cBM_DracoLichEvent	g_bmEventDracoLich;

#endif