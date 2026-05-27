#include "CGuild.H"
#include "instance_guild_battle_manager.H"
#include "CActor.H"
#include "CGame.H"
#include "CCompress.H"
#include "packetManager.H"

#define dGUILD_MASTER_GRACE_TIME		7200;

int		g_aiHallCountByHallLevel[]		=	{0,0xffff,27,9,3,1};

CGuildManager			g_guildManager;

int	l_aiGuildHallSerial[10]	=
{
	0,
	942,	//	1레벨
	943,	//	2레벨
	970,	//	3레벨
	979,	//	4레벨
	982,	//	5레벨
};



CGuildManager::CGuildManager()
{
	reset();
}

CGuildManager::~CGuildManager()
{
	reset();
}

//
//	길드마크 아이템 정보 로딩
BOOL
CGuildManager::loadGuildMarkItemInfo()
{
	cFILE	file;

	if	(!file.Open("data/guildMarkInfo.dat","rb"))	
		return	FALSE;

	file.Read(&g_guildMarkItemPackInfo,sizeof(g_guildMarkItemPackInfo));
	file.Close();

	return	TRUE;
}

void
CGuildManager::buildHallInfo()
{
	int		iHallLevel,iHallCount	=	0;

	for (iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
	{
		for	(int iHall=0;iHall<g_aiHallCountByHallLevel[iHallLevel];iHall++)
		{
			cGuildHallInfo	*lpHallInfo	=	&m_aGuildHallList[iHallLevel][iHall];

			cFIELD			*lpField	=	g_game.getFieldByUniqueSerial(lpHallInfo->m_wHallSerial,0xffffffff,-1,-1,TRUE);
			CGuild			*lpGuild	=	getBySerial(lpHallInfo->m_wOwnGuild);

			if	(!lpField)
				continue;

			if	(lpGuild	&&	lpGuild->m_wHallLevel	)
			{
				if	(g_game.isActiveGameServer()	==	FALSE	||	lpField->m_wHallOwnGuild	==	0xffff)	//	유저들이 접속하기 전에만 필드를 로딩한다.
					lpField->loadGuildHall(lpGuild);

				lpField->m_wHallOwnGuild	=	lpHallInfo->m_wOwnGuild;
			}
			else
			{
				if	(g_game.isActiveGameServer()	==	FALSE)
					lpField->loadGuildHall(iHallLevel);

				lpField->m_wHallOwnGuild	=	0xffff;
			}
		}
	}
}

//
//	초기화
void
CGuildManager::reset()
{
	m_iGuildCount			=	0;
	m_iUpdateFrameCounter	=	0;
	m_iLastCheckGuildInfo	=	0;
	m_iReceiveGuildCount	=	0;
	m_iReceiveInventoryCount=	0;
	m_iReceiveMemberCount	=	0;

	for (int i=0;i<dGUILD_MAX_COUNT;i++)
		m_aGuild[i].m_wSerial = 0xffff;

	m_timeLastUpdate.update();

	resetGuildHallInfo();
}

void
CGuildManager::buildSWSOrder()
{
	memset(m_awGuildBattleOrder,0xff,sizeof(m_awGuildBattleOrder));

	for	(int iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
	{
		int	iOrder	=	0;

		for	(int iHall=0;iHall<g_aiHallCountByHallLevel[iHallLevel];iHall++)
		{
			cGuildHallInfo	*lpInfo	=	&m_aGuildHallList[iHallLevel][iHall];

			if	(lpInfo->m_wOwnGuild	==	0xffff	||	lpInfo->m_wInvadeGuild	==	0xffff)
				continue;

			CGuild	*lpDefGuild	=	getBySerial(lpInfo->m_wOwnGuild);
			CGuild	*lpAttGuild	=	getBySerial(lpInfo->m_wInvadeGuild);

			if	(!lpAttGuild)
				continue;

			m_awGuildBattleOrder[iHallLevel][iHall]	=	iOrder;

			iOrder++;
		}
	}
}

//
//	월드에서 받은 길드홀 정보를 설정한다.
void
CGuildManager::updateGuildHallInfo(cGuildHallInfo* _lpList,int _iCount,BOOL _bIsBuildSWS)
{
	resetGuildHallInfo();

	for	(int iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
	{
		int	iHallSlot	=	0;

		for	(int i=0;i<_iCount;i++)
		{
			if	(_lpList[i].m_bf3HallLevel	!=	(DWORD)iHallLevel)
				continue;

			cGuildHallInfo	*lpHallInfo		=	getHall(iHallLevel,iHallSlot);

			memcpy(lpHallInfo,&_lpList[i],sizeof(cGuildHallInfo));

			iHallSlot++;
		}	
	}

	if	(g_game.isActiveGameServer())	//	게임서버가 온전히 돌아가는 도중 받은거다.
		buildHallInfo();

	if	(_bIsBuildSWS)
		buildSWSOrder();
}

//
//	시리얼로 찾기
CGuild*
CGuildManager::getBySerial(int _iSerial)
{
	if	(_iSerial < 0	||	_iSerial	>=	dGUILD_MAX_COUNT)
		return	NULL;

	if	(m_aGuild[_iSerial].m_wSerial	==	_iSerial)
		return	&m_aGuild[_iSerial];

	for (int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		if	(m_aGuild[i].m_wSerial == 0xffff)
			continue;

		if	(m_aGuild[i].m_wSerial == _iSerial)
			return	&m_aGuild[i];
	}

	return	NULL;
}

//
//	인덱스로 찾기
CGuild*
CGuildManager::getByIndex(int _iIndex)
{
	if	(_iIndex < 0 || _iIndex >= dGUILD_MAX_COUNT	)
		return	NULL;

	if	(m_aGuild[_iIndex].m_wSerial == 0xffff		)
		return	NULL;

	return	&m_aGuild[_iIndex];
}

void
CGuildManager::resetGuildHallInfo()
{
	memset(m_aGuildHallList,0xff,sizeof(m_aGuildHallList));
}

//	길드 정보로 길드홀 정보 찾는다.
cGuildHallInfo*	
CGuildManager::getHallInfoByGuild(int _iGuildIndex)
{
	if	(_iGuildIndex	==	0xffff)
		return	NULL;

	for	(int iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
		for	(int iHall=0;iHall<g_aiHallCountByHallLevel[iHallLevel];iHall++)
		{
			cGuildHallInfo	*lpInfo	=	&m_aGuildHallList[iHallLevel][iHall];

			if	(lpInfo->m_wOwnGuild	==	_iGuildIndex)
				return	lpInfo;
		}

	return	NULL;
}

//	공격 길드 정보로 길드홀 정보 찾는다.
cGuildHallInfo*	
CGuildManager::getHallInfoByInvadeGuild(int _iGuildIndex)
{
	if	(_iGuildIndex	==	0xffff)
		return	NULL;

	for	(int iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
		for	(int iHall=0;iHall<g_aiHallCountByHallLevel[iHallLevel];iHall++)
		{
			cGuildHallInfo	*lpInfo	=	&m_aGuildHallList[iHallLevel][iHall];

			if	(lpInfo->m_wOwnGuild	==	0xffff)
				continue;

			if	(lpInfo->m_wInvadeGuild	==	_iGuildIndex)
				return	lpInfo;
		}

	return	NULL;
}

//	공격 길드 정보로 길드홀 정보 찾는다.
cGuildHallInfo*
CGuildManager::getHallInfoByBattleFieldSerial(int _iFieldSerial)
{
	for	(int iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
		for	(int iHall=0;iHall<g_aiHallCountByHallLevel[iHallLevel];iHall++)
		{
			cGuildHallInfo	*lpInfo	=	&m_aGuildHallList[iHallLevel][iHall];

			if	(lpInfo->m_wOwnGuild	==	0xffff)
				continue;

			if	((int)lpInfo->m_bf12BattleField	==	_iFieldSerial)
				return	lpInfo;
		}

	return	NULL;
}

cGuildHallInfo*	
CGuildManager::getHall(int _iHallLevel,int _iHall)
{
	return	&m_aGuildHallList[_iHallLevel][_iHall];
}


//
//	길드 설정
BOOL
CGuildManager::setGuild(int _iIndex,CGuildAdvanceInfo *_lpInfo)
{
	BOOL	bResult	=	TRUE;

	if	(_iIndex < 0 || _iIndex >= dGUILD_MAX_COUNT	)
		return	FALSE;

	if	(m_aGuild[_iIndex].m_wSerial != 0xffff		)
		bResult	=	FALSE;

	CGuild	*lpGuild	=	&m_aGuild[_iIndex];

	memcpy(lpGuild,_lpInfo,sizeof(CGuildAdvanceInfo));

	for	(int i=0;i<lpGuild->m_wMemberCount;i++)
	{
		CGuildMan	*lpMember	=	&lpGuild->m_aMembers[i];

		if	(lpMember->m_wRank	==	dGUILD_CLASS_MASTER	&&	stricmp(lpMember->m_strName,lpGuild->m_strMasterName)	!=	0)
			g_game.sendWPChangeGuildRank(lpGuild,lpMember->m_strName,dGUILD_CLASS_MEMBER);
	}

	m_aGuild[_iIndex].operateGuildSkillData();

	return	bResult;
}

CGuild*
CGuildManager::getByName(char *_lpstrName)
{
	if	(!_lpstrName)
		return	NULL;

	int	i;

	for (i=0;i<dGUILD_MAX_COUNT;i++)
	{
		if (m_aGuild[i].m_wSerial	==	0xffff)	continue;

		if (STRCMP(m_aGuild[i].m_strName,_lpstrName) == 0)
			return	&m_aGuild[i];
	}

	return	NULL;
}

BOOL
CGuildManager::removeGuild(char *_lpstrGuildName)
{
	CGuild	*lpGuild	=	getByName(_lpstrGuildName);

	if	(!lpGuild)
		return	FALSE;

	int iTimeOrder	=	0xffff;
	int iFieldOrder	=	0xffff;

	CGuildBattleInfo* lpGuildBattleInfo	=	g_gbsToday.getBattleInfoByGuild(lpGuild->m_wSerial,&iTimeOrder,&iFieldOrder);

	if(lpGuildBattleInfo)
	{// 길드전스케쥴이 잡혀있으면 취소시킨다!
		g_game.sendWPCancelGuildBattle(lpGuild->m_wSerial,iTimeOrder,iFieldOrder);
		g_game.sendCancelGuildBattleLog(lpGuild->m_strName);
	}

	lpGuild->dissolutionGuild();

	lpGuild->reset();

	m_iGuildCount--;

	return	TRUE;
}


//
//	길드 데이터 체크
BOOL
CGuildManager::checkGuildData()
{
	int	iCount	=	0,i,iMember;

	for (i=0;i<dGUILD_MAX_COUNT;i++)
		if	(m_aGuild[i].m_wSerial	!=	0xffff)
			iCount++;

	if	(iCount	!=	m_iGuildCount)
		ERRMSG("guild init error!!","mismatch guild count %d/%d",iCount,m_iGuildCount);

	for (i=0;i<dGUILD_MAX_COUNT;i++)
	{
		if	(m_aGuild[i].m_wSerial	==	0xffff)
			continue;

		iCount	=	0;

		for (iMember=0;iMember<dGUILD_MAX_PLAYER;iMember++)
			if	(m_aGuild[i].m_aMembers[iMember].wSerial	!=	0xffff)
				iCount++;

		m_aGuild[i].updateGuildInfo();

		if	(m_aGuild[i].m_wSubMasterCount	> m_aGuild[i].m_wMaxSubMaster)
			m_aGuild[i].resetAllMembersRank(dGUILD_CLASS_SUBMASTER);
		if	(m_aGuild[i].m_wCongressManCount	> m_aGuild[i].m_wMaxCongressMan)
			m_aGuild[i].resetAllMembersRank(dGUILD_CLASS_PARLIAMENT);
	}

	return	TRUE;
}

//
//	길드 데이터 체크
void
CGuildManager::update()
{
	{
		CTimeInfo	curTime;

		curTime.update();

		if	(m_iLastCheckWeekDay!=	curTime.m_wWeekDay)
		{
			m_iLastCheckWeekDay	=	curTime.m_wWeekDay;

			if	(m_iLastCheckWeekDay	==	c_iSiegeWarfareWeekDay		||		//	공성전 있는 날이나
				m_iLastCheckWeekDay		==	(c_iSiegeWarfareWeekDay+1)%7	)	//	공성전 다음 날이면
				g_game.resetSiegeWarfareFieldStatus();
		}
	}

	m_iUpdateFrameCounter++;

	if	(m_iUpdateFrameCounter	<	dSYNC_FPS*10 || m_iUpdateFrameCounter < 0)
		return;	//	10초에 한번씩 업데이트

	m_iUpdateFrameCounter	=	0;

	int	i,iSlot=0,iSlotCount;

	SERVERStoWORLD_UPDATE_SERVER_EXP	packet;

	packet.wCount	=	0;
	iSlotCount		=	sizeof(packet.aExpData)/sizeof(CGuildExpInfo);

	packet.base.set(sizeof(packet),dSERVERStoWORLD_UPDATE_SERVER_EXP);

	for (i=0;i<dGUILD_MAX_COUNT;i++)
	{
		CGuild	*lpGuild	=	&m_aGuild[i];

		if	(lpGuild->m_wSerial				==	0xffff	)
			continue;
		if	(lpGuild->m_dwExpInServer		==	0		)
			continue;

		packet.aExpData[iSlot].m_wGuildSerial	=	i;
		packet.aExpData[iSlot].m_dwUpdateExp	=	lpGuild->m_dwExpInServer;
		iSlot++;

		if (iSlot	>=	iSlotCount)
		{
			packet.wCount	=	iSlot;

			packet.base.set(sizeof(packet),dSERVERStoWORLD_UPDATE_SERVER_EXP);

			SEND_WOLRD_PACKET(&packet);

			iSlot	=	0;
		}

		lpGuild->m_dwExpInServer		=	0;
	}

	if (iSlot)
	{
		packet.wCount		=	iSlot;
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aExpData) + sizeof(CGuildExpInfo)*iSlot;
		SEND_WOLRD_PACKET(&packet);
	}
}

int
CGuildManager::getAvailGuildCount()
{
	int	iCount=0;
		
	for (int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		CGuild	*lpGuild	=	&m_aGuild[i];

		if	(lpGuild->m_wSerial	==	0xffff	)
			continue;

		iCount++;
	}

	return	iCount;
}

void
CGuildManager::resetWeekFightInfo()
{
	for (int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		CGuild	*lpGuild	=	&m_aGuild[i];

		if	(lpGuild->m_wSerial	==	0xffff	)
			continue;

		lpGuild->m_bf2TryGuildPointBattleCountPerWeek	=	0;
		lpGuild->m_iHonorPoint							=	0;
	}
}

void
CGuildManager::checkGuildCheckSum()
{
	if	(g_bIsCompleteReceiveGuildDataWork	==	FALSE)
		return;

	if	(g_currentTime.m_wWeekDay	==	c_iSiegeWarfareWeekDay)	//	길드 공성전 결과가 날라올 시간에는 체크섬을 보내지 않는다.
	{
		if	(g_currentTime.m_wHour	>=	c_iSiegeWarfareBeginHour	&&	
			g_currentTime.m_wMinute	>=	c_iSiegeWarfareUpkeepMinute-5	&&	g_currentTime.m_wMinute	<	c_iSiegeWarfareUpkeepMinute+5	)
			return;
	}

	int		iCount	=	0;
	WORD	awCheckGuild[10];
	int		aiCheckSum[10];

	for (m_iLastCheckGuildInfo;m_iLastCheckGuildInfo<dGUILD_MAX_COUNT;m_iLastCheckGuildInfo++)
	{
		CGuild	*lpGuild	=	&m_aGuild[m_iLastCheckGuildInfo];

		awCheckGuild[iCount]	=	m_iLastCheckGuildInfo;

		if	(awCheckGuild[iCount]	>=	dGUILD_MAX_COUNT)
			ERRMSG("이팀장한테 연락해 주삼","길드 시렬 %d-%d",iCount,m_iLastCheckGuildInfo);

		if	(lpGuild->m_wSerial	==	0xffff	)
			aiCheckSum[iCount]	=	0;
		else
			aiCheckSum[iCount]	=	lpGuild->getCheckSum();

		iCount++;

		if (iCount	>=	10)
		{
			m_iLastCheckGuildInfo++;
			break;
		}
	}

	DWORD	dwTodayGBSCheckSum	=	g_gbsToday.getCheckSum();

	if	(m_iLastCheckGuildInfo	>=	dGUILD_MAX_COUNT)
		m_iLastCheckGuildInfo	=	0;

	g_game.sendWPCheckGuildData(awCheckGuild,aiCheckSum,iCount,dwTodayGBSCheckSum);
}

void
CGuildManager::changeGuildMarkTwinkleStatus(int _iGuild,BOOL _bIsTwinkle)
{
	if	(_iGuild	==	0xffff)
	{
		for (int i=0;i<dGUILD_MAX_COUNT;i++)
		{
			if	(m_aGuild[i].m_wSerial	==	0xffff)
				continue;

			m_aGuild[i].m_bf1IsTwinkle	=	FALSE;
		}

		return;
	}

	CGuild	*lpGuild	=	getBySerial(_iGuild);

	if	(lpGuild)
		lpGuild->m_bf1IsTwinkle	=	_bIsTwinkle;
}

void
CGuildManager::resetGuildHonorPoint()
{
	int	i;

	for (i=0;i<dGUILD_MAX_COUNT;i++)
	{
		if	(m_aGuild[i].m_wSerial	==	0xffff)
			continue;

		m_aGuild[i].m_iHonorPoint	=	0;
	}
}

void
CGuildManager::resetGuildPrologPoint()
{
	int	i;

	for (i=0;i<dGUILD_MAX_COUNT;i++)
	{
		if	(m_aGuild[i].m_wSerial	==	0xffff)
			continue;

		m_aGuild[i].m_iVictoryPoint	=	0;
	}
}

void
CGuildManager::changeGuildHallOpenStatus(int _iGuild,int _iStatus)
{
	CGuild	*lpGuild	=	getBySerial(_iGuild);

	if	(!lpGuild)
		return;

	lpGuild->m_bf1IsPermitVisitor	=	_iStatus;

	cACTOR	*lpMaster	=	g_game.getPlayer(lpGuild->m_strMasterName);

	if	(lpMaster)
		lpMaster->sendEtcWork(eEW_CHANGE_OPEN_GUILD_HALL_STATUS,_iStatus);
}

//
//함수이름은 actrive지만 값을 세팅한다..
void
CGuildManager::activeGuildMasterGrace(int _iGuild , int _iState)
{
	CGuild	*lpGuild	=	getBySerial(_iGuild);

	if	(!lpGuild)
		return;

	lpGuild->m_bf1IsUseGuildMasterGrace	 =	_iState;

	SG_GUILD_MASTER_GRACE_INFO		packet;	
	
	packet.base.set(sizeof(packet),dSG_GUILD_MASTER_GRACE_INFO);
	packet.wWork = eGMG_ACTIVE_GUILD_MASTER_GRACE;

	for (int i = 0 ; i < dGUILD_MAX_PLAYER ; i++)
	{
		CGuildMan	*lpGuildMan		=	lpGuild->getMember(i);

		if(!lpGuildMan)
			continue;

		cACTOR	*lpMember = g_game.getPlayer(lpGuildMan->m_strName);

		if(!lpMember)
			continue;

		lpMember->m_wGuildMasterGrace = dGUILD_MASTER_GRACE_TIME;

		g_userPM.add(lpMember->m_iClientSerial,&packet,packet.base.wSize);
	}
}


//	펫 경험치 업데이트
void
CGuildManager::updateGuildPetExp(int _iGuild,int _iPetType,int _iExp,int _iExpDuringWeek)
{
	CGuild	*lpGuild	=	getBySerial(_iGuild);

	if	(!lpGuild)
	{
		for (int i=0;i<dGUILD_MAX_COUNT;i++)
		{
			lpGuild	=	getBySerial(i);

			if	(!lpGuild)
				continue;


			for	(int iIndex=0;iIndex<dGUILD_PET_TYPE_COUNT;iIndex++)
			{
				lpGuild->m_aGuildGuardian[iIndex].m_wExpDuringWeek	=	0;
				lpGuild->m_aGuildPet[iIndex].m_wExpDuringWeek		=	0;
			}
		}

		return;
	}

	if	(_iPetType	>=	dGUILD_PET_TYPE_COUNT)
	{
		_iPetType	-=	dGUILD_PET_TYPE_COUNT;

		lpGuild->m_aGuildGuardian[_iPetType].m_dwExperience		=	_iExp;
		lpGuild->m_aGuildGuardian[_iPetType].m_wExpDuringWeek	=	_iExpDuringWeek;
	}
	else
	{
		lpGuild->m_aGuildPet[_iPetType].m_dwExperience		=	_iExp;
		lpGuild->m_aGuildPet[_iPetType].m_wExpDuringWeek	=	_iExpDuringWeek;
	}
}

void
CGuildManager::rentGuildPet(int _iGuild,int _iPetType,int _iMember)//	펫 대여
{
	CGuild	*lpGuild	=	getBySerial(_iGuild);

	if	(!lpGuild)
		return;

	CGuildMan	*lpMember=	lpGuild->getMember(_iMember);

	if	(!lpMember)
		return;

	lpMember->m_bf3ReservePet	=	_iPetType;

	lpGuild->sendUpdateMemberInfo(lpMember);

	cACTOR	*lpActor	=	g_game.getPlayer(lpMember->m_strName);

	if	(lpActor)
		if	(lpActor->m_lpField->m_bIsSiegeWarfareField)
		{
			lpActor->m_wGuildPetType				=	_iPetType;
			lpActor->m_wGuildPetLevel				=	lpGuild->m_awPetLevel[_iPetType];
			lpActor->m_dwLastActionTimeForGuildPet	=	0;

			lpActor->addSendVerySimpleInfo(lpActor);
		}
}

void
CGuildManager::retrievalGuildPet(int _iGuild,int _iMember)//	대여 한 펫 회수
{
	CGuild	*lpGuild	=	getBySerial(_iGuild);

	if	(!lpGuild)
		return;

	CGuildMan	*lpMember=	lpGuild->getMember(_iMember);

	if	(!lpMember)
		return;

	lpMember->m_bf3ReservePet	=	0;

	lpGuild->sendUpdateMemberInfo(lpMember);

	cACTOR	*lpActor	=	g_game.getPlayer(lpMember->m_strName);

	if	(lpActor)
		if	(lpActor->m_lpField->m_bIsSiegeWarfareField)
		{
			lpActor->m_wGuildPetType	=	0;
			lpActor->addSendVerySimpleInfo(lpActor);
		}
}

//
//	
int
CGuildManager::getGuildHallSlotIndexByHallLevel(int _iHallLevel,int _iGuildIndex)
{
	for	(int iHall=0;iHall<g_aiHallCountByHallLevel[_iHallLevel];iHall++)
	{
		cGuildHallInfo	*lpInfo	=	&m_aGuildHallList[_iHallLevel][iHall];

		if	(lpInfo->m_wOwnGuild	==	0xffff)
			break;

		if	(lpInfo->m_wOwnGuild	==	_iGuildIndex)
			return	iHall;
	}

	return	-1;
}

//
//	필드 시리얼로 길드 주인 알아냄
int
CGuildManager::getGuildHallOwnerGuildByFieldSerial(int _iGuildHallFieldSerial)
{
	if	(_iGuildHallFieldSerial	<	c_i2LevelGuildHallFieldSerial	||	_iGuildHallFieldSerial	>	c_i5LevelGuildHallFieldSerial)
		return	-1;

	for	(int iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
		for	(int iHall=0;iHall<g_aiHallCountByHallLevel[iHallLevel];iHall++)
		{
			cGuildHallInfo	*lpInfo	=	&m_aGuildHallList[iHallLevel][iHall];

			if	(lpInfo->m_wHallSerial	==	_iGuildHallFieldSerial)
				return	lpInfo->m_wOwnGuild;
		}

	return	-1;
}

//
//	길드홀의 필드 시리얼 리턴
int
CGuildManager::getGuildHallFieldSerial(CGuild *_lpGuild)
{
	int	iFieldSerial	=	-1;

	if	(_lpGuild->m_wHallLevel	==	0)
		return	-1;

	if	(_lpGuild->m_wHallLevel	==	1)
		iFieldSerial	=	l_aiGuildHallSerial[_lpGuild->m_wHallLevel];
	else
	{
		cGuildHallInfo	*lpHallInfo	=	getHallInfoByGuild(_lpGuild->m_wSerial);

		if	(lpHallInfo	==	NULL)
			return	-1;

		iFieldSerial	=	lpHallInfo->m_wHallSerial;
	}
	
	return	iFieldSerial;
}

//	길드전 시간 구하기
int
CGuildManager::getSiegeWarfareOfGuildOrder(int _iHallLevel,int _iIndex)
{
	int	iOrder	=	m_awGuildBattleOrder[_iHallLevel][_iIndex];

	if	(iOrder	==	0xffff)
		return	-1;

	switch(_iHallLevel)
	{
		case	2	:	//	2 vs 1
		{
			if	(iOrder	>=	0	&&	iOrder	<	10)
				return	3;
			if	(iOrder	>=	10	&&	iOrder	<	20)
				return	4;
			break;
		}

		case	3	:	//	3 vs 2
			return	2;
			break;

		case	4	:	//	4 vs 3
			return	1;
			break;

		case	5	:	//	5 vs 4
			return	0;
			break;
	}

	return	-1;
}

void
CGuildManager::getGuildHallInfoByBattleOrder(int _iBattleOrder,int *_lpiGuildHallLevel,int *_lpiHallIndex)
{
	int	iHallIndex,iHallLevel;
	int	aiBattleFieldCountByHallLevel[]	=	{0,0xffff,20,7,2,1};

	switch(_iBattleOrder)
	{
		case	0	:
			iHallLevel	=	5;
			iHallIndex	=	0;
			break;
		case	1	:
			iHallLevel	=	4;
			iHallIndex	=	0;
			break;
		case	2	:
			iHallLevel	=	3;
			iHallIndex	=	0;
			break;
		case	3	:
			iHallLevel	=	2;
			iHallIndex	=	0;
			break;
		case	4	:
			iHallLevel	=	2;
			iHallIndex	=	10;
			break;
	}

	*_lpiGuildHallLevel	=	iHallLevel;
	*_lpiHallIndex		=	iHallIndex;
}

BOOL
CGuildManager::noticeBeginSiegeWarfare(int _iBattleOrder)	//	길드 공성전 시작전 10분 공지. 이때부터 길드전 공성 필드 입장이 가능하다.
{
	if	(_iBattleOrder	==	0xffff)
	{
		for (int i=0;i<5;i++)
			noticeBeginSiegeWarfare(i);

		return	TRUE;
	}

	int	iHallBeginIndex,iHallLevel;

	getGuildHallInfoByBattleOrder(_iBattleOrder,&iHallLevel,&iHallBeginIndex);

	int	iEndHallIndex	=	iHallBeginIndex+10;

	for	(int i=0;i<g_aiHallCountByHallLevel[iHallLevel];i++)
	{
		cGuildHallInfo	*lpHallInfo	=	NULL;
		int				iOrder		=	m_awGuildBattleOrder[iHallLevel][i];

		if	(iOrder	>=	iHallBeginIndex	&&	iOrder	<	iEndHallIndex)
			lpHallInfo	=	&m_aGuildHallList[iHallLevel][i];
		else
			continue;

		if	(lpHallInfo->m_wOwnGuild	==	0xffff)
			continue;
		if	(lpHallInfo->m_wInvadeGuild	==	0xffff)
			continue;

		CGuild	*lpDefenseGuild	=	getBySerial(lpHallInfo->m_wOwnGuild);
		CGuild	*lpInvadeGuild	=	getBySerial(lpHallInfo->m_wInvadeGuild);

		if	(!lpDefenseGuild	||	!lpInvadeGuild)
			continue;

		SG_SIEGE_WARFARE_NOTICE	packet;

		packet.base.set(sizeof(packet),dSG_SIEGE_WARFARE_NOTICE);
		packet.wWork		=	eSW_BEGIN_NOTICE;
		packet.wValue		=	c_iSiegeWarfareBeginNoticeTime;

		strcpy(packet.strGuildName,lpDefenseGuild->m_strName);
		lpInvadeGuild->addSendPacketToAllMember(&packet);

		strcpy(packet.strGuildName,lpInvadeGuild->m_strName);
		lpDefenseGuild->addSendPacketToAllMember(&packet);
	}

	return	TRUE;
}

BOOL
CGuildManager::noticeEndSiegeWarfare(int _iBattleOrder)	//	길드 공성전 끝나기 5분 공지.
{
	if	(_iBattleOrder	==	0xffff)
	{
		for (int i=0;i<5;i++)
			noticeEndSiegeWarfare(i);

		return	TRUE;
	}

	int	iHallBeginIndex,iHallLevel;

	getGuildHallInfoByBattleOrder(_iBattleOrder,&iHallLevel,&iHallBeginIndex);

	int	iEndHallIndex	=	iHallBeginIndex+10;

	for	(int i=0;i<g_aiHallCountByHallLevel[iHallLevel];i++)
	{
		cGuildHallInfo	*lpHallInfo	=	NULL;
		int				iOrder		=	m_awGuildBattleOrder[iHallLevel][i];

		if	(iOrder	>=	iHallBeginIndex	&&	iOrder	<	iEndHallIndex)
			lpHallInfo	=	&m_aGuildHallList[iHallLevel][i];
		else
			continue;

		if	(lpHallInfo->m_wOwnGuild	==	0xffff)
			continue;
		if	(lpHallInfo->m_wInvadeGuild	==	0xffff)
			continue;

		CGuild	*lpDefenseGuild	=	getBySerial(lpHallInfo->m_wOwnGuild);
		CGuild	*lpInvadeGuild	=	getBySerial(lpHallInfo->m_wInvadeGuild);

		if	(!lpDefenseGuild	||	!lpInvadeGuild)
			continue;

		SG_SIEGE_WARFARE_NOTICE	packet;

		packet.base.set(sizeof(packet),dSG_SIEGE_WARFARE_NOTICE);
		packet.wWork		=	eSW_END_NOTICE;
		packet.wValue		=	c_iSiegeWarfareEndNoticeTime;

		strcpy(packet.strGuildName,lpDefenseGuild->m_strName);
		lpInvadeGuild->addSendPacketToAllMember(&packet);

		strcpy(packet.strGuildName,lpInvadeGuild->m_strName);
		lpDefenseGuild->addSendPacketToAllMember(&packet);
	}

	return	TRUE;
}

BOOL
CGuildManager::beginSiegeWarfare(int _iBattleOrder)	//	길드 공성전 시작. 잠긴 문을 연다.
{
	if	(_iBattleOrder	==	0xffff)
	{
		for (int i=0;i<5;i++)
			beginSiegeWarfare(i);

		return	TRUE;
	}

	int	iHallBeginIndex,iHallLevel;

	getGuildHallInfoByBattleOrder(_iBattleOrder,&iHallLevel,&iHallBeginIndex);

	int	iEndHallIndex	=	iHallBeginIndex+10;

	for	(int i=0;i<g_aiHallCountByHallLevel[iHallLevel];i++)
	{
		cGuildHallInfo	*lpHallInfo	=	NULL;
		int				iOrder		=	m_awGuildBattleOrder[iHallLevel][i];

		if	(iOrder	>=	iHallBeginIndex	&&	iOrder	<	iEndHallIndex)
			lpHallInfo	=	&m_aGuildHallList[iHallLevel][i];
		else
			continue;

		if	(lpHallInfo->m_wOwnGuild	==	0xffff)
			continue;
		if	(lpHallInfo->m_wInvadeGuild	==	0xffff)
			continue;

		CGuild	*lpDefenseGuild	=	getBySerial(lpHallInfo->m_wOwnGuild);
		CGuild	*lpInvadeGuild	=	getBySerial(lpHallInfo->m_wInvadeGuild);

		if	(!lpDefenseGuild	||	!lpInvadeGuild)
			continue;

		SG_SIEGE_WARFARE_NOTICE	packet;

		packet.base.set(sizeof(packet),dSG_SIEGE_WARFARE_NOTICE);
		packet.wWork		=	eSW_BEGIN;
		packet.wValue		=	c_iSiegeWarfareEndNoticeTime;

		strcpy(packet.strGuildName,lpDefenseGuild->m_strName);
		lpInvadeGuild->addSendPacketToAllMember(&packet);

		strcpy(packet.strGuildName,lpInvadeGuild->m_strName);
		lpDefenseGuild->addSendPacketToAllMember(&packet);

		cFIELD	*lpField		=	g_game.getFieldByUniqueSerial(lpHallInfo->m_bf12BattleField);

		if	(lpField)
			lpField->beginSiegeWarfare();
	}

	return	TRUE;
}

BOOL
CGuildManager::endSiegeWarfare(int _iBattleOrder)//	길드 공성전 종료
{
	if	(_iBattleOrder	==	0xffff)
	{
		for (int i=0;i<5;i++)
			endSiegeWarfare(i);

		return	TRUE;
	}

	int	iHallBeginIndex,iHallLevel;

	getGuildHallInfoByBattleOrder(_iBattleOrder,&iHallLevel,&iHallBeginIndex);

	int	iEndHallIndex	=	iHallBeginIndex+10;

	for	(int i=0;i<g_aiHallCountByHallLevel[iHallLevel];i++)
	{
		cGuildHallInfo	*lpHallInfo	=	NULL;
		int				iOrder		=	m_awGuildBattleOrder[iHallLevel][i];

		if	(iOrder	>=	iHallBeginIndex	&&	iOrder	<	iEndHallIndex)
			lpHallInfo	=	&m_aGuildHallList[iHallLevel][i];
		else
			continue;

		if	(lpHallInfo->m_wOwnGuild	==	0xffff)
			continue;
		if	(lpHallInfo->m_wInvadeGuild	==	0xffff)
			continue;

		CGuild	*lpDefenseGuild	=	getBySerial(lpHallInfo->m_wOwnGuild);
		CGuild	*lpInvadeGuild	=	getBySerial(lpHallInfo->m_wInvadeGuild);

		if	(!lpDefenseGuild	||	!lpInvadeGuild)
			continue;

		SG_SIEGE_WARFARE_NOTICE	packet;

		packet.base.set(sizeof(packet),dSG_SIEGE_WARFARE_NOTICE);
		packet.wWork		=	eSW_END;
		packet.wValue		=	c_iSiegeWarfareEndNoticeTime;

		strcpy(packet.strGuildName,lpDefenseGuild->m_strName);
		lpInvadeGuild->addSendPacketToAllMember(&packet);

		strcpy(packet.strGuildName,lpInvadeGuild->m_strName);
		lpDefenseGuild->addSendPacketToAllMember(&packet);

		cFIELD	*lpField		=	g_game.getFieldByUniqueSerial(lpHallInfo->m_bf12BattleField);

		if	(lpField)
			lpField->reportSuccessToDefense();
	}

	return	TRUE;
}

//	방어 성공
void
CGuildManager::operateSiegeWarfareResultDefenseHall(int _iHallLevel,int _iSlot)
{
	cGuildHallInfo	*lpHallInfo		=	&m_aGuildHallList[_iHallLevel][_iSlot];

	CGuild			*lpInvadeGuild	=	getBySerial(lpHallInfo->m_wInvadeGuild);
	CGuild			*lpDefenseGuild	=	getBySerial(lpHallInfo->m_wOwnGuild);

	SG_SIEGE_WARFARE_NOTICE	packet;

	packet.base.set(sizeof(packet),dSG_SIEGE_WARFARE_NOTICE);

	if	(lpDefenseGuild)
	{
		packet.wWork		=	eSW_SUCCESS_TO_DEFENSE;
		lpDefenseGuild->addSendPacketToAllMember(&packet);
	}

	if	(lpInvadeGuild)
	{
		packet.wWork		=	eSW_FAILED_TO_INVADE;
		lpInvadeGuild->addSendPacketToAllMember(&packet);
	}

	lpHallInfo->m_wInvadeGuild	=	0xffff;
}

//	공성 성공
void
CGuildManager::operateSiegeWarfareResultCaptureHall(int _iHallLevel,int _iSlot)
{
	cGuildHallInfo	*lpHallInfo		=	&m_aGuildHallList[_iHallLevel][_iSlot];

	CGuild			*lpInvadeGuild	=	getBySerial(lpHallInfo->m_wInvadeGuild);
	CGuild			*lpDefenseGuild	=	getBySerial(lpHallInfo->m_wOwnGuild);

	SG_SIEGE_WARFARE_NOTICE	packet;

	packet.base.set(sizeof(packet),dSG_SIEGE_WARFARE_NOTICE);

	if	(lpDefenseGuild)
	{
		cGuildHallInfo	*lpLowHall		=	getHallInfoByGuild(lpHallInfo->m_wInvadeGuild);

		lpDefenseGuild->m_wHallLevel	=	(WORD)lpHallInfo->m_bf3HallLevel-1;
		lpDefenseGuild->m_wGuildHallShape=	0;

		if	(lpLowHall)
		{
			lpLowHall->m_wOwnGuild	=	lpDefenseGuild->m_wSerial;
			lpLowHall->m_dwHashCode	=	GetHashCode(lpDefenseGuild->m_strName);
			lpLowHall->m_bf1IsOpen	=	lpDefenseGuild->m_bf1IsPermitVisitor;	//	열려 있냐?
			lpLowHall->m_bf3HallShape=	0;	//	홀 모양

			cFIELD	*lpGuildHall	=	g_game.getFieldByUniqueSerial(lpLowHall->m_wHallSerial);

			if	(lpGuildHall)
			{
				lpGuildHall->banishGuildHallPlayer();
				lpGuildHall->m_wHallOwnGuild	=	lpDefenseGuild->m_wSerial;
			}
		}

		packet.wWork				=	eSW_FAILED_TO_DEFENSE;
		lpDefenseGuild->addSendPacketToAllMember(&packet);
	}

	if	(lpInvadeGuild)
	{
		lpInvadeGuild->m_wGuildHallShape=	0;
		lpInvadeGuild->m_wHallLevel		=	lpHallInfo->m_bf3HallLevel;

		lpHallInfo->m_wOwnGuild			=	lpInvadeGuild->m_wSerial;
		lpHallInfo->m_dwHashCode		=	GetHashCode(lpInvadeGuild->m_strName);
		lpHallInfo->m_bf1IsOpen			=	lpInvadeGuild->m_bf1IsPermitVisitor;	//	열려 있냐?
		lpHallInfo->m_bf3HallShape		=	0;	//	홀 모양

		cFIELD	*lpGuildHall	=	g_game.getFieldByUniqueSerial(lpHallInfo->m_wHallSerial);

		if	(lpGuildHall)
		{
			lpGuildHall->banishGuildHallPlayer();
			lpGuildHall->m_wHallOwnGuild	=	lpInvadeGuild->m_wSerial;
		}

		packet.wWork		=	eSW_SUCCESS_TO_INVADE;
		lpInvadeGuild->addSendPacketToAllMember(&packet);
	}

	lpHallInfo->m_wInvadeGuild	=	0xffff;
}

//	길드전 정보
BOOL
CGuildManager::getSiegeWarFareInfoInCurrentTime(int _iGuild,cSiegeWarfareInfoOfGuild *_lpInfo,BOOL _bIsWantBattleTime)
{
	CGuild	*lpGuild		=	getBySerial(_iGuild);

	if	(!lpGuild)
		return	FALSE;

	CTimeInfo	currentTime;

	currentTime.update();

	if	(g_bIsTestSiegeWarfare	==	FALSE	&&	g_bIsChansTestServer	==	FALSE)
	{
		int	iCurrentDayPoint	=	g_currentTime.m_wYear*1000 +g_currentTime.m_wMonth*100+ g_currentTime.m_wDay;

		if	(currentTime.m_wWeekDay	!=	c_iSiegeWarfareWeekDay)
			return	FALSE;

		//	공성전 일정이 결정되고 10분이 지나야 입장 가능하다.
		if	(currentTime.m_wHour*60+currentTime.m_wMinute	<	c_iUpdateGuildWorkHour*60+10)	
			return	FALSE;
	}

	cGuildHallInfo*	lpHallInfo				=	getHallInfoByGuild(_iGuild);
	int				iFirstBattleBeginTime	=	c_iSiegeWarfareBeginHour*60+c_iSiegeWarfareBeginMinute;
	int				iCurrentTime			=	currentTime.m_wHour*60+currentTime.m_wMinute;

	if	(lpHallInfo	&&	lpHallInfo->m_wInvadeGuild	!=	0xffff)	//	방어전을 치루게 된다.
	{
		int	iOrder				=	getSiegeWarfareOfGuildOrder(lpHallInfo->m_bf3HallLevel,lpHallInfo->m_bf5Slot);

		if	(iOrder	==	-1)
			return	FALSE;

		int	iBeginTime			=	iFirstBattleBeginTime+iOrder*c_iSiegeWarfarePeriodMinute;

		_lpInfo->m_wInvadeGuild	=	lpHallInfo->m_wInvadeGuild;
		_lpInfo->m_wDefenseGuild=	lpHallInfo->m_wOwnGuild;
		_lpInfo->m_wBattleField	=	lpHallInfo->m_bf12BattleField;
		_lpInfo->m_bHour		=	iBeginTime/60;
		_lpInfo->m_bMinute		=	iBeginTime%60;

		return	TRUE;
	}

	lpHallInfo	=	getHallInfoByInvadeGuild(_iGuild);

	if	(lpHallInfo)	//	공성전을 치루게 된다.
	{
		int	iOrder		=	getSiegeWarfareOfGuildOrder(lpHallInfo->m_bf3HallLevel,lpHallInfo->m_bf5Slot);

		if	(iOrder	==	-1)
			return	FALSE;

		int	iEnterTime	=	iFirstBattleBeginTime+iOrder*c_iSiegeWarfarePeriodMinute-c_iSiegeWarfareBeginNoticeTime-1;	//	실제 입장은 공지 1분전부터 가능하다.
		int	iBeginTime	=	iFirstBattleBeginTime+iOrder*c_iSiegeWarfarePeriodMinute;

		if	(g_bIsTestSiegeWarfare	||	(iCurrentTime	>=	iEnterTime		||	_bIsWantBattleTime))
		{
			_lpInfo->m_wInvadeGuild	=	lpHallInfo->m_wInvadeGuild;
			_lpInfo->m_wDefenseGuild=	lpHallInfo->m_wOwnGuild;
			_lpInfo->m_wBattleField	=	lpHallInfo->m_bf12BattleField;
			_lpInfo->m_bHour		=	iBeginTime/60;
			_lpInfo->m_bMinute		=	iBeginTime%60;

			return	TRUE;
		}
		else
		{
			CLOG("gb","current Time value %d [%d:%d]",iCurrentTime,currentTime.m_wHour,currentTime.m_wMinute);
			CLOG("gb","Enter Time Value %d [%d:%d]",iEnterTime,iBeginTime/60,iBeginTime%60);
		}
	}

	return	FALSE;
}

//	길드홀 정보 체크섬
DWORD
CGuildManager::getGuildHallCheckSum()
{
	DWORD	dwCheckSum	=	0;

	for	(int i=2;i<=c_iMaxGuildHallLevel;i++)
		for	(int j=0;j<g_aiHallCountByHallLevel[i];j++)
		{
			cGuildHallInfo	*lpHallInfo	=	&m_aGuildHallList[i][j];

			if	(lpHallInfo->m_wOwnGuild==	0xffff)
				continue;

			DWORD	dwCheckValue;
			cCRC32::BufferToCrc32(lpHallInfo,sizeof(cGuildHallInfo),dwCheckValue);

			dwCheckSum	+=	dwCheckValue;
		}

	return	dwCheckSum;
}

void
CGuildManager::resetGuildSiegewarfareInfo()
{
	for(int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		CGuild	*lpGuild	=	getBySerial(i);

		if	(!lpGuild	|| lpGuild->m_wSerial==0xffff)
			continue;

		lpGuild->m_bf1IsUseGuildMasterGrace				=	0;
		lpGuild->m_bf2TryGuildPointBattleCountPerWeek	=	0;	//	공성전 도전 회수 초기화
		lpGuild->m_iHonorPoint							=	0;	//	길드 명예 포인트 초기화
	}
}

void
CGuildManager::reset1DayGuildInfo()
{
	for(int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		CGuild	*lpGuild	=	getBySerial(i);

		if	(!lpGuild	|| lpGuild->m_wSerial==0xffff)
			continue;

		lpGuild->m_bf3ThisDayGuildBattleCount	=	0;	//	오늘 길드 배틀한 회수 초기화
	}
}

void
CGuildManager::reset1WeekGuildInfo()
{
	for(int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		CGuild	*lpGuild	=	getBySerial(i);

		if	(!lpGuild	|| lpGuild->m_wSerial==0xffff)
			continue;

		lpGuild->m_bf3ThisDayGuildBattleCount	=	0;	//	오늘 길드 배틀한 회수 초기화
		lpGuild->m_bf5ThisWeekGuildBattleCount	=	0;	//	이번주 길드 배틀 횟수
	}
}
