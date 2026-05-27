#include "cITEM_EFFECT.H"
#include "cITEM_PREFIX_EFFECT.H"
#include "cMAIN.h"
#include "cTEXT.h"
#include "cDRAW.h"
#include "resource.h"

char	cITEM_GENERATE_EFFECT::s_strComboStr[100][20][40]	=
{
	{"numerical value 1","numerical value 2","end"},
};

cKEYWORD_ITEM	cITEM_GENERATE_EFFECT::s_aKeyword[100]	=
{
	"numerical value"			,0				,
	"end"			,dCODE_END
};


//	模,渠 熱纂陛 舒 0檜賊 彰嬪陛 -32767 ~ 32767陛 棲.
//	ぷ撫じ雖煎 瞳辨腎朝 熱纂曖 模,渠 熱纂陛 舒 0檜賊 蛤ィお煎 -400~40煎 撲薑棲.

//	dMETHOD_ANY	  VALUE,RATE 萃 瞳辨 陛棟

cDECODER		cITEM_GENERATE_EFFECT::s_Reader;
int				cITEM_GENERATE_EFFECT::s_iSelectEffect			=	0;
int				cITEM_GENERATE_EFFECT::s_iSelectEffectCategory	=	0;
cITEM_GENERATE_DATA		cITEM_GENERATE_EFFECT::s_itemData;
HWND			cITEM_GENERATE_EFFECT::s_hDlg				=	NULL;

int				cITEM_GENERATE_EFFECT::s_iSpinIndex			=	0;
int				cITEM_GENERATE_EFFECT::s_iSpinBoxIndex		=	0;
int				cITEM_GENERATE_EFFECT::s_iStreamIndex		=	0;
int				cITEM_GENERATE_EFFECT::s_iComboIndex		=	0;
BOOL			cITEM_GENERATE_EFFECT::s_isJongSung			=	FALSE;	//	
char			cITEM_GENERATE_EFFECT::s_strFilter[256]		=	"";

cSTATIC			cITEM_GENERATE_EFFECT::s_aStream[10];
cCOMBOBOX		cITEM_GENERATE_EFFECT::s_aCombo[10];
cEDITBOX		cITEM_GENERATE_EFFECT::s_aSpinBox[10];
cSPIN			cITEM_GENERATE_EFFECT::s_aSpin[10];
WORD			cITEM_GENERATE_EFFECT::s_aValue[2][2];
cTREEVIEW		cITEM_GENERATE_EFFECT::s_treeView;


