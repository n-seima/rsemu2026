#include "stdafx.h"
#include "CSkill.H"
#include "cMESSAGE.H"
#include "hero_mini_pet.H"
#include "miniPetWindow.h"
#include "cACTOR.H"
#include "CSkillDefine.h"

//
//	��E���E��E� ���� ����
void
cAbility::makeDamageTooltip(int _iMinDamage,int _iMaxDamage,int _iMinWeaponDamage,int _iMaxWeaponDamage,BOOL _bIsAidMagic,BOOL _bIsSummonbeastSkill)
{	// ������ ������ : 09.09.29
	if	(isPhysicalDamageSkill())
		makeDamageTooltip(dMSG_PHYSICAL_DAMAGE,_iMinDamage,_iMaxDamage);

	int		iMinMagicDamage,iMaxMagicDamage;
	char	buffer[2][100];

	memcpy(&buffer[0],&g_hero.m_strength,sizeof(g_hero.m_strength));
	memcpy(&buffer[1],&g_hero.m_intelligence,sizeof(g_hero.m_intelligence));

	CSkill	*lpSkill			=	getSkill();

	if	(_bIsAidMagic)
	{
		memset(&g_hero.m_strength		,0,sizeof(g_hero.m_strength));
		memset(&g_hero.m_intelligence	,0,sizeof(g_hero.m_intelligence));
	}

	int	iCorrectMagicDamageValue=	0;

	if	(lpSkill->isPhysicalMagicDamageSkill()	==	FALSE)
		iCorrectMagicDamageValue=	g_hero.m_wAlchemyTitleFactor;

	cHero_mini_pet	*lpHeroForMP	=	g_hero.iMiniPet();

	int				iMiniPetLevel	=	0;

	if	(lpHeroForMP->m_lpMiniPet)
		iMiniPetLevel	=	lpHeroForMP->m_lpMiniPet->m_bf7Level;

	int iAddDamage = 0;

	cMiniPet	*lpMiniPet	=	lpHeroForMP->getMiniPet(g_gwMiniPetStatus.m_iSelectMiniPet);
	BOOL bIsPureMagicSkill = lpSkill->isPureMagicDamageSkill();

	if	(isFireDamageSkill())
	{
		if	(lpSkill->isMiniPetSkill())
		{
			iMaxMagicDamage	=	lpHeroForMP->getMiniPetMagicDamage(this,iMiniPetLevel,lpSkill->m_sFireDamage,lpSkill->m_sFireDamagePerLevel,lpSkill->m_sFireDamageRange,lpSkill->m_sFireDamageRangePerLevel,TRUE);
			iMinMagicDamage	=	lpHeroForMP->getMiniPetMagicDamage(this,iMiniPetLevel,lpSkill->m_sFireDamage,lpSkill->m_sFireDamagePerLevel,lpSkill->m_sFireDamageRange,lpSkill->m_sFireDamageRangePerLevel,FALSE);		

			int		iCorrectDamage	= 0 ;
			iCorrectDamage = lpMiniPet->getPowerUpPercentage(lpSkill->m_wSerial);

			if	(lpHeroForMP->m_bCorrectMiniPetSkillDamage)
				iCorrectDamage += lpHeroForMP->m_bCorrectMiniPetSkillDamage * 100;

			if(iCorrectDamage)
			{
				iMaxMagicDamage += iMaxMagicDamage * (float)iCorrectDamage/10000;
				iMinMagicDamage += iMinMagicDamage * (float)iCorrectDamage/10000;
			}
		}
		else
		{
			iMinMagicDamage	=	getFireDamage(_iMinWeaponDamage,TRUE);
			iMaxMagicDamage	=	getFireDamage(_iMaxWeaponDamage,FALSE);

			//if (_bIsSummonbeastSkill	==	FALSE && !lpSkill->getExceptionstrengtheningSkill())
			if (!lpSkill->getExceptionstrengtheningSkill())
			{
				iMinMagicDamage	+=	iMinMagicDamage*g_hero.getStrengtheningFireDamage(iCorrectMagicDamageValue, bIsPureMagicSkill)/100;
				iMaxMagicDamage	+=	iMaxMagicDamage*g_hero.getStrengtheningFireDamage(iCorrectMagicDamageValue, bIsPureMagicSkill)/100;
			}
		}

		makeDamageTooltip(dMSG_FIRE_DAMAGE,iMinMagicDamage,iMaxMagicDamage);
	}

	if	(isWaterDamageSkill())
	{
		if	(lpSkill->isMiniPetSkill())
		{
			iMaxMagicDamage	=	lpHeroForMP->getMiniPetMagicDamage(this,iMiniPetLevel,lpSkill->m_sWaterDamage,lpSkill->m_sWaterDamagePerLevel,lpSkill->m_sWaterDamageRange,lpSkill->m_sWaterDamageRangePerLevel,TRUE);
			iMinMagicDamage	=	lpHeroForMP->getMiniPetMagicDamage(this,iMiniPetLevel,lpSkill->m_sWaterDamage,lpSkill->m_sWaterDamagePerLevel,lpSkill->m_sWaterDamageRange,lpSkill->m_sWaterDamageRangePerLevel,FALSE);

			int		iCorrectDamage	= 0 ;
			iCorrectDamage = lpMiniPet->getPowerUpPercentage(lpSkill->m_wSerial);
			
			if	(lpHeroForMP->m_bCorrectMiniPetSkillDamage)
				iCorrectDamage += lpHeroForMP->m_bCorrectMiniPetSkillDamage * 100;
			
			if(iCorrectDamage)
			{
				iMaxMagicDamage += iMaxMagicDamage * (float)iCorrectDamage/10000;
				iMinMagicDamage += iMinMagicDamage * (float)iCorrectDamage/10000;
			}
		}
		else
		{
			iMinMagicDamage	=	getWaterDamage(_iMinWeaponDamage,TRUE);
			iMaxMagicDamage	=	getWaterDamage(_iMaxWeaponDamage,FALSE);

			//if (_bIsSummonbeastSkill	==	FALSE && !lpSkill->getExceptionstrengtheningSkill())
			if (!lpSkill->getExceptionstrengtheningSkill())
			{
				iMinMagicDamage	+=	iMinMagicDamage*g_hero.getStrengtheningWaterDamage(iCorrectMagicDamageValue, bIsPureMagicSkill)/100;
				iMaxMagicDamage	+=	iMaxMagicDamage*g_hero.getStrengtheningWaterDamage(iCorrectMagicDamageValue, bIsPureMagicSkill)/100;
			}
		}

		makeDamageTooltip(dMSG_WATER_DAMAGE,iMinMagicDamage,iMaxMagicDamage);
	}

	if	(isWindDamageSkill())
	{
		if	(lpSkill->isMiniPetSkill())
		{
			iMaxMagicDamage	=	lpHeroForMP->getMiniPetMagicDamage(this,iMiniPetLevel,lpSkill->m_sWindDamage,lpSkill->m_sWindDamagePerLevel,lpSkill->m_sWindDamageRange,lpSkill->m_sWindDamageRangePerLevel,TRUE);
			iMinMagicDamage	=	lpHeroForMP->getMiniPetMagicDamage(this,iMiniPetLevel,lpSkill->m_sWindDamage,lpSkill->m_sWindDamagePerLevel,lpSkill->m_sWindDamageRange,lpSkill->m_sWindDamageRangePerLevel,FALSE);

			int		iCorrectDamage	= 0 ;
			iCorrectDamage = lpMiniPet->getPowerUpPercentage(lpSkill->m_wSerial);
			
			if	(lpHeroForMP->m_bCorrectMiniPetSkillDamage)
				iCorrectDamage += lpHeroForMP->m_bCorrectMiniPetSkillDamage * 100;
			
			if(iCorrectDamage)
			{
				iMaxMagicDamage += iMaxMagicDamage * (float)iCorrectDamage/10000;
				iMinMagicDamage += iMinMagicDamage * (float)iCorrectDamage/10000;
			}
		}
		else
		{
			iMinMagicDamage	=	getWindDamage(_iMinWeaponDamage,TRUE);
			iMaxMagicDamage	=	getWindDamage(_iMaxWeaponDamage,FALSE);

			//if (_bIsSummonbeastSkill	==	FALSE && !lpSkill->getExceptionstrengtheningSkill())
			if (!lpSkill->getExceptionstrengtheningSkill())
			{
				iMinMagicDamage	+=	iMinMagicDamage*g_hero.getStrengtheningWindDamage(iCorrectMagicDamageValue, bIsPureMagicSkill)/100;
				iMaxMagicDamage	+=	iMaxMagicDamage*g_hero.getStrengtheningWindDamage(iCorrectMagicDamageValue, bIsPureMagicSkill)/100;
			}
		}

		makeDamageTooltip(dMSG_WIND_DAMAGE,iMinMagicDamage,iMaxMagicDamage);
	}

	if	(isEarthDamageSkill())
	{
		if	(lpSkill->isMiniPetSkill())
		{
			iMaxMagicDamage	=	lpHeroForMP->getMiniPetMagicDamage(this,iMiniPetLevel,lpSkill->m_sEarthDamage,lpSkill->m_sEarthDamagePerLevel,lpSkill->m_sEarthDamageRange,lpSkill->m_sEarthDamageRangePerLevel,TRUE);
			iMinMagicDamage	=	lpHeroForMP->getMiniPetMagicDamage(this,iMiniPetLevel,lpSkill->m_sEarthDamage,lpSkill->m_sEarthDamagePerLevel,lpSkill->m_sEarthDamageRange,lpSkill->m_sEarthDamageRangePerLevel,FALSE);

			int		iCorrectDamage	= 0 ;
			iCorrectDamage = lpMiniPet->getPowerUpPercentage(lpSkill->m_wSerial);
			
			if	(lpHeroForMP->m_bCorrectMiniPetSkillDamage)
				iCorrectDamage += lpHeroForMP->m_bCorrectMiniPetSkillDamage * 100;
			
			if(iCorrectDamage)
			{
				iMaxMagicDamage += iMaxMagicDamage * (float)iCorrectDamage/10000;
				iMinMagicDamage += iMinMagicDamage * (float)iCorrectDamage/10000;
			}
		}
		else
		{
			iMinMagicDamage	=	getEarthDamage(_iMinWeaponDamage,TRUE);
			iMaxMagicDamage	=	getEarthDamage(_iMaxWeaponDamage,FALSE);

			//if (_bIsSummonbeastSkill	==	FALSE && !lpSkill->getExceptionstrengtheningSkill())
			if (!lpSkill->getExceptionstrengtheningSkill())
			{
				iMinMagicDamage	+=	iMinMagicDamage*g_hero.getStrengtheningEarthDamage(iCorrectMagicDamageValue, bIsPureMagicSkill)/100;
				iMaxMagicDamage	+=	iMaxMagicDamage*g_hero.getStrengtheningEarthDamage(iCorrectMagicDamageValue, bIsPureMagicSkill)/100;
			}
		}

		makeDamageTooltip(dMSG_EARTH_DAMAGE,iMinMagicDamage,iMaxMagicDamage);
	}

	if	(isLightDamageSkill())
	{
		if	(lpSkill->isMiniPetSkill())
		{
			iMaxMagicDamage	=	lpHeroForMP->getMiniPetMagicDamage(this,iMiniPetLevel,lpSkill->m_sLightDamage,lpSkill->m_sLightDamagePerLevel,lpSkill->m_sLightDamageRange,lpSkill->m_sLightDamageRangePerLevel,TRUE);
			iMinMagicDamage	=	lpHeroForMP->getMiniPetMagicDamage(this,iMiniPetLevel,lpSkill->m_sLightDamage,lpSkill->m_sLightDamagePerLevel,lpSkill->m_sLightDamageRange,lpSkill->m_sLightDamageRangePerLevel,FALSE);

			int		iCorrectDamage	= 0 ;
			iCorrectDamage = lpMiniPet->getPowerUpPercentage(lpSkill->m_wSerial);
			
			if	(lpHeroForMP->m_bCorrectMiniPetSkillDamage)
				iCorrectDamage += lpHeroForMP->m_bCorrectMiniPetSkillDamage * 100;
			
			if(iCorrectDamage)
			{
				iMaxMagicDamage += iMaxMagicDamage * (float)iCorrectDamage/10000;
				iMinMagicDamage += iMinMagicDamage * (float)iCorrectDamage/10000;
			}
		}
		else
		{
			if (lpSkill->m_wAwakenSkillBonusDamageForOrigin || lpSkill->m_wAwakenSkillBonusDamagePerLevelForOrigin)
			{
				if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken100)
					iAddDamage = 2 * lpSkill->m_wAwakenSkillBonusDamageForOrigin + 2 * lpSkill->m_wAwakenSkillBonusDamagePerLevelForOrigin + g_hero.m_iLevel;
				else if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken50)
					iAddDamage = lpSkill->m_wAwakenSkillBonusDamageForOrigin + lpSkill->m_wAwakenSkillBonusDamagePerLevelForOrigin + g_hero.m_iLevel;
			}

			iMinMagicDamage	=	getLightDamage(_iMinWeaponDamage,TRUE);
			iMaxMagicDamage	=	getLightDamage(_iMaxWeaponDamage,FALSE);

			int iCorrectMinDamage	=	g_hero.getValuePassiveSkill(eSKILL_PEE_CORRECT_LIGHT_MIN_DAMAGE)/100;

			if(iCorrectMinDamage)
			{
				iMinMagicDamage	= iMaxMagicDamage*iCorrectMinDamage/100;
			}
			//if (_bIsSummonbeastSkill	==	FALSE && !lpSkill->getExceptionstrengtheningSkill())
			if (!lpSkill->getExceptionstrengtheningSkill())
			{
				iMinMagicDamage	+=	(float)iMinMagicDamage*g_hero.getStrengtheningLightDamage(iCorrectMagicDamageValue, bIsPureMagicSkill)/100;
				iMaxMagicDamage	+=	(float)iMaxMagicDamage*g_hero.getStrengtheningLightDamage(iCorrectMagicDamageValue, bIsPureMagicSkill)/100;
			}
		}

		makeDamageTooltip(dMSG_LIGHT_DAMAGE,iMinMagicDamage,iMaxMagicDamage);
	}

	if	(isDarkDamageSkill())
	{
		if	(lpSkill->isMiniPetSkill())
		{
			iMaxMagicDamage	=	lpHeroForMP->getMiniPetMagicDamage(this,iMiniPetLevel,lpSkill->m_sDarkDamage,lpSkill->m_sDarkDamagePerLevel,lpSkill->m_sDarkDamageRange,lpSkill->m_sDarkDamageRangePerLevel,TRUE);
			iMinMagicDamage	=	lpHeroForMP->getMiniPetMagicDamage(this,iMiniPetLevel,lpSkill->m_sDarkDamage,lpSkill->m_sDarkDamagePerLevel,lpSkill->m_sDarkDamageRange,lpSkill->m_sDarkDamageRangePerLevel,FALSE);

			int		iCorrectDamage	= 0 ;
			iCorrectDamage = lpMiniPet->getPowerUpPercentage(lpSkill->m_wSerial);
			
			if	(lpHeroForMP->m_bCorrectMiniPetSkillDamage)
				iCorrectDamage += lpHeroForMP->m_bCorrectMiniPetSkillDamage * 100;
			
			if(iCorrectDamage)
			{
				iMaxMagicDamage += iMaxMagicDamage * (float)iCorrectDamage/10000;
				iMinMagicDamage += iMinMagicDamage * (float)iCorrectDamage/10000;
			}
		}
		else
		{
			//JBC 08-08-29
			iMinMagicDamage	=	getDarkDamage(_iMinWeaponDamage,TRUE);
			iMaxMagicDamage	=	getDarkDamage(_iMaxWeaponDamage,FALSE);

			//if (_bIsSummonbeastSkill	==	FALSE && !lpSkill->getExceptionstrengtheningSkill())
			if (!lpSkill->getExceptionstrengtheningSkill())
			{
				iMinMagicDamage	+=	iMinMagicDamage*g_hero.getStrengtheningDarkDamage(iCorrectMagicDamageValue, bIsPureMagicSkill)/100;
				iMaxMagicDamage	+=	iMaxMagicDamage*g_hero.getStrengtheningDarkDamage(iCorrectMagicDamageValue, bIsPureMagicSkill)/100;
			}
		}

		makeDamageTooltip(dMSG_DARK_DAMAGE,iMinMagicDamage,iMaxMagicDamage);
	}

	memcpy(&g_hero.m_strength		,&buffer[0],sizeof(g_hero.m_strength));
	memcpy(&g_hero.m_intelligence	,&buffer[1],sizeof(g_hero.m_intelligence));
}
//
//	��E���E��E� ���� ����
void
cAbility::makeAidMagicDamageTooltip(CSkillExtraEffect *_lpEffect)
{
	if (_lpEffect->m_wEffect	==	eSKILL_EE_POISON)
	{
		int	iValue1		=	_lpEffect->m_aiValue[0];
		int	iValue2		=	_lpEffect->m_aiValue[1];
		int	iDamage		=	iValue1+iValue2*m_wLevel;
		int	iUpkeepTime	=	(_lpEffect->m_sUpkeepTime+_lpEffect->m_sUpkeepTimePerLevel*m_wLevel)/100;

		iDamage			+=	iDamage*g_hero.getIntelligence()*0.005f;

		s_strTooltip.Add(dMSG_POISON_DAMAGE_SKILL_COMMENT_FORM,iDamage/100,iDamage%100,iUpkeepTime);

		return;
	}

	if (_lpEffect->m_wEffect	>=	eSKILL_EE_ADDITIONAL_FIRE_DAMAGE && _lpEffect->m_wEffect	<=	eSKILL_EE_DARK_DAMAGE_SHIELD)
	{
		int	iValue1		=	_lpEffect->m_aiValue[0];
		int	iValue2		=	_lpEffect->m_aiValue[1];
		int	iDamage		=	iValue1+iValue2*m_wLevel;

		iDamage			=	iDamage+iDamage*g_hero.getIntelligence()/200;

		int	iIndex		=	_lpEffect->m_wEffect	-	eSKILL_EE_ADDITIONAL_FIRE_DAMAGE;

		makeDamageTooltip(g_pstrCommonMessage[219+iIndex],iDamage,iDamage);
	}
}

