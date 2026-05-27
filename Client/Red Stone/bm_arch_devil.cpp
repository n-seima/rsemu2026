#include	"bm_arch_devil.h"

cBM_ArchDevilEvent	g_bmEventArchDevil;

cBM_ArchDevilEvent::cBM_ArchDevilEvent()
{
	m_pMovie	=	NULL;
}

cBM_ArchDevilEvent::~cBM_ArchDevilEvent()
{
	KILL(m_pMovie);
}

BOOL
cBM_ArchDevilEvent::open()
{
	m_pMovie	=	new cMovie();

	if	(!m_pMovie->load("data/event/bme_arch_devil.dat"))
	{
		KILL(m_pMovie);

		return	FALSE;
	}

	return	TRUE;
}

void
cBM_ArchDevilEvent::close()
{
	KILL(m_pMovie);
}

void
cBM_ArchDevilEvent::draw()
{
	if	(!m_pMovie)
		return;

	m_pMovie->draw();
}

BOOL
cBM_ArchDevilEvent::update()
{
	if	(!m_pMovie)
		return	FALSE;

	m_pMovie->operate();

	if	(m_pMovie->m_bIsEnd)
		g_bossMonsterEvent.close();

	return	TRUE;
}

BOOL
cBM_ArchDevilEvent::isOpened()
{
	if	(m_pMovie)
		return	TRUE;

	return	FALSE;
}