#ifndef __REDSTONE_COMMON_H
#define __REDSTONE_COMMON_H

#include "Tracer.h"
//#include "TracerFromSaveLog.h"

#ifdef _FOR_JAPAN
#define STRICMP		lstrcmpi
#elif _FOR_THAI
#define STRICMP		lstrcmpi
#else
#define STRICMP		stricmp
#endif

#ifdef	_FOR_USA
#define	_OGP_SERVICE	1	//	퍼빚賭셔가 OGP다
#endif

#ifdef	_OGP_SERVICE

enum
{
	eOGP_BuyItem_UsedType_purchase	=	4,
	eOGP_BuyItem_UsedType_gift		=	5,
};

#define	dTOKEN_LENGTH			64
#define	dOGP_GAME_SERVICE_ID	13
#define	dOGP_GAME_SERVICE_KEY	"f96296a478"

class	cOGP_ServiceKey
{
public:
	WORD		m_wGameServiceID;
	WORD		m_wUsedType;
	char		m_strToken[dTOKEN_LENGTH];
	char		m_strItemName[64];
	char		m_strEtcInfo[64];

	cOGP_ServiceKey()
	{
		memset(m_strToken, 0, dTOKEN_LENGTH);
	}
};

#endif

//const	int	c_iMaxAvatarCount	=	4;

#define dPATH_LENGTH		1024
#define dMAX_WORKER_COUNT	8

#define _USE_BROADCAST
#define _USE_PROVISIONRANK
#define _USE_ACCOUNT_LEVEL
#define _USE_CARROTSHOP



enum
{
	eSTATE_STRENGTH	=	0,
	eSTATE_AGILITY		,
	eSTATE_CONSTITUTION	,
	eSTATE_INTELLIGENCE	,
	eSTATE_WISDOM	,
	eSTATE_CHARISMA	,
	eSTATE_LUCK		,
};

const	int	c_iStateTypeCount	=	eSTATE_LUCK+1;

#define	dSTATE_TYPE_COUNT			c_iStateTypeCount	//	const는 색깔이 안 변해서.. define으로 다시 정의함.
#define	dITEM_COOL_TIME_BUFFER_SIZE	12

enum
enServerType
{
	eSERVER_TYPE_NORMAL	=	0,
	eSERVER_TYPE_TEST		,
	eSERVER_TYPE_INNER_TEST	,
	eSERVER_TYPE_DUEL		,
	eSERVER_TYPE_GVG		,
	eSERVER_TYPE_OFF		,
};

class	CCritical
{
public:
	CRITICAL_SECTION	* cs;

	CCritical(CRITICAL_SECTION * _cs)
	{
		cs = _cs;
		EnterCriticalSection(cs);
	}

	~CCritical()
	{
		LeaveCriticalSection(cs);
	}
};

template<class T>
class cSinglton			
{
public:
	static T & GetInstance()	{	
		static T cMng;	return cMng;
	}
	
	static T & Inst()	{	
		static T cMng;	return cMng;
	}
};

class	cPacketBuff
{
public:
	char	binData[2048000];
	WORD	wPacketCount;
	DWORD	dwLoadPos,dwSavePos;
	CRITICAL_SECTION	cs;

				cPacketBuff()	{	InitializeCriticalSection(&cs);	reset();	}
				~cPacketBuff()	{	DeleteCriticalSection(&cs);					}
	inline void	reset()			{	memset(binData,0,2048000);	dwLoadPos = 0;	dwSavePos = 0;	wPacketCount = 0;	}
	BOOL	getPacket(void * data, WORD wSize)
	{	CCritical	CS(&cs);
		if(wPacketCount<=0)		return FALSE;
		memcpy(&wSize,binData+dwLoadPos,sizeof(WORD));
		memcpy(data,binData+dwLoadPos+sizeof(WORD),wSize);
		dwLoadPos += sizeof(WORD)+wSize;
		if(wSize == 0)	reset();
		else
		{
			wPacketCount--;
			if(wPacketCount == 0 )	reset();
		}
		return TRUE;
	}

	BOOL	addPacket(void * data, WORD	wSize)
	{	CCritical	CS(&cs);
		if(wSize + dwSavePos >= 2048000)	return FALSE;
		memcpy(binData+dwSavePos,&wSize,sizeof(WORD));
		memcpy(binData+dwSavePos+sizeof(WORD),data,wSize);
		dwSavePos += wSize+sizeof(WORD);
		wPacketCount++;
		return TRUE;
	}
};

