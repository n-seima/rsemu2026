#ifndef _classActiveSkill_H
#define _classActiveSkill_H

#include	"ast_define.h"

class	cAreaDoorInfo;
class	cTrap;

#pragma	pack(2)

class	CActiveSkill	:	public cActiveSkillBaseInfo
{
public:
	WORD			m_aValue[dEXTRA_VALUE_FOR_ACTIVE_SKILL];

					CActiveSkill()	
					{
						m_lpCaster	=	NULL;
						m_lpTarget	=	NULL;
						m_lpSkill	=	NULL;	//	蝶鑒
						m_lpDoor	=	NULL;

						reset();
					}

	void			reset();
	BOOL			casterStop();	//	議蝶攪陛 薑雖ц.
	BOOL			casterDeath();	//	議蝶攪陛 避歷.
	BOOL			targetDeath();	//	顫檜 避歷.

	inline	CSkill	*getSkill()	{return m_ability.getSkill();}
	BOOL			isCaster(int _iIndex);	//	樽陛 議蝶攪傍?
	BOOL			isCorrectTarget();	//	薯渠煎脹 顫檜氈傍?

	BOOL			update();	//	機等檜お
	BOOL			fire(int  *_lpiResult);
	void			init();

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早astDefendOnImageTrap.cpp
	BOOL			updateDefendOnImageTrap();
//早astDefendOnImageTrap.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillContinuousHitOfDoppelganger.cpp
	int				fireContinuousHitOfDoppelganger();
	BOOL			updateContinuousHitOfDoppelganger();
	void			strikeContinuousHitOfDoppelgangerAttack();

	inline	void	setIsContinuousHitOfDoppelgangerTriggerAction(BOOL _bIs)				{m_aValue[49]	=	_bIs;}
	inline	BOOL	isContinuousHitOfDoppelgangerTriggerAction()							{return	m_aValue[49];}
//早CSkillContinuousHitOfDoppelganger.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillExplosion.cpp
	int				fireExplosionTypeSkillDependOnImage();
	BOOL			castExplosionTypeSkillDependOnImage();
	BOOL			updateExplosionTypeSkillDependOnImage();

	inline	void	setDependOnImageExplosionSkillCastingTime(int _iTime)	{m_aValue[0] = _iTime;}
	inline	BOOL	decreaseDependOnImageExplosionSkillCastingTime()
	{
		if (m_aValue[0] == 0 ) return FALSE;

		m_aValue[0]--;

		return	TRUE;
	}

	inline	void	setDependOnImageExplosionSkillGetCpStatus(BOOL _bIsGeted)	{m_aValue[1] = _bIsGeted;}
	inline	BOOL	getDependOnImageExplosionSkillGetCpStatus()					{return m_aValue[1];}
	inline	void	setDependOnImageExplosionSkillZoomEffectTime(int _iTime)	{m_aValue[2] = _iTime;}
	inline	BOOL	decreaseDependOnImageExplosionSkillZoomEffectTime()
	{
		if (m_aValue[2] == 0) return FALSE;

		m_aValue[2]--;

		return	TRUE;
	}
//早CSkillExplosion.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillGuardianPost.cpp
	int				fireGuardianPost();
	BOOL			updateGuardianPost();
	BOOL			castGuardianPost();
	BOOL			strikeGuardianPost();

	void			setGuardianPostBulletCount(int _iShotCount)
	{
		m_aValue[0]	=	_iShotCount;
	}
	BOOL			decreaseGuardianPostBulletCount()
	{
		if (m_aValue[0]	==	0)
			return	TRUE;

		m_aValue[0]--;

		return	FALSE;
	}

	int				getGuardianPostBulletCount()
	{
		return	m_aValue[0];
	}

	void			setGuardianPostUpkeepTime(int _iUpkeepTime)
	{
		m_aValue[1]	=	_iUpkeepTime;
	}
	int				getGuardianPostUpkeepTime()
	{
		return	m_aValue[1];
	}
	BOOL			decreaseGuardianPostUpkeepTime()
	{
		if (m_aValue[1]	==	0)
			return	TRUE;

		m_aValue[1]--;

		return	FALSE;
	}

	void			setActiveGuardianPost(BOOL _bIsActive)
	{
		m_aValue[2]	=	_bIsActive;
	}
	int				isActiveGuardianPost()
	{
		return	m_aValue[2];
	}

	void			setGuardianPostStrikePeriod(int _iPeriod)
	{
		m_aValue[3]	=	_iPeriod;
	}
	BOOL			decreaseGuardianPostStrikePeriod()
	{
		if (m_aValue[3]	==	0)
			return	TRUE;

		m_aValue[3]--;

		return	FALSE;
	}

	void			setGuardianPostImage(int _iImage)
	{
		m_aValue[4]	=	_iImage;
	}
	int				getGuardianPostImage()
	{
		return	m_aValue[4];
	}

	void			setGuardianPostJobChangeStatus(BOOL _bIsStatus)
	{
		m_aValue[5]	=	_bIsStatus;
	}
	BOOL			isGuardianPostJobChanged()
	{
		return	m_aValue[5];
	}
//早CSkillGuardianPost.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillWhirlwind.cpp
	int				fireWhirlwindTypeSkill();
	BOOL			updateWhirlwindTypeSkill();
//早CSkillWhirlwind.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillBouncingLinear.cpp
	int				fireBouncingLinear();
	BOOL			castBouncingLinear();
	BOOL			updateBouncingLinear();
	BOOL			aimingNextBouncingLinearTarget();

