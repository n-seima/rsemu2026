#include "cGame.h"

//
//	프로파일러 정보 출력
void
cGAME::exportProfilerInfomation(char *_lpstrFileName)
{
	char	strFileName[512];

	if (_lpstrFileName)
		strcpy(strFileName,_lpstrFileName);
	else
		sprintf(strFileName,"%s/profile info.txt",g_strLogFolder);

	cFILE	file;

	file.Open(strFileName,"wb");

	char	strOutput[256];
	int		i;

	for (i=0;i<m_profiler.m_iNumbers;i++)
	{
		char	*lpstrName	=	m_profiler.getName(i);

		if (lpstrName)
			sprintf(strOutput,"%-32s : Total %s MS,Hit Count %8d,This Time %s[%4d],Average %s MS,%s%%",
						m_profiler.getName(i),m_profiler.getTotalTime(i),m_profiler.getHitCount(i),
						m_profiler.getCurrentTimeInfo(i),m_profiler.getMaxHitCount(i),m_profiler.getAverageTime(i),
						m_profiler.getPercent(i)
						);
		else
			sprintf(strOutput,"                          0x%.4x : Total %s MS,Hit Count %8d,This Time %s[%4d],Average %s MS,%s%%",
						m_profiler.m_aProfiles[i].m_dwID,m_profiler.getTotalTime(i),m_profiler.getHitCount(i),
						m_profiler.getCurrentTimeInfo(i),m_profiler.getMaxHitCount(i),m_profiler.getAverageTime(i),
						m_profiler.getPercent(i)
						);

		file.writeString(strOutput);
	}

	file.Close();

	sprintf(strFileName,"%s/profile info 2.txt",g_strLogFolder);

	file.Open(strFileName,"wb");

	file.writeString("이름	소모시간	횟수	평균 소모시간	점유율");

	for (i=0;i<m_profiler.m_iNumbers;i++)
	{
		char	*lpstrName	=	m_profiler.getName(i);

		if (lpstrName)
			sprintf(strOutput,"%s	%s MS	%8d	%s MS	%s%%",
						m_profiler.getName(i),m_profiler.getTotalTime(i),m_profiler.getHitCount(i),
						m_profiler.getAverageTime(i),m_profiler.getPercent(i)
						);
		else
			sprintf(strOutput,"0x%.4x	%s MS	%8d	%s MS	%s%%",
						m_profiler.m_aProfiles[i].m_dwID,m_profiler.getTotalTime(i),m_profiler.getHitCount(i),
						m_profiler.getAverageTime(i),m_profiler.getPercent(i)
						);

		file.writeString(strOutput);
	}

	file.Close();
}

//
//	프로파일러 정보 출력
void
cGAME::exportPacketCounterInfomation()
{
	char	strFileName[512];

	sprintf(strFileName,"%s/maximum packet counter info.txt",g_strLogFolder);

	cFILE	file;

	if	(!file.Open(strFileName,"wb"))
		return;

	int		iMaxCount	=	0;
	char	strOutput[256];

	for (int i=0;i<m_iFieldCount;i++)
	{
		sprintf(strOutput,"%s [%d]",m_pField[i].m_strFileName,m_pField[i].m_queSendPacket.m_iMaxPacketCount);

		iMaxCount	=	max(iMaxCount,m_pField[i].m_queSendPacket.m_iMaxPacketCount);

		file.writeString(strOutput);
	}

	sprintf(strOutput,"maximum packet count [%d]",iMaxCount);

	file.writeString(strOutput);
	file.Close();
}

//
//	프로파일 시작
void
cGAME::beginProfile(char *_lpstrName)
{
	m_profiler.begin(_lpstrName);
}

//
//	프로파일 시작
void
cGAME::beginProfileByID(DWORD _dwID,char *_lpstrName)
{
	m_profiler.beginByID(_dwID,_lpstrName);
}

//
//	프로파일 시작
void
cGAME::beginProfile(int _iIndex)
{
	m_profiler.begin(_iIndex);
}

//
//	프로파일 종료
void
cGAME::endProfile(char *_lpstrName)
{
	m_profiler.end(_lpstrName);
}

//
//	프로파일 종료
void
cGAME::endProfile(int _iIndex)
{
	m_profiler.end(_iIndex);
}

//
//	프로파일 종료
void
cGAME::endProfileByID(DWORD _dwID)
{
	m_profiler.endByID(_dwID);
}

