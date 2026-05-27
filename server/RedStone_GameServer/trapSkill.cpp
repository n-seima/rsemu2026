#include "trap.H"
#include "cActor.H"
#include "CSkill.H"
#include "debugCode.H"

cTrapSkillManager	g_trapSkillManager;

BOOL
cTrapSkill::hitFireDamage(cACTOR *_lpTarget,CHitInfo *_lpHitInfo,int _iTrapLevel)
{
	int	iDamage		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	int	iRandValue	=	largeRandom(m_sValue[1][0]/100*2)-m_sValue[1][0]/100;

	iDamage			=	iDamage+iRandValue*iDamage/100;
	iDamage			-=	iDamage*_lpTarget->getFireResistance()/100;

	_lpHitInfo->m_dwMagicDamage	=	max(iDamage,0);

	return	TRUE;
}

BOOL
cTrapSkill::hitWaterDamage(cACTOR *_lpTarget,CHitInfo *_lpHitInfo,int _iTrapLevel)
{
	int	iDamage		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	int	iRandValue	=	largeRandom(m_sValue[1][0]/100*2)-m_sValue[1][0]/100;

	iDamage			=	iDamage+iRandValue*iDamage/100;
	iDamage			-=	iDamage*_lpTarget->getWaterResistance()/100;

	_lpHitInfo->m_dwMagicDamage	=	max(iDamage,0);

	return	TRUE;
}

BOOL
cTrapSkill::hitEarthDamage(cACTOR *_lpTarget,CHitInfo *_lpHitInfo,int _iTrapLevel)
{
	int	iDamage			=	(m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel);
	int	iUpkeepTime		=	(m_sValue[1][0]+m_sValue[1][1]*_iTrapLevel);

	int	iPoisonResistance=	_lpTarget->getPoisonResistance();
	int	iEarthResistance=	_lpTarget->getEarthResistance();

	iUpkeepTime			=	iUpkeepTime*(100-iPoisonResistance)/100;
	iDamage				=	iDamage*(100-iEarthResistance)/100;

	iUpkeepTime			/=	1000;
	iDamage				/=	100;

	_lpTarget->setPoisonDamage(iUpkeepTime,iDamage,0xffff);

	if (iUpkeepTime*iDamage	==	0)
		return	TRUE;

	return	FALSE;
}

BOOL
cTrapSkill::hitWindDamage(cACTOR *_lpTarget,CHitInfo *_lpHitInfo,int _iTrapLevel)
{
	int	iDamage		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	int	iRandValue	=	largeRandom(m_sValue[1][0]/100*2)-m_sValue[1][0]/100;

	iDamage			=	iDamage+iRandValue*iDamage/100;
	iDamage			-=	iDamage*_lpTarget->getWindResistance()/100;

	_lpHitInfo->m_dwMagicDamage	=	max(iDamage,0);

	return	TRUE;
}

BOOL
cTrapSkill::hitLightDamage(cACTOR *_lpTarget,CHitInfo *_lpHitInfo,int _iTrapLevel)
{
	int	iDamage		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	int	iRandValue	=	largeRandom(m_sValue[1][0]/100*2)-m_sValue[1][0]/100;

	iDamage			=	iDamage+iRandValue*iDamage/100;
	iDamage			-=	iDamage*_lpTarget->getLightResistance()/100;

	_lpHitInfo->m_dwMagicDamage	=	max(iDamage,0);

	return	TRUE;
}

BOOL
cTrapSkill::hitDarkDamage(cACTOR *_lpTarget,CHitInfo *_lpHitInfo,int _iTrapLevel)
{
	int	iDamage		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	int	iRandValue	=	largeRandom(m_sValue[1][0]/100*2)-m_sValue[1][0]/100;

	iDamage			=	iDamage+iRandValue*iDamage/100;
	iDamage			-=	iDamage*_lpTarget->getDarkResistance()/100;

	_lpHitInfo->m_dwMagicDamage	=	max(iDamage,0);

	return	TRUE;
}

BOOL
cTrapSkill::hitPhysicalDamage(cACTOR *_lpTarget,CHitInfo *_lpHitInfo,int _iTrapLevel)
{
	int			iAttackPower	=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	int			iRandValue		=	largeRandom(m_sValue[1][0]/100*2)-m_sValue[1][0]/100;
	int			iDefensivePower	=	_lpTarget->getDefensivePower();

	iAttackPower				=	iAttackPower+iRandValue*iAttackPower/100;

	LONGLONG	llDamage		=	iAttackPower;
	int			iDamage			=	(int)(llDamage*llDamage/(llDamage+iDefensivePower));

	_lpHitInfo->m_dwPhysicalDamage=	max(iDamage,0);

	return	TRUE;
}

