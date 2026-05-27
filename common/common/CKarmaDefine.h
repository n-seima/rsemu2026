#ifndef _classKARMA_DEFINE_H
#define	_classKARMA_DEFINE_H

//////////////////////////////////////////////////////////////////////////////////////////
//	트리거들
//////////////////////////////////////////////////////////////////////////////////////////

//
//	Reaction Damage
enum
{
	eRD_PHYSICAL,
	eRD_MAGIC
};



enum
{
//	플레이어 자신과 관련된 사항
	eTRIGGER_ABOUT_PLAYER		=	0					,
	eTRIGGER_LEVEL_CHECK								,//	레벨이 [spin]0[1,1000]~[spin]1[1,1000]이다.
	eTRIGGER_IS_GUILD_MEMBER							,//	길드에 [guild_member]
	eTRIGGER_OWN_ITEM									,//	[item]0의 보유 갯수가 [spin]1[0,255]개 [equal]2
	eTRIGGER_OWN_SPECIAL_ITEM							,//	[item]0의 보유 갯수가 [spin]1[0,255]개 [equal]2
	eTRIGGER_EQUIP_ITEM									,//	[item]0을 [item_equip]
	eTRIGGER_OWN_GOLD									,//	보유한 금액이 [number]0[0,2147483647]골드 [equal]1
	eTRIGGER_SKILL_LEVEL								,//	스킬 [skill]0의 레벨이 [spin]1[0,50] [equal]2
	eTRIGGER_TITLE_LEVEL								,//	칭호 [title]0의 레벨이 [spin]1[0,10] [equal]2
	eTRIGGER_CHECK_JOB									,//	직업 [hero_job]0을 [hero_job_own]
	eTRIGGER_CHECK_JOB_LEVEL							,//	직업레벨이 [spin]0[1,3] [equal]1
	eTRIGGER_CHECK_ALLIGNMENT							,//	성향이 [allignment]0이다.
	eTRIGGER_CHECK_PLAY_TIME							,//	플레이 시간이 [number]0[0,2147483647]시간 [equal]1
	eTRIGGER_CHECK_BASIC_POWER							,//	[power]0가 [spin]1[1,10000] [equal]2
	eTRIGGER_CHECK_GUILD_TITLE							,//	길드안에서의 직위가 [guild_title]0 이다.
	eTRIGGER_OWN_QUEST_ITEM								,//	[quest_item]0의 보유 갯수가 [spin]1[0,255]개 [equal]2
	eTRIGGER_REMAIN_ITEM_SLOT_COUNT						,//	남은 아이템 슬롯의 수가 [spin]1[0,255]개 [equal]2
	eTRIGGER_MASTER_SKILL_COUNT							,//	마스터한 스킬의 수가 [spin]0[0,52]개 [equal]1
	eTRIGGER_CHECK_OPERATOR								,//	운영자냐? 
	eTRIGGER_CHECK_BOOK_COMPLETE_PERCENTAGE				,//	책 완성도 체크
	eTRIGGER_CHECK_EXIST_BOOK							,//	책 보유 유/무 체크
	eTRIGGER_CHECK_BOOK_PAGE							,//	책 페이지 체크
	eTRIGGER_CHECK_OWN_ADD_BOOK_PAGE_ITEM				,//	책 페이지 추가 아이템이 있는지 체크
	eTRIGGER_CHECK_CURRENT_STATUS						,//	현재 상태
	eTRIGGER_IS_RANK_IN_GUILD							,//	길드 안에서 직급

	eTRIGGER_CHECK_SEX									,//	성별 체크
	eTRIGGER_CHECK_ATTENDANCE							,//	출석 체크
	eTRIGGER_CHECK_ATTR									,//	성향 체크
	eTRIGGER_CHECK_CAN_RECEIVE_CH5REWARD_TITLE_BONUS	,//	CH5의 칭호 보너스를 받을 수 있다.
	eTRIGGER_CHECK_CH5_HEAVEN_BONUS_COUNT				,//	CH5의 천상계 보너스 받은 회수
	eTRIGGER_CHECK_ENABLE_INCREASE_BONUS_GUILD_POINT	,//	보너스 길드 포인트 증가 가능
	eTRIGGER_CHECK_ENABLE_INCREASE_GUILD_STATUE_LEVEL	,//	길드 석상 레벨 증가 가능
	eTRIGGER_CHECK_GUILD_SKILL_BONUS_PENELTY			,//	
	eTRIGGER_CHECK_SAME_LEVEL_OVER_LEVEL_EQUIP_AMULET	,//	
	eTRIGGER_CHECK_SAME_LEVEL_INCREASE_HP_AMULET		,//	
	eTRIGGER_CHECK_SAME_LEVEL_INCREASE_CP_AMULET		,//	

