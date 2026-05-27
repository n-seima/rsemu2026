//	화폐단위
//	게임 시간이 흐르는 속도
//	연혁
//	1달 = ?일
//	1년 = ?개월

#include "cKARMA.H"
#include "cMESSAGE.H"
#include "resource.H"
#include "cMAP.H"
#include "secretDungeon.H"
#include "cQuest.H"

int				g_iSelectReaction		=	0;
int				g_iLastReaction			=	0;
int				g_iEditReaction			=	0;
int				g_iLastestReactionTerm	=	1;
CKarmaItem		g_EditReaction;

int				l_iReactionPos			=	0;

char			cKARMA_WND::s_strFilter[256]	=	"";

//////////////////////////////////////////////////////////////////////////////////////////
//	액션들
//////////////////////////////////////////////////////////////////////////////////////////

cKARMA_ITEM		g_aReaction[200]	=
{
"---- conversation/system related" ,0x7fff ,
" NPC dialogue/system-related reactions[border_comment]0",

"Select dialogue" ,eREACTION_SELECT_DIALOG ,
"Go to conversation [speech]0." ,

"End conversation" ,eREACTION_END_DIALOG,
"End conversation." ,

"Enter the secret dungeon" ,eREACTION_MOVE_TO_SECRET_DUNGEON ,// Move to the instanced dungeon
" Moves to a secret dungeon. (Reaction only for secret dungeon entrance)",

" S Countdown" ,eREACTION_SET_COUNTDOWN_TIMER ,// Go to instance dungeon
"Countdown timer to [start_end_extension_reduction]0. [spin]1[0,60] hours [spin]2[0,60] minutes [spin]3[0,60] seconds",


"S End Secret Dungeon" ,eREACTION_COMPLETE_SECRET_DUNGEON ,// Go to instance dungeon
"End [is_secrectD_or_guildD]2. [complete_stop]0 [spin]End after 1[0,30000] seconds. (If set to 0 seconds, set to 60 seconds)",

"Duel related interface" ,eREACTION_DUEL_INTERFACE,
"Open [duel_interface]0." ,

"Shop Transaction Window" ,eREACTION_SHOP,
"Open the transaction window of store [shop]0." ,
"Enhanced window" ,eREACTION_UPGRADE_SHOP , // 09.08.25
"Open the reinforcement window." ,
"Item storage" ,eREACTION_OPEN_BANK , // Opens the item storage window.
"Open the item storage window." ,

"Create guild" ,eREACTION_MAKE_GUILD , // Open the item storage window.
"Opens the guild creation interface." ,

"Guild Shop" ,eREACTION_OPEN_GUILD_SHOP , // Open the guild shop window
"Open the guild store transaction window." ,

"Guild Battle Application Interface" ,eREACTION_OPEN_GUILD_BATTLE_SCHEDULER, // Guild Battle Application
"Open the guild battle application interface. [guild_battle_type]0. ",

"Guild mark combination interface",eREACTION_OPEN_GUILD_MARK_COMPOSER, // Open guild mark combination interface
"Open the guild mark combination interface." ,

"Guild hall level settings",eREACTION_SET_GUILD_HALL_LEVEL, // Grant level 1 guild hall
" Sets the guild hall level to [spin]0[1,5]. (To be used only at level 1!)",

"Move to guild hall" ,eREACTION_MOVE_TO_GUILD_HALL, /// Move to guild hall
"Move to the guild hall. [own_other]0",


"Increase the guild statue skill level",eREACTION_INCREASE_GUILD_STATUE_SKILL_LEVEL ,// Increase the guild statue skill level
"The guild statue skill [guild_statue_skill] level increases by 0.",

"Open guild inventory" ,eREACTION_OPEN_GUILD_INVENTORY ,// Open guild item window
"Open the guild inventory.",

"Add guild location" ,eREACTION_ADD_GUILD_PLACE_OF_RELATED ,// Add guild location
" Move the next village to the guild place [add_remove]0. [related_place_of_guild]1",

"! Change the guild hall shape" ,eREACTION_CHANGE_GUILD_HALL_SHAPE ,// Add guild location
"Change the shape of the guild hall.",

"Application for guild point battle" ,eREACTION_TRY_GUILD_POINT_BATTLE ,// Apply for guild point battle
"Apply for the guild point contest.",

"Give guild honor points" ,eREACTION_GIVE_GUILD_HONOR_POINT ,// Apply for guild points
" [spin]0[1,30000] [method_of_give_ghp]1",

"Guild point battle ends" ,eREACTION_END_GUILD_POINT_BATTLE_AND_RECORD_GUILD_HONOR_POINT ,// Guild point battle ends
"Guild point battle ended [complete_stop]0",

"Guild Siege Field Entry" ,eREACTION_BEGIN_SIEGE_WARFARE_OF_GUILD,
"Entering the Guild Siege Field",

"Open guild location list" ,eREACTION_OPEN_RELATED_PLACE_LIST_OF_GUILD,
"Open the list of guild locations",

"View guild honor points" ,eREACTION_OPEN_GUILD_HONOR_POINT_WINDOW,
" [spin]Shows the honor points of guilds that own a level 0[1,4] guild hall.",

"Raising Guild Pets & Guardians" ,eREACTION_FEED_GUILD_PET_AND_GUARDIAN,
"Open the guild pet and guardian breeding window",

"Guild Roen's favorability" ,eREACTION_SETTING_ROEN_GOODWILL ,// Adjust Roen's favorability.
"Guild Roen's favorability [spin]0[0,6] to [add_remove_set]1",

"Guild Dungeon Entry" ,eREACTION_ENTER_GUILD_DUNGEN ,// Apply for all guild points
" [spin] Enter the 0[0,10] level guild dungeon.",

"Open item repair window" ,eREACTION_OPEN_REPAIR_ITEM_WINDOW,
"Open the item repair window." ,

"Summon a magic carpet" ,eREACTION_CALL_MAGIC_CARPET, // Summon a magic carpet
"Summon a magic carpet." ,

"BREAK" ,eREACTION_BREAK_KARMA , // Break
"Ignore karma after this." ,

"Karma Jump" ,eREACTION_JUMP_KARMA , // Karma Jump
" [spin] Move to Karma 0[0,10000] times. (Karma Jump is not used properly when using Time Delay Karima." ,

"Run random karma" ,eREACTION_LAUNCH_RANDOM_KARMA, // Random karma
" Among the karmas from [spin]0[0,10000] to [spin]1[0,10000] karmas, [spin]2[1,100] karmas are randomly selected and activated. After operation, [spin]3 Jump to [0,10000] karma [hand_anhanda]4",

"Random reaction execution" ,eREACTION_LAUNCH_RANDOM_REACTION, // Random reaction
" Among the reactions from [spin]0[0,10000] to [spin]1[0,10000] reactions, [spin]2[1,100] reactions are randomly selected and operated. After operation, [spin]3 Jump with reaction [0,10000] [hand_anhanda]4",

"Item identification window",eREACTION_OPEN_JUDGE_OF_ITEM_WINDOW, // Open item identification window
"Open the item identification window",

" Craft item pieces" ,eREACTION_OPEN_MERGE_ITEM_PIECE_WINDOW ,// Open item piece combination window
"Open the sculpture item production window.",

"Open item disassembly window" ,eREACTION_OPEN_DISJOINTING_WINDOW,
"Opens the item disassembly window." ,

"Open/Close interface window" ,eREACTION_OPEN_INTERFACE,
"Interface window [popup_interface]0 [open_and_close]1 (window operation [quest_able]2)",

"Open Fast Portal window" ,eREACTION_OPEN_FAST_PORTAL_WINDOW,
"Open the fast portal window",
"---- Quest related", 0x7fff,
"Reactions related to events[border_comment]0",

"Quest Start", eREACTION_SET_QUEST_START,// Puts the quest into the start state.
"Add quest [quest]0 to ongoing quests. Target [quest_target]1",

"Quest completed", eREACTION_SET_QUEST_COMPLETE,// Complete the quest.
" Make quest [quest]0 completed. Target [quest_target]1",

"Quest Completion Checker", eREACTION_CHECK_QUEST_COMPLETE_CHECKER,// Complete the quest.
" [check_quest_complete_checker]2 target [quest_target]1 of quest [quest]0",

"Change quest status", eREACTION_SET_QUEST_PROCESS_LEVEL,// Change quest level.
"Set the status of quest [quest]0 to [quest_process_level]1. Target [quest_target_check_solo]2",

"Setting quest-related values", eREACTION_SET_QUEST_VALUE,//Setting quest values
"Set the value of quest [quest]0 to [spin]1[0,20000]. Target [quest_target_check_solo]2 area [all_area]3",

"Increase/decrease quest value", eREACTION_CONTROL_QUEST_VALUE,//Change quest status
"Quest [quest]0's number is [spin]1[1,255] [plus_minus]2 Target [quest_target_check_solo]3 Area [all_area]4 Max [number]5[0,2047]",

"Quest Cancellation", eREACTION_CANCEL_QUEST,//Quest Cancellation
"Cancel quest [quest]0. Target [quest_target]1",

"Set quest countdown timer", eREACTION_SET_QUEST_COUNTDOWN_TIMER,// Go to instance dungeon
" Set the countdown timer for quest [quest]0 to [start_end_reset]1. [spin]2[0,60] hours [spin]3[0,60] minutes [spin]4[0,60] seconds. Target [quest_target ]5",


"Start part-time job", eREACTION_SET_ARBEIT_START,// Set to start part-time job.
"Add part-time job [quest_arbeit]0 to the ongoing quest. Target [quest_target]1",

"Part-time job completed", eREACTION_SET_ARBEIT_COMPLETE,// Puts the part-time job into the start state.
" Bring part-time job [quest_arbeit]0 to completion state. Target [quest_target]1 output end message [hand_anhanda]2",

"Part-time job completion checker", eREACTION_CHECK_ARBEIT_COMPLETE_CHECKER,// Complete the quest.
" [check_quest_complete_checker]2 target [quest_target]1 of part-time job [quest_arbeit]0",

"Change part-time job status", eREACTION_SET_ARBEIT_PROCESS_LEVEL,//Change part-time job status
"Set the status of part-time job [quest_arbeit]0 to [quest_process_level]1. Target [quest_target_check_solo]2",

"Set part-time work related figures", eREACTION_SET_ARBEIT_VALUE,//Set part-time work figures
"Set the value of part-time job [quest_arbeit]0 to [spin]1[0,20000]. Target [quest_target_check_solo]2 area [all_area]3",

"Increase/decrease part-time job value", eREACTION_CONTROL_ARBEIT_VALUE,//Change part-time job status
"The number of part-time job [quest_arbeit]0 is [spin]1[1,255] [plus_minus]2 target [quest_target_check_solo]3 area [all_area]4 Max [number]5[0,2047]",

"Cancel part-time job", eREACTION_CANCEL_ARBEIT,//Cancel part-time job
"Cancel part-time job [quest_arbeit]0. Target [quest_target]1",

"Setting part-time countdown timer", eREACTION_SET_ARBEIT_COUNTDOWN_TIMER,// Go to instance dungeon
"Countdown timer for part-time [quest_arbeit]0 to [start_end_reset]1. [spin]2[0,60] hours [spin]3[0,60] minutes [spin]4[0,60] seconds. Target [quest_target] ]5",


"Chapter settings", eREACTION_SET_CHAPTER,// Go to instance dungeon
"Change the ongoing chapter to [chapter]0 top quest [spin]1[0,60] to [set_cancel]2.",

"Main quest value setting", eREACTION_SET_MAIN_QUEST_VALUE,// Sets/increases/decreases the value of main quest value x to y [.]
"Main quest value [main_quest_value]0 to [number]1[0,65535] [set_plus_minus]2.",

"Main Quest Switch Setting", eREACTION_SET_MAIN_QUEST_SWITCH,// Sets/increases/decreases the value of main quest value x to y [.]
"Main Quest Switch [main_quest_switch]0 to [set_cancel]1",

"Reset main quest value", eREACTION_RESET_MAIN_QUEST_VALUE,// Set/increase/decrease the value of main quest value x to y [.]
"Reset all main quest values.",

"Main Quest Switch Reset", eREACTION_RESET_MAIN_SWITCH,// Sets/increases/decreases the value of main quest value x to y [.]
"Reset all main quest switches.",

"Output chapter epilogue", eREACTION_DISPLAY_MAIN_QUEST_EPILOG,// Go to instance dungeon
"Shows the epilogue screen of chapter [chapter]0. Prints the previous image [spin]1[0,60] for [spin]2[1,600] seconds",


"Heavenly Reward", eREACTION_RECEIVE_HEAVEN_REWARD,// Go to instance dungeon
"Gives a heavenly reward [heaven_reward]1.",

"Underground Reward", eREACTION_RECEIVE_HELL_REWARD,// Go to instance dungeon
"Gives an underworld reward [hell_reward]1.",

"Red Devil Reward", eREACTION_RECEIVE_RED_DEVIL_REWARD,// Go to instanced dungeon
"Gives Red Devil Reward [red_devil_reward]1.",


"---- item related", 0x7fff,
"Reactions related to items[border_comment]0",

"Add Gold", eREACTION_ADD_GOLD,
" [number]0[1,2147483647]gold to [add_remove]1 target [quest_target]2 [by_level]3 payment reason [add_gold_reason]4",

"Add/Remove Item", eREACTION_ADD_ITEM,
" [item]0 [spin]1[0,1024] [add_remove]2 target [quest_target]3 (removing 0 removes all corresponding items) [ignore_include_equipmment]4",

" Add item (option)", eREACTION_ADD_ITEM_WITH_OPTION,
" Add 1 [item]0. Target [quest_target]1 Option 1([spin]2[0,255]~[spin]3[0,255]) Option 2([spin]4[0,255]~[spin]5 [0,255])",

"Add special item", eREACTION_ADD_SPECIAL_ITEM,
" [special_item]0 [spin]1[1,255] [add_remove]2 target [quest_target]3 (removing 0 removes all corresponding items) [ignore_include_equipmment]4",

"Add Quest Item", eREACTION_ADD_QUEST_ITEM,
" [quest_item]0 [spin]1[1,255] [add_remove]2 target [quest_target]3 (removing 0 removes all corresponding items) [ignore_include_equipmment]4",

"Add a specific item", eREACTION_ADD_SPECIFIC_ITEM,
"Add [specific_item]0. Target [quest_target]1",

"Add random set items", eREACTION_ADD_SET_ITEM_RANDOM,
" Randomly add [spin] 0[0,10] part set item (if 0, completely random) to target [quest_target]1, and add [spin] 2[0,10] part (if 0, set payment) randomly (duplicate .",


" Gold Bar Selling NPC Karma", eREACTION_BUY_GOLD_BAR, // Add Gold Bar.....09.08.19
"Sell 0[1,20] gold ingots [spin] to the player. (Price of 100 million per piece excluding commission)",

"Gold Drop", eREACTION_DROP_GOLD,
" Drops [number]0[1,2147483647]gold.",

" Drop item", eREACTION_DROP_ITEM,
" Drops [item]0 [spin]1[1,255].",

" Item drop (optional)", eREACTION_DROP_ITEM_WITH_OPTION,
" Drops [item]0 [spin]1[1,255]. Option 1([spin]2[0,255]~[spin]3[0,255]) Option 2([spin]4[0,255]~[spin] 5[0,255])",

"Drop unowned item (optional)", eREACTION_UNOWNED_DROP_ITEM_WITH_OPTION,
" Drops [item]0 [spin]1[1,255] unclaimed items. Option 1 ([spin]2[0,255]~[spin]3[0,255]) Option 2 ([spin]4[0,255] ~[spin]5[0,255])",

"Special item drop", eREACTION_DROP_SPECIAL_ITEM,
" Drops 1[special_item]0 [spin]1[1,255].",

"Quest item drop", eREACTION_DROP_QUEST_ITEM,
" Drops [quest_item]0 [spin]1[1,255].",

"Drop a specific item", eREACTION_DROP_SPECIFIC_ITEM,
"Drop [specific_item]0.",

"Set composition item", eREACTION_ADD_SET_ELEMENT_ITEM,// Add set composition item
" [set_element_item]0 to [give_drop]2 target [quest_target]1 (the drop must be placed in the death trigger. At this time, the target is ignored)",

" Item Piece", eREACTION_ADD_ITEM_PIECE,// Add item piece
"The [spin]1[0,15]th piece (0 is random) of [item_piece]0 will be placed in [give_drop]2 target [quest_target]3 (the drop will be placed in the death trigger. At this time, the target will be ignored)",

"Extend item validity period", eREACTION_EXTEND_ITEM_VALIDATE_TIME_TO_NEXT,// Extend item validity period to the next day
"The validity period of [item]0 is extended to the next day.",


"Create book item", eREACTION_CREATE_BOOK,// Create book item
"Remove book [book]0 to [add_remove]1 (remove all books when removed)",

"Add a category to a book (delete this guy!!)", eREACTION_CREATE_ADD_CATEGORY_TO_BOOK_ITEM,// Add a category to a book item Create an item
" (Delete this guy!!) Create items to add from category [spin]1[1,18] to [spin]2[1,18] to book [book]0.",

"Create an item to add a page to a book", eREACTION_CREATE_ADD_PAGE_TO_BOOK_ITEM,// Create an item to add a category to a book item
" Add an item to [add_remove]4 to add pages [spin]2[1,8] to [spin]3[1,8] to category [spin]1[1,18] in book [book]0. (Remove all pages when uninstalled)",

// "Create an item that adds a random category to N books" ,eREACTION_CREATE_ADD_RANDOM_CATEGORY_TO_BOOK_ITEM,// Create an item that adds a category to a book item
// "Create an item that adds one random category to book [book]0.",

"Create an item that adds a random page to a book", eREACTION_CREATE_ADD_RANDOM_PAGE_TO_BOOK_ITEM,// Create an item that adds a category to a book item
"Creates an item that adds a random page to book [book]0.",

"Write or erase pages in a book", eREACTION_WRITE_PAGE,// Add category to book item Create item
" [write_erase]3 page [spin]2[1,8] in category [spin]1[1,18] in book [book]0",

"Exploding the event balloon", eREACTION_EXPLOSION_EVENT_BALLOON,//
"Pop up the event balloon [item]0.",

"Time stamp", eREACTION_TIME_STAMP,// Add category to book item Create item
" Stamp the current time in item [item]0.",

"Chapter 5 Red Jewel Piece Payment", eREACTION_GIVE_RED_STONE_PIECE,// Add category to book item Create item
"Considering all conditions, one red jewel piece will be given to one of the party members.",


"---- character related", 0x7fff,
"Reactions related to characters[border_comment]0",

// "Character appearance" ,eREACTION_CREATE_CHARACTER ,// Add character
// "[character]1 and [spin]2[1,255] people are created in [area]0." ,

"GET GRACE", eREACTION_GET_GRACE,// GET GRACE
"[Grace]0 to target [add_remove]1",

"Get a title", eREACTION_GET_TITLE,// Get a title
" [target_player]0 has [spin]1[1,10] level [title]2 title [add_remove]3",

" Gain experience", eREACTION_GET_EXPERIENCE,// Gain experience
" [target_player]1's experience level increases by [number]2[1,2147483647] in [all_area]0. [apply_premium_bonus]3 Related quests [quest]4 [quest_arbeit]5",

" Gain experience 2", eREACTION_GET_EXPERIENCE2,// Gain experience
"In [all_area]0, the experience required to level up [target_player]1 increases by [number]2[1,100]%. [apply_premium_bonus]3 Related quests [quest]4 [quest_arbeit]5",


"Death Penalty", eREACTION_RECOVER_DEATH_PENELTY,
"Cures the target's death penalty. Paid [ida_anida]0",

"Remove character", eREACTION_REMOVE_CHARACTER,// Remove character
"Kill all [all_character]1 in [all_area]0.",

"Move field", eREACTION_MOVE_FIELD,// Move field
"Move [target_player]1 in [all_area]0 to [number]3[0,4096],[number]4[0,4096] in [field]2.",

"Move location", eREACTION_MOVE_AREA,// Move field
"Move [target]2 to [all_area]3 in area [all_area]0 [in_out]1.",

"Shout", eREACTION_SHOUT, // Shout.
" [character]0 shouts the following message to [notice_range]1: [text_box]2",

"Talk", eREACTION_TALK, // Dialogue
" Say the following lines: [talk_method]0 [text_box]1 ",

"Talk 2", eREACTION_TALK2, // Dialogue
" [character]0 says the following lines: [talk_method]1 [text_box]2 [notice_range]3",

"Skill Redistribution", eREACTION_REDISTRIBUTE_SKILL, // Skill Redistribution
"Redistribute skills.",

"Stat Redistribution", eREACTION_REDISTRIBUTE_STATE, // Stat Redistribution
"Redistribute stats.",

"Ability score redistribution (for duel arena)", eREACTION_RESET_ACTOR_IN_DUEL_SERVER, // Ability score redistribution (for duel arena)
"Redistribute abilities.",

"Reset pet name", eREACTION_RESET_PET_NAME, // Clear pet name
" [spin]0[0,1] resets the name of the pet.",

"Add pet", eREACTION_ADD_PET, // Clear pet name
" [spin]0[1,1000] level [monster]1 is added as a pet.",

"Health recovery", eREACTION_RECOVER_HP, // Health settings
"Recovers [spin]2[1,100]% of the health of [all_character_target_player]1 within [all_area]0.",

" CP recharge", eREACTION_CARGE_CP, // CP refill
"Recharge the CP of [all_character_target_player]1 in [all_area]0 by [spin]2[1,100]％.",

"Inflict damage", eREACTION_DAMAGE, // Stamina settings
"Inflicts [physical_magic]4 damage of [number]2[1,65535]~[number]3[1,65535] points to [all_character_target_player]1 within [all_area]0. [damage_method]5",

"Change view", eREACTION_CHANGE_SIGHT_RANGE,
"Change the field of view of [all_character]1 in [all_area]0 to [spin]2[1,10000].",

"Give status abnormality", eREACTION_STATE_CHANGE,
"Give [state]1 to [quest_target]0 for [spin]2[1,10000] seconds.",

"Blind (permanent)", eREACTION_STATE_BLIND_EFFECT,
"Adds [blind_Range] level 1 blackout effect to [quest_target]0. (If removed, it will be removed regardless of level.)",

"Past Life", eREACTION_REBIRTH,
"Reincarnates the target. (Reset level to [hand_anhanda]0)",


"Remove buff", eREACTION_DISPELL,
"Removes all buffs from [target_player]0.",

"Release monster transformation", eREACTION_RELEASE_TRANSFORMATION,
"Cancels [target_player]0's monster transformation.",


"---- Monster AI, NPC related", 0x7fff,
"Actions related to monster AI[border_comment]0",

"Use skills possessed", eREACTION_USE_ABILITY,// Attempt to use skill xxx possessed by the monster. <- Fail if conditions are not met
"Use skill [skill]0.",

"Use skill", eREACTION_USE_MONSTER_ABILITY,// The monster attempts to use skill xxx. <- Failure if conditions are not met 09.08.14
"Use skill [skill]0 at [spin]1[1,200]Lv.",

"Casting magic", eREACTION_CASTING_MAGIC, // Casting NPC auxiliary magic.
" Cast [skill]2 on [target_player]1 in [all_area]0 with [spin]3[1,200] Lv and related ability [spin]4[1,5000] for [spin]5[0,20000] seconds. (If it is 0 seconds, it depends on the skill time)",

"Job change", eREACTION_TRANS_JOB,// Transform into monster xxx.
"Transforms into [character]0.",

"Move", eREACTION_MOVE_TO,// Move/attack/patrol to area x.
" [all_character_target_player]0 to [area]1 [move_patrol]2 [move_method]3",

"Guide Settings", eREACTION_SET_GUIDE,// Move/attack/patrol to area x.
" [target_player]0 as guide [set_cancel]1",

"Follow the guide", eREACTION_TRACE_GUIDE,// Move/attack/patroll to area x.
"Follow the guide [set_cancel]0",

"Monster Regen", eREACTION_REGEN_MONSTER,// Regenerate monsters in area x.
"Regenerate [all_character]1 in [area]0.",

"Random target setting", eREACTION_CHANGE_TARGET_BY_RANDOM,// Releases the current attack target, sets a random person within the field of view as the attack target, and maintains that target for x seconds.
"Selects a random person within sight and attacks. [spin]Does not change target for 0[1,30000] seconds.",
	
"Release target", eREACTION_TARGET_RESET,// Release information related to the target.
"Reset targeting information.",

"Change battle disposition", eREACTION_CHANGE_BATTLE_INCLINATION,// Change battle disposition
"Set the combat tendency of [all_character]1 in [all_area]0 to [bi]2.",

"Release status abnormalities", eREACTION_RELEASE_WRONG_STATUS,// Clears various status abnormalities.
"All abnormal states of [all_character]1 in [area]0 are cleared.",

"Action", eREACTION_NPC_ACTION,// Clears various status abnormalities.
"[all_character]1 in [area]0 take action in [direct]2 direction. [action]3 [spin]repeat 4[0,30000] times",

"Invincibility status setting", eREACTION_CONTROL_INVINCIBLE_STATUS,// Removes various status abnormalities.
" [all_character]1 in [area]0 and [invincible_onoff]2",

"Change name", eREACTION_CHANGE_NAME,// Change monster name
"Change the monster name to the name below [text_box]0",

"Event monster settings", eREACTION_SET_EVENT_MONSTER_VALUE,// Change monster name
"Set monster to [event_monster]0",

"Increased experience gained", eREACTION_INCREASE_INSTANT_EXPERIENCE,
"Increases the acquired experience by [number]0[1,255] times.",


"---- Various production/field-related actions", 0x7fff,
"Various production/field-related actions[border_comment]0",

"System message", eREACTION_SYSTEM_NOTICE,// System notice
" Print the following message for [notice_range]0 [notice_type]1 [spin]2[1,30] seconds. [effectflickeringly]3 (Time and effect do not work for the entire server, use only for global notices)[text_box]4 ",


"S Dungeon value adjustment", eREACTION_CONTROL_DUNGEON_FLAG,// Add/subtract x amount of dungeon flag, set to x.
"[dungeon_value]0 to [spin]1[0,30000] [add_remove_set]2",

"Field value control", eREACTION_CONTROL_FIELD_VALUE,// Add/subtract the field value by x, set to x.
"[field_value]0 to [spin]1[0,30000] [add_remove_set]2",

"Adjust world values", eREACTION_CONTROL_WORLD_VALUE,// Add/subtract world values by x, set to x.
"world value[world_value]0 to [spin]1[0,30000] [add_remove_set]2",

"S Extension of secret dungeon maintenance time", eREACTION_EXTEND_SECRET_DUNGEON_UPKEEP_TIME,// Extension of secret dungeon time
"Extends the secret dungeon maintenance time by [spin]0[1,1000] minutes.",

"Time delay", eREACTION_DELAY,
"Wait [spin]0[0,30000] minutes [spin]1[0,59] seconds [spin]2[0,999]ms until the next action. (Applies only to reactions after the current karma. The next karma is immediately It works.)",

"Set box/door/trap status", eREACTION_SET_ARCA_DOOR_TRAP_STATUS,// Set box/door/trap status for area x
" [arca_door_trap]1 in [area]0 to [arca_door_trap_status]2",

"Door Enable Disable", eREACTION_SET_DOOR_ENABLE_DISABLE,// Set box/door/trap state for area x
" statement in [area]0 [enable_disable]1",


"S Event Start", eREACTION_BEGIN_EVENT,
" [event_begin_end]0 [battle_status]1",

"Activate skill effect", eREACTION_OCCUR_SKILL_EFFECT,// Activate skill in specific area
"Activates [skill_effect_type]3 effect of [skill]2 at tile coordinates [spin]0[1,30000],[spin]1[1,30000].",

"Regional Focus", eREACTION_MOVE_FOCUS,// Move focus
" Set focus to [spin]0[1,30000],[spin]1[1,30000]. [set_reset_scroll]2 Scroll time [spin]3[0,30000]/100 seconds",

" S character focus", eREACTION_FOCUS_ON_CHARACTER,// Move focus
" Set focus to [character]0. [set_reset]1",

"Shake the screen", eREACTION_SHAKE_SCREEN,// Shake the screen
"An earthquake with shaking [spin]3[1,10] occurs within the pixel with range [spin]2[1,30000] at tile coordinates [spin]0[1,30000],[spin]1[1,30000]. Duration [spin]4[1,30000] Mili Second",

"Sound effect output", eREACTION_PLAY_SOUND,// Sound effect output
"Play [sound_file]1 file to the player in [all_area]0.",

"Highlight on S minimap", eREACTION_STRESS_MINIMAP,
" Output the highlight effect at [spin]0[1,30000],[spin]1[1,30000] on the minimap. [set_reset]2",


"Pattern call", eREACTION_PLAY_PATTERN,
"Call pattern [pattern]0.",

"Boss Event", eREACTION_PLAY_BOSS_EVENT,
" [boss_monster]0 [entry_death]1 event starts",

"Screen effect", eREACTION_SCREEN_EFFECT,
"Give [screen_effect]1 effect to [quest_target]0 for [spin]2[0,50000] seconds",

"Change field resistance", eREACTION_FIX_FIELD_RESISTANCE,
"Change [resistance]0 to [spin]1[0,200]",

"Climate change", eREACTION_FIELD_WEATHER,
"Change current field to [weather]1 state for [spin]0[0,84000] seconds", // Added 09.10.06.. Climate change
"Enter guild bidns with party members", eREACTION_JOIN_GUILD_SECRET_DUNGEONS_WITH_PARTY,
"Enter as a guild member with party members.", //

"Output fireworks effects at specific coordinates", eREACTION_FIRE_WORK,
"Start tile coordinates [spin]0[0,30000],[spin]1[0,30000] to target tile coordinates [spin]2[0,30000],[spin]3[0,30000] to [spin]4 Output [0,10] fireworks effects (number 0 is random)",

"Output image in screen coordinates", eREACTION_DRAW_IMAGE,
" Output index [spin]0[0,20000] of interface2.sd file to screen coordinates [spin]1[0,30000],[spin]2[0,30000] for [spin]3[0,20000] seconds (Method: Move by [drawing_method]4,[spin]5[0,1000].)",

"Turn off image in screen coordinates", eREACTION_OFF_DRAWING_IMAGE,
"Turn off the image output at index [spin]0[0,20000] in interface2.sd file (screen coordinates [spin]1[0,30000],[spin]2[0,30000])",

"---- Other", 0x7fff,
" Other[border_comment]0",

"N screen movement event", eREACTION_SET_SCREEN_EVENT,//Screen movement production
"[target_player]4 moves to tile [spin]0[0,50000],[spin]1[0,50000](X, Y) on screen for [spin]2[0,50000] seconds and [spin]3 It is maintained for [0,50000] seconds [all_area]5. (Not implemented for the party leader and anyone in the party.)",

"Offline Event", eREACTION_EVENT_VALUE,// Go to instance dungeon
"Increase the number related to event [event]0. Maximum [spin]1[1,30000]. Check duplicate receipt [hand_anhanda]2",

	"end"									,0xffff
};

