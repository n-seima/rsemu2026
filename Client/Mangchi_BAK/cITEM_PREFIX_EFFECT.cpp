#include "cITEM_PREFIX_EFFECT.H"
#include "cMAIN.h"
#include "cTEXT.h"
#include "cDRAW.h"
#include "resource.h"

cDECODER		cPREFIX_EFFECT::s_Reader;
int				cPREFIX_EFFECT::s_iSelectEffect			=	0;
int				cPREFIX_EFFECT::s_iSelectEffectCategory	=	0;
cITEM_EFFECT	cPREFIX_EFFECT::s_Effect;
HWND			cPREFIX_EFFECT::s_hDlg				=	NULL;

int				cPREFIX_EFFECT::s_iSpinIndex			=	0;
int				cPREFIX_EFFECT::s_iSpinBoxIndex		=	0;
int				cPREFIX_EFFECT::s_iStreamIndex		=	0;
int				cPREFIX_EFFECT::s_iComboIndex		=	0;
BOOL			cPREFIX_EFFECT::s_isJongSung		=	FALSE;	//	
char			cPREFIX_EFFECT::s_strFilter[256]	=	"";

cSTATIC			cPREFIX_EFFECT::s_aStream[10];
cCOMBOBOX		cPREFIX_EFFECT::s_aCombo[10];
cEDITBOX		cPREFIX_EFFECT::s_aSpinBox[10];
cSPIN			cPREFIX_EFFECT::s_aSpin[10];
cTREEVIEW		cPREFIX_EFFECT::s_treeView;

cKEYWORD_ITEM
cPREFIX_EFFECT::s_aKeyword[100]	=
{
	"spin"				,dIKW_SPIN				,
	"plus_minus_set"	,dIKW_PLUS_MINUS_SET	,
	"min_max_damage"	,dIKW_MIN_MAX_DAMAGE	,
	"base_power"		,dIKW_BASE_POWER		,
	"chargeable_skil"	,dIKW_CHARGEABLE_SKILL	,
	"attack_bonus_skill",dIKW_ATTACK_BONUS_SKILL,
	"revenge_skill"		,dIKW_REVENGE_SKILL		,
	"passive_skill"		,dIKW_PASSIVE_SKILL		,
	"owner_skill"		,dIKW_ALL_SKILL			,
	"plus_minus"		,dIKW_PLUS_MINUS		,
	"ap_dp"				,dIKW_AP_DP				,
	"magic_attrib"		,dIKW_MAGIC_ATTRIBUTE	,
	"job"				,dIKW_JOB				,
	"end"				,dCODE_END
};


char
cPREFIX_EFFECT::s_strComboStr[100][20][40]	=
{
	{""},
	{"더해준다.","빼준다.","으로 설정한다."					,"end"},	//	dIKW_PLUS_MINUS_SET
	{"최소 데미지","최대 데미지","전체 데미지"				,"end"},	//	dIKW_MIN_MAX_DAMAGE
	{"힘","민첩성","건강","카리스마","지식","지혜","행운"	,"end"},	//	dIKW_BASE_POWER
	{""},																//	dIKW_CHARGEABLE_SKILL
	{""},																//	dIKW_ATTACK_BONUS_SKILL
	{""},																//	dIKW_REVENGE_SKILL
	{""},																//	dIKW_PASSIVE_SKILL
	{""},																//	dIKW_ALL_SKILL
	{"더해준다.","빼준다."									,"end"},	//	dIKW_PLUS_MINUS
	{"공격력","방어력"										,"end"},	//	dIKW_AP_DP
	{"불꽃","물","바람","대지","빛","어둠"					,"end"},	//	dIKW_MAGIC_ATTRIBUTE
	{"견습기사","전사","마법사","늑대인간","사제","타락천사","도둑","무도가",	//	dIKW_JOB
	 "마법 창검사","마법 궁수","조련사","소환사","공주","변신 소녀","강신술사","악마","영술사", "end"},
};

