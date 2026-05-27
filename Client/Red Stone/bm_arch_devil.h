#ifndef	_boss_monster_event_arch_devil_h
#define	_boss_monster_event_arch_devil_h

#include	"bm_event.h"

class	cBM_ArchDevilEvent	: public CGamePlay,public cBM_EventCommon
{
public:
	cBM_ArchDevilEvent();
	~cBM_ArchDevilEvent();
	
	BOOL					open();
	void					close();

	void					draw();

	BOOL					update();

	BOOL					isOpened();
};

extern	cBM_ArchDevilEvent	g_bmEventArchDevil;

#endif