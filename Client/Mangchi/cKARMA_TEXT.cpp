#include "cKARMA.H"
#include "cTEXT.H"
#include "cAREA.H"
#include "cJOB.H"
#include "cVALUE.H"
#include "cMAP.H"
#include "cQUEST.H"
#include "cPROJECT.H"
#include "cACTOR.H"
#include "cMESSAGE.H"
#include "resource.h"
#include "CShop.h"
#include "CImageData.h"
#include "CTitle.h"
#include "setItem.h"
#include "pieceItem.h"
#include "secretDungeon.h"
#include "mainQuest.h"
#include "book.h"
#include "cSkill.h"
#include "cJOB.h"
#include "eventDefine.h"

BOOL	l_isEditPower	=	FALSE;

cKEYWORD_ITEM	g_aKarmaText[200]	=
{
	"spin"									,	eKARMA_KW_SPIN							,
	"equal"									,	eKARMA_KW_EQUAL							,		//	이상이다. 이하이다. 이다.
	"skill"									,	eKARMA_KW_SKILL							,		//	마법 리스트
	"power"									,	eKARMA_KW_POWER							,		//	캐릭터의 능력치
	"item"									,	eKARMA_KW_ITEM							,
	"string"								,	eKARMA_KW_STRING						,
	"number"								,	eKARMA_KW_NUMBER						,
	"hero_job"								,	eKARMA_KW_HERO_JOB						,
	"allignment"							,	eKARMA_KW_ALLIGNMENT					,
	"quest_own"								,	eKARMA_KW_QUEST_OWN						,
	"quest_complete"						,	eKARMA_KW_QUEST_COMPLETE				,
	"item_own"								,	eKARMA_KW_ITEM_OWN						,
	"guild_member"							,	eKARMA_KW_GUILD_MEMBER					,
	"item_equip"							,	eKARMA_KW_ITEM_EQUIP					,
	"hero_job_own"							,	eKARMA_KW_JOB_OWN						,
	"exist_not_exist"						,	eKARMA_KW_PARTY_GUILD_MEMBER_EXIST		,
	"all_party_member_in_same_guild"		,	eKARMA_KW_ALL_PARTY_MEMBER_IN_SAME_GUILD,
	"specific_item_own_member_exist"		,	eKARMA_KW_SPECIFIC_ITEM_OWN_MEMBER_EXIST,
	"exist"									,	eKARMA_KW_EXIST							,
	"own"									,	eKARMA_KW_OWN							,
	"speech"								,	eKARMA_KW_SPEECH						,
	"shop"									,	eKARMA_KW_SHOP							,
	"quest"									,	eKARMA_KW_QUEST							,
	"quest_process_level"					,	eKARMA_KW_QUEST_PROCESS_LEVEL			,
	"special_item"							,	eKARMA_KW_SPECIAL_ITEM					,
	"add_remove"							,	eKARMA_KW_ADD_REMOVE					,
	"area"									,	eKARMA_KW_AREA							,
	"all_area"								,	eKARMA_KW_ALL_AREA						,
	"target_player"							,	eKARMA_KW_TARGET_PLAYER					,
	"target"								,	eKARMA_KW_TARGET						,
	"character"								,	eKARMA_KW_CHARACTER						,
	"all_character"							,	eKARMA_KW_ALL_CHARACTER					,
	"all_character_target_player"			,	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER	,
	"all_character_target_player2"			,	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER2	,
	"notice_range"							,	eKARMA_KW_NOTICE_RANGE					,
	"notice_type"							,	eKARMA_KW_NOTICE_TYPE					,
	"text_box"								,	eKARMA_KW_TEXT_BOX						,
	"field"									,	eKARMA_KW_FIELD							,
	"bi"									,	eKARMA_KW_BATTLE_INCLINATION			,
	"physical_magic"						,	eKARMA_KW_PHYSICAL_MAGIC				,
	"equipment"								,	eKARMA_KW_EQUIPMENT						,
	"specific_item"							,	eKARMA_KW_SPECIFIC_ITEM					,
	"quest_arbeit"							,	eKARMA_KW_QUEST_ARBEIT					,
	"plus_minus"							,	eKARMA_KW_PLUS_MINUS					,
	"quest_item"							,	eKARMA_KW_QUEST_ITEM					,
	"title"									,	eKARMA_KW_TITLE							,
	"party_member_location"					,	eKARMA_KW_PARTY_MEMBER_LOCATION			,
	"player_job"							,	eKARMA_KW_PLAYER_JOB					,
	"include_transformation_job"			,	eKARMA_KW_INCLUDE_TRANSFORMATION_JOB	,
	"ida_anida"								,	eKARMA_KW_IDA_ANIDA						,
	"is_party_leader"						,	eKARMA_KW_IS_PARTY_LEADER				,
	"party_member_equal"					,	eKARMA_KW_PARTY_MEMBER_EQUAL			,
	"only_equal"							,	eKARMA_KW_ONLY_EQUAL					,
	"in_out"								,	eKARMA_KW_IN_OUT						,
	"check_operator"						,	eKARMA_KW_CHECK_OPERATOR				,
	"quest_target"							,	eKARMA_KW_QUEST_TARGET					,
	"time_check"							,	eKARMA_KW_TIME_CHECK					,
	"quest_complete_checkr"					,	eKARMA_KW_QUEST_COMPLETE_CHECKER		,
	"check_quest_complete_checker"			,	eKARMA_KW_CHECK_QUEST_COMPLETE_CHECKER	,
	"dungeon_value"							,	eKARMA_KW_DUNGEON_VALUE					,
	"area_in_out"							,	eKARMA_KW_AREA_IN_OUT					,
	"add_remove_set"						,	eKARMA_KW_ADD_SET_REMOVE				,
	"move_patrol"							,	eKARMA_KW_MOVE_PATROL			,
	"arca_door_trap_status"					,	eKARMA_KW_ARCA_DOOR_TRAP_STATUS			,
	"arca_door_trap"						,	eKARMA_KW_ARCA_DOOR_TRAP				,
	"monster_target_self"					,	eKARMA_KW_MONSTER_TARGET_SELF			,
	"invincible_onoff"						,	eKARMA_KW_INVINCIBLE_ON_OFF				,
	"trap_area"								,	eKARMA_KW_TRAP_AREA						,
	"event_begin_end"						,	eKARMA_KW_EVENT_BEGIN_END				,
	"quest_able"							,	eKARMA_KW_QUEST_ABLE					,
	"quest_job"								,	eKARMA_KW_QUEST_JOB						,
	"arbeit_job"							,	eKARMA_KW_ARBEIT_JOB					,
	"set_element_item"						,	eKARMA_KW_SET_ELEMENT_ITEM				,
	"item_piece"							,	eKARMA_KW_ITEM_PIECE					,
	"set_cancel"							,	eKARMA_KW_SET_CANCEL					,
	"direct"								,	eKARMA_KW_DIRECT						,
	"countdown_type"						,	eKARMA_KW_COUNTDOWN_TYPE				,
	"start_end_reset"						,	eKARMA_KW_START_END_RESET				,
	"battle_status"							,	eKARMA_KW_BATTLE_STATUS					,
	"setting_status"						,	eKARMA_KW_SETTING_STATUS				,
	"countdown_timer_status"				,	eKARMA_KW_COUNTDOWN_STATUS				,
	"quest_countdown_checker"				,	eKARMA_KW_QUEST_COUNTDOWN_CHECK			,
	"target_leader"							,	eKARMA_KW_TARGET_LEADER					,
	"skill_effect_type"						,	eKARMA_KW_SKILL_EFFECT_TYPE				,
	"set_reset"								,	eKARMA_KW_SET_RESET						,
	"action"								,	eKARMA_KW_ACTION						,
	"enable_disable"						,	eKARMA_KW_ENABLE_DISABLE				,
	"all_player"							,	eKARMA_KW_ALL_PLAYER					,
	"sound_file"							,	eKARMA_KW_SOUND_FILE					,

	"main_quest_value"						,	eKARMA_KW_MAIN_QUEST_VALUE				,
	"main_quest_switch"						,	eKARMA_KW_MAIN_QUEST_SWITCH				,
	"chapter"								,	eKARMA_KW_CHAPTER						,
	"check_on_off"							,	eKARMA_KW_CHECK_ON_OFF					,
	"set_plus_minus"						,	eKARMA_KW_SET_PLUS_MINUS				,
	"book"									,	eKARMA_KW_BOOK							,
	"write_erase"							,	eKARMA_KW_WRITE_ERASE					,
	"start_end_extension_reduction"			,	eKARMA_KW_START_END_EXTENSION_REDUCTION	,
	"apply_premium_bonus"					,	eKARMA_KW_APPLY_PREMIUM_BONUS			,
	"able_disable"							,	eKARMA_KW_ABLE_DISABLE					,
	"test_normal"							,	eKARMA_KW_TEST_NORMAL					,
	"current_status"						,	eKARMA_KW_CURRENT_STATUS				,
	"set_reset_scroll"						,	eKARMA_KW_SET_RESET_SCROLL				,
	"pattern"								,	eKARMA_KW_PATTERN						,
	"entry_death"							,	eKARMA_KW_ENTRY_DEATH					,
	"boss_monster"							,	eKARMA_KW_BOSS_MONSTER					,
	"target_owner"							,	eKARMA_KW_TARGET_OWNER					,
	"guild_battle_type"						,	eKARMA_KW_GUILD_BATTLE_TYPE				,
	"own_other"								,	eKARMA_KW_GUILD_HALL_OWN_OTHER			,
	"guild_statue_skill"					,	eKARMA_KW_GUILD_STATUE_SKILL			,
	"related_place_of_guild"				,	eKARMA_KW_RELATED_PLACE_OF_GUILD		,
	"method_of_give_ghp"					,	eKARMA_KW_METHOD_OF_GIVE_GP				,
	"move_method"							,	eKARMA_KW_MOVE_METHOD					,
	"talk_method"							,	eKARMA_KW_TALK_METHOD					,
	"damage_method"							,	eKARMA_KW_DAMAGE_METHOD					,
	"complete_stop"							,	eKARMA_KW_COMPLETE_STOP					,
	"ignore_include_equipmment"				,	eKARMA_KW_IGNORE_INCLUDE_EQUIPMENT		,
	"rank_in_guild"							,	eKARMA_KW_RANK_IN_GUILD					,
	"sex"									,	eKARMA_KW_SEX							,
	"event_monster"							,	eKARMA_KW_EVENT_MONSTER					,
	"local"									,	eKARMA_KW_LOCAL					,
	"by_level"					 			,	eKARMA_KW_BY_LEVEL,
	"give_drop"								,	eKARMA_KW_GIVE_DROP,
	"field_value"							,	eKARMA_KW_FIELD_VALUE,

	"heaven_reward"							,	eKARMA_KW_HEAVEN_REWARD,//	인스턴스 던전으로 이동
	"hell_reward"							,	eKARMA_KW_HELL_REWARD,//	인스턴스 던전으로 이동
	"red_devil_reward"						,	eKARMA_KW_RED_DEVIL_REWARD,//	인스턴스 던전으로 이동
	"plus_number_minus_number"				,	eKARMA_KW_PLUSNUMBER_MINUSNUMBER,
	"heaven_bonus"							,	eKARMA_KW_HEAVEN_BONUS,
	"jina_anjina"							,	eKARMA_KW_JANI_ANJINA,
	"bonus_penelty"							,	eKARMA_KW_BONUS_PENELTY,
	"event"									,	eKARMA_KW_EVENT,
	"hand_anhanda"							,	eKARMA_KW_HANDA_ANHANDA,
	"monster"								,	eKARMA_KW_MONSTER,
	"duel_interface"						,	eKARMA_KW_DUEL_INTERFACE,

	//	병찬씨가 추가
	"item_boolean_option_list"				,	eKARMA_KW_ITEM_BOOLEAN_OPTION_LIST	,
	"Season_Valiable"						,	eKARMA_KW_SEASON_VALIABLE				,
	"Grace"									,	eKARMA_KW_GRACE							,
	"world_value"							,	eKARMA_KW_WORLD_VALUE					,
	"effectflickeringly"					,	eKARMA_KW_EFFECT_FLICKERINGLY			,
	"weekday"								,	eKARMA_KW_WEEKDAY						,
	"is_secrectD_or_guildD"					,	eKARMA_KW_IS_SECRECTD_GUILDD			,
	"state"									,	eKARMA_STATE							,
	"resistance"							,	eKARMA_FIX_FIELD_RESISTANCE				,
	"screen_effect"							,	eKARMA_SCRREN_EFFECT					,
	"blind_Range"							,	eKARMA_BLIND_EFFECT						,

	"quest_kind"							,	eKARMA_QUEST_KIND						,
	"weather"								,	eKARMA_FIELD_WEATHER					, // 09.10.06	기후변화..
	"include_solo"							,	eKARMA_INCLUDE_SOLO						, // 파티 관련 체크 일때 솔로 포함?
	"quest_target_check_solo"				,	eKARMA_KW_QUEST_CHECK_SOLO				, // 파티 관련 체크 일때 솔로 포함?
	"tile_set"								,	eKARMA_KW_TILE_SET						, // 타일셋 체크
	"event_time_name"						,	eKARMA_KW_EVENT_TIME_NAME				, // 이벤트 타임 이름
	"drawing_method"						,	eKARMA_KW_DRAWING_METHOD				, // 그리는 방법
	"popup_interface"						,	eKARMA_KW_POPUP_INTERFACE,				//	인터페이스
	"open_and_close"						,	eKARMA_KW_OPEN_AND_CLOSE,				//	열기 닫기
	"add_gold_reason"						,	eKARMA_KW_ADD_GOLD_REASON				,	//	골드 ┷동 사嶽
	
	"border_comment"						,	eKARMA_KW_BORDER_COMMENT				,	//	얘는 항상 마지막에 있어야 한다.
	"end"									,	dCODE_END
};