	inline	void	setBouncingLinearTargetIndex(int _iIndex)	{m_aValue[40]=_iIndex;}
	inline	int		getBouncingLinearTargetIndex()				{return	m_aValue[40];}
	inline	void	increaseBouncingLinearTargetIndex()			{m_aValue[40]++;}

	inline	void	setBouncingLinearTargetCount(int _iCount)	{m_aValue[41]=_iCount;}
	inline	int		getBouncingLinearTargetCount()				{return m_aValue[41];}

	inline	void	setBouncingLinearAttackRange(int _iRange)	{m_aValue[42]=_iRange;}
	inline	int		getBouncingLinearAttackRange()				{return	m_aValue[42];}

//早CSkillBouncingLinear.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早astDoubleTarget.cpp
	int				fireDoubleTarget();
	BOOL			castDoubleTarget();
	BOOL			updateDoubleTarget();
//早astDoubleTarget.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早astContinuousBit.cpp
	int				fireContinuousBit();
	BOOL			updateContinuousBit();
//早astContinuousBit.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收	


//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillWhirlthrusting.cpp
	int				fireWhirlthrusting();
	BOOL			updateWhirlthrusting();
	inline	void	setWhirlthrustingStep(int _iStep)
	{
		m_aValue[0]	=	_iStep;
	}
	inline	int		getWhirlthrustingStep()
	{
		return	m_aValue[0];
	}
	inline	void	setWhirlthrustingStrikeTime(int _iTime)
	{
		m_aValue[1]	=	_iTime;
	}
	inline	int		getWhirlthrustingStrikeTime()
	{
		return	m_aValue[1];
	}
//早CSkillWhirlthrusting.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillSurprisingRaid.cpp
	int				fireSurprisingRaid();
	BOOL			updateSurprisingRaid();
	BOOL			castSurprisingRaid();
	inline	void	setSurprisingRaidStep(int _iStep)
	{
		m_aValue[0]	=	_iStep;
	}
	inline	int		getSurprisingRaidStep()
	{
		return	m_aValue[0];
	}
//早CSkillSurprisingRaid.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillExplosionMissile.cpp
	int				fireExplosionMissile();
	void			castExplosionMissile();
	BOOL			updateExplosionMissile();
//早CSkillExplosionMissile.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillMirrorTower.cpp
	int				fireMirrorTower();
	BOOL			updateMirrorTower();
//早CSkillMirrorTower.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillContinuousShoot.cpp
	int				fireContinuousShoot();		//	餌 蝶鑒
	BOOL			updateContinuousShoot();	//	餌 蝶鑒 機等檜お
//早CSkillContinuousShoot.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillBitGlider.cpp
	int				fireBitGlider();			//	綠お 旋檜渦
	BOOL			updateBitGlider();			//	綠お 旋檜渦 機等檜お
//早CSkillBitGlider.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillWaterFall.cpp
	BOOL			updateWaterFall();
	void			castWaterFall();
	int				fireWaterFall();

	inline	void	resetWaterFallUpkeepTime()					{m_aValue[0]	=	0;}
	inline	void	setWaterFallUpkeepTime(int _iUpkeepTime)	{m_aValue[0]	=	_iUpkeepTime;}
	inline	BOOL	decreaseWaterFallUpkeepTime()
	{
		if (m_aValue[0]	==	0)	return	TRUE;

		m_aValue[0]--;

		return	FALSE;
	}

	inline	void	resetWaterFallStrikePeriod()				{m_aValue[1]	=	0;}
	inline	void	setWaterFallStrikePeriod(int _iPeriod)		{m_aValue[1]	=	_iPeriod;}
	inline	int		getWaterFallStrikePeriod()					{return	m_aValue[1];}

	inline	void	resetWaterFallCasting()						{m_aValue[2]	=	FALSE;}
	inline	BOOL	isWaterFallCasting()						{return m_aValue[2];}
	inline	void	setWaterFallCasting()						{m_aValue[2]	=	TRUE;}

	inline	void	resetWaterFallStrikeCounter()				{m_aValue[3]	=	m_aValue[1];}
	inline	BOOL	updateWaterFallStrikeCounter()
	{
		BOOL	bIsStrikeTime	=	FALSE;

		if (m_aValue[3]	==	getWaterFallStrikePeriod()*3/4)		bIsStrikeTime	=	TRUE;

		if (m_aValue[3]		)	m_aValue[3]--;
		if (m_aValue[3]	==	0)	resetWaterFallStrikeCounter();

		return	bIsStrikeTime;
	}

	inline	void	resetWaterFallGetedCPStatus()				{m_aValue[4]	=	FALSE;}
	inline	void	setWaterFallGetedCPStatus(BOOL _bIsGetedCP)	{m_aValue[4]	=	_bIsGetedCP;}
	inline	BOOL	isWaterFallGetedCPStatus()					{return m_aValue[4];}
	inline	void	setWaterFallCastingTime(int _iTime)			{m_aValue[5]	=	_iTime;}
	inline	BOOL	decreaseWaterFallCastingTime()	
	{
		if (m_aValue[5]	==	0)	return	FALSE;

		m_aValue[5]--;

		return	TRUE;
	}

//早CSkillWaterFall.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillIntervalShooter.cpp
	inline	void	setIntervalShooterFired()						
	{
		m_aValue[2]	=	TRUE;
	}
	inline	void	resetIntervalShooterFired()						{m_aValue[2]	=	FALSE;}
	inline	BOOL	isIntervalShooterFired()						{return m_aValue[2];}

	inline	void	setIntervalShooterUpkeepTime(int _iTime)		{m_aValue[4]	=	_iTime;}
	inline	BOOL	decreaseIntervalShooterUpkeepTime()
	{
		if (m_aValue[4]	==	0)	return	TRUE;
		m_aValue[4]--;

		return	FALSE;
	}

