#ifndef	_classACTOR_STATUS_H
#define	_classACTOR_STATUS_H

#include	"SFC.H"
#include	"cItem.H"
#include	"actorDefine.H"
#include	"CAbility.h"

#define	dCOLD_FACTOR							66

#define	dMIXIMUM_IMPROVE_WEAPON_DAMAGE_VALUE	255
#define	dMIXIMUM_IMPROVE_ARMOR_DEFENSE_VALUE	255

#define	dMAXIMUM_MAGIC_STRENGTHENING_VALUE				120	//	최대 마법 증폭
#define	dMAXIMUM_MAGIC_STRENGTHENING_VALUE_FOR_UNIQUE	300	//	최대 마법 증폭
#define	dMAXIMUM_MAGIC_WEAKNING_VALUE			120	//	최대 마법 약화

#define	dNON_TARGET_HIDNIG_LEVEL				50
#define	dBLUR_HIDNIG_LEVEL						150
#define	dINVISIBLE_HIDNIG_LEVEL					100

#define	dMAXIMUM_PLAYER_MAGIC_RESISTANCE_VALUE	90	//	최대 마법저항
#define	dMAXIMUM_MONSTER_MAGIC_RESISTANCE_VALUE	100	//	최대 마법저항
#define	dMAXIMUM_RESISTANCE_VALUE				200	//	최대 저항
#define	dMAXIMUM_ABSORB_VALUE					200
#define	dMAXIMUM_UNIQUE_ITEM_DROP_RATE			200
#define	dMAXIMUM_RARE_ITEM_DROP_RATE			200
#define	dMAXIMUM_RESURRECTION_CHANCE			100
#define	dMAXIMUM_REGEN_HP						20000
#define	dMAXIMUM_TRANSFORMATION_SPEED			200
#define	dMAXIMUM_FIRST_AID_RATE					200
#define	dMAXIMUM_RECOVER_SPEED_BY_POTION		255

#define	dMAXIMUM_RETURN_DAMAGE					200
#define	dMAXIMUM_DAMAGE_TO_CP_RATE				200
#define	dMAXIMUM_INSTANCE_KILL_CHANCE			200
#define	dMAXIMUM_MAKE_FLEE_CHANCE				200
#define	dMAXIMUM_KNOCKOUT_ATTACK_CHANCE			200
#define	dMAXIMUM_MAKE_BERSERK_CHANCE			200
#define	dMAXIMUM_MAKE_BREAK_ARMOR_CHANCE		200
#define	dMAXIMUM_MAKE_BREAK_WEAPON_CHANCE		200
#define	dMAXIMUM_MAKE_CONFUSE_CHANCE			200
#define	dMAXIMUM_MAKE_CHARM_CHANCE				200
#define	dMAXIMUM_MAKE_STUN_CHANCE				200
#define	dMAXIMUM_MAKE_COLD_CHANCE				200
#define	dMAXIMUM_MAKE_FREEZE_CHANCE				200
#define	dMAXIMUM_MAKE_BLIND_CHANCE				200
#define	dMAXIMUM_POISON_DAMAGE					255
#define	dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE		255
#define dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE_DX		20000		// 09.08.07  
#define	dMAXIMUM_CORRECT_VS_ENEMY_VALUE			300
#define	dMAXIMUM_CORRECT_RELOAD_SPEED			90

#define	dMAXIMUM_FIRST_AID_TIME					180
#define	dMAXIMUM_CONCENTRATION_VALUE			90

#define	dREDUCE_CP_RUNNING_FACTOR				10

#define	dMAXIMUM_BLOCKING_CHANCE				750		//	최고 블럭킹률
#define	dLIMIT_BLOCKING_CHANCE					850		//	최고 블럭킹률
#define	dMAXIMUM_CRITICAL_HIT_CHANCE			100		//	최고 크리티컬 보정치
#define	dMAXIMUM_CRUSH_BLOW_CHANCE				100		//	최고 크러쉬샷 보정치
#define	dMAXIMUM_LIFE_STEAL_PERCENTAGE			100		//	최대 라이프 스틸 수치
#define	dMAXIMUM_BONUS_CP						100		//	최대 라이프 스틸 수치

#define	dMAXIMUM_MAKE_STONE_CHANCE				200
#define	dMAXIMUM_MAKE_SLEEP_CHANCE				200
#define	dMAXIMUM_MAKE_FIRSTTARGET_CHANCE		200
#define	dMAXIMUM_MAKE_SIMPLTONE_CHANCE			200

#define	dDECLINE_HIT_CHANCE_VALUE				5		//	명중률저하 수치
#define	dDECLINE_DODGE_CHANCE_VALUE				5		//	회피율저하 수치

#define	dEARTH_RESISTANCE_FOR_LEVITATE			25
#define	dCONCENTRATION_FOR_LEVITATE				5

enum
{
	eBLIND_RANGE_NULL			= 0,
	eBLIND_RANGE_VERY_SMALL			,
	eBLIND_RANGE_SMALL				,
	eBLIND_RANGE_MIDDLE				,
	eBLIND_RANGE_LARGE				,
	eBLIND_RANGE_VERY_LARGE			,
};
enum			//09.08.10
{
	eADD_FIRE_DAMAGE_DX,
	eADD_WATER_DAMAGE_DX,
	eADD_WIND_DAMAGE_DX,
	eADD_LIGHT_DAMAGE_DX,
	eADD_DARK_DAMAGE_DX,
	eADD_EARTH_DAMAGE_DX,
	eADD_DAMAGE_DX_END,

};

struct sADD_NATURAL_DAMAGE_DX	//09.08.10
{
	WORD	wMin;
	WORD	wMax;
};
#define	dCURSE_AMPLIFICATION_FACTOR				200

class	cACTOR_STATUS 
{
public:

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃특수 능력
	WORD				m_wSealActionTime,m_wUnsealAction,m_wSealRunningTime,m_wBeastBerserkerTime;
	WORD				m_wInvincibleTime;				//	무적시간
	short				m_sInvisivilityTime;			//	인비저빌러티시간(-이면 장비에 붙어 있는거다.)
	BYTE				m_bIsHideByShadowHideSkill;		//	그림자 숨기로 인비져블을 걸었다.
	short				m_sShadowHideAddCriticalChance;	//	그림자숨기상태일때 크리티컬추가찬스!
	WORD				m_wShadowHideAddPhysicalPowerPercent;	//	그림자숨기상태효과 공격력추가!
	short				m_sFloatTime;					//	공중부양 시간(-이면 장비에 붙어 있는거다.)
	short				m_sBlurTime;					//	흐릿한 상태 시간(-이면 장비에 붙어 있는거다.)
	cACTOR_FACTOR_TYPE1	m_detecting;					//	디텍팅
	short				m_sNonTargetTime;				//	타겟에서 제외됨(-1이면 장비에 붙어 있는 것임)
	WORD				m_wHidingLevel;
	WORD				m_wFoundOutChanceInInvisible;
	WORD				m_wFoundOutChanceInInvisibleForStand;

	WORD				m_wReversalPowerTime;			//	지식 <-> 힘, 지혜 <-> 민첩성 변환 시간
	WORD				m_wMaintainMaxCPTime;			//	이 시간동안 CP가 max로 유지된다.
	WORD				m_wMaintainMaxWeponDamageTime;	//	이 시간동안 무기의 데미지가 항상 max가 나온다.
	int					m_iFirstAidHP;					//	응급처치한 HP
	WORD				m_wFirstAidTime;				//	응급처치한 시간

