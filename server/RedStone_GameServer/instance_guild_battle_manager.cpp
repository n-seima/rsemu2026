#include "instance_guild_battle_manager.h"
#include "CGame.h"
#include "debugCode.h"

BOOL	g_bIsReceivedGuildBattleSchedule=	FALSE;

CGuildBattleManager	g_gbsToday;

int		g_iGuildBattleInitTime			=	1;
int		g_iGuildBattleInitEnterenceTime	=	4;
int		g_iGuildBattleReadyTime			=	5;
int		g_iGuildBattlePeriod			=	40;
int		g_iGuildBattleTime				=	40;
int		g_iGuildBattleBeginTime			=	10*60;
int		g_iGuildBattleProgressTime		=	g_iGuildBattleTime-5;
int		g_iGuildBattleFinishTime		=	g_iGuildBattleTime-4;
int		g_iGuildBattleEndTime			=	g_iGuildBattleTime;

int			CGuildBattleManager::s_iExceptGuildBattleWeekDay	=	-1;
CTimeInfo	CGuildBattleManager::s_timeExceptGuildBattleBegin;
CTimeInfo	CGuildBattleManager::s_timeExceptGuildBattleEnd;

//
//	초기화
void
CGuildBattleManager::init()
{
	if	(isToday() && g_bIsReceivedGuildBattleSchedule == FALSE)
	{
		int	iBattleOrder	=	getCurrentBattleOrder();
		int	iBattleStatus	=	getCurrentBattleStatus();

		if	(iBattleStatus	==	eGB_EARLY_TIME)
			return;

		if	(iBattleStatus	==	eGB_OFF_TIME)
		{
			resetSchedule();
			return;
		}
	}
}

//
//	초기화
void
CGuildBattleManager::initForTest(char *_lpGuildName1,char *_lpGuildName2,int _iTerm,int _iField)
{
	int	iGuild1	=	0;
	int	iGuild2	=	5;

	g_iGuildBattleInitTime			=	1;
	g_iGuildBattleInitEnterenceTime	=	2;
	g_iGuildBattleReadyTime			=	3;
	g_iGuildBattlePeriod			=	7+_iTerm-1;
	g_iGuildBattleTime				=	7+_iTerm-1;
	g_iGuildBattleBeginTime			=	g_currentTime.m_wHour*60+g_currentTime.m_wMinute;
	g_iGuildBattleProgressTime		=	g_iGuildBattleTime-2;
	g_iGuildBattleFinishTime		=	g_iGuildBattleTime-1;
	g_iGuildBattleEndTime			=	g_iGuildBattleTime;

	CGuild	*lpGuild1				=	g_guildManager.getByName(_lpGuildName1);
	CGuild	*lpGuild2				=	g_guildManager.getByName(_lpGuildName2);

	if (!lpGuild1	||	!lpGuild2)
		return;

	if (lpGuild1)
		iGuild1	=	lpGuild1->m_wSerial;
	if (lpGuild2)
		iGuild2	=	lpGuild2->m_wSerial;

	m_aBattleGuildSchedule[0].m_aBattleGuildInfo[0].m_awBattleGuild[0]	=	iGuild1;
	m_aBattleGuildSchedule[0].m_aBattleGuildInfo[0].m_awBattleGuild[1]	=	iGuild2;
	m_aBattleGuildSchedule[0].m_aBattleGuildInfo[0].m_wFieldSerial		=	_iField;
	
	m_wYear		=	g_currentTime.m_wYear;
	m_wMonth	=	g_currentTime.m_wMonth;
	m_wDay		=	g_currentTime.m_wDay;
}

