#include "CMessage.h"
#include "stdio.h"
#include "string.h"
#include <windows.h>
char	g_strToolMessage[200][256];
char	g_strDlgText[200][64];


#ifdef _FOR_KOREA
char	*g_strMiniPetName[]	=
{
"불의 영",
"물의 영",
"바람의 영",
"대지의 영",
"빛의 영",
"어둠의 영",
"사라만다",
"운디네",
"실프",
"놈",
"위스프",
"섀도우",
"화",
"수",
"풍",
"지",
"빛",
"암",
"하오링",
"미오링",
"수오링",
"다오링",
"차오링",
"고오링",
};

char	*g_strMessage[]	=
{
//	0~5
	"부숴진",
	"개량할 수 없는",
	"저주받은",
	"금액이 너무 많습니다. 플레이어는 21억 이상의 금액을 소지할 수 없습니다.",
	"금액이 너무 많습니다. 은행에는 42억 이상의 금액을 보관할 수 없습니다.",
	"선택한 플레이어는 그 정도의 금액을 가지고 있지 안습니다.",

//	6~10
	"은행에 그정도의 금액을 가지고 있지 안습니다.",
	"은행에 %u 골드를 더해 줍니다.\n\n계속 하시겠습니까?",
	"선택한 플레이어에게 %u 골드를 더해 줍니다.\n\n계속 하시겠습니까?",
	"선택한 플레이어의 소유금액 중에서 %u 골드를 제거합니다.\n\n계속 하시겠습니까?",
	"은행에서 %u 골드를 제거합니다.\n\n계속 하시겠습니까?",

//	11~15
	"[%s] 서버에 %d년 %d월 %d일 %d시 %d분에 OX 퀴즈 이벤트를 예약합니다.\n\n맞아유?",
	"월드 서버에 연결되어 있지 않습니다.",
	"크아아아!!",
	"[%s] 서버\n\n질문 : %s\n\n　답 : %s\n\n위 내용을 OX 퀴즈로 내려는게 맞습니까?",
	"OX 퀴즈",

//	16~20
	"현재 [%s] 서버에서 진행중인 OX 퀴즈를 종료 하시겠습니까?",
	"[%s] 서버에서 하기로 예약된 OX 퀴즈를 취소 하시겠습니까?",
	"현재 선택된 서버가\n\n%s\n\n입니다. 맞나요?",
	"OX 퀴즈 테스트",
	"[%s] 서버에서 패자 부활전을 실시합니다.\n\n맞아유?",

//	21~25
	"[%s] 서버에 %d년 %d월 %d일 %d시 %d분에 낱말 퀴즈 이벤트를 예약합니다.\n\n맞아유?",
	"[%s] 서버에서 하기로 예약된 낱말 퀴즈를 취소 하시겠습니까?",
	"[%s] 서버\n\n질문 : %s\n\n　답 : %s\n\n위 내용을 낱말 퀴즈로 내려는게 맞습니까?",
};

#else
char	*g_strMiniPetName[]	=
{
	"SpritOfFire",
	"SpritOfWater",
	"SpritOfWind",
	"SpritOfEarth",
	"SpritOfLight",
	"SpritOfDark",
	"Salamander",
	"Undine",
	"Silf",
	"Nom",
	"Wisp",
	"Shadow",
	"Hwa",
	"Soo",
	"Poong",
	"Ji",
	"Vit",
	"Am",
	"Haoring",
	"Mioring",
	"Sooring",
	"Daoring",
	"Chaoring",
	"Goring",
};

