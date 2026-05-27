#include	"screen_focus.h"

void
cScreenFocus::reset()
{
	m_posBegin.x	=	-1;
}

BOOL
cScreenFocus::getCurrentFocus(CPos *_lpPos)
{
	if	(m_posBegin.x	==	-1)
		return	FALSE;

	_lpPos->x	=	m_posCurrent.x;
	_lpPos->y	=	m_posCurrent.y;

	return	TRUE;
}

void
cScreenFocus::update()
{
	if	(m_posBegin.x	==	-1)
		return;

	if	(m_wElapseTime	==	0)
		return;

	int	iDX			=	m_posEnd.x	-	m_posBegin.x;
	int	iDY			=	m_posEnd.y	-	m_posBegin.y;

	int	iSpentTime	=	timeGetTime()-m_dwBeginTime;
	int	iProcessRate=	iSpentTime*100/m_wElapseTime;

	iProcessRate	=	min(iProcessRate,100);

	m_posCurrent.x	=	m_posBegin.x+iDX*iProcessRate/100;
	m_posCurrent.y	=	m_posBegin.y+iDY*iProcessRate/100;
}

void
cScreenFocus::setFocus(int _iX,int _iY,int _iDx,int _iDy,int _iElapseTime)
{
	m_posBegin.x	=	_iX;
	m_posBegin.y	=	_iY;
	m_posEnd.x		=	_iDx;
	m_posEnd.y		=	_iDy;

	m_wElapseTime	=	_iElapseTime*10;

	if	(_iElapseTime	==	0)
	{
		m_posCurrent.x	=	_iDx;
		m_posCurrent.y	=	_iDy;
	}

	m_dwBeginTime	=	timeGetTime();
}