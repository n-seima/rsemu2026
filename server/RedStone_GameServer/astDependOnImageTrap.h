#ifndef	_active_skill_type_depend_on_image_h
#define	_active_skill_type_depend_on_image_h

#include	"ast_define.h"

class	cAS_DefendOnImageTrap	:	public cActiveSkillBaseInfo
{
public:
	cTrap			m_trap;

	BOOL			cast(cTrap *_lpTrap);
	BOOL			update();
};

#endif