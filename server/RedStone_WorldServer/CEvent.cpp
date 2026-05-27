#include "CEvent.h"
#include "cDECODER.h"
#include "operate_packet.h"
#include "Client.h"

CEventManager				g_eventManager;
cRenewPremiumItemExpireTime	g_renewPremiumItemExpireTime;
cItemPackManager			g_itemPackManager;
cGoldRush					g_goldRush;
cCarrotShop					g_carrotShopInfo;
cDuelRankerManager			g_duelRankerManager;
cDuelTeamManager			g_duelTeamManager;
CLottoEventManager			g_lottoEventManager;
CEventTimeManager			g_eventTimeManager;

int							g_iBoostGoldByTreasureMapEventCount	=	0;
int							g_iBoostExpByTreasureMapEventCount	=	0;

int							g_iHeavenRedStoneCount	=	0;
int							g_iHellRedStoneCount	=	0;
int							g_iRedDevilRedStoneCount=	0;

CEventTimeManager::CEventTimeManager()
{
	memset(this,0,sizeof(CEventTimeManager));
}

void
CEventTimeManager::updateData(int _iCount,cEventTimeDefine *_lpData)
{
	m_iCount	=	_iCount;

	memcpy(m_aEventTimeList,_lpData,sizeof(cEventTimeDefine)*_iCount);

	sendEventData(NULL);
}

void
CEventTimeManager::sendEventData(CClient *_lpTargetServer)
{
	SERVERStoWORLD_MODIFY_EVENT_TIME	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_EVENT_TIME_DATA);

	packet.wCount		=	m_iCount;
	memcpy(packet.aEventList,m_aEventTimeList,sizeof(cEventTimeDefine)*m_iCount);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aEventList)+sizeof(cEventTimeDefine)*m_iCount;

	mSEND_GSP(_lpTargetServer,packet);
}

void
CLottoEventManager::load()
{
	cFILE	file;

	m_iCount	=	0;

	if	(file.Open(_ms("%s/data/LottoEvent.dat",g_strLogFolder),"rb"))
	{
		file.Read(&m_iCount,sizeof(int));
		file.Read(m_aEventList,sizeof(cLottoItemDropEvent)*m_iCount);
		file.Close();
	}
}

void
CLottoEventManager::save()
{
	cFILE	file;

	if	(file.Open(_ms("%s/data/LottoEvent.dat",g_strLogFolder),"wb"))
	{
		file.Write(&m_iCount,sizeof(int));
		file.Write(m_aEventList,sizeof(cLottoItemDropEvent)*m_iCount);
		file.Close();
	}
}

void
CLottoEventManager::updateData(int _iCount,cLottoItemDropEvent *_lpData)
{
	m_iCount	=	_iCount;
	memcpy(m_aEventList,_lpData,sizeof(cLottoItemDropEvent)*_iCount);

	save();

	sendEventData(NULL);
}

void
CLottoEventManager::sendEventData(CClient *_lpTargetServer)
{
	WORLDtoSERVERS_LOTTO_EVENT_DATA	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_LOTTO_EVENT_DATA);

	packet.wCount		=	m_iCount;
	memcpy(packet.aEventList,m_aEventList,sizeof(cLottoItemDropEvent)*m_iCount);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aEventList)+sizeof(cLottoItemDropEvent)*m_iCount;

	mSEND_GSP(_lpTargetServer,packet);
}


CEventManager::CEventManager()
{
	m_iChangeSystemTimeMinute	=	0;
	m_bIsCancelSoundofLeadersBellEvent	=	FALSE;
}

void
CEventManager::updateDBControlledEventValue(int _iEvent,int _iValue)
{
	m_aiDBControlledEventValue[_iEvent]	=	_iValue;
}

void
CEventManager::updateDBControlledEventValue(int *_lpiValues)
{
	memcpy(m_aiDBControlledEventValue,_lpiValues,sizeof(m_aiDBControlledEventValue));
}

