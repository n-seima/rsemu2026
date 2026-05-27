#include "cAREA.H"
#include "CGamePlay.H"
#include "CGuild.H"
#include "CMap.H"
#include "trap.H"
#include "cSound.H"
#include "cHero.H"

BOOL
cAreaInfo::isIn(int x,int y,BOOL _bIsRegardTrap)
{
	if (_bIsRegardTrap && m_wKind	==	dAREA_TRAP)
	{
		cRECT	*lpRect		=	(cRECT	*)&x1;
		int		iRange		=	lpRect->getRange(x,y,TRUE);
		int		iCheckRange	=	dRANGE_OF_DISARM_ABLE_TRAP*dRANGE_OF_DISARM_ABLE_TRAP;

		if (iRange		<=	iCheckRange)
			return	TRUE;

		return	FALSE;
	}

	if (x < x1) return FALSE;
	if (x > x2) return FALSE;
	if (y < y1) return FALSE;
	if (y > y2) return FALSE;

	return TRUE;
}

void
cAREA::reset()
{
	m_iCount			=	0;
	m_wNormalAreaCount	=	0;
	m_wVisibleAreaCount	=	0;
	m_wTouhableAreaCount=	0;
	m_iMoveArea			=	0xffff;
	m_iFocusedArea		=	0xffff;

	m_wNamedAreaCount	=	0;
	m_dwFrameCounter	=	0;
	m_wTrapAreaCount	=	0;
	m_wInstanceFieldGate=	0xffff;

	for	(int i=dMAX_AREA;i;)
	{
		--i;
		m_aArea[i].reset();
	}
}

void
cAREA::addTouchableArea(int _iIndex,BOOL _bIsVisible)
{
	m_aTouchableArea[m_wTouhableAreaCount++]	=	_iIndex;

	if (_bIsVisible)
		addVisibleArea(_iIndex);
}

void
cAREA::addVisibleArea(int _iIndex)
{
	m_aVisibleArea[m_wVisibleAreaCount++]		=	_iIndex;
}

void
cAREA::removeTouchableArea(int _iIndex)
{
	for (int i=0;i<m_wTouhableAreaCount;i++)
	{
		if (m_aTouchableArea[i]	==	_iIndex)
		{
			for (int j=i;j<m_wTouhableAreaCount-1;j++)
				m_aTouchableArea[j]	=	m_aTouchableArea[j+1];

			m_wTouhableAreaCount--;

			return;
		}
	}
}

//
//	끼워넣기-_-?
void
cAREA::insert(int _iIndex,cAreaInfo *_lpArea,BOOL _bIsGuildBattleLobby,char *_lpstrText)
{
	memcpy(&m_aArea[_iIndex],_lpArea,sizeof(cAreaInfo)-4);

	if (_lpstrText)
	{
		m_aArea[_iIndex].m_pstrString	=	new char [strlen(_lpstrText)+1];
		strcpy(m_aArea[_iIndex].m_pstrString,_lpstrText);
	}

	if	(_lpArea->m_wKind == dAREA_PORTAL)
	{
		if	(_lpArea->m_gateShape	==	ePAS_LEAVE_GUILD_HALL)
			addTouchableArea(_iIndex,TRUE);
		else
		if	(_lpArea->m_wMoveGate	!=	0xffff)
			if	(_lpArea->m_gateShape!=	2)	
				addTouchableArea(_iIndex,TRUE);
	}

	if	(_lpArea->m_wKind == dAREA_DOOR)
		addTouchableArea(_iIndex,FALSE);

	if	(_lpArea->m_wKind == dAREA_EVENT_AREA	||	_lpArea->m_wKind == dT_AREA_ARCA	||	_lpArea->m_wKind == dT_AREA_GUILD_STRUCTURE)
		addTouchableArea(_iIndex,FALSE);

	memcpy(m_aArea[_iIndex].m_aBuffer,_lpArea->m_aBuffer,sizeof(m_aArea[_iIndex].m_aBuffer));

	if	(_bIsGuildBattleLobby && _lpArea->m_wKind == dAREA_GUILD_BATTLE)
		addVisibleArea(_iIndex);

	if	(_lpArea->m_wKind == dAREA_NAMED_AREA)
		addNamedArea(_iIndex);

	if	(_lpArea->m_wKind == dAREA_TRAP	)
	{
		cAreaTrapInfo	*lpTrapArea	=	(cAreaTrapInfo	*)&m_aArea[_iIndex];

		lpTrapArea->m_wIsActive		=	FALSE;

		addTrapArea(_iIndex);
		addTouchableArea(_iIndex,FALSE);
	}

	m_aArea[_iIndex].m_wSerial	=	_iIndex;
}