int	l_aFieldReaction[100]	=
{
	eREACTION_SET_COUNTDOWN_TIMER		,//	인스턴스 던전으로 이동
	eREACTION_COMPLETE_SECRET_DUNGEON	,
	eREACTION_BREAK_KARMA				,	//	브레이크
	eREACTION_JUMP_KARMA				,
	eREACTION_REMOVE_CHARACTER			,//	캐릭터 제거
	eREACTION_REGEN_MONSTER				,//	영역 x에 몬스터들을 리젠 시킨다.
	eREACTION_RELEASE_WRONG_STATUS		,//	각종 상태이상을 해제한다.
	eREACTION_CONTROL_INVINCIBLE_STATUS	,//	각종 상태이상을 해제한다.
	eREACTION_CONTROL_DUNGEON_FLAG		,//	던전 플레그를 x만큼 더한다/뺀다, x로 한다.
	eREACTION_SET_ARCA_DOOR_TRAP_STATUS	,//	영역 x의 상자/문/함정 상태 설정
	eREACTION_BEGIN_EVENT				,
	eREACTION_OCCUR_SKILL_EFFECT		,//	특정 지역에 스킬 발동
	eREACTION_MOVE_FOCUS				,
	eREACTION_FOCUS_ON_CHARACTER		,
	eREACTION_SHAKE_SCREEN				,//	화면 흔들기
	eREACTION_PLAY_SOUND				,//	효과음 출력
	eREACTION_STRESS_MINIMAP			,
	eREACTION_EXTEND_SECRET_DUNGEON_UPKEEP_TIME	,//	비밀 던전 시간 연장
	0xffff
};

