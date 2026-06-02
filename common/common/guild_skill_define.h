#ifndef _classGuildSkillDefine_h
#define _classGuildSkillDefine_h

#include <winsock2.h>
#ifndef _WINDOWS_
#include <windows.h>
#endif

//
//	Guild Skill Effect
enum
{
	eGSE_INCREASE_MAX_GUILD_MEMBER_COUNT,	//	최대길드원증가
	eGSE_INCREASE_MAX_GUILD_SUB_MASTER_COUNT,//최대서브마스터수증가
	eGSE_INCREASE_MAX_GUILD_CONGRESS_COUNT,	//	최대길드의원인원수증가
	eGSE_INCREASE_GUILD_VILLAGE,			//	연고지 증가
	eGSE_INCREASE_GUILD_INVENTORY_SIZE,		//	길드인벤토리사이즈증가
	eGSE_INCREASE_HP,						//체력증가				
	eGSE_INCREASE_CP,						//CP증가					
	eGSE_INCREASE_HIT_CHANCE,				//정확도증가				
	eGSE_INCREASE_DODGE_CHANCE,				//회피율증가				
	eGSE_INCREASE_STRENGTH,					//힘증가					
	eGSE_INCREASE_AGILITY,					//민첩성증가				
	eGSE_INCREASE_CONSTITUTION,				//건강증가				
	eGSE_INCREASE_INTELLIGENCE,				//지식증가				
	eGSE_INCREASE_WISDOM,					//지혜증가				
	eGSE_INCREASE_CHARISMA,					//카리스마증가			
	eGSE_INCREASE_LUCK,						//운증가					
	eGSE_INCREASE_KNIGHT_SKILL_LEVEL,		//기사스킬레벨증가		
	eGSE_INCREASE_WARRIOR_SKILL_LEVEL,		//전사스킬레벨증가		
	eGSE_INCREASE_WIZARD_SKILL_LEVEL,		//마법사스킬레벨증가		
	eGSE_INCREASE_WEREWOLF_SKILL_LEVEL,		//늑대인간스킬레벨증가	
	eGSE_INCREASE_PRIEST_SKILL_LEVEL,		//사제스킬레벨증가		
	eGSE_INCREASE_FALLEN_ANGEL_SKILL_LEVEL,	//타락천사스킬레벨증가	
	eGSE_INCREASE_ROGUE_SKILL_LEVEL,		//도둑스킬레벨증가		
	eGSE_INCREASE_FIGHTER_SKILL_LEVEL,		//무도가스킬레벨증가		
	eGSE_INCREASE_MAGIC_LANCER_SKILL_LEVEL,	//마법창병스킬레벨증가	
	eGSE_INCREASE_MAGIC_ARCHER_SKILL_LEVEL,	//마법궁수스킬레벨증가	
	eGSE_INCREASE_TAMER_SKILL_LEVEL,		//조련사스킬레벨증가		
	eGSE_INCREASE_SUMMONER_SKILL_LEVEL,		//소환사스킬레벨증가		
	eGSE_INCREASE_PRINCESS_SKILL_LEVEL,		//공주스킬레벨증가		
	eGSE_INCREASE_MAGIC_GIRL_SKILL_LEVEL,	//마법소녀스킬레벨증가	
	eGSE_INCREASE_NECROMANCER_SKILL_LEVEL,	//강신술사스킬레벨증가	
	eGSE_INCREASE_DEVIL_SKILL_LEVEL,		//악마스킬레벨증가		
	
	eGSE_ABSORB_FIRE_PER_LEVEL	,	//{	"불흡수per레벨"			,	
	eGSE_ABSORB_WATER_PER_LEVEL	,	//{	"물흡수per레벨"			,	
	eGSE_ABSORB_WIND_PER_LEVEL	,	//{	"바람흡수per레벨"		,	
	eGSE_ABSORB_EARTH_PER_LEVEL	,	//{	"대지흡수per레벨"		,	
	eGSE_ABSORB_LIGHT_PER_LEVEL	,	//{	"빛흡수per레벨"			,	
	eGSE_ABSORB_DARK_PER_LEVEL	,	//{	"어둠흡수per레벨"		,	

