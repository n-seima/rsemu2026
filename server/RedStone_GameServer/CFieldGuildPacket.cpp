#include "cFIELD.H"
#include "cGAME.H"
#include "debugCode.H"
#include "CGuild.H"
#include "instance_guild_battle_manager.H"
#include "packetManager.H"

//
//	길드 생성
void
cFIELD::receiveCreateGuild(CG_CREATE_GUILD *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(!lpActor->m_wIsReadyToMakeGuild)
		return;

	_lpPacket->strGuildName[dGUILD_NAME_LENGTH-1]	=	NULL;
	
	if	(!CheckName(_lpPacket->strGuildName))
	{
		lpActor->sendGuildWorkMessage(eGWM_INCORRECT_GUILD_NAME);

		return;
	}

	g_game.sendWPCreateGuild(lpActor,_lpPacket->strGuildName,_lpPacket->strSubGuildMasterName);
}

//
//	길드 정보 요청
void
cFIELD::receiveAskGuildInfo(CG_ASK_GUILD_INFO *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->sendGuildInfo(_lpPacket->wIsIgnoreSkillData);
}

void
cFIELD::receiveAskDissoulutionGuild(int _iSerial)
{
	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
		return;
	
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	if (!lpActor->isGuildMaster())
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	CGuild	*lpGuild	=	lpActor->getGuild();

	if (!lpGuild)
		return;


	if (lpGuild->m_wMemberCount	>	1)
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	g_game.sendWPAskDeleteGuild(lpActor,lpGuild->m_strName,lpGuild->m_wSerial);
}



//
//로그인 할때 return FALSE 대신 이걸 쏴준다 -_-;
BOOL
cFIELD::sendGuildMemberNullList(cACTOR *_lpActor)
{
	SG_GUILD_MEMBER_LIST	packet;
	
	packet.base.set(sizeof(packet),dSG_GUILD_MEMBER_LIST);
	packet.bf15Count	=	0;
	packet.bf1IsAppend	=	FALSE;
	
	g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
	
	return	TRUE;
}

void
cFIELD::receiveAskGuildMemberList(CG_ASK_GUILD_MEMBER_LIST *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);
	BOOL	bIsInit		=	_lpPacket->bIsinit;

	if (!lpActor)
	{
		if(bIsInit)
		{
			sendGuildMemberNullList(lpActor);
			return; 
		}
		else
			return;
	}

	CGuild	*lpGuild	=	lpActor->getGuild();

	if (!lpGuild)
	{
		if(bIsInit)
		{
			sendGuildMemberNullList(lpActor);
			return;
		}
		else
			return;
	}

	lpGuild->sendGuildMemberList(lpActor);
}

