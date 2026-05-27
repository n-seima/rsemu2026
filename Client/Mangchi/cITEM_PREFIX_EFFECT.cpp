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
	{"adds.","Take it out.","Set to."					,"end"},	//	dIKW_PLUS_MINUS_SET
	{"Minimum damage","Maximum damage","Total damage" ,"end"}, // dIKW_MIN_MAX_DAMAGE
	{"Strength","Agility","Health","Charisma","Knowledge","Wisdom","Luck" ,"end"}, // dIKW_BASE_POWER
	{""}, // dIKW_CHARGEABLE_SKILL
	{""}, // dIKW_ATTACK_BONUS_SKILL
	{""}, // dIKW_REVENGE_SKILL
	{""}, // dIKW_PASSIVE_SKILL
	{""}, // dIKW_ALL_SKILL
	{"Adds.","Subtracts." ,"end"}, // dIKW_PLUS_MINUS
	{"Attack","Defense" ,"end"}, // dIKW_AP_DP
	{"Flame","Water","Wind","Earth","Light","Darkness" ,"end"}, // dIKW_MAGIC_ATTRIBUTE
	{"Apprentice Knight","Warrior","Wizard","Werewolf","Priest","Fallen Angel","Thief","Martial Artist", // dIKW_JOB
	"Magic Spear Swordsman", "Magic Archer", "Trainer", "Summoner", "Princess", "Transformation Girl", "Spiritualist", "Demon", "Spiritualist", "end"},
};

