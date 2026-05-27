#ifndef	_active_skill_type_fire_wall_h
#define	_active_skill_type_fire_wall_h

#include	"cSKILL.h"

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

	WORD	m_wGrade;
	WORD	m_wX1,m_wY1,m_wX2,m_wY2;

	int		fire();

	BOOL	makeSmallFlame(int _iX1,int _iY1,int _iX2,int _iY2);	//	작은 불꽃 만들기
	BOOL	makeFireWall(int _iX1,int _iY1,int _iX2,int _iY2);		//	불꽃 장벽 만들기
	BOOL	removeFireWall();	//	불꽃 장벽 제거

	BOOL	update();
};

#endif