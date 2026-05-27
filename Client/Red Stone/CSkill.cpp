#include	<math.h>
#include	"CSkill.H"
#include	"cJOB.H"
#include	"CActor.H"
#include	"CImageData.H"
#include	"cSHAKE.H"
#include	"CFieldItem.H"
#include	"CSound.H"
#include	"ast_fire_wall.H"

CSkill					g_aSkill[dMAX_SKILL];
CSkillManager			g_sm;
CEnchantedImage			g_enchantedImage;
cFighterAttackInfo		g_fighterAttackInfo;

//	디버그 모드일때 스크립트를 읽어 들이는것이기 때문에 따로 로컬라이징 작업할 필요가 없음.

CEnchantedImageInfo		g_aEnchantedImageInfo[]	=
{
//	dAMEM_DANCING_BLOCKER		,0xffff	,"댄싱 블로커"		,
	dAMEM_FOUNTAIN_BARRIER		,0xffff	,"파운틴 배리어"	,
	dAMEM_SHIMMERING_SHIELD		,0xffff	,"시머링 실드"		,
	dAMEM_MISTIC_FOG			,0xffff	,"미스틱 포그"		,
	dAMEM_TORNADO_SHIELD		,0xffff	,"토네이도 실드"	,
	dAMEM_ROCK_BOUNDING			,0xffff	,"락 바운딩"		,
	dAMEM_LEVITATE				,0xffff	,"레비테이트"		,
	dAMEM_HOT_ENCHANTING		,0xffff	,"핫 인챈팅"		,
	dAMEM_PROTECTING_EVIL		,0xffff	,"프로텍팅 이블"	,
	dAMEM_BERSERK				,0xffff	,"버서커효과"		,
	dAMEM_SANCTUARY				,0xffff	,"생츄어리"			,
	dAMEM_HOLD_PERSON			,0xffff	,"홀드퍼슨"			,
	dAMEM_HOLD_MONSTER			,0xffff	,"홀드몬스터"		,
	dAMEM_EVANGELISM			,0xffff	,"에반겔리즘"		,
	dAMEM_BLESS					,0xffff	,"축복 인챈팅"		,
	dAMEM_HEDGER_HEDGING		,0xffff	,"헷저 헷징"		,
	0xffffffff,
};

DWORD
CEnchantedImage::getMask(int _iImage)
{
	int	iIndex	=	0;

	while(1)
	{
		if (g_aEnchantedImageInfo[iIndex].m_dwMask	==	0xffffffff)
			break;
		if (g_aEnchantedImageInfo[iIndex].m_wImage	==	_iImage)
			return	g_aEnchantedImageInfo[iIndex].m_dwMask;

		iIndex++;
	}

	return	0xffffffff;
}

WORD
CEnchantedImage::getImage(int _iMask)
{
	int	iIndex	=	0;

	while(1)
	{
		if (g_aEnchantedImageInfo[iIndex].m_dwMask	==	0xffffffff		)
			break;
		if (g_aEnchantedImageInfo[iIndex].m_dwMask	==	(DWORD)_iMask	)
			return	g_aEnchantedImageInfo[iIndex].m_wImage;

		iIndex++;
	}

	return	0xffff;
}

CEnchantedImageInfo*
CEnchantedImage::get(char *_lpstrName)
{
	int	iIndex	=	0;

	while(1)
	{
		if (g_aEnchantedImageInfo[iIndex].m_dwMask	==	0xffff)	break;
		if (STRICMP(g_aEnchantedImageInfo[iIndex].m_strName,_lpstrName)	==	0)	return	&g_aEnchantedImageInfo[iIndex];

		iIndex++;
	}

	return	NULL;
}

CSkill*
GetSkill(int _iIndex)
{
	if	(_iIndex	<	0	||	_iIndex	>=	dMAX_SKILL	||	g_aSkill[_iIndex].m_wSerial	==	0xffff)
		return	NULL;

	return	&g_aSkill[_iIndex];
}

