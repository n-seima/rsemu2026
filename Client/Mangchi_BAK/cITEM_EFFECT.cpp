#include "cITEM_EFFECT.H"
#include "cITEM_PREFIX_EFFECT.H"
#include "cMAIN.h"
#include "cTEXT.h"
#include "cDRAW.h"
#include "resource.h"

char	cITEM_GENERATE_EFFECT::s_strComboStr[100][20][40]	=
{
	{"수치1","수치2","end"},
};

cKEYWORD_ITEM	cITEM_GENERATE_EFFECT::s_aKeyword[100]	=
{
	"수치"			,0				,
	"end"			,dCODE_END
};


//	최소,최대 수치가 모두 0이면 범위가 -32767 ~ 32767가 됩니다.
//	퍼센티지로 적용되는 수치의 최소,최대 수치가 모두 0이면 디폴트로 -400~40％로 설정됩니다.

//	dMETHOD_ANY	  VALUE,RATE 둘다 적용 가능

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
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기본 수치
"방어력",eIE_ADD_DEFENSE_POWER,
"방어력이 [수치]0 증가한다.",

"방패 블럭률",eIE_BLOCKING_RATE,
"방패의 블럭률이 [수치]0％이다.",

"벨트 스택 아이템수",eIE_BELT_STACK_COUNT,
"벨트 스택 아이템수가 [수치]0개 이다.",

"접두사 생성",eIE_FREE_RARE_OPTION,
"[수치]0개의 제한 없는 접두사 생성이 가능하다.",

"미니펫 소환",eIE_RECALL_MINIPET							,	//	미니펫 소환
"미니펫을 소환한다",

"미니펫 시간 연장",eIE_EXTEND_MINIPET_TIME						,	//	미니펫의 시간을 늘려줌
"미니펫의 소환 시간을 연장한다.",

"모든 미니펫 시간 연장",eIE_EXTEND_ALL_MINIPET_TIME					,	//	모든 미니펫의 시간을 늘려줌
"모든 미니펫의 소환 시간을 연장한다.",

"미니펫 봉인",eIE_SEAL_MINIPET							,	//	미니펫 봉인
"미니펫을 주머니에 봉인 시킨다",

"미니펫 이름 변경",eIE_CHANGE_MINIPET_NAME						,	//	미니펫 이름 변경
"미니펫의 이름을 변경한다",

"미니펫이 담긴 주머니",eIE_MINIPET_POUCH							,	//	미니펫이 담긴 주머니
"미니펫을 담아두는 주머니",

"미니펫 먹이",eIE_FLAKE_OF_DOING_ONE						,
"미니펫 먹이(생체형) [수치]0 [수치]1",

"미니펫 먹이",eIE_FEATHER_OF_THINKING_ONE					,
"미니펫 먹이(에너지형) [수치]0 [수치]1",

"미니펫 먹이",eIE_TOY_OF_SIN_AND_RIN						,
"미니펫 먹이(무기질형) [수치]0 [수치]1",

"미니펫 양면 프레임",eIE_REVERSIBLE_FRAME					,
"미니펫 두마리를 소환 할 수 있게 한다. 기간 [수치]0일 ",

"미니펫 리플렉팅 믹서",eIE_REFLECTING_MIXER,
"미니펫 [수치]0개를 섞어 새로운 미니펫을 소환한다.",

"미니펫 속박의 사슬",eIE_CHAIN_OF_RESTRAINT,
"미니펫을 속박해 기본 스탯 능력치를 향상 시킨다.",

"챕터 5 : 레벨업 칭호 선택",eIE_CH5_SELECT_UPGRADE_TITLE,
"챕터 5 : 레벨업 칭호 선택",

"챕터 5 : 거래 가능",eIE_CH5_RELASE_REVERSION,
"챕터 5 : 거래 불가 아이템을 거래 가능하게 한다.",

"챕터 5 : 길드 스킬 리셋",eIE_CH5_RESET_GUILD_SKILL_POINT,
"챕터 5 : 길드 스킬을 리셋 시킨다.",

"챕터 5 : 체력 증가 부적",eIE_CH5_BONUS_HP_AMULET,
"챕터 5 : 체력을 증가 시켜주는 부적",

"챕터 5 : CP 증가 부적",eIE_CH5_BONUS_CP_AMULET,
"챕터 5 : CP를을 증가 시켜주는 부적",

"챕터 5 : 착용 제한 레벨 증가 부적",eIE_CH5_OVER_POWER_ITEM_EQUIP,
"챕터 5 : 착용 제한 레벨을 증가 시켜주는 부적",

"챕터 5 : 스킬 리버스",eIE_CH5_REVERSE_SKILL_POINT,
"챕터 5 : 스킬 리버스 아이템",

"챕터 5 : 스탯 리버스",eIE_CH5_REVERSE_STATE_POINT,
"챕터 5 : 스탯 리버스 아이템",

"챕터 5 : 접두사 제거",eIE_CH5_REMOVE_PREFIX,
"챕터 5 : 접두사 제거 아이템",

"챕터 5 : 아이템 복사",eIE_CH5_COPY_ITEM,
"챕터 5 : 아이템 복사 아이템",

"챕터 5 : 일반 아이템 접두사 업그레이드",eIE_CH5_UPGRADE_NORMAL_ITEM_PREFIX_POWER,
"챕터 5 : 일반 아이템 접두사 업그레이드",

"챕터 5 : 일반 아이템 성능 향상",eIE_CH5_IMPROVE_NORMAL_ITEM_POWER,
"챕터 5 : 일반 아이템 성능 향상",

"챕터 5 : 유니크 아이템 접두사 업그레이드",eIE_CH5_UPGRADE_UNIQUE_ITEM_PREFIX_POWER,
"챕터 5 : 유니크 아이템 접두사 업그레이드",

"챕터 5 : 유니크 아이템 성능 향상",eIE_CH5_IMPROVE_UNIQUE_ITEM_POWER,
"챕터 5 : 유니크 아이템 성능 향상",

"챕터 5 : 붉은 바닥 없는 상자",eIE_CH5_MIX_ITEM_BOX,
"챕터 5 : 붉은 바닥 없는 상자",

"챕터 5 : 성향 수치 선택",eIE_CH5_SELECT_ALLIGNMENT,
"챕터 5 : 성향 수치 선택",

//┃기본 수치
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃포션/기타 약품 효과
"체력 회복(수치)",eIE_RECOVER_HP,
"체력을 [수치]0포인트 회복 시켜준다.",

"체력 회복2(수치)",eIE_RECOVER_HP2,
"체력을 [수치]0 * 100포인트 회복 시켜준다.",

"체력 즉시 회복(수치)",eIE_INSTANCE_RECOVER_HPx100,
"체력 [수치]0 * 100포인트를 즉시 회복 시켜준다.",

"체력 회복(퍼센티지)",eIE_RECOVER_HP_PERCENTAGE,
"체력 [수치]0％를 회복 시켜준다."							,

"체력 즉시 회복(퍼센티지)",	eIE_INSTANCE_RECOVER_HP_PERCENTAGE		,	//	즉석 체력 회복
"체력 [수치]0％를 즉시 회복 시켜준다."		,

"남은 체력 즉시 감소(퍼센티지)",	eIE_DECREASE_INSTANT_HP_PERCENTAGE		,	//	남은체력 즉시 감소(퍼센티지)
"남은 체력 [수치]0％를 즉시 감소 시킨다."		,

"CP 충전(수치)",eIE_RECOVER_CP,
"CP를 [수치]0포인트 충전 시켜준다."								,

"CP 충전2(수치)",eIE_RECOVER_CP2,
"CP를 [수치]0 * 100포인트 충전 시켜준다."								,

"CP 충전(퍼센티지)",eIE_RECOVER_CP_PERCENTAGE,
"CP를 [수치]0％만큼 충전 시켜준다."								,

"HP/CP 회복",eIE_RECOVER_HP_CP,
"캐릭터의 체력과 CP를 [수치]0포인트 회복 시킨다."				,

"체력/CP 같이 회복",eIE_RECOVER_HP_CP_PERCENTAGE,
"캐릭터의 체력과 CP를 [수치]0％ 회복 시킨다."				,

"타인 부활"	,eIE_RESURRECTION,
"전투불능의 캐릭터를 부활 시킨후 [수치]0％의 체력을 회복 시킨다.",

"본인 부활",eIE_SELF_RESURRECTION,
"자신이 전투불능이 되었을 경우 부활 시킨후 [수치]0％의 체력을 회복 시킨다.",

"파티 완전 부활",eIE_PERFECT_PARTY_REVIVE,
"전투불능 상태에 빠진 파티원 모두를 완전 부활 시키고 [수치]0초 동안 최대 HP,CP를 [수치]1％증가 시킨다.",

"중화",eIE_HEAL_STRANGE_STATUS,
"상태이상 모든 수치를 [수치]0％로 만든다.",		

"이상계 상태 치료",eIE_RECOVER_ALL_STRANGE_STATUS,
"모든 이상계 상태 이상을 치료한다.",

"저하계 상태 치료",eIE_RECOVER_ALL_DECLINE_STATUS,
"모든 저하계 상태 이상을 치료한다.",

"저주계 상태 치료",eIE_RECOVER_ALL_CURSE_STATUS,
"모든 저주계 상태 이상을 치료한다.",

"모든 상태 이상 치료",eIE_RECOVER_ALL_BADSTATUS,
"모든 상태 이상을 치료한다.",

"중독 치료",eIE_HEAL_POSION,
"중독 상태를 치료한다.",

"일시적으로 힘을 상승 시킨다.",eIE_SHORT_WHILE_INCREASE_POWER,
"힘을 [수치]0만큼 [수치]1초 동안 상승 시킨다.",

"일시적으로 민첩성을 상승 시킨다.",eIE_SHORT_WHILE_INCREASE_AGILITY,
"민첩성을 [수치]0만큼 [수치]1초 동안 상승 시킨다.",

"일시적으로 건강을 상승 시킨다.",eIE_SHORT_WHILE_INCREASE_CONSTITUTION,
"건강을 [수치]0만큼 [수치]1초 동안 상승 시킨다.",

"일시적으로 지혜를 상승 시킨다.",eIE_SHORT_WHILE_INCREASE_WISDOM,
"지혜를 [수치]0만큼 [수치]1초 동안 상승 시킨다.",

"일시적으로 지식을 상승 시킨다.",eIE_SHORT_WHILE_INCREASE_INTELIGENCE,
"지식을 [수치]0만큼 [수치]1초 동안 상승 시킨다.",

"일시적으로 카리스마를 상승 시킨다.",eIE_SHORT_WHILE_INCREASE_CHARISMA,
"카리스마를 [수치]0만큼 [수치]1초 동안 상승 시킨다.",

"일시적으로 행운을 상승 시킨다.",eIE_SHORT_WHILE_INCREASE_LUCK,
"행운을 [수치]0만큼 [수치]1초 동안 상승 시킨다.",

"일시적으로 힘을 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_POWER_BY_MINUTE,
"힘을 [수치]0만큼 [수치]1분 동안 상승 시킨다.",

"일시적으로 민첩성을 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_AGILITY_BY_MINUTE,
"민첩성을 [수치]0만큼 [수치]1분 동안 상승 시킨다.",

"일시적으로 건강을 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_CONSTITUTION_BY_MINUTE,
"건강을 [수치]0만큼 [수치]1분 동안 상승 시킨다.",

"일시적으로 지혜를 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_WISDOM_BY_MINUTE,
"지혜를 [수치]0만큼 [수치]1분 동안 상승 시킨다.",

"일시적으로 지식을 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_INTELIGENCE_BY_MINUTE,
"지식을 [수치]0만큼 [수치]1분 동안 상승 시킨다.",

"일시적으로 카리스마를 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_CHARISMA_BY_MINUTE,
"카리스마를 [수치]0만큼 [수치]1분 동안 상승 시킨다.",

"일시적으로 행운을 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_LUCK_BY_MINUTE,
"행운을 [수치]0만큼 [수치]1분 동안 상승 시킨다.",

"일시적으로 행운을 상승 시킨다.(분)x100",eIE_SHORT_WHILE_INCREASE_LUCKx100_BY_MINUTE,
"행운을 [수치]0 x 100만큼 [수치]1분 동안 상승 시킨다.",



