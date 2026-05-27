#ifndef	_active_skill_type_ContinuousWhirlwind_h
#define	_active_skill_type_ContinuousWhirlwind_h

#include	"ast_define.h"

class	cFIELD;

class cAS_ContinuousWhirlwind : 	public cActiveSkillBaseInfo
{
public:
		int				m_iInhaleEnemyRange;
	WORD			m_wInhalePeroid,m_wInhalePixel,m_wMinInhaleRange;
	int				m_iUpdateCounter;
	WORD			m_wStep;
	UINT			m_uiUpkeepTime;
	UINT			m_uiOccurActionTime;
	WORD			m_wInhaleRangePerLevel;

	 int			fire();
	 BOOL			update();
	 BOOL			cast();


	cAS_ContinuousWhirlwind(){}
	~cAS_ContinuousWhirlwind(){}

	inline			isFinishSkillTime(DWORD _dwTime)
	{
		if	(_dwTime	>=	m_uiUpkeepTime)
			return	TRUE;
		
		return	FALSE;
	}

	inline	BOOL	isOccurActionTime(DWORD _dwTime)
		{
		if	(_dwTime	>=	m_uiOccurActionTime)
			return	TRUE;
		
		return	FALSE;
	}

	void	release();

};

#endif