void
CEventManager::init()
{
	m_iIsSendTrialGameResultMail	=	FALSE;
	m_iTrialGameTeamSize			=	2;
	m_iOldSecondCounter				=	-1;

	memset(&m_timeTrialGameBegin,0,sizeof(m_timeTrialGameBegin));
	memset(&m_timeTrialGameEnd,0,sizeof(m_timeTrialGameEnd));
	memset(&m_timeOfflineTournamentBegin,0,sizeof(m_timeOfflineTournamentBegin));
	memset(&m_timeOfflineTournamentEnd,0,sizeof(m_timeOfflineTournamentEnd));
	memset(&m_timeLastUpdate,0,sizeof(m_timeLastUpdate));

	strcpy(m_strTrialGameName,"Trial Game");

	m_oxQuiz.reset();
	m_oxQuiz.load();
	m_wordQuiz.reset();
	m_wordQuiz.load();
	m_wordCompareEvent.reset();
	m_wordCompareEvent.load();

	memset(m_aiDBControlledEventValue,0,sizeof(m_aiDBControlledEventValue));

	cFILE	file;

	if	(file.Open(_ms("%s/data/redStone.dat",g_strLogFolder),"rb"))
	{
		file.Read(&g_iHeavenRedStoneCount,sizeof(int));
		file.Read(&g_iHellRedStoneCount,sizeof(int));
		file.Read(&g_iRedDevilRedStoneCount,sizeof(int));
		file.Close();
	}

	if	(file.Open(_ms("%s/data/WorldValueList.dat",g_strLogFolder),"rb"))
	{
		file.Read(&g_aiWorldValue,sizeof(int) * dMAX_WORLD_VALUE_MAX);
		file.Close();
	}

	g_lottoEventManager.load();

	m_iRemainSoundofBellTime				=	0;
	m_iBoostExpByLeadersSoundofBell			=	100;
	m_iBoostItemDropChanceByLeadersSoundofBell=	100;
	memset(&m_timeLastSoundOfLeadersBell,0,sizeof(m_timeLastSoundOfLeadersBell));
	memset(m_strSoundOfLeadersBellOwner,0,sizeof(m_strSoundOfLeadersBellOwner));
	memset(m_strSoundOfLeadersBellMessage,0,sizeof(m_strSoundOfLeadersBellMessage));
	m_bIsDisplayNameWithSoundofLeadersBell	=	FALSE;

	if	(file.Open(_ms("%s/data/soundOfLeadersBell.dat",g_strLogFolder),"rb"))
	{
		file.Read(&m_iRemainSoundofBellTime,sizeof(int));
		file.Read(&m_iBoostExpByLeadersSoundofBell,sizeof(int));
		file.Read(&m_iBoostItemDropChanceByLeadersSoundofBell,sizeof(int));
		file.Read(&m_timeLastSoundOfLeadersBell,sizeof(m_timeLastSoundOfLeadersBell));
		file.Read(&m_strSoundOfLeadersBellOwner,sizeof(m_strSoundOfLeadersBellOwner));
		file.Read(&m_strSoundOfLeadersBellMessage,sizeof(m_strSoundOfLeadersBellMessage));
		file.Read(&m_bIsDisplayNameWithSoundofLeadersBell,sizeof(m_bIsDisplayNameWithSoundofLeadersBell));

		file.Close();
	}

	cDECODER	decoder;

	if	(decoder.Upload("recoverRSC.txt",NULL))
	{
		int	iHeavenCount	=	decoder.GetNumber();
		int	iHellCount		=	decoder.GetNumber();
		int	iRedDevilCount	=	decoder.GetNumber();

		g_iHeavenRedStoneCount	=	iHeavenCount;
		g_iHellRedStoneCount	=	iHellCount;
		g_iRedDevilRedStoneCount=	iRedDevilCount;

		DeleteFile("recoverRSC.txt");

		cMSG::Put("Red Stone Count","Heaven : %d\nHell : %d\nRed Devil : %d",g_iHeavenRedStoneCount,g_iHellRedStoneCount,g_iRedDevilRedStoneCount);

		increaseRedStoneCount(-1);	//	저장
	}

	if	(decoder.Upload("recoverWValue.txt",NULL))
	{
		for (int i = 0 ; i < dMAX_WORLD_VALUE_MAX ; i++)
		{
			g_aiWorldValue[i] = decoder.GetNumber();
		}
		
		DeleteFile("recoverWValue.txt");
		
		cMSG::Put("WorldValue Setting Complete","WorldValue Setting Complete");
	}

	g_renewPremiumItemExpireTime.init();
	g_itemPackManager.init();
	g_goldRush.init();
	g_carrotShopInfo.init();
}

void
CEventManager::update()
{
	//	초단위 시간 카운터
	CTimeInfo	curTime;

	curTime.update();

	m_iCurrentSecondCount	=	curTime.getSecondValue();

	int	iOldSecond			=	m_timeLastUpdate.getSecondValue();

	if	(iOldSecond	==	0)
		iOldSecond			=	m_iCurrentSecondCount;

	m_iPastSecondCounter	=	m_iCurrentSecondCount-iOldSecond;

	memcpy(&m_timeLastUpdate,&curTime,sizeof(curTime));

	updateSoundOfLeadersBell();
}

