#include "CGuild.H"
#include "instance_guild_battle_manager.H"
#include "CActor.H"
#include "CGame.H"
#include "CCompress.H"
#include "packetManager.H"

CGuildMarkItemPackInfo	g_guildMarkItemPackInfo;
BOOL					g_bIsCompleteReceiveGuildDataWork	=	FALSE;
int						g_iTransToGVGPeriod					=	30;	//	30일
UINT	
l_auiGuildPetExp[]	=	//	길드펫 누적 경험치
{
	0 ,
	5 ,
	17 ,
	47 ,
	111 ,
	231 ,
	433 ,
	749 ,
	1215 ,
	1873 ,
	2769 ,
	3954 ,
	5485 ,
	7422 ,
	9831 ,
	12783 ,
	16353 ,
	20622 ,
	25674 ,
	31599 ,
	38491 ,
	46449 ,
	55577 ,
	65983 ,
	77780 ,
	91087 ,
	106025 ,
	122722 ,
	141310 ,
	161924 ,
	184706 ,
	209801 ,
	237360 ,
	267537 ,
	300492 ,
	336389 ,
	375396 ,
	417686 ,
	463437 ,
	512832 ,
	566057 ,
	623303 ,
	684766 ,
	750647 ,
	821151 ,
	896487 ,
	976869 ,
	1062516 ,
	1153651 ,
	1250501 ,
	1353298 ,
	1462279 ,
	1577685 ,
	1699761 ,
	1828757 ,
	1964928 ,
	2108533 ,
	2259835 ,
	2419102 ,
	2586606 ,
	2762624 ,
	2947437 ,
	3141332 ,
	3344598 ,
	3557530 ,
	3780426 ,
	4013591 ,
	4257332 ,
	4511962 ,
	4777797 ,
	5055159 ,
	5344373 ,
	5645769 ,
	5959682 ,
	6286450 ,
	6626417 ,
	6979930 ,
	7347342 ,
	7729009 ,
	8125291 ,
	8536555 ,
	8963169 ,
	9405508 ,
	9863950 ,
	10338878 ,
	10830679 ,
	11339745 ,
	11866472 ,
	12411260 ,
	12974514 ,
	13556642 ,
	14158059 ,
	14779181 ,
	15420432 ,
	16082237 ,
	16765028 ,
	17469239 ,
	18195310 ,
	18943685 ,
	19714813 ,
	c_iMaxGuildPetExp ,
};

CGuild::CGuild()
{
	m_wTryAddRelatedPlaceField	=	0xffff;
	m_bf1IsMoveToPointBattleField	=	FALSE;
	reset();
}

int
CGuild::getCheckSum()
{
	int	iCheckSum	=	CGuildAdvanceInfo::getCheckSum(),i,iCount;

	for (i=0,iCount=0;i<dGUILD_MAX_PLAYER && iCount<m_wMemberCount;i++)
	{
		if	(m_aMembers[i].wSerial	==	0xffff)
			continue;

		iCount++;
		iCheckSum	+=	GetHashCode(m_aMembers[i].m_strName);
		iCheckSum	+=	m_aMembers[i].m_wRank;
		iCheckSum	+=	m_aMembers[i].wSerial;
	}

	return	iCheckSum;
}
UINT
CGuild::getGuildPetExpTable(int _iLevel)
{
	_iLevel = max(_iLevel,0);
	_iLevel = min(_iLevel, 100);
	
	return l_auiGuildPetExp[_iLevel];
	
}
int
CGuild::getCheckSumForClient()
{
	int	iCheckSum	=	0,i;

//	iCheckSum	+=	m_wGuildInventorySize;										//	길드 인벤토리 사이즈
	iCheckSum	+=	m_wLevel;								//	길드 레벨
//	iCheckSum	+=	m_dwExp;								//	길드 경험치
	iCheckSum	+=	m_iVictoryPoint;						//	길드 승점
	iCheckSum	+=	m_wVictoryContinue;
	iCheckSum	+=	m_wVictoryCount;
	iCheckSum	+=	m_wDefeatCount;
	iCheckSum	+=	m_wDrawCount;
	iCheckSum	+=	m_wCancelCount;

	iCheckSum	+=	(int)m_llGuildGold;								//	길드 소유 금액
	iCheckSum	+=	m_wHallLevel;							//	길드홀 레벨
//	iCheckSum	+=	m_wMaxMember;
	iCheckSum	+=	m_wMemberCount;			//	최대 길드원 수,현재 길드원 수
//	iCheckSum	+=	m_wMaxSubMaster;
	iCheckSum	+=	m_wSubMasterCount;		//	최대 서브 길마 수,현재 서브 길마 수
//	iCheckSum	+=	m_wMaxCongressMan;
	iCheckSum	+=	m_wCongressManCount;	//	최대 길드 의원 수,현재 서길드 의원 수
	iCheckSum	+=	m_wExpTaxRate;			//	길드 경험치를 걷을것인가? 길드 경험치 세율-_-
	iCheckSum	+=	m_wGuildPoint;
	iCheckSum	+=	m_bf6BonusGuildPoint;
	iCheckSum	+=	m_bf6PeneltyBonusGuildPoint;

	for (i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
		iCheckSum	+=	m_abGuildSkill[i];

	iCheckSum	+=	GetHashCode(m_strName);			//	길드 이름

	return	iCheckSum;
}

void
CGuild::reset()
{
	memset(this,0,sizeof(CGuild));

	for (int i=0;i<dGUILD_MAX_PLAYER;i++)	
		m_aMembers[i].close();

	CGuildAdvanceInfo::reset();
}

BOOL
CGuild::setMember(int _iSlot,CGuildMan *_lpMemberInfo)
{
	BOOL	bResult	=	TRUE;

	if (_iSlot >= dGUILD_MAX_PLAYER || _iSlot < 0)
		return	FALSE;
	if (m_aMembers[_iSlot].wSerial	!=	0xffff	)
		bResult	=	FALSE;

	memcpy(&m_aMembers[_iSlot],_lpMemberInfo,sizeof(CGuildMan));

	return	bResult;
}

BOOL
CGuild::addMember(int _iSlot,CGuildMan *_lpMemberInfo)
{
	if (_iSlot >= dGUILD_MAX_PLAYER || _iSlot < 0)
		return	FALSE;

	memcpy(&m_aMembers[_iSlot],_lpMemberInfo,sizeof(CGuildMan));

	if	(m_aMembers[_iSlot].m_wRank	==	dGUILD_CLASS_SUBMASTER)
		m_wSubMasterCount++;

	cACTOR	*lpActor	=	g_game.getPlayer(m_aMembers[_iSlot].m_strName);

	if	(lpActor)
	{
		lpActor->m_wGuildSerial	=	m_wSerial;
		lpActor->m_wGuildRank	=	m_aMembers[_iSlot].m_wRank;

		SG_JOIN_GUILD	packet;

		packet.base.set(sizeof(packet),dSG_JOIN_GUILD);
		packet.wGuildSerial		=	m_wSerial;
		strcpy(packet.strGuildName,m_strName);

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

		lpActor->m_timeGuildJoinTime.m_dwValue	=	0;
		lpActor->m_timeGuildJoinTime.year		=	g_currentTime.m_wYear-2000;
		lpActor->m_timeGuildJoinTime.month		=	g_currentTime.m_wMonth;
		lpActor->m_timeGuildJoinTime.day		=	g_currentTime.m_wDay;

		g_game.sendBPUpdatePlayerInfo(lpActor);
	}

	updateGuildInfo();

	sendUpdateMemberInfo(_lpMemberInfo);

	return	TRUE;
}

//
//	멤버 제거
BOOL
CGuild::removeMember(char *_lpstrName)
{
	CGuildMan	*lpMember	=	getMember(_lpstrName);

	if	(!lpMember)
		return	FALSE;

	cACTOR		*lpActor	=	g_game.getPlayer(_lpstrName);

	if	(lpActor)
	{
		lpActor->m_wGuildSerial=	0xffff;
		lpActor->m_wGuildRank	=	0;

		if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
			memset(lpActor->m_aWayPoint, 0xffffffff,sizeof(lpActor->m_aWayPoint));

		SG_EXIT_GUILD	packet;

		packet.base.set(sizeof(packet),dSG_EXIT_GUILD);

		packet.wGuildSerial		=	m_wSerial;
		strcpy(packet.strGuildName,m_strName);

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

		g_game.sendBPUpdatePlayerInfo(lpActor);
	}

	lpMember->close();

	updateGuildInfo();
	sendRemoveMember(_lpstrName);

	return	TRUE;
}

void
CGuild::resetAllMembersRank(int _iRank)
{
	SERVERStoWORLD_USER_CHANGERANK		packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_CHANGERANK);

	packet.wGuildSerial	=	m_wSerial;
	packet.wType		=	dGUILD_DISMISSAL;
	packet.wRank		=	_iRank;

	strcpy(packet.strMaster,m_strMasterName);

	for (int i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if	(m_aMembers[i].wSerial	==	0xffff)
			continue;

		if	(m_aMembers[i].m_wRank	==	_iRank)
		{
			strcpy(packet.strTarget,m_aMembers[i].m_strName);

			SEND_WOLRD_PACKET(&packet);
		}
	}
}

