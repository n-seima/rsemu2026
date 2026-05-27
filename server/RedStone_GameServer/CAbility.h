#ifndef _classAbility_h
#define	_classAbility_h

#include	"CSkill.h"

class	cItem;

#pragma	pack(2)

class	cAbility	:	public CAbilityDefine
{
public:
					cAbility()	{reset();}

	inline	BOOL	isAvail()
	{
		if (m_wSkill	==	0xffff	)	return	FALSE;
		if (m_wLevel	<=	0		)	return	FALSE;

		return	TRUE;
	}
	
	int				getSkillRangeForAI();
	int				getSkillPoint();	//	스킬 포인트?

	inline	BOOL	isWideAreaAttack()	//	범위 공격
	{
		CSkill	*lpSkill	=	getSkill();

		if (lpSkill->m_wHitRange+lpSkill->m_wHitRangePerLevel+lpSkill->m_wWeaponHitRangeCorrect > 100)
			return	TRUE;

		return	FALSE;
	}

	inline	void	set(int _iSkill,int _iLevel){m_wSkill=_iSkill,m_wLevel=_iLevel;}
	inline	void	reset()	{m_wSkill=0xffff,m_wLevel=0;}
	inline	void	copy(cAbility *_lpAbility)	
	{
		if (!_lpAbility)	return;

		m_wSkill	=	_lpAbility->m_wSkill;
		m_wLevel	=	_lpAbility->m_wLevel;
	}


	inline	CSkill	*getSkill()			
	{
		if	(m_wSkill >= dMAX_SKILL )
			return NULL;

		return &g_aSkill[m_wSkill];
	}

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃어빌러티 인포
//┃CAbilityInfo.cpp
	int				getCoolTime();
	int				getCoolTimeIndex();
	BOOL			isThrowAblePotion(cItem *_lpItem);
	BOOL			isIgnoreOptionAttackSpeed();		// 공격속도아이템옵션무시.

	int				getRemainExpForLevelUp();	//	레벨 업까지 남은 경험치
	BOOL			isUseAble(int _iCP,BOOL _isEquipWeapon,BOOL _isEquipOtherWeapon,BOOL _isEquipShield,int _iBulletCount,int _iPetCount,int _iSummonBeastCount);//	사용가능하냐?
	BOOL			isUseAbleToObject(int _iObjectType);//	사용가능하냐?
	int				getGetCP();	//	CP 얻기
	inline	int		getSpentCP()//	예상 소모 CP
	{
		CSkill	*lpSkill	=	getSkill();

		return	lpSkill->m_wSpentCP	+	lpSkill->m_wSpentCPPerLevel*m_wLevel;
	}

	int				getDamageToHP();	//	대미지 일부를 체력으로 흡수
	BOOL			getExtraEffect(int _iIndex,CSkillExtraEffectInfo	*_lpInfo);
	BOOL			isBasicAttackSkill();
	int				getContinuousShotCount(BOOL _bIsCheckDefaultLimit=TRUE);
	inline	int		getContinuousShotPeriod(){return	getSkill()->m_sContiniousShotPeriod;}
	int				getShotCount(BOOL _bIsCheckDefaultLimit=TRUE);
	int				getBulletCount();
	int				getUpkeepTime();	//	지속시간
	int				getExtraUpkeepTime(int _iIndex);	//	지속시간

	int				getCriticalToUndead();
	int				getCriticalToHuman();
	int				getCriticalToDemon();
	int				getCriticalToAnimal();
	int				getCriticalToHolyAnimal();

	int				getInstanceKillToUndead();
	int				getInstanceKillToHuman();
	int				getInstanceKillToDemon();
	int				getInstanceKillToAnimal();
	int				getInstanceKillToHolyAnimal();

