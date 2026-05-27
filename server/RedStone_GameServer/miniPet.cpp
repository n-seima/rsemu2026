#include "SFC.h"
#include "miniPet.h"
#include "cSKILL.h"
#include "cACTOR.h"

int	g_aiExpTableForMiniPet[102]	=	
{
	0,	10,	30,	60,	100,	150,	210,	280,	360,	450,	550,	660,	780,	910,	1050,	1200,	1360,	1530,	1710,	1900,	2100,
	2310,	2530,	2760,	3000,	3250,	3510,	3780,	4060,	4350,	4650,	4960,	5280,	5610,	5950,	6300,	6660,	7030,	7410,	7800,	8200,	8610,
	9030,	9460,	9900,	10350,	10810,	11280,	11760,	12250,	12750,	13260,	13780,	14310,	14850,	15400,	15960,	16530,	17110,	17700,	18300,	18910,	19530,
	20160,	20800,	21450,	22110,	22780,	23460,	24150,	24850,	25560,	26280,	27010,	27750,	28500,	29260,	30030,	30810,	31600,	32400,	33210,	34030,	34860,
	35700,	36550,	37410,	38280,	39160,	40050,	40950,	41860,	42780,	43710,	44650,	45600,	46560,	47530,	48510,	49500,	50500
};

cMiniPet::cMiniPet()
{
	reset();
}

void
cMiniPet::reset()
{
	memset(this,0,sizeof(cMiniPet));
}



int
cMiniPet::getNeedExp(int _iMinLevel, int _iMaxLevel)
{
	int iMin	=	min(_iMinLevel, _iMaxLevel);
	int iMax	=	max(_iMinLevel, _iMaxLevel);
	int	iExp	=	0;

	for(int i=iMin; i <= iMax && i<100; ++i)
	{
		iExp += g_aiExpTableForMiniPet[i];	 
	}

	return iExp;
}


void
cMiniPet::usePolisher()
{
	CTimeInfo	currentTime,expireTime;

	currentTime.update();

	expireTime.m_wYear	=	m_bf5ExpireYear+2000;
	expireTime.m_wMonth	=	m_bf4ExpireMonth	;
	expireTime.m_wDay	=	m_bf5ExpireDay		;
	expireTime.m_wHour	=	m_bf5ExpireHour		;
	expireTime.m_wMinute=	m_bf6ExpireMinute	;

	if	(currentTime.getMinuteValue(2000)	<	expireTime.getMinuteValue(2000))
	{
		currentTime.m_wYear	=	m_bf5ExpireYear	+2000;
		currentTime.m_wMonth=	m_bf4ExpireMonth	;
		currentTime.m_wDay	=	m_bf5ExpireDay		;
		currentTime.m_wHour	=	m_bf5ExpireHour		;
		currentTime.m_wMinute=	m_bf6ExpireMinute	;
	}
	
	currentTime.increaseDay(30);

	m_bf5ExpireYear		=	currentTime.m_wYear-2000;
	m_bf4ExpireMonth	=	currentTime.m_wMonth;
	m_bf5ExpireDay		=	currentTime.m_wDay;
	m_bf5ExpireHour		=	currentTime.m_wHour;
	m_bf6ExpireMinute	=	currentTime.m_wMinute;
}

