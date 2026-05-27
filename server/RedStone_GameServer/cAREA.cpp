#include "cArea.H"
#include "cFIELD.H"
#include "CGame.H"

//
//	이 영역 안에서 임의의 좌표를 찾는다.
void
cAreaInfo::getRandomPos(cPOINT *_lpPos)
{
	_lpPos->x	=	x1+random(x2-x1);
	_lpPos->y	=	y1+random(y2-y1);
}

void
cAreaInfo::getCenterPos(cPOINT *_lpPos)
{
	_lpPos->x	=	x1+(x2-x1)/2;
	_lpPos->y	=	y1+(y2-y1)/2;
}

//
//	설정
void
cAreaInfo::set(cAreaSaveData *_lpArea)
{
	memcpy(getBasePoint(),_lpArea,sizeof(cAreaBaseInfo));

	switch(_lpArea->m_wKind)
	{
		case	dAREA_NORMAL			:
		case	dAREA_NOTICE_BOARD		:
		case	dAREA_PORTAL			:
		case	dAREA_MONSTER_ACTIVE	:
		case	dAREA_START_AREA		:
		case	dAREA_NAMED_AREA		:
		case	dAREA_GUILD_BATTLE		:
		case	dAREA_OX_AREA_O			:
		case	dAREA_OX_AREA_X			:
		case	dAREA_REVIVE_AREA		:
		case	dAREA_EVENT_AREA		:
		case	dAREA_GUILD_CREST		:
		case	dAREA_SECRET_DUNGEON_EXIT	:
			break;

		case	dAREA_DOOR				:
		{
			cAreaDoorInfo *lpDoorArea	=	(cAreaDoorInfo *)(this);

			memcpy(lpDoorArea->getSaveDataPoint(),_lpArea->m_aBuffer,sizeof(cAreaDoorDefine));
			break;
		}
		case	dAREA_TRAP				:
		{
			cAreaTrapInfo *lpDoorArea	=	(cAreaTrapInfo *)(this);

			memcpy(lpDoorArea->getSaveDataPoint(),_lpArea->m_aBuffer,sizeof(cAreaTrapDefine));
			break;
		}
		case	dAREA_ARCA				:
		{
			cAreaArcaInfo *lpArcaArea	=	(cAreaArcaInfo *)(this);

			memcpy(lpArcaArea->getSaveDataPoint(),_lpArea->m_aBuffer,sizeof(cAreaArcaDefine));

			lpArcaArea->m_wIsEnable		=	TRUE;
			break;
		}
	}

	if (_lpArea->m_bf1IsTestPortal && _lpArea->m_bf15LinkObject == 0x7000)
		m_wIsTestPortal	=	TRUE;
	else
		m_wIsTestPortal	=	FALSE;

	pKILL(m_pstrText);
}

void
cAreaInfo::operateKarma(cACTOR *_lpAsker)
{
	if (_lpAsker->m_bf1IsOperateEventAreaWork)
		return;

	_lpAsker->m_bf1IsOperateEventAreaWork	=	TRUE;
	m_cc.m_wIsEventArea						=	TRUE;
	m_cc.operateFirstSpeech(_lpAsker,_lpAsker,m_wSerial);
	_lpAsker->m_bf1IsOperateEventAreaWork	=	FALSE;
}

BOOL
cAreaInfo::selectSpeech(cACTOR *_lpAsker,int _iConversationIndex,int _iSelectSpeech,BOOL _bIsInnerSelect)
{
	if (m_cc.m_wConversationCount	==	0)
		return	FALSE;

	if (_bIsInnerSelect	==	FALSE)
	{
		g_debugSign.m_iTempStep	=	2;

		if (_lpAsker->m_wSelectConversation	!=	_iConversationIndex || _lpAsker->m_wTalkNpc	!=	m_wSerial)
		{
//			CLOG("__suspectedHacker","event area %s/%s %s[%d,%d] ",_lpAsker->m_strId,_lpAsker->m_strName,_lpAsker->m_lpField->m_strFileName,_lpAsker->m_pos.mx,_lpAsker->m_pos.my);
//			printf("suspectedHacker!! event area %s/%s %s[%d,%d] \n",_lpAsker->m_strId,_lpAsker->m_strName,_lpAsker->m_lpField->m_strFileName,_lpAsker->m_pos.mx,_lpAsker->m_pos.my);

			g_debugSign.m_iTempStep	=	3;
			return	FALSE;
		}

		if (_lpAsker->m_wBlockSelectSpeechTime)
			return	FALSE;
	}

	_lpAsker->m_wBlockSelectSpeechTime	=	4;

	g_debugSign.m_iTempStep	=	4;

	return	m_cc.operateSelectSpeechReaction(_lpAsker,_lpAsker,_iConversationIndex,_iSelectSpeech);
}

