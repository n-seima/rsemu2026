#ifndef	_screen_focus_h
#define	_screen_focus_h

#include "SFC.h"

class	cScreenFocus
{
public:
	CPos			m_posBegin,m_posEnd;
	CPos			m_posCurrent;
	DWORD			m_dwBeginTime;
	WORD			m_wElapseTime;

	void			reset();
	BOOL			getCurrentFocus(CPos *_lpPos);
	void			update();
	void			setFocus(int _iX,int _iY,int _iDx,int _iDy,int _iElapseTime);
};

#endif