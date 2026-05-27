#ifndef _classGuildSkill_H
#define _classGuildSkill_H

#include "SFC.h"
#include "guild_skill_define.h"
#include "CGuildDefine.h"

class	CGuildSkill : public CGuildSkillDefine
{
public:
};

class	CGuildSkillManager
{
public:
	int				m_iSkillCount;
	int				m_iStatueSkillCount;
	CGuildSkill		m_aSkills[dMAX_GUILD_SKILL_COUNT];
	
	BOOL			init();
	void			reset();
	BOOL			loadData();
	CGuildSkill*	get(int _iIndex);
};

extern	CGuildSkillManager	g_gsManager;

#endif