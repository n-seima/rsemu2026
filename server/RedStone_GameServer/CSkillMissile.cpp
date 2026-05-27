#include <math.h>
#include "CActiveSkill.H"
#include "cACTOR.H"
#include "debugCode.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireMissileTypeSkill()
{
	if	(m_lpTarget	==	NULL)	
		return	eSKILL_FIRE_RESULT_FAILED;

	if	(!m_lpCaster	||	m_lpCaster->m_wSerialInField	==	0xffff)
		return	eSKILL_FIRE_RESULT_FAILED;
	
	m_lpTarget->getDamage(m_lpCaster,&m_ability,0,0,&m_hitInfo);

	if	(!m_lpCaster	||	m_lpCaster->m_wSerialInField	==	0xffff)
		return	eSKILL_FIRE_RESULT_FAILED;

	m_lpCaster->m_attackInfo.reset();
	m_lpCaster->m_attackInfo.m_aInfo[0].copy(&m_hitInfo);
	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
void
CActiveSkill::castMissile()
{
	m_wFrameCounter	=	0;						//	바로시작
	resetTailChaserStatusInMissileTypeSkill();
	
	if	(m_ability.isThrowWeapon())
	{
		m_lpCaster->throwWeapon(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,&m_ability,FALSE);
		setTailChaserStatusInMissileTypeSkill(2);
	}

	if	(m_ability.isThrowOtherJobWeapon())
		m_lpCaster->throwWeapon(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,&m_ability,TRUE);

	if	(m_ability.isJobChange())
	{
		m_lpCaster->sendTransformation();
		setTailChaserStatusInMissileTypeSkill(2);
	}
}

//
//	테일 체이서 발사!!?
void
CActiveSkill::fireTailChaserInMissileTypeSkill()
{
	if (!m_lpTarget	||	m_lpTarget->isDeath())
		return;

	int	iDamage		=	m_aValue[0]*100;
	int	iTotalDamage=	0;

	for (int i=0;i<m_aValue[2];i++)
	{
		iDamage		=	iDamage*m_aValue[4]/100;
		iTotalDamage+=	iDamage;
	}

	m_lpTarget->damagedBy(m_lpCaster,NULL,iTotalDamage,0);
}

//
//	테일 체이서를 체크한다.
BOOL
CActiveSkill::checkTailChaserInMissileTypeSkill(int _iDamage)
{
	if	(getTailChaserStatusInMissileTypeSkill()==2)
		return	FALSE;

	if	(!m_lpTarget	||	m_lpTarget->isDeath()	||	m_lpTarget->m_bIsGuildObject)
		return	FALSE;
	if	(!m_lpCaster	||	m_lpCaster->isDeath())
		return	FALSE;

	cAbility	tailChaser;

	if	(_iDamage == 0 || !m_lpCaster->getActiveTailChaser(&tailChaser))
		return FALSE;

	CSkill		*lpSkill		=	tailChaser.getSkill();

	int	iActivateChance	=	tailChaser.getActivateChance();

	if	(!g_bIsInnerTestServer)
		if	(random(100)	>=	iActivateChance)
			return FALSE;

	int	iSpentCP		=	tailChaser.getSpentCP();
	int	iGetpCP			=	tailChaser.getGetCP();
	int	iCount			=	tailChaser.getContinuousShotCount();
	int	iPeriod			=	tailChaser.getContinuousShotPeriod();
	int	iCorrectDamage	=	tailChaser.getSkill()->m_sCorrectAttackPoint/100;
	int	iRemainFrame	=	0;

	if	(lpSkill->m_wSpeed==	0)
		iRemainFrame	=	2;
	else
	{
		int	iRange	=	GetOvalRange(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);
		iRange		=	(int)sqrt(iRange);
		iRemainFrame=	iRange/lpSkill->m_wSpeed;
	}

	setTailChaserStatusInMissileTypeSkill(1);

	m_lpCaster->reduceCP(iSpentCP,lpSkill->m_wSerial);
	m_lpCaster->increaseCP(iGetpCP);

	setActiveTailChaserInMissileTypeSkill(iCount,iPeriod,_iDamage,iRemainFrame,iCorrectDamage);

	m_lpCaster->fireTailChaser(m_lpTarget,tailChaser.m_wSkill, iCount,iPeriod ,_iDamage,iCorrectDamage);

	return	TRUE;
}

//
//	미사일형 업데이트
BOOL
CActiveSkill::updateMissileTypeSkill()
{
	g_debugSign.m_iAuraSkillStep	=	0;
	if (!m_lpTarget)
	{
		g_debugSign.m_iAuraSkillStep	=	-1;
		return	TRUE;	//	음냐.. -_-a
	}
	g_debugSign.m_iAuraSkillStep	=	1;
	if (m_lpTarget->isDeath())
	{
		g_debugSign.m_iAuraSkillStep	=	-1;
		return	TRUE;	//	음냐.. -_-a
	}
	g_debugSign.m_iAuraSkillStep	=	2;
	if (m_lpTarget->m_wSerialInField	==	0xffff)
	{
		g_debugSign.m_iAuraSkillStep	=	-1;
		return	TRUE;	//	음냐.. -_-a
	}

	g_debugSign.m_iAuraSkillStep	=	3;

	if	(getTailChaserStatusInMissileTypeSkill()==1)
	{
		if	(decreaseTailChaserFireTime())
		{
			fireTailChaserInMissileTypeSkill();
			g_debugSign.m_iAuraSkillStep	=	-1;

			return	TRUE;
		}

		return	FALSE;
	}

	g_debugSign.m_iAuraSkillStep	=	4;
	if (m_wFrameCounter	==	0xffff	)
	{
		g_debugSign.m_iAuraSkillStep	=	5;
		if (isActionTime())
		{
			g_debugSign.m_iAuraSkillStep	=	6;
			castMissile();
		}

		g_debugSign.m_iAuraSkillStep	=	-1;
		return	FALSE;	//	아직 쏘기 직전이다.
	}

	if (m_lpCaster)
	{
		g_debugSign.m_iAuraSkillStep	=	7;
		m_lpCaster->strike(m_lpTarget,&m_hitInfo,&m_ability);

		g_debugSign.m_iAuraSkillStep	=	8;
		if	(m_hitInfo.isHit())
		{
			g_debugSign.m_iAuraSkillStep	=	9;
			m_lpCaster->increaseCP(m_ability.getGetCP());

			g_debugSign.m_iAuraSkillStep	=	10;

			BOOL	bIsRequireCheckTailChaser	=	TRUE;
			
			if	(m_lpCaster->m_iZoneSerial	==	m_lpTarget->m_iZoneSerial	&&	m_lpCaster->m_wShakleUpkeepTime)
				bIsRequireCheckTailChaser	=	FALSE;
			if	(m_hitInfo.isDodge())
				bIsRequireCheckTailChaser	=	FALSE;

			if	(bIsRequireCheckTailChaser)
				if	(checkTailChaserInMissileTypeSkill(m_hitInfo.m_dwPhysicalDamage))
				{
					g_debugSign.m_iAuraSkillStep	=	-1;
					return	FALSE;
				}
		}
	}
	else
	{
		g_debugSign.m_iAuraSkillStep	=	11;
		m_lpTarget->damagedBy(NULL,NULL,m_hitInfo.m_dwPhysicalDamage,m_hitInfo.m_dwMagicDamage);
		g_debugSign.m_iAuraSkillStep	=	12;
	}

	g_debugSign.m_iAuraSkillStep	=	-1;

	return	TRUE;
}