//
//	랭크 변경
BOOL
CGuild::changeRank(char *_lpstrName,int _iRank)
{
	CGuildMan	*lpMember	=	getMember(_lpstrName);

	if	(!lpMember)
		return	FALSE;

	if	(_iRank	==	dGUILD_CLASS_MASTER		)
		strcpy(m_strMasterName,_lpstrName);

	lpMember->m_wRank		=	_iRank;

	cACTOR		*lpActor	=	g_game.getPlayer(_lpstrName);

	if (lpActor)
	{
		lpActor->m_wGuildRank	=	_iRank;
		lpActor->sendChangeRankInGuild(_iRank);
	}

	lpActor	=	g_game.getPlayer(m_strMasterName);

	if	(lpActor)
		lpActor->sendGuildWorkMessage(eGWM_CHANGE_RANK_IN_GUILD,_iRank,0,_lpstrName);

	updateGuildInfo();
	
	sendUpdateMemberInfo(lpMember);

	return	TRUE;
}

void
CGuild::updateGuildInfo()
{
	int	i;

	memset(getInfoInGS(),0,sizeof(CGuildInfoInGS));

	m_wSubMasterCount	=	0;
	m_wMemberCount		=	0;
	m_wCongressManCount	=	0;

	operateGuildSkillData();

	for (i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if	(m_aMembers[i].wSerial	==	0xffff)
			continue;

		m_wMemberCount++;

		if	(m_aMembers[i].m_wRank	==	dGUILD_CLASS_SUBMASTER)
			m_wSubMasterCount++;
		if	(m_aMembers[i].m_wRank	==	dGUILD_CLASS_PARLIAMENT)
			m_wCongressManCount++;
	}

	for (int iGuardian=0;iGuardian<dGUILD_PET_TYPE_COUNT;iGuardian++)
		m_awGuardianLevel[iGuardian+1]	=	getGuardianLevel(m_aGuildGuardian[iGuardian].m_dwExperience);
	for (int iPet=0;iPet<dGUILD_GUARDIAN_TYPE_COUNT;iPet++)
		m_awPetLevel[iPet+1]			=	getGuardianLevel(m_aGuildPet[iPet].m_dwExperience);
}

//
//	스킬레벨 업데이트
void
CGuild::updateSkillLevel(int _iSkill,int _iLevel,int _iRemainSkillPoint)
{
	if	(_iSkill	==	0xffff)
		memset(m_abGuildSkill,0,sizeof(m_abGuildSkill));
	else
	if	(_iSkill	==	0xfffe)
	{
		memset(m_abGuildSkill,0,c_iNormalGuildSkillCount);

		for (int i=m_wMemberCount;i;)
		{
			--i;

			if	(m_aMembers[i].m_wRank	==	dGUILD_CLASS_SUBMASTER	||	m_aMembers[i].m_wRank	==	dGUILD_CLASS_PARLIAMENT)
				m_aMembers[i].m_wRank	=	dGUILD_CLASS_MEMBER;
		}
	}
	else
	if	(_iSkill	==	0xfffd)
	{
		memset(m_abGuildSkill,0,c_iNormalGuildSkillCount);
		m_bf6PeneltyBonusGuildPoint	=	m_bf6BonusGuildPoint;

		for (int i=m_wMemberCount;i;)
		{
			--i;

			if	(m_aMembers[i].m_wRank	==	dGUILD_CLASS_SUBMASTER	||	m_aMembers[i].m_wRank	==	dGUILD_CLASS_PARLIAMENT)
				m_aMembers[i].m_wRank	=	dGUILD_CLASS_MEMBER;
		}
	}
	else
		m_abGuildSkill[_iSkill]	=	_iLevel;

	m_wGuildPoint			=	_iRemainSkillPoint;

	operateGuildSkillData();

	if	(IsAvailBCConnect()	==	FALSE)
	{
		SG_UPDATE_GUILD_SKILL_LEVEL	packet;

		packet.base.set(sizeof(packet),dSG_UPDATE_GUILD_SKILL_LEVEL);

		packet.wGuildSerial		=	m_wSerial;
		packet.wSkill			=	_iSkill;
		packet.wLevel			=	_iLevel;
		packet.wRemainSkillPoint=	_iRemainSkillPoint;

		addSendPacketToAllMember(&packet.base);
	}

	for (int i=dGUILD_MAX_PLAYER;i;)
	{
		--i;

		if	(m_aMembers[i].wSerial	==	0xffff)
			continue;

		cACTOR	*lpActor	=	g_game.getPlayer(m_aMembers[i].m_strName);

		if	(lpActor	==	NULL	)
			continue;

		CGuildMan*	lpMember	=	getMember(lpActor->m_strName);

		if	(lpMember)
		{
			if	(lpMember->m_wRank	!=	lpActor->m_wGuildRank)
				lpActor->m_wGuildRank=	lpMember->m_wRank;
		}

		lpActor->buildPower();
	}
}

