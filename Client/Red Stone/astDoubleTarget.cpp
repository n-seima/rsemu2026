#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireDoubleTargetTypeSkill()
{
	return	fireMissileTypeSkill();
}


void
cAS_DoubleTarget::getDamage(CHitInfo *_lpResult,int _iTarget)
{
	_lpResult->reset(TRUE);

	CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[_iTarget];

	if	(m_wContinuousShotCount	<=	1)
	{
		memcpy(_lpResult,lpHitInfo,sizeof(CHitInfo));
		return;
	}

	if	(lpHitInfo->getDamage() == 0)
		return;

	int		iRemainChanceCount			=	m_wContinuousShotCount-m_wShot;
	int		iHitChanceCount				=	m_wContinuousShotCount-m_abMissCount[_iTarget];
	BOOL	bIsHit						=	TRUE;
	BOOL	bIsCritical					=	FALSE;
	BOOL	bIsDoubleCritical			=	FALSE;
	BOOL	bIsHardBlow					=	FALSE;
	int		iIsIncreaseCriticalDamage	=	0;
	int		iNormalHitCount					= iHitChanceCount - m_abCriticalCount[_iTarget] - m_abDoubleCriticalCount[_iTarget] - m_abHardBlowCount[_iTarget];
	int		iDamageCount				=	m_abCriticalCount[_iTarget]*2 + m_abDoubleCriticalCount[_iTarget]*4 + iNormalHitCount + m_abHardBlowCount[_iTarget]*3;


	DWORD dwPhysicalDamage				=	lpHitInfo->m_dwPhysicalDamage;
	DWORD dwMagicDamage					=	lpHitInfo->m_dwMagicDamage;

	
	DWORD dwPhysicalDamageOfOne			=	0;
	DWORD dwMagicDamageOfOne			=	0;	

	if(dwPhysicalDamage && iDamageCount)
	{
		dwPhysicalDamageOfOne			=	dwPhysicalDamage/iDamageCount;
		if(iHitChanceCount == 1)
			dwPhysicalDamageOfOne		=	lpHitInfo->m_dwPhysicalDamage;

		dwPhysicalDamageOfOne			/=	100;

		if(dwPhysicalDamageOfOne > dMAX_PHYSICAL_DAMAGE)
		{
			iIsIncreaseCriticalDamage	=	dwPhysicalDamageOfOne%dMAX_PHYSICAL_DAMAGE;
			dwPhysicalDamageOfOne		=	dMAX_PHYSICAL_DAMAGE;
		}
		else
		{
			int	iRandRange				=	random(dwPhysicalDamageOfOne/10);

			dwPhysicalDamageOfOne		=	dwPhysicalDamageOfOne+random(iRandRange*2)-iRandRange;
			dwPhysicalDamageOfOne		=	max(dwPhysicalDamageOfOne,1);
			dwPhysicalDamageOfOne		*=	100;			
			dwPhysicalDamageOfOne		=	min(lpHitInfo->m_dwPhysicalDamage,dwPhysicalDamageOfOne);
			dwPhysicalDamageOfOne		=	min(dwPhysicalDamageOfOne, dMAX_PHYSICAL_DAMAGE);
		}
	}

	if(dwMagicDamage && iHitChanceCount)
	{
		dwMagicDamageOfOne				=	dwMagicDamage/iHitChanceCount;	

		if(iHitChanceCount == 1)
			dwMagicDamageOfOne			=	lpHitInfo->m_dwMagicDamage;

		dwMagicDamageOfOne				/=	100;
		int	iRandRange					=	random(dwMagicDamageOfOne/10);
		dwMagicDamageOfOne				=	dwMagicDamageOfOne+random(iRandRange*2)-iRandRange;
		dwMagicDamageOfOne				=	max(dwMagicDamageOfOne,1);
		dwMagicDamageOfOne				*=	100;

		dwMagicDamageOfOne				=	min(lpHitInfo->m_dwMagicDamage,dwMagicDamageOfOne);
		dwMagicDamageOfOne				=	min(dwMagicDamageOfOne , dMAX_MAGICAL_DAMAGE);
	}

	while(1)
	{
		if(m_abMissCount[_iTarget] )
		{
			if(iRemainChanceCount <= m_abMissCount[_iTarget] || random(2) )
			{
				bIsHit					= FALSE;
				dwPhysicalDamageOfOne	= 0;
				dwMagicDamageOfOne		= 0;
				break;
			}
		}
		
		if	(m_abCriticalCount[_iTarget])
		{
			if(iRemainChanceCount <= m_abCriticalCount[_iTarget] || random(2) )
			{
				dwPhysicalDamageOfOne = dwPhysicalDamageOfOne * 2 + iIsIncreaseCriticalDamage;
				_lpResult->setCritical();
				--m_abCriticalCount[_iTarget];
				break;
			}
		}

		if	(m_abDoubleCriticalCount[_iTarget])
		{
			if(iRemainChanceCount <= m_abDoubleCriticalCount[_iTarget] || random(2) )
			{
				
				dwPhysicalDamageOfOne = dwPhysicalDamageOfOne * 4 + iIsIncreaseCriticalDamage;
				_lpResult->setLuckyDoubleCritical();
				--m_abDoubleCriticalCount[_iTarget];
				break;
			}
		}

		if	(m_abHardBlowCount[_iTarget])
		{
			if(iRemainChanceCount <= m_abHardBlowCount[_iTarget] || random(2) )
			{
				
				dwPhysicalDamageOfOne = dwPhysicalDamageOfOne * 3;
				_lpResult->setHardBlow();
				--m_abHardBlowCount[_iTarget];
				break;
			}
		}

		break;
	}


	if(dwPhysicalDamageOfOne)
	{
		_lpResult->m_dwPhysicalDamage	=	dwPhysicalDamageOfOne;
	}

	if(dwMagicDamageOfOne)
	{
		_lpResult->m_dwMagicDamage		=	dwMagicDamageOfOne;
	}

	if(dwPhysicalDamageOfOne + dwMagicDamageOfOne == 0 || bIsHit == FALSE)
	{
		_lpResult->set(0,0,0);	
	}
	else
	{	
		_lpResult->setHit();
	}
	
}




