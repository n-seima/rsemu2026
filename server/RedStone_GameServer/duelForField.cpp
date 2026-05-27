#include "cFIELD.h"
#include "cGame.h"
#include "booking_work.h"
#include "cguild.h"
#include "smtp.h"

//#define	dMAIL_SERVER_IP		"211.189.66.49"

void
cFIELD::clsoeDuelField()
{
	m_wGuildBattleStatus		=	eGB_SLEEP_TIME;

	for (int iTeam=0;iTeam<2;iTeam++)
		for (int iMember=0;iMember<5;iMember++)
			m_aDuelBattler[iTeam][iMember].m_wTeam		=	0xffff;
}

void
cFIELD::setDuelMatchInfo(cDuelApplicantInfo *_lpTeam1,cDuelApplicantInfo *_lpTeam2,BOOL _bIsTrialGameBattle)
{
	ZeroMemory(m_aDuelTeam[0].m_strTeamName , sizeof(m_aDuelTeam[0].m_strTeamName));
	ZeroMemory(m_aDuelTeam[1].m_strTeamName , sizeof(m_aDuelTeam[1].m_strTeamName));

	m_iDuelSerial					=	g_iDuelFieldSerial++;
	m_bIsTrialGameBattle			=	_bIsTrialGameBattle;
	m_wWaitForEnterDuelFieldTime	=	c_iWaitForEnterDuelFieldTime;

	m_aDuelTeam[0].copy(_lpTeam1);
	m_aDuelTeam[1].copy(_lpTeam2);

	strcpy(m_aDuelTeam[0].m_strTeamName,"Blue");
	strcpy(m_aDuelTeam[1].m_strTeamName,"Red");

	if	(m_bIsTrialGameBattle)
	{
		cAdvDuelTeamInfo	*lpTeam1	=	g_duelTrialGameManager.getTeamByMember(_lpTeam1->m_strLeaderName);
		cAdvDuelTeamInfo	*lpTeam2	=	g_duelTrialGameManager.getTeamByMember(_lpTeam2->m_strLeaderName);

		if	(!lpTeam1	||	!lpTeam2)
			return;

		strcpy(m_aDuelTeam[0].m_strTeamName,lpTeam1->m_strTeamName);
		strcpy(m_aDuelTeam[1].m_strTeamName,lpTeam2->m_strTeamName);
	}

	m_wDuelMatchStep	=	eDUEL_MATCH_STEP_ENTRY;
	m_wGuildBattleStatus=	eGB_ENTERENCE;

	memset(m_awKillCount,0,sizeof(m_awKillCount));

	cACTOR	*lpaBattler[2];

	lpaBattler[0]	=	_lpTeam1->m_lpActor;
	lpaBattler[1]	=	_lpTeam2->m_lpActor;

	{
		for (int iTeam=0;iTeam<2;iTeam++)
			for (int iMember=0;iMember<5;iMember++)
				m_aDuelBattler[iTeam][iMember].reset();
	}

	if	(m_wDuelType	== eDUEL_1VS1 || (m_wDuelType	== eDUEL_TRIAL_GAME && g_iTrialGameTeamSize == 1) )
	{
		int	iMember	=	0;

		for (int iTeam=0;iTeam<2;iTeam++)
		{
			cACTOR	*lpMember	=	lpaBattler[iTeam];

			strcpy(m_aDuelBattler[iTeam][iMember].m_strId,lpMember->m_strId);
			strcpy(m_aDuelBattler[iTeam][iMember].m_strName,lpMember->m_strName);
			m_aDuelBattler[iTeam][iMember].m_wTeam		=	iTeam;
			m_aDuelBattler[iTeam][iMember].m_wKillCount	=	0;
			m_aDuelBattler[iTeam][iMember].m_wDeathCount=	0;
		}
	}
	else
	{

		for (int iTeam=0;iTeam<2;iTeam++)
		{
			cParty	*lpTeam	=	lpaBattler[iTeam]->getParty();;

			for (int iMember=0;iMember<lpTeam->m_wMemberCount;iMember++)
			{
				cACTOR	*lpMember	=	lpTeam->getActorByMemberIndex(iMember);

				strcpy(m_aDuelBattler[iTeam][iMember].m_strId,lpMember->m_strId);
				strcpy(m_aDuelBattler[iTeam][iMember].m_strName,lpMember->m_strName);
				m_aDuelBattler[iTeam][iMember].m_wTeam		=	iTeam;
				m_aDuelBattler[iTeam][iMember].m_wKillCount	=	0;
				m_aDuelBattler[iTeam][iMember].m_wDeathCount=	0;
			}
		}
	}
}

