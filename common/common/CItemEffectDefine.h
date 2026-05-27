#ifndef _classITEM_EFFECT_DEFINE_
#define _classITEM_EFFECT_DEFINE_

#include "SFC.H"




#define	dACL_OPTION_SHIELD_FORMULA1(x)					(1 + ( (x) * 0.5 ))
#define	dACL_OPTION_SHIELD_FORMULA2(x)					(20 + (x))
#define	dACL_OPTION_CHARGE_FORMULA1(x)					(8-((x) * 0.1))
#define	dACL_OPTION_CHARGE_FORMULA2(x)					((x) * 3)
#define	dACL_OPTION_INCREASE_EXP_FORMULA(x)				(5 + (x))
#define	dACL_OPTION_ADD_FIRE_DAMAGE(x)					(100 + ((x) * 200))
#define	dACL_OPTION_INCREASE_ALL_STATUS(x)				(10 + (x) * 2)
#define	dACL_OPTION_SOUL_GUARD(x)						(3 + ((x) * 0.3))
#define	dACL_OPTION_TRANSFORMAION_REPTILE_FORMULA1(x)	((x) * 10)
#define	dACL_OPTION_TRANSFORMAION_REPTILE_FORMULA2(x)	(1 + (x))
#define	dACL_OPTION_INCREASE_ATTACK_POWER_FORMULA1(x)	(5 + (x))
#define	dACL_OPTION_INCREASE_ATTACK_POWER_FORMULA2(x)	(10 + (x) * 2)
#define	dACL_OPTION_INCREASE_MAX_HP_FORMULA1(x)			(100 + (x) * 50)



