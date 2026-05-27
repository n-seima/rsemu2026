#include "CEvent.h"
#include "CGame.h"
#include "CDecoder.h"
#include	"itemPackList.h"

CEventManager		g_eventManager;
CLottoEventManager	g_lottoEventManager;
CEventTimeManager	g_eventTimeManager;

int				g_iBoostExpByGoldRush				=	100;	
int				g_iCorrectItemDropChanceByGoldRush	=	100;

int				g_iBoostExpByTreasureMapEventCount			=	0;
int				g_iBoostGoldByTreasureMapEventCount			=	0;
int				g_iBoostExpSecondByTreasureMapEventInServer	=	0;
int				g_iBoostGoldSecondByTreasureMapEventInServer=	0;
int				g_iHeavenRedStoneCount	=	0;
int				g_iHellRedStoneCount	=	0;
int				g_iRedDevilRedStoneCount=	0;
int				g_iRedDevilFactor		=	100;

int				g_iBoostExpByLeadersSoundofBell				=	100;
int				g_iBoostItemDropChanceByLeadersSoundofBell	=	100;

int				g_iCurrentSecondTime	=	0;
int				g_iPasteSecondTime		=	0;
BOOL			g_bIsCompleteReceiveLottoEventData	=	FALSE;
BOOL			g_bIsCompleteReceiveEventTimeData	=	FALSE;

CEventTimeManager::CEventTimeManager()
{
	m_iCount	=	0;
	memset(m_adwEventTimeActiveBuffer,0,sizeof(m_adwEventTimeActiveBuffer));
}

void
CEventTimeManager::updateData(int _iCount,cEventTimeDefine *_lpData)
{
	m_iCount	=	_iCount;
	memcpy(m_aEventTimeList,_lpData,sizeof(cEventTimeDefine)*_iCount);

	for (int i=0;i<m_iCount;i++)
	{
		cEventTimeDefine	*lpData	=	&m_aEventTimeList[i];

		lpData->m_iBeginTimeMinuteValue	=	lpData->m_timeBegin.getMinuteValue();
		lpData->m_iEndTimeMinuteValue	=	lpData->m_timeEnd.getMinuteValue();
		lpData->m_bIsActive				=	FALSE;
	}
}

BOOL
CEventTimeManager::isEventTime(int _iEventIndex)
{
	if	(_iEventIndex	<0	||	_iEventIndex	>=	m_iCount)
		return	FALSE;

	cEventTimeDefine	*lpData	=	&m_aEventTimeList[_iEventIndex];

	return	lpData->m_bIsActive;
}

BOOL
CEventTimeManager::update()
{
	// 매분마다 이벤트 타임체크 한다!
	BOOL	bIsEventTimeChange	=	FALSE;

	for (int i=0;i<m_iCount;i++)
	{
		cEventTimeDefine	*lpData		=	&m_aEventTimeList[i];
		BOOL				bIsActive	=	FALSE;

	if(lpData->m_iBeginTimeMinuteValue <= g_iCurrentTimeMinuteValue && g_iCurrentTimeMinuteValue	<= lpData->m_iEndTimeMinuteValue)
		bIsActive	=	TRUE;

		if(lpData->m_bIsActive != bIsActive)
		{
			lpData->m_bIsActive		=	bIsActive;
			bIsEventTimeChange		=	TRUE;
			m_adwEventTimeActiveBuffer[i/dEVENT_TIME_BUFFER_BIT] = m_adwEventTimeActiveBuffer[i/dEVENT_TIME_BUFFER_BIT] ^ 1<<i/dEVENT_TIME_BUFFER_BIT;

		}		
	}

	return bIsEventTimeChange;
}

CLottoEventManager::CLottoEventManager()
{
	m_iCount	=	0;
}

void
CLottoEventManager::update()
{
	for (int i=0;i<m_iCount;i++)
	{
		cLottoItemDropEvent	*lpData	=	&m_aEventList[i];

		lpData->m_bIsActiveEvent	=	FALSE;

		if	(g_iCurrentTimeMinuteValue	<	lpData->m_iBeginTimeValue)
			continue;
		if	(g_iCurrentTimeMinuteValue	>=	lpData->m_iEndTimeValue)
			continue;

		lpData->m_bIsActiveEvent	=	TRUE;
	}
}