void
CEventManager::updateSoundOfLeadersBell()
{
	if	(m_iPastSecondCounter	==	0)
		return;

	if	(m_iRemainSoundofBellTime	==	0)
	{
		m_bIsCancelSoundofLeadersBellEvent	=	FALSE;
		return;
	}

	if	(m_iRemainSoundofBellTime)
	{
		m_iRemainSoundofBellTime	-=	m_iPastSecondCounter;

		if	(m_bIsCancelSoundofLeadersBellEvent)
			m_iRemainSoundofBellTime	=	0;

		if	(m_iRemainSoundofBellTime	<=	0)
		{
			m_iRemainSoundofBellTime				=	0;
			m_iBoostExpByLeadersSoundofBell			=	100;
			m_iBoostItemDropChanceByLeadersSoundofBell=	100;

			WORLDtoSERVERS_SOUND_OF_LEADERS_BELL	packet;

			packet.base.set(sizeof(packet),dWORLDtoSERVERS_SOUND_OF_LEADERS_BELL);
			packet.iBoostExp			=	m_iBoostExpByLeadersSoundofBell;
			packet.iBoostItemDropChance	=	m_iBoostItemDropChanceByLeadersSoundofBell;

			packet.iUpkeepTime			=	0;
			packet.wDisplayName			=	m_bIsDisplayNameWithSoundofLeadersBell;
			packet.wItemIndex			=	0;
			packet.iActorSerial			=	-1;
			packet.wWork				=	1;	//	eSoundOfLeadersBellWork_end;

			strcpy(packet.strMessage,m_strSoundOfLeadersBellMessage);
			strcpy(packet.strCaster,m_strSoundOfLeadersBellOwner);

			CClient	*lpServer	=	NULL;

			mSEND_GSP(lpServer,packet);
		}
	}

	m_bIsCancelSoundofLeadersBellEvent	=	FALSE;

	cFILE	file;

	if	(file.Open(_ms("%s/data/soundOfLeadersBell.dat",g_strLogFolder),"wb"))
	{
		file.Write(&m_iRemainSoundofBellTime,sizeof(int));
		file.Write(&m_iBoostExpByLeadersSoundofBell,sizeof(int));
		file.Write(&m_iBoostItemDropChanceByLeadersSoundofBell,sizeof(int));
		file.Write(&m_timeLastSoundOfLeadersBell,sizeof(m_timeLastSoundOfLeadersBell));
		file.Write(&m_strSoundOfLeadersBellOwner,sizeof(m_strSoundOfLeadersBellOwner));
		file.Write(&m_strSoundOfLeadersBellMessage,sizeof(m_strSoundOfLeadersBellMessage));
		file.Write(&m_bIsDisplayNameWithSoundofLeadersBell,sizeof(m_bIsDisplayNameWithSoundofLeadersBell));

		file.Close();
	}
}

void
CEventManager::changeMsgSoundOfLeadersBell(char *_lpstrMsg)
{
	strcpy(m_strSoundOfLeadersBellMessage,_lpstrMsg);

	WORLDtoSERVERS_SOUND_OF_LEADERS_BELL	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_SOUND_OF_LEADERS_BELL);
	packet.iBoostExp			=	m_iBoostExpByLeadersSoundofBell;
	packet.iBoostItemDropChance	=	m_iBoostItemDropChanceByLeadersSoundofBell;

	if	(m_iRemainSoundofBellTime	==	0)
	{
		packet.iBoostExp			=	100;
		packet.iBoostItemDropChance	=	100;
	}

	packet.iUpkeepTime			=	m_iRemainSoundofBellTime;
	packet.wDisplayName			=	m_bIsDisplayNameWithSoundofLeadersBell;
	packet.iActorSerial			=	-1;
	packet.wItemIndex			=	0;
	packet.wWork				=	3;

	strcpy(packet.strMessage,m_strSoundOfLeadersBellMessage);
	strcpy(packet.strCaster,m_strSoundOfLeadersBellOwner);

	CClient *lpServer	=	NULL;

	mSEND_GSP(lpServer,packet);
}

void
CEventManager::resetSoundOfLedersBellTime(BOOL _bIsCancelEvent)
{
	memset(&m_timeLastSoundOfLeadersBell,0,sizeof(m_timeLastSoundOfLeadersBell));
	m_bIsCancelSoundofLeadersBellEvent	=	_bIsCancelEvent;
}