void
cAS_DoubleTarget::init(WORD *_lpwTargetList,int _iTargetCount,int _iContinuousShotCount,int _iCriticalCount,int _iMissCount,int _iDoubleCriticalCount, int _iHardBlowCount)
{
	memcpy(m_awTargetList,_lpwTargetList,sizeof(WORD)*_iTargetCount);
	memset(m_abMissCount,0,sizeof(m_abMissCount));
	memset(m_abCriticalCount,0,sizeof(m_abCriticalCount));
	memset(m_abDoubleCriticalCount,0,sizeof(m_abDoubleCriticalCount));
	memset(m_abHardBlowCount,0,sizeof(m_abHardBlowCount));

	m_wTargetCount			=	_iTargetCount;
	m_wContinuousShotCount	=	max(_iContinuousShotCount,1);
	m_wShot					=	0;
	m_wCriticalCount		=	_iCriticalCount;
	m_wMissCount			=	_iMissCount;
	m_wDoubleCriticalCount	=	_iDoubleCriticalCount;
	m_wHardBlowCount		=	_iHardBlowCount;
	if	(m_wContinuousShotCount	<=	1)
		return;

	int	i;

	for (i=0;i<_iTargetCount;i++)
		m_lpCaster->m_attackInfo.m_aInfo[i].m_dwResultField	=	0;

	if	(m_wMissCount)
	{
		int	iMissCount	=	0;

		for (i=0;i<_iTargetCount;i++)
		{
			CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[i];

			if (lpHitInfo->getDamage()	==	0)
			{
				iMissCount			+=	m_wContinuousShotCount;
				m_abMissCount[i]	=	m_wContinuousShotCount;
			}
		}

		for (i=0;i<10000 && iMissCount < m_wMissCount;i++)
		{
			int	iMissTarget	=	random(_iTargetCount);

			if (m_wContinuousShotCount	>	m_abMissCount[iMissTarget])
			{
				m_abMissCount[iMissTarget]++;
				iMissCount++;
			}
		}
	}

	if	(m_wCriticalCount)
	{
		int	iCriticalCount	=	0;

		for (i=0;i<10000 && iCriticalCount < m_wCriticalCount;i++)
		{
			int	iTarget	=	random(_iTargetCount);

			if	(m_wContinuousShotCount-m_abMissCount[iTarget]-m_abCriticalCount[iTarget]	>	0)
			{
				m_abCriticalCount[iTarget]++;
				iCriticalCount++;
			}
		}
	}

	if	(m_wDoubleCriticalCount)
	{
		int	iDoubleCriticalCount	=	0;

		for (i=0;i<10000 && iDoubleCriticalCount < m_wDoubleCriticalCount;i++)
		{
			int	iTarget	=	random(_iTargetCount);

			if	(m_wContinuousShotCount-m_abMissCount[iTarget]-m_abCriticalCount[iTarget]-m_abDoubleCriticalCount[iTarget]	>	0)
			{
				m_abDoubleCriticalCount[iTarget]++;
				iDoubleCriticalCount++;
			}
		}
	}
	if	(m_wHardBlowCount)
	{
		int	iHardBlowCount	=	0;

		for (i=0;i<10000 && iHardBlowCount < m_wHardBlowCount;i++)
		{
			int	iTarget	=	random(_iTargetCount);

			if	(m_wContinuousShotCount-m_abMissCount[iTarget]-m_abCriticalCount[iTarget]-m_abDoubleCriticalCount[iTarget] - m_abHardBlowCount[iTarget]	>	0)
			{
				m_abHardBlowCount[iTarget]++;
				iHardBlowCount++;
			}
		}
	}	
}

