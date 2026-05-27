#include "CGuildBattleManager.h"
#include "cPACKET_WORLDSERVER.h"
#include "guild.h"
#include "operate_packet.h"
#include "CDecoder.h"
#include "CUtil.h"
#include "server.h"

CGuildBattleManager	g_guildBattleManager;

int	l_iUserTestSiegeWarfareOfGuildHour	=	16;

enum
{
//	Guild Battle Data Version Info
	eGBDVI_00					,
	eGBDVI_01					,	//	길드홀 정보 추가

	eGBDVI_CURRENT_VERSION		,
};

#define	dGBDVI_CURRENT	(eGBDVI_CURRENT_VERSION-1)

//	초기화
void
CGuildBattleManager::init()
{
//	memset(&m_timeLastSiegeWarfareScheduleUpdate,0,sizeof(m_timeLastSiegeWarfareScheduleUpdate));
	m_timeLastSiegeWarfareScheduleUpdate.update();

	reset();
	load();

	InitializeCriticalSection(&m_csWork);
}

//	끝장~
void
CGuildBattleManager::close()
{
	save();

	DeleteCriticalSection(&m_csWork);
}

void
CGuildBattleManager::reset()
{
	resetTodaySchedule();
}

//	리셋
void
CGuildBattleManager::resetTodaySchedule()
{
	int	i,j;

	for (i=0;i<dGUILD_BATTLE_COUNT;i++)
		m_gbsToday.m_aBattleGuildSchedule[i].reset();

	CTimeInfo	current;

	current.update();

	m_gbsToday.m_wYear		=	current.m_wYear;
	m_gbsToday.m_wMonth		=	current.m_wMonth;
	m_gbsToday.m_wDay		=	current.m_wDay;

	for (i=0;i<dGUILD_BATTLE_COUNT;i++)
		for (j=0;j<dGUILD_BATTLE_COUNT_IN_SAME_TIME;j++)
			m_gbsToday.m_aBattleGuildSchedule[i].m_aBattleGuildInfo[j].m_wFieldSerial	=	dGUILD_BATTLE_LAST_SERIAL_FIELD-j;
}

void
CGuildBattleManager::load()
{
	cFILE	file;

	int		iVersion;

	m_timeLastSiegeWarfareScheduleUpdate.update();

	if	(!file.Open("data/guildBattle.dat","rb"))
		return;

	file.Read(&iVersion,sizeof(iVersion));
	file.Read(&m_gbsToday,sizeof(m_gbsToday));

	if	(iVersion	>=	eGBDVI_01)
	{
		file.Read(&g_guildManager.m_aHallList,sizeof(g_guildManager.m_aHallList));
		g_guildManager.m_bIsLoadGuildHallInfo	=	TRUE;
	}

	file.Close();

	CTimeInfo	todayDate,tomorrowDate;

	todayDate.update();
	memcpy(&tomorrowDate,&todayDate,sizeof(CTimeInfo));

	tomorrowDate.increaseDay(1);

	if	(todayDate.m_wMonth		!= m_gbsToday.m_wMonth || todayDate.m_wYear != m_gbsToday.m_wYear || todayDate.m_wDay != m_gbsToday.m_wDay)	//	날짜가 바뀌었다.
		reset();
}

void
CGuildBattleManager::save()
{
	char	strCurrentFolder[1024];

	GetCurrentDirectory(1024,strCurrentFolder);
	SetCurrentDirectory(g_strLogFolder);

	cFILE	file;

	int	iVersion	=	dGBDVI_CURRENT;

	file.Open("data/guildBattle.dat","wb");

	file.Write(&iVersion,sizeof(iVersion));
	file.Write(&m_gbsToday,sizeof(m_gbsToday));
	file.Write(&g_guildManager.m_aHallList,sizeof(g_guildManager.m_aHallList));

	file.Close();

	SetCurrentDirectory(strCurrentFolder);
}