void
CLottoEventManager::updateData(int _iCount,cLottoItemDropEvent *_lpData)
{
	m_iCount	=	_iCount;
	memcpy(m_aEventList,_lpData,sizeof(cLottoItemDropEvent)*_iCount);

	for (int i=0;i<m_iCount;i++)
	{
		cLottoItemDropEvent	*lpData	=	&m_aEventList[i];

		CTimeInfo	checkTime;

		checkTime.m_wYear	=	lpData->m_utBegin.year+2000;
		checkTime.m_wMonth	=	lpData->m_utBegin.month;
		checkTime.m_wDay	=	lpData->m_utBegin.day;
		checkTime.m_wHour	=	lpData->m_utBegin.hour;
		checkTime.m_wMinute	=	lpData->m_utBegin.minute;

		lpData->m_iBeginTimeValue	=	checkTime.getMinuteValue(2000);
		
		checkTime.m_wYear	=	lpData->m_utEnd.year+2000;
		checkTime.m_wMonth	=	lpData->m_utEnd.month;
		checkTime.m_wDay	=	lpData->m_utEnd.day;
		checkTime.m_wHour	=	lpData->m_utEnd.hour;
		checkTime.m_wMinute	=	lpData->m_utEnd.minute;

		lpData->m_iEndTimeValue	=	checkTime.getMinuteValue(2000);
	}
}

void
CLottoEventManager::receiveLottoBox(cACTOR *_lpPlayer,int _iPlayerLevel,int _iMonsterLevel)
{
	for (int i=0;i<m_iCount;i++)
	{
		cLottoItemDropEvent	*lpData	=	&m_aEventList[i];

		if	(lpData->m_bIsActiveEvent	==	FALSE)
			continue;

		if	(_iMonsterLevel	<	lpData->m_iMinMobLevel	||		_iMonsterLevel	>	lpData->m_iMaxMobLevel)
			continue;

		if	(_iMonsterLevel	<	_iPlayerLevel+lpData->m_iLevelRange)
			continue;

		if	(largeRandom(1000000)	>	lpData->m_iDropChance)
			continue;

		int	iDropCount	=	1;

		if	(largeRandom(1000000)	<=	lpData->m_iCriticalChance)
			iDropCount	=	lpData->m_iCriticalCount;

		cItem	item;

		if	(!g_itemPackList.generatePackItem(&item,lpData->m_iLottoBoxSerial,iDropCount))
			continue;

		_lpPlayer->sendPickItemLog(&item,ePIM_LOTTO_EVENT);
		_lpPlayer->sendAddItem(&item);

		if	(item.isMoneyItem())
			_lpPlayer->addGoldItemToInventory(&item,eGET_GOLD_BY_LOTTOBOX);
		else
			_lpPlayer->addItemToInventory(&item);
	}
}

void
cGoldRush::updateInfo(UTime *_lpBegin,UTime *_lpEnd,int _iBoostExperience,int _iCorrectItemDropChance,char *_lpstrBeginComment,char *_lpstrEndComment)
{
	if	(m_bIsForcedSetting)
		return;

	memcpy(&m_begin,_lpBegin,sizeof(UTime));
	memcpy(&m_end,_lpEnd,sizeof(UTime));

	m_wBoostExperience		=	_iBoostExperience;
	m_wCorrectItemDropChance=	_iCorrectItemDropChance;

	strcpy(m_strBeginComment,_lpstrBeginComment);
	strcpy(m_strEndComment,_lpstrEndComment);
}

void
cGoldRush::update()
{
	CTimeInfo	currentTime;

	currentTime.update();
	currentTime.m_wYear		-=	2000;

	DWORD	dwCurrentTime	=	currentTime.m_wYear*60*24*31*12+currentTime.m_wMonth*60*24*31+currentTime.m_wDay*60*24+currentTime.m_wHour*60+currentTime.m_wMinute;
	DWORD	dwBeginTime		=	m_begin.year*60*24*31*12+m_begin.month*60*24*31+m_begin.day*60*24+m_begin.hour*60+m_begin.minute;
	DWORD	dwEndTime		=	m_end.year*60*24*31*12+m_end.month*60*24*31+m_end.day*60*24+m_end.hour*60+m_end.minute;

	if (dwCurrentTime		>=	dwBeginTime	&&	dwCurrentTime		<	dwEndTime)
	{
		g_iBoostExpByGoldRush				=	max(m_wBoostExperience,100);
		g_iCorrectItemDropChanceByGoldRush	=	max(m_wCorrectItemDropChance,100);
	}
	else
	{
		g_iBoostExpByGoldRush				=	100;
		g_iCorrectItemDropChanceByGoldRush	=	100;
	}

	if (dwCurrentTime	==	dwBeginTime)
	{
		if (m_wIsSendBeginNotice==	FALSE)
			if (m_strBeginComment[0])
				g_game.notice(m_strBeginComment);

		m_wIsSendBeginNotice	=	TRUE;
	}
	else
		m_wIsSendBeginNotice	=	FALSE;

	if (dwCurrentTime		==	dwEndTime)
	{
		if (m_wIsSendEndNotice	==	FALSE)
			if (m_strEndComment[0])
				g_game.notice(m_strEndComment);

		m_wIsSendEndNotice	=	TRUE;
	}
	else
		m_wIsSendEndNotice	=	FALSE;
}

