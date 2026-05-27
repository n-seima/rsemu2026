#ifndef	_active_skill_type_mirror_image_h
#define	_active_skill_type_mirror_image_h

#include	"ast_define.h"

class	cACTOR;

class	cAS_MirrorImage	:	public cActiveSkillBaseInfo
{
public:
	enum
	{
		eCasting,
		eGenerate,
		eWaitNextTime,
		eAwakenTime,
	};
	WORD			m_wMirrorJob,m_wMasterNpcSerial;
	WORD			m_wImageCount;
	WORD			m_wGeneratedImageCount;
	WORD			m_wGrade,m_wGeneratePeriod,m_wAwakenTime,m_wEnchantSkill;
	cACTOR			*m_lpImageTarget;

	BOOL			fire();
	BOOL			casting();
	BOOL			update();
	void			actionNow();
};

#endif