	int				getCriticalChance();//크리티컬 확률 구하기 10000 분률
	int				getFixCriticalChance();//고정된 크리티컬 확률 구하기 100분률
	int				getInstanceKillChance();//즉사 확률
	int				getCrushChance();//결정타 확률 구하기
	int				getFixCrushChance();//고정된 결정타 확률 구하기
	int				getHitRange(cItem *_lpWeapon=NULL);//	타격 범위를 구한다.
	int				getMinimumHitRange();//	최소 타격 범위를 구한다.
	int				getHealPoint(BOOL _IsPetResurrection = FALSE, int _iLevel = 0);	//	힐 포인트
	int				getHealPercentageBasedRemainHP();	//	힐 포인트
	int				getCorrectAttackSpeed();	//	공격 속도
	int				getAttackSpeed();			//	공격 속도
	int				getAttackRange(cItem *_lpWeapon,int _iDefaultAttackRange);
	int				getMinimumAttackRange();	
	int				getCorrectHitChance();	//	명중률 보정 수치
	int				getFixHitChance();	//	명중률
	int				getAttackPower();//	공격력
	int				getAttackPowerPercentage(cACTOR *_lpTarget);//	공격력 퍼센티지
	int				getStrikePeriod();
	int				getValueMagicDamagePercent(int _iKind);
	int				getValueMagicDamage(int _iKind);
	int				getMagicDamage(int _iKind, int _iPhysicalDamage,int _iRemainHP=-1, int _iDamageRangeType = eDAMAGE_NORMAL);

	inline	int		getApplyAuraPeriod()
	{
		CSkill	*lpSkill	=	getSkill();

		int iPeriod			=	lpSkill->m_sAuraActivePeriod+lpSkill->m_sAuraActivePeriodPerLevel*m_wLevel;

		if (lpSkill->m_sAuraActivePeriodDecimalMethod	==	eDECIMAL_METHOD_ROUND_OFF)	if (iPeriod%100 >= 50) iPeriod += 100;
		if (lpSkill->m_sAuraActivePeriodDecimalMethod	==	eDECIMAL_METHOD_UP)			if (iPeriod%100 >  0 ) iPeriod += 100;

		return	iPeriod/100;
	}