	inline	void	setIntervalShooterAttackCount(int _iShotCount)	{m_aValue[0] = _iShotCount;}
	inline	int		getIntervalShooterAttackCount()					{return m_aValue[0];}

	inline	void	resetIntervalShooterShotCounter()				{m_aValue[1]	=	0;}
	inline	BOOL	increaseIntervalShooterShotCounter()
	{
		m_aValue[1]++;

		if (m_aValue[1]	>=	getIntervalShooterAttackCount())	return	TRUE;

		return	FALSE;
	}

	inline	void	resetDeathIntervalShooter()						{m_aValue[3]	=	FALSE;}
	inline	void	setDeathIntervalShooter()						{m_aValue[3]	=	TRUE;}
	inline	BOOL	isDeathIntervalShooter()						{return m_aValue[3];}

	inline	void	setIntervalShooterDamage(int _iPhysicalDamage,int _iFireDamage,int _iWaterDamage,int _iWindDamage,int _iEarthDamage,int _iLightDamage,int _iDarkDamage)
	{
		*(int *)(m_aValue+6)	=	_iFireDamage;
		*(int *)(m_aValue+8)	=	_iWaterDamage;
		*(int *)(m_aValue+10)	=	_iWindDamage;
		*(int *)(m_aValue+12)	=	_iEarthDamage;
		*(int *)(m_aValue+14)	=	_iLightDamage;
		*(int *)(m_aValue+16)	=	_iDarkDamage;
		*(int *)(m_aValue+18)	=	_iPhysicalDamage;
	}
	inline	void	getIntervalShooterDamage(int *_lpPhysicalDamage,int *_lpFireDamage,int *_lpWaterDamage,int *_lpWindDamage,int *_lpEarthDamage,int *_lpLightDamage,int *_lpDarkDamage)
	{
		*_lpFireDamage			=	*(int *)(m_aValue+6)	;
		*_lpWaterDamage			=	*(int *)(m_aValue+8)	;
		*_lpWindDamage			=	*(int *)(m_aValue+10)	;
		*_lpEarthDamage			=	*(int *)(m_aValue+12)	;
		*_lpLightDamage			=	*(int *)(m_aValue+14)	;
		*_lpDarkDamage			=	*(int *)(m_aValue+16)	;
		*_lpPhysicalDamage		=	*(int *)(m_aValue+18)	;
	}

	int				fireIntervalShooter();
	BOOL			updateIntervalShooter();
	void			strikeIntervalShooter(cACTOR *_lpTarget);
//早CSkillIntervalShooter.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillLandMarker.cpp
	int				fireLandMarker();
	BOOL			updateLandMarker();

	inline	void	resetLandMarkerFinish()					{m_aValue[dEXTRA_VALUE_FOR_ACTIVE_SKILL-2] = FALSE;}
	inline	void	setLandMarkerFinish()					{m_aValue[dEXTRA_VALUE_FOR_ACTIVE_SKILL-2] = TRUE;}
	inline	BOOL	isLandMarkerFinish()					{return m_aValue[dEXTRA_VALUE_FOR_ACTIVE_SKILL-2];}

	inline	void	resetLandMarkerGetedCP()				{m_aValue[dEXTRA_VALUE_FOR_ACTIVE_SKILL-1] = FALSE;}
	inline	void	setLandMarkerGetedCP(BOOL _bIsGeted)	{m_aValue[dEXTRA_VALUE_FOR_ACTIVE_SKILL-1] = _bIsGeted;}
	inline	BOOL	isLandMarkerGetedCP()					{return m_aValue[dEXTRA_VALUE_FOR_ACTIVE_SKILL-1];}
//早CSkillLandMarker.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillSpreadArrow.cpp
	int				fireSpreadArrow();
	void			castSpreadArrow();
	BOOL			updateSpreadArrow();
//早CSkillSpreadArrow.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillMissile.cpp
	int				fireMissileTypeSkill();
	void			castMissile();
	BOOL			updateMissileTypeSkill();

	BOOL			checkTailChaserInMissileTypeSkill(int _iDamage);
	void			fireTailChaserInMissileTypeSkill();

	inline	void	resetTailChaserStatusInMissileTypeSkill()	{m_aValue[1]=0;}
	inline	void	setActiveTailChaserInMissileTypeSkill(int _iHitCount,int _iPeriod,int _iDamage,int _iRemainFrame,int _iCorrectDamage)
	{
		m_aValue[0]		=	max(_iDamage/100,1);
		m_aValue[1]		=	1;
		m_aValue[2]		=	_iHitCount;
		m_aValue[3]		=	_iPeriod;
		m_aValue[4]		=	_iCorrectDamage;
		m_wRange		=	_iRemainFrame;
	}
	inline	void	setTailChaserStatusInMissileTypeSkill(int _iStatus)	{m_aValue[1]=_iStatus;}	//	 羹檜憮 嫦翕?
	inline	int		getTailChaserStatusInMissileTypeSkill()	{return	m_aValue[1];}	//	 羹檜憮 嫦翕?