void
cGAME::exportUserPlayerInfoInField()
{
	CLOGWT("active_skill_info","=========================================================================================");
	CLOG("active_skill_info","begin to export active skill data");

	for (int iField=0;iField<m_iFieldCount;++iField)	//	각 필드로 간 패킷 처리
	{
		cFIELD	*lpField	=	&m_pField[iField];

		lpField->exportSkillList();
	}

/*	CLOG("playerCountPerField","=========================================================================================");
	CLOGWT("playerCountPerField","체크 시작 시간 : %s",m_pField[0].m_beginTime.getTime());
	
	for (int iField=0;iField<m_iFieldCount;++iField)	//	각 필드로 간 패킷 처리
	{
		cFIELD	*lpField	=	&m_pField[iField];

		char	strExp[32];

		DWORD	dwHigh		=	(DWORD)(lpField->m_llExpSum/1000000000);
		DWORD	dwLow		=	(DWORD)(lpField->m_llExpSum%1000000000);

		sprintf(strExp,"%.9d%.9d",dwHigh,dwLow);

		CLOGWT("playerCountPerField","[user %5d명] [exp %s] [%.3d] %s ",lpField->getUniqueUserCount(),strExp,lpField->m_iUniqueSerial,lpField->m_strName);
	}
	*/
}

void
cGAME::resetUserPlayStaticsInField()
{
	for (int iField=0;iField<m_iFieldCount;++iField)	//	각 필드로 간 패킷 처리
		m_pField[iField].resetUserPlayStatics();
}

void
cGAME::makeStaticsOfPacketFlowing()
{
	if	(m_dwCountOfCheckSendPacketSize		==	0	||
		m_dwCountOfCheckSendPacketSize		==	0	||
		m_dwCountOfCheckReceivePacketSize	==	0	||
		m_dwCountOfCheckReceivePacketSize	==	0)
		return;

	CLOG("staticsOfPacketFlowing","---------------------------------------------------------------");

	DWORD	dwAverageSizeOfSendPacketSizePerMinute		=	(int)(m_dwSumOfSendPacketSizePerMinute/m_dwCountOfCheckSendPacketSize);
	DWORD	dwAverageCountOfSendPacketSizePerMinute		=	m_dwCountOfSendPacketSizePerMinute/m_dwCountOfCheckSendPacketSize;
	DWORD	dwAverageSizeOfReceivePacketSizePerMinute	=	(int)(m_dwSumOfReceivePacketSizePerMinute/m_dwCountOfCheckReceivePacketSize);
	DWORD	dwAverageCountOfReceivePacketSizePerMinute	=	m_dwCountOfReceivePacketSizePerMinute/m_dwCountOfCheckReceivePacketSize;

	CLOGWT("staticsOfPacketFlowing","PlayerCount [%d]",m_iPlayerCount);
	CLOGWT("staticsOfPacketFlowing","Server->Client AverageSizeOfSendPacketSizePerMinute [%d Byte]",dwAverageSizeOfSendPacketSizePerMinute);
	CLOGWT("staticsOfPacketFlowing","Server->Client AverageCountOfSendPacketSizePerMinute [%d Times]",dwAverageCountOfSendPacketSizePerMinute);
	CLOGWT("staticsOfPacketFlowing","Client->Server AverageSizeOfReceivePacketSizePerMinute [%d Byte]",dwAverageSizeOfReceivePacketSizePerMinute);
	CLOGWT("staticsOfPacketFlowing","Client->Server AverageCountOfReceivePacketSizePerMinute [%d Times]",dwAverageCountOfReceivePacketSizePerMinute);
}

void
cGAME::operateReceivePacketDataStaticsWork(int _iSize)
{
	m_dwThisTimeReceivePacketSizeSum	+=	_iSize;
	m_dwThisTimeReceivePacketCount++;

	if	(m_dwLastCheckReceivePacketSizeTime)
		if	(g_dwCurrentTime	>=	m_dwLastCheckReceivePacketSizeTime+60*1000	||	g_dwCurrentTime	<	m_dwLastCheckReceivePacketSizeTime)
		{
			if	(m_dwThisTimeReceivePacketSizeSum	>	m_dwMaxReceivePacketSizePerMinute)
			{
				m_dwMaxReceivePacketSizePerMinute	=	m_dwThisTimeReceivePacketSizeSum;
				m_dwMaxReceivePacketCountPerMinute	=	m_dwThisTimeReceivePacketCount;
			}

			m_dwCountOfCheckReceivePacketSize++;
			
			m_dwSumOfReceivePacketSizePerMinute		+=	m_dwThisTimeReceivePacketSizeSum;	//	
			m_dwCountOfReceivePacketSizePerMinute	+=	m_dwThisTimeReceivePacketCount;

			m_dwThisTimeReceivePacketSizeSum		=	0;
			m_dwThisTimeReceivePacketCount			=	0;
			m_dwLastCheckReceivePacketSizeTime		=	g_dwCurrentTime;
		}

	if	(m_dwLastCheckReceivePacketSizeTime	==	0)
		m_dwLastCheckReceivePacketSizeTime		=	g_dwCurrentTime;
}

