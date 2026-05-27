#ifndef	_active_skill_type_one_bite_h
#define	_active_skill_type_one_bite_h

#include	"cSKILL.h"

class	cFIELD;

class	cAS_OneBite	:	public cActiveSkillBaseInfo
{
public:
	int		fire();
	BOOL	update();
};

#endif