void
CSkill::reset()
{
	memset(this,0,sizeof(CSkill));
	memset(m_aRequireSkill,0xff,sizeof(m_aRequireSkill));
	memset(m_aAi,0xff,sizeof(m_aAi));
	m_sound.reset();

	m_bf5MiniPetType			=	nsMiniPetType::Count;
	m_wHitImageEffect			=	0xffff;
	m_wAidAttackImageEffect		=	0xffff;
	m_wHealImageEffect			=	0xffff;
	m_wAddHitImageEffect		=	0xffff;
	m_wCastImageEffect			=	0xffff;
	m_wAidSkillCastingImageEffect=	0xffff;
	m_wSkillImageEffect			=	0xffff;
	m_wCasterHitImageEffect		=	0xffff;
	m_wSwingImageEffect			=	0xffff;

	m_wOutputEffect				=	0xffff;
	m_wReiterationDamageCountSyncWithOverlapAction	=	0;
	m_wImageScale				=	100;
	m_sContiniousShotMaxCount	=	1000;
	m_wSerial					=	0xffff;
	m_wIconIndex				=	0xffff;
	m_wWeaponHitRangeCorrect	=	100;
	m_wDifficultyLevel			=	1;
	m_wType						=	eSKILL_TYPE_NORMAL_HIT;
	m_wAction					=	0xffff;
	m_wAction2					=	0xffff;
	m_wOverlapAction			=	0xffff;
	m_wOverlapAction2			=	0xffff;
	m_wShootImage				=	0xffff;
	m_wTargetMarkImage			=	0xffff;
	m_wMissileFollowImage		=	0xffff;
	m_wMissileHeadImage			=	0xffff;
	m_wExplosionImage			=	0xffff;
	m_wHitImage					=	0xffff;
	m_wMissImage				=	0xffff;
	m_wBottomImage				=	0xffff;
	m_wAidSkillCastingImage		=	0xffff;
	m_wAddHitImage				=	0xffff;
	m_wAidAttackImage			=	0xffff;
	m_wCastImage				=	0xffff;
	m_wCastImageOutputPart		=	eHEOP_FOOT;
	m_wCasterHitImage			=	0xffff;
	m_wSkillImage				=	0xffff;
	m_wMachineImage				=	0xffff;
	m_wEnchantedImage			=	0xffff;
	m_wSwingImage				=	0xffff;
	m_wPaletteIndex				=	0xffff;
	m_wCommandSkill				=	0xffff;
	m_wHealImageOutputPart		=	eHEOP_FOOT;
	m_wWideAreaAttackDamagePercent	=	0xffff;
	m_wHealImage				=	0xffff;
	m_wLimitActiveCount			=	0xffff;
	m_wCoolTimeIndex			=	0xffff;

	m_wCorrectFPS				=	100;
	m_sTargetLevelLimitType1	=	0x7fff;
	m_sHumanTargetLevelLimitType1=	0x7fff;
	m_sAnimalTargetLevelLimitType1=	0x7fff;
	m_sHolyBeastTargetLevelLimitType1=0x7fff;
	m_sDemonTargetLevelLimitType1=	0x7fff;
	m_sUndeadTargetLevelLimitType1=	0x7fff;

	m_wTargetTypeLimit			=	0xffff;
	m_wLimitActiveChance		=	10000;
	m_sActiveChance				=	10000;
	m_wWeaponShootRangeCorrect			=	10000;
	m_wWeaponShootRangeCorrectPerLevel	=	0;
	m_wActiveChanceByInRangeEnemyCount	=	0;
	m_wShootRangeCorrect				=	100;

	m_sFireDamagePercentageLimit	=	30000;
	m_sWaterDamagePercentageLimit	=	30000;
	m_sWindDamagePercentageLimit	=	30000;
	m_sEarthDamagePercentageLimit	=	30000;
	m_sLightDamagePercentageLimit	=	30000;
	m_sDarkDamagePercentageLimit	=	30000;

	m_wApplyExtraEffectCount	=	dEXTRA_EFFECT_COUNT;

	m_sMaximumBlockingChance	=	10000;
	m_wAfterImageType			=	0;
	m_wAfterImageGap			=	20;			//	잔상 간격
	m_wAfterImageFirstImageAlphaDepth	=	32;
	m_wAfterImageCount			=	5;			//	잔상의 수

	m_bf1IsExclusiveAction		=	TRUE;

	m_wEnableJob				=	0xffff;
	m_wSpeed					=	0xffff;
	m_sCorrectAttackPoint		=	10000;

	m_wLimitShotCount			=	100;

	strcpy(m_strName,"valid");
	strcpy(m_strComment,"no comment");
	strcpy(m_strPowerup,"");
	m_bf6NeedState				= dNEED_STATE_EMPTY;		// 필요상태.
	m_bf1IsCancelNeedState		=	FALSE;					// 필요상태해제.
	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
		m_aExtraEffect[i].reset();
}

// CSkill*
// CSkill::getPowerUpSkill()
// {
// 	for ()
// 	{
// 	}
// }

CSkillExtraEffect*
CSkill::getExtraEffectByEffect(int _iEffect)
{
	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		if (m_aExtraEffect[i].m_wEffect	!=	_iEffect)
			continue;

		return	&m_aExtraEffect[i];
	}

	return	NULL;	

}

int
CSkill::getAwakenBoJoDamage(int _iAwakenType, int _iLevel)
{
	int iAddValue			= 0;
	int iAddValuePerLevel	= 0;
	
	if(_iAwakenType == dAWAKENTYPE_100)
	{
		iAddValue			= 	m_wAwakenSkillBonusDamageForBoJoSkill * 2;
		iAddValuePerLevel	=	m_wAwakenSkillBonusDamageForBoJoSkillPerLevel * 2;	
	}
	else if(_iAwakenType == dAWAKENTYPE_50)
	{
		iAddValue			= 	m_wAwakenSkillBonusDamageForBoJoSkill;
		iAddValuePerLevel	=	m_wAwakenSkillBonusDamageForBoJoSkillPerLevel;
	}
	
	return	(iAddValue + iAddValuePerLevel * _iLevel)/100;
}
WORD
CSkillManager::castBouncingMissileSkill(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,WORD *_lpTargetList,int _iTargetCount)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));
	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y;
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y;
	lpAS->m_lpSkill		=	lpSkill;

	if (!lpAS->fire())
		return 0xffff;

	cAS_GroupBase	*lpAS_BM	=	(cAS_GroupBase	*)lpAS;

	lpAS->m_wSerial					=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	lpAS_BM->init(_lpTargetList, _iTargetCount);

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴

}
WORD
CSkillManager::castFlyBouncingMissile(CActor *_lpCaster,CActor *_lpTarget,int _iX,int _iY, cAbility *_lpAbility, WORD* _lpGroup, WORD _iTargetCount)
{

	CSkill		*lpSkill=	_lpAbility->getSkill();

	if	(!lpSkill	||	m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	if	(!lpAS)
		return 0xffff;

	memcpy(&lpAS->m_ability,_lpAbility,sizeof(cAbility));

	lpAS->m_wType		=	eSKILL_TYPE_FLY_BOUNCING_MISSILE;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_iX;
	lpAS->m_pos.y		=	_iY;
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	0xffff;


	if	(m_iRookie	==	dACTIVE_SKILL_COUNT-1)
	{

		for (int i=0,iCount=0;i<dACTIVE_SKILL_COUNT && iCount<m_iActiveSkillCount;i++)
		{
			if	(m_aActiveSkill[i].m_wSerial	==	0xffff)
				continue;
			iCount++;
		}
	}

	if	(!lpAS->fire())
		return 0xffff;

	for(int iIndex=0;iIndex<dMAX_CONTINUOUS_HIT_INFO_COUNT && iIndex < _iTargetCount;++iIndex)
		lpAS->m_vecContinuousHit.push_back(_lpCaster->m_attackInfo.m_aInfo[iIndex]);

	((cAS_GroupBase*)lpAS)->init(_lpGroup, _iTargetCount);

	((cAS_FlyBouncingMissile*)lpAS)->cast();

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}



WORD
CSkillManager::castWideMissileSkill(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,WORD *_lpTargetList,int _iTargetCount)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));
	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y;
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y;
	lpAS->m_lpSkill		=	lpSkill;

	if (!lpAS->fire())
		return 0xffff;

	cAS_GroupBase	*lpAS_BM	=	(cAS_GroupBase	*)lpAS;

	lpAS->m_wSerial					=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	lpAS_BM->init(_lpTargetList, _iTargetCount);

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴

}


