#ifndef	_active_skill_type_explosion_with_image_h
#define	_active_skill_type_explosion_with_image_h

#include	"ast_define.h"

class	cAS_ExplosionWithImage	:	public cActiveSkillBaseInfo
{
public:
	WORD			m_wCastingTime;
	WORD			m_wZoomEffectTime;
	int				m_iInhaleEnemyRange;
	WORD			m_wInhalePeroid,m_wInhalePixel,m_wMinInhaleRange;
	int				m_iUpdateCounter;

	int				fire();
	BOOL			cast();
	BOOL			update();
	
	inline	BOOL	decreaseCastingTime()
	{
		if	(m_wCastingTime == 0 )
			return FALSE;
		
		m_wCastingTime--;
		
		return	TRUE;
	}
	
	inline	BOOL	decreaseZoomEffectTime()
	{
		if	(m_wZoomEffectTime == 0)
			return FALSE;
		
		m_wZoomEffectTime--;
		
		return	TRUE;
	}
};

#endif