///////////////////////////////////////////////////////
//		DEFINE	FOR SOCKET
///////////////////////////////////////////////////////
#ifdef	_FOR_JAPAN
	#define dDATABASEPORT					50605
	#define dUSERDBCACHE_PORT				51601
	#define dDBCACHE_PORT					52611
	#define	dWORLDSERVER_PORT				53621
	#define dGAMESERVER_PORT				54631
	#define dLOGINSERVER_PORT				55661
//	#define dLOGINSERVER_PORT				55671
//	#define dGATESERVER_PORT				55661
//	#define dCONTROLSERVER_PORT				52524
	#define dTOOLLOGSERVER_PORT				57651
	#define dLOGSERVER_PORT					59641
	#define dBROADCAST_PORT					56621	
#elif _FOR_THAI
	#define dDATABASEPORT					50605
	#define dUSERDBCACHE_PORT				51601
	#define dDBCACHE_PORT					52611
	#define	dWORLDSERVER_PORT				53621
	#define dGAMESERVER_PORT				54631
	#define dLOGINSERVER_PORT				55661
//	#define dLOGINSERVER_PORT				55671
//	#define dGATESERVER_PORT				55661
//	#define dCONTROLSERVER_PORT				52524
	#define dTOOLLOGSERVER_PORT				57651
	#define dLOGSERVER_PORT					59641
	#define dBROADCAST_PORT					56621	
#elif _FOR_USA
	#define dDATABASEPORT					50605
	#define dUSERDBCACHE_PORT				51601
	#define dDBCACHE_PORT					52611
	#define	dWORLDSERVER_PORT				53621
	#define dGAMESERVER_PORT				54631
	#define dLOGINSERVER_PORT				55661
//	#define dLOGINSERVER_PORT				55671
//	#define dGATESERVER_PORT				55661
//	#define dCONTROLSERVER_PORT				52524
	#define dTOOLLOGSERVER_PORT				57651
	#define dLOGSERVER_PORT					59641
	#define dBROADCAST_PORT					56621	
#elif _FOR_INTER
	#define dDATABASEPORT					40605
	#define dUSERDBCACHE_PORT				41601
	#define dDBCACHE_PORT					42611
	#define	dWORLDSERVER_PORT				43621
	#define dGAMESERVER_PORT				44631
	#define dLOGINSERVER_PORT				45661
//	#define dLOGINSERVER_PORT				45671
//	#define dGATESERVER_PORT				45661
//	#define dCONTROLSERVER_PORT				42524
	#define dTOOLLOGSERVER_PORT				47651
	#define dLOGSERVER_PORT					49641
	#define dBROADCAST_PORT					46621	
#elif _FOR_CHINA
	#define dDATABASEPORT					50505
	#define dUSERDBCACHE_PORT				50601
	#define dDBCACHE_PORT					50611
	#define	dWORLDSERVER_PORT				50621
	#define dGAMESERVER_PORT				50631
	#define dLOGINSERVER_PORT				50661
//	#define dLOGINSERVER_PORT				50681
//	#define dGATESERVER_PORT				50661
//	#define dCONTROLSERVER_PORT				56524
	#define dTOOLLOGSERVER_PORT				50651
	#define dLOGSERVER_PORT					50641
	#define dBROADCAST_PORT					50671
#elif _FOR_KOREA
	#define dDATABASEPORT					5316
	#define dUSERDBCACHE_PORT				51029
	#define dDBCACHE_PORT					50128
	#define	dWORLDSERVER_PORT				59227
	#define dGAMESERVER_PORT				58326
	#define dLOGINSERVER_PORT				9049
//	#define dLOGINSERVER_PORT				57425
//	#define dGATESERVER_PORT				9049
//	#define dCONTROLSERVER_PORT				56524
	#define dTOOLLOGSERVER_PORT				55623
	#define dLOGSERVER_PORT					53102
	#define dBROADCAST_PORT					57421	

#endif

#define dSERVER_AGENT_PORT					31529

///////////////////////////////////////////////////////
//		DEFINE	FOR CRYPT
///////////////////////////////////////////////////////
#define	dPROTECT_NONE							0			//	아무런 보호 장치가 없음
#define	dPROTECT_ORCRYPT						1			//	간단한 OR 암호화 한다.
#define	dPROTECT_SEED							2			//	SEED로 암호화 한다.
#define	dPROTECT_COMPRESSED						3			//	압축했다.

///////////////////////////////////////////////////////
//		DEFINE	FOR USER CLASS
///////////////////////////////////////////////////////
#ifdef _FOR_INTER	
	#define dID_LENGTH						64	//	USER의 아이�E길이 
#else
	#define dID_LENGTH						20	//	USER의 아이�E길이 