void
CGuild::updateGuildMemberLevel(char *_lpstrMember,int _iLevel,BOOL _bIsSendToWorld)
{
	CGuildMan	*lpMember	=	getMember(_lpstrMember);

	if	(!lpMember)
		return;

	if	(_bIsSendToWorld)	//	월드로 보낼거다
		g_game.sendWPUpdateGuildMemberLevel(m_wSerial,_lpstrMember,_iLevel);
	else
	{
		lpMember->m_wLevel	=	_iLevel;	//	월드에서 결과를 받은거다.
		sendUpdateMemberInfo(lpMember);
	}
}

void
CGuild::changeExpTaxRate(int _iTaxRate)
{
	m_wExpTaxRate		=	_iTaxRate;
	
	if	(IsAvailBCConnect())
		return;

	SG_CHANGE_GUILD_TAX_RATE	packet;	

	packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_TAX_RATE);
	packet.wTaxRate		=	_iTaxRate;
	packet.wGuildSerial	=	m_wSerial;

	addSendPacketToAllMember((cMSG_BASE_TYPE *)&packet);
}

void
CGuild::changeNotice(char *_lpstrNotice)
{
	strcpy(m_strNotice,_lpstrNotice);

	if	(IsAvailBCConnect())
		return;

	SG_CHANGE_GUILD_NOTICE		packet;	

	packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_NOTICE);
	strcpy(packet.strGuildNotice,_lpstrNotice);
	packet.wGuildSerial	=	m_wSerial;

	addSendPacketToAllMember((cMSG_BASE_TYPE *)&packet);
}

//
//	생성한지 몇일이나 됐냐?
int
CGuild::getOrganizeDayCount(int _iCheckCount)
{
	CTimeInfo	currentTime,organizeDate;
	int			iDayCount	=	0;

	organizeDate.m_wYear	=	m_timeOrganizeDate.year;
	organizeDate.m_wMonth	=	m_timeOrganizeDate.month;
	organizeDate.m_wDay		=	m_timeOrganizeDate.day;
	organizeDate.m_wHour	=	m_timeOrganizeDate.hour;
	organizeDate.m_wMinute	=	m_timeOrganizeDate.minute;
	organizeDate.m_wSecond	=	m_timeOrganizeDate.second;

	currentTime.update();

	if	(organizeDate.m_wYear	>	currentTime.m_wYear)
		return	0; 
	if	(organizeDate.m_wMonth	>	currentTime.m_wMonth)
		return	0; 
	if	(organizeDate.m_wDay	>	currentTime.m_wDay)
		return	0; 

	while(	organizeDate.m_wYear != currentTime.m_wYear	||
			organizeDate.m_wMonth!= currentTime.m_wMonth||
			organizeDate.m_wDay	 != currentTime.m_wDay	)
	{

		organizeDate.m_wDay++;

		if	(organizeDate.m_wDay > GetLastDay(organizeDate.m_wYear,organizeDate.m_wMonth))
		{
			organizeDate.m_wMonth++;
			organizeDate.m_wDay	=	1;

			if	(organizeDate.m_wMonth	>	12)
			{
				organizeDate.m_wMonth	=	1;
				organizeDate.m_wYear	++;
			}
		}

		iDayCount++;

		if	(_iCheckCount	>=	iDayCount)
			return	iDayCount;
	}

	return	iDayCount;
}

BOOL
CGuild::isSellItem(int _iBaseItem)	//	판매 아이템이냐?
{
	for	(int i=0;i<m_wSellEnchantItemCount;i++)
	{
		if	(_iBaseItem	==	m_awSellEnchantItem[i])
			return	TRUE;
	}

	return	FALSE;
}

int
CGuild::getStorageIndexBySlot(int _iInventrySlot)
{
	int	iSlot	=	0;

	for	(int i=0;i<4;i++)
	{
		iSlot	+=	m_abInventorySize[i];

		if	(_iInventrySlot	<	iSlot)
			return	i;
	}

	return	0xffff;
}

//
//	멤버들을 위한 길드전 정보
void
CGuild::getGuildBattleInfoForMember(CGuildBattleInfoForMember *_lpGBI)
{
	_lpGBI->reset();

	int	iTodayTimeOrder,iTodayFieldOrder;

	CGuildBattleInfo	*lpTodayBattleInfo		=	g_gbsToday.getBattleInfoByGuild(m_wSerial,&iTodayTimeOrder,&iTodayFieldOrder);

	//	임시 길드전 정보
	if	(lpTodayBattleInfo)
	{
		CTimeInfo	beginTime;

		g_gbsToday.getBeginTime(iTodayTimeOrder,&beginTime,iTodayFieldOrder);

		_lpGBI->m_bIGB_Hour				=	(BYTE)beginTime.m_wHour;
		_lpGBI->m_bIGB_Minute			=	(BYTE)beginTime.m_wMinute;
		_lpGBI->m_wIGB_FieldSerial		=	lpTodayBattleInfo->m_wFieldSerial;

		for (int i=0;i<2;i++)
		{
			if	(lpTodayBattleInfo->m_awBattleGuild[i] != 0xffff && lpTodayBattleInfo->m_awBattleGuild[i] != m_wSerial)
			{
				CGuild	*lpTargetGuild	=	g_guildManager.getBySerial(lpTodayBattleInfo->m_awBattleGuild[i]);

				if	(!lpTargetGuild)
				{
					g_game.sendWPAskGuildBattleInfo(iTodayTimeOrder,iTodayFieldOrder);
					g_game.sendWPAskGuildInfo(lpTodayBattleInfo->m_awBattleGuild[i]);
				}
				else
					strcpy(_lpGBI->m_strIGB_TargetGuildName,lpTargetGuild->m_strName);
			}
		}
	}

	int	iCurrentDayPoint	=	g_currentTime.m_wYear*1000 +g_currentTime.m_wMonth*100+ g_currentTime.m_wDay;

	if	(g_currentTime.m_wWeekDay	==	c_iSiegeWarfareWeekDay)
	{
		cSiegeWarfareInfoOfGuild	info;

		if	(g_guildManager.getSiegeWarFareInfoInCurrentTime(m_wSerial,&info,TRUE))
		{
			int	iTargetGuild;

			if	(info.m_wInvadeGuild	==	m_wSerial)
			{
				_lpGBI->m_bf1IsDefense	=	FALSE;
				iTargetGuild			=	info.m_wDefenseGuild;
			}
			else
			{
				_lpGBI->m_bf1IsDefense	=	TRUE;
				iTargetGuild			=	info.m_wInvadeGuild;
			}

			CGuild	*lpTargetGuild		=	g_guildManager.getBySerial(iTargetGuild);

			if	(lpTargetGuild)
			{
				_lpGBI->m_bBSW_Hour				=	info.m_bHour;
				_lpGBI->m_bBSW_Minute			=	info.m_bMinute;
				strcpy(_lpGBI->m_strSW_TargetGuildName,lpTargetGuild->m_strName);
			}
		}
	}
}