cITEM_GENERATE_EFFECT		cITEM_GENERATE_EFFECT::s_aHyperText[dMAX_ITEM_EFFECT_DATA]	=
{
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早晦獄 熱纂
"Defense",eIE_ADD_DEFENSE_POWER,
"Defense increases by [value] 0.",

"Shield Blocking Rate",eIE_BLOCKING_RATE,
"The shield's block rate is [numerical] 0%.",

"Number of belt stack items",eIE_BELT_STACK_COUNT,
"The number of belt stack items is [numerical] 0.",

"Create prefix",eIE_FREE_RARE_OPTION,
"[Number] It is possible to create an unlimited number of prefixes.",

"Summon Minipet",eIE_RECALL_MINIPET, // Summon Minipet
"Summon a minipet",

"Extend minipet time",eIE_EXTEND_MINIPET_TIME , // Increases minipet time
"Extends the minipet's summoning time.",

"Extended time for all minipets",eIE_EXTEND_ALL_MINIPET_TIME , // Increases the time for all minipets
"Extends the summoning time of all minipets.",

"Seal the minipet",eIE_SEAL_MINIPET , // Seal the minipet
"Seal the minipet in the pocket",

"Change Minipet Name",eIE_CHANGE_MINIPET_NAME , // Change Minipet Name
"Change the name of the minipet",

"Pocket with minipets",eIE_MINIPET_POUCH , // Pocket with minipets
"Pocket for storing mini pets",

"Minipet food",eIE_FLAKE_OF_DOING_ONE,
"Minipet food (biotype) [value] 0 [value] 1",

"Minipet food",eIE_FEATHER_OF_THINKING_ONE,
"Minipet food (energy type) [value] 0 [value] 1",

"Minipet food",eIE_TOY_OF_SIN_AND_RIN,
"Mini pet food (mineral type) [value] 0 [value] 1",

"Minipet double-sided frame",eIE_REVERSIBLE_FRAME,
"Allow you to summon two minipets. Period [value] 0 days ",

"Minipet Reflecting Mixer",eIE_REFLECTING_MIXER,
"Mix 0 minipets [value] and summon a new minipet.",

"Minipet Binding Chain",eIE_CHAIN_OF_RESTRAINT,
"Binds a minipet and improves its basic stats.",

"Chapter 5: Select Level Up Title",eIE_CH5_SELECT_UPGRADE_TITLE,
"Chapter 5: Select level-up title",

"Chapter 5: Tradeable",eIE_CH5_RELASE_REVERSION,
"Chapter 5: Making non-tradable items tradable.",

"Chapter 5: Guild Skill Reset",eIE_CH5_RESET_GUILD_SKILL_POINT,
"Chapter 5: Reset guild skills.",

"Chapter 5: Health Increase Amulet",eIE_CH5_BONUS_HP_AMULET,
"Chapter 5: Amulet that increases stamina",

"Chapter 5: CP Increase Amulet",eIE_CH5_BONUS_CP_AMULET,
"Chapter 5: Amulet that increases CP",

"Chapter 5: Equip Limit Level Increase Talisman",eIE_CH5_OVER_POWER_ITEM_EQUIP,
"Chapter 5: Amulet that increases the wear limit level",

"Chapter 5: Reverse Skill",eIE_CH5_REVERSE_SKILL_POINT,
"Chapter 5: Skill Reverse Item",

"Chapter 5: Stat Reverse",eIE_CH5_REVERSE_STATE_POINT,
"Chapter 5: Stat Reverse Items",

"Chapter 5: Prefix Removal",eIE_CH5_REMOVE_PREFIX,
"Chapter 5: Prefix Removal Items",

"Chapter 5: Copy Items",eIE_CH5_COPY_ITEM,
"Chapter 5: Item Copy Item",

"Chapter 5: Upgrading Normal Item Prefixes",eIE_CH5_UPGRADE_NORMAL_ITEM_PREFIX_POWER,
"Chapter 5: Upgrading Common Item Prefixes",

"Chapter 5: Improving Normal Item Performance",eIE_CH5_IMPROVE_NORMAL_ITEM_POWER,
"Chapter 5: Improving General Item Performance",

"Chapter 5: Unique Item Prefix Upgrade",eIE_CH5_UPGRADE_UNIQUE_ITEM_PREFIX_POWER,
"Chapter 5: Unique Item Prefix Upgrade",

"Chapter 5: Improving Unique Item Performance",eIE_CH5_IMPROVE_UNIQUE_ITEM_POWER,
"Chapter 5: Improving Unique Item Performance",

"Chapter 5: Red Bottomless Box",eIE_CH5_MIX_ITEM_BOX,
"Chapter 5: Red Bottomless Box",

"Chapter 5: Select propensity values",eIE_CH5_SELECT_ALLIGNMENT,
"Chapter 5: Selecting Propensity Values",

//早晦獄 熱纂
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早ん暮/晦顫 擒ヶ 
"Health recovery (numerical value)", eIE_RECOVER_HP,
"Restores stamina to [numerical] 0 points.",

"Health recovery 2 (numerical value)", eIE_RECOVER_HP2,
"Restores stamina by [value] 0 * 100 points.",

"Instant health recovery (numerical value)", eIE_INSTANCE_RECOVER_HPx100,
"Instantly restores stamina [value] 0 * 100 points.",

"Health recovery (percentage)", eIE_RECOVER_HP_PERCENTAGE,
"Restores health [level] 0%.",

"Instant health recovery (percentage)", eIE_INSTANCE_RECOVER_HP_PERCENTAGE, // Instant health recovery
"Instantly restores 0% health [level].",

"Instant decrease in remaining health (percentage)", eIE_DECREASE_INSTANT_HP_PERCENTAGE, // Instant decrease in remaining health (percentage)
"Instantly reduces remaining health [value] by 0%.",

"CP recharge (numerical value)", eIE_RECOVER_CP,
"Recharges CP by [numerical] 0 points.",

"CP Charge 2 (numerical value)", eIE_RECOVER_CP2,
"Recharges CP by [numerical] 0 * 100 points.",

"CP Recharge (Percentage)", eIE_RECOVER_CP_PERCENTAGE,
"Recharges CP by [value] 0%.",

"HP/CP Recovery", eIE_RECOVER_HP_CP,
"Restores the character＊s stamina and CP by [numerical] 0 points.",

"Recover health/CP together", eIE_RECOVER_HP_CP_PERCENTAGE,
"Restores the character's stamina and CP by [numerical] 0%.",

"Resurrection of others", eIE_RESURRECTION,
"After reviving a character who is unable to fight, recovers [numerical] 0% physical strength.",

"Self Resurrection", eIE_SELF_RESURRECTION,
"If you become incapable of combat, revive yourself and restore [value] 0% of your health.",

"Party Completely Revived", eIE_PERFECT_PARTY_REVIVE,
"Completely revives all party members who are incapable of combat and increases maximum HP and CP by [value] 1% for [value] 0 seconds.",

"Neutralize", eIE_HEAL_STRANGE_STATUS,
"Reduces all abnormal status values to [value] 0%.",

"Treat abnormal state", eIE_RECOVER_ALL_STRANGE_STATUS,
"Cures all abnormal status conditions.",

"Treat lower-world conditions", eIE_RECOVER_ALL_DECLINE_STATUS,
"Cures all lower level status ailments.",

"Cure Curse Condition", eIE_RECOVER_ALL_CURSE_STATUS,
"Cures all low-level ailments.",

"Cure all status ailments", eIE_RECOVER_ALL_BADSTATUS,
"Cures all status ailments.",

"Addiction Treatment", eIE_HEAL_POSION,
"Treats addiction.",

"Temporarily increases power.", eIE_SHORT_WHILE_INCREASE_POWER,
"Increases strength by [value] 0 for [value] 1 second.",

"Temporarily increases agility.", eIE_SHORT_WHILE_INCREASE_AGILITY,
"Increases agility by [value] 0 for [value] 1 second.",

"Temporarily increases health.", eIE_SHORT_WHILE_INCREASE_CONSTITUTION,
"Increases health by [value] 0 for [value] 1 second.",

"Temporarily increases wisdom.", eIE_SHORT_WHILE_INCREASE_WISDOM,
"Increases wisdom by [value] 0 for [value] 1 second.",

"Temporarily increases knowledge.", eIE_SHORT_WHILE_INCREASE_INTELIGENCE,
"Increases knowledge by [value] 0 for [value] 1 second.",

"Temporarily increases charisma.", eIE_SHORT_WHILE_INCREASE_CHARISMA,
"Increases charisma by [value] 0 for [value] 1 second.",

"Temporarily increases luck.", eIE_SHORT_WHILE_INCREASE_LUCK,
"Increases luck by [value] 0 for [value] 1 second.",

"Temporarily increases power (minutes)", eIE_SHORT_WHILE_INCREASE_POWER_BY_MINUTE,
"Increases strength by [value] 0 for [value] 1 minute.",

"Temporarily increases agility (minutes)", eIE_SHORT_WHILE_INCREASE_AGILITY_BY_MINUTE,
"Increases agility by [value] 0 for [value] 1 minute.",

"Temporarily increases health (minutes)", eIE_SHORT_WHILE_INCREASE_CONSTITUTION_BY_MINUTE,
"Increases health by [value] 0 for [value] 1 minute.",

"Temporarily increases wisdom (minutes)", eIE_SHORT_WHILE_INCREASE_WISDOM_BY_MINUTE,
"Increases wisdom by [value] 0 for [value] 1 minute.",

"Temporarily increases knowledge (minutes)", eIE_SHORT_WHILE_INCREASE_INTELIGENCE_BY_MINUTE,
"Increases knowledge by [value] 0 for [value] 1 minute.",

"Temporarily increases charisma. (minutes)", eIE_SHORT_WHILE_INCREASE_CHARISMA_BY_MINUTE,
"Increases charisma by [value] 0 for [value] 1 minute.",

"Temporarily increases luck (minutes)", eIE_SHORT_WHILE_INCREASE_LUCK_BY_MINUTE,
"Increases luck by [value] 0 for [value] 1 minute.",

"Temporarily increases luck (minutes) x 100", eIE_SHORT_WHILE_INCREASE_LUCKx100_BY_MINUTE,
"Increases luck by [value] 0 x 100 for [value] 1 minute.",



"Temporarily increases attack power.", eIE_SHORT_WHILE_INCREASE_ATTACK_POWER,
"Increases attack power by [value]1% for [value]0 seconds.",

"Temporarily increases defense power.", eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER,
"Increases defense by [value]1% for [value]0 seconds.",

"Temporarily increases stamina.", eIE_SHORT_WHILE_INCREASE_HP,
"Increases stamina by [value]1% for [value]0 seconds.",

"Temporarily increases CP.", eIE_SHORT_WHILE_INCREASE_CP,
"Increases CP by [value]1% for [value]0 seconds.",

"Temporarily maintain weapon damage at max", eIE_SHORT_WHILE_SET_MAX_DAMAGE,
"[Numerical] Weapon damage is always maintained at the highest for 0 seconds.",

"Temporarily keep CP max", eIE_SHORT_WHILE_SET_MAX_CP,
"[Numerical value] CP is always maintained at the highest for 0 seconds.",

"Change health value", eIE_CHANGE_HP,
"Changes the current stamina to [value] 0 x [value] 1.",

//早ん暮/晦顫 擒ヶ 
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Other consumable items
"Open the treasure chest", eIE_OPEN_ARCA,
"[Numerical value] Opens a treasure chest of level 0 or lower.",

"Open the door", eIE_OPEN_DOOR,
"[Numerical value] Opens a door of level 0 or lower.",

"Open doors and treasure chests", eIE_OPEN_ARCA_AND_DOOR,
"[Numerical] Opens doors and treasure chests of level 0 or lower.",

"One Way Portal", eIE_ONE_WAY_PORTAL,
"Returning to the village.",

"Trip Portal", eIE_PORTAL,
"Opens a door that allows transportation to and from the village.",

"Use Jewelry", eIE_USE_JEWEL,
"You can use the rare option attached at the time of creation. ([Number] Maintains for 0 minutes, Number of uses [Number] 1)",

"Memory Stone", eIE_MEMORY_STONE,
"Remember the current location in the waypoint slot [Number] 0, and create [Number] 1 related item.",

"Teleport Stone", eIE_TELEPORT_STONE,
"Teleport to the location you remember in the waypoint slot [value] 0.",

"Portal Stone", eIE_PORTAL_STONE,
"Open a portal to the location remembered in the waypoint slot [value] 0.",

"OX Quiz Ticket", eIE_OX_QUIZ_TICKET, // OX Quiz Ticket.
"This is an OX quiz ticket.",

"Charge Magic Power", eIE_CHARGE_EXTRA_ITEM_POWER, // Charge Magic Power
"Recharges the magic power of extra items by 100%.",

"2004 Lunar New Year Event", eIE_EVENT_2004_01_21, // Horsepower Charge
"Option created temporarily for Lunar New Year 2004. Currently discarded",

"Book", eIE_BOOK,
"It＊s a book.",

"Add a category to a book", eIE_ADD_CATEGORY,
"Add a category to a book item.",

"Add page to book", eIE_ADD_PAGE,
"Add a page to a book item.",

"Increased final damage", eIE_INC_LAST_DAMAGE, // Increased final damage
"The final damage increases by [value] 1x10% for [value] 0 minutes regardless of physical or magic.",

"Increase hunting experience", eIE_INC_HUNT_EXP, // Increase hunting experience
"Experience gained by hunting monsters increases by [value] 1x10% for [value] 0 minutes.",

"Increased item drop probability", eIE_INC_HUNT_ITEM_DROP_CHANCE, // Increased hunting experience
"Item drop probability increases by [value]1x10% for [value]0 minutes.",

"Recover Death Penalty", eIE_CLEAR_DEATH_PENELTY, // Recover Death Penalty
"Recovers all death penalties.",

"Increase movement speed", eIE_INCREASE_MOVE_SPEED, // Increase movement speed
"Movement speed increases by [value]1% for [value]0 minutes.",

"Event Balloon", eIE_EVENT_BALLOON, // Event Balloon
"The moment you catch a monster, it explodes with a certain probability and your experience is multiplied by [value] 0.",


"Event Balloon", eIE_EVENT_BALLOON_INC_ITEM_DROP_CHANCE, // Event Balloon item drop rate
"The moment you catch a monster, it explodes with a certain probability and the item drop rate is [value] 0.",

"Minipet food", eIE_MINIPET_FEED,
"Gives [numerical] 10,000 experience points of type [numerical] 0 (1 = Bio, 2 = Energy, 3 = Mineral)",

"Reset Minipet Skill", eIE_RESET_MINIPET_SKILL,
"Reset the minipet skill.",

"Change minipet type", eIE_SET_MINIPET_TYPE,
"[Numerical value] 0 (1 = Bio, 2 = Energy, 3 = Mineral) type changed to 100%.",

"Move a specific field", eIE_MOVE_FIELD,
"Move to a field that is hostile to the field you are moving to.",
//早Other consumable items
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Attack additional effect/additional damage
"Instant death", eIE_INSTANCE_KILL,
"[Numerical value] 0% probability that the target dies instantly.",

"Make target Berserk", eIE_MAKE_BERSERKER,
"Puts the target into the Berserk state for [numerical] 1 second with a [numerical] 0% probability.",

"Armor Break", eIE_MAKE_ARMOR_BREAK,
"Puts the target into an armor-destructive state for [numerical] 1 second with a [numerical] 0% probability.",

"Weapon Destruction", eIE_MAKE_WEAPON_BREAK,
"Puts the target into a weapon-destructive state for [numerical] 1 second with a [numerical] 0% probability.",

"Poisons.", eIE_MAKE_POISONED,
"Poisons. (Damage: [value] 1 per second for [value] 0 seconds)",

"Confuse.", eIE_MAKE_CONFUSE,
"Puts the target into a state of confusion for [number] 1 second with a [number] 0% probability.",

"Temptation", eIE_MAKE_CHARMING,
"Lures the target for [number] 1 second with a probability of [number] 0%.",

"Temptation (30 seconds)", eIE_MAKE_CHARMING_30,
"Lures the target for 30 seconds with a probability of [numerical] 0%.",

"Stun", eIE_MAKE_STUN,
"Puts the target into a stunned state for [numerical] 1 frame with a probability of [numerical] 0%.",

"Cold", eIE_MAKE_COLD,
"Puts the target in a cold state for [numerical] 1 second with a [numerical] 0% probability.",

"Cold (100%)", eIE_MAKE_COLD_100,
"Puts the target into a cold state for [numerical] 0 seconds with a 100% probability.",

"Fire damage", eIE_HIT_FIRE_DAMAGE,
"Inflicts fire attribute damage [value] 0.",

"Water damage", eIE_HIT_WATER_DAMAGE,
"Inflicts water attribute damage [value] 0.",

"Wind attribute damage", eIE_HIT_WIND_DAMAGE,
"Inflicts wind attribute damage [value] 0.",

"Earth attribute damage", eIE_HIT_TERRA_DAMAGE,
"Inflicts earth property damage [value] 0.",

"Light attribute damage", eIE_HIT_LIGHT_DAMAGE,
"Inflicts light attribute damage [value] 0.",

"Dark attribute damage", eIE_HIT_DARK_DAMAGE,
"Inflicts darkness attribute damage [value] 0.",

"Undead Hunter", eIE_ADD_DAMAGE_TO_UNDEAD,
"Inflicts an additional [numerical] 0% damage to undead characters.",

"Demon Hunter", eIE_ADD_DAMAGE_TO_DEMON,
"Inflicts an additional [numerical] 0% damage to devil-type characters.",

"Human Hunter", eIE_ADD_DAMAGE_TO_HUMAN,
"Inflicts an additional [value] 0% damage to humanoid characters.",

"Animal Hunter", eIE_ADD_DAMAGE_TO_ANIMAL,
"Inflicts an additional [numerical value] 0% damage to animal-type characters.",

"Divine Hunter", eIE_ADD_DAMAGE_TO_HOLY_ANIMAL,
"Inflicts an additional [numerical] 0% damage to divine beast-type characters.",
//早Additional attack effect
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Various additional effects
"Reduce the time to fill the belt with items", eIE_DECREASE_RELOAD_TIME,
"The time to fill the belt with items is reduced by [numerical] 0%.",

"Fast transformation speed", eIE_INCREASE_TRANSFORMATION_SPEED,
"The transformation speed becomes [value] 0% faster.",

"Increased recovery speed by potions", eIE_INCREASE_RECOVER_SPEED,
"Potion recovery speed increases by [value] 0%.",

"thorns", eIE_RETURN_DAMAGE,
"Receives [numerical value] 0% of the damage received as damage to the enemy.",

"Spirit 40%", eIE_RETURN_DAMAGE_40,
"Deals 40% of the damage received to the enemy.",

"Infinite Bullet", eIE_INFINITY_BULLET,
"Infinite Bullets",

"Increases the speed of health recovery", eIE_INCREASE_SITDOWN_RECOVER_HP_SPEED, // Increases the speed of health recovery when sitting.
"Increases stamina recovery speed by [numerical] 0% when sitting.",

"Increases hunting experience", eIE_HUNTING_EXP_BONUS, // Increases the speed of stamina recovery when sitting.
"Experience gained through hunting increases between 1 and 200.",

"Absorb physical damage", eIE_AB_PH_DAMAGE,
"[Number] 0% chance to absorb [Number] 1% of physical damage as stamina.",

//早Various additional effects
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Various numerical values increase
"Increased attack power of melee jobs", eIE_INCREASE_MELEE_JOB_ATTACK_POWER,
"The attack power of warriors, apprentice knights, werewolves, priests, martial artists, and magic spearmen increases by [value] 0 to [value] 1.",

"Increase selected ability level", eIE_INCREASE_SELECT_POWER,
"The selected ability increases by [numerical value] 0. [numerical value] 1 (value will be set to 0)",

"Power Up", eIE_INCREASE_POWER,
"Strength increases by [value] 0.",

"Wisdom Rises", eIE_INCREASE_WISDON,
"Wisdom increases by [value] 0.",

"Knowledge Rise", eIE_INCREASE_INTELIGENCE,
"Knowledge increases by [value] 0.",

"Health Increase", eIE_INCREASE_CONSTITUTION,
"Health increases by [value] 0.",

"Increased agility", eIE_INCREASE_AGILITY,
"Agility increases by [value] 0.",

"Increased charisma", eIE_INCREASE_CHARISMA,
"Charisma increases by [value] 0.",

"Lucky Rise", eIE_INCREASE_LUCK,
"Luck increases by [value] 0.",

"Critical occurrence probability", eIE_INCREASE_CRITICAL,
"Critical occurrence probability increases by [numerical] 0.",

"Probability of occurrence of final blow", eIE_INCREASE_CRUSH_SHOT,
"The probability of a final blow increases by [value] 0.",

"Increased hit rate", eIE_INCREASE_HIT_RATE,
"Accuracy increases by [value] 0.",

"Increased evasion rate", eIE_INCREASE_DODGE_CHANCE,
"The evasion rate increases by [value] 0.",

"Increased concentration", eIE_INCREASE_CONCENTRATION,
"Concentration increases by [value] 0.",

"Increased attack speed", eIE_INCREASE_ATTACK_SPEED,
"Attack speed increases by [value] 0%.",

"Increase movement speed", eIE_EQUIP_INCREASE_MOVE_SPEED,
"Movement speed increases by [value] 0%.",

"All stats increase", eIE_INCREASE_ALL_STATUS,
"All ability [values] increase by 0.",

"Increase selected abilities (fixed)", eIE_INCREASE_FIX_SELECT_POWER,
"The selected ability increases by [numerical value] 0. [numerical value] 1 (value will be set to 0)",

"Increase maximum health (x 100)", eIE_ADD_MAX_HPx100,
"Max health [value] increased by 0 x 100",

"Increase maximum CP (x 100)", eIE_ADD_MAX_CPx100,
"Maximum CP [value] increased by 0 x 100",

"Increase defense %", eIE_ADD_DEFENSE_PERCENTAGE,
"Defense [value] increases by 0%.",

"Increase maximum health %", eIE_ADD_MAX_HP_PERCENTAGE,
"Maximum health [value] increases by 0%.",

"Max CP % increase", eIE_ADD_MAX_CP_PERCENTAGE,
"Maximum CP [value] increases by 0%.",

"Red gem acquisition level [value] increased by 0", eIE_INCREASE_LEVEL_GETING_RED_STONE,
"Red gem acquisition level [value] increases by 0.",

//早Various numerical values increase
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Resistance
"Resistance to Curse", eIE_RESISTANCE_CURSE_STATUS,
"Resistance to cursed conditions increases by [value] 0.",

"Resistance to degradation states", eIE_RESISTANCE_DECLINE_STATUS,
"Resistance to degradation increases by [value] 0.",

"Resistance to abnormal states", eIE_RESISTANCE_STRANGE_STATUS,
"Resistance to abnormal conditions increases by [value] 0.",

"Resistance to all status ailments", eIE_RESISTANCE_ALL_BAD_STATUS,
"Resistance to all status ailments increases by [value] 0.",

"Fire Resistance", eIE_INCREASE_FIRE_RESITANCE,
"Inresistance increases by [value] 0%.",

"Earth Resistance", eIE_INCREASE_TERRA_RESITANCE,
"Earth resistance increases by [value] 0%.",

"Wind Resistance", eIE_INCREASE_WIND_RESITANCE,
"Wind resistance increases by [value] 0%.",

"Water Resistance", eIE_INCREASE_WATER_RESITANCE,
"Water resistance increases by [value] 0%.",

"Light resistance", eIE_INCREASE_LIGHT_RESITANCE,
"Light resistance increases by [value] 0%.",

"Dark Resistance", eIE_INCREASE_DARK_RESITANCE,
"Dark resistance increases by [value] 0%.",

"Resistance to fire, water, wind and earth", eIE_INCREASE_FOUR_ELEMENTAL_RESITANCE,
"Resistance to fire, water, wind, and earth increases by [value] 0%.",

"Resistance to magical attacks", eIE_INCREASE_MAGIC_RESITANCE,
"Resistance to magic attribute attacks increases by [numerical] 0%.",

"NOCK BACK RESISTANCE", eIE_INCREASE_NOCK_BACK_RESISTANCE,
"Resistance to green and white increases by [value] 0%.",

"Critical Resistance", eIE_INCREASE_CRITICAL_RESISTANCE,
"Resistance to critical hits increases by [value] 0.",

"Crushing Blow Resistance", eIE_INCREASE_CRUSH_SHOT_RESISTANCE,
"Resistance to final blows increases by [value] 0.",

"Instant Kill Resistance", eIE_INCREASE_INSTANCE_KILL_RESISTANCE,
"Resistance to instant death increases by [value] 0.",
//早Resistance
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Skill level
"Increased magic spearman skill level", eIE_INCREASE_MAGIC_LANCER_SKILL,
"The magic spearman's skill level increases by [numerical] 0.",

"Increase Magic Archer skill level", eIE_INCREASE_MAGIC_ARCHER_SKILL,
"The magic archer's skill level increases by [numerical] 0.",

"Increased Apprentice Knight skill level", eIE_INCREASE_KNIGHT_SKILL,
"The apprentice knight's skill level increases by [numerical] 0.",

"Warrior series skill level increased", eIE_INCREASE_WARRIOR_SKILL,
"The warrior's skill level increases by [numerical] 0.",

"Increase wizard skill level", eIE_INCREASE_WIZARD_SKILL,
"The wizard's skill level increases by [numerical] 0.",

"Increased werewolf skill level", eIE_INCREASE_WEREWOLF_SKILL,
"The werewolf's skill level increases by [numerical] 0.",

"Increased thief skill level", eIE_INCREASE_THIEF_SKILL,
"The thief's skill level increases by [value] 0.",

"Increased martial arts skill level", eIE_INCREASE_FIGHTER_SKILL,
"The martial artist's skill level increases by [value] 0.",

"Increased trainer skill level", eIE_INCREASE_BEAST_TAMER_SKILL,
"The trainer's skill level increases by [value] 0.",

"Increased summoner skill level", eIE_INCREASE_SUMMONER_SKILL,
"The summoner's skill level increases by [value] 0.",

"Princess skill level increased", eIE_INCREASE_PRINCESSE_SKILL,
"The princess's skill level increases by [value] 0.",

"Increased transformation girl skill level", eIE_INCREASE_MAGICAL_GIRL_SKILL,
"The transformed girl's skill level increases by [numerical] 0.",

"Increased priest skill level", eIE_INCREASE_PRIEST_SKILL,
"The priest's skill level increases by [value] 0.",

"Increased Fallen Angel skill level", eIE_INCREASE_FALLEN_ANGEL_SKILL,
"The fallen angel's skill level increases by [numerical] 0.",

"Increased Necromancer skill level", eIE_INCREASE_NECROMANCER_SKILL,
"The spiritualist's skill level increases by [value] 0.",

"Demon skill level increased", eIE_INCREASE_DEMON_SKILL,
"The devil's skill level increases by [numerical] 0.",

"Increase the level of the Spiritualist skill", eIE_INCREASE_SOUL_BRINGER_SKILL,
"The spiritualist's skill level increases by [numerical] 0.",

"Increased skill level of female characters", eIE_INCREASE_FEMALE_SKILL_LEVEL,
"The skill level of female characters increases by [numerical] 0.",

"Increased skill level of male characters", eIE_INCREASE_MALE_SKILL_LEVEL,
"The skill level of male characters increases by [numerical] 0.",

"Increase all skill levels", eIE_INCREASE_SKILL_LEVEL,
"The level of all skills increases by [numerical] 0.",
//早Skill level
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Mysterious magic item
"Level Up", eIE_INCREASE_LEVEL,
"Level [numerical] 0 rises forever.",

"Move field to world map", eIE_JUMP_FIELD_USE_WORLD_MAP,
"Move the field using the world map. (There is a cooldown)",

"Teleport to the desired point on the world map.", eIE_USE_WORLD_MAP_TELEPORT, //
"Teleport to the desired point on the world map. (No cooldown)",

"Traveler bag", eIE_EXPAND_INVENTORY_BAG,
"Increases the size of the traveler's bag by [numerical] 0 spaces.",

"Item box", eIE_LOTTERY_ITEM, // Create guild
"[Numerical value] Randomly creates an item from one of the items in item pack number 0.",

"Selectable item box", eIE_SELECT_ITEM_IN_ITEM_PACK, // Create guild
"[Numerical] Select one of the items in item pack number 0.",

"strength increase", eIE_GROW_STRENGTH,
"[Numerical value] 0 point power increases forever. (Probability of success: [Numerical value] 1%) Death in case of failure",

"Increased agility", eIE_GROW_AGILITY,
"[Numerical value] 0 point's agility increases permanently. (Probability of success: [Numerical value] 1%) Death in case of failure",

"Increased health", eIE_GROW_CONSTITUTION,
"[Numerical value] 0 point's health increases permanently. (Probability of success: [Numerical value] 1%) Death in case of failure",

"Increased Wisdom", eIE_GROW_WISDOM,
"[Numerical value] 0 point's wisdom increases forever. (Probability of success: [Numerical value] 1%) Death in case of failure",

"Increasing knowledge", eIE_GROW_INTELLIGENCE,
"[Numerical value] 0 point knowledge increases forever. (Probability of success: [Numerical value] 1%) Death in case of failure",

"Increase Charisma", eIE_GROW_CHARISMA,
"[Numerical value] 0 point charisma increases permanently. (Probability of success: [Numerical value] 1%) Death in case of failure",

"Increased luck", eIE_GROW_LUCK,
"[Numerical value] 0 points' luck increases forever. (Probability of success: [Numerical value] 1%) Death in case of failure",

"Increased stamina", eIE_GROW_HP,
"[Numerical value] 0 points' HP increases permanently. (Failure probability: [Numerical value] 0 * [Numerical value] 1%) Death in case of failure",

"CP Increase", eIE_GROW_CP,
"[Numerical value] 0 points' CP increases forever. (Probability of failure: [Numerical value] 0 * [Numerical value] 1%) Death in case of failure",

"Increased weapon damage", eIE_INCREASE_WEAPON_DAMAGE,
"Weapon damage [value] 0% permanent increase, 100-100/([value] 0+0.65)% chance of weapon evaporation",

"Increases armor defense", eIE_INCREASE_ARMOR_DEFENSE,
"Armor defense [value] 0 point permanently increases, 100-100/([value] 0+0.65)% chance for armor to evaporate",

"Magic Jewel", eIE_MAGIC_JEWEL,
"Attach a [numerical value] 0 prefix to the weapon. (Probability of success: [numerical value] 1%) In case of failure, the item evaporates",

"Frontier Certificate", eIE_FRONTIER_SCROLL,
"It makes the user a [numerical] 0 level frontier.",

"Normal Item Lottery", eIE_NORMAL_ITEM_LOTTERY_TICKET,
"Randomly creates normal items between drop level [number] 0 and drop level [number] 1. Success rate 100%",

"Rare Item Lottery", eIE_RARE_ITEM_LOTTERY_TICKET,
"Randomly creates a rare item with drop level [number] 0. Success probability [number] 1%",

"Unique Item Lottery", eIE_UNIQUE_ITEM_LOTTERY_TICKET,
"Randomly creates a unique item with drop level [number] 0. Success probability [number] 1%",

"Super Item Lottery", eIE_SUPER_ITEM_LOTTERY_TICKET,
"Randomly creates a super item with drop level [number] 0. Success probability [number] 1%",

"Redistribution of skill points", eIE_RESET_SKILL_POINT,
"Redistribute skill points.",

"Redistribute stat points", eIE_RESET_STATE_POINT, // Reset stats
"Redistribute stat points.",


"Redistribute the strength stat.", eIE_RESET_STRENGTH_STATE, // Redistribute one strength stat
"Redistribute strength stats.",

"Redistribute the agility stat.", eIE_RESET_AGILITY_STATE, // Redistribute one agility stat
"Redistribute the agility stat.",

"Redistribute the health stat.", eIE_RESET_CONSTITUTION_STATE, // Redistribute one health stat
"Redistribute health stats.",

"Redistribute the knowledge stat.", eIE_RESET_INTELLIGENCE_STATE, // Redistribute one knowledge stat
"Redistribute knowledge stats.",

"Redistribute the wisdom stat.", eIE_RESET_WISDOM_STATE, // Redistribute one wisdom stat
"Redistribute wisdom stats.",

"Redistribute the charisma stat.", eIE_RESET_CHARISMA_STATE, // Redistribute one charisma stat
"Redistribute charisma stats.",

"Redistribute the luck stat.", eIE_RESET_LUCK_STATE, // Redistribute one luck stat
"Redistribute luck stats.",


"Change prefix", eIE_CHANGE_ITEM_PREFIX_2003_11_01, // Change prefix
"Change the prefix.",

"Change dangerous prefix", eIE_CHANGE_ITEM_PREFIX_2003_11_17,
"Find and change a possible prefix, even at a lower level.",

"Santa's Orb", eIE_ORB_OF_SANTA,
"1.5x Santa's Orb experience, 1.5x item drop rate, unlimited return to town, entry into premium zone, free use of town teleporter, infinite summoning of magic carpets",

"Temporarily increases final damage.", eIE_INCREASE_FINAL_DAMAGE,
"Increases final damage by [value]1% for [value]0 minutes.",

"Temporarily increases attack power.", eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE,
"Increases attack power by [value]1% for [value]0 minutes.",

"Temporarily increases attack power.", eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE_10,
"Increases attack power by [value]1*10% for [value]0 minutes.",

"Temporarily increases defense power.", eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE,
"Increases defense by [value]1% for [value]0 minutes.",

"Temporarily increases defense.", eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE_10,
"Increases defense by [value]1*10% for [value]0 minutes.",

"Temporarily increases stamina.", eIE_SHORT_WHILE_INCREASE_HP_BY_MINUTE,
"Increases stamina by [value]1% for [value]0 minutes.",

"Temporarily increases CP.", eIE_SHORT_WHILE_INCREASE_CP_BY_MINUTE,
"Increases CP by [value]1% for [value]0 minutes.",

"Temporarily maintain weapon damage at max", eIE_SHORT_WHILE_SET_MAX_DAMAGE_BY_MINUTE,
"[Numerical] Weapon damage is always maintained at the highest for 0 minutes.",

"Temporarily keep CP max", eIE_SHORT_WHILE_SET_MAX_CP_BY_MINUTE,
"[Numerical] CP is always maintained at the highest for 0 minutes.",

"You can use limited level items higher than your level", eIE_OVER_POWER_ITEM_EQUIP,
"You can use limited level items that are [numerically] 0 levels higher than your level",

"Perfect Resurrection", eIE_PERFECT_RESSURECTION,
"Reduces the inability to fight penalty time by 90% and sets maximum health to [value] 1% for [value] 0 seconds.", // Modified 09.10.06

"Breeding record", eIE_BREEDING_RECORD,
"Document your pet.",

"Secret Dungeon Key", eIE_SECRET_DUNGEON_KEY,
"It's a secret dungeon key.",

"Treasure Map", eIE_TREASURE_MAP, // Treasure Map
"It's a treasure map",

"Couple Ring Set", eIE_COUPLE_RING_SET,
"Creates a coupling.",

"Couple Ring", eIE_COUPLE_RING,
"Coupling",

"Present Couple Ring", eIE_PRESENT_COUPLE_RING,
"Gift Coupling Ring",

"Couple ring without counterpart", eIE_NOT_DEFINE_COUPLE_COUPLE_RING,
"Coupling without a partner",

"Bottomless box", eIE_MIX_ITEM_BOX, // Bottomless box
"Bottomless Box",

"Fireworks Set", eIE_FIRE_WORK,
"Detonates [numerical value] 0 fireworks for [numerical value] 1 second.",

"Random Fireworks", eIE_RANDOM_FIRE_WORK,
"Randomly selects one firework and sets it off.",

"Princess Transformation Weapon", eIE_PRINCESS_WEAPON,
"It is a princess transformation weapon.",

"Transform to Operator", eIE_TRANS_TO_OPERATOR,
"Transforming into an operator.",

"Sealing Sphere of Dawn", eIE_SEAL_SPHERE_OF_DAWN, // Seal Sphere of Dawn
"[Numerical] Makes one item of level 0*10 or lower usable regardless of ability.",

//早Mysterious magic item
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Enchant item
"Jim Mori's Enchantment Service", eIE_JIM_MORYS_ENCHANT, // Jim Mori's Enchantment
"Jimmori＊s Enchantment Service",

"Clumsy item repair", eIE_UNSKILLED_REPAIR_BROKEN_ITEM, //
"Clumsily repairs broken items.",

"Item Enchantment", eIE_ENCHANT_ITEM_BY_UNIQUE_INFO, // Jim Morey's Enchantment
"Improve unique items.",

"Repair Broken Item", eIE_REPAIR_BROKEN_ITEM, // "Repair Broken Item",
"Perfectly repairs broken items.",

"Removes the curse that makes it impossible to dismantle equipment", eIE_CURE_ITEM_EQUIP_CURSE, // Removes the curse attached to the equipment.
"Removes the curse of not being able to dismantle equipment attached to the item.",

"Remove the curse that cannot be enchanted", eIE_RELEASE_ATTACH_PREFIX_CURSE, // Removes the curse attached to the equipment.
"Removes the enchantment-impossible curse attached to the item.",

"DX Item Repair", eIE_REPAIR_DX_ITEM_KIT,
"Repairs the durability of DX items by [numerical] 0%.",

"Item Piece", eIE_ITEM_PIECE,
"[Number] 0th item fragment",

"Remove prefix", eIE_REMOVE_PREFIX,
"Remove the prefix.",

"Attach Prefix", eIE_ATTACH_PREFIX,
"[Numerical] Add a prefix with a probability of 0.",

"Copy prefix", eIE_COPY_PREFIX,
"[Number] Copy the prefix with a probability of 0.",

"Cut prefix", eIE_CUT_PREFIX,
"Cut off the prefix. (Creation item option: [numerical value] 0% probability)",

"Attach Prefix", eIE_ATTACH_PREFIX_CAN_NOT_TRADE,
"[Numerical] Add a prefix with a probability of 0. (Untradable)",

"Copy prefix", eIE_COPY_PREFIX_CAN_NOT_TRADE,
"[Numerical] Copy the prefix with a probability of 0. (Untradable)",

"Cut prefix", eIE_CUT_PREFIX_CAN_NOT_TRADE,
"Cut off the prefix. (Creation item option: [numerical value] 0% probability, not tradable)",

//早Enchant item
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

"Weaken target's fire resistance", eIE_WEAKNING_TARGET_FIRE_RESISTANCE,
"Weakens the target's fire resistance by [numerical] 0%.",
"Weaken target's water resistance", eIE_WEAKNING_TARGET_WATER_RESISTANCE,
"Weakens the target's water resistance by [numerical] 0%.",
"Weaken target's wind resistance", eIE_WEAKNING_TARGET_WIND_RESISTANCE,
"Weakens the target's wind resistance by [numerical] 0%.",
"Weaken target's earth resistance", eIE_WEAKNING_TARGET_EARTH_RESISTANCE,
"Weakens the target's ground resistance by [numerical] 0%.",
"Weaken target's light resistance", eIE_WEAKNING_TARGET_LIGHT_RESISTANCE,
"Weakens the target's light resistance by [numerical] 0%.",
"Weaken target's dark resistance", eIE_WEAKNING_TARGET_DARK_RESISTANCE,
"Weakens the target's darkness resistance by [numerical] 0%.",
"Weaken target's magic resistance", eIE_WEAKNING_TARGET_MAGIC_RESISTANCE,
"Weakens the target's magic resistance by [numerical] 0%.",
"Enhanced fire attribute attack power", eIE_STRENGTHENING_FIRE_DAMAGE,
"Increases fire attribute attack power by [numerical] 0%.",
"Enhanced water attribute attack power", eIE_STRENGTHENING_WATER_DAMAGE,
"Increases water attribute attack power by [numerical] 0%.",
"Enhanced wind attribute attack power", eIE_STRENGTHENING_WIND_DAMAGE,
"Increases wind attribute attack power by [numerical] 0%.",
"Enhanced earth attribute attack power", eIE_STRENGTHENING_EARTH_DAMAGE,
"Increases earth attribute attack power by [numerical] 0%.",
"Enhanced light attribute attack power", eIE_STRENGTHENING_LIGHT_DAMAGE,
"Increases light attribute attack power by [numerical] 0%.",
"Enhanced dark attribute attack power", eIE_STRENGTHENING_DARK_DAMAGE,
"Increases dark attribute attack power by [numerical] 0%.",
"Enhanced magic attack power", eIE_STRENGTHENING_MAGIC_DAMAGE,
"Increases magic attribute attack power by [numerical] 0%.",

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Guild related

"Create guild", eIE_CREATE_GUILD, // Create guild
"Create a guild.",

"Guild level up", eIE_GUILD_LEVEL_UP, // Raises the guild level.
"Creates the guild level to [numerical] 0.",

"Redistribution of guild skill points", eIE_RESET_GUILD_SKILL_POINT, // Reset stats
"Redistribute guild skill points.",

"Increased number of guild dungeon entries", eIE_INCREASE_GUILD_DUNGEON_PLAY_COUNT,
"Increase the limit on the number of guild dungeon entries per day to [number] 0.", // 09.10.06

"Guild Mark Expansion Pack", eIE_GS_EXPAND_PACK,
"Guild Mark Expansion Pack",

"Guild Mark Special Pack", eIE_GS_SPECIAL_PACK,
"Guild Mark Special Pack",

"Guild Mark Luxury Pack", eIE_GS_LUXURY_PACK,
"Guild Mark Luxury Pack",

"Guild Mark Hangul Pack", eIE_GS_HANGUL_PACK,
"Guild Mark Korean Pack",

"Guild Mark English Pack", eIE_GS_ENGLISH_PACK,
"Guild Mark English Pack",

"Guild Mark Chinese Characters", eIE_GS_HANMUN_PACK,
"Guild Mark Chinese Pack",

"Guild Mark Item Pack", eIE_GS_ITEM_PACK,
"Guild Mark Item Pack",

"Guild Mark Animal Pack", eIE_GS_ANIMAL_PACK,
"Guild Mark Animal Pack",

"Guild Mark Signal Pack", eIE_GS_SIGNAL_PACK,
"Guild Mark Signal Pack",

"Guild Mark Unique 1 Pack", eIE_GS_UNIQUE1_PACK,
"Guild Mark Unique 1 Pack",

"Guild Mark Unique 2 Pack", eIE_GS_UNIQUE2_PACK,
"Guild Mark Unique 2 Pack",

"Guildhall Portal", eIE_GUILD_HALL_PORTAL,
"Guildhall Portal Item",

"Summon Guild Guardian", eIE_SUMMON_GUILD_GUARDIAN,
"Summon Guild Guardian",

"Extracting guild statue fragments", eIE_GUILD_STATUE_PIECE_EXPORTER,
"Extract the [numerical value] 1 level fragment from the guild statue [numerical value] 0.",

"Guild Statue Fragment", eIE_GUILD_STATUE_PIECE,
"It is a [numerical] level 1 piece with guild statue [numerical value] 0.",



"Reliable Sign", eIE_PS_SIGNBOARD1,
"Use signs that look trustworthy.",

"Colorful sign", eIE_PS_SIGNBOARD2,
"Use colorfully shaped signs.",

"Mysterious Sign", eIE_PS_SIGNBOARD3,
"Use signs with a mysterious feel.",

"Clean Sign", eIE_PS_SIGNBOARD4,
"Use signs that have a clean feel.",

"Passionate sign", eIE_PS_SIGNBOARD5,
"Use signs with a passionate feel.",

"loudspeaker", eIE_PS_LOUD_SPEAKER,
"You can float your own sign to the top, ignoring other chat bubbles.",


"Street vendor text color", eIE_PS_COLOR_TEXT,
"It allows you to select street vendor text in color.",

"Bold street vendor text", eIE_PS_BOLD_TEXT,
"It allows you to thicken the street vendor text.",

"Increased street vendor text content", eIE_PS_EXPAND_TEXT_BUFFER,
"It allows you to write a lot of street vendor text.",

"Twinkle around street vendor signs", eIE_PS_TWINKLE,
"Sprinkle glitter around street vendor signs.",

"Five features related to street vendors", eIE_PS_PACK,
"Enables five street vendor-related functions.",

"Install horizontal banner", eIE_PUT_HORIZON_BANNER, // horizontal banner
"[Number] Install a horizontal banner in the shape of number 0.",

"Install Vertical Banner", eIE_PUT_VERTICAL_BANNER, // Vertical Banner
"[Number] Install a vertical banner in the shape of number 0",


//早Guild related
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Other
"Time Stamp", eIE_TIME_STAMP, // Timestamp
"Record the time.",

"Add title", eIE_ADD_TITLE, // Add title
"[Numerical] Add title number 0.",

"Combination Badge", eIE_SET_BADGE,
"[Numerical] 0 setting badge",

"Setting Rune", eIE_SET_RUNE,
"[Number] 0 setting badge and [Num.] 1 rune",

"Seongmyeongseok", eIE_NAME_STONE,
"Seongmyeongseok",

"Summon a Magic Carpet", eIE_CALL_MAGIC_CARPET,
"Summon a magic carpet.",

"If you have it, it changes the look of your carpet.", eIE_CHANGE_MAGIC_CARPET_SHAPE,
"Changes the carpet shape to [numerical] type 0.",

"Summon a magic carpet that gradually restores health", eIE_REGEN_HP_ON_MAGIC_CARPET,
"Summon a magic carpet that gradually restores health",

"Summon a magic carpet whose CP does not decrease when moved", eIE_NOT_DOWN_CP_ON_MAGIC_CARPET,
"Summon a magic carpet whose CP does not decrease even when moved",

"Summon a carpet faster than the existing carpet", eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET,
"Summon a carpet faster than existing carpets",

"Summon the Red Drake Carpet", eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET_RED,
"Summon the Red Drake Carpet",

"Summon a carpet that is slower than existing carpets but can fight", eIE_FIGHT_MAGIC_CARPET,
"Summon a carpet that is slower than existing carpets but capable of combat",

"Duel buff: Fire", eIE_DUEL_BUFF_FIRE, // Duel buff fire
"[Numerical value] Applies a fire attribute duel buff for 0 seconds.",

"Duel buff: Water", eIE_DUEL_BUFF_WATER, // Duel buff fire
"[Numerical value] Applies a water attribute duel buff for 0 seconds.",

"Duel buff: Wind", eIE_DUEL_BUFF_WIND, // Duel buff fire
"[Numerical value] Applies a wind attribute duel buff for 0 seconds.",

"Duel buff: Earth", eIE_DUEL_BUFF_EARTH, // Duel buff fire
"[Numerical value] Applies an earth attribute duel buff for 0 seconds.",

"Setrun (not implemented)", eIE_SET_RUNE, // Duel buff fire
"Not implemented",

"Minipet awakening: 50%", eIE_MINIPET_AWAKEN50, // Duel buff fire
"Apply 50% of the minipet's 3rd awakening formula.",

"Minipet awakening: 100%", eIE_MINIPET_AWAKEN100, // Duel buff fire
"Minipet 3rd awakening formula is applied 100%.",

"Magic Spell Book", eIE_CASTING_MAGIC, // Magic Spell Book
"Use INDEX [numerical value] 0 auxiliary magic as an ability level with LEVEL [numerical value] 1.",

"Secret dungeon entry count", eIE_INCREASE_SECRET_DUNGEON_PLAY_COUNT,
"Increase the limit on the number of times you can enter the secret dungeon per day by [numerical] 0.", // 09.10.06

"Increased minipet experience gain rate", eIE_INCREASE_GAIN_MINIPET_EXP,
"The experience gained by the minipet by eating food increases to [value] 0 times.",

"Reset pet name", eIE_CHANGE_PET_NAME,
"It resets the pet name and allows you to change the name.",

"Leaders' Bells", eIE_SOUND_OF_LEADERS_BELL,
"[Number] Increases all users＊ experience points and item drop rates by [Number] 1 for 0 hours.",

"Hunter Monster Transformation", eIE_TRANSFORMATION_HUNTER,
"Transforms into a level [numerical] 1 hunter for [numerical] 0 minutes.",

"Shoplifter Monster Transformation", eIE_TRANSFORMATION_THIEF,
"Transforms into a level [numerical] 1 petty thief for [numerical] 0 minutes.",

"Wizard Monster Transformation", eIE_TRANSFORMATION_MAGICIAN,
"Transforms into a level [numerical] 1 wizard for [numerical] 0 minutes.",

"Summon a monster", eIE_SUMMON_MONSTER,
"Summons a monster of level 0 or lower.",

"Carving level", eIE_CARVING_LEVEL,
"The imprint level [value] is 0.",

"Increase engraving level", eIE_INCREASE_CARVING_LEVEL,
"Increases the engraving level by 1 to [number] 1 with a [number] 0% probability.",

"Set engraving level", eIE_SET_CARVING_LEVEL,
"The engraving level is set to [numerical value] 1 with a probability of [numerical value] 0%.",

"Engraving option shield", eIE_ACL_SHIELD,
"[Number] 0% chance to reduce received damage by 20 + ([Number] 0 * 1)%.",

"Charge for engraving option", eIE_ACL_CHARGE,
"Restore CP by [value]0 * 3 every 8-([value]0 * 0.1) seconds.",

"Increase in engraving option experience", eIE_ACL_INCREASE_EXP,
"Acquired experience increases by [numerical] 0%.",

"Add fire property damage to the engraving option", eIE_ACL_ADD_FIRE_DAMAGE,
"[Number] 0% chance to add 100 + ([Number] 0 * 200) fire attribute damage.",

"Imprint option increases all stats", eIE_ACL_INCREASE_ALL_STATUS,
" 10 + [Numerical value] 0 * 2 All stats increase.",

"Imprint Option Soul Guard", eIE_ACL_SOUL_GUARD,
"Party member soul guard status for 3 + ([Number] 0 * 0.3) seconds.",


"Engraving option familiar transformation", eIE_ACL_TRANSFORMATION_REPTILE,
"[Number] 0 * 10 level familiar transformed into 1 + [Num.] 0 minutes)",

"Imprint option increases maximum HP", eIE_ACL_INCREASE_MAX_HP,
"Increase maximum health by 100 + [value] 0 * 50",

"Increase attack power of engraving option", eIE_ACL_INCREASE_ATTACK_POWER,
"5 + [numerical value] 0 minute attack power 10 + [numerical value] 0 * 2% increase",

"Imprint Option Hell Prison", eIE_ACL_HELL_PRISON,
"[Number] Use level 0 Hell Prison",

//早Other
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

	"end"																	,0xffff
};