WORD
CSkillManager::castArcMissile(CActor *_lpCaster,CActor *_lpTarget,int _iX,int _iY,CHitInfo *_lpHitInfo,cAbility *_lpAbility)
{

	CSkill		*lpSkill=	_lpAbility->getSkill();

	if	(!lpSkill	||	m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	if	(!_lpHitInfo)
		return	0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility,sizeof(cAbility));

	lpAS->m_wType		=	eSKILL_TYPE_ARC_MISSILE;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_iX;
	lpAS->m_pos.y		=	_iY;
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y-_lpTarget->getHeight();
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	0xffff;

	if	(m_iRookie	==	dACTIVE_SKILL_COUNT-1)
	{

		for (int i=0,iCount=0;i<dACTIVE_SKILL_COUNT && iCount<m_iActiveSkillCount;i++)
		{
			if	(m_aActiveSkill[i].m_wSerial	==	0xffff)
				continue;
			iCount++;
		}
	}

	memcpy(&lpAS->m_hitInfo,_lpHitInfo,sizeof(CHitInfo));

	if	(!lpAS->fire())
		return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

WORD
CSkillManager::castAtGroupStickedBit(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iRange,WORD *_lpwGroup,int _iTargetCount)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));
	
	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y-_lpTarget->getHeight();
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	_iRange;

	if (_lpCaster->m_wSerial == _lpTarget->m_wSerial)
		lpAS->m_posTarget.y	=	_lpTarget->m_pos.y;
	
	if (!lpAS->fire())
		return 0xffff;
	cAS_ContinuousBit	*lpAS_CB	=	(cAS_ContinuousBit	*)lpAS;

	lpAS->m_wSerial			=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.
	lpAS_CB->m_wTargetCount	=	_iTargetCount;
	memcpy(lpAS_CB->m_awTargetList, _lpwGroup, sizeof(WORD)*_iTargetCount);

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

int
CSkill::getAwakenHitRange(int _iAwakenType, int _iLevel)
{
	int iAddValue			= 0;
	int iAddValuePerLevel	= 0;
	
	if(_iAwakenType == dAWAKENTYPE_100)
	{
		iAddValue			= 	m_wAwakenSkillBonusRange * 2;
		iAddValuePerLevel	=	m_wAwakenSkillBonusRangePerLevel * 2;
		
	}
	else if(_iAwakenType == dAWAKENTYPE_50)
	{
		iAddValue			= 	m_wAwakenSkillBonusRange;
		iAddValuePerLevel	=	m_wAwakenSkillBonusRangePerLevel;
	}
	
	return	(iAddValue + iAddValuePerLevel * _iLevel)/100;
}

int
CSkill::getAwakenActiveChance(int _iAwakenType, int _iLevel)
{	
	int iAddValue			= 0;
	int iAddValuePerLevel	= 0;
	
	if(_iAwakenType == dAWAKENTYPE_100)
	{
		iAddValue			= 	m_wAwakenSkillBonusActiveChanceForBoJoSKill * 2;
		iAddValuePerLevel	=	m_wAwakenSkillBonusActiveChanceForBoJoSKillPerLevel * 2;
		
	}
	else if(_iAwakenType == dAWAKENTYPE_50)
	{
		iAddValue			= 	m_wAwakenSkillBonusActiveChanceForBoJoSKill;
		iAddValuePerLevel	=	m_wAwakenSkillBonusActiveChanceForBoJoSKillPerLevel;
	}
	
	return	iAddValue + iAddValuePerLevel * _iLevel;
	
}

int
CSkill::getActiveChance(int _iAwakenType , int _iLevel)
{
	int iAddValue			= 0;
	int iAddValuePerLevel	= 0;
	
	if(m_bf1IsNormalMiniPetSkill)
	{
		if(_iAwakenType == dAWAKENTYPE_100)
		{
			iAddValue			= 	m_wAwakenSkillBonusActiveChance * 2;
			iAddValuePerLevel	=	m_wAwakenSkillBonusActiveChancePerLevel * 2;
			
		}
		else if(_iAwakenType == dAWAKENTYPE_50)
		{
			iAddValue			= 	m_wAwakenSkillBonusActiveChance;
			iAddValuePerLevel	=	m_wAwakenSkillBonusActiveChancePerLevel;
		}
	}
	
	return	((m_sActiveChance+iAddValue)+(m_sActiveChancePerLevel +iAddValuePerLevel)*_iLevel);
}

//08-10-06
BOOL
CSkill::getExceptionstrengtheningSkill()
{
	if(m_bf1IsTrap)				//폭발 함정 설치
		return TRUE;

	return FALSE;
}

int
CSkill::getOrderingInSkillAi()
{
	for (int i=0;i<dMAX_SKILL_AI_PATTERN;i++)
	{
		if (m_aAi[i].m_wTrigger	==	eSACS_ORDERING)
			return	m_aAi[i].m_asValue[0];
	}

	return	-1;
}

BOOL
CSkill::isContinuousAttackSkill()
{
	if (m_sContiniousShotCount)
		return	TRUE;
		
	if (m_sContiniousShotCountPerLevel)
		return	TRUE;

	return	FALSE;
}

//
//	왼쪽에 등록하는게 가능한 스킬이냐?
BOOL
CSkill::isEnableLeftSkill()
{
	if (m_wType	==	eSKILL_TYPE_NORMAL_HIT					||
		m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_BY_CHARACTER	||
		m_wType	==	eSKILL_TYPE_MISSILE						||
		m_wType	==	eSKILL_TYPE_PIERCING_MISSILE			||
		m_wType	==	eSKILL_TYPE_CONTINUOUS_SHOOT)
		return	TRUE;

	return	FALSE;
}