//
//	최근에 전투를 한 길드다.
BOOL
CGuild::isRecentlyBattleGuild(int _iGuildSerial)
{
	for (int i=0;i<dMAX_FIGHT_ATWEEK;i++)
	{
		if	(m_wFightGuild[i] == _iGuildSerial)
			return	TRUE;
	}

	CGuildBattleInfo	*lpInfo		=	g_gbsToday.getBattleInfoByGuild(m_wSerial);

	if	(lpInfo	&&	lpInfo->isBattleGuild(_iGuildSerial))
		return	TRUE;

	return	FALSE;
}

CGuildMan*
CGuild::getMaster()
{
	return getMember(m_strMasterName);
}

CGuildMan*
CGuild::getMember(char *_lpstrName)
{
	DWORD	dwHashCode	=	GetHashCode(_lpstrName);

	for (int i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if	(m_aMembers[i].wSerial	==	0xffff)
			continue;

		if	(dwHashCode	!=	GetHashCode(m_aMembers[i].m_strName))
			continue;

		if	(STRCMP(m_aMembers[i].m_strName,_lpstrName)==0)	
			return	&m_aMembers[i];
	}

	return	NULL;
}

//
//	길드 뽀개기
void
CGuild::dissolutionGuild()
{
	int		i;

	for (i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if (m_aMembers[i].wSerial	==	0xffff)	continue;

		cACTOR	*lpActor	=	g_game.getPlayer(m_aMembers[i].m_strName);

		if (lpActor	==	NULL	)	continue;

		lpActor->m_wGuildSerial	=	0xffff;

		lpActor->sendDissolutionGuildMessage();
	}
}

//
//	경험치 업데이트
void
CGuild::updateExp(int _iLevel,DWORD _dwExp,int _iGuildPoint,int _iExpTaxRate)
{
	if (m_wLevel	!=	_iLevel)
	{
		for (int i=0;i<dGUILD_MAX_PLAYER;i++)
		{
			if (m_aMembers[i].wSerial	==	0xffff)
				continue;

			cACTOR	*lpActor	=	g_game.getPlayer(m_aMembers[i].m_strName);

			if (lpActor	==	NULL	)
				continue;

			lpActor->sendUpdateGuildLevel(_iLevel,_dwExp,_iGuildPoint,_iExpTaxRate);
		}
	}

	m_wLevel			=	_iLevel;
	m_dwExp				=	_dwExp;
	m_wExpTaxRate		=	_iExpTaxRate;
	m_wGuildPoint		=	_iGuildPoint;

	int	iLimitExpTax	=	getMaximumExpTax();

	if	(m_wExpTaxRate > getMaximumExpTax())
		g_game.sendWPChangeGuildExpTax(this,iLimitExpTax);
}

BOOL
CGuild::setGuildHallLevel(int _iLevel)
{
	m_wHallLevel		=	_iLevel;
	m_wGuildHallShape	=	0;

	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet),dSG_ETC_WORK);

	packet.wWork		=	eEW_CHANGE_GUILD_HALL_LEVEL;
	packet.aiValue[0]	=	_iLevel;

	addSendPacketToAllMember((cMSG_BASE_TYPE *)&packet);

	return	TRUE;
}

//
//	길드 레벨 변경
BOOL
CGuild::makeLevel(int _iLevel)
{
	if (m_wLevel >= _iLevel)
		return FALSE;

	g_game.sendWPMakeGuildLevel(this,_iLevel);

	return	TRUE;
}

//
//	길드 마크 변경
BOOL
CGuild::askSelectGuildMark(CGuildMarkInfo *_lpMarkInfo)
{
	_lpMarkInfo->m_ownedMarkItem.m_dwValue	=	m_guildMarkInfo.m_ownedMarkItem.m_dwValue;

	if	(_lpMarkInfo->m_wIcon != 0xffff)
		if	(g_guildMarkItemPackInfo.isEnableSymbol(_lpMarkInfo->m_wIcon,&_lpMarkInfo->m_ownedMarkItem) == FALSE)
			return	FALSE;

	if	(_lpMarkInfo->m_bBaseShape != 0xff)
		if	(g_guildMarkItemPackInfo.isEnableBase(_lpMarkInfo->m_bBaseShape,&_lpMarkInfo->m_ownedMarkItem) == FALSE)
			return	FALSE;

	if	(g_guildMarkItemPackInfo.isEnableDivision(_lpMarkInfo->m_bBaseColorPartition,&_lpMarkInfo->m_ownedMarkItem) == FALSE)
		return	FALSE;

//	각종 조건 확인할것
	g_game.sendWPSelectGuildMark(m_wSerial,_lpMarkInfo,FALSE);

	return	TRUE;
}

//
//	길드 마크 변경
void
CGuild::selectGuildMark(CGuildMarkInfo *_lpMarkInfo,BOOL _bIsUseGuildMarkPack)
{
	if	(_lpMarkInfo)
		memcpy(&m_guildMarkInfo,_lpMarkInfo,sizeof(CGuildMarkInfo));

	cACTOR	*lpMaster	=	g_game.getPlayer(m_strMasterName);

	if	(lpMaster && _bIsUseGuildMarkPack)
		lpMaster->openGuildMarkComposer();

	if	(IsAvailBCConnect())
		return;

	SG_CHANGE_GUILD_MARK	packet;

	packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_MARK);
	packet.bf1IsUseGuildMarkPack	=	_bIsUseGuildMarkPack;
	packet.bf4Result				=	0;
	packet.wGuildSerial				=	m_wSerial;
	memcpy(&packet.guildMarkInfo,&m_guildMarkInfo,sizeof(CGuildMarkInfo));

	addSendPacketToAllMember((cMSG_BASE_TYPE *)&packet);
}


