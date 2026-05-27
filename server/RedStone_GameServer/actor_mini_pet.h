#ifndef	_class_actor_mini_pet
#define	_class_actor_mini_pet

#include	"cACTOR.h"
#include	"miniPet.h"

class	cActor_miniPet	:	public cACTOR
{
public:
	BOOL				createMiniPet(cItem *_lpItem,int _iCreateMethod=-1);	//	미니펫 소환
	BOOL				sendMiniPetInfo(int _iSlot,int _iMethod,int _iActivePet,int _iActivePet2,int _iIncreaseExp=0,
										BOOL _bIsEvolution=FALSE,int _iFeedType=0 ,BOOL _bIsuseAwakenItme50=FALSE , BOOL _bIsuseAwakenItme100 = FALSE);
	int					getMiniPetCount();
	cMiniPet*			getInvalidMiniPetSlot();	//	빈 미니펫 슬롯 구하기
	cMiniPet*			getActiveMiniPet(int _iIndex);	//	활성화된 미니펫
	void				feedItem(int _iPetIndex,int _iItemIndex);
	void				feedAllItem(int _iPetIndex,int _iItemType , BOOL _bIncludeExtraInven);
	void				mixMiniPet(int _iItem,WORD *_lpwMixList);
	void				restraintMiniPet(int _iItem,int _iMiniPet);

	void				copy2To1();	//	활성 미니펫 2를 1의 정보로 복사하고 1을 리셋 시킨다.
	void				copy1To2();	//	활성 미니펫 1을 2의 정보로 복사하고 2을 리셋 시킨다.
	void				reset1();	//	활성 미니펫 1 리셋
	void				reset2();	//	활성 미니펫 2 리셋

	void				checkCurrentMiniPetExpiredInfo(BOOL _bIsSendMessage=TRUE);	//	현재 활성화된 미니펫의 유효기간을 확인한다.

	int					getInvalidMiniPetSlotIndex();	//	빈 미니펫 슬롯 구하기
	void				activateMiniPet(int _iIndex,BOOL _bIsActivate);
	void				increaseMiniPetSkillLevel(int _iIndex,int _iSkillSlot);
	void				changeMinipetSkillOnOff(int _iIndex,int _iSkillSlot);	//	미니펫 스킬 온오프!	

	BOOL				extendReversalFrameTime(int _iTimes,BOOL _bIsDay=TRUE,BOOL _bIsSendMessage=TRUE);	//	양면 프레임 소환 시간 연장
	void				useChargeSkill(cACTOR *_lpTarget,int _iPetOrder);
	void				useBlockLightPocket(int _iMiniPet);	//	차광 주머니에 가두기
	void				usePolisher(int _iItem,int _iMiniPet,BOOL _bIsMassive);
	void				useAwakenItem(int _iItem,int _iMiniPet,BOOL _bIs100PersentItem);	
	void				resetMiniPetName(int _iItem,int _iMiniPet);
	void				changeMiniPetPlace(int _iPetIndex,int _iDirect);	//	미니펫 위치 변경
	BOOL				unsealMiniPet(cItem *_lpItem);	//	차광 주머니에 미니펫 봉인 해제
	BOOL				resetSkill(int _iPetIndex,int _iItemSlot);		// 스킬 초기화
	BOOL				changeType(int _iPetIndex,int _iItemSlot);	//	미니펫 타입 변경


	inline	cMiniPet*	getMiniPet(int _iIndex)
	{
		if	(_iIndex	<	0	||	_iIndex	>=	c_iMiniPetCount)
			return	NULL;

		return	(cMiniPet*)&m_aMiniPet[_iIndex];
	}

	void				operateMiniPetSkill();	//	미니펫 스킬 처리
	void				operateMiniPetDamagedTriggerSkill();	//	타격 입을때 발동하는 스킬 처리
	int					getMiniPetMagicDamage(cAbility *_lpAbility,int _iPetLevel,int _iValue,int _iValuePerLevel,int _iValueRange,int _iValueRangePerLevel,cMiniPet* _lpMiniPet = NULL, int _iUserLevel = 0);
	BOOL				castImpactSkill(cMiniPet *_lpMiniPet,cMiniPetAbilityInfo *_lpAbility,cACTOR *_lpTarget=NULL,int _iPetIndex=0 , int iLevel = 0);
	void				castContinuousShoot(cMiniPet *_lpMiniPet,cMiniPetAbilityInfo *_lpAbility,cACTOR *_lpTarget,int _iPetIndex);

	void				getMagicDamageByMiniPet(cMiniPet *_lpMiniPet,cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,int _iPetOrder , int _iLevel);
	void				operateVSBossCorrectMagicPower(cACTOR *_lpActor,int *_lpiFireDamage,int *_lpiWaterDamage,int *_lpiWindDamage,int *_lpiEarthDamage,int *_lpiLightDamage,int *_lpiDarkDamage);
	int					getVSBossCorrectTargetMagicResistance(cACTOR *_lpActor);
	void				operatePowerUpSkill();	//	미니펫 파워업 스킬 처리
	void				operateDestroyObjectSkill(int *_lpiAttackPower,int *_lpiMagicPower);	//	오브젝트 파괴 스킬 처리
	int					getCorrectUnlockLevel();
	int					getDecreaseDeathPeneltyValue();	//	부확 페널티 감소 수치
	void				operateUseHealPotionWork(int _iHP);
	void				operateUseChargePotionWork(int _iCP);
	void				operateReduceCPWork(int _iSkill,int _iReduceCP);	//	CP 감소시 액션
	void				operateHealSkillWork(int _iHealHP);	//	힐 스킬 받을 시 액션
	void				operateDodgeTrapSkill(int *_lpiDodgeChance);
	BOOL				getDroppedItemByMiniPet(CDroppedItem *_lpItem,cAbility *_lpAbility);
	void				operateMiniPetChargeSkill();
	void				updateChargeCount(BOOL _bIsUpdate=FALSE);
	void				applyMiniPetBuffSkillEffects(cAbility *_lpAbility);

	void				operateSkillForKilled();	//	죽으면 발동하는 스킬 처리
	void				setTestMiniPet(int _iStyle);	//	테스트 미니펫으로 설정

	BOOL				operatePlaceCombiConditionBonus();	//	
	void				operateEvolutionGradeBonus();	

	inline	BOOL		isAvailMiniPet(int _iIndex)
	{
		if	(_iIndex	==	0)
			_iIndex	=	m_bf5ActiveMiniPet;
		else
			_iIndex	=	m_bf5ActiveMiniPet2;

		if	(_iIndex	>=	c_iMiniPetCount)
			return	FALSE;

		if	(m_aMiniPet[_iIndex].m_bf7Level	==	0)
			return	FALSE;

		return	TRUE;
	}
};


#endif