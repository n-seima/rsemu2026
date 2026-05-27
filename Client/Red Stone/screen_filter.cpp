#include	"screen_filter.h"
#include	"cDRAW.h"

void
cScreenFilter::init(int _iFPS)
{
	m_iFPS	=	_iFPS;
}

void
cScreenFilter::setFilter(WORD	_wDestColor,int _iDepth)
{
	m_wCurrentColor	=	_wDestColor;
	m_wCurrentDepth	=	_iDepth;
}

void
cScreenFilter::setWorkFilter(WORD	_wBeginColor,int _iBeginDepth,WORD _wEndColor,int _iEndDepth,int _iWorkTime,int _iLayer)
{
	m_wBeginColor	=	_wBeginColor;
	m_wBeginDepth	=	_iBeginDepth;
	m_wDestColor	=	_wEndColor;
	m_wEndDepth		=	_iEndDepth;
	m_iProcessFrame	=	0;
	m_iEndFrame		=	m_iFPS*_iWorkTime/100;
	m_wLayer		=	_iLayer;
}

void
cScreenFilter::operate()
{
	int	iDepthAlpha	=	m_wEndDepth-m_wBeginDepth;
	int	iDepth		=	0;
	
	if	(m_iEndFrame)
		iDepth	=	m_wBeginDepth+m_iProcessFrame*iDepthAlpha/m_iEndFrame;

	iDepth			=	max(iDepth,0);
	iDepth			=	min(iDepth,100);
	iDepth			=	32*iDepth/100;

	m_wCurrentDepth	=	iDepth;

	int	iProcessRate=	100;
	
	if	(m_iEndFrame)
		iProcessRate=	m_iProcessFrame*32/m_iEndFrame;

	m_wCurrentColor	=	GetMixColor(m_wBeginColor,m_wDestColor,32-iProcessRate,iProcessRate);

	m_iProcessFrame++;
	m_iProcessFrame	=	min(m_iProcessFrame,m_iEndFrame);
}