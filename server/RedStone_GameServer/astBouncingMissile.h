#ifndef	_active_skill_type_bouncing_missile_h
#define	_active_skill_type_bouncing_missile_h

#include	"ast_define.h"

class	cAS_BouncingMissile	:	public cActiveSkillBaseInfo
{
public:
	WORD			m_awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	BYTE			m_bTargetCount;
	BYTE			m_bCurrentCount;
	WORD			m_wSpeed;
	BYTE			m_bIsHit;

	void			reset();
	BOOL			cast();
	void			strike(int _iIndex);
	int				fire();
	BOOL			update();
	int				getNextStrikeTime(cPOINT _posBegin,cPOINT _posEnd);
};

#endif