char	l_strComboStr[200][25][60]	=
{
{""}, // eKARMA_KW_SPIN
{"is","is less than","is more than","end"}, // eKARMA_KW_EQUAL , // is more than. It is as follows. am.
{""}, // eKARMA_KW_SKILL , // Magic list
{""}, // eKARMA_KW_POWER , // Scenario list
{""}, // eKARMA_KW_ITEM , // Karma group list
{""}, // eKARMA_KW_STRING , // Karma list
{""}, // eKARMA_KW_NUMBER , // Turn on/off.
{""}, // eKARMA_KW_HERO_JOB , // Turn on/off.
{"Absolute evil","Evil","Neutral","Good","Absolute good","end"}, // eKARMA_KW_ALLIGNMENT ,
{""}, // eKARMA_KW_GUILD_ITEM
{""}, // eKARMA_KW_TITLE
{""}, // eKARMA_KW_GUILD_TITLE
{"I have it.","I don't have it.","end"}, // eKARMA_KW_QUEST_OWN
{"Completed.","Not completed.","end"}, // eKARMA_KW_QUEST_COMPLETE
{"I own it.","I do not own it.","end"}, // eKARMA_KW_ITEM_OWN
{"Signed up.","Not signed up.","end"}, // eKARMA_KW_GUILD_MEMBER
{"Equipped.","Not equipped.","end"}, // eKARMA_KW_ITEM_EQUIP
{"I have it.","I don't have it.","end"}, // eKARMA_KW_JOB_OWN
{"There is.","Not there","end"}, // eKARMA_KW_PARTY_GUILD_MEMBER_EXIST
{"You are in the same guild.","You are in a different guild.","end"}, // eKARMA_KW_ALL_PARTY_MEMBER_IN_SAME_GUILD
{"There is.","Not there","end"}, // eKARMA_KW_SPECIFIC_ITEM_OWN_MEMBER_EXIST
{"There is.","Not there","end"}, // eKARMA_KW_EXIST
{"I have it.","I don't have it.","end"}, // eKARMA_KW_OWN
{""}, // eKARMA_KW_SPEECH
{""}, // eKARMA_KW_SHOP
{""}, // eKARMA_KW_QUEST
{"Step 1","Step 2","Step 3","Step 4","Step 5","Step 6","end",}, // eKARMA_KW_QUEST_PROCESS_LEVEL
{""}, // eKARMA_KW_SPECIAL_ITEM
{"Add.","Subtract.","end"}, // eKARMA_KW_ADD_REMOVE
{""}, // eKARMA_KW_FIELD
{""}, // eKARMA_KW_BATTLE_INCLINATION
{""}, // eKARMA_KW_AREA
{""}, // eKARMA_KW_ALL_AREA
{"Target","All party members","Party leader","Anyone in the party","Pet", "end"}, // eKARMA_KW_TARGET_PLAYER
{"Partners","NPC","Monsters","Operator","All players except the operator","All players","end"}, // eKARMA_KW_TARGET
{""}, // eKARMA_KW_CHARACTER
{""}, // eKARMA_KW_ALL_CHARACTER
{""}, // eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER
{"To the entire field","To the entire server","Only you can hear it","Only your party can hear it","end"}, // eKARMA_KW_NOTICE_RANGE
{"Chat window message","System message","Full notice","end"}, // eKARMA_KW_NOTICE_TYPE
{""}, // eKARMA_KW_TEXT_BOX
{"Physics","Magic","end"}, // eKARMA_KW_PHYSICAL_MAGIC
{""}, // eKARMA_KW_EQUIPMENT
{""}, // eKARMA_KW_SPECIFIC_ITEM
{""}, // eKARMA_KW_QUEST_ARBEIT
{"Adds.","Subtracts.","end"}, // eKARMA_KW_PLUS_MINUS
{""}, // eKARMA_KW_QUEST_ITEM
{"Collected.","In the same field.","end"}, // eKARMA_KW_PARTY_MEMBER_LOCATION
{""}, // eKARMA_KW_PLAYER_JOB
{"Include job after transformation","Ignore job after transformation","end"}, // eKARMA_KW_INCLUDE_TRANSFORMATION_JOB
{"It is.","It is not.","end"}, // eKARMA_KW_IDA_ANIDA
{"I am the leader of the party.","I am not the leader of the party.","end"}, // eKARMA_KW_IS_PARTY_LEADER
{"is","is less than","is more than","is not","end"}, // eKARMA_KW_PARTY_MEMBER_EQUAL
{"above","below","end"}, // eKARMA_KW_ONLY_EQUAL
{"inside","outside","end"}, // eKARMA_KW_IN_OUT
{"The other party is an operator.","The other party is not an operator.","end"}, // eKARMA_KW_CHECK_OPERATOR
{"Target","All party members","Party leader","Any of the party members","end"}, // eKARMA_KW_QUEST_TARGET
{"It has passed. (including)","Not yet.","end"}, // eKARMA_KW_TIME_CHECK
{"The completion checker is checked.","The completion checker is not checked.","end"}, // eKARMA_KW_QUEST_COMPLETE_CHECKER
{"Uncheck the completion checker.","Check the completion checker.","end"}, // eKARMA_KW_CHECK_QUEST_COMPLETE_CHECKER
{""}, // eKARMA_KW_DUNGEON_VALUE
{"It's inside","It's outside.","end"}, // eKARMA_KW_AREA_IN_OUT
{"Add.","Subtract.","Use.","end"}, // eKARMA_KW_ADD_SET_REMOVE
{"Move.","Patrol.","Teleport.","end"}, // eKARMA_KW_MOVE_PATROL
{"Make it appear.", "Hide it.", "Regenerate it.", "Remove it.", "Lock it.", "Pick it (open the lock)", "Close it", "Open it", "end"} , // eKARMA_KW_ARCA_DOOR_TRAP_STATUS
{"Treasure Chest","Door","Trap","end"}, // eKARMA_KW_ARCA_DOOR_TRAP
{"Self","Monster","Target","Target's party","end"}, // eKARMA_KW_MONSTER_TARGET_SELF ,
{"Makes invincibility.","Removes invincibility.","end"}, // eKARMA_KW_INVINCIBLE_ON_OFF
{""}, // eKARMA_KW_TRAP_AREA
{"Start the event.","End the event.","end"}, // eKARMA_KW_EVENT_BEGIN_END ,
{"You can.","You cannot.","end"}, // eKARMA_KW_QUEST_ABLE,
{""}, // eKARMA_KW_QUEST_JOB
{""}, // eKARMA_KW_ARBEIT_JOB
{""}, // eKARMA_KW_SET_ELEMENT_ITEM
{""}, // eKARMA_KW_ITEM_PIECE
{"Set","Cancel Setting","end"}, // eKARMA_KW_SET_CANCEL,
{"Current direction","Towards the conversation partner","Toward the pathfinder","Toward the party leader","North","Northeast","East","Southeast","South","Southwest","West" ,"Northwest","end"},// eKARMA_KW_DIRECT
{"Large Timer","Bottom Theme Timer","end"}, // eKARMA_KW_COUNTDOWN_TYPE
{"start","end","reset","end"}, // eKARMA_KW_START_END_RESET
{"Available for combat","Not for combat","end"}, // eKARMA_KW_BATTLE_STATUS
{"It is set.","Not set.","end"}, // eKARMA_KW_SETTING_STATUS
{"is","has not yet passed","has passed","end"}, // eKARMA_KW_COUNTDOWN_STATUS
{"Completed.","In progress.","Cancelled.","Not used.","end",}, // eKARMA_KW_QUEST_COUNTDOWN_CHECK
{"Target","Target Party Leader","end"}, // eKARMA_KW_TARGET_LEADER
{"none","hit","skill","recovery","cast","auxiliary skill","caster effect","explosion","end"}, // eKARMA_KW_SKILL_EFFECT_TYPE
{"Set","Cancel Setting","end"}, // eKARMA_KW_SET_RESET
{"none","walk","run","stop","hit","transformation 1","transformation 2","death","sitting","action 1","action 2"," Action 3","Action 4","Action 5","Action 6","end"}, // eKARMA_KW_ACTION
{"Enable.","Disable.","end"}, // eKARMA_KW_ENABLE_DISABLE
{"target","target party member","all players","end"}, // eKARMA_KW_ALL_PLAYER
{"","end"}, // eKARMA_KW_SOUND_FILE
{"","end"}, // eKARMA_KW_MAIN_QUEST_VALUE
{"","end"}, // eKARMA_KW_MAIN_QUEST_SWITCH
{"","end"}, // eKARMA_KW_CHAPTER

{"on","off","end"}, // eKARMA_KW_CHECK_ON_OFF
{"Set as.","Adds.","Subtracts.","end"}, // eKARMA_KW_SET_PLUS_MINUS
{"","end"}, // eKARMA_KW_BOOK
{"write","erase","end"}, // eKARMA_KW_WRITE_ERASE
{"start","end","extend","shorten","end"}, // eKARMA_KW_START_END_EXTENSION_REDUCTION
{"Apply premium bonus","Do not apply premium bonus","Apply as percentage required for leveling up","end"}, // eKARMA_KW_APPLY_PREMIUM_BONUS

{"You can.","You cannot.","end"}, // eKARMA_KW_ABLE_DISABLE
{"test","official","end"}, // eKARMA_KW_TEST_NORMAL
{"The state of riding a carpet","end"}, // eKARMA_KW_CURRENT_STATUS

{"Set","Cancel Set","Scroll","end"}, // eKARMA_KW_SET_RESET_SCROLL

{"","end"}, // eKARMA_KW_PATTERN
{"Boss Appears","Boss Death","end"}, // eKARMA_KW_ENTRY_DEATH
{"Sage","Tiames","Water Dragon","Arch Devil","Draco Lich","end"},// eKARMA_KW_BOSS_MONSTER
{"Target","You","end"},// eKARMA_KW_TARGET_OWNER
{"Guild Battle Prologue","Official Guild Battle","end"},
{"Your guild's guild hall","Other guild's guild hall","end"},
{"","end"}, // eKARMA_KW_GUILD_STATUE_SKILL
{"","end"}, // eKARMA_KW_RELATED_PLACE_OF_GUILD
{ "Gives guild honor points of points.","X Gives guild honor points equal to the remaining time..","end" },	//	eKARMA_KW_METHOD_OF_GIVE_GP
{ "Move to a random position in the area","Move to the center of the area","end" }, // eKARMA_KW_MOVE_METHOD
{ "Event line in chat window","Balloon line","end" }, // eKARMA_KW_TALK_METHOD
{ "Do damage","Do percentage damage in proportion to remaining health","Do percentage damage in proportion to maximum health","end" }, // eKARMA_KW_DAMAGE_METHOD
{ "Completion (secret dungeon cleared)","End (unable to clear and force quit)","end" }, // eKARMA_KW_COMPLETE_STOP
{ "Ignore worn equipment","Include worn equipment","end" }, // eKARMA_KW_IGNORE_INCLUDE_EQUIPMENT ,
{ "Not registered.","Newcomer","Official member","Member","Sub-master","Elder","Master","end" }, // eKARMA_KW_RANK_IN_GUILD
{ "man","woman","end" }, // eKARMA_KW_SEX,
{ "Arbor Day Event Monster","end" }, // eKARMA_KW_SEX,
{ "Korea","Japan","China","United States","Thailand","end" }, // eKARMA_KW_LOCAL ,
{ "It is not proportional to the level.","The amount is multiplied by the level.","end" }, // eKARMA_KW_BY_LEVEL,
{ "Give it.","Drop it in the field.","end" }, // eKARMA_KW_GIVE_DROP,
{ "","end" }, // eKARMA_KW_FIELD_VALUE
{ "","end" }, // eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER2

{ "Acquire one unique item or super unique item", "Bonus stat", "Bonus skill point", "Bonus HP", "Bonus CP", "Bonus experience", "Title bonus", "Guild level bonus", "Guild skill bonus","Guild item bonus","Alignment adjustment","end" }, // eKARMA_KW_HEAVEN_REWARD,// Go to instance dungeon
{ "Change untradable items to tradable","Remove prefix","Stat reverse","Skill reverse","HP bonus restoration","CP bonus restoration","Stat bonus restoration","Skill bonus restoration", "Guild skill reset","Guild skill bonus recovery","Alignment adjustment","end" }, // eKARMA_KW_HELL_REWARD
{ "Item Duplicate","Normal and Rare Item Ability Enhancement","Normal and Rare Item Potential Enhancement","Super Unique Item Prefix Ability Enhancement","Super Unique Item Prefix Potential Enhancement","Red Bottomless Box","Bonus HP Amulet","Bonus CP Amulet","Bonus Equipment Amulet","Bonus Gold","Alignment Adjustment","end" }, // eKARMA_KW_RED_DEVIL_REWARD
{ "Plus","Minus","end" },//eKARMA_KW_PLUSNUMBER_MINUSNUMBER
{ "Stats","Skill","HP","CP","end" },//eKARMA_KW_HEAVEN_BONUS
{ "passed","did not pass","end" }, // eKARMA_KW_JANI_ANJINA
{ "Bonus","Penalty","end" },//eKARMA_KW_BONUS_PENELTY
{ "YEPP Event","end" }, // eKARMA_KW_EVENT
{ "do","don't","reset","end" }, // eKARMA_KW_HANDA_ANHANDA
{ "","end" }, // eKARMA_KW_MONSTER
{ "Duel Ranking Interface","Apply for 1 vs 1 match","Apply for 2 vs 2 match","Apply for 3 vs 3 match","Apply for 4 vs 4 match","Apply for 5 vs 5 match","Apply for preliminaries ","end" }, // eKARMA_KW_DUEL_INTERFACE

// added by Byeongchan
{ "Free town teleport", "Carpet summon request", "Acquire additional mystic stones", "Acquire additional Tantilis artifacts", "Enter Tantilis ruins without guild hall level restrictions",
"Increased probability of upgrading the Infiniti series","Free Fast Portal","Half-price Fast Portal","end" }, //eKARMA_KW_ITEM_BOOLEAN_OPTION_LIST
{ "Season 0", "Season 1","Season 2","Season 3","Season 4","Season 5","Season 6","end" }, //eKARMA_KW_SEASON_VALIABLE
{ "The Grace of Fire","The Grace of Water","The Grace of Wind","The Grace of Earth","end" },//eKARMA_KW_GRACE
{ "","end" }, // eKARMA_KW_WORLD_VALUE
{ "No effect", "Flickering" , "end" }, //eKARMA_KW_EFFECT_FLICKERINGLY
{ "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Sat","end", }, // eKARMA_KW_WEEKDAY
{ "Secret Dungeon","Guild Dungeon","end" },//eKARMA_KW_IS_SECRECTD_GUILDD
{ "Stun", "Freeze", "Cold", "Sleep", "Blind" , "Petrification", "Confusion", "Armor Destruction" , "Weapon Destruction", "Berserk" ,"end" },// eKARMA_STATE
{ "Fire Resistance", "Water Resistance", "Wind Resistance", "Earth Resistance", "Dark Resistance", "Light Resistance", "Blind Resistance", "Poison Resistance", "Sleep Resistance", "Cold Resistance" ,"Freeze Resistance","Stun Resistance","Petrification Resistance","Confusion Resistance","Charming Resistance","Critical Resistance","Death Blow Resistance","end" },//eKARMA_FIX_FIELD_RESISTANCE
{ "black and white","TV noise","end" },// eKARMA_SCRREN_EFFECT
{ "remove","very small","small","normal","large","very large","end" },//eKARMA_BLIND_EFFECT
{ "All Quests","General Quests","Skill Master","Title Quests","Main Story Quests","Guild Quests","end" },

{ "Sunny", "Snow", "Rain","end" }, // eKARMA_FIELD_WEATHER 09.10.06 Climate change.
{ "Does not include solos","Includes solos","end" }, // eKARMA_INCLUDE_SOLO , // Includes solos when doing party-related checks?
{ "target","all party members","party leader","any one of the party members","all party members (including solos)","party leader (including solos)","any one of the party members (including solos) )","end" }, // eKARMA_KW_QUEST_TARGET_CHECK_SOLO
{ "Grassland", "Mountains", "Desert", "Savannah", "Cave", "Dungeon", "Tower", "Mine", "Hell", "Heaven", "Village - Brunnenstig", "Village - Bigapple", "Village - Augusta", "Village - Bridgehead",
"Village - Mountain Village","Village - Aryan","Village - Ruins","Village - Farmhouse","Village - Gypsy Village","Interior of House","Guild Hall","end" },

{ "","end" }, // eKARMA_KW_EVENT_TIME_NAME
{ "None","Repeat up and down", "Repeat left and right", "end" }, // eKARMA_KW_DRAWING_METHOD
{ "None", "Inventory window", "Status window", "Skill window", "Quest window", "Party window", "Guild window", "Friend window", "System window", "end" }, //eKARMA_KW_DRAWING_METHOD
{ "open","close", "end" }, // eKARMA_KW_OPEN_AND_CLOSE

{ "just", "gold purchase price","quest","end" }, // eKARMA_KW_ADD_GOLD_REASON


{ "" }, // This should always be last.
};


cSTATIC			l_aStream	[10];
cBUTTON			l_aButton	[10];
cEDITBOX		l_aNumberBox[10];
cEDITBOX		l_aEditBox	[10];
cEDITBOX		l_aSpinBox	[10];
cSPIN			l_aSpin		[10];

BOOL			l_isJongSung	=	FALSE;
cDECODER		l_Reader;

int				g_iStreamIndex	=	0;
int				g_iComboIndex	=	0;
int				g_iSpinIndex	=	0;
int				g_iSpinBoxIndex	=	0;
int				g_iButtonIndex	=	0;
int				g_iNumberIndex	=	0;
int				g_iEditBoxIndex	=	0;

static	char	*l_strCheckKarmaJongsung[]	=	{"silver ","Is ","this ","go ","sec ","cast ","end"};

void
cKARMA_WND::CheckJongsung(char *str)
{
	l_isJongSung	=	FALSE;

	int		length	=	strlen(str);

	if	(length < 2)
		return;

	if	((BYTE)str[length-2]	>=	0xb0	&&	(BYTE)str[length-1]	>=	0xa1)
		if (IsJongsung(str+length-2))	l_isJongSung	=	TRUE;
}

void
cKARMA_WND::TransCode(char *lpStream)
{
	if	(!lpStream)
		return;

	int		length	=	strlen(lpStream);

	if (length < 3)	return;

	char	word[4];

	word[0]	=	lpStream[0];
	word[1]	=	lpStream[1];
	word[2]	=	lpStream[2];
	word[3]	=	0;

	for (int i=0;i<6;i++)
		if (strcmp(l_strCheckKarmaJongsung[i],word)	==	0)
		{
			if (l_isJongSung)	//	종성이 있는데..
			{
				if (i%2)		//	종성이 없을때 오는 문자다.
				{
					lpStream[0]	=	l_strCheckKarmaJongsung[i-1][0];
					lpStream[1]	=	l_strCheckKarmaJongsung[i-1][1];
				}
			}
			else
			{
				if (i%2 == 0)	//	종성이 있을때 오는 문자다.
				{
					lpStream[0]	=	l_strCheckKarmaJongsung[i+1][0];
					lpStream[1]	=	l_strCheckKarmaJongsung[i+1][1];
				}
			}

			return;
		}
}

//
//	노가다.. -- 성 짙은 콤보 박스
int
cKARMA_WND::CustomComboBox(HWND hwnd,int x,int y,int kind,int select)
{
	int	iCounter=	0;
	int	iWidth	=	0;

	while(1)
	{
		if	(strcmp(l_strComboStr[kind][iCounter],"end")	==	0)
			break;

		int	width	=	strlen(l_strComboStr[kind][iCounter])*6 + 30;

		if	(width > iWidth)
			iWidth	=	width;

		iCounter++;
	}

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,iWidth,600,CBS_DROPDOWNLIST);

	iCounter	=	0;

	while(1)
	{	
		if (strcmp(l_strComboStr[kind][iCounter],"end")	==	0)
			break;

		int	index	=	s_aCombo[g_iComboIndex].Add(l_strComboStr[kind][iCounter]);

		s_aCombo[g_iComboIndex].SetData(index,iCounter);

		iCounter++;
	}

	if	(select >= iCounter)
		select	=	0;

	s_aCombo[g_iComboIndex].Select(select);

	CheckJongsung(l_strComboStr[kind][select]);

	g_iComboIndex++;

	return	iWidth+4;
}	//	cKARMA_WND::CustomComboBox(HWND hwnd,int x,int y,int kind,int select)

/*
	"모든 영웅"
	"파티원 모두"
	"주인공"

	"모든 캐릭터"
	"모든 몬스터"
	"모든 NPC"
*/

int
cKARMA_WND::ComboHero(HWND hwnd,int x,int y,int select)
{
	return	0;
/*	int	width	=	120,i,count;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT);

	s_aCombo[g_iComboIndex].Add(_ms("_%s",dMSG_HERO		),dJOB_HERO);
	s_aCombo[g_iComboIndex].Add(_ms("_%s",dMSG_ALL_HERO	),dJOB_ALL_HERO);
	s_aCombo[g_iComboIndex].Add(_ms("_%s",dMSG_ALL_PARTY),dJOB_ALL_PARTY);

	for (i=0,count = 0;i<dMAX_CHARACTER;i++)
	{
		cCHARACTER	*lpCharacter	=	g_commonCharacterSet.getCharacter(i);

		if (lpCharacter)
		{
			cJOB	*lpJob	=	lpCharacter->getJob();

			if (lpJob->m_wType	==	dJOB_GROUP_HERO)
			{
				s_aCombo[g_iComboIndex].Add(lpCharacter->m_strName,lpCharacter->m_wSerial);
				count++;
			}
		}
	}

	if (g_commonCharacterSet.isAvailCharacter(select))
			s_aCombo[g_iComboIndex].SelectByData(select);
	else	s_aCombo[g_iComboIndex].Select(0);
	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
*/

}