BOOL
IsAbleToFieldReaction(int _iReaction)
{

	int	iIndex	=	0;

	while(1)
	{
		if (l_aFieldReaction[iIndex] == _iReaction)
			return	TRUE;

		if (l_aFieldReaction[iIndex] == 0xffff)
			return	FALSE;

		iIndex++;
	}

	return	FALSE;
}

//
//	item을 찹조해서 인덱스를 구함
int	
cKARMA_WND::GetReactionIndex(int item)
{
	int	index	=	0;

	while(1)
	{
		if (g_aReaction[index].m_wSerial	==	item	)
			return	index;
		if (g_aReaction[index].m_wSerial	==	0xffff	)
			break;

		index++;
	}

	return	0xffff;
}

//
//	item을 찹조해서 액션 텍스트를 구한다.
//
char*
cKARMA_WND::GetReactionText(int item)
{
	item	=	GetReactionIndex(item);

	return	g_aReaction[item].m_strReferenceText;
}


//
//	새로운 액션를 추가하거나 기존의 액션를 편집한다.
//
int
cKARMA_WND::AddReaction(HWND hdlg)
{
	if (!s_isEditReaction)						//	편집이 아니고 새로운 리액션을 추가하는거다.
	{
		if (g_EditKarma.m_wReactionCount	>=	dREACTION_COUNT	)
		{	
			ERRMSG(dMSG_ADD_REACTION_ERROR,dMSG_REACTION_FULL);
			return 0xffff;
		}
	}

	int	result	=	DialogBox( s_hInst,"KARMA_NEW",hdlg,(DLGPROC)s_procAddReaction);

	if (result	==	IDCANCEL)
		return 0xffff;

	return	g_iEditReaction;
}
// 대화편집 - 반응 카르마 추가
void		
cKARMA_WND::AddCopyReaction(HWND hDlg)
{
	if (g_EditKarma.m_wReactionCount	>=	dREACTION_COUNT	)
	{	
		ERRMSG(dMSG_ADD_REACTION_ERROR,dMSG_REACTION_FULL);
		return;
	}

	g_EditReaction.Reset();
	g_iEditReaction			=	g_EditKarma.m_wReactionCount;

	int	iReactionIndex	=	g_aReaction[g_iSelectTerm].m_wSerial;
	ReadKarmaText(hDlg,iReactionIndex,g_aReaction[g_iSelectTerm].m_strReferenceText,(void *)&g_EditReaction);
	
	g_EditReaction.m_bf1IsAbsolute=	FALSE;
	g_iSelectReaction	=	g_iEditReaction;

	g_EditKarma.m_pReaction[g_iEditReaction].copy(&g_EditReaction);
	g_EditKarma.m_wReactionCount++;

	g_iSelectReaction = 0;
	g_iLastReaction	=	g_iSelectReaction;

	DrawReactionWnd(hDlg);	
}