cPREFIX_EFFECT_REFERENCE
cPREFIX_EFFECT::s_aHyperText[dMAX_ITEM_PREFIX_TYPE_COUNT]	=
{
//////////////////////////////////////////////////
//	기본 능력치 조절
//////////////////////////////////////////////////
	eIP_ADD_STRENGTH,"착용자의 힘이 [spin]0[1,1000] 증가한다.",
	eIP_ADD_AGILITY,"착용자의 민첩성이 [spin]0[1,1000] 증가한다.",
	eIP_ADD_CONSTITUTION,"착용자의 건강이 [spin]0[1,1000] 증가한다.",
	eIP_ADD_WISDOM,"착용자의 지혜가 [spin]0[1,1000] 증가한다.",
	eIP_ADD_INTELIGENCE,"착용자의 지식이 [spin]0[1,1000] 증가한다.",
	eIP_ADD_CHARISMA,"착용자의 카리스마가 [spin]0[1,1000] 증가한다.",
	eIP_ADD_LUCK,"착용자의 운이 [spin]0[1,1000] 증가한다.",
	eIP_FIXING_STRENGTH,"착용자의 힘을 [spin]0[1,1000] 으로 고정 시킨다.",
	eIP_FIXING_AGILITY,"착용자의 민첩성을 [spin]0[1,1000] 으로 고정 시킨다.",
	eIP_FIXING_CONSTITUTION,"착용자의 건강을 [spin]0[1,1000] 으로 고정 시킨다.",
	eIP_FIXING_WISDOM,"착용자의 지혜를 [spin]0[1,1000] 으로 고정 시킨다.",
	eIP_FIXING_INTELLIGENCE,"착용자의 지식을 [spin]0[1,1000] 으로 고정 시킨다.",
	eIP_FIXING_CHARISMA,"착용자의 카리스마를 [spin]0[1,1000] 으로 고정 시킨다.",
	eIP_FIXING_LUCK,"착용자의 운을 [spin]0[1,1000] 으로 고정 시킨다.",
	eIP_ADD_STRENGTH_BY_LEVEL,"착용자의 레벨 [spin]0[1,100]당 힘이 [spin]1[1,100] 증가한다.",
	eIP_ADD_AGILITY_BY_LEVEL,"착용자의 레벨 [spin]0[1,100]당 민첩성이 [spin]1[1,100] 증가한다.",
	eIP_ADD_CONSTITUTION_BY_LEVEL,"착용자의 레벨 [spin]0[1,100]당 건강이 [spin]1[1,100] 증가한다.",
	eIP_ADD_WISDOM_BY_LEVEL,"착용자의 레벨 [spin]0[1,100]당 지혜가[spin]1[1,100] 증가한다.",
	eIP_ADD_INTELIGENCE_BY_LEVEL,"착용자의 레벨 [spin]0[1,100]당 지식이 [spin]1[1,100] 증가한다.",
	eIP_ADD_CHARISMA_BY_LEVEL,"착용자의 레벨 [spin]0[1,100]당 카리스마가 [spin]1[1,100] 증가한다.",
	eIP_ADD_LUCK_BY_LEVEL,"착용자의 레벨 [spin]0[1,100]당 운이 [spin]1[1,100] 증가한다.",
	eIP_ADD_DAMAGE_PERCENTAGE,"데미지가 [spin]0[1,1000]퍼센트 증가한다.",
	eIP_ADD_MIN_DAMAGE,"무기 최소 데미지가 [spin]0[1,1000] 증가한다.",
	eIP_ADD_MAX_DAMAGE,"무기 최대 데미지가 [spin]0[1,1000] 증가한다.",
	eIP_ADD_DEFENSE_PERCENTAGE,"방어력이 [spin]0[1,1000]퍼센트 증가한다.",
	eIP_ADD_DEFENSE,"방어력이 [spin]0[1,1000] 증가한다.",
	eIP_ADD_MAX_HP_PERCENTAGE,"최대 체력이 [spin]0[1,1000]퍼센트 증가한다.",
	eIP_ADD_MAX_HP,"최대 체력이 [spin]0[1,1000] 증가한다.",
	eIP_ADD_MAX_CP_PERCENTAGE,"최대 CP가 [spin]0[1,1000]퍼센트 증가한다.",
	eIP_ADD_MAX_CP,"최대 CP가 [spin]0[1,1000] 증가한다.",
	eIP_LOW_CP,"이동시 감소 한계 CP가 [spin]0[1,1000] 증가한다.",

//////////////////////////////////////////////////
//	각종 확률 보정
//////////////////////////////////////////////////
	eIP_CORRECT_HIT_RATE,"명중률이 [spin]0[1,1000] 증가한다.",
	eIP_CORRECT_DODGE_RATE,"회피율이 [spin]0[1,1000] 증가한다.",
	eIP_SHIELD_BLOCK_RATE,"방패 블럭률 [spin]0[1,1000] 증가한다.",
	eIP_CRITICAL_HIT,"치명타 발동 확률이 [spin]0[1,1000] 증가한다.",
	eIP_CRUSH_SHOT,"결정타 발동 확률이 [spin]0[1,1000] 증가한다.",
	eIP_IGNORE_TARGET_DODGE_ABILITY,"방어자의 회피율 보정치 무시한다.",
	eIP_IGNORE_ATTACKER_CORRECT_HIT_VALUE,"공격자의 명중률 보정치 무시한다.",
	eIP_PERFECT_HIT,"항상 명중한다.",
	eIP_INCREASE_HARD_BLOW,"강타 확률 [spin]0[1,1000]％ 증가한다",
//////////////////////////////////////////////////////
//	속도/시간 보정
//////////////////////////////////////////////////////
	eIP_BLOCKING_SPEED,"블럭킹 속도가 [spin]0[1,1000]퍼센트 증가한다.",
	eIP_MOVE_SPEED,"이동 속도가 [spin]0[1,1000]퍼센트 증가한다.",
	eIP_ACTION_SPEED,"공격 속도가 [spin]0[1,1000]퍼센트 증가한다.",
	eIP_POTION_HEAL_SPEED,"물약 회복 속도를 ([spin]0[1,1000]+50)퍼센트 증가 시킨다.",
	eIP_ADD_CONCENTRATION,"집중력을 [spin]0[1,1000]퍼센트 증가 시킨다.",

//////////////////////////////////////////////////////////////////////////////
//	공격 성공시 추가 효과
//////////////////////////////////////////////////////////////////////////////
	eIP_FIRE_DAMAGE,"대상에게 [spin]0[1,20000]~[spin]1[1,20000]의 불 속성 데미지를 입힌다.",
	eIP_WATER_DAMAGE,"대상에게 [spin]0[1,20000]~[spin]1[1,20000]의 물 속성 데미지를 입히고, [spin]2[1,1000]Frame동안 얼린다.",
	eIP_WIND_DAMAGE,"대상에게 [spin]0[1,20000]~[spin]1[1,20000]의 바람 속성 데미지를 입히고, [spin]2[1,1000]Frame동안 스턴 시킨다.",
	eIP_TERRA_DAMAGE,"대상에게 [spin]2[1,1000]초 동안, 초당 [spin]0[1,20000]~[spin]1[1,20000]의 중독 데미지를 입힌다.",
	eIP_LIGHT_DAMAGE,"대상에게 [spin]0[1,20000]~[spin]1[1,20000]의 빛 속성 데미지를 입히고, [spin]2[1,1000]초 동안 명중,회피 저하상태로 만든다.",
	eIP_DARK_DAMAGE,"대상에게 [spin]0[1,20000]~[spin]1[1,20000]의 어둠 속성 데미지를 입히고, [spin]2[1,1000]초 동안 저주를 건다.",
	eIP_STEAL_HP,"공격 성공시 적에게 입힌 데미지의 [spin]0[1,1000]퍼센트를 체력으로 흡수한다.",
	eIP_BONUS_CP,"CP 획득시 [spin]0[1,1000]퍼센트의 CP를 보너스로 얻는다.",
	eIP_FLEE,"공격 성공시 [spin]0[1,100]퍼센트의 확률로 대상을 도망가게 한다.",
	eIP_KNOCK_OUT,"녹아웃 공격 성공률을 [spin]0[1,100]퍼센트의 증가시킨다.",
	eIP_INSTANT_KILL,"[spin]0[1,100]퍼센트의 확률로 즉사 공격이 발동한다.",
	eIP_WEAPON_BREAKER,"[spin]0[1,100]퍼센트의 확률로 무기파괴 공격이 발동하고, [spin]1[1,1000]초동안 지속된다.",
	eIP_ARMOR_BREAKER,"[spin]0[1,100]퍼센트의 확률로 갑옷파괴 공격이 발동하고, [spin]1[1,1000]초동안 지속된다.",
	eIP_MAKE_COLD,"[spin]0[1,100]퍼센트의 확률로 콜드 공격이 발동하고, [spin]1[1,1000]초동안 지속된다.",
	eIP_MAKE_FREEZE,"[spin]0[1,100]퍼센트의 확률로 프리즈 공격이 발동하고, [spin]1[1,1000]초 동안 지속된다.",
	eIP_MAKE_BLIND,"[spin]0[1,100]퍼센트의 확률로 장님 공격이 발동하고, [spin]1[1,1000]초 동안 지속된다.",
	eIP_MAKE_STUN,"[spin]0[1,100]퍼센트의 확률로 스턴 공격이 발동하고, [spin]1[1,1000]초 동안 지속된다.",
	eIP_MAKE_STONE,"[spin]0[1,100]퍼센트의 확률로 석화 공격이 발동하고, [spin]1[1,1000]초 동안 지속된다.",
	eIP_MAKE_CONFUSE,"[spin]0[1,100]퍼센트의 확률로 혼란 공격이 발동하고, [spin]1[1,1000]초 동안 지속된다.",
	eIP_MAKE_SLEEP,"[spin]0[1,100]퍼센트의 확률로 잠 공격이 발동하고, [spin]1[1,1000]초 동안 지속된다.",
	eIP_MAKE_CHARMING,"[spin]0[1,100]퍼센트의 확률로 차밍 공격이 발동하고, [spin]1[1,1000]초 동안 지속된다.",
	eIP_MAKE_FIRST_TARGET,"[spin]0[1,100]퍼센트의 확률로 우선 타겟 만들기 공격이 발동하고, [spin]1[1,1000]초 동안 지속된다.",
	eIP_MAKE_AI_DOWN ,"[spin]0[1,100]퍼센트의 확률로 AI 저하 공격이 발동하고, [spin]1[1,1000]초 동안 지속된다.",
	eIP_MAKE_BERSERK,"[spin]0[1,100]퍼센트의 확률로 버서크 상태 만들기 공격이 발동하고, [spin]1[1,1000]초 동안 지속된다.",

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	마법 공격 저항
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	eIP_RESISTANCE_FIRE,"불 속성 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_WATER,"물 속성 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_WIND,"바람 속성 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_TERRA,"대지 속성 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_LIGHT,"빛 속성 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_DARK,"어둠 속성 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_MAGIC,"모든 마법 속성 공격에 대한 저항이 [spin]0[1,100]퍼센트의 증가한다.",
	eIP_ABSORB_FIRE,"불 속성 데미지의 [spin]0[1,100]퍼센트를 흡수해서 체력으로 만든다.",
	eIP_ABSORB_WATER,"물 속성 데미지의 [spin]0[1,100]퍼센트를 흡수해서 체력으로 만든다.",
	eIP_ABSORB_WIND,"바람 속성 데미지의 [spin]0[1,100]퍼센트를 흡수해서 체력으로 만든다.",
	eIP_ABSORB_TERRA,"대지 속성 데미지의 [spin]0[1,100]퍼센트를 흡수해서 체력으로 만든다.",
	eIP_ABSORB_LIGHT,"빛 속성 데미지의 [spin]0[1,100]퍼센트를 흡수해서 체력으로 만든다.",
	eIP_ABSORB_DARK,"어둠 속성 데미지의 [spin]0[1,100]퍼센트를 흡수해서 체력으로 만든다.",
	eIP_ABSORB_MAGIC,"모든 마법 속성 데미지의 [spin]0[1,100]퍼센트를 흡수해서 체력으로 만든다.",

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	각종 저항, 면역
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	eIP_RESISTANCE_FREEZE,"프리즈 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_COLD,"콜드 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_STUN,"스턴 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_CONFUSE,"혼란 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_CHARMING,"차밍 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_STONE,"석화 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_INSTANCE_KILL,"즉사 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_POISON,"중독 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_SLEEP,"잠 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_STRANGE_STATUS,"상태이상 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_DECLINE_STATUS,"저하계 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_CURSE_STATUS,"저주계 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_ALL_BAD_STATUS,"모든 이상 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_KNOCK_BACK,"녹백 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_CRITICAL_SHOT,"치명타 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",
	eIP_RESISTANCE_CRUSH_SHOT,"결정타 공격에 대한 저항이 [spin]0[1,100]퍼센트 증가한다.",

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	반격,스킬 관련
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	eIP_RETURN_DAMAGE,"받은 데미지의 [spin]0[1,1000]퍼센트를 공격자에게 돌려준다.",
	eIP_DAMAGE_TO_CP,"받은 데미지의 [spin]0[1,1000]퍼센트를 CP로 변환한다.",
	eIP_MAKE_FREEZE_ATTACKER,"공격을 받으면 [spin]0[1,100]퍼센트의 확률로 [spin]1[1,1000]초 동안 지속되는 프리즈 공격이 발동한다.",
	eIP_MAKE_COLD_ATTACKER,"공격을 받으면 [spin]0[1,100]퍼센트의 확률로 [spin]1[1,1000]초 동안 지속되는 콜드 공격이 발동한다.",
	eIP_CHARGE_SKILL,"공격을 받으면 [spin]0[1,100]퍼센트의 확률로 [spin]1[1,1000]초 동안 지속되는 콜드 공격이 발동한다.",
	eIP_JOB_SKILL_LEVEL,"[job]1의 스킬 레벨이 [spin]0[1,1000] 증가한다.",
	eIP_ALL_SKILL_LEVEL,"모든 스킬의 레벨이 [spin]0[1,1000] 증가한다.",

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	기타/특수 능력
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	eIP_FIRST_AID,"데미지의 [spin]0[1,1000]퍼센트를 치료한다.",
	eIP_CHANGE_WEAPON_SPEED,"무기 교환 속도를 [spin]0[1,1000]퍼센트 증가 시킨다.",
	eIP_REGENERATION_HP,"체력이 초당 [spin]0[1,1000]/10씩 회복된다.",
	eIP_RESURRECTION_CHANCE,"전투 불능 상태가 되면 [spin]0[1,1000]퍼센트의 확률로 부활한다.",
	eIP_BLURRED,"정지시 흐릿한 상태가 된다.",
	eIP_INVISIBLE,"투명 상태가 된다.(액션을 취할때만 투명 상태가 풀린다.)",
	eIP_LEVITATE,"공중 부양 상태가 된다.",
	eIP_MAGIC_BULLET,"탄환이 없으면 마법 탄환을 발사한다.",
	eIP_INFINITY_BULLET,"탄환을 무제한 사용할 수 있다.",
	eIP_NOT_AIMED,"공격하기 전에는 공격을 당하지 않는다.",
	eIP_INCREASE_RARE_ITEM_DROP_CHANCE,"마법 아이템 드랍 확률이 [spin]0[1,1000]퍼센트 증가한다.",
	eIP_INCREASE_UNIQUE_ITEM_DROP_CHANCE,"유니크 아이템 드랍 확률이 [spin]0[1,1000]퍼센트 증가한다.",
	eIP_DECREASE_RELOAD_TIME,"벨트에 아이템을 채우는 시간이 [spin]0[1,1000]퍼센트 감소한다.",
	eIP_AUTO_RELOAD,"벨트에 아이템이 자동으로 채워진다.",
	eIP_DISABLE,	"아이템을 사용할 수 없다.",
	eIP_CAN_NOT_STRIP,	"착용 후 탈의가 불가능하다.",
	eIP_CAN_NOT_ADD_PREFIX,	"추가 인챈트가 불가능하다.",

	eIP_UPGRADE_FIRE_DAMAGE_ATTACK, "불 공격 1단계 상승",
	eIP_UPGRADE_WATER_DAMAGE_ATTACK, "물 공격 1단계 상승",
	eIP_UPGRADE_WIND_DAMAGE_ATTACK, "바람 공격 1단계 상승",
	eIP_UPGRADE_EARTH_DAMAGE_ATTACK, "대지 공격 1단계 상승",
	eIP_UPGRADE_LIGHT_DAMAGE_ATTACK, "빛 공격 1단계 상승",
	eIP_UPGRADE_DARK_DAMAGE_ATTACK, "어둠 공격 1단계 상승",

	eIP_CONTROL_SKILL_LEVEL_BY_DIFFICULTY	, "스킬 난이도 [spin]0[1,5] 레벨 이하 스킬들 레벨이 [spin]1[1,100] 증가 한다.",
	eIP_INCREASE_SUMMON_BEAST_STATUS		, "소환수의 모든 스탯이 [spin]0[1,255] 증가",
	eIP_INCREASE_GAIN_EXP_FOR_PET			, "[spin]0[1,255]％의 펫 경험치 보너스",
	eIP_IS_SHOP_SPECIAL_ITEM				, "사용기간",

	eIP_WEAKNING_TARGET_FIRE_RESISTANCE			,	"타겟의 불 저항을 [spin]0[1,255]％ 약화 시킨다.",
	eIP_WEAKNING_TARGET_WATER_RESISTANCE		,	"타겟의 물 저항을 [spin]0[1,255]％ 약화 시킨다.",
	eIP_WEAKNING_TARGET_WIND_RESISTANCE			,	"타겟의 바람 저항을 [spin]0[1,255]％ 약화 시킨다.",
	eIP_WEAKNING_TARGET_EARTH_RESISTANCE		,	"타겟의 대지 저항을 [spin]0[1,255]％ 약화 시킨다.",
	eIP_WEAKNING_TARGET_LIGHT_RESISTANCE		,	"타겟의 빛 저항을 [spin]0[1,255]％ 약화 시킨다.",
	eIP_WEAKNING_TARGET_DARK_RESISTANCE			,	"타겟의 어둠 저항을 [spin]0[1,255]％ 약화 시킨다.",
	eIP_WEAKNING_TARGET_MAGIC_RESISTANCE		,	"타겟의 마법 저항을 [spin]0[1,255]％ 약화 시킨다.",
	eIP_STRENGTHENING_FIRE_DAMAGE				,	"불 속성 공격력을 [spin]0[1,300]％ 강화 시킨다.",
	eIP_STRENGTHENING_WATER_DAMAGE				,	"물 속성 공격력을 [spin]0[1,300]％ 강화 시킨다.",
	eIP_STRENGTHENING_WIND_DAMAGE				,	"바람 속성 공격력을 [spin]0[1,300]％ 강화 시킨다.",
	eIP_STRENGTHENING_EARTH_DAMAGE				,	"대지 속성 공격력을 [spin]0[1,300]％ 강화 시킨다.",
	eIP_STRENGTHENING_LIGHT_DAMAGE				,	"빛 속성 공격력을 [spin]0[1,300]％ 강화 시킨다.",
	eIP_STRENGTHENING_DARK_DAMAGE				,	"어둠 속성 공격력을 [spin]0[1,300]％ 강화 시킨다.",
	eIP_STRENGTHENING_MAGIC_DAMAGE				,	"마법 공격력을 [spin]0[1,300]％ 강화 시킨다.",

	eIP_DECREASE_STRENGTH,"착용자의 힘이 [spin]0[1,255] 감소한다.",
	eIP_DECREASE_AGILITY,"착용자의 민첩성이 [spin]0[1,255] 감소한다.",
	eIP_DECREASE_CONSTITUTION,"착용자의 건강이 [spin]0[1,255] 감소한다.",
	eIP_DECREASE_WISDOM,"착용자의 지혜가 [spin]0[1,255] 감소한다.",
	eIP_DECREASE_INTELIGENCE,"착용자의 지식이 [spin]0[1,255] 감소한다.",
	eIP_DECREASE_CHARISMA,"착용자의 카리스마가 [spin]0[1,255] 감소한다.",
	eIP_DECREASE_LUCK,"착용자의 운이 [spin]0[1,255] 감소한다.",

	eIP_DECLINE_MOVE_SPEED,"이동 속도가 [spin]0[1,255]퍼센트 감소한다.",
	eIP_DECLINE_ACTION_SPEED,"공격 속도가 [spin]0[1,255]퍼센트 감소한다.",

	eIP_ADD_DAMAGE_TO_UNDEAD					,	"언데드형 캐릭터에게 추가로 [spin]0[1,300]퍼센트의 대미지를 준다.",
	eIP_ADD_DAMAGE_TO_DEMON						,	"악마형 캐릭터에게 추가로 [spin]0[1,300]퍼센트의 대미지를 준다.",
	eIP_ADD_DAMAGE_TO_HUMAN						,	"인간형 캐릭터에게 추가로 [spin]0[1,300]퍼센트의 대미지를 준다.",
	eIP_ADD_DAMAGE_TO_ANIMAL					,	"동물형 캐릭터에게 추가로 [spin]0[1,300]퍼센트의 대미지를 준다.",
	eIP_ADD_DAMAGE_TO_HOLY_ANIMAL				,	"신수형 캐릭터에게 추가로 [spin]0[1,300]퍼센트의 대미지를 준다.",
	eIP_POTION_HEAL_SPEED2						,	"물약 회복 속도 +[spin]0[1,255]퍼센트",

	eIP_DECLINE_DEFENSE_PERCENTAGE				,"방어력이 [spin]0[1,255]퍼센트 감소한다.",
	eIP_DECLINE_DEFENSE							,"방어력이 [spin]0[1,255] 감소한다.",
	eIP_DECLINE_HIT_RATE						,"명중률 [spin]0[1,255]퍼센트 감소",
	eIP_DECLINE_DODGE_RATE						,"회피율 [spin]0[1,255]퍼센트 감소",
// VV 09.08.07  유니크 옵션  마법추뎀DX+_+
	eIP_FIRE_DAMAGE_DX,"대상에게 [spin]0[1,20000]~[spin]1[1,20000]의 불 속성 데미지를 입힌다.",
	eIP_WATER_DAMAGE_DX,"대상에게 [spin]0[1,20000]~[spin]1[1,20000]의 물 속성 데미지를 입히고, [spin]2[1,1000]Frame동안 얼린다.",
	eIP_WIND_DAMAGE_DX,"대상에게 [spin]0[1,20000]~[spin]1[1,20000]의 바람 속성 데미지를 입히고, [spin]2[1,1000]Frame동안 스턴 시킨다.",
	eIP_TERRA_DAMAGE_DX,"대상에게 [spin]2[1,1000]초 동안, 초당 [spin]0[1,20000]~[spin]1[1,20000]의 중독 데미지를 입힌다.",
	eIP_LIGHT_DAMAGE_DX,"대상에게 [spin]0[1,20000]~[spin]1[1,20000]의 빛 속성 데미지를 입히고, [spin]2[1,1000]초 동안 명중,회피 저하상태로 만든다.",
	eIP_DARK_DAMAGE_DX,"대상에게 [spin]0[1,20000]~[spin]1[1,20000]의 어둠 속성 데미지를 입히고, [spin]2[1,1000]초 동안 저주를 건다.",
	// AA 09.08.07  유니크 옵션  마법추뎀DX+_+
	eIP_BUFF_SKILL_CASTING, "[owner_skill]0 스킬을 [spin]1[1,200]레벨, 관련스텟 [spin]2[1,20000]로 캐스팅한다.(지속시간 [spin]3[0,20000]초)",	// 버프스킬받기!
	eIP_INCREASE_ALL_STATUS, "착용자의 모든 능력치 [spin]0[1,20000]이 증가한다.",	// 모든능력치 증가
	eIP_ACL_BUFF_SKILL_CASTING, "[owner_skill]0 스킬을 각인 레벨로 캐스팅한다.",	// 버프스킬받기!
	0xffff,"끝"
};