//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기본 수치
enum
{
eIE_ADD_DEFENSE_POWER						,//	"방어력"
eIE_BLOCKING_RATE							,//	"방패 블럭률",
eIE_BELT_STACK_COUNT						,//	"벨트 스택 아이템수",
eIE_FREE_RARE_OPTION						,//	"접두사 생성",
//┃기본 수치
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃포션/기타 약품 효과
eIE_RECOVER_HP								,//	"체력 회복(수치)",
eIE_RECOVER_HP_PERCENTAGE					,//	"체력 회복(퍼센티지)",
eIE_RECOVER_CP								,//	"CP 충전(수치)",
eIE_RECOVER_CP_PERCENTAGE					,//	"CP 충전(퍼센티지)",
eIE_RECOVER_HP_CP							,//	"HP,CP를 회복(수치)",
eIE_RECOVER_HP_CP_PERCENTAGE				,//	"HP,CP를 회복(퍼센티지)",
eIE_RESURRECTION							,//	"타인 부활"	
eIE_SELF_RESURRECTION						,//	"본인 부활",
eIE_HEAL_STRANGE_STATUS						,//	"중화",
eIE_RECOVER_ALL_STRANGE_STATUS				,//	"이상계 상태 치료",
eIE_RECOVER_ALL_DECLINE_STATUS				,//	"저하계 상태 치료",
eIE_RECOVER_ALL_CURSE_STATUS				,//	"저주계 상태 치료",
eIE_RECOVER_ALL_BADSTATUS					,//	"모든 상태 이상 치료",
eIE_HEAL_POSION								,//	"중독 치료",
eIE_SHORT_WHILE_INCREASE_POWER				,//	"일시적으로 힘을 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_AGILITY			,//	"일시적으로 민첩성을 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_CONSTITUTION		,//	"일시적으로 건강을 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_WISDOM				,//	"일시적으로 지혜를 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_INTELIGENCE		,//	"일시적으로 지식을 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_CHARISMA			,//	"일시적으로 카리스마를 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_LUCK				,//	"일시적으로 행운을 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_ATTACK_POWER		,//	"일시적으로 공격력을 증가 시킨다.",
eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER		,//	"일시적으로 방어력을 증가 시킨다.",
eIE_SHORT_WHILE_INCREASE_HP					,//	"일시적으로 체력을 증가 시킨다.",
eIE_SHORT_WHILE_INCREASE_CP					,//	"일시적으로 CP를 증가 시킨다.",
eIE_SHORT_WHILE_SET_MAX_DAMAGE				,//	"일시적으로 무기 데미지 맥스로 유지",
eIE_SHORT_WHILE_SET_MAX_CP					,//	"일시적으로 CP 맥스로 유지",
//┃포션/기타 약품 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기타 소모성 아이템
eIE_OPEN_ARCA								,//	"보물 상자 열기",
eIE_OPEN_DOOR								,//	"문 열기",
eIE_OPEN_ARCA_AND_DOOR						,//	"문과 보물 상자 열기",
eIE_ONE_WAY_PORTAL							,//	"편도용 포탈",
eIE_PORTAL									,//	"왕복용 포탈",
eIE_USE_JEWEL								,//	"보석 사용",
//┃기타 소모성 아이템
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃공격 부가 효과/부가 데미지
eIE_INSTANCE_KILL							,//	"즉사",
eIE_MAKE_BERSERKER							,//	"대상을 버서크 상태로 만듦",
eIE_MAKE_ARMOR_BREAK						,//	"갑옷 파괴",
eIE_MAKE_WEAPON_BREAK						,//	"무기 파괴",
eIE_MAKE_POISONED							,//	"중독시킨다.",
eIE_MAKE_CONFUSE							,//	"혼란시킨다.",
eIE_MAKE_CHARMING							,//	"유혹",
eIE_MAKE_STUN								,//	"스턴",
eIE_MAKE_COLD								,//	"콜드",
eIE_HIT_FIRE_DAMAGE						,//	"불속성 데미지",
eIE_HIT_WATER_DAMAGE						,//	"물속성 데미지",
eIE_HIT_WIND_DAMAGE						,//	"바람속성 데미지",
eIE_HIT_TERRA_DAMAGE						,//	"대지속성 데미지",
eIE_HIT_LIGHT_DAMAGE						,//	"빛속성 데미지",
eIE_HIT_DARK_DAMAGE						,//	"어둠속성 데미지",
eIE_ADD_DAMAGE_TO_UNDEAD					,//	"언데드 사냥꾼",
eIE_ADD_DAMAGE_TO_DEMON						,//	"악마 사냥꾼",
eIE_ADD_DAMAGE_TO_HUMAN						,//	"인간 사냥꾼",
eIE_ADD_DAMAGE_TO_ANIMAL					,//	"동물 사냥꾼",
eIE_ADD_DAMAGE_TO_HOLY_ANIMAL				,//	"신수 사냥꾼",
//┃공격 부가 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 부가 효과
eIE_DECREASE_RELOAD_TIME					,//	"벨트에 아이템 채우는 시간 감소",
eIE_INCREASE_TRANSFORMATION_SPEED			,//	"변신 속도 빨라짐",
eIE_INCREASE_RECOVER_SPEED					,//	"물약에 의한 회복 속도 증가",
eIE_RETURN_DAMAGE							,//	"가시돋힘",
eIE_INFINITY_BULLET							,//	"무한 탄환",
//┃각종 부가 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 수치 증가
eIE_INCREASE_MELEE_JOB_ATTACK_POWER			,//	"밀리 직업들 공격력 상승",
eIE_INCREASE_POWER							,//	"힘 상승",
eIE_INCREASE_WISDON							,//	"지혜 상승",
eIE_INCREASE_INTELIGENCE					,//	"지식 상승",
eIE_INCREASE_CONSTITUTION					,//	"건강 상승",
eIE_INCREASE_AGILITY						,//	"민첩성 상승",
eIE_INCREASE_CHARISMA						,//	"카리스마 상승",
eIE_INCREASE_LUCK							,//	"행운 상승",
eIE_INCREASE_CRITICAL						,//	"크리티컬 발생확률",
eIE_INCREASE_CRUSH_SHOT						,//	"결정타 발생확률",
eIE_INCREASE_HIT_RATE						,//	"명중률 증가",
eIE_INCREASE_CONCENTRATION					,//	"집중력 증가",
eIE_INCREASE_ATTACK_SPEED					,//	"공격 속도 증가",
//┃각종 수치 증가
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃저항
eIE_RESISTANCE_CURSE_STATUS					,//	"저주 상태에 대한 저항",
eIE_RESISTANCE_DECLINE_STATUS				,//	"저하 상태에 대한 저항",
eIE_RESISTANCE_STRANGE_STATUS				,//	"이상 상태에 대한 저항",
eIE_RESISTANCE_ALL_BAD_STATUS				,//	"모든 상태 이상에 대한 저항",
eIE_INCREASE_FIRE_RESITANCE					,//	"불저항",
eIE_INCREASE_TERRA_RESITANCE				,//	"대지저항",
eIE_INCREASE_WIND_RESITANCE					,//	"바람저항",
eIE_INCREASE_WATER_RESITANCE				,//	"물저항",
eIE_INCREASE_LIGHT_RESITANCE				,//	"빛저항",
eIE_INCREASE_DARK_RESITANCE					,//	"어둠저항",
eIE_INCREASE_FOUR_ELEMENTAL_RESITANCE		,//	"불,물,바람,대지에 대한 저항",
eIE_INCREASE_MAGIC_RESITANCE				,//	"마법 속성 공격에 대한 저항",
eIE_INCREASE_NOCK_BACK_RESISTANCE			,//	"녹백 저항",
eIE_INCREASE_CRITICAL_RESISTANCE			,//	"치명타 저항",
eIE_INCREASE_CRUSH_SHOT_RESISTANCE			,//	"결정타 저항",
eIE_INCREASE_INSTANCE_KILL_RESISTANCE		,//	"즉사 저항",
//┃저항
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃스킬레벨
eIE_INCREASE_MAGIC_LANCER_SKILL				,//	"마법 창병 스킬 레벨 증가",
eIE_INCREASE_MAGIC_ARCHER_SKILL				,//	"마법 궁수 스킬 레벨 증가",
eIE_INCREASE_KNIGHT_SKILL					,//	"기사 스킬 레벨 증가",
eIE_INCREASE_WARRIOR_SKILL					,//	"전사 계열 스킬 레벨 증가",
eIE_INCREASE_WIZARD_SKILL					,//	"마법사 스킬 레벨 증가",
eIE_INCREASE_WEREWOLF_SKILL					,//	"늑대인간 스킬 레벨 증가",
eIE_INCREASE_THIEF_SKILL					,//	"도둑 스킬 레벨 증가",
eIE_INCREASE_FIGHTER_SKILL					,//	"격투가 스킬 레벨 증가",
eIE_INCREASE_BEAST_TAMER_SKILL				,//	"조련사 스킬 레벨 증가",
eIE_INCREASE_SUMMONER_SKILL					,//	"소환사 스킬 레벨 증가",
eIE_INCREASE_PRINCESSE_SKILL				,//	"공주 스킬 레벨 증가",
eIE_INCREASE_MAGICAL_GIRL_SKILL				,//	"마법소녀 스킬 레벨 증가",
eIE_INCREASE_PRIEST_SKILL					,//	"사제 스킬 레벨 증가",
eIE_INCREASE_FALLEN_ANGEL_SKILL				,//	"타락천사 스킬 레벨 증가",
eIE_INCREASE_NECROMANCER_SKILL				,//	"네크로멘서 스킬 레벨 증가",
eIE_INCREASE_DEMON_SKILL					,//	"악마 스킬 레벨 증가",
eIE_INCREASE_FEMALE_SKILL_LEVEL				,//	"여성 캐릭터들의 스킬 레벨 증가",
eIE_INCREASE_MALE_SKILL_LEVEL				,//	"남성 캐릭터들의 스킬 레벨 증가",
eIE_INCREASE_SKILL_LEVEL					,//	"모든 스킬 레벨 증가",
//┃스킬레벨
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

eIE_INCREASE_DODGE_CHANCE					,//	"회피율 증가",
eIE_MAKE_CHARMING_30						,//	"30초동안 차밍"
eIE_MAKE_COLD_100							,//	"콜드(100%)",

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃신비한 마법 아이템
eIE_GROW_STRENGTH							,//	"힘 증가"
eIE_GROW_AGILITY							,//	"민첩성 증가"
eIE_GROW_CONSTITUTION						,//	"건강 증가"
eIE_GROW_WISDOM								,//	"지혜 증가"
eIE_GROW_INTELLIGENCE						,//	"지식 증가"
eIE_GROW_CHARISMA							,//	"카리스마 증가"
eIE_GROW_LUCK								,//	"행운 증가"
eIE_GROW_HP									,//	"체력 증가"
eIE_GROW_CP									,//	"CP 증가"
eIE_INCREASE_WEAPON_DAMAGE					,//	"무기 데미지 증가"
eIE_INCREASE_ARMOR_DEFENSE					,//	"갑옷 방어력 증가"
eIE_MAGIC_JEWEL								,//	"마법 보석"
//┃신비한 마법 아이템
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃아이템 데이터 입력후 추가됨
eIE_RETURN_DAMAGE_40						,//	"가시돋힘 40",
eIE_FRONTIER_SCROLL							,//	프론티어
eIE_RARE_ITEM_LOTTERY_TICKET				,//	레어 아이템 복권
eIE_UNIQUE_ITEM_LOTTERY_TICKET				,//	유니크 아이템 복권
eIE_SUPER_ITEM_LOTTERY_TICKET				,//	슈퍼 유니크 아이템 복권

eIE_RESET_SKILL_POINT						,//	스킬 포인트 리셋
eIE_RESET_STATE_POINT						,//	스탯 초기화
eIE_CHANGE_ITEM_PREFIX_2003_11_01			,//	아이템 접두사 변환(마법 대미지->물리 대미지)
eIE_CHANGE_ITEM_PREFIX_2003_11_17			,//	아이템 접두사 변환(마법 대미지->물리 대미지)

eIE_ORB_OF_SANTA								,	//	"산타의 오브",
eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE	,	//	"[0]분 동안 공격력을 [1]％ 증가 시킨다.",
eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE,	//	"[0]분 동안 방어력을 [1]％ 증가 시킨다.",
eIE_SHORT_WHILE_INCREASE_HP_BY_MINUTE			,	//	"[0]분 동안 체력을 [1]％ 증가 시킨다.",
eIE_SHORT_WHILE_INCREASE_CP_BY_MINUTE			,	//	"[0]분 동안 CP를 [1]％ 증가 시킨다.",
eIE_SHORT_WHILE_SET_MAX_DAMAGE_BY_MINUTE		,	//	"[0]분 동안 무기 대미지가 항상 최고로 유지된다.",
eIE_SHORT_WHILE_SET_MAX_CP_BY_MINUTE			,	//	"[0]분 동안 CP가 항상 최고로 유지된다.",
eIE_SHORT_WHILE_INCREASE_POWER_BY_MINUTE		,	//	"힘을 [0]만큼 [1]분 동안 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_AGILITY_BY_MINUTE		,	//	"민첨성을 [0]만큼 [1]분 동안 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_CONSTITUTION_BY_MINUTE	,	//	"건강을 [0]만큼 [1]분 동안 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_WISDOM_BY_MINUTE		,	//	"지혜흫 [0]만큼 [1]분 동안 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_INTELIGENCE_BY_MINUTE	,	//	"지식을 [0]만큼 [1]분 동안 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_CHARISMA_BY_MINUTE		,	//	"카리스마를 [0]만큼 [1]분 동안 상승 시킨다.",
eIE_SHORT_WHILE_INCREASE_LUCK_BY_MINUTE			,	//	"운을 [0]만큼 [1]분 동안 상승 시킨다.",
eIE_OVER_POWER_ITEM_EQUIP						,	//	"자신 레벨보다 [0]레벨 높은 제한 레벨 아이템 사용 가능",
eIE_MEMORY_STONE								,
eIE_TELEPORT_STONE								,
eIE_PERFECT_RESSURECTION						,	//	완전부활
eIE_CALL_MAGIC_CARPET							,	//	마법의 양탄자 소환
eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE_10	,	//	*10%
eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE_10	,
eIE_INCREASE_LEVEL								,	//	레벨 업
eIE_EVENT_2004_01_21							,	//	1월 21일 적용되는 이벤트 옵션
eIE_CHANGE_MAGIC_CARPET_SHAPE				,	//	마법의 양탄자 모양을 바꾼다.

eIE_JIM_MORYS_ENCHANT						,	//	짐 모리의 인챈트
eIE_UNSKILLED_REPAIR_BROKEN_ITEM			,	//	"부숴진 아이템 수리",
eIE_ENCHANT_ITEM_BY_UNIQUE_INFO				,	//	유니크 아이템에 의한 아이템 인챈트

eIE_REPAIR_BROKEN_ITEM						,	//	"부숴진 아이템 수리",
eIE_CURE_ITEM_EQUIP_CURSE					,	//	"장비에 붙어 있는 저주를 풀어준다.
eIE_RELEASE_ATTACH_PREFIX_CURSE				,	//	"장비에 붙어 있는 저주를 풀어준다.

eIE_OX_QUIZ_TICKET							,	//	OX 퀴즈 티켓이다.
eIE_INCREASE_SITDOWN_RECOVER_HP_SPEED		,	//	앉아 있을때 체력 회복 속도를 높혀준다.

eIE_CREATE_GUILD							,	//	길드 생성
eIE_GUILD_LEVEL_UP							,	//	길드 레벨을 올려준다.


eIE_GS_EXPAND_PACK							,
eIE_GS_SPECIAL_PACK							,
eIE_GS_LUXURY_PACK							,
eIE_GS_HANGUL_PACK							,
eIE_GS_ENGLISH_PACK							,
eIE_GS_HANMUN_PACK							,
eIE_GS_ITEM_PACK							,
eIE_GS_ANIMAL_PACK							,

eIE_PS_SIGNBOARD1							,
eIE_PS_COLOR_TEXT							,
eIE_PS_BOLD_TEXT							,
eIE_PS_EXPAND_TEXT_BUFFER					,
eIE_PS_TWINKLE								,
eIE_PS_PACK									,
eIE_PS_SIGNBOARD2							,
eIE_PS_SIGNBOARD3							,
eIE_PS_SIGNBOARD4							,
eIE_PS_SIGNBOARD5							,
eIE_PS_LOUD_SPEAKER							,

eIE_NORMAL_ITEM_LOTTERY_TICKET				,

eIE_WEAKNING_TARGET_FIRE_RESISTANCE			,
eIE_WEAKNING_TARGET_WATER_RESISTANCE		,
eIE_WEAKNING_TARGET_WIND_RESISTANCE			,
eIE_WEAKNING_TARGET_EARTH_RESISTANCE		,
eIE_WEAKNING_TARGET_LIGHT_RESISTANCE		,
eIE_WEAKNING_TARGET_DARK_RESISTANCE			,
eIE_WEAKNING_TARGET_MAGIC_RESISTANCE		,
eIE_STRENGTHENING_FIRE_DAMAGE				,
eIE_STRENGTHENING_WATER_DAMAGE				,
eIE_STRENGTHENING_WIND_DAMAGE				,
eIE_STRENGTHENING_EARTH_DAMAGE				,
eIE_STRENGTHENING_LIGHT_DAMAGE				,
eIE_STRENGTHENING_DARK_DAMAGE				,
eIE_STRENGTHENING_MAGIC_DAMAGE				,

eIE_BREEDING_RECORD							,
eIE_LOTTERY_ITEM							,
eIE_CHARGE_EXTRA_ITEM_POWER					,

eIE_REGEN_HP_ON_MAGIC_CARPET				,	//	양탄자위에서 체력 회복
eIE_NOT_DOWN_CP_ON_MAGIC_CARPET				,	//	양탄자위에서 CP 감소 안함
eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET		,	//	양탄자위에서 이동속도 증가
eIE_FIGHT_MAGIC_CARPET						,	//	양탄자위에서 마력충전

eIE_EXPAND_INVENTORY_BAG					,	//	인벤토리 확장 팩
eIE_REPAIR_DX_ITEM_KIT						,	//	EX 아이템 수리 키트


eIE_ITEM_PIECE								,	//	아이템 조각
eIE_SECRET_DUNGEON_KEY						,	//	아이템 조각

eIE_REMOVE_PREFIX							,	//	아이템 조각

eIE_JUMP_FIELD_USE_WORLD_MAP				,	//	아이템 조각
eIE_PUT_VERTICAL_BANNER						,	//	가로 배너
eIE_PUT_HORIZON_BANNER						,	//	세로 배너

eIE_TREASURE_MAP							,	//	보물 지도

eIE_SHORT_WHILE_INCREASE_LUCKx100_BY_MINUTE	,	//	"운을 [0]*100만큼 [1]분 동안 상승 시킨다.",
eIE_RESET_GUILD_SKILL_POINT					,	//	길드 스킬 재분배 두루마리
eIE_PORTAL_STONE							,	//	포탈 스톤
eIE_COUPLE_RING_SET							,	//	커플 링 세트
eIE_COUPLE_RING								,	//	커플 링
eIE_MIX_ITEM_BOX							,	//	바닥 없는 상자
eIE_FIRE_WORK								,
eIE_RANDOM_FIRE_WORK						,	//	랜덤 불꽃놀이
eIE_INCREASE_SELECT_POWER					,	//	선택한 능력치 하나를 증가 시킴
eIE_PRESENT_COUPLE_RING						,	//	선물 용 커플 링
eIE_NOT_DEFINE_COUPLE_COUPLE_RING			,	//	상대가 정해지지 않은 커플 링

eIE_ADD_CATEGORY							,
eIE_ADD_PAGE								,
eIE_BOOK									,
eIE_PRINCESS_WEAPON							,

eIE_GUILD_HALL_PORTAL						,

eIE_SUMMON_GUILD_GUARDIAN					,	//	길드 가디언 소환

eIE_GUILD_STATUE_PIECE_EXPORTER				,	//	길드 석상 조각 추출기
eIE_GUILD_STATUE_PIECE						,	//	길드 석상 조각 추출기
eIE_TRANS_TO_OPERATOR						,	//	영자 변신


eIE_INC_LAST_DAMAGE							,	//	최종 대미지 중가
eIE_INC_HUNT_EXP							,	//	사냥 경험치 증가
eIE_INC_HUNT_ITEM_DROP_CHANCE				,	//	아이템 드랍 찬스 증가
eIE_CLEAR_DEATH_PENELTY						,	//	죽음 페널티 회복

eIE_INCREASE_MOVE_SPEED						,	//	이동 속도 증가
eIE_EVENT_BALLOON							,	//	이벤트 풍선
eIE_SELECT_ITEM_IN_ITEM_PACK				,	//	아이템 팩 안에 있는 아이템 선택
eIE_INCREASE_FINAL_DAMAGE					,	//	최종 대미지 증가


eIE_RECALL_MINIPET							,	//	미니펫 소환
eIE_EXTEND_MINIPET_TIME						,	//	미니펫의 시간을 늘려줌
eIE_EXTEND_ALL_MINIPET_TIME					,	//	모든 미니펫의 시간을 늘려줌
eIE_SEAL_MINIPET							,	//	미니펫 봉인
eIE_CHANGE_MINIPET_NAME						,	//	미니펫 이름 변경
eIE_MINIPET_POUCH							,	//	미니펫이 담긴 주머니

eIE_FLAKE_OF_DOING_ONE						,
eIE_FEATHER_OF_THINKING_ONE					,
eIE_TOY_OF_SIN_AND_RIN						,

eIE_REVERSIBLE_FRAME						,	//	양면 프레임

/////////////////	챕터 5용 추가 ////////////////////
eIE_CH5_SELECT_UPGRADE_TITLE,
eIE_CH5_RELASE_REVERSION,
eIE_CH5_RESET_GUILD_SKILL_POINT		,	//	길드 스킬 재분배 두루마리
eIE_CH5_BONUS_HP_AMULET,
eIE_CH5_BONUS_CP_AMULET,
eIE_CH5_OVER_POWER_ITEM_EQUIP,

eIE_CH5_REVERSE_SKILL_POINT,
eIE_CH5_REVERSE_STATE_POINT,

eIE_CH5_REMOVE_PREFIX,

eIE_CH5_COPY_ITEM	,
eIE_CH5_UPGRADE_NORMAL_ITEM_PREFIX_POWER	,	//	일반 아이템 접두사 1단계 상승
eIE_CH5_IMPROVE_NORMAL_ITEM_POWER		,	//	일반 아이템 파워 증가
eIE_CH5_UPGRADE_UNIQUE_ITEM_PREFIX_POWER	,	//	일반 아이템 접두사 1단계 상승
eIE_CH5_IMPROVE_UNIQUE_ITEM_POWER		,	//	일반 아이템 파워 증가
eIE_CH5_MIX_ITEM_BOX					,	//	붉은색 바닥 없는 상자
eIE_CH5_SELECT_ALLIGNMENT				,	//	성향 수치 선택
eIE_TIME_STAMP							,	//	시간 도장
eIE_ADD_TITLE							,	//	칭호 추가

eIE_HUNTING_EXP_BONUS					,	//	사냥 경험치 보너스

eIE_SET_BADGE							,	//	세트 배지
eIE_NAME_STONE							,	//	성명석
eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET_RED	,	//	양탄자위에서 이동속도 증가

eIE_DUEL_BUFF_FIRE		,	//	결투 버프 불
eIE_DUEL_BUFF_WATER		,	//	결투 버프 물
eIE_DUEL_BUFF_WIND		,	//	결투 버프 바람
eIE_DUEL_BUFF_EARTH		,	//	결투 버프 대지
eIE_SET_RUNE			,	//	세팅 룬

eIE_MINIPET_AWAKEN50		,	//	미니펫 3창 강화
eIE_MINIPET_AWAKEN100		,	//	미니펫 3창 강화

eIE_GS_SIGNAL_PACK			,	//	길드 문양 아이템	
eIE_GS_UNIQUE1_PACK			,	//	길드 문양 아이템	
eIE_GS_UNIQUE2_PACK			,	//	길드 문양 아이템	

eIE_RECOVER_HP2				,	//	"체력 회복(수치2) * 100 입니다.",s
eIE_RECOVER_CP2				,	//	"CP 회복(수치2) * 100 입니다.",s

eIE_CASTING_MAGIC			,	//	보조 마법을 담는다.

eIE_RESET_STRENGTH_STATE		,	//	힘 스탯 하나 재 분배
eIE_RESET_AGILITY_STATE			,	//	민첩 스탯 하나 재 분배
eIE_RESET_CONSTITUTION_STATE	,	//	건강 스탯 하나 재 분배
eIE_RESET_INTELLIGENCE_STATE	,	//	지식 스탯 하나 재 분배
eIE_RESET_WISDOM_STATE			,	//	지혜 스탯 하나 재 분배
eIE_RESET_CHARISMA_STATE		,	//	카리스마 스탯 하나 재 분배
eIE_RESET_LUCK_STATE			,	//	운 스탯 하나 재 분배

eIE_USE_WORLD_MAP_TELEPORT		,	//	월드맵을 이용한 텔레포트
eIE_PERFECT_PARTY_REVIVE		,	//	파티원 전체 완벽 부활
eIE_INCREASE_SECRET_DUNGEON_PLAY_COUNT	,	//비던	플레이 횟수 증가
eIE_INCREASE_GAIN_MINIPET_EXP	,	//	미니펫이 먹이를 먹어서 올라가는 경험치 증가
eIE_EQUIP_INCREASE_MOVE_SPEED	,	//	이속 증가

eIE_INSTANCE_RECOVER_HP_PERCENTAGE		,	//	즉석 체력 회복
eIE_INSTANCE_RECOVER_HPx100			,	//	즉석 체력 회복 옵션 수치 * 100
eIE_INCREASE_GUILD_DUNGEON_PLAY_COUNT,	//	길드 던젼 플레이 횟수 증가

eIE_CHANGE_PET_NAME,

eIE_SOUND_OF_LEADERS_BELL,

eIE_AB_PH_DAMAGE		,

eIE_REFLECTING_MIXER	,	//	리플렉팅 믹서(미니펫 3개를 갈아 버린다!)
eIE_CHAIN_OF_RESTRAINT	,	//	속박의 사슬(미니펫 먹어서 능력치 업!!)
eIE_SEAL_SPHERE_OF_DAWN	,	//	새벽의 봉인구 

eIE_HIT_FIRE_DAMAGE_DX,			// VV09.08.07	 마법 추가데미지 DX  -,.-;; 
eIE_HIT_WATER_DAMAGE_DX,	
eIE_HIT_WIND_DAMAGE_DX,
eIE_HIT_TERRA_DAMAGE_DX,
eIE_HIT_LIGHT_DAMAGE_DX,
eIE_HIT_DARK_DAMAGE_DX,			// AA09.08.07

eIE_INCREASE_SOUL_BRINGER_SKILL, // 영술사 스킬레벨증가.
eIE_ATTACH_PREFIX				,//	몇%의 확률로 접두사를 붙인다.
eIE_COPY_PREFIX				,//	접두사를 복사한다.
eIE_CUT_PREFIX				,	// 접두사를 잘라낸다.

eIE_DECREASE_INSTANT_HP_PERCENTAGE		,	//	남은 체력 즉시 감소(퍼센티지)

eIE_ATTACH_PREFIX_CAN_NOT_TRADE				,//	몇%의 확률로 접두사를 붙인다.(거래불가)
eIE_COPY_PREFIX_CAN_NOT_TRADE				,//	접두사를 복사한다.(거래불가)
eIE_CUT_PREFIX_CAN_NOT_TRADE				,	// 접두사를 잘라낸다.(거래불가)
eIE_EVENT_BALLOON_INC_ITEM_DROP_CHANCE		,	// 이벤트 풍선 아이템 드랍률증가
eIE_INCREASE_ALL_STATUS						,	// 모든 능력치 증가
eIE_INCREASE_FIX_SELECT_POWER				,	//	선택한 능력치 상승(고정)
eIE_TRANSFORMATION_HUNTER				,	//	변신(사냥꾼)
eIE_TRANSFORMATION_THIEF				,	//	변신(도둑)
eIE_TRANSFORMATION_MAGICIAN				,	//	변신(마법사)
eIE_SUMMON_MONSTER						,	//	몬스터소환
eIE_CARVING_LEVEL							,	//	각인 레벨
eIE_INCREASE_CARVING_LEVEL					,	//	각인 레벨 증가 아이템.
eIE_SET_CARVING_LEVEL					,	//	각인 레벨을 설정 한다.
eIE_ACL_SHIELD							,	// 착용시 확률로 받은 데미지를 감소시킨다.
eIE_ACL_CHARGE							,	// 착용시 매 X초마다 x를 회복한다.
eIE_ACL_INCREASE_EXP					,	//	획득 경험치가 %증가한다.
eIE_ACL_ADD_FIRE_DAMAGE				,	//	%확률로 물속성 대미지 추가
eIE_ACL_INCREASE_ALL_STATUS				,	//	모든 능력치를 올린다.
eIE_ACL_SOUL_GUARD						,	//	파티원의 방어력을 최상으로 끌어올린다.
eIE_ACL_TRANSFORMATION_REPTILE			,	// 퍼밀리어로 변신!
eIE_ACL_INCREASE_MAX_HP					,	// 최대 체력증가
eIE_ACL_INCREASE_ATTACK_POWER			,	//	공격력증가
eIE_ACL_HELL_PRISON						,	//	지정한 적을 뻐감옥에 확률로 초간 가둔다.
eIE_MINIPET_FEED						,	//	미니펫먹이
eIE_RESET_MINIPET_SKILL					,	//	미니펫스킬 초기화
eIE_SET_MINIPET_TYPE					,	//	미니펫 타입 변경
eIE_ADD_MAX_HPx100						,	//	최대 HP증가 *100
eIE_ADD_MAX_CPx100						,	//	최대 CP증가 *100
eIE_ADD_DEFENSE_PERCENTAGE				,	//	방어력 퍼센트 증가
eIE_ADD_MAX_HP_PERCENTAGE				,	//	HP 퍼센트 증가
eIE_ADD_MAX_CP_PERCENTAGE				,	//	CP 퍼센트 증가
eIE_CHANGE_HP							,	//	수치1 * 수치2 로 체력을 만든다.
eIE_MOVE_FIELD							,	//	필드 이동
eIE_INCREASE_LEVEL_GETING_RED_STONE		,	//	붉은 보석 획득 가능 레벨 +%d 
//┃아이템 데이터 입력후 추가됨
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};


