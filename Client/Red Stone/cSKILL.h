#ifndef _classSKILL_H
#define _classSKILL_H

#include "cNEMO.H"
#include "cACTION.H"
#include "cItem.H"
#include "CSkillDefine.H"
#include "cSPLine.H"
#include "miniPetCommon.H"

#define	dACTIVE_SKILL_COUNT	256

class	cAS_FireWall;

class	CActor;

class	CSkill : public CSkillDefine
{
public:
						CSkill()
						{
							reset();
						}

	inline	BOOL		isMiniPetSkill()
	{
		if	(m_bf5MiniPetType	==	nsMiniPetType::Count)
			return	FALSE;

		return	TRUE;
	}

	inline	BOOL		isDecreaseWeaponCountSkill()	{return	m_bf1IsDecreaseWeaponCount;}
	inline	int			getShootingObjective()		{return	m_aValue[0];}	//	탄착점
	inline	BOOL		isCastOnEnemySkill()		{return	(m_dwTargetMethod & eSKILL_CAST_AT_ENEMY);}
	inline	BOOL		isCastOnPlayerSkill()		{return	(m_dwTargetMethod & eSKILL_CAST_AT_PLAYER);}
	inline	BOOL		isCastOnDeathPlayerSkill()	{return	(m_dwTargetMethod & eSKILL_CAST_AT_DEATH_PLAYER);}
	inline	BOOL		isCastOnGroundSkill()		{return	(m_dwTargetMethod & eSKILL_CAST_AT_GROUND);}
	inline	BOOL		isCastOnPetSkill()			{return	(m_dwTargetMethod & eSKILL_CAST_ON_PET);}
	inline	BOOL		isCastOnSummonBeastSkill()	{return	(m_dwTargetMethod & eSKILL_CAST_ON_SUMMON_BEAST);}
	inline	BOOL		isCastOnKelby()				{return	(m_dwTargetMethod & eSKILL_CAST_ON_KELBY);}
	inline	BOOL		isCastOnDeathPetSkill()		{return	(m_dwTargetMethod & eSKILL_CAST_ON_DEATH_PET);}
	inline	BOOL		isCastOnDeathSummonBeastSkill()	{return	(m_dwTargetMethod & eSKILL_CAST_ON_DEATH_SUMMON_BEAST);}
	inline	BOOL		isCastOnEnemyPetSkill()		{return	(m_dwTargetMethod & eSKILL_CAST_ON_ENEMY_PET);}
	inline	BOOL		isCastOnPartyMemberSkill()	{return	(m_dwTargetMethod & eSKILL_CAST_ON_PARTY_MEMBER);}
	inline	BOOL		isCastOnUserPartyMemberSkill(){return	(m_dwTargetMethod & eSKILL_CAST_ON_PARTY_MEMBER);}
	inline	BOOL		isCastOnCasterSkill()		{return	(m_dwTargetMethod & eSKILL_CAST_AT_CASTER);}
	inline	BOOL		isQuickCastSkill()			{return	(m_dwTargetMethod & eSKILL_CAST_QUICK);}

	inline	BOOL		isCureStrangeStatus()		{return	(m_wCureEffectField & eCE_STRANGE_STATUS_CURE);}
	inline	BOOL		isCureDeclinePowerStatus()	{return	(m_wCureEffectField & eCE_DECLINE_POWER_STATUS_CURE);}
	inline	BOOL		isCureCurseStatus()			{return	(m_wCureEffectField & eCE_CURSE_STATUS_CURE);}
	inline	BOOL		isCureAllStrangeStatus()	{return	(m_wCureEffectField & eCE_ALL_STRANGE_STATUS_CURE);}
	inline	BOOL		isResurrection()			{return	(m_wCureEffectField & eCE_RESURRECTION);}

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

	inline	BOOL		isAidMagic()
	{
		if (m_wType	==	eSKILL_TYPE_AID_MAGIC)	return	TRUE;
		return	FALSE;
	}
	inline	BOOL		isAgressiveSkill()	//	공격적인 스킬이다.
	{
		if (m_wType	<	eSKILL_TYPE_AGRESSIVE_SKILL_BORDER)
			return	TRUE;
		if (m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER)
			return	TRUE;
		return	FALSE;
	}

	inline	BOOL		isPhysicalMagicDamageSkill()
	{
		if	(m_sFireDamagePercentage	|| m_sFireDamagePercentagePerLevel||
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
	inline	BOOL		isJobChange()			{return	m_wSpecialEffect&eSKILL_SE_CHANGE_JOB;}	//	무기 던지기
	inline	BOOL		isHideShield()			{return	m_wSpecialEffect&eSKILL_SE_HIDE_SHIELD;}	//	무기 던지기
	inline	BOOL		isRemoveMisslile()		{return	m_wSpecialEffect&eSKILL_SE_REMOVE_MISSLE;}	//	무기 던지기
	inline	BOOL		isDefenseKnockout()		{return	m_wSpecialEffect&eSKILL_SE_DEFENSE_KNOCKOUT;}	//	무기 던지기
	inline	BOOL		isTaunt()				{return	m_wSpecialEffect&eSKILL_SE_TAUNT;}	//	도발-o-
	
	inline	BOOL		isRequireWeapon()			{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON;}	//	무기 필요
	inline	BOOL		isRequireOtherJobWeapon()	{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON2;}	//	무기 필요
	inline	BOOL		isRequireShield()			{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_SHIELD;}	//	방패 필요
	inline	BOOL		isRequireBullet()			{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_BULLET;}	//	탄환 필요
	inline	BOOL		isRequirePet()				{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_PET;}		//	탄환 필요
	inline	BOOL		isRequireSummonBeast()		{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_SUMMON_BEAST;}	//	소환수 필요
	inline	BOOL		isRequirePetOrBeast()		{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_PET_OR_BEAST;}	//	소환수 필요
	inline	BOOL		isRequirePotion()			{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_POTION;}	//	탄환 필요
	inline	BOOL		isRequireFlower()			{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_FLOWER;}	//	탄환 필요
	inline	BOOL		isRequireDrug()				{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_DRUG;}	//	탄환 필요
	inline	BOOL		isRequireCandy()			{return	m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_CANDY;}	//	탄환 필요

	inline	BOOL		isAttackSkill()
	{
		if (m_sAttackPoint	||	m_sAttackPointPerLevel	||	m_sAttackPointRangePerLevel	||	m_sCorrectAttackPoint	||	m_sAttackPercentage	||	m_sAttackPercentagePerLevel || m_sAttackPointPerActorLevel)
			return	TRUE;

		return	FALSE;
	}	//	탄환 필요

	CSkillExtraEffect*	getExtraEffectByEffect(int _iEffect);
	BOOL				getExceptionstrengtheningSkill();
	int					getOrderingInSkillAi();
	BOOL				isContinuousAttackSkill();
	BOOL				isEnableLeftSkill();				//	왼쪽에 등록하는게 가능한 스킬이냐?
	BOOL				isEnableRightSkill();				//	오른쪽에 등록하는게 가능한 스킬이냐?
	BOOL				isEnableSubSkill();					//	서브 스킬에 가능한 스킬이냐?
	BOOL				isSupportSkill();					//	보조 스킬이냐?
	BOOL				isPassiveSkill();					//	패시브 스킬이냐?

	BOOL				isEnableJob(int _iJob);				//	이 기술을 사용 가능한 직업이냐?(_iJob에 기본 직업이 들어 가야한다.)
	inline	BOOL		isWaterCanonType()
	{
		if (m_wAfterImageType	==	eSKILL_AFTER_IMAGE_WATER_CANON_STYLE)	return	TRUE;

		return	FALSE;
	}

	void				reset();
	int					getActiveChance(int _iAwakenType, int _iLevel);
	int					getAwakenActiveChance(int _iAwakenType, int _iLevel);
	int					getAwakenHitRange(int _iAwakenType, int _iLevel);
	int					getAwakenBoJoDamage(int _iAwakenType, int _iLevel);
};

extern	CSkill		g_aSkill[dMAX_SKILL];

#pragma	pack(2)

class	cAbility
{
public:
	static	cSTRING	s_strTooltip;

	WORD			m_wSkill;
	WORD			m_wLevel;

					cAbility(){reset();}

	inline	void	set(int _iSkill,int _iLevel){m_wSkill=_iSkill,m_wLevel=_iLevel;}
	inline	void	reset(){m_wSkill=0xffff,m_wLevel=0;}
	inline	CSkill	*getSkill()
	{
		if	(m_wSkill >= dMAX_SKILL)
			return NULL;

		if	(g_aSkill[m_wSkill].m_wSerial	==	0xffff)
			return	NULL;

		return &g_aSkill[m_wSkill];
	}
	inline	BOOL	isEnableJob(int _iJob)	//	이 기술을 사용 가능한 직업이냐?(_iJob에 기본 직업이 들어 가야한다.)
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)	return	FALSE;

		return	lpSkill->isEnableJob(_iJob);
	}

	inline	BOOL	isSameAbility(cAbility *_lpAbility)
	{
		if (m_wSkill	!=	_lpAbility->m_wSkill)	return	FALSE;
		if (m_wLevel	!=	_lpAbility->m_wLevel)	return	FALSE;

		return	TRUE;
	}

	inline	BOOL		isAgressiveSkill()	//	공격적인 스킬이다.
	{
		return	getSkill()->isAgressiveSkill();
	}

	BOOL			isAidAttackSkill();
	BOOL			isBasicAttackSkill();//	기본 공격 스킬이냐?
	BOOL			isQuickCastSkill();//	퀵 캐스트가 가능한 스킬이냐?
	BOOL			isCastEnemySkill();//	적한테 쓰는 스킬이냐?
	BOOL			isCastPlayerSkill();//	적한테 쓰는 스킬이냐?
	BOOL			isCastGroundSkill();//	적한테 쓰는 스킬이냐?
	BOOL			isReactionSkill();//	리액션 스킬이냐?
	int				getSkillPoint();
	BOOL			isCanNotActionSkillOnHill();
	int				getCoolTime();// 쿨타임.

	inline	BOOL	isActivatePassiveSkill(int _iCurrentCP)	//	활성화된 패시브 스킬이냐?
	{
		if (_iCurrentCP	>=	getSpentCP())
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isActivateReactionSkill(int _iCPLevel)	//	활성화된 리액션 스킬이냐?
	{
		if (_iCPLevel	>=	getSkill()->m_wDifficultyLevel)
			return	TRUE;

		return	FALSE;
	}

	BOOL			getExtraEffect(int _iIndex,CSkillExtraEffectInfo	*_lpInfo);
	int				getUpkeepTime();
	int				getExtraUpkeepTime(int _iIndex);

	int				getHealPoint(BOOL _IsPetResurrection = FALSE , int _iLevel = 0);
	int				getHealPercentageBasedRemainHP();
	int				getFirstAidHP();
	int				getRestoreExpPercentage();

	CSkillExtraEffect	*getAdditionalDamageInfo(int _iIndex);
	int				getRemainExpForLevelUp();	//	레벨 업까지 남은 경험치

	int				isUseAble(int _iCP,BOOL _isEquipWeapon,BOOL _isEquipWeapon2,BOOL _isEquipShield,int _iBulletCount,int _iPetCount,int _iSummonBeastCount);//	사용가능하냐?
	int				getGetCP();	//	CP 얻기
	inline	int		getSpentCP()//	예상 소모 CP
	{
		CSkill	*lpSkill	=	getSkill();

		return	lpSkill->m_wSpentCP	+	lpSkill->m_wSpentCPPerLevel*m_wLevel;
	}
	inline	int		getDodgeChance()
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)	return	0;

