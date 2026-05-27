#include "cGAME.H"
#include "cFIELD.H"
#include "cPROJECT.H"
#include "STDAFX.H"
#include "cDECODER.H"
#include "time.H"
#include "cPACKET_GAMESERVER.h"
#include "CImageData.h"
#include "CDataSave.h"
#include "CBank.h"
#include "CTitle.h"
#include "instance_guild_battle_manager.h"
#include "CGuildSkill.h"
#include "CShoppingCart.h"
#include "debugCode.h"
#include "CEvent.h"
#include "CPitchmanShop.h"
#include "trap.h"
#include "itemPackList.h"
#include "objectImage.h"
#include "secretDungeon.h"
#include "banner.h"
#include "cEvent.h"
//#include "buildVersion.h"
#include "stressTest.h"
#include "packetManager.h"
#include "booking_work.h"
#include "fireWork.h"
#include "userCustomDataDefine.h"
#include "carrotShop.h"
//#include "HackShield.h"
#include "cPACKET_BCSERVER.h"
#include "duel.h"
#include "version.h"
#include "reg_common.h"
#include "mess_sign.h"
#include "GameDLL.H"

#ifdef	_USE_XTRAP
#include "XTrap/Xtrap_S_Interface.h"
#endif

LONGLONG	g_allGettingGold[eGET_GOLD_TYPE_COUNT],g_allSpentGold[eSPENT_GOLD_TYPE_COUNT];

int			g_iServerIndex			=	0xffffffff;
int			g_iWorldServerType		=	0;

char		g_strServerInfoFileName[64];

cGAME		g_game;
CTimeInfo	g_currentTime;
int			g_iCurrentTimeMinuteValue	=	0;
int			g_iCurrentTimeSecondValue	=	0;
BOOL		g_bGameClose			=	FALSE;
BOOL		cGAME::s_isExist		=	FALSE;
int			g_iExperienceFactor		=	1;	//	я纂 ね攪
int			g_iItemFactor			=	1;
BOOL		g_bIsOpenedTestServer	=	FALSE;
BOOL		g_bIsUseRealPrice		=	FALSE;
BOOL		g_bIsDuelServer			=	FALSE;
BOOL		g_bIsChangeMinute		=	FALSE;
BOOL		g_bIsChangeHour			=	FALSE;
BOOL		g_bIsChangeDay			=	FALSE;
int			g_iSeasonVariable		=	0;

float		g_afDuelDamageFactor[dPLAYER_JOB_COUNT+1]	=
{
	2.00f, 2.00f , 
	2.00f , 2.00f ,
	2.00f , 2.00f ,
	2.00f , 2.00f ,
	2.00f , 2.00f ,
	2.00f , 2.00f ,
	2.00f , 2.00f ,
	2.00f , 2.00f ,
	2.00f , 2.00f ,
	2.00f , 2.00f ,
	2.00f
};

DWORD		g_dwCurrentTime;
int			g_iLastTimeByTimeGetTime=	0;
int			g_iCounterBySecond		=	0;
int			g_iCounterByMinute		=	0;
BOOL		g_bIsDisplayTick		=	TRUE;
int			g_iCurrentMoveNumber			=	0;
int			g_iSpecialMagicOptionDropChance	=	40;
BOOL		g_bIsWantPrintTriggerCondition	=	FALSE;

int			g_iServerSyncDate				=	20050823;
int			g_iServerSyncCounter			=	0;
int			g_iWorldServerIndex				=	0;

DWORD		g_dwRequireMajorVersion			=	295;	//	措 ⑷營
DWORD		g_dwMajorVersion;
DWORD		g_dwRequireUpdateCounter		=	3;
DWORD		g_dwUpdateCounter;

#ifdef	_USE_HS
BOOL		g_bIsUseHackShield				=	TRUE;
#else
BOOL		g_bIsUseHackShield				=	FALSE;
#endif

BOOL
cFIELD_INFO::isSurfaceField()
{
	if	(strlen(m_strName)	>	12)
		return	FALSE;

	char	*aSurfaceFieldChar	=	"TGMDSHA";
/*
T	擊
G	蟾
M	學
D	餌虞
S	餌
H	繭鼻
A	雖ж
*/
	for(int i=0;;i++)
	{
		if	(aSurfaceFieldChar[i]==NULL)
			return	FALSE;

		if	(m_strName[5]	==	aSurfaceFieldChar[i])
			return	TRUE;
	}

	return	FALSE;
}

void
cGAME::EmergencyWork(void *lpInfo)
{
	SetLogFolder();

	CreateDirectory("error sign",NULL);

	cFILE	file;

	file.Open(_ms("error sign/error sign [%.4d%.2d%.2d] [%.2d%.2d].dat",g_currentTime.m_wYear,g_currentTime.m_wMonth,g_currentTime.m_wDay,g_currentTime.m_wHour,g_currentTime.m_wMinute),"wb");
	file.Write(&g_debugSign,sizeof(g_debugSign));
	file.Close();

	file.Open("error sign/last error sign.dat","wb");
	file.Write(&g_debugSign,sizeof(g_debugSign));
	file.Close();

	g_game.urgentSave();
}

BOOL
cGAME::checkLastErrorSign()
{
#ifndef	_FOR_KOREA
	return	TRUE;
#endif

	cFILE	file;

	if	(file.Open("error sign/last error sign.dat","rb"))
	{
		if	(cMSG::YESNO(" 餌檣檜 氈肅","蛤斜 萄賊 溯檜 勘堅 羹 ж撮蹂.\n\n羹 ж褒辦?")	==	IDYES)
		{
			file.Read(&g_debugSign,sizeof(g_debugSign));
			file.Close();

			__asm
			{
				int	3
			}

			g_debugSign.reset();
		}
		else
		{
			file.Close();

			if	(cMSG::YESNO(" 餌檣"," 餌檣擊 雖辦衛蝗棲梱?\n\n 雖辦賊 憮 衛衛 檜 璽檜 樓 嗅棲.")	==	IDYES)
			{
				char	strFileName[512];

				sprintf(strFileName,"%s\\error sign\\last error sign.dat",g_strLogFolder);
				DeleteFile(strFileName);
			}
		}
	}

	return	TRUE;
}

void
CheckCS_PacketLog()
{
	if	(IsFile("bl.sonaki_bld")	==	FALSE)
		return;

	class	cPacket
	{
	public:
		char			*m_pBuff;
		WORD			wType;

		cPacket()
		{
			m_pBuff	=	NULL;
		}
		~cPacket()
		{
			pKILL(m_pBuff);
		}
		void	setData(int _iSize,void *_lpBuff)
		{
			pKILL(m_pBuff);
			m_pBuff	=	new char [_iSize];

			memcpy(m_pBuff,_lpBuff,_iSize);

			wType	=	getType();
		}
		WORD	getType()
		{
			return	*(WORD *)(m_pBuff+2);
		}
	};

	cPacket	*pPacket;
	int		iPacketCount	=	0;

	pPacket	=	new	cPacket [1024*1024];

	cFILE	file;

	if	(!file.Open("bl.sonaki_bld","rb"))
		return;

	char	*pTempBuff	=	new char [1024*1024];
	
	while(1)
	{
		cMSG_BASE_TYPE	base;

		if	(file.Locate()	>=	file.Length)
			break;

		if	(!file.Read(&base.wSize,2))
			break;
		if	(!file.Read(&base.wType,2))
			break;

		int	iHeaderSize	=	6;

		if	(base.wType	>=	0x8000)
			iHeaderSize	=	4;
		else
		if	(!file.Read(&base.wProtectType,2))
			break;

		memcpy(pTempBuff,&base,iHeaderSize);

		if	(iHeaderSize	<	base.wSize)
			if	(!file.Read(pTempBuff+iHeaderSize,base.wSize-iHeaderSize))
				break;

		cPacket	*lpPacket	=	&pPacket[iPacketCount];

		lpPacket->setData(base.wSize,pTempBuff);

		iPacketCount++;
	}

	file.Close();

	for (int iIndex=iPacketCount-1;iIndex;iIndex--)
	{
		char			*lpBuff	=	pPacket[iIndex].m_pBuff;
		cMSG_BASE_TYPE	*lpBase	=	(cMSG_BASE_TYPE	*)lpBuff;
//	GB_UPDATE_INFO			0x701b
//	CB_UPDATE_USER_ALLINFO
//	GB_UPDATE_INFO
//	WORLDtoSERVERS_DIS		0x8712	??
//	GB_GUILD_CHAT			0x7016
//	WB_UPDATE_PARTY_INFO	0x7053
		GB_UPDATE_INFO	*lpPacket	=	(GB_UPDATE_INFO	*)lpBuff;

		if	(file.Open("packet.dat","wb"))
		{
			file.Write(lpPacket,lpPacket->base.wSize);

			file.Close();
		}
	}

	pKILL(pTempBuff);
	pKILL(pPacket);
}

