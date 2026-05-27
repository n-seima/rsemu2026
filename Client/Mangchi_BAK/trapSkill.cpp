#include "trapSkill.h"
#include "cDecoder.h"

cTrapSkillManager	g_trapSkillManager;

//
//	기술을 초기화 한다.
BOOL
cTrapSkillManager::init()
{
	cFILE	file;

	file.Open("data/trapSkill.dat","rb");
	file.Read(g_trapSkillManager.m_aTrapSkill,sizeof(g_trapSkillManager.m_aTrapSkill));
	file.Close();

	m_iCount	=	0;

	for (int i=0;i<dMAX_TRAP_SKILL;i++)
		if (m_aTrapSkill[i].m_wSerial	!=	0xffff)
			m_iCount++;

	return	TRUE;
}

cTrapSkill*
cTrapSkillManager::get(char *_lpstrName)
{
	for (int i=0;i<dMAX_TRAP_SKILL;i++)
	{
		if (m_aTrapSkill[i].m_wSerial	==	0xffff)
			continue;

		if (stricmp(m_aTrapSkill[i].m_strName,_lpstrName) == 0)
			return	&m_aTrapSkill[i];
	}

	return	NULL;
}
