#include	"guild.h"
#include	"operate_packet.h"
#include	"cGuildBattleManager.h"
#include "cPACKET_WORLDSERVER.h"
#include "cPACKET_USERDBSERVER.h"
#include "cPACKET_BCSERVER.h"
#include "cPACKET_DBSERVER.h"
#include "Client.h"
#include "CUtil.h"
#include "math.h"
#include "server.h"
#include "cSRVUTIL.h"

cGuildManagerForWS	g_guildManager;
BOOL				g_bIsTestSiegeWarfareOfGuild	=	FALSE;
cGuild				g_guildForCreate; 

UINT	
g_auiGuildPetExp[]	=	//	길드펫 누적 경험치
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

int
cGuild::getPetLevel(int _iExp)
{
	for(int iLevel=0;iLevel<100;iLevel++)
	{
		if	(_iExp	>=	(int)g_auiGuildPetExp[iLevel]	&&	_iExp	<	(int)g_auiGuildPetExp[iLevel+1])
			return	iLevel+1;
	}

	return	100;
}


BOOL
cGuildForWS::checkLastGVGBattleDate()
{
	if	(m_bf6LastGVGBattleYear == 0 || m_bf6LastGVGBattleYear+2000 > g_currentTime.m_wYear)
	{
		m_bf6LastGVGBattleYear	=	g_currentTime.m_wYear-2000;
		m_bf4LastGVGBattleMonth	=	g_currentTime.m_wMonth;
		m_bf5LastGVGBattleDay	=	g_currentTime.m_wDay;
	}

	if	(g_iWorldType	!=	eSERVER_TYPE_GVG)
		return	TRUE;

	//	만약 일정기간동안 해당 길드의 어떤 길드원도 들어오지 않는다면 이 길드는 사라진다.
	int iCheckday	=	cSRVUTIL::CalcDayOnToday(m_bf6LastGVGBattleYear+2000,m_bf4LastGVGBattleMonth,m_bf5LastGVGBattleDay);

	if	(iCheckday	<=	dTIME_TO_REMOVE_GUILD_BY_NOT_GVG_BATTLE)
		return	TRUE;

	_log("DORMANCY GUILD DELETE : [%d:%s, master (%s) , lastconnected date : %d/%d/%d - %d] ", m_wSerial,m_strName,m_strMasterName,m_timeLastConnectTime.year,m_timeLastConnectTime.month,m_timeLastConnectTime.day,iCheckday);

	DeleteGuildPacket(0xffff,m_wSerial,m_strName,"", eDGR_NOT_GVG_BATTLE);
	DeleteGuild(m_wSerial,m_strName,"Can not found Next Master.");

	return	FALSE;
}

BOOL
cGuildForWS::checkGuildMemberLastConnectDate()
{
	if	(m_timeLastConnectTime.m_dwValue == 0 || m_timeLastConnectTime.year+2000 > g_currentTime.m_wYear)
	{
		m_timeLastConnectTime.year	=	g_currentTime.m_wYear-2000;
		m_timeLastConnectTime.month	=	g_currentTime.m_wMonth;
		m_timeLastConnectTime.day	=	g_currentTime.m_wDay;
	}

	//	만약 일정기간동안 해당 길드의 어떤 길드원도 들어오지 않는다면 이 길드는 사라진다.
	int iCheckday = cSRVUTIL::CalcDayOnToday(m_timeLastConnectTime.year+2000,m_timeLastConnectTime.month,m_timeLastConnectTime.day);

	if	(iCheckday <= dGUILD_DORMANCYLIMITPERIOD_FORGUILD)
		return	TRUE;

	_log("DORMANCY GUILD DELETE : [%d:%s, master (%s) , lastconnected date : %d/%d/%d - %d] ", m_wSerial,m_strName,m_strMasterName,m_timeLastConnectTime.year,m_timeLastConnectTime.month,m_timeLastConnectTime.day,iCheckday);

	DeleteGuildPacket(0xffff,m_wSerial,m_strName,"", eDGR_TIME_OVER);
	DeleteGuild(m_wSerial,m_strName,"Time Over");

	return	FALSE;
}