//
//	蟾晦
BOOL
cGAME::init(int _iZoneIndex)
{
	g_gsEncTable.build();

	memset(g_allGettingGold,0,sizeof(g_allGettingGold));
	memset(g_allSpentGold,0,sizeof(g_allSpentGold));

	int	i;

	g_currentTime.update();
	g_iCurrentTimeMinuteValue	=	g_currentTime.getMinuteValue();
	g_iCurrentTimeSecondValue	=	g_currentTime.getSecondValue();

	if	(g_bIsDuelServer)
		g_pDuelManager	=	new cDuelManager();

	m_dwThisTimeReceivePacketSizeSum	=	0;
	m_dwThisTimeReceivePacketCount		=	0;
	m_dwLastCheckReceivePacketSizeTime	=	0;
	m_dwMaxReceivePacketSizePerMinute	=	0;
	m_dwMaxReceivePacketCountPerMinute	=	0;
	m_dwCountOfCheckReceivePacketSize	=	0;
	m_dwSumOfReceivePacketSizePerMinute	=	0;
	m_dwCountOfReceivePacketSizePerMinute=	0;

	m_dwThisTimeSendPacketSizeSum		=	0;
	m_dwThisTimeSendPacketCount			=	0;
	m_dwLastCheckSendPacketSizeTime		=	0;
	m_dwMaxSendPacketSizePerMinute		=	0;
	m_dwMaxSendPacketCountPerMinute		=	0;
	m_dwCountOfCheckSendPacketSize		=	0;
	m_dwSumOfSendPacketSizePerMinute	=	0;
	m_dwCountOfSendPacketSizePerMinute	=	0;

	g_userPM.init();

	FPInitCS(&m_csActorManager);	//	葬じ 播暮 蟾晦

	g_eh.setLauncher(EmergencyWork);
	g_eh.initLog(1024*1024);	//	1詭陛.. -o-

	m_bIsBookedBanishAllUser		=	FALSE;
	m_iOncePerMinuteWorkBeginIndex	=	-1;
	m_iOncePer10SecondWorkBeginIndex=	-1;
	m_iOncePerSecondWorkBeginIndex	=	-1;
	m_iOncePerMinuteWorkLastestOperateTime	=	0;
	m_iOncePer10SecondWorkLastestOperateTime=	0;
	g_iCounterBySecond				=	0;
	m_iFieldCount					=	0;
	m_iPlayerCount					=	0;
	memset(m_iCountReserve, 0x00, sizeof(int)*3);
	m_iMaxConnectedPlayerCount		=	0;

	m_profiler.reset();

//	cTIMER::Init();//	顫檜該 蟾晦
	cMSG::Init();

	g_bankManager.init();
	g_cartManager.init();
	g_guildManager.reset();
	g_pitchmanShopManager.init();
	g_pTrapManager->init();
	g_fireWorkManager.reset();
	g_itemPackList.reset();
	g_carrotShopInfo.reset();

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早ぬ婁  檜 薑曖脹 掘褻羹曖 餌檜鍔 羹и.
	printf("\n game data initialize and data loading \n");
	printf("* check packet and game structure size\n");

	initDBPacketReceiver();
	initBCSPacketReceiver();
	initWorldPacketReceiver();

	printf("sizeof cACTOR : %d\n",sizeof(cACTOR));
	printf("sizeof CSkill : %d\n",sizeof(CSkill));
	printf("sizeof CActorEquipment : %d\n",sizeof(CActorEquipment));
	printf("sizeof CAttackInfo : %d\n",sizeof(CAttackInfo));
	printf("sizeof ALL_MSG : %d\n",sizeof(ALL_MSG));
	
	printf("sizeof g_bankManager : %d\n",sizeof(CBank)*dMAX_OPEN_BANK);
	printf("sizeof g_LogQue : %d\n",sizeof(g_LogQue));
	printf("sizeof CGuildBaseInfo : %d\n",sizeof(CGuildBaseInfo));
	printf("sizeof CGuildAdvanceInfo : %d\n",sizeof(CGuildAdvanceInfo));
	
	printf("sizeof cPLAYER_SAVE_DATA_FOR_SERVER : %d\n",sizeof(cPLAYER_SAVE_DATA_FOR_SERVER));
	printf("sizeof CPlayerSaveDataForServerDefine : %d\n",sizeof(CPlayerSaveDataForServerDefine));

	printf("sizeof cPLAYER_SAVE_DATA_BASE_FOR_CLIENT : %d\n",sizeof(cPLAYER_SAVE_DATA_BASE_FOR_CLIENT));
	printf("sizeof CPlayerSaveDataForClientDefine : %d\n",sizeof(CPlayerSaveDataForClientDefine));
//	printf("sizeof cP_CORRECT_ACTOR_STATUS_DATA : %d,cCORRECT_ACTOR_STATUS_DATA : %d\n",sizeof(cP_CORRECT_ACTOR_STATUS_DATA),sizeof(cCORRECT_ACTOR_STATUS_DATA));

	printf("- ok [%d]\n",sizeof(CPlayerSaveDataForServerDefine));
//早ぬ婁  檜 薑曖脹 掘褻羹曖 餌檜鍔 羹и.
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早晦獄 撲薑
	printf("* find data folder\n");

	if	(!findDataFolder())	
		return	ERRMSG("find data folder","- Failed");	//	等檜攪 ィ渦 瓊晦

#ifdef	_USE_XTRAP
	if	(g_bIsUseXTrap)
		BeginXtrap(1);
#endif

	//	 だ 
	{
		char	*strOrgMap[]	=	
		{
			"[990]Guild_square_0.rmd",
			"[991]Guild_Desert_0.rmd",
			"[992]Guild_Mountains_0.rmd",
			"[993]Guild_Cross counter_0.rmd",
			"[994]Guild_Ruined Sbain Fortress_0.rmd",
			"[995]Guild_square_1.rmd",
			"[996]Guild_Desert_1.rmd",
			"[997]Guild_Mountains_1.rmd",
			"[998]Guild_Cross counter_1.rmd",
			"[999]Guild_Ruined Sbain Fortress_1.rmd",
			"end"
		};

		char	*strChangeMap[]	=	
		{
			"[990]Guild_vs_00.rmd",
			"[991]Guild_vs_01.rmd",
			"[992]Guild_vs_02.rmd",
			"[993]Guild_vs_03.rmd",
			"[994]Guild_vs_04.rmd",
			"[995]Guild_vs_05.rmd",
			"[996]Guild_vs_06.rmd",
			"[997]Guild_vs_07.rmd",
			"[998]Guild_vs_08.rmd",
			"[999]Guild_vs_09.rmd",
			"end"
		};

		for (int i=0;;i++)
		{
			if	(strcmp(strChangeMap[i],"end")==0)
				break;

			char	strOrgFile[512],strChangeFile[512];

			{
				sprintf(strOrgFile,"data/scenario/red stone/map/%s",strOrgMap[i]);
				sprintf(strChangeFile,"data/scenario/red stone/map/%s",strChangeMap[i]);

				if	(!IsFile(strOrgFile))
					continue;

				SetFileAttributes(strOrgFile,FILE_ATTRIBUTE_ARCHIVE);

				if	(IsFile(strChangeFile))
				{
					DeleteFile(strOrgFile);
					continue;
				}
				else
					MoveFile(strOrgFile,strChangeFile);
			}
		}
	}
	
	CheckCS_PacketLog();

#ifdef	_USE_HS
	if	(g_bIsUseHackShield)
	{
		nsHackShield::init();
		SetLogFolder();
	}
#endif

#ifdef	_FOR_CHINA
	if	(g_dwRequireMajorVersion	!=	g_dwMajorVersion	||	g_dwRequireUpdateCounter	!=	g_dwUpdateCounter)
		return	ERRMSG("error in cGAME::init","mismatch resource version\n\n%d,%d : %d,%d",g_dwRequireMajorVersion,g_dwRequireUpdateCounter,g_dwMajorVersion,g_dwUpdateCounter);
#endif

	printf("- ok\n");
	printf("* load server info\n");
	if	(!loadServerInfo())	
		return	ERRMSG("load server info","- Failed");	//	等檜攪 ィ渦 瓊晦
	printf("- ok\n");

	buildActorBuffer();	//	議葛攪 ぷ 撲薑
//早晦獄 撲薑
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

	g_log.setLogFolderName("gameLog");
	g_pPartyManager->init(dMAX_PARTY_COUNT);

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早Щ煎薛お 煎註
	printf("* loading project data\n");
	if	(!g_project.loadProject("red stone.rpd"))
		return	ERRMSG("loading project data","- Failed");	//	等檜攪 ィ渦 瓊晦
	printf("- ok\n");
//早晦獄 撲薑
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早蟾晦音
	printf("* loading skill data\n");
#ifndef	_FOR_KOREA
	if	(!g_sm.loadData("data\\skill2.dat"))
		return	ERRMSG("loading skill data","- Failed");	//	等檜攪 ィ渦 瓊晦
#else
	if	(g_bIsDuelServer)
	{
		if	(!g_sm.loadData("data/skill2.dat"))
			return	ERRMSG("loading skill data","- Failed");	//	等檜攪 ィ渦 瓊晦
	}
	else
	if	(!g_sm.loadData("data/skill.dat"))
		return	ERRMSG("loading skill data","- Failed");	//	等檜攪 ィ渦 瓊晦
#endif

	if	(!g_trapSkillManager.init())
		return	FALSE;

	printf("- ok\n");

	printf("* loading pigeon post shop data\n");
	if	(!g_pigeonPostShop.init())
		return	ERRMSG("loading pigeon post shop data","- Failed");	//	等檜攪 ィ渦 瓊晦
	printf("- ok\n");

	printf("* loading title data\n");
	if (!g_titleManager.init()) 
		return	ERRMSG("loading title data","- Failed");	//	等檜攪 ィ渦 瓊晦
	printf("- ok\n");

	printf("* loading guild mark info\n");
	if (!g_guildManager.loadGuildMarkItemInfo())
		return	ERRMSG("loading guild mark info","- Failed");	//	等檜攪 ィ渦 瓊晦
	printf("- ok\n");

	printf("* loading guild skill data\n");
	if (!g_gsManager.init())	
		return	ERRMSG("loading guild skill data","- Failed");	//	等檜攪 ィ渦 瓊晦
	printf("- ok\n");

	printf("* loading job data and etc image\n");
	if (!cJOB::Init())
		return	ERRMSG("loading job data and etc image","- Job data Failed");	//	等檜攪 ィ渦 瓊晦
	printf("- complete job data initialize\n");
	if (!g_idm.init())	
		return	ERRMSG("loading job data and etc image","- Image data Failed");	//	等檜攪 ィ渦 瓊晦
	printf("- complete image data initialize\n");

	printf("* loading door object crashmap info\n");
	if (!g_ociManager.init())
		return	ERRMSG("loading door object crashmap info","- data file Failed");	//	等檜攪 ィ渦 瓊晦

	printf("- ok\n");

//早晦獄 撲薑
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	
	checkLastErrorSign();
	
//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早в萄 煎註
	printf("* load field data\n");
	if	(!loadField()) 
		return	ERRMSG("load field data","- Failed");	//	等檜攪 ィ渦 瓊晦
	printf("- ok\n");
//早晦獄 撲薑
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	printf("\n complete game data initializing\n");

	g_eventManager.init();	//	в萄 睡 檗擎 пл
	cItem::Init();

	CreateDirectory("save data",NULL);

	FPSetCurrentFolder("gameLog");
	CreateDirectory("backUp",NULL);
	SetLogFolder();

//	蛤ィお 濩 等檜攪 
	{
		CPlayerSaveData	player;

		cFILE	file,file2;

		file.Open("data/defaultJob.dat","wb");
		file2.Open("defaultJob.dat","wb");

		for (i=0;i<dPLAYER_JOB_COUNT;i++)
		{
			player.reset();
			player.initializeJobData("gogooma","gamza",i);
			memcpy(&g_aDefaultPlayerJobData[i],&player,sizeof(player));
			file.Write(&player,sizeof(player));
			file2.Write(&player,sizeof(player));
		}

		file.Close();
		file2.Close();
	}

	m_timer.setFPS(dSYNC_FPS);	//	Щ溯 蝶ё 蟾晦
	g_pDsManager->init();

	m_bIsActiveGame		=	TRUE;

//	в萄 &  熱
	{
		int	iTestPortalCount	=	0;
		printf("\n\nfield info in loaded this server\n-----------------------------------------------------\n");

		for (i=0;i<m_iFieldCount;i++)
		{
			iTestPortalCount	+=	m_pField[i].m_pArea->m_iTestPortalCount;
			printf("%-34s - A : %3d TF : %2d\n",m_pField[i].m_strFileName,m_pField[i].m_iSavedActorCount,m_pField[i].m_pArea->m_iTestPortalCount);
		}

		printf("-----------------------------------------------------\n");
		printf("- total [%d] numbers field loading\n",m_iFieldCount);
		printf("- total [%d] numbers NPC/Monster Exist this server\n",m_iSavedActorCount);
		printf("- total [%d] numbers Test portal Exist this server\n\n",iTestPortalCount);
	}

	printf("\n\n");

	if	(!checkLastErrorSign())
		return	FALSE;
	
	g_debugSign.init();

	return	TRUE;
}

void
cGAME::bookingBanishAllUser(BOOL isFlag)
{
	m_bIsBookedBanishAllUser	=	isFlag;
}