	eTRIGGER_CHECK_ITEM_TIME_STAMP						,	//	
	eTRIGGER_CHECK_ADD_ABLE_PET							,	//	펫 추가 체크
	eTRIGGER_CHECK_ITEM_BOOLEAN_OPTION					,	//	아이템에 설정되어 있는 불린 옵션 체크
	eTRIGGER_CHECK_GUILD_ROEN_GOOD_WILL_CHECK			,	//	로엔의 호감도 체크
	eTRIGGER_CHECK_REBIRTH_COUNT						,	//	전생 횟수 체크
	eTRIGGER_CHECK_ACTIVE_PET_COUNT						,	//	현재 펫의 수 체크
	eTRIGGER_CHECK_PLAYER_ACTION						,	//	현재 플레이어 행동이 [action]0 중 [ida_anida]1
	eTRIGGER_CHECK_SECRET_DUNGEON_PLAY_COUNT			,	//	비밀던전 횟수 체크
//	소속된 파티와 관련된 사항
	eTRIGGER_ABOUT_PARTY		=	100					,
	eTRIGGER_PARTY_MEMBER_COUNT							,//	파티원이 [spin]0[1,8]명 [equal]1
	eTRIGGER_CHECK_PARTY_MEMBER_LEVEL					,//	파티원의 레벨이 [spin]0[1,1000]~[spin]1[1,1000]이다.
	eTRIGGER_ALL_PARTY_MEMBER_IN_GUILD					,//	모든 파티원이 길드에 [guild_member]
	eTRIGGER_IN_GUILD_MEMBER_EXIST_IN_PARTY				,//	길드에 소속해 있는 파티원이 [party_guild_member_exist]
	eTRIGGER_ALL_PARTY_MEMBER_IN_SAME_GUILD				,//	모든 파티원이 [all_party_member_in_same_guild]
	eTRIGGER_ALL_PARTY_MEMBER_OWN_SPECIFIC_ITEM			,//	모든 파티원이 [item]0을 [spin]1[0,255]개 이상 보유하고 있다.
	eTRIGGER_ALL_PARTY_MEMBER_NOT_OWN_SPECIFIC_ITEM		,//	모든 파티원이 [item]0을 보유하고 있지 않다.
	eTRIGGER_OWN_SPECIFIC_ITEM_MEMBER_EXIST_IN_PARTY	,//	[item]0을 [spin]1[1,255]개 이상 보유한 파티원이 [specific_item_own_member_exist]
	eTRIGGER_GOLD_SUM_OF_PARTY							,//	파티원의 소지 금액 합이 [number]0[1,2147483647]골드 [equal]1
	eTRIGGER_CHECK_GOLD_IN_PARTY_MEMBER					,//	[number]0[1,2147483647]골드 이상 소지하고 있는 파티원이 [exist]
	eTRIGGER_CHECK_GOLD_IN_ALL_PARTY_MEMBER				,//	모든 파티원이 [number]0[1,2147483647]골드 이상 소지하고 있다.
	eTRIGGER_CHECK_SKILL_IN_PARTY_MEMBER				,//	레벨 [spin]0[0,50]이상의 [skill]1이 있는 파티원이 [exist]
	eTRIGGER_ALL_PARTY_MEMBER_LEARNED_SSPECIFIC_SKILL	,//	모든 파티원이 레벨 [spin]0[0,50]이상의 [skill]1을 익히고 있다.
	eTRIGGER_CHECK_TITLE_IN_PARTY_MEMBER				,//	칭호 [title]0을 보유한 파티원이 [exist]
	eTRIGGER_OWNED_TITLE_BY_ALL_PATY_MEMBER				,//	모든 파티원이 칭호 [title]0을 [own]
	eTRIGGER_CHECK_JOB_IN_PARTY							,//	직업이 [hero_job]0인 파티원이 [exist]
	eTRIGGER_CHECK_ALL_PARTY_MEMBER_JOB					,//	모든 파티원의 직업이 [player_job]이다.
	eTRIGGER_CHECK_JOB_LEVEL_IN_ALL_PARTY_MEMBER		,//	모든 파티원의 직업레벨이 [spin]0[1,3]이다.
	eTRIGGER_CHECK_ALLIGNMENT_IN_PARTY_MEMBER			,//	성향이 [allignment]0인 파티원이 [exist]
	eTRIGGER_CHECK_ALLIGNMENT_IN_ALL_PARTY_MEMBER		,//	모든 파티원의 성향이 [allignment]0이다.
	eTRIGGER_CHECK_BASKC_POWER_IN_PARTY_MEMBER			,//	[power]0가 [spin]1[1,10000]이상인 파티원이 [exist]
	eTRIGGER_CHECK_BASKC_POWER_IN_ALL_PARTY_MEMBER		,//	모든 파티원의 [power]0가 [spin]1[1,10000] [equal]2
	eTRIGGER_CHECK_PARTY_MEMBER_LOCATION				,//	모든 파티원이 [guild_member_location]
	eTRIGGER_IS_PARTY_LEADER							,//	[파티 리더이다.]
	eTRIGGER_HAVE_MASTER_SKILL_PARTY_MEMBER				,//	스킬을 마스터한 수가 [spin]0[0,60]개 이상인 파티 멤버의 수가 [spin]1[0,8]명 [equal]2
	eTRIGGERCHECK_ALL_PARTY_MEMBERS_MASTER_SKILL_COUNT	,//	모든 파티원이 [spin]0[0,60]개 [only_equal]1의 스킬을 마스터 했다.
	eTRIGGER_CHECK_ALL_PARTY_MEMBERS_LIFE				,