void
cFIELD::sendDuelResult()
{
	int	iWinTeam	=	2;
	int	aiTrialVP[2]=	{0,0};

	if	(m_awKillCount[0]	>	m_awKillCount[1])
		iWinTeam	=	0;
	if	(m_awKillCount[1]	>	m_awKillCount[0])
		iWinTeam	=	1;

	if	(m_aiActorCount[0]	==	0)
		iWinTeam	=	1;
	if	(m_aiActorCount[1]	==	0)
		iWinTeam	=	0;

	SG_DUEL_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_DUEL_RESULT);

	memcpy(packet.awKillCount,m_awKillCount,sizeof(m_awKillCount));
	packet.wWinTeam				=	iWinTeam;
	strcpy(packet.strTrialGameName,g_duelTrialGameManager.m_strTrialGameName);

	packet.iVPForWinner			=	GetWinPointForDuel(m_wDuelType);
	packet.iDuelPointForWinner	=	GetDuelPointForDuel(m_wDuelType);
	packet.iVPForDefeater		=	GetWinPointForDuel(m_wDuelType,TRUE);
	packet.iDuelPointForDefeater=	GetDuelPointForDuel(m_wDuelType,TRUE);
	packet.iTrialGameVPForWinner=	0;
	packet.iTrialGameVPForDefeater=	0;

	if	(m_bIsTrialGameBattle)
	{
		packet.iTrialGameVPForWinner	=	GetWinPointForDuel(m_wDuelType);
		packet.iTrialGameVPForDefeater	=	GetWinPointForDuel(m_wDuelType,TRUE);

		cDuelTeamInfo	*lpaTeam[2]	=	{NULL,};

		lpaTeam[0]	=	g_duelTrialGameManager.getTeamByMember(m_aDuelBattler[0][0].m_strId);
		lpaTeam[1]	=	g_duelTrialGameManager.getTeamByMember(m_aDuelBattler[1][0].m_strId);

		if	(lpaTeam[0])
			aiTrialVP[0]	=	lpaTeam[0]->m_record.m_iWinPoint;
		if	(lpaTeam[1])
			aiTrialVP[1]	=	lpaTeam[1]->m_record.m_iWinPoint;
	}
	else
	{
		packet.iTrialGameVPForWinner	=	0;
		packet.iTrialGameVPForDefeater	=	0;
	}

	memset(packet.m_aBattler,0,sizeof(packet.m_aBattler));

	for (int iTeam=0;iTeam<2;iTeam++)
	{
		int	iStack	=	0;

		for (int iMember=0;iMember<5;iMember++)
		{
			cDuelBattler	*lpBattler	=	&m_aDuelBattler[iTeam][iMember];
			cACTOR			*lpActor	=	getPlayerByName(lpBattler->m_strName);

			if	(!lpActor)
				continue;

			cDuelBattlerInfoForResult	*lpInfo		=	&packet.m_aBattler[iTeam][iStack];

			strcpy(lpInfo->m_strName,lpBattler->m_strName);

			lpInfo->m_iDuelPoint	=	lpActor->m_duelRecord.m_iDuelPoint;
			lpInfo->m_iTrialGameVP	=	aiTrialVP[iTeam];
			lpInfo->m_iWeeklyVP		=	lpActor->m_duelRecord.m_iWeeklyWinPoint;
			lpInfo->m_iDuelPoint	=	lpActor->m_duelRecord.m_iDuelPoint;
			lpInfo->m_wKillCount	=	lpBattler->m_wKillCount;
			lpInfo->m_wDeathCount	=	lpBattler->m_wDeathCount;

			lpActor->sendDuelRecord();

			cParty	*lpParty		=	lpActor->getParty();

			if	(lpParty)
			{
				lpParty->syncMemberStatus(lpActor);
				lpParty->syncMemberInfo(lpActor->m_strName);
			}

			iStack++;
		}
	}

	addSendPacket(&packet,0xffff);
	
	sendEndDuelLog();
}

