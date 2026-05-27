#include "secretDungeon.h"
#include "cFIELD.h"
#include "cProject.h"
#include "cGame.h"

int	l_aInstanceFieldWarningTime[dINSTANCE_FIELD_WARNING_STEP_COUNT]	=
{
	30,10,5,1
};


cInstanceField::cInstanceField()
{
	m_pFloor	=	NULL;

	reset();
}

cInstanceField::~cInstanceField()
{
	pKILL(m_pFloor);
}

void
cInstanceField::copy(cInstanceField *_lpField)
{
}

void
cInstanceField::reset()
{
	pKILL(m_pFloor);

	memset(m_awDungeonValue,0,sizeof(m_awDungeonValue));

	m_wFloorCount			=	0;
	m_iTotalGuildHonorPoint	=	0;
	m_iGuildHonorPoint		=	0;
	m_iPeneltyGuildHonorPoint=	0;
	m_wSecretDungeonSerial	=	0;
	m_wSerial				=	0xffff;
	m_wLifeTime				=	30;
	m_wFocusFloor			=	0xffff;
	m_wStressMinimapFloor	=	0xffff;
}

void
cInstanceField::close()
{
	cParty	*lpParty	=	getParty();

	if (lpParty)
		lpParty->sendEnterToTherSecretDungeonMessage(0xffff,0xffff);

	banishAllPlayer();

	reset();
}

cParty*
cInstanceField::getParty()
{
	return	g_pPartyManager->getByUniqueSerial(m_wPartySerial,m_dwPartyUniqueSerial);
}

int
cInstanceField::getFloorIndex(char *_lpstrFileName)
{
	for (int iFloor=0;iFloor<m_wFloorCount;iFloor++)
	{
		cFIELD	*lpField	=	&m_pFloor[iFloor];

		if	(STRICMP(lpField->m_strFileName+10,_lpstrFileName+10)	==	0)
			return	iFloor;
	}

	return	-1;
}

void
cInstanceField::addDungeonValue(int _iValueIndex,int _iValue)
{
	if(_iValueIndex < 0 || _iValueIndex >= dMAX_IF_VALUE_COUNT)
		return;

	int	iValue	=	m_awDungeonValue[_iValueIndex]+_iValue;

	iValue		=	max(iValue,0);
	iValue		=	min(iValue,0xffff);

	m_awDungeonValue[_iValueIndex]	=	iValue;

#ifdef	_FOR_KOREA
	if (g_bIsWantPrintTriggerCondition)
	{
		if (_iValue	>	0)
			printf("던전수치[%d] 증가[%d],현재 수치 %d\n",_iValueIndex,_iValue,iValue);
		else
			printf("던전수치[%d] 감소[%d],현재 수치 %d\n",_iValueIndex,-_iValue,iValue);
	}
#endif
}

void
cInstanceField::setDungeonValue(int _iValueIndex,int _iValue)
{
	if(_iValueIndex < 0 || _iValueIndex >= dMAX_IF_VALUE_COUNT)
		return;

	m_awDungeonValue[_iValueIndex]	=	_iValue;
#ifdef	_FOR_KOREA
	if (g_bIsWantPrintTriggerCondition)
		printf("던전수치[%d] 설정 - %d\n",_iValueIndex,_iValue);
#endif
}

int
cInstanceField::getDungeonValue(int _iValueIndex)
{
	if(_iValueIndex < 0 || _iValueIndex >= dMAX_IF_VALUE_COUNT)
		return	-1;

#ifdef	_FOR_KOREA
	if (g_bIsWantPrintTriggerCondition)
		printf("던전수치[%d] - %d\n",_iValueIndex,m_awDungeonValue[_iValueIndex]);
#endif

	return	m_awDungeonValue[_iValueIndex];
}

