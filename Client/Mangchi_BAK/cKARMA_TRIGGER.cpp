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
	"■■■ 플레이어 자신과 관련된 사항",0x7fff													,
	" 플레이어 자신과 관련된 사항[border_comment]0"												,

	"    레벨체크"							,eTRIGGER_LEVEL_CHECK								,
	" 레벨이 [spin]0[1,1000]~[spin]1[1,1000]이다."												,

	"    길드 소속 여부"					,eTRIGGER_IS_GUILD_MEMBER							,
	" 길드에 [guild_member]0"																	,

	"    길드 직급"							,eTRIGGER_IS_RANK_IN_GUILD							,
	" 길드직급이 [rank_in_guild]0 [ida_anida]1"														,


	"    빈 아이템슬롯 의 수"				,eTRIGGER_REMAIN_ITEM_SLOT_COUNT					,
	" 남은 아이템 슬롯의 수가 [spin]0[0,255]개 [equal]1"										,

 	"    아이템 옵션 확인"					,eTRIGGER_CHECK_ITEM_BOOLEAN_OPTION		,
	" 인벤토리에 [item_boolean_option_list]0 옵션이 있는 아이템이 [exist_not_exist]1 대상[target_player]2"			,//	무료 옵션
	
	"    아이템 보유 여부"					,eTRIGGER_OWN_ITEM									,
	" [item]0의 보유 개수가 [spin]1[0,1024]개 [equal]2. [ignore_include_equipmment]3"											,

	"    특수 아이템 보유 여부"			,eTRIGGER_OWN_SPECIAL_ITEM								,
	" [special_item]0의 보유 개수가 [spin]1[0,255]개 [equal]2"									,

	"    퀘스트 아이템 보유 여부"			,eTRIGGER_OWN_QUEST_ITEM							,
	" [quest_item]0의 보유 개수가 [spin]1[0,255]개 [equal]2"									,

	"    아이템 장착 여부"					,eTRIGGER_EQUIP_ITEM								,
	" [equipment]0을 [item_equip]1"																,

	"    소지금"							,eTRIGGER_OWN_GOLD									,
	" 보유한 금액이 [number]0[0,2147483647]골드 [equal]1 [by_level]2"										,

	"    스킬 보유 여부"					,eTRIGGER_SKILL_LEVEL								,
	" 스킬 [skill]0의 레벨이 [spin]1[0,50] [equal]2"											,

	"    마스터한 스킬 수"					,eTRIGGER_MASTER_SKILL_COUNT						,
	" 마스터한 스킬의 수가 [spin]0[0,60]개 [equal]1"											,

	"    운영자 여부 체크"					,eTRIGGER_CHECK_OPERATOR							,
	" [check_operator]0"																		,

	"    칭호 보유 여부"					,eTRIGGER_TITLE_LEVEL								,
	" 칭호 [title]0의 레벨이 [spin]1[0,10] [equal]2"											,

	"    직업 체크"						,eTRIGGER_CHECK_JOB										,
	" 직업이 [player_job]0 [ida_anida]1. [include_transformation_job]2"							,

	"    전생 횟수 체크"					,eTRIGGER_CHECK_REBIRTH_COUNT						,
	" 전생 횟수가 [spin]0[0,30]회 [equal]1"							,

