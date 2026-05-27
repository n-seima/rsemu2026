#include "CHero.H"
#include "CActor.H"
#include "CMessage.H"
#include "CPitchmanShop.H"
#include "tdlanguage.h"
#include "banner.h"
#include "trap.h"

//
//	이동
BOOL
CHero::moveTo(int _iX,int _iY)
{
	if	(!checkWaitGetGVGAvatarOrgData())
		return	FALSE;

	if	(m_wWaitMoveAreaResultTime)
		return	FALSE;
	if	(g_lpHero->isBlockToMove())
		return	FALSE;
	if	(g_pitchmanShop.m_wFocusShop	!=	0xffff)
		return	FALSE;
	if	(g_bannerManager.m_iFocusBanner	!=	0xffff)
		return	FALSE;
	if	(!s_isUpdateTimingByUpdatePeriod)
		return	FALSE;
	if	(m_iLastActionTime+dSEND_HERO_ACTION_PERIOD	>	s_iFrameCounter	)
		return	FALSE;
	if	(g_lpHero->isExclusiveAction()	||	g_lpHero->m_enchantedImage.m_bf1IsFeignDeath	||	g_hero.m_wPrincessWeaponOwner	!=	0xffff)
		return	FALSE;

	if	(g_trapManager.m_iTryDisarmTrap	!=	0xffff)
		g_trapManager.m_iTryDisarmTrap	=	g_trapManager.m_iTryDisarmTrap;

	if	(s_aKey[KEY_LSHIFT] && m_isWalking == FALSE && (g_lpHero->m_iAnm	==	dACT_READY	||	g_lpHero->m_iAnm	==	dACT_SITDOWN))
	{
		m_iLastActionTime	=	s_iFrameCounter;

		g_lpHero->setDirect(g_lpHero->getDirect(_iX,_iY,TRUE));

		s_agent.sendStop();

		return	FALSE;
	}

	if	(g_lpHero->m_wHillSkill	!=	0xffff	)
	{
		g_lpHero->warning(dMSG_OUT_OF_SKILL_RANGE_TARGET);
		return	FALSE;
	}

	if	(g_lpHero->m_wBiteTarget	!=	0xffff	)
		g_lpHero->stop();

	if	(g_lpControlActor)
	{
		if	(s_iFrameCounter	<	m_iLastMovedTime+s_iPlayerMovePacketPeriod)
			return	TRUE;

		s_agent.sendMove(_iX,_iY);
		m_iLastMovedTime		=	s_iFrameCounter;
		m_dwLastReportTime		=	timeGetTime();

		return	TRUE;
	}

	if	(m_iWaitMoveResultTime)
	{
		if	(s_iFrameCounter	>=	m_iLastMovedTime+s_iPlayerMovePacketPeriod)
		{
			if	(s_iFrameCounter	>=	m_iLastMovedTime+s_iPlayerMovePacketPeriod*5)
			{
				m_iWaitMoveResultTime	=	0;
				m_isWalking				=	FALSE;
				g_lpHero->stop(m_posLast.x,m_posLast.y);
				sendStop();

				return	FALSE;
			}

			m_isWalking		=	FALSE;
			g_lpHero->stop();
		}

		return	FALSE;
	}

	if	(g_lpHero->isSitdown())
	{
		g_hero.sendSitdown();
		return	TRUE;
	}

	if	(g_lpHero->moveTo(_iX,_iY,TRUE,FALSE))
	{
		m_iDirect	=	g_lpHero->m_iDirect;
		g_lpHero->setStopDirect(m_iDirect);
		m_posDest.x	=	s_posCursor.x;
		m_posDest.y	=	s_posCursor.y;
		m_isWalking	=	TRUE;

		if	(s_iFrameCounter	<	m_iLastMovedTime+s_iPlayerMovePacketPeriod)
		{
			sendReportCurrentPos();

			return	TRUE;
		}

		m_posLast.x				=	g_lpHero->m_pos.x;
		m_posLast.y				=	g_lpHero->m_pos.y;
		m_bf1IsRunning			=	g_lpHero->m_isRunning;
		m_iPPS					=	g_lpHero->m_iPPS;
		m_iLastMovedTime		=	s_iFrameCounter;
		m_dwLastReportTime		=	timeGetTime();
		m_iWaitMoveResultTime	=	1;

		s_agent.sendMove();

		return	TRUE;
	}

	if	(m_isWalking)
		sendStop();

	return	FALSE;
}