"일시적으로 공격력을 증가 시킨다.",eIE_SHORT_WHILE_INCREASE_ATTACK_POWER,
"[수치]0초 동안 공격력을 [수치]1％ 증가 시킨다.",

"일시적으로 방어력을 증가 시킨다.",eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER,
"[수치]0초 동안 방어력을 [수치]1％ 증가 시킨다.",

"일시적으로 체력을 증가 시킨다.",eIE_SHORT_WHILE_INCREASE_HP,
"[수치]0초 동안 체력을 [수치]1％ 증가 시킨다.",

"일시적으로 CP를 증가 시킨다.",eIE_SHORT_WHILE_INCREASE_CP,
"[수치]0초 동안 CP를 [수치]1％ 증가 시킨다.",

"일시적으로 무기 대미지 맥스로 유지",eIE_SHORT_WHILE_SET_MAX_DAMAGE,
"[수치]0초 동안 무기 대미지가 항상 최고로 유지된다.",

"일시적으로 CP 맥스로 유지",eIE_SHORT_WHILE_SET_MAX_CP,
"[수치]0초 동안 CP가 항상 최고로 유지된다.",

"체력 수치 변경",eIE_CHANGE_HP,
"현재 체력을 [수치]0 x [수치]1로 변경한다.",

//┃포션/기타 약품 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기타 소모성 아이템
"보물 상자 열기",eIE_OPEN_ARCA,
"[수치]0레벨 이하의 보물상자를 연다.",

"문 열기",eIE_OPEN_DOOR,
"[수치]0레벨 이하의 문을 연다.",

"문과 보물 상자 열기",eIE_OPEN_ARCA_AND_DOOR,
"[수치]0레벨 이하의 문과 보물상자를 연다.",

"편도용 포탈",eIE_ONE_WAY_PORTAL,
"마을로 귀환한다.",

"왕복용 포탈",eIE_PORTAL,
"마을로 왕복 할 수 있는 문을 연다.",

"보석 사용",eIE_USE_JEWEL,
"생성시 붙은 레어 옵션을 사용 할 수 있다.([수치]0분 동안 유지,사용 횟수 [수치]1)",

"메모리 스톤",eIE_MEMORY_STONE,
"웨이 포인트 슬롯 [수치]0에 현재 위치를 기억하고, 관련 아이템을 [수치]1개 생성한다.",

"텔레포트 스톤",eIE_TELEPORT_STONE,
"웨이 포인트 슬롯 [수치]0에 기억하고 있는 장소로 텔레포트 한다.",

"포탈 스톤",eIE_PORTAL_STONE,
"웨이 포인트 슬롯 [수치]0에 기억하고 있는 장소로 통하는 포탈을 연다.",

"OX 퀴즈 티켓",eIE_OX_QUIZ_TICKET,	//	OX 퀴즈 티켓이다.
"OX 퀴즈 티켓이다.",

"마력 충전",eIE_CHARGE_EXTRA_ITEM_POWER	,	//	마력 충전
"엑스트라 아이템의 마력을 100％ 충전해 준다.",

"04년 설날 이벤트",eIE_EVENT_2004_01_21	,	//	마력 충전
"04년 설날 임시로 만든 옵션. 현재는 폐기됨",

"책",eIE_BOOK							,
"책이다.",

"책에 카테고리 추가",eIE_ADD_CATEGORY							,
"책 아이템에 카테고리를 추가한다.",

"책에 페이지 추가",eIE_ADD_PAGE		,
"책 아이템에 페이지를 추가한다.",

"최종 대미지 증가",	eIE_INC_LAST_DAMAGE							,	//	최종 대미지 중가
"최종 대미지가 물리 마법 구분없이 [수치]0분간 [수치]1x10％ 증가한다.",

"사냥 경험치 증가",	eIE_INC_HUNT_EXP								,	//	사냥 경험치 증가
"몬스터 사냥으로 획득하는 경험치가 [수치]0분간 [수치]1x10％ 증가한다.",

"아이템 드랍 확률 증가",	eIE_INC_HUNT_ITEM_DROP_CHANCE								,	//	사냥 경험치 증가
"아이템 드랍 확률이 [수치]0분간 [수치]1x10％ 증가한다.",

"죽음 페널티 회복",	eIE_CLEAR_DEATH_PENELTY						,	//	죽음 페널티 회복
"죽음 페널티를 모두 회복 시킨다.",

"이동 속도 증가",	eIE_INCREASE_MOVE_SPEED						,	//	이속 증가
"이동 속도가 [수치]0분간 [수치]1％ 증가한다.",

"이벤트 풍선",	eIE_EVENT_BALLOON			,	//	이벤트 풍선
"몬스터를 잡는 순간 일정 확률로 터지고 경험치가 [수치]0배 된다.",


"이벤트 풍선",	eIE_EVENT_BALLOON_INC_ITEM_DROP_CHANCE			,	//	이벤트 풍선 아이템 드랍률
"몬스터를 잡는 순간 일정 확률로 터지고 아이템 드랍률이 [수치]0배 된다.",

"미니펫 먹이",	eIE_MINIPET_FEED			,
"[수치]0 (1 = Bio, 2 = Energy, 3 = Mineral) 타입의 [수치]1 만 경험치를 준다",

"미니펫 스킬 초기화",	eIE_RESET_MINIPET_SKILL			,
"미니펫 스킬을 초기화 시킨다.",

"미니펫 타입 변경",	eIE_SET_MINIPET_TYPE			,	
"[수치]0 (1 = Bio, 2 = Energy, 3 = Mineral) 타입 100％로 변경한다.",

"특정 필드 이동",eIE_MOVE_FIELD,
"이동할 필드 시리얼에 적인 필드로 이동한다.",
//┃기타 소모성 아이템
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃공격 부가 효과/부가 대미지
"즉사",eIE_INSTANCE_KILL,
"[수치]0％의 확률로 타겟이 즉사한다.",

"대상을 버서크 상태로 만듦",eIE_MAKE_BERSERKER,
"대상을 [수치]0％의 확률로 [수치]1초 동안 버서크 상태로 만든다.",

"갑옷 파괴",eIE_MAKE_ARMOR_BREAK,
"대상을 [수치]0％의 확률로 [수치]1초 동안 갑옷파괴 상태로 만든다.",

"무기 파괴",eIE_MAKE_WEAPON_BREAK,
"대상을 [수치]0％의 확률로 [수치]1초 동안 무기파괴 상태로 만든다.",

"중독시킨다.",eIE_MAKE_POISONED,
"중독시킨다.(대미지 : [수치]0초 동안 초당 [수치]1)",

"혼란시킨다.",eIE_MAKE_CONFUSE,
"대상을 [수치]0％의 확률로 [수치]1초 동안 혼란 상태로 만든다.",

"유혹",eIE_MAKE_CHARMING,
"대상을 [수치]0％의 확률로 [수치]1초 동안 유혹한다.",

"유혹(30초)",eIE_MAKE_CHARMING_30,
"대상을 [수치]0％의 확률로 30초 동안 유혹한다.",

"스턴",eIE_MAKE_STUN,
"대상을 [수치]0％의 확률로 [수치]1Frame 동안 스턴 상태로 만든다.",

"콜드",eIE_MAKE_COLD,
"대상을 [수치]0％의 확률로 [수치]1초 동안 콜드 상태로 만든다.",

"콜드(100％)",eIE_MAKE_COLD_100,
"대상을 100％의 확률로 [수치]0초 동안 콜드 상태로 만든다.",

"불속성 대미지",eIE_HIT_FIRE_DAMAGE,
"불속성 대미지를 [수치]0 입힌다.",

"물속성 대미지",eIE_HIT_WATER_DAMAGE,
"물속성 대미지를 [수치]0 입힌다.",

"바람속성 대미지",eIE_HIT_WIND_DAMAGE,
"바람속성 대미지를 [수치]0 입힌다.",

"대지속성 대미지",eIE_HIT_TERRA_DAMAGE,
"대지속성 대미지를 [수치]0 입힌다.",

"빛속성 대미지",eIE_HIT_LIGHT_DAMAGE,
"빛속성 대미지를 [수치]0 입힌다.",

"어둠속성 대미지",eIE_HIT_DARK_DAMAGE,
"어둠속성 대미지를 [수치]0 입힌다.",

"언데드 사냥꾼",eIE_ADD_DAMAGE_TO_UNDEAD,
"언데드형 캐릭터에게 추가로 [수치]0％의 대미지를 준다.",

"악마 사냥꾼",eIE_ADD_DAMAGE_TO_DEMON,
"악마형 캐릭터에게 추가로 [수치]0％의 대미지를 준다.",

"인간 사냥꾼",eIE_ADD_DAMAGE_TO_HUMAN,
"인간형 캐릭터에게 추가로 [수치]0％의 대미지를 준다.",

"동물 사냥꾼",eIE_ADD_DAMAGE_TO_ANIMAL,
"동물형 캐릭터에게 추가로 [수치]0％의 대미지를 준다.",

"신수 사냥꾼",eIE_ADD_DAMAGE_TO_HOLY_ANIMAL,
"신수형 캐릭터에게 추가로 [수치]0％의 대미지를 준다.",
//┃공격 부가 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 부가 효과
"벨트에 아이템 채우는 시간 감소",eIE_DECREASE_RELOAD_TIME,
"벨트에 아이템 채우는 시간이 [수치]0％ 감소한다.",

"변신 속도 빨라짐",eIE_INCREASE_TRANSFORMATION_SPEED,
"변신 속도가 [수치]0％ 빨라진다.",

"물약에 의한 회복 속도 증가",eIE_INCREASE_RECOVER_SPEED,
"물약 회복 속도가 [수치]0％ 빨라진다.",

"가시돋힘",eIE_RETURN_DAMAGE,
"받은 대미지의 [수치]0％를 적에게 될려준다.",

"가시돋힘 40％",eIE_RETURN_DAMAGE_40,
"받은 대미지의 40％를 적에게 될려준다.",

"무한 탄환",eIE_INFINITY_BULLET,
"무한 탄환",

"체력 회복 속도 증가",	eIE_INCREASE_SITDOWN_RECOVER_HP_SPEED		,	//	앉아 있을때 체력 회복 속도를 높혀준다.
"앉아 있을때 체력 회복 속도를 [수치]0％ 높혀준다.",

"사냥 경험치 증가",	eIE_HUNTING_EXP_BONUS,	//	앉아 있을때 체력 회복 속도를 높혀준다.
"1~200 사이 사냥으로 획득하는 경험치가 증가한다.",

"물리 대미지 흡수",	eIE_AB_PH_DAMAGE	,	
"[수치]0％ 확률로 물리 대미지의 [수치]1％를 체력으로 흡수한다.",

//┃각종 부가 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 수치 증가
"밀리 직업들 공격력 상승",eIE_INCREASE_MELEE_JOB_ATTACK_POWER,
"전사,견습기사,늑대인간,사제,무도가,마법창병의 공격력이 [수치]0~[수치]1증가한다.",

"선택한 능력치 상승",eIE_INCREASE_SELECT_POWER,
"선택한 능력치가 [수치]0 증가한다. [수치]1(값을 0으로 설정 할 것)",

"힘 상승",eIE_INCREASE_POWER,
"힘이 [수치]0 증가한다.",

"지혜 상승",eIE_INCREASE_WISDON,
"지혜가 [수치]0 증가한다.",

"지식 상승",eIE_INCREASE_INTELIGENCE,
"지식이 [수치]0 증가한다.",

"건강 상승",eIE_INCREASE_CONSTITUTION,
"건강이 [수치]0 증가한다.",

"민첩성 상승",eIE_INCREASE_AGILITY,
"민첩성이 [수치]0 증가한다.",

"카리스마 상승",eIE_INCREASE_CHARISMA,
"카리스마가 [수치]0 증가한다.",

"행운 상승",eIE_INCREASE_LUCK,
"행운이 [수치]0 증가한다.",

"크리티컬 발생확률",eIE_INCREASE_CRITICAL,
"크리티컬 발생확률이 [수치]0 증가한다.",

