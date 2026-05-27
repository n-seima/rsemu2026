#include "CGuildSkill.h"
#include "CDecoder.h"
#include "CTitle.h"
#include "CLoadTextCode.h"

CGuildSkillManager	g_gsManager;
#ifdef	_DEBUG
CDecoderKeyword	l_aGuildSkillEffect[]	=
{
	{	"최대길드원증가"		,	eGSE_INCREASE_MAX_GUILD_MEMBER_COUNT},		//
	{	"최대서브마스터수증가"	,	eGSE_INCREASE_MAX_GUILD_SUB_MASTER_COUNT},	//
	{	"최대길드의원인원수증가",	eGSE_INCREASE_MAX_GUILD_CONGRESS_COUNT},	//
	{	"연고지증가"			,	eGSE_INCREASE_GUILD_VILLAGE},		//
	{	"길드인벤토리사이즈증가",	eGSE_INCREASE_GUILD_INVENTORY_SIZE},		//
	{	"체력증가"				,	eGSE_INCREASE_HP},
	{	"CP증가"				,	eGSE_INCREASE_CP},
	{	"정확도증가"			,	eGSE_INCREASE_HIT_CHANCE},
	{	"회피율증가"			,	eGSE_INCREASE_DODGE_CHANCE},
	{	"힘증가"				,	eGSE_INCREASE_STRENGTH},
	{	"민첩성증가"			,	eGSE_INCREASE_AGILITY},
	{	"건강증가"				,	eGSE_INCREASE_CONSTITUTION},
	{	"지식증가"				,	eGSE_INCREASE_INTELLIGENCE},
	{	"지혜증가"				,	eGSE_INCREASE_WISDOM},
	{	"카리스마증가"			,	eGSE_INCREASE_CHARISMA},
	{	"운증가"				,	eGSE_INCREASE_LUCK},
	{	"기사스킬레벨증가"		,	eGSE_INCREASE_KNIGHT_SKILL_LEVEL},
	{	"전사스킬레벨증가"		,	eGSE_INCREASE_WARRIOR_SKILL_LEVEL},
	{	"마법사스킬레벨증가"	,	eGSE_INCREASE_WIZARD_SKILL_LEVEL},
	{	"늑대인간스킬레벨증가"	,	eGSE_INCREASE_WEREWOLF_SKILL_LEVEL},
	{	"사제스킬레벨증가"		,	eGSE_INCREASE_PRIEST_SKILL_LEVEL},
	{	"타락천사스킬레벨증가"	,	eGSE_INCREASE_FALLEN_ANGEL_SKILL_LEVEL},
	{	"도둑스킬레벨증가"		,	eGSE_INCREASE_ROGUE_SKILL_LEVEL},
	{	"무도가스킬레벨증가"	,	eGSE_INCREASE_FIGHTER_SKILL_LEVEL},
	{	"마법궁수스킬레벨증가"	,	eGSE_INCREASE_MAGIC_ARCHER_SKILL_LEVEL},
	{	"마법창병스킬레벨증가"	,	eGSE_INCREASE_MAGIC_LANCER_SKILL_LEVEL},
	{	"조련사스킬레벨증가"	,	eGSE_INCREASE_TAMER_SKILL_LEVEL},
	{	"소환사스킬레벨증가"	,	eGSE_INCREASE_SUMMONER_SKILL_LEVEL},
	{	"공주스킬레벨증가"		,	eGSE_INCREASE_PRINCESS_SKILL_LEVEL},
	{	"변신소녀스킬레벨증가"	,	eGSE_INCREASE_MAGIC_GIRL_SKILL_LEVEL},
	{	"강신술사스킬레벨증가"	,	eGSE_INCREASE_NECROMANCER_SKILL_LEVEL},
	{	"악마스킬레벨증가"		,	eGSE_INCREASE_DEVIL_SKILL_LEVEL},

	{	"불흡수per레벨"			,	eGSE_ABSORB_FIRE_PER_LEVEL},
	{	"물흡수per레벨"			,	eGSE_ABSORB_WATER_PER_LEVEL},
	{	"바람흡수per레벨"		,	eGSE_ABSORB_WIND_PER_LEVEL},
	{	"대지흡수per레벨"		,	eGSE_ABSORB_EARTH_PER_LEVEL},
	{	"빛흡수per레벨"			,	eGSE_ABSORB_LIGHT_PER_LEVEL},
	{	"어둠흡수per레벨"		,	eGSE_ABSORB_DARK_PER_LEVEL},


	{	"공성전시대미지상승per레벨"	,eGSE_INCREASE_DAMAGE_UNDER_SIEGE_WARFARE	},	//	"공성전시대미지상승per레벨"	,	
	{	"공성전시대미지감소per레벨"	,eGSE_REDUCE_DAMAGE_UNDER_SIEGE_WARFARE		},	//	"공성전시대미지감소per레벨"	,	
	{	"길드펫시야증가"			,eGSE_INCREASE_GUILD_PET_SIGHT				},	//	"길드펫시야증가",
	{	"길드문장효율증가"			,eGSE_INCREASE_GUILD_CREST_EFFICIENT		},	//	"길드문장효율증가",
	{	"길드깃발효율증가"			,eGSE_INCREASE_GUILD_FLAG_EFFICIENT			},	//	"길드깃발효율증가",
	{	"아스트로보우대미지증가"	,eGSE_INCREASE_ASTRO_BOW_DAMAGE				},	//	"아스트로보우대미지증가",
	{	"길드문효율증가"			,eGSE_INCREASE_GUILD_DOOR_EFFICIENT			},	//	"길드문효율증가",
	{	"길드펫액션속도증가"		,eGSE_INCREASE_GUILD_PET_ATTACK_SPEED		},	//	"길드펫액션속도증가",
	{	"길드상점아이템가격감소"	,eGSE_DECREASE_GUILD_SHOP_ITEM_PRICE		},	//	"길드상점아이템가격감소",
	{	"대쉬블레이드대미지증가"	,eGSE_INCREASE_DASH_BLADE_DAMAGE			},	//	"대쉬블레이드대미지증가",

	{	"불저항력증가per레벨"		,	eGSE_INCREASE_RESISTANCE_OF_FIRE_PER_LEVEL},
	{	"물저항력증가per레벨"		,	eGSE_INCREASE_RESISTANCE_OF_WATER_PER_LEVEL},
	{	"바람저항력증가per레벨"		,	eGSE_INCREASE_RESISTANCE_OF_WIND_PER_LEVEL},
	{	"대지저항력증가per레벨"		,	eGSE_INCREASE_RESISTANCE_OF_EARTH_PER_LEVEL},
	{	"빛저항력증가per레벨"		,	eGSE_INCREASE_RESISTANCE_OF_LIGHT_PER_LEVEL},
	{	"어둠저항력증가per레벨"		,	eGSE_INCREASE_RESISTANCE_OF_DARK_PER_LEVEL},

	{	"필드불저항페널티감소per레벨"		,	eGSE_DECREASE_FIELD_FIRE_RESISTANCE_PENELTY_PER_LEVEL	},
	{	"필드물저항페널티감소per레벨"		,	eGSE_DECREASE_FIELD_WATER_RESISTANCE_PENELTY_PER_LEVEL	},
	{	"필드바람저항페널티감소per레벨"		,	eGSE_DECREASE_FIELD_WIND_RESISTANCE_PENELTY_PER_LEVEL	},
	{	"필드대지저항페널티감소per레벨"		,	eGSE_DECREASE_FIELD_EARTH_RESISTANCE_PENELTY_PER_LEVEL	},
	{	"필드빛저항페널티감소per레벨"		,	eGSE_DECREASE_FIELD_LIGHT_RESISTANCE_PENELTY_PER_LEVEL	},
	{	"필드어둠저항페널티감소per레벨"		,	eGSE_DECREASE_FIELD_DARK_RESISTANCE_PENELTY_PER_LEVEL	},

	{	"오비티안개수증가per레벨"				,	eGSE_INCREASE_OBITIAN_COUNT_PER_LEVEL},
	{	"크리스탈워터개수증가per레벨"			,	eGSE_INCREASE_CRISTAL_WATER_COUNT_PER_LEVEL},
	{	"아스트로보우개수증가per레벨"			,	eGSE_INCREASE_ASTRAL_BOW_COUNT_PER_LEVEL},
	{	"대쉬블래이드개수증가per레벨"			,	eGSE_INCREASE_DASH_BLADE_COUNT_PER_LEVEL},

	{	"치명타결정타저항증가per레벨"			,	eGSE_INCREASE_CRITICAL_CRUSH_RESISTANCE_PER_LEVEL},
	{	"저주계저항증가per레벨"					,	eGSE_INCREASE_CURSE_RESISTANCE_PER_LEVEL},
	{	"이상계저항증가per레벨"					,	eGSE_INCREASE_STRANGE_STATUS_RESISTANCE_PER_LEVEL},
	{	"저하계저항증가per레벨"					,	eGSE_INCREASE_DECLINE_POWER_RESISTANCE_PER_LEVEL},

	{	"물리속성공격력증가per레벨"				,	eGSE_INCREASE_PHYSICAL_MAGIC_DAMAGE_PER_LEVEL},
	{	"방어력증가퍼센티지per레벨"				,	eGSE_DEFENSIVE_POWER_PER_LEVEL},

	{	"공격속도증가per레벨"					,	eGSE_INCREASE_ATTACK_SPEED_PER_LEVEL},
	{	"이동속도증가per레벨"					,	eGSE_INCREASE_MOVE_SPEED_PER_LEVEL},

	{	"명중률증가per레벨"						,	eGSE_INCREASE_HIT_CHANCE_PER_LEVEL},
	{	"회피율증가per레벨"						,	eGSE_INCREASE_DODGE_CHANCE_PER_LEVEL},

	{	"가디언수증가per레벨"					,	eGSE_INCREASE_GUARDIAN_COUNT_PER_LEVEL},
	{	"가디언시야증가"						,	eGSE_INCREASE_GUARDIAN_SIGHT_PER_LEVEL},

	{	"마법인챈트용품판매"					,	eGSE_SELL_MAGIC_ENCHANT_ITEM},

	{	"던젼속성페널티저하per레벨"				,	eGSE_DECREASE_DUNGEON_MAGIC_ATTR_PENELTY_PER_LEVEL},
	{	"사망페널티감소per레벨"					,	eGSE_DECREASE_DEATH_PENELTY_PER_LEVEL},
	{	"영술사스킬레벨증가"					,	eGSE_INCREASE_SOUL_BRINGER_SKILL_LEVEL},
	{	"투사스킬레벨증가"						,	eGSE_INCREASE_CHAMPION_SKILL_LEVEL},
	{	"광학사스킬레벨증가"					,	eGSE_INCREASE_OPTICALIST_SKILL_LEVEL},
	{	"end"									,	dCODE_END		},
};
#endif
//
//	리셋
void
CGuildSkillManager::reset()
{
	m_iSkillCount		=	0;
	m_iStatueSkillCount	=	0;
	m_iNormalSkillCount	=	0;

	for (int i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
		m_aSkills[i].reset();
}

BOOL
CGuildSkillManager::addSkill(CGuildSkill *_lpSkill)
{
#ifdef	_IS_DEV_CLIENT
	if	(_lpSkill->m_wSerial	>=	dMAX_GUILD_SKILL_COUNT)	
		EXIT("error in CGuildSkillManager::addSkill","추가하려는 길드스킬 '%s'의 시리얼이 이상합니다.",_lpSkill->m_strName);

	CGuildSkill	*lpSlot		=	&m_aSkills[_lpSkill->m_wSerial];

	if	(lpSlot->m_wSerial	!=	0xffff)
		EXIT("error in CGuildSkillManager::addSkill","추가하려는 길드스킬 '%s'의 시리얼이 이미 존재합니다.",_lpSkill->m_strName);

	memcpy(lpSlot,_lpSkill,sizeof(CGuildSkill));

	m_iSkillCount++;

	if	(_lpSkill->m_bf1IsStatueSkill)
		m_iStatueSkillCount++;
	else
		m_iNormalSkillCount++;
#endif
	return	TRUE;
}

BOOL
CGuildSkillManager::readScriptFile(char *_lpstrFN)
{
#ifdef	_IS_DEV_CLIENT
	CDecoder	reader;
	CGuildSkill	skill,*lpSkill;
	int			iValue,iEffectCount,iRequreTitleCount,iRequireSkillCount;
	BOOL		bIsLooping	=	TRUE;

	if	(!reader.Upload(_lpstrFN,NULL))
		return	loadData();

	while(bIsLooping)
	{
		char	*str;
		char	*strKeyword	=	reader.getWord(TRUE);
		BOOL	bIsCorrect;

		switch(reader.analCode(strKeyword))
		{
			case	dCODE_CLOSE		:
			{
				addSkill(&skill);
				continue;
			}

			case	dCODE_END		:
				bIsLooping	=	FALSE;
				continue;
				
			case	dCODE_FINISH	:
			case	dCODE_OPEN		:
				continue;
		}

		if (STRICMP("스킬",strKeyword)==0)
		{
			skill.reset();
			skill.m_wSerial					=	reader.getNumber(&bIsCorrect);
			iEffectCount					=	0;
			iRequreTitleCount	=	0;

			if (!bIsCorrect)
				EXIT("error in CGuildSkillManager::init()","스킬 인덱스를 정의할 수 없습니다.'");

			str					=	reader.getStream();

			strcpy(skill.m_strName,str);

			continue;
		}

		if (STRICMP("제약by길드홀레벨",strKeyword)==0)
		{
			skill.m_sMaxLevelByGuildHallLevel	=	reader.getNumber(&bIsCorrect);

			if (!bIsCorrect)
				EXIT("error in CGuildSkillManager::init()","'%s'의 제약by길드홀레벨을 정의 할 수 없습니다.'",skill.m_strName);

			continue;
		}
		if (STRICMP("한계레벨",strKeyword)==0)
		{
			skill.m_wTopLevel					=	reader.getNumber(&bIsCorrect);

			if (!bIsCorrect)
				EXIT("error in CGuildSkillManager::init()","'%s'의 한계레벨을 정의 할 수 없습니다.'",skill.m_strName);

			continue;
		}

		if (STRICMP("스킬마스터칭호필요",strKeyword)==0)
		{
			skill.m_bf1IsRequireSkillMasteryTitle	=	TRUE;
			continue;
		}
		
		if (STRICMP("한계레벨결정수치",strKeyword)==0)
		{
			int	iIndex		=	reader.getNumber(&bIsCorrect);
			if (!bIsCorrect)
				EXIT("error in CGuildSkillManager::init()","'%s'의 한계레벨결정수치를 정의할 수 없습니다.'",skill.m_strName);

			skill.m_aTopLevelLimitExpressionValue[iIndex]	=	reader.getNumber(&bIsCorrect);

			if (!bIsCorrect)
				EXIT("error in CGuildSkillManager::init()","'%s'의 한계레벨결정수치를 정의할 수 없습니다.'",skill.m_strName);

			continue;
		}
		if (STRICMP("한계레벨결정수식",strKeyword)==0)
		{
			skill.m_wTopLevelLimitExpression	=	reader.getNumber(&bIsCorrect);

			if (!bIsCorrect)
				EXIT("error in CGuildSkillManager::init()","'%s'의 한계레벨결정수치를 정의할 수 없습니다.'",skill.m_strName);

			continue;
		}

		if (STRICMP("아이콘",strKeyword)==0)
		{
			skill.m_wIconIndex					=	reader.getNumber(&bIsCorrect);

			if (!bIsCorrect)
				EXIT("error in CGuildSkillManager::init()","'%s'의 아이콘을 정의 할 수 없습니다.'",skill.m_strName);

			continue;
		}
		if	(STRICMP("필요길드홀레벨",strKeyword)==0)
		{
			skill.m_bf3RequireGuildHallLevel	=	reader.getNumber(&bIsCorrect);

			if	(!bIsCorrect)
				EXIT("error in CGuildSkillManager::init()","'%s'의 필요길드홀레벨을 정의 할 수 없습니다.'",skill.m_strName);

			continue;
		}
		if	(STRICMP("레벨업에필요한금액",strKeyword)==0)
		{
			skill.m_iRequireGold				=	reader.getNumber(&bIsCorrect);

			if	(!bIsCorrect)
				EXIT("error in CGuildSkillManager::init()","'%s'의 레벨업에필요한금액을 정의 할 수 없습니다.'",skill.m_strName);

			continue;
		}
		if	(STRICMP("석상스킬",strKeyword)==0)
		{
			skill.m_bf1IsStatueSkill	=	TRUE;
			continue;
		}
		if	(STRICMP("설명",strKeyword)==0)
		{
			str				=	reader.getStream();
			strcpy(skill.m_strComment,str);
			continue;
		}

		if (STRICMP("스킬효과",strKeyword)==0)
		{
			CGuildSkillEffectInfo	*lpInfo	=	&skill.m_aEffect[iEffectCount++];

			int	iEffectValueIndex	=	0;
			str						=	reader.getWord();
			iValue					=	CDecoder::AnalCode(l_aGuildSkillEffect,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CGuildSkillManager::init()","길드스킬효과 '%s'를 정의 할 수 없습니다.'",str);

			lpInfo->m_wEffect=	iValue;

			reader.findOpenCode();	//	오픈코드
			
			while(1)
			{
				str				=	reader.getWord();

				DWORD	dwCode	=	reader.analCode(str);

				if	(dwCode	==	dCODE_CLOSE	)
					break;
				if	(dwCode	==	dCODE_END	||	dwCode	==	dCODE_FINISH||	dwCode	==	dCODE_OPEN)
					continue;

				if (STRICMP("수치",str)==0)
				{
					iValue		=	reader.getNumber(&bIsCorrect);

					if (!bIsCorrect)
						EXIT("error in CGuildSkillManager::init()","타이틀효과-수치-수치 인덱스를 정의할 수 없습니다.");

					iEffectValueIndex	=	iValue-1;

					for (int iVariation=0;iVariation<dGUILD_SKILL_EFFECT_VARIATION_COUNT;iVariation++)
					{
						iValue	=	reader.getNumber(&bIsCorrect);

						if	(!bIsCorrect)
							EXIT("error in CGuildSkillManager::init()","타이틀효과-수치-수치 인덱스를 정의할 수 없습니다.");

						lpInfo->m_aiValue[iEffectValueIndex][iVariation]	=	iValue;
					}

					continue;
				}

				break;
			}
			continue;
		}
	}

	reader.rewind();

	bIsLooping	=	TRUE;

	while(bIsLooping)
	{
		char	*str;
		char	*strKeyword	=	reader.getWord(TRUE);
		BOOL	bIsCorrect;

		switch(reader.analCode(strKeyword))
		{
			case	dCODE_CLOSE		:
				continue;

			case	dCODE_END		:
				bIsLooping	=	FALSE;
				continue;
				
			case	dCODE_FINISH	:
			case	dCODE_OPEN		:
				continue;
		}

		if (STRICMP("스킬",strKeyword)==0)
		{
			str					=	reader.getStream();
			lpSkill				=	getByName(str);
			iRequireSkillCount	=	0;
			continue;
		}

		if (STRICMP("필요스킬",strKeyword)==0)
		{
			str							=	reader.getStream();
			CGuildSkill	*lpRequireSkill	=	getByName(str);

			if (!lpRequireSkill)
				EXIT("error in CGuildSkillManager::init()","%s 스킬의 필요 스킬 등록 중에 찾을 수 없는 필요 스킬이 있습니다. %s",lpSkill->m_strName,str);

			lpSkill->m_awRequireSkill[iRequireSkillCount]		=	lpRequireSkill->m_wSerial;
			lpSkill->m_awRequireSkillLevel[iRequireSkillCount]	=	reader.getNumber(&bIsCorrect);

			iRequireSkillCount++;

			if (!bIsCorrect)
				EXIT("error in CGuildSkillManager::init()","'%s'의 필요스킬의 레벨을 정의할 수 없습니다.'",lpSkill->m_strName);

			continue;
		}
	}
#endif
	return	TRUE;
}

BOOL
CGuildSkillManager::init()
{
#ifdef	_IS_DEV_CLIENT
	reset();

	if	(!readScriptFile("dev data/scripter/guild_skill.txt"))
		return	loadData();
	if	(!readScriptFile("dev data/scripter/guild_skill_by_statue.txt"))
		return	loadData();

	return	saveData();
#else
	return	loadData();
#endif
}	//	CGuildSkillManager::init()


//
//	데이터 읽기
BOOL
CGuildSkillManager::loadData()
{
	cFILE	file;

	if	(!file.Open("data/guildSkill.dat","rb"))
		EXIT("error in CGuildSkillManager::loadData","can not find 'data/guildSkill.dat' file.");

	file.Read(&g_gsManager.m_iSkillCount,4);
	file.Read(g_gsManager.m_aSkills,sizeof(g_gsManager.m_aSkills));

	m_iStatueSkillCount	=	0;
	m_iNormalSkillCount	=	0;

	for (int i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
	{
		if	(m_aSkills[i].m_wSerial	==	0xffff)
			continue;

		if	(m_aSkills[i].m_bf1IsStatueSkill)
			m_iStatueSkillCount++;
		else
			m_iNormalSkillCount++;
	}

	file.Close();

	return	TRUE;
}

//
//	데이터 저장
BOOL
CGuildSkillManager::saveData()
{
#ifdef	_IS_DEV_CLIENT
	cFILE	file;

	if (!file.Open("data/guildSkill.dat","wb"))	return	FALSE;

	file.Write(&g_gsManager.m_iSkillCount,4);
	file.Write(g_gsManager.m_aSkills,sizeof(g_gsManager.m_aSkills));
	file.Close();

#endif
	return	TRUE;
}