BOOL
cInstanceField::init(int _iDungeonSerial,int _iSerial,cParty *_lpParty,int _iLinkField,int _iLinkGate)
{
	cSecretDungeon	*lpDungeon	=	g_pSdManager->get(_iDungeonSerial);

	if (!lpDungeon)
		return	FALSE;

	int	i;

	reset();

	m_bIsGuildPointBattleField		=	lpDungeon->m_bf1IsTowerOfOrdeal;
	m_bISGuildDungeonFiled			=	lpDungeon->m_bf1IsGuildDungen;

	m_pFloor		=	new cFIELD [lpDungeon->m_bf8FloorCount];

	for (i=0;i<lpDungeon->m_bf8FloorCount;i++)
	{
		char	strFileName[512];

		sprintf(strFileName,"%s/expansion/[%s]%.3d.rmd",g_project.m_strMapFolder,lpDungeon->m_strIDCode,i);

		if	(!m_pFloor[i].load(strFileName,_iSerial*dINSTANCE_FIELD_FLAG+i,_iLinkField))
			return	FALSE;

		sprintf(m_pFloor[i].m_strFileName,"expansion/[%s]%.3d.rmd",lpDungeon->m_strIDCode,i);			

		m_pFloor[i].m_iSerial			=	_iSerial*dINSTANCE_FIELD_FLAG+i;
		m_pFloor[i].m_wCorrectExperience=	lpDungeon->m_aFloor[i].m_wCorrectExperience;
		m_pFloor[i].m_wIsRegenMonster	=	lpDungeon->m_aFloor[i].m_wIsRegenMonster;
	}

	m_wCountdownTimer				=	0;
	m_wCompleteCountdown			=	0;
	m_wSecretDungeonSerial			=	_iDungeonSerial;
	m_wLinkField					=	_iLinkField;
	m_wLinkGate						=	_iLinkGate;
	m_wFloorCount					=	lpDungeon->m_bf8FloorCount;
	m_wUpkeepMinuteInValidPlayerStatus=	dINSTANCE_FIELD_UPKEEPTIME_IN_VALID_PLAYER;
	m_wUpkeepMinute					=	lpDungeon->m_wUpkeepTime;
	m_wIsReadyToClose				=	FALSE;
	
	m_wIsProgressEvent				=	FALSE;

	m_iRealPlayerCount				=	0;
	m_wDestroyCounter				=	0;

	memset(m_abIsAnnouncedWarning,0,sizeof(m_abIsAnnouncedWarning));
 
	m_dwPartyUniqueSerial			=	_lpParty->m_dwUniqueSerial;
	m_wPartySerial					=	_lpParty->m_wSerial;
	m_wGenerateMinute				=	0;
	m_wEnterPlayerCount				=	0;

	cAreaInfo	*lpStartArea		=	m_pFloor[0].m_pArea->getStartArea();

	if (!lpStartArea)
		return	ERRMSG("error!!","can not find starting area!! - cInstanceField::init");

	cPOINT		posBegin;

	lpStartArea->getCenterPos(&posBegin);

	_lpParty->sendEnterToTherSecretDungeonMessage(_iLinkField,_iLinkGate);

	return	TRUE;
}

void
cInstanceField::banishAllPlayer()
{
	for (int iFloor=0;iFloor<m_wFloorCount;iFloor++)
	{
		cFIELD	*lpField	=	&m_pFloor[iFloor];

		for (int i=0;i<lpField->m_aiActorCount[0];i++)
		{
			int		iSerial		=	lpField->m_awActorList[0][i];
			cACTOR	*lpPlayer	=	lpField->getTestedActor(iSerial);

			if	(!lpPlayer)
				continue;

			if	(m_bIsGuildPointBattleField)
				lpPlayer->returnToGuildHallByEndGuildPointBattle();
			else
				lpPlayer->leaveInstanceField(m_wLinkField);
		}
	}
}

void
cInstanceField::extendUpkeepTime(int _iMinute)
{
	m_wUpkeepMinute	+=	_iMinute;
}

void
cInstanceField::complete(int _iMethod,int _iRemainTime , BOOL _bISGuildDungeon, BOOL _bISGuildPointBattle)
{
	int iMessage	=	eSDM_COMPLETE;

	if	(m_wIsReadyToClose)
		return;

	if	(_iRemainTime	==	0)
		_iRemainTime	=	dINSTANCE_FIELD_COMPLETE_COUNTER;

	m_wIsReadyToClose	=	TRUE;

	m_wCompleteCountdown=	_iRemainTime;

	if(_bISGuildDungeon)
		iMessage	=		eSDM_GUILD_DUNGEON_COMPLETE;

	if (!_bISGuildPointBattle)
		sendMessage(iMessage,m_wCompleteCountdown,_iMethod);

	cParty	*lpParty	=	getParty();

	if	(lpParty)
		lpParty->sendEnterToTherSecretDungeonMessage(0xffff,0xffff);
}

void
cInstanceField::dissolutionParty()
{
	if (m_wIsReadyToClose)
		return;

	m_wIsReadyToClose	=	TRUE;

	m_wCompleteCountdown=	dINSTANCE_FIELD_COMPLETE_COUNTER;

	sendMessage(eSDM_DISSOLUTION_PARTY,m_wCompleteCountdown);

	cParty	*lpParty	=	getParty();

	if (lpParty)
		lpParty->sendEnterToTherSecretDungeonMessage(0xffff,0xffff);
}

void
cInstanceField::readyToClose()
{
	banishAllPlayer();
	m_wDestroyCounter	=	dINSTANCE_FIELD_DEATH_COUNTER;
	m_wIsReadyToClose	=	TRUE;

	cParty	*lpParty	=	getParty();

	if (lpParty)
		lpParty->sendEnterToTherSecretDungeonMessage(0xffff,0xffff);
}

