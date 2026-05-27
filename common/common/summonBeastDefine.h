#ifndef	_classSummonBeastDefine_h
#define	_classSummonBeastDefine_h

#include "SFC.H"

#define	dOWN_SUMMON_BEAST_COUNT					2
#define	dDEFAULT_TIME_FOR_RECALL_SUMMON_BEAST	60

//
//	Summon Beast Type
enum
{
	eSBT_KELBY,
	eSBT_SWELLFER,
	eSBT_WINDY,
	eSBT_HEDGER,
};

class	cSummonBeastDefine
{
public:
	DWORD	m_bf10Level		:	10;
	DWORD	m_bf22CurrentHP	:	22;
	WORD	m_bf2Type		:	2;	
	WORD	m_bf2Grade		:	2;	

	void	reset()
	{
		m_bf10Level		=	0;
		m_bf22CurrentHP	=	0;
	}
};


#endif