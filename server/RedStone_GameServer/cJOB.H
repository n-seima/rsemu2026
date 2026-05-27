#ifndef _classJOB_H
#define	_classJOB_H

#include "SFC.H"
#include "CActiveSkill.H"
#include "cItem.H"
#include "CImageData.h"
#include "definePacketData.h"
#include "playerJobCommon.h"
#include "jobCommon.h"

const	int	c_aDroppingItemCountByMonsterLevel[]	=	{1,2,3,4,5,5,5,6,6,6};	//	몬스터 등급에 따른 떨구는 아이템 수

class	cJOB		:	public cNpcData_common,public CJobBasicDataDefine
{
public:
	enum
	{
		eML_NORMAL1	=	0,
		eML_NORMAL2,
		eML_NORMAL3,
		eML_NORMAL4,
		eML_SEMI_BOSS1,
		eML_SEMI_BOSS2,
		eML_SEMI_BOSS3,
		eML_BOSS1,
		eML_BOSS2,
		eML_BOSS3,
	};

						cJOB()
						{
							m_iSerial		=	0xffff;
						}

						~cJOB()
						{
							reset();
						}

	inline	void		reset(){m_iSerial		=	0xffff;}
	inline	int			getDroppingItemCount()	//	몬스터 등급에 따른 떨구는 아이템 수
	{
		return	c_aDroppingItemCountByMonsterLevel[m_wMonsterLevel];
	}


	static	BOOL		Init();		//	초기화
	static	BOOL		LoadDefaultEquipmentData();
	static	void		Close();	//	제거
};	//	class	cJOB

extern	cJOB		g_aJob[dMAX_JOB_COUNT];


class	CCharacterBaseInfo_old_before090717
{
public:
	WORD			m_wSerial;
	WORD			m_wBaseJob;
	WORD			m_wMinLevel;
	char			m_strName[20];	//	이름
	WORD			m_wCorrectExp;
	WORD			m_wCorrectHP;
	WORD			m_wCorrectAP;
	WORD			m_wCorrectFinalDamage;
	WORD			m_wScaleX,m_wScaleY;
	WORD			m_wMaxLevel;

	DWORD			m_bf1IsInvincible			:	1;
	DWORD			m_bf1IsNotDropGold			:	1;
	DWORD			m_bf1IsNotDropItem			:	1;
	DWORD			m_bf1IsNotExp				:	1;
	DWORD			m_bf8CorrectUseSkillChance	:	8;
	DWORD			m_bf1IsKeyMonster			:	1;
	DWORD			m_bf8Skin					:	8;

	DWORD			m_bf1IsImmuneStun			:	1;
	DWORD			m_bf1IsImmuneStone			:	1;
	DWORD			m_bf1IsImmuneCharming		:	1;
	DWORD			m_bf1IsImmuneFreeze			:	1;
	DWORD			m_bf1IsImmuneKnockBack		:	1;
	DWORD			m_bf1IsReviseIntelligence	:	1;

	DWORD			m_bf1IsImmunePhysicalDamage	:	1;
	DWORD			m_bf1IsImmuneMagicDamage	:	1;

	DWORD			m_bf1IsImmuneConfuse		:	1;
	DWORD			m_bf1IsSetBoss				:	1;		//31
};

class	CCharacterBaseInfo
{
public:
	WORD			m_wSerial;
	WORD			m_wBaseJob;
	WORD			m_wMinLevel;
	char			m_strName[20];	//	이름
	WORD			m_wCorrectExp;
	WORD			m_wCorrectHP;
	WORD			m_wCorrectAP;
	WORD			m_wCorrectFinalDamage;
	WORD			m_wScaleX,m_wScaleY;
	WORD			m_wMaxLevel;

	DWORD			m_bf1IsInvincible			:	1;
	DWORD			m_bf1IsNotDropGold			:	1;
	DWORD			m_bf1IsNotDropItem			:	1;
	DWORD			m_bf1IsNotExp				:	1;
	DWORD			m_bf8CorrectUseSkillChance	:	8;
	DWORD			m_bf1IsKeyMonster			:	1;
	DWORD			m_bf8Skin					:	8;

	DWORD			m_bf1IsImmuneStun			:	1;
	DWORD			m_bf1IsImmuneStone			:	1;
	DWORD			m_bf1IsImmuneCharming		:	1;
	DWORD			m_bf1IsImmuneFreeze			:	1;
	DWORD			m_bf1IsImmuneKnockBack		:	1;
	DWORD			m_bf1IsReviseIntelligence	:	1;

	DWORD			m_bf1IsImmunePhysicalDamage	:	1;
	DWORD			m_bf1IsImmuneMagicDamage	:	1;

	DWORD			m_bf1IsImmuneConfuse		:	1;
	DWORD			m_bf1IsSetBoss				:	1;		//31
	DWORD			m_bf1IsBlockToTame			:	1;		//32

	short			m_asAbsorbMagicDamage[7][2];	//	6개 속성별 발동 확률과 흡수량. 0 발동 확률 1 흡수량
};//
//	Event Check Condition 
enum
{
	eMECC_KILLED_BY_PLAYER,
	eMECC_BEGIN_BATTLE,
	eMECC_KILL_PLAYER,
	eMECC_ALWAYS,
	eMECC_COUNT,
};

class	cCHARACTER	: public CCharacterBaseInfo
{
public:
	CKarmaContainer	*m_pEvent[eMECC_COUNT];	//	이벤트

					cCHARACTER()	
					{
						m_wSerial	=	0xffff;
						for (int i=0;i<eMECC_COUNT;i++)
							m_pEvent[i]	=	NULL;
					}
					~cCHARACTER()	{	reset();}

	inline	void	reset()			
	{
		m_wSerial	=	0xffff;
		for (int i=0;i<eMECC_COUNT;i++)
			KILL(m_pEvent[i]);
	}

	inline	cJOB	*getJob()		{return	&g_aJob[m_wBaseJob];}
	inline	int		getGenerateLevel()	//	생성 레벨
	{
		return	random(m_wMaxLevel-m_wMinLevel+1)	+	m_wMinLevel;
	}

	static	void	Reset(int _iBeginIndex	=	0);
};

const	int	c_aMonsterSkillLevelLimit[]	=	
{
	35,	40,	45,	50,	55,	60,	75,	80,	90	,100,100,100,100,100,100,100,
};
	


#endif