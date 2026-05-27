#include "cGAME.H"
#include "debugCode.H"
#include "cServer.H"
#include "cGuild.H"

void
cGAME::sendJoinGuildLog(char *_lpstrGuildName,char *_lpstrPlayerName)
{
	LOG_GUILD_BATTLE_WORK	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_GUILD_BATTLE_WORK);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wWork				=	eGBW_JOIN;

	strcpy(logPacket.strGuild1Name,_lpstrGuildName);
	strcpy(logPacket.strGuild2Name,_lpstrPlayerName);

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cGAME::sendLeaveGuildLog(char *_lpstrGuildName,char *_lpstrPlayerName)
{
	LOG_GUILD_BATTLE_WORK	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_GUILD_BATTLE_WORK);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wWork				=	eGBW_LEAVE;

	strcpy(logPacket.strGuild1Name,_lpstrGuildName);
	strcpy(logPacket.strGuild2Name,_lpstrPlayerName);

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cGAME::sendBookingGuildBattleLog(char *_lpstrGuildName,int _iTimeOrder,int _iFieldOrder)
{
	LOG_GUILD_BATTLE_WORK	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_GUILD_BATTLE_WORK);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wWork				=	eGBW_BOOKING;

	strcpy(logPacket.strGuild1Name,_lpstrGuildName);
	logPacket.bTimeOrder		=	_iTimeOrder;
	logPacket.bFieldOrder		=	_iFieldOrder;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cGAME::sendCancelGuildBattleLog(char *_lpstrGuildName)
{
	LOG_GUILD_BATTLE_WORK	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_GUILD_BATTLE_WORK);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wWork				=	eGBW_CANCEL;

	strcpy(logPacket.strGuild1Name,_lpstrGuildName);

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cGAME::sendEngageGuildBattleLog(char *_lpstrGuildName1,char *_lpstrGuildName2,int _iTimeOrder,int _iFieldOrder)
{
	LOG_GUILD_BATTLE_WORK	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_GUILD_BATTLE_WORK);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wWork				=	eGBW_ENGAGE;

	strcpy(logPacket.strGuild1Name,_lpstrGuildName1);
	strcpy(logPacket.strGuild2Name,_lpstrGuildName2);
	logPacket.bTimeOrder		=	_iTimeOrder;
	logPacket.bFieldOrder		=	_iFieldOrder;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}


void
cGAME::sendResultOfGuildBattleLog(char *_lpstrGuildName1,char *_lpstrGuildName2,int _iGuild1VP,int _iGuild2VP)
{
	LOG_GUILD_BATTLE_WORK	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_GUILD_BATTLE_WORK);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wWork				=	eGBW_RESULT;

	strcpy(logPacket.strGuild1Name,_lpstrGuildName1);
	strcpy(logPacket.strGuild2Name,_lpstrGuildName2);

	logPacket.bGuild1VictoryPoint	=	_iGuild1VP;
	logPacket.bGuild2VictoryPoint	=	_iGuild2VP;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cGAME::sendErrorOfGuildBattleLog(int _iError)
{
	LOG_GUILD_BATTLE_WORK	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_GUILD_BATTLE_WORK);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wWork				=	_iError;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}