cITEM_GENERATE_EFFECT_INDEX		cITEM_GENERATE_EFFECT::s_aItemEffectIndex[20][60]	=
{

	//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
	//早Basic value
	{
	"Basic values / Minipet",-1,
	"Defense",eIE_ADD_DEFENSE_POWER,
	"Shield Blocking Rate",eIE_BLOCKING_RATE,
	"Number of belt stack items",eIE_BELT_STACK_COUNT,
	"Create prefix",eIE_FREE_RARE_OPTION,
	"Item Piece", eIE_ITEM_PIECE,
	"Secret Dungeon Key", eIE_SECRET_DUNGEON_KEY,

	"Summon Minipet",eIE_RECALL_MINIPET, // Summon Minipet
	"Extend minipet time",eIE_EXTEND_MINIPET_TIME , // Increases minipet time
	"Extended time for all minipets",eIE_EXTEND_ALL_MINIPET_TIME , // Increases the time for all minipets
	"Seal the minipet",eIE_SEAL_MINIPET , // Seal the minipet
	"Change Minipet Name",eIE_CHANGE_MINIPET_NAME , // Change Minipet Name
	"Pocket with minipets",eIE_MINIPET_POUCH , // Pocket with minipets
	"Minipet Food [Biotype]",eIE_FLAKE_OF_DOING_ONE,
	"Minipet food [Energy type]",eIE_FEATHER_OF_THINKING_ONE,
	"Minipet Food [Inorganic Type]",eIE_TOY_OF_SIN_AND_RIN,
	"Minipet double-sided frame",eIE_REVERSIBLE_FRAME,
	"Increased minipet experience gain rate", eIE_INCREASE_GAIN_MINIPET_EXP,
	"Minipet Reflecting Mixer" ,eIE_REFLECTING_MIXER,
	"Minipet Binding Chain" ,eIE_CHAIN_OF_RESTRAINT ,
	"end",0xffff,
	},
	//早Basic value
	//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收


	//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
	//早Temporary ability increase
	{
	"Temporary ability increase",-1,
	"Temporarily increases power (seconds)",eIE_SHORT_WHILE_INCREASE_POWER,
	"Temporarily increases power (minutes)",eIE_SHORT_WHILE_INCREASE_POWER_BY_MINUTE,
	"Temporarily increases agility (seconds)",eIE_SHORT_WHILE_INCREASE_AGILITY,
	"Temporarily increases agility (minutes)",eIE_SHORT_WHILE_INCREASE_AGILITY_BY_MINUTE,
	"Temporarily increases health (seconds)",eIE_SHORT_WHILE_INCREASE_CONSTITUTION,
	"Temporarily increases health (minutes)",eIE_SHORT_WHILE_INCREASE_CONSTITUTION_BY_MINUTE,
	"Temporarily increases wisdom (seconds)",eIE_SHORT_WHILE_INCREASE_WISDOM,
	"Temporarily increases wisdom (minutes)",eIE_SHORT_WHILE_INCREASE_WISDOM_BY_MINUTE,
	"Temporarily increases knowledge (seconds)",eIE_SHORT_WHILE_INCREASE_INTELIGENCE,
	"Temporarily increases knowledge (minutes)",eIE_SHORT_WHILE_INCREASE_INTELIGENCE_BY_MINUTE,
	"Temporarily increases charisma (seconds)",eIE_SHORT_WHILE_INCREASE_CHARISMA,
	"Temporarily increases charisma. (minutes)",eIE_SHORT_WHILE_INCREASE_CHARISMA_BY_MINUTE,
	"Temporarily increases luck (seconds)",eIE_SHORT_WHILE_INCREASE_LUCK,
	"Temporarily increases luck (minutes)",eIE_SHORT_WHILE_INCREASE_LUCK_BY_MINUTE,
	"Temporarily increases luck (x100) (minutes)",eIE_SHORT_WHILE_INCREASE_LUCKx100_BY_MINUTE,

	"Temporarily increases final damage.",eIE_INCREASE_FINAL_DAMAGE,
	"Temporarily increases attack power (seconds)",eIE_SHORT_WHILE_INCREASE_ATTACK_POWER,
	"Temporarily increases attack power (minutes)",eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE,
	"Temporarily increases attack power (minutes) (10% units)",eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE_10,
	"Temporarily increases defense power (seconds)",eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER,
	"Temporarily increases defense power (minutes)",eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE,
	"Temporarily increases defense power (minutes) (10% units)",eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE_10,
	"Temporarily increases stamina (seconds)",eIE_SHORT_WHILE_INCREASE_HP,
	"Temporarily increases stamina (minutes)",eIE_SHORT_WHILE_INCREASE_HP_BY_MINUTE,
	"Temporarily increases CP. (seconds)",eIE_SHORT_WHILE_INCREASE_CP,
	"Temporarily increases CP (minutes)",eIE_SHORT_WHILE_INCREASE_CP_BY_MINUTE,
	"Temporarily maintain maximum weapon damage (seconds)",eIE_SHORT_WHILE_SET_MAX_DAMAGE,
	"Temporarily maintain maximum weapon damage (minutes)",eIE_SHORT_WHILE_SET_MAX_DAMAGE_BY_MINUTE,
	"Temporarily keep CP at max (seconds)",eIE_SHORT_WHILE_SET_MAX_CP,
	"Temporarily keep CP at max (minutes)",eIE_SHORT_WHILE_SET_MAX_CP_BY_MINUTE,
	"end",0xffff,
	},
	//早Temporary ability increase
	//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Other consumable items
{
"Other consumable items",-1,
"Health recovery (numerical value)",eIE_RECOVER_HP,
"Health recovery (value x 100)",eIE_RECOVER_HP2,
"Instant health recovery (value x 100)",eIE_INSTANCE_RECOVER_HPx100 , // Instant health recovery option value * 100
"Health recovery (percentage)",eIE_RECOVER_HP_PERCENTAGE,
"Instant health recovery (percentage)",eIE_INSTANCE_RECOVER_HP_PERCENTAGE , // Instant health recovery
"Instant decrease in remaining health (percentage)",eIE_DECREASE_INSTANT_HP_PERCENTAGE , // Instant decrease in remaining health (percentage)
"CP recharge (numerical value)",eIE_RECOVER_CP,
"CP Charge 2 (numerical value)",eIE_RECOVER_CP2,
"CP Recharge (Percentage)",eIE_RECOVER_CP_PERCENTAGE,
"HP/CP recovery (numerical values)",eIE_RECOVER_HP_CP,
"HP/CP recovery (percentage)",eIE_RECOVER_HP_CP_PERCENTAGE,
"Resurrection of others" ,eIE_RESURRECTION,
"Self Resurrection",eIE_SELF_RESURRECTION,
"Party Completely Revived",eIE_PERFECT_PARTY_REVIVE,
"Neutralize",eIE_HEAL_STRANGE_STATUS,
"Treat abnormal state",eIE_RECOVER_ALL_STRANGE_STATUS,
"Treat lower-world conditions",eIE_RECOVER_ALL_DECLINE_STATUS,
"Cure Curse Condition",eIE_RECOVER_ALL_CURSE_STATUS,
"Cure all status ailments",eIE_RECOVER_ALL_BADSTATUS,
"Addiction Treatment",eIE_HEAL_POSION,
"Open the treasure chest",eIE_OPEN_ARCA,
"Open the door",eIE_OPEN_DOOR,
"Open doors and treasure chests",eIE_OPEN_ARCA_AND_DOOR,
"One Way Portal",eIE_ONE_WAY_PORTAL,
"Trip Portal",eIE_PORTAL,
"Use Jewelry",eIE_USE_JEWEL,
"Memory Stone",eIE_MEMORY_STONE,
"Teleport Stone",eIE_TELEPORT_STONE,
"Portal Stone",eIE_PORTAL_STONE,
"OX Quiz Ticket",eIE_OX_QUIZ_TICKET, // OX Quiz Ticket.
"Charge Magic Power",eIE_CHARGE_EXTRA_ITEM_POWER , // Charge Magic Power
"Book",eIE_BOOK,
// "Add category to book",eIE_ADD_CATEGORY,
"Add page to book",eIE_ADD_PAGE,
"Increased final damage", eIE_INC_LAST_DAMAGE , // Increased final damage
"Increase hunting experience", eIE_INC_HUNT_EXP , // Increase hunting experience
"Increased item drop probability" , eIE_INC_HUNT_ITEM_DROP_CHANCE , // Increased hunting experience
"Recover Death Penalty" , eIE_CLEAR_DEATH_PENELTY , // Recover Death Penalty
"Increase movement speed" , eIE_INCREASE_MOVE_SPEED ,
"Event balloon (experience increase)" , eIE_EVENT_BALLOON , // Event balloon
"Event balloon (item drop rate)" , eIE_EVENT_BALLOON_INC_ITEM_DROP_CHANCE , // Event balloon item drop rate
"Minipet food" , eIE_MINIPET_FEED ,
"Reset Minipet Skill" , eIE_RESET_MINIPET_SKILL ,
"Change minipet type", eIE_SET_MINIPET_TYPE,
"Change health value" ,eIE_CHANGE_HP,
"Move a specific field" ,eIE_MOVE_FIELD,
"end",0xffff,
},
//早Other consumable items
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Attack additional effect/additional damage
{
"Attack additional effect/additional damage",-1,
"Instant death",eIE_INSTANCE_KILL,
"Make target Berserk",eIE_MAKE_BERSERKER,
"Armor Break",eIE_MAKE_ARMOR_BREAK,
"Weapon Destruction",eIE_MAKE_WEAPON_BREAK,
"Poisons.",eIE_MAKE_POISONED,
"Confuse.",eIE_MAKE_CONFUSE,
"Temptation",eIE_MAKE_CHARMING,
"Temptation (30 seconds)",eIE_MAKE_CHARMING_30,
"Stun",eIE_MAKE_STUN,
"Cold",eIE_MAKE_COLD,
"Cold (100%)",eIE_MAKE_COLD_100,
"Fire damage",eIE_HIT_FIRE_DAMAGE,
"Water damage",eIE_HIT_WATER_DAMAGE,
"Wind attribute damage",eIE_HIT_WIND_DAMAGE,
// "Earth attribute damage",eIE_HIT_TERRA_DAMAGE,
"Light attribute damage",eIE_HIT_LIGHT_DAMAGE,
"Dark attribute damage",eIE_HIT_DARK_DAMAGE,
"Undead Hunter",eIE_ADD_DAMAGE_TO_UNDEAD,
"Demon Hunter",eIE_ADD_DAMAGE_TO_DEMON,
"Human Hunter",eIE_ADD_DAMAGE_TO_HUMAN,
"Animal Hunter",eIE_ADD_DAMAGE_TO_ANIMAL,
"Divine Hunter",eIE_ADD_DAMAGE_TO_HOLY_ANIMAL,
"end",0xffff,
},
//早Additional attack effect
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Various additional effects
{
"Various additional effects",-1,
"Reduce the time to fill the belt with items",eIE_DECREASE_RELOAD_TIME,
"Fast transformation speed",eIE_INCREASE_TRANSFORMATION_SPEED,
"Increased recovery speed by potions",eIE_INCREASE_RECOVER_SPEED,
"Absorb physical damage", eIE_AB_PH_DAMAGE,
"thorns",eIE_RETURN_DAMAGE,
"Bristles 40",eIE_RETURN_DAMAGE_40,
"Infinite Bullet",eIE_INFINITY_BULLET,
"Increased health recovery speed",eIE_INCREASE_SITDOWN_RECOVER_HP_SPEED,
"Increases hunting experience", eIE_HUNTING_EXP_BONUS, // Increases the speed of stamina recovery when sitting.

"end",0xffff,
},
//早Various additional effects
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Various numerical values increase
{
"Increased various values",-1,
"Increased attack power of melee jobs",eIE_INCREASE_MELEE_JOB_ATTACK_POWER,
"Increase selected ability level",eIE_INCREASE_SELECT_POWER,
"Power Up",eIE_INCREASE_POWER,
"Wisdom Rises",eIE_INCREASE_WISDON,
"Knowledge Rise",eIE_INCREASE_INTELIGENCE,
"Health Increase",eIE_INCREASE_CONSTITUTION,
"Increased agility",eIE_INCREASE_AGILITY,
"Increased charisma",eIE_INCREASE_CHARISMA,
"Lucky Rise",eIE_INCREASE_LUCK,
"Critical occurrence probability",eIE_INCREASE_CRITICAL,
"Probability of occurrence of final blow",eIE_INCREASE_CRUSH_SHOT,
"Increased hit rate",eIE_INCREASE_HIT_RATE,
"Increased evasion rate",eIE_INCREASE_DODGE_CHANCE,
"Increased concentration",eIE_INCREASE_CONCENTRATION,
"Increased attack speed",eIE_INCREASE_ATTACK_SPEED,
"Increase movement speed",eIE_EQUIP_INCREASE_MOVE_SPEED,
"Increases all stats",eIE_INCREASE_ALL_STATUS,
"Increase selected abilities (fixed)",eIE_INCREASE_FIX_SELECT_POWER,
"Increase maximum health (x 100)",eIE_ADD_MAX_HPx100,
"Increase maximum CP (x 100)",eIE_ADD_MAX_CPx100,
"Increase defense %",eIE_ADD_DEFENSE_PERCENTAGE,
"Increase maximum health %",eIE_ADD_MAX_HP_PERCENTAGE,
"Max CP % increase",eIE_ADD_MAX_CP_PERCENTAGE,
"Increased the level at which red gems can be acquired",eIE_INCREASE_LEVEL_GETING_RED_STONE,
"end",0xffff,
},
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Various resistances
{
"Various resistances",-1,
"Resistance to Curse",eIE_RESISTANCE_CURSE_STATUS,
"Resistance to degradation states",eIE_RESISTANCE_DECLINE_STATUS,
"Resistance to abnormal states",eIE_RESISTANCE_STRANGE_STATUS,
"Resistance to all status ailments",eIE_RESISTANCE_ALL_BAD_STATUS,
"Fire Resistance",eIE_INCREASE_FIRE_RESITANCE,
"Earth Resistance",eIE_INCREASE_TERRA_RESITANCE,
"Wind Resistance",eIE_INCREASE_WIND_RESITANCE,
"Water Resistance",eIE_INCREASE_WATER_RESITANCE,
"Light resistance",eIE_INCREASE_LIGHT_RESITANCE,
"Dark Resistance",eIE_INCREASE_DARK_RESITANCE,
"Resistance to fire, water, wind and earth",eIE_INCREASE_FOUR_ELEMENTAL_RESITANCE,
"Resistance to magical attacks",eIE_INCREASE_MAGIC_RESITANCE,
"NOCK BACK RESISTANCE",eIE_INCREASE_NOCK_BACK_RESISTANCE,
"Critical Resistance",eIE_INCREASE_CRITICAL_RESISTANCE,
"Crushing Blow Resistance",eIE_INCREASE_CRUSH_SHOT_RESISTANCE,
"Instant Kill Resistance",eIE_INCREASE_INSTANCE_KILL_RESISTANCE,
"end",0xffff,
},
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Skill level
{
"Skill level",-1,
"Increased magic spearman skill level",eIE_INCREASE_MAGIC_LANCER_SKILL,
"Increase Magic Archer skill level",eIE_INCREASE_MAGIC_ARCHER_SKILL,
"Increased Apprentice Knight skill level",eIE_INCREASE_KNIGHT_SKILL,
"Warrior series skill level increased",eIE_INCREASE_WARRIOR_SKILL,
"Increase wizard skill level",eIE_INCREASE_WIZARD_SKILL,
"Increased werewolf skill level",eIE_INCREASE_WEREWOLF_SKILL,
"Increased thief skill level",eIE_INCREASE_THIEF_SKILL,
"Increased martial arts skill level",eIE_INCREASE_FIGHTER_SKILL,
"Increased trainer skill level",eIE_INCREASE_BEAST_TAMER_SKILL,
"Increased summoner skill level",eIE_INCREASE_SUMMONER_SKILL,
"Princess skill level increased",eIE_INCREASE_PRINCESSE_SKILL,
"Increased transformation girl skill level",eIE_INCREASE_MAGICAL_GIRL_SKILL,
"Increased priest skill level",eIE_INCREASE_PRIEST_SKILL,
"Increased Fallen Angel skill level",eIE_INCREASE_FALLEN_ANGEL_SKILL,
"Increased Necromancer skill level",eIE_INCREASE_NECROMANCER_SKILL,
"Demon skill level increased",eIE_INCREASE_DEMON_SKILL,
"Increase the level of the Spiritualist skill",eIE_INCREASE_SOUL_BRINGER_SKILL,
"Increased skill level of female characters",eIE_INCREASE_FEMALE_SKILL_LEVEL,
"Increased skill level of male characters",eIE_INCREASE_MALE_SKILL_LEVEL,
"Increase all skill levels",eIE_INCREASE_SKILL_LEVEL,
"end",0xffff,
},
//早Skill level
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Mysterious magic item
{
"Mysterious Magic Item",-1,
"Move through the field using the world map (with cooldown)", eIE_JUMP_FIELD_USE_WORLD_MAP,
"Teleport using the world map (no cooldown)", eIE_USE_WORLD_MAP_TELEPORT , //
"Secret dungeon entry count", eIE_INCREASE_SECRET_DUNGEON_PLAY_COUNT, //09.10.06
"Level Up",eIE_INCREASE_LEVEL,
"2004 Lunar New Year Event",eIE_EVENT_2004_01_21,
"Item box", eIE_LOTTERY_ITEM, // Create guild
"Selectable item box", eIE_SELECT_ITEM_IN_ITEM_PACK, // Create guild
"strength increase",eIE_GROW_STRENGTH,
"Increased agility",eIE_GROW_AGILITY,
"Increased health",eIE_GROW_CONSTITUTION,
"Increased Wisdom",eIE_GROW_WISDOM,
"Increasing knowledge",eIE_GROW_INTELLIGENCE,
"Increase Charisma",eIE_GROW_CHARISMA,
"Increased luck",eIE_GROW_LUCK,
"Increased stamina",eIE_GROW_HP,
"CP Increase",eIE_GROW_CP,
"Increased weapon damage",eIE_INCREASE_WEAPON_DAMAGE,
"Increases armor defense",eIE_INCREASE_ARMOR_DEFENSE,
"Magic Jewel",eIE_MAGIC_JEWEL,
// "Frontier Certificate",eIE_FRONTIER_SCROLL,
"Normal Item Lottery",eIE_NORMAL_ITEM_LOTTERY_TICKET,
"Rare Item Lottery",eIE_RARE_ITEM_LOTTERY_TICKET,
"Unique Item Lottery",eIE_UNIQUE_ITEM_LOTTERY_TICKET,
"Super Item Lottery",eIE_SUPER_ITEM_LOTTERY_TICKET,
"Redistribute skill points",eIE_RESET_SKILL_POINT ,// Reset skill points
"Redistribute stat points",eIE_RESET_STATE_POINT ,// Reset stats
"Redistribute strength stat",eIE_RESET_STRENGTH_STATE , // Redistribute one strength stat
"Redistribute the agility stat",eIE_RESET_AGILITY_STATE , // Redistribute one agility stat
"Redistribute health stat",eIE_RESET_CONSTITUTION_STATE , // Redistribute one health stat
"Redistribute knowledge stat",eIE_RESET_INTELLIGENCE_STATE , // Redistribute one knowledge stat
"Redistribute the wisdom stat",eIE_RESET_WISDOM_STATE , // Redistribute one wisdom stat
"Redistribute charisma stat",eIE_RESET_CHARISMA_STATE , // Redistribute one charisma stat
"Redistribute the luck stat.",eIE_RESET_LUCK_STATE , // Redistribute one luck stat
// "Change prefix",eIE_CHANGE_ITEM_PREFIX_2003_11_01,// Reset stats
// "Dangerous prefix change",eIE_CHANGE_ITEM_PREFIX_2003_11_17,
// "Santa's Orb",eIE_ORB_OF_SANTA,
"Adjust equipment wear level",eIE_OVER_POWER_ITEM_EQUIP,
"Perfect Resurrection",eIE_PERFECT_RESSURECTION,

"Traveler bag" , eIE_EXPAND_INVENTORY_BAG,

"Breeding records", eIE_BREEDING_RECORD,
"Treasure Map", eIE_TREASURE_MAP , // Treasure Map
"Couple Ring Set",eIE_COUPLE_RING_SET,
"Couple Ring", eIE_COUPLE_RING,
"Present Couple Ring",eIE_PRESENT_COUPLE_RING,
"Couple ring without counterpart",eIE_NOT_DEFINE_COUPLE_COUPLE_RING,
"Bottomless box",eIE_MIX_ITEM_BOX , // Bottomless box
"Fireworks Set",eIE_FIRE_WORK,
"Random Fireworks", eIE_RANDOM_FIRE_WORK,
"The princess transformed into a weapon",eIE_PRINCESS_WEAPON,
"Transform to Operator",eIE_TRANS_TO_OPERATOR,
"Reset pet name", eIE_CHANGE_PET_NAME,
"Sealing Sphere of Dawn" ,eIE_SEAL_SPHERE_OF_DAWN, // Sealing Sphere of Dawn

"end",0xffff,
},
//早Mysterious magic item
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Enchant
{
"Enchant & Item Repair",-1,
"Jim Mori's Enchantment Service",eIE_JIM_MORYS_ENCHANT , // Jim Mori's Enchantment
"Clumsy item repair",eIE_UNSKILLED_REPAIR_BROKEN_ITEM ,
"Attach a prefix or upgrade an existing prefix." , eIE_ENCHANT_ITEM_BY_UNIQUE_INFO , // Item enchantment by unique item
"Perfect Item Repair",eIE_REPAIR_BROKEN_ITEM , // "Broken Item Repair",
"Removes the curse that makes it impossible to dismantle equipment",eIE_CURE_ITEM_EQUIP_CURSE, // "Removes the curse attached to the equipment.
"Remove the curse that cannot be enchanted",eIE_RELEASE_ATTACH_PREFIX_CURSE , // "Removes the curse attached to the equipment.
"DX Item Repair", eIE_REPAIR_DX_ITEM_KIT,
"Remove prefix", eIE_REMOVE_PREFIX,

"Attach Prefix", eIE_ATTACH_PREFIX,
"Copy prefix", eIE_COPY_PREFIX,
"Cut prefix", eIE_CUT_PREFIX,
"Attach prefix (cannot trade)" , eIE_ATTACH_PREFIX_CAN_NOT_TRADE,
"Copy prefix (cannot be traded)" , eIE_COPY_PREFIX_CAN_NOT_TRADE,
"Cut prefix (cannot trade)" , eIE_CUT_PREFIX_CAN_NOT_TRADE,

"end",0xffff,
},
//早Enchant
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Guild mark
{
"Guilds & Street Vendors",-1,
"Create guild",eIE_CREATE_GUILD , // Create guild
"Guild level up",eIE_GUILD_LEVEL_UP , // Raises the guild level.
"Redistribution of guild skill points",eIE_RESET_GUILD_SKILL_POINT , // Reset stats
"Increased number of guild dungeon entries", eIE_INCREASE_GUILD_DUNGEON_PLAY_COUNT, // 09.10.06

"Guild Hall Portal Item" ,eIE_GUILD_HALL_PORTAL ,
"Summon Guild Guardian",eIE_SUMMON_GUILD_GUARDIAN,
"Extracting guild statue fragments" ,eIE_GUILD_STATUE_PIECE_EXPORTER,
"Guild Statue Fragment" ,eIE_GUILD_STATUE_PIECE,

"Guild Mark Expansion Pack" ,eIE_GS_EXPAND_PACK ,
"Guild Mark Special Pack",eIE_GS_SPECIAL_PACK,
"Guild Mark Luxury Pack",eIE_GS_LUXURY_PACK,
"Guild Mark Hangul Pack",eIE_GS_HANGUL_PACK,
"Guild Mark English Pack",eIE_GS_ENGLISH_PACK,
"Guild Mark Chinese Characters",eIE_GS_HANMUN_PACK,
"Guild Mark Item Pack",eIE_GS_ITEM_PACK,
"Guild Mark Animal Pack",eIE_GS_ANIMAL_PACK,


"Street vendor sign 1", eIE_PS_SIGNBOARD1,
"Street vendor sign 2", eIE_PS_SIGNBOARD2,
"Street vendor sign 3", eIE_PS_SIGNBOARD3,
"Street vendor sign 4", eIE_PS_SIGNBOARD4,
"Street vendor sign 5", eIE_PS_SIGNBOARD5,
"loudspeaker" , eIE_PS_LOUD_SPEAKER ,

"Street vendor text color",eIE_PS_COLOR_TEXT,
"Bold street vendor text",eIE_PS_BOLD_TEXT,
"Increased street vendor text content",eIE_PS_EXPAND_TEXT_BUFFER,
"Twinkle around street vendor signs",eIE_PS_TWINKLE,
"Five features related to street vendors",eIE_PS_PACK,
"Install Vertical Banner", eIE_PUT_VERTICAL_BANNER , // Horizontal Banner
"Install horizontal banner", eIE_PUT_HORIZON_BANNER, // vertical banner

"end",0xffff,
},
//早Guild mark
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Related to magic damage
{
"Elemental damage correction",-1,
"Weaken target's fire resistance",eIE_WEAKNING_TARGET_FIRE_RESISTANCE ,
"Weaken target's water resistance",eIE_WEAKNING_TARGET_WATER_RESISTANCE ,
"Weaken target's wind resistance",eIE_WEAKNING_TARGET_WIND_RESISTANCE ,
"Weaken target's earth resistance",eIE_WEAKNING_TARGET_EARTH_RESISTANCE ,
"Weaken target's light resistance",eIE_WEAKNING_TARGET_LIGHT_RESISTANCE ,
"Weaken target's dark resistance",eIE_WEAKNING_TARGET_DARK_RESISTANCE ,
"Weaken target's magic resistance",eIE_WEAKNING_TARGET_MAGIC_RESISTANCE ,
"Enhanced fire attribute attack power",eIE_STRENGTHENING_FIRE_DAMAGE,
"Enhanced water attribute attack power",eIE_STRENGTHENING_WATER_DAMAGE,
"Enhanced wind attribute attack power",eIE_STRENGTHENING_WIND_DAMAGE,
"Enhanced earth attribute attack power",eIE_STRENGTHENING_EARTH_DAMAGE,
"Enhanced light attribute attack power",eIE_STRENGTHENING_LIGHT_DAMAGE,
"Enhanced dark attribute attack power",eIE_STRENGTHENING_DARK_DAMAGE,
"Enhanced magic attack power",eIE_STRENGTHENING_MAGIC_DAMAGE,
"end",0xffff,
},
//早Related to magic damage
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Chapter 5 Compensation Related
{
"Chapter 5 Compensation Related",-1,
"Chapter 5: Title Level Increase" ,eIE_CH5_SELECT_UPGRADE_TITLE,
"Chapter 5: Tradeable",eIE_CH5_RELASE_REVERSION,
"Chapter 5: Guild Skill Reset",eIE_CH5_RESET_GUILD_SKILL_POINT,
"Chapter 5: Health Increase Amulet",eIE_CH5_BONUS_HP_AMULET,
"Chapter 5: CP Increase Amulet",eIE_CH5_BONUS_CP_AMULET,
"Chapter 5: Equip Limit Level Increase Talisman",eIE_CH5_OVER_POWER_ITEM_EQUIP,
"Chapter 5: Reverse Skill",eIE_CH5_REVERSE_SKILL_POINT,
"Chapter 5: Stat Reverse",eIE_CH5_REVERSE_STATE_POINT,
"Chapter 5: Prefix Removal",eIE_CH5_REMOVE_PREFIX,
"Chapter 5: Copy Items",eIE_CH5_COPY_ITEM,
"Chapter 5: Upgrading Normal Item Prefixes",eIE_CH5_UPGRADE_NORMAL_ITEM_PREFIX_POWER,
"Chapter 5: Improving Normal Item Performance",eIE_CH5_IMPROVE_NORMAL_ITEM_POWER,
"Chapter 5: Unique Item Prefix Upgrade",eIE_CH5_UPGRADE_UNIQUE_ITEM_PREFIX_POWER,
"Chapter 5: Improving Unique Item Performance",eIE_CH5_IMPROVE_UNIQUE_ITEM_POWER,
"Chapter 5: Red Bottomless Box",eIE_CH5_MIX_ITEM_BOX,
"Chapter 5: Select propensity values",eIE_CH5_SELECT_ALLIGNMENT,
"end",0xffff,
},
//早Chapter 5 Compensation Related
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收
//早Other
{
"Other",-1,
"Time Stamp" ,eIE_TIME_STAMP,
"Add title" ,eIE_ADD_TITLE, // Add title
"Setting Badge" ,eIE_SET_BADGE,
"Setting Rune" ,eIE_SET_RUNE,
"Seongmyeongseok" ,eIE_NAME_STONE,
"Change the shape of the magic carpet",eIE_CHANGE_MAGIC_CARPET_SHAPE , // Change the shape of the magic carpet.
"Summon a Magic Carpet",eIE_CALL_MAGIC_CARPET,
"Summon a magic carpet that gradually restores health",eIE_REGEN_HP_ON_MAGIC_CARPET,
"Summon a magic carpet whose CP does not decrease when moved",eIE_NOT_DOWN_CP_ON_MAGIC_CARPET,
"Summon a carpet faster than the existing carpet",eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET,
"Summon the Red Drake Carpet",eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET_RED,
"Summon a carpet that is slower than existing carpets but can fight",eIE_FIGHT_MAGIC_CARPET,
"Duel buff: Fire", eIE_DUEL_BUFF_FIRE , // Duel buff fire
"Duel buff: Water", eIE_DUEL_BUFF_WATER , // Duel buff fire
"Duel buff: Wind", eIE_DUEL_BUFF_WIND , // Duel buff fire
"Duel buff: Earth", eIE_DUEL_BUFF_EARTH, // Duel buff fire
"Minipet awakening: 50%" , eIE_MINIPET_AWAKEN50 , // Minipet awakening 50%
"Minipet awakening: 100%" , eIE_MINIPET_AWAKEN100 , // Minipet awakening 100%

"Guild Mark Signal Pack",eIE_GS_SIGNAL_PACK,
"Guild Mark Unique 1 Pack",eIE_GS_UNIQUE1_PACK,
"Guild Mark Unique 2 Pack",eIE_GS_UNIQUE2_PACK,

"Magic Spellbook",eIE_CASTING_MAGIC,
"Leaders' Bells", eIE_SOUND_OF_LEADERS_BELL,

"Hunter Monster Transformation", eIE_TRANSFORMATION_HUNTER,
"Shoplifter Monster Transformation", eIE_TRANSFORMATION_THIEF,
"Wizard Monster Transformation", eIE_TRANSFORMATION_MAGICIAN,
"Summon a monster", eIE_SUMMON_MONSTER,
"Carving level", eIE_CARVING_LEVEL,
"Increase engraving level", eIE_INCREASE_CARVING_LEVEL,
"Set engraving level", eIE_SET_CARVING_LEVEL,
"Engraving option shield", eIE_ACL_SHIELD,
"Charge for engraving option", eIE_ACL_CHARGE,
"Increase in engraving option experience", eIE_ACL_INCREASE_EXP,
"Add fire property damage to the engraving option", eIE_ACL_ADD_FIRE_DAMAGE,
"Imprint option increases all stats", eIE_ACL_INCREASE_ALL_STATUS ,
"Imprint Option Soul Guard", eIE_ACL_SOUL_GUARD,
"Engraving Option Familiar Transformation", eIE_ACL_TRANSFORMATION_REPTILE,
"Imprint option increases maximum HP", eIE_ACL_INCREASE_MAX_HP,
"Increase attack power of engraving option", eIE_ACL_INCREASE_ATTACK_POWER,
"Imprint Option Hell Prison", eIE_ACL_HELL_PRISON,
"end",0xffff,
},
//早Other
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 收收收收收收收收 收收收收收收收收收收收收收收收收收收收

{
"End",0xffff,
}
};