void
CGuild::operateGuildSkillData()
{
	int	i;

	m_wMaxCongressMan		=	dGUILD_MIN_CONGRESSMAN;
	m_wMaxSubMaster			=	dGUILD_MIN_SUBMASTER;
	m_wMaxMember			=	dGUILD_MIN_PLAYER;

	m_wIncreaseGuardianSight			=	0;
	m_wIncreaseDamageUnderSiegeWarfare	=	0;
	m_wReduceDamageUnderSiegeWarfare	=	0;
	m_wIncreaeGuildPetSight				=	0;
	m_wIncreaseGuildCrestEfficient		=	0;
	m_wIncreaseGuildDoorEfficient		=	0;
	m_wIncreaseGuildPetAttackSpeed		=	0;
	m_wDecreaseGuildShopItemPrice		=	0;
	m_wIncreaseDashBladeDamage			=	0;
	m_wIncreaseGuildFlagEfficient		=	0;
	m_wIncreaseAstroBowDamage			=	0;

	m_wDecreasePeneltyOfFireResistance	=	0;
	m_wDecreasePeneltyOfWaterResistance	=	0;
	m_wDecreasePeneltyOfWindResistance	=	0;
	m_wDecreasePeneltyOfEarthResistance	=	0;
	m_wDecreasePeneltyOfLightResistance	=	0;
	m_wDecreasePeneltyOfDarkResistance	=	0;
	m_wObitianCount						=	0;
	m_wDashBladeCount					=	0;
	m_wAstralBowCount					=	0;
	m_wCristalWaterCount				=	0;
	m_wSellEnchantItemCount				=	0;
	m_wDecreaseStructurePrice			=	0;
	m_wDecreaseDungeonMagicPenelty		=	0;
	m_wDecreaseDeathPenelty				=	0;
	m_wGuildInventorySize				=	0;
	m_wRelatedPlaceCount				=	c_iMinCountForRelatedPlaceOfGuiild;
		
	for (i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
	{
		if	(m_abGuildSkill[i]	==	0)
			continue;
		if	(m_abGuildSkill[i]	==	0xffff)
		{
			m_abGuildSkill[i]	=	0;
			continue;
		}
		if	(m_abGuildSkill[i]	>	20)
		{
			m_abGuildSkill[i]	=	0;
			continue;
		}
	}

	for (i=0;i<c_iNormalGuildSkillCount;i++)
	{
		if	(m_abGuildSkill[i]	==	0)
			continue;

		int	iLevel	=	getApplySkillLevel(i);

		if	(iLevel)
			operateGuildSkillData(i,iLevel);
	}

	for	(int iStatueSkill=0;iStatueSkill<c_iGuildStatueCount;iStatueSkill++)
	{
		int	iSkillIndex	=	c_iGuildStatueSkillBeginIndex+iStatueSkill*c_iGuildStatuePartsCount;
		int	iPartsCount	=	0;

		for (i=iSkillIndex;i<iSkillIndex+c_iGuildStatuePartsCount;i++)
		{
			if	(m_abGuildSkill[i])
				iPartsCount++;
		}

		int	iEffectLimitCount	=	dMAX_GUILD_SKILL_EFFECT_COUNT;

		if	(iPartsCount	<	c_iGuildStatueSetEffectRequireCount)
			iEffectLimitCount	=	1;

		for (i=iSkillIndex;i<iSkillIndex+c_iGuildStatuePartsCount;i++)
		{
			if	(m_abGuildSkill[i]	==	0)
				continue;

			int	iLevel	=	getApplySkillLevel(i);

			if	(iLevel)
				operateGuildSkillData(i,iLevel,iEffectLimitCount);
		}
	}

	if	(m_wGuildInventorySize	<=	0)
		memset(m_abInventorySize,0,sizeof(m_abInventorySize));

	{
		int	iSize	=	0;

		for	(int i=0;i<4;i++)
			iSize	+=	m_abInventorySize[i];

		if	(iSize	<	m_wGuildInventorySize)
		{
			int	iLackSize	=	m_wGuildInventorySize-iSize;
			int	iShareSize	=	iLackSize/4;
			int	iFirstSize	=	iLackSize%4;

			for	(int i=0;i<4;i++)
				m_abInventorySize[i]	+=	iShareSize;

			m_abInventorySize[0]		+=	iFirstSize;
		}
	}

	buildReservePetInfo();
}

//
//	길드 스킬 데이터 처리
void
CGuild::operateGuildSkillData(int _iSkill,int _iLevel,int _iEffectLimitCount)
{
	int			i;
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);

	if	(lpSkill	==	NULL	||	_iLevel	>	lpSkill->m_wTopLevel)
		return;

	for (i=0;i<_iEffectLimitCount;i++)
	{
		CGuildSkillEffectInfo	*lpEffect	=	&lpSkill->m_aEffect[i];

		if	(lpEffect->m_wEffect	==	0xffff)
			continue;

		switch(lpEffect->m_wEffect)
		{
			case	eGSE_INCREASE_GUILD_VILLAGE				:
				operateGSIncreaseGuildVillage(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_MAX_GUILD_MEMBER_COUNT	:	//	최대길드원증가
				operateGSIncreaseMaxGuildMemberCount(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_MAX_GUILD_SUB_MASTER_COUNT:	//최대서브마스터수증가
				operateGSIncreaseMaxSubGuildMasterCount(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_MAX_GUILD_CONGRESS_COUNT	:	//	최대길드의원인원수증가
				operateGSIncreaseMaxGuildCongressCount(_iSkill,_iLevel);
				break;
			case	eGSE_INCREASE_GUILD_INVENTORY_SIZE		:	//	길드인벤토리사이즈증가
				operateGSIncreaseGuildInventorySize(_iSkill,_iLevel);
				break;

			case	eGSE_INCREASE_GUARDIAN_COUNT_PER_LEVEL	:	//{	"가디언수증가per레벨"					:	
				operateGSIncreaseGuardianCount(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUARDIAN_SIGHT_PER_LEVEL	:	//{	"가디언시야증가"						:	
				operateGSIncreaseGuardianSight(_iSkill,_iLevel,lpEffect);
				break;
				
//	추가된거
			case	eGSE_INCREASE_DAMAGE_UNDER_SIEGE_WARFARE	:	//{	"공성전시대미지감소
				operateGSIncreaseDamageUnderSiegeWarfare(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_REDUCE_DAMAGE_UNDER_SIEGE_WARFARE	:	//{	"공성전시대미지감소
				operateGSReduceDamageUnderSiegeWarfare(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUILD_PET_SIGHT			:
				operateGSIncreaseGuildPetSight(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUILD_CREST_EFFICIENT		:	//	"길드문장효율증가",
				operateGSIncreaseGuildCrestEfficient(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUILD_FLAG_EFFICIENT		:	//	"길드깃발효율증가",
				operateGSIncreaseGuildFlagEfficient(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_ASTRO_BOW_DAMAGE			:	//	"아스트로보우대미지증가",
				operateGSIncreaseAstroBowDamage(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUILD_DOOR_EFFICIENT		:	//	"길드문효율증가",
				operateGSIncreaseGuildDoorEfficient(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUILD_PET_ATTACK_SPEED	:	//	"길드펫액션속도증가",
				operateGSIncreaseGuildPetAttackSpeed(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_DECREASE_GUILD_SHOP_ITEM_PRICE		:	//	"길드상점아이템가격감소",
				operateGSDecreaseGuildShopItemPrice(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_DASH_BLADE_DAMAGE			:	//	"대쉬블레이드대미지증가",
				operateGSIncreaseDashBladeDamage(_iSkill,_iLevel,lpEffect);
				break;
//	추가된거


			case	eGSE_DECREASE_FIELD_FIRE_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드불저항페널티감소per레벨"		:	
			case	eGSE_DECREASE_FIELD_WATER_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드물저항페널티감소per레벨"		:	
			case	eGSE_DECREASE_FIELD_WIND_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드바람저항페널티감소per레벨"		:	
			case	eGSE_DECREASE_FIELD_EARTH_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드대지저항페널티감소per레벨"		:	
			case	eGSE_DECREASE_FIELD_LIGHT_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드빛저항페널티감소per레벨"		:	
			case	eGSE_DECREASE_FIELD_DARK_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드어둠저항페널티감소per레벨"		:	
				operateGSDecreaseMagicResistanceFieldPenelty(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_OBITIAN_COUNT_PER_LEVEL		:	//{	"오비티안개수증가per레벨"				:	
			case	eGSE_INCREASE_CRISTAL_WATER_COUNT_PER_LEVEL	:	//{	"크리스탈워터개수증가per레벨"			:	
			case	eGSE_INCREASE_ASTRAL_BOW_COUNT_PER_LEVEL	:	//{	"아스트로보우개수증가per레벨"			:	
			case	eGSE_INCREASE_DASH_BLADE_COUNT_PER_LEVEL	:	//{	"대쉬블래이드개수증가per레벨"			:	
				operateGSIncreasePetCount(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_SELL_MAGIC_ENCHANT_ITEM						:	//{	"마법인챈트용품판매"				:	
				operateGSAddSellMagicEnchantItem(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_DECREASE_DUNGEON_MAGIC_ATTR_PENELTY_PER_LEVEL	:	//{	"던젼속성페널티저하per레벨"						:	
				operateGSDecreaseDungeonMagicPeneltyPerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_DECREASE_DEATH_PENELTY_PER_LEVEL				:	//{	"사망페널티감소per레벨"					:	
				operateGSDecreaseDeathPeneltyPerLevel(_iSkill,_iLevel,lpEffect);
				break;

		}
	}
}

void
CGuild::operateGSIncreaseGuildVillage(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill=	g_gsManager.get(_iSkill);

	int	iIncreaseCount	=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][0]/100;

	m_wRelatedPlaceCount=	min(m_wRelatedPlaceCount+iIncreaseCount,dMAX_GUILD_VILLAGE_COUNT);
}

//
//	최대길드원증가
void
CGuild::operateGSIncreaseMaxGuildMemberCount(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill=	g_gsManager.get(_iSkill);

	int	iIncreaseCount	=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][0]/100;

	m_wMaxMember		=	min(m_wMaxMember+iIncreaseCount,c_iMaxGuildMemberCount);
}

//
//	최대서브마스터수증가
void
CGuild::operateGSIncreaseMaxSubGuildMasterCount(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill=	g_gsManager.get(_iSkill);

	int	iIncreaseCount	=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][0]/100;

	m_wMaxSubMaster		=	min(m_wMaxSubMaster+iIncreaseCount,c_iMaxSubGuildMasterCount);
}

//
//	최대길드의원인원수증가
void
CGuild::operateGSIncreaseMaxGuildCongressCount(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill=	g_gsManager.get(_iSkill);

	int	iIncreaseCount	=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][0]/100;

	m_wMaxCongressMan	=	min(m_wMaxCongressMan+iIncreaseCount,c_iMaxSubGuildCongressCount);
}

//
//	길드인벤토리사이즈증가
void
CGuild::operateGSIncreaseGuildInventorySize(int _iSkill,int _iLevel)
{
	CGuildSkill	*lpSkill=	g_gsManager.get(_iSkill);

	int	iIncreaseCount	=	_iLevel*lpSkill->m_aEffect[0].m_aiValue[0][0]/100;

	m_wGuildInventorySize=	min(m_wGuildInventorySize+iIncreaseCount,dGUILD_MAX_INVEN);
}

void
CGuild::operateGSIncreaseGuardianCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wMaxGuardianCount		+=	iValue;
}

void
CGuild::operateGSIncreaseGuardianSight(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wIncreaseGuardianSight+=	iValue;
}

void
CGuild::operateGSIncreaseDamageUnderSiegeWarfare(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wIncreaseDamageUnderSiegeWarfare	+=	iValue;
}

void
CGuild::operateGSReduceDamageUnderSiegeWarfare(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wReduceDamageUnderSiegeWarfare	+=	iValue;
}

void
CGuild::operateGSIncreaseGuildPetSight(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wIncreaeGuildPetSight	+=	iValue;
}

void		
CGuild::operateGSIncreaseGuildCrestEfficient(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)	//	길드문장효율증가",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wIncreaseGuildCrestEfficient	+=	iValue;
}

void		
CGuild::operateGSIncreaseGuildFlagEfficient(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)	//	"길드깃발효율증가",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wIncreaseGuildFlagEfficient	+=	iValue;
}

void		
CGuild::operateGSIncreaseAstroBowDamage(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)	//	"아스트로보우대미지증가",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wIncreaseAstroBowDamage	+=	iValue;
}

void		
CGuild::operateGSIncreaseGuildDoorEfficient(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)	//	"길드문효율증가",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wIncreaseGuildDoorEfficient	+=	iValue;
}

void		
CGuild::operateGSIncreaseGuildPetAttackSpeed(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)	//	"길드펫액션속도증가",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wIncreaseGuildPetAttackSpeed	+=	iValue;
}

void		
CGuild::operateGSDecreaseGuildShopItemPrice(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)	//	"길드상점아이템가격감소",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wDecreaseGuildShopItemPrice	+=	iValue;
}

void		
CGuild::operateGSIncreaseDashBladeDamage(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)	//	"대쉬블레이드대미지증가",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wIncreaseDashBladeDamage	+=	iValue;
}

void
CGuild::operateGSDecreaseMagicResistanceFieldPenelty(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	switch(_lpEffect->m_wEffect)
	{
		case	eGSE_DECREASE_FIELD_FIRE_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드불저항페널티감소per레벨"		:	
			m_wDecreasePeneltyOfFireResistance	+=	iValue;
			break;
		case	eGSE_DECREASE_FIELD_WATER_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드물저항페널티감소per레벨"		:	
			m_wDecreasePeneltyOfWaterResistance	+=	iValue;
			break;
		case	eGSE_DECREASE_FIELD_WIND_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드바람저항페널티감소per레벨"		:	
			m_wDecreasePeneltyOfWindResistance	+=	iValue;
			break;
		case	eGSE_DECREASE_FIELD_EARTH_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드대지저항페널티감소per레벨"		:	
			m_wDecreasePeneltyOfEarthResistance	+=	iValue;
			break;
		case	eGSE_DECREASE_FIELD_LIGHT_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드빛저항페널티감소per레벨"		:	
			m_wDecreasePeneltyOfLightResistance	+=	iValue;
			break;
		case	eGSE_DECREASE_FIELD_DARK_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드어둠저항페널티감소per레벨"		:	
			m_wDecreasePeneltyOfDarkResistance	+=	iValue;
			break;
	}
}

void
CGuild::operateGSIncreasePetCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	switch(_lpEffect->m_wEffect)
	{
		case	eGSE_INCREASE_OBITIAN_COUNT_PER_LEVEL		:	//{	"오비티안개수증가per레벨"				:	
			m_wObitianCount			+=	iValue;
			break;
		case	eGSE_INCREASE_CRISTAL_WATER_COUNT_PER_LEVEL	:	//{	"크리스탈워터개수증가per레벨"			:	
			m_wCristalWaterCount	+=	iValue;
			break;
		case	eGSE_INCREASE_ASTRAL_BOW_COUNT_PER_LEVEL	:	//{	"아스트로보우개수증가per레벨"			:	
			m_wAstralBowCount		=	iValue;
			break;
		case	eGSE_INCREASE_DASH_BLADE_COUNT_PER_LEVEL	:	//{	"대쉬블래이드개수증가per레벨"			:	
			m_wDashBladeCount		+=	iValue;
			break;
	}
}

void
CGuild::operateGSAddSellMagicEnchantItem(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)
{
	for	(int i=0;i<dGUILD_SKILL_EFFECT_VARIATION_COUNT;i++)
	{
		if	(m_wSellEnchantItemCount	>=	c_iGuildHallSellEnchantItemCount)
			break;

		if	(_lpEffect->m_aiValue[0][i])
			m_awSellEnchantItem[m_wSellEnchantItemCount++]	=	_lpEffect->m_aiValue[0][i];
	}
}

void
CGuild::operateGSDecreaseStructurePricePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wDecreaseStructurePrice+=	iValue;
}

void
CGuild::operateGSDecreaseDungeonMagicPeneltyPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wDecreaseDungeonMagicPenelty	+=	iValue;
}

void
CGuild::operateGSDecreaseDeathPeneltyPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	m_wDecreaseDeathPenelty	+=	iValue;

}

