#include <math.h>
#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"
#include "debugCode.H"

//
//	¹ ½´ ¿
int
CActiveSkill::fireIntervalShooter()
{
	int	iShotCount	=	m_ability.getContinuousShotCount();

	m_lpCaster->action(m_lpSkill->m_wAction);
	m_lpCaster->setActionSpeed(&m_ability);
	m_lpCaster->m_attackInfo.m_ability.copy(&m_ability);

	int	iRemainTime	=	m_lpCaster->m_wRemainActionTime*iShotCount;

	setIntervalShooterAttackCount(iShotCount);
	resetIntervalShooterShotCounter();
	resetIntervalShooterFired();
	resetDeathIntervalShooter();
	setIntervalShooterUpkeepTime(0);

	m_lpCaster->action(m_lpSkill->m_wAction,TRUE);
	m_lpCaster->m_wRemainActionTime	=	iRemainTime/dREVISE_ACTION_SPEED_FOR_TEST;

	int		iPhysicalDamage=0,iFireDamage=0,iWaterDamage=0,iWindDamage=0,iEarthDamage=0,iLightDamage=0,iDarkDamage=0;

	BOOL	bIsPvP			=	FALSE;
	BOOL	bIsBossBattle	=	FALSE;

	if	(m_lpCaster->isPlayer() && m_lpTarget->isPlayer())
		bIsPvP		=	TRUE;

	if	(m_lpCaster->m_wIsBoss	||	m_lpTarget->m_wIsBoss)
		bIsBossBattle	=	TRUE;

	if	(m_lpSkill->isPhysicalDamageSkill())	
		iPhysicalDamage	=	m_lpCaster->getPhysicalAttackPower(&m_ability,m_lpTarget->m_iLevel,bIsPvP,bIsBossBattle,NULL,FALSE,NULL);
	if	(m_lpSkill->isFireDamageSkill())		
		iFireDamage		=	m_ability.getMagicDamage(eAM_ADD_FIRE_DAMAGE, iPhysicalDamage);
	if	(m_lpSkill->isWaterDamageSkill())	
		iWaterDamage	=	m_ability.getMagicDamage(eAM_ADD_WATER_DAMAGE, iPhysicalDamage);
	if	(m_lpSkill->isWindDamageSkill())		
		iWindDamage		=	m_ability.getMagicDamage(eAM_ADD_WIND_DAMAGE, iPhysicalDamage);
	if	(m_lpSkill->isEarthDamageSkill())	
		iEarthDamage	=	m_ability.getMagicDamage(eAM_ADD_EARTH_DAMAGE, iPhysicalDamage);
	if	(m_lpSkill->isLightDamageSkill())	
		iLightDamage	=	m_ability.getMagicDamage(eAM_ADD_LIGHT_DAMAGE, iPhysicalDamage);
	if	(m_lpSkill->isDarkDamageSkill())		
		iDarkDamage		=	m_ability.getMagicDamage(eAM_ADD_DARK_DAMAGE, iPhysicalDamage);

	setIntervalShooterDamage(iPhysicalDamage,iFireDamage,iWaterDamage,iWindDamage,iEarthDamage,iLightDamage,iDarkDamage);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	¹ ½´ ¾÷Æ®
BOOL
CActiveSkill::updateIntervalShooter()
{
	if	(!m_lpCaster	||	m_lpCaster->isDeath())
		return	TRUE;

	if	(isIntervalShooterFired())
	{
		if	(isDeathIntervalShooter())
			return	TRUE;

		if	(decreaseIntervalShooterUpkeepTime())
		{
			m_lpCaster->releaseIntervalShooter();
			return	TRUE;
		}

		return	FALSE;
	}

	if	(isActionTime())
	{
		setIntervalShooterFired();
		setIntervalShooterUpkeepTime(m_ability.getUpkeepTime()*dSYNC_FPS);
		m_lpCaster->setIntervalShooter(m_wSerial);
	}

	return	FALSE;
}

//
//	¹ ½´ ·!!
void
CActiveSkill::strikeIntervalShooter(cACTOR *_lpTarget)
{
	if	(!m_lpCaster || m_lpCaster->m_wSerialInField == 0xffff || m_lpCaster->isDeath())
		return;

	m_lpCaster->m_wIntervalShooter=	0xffff;

	CHitInfo	hitInfo;

	int			iPhysicalDamage,iFireDamage,iWaterDamage,iWindDamage,iEarthDamage,iLightDamage,iDarkDamage;
	int			iShotCount		=	getIntervalShooterAttackCount();

	getIntervalShooterDamage(&iPhysicalDamage,&iFireDamage,&iWaterDamage,&iWindDamage,&iEarthDamage,&iLightDamage,&iDarkDamage);

	int		iCorrectMagicDamageByAlchemyTitle	=	0;
	BOOL	bIsPureMagicDamageSkill				=	m_lpSkill->isPureMagicDamageSkill();

	if	(bIsPureMagicDamageSkill)
		iCorrectMagicDamageByAlchemyTitle	=	m_lpCaster->m_wAlchemyTitleFactor;
	
	iFireDamage					+=	iFireDamage*m_lpCaster->getStrengtheningFireDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)/100;
	iWaterDamage				+=	iWaterDamage*m_lpCaster->getStrengtheningWaterDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)/100;
	iWindDamage					+=	iWindDamage*m_lpCaster->getStrengtheningWindDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)/100;
	iEarthDamage				+=	iEarthDamage*m_lpCaster->getStrengtheningEarthDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)/100;
	iLightDamage				+=	iLightDamage*m_lpCaster->getStrengtheningLightDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)/100;
	iDarkDamage					+=	iDarkDamage*m_lpCaster->getStrengtheningDarkDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)/100;

	LONGLONG	llDamage		=	iPhysicalDamage;
	iFireDamage					-=	iFireDamage*_lpTarget->getFireResistance(m_lpCaster->getWeakningTargetFireResistance())/100;
	iWaterDamage				-=	iWaterDamage*_lpTarget->getWaterResistance(m_lpCaster->getWeakningTargetWaterResistance())/100;
	iWindDamage					-=	iWindDamage*_lpTarget->getWindResistance(m_lpCaster->getWeakningTargetWindResistance())/100;
	iEarthDamage				-=	iEarthDamage*_lpTarget->getEarthResistance(m_lpCaster->getWeakningTargetEarthResistance())/100;
	iLightDamage				-=	iLightDamage*_lpTarget->getLightResistance(m_lpCaster->getWeakningTargetLightResistance())/100;
	iDarkDamage					-=	iDarkDamage*_lpTarget->getDarkResistance(m_lpCaster->getWeakningTargetDarkResistance())/100;

	hitInfo.reset(TRUE);

	LONGLONG	llDivideDamage	=	max(llDamage+_lpTarget->getDefensivePower(),1);

	hitInfo.m_dwPhysicalDamage	=	(DWORD)(llDamage*llDamage/llDivideDamage);
	hitInfo.m_dwMagicDamage		=	iFireDamage+iWaterDamage+iWindDamage+iEarthDamage+iLightDamage+iDarkDamage;

	if	(m_lpCaster->m_lpField->isPVP(m_lpCaster,_lpTarget))
	{
		hitInfo.m_dwPhysicalDamage	=	GetP2PCorrectDamage(hitInfo.m_dwPhysicalDamage,iShotCount);
		hitInfo.m_dwMagicDamage		=	GetP2PCorrectDamage(hitInfo.m_dwMagicDamage,iShotCount);
	}

	hitInfo.m_dwPhysicalDamage	*=	iShotCount;
	hitInfo.m_dwMagicDamage		*=	iShotCount;

	if	(_lpTarget->m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	hitInfo.m_dwPhysicalDamage;
		llDamage	=	llDamage*_lpTarget->m_wCorrectFinalDamage/10000;
		hitInfo.m_dwPhysicalDamage	=	(DWORD)llDamage;
		hitInfo.m_dwPhysicalDamage	=	max(hitInfo.m_dwPhysicalDamage,100);

		llDamage	=	hitInfo.m_dwMagicDamage;
		llDamage	=	llDamage*_lpTarget->m_wCorrectFinalDamage/10000;
		hitInfo.m_dwMagicDamage		=	(DWORD)llDamage;
		hitInfo.m_dwMagicDamage		=	max(hitInfo.m_dwMagicDamage,100);
	}

	hitInfo.m_dwPhysicalDamage	=	min(hitInfo.m_dwPhysicalDamage,dMAX_PHYSICAL_DAMAGE);
	hitInfo.m_dwMagicDamage		=	min(hitInfo.m_dwMagicDamage,dMAX_MAGICAL_DAMAGE);


	if	(_lpTarget->m_wInvincibleTime)
	{
		hitInfo.m_dwPhysicalDamage	=	0;
		hitInfo.m_dwMagicDamage		=	0;
		hitInfo.setInvincible();
	}

	if	(_lpTarget->m_wJumppingSkippingTime	||	_lpTarget->m_wTransToFrogTime	||	_lpTarget->m_wHwabyungTime)
	{
		hitInfo.m_dwPhysicalDamage	=	0;
		hitInfo.m_dwMagicDamage		=	0;
	}

	if	(hitInfo.m_dwPhysicalDamage + hitInfo.m_dwMagicDamage)
	{
		hitInfo.setHit();

		if	(hitInfo.m_dwPhysicalDamage	)
			hitInfo.setPhysicalDamage();

		m_lpCaster->increaseCP(m_ability.getGetCP());
		m_lpCaster->syncPlayerBasicStatus(TRUE);
	}
	
	if	(_lpTarget->m_bIsGuildObject)
	{
		hitInfo.m_dwPhysicalDamage	=	100;
		hitInfo.m_dwMagicDamage		=	0;
	}

	m_lpCaster->strike(_lpTarget,&hitInfo,&m_ability,FALSE,FALSE);

	setDeathIntervalShooter();

	if	(m_lpCaster && m_lpCaster->m_lpField)
		m_lpCaster->m_lpField->sendStrikeIntervalShooter(_lpTarget,&m_ability,hitInfo.m_dwPhysicalDamage,hitInfo.m_dwMagicDamage,getIntervalShooterAttackCount());
}