void
CEventManager::sendSoundOfLeadersBellEventInfo(CClient *_lpServer)
{
	CTimeInfo	curTime;
	int			iOldMinute		=	m_timeLastSoundOfLeadersBell.getMinuteValue(2000);
	int			iCurMinute		=	curTime.getMinuteValue(2000);
	int			iCoolTimeMinute	=	12*60-(iCurMinute-iOldMinute);

	iCoolTimeMinute	=	max(iCoolTimeMinute,0);

	WORLDtoSERVERS_SOUND_OF_LEADERS_BELL	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_SOUND_OF_LEADERS_BELL);
	packet.iBoostExp			=	m_iBoostExpByLeadersSoundofBell;
	packet.iBoostItemDropChance	=	m_iBoostItemDropChanceByLeadersSoundofBell;

	packet.iUpkeepTime			=	m_iRemainSoundofBellTime;
	packet.wDisplayName			=	m_bIsDisplayNameWithSoundofLeadersBell;

	packet.wBeginYear			=	m_timeLastSoundOfLeadersBell.m_wYear;
	packet.wBeginMonth			=	m_timeLastSoundOfLeadersBell.m_wMonth;
	packet.wBeginDay			=	m_timeLastSoundOfLeadersBell.m_wDay;
	packet.wBeginHour			=	m_timeLastSoundOfLeadersBell.m_wHour;
	packet.wBeginMinute			=	m_timeLastSoundOfLeadersBell.m_wMinute;	//	시작한 시간
	packet.wCoolTimeMinute		=	iCoolTimeMinute;	//	남은 쿨타임
	packet.wWork				=	0xffff;

	strcpy(packet.strMessage,m_strSoundOfLeadersBellMessage);
	strcpy(packet.strCaster,m_strSoundOfLeadersBellOwner);

	mSEND_GSP(_lpServer,packet);
}

void
CEventManager::useSoundOfLeadersBell(CClient *_lpServer,SERVERStoWORLD_SOUND_OF_LEADERS_BELL *_lpPacket)
{
	CTimeInfo	curTime;

	curTime.update();

	int	iOldMinute	=	m_timeLastSoundOfLeadersBell.getMinuteValue(2000);
	int	iCurMinute	=	curTime.getMinuteValue(2000);

	if	(iCurMinute-iOldMinute	<=	12*60)	//	아직 재사용 시간이 안됐다.
	{
		SendSimpleWork(_lpServer,eSWWG_REQUIRE_MORE_TIME_FOR_USE_SOUND_OF_LEADERS_BELL,12*60-(iCurMinute-iOldMinute),_lpPacket->iActorSerial,m_iRemainSoundofBellTime);

		return;
	}

	if	(_lpPacket->wIsTest)
	{
		SendSimpleWork(_lpServer,eSWWG_CONFIRM_USE_SOUND_OF_LEADERS_BELL,_lpPacket->iActorSerial);

		return;
	}

	m_iRemainSoundofBellTime			=	_lpPacket->iUpkeepMinute*60;

	m_bIsDisplayNameWithSoundofLeadersBell	=	_lpPacket->wDisplayName;
	
	memcpy(&m_timeLastSoundOfLeadersBell,&curTime,sizeof(curTime));

	m_iBoostExpByLeadersSoundofBell	=	_lpPacket->iBoostExp;
	m_iBoostItemDropChanceByLeadersSoundofBell	=	_lpPacket->iBoostItemDropChance;

	WORLDtoSERVERS_SOUND_OF_LEADERS_BELL	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_SOUND_OF_LEADERS_BELL);
	packet.iBoostExp			=	m_iBoostExpByLeadersSoundofBell;
	packet.iBoostItemDropChance	=	m_iBoostItemDropChanceByLeadersSoundofBell;

	packet.iUpkeepTime			=	m_iRemainSoundofBellTime;
	packet.wDisplayName			=	_lpPacket->wDisplayName;
	packet.wItemIndex			=	_lpPacket->wItemIndex;
	packet.iActorSerial			=	_lpPacket->iActorSerial;
	packet.wWork				=	0;	//	eSoundOfLeadersBellWork_begin

	strcpy(m_strSoundOfLeadersBellOwner,_lpPacket->strCaster);
	strcpy(m_strSoundOfLeadersBellMessage,_lpPacket->strMessage);

	strcpy(packet.strMessage,_lpPacket->strMessage);
	strcpy(packet.strCaster,_lpPacket->strCaster);

	_lpServer	=	NULL;

	mSEND_GSP(_lpServer,packet);
}

