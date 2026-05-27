#include	"operate_packet.h"
#include	"Client.h"
#include	"guild.h"
#include	"cevent.h"
#include	"cSocket.h"
#include	"server.h"
#include	"CGuildBattleManager.h"
#include	"cSRVUTIL.h"

void
nsReceiveFromGS::addGuildGold(SERVERStoWORLD_ADD_GUILD_GOLD* _lpPacket)
{
	cGuild	*lpGuild=	GetGuild(_lpPacket->wGuildSerial);
	
	if	(!lpGuild	||	lpGuild->wSerial==0xffff)	//	그런 길드가 없다.
	{
		sendGSPValidGuild(NULL,_lpPacket->wGuildSerial);
		return;
	}

	if	(_lpPacket->iGold	<	0)
		if	(lpGuild->m_llGuildGold	<	-_lpPacket->iGold)
			return;

	lpGuild->m_llGuildGold	+=	_lpPacket->iGold;

	WORLDtoSERVERS_UPDATE_GUILD_GOLD	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_UPDATE_GUILD_GOLD);
	packet.llGuildGold	=	lpGuild->m_llGuildGold;

	SendDataAllGameServer((char *)&packet,packet.base.wSize);
}

void
sendGSPDuelTeamInfo(CClient *_lpServer)
{
	int	iRankerCount=	0;

	WORLDtoSERVERS_DUEL_TEAM	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_DUEL_TEAM);

	packet.wTotalTeamCount	=	g_duelTeamManager.m_wTotalTeamCount;
	packet.wTeamCount		=	0;	//	주간 랭커 수,예선전 랭커 수

	for (int i=0;i<g_duelTeamManager.m_wTotalTeamCount;i++)
	{
		cDuelTeamInfo	*lpTeam	=	g_duelTeamManager.getTeam(i);

		memcpy(&packet.aTeams[packet.wTeamCount],lpTeam,sizeof(cDuelTeamInfo));
		packet.wTeamCount++;

		if	(packet.wTeamCount	>=	100)
		{
			mSEND_GSP(_lpServer,packet);

			packet.wTeamCount	=	0;
		}
	}

	if	(packet.wTeamCount)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aTeams)+packet.wTeamCount*sizeof(cDuelTeamInfo);

		mSEND_GSP(_lpServer,packet);
	}

}

void
SendChangeGVGServerGuildMasterPacketToLoginServer(char *_lpstrGuildName,char *_lpstrNewMasterID,CClient *_lpLoginServer)
{
	if	(!_lpLoginServer)
		return;

	WORLDtoSERVERS_CHANGE_GVG_SERVER_GUILD_MASTER	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_CHANGE_GVG_SERVER_GUILD_MASTER);
	strcpy(packet.strGuildName,_lpstrGuildName);
	strcpy(packet.strNewMasterID,_lpstrNewMasterID);

	_lpLoginServer->AddSendPacket((char*)&packet,packet.base.wSize);
}

void
sendGSPDuelRankerInfo(CClient *_lpServer,BOOL _bIsDailyRanking)
{
	int	iRankerCount=	0;

	WORLDtoSERVERS_DUEL_RANKER_INFO	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_DUEL_RANKER_INFO);

	packet.wIsDailyRanking		=	_bIsDailyRanking;	//	0 : 주간 랭킹,1 : 일간 랭킹

	packet.wWeeklyRankerCount	=	0;
	packet.wTrialGameRankerCount		=	0;	//	주간 랭커 수,TrialGame 랭커 수

	if	(_bIsDailyRanking)
	{
		packet.wWeeklyRankerCount	=	g_duelRankerManager.getDailyRankerCount();
		packet.wTrialGameRankerCount		=	g_duelRankerManager.getTrialGameRankerCount();

		int	i;

		for (i=0;i<g_duelRankerManager.getDailyRankerCount();i++,iRankerCount++)
			memcpy(&packet.aList[iRankerCount],g_duelRankerManager.getDailyRanker(i),sizeof(cDuelRankingInfo));
		for (i=0;i<g_duelRankerManager.getTrialGameRankerCount();i++,iRankerCount++)
			memcpy(&packet.aList[iRankerCount],g_duelRankerManager.getTrialGameRanker(i),sizeof(cDuelRankingInfo));
	}
	else
	{
		packet.wWeeklyRankerCount	=	g_duelRankerManager.getWeeklyRankerCount();
		int	i;

		for (i=0;i<g_duelRankerManager.getWeeklyRankerCount();i++,iRankerCount++)
			memcpy(&packet.aList[iRankerCount],g_duelRankerManager.getWeeklyRanker(i),sizeof(cDuelRankingInfo));
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aList)+iRankerCount*sizeof(cDuelRankingInfo);

	mSEND_GSP(_lpServer,packet);
}