#endif
#define	dPASS_LENGTH						20	//	패스워�E길이
#define	dNAME_LENGTH						18	//	아이�E길이
#define dCDKEY_LENGTH						18	//	CDKEY 길이 
#define	dIP_SIZE							16	//	주소

#define	dMAX_AVATAR_COUNT					6	//	최�E생성 가능한 아바타 �E//	16개 인데.. rs2 문제로 dB 최적화, 패킷 최적화를 하�E못해 �E� 6개만 지�E
#define	dDEFAULT_AVATAR_COUNT				6	//	기본 생성 가능한 아바타 수�E
#define dMAX_AVATAR_DATA_SIZE				6000//	DB에 저장되는 바이너리 최�E사이�E
///////////////////////////////////////////////////////
//		DEFINE	FOR COMMUNITY
///////////////////////////////////////////////////////
#define dCHAT_LENGTH					128	//	채팅 메세�E길이

///////////////////////////////////////////////////////
//		DEFINE	FOR GUILD
///////////////////////////////////////////////////////
#define dGUILD_MAX_COUNT				4000
#define dGUILD_COUNT_FORPACKET			3
#define dGUILD_NAME_LENGTH				34	//GUILD NAME 최�E길이

#define dGUILD_CLASS_NOT				0	//길드가 없다.
#define dGUILD_CLASS_ROOKIE				1	//신입회�E
#define	dGUILD_CLASS_MEMBER				2	//정회�E
#define dGUILD_CLASS_PARLIAMENT			3	//의�E
#define dGUILD_CLASS_SUBMASTER			4	//부 마스터
#define dGUILD_CLASS_ELDER				5	//원로 
#define dGUILD_CLASS_MASTER				6	//마스터

#define dGUILD_MIN_PLAYER				10	//처음 10뫄寡�E모집이 가능하다.
#define dGUILD_UP_PLAYER				10	//렙업시 포인트를 사�E臼� 최�E모집인원을 이 수치만큼 올릴�E있다.
#define dGUILD_MAX_PLAYER				200	//길드의 최�E모집인�E- DB SP수정할 것!! - 가입부분

#define dGUILD_MIN_SUBMASTER			1	//서버마스터는 처음에 이수치만큼 둘�E있다. 마스터가 지정
#define dGUILD_UP_SUBMASTER				1	//렙업시 포인트를 사�E臼� 최�E서�E떼�痼恝坪� 이 수치만큼 올릴�E있다.
#define dGUILD_MAX_SUBMASTER			25	//서�E떽뵀痼� 최�E인�E
#define dGUILD_MIN_CONGRESSMAN			0	//의원의 초�E설정가능한 인�E
#define dGUILD_UP_CONGRESSMAN			5	//렙업시 포인트를 사�E臼� 최�E의원수를 이 수치만큼 올릴�E있다.
#define dGUILD_MAX_CONGRESSMAN			100	//의원의 최�E모집인�E
#define dGUILD_MIN_LEVEL				0	//길드레벨의 시작수치
#define dGUILD_MAX_LEVEL				200	//길드레벨의 최�E穉�

#define dGUILD_MIN_SKILL				1	//길�E스킬의 시작수치
#define dGUILD_UP_SKILL					3	//렙업시 포인트를 사�E臼� 최�E스킬수를 이 수치만큼 올릴�E있다.
#define dGUILD_MAX_SKILL				50	//최�E길�E스킬 �E

#define dGUILD_MIN_INVEN				48	//길드의 초�E인벤토리 �E
#define dGUILD_UP_INVEN					10	//렙업시 포인트를 사�E臼� 최�E인벤토리 수를 이 수치만큼 올릴�E있다.
#define dGUILD_MAX_INVEN				200	//최�E길�E인벤토리 �E
#define dGUILD_MIN_GROUND				1	//기본으로 하나 주엉碑다. (이벤트에 따�E하나가 정해진다.)
#define dGUILD_UP_GROUND				1	//렙업시 포인트를 사�E臼� 최�E연과�E수를 이 수치만큼 올릴�E있다.
#define dGUILD_MAX_GROUND				5	//최�E연과�E�E
#define dGUILD_MAX_HALL					1	//하나의 홀로 �E構E, 연과痺가 늘엉廐�E출구가 늘엉救다. 