cDuelBattler*
cFIELD::getDuelBattler(cACTOR *_lpActor)
{
	for (int i=0;i<5;i++)
	{
		cDuelBattler	*lpBattler	=	&m_aDuelBattler[_lpActor->m_wTeam][i];

	
		if	(lpBattler->m_wTeam	==	0xffff)
			return	NULL;

		if	(STRICMP(lpBattler->m_strName,_lpActor->m_strName)	==	0)
			return	lpBattler;
	}

	return	NULL;
}

void
cFIELD::operateDuelBattleResult()
{
	m_wGuildBattleStatus=	eGB_FINISH;
	m_wDuelMatchStep	=	eDUEL_MATCH_STEP_FINISH;
	m_wWaitForExitTime	=	c_iWaitTimeToBanishFromDuelField;

	int	iWinTeam	=	2;

	if	(m_awKillCount[0]	>	m_awKillCount[1])
		iWinTeam	=	0;
	if	(m_awKillCount[1]	>	m_awKillCount[0])
		iWinTeam	=	1;

	if	(m_aiActorCount[0]	==	0)
		iWinTeam	=	1;
	if	(m_aiActorCount[1]	==	0)
		iWinTeam	=	0;

	int	iVPForWinner			=	GetWinPointForDuel(m_wDuelType);
	int	iDuelPointForWinner		=	GetDuelPointForDuel(m_wDuelType);
	int	iVPForDefeater			=	GetWinPointForDuel(m_wDuelType,TRUE);
	int	iDuelPointForDefeater	=	GetDuelPointForDuel(m_wDuelType,TRUE);

	cDuelTeamInfo	*lpaTeam[2]	=	{NULL,};

	if	(m_bIsTrialGameBattle)
	{
		lpaTeam[0]	=	g_duelTrialGameManager.getTeamByMember(m_aDuelBattler[0][0].m_strId);
		lpaTeam[1]	=	g_duelTrialGameManager.getTeamByMember(m_aDuelBattler[1][0].m_strId);

		for (int iTeam=0;iTeam<2;iTeam++)
		{
			if	(lpaTeam[iTeam]	==	NULL)
				continue;

			int	iVP	=	0;

			cDuelTeamRecord	*lpRecord	=	&lpaTeam[iTeam]->m_record;

			if	(iTeam	==	iWinTeam)	//	승리
			{
				lpRecord->m_iWinCount++;	//	승리 횟수
				iVP	=	iVPForWinner;
			}
			else
			if	(iWinTeam	==	2)	//	무승부
			{
				lpRecord->m_iDrawCount++;	//	무승부 횟수

				iVP	=	iVPForDefeater;
			}
			else	//	패배
			{
				lpRecord->m_iDefeatCount++;	//	패배 횟수

				iVP	=	iVPForDefeater;
			}

			lpRecord->m_iBattleCount++;	//	전투 횟수

			lpRecord->m_iWinPoint		+=	iVP;//	TrialGame 승점 : TrialGame만을 위한 승점

			//	월드에 lpRecord 값 전송
			{
				SERVERStoWORLD_UPDATE_DUEL_TEAM_RECORD	packet;

				packet.base.set(sizeof(packet),dSERVERStoWORLD_UPDATE_DUEL_TEAM_RECORD);

				strcpy(packet.strTeamName,lpaTeam[iTeam]->m_strTeamName);
				memcpy(&packet.record,lpRecord,sizeof(cDuelTeamRecord));

				SEND_WOLRD_PACKET(&packet);
			}
		}

		cSmtp mail;
		
		if	(g_duelTrialGameManager.m_wIsSendTrialGameResultMail && mail.ConnectServer( cConfigData::Inst().m_strMailServerIP.c_str() ))
		{
// #ifdef	_FOR_KOREA
// 			mail.SetSenderName("[KR] [Red Stone] Game-Result<WCG@logickorea.co.kr>");
// 			mail.SetSenderEmail("WCG@logickorea.co.kr");
// 			mail.SetSubject("[KR] [Red Stone] Game-Result");
// 			mail.AddRecipient("rs_result@worldcybergame.com");
// 			mail.AddCCRecipient("nemo@logickorea.co.kr"); //	-> cc추가할 때 .. 한개 이상이면 이 줄을 복사해서 쓰시면 됩니다.
// #elif	_FOR_JAPAN
// 			mail.SetSenderName("[JP] [Red Stone] Game-Result<WCG@logickorea.co.kr>");
// 			mail.SetSenderEmail("WCG@logickorea.co.kr");
// 			mail.SetSubject("[JP] [Red Stone] Game-Result");
// 			mail.AddRecipient("rs_result@worldcybergame.com");
// 			mail.AddCCRecipient("redstone_team@gameon.co.jp");
// #elif	_FOR_USA
// 			mail.SetSenderName("[US] [Red Stone] Game-Result<WCG@logickorea.co.kr>");
// 			mail.SetSenderEmail("WCG@logickorea.co.kr");
// 			mail.SetSubject("[US] [Red Stone] Game-Result");
// 			mail.AddRecipient("rs_result@worldcybergame.com");
// 			mail.AddCCRecipient("briankonar@k2network.net"); //	-> cc추가할 때 .. 한개 이상이면 이 줄을 복사해서 쓰시면 됩니다.
// #endif
			mail.SetSenderName( cConfigData::Inst().m_strWCGMailTitle.c_str() );
			mail.SetSenderEmail( cConfigData::Inst().m_strWCGMailSender.c_str() );
			mail.SetSubject( cConfigData::Inst().m_strWCGMailSubject.c_str() );
			mail.AddRecipient( 	cConfigData::Inst().m_strWCGMailRecipient.c_str() );
			int count = cConfigData::Inst().m_listWCGMailCCRecipient.size();
			for(int c=0;c<count;c++)
				mail.AddCCRecipient( cConfigData::Inst().m_listWCGMailCCRecipient.at(c).c_str() );

			CTimeInfo	curTime;

			

			curTime.update();

			char	strText[512];
			char	*strResult[]	=	{"Win","Loss","Draw"};

			int		iFullLelgth=GetDuelBattleTime(m_wDuelType);	
			int		iLength	=	iFullLelgth-m_wDuelBattleTimeCounter;
			int		aiResult[2]	=	{0,0};

			if	(iWinTeam	==	0)
				aiResult[1]	=	1;
			else
			if	(iWinTeam	==	1)
				aiResult[0]	=	1;
			else
			{
				aiResult[0]	=	2;
				aiResult[1]	=	2;
			}

			iLength			=	max(iLength,1);
			iLength			=	min(iLength,iFullLelgth);

			sprintf(strText,"<game program=\"RedStone\" start=\"%d-%.2d-%.2d %.2d:%.2d:%.2d\" length=\"%d\"><team name=\"%s\" result=\"%s\"/><team name=\"%s\" result=\"%s\"/></game>",
				m_beginDuelTime.m_wYear,m_beginDuelTime.m_wMonth,m_beginDuelTime.m_wDay,m_beginDuelTime.m_wHour,m_beginDuelTime.m_wMinute,m_beginDuelTime.m_wSecond,
				iLength,
				lpaTeam[0]->m_strTeamName,strResult[aiResult[0]],
				lpaTeam[1]->m_strTeamName,strResult[aiResult[1]]);

			mail.SetMessageBody(strText);

			if	(mail.GetConnectStatus())
 			{
 				int ret = mail.Send();

 				mail.Disconnect();
 			}
		 }
	}
	else
	for (int iTeam=0;iTeam<2;iTeam++)
	{
		for (int iMember=0;iMember<5;iMember++)
		{
			cDuelBattler	*lpBattler	=	&m_aDuelBattler[iTeam][iMember];
			cACTOR			*lpActor	=	getPlayerByName(lpBattler->m_strName);

			if	(!lpActor)
				continue;

			int	iWeeklyVP	=	0;
			int	iTrialGameVP		=	0;
			int	iDuelPoint	=	0;

			cDuelRecord	*lpRecord	=	&lpActor->m_duelRecord;

			if	(lpActor->m_wTeam	==	iWinTeam)	//	승리
			{
				lpRecord->m_iWinCount++;	//	승리 횟수
				lpRecord->m_iWeeklyWinCount++;		//	주간 결투 승리 횟수

				if	(m_bIsTrialGameBattle)
					iTrialGameVP		=	iVPForWinner;

				iWeeklyVP	=	iVPForWinner;
				iDuelPoint	=	iDuelPointForWinner;
			}
			else
			if	(iWinTeam	==	2)	//	무승부
			{
				lpRecord->m_iDrawCount++;	//	무승부 횟수
				lpRecord->m_iWeeklyDrawCount++;		//	주간 결투 무승부 횟수

				if	(m_bIsTrialGameBattle)
					iTrialGameVP		=	iVPForDefeater;

				iWeeklyVP	=	iVPForDefeater;
				iDuelPoint	=	iDuelPointForDefeater;
			}
			else	//	패배
			{
				lpRecord->m_iDefeatCount++;	//	패배 횟수
				lpRecord->m_iWeeklyDefeatCount++;	//	주간 결투 패배 횟수

				if	(m_bIsTrialGameBattle)
					iTrialGameVP		=	iVPForDefeater;

				iWeeklyVP	=	iVPForDefeater;
				iDuelPoint	=	iDuelPointForDefeater;
			}

			lpRecord->m_iBattleCount++;	//	전투 횟수
			lpRecord->m_iWeeklyBattleCount++;	//	주간 결투 횟수

			lpRecord->m_iTrialGameWinPoint	+=	iTrialGameVP;//	TrialGame 승점 : TrialGame만을 위한 승점
			lpRecord->m_iDuelPoint		+=	iDuelPoint;	//	결투 포인트 : 보상 지급용
			lpRecord->m_iWeeklyWinPoint	+=	iWeeklyVP;		//	주간 결투 승점

			strcpy(lpRecord->m_strID,lpActor->m_strId);
			strcpy(lpRecord->m_strName,lpActor->m_strName);
			lpRecord->m_wJob			=	lpActor->m_wJob;

			//	월드에 lpRecord 값 전송
			{
				SERVERStoWORLD_UPDATE_DUEL_RECORD	packet;

				packet.base.set(sizeof(packet),dSERVERStoWORLD_UPDATE_DUEL_RECORD);

				strcpy(packet.strId,lpActor->m_strId);
				memcpy(&packet.record,lpRecord,sizeof(cDuelRecord));

				SEND_WOLRD_PACKET(&packet);
			}
		}
	}

	sendDuelResult();
}