//
//	모든 이벤트 업데이트
void
CEventManager::update()
{
	m_oxQuiz.update();
	m_wordQuiz.update();
	m_itemSale.update();
	m_itemDrop.update();
	m_plusDropGold.update();
	m_goldRush.update();
	g_lottoEventManager.update();

	int	iCurrentSecond	=	g_iCurrentTimeSecondValue;

 	g_iPasteSecondTime	=	0;

	if	(g_iCurrentSecondTime	==	0)
		g_iCurrentSecondTime	=	iCurrentSecond;

	if	(iCurrentSecond	!=	g_iCurrentSecondTime)
	{
		g_iPasteSecondTime		=	iCurrentSecond-g_iCurrentSecondTime;
		g_iCurrentSecondTime	=	iCurrentSecond;
	}

	if	(g_iPasteSecondTime)
	{
		if	(m_iUpkeepSecondForSoundOfLeadersBell)
		{
			m_iUpkeepSecondForSoundOfLeadersBell			-=	g_iPasteSecondTime;
			m_iUpkeepSecondForSoundOfLeadersBell			=	max(m_iUpkeepSecondForSoundOfLeadersBell,0);
		}
	}
}

int
CEventManager::setRedStonCount(int _iHeavenRedStoneCount,int _iHellRedStoneCount,int _iRedDevilRedStoneCount)
{
	int	iPart	=	-1;

	if	(g_iHeavenRedStoneCount	<	_iHeavenRedStoneCount)
		iPart	=	0;
	else
	if	(g_iHellRedStoneCount	<	_iHellRedStoneCount)
		iPart	=	1;
	else
	if	(g_iRedDevilRedStoneCount<	_iRedDevilRedStoneCount)
		iPart	=	2;

	g_iHeavenRedStoneCount	=	_iHeavenRedStoneCount;
	g_iHellRedStoneCount	=	_iHellRedStoneCount;
	g_iRedDevilRedStoneCount=	_iRedDevilRedStoneCount;

	g_iRedDevilFactor		=	getRedDevilFactor();

	return	iPart;
}


int
CEventManager::getRedDevilFactor()
{
	int	iAverage	=	(g_iHeavenRedStoneCount	+	g_iHellRedStoneCount)/2;

	if	(g_iRedDevilRedStoneCount	==	0	||	g_iRedDevilRedStoneCount	<=	iAverage)
		return	100;

	if	(g_iRedDevilRedStoneCount <= iAverage+ iAverage*10/100)
	{
		return	110;
	}
	if	(g_iRedDevilRedStoneCount <= iAverage+ iAverage*25/100)
	{
		return	120;
	}
	if	(g_iRedDevilRedStoneCount <= iAverage+ iAverage*50/100)
	{
		return	140;
	}
	if	(g_iRedDevilRedStoneCount <= iAverage+ iAverage*75/100)
	{
		return	170;
	}
	if	(g_iRedDevilRedStoneCount <= iAverage+ iAverage)
	{
		return	200;
	}

	return	250;
}

void
CEventManager::setDBControlledEventValue(int *_lpiValues)
{
	memcpy(m_aiDBControlledEventValue,_lpiValues,sizeof(m_aiDBControlledEventValue));
}

void
CEventManager::changeDBControlledEventValue(int _iEvent,int _iValue)
{
	m_aiDBControlledEventValue[_iEvent]	=	_iValue;
}

//	초기화
void
CEventManager::init()
{
	m_bIsReceiveGoldRushInfo	=	FALSE;
	
	m_oxQuiz.reset();
	m_plusDropGold.reset();
	m_itemSale.reset();
	m_itemDrop.reset();
	m_redStonePointRank.init();
	m_goldRush.m_bIsForcedSetting	=	FALSE;

	readEventData();
}