	inline	BOOL	decreaseTailChaserFireTime()	//	お 
	{
		if (m_wRange	<=	0)	return	TRUE;

		m_wRange--;

		return	FALSE;
	}

//早CSkillMissile.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillIcyStalagmite.cpp
	int				fireIcyStalagmite();
	BOOL			castIcyStalagmite();
	BOOL			updateIcyStalagmite();
	inline	void	setIcyStalagmiteTime(int _iTime)	{m_aValue[0]	=	_iTime;}
	inline	BOOL	decreaseIcyStalagmiteTime()
	{
		if (m_aValue[0]<=	0)	return	TRUE;

		m_aValue[0]--;

		return	FALSE;
	}
//早CSkillIcyStalagmite.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillLaser.cpp
	int				fireLaserTypeSkill();
	BOOL			updateLaserTypeSkill();
//早CSkillLaser.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillPiercingMissile.cpp
	int				firePiercingMissileTypeSkill();
	BOOL			updatePiercingMissileTypeSkill();
//早CSkillPiercingMissile.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillDropOnHead.cpp
	int				fireDropOnHeadTypeSkill();
	BOOL			updateDropOnHeadTypeSkill();
	void			castDropOnHeadTypeSkill();
//早CSkillDropOnHead.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillHeal.cpp
	int				fireHealTypeSkill();
	BOOL			updateHealTypeSkill();
	inline	void	setRestoreExpPercentageForReviveSkill(int _iValue)	{m_aValue[0]	=	max(_iValue,0);}
	inline	int		getRestoreExpPercentageForReviveSkill()				{return	m_aValue[0];}
//早CSkillHeal.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillTonadoShield.cpp
	int				fireTornadoShield();
	BOOL			castTornadoShield();
	BOOL			updateTornadoShield();
//早CSkillTonadoShield.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillTeleport.cpp
	int				fireTeleport();
	BOOL			castTeleport();
	BOOL			updateTeleport();
//早CSkillTeleport.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillJump.cpp
	int				fireJump(BOOL _bIsWildStamp=FALSE);
	BOOL			castJump();
	BOOL			updateJump();

	void			setJumpFlyTime(int _iTime)			{m_aValue[0]	=	_iTime;}
	void			setIsWildStamp(BOOL _bIsWildStamp)	{m_aValue[1]	=	_bIsWildStamp;}
	BOOL			decreaseJumpFlyTime()
	{
		if (m_aValue[0]	==	0)
			return	TRUE;

		m_aValue[0]--;

		return	FALSE;
	}
	BOOL			isWildStamp()						{return	m_aValue[1];}
//早CSkillJump.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillJumpAttack.cpp
	int				fireJumpAttack();
	BOOL			castJumpAttack();
	BOOL			updateJumpAttack();
//早CSkillJumpAttack.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillAlteringHitter.cpp
	int				fireAlteringHitter();
	BOOL			castAlteringHitter();
	BOOL			updateAlteringHitter();

	void			setBeginAlteringHitterAttack(BOOL _bIsBegin)
	{
		m_aValue[11]	=	_bIsBegin;
	}
	BOOL			isBegunAlteringHitterAttack()
	{
		return	m_aValue[11];
	}
	void			setAlteringHitterAttackPeriodCounter(int _iTime)
	{
		m_aValue[10]	=	_iTime;
	}
	BOOL			decreaseAlteringHitterAttackPeriodCounter()
	{
		if (m_aValue[10]	==	0)
			return	TRUE;

		m_aValue[10]--;

		return	FALSE;
	}
//早CSkillAlteringHitter.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillRush.cpp
	int				fireRushTypeSkill();
	void			castRushTypeSkill(BOOL _bIsRunning=FALSE);
	BOOL			updateRushTypeSkill();
	void			crushRushSkill();
	
	void			setIsWhirlRunningStyleRush()
	{
		m_aValue[0]	=	TRUE;
	}
	BOOL			isWhirlRunningStyleRush()
	{
		return	m_aValue[0];
	}

	void			setRushSkillRequireFrame(int _iFrame)
	{
		m_wFrame	=	_iFrame;
	}
	BOOL			decreaseRushSkillRequireFrame()
	{
		if (m_wFrame	==	0)
			return	TRUE;

		m_wFrame--;

		return	FALSE;
	}


//早CSkillRush.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillTornado.cpp
	int				fireTornadoTypeSkill();
	BOOL			castTornadoTypeSkill();
	BOOL			updateTornadoTypeSkill();
//早CSkillTornado.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillMachine.cpp
	int				fireMachineTypeSkill();
	BOOL			castMachineTypeSkill();
	BOOL			updateMachineTypeSkill();

	BOOL			shootMachineTypeSkill();

	inline	int		getMachineFPS()						{return	m_aValue[2];}
	inline	void	setMachineFPS(int _iFPS)			{m_aValue[2]	=	_iFPS;}
	inline	void	setMachineAttackRange(int _iRange)	{m_aValue[1]	=	_iRange;}
	inline	void	setMachineTime(int _iTime)			{m_aValue[0]	=	_iTime;}
	inline	void	setMachineMinDamage(int _iDamage)	{*(int *)(m_aValue+3) = _iDamage;}
	inline	void	setMachineMaxDamage(int _iDamage)	{*(int *)(m_aValue+5) = _iDamage;}

	inline	int		getMachineMinDamage()				{return	*(int *)(m_aValue+3);}
	inline	int		getMachineMaxDamage()				{return	*(int *)(m_aValue+5);}

	inline	BOOL	decreaseMachineTime()
	{
		if (m_aValue[0]	<=	0)	return	TRUE;

		m_aValue[0]--;

		return	FALSE;
	}
//早CSkillMachine.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillMachineMissile.cpp
	int				fireMachineMissile();
	BOOL			castMachineMissile();
	BOOL			updateMachineMissile();
//早CSkillMachineMissile.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillPetCommand.cpp
	int				firePetCommand();
	BOOL			castPetCommand();
	BOOL			updatePetCommand();
	void			operatePetCommand(cACTOR *_lpPet);

