#ifndef _classSKILL_H
#define _classSKILL_H

#include "SFC.H"
#include "CSkillDefine.H"

class	cSKILL : public CSkillDefine
{
public:
};

class	cABILITY
{
public:
	WORD			m_wSkill;
	WORD			m_wLevel;
	
	int				getSkillPoint();	//	스킬 포인트?

};

extern	cSKILL			g_aSkill[dMAX_SKILL];
extern	DWORD			g_aSkilExpTable[5];

#endif