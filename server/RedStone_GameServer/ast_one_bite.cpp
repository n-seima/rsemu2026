#include	"ast_one_bite.h"
#include	"CActiveSkill.H"
#include	"cACTOR.H"
#include	"cFIELD.H"
#include	"CGame.H"

//
//	폭발형 스킬 사용
int
cAS_OneBite::fire()
{
	if (m_lpCaster)
		m_lpCaster->action(m_lpSkill->m_wAction);
	
	m_wStep			=	eCasting;
	m_wBiteCount	=	0;
	m_wShotCount	=	m_ability.getContinuousShotCount();

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	폭발형 스킬 사용
BOOL
cAS_OneBite::cast()
{
	CHitInfo	hitInfo;

	m_lpTarget->getDamage(m_lpCaster,&m_ability,m_pos.x,m_pos.y,&hitInfo);
	m_lpCaster->strike(m_lpTarget,&hitInfo,&m_ability);

	m_lpTarget->addSendHitDamage(m_lpCaster,hitInfo.m_dwPhysicalDamage,hitInfo.m_dwMagicDamage,hitInfo.m_dwResultField,m_ability.m_wSkill);

	m_lpCaster->recoverHPByAidSkill(m_ability.m_wSkill,m_ability.getHealPoint());

	m_wBiteCount++;

	return	TRUE;
}

//
//	폭발형 스킬 업데이트
BOOL
cAS_OneBite::update()
{
	if	(m_lpTarget	==	NULL	||	m_lpTarget->m_wSerialInField	==	0xffff)
		return	TRUE;

	m_lpCaster->m_wInhaleTime	=	4;

	if	(m_wStep	==	ePullTarget)
	{
		int	iRange		=	GetOvalRange(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);
		int	iDistance	=	m_lpSkill->m_aValue[0];

		if	(m_lpCaster->isTargetInAttackRange(m_lpTarget,iDistance)	==	FALSE)
			m_lpTarget->thrust(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,-iDistance,TRUE);
	}

	if	(!isActionTime())
		return	FALSE;

	if	(m_wStep	==	eCasting)
	{
		m_wStep	=	ePullTarget;

		setCastingTimeByCasterDamageFrame(1);
		
		return	FALSE;
	}
	
	if	(m_wStep	==	ePullTarget)
	{
		m_wStep	=	eCatch;
		
		setCastingTimeByCasterDamageFrame(2);

		cPOINT	pos;

		m_lpCaster->getReleasePos(&pos,m_lpSkill->m_wAction);
		m_lpTarget->setActorPos(pos.x,pos.y,TRUE,TRUE);
		m_lpTarget->sendCurrentPosition();
		m_lpTarget->m_wStunTime	=	dSYNC_FPS*4;

		return	FALSE;
	}

	if	(m_wStep	==	eCatch)
	{
		cast();

		setCastingTimeByCasterDamageFrame(2+m_wBiteCount);

		if	(m_wBiteCount	>=	m_wShotCount)
		{
			if	(m_lpTarget->isMonster())
			{
				m_lpTarget->m_bf1IsInstanceKill	=	TRUE;
				m_lpTarget->reduceHP(m_lpTarget->m_iHP);
			}

			return	TRUE;
		}

		return	FALSE;
	}

	return	FALSE;
}