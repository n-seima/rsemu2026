#include "duel.h"
#include "cACTOR.h"
#include "cGame.h"
#include "booking_work.h"

cDuelManager		*g_pDuelManager	=	NULL;
cDuelRankerManager	g_duelRankerManager;
cTrialGameManager	g_duelTrialGameManager;
int					g_aiDuelFieldCount[6];		//	결투 필드의 수
int					g_iTrialGameTeamSize					=	0;
int					g_iCorrectHealEfficientUnderDuelBattle	=	25;
int					g_iDuelFieldSerial	=	-1;

int	
GetMemberCountForDuelMatch(int _iType)	//	결투 시간 얻기
{
	const	int	c_aiMemberCountForDuelMatch[eDUEL_MATCH_TYPE_COUNT]	=	{1,2,3,4,5,6};

	if	(_iType	<	eDUEL_TRIAL_GAME)
		return	c_aiMemberCountForDuelMatch[_iType];
	
	return	c_aiMemberCountForDuelMatch[g_iTrialGameTeamSize-1];
}

int	
GetDuelBattleTime(int _iType)	//	결투 시간 얻기
{
	const	int	c_aiDuelBattleTime[5]	=	{180,240,300,360,420};	//	결투 시간

	if	(_iType	<	eDUEL_TRIAL_GAME)
		return	c_aiDuelBattleTime[_iType];

	return	c_aiDuelBattleTime[g_iTrialGameTeamSize-1];
}

int	
GetKillCountForDuel(int _iType)//	결투 시간 얻기
{
	const	int	c_aiKillCountForDuel[5]	=	{5,10,15,20,25};		//	결투 승리를 위한 킬 수
	
	if	(_iType	<	eDUEL_TRIAL_GAME)
		return	c_aiKillCountForDuel[_iType];
	
	return	c_aiKillCountForDuel[g_iTrialGameTeamSize-1];
}

//	결투 승점 얻기
int
GetWinPointForDuel(int _iType,BOOL _bIsLoser)
{
	const	int	c_aiWinPointForDuel[5][2]	=	
	{
		{12,0},
		{14,0},
		{16,0},
		{20,0},
		{24,0}
	};
	
	if	(_iType	<	eDUEL_TRIAL_GAME)
		return	c_aiWinPointForDuel[_iType][_bIsLoser];
	
	return	c_aiWinPointForDuel[g_iTrialGameTeamSize-1][_bIsLoser];
}

int	
GetDuelPointForDuel(int _iType,BOOL _bIsLoser)
{
	const	int	c_aiDuelPointForDuel[5][2]	=	
	{
		{2,1},
		{4,1},
		{6,2},
		{8,2},
		{10,3},
	};

	if	(_iType	<	eDUEL_TRIAL_GAME)
		return	c_aiDuelPointForDuel[_iType][_bIsLoser];
	
	return	c_aiDuelPointForDuel[g_iTrialGameTeamSize-1][_bIsLoser];
}

cTrialGameManager::cTrialGameManager()
{
	m_wTotalTeamCount	=	0;
	m_wTeamCount		=	0;
	m_pTeams			=	NULL;
}

cTrialGameManager::~cTrialGameManager()
{
	pKILL(m_pTeams);
}

void
cTrialGameManager::init(int _iSize)
{
	if	(m_pTeams)
		return;

	m_wTotalTeamCount	=	_iSize;

	m_pTeams	=	new cAdvDuelTeamInfo [_iSize];
}

bool
cTrialGameManager::addTeam(cDuelTeamInfo *_lpTeamInfo)
{
	if	(m_pTeams	==	NULL	||	m_wTeamCount	>=	m_wTotalTeamCount)
		return	false;

	memcpy(&m_pTeams[m_wTeamCount],_lpTeamInfo,sizeof(cDuelTeamInfo));

	for (int iMember=0;iMember<_lpTeamInfo->m_wMemberCount;iMember++)
		m_pTeams[m_wTeamCount].m_uiMemberHashCode[iMember]	=	GetHashCode(m_pTeams[m_wTeamCount].m_aMembers[iMember].m_strID);

	m_wTeamCount++;

	return	true;
}

cAdvDuelTeamInfo*
cTrialGameManager::getTeam(int _iSlot)
{
	if	(_iSlot	>=	m_wTeamCount)
		return	NULL;

	return	&m_pTeams[_iSlot];
}

