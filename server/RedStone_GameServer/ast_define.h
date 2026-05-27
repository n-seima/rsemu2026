#ifndef	_active_skill_type_define_h
#define	_active_skill_type_define_h

#include	"SFC.H"
#include	"CAbility.h"
#include	"cItemDefine.h"

#define	dSA_HOMMING						0		//	유도탄
#define	dSA_LINER						1		//	걍 직선으로 날아간다.
#define	dSA_BRING_TO_TARGET				2		//	걍 날아가서 타겟에 딱 맞춘다.
#define	dSA_BOOMERANG					3		//	부메랑
#define	dSA_RETURN_BOOMERANG			4		//	돌아오는 부메랑

#define	dSHOOT_WEAPON_FLY				0
#define	dSHOOT_WEAPON_ARRIVED			1
#define	dSHOOT_WEAPON_BLOCKED			2
#define	dSHOOT_WEAPON_RETURNED			3

#define	dJUMP_READY						0		//	점프 준비(웅크리는 동작
#define	dJUMP_JUMP						1		//	점프~~
#define	dJUMP_FLY						2		//	날자~~~
#define	dJUMP_LAND						3		//	착륙

#define	dEXTRA_VALUE_FOR_ACTIVE_SKILL	50


#define	mIS_GETEDCP						m_aValue[dEXTRA_VALUE_FOR_ACTIVE_SKILL-1]

enum
{
	eACST_FOR_TRAP,
};

#pragma	pack(2)

class	CActiveSkillDamagedActorInfo
{
public:
	WORD			m_wIsGotedCP;
	WORD			m_wCount,m_wSize;
	WORD			m_awActorList[dEXTRA_VALUE_FOR_ACTIVE_SKILL];

	CActiveSkillDamagedActorInfo()
	{
		reset();
	}

	void			reset()
	{
		m_wCount	=	0;
		m_wSize		=	dEXTRA_VALUE_FOR_ACTIVE_SKILL;
		m_wIsGotedCP	=	FALSE;
	}

	inline	void	addActor(int _iSerial)
	{
		if (m_wCount	>=	m_wSize)
			return;
		m_awActorList[m_wCount++]	=	_iSerial;	
	}

	inline	BOOL	isDamagedActor(int _iSerial)
	{
		for (int i=0;i<m_wCount;i++)
		{
			if (m_awActorList[i] == _iSerial)
				return	TRUE;
		}

		return	FALSE;
	}
};

class	cActiveSkillBaseInfo
{
public:
	cAbility		m_ability;
	WORD			m_wSerial;					//	시리얼
	cAreaDoorInfo	*m_lpDoor;
	cACTOR			*m_lpCaster,*m_lpTarget;
	CSkill			*m_lpSkill;	//	스킬
	WORD			m_wTeam;
	cPOINT			m_posTarget,m_pos;			//	현재 위치와 타겟의 위치
	DWORD			m_dwBookedActionTime,m_dwBookedActionFinishTime;
	WORD			m_wAnm,m_wDirect,m_wFrame,m_wMaxFrame,m_wFrameCounter,m_wFPS;
	WORD			m_isNotSpentCP,m_wIsGetdCP;
	WORD			m_wAngle,m_wRange;							//	각도가 필요한 기술이 있다. 타겟과의 거리가 필요한 기술이 있다.
	CHitInfo		m_hitInfo;
	int				m_iCasterZoneSerial,m_iTargetZoneSerial;
	WORD			m_wActiveField;
	WORD			m_wSkillType;

	BOOL			isActionTime();
	void			getActionTime();
	void			setCastingTimeByCasterDamageFrame(int _iTimes=-1);
};

#pragma	pack()

#endif