	eTRIGGER_CHECK_MEMBER_SEX							,//	멤버의 성별 체크
	eTRIGGER_CHECK_LEVEL_RANGE							,//	최고 레벨과 최저 레벨의 차이


	eTRIGGER_CHECK_DUEL_RANKING							,//	결투 랭킹 체크
	eTRIGGER_CHECK_HAVE_GRACE							,// 은총 보유 여부 체크.
	eTRIGGER_CHECK_IS_BLIND_NEW							,// 영구 블라인드 상태이다.

	eTRIGGER_PARTY_MEMBER_COUNT_IN_CURRENT_FIELD		,	// 현재 필드 파티원 수 

//	소속 길드와 관련된 사항
	eTRIGGER_ABOUT_GUILD		=	200					,
	eTRIGGER_CHECK_ATTACED_GUILD_LEVEL					,//	소속 길드의 레벨이 [spin]0[1,10000] [equal]1
	eTRIGGER_CHECK_MEMBER_COUNT_IN_ATTACED_GUILD		,//	소속 길드의 길드원 수가 [spin]0[1,10000] [equal]1
	eTRIGGER_CHECK_SUB_GUILD_MASTER_COUNT_IN_ATTACED_GUILD,//	소속 길드의 서브 마스터의 수가 [spin]0[1,10000] [equal]1
	eTRIGGER_CHECK_ELDER_MEMEBER_COUNT_IN_ATTACED_GUILD	,//	소속 길드의 원로 길드원의 수가 [spin]0[1,10000] [equal]1
	eTRIGGER_CHECK_OWN_GUILD_ITEM						,//	소속 길드에서 [guild_item]0을 [item_own]
	eTRIGGER_CHECK_GOLD_OF_ATTACED_GUILD				,//	소속 길드에 보유 금액이 [number]0[1,2147483647]골드 [equal]1
	eTRIGGER_CHECK_GENERATE_TIME_OF_ATTACED_GUILD		,//	소속 길드가 만들어진후 경과한 시간이 [number]0[1,2147483647]일 [equal]1
	eTRIGGER_CHECK_RANKING_OF_ATTACED_GUILD				,//	소속 길드의 랭킹이 [spin]0[1,10000] [equal]1
	eTRIGGER_CHECK_GUILD_MASTER_LEVEL					,//	길드 마스터의 레벨이 [spin]0[1,10000] [equal]1
	eTRIGGER_CHECK_GUILD_MASTER_JOB						,//	길드 마스터의 직업이 [hero_job]0이다.
	eTRIGGER_CHECK_GUILD_MASTER_JOB_LEVEL				,//	길드 마스터의 직업레벨이 [spin]0[1,3] [equal]1
	eTRIGGER_CHECK_GUILD_MASTER_ALLIGNMENT				,//	길드 마스터의 성향이 [allignment]0 이다.
	eTRIGGER_CHECK_GUILD_MASTER_BASIC_POWER				,//	길드 마스터의 [power]0이 [spin]1[1,10000] [equal]2
	eTRIGGER_CHECK_GUILD_STATUE_SKILL_LEVEL				,//	길드 석상 스킬 레벨 체크
	eTRIGGER_CHECK_GUILD_PLACE_OF_RELATED				,//	길드 연고지 체크
	eTRIGGER_CHECK_IS_OWN_GUILD_HALL					,//	이게 우리 길드홀이냐?
	eTRIGGER_CHECK_GUILD_HALL_LEVEL						,//	길드홀을 소유하고 있냐?

//	퀘스트와 관련된 사항
	eTRIGGER_ABOUT_QUEST		=	300					,
	eTRIGGER_CHECK_COMPLETE_QUEST						,//	퀘스트 [string]을 [quest_complete]
	eTRIGGER_CHECK_OWN_QUEST							,//	퀘스트 [string]을 [quest_own]
	eTRIGGER_CHECK_PROCESS_QUEST_COUNT					,//	진행중인 퀘스트의 수
	eTRIGGER_CHECK_QUEST_PROCESS_LEVEL					,// 퀘스트 진행 상태"					,
	eTRIGGER_CHECK_QUEST_VALUE							,// 퀘스트 수치"						,
	eTRIGGER_CHECK_OWN_ARBEIT							,// 아르바이트 진행 여부"				,
	eTRIGGER_CHECK_ARBEIT_PROCESS_LEVEL					,// 아르바이트 진행 상태"				,
	eTRIGGER_CHECK_ARBEIT_VALUE							,// 아르바이트 수치"					,
	eTRIGGER_REMAIN_PROCESS_QUEST_SLOT					,//	새로운 퀘스트를 추가 시킬 수 [exist]0
	eTRIGGER_CHECK_COMPLETE_ARBEIT						,//	퀘스트 [string]을 [quest_complete]
	eTRIGGER_CHECK_QUEST_COMPLETE_CHECKER				,//	퀘스트를 완료 상태로 체크한다.
	eTRIGGER_CHECK_ARBEIT_COMPLETE_CHECKER				,//	퀘스트를 완료 상태로 체크한다.
	eTRIGGER_CHECK_ENABLE_QUEST							,//	퀘스트를 시작 할 수 있는 상태다.
	eTRIGGER_CHECK_ENABLE_ARBEIT						,//	아르바이트를 시작 할 수 있는 상태다.
	eTRIGGER_CHECK_PLAYER_JOB_BY_QUEST					,//	직업 체크
	eTRIGGER_CHECK_PLAYER_JOB_BY_ARBEIT					,//	직업 체크
	eTRIGGER_CHECK_QUEST_COUNTDOWN_CHECKER				,//	퀘스트 카운트다운 타이머가 완료 되었는지 체크
	eTRIGGER_CHECK_ARBEIT_COUNTDOWN_CHECKER				,//
	eTRIGGER_CHECK_JOIN_FIELD							,//	필드 접속 직후
	eTRIGGER_CHECK_MAIN_QUEST_VALUE						,//	메인 퀘스트 수치 x의 값이 y [이다/이상이다/이하이다]
	eTRIGGER_CHECK_MAIN_QUEST_SWITCH					,//	 메인 퀘스트 스위치 x 번이 켜져 [있다/꺼져 있다.]

//	기타
	eTRIGGER_ABOUT_ETC			=	400					,
	eTRIGGER_CHECK_OX_QUIZ_START_TIME					,	
	eTRIGGER_CHECK_TIME									,	
	eTRIGGER_CHECK_TIME2								,	
	eTRIGGER_CHECK_TODAY_TIME							,	
	eTRIGGER_CHECK_GUIDE								,	
	eTRIGGER_CHECK_COUNTDOWN_TIMER						,
	eTRIGGER_CHECK_WORD_QUIZ_START_TIME					,	
	eTRIGGER_CHECK_IS_FULL_GUILD						,	//	더 이상 길드 만들 수 없다.
	eTRIGGER_CHECK_IS_TEST_SERVER						,	//	테섭이다.
	eTRIGGER_CHECK_BLOCK_TO_OBJECT_MACRO				,	//	오브젝트 매크로 방지
	eTRIGGER_CHECK_CHANCE								,	//	확률 체크
	eTRIGGER_CHECK_LOCAL								,	//	국가 체크
	eTRIGGER_CHECK_EVENT_VALUE							,	
	eTRIGGER_CHECK_EVENT_TIME							,	//	이벤트 시간 체크
	eTRIGGER_CHECK_CHANCE2								,	//	[spin]1[0,50000] / 50000 확률 체크

//	몬스터 AI를 위한 사항
	eTRIGGER_ABOUT_MONSTER		=	500					,
	eTRIGGER_CHECK_ENEMY_COUNT_IN_RANGE					,//	주변 x 픽셀 내의 플레이어의 수가 x명 이상/이하이다.
	eTRIGGER_CHECK_HP_PERCENTAGE						,//	자신의 hp가 x% 이상/이하이다.
	eTRIGGER_CHECK_DUNGEON_FLAG							,//	던전 플레그 y가 x 이상/이하이다.
	