void
SendUpdateGuildHallInfo(CClient *_lpServer,BOOL _bIsBuildSiegeWarfareSchedule)	//	길드 홀 정보 날리기
{
	WORLDtoSERVERS_UPDATE_GUILD_HALL_INFO	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_UPDATE_GUILD_HALL_INFO);

	int	iHallCount	=	0;
	int	aiHallCountByHallLevel[]	=	{0,0xffff,27,9,3,1};

	for	(int iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
	{
		for (int iHall=0;iHall<aiHallCountByHallLevel[iHallLevel];iHall++)
		{
			cGuildHallInfo	*lpHallInfo	=	&g_guildManager.m_aHallList[iHallLevel][iHall];

			memcpy(&packet.aHallList[iHallCount++],lpHallInfo,sizeof(cGuildHallInfo));
		}
	}

	packet.wCount		=	iHallCount;
	packet.wIsIsBuildSiegeWarfareSchedule	=	_bIsBuildSiegeWarfareSchedule;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aHallList)+sizeof(cGuildHallInfo)*iHallCount;

	mSEND_GSP(_lpServer,packet);
}

void
SendSimpleWork(CClient *_lpServer,int _iWork,
			int _iValue0,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,
			int _iValue6,int _iValue7,int _iValue8,int _iValue9,int _iValue10,int _iValue11)	//	공성전 시작 예고
{
	WORLDtoSERVERS_SIMPLE_WORK	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_SIMPLE_WORK);

	packet.wWork	=	_iWork;
	packet.aiValue[0]=	_iValue0;
	packet.aiValue[1]=	_iValue1;
	packet.aiValue[2]=	_iValue2;
	packet.aiValue[3]=	_iValue3;
	packet.aiValue[4]=	_iValue4;
	packet.aiValue[5]=	_iValue5;
	packet.aiValue[6]=	_iValue6;
	packet.aiValue[7]=	_iValue7;
	packet.aiValue[8]=	_iValue8;
	packet.aiValue[9]=	_iValue9;
	packet.aiValue[10]=	_iValue10;
	packet.aiValue[11]=	_iValue11;

	mSEND_GSP(_lpServer,packet);
}

void
SendNoticeBeginSiegeWarfare(CClient *_lpServer,int _iBattleOrder)	//	공성전 시작 예고
{
	SendSimpleWork(_lpServer,eSWWG_NOTICE_BEGIN_SIEGE_WARFARE,_iBattleOrder);
}

void	
SendBeginSiegeWarfare(CClient *_lpServer,int _iBattleOrder)	//	공성전 시작
{
	SendSimpleWork(_lpServer,eSWWG_BEGIN_SIEGE_WARFARE,_iBattleOrder);
}

void	
SendNoticeEndSiegeWarfare(CClient *_lpServer,int _iBattleOrder)		//	공성전 종료 x분전
{
	SendSimpleWork(_lpServer,eSWWG_NOTICE_END_SIEGE_WARFARE,_iBattleOrder);
}

void	
SendEndSiegeWarfare(CClient *_lpServer,int _iBattleOrder)		//	공성전 종료
{
	SendSimpleWork(_lpServer,eSWWG_END_SIEGE_WARFARE,_iBattleOrder);
}

void
OperateBuildGuildBattleProlog(BOOL _bIsOnlyTestGuild)
{
	g_guildManager.buildGuildHallInfoByEndGuildProlog(TRUE);
}

