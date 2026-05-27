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
	{""},																//	eKARMA_KW_SPIN			
	{"이다","이하이다","이상이다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.
	{""},																//	eKARMA_KW_SKILL			,		//	마법 리스트
	{""},																//	eKARMA_KW_POWER			,		//	시나리오 리스트
	{""},																//	eKARMA_KW_ITEM			,		//	카르마 그룹 리스트
	{""},																//	eKARMA_KW_STRING		,		//	카르마 리스트
	{""},																//	eKARMA_KW_NUMBER		,		//	켠다/끈다.
	{""},																//	eKARMA_KW_HERO_JOB		,		//	켠다/끈다.
	{"절대 악","악","중립","선","절대 선","end"},						//	eKARMA_KW_ALLIGNMENT	,
	{""},																//	eKARMA_KW_GUILD_ITEM	
	{""},																//	eKARMA_KW_TITLE
	{""},																//	eKARMA_KW_GUILD_TITLE
	{"보유하고 있다.","보유하고 있지 않다.","end"},						//	eKARMA_KW_QUEST_OWN
	{"완료했다.","완료하지 않았다.","end"},								//	eKARMA_KW_QUEST_COMPLETE
	{"소유하고 있다.","소유하고 있지 않다.","end"},						//	eKARMA_KW_ITEM_OWN
	{"가입해 있다.","가입해 있지 않다.","end"},							//	eKARMA_KW_GUILD_MEMBER
	{"장착하고 있다.","장착하고 있지 않다.","end"},						//	eKARMA_KW_ITEM_EQUIP
	{"가지고 있다.","가지고 있지 않다.","end"},							//	eKARMA_KW_JOB_OWN
	{"있다.","없다","end"},												//	eKARMA_KW_PARTY_GUILD_MEMBER_EXIST
	{"같은 길드에 가입해 있다.","서로 다른 길드에 가입해 있다.","end"},	//	eKARMA_KW_ALL_PARTY_MEMBER_IN_SAME_GUILD
	{"있다.","없다","end"},												//	eKARMA_KW_SPECIFIC_ITEM_OWN_MEMBER_EXIST
	{"있다.","없다","end"},												//	eKARMA_KW_EXIST
	{"보유하고 있다.","보유하고 있지 않다.","end"},						//	eKARMA_KW_OWN
	{""},																//	eKARMA_KW_SPEECH
	{""},																//	eKARMA_KW_SHOP
	{""},																//	eKARMA_KW_QUEST
	{"1 단계","2 단계","3 단계","4 단계","5 단계","6 단계","end",},		//	eKARMA_KW_QUEST_PROCESS_LEVEL
	{""},																//	eKARMA_KW_SPECIAL_ITEM
	{"더한다.","뺀다.","end"},											//	eKARMA_KW_ADD_REMOVE
	{""},																//	eKARMA_KW_FIELD
	{""},																//	eKARMA_KW_BATTLE_INCLINATION
	{""},																//	eKARMA_KW_AREA
	{""},																//	eKARMA_KW_ALL_AREA
	{"타겟","모든 파티원","파티 리더","파티원중 아무나 한명","펫", "end"},	//	eKARMA_KW_TARGET_PLAYER
	{"대화 상대","NPC","몬스터","운영자","운영자를 제외한 모든 플레이어","모든 플레이어","end"},	//	eKARMA_KW_TARGET
	{""},																//	eKARMA_KW_CHARACTER
	{""},																//	eKARMA_KW_ALL_CHARACTER
	{""},																//	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER
	{"필드 전체에","서버 전체에","본인만 듣게","소속 파티만 듣게","end"},	//	eKARMA_KW_NOTICE_RANGE
	{"채팅창 메시지","시스템 메시지","전체 공지","end"},				//	eKARMA_KW_NOTICE_TYPE
	{""},																//	eKARMA_KW_TEXT_BOX
	{"물리","마법","end"},												//	eKARMA_KW_PHYSICAL_MAGIC
	{""},																//	eKARMA_KW_EQUIPMENT
	{""},																//	eKARMA_KW_SPECIFIC_ITEM
	{""},																//	eKARMA_KW_QUEST_ARBEIT
	{"더해준다.","빼준다.","end"},										//	eKARMA_KW_PLUS_MINUS
	{""},																//	eKARMA_KW_QUEST_ITEM
	{"모여있다.","같은 필드 안에 있다.","end"},							//	eKARMA_KW_PARTY_MEMBER_LOCATION
	{""},																//	eKARMA_KW_PLAYER_JOB
	{"변신후 직업 포함","변신후 직업 무시","end"},						//	eKARMA_KW_INCLUDE_TRANSFORMATION_JOB
	{"이다.","아니다.","end"},											//	eKARMA_KW_IDA_ANIDA
	{"파티의 리더이다.","파티의 리더가 아니다.","end"},					//	eKARMA_KW_IS_PARTY_LEADER
	{"이다","이하이다","이상이다","이 아니다.","end"},					//	eKARMA_KW_PARTY_MEMBER_EQUAL
	{"이상","이하","end"},												//	eKARMA_KW_ONLY_EQUAL
	{"안","밖","end"},													//	eKARMA_KW_IN_OUT
	{"상대가 운영자 이다.","상대가 운영자가 아니다.","end"},			//	eKARMA_KW_CHECK_OPERATOR
	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","end"},	//	eKARMA_KW_QUEST_TARGET
	{"넘었다.(포함)","아직 안됐다.","end"},								//	eKARMA_KW_TIME_CHECK
	{"완료 체커가 체크되어 있다.","완료 체커가 체크되어 있지 않다.","end"},	//	eKARMA_KW_QUEST_COMPLETE_CHECKER
	{"완료 체커를 해제한다.","완료 체커를 체크한다.","end"},			//	eKARMA_KW_CHECK_QUEST_COMPLETE_CHECKER
	{""},																//	eKARMA_KW_DUNGEON_VALUE
	{"안에 있다","밖에 있다.","end"},									//	eKARMA_KW_AREA_IN_OUT
	{"더한다.","뺀다.","으로한다.","end"},								//	eKARMA_KW_ADD_SET_REMOVE
	{"이동 시킨다.","순찰 시킨다.","텔레포트 시킨다.","end"},			//	eKARMA_KW_MOVE_PATROL
	{"나타나게 한다.","숨긴다.","리젠 시킨다.","제거한다.","잠근다.","딴다.(자물쇠 열기)","닫는다","연다","end"},	//	eKARMA_KW_ARCA_DOOR_TRAP_STATUS
	{"보물 상자","문","함정","end"},									//	eKARMA_KW_ARCA_DOOR_TRAP
	{"스스로","몬스터","타겟","타겟의 파티","end"},						//	eKARMA_KW_MONSTER_TARGET_SELF			,
	{"무적 상태로 만든다.","무적 상태를 해제한다.","end"},				//	eKARMA_KW_INVINCIBLE_ON_OFF
	{""},																//	eKARMA_KW_TRAP_AREA
	{"이벤트를 시작한다.","이벤트를 끝낸다.","end"},					//	eKARMA_KW_EVENT_BEGIN_END				,
	{"할 수 있다.","할 수 없다.","end"},								//	eKARMA_KW_QUEST_ABLE				,
	{""},																//	eKARMA_KW_QUEST_JOB
	{""},																//	eKARMA_KW_ARBEIT_JOB
	{""},																//	eKARMA_KW_SET_ELEMENT_ITEM
	{""},																//	eKARMA_KW_ITEM_PIECE
	{"설정","설정 취소","end"},											//	eKARMA_KW_SET_CANCEL					,
	{"현재 방향","대화 상대쪽","길잡이쪽","파티 리더쪽","북쪽","북동쪽","동쪽","남동쪽","남쪽","남서쪽","서쪽","북서쪽","end"},//	eKARMA_KW_DIRECT
	{"대형 타이머","하단 테마 타이머","end"},							//	eKARMA_KW_COUNTDOWN_TYPE
	{"시작","종료","리셋","end"},										//	eKARMA_KW_START_END_RESET
	{"전투 가능","전투 불가","end"},									//	eKARMA_KW_BATTLE_STATUS
	{"설정되어 있다.","설정되어 있지 않다.","end"},						//	eKARMA_KW_SETTING_STATUS
	{"이다","가 아직 안됐다.","를 지났다.","end"},						//	eKARMA_KW_COUNTDOWN_STATUS
	{"완료되었다.","진행중이다.","취소됐다.","사용되지 않았다.","end",},			//	eKARMA_KW_QUEST_COUNTDOWN_CHECK
	{"타겟","타겟 파티 리더","end"},									//	eKARMA_KW_TARGET_LEADER
	{"없음","타격","스킬","회복","시전","보조 스킬","시전자 효과","폭발","end"},		//	eKARMA_KW_SKILL_EFFECT_TYPE
	{"설정","설정 취소","end"},											//	eKARMA_KW_SET_RESET
	{"없음","걷기","뛰기","정지","맞기","변신1","변신2","죽음","앉기","액션1","액션2","액션3","액션4","액션5","액션6","end"},	//	eKARMA_KW_ACTION
	{"활성화 시킨다.","비활성화 시킨다.","end"},						//	eKARMA_KW_ENABLE_DISABLE
	{"타겟","타겟 파티원","모든 플레이어","end"},						//	eKARMA_KW_ALL_PLAYER
	{"","end"},															//	eKARMA_KW_SOUND_FILE
	{"","end"},															//	eKARMA_KW_MAIN_QUEST_VALUE
	{"","end"},															//	eKARMA_KW_MAIN_QUEST_SWITCH
	{"","end"},															//	eKARMA_KW_CHAPTER

	{"켜져있다","꺼져있다","end"},										//	eKARMA_KW_CHECK_ON_OFF
	{"으로 설정한다.","더해준다.","빼준다.","end"},						//	eKARMA_KW_SET_PLUS_MINUS
	{"","end"},															//	eKARMA_KW_BOOK
	{"쓴다","지운다","end"},											//	eKARMA_KW_WRITE_ERASE
	{"시작","종료","연장","단축","end"},								//	eKARMA_KW_START_END_EXTENSION_REDUCTION
	{"프리미엄 보너스 적용","프리미엄 보너스 적용 안함","레벨업에 필요한 퍼센티지로 적용","end"},			//	eKARMA_KW_APPLY_PREMIUM_BONUS

	{"할 수 있다.","할 수 없다.","end"},								//	eKARMA_KW_ABLE_DISABLE
	{"테스트","정식","end"},											//	eKARMA_KW_TEST_NORMAL
	{"양탄자를 타고 있는 상태","end"},									//	eKARMA_KW_CURRENT_STATUS

	{"설정","설정 취소","스크롤","end"},								//	eKARMA_KW_SET_RESET_SCROLL

	{"","end"},															//	eKARMA_KW_PATTERN
	{"보스 등장","보스 죽음","end"},									//	eKARMA_KW_ENTRY_DEATH
	{"세이지","티아메스","워터 드래곤","아크 데빌","드라코 리치","end"},//	eKARMA_KW_BOSS_MONSTER
	{"타겟","본인","end"},//	eKARMA_KW_TARGET_OWNER
	{"길드전 프롤로그","정식 길드전","end"},
	{"소속 길드의 길드홀","다른 길드의 길드홀","end"},
	{"","end"},															//	eKARMA_KW_GUILD_STATUE_SKILL
	{"","end"},															//	eKARMA_KW_RELATED_PLACE_OF_GUILD
	{"포인트의 길드 명예 포인트를 준다.","X 남은 시간만큼의 길드 명예 포인트를 준다.","end"},	//	eKARMA_KW_METHOD_OF_GIVE_GP
	{"영역에 임의의 위치로 이동","영역에 중심으로 이동","end"},			//	eKARMA_KW_MOVE_METHOD
	{"채팅창에 이벤트 대사","말 풍선 대사","end"},						//	eKARMA_KW_TALK_METHOD
	{"대미지 주기","남은 체력에 비례해 퍼센티지 대미지 주기","최대 체력에 비례해 퍼센티지 대미지 주기","end"},						//	eKARMA_KW_DAMAGE_METHOD
	{"완료(비밀던젼을 클리어 함)","종료(클리어 못하고 강제 종료 함)","end"},	//	eKARMA_KW_COMPLETE_STOP
	{"착용 장비 무시","착용 장비 포함","end"},	//	eKARMA_KW_IGNORE_INCLUDE_EQUIPMENT		,
	{"가입해 있지 않음.","신입","정식멤버","의원","서브 마스터","원로","마스터","end"},	//	eKARMA_KW_RANK_IN_GUILD
	{"남자","여자","end"},	//	eKARMA_KW_SEX							,
	{"식목일 이벤트 몬스터","end"},	//	eKARMA_KW_SEX							,
	{"한국","일본","중국","미국","태국","end"},	//	eKARMA_KW_LOCAL							,
	{"레벨 비례하지 않는다.","금액에 레벨을 곱한다.","end"},	//	eKARMA_KW_BY_LEVEL,
	{"준다.","필드에 드랍 시킨다.","end"},	//	eKARMA_KW_GIVE_DROP,
	{"","end"},															//	eKARMA_KW_FIELD_VALUE
	{"","end"},															//	eKARMA_KW_ALL_CHARACTER_TARGET_PLAYER2

	{"유니크 아이템 혹은 슈퍼 유니크 아이템 한 개 습득","보너스 스탯치","보너스 스킬 포인트","보너스 HP","보너스 CP","보너스 경험치","칭호 보너스","길드 레벨 보너스","길드 스킬 보너스","길드 아이템 보너스","성향 조정","end"},	//	eKARMA_KW_HEAVEN_REWARD,//	인스턴스 던전으로 이동
	{"거래 불가 아이템 거래 가능하게 변경","접두사 제거","스탯 리버스","스킬 리버스","HP 보너스 복구","CP 보너스 복구","스탯 보너스 복구","스킬 보너스 복구","길드 스킬 리셋","길드 스킬 보너스 복구","성향 조정","end"},	//	eKARMA_KW_HELL_REWARD
	{"아이템 복제","노말, 레어 아이템 능력 향상","노말, 레어 아이템 잠재 능령 향상","슈퍼 유니크 아이템 접두사 능력 향상","슈퍼 유니크 아이템 접두사 잠재 능력 향상","붉은 색의 바닥 없는 상자","보너스 HP 부적","보너스 CP 부적","보너스 장비 부적","보너스 골드","성향 조정","end"},	//	eKARMA_KW_RED_DEVIL_REWARD
	{"플러스","마이너스","end"},//eKARMA_KW_PLUSNUMBER_MINUSNUMBER
	{"스탯","스킬","HP","CP","end"},//eKARMA_KW_HEAVEN_BONUS
	{"지났다","지나지 않았다","end"},	//	eKARMA_KW_JANI_ANJINA
	{"보너스","페널티","end"},//eKARMA_KW_BONUS_PENELTY
	{"YEPP 이벤트","end"},	//	eKARMA_KW_EVENT
	{"한다","안한다","초기화","end"},	//	eKARMA_KW_HANDA_ANHANDA
	{"","end"},	//	eKARMA_KW_MONSTER
	{"결투 랭킹 인터페이스","1 vs 1 대전 신청","2 vs 2 대전 신청","3 vs 3 대전 신청","4 vs 4 대전 신청","5 vs 5 대전 신청","예선전 신청","end"},	//	eKARMA_KW_DUEL_INTERFACE

	//	병찬씨가 추가
	{	"타운 텔레포트 무료","양탄자 소환 의뢰","신비석 추가 획득","탄틸리스 유물 추가 획득","길드홀 레벨 제한 없이 탄틸리스 유적 입장",
		"인피니티 시리즈 업그레이드 확률 상승","무료 패스트 포탈","반값 패스트 포탈","end"},					//eKARMA_KW_ITEM_BOOLEAN_OPTION_LIST
	{"0 시즌" , "1 시즌","2 시즌","3 시즌","4 시즌","5 시즌","6 시즌","end"},		//eKARMA_KW_SEASON_VALIABLE
	{"불의 은총","물의 은총","바람의 은총","땅의 은총","end"},//eKARMA_KW_GRACE
	{"","end"},															//	eKARMA_KW_WORLD_VALUE
	{"효과 없음", "깜빡인다" , "end"},													//eKARMA_KW_EFFECT_FLICKERINGLY
	{"일","월","화","수","목","금","토","end",},		//	eKARMA_KW_WEEKDAY
	{"비밀 던젼","길드 던젼","end"}	,//eKARMA_KW_IS_SECRECTD_GUILDD
	{"스턴","프리즈","콜드","수면","장님" ,"석화","혼란","갑옷 파괴" , "무기파괴","버서크" ,"end"},//eKARMA_STATE
	{"불 저항","물 저항","바람 저항","대지 저항","암흑 저항","빛 저항","장님 저항","중독 저항","잠 저항","콜드 저항","프리즈 저항","스턴 저항","석화 저항","혼란 저항","챠밍 저항","크리티컬 저항","결정타 저항","end"},//eKARMA_FIX_FIELD_RESISTANCE
	{"흑백","TV노이즈","end"},//	eKARMA_SCRREN_EFFECT
	{"제거","매우 작은","작은","보통의","큰","매우 큰","end"},//eKARMA_BLIND_EFFECT
	{"모든 퀘스트","일반 퀘스트","스킬 마스터","칭호 퀘스트","메인 스토리 퀘스트","길드 퀘스트","end"},

	{"맑음", "눈", "비","end"},		// eKARMA_FIELD_WEATHER		09.10.06  기후변화.
	{"솔로 포함 하지 않음","솔로 포함","end"},	//	eKARMA_INCLUDE_SOLO						, // 파티 관련 체크 일때 솔로 포함?
	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","모든 파티 멤버(솔로 포함)","파티 리더(솔로 포함)","파티원중 아무나 하나(솔로 포함)","end"},	//	eKARMA_KW_QUEST_TARGET_CHECK_SOLO
	{"초원","산악", "사막", "사바나", "동굴", "던젼", "타워", "광산", "자하세계(지옥)", "천상계(천국)", "마을 - 브룬넨슈티그", "마을 - 비개플","마을 - 아우구스타","마을 - 브리지헤드",
	"마을 - 산악마을","마을 - 아리안","마을 - 폐허","마을 - 농가","마을 - 집시촌","집안 내부","길드 홀","end"},	

	{"","end"},															//	eKARMA_KW_EVENT_TIME_NAME
	{"없음","상하반복", "좌우반복", "end"},								//	eKARMA_KW_DRAWING_METHOD
	{"없음","인벤토리창", "상태창","스킬창", "퀘스트창","파티창", "길드창", "친구창", "시스템창", "end"},								//	eKARMA_KW_DRAWING_METHOD
	{"열기","닫기", "end"},								//	eKARMA_KW_OPEN_AND_CLOSE

	{"그냥","금괴 구매 대금","퀘스트","end"},								//	eKARMA_KW_ADD_GOLD_REASON


	{""},	//	얘는 항상 마지막에 있어야 한다.
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

static	char	*l_strCheckKarmaJongsung[]	=	{"은 ","는 ","이 ","가 ","을 ","를 ","end"};

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

	s_aCombo[g_iComboIndex].Add("모든 몬스터"	,dCHARACTER_ALL);
	s_aCombo[g_iComboIndex].Add("본인"			,dCHARACTER_SELF);

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

	s_aCombo[g_iComboIndex].Add("모든 플레이어"		,dCHARACTER_PLAYER);
	s_aCombo[g_iComboIndex].Add("타겟 플레이어"		,dCHARACTER_TARGET_PLAYER);
	s_aCombo[g_iComboIndex].Add(dMSG_TARGET_PARTY	,dCHARACTER_TARGET_PARTY);
	s_aCombo[g_iComboIndex].Add("본인"				,dCHARACTER_SELF);
	s_aCombo[g_iComboIndex].Add("모든 몬스터"		,dCHARACTER_ALL);

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

	s_aCombo[g_iComboIndex].Add("타겟"					,0);
	s_aCombo[g_iComboIndex].Add("모든 파티원"			,1);
	s_aCombo[g_iComboIndex].Add("파티 리더"				,2);
	s_aCombo[g_iComboIndex].Add("파티원중 아무나 한명"	,3);
	s_aCombo[g_iComboIndex].Add("모든 몬스터"			,4);

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
		s_aCombo[g_iComboIndex].Add("먼저 링크 할 비던을 설정해 주세요.",0);
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

	if (data == dCHARACTER_PLAYER)			lpstrName	=	"모든 플레이어";
	else
	if (data == dCHARACTER_ALL)				lpstrName	=	"모든 몬스터";
	else
	if (data == dCHARACTER_TARGET_PLAYER)	lpstrName	=	"타겟 플레이어";
	else
	if (data == dCHARACTER_TARGET_PARTY)	lpstrName	=	dMSG_TARGET_PARTY;
	else
	if (data == dCHARACTER_SELF		)		lpstrName	=	"본인";
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