char	*g_strMessage[]	=
{
//	0~5
	"broken",
	"can not improve",
	"cursed",
	"too many gold. player can not own over 2100000000 gold",
	"too many gold. can not store over 4200000000 gold to bank",
	"low gold.",

//	6~10
	"too low gold in bank",
	"store [%u] gold to bank\n\ncontinue?",
	"add [%u] gold to select player.\n\ncontinue?",
	"remove [%u] gold from select player.\n\ncontinue?",
	"remove [%u] gold from bank.\n\ncontinue?",

//	11~15
	"Appointment OX quiz Event. [%s] server %d/%d/%d %d:%d \n\ncontinue?",
	"not connected with world server",
	"krrrrr!",
	"server [%s]\n\nquestion : %s\n\nsolution : %s\n\ncontinue?",
	"OX Quiz",

//	16~20
	"do you really end of ox quiz? server [%s]",
	"do you really disappointment ox quiz? server [%s]",
	"current select server\n\n%s\n\nok?",
	"OX quiz test",
	"begin Repechage. on server [%s]\n\nok?",

//	21~25
	"Appointment Word quiz Event. [%s] server %d/%d/%d %d:%d \n\ncontinue?",
	"do you really disappointment word quiz? server [%s]",
	"server [%s]\n\nquestion : %s\n\nsolution : %s\n\ncontinue?",

};
#endif

char	*g_strJobName[]	=	
{
#ifdef _FOR_KOREA
	"견습기사",
	"전사",
	"마법사",
	"늑대인간",
	"사제",
	"타락천사",
	"도둑",
	"무도가",
	"마법창병",
	"마법궁수",
	"조련사",
	"소환사",
	"공주",
	"마법소녀",
	"강신술사",
	"악마",
	"영술사",
	"투사",
	"광학사",
	"수인",
#else
	"KNIGHT",
	"WARRIOR",
	"WIZARD",
	"WEREWOLF",
	"PRIEST",
	"FALLEN_ANGEL",
	"ROGUE",
	"FIGHTER",
	"MAGIC_LANCER",
	"MAGIC_ARCHER",
	"BEAST_TAMER",
	"SUMMONER",
	"PRINCESS",
	"MAGICAL_GIRL",
	"NECROMANCER",
	"DEVIL",
	"SOUL_BRINGER",
	"CHAMPION",
	"OPTICALIST",
	"BEAST_MAN",

#endif
};


char	*g_strWeekName[] = 
{
#ifdef _FOR_KOREA
	"일요일",
	"월요일",
	"화요일",
	"수요일",
	"목요일",
	"금요일",
	"토요일",
#else
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday",
#endif
};

char	g_strItemCategory[100][32]	=
{
#ifdef _FOR_KOREA
	"헬멧",
	"관",
	"장갑",
	"장갑 대용",
	"발톱",
	"팔찌",
	"벨트",
	"부츠",
	"목걸이",
	"반지",
	"귀걸이",
	"망토",
	"브로치",
	"팔 문신",
	"어깨 문신",
	"십자가",
	"공용 갑옷",
	"전용 갑옷",
	"한 손 검",
	"방패",
	"양손 검",
	"마법 지팡이",
	"이빨",
	"메이스",
	"날개",
	"단도",
	"활",
	"화살",
	"창",
	"피리",
	"슬링",
	"슬링 탄환",
	"마술봉",
	"채찍",
	"보석",

	"체력 포션",
	"차징 포션",
	"스탯 향상 포션",
	"성능 향상 포션",
	"치료약",
	"상태 회복제",
	"열쇠",
	"포탈 스톤",
	"필살기 두루말이",
	"먹을 것, 기타",
	"능력향상제",
	"강화액",
	"마법 보석",
	"이벤트 아이템",
	"퀘스트 아이템",
	"프리미엄 아이템",
	"인챈트 아이템",
	"아이템 팩",

	"모든 무기",
	"낫",

#else
	"Helm",
	"Crown",
	"Glove",
	"like Glove",
	"Crow",
	"Bracelet",
	"Belt",
	"Boots",
	"Necklace",
	"Ring",
	"Earing",
	"Cloak",
	"Broach",
	"Arm Tattoo",
	"Shoulder Tattoo",
	"Cross",
	"Common Armor",
	"Exclusive Armor",
	"On handed Sword",
	"Shield",
	"Two handed Sword",
	"Wizard Rod",
	"Tooth",
	"Mace",
	"Wing",
	"Dagger",
	"Bow",
	"Arrow",
	"Spear",
	"Flute",
	"Sling",
	"Sling Bullet",
	"Magical Rod",
	"Whip",
	"Jewel",

	"Healing Potion",
	"Charging Potion",
	"Improve Status Potion",
	"Improve Capacity Potion",
	"Drug",
	"Drug for Bad Status",
	"Key",
	"Portal Stone",
	"Scroll",
	"Food,Etc",
	"Improve Power Potion",
	"Strengthning Liquid",
	"Magical Jewel",
	"Event Item",
	"Quest Item",
	"Premium Item",
	"Enchant Item",
	"Item Pack",

	"All Weapon",
	"Scythe",


#endif
	"end",
};