bool
cFIELD::killDuelBattler(cACTOR *_lpKiller,cACTOR *_lpTarget)
{
	if	(_lpTarget && _lpTarget->isPlayer() && _lpTarget->m_wTeam < dTEAM_COUNT-1)
	{

		_lpTarget->dispell();
		_lpTarget->cureWrongStatus();
		
		if	(m_wDuelMatchStep	!=	eDUEL_MATCH_STEP_BATTLE)
		{
			_lpTarget->revive(0,100);

			return	false;
		}

		//	플레이어 죽으면 펫이나 소환수도 같이 사라짐
		{
			int	i;
			
			for (i=0;i<2;i++)
			{
				_lpTarget->removeSummonBeast(i);

				cACTOR *lpActor	=	_lpTarget->getSummonBeastActor(i);
				
				if	(lpActor)
					lpActor->immediatelyExit();
				
				lpActor	=	_lpTarget->getPetActor(i);
				
				if	(lpActor)
					lpActor->immediatelyExit();
			}
		}

		_lpTarget->m_wRemainTimeForRebirth	=	10;
		_lpTarget->sendEtcWork(eEW_OPERATE_DUEL_MATCH,5,_lpTarget->m_wRemainTimeForRebirth);
	}
	else
		return	false;

	if(_lpTarget && _lpTarget->m_wSoulBurnCaster != 0xffff)
	{
		cACTOR* lpSoulBurnActor = getActor(_lpTarget->m_wSoulBurnCaster);
		if(lpSoulBurnActor && lpSoulBurnActor->m_dwNameHashCode == _lpTarget->m_dwSoulBurnCasterNameHashCode)
		{	
			if(!_lpKiller)
				_lpKiller = lpSoulBurnActor;
		}

	}

	if	(!_lpKiller || !_lpTarget )
		return	false;

	if	(_lpKiller)
		_lpKiller	=	_lpKiller->getControlLord();

	if	(!_lpKiller->isPlayer()|| _lpKiller->m_wTeam >= 2)
		return	false;

	m_awKillCount[_lpKiller->m_wTeam]++;

	cDuelBattler	*lpKiller	=	getDuelBattler(_lpKiller);
	cDuelBattler	*lpTarget	=	getDuelBattler(_lpTarget);

	if	(lpKiller)
		lpKiller->m_wKillCount++;

	if	(lpTarget)
		lpTarget->m_wDeathCount++;

	SG_GUILD_BATTLE_KILL_NOTICE	packet;

	packet.base.set(sizeof(packet),dSG_GUILD_BATTLE_KILL_NOTICE);

	packet.wKillerTeam		=	_lpKiller->m_wTeam;
	packet.wGetPoint		=	1;
	packet.wIsDuel			=	TRUE;
	packet.iGuild1KillPoint	=	m_awKillCount[0];
	packet.iGuild2KillPoint	=	m_awKillCount[1];

	strcpy(packet.strKiller,_lpKiller->m_strName);
	strcpy(packet.strTarget,_lpTarget->m_strName);

	addSendPacket((ALL_MSG*)&packet,0xffff);

	if	(m_awKillCount[_lpKiller->m_wTeam]	>=	GetKillCountForDuel(m_wDuelType))	//	킬 수에 의한 승리
	{
		operateDuelBattleResult();

		return	TRUE;
	}

	return	false;
}

