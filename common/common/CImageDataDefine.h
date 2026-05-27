
#ifndef	dIMAGE_DATA_DEFINE_H
#define	dIMAGE_DATA_DEFINE_H

#include <windows.h>

//
//	Effect Data Format
enum
{
	eEDF_QANM,
	eEDF_ANM,
};
//	Effect Output Method
enum
{
	eEOM_RAPE,
	eEOM_STAND,
	eEOM_FLOAT
};
#define	dEFFECT_IMAGE_DATA_COUNT		1024


class	CEffectDataInfo
{
public:
	char	m_strImageName[32];
	char	m_strImageFileName[64];
	WORD	m_wType;
};

class	CSaveImageIndexData
{
public:
	int		m_iEffectCount;		//	이펙트 수

	WORD	m_wImagePlayerEnterance;
	WORD	m_wImageMonsterEnterance;
	WORD	m_wImageLevelUp			;
	WORD	m_wImageSkillLevelUp	;
	WORD	m_wInstanceKillEffect	;
	WORD	m_wDancingBlocker		;
	WORD	m_wNormalHit			;
	WORD	m_wDamageReturnEffect	;
	WORD	m_wDamageAbsorbEffect	;
	WORD	m_wCriticalHitEffect	;
	WORD	m_wSoulOutEffect		;
	WORD	m_wBlockerImage			;
	WORD	m_wSpearBlockerImage	;
	WORD	m_wBoomerangeImage		;
	WORD	m_wRegenHPImage			;

	WORD	m_wHitPoisonEffect		;
	WORD	m_wRushDustImage		;
	WORD	m_wStunImage			;
	WORD	m_wSleepImage			;
	WORD	m_wDisplacementImage	;
	WORD	m_wCastDisplacementImage;
	WORD	m_wConfuseImage			;
	WORD	m_wBerserkImage			;
	WORD	m_wBlindImage			;
	WORD	m_wLevitateShadow		;
	WORD	m_wGuardianPostImage	;
	WORD	m_wEatPotionImage		;
	WORD	m_wCharmedImage			;
	WORD	m_wTownPortalImage		;
	WORD	m_wPortalImage			;

	WORD	m_wBreedingRecordImage	;
	WORD	m_wPetAnalCommandImage	;
	WORD	m_wPetMark				;
	WORD	m_wPetUnsealImage		;
	WORD	m_wSummonBeastPowerUp1	;
	WORD	m_wSummonBeastPowerUp2	;
	WORD	m_wSummonningSummonBeast;
	WORD	m_wInnerTeleport		;
	WORD	m_wInvincible			;
	WORD	m_wLevelDrain			;
	WORD	m_wLevelDown			;
	WORD	m_wDeathCounter			;
	WORD	m_wStealMoney			;
	WORD	m_wHitTortureDamageImage;
	
	WORD	m_wBreakArmor;
	WORD	m_wBreakWeapon;

	WORD	m_wEI_FlameRing			;
	WORD	m_wEI_Incinerate		;
	WORD	m_wEI_VaccumPoint		;
	WORD	m_wEI_InclineStatus		;
	WORD	m_wEI_DeclineStatus		;
	WORD	m_wEI_Taunt				;
	WORD	m_wEI_HotSkin			;
	WORD	m_wEI_DarkWeapon		;
	WORD	m_wEI_Torture			;
	WORD	m_wEI_CancerHall		;
	WORD	m_wEI_UntimateBarrier	;
	WORD	m_wEI_RabbitRush		;
	WORD	m_wEI_Impulse			;
	WORD	m_wEI_BloodDrain		;
	WORD	m_wEI_HwaByung			;
	WORD	m_wEI_PlotOfShadow		;
	WORD	m_wEI_SmellOfDeath		;
	WORD	m_wEI_InterruptingArmor	;
	WORD	m_wEI_Contract			;
	WORD	m_wEI_BloodCompact		;
	WORD	m_wEI_SoulOath			;
	WORD	m_wEI_Marionette		;
	WORD	m_wEI_ReversalPower		;
	WORD	m_wEI_LightningBarrier	;
	WORD	m_wEI_NoMoreFight		;

	WORD	m_wHitDarkDamageEffect	;
	WORD	m_wLaziness				;
	WORD	m_wUseSpecialAttack;
	WORD	m_wPigeonPost			;
	WORD	m_wRollinggLog			;
	WORD	m_wTransImage			;
	
	WORD	m_wMainQuestEagle;
	WORD	m_wSetOffFirecrackerImage;
	WORD	m_awFirecrackerImage[10];
	WORD	m_wShutInMagicBox;
	WORD	m_wSnowImage;		// 눈이미지.
	WORD	m_wEI_SC_Soul_Consensus;		// 교감 시전
	WORD	m_wEI_SC_Soul_WindmilSlash;	//	풍차베기
	WORD	m_wEI_SC_Soul_BindingWords;	// 언어구속
	WORD	m_wEI_SC_Soul_Blaze;		// 소울블레이즈
	WORD	m_wEI_SC_Soul_TyphoonSlash;	// 대회전베기
	WORD	m_wEI_SC_Soul_Snatcher;		// 소울스내쳐
	WORD	m_wEI_SS_Soul_Blaze;		// 소울블레이즈 
	WORD	m_wEI_SA_Soul_StrikeSlash;	// 내려베기

	WORD	m_wEI_SS_Soul_CruelSoul;		// 잔혹한 영혼

	WORD	m_wEI_SH_Soul_Binding;				// 구속류 히트
	WORD	m_wEI_SS_Soul_Release;				// 해방
	WORD	m_wEI_SC_Soul_Casting;				// 캐스팅
	WORD	m_wEI_SS_Soul_Consensus;		// 교감 상태 
	WORD	m_wEI_SS_Soul_InterruptingWeapon	;	// 무기해제
	WORD	m_wEI_SC_Soul_Release;				// 해방	캐스팅..
	WORD	m_wEI_SS_Soul_BindingWords;	// 언어구속
	WORD	m_wEI_SS_Soul_NotUseReaction;	// 리액션 스킬 사용 금지
	WORD	m_wEI_SS_Cham_Bear	;	// 투기상태 - 곰
	WORD	m_wEI_SS_Cham_Bull	;	// 투기상태 - 소
	WORD	m_wEI_SS_Cham_Hawk	;	// 투기상태 - 독수리
	WORD	m_wEI_SS_Cham_Snake	;	// 투기상태 - 뱀
	WORD	m_wEI_SS_Cham_Puma	;	// 투기상태 - 푸마
	WORD	m_wEI_Hit_Hard_Blow	;	//	강타 히트
	WORD	m_wEI_Hit_FightingSpirit	;	// 투지 히트
	WORD	m_wEI_SS_Hard_Blow	;	//	강타 효과
	WORD	m_wEI_Hit_Ignore_Block;	// 블럭무시
	WORD	m_wEI_WhiteBit;			//	화이트비트
	WORD	m_wEI_RedBit;			//	레드비트
	WORD	m_wEI_GleamTant;			//	빛의장막
	WORD	m_wEI_PowerOfVitalization;			//	활성화의 권능
	WORD	m_wHitBleedingEffect;				//	출혈
	WORD	m_wHitElectricShockEffect;		//	감전효과
	WORD	m_wEI_MakeTypeUndead;				//	언데드화
	WORD	m_wEI_ElectricShock;				//	감전
	BYTE	m_abImageOutputMethod[dEFFECT_IMAGE_DATA_COUNT];
};

#endif