	eGSE_INCREASE_DAMAGE_UNDER_SIEGE_WARFARE	,	//	"공성전시대미지상승per레벨"	,	
	eGSE_REDUCE_DAMAGE_UNDER_SIEGE_WARFARE		,	//	"공성전시대미지감소per레벨"	,	
	eGSE_INCREASE_GUILD_PET_SIGHT				,	//	"길드펫시야증가
	eGSE_INCREASE_GUILD_CREST_EFFICIENT			,	//	"길드문장효율증가"
	eGSE_INCREASE_GUILD_FLAG_EFFICIENT			,	//	"길드깃발효율증가"
	eGSE_INCREASE_ASTRO_BOW_DAMAGE				,	//	"아스트로보우대미지증가",
	eGSE_INCREASE_GUILD_DOOR_EFFICIENT			,	//	"길드문효율증가
	eGSE_INCREASE_GUILD_PET_ATTACK_SPEED		,	//	"길드펫액션속도증가
	eGSE_DECREASE_GUILD_SHOP_ITEM_PRICE			,	//	"길드상점아이템가격감소
	eGSE_INCREASE_DASH_BLADE_DAMAGE				,	//	"대쉬블레이드대미지증가

	eGSE_INCREASE_RESISTANCE_OF_FIRE_PER_LEVEL	,	//{	"불저항력증가per레벨"		,	
	eGSE_INCREASE_RESISTANCE_OF_WATER_PER_LEVEL	,	//{	"물저항력증가per레벨"		,	
	eGSE_INCREASE_RESISTANCE_OF_WIND_PER_LEVEL	,	//{	"바람저항력증가per레벨"		,	
	eGSE_INCREASE_RESISTANCE_OF_EARTH_PER_LEVEL	,	//{	"대지저항력증가per레벨"		,	
	eGSE_INCREASE_RESISTANCE_OF_LIGHT_PER_LEVEL	,	//{	"빛저항력증가per레벨"		,	
	eGSE_INCREASE_RESISTANCE_OF_DARK_PER_LEVEL	,	//{	"어둠저항력증가per레벨"		,	

	eGSE_DECREASE_FIELD_FIRE_RESISTANCE_PENELTY_PER_LEVEL	,	//{	"필드불저항페널티감소per레벨"		,	
	eGSE_DECREASE_FIELD_WATER_RESISTANCE_PENELTY_PER_LEVEL	,	//{	"필드물저항페널티감소per레벨"		,	
	eGSE_DECREASE_FIELD_WIND_RESISTANCE_PENELTY_PER_LEVEL	,	//{	"필드바람저항페널티감소per레벨"		,	
	eGSE_DECREASE_FIELD_EARTH_RESISTANCE_PENELTY_PER_LEVEL	,	//{	"필드대지저항페널티감소per레벨"		,	
	eGSE_DECREASE_FIELD_LIGHT_RESISTANCE_PENELTY_PER_LEVEL	,	//{	"필드빛저항페널티감소per레벨"		,	
	eGSE_DECREASE_FIELD_DARK_RESISTANCE_PENELTY_PER_LEVEL	,	//{	"필드어둠저항페널티감소per레벨"		,	

	eGSE_INCREASE_OBITIAN_COUNT_PER_LEVEL		,	//{	"오비티안개수증가per레벨"				,	
	eGSE_INCREASE_CRISTAL_WATER_COUNT_PER_LEVEL	,	//{	"크리스탈워터개수증가per레벨"			,	
	eGSE_INCREASE_ASTRAL_BOW_COUNT_PER_LEVEL	,	//{	"아스트로보우개수증가per레벨"			,	
	eGSE_INCREASE_DASH_BLADE_COUNT_PER_LEVEL	,	//{	"대쉬블래이드개수증가per레벨"			,	

	eGSE_INCREASE_CRITICAL_CRUSH_RESISTANCE_PER_LEVEL	,	//{	"치명타결정타저항증가per레벨"			,	
	eGSE_INCREASE_CURSE_RESISTANCE_PER_LEVEL			,	//{	"저주계저항증가per레벨"					,	
	eGSE_INCREASE_STRANGE_STATUS_RESISTANCE_PER_LEVEL	,	//{	"이상계저항증가per레벨"					,	
	eGSE_INCREASE_DECLINE_POWER_RESISTANCE_PER_LEVEL	,	//{	"저하계저항증가per레벨"					,	

	eGSE_INCREASE_PHYSICAL_MAGIC_DAMAGE_PER_LEVEL	,	//{	"물리속성공격력증가per레벨"					,	
	eGSE_DEFENSIVE_POWER_PER_LEVEL					,	//{	"방어력증가퍼센티지per레벨"				,	

	eGSE_INCREASE_ATTACK_SPEED_PER_LEVEL	,	//{	"공격속도증가per레벨"					,	
	eGSE_INCREASE_MOVE_SPEED_PER_LEVEL		,	//{	"이동속도증가per레벨"					,	

