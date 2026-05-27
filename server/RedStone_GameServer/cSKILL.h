#ifndef _classSKILL_H
#define _classSKILL_H

#include	"SFC.H"
#include	"CImageData.h"
#include	"cItem.H"
#include	"CSkillDefine.H"

class	CSkill : public CSkillDefine
{
public:
						CSkill()
						{
							reset();
						}

	inline	BOOL		isCureStrangeStatus()		{return	(m_wCureEffectField & eCE_STRANGE_STATUS_CURE);}
	inline	BOOL		isCureDeclinePowerStatus()	{return	(m_wCureEffectField & eCE_DECLINE_POWER_STATUS_CURE);}
	inline	BOOL		isCureCurseStatus()			{return	(m_wCureEffectField & eCE_CURSE_STATUS_CURE);}
	inline	BOOL		isCureAllStrangeStatus()	{return	(m_wCureEffectField & eCE_ALL_STRANGE_STATUS_CURE);}
	inline	BOOL		isResurrection()			{return	(m_wCureEffectField & eCE_RESURRECTION);}
	BOOL				isCastOnCorpseSkill();
	BOOL				isAgressiveSkill();			//	공격적인 스킬이다.
	
	inline	BOOL		isDamageSkill()	//	대미지를 주는 스킬이다.
	{
		if (m_wDamageAttribute)	return	TRUE;

		return	FALSE;
	}

	inline	BOOL		isMagicDamageSkill()
	{
		if	(isFireDamageSkill()	||	isWaterDamageSkill()||
			isWindDamageSkill()		||	isEarthDamageSkill()||
			isLightDamageSkill()	||	isDarkDamageSkill()	)
			return	TRUE;

		return	FALSE;
	}
	
	inline	BOOL		isPureMagicDamageSkill()
	{
		if (m_sFireDamagePercentage	|| m_sFireDamagePercentagePerLevel||
			m_sWaterDamagePercentage||m_sWaterDamagePercentagePerLevel||
			m_sWindDamagePercentage||m_sWindDamagePercentagePerLevel||
			m_sEarthDamagePercentage||m_sEarthDamagePercentagePerLevel||
			m_sLightDamagePercentage||m_sLightDamagePercentagePerLevel||
			m_sDarkDamagePercentage||m_sDarkDamagePercentagePerLevel)
			return	FALSE;
		
		if	(isPhysicalDamageSkill())
			return	FALSE;

		if	(isFireDamageSkill()	||	isWaterDamageSkill()||
			isWindDamageSkill()		||	isEarthDamageSkill()||
			isLightDamageSkill()	||	isDarkDamageSkill()	)
			return	TRUE;

		return	FALSE;
	}
	
	inline	BOOL		isPurePhysicalDamageSkill()
	{
		if (m_sFireDamagePercentage	|| m_sFireDamagePercentagePerLevel||
			m_sWaterDamagePercentage||m_sWaterDamagePercentagePerLevel||
			m_sWindDamagePercentage||m_sWindDamagePercentagePerLevel||
			m_sEarthDamagePercentage||m_sEarthDamagePercentagePerLevel||
			m_sLightDamagePercentage||m_sLightDamagePercentagePerLevel||
			m_sDarkDamagePercentage||m_sDarkDamagePercentagePerLevel)
			return	FALSE;
		
		if	(isFireDamageSkill()	||	isWaterDamageSkill()||
			isWindDamageSkill()		||	isEarthDamageSkill()||
			isLightDamageSkill()	||	isDarkDamageSkill()	)
			return	FALSE;

		return	isPhysicalDamageSkill();
	}
	