	inline	BOOL	isAuraSkill()	//	오러스킬?
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)	return	NULL;

		if (lpSkill->m_wType	==	eSKILL_TYPE_AURA)	return	TRUE;

		return	FALSE;
	}

	inline	BOOL	isEnableJob(int _iJob)	//	이 기술을 사용 가능한 직업이냐?(_iJob에 기본 직업이 들어 가야한다.)
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)
			return	FALSE;

		return	lpSkill->isEnableJob(_iJob);
	}

	inline	int		getWideAreaAttackDamagePercent()
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)	return	0;

		return	(lpSkill->m_wWideAreaAttackDamagePercent+lpSkill->m_wWideAreaAttackDamagePercentPerLevel*m_wLevel)/100;
	}
	
	inline	int		getKnockBackDistance()	//	녹백 거리
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)	return	0;

		return	(lpSkill->m_sNockbackDistance+lpSkill->m_sNockbackDistancePerLevel*m_wLevel)/100;
	}
	BOOL			isActivatePassiveSkill(int _iCurrentCP);	//	활성화된 패시브 스킬이냐?
	BOOL			isActivateReactionSkill(int _iCPLevel);		//	활성화된 리액션 스킬이냐?
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

	int				getActivateChance()	//	발동 확률
	{
		CSkill	*lpSkill	=	getSkill();

		if	(!lpSkill)
			return	0;

		int		iActiveChance	=	lpSkill->m_wLimitActiveChance/100;

		iActiveChance	=	min((lpSkill->m_sActiveChance+lpSkill->m_sActiveChancePerLevel*m_wLevel)/100,iActiveChance);

		return	iActiveChance;
	}

	inline	int		getPiercingChance()	//	발동 확률
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)	return	0;

		return	(lpSkill->m_sPiercingChance+lpSkill->m_sPiercingChancePerLevel*m_wLevel)/100;
	}

	inline	int		getHitAngleRange()
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)	return	0;

		return	(lpSkill->m_wHitAngleRange+lpSkill->m_wHitAngleRangePerLevel*m_wLevel)/100;
	}

	inline	int		getStrangeStatusResistance()
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)	return	0;

		return	(lpSkill->m_sStrangeStatusResistance+lpSkill->m_sStrangeStatusResistancePerLevel*m_wLevel)/100;
	}
	inline	int		getStunResistance()
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)	return	0;

		return	(lpSkill->m_sStunResistance+lpSkill->m_sStunResistancePerLevel*m_wLevel)/100;
	}
	
	inline	int		getDefensivePower()
	{
		CSkill	*lpSkill	=	getSkill();

		if	(!lpSkill)
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
	
	inline	int		getHideWeaponTime()
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)	return	0;

		return	(lpSkill->m_sHideWeaponTime+lpSkill->m_sHideWeaponTimePerLevel*m_wLevel)/100;
	}

	inline	int		getDodgeChance()
	{
		CSkill	*lpSkill	=	getSkill();

		if (!lpSkill)	return	0;

		return	(lpSkill->m_sDodgeChance+lpSkill->m_sDodgeChancePerLevel*m_wLevel)/100;
	}

	inline	BOOL	isDecreaseWeaponCountSkill()	{return	getSkill()->m_bf1IsDecreaseWeaponCount;}

	inline	BOOL	isPhysicalDamageSkill()			{return	getSkill()->isPhysicalDamageSkill();}
	inline	BOOL	isFireDamageSkill()				{return	getSkill()->isFireDamageSkill();}
	inline	BOOL	isWaterDamageSkill()			{return	getSkill()->isWaterDamageSkill();}
	inline	BOOL	isWindDamageSkill()				{return	getSkill()->isWindDamageSkill();}
	inline	BOOL	isEarthDamageSkill()			{return	getSkill()->isEarthDamageSkill();}
	inline	BOOL	isLightDamageSkill()			{return	getSkill()->isLightDamageSkill();}
	inline	BOOL	isDarkDamageSkill()				{return	getSkill()->isDarkDamageSkill();}

	inline	BOOL	isRequireWeapon()				{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON;}	//	무기 필요
	inline	BOOL	isRequireShield()				{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_SHIELD;}	//	방패 필요
	inline	BOOL	isRequireBullet()				{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_BULLET;}	//	탄환 필요
	inline	BOOL	isRequireOtherJobWeapon()		{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON2;}	//	탄환 필요
	inline	BOOL	isRequirePet()					{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_PET;}	//	탄환 필요
	inline	BOOL	isRequireSummonBeast()			{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_SUMMON_BEAST;}	//	탄환 필요
	inline	BOOL	isRequirePetOrBeast()			{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_PET_OR_BEAST;}	//	탄환 필요

	inline	BOOL	isRequirePotion()				{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_POTION;}	//	탄환 필요
	inline	BOOL	isRequireFlower()				{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_FLOWER;}	//	탄환 필요
	inline	BOOL	isRequireDrug()					{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_DRUG;}	//	탄환 필요
	inline	BOOL	isRequireCandy()				{return	getSkill()->m_wRequireEquipment&eREQUIRE_EQUIPMENT_FOR_SKILL_CANDY;}	//	탄환 필요

	inline	BOOL	isSnipeShot()					{return	getSkill()->m_wSpecialEffect&eSKILL_SE_SNIPE_SHOT;}		//	스나이프샷이냐?
	inline	BOOL	isSkullShot()					{return	getSkill()->m_wSpecialEffect&eSKILL_SE_SKULL_SHOT;}		//	스컬샷이냐?
	inline	BOOL	isNotInterrupt()				{return	getSkill()->m_wSpecialEffect&eSKILL_SE_NOT_INTERRUPT;}	//	적 공격 무시
	inline	BOOL	isThrowWeapon()					{return	getSkill()->m_wSpecialEffect&eSKILL_SE_THROW_WEAPON;}	//	무기 던지기
	inline	BOOL	isThrowOtherJobWeapon()			{return	getSkill()->m_wSpecialEffect&eSKILL_SE_THROW_OTHER_JOB_WEAPON;}	//	무기 던지기
	inline	BOOL	isJobChange()					{return	getSkill()->m_wSpecialEffect&eSKILL_SE_CHANGE_JOB;}	//	무기 던지기
	inline	BOOL	isHideShield()					{return	getSkill()->m_wSpecialEffect&eSKILL_SE_HIDE_SHIELD;}	//	무기 던지기
	inline	BOOL	isRemoveMisslile()				{return	getSkill()->m_wSpecialEffect&eSKILL_SE_REMOVE_MISSLE;}	//	무기 던지기
	inline	BOOL	isDefenseKnockout()				{return	getSkill()->m_wSpecialEffect&eSKILL_SE_DEFENSE_KNOCKOUT;}	//	무기 던지기
	inline	BOOL	isTaunt()						{return	getSkill()->m_wSpecialEffect&eSKILL_SE_TAUNT;}	//	도발-o-

	inline	BOOL	isIgnoreTargetDodgeCorrectValue()	{return	getSkill()->m_bf1IsIgnoreTargetDodgeCorrectValue;}
	inline	BOOL	isIgnoreTargetBlockingChance()		
	{
		CSkill	*lpSkill	=	getSkill();

		return	lpSkill->m_bf1IsIgnoreTargetBlockingChance+lpSkill->m_bf1IsIgnoreDodgeBlockReaction;
	}
	inline	BOOL	isIgnoreTargetReaction()
	{
		CSkill	*lpSkill	=	getSkill();

		return	lpSkill->m_bf1IsIgnoreTargetBlockingChance;
	}
	inline	BOOL	isIgnoreTargetDodge()
	{
		CSkill	*lpSkill	=	getSkill();

		return	lpSkill->m_bf1IsIgnoreTargetBlockingChance;
	}

	inline	BOOL	isApplyPhysicalDamageOnlyTarget()	{return	getSkill()->m_bf1IsApplyPhysicalDamageOnlyTarget;}	//	도발-o-
	inline	BOOL	isHpSyncSkill()						{return	getSkill()->m_bf1IsHpSyncSkill;}

	inline	int		getLimitPhysicalDamage()
	{
		CSkill	*lpSkill	=	getSkill();

		if	(lpSkill->m_wLimitPhysicalDamage	==	0	||	lpSkill->m_sLimitPhysicalDamagePerLevel	==	0)
			return	0;

		int		iDamage	=	(lpSkill->m_wLimitPhysicalDamage + lpSkill->m_sLimitPhysicalDamagePerLevel * m_wLevel);

		return	max(iDamage,100);
	}
	inline	int		getLimitMagicDamage()
	{
		CSkill	*lpSkill	=	getSkill();

		if	(lpSkill->m_wLimitMagicDamage	==	0	||	lpSkill->m_sLimitMagicDamagePerLevel	==	0)
			return	0;

		int		iDamage	=	(lpSkill->m_wLimitMagicDamage + lpSkill->m_sLimitMagicDamagePerLevel * m_wLevel);

		return	max(iDamage,100);
	}
	inline	int		getIcyStalagmiteHP()	//	얼음석순 체력
	{
		int	iBaseHP	=	getSkill()->m_aValue[0];
		int	iLevelHP=	getSkill()->m_aValue[1];

		return	iBaseHP+iLevelHP*m_wLevel;
	}

	int				getFireDamagePercentage();
	int				getWaterDamagePercentage();
	int				getWindDamagePercentage();
	int				getEarthDamagePercentage();
	int				getLightDamagePercentage();
	int				getDarkDamagePercentage();

	//보조 기술용 + 각성의 열며를 사용한 미니펫의 증가 효과 계산
	int				getCorrectAwakenDamage(int _iAwakenType , int _iMiniPetLevel);
	int				getAwakenActiveChance(int _iAwakenType , int _iMiniPetLevel);