int	
cFIELD::getTeamInDuelField(char *_lpstrName)
{
	for (int iTeam=0;iTeam<2;iTeam++)
		for (int i=0;i<5;i++)
		{
			cDuelBattler	*lpBattler	=	&m_aDuelBattler[iTeam][i];

			if	(lpBattler->m_wTeam	==	0xffff)
				continue;

			if	(strlen(lpBattler->m_strName)	>=	dNAME_LENGTH-1)
				continue;

			if	(STRICMP(lpBattler->m_strName,_lpstrName)	==	0)
				return	iTeam;
		}

	return	-1;
}

bool
cFIELD::checkReadyToBeginDuelMatch()//	결투 시작 준비가 되었는지 확인.
{
	for (int iTeam=0;iTeam<2;iTeam++)
	{
		int	iMemberCount	=	0;

		for (int iActor=m_aiActorCount[iTeam];iActor;)
		{
			--iActor;

			cACTOR	*lpActor	=	m_alpActor[m_awActorList[iTeam][iActor]];

			if	(lpActor	==	NULL || lpActor->m_iRookieTime)
				continue;

			iMemberCount++;
		}

		if	(iMemberCount	<	m_aDuelTeam[iTeam].m_wMemberCount)
			return	false;
	}

	return	true;
}

void
cFIELD::sendBeginDuelLog()
{
	LOG_SYSTEM			logSystem;
	LOG_S_BEGIN_DUEL	*lpLog	=	(LOG_S_BEGIN_DUEL	*)&logSystem;
	
	lpLog->base.set(sizeof(LOG_S_RESULT_OF_DUEL),dLOG_SYSTEM);
	
	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	m_iUniqueSerial;
	lpLog->wWork			=	eSLT_BEGIN_DUEL;
	lpLog->iDuelSerial		=	m_iDuelSerial;

	int iTeam;

	for (iTeam=0;iTeam<2;iTeam++)
		strcpy(lpLog->astrTeamName[iTeam],m_aDuelTeam[iTeam].m_strTeamName);

	for (iTeam=0;iTeam<2;iTeam++)
	{
		for (int iMember=0;iMember<5;iMember++)
		{
			cDuelBattler	*lpBattler	=	&m_aDuelBattler[iTeam][iMember];
			cACTOR			*lpActor	=	getPlayerByName(lpBattler->m_strName);

			if	(!lpActor)
				continue;

			lpLog->wTeam	=	iTeam;

			strcpy(lpLog->strId,lpActor->m_strId);
			strcpy(lpLog->strName,lpActor->m_strName);

			CClient			*lpClient	=	lpActor->getClient();

			if	(lpClient)
				memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
			else
				strcpy(lpLog->strIP,"invalid IP");

			g_LogQue.addPacket((ALL_LOGMSG *)lpLog);	
		}
	}
}