		return	(lpSkill->m_sDodgeChance+lpSkill->m_sDodgeChancePerLevel*m_wLevel)/100;
	}
	inline	int		getDefensivePower()
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)
			return	0;

		return	lpSkill->m_sDefensePoint+lpSkill->m_sDefensePointPerLevel*m_wLevel;
	}
	inline	int		getDefensivePowerPercentage()
	{
		CSkill	*lpSkill	=	getSkill();

		if	(!lpSkill)
			return	0;

		return	lpSkill->m_sDefensePercentage+lpSkill->m_sDefensePercentagePerLevel*m_wLevel;
	}
	inline	int		getCorrectBlockingChance()	//	블럭률 보정치 얻기(1000 분률)
	{
		CSkill		*lpSkill	=	getSkill();
		int			iBlockingChance;
		
		if (lpSkill->m_sFixBlockingChance)	
			iBlockingChance	=	lpSkill->m_sFixBlockingChance/10;
		else
		{
			iBlockingChance	=	(lpSkill->m_sBlockingChance+lpSkill->m_sBlockingChancePerLevel*m_wLevel)/10;
			iBlockingChance	=	min(iBlockingChance,lpSkill->m_sMaximumBlockingChance/10);
		}

		return	iBlockingChance;
	}
	int				getBulletCount();
	int				getShotCount();	//	탄수
	int				getContinuousShotCount(BOOL _bIsCheckDefaultLimit=TRUE);
	inline	int		getContinuousShotPeriod(){return	getSkill()->m_sContiniousShotPeriod;}

	int				getAttackSpeed();	//	공격 속도
	int				getCorrectAttackSpeed();	//	공격속도 보정 수치
	int				getAttackRange(cItem *_lpWeapon,int _iDefaultRange);	//	사거리 구함
	int				getMinimumAttackRange();	//	최소 사거리 구함
	int				getHitRange(cItem *_lpWeapon);//	타격 범위
	int				getCorrectHitChance();//명중률 보정 수치
	int				getFixHitChance();//명중률
	int				getAttackPower(BOOL _isMin=TRUE);//공격력
	int				getAttackPowerPercentage();//	공격력 퍼센티지
	void			copy(cAbility *_lpAbility){m_wSkill=_lpAbility->m_wSkill,m_wLevel=_lpAbility->m_wLevel;}//어빌러티 복사

	inline	BOOL	isPhysicalDamageSkill()	{return	getSkill()->isPhysicalDamageSkill();}
	inline	BOOL	isFireDamageSkill()		{return	getSkill()->isFireDamageSkill();}
	inline	BOOL	isWaterDamageSkill()	{return	getSkill()->isWaterDamageSkill();}
	inline	BOOL	isWindDamageSkill()		{return	getSkill()->isWindDamageSkill();}
	inline	BOOL	isEarthDamageSkill()	{return	getSkill()->isEarthDamageSkill();}
	inline	BOOL	isLightDamageSkill()	{return	getSkill()->isLightDamageSkill();}
	inline	BOOL	isDarkDamageSkill()		{return	getSkill()->isDarkDamageSkill();}
	inline	BOOL	isHpSyncSkill()			{return	getSkill()->m_bf1IsHpSyncSkill;}	//	도발-o-
	inline	BOOL	isDecreaseWeaponCountSkill()	{return	getSkill()->m_bf1IsDecreaseWeaponCount;}

	int				getApplyLimitHP(int _iLevel);
	int				getApplyLimitLevel(int _iLevel,int _iType);
	
	int				getFireDamage(int _iPhysicalDamage,BOOL _isMin=FALSE);
	int				getWaterDamage(int _iPhysicalDamage,BOOL _isMin=FALSE);
	int				getWindDamage(int _iPhysicalDamage,BOOL _isMin=FALSE);
	int				getEarthDamage(int _iPhysicalDamage,BOOL _isMin=FALSE);
	int				getLightDamage(int _iPhysicalDamage,BOOL _isMin=FALSE);
	int				getDarkDamage(int _iPhysicalDamage,BOOL _isMin=FALSE);

	inline	BOOL	isRequireWeapon()			{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON;}	//	무기 필요
	inline	BOOL	isRequireOtherJobWeapon()	{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON2;}	//	무기 필요
	inline	BOOL	isRequireShield()			{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_SHIELD;}	//	방패 필요
	inline	BOOL	isRequireBullet()			{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_BULLET;}	//	탄환 필요
	inline	BOOL	isRequirePotion()			{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_POTION;}	//	탄환 필요
	inline	BOOL	isRequireFlower()			{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_FLOWER;}	//	탄환 필요
	inline	BOOL	isRequireDrug()				{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_DRUG;}	//	탄환 필요
	inline	BOOL	isRequireCandy()			{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_CANDY;}	//	탄환 필요

	inline	BOOL	isSnipeShot()		{return	getSkill()->m_wSpecialEffect&eSKILL_SE_SNIPE_SHOT;}		//	스나이프샷이냐?
	inline	BOOL	isSkullShot()		{return	getSkill()->m_wSpecialEffect&eSKILL_SE_SKULL_SHOT;}		//	스컬샷이냐?
	inline	BOOL	isNotInterrupt()	{return	getSkill()->m_wSpecialEffect&eSKILL_SE_NOT_INTERRUPT;}	//	적 공격 무시
	inline	BOOL	isThrowWeapon()		{return	getSkill()->m_wSpecialEffect&eSKILL_SE_THROW_WEAPON;}	//	무기 던지기
	inline	BOOL	isJobChange()		{return	getSkill()->m_wSpecialEffect&eSKILL_SE_CHANGE_JOB;}	//	무기 던지기
	inline	BOOL	isHideShield()		{return	getSkill()->m_wSpecialEffect&eSKILL_SE_HIDE_SHIELD;}	//	무기 던지기
	inline	BOOL	isRemoveMisslile()	{return	getSkill()->m_wSpecialEffect&eSKILL_SE_REMOVE_MISSLE;}	//	무기 던지기
	inline	BOOL	isDefenseKnockout()	{return	getSkill()->m_wSpecialEffect&eSKILL_SE_DEFENSE_KNOCKOUT;}	//	무기 던지기
	inline	BOOL	isTaunt()			{return	getSkill()->m_wSpecialEffect&eSKILL_SE_TAUNT;}	//	도발-o-

	BOOL			isCloseRangeDamageAttack();
	int				getActiveChance(int _iAwakenType = FALSE);

	int				getAwakenActiveChance(int _iAwakenType, int _iLevel);
	int				getAwakenHitRange(int _iAwakenType, int _iLevel);
	int				getAwakenBoJoDamage(int _iAwakenType, int _iLevel);

//	abilityToolTip.cpp
	char*			getTooltip(int _iCorrectLevel=0,BOOL _isPetResurrection = FALSE , int _iLevel = 0);	//	툴팁 리턴-o-
	void			getEE_ToolTip(cSTRING *_lpString,CSkillExtraEffect *_lpEffect,CSkillExtraEffectInfo *_lpEE);
	void			makeAidMagicDamageTooltip(CSkillExtraEffect *_lpEffect);
	void			makeDamageTooltip(char *_lpstrTitle,int _iMinDamage,int _iMaxDamage);
	void			makeDamageTooltip(int _iMinDamage,int _iMaxDamage,int _iMinWeaponDamage,int _iMaxWeaponDamage,BOOL _bIsAidMagic=FALSE,BOOL _bIsSummonbeastSkill=FALSE);

};

#define		dATTACK_RESULT_FIELD_HIT				0x00000001
#define		dATTACK_RESULT_FIELD_CRITICAL			0x00000002
#define		dATTACK_RESULT_FIELD_CRUSH				0x00000004
#define		dATTACK_RESULT_FIELD_BLOCK				0x00000008
#define		dATTACK_RESULT_FIELD_HIT_ACTION			0x00000010
#define		dATTACK_RESULT_FIELD_INSTANCE_KILL		0x00000020
#define		dATTACK_RESULT_FIELD_PHYSICAL_DAMAGE	0x00000040

#define		dATTACK_RESULT_LUCKY_HIT				0x00000080
#define		dATTACK_RESULT_LUCKY_DOUBLE_CRITICAL	0x00000100
#define		dATTACK_RESULT_LUCKY_DODGE				0x00000200
#define		dATTACK_RESULT_IMMUNE					0x00000400
#define		dATTACK_RESULT_LUCKY_DAMAGE				0x00000800
#define		dATTACK_RESULT_LUCKY_DODGE_CRITICAL		0x00001000

#define		dATTACK_RESULT_FIELD_NO_ACTION_BLOCK	0x00002000
#define		dATTACK_RESULT_FIELD_DANCING_BLOCKER	0x00004000
#define		dATTACK_RESULT_FIELD_DODGE				0x00008000

#define		dATTACK_RESULT_INVINCIBLE				0x00010000 //(dATTACK_RESULT_FIELD_INSTANCE_KILL+dATTACK_RESULT_LUCKY_DODGE)
#define		dATTACK_RESULT_REFLECTION				0x00020000 //(dATTACK_RESULT_FIELD_INSTANCE_KILL+dATTACK_RESULT_FIELD_DODGE)
#define		dATTACK_RESULT_HIT_THE_OTHER_SELF		0x00040000 //(dATTACK_RESULT_FIELD_INSTANCE_KILL+dATTACK_RESULT_FIELD_DANCING_BLOCKER)
#define		dATTACK_RESULT_ATTACK_TO_SHAKLE			0x00080000 //(dATTACK_RESULT_FIELD_INSTANCE_KILL+dATTACK_RESULT_FIELD_BLOCK)
#define		dATTACK_RESULT_DAMAGE_POOL				0x00100000 //(dATTACK_RESULT_FIELD_INSTANCE_KILL+dATTACK_RESULT_FIELD_NO_ACTION_BLOCK)
#define		dATTACK_RESULT_FIELD_HIT_FIRE			0x00200000 //(dATTACK_RESULT_FIELD_CRITICAL+dATTACK_RESULT_LUCKY_DOUBLE_CRITICAL)
#define		dATTACK_RESULT_FIELD_HARD_BLOW			0x00400000 
#define		dATTACK_RESULT_FIELD_IGNORE_BLOCK		0x00800000 
#define		dMAX_CONTINUOUS_HIT_INFO_COUNT			12

enum
{
	eATTACK_RESULT_MISS	=	1,
	eATTACK_RESULT_HIT	,
	eATTACK_RESULT_CIRITICAL	,
	eATTACK_RESULT_LUCKY_DOUBLE_CIRITICAL	,
	eATTACK_RESULT_HARD_BLOW	,
	eATTACK_RESULT_BLOCKING,
	eATTACK_RESULT_NO_ACTION_BLOCKING,	
	eATTACK_RESULT_DANCING_BLOCKING	,
};