BOOL
cGuildForWS::checkGuildMasterLastConnectDate()
{
	if	(m_timeLastConnectedMasterTime.m_dwValue == 0 || m_timeLastConnectedMasterTime.year+2000 > g_currentTime.m_wYear)
	{
		m_timeLastConnectedMasterTime.year	=	g_currentTime.m_wYear-2000;
		m_timeLastConnectedMasterTime.month	=	g_currentTime.m_wMonth;
		m_timeLastConnectedMasterTime.day	=	g_currentTime.m_wDay;
	}

	int	iCheckday = cSRVUTIL::CalcDayOnToday(m_timeLastConnectedMasterTime.year+2000,
										m_timeLastConnectedMasterTime.month,
										m_timeLastConnectedMasterTime.day);

	if	(iCheckday <= dGUILD_DORMANCYLIMITPERIOD_FORGUILDMASTER)
		return	TRUE;

	_log("DORMANCY GUILD MASTER DELETE : [%d:%s, master (%s) , lastconnected date : %d/%d/%d - %d] ", m_wSerial,m_strName,m_strMasterName,m_timeLastConnectedMasterTime.year,m_timeLastConnectedMasterTime.month,m_timeLastConnectedMasterTime.day,iCheckday);

	CGuildMan * _pNextMaster = NULL;

	if	(GetCount()==1 || !(_pNextMaster = GetNextMaster()) )	//	이 길드는 삭제한다.
	{
		_log("CHECK POINT : Can not found Next Master. And this guild is delete!!");
		DeleteGuildPacket(0xffff,m_wSerial,m_strName,"", eDGR_TIME_OVER);
		DeleteGuild(m_wSerial,m_strName,"Can not found Next Master.");
	}
	else
	{
		//	DB에 랭크 변경을 요청한다.
		WDMSG_CHANGERANK	sPacket;
		sPacket.base.set(sizeof(WDMSG_CHANGERANK),dWDMSG_CHANGERANK);
		sPacket.dwGameServerSerial = 0xffff;
		sPacket.wIndex = m_wSerial;
		strcpy(sPacket.strName,m_strName);
		strcpy(sPacket.strMaster,m_strMasterName);
		strcpy(sPacket.strTarget,_pNextMaster->m_strName);
		sPacket.wRank = dGUILD_CLASS_MASTER;
		sPacket.wReason = eCGR_TIME_OVER;
		//	현재 마스터의 랭크를 아래와 같이 수정하자.

		if	(cSRVUTIL::CalcDayOnToday(m_timeBeginGuildMaster.year+2000,m_timeBeginGuildMaster.month,m_timeBeginGuildMaster.day) < dGUILD_MINLIMITPERIOD_FORCHANGINGELDER)
			sPacket.wRank2 = dGUILD_CLASS_MEMBER;	//	일반 맴버
		else
			sPacket.wRank2 = dGUILD_CLASS_ELDER;	//	원로.

		sPacket.wType = dGUILD_APPONT;
		GetADBSocket()->SendPacket((char *)&sPacket,sPacket.base.wSize);

		//	변경 내용은 중간중간 갱신되는 패킷을 통해 해결하자.
		int tomorrow = cSRVUTIL::GetNextTime((m_timeLastConnectedMasterTime.year+2000)*10000 + m_timeLastConnectedMasterTime.month*100 + m_timeLastConnectedMasterTime.day);

		m_timeLastConnectedMasterTime.year = (tomorrow /10000)%100;
		m_timeLastConnectedMasterTime.month = (tomorrow %10000)/100;
		m_timeLastConnectedMasterTime.day = tomorrow %100;

		SaveGuildBaseData(this);
	}
	
	return	FALSE;
}

BOOL
cGuildForWS::operate1MinuteWork()
{
	if	(!checkLastGVGBattleDate())
		return	FALSE;

	if	(!checkGuildMemberLastConnectDate())
		return	FALSE;

	if	(!checkGuildMasterLastConnectDate())
		return	FALSE;

	return	TRUE;
}

void
cGuildForWS::sendGuildBattleScore()
{
	SendSimpleWork(NULL,eSWWG_UPDATE_GUILD_BATTLE_SCORE,m_wSerial,
		m_iVictoryPoint,m_wVictoryContinue,m_wVictoryCount,m_wDefeatCount,m_wDrawCount,m_wCancelCount,
		m_bf3ThisDayGuildBattleCount,m_bf5ThisWeekGuildBattleCount);
}

cGuildHallInfo*
cGuildManager::getHallByGuild(cGuild *_lpGuild)
{
	if	(!_lpGuild)
		return	NULL;

	int		aiHallCountByHallLevel[]		=	{0,0xffff,27,9,3,1};

	for	(int i=2;i<=c_iMaxGuildHallLevel;i++)
		for	(int j=0;j<aiHallCountByHallLevel[i];j++)
		{
			cGuildHallInfo	*lpHallInfo	=	&m_aHallList[i][j];

			if	(lpHallInfo->m_wOwnGuild==	0xffff)
				continue;

			if	(lpHallInfo->m_wOwnGuild!=	_lpGuild->m_wSerial)
				continue;

			return	lpHallInfo;
		}

	return	NULL;
}

//
//	길드홀 정보에 문제가 없나 확인한다.
void
cGuildManager::checkHallInfoByGuild()
{
	BOOL	bLoop	=	TRUE;

	int		aiHallCountByHallLevel[]	=	{0,0xffff,27,9,3,1};
	int		aiBattleFieldSerial[6]		=	{0,0,912,932,939,941};

	while(bLoop)
	{
		bLoop	=	FALSE;

		for	(int i=2;i<=c_iMaxGuildHallLevel;i++)
		{
			int	iBattleCount	=	0;

			for	(int j=0;j<aiHallCountByHallLevel[i];j++)
			{
				cGuildHallInfo	*lpHallInfo	=	&m_aHallList[i][j];

				lpHallInfo->m_bf12BattleField	=	0xffff;

				if	(lpHallInfo->m_wOwnGuild==	0xffff)
					continue;

				lpHallInfo->m_bf5Slot			=	j;

				cGuild	*lpGuild		=	GetGuildPnt(lpHallInfo->m_wOwnGuild);

				if	(!lpGuild	||	lpGuild->m_wHallLevel	!=	lpHallInfo->m_bf3HallLevel)
				{
					rebuildHallInfoByRemoveGuild(lpHallInfo->m_wOwnGuild);
					bLoop	=	TRUE;
				}

				if	(lpHallInfo->m_wOwnGuild	!=	0xffff	&&	lpHallInfo->m_wInvadeGuild	!=	0xffff)
				{
					lpHallInfo->m_bf12BattleField	=	aiBattleFieldSerial[i]+iBattleCount;
					iBattleCount++;
				}
			}
		}
	}

	for	(int iGuild=0;iGuild<dGUILD_MAX_COUNT;iGuild++)
	{
		cGuild	*lpGuild	=	GetGuildPnt(iGuild);

		if	(!lpGuild	||	lpGuild->m_wHallLevel	<	2)
			continue;

		DWORD			dwHashCode	=	GetHashCode(lpGuild->m_strName);
		cGuildHallInfo	*lpHallInfo	=	getHallByGuild(lpGuild);

		if	(!lpHallInfo)
		{
			_log("not match guild hall level : guild '%s' level %d -> level %d",lpGuild->m_strName,lpGuild->m_wHallLevel,1);

			lpGuild->m_wHallLevel		=	1;
			lpGuild->m_wGuildHallShape	=	0;

			sendGSPUpdateGuildInfo(NULL,lpGuild->wSerial,FALSE);
			SaveGuildBaseData(lpGuild);
		}
		else
		if	(lpHallInfo->m_bf3HallLevel	!=	lpGuild->m_wHallLevel)
		{
			_log("not match guild hall level : guild '%s' level %d -> level %d",lpGuild->m_strName,lpGuild->m_wHallLevel,lpHallInfo->m_bf3HallLevel);

			lpGuild->m_wHallLevel		=	lpHallInfo->m_bf3HallLevel;
			lpGuild->m_wGuildHallShape	=	0;

			sendGSPUpdateGuildInfo(NULL,lpGuild->wSerial,FALSE);
			SaveGuildBaseData(lpGuild);
		}
		else
		if	(lpHallInfo->m_dwHashCode	!=	dwHashCode)	//	시리얼은 같은데 이름 해쉬 코드가 다르다. 지워졌다는 얘기지
		{
			_log("not match guild hall info : guild '%s' level %d -> level %d",lpGuild->m_strName,lpGuild->m_wHallLevel,0);

			lpGuild->m_wHallLevel		=	0;
			lpGuild->m_wGuildHallShape	=	0;

			rebuildHallInfoByRemoveGuild(lpHallInfo->m_wOwnGuild);
			SaveGuildBaseData(lpGuild);
			sendGSPUpdateGuildInfo(NULL,lpGuild->wSerial,FALSE);
		}
	}
}