//
// Get the index of item effects. (See serial)
int
cITEM_GENERATE_EFFECT::GetIndex(int serial)
{
	int	iIndex	=	0;

	while(1)
	{
		if	(s_aHyperText[iIndex].m_iSerial	==	0xffff)	break;
		if	(s_aHyperText[iIndex].m_iSerial	==	serial)	return	iIndex;

		iIndex++;
	}
	return	0xffff;
}

//
//	
char*
cITEM_GENERATE_EFFECT::GetCustomComboText(int kind,int data)
{
	CheckJongsung(s_strComboStr[kind][data]);

	return	s_strComboStr[kind][data];
}

//
//	晦顫蛔蛔檜  轎 腎朝 巍爾 夢蝶
int
cITEM_GENERATE_EFFECT::CustomComboBox(HWND hwnd,int x,int y,int kind,int select)
{
	int	iCounter=	0;
	int	iWidth	=	0;

	while(1)
	{
		if (strcmp(s_strComboStr[kind][iCounter],"end")	==	0)
			break;

		int	width	=	strlen(s_strComboStr[kind][iCounter])*6 + 30;

		if (width > iWidth)	iWidth	=	width;

		iCounter++;
	}

	s_aCombo[s_iComboIndex].Init(hwnd,MAIN.hINST,IDC_ITEM_COMBO	+	s_iComboIndex,x,y-4,iWidth,400,CBS_DROPDOWNLIST);

	iCounter	=	0;

	while(1)
	{	if (strcmp(s_strComboStr[kind][iCounter],"end")	==	0)	break;

		int	index	=	s_aCombo[s_iComboIndex].Add(s_strComboStr[kind][iCounter]);

		s_aCombo[s_iComboIndex].SetData(index,iCounter);

		iCounter++;
	}

	if(select >= iCounter	||	select	<	0)	select	=	0;

	s_aCombo[s_iComboIndex].Select(select);

	CheckJongsung(s_strComboStr[kind][select]);

	s_iComboIndex++;

	return	iWidth+2;
}