void
cMiniPet::unseal(cItem *_lpItem)
{
	cBlockLightPocket	*lpPocket	=	(cBlockLightPocket	*)_lpItem;

	m_bf7Level		=	lpPocket->m_bf7Level	;
	m_bf5Type		=	lpPocket->m_bf5Type		;
	m_bf16Exp		=	lpPocket->m_bf16Exp		;
	m_bf7SkillPoint	=	lpPocket->m_bf7SkillPoint;
	m_iEnergyPoint	=	lpPocket->m_iEnergyPoint;
	m_iMineralPoint	=	lpPocket->m_iMineralPoint;
	m_iVitalPoint	=	lpPocket->m_iVitalPoint	;

	m_bf4ExpireMonth=	lpPocket->m_bf4ExpireMonth;
	m_bf5ExpireYear	=	lpPocket->m_bf5ExpireYear	;
	m_bf5ExpireDay	=	lpPocket->m_bf5ExpireDay	;
	m_bf5ExpireHour	=	lpPocket->m_bf5ExpireHour	;
	m_bf6ExpireMinute=	lpPocket->m_bf6ExpireMinute;

	int	i,iSkillCount=0;

	for (i=0;i<dMAX_SKILL && iSkillCount < c_iMiniPetSkillCount;i++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkillCount];
		CSkill				*lpSkill	=	&g_aSkill[i];

		if	(lpSkill->m_wSerial	==	0xffff)
			continue;

		if	(lpSkill->m_bf5MiniPetType	!=	m_bf5Type	&&	lpSkill->m_bf5MiniPetType	!=	(m_bf5Type	%	6))
			continue;

		lpAbility->m_bf11Skill	=	i;
		lpAbility->m_bf4Level	=	0;
		lpAbility->m_bf1IsOFF	=	FALSE;
		if	(lpSkill->m_bf1IsDefaultMiniPetSkill)
			lpAbility->m_bf4Level	=	1;

		iSkillCount++;
	}
	
	m_aAbility[0].m_bf4Level	=	lpPocket->m_bf3SkillLevel1;
	m_aAbility[1].m_bf4Level	=	lpPocket->m_bf3SkillLevel2;
	m_aAbility[2].m_bf4Level	=	lpPocket->m_bf3SkillLevel3;
	m_aAbility[3].m_bf4Level	=	lpPocket->m_bf3SkillLevel4;
	m_aAbility[4].m_bf4Level	=	lpPocket->m_bf3SkillLevel5;
	m_aAbility[5].m_bf4Level	=	lpPocket->m_bf3SkillLevel6;
	m_aAbility[6].m_bf4Level	=	lpPocket->m_bf3SkillLevel7;
	m_aAbility[7].m_bf4Level	=	lpPocket->m_bf3SkillLevel8;
	m_aAbility[8].m_bf4Level	=	lpPocket->m_bf3SkillLevel9;
	m_aAbility[9].m_bf4Level	=	lpPocket->m_bf3SkillLevel10;

	m_bf1MinipetAwaken50		=	lpPocket->m_bf1isUseAwakenItem50;
	m_bf1MinipetAwaken100		=	lpPocket->m_bf1isUseAwakenItem100;

	int	iSkillPoint	=	m_bf7SkillPoint;

	m_bf7SkillPoint	=	m_bf7Level-1;

	for (i=0;i<iSkillCount;i++)
	{
		if	(m_aAbility[i].m_bf11Skill	==	0)
			continue;

		for (int j=0;j<m_aAbility[i].m_bf4Level;j++)
		{
			if	(j	==	0)
			{
				CSkill	*lpSkill	=	&g_aSkill[m_aAbility[i].m_bf11Skill];

				if	(lpSkill->m_bf1IsDefaultMiniPetSkill)
					continue;
			}

			if	(m_bf7SkillPoint>=	(UINT)(j+1))
				m_bf7SkillPoint	-=	(j+1);
			else
				m_bf7SkillPoint	=	m_bf7SkillPoint;
		}
	}

	addSpareSkill();

	strcpy(m_strName,"noname");

	usePolisher();
}



