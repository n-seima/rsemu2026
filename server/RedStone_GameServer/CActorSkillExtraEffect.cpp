#include <math.h>
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include "debugCode.H"
#include "arca.H"
#include "CEvent.H"
#include "secretDungeon.H"
#include "packetManager.H"
#include "actor_mini_pet.H"


enum
{	// 마지막 수정일 : 09.09.29
		eDAMAGE_FIRE,
		eDAMAGE_WIND,
		eDAMAGE_WATER,
		eDAMAGE_DARK,
		eDAMAGE_LIGHT,
		eDAMAGE_EARTH,
};


//
//	스킬 부가 효과 적용
BOOL
cACTOR::applySkillExtraEffects(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iHitDamage,int _iX,int _iY,BOOL _bIsByAura , BOOL _bIsByKarma, DWORD _dwKeepTime)
{		// 마지막 수정일 : 09.10.12
	if	(!_lpAbility)
		return	FALSE;

	if	(m_iGuildGuardianType	||	m_bIsGuildObject)
		return	FALSE;

	int		iHitCount	=	0;
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(!lpSkill)
		return	FALSE;

	if	(!checkStatusForUseSkill(&lpSkill->m_checkTargetStatus))
		return	FALSE;

//	남은 체력에 의한 적용 제한
	{
		int		iLimitHPPercentage	=	(lpSkill->m_sApplyLimitHPPercentage+lpSkill->m_sApplyLimitHPPercentagePerLevel*_lpAbility->m_wLevel)/100;
		iLimitHPPercentage			=	min(iLimitHPPercentage,lpSkill->m_wLimitApplyLimitHPPercentage/100);

		if	(iLimitHPPercentage)
		{
			int	iRemainHPPercentage	=	m_iHP*100/getMaxHP();

			if	(iRemainHPPercentage	> iLimitHPPercentage)	
				return	FALSE;
		}
	}

	int		iSkillUpkeepTime	=	_lpAbility->getUpkeepTime();
	if	(lpSkill->m_sTargetLevelLimitType1	!=	0x7fff)
	{
		int		iLimitLevel		=	_lpAttacker->getLevel()+_lpAbility->m_wLevel+lpSkill->m_sTargetLevelLimitType1;
		
		if	(getLevel()	>	max(iLimitLevel,1))
			return	FALSE;
	}

	if	(lpSkill->m_sHumanTargetLevelLimitType1	!=	0x7fff && isHuman())
	{
		int		iLimitLevel		=	_lpAttacker->getLevel()+_lpAbility->m_wLevel+lpSkill->m_sHumanTargetLevelLimitType1;
		
		if	(getLevel()	>	max(iLimitLevel,1))
			return	FALSE;
	}
	if	(lpSkill->m_sUndeadTargetLevelLimitType1	!=	0x7fff && isUndead())
	{
		int		iLimitLevel		=	_lpAttacker->getLevel()+_lpAbility->m_wLevel+lpSkill->m_sUndeadTargetLevelLimitType1;
		
		if (getLevel()	>	max(iLimitLevel,1))
			return	FALSE;
	}
	if	(lpSkill->m_sAnimalTargetLevelLimitType1	!=	0x7fff && isAnimal())
	{
		int		iLimitLevel		=	_lpAttacker->getLevel()+_lpAbility->m_wLevel+lpSkill->m_sAnimalTargetLevelLimitType1;
		
		if (getLevel()	>	max(iLimitLevel,1))
			return	FALSE;
	}
	if	(lpSkill->m_sHolyBeastTargetLevelLimitType1	!=	0x7fff && isHolyBeast())
	{
		int		iLimitLevel		=	_lpAttacker->getLevel()+_lpAbility->m_wLevel+lpSkill->m_sHolyBeastTargetLevelLimitType1;
		
		if (getLevel()	>	max(iLimitLevel,1))
			return	FALSE;
	}
	if	(lpSkill->m_sDemonTargetLevelLimitType1	!=	0x7fff && isDemon())
	{
		int		iLimitLevel		=	_lpAttacker->getLevel()+_lpAbility->m_wLevel+lpSkill->m_sDemonTargetLevelLimitType1;
		
		if (getLevel()	>	max(iLimitLevel,1))
			return	FALSE;
	}

	int	iUpkeepTimeForEncahntedSkill	=	0;

	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		CSkillExtraEffect		*lpExtraEffect	=	&lpSkill->m_aExtraEffect[i];
		CSkillExtraEffectInfo	skillEE;

		if	(!_lpAbility->getExtraEffect(i,&skillEE))
			break;

		if	(m_wInvincibleTime	)
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_ENEMY	||	lpExtraEffect->m_bf4TargetMethod	==	eEEAT_ENEMY_PET	||	
				lpExtraEffect->m_bf4TargetMethod	==	eEEAT_SELF_AND_ENEMY)
				continue;

		if	(lpExtraEffect->m_bf1IsActiveChaceInversePropotionByRemainHP)
		{
			int	iRate				=	max(m_iHP*100/getMaxHP(),1);

			skillEE.m_sActiveChance	=	skillEE.m_sActiveChance*100/iRate;
			skillEE.m_sActiveChance	=	min(skillEE.m_sActiveChance,lpSkill->m_aExtraEffect[i].m_wLimitActiveChance/100);
		}
		if	(lpExtraEffect->m_bf1IsActiveChaceInversePropotionByRemainHPxMonsterGrade1)
		{
			int		iRate			=	max(m_iHP*100/getMaxHP(),1);
			cJOB	*lpJob			=	getJob();

			if (!lpJob)
				continue;

			int		iValue			=	1<<lpJob->m_wMonsterLevel;

			iRate					*=	iValue;

			skillEE.m_sActiveChance	=	skillEE.m_sActiveChance*100/iRate;
			skillEE.m_sActiveChance	=	min(skillEE.m_sActiveChance,lpSkill->m_aExtraEffect[i].m_wLimitActiveChance/100);
		}

		if(_bIsByKarma == FALSE)
		{
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_PET_AND_BEAST)
			{
				if (	!isOwnForce(_lpAttacker))
					continue;
				if	(!isPet() && !isSummonBeast())
					continue;
			}
			else
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_ENEMY )
			{
				if	(isOwnForce(_lpAttacker))
					continue;
			}
			else
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_ENEMY_MONSTER)
			{
				if	(isOwnForce(_lpAttacker) || !isPureMonster())
					continue;
			}
			else
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_OWN_FORCE)
			{
				if	(isEnemy(_lpAttacker))
					continue;
			}
			else
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_ALL_EXCEPT_SELF)
			{
				if	(_lpAttacker->m_wSerialInField	== m_wSerialInField)
					continue;
			}
			else
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_USER_PARTY_MEMBER	)
			{
				if	(!isPlayer())
					continue;
				if	(!_lpAttacker->isPartyMember(this))
					continue;
			}
			else
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_PARTY_MEMBER)
			{
				if	(!_lpAttacker->isPartyMember(this))
					continue;
			}
			else
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_SELF)
			{
				if	(_lpAttacker->m_wSerialInField != m_wSerialInField)
					continue;
			}
			else
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_SELF_AND_ENEMY)
			{
				if	(	_lpAttacker->m_wSerialInField	!=	m_wSerialInField	&&	//	자신도 아니고
					isOwnForce(_lpAttacker)	)	//	적도 아니다.(아군이다)
					continue;
			}
			else
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_PET)
			{
				if (!isPet() || !isOwnForce(_lpAttacker))
					continue;
			}
			else
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_SUMMON_BEAST)
			{
				if (!isSummonBeast() || !isOwnForce(_lpAttacker))
					continue;
			}
			else
			if	(lpExtraEffect->m_bf4TargetMethod	==	eEEAT_ENEMY_PET)
			{
				if (!isPet() || isOwnForce(_lpAttacker))
					continue;
			}
			else
				continue;

		}
		

		if	(lpExtraEffect->m_bf1TargetUndead	==	FALSE && isUndead(TRUE))
			continue;
		if	(lpExtraEffect->m_bf1TargetHuman		==	FALSE && isHuman())
			continue;
		if	(lpExtraEffect->m_bf1TargetDemon		==	FALSE && isDemon())
			continue;
		if	(lpExtraEffect->m_bf1TargetAnimal	==	FALSE && isAnimal())
			continue;
		if	(lpExtraEffect->m_bf1TargetHolyBeast	==	FALSE && isHolyBeast())
			continue;

		DWORD	dwTime	=	iSkillUpkeepTime*100;
		BOOL	bCompulsionKeepTime = FALSE;
		if(_dwKeepTime)		// 강제로 버프시간 넣기...
		{
			dwTime = _dwKeepTime;
			if(iSkillUpkeepTime)
				iSkillUpkeepTime = _dwKeepTime/100;
			bCompulsionKeepTime = TRUE;
		}
		if	(applySkillExtraEffect(_lpAttacker,_lpAbility,_iHitDamage,&skillEE,&dwTime,_iX,_iY,_bIsByAura, bCompulsionKeepTime))
		{	// 부가효과가 성공하면...
			iHitCount++;
			if(lpExtraEffect->m_wExtraEffectImage != 0xffff)
			{	
				m_lpField->addSendEtcWork(m_wSerialInField,eEW_ADD_EXTRA_EFFECT_IMAGE,m_wSerialInField, lpExtraEffect->m_wExtraEffectOutputPart, lpExtraEffect->m_wExtraEffectImage);
			}
			if	(lpExtraEffect->m_bf1IsStop)
				break;
		}

		if	(i==0)
		{
			iUpkeepTimeForEncahntedSkill	=	dwTime;

			if	(iHitCount	==	0	&&	lpSkill->m_bf1IsStopWhenFailedFirstSE)
				break;
		}

		if	(iHitCount	>=	lpSkill->m_wApplyExtraEffectCount)
			break;
	}

	if	(iSkillUpkeepTime)
		iUpkeepTimeForEncahntedSkill	=	iSkillUpkeepTime;

	if	(iHitCount)
	{
		if	(isPlayer())
		{
			if	(lpSkill->m_bf1IsSyncSkillExtraEffectWithPlayer)	//	본인의 정보를 싱크할 필요 있을때 전송한다.
				sendApplySkillExtraEffect(_lpAttacker->m_wSerialInField,_lpAbility, _dwKeepTime);
		}

		if	(_lpAttacker	&&	_lpAttacker->isPlayer()	&&	lpSkill->m_sound.m_wSEE_Success	!=	0xffff)
			_lpAttacker->sendPlaySkillSound(lpSkill->m_wSerial,eSSD_EE_SUCCESS);

		
		int	iUpkeepTime	=	_lpAbility->getUpkeepTime()*dSYNC_FPS;
		if(_dwKeepTime)		// 강제부여 시간이 있으면 .. 그시간으로 대체한다..!
		{
			iUpkeepTime = _dwKeepTime/100*dSYNC_FPS;
		}
		if	(iUpkeepTime	==	0	)
			iUpkeepTime	=	iUpkeepTimeForEncahntedSkill;

		if	(lpSkill->m_bf1BeastBerserker)
			m_wBeastBerserkerTime	=	iUpkeepTime;

		if	(iUpkeepTime)
		{
			if (lpSkill->m_bf1IsOnlySelfEnchantSkill	==	FALSE	||	_lpAttacker->m_wSerialInField	==	m_wSerialInField)
			{
				mDS.m_aiExValue[3]	=	4;
				
				int	iIndex	=	addEnchantedMagic(_lpAbility,iUpkeepTime);

				if	(lpSkill->m_bf1IsPohibitAction && iIndex	!=	0xffff)
					m_wProhibitActionByEnchantedMagic	=	iIndex;
			}
		}

		return	TRUE;
	}

	if	(_lpAttacker	&&	_lpAttacker->isPlayer()	&&	lpSkill->m_sound.m_wSEE_Miss	!=	0xffff)
		_lpAttacker->sendPlaySkillSound(lpSkill->m_wSerial,eSSD_EE_MISS);
	
	return	FALSE;
}

int
cACTOR::applySkillExtraEffectMagicDamage(cACTOR *_lpAttacker,cAbility *_lpAbility , int _iDamage, int _iClass)
{	// 마지막 수정일 : 09.09.29
	if(!_lpAttacker)
		return NULL;

	if(!_lpAbility)
		return NULL;

	if(!_iDamage)
		return NULL;

	CSkill							*lpSkill	=	_lpAbility->getSkill();
	int		iCorrectMagicDamageByAlchemyTitle	=	0;
	BOOL	bIsPureMagicDamageSkill				=	lpSkill->isPureMagicDamageSkill();
	int		iLevelCorrectDamagePercentage		=	0;
	
	if	(bIsPureMagicDamageSkill)
		iCorrectMagicDamageByAlchemyTitle	=	_lpAttacker->m_wAlchemyTitleFactor;
	
	if	(m_lpField->isPVP(_lpAttacker,this))
		iLevelCorrectDamagePercentage		=	(m_iLevel-_lpAttacker->m_iLevel)*30/25;
	
	if	(m_wIsBoss || _lpAttacker->m_wIsBoss)
		iLevelCorrectDamagePercentage	=	(m_iLevel-_lpAttacker->m_iLevel)*30/25;
	
	int iCorrectDamage = 0; 
	switch(_iClass)
	{
	case eDAMAGE_FIRE:
		iCorrectDamage = _lpAttacker->getStrengtheningFireDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;
		break;
	case eDAMAGE_WIND:
		iCorrectDamage = _lpAttacker->getStrengtheningWindDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;
		break;
	case eDAMAGE_WATER:
		iCorrectDamage = _lpAttacker->getStrengtheningWaterDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;
		break;
	case eDAMAGE_EARTH:
		iCorrectDamage = _lpAttacker->getStrengtheningEarthDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;
		break;
	case eDAMAGE_DARK:
		iCorrectDamage = _lpAttacker->getStrengtheningDarkDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;
		break;
		
	case eDAMAGE_LIGHT:
		iCorrectDamage = _lpAttacker->getStrengtheningLightDamage(iCorrectMagicDamageByAlchemyTitle,bIsPureMagicDamageSkill)+iLevelCorrectDamagePercentage;
		break;

	}
	if	(iLevelCorrectDamagePercentage)
	{
		int	iRealDamage	=	_iDamage+_iDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
		_iDamage		+=	_iDamage*iCorrectDamage/100;
		
		if	(_iDamage<=	iRealDamage*40/100)
			_iDamage	=	iRealDamage*40/100;
		if	(_iDamage>=	iRealDamage*180/100)
			_iDamage	=	iRealDamage*180/100;
	}
	else
		_iDamage		+=	_iDamage*iCorrectDamage/100;

	return	_iDamage;
}
//
//	스킬 효과를 적용 시킨다.
BOOL
cACTOR::applySkillExtraEffect(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iHitDamage,CSkillExtraEffectInfo *_lpEffect,DWORD *_lpUpkeepTime,int _iX,int _iY,BOOL _bIsByAura, BOOL _bCompulsionTime)
{		
	int		iEffect,iActiveChance,iSkillLevel=_lpAbility->m_wLevel;
	int		iRandomValue				=	random(100);
	CSkill	*lpSkill					=	_lpAbility->getSkill();
	BOOL	bIsCanNotApplySameEffect	=	FALSE;
	BOOL	bIsPVP						=	FALSE;
	BOOL	bIsNEP						=	isNEPlayer(_lpAttacker);



	bIsPVP				=	m_lpField->isPVP(_lpAttacker,this);

	iEffect				=	_lpEffect->m_wEffect;
	iActiveChance		=	_lpEffect->m_sActiveChance;

	if	(iActiveChance	==	0	)
		iActiveChance	=	100;
	
	if(lpSkill && _lpAbility && lpSkill->m_bf1IsApplyEffectChanceToPlayer && isPlayer())
	{	// 플레이어 부가효과확률 따로적용~
		if(_lpAttacker && _lpAttacker->m_wSerialInField != m_wSerialInField )
		{
			iActiveChance	=	lpSkill->m_wApplyEffectChanceToPlayer + lpSkill->m_wApplyEffectChancePerLevelToPlayer * _lpAbility->m_wLevel;
			
			if(lpSkill->m_wApplyEffectChanceLimitToPlayer)
			{
				iActiveChance	=	min(iActiveChance,lpSkill->m_wApplyEffectChanceLimitToPlayer);
			}
			iActiveChance	/=	100;
		}
	}
	if(_lpAttacker->m_wJob == dJOB_SOUL_BRINGER)
	{	// 영술사일 경우 
		int iCharisma = _lpAttacker->getCharisma();
		iActiveChance += iCharisma * 0.02;	// 카리스마 * 0.02 ..대략 카리스마 1000당 약 20%가 나온다...		
	}

	if	(!_bCompulsionTime && _lpEffect->m_iUpkeepTime)			// 강제시간이 있으면 스킬시간을 사용하지않는다.
		*_lpUpkeepTime	=	_lpEffect->m_iUpkeepTime;


	if(lpSkill)
	{
		bIsCanNotApplySameEffect	=	lpSkill->m_bf1IsCanNotApplySameEffect;
		
		if(lpSkill->m_bf1IsApplyFightingSpirit)
		{
			if(m_wFightingSpiritTime)
			{
				*_lpUpkeepTime	=	*_lpUpkeepTime + (*_lpUpkeepTime * m_iFightingSpiritValue/100.0);
			}
		}
	}

	switch(_lpEffect->m_wEffect)
	{
		case	eSKILL_EE_NO_MORE_FIGHT	:
		{
			m_wNoMoreFightTime	=	*_lpUpkeepTime/100;
			return	TRUE;
		}

		case	eSKILL_EE_FOG_FORM					:
		{
			m_wFogFormTime	=	*_lpUpkeepTime/100;
			return	TRUE;
		}
		case	eSKILL_EE_CONVERSION_ICE			:
		{
			m_wConversionIceTime	=	*_lpUpkeepTime/100;
			return	TRUE;
		}
		case	eSKILL_EE_DECREASE_SWALLOW_CORPES	:
		{
			if	(m_wSwallowCorpesCount)
				--m_wSwallowCorpesCount;

			return	TRUE;
		}
		case	eSKILL_EE_INHALE_ENEMY	:
		{
			int	iRange			=	_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel;
			int	iPeriod			=	_lpEffect->m_aiValue[2];
			int	iPixel			=	_lpEffect->m_aiValue[3];

			*_lpUpkeepTime		/=	100;

			setInhaleEnemy(iRange,iPeriod,iPixel,*_lpUpkeepTime);

			break;
		}

		case	eSKILL_EE_UNFAIR_COVENANT				:
		{
			int	iCurseResistance	=	getCurseResistance();

			if	(_lpEffect->m_aiValue[8])
				iCurseResistance	=	iCurseResistance*_lpEffect->m_aiValue[8]/100/100;

			iActiveChance		=	iActiveChance*(100-iCurseResistance)/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iCurrentLevel	=	-1;
			iSkillLevel			=	71;
			for	(int	i=0;i<10;i++)
			{
				CSkillExtraEffect::cInterruptingArmor	*lpInfo		=	(CSkillExtraEffect::cInterruptingArmor	*)&_lpEffect->m_aiValue[i];
				if	(iSkillLevel	>=	(int)lpInfo->m_bf7BeginLevel	&&	iSkillLevel	<=	(int)lpInfo->m_bf7EndLevel)
				{
					iCurrentLevel	=	i+1;
					break;
				}
			}

			if	(iCurrentLevel	==	-1)
				break;

			*_lpUpkeepTime		/=	100;

			return	setInterruptingArmor(iCurrentLevel,*_lpUpkeepTime);
		}

		case	eSKILL_EE_SOUL_OATH						:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;

			int	iGetCP			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			return	setSoulOath(*_lpUpkeepTime,iGetCP,_lpAbility->m_wSkill);
		}

		case	eSKILL_EE_BLOOD_COMPACT					:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;

			int	iGetHP			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			return	setBloodCompact(*_lpUpkeepTime,iGetHP,_lpAbility->m_wSkill);
		}

		case	eSKILL_EE_IMMUNE_STRANGE_STAUS			:
			*_lpUpkeepTime			/=	100;

			if	(_lpAbility->m_wLevel	>=	100)
				*_lpUpkeepTime	+=	10;
			else
			if	(_lpAbility->m_wLevel	>=	80)
				*_lpUpkeepTime	+=	6;
			else
			if	(_lpAbility->m_wLevel	>=	60)
				*_lpUpkeepTime	+=	3;
			else
			if	(_lpAbility->m_wLevel	>=	30)
				*_lpUpkeepTime	+=	1;

			m_wImmuneStrageStausTime	=	*_lpUpkeepTime;
			return	TRUE;
		case	eSKILL_EE_IMMUNE_PHYSICAL_ATTACK		:
			*_lpUpkeepTime			/=	100;

			if	(_lpAbility->m_wLevel	>=	100)
				*_lpUpkeepTime	+=	10;
			else
			if	(_lpAbility->m_wLevel	>=	80)
				*_lpUpkeepTime	+=	6;
			else
			if	(_lpAbility->m_wLevel	>=	60)
				*_lpUpkeepTime	+=	3;
			else
			if	(_lpAbility->m_wLevel	>=	30)
				*_lpUpkeepTime	+=	1;

			m_wImmunePhysicalAttackTime	=	*_lpUpkeepTime;
			return	TRUE;

		case	eSKILL_EE_REGEN_MONSTER					:
			m_lpField->regenMonsterInArea(0,dCHARACTER_ALL);
			return	TRUE;
			

		case	eSKILL_EE_SWALLOW						:
		{
			int	iHealHP						=	_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*100*iSkillLevel;

			_lpAttacker->recoverHPByAidSkill(_lpAbility->m_wSkill,iHealHP);

			return	operateSwallow(_lpAttacker);
		}

		case	eSKILL_EE_MONSTER_BOMB					:
		{
			int	iPoisonDamage					=	_lpEffect->m_aiValue[0];
			int	iPoisonDamagePerLevel			=	_lpEffect->m_aiValue[1];
			int	iUpkeepPoisonDamageTime			=	_lpEffect->m_aiValue[2]/100;
			int	iDarkDamagePercentage			=	_lpEffect->m_aiValue[3];
			int	iDarkDamagePercentagePerLevel	=	_lpEffect->m_aiValue[4];
			int	iRange							=	_lpEffect->m_aiValue[5];
			int	iShootRange						=	_lpEffect->m_aiValue[6];

			iPoisonDamage			=	iPoisonDamage+iPoisonDamagePerLevel*iSkillLevel;
			iDarkDamagePercentage	=	(iDarkDamagePercentage+iDarkDamagePercentagePerLevel*iSkillLevel)/100;

			m_lpField->monsterBomb(_lpAbility,_lpAttacker,iShootRange,iPoisonDamage,iUpkeepPoisonDamageTime,iDarkDamagePercentage,iRange);
			return	TRUE;
		}

		case	eSKILL_EE_FLY							:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			m_wFlyHeight	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;

			if	(m_wFlyHeight	==	0)
				break;

			m_wFlyTime		=	(*_lpUpkeepTime)/100;

			return	TRUE;
		}
		case	eSKILL_EE_DODGE_MAGIC_DAMAGE			:
		{
			int	iChance		=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;

			m_wDodgeMagicDamageTime		=	(*_lpUpkeepTime)/100;
			m_wDodgeMagicDamageChance	=	iChance;

			if	(*_lpUpkeepTime	==	0)
				m_wDodgeMagicDamageTime	=	0xffff;

			return	TRUE;
		}

		case	eSKILL_EE_IMMUNE_MAGIC_ATTACK			:
			*_lpUpkeepTime			/=	100;

			if	(_lpAbility->m_wLevel	>=	100)
				*_lpUpkeepTime	+=	10;
			else
			if	(_lpAbility->m_wLevel	>=	80)
				*_lpUpkeepTime	+=	6;
			else
			if	(_lpAbility->m_wLevel	>=	60)
				*_lpUpkeepTime	+=	3;
			else
			if	(_lpAbility->m_wLevel	>=	30)
				*_lpUpkeepTime	+=	1;

			m_wImmuneMagicAttackTime	=	*_lpUpkeepTime;
			return	TRUE;

		case	eSKILL_EE_ANNULMENT_CONTRACT			:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			return	releaseContractTime();
		}

		case	eSKILL_EE_CONTRACT						:
		{
			int	iCurseResistance	=	getCurseResistance();

			if	(_lpEffect->m_aiValue[4])
				iCurseResistance	=	iCurseResistance*_lpEffect->m_aiValue[4]/100/100;

			iActiveChance		=	iActiveChance*(100-iCurseResistance)/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;

			int	iOccurPeriod	=	_lpEffect->m_aiValue[0]/100;
			int	iRecoverHP		=	(_lpEffect->m_aiValue[1]+_lpEffect->m_aiValue[2]*_lpAbility->m_wLevel);
			int	iDamageSkill	=	_lpEffect->m_aiValue[3];

			return	setContract(_lpAttacker,*_lpUpkeepTime,iOccurPeriod,iRecoverHP,_lpAbility->m_wSkill,iDamageSkill,_lpAbility->m_wLevel);
		}
		case	eSKILL_EE_BLOODY_CROSS					:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;
			
			return	operateBloodyCross(_lpAbility);
		}

		case	eSKILL_EE_DECLINE_RECOVER_HP_EFFICIENT	:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;

			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			iValue				=	iValue*(100-getCurseResistance())/100;
			iValue				=	min(iValue,_lpEffect->m_aiValue[2]/100);

			return	declineRecoverHPEfficient(*_lpUpkeepTime,iValue);
		}
		
		case	eSKILL_EE_INTERRUPTING_REVIVE			:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;

			return	setInterrutingRevive(*_lpUpkeepTime);
		}

		case	eSKILL_EE_FIRE_DAMAGE				:	//	불대미지
		case	eSKILL_EE_WATER_DAMAGE				:	//	물대미지
		case	eSKILL_EE_WIND_DAMAGE				:	//	바람 대미지
		case	eSKILL_EE_EARTH_DAMAGE				:	//	대지 대미지
		case	eSKILL_EE_LIGHT_DAMAGE				:	//	빛 대미지
		case	eSKILL_EE_DARK_DAMAGE				:	//	어둠 대미지
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			int	iDamage				=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
			int	iReduceResistance	=	(_lpEffect->m_aiValue[4]+_lpEffect->m_aiValue[5]*_lpAbility->m_wLevel)/100;

			{
				int			iCurrentDamage	=	iDamage;
				int			iFireDamage	=0,iWaterDamage=0,iWindDamage=0,iEarthDamage=0,iLightDamage=0,iDarkDamage=0;
				CHitInfo	hitInfo;

				hitInfo.reset(TRUE);

				if	(_lpEffect->m_wEffect	==	eSKILL_EE_FIRE_DAMAGE)
					iFireDamage		=	iCurrentDamage;
				else
				if	(_lpEffect->m_wEffect	==	eSKILL_EE_WATER_DAMAGE)
					iWaterDamage	=	iCurrentDamage;
				else
				if	(_lpEffect->m_wEffect	==	eSKILL_EE_WIND_DAMAGE)
					iWindDamage		=	iCurrentDamage;
				else
				if	(_lpEffect->m_wEffect	==	eSKILL_EE_EARTH_DAMAGE)
					iEarthDamage	=	iCurrentDamage;
				else
				if	(_lpEffect->m_wEffect	==	eSKILL_EE_LIGHT_DAMAGE)
					iLightDamage	=	iCurrentDamage;
				else
				if	(_lpEffect->m_wEffect	==	eSKILL_EE_DARK_DAMAGE)
					iDarkDamage		=	iCurrentDamage;

				getMagicDamageForEE(_lpAttacker,_lpAbility,&hitInfo,iFireDamage,iWaterDamage,iWindDamage,iEarthDamage,iLightDamage,iDarkDamage,iReduceResistance);

				iDamage				=	hitInfo.m_dwMagicDamage;
			}

			reduceHP(iDamage,_lpAttacker);
			addSendHitDamage(_lpAttacker,0,iDamage,dATTACK_RESULT_FIELD_HIT,_lpAbility->m_wSkill);
			
			return	TRUE;
		}

		case	eSKILL_EE_CHAIN_DAMAGE					:	//	"연쇄물리대미지"	,	
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iGap		=	_lpEffect->m_aiValue[3];
			int	iCount		=	(_lpEffect->m_aiValue[4]+_lpEffect->m_aiValue[5]*_lpAbility->m_wLevel)/100;
			int	iLimitCount	=	_lpEffect->m_aiValue[6]/100;
			int	iDamageSum	=	0;
			int	iPhysicalDamage	=	0,iMagicDamage	=	0,iMissCount	=	0,iCriticalCount	=	0,iDoubleCriticalCount = 0;
			int	iMinPhysicalDamage,iMinMagicDamage,iBlockingCount,iInstanceKillCount,iNoActionBlockCount,iDancingBlockCount;
			int	iHardBlowCount	=	0;
			iCount			=	min(iCount,iLimitCount);

			_lpAttacker->getContinuoueAttackDamage(_lpAbility,this,iCount,&iPhysicalDamage,&iMagicDamage,&iMissCount,&iCriticalCount,&iDoubleCriticalCount,&iMinPhysicalDamage,&iMinMagicDamage,&iBlockingCount,&iInstanceKillCount,&iNoActionBlockCount,&iDancingBlockCount, &iHardBlowCount);

			addSendHitChainedDamage(_lpAttacker,_lpAbility,iCount,iMissCount,iGap,iPhysicalDamage,iMagicDamage,iMinPhysicalDamage,iMinMagicDamage,iCriticalCount,iDoubleCriticalCount,iInstanceKillCount,iBlockingCount,iNoActionBlockCount,iDancingBlockCount, iHardBlowCount);

			if	(iPhysicalDamage+iMagicDamage)
			{
				damagedBy(_lpAttacker,_lpAbility,iPhysicalDamage,iMagicDamage,FALSE,FALSE);
//				reduceHP(iPhysicalDamage+iMagicDamage,_lpAttacker);

				return	TRUE;
			}

			if	(_lpAttacker->m_wSOD_Time)
				operateSmellOfDeath(_lpAttacker);

			break;
		}

		case	eSKILL_EE_CHAIN_FIRE_DAMAGE				:	//	"연쇄불대미지"		,
		case	eSKILL_EE_CHAIN_WATER_DAMAGE			:	//	"연쇄물대미지"		,	
		case	eSKILL_EE_CHAIN_WIND_DAMAGE				:	//	"연쇄바람대미지"	,
		case	eSKILL_EE_CHAIN_EARTH_DAMAGE			:	//	"연쇄대지대미지"	,	
		case	eSKILL_EE_CHAIN_LIGHT_DAMAGE			:	//	"연쇄빛대미지"		,	
		case	eSKILL_EE_CHAIN_DARK_DAMAGE				:	//	"연쇄어둠대미지"	,
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iDamage		=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
			int	iDamageRange=	_lpEffect->m_aiValue[2];
			int	iGap		=	_lpEffect->m_aiValue[3];
			int	iCount		=	(_lpEffect->m_aiValue[4]+_lpEffect->m_aiValue[5]*_lpAbility->m_wLevel)/100;
			int	iLimitCount	=	_lpEffect->m_aiValue[6]/100;
			int	iDamageSum	=	0;
			int	iResistance	=	0;
			int	iMinDamage	=	0X7fffffff;
			int iBlockingCount=	0;
			int iNoActionBlockCount	=	0;
			int iDancingBlockCount	=	0;

			iCount			=	min(iCount,iLimitCount);

			for	(int i=0;i<iCount;i++)
			{
				int			iCurrentDamage	=	iDamage-iDamageRange+random(iDamageRange*2);
				int			iFireDamage	=0,iWaterDamage=0,iWindDamage=0,iEarthDamage=0,iLightDamage=0,iDarkDamage=0;
				CHitInfo	hitInfo;

				hitInfo.reset(TRUE);

				if	(_lpEffect->m_wEffect	==	eSKILL_EE_CHAIN_FIRE_DAMAGE)
					iFireDamage		=	iCurrentDamage;
				else
				if	(_lpEffect->m_wEffect	==	eSKILL_EE_CHAIN_WATER_DAMAGE)
					iWaterDamage	=	iCurrentDamage;
				else
				if	(_lpEffect->m_wEffect	==	eSKILL_EE_CHAIN_WIND_DAMAGE)
					iWindDamage		=	iCurrentDamage;
				else
				if	(_lpEffect->m_wEffect	==	eSKILL_EE_CHAIN_EARTH_DAMAGE)
					iEarthDamage	=	iCurrentDamage;
				else
				if	(_lpEffect->m_wEffect	==	eSKILL_EE_CHAIN_LIGHT_DAMAGE)
					iLightDamage	=	iCurrentDamage;
				else
				if	(_lpEffect->m_wEffect	==	eSKILL_EE_CHAIN_DARK_DAMAGE)
					iDarkDamage		=	iCurrentDamage;

				correctValueEffectDuelServer(&iFireDamage , lpSkill);

				getMagicDamageForEE(_lpAttacker,_lpAbility,&hitInfo,iFireDamage,iWaterDamage,iWindDamage,iEarthDamage,iLightDamage,iDarkDamage);

				if	(hitInfo.isBlock())
					iBlockingCount	++;
				if	(hitInfo.isNoActionBlock())
					iNoActionBlockCount	++;
				if	(hitInfo.isDancingBlockerBlock())
					iDancingBlockCount	++;

				if	(hitInfo.m_dwMagicDamage	<	(DWORD)iMinDamage)
					iMinDamage		=	hitInfo.m_dwMagicDamage;

				iDamageSum			+=	hitInfo.m_dwMagicDamage;
			}

			addSendHitChainedDamage(_lpAttacker,_lpAbility,iCount,0,iGap,0,iDamageSum,0,iMinDamage,0,0,0,iBlockingCount,iNoActionBlockCount,iDancingBlockCount,0);

			if	(iDamageSum)
			{
				reduceHP(iDamageSum,_lpAttacker);
				return	TRUE;
			}

			break;
		}
		case	eSKILL_EE_DRAW_BODY						:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iDistance		=	_lpEffect->m_aiValue[0];
			int	iAngle			=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,_lpAttacker->m_pos.x,_lpAttacker->m_pos.y);

			int	iCurRange		=	GetOvalRange(m_pos.x,m_pos.y,_lpAttacker->m_pos.x,_lpAttacker->m_pos.y);

			iCurRange			=	(int)sqrt(iCurRange);

			if	(iCurRange		<=	iDistance)
				break;

			iDistance			=	iCurRange-iDistance;

			thrust(_lpAttacker->m_pos.x,_lpAttacker->m_pos.y,iDistance,TRUE,180);

			return	TRUE;
		}
		case	eSKILL_EE_SMELL_OF_DEATH					:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iReduceResistance			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int	iReduceResistanceUpkeepTime	=	_lpEffect->m_aiValue[3]/100;
			int	iLimitReduceResistance		=	_lpEffect->m_aiValue[2]/100;
			int	iDarkDamage					=	(_lpEffect->m_aiValue[4]+_lpEffect->m_aiValue[5]*_lpAbility->m_wLevel);
			int	iLimitReduceCurseResistance	=	_lpEffect->m_aiValue[6]/100;

			iReduceResistance				=	min(iReduceResistance,iLimitReduceResistance);

			int	iReduceCurseResistance		=	min(iReduceResistance,iLimitReduceCurseResistance);

			float	fIntForDamage			=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage();
			iDarkDamage						+=	(int)(iDarkDamage*_lpAttacker->getIntelligence()*fIntForDamage);

			//	죽음의 향기 어둠 강화 효과 적용
			iDarkDamage = applySkillExtraEffectMagicDamage(_lpAttacker , _lpAbility , iDarkDamage, eDAMAGE_DARK); // 09.09.29

			*_lpUpkeepTime			/=	100;

			return	setSmellOfDeath((*_lpUpkeepTime),iDarkDamage,iReduceResistanceUpkeepTime,iReduceResistance,_lpAbility->m_wSkill,iReduceCurseResistance);
		}

		case	eSKILL_EE_PLOT_OF_SHADOW					:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			int		iTargetDamage	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
			int		iCasterDamage	=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*_lpAbility->m_wLevel);
			int		iLimitTargetCount=	_lpEffect->m_aiValue[4]/100;

			float	fIntForDamage	=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage();
			iTargetDamage			+=	(int)(iTargetDamage*_lpAttacker->getIntelligence()*fIntForDamage);

			//	음모의 그림자 어둠 강화 효과 적용
			iTargetDamage = applySkillExtraEffectMagicDamage(_lpAttacker , _lpAbility , iTargetDamage, eDAMAGE_DARK); // 09.09.29

			*_lpUpkeepTime			/=	100;

			return	setPlotOfShadow(_lpAttacker,(*_lpUpkeepTime),iTargetDamage,iCasterDamage,_lpAbility->m_wSkill,iLimitTargetCount);
		}

		case	eSKILL_EE_SELF_REPROOF						:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			(*_lpUpkeepTime)	=	(*_lpUpkeepTime)*(100-getCurseResistance())/100;
			int	iDamage			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
			float	fIntForDamage=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage();
			iDamage				+=	(int)(iDamage*_lpAttacker->getIntelligence()*fIntForDamage);

			//	자책감 스킬 어둠 강화 효과 적용
			iDamage			= applySkillExtraEffectMagicDamage(_lpAttacker , _lpAbility , iDamage, eDAMAGE_DARK); // 09.09.29

			return	setSelfReproof(_lpAttacker,(*_lpUpkeepTime),iDamage,_lpAbility->m_wSkill);
		}

		case	eSKILL_EE_TORTURE							:
		{
			if	(iActiveChance	<	100)
			{
				int	iResistanceValue=	getCurseResistance();

				iActiveChance		=	iActiveChance*(100-iResistanceValue)/100;

				if	(iRandomValue	>=	iActiveChance)
					break;
			}

			*_lpUpkeepTime		/=	100;

			int		iDamage		=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel);
			float	fIntForDamage=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage();
			iDamage				+=	(int)(iDamage*_lpAttacker->getIntelligence()*fIntForDamage);

			//	고문 스킬 어둠 강화 효과 적용
			iDamage			= applySkillExtraEffectMagicDamage(_lpAttacker , _lpAbility , iDamage, eDAMAGE_DARK); // 09.09.29

			return	setTorture(_lpAttacker,*_lpUpkeepTime,iDamage);
		}

		case	eSKILL_EE_DEGENERATE						:	
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			(*_lpUpkeepTime)	/=	100;
			int		iPropertion	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;

			return	setDegenerate(_lpAttacker,(*_lpUpkeepTime),iPropertion,_lpAbility->m_wSkill);
		}
		case	eSKILL_EE_PHANTOM_IMPULSE					:	
		{
			if	(iRandomValue		>=	iActiveChance)
				break;

			int		iPropertion		=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int		iLimitPropertion=	_lpEffect->m_aiValue[2]/100;

			iPropertion				=	min(iPropertion,iLimitPropertion);

			return	operatePhantomImpulse(_lpAttacker,iPropertion,_lpAbility->m_wSkill);
		}
		case	eSKILL_EE_TERRIBLE_DREAMS					:	
		{
			if	(iRandomValue		>=	iActiveChance)
				break;

			(*_lpUpkeepTime)		/=	100;

			int		iDamage			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
			int		iActivePeriod	=	_lpEffect->m_aiValue[2];

			float	fIntForDamage	=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage();
			iDamage					+=	(int)(iDamage*_lpAttacker->getIntelligence()*fIntForDamage);

			//	가위 스킬 어둠 강화 효과 적용.
			iDamage			= applySkillExtraEffectMagicDamage(_lpAttacker , _lpAbility , iDamage, eDAMAGE_DARK); // 09.09.29

			return	setTerribleDream(_lpAttacker,(*_lpUpkeepTime),iDamage,_lpAbility->m_wSkill,iActivePeriod);
		}

		case	eSKILL_EE_HWABYUNG							:	
		{// 화병 스킬
			int	iCurseResistance	=	getCurseResistance();

			if	(_lpEffect->m_aiValue[5])
				iCurseResistance	=	iCurseResistance*_lpEffect->m_aiValue[5]/100/100;

			iActiveChance			=	iActiveChance*(100-iCurseResistance)/100;

			if	(iRandomValue		>=	iActiveChance)
				break;

			if	(m_wCorrectFinalDamage	==	1)	//	댐쥐 무조건 1빠지게 하는 애.. 임시로 한건데.. 설마 문제 안되겠지.. - -
				break;

			(*_lpUpkeepTime)		=	max((*_lpUpkeepTime),_lpEffect->m_aiValue[6]);
			(*_lpUpkeepTime)		/=	100;

			int	iDarkDamage			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
			int	iFireDamage			=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*_lpAbility->m_wLevel)+random(_lpEffect->m_aiValue[4]*2)-_lpEffect->m_aiValue[4];

			float	fIntForDamage	=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage();
			iDarkDamage				+=	(int)(iDarkDamage*_lpAttacker->getIntelligence()*fIntForDamage);
			iFireDamage				+=	(int)(iFireDamage*_lpAttacker->getIntelligence()*fIntForDamage);

			
			iDarkDamage = applySkillExtraEffectMagicDamage(_lpAttacker , _lpAbility , iDarkDamage, eDAMAGE_DARK); // 09.09.29
			iFireDamage = applySkillExtraEffectMagicDamage(_lpAttacker , _lpAbility , iFireDamage, eDAMAGE_FIRE); // 09.09.29
			return	setHwabyung(_lpAttacker,*_lpUpkeepTime,iDarkDamage,iFireDamage,_lpAbility->m_wSkill);
		}
		case	eSKILL_EE_BLOOD_DRAIN						:
		{
			if	(iRandomValue			>=	iActiveChance)
				break;

			*_lpUpkeepTime				/=	100;

			int	iBloodDrainPercentage	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			iBloodDrainPercentage		=	iBloodDrainPercentage*(100-getCurseResistance())/100;

			return	setBloodDrain(_lpAttacker,*_lpUpkeepTime,iBloodDrainPercentage,_lpAbility->m_wSkill);
//	타격치의 1+0.5Lv% 를 헌혈 함.
		}
			
		case	eSKILL_EE_IMPULSE							:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iMinUpkeepTime	=	_lpEffect->m_aiValue[2]/100;
			*_lpUpkeepTime		/=	100;

			*_lpUpkeepTime		=	max(*_lpUpkeepTime,iMinUpkeepTime);

			int		iDamage		=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			float	fIntForDamage=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage();
			iDamage				+=	(int)(iDamage*_lpAttacker->getIntelligence()*fIntForDamage);

			//	강신술사 충동 스킬에 어둠 강화효과 적용
			iDamage = applySkillExtraEffectMagicDamage(_lpAttacker , _lpAbility , iDamage, eDAMAGE_DARK); // 09.09.29

			return	setImpulse(_lpAttacker,*_lpUpkeepTime,iDamage,_lpAbility->m_wSkill);
		}
		case	eSKILL_EE_DAMAGE_POOL						:
		{
			int		iEfficient	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int		iMinEfficient=	_lpEffect->m_aiValue[2]/100;
			iEfficient			=	max(iEfficient,iMinEfficient);
			int		iLinkCount	=	_lpEffect->m_aiValue[3]*_lpAbility->m_wLevel;
			int		iMaxLinkCount=	_lpEffect->m_aiValue[4]/100;

			if	(iLinkCount	%	100)
				iLinkCount	=	(iLinkCount/100+1)*100;

			iLinkCount			/=	100;
			iLinkCount			=	min(iLinkCount,iMaxLinkCount);

			int		iRange		=	_lpEffect->m_aiValue[5]+_lpEffect->m_aiValue[6]*_lpAbility->m_wLevel;

			m_damagePool.set(*_lpUpkeepTime/100,iEfficient,iLinkCount,iRange);

			return	TRUE;
		}

		case	eSKILL_EE_MAKE_ILLUSION						:
		{
			int		iRemainHPPercentage	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int		iHP					=	_lpAttacker->getMaxHP()*iRemainHPPercentage/100;
			BOOL	bIsFirstTarget		=	_lpEffect->m_aiValue[2];
			int		iLimitCount			=	_lpEffect->m_aiValue[3];
			int		iIllusionJob		=	_lpEffect->m_aiValue[4];

			*_lpUpkeepTime				/=	100;

			return	appointmentMakeIllusion(_lpAbility->m_wSkill,*_lpUpkeepTime,iHP,bIsFirstTarget,iIllusionJob,iLimitCount,_iX,_iY);
		}

		case	eSKILL_EE_ATTACK_COMMAND					:
		{
			if	(_lpAttacker->m_lpTarget)
			{
				for (int i=0;i<dOWN_PET_COUNT;i++)
				{
					cACTOR	*lpPet	=	_lpAttacker->getPetActor(i);

					if	(!lpPet || lpPet->isDeath())
						continue;

					lpPet->commandTamedMonster(eTMD_ATTACK_ONE,_lpAttacker->m_lpTarget);
				}
			}
			return	TRUE;
		}

		case	eSKILL_EE_TRANS_POWER						:
		{
			if	(!_lpAttacker)
				break;

			_lpAttacker->setReversalPowerTime(*_lpUpkeepTime/100);

			int	iCurseResistance	=	getCurseResistance();

			if	(_lpEffect->m_aiValue[0])
				iCurseResistance	=	iCurseResistance*_lpEffect->m_aiValue[0]/100/100;

			iActiveChance		=	iActiveChance*(100-iCurseResistance)/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iTime			=	*_lpUpkeepTime;

			iTime				=	iTime*(100-getDeclinePowerResistance())/100;

			setReversalPowerTime(iTime/100);

			*_lpUpkeepTime		=	iTime;

			return	TRUE;
		}

		case	eSKILL_EE_TRANS_FROG						:
		{
			cJOB	*lpJob	=	getJob();
			int		iRemainHP=	max(getRemainHPRate(),1);

			if	(!lpJob)	//	가능한 등급
				break;

			int	iApplyChance			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int	iApplyChanceForSemiBoss	=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*_lpAbility->m_wLevel)/100;

			iApplyChance				=	min(iApplyChance,_lpEffect->m_aiValue[2]/100);
			iApplyChanceForSemiBoss		=	min(iApplyChance,_lpEffect->m_aiValue[5]/100);

			if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_BOSS1)		//	보스급은 안됨
			{
				_lpAttacker->sendEtcWork(eEW_CAN_NOT_TRANS_TARGET_TO_FROG,0xffff);	//	불가한 적
				break;
			}
			else
			if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_SEMI_BOSS1)	//	세미 보스급
			{
				if	(iRemainHP	>	iApplyChanceForSemiBoss)
				{
					_lpAttacker->sendEtcWork(eEW_CAN_NOT_TRANS_TARGET_TO_FROG,0xfffe);	//	체력을 더 빼야함
					break;
				}
			}
			else
			if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_NORMAL1)		//	가능한 등급
			{
				if	(iRemainHP	>	iApplyChance)
				{
					_lpAttacker->sendEtcWork(eEW_CAN_NOT_TRANS_TARGET_TO_FROG,0xfffe);	//	체력을 더 빼야함
					break;
				}
			}

			iActiveChance		=	_lpEffect->m_sActiveChance*100/iRemainHP;
			
			int	iResistance		=	getCurseResistance();
			iResistance			=	max(iResistance,getDeclinePowerResistance());
			iResistance			=	max(iResistance,getStrangeStatusResistance(TRUE));
			iActiveChance		=	iActiveChance*(100-iResistance)/100;

			if	(bIsPVP)
			{
				int	iCheckLevel	=	(10000-50*_lpAbility->m_wLevel)/100;
				iCheckLevel		=	min(iCheckLevel,50);

				if	(getLevel()	>=	_lpAttacker->getLevel()-iCheckLevel)
				{
					_lpAttacker->sendEtcWork(eEW_CAN_NOT_TRANS_TARGET_TO_FROG,iCheckLevel);

					break;
				}
			}
