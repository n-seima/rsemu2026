#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"

//
//	머신타입 스킬 사용
int
CActiveSkill::fireMachineTypeSkill()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
BOOL
CActiveSkill::castMachineTypeSkill()
{
	m_wFrameCounter		=	0;

	cItem	*lpWeapon	=	m_lpCaster->getWeapon();

	if (!lpWeapon)	
		return FALSE;

	int	iFPS;
	int	iActionTime	=	m_ability.getAttackSpeed();

	if (iActionTime	==	0)
	{
		int	iCorrectSpeed	=	m_ability.getCorrectAttackSpeed();

		if	(lpWeapon)
			iFPS	=	dSYNC_FPS*lpWeapon->getItemSpeed()/100;
		else
			iFPS	=	dSYNC_FPS;

		iFPS			=	iFPS*100/iCorrectSpeed;
	}

	m_wRange		=	m_lpCaster->getAttackRange(&m_ability);
	m_wFrameCounter	=	0;
	m_wAnm			=	0xffff;

	setMachineTime(m_ability.getUpkeepTime()*dSYNC_FPS);
	setMachineFPS(iFPS);
	m_lpCaster->increaseCP(m_ability.getGetCP());

	if (!m_lpCaster)
		return	FALSE;
	
	if (m_ability.isJobChange())
	{
		m_lpCaster->sendTransformation();
	}

	if (!m_lpCaster)	return	FALSE;

	m_lpCaster->floatMachine(m_wSerial,m_ability.m_wSkill);

	m_lpCaster->m_wRemainActionTime	=	dSYNC_FPS/4/dREVISE_ACTION_SPEED_FOR_TEST;

	return	TRUE;
}	//	CActiveSkill::castMachineType()

//
//	캐스팅
BOOL
CActiveSkill::shootMachineTypeSkill()
{
	cItem	*lpWeapon			=	m_lpCaster->getOtherJobWeapon();
	cItem	*lpCurrentWeapon	=	m_lpCaster->getWeapon();
	cItem	*lpWeaponSlot		=	m_lpCaster->getEquipmentSlot(dEQUIP_WEAPON);

	if (!lpWeapon)
		return FALSE;

	cItem	weapon;

	if	(!lpCurrentWeapon)
		weapon.reset();
	else
		weapon.copy(lpCurrentWeapon);

	m_lpTarget	=	m_lpCaster->m_lpTarget;

	lpWeaponSlot->copy(lpWeapon);

	cBasicItem	*lpBasicItem		=	lpWeapon->getBasicItem(TRUE);

	int	iMinDamage,iMaxDamage;

//	데미지 계산을 위해 임의로 무기 바꿈
	{
		iMinDamage	=	m_lpCaster->m_iWeaponMinDamage;
		iMaxDamage	=	m_lpCaster->m_iWeaponMaxDamage;
		m_lpCaster->m_iWeaponMinDamage	=	m_lpCaster->m_sMinDamage*100+lpBasicItem->m_wMinDamage*100;
		m_lpCaster->m_iWeaponMaxDamage	=	m_lpCaster->m_sMaxDamage*100+lpBasicItem->m_wMaxDamage*100;
	}

	m_lpTarget->getDamage(m_lpCaster,&m_ability,0,0,&m_hitInfo,TRUE,TRUE,FALSE);

//	복구 
	{
		lpWeaponSlot->copy(&weapon);
		m_lpCaster->m_iWeaponMinDamage	=	iMinDamage;
		m_lpCaster->m_iWeaponMaxDamage	=	iMaxDamage;
	}

	m_lpCaster->shootMachineMissile(m_lpTarget->m_wSerialInField,m_ability.m_wSkill,&m_hitInfo);

	m_wFrame	=	g_aEffect[m_lpSkill->m_wMachineImage].getFirstTriggerFrame(0)+2;

	return	TRUE;
}	//	CActiveSkill::castMachineType()

//
//	머신 업데이트
BOOL
CActiveSkill::updateMachineTypeSkill()
{
	if	(!m_lpCaster)
		return	TRUE;
	if	(m_lpCaster->isDeath())
		return	TRUE;

	if	(m_lpCaster->m_wSerialInField	==	0xffff)
		return	TRUE;

	if	(m_wFrameCounter==	0xffff)
	{
		if (isActionTime())
			if (!castMachineTypeSkill())	
				return	TRUE;

		return	FALSE;
	}


//	미사일이 발사되었다.
	if (m_lpTarget)
	{
		if (m_wFrame	==	0)
		{
			m_lpCaster->strike(m_lpTarget,&m_hitInfo,&m_ability);	//	타격 입히기
			m_lpTarget	=	NULL;
		}

		m_wFrame--;
	}

	if	(m_wFrameCounter)
		m_wFrameCounter--;

	if (m_wFrameCounter	==	0)
	{
		cACTOR	*lpTarget	=	m_lpCaster->m_lpTarget;

		if	(lpTarget && lpTarget->m_wSerialInField != 0xffff && !lpTarget->isDeath())
		{	
			int	iCurrentRange=	GetOvalRange(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y);
			int	iShootRange	=	m_wRange*m_wRange;

			if (iCurrentRange<	iShootRange)
			{
				if	(m_lpCaster	&&	lpTarget->m_lpField->isMissileBlockedLine(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
					return	FALSE;

				if	(!shootMachineTypeSkill())
				{
					m_lpCaster->releaseMachine();

					return	TRUE;
				}

				m_wFrameCounter	=	getMachineFPS();
			}
		}
	}

	if (decreaseMachineTime())
	{
		m_lpCaster->releaseMachine();
		return	TRUE;
	}

	return	FALSE;	//	아직 쏘기 직전이다.
}