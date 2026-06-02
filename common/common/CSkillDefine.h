#ifndef	_classSKILL_DEFINE_H
#define	_classSKILL_DEFINE_H

#ifndef __AFXWIN_H__
#ifndef _WINDOWS_
#include <windows.h>
#endif
#endif

#define		dSKILL_COMMENT_LENGTH				256
#define		dSKILL_POWER_UP_COMMENT_LENGTH		64
#define		dSKILL_NAME_LENGTH					32
#define		dSKILL_SOUND_FILE_LENGTH			32

#define		dLIMIT_SKILL_LEVEL					50

#define		dMAX_SKILL							2048
#define		dMAX_ACTIVE_SKILL					256
#define		dVALID_BLOCKER_CODE					dMAX_ACTIVE_SKILL


#define		dABILITY_COUNT						52
#define		dHERO_ABILITY_COUNT					52
#define		dMONSTER_ABILITY_COUNT				10

#define		dACTIVE_REACTION_COUNT				4
#define		dPLAYER_ACTIVE_REACTION_LIMIT		4
#define		dSKILL_SPARE_VALUE_COUNT			20	//	여분의 상태치
#define		dMAX_REQUIRE_SKILL_COUNT			5
#define		dEXTRA_EFFECT_COUNT					10

#define		dSKILL_ACTION_TO_OBJECT				0xfff9	//	보물 상자
#define		dSKILL_BASIC_ATTACK_TO_ARCA			0xfffa	//	보물 상자
#define		dSKILL_BASIC_ATTACK_TO_DOOR			0xfffb	//	문 공격
#define		dSKILL_BORDER_OF_ATTACK_TO_OBJECT	0xfffb	//	문 공격
#define		dSKILL_BASIC_ATTACK					0xfffe	//	기본공격
#define		dSKILL_CONTINUOUS_HIT_ATTACK		0xfffd	//	연속 타격 공격
#define		dSKILL_ILLUSION_ATTACK				0xfffc	//	딜레이 엉疎
#define		dSKILL_INREGULAR_SKILL				0xfff0	//	기본공격
#define		dSKILL_SERIAL_HOLYCROSS				144		// 홀리크로스시리얼번호 09.09.24 
#define		dSKILL_SERIAL_PLOT_OF_SHADOW		357		// 음모의 그림자 시리�E번호 09.09.29
#define		dSKILL_HELL_PRISON								381
#define		dJOB_MONSTER_START					201
#define		dBREAK_EQUIPMENT_FACTOR				2
#define		dNEED_STATE_EMPTY					0x3f	// 필요상태 

#define		dPASSIVE_SKILL_COUNT				5		//	패시�E뵀� 최�E�..


#define		dMAX_PHYSICAL_DAMAGE					20000000
#define		dMAX_MAGICAL_DAMAGE						20000000

const	int	c_iCoolTimeSkillCount	=	10;

#define		dSKILL_TRANSFORMATION_BOW_ATTACK				504
#define		dSKILL_TRANSFORMATION_SWORD_ATTACK				500
#define		dSKILL_TRANSFORMATION_FIRE_BOLT					545

#define		dMAX_BIT_STICK_ACTOR							10
#define		dMAX_CAN_USE_BIT_TO_ONE							2	
enum
{
	eATTACK_OBJECT_DOOR,
	eATTACK_OBJECT_ARCA,
};

enum
{
	eTARGET_OBJECT_TRAP	=	1,
	eTARGET_OBJECT_FIELD_TRAP,
	eTARGET_OBJECT_ARCA,
	eTARGET_OBJECT_DOOR,
};
enum
{
	eBUNSHIN_RUSH_ATTACK = 1,		// 분신 러쉬 공격
	eBUNSHIN_ROUND_ATTACK,			// 분신 주위돌면서 공격

};

//
//	Reaction Skill Active Trigger
enum
{
	eRSAT_NONE,			//	없�E	
	eRSAT_STRIKE,		//	누군가에게 사�E杉�.
	eRSAT_BLOCKING,		//	빚緞킹했다.
	eRSAT_HIT,			//	맞았다.
	eRSAT_DETECTING,	//	탐지할때
	eRSAT_DAMAGE_MAGIC_SKILL_SB,	//	마법 공격 당할때
	eRSAT_HIT_DAMAGE_PHYSICAL,	//	실제 �E訣嗤� 입�E�E� 물리 �E訣嗤� 입음 발동
	eRSAT_HIT_CLOSE_RANGE_ATTACK_COUNTER,	//	근접 공격을 받았을 때 카웝訴
	eRSAT_HIT_MISSILE_ATTACK,	//	원거리 공격을 받았을 때
	eRSAT_HIT_CLOSE_RANGE_ATTACK_DODGE_COUNTER,	//	근접 공격을 받았을 때 회피 카웝訴
	eRSAT_REST	,		//	정지상태
	eRSAT_DAMAGED,		//	�E訣E입을때
	eRSAT_JUDGED_BEATEN_TIMING,	//	맞았다�E판정 났을때
	eRSAT_HIT_PHYSICAL_DAMAGE_TIME_COUNTER,	//	물리공격 받았을때 카웝訴
	eRSAT_HIT_STRANGE_STATUS_ATTACK,		//	상태 이�E공격을 받았을때 카웝訴
	eRSAT_PHYSICAL_DAMAGED,	//	실제 �E訣嗤� 입�E�E� 물리 �E訣嗤� 입음 발동
};
//
//	Skill Type
enum
{
	eST_NORMAL,
	eST_PASSIVE,
	eST_REACTION,
	eST_SUPPORT,
	eST_REAL_PASSIVE,
};

//
//	Aid Magic
enum
{
	eAM_ADD_FIRE_DAMAGE,
	eAM_ADD_WATER_DAMAGE,
	eAM_ADD_WIND_DAMAGE,
	eAM_ADD_EARTH_DAMAGE,
	eAM_ADD_LIGHT_DAMAGE,
	eAM_ADD_DARK_DAMAGE,
};

enum
{//ADD Hit Type
	eAHT_POISON,
	eAHT_DARK,
	eAHT_BLEEDING,
	eAHT_ELECTRIC_SHOCK,
};

//
//	CE
enum
{
	eCE_STRANGE_STATUS_CURE			=	0x01,	//이상계�E纘쩜鵑燦튿봬》E	
	eCE_DECLINE_POWER_STATUS_CURE	=	0x02,	//저하계�E纘쩜鵑燦튿봬》E	
	eCE_CURSE_STATUS_CURE			=	0x04,	//저주계�E纘쩜鵑燦튿봬》E	
	eCE_ALL_STRANGE_STATUS_CURE		=	0x08,	//모든상태이상아군치�E	
	eCE_RESURRECTION				=	0x10,	//행동불능아군회복
};

//
//	소수점 처리
enum
{
	eDECIMAL_METHOD_LEAVE,	//	버림
	eDECIMAL_METHOD_ROUND_OFF,	//	반 올림
	eDECIMAL_METHOD_UP,			//	올림
};

//
//	사�E�EE
enum
{
	eSKILL_CAST_AT_ENEMY				=	0x00000001,
	eSKILL_CAST_AT_PLAYER				=	0x00000002,
	eSKILL_CAST_AT_DEATH_PLAYER			=	0x00000004,
	eSKILL_CAST_AT_GROUND				=	0x00000008,
	eSKILL_CAST_QUICK					=	0x00000010,
	eSKILL_CAST_AT_CASTER				=	0x00000020,
	eSKILL_CAST_ON_DEATH_SUMMON_BEAST	=	0x00000040,
	eSKILL_CAST_ON_PARTY_MEMBER			=	0x00000080,
	eSKILL_CAST_ON_PET					=	0x00000100,
	eSKILL_CAST_ON_ENEMY_PET			=	0x00000200,
	eSKILL_CAST_ON_DEATH_PET			=	0x00000400,
	eSKILL_CAST_ON_SUMMON_BEAST			=	0x00000800,
	eSKILL_CAST_ON_KELBY				=	0x00001000,
	eSKILL_CAST_ON_TRAP					=	0x00002000,
	eSKILL_CAST_ON_ARCA					=	0x00004000,
	eSKILL_CAST_ON_DOOR					=	0x00008000,
	eSKILL_CAST_ON_USER_PARTY_MEMBER	=	0x00010000,
};

//
//	데미�E타입
enum
{
	eDAMAGE_TYPE_PHYSICAL	=	0x01,
	eDAMAGE_TYPE_FIRE		=	0x02,
	eDAMAGE_TYPE_WATER		=	0x04,
	eDAMAGE_TYPE_WIND		=	0x08,
	eDAMAGE_TYPE_EARTH		=	0x10,
	eDAMAGE_TYPE_LIGHT		=	0x20,
	eDAMAGE_TYPE_DARK		=	0x40
};

enum
{
	eMDA_FIRE	,
	eMDA_WATER	,
	eMDA_WIND	,
	eMDA_EARTH	,
	eMDA_LIGHT	,
	eMDA_DARK	
};

//
//	스킬 종�E
enum
{
	eSKILL_TYPE_NORMAL_HIT					,	//	가�E일반적인 물리 공격
	eSKILL_TYPE_CONTINUOUS_HIT_BY_CHARACTER	,	//	캐릭터의 �E訣E프레임을 체크해서 2�E이�E때린다.
	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		,	//	트리거 프레임 이후에 수치에 의거해서 순차적으로 �E訣嗤� 입힌다.
	eSKILL_TYPE_CONTINUOUS_HIT_BY_ILLUSION	,	//	잔상에 의한 연속 타격
	eSKILL_TYPE_ATTACK_BY_BUNSHIN			,	//	분신에 의한 공격
	eSKILL_TYPE_SCIMITAR_CUTTING			,	//	시미터 커팅
	eSKILL_TYPE_RUSH						,	//	러쉬

	eSKILL_TYPE_JUMP_ATTACK					,	//	점프 공격
	eSKILL_TYPE_WILD_STAMP					,	//	와일�E스탬프
	eSKILL_TYPE_ALTERING_HITTER				,	//	올터링 히터
	eSKILL_TYPE_IMPACT						,	//	때리�E주위에 충격파가 생긴다.
	eSKILL_TYPE_WHIRLWIND					,	//	회오리�E자기를 중심으로 일정 영역을 공격한다.)
	eSKILL_TYPE_WHIRLTHRUSTING				,	//	돌려찰淘�E	
	eSKILL_TYPE_SURPRISING_RAID				,	//	서프라이징레이�E	
	eSKILL_TYPE_WHIRL_RUNNING				,	//	휠러닝
	eSKILL_TYPE_GUARDIAN_POST				,	//	{	"가디언포스트"			,
	eSKILL_TYPE_RADIAL_ARC					,	//	{	"레이디얼아크"			,
	eSKILL_TYPE_AWESOME_FORTRESS			,	//	오�E� 포트리스
	eSKILL_TYPE_BITE_HANGING				,	//	바이트 행잉
	eSKILL_TYPE_BOUNCING_LINEAR				,