void
cFIELD::sendEndDuelLog()
{
	int	iWinTeam	=	2;
	
	if	(m_awKillCount[0]	>	m_awKillCount[1])
		iWinTeam	=	0;
	if	(m_awKillCount[1]	>	m_awKillCount[0])
		iWinTeam	=	1;
	
	if	(m_aiActorCount[0]	==	0)
		iWinTeam	=	1;
	if	(m_aiActorCount[1]	==	0)
		iWinTeam	=	0;
	
	int	iVPForWinner			=	GetWinPointForDuel(m_wDuelType);
	int	iDuelPointForWinner		=	GetDuelPointForDuel(m_wDuelType);
	int	iVPForDefeater			=	GetWinPointForDuel(m_wDuelType,TRUE);
	int	iDuelPointForDefeater	=	GetDuelPointForDuel(m_wDuelType,TRUE);

	int	aiVP[2],aiDP[2];

	if	(iWinTeam	==	0)
	{
		aiVP[0]	=	iVPForWinner;
		aiVP[1]	=	iVPForDefeater;
		aiDP[0]	=	iDuelPointForWinner;
		aiDP[1]	=	iDuelPointForDefeater;
	}
	else
	if	(iWinTeam	==	2)
	{
		aiVP[1]	=	iVPForWinner;
		aiVP[0]	=	iVPForDefeater;
		aiDP[1]	=	iDuelPointForWinner;
		aiDP[0]	=	iDuelPointForDefeater;
	}
	else
	{
		aiVP[1]	=	iVPForDefeater;
		aiVP[0]	=	iVPForDefeater;
		aiDP[1]	=	iDuelPointForDefeater;
		aiDP[0]	=	iDuelPointForDefeater;
	}

	LOG_SYSTEM				logSystem;
	LOG_S_RESULT_OF_DUEL	*lpLog	=	(LOG_S_RESULT_OF_DUEL	*)&logSystem;
	
	lpLog->base.set(sizeof(LOG_S_RESULT_OF_DUEL),dLOG_SYSTEM);
	
	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	m_iUniqueSerial;
	lpLog->wWork			=	eSLT_RESULT_OF_DUEL;
	lpLog->iDuelSerial		=	m_iDuelSerial;

	int iTeam;
	
	for (iTeam=0;iTeam<2;iTeam++)
		strcpy(lpLog->astrTeamName[iTeam],m_aDuelTeam[iTeam].m_strTeamName);

	for (iTeam=0;iTeam<2;iTeam++)
	{
		for (int iMember=0;iMember<5;iMember++)
		{
			cDuelBattler	*lpBattler	=	&m_aDuelBattler[iTeam][iMember];
			cACTOR			*lpActor	=	getPlayerByName(lpBattler->m_strName);

			if	(!lpActor)
				continue;

			lpLog->wTeam	=	iTeam;
			strcpy(lpLog->strId,lpActor->m_strId);
			strcpy(lpLog->strName,lpActor->m_strName);

			lpLog->iVP		=	lpActor->m_duelRecord.m_iWeeklyWinPoint;
			lpLog->iDP		=	lpActor->m_duelRecord.m_iDuelPoint;
			lpLog->iGetVP	=	aiVP[iTeam];
			lpLog->iGetDP	=	aiDP[iTeam];
			lpLog->iBattle	=	lpActor->m_duelRecord.m_iBattleCount;
			lpLog->iWin		=	lpActor->m_duelRecord.m_iWinCount;
			lpLog->iDefeat	=	lpActor->m_duelRecord.m_iDefeatCount;
			lpLog->iDraw	=	lpActor->m_duelRecord.m_iDrawCount;
			lpLog->wWinTeam	=	iWinTeam;

			CClient			*lpClient	=	lpActor->getClient();

			if	(lpClient)
				memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
			else
				strcpy(lpLog->strIP,"invalid IP");

			g_LogQue.addPacket((ALL_LOGMSG *)lpLog);	
		}
	}
}