cAdvDuelTeamInfo*
cTrialGameManager::getTeamByMember(char *_lpstrName)
{
	UINT	uiCRC32	=	GetHashCode(_lpstrName);

	for (int i=0;i<m_wTeamCount;i++)
	{
		cAdvDuelTeamInfo	*lpTeam	=	&m_pTeams[i];

		for (int iMember=0;iMember<lpTeam->m_wMemberCount;iMember++)
		{
			if	(uiCRC32	==	lpTeam->m_uiMemberHashCode[iMember])
			{
				if	(STRICMP(_lpstrName,lpTeam->m_aMembers[iMember].m_strID)	==	0)
					return	lpTeam;
			}
		}
	}

	return	NULL;
}

bool
cDuelApplicantInfo::isCorrect()
{
	if	(!m_lpActor	||	m_lpActor->m_wSerialInField	==	0xffff	||	m_lpActor->m_dwNameHashCode	!=	m_auiMemberHashCode[0]	||
		m_lpActor->m_wCurrentField	!=	m_wTryField)
		return	false;

	if	(m_wPartySerial	==	0xffff)
		return	true;

	cParty	*lpParty	=	m_lpActor->getParty();

	if	(!lpParty)
		return	false;

	if	(lpParty->getMemberCount() != m_wMemberCount)
		return	false;

	if	(!lpParty->isEnableToTryDuelSatus())
		return	false;

	for (int i=0;i<lpParty->getMemberCount();i++)
	{
		cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

		if	(!lpMember)
			return	false;

		if	(m_auiMemberHashCode[i]	!=	lpMember->m_dwNameHashCode)
			return	false;
	}

	return	true;
}

cDuelManager::cDuelManager()
{
	for (int i=0;i<eDUEL_MATCH_TYPE_COUNT;i++)
		m_aDen[i].init(1000);

	m_uiCurrentUniqueSerial	=	0;
	m_uiBeginUniqueSerial	=	0;
}

cDuelManager::~cDuelManager()
{
	for (int i=0;i<eDUEL_MATCH_TYPE_COUNT;i++)
		m_aDen[i].close();
}

void
cDuelManager::cancel(int _iType,int _iSerial)
{
	if	(_iType	==	0xffff)
		return;

	cDuelApplicantInfo	*lpInfo	=	m_aDen[_iType].get(_iSerial);

	if	(lpInfo)
	{
		if	(lpInfo->m_uiUniqueSerial	==	m_uiBeginUniqueSerial)
			m_uiBeginUniqueSerial++;

		m_aDen[_iType].remove(_iSerial);
	}

	m_bIsChangeApplicantInfo	=	TRUE;
}

void
cDuelManager::cancel(cACTOR *_lpActor)
{
	cancel(_lpActor->m_wDuelType,_lpActor->m_wDuelSerial);

	_lpActor->m_wDuelType	=	0xffff;
}

