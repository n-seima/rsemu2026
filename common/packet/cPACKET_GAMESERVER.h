#ifndef __CPACKET_GAMESERVER_H
#define __CPACKET_GAMESERVER_H

#include	"definePacketData.h"
#include	"cPACKET_BASE.h"
#include	"cPACKET_LOGINSERVER.h"
#include	"cPACKET_DBSERVER.H"
#include	"CGuildDefine.h"
#include	"cPetDefine.h"
#include	"CPitchmanShopDefine.h"
#include	"doorDefine.h"
#include	"arcaDefine.h"
#include	"carrotShopDefine.h"
#include	"bannerDefine.h"
#include	"eventDefine.h"

#define	dDEFAULT_TARGET_COUNT					32
#define dMAX_WORLD_VALUE_MAX					128
#define dMAX_PARTY_NAME_LENGTH					22

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┃패킷 인덱스 정의

//	━━필드 진입 후					0x1021 ~ 
#define dCG_CONNECT_GAME						0x1021	//	게임서버 연결
#define dCG_JOIN_GAME							0x1022	//	게임 참가
#define dCG_MOVE								0x1023	//	이동
#define dCG_REPORT_CURRENT_POS					0x10b1	//	현재 위치 보고
#define dCG_STOP								0x1024	//	정지
#define dCG_ACTION_TO_OBJECT					0x10b3	//	오브젝트에 액션 사용
#define dCG_ACTION_TO_ACTOR						0x1025	//	액터 공격!!
#define dCG_ACTION_TO_GROUND					0x1026	//	대지 공격!!???
#define dCG_ACTION_QUICK						0x1038	//	퀵 스킬
#define dCG_SET_MOVE_ABILITY					0x1027	//	뛰기/걷기 토글
#define dCG_TRANSFORMATION						0x103a	//	나... 변신할껴!!!!!
#define dCG_ASK_INFO_ACTOR_LIST					0x1028	//	특정 액터의 데이터를 요구한다.
#define dCG_OBSCURITY_ACTOR_LIST				0x1099
#define	dCG_MISPLACED_ACTOR_LIST				0x1045	//	위치가 맞지 않는 액터 리스트
#define	dCG_ASK_PLAYER_INFO						0x10a1
#define	dCG_CHECK_PLAYER_CHECK_SUM				0x10a2
#define	dCG_ASK_ACTOR_ENCHANT_INFO				0x10be
#define dCG_SETTING_SEASON_VARIABLE				0x12f3
#define dCG_SETTING_SEASON_VARIABLE_SUCCESS		0x12f3

#define dCG_ENGAGE_BATTLE						0x102a	//	교전!!
#define dCG_STOP_BATTLE							0x102b	//	교전 중지!!
#define dCG_INCREASE_STATE						0x1036	//	스탯 업!!
#define dCG_INCREASE_ABILITY_LEVEL				0x1037	//	스킬 레벨 업!!
#define dCG_ASK_ACTOR_INFO						0x1039	//	몬스터의 정보 요청
#define dCG_TOGGLE_SITDOWN						0x103c	//	앉기 토글!!
#define dCG_MOVE_FIELD							0x103b	//	필드 이동
#define dCG_WARP_TO_NEAR_VILLAGE				0x109a	
#define	dCG_ENTER_PORTAL						0x109b	//	포탈로 드가기
#define dCG_SET_REACTION_STATUS					0x1043	//	리액션 상태 설정
#define dCG_TURN_ON_PASSIVE_SKILL				0x1044	//	패시브 스킬들을 켠다.
#define	dCG_RETURN_TO_VILLAGE					0x104f	//	마울로 돌아가기
#define	dCG_EXIT_GAME							0x1065	//	게임 종료
#define	dCG_HACKING_LOG							0x10ab
#define	dCG_GG_AUTH_DATA						0x10ac
#define	dCG_HS_GUID_REPLY						0x10c0
#define	dCG_HS_ACK_MSG_REPLY					0x10c1

#define dCG_WARP_FIELD_BY_GATE_GLOVE			0x10bb
#define dCG_WARP_FIELD_BY_NET_CAFE				0x10c6	//	넷카페아이콘으로 

//	━━아이템 관련
#define	dCG_PICK_ITEM							0x102c	//	필드에 떨어진 아이템 줍기
#define	dCG_EQUIP_ITEM							0x102d	//	아이템 장비
#define	dCG_RELOAD_BELT_ITEM					0x102e	//	아이템 리로드
#define	dCG_USE_ITEM							0x102f	//	아이템 사용
#define	dCG_USE_ITEM_TO_ITEM					0x1066	//	아이템에 아이템 사용
#define	dCG_USE_BELT_ITEM						0x1030	//	벨트에 아이템을 사용한다.
#define	dCG_ASK_ITEM_DATA						0x1031	//	뭔가 잘못 되었다!! 아이템 데이터를 요청한다.
#define	dCG_ASK_INVENTORY_DATA					0x1032	//	뭔가 잘못 되었다!! 인벤토리 데이터를 요청한다.
#define	dCG_ASK_EQUIPMENT_DATA					0x1033	//	뭔가 잘못 되었다!! 장비 데이터를 요청한다.
#define	dCG_DROP_ITEM							0x1034	//	아이템을 필드에 버린다.
#define	dCG_CHANGE_ITEM_PLACE					0x1035	//	아이템 위치 변경
#define	dCG_STRIP_EQUIPMENT						0x104e	//	장비 벗기
#define	dCG_DESTROY_ITEM						0x1082	//	아이템 파괴
#define	dCG_REPAIR_ITEM							0x1098	//	아이템 수리
#define	dCG_ASK_ITEM_PACK_NAME					0x10ad	//	아이템 수리
#define	dCG_REMOVE_PREFIX						0x10b7
#define	dCG_JUDGE_TO_ITEM						0x10b8
#define	dCG_MERGE_ITEM							0x10b9
#define	dCG_USE_SOUND_OF_LEADERS_BELL			0x10c7

//	━━아이템 교환 관련
#define	dCG_REQUEST_TRADE						0x1050	//	트레이드 요청
#define	dCG_TRADE_REPLY							0x1051	//	트레이드 수락
#define	dCG_CANCEL_TRADE						0x1052	//	트레이드 취소
#define	dCG_ADD_TRADE_ITEM						0x1053	//	트레이드 아이템 추가
#define	dCG_REMOVE_TRADE_ITEM					0x1054	//	트레이드 아이템 제거
#define	dCG_SET_TRADE_GOLD						0x1055	//	트레이드 할 골드 설정
#define	dCG_READY_TRADE							0x1056	//	트레이드 준비
#define	dCG_PERMIT_TRADE						0x1057	//	트레이드 허락
#define	dCG_CHANGE_TRADE_ITEM_DATA				0x1058	//	트레이드 아이템 추가

//	━━은행 관련
#define	dCG_STORE_ITEM_TO_THE_BANK				0x105d	//	은행에 아이템 저장 요청
#define	dCG_STORE_GOLD_TO_THE_BANK				0x105e	//	은행에 골드 저장 요청
#define	dCG_WITHDRAW_ITEM_FROM_THE_BANK			0x105f	//	은행에서 아이템 꺼내기
#define	dCG_WITHDRAW_GOLD_FROM_THE_BANK			0x1060	//	은행에서 골드꺼내기
#define	dCG_MOVE_BANK_ITEM						0x1061	//	은행 아이템 위치 변경
#define	dCG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	0x1062
#define	dCG_FINISH_BANK_TRANSACTION				0x1063	//	은행 거래 종료
#define	dCG_CANCEL_BANK_TRANSACTION				0x1064	//	은행 거래 취소(뭔가 문제가 있다.)

//	━━프리미엄 아이템 관련
#define	dCG_CART_OPEN							0x107b	//	카트오픈
#define	dCG_WITHDRAW_ITEM_FROM_THE_CART			0x107c	//	카트에서 아이템 꺼내기
#define	dCG_CLOSE_CART							0x107d	//	카트 닫기
#define	dCG_DESTROY_CART_ITEM					0x1081
#define	dCG_OPEN_CARROT_SHOP					0x10ba	//	당근 상점 열기

//	━━노점상 관련
#define	dCG_ASK_OPEN_THE_PITCHMAN_SHOP			0x108d	//	노점상 열거라고 요청
#define	dCG_ADD_PITCHMAN_SHOP_ITEM				0x108f	//	노점상 아이템 추가
#define	dCG_REMOVE_PITCHMAN_SHOP_ITEM			0x1090	//	노점상에서 아이템 제거
#define	dCG_CHANGE_PITCHMAN_SHOP_INFO			0x1091	//	노점상 상태 변경 (오픈,클로즈,종료)
#define	dCG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE		0x1094	//	노점상 아이템 위치 변경
#define	dCG_CLOSE_PITCHMAN_SHOP					0x1095	//	노점상 아이템 위치 변경
#define	dCG_BUY_PITCHMAN_SHOP_ITEM				0x1092	//	노점상에서 아이템 구입
#define	dCG_ASK_PITCHMAN_SHOP_INFO				0x1093	//	노점상 정보 요청
#define	dCG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST	0x1097	//	노점상 정보 요청

//	━━이벤트 & 상점 관련
#define	dCG_SELECT_NPC							0x103f	//	NPC를 선택한다.
#define	dCG_SELECT_SPEECH						0x1040	//	문장 선택
#define	dCG_BUY_ITEM							0x1041	//	아이템 구입
#define	dCG_SELL_ITEM							0x1042	//	아이템 판매
#define	dCG_CANCEL_QUEST						0x106a	//	퀘스트 취소
#define	dCG_BUY_CARROT_SHOP_ITEM				0x10a5	//	당근 상점 아이템 구입
#define	dCG_CHANGE_DOOR_STATUS					0x10aa	//	문 열어!!
#define	dCG_TOUCH_EVENT_AREA					0x10ae	//	이벤트 영역 건드림
#define	dCG_ARCA_WORK							0x10af	//	보물상자 관련 작업
#define	dCG_ASK_ARCA_INFO						0x10b0
#define	dCG_DISARM_FIELD_TRAP					0x10b2
#define	dCG_ASK_DOOR_INFO						0x10b4	//	문 열어!!
#define	dCG_PUT_BANNER							0x10bc
#define	dCG_ASK_BANNER_TEXT						0x10bd
#define	dCG_SEARCH_RELAY_STATION_ITEM			0x10c4
#define	dCG_SUMMON_EVENT_MONSTER				0x10c8		//	이벤트몬스터 생성해~!


//	━━커뮤니티 관련
#define dCG_CHAT								0x1029	//	몬스터 생성

#define	dCG_ASK_PARTY_LIST						0x1046	//	파티 리스트 요청
#define	dCG_ASK_PARTY_INFO						0x1048	//	자신이 속한 파티의 정보 요청
#define	dCG_ASK_JOIN_PARTY						0x1049	//	파티 참가 요청을 한다.
#define	dCG_ASK_JOIN_PARTY_REPLY				0x104a	//	파티 참가 요청 결과
#define	dCG_LEAVE_PARTY							0x104b	//	파티에서 탈퇴하기
#define	dCG_PARTY_LEADER_WORK					0x104c	//	파티에서 쫏아내기(리더만 가능)
#define	dCG_CHANGE_PARTY_NAME					0x104d	//	파티 이름 변경
#define	dCG_ACCUSE								0x105a	//	신고
#define	dCG_OPERATOR_COMMAND					0x105b	//	운영자 커맨드
#define	dCG_FIND_USER							0x105c	//	유저 찾기
#define	dCG_REQUEST_ADD_FRIEND					0x1067	//	친구 추가 요청
#define	dCG_REPLY_FOR_ADD_FRIEND				0x1068	//	친구 추가 요청에 대한 대답
#define	dCG_CHANGE_COMMUNITY_STATUS				0x1069	//	커뮤티니 상태 변경
#define	dCG_REMOVE_NAME_IN_TARGET_FRIEND_LIST	0x10a6	//	타겟 친구 목록에서 내 이름 제거
#define	dCG_REMOVE_NAME_IN_TARGET_FRIEND_LIST_RESULT	0x10a7	//	타겟 친구 목록에서 내 이름 제거

#define	dCG_CHANGE_PARTY_STATUS					0x10b5	//	파티 상태 수정

//	━━길드 관련
#define	dCG_CREATE_GUILD						0x106d	//	길드 생성
#define	dCG_ASK_GUILD_INFO						0x106e	//	길드 정보 요청
#define	dCG_ASK_JOIN_GUILD						0x1070	//	길드 참가 요청
#define	dCG_REPLY_ASK_JOIN_GUILD				0x1071	//	길드 참가 요청에 대한 응답
#define	dCG_ASK_CHANGE_GUILD_NOTICE				0x1072	//	길드 공지 변경 요청
#define	dCG_ASK_CHANGE_GUILD_EXP_TAX			0x1073	//	길드 경험치 세율 변경 요청
#define	dCG_ASK_DISMISS_SUB_GUILD_MASTER		0x1074	//	서브 길드 마스터 해임 건의?
#define	dCG_ASK_APPOINTMENT_SUB_GUILD_MASTER	0x1075	//	서브 길드 마스터 임명 요청
#define	dCG_ASK_DISMISS_GUILD_CONGRESS			0x108a	//	서브 길드 마스터 해임 건의?
#define	dCG_ASK_APPOINTMENT_GUILD_CONGRESS		0x108b	//	서브 길드 마스터 임명 요청
#define	dCG_ASK_GUILD_MEMBER_LIST				0x108c	//	길드 멤버 리스트 요청
#define	dCG_ASK_DISSOLUTION_GUILD				0x1096	//	길드 해체 요청
#define	dCG_GUILD_MEMBER_WORK					0x10c2	//	서브 길드 마스터 임명 요청

#define	dCG_ASK_BANISH_GUILD_MEMBER				0x1076	//	길드 멤버 추방 요청
#define	dCG_ASK_LEAVE_GUILD						0x1077	//	길드 탈퇴 요청
#define	dCG_ASK_CHANGE_GUILD_MASTER				0x1078	//	길드 탈퇴 요청
#define	dCG_REGIST_REGULAR_GUILD_MEMBER			0x1079
#define	dCG_READY_TO_MAKE_GUILD					0x107a
#define	dCG_INCREASE_GUILD_SKILL_LEVEL			0x1083
#define	dCG_ASK_GUILD_BATTLE_SCHEDULE			0x1084	//	길드전 정보 요청
#define	dCG_REQUEST_GUILD_BATTLE				0x1085	//	길드전 신청
#define	dCG_CLOSE_GUILD_BATTLE_SCHEDULER		0x1086
#define	dCG_CANCEL_GUILD_BATTLE					0x1087	
#define	dCG_SELECT_GUILD_MARK					0x1088	//	길드 마크 선택
#define	dCG_ASK_GUILD_MARK_INFO					0x109c
#define	dCG_ASK_SIMPLE_GUILD_INFO				0x10a8	//	단순한 길드 정보 요청
#define	dCG_GUILD_CHECK_SUM						0x10a9	//	단순한 길드 정보 요청


//	━━기타등등
#define	dCG_BAD_USER							0x1059	//	나쁜 유저
#define	dCG_DEBUG_MESSAGE						0x106f	//	디버그 메시지
#define	dCG_CALL_MAGIC_CARPET					0x107e
#define	dCG_REMEMBER_ORB_LOCATION				0x107f
#define	dCG_TELEPORT_TO_ORB_LOCATION			0x1080
#define	dCG_RECOVER_DEATH_PENELTY				0x109d
#define	dCG_CHANGE_PET_NAME						0x109e
#define	dCG_PARTING_WITH_PET					0x109f
#define	dCG_CHANGE_EMBLEM_SHAPE					0x10a0
#define	dCG_ETC_WORK							0x10bf
#define	dCG_XTRAP_PACKET						0x10c5
 
//	━━예약작업들
#define	dBW_MAKE_DUMMY							0x1300	//	더미 생성
#define dBW_EXIT_GAME							0x1301	//	게임 나가기
#define dBW_DEATH_ACTOR							0x1302	//	액터 죽이기
#define dBW_READY_TO_EXIT						0x1303	//	나가기 준비


#define dBW_MOVE_FIELD							0x1304	//	월드에서 받은 필드 이동 결과를 동기화 시켜서 처리한다.
#define dBW_EXIT_GAME_FOR_MOVE_FIELD			0x1305
#define	dBW_DISPLACEMENT						0x1306	//	디스 플레이스먼트 관련
#define	dBW_CHARMING							0x1307
#define dBW_MONSTER_TAME						0x1308	//	몬스터 조련
#define dBW_GENERATE_PET						0x1309	//	펫 참가
#define dBW_GENERATE_SUMMON_BEAST				0x130a	//	소환수 소환
#define dBW_INTERVAL_SHOOTER					0x130b
#define dBW_MAKE_SKILL_ZONE						0x130c
#define dBW_COPY_ACTOR							0x130d
#define dBW_MAKE_ILLUSION						0x130e

//	━━stress test client
#define dCG_CONNECT_STRESS_CLIENT				0x2000
#define dCG_STRESSDATA							0x2001
#define dCG_MODIFY_STRESS_CLIENT				0x2002
#define dCG_REGIST_STRESS_CLIENT				0x2003

//	LASTPACKET	0x10c9			//전 인덱스	dCG_SUMMON_EVENT_MONSTER	0x10c8	
//┃Client->Server
//┃패킷 인덱스 정의
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server->Client
//┃패킷 인덱스 정의

//	━━필드 진입 후					0x7021 ~ 
#define dSG_RESULT_CONNECT						0x1120
#define dSG_RESULT_JOIN							0x1121	//	게임 참가 결과
#define dSG_LOGOUT								0x12ff	//	로그 아웃
#define dSG_FAILED_TO_JOIN						0x128e
#define dSG_BCS_INFO							0x1270
#define dSG_UPDATE_PLAYER_STATUS_DATA			0x114a	//	스테이터스 데이터 업데이트
#define dSG_JOIN_PLAYER							0x1122	//	누군가 필드로 난입했다
#define dSG_MOVE								0x1123	//	이동
#define dSG_MOVE_FOR_PARTY_MEMBER				0x125a
#define dSG_MOVE_RESULT							0x1129	//	이동 결과(이동 하겠다고 한 넘한테만 보내줌)
#define dSG_MOVE_FAILED							0x118a	//	이동 실패
#define dSG_STOP								0x1124	//	정지
#define dSG_OUTER_ACTORS_POS_UPDATE				0x1177	//	주기적으로 자신의 위치를 업데이트 시킨다.
#define dSG_ACTION_TO_ACTOR						0x1125	//	캐릭터에게 공격/마법 사용
#define dSG_ACTION_TO_GROUP						0x1259	//	1:다수 타겟 스킬
#define dSG_ACTION_TO_GROUND					0x1126	//	필드에 마법 사용
#define dSG_ACTION_QUICK						0x114d	//	기술 시전 즉시 거리에 상관 없이 사용 가능한 스킬
#define	dSG_USE_SKILL_RESULT					0x114c	//	스킬 사용 결과
#define dSG_CORRECT_PLAYER_POSITION				0x1127	//	이동 실패! 플레이어의 위치가 이상하다!!
#define dSG_PACKET_TRAILER						0x1128	//	이동",정지,공격들의 ,
#define dSG_MINIMUM_ACTOR_INFO_LIST				0x112a	//	필드에 존재하는 캐릭터들..
#define dSG_OPERATOR_LIST						0x11fb	//	운영자 리스트
#define dSG_REFIT_ACTORS_POSITION				0x117b	//	캐릭터들의 위치 보정
#define dSG_EVENT_NPC_LIST						0x1178	//	고정되어 있는 NPC 리스트
#define dSG_SIMPLE_ACTOR_INFO_LIST				0x1179	//	필드에 존재하는 캐릭터들..
#define dSG_SIMPLE_ACTOR_INFO					0x123a	//	간단 정보
#define dSG_VERY_SIMPLE_ACTOR_INFO_LIST			0x1215	//	매우 간단한 액터 정보
#define dSG_PLAYER_OUT							0x112b	//	이런 저런 이유로.. 필드에서 나갔다.
#define dSG_SET_MOVE_ABILITY					0x112d	//	이동 어빌러티(결국.. 걷기/뛰기) 설정
#define dSG_REGEN_MONSTER						0x112e	//	몬스터 생성
#define dSG_BASIC_ATTACK						0x1130	//	근접공격
#define dSG_CONTINUOUS_HIT_BY_CHARACTER			0x112f	//	연속 히트 시키는 공격
#define dSG_CONTINUOUS_HIT_BY_VALUE				0x1188	//	수치에 의한 연속 공격
#define dSG_LAND_MARKER							0x11b4	//	랜드 마커 공격
#define dSG_STRIKE_INTERVAL_SHOOTER				0x11b5	//	인터벌 슈터 발사
#define dSG_CAST_INTERVAL_SHOOTER				0x11b6	//	인터벌 슈터 캐스팅
#define dSG_BUNSHINE_ATTACK						0x11ad	//	분신공격
#define dSG_SCIMITAR_CUTTING					0x11ae	//	시미터 커팅
#define dSG_HIT_CHANED_DAMAGE					0x1271	//	연속 대미지 입음
#define dSG_GUARDIAN_POST						0x1201	//	가디언 포스트
#define dSG_FIRST_AID							0x122c	//	가디언 포스트
#define dSG_REVIVE								0x122e	//	부활
#define dSG_UPDATE_BOOST_COUNT					0x122f
#define dSG_PLAYER_INFO							0x123d
#define dSG_SUCCESS_TO_JOIN_GAME				0x123e	//	게임 참가 성공

#define dSG_QUICK_ACTION						0x1151	//	퀵 액션
#define	dSG_KILL								0x1132	//	죽었다.
#define	dSG_UPDATE_EXP							0x1146	//	경험치 업데이트
#define	dSG_UPDATE_BASIC_INFORMATION			0x1147	//	체력 업데이트
#define	dSG_LEVEL_UP							0x1148	//	레벨 업
#define	dSG_LEVEL_UP_OTHER						0x1159	//	다른 누군가가 레벨 업 했다.
#define dSG_UPDATE_STATUS						0x119e	//	상태가 변했다.
#define	dSG_DEATH_PENELTY_SECOND				0x1211	//	죽음으로 인한 페널티 시간
#define	dSG_QUESTION_FOR_RECOVER_DEATH_PENELTY	0x1225

#define	dSG_CHANGE_STATE_RESULT					0x1149	//	스텟 변화 결과
#define	dSG_INCREASE_ABILITY_LEVEL				0x114b	//	어빌러티 레벨 증가
#define	dSG_VERY_SIMPLE_ACTOR_INFO				0x114e	//	몬스터 정보
#define	dSG_REMOVE_ACTOR						0x114f	//	제거해 버려!!
#define dSG_TRANSFORMATION						0x1152	//	변신하던가 말던가.. -_-a
#define dSG_TRANSFORMATION_FAILED				0x115b	//	변신 실패
#define dSG_MOVE_FIELD_RESULT					0x1155	//	필드 이동 결과
#define dSG_MOVE_FIELD_NOW						0x11e4	//	필드 이동 해!!
#define dSG_TOGGLE_SITDOWN						0x1156	//	앉기/서기 토글
#define dSG_SITDOWN_RESULT						0x1157	//	앉기/서기 결과
#define dSG_REDISTRIBUTE						0x1223
#define dSG_STOP_BATTLE							0x1239
#define dSG_GG_AUTH_DATA						0x124b
#define dSG_WINNING_PRIZE_LOTTO					0x124c
#define	dSG_HS_GUID								0x1273
#define	dSG_HS_REQ_MSG							0x1274

#define dSG_ARCA_LIST							0x124f
#define dSG_ADD_ARCA							0x1250
#define dSG_ATTACK_TO_OBJECT					0x1251
#define dSG_ARCA_WORK							0x1252
#define dSG_BROKEN_ARCA_ITEM_BY_ATTACK			0x1253
#define dSG_CLOSE_RANGE_ARCA_WORK				0x1255
#define	dSG_FIELD_TRAP_WORK						0x1256
#define	dSG_CLOSE_RANGE_FIELD_TRAP_WORK			0x1258		

#define	dSG_ENTER_TO_THE_SECRET_DUNGEON			0x125b
#define	dSG_SECRET_DUNGEON_MESSAGE				0x125c
#define	dSG_ETC_WORK							0x125d
#define	dSG_PLAY_SOUND							0x125e
#define	dSG_CHECK_RANGE_ETC_WORK				0x125f
#define	dSG_SET_ANM								0x1260
#define	dSG_DUEL_RECORD							0x1289
#define	dSG_DUEL_RESULT							0x128a
#define	dSG_DUEL_RANKING						0x128b
#define	dSG_TRY_DUEL_BATTLE						0x128c
#define	dSG_DUEL_MSG							0x128d

//	━━펫 관련
#define dSG_PET_INFO							0x1226
#define dSG_CHANGE_PET_COMMAND					0x1227
#define dSG_SYNC_PET_INFO						0x1228
#define dSG_PET_WAIT_COMMAND					0x1229
#define dSG_MAKE_BREEDING_RECORD_BOOK			0x122b
#define	dSG_UPDATE_PET_EXP						0x122d
#define	dSG_CHANGE_PET_NAME						0x1230
#define	dSG_PARTING_WITH_PET					0x1232
#define dSG_MINI_PET_INFO						0x1286

//	━━소환수 관련
#define dSG_SUMMON_BEAST_INFO					0x1231
#define dSG_RIDING_DOG							0x1233

//	━━스킬 관련
#define	dSG_WIDE_AREA_DAMAGE					0x1150	//	광역 데미지
#define	dSG_WIDE_AREA_EFFECT					0x11ac	//	광역 이펙트
#define	dSG_WIDE_HEAL							0x126c	//	광역 힐
#define	dSG_STRIKE_LIGHTNING_WINDER				0x126d
#define	dSG_WIDE_AID_ATTACK_RESULT				0x1272
#define	dSG_STRIKE_MULTI_MISSILE				0x129a	//  멀티미사일..

#define dSG_SET_REACTION_STATUS					0x1164	//	리액션 상태 설정
#define dSG_TURN_ON_PASSIVE_SKILL				0x116f	//	패시브 스킬들을 켠다.
#define	dSG_HIT_THORN_DAMAGE					0x1165	//	걍 데미지를 준다.
#define	dSG_ABSORB_DAMAGE						0x119d	//	데미지 흡수
#define	dSG_CHANGE_SPECIAL_SKILL_FIELD			0x1166	//	특수 스킬 필드 변경
#define	dSG_RELEASE_SHIMMERING_SHIELD			0x1167	//	블러커 띄우기
#define	dSG_SET_SHIMMERING_SHIELD				0x1168	//	시머링 실드 설정
#define	dSG_CURE_ACTOR							0x1169	//	액터 치유
#define	dSG_CURE_ACTOR_BY_POTION				0x11e2	//	포션에 의한 치유
#define	dSG_CHARGE_CP							0x1187	//	CP충전
#define	dSG_HIT_AURA_SKILL						0x118c	//	오라 스킬에 맞았다!!
#define	dSG_DISPLACEMENT						0x1216	//	빙의
#define	dSG_FLOAT_MACHINE						0x116b	//	머신 띄우기
#define	dSG_RELEASE_MACHINE						0x116c	//	머신 풀어주기
#define	dSG_SHOOT_MACHINE_MISSILE				0x116d	//	머신이 미사일 발사!!
#define	dSG_FIRE_TAIL_CHASER					0x116e	//	테일 체이서 발동
#define	dSG_MAKE_ACTION_FORM					0x1170	//	타격 정보로 액터를 공격
#define	dSG_THRUST_ACTORS						0x1171	//	액터들 밀어내기
#define	dSG_USE_SKILL_TO_ACTOR					0x1172	//	액터에게 스킬 사용
#define	dSG_USE_SKILL_TO_GROUND					0x1173	//	땅에다 스킬 사용
#define	dSG_CREATE_DUMMY						0x1175	//	더미 제거
#define	dSG_COPY_ACTOR							0x126b
#define	dSG_RELEASE_DUMMY						0x1174	//	더미 생성
#define	dSG_RECEIVE_AID_MAGIC					0x1186	//	보조 마법 캐스팅
#define dSG_THRUST								0x11a6	//	밀려나기
#define dSG_RUSH								0x11a9	//	러쉬
#define dSG_ACTIVE_REACTION_SKILL				0x11aa	//	녹백 방어 스킬 발동
#define dSG_ICY_STALAGMITE						0x11af	//	아이시 스탈라그마이트 사용
#define	dSG_ACTOR_ENCHANTED_STATUS				0x11b0	//	캐릭터 몸뚱아리에 마법 이펙트
#define	dSG_TELEPORT							0x11b1	//	텔레포트
#define	dSG_APPLY_SKILL_EXTRA_EFFECT			0x11b2	//	스킬 부가 효과 적용
#define	dSG_FIND_USER_RESULT					0x11b7	//	유저 찾기 결과
#define	dSG_ACTIVE_SKILL_EFFECT					0x11b3	//	스킬 효과 발동
#define	dSG_ACTIVE_MIRROR_TOWER					0x1222	//	미러타워 발동
#define	dSG_SIMPLE_HIT							0x11bb	//	간단한..
#define	dSG_HIT_DAMAGE							0x1213	//	대미지 입다
#define	dSG_REGEN_HP_BY_AID_SKILL				0x11bc	//	보조 스킬로 인한 체력 리젠
#define	dSG_RECOVER_HP_BY_AID_SKILL				0x121c
#define dSG_HIT_ACTION							0x11d1
#define dSG_DETECTING_EVIL						0x121a
#define dSG_MISS								0x121b	//	미스 났슈
#define dSG_READY_TO_WARP_TO_NEAREST_VILLAGE	0x121d
#define dSG_REMOVE_PORTAL						0x121f
#define dSG_ADD_PORTAL							0x1220
#define dSG_PORTAL_LIST							0x1221
#define dSG_SKILL_EFFECT						0x1238
#define dSG_PLAY_SKILL_SOUND					0x123c
#define dSG_EXIST_FIRE_WALL_LIST				0x1276
#define	dSG_ACTION_TO_GROUP_STICKED_BIT			0x129d
//	━━아이템 관련
#define	dSG_DROPPING_ITEM						0x1133	//	아이템을 떨군다.
#define	dSG_STRIP_EQUIPMENT						0x1153	//	장비 벗기
#define	dSG_HIDE_EQUIPMENT						0x11ab	//	장비 감추기
#define	dSG_STRIP_EQUIPMENT_RESULT				0x1189	//	장비 벗기 결과
#define	dSG_REMOVE_DROPPED_ITEMS				0x1134	//	필드에 떨어진 아이템들 제거
#define	dSG_REMOVE_DROPPED_ITEM					0x1135	//	필드에 떨어진 아이템들 제거
#define	dSG_CHANGE_DROPPED_ITEM_INFO			0x1136
#define	dSG_DROPPING_ITEMS						0x1137	//	필드에 아이템을 떨어뜨린다.
#define	dSG_PICK_ITEM_RESULT					0x1138	//	아이템을 줏었다.(아이템을 줍겠다고 신고한넘한테 날려준다. 나머지한테는 걍 사라졌다고 알려준다.)
#define	dSG_EQUIP_ITEM_RESULT					0x1139	//	아이템 장비했다고 알려준다.(외관상 변화가 있으면 필드 전체에 전달하고, 아니면 사용한 넘한테만..)
#define	dSG_USE_ITEM_RESULT						0x113b	//	아이템 사용(주위에 녀석들과 사용한넘 한테 아이템 사용했다고 알려준다.)
#define	dSG_USE_ITEM_TO_ITEM_RESULT				0x11c4	//	아이템을 아이템에 사용한 결과
#define	dSG_USE_BELT_ITEM_RESULT				0x113c	//	벨트 아이템 사용 결과.(주위에 녀석들과 사용한넘 한테 아이템 사용했다고 알려준다.)
#define	dSG_USE_ITEM							0x113d	//	아이템 사용(주위에 녀석들과 사용한넘 한테 아이템 사용했다고 알려준다.)
#define	dSG_LOTTERY_TICKET_PRIZE_WINNING		0x118b	//	복권 아이템 당첨됐다.
#define	dSG_ITEM_DATA							0x113e	//	특정 캐릭터에게 아이팀 데이터를 몽땅 날려준다.
#define	dSG_INVENTORY_DATA						0x1142	//	특정 캐릭터에게 인벤토리 데이터를 몽땅 날려준다.
#define	dSG_EQUIPMENT_DATA						0x1143	//	특정 캐릭터에게 장비 데이터를 몽땅 날려준다.
#define	dSG_CHANGE_APPEARANCE					0x113f	//	캐릭터의 외관이 변했다.
#define	dSG_DROP_ITEM_RESULT					0x1141	//	아이템 버리기 결과
#define	dSG_RELOAD_BELT_ITEM					0x113a	//	리로드~
#define	dSG_BELT_ITEM_RELOAD_RESULT				0x1144	//	리로드 결과
#define	dSG_CHANGE_ITEM_PLACE_RESULT			0x1145	//	아이템 위치 변경
#define dSG_DECREASE_ITEM_COUNT					0x11a1	//	아이템 수 줄이기
#define dSG_ADD_ITEM							0x11a2	//	아이템 추가
#define dSG_ADD_GOLD							0x11a3	//	골드 추가
#define	dSG_SET_ITEM_UNIQUE_SERIAL				0x11b9	//	유니크 시리얼
#define	dSG_UPDATE_ITEM_DATA					0x11c5	//	아이템 하나의 정보 업데이트
#define	dSG_REMOVE_ITEM							0x11cb	//	아이템 제거
#define	dSG_UPDATE_WAY_POINT_INFO				0x11e3	//	웨이 포인트 정보 변경
#define	dSG_EXPIRED_ITEM						0x11e5	//	아이템 익스파이어드(게임 도중에 체크해서 나린다.
#define	dSG_EXPIRED_ITEM_LIST					0x11e6	//	아이템 익스파이어드 리스트
#define	dSG_USE_ORB_RESULT						0x11e7	//	오브 사용 결과
#define	dSG_DESTROY_ITEM_RESULT					0x11e9	//	아이템 파괴
#define	dSG_UPDATE_ITEM_DURABILITY				0x1212	//	아이템 내구도 바뀜
#define	dSG_REPAIR_ITEM							0x1217	//	아이템 수리
#define	dSG_ITEM_WORK_RESULT					0x1218
#define	dSG_OPEN_REPAIR_ITEM_WINDOW				0x1219
#define	dSG_ITEM_PACK_NAME						0x124d
#define	dSG_REMOVE_PREFIX						0x1262
#define	dSG_OTHER_PLAYER_ITEM_DATA				0x1275
#define	dSG_ITEM_PACK_DATA						0x1285
#define	dSG_OPEN_DISJOINTING_DATA				0x12e0	//	아이템 분해


//	(dSG_PICK_ITEM_RESULT~dSG_USE_BELT_ITEM_RESULT 체크섬을 만들어서 아이템 데이터를 확인한다.)

//	━━트레이드 관련
#define	dSG_REQUEST_TRADE						0x1190	//	트레이드 요청
#define	dSG_REQUEST_TRADE_RESULT				0x1191	//	트레이드 요청 결과
#define	dSG_BEGIN_TRADE							0x1192	//	트레이드 시작
#define	dSG_ADD_TRADE_ITEM_BY_ME				0x1193	//	트레이드 아이템 추가
#define	dSG_ADD_TRADE_ITEM_BY_TRADE_MATE		0x1194	//	트레이드 아이템 추가
#define	dSG_CHANGE_TRADE_ITEM_DATA_BY_ME			0x119b	//	트레이드 아이템 추가
#define	dSG_CHANGE_TRADE_ITEM_DATA_BY_TRADE_MATE	0x119c	//	트레이드 아이템 추가
#define	dSG_SET_TRADE_GOLD						0x1195	//	트레이드 아이템 추가
#define	dSG_REMOVE_TRADE_ITEM					0x1196	//	트레이드 아이템 제거 트레이드 아이템 제거 결과
#define	dSG_READY_TRADE							0x1197	//	트레이드 준비
#define	dSG_PERMIT_TRADE						0x1198	//	트레이드 수락
#define	dSG_TRADE								0x1199	//	트레이드
#define	dSG_TRADE_MESSAGE						0x119a	//	트레이드 메시지

//	━━커뮤니티 관련
#define dSG_CHAT								0x1158	//	채팅
#define dSG_CHAT_FAILED							0x118d	//	채팅 실패
#define dSG_INFO								0x1154	//	각종 정보를 보내준다. (파티, 길드, 기타 정보들..)
#define dSG_OUT									0x1140	//	서버가 유저를 죽인다. -o-;
#define dSG_SAY_RESULT							0x1176	//	세이 실패 -o-;;
#define dSG_EVENT_NOTICE						0x11ba	//	공지?
#define dSG_NOTICE								0x123b

#define	dSG_PARTY_LIST							0x117c	//	파티리스트 전송
#define	dSG_CHANGE_PARTY_STATUS					0x1261	//	파티리스트 전송

#define	dSG_PARTY_INFO							0x117d	//	파티멤버에게 보내는 파티 정보(파티원 리스트나 기타등등)
#define dSG_ASK_JOIN_PARTY						0x117e	//	파티 참가 요청
#define	dSG_JOIN_PARTY_MEMBER					0x117f	//	파티 멤버 추가
#define	dSG_PARTY_WORK							0x1180	//	누군가 파티에서 탈퇴 했다.(거나 쫏겨 났거나.. -_-a)
#define	dSG_UPDATE_PARTY_MEMBER_HP				0x126e
#define	dSG_UPDATE_PARTY_MEMBER_INFO			0x1181	//	파티 멤버 정보를 업데이트(체력정도..)
#define	dSG_PARTY_MESSAGE						0x1182	//	파티와 관련된 이런저런 메시지(넌 리더가 아냐!!,넌 파티에 속해 있지 않아!!,파티 풀~ 등등등..)
#define	dSG_CHANGE_PARTY_NAME					0x1183	//	파티 이름 변경
#define	dSG_DISSOLUTION_PARTY					0x1185	//	파티 해체 -o-
#define	dSG_PCIK_PARTY_ITEM						0x118e	//	파티가 아이템 습득
#define	dSG_PCIK_PARTY_GOLD						0x118f	//	파티가 골드 습득
#define	dSG_MESSAGE								0x11a4	//	메시지 전송
#define dSG_REGIST_MESSAGE						0x11cf	//	등록된 서버 메시지
#define	dSG_REMAIN_HP							0x11a5	//	남은 체력
#define	dSG_REQUEST_ADD_FRIEND					0x11c8	//	친구 추가 요청
#define	dSG_REPLY_FOR_ADD_FRIEND				0x11c9	//	친구 추가에 대한 대답
#define dSG_EXIT_GAME_SUCCESS					0x11d0	//	게임에서 나가기 성공
#define dSG_PICK_PARTY_ITEM						0x123f	//	파티 아이템을 줏었다.
#define dSG_ASK_REMOVE_FRIEND					0x1244	//	친구 목록에서 내 이름 제거해줘!! 라고 요청
#define dSG_ASK_REMOVE_FRIEND_RESULT			0x1245	//	친구 목록에서 내 이름 제거해줘!! 라고 요청한거 결과
#define	dSG_GAME_SERVER_MESSAGE					0x124e

//	━━파티 연합 관련 패킷
#define dSG_ASK_JOIN_UNION_PARTY				0x1310	//	연합 파티 참가 요청
#define dSG_REPLAY_JOIN_UNION_PARTY				0x1311	//	연합 파티 참가 대답
#define dSG_UNION_PARTY_INFO					0x1312	//	연합 파티 정보 전송
#define dSG_UNION_PARTY_MEMBER_INFO				0x1313	//	연합 파티의 파티원 정보를 정보 전송
#define dSG_UNION_PARTY_MEMBER_HP_STATE			0x1314	//	연합 파티원에게 HP , 상태등을 맞춘다.
#define dSG_DISSOLUTION_UNION_PARTY				0x1315	//	연합 파티원에게 연합 파티 해체를 알린다.


//	━━길드 관련
#define	dSG_READY_TO_MAKE_GUILD					0x11ca	//	길드 맹글기 준비
#define	dSG_OPEN_GUILD_BATTLE_SCHEDULER			0x11eb	//	길드 배틀 스케쥴러 열기
#define	dSG_OPEN_GUILD_MARK_COMPOSER			0x11f2
#define	dSG_CHANGE_GUILD_BATTLE_SCHEDULE		0x11ec	//	길드 배틀 스케쥴 변경됨
#define	dSG_GUILD_BATTLE_SCHEDULE_IN_SAME_TIME	0x11ed	//	같은 시간대에 길드전 스케쥴
#define	dSG_MAKE_GUILD_RESULT					0x11cd	//	길드 만들기 결과
#define	dSG_BASE_GUILD_INFO						0x11ce	//	길드 기본 정보
#define	dSG_UPDATE_GUILD_BATTLE_INFO_FOR_MEMBER	0x11ef
#define	dSG_READY_TO_NEXT_GUILD_BATTLE			0x11f0	//	다음 길드전 준비하라고 메시지 전송
#define	dSG_GUILD_BATTLE_NOTICE					0x1210	//	길드전 관련 경지
#define	dSG_GUILD_BATTLE_KILL_NOTICE			0x1269
#define	dSG_GUILD_BATTLE_INFO					0x126a
#define	dSG_DISSOLUTION_GUILD					0x11d2	//	길드 뽀개짐
#define	dSG_JOIN_GUILD							0x11d3	//	길드 가입
#define	dSG_UPDATE_GUILD_MEMBER					0x1203	//	길드 멤버 추가
#define	dSG_REMOVE_GUILD_MEMBER					0x11dd	//	길드 멤버 제거
#define	dSG_EXIT_GUILD							0x11d4	//	길드 탈퇴
#define	dSG_ASK_JOIN_GUILD_RESULT				0x11d5	//	길드 참가 결과(실패)
#define	dSG_ASK_JOIN_GUILD						0x11d6	//	길드 참가 요청
#define	dSG_UPDATE_GUILD_LEVEL					0x11d7	//	길드 레벨 업데이트
#define	dSG_GUILD_WORK_MESSAGE					0x11d8	//	길드 작업 관련 메시지
#define	dSG_CHANGE_GUILD_TAX_RATE				0x11da	//	경험치 세율 변경
#define	dSG_CHANGE_GUILD_NOTICE					0x11db	//	길드 공지 변경
#define	dSG_CHANGE_RANK_IN_GUILD				0x11dc	//	길드 멤버 랭크 변경
#define	dSG_UPDATE_GUILD_SKILL_LEVEL			0x11ea	//	길드 멤버 랭크 변경
#define	dSG_UPDATE_GUILD_BATTLE_COUNT_AT_THIS_WEEK	0x11ee	//	한주동안 싸운 횟수 업데이트
#define dSG_GUILD_BATTLE_RESULT					0x11f1	//	길드전 결과
#define dSG_CHANGE_GUILD_MARK					0x11f3	//	길드마크 선택 결과
#define dSG_GUILD_MEMBER_LIST					0x1202	//	길드마크 선택 결과
#define dSG_GUILD_MARK_INFO						0x1224
#define dSG_SIMPLE_GUILD_INFO					0x1246	//	길드의 단순한 정보
#define	dSG_OPEN_GUILD_HALL_LIST				0x1279	//	길드홀 리스트 열기
#define	dSG_OPEN_GUILD_INVENTORY				0x127a	//	길드 인벤토리 열기
#define	dSG_STORE_GUILD_INVENTORY_ITEM			0x127b
#define	dSG_REMOVE_GUILD_INVENTORY_ITEM			0x127c
#define	dSG_STORE_GUILD_INVENTORY_GOLD			0x127d
#define	dSG_REMOVE_GUILD_INVENTORY_GOLD			0x127e
#define	dSG_CHANGE_GUILD_INVENTORY_ITEM_PLACE	0x127f
#define	dSG_SIEGE_WARFARE_NOTICE				0x1280
#define	dSG_GUILD_PET_AND_GUARDIAN_EXP			0x1281
#define	dSG_OPEN_GUILD_SHOP						0x1282
#define	dSG_OPEN_RELATED_PLACE_LIST				0x1283
#define	dSG_GUILD_HONOR_POINT_LIST				0x1284
#define	dSG_GUILD_MASTER_GRACE_INFO				0x129f	
#define	dSG_GVG_WORK_SUCCESS					0x129c

//	━━이벤트 관련
#define dSG_COMPLEX_SPEECH						0x115c	//	대화
#define dSG_EVENT_FAILED						0x115d	//	이벤트 실패
#define dSG_END_DIAOLG							0x115e	//	대화 끝내기
#define dSG_OPEN_SHOP							0x115f	//	상점 열기
#define dSG_BUY_ITEM							0x1160	//	아이템 구입
#define dSG_FAILED_BUY_ITEM						0x1161	//	아이템 구입 실패
#define dSG_SELL_ITEM							0x1162	//	아이템 판매
#define dSG_FAILED_SELL_ITEM					0x1163	//	아이템 판매 실패
#define dSG_QUEST_INFO							0x11a0	//	퀘스트 정보 전송
#define dSG_CHANGE_QUEST_INFO					0x11bd	//	변경된 퀘스트 정보 전송
#define	dSG_CANCEL_QUEST						0x11cc	//	타이틀 제거
#define dSG_DAMAGE_TO_ACTOR_BY_KARMA			0x11b8	//	카르마에 의해 캐릭터한테 대미지 입힘
#define	dSG_GET_TITLE							0x11c6	//	타이틀 획득
#define	dSG_REMOVE_TITLE						0x11c7	//	타이틀 제거
#define	dSG_ANNOUNCE_QUIZ_EVENT					0x11f4	//	OX 관련 퀴즈 공지
#define	dSG_START_COUNTDOWN						0x11f5	//	카운트 다운 시작
#define	dSG_QUIZ_EVENT_QUESTION					0x11f7	//	OX 퀴즈 질문
#define	dSG_OX_QUIZ_RESULT						0x11f6	//	OX 퀴즈 결과 받음
#define	dSG_END_QUIZ_EVENT						0x11f8	//	OX 퀴즈 종료
#define	dSG_QUIZ_EVENT_LOSER_LIST				0x11f9	//	OX 퀴즈 오답자 리스트
#define	dSG_CANCEL_QUIZ_EVENT					0x11fa	//	OX 퀴즈 취소
#define	dSG_OX_QUIZ_SURVIVOR_COUNT				0x11fc	//	OX 퀴즈 참가자중 생존자 수
#define	dSG_OX_QUIZ_LOSER_REVIVE_BATTLE			0x11fd	//	OX 퀴즈 패자 부활전
#define	dSG_OX_QUIZ_WINNER_LIST					0x11fe	//	OX 퀴즈 승자 리스트 전송
#define	dSG_BEGIN_QUIZ_EVENT					0x11ff	//	OX 퀴즈 시작
#define	dSG_BUY_CARROT_SHOP_ITEM_RESULT			0x1242
#define	dSG_CHANGE_DOOR_STATUS					0x1247
#define	dSG_DOOR_MESSAGE						0x1248
#define	dSG_DOORS_STATUS_IN_FIELD				0x1249
#define	dSG_BANNER_INFO							0x1265
#define	dSG_BANNER_TEXT							0x1266
#define	dSG_BANNER_LIST							0x1267
#define	dSG_BOOST_POWER_BY_EVENT				0x1268
#define	dSG_WORD_QUIZ_RESULT					0x126f
#define	dSG_EVENT_SCREEN_MOVE					0x1278
#define	dSG_SOUND_OF_LEADERS_BELL				0x1299
#define	dSG_EVENT_TIME_UPDATE					0x129e

//	━━은행 관련
#define dSG_OPEN_BANK							0x11a7	//	은행 열기
#define dSG_BANK_MESSAGE						0x11a8	//	서버에서 은행 데이터를 기다린다.
#define	dSG_STORE_ITEM_TO_THE_BANK				0x11be
#define	dSG_STORE_GOLD_TO_THE_BANK				0x11bf
#define	dSG_WITHDRAW_ITEM_FROM_THE_BANK			0x11c0
#define	dSG_WITHDRAW_GOLD_FROM_THE_BANK			0x11c1
#define	dSG_MOVE_BANK_ITEM						0x11c2
#define	dSG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	0x11c3

//	━━프리미엄 아이템  관련
#define dSG_OPEN_CART							0x11de	//	은행 열기
#define dSG_CART_MESSAGE						0x11df	//	서버에서 은행 데이터를 기다린다.
#define	dSG_WITHDRAW_ITEM_FROM_THE_CART			0x11e0
#define	dSG_LOAD_FAILED_CART_PREMIUM_ITEM		0x11e1
#define	dSG_DESTROY_CART_ITEM_RESULT			0x11e8
#define	dSG_CARROT_SHOP_INFO					0x1263
#define	dSG_OPEN_CARROT_SHOP					0x1264

//	━━노점상  관련
#define dSG_OPEN_PITCHMAN_SHOP					0x1204	//	
#define dSG_ADD_PITCHMAN_SHOP_ITEM				0x1205	//
#define dSG_REMOVE_PITCHMAN_SHOP_ITEM			0x1206	//
#define dSG_CHANGE_PITCHMAN_SHOP_INFO			0x1207	//
#define dSG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE		0x120f	//
#define	dSG_BUY_PITCHMAN_SHOP_ITEM				0x1208	//	노점상에서 아이템 구입
#define	dSG_SOLD_PITCHMAN_SHOP_ITEM				0x1209	//	노점상에서 아이템이 팔렸다.
#define dSG_ADD_PITCHMAN_SHOP					0x120a	//	노점상 추가
#define dSG_REMOVE_PITCHMAN_SHOP				0x120b	//	노점상 제거
#define dSG_PITCHMAN_SHOP_LIST					0x120c	//	노점상 리스트
#define dSG_PITCHMAN_SHOP_INFO					0x120d	//	노점상 정보
#define dSG_PITCHMAN_SHOP_MESSAGE				0x120e
#define dSG_SIMPLE_PITCHMAN_SHOP_INFO_LIST		0x1214
#define dSG_RESULT_OF_SEARCH_ITEM_IN_RELAY_STATION	0x1288

//	━━함정 관련
#define dSG_ADD_TRAP							0x1234
#define dSG_REMOVE_TRAP							0x1235
#define dSG_EXPLOSION_TRAP						0x1236
#define dSG_EXIST_TRAP_LIST						0x1237
#define dSG_EXPLOSION_TRAP_SKILL				0x1254
#define	dSG_EXIST_ACTIVE_FIELD_TRAP_LIST		0x1257

//	━━거신의 은총 관련		GGG = GreateGodGrace 거신의 은총.
#define dSG_ADD_GGG_LEVEL_UP					0x1290		
#define dSG_ADD_GGG_SKILL_UP					0x1291		
#define dSG_ADD_GGG_PARTY						0x1292		
#define dSG_GGG_LEVEL_UP_TIME_END				0x1293		
#define dSG_GGG_SKILL_UP_TIME_END				0x1294
#define dSG_ADD_GGG								0x1295
#define dSG_REMOVE_GGG							0x1296
#define dSG_PLAY_GGG_EFFECT						0x1297
#define dSG_GGG_INFO							0x1298
//	━━기타
#define	dSG_WARNING_MESSAGE						0x117a	//	경고 메시지
#define	dSG_CONTROL_MONSTER						0x119f	//	몬스터 콘트롤.. -o-
#define	dSG_OCCUR_EFFECT						0x122a
#define	dSG_ACCOUNT_INFO						0x1243
#define	dSG_PENELTY_TIME						0x124a
#define	dSG_TITLE_EFFECT						0x1287
#define	dSG_XTRAP_PACKET						0x128f
#define	dSG_UPDATE_PACKET_ENCRYT_CODE			0x129b	//	패킷 암호화 코드 업데이트

//	━━Admin
#define dSG_SENDINFOTOADMIN						0x1201	//	접속통계 전송
#define dSG_CLOSESERVER							0x1202	//	서버를 종료해라.

//	━━stress test client
#define dSG_STRESSCLIENT						0x2200
#define dSG_STRESS_SERVER_INFO					0x2201

//  ━━이벤트 당첨자 리스트 확인.
#define dSG_RESULT_PRIZE_PLAYER					0x2202
//┃Server->Client
//┃패킷 인덱스 정의
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//	lps	0x129f			//  사용할 차례

#define dEXIST_ACTOR_COUNT_FOR_ROOKIE				200
#define dVISIBLE_ACTOR_COUNT_FOR_ROOKIE				50
#define dEXIST_ACTOR_COUNT							10

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		PACKET		DEFINE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┃이동/정지/공격

enum
{
	eCONNECT_TYPE_FIRST		,
	eCONNECT_TYPE_MOVE_FIELD,
};

//
//	게임서버에 연결
struct	CG_CONNECT_GAME
{
	cMSG_BASE_TYPE	base;

	WORD			wType;			//	0 - 처음 로그인 , 1 - 맵이동
	char			strId[dID_LENGTH];
	DWORD			dwCode;
	char			strName[dNAME_LENGTH];	//	선택한 아바타 이름
	char			strMacAddress[64];
	char			strClientCode[12];
#ifdef	_OGP_SERVICE
	char			strToken[dTOKEN_LENGTH];
#endif
};	//	struct	CG_CONNECT_GAME

//
//	게임 참가
struct	CG_RESULT_PRIZE_PLAYER
{
	cMSG_BASE_TYPE	base;
	WORD			wSerialInField;
	WORD			wResult;
};//struct	CG_JOIN_GAME

//
//	게임 참가
struct	CG_JOIN_GAME
{
	cMSG_BASE_TYPE	base;
};//struct	CG_JOIN_GAME

//	시즌 변수 세팅.
struct	CG_SETTING_SEASON_VALIABLE
{
	cMSG_BASE_TYPE	base;
	WORD			wSeasonValiable;
};//struct	CG_JOIN_GAME

//
//	이동한다.
struct	CG_MOVE
{
	cMSG_BASE_TYPE		base;

	WORD				wPosX,wPosY;			//	현재 위치
	WORD				wDestPosX,wDestPosY;	//	목표 위치
};

//
//	이동한다.
struct	CG_REPORT_CURRENT_POS
{
	cMSG_BASE_TYPE		base;

	WORD				wPosX,wPosY;			//	현재 위치
	WORD				wDestPosX,wDestPosY;	//	목표 위치
};

//
//	정지한다.
struct	CG_STOP
{
	cMSG_BASE_TYPE	base;

	WORD			wDirect;				//	방향
	WORD			wPosX,wPosY;			//	현재 위치
};	//	struct	CG_STOP

#define	dMOVE_ABILITY_WALK		0	
#define	dMOVE_ABILITY_RUN		1
//
//	걷기/뛰기 전환
struct	CG_SET_MOVE_ABILITY
{
	cMSG_BASE_TYPE	base;

	WORD			wState;					//	0 걷기,1 뛰기
};	//	struct	CG_SET_MOVE_ABILITY

//
//	저... 변신해도 되요? *-_-*
struct	CG_TRANSFORMATION
{
	cMSG_BASE_TYPE	base;

	WORD			wPosX,wPosY;
};

struct	CG_ACTION_TO_OBJECT
{
	cMSG_BASE_TYPE	base;

	BYTE			bObjectType,bAbility;
	WORD			wObjectSerial;	//	대상 시리얼
};
//
//	몬스터나 플레이어한테 액션(공격/마법)을 취한다.
struct	CG_ACTION_TO_ACTOR
{
	cMSG_BASE_TYPE	base;

	WORD			wTargetSerial;	//	대상 시리얼
	BYTE			bAbility,bEngageAbility,bSubAbility;	//	전투 어빌러티
};	//	struct	CG_ACTION_TO_ACTOR

//
//	필드에 마법을 사용한다.
struct	CG_ACTION_TO_GROUND
{
	cMSG_BASE_TYPE	base;

	WORD			wDestPosX,wDestPosY;	//	목표 위치
	WORD			wAbility;
};	//	struct	CG_ACTION_TO_GROUND

//
//	퀵 스킬
struct	CG_ACTION_QUICK
{
	cMSG_BASE_TYPE	base;

	WORD			wAbility;
	WORD			wPosX,wPosY;
};	//	struct	CG_ACTION_QUICK

//
//	교전신청 *-_-*(... 교제 신청이 아니란 말이다!!)
struct	CG_ENGAGE_BATTLE
{
	cMSG_BASE_TYPE	base;

	WORD			wTargetSerial;			//	대상 시리얼
	WORD			wAbility;
};	//	struct	CG_ENGAGE_BATTLE

//
//	교전 중지!!
struct	CG_STOP_BATTLE
{
	cMSG_BASE_TYPE	base;

};	//	struct	CG_STOP_BATTLE

enum
{
	eCHARACTER_BASIC_STATE_STRENGTH		,
	eCHARACTER_BASIC_STATE_CONSTITUTION	,
	eCHARACTER_BASIC_STATE_AGILITY		,
	eCHARACTER_BASIC_STATE_INTELLIGENCE	,
	eCHARACTER_BASIC_STATE_WISDOM		,
	eCHARACTER_BASIC_STATE_CHARISMA		,
	eCHARACTER_BASIC_STATE_LUCK			,
	eCHARACTER_BASIC_STATE_FAILED		,
};

//
//	스탯 업!!
struct	CG_INCREASE_STATE
{
	cMSG_BASE_TYPE	base;

	WORD			wState;
	WORD			wPoint;
};	//	struct	CG_INCREASE_STATE

//
//	어빌러티 레벨 증가
struct	CG_INCREASE_ABILITY_LEVEL
{
	cMSG_BASE_TYPE	base;

	WORD			wAbility;
	WORD			wPoint;
	WORD			wAimLevel;
};	//	struct	CG_INCREASE_ABILITY_LEVEL

//
//	몬스터의 정보 요청
struct	CG_ASK_ACTOR_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget;
};

//	앉기 토글!!
struct	CG_TOGGLE_SITDOWN
{
	cMSG_BASE_TYPE	base;
	WORD			wPosX,wPosY;
};

//
//	필드 이동 하고 싶어요~~
struct	CG_MOVE_FIELD
{
	cMSG_BASE_TYPE	base;

};	//	struct	CG_FIELD_MOVE

struct	CG_WARP_TO_NEAR_VILLAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wVillage;
};

struct	CG_ENTER_PORTAL
{
	cMSG_BASE_TYPE	base;

	WORD			wPortal;
};

//
//	리액션 상태 설정
struct	CG_SET_REACTION_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD			wAbility;
	WORD			wIsOn;
};	//	struct	CG_SET_REACTION_STATUS

//
//	패시브 스킬들을 켠다.
struct	CG_TURN_ON_PASSIVE_SKILL
{
	cMSG_BASE_TYPE	base;

	BYTE			aPassiveSkill[6];
};	//	struct	CG_TURN_ON_PASSIVE_SKILL

//
//	Return To Village
enum
{
	eRTV_BY_REVIVE,
	eRTV_BY_ITEM,
	eRTV_BY_END_OF_GUILD_BATTLE,
	eRTV_BY_END_OF_EVENT,
};

//	마울로 돌아가기
struct	CG_RETURN_TO_VILLAGE
{
	cMSG_BASE_TYPE	base;
	
	WORD			wWork;
};

//
//	Exit Game Value
enum
{
	eEGV_RETURN_TO_TITLE,
	eEGV_RETURN_TO_SELECT_AVATAR,
	eEGV_EXIT_TO_WINDOW,
};

struct	CG_EXIT_GAME
{
	cMSG_BASE_TYPE	base;
	
	WORD			wValue;
};

struct	CG_HACKING_LOG
{
	cMSG_BASE_TYPE	base;

	WORD			wCode;
	char			strHackName[32];
};

struct	CG_GG_AUTH_DATA
{
	cMSG_BASE_TYPE	base;

	char			aBuffer[16];
};

struct	CG_HS_GUID_REPLY
{
	cMSG_BASE_TYPE	base;

	BYTE			abGuidAckMsg[20];
};

struct	CG_HS_ACK_MSG_REPLY
{
	cMSG_BASE_TYPE	base;

	BYTE			abAckMsg[56];
};

struct	CG_WARP_FIELD_BY_GATE_GLOVE
{
	cMSG_BASE_TYPE	base;

	WORD			wDestField;
	int				iItemSlot;
};

//┃이동/정지/공격
//┃Client->Server
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┃아이템 관련

//
//	필드에 떨어진 아이템 줍기
struct	CG_PICK_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;	//	체크 섬
	WORD			wItemSerial;//	필드에 떨어진 아이템의 시리얼
};

//
//	아이템 장비
struct	CG_EQUIP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;	//	체크 섬
	WORD			wItemSlot;	//	아이템 슬롯
	WORD			wBaseItem;	//	어떤 아이템인겨?(최소한의 정보로 확인한다.)
	WORD			wPart;		//	어느 부위?
};	//	struct	CG_EQUIP_ITEM

//
//	아이템 리로드
struct	CG_RELOAD_BELT_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;	//	체크 섬
};	//	struct	CG_RELOAD_BELT_ITEM

//
//	아이템 사용
struct	CG_USE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;	//	체크 섬
	WORD			wItemSlot;	//	아이템 슬롯
	WORD			wBaseItem;	//	어떤 아이템인겨?(최소한의 정보로 확인한다.)
	WORD			wWho;		//	언넘한테?
};	//	struct	CG_USE_ITEM

//
//	아이템 사용
struct	CG_USE_ITEM_TO_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;		//	체크 섬
	WORD			wBaseItem;		//	어떤 아이템인겨?(최소한의 정보로 확인한다.)
	BYTE			bItemSlot,bTargetItem;
};	//	struct	CG_USE_ITEM_TO_ITEM

//
//	벨트에 아이템을 사용한다.
struct	CG_USE_BELT_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;	//	체크 섬
	WORD			wBeltSlot;	//	아이템 슬롯
	WORD			wBaseItem;	//	어떤 아이템인겨?(최소한의 정보로 확인한다.)
	WORD			wTarget;
};

//
//	아이템 정보를 요청한다.
struct	CG_ASK_ITEM_DATA
{
	cMSG_BASE_TYPE	base;
};	//	struct	CG_ASK_ITEM_DATA

//
//	인벤토리 데이터를 요청
struct	CG_ASK_INVENTORY_DATA
{
	cMSG_BASE_TYPE	base;
};	//	struct	CG_ASK_INVENTORY_DATA

//
//	장비 데이터를 요청
struct	CG_ASK_EQUIPMENT_DATA
{
	cMSG_BASE_TYPE	base;
};	//	struct	CG_ASK_EQUIPMENT_DATA

//
//	아이템을 필드에 버린다.
struct	CG_DROP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;	//	체크 섬
	WORD			wPosX,wPosY;
	WORD			wItemSlot;
	DWORD			dwCount;	//	몇개나 떨굴껴?
	WORD			wBaseItem;	//	어떤 아이템인겨?(최소한의 정보로 확인한다.)
};	//	struct	CG_DROP_ITEM

//
//	아이템 위치 변경
struct	CG_CHANGE_ITEM_PLACE
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;	//	체크 섬
	WORD			wSrcItemSlot,wDestItemSlot;
};	//	struct	CG_CHANGE_ITEM_PLACE

//
//	장비 벗기
struct	CG_STRIP_EQUIPMENT
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;	//	체크 섬
	WORD			wPart;
};	//	struct	CG_STRIP_EQUIPMENT

struct	CG_DESTROY_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;	//	체크 섬
	WORD			wSlot;
};

struct	CG_REPAIR_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;	//	체크 섬
	WORD			wSlot;
	WORD			wNpcSerial;
};

struct	CG_ASK_ITEM_PACK_NAME
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			awPackIndex[32];
};

struct	CG_REMOVE_PREFIX
{
	cMSG_BASE_TYPE	base;

	WORD			wPrefixIndex;
	WORD			wUseItem,wDestItem;
};

struct	CG_JUDGE_TO_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wItemSlot;
	WORD			wNpc;
};

struct	CG_MERGE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wItemSlot;
	WORD			wNpc;
};


//┃아이템 관련
//┃Client->Server
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┃트레이드 관련

//
//	트레이드 요청
struct	CG_REQUEST_TRADE
{
	cMSG_BASE_TYPE	base;
	WORD			wTradeTarget;
	char			strTargetName[dNAME_LENGTH];
};

//
//	Reply of Trade Request
enum
{
	eRTR_OK,	//	알았오.. -o-
	eRTR_DENY,	//	시저!!
};

//
//	트레이드 수락
struct	CG_TRADE_REPLY
{
	cMSG_BASE_TYPE	base;

	char			strAsker[dNAME_LENGTH];
	WORD			wReply;
};

//
//	트레이드 취소
struct	CG_CANCEL_TRADE
{
	cMSG_BASE_TYPE	base;
};	//	struct	CG_CANCEL_TRADE

//
//	트레이드 아이템 추가
struct	CG_ADD_TRADE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wItemSlot;
	WORD			wCount;	//	몇개?
};

//
//	기존의 거래 아이템 정보 변경(갯수)
struct	CG_CHANGE_TRADE_ITEM_DATA
{
	cMSG_BASE_TYPE	base;

	WORD			wItemSlot;
	WORD			wTradeSlot;
	WORD			wCount;	//	몇개?
};	//	struct	CG_CHANGE_TRADE_ITEM_DATA

//
//	트레이드 아이템 제거
struct	CG_REMOVE_TRADE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wTradeSlot;
};

//
//	트레이드 할 골드 설정
struct	CG_SET_TRADE_GOLD
{
	cMSG_BASE_TYPE	base;

	int				iGold;
};

//
//	트레이드 준비
struct	CG_READY_TRADE
{
	cMSG_BASE_TYPE	base;
};

//
//	트레이드 허락
struct	CG_PERMIT_TRADE
{
	cMSG_BASE_TYPE	base;
	int				iCheckSum;
};

//┃트레이드 관련
//┃Client->Server
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┣━━은행 관련
struct	CG_STORE_ITEM_TO_THE_BANK	//	은행에 아이템 저장 요청
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	WORD			wInventorySlot,wBankSlot;
	WORD			wBankSerial;
};

struct	CG_STORE_GOLD_TO_THE_BANK	//	은행에 골드 저장 요청
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	int				iGold;
	WORD			wBankSerial;
};

struct	CG_WITHDRAW_ITEM_FROM_THE_BANK//	은행에서 아이템 꺼내기
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	WORD			wInventorySlot,wBankSlot;
	WORD			wBankSerial;
};

struct	CG_WITHDRAW_GOLD_FROM_THE_BANK//	은행에서 골드꺼내기
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	int				iGold;
	WORD			wBankSerial;
};

struct	CG_MOVE_BANK_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wPos1,wPos2;
	DWORD			dwCheckSum;
	WORD			wBankSerial;
};

struct	CG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION
{
	cMSG_BASE_TYPE	base;

	WORD			wPos1,wPos2;
	DWORD			dwCheckSum;
	WORD			wBankSerial;
};

struct	CG_FINISH_BANK_TRANSACTION	//	은행 거래 종료
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	WORD			wBankSerial;
};

struct	CG_CANCEL_BANK_TRANSACTION//	은행 거래 취소
{
	cMSG_BASE_TYPE	base;
};

//┣━━은행 관련
//┃Client->Server
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┣━━프리미엄 아이템 관련

struct	CG_CART_OPEN	//	카트오픈
{
	cMSG_BASE_TYPE	base;
};

struct	CG_CLOSE_CART	//	카트닫기
{
	cMSG_BASE_TYPE	base;
};

struct	CG_DESTROY_CART_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCartSerial;
	WORD			wCartSlot;
	DWORD			dwCheckSum;
};

struct	CG_WITHDRAW_ITEM_FROM_THE_CART	//	카트에서 아이템 꺼내기
{
	cMSG_BASE_TYPE	base;

	WORD			wCartSerial;
	WORD			wCartSlot;
	WORD			wInventorySlot;
	DWORD			dwCheckSum;
};

struct	CG_OPEN_CARROT_SHOP
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
};

//┣━━프리미엄 아이템 관련
//┃Client->Server
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┣━━노점상 관련
//	노점상 열거라고 요청
struct	CG_ASK_OPEN_THE_PITCHMAN_SHOP
{
	cMSG_BASE_TYPE	base;

	WORD			wPosX,wPosY;
};

//	노점상 아이템 등록
struct	CG_ADD_PITCHMAN_SHOP_ITEM		// 09.08.21
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wInventorySlot,wShopSlot;
	DWORD			dwPrice;
	BYTE			bDenomination;		// 통화 단위~
};

//	노점상에서 아이템 제거
struct	CG_REMOVE_PITCHMAN_SHOP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot,wSerial;
};

//	노점상 상태 변경 (오픈,클로즈,종료)
struct	CG_CHANGE_PITCHMAN_SHOP_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wTextColor;
	WORD			bf1IsBold			: 1;
	WORD			bf1IsShadowText		: 1;
	WORD			bf2Status			: 2;
	WORD			bf10SignboardShape	: 10;

	char			strName[dPITCHMAN_SHOP_NAME_LENGTH];
};

struct	CG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wSlot1,wSlot2;	//	구입 슬롯
};

struct	CG_CLOSE_PITCHMAN_SHOP
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
};

//	노점상에서 아이템 구입
struct	CG_BUY_PITCHMAN_SHOP_ITEM		// 09.08.21
{
	cMSG_BASE_TYPE	base;

	WORD			wPitchmanSerial;	//	노점상 시리얼
	WORD			wSlot;	//	구입 슬롯
	DWORD			dwPrice;
	BYTE			bDenomination;		// 통화 단위~
	CItemDefine		item;
};

//	노점상 정보 요청
struct	CG_ASK_PITCHMAN_SHOP_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wPitchmanSerial;	//	노점상 시리얼
};

#define	dMAX_ASK_SIMPLE_PITCHMAN_SHOP_INFO_COUNT	10

struct	CG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;	//	노점상 시리얼
	WORD			awList[dMAX_ASK_SIMPLE_PITCHMAN_SHOP_INFO_COUNT];
};

//┣━━노점상 관련
//┃Client->Server
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┃이벤트 관련

//
//	NPC를 선택했다.
struct	CG_SELECT_NPC
{
	cMSG_BASE_TYPE	base;

	WORD			wNpcSerial;
};

//
//	문장 선택
struct	CG_SELECT_SPEECH
{
	cMSG_BASE_TYPE	base;

	WORD			wSpeaker;
	WORD			wSpeechIndex;
	WORD			wSelectSpeech;
	WORD			wEventArea;
};

//
//	아이템 구입
struct	CG_BUY_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wOwner;
	WORD			wShopSerial;
	WORD			wSelectItem;
	WORD			wIsGuildShop;
	WORD			wBuyCount;
	WORD			wIsTokenShop;
	WORD			bf1IsBadgeOfTraderShop :1;		// 거래상인의 배지 상점..
	WORD			:0;
};
//
//	아이템 판매
struct	CG_SELL_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wShopKeeper;
	WORD			wShopSerial;
	WORD			wSellItemSlot;
	WORD			wCheckSum;	//	체크 섬
	WORD			wBaseItem;	//	베이스 아이템
	WORD			wCount;		//	몇개 파냐?
	WORD			bf1IsBadgeOfTraderShop :1;		// 거래상인의 배지 상점..
	WORD			:0;
};	//	struct	CG_SELL_ITEM

//
//	퀘스트 취소
struct	CG_CANCEL_QUEST
{
	cMSG_BASE_TYPE	base;

	WORD			wQuestSlot;
	WORD			wQuestIndex;
};

//
//	Change Door Status Method
enum
{
	eCDSM_OPEN_DOOR,
	eCDSM_CLOSE_DOOR,
	eCDSM_ATTACK_DOOR,
	eCDSM_DISARM_DOOR,
	eCDSM_UNLOCK_DOOR,
	eCDSM_UNLOCK_DOOR_BY_KEY,
	eCDSM_ENTER_TO_THE_SECRET_DUNGEON,
	eCDSM_COUNT
};

struct	CG_CHANGE_DOOR_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD	wArea;

	WORD	wMethod;
};

struct	CG_TOUCH_EVENT_AREA
{
	cMSG_BASE_TYPE	base;

	WORD	wArea;
};

enum
{
	eAAW_OPEN,
	eAAW_DISARM_TRAP,
	eAAW_UNLOCK,
	eAAW_UNLOCK_BY_USE_KEY,
	eAAW_ATTACK,
};

struct	CG_ARCA_WORK
{
	cMSG_BASE_TYPE	base;

	WORD	wWork,wArca;
	WORD	wValue1;
};

struct	CG_ASK_ARCA_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			awArcaIndex[32];
};

struct	CG_DISARM_FIELD_TRAP
{
	cMSG_BASE_TYPE	base;

	WORD			wFieldTrap;
};

struct	CG_ASK_DOOR_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wDoorArea;
};

struct	CG_PUT_BANNER
{
	cMSG_BASE_TYPE	base;

	WORD	wX,wY;
	WORD	wBannerItemSlot;

	char	strTitle[dBANNER_TITLE_LENGTH];
	char	strComment[dBANNER_COMMENT_LENGTH];
};

struct	CG_ASK_BANNER_TEXT
{
	cMSG_BASE_TYPE	base;

	WORD	wBannerSerial;
};

struct	CG_SEARCH_RELAY_STATION_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD	wItemType;
	WORD	wLevelRange;
	WORD	wFilterJob;		//	16  모든 직업,17 내 직업 만 
	WORD	wLastShop,wLastSlot;

	WORD	bf1IsSearchReverse	:	1;
	WORD	bf1IsORCondition:	1;	//	검색 조건이 OR 이냐?
	WORD	bf1MaskUnique	:	1;	//	유니크 아이템
	WORD	bf1MaskMagic	:	1;	//	마법 아이템
	WORD	bf1MaskDX		:	1;	//	DX 아이템
	WORD	bf1MaskUltimate	:	1;	//	얼티밋 아이템

	char	strFilter[64];

};

struct	CG_BUY_CARROT_SHOP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCategory,wItemIndex;
};

//┃이벤트 관련
//┃Client->Server
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┃데이터 보정

#define	dMAX_REQUEST_ACTOR_DATA_COUNT		50

//	특정 액터의 데이터를 요구한다.
struct	CG_ASK_INFO_ACTOR_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;					//	요청 수
	WORD			aAskInfoActorList[dMAX_REQUEST_ACTOR_DATA_COUNT];	//	누구-_-?
};

//	불분명한 액터의 데이터를 요구한다.
struct	CG_OBSCURITY_ACTOR_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;					//	요청 수
	WORD			aObscurityActor[dMAX_REQUEST_ACTOR_DATA_COUNT];	//	누구-_-?
};

enum
{
	eAEI_SHAKLE_SKILL,
	eAEI_BITE_HANGER,
	eAEI_THE_OTHER_SELF_COUNT,
};

#define	dMAX_ASK_ACTOR_ENCHANT_INFO_COUNT	50

struct	CG_ASK_ACTOR_ENCHANT_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;			//	요청 수
	WORD			awData[dMAX_ASK_ACTOR_ENCHANT_INFO_COUNT][2];
};

#define	dMAX_MISPLACED_ACTOR_COUNT			100

struct	CG_MISPLACED_ACTOR_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;			//	요청 수
	WORD			aMisplacedActorList[dMAX_MISPLACED_ACTOR_COUNT];	//	누구-_-?
};

struct	CG_ASK_PLAYER_INFO
{
	cMSG_BASE_TYPE	base;

	char			strName[dNAME_LENGTH];
};

struct	CG_CHECK_PLAYER_CHECK_SUM
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
};


//┃Client->Server
//┃데이터 보정
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┃커뮤니티

#define	dCHAT_CHAT						0
#define	dCHAT_PARTY						1
#define	dCHAT_SAY						2
#define	dCHAT_GUILD						3
#define	dCHAT_SHOUT						4
#define dCHAT_ADMIN						5
#define dCHAT_DUMMY						6

//
//	채팅 메시지
struct	CG_CHAT
{
	cMSG_BASE_TYPE	base;

	WORD			wChatType;					//	메시지 타입
	char			strDestName[dNAME_LENGTH];	//	누구한테 보낼껴?(세이의 경우)
	char			strMessage[dCHAT_LENGTH];	//	메시지
};	//	struct	CG_CHAT

//
//	Ask Party List Direction;

enum
{
	eAPLD_PREVIOUS,
	eAPLD_NEXT,
};

//
//	파티 리스트 요청
struct	CG_ASK_PARTY_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wPartyIndex;
	WORD			wDirection;
};

//
//	자신이 속한 파티의 정보 요청
struct	CG_ASK_PARTY_INFO
{
	cMSG_BASE_TYPE	base;
};

//
//	파티 참가 요청을 한다.
struct	CG_ASK_JOIN_PARTY
{
	cMSG_BASE_TYPE	base;

	WORD			bf1IsJoinPartyApplication	:	1;		//	파티에 가입 하기 위해 요청한거다.(파티 리더가 가입 권유 하는게 아님)
	WORD			bf1IsUnionPartyApplication	:	1;		//	연합 파티를 위한 요청이다.
	WORD			bf1IsGuildMemberJoinParty		:	1;

	char			strName[dNAME_LENGTH];
	
};

//
//	파티 참가 요청 결과
struct	CG_ASK_JOIN_PARTY_REPLY
{
	cMSG_BASE_TYPE	base;

	WORD			bf1IsForJoinPartyApplication	:	1;	//	파티에 가입 신청한 유저에게 답변
	WORD			bf1IsUnionPartyApplication		:	1;	//	연합 파티를 위한 요청이다.
	WORD			bf1IsGuildMemberJoinParty		:	1;

	WORD			wReply;	//	응답

	char			strAskerName[dNAME_LENGTH];	//	초대한 유저의 이름
};

struct	CG_CHANGE_PARTY_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD	bf3GoldShareMethod	:	3;
	WORD	bf3ItemShareMethod	:	3;
	WORD	bf3PartyObject		:	3;	//	파티 목적
	WORD	bf1IsOpenParty		:	1;	//	멤버를 계속 모집한다.
};

//
//	파티에서 탈퇴하기
struct	CG_LEAVE_PARTY
{
	cMSG_BASE_TYPE	base;
};

enum
{
	ePLW_BANISH_MEMBER,
	ePLW_CHANGE_LEADER,
};
//
//	파티에서 쫏아내기(리더만 가능)
struct	CG_PARTY_LEADER_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	char			strName[dNAME_LENGTH];
};

//
//	파티 이름 변경
struct	CG_CHANGE_PARTY_NAME
{
	cMSG_BASE_TYPE	base;

	char			strName[dPARTYNAME_LENGTH];
};

//
//	신고
struct	CG_ACCUSE
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum1,dwCheckSum2;
	DWORD			dwSerial;
};

//
//	operator command
enum
{
	eOC_RETURN_TO_VILLAGE	,	//	마을로 돌아가기
	eOC_CHANGE_LOCATE		,	//	위치 이동
	eOC_MOVE_FIELD			,	//	필드를 이동
	eOC_RECALL				,	//	소환 - 누군가를 내 옆으로 소환
	eOC_WARP				,	//	워프 - 누군가의 옆으로 이동
	eOC_BANISH				,	//	추방 - 누군가를 게임 밖으로 쫏아냄
	eOC_JAIL				,	//	감옥 - 누군가를 감옥으로 보냄
	eOC_DRAWING_ROOM		,	//	응접실 - 누군가를 응접실로 보냄
	eOC_VILLAGE				,	//	마을 - 마을로 보내기
	eOC_CONTROL_MONSTER		,
	eOC_TRANS_TO_EVENT_MOB	,
	eOC_REGEN_FIELD_MOB		,
	eOC_SPRINKLE_GIFT		,
	eOC_HIDE				,
	eOC_GET_ITEM			,
	eOC_SUMMON_MONSTER		,
};

//
//	운영자 명령어
struct	CG_OPERATOR_COMMAND
{
	cMSG_BASE_TYPE	base;

	WORD			wCommand;
	WORD			wX,wY;
	char			strName[dNAME_LENGTH];
};

//
//	유저 찾기
struct	CG_FIND_USER
{
	cMSG_BASE_TYPE	base;

	char			strName[dNAME_LENGTH];
};	//	struct	CG_FIND_USER

//
//	친구 추가 요청
struct	CG_REQUEST_ADD_FRIEND
{
	cMSG_BASE_TYPE	base;

	char			strName[dNAME_LENGTH];

	WORD			bf1IsRecoverListWork	:	1;
	WORD			bf8Job					:	8;
	WORD			wCheckSign;
};

//	친구 추가에 대한 대답
//	Reply of Friend Add
enum
{
	eRFA_OK,			//	OK
	eRFA_DENY,			//	거절
	eRFA_CAN_NOT_FIND,	//	못 찾겠어요. -o-
	eRFA_STATUS_DENY_COMMUNITY,	//	커뮤니티 거부 상태
	eRFA_AUTO_ADD_BY_ADDED,	//	상대방한테 추가되서 자동으로 추가
};

//
//	친구 추가에 대한 응답
struct	CG_REPLY_FOR_ADD_FRIEND
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	char			strName[dNAME_LENGTH];
};

//
//	커뮤니티 상태 변경
struct	CG_CHANGE_COMMUNITY_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD			bf1IsDenyCommunity		:	1;
	WORD			bf1IsHidePitchmanShop	:	1;
};

struct	CG_REMOVE_NAME_IN_TARGET_FRIEND_LIST//	타겟 친구 목록에서 내 이름 제거
{
	cMSG_BASE_TYPE	base;

	char	strTargetName[dNAME_LENGTH];
};

enum
{
	eRNITFLR_SUCCESS,
	eRNITFLR_NOT_EXIST,
};

struct	CG_REMOVE_NAME_IN_TARGET_FRIEND_LIST_RESULT//	타겟 친구 목록에서 내 이름 제거
{
	cMSG_BASE_TYPE	base;

	char	strRequestUserName[dNAME_LENGTH];
	WORD	wResult;
};

//
//	길드 생성
struct	CG_CREATE_GUILD
{
	cMSG_BASE_TYPE	base;

	char			strGuildName[dGUILD_NAME_LENGTH];
	char			strSubGuildMasterName[dNAME_LENGTH];
};

struct	CG_ASK_GUILD_INFO
{
	cMSG_BASE_TYPE	base;

	WORD	wIsIgnoreSkillData;
};

struct	CG_ASK_JOIN_GUILD
{
	cMSG_BASE_TYPE	base;

	WORD			wTargetSerial;
};

//
//	Ask Join Guild Reply
enum
{
	eAJGR_DENY		,
	eAJGR_ACCEPT	,
	eAJGR_BUSY		,
};

struct	CG_REPLY_ASK_JOIN_GUILD
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wAskerSerial;
	char			strAskerName[dNAME_LENGTH];
};

//	길드 공지 변경 요청
struct	CG_ASK_CHANGE_GUILD_NOTICE
{
	cMSG_BASE_TYPE	base;

	char			strNotice[dGUILD_NOTICE_LENGTH];
};

//	길드 경험치 세율 변경 요청
struct	CG_ASK_CHANGE_GUILD_EXP_TAX
{
	cMSG_BASE_TYPE	base;

	WORD			wTax;
};

//	서브 길드 마스터 해임 건의?
struct	CG_ASK_DISMISS_SUB_GUILD_MASTER
{
	cMSG_BASE_TYPE	base;

	char			strSubGuildMaster[dNAME_LENGTH];
};

//	서브 길드 마스터 임명 요청
struct	CG_ASK_APPOINTMENT_SUB_GUILD_MASTER
{
	cMSG_BASE_TYPE	base;

	char			strSubGuildMaster[dNAME_LENGTH];
};

//	서브 의원 해임 건의?
struct	CG_ASK_DISMISS_GUILD_CONGRESS
{
	cMSG_BASE_TYPE	base;

	char			strGuildCongress[dNAME_LENGTH];
};

//	서브 의원 임명 건의?
struct	CG_ASK_APPOINTMENT_GUILD_CONGRESS
{
	cMSG_BASE_TYPE	base;

	char			strGuildCongress[dNAME_LENGTH];
};

struct	CG_ASK_GUILD_MEMBER_LIST
{
	cMSG_BASE_TYPE	base;

	BOOL			bIsinit;
};

enum
{
	eGMW_RENT_GUILD_PET,
	eGMW_RETRIEVAL_RENTED_GUILD_PET,
};

struct	CG_GUILD_MEMBER_WORK
{
	cMSG_BASE_TYPE	base;

	WORD	wWork,awValue[6];
	char	strMember[dNAME_LENGTH];
};

struct	CG_ASK_DISSOLUTION_GUILD
{
	cMSG_BASE_TYPE	base;
};

//	길드 멤버 추방 요청
struct	CG_ASK_BANISH_GUILD_MEMBER
{
	cMSG_BASE_TYPE	base;

	char			strMember[dNAME_LENGTH];
};

//	길드 탈퇴 요청
struct	CG_ASK_LEAVE_GUILD
{
	cMSG_BASE_TYPE	base;
};

struct	CG_ASK_CHANGE_GUILD_MASTER
{
	cMSG_BASE_TYPE	base;

	char			strMaster[dNAME_LENGTH];
};

struct	CG_REGIST_REGULAR_GUILD_MEMBER
{
	cMSG_BASE_TYPE	base;

	char			strName[dNAME_LENGTH];
};

struct	CG_READY_TO_MAKE_GUILD
{
	cMSG_BASE_TYPE	base;
};

struct	CG_INCREASE_GUILD_SKILL_LEVEL
{
	cMSG_BASE_TYPE	base;
	WORD			wSkill;
};

struct	CG_ASK_GUILD_BATTLE_SCHEDULE
{
	cMSG_BASE_TYPE	base;

	WORD			wTimeOrder;
};

struct	CG_REQUEST_GUILD_BATTLE
{
	cMSG_BASE_TYPE	base;

	WORD			wTimeOrder,wFieldOrder;
};

struct	CG_CANCEL_GUILD_BATTLE
{
	cMSG_BASE_TYPE	base;
	
	WORD			wTimeOrder;
};

struct	CG_SELECT_GUILD_MARK
{
	cMSG_BASE_TYPE	base;

	CGuildMarkInfo	guildMarkInfo;
};

struct	CG_ASK_GUILD_MARK_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wGuildSerial;
};

struct	CG_ASK_SIMPLE_GUILD_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wGuildSerial;
};

struct	CG_GUILD_CHECK_SUM
{
	cMSG_BASE_TYPE	base;

	int				iCheckSum;
};

struct	CG_CLOSE_GUILD_BATTLE_SCHEDULER
{
	cMSG_BASE_TYPE	base;
};

//
//	Bad User Type
enum
{
	eBUT_SPEED_HACK,
};

//
//	나쁜 어린이에효!!
struct	CG_BAD_USER
{
	cMSG_BASE_TYPE	base;

	WORD			wType;
};

#define	dDEBUG_MESSAGE_LENGTH	256

struct	CG_DEBUG_MESSAGE
{
	cMSG_BASE_TYPE	base;

	char			strMessage[dDEBUG_MESSAGE_LENGTH];
};

struct	CG_CALL_MAGIC_CARPET
{
	cMSG_BASE_TYPE	base;
};

struct	CG_REMEMBER_ORB_LOCATION
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot;
};

struct	CG_TELEPORT_TO_ORB_LOCATION
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot;
};

struct	CG_RECOVER_DEATH_PENELTY
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot;
};

struct	CG_CHANGE_PET_NAME
{
	cMSG_BASE_TYPE	base;

	WORD			wPetIndex;
	WORD			wIsMiniPet;
	char			strPetName[dPET_NAME_LENGTH+1];
};

struct	CG_PARTING_WITH_PET
{
	cMSG_BASE_TYPE	base;

	WORD	bf10PetIndex	:	11;
	WORD	bf6Type			:	6;	//	0 펫,1 소환수,2 미니펫
};

struct	CG_CHANGE_EMBLEM_SHAPE
{
	cMSG_BASE_TYPE	base;

	WORD	wShape;
};

enum
{
	eCEW_USE_TITLE,
	eCEW_BUY_PIGEON_POST_ITEM,
	eCEW_USE_COUPLE_RING,
	eCEW_FIRE_FIRE_WORK,
	eCEW_CHANGE_ISPI_SETTING,
	eCEW_USE_BOTTOMLESS_BOX,
	eCEW_REMOVE_TITLE,
	eCEW_SELECT_GUILD_MARK,
	eCEW_DISCONNECT_BCS,
	eCEW_ASK_GUILD_BATTLE_SCHEDULE,
	eCEW_END_EVENT,
	eCEW_DISARM_TRAP,
	eCEW_CHANGE_MONSTER_INFO,
	eCEW_CLOSE_GUILD_HALL_LIST,

	eCEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_ITEM_COUNT	,	//	인덱스,직위,개수
	eCEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_GOLD		,	//	직위,골드
	eCEW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE				,	//	인덱스,사이즈

	eCEW_STORE_GUILD_INVENTORY_ITEM			,	//	길드 아이템 보관
	eCEW_WITHDRAW_GUILD_INVENTORY_ITEM		,	//	길드 아이템 꺼내기
	eCEW_STORE_GUILD_INVENTORY_GOLD			,	//	길드 인벤에 골드 보관
	eCEW_WITHDRAW_GUILD_INVENTORY_GOLD		,	//	길드 인벤에서 골드 꺼내기
	eCEW_CHANGE_GUILD_INVENTORY_ITEM_PLACE	,	//	길드 인벤에 아이템 위치 변경
	eCEW_SELECT_GUILD_HALL,
	
	eCEW_TRY_TO_GUILD_POINT_BATTLE,
	eCEW_RETURN_TO_GUILD_HALL_BY_END_BATTLE,
	eCEW_SUMMON_GUILD_GUARDIAN,
	eCEW_UNSUMMON_GUILD_GUARDIAN,
	eCEW_FEED_ITEM_TO_GUILD_PET	,				//	길드 펫에게 아이템 먹이기
	eCEW_RETURN_TO_THE_GUILD_HALL_BY_END_GUILD_POINT_BATTLE,

	eCEW_CHANGE_OPEN_GUILD_HALL_STATUS,
	eCEW_CLOSE_RELATED_PLACE_LIST,
	eCEW_SELECT_RELATED_PLACE,

	eCEW_ADD_RELATED_PLACE_OF_GUILD,
	eCEW_ASK_ITEM_LIST_IN_ITEM_PACK,
	eCEW_SELECT_ITEM_IN_PACK,

	eCEW_ACTIVATE_MINI_PET,	//	미니펫 활성화
	eCEW_FEED_ITEM_TO_MINIPET,

	eCEW_INCREASE_MINIPET_SKILL_LEVEL,
	eCEW_USE_MINIPET_CHARGE_SKILL,
	eCEW_USE_BLOCK_LIGHT_POCKET,

	eCEW_USE_POLISHER,
	eCEW_RESET_MINIPET_NAME,

	eCEW_CHANGE_MINIPET_PLACE,
	eCEW_INCREASE_TITLE_LEVEL_BY_CH5,
	eCEW_CH5_RELEASE_ITEM_REVERSION,
	eCEW_CH5_RESET_ONE_STATE,
	eCEW_CH5_RESET_ONE_SKILL,
	eCEW_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP,
	eCEW_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP,
	eCEW_CH5_NORMAL_ITEM_UPGRADE,
	eCEW_CH5_UNIQUE_ITEM_UPGRADE,
	eCEW_CH5_SELECT_ALLIGNMENT,
	
	eCEW_USE_BARD_TITLE,
	eCEW_ASK_DUEL_RECORD_INFO,
	eCEW_CANCEL_TRY_DUEL,	//	결투 신청 취소
	eCEW_ASK_DUEL_RANKING,	//	결투 순위 정보 요청
	
	eCEW_CHANGE_MINIPET_SKILL_ON_OFF,
	eCEW_USE_MINIPET_AWAKEN_ITEM,
	eCEW_TRY_TO_GUILD_DUNGEN,
	eCEW_ACTIVE_GUILD_MASTER_GRACE,
	eCEW_DISJOINTING_ITEM,
	eCEW_FEED_ALL_ITEM_TO_MINIPET,

	eCEW_RESET_PET_NAME,
	eCEW_MIX_MINIPET,
	eCEW_RESTRAINT_MINIPET,
	eCEW_BUY_GOLD_BAR,		// 금괴사기!! 09.08.20  
	eCEW_ETERNAL_ITEM_UPGRADE,	// 이터널 무기 강화!	09.08.25
	eCEW_REALLY_ITEM_UPGRADE,	// 강화할껀데? 재료좀.. 09.08.27
	eCEW_DRAG_ITEM_DIVIDE,		// 스택 아이템 나누기!! 09.08.31
	eCEW_USE_SHOP,				// 상점 이용하기
	eCEW_USE_BANK,				// 은행 이용하기
	eCEW_COPY_PREFIX,			// 접두사 복사하기.
	eCEW_CUT_PREFIX,			// 접두사 잘라내기.
	eCEW_MINIPET_FEED,			// 미니펫 먹이
	eCEW_RESET_MINIPET_SKILL,			// 미니펫 스킬 초기화
	eCEW_SET_MINIPET_TYPE,			// 미니펫 타입 변경

	eCEW_TRANS_AVATAR_TO_GVG_SERVER,	// 현재 캐릭터 GVG 서버로 전송
	eCEW_TRANS_GUILD_TO_GVG_SERVER,		// 현재 길드 GVG 서버로 전송
};

struct	CG_ETC_WORK
{
	cMSG_BASE_TYPE	base;

	WORD	wWork;
	WORD	awValue[12];
};

struct	CG_XTRAP_PACKET
{
	cMSG_BASE_TYPE	base;
	
	char	aBuffer[128];
};

//┃Client->Server
//┃커뮤니티
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃예약 작업들

//
//	캐릭터 복사 예약
struct	BW_COPY_ACTOR
{
	cMSG_BASE_TYPE	base;

	WORD			wLord;
	WORD			wTarget;
	WORD			wUpkeepTime;
	int				iHP;
	int				iX,iY;
};

//
//	더미 생성 예약
struct	BW_MAKE_DUMMY
{
	cMSG_BASE_TYPE	base;

	WORD			wLord;
	WORD			wUpkeepTime,wAddHPPercentage;
	int				iX,iY;
};

//
//	더미 생성 예약
struct	BW_MAKE_ILLUSION
{
	cMSG_BASE_TYPE	base;

	DWORD			dwLordHashCode;
	WORD			wLord;
	WORD			wUpkeepTime;
	WORD			wIsFirstTarget;
	WORD			wJob;
	WORD			wSkill;

	int				iHP;
	int				iX,iY;
};


struct	BW_DISPLACEMENT
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster,wTarget;
	WORD			wIsRelease;
};

struct	BW_CHARMING
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster,wTarget,wTime;
	char			strCasterName[dNAME_LENGTH],strTargetName[dNAME_LENGTH];
	WORD			wIsRelease;
};

struct	BW_INTERVAL_SHOOTER
{
	cMSG_BASE_TYPE	base;

	WORD			wSkillIndex,wCaster,wTarget;
	DWORD			dwCasterNameHashCode,dwTargetNameHashCode;
};

//
//	게임 나가기
struct	BW_EXIT_GAME
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	char			strName[dNAME_LENGTH];
};

//
//	게임 나가기
struct	BW_READY_TO_EXIT
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	char			strName[dNAME_LENGTH];
};

//
//	액터 죽이기
struct	BW_DEATH_ACTOR
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial,wKiller;
	int				iZoneSerial;

	WORD			bf1IsReleaseSummon	:	1;
	WORD			bf1IsByKarma		:	1;
};

#define	eCEW_ACTION_SKILL_TO_GROUND	29324

struct	BW_MONSTER_TAME
{
	cMSG_BASE_TYPE	base;

	char			strCaster[dNAME_LENGTH];
	WORD			wTamedLevel,wTamedLoyalty,wTargetFieldSerial,wTargetZoneSerial;
};

struct BW_GENERATE_PET
{
	cMSG_BASE_TYPE	base;

	char			strTamer[dNAME_LENGTH];
	WORD			bf4PetIndex	:	4;
	WORD			bf1IsUnseal	:	1;
};

struct BW_GENERATE_SUMMON_BEAST
{
	cMSG_BASE_TYPE	base;

	char			strTamer[dNAME_LENGTH];
	int				iRemainHP;
	
	WORD			bf10Level	:	10;
	WORD			bf2Type		:	2;
	WORD			bf2Grade	:	2;
	WORD			bf1IsSummon	:	1;
	WORD			wX,wY;
};

struct BW_MAKE_SKILL_ZONE
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster;
	DWORD			dwCasterNameHashCode;
	WORD			wSkill,wSkillLevel;
};


struct BW_EXIT_GAME_FOR_MOVE_FIELD
{
	cMSG_BASE_TYPE		base;
	WORD				wSerial;
	char				strName[dNAME_LENGTH];
};


//┃예약 작업들
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server->Client
//┃게임 서버
//  ━━>게임 참가 결과
#define dRESULT_CONNECT_SUCCESS						0x000	//	성공
#define dRESULT_CONNECT_FAIL						0x001	//	인증실패
#define dRESULT_CONNECT_FULL						0x002	//	인원초과

#define	dMAP_FILE_NAME_SIZE							64
#define	dIP_LENGTH									16

#define	dENCRYPT_TABLE_SIZE							93

struct SG_RESULT_CONNECT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;						//	결과
	UTime			serverTime;
	WORD			wFieldSerial,wXorCode,wPacketSerial;
	DWORD			dwSeasonVariable;
	char			strMapName[dMAP_FILE_NAME_SIZE];	//	필드 이름
	BYTE			abEncryptTable[dENCRYPT_TABLE_SIZE];

	char			strGuildName[dGUILD_NAME_LENGTH];	//	길드가 없으면 이 변수는 비어서 보낸다. 그러므로 이 변수는 항상 마지막에 있어야 한다.
};

#define dRESULT_JOIN_FIELD_SUCCESS									0x000	//	성공
#define dRESULT_JOIN_FIELD_FULL										0x001	//	더이상 넣을수 없다. 실제로는 이런 메세지는 나오기 힘들꺼~~ 얼 -_-a
#define dRESULT_JOIN_FIELD_FAILED									0x002	//	실패했다.
#define dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA					0x003	//	데이터에 문제가 있어 실패했다.
#define dRESULT_JOIN_FIELD_FAILED_BY_EXIST_PLAYER					0x004	//	접속이 완전히 끊기지 않았다.
#define dRESULT_JOIN_FIELD_FAILED_BY_NOT_SECRET_DUNGEON_MEMBER		0x005	//	비밀던젼이 사라지거나 파티에서 쫓겨났다.
#define dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_CONNECT				0x006	//	데이터에 문제가 있어 실패했다.
#define dRESULT_JOIN_FIELD_FAILED_BY_TOO_MANY_CONNECTOR				0x007	//	접속 시도가 너무 많다.
#define dRESULT_JOIN_FIELD_FAILED_BY_TRY_CONNECT_TO_INCORRECT_FIELD	0x008	//	엉뚱한 필드로 접속을 시도했다.
#define dRESULT_JOIN_FIELD_FAILED_BY_PROCESS_OFFLINE_TOURNAMENT		0x009	//	오프라인 이벤트 진행중이다.

#define dLIMIT_RECEIVE_EXIST_ACTOR_COUNT_FOR_ROOKIE					50		//	신입-_-에게 보내줄수 있는 캐릭터 정보의 한계 수량-_-?
#define dLIMIT_RECEIVE_EXIST_ACTOR_COUNT_FOR_JOINER					10

struct	SG_RESULT_JOIN
{		// 마지막 수정일 : 09.10.07
	cMSG_BASE_TYPE						base;

	WORD								wResult;				//	결과
	WORD								wSerialInField;			//	필드안에서 플레이어의 시리얼
	DWORD								dwEnchantedMask;

	DWORD								bf1IsGuildBattleField	: 1;
	DWORD								bf1IsOXQuizField		: 1;
	DWORD								bf1IsVillage			: 1;
	DWORD								bf4MinimapDisplayLevel	: 4;
	DWORD								bf3Team					: 3;
	DWORD								bf3OperatorLevel		: 3;			//	운영자 레벨
	DWORD								bf12DeathPeneltySecond	: 12;			//	죽음 페널티 시간
	DWORD								bf1IsProgressEvent		: 1;
	DWORD								bf1IsReadyToSelectGuildMark	: 1;
	DWORD								bf1IsBossZone			: 1;
	DWORD								bf1IsSiegeWarfareField	: 1;
	DWORD								bf1IsGuildPointBattleField	: 1;	
	DWORD								bf1IsNetCafeUser		: 1;		//	31
	DWORD								bf1IsWaitOrgAvatarDataResult	: 1;		//	32

	WORD								bf4RemainGuildStategyPointCount	:	4;
	WORD								bf7DeathPenelty					:	7;
	WORD								bf1IsDuelField					:	1;
	WORD								m_bf3GreateGodGrace				:	3;

	WORD								m_wCurrentField;		//현재 필드
	BYTE								m_bf4GuildHallLevel : 4;
	BYTE								m_bf4Weather		: 4;			// 날씨..
	BYTE								m_bGuildHallShape;

	WORD								m_wRemainSecondForUseGateGlove;
	int									m_iXPos,m_iYPos;
	WORD								m_wLastVillage;			//마지막으로 떠나온 마을

	WORD								m_wBoostExpSecondByTreasureMapEventInServer;
	WORD								m_wBoostGoldSecondByTreasureMapEventInServer;
	WORD								m_wPlayerBoostExpSecondByTreasureMapEvent;
	WORD								m_wPlayerBoostGoldSecondByTreasureMapEvent;
	WORD								m_wRedStonePoint,m_wReversalPowerTime;
	int									m_iHeavenRedStoneCount,m_iHellRedStoneCount,m_iRedDevilRedStoneCount;

	CUpkeepPlayerData					upkeepPlayerData;		//	필드를 이동해도 유지해야 하는 데이터
	WORD								m_wServerSign;
	WORD								m_awRelatedPlaceOfGuild[dMAX_GUILD_VILLAGE_COUNT];
	WORD								m_awSkillCoolTime[c_iCoolTimeSkillCount];
};	//━━게임 참가 성공

struct	SG_FAILED_TO_JOIN
{
	cMSG_BASE_TYPE						base;

	WORD	wResult;
	char	strText[256];
};

struct	SG_PLAYER_INFO
{
	cMSG_BASE_TYPE						base;
	CPlayerSaveDataForClientDefine		playerData;				//	플레이어 데이터

	WORD								m_wExtraInventorySize;
	BYTE								m_aCompleteQuest[dQUEST_BUFFER_SIZE];
	BYTE								m_aCompleteQuest2[dQUEST_BUFFER_EXTEND_SIZE];
	BYTE								m_aCompleteArbeit[dARBEIT_BUFFER_SIZE];
	BYTE								m_aCompleteArbeit2[dARBEIT_BUFFER_SIZE];
	cMiniPetCommon						m_aMiniPet[c_iMiniPetCount];
	WORD								m_awItemCoolTime[dITEM_COOL_TIME_BUFFER_SIZE];

	CItemDefine							m_aExtraInventory[dEXTRA_INVENTORY_SIZE];
	WORD								m_wBonusStatePoint;
	WORD								m_awQuestCountDownTimer[dMAX_PROCESS_QUEST_COUNT];
	WORD								m_wProcessChapterIndex;
	WORD								m_wProcessHighQuestIndex;

	UINT								m_bf5ActiveMiniPet	:	5;
	UINT								m_bf5ActiveMiniPet2	:	5;
	UINT								m_reversalFrameYear	:	5;
	UINT								m_reversalFrameMonth:	4;
	UINT								m_reversalFrameDay	:	5;
	UINT								m_reversalFrameHour	:	5;
	UINT								m_reversalFrameMinute:	6;	//	25

//	2007 11 02 챕터 5 추가를 위해 추가한 데이터들
	WORD								m_bf4CH5StateBonusCount	:	4;
	WORD								m_bf4CH5SkillBonusCount	:	4;
	WORD								m_bf4CH5HPBonusCount	:	4;
	WORD								m_bf4CH5CPBonusCount	:	4;

	WORD								m_bf4CH5PeneltyHPBonusCount		:	4;
	WORD								m_bf4CH5PeneltyCPBonusCount		:	4;
//	08-10-14	거신의 은총을 위해 추가		JBC
	WORD								m_bf3GreateGodGrace				:	3;
	WORD								m_bf3GuildDungeonPlayTime				:	3;

	UINT								m_bf4EatFireMiniPetCount	:	4;
	UINT								m_bf4EatWaterMiniPetCount	:	4;
	UINT								m_bf4EatWindMiniPetCount	:	4;
	UINT								m_bf4EatEarthMiniPetCount	:	4;
	UINT								m_bf4EatLightMiniPetCount	:	4;
	UINT								m_bf4EatDarkMiniPetCount	:	4;	//	24
	UINT								m_bf4RebirthCount			:	4;	//	전생 횟수
	UINT								m_bf4UseSealSphereOfDawnCount:	4;	//	새벽의 봉인구 사용 횟수
};

struct	SG_SUCCESS_TO_JOIN_GAME
{
	cMSG_BASE_TYPE						base;
};

struct	SG_BCS_INFO
{
	cMSG_BASE_TYPE		base;

	WORD				wIndexForBCS;
	char				strBCSIp[dIP_LENGTH];	//	필드 이름
};

//
//	플레이어의 스테이터스 데이터를 업데이트 한다.
struct	SG_UPDATE_PLAYER_STATUS_DATA
{
	cMSG_BASE_TYPE				base;

};

//━━누군가 필드에 난입해 들어 왔다. -_-
struct	SG_JOIN_PLAYER
{
	cMSG_BASE_TYPE				base;

	CSimplePlayerInfo			actorInfo;

	WORD						bf1IsByDeath	:	1;
	WORD						bf1IsByTeleport	:	1;
};

#define	dEXIST_ACTOR_COUNT_FOR_EASL	100

//
//	존재하는 얼라들의 최소한의 정보
struct	SG_MINIMUM_ACTOR_INFO_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	CMinimumActorInfo			aInfoList[dEXIST_ACTOR_COUNT_FOR_EASL];
};

#define	dEXIST_OPERATOR_COUNT_IN_FIELD	20

//
//	운영자들 리스트
struct	SG_OPERATOR_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	COperatorInfo				aOperatorList[dEXIST_OPERATOR_COUNT_IN_FIELD];
};	//	struct	


//
//	존재하는 얼라들의 최소한의 정보
struct	SG_REFIT_ACTORS_POSITION
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	CActorInfoForOuterActor		aInfoList[20];
};	//	struct	

//
//	존재하는 얼라들의 최소한의 정보
struct	SG_EVENT_NPC_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	cEventNpcInfo				aInfoList[dEXIST_ACTOR_COUNT_FOR_EASL];
};	//	struct	SG_EVENT_NPC_LIST

//
//	존재하는 얼라들
struct	SG_SIMPLE_ACTOR_INFO_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	char						aBuffer[256];
};

//
//	얼라정보
struct	SG_SIMPLE_ACTOR_INFO
{
	cMSG_BASE_TYPE				base;

	char						aBuffer[256];
};

//
//	존재하는 얼라들
struct	SG_VERY_SIMPLE_ACTOR_INFO_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	char						aBuffer[256];
};


//━━이미 필드에 있던 넘들..
struct	SG_REGEN_MONSTER
{
	cMSG_BASE_TYPE				base;

	CSimpleMonsterInfo			monsterInfo;
};	//━━이미 필드에 있던 넘들..

//━━이런저런절차로(죽음/필드 이동/팅 등등..) 필드에서 나갔다.
struct	SG_PLAYER_OUT
{
	cMSG_BASE_TYPE				base;

	WORD						wSerial;
};	//━━이런저런절차로(죽음/필드 이동/팅 등등..) 필드에서 나갔다.


//┃게임 서버
//┃Server->Client
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃소환수 관련
struct	SG_SUMMON_BEAST_INFO
{
	cMSG_BASE_TYPE				base;
	
	DWORD						bf4Index	:	4;
	DWORD						bf10Serial	:	10;
	DWORD						bf10Level	:	10;
	DWORD						bf2Grade	:	2;
	DWORD						bf2Type		:	2;
	DWORD						bf1IsUpgrade:	1;
};

struct	SG_RIDING_DOG
{
	cMSG_BASE_TYPE				base;

	WORD						wPosX,wPosY;
	WORD						bf2DogGrade	:	2;
	WORD						bf1IsRiding	:	1;
};
//┃소환수 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃펫 관련
struct	SG_CHANGE_PET_COMMAND
{
	cMSG_BASE_TYPE				base;
	
	WORD						bf2PetIndex		:	2;
	WORD						bf4Command		:	4;
	WORD						bf10ProtectActor:	10;
	WORD						wX,wY;
};

struct	SG_PET_INFO
{
	cMSG_BASE_TYPE				base;
	
	WORD						bf4PetIndex :	4;
	WORD						bf1IsTame	:	1;
	WORD						wPetSerial;
	cPetDefine					petInfo;
	WORD						wMinAttackPower,wMaxAttackPower;
	int							iDefensePower;
};

struct	SG_MAKE_BREEDING_RECORD_BOOK
{
	cMSG_BASE_TYPE				base;
	WORD						wPetIndex;
};

struct	SG_SYNC_PET_INFO
{
	cMSG_BASE_TYPE				base;

	DWORD	bf29MaxHP		:	29;
	DWORD	bf2PetIndex		:	2;
	DWORD	bf1IsSummonBeast:	1;
	WORD	wStrength,wIntelligence;
	WORD	wRemainHP,wMinAttackPower,wMaxAttackPower;
	int		iDefensePower;
};

struct	SG_PET_WAIT_COMMAND
{
	cMSG_BASE_TYPE				base;

	WORD	wPetIndex;
};
//┃펫 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server->Client
//┃이동/액션

class	cP_HIT_INFO
{
public:
	WORD			m_wPhysicalDamage,m_wMagicDamage;	//	데미지
	DWORD			m_dwResultField;			//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션,32 즉사
};	//	class	cP_HIT_INFO

typedef struct 
{
	WORD isRun			:	1;
	WORD isContinue		:	1;
	WORD wPPS			:	14;
} tsMoveInfo;

//
//	이동한다.
struct	SG_MOVE
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wPosX,wPosY;			//	현재 위치
	WORD			wDestPosX,wDestPosY;	//	목표 위치
	tsMoveInfo		moveInfo;				//	속도
};	//	struct	SG_MOVE

enum
{
	eMOVE_RESULT_SUCCESS					,
	eMOVE_RESULT_MOVE_SLAVE					,
	eMOVE_RESULT_FAILED_BY_ACTION			,		//	행동 중인디유.. -o-
	eMOVE_RESULT_FAILED_BY_BLOCKED_POS		,		//	막힌 지역이다.
	eMOVE_RESULT_FAILED_BY_INCORRECT_POS	,		//	좌표에 이상이 있음둥
	eMOVE_RESULT_FAILED_BY_ICY_STALAGMITE	,		//	좌표에 이상이 있음둥
	eMOVE_RESULT_FAILED_BY_STATUS			,		//	상태 이상이다.
};	//	이동 결과

//
//	이동한다.
struct	SG_MOVE_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			isRun			: 1;
	WORD			wPPS			: 15;

	WORD			bf14HP				:	14;
	WORD			bf1MiniPetCharge1	:	1;
	WORD			bf1MiniPetCharge2	:	1;

	short			sCP;
};

//
//	이동 실패
struct	SG_MOVE_FAILED
{
	cMSG_BASE_TYPE	base;

	WORD			bf1IsRun	:	1;
	WORD			bf10PPS		:	10;
	WORD			bf5Result	:	5;
	WORD			wX,wY;					//	위치
};

//
//	플레이어 위치 보정
struct	SG_CORRECT_PLAYER_POSITION
{
	cMSG_BASE_TYPE	base;

	WORD			bf15Serial			:	15;
	WORD			bf1IsInnerTeleport	:	1;
	WORD			wPosX,wPosY;			//	현재 위치
};	//	struct	SG_CORRECT_PLAYER_POSITION

//
//	정지한다.
struct	SG_STOP
{
	cMSG_BASE_TYPE	base;

	WORD			bf12Serial				: 12;				//	누구-_-?
	WORD			bf3Direct				: 3;				//	방향의 최상위 값이 체크되면 뭔가에 부딛혀서 막힌거다.
	WORD			bf1IsChangeOwnerDirect	: 1;				//	방향의 최상위 값이 체크되면 뭔가에 부딛혀서 막힌거다.

	WORD			wPosX,wPosY;			//	현재 위치
};	//	struct	SG_STOP

#define	dMAX_UPDATE_ACTOR_POS_COUNT		50

struct	SG_OUTER_ACTORS_POS_UPDATE
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	CMinimumActorInfo			aInfoList[dMAX_UPDATE_ACTOR_POS_COUNT];
};

//
//	이동 어빌러티 설정(뛰기/걷기) 당사자-_-?에게만..
struct	SG_SET_MOVE_ABILITY
{
	cMSG_BASE_TYPE	base;

	WORD			isRun			: 1;
	WORD			wPPS			: 15;
};

//
//	몬스터나 플레이어한테 액션(공격/마법)을 취한다.
struct	SG_ACTION_TO_ACTOR
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Serial		:	11;		//	누가-_-?
	DWORD			bf11TargetSerial:	11;		//	대상 시리얼
	DWORD			bf7Level		:	7;		//	어빌러티레벨
	DWORD			bf1FPS			:	1;		//	프레임 속도
									//	30;

	WORD			bf11Skill		:	11;
	WORD			bf5FPS			:	5;		//	프레임 속도
									//	16

	WORD			wRange;					//	사정거리
	WORD			wPosX,wPosY;			//	현재 위치

	UINT			iPhysicalDamage;		// 09.09.21
	UINT			iMagicDamage;			// 09.09.21

	DWORD			dwResultField;			//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션,32 즉사
};

#define		dMAX_CONTINUOUS_TARGET_COUNT			12

class	cSimpleDamageInfo		// 
{
public:
	WORD			wActor;					//	누가?
	
	UINT			iPhysicalDamage;
	UINT			iMagicDamage;
	DWORD			dwResultField;			//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션,32 즉사
};

class	cContinuousDamageInfo	: public	cSimpleDamageInfo
{
public:
	DWORD			m_bf6ContinuousShotCount	:	6;	//	6
	DWORD			m_bf6MissCount				:	6;	//	12
	DWORD			m_bf6CriticalCount			:	6;	//	18
	DWORD			m_bf6DoubleCriticalCount	:	6;	//	24
	DWORD			m_bf6HardBlowCount			:	6;	//	30
	DWORD			:0;
};
struct	SG_ACTION_TO_GROUP_STICKED_BIT
{
	cMSG_BASE_TYPE		base;

	DWORD				bf11Serial				:	11;		//	누가-_-?
	DWORD				bf11Skill				:	11;
	DWORD				bf6FPS					:	6;		//	프레임 속도
	DWORD				bf4TargetCount			:	4;
												//	32
	WORD				bf7SkillLevel			:	7;
	WORD				:0;
	WORD				wPosX,wPosY,wRange;			//	현재 위치

	cContinuousDamageInfo	aData[dMAX_CONTINUOUS_TARGET_COUNT];
};

#define	dACTION_TO_GROUP_TARGET_COUNT	32

struct	SG_ACTION_TO_GROUP
{
	cMSG_BASE_TYPE		base;

	DWORD				bf11Serial				:	11;		//	누가-_-?
	DWORD				bf11Skill				:	11;
	DWORD				bf6FPS					:	6;		//	프레임 속도
	DWORD				bf4TargetCount			:	4;
												//	32

	DWORD				bf7ContinuousShotCount	:	7;
	DWORD				bf7MissCount			:	7;
	DWORD				bf7CriticalCount		:	7;
	DWORD				bf7DoubleCriticalCount	:	7;
	DWORD				bf1MiniPetCharge1		:	1;
	DWORD				bf1MiniPetCharge2		:	1;
	DWORD				:0;												//	30

	WORD				bf7SkillLevel			:	7;
	WORD				bf7HardBlowCount		:	7;	//14
	WORD				:0;
	WORD				wPosX,wPosY,wRange;			//	현재 위치

	cSimpleDamageInfo	aData[dACTION_TO_GROUP_TARGET_COUNT];
};

//
//	필드에 마법을 사용한다.
struct	SG_ACTION_TO_GROUND
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;				//	누가-_-?
	WORD			wPosX,wPosY;			//	현재 위치
	WORD			wDestPosX,wDestPosY;	//	목표 위치
	WORD			wLevel;			//	기술
	WORD			wFPS;					//	프레임 속도
	WORD			wRange;					//	사정거리

	WORD			bf12Skill			:	12;
	WORD			bf1MiniPetCharge1	:	1;
	WORD			bf1MiniPetCharge2	:	1;
};

//
//	퀵 스킬
struct	SG_ACTION_QUICK
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;				//	누가-_-?
	WORD			wLevel;			//	기술

	WORD			bf12Skill			:	12;
	WORD			bf1MiniPetCharge1	:	1;
	WORD			bf1MiniPetCharge2	:	1;
};	//	struct	SG_ACTION_QUICK

//
//	근접공격
struct	SG_BASIC_ATTACK
{
	cMSG_BASE_TYPE	base;

	WORD			wPosX,wPosY;			//	공격한 녀석의 위치

	WORD			bf6FPS				:	6;	//6	프레임 속도
	WORD			bf7Level			:	7; //13
	WORD			bf1Skill			:	1;	// 14
	WORD			bf1MiniPetCharge1	:	1;	// 15
	WORD			bf1MiniPetCharge2	:	1; // 16

	DWORD			bf11Serial			:	11;				//11	누가-_-?
	DWORD			bf11TargetSerial	:	11;				//22	누구를-_-?
	DWORD			bf10Skill			:	10;				//32

	UINT			iMagicDamage	;
	UINT			iPhysicalDamage	;

	DWORD			dwResultField;
};

//
//	퀵 액션
struct	SG_QUICK_ACTION
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Serial	:	11;	//	누가-_-?
	DWORD			bf7Level	:	7;	
	DWORD			bf4Direct	:	4;
	DWORD			bf6FPS		:	6;		//	프레임 속도
								//	28

	WORD			bf11Skill			:	11;
	WORD			bf1MiniPetCharge1	:	1;
	WORD			bf1MiniPetCharge2	:	1;

	WORD			wPosX,wPosY;			//	공격한 녀석의 위치
};

//
//	Attack Result
enum
{
	eAR_NOT_LEARNED_SKILL			, 	//	배우지 않는 스킬
	eAR_OK							,	//	OK
	eAR_BUSY						,	//	바빠요
	eAR_CAN_NOT_CLOSE_ATTACK_ON_HILL,	//	언덕 위에서는 근접 공격 못혀
	eAR_LOW_CP						,	//	CP가 모자른다.
	eAR_TOO_DISTANCE				,	//	너무 멀다.
	eAR_IS_CAN_NOT_ATTACK_TARGET	,	//	공격할 수 없는 녀석이다.
	eAR_FAILED						,	//	걍 실패
	eAR_IS_UNDER_TARGET_ULTIMATE_BARRIER	,	//	타겟의 얼티밋 배리어 안이다.
	eAR_REQUIRE_WEAPON				,	//	무기가 필요한 스킬
	eAR_REQUIRE_SHIELD				,	//	방패가 필요한 스킬
	eAR_REQUIRE_BULLET				,	//	탄환이 필요한 스킬
	eAR_NOT_EXIST_SKILL				, 	//	존재하지 않는 스킬
	eAR_TARGET_DEATH				,	//	배우지 않는 스킬
	eAR_IS_INREGULAR_SKILL			,	//	통상적이지 않은 스킬
	eAR_NOT_APT_USED_SKILL_TO_TARGET,	//	타겟에 사용하기 적절하지 않은 스킬이다.
	eAR_REQUIRE_PET					,	
	eAR_REQUIRE_SUMMON_BEAST		,
	eAR_ALREADY_OPEN_ARCA			,
	eAR_CAN_NOT_USE_HIGH_LEVEL_SKILL_BY_CONFUSE	,
	eAR_CAN_NOT_USE_HIGH_LEVEL_SKILL_BY_BERSERK	,
	eAR_CAN_NOT_USE_SKILL_TO_TARGET_OBJECT,
	eAR_CAN_NOT_FIND_TARGET_IN_ATTACK_RANGE,
	eAR_CAN_NOT_APT_SKILL_TO_THIS_STATUS,
	eAR_BLOCKED_OBJECT_EXIST,

	eAR_REQUIRE_POTION				,
	eAR_REQUIRE_FLOWER				,
	eAR_REQUIRE_CANDY				,
	eAR_REQUIRE_DURG				,
	eAR_REQUIRE_TARGET_SKILL		,

	eAR_IS_COOL_TIME				,

	eAR_BORDER_SPECIAL_SKILL		,
	eAR_SET_SPECIAL_SKILL_FIELD		,	//	특수 스킬 필드를 설정했다.
	eAR_CONTINUOUS_ATTACK_BY_VALUE	,	//	연속공격
	eAR_LAND_MARKER					,	//	랜드 마커
	eAR_INTERVAL_SHOOTER			,	//	인터벌 슈터
	eAR_BUNSHINE_ATTACK				,	//	분신공격
	eAR_SCIMITAR_CUTTING			,	//	시미터 커팅
	eAR_BOUNCING_LINEAR				,	//	바운싱 리니어
	eAR_ICY_STALAGMITE				,	//	아이시 스탈라그마이트
	eAR_DOUBLE_TARGET				,
	eAR_CAN_NOT_USE_IN_DUEL_FIELD_SKILL	,	//	결투 필드에서 사용 할 수 없는 스킬이다.
	eAR_GROUP_CONTINUOUS_BY_BIT		,		//	비트그룹연타
	eAR_BOUNCING_MISSILE				,		//	바운싱미사일
	eAR_WIDE_MISSILE				,			// 범위 미사일
};

//
//	스킬 사용 결과
struct	SG_USE_SKILL_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	short			sRemainCP;
};	//	struct	SG_USE_SKILL_RESULT

//
//	연속 공격
struct	SG_CONTINUOUS_HIT_BY_CHARACTER
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Serial				:	11;	//11	누가-_-?
	DWORD			bf11TargetSerial		:	11;	//22	누구를-_-?
	DWORD			bf6FPS					:	6;	//28
	DWORD			bf4MissCount			:	4;	//32	미스 횟수
												

	DWORD			bf7Level				:	7;	//7		어빌러티
	DWORD			bf5AttackCount			:	5;	//12	공격 횟수
	DWORD			bf4CriticalCount		:	4;	//16	크리티컬 수
	DWORD			bf4DoubleCriticalCount	:	4;	//20	더블 크리티컬 수
	DWORD			bf1IsCrushBlow			:	1;	//21	크러쉬 블로우 터졌다.
	DWORD			bf11Skill				:	11;	//32
													

	WORD			wPosX,wPosY;			//	공격한 녀석의 위치
	int				iPhysicalDamage,iMagicDamage;
	WORD			wMinPhysicalDamage,wMinMagicDamage;
	
	DWORD			bf4DancingBlockCount	:	4;	//4
	DWORD			bf4NoActionBlockingCount:	4;	//8
	DWORD			bf4BlockingCount		:	4;	//12
	DWORD			bf1IsInstanceKill		:	1;	//13
	DWORD			bf1IsAttackToShakle		:	1;	//14
	DWORD			bf4HardBlowCount		:	4;	//18
	DWORD			:	0;
};

//
//	연속 공격
struct	SG_CONTINUOUS_HIT_BY_VALUE
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Serial				:	11;	//	누가-_-?
	DWORD			bf11TargetSerial		:	11;	//	누구를-_-?
	DWORD			bf6FPS					:	6;
	DWORD			bf4MissCount			:	4;	//	미스 횟수
												//	32

	DWORD			bf7Level				:	7;	//	어빌러티
	DWORD			bf5AttackCount			:	5;	//	공격 횟수
	DWORD			bf4CriticalCount		:	4;	//	크리티컬 수
	DWORD			bf4DoubleCriticalCount	:	4;	//	더블 크리티컬 수
	DWORD			bf1IsCrushBlow			:	1;	//	크러쉬 블로우 터졌다.
	DWORD			bf11Skill				:	11;
											//	32
												
	WORD			wPosX,wPosY;			//	공격한 녀석의 위치
	int				iPhysicalDamage,iMagicDamage;
	WORD			wMinPhysicalDamage,wMinMagicDamage;

	DWORD			bf4DancingBlockCount	:	4;	//4
	DWORD			bf4NoActionBlockingCount:	4;	//8
	DWORD			bf4BlockingCount		:	4;	//12
	DWORD			bf1IsInstanceKill		:	1;	//13
	DWORD			bf1IsAttackToShakle		:	1;	//14
	DWORD			bf4HardBlowCount		:	4;	//18
	DWORD			:	0;
};


//
//	분신 공격
struct	SG_BUNSHINE_ATTACK
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Serial				:	11;	//	누가-_-?
	DWORD			bf11TargetSerial		:	11;	//	누구를-_-?
	DWORD			bf6FPS					:	6;
	DWORD			bf4CriticalCount		:	4;	//	크리티컬 수
											//	32

	DWORD			bf7Level				:	7;	//	어빌러티
	DWORD			bf5AttackCount			:	5;	//	공격 횟수
	DWORD			bf4MissCount			:	4;	//	미스 횟수
	DWORD			bf4DoubleCriticalCount	:	4;	//	더블 크리티컬 수
	DWORD			bf1IsCrushBlow			:	1;	//	크러쉬 블로우 터졌다.
	DWORD			bf11Skill				:	11;
											//	32

	WORD			wPosX,wPosY;			//	공격한 녀석의 위치
	int				iPhysicalDamage,iMagicDamage;
	WORD			wMinPhysicalDamage,wMinMagicDamage;
	DWORD			bf4DancingBlockCount	:	4;	//4
	DWORD			bf4NoActionBlockingCount:	4;	//8
	DWORD			bf4BlockingCount		:	4;	//12
	DWORD			bf1IsInstanceKill		:	1;	//13
	DWORD			bf1IsAttackToShakle		:	1;	//14
	DWORD			bf4HardBlowCount		:	4;	//18
	DWORD		:0;					
};

//	시미터 커팅
struct	SG_SCIMITAR_CUTTING
{
	cMSG_BASE_TYPE	base;

	WORD			wPosX,wPosY;			//	공격한 녀석의 위치
	WORD			wRange;					//	사정거리
	int				iPhysicalDamage,iMagicDamage;

	DWORD			bf11Serial				:	11;	//	누가-_-?
	DWORD			bf11TargetSerial		:	11;	//	누구를-_-?
	DWORD			bf6FPS					:	6;
	DWORD			bf4MissCount			:	4;	//	미스 횟수
											//	32

	DWORD			bf7Level				:	7;	//	어빌러티
	DWORD			bf5AttackCount			:	5;	//	공격 횟수
	DWORD			bf4CriticalCount		:	4;	//	크리티컬 수
	DWORD			bf4DoubleCriticalCount	:	4;	//	더블 크리티컬 수
	DWORD			bf1IsCrushBlow			:	1;	//	크러쉬 블로우 터졌다.
	DWORD			bf11Skill				:	11;
											//	32

	WORD			wMinPhysicalDamage,wMinMagicDamage;
	DWORD			bf4DancingBlockCount	:	4;	//4
	DWORD			bf4NoActionBlockingCount:	4;	//8
	DWORD			bf4BlockingCount		:	4;	//12
	DWORD			bf1IsInstanceKill		:	1;	//13
	DWORD			bf1IsAttackToShakle		:	1;	//14
	DWORD			bf4HardBlowCount		:	4;	//18
	DWORD		:0;	
};

//
//	연속 공격
struct	SG_HIT_CHANED_DAMAGE
{
	cMSG_BASE_TYPE	base;

//--------------------------------------------------------
	DWORD			bf5AttackCount			:	5;	//	공격 횟수
	DWORD			bf5MissCount			:	5;	//	미스 횟수
	DWORD			bf5CriticalCount		:	5;	//	크리티컬 수
	DWORD			bf5DoubleCriticalCount	:	5;	//	더블 크리티컬 수
	DWORD			bf5DancingBlockCount	:	5;
	DWORD			bf4NoActionBlockingCount:	4;

	DWORD			bf1IsCrushBlow			:	1;	//	크러쉬 블로우 터졌다.
	DWORD			bf1IsAttackToShakle		:	1;
	DWORD			bf1IsInstanceKill		:	1;

//-------------------------------------------------------- 32bit
	DWORD			bf11Skill				:	11;
	DWORD			bf5BlockingCount		:	5;
	DWORD			bf5ShotGap				:	5;
	DWORD			bf11TargetSerial		:	11;	//	누구를-_-?
//-------------------------------------------------------- 32bit

	WORD			bf5HardBlowCount				:5;	//5
	WORD			:0;
	int				iPhysicalDamage,iMagicDamage;
	WORD			wMinPhysicalDamage,wMinMagicDamage;
	WORD			wCaster;
};

//
//	랜드 마커 공격
struct	SG_LAND_MARKER
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;					//	누가-_-?
	WORD			wPosX,wPosY;				//	공격한 녀석의 위치
	WORD			wTargetPosX,wTargetPosY;	//	타격 위치
	WORD			wSkill,wLevel;			//	어빌러티
	BYTE			bFPS,bShotCount;		//	FPS와 샷 카운트
};	//	struct	

//	인터벌 슈터 발사
struct	SG_STRIKE_INTERVAL_SHOOTER
{
	cMSG_BASE_TYPE	base;

	WORD			wTargetSerial;		//	누구를-_-?
	WORD			wPosX,wPosY;		//	공격 당한 녀석의 위치
	WORD			wSkill;				//	어빌러티
	WORD			wShotCount;			//	FPS와 샷 카운트
	int				iPhysicalDamage,iMagicDamage;
};

struct	SG_CAST_INTERVAL_SHOOTER
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;		//	누가-_-?
	WORD			wPosX,wPosY;	//	공격한 녀석의 위치
	WORD			wSkill,wLevel;	//	어빌러티
	BYTE			bFPS,bShotCount;//	FPS,샷 카운트
};

class	cWideHealE
{
public:
	WORD			wActor;
	WORD			wHealPoint;
};

struct	SG_WIDE_HEAL
{
public:
	cMSG_BASE_TYPE	base;

	WORD			wCount,wSkill;

	cWideHealE		aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};

class	cWIDE_AREA_DAMAGE_E
{
public:
	WORD			wActor;					//	누가?
	WORD			wPhysicalDamage,wMagicDamage;	//	데미지
	DWORD			dwResultField;			//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션,32 즉사
};

class	CWideAreaDamageInfo
{
public:
	cWIDE_AREA_DAMAGE_E	aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};

//
//	광역 데미지
struct	SG_WIDE_AREA_DAMAGE
{
	cMSG_BASE_TYPE		base;

	WORD				bf6Count		: 6;
	WORD				bf1IsDamagePool	: 1;
	WORD				bf7FPS			: 7;	//14

	DWORD				bf12Caster		: 12;
	DWORD				bf12Skill		: 12;
	DWORD				bf8Level		: 8;

	short				sRemainCP;	//	CP남은거

	cWIDE_AREA_DAMAGE_E	aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};

struct	SG_TITLE_EFFECT
{
	cMSG_BASE_TYPE		base;

	WORD				wTitle;
	WORD				wTargetCount;
	WORD				awTargetList[100];
};

struct	SG_XTRAP_PACKET
{
	cMSG_BASE_TYPE		base;
	
	char				aBuffer[128];
};

struct SG_UPDATE_PACKET_ENCRYT_CODE
{
	cMSG_BASE_TYPE	base;

	WORD			wNewTable;
	BYTE			abEncryptTable[dENCRYPT_TABLE_SIZE];
};

class	cWideAideAttackResultE
{
public:
	WORD				m_bf12Target	:	12;
	WORD				m_bf1IsHit		:	1;
};

struct	SG_WIDE_AID_ATTACK_RESULT
{

	cMSG_BASE_TYPE			base;

	DWORD					bf6Count		: 6;
	DWORD					bf12Skill		: 12;
	DWORD					bf12Caster		: 12;

	short					sRemainCP;

	cWideAideAttackResultE	aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};

struct	SG_GUARDIAN_POST
{
	cMSG_BASE_TYPE		base;

	DWORD				bf12Caster	: 12;
	DWORD				bf12Skill	: 12;
	DWORD				bf8Level	: 8;
	WORD				wRegSerial;
	WORD				wX,wY;
	WORD				wImage;			//	이게 0xffff이면 포스트 사라지기
	WORD				wTargetCount;	//	이게 0이면 걍 포스트 심기만 한다.
	short				sCP;

	cWIDE_AREA_DAMAGE_E	aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];	//	대미지 입었다.
};

struct	SG_FIRST_AID
{
	cMSG_BASE_TYPE		base;

	WORD				wSerial;
	int					iFirstAidHP;
};

struct	SG_REVIVE
{
	cMSG_BASE_TYPE		base;

	WORD				wSerial;
};

struct	SG_UPDATE_BOOST_COUNT
{
	cMSG_BASE_TYPE		base;

	WORD				bf3UseBoostCount		:	3;
	WORD				bf5RemainPet1BoostCount	:	5;
	WORD				bf5RemainPet2BoostCount	:	5;
};


#define	dMAX_TARGET_FOR_DAMAGE_TO_ACTOR_BY_KARMA	64

class	CDamageToActorByKarmaE
{
public:
	WORD			wActor,wDamage;	//	누가? 얼마나?
};

//
//	카르마에 의한 대미지 입히기
struct	SG_DAMAGE_TO_ACTOR_BY_KARMA
{
	cMSG_BASE_TYPE			base;

	WORD					wCount;
	WORD					wDamageType;

	CDamageToActorByKarmaE	aData[dMAX_TARGET_FOR_DAMAGE_TO_ACTOR_BY_KARMA];
};	//	struct	SG_WIDE_AREA_DAMAGE

#define	dMAX_TARGET_FOR_WIDE_AREA_EFFECT	32

//
//	타이틀 획득
struct	SG_GET_TITLE
{
	cMSG_BASE_TYPE			base;

	WORD					wSlot;
	BYTE					bTitle,bLevel;
};

//
//	타이틀 제거
struct	SG_REMOVE_TITLE
{
	cMSG_BASE_TYPE			base;

	WORD					wSlot;
	BYTE					bTitle,bLevel;
};

struct	SG_START_COUNTDOWN
{
	cMSG_BASE_TYPE			base;
	WORD					wTime;
};

//	OX 퀴즈 시작
struct	SG_BEGIN_QUIZ_EVENT
{
	cMSG_BASE_TYPE			base;
	WORD	wType;
	WORD	wCommerCount;

};

struct	SG_EVENT_TIME_UPDATE
{
	cMSG_BASE_TYPE	base;

	DWORD	adwEventTimeActiveBuffer[dEVENT_TIME_BUFFER_SIZE];
	int	iCount;
};

//	OX 퀴즈 시작 시간 공지
struct	SG_ANNOUNCE_QUIZ_EVENT
{
	cMSG_BASE_TYPE			base;

	WORD	wYear,wMonth,wDay,wHour,wMinute;
	WORD	wQuizGateField,wIsTest;
	WORD	wType;
	char	strQuizEventName[128];
};

struct	SG_OX_QUIZ_RESULT
{
	cMSG_BASE_TYPE			base;

	WORD					wSolution;
	WORD					wLoserCount;
	WORD					wSurvivorCount;
};

struct	SG_WORD_QUIZ_RESULT
{
	cMSG_BASE_TYPE			base;

	WORD					wLoserCount;
	WORD					wSurvivorCount;
	WORD					wTop10Count;
	WORD					awTop10Score[10];
	char					strSolution[16];
	char					strTop10List[dNAME_LENGTH*10];
};

#define	dQUIZ_EVENT_LOSER_SLOT_SIZE	128

struct	SG_QUIZ_EVENT_LOSER_LIST
{
	cMSG_BASE_TYPE			base;

	WORD					bf8LoserCount		:	8;
	WORD					bf4QuizType			:	4;
	WORD					bf1IsFinishEvent	:	1;

	WORD					wRound;

	WORD					awLoserList[dQUIZ_EVENT_LOSER_SLOT_SIZE];
};


struct	SG_QUIZ_EVENT_QUESTION
{
	cMSG_BASE_TYPE			base;
	WORD					wWaitCount;
	WORD					bf10QuestionCounter	:	10;
	WORD					bf4QuizType			:	4;

	char					strQuestion[256];
};

struct	SG_END_QUIZ_EVENT
{
	cMSG_BASE_TYPE			base;

	WORD	wIsReturnToVillage;
	WORD	wWinnerCount,wComerCount;
	WORD	wType;
};

struct	SG_CANCEL_QUIZ_EVENT
{
	cMSG_BASE_TYPE			base;

	WORD	wQuizType;
	WORD	wYear,wMonth,wDay,wHour,wMinute;
};

struct	SG_OX_QUIZ_SURVIVOR_COUNT
{
	cMSG_BASE_TYPE			base;

	WORD					wSurvivorCount,wLoserCount;
};

struct	SG_OX_QUIZ_LOSER_REVIVE_BATTLE
{
	cMSG_BASE_TYPE			base;
	
	WORD					wBattleCount,wWinnerCount,wRevivorCount;
};

struct	SG_OX_QUIZ_WINNER_LIST
{
	cMSG_BASE_TYPE			base;

	WORD					wWinnerCount;
	WORD					awWinnerList[100];
};

enum
{
	eBCIR_SUCCESS,
	eBCIR_FAILED,			//	걍 실패
	eBCIR_LOW_CARROT,		//	캐롯이 모자라
	eBCIR_FULL_INVENTORY,	//	인벤토리가 꽉 찼다.
};

struct	SG_BUY_CARROT_SHOP_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	int				iRemainCarrotCount;
	CItemDefine		item;
};

class	cFieldDoorStatus
{
public:
	WORD	bf10Area			:	10;
	WORD	bf1IsSleep			:	1;
	WORD	bf1IsHide			:	1;
	WORD	bf1IsLocked			:	1;
	WORD	bf1IsTrap			:	1;
	WORD	bf1IsClose			:	1;
	WORD	bf1IsActiveByKarma	:	1;
};

struct	SG_DOORS_STATUS_IN_FIELD
{
	cMSG_BASE_TYPE	base;

	WORD				wDoorCount;
	cFieldDoorStatus	aDoors[dMAX_DOOR_IN_MAP];
};

struct	SG_CHANGE_DOOR_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD	bf10Area			:	10;
	WORD	bf1IsSleep			:	1;
	WORD	bf1IsHide			:	1;
	WORD	bf1IsLocked			:	1;
	WORD	bf1IsTrap			:	1;
	WORD	bf1IsClose			:	1;
	WORD	bf1IsActiveByKarma	:	1;
};

//
//	Door Work Message
enum
{
	eDWM_UNLOCK_BY_SKILL,	//	스킬로 열었다.
	eDWM_UNLOCK_BY_KEY,		//	열쇠로 열었다.
	eDWM_UNLOCK_BY_SKILL_FAILED,		//	열기 실패했다.
	eDWM_UNLOCK_BY_ATTACK,		//	사라진 문이다.
	eDWM_UNLOCK_BY_KARMA,		//	사라진 문이다.

	eDWM_DISARM,			//	해체 했다.
	eDWM_DISARM_FAILED,		//	해체 실패 했다.
	eDWM_DISARM_FAILED_AND_EXPLOSION_TRAP,		//	해체 실패 했다.

	eDWM_IS_LOCKED,			//	잠겨 있는 문이다.
	eDWM_IS_UNLOCKED,		//	열려 있는 문이다.
	eDWM_IS_CLOSED,			//	해체 실패 했다.
	eDWM_IS_OPENED,			//	열려 있는 문입니다.
	eDWM_IS_DISARMED,		//	해체되어 있는 문이다.
	eDWM_IS_SLEEP_HIDE,		//	사라진 문이다.

	eDWM_IS_HIDE_DOOR,		//	감춰져 있는 문이다.
	eDWM_IS_ARMED_TRAP,		//	함정이 걸려 있는 문이다.

	eDWM_DO_YOU_WANT_UNLOCK_DOOR_USE_KEY,	//	그 자물쇠 열만한 열쇠가 없다.
	eDWM_CAN_NOT_UNLOCK_DOOR_BY_NOT_EXIST_AVAIL_KEY,
	eDWM_NOT_EXIST_AVAIL_KEY_OR_SKILL_FOR_UNLOCK_DOOR,	//	그 자물쇠 열만한 열쇠가 없다.
	eDWM_CAN_NOT_UNLOCK_DOOR_BY_LOW_UNLOCK_LEVEL,	//	자물쇠 해체 레벨이 낮아서 해체 불가

	eDWM_TRY_DISARM_FAILED_BY_LOW_CP,	//	함정이 발동해서 터졌다.
	eDWM_TRY_DISARM_FAILED_BY_BUSY,	//	현재 상태에서는 함정 해체 작업을 시도 할 수 없다.
	eDWM_TRY_DISARM_FAILED_BY_LOW_SKILL_LEVEL,	//	현재 상태에서는 함정 해체 작업을 시도 할 수 없다.
	eDWM_CAN_NOT_DISARM_TRAP,	//	
	eDWM_EXPLOSION_TRAP_BY_CAN_NOT_DETECT_TRAP,	//	발견 못한 함정이 발동해서 터졌다.
	eDWM_EXPLOSION_TRAP_BY_ATTACK,
	eDWM_TRY_DISARM_BY_SKILL,

	eDWM_TRY_UNLOCK_BY_SKILL,
	eDWM_TRY_UNLOCK_FAILED_BY_LOW_CP,
	eDWM_TRY_UNLOCK_FAILED_BY_BUSY,

	eDWM_OPEN,						//	문이 열렸다.
	eDWM_IS_REQURE_PARTY,			//	들어 가려면 파티를 구성해야 한다.
	eDWM_IS_FULL_SECRET_DUNGEON,	//	비밀 던젼 풀
	eDWM_FAILED_TO_ENTER_THE_SECRET_DUNGEON,
	eDWM_ALREADY_USE_SECRET_DUNGEON,
	eDWM_FAILED_TO_ENTER_THE_SECRET_DUNGEON_BY_LIMIT_LEVEL,
	eDWM_DUMMY,
	eDWM_CAN_NOT_DESTROY_DOOR,
};

struct	SG_DOOR_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wArea;
	WORD			wKey,wSkill;
	WORD			wMessage;

	char			strCasterName[dNAME_LENGTH];
};

struct	SG_BANNER_TEXT
{
	cMSG_BASE_TYPE	base;

	WORD			wBannerSerial;
	char			strText[dBANNER_COMMENT_LENGTH];
};

struct	SG_BANNER_INFO
{
	cMSG_BASE_TYPE		base;

	cBannerSimpleInfo	bannerInfo;
};

struct	SG_BANNER_LIST
{
	cMSG_BASE_TYPE	base;

	BYTE			abBuffer[512];
};

struct	CG_USE_SOUND_OF_LEADERS_BELL
{
	cMSG_BASE_TYPE	base;

	int		iItemSlot;
	WORD	wIsDisplayName;
	WORD	wTest;
	char	strMessage[64];
};

enum
{
	eSoundOfLeadersBellWork_begin,
	eSoundOfLeadersBellWork_move_field,
	eSoundOfLeadersBellWork_end,
};

struct	SG_SOUND_OF_LEADERS_BELL
{
	cMSG_BASE_TYPE	base;

	int		iUpkeepSecond;
	int		iBoostExp,iBoostItemDropChance;
	WORD	wWork,wIsDisplayNameWithSoundOfLeadersBell;

	char	strCaster[dNAME_LENGTH];
	char	strMessage[128];
};

struct	SG_BOOST_POWER_BY_EVENT
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	WORD			wSecond;
	char			strName[dNAME_LENGTH];
};

//
//	Game Server Message
enum
{
	eGSM_TOUCH_EVENT_AREA_SUCCESS,		
	eGSM_TOUCH_EVENT_AREA_FAILED,		
};

struct	SG_GAME_SERVER_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD	wMessage;
};

struct	SG_STRIKE_LIGHTNING_WINDER
{
	cMSG_BASE_TYPE	base;

	WORD	wSkill,wCaster,wTarget,wPhysicalDamage,wMagicDamage,wAbsorbDamage,wRemainTargetHP;
	DWORD	dwResultField;
};
struct	SG_STRIKE_MULTI_MISSILE
{	// 멀티미사일.
	cMSG_BASE_TYPE	base;

	WORD	wSkill,wCaster,wTarget;
	DWORD	dwPhysicalDamage,dwMagicDamage;
	DWORD	dwResultField;
	WORD	wAbsorbDamage,wRemainTargetHP;
};
//
//	광역 데미지
struct	SG_WIDE_AREA_EFFECT
{
	cMSG_BASE_TYPE		base;

	WORD				wCount,wSkill;

	WORD				aData[dMAX_TARGET_FOR_WIDE_AREA_EFFECT];
};


//
//	화면이동 이벤트를 해라.
struct SG_SCREEN_EVENT
{
	cMSG_BASE_TYPE		base;

	WORD				waValue[6];
};

//
//	죽었다.
struct	SG_KILL
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;				//	누가 -o-?
	WORD			bf12DeathPeneltySecond	:	12;
	WORD			bf1IsInstanceKill		:	1;
	WORD			bf1IsReleaseSummon		:	1;
};

//
//	경험치 업데이트
struct	SG_UPDATE_EXP
{
	cMSG_BASE_TYPE	base;

	int				iExp;			//	경험치
	WORD			wSkillExp;
	WORD			bf10Level		:	10;			//	레벨 변화
	WORD			bf1IsRestore	:	1;
};

//
//	경험치 업데이트
struct	SG_UPDATE_PET_EXP
{
	cMSG_BASE_TYPE	base;

	WORD			bf10Level	:	10;			//	레벨 변화
	WORD			bf4PetIndex	:	4;
	int				iExp;			//	경험치
};

struct	SG_CHANGE_PET_NAME
{
	cMSG_BASE_TYPE	base;

	WORD			wPetIndex;
	WORD			wIsMiniPet;
	char			strPetName[dPET_NAME_LENGTH+1];
	int				iResetItemIndex;
};

struct	SG_PARTING_WITH_PET
{
	cMSG_BASE_TYPE	base;

	WORD			wPetIndex;
	WORD			wIsMiniPet;

	WORD			bf1IsRequireMessage	:	1;
};

enum
{
	eUMIM_CREATE,
	eUMIM_UPDATE,
	eUMIM_FEEDING,
	eUMIM_INCREASE_SKILL_LEVEL,
	eUMIM_SEAL_MINIPET,
	eUMIM_UNSEAL_MINIPET,
	eUMIM_USE_POLISHER,
	eUMIM_RESET_MINIPET_NAME,
	eUMIM_EXPIRED,
	eUMIM_ACTIVATE,
	eUMIM_DISACTIVATE,
	eUMIM_FEED_AWAKEN_ITEM,
	eUMIM_MIXED,
	eUMIM_RESET_SKILL,
	eUMIM_FAIL_RESET_SKILL,
	eUMIM_CHANGE_TYPE,
};
struct	SG_MINI_PET_INFO
{
	cMSG_BASE_TYPE	base;

	cMiniPetCommon	miniPet;
	WORD			wPetIndex;
	WORD			wMethod;	//	0- 생성,1 - 그냥 업데이트
	WORD			wIncreaseExp;

	DWORD			bf5ActivePet	:	5;
	WORD			bf5ActivePet2	:	5;
	DWORD			bf1IsEvolution	:	1;
	DWORD			bf4FeedType		:	4;
	DWORD			bf1Charge1		:	1;
	DWORD			bf1Charge2		:	1;
	DWORD			bf1IsUseMiniPetAwakenItem50		:	1;
	DWORD			bf1IsUseMiniPetAwakenItem100	:	1;
};

struct	SG_UPDATE_BASIC_INFORMATION
{
	cMSG_BASE_TYPE	base;

	WORD			wHP;
	short			sCP;
	WORD			wSerial;
};

//
//	레벨이 올랐다!!
struct	SG_LEVEL_UP
{
	cMSG_BASE_TYPE	base;

	WORD			wLevel;
	int				iExp;			//	경험치
	DWORD			dwSkillExp;		//	스킬 경험치

	short			sStrength;		//	힘			5 1,000
	short			sAgility;		//	민첩성		5 1,000
	short			sConstitution;	//	건강		5 1,000
	short			sWisdom;		//	지혜		5 1,000
	short			sIntelligence;	//	지식		5 1,000
	short			sCharisma;		//	카리스마	5 1,000
	short			sLuck;			//	운			5 1,000
	WORD			wLevelPoint;	//	레벨 포인트
};	//	struct	SG_LEVEL_UP

//
//	누군가가 레벨이 올랐다.
struct	SG_LEVEL_UP_OTHER
{
	cMSG_BASE_TYPE	base;

	WORD			wWho;	//	레벨 오른넘.
	WORD			wLevel;
};	//	struct	SG_LEVEL_UP_OTHER

//
//	스탯 증가 시키기 결과
struct	SG_CHANGE_STATE_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wType;
	short			sValue;
	WORD			wLevelPoint;	//	레벨 포인트
};	//	struct	SG_CHANGE_STATE_RESULT

//
//	스탯 증가 시키기 결과
struct	SG_INCREASE_ABILITY_LEVEL
{
	cMSG_BASE_TYPE	base;

	WORD			wAbility,wLevel;	//	어빌러티,레벨
	DWORD			dwSkillExperience;	//	남은 스킬 경험치
};	//struct	SG_INCREASE_ABILITY_LEVEL

//	앉았다.

//
//	몬스터의 정보
struct	SG_VERY_SIMPLE_ACTOR_INFO
{
	cMSG_BASE_TYPE	base;

	BYTE			aBuffer[100];
};

//
//	특정 캐릭터 제거
struct	SG_REMOVE_ACTOR
{
	cMSG_BASE_TYPE	base;

	WORD			bf15Actor	:	15;	//	캐릭터
	WORD			bf1IsHide	:	1;
};

enum
{
	eTR_ASK_OF_CLIENT		,
	eTR_LOW_CP				,	//	-_-;;	CP가 떨어졌다.
	eTR_BY_SKILL			,	//	변신 하는 스킬을 사용했다.
};	//	실패 이유

//
//	변신하던가 말던가.. -o-r~~
struct	SG_TRANSFORMATION
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			bf8TransformationJob		:	8;
	WORD			bf4Reason					:	4;
	WORD			bf1IsRequreOnlyAfterMotion	:	1;

	WORD			wSpentCP;				//	소모 CP
	WORD			wPosX,wPosY;

	BYTE			bWeapon,bShield,bArmor,bWeaponEffect;
	WORD			bf6FPS						:	6;		//	프레임 속도
	WORD			:0;
};	//	struct	SG_TRANSFORMATION

enum
{
	eTRANSFORMATION_SUCCESS,
	eTRANSFORMATION_FAILED_BY_LOW_CP_LEVEL,
	eTRANSFORMATION_FAILED_BY_YET_NOT_COMPLETE_ACTION,
	eTRANSFORMATION_FAILED_BY_SECOND_JOB_SKILL_NOT_FOUND,	//	-_-;;
	eTRANSFORMATION_FAILED_BY_NOT_AVAIL_JOB,	//	-_-;;
	eTRANSFORMATION_FAILED_BY_ETC,	//	-_-;;
};	//	실패 이유

//
//	변신 실패
struct	SG_TRANSFORMATION_FAILED
{
	cMSG_BASE_TYPE	base;

	WORD			wReason;
	short			sCP;
};	//	struct	SG_TRANSFORMATION

//
//	앉기/서기 토글
struct	SG_TOGGLE_SITDOWN
{
	cMSG_BASE_TYPE	base;

	WORD			bf1IsSitDown : 1;				//	앉는겨-_-?
	WORD			bf15Serial   : 15;				//	누구?
	WORD			wPosX,wPosY;
};	//	struct	SG_TOGGLE_SITDOWN

enum
{
	eSITDOWN_RESULT_OK				,	//	성공
	eSITDOWN_RESULT_FAILED_BY_BATTLE,	//	전투중이라 앉을 수 없다.
};

//
//	앉기/서기 토글
struct	SG_SITDOWN_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;		//	결과
	WORD			wHP;
	short			sCP;		//	HP/CP
	WORD			wValue;			//	-_-?
};	//	struct	SG_TOGGLE_SITDOWN

enum
{
	eREDISTRIBUTE_SKILL,
	eREDISTRIBUTE_STATE,
	eREDISTRIBUTE_SKILL_BY_CH5,
	eREDISTRIBUTE_STATE_BY_CH5,
	eREDISTRIBUTE_FOR_DUEL_SERVER,
};

struct	SG_REDISTRIBUTE
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
};

struct	SG_STOP_BATTLE
{
	cMSG_BASE_TYPE	base;
	
	WORD			bf1IsReleaseFocus	:	1;
};

struct	SG_GG_AUTH_DATA
{
	cMSG_BASE_TYPE	base;

	char	aBuffer[16];
};

struct	SG_WINNING_PRIZE_LOTTO
{
	cMSG_BASE_TYPE	base;

	WORD	wItem;
};

struct	SG_HS_GUID
{
	cMSG_BASE_TYPE	base;

	BYTE	abGuidReqMsg[20];
};

struct	SG_HS_REQ_MSG
{
	cMSG_BASE_TYPE	base;

	BYTE	abReqMsg[160];
};

struct	SG_ARCA_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			bf8Count	:	8;
	WORD			bf1IsFirst	:	1;

	cArcaSimpleInfo	aArcaList[dMAX_ARCA_COUNT_IN_FIELD];
};

struct	SG_ADD_ARCA
{
	cMSG_BASE_TYPE	base;
	cArcaSimpleInfo	arcaInfo;
};

struct	SG_ATTACK_TO_OBJECT
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Attacker		:	11;
	DWORD			bf4ObjectType		:	4;
	DWORD			bf8RemainDurability	:	8;
	DWORD			bf6FPS				:	6;

	WORD			wObjectIndex;
	WORD			wPhysicalDamage,wMagicDamage;
	WORD			wSkill;
	WORD			wPosX,wPosY;
};

enum
{
	eFTW_CATCH,		//	결렸다!
	eFTW_EXPLOSION,	//	터졌다.
	eFTW_DETECTED,	//	탐지 했다.
	eFTW_ACTIVE,		//	활성화 됨
	eFTW_DISACTIVE,	//	비 활성
	eFTW_FAILED_BY_BAD_STATUS,	//	그 작업을 할 상태가 아니다.
	eFTW_FAILED_BY_VALID_TRAP,	//	그 작업을 할 상태가 아니다.
	eFTW_FAILED_DISARM_BY_RANGE,	//	사정거리를 벗어났다.
	eFTW_FAILED_BY_IS_CAN_NOT_DETECT,	
	eFTW_FAILED_BY_IS_DISACTIVE,	
	eFTW_FAILED_BY_IS_CAN_NOT_DISARM_TRAP_BY_LOW_DISARM_LEVEL,	//	해체 스킬이 있는데 레벨이 낮아서 해체 못함
	eFTW_FAILED_BY_NOT_USE_ABLE_SKILL,	//	해체 스킬이 없다.
	eFTW_DISARM_FAILED_AND_EXPLOSION_TRAP,	//	함정 해체 실패하고 폭발했다.
	eFTW_DISARM_BY_SKILL_FAILED,		//	스킬로 함정 해체 시도 했는데 실패 했다.
	eFTW_DISARMED_BY_SKILL,
	eFTW_TRY_DISARM,
	eFTW_TRY_DISARM_FAILED_BY_IS_LOW_CP,
	eFTW_TRY_DISARM_FAILED_BY_BUSY,
};

struct	SG_FIELD_TRAP_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	WORD			wTrapIndex;
	WORD			wValue1;
};

struct	SG_CLOSE_RANGE_FIELD_TRAP_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	WORD			wTrapIndex;
	WORD			wValue1;
	char			strName[dNAME_LENGTH];
};

struct	SG_ENTER_TO_THE_SECRET_DUNGEON
{
	cMSG_BASE_TYPE	base;

	WORD			wGateArea;
	WORD			wGateField;
};

//
//	Etc Work
enum
{
	eEW_SUCCESS_AID_ATTACK,
	eEW_LEVEL_DRAIN,
	eEW_TORTURE_DAMAGE,
	eEW_MAKE_SKILL_ZONE,
	eEW_MAKE_SHAKLE,
	eEW_DAMAGED_BY_SHAKLE,
	eEW_SHAKLE_SKILL,
	eEW_BITE_HANGER,
	eEW_THE_OTHER_SELF_COUNT,
	eEW_MAKE_THE_OTHER_SELF,
	eEW_CRUSH_MAGIC_BOX,
	eEW_SHUT_IN_MAGIC_BOX,
	eEW_CAUGHT,
	eEW_TRANS,
	eEW_DRAIN_BLOOD,
	eEW_MULTIPLE_DAMAGE,
	eEW_SET_MARIONETTE_STATUS,
	eEW_USE_ACTION,
	eEW_CHANGE_SAGE_BOOK_COLOR,	//	세이지의 책 색깔 변경
	eEW_MINIPET_ACTION,

	eEW_ARRAY_INDEX_ZERO_IS_ACTOR_SERIAL_BORDER,	//	 이 위는 0번 인덱스를 액터로 쓴다

	eEW_SHAKE_SCREEN,
	eEW_FAILED_TO_CREATE_SECRET_DUNGEON_BY_LIMIT_LEVEL,
	eEW_PERMISSION_LOGOUT,
	eEW_TRY_LOGOUT,
	eEW_CAN_NOT_EXIT_GAME_DURING_BATTLE,
	eEW_CANCEL_LOGOUT_TIMER_BY_ACTION,
	eEW_FAILED_TO_ENTER_SECRET_DUNGEON_BY_NOT_EXIST_KEY,
	eEW_NOTIFICATION_OF_DEATH_COUNTER,
	eEW_STEAL_GOLD,
	eEW_END_OF_REMAIN_SECOND_FOR_USE_GATE_GLOVE,
	eEW_USE_GATE_GLOVE_REMAIN_SECOND,
	eEW_JUDGE_ITEM,
	eEW_MERGE_ITEM,
	eEW_OPEN_JUDGE_OF_ITEM_WINDOW,
	eEW_OPEN_MERGE_ITEM_PIECE_WINDOW,
	eEW_QUEST_COMPLETE,
	eEW_REMOVE_ITEM_PREFIX,
	eEW_WITHDRAW_CARROT_AGREEMENT_FAILED,
	eEW_WITHDRAW_CARROT_FAILED_BY_OWNED_OVER_COUNT,
	eEW_OPEN_CARROT_SHOP,
	eEW_FAILED_TO_PUT_BANNER,
	eEW_FAILED_TO_BANNER_FULL,
	eEW_SUCCESS_TO_PUT_BANNER,
	eEW_REMOVE_BANNER,
	eEW_FAILED_TO_PUT_BANNER_BY_TO_CLOSE,
	eEW_FAILED_TO_PUT_BANNER_BY_INCORRECT_FIELD,
	eEW_TREASURE_MAP_DESTROYED_WHEN_USE,
	eEW_OPEN_TREASURE_BOX,
	eEW_BOOST_EXP_SECOND_BY_TREASURE_MAP_EVENT,
	eEW_BOOST_GOLD_SECOND_BY_TREASURE_MAP_EVENT,
	eEW_BOOST_EXP_BY_TREASER_MAP_EVENT_TIME_OVER,
	eEW_BOOST_GOLD_BY_TREASER_MAP_EVENT_TIME_OVER,
	eEW_DESTROY_ITEM_BY_MOVE_FIELD,
	eEW_DESTROY_TREASURE_MAP_BY_MOVE_FIELD,
	eEW_FAILED_TO_USE_TREASURE_MAP,
	eEW_FOUND_GOLD_IN_TREASURE_BOX,

	eEW_BERSERK_COUNT_OVER,
	eEW_ALREADY_BERSERK_STATUS,
	eEW_USE_BERSERK_TITLE,
	eEW_TIME_OVER_BERSERK_TIME,
	
	eEW_CHANGE_MAGIC_RESISTANCE,
	eEW_CHANGE_FIRE_RESISTANCE,
	eEW_CHANGE_WATER_RESISTANCE,
	eEW_CHANGE_EARTH_RESISTANCE,
	eEW_CHANGE_WIND_RESISTANCE,
	eEW_CHANGE_LIGHT_RESISTANCE,
	eEW_CHANGE_DARK_RESISTANCE,
	eEW_OPEN_PIGEON_POST_SHOP,
	eEW_CHANGE_NONAME_COUPLE_RING_NAME_TO_LAST_TRADER,
	eEW_TARGET_IS_IN_PREMIUM_ZONE,
	eEW_TARGET_IS_PLAY_GUILD_BATTLE,
	eEW_END_FIRE_WORK_TIME,
	eEW_FIRE_FIRE_WORK,
	eEW_USE_FIRE_WORK,
	eEW_CHANGE_ISPI_SETTING,
	eEW_USE_BOTTOMLESS_BOX_FAILED,
	eEW_USE_BOTTOMLESS_BOX_SUCCESS,
	eEW_SET_CHAPTER,	//	챕터 설정
	eEW_CAN_NOT_RETURN_TO_VILLAGE_DURING_BATTLE,
	eEW_CAN_NOT_TELEPORT_DURING_BATTLE,
	eEW_CAN_NOT_USE_GATE_GLOVE_DURING_BATTLE,
	eEW_CAN_NOT_FIRE_CRACKER_BY_ALREADY_USE_FIRE_CRACKER,
	eEW_FAILED_TO_ENTER_SECRET_DUNGEON_BY_LIMIT_LEVEL,
	eEW_TARGET_INVENTORY_FULL,
	eEW_TARGET_LEVEL_TOO_LOW,
	eEW_TRANS_TO_WEAPON,
	eEW_FINISH_LIGHTNING_WINDER,
	eEW_SET_COOL_TIME,
	eEW_IS_LIMIT_ACTIVE_COUNT_SKILL	,
	eEW_LIMIT_COPY_ACTOR_COUNT,
	eEW_DISPLAY_MAIN_QUEST_EPILOG,

	eEW_CAN_NOT_TRANS_TARGET_TO_FROG,
	eEW_LIMIT_SHUT_IN_MAGIC_BOX_ACTOR_COUNT,

	eEW_WORDQUIZ_SURVIVOR_COUNT,
	eEW_WORD_QUIZ_WINNER_COUNT,
	eEW_ANSWER_RESULT_TO_WORD_QUIZ,
	eEW_SET_REVERSAL_POWER_TIME,
	eEW_CAN_NOT_CREATE_MORE_ILLUSION,

	eEW_SHARE_DAMAGE_BY_DAMAGE_POOL,
	eEW_SET_IMPULSE_TIME,
	eEW_ANIMATE_PARTNER_TIME,
	eEW_FAILED_TO_USE_POS_BY_ALREADY_EXIST_TARGET,
	
	eEW_INLINE_STRENGTH,
	eEW_INLINE_AGILITY,
	eEW_INLINE_CONSTITUTION,
	eEW_INLINE_WISDOM,
	eEW_INLINE_INTELLIGENCE,
	eEW_INLINE_LUCK,

	eEW_DECLINE_RECOVER_HP_EFFICIENT,
	eEW_INTERRUPTING_ARMOR,
	eEW_CAN_NOT_RECALL_ACTOR_IN_JAIL,
	eEW_TARGET_ALREADY_OWNED_PRINCSS_WEAPON,
	eEW_CHANGE_DEFENSIVE_STATUS,

	eEW_HWABYUNG_TIME,
	eEW_PLOT_OF_SHADOW_TIME,
	eEW_LUCKY_SAVE_THROW_ITEM,
	eEW_SLEEP_TARGET,
	
	eEW_PLAY_SKILL_SOUND,
	eEW_DISCONNECT_BY_HACKSHIELD,
	eEW_BEGIN_SERCH_RANDOM_PLACE_BY_GIGA_LIGHTNING,
	eEW_LOCK_ON_BY_GIGA_LIGHTNING,
	eEW_FIRE_GIGA_LIGHTNING,

	eEW_FAILED_DISARM_ACTOR_TRAP_BY_RANGE,
	eEW_FAILED_DISARM_ACTOR_TRAP_BY_IS_CAN_NOT_DETECT,
	eEW_FAILED_DISARM_ACTOR_TRAP_BY_SAME_TEAM,
	eEW_FAILED_DISARM_ACTOR_TRAP,
	eEW_TRY_DISARM_ACTOR_TRAP,
	eEW_TRY_DISARM_ACTOR_TRAP_FAILED_BY_LOW_CP,
	eEW_TRY_DISARM_ACTOR_TRAP_FAILED_BY_BUSY,

	eEW_DISARM_ACTOR_TRAP_FAILED_BY_IS_CAN_NOT_DISARM_TRAP_BY_LOW_DISARM_LEVEL,
	eEW_DISARM_ACTOR_TRAP_FAILED_BY_SKILL_FAILED,

	eEW_MAKE_SMALL_FLAME,	//	작은 불꽃 생성
	eEW_MAKE_FIRE_WALL,		//	파이어 월 생성
	eEW_REMOVE_FIRE_WALL,	//	파이어 월 제거

	eEW_BOSS_BATTLE_EVENT	,	//	보스전 이벤트
	eEW_SET_FOCUS			,	//	포커스 설정
	eEW_CHANGE_GUILD_HALL_LEVEL	,

	eEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_ITEM_COUNT,	//	인덱스,직위,개수
	eEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_GOLD,		//	직위,골드
	eEW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE		,		//	인덱스,사이즈

	eEW_CHANGE_TODAY_WITHDRAW_GUILD_INVENTORY_ITEM_COUNT,
	eEW_CHANGE_TODAY_WITHDRAW_GUILD_INVENTORY_GOLD,

	eEW_ADD_RELATED_PLACE_OF_GUILD,
	eEW_REMOVE_RELATED_PLACE_OF_GUILD,

	eEW_LOW_GOLD_FOR_ADD_VILLAGE_IN_GUILD_RELATED_PLACE_LIST,
	eEW_DO_YOU_WANT_REALLY_BEGIN_GUILD_POINT_BATTLE,

	eEW_RECORD_GUILD_HONOR_POINT,

	eEW_TOO_MANY_SUMMON_GUILD_GUARDIAN,
	eEW_EXIST_GUILD_GUARDIAN_OR_CREST_IN_TO_CLOSE_RANGE,

	eEW_SGG_IS_USE_ABLE_IN_GUILD_BATTLE,
	eEW_SGG_IS_USE_ABLE_DEFENSE_GUILD,
	eEW_RENT_GUILD_PET_COUNT_OVER,

	eEW_CAN_FIND_GUILD_GUARDIAN_IN_SIGHT,
	eEW_REMOVE_NEAREST_GUILD_GUARDIAN	,
	eEW_SUMMON_GUILD_GUARDIAN,

	eEW_OPEN_FEED_GUILD_GUARDIAN_AND_PET_WINDOW,

	eEW_FEED_ITEM_TO_GUILD_PET	,
	eEW_CAN_NOT_FEED_ITEM_BY_OVER_MAX_GUILD_PET_EXP,
	eEW_CAN_NOT_OPEN_GUILD_SHOP_BY_ITEM_ZERO,
	eEW_CHANGE_REMAIN_GUILD_STATEGY_POINT_COUNT,
	eEW_END_GUILD_POINT_BATTLE,
	eEW_CHANGE_OPEN_GUILD_HALL_STATUS,

	eEW_QUESTION_FOR_ADD_RELATED_PLACE_OF_GUILD,
	eEW_CAN_NOT_RETURN_TO_GUILD_HALL_DURING_BATTLE,
	eEW_INCREASE_HUNT_EXP,
	eEW_INCREASE_ITEM_DROP_CHANCE,

	eEW_SWALLOW,	//	삼켜진다.
	eEW_ZOMBIE_BOMB,
	eEW_READY_TO_FIRE_CHARGE_MINIPET_SKILL,
	eEW_ADD_MINIPET_SPARE_SKILL,
	eEW_CHANGE_MINIPET_PLACE,
	eEW_EXTEND_REVERSIBLE_VALIDATE,

//	추가중
	eEW_INCREASE_ALLIGNMENT,
	eEW_DECREASE_ALLIGNMENT,
	eEW_SET_ALLIGNMENT,
	eEW_INCREASE_BONUS_GUILD_POINT,
	eEW_CH5_REVERSE_ITEM_PENELTY,
	eEW_CH5_RECOVER_REVERSE_PENELTY,
	eEW_RECOVER_BONUS_GUILD_POINT_PENELTY,
	eEW_CH5_POINT_BONUS,
	eEW_GUILD_EXP,
	eEW_CH5_RELEASE_ITEM_REVERSION,

	eEW_CH5_RESET_ONE_STATE,
	eEW_CH5_RESET_ONE_SKILL,

	eEW_CH5_ITEM_PREFIX_LEVEL_UP,
	eEW_CH5_UPGRADE_ITEM,

	eEW_UPDATE_RED_STONE_COUNT,
	eEW_UPDATE_BARD_TITLE_COOL_TIME,
	eEW_CANCEL_CAST_BARD_TITLE,
	eEW_USE_BARD_TITLE,
	eEW_REQUIRE_PORTAL_ITEM_TO_USE_THAT_TITLE,

	eEW_OPEN_RELAY_STATION,
	eEW_DUEL_MATCH_APPLICANT_INFO,	//	결투 신청자 수 업데이트
	eEW_CANCEL_TO_TRY_DUEL_BY_CHANGE_PARTY_MEMBER,
	eEW_OPERATE_DUEL_MATCH,	//	결투 처리 
							//	0 시작
							//	1 종료 30초전 
							//	2 종료
							//	3 결투 필드로 이동
							//	4 시작 30초전
							//	5 부활까지 10초 남았다.
							//	6 종료. 킬 카운트

	eEW_DUEL_KILL,			//	누군가를 죽였다
	eEW_OPEN_DUEL_RANKING_WINDOW,	//	결투 랭킹창 열기
	eEW_REMAIN_TIME_FOR_USE_DUEL_BUFF,
	eEW_END_DUEL_BATTLE_BY_OUT_ALL_TARGET_TEAM_MEMBER,	//	상대팀 멤버가 다 나가서 결투가 취소 되었다.
	eEW_EAT_DUEL_FIELD_ARCA_ITEM,	//	결투 필드에서 보물 상자 안에 아이템을 먹음
	eEW_RESET_WEEKLY_DUEL_RECORD	,	//	주간 결투 전적 정보 초기화

	eEW_CAN_NOT_MOVE_FIELD_DURING_BATTLE	,	//	전투중에는 필드 이동 안됨
	eEW_CAN_NOT_PRINCSS_WEAPON_CHANGE		,// 공주 무기변신 스킬을 무기변신 스킬을 상용중인 유저에게 사용할수 없다.,
	eEW_SCREEN_EVENT						,
	eEW_INSTANCE_GUILD_BATTLE	,	//	즉석 길드전 관련 정보

	eEW_DO_YOU_WANT_REALLY_BEGIN_GUILD_DUNGEN,//
	eEW_SCREEN_START_EVENT,//	스크린 이벤트 시작.
	eEW_DISJOINTING_ITEM_RESULT_FLASE,//	아이템 분해 결과 실패 (성공의 경우 아이템 제거에서 출력해 준다.).
	eEW_SET_ITEM_COOL_TIME,
	eEW_REQUIRE_MORE_TIME_FOR_USE_SOUND_OF_LEADERS_BELL,
	eEW_REBIRTH	,	//	전생 했다.
	eEW_RESTRAINT_MINIPET	,	//	미니펫을 먹었다
	eEW_UNDER_EFFECT_TWO_MOUTH_ONE_SOUND_EVENT,	//	이구 동성 이벤트 유효시간이다.
	eEW_UNDER_EFFECT_GOLD_RUSH_EVENT,	//	골드 러쉬 이벤트 유효시간이다.
	eEW_CONFIRM_USE_SOUND_OF_LEADERS_BELL,	//	인도자의 종소리 사용허가?

	eEW_CHANGE_REBIRTH_COUNT,
	eEW_CHANGE_USE_SEAL_SPHERE_OF_DAWN_COUNT,
	eEW_BUY_GOLD_BAR,			//	금괴사기~~	09.08.19
	eEW_BUY_GOLD_BAR_ERR,			//	금괴사기에러~~~	09.08.19	
	eEW_BUY_GOLD_BAR_MESSAGE,		// 금괴를 구입했으니 .. 메세지를~~ 09.08.20
	eEW_OPEN_UPGRADE_SHOP,			// 강화창을 열어라!!. 09.08.25
	eEW_UPGRADE_MATERIALS_ERR,		// 재료가 부족해용~~ 09.08.26
	eEW_UPGRADE_SUCCESS,		// 강화가 성공했따 09.08.26
	eEW_UPGRADE_FAILED,			// 강화가 실패 했다.~09.08.26
	eEW_REALLY_ITEM_UPGRADE,	// 정말로 강화 할꺼에요  ? ??  09.08.27
	eEW_DRAG_ITEM_DIVIDE,		// 스택 아이템 나누기!! 09.08.31
	eEW_DISJOINTING_RESULT,		// 분해 결과 ~~!! 09.09.07
	eEW_CHANGE_WEATHER,			// 기후변화		09.10.07
	eEW_CHANGE_CONSENSUS,						// 교감상태.
	eEW_CHANGE_FLEE,						// 도망..
	eEW_ADD_EXTRA_EFFECT_IMAGE,		// 이팩트추가..
	eEW_OPEN_COPY_OR_CUT_PREFIX_WINDOW,	//	복사창을 열어요~
	eEW_CHANGE_FIGHTING_SPIRIT			,	//	투지 
	eEW_JUMP							,		// 점프..
	eEW_DISPELL							,		// 상태해제
	eEW_END_TRANSFORMATION_MONSTER			,	// 변신이 끝났다.
	eEW_SUCCESS_INCREASE_CARVING_LEVEL,			//	각인레벨증가 성공
	eEW_RESULT_CARVE_LEVEL_MESSAGE			,	//	각인 레벨 시도 결과 메세지
	eEW_ATTACK_DOPPELLGANGER,					//	분신공격 당했다~!
	eEW_DRAW_IMAGE,								//	이미지 그리기
	eEW_RESET_ITEM_COOLTIME,								//	아이템쿨타임 초기화
	eEW_SUMMON_EVENT_MONSTER,								//	이벤트 몬스터 소환
	eEW_OFF_DRAWING_IMAGE,									//	그린 이미지 끄기
	eEW_OPEN_INTERFACE,										//	인터페이스열기

	eEW_EXTRA_BAG_FULL,				//	full!
	eEW_CHANGE_EXTRA_BAG_SIZE,				//	full!
	eEW_SEND_GOLD_LOG,						//	골드 소모 로그
	eEW_TRANS_AVATAR_TO_GVG_RESULT	,	// GVG 서버로 전송 결과
	eEW_TRANS_GUILD_TO_GVG_RESULT	,	// GVG 서버로 길드 전송 결과
	eEW_ASK_GVG_AVATAR_ORG_DATA_RESULT	,	//	GVG 서버 아바타의 본래 아바타 정보 로딩 결과
	eEW_ADD_BIT				,	//	비트추가
	eEW_REMOVE_BIT					,	// 비트 제거
	eEW_OPEN_FAST_PORTAL_WINDOW		,	//	패스트 포탈 윈도우 오픈
};	//	 여기 추가하는 애들은 0번 인덱스를 액터로 쓰는 특이한 경우가 아니다.



struct	CG_SUMMON_EVENT_MONSTER
{
	cMSG_BASE_TYPE	base;

	WORD	wIndex;					// default 0xffff
	WORD	wDropItemIndex;			// default 0xffff


	DWORD	bf10MinLevel	: 10;	//	10			default 1
	DWORD	bf10MaxLevel	: 10;	//	20			default 999
	DWORD	bf5Count		: 5;		//	25		default 0
	DWORD	bf7DropItemCount	:	7;	//	32

	DWORD	bf8ScaleX				: 8;	//8		default 100
	DWORD	bf8ScaleY				: 8;	//16	default 100
	DWORD	bf8MagicResistance		: 8;	//24	default	0
	DWORD	bf8StrangeResistance	: 8;	//32	default	0
	
	DWORD	bf10CorrectAP			:10;	//10	default	100			
	DWORD	bf10CorrectHP			:10;	//20	default	100
	DWORD	bf10CorrectEXP			:10;	//30	default	100
	DWORD	m_bf1IsImmunePhysicalDamage	:	1;	// 31	default	0
	DWORD	m_bf1IsImmuneMagicDamage	:	1;	// 32	default	0


	DWORD	bf8CorrectFinalDamage : 8;	//	8		default	100
	DWORD	bf8DropItemChance		:8;//	16		default	0
	DWORD	bf4DropKind				:4;	//	20		defualt 0
	DWORD	bf1IsDefaultItemDrop	:1;	//	21		default	0
	DWORD	bf1IsDefaultGoldDrop	:1;	//	22		default	0
	DWORD	bf1IsOwner				:1;	//	23		default	0
	DWORD	:0;

	int		aiOption[4];
	char	strName[dNAME_LENGTH];		//	default "no name"
};

struct	SG_ETC_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	int				aiValue[6];
};

struct	SG_CHECK_RANGE_ETC_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	WORD			awValue[6];
};

struct	SG_PLAY_SOUND
{
	cMSG_BASE_TYPE	base;

	char	strSoundFile[128];
};

#define	dMAX_SET_ANM_PACKET_ACTOR_COUNT	64

class	cSetAnmPacketActorInfo
{
public:
	DWORD	m_bf12Actor		:	12;
	DWORD	m_bf5Anm		:	5;
	DWORD	m_bf4Direct		:	4;
	DWORD	m_bf15ReplayCount	:	15;

	void	set(int _iActor,int _iAnm,int _iDirect)
	{
		m_bf12Actor		=	_iActor;
		m_bf5Anm		=	_iAnm;
		m_bf4Direct		=	_iDirect;
	}
};

struct	SG_DUEL_RECORD
{
	cMSG_BASE_TYPE	base;

	cDuelRecord	record;

	WORD		wWeeklyRank,wDailyRank,wTrialGameRank;
};

class	cDuelBattlerInfoForResult
{
public:
	char	m_strName[dNAME_LENGTH];
	int		m_iTrialGameVP;
	int		m_iWeeklyVP;
	int		m_iDuelPoint;
	WORD	m_wKillCount,m_wDeathCount;
};

struct	SG_DUEL_RESULT
{
	cMSG_BASE_TYPE	base;

	cDuelBattlerInfoForResult	m_aBattler[2][5];

	int		iTrialGameVPForWinner;
	int		iVPForWinner;
	int		iDuelPointForWinner;
	int		iVPForDefeater;
	int		iTrialGameVPForDefeater;
	int		iDuelPointForDefeater;
	WORD	wWinTeam;

	WORD	awKillCount[2];
	char	strTrialGameName[32];
};

class	cDuelRankingInfoForClient
{
public:
	char	m_strName[dNAME_LENGTH];
	WORD	m_wJob;

	int		m_iWinPoint;	//	승점
	WORD	m_wBattleCount;	//	전투 횟수
	WORD	m_wWinCount;	//	승리 횟수
	WORD	m_wDefeatCount;	//	패배 횟수
	WORD	m_wDrawCount;	//	무승부 횟수
};

struct	SG_TRY_DUEL_BATTLE
{
	cMSG_BASE_TYPE	base;

	WORD			wApplicantCount,wType;
	char			strTrialGameName[32];
};

struct	SG_DUEL_RANKING
{
	cMSG_BASE_TYPE	base;
	
	WORD			bf10Count		:	10;
	WORD			bf2RankType		:	2;
	char			strTrialGameName[32];
	
	cDuelRankingInfoForClient	aList[100];
};

enum
{
	eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_ENTRY_PARTY,
	eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_SEASON,
	eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_ENTRY,
};

struct	SG_DUEL_MSG
{
	cMSG_BASE_TYPE	base;

	WORD	wMessage,wOutputType;

	char	strText[256];
};

struct	SG_SET_ANM
{
	cMSG_BASE_TYPE	base;

	WORD	wCount;
	cSetAnmPacketActorInfo	aActorList[dMAX_SET_ANM_PACKET_ACTOR_COUNT];
};

enum
{
	eSDM_WARNING_DISAPEAR_DUNGEON,
	eSDM_BEGIN_COUNTDOWN_TIMER,
	eSDM_BEGIN_EVENT,
	eSDM_END_EVENT,
	eSDM_FOCUS_ON_GROUND,
	eSDM_FOCUS_ON_CHARACTER,
	eSDM_RELEASE_FOCUS,
	eSDM_STRESS_MINIMAP,
	eSDM_RELEASE_STRESS_MINIMAP,
	eSDM_COMPLETE,
	eSDM_DISSOLUTION_PARTY,

	eSDM_GET_GUILD_HONOR_POINT,
	eSDM_GUILD_DUNGEON_COMPLETE,
	eSDM_WARNING_DISAPEAR_GUILD_DUNGEON,
};

struct	SG_SECRET_DUNGEON_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wMessage;
	int				iValue1,iValue2;
};

enum
{
	eAW_OPEN,
	eAW_IS_HIDE,
	eAW_IS_TRAP,
	eAW_IS_LOCKED,
	eAW_EXPLOSION_TRAP,
	eAW_IS_FAILED,
	eAW_FULL_INVENTORY,
	eAW_FULL_GOLD,
	eAW_REMOVE,
	eAW_CAN_NOT_UNLOCK,
	eAW_TRY_UNLOCK_BY_SKILL,
	eAW_UNLOCK_BY_SKILL,
	eAW_UNLOCK_BY_KEY,
	eAW_DO_YOU_WANT_UNLOCK_ARCA_BY_USE_KEY,
	eAW_UNLOCK_FAILED,
	eAW_UNLOCK_FAILED_BY_ALREADY_UNLOCKED,
	eAW_UNLOCK_FAILED_BY_LOW_UNLOCK_LEVEL,
	eAW_TRY_UNLOCK_FAILED_BY_LOW_CP,
	eAW_TRY_UNLOCK_FAILED_BY_BUSY,	//	그 스킬을 쓸만한 상황이 아니다.
	eAW_ATTACK_ARCA_FAILED,
	eAW_ATTACK_ARCA_FAILED_BY_LOW_LEVEL,
	eAW_ATTACK_ARCA_FAILED_BY_INVINCIBLE,
	eAW_ATTACK_ARCA,
	eAW_UNLOCK_BY_ATTACK,
	eAW_ATTACK_FAILED_BY_ALREADY_UNLOCKED,

	eAW_FAILED_BY_BAD_STATUS,
	eAW_DISARM_FAILED_BY_ALREADY_DISARMED,
	eAW_DISARM_FAILED_BY_NOT_EXIST_DISARM_SKILL,
	eAW_DISARM_FAILED_AND_EXPLOSION_TRAP,
	eAW_TRY_DISARM_FAILED_BY_LOW_CP,
	eAW_TRY_DISARM_FAILED_BY_BUSY,
	eAW_TRY_DISARM_BY_SKILL,
	eAW_FAILED_DISARM_BY_LOW_DISARM_LEVEL,
	eAW_FAILED_DISARM,
	eAW_DISARMED_BY_SKILL,

	eAW_EXPLOSION_TRAP_BY_NOT_DETECTED,
	eAW_IS_OWNED_OTHER,
};

struct	SG_ARCA_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	WORD			wArcaIndex;
	WORD			wValue1;
};

struct	SG_CLOSE_RANGE_ARCA_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	WORD			wArcaIndex;
	WORD			wValue1;
	char			strName[dNAME_LENGTH];
};

struct	SG_BROKEN_ARCA_ITEM_BY_ATTACK
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	CItemBaseInfo	aItem[dMAX_ITEM_COUNT_IN_ARCA];
};

//	RRS - Result of Reaction Set
enum
{
	eRRS_OK,	//	성공
	eRRS_TOO_MANY_REACTION,	//	이미 너무 많은 리액션이 설정되어 있다.
	eRRS_FAILED,	//	아무튼 실패다.
	eRRS_NOT_USE_REACTION_SKILL_TIME, // 리액션을 사용할 수 없는 시간이다.
};
//
//	리액션 상태 설정
struct	SG_SET_REACTION_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wAbility;		//	어빌러티
	WORD			wIsOn;			//	켜진거?
};	//	struct	SG_SET_REACTION_STATUS

//
//	패시브 스킬들 상태 설정
struct	SG_TURN_ON_PASSIVE_SKILL
{
	cMSG_BASE_TYPE	base;

	BYTE			aPassiveSkill[6];
};	//	struct	SG_TURN_ON_PASSIVE_SKILL

//
//	걍 데미지를 준다.
struct	SG_HIT_THORN_DAMAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wDamage;	// 반격 데미지
	WORD			wTarget;
	short			sRemainCP;
	WORD			wAttacker;
	BYTE			bIsDiplayMagicDamage;
};	//	struct	SG_HIT_THORN_DAMAGE

//
//	데미지 흡수
struct	SG_ABSORB_DAMAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;	// 반격 데미지
	int				iAbsorbHp;
};	//	struct	SG_HIT_THORN_DAMAGE

//
//	특수 스킬 필드 변경
struct	SG_CHANGE_SPECIAL_SKILL_FIELD
{
	cMSG_BASE_TYPE	base;

	WORD			wSpecialSkillField;
	CAbilityDefine	ability;
};	//	struct	SG_CHANGE_SPECIAL_SKILL_FIELD

struct	SG_RELEASE_SHIMMERING_SHIELD
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster,wReceiver;	//	누가 누구한테?
};	//	struct	SG_RELEASE_SHIMMERING_SHIELD

struct	SG_SET_SHIMMERING_SHIELD
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster,wReceiver;

	WORD			bf6ShieldShape : 6;	//	모양
	WORD			bf1IsShield    : 1;	//	방패냐?

	short			sRemainCP;
};	//	struct	SG_SET_SHIMMERING_SHIELD

//	액터 치유
struct	SG_CURE_ACTOR
{
	cMSG_BASE_TYPE	base;

	WORD			wSkill;
	WORD			wActor;
	int				iHealPoint;
};	//	struct	SG_CURE_ACTOR

//	액터 치유
struct	SG_CURE_ACTOR_BY_POTION
{
	cMSG_BASE_TYPE	base;

	WORD			wItem;
	WORD			wActor;
	int				iHealPoint;
};	//	struct	SG_CURE_ACTOR_BY_POTION

//
//	CP충전
struct	SG_CHARGE_CP
{
	cMSG_BASE_TYPE	base;

	short			sCP;
	WORD			wSkill;
};


#define	dMAX_AURA_SKILL_HIT_ACTOR_COUNT		40

//
//	오라 스킬에 맞았다.
struct SG_HIT_AURA_SKILL
{
	cMSG_BASE_TYPE	base;

	WORD			wSkill;
	WORD			bf10Count	:	10;
	WORD			bf6HitImage	:	6;
	WORD			wCaster;	//	쓴넘
	WORD			wRemainCP;
	WORD			aActorList[dMAX_AURA_SKILL_HIT_ACTOR_COUNT];
};

//
//	Displacement Method
enum
{
	eDM_DISPLACEMENT,
	eDM_RELEASE,
};

struct SG_DISPLACEMENT	//	빙의
{
	cMSG_BASE_TYPE	base;

	WORD			wX,wY;
	WORD			wIsDisplacement;
	WORD			wTarget;
};

//
//	머신 띄우기
struct SG_FLOAT_MACHINE
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget;
	WORD			wSkill;
};	//	struct SG_FLOAT_MACHINE

//
//	머신 풀어주기
struct SG_RELEASE_MACHINE
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget;
};	//	struct SG_RELEASE_MACHINE
	
//
//	머신이 미사일을 발사했다.
struct	SG_SHOOT_MACHINE_MISSILE
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster;
	WORD			wTarget;
	WORD			bf12Skill		:	12;
	WORD			m_bf2MinpetOrder:	2;

	cP_HIT_INFO		hitInfo;
};	//	struct	SG_SHOOT_MACHINE_MISSILE

//
//	테일 체이서 발동
struct	SG_FIRE_TAIL_CHASER
{
	cMSG_BASE_TYPE	base;

	WORD			wSkill;
	WORD			wCaster,wTarget;
	WORD			wDamage,wShotCount,wShotPeriod,wCorrectDamage;
	short			sRemainCP;
};	//	struct	SG_FIRE_TAIL_CHASER

//
//
struct	SG_MAKE_ACTION_FORM
{
	cMSG_BASE_TYPE	base;

	CAbilityDefine	ability;
	cP_HIT_INFO		hitInfo;
	WORD			wCaster,wTarget;
	short			sRemainCP;
	WORD			wPosX,wPosY;
};	//	struct	SG_MAKE_ACTION_FORM


#define	dMAX_TARGET_FOR_THRUST_ATTACK	64

//
//
struct	SG_THRUST_ACTORS
{
	cMSG_BASE_TYPE	base;

	WORD				wCount;
	WORD				wSkill;
	WORD				wAxisXPos,wAxisYPos;
	CMinimumActorInfo	aActor[dMAX_TARGET_FOR_THRUST_ATTACK];
};

//
//	밀려나기
struct SG_THRUST
{
	cMSG_BASE_TYPE		base;

	WORD				bf15Serial				:	15;		//	누구-_-?
	WORD				bf1IsBlockedByObject	:	1;
	WORD				wPosX,wPosY;	//	현재 위치
	WORD				bf1IsChangeTargetDirect		:	1;	//1 방향을 타겟으로..
	WORD		:0;
};

//
//
enum
{
	eRR_ARRIVE,
	eRR_CRUSH,
	eRR_BLOCKED,
	eRR_CRUSH_AND_BLOCKED,
};

struct	SG_RUSH
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
	WORD			wArrivePosX,wArrivePosY;
	WORD			wSkill,wLevel;
	WORD			wResult,wFrame;
};

struct	SG_ACTIVE_REACTION_SKILL
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster;
	WORD			wTarget;
	short			sRemainCP;
	WORD			wSkill;
};

//
//	Result of Use Icy Stalagmite 
enum
{
	eRUIS_CAST,
	eRUIS_CANCEL,
	eRUIS_DESTROY,
	eRUIS_RELEASE,
};

struct	SG_ICY_STALAGMITE
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
	WORD			wStatus;
	WORD			wSkill,wLevel;
	short			sRemainCP;
};

//
//
//08-11-17	64비트 확장을 햇다.	JBC
#define	dAMEM_INVINCIBLE				0x0000000000000001
#define	dAMEM_SHIMMERING_SHIELD			0x0000000000000002
#define	dAMEM_FOUNTAIN_BARRIER			0x0000000000000004
#define	dAMEM_MISTIC_FOG				0x0000000000000008
#define	dAMEM_TORNADO_SHIELD			0x0000000000000010
#define	dAMEM_ROCK_BOUNDING				0x0000000000000020
#define	dAMEM_LEVITATE					0x0000000000000040
#define	dAMEM_STONED					0x0000000000000080
#define	dAMEM_COLD						0x0000000000000100
#define	dAMEM_HOT_ENCHANTING			0x0000000000000200
#define	dAMEM_BLIND						0x0000000000000400
#define	dAMEM_BERSERK					0x0000000000000800
#define	dAMEM_SLEEP						0x0000000000001000
#define	dAMEM_CONFUSE					0x0000000000002000
#define	dAMEM_CHARMING					0x0000000000004000
#define	dAMEM_STUN						0x0000000000008000
#define	dAMEM_INVISIBLE					0x0000000000010000
#define	dAMEM_BLUR						0x0000000000020000
#define	dAMEM_FREEZE					0x0000000000040000
#define	dAMEM_BLESS						0x0000000000080000
#define	dAMEM_HEDGER_HEDGING			0x0000000000100000
#define	dAMEM_IS_SITDOWN				0x0000000000200000
#define	dAMEM_PROTECTING_EVIL			0x0000000000400000
#define	dAMEM_NIGHTMARE					0x0000000000800000
#define	dAMEM_PARALYSIS					0x0000000001000000
#define	dAMEM_SPEAR_BLOCKER				0x0000000002000000
#define	dAMEM_DISPLACEMENT				0x0000000004000000
#define	dAMEM_SANCTUARY					0x0000000008000000
#define	dAMEM_HOLD_PERSON				0x0000000010000000
#define	dAMEM_HOLD_MONSTER				0x0000000020000000
#define	dAMEM_EVANGELISM				0x0000000040000000
#define	dAMEM_BOOST						0x0000000080000000
#define	dAMEM_IGNORE_ATTACK_BY_GGG		0x0000000100000000
#define	dAMEM_NEW_BLIND_VERY_SMALL		0x0000000200000000
#define	dAMEM_NEW_BLIND_SMALL			0x0000000400000000
#define	dAMEM_NEW_BLIND_MIDDLE			0x0000000800000000
#define	dAMEM_NEW_BLIND_LARGE			0x0000001000000000
#define	dAMEM_NEW_BLIND_VERY_LARGE		0x0000002000000000
#define	dAMEM_BLACK						0x0000004000000000
//
//	상태가 변했다.
struct	SG_UPDATE_STATUS
{
public:
	cMSG_BASE_TYPE			base;

	WORD					wTarget;
	CActorStatusBitField	status;
};

struct	SG_ACTOR_ENCHANTED_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
	LONGLONG		dwMask;
	uEnchantedImage	enchantedImage;
	WORD			wItemEffect;
	WORD			bf7MagicCarpetShape :	7;
	WORD			bf3CurrentDuelBuff	:	3;
	WORD			bf3CurrentGGGBuff	:	3;
};

enum
{
	eTM_TELEPORT,
	eTM_JUMP,
};

struct	SG_TELEPORT
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
	WORD			wSkill;
	WORD			wX,wY;
	WORD			wMethod;
};

struct	SG_APPLY_SKILL_EXTRA_EFFECT
{		// 마지막 수정일 : 09.10.14
	cMSG_BASE_TYPE	base;

	WORD			wActor;		// 액터
	WORD			wSkill,wLevel;	// 스킬, 레벨
	DWORD			dwKeepTime;		// 강제지속시간.
};

enum
{
	eFUR_NOT_FIND,
	eFUR_FIND,
	eFUR_TARGET_USER_NOT_OWNED_COUPLE_RING,
};

struct	SG_FIND_USER_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wField;
	WORD			wJob;
	char			strTargetName[dNAME_LENGTH];
};

struct	SG_ACTIVE_SKILL_EFFECT
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster,wTarget;
	WORD			wSkill;
	short			sRemainCP;
	WORD			wPhysicalDamage,wMagicDamage;
};

struct	SG_ACTIVE_MIRROR_TOWER
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster,wTarget,wTargetRecoverHP;
	WORD			wSkill;
	short			sRemainCP;
	WORD			wMagicDamage;
};

struct	SG_SIMPLE_HIT
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget;
	int			iPhysicalDamage;
	int			iMagicDamage;
	WORD			bf15RemainHP	:	15;	//15
	WORD			:0;
	WORD			bf7AddHitType	:	7;	//7
	WORD			:0;
};


enum
{
	eSE_VALID,
	eSE_HIT,
	eSE_SKILL,
	eSE_HEAL,
	eSE_CAST,
	eSE_AID_SKILL,
	eSE_CASTER_HIT,
	eSE_EXPLOSION,
};

struct	SG_HIT_DAMAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget,wCaster;
	int				iPhysicalDamage;
	int				iMagicDamage;
	DWORD			dwResultField;
	WORD			wRemainHP;

	WORD			bf11Skill	:	11;
	WORD			bf5ImageType:	5;
};

struct	SG_SKILL_EFFECT
{
	cMSG_BASE_TYPE	base;
	WORD			wTarget;
	WORD			wSkill;
	WORD			bf8Level	: 8;
	WORD			bf8ImageType: 8;

};

struct	SG_PLAY_SKILL_SOUND
{
	cMSG_BASE_TYPE	base;

	WORD	wSkill;
	WORD	wSound;
};

struct	SG_REGEN_HP_BY_AID_SKILL
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget;
	WORD			wRemainHP;
	WORD			wRegenHP;
};

struct	SG_RECOVER_HP_BY_AID_SKILL
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget,wSkill;
	int				iRemainHP;
	WORD			wRecoverHP;
};


//
//	액터에게 스킬 사용
struct	SG_USE_SKILL_TO_ACTOR
{
	cMSG_BASE_TYPE	base;

	WORD			wActor,wTarget;
	short			sRemainCP;
	WORD			wPosX,wPosY;
	cP_HIT_INFO		hitInfo;
	CAbilityDefine	ability;
};

//
//	땅에다 스킬 사용
struct	SG_USE_SKILL_TO_GROUND
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
	WORD			wPosX,wPosY;
	CAbilityDefine	ability;
};

//
//	더미 생성
struct	SG_CREATE_DUMMY
{
	cMSG_BASE_TYPE		base;

	CSimplePlayerInfo	actorInfo;
};

//
//	캐릭터 복사
struct	SG_COPY_ACTOR
{
	cMSG_BASE_TYPE		base;

	CSimplePlayerInfo	actorInfo;
};


//
//	더미 제거
struct	SG_RELEASE_DUMMY
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
};

//
//	보조 마법 캐스팅
struct SG_RECEIVE_AID_MAGIC
{
	cMSG_BASE_TYPE		base;

	WORD				wType;
	short				sValue;
	WORD				wTarget;
	WORD				wSkill;
};	//	struct SG_RECEIVE_AID_MAGIC

#define	dMAX_VILLAGE_COUNT	50

struct SG_READY_TO_WARP_TO_NEAREST_VILLAGE
{
	cMSG_BASE_TYPE		base;

	WORD				wVillageCount;
	WORD				wIsCreatePortal;
	WORD				wSkill;
	WORD				awVillage[dMAX_VILLAGE_COUNT];
};

class	CSimplePortalInfo
{
public:
	DWORD		m_bf7Serial		: 7;
	DWORD		m_bf12Village	: 12;
	DWORD		m_bf10Owner		: 10;
	DWORD		m_bf1IsTownPortal: 1;
	WORD		m_wX,m_wY;
};

struct	SG_REMOVE_PORTAL
{
	cMSG_BASE_TYPE	base;

	WORD			wPortalSerial;
};

struct	SG_ADD_PORTAL
{
	cMSG_BASE_TYPE	base;

	WORD			wPortalSerial;
	WORD			wX,wY,wField;
	WORD			bf12Owner		:	12;
	WORD			bf1IsTownPortal	:	1;
};

#define	dMAX_PORTAL_IN_FIELD	100

struct	SG_PORTAL_LIST
{
	cMSG_BASE_TYPE	base;

	WORD				wPortalCount;
	CSimplePortalInfo	aPortalList[dMAX_PORTAL_IN_FIELD];
};

#define	dMAP_FILE_NAME_SIZE			64
#define	dMOVE_FIELD_RESULT_FAIELD	0

struct	SG_MOVE_FIELD_RESULT
{	// 마지막 수정일 : 09.10.07
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	char			strIP[dIP_SIZE];
	char			strMapName[dMAP_FILE_NAME_SIZE];
	WORD			wFloor,wFieldSerial;
	BYTE			bf4GuildHallLevel : 4;
	BYTE			bf4Weather : 4;				
	BYTE			bGuildHallShape;
	char			strGuildName[dGUILD_NAME_LENGTH];
};

struct	SG_MOVE_FIELD_NOW
{
	cMSG_BASE_TYPE	base;
};

//┃이동/액션
//┃Server->Client
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┃아이템 관련

//
//	필드에 떨어진 아이템들 제거
struct	SG_REMOVE_DROPPED_ITEMS
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;					//	몇개?
	WORD			aRemoveItem[dMAX_OPERATE_DROPPED_ITEM];	//	제거할 아이템들
};	//	struct	SG_REMOVE_DROPPED_ITEMS

//
//	필드에 떨어진 아이템 제거
struct	SG_REMOVE_DROPPED_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wRemoveItem;	//	제거할 아이템
};	//	struct	SG_REMOVE_DROPPED_ITEM

//
//	필드에 떨어진 아이템 제거
struct	SG_CHANGE_DROPPED_ITEM_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wFieldItem;	//	변경할 필드 아이템
	DWORD			dwCount;	//	아직은 변경 될만한게 갯수 밖에 없다. -o-!!!!
};	//	struct	SG_REMOVE_DROPPED_ITEM

//
//	필드에 떨어진 아이템들 제거
struct	SG_DROPPING_ITEMS
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;	//	몇개여?
	cP_DROPPED_ITEM	aDroppedItem[dMAX_OPERATE_DROPPED_ITEM];	//	드랍될 아이템들
};	//	struct	SG_DROPPING_ITEMS

//
//	아이템을 떨군다.
struct	SG_DROPPING_ITEM
{
	cMSG_BASE_TYPE	base;

	cP_DROPPED_ITEM	droppedItem;	//	드랍될 아이템
};	//	struct	SG_DROPPING_ITEM

//
//	장비를 벗는다.
struct	SG_STRIP_EQUIPMENT
{
	cMSG_BASE_TYPE	base;

	WORD			wPart;
};

//
//	
struct	SG_HIDE_EQUIPMENT
{
	cMSG_BASE_TYPE	base;

	WORD			wPart;
	WORD			wMethod;	//	0 감추기 1 보이기
};

//
//	장비벗기 결과
struct	SG_STRIP_EQUIPMENT_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wPart;
	WORD			wResult;
};

//
//	아이템을 줏었다.(아이템을 줍겠다고 신고한넘한테 날려준다. 나머지한테는 걍 사라졌다고 알려준다.)
struct	SG_PICK_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;	//	결과
	CItemDefine		item;
};

//
//	아이템 장비했다고 알려준다.(외관상 변화가 있으면 필드 전체에 전달하고, 아니면 사용한 넘한테만..)
struct	SG_EQUIP_ITEM_RESULT
{
	cMSG_BASE_TYPE		base;

	WORD				wResult;	//	결과
	WORD				wItemSlot;	//	아이템 슬롯
	WORD				wPart;		//	어디다가?
};

//
//	Use Item Result
enum
{
	eUIR_OK								,//	성공!!
	eUIR_DEATH_BY_REACTION				,//	부작용으로 죽게 된다.-_-
	eUIR_ROTTERY_TICKET_KWANG			,//	꽝인디유... -o-
	eUIR_UPGRADE_FAILED					,//	실패.. -o-
	eUIR_SUCCESS_TO_WRITE_BOOK_CATEGORY	,//	카테고리 쓰기 성공
	eUIR_SUCCESS_TO_WRITE_BOOK_PAGE		,//	페이지 쓰기 성공

	eCIR_CH5_REWARD_BROKEN				,
	eCIR_CH5_REWARD_COPY_ITEM			,
	eUIR_SUCCESS_TO_USE_SEAL_SPHERE_OF_DAWN	,	//	새벽의 봉인구 사용 결과 성공

////////////////////////////////////////////////////////////////////
/////	여기보다 작으면 아이템 소모
////////////////////////////////////////////////////////////////////
	eUIR_DECREASE_ITEM_COUNT_BORDER		,//	여기보다 작으면 아이템 소모

	eUIR_ROTTERY_TICKET_OK				,//	당첨.. ㅠ.ㅜ
	eUIR_FAILED							,//	실패.. -o-
	eUIR_NO_MORE_ATTACH_PREFIX			,//	더이상 접두사를 붙일수 없어!!
	eUIR_IS_NOT_WEAPON					,//	무기가 아니다!!
	eUIR_NO_MORE_IMPROVE_WEAPON_DAMAGE	,//	이 이상 무기의 성능을 향상 시킬 수 없다.
	eUIR_IS_NOT_ARMOR					,//	갑옷이 아니다.
	eUIR_NO_MORE_IMPROVE_ARMOR_DEFENSE	,//	더 이상 갑옷의 성능을 향상 시킬 수 없다.
	eUIR_CAN_NOT_USE_ITEM				,//	사용할 수 없는 아이템이다.
	eUIR_CAN_NOT_USE_ON_DEST_ITEM		,//	거기에다 사용 할 수 없다!!
	eUIR_LACK_ABILITY					,//	이 아이템을 사용하기에는 능력이 부족하다.	
	eUIR_STRANGE_OPTION_ITEM			,//	이상한 옵션이 붙어 있다.
	eUIR_IS_NOT_CONSUME_ITEM			,//	소비형 아이템이 아니다.
	eUIR_IS_NOT_CORRECT_STATUS			,//	그 아이템을 쓸 만한 상황이 아녀.. -o-
	eUIR_USELESS_STATUS_TO_USE_ITEM		,//	아이템을 써도 아무런 효과가 없는 상황이다.
	eUIR_CAN_NOT_USE_THIS_FIELD			,//	이 필드에서는 사용 할 수 없다.
	eUIR_INCORRECT_TARGET				,//	대상이 적절치 않다.
	eUIR_INCORRECT_TIMING				,//	부적절한 타이밍
	eUIR_NOT_EMBODY_ITEM				,//	아직 구현이 안된 아이템
	eUIR_WRONG_ITEM_DATA				,//	아이템 데이터가 맞지 않는다.
	eUIR_TOO_FAR						,//	너무 멀다.
	eUIR_FAILED_EVENT20031101			,//	2003년 11월 1일 만든 이벤트의 법칙-_- 때문에 실패
	eUIR_CAN_NOT_MEMORY_FIELD			,//	장소를 기억할 수 없는 필드다.
	eUIR_FAILED_BY_VALID_INVENTORY_SLOT	,//	인벤토리에 빈자리가 없어서 실패

//	짐 모리스의 인챈트 서비스 결과
	eUJMER_OK									,//	짐모리스의 인챈트 서비스 성공
	eUJMER_ITEM_BROKEN							,//	짐모리스의 인챈트 서비스 실패
	eUJMER_ITEM_DESTROYED						,//	완전히 망가졌다.
	eUJMER_FAILED								,//	실패
	eUJMER_CAN_NOT_ATTACH_PREFIX_BY_PREFIX_FULL	,//	접두사가 꽉 차서 더 이상 접두사를 붙일 수 없다.
	eUJMER_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	,//	부숴진 아이템에 접두사를 붙일 수 없다.
	eUJMER_CAN_NOT_ATTACH_PREFIX_ITEM			,//	접두사를 붙일 수 없는 아이템이다.
	eUJMER_CAN_NOT_ATTACH_THAT_PREFIX_ITEM		,//	그 접두사를 붙일 수 없는 아이템이다.
	eUJMER_TOO_LOW_LEVEL						,//	짐모리스의 인챈트 서비스를 이용하기에는 레벨이 너무 낮다.
	eUJMER_IS_EXCLUSIVE_NOT_UNIQUE_ITEM			,//	유니크 아이템이 아닌 아이템을 위한거다./
	eUJMER_IS_EXCLUSIVE_UNIQUE_ITEM				,//	유니크 아이템을 위한거다.
	eUJMER_CAN_NOT_ATTACH_PREFIX_AT_CURSED_ITEM	,//	저주 받은 아이템에는 접두사 추가 불가

//	유니크 아이템에 인챈트 하기 결과
	eUEUIR_OK									,//	유니크 아이템 인챈트 서비스 결과 성공
	eUEUIR_ITEM_BROKEN							,//	뽀게진다.
	eUEUIR_ITEM_DESTROYED						,//	완전히 망가졌다.
	eUEUIR_CAN_NOT_ENCHANT_BY_PREFIX_FULL		,//	유니크 아이템 인챈트 서비스 결과 접두사가 꽉 찼다.
	eUEUIR_CAN_NOT_UPGRADE_PREFIX				,//	더이상 접두사를 업그레이드 할 수 없다.
	eUEUIR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	,//	부숴진 아이템에 접두사를 붙일 수 없다.
	eUEUIR_CAN_NOT_ATTACH_PREFIX_ITEM			,//	접두사를 붙일 수 없는 아이템이다.
	eUEUIR_CAN_NOT_ATTACH_THAT_PREFIX_ITEM		,//	그 접두사를 붙일 수 없는 아이템이다.
	eUEUIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM			,//	유니크 아이템이 아닌 아이템을 위한거다./
	eUEUIR_IS_EXCLUSIVE_UNIQUE_ITEM				,//	유니크 아이템을 위한거다.
	eUEUIR_CAN_NOT_ATTACH_PREFIX_AT_CURSED_ITEM	,//	저주 받은 아이템에는 접두사 추가 불가

//	아이템 복구하기 결과
	eRIR_OK										,//	완벽히 성공
	eRIR_OK_BUT_REMOVE_PREFIX					,//	성공했다. 하지만 접두사가 사라졌다.
	eRIR_FAILED									,//	성공했다. 하지만 접두사가 사라졌다.
	eRIR_IS_NOT_INCORRECT_ITEM					,//	문제 있는 아이템이 아니다.
	eRIR_IS_USELESS_ITEM						,//	소용 없다.
	eRIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM			,//	유니크 아이템이 아닌 아이템을 위한거다./
	eRIR_IS_EXCLUSIVE_UNIQUE_ITEM				,//	유니크 아이템을 위한거다.


	eUPR_PICK_ITEM_IS_COPY_ITEM			,//	그 아이템은 복사 아이템-o-
	eUPR_PICK_ITEM_IS_VALID_ITEM		,//	아이템이 사라졌어요
	eUPR_CAN_NOT_PICK_ITEM_UNDER_DEATH_STATUS	,//	죽은 상태에서는 아이템 획득 불능

	eUPR_PICK_ITEM_IS_PRIVATE_ITEM		,//	그 아이템 임자 있는 아이템이에요.
	eUPR_PICK_THROW_WEAPON1				,//	던진 무기 이다.
	eUPR_PICK_THROW_WEAPON2				,//	던진 무기 이다.
	eUPR_PICK_ITEM_IS_PARTY_ITEM		,//	파티 아이템을 줏었다.
	eUPR_INVENTORY_FULL					,//	인벤토리가 꽉 찼어용.
	eUPR_CAN_NOT_OWN_MORE_GOLD			,//	더 이상의 금액을 소지 할 수 없니다.
	eUPR_QUEST_ITEM						,//	퀘스트 아이템인데 당신은 퀘스트 진행중이 아니여!!

	eUPR_PICK_ITEM_BY_PARTY_MEMBER		,//	퀘스트 아이템인데 당신은 퀘스트 진행중이 아니여!!
	eUPR_FAILED_BY_BAD_STATUS			,//	아이템을 집을 만한 상황이 아니다.


//	아이템 충전 결과
	eUCIR_SUCCESS						,	//	성공
	eUCIR_FAILED						,	//	실패
	eUCIR_DEST_ITEM_NOT_EXTRA_ITEM		,	//	대상 아이템이 엑스트라 아이템이 아니다.
	eUCIR_DEST_ITEM_CAN_NOT_CHARGEABLE	,	//	더 이상 충전 시킬 수 없다.
	eUCIR_MISMATCH_EX_GRADE				,	//	등급이 맞지 않는다.

//	책 아이템에 페이지,카테고리 추가 결과
	eBWR_INCORRECT_BOOK_DATA			,	//	책이 아니다.
	eBWR_DEST_ITEM_IS_NOT_BOOK			,	//	책이 아니다.
	eBWR_IS_NOT_SAME_BOOK				,	//	같은 책이 아니다.
	eBWR_IS_NOT_DEFIEND_BOOK			,	//	등록된 책이 아니다.
	eBWR_SOURCE_ITEM_IS_NOT_BOOK_CATEGORY	,	//	카테고리가 아니다.
	eBWR_SOURCE_ITEM_IS_EXIST_BOOK_CATEGORY	,	//	이미 카테고리가 등록되어 있다.
	eBWR_SOURCE_ITEM_IS_NOT_BOOK_PAGE	,	//	책 페이지가 아니다.
	eBWR_SOURCE_ITEM_IS_EXIST_BOOK_PAGE	,	//	이미 책 페이지가 등록되어 있다.
	
//	DX 아이템 수리 결과
	eRDIR_SUCCESS						,	//	성공
	eRDIR_FAILED						,	//	실패
	eRDIR_CAN_NOT_REPAIR_DEST_ITEM_BY_HIGH_DURABILITY	,	//	내구력이 50%를 넘어서 수리 안된다.
	eRDIR_DEST_ITEM_NOT_DX_ITEM			,	//	대상 아이템이 DX 아이템이 아니다.

//	마법 양탄자 사용결과
	eUMCIR_ALREADY_EXIST_SHAPE			,	//	이미 추가된 모양의 양탄자다.
	eUMCIR_OK							,	//	이미 추가된 모양의 양탄자다.

//	아이템 접두사 제거하기 결과
	eRIPR_OK							,	//	
	eRIPR_NOT_EXIST_PREFIX_ITEM			,	//	아이템 접두사가 없셔
	eRIPR_IS_EXCLUSIVE_NORTMAL_ITEM		,	//	노말 아이템 전용이다.
	eRIPR_IS_EXCLUSIVE_UNIQUE_ITEM		,	//	유니크 아이템 전용이다.

//	기타 결과
	eUAIR_CAN_NOT_OWN_SAME_ITEM			,//	같은 아이템을 중복해서 가질수 없는 아이템이다.
	eUAIR_TOO_MANY_BADGE_ITEM			,//	배지 아이템이 너무 많아요.

	eUER_PICK_WEAPON					,//	먼저 무기를 줏어요.
	eUER_LACK_ABILITY					,//	먼저 무기를 줏어요.
	eUER_NOT_EQUIPMENT_ITEM				,//	장비가 아니다.
	eUER_NOT_RING_PLACE					,//	반지는 제자리에 끼워야 한다.
	eUER_NOT_FIST_RING_PLACE			,//	격반 낄 자리가 아니다.
	eUER_BELT_NOT_FOUND					,//	벨트를 착용해야지
	eUER_INVENTORY_FULL					,//	아이템을 벗을 공간이 없어요.
	eUER_CAN_NOT_EQUIP_EQUIPMENT_WHEN_ACTION,//	행동중에 아이템 착용 불가
	eUER_WRONG_PLACE					,//	적절한 위치가 아니다.

	eUBR_RESULT_FAILED					,//	벨트 사용 실패

	eUSR_PLEASE_PICK_WEAPON				,//	무기를 줏어 주세요.
	eUSR_CAN_NOT_STRIP_EQUIPMENT_WHEN_ACTION	,//	행동중에 아이템 벗기 불가
	eUSR_CAN_NOT_STRIP_SHIELD_STATUS	,
	eUSR_CAN_NOT_STRIP_WEAPON_STATUS	,
	eUSR_CAN_NOT_CHANGE_SHIELD_STATUS	,
	eUSR_CAN_NOT_CHANGE_WEAPON_STATUS	,

	eUDR_FIELD_FULL						,//	더 이상의 아이템을 버릴 수 없어요.
	eUDR_CAN_NOT_DROP_ITEM				,//	버릴 수 없는 아이템
	eUDR_PLEASE_PICK_WEAPON				,//	먼저 아이템을 줏어요.
	eUDR_CAN_NOT_STRIP_EQUIPMENT_WHEN_ACTION	,//	행동중에 아이템 벗기 불가

	eUDIR_CAN_NOT_DESTROY_ITEM			,	//	파괴 불가 아이템
	eUDIR_VAILD_ITEM					,	//	파괴 불가 아이템

	eRIWR_ITEM_SYNC_FAILED				,	//	싱크 실패
	eRIWR_FAILED						,	//	해킹 의혹

	eURR_FAILED							,//	리로드 실패

	eUSGG_FAILED						,	//	길드 가디언 소환 실패
	eUIR_FAILED_BY_ITEM_COOLTIME		,	//	아이템 쿨타임때문에 실패

//	아이템 복사 결과
	eCIR_FAILED_BY_NOT_EQUIPMENT		,	//	리로드 실패
	eUER_WEAR_LIMIT						,	// 더이상 착용 할수 없습니다.		//09.08.12

//	복사한 접두사 확률로 붙이기..
	eCAPR_OK									,	// 성공
	eCAPR_FAILED								,	// 실패!
	eCAPR_EXIST_SAME_PREFIX						,	// 같은 접두사는 붙일수없어
	eCAPR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	,	// 부서진 아이템도 붙일수 없어
	eCAPR_CAN_NOT_ATTACH_PREFIX_ITEM			,	// 접두사를 붙일수없는 아이템이양
	eCAPR_PREFIX_FULL							,	//	더이상 접두사를 붙일수없네요
	eCAPR_CAN_NOT_ATTACH_PREFIX					,	// 그 접두사는 그 아이템에 붙일수없네요
	eCAPR_STRANGE_ITEM							,	// 아이템정보가 이상하네?
//	접두사 잘라내기 
	ePCR_OK										,	// 접두사 잘라내기 성공
	ePCR_INVENTORY_FULL							,	// 인벤토리가 꽉찼다..!
//	접두사 복사 
	ePCOPR_OK									,	// 접두사 복사 성공
	ePCOPR_FAILED								,	// 접두사 복사 실패 ㅠㅡㅠ
	ePCOPR_STRANGE_ITEM							,	// 아이템정보가 이상하네?
	eUIR_BE_MANY_CHARACTER_IN_FIELD				,	//	필드에 몬스터와 엔피시가 많아!
	eUIR_CAN_NOT_USE_IN_FIELD					,	//	현재 필드에서는 사용할 수 없엉!	
	eCLR_CAN_NOT_CARVE_TO_ITEM					,	// 각인을 할 수 없는 아이템!
	eCLR_NOT_ENOUGH_TO_METERIALS				,	// 각인 재료가 충분하지 않아!
	eCLR_INCREASE_SUCCESS						,	//	각인 증가 성공!
	eCLR_INCREASE_FAILED						,	//	각인 증가 실패!
	eCLR_SET_SUCCESS							,	//	각인 레벨 설정 성공!
	eCLR_SET_FAILED								,	//	각인 레벨 설정 실패!
};

//
//	아이템 사용(주위에 녀석들과 사용한넘 한테 아이템 사용했다고 알려준다.)
struct	SG_USE_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;	//	결과
	WORD			wItemSlot;	//	결과
	WORD			wTarget;
};

//
//	아이템을 아이템에 아이템 사용한 결과
struct	SG_USE_ITEM_TO_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;			//	결과
	BYTE			bItemSlot,bTargetItemSlot;	//	결과
};	//	struct	SG_USE_ITEM_TO_ITEM_RESULT

//
//	복권 아이템 당첨됐다.
struct	SG_LOTTERY_TICKET_PRIZE_WINNING
{
	cMSG_BASE_TYPE	base;

	CItemDefine		itemPrize;	//	당첨 아이템
	WORD			wItemSlot;	//	결과
	WORD			bf1IsBySelect	:	1;
};	//	struct	SG_LOTTERY_TICKET_PRIZE_WINNING	

//
//	벨트 아이템 사용 결과.(주위에 녀석들과 사용한넘 한테 아이템 사용했다고 알려준다.)
struct	SG_USE_BELT_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;	//	결과
	WORD			wItemSlot;	//	결과
	WORD			wTarget;
};

//
//	아이템 사용(주위에 녀석들한테 아이템 사용했다고 알려준다.)
struct	SG_USE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wWho;	//	누구? 한테

	CItemBaseInfo	item;	//	어떤걸?

	WORD			bf1IsUseEffectToHero	:	1;	//	쥔공한테 실제 효과 줘라
	WORD			bf1IsDisplayMsg			:	1;	//	
	WORD			:	14;

	char			strCaster[dNAME_LENGTH];

};	//	struct	SG_USE_ITEM_RESULT

//
//	아이템 데이터(되도록이면 안 날렸으면 하는데... -_-)
struct	SG_ITEM_DATA
{
	cMSG_BASE_TYPE		base;

	int					iGold;
	WORD				wExtraInventorySize;
	WORD				wCount;

	cItemDataForRevise	m_aItems[dEQUIPMENT_PART_COUNT+dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE];	//	장비한거
};	//	struct	SG_ITEM_DATA

struct	SG_ITEM_PACK_DATA
{
	cMSG_BASE_TYPE		base;

	WORD				wItem;
	cItemPackInfo		packInfo;
};	//	struct	SG_ITEM_DATA

struct	SG_OTHER_PLAYER_ITEM_DATA
{
	cMSG_BASE_TYPE		base;

	int					iGold;
	WORD				bf6ExtraInventorySize	:	6;
	WORD				bf8ItemCount			:	8;
	char				strId[dID_LENGTH];
	char				strName[dNAME_LENGTH];

	cItemDataForRevise	m_aItems[dEQUIPMENT_PART_COUNT+dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE];	//	장비한거
};	//	struct	SG_OTHER_PLAYER_ITEM_DATA


//
//	아이템 데이터(되도록이면 안 날렸으면 하는데... -_-)
struct	SG_INVENTORY_DATA
{
	cMSG_BASE_TYPE	base;

	CItemDefine		m_aItems[dOWN_ITEM_COUNT];			//	인벤토리 창에거..
};	//	struct	SG_INVENTORY_DATA

//
//	아이템 데이터(되도록이면 안 날렸으면 하는데... -_-)
struct	SG_EQUIPMENT_DATA
{
	cMSG_BASE_TYPE	base;

	CItemDefine		m_aEquip[dEQUIPMENT_PART_COUNT];	//	장비한거
};	//	struct	SG_EQUIPMENT_DATA

struct	SG_CHANGE_APPEARANCE	//	캐릭터의 외관이 변했다.
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	DWORD			bf4Weapon		: 4;	//	16
	DWORD			bf8WeaponEffect : 8;	//	128
	DWORD			bf4Shield		: 4;	//	16
	DWORD			bf4Armor		: 4;	//	16
	DWORD			bf3ArmorPalette : 3;	//	8
};

struct	SG_HIT_ACTION
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
};

class	CDetectingEvilTargetInfo
{
public:
	DWORD			bf16X		:	16;
	DWORD			bf15Y		:	15;
	DWORD			bf1IsMonster:	1;
};

#define	dMAX_DETECTING_EVIL_TARGET_COUNT	100

struct	SG_DETECTING_EVIL
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;

	CDetectingEvilTargetInfo	aTarget[dMAX_DETECTING_EVIL_TARGET_COUNT];
};

struct	SG_MISS
{
	cMSG_BASE_TYPE	base;
	WORD			wActor,wSkill;
};

struct	SG_DROP_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wItemSlot;
	DWORD			dwItemCount;
};

struct	SG_BELT_ITEM_RELOAD_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
};

struct	SG_CHANGE_ITEM_PLACE_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wSrcSlot;
	WORD			wDestSlot;
};

//
//	아이템 수 줄이기
struct	SG_DECREASE_ITEM_COUNT
{
	cMSG_BASE_TYPE	base;

	WORD			wItemSlot;
	WORD			wCount;
};	//	struct	SG_DECREASE_ITEM_COUNT

enum
{
	eIAR_NORMAL,
	eIAR_WITHDRAW_FROM_GUILD_INVENTORY,
	eIAR_CH5_REWARD_COPY,
};

//
//	아이템 추가
struct	SG_ADD_ITEM
{
	cMSG_BASE_TYPE	base;

	CItemDefine		item;
	WORD			wReason;
};	//	struct	SG_ADD_ITEM

enum
{
	eRIR_NORMAL,
	eRIR_STORE_TO_GUILD_INVENTORY,
	eRIR_DISJOINTING
};
//
//	아이템 제거
struct	SG_REMOVE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wItem;
	WORD			bf14Count				:	14;
	WORD			bf1IsSlot				:	1;
	WORD			bf1IsIncludeEquipment	:	1;
	WORD			wReason;
};

enum
{
	eAGR_NORMAL,
	eAGR_STORE_TO_GUILD_INVENTORY,
	eAGR_WITHDRAW_FROM_GUILD_INVENTORY,
};
//
//	골드 추가
struct	SG_ADD_GOLD
{
	cMSG_BASE_TYPE	base;

	int				iAddGold;
	WORD			wReason;
};	//	struct	SG_ADD_GOLD

//
//	아이템의 유니크 시리얼 설정
struct	SG_SET_ITEM_UNIQUE_SERIAL
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot;
	DWORD			dwSerial;
};

//
//	아이템 하나의 정보 업데이트
struct	SG_UPDATE_ITEM_DATA
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot,wIsTwinkle;
	CItemDefine		item;
};

struct	SG_UPDATE_WAY_POINT_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wField,wXPos,wYPos,wWayPointSlot;
};

struct	SG_EXPIRED_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot,wItem;
};

struct	SG_EXPIRED_ITEM_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wItemCount;
	WORD			awItemList[dOWN_ITEM_COUNT+dEQUIPMENT_PART_COUNT+dEXTRA_INVENTORY_SIZE][2];
};

//
//	Use Orb Result
enum
{
	eUOR_SUCCESS,			//	성공
	eUOR_SUCCESS_WRITTEN,	//	성공
	eUOR_INCORRECT_COMMAND,	//	잘못된 명령이다.
	eUOR_NOT_WRITTEN_ORB,	//	기록 되지 않은 오브다
	eUOR_CAN_NOT_WRITTEN_FIELD,	//	기록할 수 없는 필드다.
	eUOR_CAN_NOT_USE_AT_THIS_FIELD,	//	이 필드에서는 쓸수 없다.
};

struct	SG_USE_ORB_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wSlot,wRememberField,wRememberXPos,wRememberYPos;
};

struct	SG_DESTROY_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wSlot;
};

struct	SG_UPDATE_ITEM_DURABILITY
{
	cMSG_BASE_TYPE	base;

	BYTE			bPart,bDurability;
};

struct	SG_REPAIR_ITEM
{
	cMSG_BASE_TYPE	base;

	BYTE			bPart,bDurability;
	DWORD			dwRepairPrice;
};

struct	SG_OPEN_REPAIR_ITEM_WINDOW
{
	cMSG_BASE_TYPE	base;

	WORD			wNpcSerial;
};

struct	SG_OPEN_DISJOINTING_ITEM_WINDOW
{
	cMSG_BASE_TYPE	base;
	
	WORD			wNpcSerial;
};

struct	SG_ITEM_WORK_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
};
//
//	벨트 아이템 리로드
struct	SG_RELOAD_BELT_ITEM
{
	cMSG_BASE_TYPE	base;
};

struct	SG_ITEM_PACK_NAME
{
	cMSG_BASE_TYPE	base;

	WORD	wPackIndex;
	char	strPackName[32];
};

struct	SG_REMOVE_PREFIX
{
	cMSG_BASE_TYPE	base;

	WORD			wPrefixIndex;
	WORD			wUseItem,wDestItem;
};

//┃아이템 관련
//┃Server->Client
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server->Client
//┃커뮤니티

//
//	트레이드 요청
struct	SG_REQUEST_TRADE
{
	cMSG_BASE_TYPE	base;

	char			strAsker[dNAME_LENGTH];
};	//	struct	SG_REQUEST_TRADE

#define	dRANGE_OF_ENABLE_TRADE		(400*400)
//
//	Result of Request Trade
enum
{
	eRRT_DENY,				//	거절했다.
	eRRT_TOO_FAR,			//	너무 멀다.
	eRRT_NOT_EXIST_TARGET,	//	그런넘 없다.
	eRRT_ALREADY_TRADE,		//	딴넘하고 트레이드 중이다.
	eRRT_DEATH_CHARACTER,	//	딴넘하고 트레이드 중이다.
	eRRT_TOO_MANY_TRADE_BOX,//	너무 많은 거래가 이루어 지고 있어요오.. -o-
	eRRT_CAN_NOT_ASK_TRADE_TO_OPERATOR,//	운영자에게는 트레이드를 요청 할 수 없습니다.
	eRRT_TARGET_DENY_COMMUNITY_STATUS,//	대상이 커뮤니티 거부 상태이다.
	eRRT_LOW_LEVEL,
	eRRT_CAN_NOT_TRADE_STATE,
};

//
//	트레이드 요청 결과
struct	SG_REQUEST_TRADE_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wReply;
};

//
//	트레이드 시작
struct	SG_BEGIN_TRADE
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget;
	WORD			wTradeBoxSerial;
	char			strTarget[dNAME_LENGTH];
	WORD			wTargetLevel;
};

//
//	트레이드 아이템 추가
struct	SG_ADD_TRADE_ITEM_BY_ME
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot,wCount;
};

//
//	트레이드 아이템 추가
struct	SG_ADD_TRADE_ITEM_BY_TRADE_MATE
{
	cMSG_BASE_TYPE	base;

	CItemDefine		item;
};

//
//	트레이드 아이템 데이터 수정
struct	SG_CHANGE_TRADE_ITEM_DATA_BY_ME
{
	cMSG_BASE_TYPE	base;

	WORD			wTradeSlot,wSlot,wCount;
};

//
//	트레이드 아이템 데이터 수정
struct	SG_CHANGE_TRADE_ITEM_DATA_BY_TRADE_MATE
{
	cMSG_BASE_TYPE	base;

	WORD			wTradeSlot;
	CItemDefine		item;
};

//
//	트레이드 아이템 추가
struct	SG_SET_TRADE_GOLD
{
	cMSG_BASE_TYPE	base;

	WORD			wOwner;
	int				iGold;
};

//
//	트레이드 아이템 제거
struct	SG_REMOVE_TRADE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wOwner;
	WORD			wSlotIndex;
};

//
//	트레이드 준비
struct	SG_READY_TRADE
{
	cMSG_BASE_TYPE	base;

	WORD			wTrader;
};

//
//	트레이드 수락
struct	SG_PERMIT_TRADE
{
	cMSG_BASE_TYPE	base;

	WORD			wTrader;
};
#define	dTRADE_ITEM_COUNT	18

class	CTradeItemInfo1
{
public:
	WORD	m_wItemSlot;
	WORD	m_wTradeCount;
};

//
//	트레이드
struct	SG_TRADE
{
	cMSG_BASE_TYPE	base;

	WORD			wReceiveItemCount;	//	받을 아이템 카운트
	DWORD			dwReceiveGold,dwGiveGold;	//	받고 줄 아이템
	WORD			wFirstWorkTrader;
	CTradeItemInfo1	aGiveItems[dTRADE_ITEM_COUNT];	//	내가 주는거
	CItemDefine		aReceiveItems[dTRADE_ITEM_COUNT];//	내가 받을거
};

//
//	Trade Message
enum
{
	eTM_TARGET_INVENTORY_FULL,					//	막상 교환 하려니까 상대방 인벤토리에 빈자리가 없다.
	eTM_OWNER_INVENTORY_FULL,					//	막상 교환 하려니까 내 인벤토리에 빈자리가 없다.
	eTM_VALID_INVENTORY_ITEM,					//	빈 인벤토리 아이템이다.
	eTM_ALREADY_ADDED_INVENTORY_ITEM,			//	이미 출가된 인벤토 아이템이다.
	eTM_NOT_TRADE_STATUS,						//	트레이드 중이 아닌디..
	eTM_INCORRECT_TRADE_SLOT,					//	잘못된 트레이드 슬롯 이다.
	eTM_TRADE_BOX_FULL	,						//	트레이드 박스가 꽉 찼다.
	eTM_INCORRECT_TRADE_ITEM_COUNT,				//	트레이드할 아이템의 수가 이상하다.
	eTM_INCORRECT_TRADE_ITEM_SLOT	,			//	잘못된 트레이드 아이템 슬롯
	eTM_LOW_EMPTY_INVENTORY_SLOT_COUNT,			//	거래하기에 인벤토리에 빈슬롯 수가 모자란다.
	eTM_LOW_GOLD		,						//	돈이 모자라는디요?
	eTM_CAN_NOT_OWN_SAME_ITEM	,				//	같은 아이템을 여러개 가질 수 없는 아이템이다.
	eTM_TOO_MANY_BADGE_ITEM		,				//	배지 아이템이 너무 많어!!

	eTM_TRADE_CANCEL_BY_TRADE_MATE,				//	트레이드 상대가 캔슬 했다.
	eTM_TRADE_CANCEL_BY_ME,						//	내가 캔슬했다.
	eTM_TRADE_CANCEL_BY_INVENTORY_DATA_CHANGE,	//	인벤토리에 변화가 있어서 캔슬되었다.
	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS,		//	뭔가 이상해서 캔슬 되었다.

	eTM_TARGET_GOLD_FULL,						//	상대방 골드가 풀이다.
	eTM_TARGET_GOLD_BAR_FULL,					//	상대방 금괴가 풀이다 .

};

//
//	트레이드 메시지
struct	SG_TRADE_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wMessage;
};

//┃아이템 관련
//┃Server->Client
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server->Client
//┃커뮤니티

//
//	채팅 메시지
enum
{
	eCT_TALK							,//	일반 말하기
	eCT_SAY								,//	귓말하기
	eCT_PARTY							,//	파티원 대화
	eCT_GUILD							,//	길드원 대화
	eCT_SHOUT							,//	외치기
	eCT_ADMIN							,//	관리자 전달 메세지
	eCT_RETURN_SAY						,//	내가 날린 세이가 돌아왔다.
	eCT_EVENT_SHOUT						,//	이벤트 샤우팅
	eCT_EVENT_CHAT						,//	이벤트 채팅
	eCT_EVENT_NOTICE_IN_CHAT_BOX		,// 이벤트로 채팅창에 공지
	eCT_EVENT_NOTICE_IN_SYSTEM_MESSAGE	,// 이벤트로 시스템 창에 공지
	eCT_EVENT_NOTICE_IN_NOTICE_BOX		,// 이벤트로 공지 창에 공지
	eCT_EVENT_OX_QUIZ					,// OX퀴즈 공지
	eCT_SYSTEM_MESSAGE					,// OX퀴즈 공지
	eCT_GET_PARTY_ITEM_MESSAGE			,// 파티 아이템 획득 메시지
	eCT_ETERNAL_UPGRADE_MESSAGE			,// 이터널업그레이드 메세지	09.10.08			
};


#define	dCHAT_PERIOD			900

//	SG_EVENT_NOTICE는 아래 세가지 타입만 사용한다.
//	eCT_EVENT_NOTICE_IN_CHAT_BOX		
//	eCT_EVENT_NOTICE_IN_SYSTEM_MESSAGE	
//	eCT_EVENT_NOTICE_IN_NOTICE_BOX		

struct	SG_EVENT_NOTICE
{
	cMSG_BASE_TYPE	base;

	WORD			wNoticeType;				//	메시지 타입
	WORD			wTime;						//	출력 시간 
	WORD			wEffectflickeringly;		//	깜빡임 효과
	char			strMessage[128];	//	메시지
};

struct	SG_CHAT
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Serial		:	11;
	DWORD			bf11PGSerial	:	11;	//	파티 혹은 길드 시리얼
	DWORD			bf6ChatType		:	6;

	char			strMessage[dNAME_LENGTH+dCHAT_LENGTH];	//	메시지
};

struct	SG_GUILD_MEMBER_CONNETION_STATE
{
	cMSG_BASE_TYPE	base;
	
// 	DWORD			bf11Serial		:	11;
// 	DWORD			bf11PGSerial	:	11;	//	파티 혹은 길드 시리얼
// 	DWORD			bf6ChatType		:	6;
	
	char			strMessage[c_iMaxGuildMemberCount];	//	메시지
};

struct	SG_NOTICE
{
	cMSG_BASE_TYPE	base;

	char			strMessage[256];	//	메시지
};

//
//	Reason of Chat Failed
enum
{
	eRCF_CAN_NOT_SHOUT_BY_TIME,	//	너무 자주 떠든다.
};

struct	SG_CHAT_FAILED
{
	cMSG_BASE_TYPE	base;

	WORD			wReason;
};

struct	SG_SAY_RESULT
{
	cMSG_BASE_TYPE	base;
	WORD			wResult;	//	cPACKET_WORLDSERVER.h에 WORLDtoSERVERS_USER_SAY_FAIL 패킷 참조 

	char			strName[dNAME_LENGTH];		//	세이 실패한 넘.
	char			strMessage[dCHAT_LENGTH];	//	메시지
};

//	정보 메세지 
#define dINFOTYPE_PARTY		0//파티 정보
#define dINFOTYPE_GUILD		1//길드 정보
#define dINFOTYPE_ADMIN		2//관리자가 전달하는 정보들

struct SG_INFO
{
	cMSG_BASE_TYPE		base;

	WORD				wInfoType;
	char				strMessage[dCHAT_LENGTH];
};

struct	SG_CHANGE_PARTY_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD	bf3GoldShareMethod	:	3;
	WORD	bf3ItemShareMethod	:	3;
	WORD	bf3PartyObject		:	3;	//파티 목적
	WORD	bf1IsOpenParty		:	1;	//멤버를 계속 모집한다.
};

//
//	파티리스트 전송
struct	SG_PARTY_LIST
{
	cMSG_BASE_TYPE		base;

	WORD				wCount;		//	몇개나 보낼껴?
	WORD				wPartySIndex;
	WORD				wPartyEIndex;	//	받을 기준이 되는 파티의 인덱스

	CPartyInfoForList	aPartyList[dPARTYLIST_COUNT];
};

//	파티멤버에게 보내는 파티 정보(파티원 리스트나 기타등등)
struct	SG_PARTY_INFO
{
	cMSG_BASE_TYPE		base;

	CPartyInfoInPacket	partyInfo;
};

//	연합 파티 참가 요청
struct	SG_ASK_JOIN_UNION_PARTY
{
	cMSG_BASE_TYPE		base;
	
	char				strAskerName[dNAME_LENGTH];		//	파티 참가 요청한 사람 이름
	WORD				wAskerJob,wAskerLevel;			//	요청한 넘의 직업과 레벨
	//WORD				wAskerPartyWantJob;				//	선호 직업	//뺀다 게임서버에 없으니까...
	WORD				wAskerPartyCount;				//	파티원수
	WORD				wAskerPartyAverageLevel;		//	평균 레벨
	WORD				wAskerPartyPurpose;				//	파티 목적
	char				strPartyName[dMAX_PARTY_NAME_LENGTH];		//	파티 이름
};

//	파티 참가 요청
struct	SG_ASK_JOIN_PARTY
{
	cMSG_BASE_TYPE		base;

	WORD				bf1IsJoinPartyApplication	:	1;		//	파티에 가입 하기 위해 요청한거다.(파티 리더가 가입 권유 하는게 아님)
	WORD				bf1IsGuildMemberJoinParty	:	1;		//2 길드 멤버를 파티에 초대한당.
	WORD	:0;
	char				strAskerName[dNAME_LENGTH];		//	파티 참가 요청한 사람 이름
	WORD				wAskerJob,wAskerLevel;			//	요청한 넘의 직업과 레벨
};

//	파티 멤버 추가
struct	SG_JOIN_PARTY_MEMBER
{
	cMSG_BASE_TYPE		base;

	char				strName[dNAME_LENGTH];
	WORD				wLevel;
	WORD				wJob;
	WORD				wSerialInField;	//	시리얼(필드에서 같은 필드에 없으면 0xffff)
	WORD				wIndex;
	WORD				wField;
	int					iDuelVP;

	DWORD				bf10IFSerial	:	10;
	DWORD				bf4IFFloor		:	4;
	DWORD				bf4SPC			:	4;
};

//	누군가 파티에서 탈퇴 했다.(거나 쫏겨 났거나.. -_-a)
struct	SG_PARTY_WORK
{
	cMSG_BASE_TYPE		base;

	WORD				wWork;
	WORD				wMemberIndex;
};

//	
struct	SG_UPDATE_PARTY_MEMBER_HP
{
	cMSG_BASE_TYPE				base;

	WORD						wRemainHPRate,wActor;
	DWORD						dwMaxHP;
};

//	
struct	SG_UPDATE_PARTY_MEMBER_INFO
{
	cMSG_BASE_TYPE				base;

	BYTE						bIndex,bRestHp;
	WORD						wLevel;
	WORD						wField;

	DWORD						bf10InstanceField			:	10;
	DWORD						bf4InstanceFieldFloor		:	4;
	DWORD						bf1IsDisconnected			:	1;
	DWORD						bf4SecretDungeonPlayCount	:	4;
	DWORD						bf8Job						:	8;
	int							iDuelVP;

};

enum
{
	ePM_TARGET_DEATH_OR_NOT_EXIST								,	//	대상이 죽었거나 존재하지 않는다.
	ePM_TARGET_BELONG_TO_ALREADY_AT_DIFFERENT_PARTY				,	//	이미 다른 파티에 소속해 있다.
	ePM_TARGET_NOT_BELONG_TO_PARTY								,	//	그 녀석.. 우리 파티원이 아니다. -_-
	ePM_TARGET_NOT_EXIST										,	//	그런넘 존재하지 않는다.
	ePM_ONLY_PARTY_LEADER_CAN_SEND_AN_INVITATION_MESSAGE		,	//	파티 리더만 초청 메시지를 보낼수 있습니다.
	ePM_ONLY_PARTY_LEADER_CAN_SEND_AN_BANISH_MESSAGE			,	//	파티 리더만 초청 메시지를 보낼수 있습니다.
	ePM_ONLY_PARTY_LEADER_CAN_SEND_AN_CHANGE_PARTY_NAME_MESSAGE	,
	ePM_YOU_ARE_NOT_BELONG_TO_PARTY								,	//	넌 파티에 소속되어 있지 않아!!

	ePM_ALREADY_FULL_PARTY										,	//	풀 파티라서 더 이상 멤버를 받을 수 없다.
	ePM_ASKER_NOT_EXIST											,	//	요청자가 없어졌다.
	ePM_DENY_INVITE_MESSAGE										,	//	초청을 거부했다.
	ePM_DEST_PARTY_ARE_FULL										,	//	풀 파티가 되어 버렸다.
	ePM_DEST_ACTOR_NOT_LEADER									,	//	초청한넘... 리더 아냐-_-
	ePM_FAILED_TO_CREATE_PARTY									,	//	파티 생성 실패
	ePM_FAILED_TO_JOIN_PARTY									,	//	파티 참가에 실패
	ePM_TARGET_DENY_COMMUNITY_STATUS							,	//	대상이 커뮤니티 거부 상태다.
	ePM_TARGET_NOT_OWN_FORCE									,	//	같은 팀이 아녀

	ePM_CAN_NOT_BANISH_CONNECT_MEMBER_IN_SECRET_DUNGEON			,	//	
	ePM_ASK_JOIN_FAILED_BY_TARGET_LEVEL_OVER_THE_SECRET_DUNGEON_LIMIT_LEVEL	,	//	
	ePM_CAN_NOT_BANISH_MEMBER_UNDER_DROP_PARTY_ITEM_TIME		,	//	파티용 아이템이 드랍되고 1분간은 멤버를 추방 할 수 없다.

	ePM_ALREADY_UNION_PARTY										,	//	이미 연합 파티에 가입되어 있습니다.
	ePM_TARGET_IS_NOT_PARTY_LEADER								,	//	대상이 파티 리더가 아니다.
};

//
//	파티와 관련된 이런저런 메시지(넌 리더가 아냐!!,넌 파티에 속해 있지 않아!!,파티 풀~ 등등등..)
struct	SG_PARTY_MESSAGE
{
	cMSG_BASE_TYPE		base;

	WORD				wMessage;
};

//
//	파티 이름 변경
struct	SG_CHANGE_PARTY_NAME
{
	cMSG_BASE_TYPE		base;

	char				strPartyName[dPARTYNAME_LENGTH];
};

//
//	파티 해체
struct	SG_DISSOLUTION_PARTY
{
	cMSG_BASE_TYPE		base;
};

//
//	파티가 아이템 습득
struct	SG_PCIK_PARTY_ITEM
{
	cMSG_BASE_TYPE		base;

	CItemDefine			item;
	WORD				wReceiver;
};

struct	SG_DEATH_PENELTY_SECOND
{
	cMSG_BASE_TYPE		base;

	WORD				wDeathPenelty;

	WORD				bf14PeneltySecond	: 14;
	WORD				bf1IsDeath			: 1;
	WORD				bf1IsRecover		: 1;
};

struct	SG_QUESTION_FOR_RECOVER_DEATH_PENELTY
{
	cMSG_BASE_TYPE		base;

	WORD				wRecoverTime;
	DWORD				dwPrice;
};
//
//	파티가 골드 습득
struct	SG_PCIK_PARTY_GOLD
{
	cMSG_BASE_TYPE		base;

	int					iGold;
};

#define	dSERVER_MESSAGE_SIZE	256

//
//	Server Message
enum
{
	eSM_SYSTEM,
	eSM_CHATTING,
	eSM_NOTICE,
	eSM_MSG_BOX,
	eSM_WARNING,
};
//
//	메시지 전송
struct	SG_MESSAGE
{
	cMSG_BASE_TYPE		base;

	WORD				wType;
	char				strMessage[dSERVER_MESSAGE_SIZE];
};

//
//	Regist Server Message
enum
{
	eRSM_DUMMY,
	eRSM_CAN_NOT_EXIT_GAME_DURING_BATTLE,
	eRSM_CAN_NOT_CALL_MAGIC_CARPET_FIELD,
	eRSM_FIND_COPIED_ITEM,
	eRSM_TEST_QUEST,
	eRSM_PLEASE_RELEASE_PARTY_FOR_GUILD_BATTLE,
	eRSM_PROTECTED_BY_PREVENT_STEAL,
	eRSM_GUILD_ONLY_GUILD_MASTER_CAN_OPEN_GUILD_BATTLE_SCHEDULER,
	eRSM_GUILD_ALREADY_BOOKING_GUILD_BATTLE,
	eRSM_GUILD_ALREADY_FINISH_GUILD_BATTLE_FOR_THIS_WEEK,
	eRSM_GUILD_ALREADY_BOOKED_SCHEDULE,
	eRSM_GUILD_INCORRECT_DATA_PLEASE_RETRY,
	eRSM_GUILD_IS_CHECKED_BY_GUILD_BATTLE_DAY_RULE,
	eRSM_GUILD_IS_RECENTLY_BATTLE_GUILD,
	eRSM_GUILD_TOO_MANY_DIFFERENT_VICTORY_POINT,
	eRSM_GUILD_ONLY_GUILD_MASTER_CAN_REQUEST_GUILD_BATTLE,
	eRSM_GUILD_ONLY_GUILD_MASTER_CAN_CANCEL_GUILD_BATTLE,
	eRSM_GUILD_TOO_LATE_TO_CANCEL_GUILD_BATTLE,
	eRSM_GUILD_NOT_EXIST_BOOKED_GUILD_BATTLE,
	eRSM_GUILD_INCORRECT_SOME_DATA_PLEASE_RECONNECT,
	eRSM_GUILD_TOO_LOW_LEVEL_TO_GUILD_BATTLE,
	eRSM_TOO_LOW_GOLD_TO_GUILD_BATTLE,
	eRSM_GUILD_ONLY_GUILD_MASTER_CAN_OPEN_GUILD_MARK_COMPOSER,
	eRSM_OVER_TIME_ITEM_TERM_OF_VALIDATE_IN_INVENTORY,
	eRSM_TOO_MANY_PORTAL_IN_THIS_FIELD,
	eRSM_REMOVE_PORTAL,
	eRSM_TOO_FAR_PORTAL,
	eRSM_LACK_MONEY_FOR_RECOVER_DEATH_PENELTY,

	eRSM_CAN_NOT_RECEIVE_ITEM_BY_INVENTORY_FULL	,
	eRSM_NOTICE_FOR_SPEEDHACK_USER_FORM,
	eRSM_NOTICE_FOR_INCORRECT_PLAYER_USER,
	eRSM_NOTICE_FOR_RELEASE_PENELTY_TIME,
	eRSM_GAME_SERVER_CLOSE,

	eRSM_IS_CAN_NOT_TAME_THAT_MONSTER_BY_THIS_SKILL,
	eRSM_IS_CAN_NOT_TAME_NO_MORE_MONSTER_BY_THIS_SKILL,
	eRSM_IS_CAN_NOT_TAME_MONSTER,
	eRSM_IS_CAN_NOT_TAME_NO_MORE_MONSTER,
	eRSM_NOT_EXIST_CONTROL_PET,
	eRSM_NOT_EXIST_OWNED_SPECIAL_ATTACK_PET,
	eRSM_THAT_SKILL_REQUIRE_VALID_SLOT,

	eRSM_THAT_PET_ALREADY_USE_ALL_BOOST_POWER_FOR_TODAY,
	eRSM_TOO_MANY_PET_CAN_NOT_UNSEAL_BREEDING_RECORD_BOOK,
	eRSM_ALREADY_NAMED_PET,
	eRSM_CAN_NOT_SUMMON_MORE_SUMMON_BEAST,
	eRSM_FOR_UNSEAL_PET_REQUIRE_MORE_POWERFUL_TAME_SKILL,
	eRSM_BREEDING_RECORD_UNSEAL_ONLY_SEAL_PLAYER,
	eRSM_NOT_EXIST_SUMMON_BEAST_USE_THAT_SKILL,

	eRSM_TOO_SHORT_TIME_WHEN_SEAL_PET,
	eRSM_CAN_NOT_SET_NO_MORE_THAT_SKILL_TRAP,
	eRSM_RELEASE_SUMMON_BEAST_BY_LOW_CP,
	eRSM_SUMMON_BEAST_GRADE_DOWN_BY_LOW_CP,
	eRSM_CAN_NOT_SUMMON_BY_DEATH_RULE,
	eRSM_REVISE_INCORRECT_PREFIX_IN_BANK,
	eRSM_REVISE_INCORRECT_PREFIX,

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	eRSM_COMMON_MESSAGE_BORDER,
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	eRSM_OWN_GOLD_LIMIT,
	eRSM_OWN_BANK_GOLD_LIMIT,
	eRSM_ALREADY_STEAL_GOLD_MONSTER,
	eRSM_ALREADY_STEAL_ITEM_MONSTER,

	eRSM_CAN_NOT_ADD_SECRET_DUNGEON_KEY_BY_FULL_INVENTORY,
	eRSM_THAT_IS_CLOSE_SECRET_DUNGEON,
	eRSM_TOO_MANY_PLAYER_ENTER_SECRET_DUNGEON,
	eRSM_OVER_THE_SECRET_DUNGEON_PLAY_COUNT_PER_DAY,
	eRSM_CAN_NOT_ENTER_TO_PORTAL_BY_IS_OWNED_DIFFERENT_PARTY,

	eRSM_LACK_MONEY_FOR_JUDGE_ITEM,
	eRSM_LACK_MONEY_FOR_MERGE_ITEM,
	eRSM_TO_USE_COUPLERING_SET_REQUIRE_TWO_VALID_INVENTORY_SLOT,
	eRSM_REQUEST_TIME_IS_MAINTENANCE_TIME,
	eRSM_EXIST_TRAFFIC_OBJECT_TO_OPEN_PORTAL,

	eRSM_CAN_NOT_USE_THAT_ITEM_IN_THIS_FIELD,
	eRSM_TARGET_IS_AT_BLOCKED_FIELD_TO_USE_THAT_ITEM,
	eRSM_CAN_NOT_TELEPORT_FIELD,
	eRSM_ALREADY_REQUEST_GUILD_SCRAMBLE,
	eRSM_CAN_NOT_WARP_OWN_GUILD_HALL_BY_VALID,
	eRSM_CAN_NOT_WARP_OWN_GUILD_HALL_BY_VALID_FIELD_SERIAL,
	eRSM_CAN_NOT_WITHDRAW_GUILD_ITEM_BY_CHECK_PERMIT_COUNT,
	eRSM_CAN_NOT_WITHDRAW_GUILD_GOLD_BY_CHECK_PERMIT_GOLD,

	eRSM_FAILED_COMMAND_WORK,
	eRSM_ALREADY_EXIST_SAME_VILLAGE_IN_GUILD_RELATED_PLACE_LIST,	//	이미 같은 연고지가 존재한다.
	eRSM_FULL_VILLAGE_IN_GUILD_RELATED_PLACE_COUNT,	//	더 이상 연고지를 추가 할 수 없다.
	eRSM_LOW_GOLD_FOR_ADD_VILLAGE_IN_GUILD_RELATED_PLACE_LIST,	//	eEW_LOW_GOLD_FOR_ADD_VILLAGE_IN_GUILD_RELATED_PLACE_LIST
	eRSM_NOT_EXIST_VILLAGE_IN_GUILD_RELATED_PLACE_LIST,	//	제거 하려는 마을이 연고지에 없다.
	eRSM_MINIMUM_COUNT_RULE_OF_GUILD_RELATED_PLACE_LIST	,	//	최소 하나 이상의 길드 연고지가 있어야 한다.
	eRSM_COUNT_OVER_TO_TRY_GUILD_POINT_BATTLE,
	eRSM_TO_TRY_GUILD_POINT_BATTLE_GATHER_PARTY_MEMBER_IN_SAME_FIELD,
	eRSM_TO_MANY_GUILD_TO_TRY_GUILD_POINT_BATTLE,
	eRSM_FAILED_TO_TRY_GUILD_POINT_BATTLE,
	eRSM_FAILED_TO_ENTER_GUILD_BATTLE_FIELD_BY_NOT_BATTLE_TIME,
	eRSM_TARGET_GUILD_HALL_IS_CLOSE_TO_STRANGER,

	eRSM_CAN_NOT_STORE_ITEM_IN_GUILD_INVENTORY,

	eRSM_PERMIT_ONLY_GUILD_MASTER_WORK			,
	eRSM_VALID_GUILD_SKILL_LEVEL	,
	eRSM_TEMP_LIMIT_GOLD_IN_GUILD_INVENTORY,
	eRSM_FAILED_BY_INSTANCE_GUILD_BATTLE_TIME_RULE,
	eRSM_CAN_NOT_SUMMON_MINI_PET_BY_FULL_COUNT,	//	미니펫을 더 이상 소환 할 수 없다.
	eRSM_ITS_USE_TO_ENEMY_SKILL,
	eRSM_TOO_FAR_TO_USE_SKILL,
	eRSM_CAN_NOT_FIND_ATTACK_ABLE_TARGET,
	eRSM_SEAL_MINIPET,
	eRSM_UNSEAL_MINIPET,
	eRSM_IS_NOT_ABLE_TO_TRY_GUILD_POINT_BATTLE_TIME,
	eRSM_CAN_NOT_ACTIVATE_MINIPET_BY_IS_EXPIRED_MINIPET,
	eRSM_EXPIRED_REVERSAL_FRAME_TIME,
	eRSM_MISSILE_BLOCKED_LINE,
	eRSM_PLEASE_SELECT_MINIPET_FOR_CHANGE_NAME,
	eRSM_REQUIRE_MINIPET_FOR_USE_THAT_ITEM,
	eRSM_DUEL_1VS1_RULE,
	eRSM_DUEL_CAN_NOT_TRY_BY_TO_MANY_BATTLER,
	eRSM_DUEL_CAN_NOT_TRY_BY_BAD_PARTY_STATUS,
	eRSM_DUEL_FIELD_IS_FULL,	//	결투 필드가 꽉 찼다.
	eRSM_DUEL_EXCEPTION_TIME_RULE,	//	3시 전후 10분간은 결투 금지
	eRSM_DUEL_CAN_NOT_TRY_BY_YOU_ARE_NOT_PARTY_LEADER,
	eRSM_DUEL_CAN_NOT_TRY_BY_MISMATCH_PARTY_MEMBER_COUNT,
	eRSM_MINI_PET_CANT_USE_AWAKEN_ITEM_LOWER,

	eRSM_GUILD_DUNGEN_ERROR_GATHER_PARTY_MEMBER_IN_SAME_FIELD,
	eRSM_COUNT_OVER_TO_TRY_GUILD_DUNGEN,
	eRSM_TO_MANY_GUILD_TO_TRY_GUILD_DUNGEN,
	eRSM_FAILED_TO_TRY_GUILD_DUNGEN,

	eRSM_EXIST_COUNT_OVER_MEMBER_TO_TRY_SECRET_DUNGEN,
	eRSM_SECRET_DUNGEN_ERROR_GATHER_PARTY_MEMBER_IN_SAME_FIELD,
	eRSM_TO_MANY_PARTY_TO_TRY_SECRET_DUNGEN,
	eRSM_TOO_MANY_BANK_TRANSACTION_WITH_REBIRTH,
	eRSM_TOO_MANY_GOLD_BAR,


//	메시지만 출력할거라면 여기에 추가 할것!!

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	eRSM_OUTPUT_MESSAGE_BORDER		=	9999,		//	여기 다음은 메시지는 안나온다
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	eRSM_TOUCH_EVENT_AREA_SUCCESS	=	10000,
	eRSM_TOUCH_EVENT_AREA_FAILED,
//	메시지만 출력할거라면 여기에 추가 하지 말것!!

};
//
//	메시지 전송
struct	SG_REGIST_MESSAGE
{
	cMSG_BASE_TYPE		base;

	WORD				wMessage;
	WORD				wType;
};

struct	SG_EXIT_GAME_SUCCESS
{
	cMSG_BASE_TYPE		base;

	WORD				wValue;
};

struct	SG_PICK_PARTY_ITEM
{
	cMSG_BASE_TYPE		base;

	char		strPickerName[dNAME_LENGTH];
	WORD		bf1IsSharingGold	:	1;

	CItemDefine	item;
};

struct	SG_REMAIN_HP
{
	cMSG_BASE_TYPE		base;

	int					iMaxHP;
	WORD				wRemainHPRate;
	WORD				wActor;
	WORD				bf1IsSyncUnionParty : 1;
	WORD				bf1IsGuildObject : 1;		//2 길드오브젝트.
	WORD				:0;
};

struct	SG_GUILD_PACKET_FORM
{
	cMSG_BASE_TYPE		base;
	WORD				wGuildSerial;
};
//
//	친구 추가 요청
struct	SG_REQUEST_ADD_FRIEND
{
	cMSG_BASE_TYPE		base;

	char				strName[dNAME_LENGTH];
};

struct	SG_REPLY_FOR_ADD_FRIEND
{
	cMSG_BASE_TYPE		base;

	WORD				wResult;
	WORD				bf8Job			:	8;
	WORD				bf1IsRecover	:	1;
	char				strName[dNAME_LENGTH];
};

//	친구 목록에서 내 이름 제거해줘!! 라고 요청
struct	SG_ASK_REMOVE_FRIEND
{
	cMSG_BASE_TYPE		base;

	char				strName[dNAME_LENGTH];
};

enum
{
	eRFR_SUCCESS,
	eRFR_NOT_EXIST_NAME_IN_TARGET_FRIEND_LIST,
	eRFR_TARGET_NOT_CONNECTED,
};

struct	SG_ASK_REMOVE_FRIEND_RESULT
{
	cMSG_BASE_TYPE		base;

	char				strName[dNAME_LENGTH];
	WORD				wResult;
};

struct	SG_READY_TO_MAKE_GUILD
{
	cMSG_BASE_TYPE		base;

	char				strMemberList[dPARTY_MAXPLAYER][dNAME_LENGTH];
};

struct	SG_OPEN_GUILD_BATTLE_SCHEDULER
{
	cMSG_BASE_TYPE			base;

	WORD					wBeginHour,wBeginMinute,wBattlePeriod,wBattleCount;
};

struct	SG_OPEN_GUILD_MARK_COMPOSER
{
	cMSG_BASE_TYPE			base;
};

struct	SG_GUILD_BATTLE_SCHEDULE_IN_SAME_TIME
{
	cMSG_BASE_TYPE						base;

	WORD								wTimeOrder;
	WORD								wYear,wMonth,wDay;
	CGuildBattleInFieldInfoForClient	data;
};

struct	SG_OPEN_GUILD_HALL_LIST
{
	cMSG_BASE_TYPE		base;

	WORD	wCount;
	char	abBuffer[dGUILD_NAME_LENGTH*c_iGuildHallCount];	//	길드홀 정보
};

struct	SG_GUILD_HONOR_POINT_LIST
{
	cMSG_BASE_TYPE		base;

	class	cGuildHonorPointInfoForPacket
	{
	public:
		int		m_iHonorPoint;
		char	m_strGuildName[dGUILD_NAME_LENGTH];
	};
	
	WORD	wCount,wHallLevel;
	cGuildHonorPointInfoForPacket	aList[30];
};

struct	SG_OPEN_RELATED_PLACE_LIST
{
	cMSG_BASE_TYPE		base;

	WORD	wCount;
	WORD	awPlaceList[dMAX_GUILD_VILLAGE_COUNT];	//	연고지 리스트
};

struct	SG_OPEN_GUILD_INVENTORY
{
	cMSG_BASE_TYPE		base;

	WORD							wCount;
	int								aiWithdrawGoldLimitPerDay[5];		//	직위별 하루에 꺼낼 수 있는 골드 제한
	BYTE							aWithdrawItemCountPerDay[4][5];		//	직위별 하루에 꺼낼 수 있는 아이템 수 제한
	BYTE							abInventorySize[4];					//	인벤토리 사이즈
	LONGLONG						llGuildGold;

	cGuildInventoryItemForPacket	aItems[c_iGuildInventorySize];		//	길드홀 정보
};

struct	SG_STORE_GUILD_INVENTORY_ITEM
{
	cMSG_BASE_TYPE		base;

	WORD		wGuildInventorySlot;
	CItemDefine	item;
};

struct	SG_REMOVE_GUILD_INVENTORY_ITEM
{
	cMSG_BASE_TYPE		base;

	WORD		wGuildInventorySlot;
	CItemDefine	item;
};

struct	SG_STORE_GUILD_INVENTORY_GOLD
{
	cMSG_BASE_TYPE		base;

	int		iGold;
};

struct	SG_REMOVE_GUILD_INVENTORY_GOLD
{
	cMSG_BASE_TYPE		base;

	int		iGold;
};

struct	SG_CHANGE_GUILD_INVENTORY_ITEM_PLACE
{
	cMSG_BASE_TYPE		base;

	WORD	wSlot1,wSlot2;
};

enum
{
	eSW_BEGIN_NOTICE,
	eSW_END_NOTICE	,
	eSW_BEGIN		,
	eSW_END			,

	eSW_SUCCESS_TO_DEFENSE	,
	eSW_SUCCESS_TO_INVADE	,
	eSW_FAILED_TO_DEFENSE	,
	eSW_FAILED_TO_INVADE	,
};

enum			//길드 은총 관련 메시지들
{
	eGMG_ACTIVE_GUILD_MASTER_GRACE			,
	eGMG_ACTIVE_GUILD_MASTER_GRACE_END		,
	eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR1	,		//길드 마스터만 활성화 할수 있습니다.
	eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR2	,		//길드 마스터의 은총은 일주일에 한번만 사용이 가능 합니다.
	eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR3	,		//길드 레벨이 낮아 불가능 합니다.
	eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR4	,		//음 알수없는 이유로 실패
};

struct	SG_GVG_WORK_SUCCESS
{
	cMSG_BASE_TYPE		base;

	WORD	wResult;
	WORD	wIsTransAvatar;
	char	strName[dGUILD_NAME_LENGTH];
};

struct	SG_GUILD_MASTER_GRACE_INFO
{
	cMSG_BASE_TYPE		base;
	
	WORD	wWork,wValue;
};

struct	SG_SIEGE_WARFARE_NOTICE	//	공성전 관련 공지
{
	cMSG_BASE_TYPE		base;

	WORD	wWork,wValue;
	char	strGuildName[dGUILD_NAME_LENGTH];
};

struct	SG_GUILD_PET_AND_GUARDIAN_EXP
{
	cMSG_BASE_TYPE		base;

	CGuildPetInfo	aPetInfo[dMAX_GUILD_PET_COUNT];
	CGuildPetInfo	aGuardianInfo[dMAX_GUILD_GUARDIAN_COUNT];
};

struct	SG_OPEN_GUILD_SHOP
{
	cMSG_BASE_TYPE		base;

	WORD	wSellItemCount;
	WORD	wDC_Factor;
	WORD	awSellItemList[c_iGuildHallSellEnchantItemCount];
};

struct	SG_CHANGE_GUILD_BATTLE_SCHEDULE
{
	cMSG_BASE_TYPE				base;

	WORD						wTimeOrder,wFieldOrder;
	CGuildBattleInfoForClient	info;
};

//
//	Make Guild Result
enum
{
	eMGR_SUCCESS				,	//	오케바리~
	eMGR_EXIST_SAME_NAME_GUILD	,	//	같은 이름의 길드가 있어
	eMGR_NOT_READY_TO_MAKE_GUILD,	//	길드 만들 준비가 안됐어
	eMGR_ALREADY_HAVEAGUILD		,	//	이미 길드에 속해 있다.
	eMGR_LOW_POWER				,	//	길드 만들 능력이 안된다.
	eMGR_FULL					,	//	더 이상 길드를 만들 수 없다.
	eMGR_FAILED					,	//	걍 실패
};

struct	SG_MAKE_GUILD_RESULT
{
	cMSG_BASE_TYPE		base;

	WORD				wResult;
	char				strGuildName[dGUILD_NAME_LENGTH];
};

//
//	길드전 결과 전송
struct	SG_GUILD_BATTLE_RESULT
{
	cMSG_BASE_TYPE				base;

	WORD						wGuildSerial;
	char						strTargetGuild[dGUILD_NAME_LENGTH];
	CGuildBattleResultInfo		battleResult;
};

#pragma	pack(1)
class	CSimpleGuildMemberInfo
{
protected:
	char	m_strName[dNAME_LENGTH-1];
public:

	char*	getName()
	{
		static	char	strName[dNAME_LENGTH];
		
		memcpy(strName,m_strName,dNAME_LENGTH-1);

		strName[dNAME_LENGTH-1]	=	NULL;

		return	strName;
	}
	void	setName(char *_lpstrName)
	{
		memcpy(m_strName,_lpstrName,dNAME_LENGTH-1);
	}

	WORD	m_bf8Job			:	8;	// 8
	WORD	m_bf4ReservePet		:	4;	// 12

	WORD	m_bf4Rank			:	4;	//16

	WORD	m_bf11Level			:	11;	//11
	WORD	m_bf1IsConnected	:	1;	//12
	WORD	m_bf1IsOnline		:	1;	//13 온라인상태..
};

#pragma	pack()

class	CSimpleGuildMemberList
{
public:
	WORD					m_wCount;

	CSimpleGuildMemberInfo	m_aMembers[dGUILD_MAX_PLAYER];
};

#define	dGUILD_MEMBER_COUNT_FOR_MEMBER_LIST_PACKET	50

struct	SG_GUILD_MEMBER_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						bf15Count	:	15;
	WORD						bf1IsAppend	:	1;

	CSimpleGuildMemberInfo		m_aMember[dGUILD_MEMBER_COUNT_FOR_MEMBER_LIST_PACKET];
};

struct	SG_GUILD_MARK_INFO
{
	cMSG_BASE_TYPE				base;

	WORD						wGuildSerial;
	WORD						bf1IsTwinkle	:	1;
	WORD						bf8Level		:	8;
	WORD						bf3HallLevel	:	3;
	CGuildMarkInfo				markInfo;
};

struct	SG_CHANGE_GUILD_MARK
{
	cMSG_BASE_TYPE				base;

	WORD						wGuildSerial;
	WORD						bf4Result				:	4;
	WORD						bf1IsUseGuildMarkPack	:	1;
	CGuildMarkInfo				guildMarkInfo;
};

//
//	길드 기본 정보
struct	SG_BASE_GUILD_INFO
{
	cMSG_BASE_TYPE				base;

	WORD						wGuildSerial;	//	이게 0xffff 이면 너 길드 없어!! 라는 뜻
	CGuildBaseInfo				guildInfo;
};

//
//	길드창을 열지 않은 상태에서 알고 있어야 할 가장 기본적인 정보
struct	SG_SIMPLE_GUILD_INFO
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	BYTE				abGuildStatueStatus[6];
	WORD				wGuildSkillCount;
	WORD				bf3HallLevel	:	3;
	WORD				bf8Level		:	8;
	WORD				bf1IsTwinkle	:	1;
	CGuildMarkInfo		guildMarkInfo;						//	길드 마크
	char				strGuildName[dGUILD_NAME_LENGTH];
	BYTE				abGuildSkill[dMAX_GUILD_SKILL_COUNT];	//	길드 스킬
};

struct	SG_UPDATE_GUILD_BATTLE_INFO_FOR_MEMBER
{
	cMSG_BASE_TYPE				base;

	WORD						wGuildSerial;
	CGuildBattleInfoForMember	data;
};


struct	SG_READY_TO_NEXT_GUILD_BATTLE
{
	cMSG_BASE_TYPE				base;
	WORD						wGuildSerial;
	char						strTargetGuildName[dGUILD_NAME_LENGTH];	//	길드 이름
	BYTE						bHour,bMinute;
};

struct	SG_GUILD_BATTLE_NOTICE
{
	cMSG_BASE_TYPE				base;
	BYTE						bRemainTimeToBeginBattle,bRemainTimeToEndBattle;
};

struct	SG_GUILD_BATTLE_INFO
{
	cMSG_BASE_TYPE				base;
	char						strTeam1Name[dGUILD_NAME_LENGTH],strTeam2Name[dGUILD_NAME_LENGTH];
	int							iGuild1KillPoint,iGuild2KillPoint;
	WORD						wIsTrialGame;
};

struct	SG_GUILD_BATTLE_KILL_NOTICE
{
	cMSG_BASE_TYPE				base;
	WORD						wKillerTeam,wGetPoint,wIsDuel;
	int							iGuild1KillPoint,iGuild2KillPoint;
	char						strKiller[dNAME_LENGTH],strTarget[dNAME_LENGTH];
};

struct	SG_DISSOLUTION_GUILD
{
	cMSG_BASE_TYPE		base;

	char				strGuildName[dGUILD_NAME_LENGTH];	//	길드 이름
};

struct	SG_JOIN_GUILD//	길드 가입
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;	//	길드 시리얼
	char				strGuildName[dGUILD_NAME_LENGTH];	//	길드 이름
};

struct	SG_UPDATE_GUILD_MEMBER	//	길드 가입
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	char				strMemberName[dNAME_LENGTH];	//	이름
	WORD				m_bf4Rank			:	4;	//4
	WORD				m_bf11Level			:	11;	//15
	WORD				m_bf1IsConnected	:	1;	//16

	WORD				m_bf8Job		:	8;		//8
	WORD				m_bf4ReservePet	:	4;		//12
	WORD				:0;

	BYTE				bMemberCount,bCongressManCount,bSubMasterCount;
};
struct	SG_REMOVE_GUILD_MEMBER	//	길드원 제거
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	char				strMemberName[dNAME_LENGTH];	//	이름
	BYTE				bMemberCount,bCongressManCount,bSubMasterCount;
};

struct	SG_UPDATE_GUILD_LEVEL
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	WORD				wGuildLevel;
	DWORD				dwGuildExp;
	WORD				wGuildPoint,wExpTaxRate;
};

//
//	Guild Work Message
enum
{
	eGWM_YOU_ARE_NOT_GUILD_LEADER,
	eGWM_TOO_LONG_GUILD_NOTICE_LENGTH,
	eGWM_YOU_ARE_NOT_GUILD_MASTER,
	eGWM_MEMBER_COUNT_NOT_ONE_AT_DISSOLUTION_GUILD,
	eGWM_INCORRECT_MEMBER_NAME_LENGTH,
	eGWM_TOO_MANY_SUB_GUILD_MASTER,
	eGWM_TOO_MANY_GUILD_CONGRESS,
	eGWM_USER_NOT_GUILD_MEMBER,
	eGWM_USER_NOT_SUB_GUILD_MASTER,
	eGWM_GUILD_MASTER_CAN_NOT_LEAVE_GUILD,
	eGWM_LEAVE_GUILD_LIMIT_BY_JOIN_DATE,
	eGWM_JOIN_GUILD_LIMIT_BY_JOIN_DATE,
	eGWM_USER_ALREADY_SUB_GUILD_MASTER,
	eGWM_USER_ALREADY_GUILD_CONGRESS,
	eGWM_USER_NOT_GUILD_CONGRESS,
	eGWM_COMPLETE_ASK_WORK,
	eGWM_FAILED_ASK_WORK,
	eGWM_YOU_ARE_NOT_READY_TO_MAKE_GUILD,
	eGWM_INCORRECT_GUILD_NAME	,
	eGWM_INCORRECT_TAX_RATE		,

	eGWM_YOU_ARE_CAN_NOT_CHANGE_SKILL_LEVEL,
	eGWM_INCORRECT_SKILL_INDEX			,
	eGWM_CAN_NOT_INCREASE_LEVEL_SKILL	,
	eGWM_NOT_REMAIN_GUILD_SKILL_POINT	,
	eGWM_GUILD_BATTLE_SLEEP_TIME		,
	eGWM_YOU_ARE_ALREADY_GUILD_MEMBER	,
	eGWM_ONLY_GUILD_MASTER_CAN_SELECT_GUILD_MARK,
	eGWM_CHANGE_RANK_IN_GUILD,
	
	eGWM_CAN_NOT_BANISH_GUILD_MASTER,
	eGWM_CAN_NOT_BANISH_GUILD_ELDER,
	eGWM_CAN_NOT_BANISH_GUILD_CONGRESS,

	eGWM_FAILED_REQUEST_GUILD_BATTLE_BY_LOW_HALL_LEVEL	,	//	길드 홀 레벨이 낮아서 길드전 참가 할 수 없다.
	eGWM_FAILED_REQUEST_GUILD_BATTLE_BY_FULL_REQUEST		,	//	그 시간에 신청한 길드전 개수가 꽉 찼다.
	eGWM_FAILED_CANCEL_GUILD_BATTLE_BY_ALREADY_SETTING_BATTLE_SCHEDULE	,	//	이미 길드전

	eGWM_LACK_INCREASE_MEMBER_COUNT_SKILL_LEVEL	,

};
//
//	길드 작업 관련 메시지
struct	SG_GUILD_WORK_MESSAGE
{
	cMSG_BASE_TYPE		base;

	WORD				wMessage;
	WORD				wValue1,wValue2;
	char				strName[dNAME_LENGTH];
};

struct	SG_CHANGE_GUILD_TAX_RATE	//	경험치 세율 변경
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	WORD				wTaxRate;
};

struct	SG_UPDATE_GUILD_SKILL_LEVEL
{
	cMSG_BASE_TYPE	base;

	WORD			wGuildSerial,wSkill,wLevel,wRemainSkillPoint;
};

struct	SG_UPDATE_GUILD_BATTLE_COUNT_AT_THIS_WEEK
{
	cMSG_BASE_TYPE	base;

	WORD			wGuildSerial;
	WORD			wBattleCountAtWeek;
};

struct	SG_CHANGE_GUILD_NOTICE		//	길드 공지 변경
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	char				strGuildNotice[dGUILD_NOTICE_LENGTH];
};

struct	SG_CHANGE_RANK_IN_GUILD		//	길드 멤버 랭크 변경
{
	cMSG_BASE_TYPE		base;

	WORD				wRank;
};
//
//	Reason of Exit Guild
enum
{
	eREG_LEAVE,
	eREG_BANISH,
};

struct	SG_EXIT_GUILD//	길드 탈퇴
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;	//	길드 시리얼
	WORD				wReason;
	char				strGuildName[dGUILD_NAME_LENGTH];	//	길드 이름
};

//
//	Ask Join Guild Result
enum
{
	eAJGR_ALREADY_GUILD_MEMBER,
	eAJGR_TARGET_NOT_EXIST,
	eAJGR_TARGET_NOT_GUILD_MASTER,
	eAJGR_TARGET_NOT_GUILD_LEADER,
	eAJGR_TARGET_DENY_COMMUNITY,
	eAJGR_TARGET_DENY,
	eAJGR_TARGET_ACCEPT,
	eAJGR_TARGET_BUSY,
	eAJGR_GUILD_MEMBER_FULL,
	eAJGR_REQUIRE_CITIZEN_TITLE,
	eAJGR_GUILD_NOT_EXIST,
	eAJGR_FAILED,
};

struct	SG_ASK_JOIN_GUILD_RESULT
{
	cMSG_BASE_TYPE		base;

	WORD				wResult;
};

struct	SG_ASK_JOIN_GUILD
{
	cMSG_BASE_TYPE		base;

	WORD				wAskerSerial;
	char				strAskerName[dNAME_LENGTH];
	WORD				wLevel,wJob;
};

//┃Client->Server
//┃커뮤니티
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server->Client
//┃

//
//	DIS 메시지
struct	SG_OUT
{
	cMSG_BASE_TYPE	base;

	char			strCause[128];		//	죽이는 이유 -_x
};	//	struct	SG_CHAT
//┃Server->Client
//┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server->Client
//┃패킷 트레일러

#define	dMAX_SPEECH_LENGTH	2048

struct	SG_COMPLEX_SPEECH
{
	cMSG_BASE_TYPE	base;

	WORD			wOwner;//말한넘
	WORD			wEventArea;
	WORD			wContentsCount;//선택문 수
	WORD			wSpeechIndex;//대화 인덱스
	WORD			wDialogBoxSize;//대화 상자 사이즈

	char			strText[dMAX_SPEECH_LENGTH];
};

//
//	이벤트 실패 이요
//	REF -> Reason of Event Failed
enum
{
	eREF_NOT_OWNED_EVENT	,	//	가지고 있는 이벤트가 없다.
	eREF_TOO_FAR			,	//	너무 멀다.
};

//
//	이벤트 같은거 없는 NPC다.
struct	SG_EVENT_FAILED
{
	cMSG_BASE_TYPE	base;
	WORD			wOwner;
	WORD			wReason;
};	//	struct	SG_EVENT_FAILED

//
//	대화 끝내기
struct	SG_END_DIAOLG
{
	cMSG_BASE_TYPE	base;
	WORD			wOwner;
};	//	struct	SG_END_DIAOLG

#define	dMAX_ITEM_IN_SHOP	50

#define	dSHOP_NAME_LENGTH	32

//
//	상점 열기
struct	SG_OPEN_SHOP
{	// 마지막 수정일 : 09.10.26
	cMSG_BASE_TYPE	base;

	WORD			wOwner;
	WORD			wShopSerial;
	WORD			bf15SellItemCount	: 15;
	WORD			bf1IsDuelPointShop	: 1;
	WORD			bf1IsLevelPerGoldShop	:	1;
	WORD			bf15LevelPerGoldValue	:	15;
	WORD			wPriceFactor;
	DWORD			dwLimitPrice;
	WORD			wTokenItem;
	WORD			bf1IsNotBuyItem		:1;		// 상점 구입 불가.
	WORD			bf1IsBadgeOfTraderShop : 1;	// 거래상인의 배지 상점..
	WORD			:0;
	char			strName[dSHOP_NAME_LENGTH];
	CItemDefine		aItem[dMAX_ITEM_IN_SHOP];
};	//	struct	SG_EVENT_FAILED


//
//아이템 구입
struct	SG_BUY_ITEM
{
	cMSG_BASE_TYPE	base;

	DWORD			dwPrice				;	//	가격
	DWORD			bf1IsPigeonPostItem	:	1;
	DWORD			bf1IsDuelPointItem	:	1;
	WORD			wTokenShop;
	int				iRemainGold;//	남은 돈
	WORD			wCount;

	CItemDefine		aBuyItem[dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE];	//	아이템
};


//	FRBI -Failed Reason of Buy Item
enum
{
	eFRBI_TOO_FAR_DISTANCE		,	//	너무 멀다.
	eFRBI_NPC_NOT_OWN_THAT_SHOP	,	//	그 NPC는 그런 상점을 운영-_-하고 있지 않다.
	eFRBI_NOT_EXIST_SHOP		,	//	존재하지 않는 상점이다.
	eFRBI_NOT_EXIST_ITEM		,	//	그 상점에는 그런 아이템이 없는디유.. -o-
	eFRBI_LACK_OF_GOLD			,	//	골드가 부족하다.
	eFRBI_INVENTORY_FULL		,	//	인벤토리가 꽉 찼다.
	eFRBI_CAN_NOT_OWN_SAME_ITEM	,	//	같은 아이템을 여러개 가질 수 없는 아이템이다.
	eFRBI_TOO_MANY_BADGE_ITEM	,	//	배지 아이템이 너무 많아.
	eFRBI_UNKONWN				,	//	걍 실패
	eFRBI_LACK_OF_DUEL_POINT	,	//	결투 포인트가 부족하다.
	eFRBI_LACK_OF_TOKEN		,	//	토큰 아이템이 부족하다.
};
//
//	아이템 구입 실패
struct	SG_FAILED_BUY_ITEM
{
	cMSG_BASE_TYPE	base;

	int				iGold;	//	소지 금액
	WORD			wReason;//	이유
};	//	struct	SG_FAILED_BUY_ITEM


//
//	아이템 구입
struct	SG_SELL_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wSellItemSlot;
	WORD			wCount;		//	몇개 파냐?
	int				iPrice;	//	가격
	int				iRemainGold;//	남은 돈
};	//	struct	SG_BUY_ITEM

//	FRSI -Failed Reason of Sell Item
enum
{
	eFRSI_TOO_FAR_DISTANCE		,	//	너무 멀다.
	eFRSI_NPC_NOT_OWN_THAT_SHOP	,	//	그 NPC는 그런 상점을 운영-_-하고 있지 않다.
	eFRSI_NOT_EXIST_SHOP		,	//	존재하지 않는 상점이다.
	eFRSI_NOT_EXIST_ITEM		,	//	존재하지 않는 아이템이다
	eFRSI_NOT_MATCHED_ITEM_COUNT,	//	아이템 갯수가 맞지 않는다.
	eFRSI_CAN_NOT_SELL_ITEM		,	//	팔 수 없는 아이템이다.
	eFRSI_OWN_TOO_MANY_GOLD		,	//	팔 수 없는 아이템이다.
	eFRSI_CHANGE_ITEM_PRICE		,	//	아이템 가격에 변동이 있어서 닫는다.
	eFRSI_UNKONWN				,	//	걍 실패
};
//
//	아이템 구입 실패
struct	SG_FAILED_SELL_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wReason;//	이유
};	//	struct	SG_FAILED_BUY_ITEM

//
//	퀘스트 정보
struct	SG_QUEST_INFO
{
	cMSG_BASE_TYPE		base;

	tsProcessQuestField	m_aProcessQuest[dMAX_PROCESS_QUEST_COUNT];
};

//
//	퀘스트 정보
struct	SG_CHANGE_QUEST_INFO
{
	cMSG_BASE_TYPE		base;

	WORD				bf4ProcessSlot			:	4;
	WORD				bf1IsForQuestComplete	:	1;
	tsProcessQuestField	processQuest;
	WORD				wCountDownTime;
};

//
//	Cancel Quest Result
enum
{
	eCQR_SUCCESS,	//	퀘스트 취소 성공
	eCQR_FAILED,	//	퀘스트 취소 실패
};

struct	SG_CANCEL_QUEST
{
	cMSG_BASE_TYPE		base;

	WORD				wQuestSlot;
	WORD				wResult;
};

//┃Server->Client
//┃패킷 트레일러
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server->Client
//┃은행 관련

//	은행 정보
struct	SG_OPEN_BANK
{
	cMSG_BASE_TYPE	base;

	CBankInfo		bankInfo;
	WORD			wBankSerial,wBankSize,wBankCharge;
};

struct	SG_STORE_ITEM_TO_THE_BANK		
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	WORD			wInventorySlot,wBankSlot;
};

struct	SG_STORE_GOLD_TO_THE_BANK		
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	int				iGold;
};

struct	SG_WITHDRAW_ITEM_FROM_THE_BANK	
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	WORD			wInventorySlot,wBankSlot;
};

struct	SG_WITHDRAW_GOLD_FROM_THE_BANK	
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	int				iGold;
};

struct	SG_MOVE_BANK_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wPos1,wPos2;
	DWORD			dwCheckSum;
};

struct	SG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION
{
	cMSG_BASE_TYPE	base;

	WORD			wPos1,wPos2;
	DWORD			dwCheckSum;
};

//
//	Message of Bank
enum
{
	eMOB_WAIT_BANK_DATA,
	eMOB_WAIT_TO_BANK_OPEN_PERIOD_MESSAGE,
	eMOB_TOO_MANY_BANK_TRANSACTION,	//	은행 거래가 너무 많아요.
	eMOB_CANCELED_BY_NOT_MATCHED_DATA_WITH_SERVER,
	eMOB_TRANSACTION_FINISH,
	eMOB_FAILED,
	eMOB_NOT_BANK_CLIENT,	//	은행 거래가 너무 많아요.
	eMOB_CAN_NOT_STORE_ITEM,	//	은행에 보관 할 수 없는 아이템
	eMOB_CAN_OWN_SAME_ITEM,	//	은행에 보관 할 수 없는 아이템
	eMOB_TOO_MANY_BADGE_ITEM,	//	배지 아이템이 너무 많어
	eMOB_REMOVE_EXPIRED_ITEM,	//	유효기간 지난 아이템 제거
	eMOB_OVER_TIME_ITEM_TERM_OF_VALIDATE,	//	유효기간 연장
	eMOB_TOO_MANY_BANK_TRANSACTION_WITH_REBIRTH,	//	전생 도중 실패
	eMOB_IS_NOT_OWN_ITEM,						// 본인이 아니면 찾을 수 없어용~
	eMOB_TOO_MANY_GOLD_BAR,						// 금괴가 너무 많아요(은행)
	eMOB_PLAYER_HAVE_TOO_MANY_GOLD_BAR,						//	금괴가 많아용(플레이어)
};

struct	SG_BANK_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wMessage;
};

//┃Server->Client
//┃은행 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┣━━프리미엄 아이템 관련

struct	SG_OPEN_CART					//	카트 열기
{
	cMSG_BASE_TYPE	base;

	WORD			wCartSerial;
	WORD			wItemCount;
	tsBuyLst		aItemList[dMAX_BUYLIST];
};

//
//	Cart Message
enum
{
	eCM_CART_CLOSE_BY_INCORRECT_INFO,	//	뭔가 정보가 잘못되어서 닫았다.
	eCM_INVENTORY_FULL				,	//	인벤토리가 비어있다.
	eCM_FAILED_BY_INVENTORY_FULL	,	//	꺼낼라고 보니까 인벤토리에 빈칸이 없다!!
	eCM_FAILED_BY_VALID_ITEM_INFO	,	//	프리미엄 아이템 정보를 기본으로 아이템을 생성하는데 실패했다.
	eCM_EXIST_FAILED_PREMIUM_ITEM	,	//	꺼내기 실패한 프리미엄 아이템이 있어
	eCM_FAILED_BY_TOO_FAST_REASK	,	//	너무 빨리 다시 오픈 했슈.
	eCM_ALREADY_OPEN_CART			,	//	이미 카트를 오픈했슈
	eCM_TOO_MANY_OPEN_SHOPPING_CART	,	//	카트 오픈이 너무 많어.. ;;
	eCM_WAIT_CART_DATA				,	//	카트 데이터 기다려요
	eCM_NOT_OPENED_CART				,	//	카트가 열려 있지 않아요.
	eCM_CAN_NOT_LOAD_THAT_PLACE		,	//	그 자리에는 아이템을 꺼낼 수 없어효!!!
	eCM_ONLY_ONE_ITEM_IN_INVENTORY	,	//	인벤토리에 하나만 꺼낼수 있는 아이템인데효.
	eCM_TOO_MANY_ITEM_COUNT					,	//	카트에 아이템 수가 너무 많아효!!
	eCM_ALREADY_EXIST_MORE_HIGH_LEVEL_ORB	,	//	이미 더 좋은 오브를 가지고 있어!!
	eCM_WAIT_FOR_PREVIOUS_WORK_RESULT		,	//	좀 전에 요청한 작업 결과 기다리는 중이잖아!!
	eCM_LOTTERY_EVENT_COMMENT				,	//	복권 이벤트 기간이다. 자리좀 비워라
	eCM_MAKE_RICE_SOUP_PLACE				,	//	떡국 이벤트 기간이다. 자리좀 비워라
	eCM_CAN_NOT_OWN_SAME_ITEM				,	//	똑같은 아이템을 보유할 수 없는 아이템이다.
	eCM_TOO_MANY_BADGE_ITEM					,	//	배지 아이템이 너무 많아.
	eCM_ALREADY_EXIST_SAME_EFFECT_IN_EMBLEM	,	//	이미 그효과를 가지고 있는 엠블럼이 있다.
	eCM_CAN_NOT_WITHDRAW_ITEM_BY_EXTRA_BAG_SIZE_FULL	,	//	가방 크기가 이미 꽉 찼다.	
	eCM_CAN_NOT_WITHDRAW_CARROT_BY_OWN_LIMIT	,	//	당근을 꺼낼 수없습ㄴ디ㅏ.
	eCM_FAILED_BY_EXPIRED_ITEM					,	//	이미 유효기간이 지난 아이템이다.
};

struct	SG_CART_MESSAGE					//	카트 메시지
{
	cMSG_BASE_TYPE	base;

	WORD			wCartMessage;
};

struct	SG_WITHDRAW_ITEM_FROM_THE_CART	
{
	cMSG_BASE_TYPE	base;

	WORD			wCartSlot;
	WORD			wInventorySlot;
	CItemDefine		item;
};

//
//	Destroy Cart Item Result
enum
{
	eDCIR_SUCCESS,
	eDCIR_FAILED,
	eDCIR_CAN_NOT_DESTROY_NOT_USED_ITEM,
};

struct	SG_DESTROY_CART_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wCartSlot;
};

struct	SG_CARROT_SHOP_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wIsOK;	//	클라이언트 정보와 서버 정보가 같다.
	WORD			wIndex,wStatus,wCount;
	int				iRemainCarrotCount;

	cCarrotShopCategoryInfo2	categoryInfo;
	cCarrotShopCategoryInfo1	data;
};

struct	SG_OPEN_CARROT_SHOP
{
	cMSG_BASE_TYPE	base;

	int				iRemainCarrotCount;	//	클라이언트 정보와 서버 정보가 같다.
};

struct	SG_LOAD_FAILED_CART_PREMIUM_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wInventorySlot;
	CItemDefine		item;
};

//┣━━프리미엄 아이템 관련
//┃Client->Server
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┣━━노점상 관련

//	Pitchman Store Message
//	노점상 메시지
enum
{
	ePSM_OK,
	ePSM_FAILED,

	ePSM_SETUP_FAILED_BY_REQUIRE_DEALER_TITLE,
	ePSM_SETUP_FAILED_BY_EXIT_ANOTHER_PITCHMAN_SHOP,
	ePSM_SETUP_FAILED_BY_TOO_MANY_PITCHMAN_SHOP,
	ePSM_SETUP_FAILED,

	ePSM_FAILED_REGIST_ITEM_BY_ALREADY_REGISTED,
	ePSM_FAILED_REGIST_ITEM_BY_VALID_INVENTORY_SLOT,
	ePSM_FAILED_SHOP_NOT_OPENED,

	ePSM_FAILED_REMOVE_ITEM_BY_VALID_SLOT,

	ePSM_FAILED_CLOSE_SHOP_BY_NOT_OPENED,

	ePSM_FAILED_BUY_ITEM_BY_CHANGE_SHOP_INFO,			//	슬롯이 비었다.
	ePSM_FAILED_BUY_ITEM_BY_INSUFFICIENT_GOLD,	//	돈이 모자라서 실패
	ePSM_FAILED_BUY_ITEM_BY_FULL_INVENTORY,
	ePSM_FAILED_BUY_ITEM_BY_FULL_GOLD,

	ePSM_FAILED_OPEN_SHOP_BY_NOT_OPENED,		//	아직 오픈 안했다.

	ePSM_FAILED_BY_VALID_SHOP_SERIAL	,	//	돈이 모자라서 실패

	ePSM_FAILED_BY_CAN_NOT_TRADE_ITEM	,	//	귀속 아이템이라 거래 할 수 없다.
	ePSM_FAILED_BY_CAN_NOT_TRADE_ITEM_IS_REVERSION_ITEM	,	//	귀속 아이템이라 거래 할 수 없다.
	ePSM_FAILED_BY_INCORRECT_NAME,	//	상점 이름으로 쓸 수 없는 이름이다.

	ePSM_FAILED_BUY_ITEM_BY_INSUFFICIENT_GOLD_BAR,	// 금괴가 모자라서 실패~!		// 09.08.21
	ePSM_FAILED_BUY_ITEM_BY_OWNER_NOT_SLOT,				// 판매자의 인벤토리 공간이 부족하다. // 	// 09.08.21
	ePSM_FAILED_BUY_ITEM_BY_MANY_GOLD_BAR,				// 자신의 금괴가 너무 많아요
	ePSM_CAN_NOT_OPEN_THIS_FIELD,						//	현재필드에서는 할수 없어요
};

struct	SG_PITCHMAN_SHOP_MESSAGE		//
{
	cMSG_BASE_TYPE	base;

	WORD			wMessage;
};

struct	SG_OPEN_PITCHMAN_SHOP	//	
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wX,wY;
};

struct	SG_ADD_PITCHMAN_SHOP_ITEM			//		09.08.21
{
	cMSG_BASE_TYPE	base;

	WORD			wInventorySlot,wShopSlot;
	DWORD			dwPrice;
	BYTE			bDenomination;
};

struct	SG_REMOVE_PITCHMAN_SHOP_ITEM			//
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot;
};

struct	SG_CHANGE_PITCHMAN_SHOP_INFO			//
{
	cMSG_BASE_TYPE	base;
	
	CSimplePitchmanShopInfo		info;
	char						strName[dPITCHMAN_SHOP_NAME_LENGTH];
};

struct	SG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot1,wSlot2;
};

struct	SG_BUY_PITCHMAN_SHOP_ITEM				//	노점상에서 아이템 구입
{
	cMSG_BASE_TYPE	base;

	WORD			wPitchmanSerial;	//	노점상 시리얼
	WORD			wSlot;		//	노점상에 아이템 위치
	CItemDefine		item;		//	구입한 아이템
	DWORD			dwPrice;	//	가격
	BYTE			bDenomination;// 통화 단위 . 09.08.21
	char			strOwnerName[dNAME_LENGTH];
};

struct	SG_SOLD_PITCHMAN_SHOP_ITEM				//	노점상에서 아이템이 팔렸다.
{
	cMSG_BASE_TYPE	base;

	char			strName[dNAME_LENGTH];
	WORD			wSlot;			//	노점상에 아이템 위치
	DWORD			dwPrice;		//	가격
	BYTE			bDenomination;// 통화 단위 . 09.08.21
	
};

struct	SG_ADD_PITCHMAN_SHOP					//	노점상 추가
{
	cMSG_BASE_TYPE				base;

	CSimplePitchmanShopInfo		info;
};

struct	SG_REMOVE_PITCHMAN_SHOP				//	노점상 제거
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
};

struct	SG_GGG_INFO							//	거신의 은총
{
	cMSG_BASE_TYPE	base;
	
	WORD			wValue[6];
};

struct	SG_UNION_PARTY_WORK					//	파티 연합 관련 정보.
{
	cMSG_BASE_TYPE	base;
	
	WORD			wValue[6];
};

#define	dUNION_PARTY_MEMBER_STATE_NORMAL		0x00000000
#define	dUNION_PARTY_MEMBER_STATE_DISCONNECT	0x00000001
#define	dUNION_PARTY_MEMBER_STATE_OTHERFILED	0x00000002

class	CUnionPartyMemberInfo
{
public:
	WORD						m_wUnionPartySerial;	//	파티 연합 시리얼.
	char						m_strName[dNAME_LENGTH];//	이름
	WORD						m_wHP;					//	남은 HP
	DWORD						m_dwState;				//	파티 원의 상태를 bit 마스크로 표현
	//WORD						m_wJob;					//	직업
	//WORD						m_wLevel;				//	레벨
	//WORD						m_wSerial;				//	필드에서의 시리얼(같은 필드가 아닐때는 0xffff)
	//WORD						m_wNameBarSizeAtField;
	//WORD						m_wStatusField;			//	상태 필드
	//CPos						m_pos;					//	위치
	//WORD						m_wFieldSerial;			//	현재 필드 시리얼
	//WORD						m_wPlaySecretDungeonCount;
};

struct	SG_UNION_PARTY_MEMBER_INFO					//	파티 연합 관련 정보 하나 하나 보낼때..
{
	cMSG_BASE_TYPE				base;
	
	WORD						wUnionPartySerial;
	WORD						wUnionPartyIndex;
	WORD						wPartyMemberIndex;
	CUnionPartyMemberInfo		Info;
};	

struct	SG_UNION_PARTY_INFO					//	파티 연합 관련 정보 통째로 보낼때.
{
	cMSG_BASE_TYPE				base;
	
	WORD						wUnionPartySerial;
	WORD						wUnionPartyIndex;
	WORD						wEffectivenessCount;
	CUnionPartyMemberInfo		Info[dMAX_UNION_PARTY_COUNT];
};	

#define	dPITCHMAN_SHOP_COUNT_FOR_LIST_PACKET		100

struct	SG_PITCHMAN_SHOP_LIST					//	노점상 리스트
{
	cMSG_BASE_TYPE				base;

	WORD						bf15Count : 15;
	WORD						bf1IsFirst: 1;

	CMinimumPitchmanShopInfo	aList[dPITCHMAN_SHOP_COUNT_FOR_LIST_PACKET];
};

#define	dPITCHMAN_SHOP_COUNT_FOR_VERY_SIMPLE_INFO_PACKET	10

struct	SG_SIMPLE_PITCHMAN_SHOP_INFO_LIST
{
	cMSG_BASE_TYPE				base;
	WORD						wCount;

	char						aData[(dPITCHMAN_SHOP_NAME_LENGTH+10)*dPITCHMAN_SHOP_COUNT_FOR_VERY_SIMPLE_INFO_PACKET];
};

struct	SG_PITCHMAN_SHOP_INFO					//	노점상 정보
{
	cMSG_BASE_TYPE				base;

	CPitchmanShopForClient		shopInfo;
};

#define	dRESULT_ITEM_COUNT_OF_SEARCH_ITEM_IN_RELAY_STATION	20

struct	SG_RESULT_OF_SEARCH_ITEM_IN_RELAY_STATION
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			wShopSerial;
	WORD			wLastSlot;
	WORD			wIsFinish;

	CPitchmanShopItemInfoForClient	m_aItemList[dRESULT_ITEM_COUNT_OF_SEARCH_ITEM_IN_RELAY_STATION];
};
//┣━━노점상 관련
//┃Client->Server
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┣━━함정 관련
struct	SG_ADD_TRAP
{
	cMSG_BASE_TYPE				base;

	WORD		wSkill,wSerial,wPosX,wPosY,wRange;
	WORD		bf3Team			:	3;
	WORD		bf8DetectLevel	:	8;
};

struct	SG_REMOVE_TRAP
{
	cMSG_BASE_TYPE				base;

	WORD		wSerial;
};

//
//	함정 터짐
struct	SG_EXPLOSION_TRAP
{
	cMSG_BASE_TYPE		base;

	DWORD				bf6TargetCount	: 6;
	DWORD				bf12Skill		: 12;
	DWORD				bf8Level		: 8;
	DWORD				bf1IsFirst		: 1;

	WORD				wPosX,wPosY;

	cWIDE_AREA_DAMAGE_E	aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};

class	cTrapSkillDamageE
{
public:
	WORD			wActor;	//	누구?
	WORD			wPhysicalDamage,wMagicDamage;	//	데미지
};

//
//	함정 터짐
struct	SG_EXPLOSION_TRAP_SKILL
{
	cMSG_BASE_TYPE		base;

	DWORD				bf6TargetCount	: 6;
	DWORD				bf11Skill		: 11;

	cTrapSkillDamageE	aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};


class	cSimpleTrapInfo
{
public:
	WORD				m_wSerial;
	WORD				m_wX,m_wY;
	WORD				m_wRange;
	WORD				m_wSkill;
	WORD				m_bf3Team			:	3;
	WORD				m_bf8DetectLevel	:	8;
};

#define	dMAX_TRAP_COUNT_IN_FIELD_FOR_PACKET	20

struct	SG_EXIST_TRAP_LIST
{
	cMSG_BASE_TYPE		base;

	WORD				wTrapCount;
	cSimpleTrapInfo		aTrapInfo[dMAX_TRAP_COUNT_IN_FIELD_FOR_PACKET];
};

class	cSimpleFireWallInfo
{
public:
	WORD	m_wSkill,m_wX1,m_wY1,m_wX2,m_wY2;
};

const	int	c_iMaxFireWallCountForPacket	=	100;

struct	SG_EXIST_FIRE_WALL_LIST
{
	cMSG_BASE_TYPE		base;

	WORD				wWallCount;
	cSimpleFireWallInfo	aWallList[c_iMaxFireWallCountForPacket];
};

#define	dMAX_ACTIVE_TRAP_COUNT_IN_PACKET	32

struct	SG_EXIST_ACTIVE_FIELD_TRAP_LIST
{
	cMSG_BASE_TYPE		base;

	WORD				wTrapCount;
	WORD				awTrapList[dMAX_ACTIVE_TRAP_COUNT_IN_PACKET];
};

//┣━━함정 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server->Client
//┃기타
struct	SG_WARNING_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wMessageIndex;
	WORD			wMessageType;
};

struct	SG_CONTROL_MONSTER
{
	cMSG_BASE_TYPE	base;

	WORD			wLord;
	WORD			wSlave;
};

struct	SG_OCCUR_EFFECT
{
	cMSG_BASE_TYPE	base;

	WORD			wX,wY,wEffect,wScale;
};

struct	SG_ACCOUNT_INFO
{
	cMSG_BASE_TYPE	base;

	int		aPlayerCountPerServer[10];
};

struct	SG_PENELTY_TIME
{
	cMSG_BASE_TYPE	base;

	DWORD	dwPeneltyTime;
};

//┃Server->Client
//┃기타
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server->Admin
//┃Admin 관련 
struct	SG_SENDINFOTOADMIN
{
	cMSG_BASE_TYPE		base;

	DWORD		wUserCount,wMaxUserCount;				//	동접수
};

struct	SG_CLOSESERVER
{
	cMSG_BASE_TYPE		base;

	char		strMsg[16];
};
//┃Server->Admin
//┃Admin 관련 
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃
//┃Stress Test 관련
struct	CG_STRESSCLIENT
{
	cMSG_BASE_TYPE		base;

	WORD				wUserCount;
	WORD				wSendSize;
};

struct	CG_REGIST_STRESS_CLIENT
{
	cMSG_BASE_TYPE		base;

	int					iUniqueSerial;
};


struct	CG_MODIFY_STRESS_CLIENT
{
	cMSG_BASE_TYPE		base;
};


struct	CG_STRESSDATA
{
	cMSG_BASE_TYPE		base;
};


struct	SG_STRESSCLIENT
{
	cMSG_BASE_TYPE		base;

	
};

struct	SG_STRESS_SERVER_INFO
{
	cMSG_BASE_TYPE		base;

	int					iConnectUserCount;
	int					iUpdateTime;
};


//┃
//┃Stress Test 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server->Client
//┃패킷 트레일러

const	int		c_iPacketTrailerDataSize	=	dMAX_PACKET_LENGTH-20;	//	20kb

struct	SG_PACKET_TRAILER
{
	cMSG_BASE_TYPE	base;

	int				m_iCount;

	char			m_aData[c_iPacketTrailerDataSize];
};
//┃Server->Client
//┃패킷 트레일러
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//
//	패킷 큐를 위해서.. -_-a
union	ALL_MSG
{
	cMSG_BASE_TYPE							base;	//	기본 패킷

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃예약 작업들
	BW_MAKE_DUMMY							mBW_MAKE_DUMMY;
	BW_DISPLACEMENT							mBW_DISPLACEMENT;
	BW_CHARMING								mBW_CHARMING;
	BW_EXIT_GAME							mBW_EXIT_GAME;
	BW_READY_TO_EXIT						mBW_READY_TO_EXIT;
	BW_DEATH_ACTOR							mBW_DEATH_ACTOR;

	BW_EXIT_GAME_FOR_MOVE_FIELD				mBW_EXIT_GAME_FOR_MOVE_FIELD;
//┃예약 작업들
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃게임서버
//┃클라이언트->서버
	
	CG_MOVE									mCG_MOVE;
	CG_STOP									mCG_STOP;
	CG_TOGGLE_SITDOWN						mCG_TOGGLE_SITDOWN;
	CG_ACTION_TO_ACTOR						mCG_ACTION_TO_ACTOR;
	CG_ACTION_TO_GROUND						mCG_ACTION_TO_GROUND;
	CG_ACTION_QUICK							mCG_ACTION_QUICK;
	CG_SET_MOVE_ABILITY						mCG_SET_MOVE_ABILITY;
	CG_ASK_INFO_ACTOR_LIST					mCG_ASK_INFO_ACTOR_LIST;
	CG_OBSCURITY_ACTOR_LIST					mCG_OBSCURITY_ACTOR_LIST;
	CG_MISPLACED_ACTOR_LIST					mCG_MISPLACED_ACTOR_LIST;
	CG_ENGAGE_BATTLE						mCG_ENGAGE_BATTLE;
	CG_STOP_BATTLE							mCG_STOP_BATTLE;	
	CG_INCREASE_STATE						mCG_INCREASE_STATE;	
	CG_INCREASE_ABILITY_LEVEL				mCG_INCREASE_ABILITY_LEVEL;
	CG_ASK_ACTOR_INFO						mCG_ASK_ACTOR_INFO;
	CG_TRANSFORMATION						mCG_TRANSFORMATION;
	CG_MOVE_FIELD							mCG_MOVE_FIELD;

	CG_PICK_ITEM							mCG_PICK_ITEM;				//	필드에 떨어진 아이템 줍기
	CG_EQUIP_ITEM							mCG_EQUIP_ITEM;				//	아이템 장비
	CG_RELOAD_BELT_ITEM						mCG_RELOAD_BELT_ITEM;		//	아이템 리로드
	CG_USE_ITEM								mCG_USE_ITEM;				//	아이템 사용
	CG_USE_ITEM_TO_ITEM						mCG_USE_ITEM_TO_ITEM;
	CG_USE_BELT_ITEM						mCG_USE_BELT_ITEM;			//	벨트에 아이템을 사용한다.
	CG_ASK_ITEM_DATA						mCG_ASK_ITEM_DATA;			//	뭔가 잘못 되었다!! 아이템 데이터를 요청한다.
	CG_DROP_ITEM							mCG_DROP_ITEM;				//	아이템 떨구기
	CG_CHANGE_ITEM_PLACE					mCG_CHANGE_ITEM_PLACE;		//	아이템 위치 변경
	CG_STRIP_EQUIPMENT						mCG_STRIP_EQUIPMENT;		//	장비 해제
	CG_DESTROY_ITEM							mCG_DESTROY_ITEM;
	CG_SET_REACTION_STATUS					mCG_SET_REACTION_STATUS;	//	리액션 상태 설정
	CG_TURN_ON_PASSIVE_SKILL				mCG_TURN_ON_PASSIVE_SKILL;	//	리액션 상태 설정
	CG_RETURN_TO_VILLAGE					mCG_RETURN_TO_VILLAGE;
	CG_OPERATOR_COMMAND						mCG_OPERATOR_COMMAND;
	CG_EXIT_GAME							mCG_EXIT_GAME;

	CG_REQUEST_TRADE						mCG_REQUEST_TRADE;		//	트레이드 요청
	CG_TRADE_REPLY							mCG_TRADE_REPLY;		//	트레이드 수락
	CG_CANCEL_TRADE							mCG_CANCEL_TRADE;		//	트레이드 취소
	CG_ADD_TRADE_ITEM						mCG_ADD_TRADE_ITEM;		//	트레이드 아이템 추가
	CG_CHANGE_TRADE_ITEM_DATA				mCG_CHANGE_TRADE_ITEM_DATA;
	CG_REMOVE_TRADE_ITEM					mCG_REMOVE_TRADE_ITEM;	//	트레이드 아이템 제거
	CG_SET_TRADE_GOLD						mCG_SET_TRADE_GOLD;		//	트레이드 할 골드 설정
	CG_READY_TRADE							mCG_READY_TRADE;		//	트레이드 준비
	CG_PERMIT_TRADE							mCG_PERMIT_TRADE;		//	트레이드 허락

	CG_SELECT_NPC							mCG_SELECT_NPC;			//	NPC 선택
	CG_SELECT_SPEECH						mCG_SELECT_SPEECH;		//	대사 선택
	CG_BUY_ITEM								mCG_BUY_ITEM;			//	아이템 구입
	CG_SELL_ITEM							mCG_SELL_ITEM;
	CG_CANCEL_QUEST							mCG_CANCEL_QUEST;
	CG_SUMMON_EVENT_MONSTER					mCG_SUMMON_EVENT_MONSTER;	// 이벤트 몬스터 소환

	CG_ASK_OPEN_THE_PITCHMAN_SHOP			mCG_ASK_OPEN_THE_PITCHMAN_SHOP;	//	노점상 아이템 등록
	CG_ADD_PITCHMAN_SHOP_ITEM				mCG_ADD_PITCHMAN_SHOP_ITEM;
	CG_REMOVE_PITCHMAN_SHOP_ITEM			mCG_REMOVE_PITCHMAN_SHOP_ITEM;	//	노점상에서 아이템 제거
	CG_CHANGE_PITCHMAN_SHOP_INFO			mCG_CHANGE_PITCHMAN_SHOP_INFO;		//	노점상 상태 변경 (오픈,클로즈,종료)
	CG_BUY_PITCHMAN_SHOP_ITEM				mCG_BUY_PITCHMAN_SHOP_ITEM;			//	노점상에서 아이템 구입
	CG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE		mCG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE;//	노점상 아이템 위치 변경
	CG_CLOSE_PITCHMAN_SHOP					mCG_CLOSE_PITCHMAN_SHOP;
	CG_ASK_PITCHMAN_SHOP_INFO				mCG_ASK_PITCHMAN_SHOP_INFO;			//	노점상 정보 요청
	CG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST	mCG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST;

	CG_STORE_ITEM_TO_THE_BANK				mCG_STORE_ITEM_TO_THE_BANK;						//	은행에 아이템 저장 요청
	CG_STORE_GOLD_TO_THE_BANK				mCG_STORE_GOLD_TO_THE_BANK;						//	은행에 골드 저장 요청
	CG_WITHDRAW_ITEM_FROM_THE_BANK			mCG_WITHDRAW_ITEM_FROM_THE_BANK;					//	은행에서 아이템 꺼내기
	CG_WITHDRAW_GOLD_FROM_THE_BANK			mCG_WITHDRAW_GOLD_FROM_THE_BANK;					//	은행에서 골드꺼내기
	CG_MOVE_BANK_ITEM						mCG_MOVE_BANK_ITEM;								//	은행 아이템 위치 변경
	CG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	mCG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION;	
	CG_FINISH_BANK_TRANSACTION				mCG_FINISH_BANK_TRANSACTION;						//	은행 거래 종료
	CG_CANCEL_BANK_TRANSACTION				mCG_CANCEL_BANK_TRANSACTION;						//	은행 거래 취소(뭔가 문제가 있다.)


	CG_CHAT									mCG_CHAT;
	CG_ASK_PARTY_LIST						mCG_ASK_PARTY_LIST;			//	파티 리스트 요청
	CG_ASK_PARTY_INFO						mCG_ASK_PARTY_INFO;			//	자신이 속한 파티의 정보 요청
	CG_ASK_JOIN_PARTY						mCG_ASK_JOIN_PARTY;			//	파티 참가 요청을 한다.
	CG_ASK_JOIN_PARTY_REPLY					mCG_ASK_JOIN_PARTY_REPLY;	//	파티 참가 요청 결과
	CG_LEAVE_PARTY							mCG_LEAVE_PARTY;			//	파티에서 탈퇴하기
	CG_PARTY_LEADER_WORK					mCG_PARTY_LEADER_WORK;				//	파티에서 쫏아내기(리더만 가능)
	CG_CHANGE_PARTY_NAME					mCG_CHANGE_PARTY_NAME;		//	파티 이름 변경
	CG_FIND_USER							mCG_FIND_USER;
	CG_REQUEST_ADD_FRIEND					mCG_REQUEST_ADD_FRIEND;
	CG_REPLY_FOR_ADD_FRIEND					mCG_REPLY_FOR_ADD_FRIEND;
	CG_CHANGE_COMMUNITY_STATUS				mCG_CHANGE_COMMUNITY_STATUS;
	CG_DEBUG_MESSAGE						mCG_DEBUG_MESSAGE;


	CG_CREATE_GUILD							mCG_CREATE_GUILD;
	CG_ASK_GUILD_INFO						mCG_ASK_GUILD_INFO;
	CG_ASK_GUILD_MEMBER_LIST				mCG_ASK_GUILD_MEMBER_LIST;
	CG_ASK_JOIN_GUILD						mCG_ASK_JOIN_GUILD;			//	길드 참가 요청
	CG_REPLY_ASK_JOIN_GUILD					mCG_REPLY_ASK_JOIN_GUILD;
	CG_DESTROY_CART_ITEM					mCG_DESTROY_CART_ITEM;

	CG_ASK_CHANGE_GUILD_NOTICE				mCG_ASK_CHANGE_GUILD_NOTICE;	//	길드 공지 변경 요청
	CG_ASK_CHANGE_GUILD_EXP_TAX				mCG_ASK_CHANGE_GUILD_EXP_TAX;	//	길드 경험치 세율 변경 요청
	CG_ASK_DISMISS_SUB_GUILD_MASTER			mCG_ASK_DISMISS_SUB_GUILD_MASTER;	//	서브 길드 마스터 해임 건의?
	CG_ASK_APPOINTMENT_SUB_GUILD_MASTER		mCG_ASK_APPOINTMENT_SUB_GUILD_MASTER;	//	서브 길드 마스터 임명 요청
	CG_ASK_APPOINTMENT_GUILD_CONGRESS		mCG_ASK_APPOINTMENT_GUILD_CONGRESS;
	CG_ASK_DISMISS_GUILD_CONGRESS			mCG_ASK_DISMISS_GUILD_CONGRESS;
	CG_ASK_BANISH_GUILD_MEMBER				mCG_ASK_BANISH_GUILD_MEMBER;	//	길드 멤버 추방 요청
	CG_ASK_LEAVE_GUILD						mCG_ASK_LEAVE_GUILD;	//	길드 탈퇴 요청
	CG_ASK_CHANGE_GUILD_MASTER				mCG_ASK_CHANGE_GUILD_MASTER;
	CG_READY_TO_MAKE_GUILD					mCG_READY_TO_MAKE_GUILD;
	CG_REGIST_REGULAR_GUILD_MEMBER			mCG_REGIST_REGULAR_GUILD_MEMBER;
	CG_INCREASE_GUILD_SKILL_LEVEL			mCG_INCREASE_GUILD_SKILL_LEVEL;
	CG_ASK_GUILD_BATTLE_SCHEDULE			mCG_ASK_GUILD_BATTLE_SCHEDULE;
	CG_REQUEST_GUILD_BATTLE					mCG_REQUEST_GUILD_BATTLE;
	CG_CANCEL_GUILD_BATTLE					mCG_CANCEL_GUILD_BATTLE;
	
	CG_CART_OPEN							mCG_CART_OPEN;
	CG_WITHDRAW_ITEM_FROM_THE_CART			mCG_WITHDRAW_ITEM_FROM_THE_CART;
	CG_CLOSE_CART							mCG_CLOSE_CART;
	CG_CALL_MAGIC_CARPET					mCG_CALL_MAGIC_CARPET;
	CG_REMEMBER_ORB_LOCATION				mCG_REMEMBER_ORB_LOCATION;
	CG_TELEPORT_TO_ORB_LOCATION				mCG_TELEPORT_TO_ORB_LOCATION;
//	━━stress test client
	CG_STRESSCLIENT							mCG_STRESSCLIENT;
//┃클라이언트->서버
//┃게임서버
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃게임서버
//┃서버->클라이언트
	SG_MOVE									mSG_MOVE;
	SG_STOP									mSG_STOP;
	SG_THRUST								mSG_THRUST;
	SG_CORRECT_PLAYER_POSITION				mSG_CORRECT_PLAYER_POSITION;
	SG_ACTION_TO_ACTOR						mSG_ACTION_TO_ACTOR;
	SG_ACTION_TO_GROUND						mSG_ACTION_TO_GROUND;
	SG_PLAYER_OUT							mSG_PLAYER_OUT;
	SG_JOIN_PLAYER							mSG_JOIN_PLAYER;
	SG_SET_MOVE_ABILITY						mSG_SET_MOVE_ABILITY;
	SG_REGEN_MONSTER						mSG_REGEN_MONSTER;
	SG_BASIC_ATTACK							mSG_BASIC_ATTACK;
	SG_CONTINUOUS_HIT_BY_CHARACTER			mSG_CONTINUOUS_HIT_BY_CHARACTER;
	SG_CONTINUOUS_HIT_BY_VALUE				mSG_CONTINUOUS_HIT_BY_VALUE;
	SG_LAND_MARKER							mSG_LAND_MARKER;
	SG_BUNSHINE_ATTACK						mSG_BUNSHINE_ATTACK;
	SG_SCIMITAR_CUTTING						mSG_SCIMITAR_CUTTING;
	SG_QUICK_ACTION							mSG_QUICK_ACTION;
	SG_KILL									mSG_KILL;
	SG_OUT									mSG_OUT;
	SG_WIDE_AREA_DAMAGE						mSG_WIDE_AREA_DAMAGE;
	SG_WIDE_AREA_EFFECT						mSG_WIDE_AREA_EFFECT;
	SG_GUARDIAN_POST						mSG_GUARDIAN_POST;
	SG_TRANSFORMATION						mSG_TRANSFORMATION;
	SG_MOVE_FIELD_RESULT					mSG_MOVE_FIELD_RESULT;
	SG_MOVE_FIELD_NOW						mSG_MOVE_FIELD_NOW;
	SG_TOGGLE_SITDOWN						mSG_TOGGLE_SITDOWN;
	SG_LEVEL_UP_OTHER						mSG_LEVEL_UP_OTHER;
	SG_CONTROL_MONSTER						mSG_CONTROL_MONSTER;
	SG_HIT_THORN_DAMAGE						mSG_HIT_THORN_DAMAGE;
	SG_ABSORB_DAMAGE						mSG_ABSORB_DAMAGE;
	SG_RELEASE_SHIMMERING_SHIELD			mSG_RELEASE_SHIMMERING_SHIELD;
	SG_SET_SHIMMERING_SHIELD				mSG_SET_SHIMMERING_SHIELD;
	SG_REMOVE_ACTOR							mSG_REMOVE_ACTOR;
	SG_FLOAT_MACHINE						mSG_FLOAT_MACHINE;
	SG_RELEASE_MACHINE						mSG_RELEASE_MACHINE;
	SG_SHOOT_MACHINE_MISSILE				mSG_SHOOT_MACHINE_MISSILE;
	SG_FIRE_TAIL_CHASER						mSG_FIRE_TAIL_CHASER;
	SG_MAKE_ACTION_FORM						mSG_MAKE_ACTION_FORM;
	SG_THRUST_ACTORS						mSG_THRUST_ACTORS;
	SG_USE_SKILL_TO_ACTOR					mSG_USE_SKILL_TO_ACTOR;
	SG_USE_SKILL_TO_GROUND					mSG_USE_SKILL_TO_GROUND;
	SG_RELEASE_DUMMY						mSG_RELEASE_DUMMY;
	SG_CREATE_DUMMY							mSG_CREATE_DUMMY;
	SG_RECEIVE_AID_MAGIC					mSG_RECEIVE_AID_MAGIC;
	SG_HIT_AURA_SKILL						mSG_HIT_AURA_SKILL;
	SG_RUSH									mSG_RUSH;
	SG_ACTIVE_REACTION_SKILL				mSG_ACTIVE_REACTION_SKILL;
	SG_ICY_STALAGMITE						mSG_ICY_STALAGMITE;
	SG_ACTOR_ENCHANTED_STATUS				mSG_ACTOR_ENCHANTED_STATUS;
	SG_ACTIVE_SKILL_EFFECT					mSG_ACTIVE_SKILL_EFFECT;

	SG_REMOVE_DROPPED_ITEMS					mSG_REMOVE_DROPPED_ITEMS;
	SG_REMOVE_DROPPED_ITEM					mSG_REMOVE_DROPPED_ITEM;
	SG_CHANGE_DROPPED_ITEM_INFO				mSG_CHANGE_DROPPED_ITEM_INFO;
	SG_DROPPING_ITEM						mSG_DROPPING_ITEM;
	SG_DROPPING_ITEMS						mSG_DROPPING_ITEMS;		//	필드에 아이템을 떨어뜨린다.
	SG_PICK_ITEM_RESULT						mSG_PICK_ITEM_RESULT;	//	아이템을 줏었다.(아이템을 줍겠다고 신고한넘한테 날려준다. 나머지한테는 걍 사라졌다고 알려준다.)
	SG_EQUIP_ITEM_RESULT					mSG_EQUIP_ITEM_RESULT;	//	아이템 장비했다고 알려준다.(외관상 변화가 있으면 필드 전체에 전달하고, 아니면 사용한 넘한테만..)
	SG_USE_ITEM_RESULT						mSG_USE_ITEM_RESULT;	//	아이템 사용(주위에 녀석들과 사용한넘 한테 아이템 사용했다고 알려준다.)
	SG_USE_BELT_ITEM_RESULT					mSG_USE_BELT_ITEM_RESULT;//	벨트 아이템 사용 결과.(주위에 녀석들과 사용한넘 한테 아이템 사용했다고 알려준다.)
	SG_USE_ITEM								mSG_USE_ITEM;			//	아이템 사용(주위에 녀석들과 사용한넘 한테 아이템 사용했다고 알려준다.)
	SG_CHANGE_APPEARANCE					mSG_CHANGE_APPEARANCE;
	SG_HIT_ACTION							mSG_HIT_ACTION;

	SG_WARNING_MESSAGE						mSG_WARNING_MESSAGE;
	SG_CHAT									mSG_CHAT;				//	챗
	SG_ADD_TRAP								mSG_ADD_TRAP;
	SG_REMOVE_TRAP							mSG_REMOVE_TRAP;
	SG_EXPLOSION_TRAP						mSG_EXPLOSION_TRAP;

//	━━stress test client
	SG_STRESSCLIENT							mSG_STRESSCLIENT;

//	━━ssl 인증 관련
	SG_COMPLETE_SSL							mSG_COMPLETE_SSL;

//┃서버->클라이언트
//┃게임서버
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

};	//	union	ALL_MSG


#define	dMAX_STORE_PACKET_COUNT		256

class	cPACKET_INFO
{
public:
	enum
	{
		eSendedPacket	=	0x7ffffffe,	//	이미 보낸 패킷
	};

	int				m_iOwner;	//	패킷 주인
	ALL_MSG			m_packet;	//	패킷들..
};
//
//	패킷 큐
class cPACKET_QUE
{
	cPACKET_INFO	m_aPacket[dMAX_STORE_PACKET_COUNT];		//	패킷들..

	int				m_iFirstQue,m_iLastQue,m_iCurrentQue,m_iCount,m_iCounter;		//	첫번째,마지막 패킷, 쌓여 있는패킷 수

public:
	int				m_iMaxPacketCount;
	
					cPACKET_QUE()	{	reset();	}

	void			reset();									//	초기화
	inline	int		getCount(){return	m_iCount;}
	BOOL			addPacket(ALL_MSG *_lpPacket,int _iSerial);	//	패킷 추가
	BOOL			isReceivedPacket(int _iPacket);				//	받은 패킷이냐?
	cPACKET_INFO*	popPacket();								//	큐에서 패킷을 빼낸다.

	cPACKET_INFO*	getOne();									//	큐에서 패킷 하나를 얻어낸다.
	void			setFront()
	{	
		m_iCounter		=	m_iCount;
		m_iCurrentQue	=	m_iFirstQue;
	}

};	//	class cPACKET_QUE

#endif	//	__CPACKET_GAMESERVER_H