//
//	현재 이동 목표로 이동
BOOL
CHero::sendMoveDestPos()
{
	m_iLastMovedTime	=	s_iFrameCounter;
	m_dwLastReportTime	=	timeGetTime();

	if (m_iWaitMoveResultTime)
	{
		m_iWaitMoveResultTime	=	0;
		m_isWalking				=	FALSE;
		sendStop();
		return	FALSE;
	}

	if	(g_lpHero->moveDestPos())
	{
		m_bf1IsRunning			=	g_lpHero->m_isRunning;
		m_iPPS					=	g_lpHero->m_iPPS;
		m_iWaitMoveResultTime	=	1;

		return s_agent.sendMove();
	}

	sendStop();

	return	FALSE;
}

//
//출구로 이동
BOOL
CHero::moveToArea()
{
	if	(!checkWaitGetGVGAvatarOrgData())
		return	FALSE;

	if	(m_wWaitMoveAreaResultTime	||	m_bf1IsWaitReleaseLeftButtonForAreaWork	==	FALSE)
		return	TRUE;

	if	(g_map.m_area.m_iMoveArea	==	0xffff)
		return	FALSE;

	if	(g_lpHero->isDeath())
		return	FALSE;

	cAreaInfo	*lpArea	=	&g_map.m_area.m_aArea[g_map.m_area.m_iMoveArea];

	sendReportCurrentPos();

	if	(lpArea->isIn(g_lpHero->m_pos.x,g_lpHero->m_pos.y,TRUE))
	{
		int	iAreaKind	=	lpArea->m_wKind;

		m_bf1IsWaitReleaseLeftButtonForAreaWork	=	FALSE;

		if	(iAreaKind	==	dAREA_DOOR)
			operateDoorWork((cAreaDoorInfo*)lpArea);

		if	(iAreaKind	==	dAREA_EVENT_AREA)
			operateEventAreaWork((cAreaEvent*)lpArea);

		if	(iAreaKind	==	dT_AREA_ARCA)
			operateArcaAreaWork((cAreaArcaInfo*)lpArea);

		if	(iAreaKind	==	dAREA_TRAP)
			operateTrapAreaWork((cAreaTrapInfo*)lpArea);

		if	(iAreaKind	==	dAREA_PORTAL)
		{
			int	iGateShape	=	g_map.m_area.m_aArea[g_map.m_area.m_iMoveArea].m_gateShape;
			
			if	(iGateShape	!=	ePAS_INNER_PORTAL	&&	iGateShape	!=	ePAS_HIDDEN_INNER_PORTAL)
			{
				//전투 후 일정 시간이 지나지 않으면 필드 이동을 제한 시킨다.
				if	(g_hero.m_wRestraintTimeByAttackAction)
				{
					char	strMessage[256];

					int		iRemainTime	=	max(1,g_hero.m_wRestraintTimeByAttackAction);

					sprintf(strMessage,dMSG_CAN_NOT_MOVE_FIELD_DURING_BATTLE_FORM,iRemainTime);
					WarningMessage(strMessage);
					return FALSE;
				}

				g_msgBox.cPopup("",dMSG_WAIT_FIELD_MOVE_RESULT,dMSG_CANCEL);
				g_msgBox.setButtonId(eMBI_CANCEL_TO_MOVE_FIELD);
				g_msgBox.setWaitPacket(dSG_MOVE_FIELD_NOW);
			}
			m_wWaitMoveAreaResultTime	=	dWAIT_MOVE_AREA_RESULT_TIME;

			sendStop(FALSE);
			s_agent.sendMoveField();
		}
	}
	else
	{
		CPos	pos;

		g_map.m_area.getFocusAreaCenterPos(&pos);

		moveTo(pos.x,pos.y);
	}

	return	TRUE;
}

//
//	정지/정지 메시지 전송
void
CHero::sendStop(BOOL _bIsCheckChangePosPacketSendCount)
{
	if (g_lpControlActor)	
		return;

	g_lpHero->stop();

	m_isWalking		=	FALSE;
	m_wEngageTarget	=	0xffff;

	s_agent.sendStop(_bIsCheckChangePosPacketSendCount);
}