	eSKILL_TYPE_FIRE_BALL					,	//	파이�E볼
	eSKILL_TYPE_MISSILE						,	//	미사일
	eSKILL_TYPE_CONTINUOUS_SHOOT			,	//	연속 발�E	
	eSKILL_TYPE_EXPLOSION_MISSILE			,	//	폭발미사일
	eSKILL_TYPE_PIERCING_MISSILE			,
	eSKILL_TYPE_ILLUSION_MISSILE			,	//	환상미사일
	eSKILL_TYPE_SPREAD_ARROW				,	//	스프레�E애로�E	
	eSKILL_TYPE_BIT_GLIDER					,	//	비트 글라이�E	
	eSKILL_TYPE_BOOMERANG					,	//	부메�E	
	eSKILL_TYPE_LASER						,	//	레이�E	
	eSKILL_TYPE_EXPLOSION					,	//	폭발�E	
	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	,	//	이미지의존폭발�E	
	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		,	//	진선�EЪ酬篇�... -_-?
	eSKILL_TYPE_LAND_MARKER					,	//	랜드마커
	eSKILL_TYPE_INTERVAL_SHOOTER			,	//	인터�E슈터
	eSKILL_TYPE_WATER_FALL					,	//	워터 �E	
	eSKILL_TYPE_TORNADO						,	//	토네이도
	eSKILL_TYPE_DROP_ON_HEAD				,	//	머리 위로 떨엉碑다.
	eSKILL_TYPE_MACHINE_GUN					,	//	머쉰건
	eSKILL_TYPE_MACHINE_MISSILE				,	//	머쉰의 미사일
	eSKILL_TYPE_SPECIAL_MISSILE				,	//	특�E미사일
	eSKILL_TYPE_TAIL_CHASER					,	//	테일 체이서
	eSKILL_TYPE_SPIN_ARISING				,	//	돌려일엉廐�E
	eSKILL_TYPE_ARMOR_DISASSEMBLER			,	//	아머디스엉蔓빚簞
	eSKILL_TYPE_AID_ATTACK					,	//	보조 공격
	eSKILL_TYPE_DOUBLE_TARGET				,	//	더�E타겟
	eSKILL_TYPE_GAS							,	//	가스
	eSKILL_TYPE_FLORENCE_SPECIAL			,	//	플로렌스 스페셜 타입
	eSKILL_TYPE_ROLLING_LOG					,
	eSKILL_TYPE_BIG_BULLET					,
	eSKILL_TYPE_THROW_ITEM					,	//	아이템 던지�E	
	eSKILL_TYPE_LIGHTNING_WINDER			,	//	라이트닝 와인�E	
	eSKILL_TYPE_MIRROR_IMAGE				,	//	"미러이미�E			,
	eSKILL_TYPE_CHAIN_LIGHTNING				,	//	"체인라이트닝"			,
	eSKILL_TYPE_FIRE_WALL					,	//	"파이엉�E				,
	eSKILL_TYPE_GIGA_LIGHTNING				,	//	"기가라이트닝"			,
	eSKILL_TYPE_CONTINUOUS_WHIRLWIND		,	//  지속�E璨瑚E.추가..
	eSKILL_TYPE_MULTI_MISSILE				,	// 멀티미사일.

//////////////////////////////////////////////////////
//	�E塚岵� 스킬 경�E	
	eSKILL_TYPE_AGRESSIVE_SKILL_BORDER		,	//	�E塚岵� 스킬 경�E//	�E塚岵� 스킬 경�E//////////////////////////////////////////////////////
	eSKILL_TYPE_JUMP						,	//	점프
	eSKILL_TYPE_PASSIVE_REACTION			,
	eSKILL_TYPE_ACTIVITY_REACTION			,
	eSKILL_TYPE_ICY_STALAGMITE				,	//	아이시스탈라그마이트
	eSKILL_TYPE_CHARGE						,	//	충�E	
	eSKILL_TYPE_HEAL						,	//	치�E	
	eSKILL_TYPE_DANCING_BLOCKER				,	//	덜牧 빚踏커
	eSKILL_TYPE_MAGIC_SHIELD				,	//	마법방패
	eSKILL_TYPE_ADVANCED_BLOCKING			,	//	빚緞�E향�E	
	eSKILL_TYPE_AURA						,	//	오�E	
	eSKILL_TYPE_BLOCKING					,	//	막�E	
	eSKILL_TYPE_DODGE						,	//	회피
	eSKILL_TYPE_COUNTER						,	//	카웝訴
	eSKILL_TYPE_TELEPORT					,	//	텔레포트

	eSKILL_TYPE_AID_MAGIC					,	//	보조 마법
	eSKILL_TYPE_COMPLETE_PROTECTION			,	//	컴플리트 프로텍션
	eSKILL_TYPE_SHIMMERING_SHIELD			,	//	시머링 실�E	
	eSKILL_TYPE_MIRROR_TOWER				,	//	미러 타�E	
	eSKILL_TYPE_DISPLACEMENT				,	//	디스플레이스먼트

	eSKILL_TYPE_PET_COMMAND					,
	eSKILL_TYPE_ONE_BITE					,
	eSKILL_TYPE_MINIPET_COMBI				,


//////////////////////////////////////////////////////
//	임시 스킬 경�E	
//////////////////////////////////////////////////////

	eSKILL_TYPE_TRAP						,
	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER	,		// 분신의 연속공격
	eSKILL_TYPE_PASSIVE						,	// 패시�E스킬 
	eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT				,	//	비트에의한 그�E연타!
	eSKILL_TYPE_BOUNCING_MISSILE,						//	팅기는 미사일
	eSKILL_TYPE_FLY_BOUNCING_MISSILE,						//	날라가는 팅기는 미사일
	eSKILL_TYPE_WIDE_MISSILE,						//�E� 미사일
	eSKILL_TYPE_ARC_MISSILE,						//포물선 미사일

};

enum
{	// 패시�E염E	
	eSKILL_PEE_INCREASE_CONCENTRATION,	// 집중력
	eSKILL_PEE_INCREASE_HARD_BLOW,		// 강타
	eSKILL_PEE_INCREASE_MAX_HP	,	// 최�E섭쩝貂�
	eSKILL_PEE_INCREASE_STRANGE_STATUS_RESISTANCE	,	// 상태이상저항
	eSKILL_PEE_INCREASE_RANGE				,	// 사거리증가
	eSKILL_PEE_INCREASE_MAGIC_RESISTANCE,		//	마법저항증가.
	eSKILL_PEE_CORRECT_LIGHT_MIN_DAMAGE,				//	빛최소�E訣E보정	
	eSKILL_PEE_ADD_BIT_CHANCE,				//	비트부착확�E증가
	eSKILL_PEE_ADD_BIT_CHANCE_WHEN_BE_HIT,		//	맞을때 상�EE비트부착확�E증가
	eSKILL_PEE_INCREASE_HIT_RANGE,			//	타격�E㎲貂�

};

//
//	스킬 사�E� 위해 필요한 장�E

enum
{
	eREQUIRE_EQUIPMENT_FOR_SKILL_NONE			=	0,
	eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON			=	0x0001,
	eREQUIRE_EQUIPMENT_FOR_SKILL_SHIELD			=	0x0002,
	eREQUIRE_EQUIPMENT_FOR_SKILL_BULLET			=	0x0004,
	eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON2		=	0x0008,
	eREQUIRE_EQUIPMENT_FOR_SKILL_PET			=	0x0010,
	eREQUIRE_EQUIPMENT_FOR_SKILL_SUMMON_BEAST	=	0x0020,
	eREQUIRE_EQUIPMENT_FOR_SKILL_PET_OR_BEAST	=	0x0040,
	eREQUIRE_EQUIPMENT_FOR_SKILL_POTION			=	0x0080,
	eREQUIRE_EQUIPMENT_FOR_SKILL_FLOWER			=	0x0100,
	eREQUIRE_EQUIPMENT_FOR_SKILL_DRUG			=	0x0200,
	eREQUIRE_EQUIPMENT_FOR_SKILL_CANDY			=	0x0400,
};	//	스킬 사�E� 위해 필요한 장�E
enum
{
	ePC_ATTACK,
	ePC_DEFENSE,
	ePC_PATROL,
	ePC_SPECIAL_ACTION,
	ePC_USE_SKILL,
};

//
//	CP를 얻엉邦는 타이밍
enum
{
	eGET_CP_HIT,//히트 순간
	eGET_CP_FIRST_HIT,//첫번째 히트
	eGET_CP_DEFENSE,//막는 순간
	eGET_CP_COUNTER,//베는 순간
	eGET_ACTION_FINISH,//행동종�E	
	eGET_ACTION,//발동 순간
	eGET_LAND,//착지순간
	eGET_EVERY_HIT,//매�E히트시
	eGET_ENNEMY_IN_ATTACK_RANGE,//사정권 내 적이 들엉龐 순간
	eGET_DEFENSE_DODGE,//막는 순간,회피 순간
	eGET_PARTY_ATTACKED,//주위 아군 상태이�E마법 공격 받았을 시
	eGET_FIND_ENEMY,//�E하나 발견 할 때 마다
	eGET_ENEMY_DEATH,//�E하나 파�E할 때 마다
//적을 판별한 순간
//설치 완�E순간
//해체 완�E순간
//탐지한 순간
//기�E성공한 순간
//한 �E체력 회복 할 때마다
//마지막 히트
//아군이 엉喫 공격 받을때마다
//아군이 �E공격 받을때마다
//아군이 불,바�E공격받을때마다
//아군이 물,땅 공격받을때마다
//아군이 마법공격 받을때마다
//적군 하나 히트시마다
//아군 한 �E적�E첩떪�
//한 개 발견시마다
};

enum
{	// 필요상태.
	eNEED_STATE_CONSENSUS,		// 교감.
	eNEED_STATE_FIGHTING_SPIRIT	,	// 투�E
};

//
//	Extra Effext Adaptive Target
enum
{
	eEEAT_ALL			,
	eEEAT_ENEMY			,
	eEEAT_OWN_FORCE		,
	eEEAT_SELF			,
	eEEAT_PARTY_MEMBER	,
	eEEAT_PET			,
	eEEAT_ENEMY_PET		,
	eEEAT_SUMMON_BEAST	,
	eEEAT_PET_AND_BEAST	,
	eEEAT_SELF_AND_ENEMY,
	eEEAT_USER_PARTY_MEMBER	,
	eEEAT_ALL_EXCEPT_SELF,
	eEEAT_ENEMY_MONSTER,
	eEEAT_ACTIVE_SKILL_AREA,
};

//
//	스킬의 부가 효�E
enum
{
	eSKILL_EE_CONFUSE,	//	혼�E	
	eSKILL_EE_BREAK_ARMOR,	//	갑옷파괴
	eSKILL_EE_BREAK_WEAPON,	//	무기파괴
	eSKILL_EE_STUN,	//	경�E	
	eSKILL_EE_FREEZE,	//	프리�E	
	eSKILL_EE_COLD,	//	콜�E	
	eSKILL_EE_BLIND,	//	장님

	eSKILL_EE_DECLINE_HIT_CHANCE,	//	뫄鵬�E저하
	eSKILL_EE_DECLINE_DODGE_CHANCE,	//	회피율 저하

	eSKILL_EE_INCLINE_ATTACK_POWER_PERCENTAGE_FOR_PET_COMMAND,
	eSKILL_EE_INCLINE_DEFENSE_POWER_PERCENTAGE_FOR_PET_COMMAND,
	eSKILL_EE_RESET_ATTACK_DEFENSE_POWER_FOR_PET_COMMAND,

	eSKILL_EE_DECLINE_ATTACK_POWER,	//	공격력 저하
	eSKILL_EE_DECLINE_ATTACK_POWER_PERCENTAGE,
	eSKILL_EE_DECLINE_HIT_CHANCE_BY_LIGHT,	//	빛에 의한 뫄鵬�E저하
	eSKILL_EE_DECLINE_DODGE_CHANCE_BY_LIGHT,	//	빛에 의한 회피율 저하
	eSKILL_EE_BLIND_BY_LIGHT,	//	빛에 의한 장님
	eSKILL_EE_LIGHT_ATTACK_ADDITIONAL_EFFECT,	//	�E공격 부가효�E	
	eSKILL_EE_CURSE_AMPLIFICATION,
	eSKILL_EE_CREATE_DUMMY,	//	더미 생성
	eSKILL_EE_DECLINE_MOVE_SPEED,
	eSKILL_EE_DECLINE_ATTACK_SPEED,

	eSKILL_EE_INCLINE_HIT_CHANCE,	//	뫄鵬�E상승
	eSKILL_EE_INCLINE_DODGE_CHANCE,	//	회피율 증가
	eSKILL_EE_INCLINE_MOVE_SPEED,	//	이동 속도 증가
	eSKILL_EE_INCLINE_ATTACK_SPEED,	//	공격 속도 증가
	eSKILL_EE_LEVITATE,	//	공중부�E
	eSKILL_EE_INCLINE_ATTACK_POWER,	//	공격력 증가
	eSKILL_EE_INCLINE_ATTACK_POWER_PERCENTAGE,	//	공격력 퍼센티�E증가

	eSKILL_EE_DECREASE_FINAL_DEFENSE_POWER_PERCENTAGE,
	eSKILL_EE_DECREASE_DEFENSE_POWER,	//	방엉澹 증가
	eSKILL_EE_DECREASE_DEFENSE_POWER_PERCENTAGE,	//	방엉澹 퍼센티�E감소
	eSKILL_EE_INCREASE_DEFENSE_POWER,	//	방엉澹 증가
	eSKILL_EE_INCREASE_DEFENSE_POWER_PERCENTAGE,	//	방엉澹 퍼센티�E증가

	eSKILL_EE_INCLINE_STRANGE_STATUS_RESISTANCE,	//	이상상태 저항 증가
	eSKILL_EE_INCLINE_DECLINE_POWER_RESISTANCE,	//	능력치 저하 저항 증가
	eSKILL_EE_INCLINE_CURSE_RESISTANCE,	//	저주 저항 증가

