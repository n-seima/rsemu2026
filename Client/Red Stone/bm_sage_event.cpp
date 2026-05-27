#include	"bm_sage_event.h"

cBM_SageEvent	g_bmEventSage;

cBM_SageEvent::cBM_SageEvent()
{
	m_pMovie	=	NULL;
}

cBM_SageEvent::~cBM_SageEvent()
{
	KILL(m_pMovie);
}

BOOL
cBM_SageEvent::open()
{
	m_pMovie	=	new cMovie();

	if	(!m_pMovie || !m_pMovie->load("data/event/bme_sage.dat"))
	{
		KILL(m_pMovie);

		return	FALSE;
	}

	return	TRUE;
}

void
cBM_SageEvent::close()
{
	KILL(m_pMovie);
}

void
cBM_SageEvent::draw()
{
	if	(!m_pMovie)
		return;

	m_pMovie->draw();
}

BOOL
cBM_SageEvent::update()
{
	if	(!m_pMovie)
		return	FALSE;

	m_pMovie->operate();

	if	(m_pMovie->m_bIsEnd)
		g_bossMonsterEvent.close();

	return	TRUE;
}

BOOL
cBM_SageEvent::isOpened()
{
	if	(m_pMovie)
		return	TRUE;

	return	FALSE;
}