cArea::cArea()
{
	memset(m_pArea,0,sizeof(m_pArea));
	
	reset();
}

cArea::~cArea()
{
	for (int i=0;i<dMAX_AREA;i++)
		KILL(m_pArea[i]);
}

//
//	초기화
void
cArea::reset()
{
	memset(m_aPortal			,0xff	,sizeof(m_aPortal)			);
	memset(m_awGuildBattleArea	,0xff	,sizeof(m_awGuildBattleArea));

	m_wGuildBattleAreaCount	=	0;
	m_iCount				=	0;
}

//
//	시작 영역
cAreaInfo*
cArea::getStartArea()
{
	if	(m_aPortal[dAREA_START_POINT]	==	0xffffffff)
		return	NULL;

	return	m_pArea[m_aPortal[dAREA_START_POINT]];
}

//
//	길드전 영역 리턴~~
cAreaInfo*
cArea::getGuildBattleArea(int _iIndex)
{
	if	(_iIndex	>=	m_wGuildBattleAreaCount)
		return	NULL;

	return	m_pArea[m_awGuildBattleArea[_iIndex]];
}

//
//	OX 퀴즈의 O 영역
cAreaInfo*
cArea::getOXQuizOArea()
{
	for (int i=0;i<dMAX_AREA;i++)
	{
		if (m_pArea[i]	==	NULL)
			continue;

		if (m_pArea[i]->m_wKind	==	dAREA_OX_AREA_O)
			return	m_pArea[i];

	}

	return	NULL;
}

//
//	OX 퀴즈의 X 영역
cAreaInfo*
cArea::getOXQuizXArea()
{
	for (int i=0;i<dMAX_AREA;i++)
	{
		if (m_pArea[i]	==	NULL)
			continue;

		if (m_pArea[i]->m_wKind	==	dAREA_OX_AREA_X)
			return	m_pArea[i];

	}

	return	NULL;
}

