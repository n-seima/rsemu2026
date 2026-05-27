#include "cACTOR.H"
#include "CGame.H"
#include "CTitle.H"
#include "cEvent.H"

void
cACTOR::checkRedStonePoint()
{
	cRedStonePointRank	*lpManager		=	g_eventManager.getRedStoneRankManager();
	int					iLevel			=	lpManager->getTitleLevel(m_strName);

	CPlayerTitleInfo	*lpTitleInfo	=	getTitle(eTN_RED_STONE_AGENT);

	if	(iLevel)
	{
		if	(!lpTitleInfo	||	lpTitleInfo->m_bLevel	!=	iLevel)
			addTitle(eTN_RED_STONE_AGENT,iLevel);
	}
	else
	{
		if	(lpTitleInfo)
			removeTitle(eTN_RED_STONE_AGENT);
	}
}

//
//	타이틀 추가
BOOL
cACTOR::addTitle(int _iTitle,int _iTitleLevel,BOOL _bIsSendMessage)
{
	int	i;

	CTitle	*lpTitle	=	g_titleManager.getTitle(_iTitle);

	if	(!lpTitle)
		return	NULL;

	for (i=0;i<dMAX_PLAYER_TITLE_COUNT;i++)
	{
		CPlayerTitleInfo	*lpTitleInfo	=	&m_aTitle[i];

		if (lpTitleInfo->m_bTitle	==	_iTitle)
		{
			if	(_iTitleLevel	>	lpTitleInfo->m_bLevel)
				if	(_bIsSendMessage)
					sendGetTitleMessage(i,lpTitleInfo->m_bTitle,_iTitleLevel);

			if	(_iTitleLevel	==	0)
				_iTitleLevel	=	lpTitleInfo->m_bLevel+1;

			if	(_iTitleLevel	>	dMAX_TITLE_LEVEL)
				return	FALSE;

			lpTitleInfo->m_bLevel		=	_iTitleLevel;
			
			buildPower();

			return	TRUE;
		}
	}

	for (i=0;i<dMAX_PLAYER_TITLE_COUNT;i++)
	{
		CPlayerTitleInfo	*lpTitleInfo	=	&m_aTitle[i];

		if	(lpTitleInfo->m_bTitle	==	0xff)
		{
			lpTitleInfo->m_bTitle		=	_iTitle;

			if	(_iTitleLevel)
				lpTitleInfo->m_bLevel	=	_iTitleLevel;
			else
				lpTitleInfo->m_bLevel	=	1;

			if	(_bIsSendMessage)
				sendGetTitleMessage(i,lpTitleInfo->m_bTitle,lpTitleInfo->m_bLevel);

			buildPower();

			return	TRUE;
		}
	}

	return	FALSE;
}

//
//	타이틀 제거
BOOL
cACTOR::removeTitle(int _iTitle,BOOL _bIsSendMessage,BOOL _bIsByCH5Reward)
{
	int	i;

	for (i=0;i<dMAX_PLAYER_TITLE_COUNT;i++)
	{
		CPlayerTitleInfo	*lpTitleInfo	=	&m_aTitle[i];

		if	(lpTitleInfo->m_bTitle	==	_iTitle)
		{
			CTitle	*lpTitle		=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);

			if	(!lpTitle)
				return	FALSE;

			if	(_bIsSendMessage)
				sendRemoveTitleMessage(i,lpTitleInfo->m_bTitle,lpTitleInfo->m_bLevel);

			if	(_bIsByCH5Reward)
			{
				lpTitleInfo->m_bLevel		=	lpTitleInfo->m_bLevel+0x80;
			}
			else
			{
				lpTitleInfo->m_bTitle		=	0xff;
				lpTitleInfo->m_bLevel		=	0;
			}
			return	TRUE;
		}
	}

	return	FALSE;
}

//
//	타이틀 얻기
CPlayerTitleInfo*
cACTOR::getTitle(int _iTitle)
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
cACTOR::getTitleByTitleEffect(int _iTitleEffect)
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
//	이거 가지고 있는 타이틀 이냐?
BOOL
cACTOR::isOwnedTitle(int _iTitle)
{
	if	(getTitle(_iTitle))
		return	TRUE;

	return	FALSE;
}

