#ifndef	_active_skill_type_throw_item_h
#define	_active_skill_type_throw_item_h

#include	"ast_define.h"

class	cAS_ThrowItem	:	public cActiveSkillBaseInfo
{
public:
	CItemDefine		m_item;
	int				m_iHitRange,m_iPotionEfficient,m_iHealHP;

	BOOL			fire();
	BOOL			cast();
	BOOL			update();
};

#endif
