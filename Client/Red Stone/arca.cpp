#include "arca.h"
#include "cArea.h"
#include "cImageData.h"
#include "cMap.h"
#include "cMessage.h"
#include "cSound.h"
#include "cHero.h"
#include "cActor.h"

cArcaManager	g_arcaManager;

cArcaInfo::cArcaInfo()
{
	m_hitEffect.init(4);
}

void
cArcaInfo::reset()
{
	m_wSerial			=	0xffff;
	m_wRemoveTimer		=	0;
	m_wRemainDurability	=	100;
}

void
cArcaInfo::hit(cAbility *_lpAbility,CHitInfo *_lpHitInfo)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	int	x				=	random(10)-5;
	int	y				=	random(10)-5-16;
	int	iPhysicalDamage	=	_lpHitInfo->m_dwPhysicalDamage;
	int	iMagicDamage	=	_lpHitInfo->m_dwMagicDamage;
	int	iDamage			=	_lpHitInfo->getDamage();

	if (iPhysicalDamage	)
		iPhysicalDamage	=	max(iPhysicalDamage/100,1);
	if (iMagicDamage)
		iMagicDamage	=	max(iMagicDamage/100,1);

	int	iHitEffectSerial=	g_hem.addEffectAndInfo(x,y,lpSkill->m_wHitImage,dHIT_INFO_WHITE_NUMBER,iPhysicalDamage,iMagicDamage,0,FALSE);	//	타격효과 추가

	m_hitEffect.addHitEffect(iHitEffectSerial);

	g_esm.playHitSound(lpSkill);

	m_wRemainDurability	=	_lpHitInfo->m_dwResultField;
}

void
cArcaManager::reset()
{
	m_iCount	=	0;

	for (int i=0;i<dMAX_ARCA_COUNT_IN_FIELD;i++)
		m_aArca[i].reset();

}

cArcaInfo*
cArcaManager::getArca(int _iSerial)
{
	for  (int i=0;i<dMAX_ARCA_COUNT_IN_FIELD;i++)
	{
		if (m_aArca[i].m_wSerial	==	_iSerial)
			return	&m_aArca[i];
	}

	return	NULL;
}

void
cArcaManager::operatePopupMenu(int _iArcaIndex)
{
	if (cINPDEV::IsExclusive(eMOUSE_STATE_POPUP_MENU))
		return;

	if (s_pmCommon.isPopuped())
	{
		s_pmCommon.reset();

		return;
	}

	m_wWorkArca	=	_iArcaIndex;

	s_pmCommon.popup(s_posMouse.x,s_posMouse.y);
	s_ttCommon.hide();

	s_pmCommon.addMenu(ePM_ATTACK_ARCA	,dMSG_ATTACK);
	cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);
}

void
cArcaManager::update()
{
	cAreaArcaInfo	*lpArcaArea				=	NULL;
	BOOL			bIsOccurPopupMenuEvent	=	FALSE;

	cAreaInfo		*lpArea	=	g_map.m_area.getFocusedArea();

	if (lpArea && lpArea->isArca() && lpArea->isIn(s_posCursor.x,s_posCursor.y))
	{
		lpArcaArea			=	(cAreaArcaInfo	*)lpArea;
		cArcaInfo	*lpArca	=	getArca(lpArcaArea->m_wLinkArca);

		if (lpArca && s_isClickedRightButton)
		{
			if	(lpArca->m_wRemoveTimer	==	0)
			{
				if	(GetRange(lpArca->m_wX,lpArca->m_wY,g_lpHero->m_pos.x,g_lpHero->m_pos.y)	<=	80*80)
					bIsOccurPopupMenuEvent	=	TRUE;
			}
		}

		if (lpArca && s_isRightButtonPressed && bIsOccurPopupMenuEvent == FALSE)
		{
			if	(lpArca->m_wRemoveTimer	==	0)
			{
				if	(!lpArea->isIn(g_am.m_posFocus.x,g_am.m_posFocus.y))
				{
					CPos	posCenter;

					posCenter.Set(lpArca->m_wX,lpArca->m_wY);

					GetTargetPos(&posCenter,g_lpHero->m_pos.x,g_lpHero->m_pos.y,50);

					g_hero.moveTo(posCenter.x,posCenter.y);
				}
			}
		}
	}

	if (bIsOccurPopupMenuEvent)
		operatePopupMenu(lpArcaArea->m_wLinkArca);

	for (int i=0;i<dMAX_ARCA_COUNT_IN_FIELD;i++)
	{
		if (m_aArca[i].m_wSerial	==	0xffff)
			continue;

		for (int j = 0;j < m_aArca[i].m_hitEffect.m_wCount; j++)	//	타격 효과 갱신
		{
			int	iEffect	=	m_aArca[i].m_hitEffect.m_pHitEffect[j];

			if	(iEffect	==	0xffff)
				continue;

			if	(!g_hem.update(iEffect))
			{
				g_hem.remove(iEffect);
				m_aArca[i].m_hitEffect.m_pHitEffect[j]	=	0xffff;
			}
		}

		if (m_aArca[i].m_wRemoveTimer)
		{
			m_aArca[i].m_wRemoveTimer--;

			if (m_aArca[i].m_wRemoveTimer	==	0)
				removeArca(m_aArca[i].m_wSerial);
		}
	}
}