"결정타 발생확률",eIE_INCREASE_CRUSH_SHOT,
"결정타 발생확률이 [수치]0 증가한다.",

"명중률 증가",eIE_INCREASE_HIT_RATE,
"명중률이 [수치]0 증가한다.",

"회피율 증가",eIE_INCREASE_DODGE_CHANCE,
"회피율이 [수치]0 증가한다.",

"집중력 증가",eIE_INCREASE_CONCENTRATION,
"집중력이 [수치]0 증가한다.",

"공격 속도 증가",eIE_INCREASE_ATTACK_SPEED,
"공격 속도가 [수치]0％ 증가한다.",

"이동 속도 증가",eIE_EQUIP_INCREASE_MOVE_SPEED,
"이동 속도가 [수치]0％ 증가한다.",

"모든 능력치 상승",eIE_INCREASE_ALL_STATUS,
"모든 능력치 [수치]0 증가한다.",

"선택한 능력치 상승(고정)",eIE_INCREASE_FIX_SELECT_POWER,
"선택한 능력치가 [수치]0 증가한다. [수치]1(값을 0으로 설정 할 것)",

"최대 체력 증가(x 100)",eIE_ADD_MAX_HPx100,
"최대 체력 [수치]0 x 100 증가",

"최대 CP 증가(x 100)",eIE_ADD_MAX_CPx100,
"최대 CP [수치]0 x 100 증가",

"방어력 ％ 증가",eIE_ADD_DEFENSE_PERCENTAGE,
"방어력 [수치]0 ％ 증가한다.",

"최대 체력 ％ 증가",eIE_ADD_MAX_HP_PERCENTAGE,
"최대 체력 [수치]0 ％ 증가한다.",

"최대 CP ％ 증가",eIE_ADD_MAX_CP_PERCENTAGE,
"최대 CP [수치]0 ％ 증가한다.",

"붉은 보석 획득 가능 레벨 [수치]0 증가",eIE_INCREASE_LEVEL_GETING_RED_STONE,
"붉은 보석 획득 가능 레벨 [수치]0 증가한다.",

//┃각종 수치 증가
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃저항
"저주 상태에 대한 저항",eIE_RESISTANCE_CURSE_STATUS,
"저주 상태에 대한 저항이 [수치]0 증가한다.",

"저하 상태에 대한 저항",eIE_RESISTANCE_DECLINE_STATUS,
"저하 상태에 대한 저항이 [수치]0 증가한다.",

"이상 상태에 대한 저항",eIE_RESISTANCE_STRANGE_STATUS,
"이상 상태에 대한 저항이 [수치]0 증가한다.",

"모든 상태 이상에 대한 저항",eIE_RESISTANCE_ALL_BAD_STATUS,
"모든 상태 이상에 대한 저항이 [수치]0 증가한다.",

"불저항",eIE_INCREASE_FIRE_RESITANCE,
"불저항이 [수치]0％ 증가한다.",

"대지저항",eIE_INCREASE_TERRA_RESITANCE,
"대지저항이 [수치]0％ 증가한다.",

"바람저항",eIE_INCREASE_WIND_RESITANCE,
"바람저항이 [수치]0％ 증가한다.",

"물저항",eIE_INCREASE_WATER_RESITANCE,
"물저항이 [수치]0％ 증가한다.",

"빛저항",eIE_INCREASE_LIGHT_RESITANCE,
"빛저항이 [수치]0％ 증가한다.",

"어둠저항",eIE_INCREASE_DARK_RESITANCE,
"어둠저항이 [수치]0％ 증가한다.",

"불,물,바람,대지에 대한 저항",eIE_INCREASE_FOUR_ELEMENTAL_RESITANCE,
"불,물,바람,대지에 대한 저항이 [수치]0％ 증가한다.",

"마법 속성 공격에 대한 저항",eIE_INCREASE_MAGIC_RESITANCE,
"마법 속성 공격에 대한 저항이 [수치]0％ 증가한다.",

"녹백 저항",eIE_INCREASE_NOCK_BACK_RESISTANCE,
"녹백에 대한 저항이 [수치]0％ 증가한다.",

"치명타 저항",eIE_INCREASE_CRITICAL_RESISTANCE,
"치명타에 대한 저항이 [수치]0 증가한다.",

"결정타 저항",eIE_INCREASE_CRUSH_SHOT_RESISTANCE,
"결정타에 대한 저항이 [수치]0 증가한다.",

"즉사 저항",eIE_INCREASE_INSTANCE_KILL_RESISTANCE,
"즉사에 대한 저항이 [수치]0 증가한다.",
//┃저항
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃스킬레벨
"마법 창병 스킬 레벨 증가",eIE_INCREASE_MAGIC_LANCER_SKILL,
"마법 창병의 스킬 레벨이 [수치]0 상승한다.",

"마법 궁수 스킬 레벨 증가",eIE_INCREASE_MAGIC_ARCHER_SKILL,
"마법 궁수의 스킬 레벨이 [수치]0 상승한다.",

"견습 기사 스킬 레벨 증가",eIE_INCREASE_KNIGHT_SKILL,
"견습 기사의스킬 레벨이 [수치]0 상승한다.",

"전사 계열 스킬 레벨 증가",eIE_INCREASE_WARRIOR_SKILL,
"전사의스킬 레벨이 [수치]0 상승한다.",

"마법사 스킬 레벨 증가",eIE_INCREASE_WIZARD_SKILL,
"마법사의 스킬 레벨이 [수치]0 상승한다.",

"늑대인간 스킬 레벨 증가",eIE_INCREASE_WEREWOLF_SKILL,
"늑대인간의 스킬 레벨이 [수치]0 상승한다.",

"도둑 스킬 레벨 증가",eIE_INCREASE_THIEF_SKILL,
"도둑의스킬 레벨이 [수치]0 상승한다.",

"무도가 스킬 레벨 증가",eIE_INCREASE_FIGHTER_SKILL,
"무도가의스킬 레벨이 [수치]0 상승한다.",

"조련사 스킬 레벨 증가",eIE_INCREASE_BEAST_TAMER_SKILL,
"조련사의스킬 레벨이 [수치]0 상승한다.",

"소환사 스킬 레벨 증가",eIE_INCREASE_SUMMONER_SKILL,
"소환사의스킬 레벨이 [수치]0 상승한다.",

"공주 스킬 레벨 증가",eIE_INCREASE_PRINCESSE_SKILL,
"공주의스킬 레벨이 [수치]0 상승한다.",

"변신 소녀 스킬 레벨 증가",eIE_INCREASE_MAGICAL_GIRL_SKILL,
"변신 소녀의스킬 레벨이 [수치]0 상승한다.",

"사제 스킬 레벨 증가",eIE_INCREASE_PRIEST_SKILL,
"사제의스킬 레벨이 [수치]0 상승한다.",

"타락천사 스킬 레벨 증가",eIE_INCREASE_FALLEN_ANGEL_SKILL,
"타락천사의스킬 레벨이 [수치]0 상승한다.",

"강신술사 스킬 레벨 증가",eIE_INCREASE_NECROMANCER_SKILL,
"강신술사의스킬 레벨이 [수치]0 상승한다.",

"악마 스킬 레벨 증가",eIE_INCREASE_DEMON_SKILL,
"악마의스킬 레벨이 [수치]0 상승한다.",

"영술사 스킬 레벨 증가",eIE_INCREASE_SOUL_BRINGER_SKILL,
"영술사의 스킬 레벨이 [수치]0 상승한다.",

"여성 캐릭터들의 스킬 레벨 증가",eIE_INCREASE_FEMALE_SKILL_LEVEL,
"여성 캐릭터들의 스킬 레벨이 [수치]0 증가한다.",

"남성 캐릭터들의 스킬 레벨 증가",eIE_INCREASE_MALE_SKILL_LEVEL,
"남성 캐릭터들의 스킬 레벨이 [수치]0 증가한다.",

"모든 스킬 레벨 증가",eIE_INCREASE_SKILL_LEVEL,
"모든 스킬의 레벨이 [수치]0 증가한다.",
//┃스킬레벨
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃신비한 마법 아이템
"레벨 상승",eIE_INCREASE_LEVEL,
"레벨 [수치]0이 영원히 상승.",

"월드맵으로 필드 이동",	eIE_JUMP_FIELD_USE_WORLD_MAP,
"월드맵을 이용해 필드를 이동한다.(쿨타임 있음)",

"월드맵 상에 원하는 지점으로 텔레포트 한다.",	eIE_USE_WORLD_MAP_TELEPORT	,	//	
"월드맵 상에 원하는 지점으로 텔레포트 한다.(쿨타임 없음)",

"여행자용 가방"	,	eIE_EXPAND_INVENTORY_BAG,
"여행자용 가방의 크기를 [수치]0칸 확장 시킨다."	,

"아이템 상자"							,	eIE_LOTTERY_ITEM,	//	길드 생성
"[수치]0 번 아이템 팩의 아이템들중 하나를 랜덤하게 아이템을 생성한다."	,

"선택 가능한 아이템 상자"							,	eIE_SELECT_ITEM_IN_ITEM_PACK,	//	길드 생성
"[수치]0 번 아이템 팩의 아이템들중 하나를 선택한다."	,

"힘 증가",eIE_GROW_STRENGTH,
"[수치]0 포인트의 힘이 영원히 상승. (성공 확률 : [수치]1％) 실패시 사망",

"민첩성 증가",eIE_GROW_AGILITY,
"[수치]0 포인트의 민첩성이 영원히 상승. (성공 확률 : [수치]1％) 실패시 사망",

"건강 증가",eIE_GROW_CONSTITUTION,
"[수치]0 포인트의 건강이 영원히 상승. (성공 확률 : [수치]1％) 실패시 사망",

"지혜 증가",eIE_GROW_WISDOM,
"[수치]0 포인트의 지혜가 영원히 상승. (성공 확률 : [수치]1％) 실패시 사망",

"지식 증가",eIE_GROW_INTELLIGENCE,
"[수치]0 포인트의 지식이 영원히 상승. (성공 확률 : [수치]1％) 실패시 사망",

"카리스마 증가",eIE_GROW_CHARISMA,
"[수치]0 포인트의 카리스마가 영원히 상승. (성공 확률 : [수치]1％) 실패시 사망",

"행운 증가",eIE_GROW_LUCK,
"[수치]0 포인트의 행운이 영원히 상승. (성공 확률 : [수치]1％) 실패시 사망",

"체력 증가",eIE_GROW_HP,
"[수치]0 포인트의 HP가 영원히 상승. (실패 확률 : [수치]0*[수치]1％) 실패시 사망",

"CP 증가",eIE_GROW_CP,
"[수치]0 포인트의 CP가 영원히 상승. (실패 확률 : [수치]0*[수치]1％) 실패시 사망",

"무기 대미지 증가",eIE_INCREASE_WEAPON_DAMAGE,
"무기 대미지 [수치]0％ 영원히 상승, 100-100/([수치]0+0.65)％ 확률 무기 증발",

"갑옷 방어력 증가",eIE_INCREASE_ARMOR_DEFENSE,
"갑옷 방어력 [수치]0 포인트 영원히 상승, 100-100/([수치]0+0.65)％ 확률로 갑옷 증발",

"마법 보석",eIE_MAGIC_JEWEL,
"무기에 [수치]0개의 접두사를 붙인다. (성공확률 : [수치]1％) 실패시 아이템 증발",

"프론티어 증서",eIE_FRONTIER_SCROLL					,
"사용자를 [수치]0 레벨의 프론티어로 만들어 준다.",

"노말 아이템 복권",eIE_NORMAL_ITEM_LOTTERY_TICKET,
"드롭레벨 [수치]0~드롭레벨 [수치]1 사이의 노말 아이템을 랜덤하게 생성한다. 성공확률 100％",

"레어 아이템 복권",eIE_RARE_ITEM_LOTTERY_TICKET		,
"드롭레벨 [수치]0의 레어 아이템을 랜덤하게 생성한다. 성공확률 [수치]1％",

"유니크 아이템 복권",eIE_UNIQUE_ITEM_LOTTERY_TICKET	,
"드롭레벨 [수치]0의 유니크 아이템을 랜덤하게 생성한다. 성공확률 [수치]1％",