//
//	리쉣
void
CGuildBattleManager::resetSchedule()
{
	int	i,j;

	for (i=0;i<dGUILD_BATTLE_COUNT;i++)
	{
		for (j=0;j<dGUILD_BATTLE_COUNT_IN_SAME_TIME;j++)
		{
			m_aBattleGuildSchedule[i].m_aBattleGuildInfo[j].m_awBattleGuild[0]	=	0xffff;
			m_aBattleGuildSchedule[i].m_aBattleGuildInfo[j].m_awBattleGuild[1]	=	0xffff;
		}
	}
}

//
//	업데이트
void
CGuildBattleManager::operateOncePerMinute()
{
	if	(g_bIsReceivedGuildBattleSchedule	==	FALSE)
		return;

	int	i,j;

	if	(g_iServerIndex	==	0)
	{
		int	iBattleOrder	=	getCurrentBattleOrder();

		for (i=0;i<iBattleOrder-1;i++)
		{
			for (j=0;j<dGUILD_BATTLE_COUNT_IN_SAME_TIME;j++)
			{
				CGuildBattleInfo	*lpInfo	=	&m_aBattleGuildSchedule[i].m_aBattleGuildInfo[j];

				if	(lpInfo->m_awBattleGuild[0]	==	0xffff	&&	lpInfo->m_awBattleGuild[1]	==	0xffff)
					continue;

				g_game.sendWPCancelGuildBattle(NULL,i,0);	//	이 시간대 건 다 취소

				break;
			}
		}
	}
}

//
//	현재 전투 순서
int
CGuildBattleManager::getCurrentBattleOrder(int _iFieldOrder)
{
	CTimeInfo	currentTime;
	int			iCurrentTimeValue;

	currentTime.update();

	iCurrentTimeValue	=	currentTime.m_wHour*60+currentTime.m_wMinute-g_iGuildBattleBeginTime;
	iCurrentTimeValue	-=	_iFieldOrder/c_iSameTimeFiledCountInGuildBattle*c_iGuildBattlePeroidByFieldOrder;

	if	(currentTime.m_wDay !=	m_wDay)
		iCurrentTimeValue	+=	24*60;

	if	(iCurrentTimeValue	<	0)	//	시작전
		return	-1;

	if	(iCurrentTimeValue/g_iGuildBattlePeriod	>=	dGUILD_BATTLE_COUNT)	//	시즌이 아니다.
		return	dGUILD_BATTLE_COUNT;

	return	iCurrentTimeValue/g_iGuildBattlePeriod;
}

//
//	현재 전투 상태
int
CGuildBattleManager::getCurrentBattleStatus(int _iFieldOrder)
{
	if	(g_bIsReceivedGuildBattleSchedule	==	FALSE)
		return	eGB_SLEEP_TIME;

	CTimeInfo	currentTime;
	int			iCurrentTimeValue,iStatusValue;

	currentTime.update();

	iCurrentTimeValue		=	currentTime.m_wHour*60+currentTime.m_wMinute-g_iGuildBattleBeginTime;

	if	(currentTime.m_wDay !=	m_wDay)
		iCurrentTimeValue	+=	24*60;

	iCurrentTimeValue	-=	_iFieldOrder/c_iSameTimeFiledCountInGuildBattle*c_iGuildBattlePeroidByFieldOrder;

	if	(iCurrentTimeValue<	0)	//	아직 길드전이 시작하기전이다.
		return	eGB_EARLY_TIME;

	if	(iCurrentTimeValue/g_iGuildBattlePeriod	>=	dGUILD_BATTLE_COUNT)
		return	eGB_OFF_TIME;

	iStatusValue		=	iCurrentTimeValue%g_iGuildBattlePeriod;

	if	(iStatusValue	<	g_iGuildBattleInitTime)
		return	eGB_INIT;	//	초기화

	if	(iStatusValue	<	g_iGuildBattleInitEnterenceTime)
		return	eGB_ENTERENCE;//	참가

	if	(iStatusValue	<	g_iGuildBattleReadyTime)
		return	eGB_READY;	//	전투 준비(대기)

	if	(iStatusValue	==	g_iGuildBattleProgressTime-1)
		return	eGB_READY_TO_BATTLE_FINISH;	//	전투중(1분후에 종료된다.)

	if	(iStatusValue	<	g_iGuildBattleProgressTime)
		return	eGB_BATTLE;	//	전투중

	if	(iStatusValue	<	g_iGuildBattleFinishTime)
		return	eGB_FINISH;	//	전투 마무리

	if	(iStatusValue	<	g_iGuildBattleEndTime)
		return	eGB_END;	//	전투 끝

	return	eGB_SLEEP_TIME;	
}