//
//	영역 정보를 설정한다.
BOOL
cArea::build(cFIELD *_lpField)
{
	int		i;
	int		iPortalCount		=	0;
	DWORD	dwTime				=	g_dwCurrentTime;

	m_iField					=	_lpField->m_iUniqueSerial;

	m_iTestPortalCount			=	0;
	m_iCount					=	0;
	m_wTrapCount				=	0;
	m_wDoorCount				=	0;
	m_wArcaCount				=	0;
	m_wEventAreaCount			=	0;
	m_wMonsterActiveAreaCount	=	0;
	m_iSecretDungeonExit		=	-1;

	m_aPortal[dAREA_START_POINT]=	0xffffffff;

	m_pArea[0]->x1	=	0;
	m_pArea[0]->y1	=	0;
	m_pArea[0]->x2	=	_lpField->m_iPixelWidth;
	m_pArea[0]->y2	=	_lpField->m_iPixelHeight;

	for (i=0;i<dMAX_AREA;i++)
	{
		if	(m_pArea[i]			==	NULL)
			continue;

		if	(m_pArea[i]->m_wIsTestPortal && g_bIsTestServer	==	FALSE)
			continue;

		m_iCount++;

		if	(m_pArea[i]->isMonsterActiveArea()	)
		{
			if	(m_wMonsterActiveAreaCount	>=	dMAX_AREA)
				ERRMSG("error","monster area count over!! [%d/%d] %s",m_wMonsterActiveAreaCount,dMAX_AREA,_lpField->m_strName);
			m_awMonsterActiveAreaList[m_wMonsterActiveAreaCount++]	=	i;
		}

		if	(m_pArea[i]->isPortal()			)
		{
			if	(iPortalCount	>=	dMAX_PORTAL_AREA_COUNT)
				ERRMSG("error","portal area count over!! [%d/%d] %s",iPortalCount,dMAX_PORTAL_AREA_COUNT,_lpField->m_strName);
			m_aPortal[iPortalCount++]	=	i;
		}

		if	(m_pArea[i]->isStartArea()		)
			m_aPortal[dAREA_START_POINT]=	i;

		if	(m_pArea[i]->isSecretDungeonExit()	)
			m_iSecretDungeonExit	=	i;

		if	(m_pArea[i]->isGuildBattleArea()	)
		{
			if	(m_wEventAreaCount	>=	dMAX_GUILD_BATTLE_AREA_COUNT)
				return	ERRMSG("error","guild battle area count over!! [%d/%d] %s",m_wGuildBattleAreaCount,dMAX_GUILD_BATTLE_AREA_COUNT,_lpField->m_strName);
			m_awGuildBattleArea[m_wGuildBattleAreaCount++]=	i;
		}

		if	(m_pArea[i]->isEventArea()	||	m_pArea[i]->isGuildCrest())
		{
			if	(m_wEventAreaCount	>=	dMAX_EVENT_AREA_IN_MAP)
				return	ERRMSG("error","event area count over!! [%d/%d] %s",m_wEventAreaCount,dMAX_EVENT_AREA_IN_MAP,_lpField->m_strName);
			
			m_awEventAreaList[m_wEventAreaCount++]=	i;
		}
		if	(m_pArea[i]->isDoor()	)
		{
			cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)m_pArea[i];

			if	(m_wDoorCount>=	dMAX_DOOR_IN_MAP)
				return	ERRMSG("error","door count over!! [%d/%d] %s",m_wDoorCount,dMAX_DOOR_IN_MAP,_lpField->m_strName);

			m_awDoorList[m_wDoorCount++]=	i;
		}

		{
			if	(m_pArea[i]->isTrap()	)
			{
				if	(m_wTrapCount	>=	dMAX_TRAP_AREA_IN_MAP)
					return	ERRMSG("error","trap count over!! [%d/%d] %s",m_wTrapCount,dMAX_TRAP_AREA_IN_MAP,_lpField->m_strName);

				cAreaTrapInfo	*lpAreaTrap		=	(cAreaTrapInfo	*)m_pArea[i];

				lpAreaTrap->m_lpField			=	_lpField;
				lpAreaTrap->initTrap();

				m_awTrapList[m_wTrapCount++]=	i;
			}

			if (m_pArea[i]->isArca()	)
			{
				if	(m_wArcaCount	>=	dMAX_ARCA_AREA_IN_MAP)
					return	ERRMSG("error","arca count over!! [%d/%d] %s",m_wArcaCount,dMAX_ARCA_AREA_IN_MAP,_lpField->m_strName);

				cAreaArcaInfo	*lpAreaArca		=	(cAreaArcaInfo	*)m_pArea[i];

				lpAreaArca->m_wLinkActiveArca	=	0xffff;
				lpAreaArca->m_lpField			=	_lpField;

				m_awArcaList[m_wArcaCount++]	=	i;
			}
		}

		if	(m_pArea[i]->m_wIsTestPortal		)
			m_iTestPortalCount++;
	}

	for (i=0;i<m_wDoorCount;i++)
	{
		cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)m_pArea[m_awDoorList[i]];

		lpDoor->m_lpField		=	_lpField;
		_lpField->getLinkObjectWithDoor(lpDoor);
		lpDoor->generate();
	}

	if	(iPortalCount==0)
		return	FALSE;

	return	TRUE;
}

//
//	포커스 영역을 구한다.
cAreaInfo*
cArea::getFocusArea(int _iX,int _iY)
{
	int		i;

	for (i=0;i<dMAX_PORTAL_AREA_COUNT;i++)
	{
		int	iArea	=	m_aPortal[i];

		if	(iArea	==	-1)
			continue;

		if	(m_pArea[iArea]->isIn(_iX,_iY))
			return	m_pArea[iArea];
	}

	return	NULL;
}

cAreaInfo*
cArea::getDoorByGeneratePeriod(int _iPeriod,int _iIndex)
{
	int	iCount	=	0;

	for (int i=0;i<m_wDoorCount;i++)
	{
		cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)m_pArea[m_awDoorList[i]];

		if	(lpDoor->m_wRegenPeriod	==	_iPeriod)
		{
			if	(iCount	==	_iIndex)
				return	m_pArea[m_awDoorList[i]];

			iCount++;
		}
	}

	return	NULL;
}

cAreaInfo*
cArea::getGuildHallGate()
{
	for (int i=0;i<dMAX_PORTAL_AREA_COUNT;i++)
	{
		int	iArea	=	m_aPortal[i];

		if	(iArea	==	-1)
			continue;

		if	(m_pArea[iArea]->m_gateShape	==	ePAS_ENTER_GUILD_HALL)
			return	m_pArea[iArea];
	}

	return	NULL;
}

//
//	아무튼 첫번째 출입구-_-를 찾는다.
cAreaInfo*
cArea::getFirstPortal()
{
	cAreaInfo*	lpArea	=	getStartArea();

	if (lpArea)
		return	lpArea;

	for (int i=0;i<dMAX_PORTAL_AREA_COUNT;i++)
	{
		int	iArea	=	m_aPortal[i];

		if (iArea	==	-1)
			continue;

		return	m_pArea[iArea];
	}

	return	NULL;
}