void
cGAME::sendPreResultOfGuildBattleLog(char *_lpstrGuildName1,char *_lpstrGuildName2,int _iGuild1KP,int _iGuild2KP,int _iCount1,int _iCount2,char *_lpstrGuild1MemberName,char *_lpstrGuild2MemberName)
{
	LOG_GUILD_BATTLE_WORK			logPacket;
	LOG_GUILD_BATTLE_WORK_RESULT	*lpPacket	=	(LOG_GUILD_BATTLE_WORK_RESULT	*)&logPacket;
	
	lpPacket->base.set(sizeof(logPacket),dLOG_GUILD_BATTLE_WORK);

	lpPacket->logTime.yy		=	g_currentTime.m_wYear%100;
	lpPacket->logTime.mm		=	g_currentTime.m_wMonth;
	lpPacket->logTime.dd		=	g_currentTime.m_wDay;
	lpPacket->logTime.hour		=	g_currentTime.m_wHour;
	lpPacket->logTime.min		=	g_currentTime.m_wMinute;
	lpPacket->wWork				=	eGBW_PRE_RESULT;

	strcpy(lpPacket->strGuild1Name,_lpstrGuildName1);
	strcpy(lpPacket->strGuild2Name,_lpstrGuildName2);

	if (_lpstrGuild1MemberName)
		strcpy(lpPacket->strGuild1MemberName,_lpstrGuild1MemberName);
	else
		strcpy(lpPacket->strGuild1MemberName,"no");

	if (_lpstrGuild2MemberName)
		strcpy(lpPacket->strGuild2MemberName,_lpstrGuild2MemberName);
	else
		strcpy(lpPacket->strGuild1MemberName,"no");

	lpPacket->wKP1				=	_iGuild1KP;
	lpPacket->wKP2				=	_iGuild2KP;
	lpPacket->bGuild1MemberCount=	_iCount1;
	lpPacket->bGuild2MemberCount=	_iCount2;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cGAME::sendExterminateGuildBattleLog(char *_lpstrGuildName1,char *_lpstrGuildName2,int _iKillerTeam)
{
	LOG_GUILD_BATTLE_WORK		logPacket;
	logPacket.base.set(sizeof(logPacket),dLOG_GUILD_BATTLE_WORK);

	LOG_GUILD_BATTLE_EXTERMINATE	*lpPacket	=	(LOG_GUILD_BATTLE_EXTERMINATE	*)&logPacket;

	lpPacket->logTime.yy		=	g_currentTime.m_wYear%100;
	lpPacket->logTime.mm		=	g_currentTime.m_wMonth;
	lpPacket->logTime.dd		=	g_currentTime.m_wDay;
	lpPacket->logTime.hour		=	g_currentTime.m_wHour;
	lpPacket->logTime.min		=	g_currentTime.m_wMinute;
	lpPacket->wWork				=	eGBW_EXTERMINATE;
	lpPacket->wWinner			=	_iKillerTeam;

	strcpy(lpPacket->strGuild1Name,_lpstrGuildName1);
	strcpy(lpPacket->strGuild2Name,_lpstrGuildName2);

	lpPacket->base.wSize		=	sizeof(LOG_GUILD_BATTLE_EXTERMINATE);

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}


//
//길드 개편 관련 로그
void
cGAME::sendPlayGuildDungeonLog(CGuild *_lpGuild,cACTOR *_lpActor,int _iFloor)
{
	LOG_ABOUT_GUILD_WORK				logSystem;
	LOG_S_RECORD_GUILD_DUNGEON_PLAYTIME	*lpLog	=	(LOG_S_RECORD_GUILD_DUNGEON_PLAYTIME *)&logSystem;
	
	lpLog->base.set(sizeof(LOG_S_RECORD_GUILD_DUNGEON_PLAYTIME),dLOG_GUILD_WORK);
	
	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	0xffff;
	lpLog->wWork			=	eGW_GUILD_DUNGEON_PLAY;

	lpLog->wPlayTime		=	0;
	lpLog->wFloor			=	_iFloor;
	lpLog->wPartyMemberCount=	0;

	if	(_lpGuild)
		strcpy(lpLog->strGuildName,_lpGuild->m_strName);
	else
		strcpy(lpLog->strGuildName,_lpGuild->m_strName);

	if	(_lpActor)			//요청자 정보가 있으면.
	{
		lpLog->wPlayTime		=	_lpActor->m_bf3GuildDungeonPlayTime;
		lpLog->wFieldUniqueIndex=	_lpActor->m_wCurrentField;

		strcpy(lpLog->strId , _lpActor->m_strId);
		strcpy(lpLog->strName , _lpActor->m_strName);

		cParty	*lpParty	=	_lpActor->getParty();

		if	(lpParty)
			lpLog->wPartyMemberCount	=	lpParty->m_wMemberCount;

		CClient			*lpClient	=	_lpActor->getClient();
		
		if	(lpClient)
			memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
		else
			strcpy(lpLog->strIP,"invalid IP");
	}
	else
	{
		strcpy(lpLog->strId , "Unknown User");
		strcpy(lpLog->strName , "Unknown User");
		strcpy(lpLog->strIP,"invalid IP");
	}
	
	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);
}