	eSKILL_EE_DECLINE_MAGIC_RESISTANCE,
	eSKILL_EE_DECLINE_TARGET_MAGIC_RESISTANCE,	//	타겟마법저항감소

	eSKILL_EE_DECLINE_FIRE_RESISTANCE,	//	불저항 감소
	eSKILL_EE_DECLINE_WATER_RESISTANCE,	//	물저항 감소
	eSKILL_EE_DECLINE_WIND_RESISTANCE,	//	바람저항 감소
	eSKILL_EE_DECLINE_EARTH_RESISTANCE,	//	�E痔墟� 감소
	eSKILL_EE_DECLINE_LIGHT_RESISTANCE,	//	빛저항 감소
	eSKILL_EE_DECLINE_DARK_RESISTANCE,	//	암흑저항 감소

	eSKILL_EE_INCLINE_MAGIC_RESISTANCE,	//	마법 저항 증가
	eSKILL_EE_INCLINE_FIRE_RESISTANCE,	//	불저항 증가
	eSKILL_EE_INCLINE_WATER_RESISTANCE,	//	물저항 증가
	eSKILL_EE_INCLINE_WIND_RESISTANCE,	//	바람저항 증가
	eSKILL_EE_INCLINE_EARTH_RESISTANCE,	//	�E痔墟� 증가
	eSKILL_EE_INCLINE_LIGHT_RESISTANCE,	//	빛저항 증가
	eSKILL_EE_INCLINE_DARK_RESISTANCE,	//	암흑저항 증가
	eSKILL_EE_INCLINE_4ELEMENT_RESISTANCE,

	eSKILL_EE_INCLINE_CONCENTRATION,	//	집중력 증가
	eSKILL_EE_INCLINE_CONCENTRATION_BY_STELLAR_PEER,
	eSKILL_EE_DECLINE_CONCENTRATION,	//	집중력 감소

	eSKILL_EE_INCLINE_POWER,	//	힘증가"	
	eSKILL_EE_INCLINE_AGILITY,	//	민첩성증가"
	eSKILL_EE_INCLINE_CONSTITUTION,	//	건강증가"	
	eSKILL_EE_INCLINE_CHARISMA,	//	건강증가"	
	eSKILL_EE_INCLINE_INTELIGENCE,	//	지식증가"	
	eSKILL_EE_INCLINE_WISDOM,	//	지혜증가"	
	eSKILL_EE_INCLINE_LUCK,	//	웝琵가"	

	eSKILL_EE_INCLINE_MAX_CP,	//	CP증가
	eSKILL_EE_INCLINE_MAX_HP,	//	체력증가"	
	eSKILL_EE_INCLINE_MAX_HP_PERCENTAGE,	//	체력증가"	
	eSKILL_EE_REGEN_HP,	//	체력자동회복"	

	eSKILL_EE_NOCK_BACK,	//	�EE
	eSKILL_EE_CAN_NOT_MOVE_BY_EARTH,	//	이동불가 by �EE	
	eSKILL_EE_CAN_NOT_MOVE,	//	이동불가
	eSKILL_EE_DECLINE_MOVE_SPEED_BY_EARTH,	//	이동속도 저하 by �EE	
	eSKILL_EE_STUN_BY_EARTH,	//	경�Eby �EE	
	eSKILL_EE_STONE_BY_EARTH,	//	석화 by �EE	
	eSKILL_EE_POISON,	//	중독
	eSKILL_EE_CURE_POISON,	//	중독 치�E
	eSKILL_EE_INCLINE_BY_OWNER_FORCE,	//	동료에의한공격력상승
	eSKILL_EE_CHANGE_ALLIGNMENT,	//	성�E변�E
	eSKILL_EE_ADDITIONAL_FIRE_DAMAGE,	//	추가불�E訣E		,
	eSKILL_EE_ADDITIONAL_WATER_DAMAGE,	//	추가물�E訣E		,
	eSKILL_EE_ADDITIONAL_WIND_DAMAGE,	//	추가바람�E訣E	,
	eSKILL_EE_ADDITIONAL_EARTH_DAMAGE,	//	추가�E測E訣E	,
	eSKILL_EE_ADDITIONAL_LIGHT_DAMAGE,	//	추가빛�E訣E		,
	eSKILL_EE_ADDITIONAL_DARK_DAMAGE,	//	추가엉喫�E訣E	,

	eSKILL_EE_FIRE_DAMAGE,	//	불�E訣E		,
	eSKILL_EE_WATER_DAMAGE,	//	물�E訣E		,
	eSKILL_EE_WIND_DAMAGE,	//	바람�E訣E	,
	eSKILL_EE_EARTH_DAMAGE,	//	�E測E訣E	,
	eSKILL_EE_LIGHT_DAMAGE,	//	빛�E訣E		,
	eSKILL_EE_DARK_DAMAGE,	//	엉喫�E訣E	,

	eSKILL_EE_FIRE_DAMAGE_SHIELD,	//	불�E訣E실�E	
	eSKILL_EE_WATER_DAMAGE_SHIELD,	//	물�E訣E실�E	
	eSKILL_EE_WIND_DAMAGE_SHIELD,	//	바�E�E訣E실�E	
	eSKILL_EE_EARTH_DAMAGE_SHIELD,	//	�EE�E訣E실�E	
	eSKILL_EE_LIGHT_DAMAGE_SHIELD,	//	�E�E訣E실�E	
	eSKILL_EE_DARK_DAMAGE_SHIELD,	//	엉喫 �E訣E실�E

	eSKILL_EE_FLEE,
	eSKILL_EE_FLEE_BY_TURNUNDEAD,	//도망by턴언데�E	
	eSKILL_EE_STUN_BY_TURNUNDEAD,	//마비by턴언데�E	
	eSKILL_EE_INSTANCE_KILL_TURNUNDEAD,	//�E�by턴언데�E	
	eSKILL_EE_DESTROYING_UNDEAD,	//	디스트로잉언데�E,
	eSKILL_EE_DESTROYING_UNHOLY,	//	디스트로잉언홀리	,

	eSKILL_EE_LIFE_DRAIN,	//체력힐�E	
	eSKILL_EE_RECOVER_HP,	//체력회복
	eSKILL_EE_SLEEP,	//�E
	eSKILL_EE_CURE_STRANGE_STATUS,	//이상계�E纘쩜鵑燦튿봬》E	
	eSKILL_EE_CURE_CURSE,	//저주계�E纘쩜鵑燦튿봬》E	
	eSKILL_EE_CURE_DECLINE_POWER_STATUS,	//저하계�E纘쩜鵑燦튿봬》E	
	eSKILL_EE_DISPLACEMENT,	//	빙의
	eSKILL_EE_DETECTING,	//	디텍팅
	eSKILL_EE_DETECTING_EVIL,	//	디텍팅 이�E
	eSKILL_EE_DISPELLING,	//	디스펠링
	eSKILL_EE_REDUCE_CP,	//	CP감소
	eSKILL_EE_REDUCE_CP_PERCENTAGE,	//	CP감소(퍼센티�E
	eSKILL_EE_CHARMING,	//	챠밍
	eSKILL_EE_SANCTUARY,
	eSKILL_EE_WARP_NEAREST_VILLAGE,
	eSKILL_EE_CALLING,
	eSKILL_EE_CREATE_TOWN_PORTAL,
	eSKILL_EE_LEVEL_DRAIN,
	eSKILL_EE_INCREASE_LEVEL,
	eSKILL_EE_DECREASE_BLOCKING_CHANCE,

	eSKILL_EE_CHARMING_RESISTANCE,
	eSKILL_EE_TAME,
	eSKILL_EE_NONE_TARGET,	//	타겟 제외 상태
	eSKILL_EE_REAIM_TARGET,	//	타겟 �E설정
	eSKILL_EE_FIRST_AID,	//	응급처치
	eSKILL_EE_BREEDING_RECORD,	//	사육기록
	eSKILL_EE_BONUS_EXP,	//	경험치보너스

	eSKILL_EE_PET_BOOST,	//
	eSKILL_EE_BURST,	//

	eSKILL_EE_STRENGTHENING_FIRE_DAMAGE,
	eSKILL_EE_STRENGTHENING_WATER_DAMAGE,
	eSKILL_EE_STRENGTHENING_WIND_DAMAGE,
	eSKILL_EE_STRENGTHENING_EARTH_DAMAGE,
	eSKILL_EE_STRENGTHENING_LIGHT_DAMAGE,
	eSKILL_EE_STRENGTHENING_DARK_DAMAGE,
	eSKILL_EE_STRENGTHENING_MAGIC_DAMAGE,

	eSKILL_EE_DECLINE_CHARMING_RESISTANCE,
	eSKILL_EE_RALLY,
	eSKILL_EE_CHARMING_PET,

	eSKILL_EE_CALL_SUMMON_BEAST,
	eSKILL_EE_POWER_UP_SUMMON_BEAST,
	eSKILL_EE_RIDING_DOG,
	eSKILL_EE_INCREASE_SKILL_LEVEL,
	eSKILL_EE_SET_TRAP,
	eSKILL_EE_ADD_THORN,
	eSKILL_EE_INCLINE_CRITICAL_CHANCE,
	eSKILL_EE_PHYSICAL_DAMAGE_BASED_REMAIN_HP,
	eSKILL_EE_FINDING_TEASURE,
	eSKILL_EE_ITEM_COLLECTOR,
	eSKILL_EE_GOLD_HOLDER,

	eSKILL_EE_DECLINE_WEAPON_ATTACK_POWER_PERCENTAGE,
	eSKILL_EE_INCLINE_WEAPON_ATTACK_POWER,
	eSKILL_EE_INCLINE_ARMOR_DEFENSE_POWER,

	eSKILL_EE_TAUNT,
	eSKILL_EE_PET_TAUNT,
	eSKILL_EE_INCREASE_STUN_RESISTANCE,
	eSKILL_EE_DECREASE_PHYSICAL_DAMAGE,
	eSKILL_EE_INVINCIBLE,
	eSKILL_EE_DODGE_THIS_TIME_ATTACK,

	eSKILL_EE_REDUCE_STRANGE_STATUS_TIME,
	eSKILL_EE_REDUCE_CURSE_TIME,
	eSKILL_EE_REDUCE_DECLINE_POWER_STATUS_TIME,

	eSKILL_EE_MAKE_THE_OTHER_SELF,

	eSKILL_EE_RESISTANCE_KNOCK_BACK,
	eSKILL_EE_RESISTANCE_KNOCK_OUT,	//	�E아�E버티�E
	eSKILL_EE_INCREASE_RIGHT_PUNCH_SKILL_LEVEL,
	eSKILL_EE_INCREASE_LEFT_PUNCH_SKILL_LEVEL,
	eSKILL_EE_INCREASE_RIGHT_KICK_SKILL_LEVEL,
	eSKILL_EE_INCREASE_LEFT_KICK_SKILL_LEVEL,
	eSKILL_EE_INCREASE_DOWN_KICK_SKILL_LEVEL,

	eSKILL_EE_NEUTRALIZE_POISON,
	eSKILL_EE_NEUTRALIZE_FREEZE_STUN,

	eSKILL_EE_DETECTING_MONSTER,
	eSKILL_EE_INVISIBLE,
	eSKILL_EE_INSTANCE_KILL,

	eSKILL_EE_DISARM_TRAP,	//	"함정해체"			,,
	eSKILL_EE_DETECT_TRAP,	//	"함정탐�E			,,
	eSKILL_EE_DETECT_SECRET_OBJECT,	//	"비밀오�E㎷�슷E	,,
	eSKILL_EE_UNLOCK_ARCA,	//	"잠긴상자열�E		,,
	eSKILL_EE_UNLOCK_DOOR,	//	"잠긴문열�E		,,
	eSKILL_EE_PICKPOCKET,	//	"소매치�E			,,
	eSKILL_EE_ROBBERY,	//	"아이템강탈"		,,

	eSKILL_NOTIFICATION_OF_DEATH,	//	죽음의 예과�E	
	eSKILL_EE_STEAL_MONEY,
	eSKILL_EE_MIN_DAMAGE_FOR_THIS_TIME_ATTACK,
	eSKILL_EE_ANIMATE_PARTNER,
	eSKILL_EE_TORTURE,
	eSKILL_EE_MAKE_SKILL_ZONE,
	eSKILL_EE_MAKE_SHAKLE,	//	족�E생성
	eSKILL_EE_PERIODICAL_DARK_DAMAGE_BY_SLEEP,	//	주기적으로 엉喫 �E訣E입�E	
	eSKILL_EE_ULTIMATE_BARRIER,
	eSKILL_EE_PIGEON_POST,
	eSKILL_EE_BERSERK,
	eSKILL_EE_DANCE,
	eSKILL_EE_MAGIC_BOX,
	eSKILL_EE_COPY,