//
//	서브 스킬에 가능한 스킬이냐?
BOOL
CSkill::isEnableSubSkill()
{
	if (m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_VALUE	||	
		m_wType	==	eSKILL_TYPE_ATTACK_BY_BUNSHIN	||	
		m_wType	==	eSKILL_TYPE_SCIMITAR_CUTTING	||	
		m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_BY_ILLUSION	||	
		m_wType	==	eSKILL_TYPE_BIT_GLIDER	||	
		m_wType	==	eSKILL_TYPE_BOOMERANG	||	
		m_wType	==	eSKILL_TYPE_FIRE_BALL	||	
		m_wType	==	eSKILL_TYPE_AWESOME_FORTRESS	||	
		m_wType	==	eSKILL_TYPE_DROP_ON_HEAD	||	
		m_wType	==	eSKILL_TYPE_IMPACT	||	
		m_wType	==	eSKILL_TYPE_BIT_GLIDER	||
		m_wType	==	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	||
		m_wType	==	eSKILL_TYPE_EXPLOSION_MISSILE	||
		m_wType	==	eSKILL_TYPE_SPREAD_ARROW	||
		m_wType	==	eSKILL_TYPE_LASER	||	
		m_wType	==	eSKILL_TYPE_WHIRLTHRUSTING	||
		m_wType	==	eSKILL_TYPE_ATTACK_BY_BUNSHIN ||
		m_wType	==	eSKILL_TYPE_BOUNCING_MISSILE
		)
		return	TRUE;

	return	isEnableLeftSkill();
}

//
//	오른쪽에 등록하는게 가능한 스킬이냐?
BOOL
CSkill::isEnableRightSkill()
{
	if	(m_wCastMethod	==	eST_NORMAL)
		return	TRUE;
/*	if (m_wType	==	eSKILL_TYPE_NORMAL_HIT					||
		m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_BY_CHARACTER	||
		m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_BY_ILLUSION	||
		m_wType	==	eSKILL_TYPE_MISSILE						||
		m_wType	==	eSKILL_TYPE_PIERCING_MISSILE			||
		m_wType	==	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		||
		m_wType	==	eSKILL_TYPE_DROP_ON_HEAD				||
		m_wType	==	eSKILL_TYPE_WHIRLWIND					||
		m_wType	==	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	||
		m_wType	==	eSKILL_TYPE_COMPLETE_PROTECTION)
		return	TRUE;
*/
	return	FALSE;
}

//
//	보조 스킬이냐?
BOOL
CSkill::isSupportSkill()
{
	if	(m_wCastMethod	==	eST_SUPPORT		)
		return	TRUE;

	return	FALSE;
}

//
//	패시브 스킬이냐?
BOOL
CSkill::isPassiveSkill()
{
	if (m_wCastMethod	==	eST_PASSIVE	||	m_wCastMethod	==	eST_REACTION	)
		return	TRUE;

	return	FALSE;
}

BOOL
CSkill::isEnableJob(int _iJob)				//	이 기술을 사용 가능한 직업이냐?(_iJob에 기본 직업이 들어 가야한다.)
{
	if (_iJob	>=	dJOB_KELBY_1	&&	_iJob	<=	dJOB_HEDGER_3)
		_iJob	=	dJOB_MONSTER_START;

	if	(m_wEnableJob	==	dJOB_CODE_MONSTER)
	{
		if (_iJob	>=	dJOB_MONSTER_START)
			return	TRUE;

		return	FALSE;
	}

	if	(m_wEnableJob	==	0xffff	||	m_wEnableJob	==	dJOB_CODE_ALL)
		return	TRUE;
	if	(m_wEnableJob	==	_iJob)
		return	TRUE;

	return	FALSE;
}

void
CSkillManager::reset()
{
	for (int i=0;i<dACTIVE_SKILL_COUNT;i++)
		m_aActiveSkill[i].reset();

	m_iActiveSkillCount	=	0;
}

//
//	테일 체이서 발동!!
void
CSkillManager::castTailChaserMissiles(CActor *_lpCaster,CActor *_lpTarget,int _iDamage,int _iCount,int _iPeriod,int _iSkill,int _iCorrectDamage)
{
	int	iRemainTime	=	0,i;

	_iDamage		*=	100;

	for (i=0;i<_iCount;i++)
	{
		iRemainTime	=	_iPeriod*i;
		_iDamage	=	_iDamage*_iCorrectDamage/100;

		castTailChaserMissile(_lpCaster,_lpTarget,_iDamage,iRemainTime,_iSkill);
	}
}

WORD
CSkillManager::castContinuousHitOfDoppelgangerSkill(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iRange,WORD *_lpwGroup,int _iTargetCount,int _iContinuousShotCount,int _iCriticalCount,int _iMissCount,int _iDoubleCriticalCount, int _iHardBlowCount)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y-_lpTarget->getHeight();
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	_iRange;

	lpAS->setIsTriggerContinuousHitOfDoppelganger(FALSE);
	if (_lpCaster->m_wSerial == _lpTarget->m_wSerial)
		lpAS->m_posTarget.y	=	_lpTarget->m_pos.y;
	
	if (!lpAS->fire())
		return 0xffff;
	
	cAS_ContinuousHitOfDoppelganger	*lpAS_DT	=	(cAS_ContinuousHitOfDoppelganger	*)lpAS;
	lpAS_DT->init(_lpwGroup,_iTargetCount,_iContinuousShotCount,_iCriticalCount,_iMissCount,_iDoubleCriticalCount, _iHardBlowCount);


	lpAS->m_wSerial			=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.
	
	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}
//
//	테일 체이서 발동!!
WORD
CSkillManager::castTailChaserMissile(CActor *_lpCaster,CActor *_lpTarget,int _iDamage,int _iPeriod,int _iSkill)
{
	cAbility	ability;

	ability.set(_iSkill,1);

	CSkill		*lpSkill=	ability.getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,&ability,sizeof(cAbility));

	lpAS->m_wType		=	eSKILL_TYPE_TAIL_CHASER;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y-_lpTarget->getHeight();
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	0xffff;

	if (lpAS->fireTailChaserMissile(_iPeriod,_iDamage)	!=	eSKILL_FIRE_RESULT_OK)	return	0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	머신건 미사일 발사