"슈퍼 아이템 복권",eIE_SUPER_ITEM_LOTTERY_TICKET	,
"드롭레벨 [수치]0의 슈퍼 아이템을 랜덤하게 생성한다. 성공확률 [수치]1％",

"스킬 포인트 재분배",eIE_RESET_SKILL_POINT	,
"스킬 포인트를 재분배 한다.",

"스탯 포인트 재분배",eIE_RESET_STATE_POINT	,	//	스탯 초기화
"스탯 포인트를 재분배 한다.",


"힘 스탯을 재분배 한다.",eIE_RESET_STRENGTH_STATE		,	//	힘 스탯 하나 재 분배
"힘 스탯을 재분배 한다.",

"민첩 스탯을 재분배 한다.",eIE_RESET_AGILITY_STATE			,	//	민첩 스탯 하나 재 분배
"민첩 스탯을 재분배 한다.",

"건강 스탯을 재분배 한다.",eIE_RESET_CONSTITUTION_STATE	,	//	건강 스탯 하나 재 분배
"건강 스탯을 재분배 한다.",

"지식 스탯을 재분배 한다.",eIE_RESET_INTELLIGENCE_STATE	,	//	지식 스탯 하나 재 분배
"지식 스탯을 재분배 한다.",

"지혜 스탯을 재분배 한다.",eIE_RESET_WISDOM_STATE			,	//	지혜 스탯 하나 재 분배
"지혜 스탯을 재분배 한다.",

"카리스마 스탯을 재분배 한다.",eIE_RESET_CHARISMA_STATE		,	//	카리스마 스탯 하나 재 분배
"카리스마 스탯을 재분배 한다.",

"운 스탯을 재분배 한다.",eIE_RESET_LUCK_STATE			,	//	운 스탯 하나 재 분배
"운 스탯을 재분배 한다.",


"접두사 변경",eIE_CHANGE_ITEM_PREFIX_2003_11_01	,	//	접두사 변경
"접두사를 변경한다.",

"위험한 접두사 변경",eIE_CHANGE_ITEM_PREFIX_2003_11_17,
"더 낮은 단계라도 가능한 접두사를 찾아서 변경한다.",

"산타의 오브",eIE_ORB_OF_SANTA							,
"산타의 오브 경험치 1.5배, 아이템 드롭율 1.5배, 마을 무제한 귀환 가능, 프리미엄 존 진입, 타운 텔레포터 무료 사용, 마법의 양탄자 무한 소환 가능",

"일시적으로 최종 대미지를 증가 시킨다.",eIE_INCREASE_FINAL_DAMAGE,
"[수치]0분 동안 최종 대미지를 [수치]1％ 증가 시킨다.",

"일시적으로 공격력을 증가 시킨다.",eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE,
"[수치]0분 동안 공격력을 [수치]1％ 증가 시킨다.",

"일시적으로 공격력을 증가 시킨다.",eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE_10,
"[수치]0분 동안 공격력을 [수치]1*10％ 증가 시킨다.",

"일시적으로 방어력을 증가 시킨다.",eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE,
"[수치]0분 동안 방어력을 [수치]1％ 증가 시킨다.",

"일시적으로 방어력을 증가 시킨다.",eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE_10	,
"[수치]0분 동안 방어력을 [수치]1*10％ 증가 시킨다.",

"일시적으로 체력을 증가 시킨다.",eIE_SHORT_WHILE_INCREASE_HP_BY_MINUTE,
"[수치]0분 동안 체력을 [수치]1％ 증가 시킨다.",

"일시적으로 CP를 증가 시킨다.",eIE_SHORT_WHILE_INCREASE_CP_BY_MINUTE,
"[수치]0분 동안 CP를 [수치]1％ 증가 시킨다.",

"일시적으로 무기 대미지 맥스로 유지",eIE_SHORT_WHILE_SET_MAX_DAMAGE_BY_MINUTE,
"[수치]0분 동안 무기 대미지가 항상 최고로 유지된다.",

"일시적으로 CP 맥스로 유지",eIE_SHORT_WHILE_SET_MAX_CP_BY_MINUTE,
"[수치]0분 동안 CP가 항상 최고로 유지된다.",

"자신 레벨보다 높은 제한 레벨 아이템 사용가능",eIE_OVER_POWER_ITEM_EQUIP,
"자신 레벨보다 [수치]0레벨 높은 제한 레벨 아이템 사용 가능",

"완전 부활",eIE_PERFECT_RESSURECTION,
"전투불능 패널티 시간 90％단축하고 [수치]0초간 최대 체력을 [수치]1％로 한다.",		// 09.10.06 수정

"사육 기록",eIE_BREEDING_RECORD							,
"펫을 문서화 시킨다.",

"비밀 던전 열쇠",eIE_SECRET_DUNGEON_KEY	,
"비밀 던전 열쇠다.",

"보물 지도",	eIE_TREASURE_MAP		,	//	보물 지도
"보물 지도다",

"커플 링 세트",eIE_COUPLE_RING_SET	,
"커플링을 생성한다.",

"커플 링",eIE_COUPLE_RING	,
"커플 링",

"증정용 커플 링",eIE_PRESENT_COUPLE_RING	,
"증정용 커플 링",

"상대 없는 커플 링",eIE_NOT_DEFINE_COUPLE_COUPLE_RING	,
"상대 없는 커플 링",

"바닥 없는 상자",eIE_MIX_ITEM_BOX	,	//	바닥 없는 상자
"바닥 없는 상자",

"불꽃 놀이 세트",eIE_FIRE_WORK,
"[수치]0번 불꽃을 [수치]1초 동안 터뜨린다.",

"무작위 불꽃 놀이",	eIE_RANDOM_FIRE_WORK,
"무작위로 하나의 불꽃을 선택해 터뜨린다.",

"공주 변신 무기",eIE_PRINCESS_WEAPON,
"공주 변신 무기이다.",

"운영자로 변신",eIE_TRANS_TO_OPERATOR,
"운영자로 변신한다.",

"새벽의 봉인구",	eIE_SEAL_SPHERE_OF_DAWN	,	//	새벽의 봉인구 
"[수치]0*10레벨 이하의 아이템 하나를 능력에 상관 없이 사용 가능하게 만들어 준다.",

//┃신비한 마법 아이템
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃인챈트 아이템
"짐모리의 인챈트 서비스",eIE_JIM_MORYS_ENCHANT			,	//	짐 모리의 인챈트
"짐모리의 인챈트 서비스",

"서투른 아이템 수리",eIE_UNSKILLED_REPAIR_BROKEN_ITEM	,	//	
"부숴진 아이템을 서투르게 수리한다.",

"아이템 인챈트",eIE_ENCHANT_ITEM_BY_UNIQUE_INFO			,	//	짐 모리의 인챈트
"유니크 아이템을 개량한다.",

"부숴진 아이템 수리",eIE_REPAIR_BROKEN_ITEM				,	//	"부숴진 아이템 수리",
"부숴진 아이템을 완벽하게 수리한다.",

"장비 해체 불가 저주 제거",eIE_CURE_ITEM_EQUIP_CURSE		,	//	"장비에 붙어 있는 저주를 풀어준다.
"아이템에 붙은 장비 해체 불가 저주를 없애준다."				,

"인챈트 불가 저주 제거",eIE_RELEASE_ATTACH_PREFIX_CURSE		,	//	"장비에 붙어 있는 저주를 풀어준다.
"아이템에 붙은 인챈트 불가 저주를 제거한다."				,

"DX 아이템 수리",	eIE_REPAIR_DX_ITEM_KIT					,
"DX 아이템의 내구도를 [수치]0％ 수리한다."					,

"아이템 조각",	eIE_ITEM_PIECE,
"[수치]0 번째 아이템 조각",

"접두사 제거",eIE_REMOVE_PREFIX,
"접두사를 제거한다.",

"접두사 붙이기",eIE_ATTACH_PREFIX,
"[수치]0 확률로 접두사를 붙인다.",

"접두사 복사하기",eIE_COPY_PREFIX,
"[수치]0 확률로 접두사를 복사한다.",

"접두사 잘라내기",eIE_CUT_PREFIX,
"접두사를 잘라낸다.(생성 아이템 옵션 : [수치]0％ 확률)",

"접두사 붙이기",eIE_ATTACH_PREFIX_CAN_NOT_TRADE,
"[수치]0 확률로 접두사를 붙인다.(거래불가)",

"접두사 복사하기",eIE_COPY_PREFIX_CAN_NOT_TRADE,
"[수치]0 확률로 접두사를 복사한다.(거래불가)",

"접두사 잘라내기",eIE_CUT_PREFIX_CAN_NOT_TRADE,
"접두사를 잘라낸다.(생성 아이템 옵션 : [수치]0％ 확률, 거래불가)",

//┃인챈트 아이템
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

"타겟의 불 저항 약화",eIE_WEAKNING_TARGET_FIRE_RESISTANCE		,
"타겟의 불 저항을 [수치]0％ 약화 시킨다.",
"타겟의 물 저항 약화",eIE_WEAKNING_TARGET_WATER_RESISTANCE		,
"타겟의 물 저항을 [수치]0％ 약화 시킨다.",
"타겟의 바람 저항 약화",eIE_WEAKNING_TARGET_WIND_RESISTANCE		,
"타겟의 바람 저항을 [수치]0％ 약화 시킨다.",
"타겟의 대지 저항 약화",eIE_WEAKNING_TARGET_EARTH_RESISTANCE	,
"타겟의 대지 저항을 [수치]0％ 약화 시킨다.",
"타겟의 빛 저항 약화",eIE_WEAKNING_TARGET_LIGHT_RESISTANCE		,
"타겟의 빛 저항을 [수치]0％ 약화 시킨다.",
"타겟의 어둠 저항 약화",eIE_WEAKNING_TARGET_DARK_RESISTANCE		,
"타겟의 어둠 저항을 [수치]0％ 약화 시킨다.",
"타겟의 마법 저항 약화",eIE_WEAKNING_TARGET_MAGIC_RESISTANCE	,
"타겟의 마법 저항을 [수치]0％ 약화 시킨다.",
"불 속성 공격력 강화",eIE_STRENGTHENING_FIRE_DAMAGE				,
"불 속성 공격력을 [수치]0％ 강화 시킨다.",
"물 속성 공격력 강화",eIE_STRENGTHENING_WATER_DAMAGE			,
"물 속성 공격력을 [수치]0％ 강화 시킨다.",
"바람 속성 공격력 강화",eIE_STRENGTHENING_WIND_DAMAGE			,
"바람 속성 공격력을 [수치]0％ 강화 시킨다.",
"대지 속성 공격력 강화",eIE_STRENGTHENING_EARTH_DAMAGE			,
"대지 속성 공격력을 [수치]0％ 강화 시킨다.",
"빛 속성 공격력 강화",eIE_STRENGTHENING_LIGHT_DAMAGE			,
"빛 속성 공격력을 [수치]0％ 강화 시킨다.",
"어둠 속성 공격력 강화",eIE_STRENGTHENING_DARK_DAMAGE			,
"어둠 속성 공격력을 [수치]0％ 강화 시킨다.",
"마법 공격력 강화",eIE_STRENGTHENING_MAGIC_DAMAGE				,
"마법 속성 공격력을 [수치]0％ 강화 시킨다.",

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃길드 관련

"길드 생성"	,eIE_CREATE_GUILD							,	//	길드 생성
"길드를 생성한다."	,

"길드 레벨 업",eIE_GUILD_LEVEL_UP							,	//	길드 레벨을 올려준다.
"길드 레벨을 [수치]0으로 만든다.",

"길드 스킬 포인트 재분배",eIE_RESET_GUILD_SKILL_POINT	,	//	스탯 초기화
"길드 스킬 포인트를 재분배 한다.",

"길드 던전 입장 횟수 증가",	eIE_INCREASE_GUILD_DUNGEON_PLAY_COUNT,
"하루 길드 던전 입장 횟수 제한을 [수치]0회 늘린다.",			// 09.10.06

"길드 마크 확장 팩",	eIE_GS_EXPAND_PACK							,
"길드 마크 확장 팩",

