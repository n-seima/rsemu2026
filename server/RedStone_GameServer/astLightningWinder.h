#ifndef	_active_skill_type_lightning_winder_h
#define	_active_skill_type_lightning_winder_h

#include	"ast_define.h"

const	int	c_iMaxLightningWinderTargetCount	=	20;

class	cAS_LightningWinder	:	public cActiveSkillBaseInfo
{
public:
	WORD			m_awTargetList[c_iMaxLightningWinderTargetCount];
	WORD			m_wHitTargetIndex,m_wAttackRange;
	WORD			m_wShotCount;
	WORD			m_wIsFired,m_wIsFinished;
	BOOL			m_bIsNotSameTarget;		// 같은타켓...공격~
	int				fire();
	BOOL			cast();
	BOOL			strike();
	BOOL			update();
};

#endif