WORD
CSkillManager::castMachineMissile(CActor *_lpCaster,CActor *_lpTarget,CHitInfo *_lpHitInfo,int _iSkill,int _iMiniPetOrder)
{
	cAbility	ability;

	ability.set(_iSkill,1);

	CSkill		*lpSkill=	ability.getSkill();

	if	(m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,&ability,sizeof(cAbility));

	lpAS->m_wType		=	eSKILL_TYPE_MACHINE_MISSILE;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y-_lpTarget->getHeight();
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	0xffff;
	lpAS->m_aValue[1]	=	_iMiniPetOrder;

	memcpy(&lpAS->m_hitInfo,_lpHitInfo,sizeof(CHitInfo));

	if	(!lpAS->fire())
		return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴

}

//
//	연속 공격 캐스팅
WORD
CSkillManager::castContinuousAttackSkill(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iShotCount,int _iFPS)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();;
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y-_lpTarget->getHeight();;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->setContinuousAttackShotCount(_iShotCount);
	lpAS->setContinuousAttackActionFPS(_iFPS);

	if (!lpAS->fire())	return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	올터링 히터 캐스팅
WORD
CSkillManager::castAlteringHitter(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iShotCount,int _iFPS)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y;

	if (_lpTarget)
	{
		lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
		lpAS->m_posTarget.y	=	_lpTarget->m_pos.y;
	}

	lpAS->m_lpSkill		=	lpSkill;
	lpAS->setAlteringHitterShotCount(_iShotCount);

	if (!lpAS->fire())
		return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}


//
//	랜드마커 캐스팅
WORD
CSkillManager::castLandMarkerAttackSkill(CActor *_lpCaster,int _iX,int _iY,cAbility *_lpAbility,int _iShotCount,int _iFPS)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	NULL;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_posTarget.x	=	_iX;
	lpAS->m_posTarget.y	=	_iY;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->setContinuousAttackShotCount(_iShotCount);
	lpAS->setContinuousAttackActionFPS(_iFPS);

	if (!lpAS->fire())	return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	인터벌 슈터 공격 작렬!!
WORD
CSkillManager::strikeIntervalShooter(CActor *_lpTarget,cAbility *_lpAbility,int _iPhycalDamage,int _iMagicDamage,int _iShotCount)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	NULL;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpTarget->m_pos.x;
	lpAS->m_pos.y		=	_lpTarget->m_pos.y-_lpTarget->getHeight()-400;
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y-_lpTarget->getHeight();
	lpAS->m_lpSkill		=	lpSkill;

	lpAS->setIsIntervalShooterStrikeTime();
	lpAS->resetIntervalShooterStrikeCounter();
	lpAS->resetIntervalShooterShotCounter();
	lpAS->setIntervalShooterShotCount(_iShotCount);
	lpAS->setIntervalShooterDamage(_iPhycalDamage,_iMagicDamage);

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	인터벌 슈터 발사
WORD
CSkillManager::castIntervalShooter(CActor *_lpCaster,cAbility *_lpAbility,int _iFPS,int _iShotCount)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	NULL;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_posTarget.x	=	_lpCaster->m_pos.x;
	lpAS->m_posTarget.y	=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->setIntervalShooterActionFPS(_iFPS);
	lpAS->setContinuousAttackShotCount(_iShotCount);

	if (!lpAS->fire())	return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

WORD
CSkillManager::castBouncingLinearSkill(CActor *_lpCaster,cAbility *_lpAbility,void *_lpInfo,int _iShotCount)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	NULL;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y;
	lpAS->m_lpSkill		=	lpSkill;
	memcpy(lpAS->m_aValue,_lpInfo,sizeof(lpAS->m_aValue));

	lpAS->setBouncingLinearTargetCount(_iShotCount);

	if (!lpAS->fire())
		return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

WORD
CSkillManager::castAwesomeFortressSkill(CActor *_lpCaster,cAbility *_lpAbility,void *_lpInfo,int _iShotCount)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	NULL;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_lpSkill		=	lpSkill;
	memcpy(lpAS->m_aValue,_lpInfo,sizeof(lpAS->m_aValue));

	lpAS->setAwesomeFortressBunshinCount(_iShotCount);
	lpAS->setIsTriggerAwesomeFortress(FALSE);

	if	(!lpAS->fire())
		return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	분신 공격 
WORD
CSkillManager::castBunshineAttackSkill(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iShotCount)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();;
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y-_lpTarget->getHeight();;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->setBunshineAttackShotCount(_iShotCount);

	if (!lpAS->fire())
		return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	분신 공격 
WORD
CSkillManager::castScimitarCuttingSkill(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iShotCount,int _iRange)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y-_lpTarget->getHeight();
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->setScimitarCuttingRoundCount(_iShotCount);
	lpAS->setScimitarCuttingRange(_iRange);

	if (!lpAS->fire())	return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	특수 미사일(?) 발사
