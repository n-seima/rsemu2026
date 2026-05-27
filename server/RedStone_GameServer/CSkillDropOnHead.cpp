#include "CActiveSkill.H"
#include "cACTOR.H"

//
//	연속 타격 스킬
int
CActiveSkill::fireDropOnHeadTypeSkill()
{
	m_lpTarget->getDamage(m_lpCaster,&m_ability,0,0,&m_hitInfo);
	m_lpCaster->m_attackInfo.m_aInfo[0].copy(&m_hitInfo);
	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
void
CActiveSkill::castDropOnHeadTypeSkill()
{
	if (m_lpSkill->m_wExplosionImage==	0xffff	)	return;

	m_wMaxFrame		=	g_aEffect[m_lpSkill->m_wExplosionImage].getFrameCount(0);
	m_wFPS			=	g_aEffect[m_lpSkill->m_wExplosionImage].getFPS(0);
	m_wFrameCounter	=	0;		//	바로시작
}

//
//	
BOOL
CActiveSkill::updateDropOnHeadTypeSkill()
{
	if (m_wFrameCounter	==	0xffff	)
	{
		if (isActionTime())
			castDropOnHeadTypeSkill();

		return	FALSE;	//	아직 쏘기 직전이다.
	}

//	프레임 갱신
	while(m_wFrameCounter	>=	dSYNC_FPS)
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if (g_aEffect[m_lpSkill->m_wExplosionImage].isTrigger(m_wAnm,m_wFrame))
		{
			if (m_lpCaster && !m_lpCaster->isDeath())
			{
				m_lpCaster->strike(m_lpTarget,&m_hitInfo,&m_ability);
				
				if (m_hitInfo.isHit())	m_lpCaster->increaseCP(m_ability.getGetCP());
			}
			else
				m_lpTarget->damagedBy(NULL,NULL,m_hitInfo.m_dwPhysicalDamage,m_hitInfo.m_dwMagicDamage);

			return	TRUE;
		}
	}

	m_wFrameCounter	+=	m_wFPS;

	return	FALSE;
}