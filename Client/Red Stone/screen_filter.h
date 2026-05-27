#ifndef	_screen_filter_h
#define	_screen_filter_h

#include	"SFC.h"

class	cScreenFilter
{
public:
	int		m_iFPS;
	int		m_iProcessFrame,m_iEndFrame;

	WORD	m_wBeginColor,m_wDestColor;
	WORD	m_wBeginDepth,m_wEndDepth;
	WORD	m_wOperateTime;

	WORD	m_wCurrentColor,m_wCurrentDepth;

	WORD	m_wLayer;

	void	init(int _iFPS);
	void	setFilter(WORD	_wDestColor,int _iDepth);
	void	setWorkFilter(WORD	_wBeginColor,int _iBeginDepth,WORD _wEndColor,int _iEndDepth,int _iWorkTime,int _iLayer);

	void	operate();
};

#endif