	eSKILL_EE_TRANS_FROG,	//	{	"개구리변신"		
	eSKILL_EE_TRANS_FROG2,	//	{	"개구리변신"		
	eSKILL_EE_TRANS_FAT_GIRL,	//	{	"비만�E			
	eSKILL_EE_TRANS_TO_WEAPON,	//	{	"무기변신"			
	eSKILL_EE_TRANS_TO_RABBIT,	//	{	"토끼변신"			
	eSKILL_EE_JUMP_JUMP,	//	{	"깡총거리�E		
	eSKILL_EE_FEIGN_DEATH,	//	{	"죽은척하�E		
	eSKILL_EE_ATTACK_UNDER_FEIGN_DATH,	//	{	"죽은척하면서공격하�E
	eSKILL_EE_BOUNCE,	//	튕기�E
	eSKILL_EE_DECREASE_ALL_RESISTANCE,	//	모�E저항 감소

	eSKILL_EE_EXPLOSION_BOTTLE,
	eSKILL_EE_CHARGE_CP,
	eSKILL_EE_MARIONETTE,	//	마리오네뜨

	eSKILL_EE_TRANS_POWER,	//	능력치변환"	
	eSKILL_EE_MAKE_ILLUSION,
	eSKILL_EE_DAMAGE_POOL,
	eSKILL_EE_IMPULSE,
	eSKILL_EE_BLOOD_DRAIN,
	eSKILL_EE_SELF_REPROOF,
	eSKILL_EE_DEGENERATE,
	eSKILL_EE_PHANTOM_IMPULSE,
	eSKILL_EE_TERRIBLE_DREAMS,
	eSKILL_EE_HWABYUNG,
	eSKILL_EE_ATTACK_COMMAND,
	eSKILL_EE_PLOT_OF_SHADOW,
	eSKILL_EE_SMELL_OF_DEATH,

	eSKILL_EE_DRAW_BODY,
	eSKILL_EE_CHAIN_DAMAGE,
	eSKILL_EE_CHAIN_FIRE_DAMAGE,
	eSKILL_EE_CHAIN_WATER_DAMAGE,
	eSKILL_EE_CHAIN_WIND_DAMAGE,
	eSKILL_EE_CHAIN_EARTH_DAMAGE,
	eSKILL_EE_CHAIN_LIGHT_DAMAGE,
	eSKILL_EE_CHAIN_DARK_DAMAGE,

	eSKILL_EE_CONTRACT,
	eSKILL_EE_DECLINE_RECOVER_HP_EFFICIENT,
	eSKILL_EE_INTERRUPTING_REVIVE,
	eSKILL_EE_BLOODY_CROSS,
	eSKILL_EE_UNFAIR_COVENANT,

	eSKILL_EE_SOUL_OATH,
	eSKILL_EE_BLOOD_COMPACT,
	eSKILL_EE_ANNULMENT_CONTRACT,

	eSKILL_EE_IMMUNE_STRANGE_STAUS,
	eSKILL_EE_IMMUNE_PHYSICAL_ATTACK,
	eSKILL_EE_IMMUNE_MAGIC_ATTACK,

	eSKILL_EE_INCREASE_FINAL_PHYSICAL_ATTACK_POWER,	//	최종물리공격력증가",

	eSKILL_EE_DODGE_MAGIC_DAMAGE,
	eSKILL_EE_FLY,
	eSKILL_EE_REGEN_MONSTER,
	eSKILL_EE_MONSTER_BOMB,

	eSKILL_EE_SWALLOW,	//	�E갚E	
	eSKILL_EE_INHALE_ENEMY,
	eSKILL_EE_DECREASE_SWALLOW_CORPES,	//	�E� 시체�E감소
	eSKILL_EE_CONVERSION_ICE,	//	모�E마법속성 공격 물 속성으로 �E�
	eSKILL_EE_FOG_FORM,
	eSKILL_EE_NO_MORE_FIGHT,
	eSKILL_EE_ONE_BITE,	//	한입!
	eSKILL_EE_ZOMBIE_BOMB,	//	좀�E폭탄

	eSKILL_EE_INCREASE_MAGIC_ITEM_DROP_CHANCE,
	eSKILL_EE_INCREASE_GOLD_DROP_CHANCE,

	eSKILL_EE_CORRECT_FIELD_FIRE_PENELTY,
	eSKILL_EE_CORRECT_FIELD_WATER_PENELTY,
	eSKILL_EE_CORRECT_FIELD_WIND_PENELTY,
	eSKILL_EE_CORRECT_FIELD_EARTH_PENELTY,
	eSKILL_EE_CORRECT_FIELD_LIGHT_PENELTY,
	eSKILL_EE_CORRECT_FIELD_DARK_PENELTY,
	eSKILL_EE_CORRECT_ITEM_EQUIP_LEVEL,
	eSKILL_EE_INTERRUPTING_WEAPON,					// 무기해제
	eSKILL_EE_DECREASE_FINAL_ATTACK_POWER_PERCENTAGE, // 최종공격력감소퍼센티�E
	eSKILL_EE_NOT_USE_ACTIVITY_REACTION_SKILL,		// 발동�E�硫퓨玲E北E	
	eSKILL_EE_CONSENSUS,							// 교감상태.
	eSKILL_EE_INCREASE_ALL_STATUS,		/// 모든능력치증가.
	eSKILL_EE_MADNESS,						// 광�E	
	eSKILL_EE_INCLINE_CRITICAL_DAMAGE_PERCENTAGE,	// 치뫄搔�E訣痴貂°蒡아셉E	
	eSKILL_EE_DRAIN_CP,			//CP힐�E
	eSKILL_EE_SOUL_CONVERSION,		// 소�E졸痔E
	eSKILL_EE_SOUL_BURN,			// 소�EE
	eSKILL_EE_RAGE_SOUL,				// 타오르는영혼..
	eSKILL_EE_BINDING_WORDS,			// 언엉拷속.
	eSKILL_EE_FIGHT_ENERGY,				// 투�E
	eSKILL_EE_RUSH,						// 돌�E	
	eSKILL_EE_FIGHTING_SPIRIT,			// 투�E
	eSKILL_EE_IGNORE_NEED_STATE,		// 필요상태무시
	eSKILL_EE_IGNORE_BLOCKING,			// 빚緞킹무시확�E	
	eSKILL_EE_RETURN_MISSILE_DAMAGE,	//	미사일�E訣嗤��
	eSKILL_EE_INCREASE_HARD_BLOW,		// 강타확�E貂�
	eSKILL_EE_JUMP,		// 점프
	eSKILL_EE_ATTACK_DOPPELGANGER,	//	분신공격
	eSKILL_EE_ADD_BIT,	//	비트추가
	eSKILL_EE_REMOVE_BIT,	//	비트제거
	eSKILL_EE_BLEEDING,	//	출�E	
	eSKILL_EE_ELECTRIC_SHOCK,	//	감�E	
	eSKILL_EE_INSTANCE_HEAL_EFFECT,	//	�E쳰虛염E	
	eSKILL_EE_MAKE_TYPE_UNDEAD,	//	언데드화
	eSKILL_EE_BLOOD_DONATION,	//	헌�E

};

enum
{	// 투기효�E	
	eSKILL_FEE_NONE,		// 없음..
	eSKILL_FEE_ADD_ABSOLUTE_ATTACK_POWER,	//	파괴 ( 절�E공격력 수치 추가)
	eSKILL_FEE_INCREASE_ATTACK_COUNT,		//	질풍 ( 공격 횟�E증가)
	eSKILL_FEE_INCREASE_GET_CP,		//	절제 (획�ECP 증가)
	eSKILL_FEE_INCREASE_ABSOLUTE_CRITICAL_PERCENT,		//	엉困니 ( 절�E크리티컬% 증가)
	eSKILL_FEE_DECREASE_COOL_TIME,		//	기�E( 쿨타임 감소 )

};

//
//	special skill effect
	enum
{
	eSKILL_SE_SNIPE_SHOT				=	0x0001,	//서있는 타겟 100% 뫄鵬
	eSKILL_SE_SKULL_SHOT				=	0x0002,	//서있는 타겟 �EE	
	eSKILL_SE_NOT_INTERRUPT				=	0x0004,	//적의 공격을 맞아도 꿋꿋이 버틴다. -_-;;
	eSKILL_SE_THROW_WEAPON				=	0x0008,	//무기던지�Eo-
	eSKILL_SE_CHANGE_JOB				=	0x0010,	//직업바꾸�E	
	eSKILL_SE_HIDE_SHIELD				=	0x0020,	//방패던지�Eo-
	eSKILL_SE_REMOVE_MISSLE				=	0x0040,	//미사일 제거
	eSKILL_SE_DEFENSE_KNOCKOUT			=	0x0080,	//�E틸E방�E	
	eSKILL_SE_TAUNT						=	0x0100,	//타웝獸
	eSKILL_SE_THROW_OTHER_JOB_WEAPON	=	0x0200,	//세컨�E무기던지�Eo-

};

//	스킬 사�E결�E
enum
{
	eSKILL_FIRE_RESULT_OK,							//	성�E
	eSKILL_FIRE_RESULT_FAILED,						//	실패
	eSKILL_FIRE_RESULT_NOT_APT_SKILL_TO_TARGET	,	//	타겟에게 적절치 않은 스킬이다.
	eSKILL_FIRE_RESULT_TOO_MANY_USE_THAT_SKILL	,	//	그 스킬을 너무 많이 사�E했엉
	eSKILL_FIRE_RESULT_BLOCKED_POINT,				//	점프 종�E스킬을 사�E狗� 했는데 해�E지점이 빚緞되�E있다.
	eSKILL_FIRE_RESULT_TOO_CLOSE,					//	점프 종�E스킬을 사�E狗� 했는데 해�E지점이 너무 가깝다.
	eSKILL_FIRE_RESULT_BLOCKED_OBJECT,
	eSKILL_FIRE_RESULT_CAN_NOT_FIND_TARGET_IN_ATTACK_RANGE,							//	성�E
	eSKILL_FIRE_RESULT_NOT_APT_SKILL_TO_THIS_STATUS,
	eSKILL_FIRE_RESULT_BY_BUSY	,					//	바뻐!!

};


enum
{
	eUSR_SUCCESS,
	eUSR_FAULT_SKILL,
	eUSR_WEAPON_NOT_FOUND,
	eUSR_SHIELD_NOT_FOUND,
	eUSR_BULLET_NOT_FOUND,
	eUSR_LOWER_CP,
	eUSR_VALID_SKILL,
	eUSR_CANO_NOT_CLOSE_ATTACK_ON_HILL,
	eUSR_PET_NOT_FOUND,
	eUSR_SUMMON_BEAST_NOT_FOUND,
	eUSR_REQUIRE_POTION,
	eUSR_REQUIRE_DURG,
	eUSR_REQUIRE_FLOWER,
	eUSR_REQUIRE_CANDY,
	eUSR_IS_COOL_TIME,
	eUSR_NOT_NEED_STATE,		// 필요상태..
	eUSR_BINDING_WORDS,	/// 언엉拷속상태..

};


//
//	Skill Ai Check Status

enum
{
	eSACS_ORDERING,							//	�E굴坪�
	eSACS_IN_SIGHT_OWNER_FORCE_HP,			//	시야내아군의HP
	eSACS_RANGE_TO_TARGET,					//	타겟과의거리
	eSACS_BUFF_ON_OWNER,					//	자신에게버프됨
	eSACS_BUFF_ON_TARGET,					//	타겟에게버프됨
	eSACS_OWNER_FORCE_COUNT_IN_SIGHT,		//	시야내동료의�E	
	eSACS_ENEMY_COUNT_IN_SIGHT,				//	시야내적의�E	
	eSACS_ENEMY_COUNT_IN_TARGET_AROUND,		//	타겟주위에적의�E	
	eSACS_DEATH_OWNER_FORCE_IN_SIGHT,		//	시야내아군의죽음
	eSACS_ON_BATTLE,						//	�E寸鑽�
	eSACS_ENABLE_CURSE_AMPLIFY,				//	저주로증폭가능
	eSACS_TARGET_IS_PET,					//	타겟이�E甄�
	eSACS_ACTIVE_CHANCE,					//	발동확�E	
	eSACS_CURRENT_HP,						//	현재HP
	eSACS_ATTACKED,							//	공격당함
	eSACS_CHECK_STATUS,						//	상태체크
	eSACS_CHECK_ANIMATE_PARTNER_COUNT,		//	에니메이트 파트너 횟�E체크
	eSACS_CHECK_TRAP_COUNT		,			//	함정개수체크
	eSACS_CHECK_SWALLOW_CORPES_COUNT	,	//	�E꼍쳉세稚셔�

};

