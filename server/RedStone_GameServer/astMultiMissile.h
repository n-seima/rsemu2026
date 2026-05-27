#ifndef	_active_skill_type_Multi_Missile_h
#define	_active_skill_type_Multi_Missile_h

#include	"ast_define.h"

const	int	c_iMaxMultiMissileCount	=	20;

class	cAS_MultiMissile	:	public cActiveSkillBaseInfo
{
public:
	WORD			m_awTargetList[c_iMaxMultiMissileCount];
	WORD			m_wAttackRange;
	WORD			m_wShotCount;
	WORD			m_wIsFired,m_wIsFinished, m_wIsStrike;
	BOOL			m_bIsNotSameTarget;		// 같은타켓...공격~
	int				fire();
	BOOL			cast();
	BOOL			strike();
	BOOL			update();
};

#endif
