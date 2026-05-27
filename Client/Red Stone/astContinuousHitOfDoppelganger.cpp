#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

int
CActiveSkill::fireContinuousHitOfDoppelganger()
{

	if	(isTriggerContinuousHitOfDoppelganger())
		m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	
	return	eSKILL_FIRE_RESULT_OK;
}

void
cAS_ContinuousHitOfDoppelganger::cast()
{
	if(m_bShot >= m_bContinuousShotCount)
		return ;

	cAS_ContinuousHitOfDoppelganger	*lpAS	=	(cAS_ContinuousHitOfDoppelganger	*)this;
	if(!lpAS)
		return;
	for (int i=0;i<lpAS->m_bTargetCount;++i)
	{
		CHitInfo	hitInfo;
		CActor		*lpTarget	=	g_am.getTestedActor(lpAS->m_awTargetList[i]);
		
		if	(!lpTarget)
			continue;
		getDamage(&hitInfo,i);
		
		if	(i== 0)
			m_lpCaster->strike(lpTarget,&m_ability,&hitInfo,0,FALSE);
		else
			m_lpCaster->strike(lpTarget,&m_ability,&hitInfo,0,FALSE,FALSE);
	}
	

	g_esm.playActionSound(m_lpSkill);
	++m_bShot;
	
}


BOOL
CActiveSkill::updateContinuousHitOfDoppelganger()
{

	if	(isTriggerContinuousHitOfDoppelganger())
		return	FALSE;

	cAS_ContinuousHitOfDoppelganger	*lpAS	=	(cAS_ContinuousHitOfDoppelganger	*)this;
	if(!lpAS)
		return TRUE;

	if	(m_lpCaster->m_continuousHitOfDoppelgangerActionInfo[0].m_bf1IsTriggerFrame)
	{
		lpAS->cast();
		m_lpCaster->m_continuousHitOfDoppelgangerActionInfo[0].m_bf1IsTriggerFrame = FALSE;
	}

	if(m_lpCaster->m_continuousHitOfDoppelgangerActionInfo[0].m_wMaxFrame-2 <= m_lpCaster->m_continuousHitOfDoppelgangerActionInfo[0].m_wFrame)
	{
		lpAS->initDoppelganger();
	}

	if(lpAS->m_bShot >= lpAS->m_bContinuousShotCount)
		return TRUE;

	return	FALSE;
}




void
cAS_ContinuousHitOfDoppelganger::getDamage(CHitInfo *_lpResult,int _iTarget)
{
	_lpResult->reset(TRUE);

	CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfoCHOD.m_aInfo[_iTarget];

	if	(m_bContinuousShotCount	<=	1)
	{
		memcpy(_lpResult,lpHitInfo,sizeof(CHitInfo));
		return;
	}
	if	(lpHitInfo->getDamage() == 0)
		return;
	
	int		iRemainChanceCount	=	m_bContinuousShotCount-m_bShot;
	int		iHitChanceCount		=	iRemainChanceCount-m_abMissCount[_iTarget];
	BOOL	bIsHit				=	TRUE;
	if	(m_abMissCount[_iTarget])
	{
		if	(iHitChanceCount	==	0)
			bIsHit	=	FALSE;
		else
		if	(iHitChanceCount	<	iRemainChanceCount+1)
		{	
			if	(random(2)	==	0)
				bIsHit	=	FALSE;
		}
	}

	if	(bIsHit	==	FALSE)
	{
		if	(m_abMissCount[_iTarget])
			m_abMissCount[_iTarget]--;

		_lpResult->set(0,0,0);

		return;
	}

	BOOL	bIsCritical				=	FALSE;
	BOOL	bIsDoubleCritical		=	FALSE;
	BOOL	bIsHardBlow				=	FALSE;


	if	(m_abCriticalCount[_iTarget])
	{
		bIsCritical	=	TRUE;

		if	(iHitChanceCount	>	m_abCriticalCount[_iTarget] + m_abDoubleCriticalCount[_iTarget] + m_abHardBlowCount[_iTarget] )
			if (random(2)	==	0)
				bIsCritical	=	FALSE;
	}

	if	(bIsCritical	==	FALSE)
	{

		if	(m_abDoubleCriticalCount[_iTarget])
		{
			bIsDoubleCritical	=	TRUE;

			if	(iHitChanceCount	>	m_abCriticalCount[_iTarget]+m_abDoubleCriticalCount[_iTarget] + m_abHardBlowCount[_iTarget])
				if	(random(2)	==	0)
					bIsDoubleCritical	=	FALSE;
		}
		
		if(bIsDoubleCritical == FALSE)
		{
			if(m_abHardBlowCount[_iTarget])
			{
				bIsHardBlow	=	TRUE;
				
				if	(iHitChanceCount	>	m_abCriticalCount[_iTarget]+m_abDoubleCriticalCount[_iTarget]+ m_abHardBlowCount[_iTarget])
					if	(random(2)	==	0)
						bIsHardBlow	=	FALSE;
					
			}
		}

	}
	int		iTempHitCount=	m_abHitCount[_iTarget];

	int		iPD			=	0;
	int		iMD			=	0;

	if	(iTempHitCount)
		iPD	=	lpHitInfo->m_dwPhysicalDamage	/	iTempHitCount;

	if	(iTempHitCount)
		iMD	=	lpHitInfo->m_dwMagicDamage		/	iTempHitCount;

	if	(bIsCritical)
		iPD	*=	2;

	if	(bIsDoubleCritical)
		iPD	*=	4;

	if	(bIsHardBlow)
		iPD	*=	3;

	if	(lpHitInfo->m_dwPhysicalDamage)
	{
		iPD				/=	100;
		int	iRandRange	=	iPD*0.1;
		iPD				=	iPD+random(iRandRange*2)-iRandRange;
		iPD				=	max(iPD,1);
		iPD				*=	100;

		iPD				=	min(lpHitInfo->m_dwPhysicalDamage,iPD);
	}

	if	(lpHitInfo->m_dwMagicDamage)
	{
		iMD				/=	100;
		int	iRandRange	=	iMD*0.1;
		iMD				=	iMD+random(iRandRange*2)-iRandRange;
		iMD				=	max(iMD,1);
		iMD				*=	100;

		iMD				=	min(lpHitInfo->m_dwMagicDamage,iMD);
	}

	_lpResult->m_dwPhysicalDamage	=	iPD;
	_lpResult->m_dwMagicDamage		=	iMD;

	_lpResult->setHit();

	if	(bIsCritical)
	{
		_lpResult->setCritical();
		m_abCriticalCount[_iTarget]--;
	}
	if	(bIsDoubleCritical)
	{
		_lpResult->setLuckyDoubleCritical();
		m_abDoubleCriticalCount[_iTarget]--;
	}
	if	(bIsHardBlow)
	{
		_lpResult->setHardBlow();
		m_abHardBlowCount[_iTarget]--;
	}
}