//
//	타이틀 처리
void
cACTOR::operateTitleData()
{
	int		i;
	
	m_wSurvivalInstinctsTitleTriggerHP	=	0;
	m_wCourtesyTitleValue				=	0;
	m_wPitchmanShopSize					=	dPITCHMAN_SHOP_MINIMUM_SIZE;
	m_wPartyExpBonus					=	0;
	m_wCorrectFieldElementalResistanceByTitle	=	0;
	m_wReduceDeathPeneltyValueByTitle	=	0;

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
void
cACTOR::operateTitleData(CPlayerTitleInfo *_lpTitleInfo)
{
	int		i;
	CTitle	*lpTitle	=	g_titleManager.getTitle(_lpTitleInfo->m_bTitle);

	if	(!lpTitle						)
		return;
	if	(!lpTitle->isEnableJob(m_wJob)	)
		return;
	
	int		iPlayerLevel=	getLevel();

	if	(_lpTitleInfo->m_bTitle	==	eTN_PORTER)
	{
		iPlayerLevel	=	m_iLevel;
	}
	if	(m_bf4RebirthCount)	//	전생자는 렙제 푼다
		iPlayerLevel	=	max(iPlayerLevel,600);

	int	iTitleLevel		=	lpTitle->getEnableLevel(_lpTitleInfo->m_bLevel,iPlayerLevel);

	if	(iTitleLevel	==	0)
		return;

	for (i=0;i<dMAX_TITLE_EFFECT_COUNT;i++)
	{
		CTitleEffectInfo	*lpTitleEffect	=	&lpTitle->m_aEffect[i];

		if	(lpTitleEffect->m_wEffect		==	0xffff)
			continue;

		switch(lpTitleEffect->m_wEffect)
		{
			case	eTE_HUNTING_BONUS		:
				m_bIsApplyHuntingExpBonus		=	TRUE;
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

			case	eTE_COURTESY			:
				operateTitleCourtesy(lpTitleEffect,iTitleLevel);
				break;
				
			case	eTE_DRUGGIST			:
				operateTitleDruggist(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_BERSERKER			:	//	칭호레벨^레벨 수치
				operateTitleBerserker(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_BARD				:	
				break;

			case	eTE_FRONTIER			:	//	칭호레벨^레벨 수치
				operateTitleFrontier(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_ELEMENTAL_MASTER	:	//	엘레멘탈 마스터
				operateTitleElementalMaster(lpTitleEffect,iTitleLevel);
				break;

			case	eTE_ADVENTURER			:	//	모험가
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
		}
	}
}

//
//	스킬 마스터 타이틀 처리
void
cACTOR::operateTitleSkillMaster(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	int	iAbility,iCorrectValue=0;

	for (iAbility=0;iAbility<dABILITY_COUNT;iAbility++)
	{
		cAbility	*lpAbility	=	&m_aAbility[iAbility];

		if	(!lpAbility->isEnableJob(m_wJob))
			continue;

		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(lpAbility->m_wLevel	<	dLIMIT_SKILL_LEVEL)
			continue;

		iCorrectValue	+=	lpSkill->m_wDifficultyLevel;
	}

	iCorrectValue	=	min(_lpTitleEffect->m_aiValue[_iTitleLevel-1][0],iCorrectValue);

	m_correctAllSkillLevel.addValue(iCorrectValue);
}

//
//	짐꾼
void
cACTOR::operateTitlePorter(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	if	(_iTitleLevel	>	10)
		return;

	m_wItemSlotCount	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

//
//	은행고객
void
cACTOR::operateTitleBankClient(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wBankSize			=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
	m_wBankCharge		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][1];
}


int
cACTOR::getTitleBankClientTax(BOOL _bRealLevel)		// 은행칭호 수수료값 리턴~~~ 이힝~09.08.19
{

	CPlayerTitleInfo* lpTitleInfo = NULL;
	
	for (int i=0;i<dMAX_PLAYER_TITLE_COUNT;++i)
	{
		int iTitle	=	m_aTitle[i].m_bTitle;

		if	(iTitle	==	0xff)
			continue;

		if	(iTitle == eTN_BANK_CLIENT )
		{
			lpTitleInfo = &m_aTitle[i];
			break;
		}
	}
	

	if(!lpTitleInfo)
		return dTITLE_NOT_BANK_GOLD_BAR_TAX;

	CTitle	*lpTitle	=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);

	if(!lpTitle)
		return dTITLE_NOT_BANK_GOLD_BAR_TAX;
	
	CTitleEffectInfo	*lpTitleEffect	=	&lpTitle->m_aEffect[0];

	int iPlayerLevel = getLevel();
	if	(m_bf4RebirthCount)	//	전생자는 렙제 푼다
		iPlayerLevel	=	max(iPlayerLevel,600);

	int	iTitleLevel		=	lpTitle->getEnableLevel(lpTitleInfo->m_bLevel,iPlayerLevel);
	int iTax = lpTitleEffect->m_aiValue[iTitleLevel-1][2];

	if(_bRealLevel)	
		iTax = lpTitleEffect->m_aiValue[lpTitleInfo->m_bLevel-1][2];

	return iTax;

}
//	시티워커
void
cACTOR::operateTitleCityWalker(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	if (m_lpField->isVillage())
		m_moveSpeed.addValue(_lpTitleEffect->m_aiValue[_iTitleLevel-1][0]);
}

void
cACTOR::operateTitleCampingMaster(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wCampingMasterTitleFactor	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0]+100;
}

void
cACTOR::operateTitleSurvivalInstincts(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	int	iLimitPercentage					=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
	int	iLimitValue							=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][1];
	int	iCriticalValue						=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][2];

	m_wSurvivalInstinctsTitleTriggerPropertion=	iLimitPercentage;
	m_wSurvivalInstinctsTitleTriggerHP		=	iLimitValue;	//	*100을 해야함
	m_wSurvivalInstinctsTitleCriticalChance	=	iCriticalValue;
}