enum
{
eIP_ADD_STRENGTH						,	//	힘 증감
eIP_ADD_AGILITY							,	//	민첩성 증감
eIP_ADD_CONSTITUTION					,	//	건강 증감
eIP_ADD_WISDOM							,	//	지혜 증감
eIP_ADD_INTELIGENCE						,	//	지식 증감
eIP_ADD_CHARISMA						,	//	카리스마 증감
eIP_ADD_LUCK							,	//	운 증감
eIP_FIXING_STRENGTH						,	//	힘 고정
eIP_FIXING_AGILITY						,	//	민첩성 고정
eIP_FIXING_CONSTITUTION					,	//	건강 고정
eIP_FIXING_WISDOM						,	//	지혜 고정
eIP_FIXING_INTELLIGENCE					,	//	지식 고정
eIP_FIXING_CHARISMA						,	//	카리스마 고정
eIP_FIXING_LUCK							,	//	운 고정
eIP_ADD_STRENGTH_BY_LEVEL				,	//	레벨 비례 힘 증감
eIP_ADD_AGILITY_BY_LEVEL				,	//	레벨 비례 민첩성 증감
eIP_ADD_CONSTITUTION_BY_LEVEL			,	//	레벨 비례 건강 증감
eIP_ADD_WISDOM_BY_LEVEL					,	//	레벨 비례 지혜 증감
eIP_ADD_INTELIGENCE_BY_LEVEL			,	//	레벨 비례 지식 증감
eIP_ADD_CHARISMA_BY_LEVEL				,	//	레벨 비례 카리스마 증감
eIP_ADD_LUCK_BY_LEVEL					,	//	레벨 비례 운 증감
eIP_ADD_DAMAGE_PERCENTAGE				,	//	무기 데미지 증감 (퍼센티지)
eIP_ADD_MIN_DAMAGE						,	//	무기 데미지 증감 (최소값)
eIP_ADD_MAX_DAMAGE						,	//	무기 데미지 증감 (최대값)
eIP_ADD_DEFENSE_PERCENTAGE				,	//	방어력 증감 (퍼센티지)
eIP_ADD_DEFENSE							,	//	방어력 증감 (수치)
eIP_ADD_MAX_HP_PERCENTAGE				,	//	최대 체력 증감 (퍼센티지)
eIP_ADD_MAX_HP							,	//	최대 체력 증감 (수치)
eIP_ADD_MAX_CP_PERCENTAGE				,	//	최대 CP 증감 (퍼센티지)
eIP_ADD_MAX_CP							,	//	최대 CP 증감 (수치)
eIP_LOW_CP								,	//	이동시 감소 한계 CP
eIP_CORRECT_HIT_RATE					,	//	공격 성공 확률 (명중률)
eIP_CORRECT_DODGE_RATE					,	//	공격을 피할 확률 (회피율)
eIP_SHIELD_BLOCK_RATE					,	//	방패 발동 확률
eIP_CRITICAL_HIT						,	//	치명타 발동 확률
eIP_CRUSH_SHOT							,	//	결정타 발동 확률
eIP_IGNORE_TARGET_DODGE_ABILITY			,	//	적 방어자의 회피 보정치 무시
eIP_IGNORE_ATTACKER_CORRECT_HIT_VALUE	,	//	적 공격자의 명중 보정치 무시
eIP_PERFECT_HIT							,	//	항상 명중
eIP_BLOCKING_SPEED						,	//	블러킹 속도
eIP_MOVE_SPEED							,	//	이동 속도
eIP_ACTION_SPEED						,	//	공격 속도
eIP_POTION_HEAL_SPEED					,	//	물약 회복 속도 조절
eIP_ADD_CONCENTRATION					,	//	CP 감소 속도 조절 (집중력)
eIP_FIRE_DAMAGE							,	//	불 속성 공격
eIP_WATER_DAMAGE						,	//	물 속성 공격
eIP_WIND_DAMAGE							,	//	바람 속성 공격
eIP_TERRA_DAMAGE						,	//	대지 속성 공격
eIP_LIGHT_DAMAGE						,	//	빛 속성 공격
eIP_DARK_DAMAGE							,	//	어둠 속성 공격
eIP_STEAL_HP							,	//	체력 스틸
eIP_BONUS_CP							,	//	보너스 CP
eIP_FLEE								,	//	도망
eIP_KNOCK_OUT							,	//	녹아웃
eIP_INSTANT_KILL						,	//	즉사
eIP_WEAPON_BREAKER						,	//	무기 파괴
eIP_ARMOR_BREAKER						,	//	갑옷 파괴
eIP_MAKE_COLD							,	//	콜드 상태
eIP_MAKE_FREEZE							,	//	프리즈 상태
eIP_MAKE_BLIND							,	//	장님 상태
eIP_MAKE_STUN							,	//	스턴 상태
eIP_MAKE_STONE							,	//	석화 상태
eIP_MAKE_CONFUSE						,	//	혼란 상태
eIP_MAKE_SLEEP							,	//	잠 상태
eIP_MAKE_CHARMING						,	//	차밍 상태
eIP_MAKE_FIRST_TARGET					,	//	우선 타겟이 된 상태
eIP_MAKE_AI_DOWN						,	//	AI 저하 상태
eIP_MAKE_BERSERK						,	//	버서크 상태
eIP_RESISTANCE_FIRE						,	//	불 속성 공격 저항
eIP_RESISTANCE_WATER					,	//	물 속성 공격 저항
eIP_RESISTANCE_WIND						,	//	바람 속성 공격 저항
eIP_RESISTANCE_TERRA					,	//	대지 속성 공격 저항
eIP_RESISTANCE_LIGHT					,	//	빛 속성 공격 저항
eIP_RESISTANCE_DARK						,	//	어둠 속성 공격 저항
eIP_RESISTANCE_MAGIC					,	//	모든 마법 속성 공격 저항
eIP_ABSORB_FIRE							,	//	불 속성 공격 흡수
eIP_ABSORB_WATER						,	//	물 속성 공격 흡수
eIP_ABSORB_WIND							,	//	바람 속성 공격 흡수
eIP_ABSORB_TERRA						,	//	대지 속성 공격 흡수
eIP_ABSORB_LIGHT						,	//	빛 속성 공격 흡수
eIP_ABSORB_DARK							,	//	어둠 속성 공격 흡수
eIP_ABSORB_MAGIC						,	//	모든 마법 속성 공격 흡수
eIP_RESISTANCE_FREEZE					,	//	프리즈 공격에 대한 저항
eIP_RESISTANCE_COLD						,	//	콜드 공격에 대한 저항
eIP_RESISTANCE_STUN						,	//	스턴 공격에 대한 저항
eIP_RESISTANCE_CONFUSE					,	//	혼란 공격에 대한 저항
eIP_RESISTANCE_CHARMING					,	//	차밍 공격에 대한 저항
eIP_RESISTANCE_STONE					,	//	석화 공격에 대한 저항
eIP_RESISTANCE_INSTANCE_KILL			,	//	즉사 공격에 대한 저항
eIP_RESISTANCE_POISON					,	//	중독 공격에 대한 저항
eIP_RESISTANCE_SLEEP					,	//	잠 공격에 대한 저항
eIP_RESISTANCE_STRANGE_STATUS			,	//	상태이상 공격에 대한 저항
eIP_RESISTANCE_DECLINE_STATUS			,	//	저하계 공격에 대한 저항
eIP_RESISTANCE_CURSE_STATUS				,	//	저주계 공격에 대한 저항
eIP_RESISTANCE_ALL_BAD_STATUS			,	//	모든 이상 공격에 대한 저항
eIP_RESISTANCE_KNOCK_BACK				,	//	녹백/넘어짐 공격에 대한 저항
eIP_RESISTANCE_CRITICAL_SHOT			,	//	치명타 공격에 대한 저항
eIP_RESISTANCE_CRUSH_SHOT				,	//	결정타 공격에 대한 저항
eIP_RETURN_DAMAGE						,	//	받은 데미지 일부 돌려 줌
eIP_DAMAGE_TO_CP						,	//	받은 데미지 CP 변환
eIP_MAKE_FREEZE_ATTACKER				,	//	공격자 프리즈
eIP_MAKE_COLD_ATTACKER					,	//	공격자 콜드
eIP_CHARGE_SKILL						,	//	스킬/마법 충전
eIP_JOB_SKILL_LEVEL						,	//	계열 스킬/마법 레벨 조절
eIP_ALL_SKILL_LEVEL						,	//	모든 스킬/마법 레벨 조절
eIP_FIRST_AID							,	//	타격 응급 처치
eIP_CHANGE_WEAPON_SPEED					,	//	무기 교환 속도
eIP_REGENERATION_HP						,	//	체력 리제네레이션
eIP_RESURRECTION_CHANCE					,	//	부활 확률
eIP_BLURRED								,	//	흐릿한 상태 (Blurred)
eIP_INVISIBLE							,	//	투명 상태 (Invisible)
eIP_LEVITATE							,	//	공중 부양
eIP_MAGIC_BULLET						,	//	마법 탄환
eIP_INFINITY_BULLET						,	//	무한 탄환
eIP_NOT_AIMED							,	//	타겟에서 제외
eIP_INCREASE_RARE_ITEM_DROP_CHANCE		,	//	마법 아이템 확률 증가
eIP_INCREASE_UNIQUE_ITEM_DROP_CHANCE	,	//	유니크 아이템 확률 증가
eIP_DECREASE_RELOAD_TIME				,	//	벨트 채우는 시간 감소
eIP_AUTO_RELOAD							,	//	벨트 자동 채우기

eIP_DISABLE								,	//	사용불가
eIP_CAN_NOT_STRIP						,	//	탈의 불기
eIP_CAN_NOT_ADD_PREFIX					,	//	추가 인챈트 불능

eIP_UPGRADE_FIRE_DAMAGE_ATTACK			, // "불 공격 1단계 상승",
eIP_UPGRADE_WATER_DAMAGE_ATTACK			, // "물 공격 1단계 상승",
eIP_UPGRADE_WIND_DAMAGE_ATTACK			, // "바람 공격 1단계 상승",
eIP_UPGRADE_EARTH_DAMAGE_ATTACK			, // "대지 공격 1단계 상승",
eIP_UPGRADE_LIGHT_DAMAGE_ATTACK			, // "빛 공격 1단계 상승",
eIP_UPGRADE_DARK_DAMAGE_ATTACK			, // "어둠 공격 1단계 상승",

eIP_CONTROL_SKILL_LEVEL_BY_DIFFICULTY	, // 레벨별 스킬/마법 레벨 조절
eIP_INCREASE_SUMMON_BEAST_STATUS		, // 소환수 스탯 증가
eIP_INCREASE_GAIN_EXP_FOR_PET			, // 펫 경험치 획득 증가

eIP_IS_SHOP_SPECIAL_ITEM				, // 상점표 특수 아이템
eIP_WEAKNING_TARGET_FIRE_RESISTANCE			,	//	"타겟의 불 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
eIP_WEAKNING_TARGET_WATER_RESISTANCE		,	//	"타겟의 물 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
eIP_WEAKNING_TARGET_WIND_RESISTANCE			,	//	"타겟의 바람 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
eIP_WEAKNING_TARGET_EARTH_RESISTANCE		,	//	"타겟의 대지 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
eIP_WEAKNING_TARGET_LIGHT_RESISTANCE		,	//	"타겟의 빛 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
eIP_WEAKNING_TARGET_DARK_RESISTANCE			,	//	"타겟의 어둠 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
eIP_WEAKNING_TARGET_MAGIC_RESISTANCE		,	//	"타겟의 마법 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
eIP_STRENGTHENING_FIRE_DAMAGE				,	//	"불 속성 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",
eIP_STRENGTHENING_WATER_DAMAGE				,	//	"물 속성 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",
eIP_STRENGTHENING_WIND_DAMAGE				,	//	"바람 속성 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",
eIP_STRENGTHENING_EARTH_DAMAGE				,	//	"대지 속성 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",
eIP_STRENGTHENING_LIGHT_DAMAGE				,	//	"빛 속성 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",
eIP_STRENGTHENING_DARK_DAMAGE				,	//	"어둠 속성 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",
eIP_STRENGTHENING_MAGIC_DAMAGE				,	//	"마법 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",

eIP_DECREASE_STRENGTH,	//	"착용자의 힘이 [spin]0[1,1000] 감소한다.",
eIP_DECREASE_AGILITY,	//	"착용자의 민첩성이 [spin]0[1,1000] 감소한다.",
eIP_DECREASE_CONSTITUTION,	//	"착용자의 건강이 [spin]0[1,1000] 감소한다.",
eIP_DECREASE_WISDOM,	//	"착용자의 지혜가 [spin]0[1,1000] 감소한다.",
eIP_DECREASE_INTELIGENCE,	//	"착용자의 지식이 [spin]0[1,1000] 감소한다.",
eIP_DECREASE_CHARISMA,	//	"착용자의 카리스마가 [spin]0[1,1000] 감소한다.",
eIP_DECREASE_LUCK,	//	"착용자의 운이 [spin]0[1,1000] 감소한다.",

eIP_DECLINE_MOVE_SPEED,	//	"이동 속도가 [spin]0[1,1000]퍼센트 감소한다.",
eIP_DECLINE_ACTION_SPEED,	//	"공격 속도가 [spin]0[1,1000]퍼센트 감소한다.",

eIP_ADD_DAMAGE_TO_UNDEAD					,//	"언데드 사냥꾼",
eIP_ADD_DAMAGE_TO_DEMON						,//	"악마 사냥꾼",
eIP_ADD_DAMAGE_TO_HUMAN						,//	"인간 사냥꾼",
eIP_ADD_DAMAGE_TO_ANIMAL					,//	"동물 사냥꾼",
eIP_ADD_DAMAGE_TO_HOLY_ANIMAL				,//	"신수 사냥꾼",
eIP_POTION_HEAL_SPEED2						,//	"물약 회복 속도",

eIP_DECLINE_DEFENSE_PERCENTAGE				,//	"방어력 [-0]％",
eIP_DECLINE_DEFENSE							,//	"방어력 [-0]",
eIP_DECLINE_HIT_RATE						,//	"명중률 [-0]％",
eIP_DECLINE_DODGE_RATE						,//	"명중률 [-0]％",

eIP_FIRE_DAMAGE_DX,	// VV09.08.07	 마법 추가데미지 DX  -,.-;; 
eIP_WATER_DAMAGE_DX,	
eIP_WIND_DAMAGE_DX,	
eIP_TERRA_DAMAGE_DX,	
eIP_LIGHT_DAMAGE_DX,	
eIP_DARK_DAMAGE_DX,// AA09.08.07
eIP_BUFF_SKILL_CASTING, // 이벤트 버프 스킬 사용~~~~ 09.10.12
eIP_INCREASE_ALL_STATUS,	// 모든 능력치 상승
eIP_ACL_BUFF_SKILL_CASTING,	//	 각인 레벨을 적용한 버프스킬 사용
eIP_INCREASE_HARD_BLOW		,	//	강타확률 증가
};

#define	dMAX_ITEM_EFFECT_DATA				512
#define	dMAX_ITEM_PREFIX_TYPE_COUNT			200
#define	dMAX_ITEM_PREFIX_COUNT				1024


#endif