	BYTE				m_bResurrectionChance;			//	죽었을때 일정 확률로 부활한다.
	BYTE				m_bIsInfinityBullet;				//	무한 탄환
	BYTE				m_isAutoReload;					//	자동으로 벨트의 아이템을 리로드 한다.
	BYTE				m_isMagicBullet;					//	마법 탄환을 쏠 수 있는 상태
	BYTE				m_isIgnoreTargetCorrectDodgeChance;	//	적의 회피 보정치 무시
	BYTE				m_isIgnoreTargetCorrectHitChance;	//	적의 명중 보정치 무시
	BYTE				m_isPerfectHit;					//	항상 명중
	BYTE				m_bGetBonusCPPercentage;		//	CP획득시 보너스..
	WORD				m_wIncreaseSitdownHPRecoverSpeed;
	WORD				m_wJumppingSkippingTime;
	WORD				m_wRabbitTime;

	cACTOR_FACTOR_TYPE1	m_regenHPPerSecond;				//	매초 체력을 회복
	cACTOR_FACTOR_TYPE1	m_bonusExp;						//	경험치 보너스
	
	cACTOR_FACTOR_TYPE1	m_bonusItemDropChance;						//	경험치 보너스
	cACTOR_FACTOR_TYPE1	m_correctLastDamage;						//	경험치 보너스

	cACTOR_FACTOR_TYPE1	m_aCorrectLevel[5];
	cACTOR_FACTOR_TYPE1	m_correctSummonBeastState;
	cACTOR_FACTOR_TYPE1	m_bonusPetExp;

	cACTOR_FACTOR_TYPE6	m_detectTrap;
	cACTOR_FACTOR_TYPE6	m_detectSecretObject;

	BYTE				m_ImmuneOneTime;				//한대만 이뮨.
	inline void			SetImmuneOneTime()		{m_ImmuneOneTime = TRUE;}
//┃특수 능력
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃능력치
	int					m_iWeaponMinDamage;				//	무기 최소 데미지
	int					m_iWeaponMaxDamage;				//	무기 최대 데미지
	
	WORD				m_wCorrectFinalDefensivePowerPercentageTime;
	short				m_sCorrectFinalDefensivePowerPercentage;

	WORD				m_wCorrectFinalAttackPowerPercentageTime;			// 최종 공격력 감소 시간.
	short				m_sCorrectFinalAttackPowerPercentage;				// 최종 공격력 감소
	
	
	cACTOR_FACTOR_TYPE3	m_attackPower;					//	공격력
	cACTOR_FACTOR_TYPE4	m_defensePower;					//	방어력

	cACTOR_FACTOR_TYPE1	m_level;
	cACTOR_FACTOR_TYPE2	m_maxHP;						//	최대 HP
	cACTOR_FACTOR_TYPE2	m_maxCP;						//	최대 CP

	cACTOR_FACTOR_TYPE2	m_weaponPower;
	cACTOR_FACTOR_TYPE1	m_armorPower;

	cACTOR_FACTOR_TYPE1	m_strength;						//	힘 
	cACTOR_FACTOR_TYPE1	m_agility;						//	민첩성
	cACTOR_FACTOR_TYPE1	m_constitution;					//	건강
	cACTOR_FACTOR_TYPE1	m_intelligence;					//	지식
	cACTOR_FACTOR_TYPE1	m_wisdom;						//	지혜
	cACTOR_FACTOR_TYPE1	m_charisma;						//	카리스마
	cACTOR_FACTOR_TYPE1	m_luck;							//	운

	cACTOR_FACTOR_TYPE1	m_actionSpeed;					//	액션 속도 보정
	cACTOR_FACTOR_TYPE1	m_moveSpeed;					//	이동 속도 보정

	cACTOR_FACTOR_TYPE1	m_hitChance;					//	명중률 보정
	cACTOR_FACTOR_TYPE1	m_dodgeChance;					//	회피 보정
	cACTOR_FACTOR_TYPE1	m_concentration;
	cACTOR_FACTOR_TYPE6	m_concentrationByStellarPeer;
	cACTOR_FACTOR_TYPE1	m_allignment;

	WORD				m_wBottomCP;					//	한계 CP 뛰기,(정지/걷기/앉기)등 CP가 떨어지는 액션에서도 CP가 떨어지지 않는다.
	WORD				m_wGGGDebufTimeByWaterGGG;
//┃능력치
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃능력치 저하
	WORD				m_wReduceAllResistanceToHalfTime;
//┃능력치 저하
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃상태 이상
	WORD				m_wBlindTime;						//	장님 상태 남은 시간
	WORD				m_wBlindByEvent;					//	장님 상태(이벤트라 해재가 안되는 상태 카르마로 해재해 주어야 한다.)
	WORD				m_wPoisonDamageCaster;
	short				m_wPoisonTime;						//	초당 데미지/10 , 중독 상태 남은 시간
	int					m_iPoisonDamagePerSec;
	WORD				m_wDarkDamageBySleepCaster;
	WORD				m_wDarkDamageBySleep;
	WORD				m_wSleepTime;						//	잠이 깰때까지 남은 시간
	WORD				m_wFreezeTime;						//	프리즈 상태 남은 시간
	WORD				m_wColdTime;						//	콜드 상태 남은 시간
	WORD				m_wStunTime;						//	스턴 상태 남은 시간
	WORD				m_wStoneTime;						//	석화 남은 시간
	WORD				m_wConfuseTime;						//	혼란 상태 남은 시간
	WORD				m_wConfuseCaster;
	DWORD				m_dwConfuseCasterNameHashCode;
	WORD				m_wCharmedTime;						//	차밍 상태 남은 시간
	WORD				m_wNightMareTime;					//	악몽
	WORD				m_wParalysisTime;					//	전신 마비
//┃상태 이상
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃저주
	WORD				m_wFalloffMoraleTime;			//	사기 저하 상태 남은 시간
	WORD				m_wTargetedTime;				//	타겟 설정 상태 남은 시간
	WORD				m_wBreakedArmorTime;			//	갑옷 파괴 상태 남은 시간
	WORD				m_wBreakedWeaponTime;			//	무기 파괴 상태 남은 시간
	WORD				m_wSimpletonTime;				//	바보-_- 상태 남은 시간
	WORD				m_wBerserkTime;					//	버서크 상태 남은 시간
	WORD				m_wTortureDarkDamage,m_wTortureTime;	//	고문률-_-??,고문 시간
	WORD				m_wBloodDonorRate,m_wBloodDonorTime;	//	피흡혈률,피흡혈 당하는 시간
//┃저주
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃공격이 성공하면 부가적으로 들어가는 효과
	BYTE				m_bInstanceKillChance;			//	즉사 시킬 확률

	BYTE				m_bTargetFleeChance;			//	대상을 도망가게 만들 확률
	BYTE				m_bTargetBerserkChance;			//	대상을 버서크로 만들 확률
	BYTE				m_bTargetArmorBreakChance;		//	대상을 갑옷 파괴 상태로 만들 확률
	BYTE				m_bTargetWeaponBreakChance;		//	대상을 무기 파괴 상태로 만들 확률
	BYTE				m_bTargetConfuseChance;			//	대상을 혼란 시킨다.
	BYTE				m_bTargetCharmingChance;		//	대상을 유혹한다.
	BYTE				m_bTargetStunChance;			//	대상을 경직 시킨다.
	BYTE				m_bTargetColdChance;			//	대상을 콜드 상태로 만든다.
	BYTE				m_bTargetFreezeChance;			//	대상을 얼린다.
	BYTE				m_bTargetBlindChance;			//	대상을 장님으로 만들 확률
	BYTE				m_bTargetStoneChance;			//	대상을 돌로 만들 확률
	BYTE				m_bTargetSleepChance;			//	대상을 재울 확률
	BYTE				m_bTargetFirstTargetChance;		//	대상을 퍼스트 타겟으로 만들 확률
	BYTE				m_bTargetSimpltoneChance;		//	대상을 AI저하 상태로 만들 확률

