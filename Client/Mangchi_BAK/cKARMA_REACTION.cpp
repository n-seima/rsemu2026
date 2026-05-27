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
	"■■■ 대화/시스템 관련"						,0x7fff				,
	" NPC 대화/시스템 관련 리액션들[border_comment]0"					,

	"    대화 선택"							,eREACTION_SELECT_DIALOG	,
	" 대화 [speech]0으로 이동한다."										,

	"    대화 종료"							,eREACTION_END_DIALOG		,
	" 대화를 종료한다."													,

	"    비밀 던젼으로 입장"				,eREACTION_MOVE_TO_SECRET_DUNGEON					,//	인스턴스 던전으로 이동
	" 비밀 던전으로 이동 시킨다.(비밀 던전 입구 전용 리액션)",

	" S 카운트 다운"						,eREACTION_SET_COUNTDOWN_TIMER	,//	인스턴스 던전으로 이동
	" 카운트 다운 타이머를 [start_end_extension_reduction]0. [spin]1[0,60]시간 [spin]2[0,60]분 [spin]3[0,60]초",


	" S 비밀 던전 종료"					,eREACTION_COMPLETE_SECRET_DUNGEON	,//	인스턴스 던전으로 이동
	" [is_secrectD_or_guildD]2을 종료한다. [complete_stop]0 [spin]1[0,30000]초 후에 종료.(0초로 설정시 60초로 설정됨)",

	"    결투장 관련 인터페이스"			,eREACTION_DUEL_INTERFACE		,
	" [duel_interface]0을 연다."									,

	"    상점 거래창"						,eREACTION_SHOP				,
	" 상점 [shop]0의 거래창을 연다."									,
	"    강화창"						,eREACTION_UPGRADE_SHOP				,			// 09.08.25
	" 강화창을 연다."													,	
	"    아이템 보관소"						,eREACTION_OPEN_BANK		,	//	아이템 보관창을 연다.
	" 아이템 보관창을 연다."											,

	"    길드 생성"							,eREACTION_MAKE_GUILD		,	//	아이템 보관창을 연다.
	" 길드 생성 인터페이스를 연다."										,

	"    길드 상점"							,eREACTION_OPEN_GUILD_SHOP	,	//	길드 상점창 열기
	" 길드 상점 거래창을 연다."										,

	"    길드전 신청 인터페이스"			,eREACTION_OPEN_GUILD_BATTLE_SCHEDULER,	//	길드전 신청
	" 길드전 신청 인터페이스를 오픈한다. [guild_battle_type]0. "								,

	"    길드마크 조합 인터페이스"				,eREACTION_OPEN_GUILD_MARK_COMPOSER,	//	길드 마크 조합 인터페이스 오픈
	" 길드마크 조합 인터페이스를 오픈한다."								,

	"    길드홀 레벨 설정"				,eREACTION_SET_GUILD_HALL_LEVEL	,	//	1 레벨 길드 홀 부여
	" 길드홀 레벨을 [spin]0[1,5]로 만들어 준다.(1레벨로만 사용 할것!)"	,

	"    길드홀로 이동"				,eREACTION_MOVE_TO_GUILD_HALL,	///	길드 홀로 이동
	" 길드 홀로 이동한다. [own_other]0"								,


	"   길드 석상 스킬 레벨 증가",eREACTION_INCREASE_GUILD_STATUE_SKILL_LEVEL	,//	길드 석상 스킬 레벨 증가
	" 길드 석상 스킬 [guild_statue_skill]0 레벨이 증가한다.",

	"   길드 인벤토리 오픈"		,eREACTION_OPEN_GUILD_INVENTORY			,//	길드 아아템 창 열기
	" 길드 인벤토리를 연다.",

	"   길드 연고지 추가"			,eREACTION_ADD_GUILD_PLACE_OF_RELATED		,//	길드 연고지 추가
	" 다음 마을을 길드 연고지에 [add_remove]0. [related_place_of_guild]1",

	"!  길드홀 모양 변경"			,eREACTION_CHANGE_GUILD_HALL_SHAPE		,//	길드 연고지 추가
	" 길드홀의 모양을 변경 한다.",

	"   길드 포인트전 신청"			,eREACTION_TRY_GUILD_POINT_BATTLE	,//	길드 포인트 전 신청
	" 길드 포인트전을 신청한다.",

	"   길드 명예 포인트 수여"			,eREACTION_GIVE_GUILD_HONOR_POINT	,//	길드 포인트 전 신청
	" [spin]0[1,30000] [method_of_give_ghp]1",

	"   길드 포인트전 종료"			,eREACTION_END_GUILD_POINT_BATTLE_AND_RECORD_GUILD_HONOR_POINT	,//	길드 포인트 전 종료
	" 길드 포인트전 종료 [complete_stop]0",

	"   길드 공성전 필드 입장"			,eREACTION_BEGIN_SIEGE_WARFARE_OF_GUILD	,
	" 길드 공성전 필드 입장",

	"   길드 연고지 리스트 열기"		,eREACTION_OPEN_RELATED_PLACE_LIST_OF_GUILD	,
	" 길드 연고지 리스트 열기",

	"   길드 명예 포인트 보기"			,eREACTION_OPEN_GUILD_HONOR_POINT_WINDOW,
	" [spin]0[1,4]레벨 길드홀을 소유한 길드들의 명예 포인트를 보여 준다.",

	"   길드 펫&가디언 사육"			,eREACTION_FEED_GUILD_PET_AND_GUARDIAN,
	" 길드 펫과 가디언 사육 창 열기",

	"   길드 로엔의 호감도 "					,eREACTION_SETTING_ROEN_GOODWILL			,//	로엔의 호감도 조정.
	" 길드의 로엔의 호감도 [spin]0[0,6]를  [add_remove_set]1",

	"   길드 던젼 입장"			,eREACTION_ENTER_GUILD_DUNGEN	,//	길드 포인트 전 신청
	" [spin]0[0,10]레벨 길드 던젼에 입장한다.",

	"    아이템 수리창 오픈"					,eREACTION_OPEN_REPAIR_ITEM_WINDOW,
	" 아이템 수리창을 오픈한다."										,

	"    마법의 양탄자 소환"					,eREACTION_CALL_MAGIC_CARPET,	//	마법의 양탄자 소환
	" 마법의 양탄자를 소환한다."										,

	"    브레이크"								,eREACTION_BREAK_KARMA	,	//	브레이크
	" 이후에 카르마를 무시한다."										,

	"    카르마 점프"							,eREACTION_JUMP_KARMA	,	//	카르마 점프
	" [spin]0[0,10000]번 카르마로 이동한다.(시간 지연 카리마 사용시 카르마 점프는 제대로 사용 되지 않습니다."	,

	"    임의의 카르마 실행"					,eREACTION_LAUNCH_RANDOM_KARMA,	//	랜덤 카르마
	" [spin]0[0,10000]번 카르마부터 [spin]1[0,10000]번 카르마까지의 카르마중 [spin]2[1,100]개의 카르마를 랜덤하게 골라 동작 시킨다. 동작후 [spin]3[0,10000]번 카르마로 점프 [hand_anhanda]4"	,
	
	"    임의의 리액션 실행"					,eREACTION_LAUNCH_RANDOM_REACTION,	//	랜덤 리類션
	" [spin]0[0,10000]번 리액션부터 [spin]1[0,10000]번 리액션까지의 리액션중 [spin]2[1,100]개의 리액션을 랜덤하게 골라 동작 시킨다. 동작후 [spin]3[0,10000]번 리액션으로 점프 [hand_anhanda]4"	,

	"    아이템 감별창"							,eREACTION_OPEN_JUDGE_OF_ITEM_WINDOW,		//	아이템 감별창 오픈
	" 아이템 감별창을 연다",

	"    조각 아이템 제작"						,eREACTION_OPEN_MERGE_ITEM_PIECE_WINDOW		,//	아이템 조각 합체창 오픈
	" 조각 아이템 제작창을 연다.",

	"    아이템 분해창 오픈"					,eREACTION_OPEN_DISJOINTING_WINDOW,
	" 아이템 분해창을 오픈한다."										,

	"    인터페이스 창 열기/닫기"					,eREACTION_OPEN_INTERFACE,
	" 인터페이스 창 [popup_interface]0을 [open_and_close]1 (창 조작을 [quest_able]2)",

	"    패스트 포탈 창 열기"					,eREACTION_OPEN_FAST_PORTAL_WINDOW,
	" 패스트 포탈 창 연다",
	
	"■■■ 퀘스트 관련"					,0x7fff						,
	"    이벤트에 관련된 리액션들[border_comment]0"						,

	"    퀘스트 시작"							,eREACTION_SET_QUEST_START	,//	퀘스트 시작 상태로 만든다.
	" 퀘스트 [quest]0을 진행중인 퀘스트에 추가한다. 대상 [quest_target]1"					,

	"    퀘스트 완료"							,eREACTION_SET_QUEST_COMPLETE,//	퀘스트를 완료한다.
	" 퀘스트 [quest]0을 완료 상태로 만든다. 대상 [quest_target]1"							,

	"   퀘스트 완료 체커"						,eREACTION_CHECK_QUEST_COMPLETE_CHECKER,//	퀘스트를 완료한다.
	" 퀘스트 [quest]0의 [check_quest_complete_checker]2 대상 [quest_target]1"		,

	"    퀘스트 상태 변경"						,eREACTION_SET_QUEST_PROCESS_LEVEL	,//	퀘스트 단계 변경.
	" 퀘스트 [quest]0의 상태를 [quest_process_level]1로 설정한다. 대상 [quest_target_check_solo]2"		,

	"    퀘스트 관련 수치 설정"					,eREACTION_SET_QUEST_VALUE	,//퀘스트 수치 설정
	" 퀘스트 [quest]0의 수치를 [spin]1[0,20000]로 설정한다. 대상 [quest_target_check_solo]2 영역 [all_area]3"	,

	"    퀘스트 수치 증가/감소"					,eREACTION_CONTROL_QUEST_VALUE	,//퀘스트 상태 변경
	" 퀘스트 [quest]0의 수치를 [spin]1[1,255] [plus_minus]2 대상 [quest_target_check_solo]3 영역 [all_area]4 Max [number]5[0,2047]"	,

	"    퀘스트 취소"							,eREACTION_CANCEL_QUEST			,//퀘스트 취소
	" 퀘스트 [quest]0를 취소시킨다. 대상 [quest_target]1"	,

	"   퀘스트 카운트 다운 타이머 설정"		,eREACTION_SET_QUEST_COUNTDOWN_TIMER	,//	인스턴스 던전으로 이동
	" 퀘스트 [quest]0의 카운트 다운 타이머를 [start_end_reset]1. [spin]2[0,60]시간 [spin]3[0,60]분 [spin]4[0,60]초. 대상 [quest_target]5",


	"    아르바이트 시작"						,eREACTION_SET_ARBEIT_START	,//	아르바이트 시작 상태로 만든다.
	" 아르바이트 [quest_arbeit]0을 진행중인 퀘스트에 추가한다. 대상 [quest_target]1"			,

	"    아르바이트 완료"						,eREACTION_SET_ARBEIT_COMPLETE,//	아르바이트 시작 상태로 만든다.
	" 아르바이트 [quest_arbeit]0을 완료 상태로 만든다. 대상 [quest_target]1 종료 메세지 출력[hand_anhanda]2"			,

	"   아르바이트 완료 체커"					,eREACTION_CHECK_ARBEIT_COMPLETE_CHECKER,//	퀘스트를 완료한다.
	" 아르바이트 [quest_arbeit]0의 [check_quest_complete_checker]2 대상 [quest_target]1"							,

	"    아르바이트 상태 변경"					,eREACTION_SET_ARBEIT_PROCESS_LEVEL	,//아르바이트 상태 변경
	" 아르바이트 [quest_arbeit]0의 상태를 [quest_process_level]1로 설정한다. 대상 [quest_target_check_solo]2"	,

	"    아르바이트 관련 수치 설정"				,eREACTION_SET_ARBEIT_VALUE	,//아르바이트 수치 설정
	" 아르바이트 [quest_arbeit]0의 수치를 [spin]1[0,20000]로 설정한다. 대상 [quest_target_check_solo]2 영역 [all_area]3"	,

	"    아르바이트 수치 증가/감소"				,eREACTION_CONTROL_ARBEIT_VALUE,//아르바이트 상태 변경
	" 아르바이트 [quest_arbeit]0의 수치를 [spin]1[1,255] [plus_minus]2 대상 [quest_target_check_solo]3 영역 [all_area]4 Max [number]5[0,2047]"	,

	"    아르바이트 취소"						,eREACTION_CANCEL_ARBEIT	,//아르바이트 취소
	" 아르바이트 [quest_arbeit]0를 취소시킨다. 대상 [quest_target]1"	,

	"   아르바잍 카운트 다운 타이머 설정"		,eREACTION_SET_ARBEIT_COUNTDOWN_TIMER	,//	인스턴스 던전으로 이동
	" 아르바이트 [quest_arbeit]0의 카운트 다운 타이머를 [start_end_reset]1. [spin]2[0,60]시간 [spin]3[0,60]분 [spin]4[0,60]초.  대상 [quest_target]5",


	"   챕터 설정"								,eREACTION_SET_CHAPTER	,//	인스턴스 던전으로 이동
	" 진행 중인 챕터를 [chapter]0 상위 퀘스트 [spin]1[0,60]으로 [set_cancel]2.",

	"   메인 퀘스트 수치 설정"							,eREACTION_SET_MAIN_QUEST_VALUE	,//	메인 퀘스트 수치 x 의 값을 y [로 설정한다/증가 시킨다/감소 시킨다.]
	" 메인 퀘스트 수치 [main_quest_value]0을 [number]1[0,65535] [set_plus_minus]2.",

	"   메인 퀘스트 스위치 설정"							,eREACTION_SET_MAIN_QUEST_SWITCH	,//	메인 퀘스트 수치 x 의 값을 y [로 설정한다/증가 시킨다/감소 시킨다.]
	" 메인 퀘스트 스위치 [main_quest_switch]0을 [set_cancel]1",

	"   메인 퀘스트 수치 리셋"							,eREACTION_RESET_MAIN_QUEST_VALUE	,//	메인 퀘스트 수치 x 의 값을 y [로 설정한다/증가 시킨다/감소 시킨다.]
	" 메인 퀘스트 수치를 모두 리셋한다.",

	"   메인 퀘스트 스위치 리셋"						,eREACTION_RESET_MAIN_SWITCH	,//	메인 퀘스트 수치 x 의 값을 y [로 설정한다/증가 시킨다/감소 시킨다.]
	" 메인 퀘스트 스위치를 모두 리셋한다.",

	"   챕터 에필로그 출력"								,eREACTION_DISPLAY_MAIN_QUEST_EPILOG	,//	인스턴스 던전으로 이동
	" 챕터 [chapter]0의 에필로그 화면을 보여준다. 이전에 [spin]1[0,60]번 이미지를 [spin]2[1,600]초간 출력",


	"   천상계 보상"									,eREACTION_RECEIVE_HEAVEN_REWARD,//	인스턴스 던전으로 이동
	" 천상계 보상 [heaven_reward]1을 준다.",
	
	"   지하계 보상"									,eREACTION_RECEIVE_HELL_REWARD,//	인스턴스 던전으로 이동
	" 지하계 보상 [hell_reward]1을 준다.",
	
	"   붉은 악마 보상"								,eREACTION_RECEIVE_RED_DEVIL_REWARD,//	인스턴스 던전으로 이동
	" 붉은 악마 보상 [red_devil_reward]1을 준다.",


	"■■■ 아이템 관련"					,0x7fff						,
	" 아이템에 관련된 리액션들[border_comment]0"						,

	"    골드 추가"								,eREACTION_ADD_GOLD			,	
	" [number]0[1,2147483647]골드를 [add_remove]1 대상 [quest_target]2  [by_level]3 지급 이유 [add_gold_reason]4"	,

	"    아이템 추가/제거"						,eREACTION_ADD_ITEM			,	
	" [item]0 [spin]1[0,1024]개를 [add_remove]2 대상 [quest_target]3 (0개를 제거 하면 해당 아이템 모두 제거) [ignore_include_equipmment]4",

	"    아이템 추가(옵션)"						,eREACTION_ADD_ITEM_WITH_OPTION			,	
	" [item]0 1개를 추가한다. 대상 [quest_target]1 옵션 1([spin]2[0,255]~[spin]3[0,255]) 옵션 2([spin]4[0,255]~[spin]5[0,255])",

	"    특수 아이템 추가"						,eREACTION_ADD_SPECIAL_ITEM	,	
	" [special_item]0 [spin]1[1,255]개를 [add_remove]2 대상 [quest_target]3 (0개를 제거 하면 해당 아이템 모두 제거) [ignore_include_equipmment]4",

	"    퀘스트 아이템 추가"					,eREACTION_ADD_QUEST_ITEM	,	
	" [quest_item]0 [spin]1[1,255]개를 [add_remove]2 대상 [quest_target]3 (0개를 제거 하면 해당 아이템 모두 제거) [ignore_include_equipmment]4" ,

	"    특정 아이템 추가"						,eREACTION_ADD_SPECIFIC_ITEM	,	
	" [specific_item]0를 더해준다. 대상 [quest_target]1"										,

	"    세트아이템 랜덤 추가"						,eREACTION_ADD_SET_ITEM_RANDOM			,	
	" [spin]0[0,10]부위 세트아이템(0이면 전체랜덤)을 랜덤으로 대상 [quest_target]1 에게 [spin]2[0,10] 부위(0이면 세트지급) 랜덤 추가(중복X).",

	
	"    금괴 판매 NPC카르마"						,eREACTION_BUY_GOLD_BAR	,			// 금괴 추가.....09.08.19
	" 플레이어에게 금괴 [spin]0[1,20]개를 판다.(수수료를 제외한 가격 개당 1억)"		,

	"    골드 드롭"								,eREACTION_DROP_GOLD		,	
	" [number]0[1,2147483647]골드를 떨어뜨린다."						,

	"    아이템 드롭"							,eREACTION_DROP_ITEM		,	
	" [item]0 [spin]1[1,255]개를 떨어뜨린다."							,

	"    아이템 드롭(옵션)"						,eREACTION_DROP_ITEM_WITH_OPTION			,	
	" [item]0 [spin]1[1,255]개를 떨어뜨린다. 옵션 1([spin]2[0,255]~[spin]3[0,255]) 옵션 2([spin]4[0,255]~[spin]5[0,255])",

	"    주인이 없는 아이템 드롭(옵션)"						,eREACTION_UNOWNED_DROP_ITEM_WITH_OPTION			,	
	" 주인이 없는 아이템 [item]0 [spin]1[1,255]개를 떨어뜨린다. 옵션 1([spin]2[0,255]~[spin]3[0,255]) 옵션 2([spin]4[0,255]~[spin]5[0,255])",

	"    특수 아이템 드롭"						,eREACTION_DROP_SPECIAL_ITEM,	
	" [special_item]0 [spin]1[1,255]개를 떨어뜨린다."					,

	"    퀘스트 아이템 드롭"					,eREACTION_DROP_QUEST_ITEM	,	
	" [quest_item]0 [spin]1[1,255]개를 떨어뜨린다."						,

	"    특정 아이템 드롭"						,eREACTION_DROP_SPECIFIC_ITEM,	
	" [specific_item]0 를 떨어뜨린다."									,

	"    세트 구성 아이템"						,eREACTION_ADD_SET_ELEMENT_ITEM			,//	세트 구성 아이템 추가
	" [set_element_item]0을 [give_drop]2 대상 [quest_target]1 (드랍은 반드시 사망 트리거에 넣을것. 이때 타겟은 무시함)"	,

	"    아이템 조각"							,eREACTION_ADD_ITEM_PIECE	,//	아이템 조각 추가
	" [item_piece]0의 [spin]1[0,15]번째 조각(0번은 랜덤)을 [give_drop]2 대상 [quest_target]3 (드랍은 사망 트리거에 넣을것.이때 타겟은 무시함)",

	"   아이템 유효기간 연장"						,eREACTION_EXTEND_ITEM_VALIDATE_TIME_TO_NEXT	,//	아이템 유효 기간을 다음날로 연장
	" [item]0 의 유효기간을 다음날로 연장한다.",
	 

	"   책 아이템 생성"						,eREACTION_CREATE_BOOK		,//	책 아이템 생성
	" 책 [book]0을 [add_remove]1(제거시 모든 책 제거)",

	"   책에 카테고리를 추가(이녀석 지울것!!)"	,eREACTION_CREATE_ADD_CATEGORY_TO_BOOK_ITEM	,//	책 아이템에 카테고리 추가 아이템 생성
	" (이녀석 지울것!!) 책 [book]0에 카테고리 [spin]1[1,18]번 부터 [spin]2[1,18]번 부터 추가하는 아이템을 생성한다.",

	"   책에 페이지를 추가하는 아이템 생성"	,eREACTION_CREATE_ADD_PAGE_TO_BOOK_ITEM	,//	책 아이템에 카테고리 추가 아이템 생성
	" 책 [book]0에 카테고리 [spin]1[1,18]번에 페이지 [spin]2[1,8]쪽 ~ [spin]3[1,8]쪽을 추가하는 아이템을 [add_remove]4(제거시 모든 페이지 제거)",