	eTRIGGER_CHECK_PLAY_TIME_IN_THIS_DUNGEON			,//	현재 파티가 이 던전에 들어온지 x분 이상/이하이다.
	eTRIGGER_CHECK_ARRIVE_AREA							,//	영역 x에 도착 했다.
	eTRIGGER_CHECK_TARGET_USE_BELT_ITEM					,//	타겟이 벨트 아이템을 사용했다.(현재 전투중인 타겟)
	eTRIGGER_CHECK_TARGET_FLEE							,//	타겟이 x초 이상 자신에게 공격 받고 있지 않다.(현재 전투중인 타겟)
	eTRIGGER_CHECK_USE_SKILL							,//	특정 스킬을 사용했다.
	eTRIGGER_CHECK_CLICKED_AREA							,//	특정 영역을 클릭했다.

	eTRIGGER_CHECK_MONSTER_LEVEL						,//	몬스터 레벨 체크
	eTRIGGER_CHECK_MONSTER_EVENT_VALUE					,//	이벤트 몬스터 체크

	eTRIGGER_CHECK_FIELD_VALUE							,//	필드 수치 체크
	eTRIGGER_SEASON_VALIABLE							,// 시즌 변수 체크
	eTRIGGER_CHECK_TIME3								,// 시간 체크3
	eTRIGGER_CHECK_TIME4								,// 시간 체크4	
	eTRIGGER_CHECK_PRIZE_PLAYER							,// 이벤트 목록에서 있는지를 체크.