	inline	void	setPetCommandWaitTime(int _iIndex,int _iTime)
	{
		m_aValue[_iIndex]	=	_iTime;
	}
	inline	int		getPetCommandWaitTime(int _iIndex)
	{
		return	m_aValue[_iIndex];
	}
	inline	BOOL	isCommandTime(int _iIndex)
	{
		if (m_aValue[_iIndex]	==	0)
			return	TRUE;

		if (m_aValue[_iIndex]	==	0xffff)
			return	FALSE;

		m_aValue[_iIndex]--;

		return	FALSE;
	}

	inline	BOOL	isWaitCommandTime()
	{
		return	m_aValue[10];
	}
	inline	void	setWaitCommandTime(BOOL _bIsSet)
	{
		m_aValue[10]	=	_bIsSet;	
	}
//早CSkillPetCommand.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillAidMagic.cpp
	int				fireAidMagic();
	BOOL			castAidMagic();
	BOOL			updateAidMagic();
//早CSkillAidMagic.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillAidAttack.cpp
	int				fireAidAttack();
	BOOL			castAidAttack();
	BOOL			updateAidAttack();
//早CSkillAidAttack.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillBiteHanging.cpp
	int				fireBiteHanging();
	BOOL			castBiteHanging();
	BOOL			updateBiteHanging();
	
	void			resetBiteHangingCasting()					{m_aValue[0]	=	FALSE;}
	void			setBiteHangingCasting()						{m_aValue[0]	=	TRUE;}
	BOOL			isBiteHangingCasting()						{return	m_aValue[0];}

	void			setBiteHangingUpkeepTime(DWORD _dwTime)
	{
		(*(DWORD *)(m_aValue+2))	=	_dwTime;
	}
	BOOL			isBiteHangingFinishTime(DWORD _dwTime)
	{
		if	(_dwTime	>=	(*(DWORD *)(m_aValue+2)))
			return	TRUE;

		return	FALSE;
	}
	void			setBiteHangingActionTime(DWORD _dwTime)
	{
		(*(DWORD *)(m_aValue+4))	=	_dwTime;
	}
	BOOL			isBiteHangingActionTime(DWORD _dwTime)
	{
		if	(_dwTime	>=	(*(DWORD *)(m_aValue+4)))
			return	TRUE;

		return	FALSE;
	}

	void			setReleaseBiteHangingCasterHPRate(int _iRemainHP)			{m_aValue[6]	=	_iRemainHP;}
	BOOL			isReleaseBiteHangingCasterHPRate(int _iCheckHP)
	{
		if	(_iCheckHP	<=	m_aValue[6])
			return	TRUE;

		return	FALSE;
	}


//早CSkillBiteHanging.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillCharging.cpp
	int				fireChargeSkill();
	BOOL			updateChargeSkill();
	inline	void	setChargingTime(int _iTime)	{m_aValue[0]	=	_iTime;}
	inline	int		getChargingTime()	{return	m_aValue[0];}
	inline	BOOL	decreaseChargingTime()
	{
		if (m_aValue[0]	==	0)	return	TRUE;

		m_aValue[0]--;

		return	FALSE;
	}
//早CSkillCharging.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillImpact.cpp
	int				fireImpactSkill();
	BOOL			updateImpactSkill();
//早CSkillImpact.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillContinuousAttack.cpp
	int				fireContinuousAttack();
	BOOL			updateContinuousAttack();

	inline	void	setContinuousAttackActionTime(int _iTime)	{m_aValue[0]	=	_iTime;}
	inline	int		getContinuousAttackActionTime()				{return m_aValue[0];}
	inline	void	setContinuousAttackCount(int _iTimes)		{m_aValue[1]	=	_iTimes;}	//	樓 奢問 蝦
	inline	int		getContinuousAttackCount()					{return	m_aValue[1];}			//	樓 奢問 蝦
	inline	void	resetContinuousAttackShotCount()			{m_aValue[2]	=	0;}			//	奢問и 蝦
	inline	int		getContinuousAttackShotCount()				{return	m_aValue[2];}			//	奢問и 蝦
	inline	BOOL	increaseContinuousAttackShotCount()			
	{
		m_aValue[2]++;
		if (m_aValue[2]	>=	getContinuousAttackCount())	return	TRUE;

		return	FALSE;
	}
	inline	void	setContinuousAttackDamageSumOfPhysicalDamage(int _iDamage)	{*(int *)(m_aValue+3) = _iDamage;}
	inline	void	setContinuousAttackDamageSumOfMagicDamage(int _iDamage)		{*(int *)(m_aValue+5) = _iDamage;}
	inline	int		getContinuousAttackDamageSumOfPhysicalDamage()				{return	*(int *)(m_aValue+3);}
	inline	int		getContinuousAttackDamageSumOfMagicDamage()					{return	*(int *)(m_aValue+5);}

	inline	void	resetContinuousAttackGetCPStatus()				{m_aValue[7]	=	FALSE;}
	inline	void	setContinuousAttackGetCPStatus()				{m_aValue[7]	=	TRUE;}
	inline	BOOL	getContinuousAttackGetCPStatus()				{return	m_aValue[7];}

	inline	void	setContinuousAttackFPS(int _iFPS)				{m_aValue[8]	=	_iFPS;}
	inline	int		getContinuousAttackFPS()						{return	m_aValue[8];}
	
	inline	void	setContinuousAttackMissCount(int _iCount)		{m_aValue[9]	=	_iCount;}
	inline	int		getContinuousAttackMissCount()					{return	m_aValue[9];}
	
	inline	void	setContinuousAttackCriticalCount(int _iCount)	{m_aValue[20]	=	_iCount;}
	inline	int		getContinuousAttackCriticalCount()				{return	m_aValue[20];}