//
//	최고레벨(이 이상 못 올라간다.)
int
CGuild::getSkillTopLevel(int _iSkill)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);

	if	(lpSkill	==	NULL)
		return	0;

	return	lpSkill->m_wTopLevel;
}


//
//	길마가 필요한 스킬들 체크
void
CGuild::checkRequireMasterGuildSkill(cACTOR *_lpMaster)
{
	for (int i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
	{
		if (m_abGuildSkill[i]	==	0)
			continue;

		if (m_abGuildSkill[i]	>=	0x8000)	//	불가 상태에서
		{
			if	(checkRequireTitle(i,_lpMaster))	//	사용가능해지면...
				g_game.sendWPUpdateGuildSkillStatus(m_wSerial,i,FALSE);

			continue;
		}

		//	사용가능 상태에서
		if	(checkRequireTitle(i,_lpMaster) == FALSE)	//	불가가되면.
			g_game.sendWPUpdateGuildSkillStatus(m_wSerial,i,TRUE);
	}
}

//
//	이런저런 상황을 고려해 계산한 한계 레벨
int
CGuild::getSkillLimitLevel(int _iSkill)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);

	if (lpSkill	==	NULL)
		return	0;

	int	iLevel	=	lpSkill->m_wTopLevel;

	if (lpSkill->m_sMaxLevelByGuildHallLevel	!=	0)
		iLevel	=	min(iLevel,m_wHallLevel-lpSkill->m_sMaxLevelByGuildHallLevel);

	switch (lpSkill->m_wTopLevelLimitExpression)
	{
		case	1	:	//	(10+[스킬0] Lv*10-[스킬1]Lv*5-1)까지만 레벨 업 가능
		{
			int	iValue1,iValue2,iLimitLevel;

			iValue1		=	lpSkill->m_aTopLevelLimitExpressionValue[0];
			iValue1		=	m_abGuildSkill[iValue1];
			iValue2		=	lpSkill->m_aTopLevelLimitExpressionValue[1];
			iValue2		=	m_abGuildSkill[iValue2];
			iLimitLevel	=	10+iValue1*10-iValue2*5-1;
			iLevel		=	min(iLevel,iLimitLevel);
			break;
		}
		
		case	2	:	//	(10+[스킬0]Lv*10-[스킬1]Lv-1)/5까지만 레벨 업 가능
		{
			int	iValue1,iValue2,iLimitLevel;

			iValue1		=	lpSkill->m_aTopLevelLimitExpressionValue[0];
			iValue1		=	m_abGuildSkill[iValue1];
			iValue2		=	lpSkill->m_aTopLevelLimitExpressionValue[1];
			iValue2		=	m_abGuildSkill[iValue2];
			iLimitLevel	=	(10+iValue1*10-iValue2-1)/5;
			iLevel		=	min(iLevel,iLimitLevel);
			break;
		}
	}

	return	iLevel;
}