void
cMiniPet::evolution(BOOL _bIsReset)
{
	if	(m_iVitalPoint	>	m_iEnergyPoint	&&	m_iVitalPoint	>	m_iMineralPoint)
		m_bf5Type	+=	c_iMiniPetEvolutionToVital;
	else
	if	(m_iEnergyPoint	>	m_iVitalPoint	&&	m_iEnergyPoint	>	m_iMineralPoint)
		m_bf5Type	+=	c_iMiniPetEvolutionToEnergy;
	else
	if	(m_iMineralPoint>	m_iVitalPoint	&&	m_iMineralPoint	>	m_iEnergyPoint)
		m_bf5Type	+=	c_iMiniPetEvolutionToMineral;
	else
		return;

	int	i,iSkillCount		=	0;
	int iOldType	=	m_bf5Type - m_bf5Type/6 * 6;
	for (i=0;i<c_iMiniPetSkillCount;i++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[i];

		if(_bIsReset)
		{
			CSkill				*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];
			if	(lpSkill && (int)lpSkill->m_bf5MiniPetType	!=	iOldType)
			{
				lpAbility->m_bf11Skill	= 0;
				lpAbility->m_bf4Level	= 0;
				lpAbility->m_bf1IsOFF	= FALSE;
				continue;
			}
		}
		if	(lpAbility->m_bf11Skill)
			iSkillCount++;
	}

	for (i=0;i<dMAX_SKILL && iSkillCount < c_iMiniPetSkillCount;i++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkillCount];
		CSkill				*lpSkill	=	&g_aSkill[i];
		

		if	(lpSkill->m_wSerial	==	0xffff)
			continue;

		if	(lpSkill->m_bf5MiniPetType	!=	m_bf5Type)
			continue;
		
		lpAbility->m_bf11Skill	=	i;
		lpAbility->m_bf4Level	=	0;
		lpAbility->m_bf1IsOFF	=	FALSE;
		iSkillCount++;
	}
}


BOOL
cMiniPet::resetSkill()
{
	int iSkillPoint = 0;
	for (int i=0;i<c_iMiniPetSkillCount;i++)
	{
		if(m_aAbility[i].m_bf11Skill  == 0)
			continue;
		if(m_aAbility[i].m_bf4Level == 0)
			continue;
		
		CSkill*	lpSkill	=	&g_aSkill[m_aAbility[i].m_bf11Skill];
		
		if(!lpSkill)
			continue;
		if(m_aAbility[i].m_bf4Level)
		{
			int iMax = m_aAbility[i].m_bf4Level;
			
			m_aAbility[i].m_bf4Level = 0;
			
			if(lpSkill->m_bf1IsDefaultMiniPetSkill)
			{
				m_aAbility[i].m_bf4Level = 1;
			}
		}

		m_aAbility[i].m_bf1IsOFF	=	FALSE;
	}
	iSkillPoint =	m_bf7Level - 1;
	if(iSkillPoint == 0)
		return FALSE;
	m_aAbility[c_iMiniPetSkillCount-1].m_bf11Skill	=	0;
	m_aAbility[c_iMiniPetSkillCount-1].m_bf4Level	=	0;	
	m_aAbility[c_iMiniPetSkillCount-1].m_bf1IsOFF	=	FALSE;	
	m_bf7SkillPoint =	iSkillPoint;
		
	return TRUE;
}


void
cMiniPet::create(int _iType)
{
	reset();

	m_bf7Level	=	1;
	m_bf5Type	=	_iType;

	CTimeInfo	currentTime;

	currentTime.update();
	currentTime.increaseDay(30);
	currentTime.m_wYear	-=	2000;

	m_bf5ExpireYear		=	currentTime.m_wYear;
	m_bf4ExpireMonth	=	currentTime.m_wMonth;
	m_bf5ExpireDay		=	currentTime.m_wDay;
	m_bf5ExpireHour		=	currentTime.m_wHour;
	m_bf6ExpireMinute	=	currentTime.m_wMinute;

	strcpy(m_strName,"noname");

	int	iSkillCount		=	0;

	for (int i=0;i<dMAX_SKILL && iSkillCount < c_iMiniPetSkillCount;i++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkillCount];
		CSkill				*lpSkill	=	&g_aSkill[i];

		if	(lpSkill->m_wSerial	==	0xffff)
			continue;

		if	(lpSkill->m_bf5MiniPetType	!=	(UINT)_iType)
			continue;

		lpAbility->m_bf11Skill	=	i;
		lpAbility->m_bf4Level	=	0;
		lpAbility->m_bf1IsOFF	=	FALSE;
		if	(lpSkill->m_bf1IsDefaultMiniPetSkill)
			lpAbility->m_bf4Level	=	1;

		iSkillCount++;
	}
}


