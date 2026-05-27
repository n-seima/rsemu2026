#ifndef	_active_skill_type_florence_special_h
#define	_active_skill_type_florence_special_h

#include	"ast_define.h"

class	cAS_FlorenceSpecial	:	public cActiveSkillBaseInfo
{
public:
	WORD			m_wShotCount,m_wCurrentShot;

	BOOL			fire();
	BOOL			strike();
	BOOL			update();
};

#endif