//	int				getAwakenDamagePercentageForAidMagic(int _iMinipetLevel , BOOL _bISAwaken100);
	

	int				getDamageRange(int _iKind, int _iDamageRangeType);
	int				getDamageRange(int _iDamageRange, int _iDamagePerLevel, int _iLevel, int _iDamageRangeType);

	BOOL			isMissileSkill();	//	미사일 공격 스킬이다.
	BOOL			isCloseRangeDamageAttack();	//	근접 대미지 입히는 공격이냐?
	BOOL			isAttackMagicShield();		//	공격하는 마법 쉴드냐?
	CSkillExtraEffect	*getAttackMagicShieldInfo(int _iIndex);	//	공격형 마법 실드 얻어오기

	int				getMirrorTowerValue();		//	미러타워 스킬의 수치
	int				getMirrorTowerAvailRange();	//	미러타워 스킬의 유효 사정거리
	int				getMirrorTowerTargetDamageToCasterCPPercentage();		//	미러타워 스킬의 유효 사정거리
	int				getUnlockDoorLevel();
	int				getUnlockArcaLevel();

//┃어빌러티 인포
//┃CAbilityInfo.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

const	int	c_iMaxBigBulletTargetCount			=	40;

class	cMinHitInfoForBigBullet
{
public:
	WORD			m_wPhysicalDamage;		//	물리 데미지
	WORD			m_wMagicDamage;			//	마법 데미지