BOOL
cTrapSkill::hitBlind(cACTOR *_lpTarget,int _iTrapLevel)
{
	int	iActiveChance	=	100*(100-_lpTarget->getBlindResistance())/100;

	if (random(100)	>=	iActiveChance)
		return	TRUE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;

	iUpkeepTime			-=	iUpkeepTime*_lpTarget->getBlindResistance()/100;

	if (iUpkeepTime%100)
		iUpkeepTime		=	iUpkeepTime/100+1;
	else
		iUpkeepTime		/=	100;

	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime	==	0)
		return	TRUE;

	_lpTarget->m_wBlindTime	=	max(_lpTarget->m_wBlindTime,iUpkeepTime);

	return	FALSE;
}

BOOL
cTrapSkill::hitCold(cACTOR *_lpTarget,int _iTrapLevel,int _iTime)
{
	int	iActiveChance	=	100*(100-_lpTarget->getColdResistance())/100;

	if (random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;

	if (_iTime)
		iUpkeepTime		=	_iTime;

	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getColdResistance())/100;
	iUpkeepTime			/=	100;

	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime	==	0)
		return	TRUE;

	_lpTarget->m_wColdTime	=	max(iUpkeepTime,_lpTarget->m_wColdTime);

	if (_lpTarget->m_wColdTime)	
		_lpTarget->moveDestPos();

	return	FALSE;
}

BOOL
cTrapSkill::hitFreeze(cACTOR *_lpTarget,int _iTrapLevel)
{
	int	iActiveChance	=	100*(100-_lpTarget->getFreezeResistance())/100;

	if (random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getFreezeResistance())/100;
	iUpkeepTime			/=	100;
	iUpkeepTime			*=	dSYNC_FPS;

	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime	==	0)
		return	TRUE;

	_lpTarget->setFreezeTime(iUpkeepTime);

	return	FALSE;
}

BOOL
cTrapSkill::hitNightmare(cACTOR *_lpTarget,int _iTrapLevel)
{
	int	iActiveChance	=	100*(100-_lpTarget->getSleepResistance())/100;

	if (random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getSleepResistance())/100;
	iUpkeepTime			/=	100;

	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime		==	0)
		return	TRUE;

	_lpTarget->setNightmareTime(iUpkeepTime);

	return	FALSE;
}

BOOL
cTrapSkill::hitParalysis(cACTOR *_lpTarget,int _iTrapLevel)
{
	int	iActiveChance	=	100*(100-_lpTarget->getStunResistance())/100;

	if (random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getStunResistance())/100;
	iUpkeepTime			/=	100;

	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime		==	0)
		return	TRUE;

	_lpTarget->setParalysisTime(iUpkeepTime);

	return	FALSE;
}

BOOL
cTrapSkill::hitConfuse(cACTOR *_lpTarget,int _iTrapLevel)
{
	int	iActiveChance	=	100*(100-_lpTarget->getConfuseResistance())/100;

	if	(random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getConfuseResistance())/100;
	iUpkeepTime			/=	100;

	if	(_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if	(iUpkeepTime		==	0)
		return	TRUE;

	_lpTarget->setCofuseTime(iUpkeepTime,NULL);

	return	FALSE;
}

BOOL
cTrapSkill::hitDeclineAttackPower(cACTOR *_lpTarget,int _iTrapLevel)
{
	int	iActiveChance		=	100*(100-_lpTarget->getDeclinePowerResistance())/100;
	
	if (random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getDeclinePowerResistance())/100;
	iUpkeepTime			/=	100;
	
	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime		==	0)
		return	TRUE;

	int	iDeclineValue	=	(100-_lpTarget->getDeclinePowerResistance())*50/100;

	if (iDeclineValue	==	0)
		return	TRUE;

	_lpTarget->m_attackPower.correctPercentageValue(-iDeclineValue,iUpkeepTime);

	return	FALSE;
}