//
//길드 
void
cGAME::sendTransGuildToGVGLog(CGuild *_lpGuild,cACTOR *_lpActor,char *_lpstrTransGuildName)
{
	LOG_ABOUT_GUILD_WORK	logSystem;
	LOG_S_TRANS_GUILD		*lpLog	=	(LOG_S_TRANS_GUILD *)&logSystem;
	
	lpLog->base.set(sizeof(LOG_S_TRANS_GUILD),dLOG_GUILD_WORK);
	
	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	0xffff;
	lpLog->wWork			=	eGW_TRANS_GUILD_TO_GVG_SERVER;

	if	(_lpGuild)
		strcpy(lpLog->strGuildName,_lpGuild->m_strName);
	else
		strcpy(lpLog->strGuildName,"??");

	strcpy(lpLog->strTransGuildName,_lpstrTransGuildName);

	if	(_lpActor)			//요청자 정보가 있으면.
	{
		strcpy(lpLog->strId , _lpActor->m_strId);
		strcpy(lpLog->strName , _lpActor->m_strName);

		CClient			*lpClient	=	_lpActor->getClient();
		
		if	(lpClient)
			memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
		else
			strcpy(lpLog->strIP,"invalid IP");
	}
	else
	{
		strcpy(lpLog->strId , "Unknown User");
		strcpy(lpLog->strName , "Unknown User");
		strcpy(lpLog->strIP,"invalid IP");
	}
	
	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);
}

//
//길드 개편 관련 로그
void
cGAME::sendGuildMasterGraceLog(CGuild *_lpGuild , int _iPlayTime , cACTOR *_lpActor)
{
	LOG_ABOUT_GUILD_WORK			logSystem;
	LOG_S_RECORD_GUILD_MASTER_GRACE	*lpLog	=	(LOG_S_RECORD_GUILD_MASTER_GRACE	*)&logSystem;
	
	lpLog->base.set(sizeof(LOG_S_RECORD_GUILD_MASTER_GRACE),dLOG_GUILD_WORK);
	
	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	0xffff;
	strcpy(lpLog->strGuildName , _lpGuild->m_strName);
	lpLog->wWork			=	eGW_ACTIVE_GUILD_MASTER_GRACE;

	lpLog->wPlayTime		=	_iPlayTime;

	if	(_lpActor)			//요청자 정보가 있으면.
	{
		lpLog->wFieldUniqueIndex=	_lpActor->m_wCurrentField;
		strcpy(lpLog->strId , _lpActor->m_strId);
		strcpy(lpLog->strName , _lpActor->m_strName);
		
		CClient			*lpClient	=	_lpActor->getClient();

		if	(lpClient)
			memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
		else
			strcpy(lpLog->strIP,"invalid IP");
	}
	else
	{
		strcpy(lpLog->strId , "Unknown User");
		strcpy(lpLog->strName , "Unknown User");
		strcpy(lpLog->strIP,"invalid IP");
	}
	
	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);
}

//
//길드 개편 관련 로그
void
cGAME::sendGuildRoenGoodWillLog(CGuild *_lpGuild, cACTOR *_lpActor , int LastData , int NewData)
{
	LOG_ABOUT_GUILD_WORK			logSystem;
	LOG_S_RECORD_GUILD_ROEN_GOOD_WILL	*lpLog	=	(LOG_S_RECORD_GUILD_ROEN_GOOD_WILL	*)&logSystem;

	lpLog->base.set(sizeof(LOG_S_RECORD_GUILD_ROEN_GOOD_WILL),dLOG_GUILD_WORK);
	
	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	0xffff;
	strcpy(lpLog->strGuildName , _lpGuild->m_strName);
	lpLog->wWork			=	eGW_ACTIVE_GUILD_ROEN_GOOD_WILL;

	if	(_lpActor)			//요청자 정보가 있으면.
	{
		lpLog->wFieldUniqueIndex=	_lpActor->m_wCurrentField;
		strcpy(lpLog->strId , _lpActor->m_strId);
		strcpy(lpLog->strName , _lpActor->m_strName);
		
		CClient			*lpClient	=	_lpActor->getClient();

		if	(lpClient)
			memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
		else
			strcpy(lpLog->strIP,"invalid IP");
	}
	else
	{
		strcpy(lpLog->strId , "Unknown User");
		strcpy(lpLog->strName , "Unknown User");
		strcpy(lpLog->strIP,"invalid IP");
	}

	lpLog->wLast			=	LastData;
	lpLog->wNew				=	NewData;

	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);
}

