#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	러쉬타입 스킬 사용
int
cAS_BigBullet::fire()
{
	m_wIsThrow	=	FALSE;
	m_lpBullet	=	m_lpTarget;
	m_wPPS		=	m_lpBullet->m_iPPS;

	m_lpCaster->action(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,m_lpSkill->m_wAction);

	return	eSKILL_FIRE_RESULT_OK;
}

BOOL
cAS_BigBullet::cast()
{
	if	(!aimingNextTarget())
		return	FALSE;

	m_wFrameCounter		=	0;
	m_lpBullet->m_iPPS	=	m_lpSkill->m_wSpeed;
	m_lpBullet->moveTo(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,TRUE,FALSE,TRUE);

	g_esm.playActionSound(m_lpSkill);

	m_lpBullet->afterImageOn(dPUT_AFTER_IMAGE,dAFTER_IMAGE_COUNT,20,1,2);


	return	TRUE;
}

//
BOOL
cAS_BigBullet::aimingNextTarget()
{
	if	(m_wTargetCount	== m_wTargetIndex)
		return	FALSE;

	while(1)
	{
		m_lpTarget	=	getCurrentTarget();

		if	(m_lpTarget)
			break;

		m_wTargetIndex++;

		if	(m_wTargetCount	== m_wTargetIndex)
			return	FALSE;
	}

	int	iRange	=	GetOvalRange(m_lpBullet->m_pos.x,m_lpBullet->m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);

	iRange		=	(int)sqrt(iRange);

	return	TRUE;
}

CActor*
cAS_BigBullet::getCurrentTarget()
{
	return	g_am.getTestedActor(m_aInfo[m_wTargetIndex].wActor);
}

void
cAS_BigBullet::strikeTarget()
{
	CActor	*lpTarget	=	getCurrentTarget();

	if	(!lpTarget)
		return;

	CHitInfo	hitInfo;

	hitInfo.m_dwResultField		=	0;
	hitInfo.m_dwPhysicalDamage	=	m_aInfo[m_wTargetIndex].wPhysicalDamage*100;
	hitInfo.m_dwMagicDamage		=	m_aInfo[m_wTargetIndex].wMagicDamage*100;

	if	(hitInfo.getDamage())
		hitInfo.setHit();

	m_lpCaster->strike(lpTarget,&m_ability,&hitInfo,0,FALSE);
}

BOOL
cAS_BigBullet::update()
{
	if	(m_wIsThrow	==	FALSE)
		return	FALSE;

	m_lpBullet->m_iPPS	=	m_lpSkill->m_wSpeed;
	if	(m_lpBullet->m_iPPS	<	1000)
		m_lpBullet->m_iPPS	=	m_lpBullet->m_iPPS;

	if	(m_lpBullet->m_isMove	==	FALSE)
	{
		strikeTarget();

		m_wTargetIndex++;

		if	(cast()	==	FALSE)
		{
			m_lpBullet->m_iPPS	=	m_wPPS;

			return	TRUE;
		}
	}

	return	FALSE;
}

void
cAS_BigBullet::throwTarget(void *_lpInfo,int _iTargetCount)
{
	CWideAreaDamageInfo	*lpDamageInfo		=	(CWideAreaDamageInfo	*)_lpInfo;

	m_wTargetCount							=	_iTargetCount;
	m_wTargetIndex							=	0;
	m_lpBullet->m_wUseSpecialActionSkill	=	eSAS_BIG_BULLET;

	for	(int i =0;i<_iTargetCount;i++)
	{
		m_aInfo[i].wActor			=	lpDamageInfo->aData[i].wActor;
		m_aInfo[i].wPhysicalDamage	=	lpDamageInfo->aData[i].wPhysicalDamage;
		m_aInfo[i].wMagicDamage		=	lpDamageInfo->aData[i].wMagicDamage;
	}

	m_wIsThrow	=	TRUE;

	cast();
}

void
cAS_BigBullet::put()
{
}