bool
cDuelManager::tryDuelMatch(cACTOR *_lpActor,int _iDuelMatchType)
{
	if	(_iDuelMatchType	==	eDUEL_TRIAL_GAME)
	{
		if	(!g_duelTrialGameManager.isTrialGameSeason())
		{
			_lpActor->sendDuelMsg(eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_SEASON,eSM_WARNING,NULL);
			return	false;
		}

		if	(!g_duelTrialGameManager.checkEntry(_lpActor))
		{
			_lpActor->sendDuelMsg(eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_ENTRY_PARTY,eSM_WARNING,g_duelTrialGameManager.m_strTrialGameName);
			return	false;
		}
	}
	
	{
		CTimeInfo	curTime;

		curTime.update();

		int	iCurTimeValue	=	curTime.m_wHour*60+curTime.m_wMinute;
		int	iCheckMinValue	=	2*60+50;
		int	iCheckMaxValue	=	3*60+10;

		if	(iCurTimeValue	>=	iCheckMinValue	&&	iCurTimeValue	<=	iCheckMaxValue)
		{
			_lpActor->sendRegistServerMessage(eRSM_DUEL_EXCEPTION_TIME_RULE,eSM_MSG_BOX);

			return	false;
		}
	}

	if	(_lpActor->m_wDuelType	!=	0xffff)
	{
		cancel(_lpActor->m_wDuelType,_lpActor->m_wDuelSerial);

		_lpActor->m_wDuelType	=	0xffff;
	}

	cDuelApplicantInfo	info;

	info.m_wType		=	_iDuelMatchType;
	info.m_iTryTime		=	0;
	info.m_wTryField	=	_lpActor->m_wCurrentField;
	info.m_lpActor		=	_lpActor;
	info.m_uiUniqueSerial=	m_uiCurrentUniqueSerial;

	strcpy(info.m_strLeaderName,_lpActor->m_strId);

	if	(_iDuelMatchType	==	eDUEL_1VS1 || (_iDuelMatchType	==	eDUEL_TRIAL_GAME && g_iTrialGameTeamSize == 1))
	{
		if	(_lpActor->getParty())	// 1vs1 인데 파티야?
		{
			_lpActor->sendRegistServerMessage(eRSM_DUEL_1VS1_RULE,eSM_WARNING);

			return	false;
		}

		info.m_wPartySerial			=	0xffff;
		info.m_auiMemberHashCode[0]	=	GetHashCode(_lpActor->m_strName);
		info.m_iWinPoint			=	_lpActor->m_duelRecord.m_iWeeklyWinPoint;
		info.m_wMemberCount			=	1;
	}
	else
	{
		cParty	*lpParty	=	_lpActor->getParty();

		if	(!lpParty || _lpActor->isPartyLeader()	==	FALSE)
		{
			_lpActor->sendRegistServerMessage(eRSM_DUEL_CAN_NOT_TRY_BY_YOU_ARE_NOT_PARTY_LEADER,eSM_SYSTEM);
			return	false;
		}

		if	(lpParty->isEnableToTryDuelSatus()	==	FALSE)
		{
			_lpActor->sendRegistServerMessage(eRSM_DUEL_CAN_NOT_TRY_BY_BAD_PARTY_STATUS,eSM_SYSTEM);

			return	false;
		}

		if	(lpParty->getMemberCount()	!=	GetMemberCountForDuelMatch(_iDuelMatchType))	//	트리거상에서 체크 해 주기로 했으니 메시지는 불필요!
		{
			_lpActor->sendRegistServerMessage(eRSM_DUEL_CAN_NOT_TRY_BY_MISMATCH_PARTY_MEMBER_COUNT,eSM_SYSTEM);
			return	false;
		}

		for (int i=0;i<lpParty->getMemberCount();i++)
		{
			cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

			info.m_auiMemberHashCode[i]	=	GetHashCode(lpMember->m_strName);
			info.m_iWinPoint			+=	lpMember->m_duelRecord.m_iWeeklyWinPoint;
		}

		info.m_iWinPoint	/=	lpParty->getMemberCount();
		info.m_wPartySerial	=	_lpActor->getPartySerial();
		info.m_wMemberCount	=	lpParty->getMemberCount();

		if	(_iDuelMatchType	==	eDUEL_TRIAL_GAME)
		{
			cAdvDuelTeamInfo	*lpTeam	=	g_duelTrialGameManager.getTeamByMember(_lpActor->m_strId);

			if	(lpTeam)
				info.m_iWinPoint	=	lpTeam->m_record.m_iWinPoint;
		}
	}

	int	iSerial	=	m_aDen[_iDuelMatchType].add(&info);

	if	(iSerial	==	-1)	//	꽉찼다.
	{
		_lpActor->sendRegistServerMessage(eRSM_DUEL_CAN_NOT_TRY_BY_TO_MANY_BATTLER,eSM_SYSTEM);

		return	false;
	}

	m_uiCurrentUniqueSerial++;

	_lpActor->m_wDuelType	=	_iDuelMatchType;
	_lpActor->m_wDuelSerial	=	iSerial	;
	m_bIsChangeApplicantInfo=	TRUE;

	return	true;
}

int
GetCheckWinPointRange(int _iTime)
{
	const	int	c_iTimeGradeContentsCount	=	3;

	int	c_aiCheckWinPointRangeValue[]	=	
	{
		0	,20		,	50,
		21	,40		,	75	,
		41	,60		,	100,
		61	,9999999,	10000000
	};

	for (int i=0;i<4;i++)
	{
		int	iIndex	=	i*c_iTimeGradeContentsCount;

		if	(_iTime	>=	c_aiCheckWinPointRangeValue[iIndex]	&&	_iTime	<=	c_aiCheckWinPointRangeValue[iIndex+1])
			return	c_aiCheckWinPointRangeValue[iIndex+2];
	}

	return	3;
}