//
//	전투 예약
int
CGuildBattleManager::bookingBattle(int _iGuildSerial,int _iTimeOrder,int _iFieldOrder,int _iCurrentTimeOrder,CGuildBattleInfo *_lpReturnInfo)
{
	mCS(m_csWork);

	CGuildBattleInfo	*lpInfo	=	getBookedSchedule(_iGuildSerial,_iTimeOrder);

	if	(lpInfo)
		return	eRBGB_ALREADY_EXIST_SCHEDULE;

	cGuild	*lpGuild				=	g_guildManager.GetGuildPntBySerial(_iGuildSerial);

	if	(!lpGuild)
		return	eRBGB_NOT_EXIST_GUILD;

	{
		int		iBookedScheduleCount	=	getBookedScheduleCount(_iGuildSerial);	//	현재 예약된 길드전 수
		if	(g_iWorldType	==	eSERVER_TYPE_GVG)
		{
			if	(iBookedScheduleCount+lpGuild->m_bf3ThisDayGuildBattleCount	>=	(UINT)g_iGuildBattleCountAtDay)	//	오늘 전투한 횟수
				return	eRBGB_OVER_BATTLE_FOR_THIS_DAY;
			
			if	(iBookedScheduleCount+lpGuild->m_bf5ThisWeekGuildBattleCount>=	(UINT)g_iGuildBattleCountAtWeek)	//	주중 전투 횟수
				return	eRBGB_OVER_BATTLE_FOR_THIS_WEEK;
		}
	}

	lpInfo	=	&m_gbsToday.m_aBattleGuildSchedule[_iTimeOrder].m_aBattleGuildInfo[_iFieldOrder];

	_lpReturnInfo->m_awBattleGuild[0]	=	lpInfo->m_awBattleGuild[0];
	_lpReturnInfo->m_awBattleGuild[1]	=	lpInfo->m_awBattleGuild[1];

	if	(lpInfo->m_awBattleGuild[0] != 0xffff && lpInfo->m_awBattleGuild[1] != 0xffff)
		return	eRBGB_ALREADY_BOOKED;

	if (lpInfo->m_awBattleGuild[0]	!= 0xffff)
	{
		cGuild	*lpGuild1	=	g_guildManager.GetGuildPntBySerial(lpInfo->m_awBattleGuild[0]);

		if	(!lpGuild1)
		{
			lpInfo->m_awBattleGuild[0]	= 0xffff;

			return	eRBGB_INCORRECT_DATA;
		}
	}

	if	(lpInfo->m_awBattleGuild[0]	== 0xffff)
		lpInfo->m_awBattleGuild[0]	=	_iGuildSerial;
	else
		lpInfo->m_awBattleGuild[1]	=	_iGuildSerial;

	_lpReturnInfo->m_awBattleGuild[0]	=	lpInfo->m_awBattleGuild[0];
	_lpReturnInfo->m_awBattleGuild[1]	=	lpInfo->m_awBattleGuild[1];

	save();

	return	eRBGB_SUCCESS;
}