	BYTE				m_bAddPoisonMinDamage,m_bAddPoisonMaxDamage;	//	추가로 대지 데미지를 입힌다.

	WORD				m_wAddDamageToUndead;			//	언데드에게 추가 데미지를 준다.
	WORD				m_wAddDamageToDemon;			//	악마에게 추가 데미지를 준다.
	WORD				m_wAddDamageToHuman;			//	인간에게 추가 데미지를 준다.
	WORD				m_wAddDamageToAnimal;			//	동물에게 추가 데미지를 준다.
	WORD				m_wAddDamageToHolyAnimal;		//	신수에게 추가 데미지를 준다.

	WORD				m_wTargetBerserkTime;			//	대상을 버서크로 만드는 시간
	WORD				m_wTargetArmorBreakTime;		//	대상을 갑옷 파괴 상태로 만들 확률
	WORD				m_wTargetWeaponBreakTime;		//	대상을 무기 파괴 상태로 만들 확률
	WORD				m_wTargetConfuseTime;			//	대상을 혼란 시킨다.
	WORD				m_wTargetCharmingTime;			//	대상을 유혹한다.
	WORD				m_wTargetStunTime;				//	대상을 경직 시킨다.
	WORD				m_wTargetColdTime;				//	대상을 콜드 상태로 만든다.
	WORD				m_wTargetFreezeTime;			//	대상을 얼린다.
	WORD				m_wTargetBlindTime;				//	대상을 중독 시킨다.
	WORD				m_wTargetStoneTime;				//	대상을 돌로 만들 확률
	WORD				m_wTargetSleepTime;				//	대상을 재울 확률
	WORD				m_wTargetFirstTargetTime;		//	대상을 퍼스트 타겟으로 만들 확률
	WORD				m_wTargetSimpltoneTime;			//	대상을 AI저하 상태로 만들 확률

	WORD				m_wTargetDeclineHitChanceTime;	//	대상을 명중률 저하 상태에 빠지게 한다.
	WORD				m_wTargetDeclineDodgeChanceTime;//	대상을 회피율 저하 상태에 빠지게 한다.
	WORD				m_wTargetCurseAmplificationTime;//	대상을 회피율 저주 증폭 상태에 빠지게 한다.

	WORD				m_wTargetPoisonedTime;			//	대상을 중독 시킨다.
	WORD				m_wLifeStealPecentage;			//	흡혈률

	cACTOR_FACTOR_TYPE5	m_addFireDamage;
	cACTOR_FACTOR_TYPE5	m_addWaterDamage;
	cACTOR_FACTOR_TYPE5	m_addWindDamage;
	cACTOR_FACTOR_TYPE5	m_addLightDamage;
	cACTOR_FACTOR_TYPE5	m_addDarkDamage;
	cACTOR_FACTOR_TYPE5	m_addEarthDamage;

	WORD				m_wAddFireMinDamage,m_wAddFireMaxDamage;	//	추가로 불꽅 데미지를 입힌다.
	WORD				m_wAddWaterMinDamage,m_wAddWaterMaxDamage;	//	추가로 물 데미지를 입힌다.
	WORD				m_wAddWindMinDamage,m_wAddWindMaxDamage;	//	추가로 바람 데미지를 입힌다.
	WORD				m_wAddLightMinDamage,m_wAddLightMaxDamage;	//	추가로 빛 데미지를 입힌다.
	WORD				m_wAddDarkMinDamage,m_wAddDarkMaxDamage;	//	추가로 어둠 데미지를 입힌다.
	WORD				m_wAddEarthDamage;	//	추가로 어둠 데미지를 입힌다.

	// VV 09.08.07 마법옵션 추가데미지  DX!!

	void				addAddNaturalDamageDX(int _iClass, int _iDamage);
	void				addAddNaturalDamageDX(int _iClass, int _iMinDamage, int _iMaxDamage);

	// AA 09.08.07 마법옵션 추가데미지  DX!!

	//vvv 09.08.10   마법추뎀 DX	 수정.
	sADD_NATURAL_DAMAGE_DX	m_sAddNaturalDamageDX[eADD_DAMAGE_DX_END];	// 추가.

	inline	int			getAddFireMinDamage()	{		return	m_sAddNaturalDamageDX[eADD_FIRE_DAMAGE_DX].wMin+m_wAddFireMinDamage+m_addFireDamage.m_sCorrectValue;	}
	inline	int			getAddWaterMinDamage()	{		return	m_sAddNaturalDamageDX[eADD_WATER_DAMAGE_DX].wMin+m_wAddWaterMinDamage+m_addWaterDamage.m_sCorrectValue;	}
	inline	int			getAddWindMinDamage()	{		return	m_sAddNaturalDamageDX[eADD_WIND_DAMAGE_DX].wMin+m_wAddWindMinDamage+m_addWindDamage.m_sCorrectValue;	}
	inline	int			getAddEarthMinDamage()	{		return	m_sAddNaturalDamageDX[eADD_EARTH_DAMAGE_DX].wMin+m_wAddEarthDamage+m_addEarthDamage.m_sCorrectValue;	}
	inline	int			getAddDarkMinDamage()	{		return	m_sAddNaturalDamageDX[eADD_DARK_DAMAGE_DX].wMin+m_wAddDarkMinDamage+m_addDarkDamage.m_sCorrectValue;	}
	inline	int			getAddLightMinDamage()	{		return	m_sAddNaturalDamageDX[eADD_LIGHT_DAMAGE_DX].wMin+m_wAddLightMinDamage+m_addLightDamage.m_sCorrectValue;	}

	inline	int			getAddFireMaxDamage()	{		return	m_sAddNaturalDamageDX[eADD_FIRE_DAMAGE_DX].wMax+m_wAddFireMaxDamage+m_addFireDamage.m_sCorrectValue+m_addFireDamage.m_sRange;	}
	inline	int			getAddWaterMaxDamage()	{		return	m_sAddNaturalDamageDX[eADD_WATER_DAMAGE_DX].wMax+m_wAddWaterMaxDamage+m_addWaterDamage.m_sCorrectValue+m_addWaterDamage.m_sRange;	}
	inline	int			getAddWindMaxDamage()	{		return	m_sAddNaturalDamageDX[eADD_WIND_DAMAGE_DX].wMax+m_wAddWindMaxDamage+m_addWindDamage.m_sCorrectValue+m_addWindDamage.m_sRange;	}
	inline	int			getAddEarthMaxDamage()	{		return	m_sAddNaturalDamageDX[eADD_EARTH_DAMAGE_DX].wMax+m_wAddEarthDamage+m_addEarthDamage.m_sCorrectValue+m_addEarthDamage.m_sRange;	}
	inline	int			getAddDarkMaxDamage()	{		return	m_sAddNaturalDamageDX[eADD_DARK_DAMAGE_DX].wMax+m_wAddDarkMaxDamage+m_addDarkDamage.m_sCorrectValue+m_addDarkDamage.m_sRange;	}
	inline	int			getAddLightMaxDamage()	{		return	m_sAddNaturalDamageDX[eADD_LIGHT_DAMAGE_DX].wMax+m_wAddLightMaxDamage+m_addLightDamage.m_sCorrectValue+m_addLightDamage.m_sRange;	}
	//AAA 09.08.10   마법추뎀 DX

//┃공격이 성공하면 부가적으로 들어가는 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃데미지를 체력으로 흡수하거나 카운터 날림
	BYTE				m_bFirstAidPercentage;			//	입은 데미지의 일부를 치료한다.