	void			reset()
	{
		m_wPhysicalDamage	=	0;
		m_wMagicDamage		=	0;
	}
};

class	cAttackInfoForBigBullet
{
public:
	cMinHitInfoForBigBullet	m_aInfo[c_iMaxBigBulletTargetCount];
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
	DWORD			m_dwPhysicalDamage;		//	물리 데미지
	DWORD			m_dwMagicDamage;			//	마법 데미지
	DWORD			m_dwResultField;			//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션,32 즉사		 데이터 늘림..ㄴ
	DWORD			m_dwAbsorbDamage;
	CContinuousHitInfo	continuousHitInfo;
	inline	void	reset(BOOL _bIsResetResult=FALSE)
	{
		if (_bIsResetResult)
			m_dwResultField	=	0;
		else
			m_dwResultField	=	0xffffffff;

		m_dwPhysicalDamage	=	0;		//	물리 데미지
		m_dwMagicDamage		=	0;		//	마법 데미지
		m_dwAbsorbDamage	=	0;
		continuousHitInfo.reset();
	}
	inline	void	set(int _iPhysicalDamage,int _iMagicDamage,DWORD _dwResult,CContinuousHitInfo* _lpContinuousHitInfo = NULL )
	{
		m_dwPhysicalDamage	=	_iPhysicalDamage;		//	물리 데미지
		m_dwMagicDamage		=	_iMagicDamage;			//	마법 데미지
		m_dwResultField		=	_dwResult;			//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션,32 즉사
		if(_lpContinuousHitInfo)
		{
			memcpy(&continuousHitInfo,_lpContinuousHitInfo, sizeof(CContinuousHitInfo));
		}
	}

	inline	void	copy(CHitInfo *_lpInfo)
	{
		m_dwPhysicalDamage	=	_lpInfo->m_dwPhysicalDamage;		//	물리 데미지
		m_dwMagicDamage		=	_lpInfo->m_dwMagicDamage;		//	마법 데미지
		m_dwResultField		=	_lpInfo->m_dwResultField;		//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션,32 즉사
		continuousHitInfo	=	_lpInfo->continuousHitInfo;
	}

	inline	int		getDamage()				{return	m_dwPhysicalDamage+m_dwMagicDamage;}

	inline	void	setHit()
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;