//	이벤트 정보 읽어오기
void
CEventManager::readEventData()
{
	m_itemSale.readData();
	m_plusDropGold.readData();
	m_itemDrop.readData();
}


void
cRedStonePointRank::init()
{
	reset();
}

void
cRedStonePointRank::setCount(int _iCount)
{
	m_wCount	=	_iCount;

	for (int i=m_wCount;i;)
	{
		--i;

		cUser	*lpUser	=	&m_aList[i];

		CLOG("redstone_point","%s rank %d,title %d",lpUser->m_strName,lpUser->m_wRank,lpUser->m_wTitleLevel);
	}
}

void
cRedStonePointRank::reset()
{
	m_wCount	=	0;
}

int
cRedStonePointRank::getTitleLevel(char *_lpstrName)
{
	cUser*	lpUser	=	_get(_lpstrName);

	if	(!lpUser)
		return	0;
	
	return	lpUser->m_wTitleLevel;
}

cRedStonePointRank::cUser*
cRedStonePointRank::_get(char *_lpstrName)
{
	DWORD	dwNameHashCode	=	GetHashCode(_lpstrName);

	for (int i=m_wCount;i;)
	{
		--i;

		cUser	*lpUser	=	&m_aList[i];

		if	(lpUser->m_dwNameHashCode	!=	dwNameHashCode)
			continue;

		if	(STRICMP(lpUser->m_strName,_lpstrName)	!=	0)
			continue;

		return	lpUser;
	}

	return	NULL;
}

void
cRedStonePointRank::setRank(int _iIndex,int _iRank,char *_lpstrName)
{
	cUser	*lpUser	=	&m_aList[_iIndex];

	lpUser->m_dwNameHashCode	=	GetHashCode(_lpstrName);

	memcpy(lpUser->m_strName,_lpstrName,dNAME_LENGTH);
	lpUser->m_strName[dNAME_LENGTH-1]	=	NULL;
	lpUser->m_wRank						=	_iRank;

	if	(_iRank	==	1)
		lpUser->m_wTitleLevel	=	10;
	else
	if	(_iRank	>=	2	&&	_iRank	<=	3	)
		lpUser->m_wTitleLevel	=	9;
	else
	if	(_iRank	>=	4	&&	_iRank	<=	5	)
		lpUser->m_wTitleLevel	=	8;
	else
	if	(_iRank	>=	6	&&	_iRank	<=	11	)
		lpUser->m_wTitleLevel	=	7;
	else
	if	(_iRank	>=	12	&&	_iRank	<=	18	)
		lpUser->m_wTitleLevel	=	6;
	else
	if	(_iRank	>=	19	&&	_iRank	<=	29	)
		lpUser->m_wTitleLevel	=	5;
	else
	if	(_iRank	>=	30	&&	_iRank	<=	41	)
		lpUser->m_wTitleLevel	=	4;
	else
	if	(_iRank	>=	42	&&	_iRank	<=	57	)
		lpUser->m_wTitleLevel	=	3;
	else
	if	(_iRank	>=	58	&&	_iRank	<=	74	)
		lpUser->m_wTitleLevel	=	2;
	else
	if	(_iRank	>=	75	&&	_iRank	<=	100	)
		lpUser->m_wTitleLevel	=	1;
	else
		lpUser->m_wTitleLevel	=	0;
}

