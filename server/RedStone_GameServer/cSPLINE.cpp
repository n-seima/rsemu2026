#include "cSPLINE.H"
#include "regular_random.H"

cSPLINE::cSPLINE()
{
	m_fT	=	1.0;
	m_wRegularRandomPattern	=	0xffff;
}

void
cSPLINE::setRegularRandomPattern(int _iPattern)
{
	m_wRegularRandomPattern			=	_iPattern;
	m_wRegularRandomPatternCounter	=	0;
}

inline float
cSPLINE::blend1(float _fT,float _fValue)
{
	return	(float) (_fT*_fT*_fT/ 6.)*_fValue;
}	//	cSPLINE::blend(float _fT,int _iIndex)

inline float
cSPLINE::blend2(float _fT,float _fValue)
{
	return	(float)((-3*_fT*_fT*_fT+ 3*_fT*_fT + 3*_fT + 1) /6.)*_fValue;
}	//	cSPLINE::blend(float _fT,int _iIndex)

inline float
cSPLINE::blend3(float _fT,float _fValue)
{
	return	(float)((3*_fT*_fT*_fT - 6*_fT*_fT + 4        ) /6.)*_fValue;
}	//	cSPLINE::blend(float _fT,int _iIndex)

inline float
cSPLINE::blend4(float _fT,float _fValue)
{
	return	(float)((-_fT*_fT*_fT  + 3*_fT*_fT - 3*_fT +1 ) /6.)*_fValue;
}	//	cSPLINE::blend(float _fT,int _iIndex)

void
cSPLINE::init(int x1,int y1,int x2,int y2,float _fDelta)
{
	m_fT					=	1.0;
	m_fDelta				=	_fDelta;

	m_rect.Set(x1,y1,x2,y2);

	for (int i=0;i<4;i++)
	{
		int	iValue1,iValue2;

		if	(m_wRegularRandomPattern	==	0xffff)
		{
			iValue1	=	random(m_rect.x2-m_rect.x1);
			iValue2	=	random(m_rect.y2-m_rect.y1);
		}
		else
		{
			iValue1	=	g_regularRandom.getValue(m_rect.x2-m_rect.x1,m_wRegularRandomPattern,m_wRegularRandomPatternCounter++);
			iValue2	=	g_regularRandom.getValue(m_rect.y2-m_rect.y1,m_wRegularRandomPattern,m_wRegularRandomPatternCounter++);
		}

		m_aValue[i][0]=(float)(iValue1+m_rect.x1);
		m_aValue[i][1]=(float)(iValue2+m_rect.y1);
	}
}

//
//	¾÷
void
cSPLINE::update()
{
	if	(m_fT	<	0.0)
	{
		m_fT	=	1.0;

		m_aValue[0][0]=m_aValue[1][0];
		m_aValue[0][1]=m_aValue[1][1];
		m_aValue[1][0]=m_aValue[2][0];
		m_aValue[1][1]=m_aValue[2][1];
		m_aValue[2][0]=m_aValue[3][0];
		m_aValue[2][1]=m_aValue[3][1];

		int	iValue1,iValue2;

		if	(m_wRegularRandomPattern	==	0xffff)
		{
			iValue1	=	random(m_rect.x2-m_rect.x1);
			iValue2	=	random(m_rect.y2-m_rect.y1);
		}
		else
		{
			iValue1	=	g_regularRandom.getValue(m_rect.x2-m_rect.x1,m_wRegularRandomPattern,m_wRegularRandomPatternCounter++);
			iValue2	=	g_regularRandom.getValue(m_rect.y2-m_rect.y1,m_wRegularRandomPattern,m_wRegularRandomPatternCounter++);
		}

		m_aValue[3][0]	=	(float)(iValue1+m_rect.x1);
		m_aValue[3][1]	=	(float)(iValue2+m_rect.y1);
	}

	m_pos.x	=	(int)(blend1(m_fT,m_aValue[0][0])+ blend2(m_fT,m_aValue[1][0])+ blend3(m_fT,m_aValue[2][0])+blend4(m_fT,m_aValue[3][0]));
	m_pos.y	=	(int)(blend1(m_fT,m_aValue[0][1])+ blend2(m_fT,m_aValue[1][1])+ blend3(m_fT,m_aValue[2][1])+blend4(m_fT,m_aValue[3][1]));

	m_fT	-=	(float)(m_fDelta);
}