cITEM_PREFIX::cTYPE		g_aItemPrefixType[dMAX_ITEM_PREFIX_TYPE_COUNT]	=
{
#ifdef _FOR_KOREA
	eIP_ADD_STRENGTH,	"힘 증감",
	eIP_ADD_AGILITY,	"민첩성 증감",
	eIP_ADD_CONSTITUTION,	"건강 증감",
	eIP_ADD_WISDOM,	"지혜 증감",
	eIP_ADD_INTELIGENCE,	"지식 증감",
	eIP_ADD_CHARISMA,	"카리스마 증감",
	eIP_ADD_LUCK,	"운 증감",
	eIP_FIXING_STRENGTH,	"힘 고정",
	eIP_FIXING_AGILITY,	"민첩성 고정",
	eIP_FIXING_CONSTITUTION,	"건강 고정",
	eIP_FIXING_WISDOM,	"지혜 고정",
	eIP_FIXING_INTELLIGENCE,	"지식 고정",
	eIP_FIXING_CHARISMA,	"카리스마 고정",
	eIP_FIXING_LUCK,	"운 고정",
	eIP_ADD_STRENGTH_BY_LEVEL,	"레벨 비례 힘 증감",
	eIP_ADD_AGILITY_BY_LEVEL,	"레벨 비례 민첩성 증감",
	eIP_ADD_CONSTITUTION_BY_LEVEL,	"레벨 비례 건강 증감",
	eIP_ADD_WISDOM_BY_LEVEL,	"레벨 비례 지혜 증감",
	eIP_ADD_INTELIGENCE_BY_LEVEL,	"레벨 비례 지식 증감",
	eIP_ADD_CHARISMA_BY_LEVEL,	"레벨 비례 카리스마 증감",
	eIP_ADD_LUCK_BY_LEVEL,	"레벨 비례 운 증감",
	eIP_ADD_DAMAGE_PERCENTAGE,	"무기 대미지 증감 (퍼센티지)",
	eIP_ADD_MIN_DAMAGE,	"무기 대미지 증감 (최소값)",
	eIP_ADD_MAX_DAMAGE,	"무기 대미지 증감 (최대값)",
	eIP_ADD_DEFENSE_PERCENTAGE,	"방어력 증감 (퍼센티지)",
	eIP_ADD_DEFENSE,	"방어력 증감 (수치)",
	eIP_ADD_MAX_HP_PERCENTAGE,	"최대 체력 증감 (퍼센티지)",
	eIP_ADD_MAX_HP,	"최대 체력 증감 (수치)",
	eIP_ADD_MAX_CP_PERCENTAGE,	"최대 CP 증감 (퍼센티지)",
	eIP_ADD_MAX_CP,	"최대 CP 증감 (수치)",
	eIP_LOW_CP,	"이동시 감소 한계 CP",
	eIP_CORRECT_HIT_RATE,	"공격 성공 확률 (명중률)",
	eIP_CORRECT_DODGE_RATE,	"공격을 피할 확률 (회피율)",
	eIP_SHIELD_BLOCK_RATE,	"방패 발동 확률",
	eIP_CRITICAL_HIT,	"치명타 발동 확률",
	eIP_CRUSH_SHOT,	"결정타 발동 확률",
	eIP_IGNORE_TARGET_DODGE_ABILITY,	"적 방어자의 회피 보정치 무시",
	eIP_IGNORE_ATTACKER_CORRECT_HIT_VALUE,	"적 공격자의 명중 보정치 무시",
	eIP_PERFECT_HIT,	"항상 명중",
	eIP_BLOCKING_SPEED,	"블러킹 속도",
	eIP_MOVE_SPEED,	"이동 속도",
	eIP_ACTION_SPEED,	"공격 속도",
	eIP_POTION_HEAL_SPEED,	"물약 회복 속도 조절",
	eIP_ADD_CONCENTRATION ,	"CP 감소 속도 조절 (집중력)",
	eIP_FIRE_DAMAGE,	"불 속성 공격",
	eIP_WATER_DAMAGE,	"물 속성 공격",
	eIP_WIND_DAMAGE,	"바람 속성 공격",
	eIP_TERRA_DAMAGE,	"대지 속성 공격",
	eIP_LIGHT_DAMAGE,	"빛 속성 공격",
	eIP_DARK_DAMAGE,	"어둠 속성 공격",
	eIP_STEAL_HP,	"체력 스틸",
	eIP_BONUS_CP,	"보너스 CP",
	eIP_FLEE,	"도망",
	eIP_KNOCK_OUT,	"녹아웃",
	eIP_INSTANT_KILL,	"즉사",
	eIP_WEAPON_BREAKER,	"무기 파괴",
	eIP_ARMOR_BREAKER,	"갑옷 파괴",
	eIP_MAKE_COLD,	"콜드 상태",
	eIP_MAKE_FREEZE,	"프리즈 상태",
	eIP_MAKE_BLIND,	"장님 상태",
	eIP_MAKE_STUN,	"스턴 상태",
	eIP_MAKE_STONE,	"석화 상태",
	eIP_MAKE_CONFUSE,	"혼란 상태",
	eIP_MAKE_SLEEP,	"잠 상태",
	eIP_MAKE_CHARMING,	"차밍 상태",
	eIP_MAKE_FIRST_TARGET,	"우선 타겟이 된 상태",
	eIP_MAKE_AI_DOWN ,	"AI 저하 상태",
	eIP_MAKE_BERSERK,	"버서크 상태",
	eIP_RESISTANCE_FIRE,	"불 속성 공격 저항",
	eIP_RESISTANCE_WATER,	"물 속성 공격 저항",
	eIP_RESISTANCE_WIND,	"바람 속성 공격 저항",
	eIP_RESISTANCE_TERRA,	"대지 속성 공격 저항",
	eIP_RESISTANCE_LIGHT,	"빛 속성 공격 저항",
	eIP_RESISTANCE_DARK,	"어둠 속성 공격 저항",
	eIP_RESISTANCE_MAGIC,	"모든 마법 속성 공격 저항",
	eIP_ABSORB_FIRE,	"불 속성 공격 흡수",
	eIP_ABSORB_WATER,	"물 속성 공격 흡수",
	eIP_ABSORB_WIND,	"바람 속성 공격 흡수",
	eIP_ABSORB_TERRA,	"대지 속성 공격 흡수",
	eIP_ABSORB_LIGHT,	"빛 속성 공격 흡수",
	eIP_ABSORB_DARK,	"어둠 속성 공격 흡수",
	eIP_ABSORB_MAGIC,	"모든 마법 속성 공격 흡수",
	eIP_RESISTANCE_FREEZE,	"프리즈 공격에 대한 저항",
	eIP_RESISTANCE_COLD,	"콜드 공격에 대한 저항",
	eIP_RESISTANCE_STUN,	"스턴 공격에 대한 저항",
	eIP_RESISTANCE_CONFUSE,	"혼란 공격에 대한 저항",
	eIP_RESISTANCE_CHARMING,	"차밍 공격에 대한 저항",
	eIP_RESISTANCE_STONE,	"석화 공격에 대한 저항",
	eIP_RESISTANCE_INSTANCE_KILL,	"즉사 공격에 대한 저항",
	eIP_RESISTANCE_POISON,	"중독 공격에 대한 저항",
	eIP_RESISTANCE_SLEEP,	"잠 공격에 대한 저항",
	eIP_RESISTANCE_STRANGE_STATUS,	"상태이상 공격에 대한 저항",
	eIP_RESISTANCE_DECLINE_STATUS,	"저하계 공격에 대한 저항",
	eIP_RESISTANCE_CURSE_STATUS,	"저주계 공격에 대한 저항",
	eIP_RESISTANCE_ALL_BAD_STATUS,	"모든 이상 공격에 대한 저항",
	eIP_RESISTANCE_KNOCK_BACK,	"녹백/넘어짐 공격에 대한 저항",
	eIP_RESISTANCE_CRITICAL_SHOT,	"치명타 공격에 대한 저항",
	eIP_RESISTANCE_CRUSH_SHOT,	"결정타 공격에 대한 저항",
	eIP_RETURN_DAMAGE,	"받은 대미지 일부 돌려 줌",
	eIP_DAMAGE_TO_CP,	"받은 대미지 CP 변환",
	eIP_MAKE_FREEZE_ATTACKER,	"공격자 프리즈",
	eIP_MAKE_COLD_ATTACKER,	"공격자 콜드",
	eIP_CHARGE_SKILL,	"스킬/마법 충전",
	eIP_JOB_SKILL_LEVEL,	"계열 스킬/마법 레벨 조절",
	eIP_ALL_SKILL_LEVEL,	"모든 스킬/마법 레벨 조절",
	eIP_FIRST_AID,	"타격 응급 처치",
	eIP_CHANGE_WEAPON_SPEED,	"무기 교환 속도",
	eIP_REGENERATION_HP,	"체력 리제네레이션",
	eIP_RESURRECTION_CHANCE,	"부활 확률",
	eIP_BLURRED,	"흐릿한 상태 (Blurred)",
	eIP_INVISIBLE,	"투명 상태 (Invisible)",
	eIP_LEVITATE,	"공중 부양",
	eIP_MAGIC_BULLET,	"마법 탄환",
	eIP_INFINITY_BULLET,	"무한 탄환",
	eIP_NOT_AIMED,	"타겟에서 제외",
	eIP_INCREASE_RARE_ITEM_DROP_CHANCE,	"마법 아이템 확률 증가",
	eIP_INCREASE_UNIQUE_ITEM_DROP_CHANCE,"희귀 아이템 확률 증가",
	eIP_DECREASE_RELOAD_TIME,	"벨트 채우는 시간 감소",
	eIP_AUTO_RELOAD,	"벨트 자동 채우기",
	eIP_DISABLE,	"사용 불가",
	eIP_CAN_NOT_STRIP,	"착용 후 탈의 불능",
	eIP_CAN_NOT_ADD_PREFIX,	"추가 인챈트 불능",
	eIP_UPGRADE_FIRE_DAMAGE_ATTACK,"불 공격 1단계 상승",
	eIP_UPGRADE_WATER_DAMAGE_ATTACK,"물 공격 1단계 상승",
	eIP_UPGRADE_WIND_DAMAGE_ATTACK,"바람 공격 1단계 상승",
	eIP_UPGRADE_EARTH_DAMAGE_ATTACK,"대지 공격 1단계 상승",
	eIP_UPGRADE_LIGHT_DAMAGE_ATTACK,"빛 공격 1단계 상승",
	eIP_UPGRADE_DARK_DAMAGE_ATTACK,"어둠 공격 1단계 상승",

	eIP_CONTROL_SKILL_LEVEL_BY_DIFFICULTY	, "레벨별 스킬/마법 레벨 조절",
	eIP_INCREASE_SUMMON_BEAST_STATUS		, "소환수 스탯 증가",
	eIP_INCREASE_GAIN_EXP_FOR_PET			, "펫 경험치 획득 증가",

#else
	eIP_ADD_STRENGTH,		"Increase Strength",
	eIP_ADD_AGILITY,		"Increase Agility",
	eIP_ADD_CONSTITUTION,	"Increase Constituton",
	eIP_ADD_WISDOM,			"Increase Wisdom",
	eIP_ADD_INTELIGENCE,	"Increase Intelligence",
	eIP_ADD_CHARISMA,		"Increase Charisma",
	eIP_ADD_LUCK,			"Increase Luck",
	eIP_FIXING_STRENGTH,	"Fixing Strength",
	eIP_FIXING_AGILITY,		"Fixing Agility",
	eIP_FIXING_CONSTITUTION,"Fixing Constituton",
	eIP_FIXING_WISDOM,		"Fixing Wisdom",
	eIP_FIXING_INTELLIGENCE,"Fixing Intelligence",
	eIP_FIXING_CHARISMA,	"Fixing Charisma",
	eIP_FIXING_LUCK,		"Fixing Luck",
	eIP_ADD_STRENGTH_BY_LEVEL,		"Increase Strength per Level",
	eIP_ADD_AGILITY_BY_LEVEL,		"Increase Agility per Level",
	eIP_ADD_CONSTITUTION_BY_LEVEL,	"Increase Constituton per Level",
	eIP_ADD_WISDOM_BY_LEVEL,		"Increase Wisdom per Level",
	eIP_ADD_INTELIGENCE_BY_LEVEL,	"Increase Intelligence per Level",
	eIP_ADD_CHARISMA_BY_LEVEL	,	"Increase Charisma per Level",
	eIP_ADD_LUCK_BY_LEVEL		,	"Increase Luck per Level",
	eIP_ADD_DAMAGE_PERCENTAGE	,	"Increase Weapon Damage(percentage)",
	eIP_ADD_MIN_DAMAGE,	"Increase Damage(min Damage)",
	eIP_ADD_MAX_DAMAGE,	"Increase Damage(max Damage)",
	eIP_ADD_DEFENSE_PERCENTAGE,	"Increase Defense (percentage)",
	eIP_ADD_DEFENSE,	"Increase Defense (value)",
	eIP_ADD_MAX_HP_PERCENTAGE,	"Increase Max HP(percentage)",
	eIP_ADD_MAX_HP,	"Increase Max HP(value)",
	eIP_ADD_MAX_CP_PERCENTAGE,	"Increase Max CP(percentage)",
	eIP_ADD_MAX_CP,	"Increase Max CP(value)",
	eIP_LOW_CP,	"Limit reduce CP(while move)",
	eIP_CORRECT_HIT_RATE,	"공격 성공 확률 (명중률)",
	eIP_CORRECT_DODGE_RATE,	"공격을 피할 확률 (회피율)",
	eIP_SHIELD_BLOCK_RATE,	"방패 발동 확률",
	eIP_CRITICAL_HIT,	"치명타 발동 확률",
	eIP_CRUSH_SHOT,	"결정타 발동 확률",
	eIP_IGNORE_TARGET_DODGE_ABILITY,	"적 방어자의 회피 보정치 무시",
	eIP_IGNORE_ATTACKER_CORRECT_HIT_VALUE,	"적 공격자의 명중 보정치 무시",
	eIP_PERFECT_HIT,	"항상 명중",
	eIP_BLOCKING_SPEED,	"블러킹 속도",
	eIP_MOVE_SPEED,	"이동 속도",
	eIP_ACTION_SPEED,	"공격 속도",
	eIP_POTION_HEAL_SPEED,	"물약 회복 속도 조절",
	eIP_ADD_CONCENTRATION ,	"CP 감소 속도 조절 (집중력)",
	eIP_FIRE_DAMAGE,	"불 속성 공격",
	eIP_WATER_DAMAGE,	"물 속성 공격",
	eIP_WIND_DAMAGE,	"바람 속성 공격",
	eIP_TERRA_DAMAGE,	"대지 속성 공격",
	eIP_LIGHT_DAMAGE,	"빛 속성 공격",
	eIP_DARK_DAMAGE,	"어둠 속성 공격",
	eIP_STEAL_HP,	"체력 스틸",
	eIP_BONUS_CP,	"보너스 CP",
	eIP_FLEE,	"도망",
	eIP_KNOCK_OUT,	"녹아웃",
	eIP_INSTANT_KILL,	"즉사",
	eIP_WEAPON_BREAKER,	"무기 파괴",
	eIP_ARMOR_BREAKER,	"갑옷 파괴",
	eIP_MAKE_COLD,	"콜드 상태",
	eIP_MAKE_FREEZE,	"프리즈 상태",
	eIP_MAKE_BLIND,	"장님 상태",
	eIP_MAKE_STUN,	"스턴 상태",
	eIP_MAKE_STONE,	"석화 상태",
	eIP_MAKE_CONFUSE,	"혼란 상태",
	eIP_MAKE_SLEEP,	"잠 상태",
	eIP_MAKE_CHARMING,	"차밍 상태",
	eIP_MAKE_FIRST_TARGET,	"우선 타겟이 된 상태",
	eIP_MAKE_AI_DOWN ,	"AI 저하 상태",
	eIP_MAKE_BERSERK,	"버서크 상태",
	eIP_RESISTANCE_FIRE,	"불 속성 공격 저항",
	eIP_RESISTANCE_WATER,	"물 속성 공격 저항",
	eIP_RESISTANCE_WIND,	"바람 속성 공격 저항",
	eIP_RESISTANCE_TERRA,	"대지 속성 공격 저항",
	eIP_RESISTANCE_LIGHT,	"빛 속성 공격 저항",
	eIP_RESISTANCE_DARK,	"어둠 속성 공격 저항",
	eIP_RESISTANCE_MAGIC,	"모든 마법 속성 공격 저항",
	eIP_ABSORB_FIRE,	"불 속성 공격 흡수",
	eIP_ABSORB_WATER,	"물 속성 공격 흡수",
	eIP_ABSORB_WIND,	"바람 속성 공격 흡수",
	eIP_ABSORB_TERRA,	"대지 속성 공격 흡수",
	eIP_ABSORB_LIGHT,	"빛 속성 공격 흡수",
	eIP_ABSORB_DARK,	"어둠 속성 공격 흡수",
	eIP_ABSORB_MAGIC,	"모든 마법 속성 공격 흡수",
	eIP_RESISTANCE_FREEZE,	"프리즈 공격에 대한 저항",
	eIP_RESISTANCE_COLD,	"콜드 공격에 대한 저항",
	eIP_RESISTANCE_STUN,	"스턴 공격에 대한 저항",
	eIP_RESISTANCE_CONFUSE,	"혼란 공격에 대한 저항",
	eIP_RESISTANCE_CHARMING,	"차밍 공격에 대한 저항",
	eIP_RESISTANCE_STONE,	"석화 공격에 대한 저항",
	eIP_RESISTANCE_INSTANCE_KILL,	"즉사 공격에 대한 저항",
	eIP_RESISTANCE_POISON,	"중독 공격에 대한 저항",
	eIP_RESISTANCE_SLEEP,	"잠 공격에 대한 저항",
	eIP_RESISTANCE_STRANGE_STATUS,	"상태이상 공격에 대한 저항",
	eIP_RESISTANCE_DECLINE_STATUS,	"저하계 공격에 대한 저항",
	eIP_RESISTANCE_CURSE_STATUS,	"저주계 공격에 대한 저항",
	eIP_RESISTANCE_ALL_BAD_STATUS,	"모든 이상 공격에 대한 저항",
	eIP_RESISTANCE_KNOCK_BACK,	"녹백/넘어짐 공격에 대한 저항",
	eIP_RESISTANCE_CRITICAL_SHOT,	"치명타 공격에 대한 저항",
	eIP_RESISTANCE_CRUSH_SHOT,	"결정타 공격에 대한 저항",
	eIP_RETURN_DAMAGE,	"받은 대미지 일부 돌려 줌",
	eIP_DAMAGE_TO_CP,	"받은 대미지 CP 변환",
	eIP_MAKE_FREEZE_ATTACKER,	"공격자 프리즈",
	eIP_MAKE_COLD_ATTACKER,	"공격자 콜드",
	eIP_CHARGE_SKILL,	"스킬/마법 충전",
	eIP_JOB_SKILL_LEVEL,	"계열 스킬/마법 레벨 조절",
	eIP_ALL_SKILL_LEVEL,	"모든 스킬/마법 레벨 조절",
	eIP_FIRST_AID,	"타격 응급 처치",
	eIP_CHANGE_WEAPON_SPEED,	"무기 교환 속도",
	eIP_REGENERATION_HP,	"체력 리제네레이션",
	eIP_RESURRECTION_CHANCE,	"부활 확률",
	eIP_BLURRED,	"흐릿한 상태 (Blurred)",
	eIP_INVISIBLE,	"투명 상태 (Invisible)",
	eIP_LEVITATE,	"공중 부양",
	eIP_MAGIC_BULLET,	"마법 탄환",
	eIP_INFINITY_BULLET,	"무한 탄환",
	eIP_NOT_AIMED,	"타겟에서 제외",
	eIP_INCREASE_RARE_ITEM_DROP_CHANCE,	"마법 아이템 확률 증가",
	eIP_INCREASE_UNIQUE_ITEM_DROP_CHANCE,"희귀 아이템 확률 증가",
	eIP_DECREASE_RELOAD_TIME,	"벨트 채우는 시간 감소",
	eIP_AUTO_RELOAD,	"벨트 자동 채우기",
	eIP_DISABLE,	"사용 불가",
	eIP_CAN_NOT_STRIP,	"착용 후 탈의 불능",
	eIP_CAN_NOT_ADD_PREFIX,	"추가 인챈트 불능",
	eIP_UPGRADE_FIRE_DAMAGE_ATTACK,"불 공격 1단계 상승",
	eIP_UPGRADE_WATER_DAMAGE_ATTACK,"물 공격 1단계 상승",
	eIP_UPGRADE_WIND_DAMAGE_ATTACK,"바람 공격 1단계 상승",
	eIP_UPGRADE_EARTH_DAMAGE_ATTACK,"대지 공격 1단계 상승",
	eIP_UPGRADE_LIGHT_DAMAGE_ATTACK,"빛 공격 1단계 상승",
	eIP_UPGRADE_DARK_DAMAGE_ATTACK,"어둠 공격 1단계 상승",

	eIP_CONTROL_SKILL_LEVEL_BY_DIFFICULTY	, "레벨별 스킬/마법 레벨 조절",
	eIP_INCREASE_SUMMON_BEAST_STATUS		, "소환수 스탯 증가",
	eIP_INCREASE_GAIN_EXP_FOR_PET			, "펫 경험치 획득 증가",

#endif
	0xffff,
};
char	*g_strEventName[] =
{	// 마지막 수정일 : 09.10.09
#ifdef	_FOR_KOREA
	"이구동성",
	"크리스마스",

#else
	"Unanimous", // 이구동성
	"Christmas", // 크리스마스..	
#endif
};
int l_iTextLine = 0;

