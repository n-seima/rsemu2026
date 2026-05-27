#pragma	once

#include "../common/miniPetCommon.h"

class	cACTOR;
class	cItem;

class cMiniPet :public cMiniPetCommon
{
public:

	cMiniPet();

	void	reset();
	void	create(int _iType);
	void	evolution(BOOL _bIsReset = FALSE);	//	진화
	BOOL	increaseExp(int _iExp,int _iType,int _iLordLevel);
	void	usePolisher();
	void	operateVSBossCorrectMagicPower(cACTOR *_lpActor,int *_lpiFireDamage	,int *_lpiWaterDamage,int *_lpiWindDamage,int *_lpiEarthDamage,int *_lpiLightDamage,int *_lpiDarkDamage);
	int		getDecreaseChargeCount(cACTOR *_lpActor ,BOOL _bIsMinipet);
	int		getVSBossCorrectTargetMagicResistance(cACTOR *_lpActor);
	int		getVSBossCorrectActiveChance(cACTOR *_lpActor);
	BOOL	isExpired();	//	시간이 다 된 미니펫
	BOOL	renewExpireTime(int _iMinute);//	시간이 다 된 미니펫
	BOOL	setAwaken(BOOL _is100persent);

	int		getEvolutionRate(int _iType);//	진화율

	cMiniPetAbilityInfo*	getPowerUpSkill(int _iSkill);
	cMiniPetAbilityInfo*	getSkillByType(int _iType);

	void	setTest(int _iStyle);

	int		addSpareSkill();	//	잔여 스킬과 레벨을 확인해 스킬 추가
	void	unseal(cItem *_lpItem);	//	꺼내기

	cMiniPetAbilityInfo*	getAbilityBySkill(int _iSkill);
	BOOL					isLevelUpAbleAbility(cMiniPetAbilityInfo *_lpAbility);
	BOOL	resetSkill();
	int		getNeedExp(int _iMinLevel, int _wMaxLevel);
};