cPREFIX_EFFECT_REFERENCE
cPREFIX_EFFECT::s_aHyperText[dMAX_ITEM_PREFIX_TYPE_COUNT]	=
{
//////////////////////////////////////////////////
//	기본 능력치 조절
//////////////////////////////////////////////////
eIP_ADD_STRENGTH,"The wearer's strength increases by [spin]0[1,1000].",
eIP_ADD_AGILITY,"The wearer's agility increases by [spin]0[1,1000].",
eIP_ADD_CONSTITUTION,"The wearer's health increases by [spin]0[1,1000].",
eIP_ADD_WISDOM,"The wearer's wisdom increases by [spin]0[1,1000].",
eIP_ADD_INTELIGENCE,"The wearer's knowledge increases by [spin]0[1,1000].",
eIP_ADD_CHARISMA,"The wearer's charisma increases by [spin]0[1,1000].",
eIP_ADD_LUCK,"The wearer's luck increases by [spin]0[1,1000].",
eIP_FIXING_STRENGTH,"Fixes the wearer's strength to [spin]0[1,1000].",
eIP_FIXING_AGILITY,"Fixes the wearer's agility to [spin]0[1,1000].",
eIP_FIXING_CONSTITUTION,"Fixes the wearer's health to [spin]0[1,1000].",
eIP_FIXING_WISDOM,"Fixes the wearer's wisdom to [spin]0[1,1000].",
eIP_FIXING_INTELLIGENCE,"Fixes the wearer's knowledge to [spin]0[1,1000].",
eIP_FIXING_CHARISMA,"Fixes the wearer's charisma to [spin]0[1,1000].",
eIP_FIXING_LUCK,"Fixes the wearer's luck to [spin]0[1,1000].",
eIP_ADD_STRENGTH_BY_LEVEL,"Strength increases by [spin]1[1,100] per wearer's level [spin]0[1,100].",
eIP_ADD_AGILITY_BY_LEVEL,"Agility increases by [spin]1[1,100] per wearer's level [spin]0[1,100].",
eIP_ADD_CONSTITUTION_BY_LEVEL,"Health increases by [spin]1[1,100] per wearer's level [spin]0[1,100].",
eIP_ADD_WISDOM_BY_LEVEL,"Wisdom increases by [spin]1[1,100] per wearer's level [spin]0[1,100].",
eIP_ADD_INTELIGENCE_BY_LEVEL,"Knowledge increases by [spin]1[1,100] per level of the wearer [spin]0[1,100].",
eIP_ADD_CHARISMA_BY_LEVEL,"Charisma increases by [spin]1[1,100] per wearer's level [spin]0[1,100].",
eIP_ADD_LUCK_BY_LEVEL,"Luck increases by [spin]1[1,100] per wearer's level [spin]0[1,100].",
eIP_ADD_DAMAGE_PERCENTAGE,"Damage increases by [spin]0[1,1000] percent.",
eIP_ADD_MIN_DAMAGE,"Weapon minimum damage increases by [spin]0[1,1000].",
eIP_ADD_MAX_DAMAGE,"Weapon maximum damage increases by [spin]0[1,1000].",
eIP_ADD_DEFENSE_PERCENTAGE,"Defense increases by [spin]0[1,1000] percent.",
eIP_ADD_DEFENSE,"Defense increases by [spin]0[1,1000].",
eIP_ADD_MAX_HP_PERCENTAGE,"Max health increases by [spin]0[1,1000] percent.",
eIP_ADD_MAX_HP,"Maximum health increases by [spin]0[1,1000].",
eIP_ADD_MAX_CP_PERCENTAGE,"Maximum CP increases by [spin]0[1,1000] percent.",
eIP_ADD_MAX_CP,"Maximum CP increases by [spin]0[1,1000].",
eIP_LOW_CP,"When moving, the reduction limit CP increases by [spin]0[1,1000].",

/////////////////////////////////////////////////////
// Various probability corrections
/////////////////////////////////////////////////////
eIP_CORRECT_HIT_RATE,"Hit rate increases by [spin]0[1,1000].",
eIP_CORRECT_DODGE_RATE,"The evasion rate increases by [spin]0[1,1000].",
eIP_SHIELD_BLOCK_RATE,"Shield block rate increases by [spin]0[1,1000].",
eIP_CRITICAL_HIT,"Critical hit activation probability increases by [spin]0[1,1000].",
eIP_CRUSH_SHOT,"The probability of triggering the final blow increases by [spin]0[1,1000].",
eIP_IGNORE_TARGET_DODGE_ABILITY,"Ignores the defender's evasion rate correction.",
eIP_IGNORE_ATTACKER_CORRECT_HIT_VALUE,"Ignores the attacker's hit rate correction.",
eIP_PERFECT_HIT,"Always hits.",
eIP_INCREASE_HARD_BLOW,"Hard hit probability increases by [spin]0[1,1000]%",
///////////////////////////////////////////////////// ////
// Speed/Time Correction
///////////////////////////////////////////////////// ////
eIP_BLOCKING_SPEED,"Blocking speed increases by [spin]0[1,1000] percent.",
eIP_MOVE_SPEED,"Movement speed increases by [spin]0[1,1000] percent.",
eIP_ACTION_SPEED,"Attack speed increases by [spin]0[1,1000] percent.",
eIP_POTION_HEAL_SPEED,"Increases potion recovery speed by ([spin]0[1,1000]+50) percent.",
eIP_ADD_CONCENTRATION,"Increases concentration by [spin]0[1,1000] percent.",

///////////////////////////////////////////////////// //////////////////////////////
// Additional effect when attack is successful
///////////////////////////////////////////////////// //////////////////////////////
eIP_FIRE_DAMAGE,"Inflicts [spin]0[1,20000]~[spin]1[1,20000] fire property damage to the target.",
eIP_WATER_DAMAGE,"Inflicts [spin]0[1,20000]~[spin]1[1,20000] water damage to the target and freezes it for [spin]2[1,1000]Frames.",
eIP_WIND_DAMAGE,"Inflicts [spin]0[1,20000]~[spin]1[1,20000] wind attribute damage to the target and stuns it for [spin]2[1,1000]Frames.",
eIP_TERRA_DAMAGE,"Inflicts [spin]0[1,20000]~[spin]1[1,20000] poison damage per second to the target for [spin]2[1,1000] seconds.",
eIP_LIGHT_DAMAGE,"Inflicts light attribute damage of [spin]0[1,20000]~[spin]1[1,20000] to the target and reduces accuracy and evasion for [spin]2[1,1000] seconds. ",
eIP_DARK_DAMAGE,"Inflicts [spin]0[1,20000]~[spin]1[1,20000] dark attribute damage to the target and curses the target for [spin]2[1,1000] seconds.",
eIP_STEAL_HP,"When the attack is successful, [spin]0[1,1000]% of the damage inflicted on the enemy is absorbed as health.",
eIP_BONUS_CP,"When obtaining CP, you get [spin] 0[1,1000] percent CP as a bonus.",
eIP_FLEE,"When the attack is successful, there is a [spin]0[1,100] percent chance of causing the target to flee.",
eIP_KNOCK_OUT,"Increases knockout attack success rate by [spin]0[1,100] percent.",
eIP_INSTANT_KILL,"[spin]Instant death attack occurs with a 0[1,100] percent chance.",
eIP_WEAPON_BREAKER,"[spin] A weapon destruction attack is triggered with a 0[1,100] percent chance, and lasts for [spin]1[1,1000] seconds.",
eIP_ARMOR_BREAKER,"[spin]An armor-breaking attack is activated with a 0[1,100] percent chance, and lasts for [spin]1[1,1000] seconds.",
eIP_MAKE_COLD,"[spin] A cold attack is activated with a 0[1,100] percent chance, and lasts for [spin]1[1,1000] seconds.",
eIP_MAKE_FREEZE,"[spin]Freeze attack is activated with a 0[1,100] percent chance, and lasts for [spin]1[1,1000] seconds.",
eIP_MAKE_BLIND,"[spin]0[1,100] percent chance to trigger a blind attack, lasting [spin]1[1,1000] seconds.",
eIP_MAKE_STUN,"[spin] A stun attack is triggered with a 0[1,100] percent chance, and lasts for [spin]1[1,1000] seconds.",
eIP_MAKE_STONE,"[spin] A petrification attack is activated with a 0[1,100] percent chance, and lasts for [spin]1[1,1000] seconds.",
eIP_MAKE_CONFUSE,"[spin] A confusion attack is triggered with a 0[1,100] percent chance, and lasts for [spin]1[1,1000] seconds.",
eIP_MAKE_SLEEP,"[spin]0[1,100] percent chance to trigger a sleep attack, lasting [spin]1[1,1000] seconds.",
eIP_MAKE_CHARMING,"[spin]Charming attack is activated with a 0[1,100] percent chance, and lasts for [spin]1[1,1000] seconds.",
eIP_MAKE_FIRST_TARGET,"[spin] A priority target creation attack is activated with a 0[1,100] percent chance, and lasts for [spin]1[1,1000] seconds.",
eIP_MAKE_AI_DOWN ,"[spin]An AI lowering attack is triggered with a 0[1,100] percent chance, and lasts for [spin]1[1,1000] seconds.",
eIP_MAKE_BERSERK,"[spin]The Berserk state creation attack is activated with a 0[1,100] percent chance, and lasts for [spin]1[1,1000] seconds.",

///////////////////////////////////////////////////// ///////////////////////////////////////////////////// //////////////////
// Magic attack resistance
///////////////////////////////////////////////////// ///////////////////////////////////////////////////// //////////////////
eIP_RESISTANCE_FIRE,"Fire resistance increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_WATER,"Water resistance increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_WIND,"Wind attribute resistance increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_TERRA,"Earth attribute resistance increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_LIGHT,"Light attribute resistance increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_DARK,"Dark attribute resistance increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_MAGIC,"Resistance to all magical attacks increases by [spin]0[1,100] percent.",
eIP_ABSORB_FIRE,"Absorbs [spin]0[1,100] percent of fire attribute damage and turns it into stamina.",
eIP_ABSORB_WATER,"Absorbs [spin]0[1,100] percent of water attribute damage and turns it into stamina.",
eIP_ABSORB_WIND,"Absorbs [spin]0[1,100] percent of wind attribute damage and turns it into stamina.",
eIP_ABSORB_TERRA,"Absorbs [spin]0[1,100] percent of earth attribute damage and turns it into stamina.",
eIP_ABSORB_LIGHT,"Absorbs [spin]0[1,100] percent of light attribute damage and turns it into stamina.",
eIP_ABSORB_DARK,"Absorbs [spin]0[1,100] percent of dark attribute damage and turns it into stamina.",
eIP_ABSORB_MAGIC,"Absorbs [spin]0[1,100] percent of all magic attribute damage and turns it into stamina.",

///////////////////////////////////////////////////// ///////////////////////////////////////////////////// ///////////////////
// Various resistances, immunity
///////////////////////////////////////////////////// ///////////////////////////////////////////////////// ///////////////////
eIP_RESISTANCE_FREEZE, "Resistance to freeze attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_COLD, "Resistance to cold attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_STUN, "Resistance to stun attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_CONFUSE, "Resistance to confusion attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_CHARMING, "Resistance to Charming attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_STONE, "Resistance to petrification attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_INSTANCE_KILL, "Resistance to instant death attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_POISON, "Resistance to poison attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_SLEEP, "Resistance to sleep attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_STRANGE_STATUS, "Resistance to status attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_DECLINE_STATUS, "Resistance to low-level attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_CURSE_STATUS, "Resistance to low-level attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_ALL_BAD_STATUS, "Resistance to all abnormal attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_KNOCK_BACK, "Resistance to green and white attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_CRITICAL_SHOT, "Resistance to critical attacks increases by [spin]0[1,100] percent.",
eIP_RESISTANCE_CRUSH_SHOT, "Resistance to final blow attacks increases by [spin]0[1,100] percent.",

///////////////////////////////////////////////////// ///////////////////////////////////////////////////// /////
// Counterattack, skill related
///////////////////////////////////////////////////// ///////////////////////////////////////////////////// /////
eIP_RETURN_DAMAGE, "Returns [spin]0[1,1000] percent of the damage received to the attacker.",
eIP_DAMAGE_TO_CP, "Converts [spin]0[1,1000] percent of received damage to CP.",
eIP_MAKE_FREEZE_ATTACKER, "When attacked, there is a [spin]0[1,100] percent chance of triggering a freeze attack that lasts for [spin]1[1,1000] seconds.",
eIP_MAKE_COLD_ATTACKER, "When attacked, there is a [spin]0[1,100] percent chance of triggering a cold attack that lasts [spin]1[1,1000] seconds.",
eIP_CHARGE_SKILL, "When attacked, there is a [spin]0[1,100] percent chance to trigger a cold attack that lasts for [spin]1[1,1000] seconds.",
eIP_JOB_SKILL_LEVEL, "[job]1's skill level increases by [spin]0[1,1000].",
eIP_ALL_SKILL_LEVEL, "The level of all skills increases by [spin]0[1,1000].",

///////////////////////////////////////////////////// ///////////////////////////////////////////////////// /////
// Other/Special Abilities
///////////////////////////////////////////////////// ///////////////////////////////////////////////////// /////
eIP_FIRST_AID, "Heals [spin]0[1,1000] percent of damage.",
eIP_CHANGE_WEAPON_SPEED, "Increases weapon swap speed by [spin]0[1,1000] percent.",
eIP_REGENERATION_HP, "Health is restored at [spin]0[1,1000]/10 per second.",
eIP_RESURRECTION_CHANCE, "If incapable of combat, there is a [spin]0[1,1000] percent chance of being revived.",
eIP_BLURRED, "It becomes blurry when stopped.",
eIP_INVISIBLE, "Becomes transparent. (The invisible state is released only when an action is taken.)",
eIP_LEVITATE, "Becomes levitated.",
eIP_MAGIC_BULLET, "If there are no bullets, fire a magic bullet.",
eIP_INFINITY_BULLET, "You can use unlimited bullets.",
eIP_NOT_AIMED, "You will not be attacked until you attack.",
eIP_INCREASE_RARE_ITEM_DROP_CHANCE, "Magic item drop probability increases by [spin]0[1,1000] percent.",
eIP_INCREASE_UNIQUE_ITEM_DROP_CHANCE, "Unique item drop probability increases by [spin]0[1,1000] percent.",
eIP_DECREASE_RELOAD_TIME, "The time it takes to fill the belt with items is reduced by [spin]0[1,1000] percent.",
eIP_AUTO_RELOAD, "Items are automatically filled in the belt.",
eIP_DISABLE, "The item cannot be used.",
eIP_CAN_NOT_STRIP, "It is impossible to take it off after wearing it.",
eIP_CAN_NOT_ADD_PREFIX, "Additional enchantments are not possible.",

eIP_UPGRADE_FIRE_DAMAGE_ATTACK, "Raise fire attack by 1 level",
eIP_UPGRADE_WATER_DAMAGE_ATTACK, "Raise water attack by 1 level",
eIP_UPGRADE_WIND_DAMAGE_ATTACK, "Wind attack increases by 1 level",
eIP_UPGRADE_EARTH_DAMAGE_ATTACK, "Earth attack increases by 1 level",
eIP_UPGRADE_LIGHT_DAMAGE_ATTACK, "Light attack increases by 1 level",
eIP_UPGRADE_DARK_DAMAGE_ATTACK, "Dark attack increases by 1 level",

eIP_CONTROL_SKILL_LEVEL_BY_DIFFICULTY, "Skill difficulty level increases by [spin]1[1,100] for skills below level [spin]0[1,5].",
eIP_INCREASE_SUMMON_BEAST_STATUS, "All stats of the summoned animal increase by [spin]0[1,255]",
eIP_INCREASE_GAIN_EXP_FOR_PET, "[spin]0[1,255]% pet experience bonus",
eIP_IS_SHOP_SPECIAL_ITEM, "Use period",

eIP_WEAKNING_TARGET_FIRE_RESISTANCE, "Weakens the target's fire resistance by [spin]0[1,255]%.",
eIP_WEAKNING_TARGET_WATER_RESISTANCE, "Weakens the target's water resistance by [spin]0[1,255]％.",
eIP_WEAKNING_TARGET_WIND_RESISTANCE, "Weakens the target's wind resistance by [spin]0[1,255]％.",
eIP_WEAKNING_TARGET_EARTH_RESISTANCE, "Weakens the target's ground resistance by [spin]0[1,255]%.",
eIP_WEAKNING_TARGET_LIGHT_RESISTANCE, "Weakens the target's light resistance by [spin]0[1,255]％.",
eIP_WEAKNING_TARGET_DARK_RESISTANCE, "Weakens the target's darkness resistance by [spin]0[1,255]％.",
eIP_WEAKNING_TARGET_MAGIC_RESISTANCE, "Weakens the target's magic resistance by [spin]0[1,255]%.",
eIP_STRENGTHENING_FIRE_DAMAGE, "Enhances fire attribute attack power by [spin]0[1,300]％.",
eIP_STRENGTHENING_WATER_DAMAGE, "Enhances water attribute attack power by [spin]0[1,300]％.",
eIP_STRENGTHENING_WIND_DAMAGE, "Enhances wind attribute attack power by [spin]0[1,300]％.",
eIP_STRENGTHENING_EARTH_DAMAGE, "Enhances earth attribute attack power by [spin]0[1,300]％.",
eIP_STRENGTHENING_LIGHT_DAMAGE, "Enhances light attribute attack power by [spin]0[1,300]％.",
eIP_STRENGTHENING_DARK_DAMAGE, "Enhances dark attribute attack power by [spin]0[1,300]％.",
eIP_STRENGTHENING_MAGIC_DAMAGE, "Enhances magic attack power by [spin]0[1,300]％.",

eIP_DECREASE_STRENGTH, "The wearer's strength is reduced by [spin]0[1,255].",
eIP_DECREASE_AGILITY, "The wearer's agility is reduced by [spin]0[1,255].",
eIP_DECREASE_CONSTITUTION, "The wearer's health decreases by [spin]0[1,255].",
eIP_DECREASE_WISDOM, "The wearer's wisdom decreases by [spin]0[1,255].",
eIP_DECREASE_INTELIGENCE, "The wearer's knowledge decreases by [spin]0[1,255].",
eIP_DECREASE_CHARISMA, "The wearer's charisma decreases by [spin]0[1,255].",
eIP_DECREASE_LUCK, "The wearer's luck decreases by [spin]0[1,255].",

eIP_DECLINE_MOVE_SPEED, "Movement speed is reduced by [spin]0[1,255] percent.",
eIP_DECLINE_ACTION_SPEED, "Attack speed is reduced by [spin]0[1,255] percent.",

eIP_ADD_DAMAGE_TO_UNDEAD, "Inflicts an additional [spin]0[1,300]% damage to undead characters.",
eIP_ADD_DAMAGE_TO_DEMON, "Inflicts an additional [spin]0[1,300] percent damage to devil-type characters.",
eIP_ADD_DAMAGE_TO_HUMAN, "Inflicts an additional [spin]0[1,300] percent damage to humanoid characters.",
eIP_ADD_DAMAGE_TO_ANIMAL, "Inflicts an additional [spin]0[1,300] percent damage to animal-type characters.",
eIP_ADD_DAMAGE_TO_HOLY_ANIMAL, "Gives an additional [spin]0[1,300] percent damage to divine beast characters.",
eIP_POTION_HEAL_SPEED2, "Potion recovery speed +[spin]0[1,255] percent",

eIP_DECLINE_DEFENSE_PERCENTAGE, "Defense decreases by [spin]0[1,255] percent.",
eIP_DECLINE_DEFENSE, "Defense decreases by [spin]0[1,255].",
eIP_DECLINE_HIT_RATE, "Hit rate decreased by [spin]0[1,255] percent",
eIP_DECLINE_DODGE_RATE, "Dodge rate reduced by [spin]0[1,255] percent",
// VV 09.08.07 Unique option Magic Chudam DX+_+
eIP_FIRE_DAMAGE_DX, "Inflicts [spin]0[1,20000]~[spin]1[1,20000] fire property damage to the target.",
eIP_WATER_DAMAGE_DX, "Inflicts [spin]0[1,20000]~[spin]1[1,20000] water damage to the target and freezes it for [spin]2[1,1000] Frames.",
eIP_WIND_DAMAGE_DX, "Inflicts [spin]0[1,20000]~[spin]1[1,20000] wind attribute damage to the target and stuns it for [spin]2[1,1000]Frames.",
eIP_TERRA_DAMAGE_DX, "Inflicts [spin]0[1,20000] to [spin]1[1,20000] poisoning damage per second to the target for [spin]2[1,1000] seconds.",
eIP_LIGHT_DAMAGE_DX, "Inflicts [spin]0[1,20000]~[spin]1[1,20000] light attribute damage to the target and reduces accuracy and evasion for [spin]2[1,1000] seconds. ",
eIP_DARK_DAMAGE_DX, "Inflicts [spin]0[1,20000]~[spin]1[1,20000] dark attribute damage to the target and curses the target for [spin]2[1,1000] seconds.",
// AA 09.08.07 Unique Option Magic Chudam DX+_+
eIP_BUFF_SKILL_CASTING, "Casts [owner_skill]0 skill at [spin]1[1,200] level, related stat [spin]2[1,20000]. (Duration [spin]3[0,20000] seconds)", // Receive buff skills!
eIP_INCREASE_ALL_STATUS, "All stats of the wearer increase by [spin]0[1,20000].", // Increases all stats
eIP_ACL_BUFF_SKILL_CASTING, "Cast [owner_skill]0 skill at imprint level.", // Receive buff skill!
0xffff, "End"
};

char* cPREFIX_EFFECT::s_strPrefixEffectCategoryList[] =
{ "Basic ability adjustment", "Attack power, defense power, stamina, CP", "Various probability adjustment", "Speed/time correction", "Additional attack/Additional attack effect",
"Magic resistance", "Various resistances and immunity", "Counterattack/Skill", "Enchant", "Elemental damage correction", "Other/special abilities", "end"
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