char	*cPREFIX_EFFECT::s_strPrefixEffectCategoryList[]	=
{	"기본 능력치 조절","공격력,방어력,체력,CP","각종 확률 조절","속도/시간 보정","공격 추가/공격 부가 효과",
	"마법 저항","각종 저항및 면역","반격/스킬","인챈트","원소 대미지 보정","기타/특수 능력","end"
};

int
cPREFIX_EFFECT::s_aPrefixEffectListByCategory[20][60]	=
{
//////////////////////////////////////////////////
//	기본 능력치 조절
//////////////////////////////////////////////////
	{
		eIP_ADD_STRENGTH,
		eIP_ADD_AGILITY,	
		eIP_ADD_CONSTITUTION,	
		eIP_ADD_WISDOM,	
		eIP_ADD_INTELIGENCE,	
		eIP_ADD_CHARISMA,	
		eIP_ADD_LUCK,	
		eIP_DECREASE_STRENGTH,
		eIP_DECREASE_AGILITY,	
		eIP_DECREASE_CONSTITUTION,	
		eIP_DECREASE_WISDOM,	
		eIP_DECREASE_INTELIGENCE,	
		eIP_DECREASE_CHARISMA,	
		eIP_DECREASE_LUCK,	
		eIP_FIXING_STRENGTH,	
		eIP_FIXING_AGILITY,	
		eIP_FIXING_CONSTITUTION,	
		eIP_FIXING_WISDOM,	
		eIP_FIXING_INTELLIGENCE,	
		eIP_FIXING_CHARISMA,	
		eIP_FIXING_LUCK,	
		eIP_ADD_STRENGTH_BY_LEVEL,	
		eIP_ADD_AGILITY_BY_LEVEL,	
		eIP_ADD_CONSTITUTION_BY_LEVEL,	
		eIP_ADD_WISDOM_BY_LEVEL,	
		eIP_ADD_INTELIGENCE_BY_LEVEL,	
		eIP_ADD_CHARISMA_BY_LEVEL,	
		eIP_ADD_LUCK_BY_LEVEL,	
		eIP_INCREASE_ALL_STATUS,	// 모든 능력치 증가
		0xffff,
	},
//////////////////////////////////////////////////
//	공격력,방어력,체력,CP
//////////////////////////////////////////////////
	{
		eIP_ADD_DAMAGE_PERCENTAGE,	
		eIP_ADD_MIN_DAMAGE,	
		eIP_ADD_MAX_DAMAGE,	
		eIP_ADD_DEFENSE_PERCENTAGE,	
		eIP_ADD_DEFENSE,	
		eIP_DECLINE_DEFENSE_PERCENTAGE				,
		eIP_DECLINE_DEFENSE							,

		eIP_ADD_MAX_HP_PERCENTAGE,	
		eIP_ADD_MAX_HP,
		eIP_ADD_MAX_CP_PERCENTAGE,
		eIP_ADD_MAX_CP,
		eIP_LOW_CP,	
		0xffff,
	},
//////////////////////////////////////////////////
//	각종 확률 보정
//////////////////////////////////////////////////
	{
		eIP_CORRECT_HIT_RATE,	
		eIP_CORRECT_DODGE_RATE,	
		eIP_DECLINE_HIT_RATE	,
		eIP_DECLINE_DODGE_RATE	,

		eIP_SHIELD_BLOCK_RATE,	
		eIP_CRITICAL_HIT,	
		eIP_CRUSH_SHOT,	
		eIP_IGNORE_TARGET_DODGE_ABILITY,	
		eIP_IGNORE_ATTACKER_CORRECT_HIT_VALUE,	
		eIP_PERFECT_HIT,
		eIP_INCREASE_HARD_BLOW,
		0xffff,
	},
//////////////////////////////////////////////////////
//	속도/시간 보정
//////////////////////////////////////////////////////
	{
		eIP_BLOCKING_SPEED,
		eIP_MOVE_SPEED,
		eIP_ACTION_SPEED,
		eIP_DECLINE_MOVE_SPEED,
		eIP_DECLINE_ACTION_SPEED,
		eIP_POTION_HEAL_SPEED,
		eIP_POTION_HEAL_SPEED2,
		eIP_ADD_CONCENTRATION,
		0xffff,
	},
//////////////////////////////////////////////////////////////////////////////
//	공격 성공시 추가 효과
//////////////////////////////////////////////////////////////////////////////
	{
		eIP_FIRE_DAMAGE,	
		eIP_WATER_DAMAGE,	
		eIP_WIND_DAMAGE,	
		eIP_TERRA_DAMAGE,	
		eIP_LIGHT_DAMAGE,	
		eIP_DARK_DAMAGE,
		// VV 09.08.07    마법추뎀DX+_+
		eIP_FIRE_DAMAGE_DX,	
		eIP_WATER_DAMAGE_DX,	
		eIP_WIND_DAMAGE_DX,	
		eIP_TERRA_DAMAGE_DX,	
		eIP_LIGHT_DAMAGE_DX,	
		eIP_DARK_DAMAGE_DX,
		// AA 09.08.07    마법추뎀DX+_+
		eIP_STEAL_HP,
		eIP_BONUS_CP,	
		eIP_FLEE,	
		eIP_KNOCK_OUT,	
		eIP_INSTANT_KILL,	
		eIP_WEAPON_BREAKER,	
		eIP_ARMOR_BREAKER,	
		eIP_MAKE_COLD,	
		eIP_MAKE_FREEZE,	
		eIP_MAKE_BLIND,	
		eIP_MAKE_STUN,	
		eIP_MAKE_STONE,	
		eIP_MAKE_CONFUSE,	
		eIP_MAKE_SLEEP,	
		eIP_MAKE_CHARMING,	
		eIP_MAKE_FIRST_TARGET,	
		eIP_MAKE_AI_DOWN ,	
		eIP_MAKE_BERSERK,
		eIP_ADD_DAMAGE_TO_UNDEAD					,
		eIP_ADD_DAMAGE_TO_DEMON						,
		eIP_ADD_DAMAGE_TO_HUMAN						,
		eIP_ADD_DAMAGE_TO_ANIMAL					,
		eIP_ADD_DAMAGE_TO_HOLY_ANIMAL				,
		
		0xffff,
	},
//////////////////////////////////////////////////////
//	마법 공격에 대한 저항
//////////////////////////////////////////////////////
	{
		eIP_RESISTANCE_FIRE,	
		eIP_RESISTANCE_WATER,	
		eIP_RESISTANCE_WIND,	
		eIP_RESISTANCE_TERRA,	
		eIP_RESISTANCE_LIGHT,	
		eIP_RESISTANCE_DARK,	
		eIP_RESISTANCE_MAGIC,	
		eIP_ABSORB_FIRE,	
		eIP_ABSORB_WATER,	
		eIP_ABSORB_WIND,	
		eIP_ABSORB_TERRA,	
		eIP_ABSORB_LIGHT,	
		eIP_ABSORB_DARK,	
		eIP_ABSORB_MAGIC,	
		0xffff,
	},
//////////////////////////////////////////////////////
//	각종 저항및 면역
//////////////////////////////////////////////////////
	{
		eIP_RESISTANCE_FREEZE,	
		eIP_RESISTANCE_COLD,	
		eIP_RESISTANCE_STUN,	
		eIP_RESISTANCE_CONFUSE,	
		eIP_RESISTANCE_CHARMING,	
		eIP_RESISTANCE_STONE,	
		eIP_RESISTANCE_INSTANCE_KILL,	
		eIP_RESISTANCE_POISON,	
		eIP_RESISTANCE_SLEEP,	
		eIP_RESISTANCE_STRANGE_STATUS,	
		eIP_RESISTANCE_DECLINE_STATUS,	
		eIP_RESISTANCE_CURSE_STATUS,	
		eIP_RESISTANCE_ALL_BAD_STATUS,	
		eIP_RESISTANCE_KNOCK_BACK,	
		eIP_RESISTANCE_CRITICAL_SHOT,	
		eIP_RESISTANCE_CRUSH_SHOT,	
		0xffff,
	},
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	반격,스킬 관련
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		eIP_RETURN_DAMAGE,	
		eIP_DAMAGE_TO_CP,	
		eIP_MAKE_FREEZE_ATTACKER,	
		eIP_MAKE_COLD_ATTACKER,	
//		eIP_CHARGE_SKILL,
		eIP_CONTROL_SKILL_LEVEL_BY_DIFFICULTY	,
		eIP_JOB_SKILL_LEVEL,	
		eIP_ALL_SKILL_LEVEL,	

		0xffff,
	},
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	인챈트 관련
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		eIP_UPGRADE_FIRE_DAMAGE_ATTACK, 
		eIP_UPGRADE_WATER_DAMAGE_ATTACK,
		eIP_UPGRADE_WIND_DAMAGE_ATTACK, 
		eIP_UPGRADE_EARTH_DAMAGE_ATTACK,
		eIP_UPGRADE_LIGHT_DAMAGE_ATTACK,
		eIP_UPGRADE_DARK_DAMAGE_ATTACK, 
		0xffff,
	},