//	睡 蟠頂!!
void
cGAME::banishAllUser()
{
	int	i;

	for (i=0;i<m_iMaxActorCount;i++)
	{
		if (m_pActor[i].m_iZoneSerial	==	0xffffffff	)
			continue;
		if (!m_pActor[i].isPlayer()						)
			continue;

		printf("save [%s/%s]\n",m_pActor[i].m_strId,m_pActor[i].m_strName);

		m_pActor[i].saveDataForTerminateServer();
		m_pActor[i].sendRegistServerMessage(eRSM_GAME_SERVER_CLOSE);
		m_pActor[i].sendGameClose("");
		
		char	strIP[dIP_SIZE];

		strcpy(strIP,"EMPTY");

		int	iClientSerial	=	m_pActor[i].m_iClientSerial;
		CClient	*lpClient	=	m_pActor[i].getClient();

		if	(lpClient)
			strcpy(strIP,lpClient->ip);

//		sendWPUserLogoutById(m_pActor[i].m_strId,strIP);

		m_pActor[i].m_lpField->removeActor(m_pActor[i].m_wSerialInField);
		Sleep(10);

//		if	(lpClient)
//			DisUser(iClientSerial,"banish all");
		DisUser(m_pActor[i].m_strId,"banish all");
	}

	for (i=0;i<m_iFieldCount;i++)
		m_pField[i].m_queReceivePacket.reset();

	g_pIfManager->resetReceivePacket();
}

//
//	謙猿
void
cGAME::close()
{
#ifdef	_USE_HS
	if	(g_bIsUseHackShield)
	{
		_AntiCpSvr_Finalize();
		g_bIsUseHackShield	=	FALSE;
	}
#endif
	
	if	(m_iFieldCount	==	0)
		return;

	int		i;

	urgentSave();

	for (i=0;i<m_iMaxActorCount;i++)
	{
		if	(m_pActor[i].m_iZoneSerial	==	0xffffffff	)
			continue;
		if	(!m_pActor[i].isPlayer()					)
			continue;
		if	(m_pActor[i].isDummy()						)
			continue;

		printf("save [%s/%s]\n",m_pActor[i].m_strId,m_pActor[i].m_strName);

		m_pActor[i].saveDataForTerminateServer();
		m_pActor[i].sendRegistServerMessage(eRSM_GAME_SERVER_CLOSE);
		m_pActor[i].sendGameClose("");

		char	strIP[dIP_SIZE];

		strcpy(strIP,"EMPTY");

		CClient	*lpClient	=	m_pActor[i].getClient();

		if	(lpClient)
			strcpy(strIP,lpClient->ip);

//		sendWPUserLogoutById(m_pActor[i].m_strId,strIP);
		DisUser(m_pActor[i].m_strId,"GAME CLOSE!!");

		m_pActor[i].m_lpField->removeActor(m_pActor[i].m_wSerialInField);

		Sleep(10);
	}

	closeDBPacketReceiver();
	closeBCSPacketReceiver();
	closeWorldPacketReceiver();
	g_idm.close();

	for (i=0;i<m_iFieldCount;i++)
		m_pField[i].reset();

	m_iFieldCount	=	0;
	g_pIfManager->close();

	pKILL(m_pField);
	pKILL(m_pActor);
	KILL(g_pDuelManager);
	
#ifdef	_USE_XTRAP
	if	(g_bIsUseXTrap)
		EndXtrap();
#endif

	DeleteCriticalSection(&m_csActorManager);
}	//	cGAME::Init()

//
//	曹晝 撮檜
void
cGAME::urgentSave()
{
	if	(m_iFieldCount	==	0)
		return;

	if	(g_bIsTestServer)
	{
		printf("test server ignore urgent save\n");

		return;
	}

	int		i;

	SetLogFolder();
	CreateDirectory("urgent save",NULL);

	cFILE	file;

	char	strFileName[128];

	for (i=0;i<m_iMaxActorCount;i++)
	{
		if	(m_pActor[i].m_iZoneSerial == 0xffffffff	)
			continue;

		if	(!m_pActor[i].isPlayer()					)
			continue;

		if	(m_pActor[i].isDummy()						)
			continue;

		sprintf(strFileName,"urgent save/%s[%d].sav",m_pActor[i].m_strId,m_pActor[i].m_wAvatarIndex);

		if (file.Open(strFileName,"rb"))
		{
			cACTOR	actor;

			file.Read(&actor,sizeof(CPlayerSaveData));

			if (actor.m_dwSaveCount > m_pActor[i].m_dwSaveCount)
			{
				file.Close();
				continue;
			}

			file.Close();
		}

		if (m_pActor[i].checkData(NULL,"cGAME::urgentSave",FALSE) != eCADR_ISOK)	continue;

		if (file.Open(strFileName,"wb"))
		{
			m_pActor[i].m_dwSaveCount++;
			file.Write(&m_pActor[i],sizeof(CPlayerSaveData));
			file.Close();
		}
		else
			CLOG("failedUrgentSave","%s/%s",m_pActor[i].m_strId,m_pActor[i].m_strName);

		Sleep(1);
	}
}

void
cGAME::recoveryUrgentSave()
{
	int		i;

	if	(g_bIsTestServer)
	{
		printf("test server ignore urgent save data file\n");

		return;
	}

	cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.sav","urgent save");

	if	(lpFiles)
	{
		cSTRING		*lpFile		=	lpFiles->Pick();
		int			iFileCount	=	lpFiles->Count;
		char		strFileName[128];
		cFILE		file;

		for (i=0;i<iFileCount;i++)
		{
			sprintf(strFileName,"urgent save/%s",lpFile->String);

			lpFile	=	lpFile->pNEXT;

			if (!file.Open(strFileName,"rb"))	continue;

			cACTOR	actor;

			file.Read(&actor,sizeof(CPlayerSaveData));
			file.Close();

			if	(actor.checkData(NULL,"recoveryUrgentSave",FALSE)	==	eCADR_ISOK)
			{
				actor.m_pos.x			=	actor.m_iXPos;
				actor.m_pos.y			=	actor.m_iYPos;
				actor.m_posLastMoved.x	=	actor.m_iXPos;
				actor.m_posLastMoved.y	=	actor.m_iYPos;

				actor.sendUrgentSaveDataToDBCache();
			}

			DeleteFile(strFileName);

			Sleep(1);
		}
	}
	
	KILL(lpFiles);
	
	return;

	lpFiles	=	cFOLDER::GetFiles("*.sav","save data");

	if	(lpFiles)
	{
		cSTRING		*lpFile		=	lpFiles->Pick();
		int			iFileCount	=	lpFiles->Count;
		char		strFileName[128];
		cFILE		file;

		for (i=0;i<iFileCount;i++)
		{
			sprintf(strFileName,"save data/%s",lpFile->String);

			lpFile	=	lpFile->pNEXT;

			if	(!file.Open(strFileName,"rb"))
				continue;

			DWORD	dwBeginCode,dwEndCode,dwSize,dwSaveCount;
			WORD	wCheckSumForSaveData;

			file.Read(&dwSize,4);
			file.Read(&dwBeginCode,4);
			file.Read(&dwSaveCount,4);
			file.Read(&wCheckSumForSaveData,2);
			file.Seek(-4,SEEK_END);
			file.Read(&dwEndCode,4);

			if	(dwBeginCode	==	0x12345678	&&	dwEndCode	==	0x87654321)
			{
				cACTOR	actor;

				file.Seek(14,SEEK_SET);
				file.Read(&actor,sizeof(CPlayerSaveDataForServerDefine));

				if	(actor.checkData(NULL,"recoveryUrgentSave",FALSE)	==	eCADR_ISOK)
				{
					actor.m_pos.x			=	actor.m_iXPos;
					actor.m_pos.y			=	actor.m_iYPos;
					actor.m_posLastMoved.x	=	actor.m_iXPos;
					actor.m_posLastMoved.y	=	actor.m_iYPos;

					actor.sendUrgentSaveDataToDBCache();
				}
			}

			file.Close();

			DeleteFile(strFileName);

			Sleep(1);
		}
	}
}

//
//	蟾晦... 陝п 爾棲 曖 橈.. --a
void
cGAME::buildActorBuffer()
{
	int		i;

	pKILL(m_pActor);
	
	m_pActor	=	new cACTOR [m_iMaxActorCount];

	for (i=0;i<m_iMaxActorCount;i++)
		m_pActor[i].reset();		//	
}	//	cGAME::reset()

//	煎萄 議蝶お 憮 IP撲薑
char* const
cGAME::getBCSIP()
{
	return	m_strBCSIP;
}

//	煎萄 議蝶お 憮 IP撲薑
void
cGAME::setBCSIP(char *_lpstrIP)
{
	strcpy(m_strBCSIP,_lpstrIP);
}

//
//	в萄  .
BOOL
cGAME::loadField()
{
	cFILE	file;

	{
#ifdef	_FOR_CHINA
		char	*lpstrClosemapListFile	=	"data/closeMapListForChina.dat";
#elif	_FOR_JAPAN
		char	*lpstrClosemapListFile	=	"data/closeMapListForJapan.dat";
#elif	_FOR_USA
		char	*lpstrClosemapListFile	=	"data/closeMapListForUsa.dat";
#else
		char	*lpstrClosemapListFile	=	"data/closeMapList.dat";
#endif

		m_iCloseMapCount	=	0;

		if (file.Open(lpstrClosemapListFile,"rb"))
		{
			file.Read(&m_iCloseMapCount,4);
			file.Read(m_awCloseMapList,2*m_iCloseMapCount);

			file.Close();
		}
	}

	int		i;
	//	蟾晦
	{
		m_pField			=	NULL;
		m_iFieldCount		=	0;
		m_iRookieActor		=	0;
		m_iActorCount		=	0;
		m_iSavedActorCount	=	0;
	}

	m_iOxQuizVillage	=	0xffffffff;
	m_iOXQuizField		=	0xffffffff;
	m_iWordQuizVillage	=	0xffffffff;
	m_iWordQuizField	=	0xffffffff;
	m_iGuildBattleLobbyField	=	0xffffffff;

	cSERVER_DATA	*lpCurrentServerInfo	=	&m_aServerList[g_iServerIndex];

	m_iFieldCount	=	lpCurrentServerInfo->m_iCount;

	int	iRealFieldCount	=	m_iFieldCount;	//	 憮 嬪и 撲薑

	if	(g_bIsDuelServer)
		for (int i=0;i<6;i++)
		{
			int		iGateField	=	c_iDuelBattleGateField[i];
			BOOL	bIsLoadField=	FALSE;
			
			if	(lpCurrentServerInfo->getFieldInfo(iGateField))
				iRealFieldCount	+=	g_aiDuelFieldCount[i];
		}

	m_pField		=	new cFIELD [iRealFieldCount];

	if	(!m_pField)	
		return	ERRMSG("error in cGAME::loadFields","can not initialize field buffer");

	g_project.setMapFolder();	//	 ィ渦煎 檜翕

	{
		char	*strDeleteMap[]	=	
		{
			"[503]weapon_test.rmd",
			"[999]Guild_Ruined Sbain Fortress_0.rmd",
			"[173]T18_L02.rmd",
			"[damel]000.rmd",
			"end"
		};

		for (int i=0;;i++)
		{
			if	(strcmp(strDeleteMap[i],"end")==0)
				break;

			if	(!IsFile(strDeleteMap[i]))
				continue;

			SetFileAttributes(strDeleteMap[i],FILE_ATTRIBUTE_ARCHIVE);

			DeleteFile(strDeleteMap[i]);
		}
	}

	for (i=0;i<m_iFieldCount;i++)
	{
		printf("load %s",lpCurrentServerInfo->m_aMap[i].m_strName);

		if (!m_pField[i].load(lpCurrentServerInfo->m_aMap[i].m_strName))
			return	ERRMSG(lpCurrentServerInfo->m_aMap[i].m_strName,"failed");

		if	(m_pField[i].m_wIsGuildBattleLobby)
			m_iGuildBattleLobbyField	=	m_pField[i].m_iUniqueSerial;
		if	(m_pField[i].m_wIsOXQuizField)
			m_iOXQuizField	=	m_pField[i].m_iUniqueSerial;
		if	(m_pField[i].m_wIsWordQuizField)
			m_iWordQuizField=	m_pField[i].m_iUniqueSerial;
		if	(m_pField[i].m_wIsOXQuizVillage)
			m_iOxQuizVillage=	m_pField[i].m_iUniqueSerial;
		if	(m_pField[i].m_wIsWordQuizVillage)
			m_iWordQuizVillage=	m_pField[i].m_iUniqueSerial;

		printf("....... ok!!\n");

		m_pField[i].m_iSerial	=	i;
		m_iSavedActorCount		+=	m_pField[i].m_iSavedActorCount;
	}

//	 憮辨 в萄 煎註
	if	(g_bIsDuelServer)
	{
		int	iFieldSlot	=	m_iFieldCount;
		int	iSerial		=	0;

		for (int i=0;i<6;i++)
		{
			i	=	i;
			for (int iField=0;iField<g_aiDuelFieldCount[i];iField++)
			{
				int		iGateField	=	c_iDuelBattleGateField[i];
				BOOL	bIsLoadField=	FALSE;

				if	(lpCurrentServerInfo->getFieldInfo(iGateField))
					bIsLoadField	=	TRUE;

				if	(bIsLoadField)
				{
					char	strMapName[64];
					int		iCount		=	i+1;

					if	(i == eDUEL_TRIAL_GAME)	//	蕨摹 в萄
						iCount		=	g_iTrialGameTeamSize;

					sprintf(strMapName,"Duel_%dvs%d.rmd",iCount,iCount);

					if	(i == eDUEL_TRIAL_GAME)	//	蕨摹 в萄
						printf("load trial game field %s ",strMapName);
					else
						printf("load [%d vs %d field #%.3d] %s ",iCount,iCount,iField,strMapName);

					if	(!m_pField[iFieldSlot].load(strMapName,iSerial,0xffff,i))
						return	ERRMSG(strMapName,"failed");

					m_pField[iFieldSlot].m_iSerial	=	iFieldSlot;
					m_iSavedActorCount				+=	m_pField[iFieldSlot].m_iSavedActorCount;
					iFieldSlot++;
					printf("....... ok!!\n");
				}

				iSerial++;
			}
		}

		m_iFieldCount	=	iRealFieldCount;
	}

	{
		cSTRINGS	*lpFields	=	cFOLDER::GetFiles("*.rmd");
		
		memset(m_aExistField,0,sizeof(m_aExistField));
		
		if (lpFields)
		{
			cSTRING	*lpFileName	=	lpFields->Pick(0);

			for (int i=0;i<lpFields->Count;i++)
			{
				int	iIndex	=	atoi(lpFileName->String+1);
				
				if	(iIndex	<	1024)
					m_aExistField[iIndex]	=	TRUE;
				
				lpFileName		=	lpFileName->pNEXT;
			}

			KILL(lpFields);
		}
	}

	SetLogFolder();

	if	(!file.Open("data/mapLocationInfo.dat","rb"))
		return	ERRMSG("error in cGAME::loadFields","can not find 'data/mapLocationInfo.dat' file");

	file.Read(g_fieldInfo.m_aField,sizeof(g_fieldInfo.m_aField));
	file.Close();

	return	TRUE;
}	//	cGAME::loadField()