//
//	status
enum
{
	eSTATUS_INVISIBLE,
};

enum
{
	eSage_book_color_normal		,
	eSage_book_color_red		,
	eSage_book_color_white_blue	,
	eSage_book_color_blue		,
};

//
//	Fire Ball Animation
enum
{
	eFBA_FLY,
	eFBA_FLOAT,
	eFBA_APPEARANCE
};

//
//	미사일이 날라가는 방�E
enum
{
	eFLY_MISSILE_DIRECT_NE,
	eFLY_MISSILE_DIRECT_SE,
	eFLY_MISSILE_DIRECT_SW,
	eFLY_MISSILE_DIRECT_NW,
};

//
//	잔�E이미�E타입
enum
{
eSKILL_AFTER_IMAGE_TYPE_NONE,
eSKILL_AFTER_IMAGE_SELF_COPY,
eSKILL_AFTER_IMAGE_NEXT_ANIMATION,
eSKILL_AFTER_IMAGE_INCREASE_FRAME,
eSKILL_AFTER_IMAGE_WATER_CANON_STYLE,
};	//	잔�E이미�E타입


//	HEOP	-	Hit Effect Ouput Part
enum
{
	eHEOP_HIT_ZONE,	//	타격 영역(sad화일에 설정되�E있다.)
	eHEOP_FOOT,		//	발 밑에..
	eHEOP_SHOULDER,	//	엉串에..
	eHEOP_ON_THE_HEAD,	//	머리 쪽에
};

//
//	After Image Type
enum
{
	eCAIT_NONE,
	eCAIT_DELAY,
};

//
//	Explosion Missile Status
enum
{
	eEMS_FLY,
	eEMS_EXPLOSION,
};

//
//	Shake Timing
enum
{
	eST_EXPLOSION,
	eST_CAST,
};

class	cREQUIRE_SKILL
{
public:
	WORD	m_wSkill;
	WORD	m_wLevel;

			cREQUIRE_SKILL()
			{
				m_wSkill	=	0xffff;
			}
};

//
//	스킬의 부가 효�E
class	CSkillExtraEffect
{
public:
	class	cInterruptingArmor
	{
	public:
		DWORD	m_bf7BeginLevel					:	7;
		DWORD	m_bf7EndLevel					:	7;
		DWORD	m_bf1InterruptingPartEaring		:	1;
		DWORD	m_bf1InterruptingPartNecklace	:	1;
		DWORD	m_bf1InterruptingPartBoots		:	1;
		DWORD	m_bf1InterruptingPartHelm		:	1;
		DWORD	m_bf1InterruptingPartGauntlet	:	1;
		DWORD	m_bf1InterruptingPartArmor		:	1;
		DWORD	m_bf1InterruptingPartRing		:	1;
		DWORD	m_bf1InterruptingPartShield		:	1;
	};

	WORD			m_wEffect;
	short			m_sActiveChance,m_sActiveChancePerLevel;
	short			m_sUpkeepTime,m_sUpkeepTimePerLevel;
	int				m_aiValue[10];
	WORD			m_wLimitActiveChance,m_wLimitUpkeepTime;

	WORD			m_bf1IsUpkeepTimeByFrame: 1;		//1

	WORD			m_bf4TargetMethod		: 4;		//5
	WORD			m_bf1TargetUndead		: 1;		//6
	WORD			m_bf1TargetHuman		: 1;		//7
	WORD			m_bf1TargetDemon		: 1;		//8
	WORD			m_bf1TargetAnimal		: 1;		//9
	WORD			m_bf1TargetHolyBeast	: 1;		//10
	WORD			m_bf1IsActiveChaceInversePropotionByRemainHP				:	1;	//11	발동확�E꼭봤섭쩔」趺廚�
	WORD			m_bf1IsActiveChaceInversePropotionByRemainHPxMonsterGrade1	:	1;	//12	발동확�E(남은체력비율x몬스터따坤(1:1,2:2,3:4,4:8)
	WORD			m_bf1IsStop				:	1;	//13	이거 성공하�E브��!!
	WORD			m_bf1IsCalcUpkeepTimeRoundOff	:	1;	//14	지속시간 반올림으로 계�E	
	WORD			:0;
	WORD			m_wExtraEffectImage ;		// 부가효�E이미�E
	WORD			m_wExtraEffectOutputPart;	// 부가효�E이미�E위치.
					CSkillExtraEffect(){reset();}

	inline	void	reset()
	{
		memset(this,0,sizeof(CSkillExtraEffect));
		
		m_bf1TargetUndead		=	TRUE;
		m_bf1TargetHuman		=	TRUE;
		m_bf1TargetDemon		=	TRUE;
		m_bf1TargetAnimal		=	TRUE;
		m_bf1TargetHolyBeast	=	TRUE;

		m_wEffect				=	0xffff;
		m_wLimitUpkeepTime		=	0xffff;
		m_wLimitActiveChance	=	10000;
		m_wExtraEffectImage		=	0xffff;		// 부가효�E이미�E
		m_wExtraEffectOutputPart	=	eHEOP_FOOT;	// 부가효�E이미�E위치.

	}
};	//	class	CSkillExtraEffect

class	CSkillExtraEffectInfo
{
public:
	WORD			m_wEffect;
	short			m_sActiveChance;
	int				m_iUpkeepTime;
	int				m_aiValue[10];

	inline	int		getNockBackDistance(int _iLevel)
	{
		return	(m_aiValue[0]+m_aiValue[1]*_iLevel)/100;
	}
};

#pragma	pack(2)
typedef union _union_skill_enchanted_image
{
    struct 
	{
		DWORD	m_bf1InclineAttackPower	:	1;
		DWORD	m_bf1InclineDefensePower:	1;
		DWORD	m_bf1InclineHitChance	:	1;
		DWORD	m_bf1InclineDodgeChance	:	1;
		DWORD	m_bf1InclineAttackSpeed	:	1;
		DWORD	m_bf1StrengthenMagicDamage:	1;
		DWORD	m_bf1StrengthenResistance:	1;
		DWORD	m_bf1InclineMoveSpeed	:	1;

		DWORD	m_bf1DeclineAttackPower	:	1;
		DWORD	m_bf1DeclineDefensePower:	1;
		DWORD	m_bf1DeclineHitChance	:	1;
		DWORD	m_bf1DeclineDodgeChance	:	1;
		DWORD	m_bf1DeclineAttackSpeed	:	1;
		DWORD	m_bf1WeaknessMagicRegistence:	1;
		DWORD	m_bf1WeaknessResistance	:	1;
		DWORD	m_bf1DeclineMoveSpeed	:	1;
		DWORD	m_bf1IsTorture			:	1;	//	17
		DWORD	m_bf1IsBloodDrain		:	1;	//
		DWORD	m_bf1IsReversalPower	:	1;	//	
		DWORD	m_bf1IsPlotOfShadow		:	1;	//	20
		DWORD	m_bf1IsBreakArmor		:	1;	//	
		DWORD	m_bf1IsBreakWeapon		:	1;	//	22

//	요기깩�E이름표 위에 표시됨
///////////////////////////////////////////////////////////////////////////////////
		DWORD	m_bf1EIFlameRing		:	1;
		DWORD	m_bf1Incinerate			:	1;
		DWORD	m_bf1VaccumPoint		:	1;
		DWORD	m_bf1HotSkin			:	1;
		DWORD	m_bf1DarkWeapon			:	1;
		DWORD	m_bf1CancerHall			:	1;	//
		DWORD	m_bf1UltimateBarrier	:	1;	//

		DWORD	m_bf1IsExistTheOtherSelf:	1;
		DWORD	m_bf1IsExistBiter		:	1;	//	31
		DWORD	m_bf1IsLevelDown		:	1;	//	32
///////////////////////////////////////////////////////////////////////////////////

		DWORD	m_bf1IsNotificationOfDeath:	1;	//
		DWORD	m_bf1IsAnimatePartner	:	1;	//	
		DWORD	m_bf1IsBlockToMove		:	1;	//

		DWORD	m_bf1IsBindByShakle		:	1;	//	1
		DWORD	m_bf1IsHiding			:	1;
		DWORD	m_bf1IsDisableInvisibleByAttack	:	1;
		DWORD	m_bf1IsRabbitRush		:	1;
		DWORD	m_bf1IsDance			:	1;
		DWORD	m_bf1IsShutInMagicBox	:	1;
		DWORD	m_bf1IsFeignDeath		:	1;
		DWORD	m_bf1IsRabbit			:	1;
		DWORD	m_bf1IsPlump			:	1;
		DWORD	m_bf1IsFrog				:	1;
		DWORD	m_bf1IsJumppingSkipping	:	1;	//	
		DWORD	m_bf1IsTransToWeapon	:	1;	//	
		DWORD	m_bf1IsImpulse			:	1;	//	
		DWORD	m_bf1IsHwabyung			:	1;	//

		DWORD	m_bf1IsSmellOfDeath		:	1;	//	18
		DWORD	m_bf1IsInterruptingArmor:	1;	//	19
		DWORD	m_bf1IsContract			:	1;	//	20
		DWORD	m_bf1IsBloodCompact		:	1;	//	21
		DWORD	m_bf1IsSoulOath			:	1;	//	22
		DWORD	m_bf1IsUseSpecialAttack	:	1;	//	23
		DWORD	m_bf1IsTaunted			:	1;	//	24
		DWORD	m_bf1IsLaziness			:	1;	//	25

		DWORD	m_bf1IsLightningBarrier	:	1;	//	26
		DWORD	m_bf1IsConversionIce	:	1;	//	27
		DWORD	m_bf1IsFogForm			:	1;	//	28
		DWORD	m_bf1IsNoMoreFight		:	1;	//	29
		DWORD	m_bf1IsZombieBomb		:	1;	//	30
		DWORD	m_bf1IsInterruptingWeapon : 1; // 31  무기해제
		DWORD	m_bf1IsNotUseActivityReactionSkill : 1; // 32 발동�E�硫퓨玲E北E

		DWORD	m_bf1IsConsensus			:	1;	//1 교감 상태.
		DWORD	m_bf1IsIncreaseAllStatus:1;		//2		모든능력치증가.
		DWORD	m_bf1IsMadness:1;		//3		광�E
		DWORD	m_bf1IsStrengtheningCriticalDamage :1;// 4 치뫄搔�E訣痴貂�.
		DWORD	m_bf1IsSoulBurn:1;					// 5 소�EE
		DWORD	m_bf1IsRageSoul:1;					//6 타오르는영혼.
		DWORD	m_bf1IsBindingWords:1;				// 7	언엉拷속.
		DWORD	m_bf1IsFlee		:1	;				// 8	공�E.
		DWORD	m_bf1IsAddAbsoluteAttackPower				:	1;
		DWORD	m_bf1IsIncreaseAttackCount					:	1;				//	10	공격횟수증가
		DWORD	m_bf1IsIncreaseGetCP						:	1;				//	11	획득CP증가
		DWORD	m_bf1IsIncreaseAbsoluteCriticalPercent		:	1;				//	12	절�Eづ㈇�셍�%증가
		DWORD	m_bf1IsDecreaseCoolTime						:	1;				//	13	쿨타임감소
		DWORD	m_bf1IsRushTime								:	1;				//	14	돌�E		
		DWORD	m_bf1IsFightingSpirit						:	1;				//	15	투�E		
		DWORD	m_bf1IsIgnoreBlocking						:	1;				//	16	빚緞킹무시
		DWORD	m_bf1IsReturnMissileDamage					:	1;
		DWORD	m_bf1IsIncreaseHardBlow						:	1;
		DWORD	m_bf1IsTransformationHunter					:	1;				//	19	사냥꾼 몬스터 변신
		DWORD	m_bf1IsTransformationThief					:	1;				//	20	좀도둑 몬스터변신
		DWORD	m_bf1IsTransformationMagician				:	1;				//	21	마법�E몬스터변신
		DWORD	m_bf1IsTransformationReptile				:	1;				//	22	퍼밀리�E몬스터변신
		DWORD	m_bf1IsImmunePhysicalDamage					:	1;
		DWORD	m_bf1IsGreamTant							:	1;				//	24	빛의장막
		DWORD	m_bf1IsPowerOfVitalization					:	1;				//	25	활성화의권능
		DWORD	m_bf1IsMakeTypeUndead						:	1;					//	26	언데드화
		DWORD	m_bf1IsElectricShock						:	1;					//	27	감�E		
		DWORD	:0;

    };

    struct 
	{
		DWORD	m_value1;
		DWORD	m_value2;
		DWORD	m_value3;
	};

	inline	void	orEffect(_union_skill_enchanted_image *_lpEffect)
	{
		m_value1	|=	_lpEffect->m_value1;
		m_value2	|=	_lpEffect->m_value2;
		m_value3	|=	_lpEffect->m_value3;
	}

	inline	void	reset()
	{
		m_value1	=	0;
		m_value2	=	0;
		m_value3	=	0;
	}

	inline	BOOL	isSame(_union_skill_enchanted_image *_lpEffect)
	{
		if	(m_value1	==	_lpEffect->m_value1	&&	m_value2	==	_lpEffect->m_value2	&&	m_value3	==	_lpEffect->m_value3)
			return	TRUE;

		return	FALSE;
	}

	inline	void	copy(_union_skill_enchanted_image *_lpEffect)
	{
		m_value1	=	_lpEffect->m_value1;
		m_value2	=	_lpEffect->m_value2;
		m_value3	=	_lpEffect->m_value3;
	}
	inline	BOOL	isExitEffect()
	{
		if	(m_value1+m_value2+m_value3)
			return	TRUE;

		return	FALSE;
	}

}	uEnchantedImage;
#pragma	pack()