//////////////////////////////////////////////////////
//	원소 대미지 보정
//////////////////////////////////////////////////////
	{
		eIP_WEAKNING_TARGET_FIRE_RESISTANCE			,	//	"타겟의 불 저항을 [spin]0[1,255]％ 약화 시킨다.",
		eIP_WEAKNING_TARGET_WATER_RESISTANCE		,	//	"타겟의 물 저항을 [spin]0[1,255]％ 약화 시킨다.",
		eIP_WEAKNING_TARGET_WIND_RESISTANCE			,	//	"타겟의 바람 저항을 [spin]0[1,255]％ 약화 시킨다.",
		eIP_WEAKNING_TARGET_EARTH_RESISTANCE		,	//	"타겟의 대지 저항을 [spin]0[1,255]％ 약화 시킨다.",
		eIP_WEAKNING_TARGET_LIGHT_RESISTANCE		,	//	"타겟의 빛 저항을 [spin]0[1,255]％ 약화 시킨다.",
		eIP_WEAKNING_TARGET_DARK_RESISTANCE			,	//	"타겟의 어둠 저항을 [spin]0[1,255]％ 약화 시킨다.",
		eIP_WEAKNING_TARGET_MAGIC_RESISTANCE		,	//	"타겟의 마법 저항을 [spin]0[1,255]％ 약화 시킨다.",
		eIP_STRENGTHENING_FIRE_DAMAGE				,	//	"불 속성 공격력을 [spin]0[1,255]％ 강화 시킨다.",
		eIP_STRENGTHENING_WATER_DAMAGE				,	//	"물 속성 공격력을 [spin]0[1,255]％ 강화 시킨다.",
		eIP_STRENGTHENING_WIND_DAMAGE				,	//	"바람 속성 공격력을 [spin]0[1,255]％ 강화 시킨다.",
		eIP_STRENGTHENING_EARTH_DAMAGE				,	//	"대지 속성 공격력을 [spin]0[1,255]％ 강화 시킨다.",
		eIP_STRENGTHENING_LIGHT_DAMAGE				,	//	"빛 속성 공격력을 [spin]0[1,255]％ 강화 시킨다.",
		eIP_STRENGTHENING_DARK_DAMAGE				,	//	"어둠 속성 공격력을 [spin]0[1,255]％ 강화 시킨다.",
		eIP_STRENGTHENING_MAGIC_DAMAGE				,	//	"마법 공격력을 [spin]0[1,255]％ 강화 시킨다.",
		0xffff,
	},
