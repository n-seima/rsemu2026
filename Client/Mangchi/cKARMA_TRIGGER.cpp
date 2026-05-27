#include "cKARMA.H"
#include "cMESSAGE.H"
#include "resource.H"
#include "cMAP.H"
#include "secretDungeon.H"

int				g_iSelectTrigger		=	0;
int				g_iLastTrigger			=	0;
int				g_iEditTrigger			=	0;
int				g_iLastestTriggerTerm	=	1;
CKarmaItem		g_EditTrigger;

int				l_iTriggerPos;

cKARMA_ITEM		g_aTrigger[200]	=
{
"----Matters related to the player himself",0x7fff,
" Matters related to the player himself [border_comment]0" ,

"Level check",eTRIGGER_LEVEL_CHECK,
"The level is [spin]0[1,1000]~[spin]1[1,1000]." ,

"Guild membership" ,eTRIGGER_IS_GUILD_MEMBER,
"Guild [guild_member]0",

"Guild rank" ,eTRIGGER_IS_RANK_IN_GUILD,
"Guild rank is [rank_in_guild]0 [ida_anida]1",


"Number of empty item slots",eTRIGGER_REMAIN_ITEM_SLOT_COUNT,
"The number of remaining item slots is [spin]0[0,255] [equal]1",

  "Check item option" ,eTRIGGER_CHECK_ITEM_BOOLEAN_OPTION,
" Items with option [item_boolean_option_list]0 in inventory are [exist_not_exist]1 target[target_player]2" ,// Free option

"Whether you own an item" ,eTRIGGER_OWN_ITEM,
"The number of [item]0 holds is [spin]1[0,1024] [equal]2. [ignore_include_equipmment]3" ,

"Whether you have a special item" ,eTRIGGER_OWN_SPECIAL_ITEM,
"The number of [special_item]0 held is [spin]1[0,255] [equal]2",

"Whether you have a quest item" ,eTRIGGER_OWN_QUEST_ITEM,
"The number of [quest_item]0 held is [spin]1[0,255] and [equal]2",

"Whether to equip an item" ,eTRIGGER_EQUIP_ITEM,
" [equipment]0 to [item_equip]1" ,

"Gold" ,eTRIGGER_OWN_GOLD,
"The amount held is [number]0[0,2147483647]gold [equal]1 [by_level]2" ,

"Skill possession" ,eTRIGGER_SKILL_LEVEL ,
"The level of skill [skill]0 is [spin]1[0,50] [equal]2",

"Number of skills mastered" ,eTRIGGER_MASTER_SKILL_COUNT,
"The number of skills mastered is [spin]0[0,60] [equal]1",

"Check whether the operator is" ,eTRIGGER_CHECK_OPERATOR,
" [check_operator]0" ,

"Whether you have a title" ,eTRIGGER_TITLE_LEVEL ,
"The level of title [title]0 is [spin]1[0,10] [equal]2" ,

"Job check" ,eTRIGGER_CHECK_JOB,
"Job [player_job]0 [ida_anida]1. [include_transformation_job]2" ,

"Check the number of past lives",eTRIGGER_CHECK_REBIRTH_COUNT,
"The number of previous lives is [spin]0[0,30] times [equal]1",

// "Job level" ,eTRIGGER_CHECK_JOB_LEVEL,
// "Job level is [spin]0[1,3] [equal]1",

"Inclination" ,eTRIGGER_CHECK_ALLIGNMENT ,
"Alignment is [alignment]0." ,

"Play Time" ,eTRIGGER_CHECK_PLAY_TIME ,
" Play time is [number]0[0,2147483647] hours [equal]1" ,

" ability level " ,eTRIGGER_CHECK_BASIC_POWER ,
" [power]0 is [spin]1[0,10000] [equal]2" ,

"Check the completeness of your books" ,eTRIGGER_CHECK_BOOK_COMPLETE_PERCENTAGE,
"The completeness of [book]0 is [spin]1[0,100]% [equal]2 (check all books - if there are no books, the check unconditionally fails)",

"Check the books you own" ,eTRIGGER_CHECK_EXIST_BOOK,
" [book]0 is [exist_not_exist]1" ,

"Check a specific page in a book you own" ,eTRIGGER_CHECK_BOOK_PAGE,
"Book [book]0, category [spin]1[1,18], page [spin]2[1,8] are [exist_not_exist]3. (Check all books)",

"Check the item that adds a specific page to the book" ,eTRIGGER_CHECK_OWN_ADD_BOOK_PAGE_ITEM,
"The item that adds page [spin]2[1,8] to category [spin]1[1,18] in book [book]0 is [exist_not_exist]3." ,

"Check current status" ,eTRIGGER_CHECK_CURRENT_STATUS,
" [current_status]0 [ida_anida]1" ,

"Sex check" ,eTRIGGER_CHECK_SEX,
" Player job gender is [sex]0" ,// Gender check

"Attendance check" ,eTRIGGER_CHECK_ATTENDANCE,
"Today's attendance check for [item]0 item [quest_complete]1." ,//	attendance check

"Inclination check" ,eTRIGGER_CHECK_ATTR,
"Player's tendency is [plus_number_minus_number]0 [spin]1[0,100] [equal]2" ,// Attendance check

"Can accept Chapter 5 title bonus" ,eTRIGGER_CHECK_CAN_RECEIVE_CH5REWARD_TITLE_BONUS ,
"You can receive the title bonus in Chapter 5 [exist_not_exist]0",

"Chapter 5 bonus guild points can be obtained" ,eTRIGGER_CHECK_ENABLE_INCREASE_BONUS_GUILD_POINT ,
"You can receive bonus guild points for Chapter 5 [exist_not_exist]0",

"Chapter 5 guild statue level can be increased." ,eTRIGGER_CHECK_ENABLE_INCREASE_GUILD_STATUE_LEVEL ,
"Chapter 5 guild statue level can be increased [exist_not_exist]0",
	
		
"Chapter 5 Number of times the heavenly world bonus was received" ,eTRIGGER_CHECK_CH5_HEAVEN_BONUS_COUNT,
"The number of times the heavenly bonus [heaven_bonus]0 [bonus_penelty]3 was received is [spin]1[0,10] [equal]2 (stats and skills do not check the number of penalties)",

"Chapter 5 Equip Level Increase Amulet Check", eTRIGGER_CHECK_SAME_LEVEL_OVER_LEVEL_EQUIP_AMULET ,//
" At the current Red Gem Hunter level, no one has received the Equip Level Increase Talisman [exist_not_exist]0",

"Chapter 5 Health Increase Amulet Check", eTRIGGER_CHECK_SAME_LEVEL_INCREASE_HP_AMULET ,//
"The current stamina increase talisman of the same level as the red gem hunter is [exist_not_exist]0",

"Chapter 5 CP Increase Amulet Check", eTRIGGER_CHECK_SAME_LEVEL_INCREASE_CP_AMULET,//
"The CP increase talisman of the same level as the current Red Gem Hunter is [exist_not_exist]0",

"Check whether a pet can be added", eTRIGGER_CHECK_ADD_ABLE_PET,//
"You can add a pet [exist_not_exist]0",

"Check the number of current pets", eTRIGGER_CHECK_ACTIVE_PET_COUNT,//
"The number of currently summoned pets is [spin]0[0,2] [equal]1",

" Check duel ranking", eTRIGGER_CHECK_DUEL_RANKING,//
"The duel rank is between [spin]0[1,100] and [spin]1[1,100] etc. [ida_anida]2",

"HAVE GRACE CHECK", eTRIGGER_CHECK_HAVE_GRACE,//
"Take player has grace and [quest_own]0" , //quest_own, but don't get caught. "I have it or I don’t have it."

"Blind (permanent) check", eTRIGGER_CHECK_IS_BLIND_NEW,//
"Take player is blind [exist]0",

" Check target player action status", eTRIGGER_CHECK_PLAYER_ACTION,//
" Current player action is [ida_anida]1 out of [action]0" ,

"Check the number of secret dungeons",eTRIGGER_CHECK_SECRET_DUNGEON_PLAY_COUNT,
"The number of secret dungeons is [spin]0[0,30] times [equal]1",


"----Matters related to the party you belong to" ,0x7fff,
" Matters related to the party you belong to[border_comment]0",

"Number of party members" ,eTRIGGER_PARTY_MEMBER_COUNT,
" Party members are [spin]0[0,8] [party_member_equal]1" ,

"Party Leader" ,eTRIGGER_IS_PARTY_LEADER ,
" [is_party_leader]0" ,

"Location of party members" ,eTRIGGER_CHECK_PARTY_MEMBER_LOCATION,
"All party members are [party_member_location]0",

"Party member level range" ,eTRIGGER_CHECK_PARTY_MEMBER_LEVEL,
"The level of the party members is [spin]0[1,1000]~[spin]1[1,1000]." ,

"Party member’s guild affiliation" ,eTRIGGER_ALL_PARTY_MEMBER_IN_GUILD,
"All party members are in the guild [guild_member]0",

"Party member’s guild affiliation" ,eTRIGGER_IN_GUILD_MEMBER_EXIST_IN_PARTY,
"Party members belonging to the guild are [exist_not_exist]0",

"Party member’s guild affiliation" ,eTRIGGER_ALL_PARTY_MEMBER_IN_SAME_GUILD,
"All party members are [all_party_member_in_same_guild]0",

"Whether a party member has an item" ,eTRIGGER_ALL_PARTY_MEMBER_OWN_SPECIFIC_ITEM,
"All party members have more than [spin]1[1,255] [item]0." ,

"Whether or not a party member owns an item" ,eTRIGGER_ALL_PARTY_MEMBER_NOT_OWN_SPECIFIC_ITEM ,
"Not all party members have [item]0." ,

"Whether a party member has an item" ,eTRIGGER_OWN_SPECIFIC_ITEM_MEMBER_EXIST_IN_PARTY,
"Party members with more than [spin]1[1,255] [item]0 are [specific_item_own_member_exist]2",

"Sum of party members' possessions" ,eTRIGGER_GOLD_SUM_OF_PARTY,
"The total amount of money held by party members is [number]0[1,2147483647]gold [equal]1",

"Amount held by party members" ,eTRIGGER_CHECK_GOLD_IN_PARTY_MEMBER,
" Party members with more than [number]0[1,2147483647] gold are [exist]1" ,

"Amount held by all party members" ,eTRIGGER_CHECK_GOLD_IN_ALL_PARTY_MEMBER ,
"All party members possess more than [number]0[1,2147483647] gold." ,

"Whether a party member has a skill" ,eTRIGGER_CHECK_SKILL_IN_PARTY_MEMBER,
"Party members with level [spin]0[0,50] or higher and [skill]1 are [exist]2",

" Whether all party members have skills" ,eTRIGGER_ALL_PARTY_MEMBER_LEARNED_SSPECIFIC_SKILL ,
"All party members are learning [skill]1 at level [spin]0[0,50] or higher." ,

"Whether the party member has a title" ,eTRIGGER_CHECK_TITLE_IN_PARTY_MEMBER,
" Party members with [title]1 title of level [spin]0[1,10] or higher are [exist]2" ,

" Whether all party members have titles" ,eTRIGGER_OWNED_TITLE_BY_ALL_PATY_MEMBER ,
" All party members have [title]1 title [own]2 of level [spin]0[1,10] or higher",

"Party member's job" ,eTRIGGER_CHECK_JOB_IN_PARTY,
"Party member with job [player_job]0 is [exist]1. [include_transformation_job]2",

// "Job levels held by party members" ,eTRIGGER_CHECK_ALL_PARTY_MEMBER_JOB,
// "Party members with job level [spin]0[1,3] are [exist]1",

"All party members' jobs" ,eTRIGGER_CHECK_JOB_LEVEL_IN_ALL_PARTY_MEMBER ,
"The job of all party members is [player_job]0. [include_transformation_job]1",

"Alignment of party members" ,eTRIGGER_CHECK_ALLIGNMENT_IN_PARTY_MEMBER ,
"Party members with alignment [alignment] 0 are [exist] 1",

"Alignment of all party members" ,eTRIGGER_CHECK_ALLIGNMENT_IN_ALL_PARTY_MEMBER,
"All party members have [alignment] 0." ,

"Party member's abilities" ,eTRIGGER_CHECK_BASKC_POWER_IN_PARTY_MEMBER ,
"Party members with [power]0 of [spin]1[1,10000] or higher are [exist]2",

" Abilities of all party members", eTRIGGER_CHECK_BASKC_POWER_IN_ALL_PARTY_MEMBER,
"All party members' [power]0 is [spin]1[1,10000] [equal]2",

"Check the number of skills mastered by some members", eTRIGGER_HAVE_MASTER_SKILL_PARTY_MEMBER,
"The number of party members who have mastered more than [spin]0[0,60] skills is [spin]1[0,8] [equal]2",

"Check the number of skills mastered by all members", eTRIGGERCHECK_ALL_PARTY_MEMBERS_MASTER_SKILL_COUNT,
"All party members have mastered [spin]0[0,60] skills and [only_equal]1 skills.",

"S party members wiped out", eTRIGGER_CHECK_ALL_PARTY_MEMBERS_LIFE,
"There are no party members alive in Bidun. (Bidun-only trigger)",

"Check party member gender", eTRIGGER_CHECK_MEMBER_SEX,// Check member gender
"There are [spin]1[0,8] party members with occupations of [sex]0 and [equal]2",

"Check party member highest and lowest level", eTRIGGER_CHECK_LEVEL_RANGE,// Difference between highest and lowest level
"The level difference between the highest level party member and the lowest level party member in the party is [spin]0[0,1000] [equal]1",

" Current number of field party members", eTRIGGER_PARTY_MEMBER_COUNT_IN_CURRENT_FIELD,
"The number of party members currently on the field is [spin]0[0,8] [equal]1",

"----Matters related to your guild", 0x7fff,
" Matters related to your guild[border_comment]0",

"Affiliated guild level", eTRIGGER_CHECK_ATTACED_GUILD_LEVEL,
"The level of your guild is [spin]0[1,10000] [equal]1",

"Number of guild members in the guild you belong to", eTRIGGER_CHECK_MEMBER_COUNT_IN_ATTACED_GUILD,
"The number of guild members in the affiliated guild is [spin]0[1,10000] [equal]1",

"Number of guild submasters", eTRIGGER_CHECK_SUB_GUILD_MASTER_COUNT_IN_ATTACED_GUILD,
"The number of submasters in the affiliated guild is [spin]0[1,10000] [equal]1",

"Number of elder guild members belonging to the guild", eTRIGGER_CHECK_ELDER_MEMEBER_COUNT_IN_ATTACED_GUILD,
"The number of senior guild members in the affiliated guild is [spin]0[1,10000] [equal]1",

"Items owned by your guild", eTRIGGER_CHECK_OWN_GUILD_ITEM,
" [guild_item]0 in your guild to [item_own]1",

"Amount held by guild", eTRIGGER_CHECK_GOLD_OF_ATTACED_GUILD,
"The amount held in the affiliated guild is [number]0[1,2147483647]gold [equal]1",

"Elapsed guild creation time", eTRIGGER_CHECK_GENERATE_TIME_OF_ATTACED_GUILD,
"The time that has passed since the affiliated guild was created is [number]0[1,2147483647] days [equal]1",

"Affiliated guild ranking", eTRIGGER_CHECK_RANKING_OF_ATTACED_GUILD,
"The ranking of the affiliated guild is [spin]0[1,10000] [equal]1",

" Check guild location", eTRIGGER_CHECK_GUILD_PLACE_OF_RELATED,// Check guild location
"Field [related_place_of_guild]0 is related to [ida_anida]1",

"Guildhall ownership check", eTRIGGER_CHECK_IS_OWN_GUILD_HALL,// Is this our guildhall?
"The current field is the guild hall of the affiliated guild [ida_anida]0",

"Guild hall level check", eTRIGGER_CHECK_GUILD_HALL_LEVEL,// Is this our guild hall?
"The guild hall level of the affiliated guild is [spin]0[0,5] [equal]1",

"Check the guild statue skill level", eTRIGGER_CHECK_GUILD_STATUE_SKILL_LEVEL,// Check the guild statue skill level
"The level of your guild's guild statue skill [guild_statue_skill]0 is [spin]1[0,5] [equal]2",

"Guild Master's Level", eTRIGGER_CHECK_GUILD_MASTER_LEVEL,
"The guild master's level is [spin]0[1,10000] [equal]1",

"Guild Master's Job", eTRIGGER_CHECK_GUILD_MASTER_JOB,
"The guild master’s job is [hero_job]0.",

"Guild Master's Job Level", eTRIGGER_CHECK_GUILD_MASTER_JOB_LEVEL,
"The guild master's job level is [spin]0[1,3] [equal]1",

"Guild Master's Alignment", eTRIGGER_CHECK_GUILD_MASTER_ALLIGNMENT,
"The guild master’s alignment is [alignment]0.",

"Guild Master's abilities", eTRIGGER_CHECK_GUILD_MASTER_BASIC_POWER,
"The guild master's [power]0 is [spin]1[1,10000] [equal]2",

"Chapter 5 Bonus Guild Point Penalty Check", eTRIGGER_CHECK_GUILD_SKILL_BONUS_PENELTY,//
"Chapter 5 bonus guild point penalty is [exist_not_exist]0",

"Guild Roen's favorability check", eTRIGGER_CHECK_GUILD_ROEN_GOOD_WILL_CHECK,//
"Roen's favorability is [spin]0[0,6] [equal]1",

"----Quest related", 0x7fff,
" Matters related to the quest[border_comment]0",

"Quest completed.", eTRIGGER_CHECK_COMPLETE_QUEST,
"Quest [quest]0 check [quest_complete]1 target [quest_target]2",
"Quest Completion Checker", eTRIGGER_CHECK_QUEST_COMPLETE_CHECKER,
" [quest_complete_checkr]1 check target [quest_target]2 of quest [quest]0",

"Quest countdown timer check", eTRIGGER_CHECK_QUEST_COUNTDOWN_CHECKER,
"The countdown timer for quest [quest]0 is [quest_countdown_checker]1. The check target is [quest_target]2",

"Quest progress status", eTRIGGER_CHECK_OWN_QUEST,
"Quest [quest]0 check [quest_own]1 target [quest_target]2",

"Quest progress status", eTRIGGER_CHECK_QUEST_PROCESS_LEVEL,
"The progress status of quest [quest]0 is [quest_process_level]1. Check target [quest_target]2",

"Quest Value", eTRIGGER_CHECK_QUEST_VALUE,
"Quest [quest]0's value is [spin]1[0,20000] [equal]2, check target [quest_target]3",

"Quest can be started", eTRIGGER_CHECK_ENABLE_QUEST,
" [quest_target]0 makes [quest]1 [quest_able]2",

"Quest-related job check", eTRIGGER_CHECK_PLAYER_JOB_BY_QUEST,
" [quest]3 - Job is [quest_job]0 [ida_anida]1. [include_transformation_job]2",


"I completed my part-time job.", eTRIGGER_CHECK_COMPLETE_ARBEIT,
"Part-time job [quest_arbeit]0 check [quest_complete]1 target [quest_target]2",

"Part-time job completion checker", eTRIGGER_CHECK_ARBEIT_COMPLETE_CHECKER,
"Part-time job [quest_arbeit]0's [quest_complete_checkr]1 check target [quest_target]2",

"Part-time job countdown timer check", eTRIGGER_CHECK_ARBEIT_COUNTDOWN_CHECKER,
"The countdown timer for part-time job [quest_arbeit]0 is [quest_countdown_checker]1. The check target is [quest_target]2",

"Whether part-time job is in progress", eTRIGGER_CHECK_OWN_ARBEIT,
"Part-time job [quest_arbeit]0 check [quest_own]1 target [quest_target]2",

"Part-time job progress status", eTRIGGER_CHECK_ARBEIT_PROCESS_LEVEL,
"The progress status of part-time job [quest_arbeit]0 is [quest_process_level]1. Check target [quest_target]2",

"Part-time job figures", eTRIGGER_CHECK_ARBEIT_VALUE,
"The number of part-time job [quest_arbeit]0 is [spin]1[0,10000] [equal]2, check target [quest_target]3",

"Number of ongoing quests and part-time jobs", eTRIGGER_CHECK_PROCESS_QUEST_COUNT,
"The number of ongoing quests (including part-time jobs) is [spin]0[0,50] [equal]1 Check target [quest_target]2 [quest_kind]3",

"Can new quests be added?", eTRIGGER_REMAIN_PROCESS_QUEST_SLOT,
"A new quest can be added [exist]0 check target [quest_target]1",

"You can start a part-time job", eTRIGGER_CHECK_ENABLE_ARBEIT,
" [quest_target]0 makes [quest_arbeit]1 [quest_able]2",

"Check jobs related to part-time jobs", eTRIGGER_CHECK_PLAYER_JOB_BY_ARBEIT,
" [quest_arbeit]3 - Job is [arbeit_job]0 [ida_anida]1. [include_transformation_job]2",

"Check main quest value", eTRIGGER_CHECK_MAIN_QUEST_VALUE,
"The value of the main quest value [main_quest_value]0 is [number]1[0,65535] [equal]2",

"Main Quest Switch Check", eTRIGGER_CHECK_MAIN_QUEST_SWITCH,
"Main quest switch [main_quest_switch]0 is [check_on_off]1",



"----event/monster related", 0x7fff,
"Event/monster related trigger [border_comment]0",

"Check the number of surrounding enemies", eTRIGGER_CHECK_ENEMY_COUNT_IN_RANGE,// The number of players within x pixels around is more than/less than x.
"The number of enemies in [spin]0[1,20000] pixels from the center is [spin]1[0,1000] [equal]2",

" Health check ", eTRIGGER_CHECK_HP_PERCENTAGE,// Your HP is above/below x%.
" Stamina is [spin]0[0,100]% [equal]1 target [target_owner]2",

"Dungeon value check", eTRIGGER_CHECK_DUNGEON_FLAG,// Dungeon flag y is greater than/less than x.
" [dungeon_value]0 is [spin]1[0,20000] [equal]2",

"Field value check", eTRIGGER_CHECK_FIELD_VALUE,// Field value y is greater than/less than x.
" [field_value]0 is [spin]1[0,20000] [equal]2",

"World value check", eTRIGGER_CHECK_WORLD_VALUE,// Field value y is greater than/less than x.
" [world_value]0 is [spin]1[0,128] [equal]2",

"S Instance Dungeon Play Time", eTRIGGER_CHECK_PLAY_TIME_IN_THIS_DUNGEON,// It has been more/less than x minutes since the current party entered this dungeon.
"Instance dungeon creation time is [spin]0[0,20000] minutes [equal]1",

"Area check", eTRIGGER_CHECK_ARRIVE_AREA,// Arrived at area x.
" [all_character_target_player2]0 is [area]1 [area_in_out]2",

"Check item use", eTRIGGER_CHECK_TARGET_USE_BELT_ITEM,
" [quest_target]0 used this item.",

"Target is running away", eTRIGGER_CHECK_TARGET_FLEE,// Target has not been attacked by itself for more than x seconds. (Target currently in combat)
"The target has not been attacked for more than [spin]0[0,20000] seconds.",

"Check use of S skill", eTRIGGER_CHECK_USE_SKILL,// A specific skill was used.
" [quest_target]0 used [skill]2 in [area]1.",

" Monster level check", eTRIGGER_CHECK_MONSTER_LEVEL,// Monster level check
" Monster level is player level - [spin]0[0,20000] [equal]1",

"Event monster check", eTRIGGER_CHECK_MONSTER_EVENT_VALUE,// Monster event settings
"The monster is [event_monster]0 [ida_anida]1",


"----Other", 0x7fff,
" Other[border_comment]0",

"Check OX event start time", eTRIGGER_CHECK_OX_QUIZ_START_TIME,
"It is [spin]0[0,10000] minutes before the OX event starts.",

"Check the start time of the word quiz event", eTRIGGER_CHECK_WORD_QUIZ_START_TIME,
"It is [spin]0[0,10000] minutes before the word quiz event starts.",

"Guide check", eTRIGGER_CHECK_GUIDE,
" Assistant is [setting_status]0",

"Check S countdown timer", eTRIGGER_CHECK_COUNTDOWN_TIMER,
"The time of the countdown timer is [spin]0[0,60] hours [spin]1[0,60] minutes [spin]2[0,60] seconds [countdown_timer_status]3",

"Check event time", eTRIGGER_CHECK_EVENT_TIME,
"The current time is [event_time_name]0 and the time the event is in progress is [ida_anida]1",

"Time check", eTRIGGER_CHECK_TIME,
" [spin]0[2004,3000] year [spin]1[1,12] month [spin]2[1,31] day [spin]3[1,24] from [spin]4[1,10000] ]It is between hours.",

"Time check 2", eTRIGGER_CHECK_TIME2,
" [spin]0[2004,3000] year [spin]1[1,12] month [spin]2[1,31] day [spin]3[1,24] hour [time_check]4",

"Daily time check", eTRIGGER_CHECK_TODAY_TIME,
"The current time is between [spin]0[0,23] hours and [spin]1[0,59] minutes to [spin]2[0,23] hours and [spin]3[0,59] minutes.",

"Test server check", eTRIGGER_CHECK_IS_TEST_SERVER,
"The current server is [test_normal]0 server.",

"Guild Check", eTRIGGER_CHECK_IS_FULL_GUILD,
"Create a guild on this server [able_disable]0",

" Object macro check", eTRIGGER_CHECK_BLOCK_TO_OBJECT_MACRO,
"You are not clicking on an object using a macro.",

"Check probability", eTRIGGER_CHECK_CHANCE,
" [spin]0[1,100]% probability check",

"Probability check 2", eTRIGGER_CHECK_CHANCE2,
" [spin]0[0,50000] / 50000 probability check",


"Check country", eTRIGGER_CHECK_LOCAL,
" Service country is [local]0 [ida_anida]1",

"Time stamp check", eTRIGGER_CHECK_ITEM_TIME_STAMP,
"It has been [spin]1[0,365] days since the timestamp at [item]0 [spin]2[0,24] at time [jina_anjina]3 (if time is 0, no timestamp)",

"Event value check", eTRIGGER_CHECK_EVENT_VALUE,
"Event [event]0 value is [spin]1[0,30000] [equal]2.",

"Check season variables", eTRIGGER_SEASON_VALIABLE,
"The season variable is [Season_Valiable]0 [equal]1.",

"Time check 3", eTRIGGER_CHECK_TIME3,
"It is between [spin]1[1,12] month [spin]2[1,31] day [spin]3[1,24] hour and [spin]4[1,10000] hour.",//0 Burn is ignored. To maintain the structure.

"Time check 4", eTRIGGER_CHECK_TIME4,
" [spin]1[1,12] month [spin]2[1,31] day [spin]3[1,24] hour [time_check]4",//No. 0 is ignored. To maintain the structure.

"Minute check", eTRIGGER_CHECK_MINUTE,
"It is between [spin]0[0,59] minutes and [spin]1[0,59] minutes.",//

"Check the day of the week", eTRIGGER_CHECK_WEEKDAY,
" [weekday]0day [ida_anida]1",

"Event winner check", eTRIGGER_CHECK_PRIZE_PLAYER,
" Target player is [exist]0 in the event winner list",

"Check tileset", eTRIGGER_CHECK_TILESET,
"The tileset of the field where the target is currently located is [tile_set]0 [ida_anida]1",

"Check during monster transformation", eTRIGGER_CHECK_TRANSFORMATION_MONSTER,
" Target is transforming into a monster [ida_anida]0",

"Check the remaining time for the event", eTRIGGER_CHECK_EVENT_COUNT_THE_HOURS,
" Remaining [event_time_name]0 event time is [spin]1[1,30000] [equal]2",

"Check before and after event time", eTRIGGER_CHECK_EVENT_TIME_BEFORE_AFTER,
" [event_time_name]0 event time is [jina_anjina]1",

"end", 0xffff
};