void
cAREA::update()
{
	for (int i=0;i<m_wTouhableAreaCount;i++)
	{
		int	iArea	=	m_aTouchableArea[i];

		if	(m_aArea[iArea].m_wKind	==	dAREA_DOOR)
		{
			cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)&m_aArea[iArea];

			if	(lpDoor->m_wLinkSecretDungeon	&&	lpDoor->m_wLinkSecretDungeon	!=	0xffff)
			{
				if	(iArea	==	m_wInstanceFieldGate)
					lpDoor->updateForSecretDungeonGate(TRUE);
				else
				if	(g_hero.getSecretDungeonKey(lpDoor->m_wLinkSecretDungeon))
					lpDoor->updateForSecretDungeonGate(TRUE);
				else
					lpDoor->updateForSecretDungeonGate(FALSE);
			}
			else
				lpDoor->update();
		}
	}
}

//
//	포커스 영역 구하기
int
cAREA::getFocusArea(int _iX,int _iY,BOOL _bIsFindFocusArea,BOOL _bIsIgnoreSpecialArea)
{
	if	(m_iMoveArea	!=	0xffff)
		return	m_iFocusedArea;

	int		i;

	m_iFocusedArea	=	0xffff;

	if (_bIsFindFocusArea)
		for (i=0;i<m_wTouhableAreaCount;i++)
		{
			int	iArea	=	m_aTouchableArea[i];

			if	(m_aArea[iArea].m_bf1IsTestPortal	&&	m_aArea[iArea].m_bf15LinkObject	==	0x7000	&&	g_bIsTestServer	==	FALSE)
				continue;

			if	(_bIsIgnoreSpecialArea)
				if	(m_aArea[iArea].m_wKind	==	dT_AREA_ARCA	||	m_aArea[iArea].m_wKind	==	dAREA_TRAP)
					continue;

			if	(m_aArea[iArea].m_pstrString)
			{
				int	iLinkField	=	atoi(m_aArea[iArea].m_pstrString+1);

				if	(g_map.isCloseMap(iLinkField))
					continue;
			}

			if	(m_aArea[iArea].isIn(_iX,_iY))
				m_iFocusedArea	=	iArea;
		}

	return	m_iFocusedArea;
}