BOOL
cAS_DoubleTarget::cast()
{
	if	(m_wShot	>=	m_wContinuousShotCount)
		return	TRUE;

	if	(m_wShot	==	0	&&	m_lpCaster->isHero())
	{
		for (int i=0;i<m_wTargetCount;i++)
		{
			CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[i];

			if	(lpHitInfo->isHit())
			{
				g_hero.increaseCP(m_ability.getGetCP());
				break;
			}
		}
	}

	int	iRange	=	m_wRange+20;

	iRange		*=	iRange;

	int	iCasterArrowHeight	=	m_lpCaster->getHeight()+m_lpCaster->getBodyHeight()*2/3;

	mBeginTLog();
	g_eh.addLog("m_wTargetCount %d",m_wTargetCount);

	for (int i=0;i<m_wTargetCount;i++)
	{
		CActor	*lpTarget	=	g_am.getTestedActor(m_awTargetList[i]);

		if	(!lpTarget || lpTarget->isDeath())
			continue;

		if	(m_lpCaster	==	NULL)
			return	TRUE;

		int	iCurRange	=	GetOvalRange(lpTarget->m_pos.x,lpTarget->m_pos.y,m_lpCaster->m_pos.x,m_lpCaster->m_pos.y);

		if	(iCurRange	>	iRange)
			continue;

		CHitInfo	hitInfo;

		getDamage(&hitInfo,i);

		if	(!lpTarget || lpTarget->isDeath())
			continue;

		g_sm.castSpecialMissile(NULL,lpTarget,m_pos.x,m_pos.y-iCasterArrowHeight,&hitInfo,&m_ability);
	}

	m_wShot++;

	if	(m_wShot	>=	m_wContinuousShotCount)
	{
		if (m_lpSkill->isContinuousAttackSkill())
			m_lpCaster->setLastFrame();

		return	TRUE;
	}

	return	FALSE;
}

//
//	미사일형 업데이트
BOOL
CActiveSkill::updateDoubleTargetTypeSkill()
{
	if	(!m_lpCaster)
		return	TRUE;

	if	(!m_lpSkill)
		return	TRUE;

	if	(m_lpCaster->m_isTriggerFrame)
	{
		cAS_DoubleTarget	*lpAS	=	(cAS_DoubleTarget	*)this;

		return	lpAS->cast();
	}

	return	FALSE;
}