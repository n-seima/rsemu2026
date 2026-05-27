#ifndef _classTRAP_DEFINE_H
#define _classTRAP_DEFINE_H

#ifndef _WINDOWS_
#include <windows.h>
#endif

#define	dWIDE_AREA_DAMAGE_TRAP_SKILL_RANGE	350
#define	dRANGE_OF_DISARM_ABLE_TRAP			70

//
//	Trap Target
enum
{
	eTT_TARGET,		//	타겟만 공격
	eTT_RANGE,		//	범위 함정
};

enum
{
	eTSE_FIRE_DAMAGE	,	//	불대미지
	eTSE_WATER_DAMAGE	,	//	물대미지;
	eTSE_EARTH_DAMAGE	,	//	대지대미지;
	eTSE_WIND_DAMAGE	,	//	바람대미지;
	eTSE_LIGHT_DAMAGE	,	//	빛대미지;
	eTSE_DARK_DAMAGE	,	//	어둠대미지;
	eTSE_PHYSICAL_DAMAGE	,	//	물리대미지;
	eTSE_DAMAGE_SKILL_BORDER,	//	물리대미지;
	eTSE_BLIND		,	//	장님;
	eTSE_CLOD		,	//	얼음결정;
	eTSE_FREEZE	,	//	얼음;
	eTSE_NIGHTMARE	,	//	악몽
	eTSE_PARALYSIS	,	//	전신마비
	eTSE_SLEEP	,	//	잠;
	eTSE_STUN	,	//	마비;
	eTSE_CONFUSE	,	//	혼란;
	eTSE_DECLINE_ATTACK_POWER,	//	공격력저하;
	eTSE_DECLINE_DEFENSIVE_POWER,	//	방어력저하;
	eTSE_DECLINE_HIT_CHANCE	,	//	명중률저하;
	eTSE_DECLINE_DODGE_CHANCE	,	//	회피율저하;
	eTSE_DECLINE_ATTACK_SPEED	,	//	공격속도저하;
	eTSE_DECLINE_MOVE_SPEED	,	//	이동속도저하;
	eTSE_DECLINE_CONCENTRATION	,	//	집중력감소;
	eTSE_DECLINE_MAGIC_RESISTANCE	,	//	속성저항력저하;
	eTSE_WEAKNING_RESISTANCE	,	//	저항력약화;
};

#define	dMAX_TRAP_SKILL			100
#define	dTRAP_SKILL_NAME_LENGTH	32

const	int	c_iDisarmTrapAbleRange	=	100;

class	cTrapSkillDefine
{
public:
	WORD	m_wSerial;
	char	m_strName[dTRAP_SKILL_NAME_LENGTH];
	WORD	m_wEffect;
	short	m_sValue[2][2];

	WORD	m_bf1IsStunBasedLevel					:	1;
	WORD	m_bf1IsDeclineDodgeHitChanceBasedLevel	:	1;
	WORD	m_bf1IsDeclineMagicResistanceBasedLevel	:	1;
	WORD	m_bf1IsColdBasedLevel					:	1;
	WORD	m_bf4HitImagePos						:	4;
	WORD	m_bf4HitImageAnm						:	4;
	
	WORD	m_wHitImage;
	char	m_strHitSound[32];

			cTrapSkillDefine()
			{
				reset();
			}

	void	reset()
	{
		memset(this,0,sizeof(cTrapSkillDefine));
		m_wHitImage	=	0xffff;
		m_wSerial	=	0xffff;
	}
};

#endif