//
//	크리스마스, 이구동성 이벤트 
void
CEventManager::operateWordEventPlayer(cACTOR* _lpChatter)
{	// 마지막 수정일 : 09.10.12
	CTimeInfo	currentTime;
	
	currentTime.update();
	currentTime.m_wYear		-=	2000;
	
	DWORD	dwCurrentTime	=	currentTime.m_wYear*60*24*31*12+currentTime.m_wMonth*60*24*31+currentTime.m_wDay*60*24+currentTime.m_wHour*60+currentTime.m_wMinute;
	DWORD	dwBeginTime		=	m_wordCompareEvent.m_wBeginTimeYear*60*24*31*12+m_wordCompareEvent.m_wBeginTimeMonth*60*24*31+m_wordCompareEvent.m_wBeginTimeDay*60*24+m_wordCompareEvent.m_wBeginTimeHour*60+m_wordCompareEvent.m_wBeginTimeMinite;
	DWORD	dwEndTime		=	m_wordCompareEvent.m_wEndTimeYear*60*24*31*12+m_wordCompareEvent.m_wEndTimeMonth*60*24*31+m_wordCompareEvent.m_wEndTimeDay*60*24+m_wordCompareEvent.m_wEndTimeHour*60+m_wordCompareEvent.m_wEndTimeMinite;
	DWORD	dwActiveTime	=	m_wordCompareEvent.m_dwTimeMinite*60;//초(Second) 개념이라 * 60을 한다.//위변수들은 분(minite)개념이당.

	if((dwEndTime - dwCurrentTime < m_wordCompareEvent.m_dwTimeMinite )&&(dwEndTime - dwCurrentTime > 0))	//종료 하기 전에 효과를 받는다.
		dwActiveTime = (dwEndTime - dwCurrentTime)*60;

	if (_lpChatter == NULL)
		return;

	if (_lpChatter->m_bf3EventCounter)
		return;
	
	switch(m_wordCompareEvent.m_bf16EventKind)
	{
	case eEVENT_UNANIMOUS :			// 이구동성 이벤트.
		_lpChatter->m_wWordEventTime = dwActiveTime;
		_lpChatter->m_bf3EventCounter++;
		break;
	case eEVENT_CHRISTMAS:			// 크리스마스 이벤트.
		{
			if(!christMasEvent(_lpChatter))
				return;
			break;		
		}
	}

	_lpChatter->sendServerMessage(m_wordCompareEvent.m_strActiveComment);
	_lpChatter->buildPower();			


}
BOOL
CEventManager::christMasEvent(cACTOR* _lpChatter)
{	// 크리스마스 이벤트
	// 마지막 수정일 : 09.11.03
	
	cFIELD* lpField = g_game.getFieldByUniqueSerial(_lpChatter->m_wCurrentField);
	
	if(!lpField || lpField->m_wWeather != eWEATHER_SNOW || !lpField->m_wIsVillage)
		return FALSE;
	
	cAbility		ability;
	DWORD dwTime = m_wordCompareEvent.m_dwTimeMinite * 60 * 100;
	int iItemSerial = m_wordCompareEvent.m_bf16BuffSerial;
	cItem	cBuffItem;
	g_im.generateItemByBaseItem(&cBuffItem, iItemSerial ,1);
	cBasicItem*	lpBasic = cBuffItem.getBasicItem();
	if(!lpBasic)
		return FALSE;
	
	if(lpBasic->m_aUniqueData[0].m_wEffect == 0xffff)
		return FALSE;
	
	for (int i=0;i<dITEM_UNIQUE_DATA_COUNT;++i)
	{
		if(lpBasic->m_aUniqueData[i].m_wEffect == 0xffff || lpBasic->m_aUniqueData[i].m_wEffect != eIP_BUFF_SKILL_CASTING)
			continue;

		cAbility		ability;

		ability.m_wSkill	= lpBasic->m_aUniqueData[i].m_aValue[0];
		ability.m_wLevel	= lpBasic->m_aUniqueData[i].m_aValue[1];
		int	iStatus			= lpBasic->m_aUniqueData[i].m_aValue[2];
		
		cACTOR cTempActor;
		cTempActor.m_strength.m_sFixValue		= iStatus;
		cTempActor.m_agility.m_sFixValue		= iStatus;
		cTempActor.m_constitution.m_sFixValue	= iStatus;
		cTempActor.m_intelligence.m_sFixValue	= iStatus;
		cTempActor.m_wisdom.m_sFixValue			= iStatus;
		cTempActor.m_charisma.m_sFixValue		= iStatus;
		cTempActor.m_luck.m_sFixValue			= iStatus;
		
		_lpChatter->applySkillExtraEffects(&cTempActor,&ability,0,_lpChatter->m_pos.x, _lpChatter->m_pos.y, FALSE, FALSE, dwTime);
		
	}
	
	return TRUE;

}