void
cGAME::operateSendPacketDataStaticsWork(int _iSize)
{
	m_dwThisTimeSendPacketSizeSum	+=	_iSize;
	m_dwThisTimeSendPacketCount++;

	if	(m_dwLastCheckSendPacketSizeTime)
		if	(g_dwCurrentTime	>=	m_dwLastCheckSendPacketSizeTime+60*1000	||	g_dwCurrentTime	<	m_dwLastCheckSendPacketSizeTime)
		{
			if	(m_dwThisTimeSendPacketSizeSum	>	m_dwMaxSendPacketSizePerMinute)
			{
				m_dwMaxSendPacketSizePerMinute	=	m_dwThisTimeSendPacketSizeSum;
				m_dwMaxSendPacketCountPerMinute	=	m_dwThisTimeSendPacketCount;
			}

			m_dwCountOfCheckSendPacketSize++;
			
			m_dwSumOfSendPacketSizePerMinute		+=	m_dwThisTimeSendPacketSizeSum;	//	
			m_dwCountOfSendPacketSizePerMinute	+=	m_dwThisTimeSendPacketCount;

			m_dwThisTimeSendPacketSizeSum		=	0;
			m_dwThisTimeSendPacketCount			=	0;
			m_dwLastCheckSendPacketSizeTime		=	g_dwCurrentTime;
		}

	if	(m_dwLastCheckSendPacketSizeTime	==	0)
		m_dwLastCheckSendPacketSizeTime		=	g_dwCurrentTime;
}

int
cGAME::checkPacketSize(int _iMaxSize,int _iSize,char *_lpstrName)
{
	if	(_iSize	>	_iMaxSize)
	{
		_iMaxSize	=	_iSize;
		CLOG("packetSize","[%4d byte] %s __max",_iSize,_lpstrName);
	}
	else
		CLOG("packetSize","[%4d byte] %s",_iSize,_lpstrName);


	return	_iMaxSize;
}