cDuelApplicantInfo*
cDuelManager::findMatchPartner(int _iMatchType,int _iApplicant)
{
	{
		CTimeInfo	curTime;
		
		curTime.update();
		
		int	iCurTimeValue	=	curTime.m_wHour*60+curTime.m_wMinute;
		int	iCheckMinValue	=	2*60+50;
		int	iCheckMaxValue	=	3*60+10;
		
		if	(iCurTimeValue	>=	iCheckMinValue	&&	iCurTimeValue	<=	iCheckMaxValue)
			return	NULL;
	}

	cDuelApplicantInfo	*lpLeft	=	m_aDen[_iMatchType].get(_iApplicant);

	int	iLeftWP		=	lpLeft->m_iWinPoint;

	for (int iApplicant=_iApplicant+1;iApplicant<m_aDen[_iMatchType].getMaxCount();iApplicant++)
	{
		cDuelApplicantInfo	*lpRight	=	m_aDen[_iMatchType].get(iApplicant);

		if	(!lpRight)
			continue;

		if	(!lpRight->isCorrect())
		{
			if	(lpRight->m_lpActor)
			{
				lpRight->m_lpActor->sendEtcWork(eEW_CANCEL_TO_TRY_DUEL_BY_CHANGE_PARTY_MEMBER);
				lpRight->m_lpActor->m_wDuelType	=	0xffff;
			}

			cancel(_iMatchType,iApplicant);

			continue;
		}

		int	iGab	=	iLeftWP-lpRight->m_iWinPoint;

		if	(iGab	<	0)
			iGab	=	-iGab;

		int	iLeftValue	=	GetCheckWinPointRange(lpLeft->m_iTryTime);
		int	iRightValue	=	GetCheckWinPointRange(lpRight->m_iTryTime);
		int	iCheckValue	=	min(iLeftValue,iRightValue);

		if	(g_bIsInnerTestServer || lpLeft->m_wType	==	eDUEL_TRIAL_GAME)
			iGab	=	0;

		if	(iGab	<	iCheckValue)
		{
			if	(g_game.findEmptyDuelField(_iMatchType)	==	-1)	//	비어 있는 필드가 없다.
				continue;

			return	lpRight;
		}
	}

	return	NULL;

}

bool
cDuelManager::beginMatch(cDuelApplicantInfo* _lpInfo,cDuelApplicantInfo* _lpPartner)
{
	int	iFieldIndex	=	g_game.findEmptyDuelField(_lpInfo->m_wType);

	if	(iFieldIndex	==	-1)
		return	false;

	cFIELD	*lpField	=	g_game.getField(iFieldIndex);

	lpField->m_wDuelMatchStep	=	eDUEL_MATCH_STEP_ENTRY;

	if	(_lpInfo->m_wType	==	eDUEL_1VS1 || (_lpInfo->m_wType	==	eDUEL_TRIAL_GAME && g_iTrialGameTeamSize == 1))
	{
		_lpInfo->m_lpActor->readyToDuel(lpField->m_iUniqueSerial);
		_lpPartner->m_lpActor->readyToDuel(lpField->m_iUniqueSerial);
	}
	else
	{
		{
			cParty	*lpParty	=	_lpInfo->m_lpActor->getParty();

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);

				lpActor->readyToDuel(lpField->m_iUniqueSerial);
			}
		}

		{
			cParty	*lpParty	=	_lpPartner->m_lpActor->getParty();

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);

				lpActor->readyToDuel(lpField->m_iUniqueSerial);
			}
		}
	}

	if	(_lpInfo->m_wType	==	eDUEL_TRIAL_GAME)
		lpField->setDuelMatchInfo(_lpInfo,_lpPartner,TRUE);
	else
		lpField->setDuelMatchInfo(_lpInfo,_lpPartner);

	cancel(_lpInfo->m_lpActor);
	cancel(_lpPartner->m_lpActor);
	
	return	true;
}

