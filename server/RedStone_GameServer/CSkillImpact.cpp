#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CGame.H"
#include <math.h>

//
//	파이어볼 발사
int
CActiveSkill::fireImpactSkill()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	m_aValue[0]	=	0;

	if	(m_lpSkill->m_aValue[0])
		m_lpCaster->m_lpField->addSendEtcWork(m_lpCaster->m_wSerialInField,eEW_CHANGE_SAGE_BOOK_COLOR,m_lpCaster->m_wSerialInField,m_lpSkill->m_aValue[0]);
	
	setCastingTimeByCasterDamageFrame(0);

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	
BOOL
CActiveSkill::updateImpactSkill()
{
	if(!m_lpCaster)
		return FALSE;

	m_lpCaster->setRemainActionTime(4);
	
	if	(isActionTime())
	{
		if	(m_lpCaster && !m_lpCaster->isDeath() && m_lpCaster->m_wSerialInField != 0xffff)
		{
			BOOL	bIsGetedCP	=	m_wIsGetdCP;

			m_lpCaster->m_lpField->strikeExplosionSkill(m_lpCaster,m_lpTarget,m_posTarget.x,m_posTarget.y,&m_ability,&bIsGetedCP);

			m_wIsGetdCP			=	bIsGetedCP;
		}
		else
			return	TRUE;

		if	(m_lpCaster && !m_lpCaster->isDeath() && m_lpCaster->m_wSerialInField != 0xffff && m_lpCaster->m_iAnm	==	m_lpSkill->m_wAction)
		{
			if	(m_lpSkill->isEnableJob(m_lpCaster->m_wJob)	==	FALSE)
				return	TRUE;
			
			if	(m_lpCaster->getBody()	==	NULL)
				return	TRUE;

			mBeginTLog();

			g_eh.addLog("updateImpactSkill %d,%d",m_lpCaster->m_iFrameCount,m_lpCaster->m_iAnm);

			m_aValue[0]++;
			setCastingTimeByCasterDamageFrame(m_aValue[0]);

			if	(m_dwBookedActionTime	==	0xffffffff)
			{
				int	iTR			=	m_lpCaster->isAbleToTransformation(TRUE);

				if	(iTR	==	eTRANSFORMATION_SUCCESS	&&	m_ability.isJobChange())
					m_lpCaster->sendTransformation(eTR_BY_SKILL,m_lpSkill->m_bf1PutTrasAfterAnm);
				return	TRUE;
			}
		}
		else
			return	TRUE;
	}

	return	FALSE;
}