void
cArcaManager::addInArea(cArcaInfo *_lpInfo)
{
	cAreaInfo		area;
	cAreaArcaInfo	*lpArcaArea	=	(cAreaArcaInfo *)&area;

	lpArcaArea->m_wKind		=	dT_AREA_ARCA;

	lpArcaArea->x1			=	_lpInfo->m_wX-64;
	lpArcaArea->y1			=	_lpInfo->m_wY-64;
	lpArcaArea->x2			=	_lpInfo->m_wX+64;
	lpArcaArea->y2			=	_lpInfo->m_wY+32;
	lpArcaArea->m_wLinkArca	=	_lpInfo->m_wSerial;

	sprintf(lpArcaArea->m_strName,"arca %d",_lpInfo->m_wSerial);

	_lpInfo->m_wLinkArea	=	g_map.m_area.addArea(&area);
}

void
cArcaManager::receiveExistArcaList(cArcaSimpleInfo *_lpList,int _iCount,BOOL _bIsFirst)
{
	if (_bIsFirst)
		reset();

	int		i;

	for (i=0;i<_iCount;i++)
	{
		memcpy(&m_aArca[i],&_lpList[i],sizeof(cArcaSimpleInfo));

		m_aArca[i].m_bf1IsOpen	=	FALSE;

		addInArea(&m_aArca[i]);
	}

	m_iCount	=	0;

	for (i=0;i<dMAX_ARCA_COUNT_IN_FIELD;i++)
	{
		if (m_aArca[i].m_wSerial==	0xffff)
			continue;

		m_iCount++;
	}
}

void
cArcaManager::addArca(cArcaSimpleInfo *_lpInfo)
{
	cArcaInfo	*lpArca	=	getArca(_lpInfo->m_wSerial);

	if	(lpArca)
		removeArca(_lpInfo->m_wSerial);

	lpArca	=	NULL;

	for (int i=0;i<dMAX_ARCA_COUNT_IN_FIELD;i++)
	{
		if (m_aArca[i].m_wSerial	==	0xffff)
		{
			lpArca	=	&m_aArca[i];

			break;
		}
	}

	if	(!lpArca)
		return;

	memcpy(lpArca,_lpInfo,sizeof(cArcaSimpleInfo));

	lpArca->m_bf1IsOpen		=	FALSE;
	lpArca->m_bf1IsArmedTrap=	FALSE;
	lpArca->m_wRemoveTimer	=	0;

	addInArea(lpArca);

	m_iCount++;
}

void
cArcaManager::removeArca(int _iSerial,BOOL _bIsFadeOut)
{
	cArcaInfo	*lpArca	=	getArca(_iSerial);

	if (!lpArca)
		return;

	if (_bIsFadeOut)
	{
		lpArca->m_wRemoveTimer	=	dSYNC_FPS*4;
		return;
	}

	g_map.m_area.removeArea(lpArca->m_wLinkArea);
	lpArca->reset();

	m_iCount--;
}

void
cArcaManager::openArca(int _iSerial)
{
	cArcaInfo	*lpArca	=	getArca(_iSerial);

	if (!lpArca)
		return;

	lpArca->m_bf1IsOpen	=	TRUE;
}

void
cArcaManager::unlokcByAttack(int _iSerial)
{
	cArcaInfo	*lpArca	=	getArca(_iSerial);

	if (!lpArca)
		return;

	lpArca->m_bf1IsOpen	=	TRUE;
}

int
cArcaManager::getViewArcas(cINDEX *_lpIndexes,cRECT *_lpRectViewport)
{
	int	iCount	=	0,iVisibleArcaCount	=	0;

	for (int i=0;i<dMAX_ARCA_COUNT_IN_FIELD	&&	iCount < m_iCount;i++)
	{
		cArcaInfo	*lpArca	=	&m_aArca[i];

		if (lpArca->m_wSerial	==	0xffff	||	!g_hero.isVisibleObject(lpArca->m_bf8SecretLevel))
			continue;

		iCount++;

		if (_lpRectViewport->isIn(lpArca->m_wX,lpArca->m_wY))
		{
			_lpIndexes[iVisibleArcaCount].Index	=	dSORT_OBJECT_BORDER_ARCA+i;
			_lpIndexes[iVisibleArcaCount].Value	=	lpArca->m_wY;
			iVisibleArcaCount++;
		}
	}

	return	iVisibleArcaCount;
}