void
cGAME::checkPacketSize()
{
	int	iMaxPacketSize=0;

	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_RESULT_CONNECT						),"SG_RESULT_CONNECT						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_RESULT_JOIN							),"SG_RESULT_JOIN							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_PLAYER_STATUS_DATA			),"SG_UPDATE_PLAYER_STATUS_DATA			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_JOIN_PLAYER							),"SG_JOIN_PLAYER							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MOVE								),"SG_MOVE								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MOVE_RESULT							),"SG_MOVE_RESULT							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MOVE_FAILED							),"SG_MOVE_FAILED							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_STOP								),"SG_STOP								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OUTER_ACTORS_POS_UPDATE				),"SG_OUTER_ACTORS_POS_UPDATE				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ACTION_TO_ACTOR						),"SG_ACTION_TO_ACTOR						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ACTION_TO_GROUP						),"SG_ACTION_TO_GROUP						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ACTION_TO_GROUND					),"SG_ACTION_TO_GROUND					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ACTION_QUICK						),"SG_ACTION_QUICK						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_USE_SKILL_RESULT					),"SG_USE_SKILL_RESULT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CORRECT_PLAYER_POSITION				),"SG_CORRECT_PLAYER_POSITION				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MINIMUM_ACTOR_INFO_LIST				),"SG_MINIMUM_ACTOR_INFO_LIST				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OPERATOR_LIST						),"SG_OPERATOR_LIST						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REFIT_ACTORS_POSITION				),"SG_REFIT_ACTORS_POSITION				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EVENT_NPC_LIST						),"SG_EVENT_NPC_LIST						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SIMPLE_ACTOR_INFO_LIST				),"SG_SIMPLE_ACTOR_INFO_LIST				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SIMPLE_ACTOR_INFO					),"SG_SIMPLE_ACTOR_INFO					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_VERY_SIMPLE_ACTOR_INFO_LIST			),"SG_VERY_SIMPLE_ACTOR_INFO_LIST			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PLAYER_OUT							),"SG_PLAYER_OUT							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SET_MOVE_ABILITY					),"SG_SET_MOVE_ABILITY					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REGEN_MONSTER						),"SG_REGEN_MONSTER						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BASIC_ATTACK						),"SG_BASIC_ATTACK						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CONTINUOUS_HIT_BY_CHARACTER			),"SG_CONTINUOUS_HIT_BY_CHARACTER			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CONTINUOUS_HIT_BY_VALUE				),"SG_CONTINUOUS_HIT_BY_VALUE				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_LAND_MARKER							),"SG_LAND_MARKER							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_STRIKE_INTERVAL_SHOOTER				),"SG_STRIKE_INTERVAL_SHOOTER				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CAST_INTERVAL_SHOOTER				),"SG_CAST_INTERVAL_SHOOTER				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BUNSHINE_ATTACK						),"SG_BUNSHINE_ATTACK						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SCIMITAR_CUTTING					),"SG_SCIMITAR_CUTTING					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_GUARDIAN_POST						),"SG_GUARDIAN_POST						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_FIRST_AID							),"SG_FIRST_AID							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REVIVE								),"SG_REVIVE								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_BOOST_COUNT					),"SG_UPDATE_BOOST_COUNT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PLAYER_INFO							),"SG_PLAYER_INFO							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SUCCESS_TO_JOIN_GAME				),"SG_SUCCESS_TO_JOIN_GAME				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_QUICK_ACTION						),"SG_QUICK_ACTION						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_KILL								),"SG_KILL								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_EXP							),"SG_UPDATE_EXP							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_BASIC_INFORMATION			),"SG_UPDATE_BASIC_INFORMATION			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_LEVEL_UP							),"SG_LEVEL_UP							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_LEVEL_UP_OTHER						),"SG_LEVEL_UP_OTHER						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_STATUS						),"SG_UPDATE_STATUS						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DEATH_PENELTY_SECOND				),"SG_DEATH_PENELTY_SECOND				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_QUESTION_FOR_RECOVER_DEATH_PENELTY	),"SG_QUESTION_FOR_RECOVER_DEATH_PENELTY	");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_STATE_RESULT					),"SG_CHANGE_STATE_RESULT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_INCREASE_ABILITY_LEVEL				),"SG_INCREASE_ABILITY_LEVEL				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_VERY_SIMPLE_ACTOR_INFO				),"SG_VERY_SIMPLE_ACTOR_INFO				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMOVE_ACTOR						),"SG_REMOVE_ACTOR						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_TRANSFORMATION						),"SG_TRANSFORMATION						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_TRANSFORMATION_FAILED				),"SG_TRANSFORMATION_FAILED				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MOVE_FIELD_RESULT					),"SG_MOVE_FIELD_RESULT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MOVE_FIELD_NOW						),"SG_MOVE_FIELD_NOW						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_TOGGLE_SITDOWN						),"SG_TOGGLE_SITDOWN						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SITDOWN_RESULT						),"SG_SITDOWN_RESULT						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REDISTRIBUTE						),"SG_REDISTRIBUTE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_STOP_BATTLE							),"SG_STOP_BATTLE							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_GG_AUTH_DATA						),"SG_GG_AUTH_DATA						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_WINNING_PRIZE_LOTTO					),"SG_WINNING_PRIZE_LOTTO					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ARCA_LIST							),"SG_ARCA_LIST							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ADD_ARCA							),"SG_ADD_ARCA							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ATTACK_TO_OBJECT					),"SG_ATTACK_TO_OBJECT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ARCA_WORK							),"SG_ARCA_WORK							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BROKEN_ARCA_ITEM_BY_ATTACK			),"SG_BROKEN_ARCA_ITEM_BY_ATTACK			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CLOSE_RANGE_ARCA_WORK				),"SG_CLOSE_RANGE_ARCA_WORK				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_FIELD_TRAP_WORK						),"SG_FIELD_TRAP_WORK						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CLOSE_RANGE_FIELD_TRAP_WORK			),"SG_CLOSE_RANGE_FIELD_TRAP_WORK			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ENTER_TO_THE_SECRET_DUNGEON			),"SG_ENTER_TO_THE_SECRET_DUNGEON			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SECRET_DUNGEON_MESSAGE				),"SG_SECRET_DUNGEON_MESSAGE				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ETC_WORK							),"SG_ETC_WORK							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PLAY_SOUND							),"SG_PLAY_SOUND							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHECK_RANGE_ETC_WORK				),"SG_CHECK_RANGE_ETC_WORK				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SET_ANM								),"SG_SET_ANM								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PET_INFO							),"SG_PET_INFO							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_PET_COMMAND					),"SG_CHANGE_PET_COMMAND					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SYNC_PET_INFO						),"SG_SYNC_PET_INFO						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PET_WAIT_COMMAND					),"SG_PET_WAIT_COMMAND					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MAKE_BREEDING_RECORD_BOOK			),"SG_MAKE_BREEDING_RECORD_BOOK			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_PET_EXP						),"SG_UPDATE_PET_EXP						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_PET_NAME						),"SG_CHANGE_PET_NAME						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PARTING_WITH_PET					),"SG_PARTING_WITH_PET					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SUMMON_BEAST_INFO					),"SG_SUMMON_BEAST_INFO					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_RIDING_DOG							),"SG_RIDING_DOG							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_WIDE_AREA_DAMAGE					),"SG_WIDE_AREA_DAMAGE					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_WIDE_AREA_EFFECT					),"SG_WIDE_AREA_EFFECT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SET_REACTION_STATUS					),"SG_SET_REACTION_STATUS					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_TURN_ON_PASSIVE_SKILL				),"SG_TURN_ON_PASSIVE_SKILL				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_HIT_THORN_DAMAGE					),"SG_HIT_THORN_DAMAGE					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ABSORB_DAMAGE						),"SG_ABSORB_DAMAGE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_SPECIAL_SKILL_FIELD			),"SG_CHANGE_SPECIAL_SKILL_FIELD			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_RELEASE_SHIMMERING_SHIELD			),"SG_RELEASE_SHIMMERING_SHIELD			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SET_SHIMMERING_SHIELD				),"SG_SET_SHIMMERING_SHIELD				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CURE_ACTOR							),"SG_CURE_ACTOR							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CURE_ACTOR_BY_POTION				),"SG_CURE_ACTOR_BY_POTION				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHARGE_CP							),"SG_CHARGE_CP							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_HIT_AURA_SKILL						),"SG_HIT_AURA_SKILL						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DISPLACEMENT						),"SG_DISPLACEMENT						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_FLOAT_MACHINE						),"SG_FLOAT_MACHINE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_RELEASE_MACHINE						),"SG_RELEASE_MACHINE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SHOOT_MACHINE_MISSILE				),"SG_SHOOT_MACHINE_MISSILE				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_FIRE_TAIL_CHASER					),"SG_FIRE_TAIL_CHASER					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MAKE_ACTION_FORM					),"SG_MAKE_ACTION_FORM					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_THRUST_ACTORS						),"SG_THRUST_ACTORS						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_USE_SKILL_TO_ACTOR					),"SG_USE_SKILL_TO_ACTOR					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_USE_SKILL_TO_GROUND					),"SG_USE_SKILL_TO_GROUND					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CREATE_DUMMY						),"SG_CREATE_DUMMY						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_RELEASE_DUMMY						),"SG_RELEASE_DUMMY						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_RECEIVE_AID_MAGIC					),"SG_RECEIVE_AID_MAGIC					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_THRUST								),"SG_THRUST								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_RUSH								),"SG_RUSH								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ACTIVE_REACTION_SKILL				),"SG_ACTIVE_REACTION_SKILL				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ICY_STALAGMITE						),"SG_ICY_STALAGMITE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ACTOR_ENCHANTED_STATUS				),"SG_ACTOR_ENCHANTED_STATUS				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_TELEPORT							),"SG_TELEPORT							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_APPLY_SKILL_EXTRA_EFFECT			),"SG_APPLY_SKILL_EXTRA_EFFECT			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_FIND_USER_RESULT					),"SG_FIND_USER_RESULT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ACTIVE_SKILL_EFFECT					),"SG_ACTIVE_SKILL_EFFECT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ACTIVE_MIRROR_TOWER					),"SG_ACTIVE_MIRROR_TOWER					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SIMPLE_HIT							),"SG_SIMPLE_HIT							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_HIT_DAMAGE							),"SG_HIT_DAMAGE							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REGEN_HP_BY_AID_SKILL				),"SG_REGEN_HP_BY_AID_SKILL				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_RECOVER_HP_BY_AID_SKILL				),"SG_RECOVER_HP_BY_AID_SKILL				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_HIT_ACTION							),"SG_HIT_ACTION							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DETECTING_EVIL						),"SG_DETECTING_EVIL						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MISS								),"SG_MISS								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_READY_TO_WARP_TO_NEAREST_VILLAGE	),"SG_READY_TO_WARP_TO_NEAREST_VILLAGE	");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMOVE_PORTAL						),"SG_REMOVE_PORTAL						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ADD_PORTAL							),"SG_ADD_PORTAL							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PORTAL_LIST							),"SG_PORTAL_LIST							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SKILL_EFFECT						),"SG_SKILL_EFFECT						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PLAY_SKILL_SOUND					),"SG_PLAY_SKILL_SOUND					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DROPPING_ITEM						),"SG_DROPPING_ITEM						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_STRIP_EQUIPMENT						),"SG_STRIP_EQUIPMENT						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_HIDE_EQUIPMENT						),"SG_HIDE_EQUIPMENT						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_STRIP_EQUIPMENT_RESULT				),"SG_STRIP_EQUIPMENT_RESULT				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMOVE_DROPPED_ITEMS				),"SG_REMOVE_DROPPED_ITEMS				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMOVE_DROPPED_ITEM					),"SG_REMOVE_DROPPED_ITEM					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_DROPPED_ITEM_INFO			),"SG_CHANGE_DROPPED_ITEM_INFO			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DROPPING_ITEMS						),"SG_DROPPING_ITEMS						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PICK_ITEM_RESULT					),"SG_PICK_ITEM_RESULT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EQUIP_ITEM_RESULT					),"SG_EQUIP_ITEM_RESULT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_USE_ITEM_RESULT						),"SG_USE_ITEM_RESULT						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_USE_ITEM_TO_ITEM_RESULT				),"SG_USE_ITEM_TO_ITEM_RESULT				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_USE_BELT_ITEM_RESULT				),"SG_USE_BELT_ITEM_RESULT				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_USE_ITEM							),"SG_USE_ITEM							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_LOTTERY_TICKET_PRIZE_WINNING		),"SG_LOTTERY_TICKET_PRIZE_WINNING		");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ITEM_DATA							),"SG_ITEM_DATA							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_INVENTORY_DATA						),"SG_INVENTORY_DATA						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EQUIPMENT_DATA						),"SG_EQUIPMENT_DATA						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_APPEARANCE					),"SG_CHANGE_APPEARANCE					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DROP_ITEM_RESULT					),"SG_DROP_ITEM_RESULT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_RELOAD_BELT_ITEM					),"SG_RELOAD_BELT_ITEM					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BELT_ITEM_RELOAD_RESULT				),"SG_BELT_ITEM_RELOAD_RESULT				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_ITEM_PLACE_RESULT			),"SG_CHANGE_ITEM_PLACE_RESULT			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DECREASE_ITEM_COUNT					),"SG_DECREASE_ITEM_COUNT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ADD_ITEM							),"SG_ADD_ITEM							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ADD_GOLD							),"SG_ADD_GOLD							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SET_ITEM_UNIQUE_SERIAL				),"SG_SET_ITEM_UNIQUE_SERIAL				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_ITEM_DATA					),"SG_UPDATE_ITEM_DATA					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMOVE_ITEM							),"SG_REMOVE_ITEM							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_WAY_POINT_INFO				),"SG_UPDATE_WAY_POINT_INFO				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EXPIRED_ITEM						),"SG_EXPIRED_ITEM						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EXPIRED_ITEM_LIST					),"SG_EXPIRED_ITEM_LIST					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_USE_ORB_RESULT						),"SG_USE_ORB_RESULT						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DESTROY_ITEM_RESULT					),"SG_DESTROY_ITEM_RESULT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_ITEM_DURABILITY				),"SG_UPDATE_ITEM_DURABILITY				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REPAIR_ITEM							),"SG_REPAIR_ITEM							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ITEM_WORK_RESULT					),"SG_ITEM_WORK_RESULT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OPEN_REPAIR_ITEM_WINDOW				),"SG_OPEN_REPAIR_ITEM_WINDOW				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ITEM_PACK_NAME						),"SG_ITEM_PACK_NAME						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMOVE_PREFIX						),"SG_REMOVE_PREFIX						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REQUEST_TRADE						),"SG_REQUEST_TRADE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REQUEST_TRADE_RESULT				),"SG_REQUEST_TRADE_RESULT				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BEGIN_TRADE							),"SG_BEGIN_TRADE							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ADD_TRADE_ITEM_BY_ME				),"SG_ADD_TRADE_ITEM_BY_ME				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ADD_TRADE_ITEM_BY_TRADE_MATE		),"SG_ADD_TRADE_ITEM_BY_TRADE_MATE		");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_TRADE_ITEM_DATA_BY_ME		),"SG_CHANGE_TRADE_ITEM_DATA_BY_ME		");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_TRADE_ITEM_DATA_BY_TRADE_MATE),"SG_CHANGE_TRADE_ITEM_DATA_BY_TRADE_MATE");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SET_TRADE_GOLD						),"SG_SET_TRADE_GOLD						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMOVE_TRADE_ITEM					),"SG_REMOVE_TRADE_ITEM					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_READY_TRADE							),"SG_READY_TRADE							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PERMIT_TRADE						),"SG_PERMIT_TRADE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_TRADE								),"SG_TRADE								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_TRADE_MESSAGE						),"SG_TRADE_MESSAGE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHAT								),"SG_CHAT								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHAT_FAILED							),"SG_CHAT_FAILED							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_INFO								),"SG_INFO								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OUT									),"SG_OUT									");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SAY_RESULT							),"SG_SAY_RESULT							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EVENT_NOTICE						),"SG_EVENT_NOTICE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_NOTICE								),"SG_NOTICE								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PARTY_LIST							),"SG_PARTY_LIST							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_PARTY_STATUS					),"SG_CHANGE_PARTY_STATUS					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PARTY_INFO							),"SG_PARTY_INFO							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ASK_JOIN_PARTY						),"SG_ASK_JOIN_PARTY						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_JOIN_PARTY_MEMBER					),"SG_JOIN_PARTY_MEMBER					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PARTY_WORK							),"SG_PARTY_WORK							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_PARTY_MEMBER_INFO			),"SG_UPDATE_PARTY_MEMBER_INFO			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PARTY_MESSAGE						),"SG_PARTY_MESSAGE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_PARTY_NAME					),"SG_CHANGE_PARTY_NAME					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DISSOLUTION_PARTY					),"SG_DISSOLUTION_PARTY					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PCIK_PARTY_ITEM						),"SG_PCIK_PARTY_ITEM						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PCIK_PARTY_GOLD						),"SG_PCIK_PARTY_GOLD						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MESSAGE								),"SG_MESSAGE								");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REGIST_MESSAGE						),"SG_REGIST_MESSAGE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMAIN_HP							),"SG_REMAIN_HP							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REQUEST_ADD_FRIEND					),"SG_REQUEST_ADD_FRIEND					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REPLY_FOR_ADD_FRIEND				),"SG_REPLY_FOR_ADD_FRIEND				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EXIT_GAME_SUCCESS					),"SG_EXIT_GAME_SUCCESS					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PICK_PARTY_ITEM						),"SG_PICK_PARTY_ITEM						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ASK_REMOVE_FRIEND					),"SG_ASK_REMOVE_FRIEND					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ASK_REMOVE_FRIEND_RESULT			),"SG_ASK_REMOVE_FRIEND_RESULT			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_GAME_SERVER_MESSAGE					),"SG_GAME_SERVER_MESSAGE					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_READY_TO_MAKE_GUILD					),"SG_READY_TO_MAKE_GUILD					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OPEN_GUILD_BATTLE_SCHEDULER			),"SG_OPEN_GUILD_BATTLE_SCHEDULER			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OPEN_GUILD_MARK_COMPOSER			),"SG_OPEN_GUILD_MARK_COMPOSER			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_GUILD_BATTLE_SCHEDULE		),"SG_CHANGE_GUILD_BATTLE_SCHEDULE		");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_GUILD_BATTLE_SCHEDULE_IN_SAME_TIME	),"SG_GUILD_BATTLE_SCHEDULE_IN_SAME_TIME	");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MAKE_GUILD_RESULT					),"SG_MAKE_GUILD_RESULT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BASE_GUILD_INFO						),"SG_BASE_GUILD_INFO						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_GUILD_BATTLE_INFO_FOR_MEMBER	),"SG_UPDATE_GUILD_BATTLE_INFO_FOR_MEMBER	");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_READY_TO_NEXT_GUILD_BATTLE			),"SG_READY_TO_NEXT_GUILD_BATTLE			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_GUILD_BATTLE_NOTICE					),"SG_GUILD_BATTLE_NOTICE					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_GUILD_BATTLE_KILL_NOTICE			),"SG_GUILD_BATTLE_KILL_NOTICE			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_GUILD_BATTLE_INFO					),"SG_GUILD_BATTLE_INFO					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DISSOLUTION_GUILD					),"SG_DISSOLUTION_GUILD					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_JOIN_GUILD							),"SG_JOIN_GUILD							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_GUILD_MEMBER					),"SG_UPDATE_GUILD_MEMBER					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMOVE_GUILD_MEMBER					),"SG_REMOVE_GUILD_MEMBER					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EXIT_GUILD							),"SG_EXIT_GUILD							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ASK_JOIN_GUILD_RESULT				),"SG_ASK_JOIN_GUILD_RESULT				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ASK_JOIN_GUILD						),"SG_ASK_JOIN_GUILD						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_GUILD_LEVEL					),"SG_UPDATE_GUILD_LEVEL					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_GUILD_WORK_MESSAGE					),"SG_GUILD_WORK_MESSAGE					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_GUILD_TAX_RATE				),"SG_CHANGE_GUILD_TAX_RATE				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_GUILD_NOTICE					),"SG_CHANGE_GUILD_NOTICE					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_RANK_IN_GUILD				),"SG_CHANGE_RANK_IN_GUILD				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_GUILD_SKILL_LEVEL			),"SG_UPDATE_GUILD_SKILL_LEVEL			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_UPDATE_GUILD_BATTLE_COUNT_AT_THIS_WEEK	),"SG_UPDATE_GUILD_BATTLE_COUNT_AT_THIS_WEEK	");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_GUILD_BATTLE_RESULT					),"SG_GUILD_BATTLE_RESULT					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_GUILD_MARK					),"SG_CHANGE_GUILD_MARK					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_GUILD_MEMBER_LIST					),"SG_GUILD_MEMBER_LIST					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_GUILD_MARK_INFO						),"SG_GUILD_MARK_INFO						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SIMPLE_GUILD_INFO					),"SG_SIMPLE_GUILD_INFO					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_COMPLEX_SPEECH						),"SG_COMPLEX_SPEECH						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EVENT_FAILED						),"SG_EVENT_FAILED						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_END_DIAOLG							),"SG_END_DIAOLG							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OPEN_SHOP							),"SG_OPEN_SHOP							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BUY_ITEM							),"SG_BUY_ITEM							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_FAILED_BUY_ITEM						),"SG_FAILED_BUY_ITEM						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SELL_ITEM							),"SG_SELL_ITEM							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_FAILED_SELL_ITEM					),"SG_FAILED_SELL_ITEM					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_QUEST_INFO							),"SG_QUEST_INFO							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_QUEST_INFO					),"SG_CHANGE_QUEST_INFO					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CANCEL_QUEST						),"SG_CANCEL_QUEST						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DAMAGE_TO_ACTOR_BY_KARMA			),"SG_DAMAGE_TO_ACTOR_BY_KARMA			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_GET_TITLE							),"SG_GET_TITLE							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMOVE_TITLE						),"SG_REMOVE_TITLE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ANNOUNCE_QUIZ_EVENT					),"SG_ANNOUNCE_QUIZ_EVENT				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_START_COUNTDOWN						),"SG_START_COUNTDOWN						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_QUIZ_EVENT_QUESTION					),"SG_QUIZ_EVENT_QUESTION				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OX_QUIZ_RESULT						),"SG_OX_QUIZ_RESULT						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_END_QUIZ_EVENT						),"SG_END_OX_QUIZ							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_QUIZ_EVENT_QUESTION					),"SG_QUIZ_EVENT_QUESTION					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CANCEL_QUIZ_EVENT					),"SG_CANCEL_QUIZ_EVENT						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OX_QUIZ_SURVIVOR_COUNT				),"SG_OX_QUIZ_SURVIVOR_COUNT				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OX_QUIZ_LOSER_REVIVE_BATTLE			),"SG_OX_QUIZ_LOSER_REVIVE_BATTLE			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OX_QUIZ_WINNER_LIST					),"SG_OX_QUIZ_WINNER_LIST					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BEGIN_QUIZ_EVENT					),"SG_BEGIN_OX_QUIZ						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BUY_CARROT_SHOP_ITEM_RESULT			),"SG_BUY_CARROT_SHOP_ITEM_RESULT			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_DOOR_STATUS					),"SG_CHANGE_DOOR_STATUS					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DOOR_MESSAGE						),"SG_DOOR_MESSAGE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DOORS_STATUS_IN_FIELD				),"SG_DOORS_STATUS_IN_FIELD				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BANNER_INFO							),"SG_BANNER_INFO							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BANNER_TEXT							),"SG_BANNER_TEXT							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BANNER_LIST							),"SG_BANNER_LIST							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BOOST_POWER_BY_EVENT				),"SG_BOOST_POWER_BY_EVENT				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OPEN_BANK							),"SG_OPEN_BANK							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BANK_MESSAGE						),"SG_BANK_MESSAGE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_STORE_ITEM_TO_THE_BANK				),"SG_STORE_ITEM_TO_THE_BANK				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_STORE_GOLD_TO_THE_BANK				),"SG_STORE_GOLD_TO_THE_BANK				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_WITHDRAW_ITEM_FROM_THE_BANK			),"SG_WITHDRAW_ITEM_FROM_THE_BANK			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_WITHDRAW_GOLD_FROM_THE_BANK			),"SG_WITHDRAW_GOLD_FROM_THE_BANK			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MOVE_BANK_ITEM						),"SG_MOVE_BANK_ITEM						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	),"SG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OPEN_CART							),"SG_OPEN_CART							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CART_MESSAGE						),"SG_CART_MESSAGE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_WITHDRAW_ITEM_FROM_THE_CART			),"SG_WITHDRAW_ITEM_FROM_THE_CART			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_LOAD_FAILED_CART_PREMIUM_ITEM		),"SG_LOAD_FAILED_CART_PREMIUM_ITEM		");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_DESTROY_CART_ITEM_RESULT			),"SG_DESTROY_CART_ITEM_RESULT			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CARROT_SHOP_INFO					),"SG_CARROT_SHOP_INFO					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OPEN_CARROT_SHOP					),"SG_OPEN_CARROT_SHOP					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OPEN_PITCHMAN_SHOP					),"SG_OPEN_PITCHMAN_SHOP					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ADD_PITCHMAN_SHOP_ITEM				),"SG_ADD_PITCHMAN_SHOP_ITEM				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMOVE_PITCHMAN_SHOP_ITEM			),"SG_REMOVE_PITCHMAN_SHOP_ITEM			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_PITCHMAN_SHOP_INFO			),"SG_CHANGE_PITCHMAN_SHOP_INFO			");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE		),"SG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE		");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_BUY_PITCHMAN_SHOP_ITEM				),"SG_BUY_PITCHMAN_SHOP_ITEM				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SOLD_PITCHMAN_SHOP_ITEM				),"SG_SOLD_PITCHMAN_SHOP_ITEM				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ADD_PITCHMAN_SHOP					),"SG_ADD_PITCHMAN_SHOP					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMOVE_PITCHMAN_SHOP				),"SG_REMOVE_PITCHMAN_SHOP				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PITCHMAN_SHOP_LIST					),"SG_PITCHMAN_SHOP_LIST					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PITCHMAN_SHOP_INFO					),"SG_PITCHMAN_SHOP_INFO					");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PITCHMAN_SHOP_MESSAGE				),"SG_PITCHMAN_SHOP_MESSAGE				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SIMPLE_PITCHMAN_SHOP_INFO_LIST		),"SG_SIMPLE_PITCHMAN_SHOP_INFO_LIST		");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ADD_TRAP							),"SG_ADD_TRAP							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_REMOVE_TRAP							),"SG_REMOVE_TRAP							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EXPLOSION_TRAP						),"SG_EXPLOSION_TRAP						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EXIST_TRAP_LIST						),"SG_EXIST_TRAP_LIST						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EXPLOSION_TRAP_SKILL				),"SG_EXPLOSION_TRAP_SKILL				");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_EXIST_ACTIVE_FIELD_TRAP_LIST		),"SG_EXIST_ACTIVE_FIELD_TRAP_LIST		");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_WARNING_MESSAGE						),"SG_WARNING_MESSAGE						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CONTROL_MONSTER						),"SG_CONTROL_MONSTER						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_OCCUR_EFFECT						),"SG_OCCUR_EFFECT						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ACCOUNT_INFO						),"SG_ACCOUNT_INFO						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_PENELTY_TIME						),"SG_PENELTY_TIME						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_SENDINFOTOADMIN						),"SG_SENDINFOTOADMIN						");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_CLOSESERVER							),"SG_CLOSESERVER							");
	iMaxPacketSize=checkPacketSize(iMaxPacketSize,sizeof(SG_ACTION_TO_GROUP_STICKED_BIT			),"SG_ACTION_TO_GROUP_STICKED_BIT			");
}