//	약사
void
cACTOR::operateTitleDruggist(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wDruggistTitleFactor	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

void
cACTOR::operateTitleAlchemy(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wAlchemyTitleFactor	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

void
cACTOR::operateTitleBerserker(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wBerserKerTitleBoostPower	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0]+100;
	m_wBerserKerTitleBoostCount	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][1];
	m_wBerserKerTitleBoostTime	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][2];
}

//	밀교
void
cACTOR::operateTitleEsoteric(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	int	iCorrectValue	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];

	m_strangeStatusResistance.addValue(iCorrectValue);
	m_curseResistance.addValue(iCorrectValue);
	m_declinePowerResistance.addValue(iCorrectValue);
}

void
cACTOR::operateTitleCourtesy(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wCourtesyTitleValue	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

void
cACTOR::operateTitleDealer(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wPitchmanShopSize	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

//
//	프론티어
void
cACTOR::operateTitleFrontier(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wPartyExpBonus	=	max(m_wPartyExpBonus,_lpTitleEffect->m_aiValue[_iTitleLevel-1][0]);
}

void
cACTOR::operateTitleElementalMaster(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wCorrectFieldElementalResistanceByTitle	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

void
cACTOR::operateTitleAdventurer(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	m_wReduceDeathPeneltyValueByTitle	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
}

void
cACTOR::operateTitleInclinePower(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
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
cACTOR::operateTitleInclineMagicResistance(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
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
cACTOR::operateTitleNaturalEnemy(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel)
{
	int	iHuman		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][0];
	int	iUndead		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][1];
	int	iDevil		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][2];
	int	iAnimal		=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][3];
	int	iHolyBeast	=	_lpTitleEffect->m_aiValue[_iTitleLevel-1][4];

	m_wCorrectHitChanceToHumanByTitle		+=	iHuman;
	m_wCorrectHitChanceToUndeadByTitle		+=	iUndead;
	m_wCorrectHitChanceToDevilByTitle		+=	iDevil;
	m_wCorrectHitChanceToAnimalByTitle		+=	iAnimal;
	m_wCorrectHitChanceToHolyBeastByTitle	+=	iHolyBeast;
}


void
cACTOR::useTitle(int _iTitle)
{
	switch(_iTitle)
	{
		case	eTN_BARD	:
		{
			if	(m_bf5BardTitleCoolTime)
			{
				sendEtcWork(eEW_UPDATE_BARD_TITLE_COOL_TIME,m_bf5BardTitleCoolTime,1);
				return;
			}

			if	(m_wPremiumServiceLevel	==	0 && m_bf1IsExistExceptExpPortal	==	FALSE)
			{
				sendEtcWork(eEW_REQUIRE_PORTAL_ITEM_TO_USE_THAT_TITLE,m_bf5BardTitleCoolTime,1);
				return;
			}

			if	(isSitdown()	==	FALSE)
				return;

			cParty	*lpParty	=	getParty();

			if	(lpParty)
			{
				SG_ETC_WORK	packet;

				packet.base.set(sizeof(packet),dSG_ETC_WORK);
				packet.wWork	=	eEW_USE_BARD_TITLE;

				CTitle	*lpTitle	=	g_titleManager.getTitle(eTN_BARD);

				lpParty->sendPacketToSameFieldMember(this,&packet,packet.base.wSize,lpTitle->m_aEffect[0].m_aiValue[0][1]);
			}
			else
				sendEtcWork(eEW_USE_BARD_TITLE);

			m_wRemainToUseBardTitleTime	=	10;

			break;
		}

		case	eTN_BERSERK	:
		{
			if	(m_bf3UseBoostCount	>=	m_wBerserKerTitleBoostCount)
			{
				sendEtcWork(eEW_BERSERK_COUNT_OVER,m_wBerserKerTitleBoostCount);

				return;
			}

			if	(m_wBoostTime)
			{
				sendEtcWork(eEW_ALREADY_BERSERK_STATUS,m_wBoostTime);

				return;
			}

			m_bf3UseBoostCount++;
			m_wBoostTime				=	m_wBerserKerTitleBoostTime*60;
			m_wBoostPower				=	m_wBerserKerTitleBoostPower;

			sendEtcWork(eEW_USE_BERSERK_TITLE,m_bf3UseBoostCount,m_wBoostTime,m_wBoostPower);
			addSendSimpleInfo();

			buildPower();
			break;
		}
	}
}

int
cACTOR::getEnableTitle(int _iTitleEffect,CTitle	*_lpTitle)
{
	CPlayerTitleInfo	*lpTitleInfo	=	getTitleByTitleEffect(_iTitleEffect);

	if	(!lpTitleInfo)
		return	0;

	_lpTitle	=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);

	if	(!_lpTitle->isEnableJob(m_wJob)	)
		return	0;
	int iPlayerLevel = getLevel();
	if	(m_bf4RebirthCount)	//	전생자는 렙제 푼다
		iPlayerLevel	=	max(iPlayerLevel,600);
	int	iTitleLevel		=	_lpTitle->getEnableLevel(lpTitleInfo->m_bLevel,getLevel());

	return	iTitleLevel;
}
int
cACTOR::getTrapEvadeCahnceByTitle()
{
	CTitle	title;

	int		iTitleLevel	=	getEnableTitle(eTE_TRAP_EVADE,&title);

	if	(iTitleLevel	==	0)
		return	0;

	return	title.m_aEffect[0].m_aiValue[iTitleLevel-1][0];
}