//	0 - 실패 : 이미 지나버린 시간은 조정불가능하다.
//	1 - 성공
//	어드민툴에서 전투신청 변경하기
//	_lpCancelInfo -> 시리얼이 설정된 길드는 캔슬해줘라.
int
CGuildBattleManager::changeBattle(int _iGuildSerial1, int _iGuildSerial2,int _iTimeOrder,WORD _wField,CGuildBattleInfo * _lpReturnInfo,CGuildBattleInfo * _lpCancelInfo)
{
	mCS(m_csWork);

	int	iResult = 0;

	CGuildBattleSchedule	* lpBS;

	lpBS = &m_gbsToday;

	CGuildBattleInfo *lpInfo = &lpBS->m_aBattleGuildSchedule[_iTimeOrder].m_aBattleGuildInfo[_wField];

	//	기존에 있던 길드정보를 cancel한다.
	if	(lpInfo->m_awBattleGuild[0]!=0xffff && lpInfo->m_awBattleGuild[0] != _iGuildSerial1)
	{
		_lpCancelInfo->m_awBattleGuild[0]	=	lpInfo->m_awBattleGuild[0];
	}
	else
		_lpCancelInfo->m_awBattleGuild[0]	=	0xffff;

	if	(lpInfo->m_awBattleGuild[1]!=0xffff && lpInfo->m_awBattleGuild[1] != _iGuildSerial1)
	{
		_lpCancelInfo->m_awBattleGuild[1] = lpInfo->m_awBattleGuild[1];
	}
	else
		_lpCancelInfo->m_awBattleGuild[1] = 0xffff;

	lpInfo->m_awBattleGuild[0] = _lpReturnInfo->m_awBattleGuild[0] = _iGuildSerial1;
	lpInfo->m_awBattleGuild[1] = _lpReturnInfo->m_awBattleGuild[1] = _iGuildSerial2;
	_lpReturnInfo->m_wFieldSerial = _wField;
	iResult = 1;
	save();

	return	iResult;
}

//	전부 취소
int
CGuildBattleManager::cancelBattles(int _iBattleOrder)
{
	mCS(m_csWork);

	m_gbsToday.m_aBattleGuildSchedule[_iBattleOrder].cancel();

	save();

	return	eRCGB_SUCCESS;
}

//
//	예약된거 취소
int
CGuildBattleManager::cancelBookedBattle(int _iGuildSerial,CGuildBattleInfo *_lpReturnInfo)
{
	mCS(m_csWork);

	CGuildBattleInfo	*lpInfo	=	getBookedScheduleWithAll(_iGuildSerial);

	_lpReturnInfo->m_awBattleGuild[0]	=	0xffff;
	_lpReturnInfo->m_awBattleGuild[1]	=	0xffff;

	if	(!lpInfo)
		return	eRCGB_VALID;

	if	(lpInfo->m_awBattleGuild[1]	!=	0xffff)
		return	eRCGB_CAN_NOT_CANCEL_BATTLE_BY_ALREADY_ENGAGED;

	cGuild	*lpGuild			=	g_guildManager.GetGuildPntBySerial(_iGuildSerial);

	lpInfo->m_awBattleGuild[1]	=	0xffff;

	if	(lpInfo->m_awBattleGuild[0]	== _iGuildSerial)
		lpInfo->m_awBattleGuild[0]	=	0xffff;

	_lpReturnInfo->m_awBattleGuild[0]	=	lpInfo->m_awBattleGuild[0];
	_lpReturnInfo->m_awBattleGuild[1]	=	lpInfo->m_awBattleGuild[1];

	save();

	return	eRCGB_SUCCESS;
}

//
//	예약된 전투 얻기
CGuildBattleInfo*
CGuildBattleManager::getBookedSchedule(int _iGuildSerial,int _iCurrentTimeOrder)
{
	int	j;

	for (j=0;j<dGUILD_BATTLE_COUNT_IN_SAME_TIME;j++)
	{
		CGuildBattleInfo	*lpInfo	=	&m_gbsToday.m_aBattleGuildSchedule[_iCurrentTimeOrder].m_aBattleGuildInfo[j];

		if	(lpInfo->isBattleGuild(_iGuildSerial))
			return	lpInfo;
	}

	return	NULL;
}

//
//	예약된 전투 얻기
CGuildBattleInfo*
CGuildBattleManager::getBookedScheduleWithAll(int _iGuildSerial)
{
	int	i,j;

	for (i=0;i<dGUILD_BATTLE_COUNT;i++)
		for (j=0;j<dGUILD_BATTLE_COUNT_IN_SAME_TIME;j++)
		{
			CGuildBattleInfo	*lpInfo	=	&m_gbsToday.m_aBattleGuildSchedule[i].m_aBattleGuildInfo[j];

			if	(lpInfo->isBattleGuild(_iGuildSerial))
				return	lpInfo;
		}

	return	NULL;
}