//
//	적용 할때 레벨
int
CGuild::getApplySkillLevel(int _iSkill)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);

	if (lpSkill	==	NULL)
		return	0;

	int	iLevel	=	m_abGuildSkill[_iSkill];

	if (iLevel	>=	0x8000)
		return	0;

	if (iLevel	==	0)
		return	0;

	int	iLimitLevel	=	getSkillLimitLevel(_iSkill);

	iLevel		=	min(iLevel,iLimitLevel);
	//iLevel		=	min(iLevel, m_wHallLevel);

	return	iLevel;
}

//
//	업그레이드 가능한 스킬인가?
BOOL
CGuild::isUpgradeAbleSkill(int _iSkill,cACTOR *_lpMaster)
{
	CGuildMan	*lpMember	=	getMaster();

	if	(!lpMember)
		return	FALSE;

	CGuildSkill	*lpSkill=	g_gsManager.get(_iSkill);
	int			i;

	if	(!lpSkill)		//	스킬 없수?
		return	FALSE;

	if	(lpSkill->m_bf1IsStatueSkill	==	FALSE	&&	m_wGuildPoint	==	0)
		return	FALSE;

	int	iCurrentLevel	=	m_abGuildSkill[_iSkill];

	if	(iCurrentLevel	>=	0x8000)	
		return	FALSE;

	if	(iCurrentLevel	>=	lpSkill->m_wTopLevel)	//	한계다.
		return	FALSE;

	if	(iCurrentLevel	>=	getSkillLimitLevel(_iSkill))	//	레벨 한계다.
		return	FALSE;

	if	(lpSkill->m_bf1IsRequireSkillMasteryTitle)
		if	(!_lpMaster || _lpMaster->getTitleByTitleEffect(eTE_SKILL_MASTER)	==	NULL)
			return	FALSE;

	if	(lpSkill->m_bf3RequireGuildHallLevel	>	m_wHallLevel)
		return	FALSE;

	if	(lpSkill->m_iRequireGold)
		if	(_lpMaster	==	NULL	||	_lpMaster->m_iGold	<	lpSkill->m_iRequireGold)
			return	FALSE;

	for (i=0;i<dGUILD_SKILL_REQUIRE_SKILL_COUNT;i++)
	{
		if	(lpSkill->m_awRequireSkill[i]==0xffff)
			break;

		if	(m_abGuildSkill[lpSkill->m_awRequireSkill[i]] < lpSkill->m_awRequireSkillLevel[i])
			return	FALSE;
	}

	return	TRUE;
}

