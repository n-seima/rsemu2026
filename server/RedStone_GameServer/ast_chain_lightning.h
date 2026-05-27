#ifndef	_active_skill_type_chain_lightning_h
#define	_active_skill_type_chain_lightning_h

#include	"ast_define.h"

class	cAS_ChainLightning	:	public cActiveSkillBaseInfo
{
public:
	WORD			m_wRelayJob;

	BOOL			fire();
	BOOL			casting();
	BOOL			update();
};

#endif