// 09.12.03 추가
//
// 대화편집 - 반응 카르마 수정
void
cKARMA_WND::EditCopyReaction(HWND hDlg)
{
	g_EditReaction.copy(&g_EditKarma.m_pReaction[g_iSelectReaction]);
	g_EditReaction.m_bf1IsAbsolute=	FALSE;
	g_iEditReaction			=	g_iSelectReaction;

	int	iReactionIndex	=	g_aReaction[g_iSelectTerm].m_wSerial;
	ReadKarmaText(hDlg,iReactionIndex,g_aReaction[g_iSelectTerm].m_strReferenceText,(void *)&g_EditReaction);
	g_EditReaction.m_bf1IsAbsolute=	FALSE;
	g_iSelectReaction	=	g_iEditReaction;

	g_EditKarma.m_pReaction[g_iEditReaction].copy(&g_EditReaction);
		
	g_iSelectReaction = 0;
	g_iLastReaction	=	g_iSelectReaction;

	DrawReactionWnd(hDlg);	
}
void
cKARMA_WND::FillReactionList(HWND hDlg,int _x,int _y)
{
	HWND	hReactionList	=	GetDlgItem(hDlg,IDC_CONTENTS	);
	int		iIndex			=	0;
	int		iSelect			=	-1,iFirst	=	-1;
	int		iCount			=	0;

	cLISTBOX::Reset(hReactionList);	//	리스트 리셋

	for (iIndex=0;;iIndex++)
	{
		if	(g_aReaction[iIndex].m_wSerial	==	0xffff)
			break;

		if (s_bIsFieldKarma && IsAbleToFieldReaction(g_aReaction[iIndex].m_wSerial) == FALSE)
			continue;

		if	(s_strFilter[0] && strstr(g_aReaction[iIndex].m_strName,s_strFilter)==NULL)
			continue;

		if	(iFirst	==	-1	&&	g_aReaction[iIndex].m_wSerial	!=	0x7fff)
			iFirst	=	iIndex;

		if	(g_iSelectTerm	==	iIndex)
			iSelect	=	iCount;

		iCount++;

		cLISTBOX::Add(hReactionList,g_aReaction[iIndex].m_strName,g_aReaction[iIndex].m_wSerial);
	}

	if	(iSelect	==	-1)
	{
		if	(iFirst	==	-1)
			return;

		iSelect					=	0;
		g_iSelectTerm			=	iFirst;
		g_iLastestReactionTerm	=	iFirst;
	}

	cLISTBOX::Select(hReactionList,g_aReaction[g_iSelectTerm].m_strName);

	WriteKarmaText(hDlg,g_aReaction[g_iSelectTerm].m_strReferenceText,g_EditReaction.m_aValue,g_EditReaction.m_strString,_x,_y);

	SendMessage(hReactionList,LB_SETTOPINDEX,l_iReactionPos,0);
}

