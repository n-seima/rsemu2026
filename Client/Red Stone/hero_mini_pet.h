//	CHero 클래스를 상속 받아 미니펫 관련 내용을 처리해 주는 클래스
#ifndef	_class_hero_mini_pet_h
#define	_class_hero_mini_pet_h

#include	"cHero.h"

enum
{
eMPFX_ACTIVATE		,	//	펫을 활성화 시킬 때 100%
eMPFX_DISACTIVATE	,	//	펫을 비활성화 시킬 때 100%
eMPFX_USE_SKILL		,	//	펫이 스킬 사용시 10%
eMPFX_USE_SKILL2	,	//	펫이 스킬 사용 시 10%

eMPFX_KILL_MONSTER		,	//	펫의 공격에 몬스터가 죽었을 경우 100%
eMPFX_KILL_BOSS_MONSTER	,	//	보스 몬스터를 죽였을 경우 100%
eMPFX_IDLE			,	//	전투 중이 아닐 때 15초당 10%
eMPFX_IDLE2			,	//	전투 중이 아닐 때 15초당 10%
eMPFX_IDLE3			,	//	전투 중이 아닐 때 15초당 10%
eMPFX_EAT_LQ_ITEM	,	//	최하급 아이템을 먹였을 경우 20%
eMPFX_EAT_MQ_ITEM	,	//	일반 아이템을 먹였을 경우 20%
eMPFX_EAT_EX_ITEM	,	//	성장전용 아이템 먹였을 경우 20%
eMPFX_LEVEL_UP		,	//	펫 레벨업 시 100%
eMPFX_PLAYER_LEVEL_UP	,	//	캐릭터 레벨업 시100%
};

class	cHero_mini_pet : public CHero
{
public:
	int			getMinipetIncreaseActiveChance(cAbility *_lpAbility , WORD _iSkillIndex);
	int			getMinipetIncreaseHitRange(cAbility *_lpAbility , WORD _iSkillIndex);
	int			getFistAvailMiniPetIndex();
	int			getMiniPetCount();	//	미니펫 수
	int			getBlockLightPocketCount();	//	차광 주머니 수
	cMiniPet*	getMiniPet(int _iSlot);

	BOOL		isAvailReversibleFrame();
	BOOL		operateComboCondition();
	void		operateEvolutionGradeBonus(int _iPetIndex);

	void		operateBuffSkill();	//	미니펫 버프 스킬 처리
	void		applyMiniPetBuffSkillEffects(cAbility *_lpAbility);

	BOOL		playFX(int _iFX,int _iMiniPetIndex,BOOL _bIsTest=FALSE);
	void		changeMiniPetPlace(int _iMiniPet1,int _iMiniPet2);
	void		restraintMiniPet(int _iAttr);
	int			getMiniPetMagicDamage(cAbility *_lpAbility,int _iPetLevel,int _iValue,int _iValuePerLevel,int _iValueRange,int _iValueRangePerLevel,BOOL _bWantMax);
};

#endif