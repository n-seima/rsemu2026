#include "CHero.H"
#include "CGame.H"
#include "CTitle.H"
#include "CActor.H"

//
//	
CPlayerTitleInfo*
CHero::getTitleByIndex(int _iTitle)
{
	int		i;

	for (i=0;i<dMAX_PLAYER_TITLE_COUNT;i++)
	{
		CPlayerTitleInfo	*lpTitleInfo	=	&m_aTitle[i];

		if	(lpTitleInfo->m_bTitle	==	_iTitle)
			return	lpTitleInfo;
	}

	return	NULL;
}

CPlayerTitleInfo*
CHero::getTitleByTitleEffect(int _iTitleEffect)
{
	int		i,iTitleEffect;

	for (i=0;i<dMAX_PLAYER_TITLE_COUNT;i++)
	{
		CPlayerTitleInfo	*lpTitleInfo	=	&m_aTitle[i];

		CTitle	*lpTitle	=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);

		if	(!lpTitle)
			continue;

		for (iTitleEffect=0;iTitleEffect<dMAX_TITLE_EFFECT_COUNT;iTitleEffect++)
			if (lpTitle->m_aEffect[iTitleEffect].m_wEffect	==	_iTitleEffect)
				return	lpTitleInfo;
	}

	return	NULL;
}

//
//	타이틀 처리
void
CHero::operateTitleData()
{
	int		i;

	m_wTitleDistinguisherValue	=	0;

	for (i=0;i<dMAX_PLAYER_TITLE_COUNT;i++)
	{
		CPlayerTitleInfo	*lpTitleInfo	=	&m_aTitle[i];

		if	(lpTitleInfo->m_bTitle	==	0xff)
			continue;

		operateTitleData(lpTitleInfo);
	}
}

//
//	타이틀 처리
int
CHero::getTitleLevel(int _iTitle)
{
	CPlayerTitleInfo	*lpTitle	=	getTitleByIndex(_iTitle);

	if	(lpTitle	==	NULL)
		return	0;

	return	lpTitle->m_bLevel;
}