void
cDuelManager::update()
{
	for (int i=0;i<eDUEL_MATCH_TYPE_COUNT;i++)
	{
		for (int iApplicant=0;iApplicant<m_aDen[i].getMaxCount();iApplicant++)
		{
			cDuelApplicantInfo	*lpInfo	=	m_aDen[i].get(iApplicant);

			if	(!lpInfo)
				continue;

			if	(lpInfo->isCorrect()	==	false)
			{
				if	(lpInfo->m_lpActor)
				{
					lpInfo->m_lpActor->sendEtcWork(eEW_CANCEL_TO_TRY_DUEL_BY_CHANGE_PARTY_MEMBER);
					lpInfo->m_lpActor->m_wDuelType	=	0xffff;
				}

				cancel(i,iApplicant);

				continue;
			}

			lpInfo->m_iTryTime++;

			cDuelApplicantInfo*	lpPartner	=	findMatchPartner(i,iApplicant);

			if	(lpPartner	==	NULL)
				continue;

			beginMatch(lpInfo,lpPartner);
		}
	}

	if	(m_bIsChangeApplicantInfo)
	{
		sendCurrentApplicantInfo();
		m_bIsChangeApplicantInfo	=	FALSE;
	}
}

void
cDuelManager::sendCurrentApplicantInfo()
{
	for (int i=0;i<eDUEL_MATCH_TYPE_COUNT;i++)
	{
		for (int iApplicant=0;iApplicant<m_aDen[i].getMaxCount();iApplicant++)
		{
			cDuelApplicantInfo	*lpInfo	=	m_aDen[i].get(iApplicant);

			if	(!lpInfo)
				continue;

			lpInfo->m_lpActor->sendEtcWork(eEW_DUEL_MATCH_APPLICANT_INFO,lpInfo->m_wType,m_aDen[lpInfo->m_wType].getCount());
		}
	}
}

void
cDuelManager::getApplicantCount(int _iType,int *_lpiCount)
{
	*_lpiCount			=	m_aDen[_iType].getCount();
}

int
cDuelRankerManager::getRanking(int _iWhat,char *_lpstrID)
{
	cDuelRankingInfo	*lpRanking	=	m_aRankInfo[_iWhat];

	UINT	uiNameHashCode	=	GetHashCode(_lpstrID);

	for (int i=m_awRankerCount[_iWhat];i;)
	{
		--i;

		if	(m_uiIDHashCode[_iWhat][i]	!=	uiNameHashCode)
			continue;

		if	(stricmp(lpRanking[i].m_strID,_lpstrID)==0)
			return	i;
	}

	return	0xffff;
}

void
cTrialGameManager::setTrialGameSchedule(char *_lpstrName,
										int _iTrialGameBeginYear,int _iTrialGameBeginMonth,int _iTrialGameBeginDay,int _iTrialGameBeginHour,int _iTrialGameBeginMinute,
										int _iTrialGameEndYear,int _iTrialGameEndMonth,int _iTrialGameEndDay,int _iTrialGameEndHour,int _iTrialGameEndMinute)
{
	strcpy(m_strTrialGameName,_lpstrName);
	
	m_timeTrialGameBegin.reset();
	m_timeTrialGameEnd.reset();
	
	m_timeTrialGameBegin.m_wYear	=	_iTrialGameBeginYear;
	m_timeTrialGameBegin.m_wMonth	=	_iTrialGameBeginMonth;
	m_timeTrialGameBegin.m_wDay		=	_iTrialGameBeginDay;
	m_timeTrialGameBegin.m_wHour	=	_iTrialGameBeginHour;
	m_timeTrialGameBegin.m_wMinute	=	_iTrialGameBeginMinute;

	m_timeTrialGameEnd.m_wYear		=	_iTrialGameEndYear;
	m_timeTrialGameEnd.m_wMonth		=	_iTrialGameEndMonth;
	m_timeTrialGameEnd.m_wDay		=	_iTrialGameEndDay;
	m_timeTrialGameEnd.m_wHour		=	_iTrialGameEndHour;
	m_timeTrialGameEnd.m_wMinute	=	_iTrialGameEndMinute;
}