void
cAS_ContinuousHitOfDoppelganger::init(WORD *_lpwTargetList,int _iTargetCount,int _iContinuousShotCount,int _iCriticalCount,int _iMissCount,int _iDoubleCriticalCount, int _iHardBlowCount)
{
	memcpy(m_awTargetList,_lpwTargetList,sizeof(WORD)*_iTargetCount);
	memset(m_abMissCount,0,sizeof(m_abMissCount));
	memset(m_abCriticalCount,0,sizeof(m_abCriticalCount));
	memset(m_abDoubleCriticalCount,0,sizeof(m_abDoubleCriticalCount));
	memset(m_abHardBlowCount,0,sizeof(m_abHardBlowCount));
	
	m_bTargetCount			=	_iTargetCount;
	m_bContinuousShotCount	=	max(_iContinuousShotCount,1);
	m_bShot					=	0;

	int iCountRange				=	_iTargetCount;
	int iContinuousShotCount	=	_iContinuousShotCount;
	int iCiriticalCount			=	_iCriticalCount;
	int	iMissCount				=	_iMissCount;
	int	iDoubleCriticalCount	=	_iDoubleCriticalCount;
	int	iHardBlowCount			=	_iHardBlowCount;

	m_lpCaster->m_attackInfoCHOD.reset();
	memcpy(&m_lpCaster->m_attackInfoCHOD,&m_lpCaster->m_attackInfo, sizeof(cATTACK_INFO));

	if	(m_bContinuousShotCount	<=	1)
		return;
	
	int	i;

	std::vector<int> vecTarget;

	for (i=0;i<_iTargetCount;i++)
	{
		m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwResultField	=	0;
		vecTarget.push_back(i);
	}

	while(iMissCount > 0)
	{
		if(iCountRange == 0)
			break;

		int		iAttackInfoIndex	= random(iCountRange);
		int		iIndex				= vecTarget[iAttackInfoIndex];
		
		CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfoCHOD.m_aInfo[iIndex];



		if(lpHitInfo->getDamage() == 0)
		{
			iMissCount			-=	m_bContinuousShotCount;
			m_abMissCount[iIndex]	=	m_bContinuousShotCount;

			--iCountRange;
			vecTarget[iAttackInfoIndex] = vecTarget[iCountRange];
			vecTarget[iCountRange]		= iIndex;

			continue;
		}

		--iMissCount;
		++m_abMissCount[iIndex];

		if(m_abMissCount[iIndex] >= m_bContinuousShotCount)
		{
			--iCountRange;
			vecTarget[iAttackInfoIndex] = vecTarget[iCountRange];
			vecTarget[iCountRange]		= iIndex;
		}

	}

	iCountRange = _iTargetCount;
	
	while(iCiriticalCount)
	{
		
		if(iCountRange <= 0)
			break;

		int		iAttackInfoIndex	= random(iCountRange);
		int		iIndex				= vecTarget[iAttackInfoIndex];
		int		iShotCount			= m_abMissCount[iIndex] + m_abCriticalCount[iIndex] ;
	

		if(iShotCount < m_bContinuousShotCount)
		{
			--iCiriticalCount;
			++m_abCriticalCount[iIndex];
		}
		else
		{
			--iCountRange;
			vecTarget[iAttackInfoIndex] = vecTarget[iCountRange];
			vecTarget[iCountRange]		= iIndex;
		}

	}


	iCountRange = _iTargetCount;
	
	while(iDoubleCriticalCount)
	{
		
		if(iCountRange <= 0)
			break;

		int		iAttackInfoIndex	= random(iCountRange);
		int		iIndex				= vecTarget[iAttackInfoIndex];
		int		iShotCount			= m_abMissCount[iIndex] + m_abCriticalCount[iIndex] + m_abDoubleCriticalCount[iIndex] ;

		if( iShotCount < m_bContinuousShotCount)
		{
			--iDoubleCriticalCount;
			++m_abDoubleCriticalCount[iIndex];
		}
		else
		{
			--iCountRange;
			vecTarget[iAttackInfoIndex] = vecTarget[iCountRange];
			vecTarget[iCountRange]		= iIndex;
		}

	}	


	iCountRange = _iTargetCount;
	
	while(iHardBlowCount)
	{
		
		if(iCountRange <= 0)
			break;

		int		iAttackInfoIndex	= random(iCountRange);
		int		iIndex				= vecTarget[iAttackInfoIndex];
		int		iShotCount			= m_abMissCount[iIndex] + m_abCriticalCount[iIndex] + m_abDoubleCriticalCount[iIndex] + m_abHardBlowCount[iIndex];
		
		

		if( iShotCount < m_bContinuousShotCount)
		{
			--iHardBlowCount;
			++m_abHardBlowCount[iIndex];
		}
		else
		{
			--iCountRange;
			vecTarget[iAttackInfoIndex] = vecTarget[iCountRange];
			vecTarget[iCountRange]		= iIndex;
		}

	}	
	for (int j=0;j<_iTargetCount;++j)
	{
		m_abHitCount[j] = _iContinuousShotCount + m_abMissCount[j];
	}
	vecTarget.clear();

	initDoppelganger();
}

