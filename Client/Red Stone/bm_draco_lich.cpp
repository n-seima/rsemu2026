#include	"bm_draco_lich.h"

cBM_DracoLichEvent	g_bmEventDracoLich;

cBM_DracoLichEvent::cBM_DracoLichEvent()
{
	m_pMovie	=	NULL;
}

cBM_DracoLichEvent::~cBM_DracoLichEvent()
{
	KILL(m_pMovie);
}

BOOL
cBM_DracoLichEvent::open()
{
	m_pMovie	=	new cMovie();
	
	if	(!m_pMovie->load("data/event/bme_draco_lich.dat"))
	{
		KILL(m_pMovie);
		
		return	FALSE;
	}
	
	return	TRUE;
}

void
cBM_DracoLichEvent::close()
{
	KILL(m_pMovie);
}

void
cBM_DracoLichEvent::draw()
{
	if	(!m_pMovie)
		return;
	
	m_pMovie->draw();
}

BOOL
cBM_DracoLichEvent::update()
{
	if	(!m_pMovie)
		return	FALSE;
	
	m_pMovie->operate();
	
	if	(m_pMovie->m_bIsEnd)
		g_bossMonsterEvent.close();
	
	return	TRUE;
}

BOOL
cBM_DracoLichEvent::isOpened()
{
	if	(m_pMovie)
		return	TRUE;
	
	return	FALSE;
}