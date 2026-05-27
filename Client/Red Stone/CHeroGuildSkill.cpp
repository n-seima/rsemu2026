#include "CHero.H"
#include "CGuild.H"
#include "CGuildSkill.H"

//
//	타이틀 처리
void
CHero::operateGuildSkillData()
{
	if	(m_wGuildSerial == 0xffff)
		return;

	KEY_1;

	switch(m_wGuildRank) 
	{
		case	dGUILD_CLASS_ROOKIE		:	//	신입회원
			m_wRankInGuildForSkill	=	3;
			break;

		case	dGUILD_CLASS_MEMBER		:	//	정회원
		case	dGUILD_CLASS_ELDER		:	//	원로 
		case	dGUILD_CLASS_PARLIAMENT	:	//	의원
			m_wRankInGuildForSkill	=	2;
			break;

		case	dGUILD_CLASS_SUBMASTER	:	//	부 마스터
			m_wRankInGuildForSkill	=	1;
			break;

		case	dGUILD_CLASS_MASTER		:	//	마스터
			m_wRankInGuildForSkill	=	0;
			break;

		default							:
			return;
	}

	m_wGuildHallLevel	=	g_guild.m_wHallLevel;

	for (int i=0;i<c_iNormalGuildSkillCount;i++)
	{
		if	(g_guild.m_abGuildSkill[i]	==	0)
			continue;

		int	iLevel	=	g_guild.getApplySkillLevel(i);

		if	(iLevel)
			operateGuildSkillData(i,iLevel);
	}

	for	(int iStatueSkill=0;iStatueSkill<c_iGuildStatueCount;iStatueSkill++)
	{
		int	iSkillIndex	=	c_iGuildStatueSkillBeginIndex+iStatueSkill*c_iGuildStatuePartsCount;
		int	iPartsCount	=	0;

		for (i=iSkillIndex;i<iSkillIndex+c_iGuildStatuePartsCount;i++)
		{
			if	(g_guild.m_abGuildSkill[i])
				iPartsCount++;
		}

		int	iEffectLimitCount	=	dMAX_GUILD_SKILL_EFFECT_COUNT;

		if	(iPartsCount	<	c_iGuildStatueSetEffectRequireCount)
			iEffectLimitCount	=	1;

		for (i=iSkillIndex;i<iSkillIndex+c_iGuildStatuePartsCount;i++)
		{
			if	(g_guild.m_abGuildSkill[i]	==	0)
				continue;

			int	iLevel	=	g_guild.getApplySkillLevel(i);

			if	(iLevel)
				operateGuildSkillData(i,iLevel,FALSE,iEffectLimitCount);
		}
	}
}