//
//	��E���E����
void
cAbility::makeDamageTooltip(char *_lpstrTitle,int _iMinDamage,int _iMaxDamage)
{
	if (_iMinDamage	==	0	&&	_iMaxDamage	==	0)
		return;

	BOOL	bIsMiniPetSkill	=	getSkill()->isMiniPetSkill();

	int	iShotCount	=	getContinuousShotCount(1-bIsMiniPetSkill);

	if(getSkill()->m_wType == eSKILL_TYPE_MULTI_MISSILE)		// ��Ƽ �̻��� ��ų�̸�E����EȽ���� ���������ʴ´�.
		iShotCount =1;

	if	(iShotCount	>	1)
	{
#ifdef	_FOR_KOREA
		if (_iMinDamage==_iMaxDamage)
			s_strTooltip.Add("- %s <c:LTYELLOW>%d.%.2d<n>��<c:LTYELLOW>%d<n>\n",_lpstrTitle,_iMinDamage/100,_iMinDamage%100,iShotCount);
		else
			s_strTooltip.Add("- %s <c:LTYELLOW>%d.%.2d~%d.%.2d<n>��<c:LTYELLOW>%d<n>\n",_lpstrTitle,_iMinDamage/100,_iMinDamage%100,_iMaxDamage/100,_iMaxDamage%100,iShotCount);
#else
		if (_iMinDamage==_iMaxDamage)
			s_strTooltip.Add("- %s <c:LTYELLOW>%d.%.2d<n>x<c:LTYELLOW>%d<n>\n",_lpstrTitle,_iMinDamage/100,_iMinDamage%100,iShotCount);
		else
			s_strTooltip.Add("- %s <c:LTYELLOW>%d.%.2d~%d.%.2d<n>x<c:LTYELLOW>%d<n>\n",_lpstrTitle,_iMinDamage/100,_iMinDamage%100,_iMaxDamage/100,_iMaxDamage%100,iShotCount);
#endif
	}
	else
	{
		if (_iMinDamage==_iMaxDamage)
			s_strTooltip.Add("- %s <c:LTYELLOW>%d.%.2d<n>\n",_lpstrTitle,_iMinDamage/100,_iMinDamage%100);
		else
			s_strTooltip.Add("- %s <c:LTYELLOW>%d.%.2d~%d.%.2d<n>\n",_lpstrTitle,_iMinDamage/100,_iMinDamage%100,_iMaxDamage/100,_iMaxDamage%100);
	}
}
int
cAbility::getAwakenBoJoDamage(int _iAwakenType, int _iLevel)
{
	CSkill	*lpSkill	=	getSkill();
	
	if	(!lpSkill)
		return	0;
	
	return lpSkill->getAwakenBoJoDamage(_iAwakenType , _iLevel);
}

int
cAbility::getAwakenHitRange(int _iAwakenType, int _iLevel)
{
	CSkill	*lpSkill	=	getSkill();
	
	if	(!lpSkill)
		return	0;

	return lpSkill->getAwakenHitRange(_iAwakenType , _iLevel);
}

int
cAbility::getAwakenActiveChance(int _iAwakenType, int _iLevel)
{
	CSkill	*lpSkill	=	getSkill();
	
	if	(!lpSkill)
		return	0;
	
	return	lpSkill->getAwakenActiveChance(_iAwakenType , _iLevel);

}

int
cAbility::getActiveChance(int _iAwakenType)
{
	CSkill	*lpSkill	=	getSkill();

	if	(!lpSkill)
		return	0;

	int iAddValue			= 0;
	int iAddValuePerLevel	= 0;
	
	if(!lpSkill->m_bf1IsNormalMiniPetSkill)
	{
		if(_iAwakenType == dAWAKENTYPE_100)
		{
			if(lpSkill->m_wType == eSKILL_TYPE_AID_MAGIC)
				iAddValue			= 	lpSkill->m_wAwakenSkillBonusActiveChance * 2 * g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf7Level;
			else
			{
				iAddValue			= 	lpSkill->m_wAwakenSkillBonusActiveChance * 2;
				iAddValuePerLevel	=	lpSkill->m_wAwakenSkillBonusActiveChancePerLevel * 2;
			}
		}
		else if(_iAwakenType == dAWAKENTYPE_50)
		{
			if(lpSkill->m_wType == eSKILL_TYPE_AID_MAGIC)
				iAddValue			= 	lpSkill->m_wAwakenSkillBonusActiveChance * 2 * g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf7Level;
			else
			{
				iAddValue			= 	lpSkill->m_wAwakenSkillBonusActiveChance;
				iAddValuePerLevel	=	lpSkill->m_wAwakenSkillBonusActiveChancePerLevel;
			}
		}
	}
	
	return	((lpSkill->m_sActiveChance+iAddValue)+(lpSkill->m_sActiveChancePerLevel +iAddValuePerLevel)*m_wLevel);

	//return lpSkill->getActiveChance(_iAwakenType , m_wLevel);
}

