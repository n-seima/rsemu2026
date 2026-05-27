#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	연사
int
CActiveSkill::fireBitGlider()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	업데이트
void
CActiveSkill::castBitGlider()
{
	int			iShotCount	=	getContinuousAttackShotCount();
	int			iArrowGab	=	20;
	int			iDamage		=	0;
	int			iAngle,iCasterArrowHeight,iGliderAngle1,iGliderAngle2,i,iDistance=iArrowGab/2,iHitIndex=0;
	CHitInfo	*lpHitInfo;
	CPos		pos;

	if	(!m_lpCaster	||	!m_lpTarget)
		return;

	iCasterArrowHeight	=	m_lpCaster->getHeight()+m_lpCaster->getBodyHeight()*2/3;
	m_posTarget			=	m_lpTarget->m_pos;
	iAngle				=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	iGliderAngle1		=	(iAngle+90)%360;
	iGliderAngle2		=	(iAngle+360-90)%360;

	g_esm.playActionSound(m_lpSkill);

	if	(iShotCount	%2	==	1)
	{
		iShotCount--;
		lpHitInfo		=	&m_lpCaster->m_attackInfo.m_aInfo[iHitIndex++];
		iDamage			+=	lpHitInfo->getDamage();

		g_sm.castSpecialMissile(NULL,m_lpTarget,m_pos.x,m_pos.y-iCasterArrowHeight,lpHitInfo,&m_ability);

		iDistance		=	iArrowGab;
	}

	mBeginTLog();
	g_eh.addLog("castBitGlider %d",iShotCount);

	for (i=0;i<iShotCount/2;i++)
	{
		pos				=	m_pos;
		GetAnglePos(&pos,iGliderAngle1,iDistance);

		if	(!m_lpCaster	||	!m_lpTarget)
			return;

		lpHitInfo		=	&m_lpCaster->m_attackInfo.m_aInfo[iHitIndex++];
		iDamage			+=	lpHitInfo->getDamage();

		//JBC	더블드로잉등의 스킬을 사용하고 나서 필드 이동하면 멈추는 문제 수정.08-09-29
		//if	(g_sm.castSpecialMissile(NULL,m_lpTarget,pos.x,pos.y-iCasterArrowHeight,lpHitInfo,&m_ability)	==	0xffff)
		if	(g_sm.castSpecialMissile(m_lpCaster,m_lpTarget,pos.x,pos.y-iCasterArrowHeight,lpHitInfo,&m_ability)	==	0xffff)
			break;

		pos				=	m_pos;
		GetAnglePos(&pos,iGliderAngle2,iDistance);
		lpHitInfo		=	&m_lpCaster->m_attackInfo.m_aInfo[iHitIndex++];
		if	(g_sm.castSpecialMissile(NULL,m_lpTarget,pos.x,pos.y-iCasterArrowHeight,lpHitInfo,&m_ability)	==	0xffff)
			break;

		iDistance		+=	iArrowGab;
	}

	if	(!m_lpCaster	||	!m_lpTarget)
		return;

	if	(iDamage	&& m_lpCaster->isHero())
		g_hero.increaseCP(m_ability.getGetCP());
}

//
//	업데이트
BOOL
CActiveSkill::updateBitGlider()
{
	if (m_lpCaster->m_isTriggerFrame)
	{
		castBitGlider();

		return	TRUE;
	}

	return	FALSE;
}

//
//	비트 글라이더 찍어 주기
void
CActiveSkill::putBitGlider()
{
}