"길드 마크 스페셜 팩",eIE_GS_SPECIAL_PACK							,
"길드 마크 스페셜 팩",

"길드 마크 럭셔리 팩",eIE_GS_LUXURY_PACK						,
"길드 마크 럭셔리 팩",

"길드 마크 한글 팩",eIE_GS_HANGUL_PACK							,
"길드 마크 한글 팩",

"길드 마크 영문 팩",eIE_GS_ENGLISH_PACK							,
"길드 마크 영문 팩",

"길드 마크 한문",eIE_GS_HANMUN_PACK							,
"길드 마크 한문 팩",

"길드 마크 아이템 팩",eIE_GS_ITEM_PACK							,
"길드 마크 아이템 팩",

"길드 마크 동물 팩",eIE_GS_ANIMAL_PACK							,
"길드 마크 동물 팩",

"길드 마크 시그널 팩",eIE_GS_SIGNAL_PACK							,
"길드 마크 시그널 팩",

"길드 마크 유니크1 팩",eIE_GS_UNIQUE1_PACK							,
"길드 마크 유니크1 팩",

"길드 마크 유니크2 팩",eIE_GS_UNIQUE2_PACK							,
"길드 마크 유니크2 팩",

"길드홀 포탈",eIE_GUILD_HALL_PORTAL,
"길드홀 포탈 아이템",

"길드 가디언 소환",eIE_SUMMON_GUILD_GUARDIAN,
"길드 가디언 소환",

"길드 석상 조각 추출"	,eIE_GUILD_STATUE_PIECE_EXPORTER,
"길드 석상 [수치]0번의 [수치]1 레벨 조각을 추출한다.",

"길드 석상 조각"		,eIE_GUILD_STATUE_PIECE,
"길드 석상 [수치]0번의 [수치]1 레벨 조각이다.",



"믿음직한 팻말",	eIE_PS_SIGNBOARD1								,
"믿음직한 모양의 팻말을 사용한다.",

"화려한 팻말",	eIE_PS_SIGNBOARD2								,
"화려한 모양의 팻말을 사용한다.",

"신비한 팻말",	eIE_PS_SIGNBOARD3								,
"신비한 느낌의 팻말을 사용한다.",

"깨끗한 팻말",	eIE_PS_SIGNBOARD4								,
"깨끗한 느낌의 팻말을 사용한다.",

"정열적인 팻말",	eIE_PS_SIGNBOARD5								,
"정열적인 느낌의 팻말을 사용한다.",

"확성기",	eIE_PS_LOUD_SPEAKER		,
"다른 채팅 풍선을 무시하고 자신의 팻말을 맨 위로 띄울 수 있다.",


"노점상 텍스트 컬러",eIE_PS_COLOR_TEXT							,
"노점상 텍스트를 컬러로 선택할 수 있게 해준다.",

"노점상 텍스트 두껍게",eIE_PS_BOLD_TEXT							,
"노점상 텍스트를 두껍게 할수 있게 해준다.",

"노점상 텍스트 내용 증가",eIE_PS_EXPAND_TEXT_BUFFER				,
"노점상 텍스트 내용을 많이 적을 수 있게 해준다.",

"노점상 팻말 주변 반짝이",eIE_PS_TWINKLE						,
"노점상 팻말 주변에 반짝이를 뿌려준다.",

"노점상 관련 기능 다섯가지",eIE_PS_PACK							,
"노점상 관련 기능 다섯가지 기능을 가능하게 해준다.",

"수평 배너 설치",	eIE_PUT_HORIZON_BANNER	,	//	가로 배너
"[수치]0번 모양의 수평 배너를 설치한다.",

"수직 배너 설치",	eIE_PUT_VERTICAL_BANNER		,	//	세로 배너
"[수치]0번 모양의 수직 배너를 설치한다",


//┃길드 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기타
"타임 스탬프"	,eIE_TIME_STAMP,	//	타임 스탬프
"시간을 기록한다.",

"칭호 추가"		,eIE_ADD_TITLE,	//	칭호 추가
"[수치]0번 칭호를 추가한다.",

"조합 배지"		,eIE_SET_BADGE,
"[수치]0번 세팅 배지",

"세팅 룬"		,eIE_SET_RUNE,
"[수치]0번 세팅 배지에 [수치]1번 룬",

"성명석"		,eIE_NAME_STONE,
"성명석",

"마법의 양탄자 소환",eIE_CALL_MAGIC_CARPET,
"마법의 양탄자를 소환한다.",

"가지고 있으면 양탄자 모양을 바꿔준다."	,	eIE_CHANGE_MAGIC_CARPET_SHAPE,
"양탄자 모양을 [수치]0번 타입으로 바꿔준다.",

"체력이 서서히 회복되는 마법의 양탄자 소환",eIE_REGEN_HP_ON_MAGIC_CARPET,
"체력이 서서히 회복되는 마법의 양탄자 소환",

"이동해도 CP가 감소하지 않는 마법의 양탄자 소환",eIE_NOT_DOWN_CP_ON_MAGIC_CARPET,
"이동해도 CP가 감소하지 않는 마법의 양탄자 소환",

"기존 양탄자보다 더 빠른 양탄자 소환",eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET,
"기존 양탄자보다 더 빠른 양탄자 소환",

"붉은색 드레이크 양탄자 소환",eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET_RED,
"붉은색 드레이크 양탄자 소환",

"기존 양탄자보다 느리지만 전투가 가능한 양탄자 소환",eIE_FIGHT_MAGIC_CARPET,
"기존 양탄자보다 느리지만 전투가 가능한 양탄자 소환",

"결투장 버프 : 불",	eIE_DUEL_BUFF_FIRE		,	//	결투 버프 불
"[수치]0초간 불 속성 결투장 버프를 걸어 준다.",

"결투장 버프 : 물",	eIE_DUEL_BUFF_WATER		,	//	결투 버프 불
"[수치]0초간 물 속성 결투장 버프를 걸어 준다.",

"결투장 버프 : 바람",	eIE_DUEL_BUFF_WIND		,	//	결투 버프 불
"[수치]0초간 바람 속성 결투장 버프를 걸어 준다.",

"결투장 버프 : 대지",	eIE_DUEL_BUFF_EARTH		,	//	결투 버프 불
"[수치]0초간 대지 속성 결투장 버프를 걸어 준다.",

"세트룬(미구현)",		eIE_SET_RUNE		,	//	결투 버프 불
"미구현",

"미니펫 각성 : 50%",	eIE_MINIPET_AWAKEN50		,	//	결투 버프 불
"미니펫 3차 각성 수식을 50% 적용한다.",

"미니펫 각성 : 100%",	eIE_MINIPET_AWAKEN100		,	//	결투 버프 불
"미니펫 3차 각성 수식을 100% 적용한다.",

"마법 주문서",			eIE_CASTING_MAGIC			,	//	마법 주문서
"INDEX[수치]0 보조 마법을 LEVEL[수치]1로 능력치로 사용한다.",

"비밀던전 입장 회수",	eIE_INCREASE_SECRET_DUNGEON_PLAY_COUNT,
"하루 비밀던전 입장 회수 제한을 [수치]0회 늘린다.",		// 09.10.06

"미니펫 경험치 획득률 증가",	eIE_INCREASE_GAIN_MINIPET_EXP,
"미니펫이 먹이를 먹어 획득하는 경험치를 [수치]0배로 증가 시킨다.",

"펫 이름 초기화",			eIE_CHANGE_PET_NAME,
"펫 이름을 초기화 시켜 이름을 변경 할 수 있게 해 준다.",

"인도자의 종소리",	eIE_SOUND_OF_LEADERS_BELL	,	
"[수치]0시간 동안 모든 유저의 경험치와 아이템 드랍률을 [수치]1배로 올려 준다.",

"사냥꾼 몬스터 변신",	eIE_TRANSFORMATION_HUNTER	,	
"[수치]0분 동안 레벨 [수치]1의 사냥꾼으로 변신한다.",

"좀도둑 몬스터 변신",	eIE_TRANSFORMATION_THIEF	,	
"[수치]0분 동안 레벨 [수치]1의 좀도둑으로 변신한다.",

"마법사 몬스터 변신",	eIE_TRANSFORMATION_MAGICIAN	,	
"[수치]0분 동안 레벨 [수치]1의 마법사로 변신한다.",

"몬스터 소환",	eIE_SUMMON_MONSTER	,	
"[수치]0 등급 이하의 몬스터를 소환한다.",

"각인 레벨",	eIE_CARVING_LEVEL	,	
"각인 레벨 [수치]0 이다.",

"각인 레벨 증가",	eIE_INCREASE_CARVING_LEVEL	,	
"각인 레벨을 [수치]0％확률로 1~[수치]1 만큼 증가시킨다.",

"각인 레벨 설정",	eIE_SET_CARVING_LEVEL	,	
"각인 레벨을 [수치]0 ％확률로 [수치]1로 설정 이다.",

"각인 옵션 실드",	eIE_ACL_SHIELD	,	
"[수치]0 ％확률로 받은 대미지 20 +([수치]0 * 1) %를 감소.",

"각인 옵션 충전",	eIE_ACL_CHARGE	,	
"매 8-([수치]0 * 0.1) 초마다  CP를 [수치]0 * 3 회복.",

"각인 옵션 경험치증가",	eIE_ACL_INCREASE_EXP	,	
"획득 경험치가 [수치]0％ 증가.",

"각인 옵션 불속성대미지 추가",	eIE_ACL_ADD_FIRE_DAMAGE	,	
"[수치]0％확률로 불 속성 대미지 100 + ([수치]0 * 200) 추가.",

"각인 옵션 모든 스텟 증가",	eIE_ACL_INCREASE_ALL_STATUS	,	
" 10 + [수치]0 * 2 모든 스텟 증가.",

"각인 옵션 소울가드",	eIE_ACL_SOUL_GUARD	,	
"3 + ([수치]0 * 0.3) 초 간 파티원 소울 가드 상태.",


"각인 옵션 퍼밀리어변신",	eIE_ACL_TRANSFORMATION_REPTILE	,	
"[수치]0 * 10 레벨의 퍼밀리어를 1 + [수치]0 분간 변신)",

"각인 옵션 최대 HP 증가",	eIE_ACL_INCREASE_MAX_HP	,	
"최대 체력 100 + [수치]0 * 50 증가",

"각인 옵션 공격력 증가",	eIE_ACL_INCREASE_ATTACK_POWER	,	
"5 + [수치]0 분간 공격력 10 + [수치]0 * 2 ％ 증가",

"각인 옵션 헬 프리즌",	eIE_ACL_HELL_PRISON	,	
"[수치]0 레벨의 헬 프리즌 사용",

//┃기타
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	"끝"																	,0xffff
};