//	" N  책에 랜덤한 카테고리를 추가하는 아이템 생성"	,eREACTION_CREATE_ADD_RANDOM_CATEGORY_TO_BOOK_ITEM,//	책 아이템에 카테고리 추가 아이템 생성
//	" 책 [book]0에 임의의 카테고리 하나를 추가하는 아이템을 생성한다.",

	"   책에 랜덤한 페이지를 추가하는 아이템 생성"	,eREACTION_CREATE_ADD_RANDOM_PAGE_TO_BOOK_ITEM,//	책 아이템에 카테고리 추가 아이템 생성
	" 책 [book]0에 임의의 페이지 하나를 추가하는 아이템을 생성한다.",

	"   책에 페이지를 쓰거나 지움"	,eREACTION_WRITE_PAGE,//	책 아이템에 카테고리 추가 아이템 생성
	" 책 [book]0에 카테고리 [spin]1[1,18]번에 페이지 [spin]2[1,8]을 [write_erase]3",

	"   이벤트 풍선 터뜨리기"	,eREACTION_EXPLOSION_EVENT_BALLOON,//	
	" 이벤트 풍선 [item]0을 터뜨린다.",

	"   타임 스탬프 찍기"	,eREACTION_TIME_STAMP	,//	책 아이템에 카테고리 추가 아이템 생성
	" 아이템 [item]0에 현재 시간을 찍는다.",

	"   챕터 5 붉은 보석 조각 지급"	,eREACTION_GIVE_RED_STONE_PIECE	,//	책 아이템에 카테고리 추가 아이템 생성
	" 이런저런 조건을 다 고려해 파티원중 한명에게 붉은 보석 조각 하나를 지급한다.",


	"■■■ 캐릭터 관련"					,0x7fff						,
	" 캐릭터와 관련된 리액션들[border_comment]0"						,