BOOL
cGAME::loadConfigEventData()
{
// 	SetLogFolder();
// 	
// 	g_eventManager.m_EventList.load(0);
// 
// 	SetLogFolder();
	
	return	TRUE;
}
//
//	夔Я斜 等檜攪 蛤註
BOOL
cGAME::loadConfigData()
{
	SetLogFolder();

	cDECODER	decoder;

	if	(IsFile("config-game_dev.txt"))
	{
		decoder.Upload("config-game_dev.txt",NULL);
	}
	else
	if	(!decoder.Upload("config-game.txt",NULL))
		return	ERRMSG("can not find 'config-game.txt' file\n");

	int	iJob	=	0;

	while(1)
	{
		char	*strCode	=	decoder.GetWord();

		if	(!strCode)
			break;

		if	(STRICMP(strCode,"gold_rush")==0)
		{
			cGoldRush	goldRush;

			int	iBeginYear	=	decoder.GetNumber()-2000;
			int	iBeginMonth	=	decoder.GetNumber();
			int	iBeginDay	=	decoder.GetNumber();
			int	iBeginHour	=	decoder.GetNumber();
			int	iBeginMinute=	decoder.GetNumber();
			int	iExpBoost	=	decoder.GetNumber();
			int	iItemBoost	=	decoder.GetNumber();

			char	*lpstrBeginComment	=	decoder.GetStream();
			strncpy(goldRush.m_strBeginComment,lpstrBeginComment,254);
			
			int	iEndYear	=	decoder.GetNumber()-2000;
			int	iEndMonth	=	decoder.GetNumber();
			int	iEndDay		=	decoder.GetNumber();
			int	iEndHour	=	decoder.GetNumber();
			int	iEndMinute	=	decoder.GetNumber();

			char	*lpstrEndComment	=	decoder.GetStream();
			strncpy(goldRush.m_strEndComment,lpstrEndComment,254);

			goldRush.m_begin.year	=	iBeginYear	;
			goldRush.m_begin.month	=	iBeginMonth	;
			goldRush.m_begin.day	=	iBeginDay	;
			goldRush.m_begin.hour	=	iBeginHour	;
			goldRush.m_begin.minute	=	iBeginMinute;
			goldRush.m_end.year		=	iEndYear	;
			goldRush.m_end.month	=	iEndMonth	;
			goldRush.m_end.day		=	iEndDay		;
			goldRush.m_end.hour		=	iEndHour	;
			goldRush.m_end.minute	=	iEndMinute	;

			goldRush.m_wBoostExperience			=	iExpBoost;
			goldRush.m_wCorrectItemDropChance	=	iItemBoost;

			cMSG::Put	("please check gold rush event info","begin time %d/%d/%d %d:%d\nbegin comment : %s\nend time %d/%d/%d/ %d:%d\nend comment : %s\n\nboost exp : %dpercent\nboost item drop chance %dpercent",
						goldRush.m_begin.year	,goldRush.m_begin.month	,goldRush.m_begin.day	,goldRush.m_begin.hour	,goldRush.m_begin.minute,
						goldRush.m_strBeginComment,
						goldRush.m_end.year		,goldRush.m_end.month	,goldRush.m_end.day		,goldRush.m_end.hour	,goldRush.m_end.minute	,
						goldRush.m_strEndComment,
						goldRush.m_wBoostExperience	,goldRush.m_wCorrectItemDropChance);

			g_eventManager.updateGoldRushInfo(&goldRush.m_begin,&goldRush.m_end,goldRush.m_wBoostExperience,goldRush.m_wCorrectItemDropChance,goldRush.m_strBeginComment,goldRush.m_strEndComment);
			g_eventManager.m_goldRush.m_bIsForcedSetting	=	TRUE;
			continue;
		}

		if	(STRICMP(strCode,"max_actor_count")==0)
		{
			m_iMaxActorCount	=	decoder.GetNumber();
			continue;
		}
		if	(STRICMP(strCode,"SeasonVariable")==0)
		{
			g_iSeasonVariable	=	decoder.GetNumber();
			continue;
		}
		if	(STRICMP(strCode,"experience_factor")==0)
		{
			g_iExperienceFactor	=	decoder.GetNumber();
			continue;
		}

		if	(STRICMP(strCode,"item_factor")==0)
		{
			g_iItemFactor	=	decoder.GetNumber();
			continue;
		}

		if	(STRICMP(strCode,"server_index")==0)
		{
			g_iServerIndex		=	decoder.GetNumber();
			g_iDuelFieldSerial	=	1+g_iServerIndex*10000000;
			
			WriteGameServerIndexToRegistry(eST_GAME,g_iServerIndex);

			continue;
		}

		if	(STRICMP(strCode,"magic_option_drop_chance")==0)
		{
			g_iSpecialMagicOptionDropChance	=	decoder.GetNumber();
			continue;
		}

		if	(STRICMP(strCode,"server_info_file")==0)
		{
			strcpy(g_strServerInfoFileName,decoder.GetStream());
			continue;
		}

		if	(STRICMP(strCode,"stress_test_server")==0)
		{
			g_pStressTestManager	=	new	cStressTestManager();
			g_bIsStressTestServer	=	TRUE;
			continue;
		}

		if	(STRICMP(strCode,"test_server")==0)
		{
			g_bIsOpenedTestServer	=	TRUE;
			continue;
		}

		if	(STRICMP(strCode,"real_price")==0)
		{
			g_bIsUseRealPrice	=	TRUE;
			continue;
		}

		if	(STRICMP(strCode,"ignore_xtrap")==0)
		{
			g_bIsUseXTrap	=	FALSE;
			continue;
		}

		if	(STRICMP(strCode,"duel_1vs1")==0)
		{
			g_aiDuelFieldCount[eDUEL_1VS1]	=	decoder.GetNumber();;
			continue;
		}

		if	(STRICMP(strCode,"duel_2vs2")==0)
		{
			g_aiDuelFieldCount[eDUEL_2VS2]	=	decoder.GetNumber();;
			continue;
		}

		if	(STRICMP(strCode,"duel_3vs3")==0)
		{
			g_aiDuelFieldCount[eDUEL_3VS3]	=	decoder.GetNumber();;
			continue;
		}

		if	(STRICMP(strCode,"duel_4vs4")==0)
		{
			g_aiDuelFieldCount[eDUEL_4VS4]	=	decoder.GetNumber();;
			continue;
		}

		if	(STRICMP(strCode,"duel_5vs5")==0)
		{
			g_aiDuelFieldCount[eDUEL_5VS5]	=	decoder.GetNumber();;
			continue;
		}
		if	(STRICMP(strCode,"duel_trial_game")==0)
		{
			g_aiDuelFieldCount[eDUEL_TRIAL_GAME]	=	decoder.GetNumber();;
			continue;
		}
		if	(STRICMP(strCode,"duel_trial_team_size")==0)
		{
			g_iTrialGameTeamSize	=	decoder.GetNumber();;
			continue;
		}

		if	(STRICMP(strCode,"duel_server")==0)
		{
			g_bIsDuelServer		=	TRUE;
			continue;
		}
		if	(STRICMP(strCode,"operatorServer")==0)
		{
			g_bIsOperatorServer		=	TRUE;
			continue;
		}
		if	(STRICMP(strCode,"MAIN_TEST")==0)
		{
			g_bIsMainTestServer		=	TRUE;
			continue;
		}

		if (STRICMP(strCode,"end")==0)	
			break;
	}

	if	(g_iExperienceFactor	!=	1	||	g_iItemFactor	!=	1)
		cMSG::Put("Warning!!","Experience Factor [x %d]\nItem Factor [x %d]",g_iExperienceFactor,g_iItemFactor);

	if	(g_bIsOpenedTestServer)
	{
		g_bIsTestServer	=	TRUE;
#ifdef	_USE_NPGL
		g_bIsUseNPGL	=	TRUE;
#endif
	}

	SetLogFolder();

	if	(g_iServerIndex == 0xffffffff)
		MessageBox(NULL , "config-game.txt 曖 server_index о跡擊 輿撮蹂." , "憮檣策蝶 撲薑 腑蝗棲." ,  MB_OK);

#ifdef	_USE_XTRAP
	if	(g_bIsTestServer == FALSE && g_bIsUseXTrap == FALSE)
		cMSG::Put("X-Trap檜 綠側 腎橫 氈蝗棲." , "X-Trap擊 側 ж溥賊 config-game.txt だ\n\nignore_xtrap;\n\n檜朝 頂辨擊 雖 輿衛晦 奧棲." ,  MB_OK);
#endif

	return	TRUE;
}