//	" 직업 레벨"						,eTRIGGER_CHECK_JOB_LEVEL								,
//	" 직업레벨이 [spin]0[1,3] [equal]1"															,

	"    성향"								,eTRIGGER_CHECK_ALLIGNMENT							,
	" 성향이 [allignment]0이다."																,

	"    플레이 타임"						,eTRIGGER_CHECK_PLAY_TIME							,
	" 플레이 시간이 [number]0[0,2147483647]시간 [equal]1"										,

	"    능력치"							,eTRIGGER_CHECK_BASIC_POWER							,
	" [power]0가 [spin]1[0,10000] [equal]2"														,

	"    보유한 책의 완성도 체크"	,eTRIGGER_CHECK_BOOK_COMPLETE_PERCENTAGE			,
	" [book]0의 완성도가 [spin]1[0,100]％ [equal]2(모든 책 체크 - 책이 없는 경우는 무조건 체크 실패)"			,

	"    보유한 책 체크"					,eTRIGGER_CHECK_EXIST_BOOK							,
	" [book]0이 [exist_not_exist]1"																,

	"    보유한 책에 특정 페이지 체크"	,eTRIGGER_CHECK_BOOK_PAGE					,
	" 책 [book]0에 카테고리 [spin]1[1,18]번에 페이지 [spin]2[1,8]이 [exist_not_exist]3. (모든 책 체크)"		,

	"    책에 특정 페이지 추가 하는 아이템 체크"	,eTRIGGER_CHECK_OWN_ADD_BOOK_PAGE_ITEM		,
	" 책 [book]0에 카테고리 [spin]1[1,18]번에 페이지 [spin]2[1,8]을 추가하는 아이템이 [exist_not_exist]3."		,

	"    현재 상태 체크"							,eTRIGGER_CHECK_CURRENT_STATUS		,
	" [current_status]0 [ida_anida]1"				,

	"    성별 체크"									,eTRIGGER_CHECK_SEX		,
	" 플레이어 직업 성별이 [sex]0"			,//	성별 체크

	"    출석 체크"									,eTRIGGER_CHECK_ATTENDANCE		,
	" [item]0 아이템에 오늘 출석 체크를 [quest_complete]1."			,//	출석 체크

	"    성향 체크"									,eTRIGGER_CHECK_ATTR	,
	" 플레이어의 성향이 [plus_number_minus_number]0 [spin]1[0,100] [equal]2"			,//	출석 체크

	"    챕터 5 칭호 보너스 수락 가능"				,eTRIGGER_CHECK_CAN_RECEIVE_CH5REWARD_TITLE_BONUS	,
	" 챕터 5의 칭호 보너스를 받을 수 [exist_not_exist]0"	,

	"    챕터 5 보너스 길드 포인트 획득 가능"				,eTRIGGER_CHECK_ENABLE_INCREASE_BONUS_GUILD_POINT	,
	" 챕터 5의 보너스 길드 포인트를 받을 수 [exist_not_exist]0"	,

	"    챕터 5 길드 석상 레벨 증가가 가능."			,eTRIGGER_CHECK_ENABLE_INCREASE_GUILD_STATUE_LEVEL	,
	" 챕터 5 길드 석상 레벨 증가 시킬 수 [exist_not_exist]0"	,
	
		
	"    챕터 5 천상계 보너스 받은 회수"				,eTRIGGER_CHECK_CH5_HEAVEN_BONUS_COUNT	,
	" 천상계 보너스 [heaven_bonus]0 [bonus_penelty]3를 받은 회수가 [spin]1[0,10] [equal]2 (스탯,스킬은 페널티 회수 체크 안됨)"	,

	"    챕터 5 착용 레벨 증가 부적 체크",	eTRIGGER_CHECK_SAME_LEVEL_OVER_LEVEL_EQUIP_AMULET		,//	
	" 현재 붉은 보석 사냥꾼 레벨에서 착용 레벨 증가 부적을 받은 적이 [exist_not_exist]0"	,

	"    챕터 5 체력 증가 부적 체크",	eTRIGGER_CHECK_SAME_LEVEL_INCREASE_HP_AMULET	,//	
	" 현재 붉은 보석 사냥꾼과 같은 레벨의 체력 증가 부적이  [exist_not_exist]0"	,

	"    챕터 5 CP 증가 부적 체크",	eTRIGGER_CHECK_SAME_LEVEL_INCREASE_CP_AMULET,//	
	" 현재 붉은 보석 사냥꾼과 같은 레벨의 CP 증가 부적이  [exist_not_exist]0"	,

	"    펫 추가 가능 여부 체크",	eTRIGGER_CHECK_ADD_ABLE_PET,//	
	" 펫을 추가 할수 [exist_not_exist]0"	,
	
	"    현재 펫의 수 체크",	eTRIGGER_CHECK_ACTIVE_PET_COUNT,//	
	" 현재 소환된 펫의 수가 [spin]0[0,2] [equal]1"	,

	"    결투 순위 체크",			eTRIGGER_CHECK_DUEL_RANKING,//	
	" 결투 순위가 [spin]0[1,100]등에서 [spin]1[1,100]등 사이 [ida_anida]2"	,
	
	"    은총 보유 체크",			eTRIGGER_CHECK_HAVE_GRACE,//	
	" 타케 플레이어가 은총을 보유하고 [quest_own]0"	,		//quest_own이지만 낚이지말자. "가지고 있다 없다"다.

	"    블라인드(영구) 체크",			eTRIGGER_CHECK_IS_BLIND_NEW,//	
	" 타케 플레이어가 블라인드 상태 [exist]0"	,		
	
	"    타겟 플레이어 액션 상태 체크",			eTRIGGER_CHECK_PLAYER_ACTION,//	
	" 현재 플레이어 행동이 [action]0 중 [ida_anida]1"	,	

	"    비밀던전 횟수 체크"					,eTRIGGER_CHECK_SECRET_DUNGEON_PLAY_COUNT						,
	" 비밀던전 횟수가 [spin]0[0,30]회 [equal]1"	,


	"■■■ 소속된 파티와 관련된 사항"	,0x7fff													,
	" 소속된 파티와 관련된 사항[border_comment]0"												,

	"    파티원 수"						,eTRIGGER_PARTY_MEMBER_COUNT							,
	" 파티원이 [spin]0[0,8]명 [party_member_equal]1"											,

	"    파티 리더"						,eTRIGGER_IS_PARTY_LEADER								,
	" [is_party_leader]0"																		,
	
	"    파티원의 위치"					,eTRIGGER_CHECK_PARTY_MEMBER_LOCATION					,
	" 모든 파티원이 [party_member_location]0"													,

	"    파티원 레벨 범위"					,eTRIGGER_CHECK_PARTY_MEMBER_LEVEL					,
	" 파티원의 레벨이 [spin]0[1,1000]~[spin]1[1,1000]이다."										,

	"    파티원의 길드 소속 여부"			,eTRIGGER_ALL_PARTY_MEMBER_IN_GUILD					,
	" 모든 파티원이 길드에 [guild_member]0"														,

	"    파티원의 길드 소속 여부"			,eTRIGGER_IN_GUILD_MEMBER_EXIST_IN_PARTY			,
	" 길드에 소속해 있는 파티원이 [exist_not_exist]0"											,

	"    파티원의 길드 소속 여부"			,eTRIGGER_ALL_PARTY_MEMBER_IN_SAME_GUILD			,
	" 모든 파티원이 [all_party_member_in_same_guild]0"											,

	"    파티원 아이템 보유 여부"			,eTRIGGER_ALL_PARTY_MEMBER_OWN_SPECIFIC_ITEM		,
	" 모든 파티원이 [item]0을 [spin]1[1,255]개 이상 보유하고 있다."								,

	"    파티원 아이템 보유 여부"			,eTRIGGER_ALL_PARTY_MEMBER_NOT_OWN_SPECIFIC_ITEM	,
	" 모든 파티원이 [item]0을 보유하고 있지 않다."												,

	"    파티원 아이템 보유 여부"			,eTRIGGER_OWN_SPECIFIC_ITEM_MEMBER_EXIST_IN_PARTY	,
	" [item]0을 [spin]1[1,255]개 이상 보유한 파티원이 [specific_item_own_member_exist]2"		,

	"    파티원 소지 금액의 합"			,eTRIGGER_GOLD_SUM_OF_PARTY								,
	" 파티원의 소지 금액 합이 [number]0[1,2147483647]골드 [equal]1"								,

	"    파티원 소지 금액"					,eTRIGGER_CHECK_GOLD_IN_PARTY_MEMBER				,
	" [number]0[1,2147483647]골드 이상 소지하고 있는 파티원이 [exist]1"							,

	"    모든 파티원의 소지 금액"			,eTRIGGER_CHECK_GOLD_IN_ALL_PARTY_MEMBER			,
	" 모든 파티원이 [number]0[1,2147483647]골드 이상 소지하고 있다."							,

	"    파티원 스킬 보유 여부"			,eTRIGGER_CHECK_SKILL_IN_PARTY_MEMBER					,
	" 레벨 [spin]0[0,50]이상의 [skill]1이 있는 파티원이 [exist]2"								,

	"    모든 파티원 스킬 보유 여부"		,eTRIGGER_ALL_PARTY_MEMBER_LEARNED_SSPECIFIC_SKILL	,
	" 모든 파티원이 레벨 [spin]0[0,50]이상의 [skill]1을 익히고 있다."							,

	"    파티원의 칭호 보유 여부"			,eTRIGGER_CHECK_TITLE_IN_PARTY_MEMBER				,
	" [spin]0[1,10]레벨 이상의 [title]1 칭호를 가진 파티원이 [exist]2"							,

	"    모든 파티원의 칭호 보유 여부"		,eTRIGGER_OWNED_TITLE_BY_ALL_PATY_MEMBER			,
	" 모든 파티원이 [spin]0[1,10]레벨 이상의 [title]1 칭호를 [own]2"							,

	"    파티원의 직업"					,eTRIGGER_CHECK_JOB_IN_PARTY							,
	" 직업이 [player_job]0인 파티원이 [exist]1. [include_transformation_job]2"					,