WORD
CSkillManager::castSpecialMissile(CActor *_lpCaster,CActor *_lpTarget,int _iX,int _iY,CHitInfo *_lpHitInfo,cAbility *_lpAbility)
{
	mBeginTLog();

	CSkill		*lpSkill=	_lpAbility->getSkill();

	if	(!lpSkill	||	m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	if	(!_lpHitInfo)
		return	0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility,sizeof(cAbility));

	lpAS->m_wType		=	eSKILL_TYPE_SPECIAL_MISSILE;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_iX;
	lpAS->m_pos.y		=	_iY;
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y-_lpTarget->getHeight();
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	0xffff;

	if	(m_iRookie	==	dACTIVE_SKILL_COUNT-1)
	{
		g_eh.addLog("csm all skill");

		for (int i=0,iCount=0;i<dACTIVE_SKILL_COUNT && iCount<m_iActiveSkillCount;i++)
		{
			if	(m_aActiveSkill[i].m_wSerial	==	0xffff)
				continue;

			g_eh.addLog("%d",m_aActiveSkill[i].m_lpSkill->m_wSerial);

			iCount++;
		}

		
	}

	g_eh.addLog("csm %s %d",lpSkill->m_strName,m_iRookie);

	g_eh.addLog("csm check lpAS");

	if	(!lpAS)
		g_eh.addLog("csm !lpAS");

	g_eh.addLog("csm check _lpHitInfo");

	if	(!_lpHitInfo)
		g_eh.addLog("csm !_lpHitInfo");

	g_eh.addLog("csm memcpy(&lpAS->m_hitInfo");

	memcpy(&lpAS->m_hitInfo,_lpHitInfo,sizeof(CHitInfo));

	g_eh.addLog("csm cp 1");

	if	(!lpAS->fire())
		return 0xffff;

	g_eh.addLog("csm cp 2");

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	특수 미사일? 캐스트
WORD
CSkillManager::castCustomMissile(int _iBeginX,int _iBeginY,int _iTargetX,int _iTargetY,cAbility *_lpAbility)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility,sizeof(cAbility));

	lpAS->m_wType		=	eSKILL_TYPE_SPECIAL_MISSILE;
	lpAS->m_lpCaster	=	NULL;
	lpAS->m_lpTarget	=	NULL;
	lpAS->m_pos.x		=	_iBeginX;
	lpAS->m_pos.y		=	_iBeginY;
	lpAS->m_posTarget.x	=	_iTargetX;
	lpAS->m_posTarget.y	=	_iTargetY;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	0xffff;

	if (!lpAS->fire())	return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	폭발형 미사일 발사
WORD
CSkillManager::castExplosionMissile(CActor *_lpCaster,CActor *_lpTarget,int _iX,int _iY,cAbility *_lpAbility)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility,sizeof(cAbility));

	lpAS->m_wType		=	eSKILL_TYPE_EXPLOSION_MISSILE;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	NULL;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y;
	lpAS->m_posTarget.x	=	_iX;
	lpAS->m_posTarget.y	=	_iY;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	0xffff;
	lpAS->m_lpTarget	=	_lpTarget;

	lpAS->fireExplosionMissileWhichQuick();
	g_esm.playCastingSound(lpSkill);
	lpAS->operateSkillEffect();

	_lpCaster->m_wIsDontHitAction	=	lpSkill->isNotInterrupt();

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	폭발
WORD
CSkillManager::castExplosion(CActor *_lpCaster,int _iX,int _iY,cAbility *_lpAbility)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if	(m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility,sizeof(cAbility));

	lpAS->m_wType		=	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	NULL;
	lpAS->m_pos.x		=	_iX;
	lpAS->m_pos.y		=	_iY;
	lpAS->m_posTarget.x	=	_iX;
	lpAS->m_posTarget.y	=	_iY;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	0xffff;

	BOOL	bIsExclusiveAction	=	_lpCaster->isExclusiveAction();

	if	(!lpAS->fire(TRUE))
		return 0xffff;

	_lpCaster->setExclusiveAction(bIsExclusiveAction);

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	퀵 캐스트
WORD
CSkillManager::castQuick(CActor *_lpCaster,cAbility *_lpAbility)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	if	(lpSkill->m_wType	==	eSKILL_TYPE_LIGHTNING_WINDER)
		m_iRookie	=	dACTIVE_SKILL_COUNT-1;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y;
	lpAS->m_posTarget.x	=	_lpCaster->m_pos.x;
	lpAS->m_posTarget.y	=	_lpCaster->m_pos.y;
	lpAS->m_lpTarget	=	_lpCaster;

	if	(!lpAS->fire())
		return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	if	(lpSkill->m_wType	==	eSKILL_TYPE_LIGHTNING_WINDER)
		m_iRookie	=	-1;

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	스킬을 액터한테 사용한다.
WORD
CSkillManager::castAtActor(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iRange)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

//	기본 공격 - 이넘은 따로 등록 안해도 된다. 걍 처리..
	if (lpSkill->m_wType	==	eSKILL_TYPE_NORMAL_HIT)
	{
		_lpCaster->action(_lpTarget->m_pos.x,_lpTarget->m_pos.y,lpSkill->m_wAction);
		_lpCaster->setExclusiveAction(TRUE);	//	기술 사용하는건 에니메이션 하나로 끝난다. 일단은.. --;;
		g_esm.playCastingSound(lpSkill);

		_lpCaster->operateSkillEffectForCaster(_lpAbility);

		_lpCaster->m_wIsDontHitAction	=	lpSkill->isNotInterrupt();
		return	dSKILL_BASIC_ATTACK;
	}	//	근접 공격 스킬

//	몸뚱아리 정보-_-에 따라서 타격을 주는 스킬
	if (lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_BY_CHARACTER)
	{
		_lpCaster->action(_lpTarget->m_pos.x,_lpTarget->m_pos.y,lpSkill->m_wAction);
		_lpCaster->setExclusiveAction(TRUE);	//	기술 사용하는건 에니메이션 하나로 끝난다. 일단은.. --;;

		_lpCaster->m_isGetCPByContinuousHitFirstAttack	=	FALSE;
		_lpCaster->m_bContinuousHitCounter				=	0;
		_lpCaster->m_bIllusionAttackFrame				=	0;

		_lpCaster->operateSkillEffectForCaster(_lpAbility);

		g_esm.playCastingSound(lpSkill);

		_lpCaster->m_wIsDontHitAction	=	lpSkill->isNotInterrupt();
		return	dSKILL_CONTINUOUS_HIT_ATTACK;
	}	//	근접 공격 스킬