//	프롤로그가 끝났다.
void
cGuildManager::buildGuildHallInfoByEndGuildProlog(BOOL _bIsOnlyTestGuild)
{
	int				iGuild,iGuildCount=0;
	cIndexForQSort	aGuildList[dGUILD_MAX_COUNT];
	int				aiChallengeGuildCount[]	=	{0,20,7,2,1,0};

	resetGuildHallInfo();

	for	(iGuild	=	dGUILD_MAX_COUNT;iGuild;)
	{
		--iGuild;

		cGuild	*lpGuild	=	GetGuildPnt(iGuild);

		if	(!lpGuild	|| lpGuild->wSerial==0xffff)
			continue;

		if	(lpGuild->m_wHallLevel	!=	1)
			lpGuild->m_wHallLevel						=	0;

//		lpGuild->m_bf3GuildDungeonPlayTime				=	0;
		lpGuild->m_wGuildHallShape						=	0;
		lpGuild->m_iHonorPoint							=	0;
		lpGuild->m_bf2TryGuildPointBattleCountPerWeek	=	0;
		lpGuild->m_bf1IsUseGuildMasterGrace				=	0;

		if	(lpGuild->m_iVictoryPoint		==	0)
			continue;

		aGuildList[iGuildCount].m_iIndex	=	iGuild;
		aGuildList[iGuildCount].m_iValue	=	lpGuild->m_iVictoryPoint;
		iGuildCount++;
	}

	qsort((void *)aGuildList,iGuildCount,sizeof(cIndexForQSort),(int(*)(const void *,const void *))DesCompare);

	int iOrder;

	for	(iOrder=0;iOrder<40 && iOrder<iGuildCount;iOrder++)
	{
		cIndexForQSort	*lpIndex	=	&aGuildList[iOrder];
		cGuild			*lpGuild	=	GetGuildPnt(lpIndex->m_iIndex);

		lpGuild->m_wGuildHallShape		=	0;
		lpGuild->m_bf1IsPermitVisitor	=	TRUE;

		if	(iOrder	<=	0)
			lpGuild->m_wHallLevel	=	5;
		else
		if	(iOrder	<=	3)
			lpGuild->m_wHallLevel	=	4;
		else
		if	(iOrder	<=	12)
			lpGuild->m_wHallLevel	=	3;
		else
			lpGuild->m_wHallLevel	=	2;
		
		sendGSPUpdateGuildInfo(NULL,lpGuild->wSerial,FALSE);
		SaveGuildBaseData(lpGuild);
	}

	buildHallInfoByGuild();
}

//	수성 성공
void
cGuildManager::defenseGuildHall(int _iHallLevel,int _iHallSlot)
{
	cGuildHallInfo	*lpHallInfo		=	&m_aHallList[_iHallLevel][_iHallSlot];

	cGuild			*lpInvadeGuild	=	GetGuildPnt(lpHallInfo->m_wInvadeGuild);
	cGuild			*lpDefenseGuild	=	GetGuildPnt(lpHallInfo->m_wOwnGuild);

	{
		CTimeInfo	currentTime;

		currentTime.update();

		cFILE	file;

		if	(file.Open(_ms("%s/data/%s_siege_warfare_result%d_%.2d_%.2d.txt",
			g_strLogFolder,strWorldName,currentTime.m_wYear,currentTime.m_wMonth,currentTime.m_wDay),"at+"))
		{
			char	strInvadeGuild[64],strDefenseGuild[64];

			strcpy(strInvadeGuild,"valid");
			strcpy(strDefenseGuild,"valid");

			if	(lpInvadeGuild)
				strcpy(strInvadeGuild,lpInvadeGuild->m_strName);
			if	(lpDefenseGuild)
				strcpy(strDefenseGuild,lpDefenseGuild->m_strName);

			file.writeStream(">> Hall Level %d : Defense [%s] , Attack [%s]\n",_iHallLevel,strDefenseGuild,strInvadeGuild);
			file.writeStream("   [%s] guild success to defense!!\n\n",strDefenseGuild);
			file.Close();
		}
	}

	lpHallInfo->m_wInvadeGuild	=	0xffff;
	g_guildBattleManager.save();

	SendSimpleWork(NULL,eSWWG_SIEGE_WARFARE_RESULT_SUCCESS_TO_DEFENSE,_iHallLevel,_iHallSlot);
}

