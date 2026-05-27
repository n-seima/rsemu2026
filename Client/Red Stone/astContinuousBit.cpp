#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"



int
CActiveSkill::fireContinuousBit()
{

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);



	return	eSKILL_FIRE_RESULT_OK;
}


BOOL
CActiveSkill::updateContinuousBit()
{

	
	cAS_ContinuousBit	*lpCB	=	(cAS_ContinuousBit	*)this;
	if(!lpCB)
		return FALSE;

	if (!m_lpCaster->m_isTriggerFrame)
		return FALSE;

	for (int i=0;i<lpCB->m_wTargetCount;++i)
	{
		CHitInfo	hitInfo;
		hitInfo.reset(TRUE);
		CActor		*lpTarget	=	g_am.getTestedActor(lpCB->m_awTargetList[i]);
		
		if	(!lpTarget)
			continue;
		
		int iContinuousShotCount	=	m_lpCaster->m_attackInfo.m_aInfo[i].continuousHitInfo.m_bf6ContinuousShotCount;
		int iCriticalCount			=	m_lpCaster->m_attackInfo.m_aInfo[i].continuousHitInfo.m_bf6CriticalCount;
		int iDoubleCriticalCount	=	m_lpCaster->m_attackInfo.m_aInfo[i].continuousHitInfo.m_bf6DoubleCriticalCount;
		int iHardBlowCount			=	m_lpCaster->m_attackInfo.m_aInfo[i].continuousHitInfo.m_bf6HardBlowCount;
		int iMissCount				=	m_lpCaster->m_attackInfo.m_aInfo[i].continuousHitInfo.m_bf6MissCount;
		
		DWORD	dwPhysicalDamage	=	m_lpCaster->m_attackInfo.m_aInfo[i].m_dwPhysicalDamage;
		DWORD	dwMagicDamage		=	m_lpCaster->m_attackInfo.m_aInfo[i].m_dwMagicDamage;
		
		
		int		iHitCount			=	iContinuousShotCount - iMissCount - iCriticalCount - iDoubleCriticalCount - iHardBlowCount;
		int		iDivCount			=	iCriticalCount * 2 + iDoubleCriticalCount * 4 + iHardBlowCount * 3 + iHitCount;
		DWORD	dwDivPD				=	0;
		DWORD	dwDivMD				=	0;
		
		if	(iDivCount)
			dwDivPD	=	dwPhysicalDamage	/	iDivCount;
		
		if	(iDivCount)
			dwDivMD	=	dwMagicDamage		/	iDivCount;
		
		std::vector<DWORD> vecContinuousList;
		while(iCriticalCount)
		{
			vecContinuousList.push_back(eATTACK_RESULT_CIRITICAL);
			--iCriticalCount;
		}
		while(iDoubleCriticalCount)
		{
			vecContinuousList.push_back(eATTACK_RESULT_LUCKY_DOUBLE_CIRITICAL);
			--iDoubleCriticalCount;
		}
		while(iHardBlowCount)
		{
			vecContinuousList.push_back(eATTACK_RESULT_HARD_BLOW);
			--iHardBlowCount;
		}
		while(iMissCount)
		{
			vecContinuousList.push_back(eATTACK_RESULT_MISS);
			--iMissCount;
		}
		while(iHitCount)
		{
			vecContinuousList.push_back(eATTACK_RESULT_HIT);
			--iHitCount;
		}

		for(int k=iContinuousShotCount;k>1;--k)
		{
			int		iIndex				=	random(k);
			DWORD	dwResult			=	vecContinuousList[iIndex];
			vecContinuousList[iIndex]	=	vecContinuousList[k-1];
			vecContinuousList[k-1]		=	dwResult;
		}
	
		int iContinuousListSize	=	vecContinuousList.size();

		for(int j=0;j<iContinuousListSize;++j)
		{
			if(vecContinuousList[j] != eATTACK_RESULT_MISS)
				hitInfo.setHit();
			switch(vecContinuousList[j])
			{
			case	eATTACK_RESULT_MISS			:
				hitInfo.set(0,0,0);
				--iMissCount;
				break;
			case	eATTACK_RESULT_HIT			:
				hitInfo.m_dwPhysicalDamage		=	dwDivPD;
				hitInfo.m_dwMagicDamage 		=	dwDivMD;

				break;
			case	eATTACK_RESULT_CIRITICAL	:
				hitInfo.m_dwPhysicalDamage		=	dwDivPD * 2;
				hitInfo.m_dwMagicDamage			=	dwDivMD;
				hitInfo.setCritical();
				break;
			case	eATTACK_RESULT_LUCKY_DOUBLE_CIRITICAL	:
				hitInfo.m_dwPhysicalDamage		=	dwDivPD * 4;
				hitInfo.m_dwMagicDamage			=	dwDivMD;			
				hitInfo.setLuckyDoubleCritical();
				break;
			case	eATTACK_RESULT_HARD_BLOW	:
				hitInfo.m_dwPhysicalDamage		=	dwDivPD * 3;
				hitInfo.m_dwMagicDamage			=	dwDivMD;
				hitInfo.setHardBlow();
				break;
			}
			m_lpCaster->strike(lpTarget,&m_ability,&hitInfo,0,FALSE,TRUE);

		}
		g_esm.playActionSound(m_lpSkill);
	}
	

	
	return	TRUE;
}