int	l_aFieldTrigger[100]	=
{
	eTRIGGER_CHECK_DUNGEON_FLAG				,//	던전 플래그 y가 x 이상/이하이다.
	eTRIGGER_CHECK_PLAY_TIME_IN_THIS_DUNGEON,//	현재 파티가 이 던전에 들어온지 x분 이상/이하이다.
	eTRIGGER_CHECK_USE_SKILL				,//	특정 스킬을 사용했다.
	eTRIGGER_CHECK_COUNTDOWN_TIMER			,
	0xffff	
};

BOOL
IsAbleToFieldTrigger(int _iTrigger)
{
	int	iIndex	=	0;

	while(1)
	{
		if (l_aFieldTrigger[iIndex] == _iTrigger)
			return	TRUE;

		if (l_aFieldTrigger[iIndex] == 0xffff)
			return	FALSE;

		iIndex++;
	}

	return	FALSE;
}

//
//	item을 찹조해서 인덱스를 구함
int	
cKARMA_WND::GetTriggerIndex(int item)
{
	int	index	=	0;

	while(1)
	{
		if (g_aTrigger[index].m_wSerial	==	item	)	return	index;
		if (g_aTrigger[index].m_wSerial	==	0xffff	)	break;

		index++;
	}

	return	0xffff;
}