	inline	BOOL		isPhysicalMagicDamageSkill()
	{
		if (m_sFireDamagePercentage	|| m_sFireDamagePercentagePerLevel||
			m_sWaterDamagePercentage||m_sWaterDamagePercentagePerLevel||
			m_sWindDamagePercentage||m_sWindDamagePercentagePerLevel||
			m_sEarthDamagePercentage||m_sEarthDamagePercentagePerLevel||
			m_sLightDamagePercentage||m_sLightDamagePercentagePerLevel||
			m_sDarkDamagePercentage||m_sDarkDamagePercentagePerLevel)
			return	TRUE;
		
		if	(isPhysicalDamageSkill())
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL		isDecreaseWeaponCountSkill()	{return	m_bf1IsDecreaseWeaponCount;}
	inline	BOOL		isPhysicalDamageSkill()	{return	m_wDamageAttribute&eDAMAGE_TYPE_PHYSICAL	;}
	inline	BOOL		isFireDamageSkill()		{return	m_wDamageAttribute&eDAMAGE_TYPE_FIRE		;}
	inline	BOOL		isWaterDamageSkill()	{return	m_wDamageAttribute&eDAMAGE_TYPE_WATER		;}
	inline	BOOL		isWindDamageSkill()		{return	m_wDamageAttribute&eDAMAGE_TYPE_WIND		;}
	inline	BOOL		isEarthDamageSkill()	{return	m_wDamageAttribute&eDAMAGE_TYPE_EARTH		;}
	inline	BOOL		isLightDamageSkill()	{return	m_wDamageAttribute&eDAMAGE_TYPE_LIGHT		;}
	inline	BOOL		isDarkDamageSkill()		{return	m_wDamageAttribute&eDAMAGE_TYPE_DARK		;}

	inline	BOOL		isSnipeShot()			{return	m_wSpecialEffect&eSKILL_SE_SNIPE_SHOT;}		//	스나이프샷이냐?
	inline	BOOL		isSkullShot()			{return	m_wSpecialEffect&eSKILL_SE_SKULL_SHOT;}		//	스컬샷이냐?
	inline	BOOL		isNotInterrupt()		{return	m_wSpecialEffect&eSKILL_SE_NOT_INTERRUPT;}	//	적 공격 무시
	inline	BOOL		isThrowWeapon()			{return	m_wSpecialEffect&eSKILL_SE_THROW_WEAPON;}	//	무기 던지기
	inline	BOOL		isThrowSecondJobWeapon(){return	m_wSpecialEffect&eSKILL_SE_THROW_OTHER_JOB_WEAPON;}	//	무기 던지기
	inline	BOOL		isJobChange()			{return	m_wSpecialEffect&eSKILL_SE_CHANGE_JOB;}	//	무기 던지기
	inline	BOOL		isHideShield()			{return	m_wSpecialEffect&eSKILL_SE_HIDE_SHIELD;}	//	무기 던지기
	inline	BOOL		isRemoveMisslile()		{return	m_wSpecialEffect&eSKILL_SE_REMOVE_MISSLE;}	//	무기 던지기
	inline	BOOL		isDefenseKnockout()		{return	m_wSpecialEffect&eSKILL_SE_DEFENSE_KNOCKOUT;}	//	무기 던지기
	inline	BOOL		isTaunt()				{return	m_wSpecialEffect&eSKILL_SE_TAUNT;}	//	도발-o-

	inline	BOOL		isRequireWeapon()		{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON;}	//	무기 필요
	inline	BOOL		isRequireShield()		{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_SHIELD;}	//	방패 필요
	inline	BOOL		isRequireBullet()		{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_BULLET;}	//	탄환 필요
	inline	BOOL		isRequireOtherJobWeapon(){return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON2;}	//	탄환 필요
	inline	BOOL		isRequirePet()			{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_PET;}	//	탄환 필요
	inline	BOOL		isRequireSummonBeast()	{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_SUMMON_BEAST;}	//	탄환 필요
	inline	BOOL		isRequirePetOrBeast()	{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_PET_OR_BEAST;}	//	탄환 필요

	inline	BOOL		isExitEnchantEffect()
	{
		return	m_enchantedImage.isExitEffect();
	}
	inline	BOOL		isCastOnEnemySkill()		
	{
		if	(m_dwTargetMethod & eSKILL_CAST_AT_ENEMY)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnPlayerSkill()		
	{
		if	(m_dwTargetMethod & eSKILL_CAST_AT_PLAYER)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnDeathPlayerSkill()	
	{
		if	(m_dwTargetMethod & eSKILL_CAST_AT_DEATH_PLAYER)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnGroundSkill()		
	{
		if	(m_dwTargetMethod & eSKILL_CAST_AT_GROUND)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnCasterSkill()
	{
		if	(m_dwTargetMethod & eSKILL_CAST_AT_CASTER)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnPetSkill()			
	{
		if	(m_dwTargetMethod & eSKILL_CAST_ON_PET)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnDeathPetSkill()
	{
		if	(m_dwTargetMethod & eSKILL_CAST_ON_DEATH_PET)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnEnemyPetSkill()
	{
		if	(m_dwTargetMethod & eSKILL_CAST_ON_ENEMY_PET)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnSummonBeastSkill()
	{
		if	(m_dwTargetMethod & eSKILL_CAST_ON_SUMMON_BEAST)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnKelby()				
	{
		if	(m_dwTargetMethod & eSKILL_CAST_ON_KELBY)
			return	TRUE;
		return	FALSE;
	}
	inline	BOOL		isCastOnDeathSummonBeastSkill()
	{
		if	(m_dwTargetMethod & eSKILL_CAST_ON_DEATH_SUMMON_BEAST)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnPartyMemberSkill()	
	{
		if	(m_dwTargetMethod & eSKILL_CAST_ON_PARTY_MEMBER)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnUserPartyMemberSkill()	
	{
		if	(m_dwTargetMethod & eSKILL_CAST_ON_USER_PARTY_MEMBER)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isQuickCastSkill()
	{
		if	(m_dwTargetMethod & eSKILL_CAST_QUICK)
			return TRUE; 
		return FALSE;
	}

	inline	BOOL		isCastOnTrap()
	{
		if	(m_dwTargetMethod & eSKILL_CAST_ON_TRAP)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnArca()
	{
		if	(m_dwTargetMethod & eSKILL_CAST_ON_ARCA)
			return TRUE; 
		return FALSE;
	}
	inline	BOOL		isCastOnDoor()
	{
		if	(m_dwTargetMethod & eSKILL_CAST_ON_DOOR)
			return TRUE; 
		return FALSE;
	}
	
	BOOL				isRequireTargetSkill();	//	대상이 반드시 필요한 스킬
	BOOL				isEnableLeftSkill();				//	왼쪽에 등록하는게 가능한 스킬이냐?
	BOOL				isEnableRightSkill();				//	오른쪽에 등록하는게 가능한 스킬이냐?
	BOOL				isAidSkill();						//	보조 스킬이냐?
	BOOL				isEnableQuickCastSkill();
	int					getOrderingInSkillAi();
	BOOL				isGoodExtraEffectSkill()
	{
		for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
		{
			if (m_aExtraEffect[i].m_wEffect	==	0xffff)
				return	FALSE;

			if (m_aExtraEffect[i].m_bf4TargetMethod	!=	eEEAT_ENEMY	&&	m_aExtraEffect[i].m_bf4TargetMethod	!=	eEEAT_ENEMY_PET)
				return	TRUE;
		}

		return	FALSE;
	}
	int					getExtraEffectCount()
	{
		int	iCount	=	0;

		for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
		{
			if (m_aExtraEffect[i].m_wEffect	==	0xffff)
				return	iCount;

			iCount++;
		}

		return	iCount;
	}
	BOOL				isBadExtraEffectSkill()
	{
		for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
		{
			if (m_aExtraEffect[i].m_wEffect	==	0xffff)
				return	FALSE;

			if (m_aExtraEffect[i].m_bf4TargetMethod	==	eEEAT_ENEMY	||	m_aExtraEffect[i].m_bf4TargetMethod	==	eEEAT_ENEMY_PET)
				return	TRUE;
		}

		return	FALSE;
	}
	int					getExtraEffectIndexByEffect(int _iEffect);
	CSkillExtraEffect*	getExtraEffectByEffect(int _iEffect);

	inline	BOOL		isPassiveSkill()
	{
		if (m_wCastMethod	==	eST_PASSIVE	||	m_wCastMethod	==	eST_REACTION)	return	TRUE;

		return	FALSE;
	}//	패시브 스킬이냐?

	inline	BOOL		isEnableJob(int _iJob)				//	이 기술을 사용 가능한 직업이냐?(_iJob에 기본 직업이 들어 가야한다.)
	{
		if	(m_wEnableJob	==	0xffff || m_wEnableJob	==	dJOB_CODE_ALL)
			return	TRUE;
		if	(m_wEnableJob	==	_iJob)
			return	TRUE;

		return	FALSE;
	}

	BOOL				isQuickMotionSkill();				//	사용하면 곧바로 발동하는 스킬

	inline	int			getShootingObjective()		{return	m_aValue[0];}	//	탄착점

	void				reset()
						{
							memset(this,0,sizeof(CSkill));
							strcpy(m_strName,"invalid");

							m_sound.reset();

							m_wDifficultyLevel			=	1;
							m_wType						=	eSKILL_TYPE_NORMAL_HIT;
							m_wSerial					=	0xffff;
							m_wAction					=	0xffff;
							m_wShootImage				=	0xffff;
							m_wExplosionImage			=	0xffff;
							m_wHitImage					=	0xffff;
							m_wCastImage				=	0xffff;
							m_wPaletteIndex				=	0xffff;

							m_wEnableJob				=	0xffff;
							m_wSpeed					=	0xffff;

							memset(m_aValue,0,dSKILL_SPARE_VALUE_COUNT);
						}
};

#pragma	pack(2)

#define	dSKILL_COUNT_FOR_GROUP	10
#define	dSKILL_GROUP_COUNT		160
#define	dNAME_LENGTH			18

class cSKILL_GROUP
{
public:
	WORD		m_wSerial;
	WORD		m_wJob;			//	가능 직업
	WORD		m_wSkilCount;	//	스킬의 수
	WORD		m_aSkill[dSKILL_COUNT_FOR_GROUP];
	char		m_strName[dNAME_LENGTH];

				cSKILL_GROUP()
				{
					m_wSerial	=	0xffff;
					m_wJob		=	0xffff;
					m_wSkilCount=	0;

					memset(m_aSkill,0xff,sizeof(m_aSkill));
				}
};

class	cSKILL_MANAGER
{
public:
	int				m_iSkillCount,m_iSkillGroupCount;
	int				m_iMiniPetCombiSkillCount,m_aiMiniPetCombiSkill[32];
	int				m_iObitianSkill,m_iDashBladeSkill,m_iAstroBowSkill,m_iCristalWaterSkill;
	
	cSKILL_GROUP	m_aSkillGroup[dSKILL_GROUP_COUNT];

					cSKILL_MANAGER()
					{
						m_iSkillCount		=	0;
						m_iSkillGroupCount	=	0;
					}

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃cSKILL_INIT.cpp
	int				getIndex(char *_strSkillName,int _iJob);
	BOOL			loadData(char *fn);
	int				getSkillGroupIndex(char *_strName,int _iJob);
//┃cSKILL_INIT.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

};

#pragma	pack()

extern	cSKILL_MANAGER	g_sm;
extern	CSkill			g_aSkill[dMAX_SKILL];

const	int	c_aMinimumCP[]	=	{0,1500,3000,6000,12000};

inline	int		GetCPLevel(int _iCP)	//	현재 CP 레벨을 구한다.
{
	for (int iLevel=4;iLevel>=0;iLevel--)	if (_iCP >= c_aMinimumCP[iLevel])	return	iLevel+1;

	return	0;
}

#define	dMAX_ENCHANTED_MAGIC_COUNT_FOR_CHARACTER	20

class	CEnchantedMagicInfo
{
public:
	WORD	m_wSkill,m_wLevel,m_wRemainTime;

			CEnchantedMagicInfo()
			{
				m_wSkill	=	0xffff;
				m_wLevel	=	0;
			}
};

#endif