int
CGuildBattleManager::getBookedScheduleCount(int _iGuildSerial)
{
	int	i,j,iCount=0;

	for (i=0;i<dGUILD_BATTLE_COUNT;i++)
		for (j=0;j<dGUILD_BATTLE_COUNT_IN_SAME_TIME;j++)
		{
			CGuildBattleInfo	*lpInfo	=	&m_gbsToday.m_aBattleGuildSchedule[i].m_aBattleGuildInfo[j];

			if	(lpInfo->isBattleGuild(_iGuildSerial))
				iCount++;
		}

	return	NULL;
}

void
CGuildBattleManager::buildProgramOfSiegeWarfare()
{
	int				iGuild,iHallLevel,iGuildCount;
	cIndexForQSort	aGuildList[dGUILD_MAX_COUNT];
	int				aiChallengeGuildCount[]	=	{0,20,7,2,1,0};
	int				aiHallCountByHallLevel[]=	{0,0xffff,27,9,3,1};
	int				aiHallOrderByHonorPoint[6][40];

	memset(aiHallOrderByHonorPoint,0xff,sizeof(aiHallOrderByHonorPoint));

	g_guildManager.resetInvadeGuildInfo();

	CTimeInfo	currentTime;

	currentTime.update();

	cFILE	fileLog;

	fileLog.Open(_ms("%s/data/%s_siege_warfare_log%d_%.2d_%.2d.txt",
		g_strLogFolder,strWorldName,currentTime.m_wYear,currentTime.m_wMonth,currentTime.m_wDay),"wt");

	fileLog.writeStream("길드홀 정보\n");

	//	순수하게 로그를 위한 코드
	for	(iHallLevel=2;iHallLevel<=5;iHallLevel++)	//	홀 별로 명예 포인트를 정렬한다.
	{
		fileLog.writeStream("==> %d레벨 길드홀\n",iHallLevel);

		for	(int iHall=0;iHall<aiHallCountByHallLevel[iHallLevel];iHall++)
		{
			cGuildHallInfo	*lpHallInfo	=	&g_guildManager.m_aHallList[iHallLevel][iHall];
			cGuild			*lpGuild	=	g_guildManager.GetGuildPntBySerial(lpHallInfo->m_wOwnGuild);

			if	(!lpGuild	|| lpGuild->wSerial==0xffff)
			{
				fileLog.writeStream("slot %d	비었음\n",iHall);

				continue;
			}

			fileLog.writeStream("slot %d	%s	%d\n",iHall,lpGuild->m_strName,lpGuild->m_iHonorPoint);
		}
	}

	fileLog.writeStream("수성측 길드홀 정렬 순서\n");

//	수성측 길드홀 순서 정리
//	길드가 없는 홀,명예 포이트가 0점인 홀, 그리고 명예 포인트에 따라 정렬 
	for	(iHallLevel=2;iHallLevel<=5;iHallLevel++)	//	홀 별로 명예 포인트를 정렬한다.
	{
		iGuildCount	=	aiHallCountByHallLevel[iHallLevel];

		fileLog.writeStream("==> %d레벨 길드홀\n",iHallLevel);

		for	(int iHall=0;iHall<iGuildCount;iHall++)
		{
			cGuildHallInfo	*lpHallInfo	=	&g_guildManager.m_aHallList[iHallLevel][iHall];
			cGuild			*lpGuild	=	g_guildManager.GetGuildPntBySerial(lpHallInfo->m_wOwnGuild);

			if	(!lpGuild	|| lpGuild->wSerial==0xffff)
			{
				aGuildList[iHall].m_iIndex	=	iHall;
				aGuildList[iHall].m_iValue	=	0;

				continue;
			}

			if	(lpGuild->m_iHonorPoint	==	0)
			{
				aGuildList[iHall].m_iIndex	=	iHall;
				aGuildList[iHall].m_iValue	=	1;

				continue;
			}

			aGuildList[iHall].m_iIndex	=	iHall;
			aGuildList[iHall].m_iValue	=	lpGuild->m_iHonorPoint;
		}

		qsort((void *)aGuildList,iGuildCount,sizeof(cIndexForQSort),(int(*)(const void *,const void *))Compare);

		for	(int iOrder=0;iOrder<iGuildCount;iOrder++)
		{
			int	iHallSlot								=	aGuildList[iOrder].m_iIndex;
			aiHallOrderByHonorPoint[iHallLevel][iOrder]	=	iHallSlot;

			int			iOwnGuild	=	g_guildManager.m_aHallList[iHallLevel][iHallSlot].m_wOwnGuild;
			cGuild		*lpGuild	=	g_guildManager.GetGuildPntBySerial(iOwnGuild);

			if	(!lpGuild)
				fileLog.writeStream("%d	slot %d	valid[%d]	[%d]\n",iOrder,aGuildList[iOrder].m_iIndex,iOwnGuild,aGuildList[iOrder].m_iValue);
			else
			{
				fileLog.writeStream("%d	slot %d	%s[%d]	[%d]\n",iOrder,aGuildList[iOrder].m_iIndex,lpGuild->m_strName,iOwnGuild,aGuildList[iOrder].m_iValue);	
			}
		}
	}

	fileLog.writeStream("공성측 순위 결정\n");

//	공성측 순위 결정
//	가장 순위가 높은 길드가 가장 순위가 낮은 수성 길드에 시비를 건다.
	for	(iHallLevel=1;iHallLevel<5;iHallLevel++)	//	순위 결정
	{
		iGuildCount	=	0;

		fileLog.writeStream("==> %d레벨 길드홀\n",iHallLevel);

		if	(iHallLevel	==	1)
		{
			for	(iGuild	=	dGUILD_MAX_COUNT;iGuild;)
			{
				--iGuild;

				cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(iGuild);

				if	(!lpGuild	|| lpGuild->wSerial==0xffff)
					continue;

				if	(lpGuild->m_wHallLevel	!=	iHallLevel	||	lpGuild->m_iHonorPoint	==	0)
					continue;

				aGuildList[iGuildCount].m_iIndex	=	iGuild;
				aGuildList[iGuildCount].m_iValue	=	lpGuild->m_iHonorPoint;
				iGuildCount++;
			}
		}
		else
		{
			for	(int iHall=0;iHall<aiHallCountByHallLevel[iHallLevel];iHall++)
			{
				cGuildHallInfo	*lpHallInfo	=	&g_guildManager.m_aHallList[iHallLevel][iHall];
				cGuild			*lpGuild	=	g_guildManager.GetGuildPntBySerial(lpHallInfo->m_wOwnGuild);

				if	(!lpGuild	|| lpGuild->wSerial==0xffff)
					continue;

				if	(lpGuild->m_iHonorPoint	==	0)
					continue;

				aGuildList[iGuildCount].m_iIndex	=	lpHallInfo->m_wOwnGuild;
				aGuildList[iGuildCount].m_iValue	=	lpGuild->m_iHonorPoint;
				iGuildCount++;
			}
		}

		qsort((void *)aGuildList,iGuildCount,sizeof(cIndexForQSort),(int(*)(const void *,const void *))DesCompare);

		int	iChallengeGuildCount	=	aiChallengeGuildCount[iHallLevel];
		iChallengeGuildCount		=	min(iChallengeGuildCount,iGuildCount);

		for	(int iOrder=0;iOrder<iChallengeGuildCount;iOrder++)
		{
			int		iChallengeGuild			=	aGuildList[iOrder].m_iIndex;
			int		iTargetHallOrder		=	aiHallOrderByHonorPoint[iHallLevel+1][iOrder];

			g_guildManager.setInvadeGuild(iHallLevel+1,iTargetHallOrder,iChallengeGuild);

			{
				int		iHallOwner			=	g_guildManager.m_aHallList[iHallLevel+1][iTargetHallOrder].m_wOwnGuild;
				cGuild	*lpOwner			=	g_guildManager.GetGuildPntBySerial(iHallOwner);

				if	(lpOwner	&&	lpOwner->m_iHonorPoint	!=	0)
					lpOwner	=	lpOwner;
			}

			cGuild	*lpGuild			=	g_guildManager.GetGuildPntBySerial(iChallengeGuild);

			if	(!lpGuild)
				fileLog.writeStream("%d	valid[%d]	[%d] vs [%d]\n",iOrder,iChallengeGuild,aGuildList[iOrder].m_iValue);
			else
				fileLog.writeStream("%d	%s[%d]	[%d] vs [%d]\n",iOrder,lpGuild->m_strName,iChallengeGuild,aGuildList[iOrder].m_iValue);	
		}
	}

	fileLog.Close();

//	전투 필드 번호 설정
	{
		int		aiBattleFieldSerial[6]		=	{0,0,912,932,939,941};

		for	(int i=2;i<=c_iMaxGuildHallLevel;i++)
		{
			int	iBattleCount	=	0;

			for	(int j=0;j<aiHallCountByHallLevel[i];j++)
			{
				cGuildHallInfo	*lpHallInfo	=	&g_guildManager.m_aHallList[i][j];

				lpHallInfo->m_bf12BattleField	=	0xffff;

				if	(lpHallInfo->m_wOwnGuild	==	0xffff	||	lpHallInfo->m_wInvadeGuild	==	0xffff)
					continue;

				lpHallInfo->m_bf12BattleField	=	aiBattleFieldSerial[i]+iBattleCount;

				iBattleCount++;
			}
		}
	}

	{
		cFILE	file;

		if	(file.Open(_ms("%s/data/%s_siege_warfare_schedule%d_%.2d_%.2d.txt",
			g_strLogFolder,strWorldName,currentTime.m_wYear,currentTime.m_wMonth,currentTime.m_wDay),"wt"))
		{
			for	(iHallLevel=5;iHallLevel>=2;--iHallLevel)	//	순위 결정
			{
				file.writeStream(">> Hall Level %d\n",iHallLevel);

				for (int iSlot=0;iSlot<aiHallCountByHallLevel[iHallLevel];iSlot++)
				{
					cGuildHallInfo	*lpHallInfo	=	&g_guildManager.m_aHallList[iHallLevel][iSlot];

					cGuild	*lpOwnGuild		=	g_guildManager.GetGuildPntBySerial(lpHallInfo->m_wOwnGuild);
					cGuild	*lpInvadeGuild	=	g_guildManager.GetGuildPntBySerial(lpHallInfo->m_wInvadeGuild);

					if	(lpOwnGuild)
						file.writeStream("  Defense Guild : %s[%d]\n",lpOwnGuild->m_strName,lpOwnGuild->m_iHonorPoint);
					else
						file.writeStream("  Defense Guild : valid guild hall\n");

					if	(lpInvadeGuild)
						file.writeStream("  Attack Guild : %s[%d]\n",lpInvadeGuild->m_strName,lpInvadeGuild->m_iHonorPoint);
					else
						file.writeStream("  Attack Guild : valid\n");

					file.writeStream("  Battle Field : %d\n",lpHallInfo->m_bf12BattleField);

					file.writeStream("\n");
				}

				file.writeStream("---------------------------\n");
			}
		}

		file.Close();
	}

	SendUpdateGuildHallInfo(NULL,TRUE);	//	클라이언트에 길드전 정보 전송

//	빈 길드 공략 길드 자동으로 길드홀 레벨 업!
	for	(iHallLevel=2;iHallLevel<=5;iHallLevel++)	//	홀 별로 명예 포인트를 정렬한다.
	{
		for	(int iHall=0;iHall<aiHallCountByHallLevel[iHallLevel];iHall++)
		{
			cGuildHallInfo	*lpHallInfo	=	&g_guildManager.m_aHallList[iHallLevel][iHall];

			if	(lpHallInfo->m_wOwnGuild	==	0xffff	&&	lpHallInfo->m_wInvadeGuild	!=	0xffff)
				g_guildManager.captureGuildHall(iHallLevel,iHall);
		}
	}

	g_guildBattleManager.save();
}