//	잔상 공격.. -o-
	if (lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_BY_ILLUSION)
	{
		_lpCaster->action(_lpTarget->m_pos.x,_lpTarget->m_pos.y,lpSkill->m_wAction);
		_lpCaster->setExclusiveAction(TRUE);	//	기술 사용하는건 에니메이션 하나로 끝난다. 일단은.. --;;

		_lpCaster->m_isGetCPByContinuousHitFirstAttack	=	FALSE;
		_lpCaster->m_bContinuousHitCounter				=	0;
		_lpCaster->m_bIllusionAttackFrame				=	0;

		_lpCaster->operateSkillEffectForCaster(_lpAbility);

		g_esm.playCastingSound(lpSkill);

		_lpCaster->m_wIsDontHitAction	=	lpSkill->isNotInterrupt();
		return	dSKILL_ILLUSION_ATTACK;
	}	//	근접 공격 스킬

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	if	(lpSkill->m_wType	==	eSKILL_TYPE_LIGHTNING_WINDER)
		m_iRookie	=	dACTIVE_SKILL_COUNT-1;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y-_lpTarget->getHeight();
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	_iRange;

	if (_lpCaster->m_wSerial == _lpTarget->m_wSerial)
		lpAS->m_posTarget.y	=	_lpTarget->m_pos.y;

	if	(!lpAS->fire())
		return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	if	(lpSkill->m_wType	==	eSKILL_TYPE_LIGHTNING_WINDER)
		m_iRookie	=	-1;

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	스킬을 액터한테 사용한다.
WORD
CSkillManager::castAtGroup(CActor *_lpCaster,CActor *_lpTarget,cAbility *_lpAbility,int _iRange,WORD *_lpwGroup,int _iTargetCount,int _iContinuousShotCount,
						   int _iCriticalCount,int _iMissCount,int _iDoubleCriticalCount, int _iHardBlowCount)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y-_lpTarget->getHeight();
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	_iRange;

	if (_lpCaster->m_wSerial == _lpTarget->m_wSerial)
		lpAS->m_posTarget.y	=	_lpTarget->m_pos.y;
	
	if (!lpAS->fire())
		return 0xffff;

	cAS_DoubleTarget	*lpAS_DT	=	(cAS_DoubleTarget	*)lpAS;

	lpAS->m_wSerial			=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.
	
	lpAS_DT->init(_lpwGroup,_iTargetCount,_iContinuousShotCount,_iCriticalCount,_iMissCount,_iDoubleCriticalCount, _iHardBlowCount);

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	필드에다 공격한다.
WORD
CSkillManager::castGasTypeSkill(CActor *_lpCaster,int _iX,int _iY,cAbility *_lpAbility)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_posTarget.x	=	_iX;
	lpAS->m_posTarget.y	=	_iY;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	0;

	lpAS->castGasTypeSkill();
	lpAS->m_wType		=	eSKILL_TYPE_GAS;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	필드에다 공격한다.
WORD
CSkillManager::castAtGround(CActor *_lpCaster,int _iX,int _iY,cAbility *_lpAbility,int _iRange)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)	return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));

	lpAS->m_wType		=	lpSkill->m_wType;
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y-_lpCaster->getHeight();
	lpAS->m_posTarget.x	=	_iX;
	lpAS->m_posTarget.y	=	_iY;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wRange		=	_iRange;

	if (!lpAS->fire())	return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	_iSerial번째거 제거
BOOL
CSkillManager::remove(int _iSerial)
{
	if	(_iSerial	<	m_iRookie)
		m_iRookie	=	_iSerial;

	if	(m_aActiveSkill[_iSerial].m_wSerial	==	0xffff)
		return	FALSE;
	
	m_aActiveSkill[_iSerial].reset();

	m_iActiveSkillCount	--;	

	return	TRUE;
}

//
//	업데이트
void
CSkillManager::update()
{
	for (int i=0,iCount=0;i<dACTIVE_SKILL_COUNT && iCount<m_iActiveSkillCount;i++)
	{
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
			continue;

		if (m_aActiveSkill[i].update())
		{
			remove(i);

			continue;
		}

		iCount++;
	}
}

//
//	그려
void
CSkillManager::draw()
{
	for (int i=0,iCount=0;i<dACTIVE_SKILL_COUNT && iCount<m_iActiveSkillCount;i++)
	{
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)	continue;

		m_aActiveSkill[i].put();
	
		iCount++;
	}
}

//
//	그려
void
CSkillManager::drawBottomImage()
{
	for (int i=0,iCount=0;i<dACTIVE_SKILL_COUNT && iCount<m_iActiveSkillCount;i++)
	{
		if	(m_aActiveSkill[i].m_wSerial	==	0xffff)
			continue;

		m_aActiveSkill[i].putBottomImage();
	
		iCount++;
	}
}

//
//	캐스터가 멈췄다.
void
CSkillManager::casterStop(int _iSerial)
{
	if	(m_aActiveSkill[_iSerial].casterStop())
		remove(_iSerial);
}

//
//	누군가 죽었다.
void
CSkillManager::actorDeath(int _iSerial)
{
	int	i,iCount;

	for (i=0,iCount=0;i<dACTIVE_SKILL_COUNT && iCount < m_iActiveSkillCount;i++)
	{
		CActiveSkill	*lpAciveSkill	=	&m_aActiveSkill[i];

		if (lpAciveSkill->m_wSerial	==	0xffff	)	continue;
		if (lpAciveSkill->m_lpCaster	)
			if (lpAciveSkill->m_lpCaster->m_wSerial	==	_iSerial)
				if (lpAciveSkill->casterDeath())
				{
					remove(i);
					continue;
				}

		if (lpAciveSkill->m_lpTarget)
			if (lpAciveSkill->m_lpTarget->m_wSerial	==	_iSerial)
				if (lpAciveSkill->targetDeath())
				{
					remove(i);
					continue;
				}

		iCount++;
	}
}

//
//	누군가 제거 되었다.
void
CSkillManager::actorRemove(int _iSerial)
{
	int	i,iCount;

	for (i=0,iCount=0;i<dACTIVE_SKILL_COUNT && iCount < m_iActiveSkillCount;i++)
	{
		CActiveSkill	*lpAciveSkill	=	&m_aActiveSkill[i];

		if (lpAciveSkill->m_wSerial	==	0xffff)	continue;

		if (lpAciveSkill->m_lpCaster)
			if (lpAciveSkill->m_lpCaster->m_wSerial	==	_iSerial)
				if (lpAciveSkill->casterRemove())
				{
					remove(i);
					continue;
				}

		if (lpAciveSkill->m_lpTarget)
			if (lpAciveSkill->m_lpTarget->m_wSerial	==	_iSerial)
				if (lpAciveSkill->targetRemove())
				{
					remove(i);
					continue;
				}

		iCount++;
	}
}