	inline	void	setContinuousAttackDoubleCriticalCount(int _iCount)	{m_aValue[21]	=	_iCount;}
	inline	int		getContinuousAttackDoubleCriticalCount()			{return	m_aValue[21];}


	inline	void	setContinuousAttackMinPhysicalDamage(int _iCount)		{m_aValue[22]	=	_iCount;}
	inline	int		getContinuousAttackMinPhysicalDamage()				{return	m_aValue[22];}

	inline	void	setContinuousAttackMinMagicDamage(int _iCount)		{m_aValue[23]	=	_iCount;}
	inline	int		getContinuousAttackMinMagicDamage()					{return	m_aValue[23];}

	inline	void	setContinuousAttackBlockingCount(int _iCount)		{m_aValue[24]	=	_iCount;}
	inline	int		getContinuousAttackBlockingCount()					{return	m_aValue[24];}
	
	inline	void	setContinuousAttackInstanceKillCount(int _iCount)		{m_aValue[25]	=	_iCount;}
	inline	int		getContinuousAttackInstanceKillCount()					{return	m_aValue[25];}

	inline	void	setContinuousAttackNoActionBlockCount(int _iCount)		{m_aValue[26]	=	_iCount;}
	inline	int		getContinuousAttackNoActionBlockCount()					{return	m_aValue[26];}

	inline	void	setContinuousAttackDancingBlockCount(int _iCount)		{m_aValue[27]	=	_iCount;}
	inline	int		getContinuousAttackDancingBlockCount()					{return	m_aValue[27];}

	inline	void	setContinuousAttackHardBlowCount(int _iCount)		{m_aValue[28]	=	_iCount;}
	inline	int		getContinuousAttackHardBlowCount()					{return	m_aValue[28];}
//早CSkillContinuousAttack.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillAwesomeFortress.cpp
	int				fireAwesomeFortress();
	BOOL			castAwesomeFortress();
	BOOL			updateAwesomeFortress();
	void			strikeAwesomeFortressAttack();

	inline	void	setAwesomeFortressBunshineCount(int _iTimes)				{m_aValue[0]	=	_iTimes;}	//	樓 奢問 蝦
	inline	int		getAwesomeFortressBunshineCount()							{return	m_aValue[0];}			//	樓 奢問 蝦

	inline	void	setIsAwesomeFortressTriggerAction(BOOL _bIs)				{m_aValue[1]	=	_bIs;}
	inline	BOOL	isAwesomeFortressTriggerAction()							{return	m_aValue[1];}

	inline	void	setAwesomeFortressTargetCount(int _iTargetCount)			{m_aValue[2]	=	_iTargetCount;}
	inline	int		getAwesomeFortressTargetCount()								{return	m_aValue[2];}
//早CSkillAwesomeFortress.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillBunshineAttack.cpp
	int				fireBunshineAttack();
	BOOL			castBunshineAttack();
	BOOL			updateBunshineAttack();

	inline	void	setBunshineAttackCount(int _iTimes)							{m_aValue[1]	=	_iTimes;}	//	樓 奢問 蝦
	inline	int		getBunshineAttackCount()									{return	m_aValue[1];}			//	樓 奢問 蝦
	inline	void	setBunshineAttackDamageSumOfPhysicalDamage(int _iDamage)	{*(int *)(m_aValue+3) = _iDamage;}
	inline	void	setBunshineAttackDamageSumOfMagicDamage(int _iDamage)		{*(int *)(m_aValue+5) = _iDamage;}
	inline	int		getBunshineAttackDamageSumOfPhysicalDamage()				{return	*(int *)(m_aValue+3);}
	inline	int		getBunshineAttackDamageSumOfMagicDamage()					{return	*(int *)(m_aValue+5);}

	inline	void	setBunshineAttackMissCount(int _iMissCount)					{m_aValue[10]	=	_iMissCount;}
	inline	int		getBunshineAttackMissCount()								{return	m_aValue[10];}

	inline	void	setBunshineAttackCriticalCount(int _iCount)					{m_aValue[20]	=	_iCount;}
	inline	int		getBunshineAttackCriticalCount()							{return	m_aValue[20];}

	inline	void	setBunshineAttackDoubleCriticalCount(int _iCount)			{m_aValue[21]	=	_iCount;}
	inline	int		getBunshineAttackDoubleCriticalCount()						{return	m_aValue[21];}

	inline	void	setBunshineAttackMinPhysicalDamage(int _iCount)				{m_aValue[22]	=	_iCount;}
	inline	int		getBunshineAttackMinPhysicalDamage()						{return	m_aValue[22];}

	inline	void	setBunshineAttackMinMagicDamage(int _iCount)				{m_aValue[23]	=	_iCount;}
	inline	int		getBunshineAttackMinMagicDamage()							{return	m_aValue[23];}

	inline	void	setBunshineAttackBlockingCount(int _iCount)					{m_aValue[24]	=	_iCount;}
	inline	int		getBunshineAttackBlockingCount()							{return	m_aValue[24];}

	inline	void	setBunshineAttackInstanceKillCount(int _iCount)				{m_aValue[25]	=	_iCount;}
	inline	int		getBunshineAttackInstanceKillCount()						{return	m_aValue[25];}

	inline	void	setBunshineAttackNoActionBlockCount(int _iCount)			{m_aValue[26]	=	_iCount;}
	inline	int		getBunshineAttackNoActionBlockCount()						{return	m_aValue[26];}

	inline	void	setBunshineAttackDancingBlockCount(int _iCount)				{m_aValue[27]	=	_iCount;}
	inline	int		getBunshineAttackDancingBlockCount()						{return	m_aValue[27];}