//
//	item을 찹조해서 트리거 텍스트를 구한다.
//
char*
cKARMA_WND::GetTriggerText(int item)
{
	item	=	GetTriggerIndex(item);

	return	g_aTrigger[item].m_strReferenceText;
}


//
//	새로운 트리거를 추가하거나 기존의 트리거를 편집한다.
int
cKARMA_WND::AddTrigger(HWND hdlg,CKarmaItem *_lpTrigger)
{
	if (g_EditKarma.m_pTrigger	==	NULL)	g_EditKarma.reset(TRUE);	//	편집할 카르마 초기화

	if (_lpTrigger)
	{
		s_isEditTrigger	=	TRUE;
		g_iSelectTrigger=	0;
		g_EditKarma.m_pTrigger[0].copy(_lpTrigger);
		g_iEditTrigger	=	0;
	}

	if (!s_isEditTrigger)										//	트리거를 편집하는게 아니다.
	{
		if (g_EditKarma.m_wTriggerCount	>=	dTRIGGER_COUNT	)
		{	ERRMSG(dMSG_ADD_TRIGGER_ERROR,dMSG_TRIGGER_FULL);
			return 0xffff;
		}
	}

	int	result	=	DialogBox( s_hInst,"KARMA_NEW",hdlg,(DLGPROC)s_procAddTrigger);

	if (result	==	IDCANCEL)	return 0xffff;

	return	g_iEditTrigger;
}
// 대화편집 - 수정
void
cKARMA_WND::EditCopyTrigger(HWND hDlg)
{
	if (g_EditKarma.m_pTrigger	==	NULL)	
		g_EditKarma.reset(TRUE);

	g_EditTrigger.copy(&g_EditKarma.m_pTrigger[g_iSelectTrigger]);

	g_iEditTrigger			=	g_iSelectTrigger;

	int	iTriggerIndex	=	g_aTrigger[g_iSelectTerm].m_wSerial;
	ReadKarmaText(hDlg,iTriggerIndex,g_aTrigger[g_iSelectTerm].m_strReferenceText,(void *)&g_EditTrigger);

	g_EditTrigger.m_bf1IsAbsolute	=	IsDlgButtonChecked(hDlg,IDC_AB_TRIGGER);
	g_iSelectTrigger				=	g_iEditTrigger;

	g_EditKarma.m_pTrigger[g_iEditTrigger]	=	g_EditTrigger;
		
	g_iSelectTrigger = 0;
	g_iLastTrigger		=	g_iSelectTrigger;

	DrawTriggerWnd(hDlg);
}