//
//	길드 참가 요청 메시지 받음
void
cFIELD::receiveAskJoinGuild(CG_ASK_JOIN_GUILD *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
	{
		char	strGuildName[dGUILD_NAME_LENGTH];
		
		memcpy(strGuildName,lpActor->m_aWayPoint,sizeof(strGuildName));

		if	(strGuildName[0]	==	0xff	&&	strGuildName[0]	==	0xff)
			return;

		CGuild	*lpGuild		=	g_guildManager.getByName(strGuildName);

		if (!lpGuild)
		{
			lpActor->sendJoinGuildResult(eAJGR_GUILD_NOT_EXIST);
			return;
		}

		if	(lpGuild->getMember(lpActor->m_strName))
		{
			lpActor->sendJoinGuildResult(eAJGR_ALREADY_GUILD_MEMBER);
			return;
		}

		lpGuild->updateGuildInfo();

		if	(lpGuild->m_wMemberCount	>=	lpGuild->m_wMaxMember)
		{
			lpActor->sendJoinGuildResult(eAJGR_GUILD_MEMBER_FULL);
			return;
		}

		if	(lpGuild->m_wIsWaitGuildWorkResult)
			return;

		CGuildMan*	lpMaster	=	lpGuild->getMaster();

		if	(!lpMaster)
			return;

		lpGuild->m_wIsWaitGuildWorkResult	=	TRUE;

		g_game.sendWPJoinGuild(lpGuild->m_strName,lpMaster->m_strName,lpActor,dGUILD_CLASS_ROOKIE);

		return;
	}
	
	if (g_bIsTestServer	==	FALSE)
		if (!lpActor->isOwnedTitle(eTN_CITIZEN))
		{
			lpActor->sendJoinGuildResult(eAJGR_REQUIRE_CITIZEN_TITLE);
			return;
		}

	cACTOR	*lpTarget	=	getTestedActor(_lpPacket->wTargetSerial);

	if (lpActor->m_wGuildSerial	!=	0xffff || lpActor->m_bf1IsReadyToMakeGuild)
	{
		lpActor->sendJoinGuildResult(eAJGR_ALREADY_GUILD_MEMBER);
		return;
	}

	if	(lpActor->m_timeGuildJoinTime.m_dwValue)
	{
		CTimeInfo	guildJoinTime;

		guildJoinTime.m_wYear	=	(WORD)(lpActor->m_timeGuildJoinTime.year+2000);
		guildJoinTime.m_wMonth	=	lpActor->m_timeGuildJoinTime.month;
		guildJoinTime.m_wDay	=	lpActor->m_timeGuildJoinTime.day;

		DWORD	dwJoinValue		=	guildJoinTime.m_wYear*12*31+guildJoinTime.m_wMonth*31+guildJoinTime.m_wDay;
		DWORD	dwCurrentValue	=	g_currentTime.m_wYear*12*31+g_currentTime.m_wMonth*31+g_currentTime.m_wDay;
	}


	if (!lpTarget)
	{
		lpActor->sendJoinGuildResult(eAJGR_TARGET_NOT_EXIST);
		return;
	}

	if (lpTarget->m_wPitchmanShopSerial	!=	0xffff)
	{
		lpActor->sendJoinGuildResult(eAJGR_TARGET_BUSY);
		return;
	}
	
	if (lpTarget->m_wGuildSerial	==	0xffff)
	{
		lpActor->sendJoinGuildResult(eAJGR_TARGET_NOT_GUILD_MASTER);
		return;
	}

	if (!lpTarget->isGuildLeader())
	{
		lpActor->sendJoinGuildResult(eAJGR_TARGET_NOT_GUILD_LEADER);
		return;
	}

	CGuild	*lpGuild		=	g_guildManager.getBySerial(lpTarget->m_wGuildSerial);

	if (!lpGuild)
	{
		lpActor->sendJoinGuildResult(eAJGR_TARGET_NOT_GUILD_MASTER);
		return;
	}

	if (lpTarget->m_bf1IsDenyCommunity	)
	{
		lpActor->sendJoinGuildResult(eAJGR_TARGET_DENY_COMMUNITY);
		return;
	}

	lpActor->m_wRequestGuild=	lpTarget->m_wGuildSerial;

	SG_ASK_JOIN_GUILD	askPacket;

	askPacket.base.set(sizeof(SG_ASK_JOIN_GUILD),dSG_ASK_JOIN_GUILD);

	askPacket.wAskerSerial	=	lpActor->m_wSerialInField;
	askPacket.wLevel		=	lpActor->m_iLevel;
	askPacket.wJob			=	lpActor->m_wJob;

	strcpy(askPacket.strAskerName,lpActor->m_strName);

	g_userPM.add(lpTarget->m_iClientSerial,&askPacket,askPacket.base.wSize);
}