//			공주 공주의 입맞춤 길드전 제한 사항 추가 : 길드전에서는 100-0.5Lv (min 50Lv) 이상 낮은 레벨의 캐릭터에게만 시전 가능

			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;

			return	transToFrog(_lpAttacker,_lpAbility,*_lpUpkeepTime);
		}

		case	eSKILL_EE_TRANS_FROG2						:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iResistance		=	min(getCurseResistance(),_lpEffect->m_aiValue[0]/100);

			*_lpUpkeepTime		=	*_lpUpkeepTime*(100-iResistance)/100;

			BOOL	bIsSuccess	=	transToFrog(_lpAttacker,_lpAbility,*_lpUpkeepTime);

			if	(_lpEffect->m_aiValue[1])
				_lpAttacker->reaimTarget(this);

			return	bIsSuccess;
		}

		case	eSKILL_EE_TRANS_FAT_GIRL					:
		{
			*_lpUpkeepTime		/=	100;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iActivePeriod	=	(iValue1+iValue2*_lpAbility->m_wLevel);

			return	transToFatGirl(_lpAttacker,_lpAbility,*_lpUpkeepTime,iActivePeriod);
		}

		case	eSKILL_EE_TRANS_TO_WEAPON					:	//	{	"무기변신"			
		{
			int	iValue1					=	_lpEffect->m_aiValue[0];
			int	iValue2					=	_lpEffect->m_aiValue[1];
			int	iLimitDamage			=	_lpEffect->m_aiValue[2]/100;
			int	iValue4					=	_lpEffect->m_aiValue[3];
			int	iValue5					=	_lpEffect->m_aiValue[4];
			int	iDamagePercentageFactor	=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;
			int	iAddMagicDamageFactor	=	(iValue4+iValue5*_lpAbility->m_wLevel)/100;

			iDamagePercentageFactor		=	min(iDamagePercentageFactor,iLimitDamage);

			*_lpUpkeepTime		/=	100;

			return	transToWeapon(_lpAttacker,_lpAbility,*_lpUpkeepTime,iAddMagicDamageFactor,iDamagePercentageFactor);
		}

		case	eSKILL_EE_TRANS_TO_RABBIT					:
		{
			*_lpUpkeepTime		/=	100;
			return	transToRabbit(_lpAbility,*_lpUpkeepTime);
		}
		case	eSKILL_EE_JUMP_JUMP							:
		{
			*_lpUpkeepTime		/=	100;

			return	jumppingSkipping(_lpAbility,*_lpUpkeepTime);
		}
		case	eSKILL_EE_ATTACK_UNDER_FEIGN_DATH			:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int	iLimit			=	(_lpEffect->m_aiValue[2])/100;
			int	iSemiBossCorrect=	(_lpEffect->m_aiValue[3])/100;
			int	iBossCorrect	=	(_lpEffect->m_aiValue[4])/100;

			iValue				=	min(iValue,iLimit);

			{
				int		iAttackRange	=	getAttackRange(_lpAbility);
				cACTOR	*lpTarget		=	m_lpField->findNeareastEnemy(m_wTeam,m_iLocalPart,m_pos.x,m_pos.y,iAttackRange);//	가장 근접해 있는 누군가를 찾는다.

				if	(!lpTarget)
					break;

				{
					cJOB	*lpJob	=	lpTarget->getJob();

					if	(!lpJob)
						return	FALSE;

//	버근데.. 그냥 둔다.. - - 아래 주석을 풀면 버그 수정됨
//					if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_BOSS1)	//	가능한 등급
//						iValue	=	iValue*iBossCorrect/100;
//					else
//					if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_SEMI_BOSS1)	//	가능한 등급
//						iValue	=	iValue*iSemiBossCorrect/100;
				}
			}

			return	trickKick(_lpAbility,iValue);
		}
		case	eSKILL_EE_FEIGN_DEATH						:
		{
			*_lpUpkeepTime		/=	100;

			return	feignDeath(_lpAbility,*_lpUpkeepTime);
		}
		
		case	eSKILL_EE_DECREASE_ALL_RESISTANCE			:
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iValue3			=	_lpEffect->m_aiValue[2];
			int	iValue4			=	_lpEffect->m_aiValue[3];
			int	iLimit			=	(iValue3+iValue4*_lpAbility->m_wLevel)/100;
			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			if	(iLimit)
				iValue			=	min(iValue,iLimit);

			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;
			iValue				=	-iValue;

			m_allMagicResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			m_declinePowerResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			m_strangeStatusResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			m_curseResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			sendEtcWork(eEW_CHANGE_MAGIC_RESISTANCE,m_allMagicResistance.m_sCorrectValue,m_allMagicResistance.m_wTime);

			return	TRUE;
		}

		case	eSKILL_EE_EXPLOSION_BOTTLE					:
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iEffcient		=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			return	explosionBottle(_lpAttacker,_lpAbility,iEffcient);
		}

		case	eSKILL_EE_INSTANCE_KILL						:
		{
			if	(bIsPVP)
			{
				if	(bIsNEP)
					iActiveChance	=	iActiveChance*dPVP_CORRECT_INSTANCE_KILL_CAHNCE_NE/100;
				else
					iActiveChance	=	iActiveChance*dPVP_CORRECT_INSTANCE_KILL_CAHNCE/100;
			}

			int	iResistanceValue	=	max(m_bInstanceKillResistance,getCurseResistance());

			if	(bIsPVP)
				iResistanceValue	=	m_bInstanceKillResistance+getCurseResistance();

			iActiveChance		=	iActiveChance*(100-iResistanceValue)/100;

			cJOB	*lpJob		=	getJob();

			if	(lpJob)
				if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_SEMI_BOSS1)
					iActiveChance	=	0;
			
			int	iDamage			=	m_iHP;

			if	(bIsPVP)
			{
				if	(bIsNEP)	//	천적이다.
				{
					iDamage			=	m_iHP*dPVP_INSTANCE_KILL_FACTOR_NE/100;
					iActiveChance	=	iActiveChance*dPVP_CORRECT_INSTANCE_KILL_CAHNCE_NE/100;
				}
				else
				{
					iDamage			=	m_iHP*dPVP_INSTANCE_KILL_FACTOR/100;
					iActiveChance	=	iActiveChance*dPVP_CORRECT_INSTANCE_KILL_CAHNCE/100;
				}
			}

			if	(iRandomValue	>=	iActiveChance)
				break;

			damagedBy(_lpAttacker,_lpAbility,iDamage,0,FALSE,FALSE);
			m_bf1IsInstanceKill	=	TRUE;

			break;
		}

		case	eSKILL_EE_DISARM_TRAP						:	//	"함정해체"			::
		{
			BOOL	bIsSuccess		=	TRUE;
			int		iDisarmLevel	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;

			if	(iRandomValue	>=	iActiveChance)
				bIsSuccess	=	FALSE;

			int		iExplosionChance=	(75*100-(getLuck()*100/20)-50*_lpAbility->m_wLevel)/100;

//			실패시 75%-(시전자의 운/20)%-0.5Lv%의 확률로 함정 작동 (min 5%)
			return	tryDisarmTrap(m_bf3TargetObjectType,m_bf13TargetObject,iDisarmLevel,iExplosionChance,*_lpUpkeepTime/100,bIsSuccess);
		}

		case	eSKILL_EE_UNLOCK_ARCA						:	//	"잠긴상자열기"		::
		{
			if	(m_bf3TargetObjectType	!=	eTARGET_OBJECT_ARCA)
				return	FALSE;

			cArca	*lpArca				=	g_arcaManager.get(m_bf13TargetObject);

			if	(!lpArca || lpArca->m_bLockLevel == 0)
				return	FALSE;

			int	iUnlockLevel			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;

			if	(iUnlockLevel	<	lpArca->m_bLockLevel)
			{
				sendArcaWork(m_bf13TargetObject,eAW_UNLOCK_FAILED_BY_LOW_UNLOCK_LEVEL,lpArca->m_bLockLevel);
				return	TRUE;
			}

			if	(iRandomValue	>=	iActiveChance)
			{
				int			iDamage		=	lpArca->m_bLockLevel*_lpEffect->m_aiValue[2];

				CHitInfo	info;

				info.reset(TRUE);

				info.m_dwPhysicalDamage	=	iDamage;
				info.m_dwMagicDamage	=	0;
				info.setHit();

				addSendHitDamage(this,info.m_dwPhysicalDamage,info.m_dwMagicDamage,info.m_dwResultField,_lpAbility->m_wSkill);

				reduceHP(iDamage,_lpAttacker);
				sendArcaWork(m_bf13TargetObject,eAW_UNLOCK_FAILED);

				break;
			}

			lpArca->unlock(this);

			return	TRUE;
		}
		case	eSKILL_EE_UNLOCK_DOOR						:	//	"잠긴문열기"		::
		{
			if	(m_bf3TargetObjectType	!=	eTARGET_OBJECT_DOOR)
				return	FALSE;

			cAreaDoorInfo	*lpDoor		=	(cAreaDoorInfo	*)m_lpField->getArea(m_bf13TargetObject);

			if	(!lpDoor)
				break;
			if	(lpDoor->m_bf1IsUnlocked)
				break;

			int		iUnlockLevel		=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;

			if	(iUnlockLevel	<	lpDoor->m_bUnlockLevel)
			{
				sendDoorMessage(m_bf13TargetObject,0xffff,0xffff,eDWM_CAN_NOT_UNLOCK_DOOR_BY_LOW_UNLOCK_LEVEL,NULL);

				return	TRUE;
			}

			if	(iRandomValue	>=	iActiveChance)
			{
				int			iDamage		=	lpDoor->m_bUnlockLevel*_lpEffect->m_aiValue[2];

				CHitInfo	info;

				info.reset(TRUE);

				info.m_dwPhysicalDamage	=	iDamage;
				info.m_dwMagicDamage	=	0;
				info.setHit();

				addSendHitDamage(NULL,info.m_dwPhysicalDamage,info.m_dwMagicDamage,info.m_dwResultField,_lpAbility->m_wSkill);

				reduceHP(iDamage,_lpAttacker);
				sendDoorMessage(m_bf13TargetObject,0xffff,0xffff,eDWM_UNLOCK_BY_SKILL_FAILED);

				break;
			}

			*_lpUpkeepTime	/=	100;

			lpDoor->unlock(*_lpUpkeepTime);
			lpDoor->sendDoorStatus();
			m_lpField->addSendDoorMessage(m_bf13TargetObject,0xffff,_lpAbility->m_wSkill,eDWM_UNLOCK_BY_SKILL,this);
			
			return	TRUE;
		}
		case	eSKILL_EE_DETECT_TRAP						:	//	"함정탐지"			::
		{
			int	iValue				=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime			/=	100;
			m_detectTrap.correctValue(iValue,*_lpUpkeepTime);
			return	TRUE;
		}
		case	eSKILL_EE_DETECT_SECRET_OBJECT				:	//	"비밀오브젝트탐지"	::
		{
			int	iValue				=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime			/=	100;
			m_detectSecretObject.correctValue(iValue,*_lpUpkeepTime);
			return	TRUE;
		}
		case	eSKILL_EE_PICKPOCKET						:	//	"소매치기"			::
		{
			if	(m_bf1IsStolenGold)
			{
				sendRegistServerMessage(eRSM_ALREADY_STEAL_GOLD_MONSTER);

				break;
			}

			if	(iRandomValue	>=	iActiveChance)
				break;

			_lpAttacker->stealGold(this);

			addSendSkillEffect(_lpAbility,eSE_SKILL);

			return	TRUE;
		}
		case	eSKILL_EE_ROBBERY							:	//	"아이템강탈"		::
		{
			if	(m_bf1IsStolenItem)
			{
				sendRegistServerMessage(eRSM_ALREADY_STEAL_ITEM_MONSTER);

				break;
			}

			if	(iRandomValue	>=	iActiveChance)
				break;

			_lpAttacker->stealItem(this);

			return	TRUE;
		}
		case	eSKILL_EE_NEUTRALIZE_POISON					:
		{
			int	iValue				=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			if (m_wPoisonTime)
			{
				if (iValue	>=	m_iPoisonDamagePerSec)
					m_wPoisonTime			=	0;
				else
					m_iPoisonDamagePerSec	-=	iValue;
			}
			return	TRUE;
		}
		case	eSKILL_EE_NEUTRALIZE_FREEZE_STUN			:
		{
			if	(m_wFreezeTime	||	m_wStunTime)
			{
				int	iValue		=	max(m_wFreezeTime,m_wStunTime);
				iValue			=	max(iValue/dSYNC_FPS,1);
				m_wColdTime		=	max(m_wColdTime,iValue);
			}

			return	TRUE;
		}
		case	eSKILL_EE_INVISIBLE							:
		{
			*_lpUpkeepTime			/=	100;

			if	(m_sInvisivilityTime	<	0)
				break;

			if	(m_sInvisivilityTime	>=	*_lpUpkeepTime)
				break;

			m_wFoundOutChanceInInvisible			=	max(iActiveChance*100,_lpEffect->m_aiValue[0]);
			m_wFoundOutChanceInInvisibleForStand	=	_lpEffect->m_aiValue[1];

			if(_lpAbility->getSkill()->m_bf1IsShadowHideSkill)
			{
				m_bIsHideByShadowHideSkill				= TRUE;
				m_sShadowHideAddCriticalChance			= _lpEffect->m_aiValue[6]*_lpAbility->m_wLevel;
				m_wShadowHideAddPhysicalPowerPercent	= _lpEffect->m_aiValue[8]*_lpAbility->m_wLevel;

				if(_lpEffect->m_aiValue[7])
					m_sShadowHideAddCriticalChance			= min(m_sShadowHideAddCriticalChance,_lpEffect->m_aiValue[7]);

				if(_lpEffect->m_aiValue[9])
					m_wShadowHideAddPhysicalPowerPercent	= min(m_wShadowHideAddPhysicalPowerPercent,_lpEffect->m_aiValue[9]);

				m_sShadowHideAddCriticalChance			/= 100;
				m_wShadowHideAddPhysicalPowerPercent	/= 100;
			}

			m_sInvisivilityTime						=	*_lpUpkeepTime;
			m_bf1IsDisableInvisibleByAttack			=	_lpEffect->m_aiValue[2];
			m_wHidingLevel							=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*_lpAbility->m_wLevel)/100;

			mDS.m_aiExValue[0]	=	3;
			updateEnchantedMagic();
			addSendActorMagicEffectStatus();

			if	(_lpEffect->m_aiValue[5])
				m_lpField->stopAllTargetedAction(this,TRUE,TRUE);

			return	TRUE;
		}
		
		case	eSKILL_EE_RESISTANCE_KNOCK_OUT				:
			if (iRandomValue	>=	iActiveChance)
				break;

			m_bf1IsCancelKnockOut	=	TRUE;
			return	TRUE;

		case	eSKILL_EE_RESISTANCE_KNOCK_BACK				:
			if (iRandomValue	>=	iActiveChance)
				break;

			m_bf1IsCancelKnockBack	=	TRUE;
			return	TRUE;

		case	eSKILL_EE_INCREASE_RIGHT_PUNCH_SKILL_LEVEL	:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			iValue				=	iValue/100;

			*_lpUpkeepTime		/=	100;

			m_correctRightPunchSkillLevel.correctValue(iValue,*_lpUpkeepTime);

			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_LEFT_PUNCH_SKILL_LEVEL	:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			iValue				=	iValue/100;

			*_lpUpkeepTime		/=	100;

			m_correctLeftPunchSkillLevel.correctValue(iValue,*_lpUpkeepTime);
			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_RIGHT_KICK_SKILL_LEVEL	:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			iValue				=	iValue/100;

			*_lpUpkeepTime		/=	100;

			m_correctRightKickSkillLevel.correctValue(iValue,*_lpUpkeepTime);
			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_LEFT_KICK_SKILL_LEVEL	:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			iValue				=	iValue/100;

			*_lpUpkeepTime		/=	100;

			m_correctLeftKickSkillLevel.correctValue(iValue,*_lpUpkeepTime);
			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_DOWN_KICK_SKILL_LEVEL	:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			iValue				=	iValue/100;

			*_lpUpkeepTime		/=	100;

			m_correctDownKickSkillLevel.correctValue(iValue,*_lpUpkeepTime);
			return	TRUE;
		}

		case	eSKILL_EE_CONFUSE				:	//	혼란
			iActiveChance		=	iActiveChance*(100-getConfuseResistance())/100;

			if (iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		-=	*_lpUpkeepTime*getConfuseResistance()/100;
			*_lpUpkeepTime		/=	100;

			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			if	(setCofuseTime(*_lpUpkeepTime,_lpAttacker))
				*_lpUpkeepTime	=	m_wConfuseTime*dSYNC_FPS;
			return	TRUE;

		case	eSKILL_EE_BREAK_ARMOR			:	//	갑옷파괴
			if (bIsPVP)
				return	TRUE;

			iActiveChance		=	iActiveChance*(100-getCurseResistance())/100;

			if (iRandomValue	>=	iActiveChance)	break;

			*_lpUpkeepTime			-=	*_lpUpkeepTime*getCurseResistance()/100;

			*_lpUpkeepTime		/=	100;

			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			m_wBreakedArmorTime	=	max(m_wBreakedArmorTime,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_wBreakedArmorTime*dSYNC_FPS;
			return	TRUE;

		case	eSKILL_EE_BREAK_WEAPON			:	//	무기파괴
			if (bIsPVP)
				return	TRUE;

			iActiveChance		=	iActiveChance*(100-getCurseResistance())/100;

			if (iRandomValue	>=	iActiveChance)	break;

			*_lpUpkeepTime		-=	*_lpUpkeepTime*getCurseResistance()/100;
			*_lpUpkeepTime		/=	100;

			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			m_wBreakedWeaponTime=	max(m_wBreakedWeaponTime,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_wBreakedWeaponTime*dSYNC_FPS;
			return	TRUE;

		case	eSKILL_EE_STUN					:	//	경직
		{
			iActiveChance		=	iActiveChance*(100-getStunResistance())/100;

			if	(bIsPVP)
				iActiveChance	=	min(iActiveChance,dPVP_TOP_STUN_CHANCE);

			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		-=	*_lpUpkeepTime*getStunResistance()/100;
			*_lpUpkeepTime		=	*_lpUpkeepTime*dSYNC_FPS/100;

			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			if	(bIsPVP)
			{
				m_wStunTime			=	max(m_wStunTime,*_lpUpkeepTime);

				if	(isNEPlayer(_lpAttacker))	//	천적이다.
					m_wStunTime			=	m_wStunTime*dPVP_CORRECT_STUN_TIME_NE/100;
				else
					m_wStunTime			=	m_wStunTime*dPVP_CORRECT_STUN_TIME/100;

				*_lpUpkeepTime		=	m_wStunTime;
			}
			else
			{
				m_wStunTime			=	max(m_wStunTime,*_lpUpkeepTime);
				*_lpUpkeepTime		=	m_wStunTime;
			}
			return	TRUE;
		}

		case	eSKILL_EE_DECLINE_HIT_CHANCE			:	//	명중률 저하
		{
			iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int	iLimitValue		=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*_lpAbility->m_wLevel)/100;

			if	(iLimitValue)
				iValue			=	min(iValue,iLimitValue);

			*_lpUpkeepTime		-=	*_lpUpkeepTime*getDeclinePowerResistance()/100;
			*_lpUpkeepTime		/=	100;

			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			m_hitChance.correctValue(-iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_hitChance.m_wTime*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_DECLINE_DODGE_CHANCE			:	//	회피율 저하
		{
			iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int	iLimitValue		=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*_lpAbility->m_wLevel)/100;

			if (iLimitValue)
				iValue			=	min(iValue,iLimitValue);
			
			*_lpUpkeepTime		-=	*_lpUpkeepTime*getDeclinePowerResistance()/100;
			*_lpUpkeepTime		/=	100;
			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			m_dodgeChance.correctValue(-iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect );
			*_lpUpkeepTime		=	m_dodgeChance.m_wTime*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_BLIND							:	//	장님
			iActiveChance		=	iActiveChance*(100-getBlindResistance())/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		-=	*_lpUpkeepTime*getBlindResistance()/100;

			if	(*_lpUpkeepTime%100)
				*_lpUpkeepTime	=	*_lpUpkeepTime/100+1;
			else
				*_lpUpkeepTime	/=	100;

			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			m_wBlindTime			=	max(m_wBlindTime,*_lpUpkeepTime);
			*_lpUpkeepTime			=	m_wBlindTime*dSYNC_FPS;

			if	(_lpAttacker && _lpAttacker->isPureMonster())
				_lpAttacker->reaimTarget();

			return	TRUE;

		case	eSKILL_EE_DECLINE_MOVE_SPEED			:	//	이동속도저하
		{
			iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			int		iValue					=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;
			BOOL	bIsForTransCharacter	=	_lpEffect->m_aiValue[4];

			if	(bIsForTransCharacter)
				if (m_wJob!=dJOB_WEREWOLF&&m_wJob!=dJOB_FALLEN_ANGEL&&m_wJob!=dJOB_MAGICAL_GIRL&&m_wJob!=dJOB_DEVIL	)
					break;

			*_lpUpkeepTime		*=	dSYNC_FPS;
			*_lpUpkeepTime		=	*_lpUpkeepTime*(100-getDeclinePowerResistance())/100/100;

			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			iValue				=	iValue*(100-getDeclinePowerResistance())/100;

			m_moveSpeed.correctValue(-iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect );
			*_lpUpkeepTime		=	m_moveSpeed.m_wTime;
			return	TRUE;
		}

		case	eSKILL_EE_DECLINE_ATTACK_SPEED			:	//	공격속도감소
		{
			iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;

			if (iRandomValue	>=	iActiveChance)
				break;

			int		iValue					=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;
			BOOL	bIsForTransCharacter	=	_lpEffect->m_aiValue[4];

			if	(bIsForTransCharacter)
				if (m_wJob!=dJOB_WEREWOLF&&m_wJob!=dJOB_FALLEN_ANGEL&&m_wJob!=dJOB_MAGICAL_GIRL&&m_wJob!=dJOB_DEVIL	)
					break;

			*_lpUpkeepTime		=	*_lpUpkeepTime*(100-getDeclinePowerResistance())/100/100;

			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			iValue				=	iValue*(100-getDeclinePowerResistance())/100;

			m_actionSpeed.correctValue(-iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect );
			*_lpUpkeepTime		=	m_actionSpeed.m_wTime;
			return	TRUE;
		}

		case	eSKILL_EE_DECLINE_ATTACK_POWER				:	//	공격력 저하
		{
			iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;
			
			if	(iRandomValue	>=	iActiveChance)
				break;
			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			*_lpUpkeepTime		-=	*_lpUpkeepTime*getDeclinePowerResistance()/100;
			
			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			iValue				=	(100-getDeclinePowerResistance())*iValue/100;
			iValue				=	iValue/100;

			*_lpUpkeepTime		/=	100;
			m_attackPower.correctValue(-iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_attackPower.m_wTimeForCorrectValue*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_DECLINE_ATTACK_POWER_PERCENTAGE	:
		{
			iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;
			
			if	(iRandomValue	>=	iActiveChance)
				break;
			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			*_lpUpkeepTime		-=	*_lpUpkeepTime*getDeclinePowerResistance()/100;
			
			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			iValue				=	(100-getDeclinePowerResistance())*iValue/100;
			iValue				=	iValue/100;

			*_lpUpkeepTime		/=	100;
			m_attackPower.correctPercentageValue(-iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_attackPower.m_wTimeForCorrectValue*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_MOVE_SPEED			:	//	이동속도증가
		{
			if	(iRandomValue		>=	iActiveChance)
				break;

			int		iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int		iLimit			=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*_lpAbility->m_wLevel)/100;
			BOOL	bIsAccumulation	=	_lpEffect->m_aiValue[4];
			int		iOldValue		=	m_moveSpeed.m_sCorrectValue;

			*_lpUpkeepTime			*=	dSYNC_FPS;
			*_lpUpkeepTime			/=	100;

			if	(bIsAccumulation)
			{
				m_moveSpeed.m_sCorrectValue	+=	iValue;
				m_moveSpeed.m_wTime			=	*_lpUpkeepTime;
			}
			else
				m_moveSpeed.correctValue(iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect );

			if	(iLimit	&&	iOldValue		<	m_moveSpeed.m_sCorrectValue)
				m_moveSpeed.m_sCorrectValue	=	min(m_moveSpeed.m_sCorrectValue,iLimit);

			*_lpUpkeepTime		=	m_moveSpeed.m_wTime;
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_ATTACK_SPEED			:	//	공격속도증가
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int		iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int		iLimit			=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*_lpAbility->m_wLevel)/100;
			BOOL	bIsAccumulation	=	_lpEffect->m_aiValue[4];
			int		iOldValue		=	m_moveSpeed.m_sCorrectValue;

			*_lpUpkeepTime		/=	100;

			if	(bIsAccumulation)
			{
				m_actionSpeed.m_sCorrectValue	+=	iValue;
				m_actionSpeed.m_wTime			=	*_lpUpkeepTime;
			}
			else
				m_actionSpeed.correctValue(iValue,*_lpUpkeepTime);

			if	(iLimit	&&	iOldValue		<	m_actionSpeed.m_sCorrectValue)
				m_actionSpeed.m_sCorrectValue	=	min(m_actionSpeed.m_sCorrectValue,iLimit);

			m_actionSpeed.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_actionSpeed.m_wTime*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_RESET_ATTACK_DEFENSE_POWER_FOR_PET_COMMAND		:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			m_attackPower.resetForPetCommandValue();
			m_defensePower.resetForPetCommandValue();
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_ATTACK_POWER_PERCENTAGE_FOR_PET_COMMAND	:
		{
			if (iRandomValue	>=	iActiveChance)
				break;
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime		/=	100;

			if	(*_lpUpkeepTime	==	0)
				*_lpUpkeepTime	=	0xffff;

			m_attackPower.correctPercentageValueForPetCommand(iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect );
			*_lpUpkeepTime		=	m_attackPower.m_wTimeForCorrectPercentageValueForPetCommand*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_DECLINE_WEAPON_ATTACK_POWER_PERCENTAGE	:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iValue3			=	_lpEffect->m_aiValue[2];
			int	iValue4			=	_lpEffect->m_aiValue[3];

			int	iLimitValue		=	(iValue3+iValue4*_lpAbility->m_wLevel);
			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime		/=	100;

			if	(iLimitValue)
				iValue			=	min(iValue,iLimitValue);

			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			*_lpUpkeepTime		/=	100;

			m_weaponPower.correctPercentageValue(-iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);

			*_lpUpkeepTime		=	m_weaponPower.m_wTimeForCorrectPercentageValue*dSYNC_FPS;

			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_WEAPON_ATTACK_POWER		:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iValue3			=	_lpEffect->m_aiValue[2];
			int	iValue4			=	_lpEffect->m_aiValue[3];

			int	iLimitValue		=	(iValue3+iValue4*_lpAbility->m_wLevel);
			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel);

			*_lpUpkeepTime		/=	100;

			if (iLimitValue)
				iValue			=	min(iValue,iLimitValue);

			m_weaponPower.correctValue(iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect);

			*_lpUpkeepTime		=	m_weaponPower.m_wTimeForCorrectValue*dSYNC_FPS;

			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_ARMOR_DEFENSE_POWER			:	//	기본 방어력증가
		{
			if (iRandomValue	>=	iActiveChance)
				break;
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iValue3			=	_lpEffect->m_aiValue[2];
			int	iValue4			=	_lpEffect->m_aiValue[3];

			int	iLimitValue		=	(iValue3+iValue4*_lpAbility->m_wLevel);
			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel);

			if (iLimitValue)
				iValue			=	min(iValue,iLimitValue);

			*_lpUpkeepTime		/=	100;

			m_armorPower.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);

			return	TRUE;
		}

		
		case	eSKILL_EE_INCLINE_ATTACK_POWER			:	//	공격력증가
		{
			if (iRandomValue	>=	iActiveChance)
				break;
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iValue3			=	_lpEffect->m_aiValue[2];
			int	iValue4			=	_lpEffect->m_aiValue[3];

			int	iLimitValue		=	(iValue3+iValue4*_lpAbility->m_wLevel);
			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel);

			if (iLimitValue)
				iValue			=	min(iValue,iLimitValue);

			*_lpUpkeepTime		/=	100;
			m_attackPower.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_attackPower.m_wTimeForCorrectValue*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_ATTACK_POWER_PERCENTAGE	:
		{
			if (iRandomValue	>=	iActiveChance)
				break;
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime		/=	100;
			m_attackPower.correctPercentageValue(iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_attackPower.m_wTimeForCorrectPercentageValue*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_DEFENSE_POWER_PERCENTAGE_FOR_PET_COMMAND	:	//	방어력 퍼센티지 감소
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			if	(*_lpUpkeepTime	==	0)
				*_lpUpkeepTime	=	0xffff;
			else
			{
				*_lpUpkeepTime		/=	100;
				*_lpUpkeepTime		=	max(*_lpUpkeepTime,1);
			}

			m_defensePower.correctPercentageValueForPetCommand(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_defensePower.m_wTimeForCorrectPercentageValueForPetCommand*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_DECREASE_DEFENSE_POWER			:
		{
			iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel);
			int	iLimit			=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel);

			iValue				=	iValue*(100-getDeclinePowerResistance())/100;

			if	(iLimit)
				iValue			=	min(iValue,iLimit);

			*_lpUpkeepTime		=	*_lpUpkeepTime*(100-getDeclinePowerResistance())/100;
			*_lpUpkeepTime		/=	100;

			*_lpUpkeepTime		=	max(*_lpUpkeepTime,1);

			m_defensePower.correctValue(-iValue/100,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_defensePower.m_wTimeForCorrectValue*dSYNC_FPS;

			sendEtcWork(eEW_CHANGE_DEFENSIVE_STATUS,m_defensePower.m_sCorrectValue,m_defensePower.m_sCorrectPercentageValue,m_defensePower.m_wTimeForCorrectValue,m_defensePower.m_wTimeForCorrectPercentageValue);

			return	TRUE;
		}

		case	eSKILL_EE_DECREASE_FINAL_DEFENSE_POWER_PERCENTAGE			:
		{
			iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		=	*_lpUpkeepTime*(100-getDeclinePowerResistance())/100;

			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int	iUpkeepTime		=	(*_lpUpkeepTime)/100;

			iValue				=	iValue*(100-getDeclinePowerResistance())/100;
			iUpkeepTime			=	max(iUpkeepTime,1);

			if	(m_wCorrectFinalDefensivePowerPercentageTime	>	iUpkeepTime)
				break;

			m_wCorrectFinalDefensivePowerPercentageTime	=	iUpkeepTime;
			m_sCorrectFinalDefensivePowerPercentage		=	-iValue;
			
			sendEtcWork(eEW_CHANGE_DEFENSIVE_STATUS,m_defensePower.m_sCorrectValue,m_defensePower.m_sCorrectPercentageValue,m_defensePower.m_wTimeForCorrectValue,m_defensePower.m_wTimeForCorrectPercentageValue);

			return	TRUE;
		}

		case	eSKILL_EE_DECREASE_DEFENSE_POWER_PERCENTAGE	:
		{
			iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			int		iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int		iLimit			=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel);
			BOOL	bIsAccumulation	=	_lpEffect->m_aiValue[4];
			int		iOldValue		=	m_defensePower.m_sCorrectPercentageValue;

			*_lpUpkeepTime		=	*_lpUpkeepTime*(100-getDeclinePowerResistance())/100;

			*_lpUpkeepTime			/=	100;
			*_lpUpkeepTime			=	max(*_lpUpkeepTime,1);

			iValue					=	iValue*(100-getDeclinePowerResistance())/100;

			if	(bIsAccumulation)
			{
				m_defensePower.m_sCorrectPercentageValue		-=	iValue;
				m_defensePower.m_wTimeForCorrectPercentageValue	=	*_lpUpkeepTime;
			}
			else
				m_defensePower.correctPercentageValue(-iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);

			if	(iLimit	&&	iOldValue	>	m_defensePower.m_sCorrectPercentageValue)
				m_defensePower.m_sCorrectPercentageValue	=	max(m_defensePower.m_sCorrectPercentageValue,-iLimit);

			*_lpUpkeepTime		=	m_defensePower.m_wTimeForCorrectPercentageValue*dSYNC_FPS;

			sendEtcWork(eEW_CHANGE_DEFENSIVE_STATUS,m_defensePower.m_sCorrectValue,m_defensePower.m_sCorrectPercentageValue,m_defensePower.m_wTimeForCorrectValue,m_defensePower.m_wTimeForCorrectPercentageValue);

			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_DEFENSE_POWER			:	//	방어력 증가
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel);
			int	iLimit			=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel);

			if	(iLimit)
				iValue			=	min(iValue,iLimit);

			correctValueEffectDuelServer(&iValue , lpSkill);

			*_lpUpkeepTime		/=	100;
			*_lpUpkeepTime		=	max(*_lpUpkeepTime,1);
			m_defensePower.correctValue(iValue/100,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_defensePower.m_wTimeForCorrectValue*dSYNC_FPS;

			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_DEFENSE_POWER_PERCENTAGE	:	//	방어력 퍼센티지 증가
		{
			if		(iRandomValue	>=	iActiveChance)
				break;

			int		iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int		iLimit			=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel);
			BOOL	bIsAccumulation	=	_lpEffect->m_aiValue[4];
			int		iOldValue		=	m_defensePower.m_sCorrectPercentageValue;

			*_lpUpkeepTime			/=	100;

			correctValueEffectDuelServer(&iValue , lpSkill);

			if	(bIsAccumulation)
			{
				m_defensePower.m_sCorrectPercentageValue		+=	iValue;
				m_defensePower.m_wTimeForCorrectPercentageValue	=	*_lpUpkeepTime;
			}
			else
				m_defensePower.correctPercentageValue(iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect );

			if	(iLimit	&&	iOldValue	<	m_defensePower.m_sCorrectPercentageValue)
				m_defensePower.m_sCorrectPercentageValue	=	min(m_defensePower.m_sCorrectPercentageValue,iLimit);
			
			*_lpUpkeepTime		=	m_defensePower.m_wTimeForCorrectPercentageValue*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_NOCK_BACK						:	//	녹백
		{
			if	(!_lpAttacker)
				break;

			if	(lpSkill->m_bf1IsResistKnockBackByCurse)
				iActiveChance		=	iActiveChance*(100-getCurseResistance())/100;

			if	(iRandomValue	>=	iActiveChance	||	m_bf1IsImmuneKnockBack)
				break;

			int	iDistance		=	_lpEffect->getNockBackDistance(_lpAbility->m_wLevel);
			int	iAngle			=	_lpEffect->m_aiValue[2];

			thrust(_lpAttacker->m_pos.x,_lpAttacker->m_pos.y,iDistance,TRUE,iAngle);

			return	TRUE;
		}

		case	eSKILL_EE_LEVITATE						:	//	공중부양
		{
			if (iRandomValue	>=	iActiveChance)	break;

			if (m_sFloatTime	==	-1)
				return	FALSE;

			*_lpUpkeepTime		/=	100;
			m_sFloatTime		=	max(m_sFloatTime,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_sFloatTime*dSYNC_FPS;

			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_DODGE_CHANCE			:	//	회피율 증가
		{
			if (iRandomValue	>=	iActiveChance)
				break;
			
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iLimit			=	_lpEffect->m_aiValue[2]/100;
			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;
			
			if (iLimit)
				iValue			=	min(iValue,iLimit);
			
			if (_lpEffect->m_aiValue[3])
			{
				int	iDodgeChance=	m_dodgeChance.m_sValue/2+iValue;

				iValue			=	iDodgeChance-m_dodgeChance.m_sValue;
			}

			*_lpUpkeepTime		/=	100;
			m_dodgeChance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_dodgeChance.m_wTime*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_CONCENTRATION			:	//	집중력 증가
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime		/=	100;
			m_concentration.correctValue(iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect );
			*_lpUpkeepTime		=	m_concentration.m_wTime*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_CONCENTRATION_BY_STELLAR_PEER		:	//	집중력 증가
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime		/=	100;
			m_concentrationByStellarPeer.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_concentrationByStellarPeer.m_wTime*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_DECLINE_CONCENTRATION			:	//	집중력 증가
		{
			iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime		/=	100;
			m_concentration.correctValue(-iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect );
			*_lpUpkeepTime		=	m_concentration.m_wTime*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_FREEZE						:	//	프리즈
		{
			iActiveChance		=	iActiveChance*(100-getFreezeResistance())/100;

			if (iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		=	*_lpUpkeepTime*(100-getFreezeResistance())/100;
			*_lpUpkeepTime		*=	dSYNC_FPS;
			*_lpUpkeepTime		/=	100;

			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			setFreezeTime(*_lpUpkeepTime);

			*_lpUpkeepTime		=	m_wFreezeTime;
			return	TRUE;
		}

		case	eSKILL_EE_COLD							:	//	콜드
		{
			iActiveChance		=	iActiveChance*(100-getColdResistance())/100;

			if (iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		=	*_lpUpkeepTime*(100-getColdResistance())/100;
					
			*_lpUpkeepTime		/=	100;

			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			m_wColdTime			=	max(*_lpUpkeepTime,m_wColdTime);
			*_lpUpkeepTime		=	m_wColdTime;

			if	(m_wColdTime)
				moveDestPos();

			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_POWER						:	//	힘증가"	
		{
			int		iValue1			=	_lpEffect->m_aiValue[0];
			int		iValue2			=	_lpEffect->m_aiValue[1];
			int		iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;
			BOOL	bIsOverlapping	=	_lpEffect->m_aiValue[2];
			int		iOveralappingLimit=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*_lpAbility->m_wLevel)/100;

			if	(bIsOverlapping)
			{
				if	(iOveralappingLimit	&&	m_strength.m_sCorrectValue	>	iOveralappingLimit)
					break;

				iValue	+=	m_strength.m_sCorrectValue;
			}

			if	(iValue	<	0)
			{
				iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;
				iValue				=	iValue*(100-getDeclinePowerResistance())/100;
			}
			
			if	(iRandomValue	>=	iActiveChance)
				break;

			if	(iOveralappingLimit)
				iValue				=	min(iValue,iOveralappingLimit);

			*_lpUpkeepTime		/=	100;
			m_strength.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_strength.m_wTime*dSYNC_FPS;
			buildPower();
			sendEtcWork(eEW_INLINE_STRENGTH,m_strength.m_sCorrectValue,m_strength.m_wTime);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_AGILITY					:	//	민첩성증가"
		{
			int		iValue1			=	_lpEffect->m_aiValue[0];
			int		iValue2			=	_lpEffect->m_aiValue[1];
			int		iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;
			BOOL	bIsOverlapping	=	_lpEffect->m_aiValue[2];
			int		iOveralappingLimit=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*_lpAbility->m_wLevel)/100;

			if	(bIsOverlapping)
			{
				if	(iOveralappingLimit	&&	m_agility.m_sCorrectValue	>	iOveralappingLimit)
					break;

				iValue	+=	m_agility.m_sCorrectValue;
			}

			if	(iValue	<	0)
			{
				iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;
				iValue				=	iValue*(100-getDeclinePowerResistance())/100;
			}
			
			if	(iRandomValue	>=	iActiveChance)
				break;

			if	(iOveralappingLimit)
				iValue				=	min(iValue,iOveralappingLimit);

			*_lpUpkeepTime		/=	100;
			m_agility.correctValue(iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_agility.m_wTime*dSYNC_FPS;
			buildPower();
			sendEtcWork(eEW_INLINE_AGILITY,m_agility.m_sCorrectValue,m_agility.m_wTime);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_CONSTITUTION				:	//	건강증가"	
		{
			int		iValue1			=	_lpEffect->m_aiValue[0];
			int		iValue2			=	_lpEffect->m_aiValue[1];
			int		iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;
			BOOL	bIsOverlapping	=	_lpEffect->m_aiValue[2];
			int		iOveralappingLimit=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*_lpAbility->m_wLevel)/100;

			if	(bIsOverlapping)
			{
				if	(iOveralappingLimit	&&	m_constitution.m_sCorrectValue	>	iOveralappingLimit)
					break;

				iValue	+=	m_constitution.m_sCorrectValue;
			}

			if	(iOveralappingLimit)
				iValue				=	min(iValue,iOveralappingLimit);

			if	(iValue	<	0)
			{
				iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;
				iValue				=	iValue*(100-getDeclinePowerResistance())/100;
			}
			
			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;
			m_constitution.correctValue(iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_constitution.m_wTime*dSYNC_FPS;
			buildPower();
			sendEtcWork(eEW_INLINE_CONSTITUTION,m_constitution.m_sCorrectValue,m_constitution.m_wTime);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_INTELIGENCE				:	//	지식증가"	
		{
			int		iValue1			=	_lpEffect->m_aiValue[0];
			int		iValue2			=	_lpEffect->m_aiValue[1];
			int		iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;
			BOOL	bIsOverlapping	=	_lpEffect->m_aiValue[2];
			int		iOveralappingLimit=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*_lpAbility->m_wLevel)/100;

			if	(bIsOverlapping)
			{
				if	(iOveralappingLimit	&&	m_intelligence.m_sCorrectValue	>	iOveralappingLimit)
					break;

				iValue	+=	m_intelligence.m_sCorrectValue;
			}

			if	(iValue	<	0)
			{
				iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;
				iValue				=	iValue*(100-getDeclinePowerResistance())/100;
			}

			if	(iRandomValue	>=	iActiveChance)
				break;

			if	(iOveralappingLimit)
				iValue				=	min(iValue,iOveralappingLimit);

			*_lpUpkeepTime		/=	100;
			m_intelligence.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_intelligence.m_wTime*dSYNC_FPS;
			buildPower();
			sendEtcWork(eEW_INLINE_INTELLIGENCE,m_intelligence.m_sCorrectValue,m_intelligence.m_wTime);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_WISDOM					:	//	지혜증가"	
		{
			int		iValue1			=	_lpEffect->m_aiValue[0];
			int		iValue2			=	_lpEffect->m_aiValue[1];
			int		iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;
			BOOL	bIsOverlapping	=	_lpEffect->m_aiValue[2];
			int		iOveralappingLimit=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*_lpAbility->m_wLevel)/100;

			if	(bIsOverlapping)
			{
				if	(iOveralappingLimit	&&	m_wisdom.m_sCorrectValue	>	iOveralappingLimit)
					break;

				iValue	+=	m_wisdom.m_sCorrectValue;
			}

			if	(iOveralappingLimit)
				iValue				=	min(iValue,iOveralappingLimit);

			if	(iValue	<	0)
			{
				iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;
				iValue				=	iValue*(100-getDeclinePowerResistance())/100;
			}
			
			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;
			m_wisdom.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_wisdom.m_wTime*dSYNC_FPS;
			buildPower();
			sendEtcWork(eEW_INLINE_WISDOM,m_wisdom.m_sCorrectValue,m_wisdom.m_wTime);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_LUCK						:	//	운증가"	
		{
			int		iValue1			=	_lpEffect->m_aiValue[0];
			int		iValue2			=	_lpEffect->m_aiValue[1];
			int		iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;
			BOOL	bIsOverlapping	=	_lpEffect->m_aiValue[2];
			int		iOveralappingLimit=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*_lpAbility->m_wLevel)/100;

			if	(bIsOverlapping)
			{
				if	(iOveralappingLimit	&&	m_luck.m_sCorrectValue	>	iOveralappingLimit)
					break;

				iValue	+=	m_luck.m_sCorrectValue;
			}

			if	(iValue	<	0)
			{
				iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;
				iValue				=	iValue*(100-getDeclinePowerResistance())/100;
			}
			
			if	(iRandomValue	>=	iActiveChance)
				break;

			if	(iOveralappingLimit)
				iValue				=	min(iValue,iOveralappingLimit);

			*_lpUpkeepTime		/=	100;
			m_luck.correctValue(iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_luck.m_wTime*dSYNC_FPS;
			sendEtcWork(eEW_INLINE_LUCK,m_luck.m_sCorrectValue,m_luck.m_wTime);
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_STRANGE_STATUS_RESISTANCE	:	//	이상상태 저항 증가
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime		/=	100;
			correctValueEffectDuelServer(&iValue , lpSkill);
			m_strangeStatusResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_strangeStatusResistance.m_wTime*dSYNC_FPS;
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_DECLINE_POWER_RESISTANCE	:	//	능력치 저하 저항 증가
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime		/=	100;
			m_declinePowerResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_declinePowerResistance.m_wTime*dSYNC_FPS;
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_CURSE_RESISTANCE			:	//	저주 저항 증가
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime		/=	100;
			m_curseResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_curseResistance.m_wTime*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_STUN_RESISTANCE	:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime		=	*_lpUpkeepTime*dSYNC_FPS/100;
			m_stunResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			return	TRUE;
		}

		case	eSKILL_EE_DECREASE_PHYSICAL_DAMAGE	:
		{
			if (iRandomValue		>=	iActiveChance)
				break;

			int	iValue1				=	_lpEffect->m_aiValue[0];
			int	iValue2				=	_lpEffect->m_aiValue[1];

			int	iValue				=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_wReducePhysicalDamage	=	max(iValue,0);
			m_wReducePhysicalDamage	=	min(m_wReducePhysicalDamage,100);

			return	TRUE;
		}

		case	eSKILL_EE_INVINCIBLE				:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			m_wInvincibleTime	=	*_lpUpkeepTime/100*dSYNC_FPS;
			*_lpUpkeepTime		=	m_wInvincibleTime;
			return	TRUE;
		}

		case	eSKILL_EE_DODGE_THIS_TIME_ATTACK	:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			m_bf1IsAvoidThisTimeAttack	=	TRUE;
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_4ELEMENT_RESISTANCE	:
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iValue3			=	_lpEffect->m_aiValue[2];
			int	iValue4			=	_lpEffect->m_aiValue[3];
			int	iLimit			=	(iValue3+iValue4*_lpAbility->m_wLevel)/100;
			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			if (iValue	<	0)
			{
				iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;

				if (iLimit)
					iValue			=	max(iValue,iLimit);

				*_lpUpkeepTime		-=	*_lpUpkeepTime*getDeclinePowerResistance()/100;

				if (m_wCurseAmplificationTime)
					*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;
			}
			else
			{
				if (iLimit)
					iValue		=	min(iValue,iLimit);
			}

			if (iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;

			m_fireResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			m_waterResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			m_earthResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			m_windResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);

			sendEtcWork(eEW_CHANGE_FIRE_RESISTANCE,m_fireResistance.m_sCorrectValue,m_fireResistance.m_wTime);
			sendEtcWork(eEW_CHANGE_WATER_RESISTANCE,m_waterResistance.m_sCorrectValue,m_waterResistance.m_wTime);
			sendEtcWork(eEW_CHANGE_EARTH_RESISTANCE,m_earthResistance.m_sCorrectValue,m_earthResistance.m_wTime);
			sendEtcWork(eEW_CHANGE_WIND_RESISTANCE,m_windResistance.m_sCorrectValue,m_windResistance.m_wTime);

			return	TRUE;
		}
		
		case	eSKILL_EE_INCLINE_MAGIC_RESISTANCE		:
		case	eSKILL_EE_INCLINE_FIRE_RESISTANCE		:	//	불저항 증가
		case	eSKILL_EE_INCLINE_WATER_RESISTANCE		:	//	물저항 증가
		case	eSKILL_EE_INCLINE_EARTH_RESISTANCE		:	//	대지저항 증가
		case	eSKILL_EE_INCLINE_WIND_RESISTANCE		:	//	바람저항 증가
		case	eSKILL_EE_INCLINE_LIGHT_RESISTANCE		:	//	빛저항 증가
		case	eSKILL_EE_INCLINE_DARK_RESISTANCE		:	//	암흑저항 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iValue3			=	_lpEffect->m_aiValue[2];
			int	iValue4			=	_lpEffect->m_aiValue[3];
			int	iLimit			=	(iValue3+iValue4*_lpAbility->m_wLevel)/100;
			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			if	(iLimit)
				iValue			=	min(iValue,iLimit);

			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;

			switch(_lpEffect->m_wEffect)
			{
				case	eSKILL_EE_INCLINE_MAGIC_RESISTANCE	:
					correctValueEffectDuelServer(&iValue , lpSkill);					
					m_allMagicResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					sendEtcWork(eEW_CHANGE_MAGIC_RESISTANCE,m_allMagicResistance.m_sCorrectValue,m_allMagicResistance.m_wTime);
					*_lpUpkeepTime		=	m_allMagicResistance.m_wTime*dSYNC_FPS;
					break;
				case	eSKILL_EE_INCLINE_FIRE_RESISTANCE	:	//	불저항 감소
					m_fireResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					sendEtcWork(eEW_CHANGE_FIRE_RESISTANCE,m_fireResistance.m_sCorrectValue,m_fireResistance.m_wTime);
					*_lpUpkeepTime		=	m_fireResistance.m_wTime*dSYNC_FPS;
					break;
				case	eSKILL_EE_INCLINE_WATER_RESISTANCE	:	//	물저항 감소
					m_waterResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					sendEtcWork(eEW_CHANGE_WATER_RESISTANCE,m_waterResistance.m_sCorrectValue,m_waterResistance.m_wTime);
					*_lpUpkeepTime		=	m_waterResistance.m_wTime*dSYNC_FPS;
					break;
				case	eSKILL_EE_INCLINE_EARTH_RESISTANCE	:	//	대지저항 감소
					m_earthResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					sendEtcWork(eEW_CHANGE_EARTH_RESISTANCE,m_earthResistance.m_sCorrectValue,m_earthResistance.m_wTime);
					*_lpUpkeepTime		=	m_earthResistance.m_wTime*dSYNC_FPS;
					break;
				case	eSKILL_EE_INCLINE_WIND_RESISTANCE	:	//	바람저항 감소
					m_windResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					sendEtcWork(eEW_CHANGE_WIND_RESISTANCE,m_windResistance.m_sCorrectValue,m_windResistance.m_wTime);
					*_lpUpkeepTime		=	m_windResistance.m_wTime*dSYNC_FPS;
					break;
				case	eSKILL_EE_INCLINE_LIGHT_RESISTANCE	:	//	빛저항 감소
					m_lightResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					sendEtcWork(eEW_CHANGE_LIGHT_RESISTANCE,m_lightResistance.m_sCorrectValue,m_lightResistance.m_wTime);
					*_lpUpkeepTime		=	m_lightResistance.m_wTime*dSYNC_FPS;
					break;
				case	eSKILL_EE_INCLINE_DARK_RESISTANCE	:	//	암흑저항 감소
					m_darkResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					sendEtcWork(eEW_CHANGE_DARK_RESISTANCE,m_darkResistance.m_sCorrectValue,m_darkResistance.m_wTime);
					*_lpUpkeepTime		=	m_darkResistance.m_wTime*dSYNC_FPS;
					break;
			}

			return	TRUE;
		}

		case	eSKILL_EE_DECLINE_MAGIC_RESISTANCE		:
		case	eSKILL_EE_DECLINE_FIRE_RESISTANCE		:	//	불저항 증가
		case	eSKILL_EE_DECLINE_WATER_RESISTANCE		:	//	물저항 증가
		case	eSKILL_EE_DECLINE_EARTH_RESISTANCE		:	//	대지저항 증가
		case	eSKILL_EE_DECLINE_WIND_RESISTANCE		:	//	바람저항 증가
		case	eSKILL_EE_DECLINE_LIGHT_RESISTANCE		:	//	빛저항 증가
		case	eSKILL_EE_DECLINE_DARK_RESISTANCE		:	//	암흑저항 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iValue3			=	_lpEffect->m_aiValue[2];
			int	iValue4			=	_lpEffect->m_aiValue[3];
			int	iLimit			=	(iValue3+iValue4*_lpAbility->m_wLevel)/100;
			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			iActiveChance		=	iActiveChance*(100-getDeclinePowerResistance())/100;
			iValue				=	iValue*(100-getDeclinePowerResistance())/100;

			if	(iLimit)
				iValue			=	min(iValue,iLimit);

			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;
			iValue				=	-iValue;

			switch(_lpEffect->m_wEffect)
			{
				case	eSKILL_EE_DECLINE_MAGIC_RESISTANCE	:
					m_allMagicResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					*_lpUpkeepTime		=	m_allMagicResistance.m_wTime*dSYNC_FPS;
					sendEtcWork(eEW_CHANGE_MAGIC_RESISTANCE,m_allMagicResistance.m_sCorrectValue,m_allMagicResistance.m_wTime);
					break;
				case	eSKILL_EE_DECLINE_FIRE_RESISTANCE	:	//	불저항 감소
					m_fireResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					*_lpUpkeepTime		=	m_fireResistance.m_wTime*dSYNC_FPS;
					sendEtcWork(eEW_CHANGE_FIRE_RESISTANCE,m_fireResistance.m_sCorrectValue,m_fireResistance.m_wTime);
					break;
				case	eSKILL_EE_DECLINE_WATER_RESISTANCE	:	//	물저항 감소
					m_waterResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					*_lpUpkeepTime		=	m_waterResistance.m_wTime*dSYNC_FPS;
					sendEtcWork(eEW_CHANGE_WATER_RESISTANCE,m_waterResistance.m_sCorrectValue,m_waterResistance.m_wTime);
					break;
				case	eSKILL_EE_DECLINE_EARTH_RESISTANCE	:	//	대지저항 감소
					m_earthResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					*_lpUpkeepTime		=	m_earthResistance.m_wTime*dSYNC_FPS;
					sendEtcWork(eEW_CHANGE_EARTH_RESISTANCE,m_earthResistance.m_sCorrectValue,m_earthResistance.m_wTime);
					break;
				case	eSKILL_EE_DECLINE_WIND_RESISTANCE	:	//	바람저항 감소
					m_windResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					*_lpUpkeepTime		=	m_windResistance.m_wTime*dSYNC_FPS;
					sendEtcWork(eEW_CHANGE_WIND_RESISTANCE,m_windResistance.m_sCorrectValue,m_windResistance.m_wTime);
					break;
				case	eSKILL_EE_DECLINE_LIGHT_RESISTANCE	:	//	빛저항 감소
					m_lightResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					*_lpUpkeepTime		=	m_lightResistance.m_wTime*dSYNC_FPS;
					sendEtcWork(eEW_CHANGE_LIGHT_RESISTANCE,m_lightResistance.m_sCorrectValue,m_lightResistance.m_wTime);
					break;
				case	eSKILL_EE_DECLINE_DARK_RESISTANCE	:	//	암흑저항 감소
					m_darkResistance.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
					*_lpUpkeepTime		=	m_darkResistance.m_wTime*dSYNC_FPS;
					sendEtcWork(eEW_CHANGE_DARK_RESISTANCE,m_darkResistance.m_sCorrectValue,m_darkResistance.m_wTime);
					break;
			}

			return	TRUE;
		}


		case	eSKILL_EE_INCLINE_MAX_CP						:	//	CP증가
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			*_lpUpkeepTime		/=	100;
			m_maxCP.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_maxCP.m_wTimeForCorrectValue*dSYNC_FPS;
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_MAX_HP						:	//	체력증가"	
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int		iValue1		=	_lpEffect->m_aiValue[0];
			int		iValue2		=	_lpEffect->m_aiValue[1];
			BOOL	bIsHeal		=	_lpEffect->m_aiValue[4];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel);
			int	iMaxHP			=	getMaxHP();

			*_lpUpkeepTime		/=	100;
			m_maxHP.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_maxHP.m_wTimeForCorrectValue*dSYNC_FPS;

			if	(iMaxHP			!=	getMaxHP())
			{
				if	(bIsHeal)
					m_iHP		+=	max(getMaxHP()-iMaxHP,0);

				addSendSimpleInfo(NULL,FALSE,TRUE);
			}
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_MAX_HP_PERCENTAGE				:	//	체력증가"	
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int		iValue1		=	_lpEffect->m_aiValue[0];
			int		iValue2		=	_lpEffect->m_aiValue[1];
			BOOL	bIsHeal		=	_lpEffect->m_aiValue[4];
			int		iMaxHP		=	getMaxHP();

			int		iValue		=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			correctValueEffectDuelServer(&iValue , lpSkill);

			*_lpUpkeepTime		/=	100;
			m_maxHP.correctPercentageValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_maxHP.m_wTimeForCorrectValue*dSYNC_FPS;

			if	(iMaxHP			!=	getMaxHP())
			{
				if	(bIsHeal)
					m_iHP		+=	max(getMaxHP()-iMaxHP,0);

				addSendPlayerBasicInformation();
				addSendSimpleInfo(NULL,FALSE,TRUE);
			}

			return	TRUE;
		}

		case	eSKILL_EE_REGEN_HP							:	//	체력자동회복"	
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	min(30000,iValue1+iValue2*_lpAbility->m_wLevel);

			*_lpUpkeepTime		/=	100;
			m_regenHPPerSecond.correctValue(iValue,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_regenHPPerSecond.m_wTime*dSYNC_FPS;
			return	TRUE;
		}


		case	eSKILL_EE_ADDITIONAL_FIRE_DAMAGE			:	//	추가불대미지	
		case	eSKILL_EE_ADDITIONAL_WATER_DAMAGE			:	//	추가물대미지	
		case	eSKILL_EE_ADDITIONAL_WIND_DAMAGE			:	//	추가바람대미지	
		case	eSKILL_EE_ADDITIONAL_EARTH_DAMAGE			:	//	추가대지대미지	
		case	eSKILL_EE_ADDITIONAL_LIGHT_DAMAGE			:	//	추가빛대미지	
		case	eSKILL_EE_ADDITIONAL_DARK_DAMAGE			:	//	추가어둠대미지	
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iDamage		=	iValue1+iValue2*_lpAbility->m_wLevel;
			
			iDamage			=	iDamage+iDamage*_lpAttacker->getIntelligence()/200;

			int	iMinDamage	=	iDamage-iValue3*iSkillLevel;
			int	iMaxDamage	=	iDamage+iValue3*iSkillLevel;
			int	iRange		=	iMaxDamage-iMinDamage;

			if	(iRange)
				iDamage		=	iMinDamage;

			iDamage			/=	100;
			iRange			/=	100;
			*_lpUpkeepTime	/=	100;

			if	(_lpEffect->m_wEffect	==	eSKILL_EE_ADDITIONAL_FIRE_DAMAGE	)
			{
				m_addFireDamage.correctValue(iDamage,iRange,*_lpUpkeepTime, bIsCanNotApplySameEffect);
				*_lpUpkeepTime	=	m_addFireDamage.m_wTime*dSYNC_FPS;
			}
			if	(_lpEffect->m_wEffect	==	eSKILL_EE_ADDITIONAL_WATER_DAMAGE	)	
			{
				m_addWaterDamage.correctValue(iDamage,iRange,*_lpUpkeepTime, bIsCanNotApplySameEffect);
				*_lpUpkeepTime	=	m_addWaterDamage.m_wTime*dSYNC_FPS;
			}
			if	(_lpEffect->m_wEffect	==	eSKILL_EE_ADDITIONAL_WIND_DAMAGE	)	
			{
				m_addWindDamage.correctValue(iDamage,iRange,*_lpUpkeepTime, bIsCanNotApplySameEffect);
				*_lpUpkeepTime	=	m_addWindDamage.m_wTime*dSYNC_FPS;
			}
			if	(_lpEffect->m_wEffect	==	eSKILL_EE_ADDITIONAL_EARTH_DAMAGE	)	
			{
				m_addEarthDamage.correctValue(iDamage,iRange,*_lpUpkeepTime,bIsCanNotApplySameEffect );
				*_lpUpkeepTime	=	m_addEarthDamage.m_wTime*dSYNC_FPS;
			}
			if	(_lpEffect->m_wEffect	==	eSKILL_EE_ADDITIONAL_LIGHT_DAMAGE	)
			{
				m_addLightDamage.correctValue(iDamage,iRange,*_lpUpkeepTime, bIsCanNotApplySameEffect);
				*_lpUpkeepTime	=	m_addLightDamage.m_wTime*dSYNC_FPS;
			}
			if	(_lpEffect->m_wEffect	==	eSKILL_EE_ADDITIONAL_DARK_DAMAGE	)
			{
				m_addDarkDamage.correctValue(iDamage,iRange,*_lpUpkeepTime, bIsCanNotApplySameEffect);
				*_lpUpkeepTime	=	m_addDarkDamage.m_wTime*dSYNC_FPS;
			}
			
			return	TRUE;
		}

		case	eSKILL_EE_FIRE_DAMAGE_SHIELD				:	//	불대미지 실드
		case	eSKILL_EE_WATER_DAMAGE_SHIELD				:	//	물대미지 실드
		case	eSKILL_EE_WIND_DAMAGE_SHIELD				:	//	바람 대미지 실드
		case	eSKILL_EE_EARTH_DAMAGE_SHIELD				:	//	대지 대미지 실드
		case	eSKILL_EE_LIGHT_DAMAGE_SHIELD				:	//	빛 대미지 실드
		case	eSKILL_EE_DARK_DAMAGE_SHIELD				:	//	어둠 대미지 실드
		{
			if (iRandomValue>=	iActiveChance)
				break;

			*_lpUpkeepTime		*=	dSYNC_FPS;
			*_lpUpkeepTime		/=	100;

			return	TRUE;
		}

		case	eSKILL_EE_CAN_NOT_MOVE						:	//	이동불가 by 대지
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		*=	dSYNC_FPS;
			*_lpUpkeepTime		/=	100;

			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			if	(getMoveSpeed()	>	-100)
			{
				stopAction();
				addPacketStop();
			}

			m_moveSpeed.correctValue(-1000,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_moveSpeed.m_wTime;

			return	TRUE;
		}

		case	eSKILL_EE_CAN_NOT_MOVE_BY_EARTH				:	//	이동불가 by 대지
		{
			int	iResistanceValue=	max(getEarthResistance(),getStunResistance());

			if	(bIsPVP)
				iResistanceValue=	getEarthResistance()+getStunResistance();

			iActiveChance		=	iActiveChance*(100-iResistanceValue)/100;

			if (iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		*=	dSYNC_FPS;
			*_lpUpkeepTime		=	*_lpUpkeepTime*(100-iResistanceValue)/100/100;

			if	(*_lpUpkeepTime	<=	0)
				break;

			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			if	(getMoveSpeed()	!=	-100)
			{
				stopAction();
				addPacketStop();
			}

			m_moveSpeed.correctValue(-100,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime		=	m_moveSpeed.m_wTime;
			return	TRUE;
		}

		case	eSKILL_EE_DECLINE_MOVE_SPEED_BY_EARTH		:	//	이동속도 저하 by 대지
		{
			int	iResistanceValue=	max(getEarthResistance(),getDeclinePowerResistance());

			if	(bIsPVP)
				iResistanceValue=	max(getEarthResistance()+getDeclinePowerResistance(),0);
			
			iActiveChance		=	iActiveChance*(100-iResistanceValue)/100;

			if (iRandomValue	>=	iActiveChance)
				break;
			int	iValue			=	_lpEffect->m_aiValue[0];


			*_lpUpkeepTime		*=	dSYNC_FPS;
			*_lpUpkeepTime		=	*_lpUpkeepTime*(100-iResistanceValue)/100/100;

			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			iValue				=	iValue*(100-iResistanceValue)/100;
			iValue				=	iValue/100;

			m_moveSpeed.correctValue(-iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect );
			*_lpUpkeepTime		=	m_moveSpeed.m_wTime;
			return	TRUE;
		}
		case	eSKILL_EE_STUN_BY_EARTH						:	//	경직 by 대지
		{
			int	iResistanceValue=	max(getEarthResistance(),getStunResistance());

			if	(bIsPVP)
				iResistanceValue=	max(getEarthResistance()+getStunResistance(),0);

			iActiveChance		=	iActiveChance*(100-iResistanceValue)/100;

			if (iRandomValue>=	iActiveChance)
				break;

			if (iRandomValue	>=	iActiveChance)	break;

			*_lpUpkeepTime		-=	*_lpUpkeepTime*iResistanceValue/100;

			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			*_lpUpkeepTime		=	*_lpUpkeepTime*dSYNC_FPS/100;
			m_wStunTime			=	max(m_wStunTime,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_wStunTime;
			return	TRUE;
		}
		case	eSKILL_EE_STONE_BY_EARTH					:	//	석화 by 대지
		{
			int	iResistanceValue=	max(getEarthResistance(),getStoneResistance());

			if	(bIsPVP)
				iResistanceValue=	max(getEarthResistance()+getStoneResistance(),0);

			iActiveChance			=	iActiveChance*(100-iResistanceValue)/100;	//	대지, 석화 저항으로 석화를 % 확률 적용해 저항

			if	(bIsPVP)
				iActiveChance	=	min(iActiveChance,dPVP_TOP_STONE_CHANCE);

			if	(iRandomValue		>=	iActiveChance)
				break;

			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			if (m_wStoneTime		==	0)
				addPacketStop();

			if (bIsPVP)
			{
				*_lpUpkeepTime		=	*_lpUpkeepTime*(100-iResistanceValue)/100/100;
				m_wStoneTime		=	max(m_wStoneTime,*_lpUpkeepTime);

				if (isNEPlayer(_lpAttacker))	//	천적이다.
					m_wStoneTime	=	m_wStoneTime*dPVP_CORRECT_STONE_TIME_NE/100;
				else
					m_wStoneTime	=	m_wStoneTime*dPVP_CORRECT_STONE_TIME/100;

				*_lpUpkeepTime		=	m_wStoneTime*dSYNC_FPS;
			}
			else
			{
				*_lpUpkeepTime		=	*_lpUpkeepTime*(100-iResistanceValue)/100/100;
				m_wStoneTime		=	max(m_wStoneTime,*_lpUpkeepTime);
				*_lpUpkeepTime		=	m_wStoneTime*dSYNC_FPS;
			}
			return	TRUE;
		}

		case	eSKILL_EE_FLEE					:	//도망
		{
			int	iResistanceValue=	getStrangeStatusResistance(TRUE);
			iActiveChance		=	iActiveChance*(100-iResistanceValue)/100;

			if (iRandomValue	>=	iActiveChance)
				break;
			
			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			*_lpUpkeepTime		=	*_lpUpkeepTime*(100-iResistanceValue)/100/100;
			flee(_lpAttacker,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_wFleeTime*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_FLEE_BY_TURNUNDEAD				:	//도망by턴언데드
		{
			if (!isUndead())
				break;
			if (_lpAttacker->getLevel() + _lpAbility->m_wLevel < getLevel())
				break;

			int	iResistanceValue=	max(getLightResistance(),getStrangeStatusResistance(TRUE));

			if	(bIsPVP)
				iResistanceValue=	max(getLightResistance()+getStrangeStatusResistance(TRUE),0);

			iActiveChance		=	iActiveChance*(100-iResistanceValue)/100;

			if (iRandomValue	>=	iActiveChance)
				break;

			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;
			
			*_lpUpkeepTime		=	*_lpUpkeepTime*(100-iResistanceValue)/100/100;
			flee(_lpAttacker,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_wFleeTime*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_STUN_BY_TURNUNDEAD				:	//마비by턴언데드
		{
			if	(!isUndead())
				break;
			if	(_lpAttacker->getLevel() + _lpAbility->m_wLevel < getLevel()  + 15)
				break;

			int	iResistanceValue=	max(getLightResistance(),getStunResistance());

			if	(bIsPVP)
				iResistanceValue=	max(getLightResistance()+getStunResistance(),0);

			iActiveChance		=	iActiveChance*(100-iResistanceValue)/100;

			if	(iRandomValue	>=	iActiveChance)
				break;
			
			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			*_lpUpkeepTime		=	*_lpUpkeepTime*dSYNC_FPS*(100-iResistanceValue)/100/100;
			m_wStunTime			=	max(*_lpUpkeepTime,m_wStunTime);
			*_lpUpkeepTime		=	m_wStunTime;
			return	TRUE;
		}

		case	eSKILL_EE_INSTANCE_KILL_TURNUNDEAD			:	//즉사by턴언데드
		{
			if	(!isUndead(TRUE))
				break;
			if	(_lpAttacker->getLevel() + _lpAbility->m_wLevel < getLevel()  + 45)
				break;

			int	iResistanceValue=	max(getLightResistance(),getCurseResistance());

			iResistanceValue	=	max(m_bInstanceKillResistance,iResistanceValue);

			if	(bIsPVP)
				iResistanceValue=	max(getLightResistance()+m_bInstanceKillResistance+getCurseResistance(),0);

			iActiveChance		=	iActiveChance*(100-iResistanceValue)/100;

			cJOB	*lpJob		=	getJob();

			if	(lpJob)
				if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_SEMI_BOSS1)
					iActiveChance	=	0;
			
			int	iDamage			=	m_iHP;

			if	(bIsPVP)
			{
				if	(bIsNEP)	//	천적이다.
				{
					iDamage		=	m_iHP*dPVP_INSTANCE_KILL_FACTOR_NE/100;
					iActiveChance=	iActiveChance*dPVP_CORRECT_INSTANCE_KILL_CAHNCE_NE/100;
				}
				else
				{
					iDamage		=	m_iHP*dPVP_INSTANCE_KILL_FACTOR/100;
					iActiveChance	=	iActiveChance*dPVP_CORRECT_INSTANCE_KILL_CAHNCE/100;
				}
			}

			if	(iRandomValue	>=	iActiveChance)
				break;

			damagedBy(_lpAttacker,_lpAbility,iDamage,0);
			m_bf1IsInstanceKill	=	TRUE;

			return	TRUE;
		}

		case	eSKILL_EE_CURE_POISON						:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			m_wPoisonTime	=	0;
			return	TRUE;
		}

		case	eSKILL_EE_POISON							:
		{
			int	iPoisonResistance	=	getPoisonResistance();
			int	iEarthResistance	=	getEarthResistance()-_lpAttacker->getWeakningTargetEarthResistance();
			int	iValue1				=	_lpEffect->m_aiValue[0];
			int	iValue2				=	_lpEffect->m_aiValue[1];
			int	iDamage				=	(iValue1+iValue2*iSkillLevel);

			iDamage					+=	(int)(iDamage*_lpAttacker->getIntelligence()*0.005f);

			int	iLevelCorrectDamagePercentage	=	0;

			if	(bIsPVP)
				iLevelCorrectDamagePercentage		=	(m_iLevel-_lpAttacker->m_iLevel)*30/25;

			BOOL	bIsBossBattle	=	FALSE;
			if	(m_wIsBoss	||	_lpAttacker->m_wIsBoss)
				bIsBossBattle	=	TRUE;

			if	(bIsBossBattle)
				iLevelCorrectDamagePercentage	=	(m_iLevel-_lpAttacker->m_iLevel)*30/25;

			int	iCorrectDamage		=	_lpAttacker->getStrengtheningEarthDamage(0,FALSE)+iLevelCorrectDamagePercentage;

			*_lpUpkeepTime			=	*_lpUpkeepTime*(100-iPoisonResistance)/100;
			iDamage					=	iDamage*(100-iEarthResistance)/100;

			iDamage					=	iDamage+iDamage*iCorrectDamage/100;

			if	(*_lpUpkeepTime	<=	0	||	iDamage	<=	0)
				break;

			*_lpUpkeepTime			/=	100;
			*_lpUpkeepTime			=	max(*_lpUpkeepTime,1);

			if	(bIsPVP)
			{
				if	(isNEPlayer(_lpAttacker))	//	천적이다.
					iDamage		=	GetP2PCorrectDamage(iDamage);//	중독
				else
					iDamage		=	GetP2PCorrectDamage(iDamage);//	중독

				*_lpUpkeepTime	=	*_lpUpkeepTime*dPVP_POISON_TIME_FACTOR/100;//	즉사
			}

			setPoisonDamage(*_lpUpkeepTime,iDamage,_lpAttacker->m_wSerialInField);
			*_lpUpkeepTime			=	m_wPoisonTime*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_BY_OWNER_FORCE			:
		{
			int	iHitRange		=	_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel;
			int	iSameJobCount	=	m_lpField->getSameJobCount(this,iHitRange);

			if (iSameJobCount	<=	0)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iDamage			=	(iValue1+iValue2*iSkillLevel)/100;

			*_lpUpkeepTime			/=	100;
			m_attackPower.correctPercentageValue(iDamage,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			*_lpUpkeepTime			=	m_attackPower.m_wTimeForCorrectValue*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_CHANGE_ALLIGNMENT					:
		{
			if (iRandomValue	>=	iActiveChance)	break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iValue			=	(iValue1+iValue2*iSkillLevel)/100;

			*_lpUpkeepTime		/=	100;
			m_allignment.correctValue(iValue,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_allignment.m_wTime*dSYNC_FPS;
			return	TRUE;
		}

		case	eSKILL_EE_DECLINE_HIT_CHANCE_BY_LIGHT	:	//	빛에 의한 명중률 저하
			{
				iActiveChance		=	iActiveChance*(100-getLightResistance())/100;
				
				if (iRandomValue	>=	iActiveChance)
					break;
				
				*_lpUpkeepTime			-=	*_lpUpkeepTime*getLightResistance()/100;
				
				if	(*_lpUpkeepTime%100)
					*_lpUpkeepTime	=	*_lpUpkeepTime/100+1;
				else
					*_lpUpkeepTime	/=	100;
				
				if	(m_wCurseAmplificationTime)
					*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;
				
				m_hitChance.correctValue(-dDECLINE_HIT_CHANCE_VALUE,*_lpUpkeepTime, bIsCanNotApplySameEffect);
				*_lpUpkeepTime			=	m_hitChance.m_wTime*dSYNC_FPS;
				return	TRUE;
			}
		case	eSKILL_EE_DECLINE_DODGE_CHANCE_BY_LIGHT	:	//	빛에 의한 회피율 저하
			{
				iActiveChance		=	iActiveChance*(100-getLightResistance())/100;
				
				if (iRandomValue	>=	iActiveChance)
					break;
				
				*_lpUpkeepTime		-=	*_lpUpkeepTime*getLightResistance()/100;
				
				if (*_lpUpkeepTime%100)	*_lpUpkeepTime=*_lpUpkeepTime/100+1;
				else					*_lpUpkeepTime/=100;
				
				if (m_wCurseAmplificationTime)
					*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;
				
				m_dodgeChance.correctValue(-dDECLINE_DODGE_CHANCE_VALUE,*_lpUpkeepTime,bIsCanNotApplySameEffect );
				*_lpUpkeepTime		=	m_dodgeChance.m_wTime*dSYNC_FPS;
				return	TRUE;
			}
		case	eSKILL_EE_BLIND_BY_LIGHT				:	//	빛에 의한 장님
			iActiveChance		=	iActiveChance*(100-getLightResistance())/100;
			if (iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		-=	*_lpUpkeepTime*getLightResistance()/100;

			if (*_lpUpkeepTime%100)	*_lpUpkeepTime=*_lpUpkeepTime/100+1;
			else					*_lpUpkeepTime/=100;

			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			m_wBlindTime		=	max(m_wBlindTime,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_wBlindTime*dSYNC_FPS;

			if	(_lpAttacker && _lpAttacker->isPureMonster())
				_lpAttacker->reaimTarget();

			return	TRUE;

		case	eSKILL_EE_LIGHT_ATTACK_ADDITIONAL_EFFECT	:
		{
			BOOL	bIsHit	=	FALSE;

			iActiveChance	=	iActiveChance*(100-getLightResistance())/100;

			if (iRandomValue>=	iActiveChance)
				break;

			*_lpUpkeepTime	-=	*_lpUpkeepTime*getLightResistance()/100;

			if	(*_lpUpkeepTime%100)
				*_lpUpkeepTime=*_lpUpkeepTime/100+1;
			else
				*_lpUpkeepTime/=100;

			m_hitChance.correctValue(-dDECLINE_HIT_CHANCE_VALUE,*_lpUpkeepTime,bIsCanNotApplySameEffect);
			m_dodgeChance.correctValue(-dDECLINE_DODGE_CHANCE_VALUE,*_lpUpkeepTime,bIsCanNotApplySameEffect );

			*_lpUpkeepTime		=	*_lpUpkeepTime*dSYNC_FPS;
			
			return	bIsHit;
		}
//	어둠:	지속 시간 동안 광범위 저주 효과 추가
//			광범위 저주 -> 이 저주가 걸릴 당시에 걸려 있던 각종 상태 이상과
//			이 저주의 지속 시간 동안 걸리는 상태 이상의 지속 시간이 2배가 됨
		case	eSKILL_EE_CURSE_AMPLIFICATION				:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime				=	(*_lpUpkeepTime)*(100-getDarkResistance())/100;

			m_wCurseAmplificationTime	=	*_lpUpkeepTime*dSYNC_FPS/100;

			return	TRUE;
		}

		case	eSKILL_EE_LIFE_DRAIN						:
		{
			int	iValue1	=	_lpEffect->m_aiValue[0];
			int	iValue2	=	_lpEffect->m_aiValue[1];
			int	iFactor	=	(iValue1+iValue2*iSkillLevel)/100;

			iFactor		=	iFactor*(100-getCurseResistance())/100;

			_lpAttacker->absorbDamage(_lpAttacker->m_wSerialInField,_iHitDamage*iFactor/100);
			return	TRUE;
		}

		case	eSKILL_EE_RECOVER_HP						:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iValue1	=	_lpEffect->m_aiValue[0];
			int	iValue2	=	_lpEffect->m_aiValue[1];
			int	iHealHP	=	iValue1+iValue2*iSkillLevel;

			if (g_iSeasonVariable >= 1)//JBC	미니펫 2차 강화 펫 상처 치료 + 펫 리제네레이션 효과 유저 레벨 연동.	08-11-03
			{
				int iTempVlue3 = _lpEffect->m_aiValue[3];
				if(iTempVlue3 != 0)
					iHealHP += _lpEffect->m_aiValue[2] / 100 + m_iLevel * 100 / iTempVlue3;
				
			}
			correctValueEffectDuelServer(&iHealHP , lpSkill);
			return	recoverHPByAidSkill(_lpAbility->m_wSkill,iHealHP);
		}

		case	eSKILL_EE_SLEEP								:
		{
			if	(m_wSleepTime)
				break;

			BOOL	bIsCanNotRegistChance	=	_lpEffect->m_aiValue[0];
				
			if	(bIsCanNotRegistChance		==	FALSE)
				iActiveChance	=	iActiveChance*(100-getSleepResistance())/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			if	(m_wSleepTime	==	0)
				m_lpField->addSendEtcWork(m_wSerialInField,eEW_SLEEP_TARGET,m_wSerialInField);

			m_wSleepTime	=	*_lpUpkeepTime*(100-getSleepResistance())/100;
			m_wSleepTime	/=	100;
			*_lpUpkeepTime	=	m_wSleepTime*dSYNC_FPS;

			if	(lpSkill->m_bf1NotRevenge	==	FALSE)
				_lpAttacker->reaimTarget(_lpAttacker);

			return	TRUE;
		}

		case	eSKILL_EE_REDUCE_STRANGE_STATUS_TIME		:
		{
			if	(!isStrangeStatus())	
				return	FALSE;	//	상태 이상 상태
			if	(iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iReduceTime	=	(iValue1+iValue2*iSkillLevel)/100;

			reduceStrangeStatusTime(iReduceTime);
			return	TRUE;
		}
		case	eSKILL_EE_REDUCE_CURSE_TIME					:
		{
			if	(!isCurseStatus())	
				return	FALSE;	//	상태 이상 상태
			if	(iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iReduceTime	=	(iValue1+iValue2*iSkillLevel)/100;

			reduceCurseTime(iReduceTime);

			return	TRUE;
		}
		case	eSKILL_EE_REDUCE_DECLINE_POWER_STATUS_TIME	:
		{
			if	(!isDeclinePowerStatus())	
				return	FALSE;	//	상태 이상 상태
			if	(iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iReduceTime	=	(iValue1+iValue2*iSkillLevel)/100;

			reduceDeclinePowerStatusTime(iReduceTime);

			return	TRUE;
		}

		case	eSKILL_EE_CURE_STRANGE_STATUS				:	//이상계통상태이상아군치료
		{
			if (!isWrongStatus())	
				return	FALSE;	//	상태 이상 상태

			int iAddValue	=	0;

			//JBC	펫 치료(Pet Cure) 발동확률 유저 레벨 연동.	08-11-03
			if(g_iSeasonVariable >= dSEASON_VARIABLE_MINIPET_POWER_UP_2)		//1시즌 이후 발동
			{
				int	iValue1		=	_lpEffect->m_aiValue[0];		//가중치.
				int	iValue2		=	_lpEffect->m_aiValue[1];
				
				
				if(iValue2 != 0)
					iAddValue = iValue1 / 100 + m_iLevel / iValue2;
			}

			if (iRandomValue>=	iActiveChance + iAddValue)
				break;

			cureWrongStatus();

			return	TRUE;
		}

		case	eSKILL_EE_CURE_CURSE						:	//저주계통상태이상아군치료
		{
			if (!isCurseStatus())		
				return	FALSE;	//	저주 상태
			if (iRandomValue>=	iActiveChance)
				break;

			cureCurseStatus();
			
			return	TRUE;
		}

		case	eSKILL_EE_CURE_DECLINE_POWER_STATUS				:	//저하계통상태이상아군치료
		{
			if	(!isDeclinePowerStatus())	
				return	FALSE;	//	능력치 저하 상태

			if	(iRandomValue>=	iActiveChance)
				break;

			cureDeclinePowerStatus();

			return	TRUE;
		}

		case	eSKILL_EE_MARIONETTE						:	//	마리오네뜨
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			*_lpUpkeepTime	/=	100;

			int	iBoostPower	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;
			int	iLimit		=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel)/100;

			iBoostPower		=	min(iBoostPower,iLimit);

			return	makeMarionette(_lpAttacker,*_lpUpkeepTime,iBoostPower);
		}

		case	eSKILL_EE_DISPLACEMENT						:	//	빙의
		{
			int	iResistanceValue=	max(getStrangeStatusResistance(TRUE),getCurseResistance());

			iActiveChance		=	iActiveChance*(100-iResistanceValue)/100;

			if	(iRandomValue	>=	iActiveChance)
				break;

			if	(m_wCharmedTime)
				break;

			if	(m_wMarionetteTime)
				break;

			if	(m_wLordOperator != 0xffff || m_wLordActor != 0xffff)
				break;

			if (isOverSemiBossMonster())
				break;

			m_lpField->bookingDisplacement(_lpAttacker,this,FALSE);

			return	TRUE;
		}

		case	eSKILL_EE_DETECTING							:	//	빙의
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			*_lpUpkeepTime		/=	100;

			int	iDetectLevel	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;

			m_detecting.correctValue(iDetectLevel,*_lpUpkeepTime);

			return	TRUE;
		}
/*
		case	eSKILL_EE_DETECTING_MONSTER					:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iCount		=	(iValue1+iValue2*iSkillLevel);
			int	iLimitLevel	=	getLevel()+iValue3*iSkillLevel;

			int	iRange		=	_lpAbility->getHitRange(getWeapon());
			int	iLimitType	=	9;	
			int	aLimitType[]=
			{
				1,2,
				3,5,
				6,9,
				10,15,
				16,20,
				21,25,
				26,30,
				31,35,
				36,40,
				41,500,
				0xffff,
			};

			for (int i=0;;i++)
				if (iSkillLevel >= aLimitType[i*2] && iSkillLevel <= aLimitType[i*2+1] )
				{
					iLimitType	=	i;
					break;
				}

			int	iTargetCount	=	m_lpField->detectingEvil(this,iRange,iCount,iLimitLevel,iLimitType);

			if (iTargetCount	==	0)
				return	FALSE;
			break;
		}
*/
		case	eSKILL_EE_DETECTING_EVIL					:
		{
			if (iRandomValue	>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iCount		=	(iValue1+iValue2*iSkillLevel);
			int	iLimitLevel	=	getLevel()+iValue3*iSkillLevel;

			int	iRange		=	_lpAbility->getHitRange(getWeapon());
			int	iLimitType	=	9;	
			int	aLimitType[]=
			{
				1,2,
				3,5,
				6,9,
				10,15,
				16,20,
				21,25,
				26,30,
				31,35,
				36,40,
				41,500,
				0xffff,
			};

			for (int i=0;;i++)
				if (iSkillLevel >= aLimitType[i*2] && iSkillLevel <= aLimitType[i*2+1] )
				{
					iLimitType	=	i;
					break;
				}

			if	(!m_lpField)
				break;

			int	iTargetCount	=	m_lpField->detectingEvil(this,iRange,iCount,iLimitLevel,iLimitType);

			if (iTargetCount	==	0)
				return	FALSE;
			break;
		}

		case	eSKILL_EE_DESTROYING_UNDEAD					:	//	디스트로잉언데드	,
		{
			if	(operateDestroyingUndead(_lpAttacker,_lpAbility))
				return	TRUE;
			break;
		}
		case	eSKILL_EE_DESTROYING_UNHOLY					:	//	디스트로잉언홀리	,
		{
			if	(operateDestroyingUnholy(_lpAttacker,_lpAbility))
				return	TRUE;
			break;
		}

		case	eSKILL_EE_DISPELLING						:	//	디스펠링
		{
			iActiveChance	=	iActiveChance*(100-getDeclinePowerResistance())/100;

			if	(iRandomValue>=	iActiveChance)
				break;

			dispell();

			return	TRUE;
		}

		case	eSKILL_EE_CHARGE_CP							:	//	CP감소
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int iValue3		=	_lpEffect->m_aiValue[2];
			int iValue4		=	_lpEffect->m_aiValue[3];
			int	iIgnoreSelf	=	_lpEffect->m_aiValue[5];
			int	iIncreaseCP	=	(iValue1+iValue2*iSkillLevel);
			int iAddValue	=	0;

			if	(iIgnoreSelf	&&	_lpAttacker->m_wSerialInField	==	m_wSerialInField)
				break;

			if	(m_iCP	==	getMaxCP())
				break;

			//JBC	펫차지 스킬2차 수정 적용..		08-11-03
			if(g_iSeasonVariable >= dSEASON_VARIABLE_MINIPET_POWER_UP_2)	
			{
				if(iValue4 != 0)
					iAddValue = iValue3 + m_iLevel * 100 / iValue4;
			
			}

			increaseCP(iIncreaseCP + iAddValue);

			if	(isPlayer())
				syncPlayerBasicStatus(TRUE);

			return	TRUE;
		}

		case	eSKILL_EE_REDUCE_CP							:	//	CP감소
		{
			int	iResistanceFactor	=	_lpEffect->m_aiValue[5];

			if	(iResistanceFactor)
			{
				int	iResistance	=	max(getDeclinePowerResistance(),0);

				iActiveChance	=	iActiveChance*(100-iResistance)/100;
			}

			if	(iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iReduceCP	=	(iValue1+iValue2*iSkillLevel);

			reduceCP(iReduceCP,_lpAbility->m_wSkill);

			if	(isPlayer())
				syncPlayerBasicStatus(TRUE);

			return	TRUE;
		}

		case	eSKILL_EE_REDUCE_CP_PERCENTAGE					:	//	CP감소
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			int	iValue1				=	_lpEffect->m_aiValue[0];
			int	iValue2				=	_lpEffect->m_aiValue[1];
			int	iLimit				=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel)/100;
			int	iReduceCPPercentage	=	(iValue1+iValue2*iSkillLevel)/100;

			iReduceCPPercentage		=	min(iReduceCPPercentage,iLimit);
			
			int	iReduceCP			=	max(m_iCP*iReduceCPPercentage/100,0);

			reduceCP(iReduceCP,_lpAbility->m_wSkill);

			if	(isPlayer())
				syncPlayerBasicStatus(TRUE);

			return	TRUE;
		}
		
		case	eSKILL_EE_CHARMING_PET						:	//	유혹	
		{
			if	(getCharmingResistance()	>=	100)
				break;

			iActiveChance	=	iActiveChance*(100-getCharmingResistance())/100;

			if	(iRandomValue>=	iActiveChance)	
				break;

			*_lpUpkeepTime	=	*_lpUpkeepTime*(100-getCharmingResistance())/100;
			*_lpUpkeepTime	/=	100;

			if	(*_lpUpkeepTime	==	0)
				break;

			charm(_lpAttacker,*_lpUpkeepTime);

			*_lpUpkeepTime	=	*_lpUpkeepTime*dSYNC_FPS;

			return	TRUE;
		}

		case	eSKILL_EE_CHARMING							:	//	유혹
		{
			if	(bIsPVP || g_bIsDuelServer)
				break;

			if	(m_wLordActor!=	0xffff)
				break;
			
			if	(getCharmingResistance()	>=	100)
				break;

			iActiveChance	=	iActiveChance*(100-getCharmingResistance())/100;

			if	(iRandomValue>=	iActiveChance)	
				break;

			*_lpUpkeepTime	=	*_lpUpkeepTime*(100-getCharmingResistance())/100;
			*_lpUpkeepTime	=	*_lpUpkeepTime*dSYNC_FPS;
			*_lpUpkeepTime	/=	100;

			if	(*_lpUpkeepTime	==	0)
				break;

			int	iIncreaseStrength	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;
			int	iIncreaseIntelligence=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel)/100;

			m_strength.correctValue(iIncreaseStrength,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			m_intelligence.correctValue(iIncreaseStrength,*_lpUpkeepTime, bIsCanNotApplySameEffect);

			charm(_lpAttacker,*_lpUpkeepTime/dSYNC_FPS);

			return	TRUE;
		}

		case	eSKILL_EE_SANCTUARY							:
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			if	(m_wInvincibleTime)
			{
				m_wInvincibleTime	=	0;
				m_wSealActionTime	=	0;
				m_wSealRunningTime	=	0;
				*_lpUpkeepTime		=	1;

				return	TRUE;
			}

			int	iUpkeepSecond	=	(*_lpUpkeepTime)/100*dSYNC_FPS;

			iUpkeepSecond		=	max(iUpkeepSecond,1);
			m_wInvincibleTime	=	iUpkeepSecond;
			m_wSealActionTime	=	iUpkeepSecond;
			m_wSealRunningTime	=	iUpkeepSecond;
			m_wUnsealAction		=	_lpAbility->m_wSkill;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iValue			=	(iValue1+iValue2*iSkillLevel)/100;

			*_lpUpkeepTime		=	iUpkeepSecond;

			m_moveSpeed.correctValue(iValue,iUpkeepSecond, bIsCanNotApplySameEffect);

			setMoveAbility(FALSE);
			sendMoveState();

			return	TRUE;
		}

		case	eSKILL_EE_WARP_NEAREST_VILLAGE	:
		{
			if	(m_lpField->m_bIsGuildBattleField)
				break;

			if	(m_lpField->isCanNotUseTeleportEffectField())
			{
				sendRegistServerMessage(eRSM_CAN_NOT_TELEPORT_FIELD,eSM_SYSTEM);

				break;
			}

			if	(m_wPitchmanShopSerial	!=	0xffff)
				break;
			
			if	(iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];

			int	iLimitRange	=	iValue1+iValue2*iSkillLevel;
			int	iCount		=	(iValue3+iValue4*iSkillLevel)/100;

			return	readyWarpNearestVillage(iLimitRange,iCount,FALSE,_lpAbility->m_wSkill);
		}

		case	eSKILL_EE_CALLING	:
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];

			int	iLimitRange	=	iValue1+iValue2*iSkillLevel;

			return	calling(iLimitRange,_lpAbility->m_wSkill);
		}

		case	eSKILL_EE_CREATE_TOWN_PORTAL	:
		{
			if	(m_lpField->m_bIsGuildBattleField	||	m_lpField->isCanNotUseTeleportEffectField())
			{
				sendRegistServerMessage(eRSM_CAN_NOT_TELEPORT_FIELD,eSM_SYSTEM);

				break;
			}

			if	(m_wPitchmanShopSerial	!=	0xffff)
				break;

			if	(iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];

			int	iLimitRange	=	iValue1+iValue2*iSkillLevel;
			int	iCount		=	(iValue3+iValue4*iSkillLevel)/100;

			*_lpUpkeepTime	/=	100;

			return	readyWarpNearestVillage(iLimitRange,iCount,TRUE,_lpAbility->m_wSkill,*_lpUpkeepTime);
		}

		case	eSKILL_EE_INCLINE_HIT_CHANCE:
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			*_lpUpkeepTime	/=	100;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;

			m_hitChance.correctValue(iValue,*_lpUpkeepTime,bIsCanNotApplySameEffect );

			return	TRUE;
		}
		
		case	eSKILL_EE_INCREASE_LEVEL	:
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	bIsRoundOff	=	_lpEffect->m_aiValue[4];
			int	bIsSyncState=	_lpEffect->m_aiValue[5];

			int	iAddLevel	=	iValue1+iValue2*iSkillLevel;
			int	iLimitLevel	=	(iValue3+iValue4*iSkillLevel)/100;

			if	(bIsRoundOff)
				if	((iAddLevel%100) >=	50)
					iAddLevel+=	50;

			iAddLevel		/=	100;

			if	(iLimitLevel )
				iAddLevel		=	min(iAddLevel,iLimitLevel);

			*_lpUpkeepTime	/=	100;

			m_level.correctValue(iAddLevel,*_lpUpkeepTime, bIsCanNotApplySameEffect);

			{
				m_strength.correctValue(iAddLevel,*_lpUpkeepTime, bIsCanNotApplySameEffect);
				m_constitution.correctValue(iAddLevel,*_lpUpkeepTime, bIsCanNotApplySameEffect);
				m_agility.correctValue(iAddLevel,*_lpUpkeepTime, bIsCanNotApplySameEffect);
				m_charisma.correctValue(iAddLevel,*_lpUpkeepTime, bIsCanNotApplySameEffect);
				m_intelligence.correctValue(iAddLevel,*_lpUpkeepTime, bIsCanNotApplySameEffect);
			}

			sendEtcWork(eEW_LEVEL_DRAIN,m_wSerialInField,m_level.m_sCorrectValue,m_level.m_wTime,TRUE,getLevel(),0);

			if	(isPlayer())
				buildPower();
			else
				rebuildForUpdateLevel();

			cParty	*lpParty	=	getParty();

			if	(lpParty)
			{
				g_pPartyManager->sendWPChangeMemberInfo(getPartySerial(),m_strName,getLevel(),m_bf4SecretDungeonPlayCount,m_wJob,m_bf3GuildDungeonPlayTime);
				sendEtcWorkToPartyMember(TRUE,eEW_LEVEL_DRAIN,m_wSerialInField,m_level.m_sCorrectValue,m_level.m_wTime,TRUE,getLevel(),1);
			}

			return	TRUE;
		}

		case	eSKILL_EE_LEVEL_DRAIN		:
		{
			iActiveChance	=	iActiveChance*(100-getCurseResistance())/100;

			if	(iRandomValue>=	iActiveChance)
				break;

			m_iMaxHPForDrainLevel	=	getMaxHP();

			int	iValue1				=	_lpEffect->m_aiValue[0];
			int	iValue2				=	_lpEffect->m_aiValue[1];
			int	iValue3				=	_lpEffect->m_aiValue[2];
			int	iValue4				=	_lpEffect->m_aiValue[3];
			int	iRoundOff			=	_lpEffect->m_aiValue[4];
			int	isReiteration		=	_lpEffect->m_aiValue[5];	//	중첩된다.

			int	iDrainLevel		=	iValue1+iValue2*iSkillLevel;
			int	iLimitDrainLevel=	(iValue3+iValue4*iSkillLevel)/100;
			
			if	(iRoundOff	&&	(iDrainLevel%100)	>=	50)
				iDrainLevel	+=	50;

			iDrainLevel		/=	100;

			if	(m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			*_lpUpkeepTime	=	*_lpUpkeepTime*(100-getCurseResistance())/100;
			*_lpUpkeepTime	/=	100;

			CTitle	*lpTitle	=	g_titleManager.getTitle(eTN_PORTER);

			for (int i = 0 ; i < dMAX_PLAYER_TITLE_COUNT ; i++)
			{
				if(m_aTitle[i].m_bTitle == eTN_PORTER)
					break;
			}

			int iRequireLevel =	lpTitle->getRequireLevel(m_aTitle[i].m_bLevel);

			if(iRequireLevel > m_iLevel - iDrainLevel)
			{
				iDrainLevel = max(0 , m_iLevel - iRequireLevel);
			}
			
			if	(isReiteration)
			{      
				m_level.m_sCorrectValue		-=	iDrainLevel;

				if	(m_level.m_sCorrectValue	<	-iLimitDrainLevel)
					m_level.m_sCorrectValue		=	-iLimitDrainLevel;

				m_level.m_wTime	=	*_lpUpkeepTime;
			}
			else
				m_level.correctValue(-iDrainLevel,*_lpUpkeepTime, bIsCanNotApplySameEffect);

			{
				m_strength.correctValue(-iDrainLevel,*_lpUpkeepTime,bIsCanNotApplySameEffect);
				m_constitution.correctValue(-iDrainLevel,*_lpUpkeepTime,bIsCanNotApplySameEffect);
				m_agility.correctValue(-iDrainLevel,*_lpUpkeepTime,bIsCanNotApplySameEffect);
				m_charisma.correctValue(-iDrainLevel,*_lpUpkeepTime,bIsCanNotApplySameEffect);
				m_intelligence.correctValue(-iDrainLevel,*_lpUpkeepTime,bIsCanNotApplySameEffect);
			}

			*_lpUpkeepTime	=	m_level.m_wTime*dSYNC_FPS;

			m_lpField->addSendEtcWork(m_wSerialInField,eEW_LEVEL_DRAIN,m_wSerialInField,m_level.m_sCorrectValue,m_level.m_wTime,TRUE,getLevel(),0);

			if	(isPlayer())
				buildPower();
			else
				rebuildForUpdateLevel();

			cParty	*lpParty	=	getParty();

			if	(lpParty)
			{
				g_pPartyManager->sendWPChangeMemberInfo(getPartySerial(),m_strName,getLevel(),m_bf4SecretDungeonPlayCount,m_wJob,m_bf3GuildDungeonPlayTime);
				sendEtcWorkToPartyMember(TRUE,eEW_LEVEL_DRAIN,m_wSerialInField,m_level.m_sCorrectValue,m_level.m_wTime,TRUE,getLevel(),1);
			}

			return	TRUE;
		}

		case	eSKILL_EE_DECREASE_BLOCKING_CHANCE	:
		{
			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;

			if (m_wCurseAmplificationTime)
				*_lpUpkeepTime	=	*_lpUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

			*_lpUpkeepTime	/=	100;

			m_blockingChance.correctValue(-iValue,*_lpUpkeepTime);
				
			return	TRUE;
		}

		case	eSKILL_EE_TAME						:
		{
			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue5		=	_lpEffect->m_aiValue[4];

			if	(m_wCharmedTime)
				break;
			
			cCHARACTER	*lpCharacter	=	getCharacter();

			if	(!lpCharacter)
				break;

			if	(lpCharacter->m_bf1IsBlockToTame)
			{
				_lpAttacker->sendRegistServerMessage(eRSM_IS_CAN_NOT_TAME_MONSTER,eSM_SYSTEM);
				break;
			}

			cJOB	*lpJob	=	getJob();

			if (!lpJob || lpJob->m_bf1IsEnableTameMonster == FALSE || m_lpField->m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG)	//	가능한 등급
			{
				_lpAttacker->sendRegistServerMessage(eRSM_IS_CAN_NOT_TAME_MONSTER,eSM_SYSTEM);
				break;
			}
			if	(iValue4 <=	lpJob->m_wMonsterLevel)	//	가능한 등급
			{
				_lpAttacker->sendRegistServerMessage(eRSM_IS_CAN_NOT_TAME_THAT_MONSTER_BY_THIS_SKILL,eSM_SYSTEM);
				break;
			}
			if	(_lpAttacker->getPetCount() >= dOWN_PET_COUNT)
			{
				_lpAttacker->sendRegistServerMessage(eRSM_IS_CAN_NOT_TAME_NO_MORE_MONSTER,eSM_SYSTEM);
				break;
			}

			if	(_lpAttacker->getPetCount() >= iValue5)
			{
				_lpAttacker->sendRegistServerMessage(eRSM_IS_CAN_NOT_TAME_NO_MORE_MONSTER_BY_THIS_SKILL,eSM_SYSTEM);
				break;
			}

			if	(bIsPVP)
				break;

			if (m_wLordActor!=	0xffff)
				break;
			
			int	iCharmingResistance	=	getCharmingResistance();

			iActiveChance	=	iActiveChance*(100-getCharmingResistance())/100;
			
			if	(iRandomValue		>=	iActiveChance)
				break;
			if	(random(lpJob->m_bf15TameResistanceValue)!=	0)
				break;

			int			iValue			=	min((iValue1+iValue2*iSkillLevel),iValue3)/100;
			int			iTamedLevel		=	getLevel()*iValue/100;
			int			iTamedLoyalty	=	0;

			iTamedLevel		=	max(iTamedLevel,1);

			cAbility	*lpGlareAbility	=	_lpAttacker->getGlareSkill(TRUE);

			if	(lpGlareAbility)
			{
				cAbility	glareAbility;
				int			iCorrectSkillLevel	=	_lpAttacker->getCorrectSkillLevel(lpGlareAbility);

				glareAbility.copy(lpGlareAbility);
				glareAbility.m_wLevel	+=	iCorrectSkillLevel;

				if	(glareAbility.isActivatePassiveSkill(_lpAttacker->m_iCP))
				{
					int	iCorrectLevel=	min((500+glareAbility.m_wLevel*40)/100,30);

					iTamedLoyalty	=	glareAbility.m_wLevel*2;
					iTamedLevel		+=	iCorrectLevel*m_iLevel/100;
				}
			}

			m_lpField->bookingMonsterTame(_lpAttacker->m_strName,iTamedLevel,iTamedLoyalty,m_wSerialInField,m_iZoneSerial);

			return	TRUE;
		}

		case	eSKILL_EE_NONE_TARGET				:	//	타겟 제외 상태
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			if	(m_sNonTargetTime	<	0 || m_sInvisivilityTime || m_sBlurTime)
				return	TRUE;

			m_sNonTargetTime	=	max(*_lpUpkeepTime/100,m_sNonTargetTime);
			*_lpUpkeepTime		=	m_sNonTargetTime;
			m_wHidingLevel		=	dNON_TARGET_HIDNIG_LEVEL;

			return	TRUE;
		}

		case	eSKILL_EE_REAIM_TARGET				:	//	타겟 재 설정
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			reaimTarget(_lpAttacker);

			return	TRUE;
		}

		case	eSKILL_EE_BREEDING_RECORD			:	//	타겟 재 설정
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			int		iLimitLevel	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;

			if	(m_iLevel	>	iLimitLevel)
				break;

			return	transBreedingRecordBook();
		}

		case	eSKILL_EE_FIRST_AID					:
		{
			BOOL	bIsProportionToRemainHPAndHitDamage	=	_lpEffect->m_aiValue[4];
			BOOL	bIsWantSettingUpkeepTime			=	_lpEffect->m_aiValue[5];
			int		iResistance							=	getCurseResistance();

			if	(bIsWantSettingUpkeepTime	==	FALSE)
				*_lpUpkeepTime	=	0;
			else
				*_lpUpkeepTime	/=	100;

			if	(bIsProportionToRemainHPAndHitDamage)
			{
				if	(iResistance>=	100)
					break;

				iActiveChance	=	iActiveChance*(100-iResistance)/100;
			}

			if	(iRandomValue	>=	iActiveChance)
				break;

			int		iValue1		=	_lpEffect->m_aiValue[0];
			int		iValue2		=	_lpEffect->m_aiValue[1];
			int		iValue		=	iValue1+iValue2*iSkillLevel;
			int		iLimit		=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel);

			if	(iValue	==	0)
				iValue	=	getMaxHP()-m_iHP;

			if	(bIsProportionToRemainHPAndHitDamage)
			{
				iLimit	/=	100;
				iValue	/=	100;
				iValue	=	iValue*(100-iResistance)/100;

				iValue	=	min(iValue,iLimit);

				LONGLONG	llValue	=	m_iHP;

				llValue	=	llValue*iValue/100;
				iValue	=	max((int)llValue,100);

				reduceHP(iValue,_lpAttacker);
			}

			if(g_iSeasonVariable >= dSEASON_VARIABLE_MINIPET_POWER_UP_2)//JBC	미니펫 2차 강화 펫 응급처치 효과 유저 레벨 연동.	08-11-03
			{
				//0으로 나누는 일이 없게 하는 작업.
				int iTempValue7 = _lpEffect->m_aiValue[7];
				if(_lpEffect->m_aiValue[7] != 0)
					iValue += _lpEffect->m_aiValue[6] / 100 + m_iLevel * 100 / iTempValue7;
			}

			if	(!firstAid(iValue,*_lpUpkeepTime))
				break;

			*_lpUpkeepTime	=	m_wFirstAidTime;

			return	TRUE;
		}

		case	eSKILL_EE_BONUS_EXP					:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iLimitLevel	=	_lpEffect->m_aiValue[4];

			int	iBonusExp	=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (m_iLevel	>=	_lpAttacker->m_iLevel+iLimitLevel)
				break;
			
			if (iLimit)
				iBonusExp	=	min(iBonusExp,iLimit);

			*_lpUpkeepTime	/=	100;
			
			m_bonusExp.correctValue(iBonusExp,*_lpUpkeepTime);
			*_lpUpkeepTime	=	m_bonusExp.m_wTime;

			return	TRUE;
		}

		case	eSKILL_EE_PET_BOOST					:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue5		=	_lpEffect->m_aiValue[4]/100;

			int	iBoostPower	=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimitCount	=	(iValue3+iValue4*iSkillLevel)/100;

			iLimitCount		=	min(iLimitCount,iValue5);

			*_lpUpkeepTime	/=	100;

			return	boostPetPower(iBoostPower,*_lpUpkeepTime,iLimitCount);
		}

		case	eSKILL_EE_STRENGTHENING_FIRE_DAMAGE		:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue			=	min(iValue,iLimit);

			*_lpUpkeepTime	/=	100;

			m_strengtheningFireDamage.correctValue(iValue,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_strengtheningFireDamage.m_wTime;
			return	TRUE;
		}
		case	eSKILL_EE_STRENGTHENING_WATER_DAMAGE	:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue		=	min(iValue,iLimit);

			*_lpUpkeepTime	/=	100;

			m_strengtheningWaterDamage.correctValue(iValue,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_strengtheningWaterDamage.m_wTime;
			return	TRUE;
		}
		case	eSKILL_EE_STRENGTHENING_WIND_DAMAGE		:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue		=	min(iValue,iLimit);

			*_lpUpkeepTime	/=	100;

			m_strengtheningWindDamage.correctValue(iValue,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_strengtheningWindDamage.m_wTime;
			return	TRUE;
		}
		case	eSKILL_EE_STRENGTHENING_EARTH_DAMAGE	:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue		=	min(iValue,iLimit);

			*_lpUpkeepTime	/=	100;

			m_strengtheningEarthDamage.correctValue(iValue,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_strengtheningEarthDamage.m_wTime;
			return	TRUE;
		}
		case	eSKILL_EE_STRENGTHENING_LIGHT_DAMAGE	:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue		=	min(iValue,iLimit);

			*_lpUpkeepTime	/=	100;

			m_strengtheningLightDamage.correctValue(iValue,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_strengtheningLightDamage.m_wTime;
			return	TRUE;
		}
		case	eSKILL_EE_STRENGTHENING_DARK_DAMAGE		:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue		=	min(iValue,iLimit);

			*_lpUpkeepTime	/=	100;

			m_strengtheningDarkDamage.correctValue(iValue,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_strengtheningDarkDamage.m_wTime;
			return	TRUE;
		}
		case	eSKILL_EE_STRENGTHENING_MAGIC_DAMAGE	:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue		=	min(iValue,iLimit);

			*_lpUpkeepTime	/=	100;

			m_strengtheningMagicDamage.correctValue(iValue,*_lpUpkeepTime);
			*_lpUpkeepTime		=	m_strengtheningMagicDamage.m_wTime;
			return	TRUE;
		}

		case	eSKILL_EE_DECLINE_CHARMING_RESISTANCE	:
		{
			int	iResistance	=	getCharmingResistance();

			if (iResistance	>=	100)
				break;

			iActiveChance	=	iActiveChance*(100-iResistance)/100;

			if (iRandomValue>=	iActiveChance)
				break;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];

			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;

			*_lpUpkeepTime	/=	100;

			m_charmingResistance.correctValue(-iValue,*_lpUpkeepTime);

			return	TRUE;
		}

		case	eSKILL_EE_RALLY							:
		{
			if	(m_lpTarget || !isMonster())	//	싸우는 중이라면 무시
				break;

			int	iResistance	=	getConfuseResistance();

			iActiveChance	=	iActiveChance*(100-iResistance)/100;

			if (iRandomValue>=	iActiveChance)
				break;

			*_lpUpkeepTime	/=	100;

			rally(_iX,_iY,*_lpUpkeepTime);

			return	TRUE;
		}

		case	eSKILL_EE_CALL_SUMMON_BEAST		:
		{
			int	iType	=	_lpEffect->m_aiValue[0];

			int	iValue1	=	_lpEffect->m_aiValue[1];
			int	iValue2	=	_lpEffect->m_aiValue[2];
			int	iValue3	=	_lpEffect->m_aiValue[3];
			int	iValue4	=	_lpEffect->m_aiValue[4];
			int	iLevel	=	m_iLevel*(iValue1+iValue2*iSkillLevel)/10000+(iValue3+iValue4*iSkillLevel)/100;

			return	callSummonBeast(_iX,_iY,iType,iLevel);
		}
		case	eSKILL_EE_POWER_UP_SUMMON_BEAST		:
		{
			int	iGrade		=	_lpEffect->m_aiValue[0];
			int	iTargetGrade=	_lpEffect->m_aiValue[5]-1;

			int	iValue1		=	_lpEffect->m_aiValue[1];
			int	iValue2		=	_lpEffect->m_aiValue[2];
			int	iValue3		=	_lpEffect->m_aiValue[3];
			int	iValue4		=	_lpEffect->m_aiValue[4];
			int	iLevel		=	m_iLevel*(iValue1+iValue2*iSkillLevel)/10000+(iValue3+iValue4*iSkillLevel)/100;

			if(iTargetGrade == -1)
				iTargetGrade = m_wSummonBeastGrade;

			iGrade = iGrade+iTargetGrade+1;

			if(_lpEffect->m_aiValue[6])	//	제한단계
			{
				iGrade = min(iGrade,_lpEffect->m_aiValue[6]);
				if(m_wSummonBeastGrade+1>=_lpEffect->m_aiValue[6])
					return TRUE;
					
			}
			if (m_wSummonBeastGrade	!=	iTargetGrade)
				return	TRUE;

			powerUpSummonBeast(iGrade,iLevel);

			return	TRUE;
		}
		case	eSKILL_EE_RIDING_DOG				:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iDestType	=	_lpEffect->m_aiValue[0];

			if (iDestType	!=	m_wSummonBeastType)
				break;

			*_lpUpkeepTime	/=	100;

			int	iCorrectConcentration,iCorrectMoveSpeed;

//	집중력 상승
			{
				int	iValue1			=	_lpEffect->m_aiValue[1];
				int	iValue2			=	_lpEffect->m_aiValue[2];
				iCorrectConcentration=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			}
			{
				int	iValue			=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*_lpAbility->m_wLevel);

				iCorrectMoveSpeed	=	iValue/100;
			}

			rideTamer(*_lpUpkeepTime,iCorrectConcentration,iCorrectMoveSpeed);

			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_SKILL_LEVEL		:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iLimit			=	_lpEffect->m_aiValue[2]/100;

			int	iLevel			=	(iValue1+iValue2*iSkillLevel)/100;

			if (iLimit)
				iLevel	=	min(iLevel,iLimit);

			*_lpUpkeepTime		/=	100;

			correctValueEffectDuelServer(&iLevel , lpSkill);
			m_correctAllSkillLevel.correctValue(iLevel,*_lpUpkeepTime);

			return	TRUE;
		}

		case	eSKILL_EE_SET_TRAP					:
		{
			if	(_bIsByAura)
				break;

			if	(iRandomValue>=	iActiveChance)
				break;

			int	iRange			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;
			int	iDisarmLevel	=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel)/100;
			int	iDetectLevel	=	(_lpEffect->m_aiValue[4]+_lpEffect->m_aiValue[5]*iSkillLevel)/100;
			
			iDisarmLevel		=	min(iDisarmLevel,100);
			iDetectLevel		=	min(iDetectLevel,100);

			*_lpUpkeepTime		/=	100;

			cTrap	trap;

			int	iLimitTrapCount	=	_lpAbility->getShotCount();
			int	iExistTrapCount	=	m_lpField->getExistTrapCount(this,_lpAbility->m_wSkill);

			if	(m_lpField->isTrapFull()	||	iExistTrapCount>=iLimitTrapCount)
			{
				cACTOR	*lpActor=	getControlLord();

				if	(!lpActor)
					break;

				lpActor->increaseCP(_lpAbility->getSpentCP());

				if	(lpActor->isPlayer())
					lpActor->syncPlayerBasicStatus(TRUE);

				lpActor->sendRegistServerMessage(eRSM_CAN_NOT_SET_NO_MORE_THAT_SKILL_TRAP,eSM_SYSTEM);
				break;
			}

			trap.build(this,_lpAbility,_iX,_iY,iRange,iDetectLevel,iDisarmLevel,*_lpUpkeepTime);

			int	iAddTrap		=	g_pTrapManager->addTrap(&trap);

			if	(iAddTrap		==	0xffff)
				break;

			m_lpField->addTrap(&trap);

			return	TRUE;
		}

		case	eSKILL_EE_ADD_THORN					:
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			int	iReturnDamageRate	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;

			*_lpUpkeepTime			/=	100;

			if	(_lpEffect->m_aiValue[5])
				m_returnDamageByRootRate.correctValue(iReturnDamageRate,*_lpUpkeepTime);
			else
				m_returnDamageRate.correctValue(iReturnDamageRate,*_lpUpkeepTime);

			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_CRITICAL_CHANCE	:
		{
			if (iRandomValue>=	iActiveChance)
				break;
			
			int	iLimit					=	_lpEffect->m_aiValue[2]/100;
			int	iIncreaseCriticalChance	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;

			if (iLimit)
				iIncreaseCriticalChance		=	min(iIncreaseCriticalChance,iLimit);

			*_lpUpkeepTime			/=	100;
			m_correctCriticalHitChance.correctValue(iIncreaseCriticalChance,*_lpUpkeepTime);

			return	TRUE;
		}

		case	eSKILL_EE_PHYSICAL_DAMAGE_BASED_REMAIN_HP	:
		{
			if (iRandomValue>=	iActiveChance)
				break;

			int	iLimit				=	_lpEffect->m_aiValue[2]/100;
			int	iDamagePercentage	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;

			if (iLimit)
				iDamagePercentage	=	min(iDamagePercentage,iLimit);
			iDamagePercentage		-=	iDamagePercentage*getCurseResistance()/100;

			int	iDamage				=	m_iHP*iDamagePercentage/100;

			CHitInfo	hitInfo;

			hitInfo.reset();
			hitInfo.setHit();
			hitInfo.setPhysicalDamage();

			reduceHP(iDamage,_lpAttacker);

			addSendHitDamage(_lpAttacker,iDamage,0,hitInfo.m_dwResultField,_lpAbility->m_wSkill);

			return	TRUE;
		}

		case	eSKILL_EE_GOLD_HOLDER			:
		{
			int				iRange	=	getAttackRange(_lpAbility);
			CDroppedItem	*lpItem	=	m_lpField->m_droppedItem.getNeareastItem(this,iRange,iSkillLevel,TRUE,FALSE);

			if	(!lpItem)
				return	FALSE;

			if	(iMiniPet()->getDroppedItemByMiniPet(lpItem,_lpAbility))
				return	TRUE;

			break;
		}

		case	eSKILL_EE_ITEM_COLLECTOR		:
		{

			int		 		iRange	=	getAttackRange(_lpAbility);
			CDroppedItem	*lpItem	=	m_lpField->m_droppedItem.getNeareastItem(this,iRange,iSkillLevel,TRUE,TRUE);

			if	(!lpItem)
				return	FALSE;

			if	(iMiniPet()->getDroppedItemByMiniPet(lpItem,_lpAbility))
				return	TRUE;

			break;
		}
		
		case	eSKILL_EE_FINDING_TEASURE		:
		{
			if	(iRandomValue>=	iActiveChance)
				return	TRUE;

			if	(m_lpTarget)	//	전투중
				return	TRUE;

			if	(m_wObjectItemForCatch	!=	0xffff	||	m_wIsReturnToTransferItem)
				return	TRUE;

			int	iRange	=	getAttackRange(_lpAbility);

			CDroppedItem	*lpItem	=	m_lpField->m_droppedItem.getNeareastItem(this,iRange,iSkillLevel);

			if (!lpItem)
				return	TRUE;

			catchItem(lpItem);
			
			if	(isPet() || isSummonBeast())
			{
				cACTOR	*lpTamer	=	getTamer();

				if	(lpTamer)
					lpTamer->sendPlaySkillSound(lpSkill->m_wSerial,eSSD_ACTION);
			}

			return	TRUE;
		}

		case	eSKILL_EE_TAUNT					:
		{
			if (iRandomValue>=	iActiveChance)
				return	TRUE;

			*_lpUpkeepTime			/=	100;
			int	iDamagePercent	=	_lpEffect->m_aiValue[0] + _lpEffect->m_aiValue[1] * _lpAbility->m_wLevel;
			if(_lpEffect->m_aiValue[2])
				iDamagePercent		=	min(iDamagePercent,_lpEffect->m_aiValue[2]);
			
			operateTauntSkill(_lpAttacker,iDamagePercent,*_lpUpkeepTime);

			return	TRUE;
		}

		case	eSKILL_EE_PET_TAUNT				:
		{
			if (iRandomValue>=	iActiveChance)
				return	TRUE;

			*_lpUpkeepTime			/=	100;

			operatePetTauntSkill(_lpAttacker,*_lpUpkeepTime);

			return	TRUE;
		}

		case	eSKILL_EE_MAKE_THE_OTHER_SELF	:
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			*_lpUpkeepTime	/=	100;

			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];

			int	iCount		=	iValue1+iValue2*iSkillLevel;

			if (iCount	%	100)
				iCount	=	iCount/100+1;
			else
				iCount	=	iCount/100;

			makeTheOtherSelf(*_lpUpkeepTime,iCount);

			return	TRUE;
		}

		case	eSKILL_EE_ZOMBIE_BOMB			:
		{
			if	(iRandomValue>=	iActiveChance	||	m_wNotificationOfDeathCounter)
				break;
			
			*_lpUpkeepTime						/=	100;
			
			int	iBombPower						=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;

			m_wNotificationOfDeathCounter		=	*_lpUpkeepTime;
			m_wNotificationOfDeathCounterCaster	=	_lpAttacker->m_wSerialInField;
			m_wNotificationOfDeathCounterRange	=	_lpEffect->m_aiValue[2];
			m_wNotificationOfDeathChance		=	0;
			m_wZombieBombPower					=	iBombPower;
			m_wZombieBombSkill					=	_lpAbility->m_wSkill;

			sendEtcWork(eEW_ZOMBIE_BOMB,m_wNotificationOfDeathCounter);
			
			return	TRUE;
		}

		case	eSKILL_NOTIFICATION_OF_DEATH	:
		{
			if	(iRandomValue>=	iActiveChance	||	m_wNotificationOfDeathCounter)
				break;

			if	(isPureMonster() && getCurseResistance() == 100)
				break;

			*_lpUpkeepTime						/=	100;

			int	iValue1							=	_lpEffect->m_aiValue[0];
			int	iValue2							=	_lpEffect->m_aiValue[1];
			int	iLimitTime						=	(iValue1+iValue2*iSkillLevel)/100;
			int	iDeathChance					=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel)/100;

			*_lpUpkeepTime						=	max(*_lpUpkeepTime,iLimitTime);

			m_wNotificationOfDeathCounter		=	*_lpUpkeepTime;
			m_wNotificationOfDeathCounterCaster	=	_lpAttacker->m_wSerialInField;
			m_wNotificationOfDeathCounterRange	=	_lpAbility->getHitRange();
			m_wNotificationOfDeathChance		=	iDeathChance;

			sendEtcWork(eEW_NOTIFICATION_OF_DEATH_COUNTER,m_wNotificationOfDeathCounter);

			return	TRUE;
		}

		case	eSKILL_EE_STEAL_MONEY			:
		{
			if	(iRandomValue>=	iActiveChance	||	m_iGold	==	0)
				break;

			int	iStealGold	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;

			iStealGold		=	min(m_iGold,iStealGold);

			m_iGold			-=	iStealGold;

			if	(isPlayer())
				updateGoldLog(-iStealGold,eSPENT_GOLD_BY_STEAL_SKILL);

			sendEtcWork(eEW_STEAL_GOLD,iStealGold);

			return	TRUE;
		}

		case	eSKILL_EE_MIN_DAMAGE_FOR_THIS_TIME_ATTACK	:
		{
			if	(iRandomValue>=	iActiveChance)
				break;

			m_bf1IsTakeMinDamageForThisTimeAttack	=	TRUE;

			return	TRUE;
		}

		case	eSKILL_EE_ANIMATE_PARTNER					:
		{
			if	(iRandomValue	>=	iActiveChance)
				break;

			if	(isMonster()	&&	m_bf2AnimatePartnerCount	>=	dMONSTER_ANIMATE_PARTNER_LIMIT_COUNT)
				break;

			int	iAnimatePartnerReviveTime	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;

			animatePartner(*_lpUpkeepTime/100,iAnimatePartnerReviveTime);

			return	TRUE;
		}

		case		eSKILL_EE_MAKE_SKILL_ZONE				:
		{
			cAbility	ability;

			ability.set(_lpEffect->m_aiValue[0],iSkillLevel);

			return	m_lpField->bookingMakeSkillZone(this,&ability);
		}

		case		eSKILL_EE_MAKE_SHAKLE					:
		{
			if(m_wOperatorLevel	>=	3)
				break;
			
			int		iHP	=	_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel;
			int		iDP	=	_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel;

			*_lpUpkeepTime	/=	100;
			int		iIsFreeze	=	_lpEffect->m_aiValue[4];
			int		iIsCritical	=	_lpEffect->m_aiValue[5];
			int		iIsSyncDamage=	_lpEffect->m_aiValue[6];

			return	makeShakle(iHP,iDP,_lpAttacker,_lpAbility,*_lpUpkeepTime,iIsFreeze,iIsCritical,iIsSyncDamage,_lpEffect->m_aiValue[8]);
		}

		case	eSKILL_EE_PERIODICAL_DARK_DAMAGE_BY_SLEEP	:
		{
			if	(m_wSleepTime	==	0)
				break;

			int	iResistance			=	getEarthResistance();
			int	iValue1				=	_lpEffect->m_aiValue[0];
			int	iValue2				=	_lpEffect->m_aiValue[1];
			int	iDamage				=	(iValue1+iValue2*iSkillLevel);

			m_wDarkDamageBySleepCaster=	_lpAttacker->m_wSerialInField;

			int	iCorrectDamage		=	_lpAttacker->getStrengtheningDarkDamage(0,FALSE);

			iDamage					=	iDamage+iDamage*iCorrectDamage/100;
			iDamage					+=	(int)(iDamage*_lpAttacker->getIntelligence()*0.005f);
			iDamage					=	iDamage*(100-iResistance)/100;

			m_wDarkDamageBySleep	=	iDamage/100;

			return	TRUE;
		}

		case	eSKILL_EE_ULTIMATE_BARRIER					:
		{
			*_lpUpkeepTime			/=	100;

			m_wUntimateBarrierTime	=	*_lpUpkeepTime;
			m_wUntimateBarrierRange	=	_lpAbility->getHitRange();

			return	TRUE;
		}

		case	eSKILL_EE_PIGEON_POST						:
		{
			if	(m_wTradeBoxSerial	!=	0xffff)
				return	FALSE;

			int	iCharge						=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;
			int	iEnableItem					=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel)/100;
			int	iLimitItem					=	_lpEffect->m_aiValue[4]/100;
			int	iMinCharge					=	_lpEffect->m_aiValue[5];

			iCharge							=	max(iCharge,iMinCharge);
			iEnableItem						=	min(iEnableItem,iLimitItem);
			m_bf6EnablePigeonPostItemCount	=	iEnableItem;
			m_bf8DeliverPrice				=	iCharge;

			m_bf1IsReadyToBuyPigeonPostItem	=	TRUE;

			sendEtcWork(eEW_OPEN_PIGEON_POST_SHOP,(short)iCharge,iEnableItem);
			
			return	TRUE;
		}
		
		case	eSKILL_EE_BERSERK			:
		{
			iActiveChance	=	iActiveChance*(100-getCurseResistance())/100;

			if	(iRandomValue>=	iActiveChance)
				break;

			*_lpUpkeepTime			/=	100;

			int	iBerserkTime		=	*_lpUpkeepTime*(100-getCurseResistance())/100;

			if	(m_wCurseAmplificationTime)
				iBerserkTime		=	iBerserkTime*dCURSE_AMPLIFICATION_FACTOR/100;

			m_wBerserkTime			=	max(m_wBerserkTime,iBerserkTime);

			break;
		}
		
		case	eSKILL_EE_DANCE				:
		{
			iActiveChance	=	iActiveChance*(100-getStrangeStatusResistance(TRUE))/100;

			if	(iRandomValue>=	iActiveChance)
				break;

			*_lpUpkeepTime	=	*_lpUpkeepTime*(100-getStrangeStatusResistance(TRUE))/100;
			*_lpUpkeepTime	/=	100;
			*_lpUpkeepTime	=	max(*_lpUpkeepTime,1);

			dance(*_lpUpkeepTime);

			return	TRUE;
		}

		case	eSKILL_EE_MAGIC_BOX				:
		{
			iActiveChance	=	iActiveChance*(100-getCurseResistance())/100;

			if	(iRandomValue>=	iActiveChance)
				break;

			int	iKillChance	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;
			int	iLimitCount	=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*iSkillLevel)/100;
			*_lpUpkeepTime	/=	100;

			return	shutInMagicBox(_lpAttacker,*_lpUpkeepTime,iKillChance,iLimitCount);
		}

		case	eSKILL_EE_COPY					:
		{
			iActiveChance	=	iActiveChance*(100-getCurseResistance())/100;

			if	(iRandomValue>=	iActiveChance)
				break;

			if	(_lpAttacker->m_bf1IsExistCopySlave)
				break;

			int	iLimitCount	=	(_lpEffect->m_aiValue[4]+_lpEffect->m_aiValue[5]*iSkillLevel)/100;
			int	iHP			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;
			int	iUpkeepTime	=	*_lpUpkeepTime/100;

			iUpkeepTime		=	iUpkeepTime*(100-getCurseResistance())/100;

			if	(iUpkeepTime	==	0	||	iHP	==	0)
				break;

			return	copyActor(_lpAttacker,iHP,iUpkeepTime,iLimitCount);
		}
		
		case	eSKILL_EE_BOUNCE				:
		{
			iActiveChance	=	iActiveChance*(100-getCurseResistance())/100;

			if	(iRandomValue>=	iActiveChance)
				break;

			int	iDistance	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;

			bounce(_lpAttacker,_lpAbility,iDistance,_iHitDamage);
			return TRUE;
		}
		case eSKILL_EE_INTERRUPTING_WEAPON			:
			{	// 무기해제(무기사용불가)
				float	fCorrectValue	=	(100-min(70,getCurseResistance()))/100.0;
				iActiveChance	=	iActiveChance*fCorrectValue;

				if	(iRandomValue>=	iActiveChance)
					break;

				*_lpUpkeepTime		=	*_lpUpkeepTime*fCorrectValue;

				setInterruptingWeapon(*_lpUpkeepTime/100);
				return TRUE;
			}

		case eSKILL_EE_DECREASE_FINAL_ATTACK_POWER_PERCENTAGE		:
			{	// 최종공격력감소퍼센티지
				if	(iRandomValue	>=	iActiveChance)
					break;
				int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
				int	iUpkeepTime		=	(*_lpUpkeepTime)/100;
				
				if(m_wCorrectFinalAttackPowerPercentageTime > iUpkeepTime)
					break;
				m_wCorrectFinalAttackPowerPercentageTime = iUpkeepTime;
				m_sCorrectFinalAttackPowerPercentage	= iValue;
				
				return TRUE;
			}
		case eSKILL_EE_NOT_USE_ACTIVITY_REACTION_SKILL :
			{	// 리액션사용금지.
				float	fCorrectValue	=	(100-min(70,getCurseResistance()))/100.0;
				iActiveChance	=	iActiveChance*fCorrectValue;

				if	(iRandomValue>=	iActiveChance)
					break;

				if(!isPlayer())
					break;

				*_lpUpkeepTime		=	*_lpUpkeepTime*fCorrectValue;

				m_wNotUseActivityReactionSkillTime		=	*_lpUpkeepTime/100;				

				reactionAllOff();
				
				return TRUE;
			}
		case eSKILL_EE_CONSENSUS:
			{	// 교감 상태.
				if(iRandomValue>=iActiveChance)
					break;
				
				m_wConsensusTime	= *_lpUpkeepTime/100;
				
				return TRUE;
			}
		case eSKILL_EE_INCREASE_ALL_STATUS:
			{	// 모든능력치증가 
				if(iRandomValue>=iActiveChance)
					break;
				if(m_wIncreaseAllStatusTime > *_lpUpkeepTime/100)
					break;
				m_wIncreaseAllStatusTime	= *_lpUpkeepTime/100;
				
				m_wIncreaseAllStatus = (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
				
				m_strength.correctValue(m_wIncreaseAllStatus,m_wIncreaseAllStatusTime,bIsCanNotApplySameEffect);
				m_agility.correctValue(m_wIncreaseAllStatus,m_wIncreaseAllStatusTime,bIsCanNotApplySameEffect);
				m_constitution.correctValue(m_wIncreaseAllStatus,m_wIncreaseAllStatusTime,bIsCanNotApplySameEffect);
				m_intelligence.correctValue(m_wIncreaseAllStatus,m_wIncreaseAllStatusTime,bIsCanNotApplySameEffect);
				m_wisdom.correctValue(m_wIncreaseAllStatus,m_wIncreaseAllStatusTime,bIsCanNotApplySameEffect);
				m_charisma.correctValue(m_wIncreaseAllStatus,m_wIncreaseAllStatusTime,bIsCanNotApplySameEffect);
				m_luck.correctValue(m_wIncreaseAllStatus,m_wIncreaseAllStatusTime,bIsCanNotApplySameEffect);
				
				buildPower();
				*_lpUpkeepTime		=	m_wIncreaseAllStatusTime*dSYNC_FPS;
				return TRUE;
			}

			case eSKILL_EE_MADNESS:
				{	// 광란 상태.
					if(iRandomValue>=iActiveChance)
						break;

					m_wMadnessTime	= *_lpUpkeepTime/100;
					int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;
					m_wCorrectFinalDefensivePowerPercentageTime		= m_wMadnessTime;
					m_sCorrectFinalDefensivePowerPercentage			= -iValue;
					m_dodgeChance.correctValue(-iValue,m_wMadnessTime, bIsCanNotApplySameEffect);
					return TRUE;
				}

			case eSKILL_EE_INCLINE_CRITICAL_DAMAGE_PERCENTAGE:
				{	// 치명타대미지증가퍼센티지
					if(iRandomValue>=iActiveChance)
						break;

					int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel);
	
					m_strengtheningCriticalDamage.correctPercentageValue(iValue,*_lpUpkeepTime/100);
		
					return TRUE;
				}
			case eSKILL_EE_DRAIN_CP:
				{	// CP 흡수.
					if(iRandomValue>=iActiveChance)
						break;
					
					int	iDrainCP			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel);
					float	fIntForDamage	=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage();
					iDrainCP			+=	(int)(iDrainCP*_lpAttacker->getIntelligence()/4*fIntForDamage);
					if(isPlayer())
					{
						if(m_iCP <= 0) 
							break;
						
						if(iDrainCP >	m_iCP)
							iDrainCP = m_iCP;
					}
					correctValueEffectDuelServer(&iDrainCP, lpSkill);
					reduceCP(iDrainCP,_lpAbility->m_wSkill);
					_lpAttacker->increaseCP(iDrainCP);
					_lpAttacker->sendPlayerBasicInformation();
					if	(isPlayer())
						syncPlayerBasicStatus(TRUE);
					return TRUE;
				}
			case eSKILL_EE_SOUL_CONVERSION:
				{ // 소울 컨버전.
					float	fCorrectValue	=	(100-min(70,getCurseResistance()))/100.0;
					iActiveChance	=	iActiveChance*fCorrectValue;
					
					if	(iRandomValue>=	iActiveChance)
						break;
					
					*_lpUpkeepTime		=	*_lpUpkeepTime*fCorrectValue;
					float fValue = (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/10000.0f;
					int iTime	= *_lpUpkeepTime/100;
					setInverseBuff(fValue, iTime);
					dispell();
					buildPower();
					
					return TRUE;
				}

			case eSKILL_EE_SOUL_BURN:
				{ // 소울 번.
					float	fCorrectValue	=	(100-min(70,getCurseResistance()))/100.0;
					iActiveChance	=	iActiveChance*fCorrectValue;
					
					if	(iRandomValue>=	iActiveChance)
						break;
					
					*_lpUpkeepTime		=	*_lpUpkeepTime*fCorrectValue;
					
					int		iTargetDamage	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
					
					float	fIntForDamage	=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage();
					iTargetDamage			+=	(int)(iTargetDamage*_lpAttacker->getIntelligence()*fIntForDamage);
					
					//	불강화효과 
					iTargetDamage = applySkillExtraEffectMagicDamage(_lpAttacker , _lpAbility , iTargetDamage, eDAMAGE_FIRE); // 09.09.29
					

#ifdef	_FOR_JAPAN
					if	(m_lpField->m_bIsGuildBattleField || m_lpField->m_bIsSiegeWarfareField)
						iTargetDamage	=	GetP2PCorrectDamage(iTargetDamage);
#else
					if(!g_bIsDuelServer && bIsPVP)
					{
						iTargetDamage = iTargetDamage* 0.5;				// PVP 일경우... 50%만 적용..
					}
#endif
					correctValueEffectDuelServer(&iTargetDamage , lpSkill);
					m_wSoulBurnDamage = iTargetDamage / 100; //
					m_wSoulBurnSkill	=	_lpAbility->m_wSkill;		
					m_wSoulBurnTime = 	*_lpUpkeepTime/100;
					m_wIncreaseSoulBurnDamage = (m_wSoulBurnDamage*_lpEffect->m_aiValue[2]/100)/100;
					m_wSoulBurnDamageSecond = _lpEffect->m_aiValue[3];
					m_wSoulBurnCaster = _lpAttacker->m_wSerialInField;
					m_dwSoulBurnCasterNameHashCode = _lpAttacker->m_dwNameHashCode;
					return TRUE;
				}
			case eSKILL_EE_RAGE_SOUL:
				{ // 타오르는영혼
					if(iRandomValue>=iActiveChance)
						break;
					
					m_wRageSoulTime		= 	*_lpUpkeepTime/100;

					int		iTargetDamage	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
					
					float	fIntForDamage	=	_lpAttacker->getCorrectIntelligenceFactorForMagicDamage();
					iTargetDamage			+=	(int)(iTargetDamage*_lpAttacker->getIntelligence()*fIntForDamage);
					
					//	불강화효과 
					iTargetDamage = applySkillExtraEffectMagicDamage(_lpAttacker , _lpAbility , iTargetDamage, eDAMAGE_FIRE); // 09.09.29


					m_wRageSoulDamage	=iTargetDamage/100 ;
					
					m_wRageSoulDamageGap		= _lpEffect->m_aiValue[2]/100;
					m_wRageSoulChance	=	_lpEffect->m_aiValue[3] + _lpEffect->m_aiValue[4]*_lpAbility->m_wLevel;
								
					return TRUE;
				}
			case eSKILL_EE_BINDING_WORDS:
				{ // 언어구속.

					if(iRandomValue>=iActiveChance)
						break;

					m_wBindingWordsTime		=	*_lpUpkeepTime/100;	

					m_wBindingWordsOverLimitLevel	=	_lpEffect->m_aiValue[0]; //
					return TRUE;
				}
			case eSKILL_EE_FIGHT_ENERGY:
			{	// 투기

				if(iRandomValue>=iActiveChance)
					break;

				WORD	wFightEnergyTime	= *_lpUpkeepTime/100;
				WORD	wFightEnergyState	= _lpEffect->m_aiValue[9];			
				int		iFightEnergyValue	= (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
				if(_lpEffect->m_aiValue[2] != 0)
					iFightEnergyValue = min(iFightEnergyValue, _lpEffect->m_aiValue[2]);
				m_fightEnergy.set(wFightEnergyTime,wFightEnergyState, iFightEnergyValue);
				return TRUE;
			}
			case eSKILL_EE_RUSH:
			{	// 돌진 

				if(iRandomValue>=iActiveChance)
					break;

				m_wRushTime	= *_lpUpkeepTime/100;
				m_isRunning = TRUE;
				return TRUE;
			}
			case eSKILL_EE_FIGHTING_SPIRIT:
			{	// 투지 

				if(iRandomValue>=iActiveChance)
					break;

				m_wFightingSpiritTime	= *_lpUpkeepTime/100;
		
				m_iFightingSpiritValue	= (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;

				if(_lpEffect->m_aiValue[2] != 0)
					m_iFightingSpiritValue = min(m_iFightingSpiritValue, _lpEffect->m_aiValue[2]);

				return TRUE;
			}
			case eSKILL_EE_IGNORE_NEED_STATE:
			{	// 필요상태무시 

				if(iRandomValue>=iActiveChance)
					break;

				m_wIgnoreNeedStateTime	= *_lpUpkeepTime/100;
				return TRUE;
			}
			case eSKILL_EE_IGNORE_BLOCKING:			// 블럭킹무시확률
			{
				if(iRandomValue>=iActiveChance)
					break;
				
				m_wIgnoreBlockingTime	= *_lpUpkeepTime/100;
				m_wIgnoreBlockingValue = (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
				if(_lpEffect->m_aiValue[2] != 0)
					m_wIgnoreBlockingValue = min(m_wIgnoreBlockingValue , _lpEffect->m_aiValue[2]);
				return TRUE;
			}
			case eSKILL_EE_RETURN_MISSILE_DAMAGE:			// 미사일대미지리턴
			{
				if(iRandomValue>=iActiveChance)
					break;
				
				m_wReturnMissileDamageTime	= *_lpUpkeepTime/100;
				m_wReturnMissileDamageValue = (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
				if(_lpEffect->m_aiValue[2] != 0)
					m_wReturnMissileDamageValue = min(m_wReturnMissileDamageValue , _lpEffect->m_aiValue[2]);
				return TRUE;
			}

			case eSKILL_EE_INCREASE_HARD_BLOW:			// 강타확률증가
			{
				if(iRandomValue>=iActiveChance)
					break;
				
				m_wIncreaseHardBlowTime	= *_lpUpkeepTime/100;
				m_wHardBlowValue = (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
				if(_lpEffect->m_aiValue[2] != 0)
					m_wHardBlowValue = min(m_wHardBlowValue , _lpEffect->m_aiValue[2]);
				return TRUE;
			}
			case eSKILL_EE_JUMP	:			// 점프
				{
					if(iRandomValue>=iActiveChance)
						break;
					if(isDeath())
						break;
					if(isMonster())
					{
						cJOB*	lpJob = getJob();
						
						if(lpJob == NULL)
							return FALSE;
						
						if (lpJob->m_wMonsterLevel>=cJOB::eML_BOSS1)
							return FALSE;
					}

					m_lpField->addSendEtcWork(m_wSerialInField,eEW_JUMP, m_wSerialInField ,_lpEffect->m_aiValue[0], _lpEffect->m_aiValue[1]);
					return TRUE;
				}
			case	eSKILL_EE_ATTACK_DOPPELGANGER:
				{
					if(iRandomValue>=iActiveChance)
						break;
					if(m_wAttackDoppelgangerCount)
						break;

					int		iAttackCount	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
					if(_lpEffect->m_aiValue[2])
						iAttackCount	=	min(_lpEffect->m_aiValue[2], iAttackCount);
					iAttackCount	/=	100;
					int		iDamagePercent	=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*_lpAbility->m_wLevel)/100;
					int		iActionPerion = _lpEffect->m_aiValue[5]+_lpEffect->m_aiValue[6]*_lpAbility->m_wLevel;

					if(_lpEffect->m_aiValue[7])
						iActionPerion	=	max(_lpEffect->m_aiValue[7],iActionPerion);
					m_wActionPerionAttackDoppelganger			= iActionPerion;
					m_dwActionTimeAttackDoppelganger			=	g_dwCurrentTime+m_wActionPerionAttackDoppelganger;
					m_iAttackDoppelgangerDamage					= 0; 
					m_wAttackDoppelgangerCount					= iAttackCount;
					m_wAttackDoppelgangerCaster					= _lpAttacker->m_wSerialInField;
					m_dwAttackDoppelgangerCasterNameHashCode	= _lpAttacker->m_dwNameHashCode;
					CHitInfo	hitInfo;	
					getDamage(_lpAttacker, _lpAbility, m_pos.x,m_pos.y, &hitInfo, TRUE, TRUE, TRUE, 1, 0, TRUE);
					if(hitInfo.m_dwPhysicalDamage )
						m_iAttackDoppelgangerDamage =  hitInfo.m_dwPhysicalDamage * iDamagePercent/100;

					m_lpField->addSendEtcWork(0xffff,eEW_ATTACK_DOPPELLGANGER, m_wSerialInField , m_wAttackDoppelgangerCount,m_iAttackDoppelgangerDamage,m_wActionPerionAttackDoppelganger);
					return TRUE;
				}
			case	eSKILL_EE_ADD_BIT:
				{

					if(isLightAdditionalEffect())
					{
						
						int iChance	=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*_lpAbility->m_wLevel);
						if(_lpEffect->m_aiValue[5])
							iChance	=	min(_lpEffect->m_aiValue[5], iChance) / 100;
						iActiveChance += iChance;
					}

					iActiveChance += _lpAttacker->getValuePassiveSkill(eSKILL_PEE_ADD_BIT_CHANCE)/100;
					
					if(iRandomValue>=iActiveChance)
						break;

					int iCount	=		(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

					if(_lpEffect->m_aiValue[2])
						iCount	=	min(_lpEffect->m_aiValue[2], iCount);

					iCount /= 100;
						
					addBit(_lpAttacker->m_wSerialInField,_lpAttacker->m_dwNameHashCode, iCount,*_lpUpkeepTime/100 );

					return TRUE;
				}
			case	eSKILL_EE_REMOVE_BIT:
				{
	
					if(iRandomValue>=iActiveChance)
						break;

					int iCount	=		(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

					if(_lpEffect->m_aiValue[2])
						iCount	=	min(_lpEffect->m_aiValue[2], iCount);

					iCount /= 100;
					removeBit(_lpAttacker->m_wSerialInField,_lpAttacker->m_dwNameHashCode, iCount);
					return TRUE;
				}
			case	eSKILL_EE_BLEEDING:
				{
					if(iRandomValue>=iActiveChance)
						break;
					if(m_wBleedingTime)
						break;
					
					int		iBleedingDamagePercent	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
					if(_lpEffect->m_aiValue[2])
						iBleedingDamagePercent	=	min(_lpEffect->m_aiValue[2], iBleedingDamagePercent);

					m_wBleedingPeriod					= _lpEffect->m_aiValue[3];
					m_wBleedingTime						= *_lpUpkeepTime/100;
					m_wBleedingCaster					= _lpAttacker->m_wSerialInField;

					CHitInfo	hitInfo;	
					getDamage(_lpAttacker, _lpAbility, m_pos.x,m_pos.y, &hitInfo, TRUE, TRUE, TRUE, 1, 0, TRUE);

					if(hitInfo.m_dwPhysicalDamage )
						m_iBleedingDamage =  hitInfo.m_dwPhysicalDamage * iBleedingDamagePercent/10000;

					return TRUE;
				}

			case	eSKILL_EE_ELECTRIC_SHOCK:
				{
					if(iRandomValue>=iActiveChance)
						break;
					if(m_wElectricShockTime)
						break;
					
					m_wElectricShockDamagePercent	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

					if(_lpEffect->m_aiValue[2])
						m_wElectricShockDamagePercent	=	min(_lpEffect->m_aiValue[2], m_wElectricShockDamagePercent);

					m_wElectricShockTime						= *_lpUpkeepTime/100;

					return TRUE;
				}
			case	eSKILL_EE_INSTANCE_HEAL_EFFECT:
				{
					if(iRandomValue>=iActiveChance)
						break;
					if(m_wInstanceHealEffectTime)
						break;
					
					m_wInstanceHealEffectPercent	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

					if(_lpEffect->m_aiValue[2])
						m_wInstanceHealEffectPercent	=	min(_lpEffect->m_aiValue[2], m_wInstanceHealEffectPercent);

					m_wInstanceHealEffectTime						= *_lpUpkeepTime/100;

					return TRUE;
				}
			case	eSKILL_EE_MAKE_TYPE_UNDEAD:
				{
					if(iRandomValue>=iActiveChance)
						break;
					if(m_wMakeTypeUndeadTime)
						break;
					m_wMakeTypeUndeadTime						= *_lpUpkeepTime/100;
					return TRUE;
				}
			case	eSKILL_EE_BLOOD_DONATION:
				{ // 헌혈 상태!
					if(iRandomValue>=iActiveChance)
						break;

					if(m_wBloodDonationTime)
						break;

					m_wBloodDonationTime						= *_lpUpkeepTime/100;

					m_wBloodDonationPercent	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

					if(_lpEffect->m_aiValue[2])
						m_wBloodDonationPercent	=	min(_lpEffect->m_aiValue[2], m_wBloodDonationPercent);
					m_wBloodDonationPercent	/=	100;

					return TRUE;
				}
	}

	return	FALSE;
}

void
cACTOR::addBit(WORD _wSerialInField, DWORD _dwNameHashCode, int _iCount, int iTime)
{

	for(int i=0;i<_iCount;++i)
	{
		if(m_bitInfoManager.addBit(_wSerialInField,_dwNameHashCode,iTime))
		{
			m_lpField->addSendEtcWork(0xffff,eEW_ADD_BIT, m_wSerialInField,_wSerialInField , (int)_dwNameHashCode);
		}
	}
}

void
cACTOR::removeBit(WORD _wSerialInField, DWORD _dwNameHashCode, int _iCount)
{
	int iRemoveCount = m_bitInfoManager.removeBit(_wSerialInField,_dwNameHashCode, _iCount);

	m_lpField->addSendEtcWork(0xffff,eEW_REMOVE_BIT,m_wSerialInField, _wSerialInField , _dwNameHashCode, iRemoveCount);
}


void
cACTOR::setInterruptingWeapon(WORD _wTime)		// 무기해제(무기사용불가)
{
	if(!isPlayer())
		return;
	if(m_wInterruptingWeaponTime)
		return;
	
	m_wInterruptingWeaponTime = _wTime;	

	buildPower();

}
BOOL
cACTOR::operateDestroyingUndead(cACTOR *_lpAttacker,cAbility *_lpAbility)
{
	if	(!isUndead())
		return	FALSE;

	int		iHitCount	=	0;
	BOOL	bIsPVP		=	FALSE;

	if	(_lpAttacker->m_wSerialInField != m_wSerialInField)
		bIsPVP			=	m_lpField->isPVP(_lpAttacker,this);

	if	(getLevel()	<=	_lpAttacker->getLevel()+_lpAbility->m_wLevel -10)	//	도망
	{
		int	iResistance		=	max(getLightResistance(),getStrangeStatusResistance(TRUE));

		if	(bIsPVP)
			iResistance		=	max(getLightResistance()+getStrangeStatusResistance(TRUE),0);

		int	iActiveChance	=	85*(100-iResistance	)/100;

		if	(random(100) < iActiveChance)
		{
			if (isMonster())
			{
				flee(_lpAttacker,10);
				iHitCount++;
			}
		}
	}

	if	(getLevel()	<=	_lpAttacker->getLevel()+_lpAbility->m_wLevel -25)	//	마비
	{
		int	iResistance		=	max(getLightResistance(),getStunResistance());

		if	(bIsPVP)
			iResistance		=	max(getLightResistance()+getStunResistance(),0);

		int	iActiveChance	=	60*(100-iResistance)/100;

		if (random(100) < iActiveChance)
		{
			if (bIsPVP)
			{
				m_wStunTime			=	max(m_wStunTime,3*dSYNC_FPS);

				if (isNEPlayer(_lpAttacker))	//	천적이다.
					m_wStunTime			=	m_wStunTime*dPVP_CORRECT_STUN_TIME_NE/100;
				else
					m_wStunTime			=	m_wStunTime*dPVP_CORRECT_STUN_TIME/100;
			}
			else
				m_wStunTime			=	max(m_wStunTime,3*dSYNC_FPS);

			iHitCount++;
		}
	}

	if (getLevel()	<=	_lpAttacker->getLevel()+_lpAbility->m_wLevel -45)	//	대미지
	{
		int	iActiveChance	=	30*(100-getLightResistance())/100;

		if (random(100) < iActiveChance)
		{
			m_hitChance.correctValue(-dDECLINE_HIT_CHANCE_VALUE,5);
			m_dodgeChance.correctValue(-dDECLINE_DODGE_CHANCE_VALUE,5);
			m_wBlindTime		=	max(m_wBlindTime,5);

			iHitCount++;
		}
	}

	if (iHitCount)
		return	TRUE;

	return	FALSE;
}

BOOL
cACTOR::operateDestroyingUnholy(cACTOR *_lpAttacker,cAbility *_lpAbility)
{
	if (!isUndead() && !isDemon())
		return	FALSE;

	int		iHitCount	=	0;
	BOOL	bIsPVP		=	FALSE;

	bIsPVP				=	m_lpField->isPVP(_lpAttacker,this);

	if	(getLevel()		<=	_lpAttacker->getLevel()+_lpAbility->m_wLevel -10)	//	도망
	{
		int	iResistance	=	max(getLightResistance(),getStrangeStatusResistance(TRUE));

		if	(bIsPVP)
			iResistance	=	max(getLightResistance()+getStrangeStatusResistance(TRUE),0);

		int	iActiveChance	=	90*(100-iResistance)/100;

		if	(random(100) < iActiveChance)
		{
			if	(isMonster())
			{
				flee(_lpAttacker,10);
				iHitCount++;
			}
		}
	}

	if (getLevel()	<=	_lpAttacker->getLevel()+_lpAbility->m_wLevel -20)	//	마비
	{
		int	iResistance	=	max(getLightResistance(),getStunResistance());

		if	(bIsPVP)
			iResistance	=	max(getLightResistance()+getStunResistance(),0);

		int	iActiveChance	=	70*(100-iResistance)/100;

		if (random(100) < iActiveChance)
		{
			if (bIsPVP)
			{
				m_wStunTime			=	max(m_wStunTime,3*dSYNC_FPS);

				if (isNEPlayer(_lpAttacker))	//	천적이다.
					m_wStunTime			=	m_wStunTime*dPVP_CORRECT_STUN_TIME_NE/100;
				else
					m_wStunTime			=	m_wStunTime*dPVP_CORRECT_STUN_TIME/100;
			}
			else
				m_wStunTime			=	max(m_wStunTime,3*dSYNC_FPS);

			iHitCount++;
		}
	}

	if (getLevel()	<=	_lpAttacker->getLevel()+_lpAbility->m_wLevel -40)	//	대미지
	{
		int	iActiveChance	=	35*(100-getLightResistance())/100;

		if (random(100) < iActiveChance)
		{
			m_hitChance.correctValue(-dDECLINE_HIT_CHANCE_VALUE,5);
			m_dodgeChance.correctValue(-dDECLINE_DODGE_CHANCE_VALUE,5);
			m_wBlindTime		=	max(m_wBlindTime,5);

			iHitCount++;
		}
	}

	if	(iHitCount)
		return	TRUE;
	
	return	FALSE;
}

void
cACTOR::operateAttackMagicShield(cAbility *_lpAbility)
{
//	return;
	
	mDS.m_iTempStep		=	0;
	mDS_P.m_iStep		=	27071;

	int		iIndex		=	0;
	BOOL	bIsGetedCP	=	FALSE;

	while(1)
	{
		mDS_P.m_iStep		=	27072;
		CSkillExtraEffect	*lpEffect	=	_lpAbility->getAttackMagicShieldInfo(iIndex);
		mDS_P.m_iStep		=	27073;
		CSkill				*lpSkill	=	_lpAbility->getSkill();

		if (!lpEffect)
			break;

		iIndex++;

		int	iPeriod	=	lpEffect->m_aiValue[0]+lpEffect->m_aiValue[1]*_lpAbility->m_wLevel;

//		if (lpEffect->m_aiValue[2]	==	eDECIMAL_METHOD_ROUND_OFF)	if (iPeriod%100 >= 50) iPeriod += 100;
//		if (lpEffect->m_aiValue[2]	==	eDECIMAL_METHOD_UP)			if (iPeriod%100 >  0 ) iPeriod += 100;
//		iPeriod		=	iPeriod/100;
		iPeriod		=	iPeriod*dSYNC_FPS/100;

		mDS_P.m_iStep		=	27074;

		if (iPeriod	==	0)
		{
			mDS_P.m_iStep		=	27075;

			CLOG("error","in cACTOR::operateAttackMagicShield - iPeriod == 0 %s %d,%d",lpSkill->m_strName,_lpAbility->m_wSkill,_lpAbility->m_wLevel);

			if (g_bIsTestServer	==	FALSE)
				continue;
		}

		mDS_P.m_iStep		=	27076;

		if (m_lpField->m_iFrameCounter	% iPeriod != 0)
			continue;

		mDS_P.m_iStep		=	27077;

		int	iRange	=	_lpAbility->getHitRange(NULL);

		mDS_P.m_iStep		=	27078;

		mDS.m_activeSkill.m_iCurrentSkill		=	_lpAbility->m_wSkill;
		mDS.m_activeSkill.m_iCurrentSkillLevel	=	_lpAbility->m_wLevel;
		
		m_lpField->strikeWideAreaDamageSkill(this,_lpAbility,&m_pos,iRange,NULL,0,&bIsGetedCP);

		mDS_P.m_iStep		=	27079;
//		수치1					500;	//	마법실드액션주기
//		수치2					-8;		//	마법실드액션주기per레벨
//		수치3					1;		//	마법실드액션주기반올림계산
	}
}

BOOL
cACTOR::thrust(cACTOR *_lpTarget,BOOL _bIsSendPosInfo)
{
	if	(m_wBiteTarget	!=	0xffff	||	m_iGuildGuardianType)
		return	FALSE;

	if	(random(100)	<	m_bKnockBackResistance)
		return	FALSE;

	if	(m_bf1IsCancelKnockBack	||	m_bf1IsImmuneKnockBack)
	{
		m_bf1IsCancelKnockBack	=	FALSE;
		return	FALSE;
	}

	int			iAngle		=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,_lpTarget->m_pos.x,_lpTarget->m_pos.y);
	int			iRange		=	GetOvalRange(m_pos.x,m_pos.y,_lpTarget->m_pos.x,_lpTarget->m_pos.y);

	iRange	=	(int)sqrt(iRange)-_lpTarget->getBodySize();

	if	(iRange	<	_lpTarget->getBodySize())
		iRange	=	_lpTarget->getBodySize()*2;

	return	thrustByAngle(iAngle,iRange,_bIsSendPosInfo);
}

//	밀려나기.. -o-;;
BOOL
cACTOR::thrust(int _iAxisX,int _iAxisY,int _iDistance,BOOL _bIsSendPosInfo,int _iCorrectAngle,BOOL _bIsIgnoreResistance)
{
	if	(_bIsIgnoreResistance	==	FALSE	&&	random(100)	<	m_bKnockBackResistance)
		return	FALSE;

	if	(m_bf1IsCancelKnockBack	||	m_bf1IsImmuneKnockBack)
	{
		m_bf1IsCancelKnockBack	=	FALSE;
		return	FALSE;
	}

	int			iAngle		=	g_protractor.getAngleToTarget(_iAxisX,_iAxisY,m_pos.x,m_pos.y);

	iAngle	=	(iAngle+_iCorrectAngle)%360;

	return	thrustByAngle(iAngle,_iDistance,_bIsSendPosInfo);
}

//	밀려나기.. -o-;;
BOOL
cACTOR::thrustByAngle(int _iAngle,int _iDistance,BOOL _bIsSendPosInfo, BOOL _bIsChangeTargetDirect)
{
	if	(m_wBiteTarget	!=	0xffff	||	m_iGuildGuardianType)
		return	FALSE;

	if	(isMonster() && m_wNpcMoveSpeed == 0)
		return	FALSE;

	if	(_iDistance	<	0)
	{
		_iDistance	=	-_iDistance;
		_iAngle		=	(_iAngle+180)%360;
	}
	
	BOOL	bIsBlocked	=	FALSE;

	cPOINT	pos;

	pos.Set(m_pos.x,m_pos.y);

	while(_iDistance	>	10)
	{
		g_protractor.getAnglePos(&pos,_iAngle,_iDistance);

		if	(!m_lpField)
			return	FALSE;

		if	(m_lpField->isMissileBlockedLine(m_pos.x,m_pos.y,pos.x,pos.y)	==	FALSE)
			break;

		_iDistance	-=	10;
		bIsBlocked	=	TRUE;
		pos.Set(m_pos.x,m_pos.y);
	}

	setActorPos(pos.x,pos.y,TRUE);

	if	(_bIsSendPosInfo)
	{
		SG_THRUST	packet;

		packet.base.set(sizeof(packet),dSG_THRUST);

		packet.bf15Serial				=	m_wSerialInField;		//	누구-_-?
		packet.bf1IsBlockedByObject		=	bIsBlocked;
		packet.wPosX					=	m_pos.x;
		packet.wPosY					=	m_pos.y;//	현재 위치
		packet.bf1IsChangeTargetDirect	=	_bIsChangeTargetDirect;

		m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
	}

	return	TRUE;
}

//
//	회피 액숀!!
BOOL
cACTOR::dodgeAction(cACTOR *_lpAttacker,int _iSpentCP)
{
	if	(!_lpAttacker						)
		return FALSE;
	if	(m_wDodgeAbility	>=	dABILITY_COUNT	)
		return FALSE;

	cAbility	*lpAbility	=	&m_aAbility[m_wDodgeAbility];
	CSkill		*lpSkill	=	lpAbility->getSkill();

	m_wDodgeAbility	=	0xffff;

	if	(!lpSkill)
		return	FALSE;

	cPOINT	pos;

	pos.Set(m_pos.x,m_pos.y);

	if	(lpSkill->m_wDodgeDistance)
	{
		int	iAngle	=	g_protractor.getAngleToTarget(_lpAttacker->m_pos.x,_lpAttacker->m_pos.y,m_pos.x,m_pos.y);

		iAngle		+=	90;
		iAngle		%=	360;

		g_protractor.getAnglePos(&pos,iAngle,lpSkill->m_wDodgeDistance);

		if (m_lpField->isBlocked(pos.x/dTILE_XS,pos.y/dTILE_YS))
		{
			iAngle		+=	180;
			iAngle		%=	360;

			pos.Set(m_pos.x,m_pos.y);
			g_protractor.getAnglePos(&pos,iAngle,20);

			if (m_lpField->isBlocked(pos.x/dTILE_XS,pos.y/dTILE_YS))	return FALSE;
		}
	}

//	막힌 지형 아닌지 확인
	operateAdditionalDodgeEffect(lpAbility);

	setActorPos(pos.x,pos.y,TRUE);

	if	(lpSkill->m_wActivateTrigger	==	eRSAT_HIT_CLOSE_RANGE_ATTACK_DODGE_COUNTER)
	{
		int	iAttackRange	=	getAttackRange(lpAbility);

		if	(isTargetInAttackRange(_lpAttacker,iAttackRange))
			fireCounterReactionSkill(_lpAttacker,lpAbility);
	}

	return	TRUE;
}

//
//	추가 회피 효과 처리
BOOL
cACTOR::operateAdditionalDodgeEffect(cAbility	*_lpAbility)
{
	if (!_lpAbility)
		return	FALSE;

	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if (!lpSkill)
		return	FALSE;

	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		CSkillExtraEffectInfo	skillEE;

		if	(!_lpAbility->getExtraEffect(i,&skillEE))
			break;

		if	(skillEE.m_wEffect	!=	eSKILL_EE_CREATE_DUMMY	)
			continue;
		if	(random(100)		>=	skillEE.m_sActiveChance	)
			break;

		int	iAddHPPercentage	=	(_lpAbility->m_wLevel*skillEE.m_aiValue[1])/100;	

		m_lpField->bookingMakeDummy(m_pos.x,m_pos.y,m_wSerialInField,skillEE.m_iUpkeepTime/100,iAddHPPercentage);
	}

	return	TRUE;
}


BOOL
cACTOR::readyWarpNearestVillage(int _iLimitRange,int _iLimitVillageCount,BOOL _bIsCreatePortal,int _iSkill,int _iPortalTime)
{
	if	(m_wCurrentField	==	27	||	m_wCurrentField	==	28)
		return	FALSE;

	SG_READY_TO_WARP_TO_NEAREST_VILLAGE	packet;

	packet.base.set(sizeof(packet),dSG_READY_TO_WARP_TO_NEAREST_VILLAGE);

	packet.wVillageCount	=	0;
	packet.wIsCreatePortal	=	_bIsCreatePortal;
	packet.wSkill			=	_iSkill;

	int	i;

	for (i=0;i<dMAX_FIELD_COUNT;i++)
	{
		if	(!g_fieldInfo.isVillage(i) || g_game.isCloseMap(i))
			continue;

		if	(i	==	476	||	i	==	477)
			continue;

		if	(i	>=	900)
			continue;

		int	iDistance	=	g_fieldInfo.getDistance(m_wCurrentField,i);

		if (iDistance	<=	_iLimitRange)
		{
			packet.awVillage[packet.wVillageCount++]	=	i;

			if (packet.wVillageCount	>=	dMAX_VILLAGE_COUNT)
				break;
		}
	}

	m_wWarpAbleRange	=	_iLimitRange;

	if	(packet.wVillageCount)
	{
		m_bf1IsReadyToWarpToNearestVillage	=	1-_bIsCreatePortal;
		m_bf1IsReadyToCreateTownPortal		=	_bIsCreatePortal;
		m_wPortalTime						=	_iPortalTime;
		packet.base.wSize					=	sizeof(packet)-sizeof(packet.awVillage)+packet.wVillageCount*2;

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

		return	TRUE;
	}

	return	FALSE;
}

BOOL
cACTOR::calling(int _iLimitRange,int _iSkill)
{
	if	(m_wCurrentField	>=	dINSTANCE_FIELD_FLAG)
		return	FALSE;

	if	(m_wCurrentField	==	27	||	m_wCurrentField	==	28)
		return	FALSE;
	
	if	(m_lpField->m_bIsCanNotUseCalling)
		return	FALSE;		

	if	(getPartySerial()	==	0xffff)
		return	FALSE;

	g_game.sendWPCalling(this,_iLimitRange,_iSkill);

	return	TRUE;
}

BOOL
cACTOR::rally(int _iX,int _iY,int _iTime)
{
	m_wRallyTime			=	max(_iTime,m_wRallyTime);
	m_wBattleInclination	=	dBI_PASSIVE_SOLO;

	for (int iTryCount=100;iTryCount;--iTryCount)
	{
		int		iAngle	=	random(360);
		int		iDistance=	random(40)+40;
		cPOINT	posMove;

		posMove.x		=	_iX;
		posMove.y		=	_iY;

		g_protractor.getAnglePos(&posMove,iAngle,iDistance);

		if (moveTo(posMove.x,posMove.y))
		{
			setAiMove();

			return	TRUE;
		}
	}

	return	FALSE;
}

BOOL
cACTOR::copyActor(cACTOR *_lpAttacker,int _iRemainHPPercentage,int _iUpkeepTime,int _iLimitCount)
{
	if	(m_lpField->getCopyActorCount(_lpAttacker)	>=	_iLimitCount)
	{
		_lpAttacker->sendEtcWork(eEW_LIMIT_COPY_ACTOR_COUNT);

		return	FALSE;
	}

	LONGLONG	llHP	=	getMaxHP();

	llHP	=	llHP*_iRemainHPPercentage/100;

	int		iRemainHP	=	(int)llHP;
	int		iBodySize	=	getBodySize()*2+random(40);
	int		iAngle		=	g_protractor.getAngleToTarget(_lpAttacker->m_pos.x,_lpAttacker->m_pos.y,m_pos.x,m_pos.y);

	cPOINT	pos;

	pos.x	=	_lpAttacker->m_pos.x;
	pos.y	=	_lpAttacker->m_pos.y;

	g_protractor.getAnglePos(&pos,iAngle,iBodySize);

	if	(m_lpField->isBlocked(pos.x/dTILE_XS,pos.y/dTILE_YS))
	{
		pos.x	=	_lpAttacker->m_pos.x;
		pos.y	=	_lpAttacker->m_pos.y;
	}

	m_lpField->bookingCopyActor(pos.x,pos.y,_lpAttacker->m_wSerialInField,m_wSerialInField,_iUpkeepTime,iRemainHP);

	return	TRUE;
}

BOOL
cACTOR::shutInMagicBox(cACTOR *_lpCaster,int _iUpkeepTime,int _iKillChance,int _iLimitCount)
{
	if	(m_lpField->getShutInMagicBoxTargetCount(_lpCaster)	>=	_iLimitCount)
	{
		_lpCaster->sendEtcWork(eEW_LIMIT_SHUT_IN_MAGIC_BOX_ACTOR_COUNT);

		return	FALSE;
	}

	if	(m_wInMagicBoxTime)
		return	FALSE;

	m_wInMagicBoxTime				=	_iUpkeepTime;
	m_wKillChanceByMagicBox			=	_iKillChance;

	m_wMagicBoxCasterSerial			=	_lpCaster->m_wSerialInField;
	m_dwMagicBoxCasterNameHashCode	=	_lpCaster->m_dwNameHashCode;

	stop(TRUE);
	addPacketStop();
	setTarget(NULL);
	setAiReady();

	m_lpField->addSendEtcWork(m_wSerialInField,eEW_SHUT_IN_MAGIC_BOX,m_wSerialInField);

	return	TRUE;
}

BOOL
cACTOR::trickKick(cAbility *_lpAbility,int _iNotRevengeChance)
{
	int		iAttackRange	=	getAttackRange(_lpAbility);
	cACTOR	*lpTarget		=	m_lpField->findNeareastEnemy(m_wTeam,m_iLocalPart,m_pos.x,m_pos.y,iAttackRange);//	가장 근접해 있는 누군가를 찾는다.

	if	(!lpTarget)
		return	FALSE;

	CHitInfo	hitInfo;

	lpTarget->getDamage(this,_lpAbility,m_pos.x,m_pos.y,&hitInfo);

	strike(lpTarget,&hitInfo,_lpAbility);

	lpTarget->addSendHitDamage(this,hitInfo.m_dwPhysicalDamage,hitInfo.m_dwMagicDamage,hitInfo.m_dwResultField,_lpAbility->m_wSkill);

/*	if	(hitInfo.isHit())
	{
		increaseCP(_lpAbility->getGetCP());

		if	(isPlayer())
			syncPlayerBasicStatus(TRUE);
	}
*/
	if	(random(100)	<=	_iNotRevengeChance)
		return	TRUE;

	lpTarget->operateRevenge(this,_lpAbility,TRUE);

	return	TRUE;
}

BOOL
cACTOR::transToFrog(cACTOR *_lpCaster,cAbility *_lpAbility,int _iUpkeepTime)
{
	m_wTransToFrogTime					=	max(_iUpkeepTime,m_wTransToFrogTime);
	
	stop();

	addSendSkillEffect(_lpAbility,eSE_SKILL);

	emptyBookedBattle();

	m_lpField->stopAllTargetedAction(this);

	return	TRUE;
}

BOOL
cACTOR::transToFatGirl(cACTOR *_lpCaster,cAbility *_lpAbility,int _iUpkeepTime,int _iActivePeriod)
{
	addSendSkillEffect(_lpAbility,eSE_SKILL);

	if	(m_wTransToFatGirlTime)
	{
		m_wTransToFatGirlTime			=	0;
		m_wUnsealAction					=	0xffff;
		m_defensePower.dispell();

		return	FALSE;
	}

	emptyBookedBattle();

	m_wFatGirlSkill						=	_lpAbility->m_wSkill;
	m_wFatGirlSkillLevel				=	_lpAbility->m_wLevel;
	m_wActionPerionUnderFatGirlStatus	=	_iActivePeriod*10;
	m_wTransToFatGirlTime				=	_iUpkeepTime;
	m_dwActionTimeUnderFatGirlStatus	=	g_dwCurrentTime+_iActivePeriod;
	m_wUnsealAction						=	_lpAbility->m_wSkill;

	return	TRUE;
}

void
cACTOR::releasePrincessWeaponStatus(BOOL _bIsWantCheckReceiverPos)
{
	cACTOR	*lpReceiver	=	m_lpField->getActor(m_wPrincessWeaponReceiver);

	if	(lpReceiver)
	{
		lpReceiver->removePrincessWeapon(m_wSerialInField,m_dwNameHashCode);

		if	(_bIsWantCheckReceiverPos)
		{
			cPOINT	pos;

			pos.x	=	lpReceiver->m_pos.x;
			pos.y	=	lpReceiver->m_pos.y;

			g_protractor.getAnglePos(&pos,0,60);

			if	(m_lpField->isBlocked(pos.x/dTILE_XS,pos.y/dTILE_YS))
			{
				pos.x	=	lpReceiver->m_pos.x;
				pos.y	=	lpReceiver->m_pos.y;
			}

			setActorPos(pos.x,pos.y);
		}
	}

	m_wPrincessWeaponReceiver	=	0xffff;
	m_wTransToWeaponTime		=	0;

	addPacketStop();
	
	m_lpField->addSendEtcWork(m_wSerialInField,eEW_TRANS_TO_WEAPON,m_wSerialInField,0xffff,m_pos.x,m_pos.y);
}

BOOL
cACTOR::transToWeapon(cACTOR *_lpCaster,cAbility *_lpAbility,int _iUpkeepTime,int _iMagicDamageFactor,int _iDamagePercentageFactor)
{
	if	(_lpCaster->m_wTransToWeaponTime)
	{
		_lpCaster->releasePrincessWeaponStatus(TRUE);

		return	FALSE;
	}

	cItem	*lpSlot	=	getValidInventorySlot();

	if	(!lpSlot)
	{
		_lpCaster->sendEtcWork(eEW_TARGET_INVENTORY_FULL);
		return	FALSE;
	}

	if	(m_wPrincessWeaponPrincess	!=	0xffff)
	{
		_lpCaster->sendEtcWork(eEW_TARGET_ALREADY_OWNED_PRINCSS_WEAPON);
		return	FALSE;
	}
	
	if	(m_wTransToWeaponTime)
	{
		_lpCaster->sendEtcWork(eEW_CAN_NOT_PRINCSS_WEAPON_CHANGE);
		return	FALSE;
	}

	cPrincessWeaponDefine	item;

	item.m_dwSerial				=	random(10000)+10;
	item.m_wBaseItem			=	g_im.m_wPrincessWeapon;
	item.m_wBasicItem			=	g_im.getTopLevelNormalWeapon(m_wJob,_lpCaster->getLevel());

	if	(item.m_wBasicItem	==	0xffff)
		return	FALSE;

	if	(g_aBasicItem[item.m_wBasicItem].m_wRequireLevel	>	getLevel())
	{
		item.m_wBasicItem		=	g_im.getTopLevelNormalWeapon(m_wJob,getLevel());

		if	(item.m_wBasicItem	==	0xffff	||	g_aBasicItem[item.m_wBasicItem].m_wRequireLevel	>	getLevel())
		{
			_lpCaster->sendEtcWork(eEW_TARGET_LEVEL_TOO_LOW);
			return	FALSE;
		}
	}

	g_im.setPrincessWeaponOptionValue((cItem*)&item,item.m_wBasicItem);

	int	iFireDamage				=	_lpCaster->getFireResistance()*_iMagicDamageFactor/100;
	int	iWaterDamage			=	_lpCaster->getWaterResistance()*_iMagicDamageFactor/100;
	int	iWindDamage				=	_lpCaster->getWindResistance()*_iMagicDamageFactor/100;
	int	iEarthDamage			=	_lpCaster->getEarthResistance()*_iMagicDamageFactor/100;
	int	iLightDamage			=	_lpCaster->getLightResistance()*_iMagicDamageFactor/100;
	int	iDarkDamage				=	_lpCaster->getDarkResistance()*_iMagicDamageFactor/100;

	item.m_bFireDamage			=	min(max(iFireDamage		,0),255);
	item.m_bWaterDamage			=	min(max(iWaterDamage	,0),255);
	item.m_bWindDamage			=	min(max(iWindDamage		,0),255);
	item.m_bEarthDamage			=	min(max(iEarthDamage	,0),255);
	item.m_bLightDamage			=	min(max(iLightDamage	,0),255);
	item.m_bDarkDamage			=	min(max(iDarkDamage		,0),255);

	item.m_dwPrincessNameHashCode=	_lpCaster->m_dwNameHashCode;
	item.m_wPrincessSerial		=	_lpCaster->m_wSerialInField;

	item.m_wAddDamagePercentage	=	_iDamagePercentageFactor;

	memcpy(lpSlot,&item,sizeof(cItem));

	int	iSlot					=	getItemSlotIndex(lpSlot);

	sendUpdateItemData(iSlot,TRUE);

	_lpCaster->setTarget(NULL);
	_lpCaster->emptyBookedBattle();
	m_lpField->stopAllTargetedAction(_lpCaster);

	_lpCaster->m_wTransToWeaponTime						=	_iUpkeepTime;
	_lpCaster->m_wPrincessWeaponReceiver				=	m_wSerialInField;
	_lpCaster->m_dwPrincessWeaponReceiverNameHashCode	=	m_dwNameHashCode;
	_lpCaster->m_wUnsealAction							=	_lpAbility->m_wSkill;
	m_wPrincessWeaponPrincess							=	_lpCaster->m_wSerialInField;
	m_dwPrincessWeaponPrincessNameHashCode				=	_lpCaster->m_dwNameHashCode;
	
	m_lpField->addSendEtcWork(m_wSerialInField,eEW_TRANS_TO_WEAPON,_lpCaster->m_wSerialInField,m_wSerialInField);

	return	TRUE;
}

BOOL
cACTOR::transToRabbit(cAbility *_lpAbility,int _iUpkeepTime)
{
	addSendSkillEffect(_lpAbility,eSE_HIT);

	if	(m_wRabbitTime)
	{
		m_moveSpeed.dispell();
		m_dodgeChance.dispell();
		m_allMagicResistance.dispell();
		m_sNonTargetTime=	0;
		m_wRabbitTime	=	0;

		return	FALSE;
	}

	emptyBookedBattle();

	m_wUnsealAction	=	_lpAbility->m_wSkill;
	m_wRabbitTime	=	_iUpkeepTime;
	m_lpField->stopAllTargetedAction(this);

	return	TRUE;
}

BOOL
cACTOR::jumppingSkipping(cAbility *_lpAbility,int _iUpkeepTime)
{
	if	(m_wJumppingSkippingTime)
	{
		m_moveSpeed.dispell();
		m_wJumppingSkippingTime	=	0;
		return	FALSE;
	}

	emptyBookedBattle();

	m_lpField->stopAllTargetedAction(this);
	m_wUnsealAction			=	_lpAbility->m_wSkill;
	m_wJumppingSkippingTime	=	_iUpkeepTime;

	return	TRUE;
}

BOOL
cACTOR::explosionBottle(cACTOR *_lpCaster,cAbility *_lpAbility,int _iEffecient)
{
	int	iFireDamage		=	0;
	int	iWaterDamage	=	0;
	int	iWindDamage		=	0;
	int	iEarthDamage	=	0;
	int	iLightDamage	=	0;
	int	iDarkDamage		=	0;

	getDamageByBullet(_lpCaster,&iFireDamage,&iWaterDamage,&iWindDamage,&iEarthDamage,&iLightDamage,&iDarkDamage);

	if	(iFireDamage)
		iFireDamage		-=	iFireDamage*getFireResistance()/100;
	if	(iWaterDamage)
		iWaterDamage	-=	iWaterDamage*getWaterResistance()/100;
	if	(iWindDamage)
		iWindDamage		-=	iWindDamage*getWindResistance()/100;
	if	(iEarthDamage)
		iEarthDamage	-=	iEarthDamage*getEarthResistance()/100;
	if	(iLightDamage)
		iLightDamage	-=	iLightDamage*getLightResistance()/100;
	if	(iDarkDamage)
		iDarkDamage		-=	iDarkDamage*getDarkResistance()/100;

	int		iMagicDamage	=	iFireDamage+iWaterDamage+iWindDamage+iEarthDamage+iLightDamage+iDarkDamage;

	iMagicDamage		*=	100;
	iMagicDamage		+=	iMagicDamage*_iEffecient/100;

	if	(m_lpField->isPVP(_lpCaster,this))
		iMagicDamage	=	GetP2PCorrectDamage(iMagicDamage);

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	iMagicDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		iMagicDamage=	(DWORD)llDamage;
	}

	if	(iMagicDamage)
	{
		float	fCorrectIntelligenceValue	=	_lpCaster->getCorrectIntelligenceFactorForMagicDamage();

		iMagicDamage						=	int(iMagicDamage+iMagicDamage*_lpCaster->getIntelligence()*fCorrectIntelligenceValue);

		reduceHP(iMagicDamage,_lpCaster);
		
		addSendHitDamage(_lpCaster,0,iMagicDamage,dATTACK_RESULT_FIELD_HIT,_lpAbility->m_wSkill);
	}

	return	TRUE;
}

BOOL
cACTOR::feignDeath(cAbility *_lpAbility,int _iUpkeepTime)
{
	if	(m_wFeignDeathTime)
	{
		m_wFeignDeathTime	=	0;

		m_allMagicResistance.dispell();
		m_attackPower.dispell();
		m_defensePower.dispell();
		m_wIsFeignDeath		=	FALSE;

		return	FALSE;
	}

	m_lpField->stopAllTargetedAction(this);
	
	m_wUnsealAction		=	_lpAbility->m_wSkill;
	m_wFeignDeathTime	=	_iUpkeepTime;
	m_wIsFeignDeath		=	TRUE;

	return	TRUE;
}

BOOL
cACTOR::bounce(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iRange,int _iDamage)
{
	cACTOR	*lpTarget	=	m_lpField->findNeareastActor(m_iLocalPart,m_wSerialInField,m_pos.x,m_pos.y,_iRange,m_wTeam);//	가장 근접해 있는 누군가를 찾는다.

	if	(!lpTarget)
		return	FALSE;

	int			iDefensivePower	=	lpTarget->getDefensivePower();
	LONGLONG	llDamage		=	_iDamage;
	LONGLONG	llDivedeValue	=	max(llDamage+iDefensivePower,1);

	llDamage					=	llDamage*llDamage/llDivedeValue;

	int			iDamage			=	(int)llDamage;

	lpTarget->reduceHP(iDamage,_lpAttacker);
	lpTarget->addSendHitDamage(_lpAttacker,iDamage,0,dATTACK_RESULT_FIELD_HIT,_lpAbility->m_wSkill);
	lpTarget->operateRevenge(_lpAttacker,_lpAbility);
	thrust(lpTarget,TRUE);
//	이녀석도 대미지 한번 더 입어야 한다.

	return	TRUE;
}

BOOL
cACTOR::dance(int _iTime)
{
	m_wDanceTime			=	max(_iTime,m_wDanceTime);
	m_wBattleInclination	=	dBI_PASSIVE_SOLO;

	stop(TRUE);
	addPacketStop();
	setTarget(NULL);
	setAiReady();

	return	FALSE;
}

BOOL
cACTOR::releaseDance()
{
	m_wDanceTime			=	0;
	m_wBattleInclination	=	m_wOrgBattleInclination;

	return	FALSE;
}

BOOL
cACTOR::makeTheOtherSelf(int _iTime,int _iCount)
{
	if	(m_wTheOtherSelfCount		>	_iCount)
		return	FALSE;

	if	(m_wUpkeepTheOtherSelfTime	>	_iTime)
		return	FALSE;

	m_wTheOtherSelfCount		=	_iCount;
	m_wUpkeepTheOtherSelfTime	=	_iTime;

	m_lpField->addSendEtcWork(m_wSerialInField,eEW_MAKE_THE_OTHER_SELF,m_wSerialInField,m_wTheOtherSelfCount);

	addSendSimpleInfo();

	return	TRUE;
}

BOOL
cACTOR::tryDisarmTrap(int _iTargetType,int _iObjectSerial,int _iDisarmLevel,int _iExplosionChance,int _iUpkeepTime,BOOL _bIsSuccess)
{
	BOOL	bIsExplosion	=	FALSE;

	if	(_bIsSuccess	==	FALSE)
	{
		if	(random(100) < _iExplosionChance)
			bIsExplosion	=	TRUE;
	}

	if	(_iTargetType	==	eTARGET_OBJECT_TRAP	)
	{
		cTrap	*lpTrap	=	m_lpField->getTrap(_iObjectSerial);

		if	(!lpTrap)
			return	FALSE;

		if	(lpTrap->m_wTeam	==	m_wTeam)
		{
			return	FALSE;
		}

		if	(_iDisarmLevel	<	lpTrap->m_wDisarmLevel	||	bIsExplosion)
		{
			sendEtcWork(eEW_DISARM_ACTOR_TRAP_FAILED_BY_IS_CAN_NOT_DISARM_TRAP_BY_LOW_DISARM_LEVEL,_iObjectSerial,lpTrap->m_wDisarmLevel);

			return	FALSE;
		}

		if	(_bIsSuccess	==	FALSE)
		{
			sendEtcWork(eEW_DISARM_ACTOR_TRAP_FAILED_BY_SKILL_FAILED,_iObjectSerial,lpTrap->m_wDisarmLevel);

			return	FALSE;
		}

		m_lpField->removeTrap(_iObjectSerial);

		addSendCloseRangeFieldTrapWork(_iObjectSerial,eFTW_DISARMED_BY_SKILL,0xffff,m_strName);	

		return	TRUE;
	}

	if	(_iTargetType	==	eTARGET_OBJECT_FIELD_TRAP	)
	{
		cAreaTrapInfo	*lpTrap		=	m_lpField->m_pArea->getFieldTrapBySerial(_iObjectSerial);
		
		if	(!lpTrap)
			return	FALSE;

		if	(_iDisarmLevel	<	lpTrap->m_wDisarmTrapLevel)
		{
			sendFieldTrapWork(_iObjectSerial,eFTW_FAILED_BY_IS_CAN_NOT_DISARM_TRAP_BY_LOW_DISARM_LEVEL,lpTrap->m_wDisarmTrapLevel);

			return	FALSE;
		}

		if	(bIsExplosion)
		{
			lpTrap->explosionTrap(this);

			addSendCloseRangeFieldTrapWork(_iObjectSerial,eFTW_DISARM_FAILED_AND_EXPLOSION_TRAP,lpTrap->m_wDisarmTrapLevel,m_strName);

			return	FALSE;
		}

		if	(_bIsSuccess	==	FALSE)
		{
			sendFieldTrapWork(_iObjectSerial,eFTW_DISARM_BY_SKILL_FAILED,lpTrap->m_wDisarmTrapLevel);

			return	FALSE;
		}

		lpTrap->disarm(_iUpkeepTime);

		addSendCloseRangeFieldTrapWork(_iObjectSerial,eFTW_DISARMED_BY_SKILL,0xffff,m_strName);

		return	TRUE;
	}

	if	(_iTargetType	==	eTARGET_OBJECT_ARCA	)
	{
		cArca	*lpArca	=	g_arcaManager.get(_iObjectSerial);

		if	(!lpArca || lpArca->m_wTrapLevel	== 0)
			return	FALSE;

		if	(_iDisarmLevel	<	lpArca->m_wDisarmTrapLevel)
		{
			sendArcaWork(_iObjectSerial,eAW_FAILED_DISARM_BY_LOW_DISARM_LEVEL,lpArca->m_wDisarmTrapLevel);
			return	FALSE;
		}

		if	(bIsExplosion)
		{
			lpArca->explosionTrap(this);

			addSendCloseRangeArcaWork(_iObjectSerial,eAW_DISARM_FAILED_AND_EXPLOSION_TRAP,lpArca->m_wDisarmTrapLevel,m_strName);

			return	FALSE;
		}

		if	(_bIsSuccess	==	FALSE)
		{
			sendArcaWork(_iObjectSerial,eAW_FAILED_DISARM,lpArca->m_wDisarmTrapLevel);
			return	FALSE;
		}

		lpArca->disarmTrap(this);
		addSendCloseRangeArcaWork(_iObjectSerial,eAW_DISARMED_BY_SKILL,0xffff,m_strName);

		return	TRUE;
	}

	if	(_iTargetType	==	eTARGET_OBJECT_DOOR	)
	{
		cAreaDoorInfo	*lpDoor		=	(cAreaDoorInfo	*)m_lpField->getArea(_iObjectSerial);

		if	(!lpDoor || lpDoor->m_bf1IsDisarmed	==	TRUE)
			return	FALSE;

		if	(_iDisarmLevel	<	lpDoor->m_bDisarmTrapLevel)
		{
			sendDoorMessage(_iObjectSerial,0xffff,0xffff,eDWM_TRY_DISARM_FAILED_BY_LOW_SKILL_LEVEL);
			return	FALSE;
		}

		if	(bIsExplosion)
		{
			lpDoor->explosionTrap(this);

			m_lpField->addSendDoorMessage(_iObjectSerial,0xffff,0xffff,eDWM_DISARM_FAILED_AND_EXPLOSION_TRAP,this);

			return	FALSE;
		}

		if	(_bIsSuccess	==	FALSE)
		{
			sendDoorMessage(_iObjectSerial,0xffff,0xffff,eDWM_DISARM_FAILED);
			return	FALSE;
		}

		lpDoor->disarmedTrap(_iUpkeepTime);
		m_lpField->addSendDoorMessage(_iObjectSerial,0xffff,0xffff,eDWM_DISARM,this);
		lpDoor->sendDoorStatus();

		return	TRUE;
	}

	return	FALSE;
}

BOOL
cACTOR::appointmentMakeIllusion(int _iSkill,int _iUpkeepTime,int _iRemainHP,BOOL _bIsFirstTarget,int _iIllusionJob,int _iLimitIllusionCount,int _iX,int _iY)
{
	if	(_iLimitIllusionCount	<=	m_lpField->getIllusionCount(this))
	{
		sendEtcWork(eEW_CAN_NOT_CREATE_MORE_ILLUSION);
		return	FALSE;
	}

	m_lpField->appointmentMakeIllusion(_iSkill,_iX,_iY,m_wSerialInField,m_dwNameHashCode,_iUpkeepTime,_iRemainHP,_bIsFirstTarget,_iIllusionJob);

	return	TRUE;
}

BOOL
cACTOR::stealGold(cACTOR *_lpTarget)
{
	if	(_lpTarget->m_wNpcActorSaveSerial	==	0xffff)
		return	FALSE;

	cNPC		*lpNpc		=	m_lpField->m_apNpcGenerateData[_lpTarget->m_wNpcActorSaveSerial];
	cCHARACTER	*lpCharacter=	&m_lpField->m_aCharacter[lpNpc->m_wCharacter];
	cJOB		*lpJob		=	lpCharacter->getJob();

	int			iDropMoney	=	lpJob->m_wGold+_lpTarget->m_iLevel*lpJob->m_wGoldPerLevel/100;

	iDropMoney	=	max(iDropMoney,1);
	iDropMoney	*=	g_iItemFactor;
	iDropMoney	+=	g_eventManager.getPlusDropGold();
	iDropMoney	=	iDropMoney*g_iCorrectItemDropChanceByGoldRush/100;
	
	iDropMoney	=	min(iDropMoney,dLIMIT_DROP_GOLD);

	iDropMoney	=	min(dMAX_OWN_GOLD-m_iGold,iDropMoney);

	if (iDropMoney	<=	0)
		return FALSE;

	m_iGold		+=	iDropMoney;

	if	(isPlayer())
		updateGoldLog(iDropMoney,eGET_GOLD_BY_STEAL_GOLD);

	_lpTarget->m_bf1IsStolenGold	=	TRUE;

	sendAddGold(iDropMoney);

	return	TRUE;
}

BOOL
cACTOR::stealItem(cACTOR *_lpTarget)
{
	if	(_lpTarget->m_wNpcActorSaveSerial	==	0xffff)
		return	FALSE;

	cNPC			*lpNpc		=	m_lpField->m_apNpcGenerateData[_lpTarget->m_wNpcActorSaveSerial];
	cCHARACTER		*lpCharacter=	&m_lpField->m_aCharacter[lpNpc->m_wCharacter];
	cJOB			*lpJob		=	lpCharacter->getJob();
	int				iDropLevel	=	_lpTarget->m_iLevel/2;
	CDroppedItem	droppedItem;

	for (int i=0;i<dDROPPING_ITEM_COUNT;i++)
	{
		droppedItem.reset();

		BOOL	bIsDropUnique	=	m_lpField->isDropUniqueChance(lpJob->m_aDroppingItem[i].m_wItemType,_lpTarget->m_iLevel,getLuck(),m_bCorrectUniqueItemDropRate , m_wWordEventTime , m_wGuildMasterGrace);
		BOOL	bIsDropRare		=	m_lpField->isDropRareChance(lpJob->m_aDroppingItem[i].m_wItemType,_lpTarget->m_iLevel,m_wCorrectItemDropRate,getLuck(),
																m_bCorrectUniqueItemDropRate,m_bCorrectRareItemDropRate,m_bCorrectMagicItemDropChance , m_wWordEventTime, m_wGuildMasterGrace);

		if	(m_lpField->makeDropItem(_lpTarget,this,&droppedItem,&lpJob->m_aDroppingItem[i],iDropLevel,0xffff,bIsDropUnique,bIsDropRare)	==	FALSE)
			continue;

		if	(m_lpField->isDropItemChance(&droppedItem,&lpJob->m_aDroppingItem[i],this)	==	FALSE)
			continue;

		break;
	}

	if	(droppedItem.m_item.m_wBaseItem	==	0xffff)
		return	FALSE;

	cItem	*lpItem	=	&droppedItem.m_item;

	_lpTarget->m_bf1IsStolenItem	=	TRUE;

	if	(getValidInventorySlot()	==	NULL)
	{
		CDroppedItem	droppedItem;

		memcpy(&droppedItem.m_item,lpItem,sizeof(cItem));
		droppedItem.m_item.m_dwSerial	=	largeRandom(0x7ffffff0)+10;
		droppedItem.m_wOwner			=	m_wSerialInField;
		droppedItem.m_pos.x				=	m_pos.x+random(40)-20;
		droppedItem.m_pos.y				=	m_pos.y+random(40)-20;
		droppedItem.m_wSealedTime		=	6000;

		m_lpField->dropItem(&droppedItem);

		sendRegistServerMessage(eRSM_CAN_NOT_RECEIVE_ITEM_BY_INVENTORY_FULL);

		return	TRUE;
	}

	sendPickItemLog(lpItem,ePIM_STEAL);
	sendAddItem(lpItem);

	if	(lpItem->isMoneyItem())
		addGoldItemToInventory(lpItem,eGET_GOLD_BY_STEAL_GOLD);
	else
		addItemToInventory(lpItem);

	return	TRUE;
}

BOOL
cACTOR::makeShakle(int	_iHP,int	_iDP,cACTOR *_lpCaster,cAbility *_lpAbility,int _iUpkeepTime,int iIsFreeze,int iIsCritical,int iIsSyncDamage,int _iMagicResistance)
{
	if	(m_iShakleHP)
		return	FALSE;

	m_wShakleUpkeepTime						=	_iUpkeepTime;
	m_iShakleHP								=	_iHP;
	m_bf10ShakleCaster						=	_lpCaster->m_wSerialInField;
	m_bf10ShakleSkill						=	_lpAbility->m_wSkill;
	m_bf8ShakleSkillLevel					=	_lpAbility->m_wLevel;
	m_bf4RemainTimeForDamageByShakleSkill	=	_lpAbility->getStrikePeriod()/100;
	m_bf1IsFreezeShakle						=	iIsFreeze;
	m_bf1IsCriticalShakle					=	iIsCritical;
	m_bf1IsSyncDamageShakle					=	iIsSyncDamage;
	m_bf7ShakleMagicResistance				=	_iMagicResistance;

	m_wShakleDefensivePower					=	_iDP/100;

	m_lpField->addSendEtcWork(m_wSerialInField,eEW_MAKE_SHAKLE,m_wSerialInField,_lpAbility->m_wSkill);

	return	TRUE;
}

void
cACTOR::strikeShakleSkillDamage()
{
	cAbility		ability;
	CHitInfo		hitInfo;

	ability.set(m_bf10ShakleSkill,m_bf8ShakleSkillLevel);

	if	(ability.getStrikePeriod()	==	0)
		return;

	cACTOR			*lpCaster	=	m_lpField->getActor(m_bf10ShakleCaster);
	m_bf4RemainTimeForDamageByShakleSkill	=	ability.getStrikePeriod()/100;

	if	(!lpCaster	||	lpCaster->isDeath())
	{
		releaseShakle();

		return;
	}

	getDamage(lpCaster,&ability,0,0,&hitInfo,TRUE,TRUE,FALSE);
	damagedBy(lpCaster,&ability,hitInfo.m_dwPhysicalDamage,hitInfo.m_dwMagicDamage,FALSE,TRUE);

	addSendHitDamage(lpCaster,hitInfo.m_dwPhysicalDamage,hitInfo.m_dwMagicDamage,hitInfo.m_dwResultField,ability.m_wSkill);

	m_lpField->addSendEtcWork(m_wSerialInField,eEW_DAMAGED_BY_SHAKLE,m_wSerialInField,ability.m_wSkill);
}

void
cACTOR::releaseShakle()
{
	m_iShakleHP			=	0;
	m_wShakleUpkeepTime	=	0;

	m_attackInfo.reset();
	m_bookedAction.reset();
	m_bookedBattle.reset();

	stopBattle();

	setTarget(NULL);
	setCurrentTarget(NULL);

	setAiReady();
	
	mDS.m_aiExValue[0]	=	4;
	updateEnchantedMagic();
}

BOOL
cACTOR::isAttackToShakleActor(cACTOR *_lpAttacker)
{
	if	(m_iShakleHP	<=	0)
		return	FALSE;

	if	(m_wSerialInField	==	_lpAttacker->m_wSerialInField)
		return	TRUE;

	if	(_lpAttacker->m_wCharmedTime	==	0	&&	_lpAttacker->isTamer(this))
		return	TRUE;

	if	(m_bf1IsFreezeShakle)
		return	TRUE;

	return	FALSE;
}

BOOL
cACTOR::setImpulse(cACTOR *_lpCaster,int _iTime,int _iDamage,int _iSkill)
{
	if	(m_wRemainImpulseTime)
		return	FALSE;

	m_wRemainImpulseTime			=	_iTime;
	m_wImpulseDamage				=	_iDamage/100;
	m_wImpulseSkill					=	_iSkill;
	m_wImpulseCaster				=	0xffff;

	if	(_lpCaster)
	{
		m_wImpulseCaster				=	_lpCaster->m_wSerialInField;
		m_dwImpulseCasterNameHashCode	=	_lpCaster->m_dwNameHashCode;
	}

	if	(isPlayer())
		sendEtcWork(eEW_SET_IMPULSE_TIME,_iTime);

	return	TRUE;
}

BOOL
cACTOR::setBloodDrain(cACTOR *_lpCaster,int _iTime,int _iPercentage,int _iSkill)
{
	if	(m_wBloodDonorTime	>=	_iTime)
		return	FALSE;

	m_wBloodDonorTime				=	_iTime;
	m_wBloodDonorRate				=	_iPercentage;
	m_wBloodDrainSkill				=	_iSkill;
	m_wBloodDrainCaster				=	0xffff;

	if	(_lpCaster)
	{
		m_wBloodDrainCaster				=	_lpCaster->m_wSerialInField;
		m_dwBloodDrainCasterNameHashCode=	_lpCaster->m_dwNameHashCode;
	}

	return	TRUE;
}

BOOL
cACTOR::setSelfReproof(cACTOR *_lpCaster,int _iTime,int _iDamage,int _iSkill)
{
	if	(m_wSelfReproofTime	>=	_iTime)
		return	FALSE;

	if	(m_lpField->isPVP(_lpCaster,this))
		_iDamage	=	GetP2PCorrectDamage(_iDamage);

	m_wSelfReproofTime				=	_iTime;
	m_wSelfReproofDamage			=	_iDamage/100;
	m_wSelfReproofSkill				=	_iSkill;
	m_wSelfReproofCaster			=	0xffff;

	if	(_lpCaster)
	{
		m_wSelfReproofCaster			=	_lpCaster->m_wSerialInField;
		m_dwSelfReproofCasterNameHashCode=	_lpCaster->m_dwNameHashCode;
	}

	return	TRUE;
}

BOOL
cACTOR::setTorture(cACTOR *_lpCaster,int _iTime,int _iDamage)
{
	if	(m_wTortureTime	>=	_iTime)
		return	FALSE;

	if	(m_lpField->isPVP(_lpCaster,this))
		_iDamage	=	GetP2PCorrectDamage(_iDamage);

	m_wTortureTime				=	_iTime;
	m_wTortureDarkDamage		=	_iDamage/100;
	m_wTortureCaster			=	0xffff;

	if	(_lpCaster)
	{
		m_wTortureCaster				=	_lpCaster->m_wSerialInField;
		m_dwTortureCasterNameHashCode	=	_lpCaster->m_dwNameHashCode;
	}

	return	TRUE;
}

BOOL
cACTOR::setDegenerate(cACTOR *_lpCaster,int _iTime,int _iPropertion,int _iSkill)
{
	if	(m_wDegenerateTime	>=	_iTime)
		return	FALSE;

	m_wDegenerateTime		=	_iTime;
	m_wDegeneratePropertion	=	_iPropertion;
	m_wDegenerateCaster		=	0xffff;
	m_wDegenerateSkill		=	_iSkill;

	if	(_lpCaster)
	{
		m_wDegenerateCaster				=	_lpCaster->m_wSerialInField;
		m_dwDegenerateCasterNameHashCode=	_lpCaster->m_dwNameHashCode;
	}

	return	TRUE;
}

BOOL
cACTOR::setTerribleDream(cACTOR *_lpCaster,int _iTime,int _iDamage,int _iSkill,int _iPeriod)
{
	if	(m_wTerribleDreamTime	>=	_iTime	||	m_wSleepTime	==	0)
		return	FALSE;

	if	(m_lpField->isPVP(_lpCaster,this))
		_iDamage	=	GetP2PCorrectDamage(_iDamage);

	m_wTerribleDreamTime		=	_iTime;
	m_wTerribleDreamDamage		=	_iDamage/100;
	m_wTerribleDreamCaster		=	0xffff;
	m_wTerribleDreamSkill		=	_iSkill;
	m_wTerribleDreamActivePeriod=	_iPeriod;
	m_wSleepTime				=	max(m_wSleepTime,m_wTerribleDreamTime);

	if	(_lpCaster)
	{
		m_wTerribleDreamCaster				=	_lpCaster->m_wSerialInField;
		m_dwTerribleDreamCasterNameHashCode	=	_lpCaster->m_dwNameHashCode;
	}

	m_lpField->stopAllTargetedAction(this,FALSE,TRUE);

	return	TRUE;
}

BOOL
cACTOR::setHwabyung(cACTOR *_lpCaster,int _iTime,int _iFireDamage,int _iDarkDamage,int _iSkill)
{
	if	(m_wHwabyungTime	>	_iTime)
		return	FALSE;

	if	(m_lpField->isPVP(_lpCaster,this))
	{
		_iFireDamage	=	GetP2PCorrectDamage(_iFireDamage);
		_iDarkDamage	=	GetP2PCorrectDamage(_iDarkDamage);
	}

	m_wHwabyungTime				=	_iTime;
	m_wHwabyungFireDamage		=	_iFireDamage/100;
	m_wHwabyungDarkDamage		=	_iDarkDamage/100;
	m_wHwabyungCaster			=	0xffff;
	m_wHwabyungSkill			=	_iSkill;

	m_lpField->stopAllTargetedAction(this,FALSE,TRUE);

	stop();
	setAiReady();
	addPacketStop();

	if	(_lpCaster)
	{
		m_wHwabyungCaster				=	_lpCaster->m_wSerialInField;
		m_dwHwabyungCasterNameHashCode	=	_lpCaster->m_dwNameHashCode;
	}

	sendEtcWork(eEW_HWABYUNG_TIME,m_wHwabyungTime);

	return	TRUE;
}

BOOL
cACTOR::setSmellOfDeath(int _iTime,int _iDarkDamage,int _iReduceResistanceUpkeepTime,int _iReduceResistance,int _iSkill,int _iReduceCurseResistance)
{
	if	(m_wSOD_Time	>=	_iTime)
		return	FALSE;

	m_wSOD_Time					=	_iTime;
	m_wSOD_Damage				=	_iDarkDamage/100;
	m_wSOD_Skill				=	_iSkill;
	m_wSOD_ReduceResistance		=	_iReduceResistance;
	m_wSOD_ReduceCurseResistance=	_iReduceCurseResistance;
	m_wSOD_ReduceResistanceTime	=	_iReduceResistanceUpkeepTime;

	return	TRUE;
}

BOOL
cACTOR::setPlotOfShadow(cACTOR *_lpCaster,int _iTime,int _iDamage,int _iCasterDamage,int _iSkill,int _iLimitTargetCount)
{
	if	(m_wPOS_Time)
		return	FALSE;

	if	(!_lpCaster)
		return	FALSE;

	if	(m_lpField->getPOSTargetCount(_lpCaster)	>=	_iLimitTargetCount)
	{
		_lpCaster->sendEtcWork(eEW_FAILED_TO_USE_POS_BY_ALREADY_EXIST_TARGET,_iLimitTargetCount);
		return	FALSE;
	}

	if	(m_lpField->isPVP(_lpCaster,this))
	{
		_iDamage			=	GetP2PCorrectDamage(_iDamage);
		_iCasterDamage		=	GetP2PCorrectDamage(_iCasterDamage);
	}

	_lpCaster->m_wPOS_Target	=	m_wSerialInField;

	m_wPOS_FullTime				=	_iTime;
	m_wPOS_Time					=	_iTime;
	m_wPOS_Damage				=	_iDamage/100;
	m_wPOS_CasterDamage			=	_iCasterDamage/100;
	m_wPOS_Skill				=	_iSkill;
	m_wPOS_Caster				=	_lpCaster->m_wSerialInField;
	m_dwPOS_CasterNameHashCode	=	_lpCaster->m_dwNameHashCode;

	sendEtcWork(eEW_PLOT_OF_SHADOW_TIME,m_wPOS_Time);

	return	TRUE;
}

BOOL
cACTOR::operatePhantomImpulse(cACTOR *_lpCaster,int _iPropertion,int _iSkill)
{
	if	(_lpCaster->m_iCP	>=	0)
		return	FALSE;

	int		iDamage		=	(-_lpCaster->m_iCP)*_iPropertion/100;
	float	fIntForDamage=	_lpCaster->getCorrectIntelligenceFactorForMagicDamage();
	iDamage				+=	(int)(iDamage*_lpCaster->getIntelligence()*fIntForDamage);

	if	(m_lpField->isPVP(_lpCaster,this))
		iDamage	=	GetP2PCorrectDamage(iDamage);

	reduceHP(iDamage,_lpCaster);
	addSendHitDamage(_lpCaster,0,iDamage,dATTACK_RESULT_FIELD_HIT,_iSkill);

	_lpCaster->m_iCP	=	0;

	return	TRUE;
}


//	삼키기
BOOL
cACTOR::operateSwallow(cACTOR *_lpAttacker)
{
	sendEtcWork(eEW_SWALLOW);
	reduceHP(m_iHP,_lpAttacker);

	setActorPos(_lpAttacker->m_pos.x,_lpAttacker->m_pos.y);

	m_wSwallowCorpesCount++;

	m_bf1IsImmuneKnockBack	=	TRUE;

	addSendSimpleInfo(NULL,FALSE,TRUE);

	return	TRUE;
}

BOOL
cACTOR::setContract(cACTOR *_lpAttacker,int _iUpkeepTime,int _iOccurPeriod,int _iRecoverHP,int _iSkill,int _iDamageSkill,int _iDamageSkillLevel)
{
	if	(m_wCurseAmplificationTime)
		_iUpkeepTime	=	_iUpkeepTime*dCURSE_AMPLIFICATION_FACTOR/100;

	if	(_iUpkeepTime	<	m_wContractTime)
		return	FALSE;

	m_wContractSkill				=	_iSkill;
	m_wContractCaster				=	_lpAttacker->m_wSerialInField;
	m_dwContractCasterNameHashCode	=	_lpAttacker->m_dwNameHashCode;
	m_wContractTime					=	_iUpkeepTime;
	m_wContractOccurPeriod			=	_iOccurPeriod;
	m_wContractRecoverHP			=	_iRecoverHP/100;
	m_wContractDamageSkill			=	_iDamageSkill;
	m_wContractDamageSkillLevel		=	_iDamageSkillLevel;

	return	TRUE;
}

BOOL
cACTOR::makeMarionette(cACTOR *_lpCaster,int _iTime,int _iBoostPower)
{
	if	(m_wCharmedTime)
		return	FALSE;

	if	(m_wLordActor	!=	0xffff	||	m_wLordOperator	!=	0xffff)
		return	FALSE;

	if	(m_wMarionetteTime)
		return	FALSE;

	m_wMarionetteCaster				=	_lpCaster->m_wSerialInField;
	m_dwMarionetteCasterNameHashCode=	_lpCaster->m_dwNameHashCode;
	m_wMarionetteTime				=	_iTime;
	m_wCorrectMarionetteStrength	=	_lpCaster->getStrength()*_iBoostPower/100;
	m_wCorrectMarionetteAgility		=	_lpCaster->getAgility()*_iBoostPower/100;
	m_wCorrectMarionetteConstitution=	_lpCaster->getConstitution()*_iBoostPower/100;
	m_wCorrectMarionetteIntelligence=	_lpCaster->getIntelligence()*_iBoostPower/100;
	m_wCorrectMarionetteWisdom		=	_lpCaster->getWisdom()*_iBoostPower/100;
	m_wCorrectMarionetteLuck		=	_lpCaster->getLuck()*_iBoostPower/100;
	m_wCorrectMarionetteCharisma	=	_lpCaster->getCharisma()*_iBoostPower/100;

	_lpCaster->sendDisplacement(this,TRUE);

	_lpCaster->m_wSlaveActor	=	m_wSerialInField;
	_lpCaster->m_posFeignDeath.x=	_lpCaster->m_pos.x;
	_lpCaster->m_posFeignDeath.y=	_lpCaster->m_pos.y;
	_lpCaster->setActorPos(m_pos.x,m_pos.y,TRUE,TRUE);
	_lpCaster->sendCurrentPosition();

	_lpCaster->stop();
	setAiReady();

	_lpCaster->m_wRemainActionTime	=	0;
	m_wRemainActionTime	=	0;

	m_lpField->addSendEtcWork(0xffff,eEW_SET_MARIONETTE_STATUS,m_wSerialInField,TRUE);

	buildPower();

	return	TRUE;
}

void
cACTOR::releaseMarionette()
{
	cACTOR	*lpCaster	=	getMarionetteLord();

	if	(lpCaster)
	{
		lpCaster->m_wSlaveActor	=	0xffff;
		lpCaster->stop();
		lpCaster->stopBattle();

		m_lpField->stopAllTargetedAction(lpCaster);

		lpCaster->sendDisplacement(this,FALSE);
		lpCaster->setActorPos(lpCaster->m_posFeignDeath.x,lpCaster->m_posFeignDeath.y,TRUE,TRUE);
		lpCaster->sendCurrentPosition();
	}

	m_wMarionetteTime	=	0;
	m_wMarionetteCaster	=	0xffff;
	m_wLordActor		=	0xffff;
	setAiReady();

	m_lpField->stopAllTargetedAction(this);

	m_lpField->addSendEtcWork(0xffff,eEW_SET_MARIONETTE_STATUS,m_wSerialInField,FALSE);

}

BOOL
cACTOR::declineRecoverHPEfficient(int _iTime,int _iEfficient)
{
	if	(_iTime	<	m_wDeclientRecoverHPEfficientTime)
		return	FALSE;

	m_wDeclientRecoverHPEfficientTime	=	_iTime;
	m_wDeclientRecoverHPEfficient		=	_iEfficient;

	sendEtcWork(eEW_DECLINE_RECOVER_HP_EFFICIENT,_iTime,_iEfficient);

	return	TRUE;
}

BOOL
cACTOR::setInterrutingRevive(int _iTime)
{
	if	(_iTime	<	m_wInterruptingReviveTime)
		return	FALSE;

	m_wInterruptingReviveTime	=	_iTime;

	return	TRUE;
}

BOOL
cACTOR::operateBloodyCross(cAbility *_lpAbility)
{
	BOOL		bIsGotedCP	=	TRUE;
	CHitInfo	hitInfo;

	hitInfo.reset(TRUE);

	m_lpField->strikeWideAreaDamageSkill(this,_lpAbility,&m_pos,_lpAbility->getHitRange(),NULL,0,&bIsGotedCP,&hitInfo);

	int			iDamageSum	=	hitInfo.getDamage();

	if	(iDamageSum	<=	0)
		return	TRUE;

	int			iHitRange	=	_lpAbility->getHitRange(getWeapon());

	iHitRange				*=	iHitRange;

	cParty		*lpParty	=	getParty();

	if	(lpParty)
	{
		int	iMemberCount	=	0,iMember,iRecoverHP;

		for	(iMember=0;iMember<lpParty->m_wMemberCount;iMember++)
		{
			cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(iMember);

			if	(!lpMember	||	!isOnSameField(lpMember)	||	lpMember->isDeath())
				continue;

			int		iRange		=	GetOvalRange(m_pos.x,m_pos.y,lpMember->m_pos.x,lpMember->m_pos.y);

			if	(iRange	>=	iHitRange)
				continue;

			iMemberCount++;
		}

		if	(iMemberCount	==	0)
			return	TRUE;

		iRecoverHP	=	iDamageSum/iMemberCount;

		for	(iMember=0;iMember<lpParty->m_wMemberCount;iMember++)
		{
			cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(iMember);

			if	(!lpMember	||	!isOnSameField(lpMember)	||	lpMember->isDeath())
				continue;

			int		iRange		=	GetOvalRange(m_pos.x,m_pos.y,lpMember->m_pos.x,lpMember->m_pos.y);

			if	(iRange	>=	iHitRange)
				continue;

			lpMember->recoverHPByAidSkill(_lpAbility->m_wSkill,iRecoverHP);
		}
	}
	else
	{
		recoverHPByAidSkill(_lpAbility->m_wSkill,iDamageSum);
	}

	return	TRUE;
}

BOOL
cACTOR::setSoulOath(int _iUpkeepTime,int _iGetCP,int _iSkill)
{
	if	(_iUpkeepTime	<	m_wSoulOathTime)
		return	FALSE;

	m_wSoulOathTime	=	_iUpkeepTime;
	m_wSoulOathCP	=	_iGetCP/100;
	m_wSoulOathSkill=	_iSkill;

	return	TRUE;
}

BOOL
cACTOR::setBloodCompact(int _iUpkeepTime,int _iGetHP,int _iSkill)
{
	if	(_iUpkeepTime	<	m_wSoulOathTime)
		return	FALSE;

	m_wBloodCompactTime	=	_iUpkeepTime;
	m_wBloodCompactHP	=	_iGetHP/100;
	m_wBloodCompactSkill=	_iSkill;

	return	TRUE;
}

BOOL
cACTOR::setInhaleEnemy(int _iRange,int _iPeriod,int _iPixel,int _iTime)
{
	cACTOR	*lpFeeler;

	lpFeeler	=	m_lpField->getActorByBody(dJOB_MONSTER_TIAMATH_FEELER);

	if	(lpFeeler)
	{
		lpFeeler->stop();
		lpFeeler->m_wBlockToActionTime		=	_iTime;
	}
	lpFeeler	=	m_lpField->getActorByBody(dJOB_MONSTER_TIAMATH_BOTTOM_FEELER);

	if	(lpFeeler)
	{
		lpFeeler->stop();
		lpFeeler->m_wBlockToActionTime		=	_iTime;
	}

	return	TRUE;
}

void
cACTOR::operateInhaleEnemy()
{
}

BOOL
cACTOR::setInterruptingArmor(int _iCurrentLevel,int _iTime)
{
	if	(_iTime	<	m_wInterruptingArmorTime)
		return	FALSE;

	m_wInterruptingArmorTime	=	_iTime;
	m_wInterruptingArmorLevel	=	_iCurrentLevel;

	if	(isPlayer())
	{
		buildPower();
		sendEtcWork(eEW_INTERRUPTING_ARMOR,m_wInterruptingArmorLevel,m_wInterruptingArmorTime);
	}

	return	TRUE;
}

void
cACTOR::correctValueEffectDuelServer(int* _iValue , CSkill* _lpSkill)
{
	if(!g_bIsDuelServer)
		return;

	if (!*_iValue)
		return;

	if (!_lpSkill->m_bf7DuelServerpenaltyDiv)
		return;

	if (g_iSeasonVariable >= dSEASON_VARIABLE_PVP_REFORM)
	{
		*_iValue = *_iValue / _lpSkill->m_bf7DuelServerpenaltyDiv;
	}
}