void
cTrialGameManager::setOfflineTournamentSchedule(char *_lpstrComment,int _iBeginYear,int _iBeginMonth,int _iBeginDay,int _iBeginHour,int _iBeginMinute,
									 int _iEndYear,int _iEndMonth,int _iEndDay,int _iEndHour,int _iEndMinute)
{
	strcpy(m_strOfflineTournamentComment,_lpstrComment);
	
	m_timeOfflineTournamentBegin.reset();
	m_timeOfflineTournamentEnd.reset();
	
	m_timeOfflineTournamentBegin.m_wYear	=	_iBeginYear;
	m_timeOfflineTournamentBegin.m_wMonth	=	_iBeginMonth;
	m_timeOfflineTournamentBegin.m_wDay		=	_iBeginDay;
	m_timeOfflineTournamentBegin.m_wHour	=	_iBeginHour;
	m_timeOfflineTournamentBegin.m_wMinute	=	_iBeginMinute;

	m_timeOfflineTournamentEnd.m_wYear		=	_iEndYear;
	m_timeOfflineTournamentEnd.m_wMonth		=	_iEndMonth;
	m_timeOfflineTournamentEnd.m_wDay		=	_iEndDay;
	m_timeOfflineTournamentEnd.m_wHour		=	_iEndHour;
	m_timeOfflineTournamentEnd.m_wMinute	=	_iEndMinute;
}

BOOL
cTrialGameManager::isTrialGameSeason()
{
	CTimeInfo	curTime;

	curTime.update();

	int	iCurDay		=	curTime.m_wYear*12*31+curTime.m_wMonth*31+curTime.m_wDay;
	int	iBeginDay	=	m_timeTrialGameBegin.m_wYear*12*31+m_timeTrialGameBegin.m_wMonth*31+m_timeTrialGameBegin.m_wDay;
	int	iEndDay		=	m_timeTrialGameEnd.m_wYear*12*31+m_timeTrialGameEnd.m_wMonth*31+m_timeTrialGameEnd.m_wDay;

	if	(iCurDay	<	iBeginDay	||	iCurDay	>	iEndDay)
		return	FALSE;

	int	iCurMinute	=	curTime.m_wHour*60+curTime.m_wMinute;
	int	iBeginMinute=	m_timeTrialGameBegin.m_wHour*60+m_timeTrialGameBegin.m_wMinute;
	int	iEndMinute	=	m_timeTrialGameEnd.m_wHour*60+m_timeTrialGameEnd.m_wMinute;

	if	(iCurMinute	>=	iBeginMinute && iCurMinute < iEndMinute)
		return	TRUE;

	return	FALSE;
}

BOOL
cTrialGameManager::isOfflineTournamentSeason()
{
	CTimeInfo	curTime;
	
	curTime.update();
	
	int	iCurValue	=	curTime.getMinuteValue(2000);
	int	iBeginValue	=	m_timeOfflineTournamentBegin.getMinuteValue(2000);
	int	iEndValue	=	m_timeOfflineTournamentEnd.getMinuteValue(2000);
	
	if	(iCurValue	>=	iBeginValue	&&	iCurValue	<=	iEndValue)
		return	TRUE;
	
	return	FALSE;
}

BOOL
cTrialGameManager::checkEntry(cACTOR *_lpActor)
{
	cAdvDuelTeamInfo	*lpTeam	=	g_duelTrialGameManager.getTeamByMember(_lpActor->m_strId);

	if	(!lpTeam)
		return	FALSE;

	cParty	*lpParty	=	_lpActor->getParty();

	if	(lpParty)
	{
		if	(g_iTrialGameTeamSize	<	lpParty->m_wMemberCount)
			return	FALSE;

		for (int i=0;i<lpParty->m_wMemberCount;i++)
		{
			cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);

			if	(!lpActor)
				return	FALSE;

			if	(!lpTeam->isMember(lpActor->m_strId))
				return	FALSE;
		}
	}

	return	TRUE;
}

BOOL
cAdvDuelTeamInfo::isMember(char *_lpstrName)
{
	UINT	uiCRC32	=	GetHashCode(_lpstrName);

	for (int iMember=0;iMember<m_wMemberCount;iMember++)
	{
		if	(uiCRC32	==	m_uiMemberHashCode[iMember])
		{
			if	(STRICMP(_lpstrName,m_aMembers[iMember].m_strID)	==	0)
				return	TRUE;
		}
	}

	return	FALSE;
}