BOOL
cTrapSkill::hitDeclineDefensivePower(cACTOR *_lpTarget,int _iTrapLevel)
{
	int	iActiveChance		=	100*(100-_lpTarget->getDeclinePowerResistance())/100;
	
	if (random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getDeclinePowerResistance())/100;
	iUpkeepTime			/=	100;
	
	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime		==	0)
		return	TRUE;

	int	iDeclineValue	=	(100-_lpTarget->getDeclinePowerResistance())*50/100;

	if (iDeclineValue	==	0)
		return	TRUE;

	_lpTarget->m_defensePower.correctPercentageValue(-iDeclineValue,iUpkeepTime);

	return	FALSE;
}

BOOL
cTrapSkill::hitDeclineHitChance(cACTOR *_lpTarget,int _iTrapLevel,int _iTime)
{
	int	iActiveChance		=	100*(100-_lpTarget->getDeclinePowerResistance())/100;
	
	if (random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;

	if	(_iTime)
		iUpkeepTime		=	iUpkeepTime;

	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getDeclinePowerResistance())/100;
	iUpkeepTime			/=	100;
	
	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime		==	0)
		return	TRUE;

	int	iDeclineValue	=	(100-_lpTarget->getDeclinePowerResistance())*50/100;

	if (iDeclineValue	==	0)
		return	TRUE;

	_lpTarget->m_hitChance.correctValue(-iDeclineValue,iUpkeepTime);

	return	FALSE;
}

BOOL
cTrapSkill::hitDeclineDodgeChance(cACTOR *_lpTarget,int _iTrapLevel,int _iTime)
{
	int	iActiveChance		=	100*(100-_lpTarget->getDeclinePowerResistance())/100;
	
	if (random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;

	if	(_iTime)
		iUpkeepTime		=	iUpkeepTime;

	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getDeclinePowerResistance())/100;
	iUpkeepTime			/=	100;
	
	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime		==	0)
		return	TRUE;

	int	iDeclineValue	=	(100-_lpTarget->getDeclinePowerResistance())*50/100;

	if (iDeclineValue	==	0)
		return	TRUE;

	_lpTarget->m_dodgeChance.correctValue(-iDeclineValue,iUpkeepTime);

	return	FALSE;
}

BOOL
cTrapSkill::hitDeclineAttackSpeed(cACTOR *_lpTarget,int _iTrapLevel)
{
	int	iActiveChance		=	100*(100-_lpTarget->getDeclinePowerResistance())/100;
	
	if (random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getDeclinePowerResistance())/100;
	iUpkeepTime			/=	100;
	
	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime		==	0)
		return	TRUE;

	int	iDeclineValue	=	(100-_lpTarget->getDeclinePowerResistance())*50/100;

	if (iDeclineValue	==	0)
		return	TRUE;

	_lpTarget->m_actionSpeed.correctValue(-iDeclineValue,iUpkeepTime);

	return	FALSE;
}

BOOL
cTrapSkill::hitDeclineMoveSpeed(cACTOR *_lpTarget,int _iTrapLevel)
{
	int	iActiveChance		=	100*(100-_lpTarget->getDeclinePowerResistance())/100;
	
	if (random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getDeclinePowerResistance())/100;
	iUpkeepTime			/=	100;
	
	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime		==	0)
		return	TRUE;

	int	iDeclineValue	=	(100-_lpTarget->getDeclinePowerResistance())*50/100;

	if (iDeclineValue	==	0)
		return	TRUE;

	_lpTarget->m_moveSpeed.correctValue(-iDeclineValue,iUpkeepTime*dSYNC_FPS);

	return	FALSE;
}

BOOL
cTrapSkill::hitDeclineConcentration(cACTOR *_lpTarget,int _iTrapLevel)
{
	int	iActiveChance		=	100*(100-_lpTarget->getDeclinePowerResistance())/100;
	
	if (random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getDeclinePowerResistance())/100;
	iUpkeepTime			/=	100;
	
	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime		==	0)
		return	TRUE;

	int	iDeclineValue	=	(100-_lpTarget->getDeclinePowerResistance())*500/100;

	if (iDeclineValue	==	0)
		return	TRUE;

	_lpTarget->m_concentration.correctValue(-iDeclineValue,iUpkeepTime);

	return	FALSE;
}

BOOL
cTrapSkill::hitDeclineMagicResistance(cACTOR *_lpTarget,int _iTrapLevel)
{
	int	iActiveChance		=	100*(100-_lpTarget->getDeclinePowerResistance())/100;

	if (random(100)	>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getDeclinePowerResistance())/100;
	iUpkeepTime			/=	100;

	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime		==	0)
		return	TRUE;

	int	iDeclineValue	=	(100-_lpTarget->getDeclinePowerResistance())*500/100;

	if (iDeclineValue	==	0)
		return	TRUE;

	_lpTarget->m_allMagicResistance.correctValue(-iDeclineValue,iUpkeepTime);

	return	FALSE;
}