		m_dwResultField|=dATTACK_RESULT_FIELD_HIT;
	}
	inline	void	setCritical()
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField|=dATTACK_RESULT_FIELD_CRITICAL;
	}
	inline	void	setHitFire()
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField|=dATTACK_RESULT_FIELD_HIT_FIRE;
	}
	inline	void	setCrush()
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField|=dATTACK_RESULT_FIELD_CRUSH;
	}
	inline	void	setBlock()
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField|=dATTACK_RESULT_FIELD_BLOCK;
	}
	inline	void	setNoActionBlock()		
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField|=dATTACK_RESULT_FIELD_NO_ACTION_BLOCK;
	}
	inline	void	setDancingBlockerBlock()
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField|=dATTACK_RESULT_FIELD_DANCING_BLOCKER;
	}
	inline	void	setHitAction()			
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField|=dATTACK_RESULT_FIELD_HIT_ACTION;
	}
	inline	void	setInstanceKill()		
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField|=dATTACK_RESULT_FIELD_INSTANCE_KILL;
	}
	inline	void	setPhysicalDamage()		
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField|=dATTACK_RESULT_FIELD_PHYSICAL_DAMAGE;
	}

	inline	void	setLuckyHit()
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_LUCKY_HIT;
	}
	inline	void	setLuckyDoubleCritical()
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_LUCKY_DOUBLE_CRITICAL;
	}
	inline	void	setLuckyDodge()
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_LUCKY_DODGE;
	}
	inline	void	setLuckyDamage()
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_LUCKY_DAMAGE;
	}
	inline	void	setLuckyDodgeCritical()
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_LUCKY_DODGE_CRITICAL;
	}

	inline	void	setDodge()				
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_FIELD_DODGE;

		if	(isHitAction())	m_dwResultField	-=	dATTACK_RESULT_FIELD_HIT_ACTION;
	}
	inline	void	setImmune()				
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_IMMUNE;
	}
	inline	void	setInvincible()			
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;

		m_dwResultField	|=	dATTACK_RESULT_INVINCIBLE;
	}
	inline	void	setReflection()
	{
		if (m_dwResultField == 0xffffffff )	m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_REFLECTION;
	}
	inline	void	setHitTheOtherSelf()
	{
		if (m_dwResultField == 0xffffffff )	
			m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_HIT_THE_OTHER_SELF;
	}
	inline	void	setAttackToShakle()
	{
		if	(m_dwResultField == 0xffffffff )	
			m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_ATTACK_TO_SHAKLE;
	}
	inline	void	setHardBlow()
	{
		if	(m_dwResultField == 0xffffffff )	
			m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_FIELD_HARD_BLOW;
	}
	inline	void	setIgnoreBlock()
	{
		if	(m_dwResultField == 0xffffffff )	
			m_dwResultField	=	0;
		m_dwResultField	|=	dATTACK_RESULT_FIELD_IGNORE_BLOCK;
	}

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
	inline BOOL		isHitFire()
	{
		if (m_dwResultField&dATTACK_RESULT_FIELD_HIT_FIRE)
			return	TRUE;
		return	FALSE;
	}
	inline	BOOL	isNoActionBlock()		{if (m_dwResultField&dATTACK_RESULT_FIELD_NO_ACTION_BLOCK)	return	TRUE;return	FALSE;}
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
		if	(isReflection()	||	isInvincible()	|| isHitTheOtherSelf()	||	isAttackToShakle())
			return	FALSE;
		if	(m_dwResultField&dATTACK_RESULT_FIELD_INSTANCE_KILL	)	
			return	TRUE;
		return	FALSE;
	}
	inline	BOOL	isPhysicalDamage()		{if (m_dwResultField&dATTACK_RESULT_FIELD_PHYSICAL_DAMAGE)	return	TRUE;return	FALSE;}
	inline	BOOL	isImmune()				{if (m_dwResultField&dATTACK_RESULT_IMMUNE				)	return	TRUE;return	FALSE;}

	inline	BOOL	isLuckyHit()			{if (m_dwResultField&dATTACK_RESULT_LUCKY_HIT			)	return	TRUE;return	FALSE;}
	inline	BOOL	isLuckyDoubleCritical()	{if (m_dwResultField&dATTACK_RESULT_LUCKY_DOUBLE_CRITICAL)	return	TRUE;return	FALSE;}
	inline	BOOL	isLuckyDodge()			
	{
		if	(isInvincible())
			return	FALSE;
		if	(m_dwResultField&dATTACK_RESULT_LUCKY_DODGE			)	
			return	TRUE;
		return	FALSE;
	}
	inline	BOOL	isLuckyDamage()			{if (m_dwResultField&dATTACK_RESULT_LUCKY_DAMAGE			)	return	TRUE;return	FALSE;}
	inline	BOOL	isLuckyDodgeCritical()	{if (m_dwResultField&dATTACK_RESULT_LUCKY_DODGE_CRITICAL	)	return	TRUE;return	FALSE;}
	inline	BOOL	isInvincible()			{if ((m_dwResultField&dATTACK_RESULT_INVINCIBLE)	==	dATTACK_RESULT_INVINCIBLE)	return	TRUE;return	FALSE;}
	inline	BOOL	isReflection()			{if ((m_dwResultField&dATTACK_RESULT_REFLECTION)	==	dATTACK_RESULT_REFLECTION)	return	TRUE;return	FALSE;}
	inline	BOOL	isHitTheOtherSelf()		{if ((m_dwResultField&dATTACK_RESULT_HIT_THE_OTHER_SELF)	==	dATTACK_RESULT_HIT_THE_OTHER_SELF)	return	TRUE;return	FALSE;}
	inline	BOOL	isAttackToShakle()		{if ((m_dwResultField&dATTACK_RESULT_ATTACK_TO_SHAKLE)	==	dATTACK_RESULT_ATTACK_TO_SHAKLE)	return	TRUE;return	FALSE;}

	inline	BOOL	isDodge()				
	{
		if (isReflection())
			return	FALSE;
		return	m_dwResultField&dATTACK_RESULT_FIELD_DODGE;
	}
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