//
//	길드 참가 요청에 대한 대답을 받았다
void
cFIELD::receiveReplyAskJoinGuild(CG_REPLY_ASK_JOIN_GUILD *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	cACTOR	*lpTarget	=	getTestedActor(_lpPacket->wAskerSerial);

	if (!lpTarget)	return;

	if (STRCMP(lpTarget->m_strName,_lpPacket->strAskerName) != 0)
		return;
	if (lpTarget->m_wGuildSerial	!=	0xffff					)
		return;
	if (lpActor->m_wGuildSerial		==	0xffff					)
		return;
	if (!lpActor->isGuildLeader()								)
		return;

	if (lpTarget->m_wRequestGuild	!=	lpActor->m_wGuildSerial)
	{
		lpTarget->m_wRequestGuild	=	0xffff;
		return;
	}

	lpTarget->m_wRequestGuild	=	0xffff;

	CGuild	*lpGuild		=	g_guildManager.getBySerial(lpActor->m_wGuildSerial);

	if (!lpGuild)
		return;

	switch(_lpPacket->wResult)
	{
		case	eAJGR_DENY			:
		{
			lpActor->sendJoinGuildResult(eAJGR_TARGET_DENY);
			return;
		}

		case	eAJGR_BUSY			:
		{
			lpActor->sendJoinGuildResult(eAJGR_TARGET_BUSY);
			return;
		}
	}

	lpGuild->updateGuildInfo();

	if	(lpGuild->m_wMemberCount	>=	lpGuild->m_wMaxMember)
	{
		lpActor->sendJoinGuildResult(eAJGR_GUILD_MEMBER_FULL);
		return;
	}

	if	(lpGuild->m_wIsWaitGuildWorkResult)
		return;
		
	lpGuild->m_wIsWaitGuildWorkResult	=	TRUE;

	g_game.sendWPJoinGuild(lpGuild->m_strName,lpActor->m_strName,lpTarget,dGUILD_CLASS_ROOKIE);
}

//
//	길드공지 변경요청을 받았다.
void
cFIELD::receiveAskChangeGuildNotice(CG_ASK_CHANGE_GUILD_NOTICE	*_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	if (!lpActor->isGuildLeader())
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_LEADER);
		return;
	}

	_lpPacket->strNotice[dGUILD_NOTICE_LENGTH-1]	=	NULL;

	if (strlen(_lpPacket->strNotice)>sizeof(_lpPacket->strNotice)-2)
	{
		lpActor->sendGuildWorkMessage(eGWM_TOO_LONG_GUILD_NOTICE_LENGTH);
		return;
	}

	g_game.sendWPChangeGuildNotice(lpActor,_lpPacket->strNotice);
}

//
//	길드세율 변경에 대한 공지
void
cFIELD::receiveAskChangeGuildExpTax(CG_ASK_CHANGE_GUILD_EXP_TAX	*_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	if (!lpActor->isGuildMaster())
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	CGuild	*lpGuild	=	lpActor->getGuild();

	if (!lpGuild)
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	int	iMaxTax	=	lpGuild->getMaximumExpTax();
	int	iMinTax	=	lpGuild->getMinimumExpTax();

	if (_lpPacket->wTax < iMinTax	|| _lpPacket->wTax > iMaxTax)
	{
		lpActor->sendGuildWorkMessage(eGWM_INCORRECT_TAX_RATE,iMinTax,iMaxTax);
		return;
	}

	g_game.sendWPChangeGuildExpTax(lpActor,_lpPacket->wTax);
}

//
//	서브 길마 해임
void
cFIELD::receiveAskDismissSubGuildMaster(CG_ASK_DISMISS_SUB_GUILD_MASTER	*_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	if (!lpActor->isGuildMaster())
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);

		return;
	}

	if (strlen(_lpPacket->strSubGuildMaster) > sizeof(_lpPacket->strSubGuildMaster)-1 || strlen(_lpPacket->strSubGuildMaster) < 4)
	{
		lpActor->sendGuildWorkMessage(eGWM_INCORRECT_MEMBER_NAME_LENGTH);
		return;
	}

	CGuild	*lpGuild	=	lpActor->getGuild();

	if (!lpGuild)
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	CGuildMan	*lpMember	=	lpGuild->getMember(_lpPacket->strSubGuildMaster);

	if (!lpMember)
	{
		lpActor->sendGuildWorkMessage(eGWM_USER_NOT_GUILD_MEMBER);
		return;
	}

	if (lpMember->m_wRank != dGUILD_CLASS_SUBMASTER)
	{
		lpActor->sendGuildWorkMessage(eGWM_USER_NOT_SUB_GUILD_MASTER);
		return;
	}

	g_game.sendWPDismissSubGuildMaster(lpActor,_lpPacket->strSubGuildMaster);
}