BOOL
cTrapSkill::hitWeakningResistance(cACTOR *_lpTarget,int _iTrapLevel,int _iTime)
{
	int	iActiveChance	=	100*(100-_lpTarget->getDeclinePowerResistance())/100;

	if (random(100)		>=	iActiveChance)
		return	FALSE;

	int	iUpkeepTime		=	m_sValue[0][0]+m_sValue[0][1]*_iTrapLevel;
	
	if (_iTime)
		iUpkeepTime		=	_iTime;

	iUpkeepTime			=	iUpkeepTime*(100-_lpTarget->getDeclinePowerResistance())/100;
	iUpkeepTime			/=	100;

	if (_lpTarget->m_wCurseAmplificationTime)
		iUpkeepTime		=	iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if (iUpkeepTime		==	0)
		return	TRUE;

	_lpTarget->m_wReduceAllResistanceToHalfTime	=	max(_lpTarget->m_wReduceAllResistanceToHalfTime,iUpkeepTime);

	return	FALSE;
}

BOOL
cTrapSkill::explosion(cACTOR *_lpTarget,CHitInfo *_lpHitInfo,int _iTrapLevel)
{
	_lpHitInfo->reset();

	BOOL	bIsHit	=	FALSE;

	switch(m_wEffect)
	{
		case	eTSE_FIRE_DAMAGE			:	//	불대미지
			bIsHit	=	hitFireDamage(_lpTarget,_lpHitInfo,_iTrapLevel);
			break;
		case	eTSE_WATER_DAMAGE			:	//	물대미지;
			bIsHit	=	hitWaterDamage(_lpTarget,_lpHitInfo,_iTrapLevel);
			break;
		case	eTSE_EARTH_DAMAGE			:	//	대지대미지;
			bIsHit	=	hitEarthDamage(_lpTarget,_lpHitInfo,_iTrapLevel);
			break;
		case	eTSE_WIND_DAMAGE			:	//	바람대미지;
			bIsHit	=	hitWindDamage(_lpTarget,_lpHitInfo,_iTrapLevel);
			break;
		case	eTSE_LIGHT_DAMAGE			:	//	빛대미지;
			bIsHit	=	hitLightDamage(_lpTarget,_lpHitInfo,_iTrapLevel);
			break;
		case	eTSE_DARK_DAMAGE			:	//	어둠대미지;
			bIsHit	=	hitDarkDamage(_lpTarget,_lpHitInfo,_iTrapLevel);
			break;
		case	eTSE_PHYSICAL_DAMAGE		:	//	물리대미지;
			bIsHit	=	hitPhysicalDamage(_lpTarget,_lpHitInfo,_iTrapLevel);
			break;
		case	eTSE_BLIND					:	//	장님;
			bIsHit	=	hitBlind(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_CLOD					:	//	얼음결정;
			bIsHit	=	hitCold(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_FREEZE					:	//	얼음;
			bIsHit	=	hitFreeze(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_NIGHTMARE				:	//	잠;
			bIsHit	=	hitNightmare(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_PARALYSIS				:	//	마비;
			bIsHit	=	hitParalysis(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_CONFUSE				:	//	혼란;
			bIsHit	=	hitConfuse(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_DECLINE_ATTACK_POWER	:	//	공격력저하;
			bIsHit	=	hitDeclineAttackPower(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_DECLINE_DEFENSIVE_POWER:	//	방어력저하;
			bIsHit	=	hitDeclineDefensivePower(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_DECLINE_HIT_CHANCE		:	//	명중률저하;
			bIsHit	=	hitDeclineHitChance(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_DECLINE_DODGE_CHANCE	:	//	회피율저하;
			bIsHit	=	hitDeclineDodgeChance(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_DECLINE_ATTACK_SPEED	:	//	공격속도저하;
			bIsHit	=	hitDeclineAttackSpeed(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_DECLINE_MOVE_SPEED		:	//	이동속도저하;
			bIsHit	=	hitDeclineMoveSpeed(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_DECLINE_CONCENTRATION	:	//	집중력감소;
			bIsHit	=	hitDeclineConcentration(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_DECLINE_MAGIC_RESISTANCE:	//	속성저항력저하;
			bIsHit	=	hitDeclineMagicResistance(_lpTarget,_iTrapLevel);
			break;
		case	eTSE_WEAKNING_RESISTANCE	:	//	저항력약화;
			bIsHit	=	hitWeakningResistance(_lpTarget,_iTrapLevel);
			break;
	}
	
	if	(_lpTarget->m_wInvincibleTime)
	{
		_lpHitInfo->m_dwPhysicalDamage	=	0;
		_lpHitInfo->m_dwMagicDamage		=	0;
		_lpHitInfo->setInvincible();
		_lpHitInfo->setHit();
	}

	if	(_lpTarget->m_bIsGuildObject)
	{
		if	(_lpHitInfo->m_dwPhysicalDamage)
			_lpHitInfo->m_dwPhysicalDamage	=	100;
		if	(_lpHitInfo->m_dwMagicDamage)
			_lpHitInfo->m_dwMagicDamage	=	100;

		_lpHitInfo->setHit();
	}

	if	(_lpHitInfo->getDamage())
	{
		if	(_lpTarget->m_bIsGuildObject	==	FALSE)
		{
			if	(m_bf1IsStunBasedLevel)
				_lpTarget->m_wStunTime	=	max(_iTrapLevel,_lpTarget->m_wStunTime);
			
			if	(m_bf1IsDeclineDodgeHitChanceBasedLevel)
			{
				hitDeclineHitChance(_lpTarget,_iTrapLevel,_iTrapLevel);
				hitDeclineDodgeChance(_lpTarget,_iTrapLevel,_iTrapLevel);
			}

			if	(m_bf1IsDeclineMagicResistanceBasedLevel)
				hitWeakningResistance(_lpTarget,_iTrapLevel,_iTrapLevel);

			if	(m_bf1IsColdBasedLevel)
				hitCold(_lpTarget,_iTrapLevel,_iTrapLevel);
		}
		
		_lpTarget->reduceHP(_lpHitInfo->getDamage());
		_lpTarget->sendRemainHP(_lpTarget);
	}

	return	bIsHit;
}

BOOL
cTrapSkillManager::init()
{
	cFILE	file;

	if (!file.Open("data/trapSkill.dat","rb"))
		return	ERRMSG("Red Stone Loading Error","can not find 'data/trapSkill.dat' file");
	
	file.Read(m_aTrapSkill,sizeof(m_aTrapSkill));
	file.Close();

	m_iDamageSkillCount		=	0;
	m_iBadStatusSkillCount	=	0;

	for (int i=0;i<dMAX_TRAP_SKILL;i++)
	{
		if (m_aTrapSkill[i].m_wSerial	==	0xffff)
			continue;

		if (m_aTrapSkill[i].m_wEffect	<	eTSE_DAMAGE_SKILL_BORDER)
			m_iDamageSkillCount++;
		else
			m_iBadStatusSkillCount++;
	}

	return	TRUE;
}

cTrapSkill*
cTrapSkillManager::getSkill(int _iIndex)
{
	if (_iIndex < 0 || _iIndex >= dMAX_TRAP_SKILL)
		return	NULL;

	if (m_aTrapSkill[_iIndex].m_wSerial	==	0xffff)
		return	NULL;

	return	&m_aTrapSkill[_iIndex];
}

cTrapSkill*
cTrapSkillManager::getRandomSkill(int _iSkillType)
{
	int	iRandSkill	=	0;

	if (_iSkillType	==	0)
		iRandSkill	=	random(m_iDamageSkillCount);
	else
		iRandSkill	=	random(m_iBadStatusSkillCount);

	int	iDamageSkillCount		=	0;
	int	iBadStatusSkillCount	=	0;

	for (int i=0;i<dMAX_TRAP_SKILL;i++)
	{
		if (m_aTrapSkill[i].m_wSerial	==	0xffff)
			continue;

		if (m_aTrapSkill[i].m_wEffect	<	eTSE_DAMAGE_SKILL_BORDER)
			iDamageSkillCount++;
		else
			iBadStatusSkillCount++;

		if(_iSkillType	==	0)
		{
			if (iDamageSkillCount	==	iRandSkill)
				return	&m_aTrapSkill[i];
		}
		else
		{
			if (iBadStatusSkillCount==	iRandSkill)
				return	&m_aTrapSkill[i];
		}
	}

	return	NULL;
}