int
CGuildBattleManager::getFieldOrderByFieldSerial(int _iUniqueFieldSerial,int _iTimeOrder)
{
	for (int i=0;i<dGUILD_BATTLE_COUNT_IN_SAME_TIME;i++)
	{
		CGuildBattleInfo*	lpInfo	=	&m_aBattleGuildSchedule[_iTimeOrder].m_aBattleGuildInfo[i];

		if	(lpInfo->m_wFieldSerial == _iUniqueFieldSerial)
		{
			if	(lpInfo->m_awBattleGuild[0]	==	0xffff || lpInfo->m_awBattleGuild[1]	==	0xffff)
				return	0xffff;

			return	i;
		}
	}

	return	0xffff;
}

//
//	길드전 정보 얻기
CGuildBattleInfo*
CGuildBattleManager::getBattleInfoByFieldSerial(int _iUniqueFieldSerial,int _iTimeOrder)
{
	int	iFieldOrder	=	getFieldOrderByFieldSerial(_iUniqueFieldSerial,_iTimeOrder);

	if	(iFieldOrder	==	0xffff)
		return	NULL;

	CGuildBattleInfo*	lpInfo	=	&m_aBattleGuildSchedule[_iTimeOrder].m_aBattleGuildInfo[iFieldOrder];

	if	(lpInfo->m_awBattleGuild[0]	==	0xffff || lpInfo->m_awBattleGuild[1]	==	0xffff)
		return	NULL;

	return	lpInfo;
}

//
//	길드전 정보 업데이트
void
CGuildBattleManager::updateBattleInfo(int _iTimeOrder,int _iFieldOrder,int _iGuild1,int _iGuild2)
{
	m_aBattleGuildSchedule[_iTimeOrder].m_aBattleGuildInfo[_iFieldOrder].m_awBattleGuild[0]	=	_iGuild1;
	m_aBattleGuildSchedule[_iTimeOrder].m_aBattleGuildInfo[_iFieldOrder].m_awBattleGuild[1]	=	_iGuild2;

	SG_CHANGE_GUILD_BATTLE_SCHEDULE	packet;
	CGuild	*lpGuild1,*lpGuild2;

	packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_BATTLE_SCHEDULE);

	packet.wTimeOrder				=	_iTimeOrder;
	packet.wFieldOrder				=	_iFieldOrder;
	packet.info.m_awBattleGuild[0]	=	_iGuild1;
	packet.info.m_awBattleGuild[1]	=	_iGuild2;
	packet.info.m_astrGuild[0][0]	=	NULL;
	packet.info.m_astrGuild[1][0]	=	NULL;
	packet.info.m_wFieldSerial		=	m_aBattleGuildSchedule[_iTimeOrder].m_aBattleGuildInfo[_iFieldOrder].m_wFieldSerial;

	lpGuild1	=	g_guildManager.getBySerial(_iGuild1);
	lpGuild2	=	g_guildManager.getBySerial(_iGuild2);

	if	(_iGuild1 != 0xffff && !lpGuild1)
		g_game.sendWPAskGuildInfo(_iGuild1);

	if	(_iGuild2 != 0xffff && lpGuild2 == NULL)
		g_game.sendWPAskGuildInfo(_iGuild2);

	if	(lpGuild1)
		strcpy(packet.info.m_astrGuild[0],lpGuild1->m_strName);
	if	(lpGuild2)
		strcpy(packet.info.m_astrGuild[1],lpGuild2->m_strName);

	g_game.sendPacketToAllPlayer((char *)&packet);	//	SG_CHANGE_GUILD_BATTLE_SCHEDULE 패킷 자체 길드전 스케쥴 열어둔 대상에게만 전달되게 되어 있음
}