BOOL
cMiniPet::increaseExp(int _iExp,int _iType,int _iLordLevel)
{
	int		iCurrrentExp	=	m_bf16Exp;

	iCurrrentExp	+=	_iExp;

	switch(_iType)
	{
		case	nsMiniPetStyle::Bio		:
			if	(m_iVitalPoint+_iExp	<	200000000)
				m_iVitalPoint	+=	_iExp;
			break;
		case	nsMiniPetStyle::Energy	:
			if	(m_iEnergyPoint+_iExp	<	200000000)
				m_iEnergyPoint	+=	_iExp;
			break;
		case	nsMiniPetStyle::Mineral	:
			if	(m_iMineralPoint+_iExp	<	200000000)
				m_iMineralPoint	+=	_iExp;
			break;
	}

	while(m_bf7Level	<	100)
	{
		if	(iCurrrentExp	<	g_aiExpTableForMiniPet[m_bf7Level])
			break;

		if	(m_bf7Level	>=	(UINT)_iLordLevel)
			break;

		iCurrrentExp	-=	g_aiExpTableForMiniPet[m_bf7Level];
		m_bf7SkillPoint++;

		m_bf7Level++;
	}

	if	(m_bf7Level	>=	c_iMiniPetEvolutionLevel	&&	m_bf5Type	<=	nsMiniPetType::SpritOfDark)
		evolution();

	m_bf16Exp	=	min(iCurrrentExp,0xffff);

	return	TRUE;
}

cMiniPetAbilityInfo*
cMiniPet::getPowerUpSkill(int _iSkill)
{
	if	(m_bf7Level	==	0)
		return	NULL;

	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkill];

		if	(lpAbility->m_bf4Level	==	0)
			continue;

		if	(lpAbility->m_bf1IsOFF)
			continue;

		CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

		if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::MiniPetSkillPowerUp)
			continue;

		if	(lpSkill->m_aValue[0]	==	_iSkill)
			return	lpAbility;
	}

	return	NULL;
}


cMiniPetAbilityInfo*
cMiniPet::getSkillByType(int _iType)
{
	if	(m_bf7Level	==	0)
		return	NULL;

	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkill];

		if	(lpAbility->m_bf4Level	==	0)
			continue;

		if	(lpAbility->m_bf1IsOFF)
			continue;

		CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

		if	(lpSkill->m_bf5MiniPetSkillType	!=	(UINT)_iType)
			continue;

		return	lpAbility;
	}

	return	NULL;
}


int
cMiniPet::getVSBossCorrectActiveChance(cACTOR *_lpActor)
{
	//if	(!_lpActor)
	if	(!_lpActor || !_lpActor->isBossMonster())
		return	0;

	if	(m_bf7Level	==	0)
		return	0;

	int	iMonsterType	=	_lpActor->getJob()->m_wKind;

	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkill];

		if	(lpAbility->m_bf4Level	==	0)
			continue;

		if	(lpAbility->m_bf1IsOFF)
			continue;

		CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

		if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::IncreaseActiveChanceUnderBossBattle)
			continue;

		int	iLevel	=	lpAbility->m_bf4Level;

		if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			iLevel		+=	_lpActor->m_bIncreaseMiniPetBasicSkillLevel;
		iLevel		+=	_lpActor->m_bIncreaseMiniPetSkillLevel; 

		int	iCorrectChance	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*iLevel);

 		if(m_bf1MinipetAwaken100)
 			iCorrectChance += lpSkill->m_wAwakenSkillBonusDamage + lpSkill->m_wAwakenSkillBonusDamagePerLevel * m_bf7Level * 2;
 		else if(m_bf1MinipetAwaken50)
 			iCorrectChance += lpSkill->m_wAwakenSkillBonusDamage + lpSkill->m_wAwakenSkillBonusDamagePerLevel * m_bf7Level;

		if	(iMonsterType	==	lpSkill->m_aValue[2]	||	lpSkill->m_aValue[2]	==	eJT_ALL)
			return	iCorrectChance;

		break;
	}

	return	0;
}


