#ifndef	_active_skill_type_continuous_bit_h
#define	_active_skill_type_continuous_bit_h

#include	"ast_define.h"

class	cAS_ContinuousBit	:	public cActiveSkillBaseInfo
{
public:
	WORD			m_awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	BYTE			m_bContinuousShotCount;
	BYTE			m_bTargetCount;
	
	BOOL			cast();
	void			strike();
};

#endif