//
//	엡뒛
BOOL
CGuildBattleManager::updateUserTestSiegeWarfare()
{
	buildProgramOfSiegeWarfare();	//	공성전 일정 빌드

	for(int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(i);

		if	(!lpGuild	|| lpGuild->wSerial==0xffff)
			continue;

		lpGuild->m_bf2TryGuildPointBattleCountPerWeek	=	0;	//	공성전 도전 회수 초기화
		lpGuild->m_iHonorPoint							=	0;	//	길드 명예 포인트 초기화
	}

	save();

	return	TRUE;
}

void
CGuildBattleManager::correctGuildBattle()
{
	// 잘못되어있는 길드전을 수정?한다
	
	for (int i=0;i<dGUILD_BATTLE_COUNT;++i)
	{
		for (int j=0;j<dGUILD_BATTLE_COUNT_IN_SAME_TIME;++j)
		{
			CGuildBattleInfo	*lpInfo	=	&m_gbsToday.m_aBattleGuildSchedule[i].m_aBattleGuildInfo[j];
			
			for (int k=0;k<dBATTLE_GUILD_COUNT_IN_FIELD;++k)
			{
				if(lpInfo->m_awBattleGuild[k] == 0xffff)
					continue;
				cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(lpInfo->m_awBattleGuild[k]);
				if(!lpGuild)
				{	// 길드전은 등록되어있는데 길드가 없어?!! 뭥미
					lpInfo->m_awBattleGuild[k]	=	0xffff;
				}
				
			}
		}
	}
}



