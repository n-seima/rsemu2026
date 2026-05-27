#ifndef _classSPLINE_H
#define _classSPLINE_H

#include "cDRAW.H"

class cSPLINE
{
public:

	POINT			m_pos;
	cRECT			m_rect;
	float			m_fDelta,m_fT,m_aValue[4][2];
	WORD			m_wRegularRandomPattern,m_wRegularRandomPatternCounter;

					cSPLINE();

	inline float	blend1(float _fT,float _fValue);
	inline float	blend2(float _fT,float _fValue);
	inline float	blend3(float _fT,float _fValue);
	inline float	blend4(float _fT,float _fValue);
	void			init(int x1,int y1,int x2,int y2,float _fDelta=0.05);
	void			setRegularRandomPattern(int _iPattern);
	void			update();
};

#endif