void
CEventManager::increaseRedStoneCount(int _iType)
{
	switch(_iType)
	{
		case	eRSRT_HEAVEN	:
			g_iHeavenRedStoneCount++;
			break;
		case	eRSRT_HELL		:
			g_iHellRedStoneCount++;
			break;
		case	eRSRT_RED_DEVIL	:
			g_iRedDevilRedStoneCount++;
			break;
	}

	cFILE	file;

	if	(file.Open(_ms("%s/data/redStone.dat",g_strLogFolder),"wb"))
	{
		file.Write(&g_iHeavenRedStoneCount,sizeof(int));
		file.Write(&g_iHellRedStoneCount,sizeof(int));
		file.Write(&g_iRedDevilRedStoneCount,sizeof(int));
		file.Close();
	}

	SendSimpleWork(NULL,eSWWG_UPDATE_RED_STONE_COUNT,g_iHeavenRedStoneCount,g_iHellRedStoneCount,g_iRedDevilRedStoneCount);
}

void
CEventManager::setOXQuizVillage(int _iVillage)
{
	if	(_iVillage	==	0xffff)
		return;

	m_oxQuiz.m_wGateVillage	=	_iVillage;
}

void
CEventManager::setWordQuizVillage(int _iVillage)
{
	if	(_iVillage	==	0xffff)
		return;

	m_wordQuiz.m_wGateVillage	=	_iVillage;
}

void
CEventManager::setQuizEventBeginTime(int _iType,int _iBeginYear,int _iBeginMonth,int _iBeginDay,int _iBeginHour,int _iBeginMinute,int _iLimitPlayerCount,BOOL _bIsTest)
{
	if	(_iType	==	eET_OX_QUIZ)
	{
		setOXQuizBeginTime(_iBeginYear,_iBeginMonth,_iBeginDay,_iBeginHour,_iBeginMinute,_iLimitPlayerCount,_bIsTest);
	}
	if	(_iType	==	eET_WORD_QUIZ)
	{
		setWordQuizBeginTime(_iBeginYear,_iBeginMonth,_iBeginDay,_iBeginHour,_iBeginMinute,_iLimitPlayerCount,_bIsTest);
	}
	
}

void
CEventManager::setOXQuizBeginTime(int _iBeginYear,int _iBeginMonth,int _iBeginDay,int _iBeginHour,int _iBeginMinute,int _iLimitPlayerCount,BOOL _bIsTest)
{
	m_oxQuiz.m_wYear	=	_iBeginYear;
	m_oxQuiz.m_wMonth	=	_iBeginMonth;
	m_oxQuiz.m_wDay		=	_iBeginDay;
	m_oxQuiz.m_wHour	=	_iBeginHour;
	m_oxQuiz.m_wMinute	=	_iBeginMinute;
	m_oxQuiz.m_wSecond	=	0;
	m_oxQuiz.m_wIsTest	=	_bIsTest;
	m_oxQuiz.m_wLimitPlayerCount	=	_iLimitPlayerCount;

	m_oxQuiz.save();
}

void
CEventManager::setWordQuizBeginTime(int _iBeginYear,int _iBeginMonth,int _iBeginDay,int _iBeginHour,int _iBeginMinute,int _iLimitPlayerCount,BOOL _bIsTest)
{
	m_wordQuiz.m_wYear		=	_iBeginYear;
	m_wordQuiz.m_wMonth		=	_iBeginMonth;
	m_wordQuiz.m_wDay		=	_iBeginDay;
	m_wordQuiz.m_wHour		=	_iBeginHour;
	m_wordQuiz.m_wMinute	=	_iBeginMinute;
	m_wordQuiz.m_wSecond	=	0;
	m_wordQuiz.m_wIsTest	=	_bIsTest;
	m_wordQuiz.m_wLimitPlayerCount	=	_iLimitPlayerCount;

	m_wordQuiz.save();
}

void
CEventManager::rollbackChangeTimeInfo()	//	시스템 시간 변경.. -_-?;;
{
	if	(m_iChangeSystemTimeMinute	==	0)
		return;

	CTimeInfo	curTime;

	curTime.update();
	curTime.increaseMinute(m_iChangeSystemTimeMinute);

 	SYSTEMTIME	sysTime;

 	GetLocalTime(&sysTime);

 	sysTime.wYear		=	curTime.m_wYear;
 	sysTime.wMonth		=	curTime.m_wMonth;
 	sysTime.wDay		=	curTime.m_wDay;
 	sysTime.wHour		=	curTime.m_wHour;
 	sysTime.wMinute		=	curTime.m_wMinute;
	sysTime.wSecond		=	curTime.m_wSecond;
 	sysTime.wDayOfWeek	=	curTime.getDayOfWeek();

 	SetLocalTime(&sysTime);

	m_iChangeSystemTimeMinute=	0;
}