void
cGuildManager::captureGuildHall(int _iHallLevel,int _iHallSlot)
{
	cGuildHallInfo	*lpHallInfo		=	&m_aHallList[_iHallLevel][_iHallSlot];

	cGuild			*lpInvadeGuild	=	GetGuildPnt(lpHallInfo->m_wInvadeGuild);
	cGuild			*lpDefenseGuild	=	GetGuildPnt(lpHallInfo->m_wOwnGuild);

	lpHallInfo->m_wInvadeGuild		=	0xffff;

	SendSimpleWork(NULL,eSWWG_SIEGE_WARFARE_RESULT_SUCCESS_TO_INVADE,_iHallLevel,_iHallSlot);

	if	(!lpInvadeGuild)
	{
		g_guildBattleManager.save();
		return;
	}

	{
		CTimeInfo	currentTime;

		currentTime.update();

		cFILE	file;

		if	(file.Open(_ms("%s/data/%s_siege_warfare_result%d_%.2d_%.2d.txt",
			g_strLogFolder,strWorldName,currentTime.m_wYear,currentTime.m_wMonth,currentTime.m_wDay),"at+"))
		{
			char	strInvadeGuild[64],strDefenseGuild[64];

			strcpy(strInvadeGuild,"valid");
			strcpy(strDefenseGuild,"valid");

			if	(lpInvadeGuild)
				strcpy(strInvadeGuild,lpInvadeGuild->m_strName);
			if	(lpDefenseGuild)
				strcpy(strDefenseGuild,lpDefenseGuild->m_strName);

			file.writeStream(">> Hall Level %d : Defense [%s] , Attack [%s]\n",_iHallLevel,strDefenseGuild,strInvadeGuild);
			file.writeStream("   [%s] guild win!! guild hall change\n\n",strInvadeGuild);
			file.Close();
		}
	}
	
	cGuildHallInfo	*lpLowHall			=	getHallByGuild(lpInvadeGuild);

	//	점령 길드 처리
	{
		lpInvadeGuild->m_wGuildHallShape=	0;
		lpInvadeGuild->m_wHallLevel		=	lpHallInfo->m_bf3HallLevel;

		lpHallInfo->m_wOwnGuild			=	lpInvadeGuild->m_wSerial;
		lpHallInfo->m_dwHashCode		=	GetHashCode(lpInvadeGuild->m_strName);
		lpHallInfo->m_bf1IsOpen			=	lpInvadeGuild->m_bf1IsPermitVisitor;	//	열려 있냐?
		lpHallInfo->m_bf3HallShape		=	0;	//	홀 모양

		SaveGuildBaseData(lpInvadeGuild);
	}

	//	패배 길드 처리
	if	(lpDefenseGuild)
	{
		lpDefenseGuild->m_wHallLevel	=	(WORD)lpHallInfo->m_bf3HallLevel-1;
		lpDefenseGuild->m_wGuildHallShape=	0;

		if	(lpLowHall)
		{
			lpLowHall->m_wOwnGuild	=	lpDefenseGuild->m_wSerial;
			lpLowHall->m_dwHashCode	=	GetHashCode(lpDefenseGuild->m_strName);
			lpLowHall->m_bf1IsOpen	=	lpDefenseGuild->m_bf1IsPermitVisitor;	//	열려 있냐?
			lpLowHall->m_bf3HallShape	=	0;	//	홀 모양
		}

		SaveGuildBaseData(lpDefenseGuild);
	}
	else
	{
		if	(lpLowHall)
		{
			lpLowHall->m_wOwnGuild			=	0xffff;
			lpLowHall->m_dwHashCode			=	0xffffffff;
			lpLowHall->m_bf1IsOpen			=	TRUE;
			lpLowHall->m_bf3HallShape		=	0;
			lpLowHall->m_wInvadeGuild		=	0xffff;
		}
	}

	g_guildBattleManager.save();
}

//
//	길드홀의 필드 시리얼 리턴
int
cGuildManager::getGuildHallFieldSerial(int _iHallLevel,int _iHallSlot,int _iHallShape)
{
	int	l_aiGuildHallSerial[10]	=
	{
		0,
		942,	//	1레벨
		943,	//	2레벨
		970,	//	3레벨
		979,	//	4레벨
		982,	//	5레벨
	};

	int	iFieldSerial;

	if	(_iHallLevel	==	1)
	{
		iFieldSerial	=	l_aiGuildHallSerial[_iHallLevel]+_iHallShape;
	}
	else
	{
		iFieldSerial	=	l_aiGuildHallSerial[_iHallLevel]+_iHallSlot;
	}

	return	iFieldSerial;
}

//	길드홀 정보 체크섬
DWORD
cGuildManager::getGuildHallCheckSum()
{
	DWORD	dwCheckSum	=	0;
	int		aiHallCountByHallLevel[]		=	{0,0xffff,27,9,3,1};

	for	(int i=2;i<=c_iMaxGuildHallLevel;i++)
		for	(int j=0;j<aiHallCountByHallLevel[i];j++)
		{
			cGuildHallInfo	*lpHallInfo	=	&m_aHallList[i][j];

			if	(lpHallInfo->m_wOwnGuild==	0xffff)
				continue;

			DWORD	dwCheckValue;
			cCRC32::BufferToCrc32(lpHallInfo,sizeof(cGuildHallInfo),dwCheckValue);

			dwCheckSum	+=	dwCheckValue;
		}

	return	dwCheckSum;
}