enum
{
	eWAD_METHOD_CASTER		=	1,
	eWAD_MAX_EXPLOSION_RANGE,
	eWAD_EXPLOSION_RANGE	,
	eWAD_SHOOT_RANGE	,
	eWAD_TARGET_NON_TARGET,
};

//	CSkillDefine::m_wSkillUniqueSpecialFeature
enum
{
	eSUSF_IS_DISARM		=	1,
	eSUSF_IS_UNLOCK_DOOR,
	eSUSF_IS_UNLOCK_ARCA,
};


#define		dMAX_SKILL_AI_PATTERN				8
#define		dMAX_SKILL_AI_PATTERN_VALUE_COUNT	5

//
//	Skill Ai Target
enum
{
	eSAT_OWN_FORCE,
	eSAT_ENEMY,
	eSAT_SELF,
	eSAT_OWN_FORCE_CORPSE,
	eSAT_OWN_FORCE_SAME_ORDER_JOB_CORPSE,
};

class	cSkillAiPatternInfo
{
public:
	WORD	m_wTrigger;
	short	m_asValue[dMAX_SKILL_AI_PATTERN_VALUE_COUNT];
};

enum
{
	eSSD_CASTING,
	eSSD_ACTION,
	eSSD_HIT,
	eSSD_CREATE,
	eSSD_EXPLOSION,
	eSSD_MISS,

	eSSD_EE_SUCCESS,
	eSSD_EE_MISS,
};

class	CSkillDefine
{
public:
	class	CSoundInfo
	{
	public:
		char				m_strCasting[dSKILL_SOUND_FILE_LENGTH];
		char				m_strAction[dSKILL_SOUND_FILE_LENGTH];
		char				m_strHit[dSKILL_SOUND_FILE_LENGTH];
		char				m_strCreate[dSKILL_SOUND_FILE_LENGTH];
		char				m_strExplosion[dSKILL_SOUND_FILE_LENGTH];
		char				m_strMiss[dSKILL_SOUND_FILE_LENGTH];
		WORD				m_wSEE_Success,m_wSEE_Miss;

							CSoundInfo()	{reset();}

		void				reset()
		{
			m_strCasting[0]		=	0;
			m_strAction[0]		=	0;
			m_strHit[0]			=	0;
			m_strCreate[0]		=	0;
			m_strMiss[0]		=	0;
			m_wSEE_Success		=	0xffff;
			m_wSEE_Miss			=	0xffff;

		}
	};

	typedef union _union_check_status
	{
		struct 
		{
			DWORD	m_bf1IsPoison	:	1;
			DWORD	m_bf1IsContract	:	1;
			DWORD	m_bf1IsMinusCP	:	1;
		};

		struct 
		{
			DWORD	m_dwValue1;
		};

	}	uCheckStatus;

	WORD				m_wSerial;							//	시리�E	
	WORD				m_wIconIndex;						//	아이콘 인덱스
	WORD				m_wType;							//	타입
	WORD				m_wAction,m_wAction2;				//	기�E발동시 사�E求� 액션,액션이 2개 있을수도 있다. 이를테�E파이엉玲 준�E단계가 있�E발�E단계가 있다.
	WORD				m_wOverlapAction,m_wOverlapAction2;	//	오버랩되는 액션
	WORD				m_wReiterationDamageCountSyncWithOverlapAction;	//	오버랩되는 액션�E싱크해서 출력할 �E訣E횟�E	
	WORD				m_wEnableJob;						//	사�E가능한 직�E0xffff�E모�E직업이 가능
	WORD				m_wSpeed,m_wCorrectFPS;							//	속도

	char				m_strName[dSKILL_NAME_LENGTH];
	WORD				m_wDifficultyLevel;					//	기�E난이도
	WORD				m_wSkillGroup;						//	기�E그�E	
	WORD				m_wCastMethod;						//	액션,지속�E발동�E서포트,충�E
	WORD				m_wDamageAttribute;					//	�E訣E속성
	WORD				m_wTargetTypeLimit;						//	�EE제한
	DWORD				m_dwTargetMethod;

	WORD				m_wSpentHPPercentage,m_wSpentHPPercentageBasedBloodWing;				//	소�EHP 퍼센티�E	
	WORD				m_wSpentCP,m_wSpentCPPerLevel;		//	소�ECP
	WORD				m_wGetCP,m_wGetCPPerLevel,m_wGetCPTiming;	//	획�ECP,CP 획�E타이밍

	WORD				m_wMissileDustRange;					//	한�E사�E構E다시 사�E玖E꺼�E	
	WORD				m_wIsNotSyncCP;						//	CP소모를 서버와 싱크 시키�E않는다.
	WORD				m_wIsMagicDamageBasedLastHitDamage;	//	마법 �E訣側� 무�E공격력 기반이 아니�E마지막에 맞�E때린거의 더한거 합이다.
	DWORD				m_bf1IsIgnoreWeaponDamage			:	1;	//	무�E�E訣E무시
	DWORD				m_bf1IsPenetrationRush				:	1;	//	�EE��?
	DWORD				m_bf1IsLoopRushAni					:	1;	//	러쉬 에니가 루프한다.
	DWORD				m_bf1IsAttackOnlyTargetRush			:	1;
	DWORD				m_bf1IsWhirlRunningStyleRush		:	1;
	DWORD				m_bf9IsParallellRushAngle			:	9;
	DWORD				m_bf8ParallellRushGab				:	8;
	DWORD				m_bf1IsShimmeringShieldByWeapon		:	1;
	DWORD				m_bf1IsIgnoreTargetDodgeCorrectValue:	1;	//	타겟의 회피 모정 무시
	DWORD				m_bf1IsSyncShieldShape				:	1;	//	방패 모�E싱크
	DWORD				m_bf1IsNotWhirlTornado				:	1;
	DWORD				m_bf1SetRushImageByTriggerNextFrame	:	1;
	DWORD				m_bf1BeastBerserker					:	1;
	DWORD				m_bf1IsGlareSkill					:	1;	//	29
	DWORD				m_bf1NotRevenge						:	1;
	DWORD				m_bf1IsSummonBeastCommand			:	1;	//	소환�E뫄譚 
	DWORD				m_bf1IsSummonedBeastSkill			:	1;	//	소환�E스킬이다.

	cSkillAiPatternInfo	m_aAi[dMAX_SKILL_AI_PATTERN];
	WORD				m_wAI_Target;
	WORD				m_wLimitCrushChance;
	WORD				m_wShootRangeCorrect;
	WORD				m_wLimitShotCount;
	WORD				m_wSkillUniqueSpecialFeature;
	WORD				m_wImageRadius;
	WORD				m_wOutputEffect;
	WORD				m_wLimitActiveCount;
	WORD				m_wMissileFollowImage;						//	미스 낫을때 이미�E	
	BYTE				m_aTempBuffer[2];
	
	DWORD				m_bf1IsHideMissImage					:	1;
	DWORD				m_bf1IsRightPunchSkill					:	1;
	DWORD				m_bf1IsLeftPunchSkill					:	1;
	DWORD				m_bf1IsRightKickSkill					:	1;
	DWORD				m_bf1IsLeftKickSkill					:	1;
	DWORD				m_bf1IsDownKickSkill					:	1;
	DWORD				m_bf1IsCancelKnockBackSkill				:	1;
	DWORD				m_bf1IsDecreaseWeaponCount				:	1;
	DWORD				m_bf1IsInstanceApplyAura				:	1;
	DWORD				m_bf1IsDefendOnImageTrap				:	1;
	DWORD				m_bf1IsMonsterCounter					:	1;
	DWORD				m_bf1IsApplyHealPointByPercentage		:	1;
	DWORD				m_bf1IsIgnoreDodgeBlockReaction			:	1;
	DWORD				m_bf1IsAttackDamageBasedAttackerRemainHPRate	:	1;
	DWORD				m_bf1IsSelfDestructionSkill				:	1;	//	15
	DWORD				m_bf1IsRefitImageSizeByHitRange			:	1;
	DWORD				m_bf1IsCatchAndThrowingTypeAttackPower	:	1;
	DWORD				m_bf1IsTagetingToBattleWithPartyMember	:	1;
	DWORD				m_bf1IsPutHitDamage						:	1;
	DWORD				m_bf1IsNonAggressiveSkill				:	1;
	DWORD				m_bf1DefensePowerByCurseResistance		:	1;
	DWORD				m_bf1DarkDamageAttackByDamagePoint		:	1;	//	22
	DWORD				m_bf1IsPigeonPostItemSkill				:	1;
	DWORD				m_bf1IsLoopLastFrame					:	1;
	DWORD				m_bf1IsCastOnDestPosSkill				:	1;
	DWORD				m_bf1PutTrasAfterAnm					:	1;
	DWORD				m_bf1IsIllusionAttack					:	1;
	DWORD				m_bf1IsResistKnockBackByCurse			:	1;
	DWORD				m_bf1IsRequireFeignDeathStatus			:	1;
	DWORD				m_bf1IsStopWhenFailedFirstSE			:	1;
	DWORD				m_bf1IsToggleSkill						:	1;
	DWORD				m_bf1IsCastImageOnReleasPos				:	1;	//	32

	WORD				m_wDamageToHP,m_wDamageToHPPerLevel;	//	입�E�E訣E힐�E	
	WORD				m_wWADDamageMethod,m_wWADDamageGradeCount,m_wWADDamageValue,m_wWADMaxDamage,m_wWADMinDamage;

	WORD				m_bf1IsIgnoreTargetBlockingChance		:	1;	//	타겟의 방패 빚緞�E무시
	WORD				m_bf1IsRandomDustImageFrame				:	1;	//	타겟의 방패 빚緞�E무시
	WORD				m_bf1IsOnOffSkill						:	1;
	WORD				m_bf1IsGroundMissile					:	1;	//	땅타�E날아가는 미사일
	WORD				m_bf1IsApplyPhysicalDamageOnlyTarget	:	1;	//	타겟에게만물리�E訣痔好E	
	WORD				m_bf1IsParallelAfterImage				:	1;			//	수�E분신
	WORD				m_bf1IsSyncSkillExtraEffectWithPlayer	:	1;//	플레이엉盃 스킬 부가효�E싱크 시킴
	WORD				m_bf1IsMagicDamageBasedRemainHP			:	1;
	WORD				m_bf1IsZoomInEffectSkill				:	1;
	WORD				m_bf1IsHpSyncSkill						:	1;					//	얼렁뚱땅 HP를 싱크 시키는 스킬
	WORD				m_bf1IsTestSkill						:	1;	//	테스트중인 스킬
	WORD				m_bf1IsUsHealEffectToHitImageForAuraSkill:	1;	//	
	WORD				m_bf1IsNotIncreaseByActiveAura			:	1;
	WORD				m_bf1IsGetCPGasSkillByCasting			:	1;	//	14
	WORD				m_bf1IsRequireMissImage					:	1;	//	15	미스 이미�E필�E
	WORD				m_bf1IsBlockInDuelField					:	1;	//	16	결�E필드에서 사�E불가