//
//	길드 스킬 데이터 처리
int
CHero::operateGuildSkillData(int _iSkill,int _iLevel,int _bIsGetValue,int _iEffectLimitCount)
{
	int			i;
	CGuildSkill	*lpSkill	=	&g_gsManager.m_aSkills[_iSkill];
	int			iReturnValue;

	if(m_wTransformationTime)
		return 0;

	for (i=0;i<_iEffectLimitCount;i++)
	{
		CGuildSkillEffectInfo	*lpEffect	=	&lpSkill->m_aEffect[i];

		if	(lpEffect->m_wEffect	==	0xffff)
			continue;

		switch(lpEffect->m_wEffect)
		{
			case	eGSE_INCREASE_HP						:	//체력증가	
				iReturnValue	=	operateGSIncreaseHP(_iSkill,_iLevel,_bIsGetValue);
				break;
			case	eGSE_INCREASE_CP						:	//CP증가					
				iReturnValue	=	operateGSIncreaseCP(_iSkill,_iLevel,_bIsGetValue);
				break;
			case	eGSE_INCREASE_HIT_CHANCE				:	//정확도증가				
				iReturnValue	=	operateGSIncreaseHitChance(_iSkill,_iLevel,_bIsGetValue);
				break;
			case	eGSE_INCREASE_DODGE_CHANCE				:	//회피율증가				
				iReturnValue	=	operateGSIncreaseDodgeChance(_iSkill,_iLevel,_bIsGetValue);
				break;
			case	eGSE_INCREASE_STRENGTH					:	//힘증가					
				iReturnValue	=	operateGSIncreaseStrength(_iSkill,_iLevel,_bIsGetValue);
				break;
			case	eGSE_INCREASE_AGILITY					:	//민첩성증가				
				iReturnValue	=	operateGSIncreaseAgility(_iSkill,_iLevel,_bIsGetValue);
				break;
			case	eGSE_INCREASE_CONSTITUTION				:	//건강증가				
				iReturnValue	=	operateGSIncreaseConstitution(_iSkill,_iLevel,_bIsGetValue);
				break;
			case	eGSE_INCREASE_INTELLIGENCE				:	//지식증가				
				iReturnValue	=	operateGSIncreaseIntelligence(_iSkill,_iLevel,_bIsGetValue);
				break;
			case	eGSE_INCREASE_WISDOM					:	//지혜증가				
				iReturnValue	=	operateGSIncreaseWisdom(_iSkill,_iLevel,_bIsGetValue);
				break;
			case	eGSE_INCREASE_CHARISMA					:	//카리스마증가			
				iReturnValue	=	operateGSIncreaseCharisma(_iSkill,_iLevel,_bIsGetValue);
				break;
			case	eGSE_INCREASE_LUCK						:	//운증가					
				iReturnValue	=	operateGSIncreaseLuck(_iSkill,_iLevel,_bIsGetValue);
				break;

			case	eGSE_INCREASE_KNIGHT_SKILL_LEVEL		:	//기사스킬레벨증가		
			case	eGSE_INCREASE_WARRIOR_SKILL_LEVEL		:	//전사스킬레벨증가		
			case	eGSE_INCREASE_WIZARD_SKILL_LEVEL		:	//마법사스킬레벨증가		
			case	eGSE_INCREASE_WEREWOLF_SKILL_LEVEL		:	//늑대인간스킬레벨증가	
			case	eGSE_INCREASE_PRIEST_SKILL_LEVEL		:	//사제스킬레벨증가		
			case	eGSE_INCREASE_FALLEN_ANGEL_SKILL_LEVEL	:	//타락천사스킬레벨증가	
			case	eGSE_INCREASE_ROGUE_SKILL_LEVEL			:	//도둑스킬레벨증가		
			case	eGSE_INCREASE_FIGHTER_SKILL_LEVEL		:	//무도가스킬레벨증가		
			case	eGSE_INCREASE_MAGIC_ARCHER_SKILL_LEVEL	:	//마법궁수스킬레벨증가	
			case	eGSE_INCREASE_MAGIC_LANCER_SKILL_LEVEL	:	//마법창병스킬레벨증가	
			case	eGSE_INCREASE_TAMER_SKILL_LEVEL			:	//조련사스킬레벨증가		
			case	eGSE_INCREASE_SUMMONER_SKILL_LEVEL		:	//소환사스킬레벨증가		
			case	eGSE_INCREASE_PRINCESS_SKILL_LEVEL		:	//공주스킬레벨증가		
			case	eGSE_INCREASE_MAGIC_GIRL_SKILL_LEVEL	:	//변신소녀스킬레벨증가	
			case	eGSE_INCREASE_NECROMANCER_SKILL_LEVEL	:	//강신술사스킬레벨증가	
			case	eGSE_INCREASE_DEVIL_SKILL_LEVEL			:	//악마스킬레벨증가		
				iReturnValue	=	operateGSIncreaseJobSkillLevel(_iSkill,_iLevel,lpEffect->m_wEffect-eGSE_INCREASE_KNIGHT_SKILL_LEVEL,_bIsGetValue);
				break;
			case	eGSE_INCREASE_SOUL_BRINGER_SKILL_LEVEL			:	//영술사스킬레벨증가
			case	eGSE_INCREASE_CHAMPION_SKILL_LEVEL				:	// 투사 스킬레벨증가					
			case	eGSE_INCREASE_OPTICALIST_SKILL_LEVEL			:	// 광학사 스킬레벨증가	
				iReturnValue	=	operateGSIncreaseJobSkillLevel(_iSkill,_iLevel,dJOB_SOUL_BRINGER + lpEffect->m_wEffect - eGSE_INCREASE_SOUL_BRINGER_SKILL_LEVEL,_bIsGetValue);
				break;

			case	eGSE_ABSORB_FIRE_PER_LEVEL	:	//{	"불흡수per레벨"			:	
			case	eGSE_ABSORB_WATER_PER_LEVEL	:	//{	"물흡수per레벨"			:	
			case	eGSE_ABSORB_WIND_PER_LEVEL	:	//{	"바람흡수per레벨"		:	
			case	eGSE_ABSORB_EARTH_PER_LEVEL	:	//{	"대지흡수per레벨"		:	
			case	eGSE_ABSORB_LIGHT_PER_LEVEL	:	//{	"빛흡수per레벨"			:	
			case	eGSE_ABSORB_DARK_PER_LEVEL	:	//{	"어둠흡수per레벨"		:	
				iReturnValue	=	operateGSAbsorbMagicDamagePerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_RESISTANCE_OF_FIRE_PER_LEVEL	:	//{	"불저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_WATER_PER_LEVEL	:	//{	"물저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_WIND_PER_LEVEL	:	//{	"바람저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_EARTH_PER_LEVEL	:	//{	"대지저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_LIGHT_PER_LEVEL	:	//{	"빛저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_DARK_PER_LEVEL	:	//{	"어둠저항력증가per레벨"		:	
				iReturnValue	=	operateGSIncreaseMagicResistancePerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_CRITICAL_CRUSH_RESISTANCE_PER_LEVEL	:	//{	"치명타결정타저항증가per레벨"			:	
				iReturnValue	=	operateGSIncreaseCrushResistancePerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_CURSE_RESISTANCE_PER_LEVEL	:	//{	"저주계저항증가per레벨"					:	
				iReturnValue	=	operateGSIncreaseCurseResistancePerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_STRANGE_STATUS_RESISTANCE_PER_LEVEL	:	//{	"이상계저항증가per레벨"					:	
				iReturnValue	=	operateGSIncreaseStrangeStatusResistancePerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_DECLINE_POWER_RESISTANCE_PER_LEVEL	:	//{	"저하계저항증가per레벨"					:	
				iReturnValue	=	operateGSIncreaseDeclinePowerResitancePerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_PHYSICAL_MAGIC_DAMAGE_PER_LEVEL		:	//{	"물리공격력증가per레벨"					:	
				iReturnValue	=	operateGSIncreasePhysicalMagicDamagePerLevel(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_DEFENSIVE_POWER_PER_LEVEL	:	//{	"방어력증가퍼센티지per레벨"				:	
				iReturnValue	=	operateGSIncreaseDefensivePowerPerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_ATTACK_SPEED_PER_LEVEL	:	//{	"공격속도증가per레벨"					:	
				iReturnValue	=	operateGSIncreaseActionSpeedPerLevel(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_MOVE_SPEED_PER_LEVEL	:	//{	"이동속도증가per레벨"					:	
				iReturnValue	=	operateGSIncreaseMoveSpeedPerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_HIT_CHANCE_PER_LEVEL	:	//{	"명중률증가per레벨"						:	
				iReturnValue	=	operateGSIncreaseHitChancePerLevel(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_DODGE_CHANCE_PER_LEVEL	:	//{	"회피율증가per레벨"						:	
				iReturnValue	=	operateGSIncreaseDodgeChancePerLevel(_iSkill,_iLevel,lpEffect);
				break;

		}

		if	(iReturnValue	!=	0xffff)
			return	iReturnValue;
	}

	return	0;
}



int
CHero::operateGSIncreaseHP(int _iSkill,int _iLevel,int _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iIncreaseHP	=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];

	if (_bIsGetValue)
		return	iIncreaseHP;

	m_maxHP.m_sCorrectValueByPrefix	+=	iIncreaseHP;

	return	0xffff;
}

int
CHero::operateGSIncreaseCP(int _iSkill,int _iLevel,int _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iIncreaseCP	=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];

	if (_bIsGetValue)
		return	iIncreaseCP;

	m_maxCP.m_sCorrectValueByPrefix	+=	iIncreaseCP;

	return	0xffff;
}	//	CHero::operateGSIncreaseCP(int _iSkill,int _iLevel,int _bIsGetValue)

int
CHero::operateGSIncreaseHitChance(int _iSkill,int _iLevel,int _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iHitChance	=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill]/100;

	if (_bIsGetValue)
		return	iHitChance;

	m_hitChance.m_sValue	+=	iHitChance;

	return	0xffff;
}	//	CHero::operateGSIncreaseHitChance(int _iSkill,int _iLevel,int _bIsGetValue)