//
//	엡뒛
BOOL
CGuildBattleManager::update(BOOL _bIsCheckTime)
{
	CTimeInfo	current;

	current.update();
	correctGuildBattle();
	if	(m_timeLastSiegeWarfareScheduleUpdate.m_wDay !=	current.m_wDay)
	{
		for(int i=0;i<dGUILD_MAX_COUNT;i++)
		{
			cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(i);
			
			if	(!lpGuild	|| lpGuild->wSerial==0xffff)
				continue;
			
			//	명예 포인트에 따라 도전 길드 정할 것

// 			if(lpGuild->m_bf3GuildDungeonPlayTime != 0)			//정보가 바뀌면 저장하고 전송해 준다.
// 			{
// 				lpGuild->m_bf3GuildDungeonPlayTime = 0;
// 
// 				SendSimpleWork(NULL,eSWWG_CHANGE_TRY_GUILD_DUNGEN_COUNT,i,lpGuild->m_bf3GuildDungeonPlayTime);				
// 
// 				SaveGuildBaseData(lpGuild);	//	길드 정보 저장
// 			}
		}
	}

//	주가 넘어가면 싸운 횟수 - dMAX_GUILD_BATTLE_COUNT_AT_WEEK
	if	(m_timeLastSiegeWarfareScheduleUpdate.m_wDay !=	current.m_wDay && current.m_wHour == c_iUpdateGuildWorkHour)
		_bIsCheckTime	=	TRUE;

	if	(g_bIsTestSiegeWarfareOfGuild)
	{
		if	(g_bIsTestSiegeWarfareOfGuild	==	1)
			g_bIsTestSiegeWarfareOfGuild	=	FALSE;
		else
			_bIsCheckTime	=	TRUE;
	}

	if	(_bIsCheckTime)	//	담날 새벽 1시거나 처음 체크다.
	{
		if	(current.m_wWeekDay	==	c_iDownGuildPetGuardianLevelWeekDay)	//	가디언과 펫 경험치 다운
			g_guildManager.decreasePetAndGuardianExp();

		BOOL	bIsSiegeWarfareDay	=	FALSE;

		if	(current.m_wWeekDay	==	c_iSiegeWarfareWeekDay)	//	공성전이 시작되는 날이다.
			bIsSiegeWarfareDay	=	TRUE;
		if	(g_bIsTestSiegeWarfareOfGuild)
			bIsSiegeWarfareDay	=	TRUE;

		if	(current.m_wWeekDay	==	c_iResetSiegeWarfareWeekDay)	//	공성전 다음날
		{
			g_guildManager.resetInvadeGuildInfo();
			SendUpdateGuildHallInfo(NULL);
		}
		else
		if	(bIsSiegeWarfareDay)	//	공성전이 시작되는 날이다.
		{
			buildProgramOfSiegeWarfare();	//	공성전 일정 빌드

			for(int i=0;i<dGUILD_MAX_COUNT;i++)
			{
				cGuild	*lpGuild	=	g_guildManager.GetGuildPntBySerial(i);

				if	(!lpGuild	|| lpGuild->wSerial==0xffff)
					continue;

				lpGuild->m_bf1IsUseGuildMasterGrace				=	0;
				lpGuild->m_bf2TryGuildPointBattleCountPerWeek	=	0;	//	공성전 도전 회수 초기화
				lpGuild->m_iHonorPoint							=	0;	//	길드 명예 포인트 초기화

				SaveGuildBaseData(lpGuild);	//	길드 정보 저장
			}

			SendSimpleWork(NULL,eSWWG_RESET_GUILD_SIEGEWARFARE_INFO);
		}

		m_timeLastSiegeWarfareScheduleUpdate.update();

		resetTodaySchedule();
		g_guildManager.operatePeriodicallyResetInfoWork(current.m_wWeekDay);
		save();

		g_bIsTestSiegeWarfareOfGuild	=	FALSE;

		return	TRUE;
	}

	updateSiegeWarfareSchedule();

	return	FALSE;
}

