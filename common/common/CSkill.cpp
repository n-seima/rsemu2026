#include "CSkill.h"

cSKILL			g_aSkill[dMAX_SKILL];
DWORD			g_aSkilExpTable[5]=	{0,1,5,20,45};

//
//	스킬 포인트?
int
cABILITY::getSkillPoint()
{
	cSKILL	*lpSkill=	&g_aSkill[m_wSkill];

	int	iPoint	=	g_aSkilExpTable[lpSkill->m_wDifficultyLevel-1]+lpSkill->m_wDifficultyLevel;

	for (int i=1;i<m_wLevel;i++)
		iPoint	+=	lpSkill->m_wDifficultyLevel*(i+1);

	return	iPoint;
}	//	cABILITY::getSkillPoint()