class	CContinuousHitInfo
{
public:

	DWORD			m_bf6ContinuousShotCount	:	6;	//	6
	DWORD			m_bf6MissCount			:	6;	//	12
	DWORD			m_bf6CriticalCount		:	6;	//	18
	DWORD			m_bf6DoubleCriticalCount	:	6;	//	24
	DWORD			m_bf6HardBlowCount		:	6;	//	30
	DWORD			:0;
				 
				 
	void	reset()
	{
		memset(this, 0, sizeof(CContinuousHitInfo));
	}
};


class	CHitInfo
{
public:
	DWORD			m_dwPhysicalDamage;		//	물리 대미지
	DWORD			m_dwMagicDamage;			//	마법 대미지
	DWORD			m_dwResultField;			//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션,32 즉사
	CContinuousHitInfo	continuousHitInfo;
	inline	void	reset(BOOL _bIsWantResetResult=FALSE)
	{
		if (_bIsWantResetResult)
			m_dwResultField	=	0;
		else
			m_dwResultField	=	0xffffffff;
		
		m_dwPhysicalDamage	=	0;		//	물리 대미지
		m_dwMagicDamage		=	0;		//	마법 대미지
		continuousHitInfo.reset();
	}
	inline	void	set(int _iPhysicalDamage,int _iMagicDamage,DWORD _dwResult,CContinuousHitInfo* _lpContinuousHitInfo = NULL )
	{
		m_dwPhysicalDamage	=	_iPhysicalDamage;		//	물리 대미지
		m_dwMagicDamage		=	_iMagicDamage;			//	마법 대미지
		m_dwResultField		=	_dwResult;			//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션,32 즉사
		if(_lpContinuousHitInfo)
		{
			memcpy(&continuousHitInfo,_lpContinuousHitInfo, sizeof(CContinuousHitInfo));
		}
	}
	inline	void	copy(CHitInfo *_lpInfo)
	{
		m_dwPhysicalDamage	=	_lpInfo->m_dwPhysicalDamage;		//	물리 대미지
		m_dwMagicDamage		=	_lpInfo->m_dwMagicDamage;		//	마법 대미지
		m_dwResultField		=	_lpInfo->m_dwResultField;		//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션,32 즉사
		continuousHitInfo	=	_lpInfo->continuousHitInfo;
	}

	inline	int		getDamage()				{return	m_dwPhysicalDamage+m_dwMagicDamage;}

	inline	void	setHit()				{m_dwResultField|=dATTACK_RESULT_FIELD_HIT;}
	inline	void	setCritical()			{m_dwResultField|=dATTACK_RESULT_FIELD_CRITICAL;}
	inline	void	setCrush()				{m_dwResultField|=dATTACK_RESULT_FIELD_CRUSH;}
	inline	void	setBlock()				{m_dwResultField|=dATTACK_RESULT_FIELD_BLOCK;}
	inline	void	setNoActionBlock()		{m_dwResultField|=dATTACK_RESULT_FIELD_NO_ACTION_BLOCK;}
	inline	void	setDancingBlockerBlock(){m_dwResultField|=dATTACK_RESULT_FIELD_DANCING_BLOCKER;}
	inline	void	setHitAction()			{m_dwResultField|=dATTACK_RESULT_FIELD_HIT_ACTION;}
	inline	void	setInstanceKill()		{m_dwResultField|=dATTACK_RESULT_FIELD_INSTANCE_KILL;}
	inline	void	setPhysicalDamage()		{m_dwResultField|=dATTACK_RESULT_FIELD_PHYSICAL_DAMAGE;}
	inline	void	setDodge()				{m_dwResultField|=dATTACK_RESULT_FIELD_DODGE			;}
	inline	void	setLuckyDoubleCritical(){m_dwResultField|=dATTACK_RESULT_LUCKY_DOUBLE_CRITICAL;}
	inline	void	setAttackToShakle()
	{
		if	(m_dwResultField == 0xffffffff )	
			m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_ATTACK_TO_SHAKLE;
	}
	inline	void	setHardBlow()			{m_dwResultField|=dATTACK_RESULT_FIELD_HARD_BLOW;}
	inline	void	setIgnoreBlock()			{m_dwResultField|=dATTACK_RESULT_FIELD_IGNORE_BLOCK;}
	inline	BOOL	isHit()					{if (m_dwResultField&dATTACK_RESULT_FIELD_HIT			)	return	TRUE;return	FALSE;}
	inline	BOOL	isMiss()				{if (m_dwResultField&dATTACK_RESULT_FIELD_HIT			)	return	FALSE;return TRUE;}
	inline	BOOL	isCritical()			{if (m_dwResultField&dATTACK_RESULT_FIELD_CRITICAL		)	return	TRUE;return	FALSE;}
	inline	BOOL	isCrush()				{if (m_dwResultField&dATTACK_RESULT_FIELD_CRUSH			)	return	TRUE;return	FALSE;}
	inline	BOOL	isBlock()				
	{
		if	(isAttackToShakle())
			return	FALSE;

		if	(m_dwResultField&dATTACK_RESULT_FIELD_BLOCK			)
			return	TRUE;
		return	FALSE;
	}
	inline	BOOL	isNoActionBlock()
	{
		if	(m_dwResultField&dATTACK_RESULT_FIELD_INSTANCE_KILL)
			return	FALSE;

		if	(m_dwResultField&dATTACK_RESULT_FIELD_NO_ACTION_BLOCK)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isDancingBlockerBlock()	
	{
		if	(isHitTheOtherSelf())
			return	FALSE;

		if	(m_dwResultField&dATTACK_RESULT_FIELD_DANCING_BLOCKER)
			return	TRUE;
		return	FALSE;
	}
	inline	BOOL	isHitAction()			{if (m_dwResultField&dATTACK_RESULT_FIELD_HIT_ACTION		)	return	TRUE;return	FALSE;}
	inline	BOOL	isInstanceKill()		
	{
		if	(isReflection() || isInvincible() || isHitTheOtherSelf()	||	isAttackToShakle()	||	isByDamagePool())
			return	FALSE;
		if	(m_dwResultField&dATTACK_RESULT_FIELD_INSTANCE_KILL	)
			return	TRUE;
		return	FALSE;
	}
	inline	BOOL	isPhysicalDamage()		{if (m_dwResultField&dATTACK_RESULT_FIELD_PHYSICAL_DAMAGE)	return	TRUE;return	FALSE;}
	inline	BOOL	isDodge()				
	{
		if	(isReflection())
			return	FALSE;

		if (m_dwResultField&dATTACK_RESULT_FIELD_DODGE)
			return	TRUE;

		return	FALSE;
	}
	inline BOOL		isHitFire()
	{
		if (m_dwResultField&dATTACK_RESULT_FIELD_HIT_FIRE)
			return	TRUE;
		return	FALSE;
	}
	inline	BOOL	isImmune()				
	{
		if	(m_dwResultField&dATTACK_RESULT_FIELD_INSTANCE_KILL	)
			return	FALSE;

		if	(m_dwResultField&dATTACK_RESULT_IMMUNE				)
			return	TRUE;
		
		return	FALSE;
	}
	inline	BOOL	isInvincible()			{if ((m_dwResultField&dATTACK_RESULT_INVINCIBLE)	==	dATTACK_RESULT_INVINCIBLE)	return	TRUE;return	FALSE;}
	inline	BOOL	isHitTheOtherSelf()		{if ((m_dwResultField&dATTACK_RESULT_HIT_THE_OTHER_SELF)	==	dATTACK_RESULT_HIT_THE_OTHER_SELF)	return	TRUE;return	FALSE;}
	inline	BOOL	isReflection()			{if ((m_dwResultField&dATTACK_RESULT_REFLECTION)	==	dATTACK_RESULT_REFLECTION)	return	TRUE;return	FALSE;}
	inline	BOOL	isAttackToShakle()		{if ((m_dwResultField&dATTACK_RESULT_ATTACK_TO_SHAKLE)	==	dATTACK_RESULT_ATTACK_TO_SHAKLE)	return	TRUE;return	FALSE;}
	inline	BOOL	isByDamagePool()		{if ((m_dwResultField&dATTACK_RESULT_DAMAGE_POOL	)	==	dATTACK_RESULT_DAMAGE_POOL	)	return	TRUE;return	FALSE;}

	inline	BOOL	isLuckyHit()			{if (m_dwResultField&dATTACK_RESULT_LUCKY_HIT			)	return	TRUE;return	FALSE;}
	inline	BOOL	isLuckyDoubleCritical()	{if (m_dwResultField&dATTACK_RESULT_LUCKY_DOUBLE_CRITICAL)	return	TRUE;return	FALSE;}
	inline	BOOL	isLuckyDodge()			
	{
		if (isInstanceKill())
			return	FALSE;
		if (m_dwResultField&dATTACK_RESULT_LUCKY_DODGE			)	
			return	TRUE;
		return	FALSE;
	}
	inline	BOOL	isLuckyDamage()			{if (m_dwResultField&dATTACK_RESULT_LUCKY_DAMAGE			)	return	TRUE;return	FALSE;}
	inline	BOOL	isLuckyDodgeCritical()	{if (m_dwResultField&dATTACK_RESULT_LUCKY_DODGE_CRITICAL	)	return	TRUE;return	FALSE;}
	inline BOOL		isHardBlow()
	{
		if (m_dwResultField&dATTACK_RESULT_FIELD_HARD_BLOW)
			return	TRUE;
		return	FALSE;
	}
	inline BOOL		isIgnoreBlock()
	{
		if (m_dwResultField&dATTACK_RESULT_FIELD_IGNORE_BLOCK)
			return	TRUE;
		return	FALSE;
	}
};	//	class	cCONTINUOUS_HIT_INFO

class		cATTACK_INFO
{
public:
	cAbility			m_ability;
	WORD				m_wTarget;
	CHitInfo			m_aInfo[dMAX_CONTINUOUS_HIT_INFO_COUNT];

						cATTACK_INFO()	{reset();}

	void				setAbility(cAbility *_lpAbility)	{m_ability.copy(_lpAbility);}
	void				setTarget(int _iSerial)				{m_wTarget	=	_iSerial;}

