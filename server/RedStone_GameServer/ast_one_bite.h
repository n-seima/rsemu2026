#ifndef	_active_skill_type_one_bite_h
#define	_active_skill_type_one_bite_h

#include	"ast_define.h"

class	cFIELD;

class	cAS_OneBite	:	public cActiveSkillBaseInfo
{
public:
	enum
	{
		eCasting,
		ePullTarget,
		eCatch,
		eBiting,
	};

	WORD			m_wStep,m_wBiteCount,m_wShotCount;

	int				fire();
	BOOL			update();
	BOOL			cast();
};

#endif