void
CEventManager::changeSystemTime(int _iYear,int _iMonth,int _iDay,int _iHour,int _iMinute)	//	시스템 시간 변경.. -_-?;;
{
	rollbackChangeTimeInfo();

	CClient	*lpServer	=	NULL;

	if	(_iYear	==	0)
	{
		CTimeInfo	curTime;

		curTime.update();

		SendSimpleWork(lpServer,eSWWG_CHANGE_SYSTEM_TIME,curTime.m_wYear,curTime.m_wMonth,curTime.m_wDay,curTime.m_wHour,curTime.m_wMinute);
		return;
	}

	CTimeInfo	curTime;

	curTime.update();

	int	iCurrentMinuteValue	=	curTime.getMinuteValue(2000);

	curTime.m_wYear		=	_iYear;
	curTime.m_wMonth	=	_iMonth;
	curTime.m_wDay		=	_iDay;
	curTime.m_wHour		=	_iHour;
	curTime.m_wMinute	=	_iMinute;

	int	iChangedMinuteValue	=	curTime.getMinuteValue(2000);

	m_iChangeSystemTimeMinute=	iCurrentMinuteValue-iChangedMinuteValue;

 	SYSTEMTIME	sysTime;

 	GetLocalTime(&sysTime);

 	sysTime.wYear		=	curTime.m_wYear;
 	sysTime.wMonth		=	curTime.m_wMonth;
 	sysTime.wDay		=	curTime.m_wDay;
 	sysTime.wHour		=	curTime.m_wHour;
 	sysTime.wMinute		=	curTime.m_wMinute;
	sysTime.wSecond		=	curTime.m_wSecond;
 	sysTime.wDayOfWeek	=	curTime.getDayOfWeek();

 	SetLocalTime(&sysTime);

	SendSimpleWork(lpServer,eSWWG_CHANGE_SYSTEM_TIME,_iYear,_iMonth,_iDay,_iHour,_iMinute);
}

void
cOXQuiz::reset()
{
	m_wYear	=	0;
}

void
cOXQuiz::save()
{
	char	strCurrentFolder[1024];

	GetCurrentDirectory(1024,strCurrentFolder);
	SetCurrentDirectory(g_strLogFolder);

	cFILE	file;

	file.Open(_ms("%s/data/OXQuiz.dat",g_strLogFolder),"wb");
	file.Write(this,sizeof(cOXQuiz));
	file.Close();

	SetCurrentDirectory(strCurrentFolder);
}

void
cOXQuiz::load()
{
	cFILE	file;

	if	(!file.Open(_ms("%s/data/OXQuiz.dat",g_strLogFolder),"rb"))
		return;

	file.Read(this,sizeof(cOXQuiz));
	file.Close();
}

void
cWordQuiz::reset()
{
	m_wYear	=	0;
}

void
cWordQuiz::save()
{
	char	strCurrentFolder[1024];

	GetCurrentDirectory(1024,strCurrentFolder);
	SetCurrentDirectory(g_strLogFolder);

	cFILE	file;

	file.Open(_ms("%s/data/wordQuiz.dat",g_strLogFolder),"wb");
	file.Write(this,sizeof(cWordQuiz));
	file.Close();

	SetCurrentDirectory(strCurrentFolder);
}

void
cWordQuiz::load()
{
	cFILE	file;

	if	(!file.Open(_ms("%s/data/wordQuiz.dat",g_strLogFolder),"rb"))
		return;

	file.Read(this,sizeof(cWordQuiz));
	file.Close();
}

void
cEventWord::reset()
{
	;
}

void
cEventWord::save()
{
	char	strCurrentFolder[1024];
	
	GetCurrentDirectory(1024,strCurrentFolder);
	SetCurrentDirectory(g_strLogFolder);
	
	cFILE	file;
	
	file.Open(_ms("%s/data/WordCompare.dat",g_strLogFolder),"wb");
	file.Write(this,sizeof(cEventWord));
	file.Close();
	
	SetCurrentDirectory(strCurrentFolder);
}

void
cEventWord::load()
{
	cFILE	file;
	
	if	(!file.Open(_ms("%s/data/WordCompare.dat",g_strLogFolder),"rb"))
		return;
	
	file.Read(this,sizeof(cEventWord));
	file.Close();
}

