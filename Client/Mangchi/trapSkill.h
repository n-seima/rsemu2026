#ifndef	_classTrapSkill_h
#define	_classTrapSkill_h

#include "SFC.h"
#include "trapDefine.h"

class	cTrapSkill	:	public cTrapSkillDefine
{
public:
	
};

class	cTrapSkillManager
{
public:
	int			m_iCount;
	cTrapSkill	m_aTrapSkill[dMAX_TRAP_SKILL];
	
	BOOL		init();
	cTrapSkill*	get(char *_lpstrName);
};

extern	cTrapSkillManager	g_trapSkillManager;
extern	char				*g_strTrapTarget[];

#endif