//
//	길드로 등록된 전투가 없는지 검색한다.
CGuildBattleInfo*
CGuildBattleManager::getBattleInfoByGuild(int _iGuildSerial,int *_lpTimeOrder,int *_lpFieldOrder)
{
	int	iFieldOrder,iTimeOrder	=	0;

	for (;iTimeOrder<dGUILD_BATTLE_COUNT;iTimeOrder++)
		for (iFieldOrder=0;iFieldOrder<dGUILD_BATTLE_COUNT_IN_SAME_TIME;iFieldOrder++)
		{
			CGuildBattleInfo	*lpInfo	=	&m_aBattleGuildSchedule[iTimeOrder].m_aBattleGuildInfo[iFieldOrder];
			
			if	(lpInfo->m_awBattleGuild[0] == _iGuildSerial || lpInfo->m_awBattleGuild[1] == _iGuildSerial)
			{
				if	(_lpTimeOrder)
					*_lpTimeOrder	=	iTimeOrder;
				if	(_lpFieldOrder)
					*_lpFieldOrder	=	iFieldOrder;

				return	lpInfo;
			}
		}

	return	NULL;
}

//
//	길드전 정보 얻기
CGuildBattleInfo*
CGuildBattleManager::getBattleInfo(int _iTimeOrder,int _iFieldOrder)
{
	return	&m_aBattleGuildSchedule[_iTimeOrder].m_aBattleGuildInfo[_iFieldOrder];
}

//
//	길드전 정보 업데이트
void
CGuildBattleManager::requestGuildBattle(cACTOR *_lpActor,int _iTimeOrder,int _iFieldOrder)
{
//	길마인가?
	if	(!_lpActor->isGuildMaster())
	{
		_lpActor->sendRegistServerMessage(eRSM_GUILD_ONLY_GUILD_MASTER_CAN_REQUEST_GUILD_BATTLE);

		return;
	}

	CGuildBattleInfo	*lpGuildBattleInfo	=	getBattleInfoByGuild(_lpActor->m_wGuildSerial);

//	이미 예약 되어있다.
	if	(lpGuildBattleInfo)
	{
		_lpActor->sendRegistServerMessage(eRSM_GUILD_ALREADY_BOOKING_GUILD_BATTLE);

		return;
	}

	CGuild	*lpGuild	=	_lpActor->getGuild();

	lpGuildBattleInfo	=	&m_aBattleGuildSchedule[_iTimeOrder].m_aBattleGuildInfo[_iFieldOrder];

//	이미 누군가가 예약한 스케쥴이다.
	if	(lpGuildBattleInfo->m_awBattleGuild[0] != 0xffff && lpGuildBattleInfo->m_awBattleGuild[1] != 0xffff)
	{
		_lpActor->sendRegistServerMessage(eRSM_GUILD_ALREADY_BOOKED_SCHEDULE);
		_lpActor->sendChangeGuildBattleSchedule(_iTimeOrder,_iFieldOrder,lpGuildBattleInfo);

		return;
	}

	if	(lpGuildBattleInfo->m_awBattleGuild[0] != 0xffff)
	{
		CGuild	*lpTargetGuild	=	g_guildManager.getBySerial(lpGuildBattleInfo->m_awBattleGuild[0]);

		if	(!lpTargetGuild)
		{
			g_game.sendWPAskGuildInfo(lpGuildBattleInfo->m_awBattleGuild[0]);
			g_game.sendWPAskGuildBattleSchedule();

			_lpActor->sendRegistServerMessage(eRSM_GUILD_INCORRECT_DATA_PLEASE_RETRY);

			return;
		}
	}

	if	(!g_bIsTestServer && g_iWorldServerType	!=	eSERVER_TYPE_GVG)
	{
		if	(_lpActor->m_iGold	<	dGUILD_BATTLE_REQUEST_PRICE)
		{
			_lpActor->sendRegistServerMessage(eRSM_TOO_LOW_GOLD_TO_GUILD_BATTLE);

			return;
		}
		_lpActor->m_iGold	-= dGUILD_BATTLE_REQUEST_PRICE;

		_lpActor->updateGoldLog(-dGUILD_BATTLE_REQUEST_PRICE,eSPENT_GOLD_BY_REQUEST_GUILD_BATTLE);

		_lpActor->sendAddGold(-dGUILD_BATTLE_REQUEST_PRICE);
	}

	//	등을 확인한후 결과를 리턴하거나 월드에 길드전 요청

	g_game.sendBookingGuildBattleLog(lpGuild->m_strName,_iTimeOrder,_iFieldOrder);
	g_game.sendWPRequestGuildBattle(_lpActor,_iTimeOrder,_iFieldOrder);
}