BOOL
cGAME::loadConfigForBattleData()
{
	SetLogFolder();

	cDECODER	decoder;

	if	(!decoder.Upload("config-Battle.txt",NULL))
		return	ERRMSG("can not find 'config-Battle.txt' file\n");

	int	iJob	=	0;

	while(1)
	{
		char	*strCode	=	decoder.GetWord();

		if	(!strCode)
			break;

		//舒⑷  蝶 萄.... 雖紫 僥薯朝  橈橫辨...;;
		if	(STRICMP(strCode,"knight")==0)
		{
			g_afDuelDamageFactor[dJOB_KNIGHT]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"warrior")==0)
		{
			g_afDuelDamageFactor[dJOB_WARRIOR]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"wizard")==0)
		{
			g_afDuelDamageFactor[dJOB_WIZARD]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"werewolf")==0)
		{
			g_afDuelDamageFactor[dJOB_WEREWOLF]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"priest")==0)
		{
			g_afDuelDamageFactor[dJOB_PRIEST]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"angel")==0)
		{
			g_afDuelDamageFactor[dJOB_FALLEN_ANGEL]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"rogue")==0)
		{
			g_afDuelDamageFactor[dJOB_ROGUE]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"fighter")==0)
		{
			g_afDuelDamageFactor[dJOB_FIGHTER]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"lancer")==0)
		{
			g_afDuelDamageFactor[dJOB_MAGIC_LANCER]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"archer")==0)
		{
			g_afDuelDamageFactor[dJOB_MAGIC_ARCHER]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"tamer")==0)
		{
			g_afDuelDamageFactor[dJOB_BEAST_TAMER]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"summoner")==0)
		{
			g_afDuelDamageFactor[dJOB_SUMMONER]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"princess")==0)
		{
			g_afDuelDamageFactor[dJOB_PRINCESS]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"girl")==0)
		{
			g_afDuelDamageFactor[dJOB_MAGICAL_GIRL]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"necro")==0)
		{
			g_afDuelDamageFactor[dJOB_NECROMANCER]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"demon")==0)
		{
			g_afDuelDamageFactor[dJOB_DEVIL]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"soulbringer")==0)
		{
			g_afDuelDamageFactor[dJOB_SOUL_BRINGER]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"Champion")==0)
		{
			g_afDuelDamageFactor[dJOB_CHAMPION]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"opticalist")==0)
		{
			g_afDuelDamageFactor[dJOB_OPTICALIST]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"beastman")==0)
		{
			g_afDuelDamageFactor[dJOB_BEAST_MAN]	=	decoder.GetNumber() / 100.0f;
			continue;
		}
		if	(STRICMP(strCode,"NPC")==0)
		{
			g_afDuelDamageFactor[dPLAYER_JOB_COUNT]	=	decoder.GetNumber() / 100.0f;
			continue;
		}

		if (STRICMP(strCode,"end")==0)	
			break;
	}
	SetLogFolder();

	return	TRUE;
}


//
//	憮 薑爾 煎註и.
BOOL
cGAME::loadServerInfo()
{
	class cSERVER_MAP_INFO
	{
	public:
		int		m_iSerial;
		int		m_iType;
		char	m_strName[64];
		int		m_iServerSerial;

		cSERVER_MAP_INFO()
		{
			m_iSerial	=	0xffff;
		}
	};

	class cSERVER_INFO
	{
	public:
		int			m_iSerial;
		int			m_iCount;
		char		m_strName[64];
		char		m_strIP[20];
		int			m_iPort;
		cSERVER_MAP_INFO	m_aMap[dMAX_MAP_COUNT_IN_SERVER];

					cSERVER_INFO()
					{
						m_iSerial	=	0xffff;
					}
	};

	char	strFileName[256];

	sprintf(strFileName,"data/%s",g_strServerInfoFileName);

	cFILE	file;

	if (!file.Open(strFileName,"rb"))
		return	ERRMSG("error! can not find follow file",strFileName);

	file.Read(&m_iServerCount,4);

	for (int i=0;i<m_iServerCount;i++)
	{
		cSERVER_INFO	serverInfo;

		file.Read(&serverInfo,sizeof(cSERVER_INFO)-sizeof(serverInfo.m_aMap));

		if	(serverInfo.m_iCount	>	0)
			file.Read(serverInfo.m_aMap,sizeof(cSERVER_MAP_INFO)*serverInfo.m_iCount);

		strcpy(m_aServerList[i].m_strName,serverInfo.m_strName);

		m_aServerList[i].m_iCount	=	serverInfo.m_iCount;

		for (int j=0;j<serverInfo.m_iCount;j++)
		{
			if	(strlen(serverInfo.m_aMap[j].m_strName)	>=	32)
			{
				cMSG::Put("loading error!!","field %s name to long!!\n\nunder 32 character",serverInfo.m_aMap[j].m_strName);

				return	FALSE;
			}
			memcpy(&m_aServerList[i].m_aMap[j],&serverInfo.m_aMap[j],sizeof(cFIELD_INFO));
		}
	}

	file.Close();

	return	TRUE;
}

//
//	等檜攪 ィ渦 瓊朝.
BOOL
cGAME::findDataFolder()
{
	FPGetCurrentFolder(256,g_strLogFolder);

	{
		SetLogFolder();

		CRegKey		reg;

		if	(reg.Open( g_hRegKey, g_strRegAddress) == ERROR_SUCCESS)
		{
			DWORD	count	=	sizeof(g_strLogFolder);

			if	(reg.QueryValue(g_dwMajorVersion,"Version") != ERROR_SUCCESS)
				g_dwMajorVersion	=	0;
			if	(reg.QueryValue(g_dwUpdateCounter,"updateCounter") != ERROR_SUCCESS)
				g_dwUpdateCounter	=	0;

			if	(reg.QueryValue((LPTSTR )g_strLogFolder,"Path",&count) == ERROR_SUCCESS)
			{	
				if	(count	<=	0)
					return ERRMSG("Error in cGAME::findDataFolder","can not find registry data of Red Stone Path1");
			}
			else
				return ERRMSG("Error in cGAME::findDataFolder","can not find registry data of Red Stone Path2");
		}
		else
			return ERRMSG("Error in cGAME::findDataFolder","can not find registry data of Red Stone");

		DWORD	dwIsTestServer;

		if	(reg.QueryValue(dwIsTestServer,"isDevPC") == ERROR_SUCCESS)
			g_bIsInnerTestServer	=	dwIsTestServer;

		if	(reg.QueryValue(dwIsTestServer,"isDuelTestPC") == ERROR_SUCCESS)
			g_bIsDuelTestPC			=	dwIsTestServer;

		if	(reg.QueryValue(dwIsTestServer,"isChansPC") == ERROR_SUCCESS)
			g_bIsChansTestServer	=	dwIsTestServer;

		if	(g_bIsInnerTestServer	||	g_bIsChansTestServer)
			g_bIsTestServer	=	TRUE;

		strcpy(m_strDataFolder,_ms("%s\\data",g_strLogFolder));

		if (!FPSetCurrentFolder(m_strDataFolder))
			return ERRMSG("error!!","can not find folder '%s'",m_strDataFolder);
	}

	if	(g_bIsInnerTestServer)
		g_bIsUseHackShield	=	FALSE;
	
#ifdef	_FOR_KOREA	
//	g_bIsTestBCS_Deliver	=	TRUE;
#endif

	SetLogFolder();

	//intf("data folder : %s\n",m_strDataFolder);

#ifdef	_FOR_CHINA
	cFILE	file;

	file.Open("red stone.cfg","rb");
	file.Write(&g_dwMajorVersion,4);
	file.Write(&g_dwUpdateCounter,4);
	file.Close();
#endif	

	return	TRUE;
}

//
//	в萄 睡 機等檜お
void
cGAME::update()
{
	g_eh.resetLog();

	updateInterface();

	updateGame();

	if	(g_bIsStressTestServer)
		g_pStressTestManager->updateJoin();

	if	(m_bIsBookedBanishAllUser)
	{
		m_bIsBookedBanishAllUser	=	FALSE;

		banishAllUser();
	}
}