//////////////////////////////////////////////////////
//	기타/특수 능력
//////////////////////////////////////////////////////
	{
		eIP_FIRST_AID,
		eIP_CHANGE_WEAPON_SPEED,	
		eIP_REGENERATION_HP,	
		eIP_RESURRECTION_CHANCE,	
		eIP_BLURRED,	
		eIP_INVISIBLE,	
		eIP_LEVITATE,
		eIP_MAGIC_BULLET,
		eIP_INFINITY_BULLET,	
		eIP_NOT_AIMED,	
		eIP_INCREASE_RARE_ITEM_DROP_CHANCE,	
		eIP_INCREASE_UNIQUE_ITEM_DROP_CHANCE,
		eIP_DECREASE_RELOAD_TIME,
		eIP_AUTO_RELOAD,
		eIP_INCREASE_SUMMON_BEAST_STATUS		,
		eIP_INCREASE_GAIN_EXP_FOR_PET			,
		eIP_IS_SHOP_SPECIAL_ITEM,
		eIP_BUFF_SKILL_CASTING,				// 버프스킬케스팅~ 09.10.12
		eIP_ACL_BUFF_SKILL_CASTING,				// 각인버프스킬케스팅~
		0xffff,
	}
};

//
//	아이템 효과들의 인덱스를 얻어 온다.(시리얼 참조)
int
cPREFIX_EFFECT::GetIndex(int serial)
{
	int	iIndex	=	0;

	while(1)
	{
		if	(s_aHyperText[iIndex].m_iSerial	==	0xffff)	break;
		if	(s_aHyperText[iIndex].m_iSerial	==	serial)	return	iIndex;

		iIndex++;
	}
	return	0xffff;
}	//	cPREFIX_EFFECT::GetIndex(int serial)