//
//	_iEffect陛 樓п 氈朝 堅葬 橫 .
int
cITEM_GENERATE_EFFECT::GetCategory(int _iEffect)
{
	for (int i=0;;i++)
	{
		int	iEffect	=	1;

		while(1)
		{
			if (s_aItemEffectIndex[i][iEffect].m_iSerial	==	0xffff	)	break;

			if (s_aItemEffectIndex[i][iEffect].m_iSerial	==	_iEffect)	return	i;

			iEffect++;
		}
	}

	return	0xffff;
}

//
//	檜蠱  橫 .(衛葬橡 褻)
cITEM_GENERATE_EFFECT*
cITEM_GENERATE_EFFECT::Get(int serial)
{
	int	iIndex	=	0;

	while(1)
	{
		if	(s_aHyperText[iIndex].m_iSerial	==	0xffff)
			break;
		if	(s_aHyperText[iIndex].m_iSerial	==	serial)
			return	&s_aHyperText[iIndex];

		iIndex++;
	}

	return	NULL;
}

//
//	 陛 衛鑑.
BOOL
cITEM_GENERATE_EFFECT::AddEffect(HWND hwnd,cBASIC_ITEM *_lpItem,cITEM_GENERATE_EFFECT *lpEffect)
{
	s_aValue[0][0]	=	_lpItem->m_aValue[0][0];
	s_aValue[0][1]	=	_lpItem->m_aValue[0][1];
	s_aValue[1][0]	=	_lpItem->m_aValue[1][0];
	s_aValue[1][1]	=	_lpItem->m_aValue[1][1];

	strcpy(s_strComboStr[0][0],_ms("%d at %d",s_aValue[0][0],s_aValue[0][1]));
	strcpy(s_strComboStr[0][1],_ms("%d at %d",s_aValue[1][0],s_aValue[1][1]));

	if (lpEffect)
	{
		memcpy(&s_itemData,lpEffect,sizeof(s_itemData));

		int	iCategory	=	GetCategory(s_itemData.m_wEffect);

		if (iCategory	!=	0xffff)
		{
			s_iSelectEffectCategory	=	iCategory;
			s_iSelectEffect			=	s_itemData.m_wEffect;
		}
		else
		{
			memset(&s_itemData,0,sizeof(s_itemData));
			s_itemData.m_wEffect	=	s_iSelectEffect;
		}
	}
	else
	{
		memset(&s_itemData,0,sizeof(s_itemData));
		s_itemData.m_wEffect	=	s_iSelectEffect;
	}

	memset(s_strFilter,0,sizeof(s_strFilter));

	int	result	=	DialogBox( MAIN.hINST,"SET_ITEM_EFFECT",hwnd,(DLGPROC)s_procItemEffect);

	return	result;
}


