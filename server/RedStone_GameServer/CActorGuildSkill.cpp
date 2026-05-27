#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "CGame.H"
#include "CDataSave.H"
#include "CGuild.h"
#include "CGuildSkill.h"

//
//	타이틀 처리
void
cACTOR::operateGuildSkillData()
{
	if	(m_wGuildSerial == 0xffff)
		return;

	CGuild	*lpGuild	=	g_guildManager.getBySerial(m_wGuildSerial);
	int				i;

	if	(!lpGuild)
		return;

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

	m_wGuildHallLevel	=	lpGuild->m_wHallLevel;

	for (i=0;i<c_iNormalGuildSkillCount;i++)
	{
		if	(lpGuild->m_abGuildSkill[i]	==	0)
			continue;

		int	iLevel	=	lpGuild->getApplySkillLevel(i);

		if	(iLevel)	
			operateGuildSkillData(i,iLevel);
	}

	for	(int iStatueSkill=0;iStatueSkill<c_iGuildStatueCount;iStatueSkill++)
	{
		int	iSkillIndex	=	c_iGuildStatueSkillBeginIndex+iStatueSkill*c_iGuildStatuePartsCount;
		int	iPartsCount	=	0;

		for (i=iSkillIndex;i<iSkillIndex+c_iGuildStatuePartsCount;i++)
		{
			if	(lpGuild->m_abGuildSkill[i])
				iPartsCount++;
		}

		int	iEffectLimitCount	=	dMAX_GUILD_SKILL_EFFECT_COUNT;

		if	(iPartsCount	<	c_iGuildStatueSetEffectRequireCount)
			iEffectLimitCount	=	1;

		for (i=iSkillIndex;i<iSkillIndex+c_iGuildStatuePartsCount;i++)
		{
			if	(lpGuild->m_abGuildSkill[i]	==	0)
				continue;

			int	iLevel	=	lpGuild->getApplySkillLevel(i);

			if	(iLevel)
				operateGuildSkillData(i,iLevel,iEffectLimitCount);
		}
	}
}