int
cMiniPet::getVSBossCorrectTargetMagicResistance(cACTOR *_lpActor)
{
	if	(!_lpActor->isBossMonster())
		return 0;

	if	(m_bf7Level	==	0)
		return 0;

	int	iMonsterType	=	_lpActor->getJob()->m_wKind;

	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkill];

		if	(lpAbility->m_bf4Level	==	0)
			continue;
		if	(lpAbility->m_bf1IsOFF)
			continue;
		CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

		if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::IgnoreTargetMagicResistanceUnderBossBattle)
			continue;

		int	iLevel	=	lpAbility->m_bf4Level;

		if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			iLevel		+=	_lpActor->m_bIncreaseMiniPetBasicSkillLevel;
		iLevel		+=	_lpActor->m_bIncreaseMiniPetSkillLevel;

		int	iCorrectMagicResistance	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*iLevel)/100;

		//JBC 미니펫 4차 강화 내용. 플레임 마스터 따위들...09-04-07
		if(m_bf1MinipetAwaken100)
			iCorrectMagicResistance += (lpSkill->m_wAwakenSkillBonusDamage + m_bf7Level * lpSkill->m_wAwakenSkillBonusDamagePerLevel * 2)/100;
		else if(m_bf1MinipetAwaken50)
			iCorrectMagicResistance += (lpSkill->m_wAwakenSkillBonusDamage + m_bf7Level * lpSkill->m_wAwakenSkillBonusDamagePerLevel)/100;

		if	(iMonsterType	==	lpSkill->m_aValue[2]	||	lpSkill->m_aValue[2]	==	eJT_ALL)
			return	-iCorrectMagicResistance;
	}

	return	0;
}



int
cMiniPet::getDecreaseChargeCount(cACTOR *_lpActor,BOOL _bIsMinipet)
{
	if	(m_bf7Level	==	0)
		return	0;

	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkill];

		if	(lpAbility->m_bf4Level	==	0)
			continue;

		if	(lpAbility->m_bf1IsOFF)
			continue;

		CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

		if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::DecreaseChargeCount)
			continue;

		int	iLevel	=	lpAbility->m_bf4Level;
		
		if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			iLevel		+=	_lpActor->m_bIncreaseMiniPetBasicSkillLevel;
		iLevel		+=	_lpActor->m_bIncreaseMiniPetSkillLevel;
		iLevel		+=	_lpActor->m_abCorrectMiniPetSkillLevel[_bIsMinipet];

		return	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*iLevel)/100;
	}

	return	0;
}


