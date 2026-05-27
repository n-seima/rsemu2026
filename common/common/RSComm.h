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
#define	_OGP_SERVICE	1	//	∆€∫˙‘Æº≈∞° OGP¥Ÿ
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

#define	dSTATE_TYPE_COUNT			c_iStateTypeCount	//	const¥¬ ªˆ±Ú¿Ã æ» ∫Ø«ÿº≠.. define¿∏∑Œ ¥ŸΩ√ ¡§¿««‘.
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
#define	dPROTECT_NONE							0			//	æ∆π´∑± ∫∏»£ ¿Âƒ°∞° æ¯¿Ω
#define	dPROTECT_ORCRYPT						1			//	∞£¥‹«— OR æœ»£»≠ «—¥Ÿ.
#define	dPROTECT_SEED							2			//	SEED∑Œ æœ»£»≠ «—¥Ÿ.
#define	dPROTECT_COMPRESSED						3			//	æ–√‡«ﬂ¥Ÿ.

///////////////////////////////////////////////////////
//		DEFINE	FOR USER CLASS
///////////////////////////////////////////////////////
#ifdef _FOR_INTER	
	#define dID_LENGTH						64	//	USER¿« æ∆¿ÃµÅE±Ê¿Ã 
#else
	#define dID_LENGTH						20	//	USER¿« æ∆¿ÃµÅE±Ê¿Ã 
#endif
#define	dPASS_LENGTH						20	//	∆–Ω∫øˆµÅE±Ê¿Ã
#define	dNAME_LENGTH						18	//	æ∆¿ÃµÅE±Ê¿Ã
#define dCDKEY_LENGTH						18	//	CDKEY ±Ê¿Ã 
#define	dIP_SIZE							16	//	¡÷º“

#define	dMAX_AVATAR_COUNT					6	//	√÷¥ÅEª˝º∫ ∞°¥…«— æ∆πŸ≈∏ ºÅE//	16∞≥ ¿Œµ•.. rs2 πÆ¡¶∑Œ dB √÷¿˚»≠, ∆–≈∂ √÷¿˚»≠∏¶ «œ¡ÅE∏¯«ÿ øÅE± 6∞≥∏∏ ¡ˆøÅE
#define	dDEFAULT_AVATAR_COUNT				6	//	±‚∫ª ª˝º∫ ∞°¥…«— æ∆πŸ≈∏ ºˆ¿ÅE
#define dMAX_AVATAR_DATA_SIZE				6000//	DBø° ¿˙¿Âµ«¥¬ πŸ¿Ã≥ ∏Æ √÷¥ÅEªÁ¿Ã¡ÅE
///////////////////////////////////////////////////////
//		DEFINE	FOR COMMUNITY
///////////////////////////////////////////////////////
#define dCHAT_LENGTH					128	//	√§∆√ ∏ﬁºº¡ÅE±Ê¿Ã

///////////////////////////////////////////////////////
//		DEFINE	FOR GUILD
///////////////////////////////////////////////////////
#define dGUILD_MAX_COUNT				4000
#define dGUILD_COUNT_FORPACKET			3
#define dGUILD_NAME_LENGTH				34	//GUILD NAME √÷¥ÅE±Ê¿Ã

#define dGUILD_CLASS_NOT				0	//±ÊµÂ∞° æ¯¥Ÿ.
#define dGUILD_CLASS_ROOKIE				1	//Ω≈¿‘»∏øÅE
#define	dGUILD_CLASS_MEMBER				2	//¡§»∏øÅE
#define dGUILD_CLASS_PARLIAMENT			3	//¿«øÅE
#define dGUILD_CLASS_SUBMASTER			4	//∫Œ ∏∂Ω∫≈Õ
#define dGUILD_CLASS_ELDER				5	//ø¯∑Œ 
#define dGUILD_CLASS_MASTER				6	//∏∂Ω∫≈Õ

#define dGUILD_MIN_PLAYER				10	//√≥¿Ω 10∏˙Õ˚›ÅE∏¡˝¿Ã ∞°¥…«œ¥Ÿ.
#define dGUILD_UP_PLAYER				10	//∑ææ˜Ω√ ∆˜¿Œ∆Æ∏¶ ªÁøÅEœø© √÷¥ÅE∏¡˝¿Œø¯¿ª ¿Ã ºˆƒ°∏∏≈≠ ø√∏±ºÅE¿÷¥Ÿ.
#define dGUILD_MAX_PLAYER				200	//±ÊµÂ¿« √÷¥ÅE∏¡˝¿ŒøÅE- DB SPºˆ¡§«“ ∞Õ!! - ∞°¿‘∫Œ∫–