cITEM_GENERATE_EFFECT_INDEX		cITEM_GENERATE_EFFECT::s_aItemEffectIndex[20][60]	=
{

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기본 수치
	{
		"기본 수치 / 미니펫",-1,
		"방어력",eIE_ADD_DEFENSE_POWER,
		"방패 블럭률",eIE_BLOCKING_RATE,
		"벨트 스택 아이템수",eIE_BELT_STACK_COUNT,
		"접두사 생성",eIE_FREE_RARE_OPTION,
		"아이템 조각",	eIE_ITEM_PIECE,
		"비밀 던전 열쇠",	eIE_SECRET_DUNGEON_KEY,
		
		"미니펫 소환",eIE_RECALL_MINIPET							,	//	미니펫 소환
		"미니펫 시간 연장",eIE_EXTEND_MINIPET_TIME						,	//	미니펫의 시간을 늘려줌
		"모든 미니펫 시간 연장",eIE_EXTEND_ALL_MINIPET_TIME					,	//	모든 미니펫의 시간을 늘려줌
		"미니펫 봉인",eIE_SEAL_MINIPET							,	//	미니펫 봉인
		"미니펫 이름 변경",eIE_CHANGE_MINIPET_NAME						,	//	미니펫 이름 변경
		"미니펫이 담긴 주머니",eIE_MINIPET_POUCH							,	//	미니펫이 담긴 주머니
		"미니펫 먹이[생체형]",eIE_FLAKE_OF_DOING_ONE						,
		"미니펫 먹이[에너지형]",eIE_FEATHER_OF_THINKING_ONE					,
		"미니펫 먹이[무기질형]",eIE_TOY_OF_SIN_AND_RIN						,
		"미니펫 양면 프레임",eIE_REVERSIBLE_FRAME	,
		"미니펫 경험치 획득률 증가",	eIE_INCREASE_GAIN_MINIPET_EXP,
		"미니펫 리플렉팅 믹서"	,eIE_REFLECTING_MIXER	,
		"미니펫 속박의 사슬"	,eIE_CHAIN_OF_RESTRAINT	,
		"end",0xffff,
	},
//┃기본 수치
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃일시적인 능력치 상승
	{
		"일시적인 능력치 상승",-1,
		"일시적으로 힘을 상승 시킨다.(초)",eIE_SHORT_WHILE_INCREASE_POWER,
		"일시적으로 힘을 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_POWER_BY_MINUTE,
		"일시적으로 민첩성을 상승 시킨다.(초)",eIE_SHORT_WHILE_INCREASE_AGILITY,
		"일시적으로 민첩성을 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_AGILITY_BY_MINUTE,
		"일시적으로 건강을 상승 시킨다.(초)",eIE_SHORT_WHILE_INCREASE_CONSTITUTION,
		"일시적으로 건강을 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_CONSTITUTION_BY_MINUTE,
		"일시적으로 지혜를 상승 시킨다.(초)",eIE_SHORT_WHILE_INCREASE_WISDOM,
		"일시적으로 지혜를 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_WISDOM_BY_MINUTE,
		"일시적으로 지식을 상승 시킨다.(초)",eIE_SHORT_WHILE_INCREASE_INTELIGENCE,
		"일시적으로 지식을 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_INTELIGENCE_BY_MINUTE,
		"일시적으로 카리스마를 상승 시킨다.(초)",eIE_SHORT_WHILE_INCREASE_CHARISMA,
		"일시적으로 카리스마를 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_CHARISMA_BY_MINUTE,
		"일시적으로 행운을 상승 시킨다.(초)",eIE_SHORT_WHILE_INCREASE_LUCK,
		"일시적으로 행운을 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_LUCK_BY_MINUTE,
		"일시적으로 행운을(x100) 상승 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_LUCKx100_BY_MINUTE,

		"일시적으로 최종 대미지를 증가 시킨다.",eIE_INCREASE_FINAL_DAMAGE,
		"일시적으로 공격력을 증가 시킨다.(초)",eIE_SHORT_WHILE_INCREASE_ATTACK_POWER,
		"일시적으로 공격력을 증가 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE,
		"일시적으로 공격력을 증가 시킨다.(분)(10％단위)",eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE_10,
		"일시적으로 방어력을 증가 시킨다.(초)",eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER,
		"일시적으로 방어력을 증가 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE,
		"일시적으로 방어력을 증가 시킨다.(분)(10％단위)",eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE_10,
		"일시적으로 체력을 증가 시킨다.(초)",eIE_SHORT_WHILE_INCREASE_HP,
		"일시적으로 체력을 증가 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_HP_BY_MINUTE,
		"일시적으로 CP를 증가 시킨다.(초)",eIE_SHORT_WHILE_INCREASE_CP,
		"일시적으로 CP를 증가 시킨다.(분)",eIE_SHORT_WHILE_INCREASE_CP_BY_MINUTE,
		"일시적으로 무기 대미지 맥스로 유지(초)",eIE_SHORT_WHILE_SET_MAX_DAMAGE,
		"일시적으로 무기 대미지 맥스로 유지(분)",eIE_SHORT_WHILE_SET_MAX_DAMAGE_BY_MINUTE,
		"일시적으로 CP 맥스로 유지(초)",eIE_SHORT_WHILE_SET_MAX_CP,
		"일시적으로 CP 맥스로 유지(분)",eIE_SHORT_WHILE_SET_MAX_CP_BY_MINUTE,
		"end",0xffff,
	},
//┃일시적인 능력치 상승
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기타 소모성 아이템
	{
		"기타 소모성 아이템",-1,
		"체력 회복(수치)",eIE_RECOVER_HP,
		"체력 회복(수치x100)",eIE_RECOVER_HP2,
		"체력 즉시 회복(수치x100)",eIE_INSTANCE_RECOVER_HPx100			,	//	즉석 체력 회복 옵션 수치 * 100
		"체력 회복(퍼센티지)",eIE_RECOVER_HP_PERCENTAGE,
		"체력 즉시 회복(퍼센티지)",eIE_INSTANCE_RECOVER_HP_PERCENTAGE		,	//	즉석 체력 회복
		"남은 체력 즉시 감소(퍼센티지)",eIE_DECREASE_INSTANT_HP_PERCENTAGE		,	//	남은 체력 즉시감소(퍼센티지)
		"CP 충전(수치)",eIE_RECOVER_CP,
		"CP 충전2(수치)",eIE_RECOVER_CP2,
		"CP 충전(퍼센티지)",eIE_RECOVER_CP_PERCENTAGE,
		"HP/CP 회복(수치)",eIE_RECOVER_HP_CP,
		"HP/CP 회복(퍼센티지)",eIE_RECOVER_HP_CP_PERCENTAGE,
		"타인 부활"	,eIE_RESURRECTION,
		"본인 부활",eIE_SELF_RESURRECTION,
		"파티 완전 부활",eIE_PERFECT_PARTY_REVIVE,
		"중화",eIE_HEAL_STRANGE_STATUS,
		"이상계 상태 치료",eIE_RECOVER_ALL_STRANGE_STATUS,
		"저하계 상태 치료",eIE_RECOVER_ALL_DECLINE_STATUS,
		"저주계 상태 치료",eIE_RECOVER_ALL_CURSE_STATUS,
		"모든 상태 이상 치료",eIE_RECOVER_ALL_BADSTATUS,
		"중독 치료",eIE_HEAL_POSION,
		"보물 상자 열기",eIE_OPEN_ARCA,
		"문 열기",eIE_OPEN_DOOR,
		"문과 보물 상자 열기",eIE_OPEN_ARCA_AND_DOOR,
		"편도용 포탈",eIE_ONE_WAY_PORTAL,
		"왕복용 포탈",eIE_PORTAL,
		"보석 사용",eIE_USE_JEWEL,
		"메모리 스톤",eIE_MEMORY_STONE,
		"텔레포트 스톤",eIE_TELEPORT_STONE,
		"포탈 스톤",eIE_PORTAL_STONE,
		"OX 퀴즈 티켓",eIE_OX_QUIZ_TICKET,	//	OX 퀴즈 티켓이다.
		"마력 충전",eIE_CHARGE_EXTRA_ITEM_POWER	,	//	마력 충전
		"책",eIE_BOOK							,
//		"책에 카테고리 추가",eIE_ADD_CATEGORY							,
		"책에 페이지 추가",eIE_ADD_PAGE		,
		"최종 대미지 증가",	eIE_INC_LAST_DAMAGE							,	//	최종 대미지 중가
		"사냥 경험치 증가",	eIE_INC_HUNT_EXP								,	//	사냥 경험치 증가
		"아이템 드랍 확률 증가"	,	eIE_INC_HUNT_ITEM_DROP_CHANCE								,	//	사냥 경험치 증가
		"죽음 페널티 회복"		,	eIE_CLEAR_DEATH_PENELTY						,	//	죽음 페널티 회복
		"이동 속도 증가"		,	eIE_INCREASE_MOVE_SPEED		,
		"이벤트 풍선(경험치증가)"			,	eIE_EVENT_BALLOON			,	//	이벤트 풍선
		"이벤트 풍선(아이템드랍률)"			,	eIE_EVENT_BALLOON_INC_ITEM_DROP_CHANCE			,	//	이벤트 풍선 아이템드랍률
		"미니펫 먹이"			,	eIE_MINIPET_FEED			,	
		"미니펫 스킬 초기화"			,	eIE_RESET_MINIPET_SKILL			,	
		"미니펫 타입 변경"			,	eIE_SET_MINIPET_TYPE			,	
		"체력 수치 변경"			,eIE_CHANGE_HP,
		"특정 필드 이동"			,eIE_MOVE_FIELD,
		"end",0xffff,
	},
//┃기타 소모성 아이템
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃공격 부가 효과/부가 대미지
	{
		"공격 부가 효과/부가 대미지",-1,
		"즉사",eIE_INSTANCE_KILL,
		"대상을 버서크 상태로 만듦",eIE_MAKE_BERSERKER,
		"갑옷 파괴",eIE_MAKE_ARMOR_BREAK,
		"무기 파괴",eIE_MAKE_WEAPON_BREAK,
		"중독시킨다.",eIE_MAKE_POISONED,
		"혼란시킨다.",eIE_MAKE_CONFUSE,
		"유혹",eIE_MAKE_CHARMING,
		"유혹(30초)",eIE_MAKE_CHARMING_30,
		"스턴",eIE_MAKE_STUN,
		"콜드",eIE_MAKE_COLD,
		"콜드(100％)",eIE_MAKE_COLD_100,
		"불속성 대미지",eIE_HIT_FIRE_DAMAGE,
		"물속성 대미지",eIE_HIT_WATER_DAMAGE,
		"바람속성 대미지",eIE_HIT_WIND_DAMAGE,
//		"대지속성 대미지",eIE_HIT_TERRA_DAMAGE,
		"빛속성 대미지",eIE_HIT_LIGHT_DAMAGE,
		"어둠속성 대미지",eIE_HIT_DARK_DAMAGE,
		"언데드 사냥꾼",eIE_ADD_DAMAGE_TO_UNDEAD,
		"악마 사냥꾼",eIE_ADD_DAMAGE_TO_DEMON,
		"인간 사냥꾼",eIE_ADD_DAMAGE_TO_HUMAN,
		"동물 사냥꾼",eIE_ADD_DAMAGE_TO_ANIMAL,
		"신수 사냥꾼",eIE_ADD_DAMAGE_TO_HOLY_ANIMAL,
		"end",0xffff,
	},
//┃공격 부가 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 부가 효과
	{
		"각종 부가 효과",-1,
		"벨트에 아이템 채우는 시간 감소",eIE_DECREASE_RELOAD_TIME,
		"변신 속도 빨라짐",eIE_INCREASE_TRANSFORMATION_SPEED,
		"물약에 의한 회복 속도 증가",eIE_INCREASE_RECOVER_SPEED,
		"물리 대미지 흡수",	eIE_AB_PH_DAMAGE	,	
		"가시돋힘",eIE_RETURN_DAMAGE,
		"가시돋힘 40",eIE_RETURN_DAMAGE_40,
		"무한 탄환",eIE_INFINITY_BULLET,
		"체력회복 속도 증가",eIE_INCREASE_SITDOWN_RECOVER_HP_SPEED,
		"사냥 경험치 증가",	eIE_HUNTING_EXP_BONUS,	//	앉아 있을때 체력 회복 속도를 높혀준다.

		"end",0xffff,
	},
//┃각종 부가 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 수치 증가
	{
		"각종 수치 증가",-1,
		"밀리 직업들 공격력 상승",eIE_INCREASE_MELEE_JOB_ATTACK_POWER,
		"선택한 능력치 상승",eIE_INCREASE_SELECT_POWER,
		"힘 상승",eIE_INCREASE_POWER,
		"지혜 상승",eIE_INCREASE_WISDON,
		"지식 상승",eIE_INCREASE_INTELIGENCE,
		"건강 상승",eIE_INCREASE_CONSTITUTION,
		"민첩성 상승",eIE_INCREASE_AGILITY,
		"카리스마 상승",eIE_INCREASE_CHARISMA,
		"행운 상승",eIE_INCREASE_LUCK,
		"크리티컬 발생확률",eIE_INCREASE_CRITICAL,
		"결정타 발생확률",eIE_INCREASE_CRUSH_SHOT,
		"명중률 증가",eIE_INCREASE_HIT_RATE,
		"회피율 증가",eIE_INCREASE_DODGE_CHANCE,
		"집중력 증가",eIE_INCREASE_CONCENTRATION,
		"공격 속도 증가",eIE_INCREASE_ATTACK_SPEED,
		"이동 속도 증가",eIE_EQUIP_INCREASE_MOVE_SPEED,
		"모든 능력치 증가",eIE_INCREASE_ALL_STATUS,
		"선택한 능력치 상승(고정)",eIE_INCREASE_FIX_SELECT_POWER,
		"최대 체력 증가(x 100)",eIE_ADD_MAX_HPx100,
		"최대 CP 증가(x 100)",eIE_ADD_MAX_CPx100,
		"방어력 ％ 증가",eIE_ADD_DEFENSE_PERCENTAGE,
		"최대 체력 ％ 증가",eIE_ADD_MAX_HP_PERCENTAGE,
		"최대 CP ％ 증가",eIE_ADD_MAX_CP_PERCENTAGE,
		"붉은 보석 획득 가능 레벨 증가",eIE_INCREASE_LEVEL_GETING_RED_STONE,
		"end",0xffff,
	},
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 저항
	{
		"각종 저항",-1,
		"저주 상태에 대한 저항",eIE_RESISTANCE_CURSE_STATUS,
		"저하 상태에 대한 저항",eIE_RESISTANCE_DECLINE_STATUS,
		"이상 상태에 대한 저항",eIE_RESISTANCE_STRANGE_STATUS,
		"모든 상태 이상에 대한 저항",eIE_RESISTANCE_ALL_BAD_STATUS,
		"불저항",eIE_INCREASE_FIRE_RESITANCE,
		"대지저항",eIE_INCREASE_TERRA_RESITANCE,
		"바람저항",eIE_INCREASE_WIND_RESITANCE,
		"물저항",eIE_INCREASE_WATER_RESITANCE,
		"빛저항",eIE_INCREASE_LIGHT_RESITANCE,
		"어둠저항",eIE_INCREASE_DARK_RESITANCE,
		"불,물,바람,대지에 대한 저항",eIE_INCREASE_FOUR_ELEMENTAL_RESITANCE,
		"마법 속성 공격에 대한 저항",eIE_INCREASE_MAGIC_RESITANCE,
		"녹백 저항",eIE_INCREASE_NOCK_BACK_RESISTANCE,
		"치명타 저항",eIE_INCREASE_CRITICAL_RESISTANCE,
		"결정타 저항",eIE_INCREASE_CRUSH_SHOT_RESISTANCE,
		"즉사 저항",eIE_INCREASE_INSTANCE_KILL_RESISTANCE,
		"end",0xffff,
	},
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃스킬레벨
	{
		"스킬레벨",-1,
		"마법 창병 스킬 레벨 증가",eIE_INCREASE_MAGIC_LANCER_SKILL,
		"마법 궁수 스킬 레벨 증가",eIE_INCREASE_MAGIC_ARCHER_SKILL,
		"견습 기사 스킬 레벨 증가",eIE_INCREASE_KNIGHT_SKILL,
		"전사 계열 스킬 레벨 증가",eIE_INCREASE_WARRIOR_SKILL,
		"마법사 스킬 레벨 증가",eIE_INCREASE_WIZARD_SKILL,
		"늑대인간 스킬 레벨 증가",eIE_INCREASE_WEREWOLF_SKILL,
		"도둑 스킬 레벨 증가",eIE_INCREASE_THIEF_SKILL,
		"무도가 스킬 레벨 증가",eIE_INCREASE_FIGHTER_SKILL,
		"조련사 스킬 레벨 증가",eIE_INCREASE_BEAST_TAMER_SKILL,
		"소환사 스킬 레벨 증가",eIE_INCREASE_SUMMONER_SKILL,
		"공주 스킬 레벨 증가",eIE_INCREASE_PRINCESSE_SKILL,
		"변신 소녀 스킬 레벨 증가",eIE_INCREASE_MAGICAL_GIRL_SKILL,
		"사제 스킬 레벨 증가",eIE_INCREASE_PRIEST_SKILL,
		"타락천사 스킬 레벨 증가",eIE_INCREASE_FALLEN_ANGEL_SKILL,
		"강신술사 스킬 레벨 증가",eIE_INCREASE_NECROMANCER_SKILL,
		"악마 스킬 레벨 증가",eIE_INCREASE_DEMON_SKILL,
		"영술사 스킬 레벨 증가",eIE_INCREASE_SOUL_BRINGER_SKILL,
		"여성 캐릭터들의 스킬 레벨 증가",eIE_INCREASE_FEMALE_SKILL_LEVEL,
		"남성 캐릭터들의 스킬 레벨 증가",eIE_INCREASE_MALE_SKILL_LEVEL,
		"모든 스킬 레벨 증가",eIE_INCREASE_SKILL_LEVEL,
		"end",0xffff,
	},
//┃스킬레벨
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃신비한 마법 아이템
	{
		"신비한 마법 아이템",-1,
		"월드맵을 이용해 필드 이동(쿨 타임 있음)",	eIE_JUMP_FIELD_USE_WORLD_MAP,
		"월드맵을 이용한 텔레포트(쿨 타임 없음)",	eIE_USE_WORLD_MAP_TELEPORT	,	//	
		"비밀던전 입장 회수",	eIE_INCREASE_SECRET_DUNGEON_PLAY_COUNT,  //09.10.06
		"레벨 상승",eIE_INCREASE_LEVEL,
		"04년 설날 이벤트",eIE_EVENT_2004_01_21,
		"아이템 상자"	,	eIE_LOTTERY_ITEM,	//	길드 생성
		"선택 가능한 아이템 상자"	,	eIE_SELECT_ITEM_IN_ITEM_PACK,	//	길드 생성
		"힘 증가",eIE_GROW_STRENGTH,
		"민첩성 증가",eIE_GROW_AGILITY,
		"건강 증가",eIE_GROW_CONSTITUTION,
		"지혜 증가",eIE_GROW_WISDOM,
		"지식 증가",eIE_GROW_INTELLIGENCE,
		"카리스마 증가",eIE_GROW_CHARISMA,
		"행운 증가",eIE_GROW_LUCK,
		"체력 증가",eIE_GROW_HP,
		"CP 증가",eIE_GROW_CP,
		"무기 대미지 증가",eIE_INCREASE_WEAPON_DAMAGE,
		"갑옷 방어력 증가",eIE_INCREASE_ARMOR_DEFENSE,
		"마법 보석",eIE_MAGIC_JEWEL,
//		"프론티어 증서",eIE_FRONTIER_SCROLL					,
		"노말 아이템 복권",eIE_NORMAL_ITEM_LOTTERY_TICKET,
		"레어 아이템 복권",eIE_RARE_ITEM_LOTTERY_TICKET		,
		"유니크 아이템 복권",eIE_UNIQUE_ITEM_LOTTERY_TICKET	,
		"슈퍼 아이템 복권",eIE_SUPER_ITEM_LOTTERY_TICKET	,
		"스킬 포인트 재분배",eIE_RESET_SKILL_POINT	,//	스킬 포인트 리셋
		"스탯 포인트 재분배",eIE_RESET_STATE_POINT	,//	스탯 초기화
		"힘 스탯을 재분배",eIE_RESET_STRENGTH_STATE		,	//	힘 스탯 하나 재 분배
		"민첩 스탯을 재분배",eIE_RESET_AGILITY_STATE			,	//	민첩 스탯 하나 재 분배
		"건강 스탯을 재분배",eIE_RESET_CONSTITUTION_STATE	,	//	건강 스탯 하나 재 분배
		"지식 스탯을 재분배",eIE_RESET_INTELLIGENCE_STATE	,	//	지식 스탯 하나 재 분배
		"지혜 스탯을 재분배",eIE_RESET_WISDOM_STATE			,	//	지혜 스탯 하나 재 분배
		"카리스마 스탯을 재분배",eIE_RESET_CHARISMA_STATE		,	//	카리스마 스탯 하나 재 분배
		"운 스탯을 재분배 한다.",eIE_RESET_LUCK_STATE			,	//	운 스탯 하나 재 분배
//		"접두사 변경",eIE_CHANGE_ITEM_PREFIX_2003_11_01,//	스탯 초기화
//		"위험한 접두사 변경",eIE_CHANGE_ITEM_PREFIX_2003_11_17,
//		"산타의 오브",eIE_ORB_OF_SANTA,
		"장비 착용레벨 조절",eIE_OVER_POWER_ITEM_EQUIP,
		"완전 부활",eIE_PERFECT_RESSURECTION,

		"여행자용 가방"	,	eIE_EXPAND_INVENTORY_BAG,

		"사육 기록",	eIE_BREEDING_RECORD		,
		"보물 지도",	eIE_TREASURE_MAP		,	//	보물 지도
		"커플 링 세트",eIE_COUPLE_RING_SET	,
		"커플 링",	eIE_COUPLE_RING	,
		"증정용 커플 링",eIE_PRESENT_COUPLE_RING	,
		"상대 없는 커플 링",eIE_NOT_DEFINE_COUPLE_COUPLE_RING	,
		"바닥 없는 상자",eIE_MIX_ITEM_BOX	,	//	바닥 없는 상자
		"불꽃 놀이 세트",eIE_FIRE_WORK,
		"무작위 불꽃 놀이",	eIE_RANDOM_FIRE_WORK,
		"공주가 변신한 무기",eIE_PRINCESS_WEAPON,
		"운영자로 변신",eIE_TRANS_TO_OPERATOR,
		"펫 이름 초기화",			eIE_CHANGE_PET_NAME,
		"새벽의 봉인구"	,eIE_SEAL_SPHERE_OF_DAWN,	//	새벽의 봉인구 

		"end",0xffff,
	},
//┃신비한 마법 아이템
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃인챈트
	{
		"인챈트 & 아이템 수리",-1,
		"짐모리의 인챈트 서비스",eIE_JIM_MORYS_ENCHANT			,	//	짐 모리의 인챈트
		"서투른 아이템 수리",eIE_UNSKILLED_REPAIR_BROKEN_ITEM	,
		"접두사를 붙이거나 기존의 접두사를 업그레이드 한다."	,	eIE_ENCHANT_ITEM_BY_UNIQUE_INFO	,	//	유니크 아이템에 의한 아이템 인챈트
		"완벽한 아이템 수리",eIE_REPAIR_BROKEN_ITEM				,	//	"부숴진 아이템 수리",
		"장비 해체 불가 저주 제거",eIE_CURE_ITEM_EQUIP_CURSE	,	//	"장비에 붙어 있는 저주를 풀어준다.
		"인챈트 불가 저주 제거",eIE_RELEASE_ATTACH_PREFIX_CURSE	,	//	"장비에 붙어 있는 저주를 풀어준다.
		"DX 아이템 수리",	eIE_REPAIR_DX_ITEM_KIT	,
		"접두사 제거",	eIE_REMOVE_PREFIX,

		"접두사 붙이기",	eIE_ATTACH_PREFIX,
		"접두사 복사하기",	eIE_COPY_PREFIX,
		"접두사 잘라내기",	eIE_CUT_PREFIX,
		"접두사 붙이기(거래불가)"	,	eIE_ATTACH_PREFIX_CAN_NOT_TRADE,
		"접두사 복사하기(거래불가)"	,	eIE_COPY_PREFIX_CAN_NOT_TRADE,
		"접두사 잘라내기(거래불가)"	,	eIE_CUT_PREFIX_CAN_NOT_TRADE,

		"end",0xffff,
	},
//┃인챈트
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃길드 마크
	{
		"길드 & 노점상",-1,
		"길드 생성",eIE_CREATE_GUILD		,	//	길드 생성
		"길드 레벨 업",eIE_GUILD_LEVEL_UP	,	//	길드 레벨을 올려준다.
		"길드 스킬 포인트 재분배",eIE_RESET_GUILD_SKILL_POINT	,	//	스탯 초기화
		"길드 던전 입장 횟수 증가",	eIE_INCREASE_GUILD_DUNGEON_PLAY_COUNT,		// 09.10.06

		"길드 홀 포탈 아이템"	,eIE_GUILD_HALL_PORTAL	,
		"길드 가디언 소환",eIE_SUMMON_GUILD_GUARDIAN,
		"길드 석상 조각 추출"	,eIE_GUILD_STATUE_PIECE_EXPORTER,
		"길드 석상 조각"		,eIE_GUILD_STATUE_PIECE,

		"길드 마크 확장 팩"		,eIE_GS_EXPAND_PACK						,
		"길드 마크 스페셜 팩",eIE_GS_SPECIAL_PACK						,
		"길드 마크 럭셔리 팩",eIE_GS_LUXURY_PACK						,
		"길드 마크 한글 팩",eIE_GS_HANGUL_PACK							,
		"길드 마크 영문 팩",eIE_GS_ENGLISH_PACK							,
		"길드 마크 한문",eIE_GS_HANMUN_PACK								,
		"길드 마크 아이템 팩",eIE_GS_ITEM_PACK							,
		"길드 마크 동물 팩",eIE_GS_ANIMAL_PACK							,


		"노점상 팻말 1"	,	eIE_PS_SIGNBOARD1							,
		"노점상 팻말 2"	,	eIE_PS_SIGNBOARD2							,
		"노점상 팻말 3"	,	eIE_PS_SIGNBOARD3							,
		"노점상 팻말 4"	,	eIE_PS_SIGNBOARD4							,
		"노점상 팻말 5"	,	eIE_PS_SIGNBOARD5							,
		"확성기"		,	eIE_PS_LOUD_SPEAKER							,

		"노점상 텍스트 컬러",eIE_PS_COLOR_TEXT							,
		"노점상 텍스트 두껍게",eIE_PS_BOLD_TEXT							,
		"노점상 텍스트 내용 증가",eIE_PS_EXPAND_TEXT_BUFFER				,
		"노점상 팻말 주변 반짝이",eIE_PS_TWINKLE						,
		"노점상 관련 기능 다섯가지",eIE_PS_PACK							,
		"수직 배너 설치",	eIE_PUT_VERTICAL_BANNER		,	//	가로 배너
		"수평 배너 설치",	eIE_PUT_HORIZON_BANNER		,	//	세로 배너

		"end",0xffff,
	},
//┃길드 마크
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃마법 대미지 관련
	{
		"원소 대미지 보정",-1,
		"타겟의 불 저항 약화",eIE_WEAKNING_TARGET_FIRE_RESISTANCE		,
		"타겟의 물 저항 약화",eIE_WEAKNING_TARGET_WATER_RESISTANCE		,
		"타겟의 바람 저항 약화",eIE_WEAKNING_TARGET_WIND_RESISTANCE		,
		"타겟의 대지 저항 약화",eIE_WEAKNING_TARGET_EARTH_RESISTANCE	,
		"타겟의 빛 저항 약화",eIE_WEAKNING_TARGET_LIGHT_RESISTANCE		,
		"타겟의 어둠 저항 약화",eIE_WEAKNING_TARGET_DARK_RESISTANCE		,
		"타겟의 마법 저항 약화",eIE_WEAKNING_TARGET_MAGIC_RESISTANCE		,
		"불 속성 공격력 강화",eIE_STRENGTHENING_FIRE_DAMAGE				,
		"물 속성 공격력 강화",eIE_STRENGTHENING_WATER_DAMAGE				,
		"바람 속성 공격력 강화",eIE_STRENGTHENING_WIND_DAMAGE				,
		"대지 속성 공격력 강화",eIE_STRENGTHENING_EARTH_DAMAGE				,
		"빛 속성 공격력 강화",eIE_STRENGTHENING_LIGHT_DAMAGE				,
		"어둠 속성 공격력 강화",eIE_STRENGTHENING_DARK_DAMAGE				,
		"마법 공격력 강화",eIE_STRENGTHENING_MAGIC_DAMAGE				,
		"end",0xffff,
	},
//┃마법 대미지 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃챕터 5 보상 관련
	{
		"챕터 5 보상 관련",-1,
		"챕터 5 : 칭호 레벨 증가"	,eIE_CH5_SELECT_UPGRADE_TITLE,
		"챕터 5 : 거래 가능",eIE_CH5_RELASE_REVERSION,
		"챕터 5 : 길드 스킬 리셋",eIE_CH5_RESET_GUILD_SKILL_POINT,
		"챕터 5 : 체력 증가 부적",eIE_CH5_BONUS_HP_AMULET,
		"챕터 5 : CP 증가 부적",eIE_CH5_BONUS_CP_AMULET,
		"챕터 5 : 착용 제한 레벨 증가 부적",eIE_CH5_OVER_POWER_ITEM_EQUIP,
		"챕터 5 : 스킬 리버스",eIE_CH5_REVERSE_SKILL_POINT,
		"챕터 5 : 스탯 리버스",eIE_CH5_REVERSE_STATE_POINT,
		"챕터 5 : 접두사 제거",eIE_CH5_REMOVE_PREFIX,
		"챕터 5 : 아이템 복사",eIE_CH5_COPY_ITEM,
		"챕터 5 : 일반 아이템 접두사 업그레이드",eIE_CH5_UPGRADE_NORMAL_ITEM_PREFIX_POWER,
		"챕터 5 : 일반 아이템 성능 향상",eIE_CH5_IMPROVE_NORMAL_ITEM_POWER,
		"챕터 5 : 유니크 아이템 접두사 업그레이드",eIE_CH5_UPGRADE_UNIQUE_ITEM_PREFIX_POWER,
		"챕터 5 : 유니크 아이템 성능 향상",eIE_CH5_IMPROVE_UNIQUE_ITEM_POWER,
		"챕터 5 : 붉은 바닥 없는 상자",eIE_CH5_MIX_ITEM_BOX,
		"챕터 5 : 성향 수치 선택",eIE_CH5_SELECT_ALLIGNMENT,
		"end",0xffff,
	},
//┃챕터 5 보상 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기타
	{
		"기타",-1,
		"타임 스탬프"	,eIE_TIME_STAMP,
		"칭호 추가"		,eIE_ADD_TITLE,	//	칭호 추가
		"세팅 배지"		,eIE_SET_BADGE,
		"세팅 룬"		,eIE_SET_RUNE,
		"성명석"		,eIE_NAME_STONE,
		"마법의 양탄자 모양이 바뀜",eIE_CHANGE_MAGIC_CARPET_SHAPE				,	//	마법의 양탄자 모양을 바꾼다.
		"마법의 양탄자 소환",eIE_CALL_MAGIC_CARPET,
		"체력이 서서히 회복되는 마법의 양탄자 소환",eIE_REGEN_HP_ON_MAGIC_CARPET,
		"이동해도 CP가 감소하지 않는 마법의 양탄자 소환",eIE_NOT_DOWN_CP_ON_MAGIC_CARPET,
		"기존 양탄자보다 더 빠른 양탄자 소환",eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET,
		"붉은색 드레이크 양탄자 소환",eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET_RED,
		"기존 양탄자보다 느리지만 전투가 가능한 양탄자 소환",eIE_FIGHT_MAGIC_CARPET,
		"결투장 버프 : 불",	eIE_DUEL_BUFF_FIRE		,	//	결투 버프 불
		"결투장 버프 : 물",	eIE_DUEL_BUFF_WATER		,	//	결투 버프 불
		"결투장 버프 : 바람",	eIE_DUEL_BUFF_WIND		,	//	결투 버프 불
		"결투장 버프 : 대지",	eIE_DUEL_BUFF_EARTH		,	//	결투 버프 불
		"미니펫 각성 : 50%"	,	eIE_MINIPET_AWAKEN50	,	//	미니펫 각성 50%
		"미니펫 각성 : 100%"	,	eIE_MINIPET_AWAKEN100	,	//	미니펫 각성 100%

		"길드 마크 시그널 팩",eIE_GS_SIGNAL_PACK							,
		"길드 마크 유니크1 팩",eIE_GS_UNIQUE1_PACK							,
		"길드 마크 유니크2 팩",eIE_GS_UNIQUE2_PACK							,

		"마법 주문서",eIE_CASTING_MAGIC,
		"인도자의 종소리",	eIE_SOUND_OF_LEADERS_BELL	,

		"사냥꾼 몬스터 변신",	eIE_TRANSFORMATION_HUNTER,
		"좀도둑 몬스터 변신",	eIE_TRANSFORMATION_THIEF,
		"마법사 몬스터 변신",	eIE_TRANSFORMATION_MAGICIAN,
		"몬스터 소환",	eIE_SUMMON_MONSTER,
		"각인 레벨",	eIE_CARVING_LEVEL,
		"각인 레벨 증가",	eIE_INCREASE_CARVING_LEVEL,
		"각인 레벨 설정",	eIE_SET_CARVING_LEVEL,
		"각인 옵션 실드",	eIE_ACL_SHIELD	,
		"각인 옵션 충전",	eIE_ACL_CHARGE	,
		"각인 옵션 경험치증가",	eIE_ACL_INCREASE_EXP	,	
		"각인 옵션 불속성대미지 추가",	eIE_ACL_ADD_FIRE_DAMAGE	,
		"각인 옵션 모든 스텟 증가",	eIE_ACL_INCREASE_ALL_STATUS	,
		"각인 옵션 소울 가드",	eIE_ACL_SOUL_GUARD	,
		"각인 옵션 퍼밀리어 변신",	eIE_ACL_TRANSFORMATION_REPTILE	,
		"각인 옵션 최대 HP 증가",	eIE_ACL_INCREASE_MAX_HP	,
		"각인 옵션 공격력 증가",	eIE_ACL_INCREASE_ATTACK_POWER	,
		"각인 옵션 헬 프리즌",	eIE_ACL_HELL_PRISON	,
		"end",0xffff,
	},
//┃기타
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	{
		"끝",0xffff,
	}
};

