#ifndef	_active_skill_type_rolling_log_h
#define	_active_skill_type_rolling_log_h

#include	"ast_define.h"

enum
{
	eRL_READY,
	eRL_TRANS,
	eBL_STRIKE,
};

class	cAS_RollingLog	:	public cActiveSkillBaseInfo
{
public:
	WORD			m_wStep,m_wAttackRange;

	BOOL			fire();
	BOOL			trans();	//	통마무로 변신
	BOOL			strike();	//	타격!!
	BOOL			update();
};

#endif