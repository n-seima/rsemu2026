#include	"bm_tiamath.h"

cBM_TiamathEvent	g_bmEventTiamath;

cBM_TiamathEvent::cBM_TiamathEvent()
{
	m_pMovie	=	NULL;
}

cBM_TiamathEvent::~cBM_TiamathEvent()
{
	KILL(m_pMovie);
}

BOOL
cBM_TiamathEvent::open()
{
	m_pMovie	=	new cMovie();

	if	(!m_pMovie->load("data/event/bme_tiamath.dat"))
	{
		KILL(m_pMovie);

		return	FALSE;
	}

	return	TRUE;
}

void
cBM_TiamathEvent::close()
{
	KILL(m_pMovie);
}

void
cBM_TiamathEvent::draw()
{
	if	(!m_pMovie)
		return;

	m_pMovie->draw();
}

BOOL
cBM_TiamathEvent::update()
{
	if	(!m_pMovie)
		return	FALSE;

	m_pMovie->operate();

	if	(m_pMovie->m_bIsEnd)
		g_bossMonsterEvent.close();

	return	TRUE;
}

BOOL
cBM_TiamathEvent::isOpened()
{
	if	(m_pMovie)
		return	TRUE;

	return	FALSE;
}