	BYTE				m_bAbsorbFireDamage;			//	불꽃 데미지를 흡수해서 체력으로 변환
	BYTE				m_bAbsorbWaterDamage;			//	물 데미지를 흡수해서 체력으로 변환
	BYTE				m_bAbsorbWindDamage;			//	바람 데미지를 흡수해서 체력으로 변환
	BYTE				m_bAbsorbEarthDamage;			//	대지 데미지를 흡수해서 체력으로 변환
	BYTE				m_bAbsorbLightDamage;			//	빛 데미지를 흡수해서 체력으로 변환
	BYTE				m_bAbsorbDarkDamage;			//	어둠 데미지를 흡수해서 체력으로 변환
	BYTE				m_bAbsorbMagicDamage;			//	모든 마법 데미지를 흡수해서 체력으로 변환

	cACTOR_FACTOR_TYPE1	m_returnDamageRate;				//	데미지의 일부를 공격자에게 돌려준다.
	cACTOR_FACTOR_TYPE1	m_returnDamageRateIDEF;			//	데미지의 일부를 공격자에게 돌려준다.
	cACTOR_FACTOR_TYPE6	m_returnDamageByRootRate;		//	데미지의 일부를 공격자에게 돌려준다.
	BYTE				m_bDamageToCPRate;				//	데미지의 일부를 CP로 변환해 준다.
	BYTE				m_bReturnFreezeChance;			//	리턴 프리즈 확률
	BYTE				m_bReturnColdChance;			//	리턴 콜드 확률

	WORD				m_wReturnFreezeTime;			//	반격!! 프리즈가 성공 했을때 프리즈 되는 시간.
	WORD				m_wReturnColdTime;				//	반격!! 콜드가 성공 했을때 프리즈 되는 시간.
//┃데미지를 체력으로 흡수하거나 카운터 날림
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 수치 보정
	cACTOR_FACTOR_TYPE1	m_correctAllSkillLevel;
	cACTOR_FACTOR_TYPE6	m_correctRightPunchSkillLevel;
	cACTOR_FACTOR_TYPE6	m_correctLeftPunchSkillLevel;
	cACTOR_FACTOR_TYPE6	m_correctRightKickSkillLevel;
	cACTOR_FACTOR_TYPE6	m_correctLeftKickSkillLevel;
	cACTOR_FACTOR_TYPE6	m_correctDownKickSkillLevel;

	int					m_iCorrectRecoverBlocking;		//	블럭킹 회복 시간 보정
	BYTE				m_bCorrectRareItemDropRate;		//	레어 아이템이 떨어질 확률을 보정한다.
	BYTE				m_bCorrectUniqueItemDropRate;	//	유니크 아이템이 떨어질 확률을 보정한다.
	BYTE				m_bCorrectMagicItemDropChance;
	cACTOR_FACTOR_TYPE1	m_correctCriticalHitChance;	//	크리티컬 히트 날릴 기회를 보정한다.
	BYTE				m_bCorrectCrushBlowChance;		//	크러쉬 블로우 날릴 기회를 보정한다.
	BYTE				m_bCorrectKnockoutAttackChance;	//	녹아웃 공격을 시킬 기회를 보정한다.
	BYTE				m_bCorrectReloadSpeed;			//	벨트 채우는 속도 보정
	BYTE				m_bCorrectTransformationSpeed;	//	변신 속도 보정
	BYTE				m_bCorrectHPRecoverSpeed;		//	체력 회복 속도 보정
	cACTOR_FACTOR_TYPE1	m_blockingChance;
	
	cACTOR_FACTOR_TYPE1	m_weakningTargetFireResistance;
	cACTOR_FACTOR_TYPE1	m_weakningTargetWaterResistance;
	cACTOR_FACTOR_TYPE1	m_weakningTargetWindResistance;
	cACTOR_FACTOR_TYPE1	m_weakningTargetEarthResistance;
	cACTOR_FACTOR_TYPE1	m_weakningTargetLightResistance;
	cACTOR_FACTOR_TYPE1	m_weakningTargetDarkResistance;
	cACTOR_FACTOR_TYPE1	m_weakningTargetMagicResistance;

	cACTOR_FACTOR_TYPE1	m_strengtheningFireDamage;
	cACTOR_FACTOR_TYPE1	m_strengtheningWaterDamage;
	cACTOR_FACTOR_TYPE1	m_strengtheningWindDamage;
	cACTOR_FACTOR_TYPE1	m_strengtheningEarthDamage;
	cACTOR_FACTOR_TYPE1	m_strengtheningDarkDamage;
	cACTOR_FACTOR_TYPE1	m_strengtheningLightDamage;
	cACTOR_FACTOR_TYPE1	m_strengtheningMagicDamage;
	cACTOR_FACTOR_TYPE2 m_strengtheningCriticalDamage;	// 치명타대미지증가.
	cACTOR_FACTOR_TYPE1	m_correcthardBlowHitChance;	//	강타확률 
//┃각종 수치 보정
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 저항
	BYTE				m_bFreezeResistance;		//	상태 이상 저항
	BYTE				m_bColdResistance;			//	상태 이상 저항
	BYTE				m_bBlindResistance;			//	상태 이상 저항
	BYTE				m_bConfuseResistance;		//	상태 이상 저항
	cACTOR_FACTOR_TYPE1	m_charmingResistance;
	BYTE				m_bStoneResistance;			//	상태 이상 저항
	BYTE				m_bPoisonResistance;		//	상태 이상 저항
	BYTE				m_bSleepResistance;			//	상태 이상 저항

	BYTE				m_bCriticalHitResistance;			//	크리티컬 저항
	BYTE				m_bCrushBlowResistance;				//	크러쉬 블로우 저항
	BYTE				m_bKnockBackResistance;			//	녹아웃 저항
	BYTE				m_bInstanceKillResistance;		//	즉사 저항
	BYTE				m_bCorrectGoldDropChance;

	BYTE				m_bCorrectFieldFirePenelty;
	BYTE				m_bCorrectFieldWaterPenelty;
	BYTE				m_bCorrectFieldWindPenelty;
	BYTE				m_bCorrectFieldEarthPenelty;
	BYTE				m_bCorrectFieldLightPenelty;
	BYTE				m_bCorrectFieldDarkPenelty;

	cACTOR_FACTOR_TYPE1	m_stunResistance;
	cACTOR_FACTOR_TYPE1	m_strangeStatusResistance;
	cACTOR_FACTOR_TYPE1	m_curseResistance;
	cACTOR_FACTOR_TYPE1	m_declinePowerResistance;

	cACTOR_FACTOR_TYPE1	m_fireResistance;				//	불꽃 저항
	cACTOR_FACTOR_TYPE1	m_waterResistance;				//	물 저항
	cACTOR_FACTOR_TYPE1	m_windResistance;				//	바람 저항
	cACTOR_FACTOR_TYPE1	m_earthResistance;				//	대지 저항
	cACTOR_FACTOR_TYPE1	m_lightResistance;				//	빛 저항
	cACTOR_FACTOR_TYPE1	m_darkResistance;				//	어둠 저항
	cACTOR_FACTOR_TYPE1	m_allMagicResistance;			//	모든 마법 저항
//┃보정된 마법 저항
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기타
	WORD				m_wIncreaseFinalPhysicalAttackPower;
	WORD				m_wResurrectionCount;			//	부활 카운트
	WORD				m_wCurseAmplificationTime;		//	저주 증폭 상태
	int					m_iRecoverHP;					//	회복될 HP
	WORD				m_wBeltStackCount;				//	벨트에 들어가는 아이템 수
	int					m_iCorrectMinimumDamage,m_iCorrectMaximumDamage;
	