//	길드 펫 대여
void
cGuildManager::rentGuildPet(int _iGuild,int _iPetType,int _iMember)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);

	if	(!lpGuild)
		return;

	CGuildMan	*lpMember=	lpGuild->getMember(_iMember);

	if	(!lpMember)
		return;

	lpMember->m_bf3ReservePet	=	_iPetType;

	SendSimpleWork(NULL,eSWWG_RENT_GUILD_PET,_iGuild,_iPetType,_iMember);

	SendGuildMemberUpdateInfoToBCS(lpGuild,lpMember);
}

void
cGuildManager::setGuildHonorPoint(int _iGuild,int _iPoint)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);

	if	(!lpGuild)
		return;

	lpGuild->m_iHonorPoint	=	_iPoint;

	{
		SG_ETC_WORK	packet;

		packet.base.set(sizeof(packet),dSG_ETC_WORK);

		packet.wWork		=	eEW_RECORD_GUILD_HONOR_POINT;
		packet.aiValue[0]	=	_iPoint>>16;
		packet.aiValue[1]	=	_iPoint&0xffff;

		SendGuildPacketToBCS(&packet,lpGuild->m_wSerial);
	}

	WORLDtoSERVERS_RECORD_GUILD_HONOR_POINT	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_RECORD_GUILD_HONOR_POINT);

	packet.iGuild			=	_iGuild;
	packet.iGuildHonorPoint	=	_iPoint;
	packet.bf1IsVictoryPoint=	FALSE;
	packet.bf2TryGuildPointBattleCountPerWeek	=	lpGuild->m_bf2TryGuildPointBattleCountPerWeek;

	SendDataAllGameServer((char *)&packet,packet.base.wSize);
}

void
cGuildManager::endGuildProlog()
{
	buildGuildHallInfoByEndGuildProlog();
}

void
cGuildManager::resetGuildHonorPoint()
{
	for	(int	iGuild	=	dGUILD_MAX_COUNT;iGuild;)
	{
		--iGuild;

		cGuild	*lpGuild	=	GetGuildPnt(iGuild);

		if	(!lpGuild	|| lpGuild->wSerial==0xffff)
			continue;

		lpGuild->m_iHonorPoint	=	0;
	}

	SendSimpleWork(NULL,eSWWG_RESET_GUILD_HONOR_POINT);
}

//	길드 프롤로그 포인트 초기화
void
cGuildManager::resetGuildPrologPoint()
{
	for	(int	iGuild	=	dGUILD_MAX_COUNT;iGuild;)
	{
		--iGuild;

		cGuild	*lpGuild	=	GetGuildPnt(iGuild);

		if	(!lpGuild	|| lpGuild->wSerial==0xffff)
			continue;

		lpGuild->m_iVictoryPoint	=	0;
	}

	SendSimpleWork(NULL,eSWWG_RESET_GUILD_PROLOG_POINT);
}

void
cGuildManager::setGuildPrologPoint(int _iGuild,int _iPoint)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);

	if	(!lpGuild)
		return;

	lpGuild->m_iVictoryPoint	=	_iPoint;

	WORLDtoSERVERS_RECORD_GUILD_HONOR_POINT	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_RECORD_GUILD_HONOR_POINT);

	packet.iGuild			=	_iGuild;
	packet.iGuildHonorPoint	=	_iPoint;
	packet.bf1IsVictoryPoint=	TRUE;
	packet.bf2TryGuildPointBattleCountPerWeek	=	lpGuild->m_bf2TryGuildPointBattleCountPerWeek;

	SendDataAllGameServer((char *)&packet,packet.base.wSize);
}

void
cGuildManager::toggleGuildHallOpenStatus(int _iGuild)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);

	if	(!lpGuild)
		return;

	lpGuild->m_bf1IsPermitVisitor	=	1-lpGuild->m_bf1IsPermitVisitor;

	SendSimpleWork(NULL,eSWWG_CHANGE_GUILD_HALL_OPEN_STATUS,_iGuild,lpGuild->m_bf1IsPermitVisitor);
}

void
cGuildManager::activeGuildMasterGrace(int _iGuild)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);
	
	if	(!lpGuild)
		return;

	if(lpGuild->m_wHallLevel < 4)				//4렙 이상인 길드홀을 소유한 길드만 능력을 쓸수 잇다.
		return;
	
	if(lpGuild->m_bf1IsUseGuildMasterGrace)		//한주에 한번만...
		return;
	
	//길드 마스터 은총 로그를 남긴다. 로그기록
	lpGuild->m_bf1IsUseGuildMasterGrace = TRUE;

	SendSimpleWork(NULL,eSWGW_ACTIVE_GUILD_MASTER_GRACE,_iGuild , TRUE);
}