//
//시리얼을 기준으로 영역 인덱스를 찾아낸다.
int
cArea::findAreaIndexBySerial(int _iSerial)
{
	if (_iSerial	>=	dMAX_AREA)	
		return	-1;

	if(!_iSerial)
		return -1;

	if(_iSerial < 0)
		return -1;

	for (int i=0;i<dMAX_AREA;i++)
	{
		if (m_pArea[i]	==	NULL)
			continue;
		if (m_pArea[i]->m_wSerial	==	_iSerial)
			return	i;
	}

	return	-1;
}

cAreaInfo*
cArea::getArea(int _iIndex,BOOL _bIsAlloc,int _iKind)
{
	if (_iIndex<0 || _iIndex>=dMAX_AREA)
		return	NULL;

	if (m_pArea[_iIndex]	==	NULL)
	{
		if (_bIsAlloc)
		{
			switch(_iKind)
			{
				case	dAREA_NORMAL			:
				case	dAREA_NOTICE_BOARD		:
				case	dAREA_PORTAL			:
				case	dAREA_MONSTER_ACTIVE	:
				case	dAREA_SECRET_DUNGEON_EXIT:
				case	dAREA_START_AREA		:
				case	dAREA_NAMED_AREA		:
				case	dAREA_GUILD_BATTLE		:
				case	dAREA_OX_AREA_O			:
				case	dAREA_OX_AREA_X			:
				case	dAREA_REVIVE_AREA		:
				case	dAREA_EVENT_AREA		:
				case	dAREA_GUILD_CREST		:
					m_pArea[_iIndex]	=	new cAreaInfo();
					break;

				case	dAREA_DOOR				:
					m_pArea[_iIndex]	=	(cAreaInfo *) new cAreaDoorInfo();
					break;
				case	dAREA_TRAP				:
					m_pArea[_iIndex]	=	(cAreaInfo *) new cAreaTrapInfo();
					break;
				case	dAREA_ARCA				:
					m_pArea[_iIndex]	=	(cAreaInfo *) new cAreaArcaInfo();
					break;
				case	dAREA_HUNTING_AREA		:
					m_pArea[_iIndex]	=	(cAreaInfo *) new cAreaHuntingInfo();
					break;
			}

			m_pArea[_iIndex]->resetForInit();
		}
		else
			return	NULL;
	}

	return	m_pArea[_iIndex];
}

void
cArea::operate1SecondWork()
{
	int	i;

	for (i=0;i<m_wDoorCount;i++)
	{
		cAreaDoorInfo	*lpDoor		=	(cAreaDoorInfo	*)m_pArea[m_awDoorList[i]];

		lpDoor->operate1SecondWork();
	}

	for (i=0;i<m_wArcaCount;i++)
	{
		cAreaArcaInfo	*lpArcaArea	=	(cAreaArcaInfo	*)m_pArea[m_awArcaList[i]];

		lpArcaArea->operate1SecondWork();
	}

	for (i=0;i<m_wTrapCount;i++)
	{
		cAreaTrapInfo	*lpTrapArea	=	(cAreaTrapInfo	*)m_pArea[m_awTrapList[i]];

		lpTrapArea->operate1SecondWork();
	}
}

void
cArea::operate1MinuteWork()
{
	int	i;

	for (i=0;i<m_wDoorCount;i++)
	{
		cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)m_pArea[m_awDoorList[i]];

		lpDoor->operate1MinuteWork();
	}
}

cAreaTrapInfo*
cArea::getActiveTrap(int _iX,int _iY)
{
	for (int i=0;i<m_wTrapCount;i++)
	{
		cAreaInfo		*lpArea	=	m_pArea[m_awTrapList[i]];
		cAreaTrapInfo	*lpTrap	=	(cAreaTrapInfo	*)lpArea;

		if (lpTrap->m_wIsActive	==	FALSE)
			continue;

		if (lpArea->isIn(_iX,_iY))
			return	lpTrap;
	}

	return	NULL;
}

cAreaTrapInfo*
cArea::getFieldTrap(int _iTrapIndex)
{
	if (_iTrapIndex<0 || _iTrapIndex >= m_wTrapCount)
		return	NULL;

	return	(cAreaTrapInfo	*)m_pArea[m_awTrapList[_iTrapIndex]];
}

cAreaTrapInfo*
cArea::getFieldTrapBySerial(int _iTrapIndex)
{
	if (_iTrapIndex<0 || _iTrapIndex >= dMAX_AREA)
		return	NULL;

	cAreaInfo	*lpArea	=	m_pArea[_iTrapIndex];

	if (lpArea->m_wKind	!=	dAREA_TRAP)
		return	NULL;

	return	(cAreaTrapInfo	*)lpArea;
}