void
cAbility::getEE_ToolTip(cSTRING *_lpString,CSkillExtraEffect *_lpEffect,CSkillExtraEffectInfo *_lpEE)
{
	CSkill	*lpSkill	=	getSkill();
	int		iUpkeepTime	=	getUpkeepTime();

	BOOL	bIsMiniPetSkill	=	lpSkill->isMiniPetSkill();

	if	(_lpEE->m_iUpkeepTime)
		iUpkeepTime		=	_lpEE->m_iUpkeepTime/100;

	int	iActiveChance	=	_lpEE->m_sActiveChance;

	int	iValue1			=	_lpEE->m_aiValue[0];
	int	iValue2			=	_lpEE->m_aiValue[1];
	int	iValue3			=	_lpEE->m_aiValue[2];
	int	iValue4			=	_lpEE->m_aiValue[3];
	int	iValue5			=	_lpEE->m_aiValue[4];
	int	iValue6			=	_lpEE->m_aiValue[5];
	int	iValue7			=	_lpEE->m_aiValue[6];
	int	iValue8			=	_lpEE->m_aiValue[7];
	int	iValue9			=	_lpEE->m_aiValue[8];
	int	iValue10		=	_lpEE->m_aiValue[9];

	if	(iActiveChance	==	0)
		iActiveChance	=	100;

	if	(bIsMiniPetSkill)
	{
		switch(_lpEE->m_wEffect)
		{
			case	eSKILL_EE_CORRECT_FIELD_FIRE_PENELTY	:
			case	eSKILL_EE_CORRECT_FIELD_WATER_PENELTY	:
			case	eSKILL_EE_CORRECT_FIELD_WIND_PENELTY	:
			case	eSKILL_EE_CORRECT_FIELD_EARTH_PENELTY	:
			case	eSKILL_EE_CORRECT_FIELD_LIGHT_PENELTY	:
			case	eSKILL_EE_CORRECT_FIELD_DARK_PENELTY	:
			{
				int		iAttr		=	dWM_ATTR_MESSAGE_BEGIN+_lpEE->m_wEffect-eSKILL_EE_CORRECT_FIELD_FIRE_PENELTY;
				int		iValue		=	(iValue1+iValue2*m_wLevel)/100;

				
				_lpString->Add(dMSG_STT_DECREASE_FILED_MAGIC_PENELTY_FORM,g_pstrCommonMessage[iAttr],iValue);

				break;
			}

			case	eSKILL_EE_INCREASE_FINAL_PHYSICAL_ATTACK_POWER	:
			{
				int		iValue	=	iValue1+iValue2*m_wLevel;

				_lpString->Add(dMSG_STT_INCREASE_PLAYER_FINAL_PHYSICAL_ATTACK_POWER_FORM,iValue/100,iValue%100);
				break;
			}
			
			case	eSKILL_EE_DECLINE_TARGET_MAGIC_RESISTANCE	:
			{
				int		iValue	=	iValue1+iValue2*m_wLevel;
				
				_lpString->Add(dMSG_STT_DECLINE_TARGET_MAGIC_RESISTANCE_FORM,iValue/100,iValue%100);
				break;
			}
			case	eSKILL_EE_INCLINE_DODGE_CHANCE				:
			{
				
				int		iValue	=	iValue1+iValue2*m_wLevel;
				_lpString->Add(dMSG_STT_INCLINE_DODGE_CHANCE_FORM,iValue/100,iValue%100);
				break;
			}
			case	eSKILL_EE_INCLINE_HIT_CHANCE				:
			{
				
				int		iValue	=	iValue1+iValue2*m_wLevel;
				_lpString->Add(dMSG_STT_INCLINE_HIT_CHANCE_FORM,iValue/100,iValue%100);
				break;
			}
			case	eSKILL_EE_STRENGTHENING_MAGIC_DAMAGE		:
			{
				int		iValue	=	iValue1+iValue2*m_wLevel;
				_lpString->Add(dMSG_STT_STRENGTHENING_MAGIC_DAMAGE_FORM,iValue/100,iValue%100);
				break;
			}
			case	eSKILL_EE_CORRECT_ITEM_EQUIP_LEVEL			:
			{
				int		iValue	=	(iValue1+iValue2*m_wLevel)/100;
				_lpString->Add(dMSG_STT_CORRECT_ITEM_EQUIP_LEVEL_FORM,iValue);
				break;
			}

			case	eSKILL_EE_INCREASE_SKILL_LEVEL				:
			{
				int		iValue	=	(iValue1+iValue2*m_wLevel)/100;
				_lpString->Add(dMSG_STT_INCREASE_SKILL_LEVEL_FORM,iValue);
				break;
			}
			case	eSKILL_EE_INCREASE_DEFENSE_POWER_PERCENTAGE	:
			{
				int		iValue	=	(iValue1+iValue2*m_wLevel)/100;

				g_hero.correctValueEffectDuelServer(&iValue , lpSkill);

				_lpString->Add(dMSG_STT_INCREASE_DEFENSE_POWER_PERCENTAGE_FORM,iValue);
				break;
			}
			case	eSKILL_EE_CURE_STRANGE_STATUS			:
			{
				int	iOccurPeriod=	lpSkill->m_sOccurActionPeriod/100;
				iActiveChance	=	lpSkill->m_sActiveChance;

				if	(bIsMiniPetSkill	&&	lpSkill->m_bf1IsXMiniPetLevel)
					iActiveChance	+=	lpSkill->m_sActiveChancePerLevel*m_wLevel*g_hero.m_lpMiniPet->m_bf7Level;
				else
					iActiveChance	+=	lpSkill->m_sActiveChancePerLevel*m_wLevel;

				if(g_iSeasonVariable >= dSEASON_VARIABLE_MINIPET_POWER_UP_2)		//1����E���� �ߵ�
				{
					int	iValue1		=	_lpEE->m_aiValue[0];		//����ġ.
					int	iValue2		=	_lpEE->m_aiValue[1];
					
					if(iValue2 != 0)
						iActiveChance += iValue1 + g_hero.m_iLevel * 100 / iValue2;
				}

				_lpString->Add(dMSG_STT_CURE_STRANGE_STATUS_FORM,iOccurPeriod,iActiveChance/100,iActiveChance%100);
				break;	
			}
			case	eSKILL_EE_GOLD_HOLDER					:
			{
				iActiveChance	=	getActiveChance();

				int	iOccurPeriod=	lpSkill->m_sOccurActionPeriod/100;
				int	iAttackRange=	getAttackRange(NULL,0);
				_lpString->Add(dMSG_STT_GOLD_HOLDER_FORM,iOccurPeriod,iAttackRange,iActiveChance/100,iActiveChance%100);
				break;
			}

			case	eSKILL_EE_ITEM_COLLECTOR				:
			{
				iActiveChance	=	getActiveChance();

				int	iOccurPeriod=	lpSkill->m_sOccurActionPeriod/100;
				int	iAttackRange=	getAttackRange(NULL,0);
				_lpString->Add(dMSG_STT_ITEM_COLLECTOR_FORM,iOccurPeriod,iAttackRange,iActiveChance/100,iActiveChance%100);
				break;
			}
			case	eSKILL_EE_INCLINE_ATTACK_SPEED			:
			{
				int	iValue		=	(iValue1+iValue2*m_wLevel)/100;

				_lpString->Add(dMSG_STT_INCLINE_ATTACK_SPEED_FORM,iValue);
				break;
			}
			case	eSKILL_EE_INCLINE_MOVE_SPEED			:
			{
				int	iValue		=	(iValue1+iValue2*m_wLevel)/100;

				_lpString->Add(dMSG_STT_INCLINE_MOVE_SPEED_FORM,iValue);
				break;
			}

			case	eSKILL_EE_RECOVER_HP					:
			{
				int	iSecond		=	lpSkill->m_sOccurActionPeriod/100;
				int	iMiliSecond	=	lpSkill->m_sOccurActionPeriod%100;
				int	iValue		=	(iValue1+iValue2*m_wLevel)/100;
				int	iActiveChance=	getActiveChance();

				if	(bIsMiniPetSkill)
				{
					if	(bIsMiniPetSkill	&&	lpSkill->m_bf1IsXMiniPetLevel)
						iActiveChance	+=	lpSkill->m_sActiveChancePerLevel*m_wLevel*g_hero.m_lpMiniPet->m_bf7Level;
					else
						iActiveChance	+=	lpSkill->m_sActiveChancePerLevel*m_wLevel;

					if (g_iSeasonVariable >= dSEASON_VARIABLE_MINIPET_POWER_UP_2)//JBC	�̴�ƁE2E��ȭ ƁE��ó ġ��E+ ƁE�����׷��̼� ȿ��E����E���� ����.	08-11-03
					{
						int iTempVlue3 = _lpEffect->m_aiValue[3];
						if(iTempVlue3 == 0)
							iTempVlue3 = 1;
						iValue += _lpEffect->m_aiValue[2] / 100 + g_hero.m_iLevel / iTempVlue3;
					}
				}

				if	(iActiveChance)
					_lpString->Add(dMSG_STT_RECOVER_HP_WITH_CHANCE_FORM,iSecond,iMiliSecond,iActiveChance/100,iActiveChance%100,iValue);
				else
					_lpString->Add(dMSG_STT_RECOVER_HP_WITHOUT_CHANCE_FORM,iSecond,iMiliSecond,iValue);
				break;
			}

			case	eSKILL_EE_INCLINE_MAX_CP				:	//	CP����"	
			{
				int		iValue	=	(iValue1+iValue2*m_wLevel)/100;
				_lpString->Add(dMSG_STT_INCLINE_MAX_CP_FORM,iValue);
				break;
			}

			case	eSKILL_EE_INCLINE_MAX_HP				:	//	ü������"	
			{
				int		iValue	=	(iValue1+iValue2*m_wLevel)/100;
				_lpString->Add(dMSG_STT_INCLINE_MAX_HP_FORM,iValue);
				break;
			}

			case	eSKILL_EE_CHARGE_CP		:
			{
				int	iValue		=	(iValue1+iValue2*m_wLevel);

				if(g_iSeasonVariable >= dSEASON_VARIABLE_MINIPET_POWER_UP_2)
				{
					if(iValue4 == 0)
						iValue4 = 1;
					
					iValue += iValue3 + g_hero.m_iLevel * 100 / iValue4;
				}

				_lpString->Add(dMSG_STT_CHARGE_CP_FORM,iValue/100,iValue%100);

				break;
			}

			case	eSKILL_EE_INCLINE_MAGIC_RESISTANCE			:	//	���� ���� ����
			case	eSKILL_EE_INCLINE_FIRE_RESISTANCE			:	//	������ ����
			case	eSKILL_EE_INCLINE_WATER_RESISTANCE			:	//	������ ����
			case	eSKILL_EE_INCLINE_EARTH_RESISTANCE			:	//	��E����� ����
			case	eSKILL_EE_INCLINE_WIND_RESISTANCE			:	//	�ٶ����� ����
			case	eSKILL_EE_INCLINE_LIGHT_RESISTANCE			:	//	������ ����
			case	eSKILL_EE_INCLINE_DARK_RESISTANCE			:
			{
				int		iAttr		=	dWM_ATTR_MESSAGE_BEGIN+_lpEE->m_wEffect-eSKILL_EE_INCLINE_FIRE_RESISTANCE;
				int		iValue		=	(iValue1+iValue2*m_wLevel)/100;
				int		iOccurPeriod=	lpSkill->m_sOccurActionPeriod/100;

				iActiveChance	=	lpSkill->m_sActiveChance;

				if	(bIsMiniPetSkill	&&	lpSkill->m_bf1IsXMiniPetLevel)
					iActiveChance	+=	lpSkill->m_sActiveChancePerLevel*m_wLevel*g_hero.m_lpMiniPet->m_bf7Level;
				else
					iActiveChance	+=	lpSkill->m_sActiveChancePerLevel*m_wLevel;

				
				if	(iActiveChance	<	10000)
					_lpString->Add(dMSG_STT_EFFECT_ACTIVE_CHANCE_AND_PERIOD_FORM,iOccurPeriod,iActiveChance/100,iActiveChance%100);

				if	(_lpEE->m_wEffect	==	eSKILL_EE_INCLINE_MAGIC_RESISTANCE)
					_lpString->Add(dMSG_STT_INCLINE_ALL_MAGIC_RESISTANCE_FORM,iValue);
				else
					_lpString->Add(dMSG_STT_INCLINE_MAGIC_RESISTANCE_FORM,g_pstrCommonMessage[iAttr],iValue);

				break;
			}
		}
	}

	switch(_lpEE->m_wEffect)
	{
		case	eSKILL_EE_TAME							:
			_lpString->Add(dMSG_ABLE_TO_TAME_MONSTER_GRADE_FORM,g_pstrMonsterGrade[iValue4-1]);
			break;
		case	eSKILL_EE_CHAIN_FIRE_DAMAGE				:	
		case	eSKILL_EE_CHAIN_WATER_DAMAGE			:	
		case	eSKILL_EE_CHAIN_WIND_DAMAGE				:	
		case	eSKILL_EE_CHAIN_EARTH_DAMAGE			:	
		case	eSKILL_EE_CHAIN_LIGHT_DAMAGE			:	
		case	eSKILL_EE_CHAIN_DARK_DAMAGE				:	
		{
			int	iDamage		=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);
			int	iDamageRange=	_lpEffect->m_aiValue[2];
			int	iGap		=	_lpEffect->m_aiValue[3];
			int	iCount		=	(_lpEffect->m_aiValue[4]+_lpEffect->m_aiValue[5]*m_wLevel)/100;
			int	iLimitCount	=	_lpEffect->m_aiValue[6]/100;

			int	iCorrectMagicDamageValue=	0;
			BOOL bIsPure = lpSkill->isPureMagicDamageSkill();
			if	(bIsPure)
				iCorrectMagicDamageValue=	g_hero.m_wAlchemyTitleFactor;

			iCount			=	min(iCount,iLimitCount);

			int	iMinDamage	=	iDamage-iDamageRange;
			int	iMaxDamage	=	iDamage+iDamageRange;

			iMinDamage		+=	iMinDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill);
			iMaxDamage		+=	iMaxDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill);
			int iDamagePercent = g_hero.m_strengtheningFireDamage.getCorrectedValue();

			
			switch(_lpEE->m_wEffect)
			{
			case	eSKILL_EE_CHAIN_FIRE_DAMAGE				:	
				iMinDamage	+=	iMinDamage*g_hero.getStrengtheningFireDamage(iCorrectMagicDamageValue, bIsPure)/100;
				iMaxDamage	+=	iMaxDamage*g_hero.getStrengtheningFireDamage(iCorrectMagicDamageValue, bIsPure)/100;
				break;
			case	eSKILL_EE_CHAIN_WATER_DAMAGE			:	
				iMinDamage	+=	iMinDamage*g_hero.getStrengtheningWaterDamage(iCorrectMagicDamageValue, bIsPure)/100;
				iMaxDamage	+=	iMaxDamage*g_hero.getStrengtheningWaterDamage(iCorrectMagicDamageValue, bIsPure)/100;
				break;
			case	eSKILL_EE_CHAIN_WIND_DAMAGE				:	
				iMinDamage	+=	iMinDamage*g_hero.getStrengtheningWindDamage(iCorrectMagicDamageValue, bIsPure)/100;
				iMaxDamage	+=	iMaxDamage*g_hero.getStrengtheningWindDamage(iCorrectMagicDamageValue, bIsPure)/100;
				break;
			case	eSKILL_EE_CHAIN_EARTH_DAMAGE			:	
				iMinDamage	+=	iMinDamage*g_hero.getStrengtheningEarthDamage(iCorrectMagicDamageValue, bIsPure)/100;
				iMaxDamage	+=	iMaxDamage*g_hero.getStrengtheningEarthDamage(iCorrectMagicDamageValue, bIsPure)/100;
				break;
			case	eSKILL_EE_CHAIN_LIGHT_DAMAGE			:	
				iMinDamage	+=	iMinDamage*g_hero.getStrengtheningLightDamage(iCorrectMagicDamageValue, bIsPure)/100;
				iMaxDamage	+=	iMaxDamage*g_hero.getStrengtheningLightDamage(iCorrectMagicDamageValue, bIsPure)/100;
				break;
			case	eSKILL_EE_CHAIN_DARK_DAMAGE				:	
				iMinDamage	+=	iMinDamage*g_hero.getStrengtheningDarkDamage(iCorrectMagicDamageValue, bIsPure)/100;
				iMaxDamage	+=	iMaxDamage*g_hero.getStrengtheningDarkDamage(iCorrectMagicDamageValue, bIsPure)/100;
				break;
			}


			int		iAttr			=	_lpEE->m_wEffect-eSKILL_EE_CHAIN_FIRE_DAMAGE;
			char	*lpstrDamageAttr=	g_pstrCommonMessage[dWM_DAMAGE_ATTR_MESSAGE_BEGIN+iAttr];

			_lpString->Add(dMSG_STT_CHANIN_MAGIC_DAMAGE_FORM,iMinDamage/100,iMinDamage%100,iMaxDamage/100,iMaxDamage%100,lpstrDamageAttr,iCount);
			
			break;
		}

		case	eSKILL_EE_BLOOD_COMPACT					:
		{
			int	iGetHP			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);

			_lpString->Add(dMSG_STT_BLOOD_COMPACT,iGetHP/100);

			break;
		}
		case	eSKILL_EE_BLEEDING					:
		{
			
			int		iBleedingDamagePercent	=	(_lpEE->m_aiValue[0]+_lpEE->m_aiValue[1]*m_wLevel);
			int		iBleedingPeriod			=	_lpEE->m_aiValue[3];
			int		iBleedingTime			=	_lpEE->m_iUpkeepTime/100;
			if(_lpEE->m_aiValue[2])
				iBleedingDamagePercent	=	min(_lpEE->m_aiValue[2], iBleedingDamagePercent);

			iBleedingDamagePercent	/=		100;

			_lpString->Add(dMSG_FORM_EE_BLEEDING,iBleedingTime,iBleedingDamagePercent,iBleedingPeriod, iActiveChance);

			break;
		}
		case	eSKILL_EE_ELECTRIC_SHOCK					:
		{
			
			//int		iElectricShockDamagePercent	=	(_lpEE->m_aiValue[0]+_lpEE->m_aiValue[1]*m_wLevel);

			int		iElectricShockTime				=	_lpEE->m_iUpkeepTime/100;
		//	if(_lpEE->m_aiValue[2])
			//	iElectricShockDamagePercent	=	min(_lpEE->m_aiValue[2], iElectricShockDamagePercent);

		//	iElectricShockDamagePercent	/=		100;

			_lpString->Add(dMSG_FORM_EE_ELECTRIC_SHOCK,iElectricShockTime, iActiveChance);
					
			break;
		}
		case	eSKILL_EE_INSTANCE_HEAL_EFFECT					:
		{
			
			int		iInstanceHealEffect		=	(_lpEE->m_aiValue[0]+_lpEE->m_aiValue[1]*m_wLevel);
			int		iInstanceHealEffectTime	=	_lpEE->m_iUpkeepTime/100;

			if(_lpEE->m_aiValue[2])
				iInstanceHealEffect			=	min(_lpEE->m_aiValue[2], iInstanceHealEffect);

			iInstanceHealEffect				/=		100;

			_lpString->Add(dMSG_FORM_EE_INSTANCE_HEAL_EFFECT,iInstanceHealEffectTime,iInstanceHealEffect);

			break;
		}
		case	eSKILL_EE_MAKE_TYPE_UNDEAD					:
		{
			
			int		iMakeTypeUndeadTime			=	_lpEE->m_iUpkeepTime/100;

			_lpString->Add(dMSG_FORM_EE_MAKE_TYPE_UNDEAD,iMakeTypeUndeadTime, iActiveChance);

			break;
		}
		case	eSKILL_EE_SOUL_OATH						:
		{
			int	iGetCP			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);

			_lpString->Add(dMSG_STT_SOUL_OATH_FORM,iGetCP/100);

			break;
		}

		case	eSKILL_EE_CONTRACT						:
		{
			int	iOccurPeriod	=	_lpEffect->m_aiValue[0]/100;
			int	iRecoverHP		=	(_lpEffect->m_aiValue[1]+_lpEffect->m_aiValue[2]*m_wLevel);

			_lpString->Add(dMSG_STT_CONTRACT_FORM,iOccurPeriod,iRecoverHP/100);
	
			break;
		}

		case	eSKILL_EE_INTERRUPTING_REVIVE			:
		{
			_lpString->Add(dMSG_STT_INTERRUPTING_REVIVE_FROM,iUpkeepTime);

			break;
		}

		case	eSKILL_EE_DECLINE_RECOVER_HP_EFFICIENT	:
		{
			_lpString->Add(dMSG_STT_DECLINE_RECOVER_HP_EFFICIENT_FORM,iUpkeepTime);

			break;
		}

		case	eSKILL_EE_PERIODICAL_DARK_DAMAGE_BY_SLEEP	:
		{
			int	iValue1				=	_lpEffect->m_aiValue[0];
			int	iValue2				=	_lpEffect->m_aiValue[1];
			int	iDamage				=	(iValue1+iValue2*m_wLevel);

			iDamage					+=	(int)(iDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));
			
			_lpString->Add(dMSG_STT_PERIODICAL_DARK_DAMAGE_BY_SLEEP_FROM,iDamage/100,iDamage%100);

			break;;
		}

		case	eSKILL_NOTIFICATION_OF_DEATH				:
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iLimitTime		=	(iValue1+iValue2*m_wLevel)/100;
			int	iDeathChance	=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*m_wLevel)/100;
			int	iHitRange		=	getHitRange(NULL);

			iUpkeepTime			=	max(iUpkeepTime,iLimitTime);

			_lpString->Add(dMSG_STT_NOTIFICATION_OF_DEATH_FORM,iUpkeepTime);
			
			break;;
		}

		case	eSKILL_EE_CURE_POISON						:
		{
			_lpString->Add(dMSG_STT_CURE_POISON_FORM,iActiveChance);

			break;
		}

		case	eSKILL_EE_INCLINE_POWER						:	//	������"	
		case	eSKILL_EE_INCLINE_AGILITY					:	//	��ø������"
		case	eSKILL_EE_INCLINE_CONSTITUTION				:	//	�ǰ�����"	
		case	eSKILL_EE_INCLINE_CHARISMA					:	//	ī����������
		case	eSKILL_EE_INCLINE_INTELIGENCE				:	//	��������"	
		case	eSKILL_EE_INCLINE_WISDOM					:	//	��������"	
		case	eSKILL_EE_INCLINE_LUCK						:	//	������"	
		{
			int		iValue1			=	_lpEffect->m_aiValue[0];
			int		iValue2			=	_lpEffect->m_aiValue[1];
			int		iValue			=	(iValue1+iValue2*m_wLevel)/100;
			BOOL	bIsOverlapping	=	_lpEffect->m_aiValue[2];
			int		iOveralappingLimit=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*m_wLevel)/100;
			int		iPower			=	_lpEE->m_wEffect-eSKILL_EE_INCLINE_POWER;
			char	*lpstrPower		=	g_pstrEquipUseLimitContents[iPower+1];
			int		iValueForIncPowerLevel =	0 ;

			//JBC	�̴�ƁE��E� ���� ���� ���� ����E	08-11-03
			switch (g_iSeasonVariable)
			{
			case	0:			//0����E�ᱹ ���� �����̶�� ����...
				iValueForIncPowerLevel		=	0;
				break;
			default:			// �̴�ƁE2E��ȭ(����E1) E�.
				iValueForIncPowerLevel		=	g_hero.m_iLevel / 10 + (_lpEffect->m_aiValue[2])/100;
				break;
			}