//
//	길드 스킬 데이터 처리
void
cACTOR::operateGuildSkillData(int _iSkill,int _iLevel,int _iLimitEffectCount)
{
	int			i;
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);

	if	(_iLevel	>	lpSkill->m_wTopLevel)
		return;

	if(m_wTransformationTime)
		return;

	for (i=0;i<_iLimitEffectCount;i++)
	{
		CGuildSkillEffectInfo	*lpEffect	=	&lpSkill->m_aEffect[i];

		if	(lpEffect->m_wEffect	==	0xffff)
			continue;

		switch(lpEffect->m_wEffect)
		{
			case	eGSE_INCREASE_HP						:	//체력증가	
				operateGSIncreaseHP(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_CP						:	//CP증가					
				operateGSIncreaseCP(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_HIT_CHANCE				:	//정확도증가				
				operateGSIncreaseHitChance(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_DODGE_CHANCE				:	//회피율증가				
				operateGSIncreaseDodgeChance(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_STRENGTH					:	//힘증가					
				operateGSIncreaseStrength(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_AGILITY					:	//민첩성증가				
				operateGSIncreaseAgility(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_CONSTITUTION				:	//건강증가				
				operateGSIncreaseConstitution(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_INTELLIGENCE				:	//지식증가				
				operateGSIncreaseIntelligence(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_WISDOM					:	//지혜증가				
				operateGSIncreaseWisdom(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_CHARISMA					:	//카리스마증가			
				operateGSIncreaseCharisma(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_LUCK						:	//운증가					
				operateGSIncreaseLuck(_iSkill,_iLevel);
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
			case	eGSE_INCREASE_MAGIC_GIRL_SKILL_LEVEL	:	//마법소녀스킬레벨증가	
			case	eGSE_INCREASE_NECROMANCER_SKILL_LEVEL	:	//강신술사스킬레벨증가	
			case	eGSE_INCREASE_DEVIL_SKILL_LEVEL			:	//악마스킬레벨증가		
				operateGSIncreaseJobSkillLevel(_iSkill,_iLevel,lpEffect->m_wEffect-eGSE_INCREASE_KNIGHT_SKILL_LEVEL);
				break;
			case	eGSE_INCREASE_SOUL_BRINGER_SKILL_LEVEL			:	//영술사스킬레벨증가	
			case	eGSE_INCREASE_CHAMPION_SKILL_LEVEL				:// 투사스킬레벨증가	
			case	eGSE_INCREASE_OPTICALIST_SKILL_LEVEL			:// 광학사스킬레벨증가		
				operateGSIncreaseJobSkillLevel(_iSkill,_iLevel,dJOB_SOUL_BRINGER + lpEffect->m_wEffect- eGSE_INCREASE_SOUL_BRINGER_SKILL_LEVEL);
				break;
			case	eGSE_ABSORB_FIRE_PER_LEVEL	:	//{	"불흡수per레벨"			:	
			case	eGSE_ABSORB_WATER_PER_LEVEL	:	//{	"물흡수per레벨"			:	
			case	eGSE_ABSORB_WIND_PER_LEVEL	:	//{	"바람흡수per레벨"		:	
			case	eGSE_ABSORB_EARTH_PER_LEVEL	:	//{	"대지흡수per레벨"		:	
			case	eGSE_ABSORB_LIGHT_PER_LEVEL	:	//{	"빛흡수per레벨"			:	
			case	eGSE_ABSORB_DARK_PER_LEVEL	:	//{	"어둠흡수per레벨"		:	
				operateGSAbsorbMagicDamagePerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_RESISTANCE_OF_FIRE_PER_LEVEL	:	//{	"불저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_WATER_PER_LEVEL	:	//{	"물저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_WIND_PER_LEVEL	:	//{	"바람저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_EARTH_PER_LEVEL	:	//{	"대지저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_LIGHT_PER_LEVEL	:	//{	"빛저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_DARK_PER_LEVEL	:	//{	"어둠저항력증가per레벨"		:	
				operateGSIncreaseMagicResistancePerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_CRITICAL_CRUSH_RESISTANCE_PER_LEVEL	:	//{	"치명타결정타저항증가per레벨"			:	
				operateGSIncreaseCrushResistancePerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_CURSE_RESISTANCE_PER_LEVEL	:	//{	"저주계저항증가per레벨"					:	
				operateGSIncreaseCurseResistancePerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_STRANGE_STATUS_RESISTANCE_PER_LEVEL	:	//{	"이상계저항증가per레벨"					:	
				operateGSIncreaseStrangeStatusResistancePerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_DECLINE_POWER_RESISTANCE_PER_LEVEL	:	//{	"저하계저항증가per레벨"					:	
				operateGSIncreaseDeclinePowerResitancePerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_PHYSICAL_MAGIC_DAMAGE_PER_LEVEL		:	//{	"물리공격력증가per레벨"					:	
				operateGSIncreasePhysicalMagicDamagePerLevel(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_DEFENSIVE_POWER_PER_LEVEL	:	//{	"방어력증가퍼센티지per레벨"				:	
				operateGSIncreaseDefensivePowerPerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_ATTACK_SPEED_PER_LEVEL	:	//{	"공격속도증가per레벨"					:	
				operateGSIncreaseActionSpeedPerLevel(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_MOVE_SPEED_PER_LEVEL	:	//{	"이동속도증가per레벨"					:	
				operateGSIncreaseMoveSpeedPerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_HIT_CHANCE_PER_LEVEL	:	//{	"명중률증가per레벨"						:	
				operateGSIncreaseHitChancePerLevel(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_DODGE_CHANCE_PER_LEVEL	:	//{	"회피율증가per레벨"						:	
				operateGSIncreaseDodgeChancePerLevel(_iSkill,_iLevel,lpEffect);
				break;
		}
	}
}

void
cACTOR::operateGSIncreaseHP(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iIncreaseHP	=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];

	m_maxHP.m_sCorrectValueByPrefix	+=	iIncreaseHP;
}	//	cACTOR::operateGSIncreaseHP(int _iSkill,int _iLevel)

void
cACTOR::operateGSIncreaseCP(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iIncreaseCP	=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];

	m_maxCP.m_sCorrectValueByPrefix	+=	iIncreaseCP;
}	//	cACTOR::operateGSIncreaseCP(int _iSkill,int _iLevel)

void
cACTOR::operateGSIncreaseHitChance(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iHitChance	=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill]/100;

	m_hitChance.m_sValue	+=	iHitChance;
}	//	cACTOR::operateGSIncreaseHitChance(int _iSkill,int _iLevel)

void
cACTOR::operateGSIncreaseDodgeChance(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iDodgeChance=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill]/100;

	m_dodgeChance.m_sValue	+=	iDodgeChance;
}	//	cACTOR::operateGSIncreaseDodgeChance(int _iSkill,int _iLevel)

void
cACTOR::operateGSIncreaseStrength(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	m_strength.m_sValue		+=	iValue;
}	//	cACTOR::operateGSIncreaseStrength(int _iSkill,int _iLevel)

void
cACTOR::operateGSIncreaseAgility(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	m_agility.m_sValue		+=	iValue;
}	//	cACTOR::operateGSIncreaseAgility(int _iSkill,int _iLevel)

void
cACTOR::operateGSIncreaseConstitution(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	m_constitution.m_sValue	+=	iValue;
}	//	cACTOR::operateGSIncreaseConstitution(int _iSkill,int _iLevel)

void
cACTOR::operateGSIncreaseIntelligence(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	m_intelligence.m_sValue	+=	iValue;
}	//	cACTOR::operateGSIncreaseIntelligence(int _iSkill,int _iLevel)

void
cACTOR::operateGSIncreaseWisdom(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	m_wisdom.m_sValue		+=	iValue;
}	//	cACTOR::operateGSIncreaseWisdom(int _iSkill,int _iLevel)

void
cACTOR::operateGSIncreaseCharisma(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	m_charisma.m_sValue		+=	iValue;
}	//	cACTOR::operateGSIncreaseCharisma(int _iSkill,int _iLevel)

void
cACTOR::operateGSIncreaseLuck(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];
	
	m_luck.m_sValue			+=	iValue;
}	//	cACTOR::operateGSIncreaseLuck(int _iSkill,int _iLevel)

void
cACTOR::operateGSIncreaseJobSkillLevel(int _iSkill,int _iLevel,int _iJob)
{
	if (m_wJob	!=	_iJob)	return;

	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*100/lpSkill->m_aEffect[0].m_aiValue[0][m_wRankInGuildForSkill];

	if	(iValue%50	>=	50)	
		iValue	=	iValue/100+1;
	else
		iValue	/=	100;

	m_correctAllSkillLevel.addValue(iValue);;
}

void
cACTOR::operateGSAbsorbMagicDamagePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

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
}

void
cACTOR::operateGSIncreaseMagicResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;
	
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
}

void
cACTOR::operateGSIncreaseCrushResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	addCrushBlowResistance(iValue);
}

void
cACTOR::operateGSIncreaseCurseResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	addCurseResistance(iValue);
}

void
cACTOR::operateGSIncreaseStrangeStatusResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	addStrangeStatusResistance(iValue);
}

void
cACTOR::operateGSIncreaseDeclinePowerResitancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	addDeclinePowerResistance(iValue);
}

void
cACTOR::operateGSIncreasePhysicalMagicDamagePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_attackPower.m_sCorrectPercentageValueByPrefix		+=	iValue;

	m_strengtheningFireDamage.addValue(iValue,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
	m_strengtheningWaterDamage.addValue(iValue,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
	m_strengtheningWindDamage.addValue(iValue,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
	m_strengtheningEarthDamage.addValue(iValue,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
	m_strengtheningLightDamage.addValue(iValue,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
	m_strengtheningDarkDamage.addValue(iValue,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
}

void
cACTOR::operateGSIncreaseDefensivePowerPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_defensePower.m_sCorrectPercentageValueByPrefix	+=	iValue;
}


void
cACTOR::operateGSIncreaseActionSpeedPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_actionSpeed.addValue(iValue);
}

void
cACTOR::operateGSIncreaseMoveSpeedPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_moveSpeed.addValue(iValue);
}

void
cACTOR::operateGSIncreaseHitChancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_hitChance.addValue(iValue);
}

void
cACTOR::operateGSIncreaseDodgeChancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_dodgeChance.addValue(iValue);
}
