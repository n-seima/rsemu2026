/********************************************************************
	created:	2002/11/15
	created:	15:11:2002   18:51
	filename: 	C:\Work\Project Red Stone\Mangchi\cITEM_PREFIX.h
	file path:	C:\Work\Project Red Stone\Mangchi
	file base:	cITEM_PREFIX
	file ext:	h
	author:		Young-chan LEE
	
	purpose:	아이템 접두사
*********************************************************************/

#ifndef _classITEM_RARE_OPTION_
#define _classITEM_RARE_OPTION_

#include <windows.h>
#include "cITEM.H"

enum
{
eIP_ADD_STRENGTH						,	//	힘 증감
eIP_ADD_AGILITY							,	//	민첩성 증감
eIP_ADD_CONSTITUTION					,	//	건강 증감
eIP_ADD_WISDOM							,	//	지혜 증감
eIP_ADD_INTELIGENCE						,	//	지식 증감
eIP_ADD_CHARISMA						,	//	카리스마 증감
eIP_ADD_LUCK							,	//	운 증감
eIP_FIXING_POWER						,	//	힘 고정
eIP_FIXING_AGILITY						,	//	민첩성 고정
eIP_FIXING_CONSTITUTION					,	//	건강 고정
eIP_FIXING_WISDON						,	//	지혜 고정
eIP_FIXING_INTELIGENCE					,	//	지식 고정
eIP_FIXING_CHARISMA						,	//	카리스마 고정
eIP_FIXING_LUCK							,	//	운 고정
eIP_ADD_STRENGTH_BY_LEVEL				,	//	레벨 비례 힘 증감
eIP_ADD_AGILITY_BY_LEVEL				,	//	레벨 비례 민첩성 증감
eIP_ADD_CONSTITUTION_BY_LEVEL			,	//	레벨 비례 건강 증감
eIP_ADD_WISDOM_BY_LEVEL					,	//	레벨 비례 지혜 증감
eIP_ADD_INTELIGENCE_BY_LEVEL			,	//	레벨 비례 지식 증감
eIP_ADD_CHARISMA_BY_LEVEL				,	//	레벨 비례 카리스마 증감
eIP_ADD_LUCK_BY_LEVEL					,	//	레벨 비례 운 증감
eIP_ADD_DAMAGE_PERCENTAGE				,	//	무기 대미지 증감 (퍼센티지)
eIP_ADD_MIN_DAMAGE						,	//	무기 대미지 증감 (최소값)
eIP_ADD_MAX_DAMAGE						,	//	무기 대미지 증감 (최대값)
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
eIP_RETURN_DAMAGE						,	//	받은 대미지 일부 돌려 줌
eIP_DAMAGE_TO_CP						,	//	받은 대미지 CP 변환
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
};


#define	dMAX_ITEM_PREFIX_TYPE_COUNT			200
#define	dMAX_ITEM_PREFIX_COUNT				1024

class cITEM_PREFIX
{
public:
	WORD			m_wSerial;
	WORD			m_wDiscernmentCode;
	WORD			m_wType;
	short			m_aValue[2][2];//수치 1,2의 최대 최소치
	short			m_sValue3;//3번째 수치
	char			m_str1stPrefix[20],m_str2stPrefix[20];
	WORD			m_wDropLevel;
	WORD			m_wPrefixForm;//접두사 타입 0은 명사,1은 형용사

	DWORD			m_dwPrice;//가격
	WORD			m_wComputePriceMethod;	//	가격 계산 방법
	WORD			m_wPriceFactor;	//	곱할 것인가 더할 것인가

	BYTE			m_aEnableSet[4];	//	처음,두번째,세번째,덤-_-
	BYTE			m_aStickableItem[dITEM_KIND_COUNT];
};

extern	cITEM_PREFIX	g_aItemPrefix[dMAX_ITEM_PREFIX_COUNT];

#endif