//
//	길드 마스터의 타이틀 체크
BOOL
CGuild::checkRequireTitle(int _iSkill,cACTOR *_lpMaster)
{
	CGuildSkill	*lpSkill=	g_gsManager.get(_iSkill);

	if	(!lpSkill)
		return	NULL;

	if	(lpSkill->m_bf1IsRequireSkillMasteryTitle)
		if	(_lpMaster->getTitleByTitleEffect(eTE_SKILL_MASTER)	==	NULL)
			return	FALSE;

	return	TRUE;
}

int
CGuild::getGuardianLevel(UINT _uiExp)
{
	for(int iLevel=0;iLevel<100;iLevel++)
	{
		if	(_uiExp	>=	l_auiGuildPetExp[iLevel]	&&	_uiExp	<	l_auiGuildPetExp[iLevel+1])
			return	iLevel+1;
	}

	return	100;
}

int
CGuild::getGuardianExpTableValue(int _iLevel)
{
	return	l_auiGuildPetExp[_iLevel];
}

int	
CGuild::getRentedPetCount(int _iPetType)
{
	int	iPetCount=	0;

	for (int i=0,iCount=0;i<dGUILD_MAX_PLAYER && iCount<m_wMemberCount;i++)
	{
		if	(m_aMembers[i].wSerial	==	0xffff)
			continue;

		if	(m_aMembers[i].m_bf3ReservePet	==	_iPetType)
			iPetCount++;
	}

	return	iPetCount;
}

void
CGuild::buildReservePetInfo()
{
	int	iPetCount=	0,i,iCount;
	int	aiPetCount[5];

	memset(aiPetCount,0,sizeof(aiPetCount));

	for (i=0,iCount=0;i<dGUILD_MAX_PLAYER && iCount<m_wMemberCount;i++)
	{
		if	(m_aMembers[i].wSerial	==	0xffff)
			continue;

		if	(m_aMembers[i].m_bf3ReservePet	>	eGUILD_PET_CRISTAL_WATER)
			m_aMembers[i].m_bf3ReservePet	=	0;

		aiPetCount[m_aMembers[i].m_bf3ReservePet]++;
	}

	if	(aiPetCount[eGUILD_PET_OBITIAN]		>	m_wObitianCount	||
		aiPetCount[eGUILD_PET_DASH_BLADE]	>	m_wDashBladeCount	||
		aiPetCount[eGUILD_PET_ASTRO_BOW]	>	m_wAstralBowCount	||
		aiPetCount[eGUILD_PET_CRISTAL_WATER]>	m_wCristalWaterCount)
		for (i=0,iCount=0;i<dGUILD_MAX_PLAYER && iCount<m_wMemberCount;i++)
		{
			if	(m_aMembers[i].wSerial	==	0xffff)
				continue;

			m_aMembers[i].m_bf3ReservePet	=	0;
		}
}

BOOL
CGuild::checkLevelUpAbleGuildStatue(int _iLimitLevel)
{
	for (int i=c_iGuildStatueSkillBeginIndex;i<dMAX_GUILD_SKILL_COUNT;i++)
	{
		CGuildSkill*	lpSkill	=	g_gsManager.get(i);

		if	(!lpSkill)
			continue;

		if	(m_abGuildSkill[i]	>=	_iLimitLevel)
			continue;

		if	(isUpgradeAbleSkill(i,NULL))
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CGuild::increaseRandomGuildStatueLevel(int _iLimitLevel)
{
	int	iLevelUpSkill	=	-1;

	for (int i=0;i<1000;i++)
	{
		int	iSkill	=	random(30)+c_iGuildStatueSkillBeginIndex;

		if	(m_abGuildSkill[iSkill]	>=	_iLimitLevel)
			continue;

		if	(isUpgradeAbleSkill(iSkill,NULL)	==	FALSE)
			continue;

		iLevelUpSkill	=	iSkill;

		break;
	}

	if	(iLevelUpSkill	==	-1)
	{
		for (int i=c_iGuildStatueSkillBeginIndex;i<dMAX_GUILD_SKILL_COUNT;i++)
		{
			CGuildSkill*	lpSkill	=	g_gsManager.get(i);

			if	(!lpSkill)
				continue;

			if	(m_abGuildSkill[i]	>=	_iLimitLevel)
				continue;

			if	(isUpgradeAbleSkill(i,NULL))
			{
				iLevelUpSkill	=	i;
				break;
			}
		}
	}

	if	(iLevelUpSkill	==	-1)
		return	FALSE;

	g_game.sendWPIncreaseGuildSkillLevel(m_wSerial,iLevelUpSkill);

	return	TRUE;
}