	WORD				m_wMinAddPoisonDamage,m_wMaxAddPoisonDamage;
	WORD				m_wAddPoisonDamageOptionCount;

	BYTE				m_abCorrectMiniPetBasicSkillLevel[2];
	BYTE				m_abCorrectMiniPetSkillLevel[2];
	BYTE				m_abCorrectMiniPetSkillActiveChance[2];
	BYTE				m_abCorrectMiniPetSkillDamage[2];
	
	BYTE				m_bIncreaseMiniPetFireDamage	;
	BYTE				m_bIncreaseMiniPetWaterDamage	;
	BYTE				m_bIncreaseMiniPetWindDamage	;
	BYTE				m_bIncreaseMiniPetEarthDamage	;
	BYTE				m_bIncreaseMiniPetLightDamage	;
	BYTE				m_bIncreaseMiniPetDarkDamage	;
	BYTE				m_bIncreaseMiniPetMagicDamage	;
	BYTE				m_bIncreaseMiniPetSkillActiveChance	;
	BYTE				m_bIncreaseMiniPetBasicSkillLevel	;
	BYTE				m_bIncreaseMiniPetSkillLevel	;
	BYTE				m_bIncreaseMiniPetMagicDamageVsBoss	;

	WORD				m_wCarvingOptionShieldChance;
	WORD				m_wCarvingOptionShieldValue;

	WORD				m_wCarvingOptionChargeTime;
	WORD				m_wCarvingOptionChargeValue;

	WORD				m_wCarvingOptionIncreaseExp;