void
cRenewPremiumItemExpireTime::init()
{
	cFILE	file;

	m_iCount		=	0;
	m_iLastRenew	=	0;

	if (!file.Open(_ms("%s/data/renewItemExpireTime.dat",g_strLogFolder),"rb"))
		return;

	file.Read(&m_iCount,4);
	file.Read(m_aList,m_iCount*sizeof(cRenewItemExpireTimeInfo));
	file.Close();

	m_iLastRenew	=	m_iCount-1;
}

void
cRenewPremiumItemExpireTime::save()
{
	cFILE	file;

	file.Open(_ms("%s/data/renewItemExpireTime.dat",g_strLogFolder),"wb");

	file.Write(&m_iCount,4);
	file.Write(m_aList,m_iCount*sizeof(cRenewItemExpireTimeInfo));
	file.Close();
}

void
cRenewPremiumItemExpireTime::addRenewItemExpireTime(UTime *_lpTime,DWORD _dwRenewMinute,BOOL _bIsForOnlyYahoo,BOOL _bIsForOnlyRedGem)
{
	cRenewItemExpireTimeInfo	*lpInfo	=	&m_aList[m_iCount];

	memcpy(&lpInfo->m_time,_lpTime,sizeof(UTime));
	lpInfo->m_dwRenewMinuite	=	_dwRenewMinute;
	lpInfo->m_bf14Index			=	m_iCount+1;
	lpInfo->m_bf1IsForOnlyYahoo	=	_bIsForOnlyYahoo;
	lpInfo->m_bf1IsForOnlyRedGem=	_bIsForOnlyRedGem;

	m_iLastRenew				=	m_iCount+1;

	m_iCount++;

	save();
}

void
cItemPackManager::init()
{
	reset();

//	cFILE	file;

//	if (file.Open(_ms("%s/data/itemPackData.dat",g_strLogFolder),"rb"))
//	{
//		file.Read(m_aPacks,sizeof(m_aPacks));
//		file.Close();
//	}
}

void
cItemPackManager::save()
{
//	cFILE	file;

//	file.Open(_ms("%s/data/itemPackData.dat",g_strLogFolder),"wb");
//	file.Write(m_aPacks,sizeof(m_aPacks));
//	file.Close();
}

void
cItemPackManager::reset()
{
	for (int i=0;i<256;i++)
	{
		m_aPacks[i].reset();
		m_aPacks[i].m_wSerial	=	i;
	}
}

DWORD
cItemPackManager::getCheckSum()
{
	DWORD	dwCheckSum	=	0;

	for (int i=0;i<256;i++)
	{
		if (m_aPacks[i].m_wSerial	==	0xffff)
			continue;

		dwCheckSum	+=	m_aPacks[i].getCheckSum();
	}

	return	dwCheckSum;
}

void
cItemPackManager::modifyData(int _iSerial,cItemPackInfo *_lpData)
{
	cItemPackInfo	*lpPackInfo	=	&m_aPacks[_iSerial];
	int				i;

	if (lpPackInfo->m_wLinkItem	!=	0xffff)
	{
		for (i=0;i<lpPackInfo->m_wCount;i++)
		{
			if (lpPackInfo->m_aItems[i].m_wBaseItem	==	_lpData->m_aItems[i].m_wBaseItem)
			{
				DWORD	dwTotalCount=	lpPackInfo->m_aItems[i].m_dwSerial;
				WORD	wCount		=	lpPackInfo->m_aItems[i].m_wCurrentCount;

				memcpy(&lpPackInfo->m_aItems[i],&_lpData->m_aItems[i],sizeof(cItemInPack));

				lpPackInfo->m_aItems[i].m_dwSerial		=	dwTotalCount;
				lpPackInfo->m_aItems[i].m_wCurrentCount	=	wCount;
			}
			else
			{
				memcpy(&lpPackInfo->m_aItems[i],&_lpData->m_aItems[i],sizeof(cItemInPack));
				lpPackInfo->m_aItems[i].m_wCurrentCount	=	0;
				lpPackInfo->m_aItems[i].m_dwSerial		=	0;
			}
		}
	}
	else
		lpPackInfo->m_wCount	=	0;

	for (i=lpPackInfo->m_wCount;i<_lpData->m_wCount;i++)
	{
		memcpy(&lpPackInfo->m_aItems[i],&_lpData->m_aItems[i],sizeof(cItemInPack));

		lpPackInfo->m_aItems[i].m_wCurrentCount	=	0;
		lpPackInfo->m_aItems[i].m_dwSerial		=	0;
	}

	memcpy(lpPackInfo,_lpData,sizeof(cItemPackInfo)-sizeof(_lpData->m_aItems));
	lpPackInfo->m_wSerial	=	_iSerial;

	save();
}