	eTRIGGER_CHECK_WORLD_VALUE							,//	월드 수치 체크
	eTRIGGER_CHECK_WEEKDAY								,//	요일 체크.
	eTRIGGER_CHECK_MINUTE								,	// 분(포함)~분(포함) 사이이다. 체크..
	eTRIGGER_CHECK_TILESET								,	// 타일셋
	eTRIGGER_CHECK_TRANSFORMATION_MONSTER				,	// 변신중이다.
	eTRIGGER_CHECK_EVENT_COUNT_THE_HOURS				,	// 이벤트 남은 시간 체크
	eTRIGGER_CHECK_EVENT_TIME_BEFORE_AFTER				,	//	이벤트 시간 전후 체크
};	//	트리거들

//////////////////////////////////////////////////////////////////////////////////////////
//	반응들
//////////////////////////////////////////////////////////////////////////////////////////
enum
{
	eREACTION_SELECT_DIALOG								,//	대화 선택
	eREACTION_END_DIALOG								,//	대화 종료
	eREACTION_SHOP										,//	상점
	eREACTION_OPEN_BANK									,//	은행 열어!!
	eREACTION_MAKE_GUILD								,//	길드 만들어
	eREACTION_BREAK_KARMA								,//	카르마 정지!!
	eREACTION_CALL_MAGIC_CARPET							,//	마법의 양탄자 소환
	eREACTION_OPEN_GUILD_BATTLE_SCHEDULER				,//	길드전 스캐쥴 열기
	eREACTION_OPEN_GUILD_MARK_COMPOSER					,//	길드 마크 조합 인터페이스 열기
	eREACTION_OPEN_REPAIR_ITEM_WINDOW					,//	아이템 수리창 열기
	eREACTION_OPEN_JUDGE_OF_ITEM_WINDOW					,//	아이템 감별창 오픈
	eREACTION_OPEN_MERGE_ITEM_PIECE_WINDOW				,//	아이템 조각 합체창 오픈
	eREACTION_MOVE_TO_SECRET_DUNGEON					,//	인스턴스 던젼으로 이동
	eREACTION_SET_COUNTDOWN_TIMER						,//	카운트 타이머 설정
	eREACTION_COMPLETE_SECRET_DUNGEON					,//	비밀 던전 종료
	eREACTION_SET_GUILD_HALL_LEVEL						,//	1 레벨 길드 홀 부여
	eREACTION_MOVE_TO_GUILD_HALL						,//	길드 홀로 이동
	eREACTION_INCREASE_GUILD_STATUE_SKILL_LEVEL			,//	길드 석상 스킬 레벨 증가
	eREACTION_OPEN_GUILD_INVENTORY						,//	길드 아아템 창 열기
	eREACTION_ADD_GUILD_PLACE_OF_RELATED				,//	길드 연고지 추가 제거
	eREACTION_CHANGE_GUILD_HALL_SHAPE					,//	길드 홀 모양 변경
	eREACTION_TRY_GUILD_POINT_BATTLE					,//	길드 포인트 전 도전
	eREACTION_GIVE_GUILD_HONOR_POINT					,//	길드 명예 포인트 주기
	eREACTION_END_GUILD_POINT_BATTLE_AND_RECORD_GUILD_HONOR_POINT	,//	길드 명예 포인트 주기
	eREACTION_BEGIN_SIEGE_WARFARE_OF_GUILD				,//	길드 공성전 시작
	eREACTION_FEED_GUILD_PET_AND_GUARDIAN				,//	길드 펫,가디언 사육
	eREACTION_OPEN_GUILD_SHOP							,//	길드 상점 열기
	eREACTION_OPEN_RELATED_PLACE_LIST_OF_GUILD			,//	길드 연고지 리스트 열기
	eREACTION_OPEN_GUILD_HONOR_POINT_WINDOW				,//	길드 명예 포인트 보기