// 09.12.03 추가
//
// 대화편집 - 트리거 추가
void
cKARMA_WND::AddCopyTrigger(HWND hDlg)
{
	if (g_EditKarma.m_wTriggerCount	>=	dTRIGGER_COUNT	)
	{	
		ERRMSG(dMSG_ADD_TRIGGER_ERROR,dMSG_TRIGGER_FULL);
		return ;
	}

	g_EditTrigger.Reset();
	g_iEditTrigger		=	g_EditKarma.m_wTriggerCount;

	int	iTriggerIndex	=	g_aTrigger[g_iSelectTerm].m_wSerial;
	ReadKarmaText(hDlg,iTriggerIndex,g_aTrigger[g_iSelectTerm].m_strReferenceText,(void *)&g_EditTrigger);

	g_EditTrigger.m_bf1IsAbsolute	=	IsDlgButtonChecked(hDlg,IDC_AB_TRIGGER);
	g_iSelectTrigger				=	g_iEditTrigger;

	g_EditKarma.m_pTrigger[g_iEditTrigger]	=	g_EditTrigger;
	g_EditKarma.m_wTriggerCount++;
			
	g_iSelectTrigger	= 0;
	g_iLastTrigger		=	g_iSelectTrigger;

	DrawTriggerWnd(hDlg);
}
void
cKARMA_WND::FillTriggerList(HWND hDlg, int _x, int _y)
{
	HWND	hTriggerList	=	GetDlgItem(hDlg,IDC_CONTENTS	);
	int		iIndex			=	0;
	int		iSelect			=	-1,iFirst	=	-1;
	int		iCount			=	0;

	cLISTBOX::Reset(hTriggerList);	//	리스트 리셋

	for (iIndex=0;;iIndex++)
	{
		if	(g_aTrigger[iIndex].m_wSerial	==	0xffff)
			break;

		if	(s_bIsFieldKarma && IsAbleToFieldTrigger(g_aTrigger[iIndex].m_wSerial) == FALSE)
			continue;

		if	(s_strFilter[0] && strstr(g_aTrigger[iIndex].m_strName,s_strFilter)==NULL)
			continue;

		if	(iFirst	==	-1	&&	g_aTrigger[iIndex].m_wSerial	!=	0x7fff)
			iFirst	=	iIndex;

		if	(g_iSelectTerm	==	iIndex)
			iSelect	=	iCount;

		iCount++;

		cLISTBOX::Add(hTriggerList,g_aTrigger[iIndex].m_strName,g_aTrigger[iIndex].m_wSerial);
	}

	if	(iSelect	==	-1)
	{
		if	(iFirst	==	-1)
			return;

		iSelect					=	0;
		g_iSelectTerm			=	iFirst;
		g_iLastestTriggerTerm	=	iFirst;
	}

	cLISTBOX::Select(hTriggerList,iSelect);

	WriteKarmaText(hDlg,g_aTrigger[g_iSelectTerm].m_strReferenceText,g_EditTrigger.m_aValue,NULL, _x, _y);

	SendMessage(hTriggerList,LB_GETTOPINDEX,l_iTriggerPos,0);
}