// 			switch (_lpEE->m_wEffect)
// 			{
// 			case eSKILL_EE_INCLINE_POWER:			g_hero.m_strength.addValue(iValueForIncPowerLevel);		break;
// 			case eSKILL_EE_INCLINE_AGILITY:			g_hero.m_agility.addValue(iValueForIncPowerLevel);		break;
// 			case eSKILL_EE_INCLINE_CONSTITUTION:	g_hero.m_constitution.addValue(iValueForIncPowerLevel);	break;
// 			case eSKILL_EE_INCLINE_CHARISMA:		g_hero.m_charisma.addValue(iValueForIncPowerLevel);		break;
// 			case eSKILL_EE_INCLINE_INTELIGENCE:		g_hero.m_intelligence.addValue(iValueForIncPowerLevel);	break;
// 			case eSKILL_EE_INCLINE_WISDOM:			g_hero.m_wisdom.addValue(iValueForIncPowerLevel);		break;
// 			case eSKILL_EE_INCLINE_LUCK:			g_hero.m_luck.addValue(iValueForIncPowerLevel);			break;
// 			}

			if	(bIsMiniPetSkill)
				_lpString->Add(dMSG_STT_INCLINE_POWER_BY_MP_FORM,lpstrPower,iValue + iValueForIncPowerLevel,iUpkeepTime);
			else
			if	(bIsOverlapping)
				_lpString->Add(dMSG_STT_OVERLAPPING_INCLINE_POWER_FORM,lpstrPower,iValue,iUpkeepTime,iOveralappingLimit);
			else
				_lpString->Add(dMSG_STT_INCLINE_POWER_FORM,lpstrPower,iValue,iUpkeepTime);
			break;
		}

		case	eSKILL_EE_FIRE_DAMAGE				:	//	�Ҵ�E���E		case	eSKILL_EE_WATER_DAMAGE				:	//	����E���E		case	eSKILL_EE_WIND_DAMAGE				:	//	�ٶ�E��E���E		case	eSKILL_EE_EARTH_DAMAGE				:	//	��E�E��E���E		case	eSKILL_EE_LIGHT_DAMAGE				:	//	��E��E���E		case	eSKILL_EE_DARK_DAMAGE				:	//	���� ��E���E		{
			int	iDamage				=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);
			int	iReduceResistance	=	(_lpEffect->m_aiValue[4]+_lpEffect->m_aiValue[5]*m_wLevel)/100;
			int	iAttr				=	_lpEE->m_wEffect-eSKILL_EE_FIRE_DAMAGE;

			iDamage					+=	iDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill);

			int	iCorrectMagicDamageValue=	0;

			if	(lpSkill->isPhysicalMagicDamageSkill()	==	FALSE)
				iCorrectMagicDamageValue=	g_hero.m_wAlchemyTitleFactor;

			if	(lpSkill->isPureMagicDamageSkill())
			{
				switch(_lpEE->m_wEffect)
				{
					case	eSKILL_EE_FIRE_DAMAGE				:	
						iDamage	+=	iDamage*g_hero.getStrengtheningFireDamage(iCorrectMagicDamageValue)/100;
						break;
					case	eSKILL_EE_WATER_DAMAGE			:	
						iDamage	+=	iDamage*g_hero.getStrengtheningWaterDamage(iCorrectMagicDamageValue)/100;
						break;
					case	eSKILL_EE_WIND_DAMAGE				:	
						iDamage	+=	iDamage*g_hero.getStrengtheningWindDamage(iCorrectMagicDamageValue)/100;
						break;
					case	eSKILL_EE_EARTH_DAMAGE			:	
						iDamage	+=	iDamage*g_hero.getStrengtheningEarthDamage(iCorrectMagicDamageValue)/100;
						break;
					case	eSKILL_EE_LIGHT_DAMAGE			:	
						iDamage	+=	iDamage*g_hero.getStrengtheningLightDamage(iCorrectMagicDamageValue)/100;
						break;
					case	eSKILL_EE_DARK_DAMAGE				:	
						iDamage	+=	iDamage*g_hero.getStrengtheningDarkDamage(iCorrectMagicDamageValue)/100;
						break;
				}
			}

			char	*lpstrAttr		=	g_pstrCommonMessage[dWM_ATTR_MESSAGE_BEGIN+iAttr];
			char	*lpstrDamageAttr=	g_pstrCommonMessage[dWM_DAMAGE_ATTR_MESSAGE_BEGIN+iAttr];

			if	(iReduceResistance)
				_lpString->Add(dMSG_STT_ATTR_DAMAGE_WITH_REDUCE_RESISTENCE_FORM,lpstrAttr,iReduceResistance,iDamage/100,iDamage%100,lpstrDamageAttr);
			else
				_lpString->Add(dMSG_STT_ATTR_DAMAGE_FORM,iDamage/100,iDamage%100,lpstrDamageAttr);

			break;
		}

		case		eSKILL_EE_MAKE_SHAKLE					:
		{
			int		iHP	=	_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel;
			int		iDP	=	_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*m_wLevel;

			_lpString->Add(dMSG_STT_SHAKLE_FORM,iUpkeepTime,iHP/100,iDP/100);

			break;
		}

		case	eSKILL_EE_ANIMATE_PARTNER					:
		{
			_lpString->Add(dMSG_STT_ANIMATE_UNDEAD_FORM,iUpkeepTime);

			break;
		}

		case	eSKILL_EE_DEGENERATE						:	
		{
			int		iPropertion	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel)/100;

			_lpString->Add(dMSG_STT_DEGENERATE_FORM,iPropertion);

			break;
		}

		case	eSKILL_EE_MARIONETTE						:	//	�������׶�
		{
			int	iBoostPower	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel)/100;
			int	iLimit		=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*m_wLevel)/100;

			iBoostPower		=	min(iBoostPower,iLimit);

			_lpString->Add(dMSG_STT_MARIONETTE_FORM,iBoostPower);

			break;
		}

		case	eSKILL_EE_FIRST_AID					:
		{
			BOOL	bIsProportionToRemainHPAndHitDamage	=	_lpEE->m_aiValue[4];
			BOOL	bIsWantSettingUpkeepTime			=	_lpEE->m_aiValue[5];

			if	(bIsWantSettingUpkeepTime	==	FALSE)
				iUpkeepTime	=	0;

			int		iValue1		=	_lpEE->m_aiValue[0];
			int		iValue2		=	_lpEE->m_aiValue[1];
			int		iValue		=	iValue1+iValue2*m_wLevel;
			int		iLimit		=	(_lpEE->m_aiValue[2]+_lpEE->m_aiValue[3]*m_wLevel);

			if	(bIsMiniPetSkill)
			{
				int	iOccurPeriod	=	lpSkill->m_sOccurActionPeriod/100;
				int	iActiveChance	=	lpSkill->m_sActiveChance;

				if	(bIsMiniPetSkill	&&	lpSkill->m_bf1IsXMiniPetLevel)
					iActiveChance	+=	lpSkill->m_sActiveChancePerLevel*m_wLevel*g_hero.m_lpMiniPet->m_bf7Level;
				else
					iActiveChance	+=	lpSkill->m_sActiveChancePerLevel*m_wLevel;

				if(g_iSeasonVariable >= dSEASON_VARIABLE_MINIPET_POWER_UP_2)//JBC	�̴�ƁE2E��ȭ ƁE����óġ ȿ��E����E���� ����.	08-11-03
				{
					//0���� ������ ���� ���� �ϴ� �۾�E
					int iTempValue7 = _lpEffect->m_aiValue[7];
					if(_lpEffect->m_aiValue[7] == 0)
						iTempValue7 = 1;
					
					iValue += _lpEffect->m_aiValue[6] + g_hero.m_iLevel * 100 / iTempValue7;
				}

				_lpString->Add(dMSG_STT_EFFECT_ACTIVE_CHANCE_AND_PERIOD_FORM,iOccurPeriod,iActiveChance/100,iActiveChance%100);
			}

			{
				if	(iValue	&&	bIsProportionToRemainHPAndHitDamage	==	FALSE)
					_lpString->Add("- %s <c:LTYELLOW>%d<n>(%s)\n",dMSG_FIRST_AID,iValue/100,dMSG_FIRST_AID_RULE);
				else
					_lpString->Add(dMSG_STT_DAMAGED_FIRST_AID_FORM,iValue/100);
			}

			break;
		}
		case	eSKILL_EE_BREEDING_RECORD			:
		{
			int	iLimitLevel	=	(iValue1+iValue2*m_wLevel)/100;

			s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_LIMIT_LEVEL,iLimitLevel);
			break;
		}

		case	eSKILL_EE_WARP_NEAREST_VILLAGE								:
		{
			int	iLimitRange	=	iValue1+iValue2*m_wLevel;

			s_strTooltip.Add(dMSG_JUMP_ABLE_DISTANCE_FROM,iLimitRange);
			break;
		}

		case	eSKILL_EE_TRANS_TO_WEAPON	:
		{
			s_strTooltip.Add(dMSG_TRANS_TO_WEAPON_UPKEEP_TIME_FORM,iUpkeepTime);
			break;
		}

		case	eSKILL_EE_INCLINE_ATTACK_POWER_PERCENTAGE_FOR_PET_COMMAND	:
		{
			int	iValue			=	(iValue1+iValue2*m_wLevel)/100;
			
			if	(_lpEffect->m_bf4TargetMethod	==	eEEAT_PET)
				s_strTooltip.Add(dMSG_SEE_INCLINE_ATTACK_POWER_POWER_PERCENTAGE_FOR_PET_COMMAND_FORM,iValue);
			if	(_lpEffect->m_bf4TargetMethod	==	eEEAT_SUMMON_BEAST)
				s_strTooltip.Add(dMSG_SEE_INCLINE_ATTACK_POWER_POWER_PERCENTAGE_FOR_SB_COMMAND_FORM,iValue);
			if	(_lpEffect->m_bf4TargetMethod	==	eEEAT_PET_AND_BEAST)
				s_strTooltip.Add(dMSG_SEE_INCLINE_ATTACK_POWER_POWER_PERCENTAGE_FOR_PET_AND_SB_COMMAND_FORM,iValue);
			break;
		}
		case	eSKILL_EE_INCLINE_DEFENSE_POWER_PERCENTAGE_FOR_PET_COMMAND	:
		{
			int	iValue			=	(iValue1+iValue2*m_wLevel)/100;

			if	(_lpEffect->m_bf4TargetMethod	==	eEEAT_PET)
				s_strTooltip.Add(dMSG_SEE_INCLINE_DEFENSE_POWER_PERCENTAGE_FOR_PET_COMMAND_FORM,iValue);
			if	(_lpEffect->m_bf4TargetMethod	==	eEEAT_SUMMON_BEAST)
				s_strTooltip.Add(dMSG_SEE_INCLINE_DEFENSE_POWER_PERCENTAGE_FOR_SB_COMMAND_FORM,iValue);
			if	(_lpEffect->m_bf4TargetMethod	==	eEEAT_PET_AND_BEAST)
				s_strTooltip.Add(dMSG_SEE_INCLINE_DEFENSE_POWER_PERCENTAGE_FOR_PET_AND_SB_COMMAND_FORM,iValue);
			break;
		}

		case	eSKILL_EE_INCLINE_WEAPON_ATTACK_POWER		:
		{
			int	iLimitValue		=	(iValue3+iValue4*m_wLevel);
			int	iValue			=	(iValue1+iValue2*m_wLevel);

			if	(iLimitValue)
				iValue			=	min(iValue,iLimitValue);

			s_strTooltip.Add(dMSG_SEE_INCLINE_WEAPON_ATTACK_POWER_FORM,iValue/100,iValue%100);

			break;
		}

		case	eSKILL_EE_INCLINE_ARMOR_DEFENSE_POWER		:
		{
			int	iLimitValue		=	(iValue3+iValue4*m_wLevel);
			int	iValue			=	(iValue1+iValue2*m_wLevel);

			if	(iLimitValue)
				iValue			=	min(iValue,iLimitValue);

			s_strTooltip.Add(dMSG_SEE_INCLINE_ARMOR_DEFENSE_POWER_FORM,iValue/100,iValue%100);

			break;
		}

		case	eSKILL_EE_INCREASE_LEVEL	:
		{
			int	iAddLevel	=	iValue1+iValue2*m_wLevel;
			int	iLimitLevel	=	(iValue3+iValue4*m_wLevel)/100;
			int	bIsRoundOff	=	iValue5;

			if	(bIsRoundOff)
				if	((iAddLevel%100) >=	50)
					iAddLevel+=	50;

			iAddLevel		/=	100;

			if	(iLimitLevel )
				iAddLevel		=	min(iAddLevel,iLimitLevel);

			s_strTooltip.Add(dMSG_SEE_INCREASE_LEVEL_FORM,iAddLevel);

			break;
		}


		case	eSKILL_EE_BONUS_EXP					:
		{
			int	iBonusExp	=	(iValue1+iValue2*m_wLevel)/100;
			int	iLimit		=	(iValue3+iValue4*m_wLevel)/100;

			if	(iLimit)
				iBonusExp		=	min(iBonusExp,iLimit);

			s_strTooltip.Add(dMSG_SEE_BONUS_EXP_FORM,iBonusExp);
			break;
		}

		case	eSKILL_EE_PET_BOOST					:
		{
			int	iBoostPower	=	(iValue1+iValue2*m_wLevel)/100;
			int	iLimitCount	=	(iValue3+iValue4*m_wLevel)/100;

			iLimitCount		=	min(iLimitCount,iValue5/100);

			s_strTooltip.Add(dMSG_SEE_PET_BOOST_FORM,iLimitCount,iBoostPower);
			break;
		}

		case	eSKILL_EE_SET_TRAP					:
		{
			int	iRange			=	(_lpEE->m_aiValue[0]+_lpEE->m_aiValue[1]*m_wLevel)/100;
			int	iLimitTrapCount	=	getShotCount();

			s_strTooltip.Add(dMSG_SEE_SET_TRAP_FORM,iRange,iLimitTrapCount);

			char strSpeed[100];

			int	iAttackSpeed	=	getAttackSpeed();
			int	iSetSpeed		=	iAttackSpeed*100/dSYNC_FPS_IN_SERVER;

			sprintf(strSpeed,dMSG_SET_TRAP_SPEED_FORM,iSetSpeed/100,iSetSpeed%100);
			s_strTooltip.Add("- %s\n",strSpeed);
			break;
		}

		case	eSKILL_EE_PHANTOM_IMPULSE			:
		{
			if	(g_hero.m_iCP	>	0)
			{
				s_strTooltip.Add("- %s\n",dMSG_IS_ONLY_ACTIVE_UNDER_MINUS_CP);
				break;
			}

			int		iPropertion		=	(_lpEE->m_aiValue[0]+_lpEE->m_aiValue[1]*m_wLevel)/100;
			int		iLimitPropertion=	_lpEE->m_aiValue[2]/100;

			iPropertion				=	min(iPropertion,iLimitPropertion);

			s_strTooltip.Add(dMSG_STT_HIT_DARK_DAMAGE_TO_TARGET_FORM,iPropertion);

			break;
		}

		case	eSKILL_EE_SMELL_OF_DEATH					:
		{
			int	iReduceResistance			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel)/100;
			int	iReduceResistanceUpkeepTime	=	_lpEffect->m_aiValue[3]/100;
			int	iLimitReduceResistance		=	_lpEffect->m_aiValue[2]/100;
			int	iDarkDamage					=	(_lpEffect->m_aiValue[4]+_lpEffect->m_aiValue[5]*m_wLevel);
			iDarkDamage						+=	iDarkDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill);
			
			iReduceResistance				=	min(iReduceResistance,iLimitReduceResistance);

			//JBC ������ �ⱁE������E��ȭ ȿ���� ���� ������E��ȭ�� �軁EѴ�. 08-08-29	
			iDarkDamage  +=	iDarkDamage*g_hero.getStrengtheningDarkDamage(g_hero.m_wAlchemyTitleFactor)/100;			

			s_strTooltip.Add(dMSG_STT_SOD_REDUCE_RESISTANSE_FORM,iReduceResistance);
			s_strTooltip.Add(dMSG_STT_SOD_ADD_DARK_DAMAGE_FORM,iDarkDamage/100,iDarkDamage%100);

			break;
		}

		case	eSKILL_EE_TERRIBLE_DREAMS					:	//����
		{
			int		iDamage			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);
			int		iActivePeriod	=	_lpEffect->m_aiValue[2];
			iDamage					+=	iDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill);

			//JBC ���� ������E��ȭ ȿ���� ���� ������E��ȭ�� �軁EѴ�. 08-08-29
			iDamage		+= iDamage*g_hero.getStrengtheningDarkDamage(g_hero.m_wAlchemyTitleFactor)/100;

			s_strTooltip.Add(dMSG_STT_TERRIBLE_DREAMS_FROM,iActivePeriod,iDamage/100,iDamage%100);
			break;
		}

		case	eSKILL_EE_HWABYUNG							:	
		{
			int	iDarkDamage			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);
			int	iFireDamage			=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*m_wLevel);
			int	iFireDamageRange	=	_lpEffect->m_aiValue[4];

			iDarkDamage				+=	(int)(iDarkDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));
			iFireDamage				+=	(int)(iFireDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

			//JBC ������E��ȭ ȿ���� ���� ������E��ȭ�� �軁EѴ�. 08-08-29	
			iDarkDamage		+=	iDarkDamage*g_hero.getStrengtheningDarkDamage(g_hero.m_wAlchemyTitleFactor)/100;
			iFireDamage		+=	iFireDamage*g_hero.getStrengtheningFireDamage(g_hero.m_wAlchemyTitleFactor)/100;		// 09.09.2009-10-19 ���� 5:1229
			s_strTooltip.Add(dMSG_STT_HWABYUNG_FROM,iUpkeepTime,iDarkDamage/100,iDarkDamage%100,iFireDamage/100,iFireDamage%100,iFireDamageRange/100,iFireDamageRange%100);
			break;
		}

		case	eSKILL_EE_PLOT_OF_SHADOW					:			//������ �׸���
		{
			int		iTargetDamage	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);
			int		iCasterDamage	=	(_lpEffect->m_aiValue[2]+_lpEffect->m_aiValue[3]*m_wLevel);

			int		iLimitTargetCount=	_lpEffect->m_aiValue[4]/100;

			
			iTargetDamage			+=	(int)(iTargetDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

			//JBC ������E��ȭ ȿ���� ���� ������E��ȭ�� �軁EѴ�. 08-08-29	
			iTargetDamage  +=	iTargetDamage*g_hero.getStrengtheningDarkDamage(g_hero.m_wAlchemyTitleFactor)/100;
			iCasterDamage  +=	iCasterDamage*g_hero.getStrengtheningDarkDamage(g_hero.m_wAlchemyTitleFactor)/100;

			s_strTooltip.Add(dMSG_STT_PLOT_OF_SHADOW_TARGET_DAMAGE_FORM,iTargetDamage/100,iTargetDamage%100);
			s_strTooltip.Add(dMSG_STT_PLOT_OF_SHADOW_CASTER_DAMAGE_FORM,iCasterDamage/100,iCasterDamage%100);

			break;
		}

		case	eSKILL_EE_IMPULSE							:
		{
			int	iMinUpkeepTime	=	_lpEffect->m_aiValue[2]/100;

			iUpkeepTime		=	max(iUpkeepTime,iMinUpkeepTime);

			int	iDamage		=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);

			iDamage			+=	(int)(iDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

			//JBC	�浿 ��ų ���� ��ȭ ȿ��E����E			iDamage			+=	iDamage*g_hero.getStrengtheningDarkDamage(g_hero.m_wAlchemyTitleFactor)/100;

			s_strTooltip.Add(dMSG_STT_IMPULSE_DAMAGE_FORM,iUpkeepTime,iDamage/100,iDamage%100);

			break;
		}

		case	eSKILL_EE_SELF_REPROOF						:
		{
			int	iDamage			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);

			iDamage				+=	(int)(iDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

			//JBC	��å�� ��ų ���� ��ȭ ȿ��E����E			iDamage			+=	iDamage*g_hero.getStrengtheningDarkDamage(g_hero.m_wAlchemyTitleFactor)/100;

			s_strTooltip.Add(dMSG_STT_SELF_REPROOF_DAMAGE_FORM,iDamage/100,iDamage%100);

			break;
		}

		case	eSKILL_EE_TORTURE							:
		{
			int		iDamage		=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);
			int		iAddDamage	=	(int)(iDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));
			iDamage				+=	iAddDamage;

			//JBC	��ծ ��ų ���� ��ȭ ȿ��E����E			iDamage			+=	iDamage*g_hero.getStrengtheningDarkDamage(g_hero.m_wAlchemyTitleFactor)/100;

			s_strTooltip.Add(dMSG_STT_TORTURE_DAMAGE_FORM,iDamage/100,iDamage%100);

			break;
		}

		case	eSKILL_EE_DAMAGE_POOL						:
		{
			int		iEfficient	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);
			int		iMinEfficient=	_lpEffect->m_aiValue[2];
			iEfficient			=	max(iEfficient,iMinEfficient);
			int		iLinkCount	=	_lpEffect->m_aiValue[3]*m_wLevel;
			int		iMaxLinkCount=	_lpEffect->m_aiValue[4]/100;
			int		iRange		=	_lpEffect->m_aiValue[5]+_lpEffect->m_aiValue[6]*m_wLevel;

			if	(iLinkCount	%	100)
				iLinkCount	=	(iLinkCount/100+1)*100;

			iLinkCount			/=	100;
			iLinkCount			=	min(iLinkCount,iMaxLinkCount);

			s_strTooltip.Add(dMSG_STT_DAMAGE_POOL_FORM,iEfficient/100,iEfficient%100,iRange,iLinkCount);

			break;
		}
		case eSKILL_EE_REDUCE_CP			:
			{		// CP����

			int		iReduceCP	= (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);
			int		iPreiod		=	lpSkill->m_sAuraActivePeriod;
			
			
			s_strTooltip.Add(dMSG_EE_REDUCE_CP_FORM,iPreiod/100,iReduceCP/100);				
			break;
			}
		case eSKILL_EE_DRAIN_CP			:
			{		// CP��؁E				int	iDrainCP			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);
				float	fIntForDamage	=	g_hero.getCorrectIntelligenceFactorForMagicDamage();
				iDrainCP			+=	(int)(iDrainCP*g_hero.getIntelligence()/4*fIntForDamage);	
				g_hero.correctValueEffectDuelServer(&iDrainCP , lpSkill);
				s_strTooltip.Add(dMSG_EE_DRAIN_CP_FORM,iDrainCP/100);				
				break;
			}
		case eSKILL_EE_SOUL_BURN			:
			{		// ���ε�E������E
			s_strTooltip.Add(dMSG_EE_SOUL_BURN_FORM, _lpEffect->m_aiValue[3]);				// Ÿ���� ���ε�E������ۡ �ɸ���E�����ð� ���� %d�ʸ��� �Ҳɵ������� ��ȁE
			break;
			}
		case	eSKILL_EE_RAGE_SOUL		:
			{	// ���� �г�E				int iRageSoulChance		=	(_lpEffect->m_aiValue[3] + _lpEffect->m_aiValue[4]*m_wLevel)/100;
				float	fRageSoul			=	_lpEffect->m_aiValue[0] + _lpEffect->m_aiValue[1]*m_wLevel;
				float	fRageSoulMinDamage = 0;
				float	fRageSoulMaxDamage = 0;
				float	fIntForDamage	=	g_hero.getCorrectIntelligenceFactorForMagicDamage();
				
				fRageSoul			+=	(int)(fRageSoul*g_hero.getIntelligence()*fIntForDamage);
				fRageSoul			+=	fRageSoul*g_hero.getStrengtheningFireDamage(g_hero.m_wAlchemyTitleFactor)/100;
				
				fRageSoulMinDamage = (fRageSoul - _lpEffect->m_aiValue[2])/100;
				fRageSoulMaxDamage = (fRageSoul + _lpEffect->m_aiValue[2])/100;
				s_strTooltip.Add(dMSG_EE_RAGE_SOUL_FORM,iRageSoulChance, fRageSoulMinDamage, fRageSoulMaxDamage );				// %dȮ��E� %f~%f�� �Ҳ� ��E����� ��ȁE
				break;
			}
		case	eSKILL_EE_FIGHT_ENERGY:
			{
				
				WORD	wFightEnergyTime	= iUpkeepTime;
				WORD	wFightEnergyState	= _lpEffect->m_aiValue[9];			
				int		iFightEnergyValue	= (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);
				if(_lpEffect->m_aiValue[2] != 0)
					iFightEnergyValue = min(iFightEnergyValue, _lpEffect->m_aiValue[2]);
				
				switch(wFightEnergyState)
				{
				case eSKILL_FEE_ADD_ABSOLUTE_ATTACK_POWER :	//	�ı� ( ����E���ݷ� ��ġ �߰�)
					{
						iFightEnergyValue /= 100;
						s_strTooltip.Add(dMSG_FEE_FORM_ADD_ABSOLUTE_ATTACK_POWER,wFightEnergyTime, iFightEnergyValue);
						break;
					}
				case eSKILL_FEE_INCREASE_ATTACK_COUNT :		//	��ǳ ( ���� Ƚ��E����)
					{
						iFightEnergyValue /= 100;
						s_strTooltip.Add(dMSG_FEE_FORM_INCREASE_ATTACK_COUNT,wFightEnergyTime, iFightEnergyValue);
						
						break;
					}
				case eSKILL_FEE_INCREASE_GET_CP :		//	���� (ȹ��ECP ����)
					{
						iFightEnergyValue /= 100;
						s_strTooltip.Add(dMSG_FEE_FORM_INCREASE_GET_CP,wFightEnergyTime, iFightEnergyValue);
						
						break;
					}
				case eSKILL_FEE_INCREASE_ABSOLUTE_CRITICAL_PERCENT :		//	���ݴ� ( ����Eũ��Ƽ��% ����)
					{
						iFightEnergyValue /= 100;
						s_strTooltip.Add(dMSG_FEE_FORM_INCREASE_ABSOLUTE_CRITICAL_PERCENT,wFightEnergyTime, iFightEnergyValue);
						
						break;
					}
				case eSKILL_FEE_DECREASE_COOL_TIME :		//	�ⴁE( ��Ÿ�� ���� )
					{
						iFightEnergyValue /= 100;
						s_strTooltip.Add(dMSG_FEE_FORM_FEE_DECREASE_COOL_TIME,wFightEnergyTime, iFightEnergyValue);
						
						break;					
					}
				}
				
				break;
			}
		case	eSKILL_EE_IGNORE_BLOCKING		:
			{	// ��Ӱŷ����
				int iIgnoreBlockingValue = (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel)/100;
				if(_lpEffect->m_aiValue[2] != 0)
					iIgnoreBlockingValue = min(iIgnoreBlockingValue, _lpEffect->m_aiValue[2]);
				
				s_strTooltip.Add(dMSG_EE_FORM_IGNORE_BLOCKING,iIgnoreBlockingValue );				//	"- ��Ӱ ���� <c:LTYELLOW>%d��<n>",
				break;
			}	