int
cItemPackManager::increaseItemCounter(int _iPackSerial,int _iIndex)
{
	cItemInPack	*lpItem	=	&m_aPacks[_iPackSerial].m_aItems[_iIndex];

	if (lpItem->m_dwSerial	==	0xffffffff)
		lpItem->m_dwSerial	=	0;

	if (lpItem->m_dwSerial	<	0x7fffffff)
		lpItem->m_dwSerial++;

	if (lpItem->m_wCurrentCount	<	0xffff)
	{
		lpItem->m_wCurrentCount++;

		if (lpItem->m_dwSerial	<	lpItem->m_wCurrentCount)
			lpItem->m_dwSerial	=	lpItem->m_wCurrentCount;

		save();

		return	m_aPacks[_iPackSerial].m_aItems[_iIndex].m_wCurrentCount;
	}

	return	0;
}

void
cItemPackManager::resetCounter(int _iPackSerial)
{
	if (_iPackSerial<0||_iPackSerial>=256)
		return;

	for (int i=0;i<dMAX_ITEM_IN_ITEM_PACK;i++)
		m_aPacks[_iPackSerial].m_aItems[i].m_wCurrentCount	=	0;

	save();
}

void
cGoldRush::init()
{
	reset();
}

void
cGoldRush::save()
{
}

void
cGoldRush::reset()
{
	memset(this,0,sizeof(cGoldRush));
}

void
cGoldRush::updateInfo(UTime *_lpBegin,UTime *_lpEnd,int _iBoostExperience,int _iCorrectItemDropChance,char *_lpstrBeginComment,char *_lpstrEndComment)
{
	memcpy(&m_begin,_lpBegin,sizeof(UTime));
	memcpy(&m_end,_lpEnd,sizeof(UTime));

	m_wBoostExperience		=	_iBoostExperience;
	m_wCorrectItemDropChance=	_iCorrectItemDropChance;

	strcpy(m_strBeginComment,_lpstrBeginComment);
	strcpy(m_strEndComment	,_lpstrEndComment);

	save();
}

void
cCarrotShop::init()
{
	reset();

//	cFILE	file;
//
//	if (file.Open(_ms("%s/data/carrotShop.dat",g_strLogFolder),"rb"))
//	{
//		file.Read(this,sizeof(cCarrotShop));
//		file.Close();
//	}
}
/*
void
cCarrotShop::save()
{
	cFILE	file;

	if (file.Open(_ms("%s/data/carrotShop.dat",g_strLogFolder),"wb"))
	{
		file.Write(this,sizeof(cCarrotShop));
		file.Close();
	}
}
*/

void
cCarrotShop::reset()
{
	m_wCategoryCount	=	0;
}

void
cCarrotShop::updateInfo(cCarrotShopDefine *_lpData)
{
	memcpy(this,_lpData,sizeof(cCarrotShopDefine));
}

/*
void
cCarrotShop::updateInfo(int _iCategory,cCarrotShopCategoryInfoDefine *_lpData)
{
	memcpy(&m_aData[_iCategory],_lpData,sizeof(cCarrotShopCategoryInfoDefine));
}
*/



cDuelTeamManager::cDuelTeamManager()
{
	m_wTotalTeamCount	=	0;
	m_wTeamCount		=	0;
	m_pTeams			=	NULL;
}

cDuelTeamManager::~cDuelTeamManager()
{
	pKILL(m_pTeams);
}

void
cDuelTeamManager::init(int _iSize)
{
	if	(m_pTeams)
		return;

	m_wTotalTeamCount	=	_iSize;

	m_pTeams	=	new cDuelTeamInfo [_iSize];
}

bool
cDuelTeamManager::addTeam(cDuelTeamInfo *_lpTeamInfo)
{
	if	(m_pTeams	==	NULL	||	m_wTeamCount	>=	m_wTotalTeamCount)
		return	false;

	memcpy(&m_pTeams[m_wTeamCount],_lpTeamInfo,sizeof(cDuelTeamInfo));

	m_wTeamCount++;

	return	true;
}

cDuelTeamInfo*
cDuelTeamManager::getTeam(char *_lpstrName)
{
	for (int i=0;i<m_wTeamCount;i++)
	{
		if	(STRICMP(m_pTeams[i].m_strTeamName,_lpstrName) == 0)
			return	&m_pTeams[i];
	}

	return	NULL;
}

cDuelTeamInfo*
cDuelTeamManager::getTeam(int _iSlot)
{
	if	(_iSlot	>=	m_wTeamCount)
		return	NULL;

	return	&m_pTeams[_iSlot];
}