//	"    파티원들의 보유 직업 레벨"		,eTRIGGER_CHECK_ALL_PARTY_MEMBER_JOB					,
//	" 직업레벨이 [spin]0[1,3]인 파티원이 [exist]1"												,

	"    모든 파티원의 직업"				,eTRIGGER_CHECK_JOB_LEVEL_IN_ALL_PARTY_MEMBER		,
	" 모든 파티원의 직업이 [player_job]0이다. [include_transformation_job]1"					,

	"    파티원들의 성향"					,eTRIGGER_CHECK_ALLIGNMENT_IN_PARTY_MEMBER			,
	" 성향이 [allignment]0인 파티원이 [exist]1"													,

	"    모든 파티원들의 성향"				,eTRIGGER_CHECK_ALLIGNMENT_IN_ALL_PARTY_MEMBER		,
	" 모든 파티원의 성향이 [allignment]0이다."													,

	"    파티원의 능력치"					,eTRIGGER_CHECK_BASKC_POWER_IN_PARTY_MEMBER			,
	" [power]0가 [spin]1[1,10000]이상인 파티원이 [exist]2"										,

	"    모든 파티원의 능력치"				,eTRIGGER_CHECK_BASKC_POWER_IN_ALL_PARTY_MEMBER		,
	" 모든 파티원의 [power]0가 [spin]1[1,10000] [equal]2"										,

	"    일부 멤버의 마스터한 스킬의 수 체크",eTRIGGER_HAVE_MASTER_SKILL_PARTY_MEMBER			,
	" 스킬을 마스터한 수가 [spin]0[0,60]개 이상인 파티 멤버의 수가 [spin]1[0,8]명 [equal]2"		,

	"    모든 멤버의 마스터한 스킬 수 체크",eTRIGGERCHECK_ALL_PARTY_MEMBERS_MASTER_SKILL_COUNT	,
	" 모든 파티원이 [spin]0[0,60]개 [only_equal]1의 스킬을 마스터 했다."						,

	"  S 파티원 전멸"						,eTRIGGER_CHECK_ALL_PARTY_MEMBERS_LIFE				,
	" 비던에 생존해 있는 파티원이 없다.(비던 전용 트리거)"										,

	"    파티원 성별 체크"					,eTRIGGER_CHECK_MEMBER_SEX							,//	멤버의 성별 체크
	" 직업이 [sex]0인 파티원이 [spin]1[0,8]명 [equal]2",

	"    파티원 최고,최저 레벨 체크"		,eTRIGGER_CHECK_LEVEL_RANGE							,//	최고 레벨과 최저 레벨의 차이
	" 파티에 최고 레벨 파티원과 최저 레벨 파티원의 레벨 차이가[spin]0[0,1000] [equal]1",

	"    현재 필드 파티원 수"						,eTRIGGER_PARTY_MEMBER_COUNT_IN_CURRENT_FIELD					,	
	" 현재 필드에 있는 파티원이 [spin]0[0,8]명 [equal]1"	,	

	"■■■ 소속 길드와 관련된 사항"		,0x7fff													,
	" 소속 길드와 관련된 사항[border_comment]0"													,

	"    소속 길드 레벨"					,eTRIGGER_CHECK_ATTACED_GUILD_LEVEL						,
	" 소속 길드의 레벨이 [spin]0[1,10000] [equal]1"												,

	"    소속 길드 길드원 수"				,eTRIGGER_CHECK_MEMBER_COUNT_IN_ATTACED_GUILD			,
	" 소속 길드의 길드원 수가 [spin]0[1,10000] [equal]1"										,

	"    소속 길드 서브 마스터 수"			,eTRIGGER_CHECK_SUB_GUILD_MASTER_COUNT_IN_ATTACED_GUILD	,
	" 소속 길드의 서브 마스터의 수가 [spin]0[1,10000] [equal]1"									,

	"    소속 길드 원로 길드원 수"			,eTRIGGER_CHECK_ELDER_MEMEBER_COUNT_IN_ATTACED_GUILD	,
	" 소속 길드의 원로 길드원의 수가 [spin]0[1,10000] [equal]1"									,

	"    소속 길드 보유 아이템"				,eTRIGGER_CHECK_OWN_GUILD_ITEM						,
	" 소속 길드에서 [guild_item]0을 [item_own]1"												,

	"    소속 길드 보유 금액"				,eTRIGGER_CHECK_GOLD_OF_ATTACED_GUILD					,
	" 소속 길드에 보유 금액이 [number]0[1,2147483647]골드 [equal]1"								,

	"    소속 길드 생성 경과 시간"			,eTRIGGER_CHECK_GENERATE_TIME_OF_ATTACED_GUILD			,
	" 소속 길드가 만들어진후 경과한 시간이 [number]0[1,2147483647]일 [equal]1"					,

	"    소속 길드 랭킹"					,eTRIGGER_CHECK_RANKING_OF_ATTACED_GUILD				,
	" 소속 길드의 랭킹이 [spin]0[1,10000] [equal]1"												,

	"    길드 연고지 체크"					,eTRIGGER_CHECK_GUILD_PLACE_OF_RELATED				,//	길드 연고지 체크
	" 필드 [related_place_of_guild]0가 연고지 [ida_anida]1",

	"    길드홀 소유 체크"					,eTRIGGER_CHECK_IS_OWN_GUILD_HALL					,//	이게 우리 길드홀이냐?
	"  현재 필드가 소속 길드의 길드홀 [ida_anida]0",

	"    길드홀 레벨 체크"					,eTRIGGER_CHECK_GUILD_HALL_LEVEL					,//	이게 우리 길드홀이냐?
	" 소속 길드의 길드홀 레벨이 [spin]0[0,5] [equal]1",

	"    길드 석상 스킬 레벨 체크"			,eTRIGGER_CHECK_GUILD_STATUE_SKILL_LEVEL				,//	길드 석상 스킬 레벨 체크
	" 소속 길드의 길드 석상 스킬 [guild_statue_skill]0의 레벨이 [spin]1[0,5] [equal]2",

	"    길드 마스터의 레벨"				,eTRIGGER_CHECK_GUILD_MASTER_LEVEL						,
	" 길드 마스터의 레벨이 [spin]0[1,10000] [equal]1"											,

	"    길드 마스터의 직업"				,eTRIGGER_CHECK_GUILD_MASTER_JOB						,
	" 길드 마스터의 직업이 [hero_job]0이다."													,

	"    길드 마스터의 직업레벨"			,eTRIGGER_CHECK_GUILD_MASTER_JOB_LEVEL					,
	" 길드 마스터의 직업레벨이 [spin]0[1,3] [equal]1"											,

	"    길드 마스터의 성향"				,eTRIGGER_CHECK_GUILD_MASTER_ALLIGNMENT					,
	" 길드 마스터의 성향이 [allignment]0 이다."													,

	"    길드 마스터의 능력치"				,eTRIGGER_CHECK_GUILD_MASTER_BASIC_POWER				,
	" 길드 마스터의 [power]0이 [spin]1[1,10000] [equal]2"										,

	"    챕터 5 보너스 길드 포인트 페널티 체크",	eTRIGGER_CHECK_GUILD_SKILL_BONUS_PENELTY			,//	
	" 챕터 5 보너스 길드 포인트 페널티가 [exist_not_exist]0"	,

	"    길드 로엔의 호감도 체크",					eTRIGGER_CHECK_GUILD_ROEN_GOOD_WILL_CHECK			,//	
	" 로엔의 호감도가 [spin]0[0,6] [equal]1"	,

	"■■■ 퀘스트 관련"				,0x7fff													,
	" 퀘스트와 관련된 사항[border_comment]0"													,

	"    퀘스트 완료했다."					,eTRIGGER_CHECK_COMPLETE_QUEST							,
	" 퀘스트 [quest]0을 [quest_complete]1 체크 대상 [quest_target]2"							,

	"    퀘스트 완료 체커"					,eTRIGGER_CHECK_QUEST_COMPLETE_CHECKER					,
	" 퀘스트 [quest]0의 [quest_complete_checkr]1 체크 대상 [quest_target]2"							,

	"    퀘스트 카운트다운 타이머 체크"		,eTRIGGER_CHECK_QUEST_COUNTDOWN_CHECKER					,
	" 퀘스트 [quest]0의 카운트다운 타이머가 [quest_countdown_checker]1. 체크 대상 [quest_target]2"				,

	"    퀘스트 진행 여부"						,eTRIGGER_CHECK_OWN_QUEST								,
	" 퀘스트 [quest]0을 [quest_own]1 체크 대상 [quest_target]2"															,

	"    퀘스트 진행 상태"						,eTRIGGER_CHECK_QUEST_PROCESS_LEVEL						,
	" 퀘스트 [quest]0의 진행 상태가 [quest_process_level]1 이다. 체크 대상 [quest_target]2"								,

	"    퀘스트 수치"							,eTRIGGER_CHECK_QUEST_VALUE								,
	" 퀘스트 [quest]0의 수치가 [spin]1[0,20000] [equal]2 체크 대상 [quest_target]3"										,

	"    퀘스트 시작 가능"					,eTRIGGER_CHECK_ENABLE_QUEST		,
	" [quest_target]0가 [quest]1을 [quest_able]2"			,
	
	"    퀘스트 관련 직업 체크"			,eTRIGGER_CHECK_PLAYER_JOB_BY_QUEST										,
	" [quest]3 - 직업이 [quest_job]0 [ida_anida]1. [include_transformation_job]2"		,


	"    아르바이트를 완료했다."				,eTRIGGER_CHECK_COMPLETE_ARBEIT	,
	" 아르바이트 [quest_arbeit]0을 [quest_complete]1 체크 대상 [quest_target]2"														,

	"    아르바이트 완료 체커"					,eTRIGGER_CHECK_ARBEIT_COMPLETE_CHECKER					,
	" 아르바이트 [quest_arbeit]0의 [quest_complete_checkr]1 체크 대상 [quest_target]2"														,

	"    아르바이트 카운트다운 타이머 체크"					,eTRIGGER_CHECK_ARBEIT_COUNTDOWN_CHECKER				,
	" 아르바이트 [quest_arbeit]0의 카운트다운 타이머가 [quest_countdown_checker]1. 체크 대상 [quest_target]2"				,

	"    아르바이트 진행 여부"					,eTRIGGER_CHECK_OWN_ARBEIT								,
	" 아르바이트 [quest_arbeit]0을 [quest_own]1 체크 대상 [quest_target]2"												,

	"    아르바이트 진행 상태"					,eTRIGGER_CHECK_ARBEIT_PROCESS_LEVEL					,
	" 아르바이트 [quest_arbeit]0의 진행 상태가 [quest_process_level]1 이다. 체크 대상 [quest_target]2"					,

	"    아르바이트 수치"						,eTRIGGER_CHECK_ARBEIT_VALUE							,
	" 아르바이트 [quest_arbeit]0의 수치가 [spin]1[0,10000] [equal]2 체크 대상 [quest_target]3"							,

	"    진행중인 퀘스트와 아르바이트의 수"	,eTRIGGER_CHECK_PROCESS_QUEST_COUNT						,
	" 진행중인 퀘스트(아르바이트 포함)의 수가 [spin]0[0,50] [equal]1 체크 대상 [quest_target]2 [quest_kind]3",

	"    새로운 퀘스트 추가 가능 여부"			,eTRIGGER_REMAIN_PROCESS_QUEST_SLOT			,
	" 새로운 퀘스트를 추가 시킬 수 [exist]0 체크 대상 [quest_target]1"				,

	"    아르바이트 시작 가능"						,eTRIGGER_CHECK_ENABLE_ARBEIT		,
	" [quest_target]0가 [quest_arbeit]1을 [quest_able]2"			,

	"    아르바이트 관련 직업 체크"				,eTRIGGER_CHECK_PLAYER_JOB_BY_ARBEIT										,
	" [quest_arbeit]3 - 직업이 [arbeit_job]0 [ida_anida]1. [include_transformation_job]2"		,

	"    메인 퀘스트 수치 체크"							,eTRIGGER_CHECK_MAIN_QUEST_VALUE			,
	" 메인 퀘스트 수치 [main_quest_value]0의 값이 [number]1[0,65535] [equal]2"		,

	"    메인 퀘스트 스위치 체크"							,eTRIGGER_CHECK_MAIN_QUEST_SWITCH			,
	" 메인 퀘스트 스위치 [main_quest_switch]0가 [check_on_off]1"		,



	"■■■ 이벤트/몬스터 관련"						,0x7fff			,
	" 이벤트/몬스터 관련 트리거 [border_comment]0"					,

	"    주변의 적의 수 체크 "				,eTRIGGER_CHECK_ENEMY_COUNT_IN_RANGE		,//	주변 x 픽셀 내의 플레이어의 수가 x명 이상/이하이다.
	" 중심에서 [spin]0[1,20000] 픽셸안에 적의 수가 [spin]1[0,1000]명 [equal]2",

	"    체력 체크 "						,eTRIGGER_CHECK_HP_PERCENTAGE				,//	자신의 hp가 x% 이상/이하이다.
	" 체력이 [spin]0[0,100]％ [equal]1 대상 [target_owner]2",

	"    던젼 수치 체크"					,eTRIGGER_CHECK_DUNGEON_FLAG			,//	던전 플래그 y가 x 이상/이하이다.
	" [dungeon_value]0이 [spin]1[0,20000] [equal]2",

	"    필드 수치 체크"					,eTRIGGER_CHECK_FIELD_VALUE	,//	필드 수치 y가 x 이상/이하이다.
	" [field_value]0이 [spin]1[0,20000] [equal]2",
	
	"    월드 수치 체크"					,eTRIGGER_CHECK_WORLD_VALUE	,//	필드 수치 y가 x 이상/이하이다.
	" [world_value]0이 [spin]1[0,128] [equal]2",

	"  S 인스턴스 던젼 플래이 타임"			,eTRIGGER_CHECK_PLAY_TIME_IN_THIS_DUNGEON	,//	현재 파티가 이 던전에 들어온지 x분 이상/이하이다.
	" 인스턴스 던젼 생성된 시간이 [spin]0[0,20000]분 [equal]1",

	"    영역 체크"							,eTRIGGER_CHECK_ARRIVE_AREA				,//	영역 x에 도착 했다.
	" [all_character_target_player2]0이 [area]1 [area_in_out]2",

	"    아이템 사용 체크"					,eTRIGGER_CHECK_TARGET_USE_BELT_ITEM	,
	" [quest_target]0이 아이템을 사용했다.",

	"    타겟 도망중"						,eTRIGGER_CHECK_TARGET_FLEE				,//	타겟이 x초 이상 자신에게 공격 받고 있지 않다.(현재 전투중인 타겟)
	" 타겟이 [spin]0[0,20000]초 이상 공격 받지 않고 있다.",

	"  S 스킬 사용 체크"		,eTRIGGER_CHECK_USE_SKILL				,//	특정 스킬을 사용했다.
	" [quest_target]0가 [area]1 안에서 [skill]2를 사용해다.",

	"    몬스터 레벨 체크"		,eTRIGGER_CHECK_MONSTER_LEVEL			,//	몬스터 레벨 체크
	" 몬스터의 레벨이 플레이어 레벨 - [spin]0[0,20000] [equal]1",

	"    이벤트 몬스터 체크"		,eTRIGGER_CHECK_MONSTER_EVENT_VALUE		,//	몬스터 이벤트 설정
	" 몬스터가 [event_monster]0 [ida_anida]1",


	"■■■ 기타"						,0x7fff													,
	" 기타[border_comment]0"																	,

	"    OX 이벤트 시작 시간 체크"			,eTRIGGER_CHECK_OX_QUIZ_START_TIME						,
	" OX 이벤트를 시작하기 [spin]0[0,10000]분 전이다.",

	"    낱말 퀴즈 이벤트 시작 시간 체크"	,eTRIGGER_CHECK_WORD_QUIZ_START_TIME						,
	" 낱말 퀴즈 이벤트를 시작하기 [spin]0[0,10000]분 전이다.",

	"    길잡이 체크"						,eTRIGGER_CHECK_GUIDE								,	
	" 길잡이가 [setting_status]0",

	"  S 카운트 다운 타이머 체크"			,eTRIGGER_CHECK_COUNTDOWN_TIMER						,
	" 카운트 다운 타이머의 시간이 [spin]0[0,60]시간 [spin]1[0,60]분 [spin]2[0,60]초 [countdown_timer_status]3", 

	"    이벤트 시간 체크"					,eTRIGGER_CHECK_EVENT_TIME							,
	" 현재 시간이 [event_time_name]0 이벤트가 진행 중인 시간[ida_anida]1",

	"    시간 체크"						,eTRIGGER_CHECK_TIME									,
	" [spin]0[2004,3000]년 [spin]1[1,12]월 [spin]2[1,31]일 [spin]3[1,24]시 부터 [spin]4[1,10000]시간 사이이다.",

	"    시간 체크 2"						,eTRIGGER_CHECK_TIME2									,
	" [spin]0[2004,3000]년 [spin]1[1,12]월 [spin]2[1,31]일 [spin]3[1,24]시가 [time_check]4",

	"    일일 시간 체크"					,eTRIGGER_CHECK_TODAY_TIME								,
	" 현재 시간이 [spin]0[0,23]시 [spin]1[0,59]분 부터 [spin]2[0,23]시 [spin]3[0,59]분 사이이다.",

	"    테스트 서버 체크"					,eTRIGGER_CHECK_IS_TEST_SERVER								,
	" 현재 서버가 [test_normal]0 서버 이다.",

	"    길드 체크"							,eTRIGGER_CHECK_IS_FULL_GUILD								,
	" 이 서버에 길드를 생성 [able_disable]0 ",

	"    오브젝트 매크로 체크"							,eTRIGGER_CHECK_BLOCK_TO_OBJECT_MACRO	,
	" 매크로를 이용해 오브젝트에 클릭하고 있지 않다.",

	"    확률 체크"							,eTRIGGER_CHECK_CHANCE	,
	" [spin]0[1,100]％ 확률 체크",

	"    확률 체크2"							,eTRIGGER_CHECK_CHANCE2	,
	" [spin]0[0,50000] / 50000 확률 체크",


	"    국가 체크"							,eTRIGGER_CHECK_LOCAL	,
	" 서비스 국가가 [local]0 [ida_anida]1",

	"    타임 스탬프 체크"					,eTRIGGER_CHECK_ITEM_TIME_STAMP	,
	" [item]0 에 타임 스탬프를 찍은지 [spin]1[0,365]일 [spin]2[0,24] 시간이 [jina_anjina]3 (시간이 0 이면 타임 스탬프 안 찍음)",

	"    이벤트 수치 체크"					,eTRIGGER_CHECK_EVENT_VALUE	,
	" 이벤트 [event]0 수치가 [spin]1[0,30000] [equal]2.",

	"    시즌 변수 체크"					,eTRIGGER_SEASON_VALIABLE	,
	" 시즌 변수가 [Season_Valiable]0 [equal]1 .",

	"    시간 체크 3"						,eTRIGGER_CHECK_TIME3									,
	" [spin]1[1,12]월 [spin]2[1,31]일 [spin]3[1,24]시 부터 [spin]4[1,10000]시간 사이이다.",//0번은 무시한다. 구조 유지를 위해.
	
	"    시간 체크 4"						,eTRIGGER_CHECK_TIME4									,
	" [spin]1[1,12]월 [spin]2[1,31]일 [spin]3[1,24]시가 [time_check]4",//0번은 무시한다. 구조 유지를 위해.

	"    분 체크 "						,eTRIGGER_CHECK_MINUTE									,
	" [spin]0[0,59]분 ~ [spin]1[0,59]분 사이이다.",//
	
	"    요일 체크"							,eTRIGGER_CHECK_WEEKDAY									,
	" [weekday]0요일 [ida_anida]1",

	"    이벤트 당첨자 체크"				,eTRIGGER_CHECK_PRIZE_PLAYER							,
	" 타겟 플레이어가 이벤트 당첨자 리스트에 [exist]0",

	"    타일셋 체크"				,eTRIGGER_CHECK_TILESET							,
	" 대상이 현재 위치한 필드의 타일셋이 [tile_set]0  [ida_anida]1",

	"    몬스터변신중체크"				,eTRIGGER_CHECK_TRANSFORMATION_MONSTER							,
	" 대상이 몬스터로 변신 중 [ida_anida]0",

	"    이벤트 남은 시간 체크"					,eTRIGGER_CHECK_EVENT_COUNT_THE_HOURS							,
	" 남은 [event_time_name]0 이벤트 시간이[spin]1[1,30000]  [equal]2",

	"    이벤트 시간 전 후 체크"					,eTRIGGER_CHECK_EVENT_TIME_BEFORE_AFTER							,
	" [event_time_name]0 이벤트 시간이  [jina_anjina]1",	
	
	"end"								,0xffff
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

	SetWindowText(hDlg,_ms("%s 그룹에 카르마 %s의 %d번째 트리거",g_strGroupName,g_strKarmaName,g_iEditTrigger));

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