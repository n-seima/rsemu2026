#ifndef	_classSummonBeast_H
#define	_classSummonBeast_H

#include "summonBeastDefine.h"
#include "SFC.h"
#include "cJOB.h"

class	cSummonBeast	:	public cSummonBeastDefine
{
public:
	WORD		m_wIndex;
	WORD		m_wBodySerial;
	WORD		m_wMinAttackPower,m_wMaxAttackPower,m_wDefensePower;
	WORD		m_wStrength,m_wIntelligence;
	WORD		m_wEngageTarget,m_wNameBarSizeAtField;

	void		reset()
	{
		cSummonBeastDefine::reset();

		m_wBodySerial	=	0xffff;
	}
};

#endif