	eREACTION_EVENT_VALUE					,//	이벤트 수치
	eREACTION_OPEN_RELAY_STATION			,//	아이템 중계상 오픈

	eREACTION_DUEL_INTERFACE				,//	결투장 인터페이스
	eREACTION_JUMP_KARMA					,
	eREACTION_LAUNCH_RANDOM_KARMA			,
	eREACTION_SETTING_ROEN_GOODWILL			,// 로엔의 호감도... 길드 상점 관련.
	eREACTION_ENTER_GUILD_DUNGEN			,// 로엔의 호감도... 길드 상점 관련.
	eREACTION_OPEN_DISJOINTING_WINDOW		,//	아템 분해 창을 연다.
	eREACTION_LAUNCH_RANDOM_REACTION		,
	eREACTION_OPEN_INTERFACE				,	//	인터페이스창을 연다
	eREACTION_OPEN_FAST_PORTAL_WINDOW		,	//	패스트 포탈 창을 연다
//	퀘스트 관련
	eREACTION_ABOUT_QUEST		=	100					,
	eREACTION_SET_QUEST_START							,//	퀘스트 시작 상태로 만든다.
	eREACTION_SET_QUEST_COMPLETE						,//	퀘스트를 완료한다.
	eREACTION_SET_QUEST_PROCESS_LEVEL					,//	퀘스트 상태를 설정한다.
	eREACTION_CREATE_CHARACTER							,//	캐릭터 생성
	eREACTION_REMOVE_CHARACTER							,//	캐릭터 제거
	eREACTION_MOVE_FIELD								,//	필드 이동
	eREACTION_CHANGE_BATTLE_INCLINATION					,//	전투 성향 설정
	eREACTION_SYSTEM_NOTICE								,
	eREACTION_SET_QUEST_VALUE							,//퀘스트 관련 수치 설정"		
	eREACTION_CONTROL_QUEST_VALUE						,//퀘스트 수치 증가/감소"		
	eREACTION_SET_ARBEIT_START							,//아르바이트 시작"				
	eREACTION_SET_ARBEIT_PROCESS_LEVEL					,//아르바이트 상태 변경"		
	eREACTION_SET_ARBEIT_VALUE							,//아르바이트 관련 수치 설정"	
	eREACTION_CONTROL_ARBEIT_VALUE						,//아르바이트 수치 증가/감소"	
	eREACTION_CANCEL_QUEST								,//퀘스트 취소
	eREACTION_CANCEL_ARBEIT								,//아르바이트 취소
	eREACTION_GET_EXPERIENCE							,//경험치 얻기
	eREACTION_RECOVER_DEATH_PENELTY						,//죽음 페널티 치료
	eREACTION_SET_ARBEIT_COMPLETE						,//	퀘스트를 완료한다.
	eREACTION_CHECK_QUEST_COMPLETE_CHECKER				,//	퀘스트를 완료 상태로 체크한다.
	eREACTION_CHECK_ARBEIT_COMPLETE_CHECKER				,//	아르바이트를 완료 상태로 체크한다.
	eREACTION_GET_EXPERIENCE_PERCENTAGE					,//	경험치 얻기
	eREACTION_SET_QUEST_COUNTDOWN_TIMER					,
	eREACTION_SET_ARBEIT_COUNTDOWN_TIMER				,
	eREACTION_SET_CHAPTER								,	//	챕터 x 시작/종료
	eREACTION_SET_MAIN_QUEST_VALUE						,	//	메인 퀘스트 수치 x 의 값을 y [로 설정한다/증가 시킨다/감소 시킨다.]
	eREACTION_SET_MAIN_QUEST_SWITCH						,	//	메인 퀘스트 스위치 x 번을 [켠다/끈다]
	eREACTION_RESET_MAIN_QUEST_VALUE					,	//	메인 퀘스트 수치를 모두 초기화 시킨다.(0이된다.)
	eREACTION_RESET_MAIN_SWITCH							,	//	메인 퀘스트 스위치를 모두 초기화 시킨다.(모두off가 된다.)
	eREACTION_DISPLAY_MAIN_QUEST_EPILOG					,	//	메인 퀘스트 에필로그 출력
	eREACTION_EXTEND_SECRET_DUNGEON_UPKEEP_TIME			,	//	비밀던젼 플레이 타임 연장
	eREACTION_PLAY_PATTERN,
	eREACTION_PLAY_BOSS_EVENT,
	
