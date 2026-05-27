#ifndef	_active_skill_type_giga_lightning_h
#define	_active_skill_type_giga_lightning_h

#include	"ast_define.h"
#include	"cSPLine.h"

class	cAS_GigaLightning	:	public cActiveSkillBaseInfo
{
public:
	enum
	{
		eStep_ReadyMakeCrossTarget,
		eStep_SerchRandomLocation,
		eStep_LockOn,
		eStep_Fire,
	};
	WORD			m_wGrade;
	WORD			m_wTrapSkill,m_wFinishStep,m_wRandomPattern,m_wTrapRange;
	WORD			m_wCastingTime,m_wSerchTime,m_wWarningTime;
	
	cSPLINE			m_spLine;

	BOOL			fire();
	BOOL			casting();
	BOOL			update();
};

#endif