	inline	cAbility*	getAbility()	{if (m_wTarget	!=	0xffff)return	&m_ability;return	NULL;}
	inline	CHitInfo*	getInfo(int _iIndex){return	&m_aInfo[_iIndex];}
	inline	void		reset()	
	{
		m_wTarget	=	0xffff;	//	으음.. 당연한걸 왜 안해 줬을까? 어째 불안하다.. ;;
		for (int i=0;i<dMAX_CONTINUOUS_HIT_INFO_COUNT;i++)	m_aInfo[i].reset();
	}
	inline	int			getContinuousShotCount(){return	m_ability.getContinuousShotCount();}
	inline	int			getContinuousShotPeriod(){return	m_ability.getSkill()->m_sContiniousShotPeriod;}
};


#define	dSA_HOMMING					0		//	유도탄
#define	dSA_LINER					1		//	걍 직선으로 날아간다.
#define	dSA_BRING_TO_TARGET			2		//	걍 날아가서 타겟에 딱 맞춘다.
#define	dSA_BOOMERANG				3		//	부메랑
#define	dSA_RETURN_BOOMERANG		4		//	돌아오는 부메랑

#define	dMISSILE_WEAPON_FLY			0
#define	dMISSILE_WEAPON_ARRIVED		1
#define	dMISSILE_WEAPON_BLOCKED		2
#define	dMISSILE_WEAPON_RETURNED	3

#define	dJUMP_READY					0		//	점프 준비(웅크리는 동작
#define	dJUMP_JUMP					1		//	점프~~
#define	dJUMP_FLY					2		//	날자~~~
#define	dJUMP_LAND					3		//	착륙

#define	dPIERCING_MISSILE_RANGE		1200

//
//	Boomerang Status
enum
{
	eBS_FLY_TO_TARGET,
	eBS_DECREASE_SPEED,
	eBS_RETURN_TO_OWNER,
	eBS_RETURNED,
};

//
//	Fire Ball Type Skill Status
enum
{
	eFBTSS_READY,
	eFBTSS_CAST,
	eFBTSS_FIRE,
};

#define	dSHOOT_WEAPON_FLY			0
#define	dSHOOT_WEAPON_ARRIVED		1
#define	dSHOOT_WEAPON_BLOCKED		2
#define	dSHOOT_WEAPON_RETURNED		3

//
//	Icy Stalagmite Status
enum
{
	eISS_RISE	,	//	솟아 오르는중
	eISS_STAND	,	//	서있다.
	eISS_SINK	,	//	가라 앉는다.
	eISS_DESTROY	//	뽀개진다.
};

class	cActiveSkillBaseInfo
{
public:
	cAbility			m_ability;
	WORD				m_wSerial;					//	시리얼
	CActor				*m_lpCaster,*m_lpTarget;
	CSkill				*m_lpSkill;	//	스킬
	WORD				m_wType;
	CPos				m_posBegin,m_posTarget,m_pos,m_posAcc;			//	현재 위치와 타겟의 위치
	WORD				m_wAnm,m_wDirect,m_wFrame,m_wMaxFrame,m_wFrameCounter,m_wFPS;
	WORD				m_isGetdCP;
	int					m_iLastDistance;
	short				m_sCurAngle;								//	현재 각도
	WORD				m_wAngle,m_wRange;							//	각도가 필요한 기술이 있다. 타겟과의 거리가 필요한 기술이 있다.
	short				m_sHeight,m_sDestHeight,m_sReviseHeight;	//	날아가는 무기의 경우.. 높이..
	CHitInfo			m_hitInfo;	//	타격결과를 가지고 있다.
	std::vector<CHitInfo>	m_vecContinuousHit;
};

class	cAS_DoubleTarget	:	public cActiveSkillBaseInfo
{
public:
	WORD				m_awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	BYTE				m_abMissCount[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	BYTE				m_abCriticalCount[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	BYTE				m_abDoubleCriticalCount[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	BYTE				m_abHardBlowCount[dMAX_CONTINUOUS_HIT_INFO_COUNT];

	WORD				m_wTargetCount;
	WORD				m_wShot,m_wContinuousShotCount;
	WORD				m_wCriticalCount,m_wDoubleCriticalCount,m_wMissCount;
	WORD				m_wHardBlowCount;
	BOOL				cast();
	void				init(WORD *_lpwTargetList,int _iTargetCount,int _iContinuousShotCount,int _iCriticalCount,int _iMissCount,int _iDoubleCriticalCount, int _iHardBlowCount);
	void				getDamage(CHitInfo *_lpResult,int _iTarget);
};

class	cAS_GroupBase:	public cActiveSkillBaseInfo
{
	
public:
	WORD	m_awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	WORD	m_wTargetCount;
	
	void	reset()
	{
		memset(m_awTargetList, 0xffff,sizeof(m_awTargetList));
		m_wTargetCount	=	0;
	}
	void	init(WORD* _lpGroup, WORD _wTargetCount)
	{
		memcpy(m_awTargetList, _lpGroup,sizeof(WORD) * _wTargetCount);
		m_wTargetCount	=	_wTargetCount;
	}
	
};

class	cAS_BouncingMissile	:	public cAS_GroupBase
{
public:
	
	int		fire();
	BOOL	update();				
};



class	cAS_FlyBouncingMissile	:	public cAS_GroupBase
{
public:

	CPos	m_posStart;
	WORD	m_wMissileSize;
	WORD	m_wMissileFrameCounter;
	WORD	m_wCurrentCount;
	void	reset();
	int		fire();
	BOOL	cast();

	BOOL	update();
	BOOL	updateBouncingMissile();
	void	put();
};


class	cAS_WideMissile	:	public cAS_GroupBase
{
public:
	
	BYTE	m_bIsTrigger;
	DWORD	m_dwCastTime;
	WORD	m_wCurrentCount;

	int		fire();
	BOOL	update();				
};

class	cAS_ArcMissile	:	public cActiveSkillBaseInfo
{
public:
	WORD	m_wMissileSize;
	WORD	m_wMissileFrameCounter;
	WORD	m_wHeight;
	int		m_iDistance;	

	int		fire();
	BOOL	update();				
	BOOL	cast();
	void	put();
};


class	cAS_ContinuousHitOfDoppelganger	:	public cActiveSkillBaseInfo
{
public:

	WORD				m_awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	BYTE				m_abMissCount[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	BYTE				m_abCriticalCount[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	BYTE				m_abDoubleCriticalCount[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	BYTE				m_abHardBlowCount[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	BYTE				m_abHitCount[dMAX_CONTINUOUS_HIT_INFO_COUNT];

	BYTE				m_bTargetCount;
	BYTE				m_bShot;
	BYTE				m_bContinuousShotCount;


	void				cast();
	void				init(WORD *_lpwTargetList,int _iTargetCount,int _iContinuousShotCount,int _iCriticalCount,int _iMissCount,int _iDoubleCriticalCount, int _iHardBlowCount);
	void				getDamage(CHitInfo *_lpResult,int _iTarget);
	void				initDoppelganger();

};
class	cAS_ContinuousBit	:	public cActiveSkillBaseInfo
{
public:
	WORD				m_awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	WORD				m_wTargetCount;
	BOOL				cast();
	void				getDamage(CHitInfo *_lpResult,int _iTarget);
};


class	cBigBulletHitInfo
{
public:
	WORD				wActor;
	WORD				wPhysicalDamage,wMagicDamage;	//	데미지
};

class	cAS_BigBullet	:	public cActiveSkillBaseInfo
{
public:
	WORD				m_wIsThrow;
	CActor				*m_lpBullet;
	WORD				m_wTargetCount,m_wTargetIndex,m_wPPS;
	cBigBulletHitInfo	m_aInfo[15];

	int					fire();
	
	BOOL				aimingNextTarget();
	CActor*				getCurrentTarget();
	void				strikeTarget();

	BOOL				cast();
	void				throwTarget(void *_lpInfo,int _iTargetCount);
	void				getDamage(CHitInfo *_lpResult,int _iTarget);
	void				put();

	BOOL				update();
};

class	cAS_LightningWinder	:	public cActiveSkillBaseInfo
{
public:
	WORD				m_wIsFinished;

	int					fire();
	void				shoot(CActor *_lpTarget,int _iPhysicalDamage,int _iMagicDamage,DWORD _dwResult);
	BOOL				update();
};
class	cAS_MultiMissile	:	public cActiveSkillBaseInfo
{
public:
	WORD				m_wIsFinished;

	int					fire();
	void				shoot(CActor *_lpTarget,int _iPhysicalDamage,int _iMagicDamage,DWORD _dwResult);
	BOOL				update();
};

class	cAS_RollingLog	:	public cActiveSkillBaseInfo
{
public:
	int					fire();

	void				cast();
	void				put();

	BOOL				update();
};

class	cAS_GigaLightning	:	public cActiveSkillBaseInfo
{
public:
	enum
	{
		eStep_ReadyMakeCrossTarget,
		eStep_SerchRandomLocation,
		eStep_LockOn,
		eStep_Fire,
		eStep_Action,
		eStep_Finished,
	};

	WORD				m_wGrade,m_wSkillTime;
	WORD				m_wMatchStep,m_wUpdeteSPLineCounter;
	cSPLINE				m_spLine;

	int					fire();
	void				put();
	void				putBottom();

	BOOL				cast();

	BOOL				update();

	BOOL				casterActionFinished();

	void				serchRandomPlace(int _iPattern,int _iMatchStep,int _iRange);
	void				lockOn();
	void				fireSkill();
};

class	CActiveSkill	:	public cActiveSkillBaseInfo
{
public:
	WORD				m_aValue[50];

						CActiveSkill()	
						{
							m_lpCaster	=	NULL;
							m_lpTarget	=	NULL;
							m_lpSkill	=	NULL;	//	스킬
							reset();
						}

	void				reset();

	BOOL				casterStop();	//	캐스터가 동작을 정지했다.
	BOOL				casterDeath();//	캐스터가 죽었다.
	BOOL				targetDeath();	//	타겟이 죽었다.
	inline	BOOL		casterRemove(){return casterDeath();}//	캐스터가 제거 되었다.
	BOOL				targetRemove();	//	타겟이 제거 되었다.

	inline	CSkill		*getSkill()	{return m_ability.getSkill();}

	BOOL				update();	//	업데이트
	BOOL				fire(BOOL _bIsIndependenceCasterSkill=FALSE);
	void				put();
	void				putBottomImage();

	BOOL				fireHitTypeSkill();

	inline	BOOL		isLoopActionSkill()	//	이거 루프 액션 스킬이가?
	{
		if (m_wType	==	eSKILL_TYPE_CONTINUOUS_WHIRLWIND || m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_VALUE || m_wType	==	eSKILL_TYPE_CONTINUOUS_SHOOT|| m_wType	==	eSKILL_TYPE_LAND_MARKER|| m_wType	==	eSKILL_TYPE_INTERVAL_SHOOTER)	return	TRUE;

		return	FALSE;
	}

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillExplosionMissile.cpp
	int					fireExplosionMissileWhichQuick();
	int					fireExplosionMissile();
	void				castExplosionMissile();
	BOOL				updateExplosionMissile();
	void				putExplosionMissile();

	BOOL				explosionMissileExplosion();

	inline	void		setExplosionMissileStatus(int _iStatus) {m_aValue[0] = _iStatus;}
	inline	int			getExplosionMissileStatus()				{return m_aValue[0];}

	inline	void		setExplosionMissileTargetPos(int _iX,int _iY)			{m_aValue[1]	=	 _iX,m_aValue[2]	=	_iY;}
	inline	void		setExplosionMissileFPS(int _iFps)						{m_aValue[3]	=	_iFps;}
	inline	void		setExplosionMissileMaxFrame(int _iFrame)				{m_aValue[4]	=	_iFrame;}
	inline	void		setExplosionMissileFrameCounter(int _iFrameCounter)		{m_aValue[5]	=	_iFrameCounter;}
	inline	void		setExplosionMissileAnm(int _iAnm)						{m_aValue[6]	=	_iAnm;}
	inline	void		setExplosionMissileFrame(int _iFrame)					{m_aValue[7]	=	_iFrame;}
	inline	void		setExplosionMissileFnishFrame(int _iFrame)				{m_aValue[8]	=	_iFrame;}
	inline	void		increaseExplosionMissileFnishFrame()					{m_aValue[8]++;}

	inline	int			getExplosionMissileTargetXPos()							{return m_aValue[1];}
	inline	int			getExplosionMissileTargetYPos()							{return m_aValue[2];}
	inline	int			getExplosionMissileFPS()								{return m_aValue[3];}
	inline	int			getExplosionMissileMaxFrame()							{return m_aValue[4];}
	inline	int			getExplosionMissileFrameCounter()						{return m_aValue[5];}
	inline	int			getExplosionMissileAnm()								{return m_aValue[6];}
	inline	int			getExplosionMissileFrame()								{return	m_aValue[7];}
	inline	int			getExplosionMissileFnishFrame()							{return	m_aValue[8];}

//┃CSkillExplosionMissile.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillContinuousHitOfDoppelganger.cpp
	int					fireContinuousHitOfDoppelganger();
	BOOL				updateContinuousHitOfDoppelganger();

	inline	void		setIsTriggerContinuousHitOfDoppelganger(BOOL _bIsSet)
	{
		m_wAngle	=	_bIsSet;
	}
	inline	BOOL		isTriggerContinuousHitOfDoppelganger()
	{
		return	m_wAngle;
	}
//┃CSkillContinuousHitOfDoppelganger.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃astContinuousBit.cpp
	int					fireContinuousBit();
	BOOL				updateContinuousBit();
//┃astContinuousBit.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃gasTypeSkill.cpp
//┃가스 타입 스킬
	BOOL				fireGasTypeSkill();
	BOOL				castGasTypeSkill();
	BOOL				updateGasTypeSkill();
	BOOL				putGasTypeSkill();

	void				setGasTypeSkillUpkeepTime(DWORD _dwTime)
	{
		(*(DWORD *)(m_aValue+23))	=	_dwTime;
	}
	BOOL				isGasTypeSkillFinishSkillTime(DWORD _dwTime)
	{
		if	(_dwTime	>=	(*(DWORD *)(m_aValue+23)))
			return	TRUE;

		return	FALSE;
	}

	void				setGasTypeSkillOccurActionTime(DWORD _dwTime)
	{
		(*(DWORD *)(m_aValue+21))	=	_dwTime;
	}
	BOOL				isGasTypeSkillOccurActionTime(DWORD _dwTime)
	{
		if	(_dwTime	>=	(*(DWORD *)(m_aValue+21)))
			return	TRUE;

		return	FALSE;
	}

//┃가스 타입 스킬
//┃gasTypeSkill.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillContinuousWhirlwind.cpp
//┃지속형라운딩스킬
	BOOL				fireContinuousWhirlwindSkill();
	BOOL				castContinuousWhirlwindSkill();
	BOOL				updateContinuousWhirlwindSkill();

	void				setContinuousWhirlwindSkillUpkeepTime(DWORD _dwTime)
	{
		(*(DWORD *)(m_aValue+23))	=	_dwTime;
	}
	BOOL				isContinuousWhirlwindSkillFinishSkillTime(DWORD _dwTime)
	{
		if	(_dwTime	>=	(*(DWORD *)(m_aValue+23)))
			return	TRUE;

		return	FALSE;
	}

//┃지속형라운딩스킬
//┃CSkillContinuousWhirlwind.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillExplosionDefendOnImage.cpp
	BOOL				fireExplosionTypeSkillDependOnImage();
	BOOL				castExplosionTypeSkillDependOnImage();
	BOOL				updateExplosionTypeSkillDependOnImage();
	void				putExplosionTypeSkillDependOnImage();
	void				putBottomExplosionTypeSkillDependOnImage();
	inline	void		setExplosionWhichDefendOnImageSkillScale(int _iValue)	{m_aValue[0]	=	_iValue;}
	inline	int			getExplosionWhichDefendOnImageSkillScale()				{return	m_aValue[0];}

	inline	void		setExplosionWhichDefendOnImageSkillZoommingStart()		{m_aValue[1]	=	0;}
	inline	BOOL		isExplosionWhichDefendOnImageSkillZoomming()			{if (m_aValue[1] == 0) return	TRUE; return FALSE;}
	inline	void		setExplosionWhichDefendOnImageSkillZoommingComplete()	{m_aValue[1]	=	1;}
	inline	void		setExplosionWhichDefendOnImageSkillDeclineScale()		{m_aValue[1]	=	2;}
	inline	BOOL		isExplosionWhichDefendOnImageSkillDeclineScale()		{if (m_aValue[1]==	2) return TRUE; return FALSE;}
	inline	void		setExplosionWhichDefendOnImageSkillDeclineScaleFinish()	{m_aValue[1]	=	3;}

	inline	void		setExplosionWhichDefendOnImageSkillFrameStart()			{m_aValue[2]	=	0;}
	inline	void		setExplosionWhichDefendOnImageSkillFrameFinish()		{m_aValue[2]	=	1;}
	inline	BOOL		isExplosionWhichDefendOnImageSkillFrameFinish()			{if (m_aValue[2]==	1) return TRUE; return FALSE;}

	inline	void		setDependOnImageExplosionSkillCastingTime(int _iTime)	{m_aValue[3]	=	_iTime;}
	inline	BOOL		decreaseDependOnImageExplosionSkillCastingTime()
	{
		if (m_aValue[3]	==	0)	return	FALSE;
		m_aValue[3]--;

		return	TRUE;
	}

	inline	int			getDependOnImageExplosionSkillHitCount()				{return	m_aValue[4];}
	inline	void		setDependOnImageExplosionSkillHitCount(int _iTime=0)		{m_aValue[4]	=	0;}
	inline	void		increaseDependOnImageExplosionSkillHitCount()			{m_aValue[4]++;}

//┃CSkillExplosionDefendOnImage.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillImpact.cpp
	int					fireImpactSkill();
	BOOL				castImpactSkill();
	BOOL				updateImpactSkill();	//	미러 타워
	void				putImpactSkill();	//	미러 타워
	void				putImpactSkillIllusion();
//┃CSkillImpact.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillMirrorTower.cpp
	int					fireMirrorTower();
	BOOL				updateMirrorTower();	//	미러 타워
//┃CSkillMirrorTower.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillIntervalShooter.cpp
	int					fireIntervalShooter();
	BOOL				updateIntervalShooter();
	BOOL				updateStrikeIntervalShooter();

	inline	void		setIntervalShooterShotCount(int _iShotCount)			{m_aValue[0]	=	_iShotCount;}
	inline	int			getIntervalShooterShotCount()							{return	m_aValue[0];}
	inline	void		resetIsIntervalShooterStrikeTime()						{m_aValue[10]	=	FALSE;}
	inline	void		setIsIntervalShooterStrikeTime()						{m_aValue[10]	=	TRUE;}
	inline	BOOL		isIsIntervalShooterStrikeTime()							{return	m_aValue[10];}
	inline	void		resetIntervalShooterStrikeCounter()						{m_aValue[11]	=	0;}
	inline	void		increaseIntervalShooterStrikeCounter()					{m_aValue[11]++;}
	inline	int			getIntervalShooterStrikeCounter()						{return m_aValue[11];}
	inline	void		setIntervalShooterActionFPS(int _iFPS)					{m_aValue[13]	=	_iFPS;}
	inline	int			getIntervalShooterActionFPS()							{return m_aValue[13];}

	inline	void		resetIntervalShooterShotCounter()						{m_aValue[12]	=	0;}
	inline	BOOL		increaseIntervalShooterShotCounter()
	{
		m_aValue[12]++;
		if (m_aValue[12]	==	getIntervalShooterShotCount())	return	TRUE;

		return	FALSE;
	}
	inline	int			getIntervalShooterShotCounter()							{return m_aValue[12];}

	inline	void		setIntervalShooterDamage(int _iPhycalDamage,int _iMagicDamage)
	{
		*(int *)&m_aValue[2]	=	_iPhycalDamage;
		*(int *)&m_aValue[4]	=	_iMagicDamage;
	}
	inline	void		getIntervalShooterDamage(int *_lpPhycalDamage,int *_lpMagicDamage)
	{
		*_lpPhycalDamage		=	*(int *)&m_aValue[2];
		*_lpMagicDamage			=	*(int *)&m_aValue[4];
	}

//┃CSkillIntervalShooter.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillDropOnHead.cpp
	int					fireDropOnHeadTypeSkill();
	BOOL				updateDropOnHeadTypeSkill();
	void				castDropOnHeadTypeSkill();
	void				putDropOnHeadTypeSkill();
//┃CSkillDropOnHead.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃astDoubleTarget.cpp
	int					fireDoubleTargetTypeSkill();
	BOOL				updateDoubleTargetTypeSkill();
	void				castDoubleTargetTypeSkill();
//┃astDoubleTarget.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillMissile.cpp
	int					fireMissileTypeSkill();
	void				castMissile();
	BOOL				updateMissileTypeSkill();
	void				putMissileTypeSkill();
	inline	void		putMissile(int _iX,int _iY);

	inline	void		setMissileSize(int _iValue)	{m_aValue[0] = _iValue;}
	inline	int			getMissileSize()			{return	m_aValue[0];}

	inline	void		resetMissileFrameCounter()	{m_aValue[1] = 2;}
	inline	int			getMissileFrameCounter()	{return m_aValue[1];}
	inline	void		increaseMissileFrameCounter(){m_aValue[1]++;}
//┃CSkillMissile.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillSpreadArrow.cpp
	int					fireSpreadArrow();
	void				castSpreadArrow();
	BOOL				updateSpreadArrow();
	inline	void		putSpreadArrow(int _iX,int _iY,int _iDirect,int _iAngle);
	void				putSpreadArrow();
//┃CSkillSpreadArrow.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillFireBall.cpp
	int					fireFireBallTypeSkill();
	BOOL				castFireBallTypeSkill();
	BOOL				updateFireBallTypeSkill();
	void				putFireBallTypeSkill();

	inline	void		setCurrentFireBallShotCount(int _iCount)	{m_aValue[3]	=	_iCount;}
	inline	int			getCurrentFireBallShotCount()				{return	m_aValue[3];}

	inline	void		setFireBallCastTime(int _iTime)		{m_aValue[0]	=	_iTime;}
	inline	int			getFireBallCastTime()				{return	m_aValue[0];}

	inline	void		setFireBallShotCount(int _iCount)	{m_aValue[1]	=	_iCount;}
	inline	int			getFireBallShotCount()				{return	m_aValue[1];}

	inline	void		setFireBallStatus(int _iStatus)		{m_aValue[2]	=	_iStatus;}
	inline	BOOL		getFireBallStatus()					{return	m_aValue[2];}

	inline	void		setFireBallCounter(int _iTimes)		{m_aValue[4]	=	_iTimes;}
	inline	BOOL		increaseFireBallCounter()
	{
		m_aValue[4]++;

		if (m_aValue[4]	>=	getFireBallShotCount())	return	TRUE;

		return	FALSE;
	}
	inline	int			getFireBallCounter()				{return	m_aValue[4];}

	inline	void		setFireBallFireDelay(int _iTimes)	{m_aValue[5]	=	0;}
	inline	void		increaseFireBallFireDelay()			{m_aValue[5]++;}
	inline	int			getFireBallFireDelay()				{return	m_aValue[5];}

//┃CSkillFireBall.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillCharge.cpp
	int					fireChargeTypeSkill();
//┃CSkillCharge.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillBiteHanging.cpp
	int					fireBiteHanging();
	BOOL				updateBiteHanging();
//┃CSkillBiteHanging.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillIcyStalagmite.cpp
	int					fireIcyStalagmite();
	BOOL				castIcyStalagmite();
	BOOL				updateIcyStalagmite();
	void				putIcyStalagmite(int _iX,int _iY);

	BOOL				updateIcyStalagmiteRise();
	BOOL				updateIcyStalagmiteStand();
	BOOL				updateIcyStalagmiteSink();
	BOOL				updateIcyStalagmiteDestroy();

	inline	void		setIcyStalagmiteStatus(int _iStatus){m_aValue[0]	=	_iStatus;}
	inline	int			getIcyStalagmiteStatus()			{return	m_aValue[0];}

	void				sinkIcyStalagmite();
	void				destroyIcyStalagmite();

	inline	void		setReleaseIcyStalagmite()				{m_aValue[1]	=	TRUE;}
	inline	void		resetReleaseIcyStalagmite()				{m_aValue[1]	=	FALSE;}
	inline	BOOL		isReleaseIcyStalagmite()				{return	m_aValue[1];}

	inline	void		setIcyStalagmiteHeight(int _iHeight)	{m_aValue[2] = _iHeight;}
	inline	int			getIcyStalagmiteHeight()				{return m_aValue[2];}

//┃CSkillIcyStalagmite.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillScimitarCutting.cpp
	int					fireScimitarCutting();
	void				castScimitarCutting();
	BOOL				updateScimitarCutting();
	void				putScimitarCutting();

	inline	void		setScimitarCuttingAngleStep(int _iAngleStep)			{m_aValue[0]	=	_iAngleStep;}
	inline	int			getScimitarCuttingAngleStep()							{return m_aValue[0];}

	inline	void		setScimitarCuttingAngle(int _iAngle)					{m_aValue[1]	=	_iAngle;}
	inline	int			getScimitarCuttingAngle()								{return m_aValue[1];}
	inline	BOOL		increaseScimitarCuttingAngle()
	{
		m_aValue[1]		+=	getScimitarCuttingAngleStep();

		if (m_aValue[1]	>=	360)
		{
			m_aValue[1]	-=	360;

			increaseScimitarCuttingRoundingCount();
		}

		if (m_aValue[1]	>=	90)	return	TRUE;

		return	FALSE;
	}

	inline	void		resetScimitarCuttingRoundingCount()						{m_aValue[2]=0;}
	inline	int			getScimitarCuttingRoundingCount()						{return m_aValue[2];}
	inline	void		increaseScimitarCuttingRoundingCount()					{m_aValue[2]++;}
	inline	void		setScimitarCuttingRoundingCount(int _iCount)			{m_aValue[2]=_iCount;}

	inline	void		setScimitarCuttingRoundCount(int _iRoundCount)			{m_aValue[3]=_iRoundCount;}
	inline	int			getScimitarCuttingRoundCount()							{return m_aValue[3];}

	inline	void		setScimitarCuttingRange(int _iAttackRange)				{m_aValue[4]=_iAttackRange;}
	inline	int			getScimitarCuttingRange()								{return m_aValue[4];}
	inline	void		setScimitarCuttingCurrentRange(int _iRange)				{m_aValue[5]=_iRange;}
	inline	int			getScimitarCuttingCurrentRange()						{return m_aValue[5];}
	inline	void		increaseScimitarCuttingCurrentRange(int _iRange)		{m_aValue[5]+=_iRange;}

	inline	void		setScimitarCuttingBeginAngle(int _iAngle)				{m_aValue[6]=_iAngle;}
	inline	int			getScimitarCuttingBeginAngle()							{return m_aValue[6];}

	inline	void		endScimitarCutting()									{setScimitarCuttingRoundingCount(getScimitarCuttingRoundCount()-1);}

	inline	void		resetScimitarCuttingGetCPStatus()						{m_aValue[8] = FALSE;}
	inline	BOOL		getScimitarCuttingGetCPStatus()							{return m_aValue[8];}
	inline	void		setScimitarCuttingGetCPStatus()							{m_aValue[8] = TRUE;}
	inline  void		resetScimitarCuttingIsWhenStartBiteHanging()			{m_aValue[9] = FALSE;}
	inline	void		setScimitarCuttingIsWhenStartBiteHanging()				{m_aValue[9] = TRUE;}
	inline	BOOL		getScimitarCuttingIsWhenStartBiteHanging()				{return m_aValue[9];}
	inline  void		setScimitarCuttingHitCount(int _iCount)					{m_aValue[10] = _iCount;}
	inline	void		increaseScimitarCuttingHitCount()						{++m_aValue[10];}
	inline	int			getScimitarCuttingHitCount()							{return m_aValue[10];}
//┃CSkillScimitarCutting.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/*
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃cSKILL_ILLUSION_MISSILE.cpp
	inline	int			getIllusionMissileCount()			{return	m_aValue[49];}
	inline	void		increaseIllusionMissileCount()		{m_aValue[49]++;}
	inline	BOOL		isGetedCPInIllusionMissileSkill()	{return	m_isGetdCP;}
	inline	void		setGetedCPInIllusionMissileSkill()	{m_isGetdCP	=	TRUE;}
	int					fireIllusionMissileTypeSkill();
	void				castIllusionMissileTypeSkill();
	BOOL				updateIllusionMissileTypeSkill();
	inline	CHitInfo*	getHitInfoByInIllusionMissile(int _iIndex)	{return	(CHitInfo *)m_aValue + _iIndex;}
	void				putIllusionMissileTypeSkill();
//┃cSKILL_ILLUSION_MISSILE.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
*/
	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSKillPiercingMissile.cpp
	void	inline		setPMCasterPos(int _iX,int _iY)	{m_aValue[0] = _iX,m_aValue[1] = _iY;}
	void	inline		getPMCasterPos(CPos *_lpPoint){_lpPoint->x =	m_aValue[0],_lpPoint->y =	m_aValue[1];}

	int					firePiercingMissileTypeSkill();
	void				castPiercingMissile();
	BOOL				updatePiercingMissileTypeSkill();
//	void				putPiercingMissileTypeSkill();
//┃CSKillPiercingMissile.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillGuardianPost.cpp
	int					fireGuardianPost();
	BOOL				castGuardianPost();
	BOOL				updateGuardianPost();
	void				putGuardianPost();
//┃CSkillGuardianPost.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillRadialArc.cpp
	int					fireRadialArc();
	BOOL				castRadialArc();
	BOOL				updateRadialArc();
	void				putRadialArc();
//┃CSkillRadialArc.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillWhirlwind.cpp
	int					fireWhirlwindTypeSkill();
	void				castWhirlwindTypeSkill();
	BOOL				updateWhirlwindTypeSkill();
	void				putWhirlwindTypeSkill();
//┃CSkillWhirlwind.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillWhirlthrusting.cpp
	int					fireWhirlthrusting();
	void				castWhirlthrusting();
	BOOL				updateWhirlthrusting();
	void				putWhirlthrusting();

	inline	void	setWhirlthrustingStep(int _iStep)
	{
		m_aValue[0]	=	_iStep;
	}
	inline	int		getWhirlthrustingStep()
	{
		return	m_aValue[0];
	}

//┃CSkillWhirlthrusting.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillSurprisingRaid.cpp
	int					fireSurprisingRaid();
	void				castSurprisingRaid();
	BOOL				updateSurprisingRaid();
	void				putSurprisingRaid();

	inline	void	setSurprisingRaidStep(int _iStep)
	{
		m_aValue[0]	=	_iStep;
	}
	inline	int		getSurprisingRaidStep()
	{
		return	m_aValue[0];
	}

//┃CSkillSurprisingRaid.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillLaser.cpp
	int					fireLaserTypeSkill();
	void				castLaserTypeSkill();
	BOOL				updateLaserTypeSkill();
	void				putLaserTypeSkill();
//┃cSKILL_LASER.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillLinerChainExplosion.cpp

	inline	void		setLCECasterBodySize(int _iSize){m_aValue[0] = m_ability.getHitRange(NULL)/2;}
	inline	void		setLCEHitRange(int _iRange)		{m_aValue[1] = _iRange;}
	inline	int			getLCECasterBodySize()			{return	m_aValue[0];}	//	몸통 사이즈 리턴
	inline	int			getLCEHitRange()				{return	m_aValue[1];}	//	타격 범위 리턴
	inline	void		setLCEAnm(int _iAnm)			{m_aValue[2]	=	_iAnm;}	//	몸통 사이즈 리턴
	inline	void		setLCEDirect(int _iDirect)		{m_aValue[3]	=	_iDirect;}	//	타격 범위 리턴
	inline	int			getLCEAnm()						{return	m_aValue[2];}	//	몸통 사이즈 리턴
	inline	int			getLCEDirect()					{return	m_aValue[3];}	//	타격 범위 리턴

	int					fireLinerChainExplosionTypeSkill();
	void				castLinerChainExplosionTypeSkill();
	BOOL				updateLinerChainExplosionTypeSkill();
	void				putLinerChainExplosionTypeSkill();
//┃CSkillLinerChainExplosion.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillMachineMissile.cpp
	int					fireMachineMissile();
	void				castMachineMissile();
	BOOL				updateMachineMissile();
	void				putMachineMissile();
	inline	void		setMachineSkillUpdateStatus(int _iStatus)	{m_aValue[0]=_iStatus;}
	inline	int			getMachineSkillUpdateStatus()	{return	m_aValue[0];}
//┃CSkillMachineMissile.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillAwesomeFortress.cpp
	int					fireAwesomeFortress();
	void				strikeAwesomeFortressAttack();
	BOOL				updateAwesomeFortress();
	void				putAwesomeFortress();

	inline	void		setAwesomeFortressBunshinCount(int _iShotCount)	{m_wRange	=	_iShotCount;}
	inline	int			getAwesomeFortressBunshinCount()				{return	m_wRange;}
	inline	void		setIsTriggerAwesomeFortress(BOOL _bIsSet)
	{
		m_wAngle	=	_bIsSet;
	}
	inline	BOOL		isTriggerAwesomeFortress()
	{
		return	m_wAngle;
	}
//┃CSkillAwesomeFortress.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillBunshine.cpp
	int					fireBunshine();
	void				castBunshine();
	BOOL				updateBunshine();
	void				putBunshine();

	inline	void		setBunshineAttackShotCount(int _iShotCount)	{m_aValue[0]	=	_iShotCount;}
	inline	int			getBunshineAttackShotCount()				{return	m_aValue[0];}
//┃CSkillBunshine.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillBoomerang.cpp
	int					fireBoomerang();
	void				castBoomerang();
	BOOL				updateFlyBoomerang();
	BOOL				updateBoomerang();
	void				putBoomerang();
	inline	void		setBoomerangStep(int _iStep)		{m_aValue[0]+=_iStep;}
	inline	void		increaseBoomerangStep()				{m_aValue[0]++;}
	inline	int			getBoomerangStep()					{return m_aValue[0];}
	inline	void		setBoomerangFlyStatus(int _iStatus)	{m_aValue[1] = _iStatus;}
	inline	int			getBoomerangFlyStatus()				{return m_aValue[1];}
	inline	void		setBoomerangSpeed(int _iSpeed)		{m_aValue[2] = _iSpeed;}
	inline	int			getBoomerangSpeed()					{return m_aValue[2];}

	inline	void		setBoomerangStatus(int _iStatus)	{m_aValue[3]=_iStatus;}
	inline	int			getBoomerangStatus()				{return m_aValue[3];}

	inline	void		increaseBoomerangAngle(int _iDelta)	{m_aValue[4]+=_iDelta;}
	inline	void		setBoomerangAngle(int _iAngle)		{m_aValue[4]=_iAngle;}
	inline	int			getBoomerangAngle()					{return m_aValue[4];}

	inline	void		setBoomerangImage(int _iShape)		{m_aValue[5]=_iShape;}
	inline	int			getBoomerangImage()					{return m_aValue[5];}
//┃CSkillBoomerang.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃aidAttackSkill.cpp
	int					updateAidAttackSkill();
//┃aidAttackSkill.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillAid.cpp
	int					fireAidSkill();
	int					updateAidSkill();
//┃CSkillAid.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillBouncingLinear.cpp
	int					fireBouncingLinear();
	BOOL				castBouncingLinear();
	BOOL				aimingNextBouncingLinearTarget();
	int					updateBouncingLinear();
	void				strikeBouncingLinearTarget();
	CActor*				getBouncingLinearCurrentTarget();
	void				putBouncingLinear();

	inline	void		setBouncingLinearTargetCount(int _iCount)	{m_aValue[40]	=	_iCount;}
	inline	int			getBouncingLinearTargetCount()				{return	m_aValue[40];		}
	inline	void		setBouncingLinearTargetIndex(int _iIndex)	{m_aValue[41]	=	_iIndex;}
	inline	int			getBouncingLinearTargetIndex()				{return	m_aValue[41];		}
	inline	void		increaseBouncingLinearTargetIndex()			{m_aValue[41]++;			}

//┃CSkillBouncingLinear.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillContinuousAttack.cpp
	int					fireContinuousAttackTypeSkill();
	BOOL				castContinuousAttackTypeSkill();
	BOOL				updateContinuousAttackTypeSkill();
	void				putContinuousAttackTypeSkill();

	inline	void		setContinuousAttackShotCount(int _iShotCount)	{m_aValue[0]	=	_iShotCount;}
	inline	int			getContinuousAttackShotCount()					{return	m_aValue[0];}
	inline	void		setContinuousAttackActionFPS(int _iFPS)			{m_aValue[1]	=	_iFPS;}
	inline	int			getContinuousAttackActionFPS()					{return	m_aValue[1];}
	inline	void		resetContinuousAttackShotIndex()				{m_aValue[2]	=	0;}
	inline	BOOL		increaseContinuousAttackShotIndex()
	{
		m_aValue[2]++;
		if (m_aValue[2]	>=	getContinuousAttackShotCount())	return	TRUE;

		return	FALSE;
	}
	inline	int			getContinuousAttackShotIndex()					{return	m_aValue[2];}

	inline	void		resetContinuousAttackAnm()						{m_aValue[3]	=	0;}
	inline	void		increaseContinuousAttackAnm()
	{
		m_aValue[3]++;

		if (m_aValue[3] >= m_lpSkill->m_wIncreaseActionAnmCount)	m_aValue[3]	=	0;
	}
	inline	int			getContinuousAttackAnm()	{return	m_aValue[3];}
//┃CSkillContinuousAttack.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillSpecialMissile.cpp
	int					fireSpecialMissile();
	void				castSpecialMissile();
	BOOL				updateSpecialMissile();
	void				putSpecialMissile();
	inline	void		setSpecialSkillUpdateStatus(int _iStatus)		{m_aValue[0]=_iStatus;}
	inline	int			getSpecialSkillUpdateStatus()					{return	m_aValue[0];}

	inline	void		setSpecialMissileUpkeepTime(int _iFrame)		{m_aValue[1]=_iFrame;}
	inline	int			getSpecialMissileUpkeepTime()					{return	m_aValue[1];}
	inline	void		resetSpecialMissileCurrentUpkeepTime()			{m_aValue[2]=0;}
	inline	void		increaseSpecialMissileCurrentUpkeepTime()		{m_aValue[2]++;}
	inline	int			getSpecialMissileCurrentUpkeepTime()			{return	m_aValue[2];}
	inline	void		setIsMagicMissile(BOOL _bIsMagicMissile)			{	m_aValue[3]	=	_bIsMagicMissile;}
	inline	int			getIsMagicMissile()			{return	m_aValue[3];}
	inline	void		setMagicMissileMaxSinHeight(WORD _wHeight)			{	m_aValue[4]	=	_wHeight;}
	inline	int			getMagicMissileMaxSinHeight()			{return	m_aValue[4];}	
	inline	void		setMagicMissileBeginRange(WORD _wRange)			{	m_aValue[5]	=	_wRange;}
	inline	int			getMagicMissileBeginRange()			{return	m_aValue[5];}	
	inline	void		setMagicMissileAngle(WORD _wAngle)			{	m_aValue[6]	=	_wAngle;}
	inline	int			getMagicMissileAngle()			{return	m_aValue[6];}	
	inline	void		setMagicMissileSinHeight(WORD _wHeight)			{	m_aValue[7]	=	_wHeight;}
	inline	int			getMagicMissileSinHeight()			{return	m_aValue[7];}
	inline	void		setMagicMissileAcceleration(WORD _wAcceleration)			{	m_aValue[8]	=	_wAcceleration;}
	inline	int			getMagicMissileAcceleration()			{return	m_aValue[8];}
	inline	void		setMagicMissileSpeedFrame(WORD _wFrame)			{	m_aValue[9]	=	_wFrame;}
	inline	void		increaseMagicMissileSpeedFrame()			{++m_aValue[9];}
	inline	int			getMagicMissileSpeedFrame()			{return	m_aValue[9];}

	BOOL				updateMagicMissile();

//┃CSkillSpecialMissile.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillWaterFall.cpp
	int					fireWaterFall();
	void				castWaterFall();
	BOOL				updateWaterFall();
	void				strikeWaterFall(int _iAngle,int _iDistance);

	inline	void		resetWaterFallUpkeepTime()					{m_aValue[0]	=	0;}
	inline	void		setWaterFallUpkeepTime(int _iUpkeepTime)	{m_aValue[0]	=	_iUpkeepTime;}
	inline	BOOL		decreaseWaterFallUpkeepTime()
	{
		if (m_aValue[0]	==	0)	return	TRUE;

		m_aValue[0]--;

		return	FALSE;
	}

	inline	void		resetWaterFallStrikePeriod()				{m_aValue[1]	=	0;}
	inline	void		setWaterFallStrikePeriod(int _iPeriod)		{m_aValue[1]	=	_iPeriod;}
	inline	int			getWaterFallStrikePeriod()					{return	m_aValue[1];}

	inline	void		resetWaterFallCasting()						{m_aValue[2]	=	FALSE;}
	inline	BOOL		isWaterFallCasting()						{return m_aValue[2];}
	inline	void		setWaterFallCasting()						{m_aValue[2]	=	TRUE;}

	inline	void		resetWaterFallStrikeTime()					{m_aValue[3]	=	0;}
	inline	void		increaseWaterFallStrikeTime()				{m_aValue[3]++;}
	inline	int			getWaterFallStrikeTime()					{return m_aValue[3];}

	inline	void		setWaterFallCastingTime(int _iTime)			{m_aValue[5]	=	_iTime;}
	inline	BOOL		decreaseWaterFallCastingTime()	
	{
		if (m_aValue[5]	==	0)	return	FALSE;

		m_aValue[5]--;

		return	TRUE;
	}
//┃CSkillWaterFall.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillRush.cpp
//┃러쉬 타입 스킬
	int					fireRushTypeSkill();
	void				castRushTypeSkill();
	BOOL				updateRushTypeSkill();
	void				putRushTypeSkill();
//┃러쉬 타입 스킬
//┃CSkillRush.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillContinuousShoot.cpp
//┃연사
	int					fireContinuousShoot();
	BOOL				updateContinuousShoot();
//┃연사
//┃CSkillContinuousShoot.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillBitGlider.cpp
//┃비트 글라이더
	int					fireBitGlider();
	void				castBitGlider();

	BOOL				updateBitGlider();
	void				putBitGlider();
//┃비트 글라이더
//┃CSkillBitGlider.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillLandMarker.cpp
//┃랜드 마커
	int					fireLandMarker();
	BOOL				updateLandMarker();
//┃랜드 마커
//┃CSkillLandMarker.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillTornado.cpp
	int				fireTornadoTypeSkill();
	void			castTornadoTypeSkill();
	BOOL			updateTornadoTypeSkill();
	void			putTornadoTypeSkill();

	inline	void	setTornadoCoreImage(int _iImage)			{m_aValue[0]=_iImage;}
	inline	int		getTornadoCoreImage()						{return m_aValue[0];}

	inline	void	setTornadoCoreFrameCount(int _iFrameCount)	{m_aValue[1] = _iFrameCount;}
	inline	int		getTornadoCoreFrameCount()					{return m_aValue[1];}

	inline	void	setTornadoCoreFPS(int _iFPS)				{m_aValue[2] = _iFPS;}
	inline	int		getTornadoCoreFPS()							{return m_aValue[2];}

//┃CSkillTornado.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillTeleport.cpp
//┃텔레포트
	int				fireTeleport();
	void			castTeleport();
	BOOL			updateTeleport();
	void			putTeleport();
//┃텔레포트
//┃CSkillTeleport.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillJump.cpp
//┃점프
	int				fireJump();
	void			castJump();
	BOOL			updateJump();
	void			putJump();

	inline	void	setJumpStatus(int _iStatus)				{m_aValue[0]	=	_iStatus;}
	inline	int		getJumpStatus()							{return	m_aValue[0];}
//┃점프
//┃CSkillJump.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillJumpAttack.cpp
//┃점프
	int				fireJumpAttack();
	void			castJumpAttack();
	BOOL			updateJumpAttack();
	void			putJumpAttack();
//┃점프
//┃CSkillJumpAttack.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillAlteringHitter.cpp
//┃올터링 히터
	int				fireAlteringHitter();
	void			castAlteringHitter();
	BOOL			updateAlteringHitter();
	void			putAlteringHitter();
	void			setAlteringHitterAttackPeriod(int _iPeriod)
	{
		m_aValue[0]	=	_iPeriod;
	}
	void			setBeginAlteringHitterAttack(BOOL _bIsBegin)
	{
		m_aValue[1]	=	_bIsBegin;
	}
	void			resetAlteringHitterShotCounter()
	{
		m_aValue[2]	=	0;
	}

	void			setAlteringHitterShotCount(int _iCount)
	{
		m_aValue[3]	=	_iCount;
	}

	BOOL			isBegunAlteringHitterAttack()
	{
		return	m_aValue[1];
	}
	BOOL			decreaseAlteringHitterAttackTime()
	{
		if (m_aValue[0] == 0)
			return	TRUE;

		m_aValue[0]--;

		return	FALSE;
	}

	int				getAlteringHitterShotCounter()
	{
		return	m_aValue[2];
	}
	BOOL			increaseAlteringHitterShotCounter()
	{
		m_aValue[2]++;

		if (m_aValue[2] >= getAlteringHitterShotCount())
			return	TRUE;

		return	FALSE;
	}

	int				getAlteringHitterShotCount()
	{
		return	m_aValue[3];
	}
//┃올터링 히터
//┃CSkillAlteringHitter.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillMotion.cpp
//┃모션만 취하면 되는 스킬들 처리
	int					fireMotionSkill();
//┃모션만 취하면 되는 스킬들 처리
//┃CSkillMotion.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSkillTailChaser.cpp
//┃테일 체이서
	//	0    테일 체이서 카운트
	//  1~2  테일 체이서 데미지
	int					fireTailChaserMissile(int _iCountDown,int _iDamage);
	void				castTailChaserMissile();
	BOOL				updateTailChaserMissile();
	void				putTailChaserMissile();

	inline	void		setTailChaserDamage(int _iDamage)
	{
		*(int *)(m_aValue+1) = _iDamage;
	}
	inline	int			getTailChaserDamage()
	{
		return	*(int *)(m_aValue+1);
	}
	inline	void		setTailChaserFireTime(int _iTime){m_aValue[0]=_iTime;}
	inline	BOOL		decreaseTailChaserFireTime()
	{
		if (m_aValue[0]	==	0)	return	TRUE;
		m_aValue[0]--;

		return	FALSE;
	}
//┃테일 체이서
//┃CSkillTailChaser.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	void				operateSkillEffect();

	int					getIndex(char *_strSkillName,int _iJob);
	void				strike(int x,int y);
	void				strike(int actor);
	void				panStrike(int x,int y,int iAngle);
};


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


class	CSkillManager
{
public:
	int				m_iSkillCount,m_iSkillGroupCount;
	BOOL			m_bIsCorrectSkillForAnotherSkillBalance;	//	
	int				m_iMiniPetCombiSkillCount,m_aiMiniPetCombiSkill[32];
	char			m_strLastLoadFileName[256];

	CActiveSkill	m_aActiveSkill[dACTIVE_SKILL_COUNT];
	cSKILL_GROUP	m_aSkillGroup[dSKILL_GROUP_COUNT];
	int				m_iActiveSkillCount,m_iRookie;

					CSkillManager()
					{
						m_strLastLoadFileName[0]	=	NULL;
						m_iSkillCount		=	0;
						m_iSkillGroupCount	=	0;
						m_iActiveSkillCount	=	0;
						m_iRookie			=	0;
					}

	inline	CActiveSkill*	get(int _iIndex)
	{
		if (_iIndex	>=	dACTIVE_SKILL_COUNT	||	_iIndex	<	0)	return	NULL;

		if(m_aActiveSkill[_iIndex].m_wSerial	==	0xffff)	return	NULL;

		return	&m_aActiveSkill[_iIndex];
	}

	CActiveSkill*	getActiveSkill(int _iSkill,int _iCaster);	//	현재 사용중인 스킬

	void			castTailChaserMissiles(CActor *_lpCaster,CActor *_lpTarget,int _iDamage,int _iCount,int _iPeriod,int _iSkill,int _iCorrectDamage);	//	테일 체이서 발동!!
	WORD			castTailChaserMissile(CActor *_lpCaster,CActor *_lpTarget,int _iDamage,int _iPeriod,int _iSkill);	//	테일 체이서 발동!!
	WORD			castMachineMissile(CActor *_lpCaster,CActor *_lpTarget,CHitInfo *_lpHitInfo,int _iSkill,int _iMiniPetOrder);	//	머쉰 미사일 캐스트
	WORD			castSpecialMissile(CActor *_lpCaster,CActor *_lpTarget,int _iX,int _iY,CHitInfo *_lpHitInfo,cAbility *_lpAbility);	//	특수 미사일? 캐스트
	WORD			castCustomMissile(int _iBeginX,int _iBeginY,int _iTargetX,int _iTargetY,cAbility *_lpAbility);	//	특수 미사일? 캐스트
	WORD			castExplosionMissile(CActor *_lpCaster,CActor *_lpTarget,int _iX,int _iY,cAbility *_lpAbility);	//	폭발하는 미사일
	WORD			castExplosion(CActor *_lpCaster,int _iX,int _iY,cAbility *_lpAbility);	//	폭발
	WORD			castContinuousAttackSkill(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iShotCount,int _iFPS);//	연속 공격 캐스팅
	WORD			castAlteringHitter(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iShotCount,int _iFPS);
	WORD			castLandMarkerAttackSkill(CActor *_lpCaster,int _iX,int _iY,cAbility *_lpAbility,int _iShotCount,int _iFPS);//	랜드 마커 공격 캐스팅
	WORD			strikeIntervalShooter(CActor *_lpTarget,cAbility *_lpAbility,int _iPhycalDamage,int _iMagicDamage,int _iShotCount);//	인터벌 슈터 공격
	WORD			castIntervalShooter(CActor *_lpCaster,cAbility *_lpAbility,int _iFPS,int _iShotCount);//	인터벌 슈터 발사
	WORD			castBunshineAttackSkill(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iShotCount);//	연속 공격 캐스팅
	WORD			castAwesomeFortressSkill(CActor *_lpCaster,cAbility *_lpAbility,void *_lpInfo,int _iShotCount);
	WORD			castBouncingLinearSkill(CActor *_lpCaster,cAbility *_lpAbility,void *_lpInfo,int _iShotCount);	//	바운싱 리니어
	WORD			castContinuousHitOfDoppelgangerSkill(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iRange,WORD *_lpwGroup,
					int _iTargetCount,int _iContinuousShotCount,int _iCriticalCount,int _iMissCount,int _iDoubleCriticalCount, int _iHardBlowCount);		// 분신 연속 공격

	WORD			castScimitarCuttingSkill(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iShotCount,int _iRange);

	WORD			castQuick(CActor *_lpCaster,cAbility *_lpAbility);	//	퀵 캐스트
	WORD			castAtActor(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iRange=0xffff);
	WORD			castAtGroup(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iRange,WORD *_lpwGroup,int _iTargetCount,
		int _iContinuousShotCount,int _iCriticalCount,int _iMissCount,int _iDoubleCriticalCount, int _iHardBlowCount);
	WORD			castAtGroupStickedBit(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iRange,WORD *_lpwGroup,int _iTargetCount);

	WORD			castGasTypeSkill(CActor *_lpCaster,int _iX,int _iY,cAbility *_lpAbility);
	WORD			castAtGround(CActor *_lpCaster,int _iX,int _iY,cAbility *_lpAbility,int _iRange=0xffff);
	WORD			castBouncingMissileSkill(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,WORD *_lpTargetList,int _iTargetCount);	//	바운싱 미사일
	WORD			castFlyBouncingMissile(CActor *_lpCaster,CActor *_lpTarget,int _iX,int _iY, cAbility *_lpAbility, WORD* _lpGroup, WORD _iTargetCount);	// 바운싱 미사일 
	WORD			castWideMissileSkill(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,WORD *_lpwGroup,int _iTargetCount);	//	바운싱 미사일
	WORD			castArcMissile(CActor *_lpCaster,CActor *_lpTarget,int _iX,int _iY,CHitInfo *_lpHitInfo,cAbility *_lpAbility);	//	특수 미사일? 캐스트


	BOOL			remove(int _iSerial);

	void			casterStop(int _iSerial);//	캐스터가 멈췄다.
	void			actorRemove(int _iSerial);// 누군가 제거 되었다.
	void			actorDeath(int _iSerial);//	누군가 죽었다.

	void			reset();
	void			update();
	void			draw();
	void			drawBottomImage();

	cAS_FireWall*	getActiveFireWallSkill(int _iSkill,int _iX1,int _iY1,int _iX2,int _iY2);
	int				castFireWallSkill(int _iSkill);
	BOOL			makeSmallFlame(int _iSkill,int _iX1,int _iY1,int _iX2,int _iY2);
	BOOL			makeFireWall(int _iSkill,int _iX1,int _iY1,int _iX2,int _iY2);
	BOOL			removeFireWall(int _iSkill,int _iX1,int _iY1,int _iX2,int _iY2);

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃cSKILL_INIT.cpp
	BOOL			init(char *fn,BOOL _bIsWantMakeSkill2=FALSE);//초기화
	BOOL			initFighterData();//초기화
	BOOL			initForDev();
	BOOL			decodeScripter(char *fn);
	BOOL			decodeScripterForRequireSkill(char *fn);	//	필요한 스킬을 설정하기 위해 한번 더 읽는다.
	BOOL			addSkill(CSkill *skill);

	int				getIndex(char *_strSkillName,int _iJob,BOOL _bIsWantSummonBeastSkill=FALSE);
	BOOL			saveData(char *fn);
	BOOL			loadData(char *fn);
//┃cSKILL_INIT.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

};

#pragma	pack()

class	CEnchantedImageInfo
{
public:
	DWORD		m_dwMask;
	WORD		m_wImage;
	char		m_strName[dSKILL_NAME_LENGTH];
};

class	CEnchantedImage
{
public:
	DWORD					getMask(int _iImage);
	WORD					getImage(int _iMask);

	CEnchantedImageInfo*	get(char *_lpstrName);
};


extern	CEnchantedImageInfo		g_aEnchantedImageInfo[];
extern	CEnchantedImage			g_enchantedImage;
extern	CSkillManager			g_sm;

const	int		c_aMinimumCP[]	=	{0,1500,3000,6000,12000,0x7fffffff};

inline	int		GetCPLevel(int _iCP)	//	현재 CP 레벨을 구한다.
{
	for (int iLevel=4;iLevel>=0;iLevel--)	if (_iCP >= c_aMinimumCP[iLevel])	return	iLevel+1;

	return	0;
}

inline	int		GetFullCP(int _iCP)
{
	int	iLevel	=	GetCPLevel(_iCP);

	if (iLevel	<=	0)	return	0;

	return	c_aMinimumCP[iLevel]	-	c_aMinimumCP[iLevel-1];
}

inline	int		GetFilledCP(int _iCP)
{
	int	iLevel	=	GetCPLevel(_iCP);

	if (iLevel	<=	0)	return	0;

	return	_iCP-c_aMinimumCP[iLevel-1];
}

extern	CSkill*		GetSkill(int _iIndex);

#endif