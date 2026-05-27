
#ifndef	_active_skill_type_continuous_hit_of_doppelganger_h
#define	_active_skill_type_continuous_hit_of_doppelganger_h

#include	"ast_define.h"

class	cAS_ContinuousHitOfDoppelganger	:	public cActiveSkillBaseInfo
{
public:
	WORD			m_awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	WORD			m_wContinuousShotCount;
	WORD			m_wTargetCount;
	BYTE			m_bMissCount	;
	BYTE			m_bCriticalCount	;
	BYTE			m_bDoubleCriticalCount	;
	BYTE			m_bHardBlowCount	;
	
	WORD			m_wShotCount;
	WORD			m_wCheckShot;
	WORD			m_wDoppelgangerActionSpeed;	
	BOOL			cast();
	void			strike();
	BOOL			update();
};

#endif