#define dGUILD_DORMANCYLIMITPERIOD_FORGUILDMASTER	30	//	day : 이 기간동안 마스터가 접속하�E않으�E 다른 유저에게 마스터 권한이 넘엉黔다.
#define dGUILD_MINLIMITPERIOD_FORCHANGINGELDER		30	//	day : 이 기간이�E마스터를 지낸 길드원은 추후 원로로 추�E홱�.
#define dGUILD_DORMANCYLIMITPERIOD_FORGUILD			45	//	day : 이 기간동안 길드에 속한 엉冷 유저도 접속하�E않는다�E길드는 삭제된다.

///////////////////////////////////////////////////////
//		DEFINE	FOR GAME PROCESS SERVER
///////////////////////////////////////////////////////
#define dBC_MAX_USER_COUNT				8196


///////////////////////////////////////////////////////
//		DEFINE	FOR PARTY
///////////////////////////////////////////////////////
#define dMAX_PARTY_COUNT				1000
#define dPARTYNAME_LENGTH				22	//	파티이름 길이
#define dPARTY_MAXPLAYER				8	//	파티의 최�E인원�E
#define dPARTYLIST_COUNT				8	//	한번에 보내는 파티 리스트 �E

///////////////////////////////////////////////////////
//		DEFINE	FOR LOG SERVER
///////////////////////////////////////////////////////
#define dLOG_MAX_USER_COUNT				128		//LOG서버에서 한번에 수�E〈� 서버�E
///////////////////////////////////////////////////////
//		DEFINE	FOR LOGIN SERVER
///////////////////////////////////////////////////////
#define dLOGIN_MAX_USER_COUNT			5000	//LOGIN서버에서 한번에 수�E〈� USER�E
#define dNOTICE_MAX_LENGTH				4000	//공�E최�E길이 - db의 최�EVARCHAR�E길이는 4000

///////////////////////////////////////////////////////
//		DEFINE	FOR GAME PROCESS SERVER
///////////////////////////////////////////////////////
#define dGAME_MAX_USER_COUNT			2300	//게임 서버에서 한번에 수�E〈� USER�E

#ifdef _FOR_CHINA
#define dGAME_MAX_FIELD_COUNT 2048
#elif _FOR_JAPAN
#define dGAME_MAX_FIELD_COUNT 2048
#elif _FOR_USA
#define dGAME_MAX_FIELD_COUNT 2048
#else
#define dGAME_MAX_FIELD_COUNT 2048
#endif

///////////////////////////////////////////////////////
//		DEFINE	FOR WORLD MANAGEMENT SERVER
///////////////////////////////////////////////////////
#define dWORLD_NAME_LENGTH				32
#define dWORLD_MAX_USER_COUNT			32		//게임서버다 -_-a + admin 
#define dWORLD_MAX_ALLUSER_COUNT		8000	//한 월드�E수�E〈� 인�E
///////////////////////////////////////////////////////
//		DEFINE	FOR DB MANAGEMENT SERVER
///////////////////////////////////////////////////////
#define dDB_MAX_USER_COUNT				16		//월�E서버만...
#define dUSERDB_MAX_USER_COUNT			16		//월�E서버만...
///////////////////////////////////////////////////////
//		DEFINE	FOR PACKET
///////////////////////////////////////////////////////
#define	dMAX_PACKET_LENGTH				8196		//클라이언트와의 패킷 버퍼의 길이 
#define	dMAX_PACKET_LENGTH_SERVER		256000		//서버간의 패킷 버퍼의 길이 

///////////////////////////////////////////////////////
//		DEFINE	FOR PROFILE
///////////////////////////////////////////////////////
extern	char	g_strGameServerName[dWORLD_NAME_LENGTH];
extern DebugTracer		* cLOG;
#define	_log			cLOG->output
#define _logc			cLOG->outputc
#define _logf			cLOG->outputf
extern void	ProcessSendPacket(int serial);
#define	CM				CClientManager::GetInstance()

//extern CTracerFromSaveLog	* _SLOG;
#define	_sLog			_SLOG->print

const int	c_iDBControlledEvnetCount = 256;	//	DB로 제엉豺는 이벤트 �E
///////////////////////////////////////////////////////
//		DEFINE	FOR ADMIN TOOL
///////////////////////////////////////////////////////
#define dMAX_WORLD_SERVER_COUNT			20	//	최�E월�E서�E갯�E
#define dMAX_ADMIN_COUNT				10

typedef struct 
{
	WORD	wIndex;	//	과鼈 인덱스
	char	strName[dGUILD_NAME_LENGTH];
	int		iLevel;
	int		iHallLevel;
	char	strMasterName[dNAME_LENGTH];
	int		iTargetGuild;
	int		iInvadeGuild;
	int		iGuildPoint;
	int		iVicPoint;
	int		iVicContinue;
	int		iWin;
	int		iLost;
	int		iDraw;
	int		iCancel;
}GUILDINFO_FORADMIN;	//	admin 툴에서 사�E� 정보