int
cACTOR::getCorrectUnlockLevelByTitle()
{
	CTitle	title;

	int		iTitleLevel	=	getEnableTitle(eTE_LOCK_MASTER,&title);

	if	(iTitleLevel	==	0)
		return	0;

	return	title.m_aEffect[0].m_aiValue[iTitleLevel-1][0];
}

int
cACTOR::getBonusAttackDoorPowerByTitle()
{
	CTitle	title;

	int		iTitleLevel	=	getEnableTitle(eTE_TREASURE_HUNTER,&title);

	if	(iTitleLevel	==	0)
		return	0;

	return	title.m_aEffect[0].m_aiValue[iTitleLevel-1][0];
}

void
cACTOR::increaseTitleLevelByCH5(WORD *_lpInfo)
{
	int	iItem	=	_lpInfo[0];
	int	iCount	=	_lpInfo[1];

	CPlayerTitleInfo	*lpTitleInfo	=	getTitle(eTN_RED_STONE_HUNTER);

	if	(!lpTitleInfo)
		return;

	int	iIncreaseCount	=	lpTitleInfo->m_bLevel/3;

	if	(lpTitleInfo->m_bLevel%3)
		iIncreaseCount++;

	if	(iCount	!=	iIncreaseCount)
		return;

	cItem	*lpItem	=	getInventoryItem(iItem);

	if	(!lpItem	||	lpItem->isOwnSpecificEffectItem(eIE_CH5_SELECT_UPGRADE_TITLE)	==	FALSE)
		return;

	int	i;

	CPlayerTitleInfo	aTitle[dMAX_PLAYER_TITLE_COUNT];

	memcpy(aTitle,m_aTitle,sizeof(aTitle));

	for (i=0;i<iCount;i++)
	{
		int	iTitle		=	_lpInfo[2+i];

		lpTitleInfo		=	getTitle(iTitle);
		int	iIndex		=	lpTitleInfo-m_aTitle;

		if	(!lpTitleInfo)
			return;

		CTitle	*lpTitle	=	g_titleManager.getTitle(iTitle);

		if	(!lpTitle	||	lpTitle->m_bf1ExceptCH5Reward)
			return;

		aTitle[iIndex].m_bLevel++;

		if	(aTitle[iIndex].m_bLevel	>	10)
			return;
		
		if	(lpTitleInfo->m_bTitle == eTN_DEALER && aTitle[iIndex].m_bLevel > 5)
			return;
	}

	for (i=0;i<iCount;i++)
	{
		int	iTitle		=	_lpInfo[2+i];
		lpTitleInfo		=	getTitle(iTitle);
		int	iIndex		=	lpTitleInfo-m_aTitle;

		lpTitleInfo->m_bLevel++;

		sendGetTitleMessage(iIndex,lpTitleInfo->m_bTitle,lpTitleInfo->m_bLevel);
	}

	lpItem->reset();

	sendRemoveItem(iItem,1,TRUE);

	buildPower();
}