// �߰�
		case	eSKILL_EE_ATTACK_DOPPELGANGER		:
			{	// �нŰ��� �ΰ�ȿ��E
					int		iAttackCount	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*m_wLevel);
					if(_lpEffect->m_aiValue[2])
						iAttackCount	=	min(_lpEffect->m_aiValue[2], iAttackCount);
					iAttackCount	/=	100;
					int		iDamagePercent	=	(_lpEffect->m_aiValue[3]+_lpEffect->m_aiValue[4]*m_wLevel)/100;
					float		fActionPerion = _lpEffect->m_aiValue[5]+_lpEffect->m_aiValue[6]*m_wLevel;

					if(_lpEffect->m_aiValue[7])
						fActionPerion	=	max(_lpEffect->m_aiValue[7],fActionPerion);
					fActionPerion	/=	1000;
				
				s_strTooltip.Add(dMSG_EE_FORM_ATTACK_DOPPELGANGER,fActionPerion,iDamagePercent,iAttackCount );		
				//	"- �н��� <c:LTYELLOW>%.2f<n> �ʸ��� ����E���ݷ��� <c:LTYELLOW>%d��<n>�� ���ݷ����� <c:LTYELLOW>%d<n> ȸ ����",
				break;
				
			}
		case	eSKILL_EE_ADD_BIT		:
			{//��Ʈ ��E				iActiveChance += g_hero.getValuePassiveSkill(eSKILL_PEE_ADD_BIT_CHANCE)/100;
				s_strTooltip.Add(dMSG_EE_FORM_ADD_BIT,iActiveChance );
				break;
			}

	}
}