//
//	서브 길드 마스터 임명
void
cFIELD::receiveAskAppointmentSubGuildMaster(CG_ASK_APPOINTMENT_SUB_GUILD_MASTER	*_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	if (!lpActor->isGuildLeader())
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_LEADER);
		return;
	}

	if (strlen(_lpPacket->strSubGuildMaster) > sizeof(_lpPacket->strSubGuildMaster)-1 || strlen(_lpPacket->strSubGuildMaster) < 4)
	{
		lpActor->sendGuildWorkMessage(eGWM_INCORRECT_MEMBER_NAME_LENGTH);
		return;
	}

	CGuild	*lpGuild	=	lpActor->getGuild();

	if (!lpGuild)
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	lpGuild->updateGuildInfo();

	CGuildMan	*lpMember	=	lpGuild->getMember(_lpPacket->strSubGuildMaster);

	if (!lpMember)
	{
		lpActor->sendGuildWorkMessage(eGWM_USER_NOT_GUILD_MEMBER);
		return;
	}

	if (lpGuild->m_wSubMasterCount	>=	lpGuild->m_wMaxSubMaster)
	{
		lpActor->sendGuildWorkMessage(eGWM_TOO_MANY_SUB_GUILD_MASTER);
		return;
	}

	if (lpMember->m_wRank == dGUILD_CLASS_SUBMASTER)
	{
		lpActor->sendGuildWorkMessage(eGWM_USER_ALREADY_SUB_GUILD_MASTER);
		return;
	}

	if (lpGuild->m_wIsWaitGuildWorkResult)
		return;

	lpGuild->m_wIsWaitGuildWorkResult	=	TRUE;

	g_game.sendWPAppointmentSubGuildMaster(lpActor,_lpPacket->strSubGuildMaster);
}

void
cFIELD::receiveAskDismissGuildCongress(CG_ASK_DISMISS_GUILD_CONGRESS *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	if (!lpActor->isGuildMaster())
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	if (strlen(_lpPacket->strGuildCongress) > sizeof(_lpPacket->strGuildCongress)-1 || strlen(_lpPacket->strGuildCongress) < 4)
	{
		lpActor->sendGuildWorkMessage(eGWM_INCORRECT_MEMBER_NAME_LENGTH);
		return;
	}

	CGuild	*lpGuild	=	lpActor->getGuild();

	if (!lpGuild)
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	CGuildMan	*lpMember	=	lpGuild->getMember(_lpPacket->strGuildCongress);

	if (!lpMember)
	{
		lpActor->sendGuildWorkMessage(eGWM_USER_NOT_GUILD_MEMBER);
		return;
	}

	if (lpMember->m_wRank != dGUILD_CLASS_PARLIAMENT)
	{
		lpActor->sendGuildWorkMessage(eGWM_USER_NOT_GUILD_CONGRESS);
		return;
	}

	g_game.sendWPDismissGuildCongress(lpActor,_lpPacket->strGuildCongress);
}
//	길드 의원 임명
void
cFIELD::receiveAskAppoitmentGuildCongress(CG_ASK_APPOINTMENT_GUILD_CONGRESS	*_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	if (!lpActor->isGuildMaster())
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	if (strlen(_lpPacket->strGuildCongress) > sizeof(_lpPacket->strGuildCongress)-1 || strlen(_lpPacket->strGuildCongress) < 4)
	{
		lpActor->sendGuildWorkMessage(eGWM_INCORRECT_MEMBER_NAME_LENGTH);
		return;
	}

	CGuild	*lpGuild	=	lpActor->getGuild();

	if (!lpGuild)
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	lpGuild->updateGuildInfo();

	CGuildMan	*lpMember	=	lpGuild->getMember(_lpPacket->strGuildCongress);

	if (!lpMember)
	{
		lpActor->sendGuildWorkMessage(eGWM_USER_NOT_GUILD_MEMBER);
		return;
	}

	if (lpGuild->m_wCongressManCount	>=	lpGuild->m_wMaxCongressMan)
	{
		lpActor->sendGuildWorkMessage(eGWM_TOO_MANY_GUILD_CONGRESS);
		return;
	}

	if (lpMember->m_wRank == dGUILD_CLASS_PARLIAMENT)
	{
		lpActor->sendGuildWorkMessage(eGWM_USER_ALREADY_GUILD_CONGRESS);
		return;
	}

	if (lpGuild->m_wIsWaitGuildWorkResult)
		return;

	lpGuild->m_wIsWaitGuildWorkResult	=	TRUE;

	g_game.sendWPAppointmentGuildCongress(lpActor,_lpPacket->strGuildCongress);
}