///////////////////////////////////////////////////////
//		DEFIEN MEMORY CHECK
/*class MEMCHECK
{
public:
	inline static void	PrintMemory(HANDLE _hProcess, char * _strTitle)
	{
		PROCESS_MEMORY_COUNTERS	stat;
		GetProcessMemoryInfo (_hProcess,&stat,sizeof(PROCESS_MEMORY_COUNTERS));
		printf ("[%s] Using Memory : %ld Kbytes\n",_strTitle,stat.PagefileUsage );
	}
	inline static void	LogMemory(char * _strTitle)
	{
		MEMORYSTATUS stat;
		GlobalMemoryStatus (&stat);	
		_log ("[%s] Using Memory : %ld Kbytes\n",_strTitle,stat.dwTotalPageFile/1024-stat.dwAvailPageFile/1024);
	}
};
*/


////////////////////////////////////////////////////////////////////////////////////////////////////	
//	결투�E�E� 정보 정의
////////////////////////////////////////////////////////////////////////////////////////////////////	

const	int	c_iDuelServerLobyIndex		=	900;	//	결투�E서�E로�E필�E인덱스
const	int	c_iDefaultGoldForDuelPlayer	=	100;
const	int	c_iLevelForDuelServer		=	300;	//	결투서버에서의 캐릭터 레벨

class	cDuelRecord
{
public:
	int	m_iTrialGameWinPoint;//	예선�E승점 : 
	int	m_iDuelPoint;	//	결�E포인트 : 보�E지급�E
	int	m_iBattleCount;	//	�EE횟�E	
	int	m_iWinCount;	//	승리 횟�E	
	int	m_iDefeatCount;	//	패�E횟�E	
	int	m_iDrawCount;	//	무승부 횟�E
	int	m_iWeeklyWinPoint;		//	주간 결�E승점
	int	m_iWeeklyBattleCount;	//	주간 결�E횟�E	
	int	m_iWeeklyWinCount;		//	주간 결�E승리 횟�E	
	int	m_iWeeklyDefeatCount;	//	주간 결�E패�E횟�E	
	int	m_iWeeklyDrawCount;		//	주간 결�E무승부 횟�E
	char	m_strID[dID_LENGTH];
	char	m_strName[dNAME_LENGTH];
	WORD	m_wJob;
};

//	여기에 결투팀 정보를 담을 때는 
//	m_strID는 비�EE
//	m_strName에 팀 이름을 넣는다.
class	cDuelRankingInfo
{
public:
	char	m_strID[dID_LENGTH];
	char	m_strName[dNAME_LENGTH];
	WORD	m_wJob,m_wRank;

	int		m_iWinPoint;	//	승점
	int		m_iBattleCount;	//	�EE횟�E	
	int		m_iWinCount;	//	승리 횟�E	
	int		m_iDefeatCount;	//	패�E횟�E	
	int		m_iDrawCount;	//	무승부 횟�E
};

const	int	c_iDuelTeamMaxCount		=	1000;
const	int	c_iDuelTeamNameLength	=	18;
const	int	c_iDuelTeamMemberCount	=	5;

class	cDuelTeamBattlerInfo
{
public:
	char	m_strID[dID_LENGTH];		//	계정
	char	m_strName[dNAME_LENGTH];	//	이름
	WORD	m_wJob;	//	직�E
};

class	cDuelTeamRecord
{
public:
	int		m_iWinPoint;	//	승점

	int		m_iBattleCount;	//	�EE횟�E	
	int		m_iWinCount;	//	승리 횟�E	
	int		m_iDefeatCount;	//	패�E횟�E	
	int		m_iDrawCount;	//	무승부 횟�E
};

class	cDuelTeamInfo
{
public:
	WORD	m_wIndex;	//	인덱스
	char	m_strTeamName[c_iDuelTeamNameLength];	//	팀이름
	WORD	m_wMemberCount;	//	팀에 속한 멤�E�E
	cDuelTeamRecord			m_record;

	cDuelTeamBattlerInfo	m_aMembers[c_iDuelTeamMemberCount];	//	멤�E정보
};

enum 
{
	eWeeklyRanking,
	eDailyRanking,
	eTrialGameRanking,
};

enum
{
	eNationKorea	= 0,
	eNationJapan,
	eNationChina,
	eNationKoreaNotUsed,
	eNationInter,
	eNationUsa,
	eNationThai,
};


#endif	//	__REDSTONE_COMMON_H