	WORD				m_wCoolTimeIndex;		//	타겟에게만물리�E訣痔好E	
	WORD				m_wCoolTime;			//	수�E분신
	WORD				m_wCoolTimePerLevel;	//	플레이엉盃 스킬 부가효�E싱크 시킴
	WORD				m_wMinimumCoolTime;
	WORD				m_wCorrectTargetDefensivePower;
	WORD				m_wPetCommand;
	short				m_sIncreasePetPowerPerLevel;

	WORD				m_wLimitPhysicalDamage,m_wLimitMagicDamage;
	short				m_sLimitPhysicalDamagePerLevel,m_sLimitMagicDamagePerLevel;
	WORD				m_wCircleRange;						//	빙빙도는스킬의 반�E
	short				m_sAuraActivePeriod,m_sAuraActivePeriodPerLevel,m_sAuraActivePeriodDecimalMethod;	//	오�E스킬의 발동 주�E	
	WORD				m_wCureEffectField;					//	치�E효�E필�E	
	short				m_sApplyLimitHPPercentage,m_sApplyLimitHPPercentagePerLevel;
	WORD				m_wLimitApplyLimitHPPercentage;
	short				m_sTargetLevelLimitType1;
	short				m_sHumanTargetLevelLimitType1,m_sAnimalTargetLevelLimitType1,m_sHolyBeastTargetLevelLimitType1,m_sDemonTargetLevelLimitType1,m_sUndeadTargetLevelLimitType1;
	short				m_sHealPercentageBasedCurrentHP,m_sHealPercentageBasedCurrentHPperLevel;
	WORD				m_wHealPoint,m_wHealPointPerLevel;	//	치�E수치
	short				m_sHideWeaponTime,m_sHideWeaponTimePerLevel;

	WORD				m_wWideAreaAttackDamagePercent,m_wWideAreaAttackDamagePercentPerLevel;
	WORD				m_wBlackBluesFactor,m_wBlackBluesFactorPerLevel,m_wBlackBluesFactorLimit;
	short				m_sAttackPoint,m_sAttackPointPerLevel,m_sAttackPointRange,m_sAttackPointRangePerLevel,m_sCorrectAttackPoint;	//	공격력
	short				m_sAttackPercentage,m_sAttackPercentagePerLevel;
	short				m_sDefensePoint,m_sDefensePointPerLevel;	//	방엉澹
	short				m_sDefensePercentage,m_sDefensePercentagePerLevel;

	short				m_sFireDamage,m_sFireDamagePerLevel,m_sFireDamageRange,m_sFireDamageRangePerLevel;	//	불 �E訣E	
	short				m_sFireDamagePercentage,m_sFireDamagePercentagePerLevel,m_sFireDamagePercentageLimit;
	short				m_sWaterDamage,m_sWaterDamagePerLevel,m_sWaterDamageRange,m_sWaterDamageRangePerLevel;	//	물 �E訣E	
	short				m_sWaterDamagePercentage,m_sWaterDamagePercentagePerLevel,m_sWaterDamagePercentageLimit;
	short				m_sWindDamage,m_sWindDamagePerLevel,m_sWindDamageRange,m_sWindDamageRangePerLevel;	//	바�E�E訣E	
	short				m_sWindDamagePercentage,m_sWindDamagePercentagePerLevel,m_sWindDamagePercentageLimit;
	short				m_sEarthDamage,m_sEarthDamagePerLevel,m_sEarthDamageRange,m_sEarthDamageRangePerLevel;	//	�EE�E訣E	
	short				m_sEarthDamagePercentage,m_sEarthDamagePercentagePerLevel,m_sEarthDamagePercentageLimit;
	short				m_sLightDamage,m_sLightDamagePerLevel,m_sLightDamageRange,m_sLightDamageRangePerLevel;	//	�E�E訣E	
	short				m_sLightDamagePercentage,m_sLightDamagePercentagePerLevel,m_sLightDamagePercentageLimit;
	short				m_sDarkDamage,m_sDarkDamagePerLevel,m_sDarkDamageRange,m_sDarkDamageRangePerLevel;	//	엉喫 �E訣E	
	short				m_sDarkDamagePercentage,m_sDarkDamagePercentagePerLevel,m_sDarkDamagePercentageLimit;

	short				m_sPiercingChance,m_sPiercingChancePerLevel;

	WORD				m_wSpecialEffect;
	CSkillExtraEffect	m_aExtraEffect[dEXTRA_EFFECT_COUNT];
	WORD				m_wApplyExtraEffectCount;
	WORD				m_wActivateTrigger;
	short				m_sNockbackDistance,m_sNockbackDistancePerLevel;	//	�EE거리

	short				m_sCastingTime;		//	캐스팅 타임 
	short				m_sAttackSpeed,m_sAttackSpeedPerLevel,m_sLimitAttackSpeed;		//	공격 속도(1/16 프레임 단위 * 100)
	short				m_sCorrectAttackSpeed,m_sCorrectAttackSpeedPerLevel;

	WORD				m_wMinimumShootRange;					//	최소 사정 거리
	WORD				m_wShootRange,m_wShootRangePerLevel;	//	사정 거리
	WORD				m_wWeaponShootRangeCorrect,m_wWeaponShootRangeCorrectPerLevel;
	
	WORD				m_wMinimumHitRange,m_wHitRange,m_wHitRangePerLevel;		//	 타격 �E�
	WORD				m_wWeaponHitRangeCorrect;
	WORD				m_wAngle;							//	부채꼴 �E訣痔甄�.

	short				m_sHitChance,m_sHitChancePerLevel,m_sFixHitChance;	//	뫄鵬�E보정,레벨�E뫄鵬�E보정,뫄鵬�E	
	short				m_sDodgeChance,m_sDodgeChancePerLevel,m_sFixDodgeCahnce;	//	회피�E보정,레벨�E회피�E보정,회피�E	
	short				m_sCriticalChance,m_sCriticalChancePerLevel,m_sFixCriticalChance;	//	치뫄搔
	short				m_sCrushChance,m_sCrushChancePerLevel,m_sFixCrushChance;	//	크러쉬샷
	short				m_sCriticalToUndead,m_sCriticalToUndeadPerLevel,m_sCriticalToDemon,m_sCriticalToDemonPerLevel,m_sCriticalToAnimal,m_sCriticalToAnimalPerLevel;
	short				m_sCriticalToHuman,m_sCriticalToHumanPerLevel,m_sCriticalToHolyAnimal,m_sCriticalToHolyAnimalPerLevel;//각종 몬스터에 �E� 크리티컬 보정치
	WORD				m_wInstanceKillMethod;	//	�EE계�E방법
	short				m_sInstanceKillToUndead,m_sInstanceKillToDemon,m_sInstanceKillToAnimal,m_sInstanceKillToHuman,m_sInstanceKillToHolyAnimal;//각종 몬스터에 �E� �EE보정치
	short				m_sInstanceKillToUndeadPerLevel,m_sInstanceKillToDemonPerLevel,m_sInstanceKillToAnimalPerLevel,m_sInstanceKillToHumanPerLevel,m_sInstanceKillToHolyAnimalPerLevel;//각종 몬스터에 �E� �EE보정치
	short				m_sBlockingChance,m_sBlockingChancePerLevel,m_sMaximumBlockingChance,m_sFixBlockingChance;	//	빚緞킹 보정,레벨�E빚緞킹 보정,빚緞킹
	short				m_sConcentration,m_sConcentrationPerLevel;	//	집중력 보정,레벨�E집중력 보정,집중력

	short				m_sStunResistance,m_sStunResistancePerLevel;	//	마�E저항
	short				m_sStrangeStatusResistance,m_sStrangeStatusResistancePerLevel;	//	이상�E저항
	short				m_sDeclineResistance,m_sDeclineResistancePerLevel;	//	저하�E저항
	short				m_sCurseResistance,m_sCurseResistancePerLevel;	//	저주�E저항

	short				m_sFireResistance,m_sFireResistancePerLevel;	//	불 저항
	short				m_sWaterResistance,m_sWaterResistancePerLevel;	//	물 저항
	short				m_sWindResistance,m_sWindResistancePerLevel;	//	바�E저항
	short				m_sEarthResistance,m_sEarthResistancePerLevel;	//	�EE저항
	short				m_sLightResistance,m_sLightResistancePerLevel;	//	�E저항
	short				m_sDarkResistance,m_sDarkResistancePerLevel;	//	엉喫 저항

	short				m_sBulletCount,m_sBulletCountPerLevel,m_sBulletLimitCount,m_wBulletDecimalMethod;
	short				m_sShotCount,m_sShotCountPerLevel,m_wShotCountDecimalMethod;	//	동시 공격(?) 횟�E 소수점 처리 메소�E	
	short				m_sContiniousShotMaxCount,m_sContiniousShotCount,m_sContiniousShotCountPerLevel,m_sContiniousShotPeriod,m_wContiniousShotDecimalMethod;//연속 사격
	short				m_wIncreaseActionAnmCount;

	short				m_sCorrectTargetHitChance,m_sCorrectTargetHitChancePerLevel;	//	�E뫄鵬�E보정

	WORD				m_wUpkeepTime,m_wUpkeepTimePerLevel;	//	유�E시간

	short				m_sTargetFleeChance,m_sTargetStunChance;
	short				m_sInstanceKillChance,m_sInstanceKillChancePerLevel;	//	타겟이 도망 가거나 스턴되거나 �EE_-
	short				m_sTargetDecreaseHitChance,m_sTargetDecreaseHitChancePerLevel;	//	맞은넘은 뫄鵬�E저하

	short				m_isExemptToDemonType,m_isExemptToHumanType,m_isExemptToAnimalType,m_isExemptToHolyAnimalType,m_isExemptToUndeadType;	//	영향을 안 받는 몬스터
	short				m_sIntelligencePerLevel,m_sStrengthPerLevel,m_sLuckPerLevel,m_sAgilityPerLevel,m_sWisdomPerLevel,m_sCharismaPerLevel;

	short				m_sOccurActionPeriod;	//	보조마법에서 적�E주�E
	short				m_sHealPoint,m_sHealPointPerLevel;	//	체력 회복 수치
	short				m_sFirstAidPoint,m_sFirstAidPointPerLevel,m_sFirstAidUpkeepTime;	//	응급 조치
	short				m_sDisarmLevel,m_sDisarmLevelPerLevel,m_sDetectLevelPerLevel,m_sDetectLevel;	//	해체 레벨,탐�E레벨
	short				m_sActiveChance,m_sActiveChancePerLevel;		//	발동확�E발동확�E레벨 보정
	WORD				m_wActiveChanceByInRangeEnemyCount;
	WORD				m_wLimitActiveChance;

	WORD				m_wRequireEquipment;							//	필�E장�E	
	cREQUIRE_SKILL		m_aRequireSkill[dMAX_REQUIRE_SKILL_COUNT];

	CSoundInfo			m_sound;

	short				m_aValue[dSKILL_SPARE_VALUE_COUNT];		//	여분의 수치..

//	스킬 구현을 위해 필요한 데이터
	WORD				m_wImageScale;
	WORD				m_wTargetMarkImage;
	WORD				m_wShootImage;						//	날아갈때 이미�E	
	WORD				m_wMissileHeadImage;				//	날아가는 무�E머리에 �EE이미�E	
	WORD				m_wMachineImage;					//	머신 이미�E	
	WORD				m_wExplosionImage;					//	폭발할때 이미�E	
	WORD				m_wMissImage;						//	미스 낫을때 이미�E
	WORD				m_wAidAttackImage,m_wAidAttackImageOutputPart,m_wAidAttackImageEffect;
	WORD				m_wHitImage,m_wHitImageOutputPart,m_wHitImageEffect;
	WORD				m_wHealImage,m_wHealImageOutputPart,m_wHealImageEffect;
	WORD				m_wAddHitImage,m_wAddHitImageOutputPart,m_wAddHitImageEffect;
	WORD				m_wCastImage,m_wCastImageOutputPart,m_wCastImageEffect;
	WORD				m_wAidSkillCastingImage,m_wAidSkillCastingImageOutputPart,m_wAidSkillCastingImageEffect;	//	사�E秊� 시�E悶“� 나타나는 이미�E	
	WORD				m_wSkillImage,m_wSkillImageOutputPart,m_wSkillImageEffect;						//	스킬 이미�E	
	WORD				m_wCasterHitImage,m_wCasterHitImageOutputPart,m_wCasterHitImageEffect;		//	타격이미�E출력 부위
	WORD				m_wSwingImage,m_wSwingImageEffect;						//	무기를 휘두를때 나타나는 이미�E	
	WORD				m_wBottomImage;	//	바닥 이G