#define dGUILD_MIN_SUBMASTER			1	//º≠πˆ∏∂Ω∫≈Õ¥¬ √≥¿Ωø° ¿Ãºˆƒ°∏∏≈≠ µ—ºÅE¿÷¥Ÿ. ∏∂Ω∫≈Õ∞° ¡ˆ¡§
#define dGUILD_UP_SUBMASTER				1	//∑ææ˜Ω√ ∆˜¿Œ∆Æ∏¶ ªÁøÅEœø© √÷¥ÅEº≠∫ÅE∂º≠≈Õ¿Œø¯¿ª ¿Ã ºˆƒ°∏∏≈≠ ø√∏±ºÅE¿÷¥Ÿ.
#define dGUILD_MAX_SUBMASTER			25	//º≠∫ÅE∂Ω∫≈Õ¿« √÷¥ÅE¿ŒøÅE
#define dGUILD_MIN_CONGRESSMAN			0	//¿«ø¯¿« √ ±ÅEº≥¡§∞°¥…«— ¿ŒøÅE
#define dGUILD_UP_CONGRESSMAN			5	//∑ææ˜Ω√ ∆˜¿Œ∆Æ∏¶ ªÁøÅEœø© √÷¥ÅE¿«ø¯ºˆ∏¶ ¿Ã ºˆƒ°∏∏≈≠ ø√∏±ºÅE¿÷¥Ÿ.
#define dGUILD_MAX_CONGRESSMAN			100	//¿«ø¯¿« √÷¥ÅE∏¡˝¿ŒøÅE
#define dGUILD_MIN_LEVEL				0	//±ÊµÂ∑π∫ß¿« Ω√¿€ºˆƒ°
#define dGUILD_MAX_LEVEL				200	//±ÊµÂ∑π∫ß¿« √÷¥ÅEˆƒ°

#define dGUILD_MIN_SKILL				1	//±ÊµÅEΩ∫≈≥¿« Ω√¿€ºˆƒ°
#define dGUILD_UP_SKILL					3	//∑ææ˜Ω√ ∆˜¿Œ∆Æ∏¶ ªÁøÅEœø© √÷¥ÅEΩ∫≈≥ºˆ∏¶ ¿Ã ºˆƒ°∏∏≈≠ ø√∏±ºÅE¿÷¥Ÿ.
#define dGUILD_MAX_SKILL				50	//√÷¥ÅE±ÊµÅEΩ∫≈≥ ºÅE

#define dGUILD_MIN_INVEN				48	//±ÊµÂ¿« √ ±ÅE¿Œ∫•≈‰∏Æ ºÅE
#define dGUILD_UP_INVEN					10	//∑ææ˜Ω√ ∆˜¿Œ∆Æ∏¶ ªÁøÅEœø© √÷¥ÅE¿Œ∫•≈‰∏Æ ºˆ∏¶ ¿Ã ºˆƒ°∏∏≈≠ ø√∏±ºÅE¿÷¥Ÿ.
#define dGUILD_MAX_INVEN				200	//√÷¥ÅE±ÊµÅE¿Œ∫•≈‰∏Æ ºÅE
#define dGUILD_MIN_GROUND				1	//±‚∫ª¿∏∑Œ «œ≥™ ¡÷æ˚›¯¥Ÿ. (¿Ã∫•∆Æø° µ˚∂ÅE«œ≥™∞° ¡§«ÿ¡¯¥Ÿ.)
#define dGUILD_UP_GROUND				1	//∑ææ˜Ω√ ∆˜¿Œ∆Æ∏¶ ªÁøÅEœø© √÷¥ÅEø¨∞˙›ÅEºˆ∏¶ ¿Ã ºˆƒ°∏∏≈≠ ø√∏±ºÅE¿÷¥Ÿ.
#define dGUILD_MAX_GROUND				5	//√÷¥ÅEø¨∞˙›ÅEºÅE
#define dGUILD_MAX_HALL					1	//«œ≥™¿« »¶∑Œ ≈ÅEœ∞ÅE, ø¨∞˙›ˆ∞° ¥√æ˚œ™∏ÅE√‚±∏∞° ¥√æ˚œ≠¥Ÿ. 