void
cMiniPet::operateVSBossCorrectMagicPower(cACTOR *_lpActor,int *_lpiFireDamage,int *_lpiWaterDamage,int *_lpiWindDamage,int *_lpiEarthDamage,int *_lpiLightDamage,int *_lpiDarkDamage)
{
	if	(!_lpActor->isBossMonster())
		return;

	if	(m_bf7Level	==	0)
		return;

	int	iMonsterType	=	_lpActor->getJob()->m_wKind;

	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkill];

		if	(lpAbility->m_bf4Level	==	0)
			continue;

		if	(lpAbility->m_bf1IsOFF)
			continue;

		CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

		if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::PowerUpUnderBattleToBoss)
			continue;

		int	iLevel	=	lpAbility->m_bf4Level;

		if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			iLevel		+=	_lpActor->m_bIncreaseMiniPetBasicSkillLevel;
		iLevel		+=	_lpActor->m_bIncreaseMiniPetSkillLevel;

		int	iCorrectDamage	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*iLevel)/100;

		if	(iMonsterType	==	lpSkill->m_aValue[2]	||	lpSkill->m_aValue[2]	==	eJT_ALL)
		{
			*_lpiFireDamage		+=	*_lpiFireDamage*iCorrectDamage/100;
			*_lpiWaterDamage	+=	*_lpiWaterDamage*iCorrectDamage/100;
			*_lpiWindDamage		+=	*_lpiWindDamage*iCorrectDamage/100;
			*_lpiEarthDamage	+=	*_lpiEarthDamage*iCorrectDamage/100;
			*_lpiLightDamage	+=	*_lpiLightDamage*iCorrectDamage/100;
			*_lpiDarkDamage		+=	*_lpiDarkDamage*iCorrectDamage/100;

			return;
		}
	}
}

//	잔여 스킬과 레벨을 확인해 스킬 추가

int
cMiniPet::addSpareSkill()
{
	if	(m_bf7Level			<	100)
		return	-1;	
	if	(m_bf7SkillPoint	==	0)
		return	-1;

	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkill];

		if	(lpAbility->m_bf11Skill	!=	0	&&lpAbility->m_bf4Level	<	5	&&	lpAbility->m_bf4Level	<	m_bf7SkillPoint)
		{
			if	(isLevelUpAbleAbility(lpAbility))
				return	-1;
		}
	}

	int	iType	=	m_bf5Type%6;

	for (int i=0;i<dMAX_SKILL;i++)
	{
		CSkill				*lpSkill	=	&g_aSkill[i];

		if	(lpSkill->m_wSerial	==	0xffff)
			continue;

		if	(lpSkill->m_bf5MiniPetType	!=	c_iMiniPetSpareSkillType)
			continue;

		if	(lpSkill->m_aValue[0]	!=	(short)m_bf7SkillPoint)
			continue;

		if	(lpSkill->m_aValue[1]	!=	iType	&&	lpSkill->m_aValue[2]	!=	iType	&&	lpSkill->m_aValue[3]	!=	iType)
			continue;

		m_aAbility[c_iMiniPetSkillCount-1].m_bf11Skill	=	i;
		m_aAbility[c_iMiniPetSkillCount-1].m_bf4Level	=	1;
		m_aAbility[c_iMiniPetSkillCount-1].m_bf1IsOFF	=	FALSE;
		m_bf7SkillPoint	=	0;

		return	i;
	}

	return	-1;
}



void
cMiniPet::setTest(int _iStyle)
{
	m_bf5Type	=	m_bf5Type%6;

	create(m_bf5Type);

	switch(_iStyle)
	{
		case	nsMiniPetStyle::Deault	:
			m_bf7Level		=	29;
			m_bf7SkillPoint	=	28;
			break;
		case	nsMiniPetStyle::Mineral	:
			m_iMineralPoint	=	1000;
			m_bf7Level		=	100;
			m_bf7SkillPoint	=	99;
			evolution();
			break;
		case	nsMiniPetStyle::Energy	:
			m_iEnergyPoint	=	1000;
			m_bf7Level		=	100;
			m_bf7SkillPoint	=	99;
			evolution();
			break;
		case	nsMiniPetStyle::Bio		:
			m_iVitalPoint	=	1000;
			m_bf7Level		=	100;
			m_bf7SkillPoint	=	99;
			evolution();
			break;
	}

}

cMiniPetAbilityInfo*
cMiniPet::getAbilityBySkill(int _iSkill)
{
	for (int i=0;i<c_iMiniPetSkillCount;i++)
	{
		if	(m_aAbility[i].m_bf11Skill	==	_iSkill)
			return	&m_aAbility[i];
	}

	return	NULL;
}