int
CHero::operateGSIncreaseDodgeChance(int _iSkill,int _iLevel,int _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iDodgeChance=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill]/100;

	if (_bIsGetValue)
		return	iDodgeChance;

	m_dodgeChance.m_sValue	+=	iDodgeChance;

	return	0xffff;
}	//	CHero::operateGSIncreaseDodgeChance(int _iSkill,int _iLevel,int _bIsGetValue)

int
CHero::operateGSIncreaseStrength(int _iSkill,int _iLevel,int _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	if (_bIsGetValue)
		return	iValue;

	m_strength.m_sValue		+=	iValue;

	return	0xffff;
}	//	CHero::operateGSIncreaseStrength(int _iSkill,int _iLevel,int _bIsGetValue)

int
CHero::operateGSIncreaseAgility(int _iSkill,int _iLevel,int _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	if (_bIsGetValue)
		return	iValue;

	m_agility.m_sValue		+=	iValue;

	return	0xffff;
}	//	CHero::operateGSIncreaseAgility(int _iSkill,int _iLevel,int _bIsGetValue)

int
CHero::operateGSIncreaseConstitution(int _iSkill,int _iLevel,int _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	if (_bIsGetValue)
		return	iValue;

	m_constitution.m_sValue	+=	iValue;

	return	0xffff;
}	//	CHero::operateGSIncreaseConstitution(int _iSkill,int _iLevel,int _bIsGetValue)