//
//	액션 추가 윈도우 초기화.
void
cKARMA_WND::InitAddReactionDlg(HWND hDlg,int _x ,int _y )
{
	HWND	hReactionList	=	GetDlgItem(hDlg,IDC_CONTENTS	);

	memset(s_strFilter,0,sizeof(s_strFilter));
	cLISTBOX::Reset(hReactionList);	//	리스트 리셋

	if (s_isEditReaction)											//	액션를 편집하는거다.
	{
		g_EditReaction.copy(&g_EditKarma.m_pReaction[g_iSelectReaction]);
		g_EditReaction.m_bf1IsAbsolute=	FALSE;
		g_iEditReaction			=	g_iSelectReaction;
		g_iSelectTerm			=	GetReactionIndex(g_EditReaction.m_wItem);	
	}
	else
	{
		g_EditReaction.Reset();
		g_iEditReaction			=	g_EditKarma.m_wReactionCount;	//	마지막 액션
		g_iSelectTerm			=	g_iLastestReactionTerm;			//	마지막으로 작업한 액션 아이템
	}

	FillReactionList(hDlg,_x,_y);

	EnableWindow(GetDlgItem(hDlg,IDC_AB_TRIGGER),FALSE);

	SetWindowText(hDlg,_ms("%s 그룹에 카르마 %s의 %d번째 반응",g_strGroupName,g_strKarmaName,g_iEditReaction+1));
}