//
//	타이틀 처리
void
CHero::operateTitleData(CPlayerTitleInfo *_lpTitleInfo)
{
	int		i;
	CTitle	*lpTitle	=	g_titleManager.getTitle(_lpTitleInfo->m_bTitle);

	if	(!lpTitle						)
		return;
	if	(!lpTitle->isEnableJob(m_wJob)	)
		return;	//	가능한 직업이 아니다.
	
	int		iPlayerLevel=	getLevel();
	
	if	(_lpTitleInfo->m_bTitle	==	eTN_PORTER)
	{
		iPlayerLevel	=	m_iLevel;
		
	}
	
	if	(m_bf4RebirthCount)	//	전생자는 렙제 푼다
		iPlayerLevel	=	max(iPlayerLevel,600);

	int		iTitleLevel	=	lpTitle->getEnableLevel(_lpTitleInfo->m_bLevel,iPlayerLevel);

	if	(iTitleLevel	==	0)
		return;

	for (i=0;i<dMAX_TITLE_EFFECT_COUNT;i++)
	{
		CTitleEffectInfo	*lpTitleEffect	=	&lpTitle->m_aEffect[i];

		switch(lpTitleEffect->m_wEffect)
		{
			case	eTE_DISTINGUISHER		:	//	변별자
				operateTitleDistinguisher(lpTitleEffect,iTitleLevel);
				break;
			
			case	eTE_SKILL_MASTER		:	//	레벨 수치 자체로 계산
				operateTitleSkillMaster(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_PORTER				:	//	칭호레벨*레벨 수치
				operateTitlePorter(lpTitleEffect,iTitleLevel);
				break;
				
			case	eTE_DEALER				:
				operateTitleDealer(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_BANK_CLIENT			:	//	칭호레벨^레벨 수치
				operateTitleBankClient(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_COURTESY			:
				operateTitleCourtesy(lpTitleEffect,iTitleLevel);
				break;
				
			case	eTE_CITY_WALKER			:	//	칭호레벨^레벨 수치
				operateTitleCityWalker(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_CAMPING_MASTER		:	//	칭호레벨^레벨 수치
				operateTitleCampingMaster(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_SURVIVAL_INSTINCTS	:
				operateTitleSurvivalInstincts(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_ALCHEMY				:	//	칭호레벨^레벨 수치
				operateTitleAlchemy(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_ESOTERIC			:	//	칭호레벨^레벨 수치
				operateTitleEsoteric(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_ELEMENTAL_MASTER	:	//	엘레멘탈 마스터
				operateTitleElementalMaster(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_ADVENTURER			:
				operateTitleAdventurer(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_INCLINE_POWER		:
				operateTitleInclinePower(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_INCLINE_MAGIC_RESISTANCE	:
				operateTitleInclineMagicResistance(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_NATURAL_ENEMY				:
				operateTitleNaturalEnemy(lpTitleEffect,iTitleLevel);
				break;

//			case	eTE_BERSERKER			:	//	칭호레벨^레벨 수치
//				operateTitleBerserker(lpTitleEffect,iTitleLevel);
//				break;

//			case	eTE_FRONTIER			:	//	칭호레벨^레벨 수치
//				operateTitleFrontier(lpTitleEffect,iTitleLevel);
//				break;
		}
	}
}	//	CHero::operateTitleData()

void
CHero::operateTitleDistinguisher(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wTitleDistinguisherValue	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

//
//	스킬 마스터 타이틀 처리
void
CHero::operateTitleSkillMaster(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	int	iCorrectLevel=0;

	iCorrectLevel		=	getMasterSkillDifficultySum(m_wJob);
	iCorrectLevel		=	min(_lpTitleEffect->m_aiValue[_iTitleLevel-1][0],iCorrectLevel);

	m_correctAllSkillLevel.addValue(iCorrectLevel);
}

void
CHero::operateTitleDealer(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wPitchmanShopSize	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

//
//	짐꾼
void
CHero::operateTitlePorter(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wItemSlotCount	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

//	시티워커
void
CHero::operateTitleCityWalker(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	if (g_map.m_bf4FieldType	==	eFT_VILLAGE)
		m_moveSpeed.addValue(_lpTitleEffect->m_aiValue[_iTitleLevel-1][0]);
}

void
CHero::operateTitleCampingMaster(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wCampingMasterTitleFactor	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0]+100;
}

void
CHero::operateTitleSurvivalInstincts(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	int	iLimitPercentage					=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
	int	iLimitValue							=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][1];
	int	iCriticalValue						=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][2];

	iLimitValue								=	min(getMaxHP()/100*iLimitPercentage/100,iLimitValue);
	m_wSurvivalInstinctsTitleTriggerHP		=	iLimitValue*100;
	m_wSurvivalInstinctsTitleCriticalChance	=	iCriticalValue;
}

void
CHero::operateTitleAlchemy(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wAlchemyTitleFactor						=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

void
CHero::operateTitleElementalMaster(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wCorrectFieldElementalResistanceByTitle	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

/*
void
CHero::operateTitleBerserker(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wBerserKerTitleBoostPower	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0]+100;
	m_wBerserKerTitleBoostCount	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][1];
	m_wBerserKerTitleBoostTime	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][2];

	if (m_bf5BoostChargeDay		!=	g_currentTime.m_wDay)
		m_bf3RemainBoostCount	=	m_wBerserKerTitleBoostCount;
}
*/
//	약사
void
CHero::operateTitleDruggist(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wDruggistTitleFactor	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

void
CHero::operateTitleEsoteric(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	int	iCorrectValue	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];

	m_strangeStatusResistance.addValue(iCorrectValue);
	m_curseResistance.addValue(iCorrectValue);
	m_declinePowerResistance.addValue(iCorrectValue);
}

//
//	은행고객
void
CHero::operateTitleBankClient(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
}

void
CHero::operateTitleCourtesy(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wCourtesyTitleValue	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

void
CHero::operateTitleAdventurer(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wReduceDeathPeneltyValueByTitle	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

BOOL
CHero::getDiscermentMonsterInfo(int *_lpiLimitLevel,int *_lpiEnableType,int *_lpiLimitRank)
{
	*_lpiLimitLevel	=	0;
	*_lpiEnableType	=	0;
	*_lpiLimitRank	=	0;
	
	CPlayerTitleInfo	*lpTitleInfo	=	getTitleByTitleEffect(eTE_MONSTER_DISCERNER);

	if (!lpTitleInfo)
		return FALSE;

	CTitle	*lpTitle	=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);

	if (!lpTitle)
		return FALSE;

	int	iTitleLevel		=	lpTitleInfo->m_bLevel;
	int	iLevelFactor	=	lpTitle->m_aEffect[0].m_aiValue[iTitleLevel-1][0];
	int	iLimitLevel		=	iLevelFactor*getLevel()/100;
	int	iLimitRank		=	lpTitle->m_aEffect[0].m_aiValue[iTitleLevel-1][1];

	*_lpiLimitLevel		=	iLimitLevel;
	*_lpiLimitRank		=	iLimitRank;

	switch(iTitleLevel)
	{
		case	1	:
		case	2	:
			*_lpiEnableType		+=	1<<eJT_UNDEAD;
			break;
		case	3	:
		case	4	:
			*_lpiEnableType		+=	1<<eJT_UNDEAD;
			*_lpiEnableType		+=	1<<eJT_ANIMAL;
			break;
		case	5	:
		case	6	:
			*_lpiEnableType		+=	1<<eJT_UNDEAD;
			*_lpiEnableType		+=	1<<eJT_ANIMAL;
			*_lpiEnableType		+=	1<<eJT_HOLY_BEAST;
			break;
		case	7	:
		case	8	:
			*_lpiEnableType		+=	1<<eJT_UNDEAD;
			*_lpiEnableType		+=	1<<eJT_ANIMAL;
			*_lpiEnableType		+=	1<<eJT_HOLY_BEAST;
			*_lpiEnableType		+=	1<<eJT_DEVIL;
			break;
		case	9	:
		case	10	:
			*_lpiEnableType		+=	1<<eJT_UNDEAD;
			*_lpiEnableType		+=	1<<eJT_ANIMAL;
			*_lpiEnableType		+=	1<<eJT_HOLY_BEAST;
			*_lpiEnableType		+=	1<<eJT_DEVIL;
			*_lpiEnableType		+=	1<<eJT_HUMAN;
			break;
	}

	return	TRUE;
}

void
CHero::discermentMonster(CActor *_lpActor,int *_lpiLevel,int *_lpiType,int *_lpiRank)
{
	*_lpiLevel	=	-1;
	*_lpiType	=	-1;
	*_lpiRank	=	-1;
	
	if (!_lpActor->isMonster())
		return;

	CPlayerTitleInfo	*lpTitleInfo	=	getTitleByTitleEffect(eTE_MONSTER_DISCERNER);

	if (!lpTitleInfo)
		return;

	CTitle	*lpTitle	=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);

	if (!lpTitle)
		return;

	int	iTitleLevel		=	lpTitleInfo->m_bLevel;
	int	iLevelFactor	=	lpTitle->m_aEffect[0].m_aiValue[iTitleLevel-1][0];
	int	iLimitLevel		=	iLevelFactor*getLevel()/100;
	int	iLimitRank		=	lpTitle->m_aEffect[0].m_aiValue[iTitleLevel-1][1];

	if (iLimitLevel	>=	_lpActor->m_wLevel)
		*_lpiLevel		=	_lpActor->m_wLevel;

	CJobInfo	*lpJob	=	g_jm.getJob(_lpActor->m_wTrueJob);

	if (!lpJob)
		return;

	if (lpJob->m_wRank	<=	iLimitRank)
		*_lpiRank	=	lpJob->m_wRank;

	switch(iTitleLevel)
	{
		case	1	:
		case	2	:
			if (lpJob->m_wKind	==	eJT_UNDEAD)
				*_lpiType	=	lpJob->m_wKind;
			break;
		case	3	:
		case	4	:
			if (lpJob->m_wKind	==	eJT_UNDEAD || lpJob->m_wKind	==	eJT_ANIMAL)
				*_lpiType	=	lpJob->m_wKind;
			break;
		case	5	:
		case	6	:
			if (lpJob->m_wKind	==	eJT_UNDEAD || lpJob->m_wKind	==	eJT_ANIMAL || lpJob->m_wKind	==	eJT_HOLY_BEAST)
				*_lpiType	=	lpJob->m_wKind;
			break;
		case	7	:
		case	8	:
			if (lpJob->m_wKind	==	eJT_UNDEAD || lpJob->m_wKind	==	eJT_ANIMAL || lpJob->m_wKind	==	eJT_HOLY_BEAST || lpJob->m_wKind	==	eJT_DEVIL)
				*_lpiType	=	lpJob->m_wKind;
			break;
		case	9	:
		case	10	:
			*_lpiType	=	lpJob->m_wKind;
			break;
	}
}

void
CHero::operateTitleInclinePower(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	int	iStrength	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
	int	iAgility	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][1];
	int	iIntelligence=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][2];
	int	iWisdom		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][3];
	int	iCharisma	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][4];
	int	iLuck		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][5];
	int	iConstitution=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][6];
	int	iHP			=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][7];
	int	iCP			=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][8];

	m_strength.addValue(iStrength);
	m_agility.addValue(iAgility);						//	민첩성
	m_constitution.addValue(iConstitution);					//	건강
	m_intelligence.addValue(iIntelligence);					//	지식
	m_wisdom.addValue(iWisdom);						//	지혜
	m_charisma.addValue(iCharisma);						//	카리스마
	m_luck.addValue(iLuck);							//	운

	m_maxHP.m_iValue+=	iHP*100;
	m_maxCP.m_iValue+=	iCP*100;
}

void
CHero::operateTitleInclineMagicResistance(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	int	iFire		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
	int	iWater		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][1];
	int	iWind		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][2];
	int	iEarth		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][3];
	int	iLight		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][4];
	int	iDark		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][5];

	m_fireResistance.addValue(iFire);
	m_waterResistance.addValue(iWater);
	m_windResistance.addValue(iWind);
	m_earthResistance.addValue(iEarth);
	m_lightResistance.addValue(iLight);
	m_darkResistance.addValue(iDark);
}

void
CHero::operateTitleNaturalEnemy(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
}

