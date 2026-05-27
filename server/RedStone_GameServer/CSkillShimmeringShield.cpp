#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "cExceptionHandler.H"
#include "debugCode.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireShimmeringShield()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireShimmeringShield()

//
//	캐스팅
BOOL
CActiveSkill::castShimmeringShield()
{
	if (m_lpCaster->m_wCastBlocker	!=	0xffff || m_lpCaster->m_bf10CastWeaponBlocker	!=	dVALID_BLOCKER_CODE )	//	이미 시전했다!!
	{
		m_lpCaster->releaseCastShimmeringShield();

		return TRUE;
	}

	cItem	*lpShield;
	BOOL	bIsShield	=	TRUE;

	if (m_lpSkill->m_bf1IsShimmeringShieldByWeapon)
	{
		lpShield	=	m_lpCaster->getWeapon();
		bIsShield	=	FALSE;
	}
	else
		lpShield	=	m_lpCaster->getShield();

	if (!lpShield)
		return	TRUE;

	if (m_lpTarget->m_wReceiveBlocker	!=	0xffff || m_lpTarget->m_wReceiveWeaponBlocker	!=	0xffff)	
		m_lpTarget->releaseReceiveShimmeringShield();

	int	iShieldShape	=	lpShield->getBasicItem(TRUE)->m_wEquippedShape;
	int	iTime			=	m_lpSkill->m_wUpkeepTime+m_ability.m_wLevel*m_lpSkill->m_wUpkeepTimePerLevel;
	int	iBlockingChance	=	m_lpCaster->getBlockingChance()+m_ability.getCorrectBlockingChance();

	iTime				=	iTime*dSYNC_FPS/100;

	m_wFrameCounter						=	0;

	setShimmeringShieldCaster(m_lpCaster->m_wSerialInField);
	setShimmeringShieldReceiver(m_lpTarget->m_wSerialInField);
	setShimmeringShieldTime(iTime);
	setShimmeringShieldBlockingChance(iBlockingChance);
	setShimmeringShieldShape(iShieldShape);	//	시머링 실드 모양 설정

	int	iGetCP		=	m_ability.getGetCP();

	m_lpCaster->increaseCP(iGetCP);	//	CP 증가
	m_lpCaster->m_lpField->sendSetShimmeringShield(m_lpCaster->m_wSerialInField,m_lpTarget->m_wSerialInField,m_lpCaster->m_iCP,iShieldShape,bIsShield);

	int	iCasterJob	=	m_lpCaster->m_wJob;

	if	(m_ability.isJobChange())
		m_lpCaster->sendTransformation();

	if	(!m_lpCaster || !m_lpTarget)
		return	TRUE;

	if	(m_lpSkill->m_bf1IsShimmeringShieldByWeapon)
	{
		m_lpTarget->m_wReceiveWeaponBlocker	=	m_wSerial;
		m_lpCaster->m_bf10CastWeaponBlocker	=	m_wSerial;
		m_lpTarget->m_bf4BlockerShape		=	iShieldShape;

		if	((iCasterJob%2)	==	0)
		{
			m_lpCaster->m_bf1CastWeaponBlockerJob	=	0;
			m_lpCaster->m_wHideWeapon1Time			=	iTime/dSYNC_FPS+10;
		}
		else
		{
			m_lpCaster->m_bf1CastWeaponBlockerJob	=	1;
			m_lpCaster->m_wHideWeapon2Time			=	iTime/dSYNC_FPS+10;
		}
	}
	else
	{
		m_lpTarget->m_wReceiveBlocker		=	m_wSerial;
		m_lpCaster->m_wCastBlocker			=	m_wSerial;
		m_lpTarget->m_bf4BlockerShape		=	iShieldShape;
	}
	
	return	FALSE;
}