//
//	아이템 효과들의 인덱스를 얻어 온다.(시리얼 참조)
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
//	기타등등이  출력 되는 콤보 박스
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
//	_iEffect가 속해 있는 카테고리를 얻어 온다.
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
//	아이템 효과를 얻어 온다.(시리얼 참조)
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
//	효과를 추가 시킨다.
BOOL
cITEM_GENERATE_EFFECT::AddEffect(HWND hwnd,cBASIC_ITEM *_lpItem,cITEM_GENERATE_EFFECT *lpEffect)
{
	s_aValue[0][0]	=	_lpItem->m_aValue[0][0];
	s_aValue[0][1]	=	_lpItem->m_aValue[0][1];
	s_aValue[1][0]	=	_lpItem->m_aValue[1][0];
	s_aValue[1][1]	=	_lpItem->m_aValue[1][1];

	strcpy(s_strComboStr[0][0],_ms("%d 에서 %d",s_aValue[0][0],s_aValue[0][1]));
	strcpy(s_strComboStr[0][1],_ms("%d 에서 %d",s_aValue[1][0],s_aValue[1][1]));

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
//	아이템 효과 설정
//////////////////////////////////////////////////////////////////////////////////////////////

//
//	아이템 효과 대화박스 초기화
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

//	트리 내용 채우기
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
//	종성을 체크 한다
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
//	콤보 박스들..
//////////////////////////////////////////////////////////////////////////////////////

//
//	은는이가을를 을 선택
void
cITEM_GENERATE_EFFECT::TransCode(char *lpStream)
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
}


//
//	아이템 효과를 읽는다.
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
//	아이템 부가 효과를텍스트를 그린다.
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
			break;		//	다 읽어 들였다.

		lpStream	=	s_Reader.GetStream('[');

		
		if (iValueIndex	>=0	)
			TransCode(lpStream);

		//	스태틱 문장 출력
		if	(lpStream	)
			x	+=	s_aStream[s_iStreamIndex++].Init(hwnd,MAIN.hINST,lpStream,x,y,0,12);
		if	(s_Reader.m_isDone	)
			break;		//	다 읽어 들였다.

		//	콤보 박스나 머 그런거..
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
//	아이템 부가 효과 설명
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
		strcpy(s_strComboStr[0][0],_ms("%d 에서 %d",s_aValue[0][0],s_aValue[0][1]));
	else
		strcpy(s_strComboStr[0][0],_ms("%d",s_aValue[0][0]));
		
	if (_iValue2	!=	-1)
		strcpy(s_strComboStr[0][1],_ms("%d",_iValue2));
	else
	if (s_aValue[1][0]	!=	s_aValue[1][1])
		strcpy(s_strComboStr[0][1],_ms("%d 에서 %d",s_aValue[1][0],s_aValue[1][1]));
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