BOOL
cMiniPet::isLevelUpAbleAbility(cMiniPetAbilityInfo	*_lpAbility)
{
	if	(!_lpAbility	||	_lpAbility->m_bf11Skill	==	0	||	_lpAbility->m_bf4Level	>=	c_iMiniPetSkillMaxLevel)
		return	FALSE;

	cAbility	ability;

	ability.set(_lpAbility->m_bf11Skill,_lpAbility->m_bf4Level);
	
	CSkill	*lpSkill	=	ability.getSkill();

	if	(!lpSkill	)
		return	FALSE;	//	이건 문제인데.. --
	if	(m_bf7SkillPoint	<	(UINT)_lpAbility->m_bf4Level+1)
		return	FALSE;

	for (int i=0;i<dMAX_REQUIRE_SKILL_COUNT;i++)
	{
		if	(lpSkill->m_aRequireSkill[i].m_wSkill	==	0xffff)
			break;
		
		int	iSkill	=	lpSkill->m_aRequireSkill[i].m_wSkill;

		_lpAbility	=	getAbilityBySkill(iSkill);

		if	(!_lpAbility)
			return	FALSE;
		if	(_lpAbility->m_bf4Level	<	lpSkill->m_aRequireSkill[i].m_wLevel)
			return	FALSE;
	}

	return	TRUE;
}

BOOL
cMiniPet::isExpired()//	시간이 다 된 미니펫
{
	CTimeInfo	currentTime,expireTime;

	currentTime.update();

	expireTime.m_wYear	=	m_bf5ExpireYear+2000;
	expireTime.m_wMonth	=	m_bf4ExpireMonth	;
	expireTime.m_wDay	=	m_bf5ExpireDay		;
	expireTime.m_wHour	=	m_bf5ExpireHour		;
	expireTime.m_wMinute=	m_bf6ExpireMinute	;

	if	(currentTime.getMinuteValue(2000)	>	expireTime.getMinuteValue(2000))
		return	TRUE;

	return	FALSE;
}

BOOL
cMiniPet::renewExpireTime(int _iMinute)//	시간이 다 된 미니펫
{
	if	(m_bf7Level	==	0)
		return	FALSE;

	CTimeInfo	currentTime,expireTime;

	currentTime.update();

	expireTime.m_wYear	=	m_bf5ExpireYear+2000;
	expireTime.m_wMonth	=	m_bf4ExpireMonth	;
	expireTime.m_wDay	=	m_bf5ExpireDay		;
	expireTime.m_wHour	=	m_bf5ExpireHour		;
	expireTime.m_wMinute=	m_bf6ExpireMinute	;

	expireTime.increaseMinute(_iMinute);

	if	(currentTime.getMinuteValue(2000)	>	expireTime.getMinuteValue(2000))
		return	FALSE;

	m_bf5ExpireYear		=	expireTime.m_wYear-2000;
	m_bf4ExpireMonth	=	expireTime.m_wMonth;
	m_bf5ExpireDay		=	expireTime.m_wDay;
	m_bf5ExpireHour		=	expireTime.m_wHour;
	m_bf6ExpireMinute	=	expireTime.m_wMinute;

	return	TRUE;
}

int
cMiniPet::getEvolutionRate(int _iStyle)//	진화율
{
	LONGLONG	llValue	=	m_iVitalPoint+m_iEnergyPoint+m_iMineralPoint;
	
	if	(llValue	==	0)
		return	0;
	
	if	(_iStyle	==	nsMiniPetStyle::Bio)
		return 	(int)(m_iVitalPoint*100.0/llValue);
	if	(_iStyle	==	nsMiniPetStyle::Energy)
		return 	(int)(m_iEnergyPoint*100.0/llValue);
	if	(_iStyle	==	nsMiniPetStyle::Mineral)
		return	(int)(m_iMineralPoint*100.0/llValue);
	
	
	return	0;
}

BOOL
cMiniPet::setAwaken(BOOL _is100persent)
{
	return TRUE;
}