//
//	 瞳檣 睡 機等檜お
void
cGAME::updateInterface()
{
	//	曹晝 
	if	(m_wIsReadyToUrgentSave)
	{
		urgentSave();
		m_wIsReadyToUrgentSave	=	FALSE;
	}

	//	憮晦遽 ⑷營 衛除
	{
		CTimeInfo	timeOld;

		memcpy(&timeOld,&g_currentTime,sizeof(g_currentTime));
		g_currentTime.update();

		g_iCurrentTimeMinuteValue	=	g_currentTime.getMinuteValue();
		g_iCurrentTimeSecondValue	=	g_currentTime.getSecondValue();


		g_bIsChangeMinute	=	FALSE;
		g_bIsChangeHour		=	FALSE;
		g_bIsChangeDay		=	FALSE;

		if	(timeOld.m_wMinute	!=	g_currentTime.m_wMinute)
		{
			increasePlayerPlayTime();
			g_bIsChangeMinute	=	TRUE;

			if	(g_currentTime.m_wMinute	%	10	==	0)
				makeStaticsOfPacketFlowing();

			if	(timeOld.m_wHour	!=	g_currentTime.m_wHour)
			{
				g_bIsChangeHour	=	TRUE;

				if	(timeOld.m_wDay	!=	g_currentTime.m_wDay)
					g_bIsChangeDay	=	TRUE;
			}
		}

		if	(g_bIsChangeHour)
		{
			int	iPlayerSum	=	0;

			SERVERStoWORLD_PLAYER_COUNT	packet;

			packet.base.set(sizeof(packet),dSERVERStoWORLD_PLAYER_COUNT);
			packet.wHour	=	g_currentTime.m_wHour;
			packet.wCount	=	0;

			//	萄 煎斜 晦晦
			{
				int	i;

				char	strLog[512]	=	"";

				for (i=0;i<eGET_GOLD_TYPE_COUNT;i++)
					strcat(strLog,_ms("	%I64d",g_allGettingGold[i]));
				for (i=0;i<eSPENT_GOLD_TYPE_COUNT;i++)
					strcat(strLog,_ms("	%I64d",g_allSpentGold[i]));

				CLOG("goldIO",strLog);
			}


			for (int iField=0;iField<m_iFieldCount;++iField)	//	陝в萄煎 除 ぬ 葬
			{
				cFIELD	*lpField	=	&m_pField[iField];
				int		iPlayerCount=	0;

				for	(int iTeam	=	lpField->m_bIsGuildBattleField+1;iTeam;)
				{
					--iTeam;
					
					for (int iPlayer = lpField->m_aiActorCount[iTeam]; iPlayer >0 ; )
					{
						--iPlayer;

						cACTOR	*lpPlayer	=	lpField->getActor((int)lpField->m_awActorList[0][iPlayer]);

						if	(lpPlayer && lpPlayer->isPlayer())
							++iPlayerCount;
					}
				}

				iPlayerSum	+=	iPlayerCount;

				packet.awPlayerCount[packet.wCount*2	]	=	lpField->m_iUniqueSerial;
				packet.awPlayerCount[packet.wCount*2+1	]	=	iPlayerCount;
				
				packet.wCount++;

				if	(packet.wCount	>=	1000)
				{
					SEND_WOLRD_PACKET(&packet);
					packet.wCount	=	0;
				}
			}

			WORD	awPlayerPerSecretDungeon[200];

			memset(awPlayerPerSecretDungeon,0,sizeof(awPlayerPerSecretDungeon));

			int	i;

			for (i =1;i<dMAX_IF_COUNT;i++)
			{
				cInstanceField*	lpIF	=	&g_pIfManager->m_aInstanceField[i];

				if	(lpIF->m_wSerial	==	0xffff)
					continue;

				for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
				{
					cFIELD	*lpField	=	&lpIF->m_pFloor[iFloor];

					int		iPlayerCount=	0;

					for	(int iTeam	=	lpField->m_bIsGuildBattleField+1;iTeam;)
					{
						--iTeam;

						for (int iPlayer = lpField->m_aiActorCount[iTeam]; iPlayer >0 ; )
						{
							--iPlayer;

							cACTOR	*lpPlayer	=	lpField->getActor((int)lpField->m_awActorList[0][iPlayer]);

							if	(lpPlayer && lpPlayer->isPlayer())
								++iPlayerCount;
						}
					}

					awPlayerPerSecretDungeon[lpIF->m_wSecretDungeonSerial]	+=	iPlayerCount;
				}
			}

			for	(i=0;i<200;i++)
			{
				if	(awPlayerPerSecretDungeon[i]	==	0)
					continue;

				packet.awPlayerCount[packet.wCount*2	]	=	i+1000;
				packet.awPlayerCount[packet.wCount*2+1	]	=	awPlayerPerSecretDungeon[i];
				packet.wCount++;

				if	(packet.wCount	>=	1000)
				{
					SEND_WOLRD_PACKET(&packet);
					packet.wCount	=	0;
				}
			}

			packet.base.wSize	=	sizeof(packet)-sizeof(packet.awPlayerCount)+sizeof(WORD)*2*packet.wCount;
			SEND_WOLRD_PACKET(&packet);
		}
	}

	//	蟾渡 機等檜お 樓紫
	{
		static	DWORD	dwTime	=	0;

		if	(m_bIsChangeSecond)
		{
			m_dw1SecondTime	=	g_dwCurrentTime-dwTime;

			if	(isActiveGameServer())
			{
				if	(g_bIsDisplayTick && g_bIsWantPrintTriggerCondition == FALSE)
				{
					if	(g_bIsStressTestServer)
						printf("- %.4d[%4d/%4d/%4d][%.5d] stress test server -\n",m_dw1SecondTime,m_iPlayerCount,CM.Size(),m_iMaxConnectedPlayerCount,g_game.m_iActorCount);
					else
						printf("- %.4d[%4d/%4d/%4d][%.5d]\n",m_dw1SecondTime,m_iPlayerCount,CM.Size(),m_iMaxConnectedPlayerCount,g_game.m_iActorCount);
				}
			}
			else
			{
				if	(m_wCheckCountForReceiveInitDataFromWorld)
					--m_wCheckCountForReceiveInitDataFromWorld;

				if	(m_wCheckCountForReceiveInitDataFromWorld	==	0)
				{
					if	(!g_bIsCompleteReceiveGuildDataWork)
						printf("wait for receive Guild Info\n");
					if	(!g_bIsCompleteReceiveEventTimeData)
						printf("wait for receive Event Time Data\n");
					if	(!g_bIsCompleteReceiveLottoEventData)
						printf("wait for receive LOTTO event Info\n");
					if	(g_itemPackList.checkReceiveAllInfo()		==	FALSE)
						printf("wait for receive Item pack List\n");
					if	(g_eventManager.m_bIsReceiveGoldRushInfo	==	FALSE)
						printf("wait for receive Gold rush info\n");
					if	(g_carrotShopInfo.m_bIsCompleteReceiveCarrotShopInfo	==	FALSE)
						printf("wait for receive Carrot shop Info\n");
				}
			}

			dwTime			=	g_dwCurrentTime;
		}
	}

	if	(m_bIsExportProfile	)
	{
		m_bIsExportProfile	=	FALSE;

		exportProfilerInfomation();
//		exportPacketCounterInfomation();
		exportUserPlayerInfoInField();
	}

	if	(m_bIsResetFieldStatics)
	{
		m_bIsResetFieldStatics	=	FALSE;
		resetUserPlayStaticsInField();
	}

	if	(m_bResetProfiler)
	{
		m_bResetProfiler	=	FALSE;
		m_profiler.reset();
	}
}

//
//	в萄 睡 機等檜お
inline	void
cGAME::updateGame()
{
	g_debugSign.m_iCurrentGameStep	=	1;

	DWORD	dwCurrentTime	=	timeGetTime();

#ifdef	_IS_TOOL
//	dwCurrentTime	*=	4;
#endif

	if	(dwCurrentTime	<	g_dwCurrentTime)
	{
		//	 議葛攪曖 暮婦 顫 蟾晦
	}

	g_dwCurrentTime		=	dwCurrentTime;
	m_bIsChangeSecond	=	FALSE;

//	蟾欽嬪 攪 陛
	if	(g_dwCurrentTime/1000	!=	(DWORD)g_iLastTimeByTimeGetTime)
	{
		m_bIsChangeSecond		=	TRUE;
		g_iLastTimeByTimeGetTime=	g_dwCurrentTime/1000;
		g_iCounterBySecond++;
	}

	if (m_dwFrameCounter%dSYNC_FPS==0)
		m_profiler.begin();
	else
		m_profiler.wakening();

	m_profiler.begin("羹 ぬ 葬");

	g_debugSign.m_iCurrentGameStep	=	2;

	operateBCSPacket();	//	B憮 嫡擎 ぬ 葬
	g_debugSign.m_iCurrentGameStep	=	21;

	m_bIsRequireRebuildRedStonePointTitle	=	FALSE;

	operateDBPacket();	//	DB憮 嫡擎 ぬ 葬
	g_debugSign.m_iCurrentGameStep	=	3;
	operateWorldPacket();

	g_debugSign.m_iCurrentGameStep	=	4;
	m_iPlayerCount	=	0;
	memset(m_iCountReserve, 0x00, sizeof(int)*3);

	g_iCurrentMoveNumber++;

	if	(g_iCurrentMoveNumber	>=	dUPDATE_UPDATE_MOVE_POS_PERIOD)
		g_iCurrentMoveNumber	=	0;

	for (g_debugSign.m_iCurrentUpdateField=m_iFieldCount;g_debugSign.m_iCurrentUpdateField;)	//	陝в萄煎 除 ぬ 葬
	{
		--g_debugSign.m_iCurrentUpdateField;

		cFIELD	*lpField	=	&m_pField[g_debugSign.m_iCurrentUpdateField];

		lpField->operateStoredPacket();
	}

	g_pIfManager->operateStoredPacket();

	if	(m_dwFrameCounter%2	==	0)
		g_userPM.operate();

	m_profiler.end("羹 ぬ 葬");

//	FPS 蜃醮 諒 ж朝 睡
	{
		m_timer.syncFrame();	//	dSYC_FPS 蜃 諒 衛鑑.

		if (m_iResetTimerCounterForSync	==	0)	//	20蟾 и廓 脹 衛除擊 葬 衛鑑.
		{
			m_timer.resetOverTime();

			m_iResetTimerCounterForSync	=	dSYNC_FPS*20;
		}

		m_iResetTimerCounterForSync--;
	}
	
	m_profiler.begin(" 葬");

	g_debugSign.m_iCurrentGameStep	=	5;
	m_profiler.begin("望萄 詭棲螳");
	g_guildManager.update();
	m_profiler.end("望萄 詭棲螳");

	g_debugSign.m_iCurrentGameStep	=	6;

	g_pJoinPlayerManager->operate();
	g_pMoveFieldUserManager->operate();

	g_debugSign.m_iCurrentGameStep	=	7;

	for (g_debugSign.m_iCurrentUpdateField=m_iFieldCount;g_debugSign.m_iCurrentUpdateField;)
	{
		--g_debugSign.m_iCurrentUpdateField;

		cFIELD	*lpField	=	&m_pField[g_debugSign.m_iCurrentUpdateField];

		lpField->updateGame();

		m_iPlayerCount	+=	lpField->m_iRealPlayerCount;
	}

	m_iPlayerCount	+=	g_pIfManager->getRealPlayerCount();

	if	(m_iPlayerCount	>	m_iMaxConnectedPlayerCount)
		m_iMaxConnectedPlayerCount	=	m_iPlayerCount;

	g_pIfManager->updateGame();

	g_debugSign.m_iCurrentGameStep	=	8;
	m_profiler.begin("1蟾 機");
	operateOncePerSecondWork();		//	蟾渡 и廓 機等檜お п 機 葬
	m_profiler.end("1蟾 機");
	g_debugSign.m_iCurrentGameStep	=	9;
	m_profiler.begin("1 機");
	operateOncePerMinuteWork();		//	渡 и廓 機等檜お п 機 葬
	m_profiler.end("1 機");
	g_debugSign.m_iCurrentGameStep	=	10;
	m_profiler.begin("10蟾 機");
	operateOncePer10SecondWork();	//	10蟾 и廓 機等檜お п 機 葬
	m_profiler.end("10蟾 機");
	g_debugSign.m_iCurrentGameStep	=	11;
	operateOncePerHourWork();		//	衛除渡 и廓 機等檜お п 機

	g_debugSign.m_iCurrentGameStep	=	12;
	m_profiler.begin("檜漸お 機渤");
	g_eventManager.update();
	m_profiler.end("檜漸お 機渤");

	g_delayKarmaManager.updateDelayKarma();
	
	g_debugSign.m_iCurrentFieldStep	=	12;

	g_debugSign.m_iCurrentGameStep	=	-1;

	m_profiler.end(" 葬");

	if (m_dwFrameCounter%dSYNC_FPS==0)
		m_profiler.end();
	else
		m_profiler.sleep();

	m_dwFrameCounter++;
}

//
//	Ы溯檜橫曖 Ы溯檜 顫 陛 衛晦
inline	void
cGAME::increasePlayerPlayTime()
{
	for	(int iIndex=m_iFieldCount;iIndex;iIndex--)
		m_pField[iIndex-1].increasePlayerPlayTime();

	g_pIfManager->increasePlayerPlayTime();
}

//
//	衛除渡 и廓 葬пй 機
inline	void
cGAME::operateOncePerHourWork()
{
	if (g_bIsChangeHour	==	FALSE)
		return;
		
	for(int	iIndex=m_iFieldCount;iIndex;iIndex--)
		m_pField[iIndex-1].operateOncePerHourWork();

	g_pIfManager->operateOncePerHourWork();
}