//
//	길드 멤버 추방
void
cFIELD::receiveAskBanishGuildMember(CG_ASK_BANISH_GUILD_MEMBER	*_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(!lpActor->isGuildLeader())
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_LEADER);
		return;
	}

	if	(strlen(_lpPacket->strMember) > sizeof(_lpPacket->strMember)-1 || strlen(_lpPacket->strMember) < 4)
	{
		lpActor->sendGuildWorkMessage(eGWM_INCORRECT_MEMBER_NAME_LENGTH);
		return;
	}

	CGuild	*lpGuild	=	lpActor->getGuild();

	if	(!lpGuild)
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	CGuildMan	*lpMember	=	lpGuild->getMember(_lpPacket->strMember);

	if	(!lpMember)
	{
		lpActor->sendGuildWorkMessage(eGWM_USER_NOT_GUILD_MEMBER);
		return;
	}

	if	(lpMember->m_wRank	==	dGUILD_CLASS_MASTER)
	{
		lpActor->sendGuildWorkMessage(eGWM_CAN_NOT_BANISH_GUILD_MASTER);
		return;
	}

	if	(!lpActor->isGuildMaster()	&&	lpMember->m_wRank	>=	dGUILD_CLASS_PARLIAMENT)
	{
		lpActor->sendGuildWorkMessage(eGWM_CAN_NOT_BANISH_GUILD_CONGRESS);
		return;
	}

	g_game.sendWPBanishGuildMember(lpActor,_lpPacket->strMember);
}

//
//	길드 탈퇴 요청
void
cFIELD::receiveAskLeaveGuild(CG_ASK_LEAVE_GUILD	*_lpPacket,int _iSerial)
{
	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
		return;
	
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	CGuild	*lpGuild	=	lpActor->getGuild();

	if	(!lpGuild)
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);

		return;
	}

	if	(lpActor->isGuildMaster())
	{
		lpActor->sendGuildWorkMessage(eGWM_GUILD_MASTER_CAN_NOT_LEAVE_GUILD);

		return;
	}

	if	(lpActor->m_timeGuildJoinTime.m_dwValue)
	{
		CTimeInfo	guildJoinTime;

		guildJoinTime.m_wYear	=	(WORD)(lpActor->m_timeGuildJoinTime.year+2000);
		guildJoinTime.m_wMonth	=	lpActor->m_timeGuildJoinTime.month;
		guildJoinTime.m_wDay	=	lpActor->m_timeGuildJoinTime.day;

		DWORD	dwJoinValue		=	guildJoinTime.m_wYear*12*31+guildJoinTime.m_wMonth*31+guildJoinTime.m_wDay;
		DWORD	dwCurrentValue	=	g_currentTime.m_wYear*12*31+g_currentTime.m_wMonth*31+g_currentTime.m_wDay;


		if	(dwCurrentValue	-	dwJoinValue	<	15)
		{
			lpActor->sendGuildWorkMessage(eGWM_LEAVE_GUILD_LIMIT_BY_JOIN_DATE);
			return;
		}
	}

	g_game.sendWPLeaveGuild(lpActor);
}

//
//	길드 마스터 변경 요청
void
cFIELD::receiveAskChangeGuildMaster(CG_ASK_CHANGE_GUILD_MASTER *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	if (!lpActor->isGuildMaster())
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	if (strlen(_lpPacket->strMaster) > sizeof(_lpPacket->strMaster)-1 || strlen(_lpPacket->strMaster) < 4)
	{
		lpActor->sendGuildWorkMessage(eGWM_INCORRECT_MEMBER_NAME_LENGTH);
		return;
	}

	CGuild	*lpGuild	=	lpActor->getGuild();

	if (!lpGuild)
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);
		return;
	}

	CGuildMan	*lpMember	=	lpGuild->getMember(_lpPacket->strMaster);

	if (!lpMember)
	{
		lpActor->sendGuildWorkMessage(eGWM_USER_NOT_GUILD_MEMBER);
		return;
	}

	if (lpMember->m_wRank != dGUILD_CLASS_SUBMASTER)
	{
		lpActor->sendGuildWorkMessage(eGWM_USER_NOT_SUB_GUILD_MASTER);
		return;
	}

	g_game.sendWPChangeGuildMaster(lpActor,_lpPacket->strMaster);
}