	eREACTION_RECEIVE_HEAVEN_REWARD,						//	천상계 보상
	eREACTION_RECEIVE_HELL_REWARD,							//	지하계 보상
	eREACTION_RECEIVE_RED_DEVIL_REWARD,						//	붉은 악마 보상

	eREACTION_GET_EXPERIENCE2							,	//	경험치 획득2
	eREACTION_FIX_FIELD_RESISTANCE						,	//	필드 저항 변경
	eREACTION_SCREEN_EFFECT								,	//	화면 효과


	eREACTION_ABOUT_ITEM		=	200					,
	eREACTION_ADD_GOLD									,	
	eREACTION_ADD_ITEM									,	
	eREACTION_ADD_SPECIAL_ITEM							,	
	eREACTION_DROP_GOLD									,
	eREACTION_DROP_ITEM									,//	아이템 떨구기
	eREACTION_DROP_SPECIAL_ITEM							,//	특수 아이템 떨구기
	eREACTION_ADD_SPECIFIC_ITEM							,//	특정 아이템 추가
	eREACTION_DROP_SPECIFIC_ITEM						,//	특정 아이템 떨어뜨리기
	eREACTION_ADD_QUEST_ITEM							,//	퀘스트 아이템 추가	
	eREACTION_DROP_QUEST_ITEM							,//	퀘스트 아이템 떨구기
	eREACTION_MOVE_AREA									,//	영역 이동
	eREACTION_SELECT_OX_RESULT							,//	OX 퀴즈 결과 선택
	eREACTION_ADD_SET_ELEMENT_ITEM						,//	세트 구성 아이템 추가
	eREACTION_ADD_ITEM_PIECE							,//	아이템 조각 추가
	eREACTION_CREATE_BOOK								,//	책 아이템 생성
	eREACTION_CREATE_ADD_CATEGORY_TO_BOOK_ITEM			,//	책 아이템에 카테고리 추가 아이템 생성
	eREACTION_CREATE_ADD_PAGE_TO_BOOK_ITEM				,//	책 아이템에 페이지 추가 아이템 생성
	eREACTION_CREATE_ADD_RANDOM_CATEGORY_TO_BOOK_ITEM	,//	책에 랜덤한 카테고리를 추가하는 아이템 생성
	eREACTION_CREATE_ADD_RANDOM_PAGE_TO_BOOK_ITEM		,//	책에 랜덤한 페이지를 추가하는 아이템 생성
	eREACTION_WRITE_PAGE								,//	책에 페이지를 쓰거나 지운다.
	eREACTION_EXTEND_ITEM_VALIDATE_TIME_TO_NEXT			,
	eREACTION_EXPLOSION_EVENT_BALLOON					,//	이벤트 풍선을 터뜨린다.
	eREACTION_TIME_STAMP								,//	타임 스탬프
	eREACTION_GIVE_RED_STONE_PIECE						,//	붉은 보석 조각 주기
	eREACTION_UNOWNED_DROP_ITEM_WITH_OPTION				,// 주인이 없는 아이템 드롭(옵션)

	eREACTION_ABOUT_CHARACTER	=	300					,
	eREACTION_SHOUT										,
	eREACTION_TALK										,	//	대화
	eREACTION_RECOVER_HP								,	//	체력 설정
	eREACTION_CARGE_CP									,	//	CP 채워주기
	eREACTION_DAMAGE									,	//	체력 설정
	eREACTION_GET_TITLE									,	//	타이틀 얻기
	eREACTION_REDISTRIBUTE_SKILL						,	//	스킬 재분배
	eREACTION_REDISTRIBUTE_STATE						,	//	스탯 재분배
	eREACTION_RESET_PET_NAME							,	//	펫 이름 지우기
	eREACTION_TALK2										,
	eREACTION_CHANGE_SIGHT_RANGE						,
	eREACTION_ADD_PET									,	//	펫 추가
	eREACTION_RESET_ACTOR_IN_DUEL_SERVER				,	//	캐릭터 초기화
	eREACTION_GET_GRACE									,	//	은총 부여
	eREACTION_STATE_CHANGE								,	//	상태 이상 부여
	eREACTION_STATE_BLIND_EFFECT						,	//	암전 부여
	eREACTION_REBIRTH									,	//	전생 시킨다.
	eREACTION_DISPELL									,	//	모든 버프 해제
	eREACTION_RELEASE_TRANSFORMATION					,	//	몬스터 변신 해제
	eREACTION_INCREASE_INSTANT_EXPERIENCE,	//	일시적 경험치 증가
	