#define dGUILD_DORMANCYLIMITPERIOD_FORGUILDMASTER	30	//	day : ¿Ã ±‚∞£µøæ» ∏∂Ω∫≈Õ∞° ¡¢º”«œ¡ÅEæ ¿∏∏ÅE ¥Ÿ∏• ¿Ø¿˙ø°∞‘ ∏∂Ω∫≈Õ ±««—¿Ã ≥—æ˚Ã£¥Ÿ.
#define dGUILD_MINLIMITPERIOD_FORCHANGINGELDER		30	//	day : ¿Ã ±‚∞£¿ÃªÅE∏∂Ω∫≈Õ∏¶ ¡ˆ≥Ω ±ÊµÂø¯¿∫ √ﬂ»ƒ ø¯∑Œ∑Œ √ﬂ¥ÅE»¥Ÿ.
#define dGUILD_DORMANCYLIMITPERIOD_FORGUILD			45	//	day : ¿Ã ±‚∞£µøæ» ±ÊµÂø° º”«— æ˚“≤ ¿Ø¿˙µµ ¡¢º”«œ¡ÅEæ ¥¬¥Ÿ∏ÅE±ÊµÂ¥¬ ªË¡¶µ»¥Ÿ.

///////////////////////////////////////////////////////
//		DEFINE	FOR GAME PROCESS SERVER
///////////////////////////////////////////////////////
#define dBC_MAX_USER_COUNT				8196


///////////////////////////////////////////////////////
//		DEFINE	FOR PARTY
///////////////////////////////////////////////////////
#define dMAX_PARTY_COUNT				1000
#define dPARTYNAME_LENGTH				22	//	∆ƒ∆º¿Ã∏ß ±Ê¿Ã
#define dPARTY_MAXPLAYER				8	//	∆ƒ∆º¿« √÷¥ÅE¿Œø¯ºÅE
#define dPARTYLIST_COUNT				8	//	«—π¯ø° ∫∏≥ª¥¬ ∆ƒ∆º ∏ÆΩ∫∆Æ ºÅE

///////////////////////////////////////////////////////
//		DEFINE	FOR LOG SERVER
///////////////////////////////////////////////////////
#define dLOG_MAX_USER_COUNT				128		//LOGº≠πˆø°º≠ «—π¯ø° ºˆøÅE°¥…«— º≠πˆºÅE
///////////////////////////////////////////////////////
//		DEFINE	FOR LOGIN SERVER
///////////////////////////////////////////////////////
#define dLOGIN_MAX_USER_COUNT			5000	//LOGINº≠πˆø°º≠ «—π¯ø° ºˆøÅE°¥…«— USERºÅE
#define dNOTICE_MAX_LENGTH				4000	//∞¯¡ÅE√÷¥ÅE±Ê¿Ã - db¿« √÷¥ÅEVARCHAR«ÅE±Ê¿Ã¥¬ 4000

///////////////////////////////////////////////////////
//		DEFINE	FOR GAME PROCESS SERVER
///////////////////////////////////////////////////////
#define dGAME_MAX_USER_COUNT			2300	//∞‘¿” º≠πˆø°º≠ «—π¯ø° ºˆøÅE°¥…«— USERºÅE

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
#define dWORLD_MAX_USER_COUNT			32		//∞‘¿”º≠πˆ¥Ÿ -_-a + admin 
#define dWORLD_MAX_ALLUSER_COUNT		8000	//«— ø˘µÂ¥ÅEºˆøÅE°¥…«— ¿ŒøÅE
///////////////////////////////////////////////////////
//		DEFINE	FOR DB MANAGEMENT SERVER
///////////////////////////////////////////////////////
#define dDB_MAX_USER_COUNT				16		//ø˘µÅEº≠πˆ∏∏...
#define dUSERDB_MAX_USER_COUNT			16		//ø˘µÅEº≠πˆ∏∏...
///////////////////////////////////////////////////////
//		DEFINE	FOR PACKET
///////////////////////////////////////////////////////
#define	dMAX_PACKET_LENGTH				8196		//≈¨∂Û¿Ãæ∆ÆøÕ¿« ∆–≈∂ πˆ∆€¿« ±Ê¿Ã 
#define	dMAX_PACKET_LENGTH_SERVER		256000		//º≠πˆ∞£¿« ∆–≈∂ πˆ∆€¿« ±Ê¿Ã 

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

const int	c_iDBControlledEvnetCount = 256;	//	DB∑Œ ¡¶æ˚„œ¥¬ ¿Ã∫•∆Æ ºÅE
///////////////////////////////////////////////////////
//		DEFINE	FOR ADMIN TOOL
///////////////////////////////////////////////////////
#define dMAX_WORLD_SERVER_COUNT			20	//	√÷¥ÅEø˘µÅEº≠πÅE∞πºÅE
#define dMAX_ADMIN_COUNT				10

