#ifndef	_active_skill_type_fire_wall_h
#define	_active_skill_type_fire_wall_h

#include	"ast_define.h"

class	cFIELD;

class	cAS_FireWall	:	public cActiveSkillBaseInfo
{
public:
	enum
	{
		eReady,
		eSmall_Flame,
		eWall,
		eFinish,
	};

	cFIELD	*m_lpField;

	WORD	m_wGrade,m_wSmallFlameTime;
	WORD	m_wX1,m_wY1,m_wX2,m_wY2;
	WORD	m_wActivePeriod;
	DWORD	m_dwEngagedStrikeTime;

	BOOL	fire();

	BOOL	makeSmallFlame();	//	작은 불꽃 만들기
	BOOL	makeFireWall();		//	불꽃 장벽 만들기
	BOOL	removeFireWall();	//	불꽃 장벽 제거
	void	strike();			//	스트라이크


	BOOL	update();
};

#endif