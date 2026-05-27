#include	"bm_event.h"
#include	"bm_sage_event.h"
#include	"bm_tiamath.h"
#include	"bm_arch_devil.h"
#include	"bm_draco_lich.h"
#include	"bm_water_dragon.h"

cBM_Event	g_bossMonsterEvent;	//	보스 몬스터 이벤트 공통

cBM_Event::cBM_Event()
{
	m_lpCurrentEvent	=	NULL;
}

cBM_Event::~cBM_Event()
{
	g_bmEventSage.close();
}

BOOL
cBM_Event::buildScript()
{
	BOOL	bIsSuccess	=	TRUE;
	cMovie	movie;

	cFOLDER::Create("data/event");
	
	if	(movie.loadScript("dev Data/scripter/event/bme_sage.txt",dSYNC_FPS))
		movie.save("Data/event/bme_sage.dat");
	else
		bIsSuccess	=	FALSE;

	if	(movie.loadScript("dev Data/scripter/event/bme_arch_devil.txt",dSYNC_FPS))
		movie.save("Data/event/bme_arch_devil.dat");
	else
		bIsSuccess	=	FALSE;

	if	(movie.loadScript("dev Data/scripter/event/bme_tiamath.txt",dSYNC_FPS))
		movie.save("Data/event/bme_tiamath.dat");
	else
		bIsSuccess	=	FALSE;
	
	if	(movie.loadScript("dev Data/scripter/event/bme_water_dragon.txt",dSYNC_FPS))
		movie.save("Data/event/bme_water_dragon.dat");
	else
		bIsSuccess	=	FALSE;

	if	(movie.loadScript("dev Data/scripter/event/bme_draco_lich.txt",dSYNC_FPS))
		movie.save("Data/event/bme_draco_lich.dat");
	else
		bIsSuccess	=	FALSE;

	return	bIsSuccess;
}

BOOL
cBM_Event::beginBossBattleEvent(int _iBoss,int _iBeginEnd)
{
	switch(_iBoss)
	{
		case	eBoss_Sage			:
			m_lpCurrentEvent	=	&g_bmEventSage;
			break;
		case	eBoss_Tiamath		:
			m_lpCurrentEvent	=	&g_bmEventTiamath;
			break;
		case	eBoss_WaterDragon	:
			m_lpCurrentEvent	=	&g_bmEventWaterDragon;
			break;
		case	eBoss_ArchDevil		:
			m_lpCurrentEvent	=	&g_bmEventArchDevil;
			break;
		case	eBoss_DracoLich		:
			m_lpCurrentEvent	=	&g_bmEventDracoLich;
			break;
	}
	if	(!isAvail() || !m_lpCurrentEvent->open())
	{
		close();

		return	FALSE;
	}

	return	TRUE;
}

void
cBM_Event::close()
{
	if	(!isAvail())
		return;

	m_lpCurrentEvent->close();
	m_lpCurrentEvent	=	NULL;
}

void
cBM_Event::draw()
{
	if	(!isAvail())
		return;

	m_lpCurrentEvent->draw();

#ifdef	_DEBUG
	s_bText.put(5,50,WHITE,"Time %d",m_lpCurrentEvent->m_pMovie->m_iFrame*100/dSYNC_FPS);
#endif
}

void
cBM_Event::update()
{
	if	(!isAvail())
		return;

	m_lpCurrentEvent->update();
}