//////////////////////////////////////////////////////////////////////////////////////////////
//	檜蠱  撲薑
//////////////////////////////////////////////////////////////////////////////////////////////

//
//	檜蠱  渠食睍 蟾晦
void
cITEM_GENERATE_EFFECT::InitItemEffectDlg(HWND hDlg)
{
	s_hDlg	=	hDlg;

	HWND	hTree			=	GetDlgItem(hDlg,IDC_TREE);

	s_treeView.Init(hTree,MAIN.hINST,NULL);

	FillTree();

	WriteItemEffectText(hDlg,&s_itemData);
}

int
cITEM_GENERATE_EFFECT::GetEffectCount(int _iCategory)
{
	int	iCount	=	0;

	for (int iSlot=1;;iSlot++)
	{
		int	iSerial	=	s_aItemEffectIndex[_iCategory][iSlot].m_iSerial;

		if	(iSerial	==	0xffff)
			break;

		char	*lpComment	=	s_aItemEffectIndex[_iCategory][iSlot].m_strName;

		if	(s_strFilter[0] && strstr(lpComment,s_strFilter)	==	NULL)
			continue;

		iCount++;
	}

	return	iCount;
}

//	お葬 頂辨 瓣辦晦
void
cITEM_GENERATE_EFFECT::FillTree()
{
	s_treeView.Clear();

	for (int iCategory=0;;iCategory++)
	{
		char	*lpstrCategory	=	s_aItemEffectIndex[iCategory][0].m_strName;

		if	(s_aItemEffectIndex[iCategory][0].m_iSerial	==	0xffff)
			break;

		if	(s_strFilter[0] && GetEffectCount(iCategory)	==	NULL)
			continue;

		HTREEITEM	hParents	=	s_treeView.Add(lpstrCategory,-1);

		for (int iSlot=1;;iSlot++)
		{
			int	iSerial	=	s_aItemEffectIndex[iCategory][iSlot].m_iSerial;

			if	(iSerial	==	0xffff)
				break;

			char	*lpComment	=	s_aItemEffectIndex[iCategory][iSlot].m_strName;

			if	(s_strFilter[0] && strstr(lpComment,s_strFilter)	==	NULL)
				continue;

			int		iParam	=	(iCategory<<16)+iSerial;

			HTREEITEM	hCurrent	=	s_treeView.Add(hParents,lpComment,iParam);

			if	(iSerial	==	s_iSelectEffect)
				s_treeView.Select(hCurrent);
		}

		if	(s_strFilter[0] || iCategory == s_iSelectEffectCategory)
		{
			s_treeView.Expand(hParents);
		}
	}
}