	WORD				m_wCarvingOptionAddFireDamageChance;
	WORD				m_iCarvingOptionAddFireDamage;
	WORD				m_wCarvingOptionSoulGuardTime;
	int					m_iCarvingOptionMaxHP;
	int					m_iLevelGetingRedStone;
	WORD				m_wItemEffect;
//┃기타
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	inline	void		addAbsorbFireDamage(int _iValue)		{m_bAbsorbFireDamage	=	min(m_bAbsorbFireDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	불 속성 공격 흡수
	inline	void		addAbsorbWaterDamage(int _iValue)		{m_bAbsorbWaterDamage	=	min(m_bAbsorbWaterDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	물 속성 공격 흡수
	inline	void		addAbsorbWindDamage(int _iValue)		{m_bAbsorbWindDamage	=	min(m_bAbsorbWindDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	바람 속성 공격 흡수
	inline	void		addAbsorbEarthDamage(int _iValue)		{m_bAbsorbEarthDamage	=	min(m_bAbsorbEarthDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	대지 속성 공격 흡수
	inline	void		addAbsorbLightDamage(int _iValue)		{m_bAbsorbLightDamage	=	min(m_bAbsorbLightDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	빛 속성 공격 흡수
	inline	void		addAbsorbDarkDamage(int _iValue)		{m_bAbsorbDarkDamage	=	min(m_bAbsorbDarkDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	어둠 속성 공격 흡수
	inline	void		addAbsorbMagicDamage(int _iValue)		{m_bAbsorbMagicDamage	=	min(m_bAbsorbMagicDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	모든 마법 속성 공격 흡수

	inline	void		addFreezeResistance(int _iValue)		{m_bFreezeResistance	=	min(m_bFreezeResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	프리즈 공격에 대한 저항
	inline	void		addColdResistance(int _iValue)			{m_bColdResistance		=	min(m_bColdResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	콜드 공격에 대한 저항
	inline	void		addStunResistance(int _iValue)			{m_stunResistance.addValue(_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	스턴 공격에 대한 저항
	inline	void		addConfuseResistance(int _iValue)		{m_bConfuseResistance	=	min(m_bConfuseResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	혼란 공격에 대한 저항
	inline	void		addCharmingResistance(int _iValue)		{m_charmingResistance.addValue(_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	차밍 공격에 대한 저항
	inline	void		addStoneResistance(int _iValue)			{m_bStoneResistance		=	min(m_bStoneResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	석화 공격에 대한 저항
	inline	void		addInstanceKillResistance(int _iValue)	{m_bInstanceKillResistance	=	min(m_bInstanceKillResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	즉사 공격에 대한 저항
	inline	void		addPoisonResistance(int _iValue)		{m_bPoisonResistance	=	min(m_bPoisonResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	중독 공격에 대한 저항
	inline	void		addSleepResistance(int _iValue)			{m_bSleepResistance		=	min(m_bSleepResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	잠 공격에 대한 저항
	inline	void		addCurseResistance(int _iValue)			{m_curseResistance.m_sValue			=	min(m_curseResistance.m_sValue+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//"저주 상태에 대한 저항"
	inline	void		addDeclinePowerResistance(int _iValue)	{m_declinePowerResistance.m_sValue	=	min(m_declinePowerResistance.m_sValue+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//"저하 상태에 대한 저항"
	inline	void		addStrangeStatusResistance(int _iValue)	{m_strangeStatusResistance.m_sValue	=	min(m_strangeStatusResistance.m_sValue+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//"이상 상태에 대한 저항"
	inline	void		addAllBadStatusResistance(int _iValue)	
	{	
		addCurseResistance(_iValue);
		addDeclinePowerResistance(_iValue);
		addStrangeStatusResistance(_iValue);
	}
	inline	void		addKnockoutResistance(int _iValue)		{m_bKnockBackResistance	=	min(m_bKnockBackResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	녹백/넘어짐 공격에 대한 저항
	inline	void		addCriticalHitResistance(int _iValue)	{m_bCriticalHitResistance	=	min(m_bCriticalHitResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	치명타 공격에 대한 저항
	inline	void		addCrushBlowResistance(int _iValue)		{m_bCrushBlowResistance	=	min(m_bCrushBlowResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	결정타 공격에 대한 저항

	inline	void		addReturnDamageRate(int _iValue)		{m_returnDamageRate.addValue(_iValue,dMAXIMUM_RETURN_DAMAGE);}	//	받은 데미지 일부 돌려 줌
	inline	void		addReturnDamageRateIDEF(int _iValue)	{m_returnDamageRateIDEF.addValue(_iValue,dMAX_MAGICAL_DAMAGE);}	//	받은 데미지 일부 돌려 줌(수치로 방어력 무시고하고..)
	inline	void		addDamageToCPRate(int _iValue)			{m_bDamageToCPRate		=	min(m_bDamageToCPRate+_iValue,dMAXIMUM_DAMAGE_TO_CP_RATE);}//	받은 데미지 CP 변환

	inline	void		addBlockingChance(int _iValue)				
	{
		m_blockingChance.m_sValue	+=	_iValue;
		m_blockingChance.m_sValue	=	min(m_blockingChance.m_sValue,dMAXIMUM_BLOCKING_CHANCE);
	}
	inline	void		addFirstAidPercentage(int _iValue)			{m_bFirstAidPercentage=min(m_bFirstAidPercentage+_iValue,dMAXIMUM_FIRST_AID_RATE);}//	타격 응급 처치
	inline	void		addRegenHPPerSecond(int _iValue)			{m_regenHPPerSecond.m_sValue=min(m_regenHPPerSecond.m_sValue+_iValue,dMAXIMUM_REGEN_HP);}//	체력 리제네레이션
	inline	void		addResurrectionChance(int _iValue)			{m_bResurrectionChance=min(m_bResurrectionChance+_iValue,dMAXIMUM_RESURRECTION_CHANCE);}//	부활 확률

	inline	void		addCorrectCriticalHitChance(int _iValue)	{m_correctCriticalHitChance.addValue(_iValue,dMAXIMUM_CRITICAL_HIT_CHANCE);};//	치명타 발동 확률
	inline	void		addCorrectCrushBlowChance(int _iValue)		{m_bCorrectCrushBlowChance=min(m_bCorrectCrushBlowChance+_iValue,dMAXIMUM_CRUSH_BLOW_CHANCE);};//	치명타 발동 확률//	결정타 발동 확률
	inline	void		addCorrectHPRecoverSpeed(int _iValue)		{m_bCorrectHPRecoverSpeed=min(m_bCorrectHPRecoverSpeed+_iValue,dMAXIMUM_RECOVER_SPEED_BY_POTION);}//	물약 회복 속도 조절
	inline	void		addCorrectKnockoutAttackChance(int _iValue)	{m_bCorrectKnockoutAttackChance=min(m_bCorrectKnockoutAttackChance+_iValue,dMAXIMUM_KNOCKOUT_ATTACK_CHANCE);}//	녹아웃
	inline	void		addCorrectTransformationSpeed(int _iValue)	{m_bCorrectTransformationSpeed=min(m_bCorrectTransformationSpeed+_iValue,dMAXIMUM_TRANSFORMATION_SPEED);}//	무기 교환 속도
	inline	void		addCorrectRareItemDropRate(int _iValue)		{m_bCorrectRareItemDropRate=min(m_bCorrectRareItemDropRate+_iValue,dMAXIMUM_RARE_ITEM_DROP_RATE);}//	마법 아이템 확률 증가
	inline	void		addCorrectUniqueItemDropRate(int _iValue)	{m_bCorrectUniqueItemDropRate=min(m_bCorrectUniqueItemDropRate+_iValue,dMAXIMUM_UNIQUE_ITEM_DROP_RATE);}//	유니크 아이템 확률 증가
	inline	void		addCorrectReloadSpeed(int _iValue)			{m_bCorrectReloadSpeed=min(m_bCorrectReloadSpeed+_iValue,dMAXIMUM_CORRECT_RELOAD_SPEED);}//	"벨트에 아이템 채우는 시간 감소"

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃공격후 이어지는 부가 공격 효과 설정
	inline	void		addLifeStealPecentage(int _iValue)			{m_wLifeStealPecentage=min(m_wLifeStealPecentage+_iValue,dMAXIMUM_LIFE_STEAL_PERCENTAGE);}//	체력 스틸
	inline	void		addGetBonusCPPercentage(int _iValue)		{m_bGetBonusCPPercentage=min(m_bGetBonusCPPercentage+_iValue,dMAXIMUM_BONUS_CP);}//	보너스 CP
	inline	void		addTargetFleeChance(int _iValue)			{m_bTargetFleeChance=min(m_bTargetFleeChance+_iValue,dMAXIMUM_MAKE_FLEE_CHANCE);}//	도망
	inline	void		addInstanceKillChance(int _iChance)			
	{
		m_bInstanceKillChance	=	max(_iChance,m_bInstanceKillChance);
		m_bInstanceKillChance	=	min(m_bInstanceKillChance,dMAXIMUM_INSTANCE_KILL_CHANCE);
	}//	"즉사"
	void				addMakeBerserk(int _iChance,int _iTime);
	void				addMakeArmorBreak(int _iChance,int _iTime);
	void				addMakeWeaponBreak(int _iChance,int _iTime);
	void				addAddPoisonDamage(int _iDamage,int _iTime);
	void				addAddPoisonDamage(int _iMinDamage,int _iMaxDamage,int _iTime);
	void				calcAddPoisonDamageOption();
	void				addMakeConfuse(int _iChance,int _iTime);
	void				addMakeCharmed(int _iChance,int _iTime);
	void				addMakeStun(int _iChance,int _iTime);
	void				addMakeCold(int _iChance,int _iTime);
	void				addMakeFreeze(int _iChance,int _iTime);
	void				addMakeBlind(int _iChance,int _iTime);
	void				addMakeStone(int _iChance,int _iTime);
	void				addMakeSleep(int _iChance,int _iTime);
	void				addMakeFirstTarget(int _iChance,int _iTime);
	void				addMakeSimpleton(int _iChance,int _iTime);

	void				addAddFireDamage(int _iDamage);
	void				addAddWaterDamage(int _iDamage);
	void				addAddWindDamage(int _iDamage);
	void				addAddLightDamage(int _iDamage);
	void				addAddDarkDamage(int _iDamage);
	void				addAddEarthDamage(int _iDamage);
	void				addAddFireDamage(int _iMinDamage,int _iMaxDamage);
	void				addAddWaterDamage(int _iMinDamage,int _iMaxDamage);
	void				addAddWindDamage(int _iMinDamage,int _iMaxDamage);
	void				addAddLightDamage(int _iMinDamage,int _iMaxDamage);
	void				addAddDarkDamage(int _iMinDamage,int _iMaxDamage);

	void				addAddDamageToUndead(int _iDamage);
	void				addAddDamageToAnimal(int _iDamage);
	void				addAddDamageToHuman(int _iDamage);
	void				addAddDamageToHolyAnimal(int _iDamage);
	void				addAddDamageToDemon(int _iDamage);
//┃공격후 이어지는 부가 공격 효과 설정
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	inline	void		reset()
	{
		memset(this,0,sizeof(cACTOR_STATUS));
		
		m_wUnsealAction	=	0xffff;
		m_wItemEffect	=	0xffff;
	}

	inline	void		resetEquipmentData()
	{
	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃특수 능력
		if (m_sInvisivilityTime	==	-1)	m_sInvisivilityTime	=	0;	//	인비저빌러티시간(-이면 장비에 붙어 있는거다.)
		if (m_sFloatTime		==	-1)	m_sFloatTime		=	0;	//	공중부양 시간(-이면 장비에 붙어 있는거다.)
		if (m_sBlurTime			==	-1)	m_sBlurTime			=	0;	//	흐릿한 상태 시간(-이면 장비에 붙어 있는거다.)
		if (m_sNonTargetTime	==	-1)	m_sNonTargetTime	=	0;	//	타겟에서 제외됨(-1이면 장비에 붙어 있는 것임)

		memset(m_abCorrectMiniPetBasicSkillLevel,0,2);
		memset(m_abCorrectMiniPetSkillLevel,0,2);
		memset(m_abCorrectMiniPetSkillActiveChance,0,2);
		memset(m_abCorrectMiniPetSkillDamage,0,2);

		m_bIncreaseMiniPetFireDamage	=	0;
		m_bIncreaseMiniPetWaterDamage	=	0;
		m_bIncreaseMiniPetWindDamage	=	0;
		m_bIncreaseMiniPetEarthDamage	=	0;
		m_bIncreaseMiniPetLightDamage	=	0;
		m_bIncreaseMiniPetDarkDamage	=	0;
		m_bIncreaseMiniPetMagicDamage	=	0;
		m_bIncreaseMiniPetSkillActiveChance	=	0;
		m_bIncreaseMiniPetBasicSkillLevel	=	0;
		m_bIncreaseMiniPetSkillLevel	=	0;
		m_bIncreaseMiniPetMagicDamageVsBoss	=	0;

		m_detecting.resetEquipmentValue();

		m_regenHPPerSecond.resetEquipmentValue();
		m_bonusExp.resetEquipmentValue();
		m_correctLastDamage.resetEquipmentValue();
		m_bonusItemDropChance.resetEquipmentValue();

		m_aCorrectLevel[0].resetEquipmentValue();
		m_aCorrectLevel[1].resetEquipmentValue();
		m_aCorrectLevel[2].resetEquipmentValue();
		m_aCorrectLevel[3].resetEquipmentValue();
		m_aCorrectLevel[4].resetEquipmentValue();
		m_correctSummonBeastState.resetEquipmentValue();
		m_bonusPetExp.resetEquipmentValue();

		m_bResurrectionChance				=	0;			//	죽었을때 일정 확률로 부활한다.
		m_bIsInfinityBullet					=	FALSE;		//	탄환 리젠 주기
		m_isAutoReload						=	FALSE;		//	자동으로 벨트의 아이템을 리로드 한다.
		m_isMagicBullet						=	FALSE;		//	마법 탄환을 쏠 수 있는 상태
		m_isIgnoreTargetCorrectDodgeChance	=	FALSE;		//	적의 회피 보정치 무시
		m_isIgnoreTargetCorrectHitChance	=	FALSE;		//	적의 명중 보정치 무시
		m_isPerfectHit						=	FALSE;					//	항상 명중
		m_bGetBonusCPPercentage				=	FALSE;		//	CP획득시 보너스..
		m_wIncreaseSitdownHPRecoverSpeed	=	0;
		m_bCorrectGoldDropChance			=	0;
		m_bCorrectFieldFirePenelty			=	0;
		m_bCorrectFieldWaterPenelty			=	0;
		m_bCorrectFieldWindPenelty			=	0;
		m_bCorrectFieldEarthPenelty			=	0;
		m_bCorrectFieldLightPenelty			=	0;
		m_bCorrectFieldDarkPenelty			=	0;

		m_wIncreaseFinalPhysicalAttackPower	=	0;
	//┃특수 능력
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃능력치
		m_iWeaponMinDamage	=	0;
		m_iWeaponMaxDamage	=	0;

		m_attackPower.resetPrefixData();
		m_defensePower.resetPrefixData();
		m_maxHP.resetPrefixData();
		m_maxCP.resetPrefixData();

		m_level.resetEquipmentValue();
		m_weaponPower.resetPrefixData();
		m_armorPower.resetEquipmentValue();

		m_strength.resetEquipmentValue();			//	힘 
		m_agility.resetEquipmentValue();			//	민첩성
		m_constitution.resetEquipmentValue();		//	건강
		m_intelligence.resetEquipmentValue();		//	지식
		m_wisdom.resetEquipmentValue();				//	지혜
		m_charisma.resetEquipmentValue();			//	카리스마
		m_luck.resetEquipmentValue();				//	운

		m_actionSpeed.resetEquipmentValue();		//	액션 속도 보정
		m_moveSpeed.resetEquipmentValue();			//	이동 속도 보정

		m_hitChance.resetEquipmentValue();			//	명중률 보정
		m_dodgeChance.resetEquipmentValue();		//	회피 보정
		m_concentration.resetEquipmentValue();
		m_allignment.resetEquipmentValue();

		m_wBottomCP					=	0;					//	한계 CP 뛰기,(정지/걷기/앉기)등 CP가 떨어지는 액션에서도 CP가 떨어지지 않는다.
	//┃능력치
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃공격이 성공하면 부가적으로 들어가는 효과
		m_bInstanceKillChance		=	0;			//	즉사 시킬 확률

		m_bTargetFleeChance			=	0;			//	대상을 도망가게 만들 확률
		m_bTargetBerserkChance		=	0;			//	대상을 버서크로 만들 확률
		m_bTargetArmorBreakChance	=	0;		//	대상을 갑옷 파괴 상태로 만들 확률
		m_bTargetWeaponBreakChance	=	0;		//	대상을 무기 파괴 상태로 만들 확률
		m_bTargetConfuseChance		=	0;			//	대상을 혼란 시킨다.
		m_bTargetCharmingChance		=	0;		//	대상을 유혹한다.
		m_bTargetStunChance			=	0;			//	대상을 경직 시킨다.
		m_bTargetColdChance			=	0;			//	대상을 콜드 상태로 만든다.
		m_bTargetFreezeChance		=	0;			//	대상을 얼린다.
		m_bTargetBlindChance		=	0;			//	대상을 장님으로 만들 확률
		m_bTargetStoneChance		=	0;			//	대상을 돌로 만들 확률
		m_bTargetSleepChance		=	0;			//	대상을 재울 확률
		m_bTargetFirstTargetChance	=	0;		//	대상을 퍼스트 타겟으로 만들 확률
		m_bTargetSimpltoneChance	=	0;		//	대상을 AI저하 상태로 만들 확률

		m_bAddPoisonMinDamage	=	0;
		m_bAddPoisonMaxDamage	=	0;	//	추가로 대지 데미지를 입힌다.

		m_wAddDamageToUndead	=	0;			//	언데드에게 추가 데미지를 준다.
		m_wAddDamageToDemon		=	0;			//	악마에게 추가 데미지를 준다.
		m_wAddDamageToHuman		=	0;			//	인간에게 추가 데미지를 준다.
		m_wAddDamageToAnimal	=	0;			//	동물에게 추가 데미지를 준다.
		m_wAddDamageToHolyAnimal=	0;		//	신수에게 추가 데미지를 준다.

		m_wTargetBerserkTime	=	0;			//	대상을 버서크로 만드는 시간
		m_wTargetArmorBreakTime	=	0;		//	대상을 갑옷 파괴 상태로 만들 확률
		m_wTargetWeaponBreakTime=	0;		//	대상을 무기 파괴 상태로 만들 확률
		m_wTargetConfuseTime	=	0;			//	대상을 혼란 시킨다.
		m_wTargetCharmingTime	=	0;			//	대상을 유혹한다.
		m_wTargetStunTime		=	0;				//	대상을 경직 시킨다.
		m_wTargetColdTime		=	0;				//	대상을 콜드 상태로 만든다.
		m_wTargetFreezeTime		=	0;			//	대상을 얼린다.
		m_wTargetBlindTime		=	0;				//	대상을 중독 시킨다.
		m_wTargetStoneTime		=	0;				//	대상을 돌로 만들 확률
		m_wTargetSleepTime		=	0;				//	대상을 재울 확률
		m_wTargetFirstTargetTime	=	0;		//	대상을 퍼스트 타겟으로 만들 확률
		m_wTargetSimpltoneTime		=	0;			//	대상을 AI저하 상태로 만들 확률

		m_wTargetDeclineHitChanceTime	=	0;	//	대상을 명중률 저하 상태에 빠지게 한다.
		m_wTargetDeclineDodgeChanceTime	=	0;//	대상을 회피율 저하 상태에 빠지게 한다.
		m_wTargetCurseAmplificationTime	=	0;//	대상을 회피율 저주 증폭 상태에 빠지게 한다.

		m_wTargetPoisonedTime	=	0;			//	대상을 중독 시킨다.
		m_wLifeStealPecentage	=	0;			//	흡혈률

		m_wMinAddPoisonDamage	=	0;
		m_wMaxAddPoisonDamage	=	0;
		m_wAddPoisonDamageOptionCount	=	0;

		m_wAddFireMinDamage		=	0;
		m_wAddFireMaxDamage		=	0;	//	추가로 불꽅 데미지를 입힌다.
		m_wAddWaterMinDamage	=	0;
		m_wAddWaterMaxDamage	=	0;	//	추가로 물 데미지를 입힌다.
		m_wAddWindMinDamage		=	0;
		m_wAddWindMaxDamage		=	0;	//	추가로 바람 데미지를 입힌다.
		m_wAddLightMinDamage	=	0;
		m_wAddLightMaxDamage	=	0;	//	추가로 빛 데미지를 입힌다.
		m_wAddDarkMinDamage		=	0;
		m_wAddEarthDamage		=	0;
		m_wAddDarkMaxDamage		=	0;	//	추가로 어둠 데미지를 입힌다.
		//vvv 09.08.10   마법추뎀 DX
		memset(m_sAddNaturalDamageDX,0,sizeof(sADD_NATURAL_DAMAGE_DX)*eADD_DAMAGE_DX_END);
		//AAA 09.08.10   마법추뎀 DX
	//┃공격이 성공하면 부가적으로 들어가는 효과
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃데미지를 체력으로 흡수하거나 카운터 날림
		m_bFirstAidPercentage	=	0;			//	입은 데미지의 일부를 치료한다.

		m_bAbsorbFireDamage		=	0;			//	불꽃 데미지를 흡수해서 체력으로 변환
		m_bAbsorbWaterDamage	=	0;			//	물 데미지를 흡수해서 체력으로 변환
		m_bAbsorbWindDamage		=	0;			//	바람 데미지를 흡수해서 체력으로 변환
		m_bAbsorbEarthDamage	=	0;			//	대지 데미지를 흡수해서 체력으로 변환
		m_bAbsorbLightDamage	=	0;			//	빛 데미지를 흡수해서 체력으로 변환
		m_bAbsorbDarkDamage		=	0;			//	어둠 데미지를 흡수해서 체력으로 변환
		m_bAbsorbMagicDamage	=	0;			//	모든 마법 데미지를 흡수해서 체력으로 변환

		m_returnDamageRate.resetEquipmentValue();//	데미지의 일부를 공격자에게 돌려준다.
		m_returnDamageRateIDEF.resetEquipmentValue();
		m_bDamageToCPRate		=	0;			//	데미지의 일부를 CP로 변환해 준다.
		m_bReturnFreezeChance	=	0;			//	리턴 프리즈 확률
		m_bReturnColdChance		=	0;			//	리턴 콜드 확률

		m_wReturnFreezeTime		=	0;			//	반격!! 프리즈가 성공 했을때 프리즈 되는 시간.
		m_wReturnColdTime		=	0;			//	반격!! 콜드가 성공 했을때 프리즈 되는 시간.
	//┃데미지를 체력으로 흡수하거나 카운터 날림
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃각종 수치 보정
		m_correctAllSkillLevel.resetEquipmentValue();

		m_iCorrectRecoverBlocking		=	0;	//	블럭킹 회복 시간 보정
		m_bCorrectRareItemDropRate		=	0;	//	레어 아이템이 떨어질 확률을 보정한다.
		m_bCorrectUniqueItemDropRate	=	0;	//	유니크 아이템이 떨어질 확률을 보정한다.
		m_bCorrectMagicItemDropChance	=	0;
		m_correctCriticalHitChance.resetEquipmentValue();	//	크리티컬 히트 날릴 기회를 보정한다.
		m_bCorrectCrushBlowChance		=	0;	//	크러쉬 블로우 날릴 기회를 보정한다.
		m_bCorrectKnockoutAttackChance	=	0;	//	녹아웃 공격을 시킬 기회를 보정한다.
		m_bCorrectReloadSpeed			=	0;	//	벨트 채우는 속도 보정
		m_bCorrectTransformationSpeed	=	0;	//	변신 속도 보정
		m_bCorrectHPRecoverSpeed		=	0;	//	체력 회복 속도 보정
		m_blockingChance.resetEquipmentValue();				//	블럭킹률

		m_weakningTargetFireResistance.resetEquipmentValue();
		m_weakningTargetWaterResistance.resetEquipmentValue();
		m_weakningTargetWindResistance.resetEquipmentValue();
		m_weakningTargetEarthResistance.resetEquipmentValue();
		m_weakningTargetLightResistance.resetEquipmentValue();
		m_weakningTargetDarkResistance.resetEquipmentValue();
		m_weakningTargetMagicResistance.resetEquipmentValue();

		m_strengtheningFireDamage.resetEquipmentValue();
		m_strengtheningWaterDamage.resetEquipmentValue();
		m_strengtheningWindDamage.resetEquipmentValue();
		m_strengtheningEarthDamage.resetEquipmentValue();
		m_strengtheningDarkDamage.resetEquipmentValue();
		m_strengtheningLightDamage.resetEquipmentValue();
		m_strengtheningMagicDamage.resetEquipmentValue();
		m_strengtheningCriticalDamage.resetPrefixData();		///치명타대미지증가.
		m_correcthardBlowHitChance.resetEquipmentValue();	//	강타 보정

	//┃각종 수치 보정
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃각종 저항
		m_bFreezeResistance		=	0;		//	상태 이상 저항
		m_bColdResistance		=	0;		//	상태 이상 저항
		m_bBlindResistance		=	0;		//	상태 이상 저항
		m_stunResistance.resetEquipmentValue();//
		m_bConfuseResistance	=	0;		//	상태 이상 저항
		m_charmingResistance.resetEquipmentValue();//
		m_bStoneResistance		=	0;		//	상태 이상 저항
		m_bPoisonResistance		=	0;		//	상태 이상 저항
		m_bSleepResistance		=	0;		//	상태 이상 저항

		m_bCriticalHitResistance	=	0;	//	크리티컬 저항
		m_bCrushBlowResistance		=	0;	//	크러쉬 블로우 저항
		m_bKnockBackResistance		=	0;	//	녹아웃 저항
		m_bInstanceKillResistance	=	0;	//	즉사 저항

		m_strangeStatusResistance.resetEquipmentValue();//
		m_declinePowerResistance.resetEquipmentValue();//
		m_curseResistance.resetEquipmentValue();//

		m_fireResistance.resetEquipmentValue();//	"불저항"
		m_earthResistance.resetEquipmentValue();//	"대지저항"
		m_windResistance.resetEquipmentValue();//	"바람저항"
		m_waterResistance.resetEquipmentValue();//	"물저항"
		m_lightResistance.resetEquipmentValue();//	"빛저항"
		m_darkResistance.resetEquipmentValue();//	"어둠저항"
		m_allMagicResistance.resetEquipmentValue();//	"마법 속성 공격에 대한 저항"
	//┃보정된 마법 저항
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃기타
		m_wBeltStackCount			=	0;				//	벨트에 들어가는 아이템 수
		m_iCorrectMinimumDamage		=	0;
		m_iCorrectMaximumDamage		=	0;

		m_wCarvingOptionShieldChance	=	0;
		m_wCarvingOptionShieldValue		=	0;
		
		m_wCarvingOptionChargeTime		=	0;
		m_wCarvingOptionChargeValue		=	0;
		
		m_wCarvingOptionIncreaseExp		=	0;

		m_wCarvingOptionAddFireDamageChance		=	0;
		m_iCarvingOptionAddFireDamage			=	0;
		m_wCarvingOptionSoulGuardTime			=	0;
		m_iCarvingOptionMaxHP					=	0;
		m_iLevelGetingRedStone					=	0;
	//┃기타
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	}
	inline	char*		getStatusPointer(){return	(char*)this;}
	
};



class	CPlayerDeathPeneltyInfo
{
public:
	int		m_iBeginLevel,m_iEndLevel;
	int		m_iPeneltySecond,m_iPeneltyValue;
	int		m_iRecoverPriceValue;
};

extern	CPlayerDeathPeneltyInfo	g_aPlayerDeathPeneltyInfo[20];

#endif