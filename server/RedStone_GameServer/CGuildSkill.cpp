#include "CGuildSkill.h"
#include "CGuild.h"

CGuildSkillManager	g_gsManager;

//
//	데이터 읽기
BOOL
CGuildSkillManager::init()
{
	return	loadData();
}

//
//	데이터 읽기
BOOL
CGuildSkillManager::loadData()
{
	cFILE	file;

	if (!file.Open("data/guildSkill.dat","rb"))
		return	ERRMSG("error in CGuildSkillManager::loadData","can not find 'data/guildSkill.dat' file.");

	file.Read(&m_iSkillCount,4);
	file.Read(m_aSkills,sizeof(g_gsManager.m_aSkills));
	file.Close();
	
	m_iStatueSkillCount	=	0;

	for (int i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
	{
		if	(m_aSkills[i].m_wSerial	==	0xffff)
			continue;

		if	(m_aSkills[i].m_bf1IsStatueSkill)
			m_iStatueSkillCount++;
	}

	return	TRUE;
}

//
//	길드 스킬 얻기
CGuildSkill*
CGuildSkillManager::get(int _iIndex)
{
	if	(_iIndex < 0 || _iIndex >= dMAX_GUILD_SKILL_COUNT)
		return	NULL;

	if	(m_aSkills[_iIndex].m_wSerial == 0xffff)
		return	NULL;

	return	&m_aSkills[_iIndex];
}