//
//	출구 그리기
void
cAREA::drawGateWay(int _iDx,int _iDy,int _iFrameCounter,BOOL _bIsAddPaint)
{
	int		iArea,i,iGateAnm;
	CPos	pos;

	for (i=0;i<m_wVisibleAreaCount;i++)
	{
		iArea		=	m_aVisibleArea[i];

		m_aArea[iArea].getCenter(&pos);

		pos.x		=	GetScaledXPos(pos.x);
		pos.y		=	GetScaledYPos(pos.y);
		
		if (m_aArea[iArea].m_pstrString)
		{
			int	iLinkField	=	atoi(m_aArea[iArea].m_pstrString+1);

			if	(g_map.isCloseMap(iLinkField))
				continue;
		}

		if (m_aArea[iArea].m_wKind	==	dAREA_GUILD_BATTLE)
		{
			g_anmGuildBattlePortal.PutReg(pos.x,pos.y,0,0,0,g_iGameScale,g_iGameScale);

			int		j,iValue	=	(CGamePlay::s_iFrameCounter+pos.x+_iDx+pos.y+_iDy)*3;

			for (j=0;j<20;j++)
			{
				int		iAngle	=	(iValue*4+360-j*12)%360;
				CPos	posLight;

				posLight	=	pos;

				GetOvalAnglePos(&posLight,iAngle,90);

				g_anmGuildBattlePortal.PutReg(posLight.x,posLight.y,j/4+1,0,0,g_iGameScale,g_iGameScale);
			}

			for (j=0;j<5*8;j++)
			{
				int		iAngle	=	(iValue+360-j*3)%360;
				CPos	posLight;

				posLight	=	pos;

				GetOvalAnglePos(&posLight,iAngle,dGUILD_BATTLE_LOBBY_WARP_RANGE);

				g_anmGuildBattlePortal.PutReg(posLight.x,posLight.y,j/8+1,0,0,g_iGameScale,g_iGameScale);
			}
			continue;
		}

		int	iDirect		=	m_aArea[iArea].m_gateDirect;

		if	(m_aArea[iArea].m_bf1IsTestPortal	&&	m_aArea[iArea].m_bf15LinkObject	==	0x7000	&&	g_bIsTestServer	==	FALSE)
			continue;

		switch(m_aArea[iArea].m_gateShape)
		{
			case	ePAS_BUILDING_GATE	:
			case	ePAS_ONE_WAY_GATE	:

			{
				iGateAnm=	0;
				break;
			}

			case	ePAS_NORMAL				:
			case	ePAS_LEAVE_GUILD_HALL	:
			case	ePAS_INNER_DUNGEON:	// insu add
			case	ePAS_INNER_TOWER:	// insu add
			case	ePAS_INNER_BUILDING:	// insu add
			{
				iGateAnm=	2;
				break;
			}

			case	ePAS_INNER_PORTAL	:
			{
				iGateAnm=	4;
				break;
			}

			case	ePAS_HIDDEN_INNER_PORTAL	:
			case	ePAS_HIDDEN			:
				continue;
		}

		if	(iArea	==	m_iFocusedArea)
			iGateAnm++;

		int	iFrameCount	=	g_anmGate.GetFrameCount(iGateAnm);
		int	iFPS		=	g_anmGate.GetFPS(iGateAnm);

		if	(g_bIsTerminalPC)
			cDRAW::Fill(_WHITE,pos.x-20,pos.y-20,pos.x+20,pos.y+20);
		else
		{
			if	(_bIsAddPaint)
				g_anmGate.Put(pos.x,pos.y,iGateAnm,iDirect,(_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount,g_iGameScale,g_iGameScale,dPUT_HALF_BLENDING);
			else
				g_anmGate.PutReg(pos.x,pos.y,iGateAnm,iDirect,(_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount,g_iGameScale,g_iGameScale);
		}
	}
}

int
cAREA::getAreaKind(int _iIndex)
{
	if (_iIndex	>=	dMAX_AREA || _iIndex < 0 || m_aArea[_iIndex].m_wSerial	==	0xffff)
		return	-1;

	return	m_aArea[_iIndex].m_wKind;
}

void
cAREA::activeTrap(int _iIndex)
{
	cAreaTrapInfo	*lpTrapArea	=	(cAreaTrapInfo	*)&m_aArea[_iIndex];

	lpTrapArea->m_wIsActive		=	TRUE;
}

void
cAREA::disactiveTrap(int _iIndex)
{
	cAreaTrapInfo	*lpTrapArea	=	(cAreaTrapInfo	*)&m_aArea[_iIndex];

	lpTrapArea->m_wIsActive		=	FALSE;
}

BOOL
cAREA::isTouchableArea(int _iIndex)
{
	for (int i=0;i<m_wTouhableAreaCount;i++)
		if	(m_aTouchableArea[i]	==	_iIndex)
			return	TRUE;

	return	FALSE;
}

BOOL
cAREA::isGate(int _iIndex)
{
	if (_iIndex	>=	dMAX_AREA || _iIndex < 0 || m_aArea[_iIndex].m_wSerial	==	0xffff)
		return	FALSE;

	if (m_aArea[_iIndex].m_wKind	!=	dAREA_PORTAL)
		return	FALSE;

	return	isTouchableArea(_iIndex);
}

void
cAREA::buildInfo()
{
	int	i;

	for (i=0;i<m_wTouhableAreaCount;i++)
	{
		cAreaInfo	*lpArea	=	getTouchableArea(i);

		if (lpArea->m_wKind == dAREA_DOOR)
		{
			cAreaDoorInfo *lpDoor		=	(cAreaDoorInfo *)lpArea;
			g_map.getLinkObjectWithDoor(lpDoor);

			lpDoor->m_hitEffect.init(4);
			lpDoor->m_wRemainDurability	=	100;
		}

		if (lpArea->m_wKind == dAREA_EVENT_AREA)
		{
			cAreaEvent *lpEventArea	=	(cAreaEvent *)lpArea;

			g_map.getLinkObject((cAreaEvent *)lpArea);
		}
	}
}


int
cAREA::addArea(cAreaInfo *_lpArea)
{
	int	iRookieArea	=	0xffff;

	for (int i=0;i<dMAX_AREA;i++)
		if (m_aArea[i].m_wSerial	==	0xffff)
		{
			iRookieArea	=	i;
			break;
		}

	if	(iRookieArea	==	0xffff)
		return	FALSE;

	insert(iRookieArea,_lpArea,FALSE,NULL);

	return	iRookieArea;
}

BOOL
cAREA::removeArea(int _iSerial)
{
	m_aArea[_iSerial].reset();

	removeTouchableArea(_iSerial);

	return	TRUE;
}

void
cAreaDoorInfo::update()
{
	if	(m_bf1IsActive	==	FALSE)
		return;

	g_map.changeDoorStatus(m_wSerial,FALSE,m_bf1IsHide,1-m_bf1IsUnlocked,1-m_bf1IsDisarmed,1-m_bf1IsOpened,m_bf1IsActiveByKarma);
}

void
cAreaDoorInfo::updateForSecretDungeonGate(BOOL _bIsVisible)
{
	if	(_bIsVisible)
	{
		m_bf1IsActive	=	TRUE;
		g_map.changeDoorStatus(m_wSerial,FALSE,FALSE,FALSE,FALSE,FALSE,m_bf1IsActiveByKarma);
		return;
	}

	m_bf1IsActive	=	FALSE;
	g_map.changeDoorStatus(m_wSerial,TRUE,TRUE,FALSE,FALSE,TRUE,m_bf1IsActiveByKarma);
}

void
cAreaDoorInfo::hit(cAbility *_lpAbility,CHitInfo *_lpHitInfo)
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