//
//	���� ����-o-
char*
cAbility::getTooltip(int _iCorrectLevel,BOOL _isPetResurrection, int _iLevel)
{
	BOOL		bIsCanNotCalcInfo	=	FALSE;
	CSkill		*lpSkill			=	getSkill();
	cMiniPet    *lpMiniPet			=	&g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet];

	if	(!lpSkill)
		return	NULL;

	//CSkill		*lpPowerUpSkill		=	lpSkill->getPowerUpSkill();

	//lpSkill->getExtraEffectByEffect();

	if	(lpSkill->m_bf1IsSummonedBeastSkill)
	{
		cSummonBeast*	lpBeastInfo	=	g_hero.getSummonBeastByType(lpSkill->m_bf2OperateSummonBeast,lpSkill->m_bf2RequireSummonBeastGrade);

		if	(lpBeastInfo)
			g_hero.setWantSummonedBeastData(lpBeastInfo->m_wIndex);
		else
		{
			g_hero.setWantSummonedBeastData(0xffff-1);
			bIsCanNotCalcInfo	=	TRUE;
		}
	}

	if	(lpSkill->m_wType	==	eSKILL_TYPE_PET_COMMAND	&&	lpSkill->m_wCommandSkill	!=	0xffff)
	{
		CSkill		*lpCommandSkill		=	&g_aSkill[lpSkill->m_wCommandSkill];
		cAbility	ability;

		ability.set(lpSkill->m_wCommandSkill,m_wLevel);

		WORD	wEnableJob				=	lpCommandSkill->m_wEnableJob;

		lpCommandSkill->m_wEnableJob	=	lpSkill->m_wEnableJob;

		char*	lpstrTooltip			=	ability.getTooltip(_iCorrectLevel);

		lpCommandSkill->m_wEnableJob	=	wEnableJob;

		return	lpstrTooltip;
	}

	BOOL	bIsMiniPetSkill	=	FALSE;

	if	(lpSkill->isMiniPetSkill())
		bIsMiniPetSkill	=	TRUE;

	BOOL	bIsMonsterSkill	=	FALSE;

	if	(lpSkill->m_wEnableJob	==	dJOB_CODE_MONSTER)
		bIsMonsterSkill	=	TRUE;

	int	i,iOriginalLevel	=	m_wLevel-_iCorrectLevel;

	s_strTooltip.Reset();

	if	(iOriginalLevel > 0)
	{
		if	(_iCorrectLevel)
			s_strTooltip.Add("<c:LTYELLOW>%s<n><Lv %d+<c:LTGREEN>%d<n>>\n\n",lpSkill->m_strName,iOriginalLevel,_iCorrectLevel);
		else
			s_strTooltip.Add("<c:LTYELLOW>%s<n><Lv %d>\n\n",lpSkill->m_strName,m_wLevel);
	}
	else
		s_strTooltip.Add("<c:LTYELLOW>%s<n>\n\n",lpSkill->m_strName);

	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_INFORMATION);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_SKILL_DIFFICULTY,lpSkill->m_wDifficultyLevel);

	if	(g_bIsDuelServer && lpSkill->m_bf1IsBlockInDuelField)
	{
		s_strTooltip.Add("<c:LTRED>- %s<n>\n",dMSG_IS_BLOCK_IN_DUEL_FIELD);
	}