//
//	渡 и廓 葬пй 機
inline	void
cGAME::operateOncePerMinuteWork()
{
	g_debugSign.m_iTempStep = 1;

	if	(m_bIsChangeSecond	==	FALSE)
		return;

	g_debugSign.m_iTempStep = 2;

	if	(g_iCounterBySecond - m_iOncePerMinuteWorkLastestOperateTime	>=	60)	//	詞 1 и廓 ж朝 機
	{
		m_iOncePerMinuteWorkLastestOperateTime	=	g_iCounterBySecond;

		g_pIfManager->operateOncePerMinuteWork();
		g_bannerManager.operateOncePerMinute();
		g_gbsToday.operateOncePerMinute();	//	棺 望萄 鼻 機渤~

		sendWPPlayerCount(g_iServerIndex,m_iPlayerCount);
	}

//	1 и廓 ж曹 п ж朝等.. и廓  ж晦朝 睡 蝶 散憮 п ж朝 機
	
	g_debugSign.m_iTempStep = 3;
	
#define	dSECOND_PER_ONCE_MINUTE	60
	int	iValue		=	g_iCounterBySecond%dSECOND_PER_ONCE_MINUTE;
	int	iSharedCount=	max(m_iFieldCount/dSECOND_PER_ONCE_MINUTE,1);
	int	iBeginIndex	=	min(iSharedCount*iValue,m_iFieldCount-1);
	int	iModCount	=	m_iFieldCount%dSECOND_PER_ONCE_MINUTE;
	int	iEndIndex	=	iSharedCount*(iValue+1)-1;

	if	(m_iOncePerMinuteWorkBeginIndex	==	iBeginIndex)
		return;

	m_iOncePerMinuteWorkBeginIndex	=	iBeginIndex;

	iEndIndex		=	min(m_iFieldCount-1,iEndIndex);
	int	iCount		=	iEndIndex-iBeginIndex+1;

	for(int	iIndex=iBeginIndex;iCount;iCount--,iIndex++)
	{
		m_pField[iIndex].operateOncePerMinuteWork();
	}

	g_debugSign.m_iTempStep = 4;

	if (iValue < iModCount && m_iFieldCount > dSECOND_PER_ONCE_MINUTE)
	{
		m_pField[iValue+dSECOND_PER_ONCE_MINUTE*iSharedCount].operateOncePerMinuteWork();
	}
	
	g_debugSign.m_iTempStep = 5;

	g_eventManager.m_wordCompareEvent.CheckWordEventAppointment();

	g_debugSign.m_iTempStep = 0;
}

//
//	10蟾渡 и廓 葬пй 機 
inline	void
cGAME::operateOncePer10SecondWork()
{
	if (m_bIsChangeSecond	==	FALSE)
		return;

	if (g_iCounterBySecond	-	m_iOncePer10SecondWorkLastestOperateTime	>=	10	)
	{
		m_iOncePer10SecondWorkLastestOperateTime	=	g_iCounterBySecond;

		g_pIfManager->operateOncePer10SecondWork();
	}

#define	d10SECOND	10
	int	iValue		=	g_iCounterBySecond%d10SECOND;
	int	iSharedCount=	max(m_iFieldCount/d10SECOND,1);
	int	iBeginIndex	=	min(iSharedCount*iValue,m_iFieldCount-1);
	int	iModCount	=	m_iFieldCount%d10SECOND;
	int	iEndIndex	=	iSharedCount*(iValue+1)-1;

	if (m_iOncePer10SecondWorkBeginIndex==	iBeginIndex)
		return;

	m_iOncePer10SecondWorkBeginIndex	=	iBeginIndex;

	iEndIndex		=	min(m_iFieldCount-1,iEndIndex);
	int	iCount		=	iEndIndex-iBeginIndex+1;

	for(int	iIndex=iBeginIndex;iCount;iCount--,iIndex++)
	{
		m_pField[iIndex].operateOncePer10SecondWork();
	}

	if (iValue < iModCount && m_iFieldCount > d10SECOND)
	{
		m_pField[iValue+d10SECOND*iSharedCount].operateOncePer10SecondWork();
	}
}

#ifdef	_USE_HS
void
cGAME::operateOncePerSecondHS_Work()
{
	if	(g_bIsUseHackShield	==	FALSE)
		return;

	for (int iClient=dGAME_MAX_USER_COUNT;iClient;)
	{
		--iClient;

		CClient	*lpClient	=	CM.mGETCLIENT(iClient);

		if	(!lpClient)
			continue;

		if	(lpClient->m_wAppointmentDisconnectTime)
		{
			--lpClient->m_wAppointmentDisconnectTime;

			if	(lpClient->m_wAppointmentDisconnectTime	==	0)
				DisUser(lpClient->wSerial,"by Hack Shield");

			continue;
		}

		if	(lpClient->m_wWaitHS_GuidResultTime)
		{
			--lpClient->m_wWaitHS_GuidResultTime;

			if	(lpClient->m_wWaitHS_GuidResultTime	==	0)
			{
				int		iZoneSerial	=	lpClient->GetActorSerialInServer();
				cACTOR	*lpPlayer	=	getActor(iZoneSerial);

				g_game.sendDisconnectByHackShieldPacket(lpClient);

				if	(lpPlayer)
				{
					printf("Disconnect HS GUID Failed User [%s/%s] \n",lpPlayer->m_strId,lpPlayer->m_strName);
					printf("    by not received result\n");
					CLOG("HackShield","Disconnect HS GUID Failed User [%s/%s] by not received result",lpPlayer->m_strId,lpPlayer->m_strName);

					lpPlayer->bookingReadyToExitGame();
				}

				continue;
			}
		}

		if	(lpClient->m_wWaitHS_ReqMsgResult	==	nsHackShield::eWR_None)
			continue;

		if	(lpClient->m_iSendHS_AckMsgCount	>=	5)
		{
			int		iZoneSerial	=	lpClient->GetActorSerialInServer();
			cACTOR	*lpPlayer	=	getActor(iZoneSerial);

			g_game.sendDisconnectByHackShieldPacket(lpClient);

			if	(lpPlayer)
			{
				printf("Disconnect HS Auth Failed User [%s/%s] \n",lpPlayer->m_strId,lpPlayer->m_strName);
				printf("    by not received result\n");
				CLOG("HackShield","Disconnect HS Auth Failed User [%s/%s] by not received result",lpPlayer->m_strId,lpPlayer->m_strName);

				lpPlayer->bookingReadyToExitGame();
			}
			continue;
		}

		if	(lpClient->m_iLastSendHS_ReqMsgTime	<	g_iCounterBySecond-5)
		{
			if	(nsHackShield::sendReqMsg(lpClient,ANTICPSVR_CHECK_GAME_MEMORY)	==	FALSE)
			{
				int		iZoneSerial	=	lpClient->GetActorSerialInServer();
				cACTOR	*lpPlayer	=	getActor(iZoneSerial);

				g_game.sendDisconnectByHackShieldPacket(lpClient);

				if	(lpPlayer)
				{
					printf("Disconnect HS Auth Failed User [%s/%s]\n",lpPlayer->m_strId,lpPlayer->m_strName);
					printf("    by Failed nsHackShield::sendReqMsg Work\n");

					lpPlayer->bookingReadyToExitGame();
				}

				continue;
			}
		}
	}

	int	iSendActorCount	=	0;

	for (;m_iLastUpdateActorForHS<dGAME_MAX_USER_COUNT && iSendActorCount < 10;m_iLastUpdateActorForHS++)
	{
		CClient	*lpClient	=	CM.mGETCLIENT(m_iLastUpdateActorForHS);

		if	(!lpClient ||	lpClient->m_iLastSendHS_ReqMsgTime	==	0	|| lpClient->m_iLastSendHS_ReqMsgTime > g_iCounterBySecond-20)
			continue;

		if	(lpClient->m_wAppointmentDisconnectTime)
			continue;

		if	(nsHackShield::sendReqMsg(lpClient,ANTICPSVR_CHECK_GAME_MEMORY)	==	FALSE)
		{
			int		iZoneSerial	=	lpClient->GetActorSerialInServer();
			cACTOR	*lpPlayer	=	getActor(iZoneSerial);

			g_game.sendDisconnectByHackShieldPacket(lpClient);

			if	(lpPlayer)
			{
				printf("Disconnect HS Auth Failed User [%s/%s]\n",lpPlayer->m_strId,lpPlayer->m_strName);
				printf("    by Failed nsHackShield::sendReqMsg Work\n");

				lpPlayer->bookingReadyToExitGame();
			}

			continue;
		}
	}

	if	(m_iLastUpdateActorForHS	>=	m_iMaxActorCount)
		m_iLastUpdateActorForHS		=	0;
}
#endif

#ifdef	_USE_NPGL
inline	void
cGAME::operateOncePerSecondNPGLWork()
{
	if	(g_bIsUseNPGL	==	FALSE)
		return;
	
	int	iSendActorCount	=	0;

	for (;m_iLastUpdateActorForNPGL<m_iMaxActorCount && iSendActorCount < 10;m_iLastUpdateActorForNPGL++)
	{
		cACTOR	*lpActor	=	&m_pActor[m_iLastUpdateActorForNPGL];

		if (lpActor->m_iZoneSerial	==	0xffffffff)
			continue;

		if (!lpActor->isPlayer() || lpActor->m_iLastSendGGAuthCodeTime > g_iCounterBySecond-60*5)
			continue;

		CClient	*lpClient	=	lpActor->getClient();

		if (!lpClient)
			continue;

		iSendActorCount++;

		DWORD	dwResult	=	lpClient->m_csa.GetAuthQuery();

		if (dwResult	!=	0)
		{
			if (g_bIsDisconnectGGAuthFailedUser)
			{
				printf("Disconnect GG Auth Failed User [%s/%s] \n",lpActor->m_strId,lpActor->m_strName);
				printf("    by Error Code [#%.3d]\n",dwResult);
				CLOG("GG Auth","Loop : Disconnect GG Auth Failed User [%s/%s] by Error Code [#%.3d]",lpActor->m_strId,lpActor->m_strName,dwResult);

				lpActor->bookingReadyToExitGame();
			}
			continue;
		}

		lpActor->sendGGAuthCode(&lpClient->m_csa.m_AuthQuery);
	}

	if (m_iLastUpdateActorForNPGL	>=	m_iMaxActorCount)
		m_iLastUpdateActorForNPGL	=	0;
}
#endif

//
//	蟾渡 и廓 葬пй 機 
inline	void
cGAME::operateOncePerSecondWork()
{
	g_debugSign.m_iTempStep	=	0;

	if	(m_bIsChangeSecond)
	{
		if	(g_bIsStressTestServer)
			g_pStressTestManager->sendReport();

		if	(g_iBoostExpSecondByTreasureMapEventInServer)
		{
			g_iBoostExpSecondByTreasureMapEventInServer--;

			if	(g_iBoostExpSecondByTreasureMapEventInServer	==	0)
				g_game.sendEtcWorkPacketToAllPlayer(eEW_BOOST_EXP_BY_TREASER_MAP_EVENT_TIME_OVER);
		}

		if	(g_iBoostGoldSecondByTreasureMapEventInServer)
		{
			g_iBoostGoldSecondByTreasureMapEventInServer--;

			if	(g_iBoostGoldSecondByTreasureMapEventInServer	==	0)
				g_game.sendEtcWorkPacketToAllPlayer(eEW_BOOST_GOLD_BY_TREASER_MAP_EVENT_TIME_OVER);
		}

		g_guildManager.checkGuildCheckSum();
		g_pIfManager->operateOncePerSecondWork();
		g_fireWorkManager.operateOncePerSecond();
		g_pPartyManager->operateOncePerSecond();

		if	(g_bIsDuelServer)
			g_pDuelManager->update();

		if	(m_iFieldCount	==	1)
		{
			m_pField[0].operateOncePerSecondWork();

			return;
		}

#ifdef	_USE_NPGL
		operateOncePerSecondNPGLWork();
#endif
#ifdef	_USE_HS
		operateOncePerSecondHS_Work();
#endif
	}

	{
		g_debugSign.m_iTempStep	=	1;

		int	iValue		=	m_dwFrameCounter%dSYNC_FPS;
		int	iSharedCount=	max(m_iFieldCount/dSYNC_FPS,1);
		int	iBeginIndex	=	min(iSharedCount*iValue,m_iFieldCount-1);
		int	iModCount	=	m_iFieldCount%dSYNC_FPS;
		int	iEndIndex	=	iSharedCount*(iValue+1)-1;

		if (m_iOncePerSecondWorkBeginIndex==	iBeginIndex)
		{
			g_debugSign.m_iTempStep	=	-1;
			return;
		}

		g_debugSign.m_iTempStep	=	2;

		m_iOncePerSecondWorkBeginIndex	=	iBeginIndex;

		iEndIndex		=	min(m_iFieldCount-1,iEndIndex);
		int	iCount		=	iEndIndex-iBeginIndex+1;

		g_debugSign.m_iTempStep	=	3;

		for(int	iIndex=iBeginIndex;iCount;iCount--,iIndex++)
			m_pField[iIndex].operateOncePerSecondWork();

		g_debugSign.m_iTempStep	=	4;

		if	(iValue < iModCount && m_iFieldCount > dSYNC_FPS)
			m_pField[iValue+dSYNC_FPS*iSharedCount].operateOncePerSecondWork();
	}

	g_debugSign.m_iTempStep	=	-1;
}

