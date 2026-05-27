#include	"movie.h"
#include	"cDECODER.h"
#include	"cDRAW.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cMovieEvent::cMovieEvent()
{
	m_wWorkCount=	0;
	m_pWork		=	NULL;
}

cMovieEvent::~cMovieEvent()
{
	pKILL(m_pWork);
}

BOOL
cMovieEvent::init(int _iWorkCount)
{
	__destroy();

	m_wWorkCount	=	_iWorkCount;
	m_pWork			=	new cMovieFrame [_iWorkCount];

	if	(!m_pWork)
		return	FALSE;

	return	TRUE;
}

void
cMovieEvent::__destroy()
{
	m_wWorkCount=	0;
	pKILL(m_pWork);
}

void
cMovieEvent::copy(cMovieEvent *_lpSrc)
{
	__destroy();

	init(_lpSrc->m_wWorkCount);

	for	(int i=_lpSrc->m_wWorkCount;i;)
	{
		--i;
		m_pWork[i].copy(&_lpSrc->m_pWork[i]);
	}

	m_wFrame	=	_lpSrc->m_wFrame;
}

void
cMovieEvent::operate()
{
	for	(int i=0;i<m_wWorkCount;i++)
	{
		cMovieFrame	*lpFrame	=	&m_pWork[i];

		lpFrame->operate();
	}
}