void
cGAME::sendBeginGuildBattleLog(CGuild *_lpGuild1,CGuild *_lpGuild2,int _iCount1,int _iCount2)
{
	LOG_GUILD_BATTLE_WORK		logPacket;
	logPacket.base.set(sizeof(logPacket),dLOG_GUILD_BATTLE_WORK);
	
	LOG_GUILD_BATTLE_BEGIN_WORK	*lpPacket	=	(LOG_GUILD_BATTLE_BEGIN_WORK	*)&logPacket;

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wWork				=	eGBW_BEGIN;

	strcpy(logPacket.strGuild1Name,_lpGuild1->m_strName);
	strcpy(logPacket.strGuild2Name,_lpGuild2->m_strName);

	lpPacket->m_aInfo[0].m_wVictory			=	_lpGuild1->m_wVictoryCount;
	lpPacket->m_aInfo[0].m_wDefeat			=	_lpGuild1->m_wDefeatCount;
	lpPacket->m_aInfo[0].m_wFightAtWeek		=	0;
	lpPacket->m_aInfo[0].m_wMemberCount		=	_iCount1;
	lpPacket->m_aInfo[0].m_wCVCount			=	_lpGuild1->m_wVictoryContinue;
	lpPacket->m_aInfo[0].m_iVictoryPoint	=	_lpGuild1->m_iVictoryPoint;

	lpPacket->m_aInfo[1].m_wVictory			=	_lpGuild2->m_wVictoryCount;
	lpPacket->m_aInfo[1].m_wDefeat			=	_lpGuild2->m_wDefeatCount;
	lpPacket->m_aInfo[1].m_wFightAtWeek		=	0;
	lpPacket->m_aInfo[1].m_wMemberCount		=	_iCount2;
	lpPacket->m_aInfo[1].m_wCVCount			=	_lpGuild2->m_wVictoryContinue;
	lpPacket->m_aInfo[1].m_iVictoryPoint	=	_lpGuild2->m_iVictoryPoint;

	logPacket.base.wSize	=	sizeof(LOG_GUILD_BATTLE_BEGIN_WORK);

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cGAME::sendKillPlayerGuildBattleLog(char *_lpstrGuildName1,char *_lpstrGuildName2,char *_lpstrKiller,char *_lpstrTarget,int _iAddKP,int _iGuild1KP,int _iGuild2KP,int _iKillerTeam)
{
	LOG_GUILD_BATTLE_WORK	logPacket;

	LOG_GUILD_BATTLE_WORK_KILL3	*lpPacket	=	(LOG_GUILD_BATTLE_WORK_KILL3 *)&logPacket;

	lpPacket->base.set(sizeof(logPacket),dLOG_GUILD_BATTLE_WORK);

	lpPacket->logTime.yy		=	g_currentTime.m_wYear%100;
	lpPacket->logTime.mm		=	g_currentTime.m_wMonth;
	lpPacket->logTime.dd		=	g_currentTime.m_wDay;
	lpPacket->logTime.hour		=	g_currentTime.m_wHour;
	lpPacket->logTime.min		=	g_currentTime.m_wMinute;
	lpPacket->wWork				=	eGBW_NEW_KILL;

	strcpy(lpPacket->strGuildName[0],_lpstrGuildName1);
	strcpy(lpPacket->strGuildName[1],_lpstrGuildName2);
	strcpy(lpPacket->strKiller,_lpstrKiller);
	strcpy(lpPacket->strTarget,_lpstrTarget);

	lpPacket->bf2KillerTeam		=	_iKillerTeam;
	lpPacket->bf10AddKP			=	_iAddKP;
	lpPacket->wKP1				=	_iGuild1KP;
	lpPacket->wKP2				=	_iGuild2KP;
	lpPacket->base.wSize		=	sizeof(LOG_GUILD_BATTLE_WORK_KILL3);

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}


//
//	OX 퀴즈 승자 로그 남기기
void
cGAME::sendQuizEventWinnerLog(int _iType,char *_lpstrWinners,int _iCount,int _iReviveLoserBattleCount)
{
	LOG_EVENT	logPacket;

	_iCount					=	min(_iCount,100);

	logPacket.base.set(sizeof(logPacket),dLOG_EVENT);
	logPacket.base.wSize	=	sizeof(logPacket)-sizeof(logPacket.astrName)+dNAME_LENGTH*_iCount;

	logPacket.logTime.yy	=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm	=	g_currentTime.m_wMonth;
	logPacket.logTime.dd	=	g_currentTime.m_wDay;
	logPacket.logTime.hour	=	g_currentTime.m_wHour;
	logPacket.logTime.min	=	g_currentTime.m_wMinute;
	logPacket.wCount		=	_iCount;
	logPacket.wType			=	_iType;
	logPacket.wValue		=	_iReviveLoserBattleCount;

	for (int i=0;i<_iCount;i++)
		strcpy(logPacket.astrName[i],_lpstrWinners+i*dNAME_LENGTH);

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);
}

