#ifndef	_active_skill_type_double_target_h
#define	_active_skill_type_double_target_h

#include	"ast_define.h"

class	cAS_DoubleTarget	:	public cActiveSkillBaseInfo
{
public:
	WORD			m_awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	WORD			m_wTargetCount;
	BYTE			m_bContinuousShotCount;
	BYTE			m_bMissCount;
	BYTE			m_bCriticalCount;
	BYTE			m_bDoubleCriticalCount;
	BYTE			m_bHardBlowCount;

	
	BOOL			cast();
	void			strike();
};

#endif
