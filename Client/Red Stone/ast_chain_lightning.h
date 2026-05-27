#ifndef	_active_skill_type_chain_lightning_h
#define	_active_skill_type_chain_lightning_h

#include	"cSKILL.h"

class	cAS_ChainLightning	:	public cActiveSkillBaseInfo
{
public:
	enum
	{
		eWait,
		eFire,
	};
	WORD			m_wGrade;
	WORD			m_awTargetList[32];
	WORD			m_wTargetCount;

	BOOL			fire();
	BOOL			casting(WORD *_lpwTargetList,int _iTargetCount);
	BOOL			update();
	void			put();
};

#endif