//
//	_iEffect가 속해 있는 카테고리를 얻어 온다.
int
cPREFIX_EFFECT::GetCategory(int _iEffect)
{
	int	iCategory	=	0;

	while(iCategory	<=	eEE_ETC_SPECIAL_ABILITY)
	{
		int	iEffect	=	0;

		while(s_aPrefixEffectListByCategory[iCategory][iEffect]		!=	0xffff	)
		{
			if (s_aPrefixEffectListByCategory[iCategory][iEffect]	==	_iEffect)	return	iCategory;
			iEffect++;
		}

		iCategory++;
	}

	return	0xffff;
}	//	cPREFIX_EFFECT::GetCategory(int _iEffect)

//
//	아이템 효과를 얻어 온다.(시리얼 참조)
cPREFIX_EFFECT_REFERENCE*
cPREFIX_EFFECT::Get(int serial)
{
	int	iIndex	=	0;

	while(1)
	{
		if	(s_aHyperText[iIndex].m_iSerial	==	0xffff)	break;
		if	(s_aHyperText[iIndex].m_iSerial	==	serial)	return	&s_aHyperText[iIndex];

		iIndex++;
	}

	return	NULL;
}	//	cPREFIX_EFFECT::Get(int serial)

//
//	효과를 추가 시킨다.
BOOL
cPREFIX_EFFECT::AddEffect(HWND hwnd,cITEM_EFFECT *lpEffect)
{
	if (lpEffect)
	{	memcpy(&s_Effect,lpEffect,sizeof(s_Effect));

		int	iCategory	=	GetCategory(s_Effect.m_wEffect);

		if (iCategory	!=	0xffff)
		{	s_iSelectEffectCategory	=	iCategory;
			s_iSelectEffect			=	s_Effect.m_wEffect;
		}
		else
		{
			memset(&s_Effect,0,sizeof(s_Effect));
			s_Effect.m_wEffect	=	s_iSelectEffect;
		}
	}
	else
	{	memset(&s_Effect,0,sizeof(s_Effect));
		s_Effect.m_wEffect	=	s_iSelectEffect;
	}

	int	result	=	DialogBox( MAIN.hINST,"SET_ITEM_EFFECT",hwnd,(DLGPROC)s_procItemEffect);

	return	result;
}	//	cPREFIX_EFFECT::AddEffect(HWND hwnd)


//////////////////////////////////////////////////////////////////////////////////////////////
//	아이템 효과 설정
//////////////////////////////////////////////////////////////////////////////////////////////

//
//	아이템 효과 대화박스 초기화
void
cPREFIX_EFFECT::InitItemEffectDlg(HWND hDlg)
{
	s_hDlg	=	hDlg;

	HWND	hEffectList		=	GetDlgItem(hDlg,IDC_CONTENTS		);
	HWND	hEffectCategory	=	GetDlgItem(hDlg,IDC_ITEM_EFFECT_KIND);
	int		iCount		=	1;

	memset(s_strFilter,0,sizeof(s_strFilter));

	HWND	hTree			=	GetDlgItem(hDlg,IDC_TREE);

	s_treeView.Init(hTree,MAIN.hINST,NULL);

	FillTree();

	WriteItemEffectText(hDlg,&s_Effect);

}	//	cPREFIX_EFFECT::InitItemEffectDlg(HWND hDlg)

//
//	_iEffect가 속해 있는 카테고리를 얻어 온다.
int
cPREFIX_EFFECT::GetEffectCountByCategory(int _iCategory)
{
	int	iCount	=	0;

	for (int iPrefix=0;;iPrefix++)
	{
		int		iSerial		=	s_aPrefixEffectListByCategory[_iCategory][iPrefix];

		if	(iSerial	==	0xffff)
			break;

		char	*lpComment	=	g_aItemPrefixType[iSerial].m_strName;

		if	(s_strFilter[0] && strstr(lpComment,s_strFilter)	==	NULL)
			continue;

		iCount++;
	}

	return	iCount;
}

void
cPREFIX_EFFECT::FillTree()
{
	s_treeView.Clear();

	for (int iCategory=0;iCategory<=eEE_ETC_SPECIAL_ABILITY;iCategory++)
	{
		if	(s_strFilter[0] && GetEffectCountByCategory(iCategory)	==	0)
			continue;

		char	*lpstrCategory	=	s_strPrefixEffectCategoryList[iCategory];

		HTREEITEM	hParents	=	s_treeView.Add(lpstrCategory,-1);

		for (int iPrefix=0;;iPrefix++)
		{
			int		iSerial		=	s_aPrefixEffectListByCategory[iCategory][iPrefix];

			if	(iSerial	==	0xffff)
				break;

			char	*lpComment	=	g_aItemPrefixType[iSerial].m_strName;

			if	(s_strFilter[0] && strstr(lpComment,s_strFilter)	==	NULL)
				continue;

			int			iParam		=	(iCategory<<16)+iSerial;
			HTREEITEM	hCurrent	=	s_treeView.Add(hParents,lpComment,iParam);

			if	(iSerial	==	s_iSelectEffect)
				s_treeView.Select(hCurrent);
		}

		if	(iCategory	==	s_iSelectEffectCategory || s_strFilter[0])
			s_treeView.Expand(hParents);
	}
}