#define	dDAMAGE_ACTOR_INFO_TRAILER_SIZE	64

class	CDamageActorInfo
{
public:
	CHitInfo			m_hitInfo;
	WORD				m_wActorSerial;
};

class	CDamageActorInfoTrailer
{
public:
	WORD				m_wCount;
	CDamageActorInfo	m_aData[dDAMAGE_ACTOR_INFO_TRAILER_SIZE];

	CDamageActorInfoTrailer()
	{
		reset();
	}

	void				reset()
	{
		for (int i=0;i<dDAMAGE_ACTOR_INFO_TRAILER_SIZE;i++)
			m_aData[i].m_hitInfo.reset();

		m_wCount	=	0;	}
};

class	CAttackInfo
{
public:
	WORD				m_wTarget;
	cAbility			m_ability;
	CHitInfo			m_aInfo[dMAX_CONTINUOUS_HIT_INFO_COUNT];

						CAttackInfo()	{reset();}

	void				setBasicData(cAbility *_lpAbility,int _iTarget)
	{
		reset();
		m_ability.copy(_lpAbility);
		m_wTarget	=	_iTarget;
	}

	inline	CHitInfo*	getInfo(int _iIndex){return	&m_aInfo[_iIndex];}
	inline	void		reset()	
	{
		m_wTarget	=	0xffff;
		for (int i=0;i<dMAX_CONTINUOUS_HIT_INFO_COUNT;i++)	m_aInfo[i].reset();
	}

	inline	int			getContinuousShotCount(){return	m_ability.getContinuousShotCount();}
	inline	int			getContinuousShotPeriod(){return	m_ability.getSkill()->m_sContiniousShotPeriod;}
};

#pragma	pack()

#endif