void
cArcaManager::drawArca(int _iIndex,int _iDX,int _iDY)
{
	cArcaInfo	*lpArca	=	&m_aArca[_iIndex];

	if	(lpArca->m_wSerial	==	0xffff	||	!g_hero.isVisibleObject(lpArca->m_bf8SecretLevel))
		return;

	int	iX		=	GetScaledXPos(lpArca->m_wX);
	int	iY		=	GetScaledYPos(lpArca->m_wY);
	int	iIndex	=	0;

	if	(lpArca->m_bf1IsOpen)
		iIndex	=	1;

	WORD	*lpPlt		=	g_sprArca.getOrgPalette(),awPlt[256],awTempPlt[256];;

	if	(g_map.m_area.m_iFocusedArea	!=	0xffff)
		g_map.m_area.m_iFocusedArea	=	g_map.m_area.m_iFocusedArea;

	if (lpArca->m_bf1IsArmedTrap)
	{
		EffectPalette(dPLT_EFFECT_RED,lpPlt,awTempPlt,190,0);
		memcpy(awPlt,awTempPlt,sizeof(awPlt));
		lpPlt	=	awPlt;
	}

	if	(lpArca->m_wLinkArea	==	g_map.m_area.m_iFocusedArea && lpArca->m_wRemoveTimer == 0)
	{
		int	iValue	=	g_iImageFrameCounter*200/100%100*2;

		if (iValue	>	100)
			iValue	=	100*2-iValue;

		EffectPalette(dPLT_EFFECT_BRIGHT,lpPlt,awTempPlt,100+iValue,0);
		memcpy(awPlt,awTempPlt,sizeof(awPlt));
		lpPlt	=	awPlt;
	}

	int	iAlpha		=	32;

	if (lpArca->m_wRemoveTimer)
		iAlpha		=	lpArca->m_wRemoveTimer/2;

	g_sprArca.setPalette(lpPlt);
	g_sprArca.PutShadow(iX,iY,iIndex,g_iGameScale,g_iGameScale,iAlpha/2);
	g_sprArca.Put(iX,iY,iIndex,g_iGameScale,g_iGameScale,dPUT_ALPHA_BLENDING,iAlpha);
	g_sprArca.restorePalette();

	for (int i=0;i<lpArca->m_hitEffect.m_wCount;i++)
	{
		if (lpArca->m_hitEffect.m_pHitEffect[i] == 0xffff)
			continue;

		g_hem.put(lpArca->m_hitEffect.m_pHitEffect[i],iX,iY);
	}
}

void
cArcaManager::operateAW_DisarmTrap(int _iSerial)
{
	cArcaInfo	*lpArca	=	getArca(_iSerial);

	if (!lpArca)
		return;

	lpArca->m_bf1IsArmedTrap	=	FALSE;
}

void
cArcaManager::operateAW_IsHideArca(int _iSerial)
{
}

void
cArcaManager::operateAW_IsFailedWork(int _iSerial)
{
}

void
cArcaManager::operateAW_UnlockBySkill(int _iSerial,char *_lpstrName)
{
	cArcaInfo	*lpArca		=	getArca(_iSerial);

	if (!lpArca)
		return;

	lpArca->m_bf1IsLocked	=	FALSE;

	AddSystemMessage(LTYELLOW,dMSG_UNLOCK_ARCA_BY_SKILL_FORM,_lpstrName);
}

void
cArcaManager::operateAW_UnlockByKey(int _iSerial,char *_lpstrName,int _iKey)
{
	cArcaInfo	*lpArca		=	getArca(_iSerial);

	if (!lpArca)
		return;

	lpArca->m_bf1IsLocked	=	FALSE;

	AddSystemMessage(WHITE,dMSG_UNLOCK_BY_KEY_FORM,_lpstrName);

	if (STRICMP(g_hero.m_strName,_lpstrName) != 0)
		return;

	cItem		*lpKey			=	g_hero.getItem(_iKey);

	if (!lpKey)
		return;

	lpKey->decreaseCount(1);
}

void
cArcaManager::operateAW_IsTrapArca(int _iSerial)
{
	cArcaInfo	*lpArca	=	getArca(_iSerial);

	if (!lpArca)
		return;

	lpArca->m_bf1IsArmedTrap	=	TRUE;
	m_wWorkArca					=	_iSerial;

	g_msgBox.cPopup("",dMSG_IS_TRAP_ARCA_DO_YOU_WANT_DISARM_THIS_ARCA,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_ARCA_DISARM_TRAP);
}

void
cArcaManager::operateAW_IsLockedArca(int _iSerial)
{
	m_wWorkArca	=	_iSerial;

	g_msgBox.cPopup("",dMSG_IS_TRAP_ARCA_DO_YOU_WANT_UNLOCK_THIS_ARCA,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_ARCA_UNLOCK);
}

BOOL
cArcaManager::isVisible(int _iSerial)
{
	cArcaInfo	*lpArca	=	getArca(_iSerial);

	if (!lpArca)
		return	FALSE;
	
	return	g_hero.isVisibleObject(lpArca->m_bf8SecretLevel);
}