int readline(FILE * fp,char * _aTxt)
{
	int		tmp = 0;
	int		count=0;
	bool	isSpecial = false;
	char	_readline[512];
	memset(_readline,0,512);

	tmp = fgetc(fp);
	while(tmp!=EOF && tmp!=10)
	{
		_readline[count] = (char)tmp;
		if(isSpecial)
		{
			if(_readline[count]=='n')
			{
				count--;
				_readline[count]='\n';
				count++;
				tmp = fgetc(fp);
				isSpecial = false;
				continue;
			}else if(_readline[count]=='r')
			{
				count--;
				_readline[count]='\r';
				count++;
				tmp = fgetc(fp);
				isSpecial = false;
				continue;
			}else
				isSpecial = false;
		}
		if(_readline[count]=='\\')
		{
			isSpecial = true;
		}
		count++;
		tmp = fgetc(fp);
	}
	_readline[count]=0;
	if(tmp==EOF)	return 0;
	if(count==0)	return 1;

	if(strncmp(_readline,"//",2)==0)	return 1;

	strcpy(_aTxt,_readline);
	l_iTextLine++;

	return 2;
}

int		initMessage()
{
	int ret = 0;
	int cc = 0;
	bool	loop = true;
	l_iTextLine = 0;

#ifndef _FOR_KOREA
	FILE * fp = fopen("message_eng.txt","r");

	if (!fp)
		fp = fopen("message.txt","r");
#else
	FILE * fp = fopen("message.txt","r");
#endif

	if(!fp)		return 0;
	while(loop)
	{
		ret = readline(fp,g_strToolMessage[cc]);
		switch(ret)
		{
		case 0:
			loop = false;
			break;
		case 1:
			break;
		case 2:
			cc++;
			break;
		}
	}
	fclose(fp);

#ifndef _FOR_KOREA
	ret = 0;
	cc = 0;
	loop = true;
	l_iTextLine = 0;
	fp = fopen("controltext.txt","r");
	if(!fp)		return 0;
	while(loop)
	{
		ret = readline(fp,g_strDlgText[cc]);
		switch(ret)
		{
		case 0:
			loop = false;
			break;
		case 1:
			break;
		case 2:
			cc++;
			break;
		}
	}
	fclose(fp);
#endif

	return 1;
}

