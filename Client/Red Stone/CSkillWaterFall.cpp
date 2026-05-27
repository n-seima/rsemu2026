#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	워터폴 사용
int
CActiveSkill::fireWaterFall()
{
	m_lpCaster->action(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpSkill->m_wAction);

	m_wFrameCounter		=	0xffff;
	resetWaterFallUpkeepTime();
	resetWaterFallStrikePeriod();
	resetWaterFallCasting();
	resetWaterFallStrikeTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	워터폴 사용
void
CActiveSkill::castWaterFall()
{
	m_wFrameCounter	=	0;

	if	(m_lpSkill->m_wShakeIntensity)
		g_shaker.add(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wShakeIntensity,m_lpSkill->m_wShakeTime,m_ability.getHitRange(NULL));

	setWaterFallUpkeepTime(m_ability.getUpkeepTime()*dSYNC_FPS);
	setWaterFallCasting();

	int	iHitRange	=	m_ability.getHitRange(NULL);
	int	iHitCount	=	6;

	if	(m_lpSkill->m_aValue[1])
		iHitCount	=	m_lpSkill->m_aValue[1];

	for (int i=0;i<iHitCount;i++)
		strikeWaterFall(360/iHitCount*i+random(20),iHitRange/2+random(iHitRange/2));

	g_esm.playActionSound(m_lpSkill);
}

//
//	워터폴 직격!!
void
CActiveSkill::strikeWaterFall(int _iAngle,int _iDistance)
{
	CPos	pos	=	m_posTarget;

	GetOvalAnglePos(&pos,_iAngle,_iDistance);

	CPos	pos2=	m_posTarget;

	int		iRange=GetOvalRange(pos2.x,pos2.y,pos.x,pos.y);

	iRange		=	sqrt(iRange);

	g_sm.castExplosion(m_lpCaster,pos.x,pos.y,&m_ability);
}

//
//	워터폴 업데이트
BOOL
CActiveSkill::updateWaterFall()
{
	if (!isWaterFallCasting())
	{
		if	(m_wFrameCounter	!=	0xffff)
		{
			if	(decreaseWaterFallCastingTime())	
				return	FALSE;

			castWaterFall();
		}
		else
		if	(m_lpCaster->m_isTriggerFrame)
		{
			if	(m_lpSkill->m_bf1InsInstanceWaterFall)
				castWaterFall();
			else
			{
				CPos		pos;

				m_lpCaster->getReleasePos(&pos);

				g_sm.castCustomMissile(pos.x,pos.y,pos.x,pos.y-400,&m_ability);

				int	iCastingTime	=	m_lpSkill->m_sCastingTime*dSYNC_FPS/1000;

				setWaterFallCastingTime(iCastingTime);

				m_wFrameCounter		=	0;
			}
		}

		return	FALSE;
	}

	if	(decreaseWaterFallUpkeepTime())
		return	TRUE;

	int	iHitRange	=	m_ability.getHitRange(NULL);
	int	iHitPeriod	=	3;
	
	if	(m_lpSkill->m_aValue[2])
		iHitPeriod	=	m_lpSkill->m_aValue[2];

	if	(random(iHitPeriod)	==	0)
		strikeWaterFall(random(360),random(iHitRange));

	return	FALSE;
}