	eGSE_INCREASE_HIT_CHANCE_PER_LEVEL		,	//{	"명중률증가per레벨"						,	
	eGSE_INCREASE_DODGE_CHANCE_PER_LEVEL	,	//{	"회피율증가per레벨"						,	

	eGSE_INCREASE_GUARDIAN_COUNT_PER_LEVEL	,	//{	"가디언수증가per레벨"					,	
	eGSE_INCREASE_GUARDIAN_SIGHT_PER_LEVEL	,	//{	"가디언시야증가"						,	

	eGSE_SELL_MAGIC_ENCHANT_ITEM			,	//{	"마법인챈트용품판매"				,	

	eGSE_DECREASE_DUNGEON_MAGIC_ATTR_PENELTY_PER_LEVEL	,	//{	"던젼속성페널티저하per레벨"						,	
	eGSE_DECREASE_DEATH_PENELTY_PER_LEVEL				,	//{	"사망페널티감소per레벨"					,	
	eGSE_INCREASE_SOUL_BRINGER_SKILL_LEVEL,			// 영술사스킬레벨증가	
	eGSE_INCREASE_CHAMPION_SKILL_LEVEL,			// 투사스킬레벨증가	
	eGSE_INCREASE_OPTICALIST_SKILL_LEVEL,		//	광학사스킬레벨증가
};

#define	dGUILD_SKILL_EFFECT_VARIATION_COUNT					4
#define	dGUILD_SKILL_EFFECT_VALUE_COUNT						6
#define	dMAX_GUILD_SKILL_EFFECT_COUNT						6
#define	dGUILD_SKILL_TOP_LEVEL_LIMIT_EXPRESSION_VALUE_COUNT	6
#define	dGUILD_SKILL_REQUIRE_TITLE_COUNT					6
#define	dGUILD_SKILL_REQUIRE_SKILL_COUNT					6
#define	dSKILL_NAME_LENGTH									32
#define	dGUILD_SKILL_COMMENT_LENGTH							252

const	int	c_iMaxGuildStatueSkillLevel	=	5;

class	CGuildSkillEffectInfo
{
public:
	WORD			m_wEffect;
	int				m_aiValue[dGUILD_SKILL_EFFECT_VALUE_COUNT][dGUILD_SKILL_EFFECT_VARIATION_COUNT];
};	//	class	CSkillExtraEffectInfo

class	CGuildSkillDefine
{
public:
	WORD					m_wSerial;
	WORD					m_wIconIndex;						//	아이콘 인덱스
	char					m_strName[dSKILL_NAME_LENGTH];

	CGuildSkillEffectInfo	m_aEffect[dMAX_GUILD_SKILL_EFFECT_COUNT];
	WORD					m_awRequireSkill[dGUILD_SKILL_REQUIRE_SKILL_COUNT];
	WORD					m_awRequireSkillLevel[dGUILD_SKILL_REQUIRE_SKILL_COUNT];
	
	WORD					m_wTopLevelLimitExpression;
	WORD					m_aTopLevelLimitExpressionValue[dGUILD_SKILL_TOP_LEVEL_LIMIT_EXPRESSION_VALUE_COUNT];

	WORD					m_wTopLevel;						//	한계 레벨
	short					m_sMaxLevelByGuildHallLevel;		//	길드 홀 레벨에 의한 최고 레벨
	int						m_iRequireGold;

	char					m_strComment[dGUILD_SKILL_COMMENT_LENGTH];
	DWORD					m_bf1IsStatueSkill				:	1;	//	석상 스킬이다.
	DWORD					m_bf1IsRequireSkillMasteryTitle :	1;
	DWORD					m_bf3RequireGuildHallLevel		:	3;	//	필요한 길드 홀 레벨
	
							CGuildSkillDefine()
							{
								reset();
							}

	void					reset()
	{
		memset(this,0,sizeof(CGuildSkillDefine));

		m_wSerial					=	0xffff;
		m_wIconIndex				=	0;

		memset(m_aEffect,0xff,sizeof(m_aEffect));
		memset(m_awRequireSkill,0xff,sizeof(m_awRequireSkill));

		m_wTopLevelLimitExpression	=	0;
		m_wTopLevel					=	0;						//	한계 레벨
		m_sMaxLevelByGuildHallLevel	=	0;		//	길드 홀 레벨에 의한 최고 레벨
		m_iRequireGold				=	0;

		m_bf1IsRequireSkillMasteryTitle	=	FALSE;
	}
};

#endif