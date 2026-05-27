#ifndef	_active_skill_type_big_bullet_h
#define	_active_skill_type_big_bullet_h

#include	"ast_define.h"

enum
{
	eBBS_READY,
	eBBS_CAUGHT,
	eBBS_THROW,
};

class	cAS_BigBullet	:	public cActiveSkillBaseInfo
{
public:
	DWORD			m_dwLastHitDamage;
	WORD			m_wStep,m_wAttackRange;
	WORD			m_wTargetIndex,m_wTargetCount;
	WORD			m_awTargetList[c_iMaxBigBulletTargetCount];

	BOOL			fire();
	BOOL			catchTarget();		//	타겟을 잡아라
	BOOL			aimingNextTarget();	//	다음 타겟을 노려라!
	BOOL			hit();				//	때려라!!
	BOOL			throwTarget();		//	타겟을 던져라!!
	BOOL			update();
};

#endif