typedef struct 
{
	WORD	wIndex;	//	∞˙‹Ø ¿Œµ¶Ω∫
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
}GUILDINFO_FORADMIN;	//	admin ≈¯ø°º≠ ªÁøÅE“ ¡§∫∏


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
//	∞·≈ı¿ÅE∞ÅE√ ¡§∫∏ ¡§¿«
////////////////////////////////////////////////////////////////////////////////////////////////////	

const	int	c_iDuelServerLobyIndex		=	900;	//	∞·≈ı¿ÅEº≠πÅE∑Œ∫ÅE« µÅE¿Œµ¶Ω∫
const	int	c_iDefaultGoldForDuelPlayer	=	100;
const	int	c_iLevelForDuelServer		=	300;	//	∞·≈ıº≠πˆø°º≠¿« ƒ≥∏Ø≈Õ ∑π∫ß

class	cDuelRecord
{
public:
	int	m_iTrialGameWinPoint;//	øπº±¿ÅEΩ¬¡° : 
	int	m_iDuelPoint;	//	∞·≈ÅE∆˜¿Œ∆Æ : ∫∏ªÅE¡ˆ±ﬁøÅE
	int	m_iBattleCount;	//	¿ÅEÅE»ΩºÅE	
	int	m_iWinCount;	//	Ω¬∏Æ »ΩºÅE	
	int	m_iDefeatCount;	//	∆–πÅE»ΩºÅE	
	int	m_iDrawCount;	//	π´Ω¬∫Œ »ΩºÅE
	int	m_iWeeklyWinPoint;		//	¡÷∞£ ∞·≈ÅEΩ¬¡°
	int	m_iWeeklyBattleCount;	//	¡÷∞£ ∞·≈ÅE»ΩºÅE	
	int	m_iWeeklyWinCount;		//	¡÷∞£ ∞·≈ÅEΩ¬∏Æ »ΩºÅE	
	int	m_iWeeklyDefeatCount;	//	¡÷∞£ ∞·≈ÅE∆–πÅE»ΩºÅE	
	int	m_iWeeklyDrawCount;		//	¡÷∞£ ∞·≈ÅEπ´Ω¬∫Œ »ΩºÅE
	char	m_strID[dID_LENGTH];
	char	m_strName[dNAME_LENGTH];
	WORD	m_wJob;
};

//	ø©±‚ø° ∞·≈ı∆¿ ¡§∫∏∏¶ ¥„¿ª ∂ß¥¬ 
//	m_strID¥¬ ∫ÒøÅEÅE
//	m_strNameø° ∆¿ ¿Ã∏ß¿ª ≥÷¥¬¥Ÿ.
class	cDuelRankingInfo
{
public:
	char	m_strID[dID_LENGTH];
	char	m_strName[dNAME_LENGTH];
	WORD	m_wJob,m_wRank;

	int		m_iWinPoint;	//	Ω¬¡°
	int		m_iBattleCount;	//	¿ÅEÅE»ΩºÅE	
	int		m_iWinCount;	//	Ω¬∏Æ »ΩºÅE	
	int		m_iDefeatCount;	//	∆–πÅE»ΩºÅE	
	int		m_iDrawCount;	//	π´Ω¬∫Œ »ΩºÅE
};

const	int	c_iDuelTeamMaxCount		=	1000;
const	int	c_iDuelTeamNameLength	=	18;
const	int	c_iDuelTeamMemberCount	=	5;

class	cDuelTeamBattlerInfo
{
public:
	char	m_strID[dID_LENGTH];		//	∞Ë¡§
	char	m_strName[dNAME_LENGTH];	//	¿Ã∏ß
	WORD	m_wJob;	//	¡˜æÅE
};

class	cDuelTeamRecord
{
public:
	int		m_iWinPoint;	//	Ω¬¡°

	int		m_iBattleCount;	//	¿ÅEÅE»ΩºÅE	
	int		m_iWinCount;	//	Ω¬∏Æ »ΩºÅE	
	int		m_iDefeatCount;	//	∆–πÅE»ΩºÅE	
	int		m_iDrawCount;	//	π´Ω¬∫Œ »ΩºÅE
};

class	cDuelTeamInfo
{
public:
	WORD	m_wIndex;	//	¿Œµ¶Ω∫
	char	m_strTeamName[c_iDuelTeamNameLength];	//	∆¿¿Ã∏ß
	WORD	m_wMemberCount;	//	∆¿ø° º”«— ∏‚πÅEºÅE
	cDuelTeamRecord			m_record;

	cDuelTeamBattlerInfo	m_aMembers[c_iDuelTeamMemberCount];	//	∏‚πÅE¡§∫∏
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