	inline	void	setBunshineAttackHardBlowCount(int _iCount)				{m_aValue[28]	=	_iCount;}
	inline	int		getBunshineAttackHardBlowCount()						{return	m_aValue[28];}
//早CSkillBunshineAttack.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillFireBall.cpp
	int				fireFireBallTypeSkill();
	BOOL			castFireBallTypeSkill();
	BOOL			updateFireBallTypeSkill();

	inline	BOOL	decreaseFireBallCastTime()
	{
		if (m_aValue[4]	==	0)	return	TRUE;

		m_aValue[4]--;
		
		return	FALSE;
	}
	inline	void	setFireBallPhysicalDamage(int _iPhysicalDamage)	{*(int *)(m_aValue + 0)	=	_iPhysicalDamage;}
	inline	void	setFireBallMagicDamage(int _iMagicDamage)		{*(int *)(m_aValue + 2)	=	_iMagicDamage;}
	inline	int		getFireBallPhysicalDamage()						{return *(int *)(m_aValue + 0);}
	inline	int		getFireBallMagicDamage()						{return *(int *)(m_aValue + 2);}

//早CSkillFireBall.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillScimitarCutting.cpp
	int				fireScimitarCutting();
	void			castScimitarCutting();
	BOOL			updateScimitarCutting();

	inline	void	setScimitarCuttingAngleStep(int _iAngleStep)	{m_aValue[0]	=	_iAngleStep;}
	inline	int		getScimitarCuttingAngleStep()					{return m_aValue[0];}

	inline	void	setScimitarCuttingAngle(int _iAngle)			{m_aValue[1]	=	_iAngle;}
	inline	int		getScimitarCuttingAngle()						{return m_aValue[1];}
	inline	BOOL	increaseScimitarCuttingAngle()
	{
		m_aValue[1]		+=	getScimitarCuttingAngleStep();

		if (m_aValue[1]	>=	360)
		{
			m_aValue[1]	-=	360;

			increaseScimitarCuttingRoundingCount();
		}

		if (m_aValue[1]	>=	90)	return	TRUE;

		return	FALSE;
	}

	inline	void	resetScimitarCuttingRoundingCount()						{m_aValue[2]=0;}
	inline	int		getScimitarCuttingRoundingCount()						{return m_aValue[2];}
	inline	void	increaseScimitarCuttingRoundingCount()					{m_aValue[2]++;}

	inline	void	setScimitarCuttingRoundCount(int _iRoundCount)			{m_aValue[3]=_iRoundCount;}
	inline	int		getScimitarCuttingRoundCount()							{return m_aValue[3];}

	inline	void	setScimitarCuttingPhysicalDamage(int _iPhysicalDamage)	{*(int *)(m_aValue + 4)	=	_iPhysicalDamage;}
	inline	void	setScimitarCuttingMagicDamage(int _iMagicDamage)		{*(int *)(m_aValue + 6)	=	_iMagicDamage;}
	inline	int		getScimitarCuttingPhysicalDamage()						{return *(int *)(m_aValue + 4);}
	inline	int		getScimitarCuttingMagicDamage()							{return *(int *)(m_aValue + 6);}

	inline	void	resetScimitarCuttingGetCPStatus()						{m_aValue[8] = FALSE;}
	inline	BOOL	getScimitarCuttingGetCPStatus()							{return m_aValue[8];}
	inline	void	setScimitarCuttingGetCPStatus()							{m_aValue[8] = TRUE;}

	inline	void	setScimitarCuttingRange(int _iRange)					{m_aValue[9]=_iRange;}
	inline	int		getScimitarCuttingRange()								{return m_aValue[9];}

	inline	void	setScimitarCuttingBeginAngle(int _iAngle)				{m_aValue[10]	=	_iAngle;}
	inline	int		getScimitarCuttingBeginAngle()							{return m_aValue[10];}

	inline	void	setScimitarCuttingCurrentRange(int _iRange)				{m_aValue[11]	=	_iRange;}
	inline	int		getScimitarCuttingCurrentRange()						{return m_aValue[11];}
	inline	void	increaseScimitarCuttingCurrentRange(int _iRange)		{m_aValue[11]	+=	_iRange;}
	
	inline	void	setScimitarCuttingMissCount(int _iMissCount)			{m_aValue[12]	=	_iMissCount;}
	inline	int		getScimitarCuttingMissCount()							{return m_aValue[12];}

	inline	void	setScimitarCuttingCriticalCount(int _iCount)			{m_aValue[20]	=	_iCount;}
	inline	int		getScimitarCuttingCriticalCount()						{return	m_aValue[20];}

	inline	void	setScimitarCuttingDoubleCriticalCount(int _iCount)		{m_aValue[21]	=	_iCount;}
	inline	int		getScimitarCuttingDoubleCriticalCount()					{return	m_aValue[21];}

	inline	void	setScimitarCuttingMinPhysicalDamage(int _iCount)		{m_aValue[22]	=	_iCount;}
	inline	int		getScimitarCuttingMinPhysicalDamage()					{return	m_aValue[22];}

	inline	void	setScimitarCuttingMinMagicDamage(int _iCount)			{m_aValue[23]	=	_iCount;}
	inline	int		getScimitarCuttingMinMagicDamage()						{return	m_aValue[23];}

	inline	void	setScimitarCuttingBlockingCount(int _iCount)		{m_aValue[24]	=	_iCount;}
	inline	int		getScimitarCuttingBlockingCount()					{return	m_aValue[24];}

	inline	void	setScimitarCuttingInstanceKillCount(int _iCount)		{m_aValue[25]	=	_iCount;}
	inline	int		getScimitarCuttingInstanceKillCount()					{return	m_aValue[25];}