//
//	謙擊 羹 и
void
cITEM_GENERATE_EFFECT::CheckJongsung(char *str)
{
	s_isJongSung	=	FALSE;

	int		length	=	strlen(str);

	if (length < 2)	return;

	if ((BYTE)str[length-2]	>=	0xb0	&&	(BYTE)str[length-1]	>=	0xa1)
		if (IsJongsung(str+length-2))	s_isJongSung	=	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////
//	巍爾 夢蝶..
//////////////////////////////////////////////////////////////////////////////////////

//
//	擎朝檜陛擊 擊 摹鷗
void
cITEM_GENERATE_EFFECT::TransCode(char *lpStream)
{
	char	*strCheckKarmaJongsung[]	=	{"is ","do ","~ga ","ga ","go ","wo ","end"};

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
			if (s_isJongSung)	//	謙檜 氈朝等..
			{
				if (i%2)		//	謙檜 橈擊陽 朝 僥濠.
				{
					lpStream[0]	=	strCheckKarmaJongsung[i-1][0];
					lpStream[1]	=	strCheckKarmaJongsung[i-1][1];
				}
			}
			else
			{
				if (i%2 == 0)	//	謙檜 氈擊陽 朝 僥濠.
				{
					lpStream[0]	=	strCheckKarmaJongsung[i+1][0];
					lpStream[1]	=	strCheckKarmaJongsung[i+1][1];
				}
			}

			return;
		}
}