void
OperateSimpleWork(SERVERStoWORLD_SIMPLE_WORK *_lpPacket,CClient *_lpUser,CClient *_lpLoginServer,cSOCKET *_lpUSerDbc)
{
	if	(_lpPacket->wWork	>=	eSWLW_BEGIN)
	{
		switch(_lpPacket->wWork)
		{
			case	eSWLW_SMS_ALARM_ERROR			:
			{
				_lpUser	=	NULL;
				SendSimpleWork(_lpUser,eSWWG_SMS_ALARM_ERROR);
				break;
			}

			case	eSWLW_UPDATE_CARROT_SHOP_INFO	:
			{
				WU_ASK_CARROTSHOPINFO sPacket;
				sPacket.base.set(sizeof(WU_ASK_CARROTSHOPINFO),dWU_ASK_CARROTSHOPINFO,0xffff);
				sPacket.wIsUpdateNow	=	TRUE;
				
				_lpUSerDbc->SendPacket((char *)&sPacket,sPacket.base.wSize);
				break;
			}
		}
	}
	else
	if	(_lpPacket->wWork	>=	eSWAW_BEGIN)
	{
		switch(_lpPacket->wWork)
		{
			case	eSWAW_CHANGE_TODAY_GUILD_BATTLE_COUNT	:
			{
				g_guildManager.changeTodayGuildBattleCount(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			
				break;
			}

			case	eSWAW_CHANGE_THIS_WEEK_GUILD_BATTLE_COUNT	:
			{
				g_guildManager.changeThisWeekGuildBattleCount(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			
				break;
			}

			case	eSWAW_UPDATE_CARROT_SHOP_INFO				:
			{
				if	(_lpLoginServer)
				{
					WORLDtoSERVERS_SIMPLE_WORK	packet;

					packet.base.set(sizeof(packet),dWORLDtoSERVERS_SIMPLE_WORK);
					packet.wWork	=	eSWWL_UPDATE_CARROT_SHOP_INFO;

					_lpLoginServer->AddSendPacket((char*)&packet,packet.base.wSize);
				}
				break;
			}

			case	eSWAW_ASK_LOTTO_EVENT_DATA					:
			{
				g_lottoEventManager.sendEventData(_lpUser);

				break;
			}
			case	eSWAW_ASK_EVENT_TIME_DATA					:
			{
				g_eventTimeManager.sendEventData(_lpUser);

				break;
			}

			case	eSWAW_RESET_SOUND_OF_LEDERS_BELL_COOLTIME	:
				g_eventManager.resetSoundOfLedersBellTime(_lpPacket->aiValue[0]);
				break;

			case	eSWAW_ASK_SOUND_OF_LEDERS_BELL_EVENT_INFO	:
				g_eventManager.sendSoundOfLeadersBellEventInfo(_lpUser);
				break;
			
			case	eSWAW_CHANGE_GUILD_MARK_TWINKLE_STATUS	:
				g_guildManager.changeGuildMarkTwinkleStatus(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
				break;

			case	eSWAW_CHANGE_GUILD_HALL_LEVEL			:
				g_guildManager.changeGuildHallLevel(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
				break;

			case	eSWAW_CHANGE_GUILD_VILLAGE_INFO			:
				g_guildManager.changeGuildVillageInfo(_lpPacket->aiValue[0] , _lpPacket->aiValue[1], _lpPacket->aiValue[2] , _lpPacket->aiValue[2]
					, _lpPacket->aiValue[3] , _lpPacket->aiValue[4] , _lpPacket->aiValue[5], _lpPacket->aiValue[6]);
				break;
			case	eSWAW_CHANGE_GUILD_MASTER_GRACE			:
				g_guildManager.changeGuildMasterGrace(_lpPacket->aiValue[0] , _lpPacket->aiValue[1]);
				break;
			case	eSWAW_CHANGE_GUILD_INFO_ETC_BY_ADMINTOOL:
				g_guildManager.changeGuildETCInfo(_lpPacket->aiValue[0] , _lpPacket->aiValue[1] , _lpPacket->aiValue[2] );
				break;

			case	eSWAW_CHANGE_GUILD_POINT_BATTLE_COUNT	:
			{
				cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(_lpPacket->aiValue[0]);

				if	(lpGuild)
				{
					lpGuild->m_bf2TryGuildPointBattleCountPerWeek	=	_lpPacket->aiValue[1];

					SendSimpleWork(NULL,eSWWG_CHANGE_TRY_GUILD_POINT_BATTLE_COUNT,_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
				}
				break;
			}
		}
	}
	else
	{
		switch(_lpPacket->wWork)
		{
			case	eSWGW_INCREASE_TRY_GUILD_POINT_BATTLE_COUNT	:
			{
				cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(_lpPacket->aiValue[0]);

				if	(lpGuild)
				{
					lpGuild->m_bf2TryGuildPointBattleCountPerWeek++;
					SendSimpleWork(NULL,eSWWG_CHANGE_TRY_GUILD_POINT_BATTLE_COUNT,_lpPacket->aiValue[0],lpGuild->m_bf2TryGuildPointBattleCountPerWeek);
				}

				break;
			}
			case	eSWGW_INCREASE_RED_STONE_COUNT		:
			{
				g_eventManager.increaseRedStoneCount(_lpPacket->aiValue[0]);
				break;
			}
			case	eSWGW_RECOVER_BONUS_GUILD_POINT_PENELTY		:
			{
				cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(_lpPacket->aiValue[0]);

				if	(lpGuild && lpGuild->m_bf6PeneltyBonusGuildPoint)
				{
					lpGuild->m_wGuildPoint				+=	(WORD)lpGuild->m_bf6PeneltyBonusGuildPoint;
					lpGuild->m_bf6PeneltyBonusGuildPoint=	0;
					SendSimpleWork(NULL,eSWWG_RECOVER_BONUS_GUILD_POINT_PENELTY,_lpPacket->aiValue[0],lpGuild->m_bf6BonusGuildPoint,lpGuild->m_wGuildPoint);
					SaveGuildBaseData(lpGuild);
				}
				break;
			}
			case	eSWGW_INCREASE_BONUS_GUILD_POINT	:
			{
				cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(_lpPacket->aiValue[0]);

				if	(lpGuild && lpGuild->m_bf6BonusGuildPoint < c_iMaxBonusGuildPoint)
				{
					lpGuild->m_bf6BonusGuildPoint++;
					lpGuild->m_wGuildPoint++;
					
					SendSimpleWork(NULL,eSWWG_INCREASE_BONUS_GUILD_POINT,_lpPacket->aiValue[0],lpGuild->m_bf6BonusGuildPoint,lpGuild->m_wGuildPoint);
					SaveGuildBaseData(lpGuild);
				}
				break;
			}
			case	eSWGW_CHANGE_GUILD_HALL_LEVEL		:
			{
				g_guildManager.changeGuildHallLevel(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
				break;
			}
			
			case	eSWGW_RESET_GUILD_PROLOG_POINT		:
			{
				g_guildManager.resetGuildPrologPoint();
				break;
			}
			case	eSWGW_SET_GUILD_PROLOG_POINT		:
			{
				g_guildManager.setGuildPrologPoint(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
				break;
			}
			case	eSWGW_END_GUILD_PROLOG						:
			{
				g_guildManager.endGuildProlog();
				break;
			}
			case	eSWGW_BEGIN_TEST_SIEGE_WARFARE_OF_GUILD	:
			{
				if	(_lpPacket->aiValue[0]	==	0)
					g_bIsTestSiegeWarfareOfGuild	=	2;
				if	(_lpPacket->aiValue[0]	==	1)
					SendNoticeBeginSiegeWarfare(NULL,0xffff);	//	공성전 준비
				if	(_lpPacket->aiValue[0]	==	2)
					SendBeginSiegeWarfare(NULL,0xffff);			//	공성전 시작
				if	(_lpPacket->aiValue[0]	==	3)
					SendNoticeEndSiegeWarfare(NULL,0xffff);		//	공성전 종료 준비
				if	(_lpPacket->aiValue[0]	==	4)
					SendEndSiegeWarfare(NULL,0xffff);			//	공성전 종료
				break;
			}
			case	eSWGW_SET_GUILD_HONOR_POINT					:
			{
				g_guildManager.setGuildHonorPoint(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
				break;
			}
			case	eSWGW_RESET_GUILD_HONOR_POINT					:
			{
				g_guildManager.resetGuildHonorPoint();
				break;
			}
			case	eSWGW_TOGGLE_GUILD_HALL_OPEN_STATUS				:
			{
				g_guildManager.toggleGuildHallOpenStatus(_lpPacket->aiValue[0]);
				break;
			}
			case	eSWGW_ACTIVE_GUILD_MASTER_GRACE					:
			{
				g_guildManager.activeGuildMasterGrace(_lpPacket->aiValue[0]);
				break;
			}
			case	eSWGW_INCREASE_PET_EXP							:
			{
				g_guildManager.increasePetExp(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
				break;
			}

			case	eSWGW_RESET_GUILD_BATTLE_SCHEDULE			:
			{
				g_guildBattleManager.resetTodaySchedule();

				sendGSPGuildBattleSchedule(NULL);

				break;
			}
			
			case	eSWGW_CHANGE_GUILD_PET_EXP							:
			{
				g_guildManager.changePetExp(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
				break;
			}
			case	eSWGW_RENT_GUILD_PET							:
			{
				g_guildManager.rentGuildPet(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
				break;
			}
			case	eSWGW_RETRIEVAL_RENTED_GUILD_PET				:
			{
				g_guildManager.retrievalRentedGuildPet(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
				break;
			}
			case	eSWGW_SIEGE_WARFARE_RESULT_SUCCESS_TO_DEFENSE	:	//	길드전 수성 성공
			{
				g_guildManager.defenseGuildHall(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
				break;
			}
			case	eSWGW_SIEGE_WARFARE_RESULT_SUCCESS_TO_INVADE	:
			{
				g_guildManager.captureGuildHall(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
				break;
			}

			case	eSWAW_CHANGE_GUILD_VILLAGE_INFO			:
				g_guildManager.changeGuildVillageInfo(_lpPacket->aiValue[0] , _lpPacket->aiValue[1], _lpPacket->aiValue[2] , _lpPacket->aiValue[3]
					, _lpPacket->aiValue[4] , _lpPacket->aiValue[5] , _lpPacket->aiValue[6], _lpPacket->aiValue[7]);
				break;
				
			case	eSWGW_CHANGE_SYSTEM_TIME	:
			{
				g_eventManager.changeSystemTime(_lpPacket->aiValue[0] , _lpPacket->aiValue[1], _lpPacket->aiValue[2] , _lpPacket->aiValue[3], _lpPacket->aiValue[4]);
				break;
			}
		}
	}
}

void	SendAskDuelRankingInfoMsg()
{
	WU_ASK_DUEL_RANKING_INFO	sPacket;
	sPacket.base.set(sizeof(WU_ASK_DUEL_RANKING_INFO),dWU_ASK_DUEL_RANKING_INFO,-1);
			
	GetIDDBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
}

void	SendAskDuelTeamInfoMsg()
{
	WU_ASK_DUEL_TEAM_INFO	sPacket;
	sPacket.base.set(sizeof(WU_ASK_DUEL_TEAM_INFO),dWU_ASK_DUEL_TEAM_INFO,-1);
			
	GetIDDBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);
}

void
operateTransAvatarToGVGServerPacket(CClient *_lpLoginServer,CPlayerSaveDataForServerDefine *_lpSaveData,CClient *_lpGS,int _iServerIndex,cSOCKET *_lpDBCSocket)
{
	if	(!_lpLoginServer)
	{
		WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT	packet;

		packet.base.set(sizeof(packet),dWORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT);

		packet.wResult	=	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_LOGINSERVER_NOT_ACTIVE;
		strcpy(packet.strName,_lpSaveData->m_strName);

		mSEND_GSP(_lpGS,packet);

		return;
	}

	if	(_iServerIndex	==	-1)	//	게임 서버에서 받은거다. 로그인 서버로 패킷 전달하자.
	{
// 		_log("[GVG] dSERVERStoWORLD_TRANS_AVATAR_TO_GVG_SERVER from GS\n");
		WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_SERVER	packet;

		packet.base.set(sizeof(packet),dWORLDtoSERVERS_TRANS_AVATAR_TO_GVG_SERVER);

		packet.iWorldIndex	=	g_iWorldServerIndex;

		memcpy(&packet.avatarData,_lpSaveData,sizeof(packet.avatarData));

		_lpLoginServer->AddSendPacket((char*)&packet,packet.base.wSize);

		return;
	}

// 	_log("[GVG] dSERVERStoWORLD_TRANS_AVATAR_TO_GVG_SERVER from LS\n");

	if	(!_lpDBCSocket)
	{
		WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT	packet;

		packet.base.set(sizeof(packet),dWORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT);

		packet.wResult				=	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_DBCSERVER_NOT_ACTIVE;
		packet.wWorldServerIndex	=	g_iWorldServerIndex;

		strcpy(packet.strName,_lpSaveData->m_strName);

		mSEND_GSP(_lpLoginServer,packet);

		return;
	}

	//	DBC에 전송!
	{
		WDMSG_TRANS_AVATAR_TO_GVG	packet;

		packet.base.set(sizeof(packet),dWDMSG_TRANS_AVATAR_TO_GVG);

		memcpy(&packet.avatarData,_lpSaveData,sizeof(packet.avatarData));

		packet.iWorldIndex	=	_iServerIndex;
		_lpDBCSocket->SendPacket((char *)&packet,packet.base.wSize);
	}
}

void
operateTransAvatarToGVGResultPacket(CClient *_lpGameServer,SERVERStoWORLD_TRANS_AVATAR_TO_GVG_RESULT	*_lpPacket)
{
// 	_log("[GVG]receive result from LS [%s->%s]\n",_lpPacket->strName,_lpPacket->strNewName);

	WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT);

	packet.wResult	=	_lpPacket->wResult;
	strcpy(packet.strName,_lpPacket->strName);
	memcpy(packet.strNewName,_lpPacket->strNewName,sizeof(_lpPacket->strNewName));

	mSEND_GSP(_lpGameServer,packet);
}

void
operateTransAvatarToGVGResultDBPacket(CClient *_lpLoginServer,DWMSG_TRANS_AVATAR_TO_GVG_RESULT *_lpPacket)
{
	if	(!_lpLoginServer)	//	어찌 할 수가 엄따
		return;

//	_log("[GVG]receive result from DS\n");

	WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT);
	packet.wResult				=	_lpPacket->wResult;
	packet.wWorldServerIndex	=	_lpPacket->wWorldServerIndex;

	strcpy(packet.strName,_lpPacket->strName);
	memcpy(packet.strNewName,_lpPacket->strNewName,sizeof(packet.strNewName));

	_lpLoginServer->AddSendPacket((char*)&packet,packet.base.wSize);
}

void
operateTransGuildToGVGServerPacket(CClient *_lpLoginServer,SERVERStoWORLD_TRANS_GUILD_TO_GVG_SERVER *_lpPacket,CClient *_lpGS,cSOCKET *_lpDBCSocket)
{
	WORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT	resultPacket;

	resultPacket.base.set(sizeof(resultPacket),dWORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT);
	strcpy(resultPacket.strMasterName,_lpPacket->strMasterName);
	memcpy(resultPacket.strGuildName,_lpPacket->guildData.m_strName,sizeof(_lpPacket->guildData.m_strName));
	resultPacket.wWorldServerIndex	=	_lpPacket->iWorldIndex;

	if	(!_lpLoginServer)
	{
		resultPacket.wResult	=	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_LOGINSERVER_NOT_ACTIVE;

		mSEND_GSP(_lpGS,resultPacket);

		return;
	}

	if	(_lpPacket->iWorldIndex	==	-1)	//	게임 서버에서 받은거다. 로그인 서버로 패킷 전달하자.
	{
		WORLDtoSERVERS_TRANS_GUILD_TO_GVG_SERVER	packet;

		packet.base.set(sizeof(packet),dWORLDtoSERVERS_TRANS_GUILD_TO_GVG_SERVER);

		packet.iWorldIndex	=	g_iWorldServerIndex;
		memcpy(packet.strMasterName,_lpPacket->strMasterName,sizeof(packet.strMasterName));
		strcpy(packet.strMasterId,_lpPacket->strMasterId);

		memcpy(&packet.guildData,&_lpPacket->guildData,sizeof(packet.guildData));

		packet.wMasterLevel	=	_lpPacket->wMasterLevel;
		packet.wMasterJob	=	_lpPacket->wMasterJob;

		mSEND_GSP(_lpLoginServer,packet);

		return;
	}

	cGuildInfoForTransToGVG	*lpOrgGuild	=	&_lpPacket->guildData;
	char	strNewGuildName[dGUILD_NAME_LENGTH];
	char	strNewMasterName[dNAME_LENGTH];

	{
		int		iGuildNameLength	=	strlen(lpOrgGuild->m_strName);

		strcpy(strNewGuildName,lpOrgGuild->m_strName);

		if	(iGuildNameLength >= dGUILD_NAME_LENGTH-2)
		{
			strNewGuildName[iGuildNameLength]	=	'A'+_lpPacket->iWorldIndex;
			strNewGuildName[iGuildNameLength+1]	=	NULL;
		}
		else
		{
			strNewGuildName[iGuildNameLength]	=	'_';
			strNewGuildName[iGuildNameLength+1]	=	'A'+_lpPacket->iWorldIndex;
			strNewGuildName[iGuildNameLength+2]	=	NULL;
		}

		int	iNameLength	=	strlen(_lpPacket->strMasterName);

		strcpy(strNewMasterName,_lpPacket->strMasterName);
		
		if	(iNameLength >= dNAME_LENGTH-2)
		{
			strNewMasterName[iNameLength]	=	'A'+_lpPacket->iWorldIndex;
			strNewMasterName[iNameLength+1]	=	NULL;
		}
		else
		{
			strNewMasterName[iNameLength]	=	'_';
			strNewMasterName[iNameLength+1]	=	'A'+_lpPacket->iWorldIndex;
			strNewMasterName[iNameLength+2]	=	NULL;
		}
	}

	cGuild*	lpGuild	=	g_guildManager.GetGuildPnt(strNewGuildName);

	if	(!lpGuild)				//	길드가 없어?;;
	{
		resultPacket.wResult	=	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_GUILD_NOT_EXIST;	//	길드가 엄따
		mSEND_GSP(_lpLoginServer,resultPacket);
		return;
	}

	if	(lpGuild->m_wLevel > lpOrgGuild->m_wLevel)
	{
		resultPacket.wResult	=	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_MISMATCH_LEVEL;		//	원본 길드의 레벨이 낮다?
		mSEND_GSP(_lpLoginServer,resultPacket);
		return;
	}

	{
		lpGuild->m_wLevel					=	lpOrgGuild->m_wLevel;			//	길드 레벨
		lpGuild->m_wGuildPoint				=	lpOrgGuild->m_wGuildPoint;		//	스킬 포인트
		lpGuild->m_bf1IsTwinkle				=	lpOrgGuild->m_bf1IsTwinkle;		//	길드 마크가 반짝 거린다.
		lpGuild->m_bf6BonusGuildPoint		=	lpOrgGuild->m_bf6BonusGuildPoint;
		lpGuild->m_bf6PeneltyBonusGuildPoint=	lpOrgGuild->m_bf6PeneltyBonusGuildPoint;

		memset(lpGuild->m_awGuildVillage,0xff,sizeof(lpGuild->m_awGuildVillage));

		memcpy(&lpGuild->m_guildMarkInfo,&lpOrgGuild->m_guildMarkInfo,sizeof(lpGuild->m_guildMarkInfo));						//	길드 마크
		memcpy(lpGuild->m_abGuildSkill,lpOrgGuild->m_abGuildSkill,sizeof(lpGuild->m_abGuildSkill));

		SaveGuildBaseData(lpGuild);
		SaveGuildSkill(lpGuild);

		sendGSPUpdateGuildInfo(NULL,lpGuild->m_wSerial,FALSE);
	}

	{
		resultPacket.wResult				=	eTRAN_GUILD_TO_GVG_RESULT_SUCCESS;

		memcpy(resultPacket.strNewMasterName,strNewMasterName,sizeof(strNewMasterName));
		memcpy(resultPacket.strNewGuildName,strNewGuildName,sizeof(strNewGuildName));

		mSEND_GSP(_lpLoginServer,resultPacket);
	}

	if	(stricmp(lpGuild->m_strMasterName,strNewMasterName) != NULL && _lpDBCSocket)
	{
		CGuildMan	*lpNewMaster	=	lpGuild->GetGuilderPntByID(_lpPacket->strMasterId);

		if	(lpNewMaster && stricmp(lpGuild->m_strMasterName,lpNewMaster->m_strName) != NULL )
		{
			WDMSG_CHANGERANK	cRank;

			cRank.base.set(sizeof(WDMSG_CHANGERANK),dWDMSG_CHANGERANK);
			cRank.dwGameServerSerial=	0xffffffff;
			cRank.wIndex			=	lpGuild->m_wSerial;
			cRank.wRank				=	dGUILD_CLASS_MASTER;
			cRank.wType				=	dGUILD_APPONT;

			strcpy(cRank.strName,lpGuild->m_strName);
			strcpy(cRank.strTarget,lpNewMaster->m_strName);
			strcpy(cRank.strMaster,lpGuild->m_strMasterName);

			CTimeInfo	curTime,appointTime;

			appointTime.reset();
			appointTime.m_wYear	=	lpGuild->m_timeBeginGuildMaster.year;
			appointTime.m_wYear	+=	2000;
			appointTime.m_wMonth=	lpGuild->m_timeBeginGuildMaster.month;
			appointTime.m_wDay	=	lpGuild->m_timeBeginGuildMaster.day;

			int		iSpentDay	=	curTime.getDayValue(2000)-appointTime.getDayValue(2000);

			if	(iSpentDay	<	dGUILD_MINLIMITPERIOD_FORCHANGINGELDER	)
				cRank.wRank2=	dGUILD_CLASS_MEMBER;
			else
				cRank.wRank2=	dGUILD_CLASS_ELDER;	//	원로.

			_lpDBCSocket->SendPacket((char *)&cRank,cRank.base.wSize);
		}
	}
}

void
operateTransGuildToGVGResultPacket(CClient *_lpGameServer,SERVERStoWORLD_TRANS_GUILD_TO_GVG_RESULT	*_lpPacket)
{
	if	(_lpGameServer)
	{
		WORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT	packet;

		packet.base.set(sizeof(packet),dWORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT);

		packet.wResult	=	_lpPacket->wResult;
		memcpy(packet.strMasterName,_lpPacket->strMasterName,sizeof(packet.strMasterName));
		memcpy(packet.strNewMasterName,_lpPacket->strNewMasterName,sizeof(packet.strNewMasterName));
		memcpy(packet.strNewGuildName,_lpPacket->strNewGuildName,sizeof(packet.strNewGuildName));

		mSEND_GSP(_lpGameServer,packet);
	}
}

void
operateChangeGVGServerGuildMasterPacket(SERVERStoWORLD_CHANGE_GVG_SERVER_GUILD_MASTER *_lpPacket,cSOCKET *_lpDBCSocket)
{
	if	(!_lpDBCSocket)	//	DB 연결 안된다;;
		return;

	char	strNewGuildName[dGUILD_NAME_LENGTH];

	{
		int		iGuildNameLength	=	strlen(_lpPacket->strGuildName);

		strcpy(strNewGuildName,_lpPacket->strGuildName);

		if	(iGuildNameLength >= dGUILD_NAME_LENGTH-2)
		{
			strNewGuildName[iGuildNameLength]	=	'A'+g_iWorldServerIndex;
			strNewGuildName[iGuildNameLength+1]	=	NULL;
		}
		else
		{
			strNewGuildName[iGuildNameLength]	=	'_';
			strNewGuildName[iGuildNameLength+1]	=	'A'+g_iWorldServerIndex;
			strNewGuildName[iGuildNameLength+2]	=	NULL;
		}
	}

	cGuild*	lpGuild	=	g_guildManager.GetGuildPnt(strNewGuildName);

	if	(!lpGuild)	//	그런 길드가 없다.;;
		return;

	CGuildMan	*lpNewMaster	=	lpGuild->GetGuilderPntByID(_lpPacket->strNewMasterID);

	if	(!lpNewMaster)	//	그런 멤버 없다.. ;;
		return;

	if	(stricmp(lpGuild->m_strMasterName,lpNewMaster->m_strName) == 0)	//	이미 길마다
		return;

	WDMSG_CHANGERANK	cRank;

	cRank.base.set(sizeof(WDMSG_CHANGERANK),dWDMSG_CHANGERANK);
	cRank.dwGameServerSerial=	0xffffffff;
	cRank.wIndex			=	lpGuild->m_wSerial;
	cRank.wRank				=	dGUILD_CLASS_MASTER;
	cRank.wType				=	dGUILD_APPONT;

	strcpy(cRank.strName,lpGuild->m_strName);
	strcpy(cRank.strTarget,lpNewMaster->m_strName);
	strcpy(cRank.strMaster,lpGuild->m_strMasterName);

	CTimeInfo	curTime,appointTime;

	appointTime.reset();
	appointTime.m_wYear	=	lpGuild->m_timeBeginGuildMaster.year;
	appointTime.m_wYear	+=	2000;
	appointTime.m_wMonth=	lpGuild->m_timeBeginGuildMaster.month;
	appointTime.m_wDay	=	lpGuild->m_timeBeginGuildMaster.day;

	int		iSpentDay	=	curTime.getDayValue(2000)-appointTime.getDayValue(2000);

	if	(iSpentDay	<	dGUILD_MINLIMITPERIOD_FORCHANGINGELDER	)
		cRank.wRank2=	dGUILD_CLASS_MEMBER;
	else
		cRank.wRank2=	dGUILD_CLASS_ELDER;	//	원로.

	_lpDBCSocket->SendPacket((char *)&cRank,cRank.base.wSize);
}


void	operateG2W_ASK_GVG_AVATAR_ORG_DATA(CClient *_lpLoginServer,CClient *_lpGameServer,G2W_ASK_GVG_AVATAR_ORG_DATA *_lpPacket)
{
// 	_log("[GVG] receive G2W_ASK_GVG_AVATAR_ORG_DATA\n");

	Avatar_Data	*lpAvatar	=	_lpGameServer->GameClientMagager.GetAvatarPnt(_lpPacket->strName,TRUE);

	if	(!lpAvatar)
	{
		operateL2W_ASK_GVG_AVATAR_ORG_DATA_RESULT(_lpGameServer,NULL,_lpPacket->strName,eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_AVATAR);
		return;
	}

	if	(lpAvatar->bMovingMap)
	{
		operateL2W_ASK_GVG_AVATAR_ORG_DATA_RESULT(_lpGameServer,NULL,_lpPacket->strName,eGET_GVG_AVATAR_ORG_DATA_RESULT_SUCCESS_BY_IS_MOVEFIELD);
		return;
	}

	if	(!_lpLoginServer)
	{
		operateL2W_ASK_GVG_AVATAR_ORG_DATA_RESULT(_lpGameServer,NULL,_lpPacket->strName,eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_LOGINSERVER);

		return;
	}

	W2L_TOSS_ASK_GVG_AVATAR_ORG_DATA	packet;

	packet.base.set(sizeof(packet),dW2L_TOSS_ASK_GVG_AVATAR_ORG_DATA);
	
	packet.wOrgServer			=	_lpPacket->wOrgServer;
	packet.wNotUseUnderBarName	=	_lpPacket->wNotUseUnderBarName;
	
	memcpy(packet.strName,_lpPacket->strName,dNAME_LENGTH);
	memcpy(packet.strId,_lpPacket->strId,dID_LENGTH);

	_lpLoginServer->AddSendPacket((char*)&packet,packet.base.wSize);
}

void	operateL2W_TOSS_ASK_GVG_AVATAR_ORG_DATA(CClient *_lpLoginServer,L2W_TOSS_ASK_GVG_AVATAR_ORG_DATA *_lpPacket)
{
// 	_log("[GVG] receive L2W_TOSS_ASK_GVG_AVATAR_ORG_DATA\n");

	W2D_ASK_GVG_AVATAR_ORG_DATA	packet;

	packet.base.set(sizeof(packet),dW2D_ASK_GVG_AVATAR_ORG_DATA);
	
	memcpy(packet.strName,_lpPacket->strName,dNAME_LENGTH);
	memcpy(packet.strId,_lpPacket->strId,dID_LENGTH);

	packet.wNotUseUnderBarName	=	_lpPacket->wNotUseUnderBarName;

	GetADBSocket()->SendPacket((char*)&packet,packet.base.wSize);
}

void	operateD2W_GVG_AVATAR_ORG_DATA(CClient *_lpLoginServer,D2W_GVG_AVATAR_ORG_DATA *_lpPacket)
{
//	_log("[GVG] receive D2W_GVG_AVATAR_ORG_DATA\n");

	if	(!_lpLoginServer)
		return;

	W2L_GVG_AVATAR_ORG_DATA		packet;

	packet.base.set(sizeof(packet),dW2L_GVG_AVATAR_ORG_DATA);

	memcpy(packet.strName,_lpPacket->strName,dNAME_LENGTH);
	memcpy(&packet.avatarData,&_lpPacket->avatarData,sizeof(packet.avatarData));

	CM_COMPRESS		compPacket;
	WORD			wCompSize	=	cSRVUTIL::MakeCompressPacket(&compPacket,&packet,packet.base.wSize);

	if	(wCompSize == 0)
		_lpLoginServer->AddSendPacket((char *)&packet,packet.base.wSize);
	else
		_lpLoginServer->AddSendPacket((char *)&compPacket,compPacket.wSize);
}

void	operateD2W_SIMPLE_WORK(cSOCKET *_lpUser,CClient *_lpLoginServer,D2W_SIMPLE_WORK* _lpPacket)
{
// 	switch(_lpPacket->wWork)
// 	{
// 	}
}

void	operateD2W_ASK_GVG_AVATAR_ORG_DATA_RESULT(CClient *_lpLoginServer,D2W_ASK_GVG_AVATAR_ORG_DATA_RESULT* _lpPacket)
{
//	_log("[GVG] receive D2W_ASK_GVG_AVATAR_ORG_DATA_RESULT\n");

	if	(!_lpLoginServer)
		return;
	
	W2L_ASK_GVG_AVATAR_ORG_DATA_RESULT	packet;

	packet.base.set(sizeof(packet),dW2L_ASK_GVG_AVATAR_ORG_DATA_RESULT);
	memcpy(packet.strName,_lpPacket->strName,dNAME_LENGTH);
	packet.wResult	=	_lpPacket->wResult;

	_lpLoginServer->AddSendPacket((char*)&packet,packet.base.wSize);
}


void	operateL2W_ASK_GVG_AVATAR_ORG_DATA_RESULT(CClient *_lpGameServer,L2W_ASK_GVG_AVATAR_ORG_DATA_RESULT *_lpPacket,char *_lpstrName,int _iResult)
{
//	_log("[GVG] receive L2W_ASK_GVG_AVATAR_ORG_DATA_RESULT %s\n");

	if	(!_lpGameServer)
		return;
	
	W2G_ASK_GVG_AVATAR_ORG_DATA_RESULT	packet;

	packet.base.set(sizeof(packet),dW2G_ASK_GVG_AVATAR_ORG_DATA_RESULT);

	if	(_lpPacket)
	{
		_lpstrName	=	_lpPacket->strName;
		_iResult	=	_lpPacket->wResult;
	}

	if	(!_lpstrName)
		return;
	
	memcpy(packet.strName,_lpstrName,dNAME_LENGTH);
	packet.wResult	=	_iResult;

	_lpGameServer->AddSendPacket((char*)&packet,packet.base.wSize);
}

void	operateL2W_GVG_AVATAR_ORG_DATA(CClient *_lpGameServer,L2W_GVG_AVATAR_ORG_DATA *_lpPacket)
{
//	_log("[GVG] receive L2W_GVG_AVATAR_ORG_DATA\n");

	if	(!_lpGameServer)
	{
//		_log("[GVG] L2W_GVG_AVATAR_ORG_DATA can not find gs\n");
		return;
	}

	W2G_GVG_AVATAR_ORG_DATA	packet;

	packet.base.set(sizeof(packet),dW2G_GVG_AVATAR_ORG_DATA);

	memcpy(packet.strName,_lpPacket->strName,dNAME_LENGTH);
	memcpy(&packet.avatarData,&_lpPacket->avatarData,sizeof(packet.avatarData));

	CM_COMPRESS		compPacket;
	WORD			wCompSize	=	cSRVUTIL::MakeCompressPacket(&compPacket,&packet,packet.base.wSize);

	if	(wCompSize == 0)
		_lpGameServer->AddSendPacket((char *)&packet,packet.base.wSize);
	else
		_lpGameServer->AddSendPacket((char *)&compPacket,compPacket.wSize);
}