BOOL
CEventManager::checkTwoMouthOneSoundEventTime(int _iPreMinute)
{
	CTimeInfo	currentTime,beginTime,endTime;
	
	currentTime.update();
	beginTime.reset();
	endTime.reset();

	beginTime.m_wYear		=	m_wordCompareEvent.m_wBeginTimeYear+2000;
	beginTime.m_wMonth		=	m_wordCompareEvent.m_wBeginTimeMonth;
	beginTime.m_wDay		=	m_wordCompareEvent.m_wBeginTimeDay;
	beginTime.m_wHour		=	m_wordCompareEvent.m_wBeginTimeHour;
	beginTime.m_wMinute		=	m_wordCompareEvent.m_wBeginTimeMinite;
	beginTime.m_wSecond		=	0;

	endTime.m_wYear			=	m_wordCompareEvent.m_wEndTimeYear+2000;
	endTime.m_wMonth		=	m_wordCompareEvent.m_wEndTimeMonth;
	endTime.m_wDay			=	m_wordCompareEvent.m_wEndTimeDay;
	endTime.m_wHour			=	m_wordCompareEvent.m_wEndTimeHour;
	endTime.m_wMinute		=	m_wordCompareEvent.m_wEndTimeMinite;
	endTime.m_wSecond		=	0;

	DWORD	dwCurrentTime	=	currentTime.getMinuteValue(2000);
	DWORD	dwBeginTime		=	beginTime.getMinuteValue(2000);
	DWORD	dwEndTime		=	endTime.getMinuteValue(2000);

	if	(dwCurrentTime+_iPreMinute	<	dwBeginTime	)
		return	TRUE;

	if	(dwCurrentTime	>	dwEndTime+m_wordCompareEvent.m_dwTimeMinite)	//	이벤트 끝나는 시간+효과 지속시간보다 크면 OK
		return	TRUE;

	return	FALSE;
}

BOOL
CEventManager::checkGoldRushEventTime(int _iPreMinute)
{
	CTimeInfo	currentTime,beginTime,endTime;
	
	currentTime.update();
	beginTime.reset();
	endTime.reset();

	beginTime.m_wYear		=	m_goldRush.m_begin.year+2000;
	beginTime.m_wMonth		=	m_goldRush.m_begin.month;
	beginTime.m_wDay		=	m_goldRush.m_begin.day;
	beginTime.m_wHour		=	m_goldRush.m_begin.hour;
	beginTime.m_wMinute		=	m_goldRush.m_begin.minute;
	beginTime.m_wSecond		=	0;

	endTime.m_wYear			=	m_goldRush.m_end.year+2000;
	endTime.m_wMonth		=	m_goldRush.m_end.month;
	endTime.m_wDay			=	m_goldRush.m_end.day;
	endTime.m_wHour			=	m_goldRush.m_end.hour;
	endTime.m_wMinute		=	m_goldRush.m_end.minute;
	endTime.m_wSecond		=	0;

	DWORD	dwCurrentTime	=	currentTime.getMinuteValue(2000);
	DWORD	dwBeginTime		=	beginTime.getMinuteValue(2000);
	DWORD	dwEndTime		=	endTime.getMinuteValue(2000);

	if	(dwCurrentTime+_iPreMinute	<	dwBeginTime	)
		return	TRUE;

	if	(dwCurrentTime	>	dwEndTime)	//	이벤트 끝나는 시간 크면 OK
		return	TRUE;

	return	FALSE;
}

BOOL
cEventWord::CheckWordEventAppointment()
{
	CTimeInfo	currentTime;
	
	currentTime.update();
	currentTime.m_wYear		-=	2000;

	DWORD	dwCurrentTime	=	currentTime.m_wYear*60*24*31*12+currentTime.m_wMonth*60*24*31+currentTime.m_wDay*60*24+currentTime.m_wHour*60+currentTime.m_wMinute;
	DWORD	dwBeginTime		=	m_wBeginTimeYear*60*24*31*12+m_wBeginTimeMonth*60*24*31+m_wBeginTimeDay*60*24+m_wBeginTimeHour*60+m_wBeginTimeMinite;
	DWORD	dwEndTime		=	m_wEndTimeYear*60*24*31*12+m_wEndTimeMonth*60*24*31+m_wEndTimeDay*60*24+m_wEndTimeHour*60+m_wEndTimeMinite;

	if (dwCurrentTime		>=	dwBeginTime	&&	dwCurrentTime		<	dwEndTime)
		m_bIsActiveWordCompareEvent = TRUE;
	else
		m_bIsActiveWordCompareEvent = FALSE;
	
	if (dwCurrentTime	==	dwBeginTime)
	{
		if (m_wIsSendBeginNotice ==	FALSE)
			if (m_strBeginComment[0])
				g_game.notice(m_strBeginComment);
			
			m_wIsSendBeginNotice	=	TRUE;
	}
	else
		m_wIsSendBeginNotice	=	FALSE;
	
	if (dwCurrentTime		==	dwEndTime)
	{
		if (m_wIsSendEndNotice	==	FALSE)
			if (m_strEndComment[0])
				g_game.notice(m_strEndComment);
			
			m_wIsSendEndNotice	=	TRUE;
	}
	else
		m_wIsSendEndNotice	=	FALSE;

	return TRUE;
}