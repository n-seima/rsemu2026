#ifndef	_classTrapSkill_h
#define	_classTrapSkill_h

#include "cNEMO.H"
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

	BOOL		init(char *fn);
	BOOL		addSkill(cTrapSkill	*_lpSkill);
	cTrapSkill*	getSkill(int _iIndex);
};

extern	cTrapSkillManager	g_trapSkillManager;

#endif