//
//	시작 시간
void
CGuildBattleManager::getBeginTime(int _iTimeOrder,CTimeInfo *_lpTime,int _iFieldOrder)
{
	int	iBeginTime	=	g_iGuildBattleBeginTime+_iFieldOrder/c_iSameTimeFiledCountInGuildBattle*c_iGuildBattlePeroidByFieldOrder;

	iBeginTime		+=	(_iTimeOrder*g_iGuildBattlePeriod);

	memcpy(_lpTime,&g_currentTime,sizeof(g_currentTime));

	_lpTime->m_wHour	=	iBeginTime/60;
	_lpTime->m_wMinute	=	iBeginTime%60;
	
	if	(_lpTime->m_wHour	>=	24)
	{
		_lpTime->m_wHour	-=	24;
		_lpTime->increaseDay(1);
	}
}

void
CGuildBattleManager::resetFieldSerial()
{
	int	i,j;

	for (i=0;i<dGUILD_BATTLE_COUNT;i++)
	{
		for (j=0;j<dGUILD_BATTLE_COUNT_IN_SAME_TIME;j++)
		{
			if	(m_aBattleGuildSchedule[i].m_aBattleGuildInfo[j].m_wFieldSerial	!=	dGUILD_BATTLE_LAST_SERIAL_FIELD-j)
				m_aBattleGuildSchedule[i].m_aBattleGuildInfo[j].m_wFieldSerial=	dGUILD_BATTLE_LAST_SERIAL_FIELD-j;
		}
	}
}

//	특정 시간대 길드전 통째로 취소
void
CGuildBattleManager::resetGuildBattle(int _iBattleOrder)
{
	m_aBattleGuildSchedule[_iBattleOrder].cancel();

	if	(g_game.m_iGuildBattleLobbyField	==	0xffffffff)
		return;

	cFIELD	*lpField	=	g_game.getFieldByUniqueSerial(g_game.m_iGuildBattleLobbyField);

	if	(!lpField)
		return;

	SG_CHANGE_GUILD_BATTLE_SCHEDULE	packet;

	packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_BATTLE_SCHEDULE);

	packet.wFieldOrder	=	0xffff;
	packet.wTimeOrder	=	_iBattleOrder;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.info);

	lpField->sendPacketToAllPlayer(&packet);
}