//
//	시머링 실드 업데이트
BOOL
CActiveSkill::updateShimmeringShield()
{
	if (m_wFrameCounter==	0xffff)
	{
		if (isActionTime())
			return	castShimmeringShield();

		return	FALSE;
	}

	if (m_lpCaster->m_wCastBlocker		==	0xffff	&&	m_lpCaster->m_bf10CastWeaponBlocker	==	dVALID_BLOCKER_CODE)
		return	TRUE;
	if (m_lpTarget->m_wReceiveBlocker	==	0xffff	&&	m_lpTarget->m_wReceiveWeaponBlocker	==	0xffff)
		return	TRUE;

	if (decreaseShimmeringShieldTime())
	{
		releaseShimmeringShield();

		return	TRUE;
	}

	return	FALSE;	//	아직 쏘기 직전이다.
}	//	CActiveSkill::updateDropOnHeadTypeSkill()

//
//	시머링 실드 없애기
void
CActiveSkill::releaseShimmeringShield()
{
	if	(m_lpCaster && m_lpTarget && m_lpCaster->m_wSerialInField != 0xffff && m_lpTarget->m_wSerialInField != 0xffff )
	{
		m_lpCaster->m_lpField->sendReleaseShimmeringShield(m_lpCaster->m_wSerialInField,m_lpTarget->m_wSerialInField);

		if	(m_lpSkill->m_bf1IsShimmeringShieldByWeapon)
		{
			m_lpCaster->m_bf10CastWeaponBlocker	=	dVALID_BLOCKER_CODE;
			m_lpTarget->m_wReceiveWeaponBlocker	=	0xffff;

			if	(m_lpCaster->m_bf1CastWeaponBlockerJob	==	0)
				m_lpCaster->m_wHideWeapon1Time	=	0;
			else
				m_lpCaster->m_wHideWeapon2Time	=	0;
		}
		else
		{
			m_lpCaster->m_wCastBlocker		=	0xffff;
			m_lpTarget->m_wReceiveBlocker	=	0xffff;
		}

		mDS.m_aiExValue[0]	=	14;
		m_lpCaster->updateEnchantedMagic();
		m_lpCaster->addSendVerySimpleInfo();
	}
	else
	if	(m_lpCaster	&&	m_lpCaster->m_wSerialInField != 0xffff &&	m_lpCaster->m_lpField)
	{
		m_lpCaster->m_lpField->sendReleaseShimmeringShield(m_lpCaster->m_wSerialInField,0xffff);

		if	(m_lpSkill->m_bf1IsShimmeringShieldByWeapon)
		{
			m_lpCaster->m_bf10CastWeaponBlocker	=	dVALID_BLOCKER_CODE;

			if	(m_lpCaster->m_bf1CastWeaponBlockerJob	==	0)
				m_lpCaster->m_wHideWeapon1Time	=	0;
			else
				m_lpCaster->m_wHideWeapon2Time	=	0;
		}
		else
			m_lpCaster->m_wCastBlocker			=	0xffff;

		mDS.m_aiExValue[0]	=	15;
		m_lpCaster->updateEnchantedMagic();
		m_lpCaster->addSendVerySimpleInfo();
	}
	else
	if	(m_lpTarget	&& m_lpTarget->m_wSerialInField != 0xffff	&&	m_lpTarget->m_lpField)
	{
		m_lpTarget->m_lpField->sendReleaseShimmeringShield(0xffff,m_lpTarget->m_wSerialInField);

		if	(m_lpSkill->m_bf1IsShimmeringShieldByWeapon)
			m_lpCaster->m_wReceiveWeaponBlocker	=	0xffff;
		else
			m_lpTarget->m_wReceiveBlocker		=	0xffff;
	}
	
	if	(m_lpTarget	&& m_lpTarget->m_wSerialInField != 0xffff	&&	m_lpTarget->m_lpField)
	{
		if	(m_lpTarget->m_dwEnchantedMask	&	dAMEM_SHIMMERING_SHIELD)	
			m_lpTarget->m_dwEnchantedMask	-=	dAMEM_SHIMMERING_SHIELD;
		if	(m_lpTarget->m_dwEnchantedMask	&	dAMEM_SPEAR_BLOCKER)	
			m_lpTarget->m_dwEnchantedMask	-=	dAMEM_SPEAR_BLOCKER;

		mDS.m_aiExValue[0]	=	16;
		m_lpTarget->updateEnchantedMagic();
		m_lpTarget->addSendVerySimpleInfo();
	}
}