#include	"regular_random.h"

cRegularRandom	g_regularRandom;

cRegularRandom::cRegularRandom()
{
	m_pValueBuffer	=	NULL;

	init(64,512);
}

cRegularRandom::~cRegularRandom()
{
	__destroy();
}

void
cRegularRandom::__destroy()
{
	if	(m_pValueBuffer)
	{
		for	(int i=0;i<m_iPatternCount;i++)
			pKILL(m_pValueBuffer[i]);

		pKILL(m_pValueBuffer);
	}
}

void
cRegularRandom::init(int _iPattenCount,int _iBufferSize,int _iSeedValue)
{
	__destroy();

	srand(_iSeedValue);

	m_iPatternCount		=	_iPattenCount;
	m_iValueBufferSize	=	_iBufferSize;
	
	m_pValueBuffer	=	new int* [_iPattenCount];

	for	(int i=0;i<_iPattenCount;i++)
	{
		m_pValueBuffer[i]	=	new int [_iBufferSize];

		for	(int j=0;j<_iBufferSize;j++)
			m_pValueBuffer[i][j]	=	largeRandom(0x7fffffff);
	}
}

int
cRegularRandom::getValue(int _iRange,int _iPattern,int _iIndex)
{
	if	(_iPattern	>=	m_iPatternCount)
		return	0;

	if	(_iRange	==	0)
		return	0;

	_iIndex	=	_iIndex%m_iValueBufferSize;

	return	m_pValueBuffer[_iPattern][_iIndex]	%	_iRange;
}
