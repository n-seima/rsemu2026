#ifndef	_common_regular_random_
#define	_common_regular_random_

#include	"SFC.H"

class	cRegularRandom
{
public:
	int			**m_pValueBuffer;
	int			m_iPatternCount;
	int			m_iValueBufferSize;

	cRegularRandom();
	~cRegularRandom();

	void		init(int _iPattenCount,int _iBufferSize,int _iSeedValue=0x3f6e6ad3);
	void		__destroy();
	int			getValue(int _iRange,int _iPattern,int _iIndex);
};

extern	cRegularRandom	g_regularRandom;

#endif