BOOL
cInstanceField::update()
{
	if	(m_wIsReadyToClose	==	FALSE)
	{
		if	(g_pPartyManager->getByUniqueSerial(m_wPartySerial,m_dwPartyUniqueSerial)	==	NULL)
		{
			dissolutionParty();

			return	FALSE;
		}

		if	(m_wUpkeepMinuteInValidPlayerStatus == 0 || m_wUpkeepMinute == 0)
		{
			readyToClose();

			return	FALSE;
		}

		if	(m_iRealPlayerCount)
			m_wUpkeepMinuteInValidPlayerStatus	=	dINSTANCE_FIELD_UPKEEPTIME_IN_VALID_PLAYER;

		for (int i=0;i<dINSTANCE_FIELD_WARNING_STEP_COUNT;i++)
		{
			if	(m_wUpkeepMinute	==	l_aInstanceFieldWarningTime[i]	&&	m_abIsAnnouncedWarning[i]	==	FALSE)
			{
				m_abIsAnnouncedWarning[i]	=	TRUE;

				
				if(m_bISGuildDungeonFiled)
					sendMessage(eSDM_WARNING_DISAPEAR_GUILD_DUNGEON,m_wUpkeepMinute);
				else
					sendMessage(eSDM_WARNING_DISAPEAR_DUNGEON,m_wUpkeepMinute);
			}
		}

		if	(m_wCountdownTimer)
		{
			if	(m_wIsProgressEvent	==	FALSE)
			{
				m_wCountdownTimer--;

				if	((m_wCountdownTimer%30)	==	5)
					sendCountdownRemainTime();
			}
		}

		return	FALSE;
	}

	if	(m_wCompleteCountdown)
	{
		m_wCompleteCountdown--;

		if	(m_iRealPlayerCount	==	0)
			m_wCompleteCountdown	=	0;

		if	(m_wCompleteCountdown	==	0)
			readyToClose();

		return	FALSE;
	}

	if	(m_iRealPlayerCount	==	0)
		m_wDestroyCounter--;

	if	(m_wDestroyCounter	==	0)
		return	TRUE;

	return	FALSE;
}

void
cInstanceField::beginCountdownTimer(int _iTime)
{
	m_wCountdownTimer	=	_iTime;

	sendCountdownRemainTime();
}

void
cInstanceField::stopCountdownTimer()
{
	m_wCountdownTimer	=	0;

	sendCountdownRemainTime();
}

void
cInstanceField::extensionCountdownTimer(int _iTime)
{
	m_wCountdownTimer	+=	_iTime;
	
	sendCountdownRemainTime();
}

void
cInstanceField::reductionCountdownTimer(int _iTime)
{
	if	(m_wCountdownTimer	>=	_iTime)
		m_wCountdownTimer	-=	_iTime;
	else
		m_wCountdownTimer	=	0;

	m_wCountdownTimer		=	max(m_wCountdownTimer,1);

	sendCountdownRemainTime();
}

void
cInstanceField::sendPacketToAllPlayer(char *_lpData,int _iFloor)
{
	if	(_iFloor	==	-1)
	{
		for (int iFloor=0;iFloor<m_wFloorCount;iFloor++)
			m_pFloor[iFloor].addSendPacket((ALL_MSG*)_lpData,0xffff);

		return;
	}

	if	(_iFloor	<	0	||	_iFloor	>=	m_wFloorCount)
		return;

	m_pFloor[_iFloor].addSendPacket((ALL_MSG*)_lpData,0xffff);
}

void
cInstanceField::sendMessage(int _iMessage,int _iValue1,int _iValue2,int _iFloor)
{
	SG_SECRET_DUNGEON_MESSAGE	packet;

	packet.base.set(sizeof(packet),dSG_SECRET_DUNGEON_MESSAGE);

	packet.wMessage		=	_iMessage;
	packet.iValue1		=	_iValue1;
	packet.iValue2		=	_iValue2;

	sendPacketToAllPlayer((char *)&packet,_iFloor);
}

void
cInstanceField::sendCountdownRemainTime()
{
	sendMessage(eSDM_BEGIN_COUNTDOWN_TIMER,m_wCountdownTimer);
}

