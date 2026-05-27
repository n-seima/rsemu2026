#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"

//
//	차지스킬 사용
int
CActiveSkill::fireChargeSkill()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	if(m_lpCaster->m_bIsHideByShadowHideSkill)
	{
		m_lpCaster->m_sInvisivilityTime	= 0;
		mDS.m_aiExValue[0]	=	13;
		m_lpCaster->updateEnchantedMagic();
		m_lpCaster->m_sShadowHideAddCriticalChance	= 0;
		m_lpCaster->m_wShadowHideAddPhysicalPowerPercent	= 0;
	}

	int iTime	=	max(m_ability.getAttackSpeed(),dSYNC_FPS);

	setChargingTime(iTime);
	
	m_dwBookedActionTime	=	g_dwCurrentTime+(iTime*1000/dSYNC_FPS/dREVISE_ACTION_SPEED_FOR_TEST);

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	차지 스킬 업데이트
BOOL
CActiveSkill::updateChargeSkill()
{
	if	(m_lpCaster->m_bIsMoving)
	{
		m_lpCaster->setChargeTime(0);
		/*m_lpCaster->buildPower();*/
		return	TRUE;	//	이동하면 취소
	}

	if	(isActionTime())
	{
		BOOL	bIsGetedCP	=	FALSE;

		if	(m_lpTarget && m_lpSkill->isCastOnSummonBeastSkill() && m_lpTarget->isSummonBeast())
			m_lpTarget->applySkillExtraEffects(m_lpCaster,&m_ability,0);
		else
		{
			if	(m_lpSkill->getExtraEffectCount() && m_lpCaster && m_lpCaster->m_wSerialInField	!= 0xffff)
				m_lpCaster->m_lpField->operateAuraSkill(&m_ability,m_posTarget.x,m_posTarget.y,m_ability.getHitRange(),m_lpCaster);
			if	(m_lpSkill->isBadExtraEffectSkill() && m_lpCaster && m_lpCaster->m_wSerialInField	!= 0xffff)
				m_lpCaster->m_lpField->strikeWhirlWind(m_lpCaster,&m_ability,FALSE);
		}

		if	(m_ability.getGetCP()	&&	m_lpCaster)
		{
			int iCP = m_ability.getGetCP();
			m_lpTarget->correctValueEffectDuelServer(&iCP , m_lpSkill);	//결투 서버에서 CP차징 보정.
			m_lpCaster->increaseCP(iCP);
			m_lpCaster->sendPlayerBasicInformation();
			m_lpCaster->sendChargeCP(m_ability.m_wSkill);
		}

		//m_lpCaster->buildPower();
		return	TRUE;
	}

	return	FALSE;
}