CActiveSkill*
CSkillManager::getActiveSkill(int _iSkill,int _iCaster)
{
	int	i,iCount;

	for (i=0,iCount=0;i<dACTIVE_SKILL_COUNT && iCount < m_iActiveSkillCount;i++)
	{
		CActiveSkill	*lpAciveSkill	=	&m_aActiveSkill[i];

		if	(lpAciveSkill->m_wSerial	==	0xffff)
			continue;

		if (lpAciveSkill->m_lpCaster)
			if (lpAciveSkill->m_lpCaster->m_wSerial	==	_iCaster)
			{
				if	(lpAciveSkill->m_lpSkill->m_wSerial	==	_iSkill)
					return	lpAciveSkill;
			}

		iCount++;
	}

	return	NULL;
}

cAS_FireWall*
CSkillManager::getActiveFireWallSkill(int _iSkill,int _iX1,int _iY1,int _iX2,int _iY2)
{
	for (int i=0,iCount=0;i<dACTIVE_SKILL_COUNT && iCount < m_iActiveSkillCount;i++)
	{
		CActiveSkill	*lpAciveSkill	=	&m_aActiveSkill[i];

		if	(lpAciveSkill->m_wSerial	==	0xffff)
			continue;

		if	(lpAciveSkill->m_wType	!=	eSKILL_TYPE_FIRE_WALL)
			continue;

		cAS_FireWall	*lpFireWall		=	(cAS_FireWall	*)lpAciveSkill;

		if	(lpFireWall->m_wX1	!=	_iX1)
			continue;
		if	(lpFireWall->m_wY1	!=	_iY1)
			continue;
		if	(lpFireWall->m_wX2	!=	_iX2)
			continue;
		if	(lpFireWall->m_wY2	!=	_iY2)
			continue;

		return	lpFireWall;
	}

	return	NULL;
}

int
CSkillManager::castFireWallSkill(int _iSkill)
{
	CSkill	*lpSkill	=	&g_aSkill[_iSkill];

	if	(m_iActiveSkillCount	>=	dACTIVE_SKILL_COUNT)
		return 0xffff;

	CActiveSkill	*lpAS		=	&m_aActiveSkill[m_iRookie];

	lpAS->m_ability.set(_iSkill,10);

	lpAS->m_wType		=	eSKILL_TYPE_FIRE_WALL;
	lpAS->m_lpCaster	=	NULL;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_pos.x		=	0;
	lpAS->m_pos.y		=	0;
	lpAS->m_posTarget.x	=	0;
	lpAS->m_posTarget.y	=	0;
	lpAS->m_lpTarget	=	NULL;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	cAS_FireWall	*lpFireWall	=	(cAS_FireWall	*)lpAS;

	lpFireWall->m_wX1	=	0xffff;
	lpFireWall->m_wY1	=	0xffff;
	lpFireWall->m_wX2	=	0xffff;
	lpFireWall->m_wY2	=	0xffff;
	lpFireWall->m_wGrade=	cAS_FireWall::eReady;

	for (int i=m_iRookie + 1;i<dACTIVE_SKILL_COUNT;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴

}

BOOL
CSkillManager::makeSmallFlame(int _iSkill,int _iX1,int _iY1,int _iX2,int _iY2)
{
	cAS_FireWall	*lpSkill	=	getActiveFireWallSkill(_iSkill,_iX1,_iY1,_iX2,_iY2);	//	기존에 설정된 녀석이 있냐?

	if	(!lpSkill)	//	없으면 빈 녀석을 찾아라(이게 정상)
		lpSkill	=	getActiveFireWallSkill(_iSkill,0xffff,0xffff,0xffff,0xffff);

	if	(!lpSkill)	//	빈녀석이 없다!!
	{
		castFireWallSkill(_iSkill);	//	어거지로 스킬 사용

		lpSkill	=	getActiveFireWallSkill(_iSkill,0xffff,0xffff,0xffff,0xffff);
	}

	if	(!lpSkill)	//	그래도 없다. OTL
		return	FALSE;

	return	lpSkill->makeSmallFlame(_iX1,_iY1,_iX2,_iY2);
}

BOOL
CSkillManager::makeFireWall(int _iSkill,int _iX1,int _iY1,int _iX2,int _iY2)
{
	cAS_FireWall	*lpSkill	=	getActiveFireWallSkill(_iSkill,_iX1,_iY1,_iX2,_iY2);	//	기존에 설정된 녀석이 있냐?

	if	(!lpSkill)	//	없으면 빈 녀석을 찾아라(이게 정상)
		lpSkill	=	getActiveFireWallSkill(_iSkill,0xffff,0xffff,0xffff,0xffff);

	if	(!lpSkill)	//	빈녀석이 없다!!
	{
		castFireWallSkill(_iSkill);	//	어거지로 스킬 사용

		lpSkill	=	getActiveFireWallSkill(_iSkill,0xffff,0xffff,0xffff,0xffff);

		if	(lpSkill)
			lpSkill->makeSmallFlame(_iX1,_iY1,_iX2,_iY2);
	}

	if	(!lpSkill)	//	그래도 없다. OTL
		return	FALSE;

	return	lpSkill->makeFireWall(_iX1,_iY1,_iX2,_iY2);
}

BOOL
CSkillManager::removeFireWall(int _iSkill,int _iX1,int _iY1,int _iX2,int _iY2)
{
	cAS_FireWall	*lpSkill	=	getActiveFireWallSkill(_iSkill,_iX1,_iY1,_iX2,_iY2);	//	기존에 설정된 녀석이 있냐?

	if	(!lpSkill)
		return	FALSE;

	lpSkill->removeFireWall();

	remove(lpSkill->m_wSerial);

	return	TRUE;
}