//
//	檜蠱  檗朝.
BOOL
cITEM_GENERATE_EFFECT::ReadEffectData(HWND hwnd,cITEM_GENERATE_DATA *lpItemData)
{
	int		iIndex	=	GetIndex(lpItemData->m_wEffect);
	char	*lpText	=	s_aHyperText[iIndex].m_strReferenceText;
	WORD	*lpData	=	lpItemData->m_aValue;

	char	*lpStream,*lpKeyWord;
	int		iCode;

	s_iComboIndex		=	0;
	s_iSpinIndex		=	0;
	s_iSpinBoxIndex		=	0;

	int	iValueIndex		=	0;

	s_Reader.UploadBuffer(lpText,s_aKeyword);

	while(1)
	{
		if (s_Reader.m_isDone	)	break;		//	 檗橫 .

		lpStream	=	s_Reader.GetStream('[');

		if (iValueIndex	>=	0	)	TransCode(lpStream);
		if (s_Reader.m_isDone	)	break;		//	 檗橫 .

		//	巍爾 夢蝶 該 斜剪..
		lpKeyWord	=	s_Reader.GetStream(']');
		iValueIndex	=	s_Reader.GetNumber();

		iCode		=	s_Reader.AnalCode(lpKeyWord);

		switch(iCode)
		{
			case	0			:
				lpData[iValueIndex]	=	s_aCombo[s_iComboIndex].GetData();
				s_iComboIndex++;
				break;

			default					:
//				ERRMSG("error","what is it?? [%s]",lpKeyWord);
				break;
		}

	}

	return	TRUE;
}

//
//	檜蠱 睡陛 蝶お 斜.
BOOL
cITEM_GENERATE_EFFECT::WriteItemEffectText(HWND hwnd,cITEM_GENERATE_DATA *lpItemData)
{
	int		iIndex		=	GetIndex(lpItemData->m_wEffect);
	char	*lpText		=	s_aHyperText[iIndex].m_strReferenceText;
	WORD	*lpValue	=	lpItemData->m_aValue;

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
		if (s_Reader.m_isDone	)
			break;		//	 檗橫 .

		lpStream	=	s_Reader.GetStream('[');

		
		if (iValueIndex	>=0	)
			TransCode(lpStream);

		//	蝶す 僥 轎
		if	(lpStream	)
			x	+=	s_aStream[s_iStreamIndex++].Init(hwnd,MAIN.hINST,lpStream,x,y,0,12);
		if	(s_Reader.m_isDone	)
			break;		//	 檗橫 .

		//	巍爾 夢蝶 該 斜剪..
		lpKeyWord	=	s_Reader.GetStream(']');
		iCode		=	s_Reader.AnalCode(lpKeyWord);
		iValueIndex	=	s_Reader.GetNumber();

		switch(iCode)
		{
			case	0		:
				x	+=	CustomComboBox(hwnd,x,y,iCode,lpValue[iValueIndex]);
				break;

			default							:
				ERRMSG("error","what is it?? [%s]",lpKeyWord);
				break;
		}
	}

	return	TRUE;
}	//	cITEM_GENERATE_EFFECT::WriteItemEffectText(HWND hwnd,char *lpText,short *lpValue)

//
//	檜蠱 睡陛  撲
char*
cITEM_GENERATE_EFFECT::GetComment(cITEM_GENERATE_DATA *_lpData,cBASIC_ITEM *_lpItem,BOOL _bSpecial,int _iValue1,int _iValue2)
{
	int		iIndex	=	GetIndex(_lpData->m_wEffect);
	char	*lpText	=	s_aHyperText[iIndex].m_strReferenceText;
	WORD	*lpValue=	_lpData->m_aValue;

	s_aValue[0][0]	=	_lpItem->m_aValue[0][0];
	s_aValue[0][1]	=	_lpItem->m_aValue[0][1];
	s_aValue[1][0]	=	_lpItem->m_aValue[1][0];
	s_aValue[1][1]	=	_lpItem->m_aValue[1][1];

	if (_iValue1	!=	-1)
		strcpy(s_strComboStr[0][0],_ms("%d",_iValue1));
	else
	if (s_aValue[0][0]	!=	s_aValue[0][1])
		strcpy(s_strComboStr[0][0],_ms("%d at %d",s_aValue[0][0],s_aValue[0][1]));
	else
		strcpy(s_strComboStr[0][0],_ms("%d",s_aValue[0][0]));
		
	if (_iValue2	!=	-1)
		strcpy(s_strComboStr[0][1],_ms("%d",_iValue2));
	else
	if (s_aValue[1][0]	!=	s_aValue[1][1])
		strcpy(s_strComboStr[0][1],_ms("%d at %d",s_aValue[1][0],s_aValue[1][1]));
	else
		strcpy(s_strComboStr[0][1],_ms("%d",s_aValue[1][0]));


	

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
		if (s_Reader.m_isDone	)	break;		//	 檗橫 .

		lpStream	=	s_Reader.GetStream('[');

		if (iValueIndex	>=0	)	TransCode(lpStream);
		Text.Add(lpStream);

		if (s_Reader.m_isDone	)	break;		//	 檗橫 .

		//	巍爾 夢蝶 該 斜剪..
		lpKeyWord	=	s_Reader.GetStream(']');
		iValueIndex	=	s_Reader.GetNumber();

		iCode		=	s_Reader.AnalCode(lpKeyWord);

		if (_bSpecial)	Text.Add(_ms("%c",1));

		char	*str	=	NULL;

		switch(iCode)
		{
			case	0		:
				str	=	GetCustomComboText(iCode,lpValue[iValueIndex]);
				break;

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
}


LRESULT CALLBACK 
cITEM_GENERATE_EFFECT::s_procItemEffect(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{	
	switch(message)
	{
		case WM_INITDIALOG	:
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

						memset(&s_itemData,0,sizeof(s_itemData));

						s_itemData.m_wEffect		=	s_iSelectEffect;

						WriteItemEffectText(s_hDlg,&s_itemData);

						break;
					}
				}
			}

			break;
		}
		case WM_COMMAND :
		{
			switch( LOWORD( wParam ) )
			{
				case	IDOK				:
					ReadEffectData(hDlg,&s_itemData);
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