	inline	void	setScimitarCuttingNoActionBlockCount(int _iCount)		{m_aValue[26]	=	_iCount;}
	inline	int		getScimitarCuttingNoActionBlockCount()					{return	m_aValue[26];}

	inline	void	setScimitarCuttingDancingBlockCount(int _iCount)		{m_aValue[27]	=	_iCount;}
	inline	int		getScimitarCuttingDancingBlockCount()					{return	m_aValue[27];}

	inline	void	setScimitarCuttingHardBlowCount(int _iCount)		{m_aValue[28]	=	_iCount;}
	inline	int		getScimitarCuttingHardBlowCount()					{return	m_aValue[28];}
//早CSkillScimitarCutting.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillBoomerang.cpp
	int				fireBoomerang();
	void			castBoomerang();
	BOOL			updateBoomerang();
	inline	void	setBoomerangShieldTime(int _iTime)				{m_aValue[0]	=	0;}
	inline	void	increaseBoomerangShieldTime()					{m_aValue[0]++;}
	inline	int		getBoomerangShieldTime()						{return	m_aValue[0];}
//早CSkillBoomerang.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CSkillShimmeringShield.cpp
	int				fireShimmeringShield();
	BOOL			castShimmeringShield();
	BOOL			updateShimmeringShield();
	void			releaseShimmeringShield();

	inline	void	setShimmeringShieldReceiver(int _iSerial)		{m_aValue[0]	=	_iSerial;}
	inline	void	setShimmeringShieldCaster(int _iSerial)			{m_aValue[1]	=	_iSerial;}
	inline	void	setShimmeringShieldBlockingChance(int _iChance)	{m_aValue[2]	=	_iChance;}
	inline	int		getShimmeringShieldBlockingChance()				{return m_aValue[2];}
	inline	void	setShimmeringShieldTime(int _iTime)				{m_aValue[3]	=	_iTime;}
	inline	BOOL	decreaseShimmeringShieldTime()				
	{
		if (m_aValue[3]	==	0)	return	TRUE;

		m_aValue[3]--;

		return	FALSE;
	}
	inline	BOOL	isShimmeringReceiver(int _iSerial)		{if (m_aValue[0]==_iSerial)	return	TRUE;return FALSE;}
	inline	BOOL	isShimmeringCaster(int _iSerial)		{if (m_aValue[1]==_iSerial)	return	TRUE;return FALSE;}
	inline	int		getShimmeringShieldShape()				{return m_aValue[4];}	//	衛該 褒萄 曄 晦
	inline	void	setShimmeringShieldShape(int _iShape)	{m_aValue[4]	=	_iShape;}	//	衛該 褒萄 曄 撲薑
//早CSkillShimmeringShield.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

/*
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早cSKILL_ILLUSION_MISSILE.cpp
	inline	int		getIllusionMissileCount()			{return	m_aValue[49];}
	inline	void	increaseIllusionMissileCount()		{m_aValue[49]++;}
	inline	BOOL	isGetedCPInIllusionMissileSkill()	{return	m_isGetdCP;}
	inline	void	setGetedCPInIllusionMissileSkill()	{m_isGetdCP	=	TRUE;}
	inline	CHitInfo*	getHitInfoByInIllusionMissile(int _iIndex)	{return	(CHitInfo *)m_aValue + _iIndex;}
	int				fireIllusionMissileTypeSkill();
	void			castIllusionMissile();
	BOOL			updateIllusionMissileTypeSkill();
//早cSKILL_ILLUSION_MISSILE.cpp
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/
	int				getIndex(char *_strSkillName,int _iJob);
};

class	CActiveSkillManager
{
public:
	CActiveSkill			m_aActiveSkill[dMAX_ACTIVE_SKILL];
	int						m_iActiveSkillCount,m_iRookie;
	int						m_iCurrentSkill;
	int						m_iMaxSkillCount;
	int						m_iLastError;

							CActiveSkillManager()
							{
								m_iActiveSkillCount	=	0;
								m_iRookie			=	0;
								m_iCurrentSkill		=	0;
								m_iMaxSkillCount	=	0;
							}

	inline	CActiveSkill*	getActiveSkill(int _iIndex)
	{
		if	(_iIndex	>=	dMAX_ACTIVE_SKILL || _iIndex < 0)
			return	NULL;

		if	(m_aActiveSkill[_iIndex].m_wSerial	==	0xffff)
			return	NULL;
		
		return	&m_aActiveSkill[_iIndex];
	}

	BOOL					remove(int _iSerial);
	int						findActiveSkill(int _iX,int _iY,int _iSkill,int _iRange);
	int						getActiveSkillCount(int _iSkill,int _iCasterZoneSerial);
	WORD					castAtActor(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,int _iRange);
	WORD					castAtGround(cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility,int _iRange);

	WORD					castFireWall(cACTOR *_lpCaster,cAbility *_lpAbility,int _iX1,int _iY1,int _iX2,int _iY2);

	WORD					castGasTypeSkill(cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility,int _iRange);
	WORD					castDefendOnImageTrap(cACTOR *_lpCaster,cTrap *_lpTrap);
	WORD					castQuick(cACTOR *_lpCaster,cAbility *_lpAbility,int _iRange,int *_lpiResult);	//	 議蝶お
	WORD					quickCastAtActor(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility);
	WORD					quickCastAtGround(cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility);

	BOOL					casterStop(int _iSerial);//	議蝶攪陛 薑雖ц.

	void					actorDeath(int _iSerial);//	議蝶攪 顫檜.. 避歷. -_-

	void					update();
};

#pragma	pack()

#endif