void
cFIELD::updateDuelField()
{
	if	(m_wDuelType	==	0xffff)
		return;

	if	(m_wDuelMatchStep	!=	eDUEL_MATCH_STEP_IDLE)
		m_wDuelMatchStep	=	m_wDuelMatchStep;

	switch(m_wDuelMatchStep)
	{
		case	eDUEL_MATCH_STEP_IDLE	:
			return;

		case	eDUEL_MATCH_STEP_ENTRY	:
			m_wWaitForEnterDuelFieldTime--;

			if	(checkReadyToBeginDuelMatch() || m_wWaitForEnterDuelFieldTime == 0)
			{
				m_wDuelMatchStep		=	eDUEL_MATCH_STEP_READY;
				m_wReadyForDuelCounter	=	c_iWaitTimeToBeginDuel;

				addSendGuildBattleInfo();

				addSendEtcWork(0xffff,eEW_OPERATE_DUEL_MATCH,4,m_wReadyForDuelCounter);
			}
			return;

		case	eDUEL_MATCH_STEP_READY	:
		{
			m_wReadyForDuelCounter--;

			if	(m_aiActorCount[0] == 0 || m_aiActorCount[1] == 0)	//	대기중인데 애들이 나갔다.
			{
				m_wGuildBattleStatus=	eGB_FINISH;
				m_wDuelMatchStep	=	eDUEL_MATCH_STEP_FINISH;
				m_wWaitForExitTime	=	c_iWaitTimeToBanishFromDuelField;
				addSendEtcWork(0xffff,eEW_OPERATE_DUEL_MATCH,7);
				break;
			}

			if	(m_wReadyForDuelCounter	==	0)
			{
				sendBeginDuelLog();
				m_wDuelMatchStep			=	eDUEL_MATCH_STEP_BATTLE;
				m_wGuildBattleStatus		=	eGB_BATTLE;

				m_wDuelBattleTimeCounter	=	GetDuelBattleTime(m_wDuelType);
				m_beginDuelTime.update();
				addSendEtcWork(0xffff,eEW_OPERATE_DUEL_MATCH,0,m_wDuelBattleTimeCounter);
			}

			break;
		}
		case	eDUEL_MATCH_STEP_BATTLE	:
		{
			m_wDuelBattleTimeCounter--;

			if	(m_aiActorCount[0] == 0 || m_aiActorCount[1] == 0)
			{
				operateDuelBattleResult();
				break;
			}

			if	(m_wDuelBattleTimeCounter	%	5	==	1)
				addSendEtcWork(0xffff,eEW_OPERATE_DUEL_MATCH,0,m_wDuelBattleTimeCounter);

			if	(m_wDuelBattleTimeCounter	==	20)
				addSendEtcWork(0xffff,eEW_OPERATE_DUEL_MATCH,1,m_wDuelBattleTimeCounter);

			if	(m_wDuelBattleTimeCounter	==	0)
				operateDuelBattleResult();

			break;
		}

		case	eDUEL_MATCH_STEP_FINISH			:
		{
			m_wWaitForExitTime--;

			if	(m_wWaitForExitTime	==	0)
			{
				m_wDuelMatchStep	=	eDUEL_MATCH_STEP_CLOSING;
				clsoeDuelField();
			}

			break;
		}

		case	eDUEL_MATCH_STEP_CLOSING:
		{
			if	(m_iActorCount	==	0)
			{
				m_wDuelMatchStep	=	eDUEL_MATCH_STEP_IDLE;

				break;
			}

			for (int iTeam=0;iTeam<3;iTeam++)
				for (int iActor=m_aiActorCount[iTeam];iActor;)
				{
					--iActor;

					cACTOR	*lpActor	=	m_alpActor[m_awActorList[iTeam][iActor]];

					if	(lpActor	==	NULL	||	lpActor->m_bf1IsMoveByEndOfGuildBattle)
						continue;

					lpActor->m_bf1IsMoveByEndOfGuildBattle	=	TRUE;
					lpActor->m_bf1IsPlayOnGuildBattle		=	FALSE;

					if	(lpActor->isPlayer())
						g_pMoveFieldUserManager->booking(lpActor,NULL,lpActor->m_wLastVillage,0xffff);
					else
						removeActor(m_awActorList[iTeam][iActor]);
				}

			break;
		}
	}
}