//	" 캐릭터 출현"							,eREACTION_CREATE_CHARACTER	,//	캐릭터 추가
//	" [area]0에 [character]1가 [spin]2[1,255]명이 생성된다."			,
 
	"    은총 획득"								,eREACTION_GET_GRACE		,//	은총 획득
	" 타겟에게 [Grace]0을 [add_remove]1"		,

	"    칭호 획득"								,eREACTION_GET_TITLE		,//	칭호 획득
	" [target_player]0가 [spin]1[1,10]레벨 [title]2 칭호를 [add_remove]3"	,

	"    경험치 획득"							,eREACTION_GET_EXPERIENCE	,//	경험치 획득
	" [all_area]0에 [target_player]1의 경험치가 [number]2[1,2147483647] 오른다. [apply_premium_bonus]3 연관 퀘스트 [quest]4 [quest_arbeit]5"	,

	"    경험치 획득2"							,eREACTION_GET_EXPERIENCE2	,//	경험치 획득
	" [all_area]0에 [target_player]1의 레벨업에 필요한 경험치의 [number]2[1,100]% 오른다. [apply_premium_bonus]3 연관 퀘스트 [quest]4 [quest_arbeit]5",


	"    죽음 페널티"							,eREACTION_RECOVER_DEATH_PENELTY,
	" 타겟의 죽음 페널티를 치료한다. 유료 [ida_anida]0"										,

	"    캐릭터 제거"							,eREACTION_REMOVE_CHARACTER	,//	캐릭터 제거
	" [all_area]0에 모든 [all_character]1를 죽인다."					,

	"    필드 이동"								,eREACTION_MOVE_FIELD		,//	필드 이동
	" [all_area]0에 [target_player]1를 [field]2의 [number]3[0,4096],[number]4[0,4096]으로 이동시킨다."	,

	"    위치 이동"								,eREACTION_MOVE_AREA		,//	필드 이동
	" 영역 [all_area]0 [in_out]1에 [target]2를 [all_area]3으로 이동시킨다."	,

	"    외치기"								,eREACTION_SHOUT				,	//	외친다.
	" 다음 메시지를 [character]0가 [notice_range]1에 외친다. [text_box]2",

	"    대사"									,eREACTION_TALK					,	//	대화
	" 다음 대사를 말한다. [talk_method]0 [text_box]1 ",

	"    대사2"									,eREACTION_TALK2				,	//	대화
	" [character]0이 다음 대사를 말한다. [talk_method]1 [text_box]2	[notice_range]3",

	"    스킬 재분배"							,eREACTION_REDISTRIBUTE_SKILL						,	//	스킬 재분배
	" 스킬을 재분배 한다.",

	"    스탯 재분배"							,eREACTION_REDISTRIBUTE_STATE						,	//	스탯 재분배
	" 스탯을 재분배 한다.",

	"    능력치 재분배(결투장용)"				,eREACTION_RESET_ACTOR_IN_DUEL_SERVER				,	//	능력치 재분배(결투장용)
	" 능력치를 재분배 시킨다.",
	
	"    펫 이름 초기화"						,eREACTION_RESET_PET_NAME							,	//	펫 이름 지우기
	" [spin]0[0,1]번 펫의 이름을 초기화 한다.",

	"    펫 추가"								,eREACTION_ADD_PET			,	//	펫 이름 지우기
	" [spin]0[1,1000]레벨 [monster]1을 펫으로 추가 시킨다.",

	"    체력 회복"								,eREACTION_RECOVER_HP			,	//	체력 설정
	" [all_area]0 안에 [all_character_target_player]1의 체력을 [spin]2[1,100]％ 회복 시킨다.",

	"    CP 충전"								,eREACTION_CARGE_CP				,	//	CP 채워주기
	" [all_area]0 안에 [all_character_target_player]1의 CP를 [spin]2[1,100]％ 충전 시킨다.",

	"    대미지 입히기"							,eREACTION_DAMAGE				,	//	체력 설정
	" [all_area]0 안에 [all_character_target_player]1에게 [number]2[1,65535]~[number]3[1,65535] 포인트의 [physical_magic]4 대미지를 입힌다. [damage_method]5",

	"    시야 변경"								,eREACTION_CHANGE_SIGHT_RANGE	,
	" [all_area]0 안에 [all_character]1의 시야를 [spin]2[1,10000]으로 변경한다.",

	"    상태 이상 부여"						,eREACTION_STATE_CHANGE	,
	" [quest_target]0에게 [state]1을 [spin]2[1,10000]초 부여 한다.",

	"    블라인드(영구)"						,eREACTION_STATE_BLIND_EFFECT	,
	" [quest_target]0에게 [blind_Range]1 단계 암전효과를 더한다.(제거 하면 단계에 상관없이 제거 됩니다.)",
	
	"    전생"									,eREACTION_REBIRTH	,
	" 타겟을 전생 시킨다.(레벨초기화를 [hand_anhanda]0)",


	"    버프해제"									,eREACTION_DISPELL	,
	" [target_player]0 의 모든 버프를 해제시킨다.",

	"    몬스터 변신 해제"									,eREACTION_RELEASE_TRANSFORMATION	,
	" [target_player]0 의 몬스터 변신을 해제시킨다.",


	"■■■ 몬스터 AI,NPC 관련"					,0x7fff						,
	" 몬스터 AI와 관련된 액션들[border_comment]0"						,

	"    보유 스킬 사용"						,eREACTION_USE_ABILITY		,//	몬스터가 보유한 스킬 xxx를 사용을 시도 한다. <- 조건에 맞지 않는다면 실패
	" 스킬 [skill]0을 사용한다.",

	"    스킬 사용"								,eREACTION_USE_MONSTER_ABILITY		,//	몬스터가 스킬 xxx를 사용을 시도 한다. <- 조건에 맞지 않는다면 실패	09.08.14
	" 스킬 [skill]0을 [spin]1[1,200]Lv로 사용한다.",

	"    마법 시전"								,eREACTION_CASTING_MAGIC	,			// npc 보조마법 캐스팅.
	" [all_area]0 안에 [target_player]1에게  [skill]2을 [spin]3[1,200]Lv , 관련 능력치[spin]4[1,5000]로 [spin]5[0,20000]초간 시전한다.(0초 이면 스킬시간에 따른다)",

	"    직업 전환"								,eREACTION_TRANS_JOB		,//	몬스터 xxx로 변신 한다.
	" [character]0로 변신한다.",

	"    이동"									,eREACTION_MOVE_TO			,//	영역 x로 이동/공격/순찰 한다.
	" [all_character_target_player]0를 [area]1으로 [move_patrol]2 [move_method]3",

	"    길잡이 설정"							,eREACTION_SET_GUIDE		,//	영역 x로 이동/공격/순찰 한다.
	" [target_player]0를 길잡이로 [set_cancel]1",

	"    길잡이 따라가기"						,eREACTION_TRACE_GUIDE	,//	영역 x로 이동/공격/순찰 한다.
	" 길잡이를 따라기 [set_cancel]0",

	"    몬스터 리젠"							,eREACTION_REGEN_MONSTER		,//	영역 x에 몬스터들을 리젠 시킨다.
	" [area]0에 [all_character]1를 리젠 시킨다.",

	"    랜덤 타겟 설정"						,eREACTION_CHANGE_TARGET_BY_RANDOM	,//	현재의 공격 타겟을 해재하고 시야 내의 렌덤한 한명을 공격 타겟으로 설정한 뒤 x초 동안 해당 타겟을 유지 한다.
	" 시야내에 임의의 한명을 선택해 공격한다. [spin]0[1,30000]초 동안 타겟을 바꾸지 않는다.",
	
	"    타겟 해제"								,eREACTION_TARGET_RESET				,//	타켓에 관련된 정보를 해제한다.
	" 타켓팅 정보를 리셋한다.",

	"    전투 성향 변경"						,eREACTION_CHANGE_BATTLE_INCLINATION,//	전투 성향 변경
	" [all_area]0 안에 [all_character]1의 전투 성향을 [bi]2로 설정한다."			,

	"    상태이상 해제"							,eREACTION_RELEASE_WRONG_STATUS		,//	각종 상태이상을 해제한다.
	" [area]0 안에 [all_character]1의 이상 상태를 모두 해제한다.",

	"    액션"									,eREACTION_NPC_ACTION	,//	각종 상태이상을 해제한다.
	" [area]0 안에 [all_character]1들이 [direct]2 방향으로 액션을 한다. [action]3 [spin]4[0,30000]회 반복",

	"    무적 상태 설정"						,eREACTION_CONTROL_INVINCIBLE_STATUS,//	각종 상태이상을 해제한다.
	" [area]0 안에 [all_character]1를 [invincible_onoff]2",

	"    이름 변경"								,eREACTION_CHANGE_NAME,//	몬스터 이름 변경 
	" 몬스터 이름을 아래 이름 으로 변경 [text_box]0",

	"    이벤트 몬스터 설정"					,eREACTION_SET_EVENT_MONSTER_VALUE,//	몬스터 이름 변경 
	" 몬스터를 [event_monster]0 로 설정 ",

	"    획득경험치 증가"									,eREACTION_INCREASE_INSTANT_EXPERIENCE	,
	" 획득 경험치를  [number]0[1,255] 배 증가시킨다.",


	"■■■ 각종 연출/필드 관련 액션들"		,0x7fff						,
	" 각종 연출/필드 관련 액션들[border_comment]0"						,

	"    시스템 메시지"							,eREACTION_SYSTEM_NOTICE	,//	시스템 공지
	" 다음 메시지를 [notice_range]0 [notice_type]1 [spin]2[1,30]초간 출력한다. [effectflickeringly]3 (시간과 효과는 서버 전체에는 작동X , 전체공지에서만 쓰세요)[text_box]4 "	,
	

	"  S 던전 수치 조절"						,eREACTION_CONTROL_DUNGEON_FLAG	,//	던전 플레그를 x만큼 더한다/뺀다, x로 한다.
	" [dungeon_value]0을 [spin]1[0,30000] [add_remove_set]2",

	"    필드 수치 조절"						,eREACTION_CONTROL_FIELD_VALUE	,//	필드 수치를 x만큼 더한다/뺀다, x로 한다.
	" [field_value]0을 [spin]1[0,30000] [add_remove_set]2",

	"    월드 수치 조절"						,eREACTION_CONTROL_WORLD_VALUE	,//	월드 수치를 x만큼 더한다/뺀다, x로 한다.
	" 월드 수치[world_value]0을 [spin]1[0,30000] [add_remove_set]2",

	"  S 비밀 던전 유지 시간 연장"				,eREACTION_EXTEND_SECRET_DUNGEON_UPKEEP_TIME	,//	비밀 던전 시간 연장
	" 비밀 던전 유지 시간을 [spin]0[1,1000]분 연장 시킨다.",

	"    시간 지연"								,eREACTION_DELAY			,
	" 다음 액션까지 [spin]0[0,30000]분 [spin]1[0,59]초 [spin]2[0,999]ms를 기다린다.(현재 카르마의 이후 리액션들에게만 적용된다. 다음 카르마는 바로 작동된다.)",

	"    상자/문/함정 상태 설정"				,eREACTION_SET_ARCA_DOOR_TRAP_STATUS	,//	영역 x의 상자/문/함정 상태 설정
	" [area]0의 [arca_door_trap]1을 [arca_door_trap_status]2",

	"    문 활성 비활성"						,eREACTION_SET_DOOR_ENABLE_DISABLE	,//	영역 x의 상자/문/함정 상태 설정
	" [area]0의 문을 [enable_disable]1",


	"  S 이벤트 시작"							,eREACTION_BEGIN_EVENT	,
	" [event_begin_end]0 [battle_status]1",

	"    스킬 효과 발동"						,eREACTION_OCCUR_SKILL_EFFECT		,//	특정 지역에 스킬 발동
	" 타일 좌표 [spin]0[1,30000],[spin]1[1,30000]에 [skill]2의 [skill_effect_type]3 효과를 발동시킨다.",

	"    지역 포커스"							,eREACTION_MOVE_FOCUS				,//	포커스 이동
	" 포커스를 [spin]0[1,30000],[spin]1[1,30000]에 맞춘다. [set_reset_scroll]2 스크롤 시간 [spin]3[0,30000]/100초",

	"  S 캐릭터 포커스"							,eREACTION_FOCUS_ON_CHARACTER		,//	포커스 이동
	" 포커스를 [character]0에 맞춘다. [set_reset]1",

	"    화면 흔들기"							,eREACTION_SHAKE_SCREEN				,//	화면 흔들기
	" 타일 좌표 [spin]0[1,30000],[spin]1[1,30000]에 사정 거리 [spin]2[1,30000] Pixel안에 흔들림 [spin]3[1,10]의 지진 발생. 지속시간 [spin]4[1,30000] Mili Second",

	"    효과음 출력"							,eREACTION_PLAY_SOUND				,//	효과음 출력
	" [all_area]0 안에 플레이어에게 [sound_file]1 파일을 플레이 한다.",

	"  S 미니맵에 강조 표시"					,eREACTION_STRESS_MINIMAP			,
	" 미니맵의 [spin]0[1,30000],[spin]1[1,30000]에 강조 효과를 출력한다. [set_reset]2",


	"    패턴 호출"								,eREACTION_PLAY_PATTERN			,
	" 패턴 [pattern]0을 호출 한다.",

	"    보스 이벤트"							,eREACTION_PLAY_BOSS_EVENT			,
	" [boss_monster]0 [entry_death]1 이벤트 시작",

	"    화면 효과"								,eREACTION_SCREEN_EFFECT			,
	" [quest_target]0에게 [screen_effect]1 효과를 [spin]2[0,50000]초 간 부여",

	"    필드 저항 변경"						,eREACTION_FIX_FIELD_RESISTANCE			,
	" [resistance]0 을 [spin]1[0,200]으로 변경",

	"    기후변화"						,eREACTION_FIELD_WEATHER			,
	" 현재필드를  [spin]0[0,84000]초 동안 [weather]1 상태로 변경",		// 09.10.06 추가.. 기후변화
	"    파티원이 있는 길드 비던 입장"						,eREACTION_JOIN_GUILD_SECRET_DUNGEONS_WITH_PARTY			,
	" 파티원이 있는 길드 비던으로 입장한다.",		// 

	"    특정 좌표에 불꽃놀이 이팩트를 출력"						,eREACTION_FIRE_WORK			,
	" 시작 타일 좌표 [spin]0[0,30000],[spin]1[0,30000] 목표 타일 좌표[spin]2[0,30000],[spin]3[0,30000]에  [spin]4[0,10]번의 불꽃놀이 이팩트를 출력(0번은 랜덤)",

	"    화면 좌표에 이미지 출력"						,eREACTION_DRAW_IMAGE			,
	" interface2.sd 파일 [spin]0[0,20000] 번 인덱스를  화면 좌표 [spin]1[0,30000],[spin]2[0,30000]에  [spin]3[0,20000]초간 출력(방법 : [drawing_method]4 ,[spin]5[0,1000] 만큼 이동.)",

	"    화면 좌표에 이미지 끄기"						,eREACTION_OFF_DRAWING_IMAGE			,
	" interface2.sd 파일 [spin]0[0,20000] 번 인덱스(화면 좌표 [spin]1[0,30000],[spin]2[0,30000]) 출력된 이미지 끄기",

	"■■■ 기타"		,0x7fff						,
	" 기타[border_comment]0"						,

	" N  화면 이동 이벤트"						,eREACTION_SET_SCREEN_EVENT	,//화면 이동 연출
	"[target_player]4가 화면의 타일[spin]0[0,50000],[spin]1[0,50000](X , Y)로 [spin]2[0,50000]초간 이동하고 [spin]3[0,50000]초 유지된다[all_area]5.(파티 리더와 파티중 아무나는 미구현.)" , 

	"    오프라인 이벤트"								,eREACTION_EVENT_VALUE,//	인스턴스 던전으로 이동
	" 이벤트 [event]0 관련 수치를 증가 시킨다. 최대치[spin]1[1,30000]. 중복 수령 체크 [hand_anhanda]2",

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