//
//	종성을 체크 한다
void
cPREFIX_EFFECT::CheckJongsung(char *str)
{
	s_isJongSung	=	FALSE;

	int		length	=	strlen(str);

	if (length < 2)	return;

	if ((BYTE)str[length-2]	>=	0xb0	&&	(BYTE)str[length-1]	>=	0xa1)
		if (IsJongsung(str+length-2))	s_isJongSung	=	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////
//	콤보 박스들..
//////////////////////////////////////////////////////////////////////////////////////


//
//	스핀이 있는 에디트 박스
int
cPREFIX_EFFECT::SpinEditBox(HWND hwnd,int x,int y,int select)
{
	int	width	=	70;
	x	+=	2;

	int	iMinValue	=	s_Reader.GetNumber();
	int	iMaxValue	=	s_Reader.GetNumber();

	s_Reader.FindCharOver(']');

	select	=	min(iMaxValue,max(iMinValue,select));

			if(iMaxValue	>	1000000000)	width	=	10;
	else	if(iMaxValue	>	100000000)	width	=	9;
	else	if(iMaxValue	>	10000000)	width	=	8;
	else	if(iMaxValue	>	1000000)	width	=	7;
	else	if(iMaxValue	>	100000)		width	=	6;
	else	if(iMaxValue	>	10000)		width	=	5;
	else									width	=	4;

	width	=	(width+4)*6;

	s_aSpinBox	[s_iSpinIndex].Init(hwnd,MAIN.hINST,IDC_ITEM_SPINBOX	+s_iSpinIndex	,_ms("%d",select),x,y-4,width,20,ES_NUMBER|WS_BORDER);
	s_aSpin		[s_iSpinIndex].Init(hwnd,MAIN.hINST,IDC_ITEM_SPIN		+s_iSpinIndex	,s_aSpinBox	[s_iSpinIndex].hWND,iMinValue,iMaxValue,select);

	s_iSpinIndex++;


	return	width+5;
}	//	cPREFIX_EFFECT::SpinEditBox(HWND hwnd,int x,int y,int select)

//
//	공격후 보너스로 줄 수 있는 기술들
int
cPREFIX_EFFECT::AttackBonusSkillComboBox(HWND hwnd,int x,int y,int kind,int select)
{
	return	0;
}	//	cPREFIX_EFFECT::AttackBonusSkillComboBox(HWND hwnd,int x,int y,int kind,int select)

//
//	공격 받은후 보복-_-?으로 날릴 수 있는 스킬들..
int
cPREFIX_EFFECT::RevengeSkillComboBox(HWND hwnd,int x,int y,int kind,int select)
{
	return	0;
}	//	cPREFIX_EFFECT::RevengeSkillComboBox(HWND hwnd,int x,int y,int kind,int select)

//
//	패시브 스킬들
int
cPREFIX_EFFECT::PassiveSkillComboBox(HWND hwnd,int x,int y,int kind,int select)
{
	return	0;
}	//	cPREFIX_EFFECT::PassiveSkillComboBox(HWND hwnd,int x,int y,int kind,int select)

//
//	모든 스킬
int
cPREFIX_EFFECT::AllSkillComboBox(HWND hwnd,int x,int y,int kind,int select)
{		// 마지막 수정일 : 09.10.13
	
	int	iWidth	=	220;

	x	+=	2;
	
	s_aCombo[s_iComboIndex].Init(hwnd,MAIN.hINST,IDC_ITEM_COMBO+s_iComboIndex,x,y-4,iWidth+20,800,CBS_DROPDOWNLIST);

	for (int i=0;i<dMAX_SKILL;i++)
	{
		if (g_aSkill[i].m_wSerial	==	0xffff)
			continue;


		int	iJob	=	i/25;
		if (iJob	<	dPLAYER_JOB_COUNT)
		{

			char	strSkillName[128];

			sprintf(strSkillName,"[%s] %s",g_aDefaultJobName[iJob],g_aSkill[i].m_strName);
			s_aCombo[s_iComboIndex].Add(strSkillName,i);
		}
		else
			s_aCombo[s_iComboIndex].Add(g_aSkill[i].m_strName,i);
	}

	if (s_aCombo[s_iComboIndex].SelectByData(select) == CB_ERR)
	{
		s_aCombo[s_iComboIndex].Select(0);
		select	=	0;
	}

	CheckJongsung(g_aSkill[select].m_strName);

	return	iWidth+24;
}	

//
//	기타등등이  출력 되는 콤보 박스
int
cPREFIX_EFFECT::CustomComboBox(HWND hwnd,int x,int y,int kind,int select)
{
	int	iCounter=	0;
	int	iWidth	=	0;

	while(1)
	{
		if	(strcmp(s_strComboStr[kind][iCounter],"end")	==	0)
			break;

		int	width	=	strlen(s_strComboStr[kind][iCounter])*6 + 30;

		if	(width > iWidth)
			iWidth	=	width;

		iCounter++;
	}

	x	+=	2;

	s_aCombo[s_iComboIndex].Init(hwnd,MAIN.hINST,IDC_ITEM_COMBO	+	s_iComboIndex,x,y-4,iWidth,400,CBS_DROPDOWNLIST);

	iCounter	=	0;

	while(1)
	{
		if	(strcmp(s_strComboStr[kind][iCounter],"end")	==	0)
			break;

		int	index	=	s_aCombo[s_iComboIndex].Add(s_strComboStr[kind][iCounter]);

		s_aCombo[s_iComboIndex].SetData(index,iCounter);

		iCounter++;
	}

	if	(select >= iCounter	||	select	<	0)
		select	=	0;

	s_aCombo[s_iComboIndex].Select(select);

	CheckJongsung(s_strComboStr[kind][select]);

	s_iComboIndex++;

	return	iWidth+4;
}	//	cKARMA_WND::CustomComboBox(HWND hwnd,int x,int y,int kind,int select)

//
//	
char*
cPREFIX_EFFECT::GetCustomComboText(int kind,int data)
{
	CheckJongsung(s_strComboStr[kind][data]);

	return	s_strComboStr[kind][data];
}	//	cPREFIX_EFFECT::GetCustomComboText(int kind,int data)

//
//	음-_-
char*
cPREFIX_EFFECT::GetSpinText(int data)
{
	static	char	strNumber[12];

	strcpy(strNumber,_ms("%d",data));

	CheckJongsung(strNumber);

	return	strNumber;
}	//	cPREFIX_EFFECT::GetSpinText(int data)

//
//	은는이가을를 을 선택
void
cPREFIX_EFFECT::TransCode(char *lpStream)
{
	char	*strCheckKarmaJongsung[]	=	{"은 ","는 ","이 ","가 ","을 ","를 ","end"};

	if	(!lpStream)	return;
	int		length	=	strlen(lpStream);

	if (length < 3)	return;

	char	word[4];

	word[0]	=	lpStream[0];
	word[1]	=	lpStream[1];
	word[2]	=	lpStream[2];
	word[3]	=	0;

	for (int i=0;i<6;i++)
		if (strcmp(strCheckKarmaJongsung[i],word)	==	0)
		{
			if (s_isJongSung)	//	종성이 있는데..
			{
				if (i%2)		//	종성이 없을때 오는 문자다.
				{
					lpStream[0]	=	strCheckKarmaJongsung[i-1][0];
					lpStream[1]	=	strCheckKarmaJongsung[i-1][1];
				}
			}
			else
			{
				if (i%2 == 0)	//	종성이 있을때 오는 문자다.
				{
					lpStream[0]	=	strCheckKarmaJongsung[i+1][0];
					lpStream[1]	=	strCheckKarmaJongsung[i+1][1];
				}
			}

			return;
		}
}	//	cPREFIX_EFFECT::TransCode(char *lpStream)

//
//	아이템 효과를 읽는다.
BOOL
cPREFIX_EFFECT::ReadEffectData(HWND hwnd,cITEM_EFFECT *lpEffect)
{ // 마지막 수정일 : 09.10.13
	int		iIndex	=	GetIndex(lpEffect->m_wEffect);
	char	*lpText	=	s_aHyperText[iIndex].m_strReferenceText;
	short	*lpData	=	lpEffect->m_aValue;

	char	*lpStream,*lpKeyWord;
	int		iCode;

	s_iComboIndex		=	0;
	s_iSpinIndex		=	0;
	s_iSpinBoxIndex		=	0;

	int	iValueIndex		=	0;

	s_Reader.UploadBuffer(lpText,s_aKeyword);

	while(1)
	{
		if (s_Reader.m_isDone	)	break;		//	다 읽어 들였다.

		lpStream	=	s_Reader.GetStream('[');

		if (iValueIndex	>=	0	)	TransCode(lpStream);
		if (s_Reader.m_isDone	)	break;		//	다 읽어 들였다.

		//	콤보 박스나 머 그런거..
		lpKeyWord	=	s_Reader.GetStream(']');
		iValueIndex	=	s_Reader.GetNumber();

		iCode		=	s_Reader.AnalCode(lpKeyWord);


		switch(iCode)
		{
			case	dIKW_PLUS_MINUS_SET		:
			case	dIKW_PLUS_MINUS			:
			case	dIKW_MIN_MAX_DAMAGE		:
			case	dIKW_BASE_POWER			:
			case	dIKW_CHARGEABLE_SKILL	:
			case	dIKW_ATTACK_BONUS_SKILL	:
			case	dIKW_REVENGE_SKILL		:
			case	dIKW_PASSIVE_SKILL		:
			case	dIKW_ALL_SKILL			:
			case	dIKW_AP_DP				:
			case	dIKW_MAGIC_ATTRIBUTE	:
			case	dIKW_JOB				:
				lpData[iValueIndex]	=	s_aCombo[s_iComboIndex].GetData();
				++s_iComboIndex;
				break;

			case	dIKW_SPIN		:
			{
				int	value		=	s_aSpinBox[s_iSpinIndex].GetInt();

				int	iMinValue	=	s_Reader.GetNumber();
				int	iMaxValue	=	s_Reader.GetNumber();

				value			=	max(min(value,iMaxValue),iMinValue);

				lpData[iValueIndex]	=	value;
				s_iSpinIndex++;

				break;
			}

			default					:
//				ERRMSG("error","what is it?? [%s]",lpKeyWord);
				break;
		}

	}

	return	TRUE;
}	//	cPREFIX_EFFECT::ReadEffectData(HWND hwnd,cITEM_EFFECT *lpEffect)

//
//	아이템 부가 효과를텍스트를 그린다.
BOOL
cPREFIX_EFFECT::WriteItemEffectText(HWND hwnd,cITEM_EFFECT *lpEffect)
{ // 마지막 수정일 : 09.10.13
	int		iIndex		=	GetIndex(lpEffect->m_wEffect);
	char	*lpText		=	s_aHyperText[iIndex].m_strReferenceText;
	short	*lpValue	=	lpEffect->m_aValue;


	int			x	=	10;
	int			y	=	12;

	char		*lpStream,*lpKeyWord;
	int			iCode;

	s_iComboIndex	=	0;
	s_iSpinIndex	=	0;
	s_iSpinBoxIndex	=	0;
	s_iStreamIndex	=	0;

	int	iValueIndex	=	0;

	s_Reader.UploadBuffer(lpText,s_aKeyword);

	EnableWindow(GetDlgItem(hwnd,IDOK),TRUE);

	for (int i=0;i<10;i++)
	{
		s_aCombo[i]		.Close();
		s_aSpinBox[i]	.Close();
		s_aSpin[i]		.Close();
		s_aStream[i]	.Close();
	}
	
	while(1)
	{
		if (s_Reader.m_isDone	)	break;		//	다 읽어 들였다.
		
		lpStream	=	s_Reader.GetStream('[');
		
		
		if (iValueIndex	>=0		)
			TransCode(lpStream);
		
		//	스태틱 문장 출력
		if (lpStream			)
			x	+=	s_aStream[s_iStreamIndex++].Init(hwnd,MAIN.hINST,lpStream,x,y,0,12);
		
		if (s_Reader.m_isDone	)
			break;		//	다 읽어 들였다.
		
		//	콤보 박스나 머 그런거..
		lpKeyWord	=	s_Reader.GetStream(']');
		iCode		=	s_Reader.AnalCode(lpKeyWord);
		iValueIndex	=	s_Reader.GetNumber();
		
		switch(iCode)
		{
			case	dIKW_PLUS_MINUS_SET		:
			case	dIKW_PLUS_MINUS			:
			case	dIKW_MIN_MAX_DAMAGE		:
			case	dIKW_BASE_POWER			:
			case	dIKW_AP_DP				:
			case	dIKW_MAGIC_ATTRIBUTE	:
			case	dIKW_JOB				:
				x	+=	CustomComboBox(hwnd,x,y,iCode,lpValue[iValueIndex]);
				break;

			case	dIKW_CHARGEABLE_SKILL	:
				break;
			case	dIKW_ATTACK_BONUS_SKILL	:
				break;
			case	dIKW_REVENGE_SKILL		:
				break;
			case	dIKW_PASSIVE_SKILL		:
				break;
			case	dIKW_ALL_SKILL			:
				x	+=	AllSkillComboBox(hwnd,x,y,iCode,lpValue[iValueIndex]);
				break;

//	문장 선택형 콤보 박스들

			case	dIKW_SPIN				:
			{
				x	+=	SpinEditBox(hwnd,x,y,lpValue[iValueIndex]);
				break;
			}


			default							:
				ERRMSG("error","what is it?? [%s]",lpKeyWord);
				break;
		}
	}

	return	TRUE;
}	//	cPREFIX_EFFECT::WriteItemEffectText(HWND hwnd,char *lpText,short *lpValue)


//
//	아이템 부가 효과 설명
char*
cPREFIX_EFFECT::GetComment(cITEM_EFFECT *lpEffect,BOOL _bSpecial,char *_strValue1,char *_strValue2)
{	// 마지막 수정일 : 09.10.13
	int		iIndex	=	GetIndex(lpEffect->m_wEffect);
	char	*lpText	=	s_aHyperText[iIndex].m_strReferenceText;
	short	*lpValue=	lpEffect->m_aValue;

	static	cSTRING	Text;

	Text.Reset();

	char			*lpStream,*lpKeyWord;
	int				iCode;

	s_iComboIndex	=	0;
	s_iSpinIndex	=	0;
	s_iSpinBoxIndex	=	0;
	s_iStreamIndex	=	0;

	int	iValueIndex	=	0;

	s_Reader.UploadBuffer(lpText,s_aKeyword);

	while(1)
	{
		if (s_Reader.m_isDone	)	break;		//	다 읽어 들였다.

		lpStream	=	s_Reader.GetStream('[');

		if (iValueIndex	>=0	)	TransCode(lpStream);
		Text.Add(lpStream);

		if (s_Reader.m_isDone	)	break;		//	다 읽어 들였다.

		//	콤보 박스나 머 그런거..
		lpKeyWord	=	s_Reader.GetStream(']');
		iValueIndex	=	s_Reader.GetNumber();

		iCode		=	s_Reader.AnalCode(lpKeyWord);

		if (_bSpecial)	Text.Add(_ms("%c",1));

		char	*str	=	NULL;

		switch(iCode)
		{
			case	dIKW_PLUS_MINUS_SET		:
			case	dIKW_PLUS_MINUS			:
			case	dIKW_MIN_MAX_DAMAGE		:
			case	dIKW_BASE_POWER			:
			case	dIKW_AP_DP				:
			case	dIKW_MAGIC_ATTRIBUTE	:
			case	dIKW_JOB				:
				str	=	GetCustomComboText(iCode,lpValue[iValueIndex]);
				break;

			case	dIKW_CHARGEABLE_SKILL	:
				break;

			case	dIKW_ATTACK_BONUS_SKILL	:
				break;

			case	dIKW_REVENGE_SKILL		:
				break;

			case	dIKW_PASSIVE_SKILL		:
				break;

			case	dIKW_ALL_SKILL		:
				
				str = g_aSkill[lpValue[iValueIndex]].m_strName ;
				break;

//	문장 선택형 콤보 박스들

			case	dIKW_SPIN				:
			{
				s_Reader.FindCharOver(']');

				str	=	GetSpinText(lpValue[iValueIndex]);
				if (iValueIndex	==	0	&&	_strValue1)	str	=	_strValue1;
				if (iValueIndex	==	1	&&	_strValue2)	str	=	_strValue2;
				else 
				break;
			}

			default					:
//				ERRMSG("error","what is it?? [%s]",lpKeyWord);
				break;
		}

		if (str)
		{
			CheckJongsung(str);
			Text.Add(str);
		}

		if (_bSpecial)	Text.Add(_ms("%c",2));
	}

	return	Text.String;
}	//	cPREFIX_EFFECT::GetComment(cITEM_EFFECT *lpEffect)




LRESULT CALLBACK 
cPREFIX_EFFECT::s_procItemEffect(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{	switch(message)
	{	case WM_INITDIALOG	:
			InitItemEffectDlg(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,FALSE);
			break;

		case WM_VSCROLL :
		{
			cSPIN::setValue((HWND)lParam,HIWORD( wParam ));
			break;
		}

		case	WM_NOTIFY	:
		{
			if	(wParam	==	IDC_TREE)
			{
				NM_TREEVIEW	*lpNM	=	(NM_TREEVIEW *)lParam;

				switch(lpNM->hdr.code)
				{
					case	TVN_SELCHANGED		:
					{
						int	iSerial	=	lpNM->itemNew.lParam;

						if	(lpNM->itemNew.lParam	==	-1)
							break;

						s_iSelectEffectCategory	=	iSerial>>16;
						s_iSelectEffect			=	iSerial&0xffff;

						memset(&s_Effect,0,sizeof(s_Effect));

						s_Effect.m_wEffect		=	s_iSelectEffect;

						WriteItemEffectText(s_hDlg,&s_Effect);

						break;
					}
				}
			}

			break;
		}

		case WM_COMMAND :
		{
			int	command	=	LOWORD( wParam );

			if (command	>=	IDC_ITEM_COMBO	&& command	<	IDC_ITEM_COMBO	+	s_iComboIndex)
			{
				if (HIWORD( wParam) == 1)
				{
					if	(s_Effect.m_wEffect	!=	eIP_JOB_SKILL_LEVEL)
					{
						ReadEffectData(	hDlg,&s_Effect);
						WriteItemEffectText(hDlg,&s_Effect);
					}
				}
			}

			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
					ReadEffectData(hDlg,&s_Effect);
					EndDialog(hDlg,TRUE);
					break;

				case	IDCANCEL			:
					EndDialog(hDlg,FALSE);
					break;

				case	IDC_FILTER	:
					switch (HIWORD(wParam))
                    {
						case	EN_CHANGE	:
						{
							GetDlgItemText(hDlg,IDC_FILTER,s_strFilter,256);

							FillTree();

							break;
						}
					}
					break;
			}
			break;
		}
	}

	return (0L);
}