//
//	트리거 추가 윈도우 초기화.
void
cKARMA_WND::InitAddTriggerDlg(HWND hDlg,int _x, int _y)
{
	if (s_isEditTrigger)										//	트리거를 편집하는게 아니다.
	{
		g_EditTrigger.copy(&g_EditKarma.m_pTrigger[g_iSelectTrigger]);
		g_iEditTrigger			=	g_iSelectTrigger;
		g_iSelectTerm			=	GetTriggerIndex(g_EditTrigger.m_wItem);	
	}
	else
	{
		g_EditTrigger.Reset();
		g_iEditTrigger			=	g_EditKarma.m_wTriggerCount;	//	마지막 트리거
		g_iSelectTerm			=	g_iLastestTriggerTerm;			//	마지막으로 작업한 트리거 아이템
	}

	memset(s_strFilter,0,sizeof(s_strFilter));

	FillTriggerList(hDlg, _x, _y);	//	리스트 리셋

	SetWindowText(hDlg, _ms("%d trigger of karma %s in group %s", g_strGroupName, g_strKarmaName, g_iEditTrigger));

	CheckDlgButton(hDlg,IDC_AB_TRIGGER,g_EditTrigger.m_bf1IsAbsolute);
}

//
//	새로운 트리거 아이템을 선택했다.
void
cKARMA_WND::SelectTriggerItem(HWND hDlg,int _x, int _y)
{
	HWND	hTriggerList	=	GetDlgItem(hDlg,IDC_CONTENTS	);
	int		iSelect			=	SendMessage(hTriggerList,LB_GETCURSEL,0,0);

	if	(iSelect	==	LB_ERR			)
		return;

	iSelect			=	cLISTBOX::GetData(hTriggerList);
	iSelect			=	GetTriggerIndex(iSelect);

	if	(iSelect	==	g_iSelectTerm	)
		return;

	g_iLastestTriggerTerm	=	iSelect;
	g_iSelectTerm			=	iSelect;

	g_EditTrigger.Reset();

	WriteKarmaText(hDlg,g_aTrigger[g_iSelectTerm].m_strReferenceText,g_EditTrigger.m_aValue,NULL,_x, _y);
}