//	CP
	if	(iOriginalLevel)
	{
		int	iAttackSpeed		=	getAttackSpeed();
		int	iCorrectAttackSpeed	=	getCorrectAttackSpeed();
		int	iMinDamage			=	g_hero.getAttackPower(this,TRUE);
		int	iMaxDamage			=	g_hero.getAttackPower(this,FALSE);
		int	iMinWeaponDamage	=	g_hero.getWeaponDamage(TRUE);
		int	iMaxWeaponDamage	=	g_hero.getWeaponDamage(FALSE);

		if	(bIsMiniPetSkill	==	FALSE && lpSkill->m_wType	!=	eSKILL_TYPE_PASSIVE)		// �нú�E����̸�ECP�� ���������ʴ´�E
		{
			int	iRequireCP			=	c_aMinimumCP[lpSkill->m_wDifficultyLevel-1];
			int	iSpentCP			=	getSpentCP();
			int	iGetCP				=	getGetCP();
			if(g_hero.m_fightEnergy.isIncreaseGetCP())
			{
				iGetCP += g_hero.m_fightEnergy.m_iValue;
			}
			if	(iSpentCP	||	iRequireCP)
			{
				if	(iSpentCP)
					iSpentCP	=	max(iSpentCP/100,1);

				if	(lpSkill->m_wCastMethod	==	eST_PASSIVE)
					s_strTooltip.Add("- <c:CTBLUE>%s<n> <c:LTYELLOW>%d<n>\n",dMSG_REQUIRE_CP,iSpentCP);
				else
				{
					s_strTooltip.Add("- <c:CTBLUE>%s<n> <c:LTYELLOW>%d<n>\n",dMSG_REQUIRE_CP,iRequireCP/100);
					s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_SPENT_CP	,iSpentCP);
				}
			}

			if	(iGetCP)
				s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_GET_CP		,max(iGetCP/100,1));
		}

		if	(lpSkill->m_wSpentHPPercentageBasedBloodWing)
			s_strTooltip.Add("- %s <c:LTYELLOW>%s%d.%.2d%s<n>\n",dMSG_SPENT_HP,dMSG_LIGHT_DAMAGE_POINT,lpSkill->m_wSpentHPPercentageBasedBloodWing/100,lpSkill->m_wSpentHPPercentageBasedBloodWing%100,dMSG_PERCENTAGE);

		if	(lpSkill->m_bf3MaxPetCount)
			s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>%s\n",dMSG_MAX_TAME_MONSTER_COUNT,lpSkill->m_bf3MaxPetCount,dMSG_PET_NUMBERS);


		int	iCoolTime	=	g_hero.getCoolTime(this);

		if	(iCoolTime)
			s_strTooltip.Add(dMSG_REMAIN_COOL_TIME_FOR_USE_SKILL_FORM,iCoolTime);

		if	(lpSkill->m_wType	==	eSKILL_TYPE_THROW_ITEM)
		{
			if	(lpSkill->isRequirePotion())
			{
				for	(int i=0;i<lpSkill->m_aValue[2];i++)
				{
					int	iIndex	=	3+i*2;

					if	(m_wLevel	>=	lpSkill->m_aValue[iIndex])
					{
						char	*lpstrItemName	=	g_aBasicItem[lpSkill->m_aValue[iIndex+1]].m_strName;

						s_strTooltip.Add(dMSG_USE_ABLE_POTION_FORM,lpstrItemName);
					}
				}
			}
		}

		{
			int	iHealPoint					=	getHealPoint(_isPetResurrection , _iLevel)/100;
			int	iHealPercentageBasedRemainHP=	getHealPercentageBasedRemainHP();
			int	iRestoreExpPercentage		=	getRestoreExpPercentage();

			if	(iHealPoint)
				s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_HEAL_POINT,iHealPoint);
			if	(iHealPercentageBasedRemainHP)
				s_strTooltip.Add(dMSG_HEAL_PERCENTAGE_BASED_REMAIN_HP_FORM,iHealPercentageBasedRemainHP);

			if	(iRestoreExpPercentage)
			{
				if	(lpSkill->isCastOnDeathPetSkill())
					s_strTooltip.Add(dMSG_RESTORE_PET_EXP_PERCENTAGE_FORM,iRestoreExpPercentage);
				else
					s_strTooltip.Add(dMSG_DECREASE_DEATH_PENELTY_TIME_FORM,iRestoreExpPercentage);
			}

			int	iDodgeChance			=	getDodgeChance();
			int	iCorrectBlockingChance	=	getCorrectBlockingChance();
			int	iDefensivePower			=	getDefensivePower();

			if	(iDodgeChance)
				s_strTooltip.Add("- %s <c:LTYELLOW>%d%s<n>\n",dMSG_INCREASE_DODGE_CHANCE,iDodgeChance,dMSG_PERCENTAGE);
			if	(iCorrectBlockingChance)
				s_strTooltip.Add("- %s <c:LTYELLOW>%d.%d%s<n>\n",dMSG_INCREASE_BLOCKING_CHANCE,iCorrectBlockingChance/10,iCorrectBlockingChance%10,dMSG_PERCENTAGE);
			if	(iDefensivePower)
				s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_INCREASE_DEFENSIVE_POWER,iDefensivePower/100);

			if	(lpSkill->isNotInterrupt())
				s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_IGNORE_ENEMY_ATTACK);

			if	(lpSkill->m_bf1IsInstanceHeal)
				s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_INSTANCE_HEAL);

			if	(lpSkill->m_bf1IsRightPunchSkill)
				s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_RIGHT_PUNCH_SKILL);
			if	(lpSkill->m_bf1IsLeftPunchSkill)
				s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_LEFT_PUNCH_SKILL);
			if	(lpSkill->m_bf1IsRightKickSkill)
				s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_RIGHT_KICK_SKILL);
			if	(lpSkill->m_bf1IsLeftKickSkill)
				s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_LEFT_KICK_SKILL);
			if	(lpSkill->m_bf1IsDownKickSkill)
				s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_DOWN_KICK_SKILL);

			if	(lpSkill->m_wWADDamageMethod)
			{
				s_strTooltip.Add(dMSG_GRADE_DAMAGE_FORM,lpSkill->m_wWADMinDamage,lpSkill->m_wWADMaxDamage);

				switch(lpSkill->m_wWADDamageMethod)
				{
					case	eWAD_METHOD_CASTER			:
						s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_WAD_CASTER);
						break;
					case	eWAD_SHOOT_RANGE			:
						s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_WAD_SHOOT_RANGE);
						break;
					case	eWAD_MAX_EXPLOSION_RANGE	:
						s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_WAD_MAX_EXPLOSION_RANGE);
						break;
					case	eWAD_EXPLOSION_RANGE		:
						s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_WAD_EXPLOSION_RANGE);
						break;
				}
			}

			if	(bIsMiniPetSkill)
			{
				switch(lpSkill->m_bf5MiniPetSkillType)
				{
					case	nsMiniPetST::LordPowerUp	:
					{
						CSkillExtraEffect	*lpEE	=	&lpSkill->m_aExtraEffect[0];
						int					iEffect	=	lpEE->m_wEffect;

						if	(iEffect	==	eSKILL_EE_INCREASE_MAGIC_ITEM_DROP_CHANCE)
						{
							int	iValue		=	(lpEE->m_aiValue[0]+lpEE->m_aiValue[1]*m_wLevel)/100;

							s_strTooltip.Add(dMSG_MSTT_IncreaseDropItemChance_FORM,iValue);
						}
						if	(iEffect	==	eSKILL_EE_INCREASE_GOLD_DROP_CHANCE)
						{
							int	iValue		=	(lpEE->m_aiValue[0]+lpEE->m_aiValue[1]*m_wLevel)/100;

							s_strTooltip.Add(dMSG_MSTT_IncreaseDropGoldChance_FORM,iValue);
						}

						break;
					}

					case	nsMiniPetST::RecoverUseCP	:
					{
						int	iValue		=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel)/100;
						int	iActiveChance=	getActiveChance();

						s_strTooltip.Add(dMSG_MSTT_RECOVER_USE_CP_FORM,iActiveChance/100,iValue);
						break;
					}
					case	nsMiniPetST::UseChargePotion:
					{
						int	iValue		=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel)/100;
						int	iActiveChance=	getActiveChance();

						s_strTooltip.Add(dMSG_MSTT_USE_CHARGE_PORION_FORM,iActiveChance/100,iValue);
						break;
					}
					case	nsMiniPetST::HealBonusByHealSkill:
					{
						int	iValue		=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel)/100;
						int	iActiveChance=	getActiveChance();

						s_strTooltip.Add(dMSG_MSTT_HealBonusByHealSkill_FORM,iActiveChance/100,iValue);
						break;
					}
					case	nsMiniPetST::UseHealPotion	:
					{
						int	iValue		=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel)/100;
						int	iActiveChance=	getActiveChance();

						s_strTooltip.Add(dMSG_MSTT_UseHealPotion_FORM,iActiveChance/100,iValue);
						break;
					}
					case	nsMiniPetST::IgnoreTargetMagicResistanceUnderBossBattle	:
					{
						int	iTarget	=	896+lpSkill->m_aValue[2];
						int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel)/100;

						if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken100)
							iValue += (2 * lpSkill->m_wAwakenSkillBonusDamage + 2 * lpSkill->m_wAwakenSkillBonusDamagePerLevel * lpMiniPet->m_bf7Level)/100;
						else if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken50)
							iValue += (lpSkill->m_wAwakenSkillBonusDamage + lpSkill->m_wAwakenSkillBonusDamagePerLevel * lpMiniPet->m_bf7Level)/100;

						if	(lpSkill->m_aValue[2]	==	5)
							s_strTooltip.Add(dMSG_MSTT_IgnoreTargetMagicResistanceUnderAllBossBattle_FORM,iValue);
						else
							s_strTooltip.Add(dMSG_MSTT_IgnoreTargetMagicResistanceUnderBossBattle_FORM,g_pstrCommonMessage[iTarget],iValue);
						break;
					}
					case	nsMiniPetST::PowerUpUnderBattleToBoss		:
					{
						int	iTarget	=	896+lpSkill->m_aValue[2];
						int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel)/100;

						if	(lpSkill->m_aValue[2]	==	5)
							s_strTooltip.Add(dMSG_MSTT_PowerUpUnderBattleToAllBoss_FORM,iValue);
						else
							s_strTooltip.Add(dMSG_MSTT_PowerUpUnderBattleToBoss_FORM,g_pstrCommonMessage[iTarget],iValue);
						break;
					}
					case	nsMiniPetST::ActionWhenDamaged				:
					{
						int iMiniPetAwakenType = g_hero.getMinipetAwakenType();
						cMiniPetAbilityInfo	*lpPowerUpAbility	=	lpMiniPet->getPowerUpSkill(m_wSkill);

						int	iActiveChance	=	getActiveChance(iMiniPetAwakenType);

						if(lpPowerUpAbility)
						{
							CSkill*	lpPowerUpSkill = &g_aSkill[lpPowerUpAbility->m_bf11Skill];		
							cAbility Ability;

							Ability.m_wLevel	= lpPowerUpAbility->m_bf4Level;
							Ability.m_wSkill	= lpPowerUpAbility->m_bf11Skill;

							iActiveChance		+=	Ability.getActiveChance(iMiniPetAwakenType);
							iActiveChance		+=	lpPowerUpSkill->getAwakenActiveChance(iMiniPetAwakenType , lpMiniPet->m_bf7Level);
						}
						
// 						int	iHitRange		=	getHitRange(NULL);
// 						iHitRange			+=	lpPowerUpSkill->getAwakenHitRange(iMiniPetAwakenType , lpMiniPet->m_bf7Level);

						s_strTooltip.Add(dMSG_MSTT_ActionWhenDamaged_FORM,iActiveChance/100,iActiveChance%100);
						break;
					}
					case	nsMiniPetST::IncreaeUnlockLevel				:
					{
						int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel)/100;

						s_strTooltip.Add(dMSG_MSTT_IncreaeUnlockLevel_FORM,iValue);
						break;
					}
					case	nsMiniPetST::DecreaseResurrectionPenelty	:
					{
						int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel)/100;

						s_strTooltip.Add(dMSG_MSTT_DecreaseResurrectionPenelty_FORM,iValue);
						break;
					}
					case	nsMiniPetST::Resurrection	:
					{
						int	iActiveChance	=	getActiveChance();

//JBC	ƁE��Ȱ 2E�̴�ƁE��ȭ ����E	08-11-03
// 						if(g_iSeasonVariable >= 1)
// 						{
// 							int iValue3 = lpSkill->m_aValue[3];
// 							int iValue4 = lpSkill->m_aValue[4];
// 							
// 							if(iValue4 == 0)		iValue4 = 1;
// 							
// 							iActiveChance	+=	iValue3 + (g_hero.m_iLevel * 100 ) / iValue4;
// 						}

						s_strTooltip.Add(dMSG_MSTT_Resurrection_FORM,iActiveChance/100);
						break;
					}
					case	nsMiniPetST::AvoidTrap	:
					{
						int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel)/100;

						s_strTooltip.Add(dMSG_MSTT_AvoidTrap_FORM,iValue);
						break;
					}

					case	nsMiniPetST::IncreaseActiveChanceUnderBossBattle	:
					{
						int	iTarget	=	896+lpSkill->m_aValue[2];
						int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel)/100;

						if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken100)
							iValue += (2 * lpSkill->m_wAwakenSkillBonusDamage + 2 * lpSkill->m_wAwakenSkillBonusDamagePerLevel * lpMiniPet->m_bf7Level)/100;
						else if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken50)
							iValue += (lpSkill->m_wAwakenSkillBonusDamage + lpSkill->m_wAwakenSkillBonusDamagePerLevel * lpMiniPet->m_bf7Level)/100;

						if	(lpSkill->m_aValue[2]	==	5)
							s_strTooltip.Add(dMSG_MSTT_IncreaseActiveChanceUnderAllBossBattle_FORM,iValue);
						else
							s_strTooltip.Add(dMSG_MSTT_IncreaseActiveChanceUnderBossBattle_FORM,g_pstrCommonMessage[iTarget],iValue);
						break;
					}
					case	nsMiniPetST::DecreaseChargeCount	:
					{
						int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel)/100;

						s_strTooltip.Add(dMSG_MSTT_DecreaseChargeCount_FORM,g_aSkill[lpSkill->m_aValue[2]].m_strName,iValue);	
						break;
					}
					case	nsMiniPetST::AttackTheObject		:
					{
						int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel)/100;
							
						s_strTooltip.Add(dMSG_MSTT_AttackTheObject_FORM,iValue);
						break;
					}
					case	nsMiniPetST::MiniPetSkillPowerUp	:
					{
						int		iAwakenType		=	g_hero.getMinipetAwakenType();

						int		iActiveChance	=	getActiveChance(iAwakenType);
						iActiveChance			+=	getAwakenActiveChance(iAwakenType , lpMiniPet->m_bf7Level);
						int		iHitRange		=	getHitRange(NULL);
						iHitRange				+=	getAwakenHitRange(iAwakenType , lpMiniPet->m_bf7Level);
						int		iCorrectFire	=	(lpSkill->m_sFireDamagePercentage+lpSkill->m_sFireDamagePercentagePerLevel*m_wLevel)/100;
						int		iCorrectWater	=	(lpSkill->m_sWaterDamagePercentage+lpSkill->m_sWaterDamagePercentagePerLevel*m_wLevel)/100;
						int		iCorrectWind	=	(lpSkill->m_sWindDamagePercentage+lpSkill->m_sWindDamagePercentagePerLevel*m_wLevel)/100;
						int		iCorrectEarth	=	(lpSkill->m_sEarthDamagePercentage+lpSkill->m_sEarthDamagePercentagePerLevel*m_wLevel)/100;
						int		iCorrectLight	=	(lpSkill->m_sLightDamagePercentage+lpSkill->m_sLightDamagePercentagePerLevel*m_wLevel)/100;
						int		iCorrectDark	=	(lpSkill->m_sDarkDamagePercentage+lpSkill->m_sDarkDamagePercentagePerLevel*m_wLevel)/100;
						int		iCorrectAwakenDamage	=	getAwakenBoJoDamage(iAwakenType , lpMiniPet->m_bf7Level);
						int		iCorrectDamage	=	iCorrectFire+iCorrectWater+iCorrectWind+iCorrectEarth+iCorrectLight+iCorrectDark+iCorrectAwakenDamage;

						char	*lpstrTargetSkill=	g_aSkill[lpSkill->m_aValue[0]].m_strName;

						if	(iActiveChance)
							s_strTooltip.Add(dMSG_MSTT_INCREASE_ACTIVE_CHANCE_FORM,lpstrTargetSkill,iActiveChance/100,iActiveChance%100);
						if	(iHitRange)
							s_strTooltip.Add(dMSG_MSTT_INCREASE_HIT_RANGE_FORM,lpstrTargetSkill,iHitRange);
						if	(iCorrectDamage)
							s_strTooltip.Add(dMSG_MSTT_INCREASE_DAMAGE_FORM,lpstrTargetSkill,iCorrectDamage);
						break;
					}
				}
			}

			for (i=0;i<dEXTRA_EFFECT_COUNT;i++)
			{
				CSkillExtraEffect	*lpEffect	=	&lpSkill->m_aExtraEffect[i];

				if	(lpEffect->m_wEffect	==	0xffff)
					break;

				CSkillExtraEffectInfo	skillEE;

				getExtraEffect(i,&skillEE);
				getEE_ToolTip(&s_strTooltip,lpEffect,&skillEE);
			}

			if	(lpSkill->m_sTargetLevelLimitType1	!=	0x7fff)
			{
				int	iLimitLevel	=	max(g_hero.getLevel()+m_wLevel+lpSkill->m_sTargetLevelLimitType1,1);

				s_strTooltip.Add(dMSG_SKILL_LIMIT_LEVEL_FROM,iLimitLevel);
			}
		}