//
//	정회원 등록
void
cFIELD::receiveAskRegistRegularMember(CG_REGIST_REGULAR_GUILD_MEMBER *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	if (!lpActor->isGuildMaster())
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);

		return;
	}

	CGuild	*lpGuild	=	lpActor->getGuild();

	if (!lpGuild || lpGuild->m_wIsWaitGuildWorkResult)
		return;

	lpGuild->m_wIsWaitGuildWorkResult	=	TRUE;

	g_game.sendWPRegistRegular(lpActor,_lpPacket->strName);
}

void
cFIELD::receiveReadyToMakeGuild(CG_READY_TO_MAKE_GUILD *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(!lpActor->m_bf1IsReadyToMakeGuild)
	{
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_READY_TO_MAKE_GUILD);

		return;
	}

	if	(lpActor->getGuild())
	{
		lpActor->m_bf1IsReadyToMakeGuild	=	FALSE;
		lpActor->sendGuildWorkMessage(eGWM_YOU_ARE_ALREADY_GUILD_MEMBER);

		return;
	}

	lpActor->readyToMakeGuild();
}

//
//	길드 스킬 레벨 올림
void
cFIELD::receiveIncreaseGuildSkillLevel(CG_INCREASE_GUILD_SKILL_LEVEL *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	CGuild	*lpGuild	=	lpActor->getGuild();

	if	(!lpGuild || lpGuild->m_wIsWaitGuildWorkResult)
		return;

	lpActor->increaseGuildSkillLevel(_lpPacket->wSkill);
}

//
//	길드전 정보를 요청 받았다.
void
cFIELD::receiveAskGuildBattleSchedule(CG_ASK_GUILD_BATTLE_SCHEDULE *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(g_gbsToday.isBlockToRequestBattleTime())
	{
		lpActor->sendGuildWorkMessage(eGWM_GUILD_BATTLE_SLEEP_TIME);

		return;
	}

	SG_GUILD_BATTLE_SCHEDULE_IN_SAME_TIME	packet;
	CTimeInfo	beginTime;

	packet.base.set(sizeof(packet),dSG_GUILD_BATTLE_SCHEDULE_IN_SAME_TIME);
	
	packet.wYear	=	g_gbsToday.m_wYear;
	packet.wMonth	=	g_gbsToday.m_wMonth;
	packet.wDay		=	g_gbsToday.m_wDay;
	packet.wTimeOrder=	_lpPacket->wTimeOrder;

	g_gbsToday.getBeginTime(packet.wTimeOrder,&beginTime);

	CGuildBattleInField	*lpSchedule		=	&g_gbsToday.m_aBattleGuildSchedule[_lpPacket->wTimeOrder];

	for (int i=0;i<dGUILD_BATTLE_COUNT_IN_SAME_TIME;i++)
	{
		packet.data.m_aBattleGuildInfo[i].m_wFieldSerial		=	lpSchedule->m_aBattleGuildInfo[i].m_wFieldSerial;

		for (int j=0;j<2;j++)
		{
			packet.data.m_aBattleGuildInfo[i].m_awBattleGuild[j]	=	lpSchedule->m_aBattleGuildInfo[i].m_awBattleGuild[j];
			packet.data.m_aBattleGuildInfo[i].m_astrGuild[j][0]		=	0;

			if (lpSchedule->m_aBattleGuildInfo[i].m_awBattleGuild[j]!=	0xffff)
			{
				CGuild	*lpGuild	=	g_guildManager.getBySerial(lpSchedule->m_aBattleGuildInfo[i].m_awBattleGuild[j]);

				if	(lpGuild)
					strcpy(packet.data.m_aBattleGuildInfo[i].m_astrGuild[j],lpGuild->m_strName);
			}
		}
	}

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

	lpActor->sendGuildBattleInfoForMember();
}