	WORD				m_wDodgeAngle;						//	회피 각
	WORD				m_wHitAngleRange,m_wHitAngleRangePerLevel;	//	타격각도�E�
	WORD				m_wDodgeDistance;					//	회피 거리
	WORD				m_wPaletteIndex;

	DWORD				m_dwEnchantedEffectMask;			//	마아스크~
	WORD				m_wEnchantedImage;					//	인챈트 이미�E	
	WORD				m_wDustImageRange;

	WORD				m_wShakeTiming;						//	언제 흔들껴?
	WORD				m_wShakeIntensity;					//	지진의 강도
	WORD				m_wShakeTime;						//	지진이 유지되는 시간..

	BYTE				m_bCharacterAfterImageType;			//	몸뚱아리 잔�E타입
	BYTE				m_bCharacterAfterImageDelayTime;	//	딜레이 캐릭터 잔�E딜레이 타임
	WORD				m_wStrikePeriod;
	short				m_sStrikePeriodPerLevel;
	WORD				m_wMinimumStrikePeriod;

	WORD				m_wAfterImageType;					//	잔�E타입
	WORD				m_wAfterImageGap;					//	잔�E간격
	WORD				m_wAfterImageFirstImageDistance;	//	두번째 이미�E위치
	WORD				m_wAfterImageFirstImageAlphaDepth,m_sAfterImageDecreaseAlphaDepthValue;	//	잔�E첫번째 이미지와 실 이미지의 간격
	WORD				m_wAfterImageCount;					//	잔상의 �E
	DWORD				m_bf1IsBlockOnlyMissilAttack	: 1;
	DWORD				m_bf1IsExclusiveAction			: 1;
	DWORD				m_bf5HitDamagePeriod			: 5;	//	�E訣E입히는 주�E	
	DWORD				m_bf1IsRoundRappedBunshinAttack	: 1;	//	분신 공격인데... 적을 둘러싼다.
	DWORD				m_bf5BunshineAlphaDepth			: 5;	//	분신 농도
	DWORD				m_bf3MaxPetCount				: 3;
	DWORD				m_bf2RequireSummonBeastGrade	: 2;
	DWORD				m_bf2OperateSummonBeast			: 2;
	DWORD				m_bf1IsPohibitAction			: 1;
	DWORD				m_bf1ShootRangeZeroSkill		: 1;
	DWORD				m_bf1IsTrap						: 1;
	DWORD				m_bf1IsFlatTrap					: 1;
	DWORD				m_bf1IsInstanceHeal				: 1;
	DWORD				m_bf1IsOnlySelfEnchantSkill		: 1;	//	자기한테만 인챈트 하는 스킬이다.
	DWORD				m_bf1IsSecondJobMachine			: 1;
	DWORD				m_bf1IsRapeExplosionImage		: 1;	//	28
	DWORD				m_bf1IsLaser					: 1;	//	29
	DWORD				m_bf1AttackByDamagedDamage		: 1;	//	30
	DWORD				m_bf1PlayCastSoundByEnchantAidSkill	: 1;	//	31
	DWORD				m_bf1RootAttackPower			: 1;	//	32

	uEnchantedImage		m_enchantedImage;
	WORD				m_wCommandSkill;

	uCheckStatus		m_checkTargetStatus,m_checkCasterStatus;

	DWORD				m_bf1IsObitianSkill				:	1;
	DWORD				m_bf1IsDashBladeSkill			:	1;
	DWORD				m_bf1IsAstroBowSkill			:	1;
	DWORD				m_bf1IsCristalWaterSkill		:	1;
	DWORD				m_bf1IsApplyToPartyAura			:	1;
	DWORD				m_bf4ChangeDirect				:	4;
	DWORD				m_bf1IsCastSelfBuff				:	1;
	DWORD				m_bf1IsExplosionAtCastPos		:	1;	//	11
	DWORD				m_bf1InsInstanceWaterFall		:	1;
	DWORD				m_bf5MiniPetType				:	5;
	DWORD				m_bf1IsDefaultMiniPetSkill		:	1;	//	18
	DWORD				m_bf5MiniPetSkillType			:	5;	//	23
	DWORD				m_bf1IsXMiniPetLevel			:	1;	//	24
	DWORD				m_bf4PetSkillDamageExpressionType	:	4;	//	28
	DWORD				m_bf1IsNormalMiniPetSkill		:	1;	//	29
	DWORD				m_bf1IsDuelCoolTimeSkill		:	1;	//	30
	DWORD				m_bf1IsShadowHideSkill			:	1;	//	31	도둑 그림자 숨�E스킬이냐?.
	DWORD				m_bf1IsApplyShadowHideEffect	:	1;	//	32	도둑 그림자 숨�E스킬효�E적�E	
	DWORD				m_bf7DuelServerpenaltyDiv		:	7;	//	듀얼서�E에서 페널티를 받는 기�E(나누엉幕 쓴다.)_ / 0 ~ 127 깩�E	
	DWORD				m_bf4PetSkillDamageExpressionTypeForAwaken	:	4;	//	11
	DWORD				m_bf6NeedState					:	6	;// 17		스킬을 사�E歐E위한 필요상태
	DWORD				m_bf1IsCancelNeedState			:	1	; // 18  필요상태해제
	DWORD				m_bf1IsReactionLimitCount		:	1	; //19 리액션발동수제한...
	DWORD				m_bf1IsIgnoreOptionAttackSpeed	:	1	;// 20	공격속도아이템옵션무시.
	DWORD				m_bf1IsIgnoreLucky				:	1	;	// 21 웝諾시.
	DWORD				m_bf1IsRushAttack				:	1	;	//	 22 돌�E엉疎...
	DWORD				m_bf1IsIgnoreDelayAfterAttack	:	1	;	//	 23 공격후 딜레이 무시
	DWORD				m_bf1IsApplyEffectChanceToPlayer	:	1	;	//	 24 플레이�E부가효�E확�E好E	
	DWORD				m_bf1IsApplyFightingSpirit			:	1	;	//	 25 투지영향받는 스킬
	DWORD				m_bf1IsCanNotApplySameEffect			:	1	;	//	 26 같은 부가효�E적�E안됨(일부 능력치 상승 버프만)
	DWORD				m_bf1IsTargetingToActorStickedBit		:	1	;	//	 27 비트붙은 타겟
	DWORD				m_bf4SpendBit							:	4	;	//	 31 소모비트
	DWORD				m_bf1IsApplyCriticalEffectWhenBeLightEffect :	1;	//	 32	빛속성�E청纘쩜鵑瓚灸� 크리티컬효과적�E	
	DWORD				:0;
	//JBC		미니�E3�E각성 각성의 씨앗 열매데미지를 계�E秊� 필요한 변수�E09-01-07
	WORD				m_wAwakenSkillBonusActiveChance;			//각성의 씨앗 보너스 발동 확�E과府).
	WORD				m_wAwakenSkillBonusActiveChancePerLevel;	//각성의 씨앗 보너스 발동 확�E레벨�E.
	
	short				m_wAwakenSkillBonusDamage;					//각성의 씨앗 보너스 데미�E과府).
	short				m_wAwakenSkillBonusDamagePerLevel;			//각성의 씨앗 보너스 데미�E레벨�E.
	short				m_wAwakenSkillBonusDamageForOrigin;			//각성의 씨앗 보너스 데미�E진화�E수식의 과府값)에 추가값.
	short				m_wAwakenSkillBonusDamagePerLevelForOrigin;	//각성의 씨앗 보너스 데미�E진화�E수식의 레벨�E값)에 추가값.
	short				m_wAwakenSkillBonusDamageValueRangeForOrigin;//각성의 씨앗 보너스 데미�E진화�E수식의 +-값)에 추가값.
	short				m_wAwakenSkillBonusDamageValueRangePerLevelForOrigin;//각성의 씨앗 보너스 데미�E진화�E수식의 레벨�E+-값 )에 추가값.
	
	//JBC		미니�E4�E각성 각성의 씨앗 열매강화에 적�E� 변수�E9-04-06
	short				m_wAwakenSkillBonusRange;					//각성의 씨앗 보너스 사정 거리(과府).
	short				m_wAwakenSkillBonusRangePerLevel;			//각성의 씨앗 보너스 사정 거리(레벨�E..
	
	short				m_wAwakenSkillBonusDamageForBoJoSkill;			//각성의 씨앗 보조스킬을 위한 보너스 데미�E과府).
	short				m_wAwakenSkillBonusDamageForBoJoSkillPerLevel;	//각성의 씨앗 보조스킬을 위한 보너스 데미�E레벨�E..
	
	short				m_wAwakenSkillBonusActiveChanceForBoJoSKill;					//각성의 씨앗 보조스킬에 붙일 보너스 발동 확�E과府).
	short				m_wAwakenSkillBonusActiveChanceForBoJoSKillPerLevel;			//각성의 씨앗 보조스킬에 붙일 보너스 발동 확�E레벨�E.
	
	WORD				m_wApplyEffectChanceToPlayer;
	WORD				m_wApplyEffectChancePerLevelToPlayer;
	WORD				m_wApplyEffectChanceLimitToPlayer;

	WORD				m_bf1IsApplyMinDamageWhenBeNotLightEffect			:1	;	// 빛�E청纘쩜鵑鑽염解� 아닐때 최소�E訣E적�E	
	WORD				m_bf1IsApplyExplosionWhenHitTargetStickedBit		:1	;	//2 비트부착중인 타겟 히트시 폭발적�E	
	WORD				m_bf1IsApplySameTarget								:1	;	//3 중복타겟적�E	
	WORD				m_bf11ArcHeight										:11	;	//14 포물선높이
	WORD				bf1IsAbleSkillWhenTransToWeapon						:1	;	//15 무기변신중에도사�E〈�
	WORD				:0;	

	short				m_sAttackPointPerActorLevel;
	short				m_sAddDamagePercent;

	WORD				m_bf5MagicDamageDiceCount						:	5;		//	5	마법�E訣E주사위굴림횟�E	
	WORD				m_bf5MagicDamageDiceCountLimit					:	5;		//	10	마법�E訣E주사위굴림횟수한�E	
	WORD				:0;
	short				m_sMagicDamageDiceCountPerLevel;							//	마법�E訣E주사위굴림per레벨	

	BYTE				m_abSpareSkillBuffer[12];

	char				m_strComment[dSKILL_COMMENT_LENGTH];	
	char				m_strPowerup[dSKILL_POWER_UP_COMMENT_LENGTH];
};


class	CAbilityDefine
{
public:
	WORD			m_wSkill;
	WORD			m_wLevel;
};

enum
{	// 필요상태.
	eDAMAGE_NORMAL,		// 일반
	eDAMAGE_MIN	,	// 최소�E訣E	
	eDAMAGE_MAX	,	// 최�EE訣E
};



class	cFighterAttackInfoE
{
public:
	WORD	m_wLevel1,m_wLevel2;
	WORD	m_wMinAttackPower,m_wMaxAttackPower;
	WORD	m_wAttackSpeed,m_wAttackRange;
};

class	cFighterAttackInfo
{
public:
	int						m_iCount;
	cFighterAttackInfoE		m_aData[100];

	cFighterAttackInfoE*	get(int _iLevel)
	{
		for (int i=0;i<m_iCount;i++)
			if (_iLevel >= m_aData[i].m_wLevel1 && _iLevel <= m_aData[i].m_wLevel2)
				return	&m_aData[i];

		return	NULL;
	}

	int		getMinAttackPower(int _iLevel)
	{
		return	get(_iLevel)->m_wMinAttackPower;
	}
	int		getMaxAttackPower(int _iLevel)
	{
		return	get(_iLevel)->m_wMaxAttackPower;
	}
	int		getAttackSpeed(int _iLevel)
	{
		return	get(_iLevel)->m_wAttackSpeed;
	}
	int		getAttackRange(int _iLevel)
	{
		return	get(_iLevel)->m_wAttackRange;
	}
};

extern	cFighterAttackInfo	g_fighterAttackInfo;

enum
{
	eSKILL_DATA_VERSION_FIRST,
	eSKILL_DATA_VERSION_ENCRYPT = eSKILL_DATA_VERSION_FIRST,
	eSKILL_DATA_VERSION_ENCRYPT2,
		
	eSKILL_DATA_VERSION_LAST,
	eSKILL_DATA_VERSION_CURRENT	=	eSKILL_DATA_VERSION_LAST-1,
};

#define	dSKILL_DATA_HEADER_SIGN	0xabcd1234

#endif