void
cInstanceField::sendMessageForChangeFloorActor(cACTOR *_lpActor)
{
	if	(m_wFocusFloor	!=	0xffff	&&	m_wFocusFloor	==	_lpActor->m_bf4InstanceFieldFloor)
	{
		if	(m_wFocusActor		!=	0xffff)
			_lpActor->sendSecretDungeonMessage(eSDM_FOCUS_ON_CHARACTER,m_wFocusActor);

		if	(m_posGroundFocus.x	!=	-1		)
			_lpActor->sendSecretDungeonMessage(eSDM_FOCUS_ON_GROUND,m_posGroundFocus.x,m_posGroundFocus.y);
	}

	if	(m_wStressMinimapFloor	!=	0xffff	&&	m_wStressMinimapFloor	==	_lpActor->m_bf4InstanceFieldFloor)
		_lpActor->sendSecretDungeonMessage(eSDM_STRESS_MINIMAP,m_posStressMinimap.x,m_posStressMinimap.y);

	if	(m_wCountdownTimer)
		_lpActor->sendSecretDungeonMessage(eSDM_BEGIN_COUNTDOWN_TIMER,m_wCountdownTimer);
}

void
cInstanceField::setEvent(BOOL _bIsEnd,BOOL _bIsDisableBattle)
{
	m_wIsProgressEvent	=	1-_bIsEnd;

	for (int iFloor=0;iFloor<m_wFloorCount;iFloor++)
	{
		cFIELD	*lpField	=	&m_pFloor[iFloor];

		lpField->m_bIsBlockBattle	=	_bIsDisableBattle;
		lpField->m_bIsProcessEvent	=	1-_bIsEnd;
	}

	if (_bIsEnd)
		sendMessage(eSDM_END_EVENT);
	else
		sendMessage(eSDM_BEGIN_EVENT);
}

void
cInstanceField::setGroundFocus(int _iX,int _iY,int _iFloor)
{
	m_wFocusActor		=	0xffff;

	m_posGroundFocus.x	=	_iX;
	m_posGroundFocus.y	=	_iY;

	m_wFocusFloor		=	_iFloor;

	sendMessage(eSDM_FOCUS_ON_GROUND,m_posGroundFocus.x,m_posGroundFocus.y,_iFloor);
}

void
cInstanceField::setCharacterFocus(int _iActorSerial,int _iFloor)
{
	m_posGroundFocus.x	=	-1;
	m_posGroundFocus.y	=	-1;

	m_wFocusFloor		=	_iFloor;
	m_wFocusActor		=	_iActorSerial;

	sendMessage(eSDM_FOCUS_ON_CHARACTER,_iActorSerial,0,_iFloor);
}

void
cInstanceField::releaseEventFocus()
{
	m_wFocusFloor		=	0xffff;

	sendMessage(eSDM_RELEASE_FOCUS);
}

void
cInstanceField::addGuildHonorPoint(int _iPoint)
{
	if	(_iPoint	>	0)
		m_iTotalGuildHonorPoint	+=	_iPoint;

	m_iGuildHonorPoint	+=	_iPoint;

	if	(_iPoint	<	0)
		m_iPeneltyGuildHonorPoint	-=	_iPoint;

	sendMessage(eSDM_GET_GUILD_HONOR_POINT,_iPoint,m_iGuildHonorPoint);
}

void
cInstanceField::stressMinimap(int _iX,int _iY,int _iFloor)
{
	m_posStressMinimap.x	=	_iX;
	m_posStressMinimap.y	=	_iY;

	m_wStressMinimapFloor	=	_iFloor;

	sendMessage(eSDM_STRESS_MINIMAP,_iX,_iY,_iFloor);
}

void
cInstanceField::releaseStressMinimap()
{
	m_wStressMinimapFloor	=	0xffff;

	sendMessage(eSDM_RELEASE_STRESS_MINIMAP);
}

int
cInstanceField::addEnterPlayerLog(char *_lpstrName,BOOL *_lpbIsFirst)
{
	int	i;

	*_lpbIsFirst	=	TRUE;

	for (i=0;i<m_wEnterPlayerCount;i++)
	{
		if (STRICMP(_lpstrName,m_strEnterPlayerList[i])	==	0)
		{
			*_lpbIsFirst	=	FALSE;

			return	i;
		}
	}

	if	(m_wEnterPlayerCount	>=	dENTER_PLAY_IF_PLAYER_LIST_BUFFER_SIZE)
		return	0xffff;

	i	=	m_wEnterPlayerCount;

	strcpy(m_strEnterPlayerList[m_wEnterPlayerCount],_lpstrName);
	m_wEnterPlayerCount++;

	return	i;
}

void
cInstanceField::removeEnterPlayerLog(char *_lpstrName)
{
	int	i,j;

	for (i=0;i<m_wEnterPlayerCount;i++)
	{
		if (STRICMP(_lpstrName,m_strEnterPlayerList[i])	==	0)
		{
			for (j=i;j<m_wEnterPlayerCount-1;j++)
				strcpy(m_strEnterPlayerList[j],m_strEnterPlayerList[j+1]);

			m_wEnterPlayerCount--;

			return;
		}
	}
}