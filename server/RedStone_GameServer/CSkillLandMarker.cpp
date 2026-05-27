#include <math.h>
#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"

//
//	랜드마커 사용
int
CActiveSkill::fireLandMarker()
{
	int	iShotCount	=	m_ability.getContinuousShotCount();

	memset(m_aValue+10,0xff,iShotCount*sizeof(WORD));

	m_lpCaster->action(m_lpSkill->m_wAction);
	m_lpCaster->setActionSpeed(&m_ability);
	m_lpCaster->m_attackInfo.m_ability.copy(&m_ability);

	setContinuousAttackActionTime(m_lpCaster->m_wRemainActionTime*iShotCount);
	setContinuousAttackCount(iShotCount);
	resetContinuousAttackShotCount();
	resetContinuousAttackGetCPStatus();	//	CP 아직 안 얻었다!!
	resetLandMarkerFinish();
	resetLandMarkerGetedCP();

	m_lpCaster->action(m_lpSkill->m_wAction,TRUE);

	m_lpCaster->m_wRemainActionTime	=	getContinuousAttackActionTime()/dREVISE_ACTION_SPEED_FOR_TEST;

	m_wMaxFrame		=	g_aEffect[m_lpSkill->m_wExplosionImage].getFrameCount(0);
	m_wFPS			=	g_aEffect[m_lpSkill->m_wExplosionImage].getFPS(0);
	m_wFrame		=	0;

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireLandMarker()

//
//	랜드 마커 업데이트
BOOL
CActiveSkill::updateLandMarker()
{
	cBODY	*lpExplosionImage	=	&g_aEffect[m_lpSkill->m_wExplosionImage];

	if	(!isLandMarkerFinish())	//	발사 액션?이 아직 안 끝났다.
	{
		if (m_lpCaster->m_wIsActionFinished)
		{
			if	(increaseContinuousAttackShotCount())
			{
				setLandMarkerFinish();

				return	FALSE;
			}

			m_lpCaster->action(m_lpSkill->m_wAction,TRUE);
		}

		if	(m_lpCaster->m_isTriggerFrame)
			m_aValue[getContinuousAttackShotCount()+10]	=	0;
	}

	int	iShotCount			=	getContinuousAttackCount(),i;
	int	iFinishMarkerCount	=	0;
	
	for (i=0;i<iShotCount;i++)
	{
		if	(m_aValue[i+10]	==	0xffff)
			continue;
		if	(m_aValue[i+10]	==	0xfffe)
		{
			iFinishMarkerCount	++;
			continue;
		}

		m_aValue[i+10]			+=	m_wFPS;

		if (m_aValue[i+10]	>=	dSYNC_FPS)
		{
			m_aValue[i+10]		-=	dSYNC_FPS;
			m_aValue[i+30]++;

			int	iFrame		=	m_aValue[i+30];

			if	(iFrame		>=	m_wMaxFrame)
			{
				m_aValue[i+10]	=	0xfffe;
				continue;
			}

			if	(lpExplosionImage->isDamage(0,iFrame))
			{
				BOOL	bIsGetedCP	=	isLandMarkerGetedCP();;
				cPOINT	pos			=	m_pos;

				g_protractor.getTargetPos(&pos,m_posTarget.x,m_posTarget.y,(i+1)*m_lpSkill->getShootingObjective());
				m_lpCaster->m_lpField->strikeExplosionSkill(m_lpCaster,m_lpTarget,pos.x,pos.y,&m_ability,&bIsGetedCP);

				setLandMarkerGetedCP(bIsGetedCP);
			}
		}
	}

	if	(iFinishMarkerCount	==	iShotCount && isLandMarkerFinish())
		return	TRUE;

	return	FALSE;
}