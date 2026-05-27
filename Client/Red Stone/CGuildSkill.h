#ifndef _classGuildSkill_H
#define	_classGuildSkill_H

#include <winsock2.h>
#include "CNemo.h"
#include "guild_skill_define.h"
#include "CGuildDefine.h"

#define	dMAX_GUILD_SKILL_COUNT					150	//	최대 길드 스킬 수

class	CGuildSkill : public CGuildSkillDefine
{
public:
					CGuildSkill();

	void			reset();
};

class	CGuildSkillManager
{
public:
	int				m_iSkillCount;
	int				m_iNormalSkillCount,m_iStatueSkillCount;
	CGuildSkill		m_aSkills[dMAX_GUILD_SKILL_COUNT];
	
	BOOL			init();
	BOOL			readScriptFile(char *_lpstrFN);
	void			reset();
	BOOL			addSkill(CGuildSkill *_lpSkill);
	BOOL			loadData();
	BOOL			saveData();
	CGuildSkill*	get(int _iSkill)
	{
		if (_iSkill < 0 || _iSkill >= dMAX_GUILD_SKILL_COUNT)	return	NULL;
		if (m_aSkills[_iSkill].m_wSerial	==	0xffff		)	return	NULL;

		return	&m_aSkills[_iSkill];
	}
	CGuildSkill*	getByName(char *_lpstrName)
	{
		for (int i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
		{
			if	(m_aSkills[i].m_wSerial	==	0xffff)	continue;

			if (STRICMP(m_aSkills[i].m_strName,_lpstrName) == 0)
				return	&m_aSkills[i];
		}

		return	NULL;
	}
};

extern	CGuildSkillManager	g_gsManager;

#endif