//
//	새로운 액션 아이템을 선택했다.
//
void
cKARMA_WND::SelectReactionItem(HWND hDlg,int _x,int _y)
{
	HWND	hReactionList	=	GetDlgItem(hDlg,IDC_CONTENTS	);
	int		iSelect			=	SendMessage(hReactionList,LB_GETCURSEL,0,0);

	if	(iSelect	==	LB_ERR			)
		return;

	iSelect			=	cLISTBOX::GetData(hReactionList);
	iSelect			=	GetReactionIndex(iSelect);

	if	(iSelect	==	g_iSelectTerm	)
		return;

	g_iLastestReactionTerm	=	iSelect;
	g_iSelectTerm			=	iSelect;

	g_EditReaction.Reset();

	WriteKarmaText(hDlg,g_aReaction[g_iSelectTerm].m_strReferenceText,g_EditReaction.m_aValue,NULL,_x,_y);
}	//	cKARMA_WND::SelectReactionItem(HWND hDlg)


//
//	액션 데이터를 읽어 들인다.
//
void
cKARMA_WND::ReadReactionData(HWND hDlg)
{
	int	iReactionIndex	=	g_aReaction[g_iSelectTerm].m_wSerial;

	ReadKarmaText(hDlg,iReactionIndex,g_aReaction[g_iSelectTerm].m_strReferenceText,(void *)&g_EditReaction);

	g_EditReaction.m_bf1IsAbsolute=	FALSE;

	g_iSelectReaction	=	g_iEditReaction;

	if	(s_isEditReaction)
		g_EditKarma.m_pReaction[g_iEditReaction].copy(&g_EditReaction);	//	편집중이라면..
	else
	{
		g_EditKarma.m_pReaction[g_iEditReaction].copy(&g_EditReaction);

		g_EditKarma.m_wReactionCount++;
	}

	l_iReactionPos	=	SendMessage(GetDlgItem(hDlg,IDC_CONTENTS),LB_GETTOPINDEX,0,0);

}	//	cKARMA_WND::ReadReactionData(HWND hDlg)

LRESULT CALLBACK 
cKARMA_WND::s_procAddReaction(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG	:
			InitAddReactionDlg(hDlg);
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
					OperateSelectChangeComboBox(hDlg,message,wParam,lParam,TRUE);
				break;
			}

			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
					ReadReactionData(hDlg);
					EndDialog(hDlg,IDOK);
					break;

				case	IDCANCEL			:
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_CONTENTS		:
					SelectReactionItem(hDlg);
					break;

				case	IDC_FILTER	:
					switch (HIWORD(wParam))
                    {
						case	EN_CHANGE	:
						{
							GetDlgItemText(hDlg,IDC_FILTER,s_strFilter,256);
							FillReactionList(hDlg);
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