void
cGuildManager::increasePetExp(int _iGuild,int _iPetType,int _iExp)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);

	if	(!lpGuild)
		return;

	DWORD	dwPetExp,wExpDuringWeek;

	if	(_iPetType	>=	dGUILD_PET_TYPE_COUNT)
	{
		_iPetType	-=	dGUILD_PET_TYPE_COUNT;

		int	iExpDuringWeek		=	min(lpGuild->m_aGuildGuardian[_iPetType].m_wExpDuringWeek+_iExp,0xffff);
		lpGuild->m_aGuildGuardian[_iPetType].m_wExpDuringWeek=	iExpDuringWeek;
		lpGuild->m_aGuildGuardian[_iPetType].m_dwExperience	+=	_iExp;
		dwPetExp		=	lpGuild->m_aGuildGuardian[_iPetType].m_dwExperience;
		wExpDuringWeek	=	iExpDuringWeek;	

		_iPetType	+=	dGUILD_PET_TYPE_COUNT;
		SaveGuildGuardian(lpGuild);
	}
	else
	{
		int	iExpDuringWeek		=	min(lpGuild->m_aGuildPet[_iPetType].m_wExpDuringWeek+_iExp,0xffff);
		lpGuild->m_aGuildPet[_iPetType].m_wExpDuringWeek=	iExpDuringWeek;
		lpGuild->m_aGuildPet[_iPetType].m_dwExperience	+=	_iExp;
		dwPetExp		=	lpGuild->m_aGuildPet[_iPetType].m_dwExperience;
		wExpDuringWeek	=	iExpDuringWeek;	

		SaveGuildPet(lpGuild);
	}

	WORD	wExp1	=	(WORD)(dwPetExp>>16);
	WORD	wExp2	=	(WORD)(dwPetExp&0xffff);

	SendSimpleWork(NULL,eSWWG_UPDATE_GUILD_PET_EXP,_iGuild,_iPetType,wExp1,wExp2,wExpDuringWeek);
}
void
cGuildManager::changePetExp(int _iGuild,int _iPetType,int _iExp)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);

	if	(!lpGuild)
		return;

	DWORD	dwPetExp,wExpDuringWeek;

	if	(_iPetType	>=	dGUILD_PET_TYPE_COUNT)
	{
		_iPetType	-=	dGUILD_PET_TYPE_COUNT;

		lpGuild->m_aGuildGuardian[_iPetType].m_wExpDuringWeek	=	_iExp;
		lpGuild->m_aGuildGuardian[_iPetType].m_dwExperience		=	_iExp;
		dwPetExp		=	lpGuild->m_aGuildGuardian[_iPetType].m_dwExperience;
		wExpDuringWeek	=	_iExp;	

		_iPetType	+=	dGUILD_PET_TYPE_COUNT;
		SaveGuildGuardian(lpGuild);
	}
	else
	{

		lpGuild->m_aGuildPet[_iPetType].m_wExpDuringWeek=	_iExp;
		lpGuild->m_aGuildPet[_iPetType].m_dwExperience	=	_iExp;
		dwPetExp		=	lpGuild->m_aGuildPet[_iPetType].m_dwExperience;
		wExpDuringWeek	=	_iExp;	

		SaveGuildPet(lpGuild);
	}

	WORD	wExp1	=	(WORD)(dwPetExp>>16);
	WORD	wExp2	=	(WORD)(dwPetExp&0xffff);

	SendSimpleWork(NULL,eSWWG_UPDATE_GUILD_PET_EXP,_iGuild,_iPetType,wExp1,wExp2,wExpDuringWeek);
}

//	길드 펫 회수
void
cGuildManager::retrievalRentedGuildPet(int _iGuild,int _iMember)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);

	if	(!lpGuild)
		return;

	CGuildMan	*lpMember=	lpGuild->getMember(_iMember);

	if	(!lpMember)
		return;

	lpMember->m_bf3ReservePet	=	0;

	SendSimpleWork(NULL,eSWWG_RETRIEVAL_RENTED_GUILD_PET,_iGuild,_iMember);

	SendGuildMemberUpdateInfoToBCS(lpGuild,lpMember);
}

void
cGuildManager::decreasePetAndGuardianExp()
{
	for(int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(i);

		if	(!lpGuild	|| lpGuild->wSerial==0xffff)
			continue;

		BOOL	bIsLevelDown	=	FALSE;

		for	(int iPet=0;iPet<dGUILD_PET_TYPE_COUNT;iPet++)
		{
			int	iLevel			=	lpGuild->getPetLevel(lpGuild->m_aGuildPet[iPet].m_dwExperience);
			int	iOldExp			=	lpGuild->m_aGuildPet[iPet].m_dwExperience-lpGuild->m_aGuildPet[iPet].m_wExpDuringWeek;
			int	iOldLevel		=	lpGuild->getPetLevel(iOldExp);
			int	iExpDuringWeek	=	lpGuild->m_aGuildPet[iPet].m_wExpDuringWeek;
			int	iMinExp			=	iOldLevel*c_iDownGuildPetGuardianLevelFactor;

			lpGuild->m_aGuildPet[iPet].m_wExpDuringWeek	=	0;

			if	(iOldLevel	<	c_iMinLevelForCheckUpkeepGuildPetAndGuardianLevel)
				continue;
			if	(iExpDuringWeek	>=	iMinExp)
				continue;

			lpGuild->m_aGuildPet[iPet].m_dwExperience			=	g_auiGuildPetExp[iLevel-1];

			WORD	wExp1	=	(WORD)(g_auiGuildPetExp[iLevel-1]>>16);
			WORD	wExp2	=	(WORD)(g_auiGuildPetExp[iLevel-1]&0xffff);

			SendSimpleWork(NULL,eSWWG_UPDATE_GUILD_PET_EXP,i,iPet,wExp1,wExp2,0);
		}

		for	(int iGuardian=0;iGuardian<dGUILD_GUARDIAN_TYPE_COUNT;iGuardian++)
		{
			int	iLevel		=	lpGuild->getPetLevel(lpGuild->m_aGuildGuardian[iGuardian].m_dwExperience);
			int	iOldExp		=	lpGuild->m_aGuildGuardian[iGuardian].m_dwExperience-lpGuild->m_aGuildGuardian[iGuardian].m_wExpDuringWeek;
			int	iOldLevel	=	lpGuild->getPetLevel(iOldExp);
			int	iExpDuringWeek	=	lpGuild->m_aGuildGuardian[iGuardian].m_wExpDuringWeek;
			int	iMinExp		=	iOldLevel*c_iDownGuildPetGuardianLevelFactor;

			lpGuild->m_aGuildPet[iGuardian].m_wExpDuringWeek	=	0;

			if	(iOldLevel	<	c_iMinLevelForCheckUpkeepGuildPetAndGuardianLevel)
				continue;
			if	(iExpDuringWeek	>=	iMinExp)
				continue;

			lpGuild->m_aGuildGuardian[iGuardian].m_dwExperience	=	g_auiGuildPetExp[iLevel-1];

			WORD	wExp1	=	(WORD)(g_auiGuildPetExp[iLevel-1]>>16);
			WORD	wExp2	=	(WORD)(g_auiGuildPetExp[iLevel-1]&0xffff);

			SendSimpleWork(NULL,eSWWG_UPDATE_GUILD_PET_EXP,i,iGuardian+dGUILD_PET_TYPE_COUNT,wExp1,wExp2,0);
		}
	}

	SendSimpleWork(NULL,eSWWG_UPDATE_GUILD_PET_EXP,0xffff);
}

