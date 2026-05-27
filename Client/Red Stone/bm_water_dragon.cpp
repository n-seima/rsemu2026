#include	"bm_water_dragon.h"

cBM_WaterDragonEvent	g_bmEventWaterDragon;

cBM_WaterDragonEvent::cBM_WaterDragonEvent()
{
	m_pMovie	=	NULL;
}

cBM_WaterDragonEvent::~cBM_WaterDragonEvent()
{
	KILL(m_pMovie);
}

BOOL
cBM_WaterDragonEvent::open()
{
	m_pMovie	=	new cMovie();
	
	if	(!m_pMovie->load("data/event/bme_water_dragon.dat"))
	{
		KILL(m_pMovie);
		
		return	FALSE;
	}
	
	return	TRUE;
}

void
cBM_WaterDragonEvent::close()
{
	KILL(m_pMovie);
}

void
cBM_WaterDragonEvent::draw()
{
	if	(!m_pMovie)
		return;
	
	m_pMovie->draw();
}

BOOL
cBM_WaterDragonEvent::update()
{
	if	(!m_pMovie)
		return	FALSE;
	
	m_pMovie->operate();
	
	if	(m_pMovie->m_bIsEnd)
		g_bossMonsterEvent.close();
	
	return	TRUE;
}

BOOL
cBM_WaterDragonEvent::isOpened()
{
	if	(m_pMovie)
		return	TRUE;
	
	return	FALSE;
}