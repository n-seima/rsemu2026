#include "cHero.h"
#include "cArea.h"
#include "cMap.h"
#include "cMessage.h"
#include "cActor.h"

BOOL
CHero::isVisibleDoor(cAreaDoorInfo *_lpDoor)
{
	if (_lpDoor->m_bf1IsActive	==	FALSE)
		return	FALSE;
	if (_lpDoor->m_bSecretLevel	>	m_detectSecretObject.getCorrectedValue())
		return	FALSE;

	return	TRUE;
}

BOOL
CHero::isVisibleDoorTrap(cAreaDoorInfo *_lpDoor)
{
	if (_lpDoor->m_bf1IsActive		==	FALSE)
		return	FALSE;
	if (_lpDoor->m_bf1IsDisarmed)
		return	FALSE;
	if (_lpDoor->m_bSecretLevel		>	m_detectSecretObject.getCorrectedValue())
		return	FALSE;
	if (_lpDoor->m_bDetectTrapLevel	>	m_detectTrap.getCorrectedValue())
		return	FALSE;

	return	TRUE;
}

BOOL
CHero::isVisibleTrap(cAreaTrapInfo *_lpTrap)
{
	if (_lpTrap->m_wIsActive		==	FALSE)
		return	FALSE;

	if (_lpTrap->m_wDetectTrapLevel	>	m_detectTrap.getCorrectedValue())
		return	FALSE;

	return	TRUE;
}

void
CHero::operateDoorWork(cAreaDoorInfo *_lpDoor)
{
	m_wWaitMoveAreaResultTime	=	dSYNC_FPS/4;
	m_bf1IsWaitDoorWorkResult	=	TRUE;

	sendStop();

	if	(_lpDoor->m_bf1IsOpened)
		if	(_lpDoor->m_bf1IsOpenedByHero	||	(_lpDoor->m_wLinkSecretDungeon	&&	_lpDoor->m_wLinkSecretDungeon	!=	0xffff))
		{
			if	(_lpDoor->m_wLinkSecretDungeon	&&	_lpDoor->m_wLinkSecretDungeon	!=	0xffff)
				s_agent.sendChangeDoorStatus(g_map.m_area.m_iMoveArea,eCDSM_ENTER_TO_THE_SECRET_DUNGEON);
			else
				s_agent.sendChangeDoorStatus(g_map.m_area.m_iMoveArea,eCDSM_CLOSE_DOOR);

			return;
		}

	if	(_lpDoor->m_bf1IsHide)
	{
		if (!isVisibleDoor(_lpDoor))
			return;
	}

	if	(_lpDoor->m_bf1IsDisarmed	==	FALSE && isVisibleDoorTrap(_lpDoor))
	{
		s_agent.sendChangeDoorStatus(g_map.m_area.m_iMoveArea,eCDSM_DISARM_DOOR);

		return;
	}

	if	(_lpDoor->m_bf1IsUnlocked	==	FALSE)
	{
		s_agent.sendChangeDoorStatus(g_map.m_area.m_iMoveArea,eCDSM_UNLOCK_DOOR);

		return;
	}

	s_agent.sendChangeDoorStatus(g_map.m_area.m_iMoveArea,eCDSM_OPEN_DOOR);
}

void
CHero::operateEventAreaWork(cAreaEvent *_lpArea)
{
	m_wWaitMoveAreaResultTime	=	dSYNC_FPS/4;
	m_bf1IsReadyToEventAreaWork	=	FALSE;

	sendStop();

	s_agent.sendTouchEventArea(_lpArea->m_wSerial);
}

void
CHero::operateArcaAreaWork(cAreaArcaInfo *_lpArea)
{
	g_lpHero->stop();

	m_isWalking					=	FALSE;
	m_wEngageTarget				=	0xffff;

	m_wWaitMoveAreaResultTime	=	dSYNC_FPS/4;
	m_bf1IsWaitArcaWorkResult	=	TRUE;

	sendStop();

	s_agent.sendArcaWork(_lpArea->m_wLinkArca,eAAW_OPEN);
}

void
CHero::operateTrapAreaWork(cAreaTrapInfo *_lpArea)
{
	g_lpHero->stop();

	m_isWalking					=	FALSE;
	m_wEngageTarget				=	0xffff;

	m_wWaitMoveAreaResultTime	=	dSYNC_FPS/4;
	m_bf1IsWaitTrapWorkResult	=	TRUE;

	sendStop();

	s_agent.sendDisarmFieldTrap(_lpArea->m_wSerial);
}