//
//	и 等檜攪 薯剪
void
cGAME::removeSaveData(char *_lpstrId,int _iAvatarIndex,BOOL _bIsIncludeOldData)
{
	char	strFileName[256];

	sprintf(strFileName,"%s/save/%s[%d].sav",g_strLogFolder,_lpstrId,_iAvatarIndex);
	DeleteFile(strFileName);

	if (_bIsIncludeOldData)
	{
		sprintf(strFileName,"%s/save/%s[%d][old].sav",g_strLogFolder,_lpstrId,_iAvatarIndex);
		DeleteFile(strFileName);

		if(_bIsIncludeOldData==2)
		{
			sprintf(strFileName,"%s/backup/%s[%d].sav",g_strLogFolder,_lpstrId,_iAvatarIndex);
			DeleteFile(strFileName);
		}
	}
}


/*
void
cGAME::checkClient()
{
	for (int i=0;i<dGAME_MAX_USER_COUNT;i++)
	{
		CClient	*lpClient	=	CM.mGETCLIENT(i);

		if (lpClient	==	NULL)	continue;
		if (lpClient->GetActorSerialInServer() == 0xffff)	continue;

		lpClient->GetActorSerialInField()
	}
}*/

int
cGAME::operteLogoutActorWorkById(char *_lpstrId)
{
	int		iCount		=	0;

	CClient	*lpClient	=	CM.getClientByIdIgnoreUsedFlag(_lpstrId);

// 	if	(lpClient)
// 		sendWPUserLogoutById(_lpstrId,lpClient->ip);
// 	else
// 		sendWPUserLogoutById(_lpstrId,"EMPTY");

 	DisUser(_lpstrId,"EMPTY");
// 
// 	while(lpClient && lpClient->wSerial != 0xffff)
// 	{
// 		if	(!CM.removeClientWithCheckId(lpClient->GetSerial(),_lpstrId))
// 			break;
// 
// 		lpClient	=	CM.getClientByIdIgnoreUsedFlag(_lpstrId);
// 
// 		iCount++;
// 	}

	return	iCount;
}	//	cGAME::operteLogoutActorWorkById(char *_lpstrId)

int
cGAME::operteLogoutActorWorkByName(char *_lpstrName)
{
	int		iCount		=	0;
	CClient	*lpClient	=	CM.getClientByNameIgnoreUsedFlag(_lpstrName);
	char	strId[dID_LENGTH];

	strId[0]	=	NULL;
	char	strIP[dIP_SIZE]	=	"EMPTY";

	while(lpClient && lpClient->wSerial !=0xffff)
	{
		strcpy(strId,lpClient->GetID());
		strcpy(strIP,lpClient->ip);

		CM.removeClientWithCheckId(lpClient->GetSerial(),strId);

		lpClient	=	CM.getClientByIdIgnoreUsedFlag(strId);

		iCount++;
	}

	if	(iCount)
	{
		DisUser( strId, "TEST" );
	}
//		sendWPUserLogoutById(strId,strIP);

	return	iCount;
}

//	 鼻薄曖 薑爾 機等檜お и.
void
cGAME::rebuildAllShopInfo()
{
	for (int i=0;i<m_iFieldCount;i++)
		m_pField[i].m_shop.generateItem();

	if (m_iPlayerCount == 0)
		return;

	SG_FAILED_SELL_ITEM	packetFailed;

	packetFailed.base.set(sizeof(SG_FAILED_SELL_ITEM),dSG_FAILED_SELL_ITEM);
	packetFailed.wReason	=	eFRSI_CHANGE_ITEM_PRICE;

	sendPacketToAllPlayer((char *)&packetFailed);
}

BOOL
cGAME::checkIncorrectInfo()
{
	for (int i=m_iFieldCount;i;--i)
		m_pField[i-1].checkIncorrectInfo();

	return	TRUE;
}

int
cGAME::findEmptyDuelField(int _iDuelMatchType)
{
	int	iFieldSlot	=	m_iFieldCount;
	int	iSerial		=	0;

	for (int iField=0;iField<m_iFieldCount;iField++)
	{
		if	(m_pField[iField].m_wDuelType	!=	_iDuelMatchType)
			continue;

		if	(m_pField[iField].m_wDuelMatchStep	==	eDUEL_MATCH_STEP_IDLE)
			return	iField;
	}

	return	-1;
}

cFIELD*
cGAME::getFieldByUniqueSerial(int _iSerial,DWORD _dwPartyUniqueSerial,int _iInstanceFieldSerial,int _iInstanceFieldFloor,BOOL _bIsForLoadGuildHall,int _iCreateTimeValue)//堅嶸衛葬橡煎 в萄 朝.
{
	cFIELD	*lpField	=	NULL;

	if	(_iSerial	<	dINSTANCE_FIELD_FLAG)
	{
		for (int i=0;i<m_iFieldCount;i++)
			if	(m_pField[i].m_iUniqueSerial	==	_iSerial)	
			{
				lpField	=	&m_pField[i];

				break;
			}
	}
	else
		lpField	=	g_pIfManager->getFloor(_iInstanceFieldSerial,_iInstanceFieldFloor,_dwPartyUniqueSerial,_iCreateTimeValue);

	if	(!lpField)
		return	NULL;

	return	lpField;
}

cFIELD*
cGAME::getField(int _iIndex)//_iIndex廓簞 в萄 葬и.
{
	if (_iIndex	>=	dINSTANCE_FIELD_FLAG)
		return	g_pIfManager->getFloor(_iIndex/dINSTANCE_FIELD_FLAG,_iIndex%dINSTANCE_FIELD_FLAG);

	if (_iIndex	>=	m_iFieldCount || _iIndex < 0)
		return	NULL;

	return	&m_pField[_iIndex];

}

cFIELD_INFO*
cGAME::getFieldInfo(int _iFieldSerial)
{
	for	(int	i=0;i<m_iServerCount;i++)
	{
		cFIELD_INFO	*lpInfo	=	m_aServerList[i].getFieldInfo(_iFieldSerial);

		if	(lpInfo)
			return	lpInfo;
	}

	return	NULL;
}

void
cGAME::resetGuildHallFieldInfo()
{
	for	(int	i=0;i<m_iFieldCount;i++)
		m_pField[i].m_wHallOwnGuild	=	0xffff;
}

void
cGAME::resetSiegeWarfareFieldStatus()
{
	for	(int	i=0;i<m_iFieldCount;i++)
	{
		if	(m_pField[i].m_bIsSiegeWarfareField	==	FALSE)
			continue;

		m_pField[i].resetSiegeWarfareStatus();
	}
}

BOOL
cGAME::addReceivePacket(int _iField,ALL_MSG* _lpPacket,int _iSerial)
{
	if	(_iField < dINSTANCE_FIELD_FLAG)
		return	m_pField[_iField].addReceivePacket(_lpPacket,_iSerial);
	else
	{
		cFIELD	*lpField	=	g_pIfManager->getFloor(_iField/dINSTANCE_FIELD_FLAG,_iField%dINSTANCE_FIELD_FLAG);

		if	(lpField)
			return	lpField->addReceivePacket(_lpPacket,_iSerial);
	}

	return	FALSE;
}

BOOL
cGAME::isActiveGameServer()
{
	if	(!g_bIsCompleteReceiveGuildDataWork)
		return	FALSE;

	if	(!g_bIsCompleteReceiveLottoEventData)
		return	FALSE;
	
	if	(g_bGameClose)
		return	FALSE;

	if	(g_eventManager.m_bIsReceiveGoldRushInfo	==	FALSE)
		return	FALSE;

	if	(g_itemPackList.checkReceiveAllInfo()		==	FALSE)
		return	FALSE;

	if	(!g_carrotShopInfo.m_bIsCompleteReceiveCarrotShopInfo)
		return	FALSE;

	if	(m_bIsReceiveAllData	==	FALSE)
	{
		m_bIsReceiveAllData		=	TRUE;

		WriteServerStatusToRegistry(eST_GAME,eSERVER_STATUS_ON);
	}

	return	TRUE;
}

void
cGAME::receiveInitDataFromWorld()
{
	m_wCheckCountForReceiveInitDataFromWorld	=	3;
}

BOOL
cGAME::setHeroBodyFolder()		
{
	return	FPSetCurrentFolder(_ms("%s/heros",m_strDataFolder));
}

BOOL
cGAME::setEffectDataFolder()	
{
	return	FPSetCurrentFolder(_ms("%s/Effects",m_strDataFolder));
}

BOOL
cGAME::setMonsterBodyFolder()
{
	return	FPSetCurrentFolder(_ms("%s/monsters",m_strDataFolder));
}

BOOL
cGAME::setNpcBodyFolder()
{
	return	FPSetCurrentFolder(_ms("%s/npc",m_strDataFolder));
}

BOOL
cGAME::setMapFolder()
{
	return	FPSetCurrentFolder(_ms("%s/map",m_strDataFolder));
}

int		g_iXtrapMapFileCount=	1;
char	*g_pXtrapMapFile	=	NULL;

#define	dXTRAP_MAP_FILE_SIZE	13000

#ifdef	_USE_XTRAP
bool
BeginXtrap(int _iCount)
{
	g_iXtrapMapFileCount	=	_iCount;
	g_pXtrapMapFile			=	new char [_iCount*dXTRAP_MAP_FILE_SIZE];

	XTrap_S_LoadDll();

	char	*strFiles[]	=	{"release.CS3","release2.CS3","release3.CS3","release4.CS3","release5.CS3","release6.CS3"};
	
	cFILE	file;

	for(int i=0;i<g_iXtrapMapFileCount;i++)
	{
		if	(!file.Open(_ms("%s/%s",g_strServerFolder,strFiles[i]),"rb"))
			return	false;

		file.Read(g_pXtrapMapFile+dXTRAP_MAP_FILE_SIZE*i,dXTRAP_MAP_FILE_SIZE);
		file.Close();
	}
	
	int	iResult	=	XTrap_S_Start(600,g_iXtrapMapFileCount,g_pXtrapMapFile,NULL);
	
	if	(iResult	!=	0)
		cMSG::Error("XTrap - error!!","in XTrap_S_Start %d",iResult);

	return	true;
}

void
EndXtrap()
{
	pKILL(g_pXtrapMapFile);
}

#endif