void
cAS_ContinuousHitOfDoppelganger::initDoppelganger()
{

	m_lpCaster->m_wDoppelgangerCount	=	m_bTargetCount;

	int iIndex = 0;
	for (int i=0;i<m_bTargetCount;i++)
	{
		m_lpCaster->m_wContinuousHitOfDoppelgangerAction[i]	=	m_lpSkill->m_wAction2+random(3);
		int iFrameCount = m_lpCaster->getFrameCount(m_lpCaster->m_wContinuousHitOfDoppelgangerAction[i]	);
		int iFPS = 100/m_lpSkill->m_aValue[0] * iFrameCount ;
		iFPS = min(iFPS ,  dSYNC_FPS);
		m_lpCaster->m_continuousHitOfDoppelgangerActionInfo[i].init(m_lpCaster,m_lpCaster->m_wContinuousHitOfDoppelgangerAction[i]	,m_lpSkill->m_bf5BunshineAlphaDepth);
		m_lpCaster->m_continuousHitOfDoppelgangerActionInfo[i].m_wFPS = iFPS;
		
		CActor	*lpTarget		=	g_am.getTestedActor(m_awTargetList[i]);
		
		if (!lpTarget || lpTarget->isDeath())
		{
			g_am.addObscurityActor(m_awTargetList[i]);
			continue;
		}
		
		CPos	pos;
		int		iAngle	=	random(360);
		int		iRange	=	lpTarget->getBodySize()+m_lpCaster->getBodySize()+40;
		
		pos.x	=	lpTarget->m_pos.x;
		pos.y	=	lpTarget->m_pos.y;
		
		GetOvalAnglePos(&pos,iAngle,iRange);
		
		m_lpCaster->m_aAfterImageList[i].m_pos.x		=	pos.x;
		m_lpCaster->m_aAfterImageList[i].m_pos.y		=	pos.y;
		m_lpCaster->m_aAfterImageList[i].m_bDirect		=	GetDirect((iAngle+180)%360,m_lpCaster->m_iDirectCount);
		
		iIndex++;
	}
}