//
//	길드전 요청
void
cFIELD::receiveRequestGuildBattle(CG_REQUEST_GUILD_BATTLE *_lpPacket,int _iSerial)
{
	if	(g_bIsTestGuildBattle)
		return;

	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;
	
	if	(!lpActor->isGuildMaster())
	{
		lpActor->sendRegistServerMessage(eRSM_GUILD_ONLY_GUILD_MASTER_CAN_OPEN_GUILD_BATTLE_SCHEDULER);
		return;
	}

	if	(g_gbsToday.isBlockToRequestBattleTime())
	{
		lpActor->sendGuildWorkMessage(eGWM_GUILD_BATTLE_SLEEP_TIME);

		return;
	}

	CGuild	*lpGuild	=	lpActor->getGuild();

	if	(!lpGuild)
	{
		lpActor->sendRegistServerMessage(eRSM_GUILD_ONLY_GUILD_MASTER_CAN_OPEN_GUILD_BATTLE_SCHEDULER);
		return;
	}

	g_gbsToday.requestGuildBattle(lpActor,_lpPacket->wTimeOrder,_lpPacket->wFieldOrder);
}

//
//	길드전 취소
void
cFIELD::receiveCancelGuildBattle(CG_CANCEL_GUILD_BATTLE *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(g_gbsToday.isBlockToRequestBattleTime())
	{
		lpActor->sendGuildWorkMessage(eGWM_GUILD_BATTLE_SLEEP_TIME);

		return;
	}

	g_gbsToday.cancelGuildBattle(lpActor);
}

//
//	길드전 스케쥴러 닫기
void
cFIELD::receiveCloseGuildBattleScheduler(int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	lpActor->m_wIsOpendGuildBattleScheduler	=	FALSE;
}

//
//	길드 마크 선택
void
cFIELD::receiveSelectGuildMark(CG_SELECT_GUILD_MARK	*_lpPacket,int _iSerial)
{
	cACTOR	*lpMaster	=	getTestedActor(_iSerial);

	if	(!lpMaster)
		return;

	if	(!lpMaster->isGuildMaster())
	{
		lpMaster->sendGuildWorkMessage(eGWM_ONLY_GUILD_MASTER_CAN_SELECT_GUILD_MARK);

		return;
	}

	if	(!lpMaster->m_bf1IsReadyToSelectGuildMark)
		return;
	
	CGuild	*lpGuild	=	lpMaster->getGuild();

	if	(lpGuild)
		if	(lpGuild->askSelectGuildMark(&_lpPacket->guildMarkInfo))
			lpMaster->m_bf1IsReadyToSelectGuildMark	=	FALSE;
}

void
cFIELD::receiveAskGuildMarkInfo(CG_ASK_GUILD_MARK_INFO *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	CGuild	*lpGuild	=	g_guildManager.getBySerial(_lpPacket->wGuildSerial);

	CGuildMarkInfo	info;

	info.reset();

	if	(lpGuild)
		memcpy(&info,&lpGuild->m_guildMarkInfo,sizeof(CGuildMarkInfo));

	SG_GUILD_MARK_INFO	packet;

	packet.base.set(sizeof(packet),dSG_GUILD_MARK_INFO);
	packet.wGuildSerial	=	_lpPacket->wGuildSerial;

	if	(lpGuild)
		packet.bf1IsTwinkle	=	lpGuild->m_bf1IsTwinkle;
	else
		packet.bf1IsTwinkle	=	FALSE;

	if	(lpGuild)
		packet.bf3HallLevel	=	lpGuild->m_wHallLevel;
	else
		packet.bf3HallLevel	=	0;

	if	(lpGuild)
		packet.bf8Level		=	lpGuild->m_wLevel;
	else
		packet.bf8Level		=	0;

	memcpy(&packet.markInfo,&info,sizeof(CGuildMarkInfo));

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

void
cFIELD::receiveAskSimpleGuildInfo(CG_ASK_SIMPLE_GUILD_INFO* _lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->sendSimpleGuildInfo();
}

void
cFIELD::receiveGuildCheckSum(CG_GUILD_CHECK_SUM* _lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor || lpActor->m_wGuildSerial	==	0xffff)
		return;

	CGuild	*lpGuild	=	lpActor->getGuild();

	if	(!lpGuild)
		return;

	if	(_lpPacket->iCheckSum	!=	lpGuild->getCheckSumForClient())
		lpActor->sendGuildInfo(FALSE);
}