//
//	OX 퀴즈 참가자 로그 남기기
void
cGAME::sendQuizComerListLog(char *_lpstrComerList,int _iCount,int _iType)
{
	LOG_EVENT	logPacket;

	_iCount					=	min(_iCount,100);

	logPacket.base.set(sizeof(logPacket),dLOG_EVENT);
	logPacket.base.wSize	=	sizeof(logPacket)-sizeof(logPacket.astrName)+dNAME_LENGTH*_iCount;

	logPacket.logTime.yy	=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm	=	g_currentTime.m_wMonth;
	logPacket.logTime.dd	=	g_currentTime.m_wDay;
	logPacket.logTime.hour	=	g_currentTime.m_wHour;
	logPacket.logTime.min	=	g_currentTime.m_wMinute;
	logPacket.wCount		=	_iCount;
	logPacket.wType			=	_iType;

	for (int i=0;i<_iCount;i++)
		strcpy(logPacket.astrName[i],_lpstrComerList+i*dNAME_LENGTH);

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);
}

//
//	OX 퀴즈 탈락자 로그
void
cGAME::sendQuizLoserListLog(char *_lpstrLoserList,int _iCount,int _iQuestionCounter,int _iType)
{
	LOG_EVENT	logPacket;

	_iCount					=	min(_iCount,100);

	logPacket.base.set(sizeof(logPacket),dLOG_EVENT);
	logPacket.base.wSize	=	sizeof(logPacket)-sizeof(logPacket.astrName)+dNAME_LENGTH*_iCount;

	logPacket.logTime.yy	=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm	=	g_currentTime.m_wMonth;
	logPacket.logTime.dd	=	g_currentTime.m_wDay;
	logPacket.logTime.hour	=	g_currentTime.m_wHour;
	logPacket.logTime.min	=	g_currentTime.m_wMinute;
	logPacket.wCount		=	_iCount;
	logPacket.wValue		=	_iQuestionCounter;
	logPacket.wType			=	_iType;

	for (int i=0;i<_iCount;i++)
		strcpy(logPacket.astrName[i],_lpstrLoserList+i*dNAME_LENGTH);

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);
}

//	OX 퀴즈 문제
void
cGAME::sendOXQuizQuestionLog(char *_lpstrQuestion,int _iSolution,int _iQuestionCounter)
{
	LOG_EVENT	logPacket;

	COXQuizQuesionForLogPacket	*lpQuestionInfo	=	(COXQuizQuesionForLogPacket	*)logPacket.astrName;

	logPacket.base.set(sizeof(logPacket),dLOG_EVENT);
	logPacket.base.wSize	=	sizeof(logPacket)-sizeof(logPacket.astrName)+sizeof(COXQuizQuesionForLogPacket);

	logPacket.logTime.yy	=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm	=	g_currentTime.m_wMonth;
	logPacket.logTime.dd	=	g_currentTime.m_wDay;
	logPacket.logTime.hour	=	g_currentTime.m_wHour;
	logPacket.logTime.min	=	g_currentTime.m_wMinute;

	strcpy(lpQuestionInfo->m_strQuestion,_lpstrQuestion);
	lpQuestionInfo->m_wQuestionCounter	=	_iQuestionCounter;
	lpQuestionInfo->m_wSolution			=	_iSolution;

	logPacket.wType			=	eEW_OXQUIZ_QUESTION;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);
}

//	OX 퀴즈 문제
void
cGAME::sendWordQuizQuestionLog(char *_lpstrQuestion,char *_lpstrSolution,int _iQuestionCounter)
{
	LOG_EVENT	logPacket;

	cWordQuizQuesionForLogPacket	*lpQuestionInfo	=	(cWordQuizQuesionForLogPacket	*)logPacket.astrName;

	logPacket.base.set(sizeof(logPacket),dLOG_EVENT);
	logPacket.base.wSize	=	sizeof(logPacket)-sizeof(logPacket.astrName)+sizeof(COXQuizQuesionForLogPacket);

	logPacket.logTime.yy	=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm	=	g_currentTime.m_wMonth;
	logPacket.logTime.dd	=	g_currentTime.m_wDay;
	logPacket.logTime.hour	=	g_currentTime.m_wHour;
	logPacket.logTime.min	=	g_currentTime.m_wMinute;

	strcpy(lpQuestionInfo->m_strQuestion,_lpstrQuestion);
	strcpy(lpQuestionInfo->m_strSolution,_lpstrSolution);
	lpQuestionInfo->m_wQuestionCounter	=	_iQuestionCounter;

	logPacket.wType			=	eEW_WORDQUIZ_QUESTION;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);
}