void
cGuildManagerForWS::changeTodayGuildBattleCount(int _iGuild,int _iCount)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);

	if	(!lpGuild)
		return;

	lpGuild->m_bf3ThisDayGuildBattleCount	=	_iCount;

	((cGuildForWS *)lpGuild)->sendGuildBattleScore();

	SaveGuildBaseData(lpGuild);
}

void
cGuildManagerForWS::changeThisWeekGuildBattleCount(int _iGuild,int _iCount)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);

	if	(!lpGuild)
		return;

	lpGuild->m_bf5ThisWeekGuildBattleCount	=	_iCount;

	((cGuildForWS *)lpGuild)->sendGuildBattleScore();

	SaveGuildBaseData(lpGuild);
}

void
cGuildManager::changeGuildMarkTwinkleStatus(int _iGuild,BOOL _bIsTwinkle)
{
	if	(_iGuild	==	0xffff)
	{
		for	(int	iGuild	=	dGUILD_MAX_COUNT;iGuild;)
		{
			--iGuild;

			cGuild	*lpGuild	=	GetGuildPnt(iGuild);

			if	(!lpGuild	|| lpGuild->wSerial==0xffff)
				continue;

			lpGuild->m_bf1IsTwinkle	=	FALSE;
		}

		SendSimpleWork(NULL,eSWWG_CHANGE_GUILD_MARK_TWINKLE_STATUS,0xffff);
	}
	
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);

	if	(!lpGuild)
		return;

	lpGuild->m_bf1IsTwinkle	=	_bIsTwinkle;

	SaveGuildBaseData(lpGuild);
	SendSimpleWork(NULL,eSWWG_CHANGE_GUILD_MARK_TWINKLE_STATUS,_iGuild,_bIsTwinkle);
}

void
cGuildManager::changeGuildHallLevel(int _iGuild,int _iHallLevel)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);

	if	(!lpGuild)
		return;

	cGuildHallInfo	*lpOwnHall	=	getHallByGuild(lpGuild);

	if	(_iHallLevel	>=	2)
	{
		int	iHallCount,iHallSlot	=	-1;
		int	aiHallCountByHallLevel[]=	{0,0xffff,27,9,3,1};

		iHallCount	=	aiHallCountByHallLevel[_iHallLevel];

		for	(int i=0;i<iHallCount;i++)
		{
			if	(m_aHallList[_iHallLevel][i].m_wOwnGuild	==	0xffff)
			{
				iHallSlot	=	i;
				break;
			}
		}

		if	(iHallSlot	==	-1)
			return;

		cGuildHallInfo	*lpHallInfo	=	&m_aHallList[_iHallLevel][iHallSlot];

		lpHallInfo->m_wOwnGuild	=	lpGuild->m_wSerial;
		lpHallInfo->m_dwHashCode=	GetHashCode(lpGuild->m_strName);
		lpHallInfo->m_bf1IsOpen	=	TRUE;
	}

	if	(lpOwnHall)
	{
		lpOwnHall->m_wOwnGuild			=	0xffff;
		lpOwnHall->m_dwHashCode			=	0xffffffff;
		lpOwnHall->m_bf1IsOpen			=	TRUE;
		lpOwnHall->m_bf3HallShape		=	0;
		lpOwnHall->m_wInvadeGuild		=	0xffff;
	}

	lpGuild->m_wHallLevel		=	_iHallLevel;

	SaveGuildBaseData(lpGuild);
	sendGSPUpdateGuildInfo(NULL,lpGuild->m_wSerial,FALSE);

	SendUpdateGuildHallInfo();
}

void
cGuildManager::changeGuildETCInfo(int _iGuild , WORD _wGuildMasterGrace, WORD _wRoenGoodWill)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);
	
	if	(!lpGuild)
		return;	

	lpGuild->m_bf3GuildGoodwillForRoen	= _wRoenGoodWill;
	lpGuild->m_bf1IsUseGuildMasterGrace	= _wGuildMasterGrace;
	
	
	SaveGuildBaseData(lpGuild);
	sendGSPUpdateGuildInfo(NULL,lpGuild->m_wSerial,FALSE);
	
	SendUpdateGuildHallInfo();
}

void
cGuildManager::changeGuildMasterGrace(int _iGuild,WORD _wValue1)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);
	
	if	(!lpGuild)
		return;	
	
	lpGuild->m_bf1IsUseGuildMasterGrace	= _wValue1;
	
	SaveGuildBaseData(lpGuild);
	sendGSPUpdateGuildInfo(NULL,lpGuild->m_wSerial,FALSE);
	
	SendUpdateGuildHallInfo();
}