	eREACTION_ABOUT_MONSTER		=	400					,	//	몬스터 AI 관련	
	eREACTION_USE_ABILITY				,//	몬스터 스킬 xxx를 사용을 시도 한다. <- 조건에 맞지 않는다면 실패
	eREACTION_TRANS_JOB					,//	몬스터 xxx로 변신 한다.
	eREACTION_CONTROL_DUNGEON_FLAG		,//	던전 플레그를 x만큼 더한다/뺀다, x로 한다.
	eREACTION_DELAY						,//	현재 이 스크립트를 다시 시도 하는데 xxx초 만큼 딜레이를 준다.
	eREACTION_MOVE_TO					,//	영역 x로 이동/공격/순찰 한다.
	eREACTION_SET_ARCA_DOOR_TRAP_STATUS	,//	영역 x의 상자/문/함정 상태 설정.
	eREACTION_REGEN_MONSTER				,//	영역 x에 몬스터들을 리젠 시킨다.
	eREACTION_REGEN_TRAP				,//	함정 데이터 1을 x영역에 리젠 시킨다.
	eREACTION_CHANGE_TARGET_BY_RANDOM	,//	현재의 공격 타겟을 해재하고 시야 내의 렌덤한 한명을 공격 타겟으로 설정한 뒤 x초 동안 해당 타겟을 유지 한다.
	eREACTION_CHANGE_AI_TYPE			,//	AI 타입을 재설정 한다.
	eREACTION_RELEASE_WRONG_STATUS		,//	각종 상태이상을 해제한다.
	eREACTION_CONTROL_INVINCIBLE_STATUS	,//	무적을 설정/해제한다.
	eREACTION_CHANGE_DIRECT				,//	방향 바꾸기
	eREACTION_BEGIN_EVENT				,//	이벤트 중으로 설정
	eREACTION_OCCUR_SKILL_EFFECT		,//	특정 지역에 스킬 발동
	eREACTION_MOVE_FOCUS				,//	포커스 이동
	eREACTION_SHAKE_SCREEN				,//	화면 흔들기
	eREACTION_PLAY_SOUND				,//	효과음 출력
	eREACTION_STRESS_MINIMAP			,//	미니맵 특정 지역 강조
	eREACTION_SET_GUIDE					,//	가이드 설정
	eREACTION_TRACE_GUIDE				,//	가이드 따라가기
	eREACTION_FOCUS_ON_CHARACTER		,//	캐릭터에게 포커스 맞춤
	eREACTION_NPC_ACTION				,
	eREACTION_SET_DOOR_ENABLE_DISABLE	,//	영역 x의 문 상태 설정.
	eREACTION_SHAKE_SCREEN_AXIS_PLAYER	,//	플레이어 중심 지진
	eREACTION_CHANGE_NAME				,//	몬스터 이름 변경
	eREACTION_SET_EVENT_MONSTER_VALUE	,//	이벤트 몬스터로 설정

	eREACTION_SET_SCREEN_EVENT			,
	eREACTION_CONTROL_FIELD_VALUE		,//	필드 수치 설정
	eREACTION_CONTROL_WORLD_VALUE		,//	월드 수치 설정	
	
	eREACTION_TARGET_RESET				,//	타켓팅 리셋
	eREACTION_CASTING_MAGIC				,//	마법을 케스팅
	eREACTION_USE_MONSTER_ABILITY				,	// 몬스터 스킬 사용 ~!	09.08.14
	eREACTION_BUY_GOLD_BAR				,	// 금괴 추가..		09.08.19
	eREACTION_UPGRADE_SHOP				,	// 강화창을 연다.  09.08.25
	eREACTION_FIELD_WEATHER				, // 기후변화			09.10.06
	eREACTION_ADD_SET_ITEM_RANDOM		, // 세트아이템 랜덤 지급 
	eREACTION_ADD_ITEM_WITH_OPTION		,	// 아이템추가(옵션)
	eREACTION_DROP_ITEM_WITH_OPTION		,	// 아이템드롭(옵션)
	eREACTION_JOIN_GUILD_SECRET_DUNGEONS_WITH_PARTY,	// 파티원이 있는 길드비던입장.
	eREACTION_FIRE_WORK,					//	  특정 좌표에 불꽃놀이 이팩트를 뿌린다.
	eREACTION_DRAW_IMAGE,		//		화면에 이미지 그리기 ~ 
	eREACTION_OFF_DRAWING_IMAGE,		//		화면에 그린 이미지 끄기~ 

};

//	{"그냥","금괴","퀘스트","end"},								//	eKARMA_KW_ADD_GOLD_REASON

enum
{
	eADD_KARMA_GOLD_REASON_NOTHING	=	0,
	eADD_KARMA_GOLD_REASON_GOLDBAR,
	eADD_KARMA_GOLD_REASON_QUEST,
};

#endif