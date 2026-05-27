#include	"movie.h"
#include	"cDECODER.h"
#include	"cDRAW.h"
#include	"cMap.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cMovie::cMovie()
{
	m_pEvents			=	NULL;
	m_wLastEventIndex	=	0;
	m_iFrame			=	0;
	m_bIsEnd			=	FALSE;
}

cMovie::~cMovie()
{
	__destroy();
}

void
cMovie::__destroy()
{
	m_wLastEventIndex	=	0;
	m_wEventCount		=	0;
	pKILL(m_pEvents);
}

void
cMovie::init()
{
	m_iFrame	=	0;
	m_bIsEnd	=	FALSE;

	m_anmPlayer.setGameFPS(dSYNC_FPS);
}

void
cMovie::operate()
{
	s_lpMovie	=	this;
	
	for	(int i=m_wLastEventIndex;i<m_wEventCount;i++)
	{
		if	(m_pEvents[i].m_wFrame	!=	m_iFrame)
			break;

		m_pEvents[i].operate();

		m_wLastEventIndex++;
	}

	m_anmPlayer.operate();
	
	m_iFrame++;
}

void
cMovie::draw()
{
	m_anmPlayer.draw(GetCorrectViewXPos(),GetCorrectViewYPos());
}