int
CHero::operateGSIncreaseIntelligence(int _iSkill,int _iLevel,int _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	if (_bIsGetValue)
		return	iValue;

	m_intelligence.m_sValue	+=	iValue;

	return	0xffff;
}	//	CHero::operateGSIncreaseIntelligence(int _iSkill,int _iLevel,int _bIsGetValue)

int
CHero::operateGSIncreaseWisdom(int _iSkill,int _iLevel,int _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	if (_bIsGetValue)
		return	iValue;

	m_wisdom.m_sValue		+=	iValue;

	return	0xffff;
}	//	CHero::operateGSIncreaseWisdom(int _iSkill,int _iLevel,int _bIsGetValue)

int
CHero::operateGSIncreaseCharisma(int _iSkill,int _iLevel,int _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	if	(_bIsGetValue)
		return	iValue;

	m_charisma.m_sValue		+=	iValue;

	return	0xffff;
}	//	CHero::operateGSIncreaseCharisma(int _iSkill,int _iLevel,int _bIsGetValue)

int
CHero::operateGSIncreaseLuck(int _iSkill,int _iLevel,int _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	if	(_bIsGetValue)
		return	iValue;

	m_luck.m_sValue			+=	iValue;

	return	0xffff;
}	//	CHero::operateGSIncreaseLuck(int _iSkill,int _iLevel)

int
CHero::operateGSIncreaseJobSkillLevel(int _iSkill,int _iLevel,int _iJob,int _bIsGetValue)
{
	if	(m_wJob	!=	_iJob && _bIsGetValue == FALSE)	
		return 0xffff;

	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*100/lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];

	if	(iValue%50	>=	50)
		iValue	=	iValue/100+1;
	else
		iValue	/=	100;

	if	(_bIsGetValue)
		return	iValue;

	m_correctAllSkillLevel.addValue(iValue);

	return	0xffff;
}