void
CGuildBattleManager::updateSiegeWarfareSchedule()
{
	CTimeInfo	current;
	static	CTimeInfo	s_lastUpdateTime;

	current.update();

	if	(current.m_wWeekDay	!=	c_iSiegeWarfareWeekDay)
		return;

	if	(current.m_wMinute	==	s_lastUpdateTime.m_wMinute)
		return;

	s_lastUpdateTime.update();

	int	iBattleTime;
	int	iCurrentTime	=	current.m_wHour*60+current.m_wMinute;
	int	iBeginTime		=	c_iSiegeWarfareBeginHour*60+c_iSiegeWarfareBeginMinute;

	for	(iBattleTime=0;iBattleTime<5;iBattleTime++)
	{
		int	iCheckTime	=	iBeginTime+iBattleTime*c_iSiegeWarfarePeriodMinute;

		if	(iCurrentTime	==	iCheckTime - c_iSiegeWarfareBeginNoticeTime)
			SendNoticeBeginSiegeWarfare(NULL,iBattleTime);	//	공성전 준비
		if	(iCurrentTime	==	iCheckTime)
			SendBeginSiegeWarfare(NULL,iBattleTime);	//	공성전 시작
		if	(iCurrentTime	==	iCheckTime+c_iSiegeWarfareUpkeepMinute)
			SendEndSiegeWarfare(NULL,iBattleTime);		//	공성전 종료
		if	(iCurrentTime	==	iCheckTime+c_iSiegeWarfareUpkeepMinute-c_iSiegeWarfareEndNoticeTime)
			SendNoticeEndSiegeWarfare(NULL,iBattleTime);	//	공성전 종료 x분전
	}
}