//	���� ���� ��ų
		if	(isAidAttackSkill())
		{
			s_strTooltip.Add("\n<c:LTCYAN><%s><n>\n",dMSG_AID_EFFECT);

			for (int i=0;;i++)
			{
				CSkillExtraEffect	*lpEffect	=	getAdditionalDamageInfo(i);

				if (!lpEffect)
					break;

				makeAidMagicDamageTooltip(lpEffect);
			}
		}

		if	(lpSkill->isAgressiveSkill() || lpSkill->m_wDamageAttribute)
		{
			s_strTooltip.Add("\n");
			s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_ATTACK_POINT_COMMENT);

			makeDamageTooltip(iMinDamage,iMaxDamage,iMinWeaponDamage,iMaxWeaponDamage,FALSE,bIsMonsterSkill+(1-lpSkill->isPureMagicDamageSkill()));	//	��E���E
			if	(lpSkill->m_bf5MiniPetSkillType	==	nsMiniPetST::ActionWhenDamaged)
			{
				int iMiniPetAwakenType = g_hero.getMinipetAwakenType();
				cMiniPetAbilityInfo	*lpPowerUpAbility	=	lpMiniPet->getPowerUpSkill(m_wSkill);
				cAbility	Ability;	

				int	iHitRange	=	getHitRange(NULL);

				if(lpPowerUpAbility)
				{
					Ability.m_wLevel	=	lpPowerUpAbility->m_bf4Level;
					Ability.m_wSkill	=	lpPowerUpAbility->m_bf11Skill;

					CSkill*	lpPowerUpSkill = &g_aSkill[lpPowerUpAbility->m_bf11Skill];	

					iHitRange		+=	Ability.getHitRange(NULL);
					iHitRange		+=	lpPowerUpSkill->getAwakenHitRange(iMiniPetAwakenType , lpMiniPet->m_bf7Level);
				}

				s_strTooltip.Add(dMSG_HIT_RANGE_FORM,iHitRange);
			}

			for (i=0;i<dEXTRA_EFFECT_COUNT;i++)
			{
				CSkillExtraEffect	*lpEffect	=	&lpSkill->m_aExtraEffect[i];

				if	(lpEffect->m_wEffect	==	0xffff)
					break;

				CSkillExtraEffectInfo	skillEE;

				getExtraEffect(i,&skillEE);

				switch(lpEffect->m_wEffect)
				{
					case	eSKILL_EE_PHYSICAL_DAMAGE_BASED_REMAIN_HP	:
					{
						int	iLimit				=	skillEE.m_aiValue[2]/100;
						int	iDamagePercentage	=	(skillEE.m_aiValue[0]+skillEE.m_aiValue[1]*m_wLevel)/100;

						if (iLimit)
							iDamagePercentage	=	min(iDamagePercentage,iLimit);

						s_strTooltip.Add(dMSG_SEE_PHYSICAL_DAMAGE_BASED_REMAIN_HP_FORM,iDamagePercentage);
						break;
					}
				}
			}

			char	*lpstrAttackSpeedWord	=	dMSG_ATTACK_SPEED;

			if	(lpSkill->isAgressiveSkill()	==	FALSE)
				lpstrAttackSpeedWord=	dMSG_CASTING_SPEED;

			if	(bIsMiniPetSkill)
			{
				if	(lpSkill->m_bf5MiniPetSkillType	==	nsMiniPetST::AttackUnderBattleEnemy)
				{
					int iMiniPetAwakenType = g_hero.getMinipetAwakenType();
					cMiniPetAbilityInfo	*lpPowerUpAbility	=	lpMiniPet->getPowerUpSkill(m_wSkill);

					int	iSecond		=	lpSkill->m_sOccurActionPeriod/100;	
					int iAddActiveChance		= getActiveChance(iMiniPetAwakenType); 

					if(lpPowerUpAbility)
					{
						CSkill*	lpPowerUpSkill = &g_aSkill[lpPowerUpAbility->m_bf11Skill];	

						int iPowerupSkillLLevel	=	lpPowerUpAbility->m_bf4Level + g_hero.m_bIncreaseMiniPetSkillLevel;

						if	(lpSkill->m_bf1IsNormalMiniPetSkill)
						{
							iPowerupSkillLLevel	+=	g_hero.m_bIncreaseMiniPetBasicSkillLevel;
							iPowerupSkillLLevel	+=	g_hero.m_bCorrectMiniPetBasicSkillLevel;
						}
						
						iPowerupSkillLLevel	+=	g_hero.m_bIncreaseMiniPetSkillLevel;
						iPowerupSkillLLevel	+=	g_hero.m_bCorrectMiniPetSkillLevel;
						
						iAddActiveChance			+= lpPowerUpSkill->getActiveChance(iMiniPetAwakenType ,iPowerupSkillLLevel);
						iAddActiveChance			+= lpPowerUpSkill->getAwakenActiveChance(iMiniPetAwakenType , lpMiniPet->m_bf7Level);
					}

					s_strTooltip.Add(dMSG_STT_EFFECT_ACTIVE_CHANCE_AND_PERIOD_FORM,iSecond,iAddActiveChance/100,iAddActiveChance%100);
				}
				else
				if	(lpSkill->m_bf5MiniPetSkillType	==	nsMiniPetST::ChargeAndFire)
				{
					int	iChargeCount	=	lpSkill->m_aValue[0];

					cMiniPet	*lpMiniPet	=	&g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet];

					iChargeCount -= lpMiniPet->getDecreaseChargeCount();

					if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken100)
						iChargeCount	-=	lpSkill->m_wAwakenSkillBonusActiveChance * 2;	//�������� �̴�ƁE� �濁E�ߵ� Ȯ��E� ī��⮸� ��E� ȿ���� �翁E߽��ϴ�.
					else if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken50)
						iChargeCount	-=	lpSkill->m_wAwakenSkillBonusActiveChance;

					s_strTooltip.Add(dMSG_MSTT_CHARGE_AND_FIRE_FORM,iChargeCount);

					int	iHitRange	=	getHitRange(NULL);

					if	(iHitRange)
						s_strTooltip.Add(dMSG_HIT_RANGE_FORM,iHitRange);
				}
			}
			else
			if	(lpSkill->m_wCastMethod	!=	eST_REACTION)
			{
	//	���ݼӵ�
				if	(iAttackSpeed	!=	0	&&	lpSkill->m_bf1IsTrap	==	FALSE)
				{
					char strSpeed[100];

					iAttackSpeed	=	iAttackSpeed*100/dSYNC_FPS_IN_SERVER;
					
					if	(lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_VALUE)
						sprintf(strSpeed,dMSG_CONTINUOUS_SKILL_ATTACK_SPEED_FORM,iAttackSpeed/100,iAttackSpeed%100);
					else
						sprintf(strSpeed,dMSG_SKILL_ATTACK_SPEED_FORM,iAttackSpeed/100,iAttackSpeed%100);
					
					s_strTooltip.Add("- %s\n",strSpeed);
				}
				else
				{
					if	(iCorrectAttackSpeed	==	100)
					{
						if	(isEnableJob(dJOB_FIGHTER))
							s_strTooltip.Add("- %s <c:LTYELLOW>%s<n>\n",lpstrAttackSpeedWord,dMSG_BASIC_ATTACK_SPEED);
						else
						{
							if	(lpSkill->m_wType	==	eSKILL_TYPE_GUARDIAN_POST)
							{
								char	strSpeed[100];

								iAttackSpeed	=	(lpSkill->m_wStrikePeriod+lpSkill->m_sStrikePeriodPerLevel*m_wLevel);

								sprintf(strSpeed,dMSG_SKILL_ATTACK_SPEED_FORM,iAttackSpeed/100,iAttackSpeed%100);
								s_strTooltip.Add("- %s\n",strSpeed);
							}
							else
								s_strTooltip.Add("- %s <c:LTYELLOW>%s<n>\n",lpstrAttackSpeedWord,dMSG_WEAPON_ATTACK_SPEED);
						}
					}
					else
					{
						if	(isEnableJob(dJOB_FIGHTER))
							s_strTooltip.Add("- %s <c:LTYELLOW>%sx%d%s<n>\n",lpstrAttackSpeedWord,dMSG_BASIC_ATTACK_SPEED,iCorrectAttackSpeed,dMSG_PERCENTAGE);
						else
							s_strTooltip.Add("- %s <c:LTYELLOW>%sx%d%s<n>\n",lpstrAttackSpeedWord,dMSG_WEAPON_ATTACK_SPEED,iCorrectAttackSpeed,dMSG_PERCENTAGE);
					}
				}
			}
		}

		int	iBuletCount	=	getBulletCount();

		if (iBuletCount)
			s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_ATTACK_COUNT,iBuletCount);

		if (lpSkill->m_sAttackPointPerActorLevel)
		{
			s_strTooltip.Add(dMSG_FORM_ATTACK_POINT_PER_ACTOR_LEVEL,lpSkill->m_sAttackPointPerActorLevel*g_hero.getLevel()/100);
			s_strTooltip.Add("\n");
		}
		if (lpSkill->m_sAddDamagePercent)
		{
			s_strTooltip.Add(dMSG_FORM_ADD_DAMAGE_PERCENT,lpSkill->m_sAddDamagePercent/100);
			s_strTooltip.Add("\n");
		}
		if (lpSkill->bf1IsAbleSkillWhenTransToWeapon)
		{
			s_strTooltip.Add(dMSG_CAN_USE_SKILL_WHEN_TRANS_TO_WEAPON);
			s_strTooltip.Add("\n");
		}
	}

	s_strTooltip.Add("\n");

	BOOL	bIsImpossibleLearnSkill	=	FALSE;

	if	((!bIsMiniPetSkill	&&	iOriginalLevel < 50)	||	(bIsMiniPetSkill	&&	iOriginalLevel	<	c_iMiniPetSkillMaxLevel))	
	{
		s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_SKILL_LEVEL_UP_INFORMATION);

		if	(iOriginalLevel==0)
		{
			for (i=0;i<dMAX_REQUIRE_SKILL_COUNT;i++)
			{
				if	(lpSkill->m_aRequireSkill[i].m_wSkill==	0xffff)
					break;

				s_strTooltip.Add("- %s ",dMSG_REQUIRE_SKILL);

				CSkill	*lpRequireSkill	=	&g_aSkill[lpSkill->m_aRequireSkill[i].m_wSkill];

				if ((!lpRequireSkill->isEnableJob(g_hero.m_wJob) && lpSkill->isEnableJob(g_hero.m_wJob)) || (lpRequireSkill->isEnableJob(g_hero.m_wJob) && !lpSkill->isEnableJob(g_hero.m_wJob)))
				{
					int	iAnotherJob	=	(1-g_hero.m_wJob%2)+g_hero.m_wJob/2*2;

					if (!lpSkill->isEnableJob(g_hero.m_wJob))
						iAnotherJob	=	g_hero.m_wJob;

					s_strTooltip.Add("<c:LTYELLOW>%s<n>:<c:LTYELLOW>%s<n>(<c:LTYELLOW>Lv%d<n>)\n",g_pstrHeroJobName[iAnotherJob],lpRequireSkill->m_strName,lpSkill->m_aRequireSkill[i].m_wLevel);
				}
				else
				{
					s_strTooltip.Add("<c:LTYELLOW>%s<n>(<c:LTYELLOW>Lv%d<n>)\n",lpRequireSkill->m_strName,lpSkill->m_aRequireSkill[i].m_wLevel);
				}

				if (lpSkill->m_aRequireSkill[i].m_wLevel	>=	100)
					bIsImpossibleLearnSkill	=	TRUE;
			}
		}

		cAbility	ability;

		ability.copy(this);
		ability.m_wLevel	-=	_iCorrectLevel;

		s_strTooltip.Add("- %s <c:LTYELLOW>%u<n>\n",dMSG_REMAIN_POINT_FOR_LEVEL_UP,ability.getRemainExpForLevelUp());
		s_strTooltip.Add("\n");
	}

	if (lpSkill->m_bf2RequireSummonBeastGrade)
	{
		s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_OPERATE_SUMMONED_BEAST);

		char	strText[128];

		sprintf(strText,"- <c:LTYELLOW>%s %d %s<n>\n\n",g_pstrSummonBeastName[lpSkill->m_bf2OperateSummonBeast],lpSkill->m_bf2RequireSummonBeastGrade,dMSG_GRADE);
		s_strTooltip.Add(strText);
	}

	if(lpSkill->m_wType == eSKILL_TYPE_PASSIVE)
	{
		s_strTooltip.Add("%s\n",dMSG_PASSIVE_EFFECT );	
		
		int iValue = lpSkill->m_aValue[1] + lpSkill->m_aValue[2] * m_wLevel;
		if(lpSkill->m_aValue[3] != 0)
			iValue = min(iValue, lpSkill->m_aValue[3]);
		char	strText[128] = "";
		switch(lpSkill->m_aValue[0])
		{
		case eSKILL_PEE_INCREASE_CONCENTRATION :
			sprintf(strText, dMSG_INCREASE_CONCENTRATION_FORM, iValue/100);		
			break;

		case eSKILL_PEE_INCREASE_HARD_BLOW:
			sprintf(strText, dMSG_INCREASE_HARD_BLOW_FORM, iValue/100.0);		
			break;

		case eSKILL_PEE_INCREASE_MAX_HP :
			sprintf(strText, dMSG_INCREASE_MAX_HP_FORM, iValue/100);
			break;
		case eSKILL_PEE_INCREASE_STRANGE_STATUS_RESISTANCE :
			sprintf(strText, dMSG_INCREASE_STRANGE_STATUS_RESISTANCE_FORM, iValue/100);
			break;
		case eSKILL_PEE_INCREASE_RANGE :
			sprintf(strText, dMSG_INCREASE_RANGE_FORM, iValue/100);
			break;
		case eSKILL_PEE_INCREASE_MAGIC_RESISTANCE	:
			sprintf(strText, dMSG_INCREASE_MAGIC_RESISTANCE_FORM, iValue/100);
			break;
		case eSKILL_PEE_CORRECT_LIGHT_MIN_DAMAGE	:
			sprintf(strText, dMSG_CORRECT_LIGHT_MIN_DAMAGE_FORM, iValue/100);
			break;
		case eSKILL_PEE_ADD_BIT_CHANCE	:
			sprintf(strText, dMSG_ADD_BIT_CHANCE_FORM, iValue/100);
			break;
		case eSKILL_PEE_ADD_BIT_CHANCE_WHEN_BE_HIT	:
			sprintf(strText, dMSG_ADD_BIT_CHANCE_WHEN_BE_HIT_FORM, iValue/100);
			break;
		case eSKILL_PEE_INCREASE_HIT_RANGE	:
			sprintf(strText, dMSG_INCREASE_HIT_RANGE_FORM, iValue/100);
			break;

		}
		s_strTooltip.Add("%s\n\n",strText);			

	}
	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_COMMENT);
	s_strTooltip.Add("- %s\n\n",lpSkill->m_strComment);

	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_ITEM_POWER_UP_METHOD);
	s_strTooltip.Add("- %s\n",lpSkill->m_strPowerup);

	if (bIsImpossibleLearnSkill								)
		s_strTooltip.Insert("<c:LTRED> !!! %s<n>\n\n",dMSG_NOT_EMBODED_SKILL);
	if (lpSkill->m_bf1IsTestSkill && g_bIsTestServer == FALSE	)
		s_strTooltip.Insert("<c:LTRED> !!! %s<n>\n\n",dMSG_IS_TEST_SKILL);
	if (bIsCanNotCalcInfo									)
		s_strTooltip.Insert("<c:LTRED> !!! %s<n>\n\n",dMSG_IS_NOT_EXIST_SKILL_SUMMONED_BEAST);
	

	g_hero.setWantSummonedBeastData(0xffff);

	return	s_strTooltip.String;
}