int
CHero::operateGSAbsorbMagicDamagePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	switch(_lpEffect->m_wEffect)
	{
		case	eGSE_ABSORB_FIRE_PER_LEVEL	:	//{	"불흡수per레벨"			:
			addAbsorbFireDamage(iValue);
			break;
		case	eGSE_ABSORB_WATER_PER_LEVEL	:	//{	"물흡수per레벨"			:	
			addAbsorbWaterDamage(iValue);
			break;
		case	eGSE_ABSORB_WIND_PER_LEVEL	:	//{	"바람흡수per레벨"		:	
			addAbsorbWindDamage(iValue);
			break;
		case	eGSE_ABSORB_EARTH_PER_LEVEL	:	//{	"대지흡수per레벨"		:	
			addAbsorbEarthDamage(iValue);
			break;
		case	eGSE_ABSORB_LIGHT_PER_LEVEL	:	//{	"빛흡수per레벨"			:	
			addAbsorbLightDamage(iValue);
			break;
		case	eGSE_ABSORB_DARK_PER_LEVEL	:	//{	"어둠흡수per레벨"		:	
			addAbsorbDarkDamage(iValue);
			break;
	}

	return	0xffff;
}

int
CHero::operateGSIncreaseMagicResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;
	
	if	(_bIsGetValue)
		return	iValue;

	switch(_lpEffect->m_wEffect)
	{
		case	eGSE_INCREASE_RESISTANCE_OF_FIRE_PER_LEVEL	:	//{	"불저항력증가per레벨"		:	
			m_fireResistance.addValue(iValue);
			break;
		case	eGSE_INCREASE_RESISTANCE_OF_WATER_PER_LEVEL	:	//{	"물저항력증가per레벨"		:	
			m_waterResistance.addValue(iValue);
			break;
		case	eGSE_INCREASE_RESISTANCE_OF_WIND_PER_LEVEL	:	//{	"바람저항력증가per레벨"		:	
			m_windResistance.addValue(iValue);
			break;
		case	eGSE_INCREASE_RESISTANCE_OF_EARTH_PER_LEVEL	:	//{	"대지저항력증가per레벨"		:	
			m_earthResistance.addValue(iValue);
			break;
		case	eGSE_INCREASE_RESISTANCE_OF_LIGHT_PER_LEVEL	:	//{	"빛저항력증가per레벨"		:	
			m_lightResistance.addValue(iValue);
			break;
		case	eGSE_INCREASE_RESISTANCE_OF_DARK_PER_LEVEL	:	//{	"어둠저항력증가per레벨"		:	
			m_darkResistance.addValue(iValue);
			break;
	}

	return	0xffff;
}

int
CHero::operateGSIncreaseCrushResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	addCrushBlowResistance(iValue);

	return	0xfffff;
}

int
CHero::operateGSIncreaseCurseResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	addCurseResistance(iValue);

	return	0xfffff;
}

int
CHero::operateGSIncreaseStrangeStatusResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	addStrangeStatusResistance(iValue);

	return	0xfffff;
}

int
CHero::operateGSIncreaseDeclinePowerResitancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	addDeclinePowerResistance(iValue);

	return	0xfffff;
}

int
CHero::operateGSIncreasePhysicalMagicDamagePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_attackPower.m_sCorrectPercentageValueByPrefix		+=	iValue;

	m_strengtheningFireDamage.addValue(iValue,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
	m_strengtheningWaterDamage.addValue(iValue,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
	m_strengtheningWindDamage.addValue(iValue,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
	m_strengtheningEarthDamage.addValue(iValue,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
	m_strengtheningLightDamage.addValue(iValue,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
	m_strengtheningDarkDamage.addValue(iValue,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);

	return	0xfffff;
}

int
CHero::operateGSIncreaseDefensivePowerPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_defensePower.m_sCorrectPercentageValueByPrefix	+=	iValue;

	return	0xfffff;
}

int
CHero::operateGSIncreaseActionSpeedPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_actionSpeed.addValue(iValue);

	return	0xfffff;
}

int
CHero::operateGSIncreaseMoveSpeedPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_moveSpeed.addValue(iValue);

	return	0xfffff;
}

int
CHero::operateGSIncreaseHitChancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_hitChance.addValue(iValue);

	return	0xfffff;
}

int
CHero::operateGSIncreaseDodgeChancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_dodgeChance.addValue(iValue);

	return	0xfffff;
}