void
cGuildManager::changeGuildVillageInfo(int _iGuild,WORD _wValue1 ,WORD _wValue2 , WORD _wValue3 , WORD _wValue4 , 
									  WORD _wValue5 , WORD _wValue6, WORD _wValue7)
{
	cGuild	*lpGuild	=	GetGuildPnt(_iGuild);
	
	if	(!lpGuild)
		return;	

	lpGuild->m_awGuildVillage[0] = _wValue1;
	lpGuild->m_awGuildVillage[1] = _wValue2;
	lpGuild->m_awGuildVillage[2] = _wValue3;
	lpGuild->m_awGuildVillage[3] = _wValue4;
	lpGuild->m_awGuildVillage[4] = _wValue5;
	lpGuild->m_awGuildVillage[5] = _wValue6;
	lpGuild->m_awGuildVillage[6] = _wValue7;
	
	SaveGuildBaseData(lpGuild);
	sendGSPUpdateGuildInfo(NULL,lpGuild->m_wSerial,FALSE);
	
	SendUpdateGuildHallInfo();
}

void
cGuildManagerForWS::operatePeriodicallyResetInfoWork(WORD _wWeekDay)
{
	BOOL	bIsWeekDay	=	FALSE;

	if	(_wWeekDay	==	dRESET_WEEKDAY_TO_RESET_DAY_BASED_GUILD_INFO)	//	수요일이다
		bIsWeekDay	=	TRUE;

	for(int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		cGuild	*lpGuild	=	GetGuildPntBySerial(i);

		if	(!lpGuild	|| lpGuild->wSerial==0xffff)
			continue;

		if	(bIsWeekDay)
			lpGuild->m_bf5ThisWeekGuildBattleCount	=	0;	//	이번주 길드 배틀 횟수

		lpGuild->m_bf3ThisDayGuildBattleCount	=	0;	//	오늘 길드 배틀한 회수 초기화
	}

	if	(bIsWeekDay)
		SendSimpleWork(NULL,eSWWG_RESET_1WEEK_GUILD_INFO);
	else
		SendSimpleWork(NULL,eSWWG_RESET_1DAY_GUILD_INFO);
}


void
cGuildManagerForWS::operate1MinuteWork()	//	주기적으로 처리해야 하는 길드 작업 처리
{
	int iCurrentTimeSaveGuildCount	= 0;

	for(WORD i=dGUILD_MAX_COUNT;i;)
	{	
		--i;

		cGuildForWS * lpGuild =	(cGuildForWS*)g_guildManager.GetGuildPnt(i);

		if	(!lpGuild || lpGuild->m_wSerial==0xffff )
			continue;
		
		if	(!lpGuild->operate1MinuteWork())
			continue;
		int iTime = timeGetTime() - lpGuild->dwUpdateTime;
		int	iDeltaSaveTime	=	abs(iTime)/60/1000;
 
 		if	(iDeltaSaveTime	<=	10)	//	10분에 한번 저장
			continue;

 		if	(iDeltaSaveTime	<=	20 && iCurrentTimeSaveGuildCount >= 10)	//	너무 많이 저장하는걸 막지만 그래도 20분 이상 되면 저장
 			continue;

 		lpGuild->dwUpdateTime	=	timeGetTime();

 		UpdateGuildData(lpGuild);

 		++iCurrentTimeSaveGuildCount;
	}
}

void
cGuildManagerForWS::updateLastConnectTime(int _iGuild,int _iMemberRank)
{
	if	(_iGuild	==	0xffff)
		return;

	cGuild	*lpGuild	=	g_guildManager.GetGuildPnt(_iGuild);

	if	(!lpGuild)
		return;

	BOOL	bIsRequireSave=	FALSE;
	
	//	길드원 접속 일자 갱신
	{
		int	iOldDay	=	lpGuild->m_timeLastConnectTime.year*372+
						lpGuild->m_timeLastConnectTime.month*31+
						lpGuild->m_timeLastConnectTime.day;

		lpGuild->m_timeLastConnectTime.year	=	cSRVUTIL::GetYear()-100;
		lpGuild->m_timeLastConnectTime.month=	cSRVUTIL::GetMonth()+1;
		lpGuild->m_timeLastConnectTime.day	=	cSRVUTIL::GetDay();

		int	iCurDay	=	lpGuild->m_timeLastConnectTime.year*372+lpGuild->m_timeLastConnectTime.month*31+lpGuild->m_timeLastConnectTime.day;

		if	(iOldDay	!=	iCurDay)
			bIsRequireSave=	TRUE;
	}

	if	(_iMemberRank==dGUILD_CLASS_MASTER)	//	길마 접속 일자 갱신
	{
		int	iOldDay	=	lpGuild->m_timeLastConnectedMasterTime.year*372+lpGuild->m_timeLastConnectedMasterTime.month*31+lpGuild->m_timeLastConnectedMasterTime.day;

		lpGuild->m_timeLastConnectedMasterTime.year	=	cSRVUTIL::GetYear()-100;
		lpGuild->m_timeLastConnectedMasterTime.month=	cSRVUTIL::GetMonth()+1;
		lpGuild->m_timeLastConnectedMasterTime.day	=	cSRVUTIL::GetDay();

		int	iCurDay	=	lpGuild->m_timeLastConnectedMasterTime.year*372+
						lpGuild->m_timeLastConnectedMasterTime.month*31+
						lpGuild->m_timeLastConnectedMasterTime.day;

		if	(iOldDay!=	iCurDay)
			bIsRequireSave=	TRUE;
	}

	if	(bIsRequireSave)
		SaveGuildBaseData(lpGuild);
}