//
//	길드전 취소
BOOL
CGuildBattleManager::cancelGuildBattle(cACTOR *_lpActor)
{
//	길마인가?
	if	(!_lpActor->isGuildMaster())
	{
		_lpActor->sendRegistServerMessage(eRSM_GUILD_ONLY_GUILD_MASTER_CAN_CANCEL_GUILD_BATTLE);

		return FALSE;
	}

	int	iTimeOrder,iFieldOrder;

	CGuildBattleInfo	*lpInfo	=	getBattleInfoByGuild(_lpActor->m_wGuildSerial,&iTimeOrder,&iFieldOrder);
	CGuild				*lpGuild=	_lpActor->getGuild();

	if	(!lpInfo)
		return	FALSE;

	{
		CTimeInfo	beginTime;
		
		getBeginTime(iTimeOrder,&beginTime);

		int	iCurrentTime	=	(g_currentTime.m_wYear-2000)*365*31*60*24+g_currentTime.m_wMonth*31*60*24+g_currentTime.m_wDay*60*24+g_currentTime.m_wHour*60+g_currentTime.m_wMinute;
		int	iBeginTime		=	(beginTime.m_wYear-2000)*365*31*60*24+beginTime.m_wMonth*31*60*24+beginTime.m_wDay*60*24+beginTime.m_wHour*60+beginTime.m_wMinute;

		if	(iCurrentTime+60>=	iBeginTime)
		{
			_lpActor->sendRegistServerMessage(eRSM_GUILD_TOO_LATE_TO_CANCEL_GUILD_BATTLE);

			return TRUE;
		}
	}

//	9. 길드전 취소는 시작 시간 1시간 전에만 가능하며 취소된 길드전은 승패와 랭킹 포인트 증감 없이 그냥 사라진다
//	현재 예약 되어있나(타임오더와 필드 오더 구함)
//	등을 확인한후 결과를 리턴하거나 월드에 길드전 취소 요청

	g_game.sendWPCancelGuildBattle(_lpActor,iTimeOrder,iFieldOrder);
	g_game.sendCancelGuildBattleLog(lpGuild->m_strName);

	return	TRUE;
}


BOOL
CGuildBattleManager::IsExceptGuildBattleTime(int _iWeekDay,int _iHour,int _iMinute)
{
	if	(s_iExceptGuildBattleWeekDay	==	-1)
		return	FALSE;

	if	(_iWeekDay	!=	s_iExceptGuildBattleWeekDay)
		return	FALSE;

	int	iAppointmentTime	=	_iHour*60	+	_iMinute;
	int	iBeginTime			=	s_timeExceptGuildBattleBegin.m_wHour*60+s_timeExceptGuildBattleBegin.m_wMinute;
	int	iEndTime			=	s_timeExceptGuildBattleEnd.m_wHour*60+s_timeExceptGuildBattleEnd.m_wMinute;

	if	(iAppointmentTime+40	>=	iBeginTime	&&	iAppointmentTime	<=	iEndTime)
		return	TRUE;

	return	FALSE;
}

void
CGuildBattleManager::SetMaintenanceTime(int _iWeekDay,int _iBeginHour,int _iBeginMinute,int _iEndHour,int _iEndMinute)
{
	if	(_iWeekDay+_iBeginHour+_iBeginMinute+_iEndHour+_iEndMinute	==	0)
	{
		s_iExceptGuildBattleWeekDay	=	-1;

		return;
	}

	s_iExceptGuildBattleWeekDay				=	_iWeekDay;
	s_timeExceptGuildBattleBegin.m_wHour	=	_iBeginHour;
	s_timeExceptGuildBattleBegin.m_wMinute	=	_iBeginMinute;
	s_timeExceptGuildBattleEnd.m_wHour		=	_iEndHour;
	s_timeExceptGuildBattleEnd.m_wMinute	=	_iEndMinute;
}

//	길드전에 관한 신청을 안 받는 시간이다.(자정 전후 10분)
BOOL
CGuildBattleManager::isBlockToRequestBattleTime()
{
	if	(g_currentTime.m_wHour	==	23	&&	g_currentTime.m_wMinute	>=	50)
		return	TRUE;
	if	(g_currentTime.m_wHour	==	0	&&	g_currentTime.m_wMinute	<=	10)
		return	TRUE;

	return	FALSE;
}