//
//	트리거 데이터를 읽어 들인다.
void
cKARMA_WND::ReadTriggerData(HWND hDlg)
{
	int	iTriggerIndex	=	g_aTrigger[g_iSelectTerm].m_wSerial;

	ReadKarmaText(hDlg,iTriggerIndex,g_aTrigger[g_iSelectTerm].m_strReferenceText,(void *)&g_EditTrigger);

	g_EditTrigger.m_bf1IsAbsolute	=	IsDlgButtonChecked(hDlg,IDC_AB_TRIGGER);

	g_iSelectTrigger	=	g_iEditTrigger;

	if	(s_isEditTrigger)
		g_EditKarma.m_pTrigger[g_iEditTrigger]	=	g_EditTrigger;	//	편집중이라면..
	else
	{
		g_EditKarma.m_pTrigger[g_iEditTrigger]	=	g_EditTrigger;
		g_EditKarma.m_wTriggerCount++;
	}

	l_iTriggerPos					=	SendMessage(GetDlgItem(hDlg,IDC_CONTENTS),LB_GETTOPINDEX,0,0);
}


LRESULT CALLBACK 
cKARMA_WND::s_procAddTrigger(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{	switch(message)
	{	case WM_INITDIALOG	:
			InitAddTriggerDlg(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDOK);
			break;

		case WM_VSCROLL :
			cSPIN::setValue((HWND)lParam,HIWORD( wParam ));
			break;

		case WM_COMMAND :
		{
			int	command	=	LOWORD( wParam );

			if (command	>=	IDC_KARMA_COMBO	&& command	<	IDC_KARMA_COMBO	+	g_iComboIndex)
			{
				
				int	iHiWparam	=	HIWORD( wParam);

				if	(iHiWparam	==	CBN_EDITCHANGE)
					OperateChangeComboBoxText(hDlg,message,wParam,lParam);
					
				if	(iHiWparam	== CBN_SELCHANGE)
					OperateSelectChangeComboBox(hDlg,message,wParam,lParam,FALSE);

			}
			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
					ReadTriggerData(hDlg);
					EndDialog(hDlg,IDOK);
					break;

				case	IDCANCEL			:
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_CONTENTS		:
					SelectTriggerItem(hDlg);
					break;

				case	IDC_FILTER	:
					switch (HIWORD(wParam))
                    {
						case	EN_CHANGE	:
						{
							GetDlgItemText(hDlg,IDC_FILTER,s_strFilter,256);
							FillTriggerList(hDlg);
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