//
//	정지했냐?
void
CHero::checkStop()
{
	if (g_lpControlActor)	
		return;

	if (g_lpHero->isDeath()	)
		return;
	if (!m_isWalking		)
		return;	//	이동중이다.

	if (!g_lpHero->m_isMove	)	//	도착했다.
	{
		m_isWalking	=	FALSE;
		sendStop();
	}
}

//
//	이동 업데이트
BOOL
CHero::walking(BOOL _bIsIgnoreKey)
{
	if	(g_lpHero->isDeath())
		return	FALSE;
	if(m_wFleeTime)
		return FALSE;
	if	( g_lng.is_ok() == false )
		return FALSE;

	BOOL	bIsExclusiveOtherAction	=	cINPDEV::IsExclusive(eMOUSE_STATE_MOVE);

	if	(bIsExclusiveOtherAction)
		_bIsIgnoreKey	=	TRUE;

	if (!_bIsIgnoreKey &&	s_isLeftButtonPressed && m_wEngageTarget !=	0xffff && CGamePlay::s_iFocusActor	==	0xffff)
	{
		stopBattle();

		return	FALSE;
	}

	if	(s_isLeftButtonPressed)
		s_isLeftButtonPressed	=	s_isLeftButtonPressed;

	if	(!_bIsIgnoreKey)
	{
		BOOL bIsQuickSlotDelay = FALSE;

		if	(m_dwQuickSlotDelayTime > timeGetTime())
			bIsQuickSlotDelay = TRUE;

		if	(s_isLeftButtonPressed && !m_wRushTime && !bIsQuickSlotDelay)
		{
			if	(!checkWaitGetGVGAvatarOrgData())
				return	FALSE;

			if	(g_lpHero->m_wHillSkill != 0xffff)
			{
				g_lpHero->m_iDirect	=	g_lpHero->getDirect(s_posCursor.x,s_posCursor.y,TRUE);

				return	FALSE;
			}

			stopBattle();

			int	iRange	=	GetOvalRange(s_posCursor.x,s_posCursor.y,g_lpHero->m_pos.x,g_lpHero->m_pos.y);

			if (iRange	<=	32*32)
				return	FALSE;

			lockOff();
			m_posClick.Set(s_posCursor.x , s_posCursor.y);//JBC 이동중 도착점 X표시 08-06-30
			moveTo(s_posCursor.x,s_posCursor.y);
			cINPDEV::SetExclusive(eMOUSE_STATE_MOVE);

			return	TRUE;
		}
		else if(m_wRushTime)
		{
			moveTo(s_posCursor.x,s_posCursor.y);
			cINPDEV::SetExclusive(eMOUSE_STATE_MOVE);
		}
		else
		{
			cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_MOVE);
		}
	}

	if (!m_isWalking		)
		return	FALSE;	//	이동중이 아니다.

	if (g_lpControlActor	)
		return	FALSE;

	if (s_iFrameCounter	>=	m_iLastMovedTime+s_iPlayerMovePacketPeriod)
		sendMoveDestPos();

	sendReportCurrentPos();

	return	FALSE;
}

BOOL
CHero::checkWaitGetGVGAvatarOrgData()
{
	if	(g_bIsWaitGetGVGAvatarOrgData)
	{
		WarningMessage(dMSG_WAIT_FOR_SYNC_GVG_AVATAR_DATA);
		return	FALSE;
	}

	return	TRUE;
}
//
//	이동 방법 변경
void
CHero::changeMoveAbility()
{
	if	(!checkWaitGetGVGAvatarOrgData())
		return;
	if	(m_wRushTime)	// 돌진 상태
		return;
	if	(g_lpControlActor				)
		return;
	if	(g_lpHero->m_isExclusiveAction	)
		return;
	if	(!isAbleToChangeMoveMethod()		)
		return;

	m_iLastActionTime	=	s_iFrameCounter;

	s_agent.sendSetMoveAbility();
}

void
CHero::sendReportCurrentPos()
{
#ifdef	_FOR_KOREA
	if (m_isWalking	&&	timeGetTime()	>=	m_dwLastReportTime+1000/4)
	{
		m_dwLastReportTime	=	timeGetTime();
		s_agent.sendReportCurrentPos();
	}
#endif
}