int
cKARMA_WND::ComboPlayerJob(HWND hwnd,int x,int y,int select)
{
	int	width	=	120,iIndex = 0;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	iIndex	=	0;

	for (iIndex=0;;iIndex++)
	{
		if	(stricmp(g_aDefaultJobName[iIndex],"end")==0)
			break;

		s_aCombo[g_iComboIndex].Add(g_aDefaultJobName[iIndex],iIndex);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select)	==	CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	CheckJongsung(g_aDefaultJobName[select]);

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboQuestJob(HWND hwnd,int x,int y,int select)
{
	int	width	=	120,iIndex = 0;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	iIndex	=	0;

	CQuestItem	*lpQuest	=	&g_quest.m_aQuest[s_iQuestJobQuest];

	for (iIndex=0;;iIndex++)
	{
		int	iCheckJob	=	iIndex;

		if	(iCheckJob	>=	18)
			iCheckJob	-=	2;

		if	(stricmp(g_aDefaultJobName[iIndex],"end")==0)
			break;

		if	(iIndex	==	16)
		{
			if	(lpQuest->m_bf1IsMaleJobEquipAble	==	FALSE)
				continue;
		}
		else
		if	(iIndex	==	17)
		{
			if	(lpQuest->m_bf1IsFemaleJobEquipAble	==	FALSE)
				continue;
		}
		else
		if	(lpQuest->isEnableJob(iCheckJob)	==	FALSE)
			continue;

		s_aCombo[g_iComboIndex].Add(g_aDefaultJobName[iIndex],iIndex);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select)	==	CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	CheckJongsung(g_aDefaultJobName[select]);

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboArbeitJob(HWND hwnd,int x,int y,int select)
{
	int	width	=	120,iIndex = 0;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	iIndex	=	0;

	CQuestItem	*lpQuest	=	&g_questArbeit.m_aQuest[s_iQuestArbeitJobQuest];

	for (iIndex=0;;iIndex++)
	{
		int	iCheckJob	=	iIndex;

		if	(iCheckJob	>=	18)
			iCheckJob	-=	2;

		if	(stricmp(g_aDefaultJobName[iIndex],"end")==0)
			break;

		if	(iIndex	==	16)
		{
			if	(lpQuest->m_bf1IsMaleJobEquipAble	==	FALSE)
				continue;
		}
		else
		if	(iIndex	==	17)
		{
			if	(lpQuest->m_bf1IsFemaleJobEquipAble	==	FALSE)
				continue;
		}
		else
		if	(lpQuest->isEnableJob(iCheckJob)	==	FALSE)
			continue;

		s_aCombo[g_iComboIndex].Add(g_aDefaultJobName[iIndex],iIndex);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select)	==	CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	CheckJongsung(g_aDefaultJobName[select]);

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboCaster(HWND hwnd,int x,int y,int select)
{
	return	0;
/*	int	width	=	120,i,count;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	s_aCombo[g_iComboIndex].Add(_ms("_%s",dMSG_SOMETHING),dJOB_SOMETHING);

	for (i=0,count = 0;i<dMAX_CHARACTER && count < g_commonCharacterSet.m_iCount;i++)
	{
		cCHARACTER	*lpCharacter	=	g_commonCharacterSet.getCharacter(i);

		if (lpCharacter)
		{
			s_aCombo[g_iComboIndex].Add(lpCharacter->m_strName,lpCharacter->m_wSerial);
			count++;
		}
	}

	if (g_commonCharacterSet.isAvailCharacter(select))
			s_aCombo[g_iComboIndex].SelectByData(select);
	else	s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
*/
}

int
cKARMA_WND::ComboOnlyHero(HWND hwnd,int x,int y,int select)
{
	return	0;
/*
	int	width	=	120,i,count;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (i=0,count = 0;i<dMAX_CHARACTER&& count < g_commonCharacterSet.m_iCount;i++)
	{
		cCHARACTER	*lpCharacter	=	g_commonCharacterSet.getCharacter(i);

		if (lpCharacter)
		{
			s_aCombo[g_iComboIndex].Add(lpCharacter->m_strName,lpCharacter->m_wSerial);

			count++;
		}
	}

	if (g_commonCharacterSet.isAvailCharacter(select))
			s_aCombo[g_iComboIndex].SelectByData(select);
	else	s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
*/
}

int
cKARMA_WND::ComboJob(HWND hwnd,int x,int y,int select)
{
	int	width	=	120,index = 0;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

/*	영웅
	while(1)
	{
		if (g_aHeroBodyData[index].m_iBody	==	0xffff)	break;

		s_aCombo[g_iComboIndex].Add(g_aHeroBodyData[index].m_strName,g_aHeroBodyData[index].m_iBody);

		index++;
	}
*/
	index	=	0;
//	몬스터
	while(1)
	{
		if (g_aMonsterBodyData[index].m_iBody	==	0xffff)	break;

		s_aCombo[g_iComboIndex].Add(g_aMonsterBodyData[index].m_strName,g_aMonsterBodyData[index].m_iBody);

		index++;
	}

	s_aCombo[g_iComboIndex].Select(g_aDefaultJobName[select]);

	CheckJongsung(g_aDefaultJobName[select]);

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboMonster(HWND hwnd,int x,int y,int select)
{
	int	width	=	120;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	for (int iJob=0;iJob<dMAX_JOB_COUNT;iJob++)
	{
		cJOB	*lpJobInfo	=	g_denJob.getJob(iJob);

		if	(!lpJobInfo || lpJobInfo->m_wType	!=	1)
			continue;

		s_aCombo[g_iComboIndex].Add(lpJobInfo->m_strName,iJob);
	}
	
	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}


int
cKARMA_WND::ComboWorldValue	(HWND hwnd,int x,int y,int select)
{
	int	width	=	120,i;
	x	+=	2;
	
	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);
	
 	for (i=0;i<50;i++)
 		s_aCombo[g_iComboIndex].Add(GetNumberText(i),i);
	
	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);
	
	g_iComboIndex++;
	
	return	width+24;
}

int
cKARMA_WND::ComboEventTimeName	(HWND hwnd,int x,int y,int select)
{
	int	width	=	240,i;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	for (i=0;;i++)
	{
		if	(strcmp(g_strEventTimeName[i],"end")==NULL)
			break;

		s_aCombo[g_iComboIndex].Add(g_strEventTimeName[i],i);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboFieldValue	(HWND hwnd,int x,int y,int select)
{
	int	width	=	120,i;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	for (i=0;i<50;i++)
		s_aCombo[g_iComboIndex].Add(g_lpActmap->m_astrFieldValue[i],i);

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboCharacter(HWND hwnd,int x,int y,int select)
{
	int	width	=	120,i,count;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	cCHARACTER_SET	*lpJobSet;
	cCHARACTER		*lpCharacter;

	lpJobSet	=	&g_lpActmap->m_setCharacter	;

	for (i=0,count = 0;i<dMAX_CHARACTER&& count < lpJobSet->m_iCount;i++)
	{
		lpCharacter	=	lpJobSet->getCharacter(i);

		if (lpCharacter)
			s_aCombo[g_iComboIndex].Add(lpCharacter->m_strName,lpCharacter->m_wSerial);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboAllCharacter(HWND hwnd,int x,int y,int select)
{
	int	width	=	120,i,count;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	cCHARACTER_SET	*lpJobSet;
	cCHARACTER		*lpCharacter;

	lpJobSet	=	&g_lpActmap->m_setCharacter	;

	s_aCombo[g_iComboIndex].Add("All monsters" ,dCHARACTER_ALL);
	s_aCombo[g_iComboIndex].Add("self" ,dCHARACTER_SELF);

	for (i=0,count = 0;i<dMAX_CHARACTER&& count < lpJobSet->m_iCount;i++)
	{
		lpCharacter	=	lpJobSet->getCharacter(i);

		if (lpCharacter)
			s_aCombo[g_iComboIndex].Add(lpCharacter->m_strName,lpCharacter->m_wSerial);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)	
		s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboAllCharacterTargetPlayer(HWND hwnd,int x,int y,int select)
{
	int	width	=	120,i,count;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	cCHARACTER_SET	*lpJobSet;
	cCHARACTER		*lpCharacter;

	lpJobSet	=	&g_lpActmap->m_setCharacter	;

	s_aCombo[g_iComboIndex].Add("All players" ,dCHARACTER_PLAYER);
	s_aCombo[g_iComboIndex].Add("Target Player" ,dCHARACTER_TARGET_PLAYER);
	s_aCombo[g_iComboIndex].Add(dMSG_TARGET_PARTY	,dCHARACTER_TARGET_PARTY);
	s_aCombo[g_iComboIndex].Add("self" ,dCHARACTER_SELF);
	s_aCombo[g_iComboIndex].Add("All monsters" ,dCHARACTER_ALL);

	for (i=0,count = 0;i<dMAX_CHARACTER&& count < lpJobSet->m_iCount;i++)
	{
		lpCharacter	=	lpJobSet->getCharacter(i);

		if (lpCharacter)
			s_aCombo[g_iComboIndex].Add(lpCharacter->m_strName,lpCharacter->m_wSerial);
	}

	if	(s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboAllCharacterTargetPlayer2(HWND hwnd,int x,int y,int select)
{
	int	width	=	120,i,count;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	cCHARACTER_SET	*lpJobSet;
	cCHARACTER		*lpCharacter;

	lpJobSet	=	&g_lpActmap->m_setCharacter	;

	s_aCombo[g_iComboIndex].Add("Target" ,0);
	s_aCombo[g_iComboIndex].Add("All party members" ,1);
	s_aCombo[g_iComboIndex].Add("Party Leader" ,2);
	s_aCombo[g_iComboIndex].Add("Anyone in the party" ,3);
	s_aCombo[g_iComboIndex].Add("All monsters" ,4);

	for (i=0,count = 0;i<dMAX_CHARACTER&& count < lpJobSet->m_iCount;i++)
	{
		lpCharacter	=	lpJobSet->getCharacter(i);

		if	(lpCharacter)
			s_aCombo[g_iComboIndex].Add(lpCharacter->m_strName,lpCharacter->m_wSerial+10000);
	}

	if	(s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboArea(HWND hwnd,int x,int y,int select)
{
	int	width	=	120;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (int i=2,count=2;i < dMAX_AREA && count <g_lpActmap->m_pArea->m_iCount ;i++)
	{
		if (g_lpActmap->m_pArea->m_aArea[i].m_wSerial	!=	0xffff)
		{
			int	index	=	s_aCombo[g_iComboIndex].Add(g_lpActmap->m_pArea->m_aArea[i].m_strName);

			s_aCombo[g_iComboIndex].SetData(index,i);

			count++;
		}
	}

	if (s_aCombo[g_iComboIndex].Select(g_lpActmap->m_pArea->m_aArea[select].m_strName) == CB_ERR)	s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(g_lpActmap->m_pArea->m_aArea[select].m_strName);

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboTrapArea(HWND hwnd,int x,int y,int select)
{
	int	width	=	120;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (int i=0;i < dMAX_AREA ;i++)
	{
		if (g_lpActmap->m_pArea->m_aArea[i].m_wSerial	==	0xffff)
			continue;
		if (g_lpActmap->m_pArea->m_aArea[i].m_wKind		!=	dAREA_TRAP)
			continue;

		int	index	=	s_aCombo[g_iComboIndex].Add(g_lpActmap->m_pArea->m_aArea[i].m_strName);

		s_aCombo[g_iComboIndex].SetData(index,i);
	}

	if (s_aCombo[g_iComboIndex].Select(g_lpActmap->m_pArea->m_aArea[select].m_strName) == CB_ERR)
	{
		select	=	0;
		s_aCombo[g_iComboIndex].Select(0);
	}

	CheckJongsung(g_lpActmap->m_pArea->m_aArea[select].m_strName);

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboAllArea(HWND hwnd,int x,int y,int select)
{
	int	width	=	120;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	int	index	=	s_aCombo[g_iComboIndex].Add(g_lpActmap->m_pArea->m_aArea[0].m_strName,0);

	for (int i=2,count=2;i < dMAX_AREA && count <g_lpActmap->m_pArea->m_iCount ;i++)
	{
		if (g_lpActmap->m_pArea->m_aArea[i].m_wSerial	!=	0xffff)
		{
			int	index	=	s_aCombo[g_iComboIndex].Add(g_lpActmap->m_pArea->m_aArea[i].m_strName);

			s_aCombo[g_iComboIndex].SetData(index,i);

			count++;
		}
	}

	if (s_aCombo[g_iComboIndex].Select(g_lpActmap->m_pArea->m_aArea[select].m_strName) == CB_ERR)	s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(g_lpActmap->m_pArea->m_aArea[select].m_strName);

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboOnlyArea(HWND hwnd,int x,int y,int select)
{
	int	width	=	120;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (int i=2,count = 2;i < dMAX_AREA && count <g_lpActmap->m_pArea->m_iCount ;i++)
	{
		if (g_lpActmap->m_pArea->m_aArea[i].m_wSerial	!=	0xffff)
		{
			int	index	=	s_aCombo[g_iComboIndex].Add(g_lpActmap->m_pArea->m_aArea[i].m_strName);

			s_aCombo[g_iComboIndex].SetData(index,i);

			count++;
		}
	}

	if (select	==	0)
	{
		select	=	g_lpActmap->m_pArea->GetFirstArea();

		if (select	==	0xffff)		return	0xffff;
	}

	s_aCombo[g_iComboIndex].Select(g_lpActmap->m_pArea->m_aArea[select].m_strName);

	CheckJongsung(g_lpActmap->m_pArea->m_aArea[select].m_strName);

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboValue(HWND hwnd,int x,int y,int select)
{
	int	width	=	28*6;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	cVALUE	*lpValue;

	for (int iVaueKind=0;iVaueKind<2;iVaueKind++)
	{
		if (iVaueKind	==	0)
			lpValue	=	&g_ProjectValue;
		else
			lpValue	=	&g_lpActmap->m_Value;

		for (int i=0,count = 0;i < dVALUE_COUNT && count < lpValue->m_iCount ;i++)
		{
			if (lpValue->m_aValue[i].m_iValue	!=	dVALID_VALUE)
			{
				if (iVaueKind	==	0)
					s_aCombo[g_iComboIndex].Add(_ms("_%s",lpValue->GetName(i)),i);
				else
					s_aCombo[g_iComboIndex].Add(lpValue->GetName(i),i);

				count++;
			}
		}
	}

	if (select	>=	dVALUE_COUNT)
	{
		select	-=	dVALUE_COUNT;

		s_aCombo[g_iComboIndex].Select(g_lpActmap->m_Value.GetName(select));
		CheckJongsung(g_lpActmap->m_Value.GetName(select));
	}
	else
	{
		s_aCombo[g_iComboIndex].Select(_ms("_%s",g_ProjectValue.GetName(select)));
		CheckJongsung(g_ProjectValue.GetName(select));
	}

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboMagic(HWND hwnd,int x,int y,int select)
{
	int	width	=	120;
	x	+=	2;

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboBookList(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	if	(g_book.m_iBookCount	==	0)
		return	0;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,800,CBS_DROPDOWNLIST);

	for (int i=0;i<g_book.m_iBookCount;i++)
		s_aCombo[g_iComboIndex].Add(g_book.getBook(i)->m_strBookName,i);

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	CheckJongsung(g_book.getBook(select)->m_strBookName);

	g_iComboIndex++;

	return	iWidth+24;

}

int
cKARMA_WND::ComboDungeonValue(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,800,CBS_DROPDOWN);

	s_iDungeonvalueComboBox			=	IDC_KARMA_COMBO+g_iComboIndex;

	cSecretDungeonValue	*lpString	=	g_sdManager.getString(g_lpActmap->m_wLinkSecretDungeon);

	if	(!lpString)
	{
		s_aCombo[g_iComboIndex].Add("Please set the bidon to link first.",0);
		s_aCombo[g_iComboIndex].Select(0);
		g_iComboIndex++;
		return iWidth+24;
	}

	for (int i=0;i<dMAX_DUNGEON_VALUE_COUNT;i++)
		s_aCombo[g_iComboIndex].Add(lpString->m_strDungeonValue[i],i);

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	CheckJongsung(lpString->m_strDungeonValue[select]);

	s_iSelectDungeonValue	=	select;

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboMainQuestValue(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,800,CBS_DROPDOWN);

	s_iMainQuestValueComboBox		=	IDC_KARMA_COMBO+g_iComboIndex;

	for (int i=0;i<dMAX_DUNGEON_VALUE_COUNT;i++)
	{
		if	(stricmp(g_strMainQuestValue[i],"end")	==	0)
			break;

		s_aCombo[g_iComboIndex].Add(g_strMainQuestValue[i],i);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	CheckJongsung(g_strMainQuestValue[select]);

	s_iSelectMainQuestValue	=	select;

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboMainQuestSwitch(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,800,CBS_DROPDOWN);

	s_iMainQuestSwitchComboBox		=	IDC_KARMA_COMBO+g_iComboIndex;

	for (int i=0;i<c_iUseMainQuestSwitchCount;i++)
		s_aCombo[g_iComboIndex].Add(g_strMainQuestSwitch[i],i);

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	CheckJongsung(g_strMainQuestSwitch[select]);

	s_iSelectMainQuestSwitch	=	select;

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboRelatedPlaceOfGuild(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,800,CBS_DROPDOWNLIST);

	for (int i=0;;i+=2)
	{
		if	(stricmp(g_strRelatedPlaceOfGuildList[i],"end")	==	0)
			break;

		int	iSerial	=	atoi(g_strRelatedPlaceOfGuildList[i+1]);

		s_aCombo[g_iComboIndex].Add(g_strRelatedPlaceOfGuildList[i],iSerial);
	}

	if	(s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	s_aCombo[g_iComboIndex].GetData();
	}

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboGuildStatueSkill(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,800,CBS_DROPDOWNLIST);

	for (int i=100;i<dMAX_GUILD_SKILL_COUNT;i++)
	{
		CGuildSkill	*lpGS	=	&g_gsManager.m_aSkills[i];

		if	(lpGS->m_wSerial==	0xffff)
			continue;

		s_aCombo[g_iComboIndex].Add(lpGS->m_strName,i);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	CGuildSkill	*lpSelectGS	=	&g_gsManager.m_aSkills[select];

	CheckJongsung(lpSelectGS->m_strName);

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboPattern(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,800,CBS_DROPDOWNLIST);

	cCharacterPatternPack	*lpPP	=	&g_lpActmap->m_cpp;

	if	(lpPP->m_wCount	==	0)
		return	0xffff;

	for (int i=0;i<c_iCharacterPatternCountInField;i++)
	{
		cCharacterPattern	*lpPattern	=	lpPP->get(i);

		if	(!lpPattern)
			continue;

		s_aCombo[g_iComboIndex].Add(lpPattern->m_strName,i);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	CheckJongsung(lpPP->get(select)->m_strName);

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboChapter(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,800,CBS_DROPDOWN);

	if	(g_chapterManager.m_iChapterCount	==	0)
		return	0xffff;

	for (int i=0;i<g_chapterManager.m_iChapterCount;i++)
		s_aCombo[g_iComboIndex].Add(g_chapterManager.get(i)->m_strName,i);

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	CheckJongsung(g_chapterManager.get(select)->m_strName);

	s_iSelectMainQuestSwitch	=	select;

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::CustomSoundFile(HWND hwnd,int x,int y,char *_lpstrSelectText)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,800,CBS_DROPDOWNLIST|CBS_SORT);

	SetLogFolder();
	SetCurrentDirectory("data/sound");

	cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.wav");

	SetLogFolder();

	if	(!lpFiles	||	lpFiles->Count	==	0)
		return	0xffff;

	cSTRING		*lpFile		=	lpFiles->Pick();

	for (int i=0;i<lpFiles->Count;i++)
	{
		s_aCombo[g_iComboIndex].Add(lpFile->String,i);
		lpFile		=	lpFile->pNEXT;
	}

	if	(s_aCombo[g_iComboIndex].Select(_lpstrSelectText) == CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);

		_lpstrSelectText	=	s_aCombo[g_iComboIndex].GetString(0);
	}

	CheckJongsung(_lpstrSelectText);

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboSkill(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,800,CBS_DROPDOWNLIST);

	for (int i=0;i<dMAX_SKILL;i++)
	{
		if (g_aSkill[i].m_wSerial	==	0xffff)
			continue;

		int	iJob	=	i/25;

		if (iJob	<	dPLAYER_JOB_COUNT)
		{
			char	strSkillName[128];

			sprintf(strSkillName,"[%s] %s",g_aDefaultJobName[iJob],g_aSkill[i].m_strName);

			s_aCombo[g_iComboIndex].Add(strSkillName,i);
		}
		else
			s_aCombo[g_iComboIndex].Add(g_aSkill[i].m_strName,i);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	CheckJongsung(g_aSkill[select].m_strName);

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboItem(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (int i=1;i<dBASIC_ITEM_COUNT;i++)
	{
		cBASIC_ITEM	*lpItem	=	&g_aBasicItem[i];

		if (lpItem->m_iSerial		==	-1)
			continue;

		s_aCombo[g_iComboIndex].Add(lpItem->m_strName,i);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(g_aBasicItem[select].m_strName);

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboEquipment(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (int i=1;i<dBASIC_ITEM_COUNT;i++)
	{
		cBASIC_ITEM	*lpItem	=	&g_aBasicItem[i];

		if (!lpItem->isEquipment())	
			continue;

		if (lpItem->m_iSerial		==	-1)
			continue;


		s_aCombo[g_iComboIndex].Add(lpItem->m_strName,i);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)	s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(g_aBasicItem[select].m_strName);

	g_iComboIndex++;

	return	iWidth+24;
}	//	cKARMA_WND::ComboEquipment(HWND hwnd,int x,int y,int select)

int
cKARMA_WND::ComboSpecificItem(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	CCustomItemManager	*lpIM	=	&g_lpActmap->m_customItem;

	if (lpIM->getCount() == 0)	return	0xffff;


	for (int i=0;i<dMAX_CUSTOM_ITEM_COUNT;i++)
	{
		CCustomItem	*lpItem	=	lpIM->getItem(i);

		if (!lpItem)	continue;

		s_aCombo[g_iComboIndex].Add(lpItem->m_strName,i);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)	s_aCombo[g_iComboIndex].Select(0);

	select	=	s_aCombo[g_iComboIndex].GetData();

	CCustomItem	*lpItem	=	lpIM->getItem(select);

	CheckJongsung(((cITEM *)lpItem)->getName(FALSE,FALSE));

	g_iComboIndex++;

	return	iWidth+24;
}	//	cKARMA_WND::ComboSpecificItem(HWND hwnd,int x,int y,int select)

int
cKARMA_WND::ComboSetElementItem(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (int i=1;i<g_itemSetManager.getMaxCount();i++)
	{
		cItemSet	*lpGroup	=	g_itemSetManager.get(i);

		if (!lpGroup)
			continue;

		for (int iIndex=0;iIndex<dMAX_ITEM_COUNT_IN_SET;iIndex++)
		{
			cSetItem	*lpSetItem	=	lpGroup->getItem(iIndex);

			if (!lpSetItem)
				continue;

			if (!lpSetItem->m_bf1IsDropInQuest)
				continue;

			s_aCombo[g_iComboIndex].Add(_ms("[%s] %s",lpGroup->m_strName,g_aBasicItem[lpSetItem->m_wBaseItem].m_strName),i*0x8000 + iIndex);
		}
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboItemPiece(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (int i=1;i<g_pieceItemManager.getMaxCount();i++)
	{
		cPieceItem	*lpPieceItem	=	g_pieceItemManager.get(i);

		if (!lpPieceItem)
			continue;

		s_aCombo[g_iComboIndex].Add(_ms("[G]%s",lpPieceItem->m_strName),i);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboSpecialItem(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (int i=1;i<dBASIC_ITEM_COUNT;i++)
	{
		cBASIC_ITEM	*lpItem	=	&g_aBasicItem[i];

		if (lpItem->m_wKind	!=	eIK_SPECIAL && lpItem->m_attr.isGuildBattleItem == FALSE)
			continue;

		if (lpItem->m_iSerial		==	-1)
			continue;

		if (lpItem->m_attr.isGuildBattleItem)
				s_aCombo[g_iComboIndex].Add(_ms("[G]%s",lpItem->m_strName),i);
		else	s_aCombo[g_iComboIndex].Add(lpItem->m_strName,i);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)	s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(g_aBasicItem[select].m_strName);

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboQuestItem(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220,iCount = 0;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (int i=1;i<dBASIC_ITEM_COUNT;i++)
	{
		cBASIC_ITEM	*lpItem	=	&g_aBasicItem[i];

		if (lpItem->m_questIndex	==	dNOT_QUEST_ITEM)
			continue;
		if (lpItem->m_iSerial		==	-1)
			continue;

		s_aCombo[g_iComboIndex].Add(lpItem->m_strName,i);
		iCount++;
	}

	if (iCount	<=	0)
		return	0xffff;

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
	{
		select				=	0;
		s_aCombo[g_iComboIndex].Select(0);
	}

	CheckJongsung(g_aBasicItem[select].m_strName);

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboTitle(HWND hwnd,int x,int y,int select)
{
	int	iWidth	=	220,iCount = 0;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO+g_iComboIndex,x,y-4,iWidth+20,600,CBS_DROPDOWNLIST);

	for (int i=0;i<dMAX_TITLE_COUNT;i++)
	{
		CTitle	*lpTitle	=	g_titleManager.getTitle(i);

		if (!lpTitle)	continue;

		s_aCombo[g_iComboIndex].Add(lpTitle->m_strName,lpTitle->m_wSerial);
		iCount++;
	}

	if (iCount	<=	0)	return	0xffff;

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)	
		s_aCombo[g_iComboIndex].Select(0);

	CTitle	*lpSelectTitle	=	g_titleManager.getTitle(select);

	if (lpSelectTitle	==	NULL)
	{
		select			=	s_aCombo[g_iComboIndex].GetData();
		lpSelectTitle	=	g_titleManager.getTitle(select);
	}

	if (lpSelectTitle)
		CheckJongsung(lpSelectTitle->m_strName);

	g_iComboIndex++;

	return	iWidth+24;
}

int
cKARMA_WND::ComboArmor(HWND hwnd,int x,int y,int select)
{
	int	width	=	120;
	x	+=	2;

	g_iComboIndex++;

	return	width+24;
}


int
cKARMA_WND::ComboMagicEffect(HWND hwnd,int x,int y,int select)
{
	int	width	=	120;
	x	+=	2;

	g_iComboIndex++;

	return	width+24;
}


int
cKARMA_WND::ComboImageFile(HWND hwnd,int x,int y,char *lpString)
{
	int	width	=	120,iFileCount=0;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	cPROJECT::SetProjectFolder();

	char	*strExts[]	=	{"*.jpg","*.pcx","*.bmp"};

	cSTRINGS	*lpFileList;

	for (int ext=0;ext<3;ext++)
	{
		lpFileList	=	cFOLDER::GetFiles(strExts[ext]);	//	화일들을 얻어 온다.

		if (lpFileList)
		{
			cSTRING	*lpFile	=	lpFileList->Pick();

			for (int i=0;i<lpFileList->Count;i++)
			{
				s_aCombo[g_iComboIndex].Add(lpFile->String,i);

				lpFile	=	lpFile->pNEXT;
			}

			iFileCount	+=	lpFileList->Count;
		}
	}

	if (iFileCount	<=	0)	return	0xffff;

	SetLogFolder();

	if (lpString)
	{
		if (s_aCombo[g_iComboIndex].Select(lpString)	==	CB_ERR)
			s_aCombo[g_iComboIndex].Select(0);
	}
	else	s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}


int
cKARMA_WND::ComboMusicFile(HWND hwnd,int x,int y,char *lpString)
{
	int	width	=	120,iFileCount=0;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	cPROJECT::SetProjectFolder();

	char	*strExts[]	=	{"*.mp3","*.wav","*.mid"};

	cSTRINGS	*lpFileList;

	for (int ext=0;ext<3;ext++)
	{
		lpFileList	=	cFOLDER::GetFiles(strExts[ext]);	//	화일들을 얻어 온다.

		if (lpFileList)
		{
			cSTRING	*lpFile	=	lpFileList->Pick();

			for (int i=0;i<lpFileList->Count;i++)
			{
				s_aCombo[g_iComboIndex].Add(lpFile->String,i);

				lpFile	=	lpFile->pNEXT;
			}

			iFileCount	+=	lpFileList->Count;
		}
	}

	if (iFileCount	<=	0)	return	0xffff;

	SetLogFolder();

	if (lpString)
	{
		if (s_aCombo[g_iComboIndex].Select(lpString)	==	CB_ERR)
			s_aCombo[g_iComboIndex].Select(0);
	}
	else	s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}


int
cKARMA_WND::ComboSoundFile(HWND hwnd,int x,int y,char *lpString)
{
	int	width	=	120;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	cPROJECT::SetProjectFolder();

	cSTRINGS	*lpFileList;

	lpFileList	=	cFOLDER::GetFiles("*.wav");	//	화일들을 얻어 온다.

	if (lpFileList)
	{
		cSTRING	*lpFile	=	lpFileList->Pick();

		for (int i=0;i<lpFileList->Count;i++)
		{
			s_aCombo[g_iComboIndex].Add(lpFile->String,i);

			lpFile	=	lpFile->pNEXT;
		}
	}	else	return	0xffff;

	SetLogFolder();

	if (lpString)
	{
		if (s_aCombo[g_iComboIndex].Select(lpString)	==	CB_ERR)
			s_aCombo[g_iComboIndex].Select(0);
	}
	else	s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}


int
cKARMA_WND::ComboDoor(HWND hwnd,int x,int y,int select)
{
	int	width	=	120;
	x	+=	2;

	int	iDoorCount	=	0;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (int i=0,count = 0;i < dMAX_AREA && count < g_lpActmap->m_pArea->m_iCount ;i++)
	{
		if (g_lpActmap->m_pArea->IsAvail(i))
		{
			if (g_lpActmap->m_pArea->IsDoor(i))
			{
				s_aCombo[g_iComboIndex].Add(g_lpActmap->m_pArea->GetName(i),i);
				iDoorCount++;
			}

			count++;
		}
	}

	if (iDoorCount	==	0)	return	0xffff;

	if (!g_lpActmap->m_pArea->IsDoor(select))	s_aCombo[g_iComboIndex].Select(0);
	else										s_aCombo[g_iComboIndex].Select(g_lpActmap->m_pArea->GetName(select));

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboBox(HWND hwnd,int x,int y,int select)
{
	int	width	=	120;
	x	+=	2;

	g_iComboIndex++;

	return	width+24;
}

//
//
int
cKARMA_WND::ComboBattleInclination(HWND hwnd,int x,int y,int select)
{
	int	width	=	300;
	x	+=	2;

	int	iDoorCount	=	0;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (int i=0;;i++)
	{
		if (stricmp(g_strBattleInclination[i],"end")==0) break;

		s_aCombo[g_iComboIndex].Add(g_strBattleInclination[i],i);
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR) s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(g_strBattleInclination[select]);

	g_iComboIndex++;

	return	width+24;
//	g_strBattleInclination
}	//	cKARMA_WND::ComboBattleInclination(HWND hwnd,int x,int y,int select)

//
//
int
cKARMA_WND::ComboScenario(HWND hwnd,int x,int y,int _iSelect)
{
	int	width	=	200;

	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	cPROJECT::SetMapFolder();

	cSTRINGS	*lpFileList	=	cFOLDER::GetFiles("*.rmd");//	화일들을 얻어 온다.

	if (lpFileList)
	{
		cSTRING	*lpFile	=	lpFileList->Pick();

		for (int i=0;i<lpFileList->Count;i++)
		{
			int	iFieldSerial	=	atoi(lpFile->String+1);

			s_aCombo[g_iComboIndex].Add(lpFile->String,iFieldSerial);

			lpFile	=	lpFile->pNEXT;
		}
	}	else	return	0xffff;

	SetLogFolder();

	s_aCombo[g_iComboIndex].SelectByData(_iSelect);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboQuest(HWND hwnd,int x,int y,int select)
{
	int	width	=	0,i,count ;
	x	+=	2;

	if (g_quest.m_iCount	<=	0)	return	0xffff;

	for (i=0,count = 0;i < dQUEST_COUNT && count < g_quest.m_iCount ;i++)
	{
		if (g_quest.m_aQuest[i].m_wQuestType != eQT_DEATH)
			width	=	max(strlen(g_quest.getName(i))*6,(DWORD)width);

		count++;
	}

	width	+=	10;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (i=0,count = 0;i < dQUEST_COUNT && count < g_quest.m_iCount ;i++)
	{
		if (g_quest.m_aQuest[i].m_wQuestType != eQT_DEATH)	s_aCombo[g_iComboIndex].Add(g_quest.getName(i),i);

		count++;
	}

	if (select	>=	dQUEST_COUNT)
		select	=	0;

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	s_iQuestJobQuest		=	select;


	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboQuestArbeit(HWND hwnd,int x,int y,int select)
{
	int	width	=	0,i,count;
	x	+=	2;

	if (g_questArbeit.m_iCount	<=	0)
		return	0xffff;

	for (i=0,count = 0;i < dQUEST_COUNT && count < g_questArbeit.m_iCount ;i++)
	{
		if (g_questArbeit.m_aQuest[i].m_wQuestType != eQT_DEATH)
			width	=	max(strlen(g_questArbeit.getName(i))*6,(DWORD)width);

		count++;
	}

	width	+=	10;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_SORT|CBS_DROPDOWNLIST);

	for (i=0,count = 0;i < dQUEST_COUNT && count < g_questArbeit.m_iCount ;i++)
	{
		if (g_questArbeit.m_aQuest[i].m_wQuestType != eQT_DEATH)	s_aCombo[g_iComboIndex].Add(g_questArbeit.getName(i),i);

		count++;
	}

	if (select	>=	dQUEST_COUNT)	select	=	0;

	if (s_aCombo[g_iComboIndex].SelectByData(select) == CB_ERR)
	{
		s_aCombo[g_iComboIndex].Select(0);
		select	=	0;
	}

	s_iQuestArbeitJobQuest		=	select;

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboPower(HWND hwnd,int x,int y,int select)
{
	int	width	=	120,index = 0;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	while(1)
	{
		if (g_aPowers[index].m_iType	==	0xffff)	break;

		s_aCombo[g_iComboIndex].Add(g_aPowers[index].m_strName,g_aPowers[index].m_iType);

		index++;
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select)	==	CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);


	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboSpeech(HWND hwnd,int x,int y,int select)
{
	int	width	=	620,index = 0;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	while(1)
	{
		if (strcmp(g_strCurrentSpeechList[index],"")==0)
			break;

		s_aCombo[g_iComboIndex].Add(g_strCurrentSpeechList[index],index);

		index++;
	}

	if (index	==	0)	return	0xffff;

	if (s_aCombo[g_iComboIndex].SelectByData(select)	==	CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);


	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

//	ShowWindow(GetDlgItem(hwnd,IDC_ACTOR_POWER_ROUND),SW_SHOW);
//	ShowWindow(GetDlgItem(hwnd,IDC_ACTOR_POWER),SW_SHOW);

	return	width+24;
}

//
//	상점 콤보
int
cKARMA_WND::ComboShop(HWND hwnd,int x,int y,int select)
{
	CShopManager	*lpShopList	=	&g_lpActmap->m_shop;

	int	width	=	120,index = 0,iCount=0;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	for (index=0;index<dMAX_SHOP;index++)
	{
		CShop	*lpShop	=	lpShopList->get(index);

		if (lpShop==NULL)	continue;

		s_aCombo[g_iComboIndex].Add(lpShop->m_strName,index);

		iCount++;
	}

	if (iCount	==	0)	return	0xffff;

	if (s_aCombo[g_iComboIndex].SelectByData(select)	==	CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

	return	width+24;
}	//	cKARMA_WND::ComboShop(HWND hwnd,int x,int y,int select)

int
cKARMA_WND::ComboLimitedPower(HWND hwnd,int x,int y,int select)
{
	int	width	=	120,index = 0;
	x	+=	2;

	s_aCombo[g_iComboIndex].Init(hwnd,s_hInst,IDC_KARMA_COMBO	+	g_iComboIndex,x,y-4,width+20,600,CBS_DROPDOWNLIST);

	while(1)
	{
		if (g_aLimitedPowers[index].m_iType	==	0xffff)	break;

		s_aCombo[g_iComboIndex].Add(g_aLimitedPowers[index].m_strName,g_aLimitedPowers[index].m_iType);

		index++;
	}

	if (s_aCombo[g_iComboIndex].SelectByData(select)	==	CB_ERR)
		s_aCombo[g_iComboIndex].Select(0);

	CheckJongsung(s_aCombo[g_iComboIndex].GetString());

	g_iComboIndex++;

//	ShowWindow(GetDlgItem(hwnd,IDC_ACTOR_POWER_ROUND),SW_SHOW);
//	ShowWindow(GetDlgItem(hwnd,IDC_ACTOR_POWER),SW_SHOW);
//	SetWindowText(GetDlgItem(hwnd,IDC_ACTOR_POWER),dMSG_ACTOR_POWER_LIMIT);
	return	width+24;
}

int
cKARMA_WND::ComboStatus(HWND hwnd,int x,int y,int select)
{
	int	width	=	120;
	x	+=	2;

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::ComboAbility(HWND hwnd,int x,int y,int select)
{
	int	width	=	120;
	x	+=	2;

	g_iComboIndex++;

	return	width+24;
}

int
cKARMA_WND::KarmaTextSpin(HWND hwnd,int x,int y,int select)
{
	int	width	=	70;
	x	+=	2;

	int	iMinValue	=	l_Reader.GetNumber();
	int	iMaxValue	=	l_Reader.GetNumber();

	l_Reader.FindCharOver(']');

	select	=	min(iMaxValue,max(iMinValue,select));

			if(iMaxValue	>	1000000000)	width	=	10;
	else	if(iMaxValue	>	100000000)	width	=	9;
	else	if(iMaxValue	>	10000000)	width	=	8;
	else	if(iMaxValue	>	1000000)	width	=	7;
	else	if(iMaxValue	>	100000)		width	=	6;
	else	if(iMaxValue	>	10000)		width	=	5;
	else									width	=	4;

	width	=	(width+4)*6;

	l_aSpinBox	[g_iSpinIndex].Init(hwnd,s_hInst,IDC_KARMA_SPINBOX	+g_iSpinIndex	,_ms("%d",select),x,y-4,width,20,ES_NUMBER|WS_BORDER);
	l_aSpin		[g_iSpinIndex].Init(hwnd,s_hInst,IDC_KARMA_SPIN		+g_iSpinIndex	,l_aSpinBox	[g_iSpinIndex].hWND,iMinValue,iMaxValue,select);

	g_iSpinIndex++;


	return	width+5;
}

int
cKARMA_WND::KarmaTextNumber(HWND hwnd,int x,int y,int select)
{
	int	width	=	70;
	x	+=	2;

	int	iMinValue	=	l_Reader.GetNumber();
	int	iMaxValue	=	l_Reader.GetNumber();

	l_Reader.FindCharOver(']');

	select			=	min(iMaxValue,max(iMinValue,select));

			if(iMaxValue	>	1000000000)	width	=	10;
	else	if(iMaxValue	>	100000000)	width	=	9;
	else	if(iMaxValue	>	10000000)	width	=	8;
	else	if(iMaxValue	>	1000000)	width	=	7;
	else	if(iMaxValue	>	100000)		width	=	6;
	else	if(iMaxValue	>	10000)		width	=	5;
	else									width	=	4;

	width	=	(width+4)*6;

	l_aNumberBox	[g_iNumberIndex].Init(hwnd,s_hInst,IDC_KARMA_NUMBERBOX	+g_iNumberIndex	,_ms("%d",select),x,y-4,width,20,ES_NUMBER|WS_BORDER);

	g_iNumberIndex++;

	return	width+5;
}

int
cKARMA_WND::KarmaTextEditBox(HWND hwnd,int x,int y,char *lpStr)
{
	int	width	=	120;

	if (lpStr)	l_aEditBox[g_iEditBoxIndex].Init(hwnd,s_hInst,IDC_KARMA_EDITBOX	+g_iEditBoxIndex	,lpStr	,x,y-4,width,20,WS_BORDER);
	else		l_aEditBox[g_iEditBoxIndex].Init(hwnd,s_hInst,IDC_KARMA_EDITBOX	+g_iEditBoxIndex	,""		,x,y-4,width,20,WS_BORDER);

	g_iEditBoxIndex++;

	return	width+5;
}

void
cKARMA_WND::KarmaTextBox(HWND hwnd,int x,int y,char *lpStr)
{
	ShowWindow(GetDlgItem(hwnd,IDC_TEXT),SW_SHOW);

	HWND	hTextBox	=	GetDlgItem(hwnd,IDC_TEXT);

	if (lpStr)	SetWindowText(hTextBox,lpStr);
	else		SetWindowText(hTextBox,"");
}

///////////////////////////////////////////////////////////////////////////////
//	항목별로 텍스트를 얻어 온다.
///////////////////////////////////////////////////////////////////////////////
char*
cKARMA_WND::GetCustomComboText(int kind,int data)
{
	CheckJongsung(l_strComboStr[kind][data]);

	return	l_strComboStr[kind][data];
}

char*
cKARMA_WND::GetHeroText(int data)
{
	cJOB	*lpJob	=	cJOB::GetJob(data);

	if	(!lpJob)
		return	"It's Remove Job";

	CheckJongsung(lpJob->m_strName);

	return	lpJob->m_strName;
}

char*
cKARMA_WND::GetJobText(int data)
{
	static	char	*jobName	=	cJOB::GetName(data);

	if	(!jobName)
		return	"It's valid Job";

	return	jobName;
}

char*
cKARMA_WND::GetAllCharacterTargetPlayerText(int data)
{
	char	*lpstrName;

	if	(data	<	10000)
		lpstrName	=	l_strComboStr[eKARMA_KW_TARGET_PLAYER][data];
	else
	{
		data		-=	10000;
		lpstrName	=	g_lpActmap->m_setCharacter.m_aCharacter[data].m_strName;
	}

	CheckJongsung(lpstrName);

	return	lpstrName;
}

char*
cKARMA_WND::GetCharacterText(int data)
{
	char	*lpstrName;

	if (data == dCHARACTER_PLAYER) lpstrName = "All Players";
	else
		if (data == dCHARACTER_ALL) lpstrName = "All monsters";
		else
			if (data == dCHARACTER_TARGET_PLAYER) lpstrName = "Target Player";
			else
				if (data == dCHARACTER_TARGET_PARTY) lpstrName = dMSG_TARGET_PARTY;
				else
					if (data == dCHARACTER_SELF) lpstrName = "Self";
	else
		lpstrName	=	g_lpActmap->m_setCharacter.m_aCharacter[data].m_strName;

	CheckJongsung(lpstrName);

	return	lpstrName;
}

char*
cKARMA_WND::GetAreaText(int data)
{
	CheckJongsung(g_lpActmap->m_pArea->m_aArea[data].m_strName);

	return	g_lpActmap->m_pArea->m_aArea[data].m_strName;
}

char*
cKARMA_WND::GetOnlyAreaText(int data)
{
	CheckJongsung(g_lpActmap->m_pArea->m_aArea[data].m_strName);

	return	g_lpActmap->m_pArea->m_aArea[data].m_strName;
}

char*
cKARMA_WND::GetSpinText(int data)
{
	static	char	strNumber[12];

	strcpy(strNumber,_ms("%d",data));

	CheckJongsung(strNumber);

	return	strNumber;
}

char*
cKARMA_WND::GetNumberText(int data)
{
	static	char	strNumber[12];

	strcpy(strNumber,_ms("%d",data));
	CheckJongsung(strNumber);

	return	strNumber;
}

char*
cKARMA_WND::GetValueText(int data)
{
	if(data > dVALUE_COUNT)
	{
		data	-=	dVALUE_COUNT;

		CheckJongsung(g_lpActmap->m_Value.GetName(data));

		return	g_lpActmap->m_Value.GetName(data);
	}

	CheckJongsung(g_ProjectValue.GetName(data));

	return	g_ProjectValue.GetName(data);
}

char*
cKARMA_WND::GetMagicText(int data)
{
	return	NULL;
}

char*
cKARMA_WND::GetSetElementItemText(int data)
{
	int	iGroup	=	data/0x8000;
	int	iItem	=	data%0x8000;

	cItemSet	*lpGroup	=	g_itemSetManager.get(iGroup);

	if(!lpGroup)
		return	"null";

	cSetItem		*lpSetItem	=	lpGroup->getItem(iItem);

	if(!lpSetItem)
		return	"null";

	static	char	strText[64];

	sprintf(strText,"[%s] %s",lpGroup->m_strName,g_aBasicItem[lpSetItem->m_wBaseItem].m_strName);

	return	strText;		
}

char*
cKARMA_WND::GetItemPieceText(int data)
{
	cPieceItem	*lpPieceItem	=	g_pieceItemManager.get(data);

	if (!lpPieceItem)
		return	"null";

	return	lpPieceItem->m_strName;
}

char*
cKARMA_WND::GetMonsterText(int data)
{
	cJOB	*lpJob	=	g_denJob.getJob(data);

	if	(lpJob)
		return	lpJob->m_strName;

	return	"null";
}

char*
cKARMA_WND::GetEventTimeNameText(int data)
{
	return	g_strEventTimeName[data];
}

char*
cKARMA_WND::GetFieldValueText(int data)
{
	return	g_lpActmap->m_astrFieldValue[data];
}

char*
cKARMA_WND::GetItemText(int data)
{
	return	g_aBasicItem[data].m_strName;
}

char*
cKARMA_WND::GetPlayerJobText(int _iData)
{
	return	g_aDefaultJobName[_iData];
}

char*
cKARMA_WND::GetTitleText(int data)
{
	CTitle	*lpTitle	=	g_titleManager.getTitle(data);

	if (!lpTitle)
		return	"null";

	return	lpTitle->m_strName;
}

char*
cKARMA_WND::GetSpecificItemText(int data)
{
	CCustomItem	*lpItem	=	g_lpActmap->m_customItem.getItem(data);

	if (!lpItem)	"removed";

	return	lpItem->m_strName;
}

char*
cKARMA_WND::GetBookText(int data)
{
	cBookInfo	*lpBook	=	g_book.getBook(data);

	if	(!lpBook)
		return	"error!!";

	return	lpBook->m_strBookName;
}

char*
cKARMA_WND::GetDungeonValueText(int data)
{
	cSecretDungeonValue	*lpString	=	g_sdManager.getString(g_lpActmap->m_wLinkSecretDungeon);

	if (!lpString)
		return	"error!!";

	return	lpString->m_strDungeonValue[data];
}

char*
cKARMA_WND::GetMainQuestValueText(int data)
{
	return	g_strMainQuestValue[data];
}

char*
cKARMA_WND::GetMainQuestSwitchText(int data)
{
	return	g_strMainQuestSwitch[data];
}

char*
cKARMA_WND::GetGuildStatueSkillText(int _iData)
{
	CGuildSkill	*lpGS	=	&g_gsManager.m_aSkills[_iData];

	return	lpGS->m_strName;
}

char*
cKARMA_WND::GetRelatedPlaceOfGuildText(int _iData)
{
	for (int i=0;;i+=2)
	{
		if	(stricmp(g_strRelatedPlaceOfGuildList[i],"end")	==	0)
			break;

		int	iSerial	=	atoi(g_strRelatedPlaceOfGuildList[i+1]);

		if	(iSerial	==	_iData)
			return	g_strRelatedPlaceOfGuildList[i];
	}

	return	"null";
}


char*
cKARMA_WND::GetPatternText(int _iData)
{
	cCharacterPattern	*lpPattern	=	g_lpActmap->m_cpp.get(_iData);

	if	(!lpPattern)
		return	"지워진 패턴!!";

	return	lpPattern->m_strName;
}

char*
cKARMA_WND::GetChapterText(int data)
{
	cChapterInfo	*lpChapter	=	g_chapterManager.get(data);

	if	(!lpChapter)
		return	"valid";

	return	lpChapter->m_strName;
}

char*
cKARMA_WND::GetSkillText(int data)
{
	return	g_aSkill[data].m_strName;
}

char*
cKARMA_WND::GetBattleInclinationText(int data)
{
	return	g_strBattleInclination[data];
}

char*
cKARMA_WND::GetArmorText(int data)
{
	return	NULL;
}

char*
cKARMA_WND::GetMagicEffectText(int data)
{
	return	NULL;
}

char*
cKARMA_WND::GetMusicFileText(int data)
{
	return	NULL;
}

char*
cKARMA_WND::GetSoundFileText(int data)
{
	return	NULL;
}

char*
cKARMA_WND::GetDoorText(int data)
{
	if (!g_lpActmap->m_pArea->IsDoor(data))	return	dMSG_ITS_NOT_DOOR;

	return	g_lpActmap->m_pArea->GetName(data);
}

char*
cKARMA_WND::GetBoxText(int data)
{
	return	NULL;
}

char*
cKARMA_WND::GetQuestText(int data)
{
	return	g_quest.getName(data);
}

char*
cKARMA_WND::GetQuestArbeitText(int data)
{
	return	g_questArbeit.getName(data);
}

char*
cKARMA_WND::GetFieldText(int _iData)
{
	static	cSTRINGS	*lpFileList	=	NULL;

	KILL(lpFileList);
	
	cPROJECT::SetMapFolder();

	lpFileList	=	cFOLDER::GetFiles("*.rmd");//	화일들을 얻어 온다.

	SetLogFolder();

	if (lpFileList == NULL)
		return	"not map";

	if (lpFileList)
	{
		cSTRING	*lpFile	=	lpFileList->Pick();

		for (int i=0;i<lpFileList->Count;i++)
		{
			int	iFieldSerial	=	atoi(lpFile->String+1);

			if (iFieldSerial	==	_iData)
				return	lpFile->String;

			lpFile	=	lpFile->pNEXT;
		}
	}

	return	"not map";;
}	//	cKARMA_WND::GetFieldText(int _iData)

char*
cKARMA_WND::GetPowerText(int data)
{
	int	index	=	0;

	while(1)
	{
		if (g_aPowers[index].m_iType	==	0xffff)	return	NULL;
		if (g_aPowers[index].m_iType	==	data)
			return	g_aPowers[index].m_strName;

		index++;
	}

	return	NULL;
}

char*
cKARMA_WND::GetShopText(int data)
{
	CShop	*lpShop	=	g_lpActmap->m_shop.get(data);

	if (!lpShop)	return	"삭제된 가게 =o-";

	return	lpShop->m_strName;
}

char*
cKARMA_WND::GetSpeechText(int data)
{
	return	g_strCurrentSpeechList[data];
}

char*
cKARMA_WND::GetLimitedPowerText(int data)
{
	int	index	=	0;

	while(1)
	{
		if (g_aLimitedPowers[index].m_iType	==	0xffff)	return	NULL;
		if (g_aLimitedPowers[index].m_iType	==	data)
			return	g_aLimitedPowers[index].m_strName;

		index++;
	}

	return	NULL;
}

char*
cKARMA_WND::GetStatusText(int data)
{
	return	NULL;
}

char*
cKARMA_WND::GetAbilityText(int data)
{
	return	NULL;
}


//
//	카르마 텍스트를 그린다.
//
BOOL
cKARMA_WND::WriteKarmaText(HWND hwnd,char *lpText,int *lpValue,char *lpString, int _x, int _y, int _Enable)
{
	int			x	=	_x;		
	int			y	=	_y;

	char		*lpStream,*lpKeyWord;
	int			iCode;

//	ShowWindow(GetDlgItem(hwnd,IDC_ACTOR_POWER_ROUND)	,SW_HIDE);
//	ShowWindow(GetDlgItem(hwnd,IDC_ACTOR_POWER)			,SW_HIDE);
	ShowWindow(GetDlgItem(hwnd,IDC_TEXT)				,SW_HIDE);

	g_iStreamIndex			=	0;
	g_iComboIndex			=	0;
	g_iSpinIndex			=	0;
	g_iSpinBoxIndex			=	0;
	g_iButtonIndex			=	0;
	g_iNumberIndex			=	0;
	g_iEditBoxIndex			=	0;
	s_iDungeonvalueComboBox	=	-1;
	s_iQuestJobQuest		=	-1;
	s_iMainQuestValueComboBox=	-1;
	s_iMainQuestSwitchComboBox=	-1;

	int	iValueIndex	=	0xffff;

	l_Reader.UploadBuffer(lpText,g_aKarmaText);

	EnableWindow(GetDlgItem(hwnd,_Enable),TRUE);

	for (int i=0;i<10;i++)
	{
		l_aStream[i]	.Close();
		l_aButton[i]	.Close();
		s_aCombo[i]		.Close();
		l_aSpinBox[i]	.Close();
		l_aSpin[i]		.Close();
		l_aNumberBox[i]	.Close();
		l_aEditBox[i]	.Close();
	}

	while(1)
	{
		if (l_Reader.m_isDone	)
			break;		//	다 읽어 들였다.

		lpStream	=	l_Reader.GetStream('[');

		if (iValueIndex	!=	0xffff	)
			TransCode(lpStream);

		//	스태틱 문장 출력
		if (lpStream			)
			x	+=	l_aStream[g_iStreamIndex++].Init(hwnd,s_hInst,lpStream,x,y,0,12);
		if (l_Reader.m_isDone	)
			break;		//	다 읽어 들였다.

		//	콤보 박스나 머 그런거..
		lpKeyWord	=	l_Reader.GetStream(']');
		iCode		=	l_Reader.AnalCode(lpKeyWord);
		iValueIndex	=	l_Reader.GetNumber();

		switch(iCode)
		{
//	문장 선택형 콤보 박스들
			case	eKARMA_KW_EQUAL							:
			case	eKARMA_KW_ONLY_EQUAL					:
			case	eKARMA_KW_IN_OUT						:
			case	eKARMA_KW_CHECK_OPERATOR				:
			case	eKARMA_KW_QUEST_TARGET					:
			case	eKARMA_KW_QUEST_CHECK_SOLO				:
			case	eKARMA_KW_TIME_CHECK					:
			case	eKARMA_KW_PARTY_MEMBER_EQUAL			:
			case	eKARMA_KW_PLUS_MINUS					:
			case	eKARMA_KW_HERO_JOB						:
			case	eKARMA_KW_ALLIGNMENT					:
			case	eKARMA_KW_QUEST_OWN						:
			case	eKARMA_KW_QUEST_COMPLETE				:
			case	eKARMA_KW_QUEST_COMPLETE_CHECKER		:
			case	eKARMA_KW_CHECK_QUEST_COMPLETE_CHECKER	:
			case	eKARMA_KW_AREA_IN_OUT					:
			case	eKARMA_KW_ITEM_OWN						:
			case	eKARMA_KW_GUILD_MEMBER					:
			case	eKARMA_KW_ITEM_EQUIP					:
			case	eKARMA_KW_JOB_OWN						:
			case	eKARMA_KW_PARTY_GUILD_MEMBER_EXIST		:
			case	eKARMA_KW_ALL_PARTY_MEMBER_IN_SAME_GUILD:
			case	eKARMA_KW_SPECIFIC_ITEM_OWN_MEMBER_EXIST:
			case	eKARMA_KW_EXIST							:
			case	eKARMA_KW_OWN							:
			case	eKARMA_KW_ADD_REMOVE					:
			case	eKARMA_KW_ADD_SET_REMOVE				:
			case	eKARMA_KW_MOVE_PATROL			:
			case	eKARMA_KW_ARCA_DOOR_TRAP				:
			case	eKARMA_KW_ARCA_DOOR_TRAP_STATUS			:
			case	eKARMA_KW_TARGET_PLAYER					:
			case	eKARMA_KW_TARGET						:
			case	eKARMA_KW_NOTICE_RANGE					:
			case	eKARMA_KW_NOTICE_TYPE					:
			case	eKARMA_KW_PHYSICAL_MAGIC				:
			case	eKARMA_KW_QUEST_PROCESS_LEVEL			:
			case	eKARMA_KW_PARTY_MEMBER_LOCATION			:
			case	eKARMA_KW_INCLUDE_TRANSFORMATION_JOB	:
			case	eKARMA_KW_IDA_ANIDA						:
			case	eKARMA_KW_IS_PARTY_LEADER				:
			case	eKARMA_KW_MONSTER_TARGET_SELF			:
			case	eKARMA_KW_DUEL_INTERFACE				:
			case	eKARMA_KW_INVINCIBLE_ON_OFF				:
			case	eKARMA_KW_EVENT_BEGIN_END				:
			case	eKARMA_KW_QUEST_ABLE					:
			case	eKARMA_KW_SET_CANCEL					:
			case	eKARMA_KW_START_END_RESET				:
			case	eKARMA_KW_BATTLE_STATUS					:
			case	eKARMA_KW_DIRECT						:
			case	eKARMA_KW_COUNTDOWN_TYPE				:
			case	eKARMA_KW_SETTING_STATUS				:
			case	eKARMA_KW_COUNTDOWN_STATUS				:
			case	eKARMA_KW_QUEST_COUNTDOWN_CHECK			:
			case	eKARMA_KW_TARGET_LEADER					:
			case	eKARMA_KW_SKILL_EFFECT_TYPE				:
			case	eKARMA_KW_SET_RESET						:
			case	eKARMA_KW_ENABLE_DISABLE				:
			case	eKARMA_KW_ALL_PLAYER					:
			case	eKARMA_KW_CHECK_ON_OFF					:
			case	eKARMA_KW_SET_PLUS_MINUS				:
			case	eKARMA_KW_WRITE_ERASE					:
			case	eKARMA_KW_START_END_EXTENSION_REDUCTION	:
			case	eKARMA_KW_APPLY_PREMIUM_BONUS			:
			case	eKARMA_KW_ABLE_DISABLE					:
			case	eKARMA_KW_TEST_NORMAL					:
			case	eKARMA_KW_CURRENT_STATUS				:
			case	eKARMA_KW_SET_RESET_SCROLL				:
			case	eKARMA_KW_ENTRY_DEATH					:
			case	eKARMA_KW_BOSS_MONSTER					:
			case	eKARMA_KW_METHOD_OF_GIVE_GP				:
			case	eKARMA_KW_MOVE_METHOD					:
			case	eKARMA_KW_TALK_METHOD					:
			case	eKARMA_KW_DAMAGE_METHOD					:
			case	eKARMA_KW_COMPLETE_STOP					:
			case	eKARMA_KW_IGNORE_INCLUDE_EQUIPMENT		:
			case	eKARMA_KW_RANK_IN_GUILD					:
			case	eKARMA_KW_SEX							:
			case	eKARMA_KW_LOCAL							:
			case	eKARMA_KW_BY_LEVEL						:
			case	eKARMA_KW_GIVE_DROP						:
			case	eKARMA_KW_HEAVEN_REWARD		:
			case	eKARMA_KW_HELL_REWARD		:
			case	eKARMA_KW_RED_DEVIL_REWARD	:
			case	eKARMA_KW_PLUSNUMBER_MINUSNUMBER:
			case	eKARMA_KW_HEAVEN_BONUS	:
			case	eKARMA_KW_JANI_ANJINA	:
			case	eKARMA_KW_BONUS_PENELTY	:
			case	eKARMA_KW_EVENT	:
			case	eKARMA_KW_HANDA_ANHANDA	:
			case	eKARMA_KW_EVENT_MONSTER					:
			case	eKARMA_KW_TARGET_OWNER					:
			case	eKARMA_KW_GUILD_BATTLE_TYPE				:
			case	eKARMA_KW_GUILD_HALL_OWN_OTHER			:
			case	eKARMA_KW_ACTION						:
			case	eKARMA_KW_ITEM_BOOLEAN_OPTION_LIST		:
			case	eKARMA_KW_SEASON_VALIABLE				:
			case	eKARMA_KW_GRACE							:
			case	eKARMA_KW_EFFECT_FLICKERINGLY			:
			case	eKARMA_KW_WEEKDAY						:
			case	eKARMA_KW_IS_SECRECTD_GUILDD	:
			case	eKARMA_STATE					:
			case	eKARMA_FIX_FIELD_RESISTANCE		:
			case	eKARMA_SCRREN_EFFECT			:
			case	eKARMA_BLIND_EFFECT				:
			case	eKARMA_QUEST_KIND	:
			case	eKARMA_FIELD_WEATHER	:		// 기후변화 09.10.06
			case	eKARMA_INCLUDE_SOLO		:
			case	eKARMA_KW_TILE_SET		:		// 타일셋
			case	eKARMA_KW_DRAWING_METHOD		:		// 드로잉 반복
			case	eKARMA_KW_POPUP_INTERFACE		:		//	인터페이스
			case	eKARMA_KW_OPEN_AND_CLOSE		:		//	열기 닫기
			case	eKARMA_KW_ADD_GOLD_REASON		:
				x	+=	CustomComboBox(hwnd,x,y,iCode,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_BOOK			:
				x	+=	ComboBookList(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_DUNGEON_VALUE	:
				x	+=	ComboDungeonValue(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_MAIN_QUEST_VALUE				:
				x	+=	ComboMainQuestValue(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_MAIN_QUEST_SWITCH				:
				x	+=	ComboMainQuestSwitch(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_PATTERN		:
				x	+=	ComboPattern(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_GUILD_STATUE_SKILL	:
				x	+=	ComboGuildStatueSkill(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_RELATED_PLACE_OF_GUILD	:
				x	+=	ComboRelatedPlaceOfGuild(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_CHAPTER		:
				x	+=	ComboChapter(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_MONSTER		:
				x	+=	ComboMonster(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_FIELD_VALUE	:
				x	+=	ComboFieldValue(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_EVENT_TIME_NAME	:
				x	+=	ComboEventTimeName(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_WORLD_VALUE	:
				x	+=	ComboWorldValue(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_SKILL			:
				x	+=	ComboSkill(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_EQUIPMENT		:
				x	+=	ComboEquipment(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPECIFIC_ITEM	:
				x	+=	ComboSpecificItem(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_ITEM			:
				x	+=	ComboItem(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPECIAL_ITEM	:
				x	+=	ComboSpecialItem(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_QUEST_ITEM	:
				x	+=	ComboQuestItem(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_PLAYER_JOB	:
				x	+=	ComboPlayerJob(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_QUEST_JOB		:
				x	+=	ComboQuestJob(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_ARBEIT_JOB		:
				x	+=	ComboArbeitJob(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_SET_ELEMENT_ITEM				:
				x	+=	ComboSetElementItem(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_ITEM_PIECE					:
				x	+=	ComboItemPiece(hwnd,x,y,lpValue[iValueIndex]);
				break;


			case	eKARMA_KW_TITLE			:
				x	+=	ComboTitle(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPEECH		:
				x	+=	ComboSpeech(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_SHOP			:
				x	+=	ComboShop(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_QUEST			:
				x	+=	ComboQuest(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_QUEST_ARBEIT	:
				x	+=	ComboQuestArbeit(hwnd,x,y,lpValue[iValueIndex]);
				break;

				
			case	eKARMA_KW_AREA			:
				x	+=	ComboArea(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_TRAP_AREA			:
				x	+=	ComboTrapArea(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_ALL_AREA		:
				x	+=	ComboAllArea(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_FIELD			:
				x	+=	ComboScenario(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_BATTLE_INCLINATION	:
				x	+=	ComboBattleInclination(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_CHARACTER		:
				x	+=	ComboCharacter(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_ALL_CHARACTER					:
				x	+=	ComboAllCharacter(hwnd,x,y,lpValue[iValueIndex]);
				break;
			case	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER	:
				x	+=	ComboAllCharacterTargetPlayer(hwnd,x,y,lpValue[iValueIndex]);
				break;
			case	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER2	:
				x	+=	ComboAllCharacterTargetPlayer2(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_POWER			:		//	캐릭터의 능력치
				x	+=	ComboPower(hwnd,x,y,lpValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPIN			:
			{
				x	+=	KarmaTextSpin(hwnd,x,y,lpValue[iValueIndex]);
				break;
			}

			case	eKARMA_KW_NUMBER			:
			{
				x	+=	KarmaTextNumber(hwnd,x,y,lpValue[iValueIndex]);
				break;
			}

			case	eKARMA_KW_STRING			:
			{
				x	+=	KarmaTextEditBox(hwnd,x,y,lpString);
				break;
			}

			case	eKARMA_KW_SOUND_FILE					:
				x	+=	CustomSoundFile(hwnd,x,y,lpString);
				break;


			case	eKARMA_KW_TEXT_BOX				:
				KarmaTextBox(hwnd,x,y,lpString);
				break;

			case	eKARMA_KW_BORDER_COMMENT		:
				x	=	0xfffe;
				break;

			default						:
//				ERRMSG("error","what is it?? [%s]",lpKeyWord);
				break;
		}

		if (x	>=	0xfffe)
		{
			if (x	>=	0xffff)
			{
				for (int i=0;i<10;i++)
				{
					l_aStream[i]	.Close();
					l_aButton[i]	.Close();
					s_aCombo[i]		.Close();
					l_aSpinBox[i]	.Close();
					l_aSpin[i]		.Close();
				}

				l_aStream[0].Init(hwnd,s_hInst,dMSG_LACK_KARMA_DATA,18,y,0,12);
			}

			EnableWindow(GetDlgItem(hwnd,_Enable),TRUE);

			return	FALSE;
		}
	}

	return	TRUE;
}

//
//	카르마 텍스트를 해독한다.
//
BOOL
cKARMA_WND::ReadKarmaText(HWND hwnd,int iIndex,char *lpText,void *lpData)
{
	char		*lpStream,*lpKeyWord;
	int			iCode;

	CKarmaItem	*lpKarmaData	=	((CKarmaItem *)lpData);

	g_iStreamIndex	=	0;
	g_iComboIndex	=	0;
	g_iSpinIndex	=	0;
	g_iSpinBoxIndex	=	0;
	g_iButtonIndex	=	0;
	g_iNumberIndex	=	0;
	g_iEditBoxIndex	=	0;

	int	iValueIndex		=	0;

	l_Reader.UploadBuffer(lpText,g_aKarmaText);

	lpKarmaData->m_wItem	=	iIndex;

//	if (iIndex	==	dREACTION_SET_POWER || iIndex	==	dREACTION_ADD_POWER)	l_isEditPower	=	TRUE;
//	else																		l_isEditPower	=	FALSE;

	while(1)
	{
		if (l_Reader.m_isDone	)	break;		//	다 읽어 들였다.

		lpStream	=	l_Reader.GetStream('[');

		if (iValueIndex	>	0	)	TransCode(lpStream);
		if (l_Reader.m_isDone	)	break;		//	다 읽어 들였다.

		//	콤보 박스나 머 그런거..
		lpKeyWord	=	l_Reader.GetStream(']');
		iValueIndex	=	l_Reader.GetNumber();

		iCode		=	l_Reader.AnalCode(lpKeyWord);

		switch(iCode)
		{
			case	eKARMA_KW_EQUAL							:
			case	eKARMA_KW_ONLY_EQUAL					:
			case	eKARMA_KW_IN_OUT						:
			case	eKARMA_KW_CHECK_OPERATOR				:
			case	eKARMA_KW_TIME_CHECK					:
			case	eKARMA_KW_QUEST_TARGET					:
			case	eKARMA_KW_QUEST_CHECK_SOLO				:
			case	eKARMA_KW_PARTY_MEMBER_EQUAL			:
			case	eKARMA_KW_PLUS_MINUS					:
			case	eKARMA_KW_POWER							:
			case	eKARMA_KW_ITEM							:
			case	eKARMA_KW_SKILL							:
			case	eKARMA_KW_FIELD_VALUE	:
			case	eKARMA_KW_EVENT_TIME_NAME	:
			case	eKARMA_KW_WORLD_VALUE	:
			case	eKARMA_KW_MONSTER		:
			case	eKARMA_KW_EQUIPMENT						:
			case	eKARMA_KW_SPECIFIC_ITEM					:
			case	eKARMA_KW_SPECIAL_ITEM					:
			case	eKARMA_KW_QUEST_ITEM					:
			case	eKARMA_KW_PLAYER_JOB					:
			case	eKARMA_KW_QUEST_JOB						:
			case	eKARMA_KW_ARBEIT_JOB					:
			case	eKARMA_KW_SET_ELEMENT_ITEM				:
			case	eKARMA_KW_ITEM_PIECE					:
			case	eKARMA_KW_HERO_JOB						:
			case	eKARMA_KW_ALLIGNMENT					:
			case	eKARMA_KW_QUEST_OWN						:
			case	eKARMA_KW_QUEST_COMPLETE				:
			case	eKARMA_KW_QUEST_COMPLETE_CHECKER		:
			case	eKARMA_KW_CHECK_QUEST_COMPLETE_CHECKER	:
			case	eKARMA_KW_AREA_IN_OUT					:
			case	eKARMA_KW_ITEM_OWN						:
			case	eKARMA_KW_GUILD_MEMBER					:
			case	eKARMA_KW_ITEM_EQUIP					:
			case	eKARMA_KW_JOB_OWN						:
			case	eKARMA_KW_PARTY_GUILD_MEMBER_EXIST		:
			case	eKARMA_KW_ALL_PARTY_MEMBER_IN_SAME_GUILD:
			case	eKARMA_KW_SPECIFIC_ITEM_OWN_MEMBER_EXIST:
			case	eKARMA_KW_EXIST							:
			case	eKARMA_KW_OWN							:
			case	eKARMA_KW_ADD_REMOVE					:
			case	eKARMA_KW_ADD_SET_REMOVE				:
			case	eKARMA_KW_MOVE_PATROL			:
			case	eKARMA_KW_ARCA_DOOR_TRAP				:
			case	eKARMA_KW_ARCA_DOOR_TRAP_STATUS			:
			case	eKARMA_KW_TARGET_PLAYER					:
			case	eKARMA_KW_TARGET						:
			case	eKARMA_KW_NOTICE_RANGE					:
			case	eKARMA_KW_NOTICE_TYPE					:
			case	eKARMA_KW_SPEECH						:
			case	eKARMA_KW_SHOP							:
			case	eKARMA_KW_QUEST							:
			case	eKARMA_KW_QUEST_ARBEIT					:
			case	eKARMA_KW_AREA							:
			case	eKARMA_KW_TRAP_AREA						:
			case	eKARMA_KW_ALL_AREA						:
			case	eKARMA_KW_PHYSICAL_MAGIC				:
			case	eKARMA_KW_CHARACTER						:
			case	eKARMA_KW_ALL_CHARACTER					:
			case	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER	:
			case	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER2	:
			case	eKARMA_KW_QUEST_PROCESS_LEVEL			:
			case	eKARMA_KW_PARTY_MEMBER_LOCATION			:
			case	eKARMA_KW_INCLUDE_TRANSFORMATION_JOB	:
			case	eKARMA_KW_IDA_ANIDA						:
			case	eKARMA_KW_IS_PARTY_LEADER				:
			case	eKARMA_KW_BATTLE_INCLINATION			:
			case	eKARMA_KW_FIELD							:
			case	eKARMA_KW_TITLE							:
			case	eKARMA_KW_MONSTER_TARGET_SELF			:
			case	eKARMA_KW_DUEL_INTERFACE				:
			case	eKARMA_KW_INVINCIBLE_ON_OFF				:
			case	eKARMA_KW_EVENT_BEGIN_END				:
			case	eKARMA_KW_QUEST_ABLE					:
			case	eKARMA_KW_SET_CANCEL					:
			case	eKARMA_KW_START_END_RESET				:
			case	eKARMA_KW_BATTLE_STATUS					:
			case	eKARMA_KW_DIRECT						:
			case	eKARMA_KW_COUNTDOWN_TYPE				:
			case	eKARMA_KW_SETTING_STATUS				:
			case	eKARMA_KW_COUNTDOWN_STATUS				:
			case	eKARMA_KW_QUEST_COUNTDOWN_CHECK			:
			case	eKARMA_KW_TARGET_LEADER					:
			case	eKARMA_KW_SKILL_EFFECT_TYPE				:
			case	eKARMA_KW_SET_RESET						:
			case	eKARMA_KW_ENABLE_DISABLE				:
			case	eKARMA_KW_ALL_PLAYER					:
			case	eKARMA_KW_ACTION						:
			case	eKARMA_KW_CHECK_ON_OFF					:
			case	eKARMA_KW_SET_PLUS_MINUS				:
			case	eKARMA_KW_WRITE_ERASE					:
			case	eKARMA_KW_APPLY_PREMIUM_BONUS			:
			case	eKARMA_KW_ABLE_DISABLE					:
			case	eKARMA_KW_TEST_NORMAL					:
			case	eKARMA_KW_CURRENT_STATUS				:
			case	eKARMA_KW_SET_RESET_SCROLL				:
			case	eKARMA_KW_ENTRY_DEATH					:
			case	eKARMA_KW_BOSS_MONSTER					:
			case	eKARMA_KW_METHOD_OF_GIVE_GP				:
			case	eKARMA_KW_TARGET_OWNER					:
			case	eKARMA_KW_MOVE_METHOD					:
			case	eKARMA_KW_TALK_METHOD					:
			case	eKARMA_KW_DAMAGE_METHOD					:
			case	eKARMA_KW_COMPLETE_STOP					:
			case	eKARMA_KW_IGNORE_INCLUDE_EQUIPMENT		:
			case	eKARMA_KW_RANK_IN_GUILD					:
			case	eKARMA_KW_SEX							:
			case	eKARMA_KW_LOCAL							:
			case	eKARMA_KW_BY_LEVEL						:
			case	eKARMA_KW_GIVE_DROP						:
			case	eKARMA_KW_HEAVEN_REWARD		:
			case	eKARMA_KW_HELL_REWARD		:
			case	eKARMA_KW_RED_DEVIL_REWARD	:
			case	eKARMA_KW_PLUSNUMBER_MINUSNUMBER:
			case	eKARMA_KW_HEAVEN_BONUS	:
			case	eKARMA_KW_JANI_ANJINA	:
			case	eKARMA_KW_BONUS_PENELTY	:
			case	eKARMA_KW_EVENT	:
			case	eKARMA_KW_HANDA_ANHANDA	:
			case	eKARMA_KW_EVENT_MONSTER					:
			case	eKARMA_KW_GUILD_BATTLE_TYPE				:
			case	eKARMA_KW_GUILD_HALL_OWN_OTHER			:
			case	eKARMA_KW_START_END_EXTENSION_REDUCTION	:
			case	eKARMA_KW_BOOK							:
			case	eKARMA_KW_CHAPTER						:
			case	eKARMA_KW_PATTERN						:
			case	eKARMA_KW_GUILD_STATUE_SKILL			:
			case	eKARMA_KW_RELATED_PLACE_OF_GUILD	:
			case	eKARMA_KW_ITEM_BOOLEAN_OPTION_LIST	:
			case	eKARMA_KW_SEASON_VALIABLE			:
			case	eKARMA_KW_GRACE						:
			case	eKARMA_KW_EFFECT_FLICKERINGLY		:
			case	eKARMA_KW_WEEKDAY					:
			case	eKARMA_KW_IS_SECRECTD_GUILDD	:
			case	eKARMA_STATE					:
			case	eKARMA_FIX_FIELD_RESISTANCE		:
			case	eKARMA_SCRREN_EFFECT			:
			case	eKARMA_BLIND_EFFECT				:
			case	eKARMA_QUEST_KIND	:
			case	eKARMA_FIELD_WEATHER :		// 기후변화 09.10.06
			case	eKARMA_INCLUDE_SOLO	:
			case	eKARMA_KW_TILE_SET		:		// 타일셋
			case	eKARMA_KW_DRAWING_METHOD		:		// 드로잉 반복
			case	eKARMA_KW_POPUP_INTERFACE		:		//	인터페이스
			case	eKARMA_KW_OPEN_AND_CLOSE		:		//	열기 닫기
			case	eKARMA_KW_ADD_GOLD_REASON		:
				lpKarmaData->m_aValue[iValueIndex]	=	s_aCombo[g_iComboIndex].GetData();
				g_iComboIndex++;
				break;

			case	eKARMA_KW_DUNGEON_VALUE					:
			{
				lpKarmaData->m_aValue[iValueIndex]	=	s_aCombo[g_iComboIndex].GetData();
				g_iComboIndex++;

				if (lpKarmaData->m_aValue[iValueIndex]	==	-1)
					lpKarmaData->m_aValue[iValueIndex]	=	s_iSelectDungeonValue;
				break;
			}

			case	eKARMA_KW_MAIN_QUEST_VALUE				:
			{
				lpKarmaData->m_aValue[iValueIndex]	=	s_aCombo[g_iComboIndex].GetData();
				g_iComboIndex++;

				if	(lpKarmaData->m_aValue[iValueIndex]	==	-1)
					lpKarmaData->m_aValue[iValueIndex]	=	s_iSelectMainQuestValue;
				break;
			}

			case	eKARMA_KW_MAIN_QUEST_SWITCH				:
			{
				lpKarmaData->m_aValue[iValueIndex]	=	s_aCombo[g_iComboIndex].GetData();
				g_iComboIndex++;

				if	(lpKarmaData->m_aValue[iValueIndex]	==	-1)
					lpKarmaData->m_aValue[iValueIndex]	=	s_iSelectMainQuestSwitch;
				break;
			}

			case	eKARMA_KW_SPIN		:
			{
				int	value		=	l_aSpinBox[g_iSpinIndex].GetInt();

				int	iMinValue	=	l_Reader.GetNumber();
				int	iMaxValue	=	l_Reader.GetNumber();

				value			=	max(min(value,iMaxValue),iMinValue);

				lpKarmaData->m_aValue[iValueIndex]	=	value;
				g_iSpinIndex++;

				break;
			}

			case	eKARMA_KW_NUMBER		:
			{
				int	value		=	l_aNumberBox[g_iNumberIndex].GetInt();

				int	iMinValue	=	l_Reader.GetNumber();
				int	iMaxValue	=	l_Reader.GetNumber();

				value			=	max(min(value,iMaxValue),iMinValue);

				lpKarmaData->m_aValue[iValueIndex]	=	value;

				g_iNumberIndex++;

				break;
			}

			case	eKARMA_KW_STRING		:
			{
				char	*lpStr	=	l_aEditBox[g_iEditBoxIndex].GetStr();

				lpKarmaData->SetString(lpStr);

				g_iEditBoxIndex++;
				break;
			}

			case	eKARMA_KW_SOUND_FILE					:
			{
				char	*lpstrSelect	=	s_aCombo[g_iComboIndex].GetString();

				lpKarmaData->SetString(lpstrSelect);

				g_iComboIndex++;
				break;
			}

			case	eKARMA_KW_TEXT_BOX				:
			{
				char	*pStr;
				int		iSize	=	1024*10,rSize;

				pStr	=	new	char [iSize];

				rSize	=	GetDlgItemText(hwnd,IDC_TEXT,pStr,iSize);
				lpKarmaData->SetString(pStr);

				pKILL(pStr);
 				break;
			}

			default					:
				ERRMSG("error","what is it?? [%s]",lpKeyWord);
				break;
		}

	}

	return	TRUE;
}

//
//	카르마 텍스트를 구한다.
char*
cKARMA_WND::KarmaText(void *lpData,BOOL isTrigger)
{
	static	cSTRING	Text;

	CKarmaItem	*lpKarmaData	=	(CKarmaItem *)lpData;

	Text.Reset();

	if	(lpKarmaData->m_bf1IsAbsolute)
		Text.Add(_ms("%c[절대]%c",3,2));

	char			*lpStream,*lpKeyWord;
	int				iCode;

	g_iStreamIndex	=	0;
	g_iComboIndex	=	0;
	g_iSpinIndex	=	0;
	g_iSpinBoxIndex	=	0;
	g_iButtonIndex	=	0;
	g_iNumberIndex	=	0;

	int	iValueIndex	=	0;

	char	*lpText;

	if	(isTrigger)
		lpText	=	GetTriggerText(lpKarmaData->m_wItem);
	else
		lpText	=	GetReactionText(lpKarmaData->m_wItem);

	l_Reader.UploadBuffer(lpText,g_aKarmaText);

	while(1)
	{
		if (l_Reader.m_isDone	)	break;		//	다 읽어 들였다.

		lpStream	=	l_Reader.GetStream('[');

		if (iValueIndex	>0	)	TransCode(lpStream);
		Text.Add(lpStream);

		if (l_Reader.m_isDone	)	break;		//	다 읽어 들였다.

		//	콤보 박스나 머 그런거..
		lpKeyWord	=	l_Reader.GetStream(']');
		iValueIndex	=	l_Reader.GetNumber();

		iCode		=	l_Reader.AnalCode(lpKeyWord);

		Text.Add(_ms("%c",1));

		char	*str	=	NULL;

		switch(iCode)
		{
			case	eKARMA_KW_EQUAL							:
			case	eKARMA_KW_ONLY_EQUAL					:
			case	eKARMA_KW_IN_OUT						:
			case	eKARMA_KW_CHECK_OPERATOR				:
			case	eKARMA_KW_TIME_CHECK					:
			case	eKARMA_KW_QUEST_TARGET					:
			case	eKARMA_KW_QUEST_CHECK_SOLO				:
			case	eKARMA_KW_PARTY_MEMBER_EQUAL			:
			case	eKARMA_KW_PLUS_MINUS					:
			case	eKARMA_KW_HERO_JOB						:
			case	eKARMA_KW_ALLIGNMENT					:
			case	eKARMA_KW_QUEST_OWN						:
			case	eKARMA_KW_QUEST_COMPLETE				:
			case	eKARMA_KW_QUEST_COMPLETE_CHECKER		:
			case	eKARMA_KW_CHECK_QUEST_COMPLETE_CHECKER	:
			case	eKARMA_KW_AREA_IN_OUT					:
			case	eKARMA_KW_ITEM_OWN						:
			case	eKARMA_KW_GUILD_MEMBER					:
			case	eKARMA_KW_ITEM_EQUIP					:
			case	eKARMA_KW_JOB_OWN						:
			case	eKARMA_KW_PARTY_GUILD_MEMBER_EXIST		:
			case	eKARMA_KW_ALL_PARTY_MEMBER_IN_SAME_GUILD:
			case	eKARMA_KW_SPECIFIC_ITEM_OWN_MEMBER_EXIST:
			case	eKARMA_KW_EXIST							:
			case	eKARMA_KW_OWN							:
			case	eKARMA_KW_ADD_REMOVE					:
			case	eKARMA_KW_ADD_SET_REMOVE				:
			case	eKARMA_KW_MOVE_PATROL					:
			case	eKARMA_KW_ARCA_DOOR_TRAP				:
			case	eKARMA_KW_ARCA_DOOR_TRAP_STATUS			:
			case	eKARMA_KW_TARGET_PLAYER					:
			case	eKARMA_KW_TARGET						:
			case	eKARMA_KW_NOTICE_RANGE					:
			case	eKARMA_KW_NOTICE_TYPE					:
			case	eKARMA_KW_PHYSICAL_MAGIC				:
			case	eKARMA_KW_QUEST_PROCESS_LEVEL			:
			case	eKARMA_KW_PARTY_MEMBER_LOCATION			:
			case	eKARMA_KW_INCLUDE_TRANSFORMATION_JOB	:
			case	eKARMA_KW_IDA_ANIDA						:
			case	eKARMA_KW_IS_PARTY_LEADER				:
			case	eKARMA_KW_MONSTER_TARGET_SELF			:
			case	eKARMA_KW_DUEL_INTERFACE				:
			case	eKARMA_KW_INVINCIBLE_ON_OFF				:
			case	eKARMA_KW_EVENT_BEGIN_END				:
			case	eKARMA_KW_QUEST_ABLE					:
			case	eKARMA_KW_SET_CANCEL					:
			case	eKARMA_KW_START_END_RESET				:
			case	eKARMA_KW_BATTLE_STATUS					:
			case	eKARMA_KW_DIRECT						:
			case	eKARMA_KW_COUNTDOWN_TYPE				:
			case	eKARMA_KW_SETTING_STATUS				:
			case	eKARMA_KW_COUNTDOWN_STATUS				:
			case	eKARMA_KW_QUEST_COUNTDOWN_CHECK			:
			case	eKARMA_KW_TARGET_LEADER					:
			case	eKARMA_KW_SKILL_EFFECT_TYPE				:
			case	eKARMA_KW_SET_RESET						:
			case	eKARMA_KW_ENABLE_DISABLE				:
			case	eKARMA_KW_ALL_PLAYER					:
			case	eKARMA_KW_ACTION						:
			case	eKARMA_KW_CHECK_ON_OFF					:
			case	eKARMA_KW_SET_PLUS_MINUS				:
			case	eKARMA_KW_WRITE_ERASE					:
			case	eKARMA_KW_APPLY_PREMIUM_BONUS			:
			case	eKARMA_KW_ABLE_DISABLE					:
			case	eKARMA_KW_TEST_NORMAL					:
			case	eKARMA_KW_CURRENT_STATUS				:
			case	eKARMA_KW_SET_RESET_SCROLL				:
			case	eKARMA_KW_ENTRY_DEATH					:
			case	eKARMA_KW_BOSS_MONSTER					:
			case	eKARMA_KW_METHOD_OF_GIVE_GP				:
			case	eKARMA_KW_TARGET_OWNER					:
			case	eKARMA_KW_MOVE_METHOD					:
			case	eKARMA_KW_TALK_METHOD					:
			case	eKARMA_KW_DAMAGE_METHOD					:
			case	eKARMA_KW_COMPLETE_STOP					:
			case	eKARMA_KW_IGNORE_INCLUDE_EQUIPMENT		:
			case	eKARMA_KW_RANK_IN_GUILD					:
			case	eKARMA_KW_SEX							:
			case	eKARMA_KW_LOCAL							:
			case	eKARMA_KW_BY_LEVEL						:
			case	eKARMA_KW_GIVE_DROP						:
			case	eKARMA_KW_HEAVEN_REWARD		:
			case	eKARMA_KW_HELL_REWARD		:
			case	eKARMA_KW_RED_DEVIL_REWARD	:
			case	eKARMA_KW_PLUSNUMBER_MINUSNUMBER:
			case	eKARMA_KW_HEAVEN_BONUS	:
			case	eKARMA_KW_JANI_ANJINA	:
			case	eKARMA_KW_BONUS_PENELTY	:
			case	eKARMA_KW_EVENT	:
			case	eKARMA_KW_HANDA_ANHANDA	:
			case	eKARMA_KW_EVENT_MONSTER					:
			case	eKARMA_KW_GUILD_BATTLE_TYPE				:
			case	eKARMA_KW_GUILD_HALL_OWN_OTHER			:
			case	eKARMA_KW_START_END_EXTENSION_REDUCTION	:
			case	eKARMA_KW_ITEM_BOOLEAN_OPTION_LIST		:
			case	eKARMA_KW_SEASON_VALIABLE		:
			case	eKARMA_KW_GRACE					:
			case	eKARMA_KW_EFFECT_FLICKERINGLY	:
			case	eKARMA_KW_WEEKDAY				:
			case	eKARMA_KW_IS_SECRECTD_GUILDD		:
			case	eKARMA_STATE					:
			case	eKARMA_FIX_FIELD_RESISTANCE		:
			case	eKARMA_SCRREN_EFFECT			:
			case	eKARMA_BLIND_EFFECT				:
			case	eKARMA_QUEST_KIND	:
			case	eKARMA_FIELD_WEATHER :		// 기후변화 09.10.06				
			case	eKARMA_INCLUDE_SOLO	:
			case	eKARMA_KW_TILE_SET		:		// 타일셋
			case	eKARMA_KW_DRAWING_METHOD		:		// 드로잉 반복
			case	eKARMA_KW_POPUP_INTERFACE		:		//	인터페이스
			case	eKARMA_KW_OPEN_AND_CLOSE		:		//	열기 닫기
			case	eKARMA_KW_ADD_GOLD_REASON		:
				str	=	GetCustomComboText(iCode,lpKarmaData->m_aValue[iValueIndex]);
				break;
				
			case	eKARMA_KW_ITEM			:
			case	eKARMA_KW_EQUIPMENT		:
			case	eKARMA_KW_SPECIAL_ITEM	:
			case	eKARMA_KW_QUEST_ITEM	:
				str	=	GetItemText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_PLAYER_JOB	:
			case	eKARMA_KW_QUEST_JOB		:
			case	eKARMA_KW_ARBEIT_JOB	:
				str	=	GetPlayerJobText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SET_ELEMENT_ITEM	:
				str	=	GetSetElementItemText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_ITEM_PIECE		:
				str	=	GetItemPieceText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_TITLE			:
				str	=	GetTitleText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPECIFIC_ITEM	:
				str	=	GetSpecificItemText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_BOOK							:
				str	=	GetBookText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_DUNGEON_VALUE					:
				str	=	GetDungeonValueText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_MAIN_QUEST_VALUE				:
				str	=	GetMainQuestValueText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_MAIN_QUEST_SWITCH				:
				str	=	GetMainQuestSwitchText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_PATTERN		:
				str	=	GetPatternText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_GUILD_STATUE_SKILL			:
				str	=	GetGuildStatueSkillText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_RELATED_PLACE_OF_GUILD	:
				str	=	GetRelatedPlaceOfGuildText(lpKarmaData->m_aValue[iValueIndex]);
				break;


			case	eKARMA_KW_CHAPTER				:
				str	=	GetChapterText(lpKarmaData->m_aValue[iValueIndex]);
				break;
			case	eKARMA_KW_SKILL			:
				str	=	GetSkillText(lpKarmaData->m_aValue[iValueIndex]);
				break;

				//JBC	월드 수치 추가.	09-01-13
			case	eKARMA_KW_WORLD_VALUE	:
				str	=	GetNumberText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_FIELD_VALUE	:
				str	=	GetFieldValueText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_EVENT_TIME_NAME	:
				str	=	GetEventTimeNameText(lpKarmaData->m_aValue[iValueIndex]);
				break;


			case	eKARMA_KW_MONSTER	:
				str	=	GetMonsterText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_BATTLE_INCLINATION	:
				str	=	GetBattleInclinationText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SHOP			:
				str	=	GetShopText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_QUEST			:
				str	=	GetQuestText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_QUEST_ARBEIT					:
				str	=	GetQuestArbeitText(lpKarmaData->m_aValue[iValueIndex]);
				break;


			case	eKARMA_KW_CHARACTER						:
			case	eKARMA_KW_ALL_CHARACTER					:
			case	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER	:
				str	=	GetCharacterText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER2	:
				str	=	GetAllCharacterTargetPlayerText(lpKarmaData->m_aValue[iValueIndex]);
				break;


			case	eKARMA_KW_AREA			:
			case	eKARMA_KW_ALL_AREA		:
			case	eKARMA_KW_TRAP_AREA		:
				str	=	GetAreaText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPEECH		:
				str	=	GetSpeechText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_POWER			:
				str	=	GetPowerText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_FIELD			:
				str	=	GetFieldText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPIN		:
				l_Reader.FindCharOver(']');
				str	=	GetSpinText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_NUMBER		:
				l_Reader.FindCharOver(']');
				str	=	GetNumberText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_STRING		:
				str	=	lpKarmaData->m_strString;
				break;

			case	eKARMA_KW_SOUND_FILE					:
				str	=	lpKarmaData->m_strString;
				break;

			case	eKARMA_KW_TEXT_BOX		:
			{
				static	char	strString[80];

				ustrncpy(strString,lpKarmaData->m_strString,76);
				strcpy(strString,_ms("\n\n%s",strString));

				str	=	strString;
 				break;
			}
			default					:
				ERRMSG("error","what is it?? [%s]",lpKeyWord);
				break;
		}

		if (str)
		{
			CheckJongsung(str);
			Text.Add(str);
		}

		Text.Add(_ms("%c",2));
	}

	return	Text.String;
}	//	cKARMA_WND::KarmaText(CKarmaItem	*lpKarmaData)

char*
cKARMA_WND::KarmaTextOut(void *lpData,BOOL* _bSpeech, BOOL isTrigger)
{
	static cSTRING	Text;
	
	CKarmaItem	*lpKarmaData	=	(CKarmaItem *)lpData;

	Text.Reset();

	if	(lpKarmaData->m_bf1IsAbsolute)
		Text.Add(_ms("%c[절대]%c",3,2));

	char			*lpStream,*lpKeyWord;
	int				iCode;

	g_iStreamIndex	=	0;
	g_iComboIndex	=	0;
	g_iSpinIndex	=	0;
	g_iSpinBoxIndex	=	0;
	g_iButtonIndex	=	0;
	g_iNumberIndex	=	0;

	int	iValueIndex	=	0;

	char	*lpText;

	if	(isTrigger)
		lpText	=	GetTriggerText(lpKarmaData->m_wItem);
	else
		lpText	=	GetReactionText(lpKarmaData->m_wItem);

	l_Reader.UploadBuffer(lpText,g_aKarmaText);

	while(1)
	{
		if (l_Reader.m_isDone	)	break;		//	다 읽어 들였다.

		lpStream	=	l_Reader.GetStream('[');

		if (iValueIndex	>0	)	TransCode(lpStream);
		Text.Add(lpStream);

		if (l_Reader.m_isDone	)	break;		//	다 읽어 들였다.

		//	콤보 박스나 머 그런거..
		lpKeyWord	=	l_Reader.GetStream(']');
		iValueIndex	=	l_Reader.GetNumber();

		iCode		=	l_Reader.AnalCode(lpKeyWord);

		char	*str	=	NULL;

		switch(iCode)
		{
			case	eKARMA_KW_EQUAL							:
			case	eKARMA_KW_ONLY_EQUAL					:
			case	eKARMA_KW_IN_OUT						:
			case	eKARMA_KW_CHECK_OPERATOR				:
			case	eKARMA_KW_TIME_CHECK					:
			case	eKARMA_KW_QUEST_TARGET					:
			case	eKARMA_KW_QUEST_CHECK_SOLO				:
			case	eKARMA_KW_PARTY_MEMBER_EQUAL			:
			case	eKARMA_KW_PLUS_MINUS					:
			case	eKARMA_KW_HERO_JOB						:
			case	eKARMA_KW_ALLIGNMENT					:
			case	eKARMA_KW_QUEST_OWN						:
			case	eKARMA_KW_QUEST_COMPLETE				:
			case	eKARMA_KW_QUEST_COMPLETE_CHECKER		:
			case	eKARMA_KW_CHECK_QUEST_COMPLETE_CHECKER	:
			case	eKARMA_KW_AREA_IN_OUT					:
			case	eKARMA_KW_ITEM_OWN						:
			case	eKARMA_KW_GUILD_MEMBER					:
			case	eKARMA_KW_ITEM_EQUIP					:
			case	eKARMA_KW_JOB_OWN						:
			case	eKARMA_KW_PARTY_GUILD_MEMBER_EXIST		:
			case	eKARMA_KW_ALL_PARTY_MEMBER_IN_SAME_GUILD:
			case	eKARMA_KW_SPECIFIC_ITEM_OWN_MEMBER_EXIST:
			case	eKARMA_KW_EXIST							:
			case	eKARMA_KW_OWN							:
			case	eKARMA_KW_ADD_REMOVE					:
			case	eKARMA_KW_ADD_SET_REMOVE				:
			case	eKARMA_KW_MOVE_PATROL					:
			case	eKARMA_KW_ARCA_DOOR_TRAP				:
			case	eKARMA_KW_ARCA_DOOR_TRAP_STATUS			:
			case	eKARMA_KW_TARGET_PLAYER					:
			case	eKARMA_KW_TARGET						:
			case	eKARMA_KW_NOTICE_RANGE					:
			case	eKARMA_KW_NOTICE_TYPE					:
			case	eKARMA_KW_PHYSICAL_MAGIC				:
			case	eKARMA_KW_QUEST_PROCESS_LEVEL			:
			case	eKARMA_KW_PARTY_MEMBER_LOCATION			:
			case	eKARMA_KW_INCLUDE_TRANSFORMATION_JOB	:
			case	eKARMA_KW_IDA_ANIDA						:
			case	eKARMA_KW_IS_PARTY_LEADER				:
			case	eKARMA_KW_MONSTER_TARGET_SELF			:
			case	eKARMA_KW_DUEL_INTERFACE				:
			case	eKARMA_KW_INVINCIBLE_ON_OFF				:
			case	eKARMA_KW_EVENT_BEGIN_END				:
			case	eKARMA_KW_QUEST_ABLE					:
			case	eKARMA_KW_SET_CANCEL					:
			case	eKARMA_KW_START_END_RESET				:
			case	eKARMA_KW_BATTLE_STATUS					:
			case	eKARMA_KW_DIRECT						:
			case	eKARMA_KW_COUNTDOWN_TYPE				:
			case	eKARMA_KW_SETTING_STATUS				:
			case	eKARMA_KW_COUNTDOWN_STATUS				:
			case	eKARMA_KW_QUEST_COUNTDOWN_CHECK			:
			case	eKARMA_KW_TARGET_LEADER					:
			case	eKARMA_KW_SKILL_EFFECT_TYPE				:
			case	eKARMA_KW_SET_RESET						:
			case	eKARMA_KW_ENABLE_DISABLE				:
			case	eKARMA_KW_ALL_PLAYER					:
			case	eKARMA_KW_ACTION						:
			case	eKARMA_KW_CHECK_ON_OFF					:
			case	eKARMA_KW_SET_PLUS_MINUS				:
			case	eKARMA_KW_WRITE_ERASE					:
			case	eKARMA_KW_APPLY_PREMIUM_BONUS			:
			case	eKARMA_KW_ABLE_DISABLE					:
			case	eKARMA_KW_TEST_NORMAL					:
			case	eKARMA_KW_CURRENT_STATUS				:
			case	eKARMA_KW_SET_RESET_SCROLL				:
			case	eKARMA_KW_ENTRY_DEATH					:
			case	eKARMA_KW_BOSS_MONSTER					:
			case	eKARMA_KW_METHOD_OF_GIVE_GP				:
			case	eKARMA_KW_TARGET_OWNER					:
			case	eKARMA_KW_MOVE_METHOD					:
			case	eKARMA_KW_TALK_METHOD					:
			case	eKARMA_KW_DAMAGE_METHOD					:
			case	eKARMA_KW_COMPLETE_STOP					:
			case	eKARMA_KW_IGNORE_INCLUDE_EQUIPMENT		:
			case	eKARMA_KW_RANK_IN_GUILD					:
			case	eKARMA_KW_SEX							:
			case	eKARMA_KW_LOCAL							:
			case	eKARMA_KW_BY_LEVEL						:
			case	eKARMA_KW_GIVE_DROP						:
			case	eKARMA_KW_HEAVEN_REWARD		:
			case	eKARMA_KW_HELL_REWARD		:
			case	eKARMA_KW_RED_DEVIL_REWARD	:
			case	eKARMA_KW_PLUSNUMBER_MINUSNUMBER:
			case	eKARMA_KW_HEAVEN_BONUS	:
			case	eKARMA_KW_JANI_ANJINA	:
			case	eKARMA_KW_BONUS_PENELTY	:
			case	eKARMA_KW_EVENT	:
			case	eKARMA_KW_HANDA_ANHANDA	:
			case	eKARMA_KW_EVENT_MONSTER					:
			case	eKARMA_KW_GUILD_BATTLE_TYPE				:
			case	eKARMA_KW_GUILD_HALL_OWN_OTHER			:
			case	eKARMA_KW_START_END_EXTENSION_REDUCTION	:
			case	eKARMA_KW_ITEM_BOOLEAN_OPTION_LIST		:
			case	eKARMA_KW_SEASON_VALIABLE		:
			case	eKARMA_KW_GRACE					:
			case	eKARMA_KW_EFFECT_FLICKERINGLY	:
			case	eKARMA_KW_WEEKDAY				:
			case	eKARMA_KW_IS_SECRECTD_GUILDD		:
			case	eKARMA_STATE					:
			case	eKARMA_FIX_FIELD_RESISTANCE		:
			case	eKARMA_SCRREN_EFFECT			:
			case	eKARMA_BLIND_EFFECT				:
			case	eKARMA_QUEST_KIND	:
			case	eKARMA_FIELD_WEATHER :		// 기후변화 09.10.06		
			case	eKARMA_INCLUDE_SOLO	:
			case	eKARMA_KW_TILE_SET		:		// 타일셋
			case	eKARMA_KW_DRAWING_METHOD		:		// 드로잉 반복
			case	eKARMA_KW_POPUP_INTERFACE		:		//	인터페이스
			case	eKARMA_KW_OPEN_AND_CLOSE		:		//	열기 닫기
			case	eKARMA_KW_ADD_GOLD_REASON		:
				str	=	GetCustomComboText(iCode,lpKarmaData->m_aValue[iValueIndex]);
				break;
				
			case	eKARMA_KW_ITEM			:
			case	eKARMA_KW_EQUIPMENT		:
			case	eKARMA_KW_SPECIAL_ITEM	:
			case	eKARMA_KW_QUEST_ITEM	:
				str	=	GetItemText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_PLAYER_JOB	:
			case	eKARMA_KW_QUEST_JOB		:
			case	eKARMA_KW_ARBEIT_JOB	:
				str	=	GetPlayerJobText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SET_ELEMENT_ITEM	:
				str	=	GetSetElementItemText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_ITEM_PIECE		:
				str	=	GetItemPieceText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_TITLE			:
				str	=	GetTitleText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPECIFIC_ITEM	:
				str	=	GetSpecificItemText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_BOOK							:
				str	=	GetBookText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_DUNGEON_VALUE					:
				str	=	GetDungeonValueText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_MAIN_QUEST_VALUE				:
				str	=	GetMainQuestValueText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_MAIN_QUEST_SWITCH				:
				str	=	GetMainQuestSwitchText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_PATTERN		:
				str	=	GetPatternText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_GUILD_STATUE_SKILL			:
				str	=	GetGuildStatueSkillText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_RELATED_PLACE_OF_GUILD	:
				str	=	GetRelatedPlaceOfGuildText(lpKarmaData->m_aValue[iValueIndex]);
				break;


			case	eKARMA_KW_CHAPTER				:
				str	=	GetChapterText(lpKarmaData->m_aValue[iValueIndex]);
				break;
			case	eKARMA_KW_SKILL			:
				str	=	GetSkillText(lpKarmaData->m_aValue[iValueIndex]);
				break;

				//JBC	월드 수치 추가.	09-01-13
			case	eKARMA_KW_WORLD_VALUE	:
				str	=	GetNumberText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_FIELD_VALUE	:
				str	=	GetFieldValueText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_EVENT_TIME_NAME	:
				str	=	GetEventTimeNameText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_MONSTER	:
				str	=	GetMonsterText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_BATTLE_INCLINATION	:
				str	=	GetBattleInclinationText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SHOP			:
				str	=	GetShopText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_QUEST			:
				str	=	GetQuestText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_QUEST_ARBEIT					:
				str	=	GetQuestArbeitText(lpKarmaData->m_aValue[iValueIndex]);
				break;


			case	eKARMA_KW_CHARACTER						:
			case	eKARMA_KW_ALL_CHARACTER					:
			case	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER	:
				str	=	GetCharacterText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER2	:
				str	=	GetAllCharacterTargetPlayerText(lpKarmaData->m_aValue[iValueIndex]);
				break;


			case	eKARMA_KW_AREA			:
			case	eKARMA_KW_ALL_AREA		:
			case	eKARMA_KW_TRAP_AREA		:
				str	=	GetAreaText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPEECH		:
				str	=	GetSpeechText(lpKarmaData->m_aValue[iValueIndex]);
				*_bSpeech = TRUE;
				break;

			case	eKARMA_KW_POWER			:
				str	=	GetPowerText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_FIELD			:
				str	=	GetFieldText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPIN		:
				l_Reader.FindCharOver(']');
				str	=	GetSpinText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_NUMBER		:
				l_Reader.FindCharOver(']');
				str	=	GetNumberText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_STRING		:
				str	=	lpKarmaData->m_strString;
				break;

			case	eKARMA_KW_SOUND_FILE					:
				str	=	lpKarmaData->m_strString;
				break;

			case	eKARMA_KW_TEXT_BOX		:
			{
				static	char	strString[80];

				ustrncpy(strString,lpKarmaData->m_strString,76);
				strcpy(strString,_ms("\n\n%s",strString));

				str	=	strString;
 				break;
			}
			default					:
				ERRMSG("error","what is it?? [%s]",lpKeyWord);
				break;
		}

		if (str)
		{
			CheckJongsung(str);
			Text.Add(str);
		}


	}

	return	Text.String;
}	//	cKARMA_WND::KarmaText(CKarmaItem	*lpKarmaDat
char*
cKARMA_WND::KarmaTextOut(void *lpData,BOOL isTrigger)
{
	static cSTRING	Text;
	
	CKarmaItem	*lpKarmaData	=	(CKarmaItem *)lpData;

	Text.Reset();

	if	(lpKarmaData->m_bf1IsAbsolute)
		Text.Add(_ms("%c[절대]%c",3,2));

	char			*lpStream,*lpKeyWord;
	int				iCode;

	g_iStreamIndex	=	0;
	g_iComboIndex	=	0;
	g_iSpinIndex	=	0;
	g_iSpinBoxIndex	=	0;
	g_iButtonIndex	=	0;
	g_iNumberIndex	=	0;

	int	iValueIndex	=	0;

	char	*lpText;

	if	(isTrigger)
		lpText	=	GetTriggerText(lpKarmaData->m_wItem);
	else
		lpText	=	GetReactionText(lpKarmaData->m_wItem);

	l_Reader.UploadBuffer(lpText,g_aKarmaText);

	while(1)
	{
		if (l_Reader.m_isDone	)	break;		//	다 읽어 들였다.

		lpStream	=	l_Reader.GetStream('[');

		if (iValueIndex	>0	)	TransCode(lpStream);
		Text.Add(lpStream);

		if (l_Reader.m_isDone	)	break;		//	다 읽어 들였다.

		//	콤보 박스나 머 그런거..
		lpKeyWord	=	l_Reader.GetStream(']');
		iValueIndex	=	l_Reader.GetNumber();

		iCode		=	l_Reader.AnalCode(lpKeyWord);

		char	*str	=	NULL;

		switch(iCode)
		{
			case	eKARMA_KW_EQUAL							:
			case	eKARMA_KW_ONLY_EQUAL					:
			case	eKARMA_KW_IN_OUT						:
			case	eKARMA_KW_CHECK_OPERATOR				:
			case	eKARMA_KW_TIME_CHECK					:
			case	eKARMA_KW_QUEST_TARGET					:
			case	eKARMA_KW_QUEST_CHECK_SOLO				:
			case	eKARMA_KW_PARTY_MEMBER_EQUAL			:
			case	eKARMA_KW_PLUS_MINUS					:
			case	eKARMA_KW_HERO_JOB						:
			case	eKARMA_KW_ALLIGNMENT					:
			case	eKARMA_KW_QUEST_OWN						:
			case	eKARMA_KW_QUEST_COMPLETE				:
			case	eKARMA_KW_QUEST_COMPLETE_CHECKER		:
			case	eKARMA_KW_CHECK_QUEST_COMPLETE_CHECKER	:
			case	eKARMA_KW_AREA_IN_OUT					:
			case	eKARMA_KW_ITEM_OWN						:
			case	eKARMA_KW_GUILD_MEMBER					:
			case	eKARMA_KW_ITEM_EQUIP					:
			case	eKARMA_KW_JOB_OWN						:
			case	eKARMA_KW_PARTY_GUILD_MEMBER_EXIST		:
			case	eKARMA_KW_ALL_PARTY_MEMBER_IN_SAME_GUILD:
			case	eKARMA_KW_SPECIFIC_ITEM_OWN_MEMBER_EXIST:
			case	eKARMA_KW_EXIST							:
			case	eKARMA_KW_OWN							:
			case	eKARMA_KW_ADD_REMOVE					:
			case	eKARMA_KW_ADD_SET_REMOVE				:
			case	eKARMA_KW_MOVE_PATROL					:
			case	eKARMA_KW_ARCA_DOOR_TRAP				:
			case	eKARMA_KW_ARCA_DOOR_TRAP_STATUS			:
			case	eKARMA_KW_TARGET_PLAYER					:
			case	eKARMA_KW_TARGET						:
			case	eKARMA_KW_NOTICE_RANGE					:
			case	eKARMA_KW_NOTICE_TYPE					:
			case	eKARMA_KW_PHYSICAL_MAGIC				:
			case	eKARMA_KW_QUEST_PROCESS_LEVEL			:
			case	eKARMA_KW_PARTY_MEMBER_LOCATION			:
			case	eKARMA_KW_INCLUDE_TRANSFORMATION_JOB	:
			case	eKARMA_KW_IDA_ANIDA						:
			case	eKARMA_KW_IS_PARTY_LEADER				:
			case	eKARMA_KW_MONSTER_TARGET_SELF			:
			case	eKARMA_KW_DUEL_INTERFACE				:
			case	eKARMA_KW_INVINCIBLE_ON_OFF				:
			case	eKARMA_KW_EVENT_BEGIN_END				:
			case	eKARMA_KW_QUEST_ABLE					:
			case	eKARMA_KW_SET_CANCEL					:
			case	eKARMA_KW_START_END_RESET				:
			case	eKARMA_KW_BATTLE_STATUS					:
			case	eKARMA_KW_DIRECT						:
			case	eKARMA_KW_COUNTDOWN_TYPE				:
			case	eKARMA_KW_SETTING_STATUS				:
			case	eKARMA_KW_COUNTDOWN_STATUS				:
			case	eKARMA_KW_QUEST_COUNTDOWN_CHECK			:
			case	eKARMA_KW_TARGET_LEADER					:
			case	eKARMA_KW_SKILL_EFFECT_TYPE				:
			case	eKARMA_KW_SET_RESET						:
			case	eKARMA_KW_ENABLE_DISABLE				:
			case	eKARMA_KW_ALL_PLAYER					:
			case	eKARMA_KW_ACTION						:
			case	eKARMA_KW_CHECK_ON_OFF					:
			case	eKARMA_KW_SET_PLUS_MINUS				:
			case	eKARMA_KW_WRITE_ERASE					:
			case	eKARMA_KW_APPLY_PREMIUM_BONUS			:
			case	eKARMA_KW_ABLE_DISABLE					:
			case	eKARMA_KW_TEST_NORMAL					:
			case	eKARMA_KW_CURRENT_STATUS				:
			case	eKARMA_KW_SET_RESET_SCROLL				:
			case	eKARMA_KW_ENTRY_DEATH					:
			case	eKARMA_KW_BOSS_MONSTER					:
			case	eKARMA_KW_METHOD_OF_GIVE_GP				:
			case	eKARMA_KW_TARGET_OWNER					:
			case	eKARMA_KW_MOVE_METHOD					:
			case	eKARMA_KW_TALK_METHOD					:
			case	eKARMA_KW_DAMAGE_METHOD					:
			case	eKARMA_KW_COMPLETE_STOP					:
			case	eKARMA_KW_IGNORE_INCLUDE_EQUIPMENT		:
			case	eKARMA_KW_RANK_IN_GUILD					:
			case	eKARMA_KW_SEX							:
			case	eKARMA_KW_LOCAL							:
			case	eKARMA_KW_BY_LEVEL						:
			case	eKARMA_KW_GIVE_DROP						:
			case	eKARMA_KW_HEAVEN_REWARD		:
			case	eKARMA_KW_HELL_REWARD		:
			case	eKARMA_KW_RED_DEVIL_REWARD	:
			case	eKARMA_KW_PLUSNUMBER_MINUSNUMBER:
			case	eKARMA_KW_HEAVEN_BONUS	:
			case	eKARMA_KW_JANI_ANJINA	:
			case	eKARMA_KW_BONUS_PENELTY	:
			case	eKARMA_KW_EVENT	:
			case	eKARMA_KW_HANDA_ANHANDA	:
			case	eKARMA_KW_EVENT_MONSTER					:
			case	eKARMA_KW_GUILD_BATTLE_TYPE				:
			case	eKARMA_KW_GUILD_HALL_OWN_OTHER			:
			case	eKARMA_KW_START_END_EXTENSION_REDUCTION	:
			case	eKARMA_KW_ITEM_BOOLEAN_OPTION_LIST		:
			case	eKARMA_KW_SEASON_VALIABLE		:
			case	eKARMA_KW_GRACE					:
			case	eKARMA_KW_EFFECT_FLICKERINGLY	:
			case	eKARMA_KW_WEEKDAY				:
			case	eKARMA_KW_IS_SECRECTD_GUILDD		:
			case	eKARMA_STATE					:
			case	eKARMA_FIX_FIELD_RESISTANCE		:
			case	eKARMA_SCRREN_EFFECT			:
			case	eKARMA_BLIND_EFFECT				:
			case	eKARMA_QUEST_KIND	:
			case	eKARMA_FIELD_WEATHER :		// 기후변화 09.10.06		
			case	eKARMA_INCLUDE_SOLO	:
			case	eKARMA_KW_TILE_SET		:		// 타일셋
			case	eKARMA_KW_DRAWING_METHOD		:		// 드로잉 반복
			case	eKARMA_KW_POPUP_INTERFACE		:		//	인터페이스
			case	eKARMA_KW_OPEN_AND_CLOSE		:		//	열기 닫기
			case	eKARMA_KW_ADD_GOLD_REASON		:
				str	=	GetCustomComboText(iCode,lpKarmaData->m_aValue[iValueIndex]);
				break;
				
			case	eKARMA_KW_ITEM			:
			case	eKARMA_KW_EQUIPMENT		:
			case	eKARMA_KW_SPECIAL_ITEM	:
			case	eKARMA_KW_QUEST_ITEM	:
				str	=	GetItemText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_PLAYER_JOB	:
			case	eKARMA_KW_QUEST_JOB		:
			case	eKARMA_KW_ARBEIT_JOB	:
				str	=	GetPlayerJobText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SET_ELEMENT_ITEM	:
				str	=	GetSetElementItemText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_ITEM_PIECE		:
				str	=	GetItemPieceText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_TITLE			:
				str	=	GetTitleText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPECIFIC_ITEM	:
				str	=	GetSpecificItemText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_BOOK							:
				str	=	GetBookText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_DUNGEON_VALUE					:
				str	=	GetDungeonValueText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_MAIN_QUEST_VALUE				:
				str	=	GetMainQuestValueText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_MAIN_QUEST_SWITCH				:
				str	=	GetMainQuestSwitchText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_PATTERN		:
				str	=	GetPatternText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_GUILD_STATUE_SKILL			:
				str	=	GetGuildStatueSkillText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_RELATED_PLACE_OF_GUILD	:
				str	=	GetRelatedPlaceOfGuildText(lpKarmaData->m_aValue[iValueIndex]);
				break;


			case	eKARMA_KW_CHAPTER				:
				str	=	GetChapterText(lpKarmaData->m_aValue[iValueIndex]);
				break;
			case	eKARMA_KW_SKILL			:
				str	=	GetSkillText(lpKarmaData->m_aValue[iValueIndex]);
				break;

				//JBC	월드 수치 추가.	09-01-13
			case	eKARMA_KW_WORLD_VALUE	:
				str	=	GetNumberText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_FIELD_VALUE	:
				str	=	GetFieldValueText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_EVENT_TIME_NAME	:
				str	=	GetEventTimeNameText(lpKarmaData->m_aValue[iValueIndex]);
				break;


			case	eKARMA_KW_MONSTER	:
				str	=	GetMonsterText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_BATTLE_INCLINATION	:
				str	=	GetBattleInclinationText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SHOP			:
				str	=	GetShopText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_QUEST			:
				str	=	GetQuestText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_QUEST_ARBEIT					:
				str	=	GetQuestArbeitText(lpKarmaData->m_aValue[iValueIndex]);
				break;


			case	eKARMA_KW_CHARACTER						:
			case	eKARMA_KW_ALL_CHARACTER					:
			case	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER	:
				str	=	GetCharacterText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER2	:
				str	=	GetAllCharacterTargetPlayerText(lpKarmaData->m_aValue[iValueIndex]);
				break;


			case	eKARMA_KW_AREA			:
			case	eKARMA_KW_ALL_AREA		:
			case	eKARMA_KW_TRAP_AREA		:
				str	=	GetAreaText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPEECH		:
				str	=	GetSpeechText(lpKarmaData->m_aValue[iValueIndex]);
				
				break;

			case	eKARMA_KW_POWER			:
				str	=	GetPowerText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_FIELD			:
				str	=	GetFieldText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_SPIN		:
				l_Reader.FindCharOver(']');
				str	=	GetSpinText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_NUMBER		:
				l_Reader.FindCharOver(']');
				str	=	GetNumberText(lpKarmaData->m_aValue[iValueIndex]);
				break;

			case	eKARMA_KW_STRING		:
				str	=	lpKarmaData->m_strString;
				break;

			case	eKARMA_KW_SOUND_FILE					:
				str	=	lpKarmaData->m_strString;
				break;

			case	eKARMA_KW_TEXT_BOX		:
			{
				static	char	strString[80];

				ustrncpy(strString,lpKarmaData->m_strString,76);
				strcpy(strString,_ms("\n\n%s",strString));

				str	=	strString;
 				break;
			}
			default					:
				ERRMSG("error","what is it?? [%s]",lpKeyWord);
				break;
		}

		if (str)
		{
			CheckJongsung(str);
			Text.Add(str);
		}


	}

	return	Text.String;
}	//	cKARMA_WND::KarmaText(CKarmaItem	*lpKarmaDa