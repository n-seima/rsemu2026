#ifndef _DEFINE_PACKET_DATA__
#define _DEFINE_PACKET_DATA__

#include <winsock2.h>
#ifndef _WINDOWS_
#include <windows.h>
#endif
#include "RSComm.h"
#include "CItemDefine.h"
#include "CSkillDefine.h"
#include "CPetDefine.h"
#include "summonBeastDefine.h"
#include "miniPetCommon.h"

const	int	c_iCH5BonusHPPerLevel	=	10;
const	int	c_iCH5BonusCPPerLevel	=	10;

namespace	nsModifySaveData
{
	enum
	{
		Last
	};
};

#define dMAX_STORE_ITEM_COUNT_BY_LOG		10
#define dBUY_ITEM_COUNT						10
#define	dTRADE_ITEM_COUNT					18
#define dFILEDNAME_LENGTH					34
#define	dCHAT_LENGTH						128
#define	dABILITY_COUNT						52	//æ˚÷Ù∑Ø∆º
#define	dMAX_SERVER_COUNT					20
#define	dDROPPING_ITEM_COUNT				10	//	∂≥æ˚—Â∏Æ¥¬ æ∆¿Ã≈€ ∆–≈œ
#define	dITEM_MONEY							0	//	µ∑ -o-
#define	dMAX_OPERATE_DROPPED_ITEM			20
#define	dMAX_TARGET_FOR_WIDE_AREA_ATTACK	32
#define	dMAX_PROCESS_QUEST_COUNT			6
#define	dQUEST_BUFFER_SIZE					20
#define	dARBEIT_BUFFER_SIZE					50

#define dTOKEN_LENGTH	64

#ifdef _FOR_INTER

	#ifndef dID_LENGTH
		#define	dID_LENGTH							64
	#endif 

	#ifndef dNAME_LENGTH
		#define	dNAME_LENGTH						18
	#endif 

	#ifndef dPASS_LENGTH
		#define	dPASS_LENGTH						20
	#endif

#else

	#ifndef dID_LENGTH
		#define	dID_LENGTH							20
	#endif 

	#ifndef dNAME_LENGTH
		#define	dNAME_LENGTH						18
	#endif 

	#ifndef dPASS_LENGTH
		#define	dPASS_LENGTH						20
	#endif

#endif 



#define	dQUEST_BUFFER_EXTEND_SIZE			30


#define	dEXTRA_INVENTORY_SIZE				16

#define dMAX_PARTY_COUNT					1000
#define dPARTYNAME_LENGTH					22	//	∆ƒ∆º¿Ã∏ß ±Ê¿Ã
#define dPARTY_MAXPLAYER					8	//	∆ƒ∆º¿« √÷¥ÅE¿Œø¯ºÅE
#define dPARTYLIST_COUNT					8	//	«—π¯ø° ∫∏≥ª¥¬ ∆ƒ∆º ∏ÆΩ∫∆Æ ºÅE

#define dUNION_PARTY_COUNT					8	//	∆ƒ∆º¿Ã∏ß ±Ê¿Ã
#define dMAX_UNION_PARTY_COUNT				256	//	∆ƒ∆º¿Ã∏ß ±Ê¿Ã
#define dUNION_PARTYNAME_LENGTH				22	//	∆ƒ∆º¿Ã∏ß ±Ê¿Ã
#define dMAX_PLAYER_TITLE_COUNT				50

#define	dDISSOLUTION_PARTY_AT_MOVE_FIELD	0xfffe

//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠«√∑π¿ÃæÅEµ•¿Ã≈Õ ¡§¿«
class	CJobBasicDataDefine	//	±‚∫ª ¡˜æ˜ø° « ø‰«— µ•¿Ã≈Õ
{
public:
	int				m_iLevel;							//	∑π∫ß
	int				m_iExperience;						//	∞Ê«Ëƒ°
	DWORD			m_dwSkillExperience;				//	Ω∫≈≥ ∞Ê«Ëƒ°

	int				m_iHP;								//	«ˆ¿ÅE√º∑¬			10.00	10,000.00  
	int				m_iMaxHP;							//	√÷¥ÅE√º∑¬			10.00	10,000.00  
	int				m_iCP;								//	«ˆ¿ÅE  ∆ƒøÅE∞‘¿Ã¡ÅE  1.00	 5,000.00  
	int				m_iMaxCP;							//	∏∆Ω√∏ÿ ∆ƒøÅE∞‘¿Ã¡ÅE10.00	10,000.00  
	WORD			m_wCorrectMaxHPFactor;				//	√÷¥ÅEHP ∞ËªÅEƒø° « ø‰«— HP ¿Œ¿⁄
	WORD			m_wCorrectMaxHPConstitutionFactor;	//	√÷¥ÅEHP ∞ËªÅEƒø° « ø‰«— ∞«∞≠ ¿Œ¿⁄

	short			m_sStrength;						//	»ÅE		5 1,000
	short			m_sAgility;							//	πŒ√∏º∫		5 1,000
	short			m_sConstitution;					//	∞«∞≠		5 1,000
	short			m_sWisdom;							//	¡ˆ«˝		5 1,000
	short			m_sIntelligence;					//	¡ˆΩƒ		5 1,000
	short			m_sCharisma;						//	ƒ´∏ÆΩ∫∏∂	5 1,000
	short			m_sLuck;							//	øÅE		5 1,000
	short			m_sSight;							//	Ω√æﬂ

	short			m_sMinDamage;						//	√÷º“ µ•πÃ¡ÅE	
	short			m_sMaxDamage;						//	√÷¥ÅEµ•πÃ¡ÅE	
	short			m_sDefensivePower;					//	πÊæ˚”¬

	short			m_sAllignment;						//	º∫«ÅE-100.00~100.00(absoulte evil,evil,neutral,good,absoulte good)

	short			m_sFireResistance;					//	∫“ ¿˙«◊∑¬ 0% 100.00%  
	short			m_sWaterResistance;					//	π∞ ¿˙«◊∑¬ 0% 100.00%  
	short			m_sWindResistance;					//	πŸ∂ÅE¿˙«◊∑¬ 0% 100.00%  
	short			m_sEarthResistance;					//	¥ÅEÅE¿˙«◊∑¬ 0% 100.00%  
	short			m_sLightResistance;					//	∫ÅE¿˙«◊∑¬ 0% 100.00%
	short			m_sDarkResistance;					//	æ˚—“ ¿˙«◊∑¬ 0% 100.00%

	short			m_sBlindResistance;					//	¿Â¥‘ ¿˙«◊
	short			m_sPoisonResistance;				//	¡ﬂµ∂ ¿˙«◊
	short			m_sSleepResistance;					//	¿ÅE¿˙«◊
	short			m_sColdResistance;					//	ƒ›µÅE¿˙«◊
	short			m_sFreezeResistance;				//	«¡∏Æ¡ÅE¿˙«◊
	short			m_sStunResistance;					//	∞Ê¡ÅE¿˙«◊
	short			m_sStoneResistance;					//	ºÆ»≠ ¿˙«◊
	short			m_sConfuseResistance;				//	»•∂ÅE¿˙«◊
	short			m_sCharmingResistance;				//	√≠π÷ ¿˙«◊

	short			m_sBadStatusResistance;				//	ªÛ≈¬ ¿ÃªÛ∞¯∞›ø° ¥ÅE— ¿˙«◊ 0% 100.00%
	short			m_sDeclinePowerResistance;			//	¥…∑¬ƒ° ¿˙«œ∞Ëø° ¥ÅE— ¿˙«◊ 0% 100.00%
	short			m_sCurseResistance;					//	¿˙¡÷ø° ¥ÅE— ¿˙«◊ 0% 100.00%
};

class cP_4POINT
{
public:
	int		x,y;
	int		mx,my;
};

class cP_POINT
{
public:
	int		x,y;
};

#pragma	pack(1)

#define	dITEM_PREFIX_COUNT	3

class	cP_ITEM_PREFIX_INFO
{
public:
	WORD		m_wPrefix;
	BYTE		m_aValue[2];
};

class	cP_ITEM
{
public:
	DWORD					m_dwSerial;
	WORD					m_wBaseItem;					//	æ∆¿Ã≈€ ¿Œµ¶Ω∫
	BYTE					m_bCount;						//	º˝¿⁄
	BYTE					m_bDurability;					//	≥ª±∏∑¬
	BYTE					m_aOption[2];					//	ª˝º∫µ…∂ß ∫Ÿ¿∫ ø…º«
	cP_ITEM_PREFIX_INFO		m_aPrefix[dITEM_PREFIX_COUNT];	//	√ﬂ∞°»ø∞ÅE3∞≥
};

#pragma	pack()

#pragma	pack(2)

class	cP_ACTOR_EQUIPMENT
{
public:
	WORD			m_wItemCount;			//	æ∆¿Ã≈€¿« ºÅE	
	cP_ITEM			m_aEquip[dEQUIPMENT_PART_COUNT];
	cP_ITEM			m_aItems[dOWN_ITEM_COUNT];
};

//
//	« µÂø° ∂≥æ˚›ÅEæ∆¿Ã≈€
class	cP_DROPPED_ITEM
{
public:
	DWORD			m_bf10Serial	:	10;
	DWORD			m_bf14BaseItem	:	14;
	DWORD			m_bf4Type		:	4;	//	≈∏¿‘
	WORD			m_wCount;
	WORD			wX,wY;
};	//	class	cDROPPED_ITEM

class	CPlayerTitleInfo
{
public:
	BYTE				m_bTitle;
	BYTE				m_bLevel;
};

#pragma	pack()

//
//	Operater Status
enum
{
	eOS_NORMAL		=	1,	//	≥ÅEª
	eOS_HIDE		=	2,	//	¿·«ÅE_-
	eOS_ANNOUNCE	=	3,	//	±§∞ÅE.
};

//
//	Operater Level
enum
{
	eOL_0			=	0,	//	¿œπ› «√∑π¿ÃæÅE	eOL_1			=	1,	//	∏∂Ω∫≈Õ(¿ÅE…-_-)
	eOL_2			=	2,	//	¡˜øÅE	eOL_3			=	3,	//	æÀπŸª˝
};

enum
{
	eQCTS_COMPLETE,
	eQCTS_PROCESS,
	eQCTS_CANCEL,
	eQCTS_SLEEP,
};

typedef struct 
{
	DWORD	isActive			:	1;
	DWORD	questKind			:	2;
	DWORD	questProcessLevel	:	4;
	DWORD	questIndex			:	10;
	DWORD	questValue			:	11;
	DWORD	isComplete			:	1;
	DWORD	countdownStatus		:	2;
} tsProcessQuestField;

typedef struct 
{
//	WORD	isDenySay				:	1;
	WORD	isDenyCommunity			:	1;
	WORD	isDenyParty				:	1;
	WORD	isDenyTrade				:	1;
	WORD	isResetDataWhen20031008	:	1;
	WORD	isResetDataWhen20031027	:	1;
	WORD	isResetDataWhen20031106	:	1;
} tsSpecialField;

typedef union _union_time
{
    struct 
	{
		DWORD	year		:	6;	//	ø¨
		DWORD	month		:	4;	//	øÅE		
		DWORD	day			:	5;	//	¿œ
		DWORD	hour		:	5;	//	Ω√
		DWORD	minute		:	6;	//	∫–
		DWORD	second		:	6;	//	√ 
    };

    DWORD	m_dwValue;
}	UTime;

#define	dMAX_PLAYER_WAY_POINT_COUNT	10

class	CPlayerWayPointInfo
{
public:
	DWORD	m_bf12Field : 12;
	DWORD	m_bf10XPos	: 10;
	DWORD	m_bf10YPos	: 10;
};

//
//	Premium Service
enum
{
	ePS_FREE	,	//	∞¯¬• ¿Ø¿ÅE	
	ePS_LEVEL_1	,	//	∑π∫ß 1
	ePS_LEVEL_2	,	//	∑π∫ß 2
	ePS_LEVEL_3	,	//	∑π∫ß 3
};

class	CPlayerSaveDataForClientDefine :	public CJobBasicDataDefine , public CPlayerEquipmentDataDefine
{
public:
	char				m_strId[dID_LENGTH];//ID
	char				m_strName[dNAME_LENGTH];//¿Ã∏ß

	WORD				m_wJob;				//	«ˆ¿ÅE¡˜æÅE	
	int					m_iGold;			//	µ∑
	WORD				m_wLevelPoint;		//	∑π∫ßæÅE∆˜¿Œ∆Æ

	WORD				m_wCurrentField;	//«ˆ¿ÅE« µÅE
	int					m_iXPos,m_iYPos;
	WORD				m_wLastVillage;		//∏∂¡ˆ∏∑¿∏∑Œ ∂∞≥™ø¬ ∏∂¿ª

	WORD				m_wGuildSerial;
	WORD				m_wGuildRank;
	WORD				m_wBonusSkillPoint;

	WORD				m_bf1IsDenyCommunity							:	1;
	WORD				m_bf1IsAddPetSystemBy20040916					:	1;	//	∆ÅEΩ√Ω∫≈€ Ω√¿€
	WORD				m_bf1IsReadyToMakeGuild							:	1;
	WORD				m_bf1IsAddFailedPremiumItemInfoAndResetWayPoint	:	1;
	WORD				m_bf1IsResetPremiumFieldInItem					:	1;
	WORD				m_bf1IsBeginPremiumService						:	1;

	WORD				m_bf1IsReceiveRiceCakeSoup						:	1;
	WORD				m_bf1IsAddQuestCountDownTimer20055027			:	1;	//	∫Û∞≈

	WORD				m_bf1IsIncorrectConnectByGuildBattle			:	1;
	WORD				m_bf1IsResetHidePitchmanShop20041102			:	1;	//	∫Û∞≈
	WORD				m_bf3UseBoostCount								:	3;
	WORD				m_bf1AddResetSkillStateScroll					:	1;
	WORD				m_bf1IsHidePitchmanShop							:	1;
	WORD				m_bf1AddData2005_03_03							:	1;	//	16 ∏µŒ ªÁøÅE‘

	CAbilityDefine		m_aAbility[dABILITY_COUNT];	//æ˚÷Ù∑Ø∆º
	CPlayerTitleInfo	m_aTitle[dMAX_PLAYER_TITLE_COUNT];
	tsProcessQuestField	m_aProcessQuest[dMAX_PROCESS_QUEST_COUNT];
	CPlayerWayPointInfo	m_aWayPoint[dMAX_PLAYER_WAY_POINT_COUNT];
};

class	CPlayerSaveDataForServerDefine	:	public CPlayerSaveDataForClientDefine
{
public:
	DWORD				m_dwPlayTime;					//	«√∑π¿Ã ≈∏¿” ∫– ¥‹¿ß

	DWORD				m_bf12DeathPeneltySecond		: 12;	//	¡◊¿Ω¿∏∑Œ ¿Œ«— ¥…∑¬ƒ° ∆‰≥Œ∆º ≈∏¿”
	DWORD				m_bf6ProhibitCommunityMinute	: 6;
	DWORD				m_bf5LastUpdateDay				: 5;
	DWORD				m_bf1WaitForRecallForKelby		: 1;
	DWORD				m_bf1WaitForRecallForSwellfer	: 1;
	DWORD				m_bf1WaitForRecallForWindy		: 1;
	DWORD				m_bf1WaitForRecallForHedger		: 1;
	DWORD				m_bf4RemainTimeForUseBreedingRecordBook	:	4;

	DWORD				m_dwSaveCount;
	DWORD				m_dwPeneltyTime;

	WORD				m_bf1IsPlayOnGuildBattle		:	1;	//	±ÊµÂ¿ÅE¡ﬂ¿Ã¥Ÿ
	WORD				m_bf5GuildBattleDay				:	5;	//	±ÊµÂ¿ÅE¿‘¿Â«— ≥Ø
	WORD				m_bf1IsTeleportToCouple			:	1;	//	ƒø«√∏µ¿ª ªÁøÅEÿ ≈⁄∑π∆˜∆Æ¡ﬂ¿Ã¥Ÿ.
	WORD				m_bf7UseCoupleRingSlot			:	7;	//	ªÁøÅE— ƒø«√∏µ æ∆¿Ã≈€ ΩΩ∑‘
	WORD				m_bf1CancelQuestWork			:	1;
	WORD				m_bf1CancelQuestWork2			:	1;	//	16

	WORD				m_wAvatarIndex;				//	æ∆πŸ≈∏ ¿Œµ¶Ω∫
	WORD				m_wRequitalCounter;			//	«¡∏ÆπÃæÅEæ∆¿Ã≈€ ∫∏ªÅEƒ´ø˚·Õ

	BYTE				m_aCompleteQuest[dQUEST_BUFFER_SIZE];

	WORD				m_bf10ESD_Time			:	10;	//	enter secret dungeon time
	WORD				m_bf6ExtraInventorySize	:	6;	//	16

	UTime				m_timeGuildJoinTime;		//	±ÊµÅE∞°¿‘«— Ω√∞£
	CItemDefine			m_itemFailedPremiumItem;

	cPetDefine			m_aPet[dOWN_PET_COUNT];

	WORD				m_bf8ModifyDataCounter	:	8;
	WORD				m_bf5ActiveMiniPet2		:	5;
	WORD				m_bf3GreateGodGrace		:	3;

	BYTE				m_bRemainSecondForUseGateGlove;
	BYTE				m_bf1IsMoveFieldByDeath		:	1;	//	¡◊æ˚ÿ≠ ∏∂¿ª∑Œ µπæ∆ ø‘¥Ÿ.
	BYTE				m_bf1IsMoveFieldByTeleport	:	1;	//	¡◊æ˚ÿ≠ ∏∂¿ª∑Œ µπæ∆ ø‘¥Ÿ.
	BYTE				m_bf1IsAddIncreaseLastDamageBy20070508	:	1;
	BYTE				m_bf1IsChangePetJobData	:	1;
	BYTE				m_bf1IsAddCH5System		:	1;
	BYTE				m_bf3EventCounter		:	3;
	
	UTime				m_timeLastNomannerPointTime;
	BYTE				m_aCompleteArbeit[dARBEIT_BUFFER_SIZE];

	CItemDefine			m_aExtraInventory[dEXTRA_INVENTORY_SIZE];
	WORD				m_wBonusStatePoint;

	DWORD				m_bf5GiveANoMannerPointDay	:	5;
	DWORD				m_bf8InstanceField			:	8;
	DWORD				m_bf4InstanceFieldFloor		:	4;
	DWORD				m_bf4SecretDungeonPlayCount	:	4;
	DWORD				m_bf1IsAnimatePartnerStatus	:	1;	//	22
	DWORD				m_bf1IsResetRedStonePoint	:	1;
	DWORD				m_bf1IsAddAboutMainQuestInfo:	1;
	DWORD				m_bf1IsSaveUserCustomData	:	1;
	DWORD				m_bf1IsReadyToSelectGuildMark:	1;
	DWORD				m_bf1IsAddMiniPetSystem		:	1;	//	27
	DWORD				m_bf5ActiveMiniPet			:	5;	//	«ˆ¿ÅE»∞º∫»≠µ» πÃ¥œ∆ÅE
	WORD				m_awQuestCountDownTimer[dMAX_PROCESS_QUEST_COUNT];

	WORD				m_wRedStonePoint;
	WORD				m_wBoostExpSecondByTreasureMapEvent,m_wBoostGoldSecondByTreasureMapEvent;

	WORD				m_wProcessChapterIndex;	//	¡¯«ÅE¡ﬂ¿Œ ∏ﬁ¿Œ ƒ˘Ω∫∆Æ √©≈Õ ¿Œµ¶Ω∫. GVG º≠πÅEƒ≥∏Ø≈Õ¥¬ ¿Ã ¡§∫∏∞° 1¿Ã∏ÅE¿Ã∏ßø° '_' ∞° æ»∫Ÿ∞ÅEπŸ∑Œ æÀ∆ƒ∫™¿Ã ∫Ÿ¿∫ ∞ÊøÅEŸ.
	WORD				m_wProcessHighQuestIndex;

	WORD				m_awMainQuestValue[10];
	BYTE				m_abMainQuestSwitchStatus[12];

	WORD				m_awSkillCoolTime[c_iCoolTimeSkillCount];

	int					m_iTodayWithdrawGoldInGuild;
	BYTE				m_abTodayWithdrawItemCount[4];

	BYTE				m_aCompleteQuest2[dQUEST_BUFFER_EXTEND_SIZE];
	
	WORD				m_wIncreaseFinalDamageTime;
	WORD				m_wIncreaseFinalDamage;

	cMiniPetCommon		m_aMiniPet[c_iMiniPetCount];

	UINT				m_reversalFrameYear	:	5;
	UINT				m_reversalFrameMonth:	4;
	UINT				m_reversalFrameDay	:	5;
	UINT				m_reversalFrameHour	:	5;
	UINT				m_reversalFrameMinute:	6;	//	25	
	UINT				m_bf1IsLevelDown	:	1;	//	26	¿˙¿Â«“∂ß ∑π∫ß ¥ŸøÅE«„∞°

//	2007 11 02 √©≈Õ 5 √ﬂ∞°∏¶ ¿ß«ÿ √ﬂ∞°«— µ•¿Ã≈ÕµÅE	
	WORD				m_bf4CH5StateBonusCount	:	4;
	WORD				m_bf4CH5SkillBonusCount	:	4;
	WORD				m_bf4CH5HPBonusCount	:	4;
	WORD				m_bf4CH5CPBonusCount	:	4;

	UINT				m_bf4CH5PeneltyHPBonusCount		:	4;
	UINT				m_bf4CH5PeneltyCPBonusCount		:	4;
	UINT				m_bf5BardTitleCoolTime			:	5;
	UINT				m_bf4LevelOfLatestReceivedCH5EquipLevelBonus	:	4;	//	∏∂¡ˆ∏∑¿∏∑Œ √©≈Õ 5 ¿Â∫ÅE∑π∫ß ∫∏¡§ ∫Œ¿ÅEπﬁæ“¿ª ∂ß ƒ™»£ ∑π∫ß
	UINT				m_bf3GuildDungeonPlayTime		:	3;	//	20
	UINT				:	0;	//	

	WORD				m_awItemCoolTime[dITEM_COOL_TIME_BUFFER_SIZE];
	
	UINT				m_bf4EatFireMiniPetCount	:	4;
	UINT				m_bf4EatWaterMiniPetCount	:	4;
	UINT				m_bf4EatWindMiniPetCount	:	4;
	UINT				m_bf4EatEarthMiniPetCount	:	4;
	UINT				m_bf4EatLightMiniPetCount	:	4;
	UINT				m_bf4EatDarkMiniPetCount	:	4;
	UINT				m_bf4RebirthCount			:	4;	//	¿ÅE˝ »ΩºÅE	
	UINT				m_bf4UseSealSphereOfDawnCount:	4;	//	ªı∫Æ¿« ∫¿¿Œ±∏ ªÁøÅE»ΩºÅE
	BYTE				m_aCompleteArbeit2[dARBEIT_BUFFER_SIZE];

	UINT				m_bf6LastTransAvatarToGVGYear		:	6;	//	∏∂¡ˆ∏∑¿∏∑Œ ¿ÅE€«— ø¨µµ
	UINT				m_bf4LastTransAvatarToGVGMonth	:	4;
	UINT				m_bf5LastTransAvatarToGVGDay		:	5;

};

class	cPLAYER_SAVE_DATA_BASE_FOR_CLIENT	:	public CJobBasicDataDefine , public cP_ACTOR_EQUIPMENT
{
public:
	char				m_strId[dID_LENGTH];//ID
	char				m_strName[dNAME_LENGTH];//¿Ã∏ß
	WORD				m_wAvatarIndex;//æ∆πŸ≈∏ ¿Œµ¶Ω∫

	WORD				m_wJob;//«ˆ¿ÅE¡˜æÅE	int					m_iGold;
	WORD				m_wLevelPoint;//∑π∫ßæÅE∆˜¿Œ∆Æ

	WORD				m_wLastVillage;//∏∂¡ˆ∏∑¿∏∑Œ ∂∞≥™ø¬ ∏∂¿ª
	WORD				m_wCurrentField;//«ˆ¿ÅE« µÅE	cP_4POINT			m_pos;//¿ßƒ°

	DWORD				m_dwPlayTime;	//	«√∑π¿Ã ≈∏¿” ∫– ¥‹¿ß
	int					m_iStolenExperience;
	WORD				m_wGuildSerial;
	WORD				m_wGuildRank;
	BYTE				m_bCurrentLevelDeathCount;
	BYTE				m_bDummyValue;
	char				m_strDummy[6];	//	20πŸ¿Ã∆Æ (dNAME_LENGTH +2) - 8 m_dwPlayTime,m_wGuildSerial,m_wGuildRank

	CAbilityDefine		m_aAbility[dABILITY_COUNT];	//æ˚÷Ù∑Ø∆º

	CPlayerTitleInfo	m_aTitle[dMAX_PLAYER_TITLE_COUNT];
	WORD				m_wBonusSkillPoint;
	tsSpecialField		m_sfInfo;
	DWORD				m_dwSaveCount;

	tsProcessQuestField	m_aProcessQuest[dMAX_PROCESS_QUEST_COUNT];

};

class	cPLAYER_SAVE_DATA_FOR_SERVER	:	public cPLAYER_SAVE_DATA_BASE_FOR_CLIENT
{
public:
	DWORD				m_dwEventCheckerCounter;	//	¿Ã∫•∆Æ √ºƒø
	DWORD				m_dwPeneltyTime;
	BYTE				m_aCompleteQuest[dQUEST_BUFFER_SIZE];
};

//
//	æ◊≈Õ µ•¿Ã≈Õ(º≠πˆø° ¿˙¿Â«“∂ß ¿Ã µ•¿Ã≈Õ∏¶ ¿˙¿Â«—¥Ÿ.)
class	cP_PLAYER_SAVE_DATA	:	public cPLAYER_SAVE_DATA_FOR_SERVER
{
public:
};

class	cACTOR_FACTOR_TYPE_FOR_SAVE
{
public:
	short			m_sValue;		//	¿Â∫Ò«— ªÛ≈¬ø°º≠¿« ºˆƒ°
	short			m_sFixValue;

	short			m_sCorrectValue;//	¿œΩ√¿˚¿∏∑Œ ¥ı«œ∞≈≥™ ª©æﬂ «œ¥¬ ºˆƒ°
	WORD			m_wTime;		//	¿Ø¡ÅEΩ√∞£
};

class cACTOR_GGG
{
public:	
	WORD				m_wGGGTimeForSkillUp;			//GGG	=	GreatGodGrace 
	
	WORD				m_wGGGValue1ForSkillUp;
	WORD				m_wGGGValue2ForSkillUp;
	WORD				m_wGGGAttackCoolTime;
	WORD				m_wGGGDefenceCoolTime;
	
	inline	void		ResetGGG()
	{
		ResetGGGSkill();
		m_wGGGAttackCoolTime = 0;
		m_wGGGDefenceCoolTime = 0;
	}
	
	inline void			ResetGGGSkill()
	{
		m_wGGGTimeForSkillUp	= 0;
		m_wGGGValue1ForSkillUp	= 0;
		m_wGGGValue2ForSkillUp	= 0;		
	}
};

//
//	« µÂ∏¶ ¿Ãµø«œ∞≈≥™ «ÿµµ ¿Ø¡ˆµ«¥¬ µ•¿Ã≈ÕµÅE
class	CUpkeepPlayerData
{
private:
	WORD					m_wParty;
public:
	WORD					m_wSpecialSkillField;	//	∆ØºÅEΩ∫≈≥ « µÅE_-?
	WORD					m_wMachine;
	WORD					m_bf10Costume					:	10;
	WORD					m_bf1IsEvilSpritOfChristmas		:	1;
	WORD					m_bf1IsHide						:	1;
	DWORD					m_bf4Skin						:	4;			//16

	DWORD					m_bf1IsMoveByChangeGuildHallOwner:	1;	//	±ÊµÂ»¶ ¡÷¿Œ¿Ã≥™ ±ÊµÂ»¶ ∏æÁ¿Ã πŸ≤˚⁄˚ÿ≠ ∞≠¡¶∑Œ ∏∂¿ª∑Œ ¿Ãµø Ω√ƒ◊¥Ÿ.
	DWORD					m_bf1IsMoveByEndOfGuildBattle	:	1;	//	±ÊµÂ¿ÅEÃ ≥°≥™º≠ ∞≠¡¶∑Œ ∏∂¿ª∑Œ ¿Ãµø Ω√ƒ◊¥Ÿ.
	DWORD					m_bf1IsMoveByNotGuildBattleTime	:	1;	//	±ÊµÂ¿ÅEﬂ¿Ã æ∆¥œ∂Ûº≠ ∏∂¿ª∑Œ ¿Ãµø Ω√ƒ◊¥Ÿ.
	DWORD					m_bf1IsMoveByOXQuizProcess		:	1;	//	¿ÃπÃ OX ƒ˚¡˚Ã° Ω√¿€«œ∞ÅE¿÷æ˚ÿ≠ ∏∂¿ª∑Œ ¿Ãµø Ω√ƒ◊¥Ÿ.
	DWORD					m_bf1IsMoveByNotOXQuizTime		:	1;	//	OX ƒ˚¡ÅEΩ√∞£¿Ã æ∆¥œ∂ÅE∏∂¿ª∑Œ ¿Ãµø Ω√ƒ◊¥Ÿ.
	DWORD					m_bf1IsMoveByEndOfOXQuiz		:	1;	//	OX ƒ˚¡˚Ã° ¡æ∑·µ«º≠ ∏∂¿ª∑Œ ¿Ãµø Ω√≈¥
	DWORD					m_bf1IsBannedSecretDungeon		:	1;			//23

	DWORD					m_bf5MagicCarpetShape			:	5;	//	«ˆ¿ÅEæÁ≈∫¿⁄ ∏æÅE	//28
	DWORD					m_bf1IsJoinGuildDungeon			:	1;	//	±ÊµÂ¥¯¿ÅE‘¿Â«œ¥¬¡ﬂ.		//29

	WORD					m_wBoostPower,m_wBoostTime;
	WORD					m_wWordEventTime;
	//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
	//¶≠∞≈Ω≈¿« ¿∫√—
	cACTOR_GGG				m_GGG;
	WORD					m_wGuildMasterGrace;
	//¶≠∞≈Ω≈¿« ¿∫√—
	//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

	cSummonBeastDefine		m_aSummonBeast[dOWN_SUMMON_BEAST_COUNT];

	CItemDefine				m_jewel;						//	∫∏ºÆ
	WORD					m_wJewelTime;

	WORD							m_wTransformationTime;		// ∫ØΩ≈ Ω√∞£
	WORD							m_wTransformationLevel;		// ∫ØΩ≈ ∑π∫ß
	WORD							m_wTransformationSkin;		// ∫ØΩ≈ Ω∫≈≤	
	WORD							m_wTransformationSkill;		// ∫ØΩ≈ Ω∫≈≥	

	inline	int				getPartySerial()				{return	m_wParty;}	//	∆ƒ∆ºø° µÈæ˚Ã¨¥Ÿ.
	inline	void			setPartySerial(int _iSerial)	{m_wParty	=	_iSerial;}	//	∆ƒ∆ºø° µÈæ˚Ã¨¥Ÿ.
	inline	char*			getUpkeepPlayerDataPointer()	{return	(char*)this;}
	inline	void			reset()
	{
		for (int i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
			m_aSummonBeast[i].reset();

		m_bf1IsMoveByEndOfGuildBattle	=	FALSE;
		m_bf1IsMoveByNotGuildBattleTime	=	FALSE;
		m_bf1IsMoveByOXQuizProcess		=	FALSE;	//	¿ÃπÃ OX ƒ˚¡˚Ã° Ω√¿€«œ∞ÅE¿÷æ˚ÿ≠ ∏∂¿ª∑Œ ¿Ãµø Ω√ƒ◊¥Ÿ.
		m_bf1IsMoveByNotOXQuizTime		=	FALSE;	//	OX ƒ˚¡ÅEΩ√∞£¿Ã æ∆¥œ∂ÅE∏∂¿ª∑Œ ¿Ãµø Ω√ƒ◊¥Ÿ.
		m_bf1IsMoveByEndOfOXQuiz		=	FALSE;
		m_bf1IsBannedSecretDungeon		=	FALSE;
		m_bf1IsHide						=	FALSE;
		m_bf1IsMoveByChangeGuildHallOwner=	FALSE;

		m_wBoostPower					=	100;
		m_wBoostTime					=	0;
		m_wParty						=	0xffff;
		m_wMachine						=	0xffff;
		m_bf10Costume					=	0;
		m_wSpecialSkillField			=	0;
		m_bf5MagicCarpetShape			=	0;
		m_wJewelTime					=	0;
		m_bf1IsEvilSpritOfChristmas		=	0;
		m_bf4Skin						=	0;
		m_wWordEventTime				=	0;
		m_wGuildMasterGrace				=	0;
		m_bf1IsJoinGuildDungeon			=	0;
		m_wTransformationTime			=	0;
		m_wTransformationLevel			=	0;
		m_wTransformationSkin			=	0xffff;
		m_wTransformationSkill			=	0xffff;
		m_GGG.ResetGGG();
	}
};





#define	dMAX_ITEM_IN_BANK			96
#define	dDEFAULT_BANK_SIZE			48

class	CBankInfo20031203
{
public:
	char		m_strId[dID_LENGTH];
	DWORD		m_dwGold;
	DWORD		m_dwSaveCounter;
	cP_ITEM		m_aItems[dMAX_ITEM_IN_BANK];

	void		reset()
	{
		m_strId[0]		=	NULL;
		m_dwGold		=	0;
		m_dwSaveCounter	=	0;
		memset(m_aItems,0xff,sizeof(m_aItems));
	}
};

class	CBankInfo20040108
{
public:
	char		m_strId[dID_LENGTH];
	DWORD		m_dwGold;
	DWORD		m_dwSaveCounter;
	CItemDefine	m_aItems[dMAX_ITEM_IN_BANK];

	void		reset()
	{
		m_strId[0]		=	NULL;
		m_dwGold		=	0;
		m_dwSaveCounter	=	0;
		memset(m_aItems,0xff,sizeof(m_aItems));
	}
};

class	CBankInfo
{
public:
	char		m_strId[dID_LENGTH];
	DWORD		m_dwGold;
	DWORD		m_dwSaveCounter;
	WORD		m_wRequitalCounter;
	CItemDefine	m_aItems[dMAX_ITEM_IN_BANK];

	void		reset()
	{
		m_strId[0]			=	NULL;
		m_dwGold			=	0;
		m_dwSaveCounter		=	0;
		m_wRequitalCounter	=	0;
		memset(m_aItems,0xff,sizeof(m_aItems));
	}
};

#define	dACTIVE_REACTION_COUNT	4

//¶≠«√∑π¿ÃæÅEµ•¿Ã≈Õ ¡§¿«
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

//
//	≈¨∂Û¿Ãæ∆Æ ¡¬«• ¡§∫∏
class	CMinimumActorInfo
{
public:
	WORD			bf11SerialInField	:	11;			//	« µÂæ»ø°º≠ «√∑π¿Ãæ˚‹« Ω√∏ÆæÅE	
	WORD			bf2Team				:	2;
	WORD			bf1IsNpc			:	1;

	WORD			wNpcActorSerial;

	WORD			wX,wY;
};	//	class	CMinimumActorInfo

//
//	≈¨∂Û¿Ãæ∆Æ ¡¬«• ¡§∫∏
class	cEventNpcInfo
{
public:
	DWORD			bf11Serial	:	11;		//11	« µÂæ»ø°º≠ «√∑π¿Ãæ˚‹« Ω√∏ÆæÅE	
	DWORD			bf7Type		:	7;		//18
	DWORD	:0;
	WORD			wX,wY;
};


//
//	ø˚€µ¿⁄ ¡§∫∏
class	COperatorInfo
{
public:
	WORD			wSerialInField;			//	« µÂæ»ø°º≠ ø˚€µ¿⁄¿« Ω√∏ÆæÅE	
	WORD			wOperatorLevel;
	char			strName[dNAME_LENGTH];
};

//
//	≈¨∂Û¿Ãæ∆Æ ¡¬«• ¡§∫∏
class	CActorInfoForOuterActor
{
public:
	WORD			wSerialInField;			//	« µÂæ»ø°º≠ «√∑π¿Ãæ˚‹« Ω√∏ÆæÅE	
	WORD			wX,wY;
	LONGLONG		dwEnchantedMask;
};	//	class	CMinimumActorInfo

#define	dVAILD_EQUIPMENT_VALUE_FOR_SAI		15
#define	dVAILD_EQUIPMENT_VALUE_FOR_ARMOR	3

class	CActorStatusBitField
{
public:
	DWORD			dancingBlocker		: 1;
	DWORD			shimmeringShield	: 1;
	DWORD			fountainBarrier		: 1;
	DWORD			misticFog			: 1;
	DWORD			tornadoShield		: 1;
	DWORD			rockBounding		: 1;
	DWORD			levitate			: 1;
	DWORD			stoned				: 1;
	DWORD			cold				: 1;
	DWORD			hotEnchanting		: 1;
	DWORD			blind				: 1;
	DWORD			berserk				: 1;
	DWORD			sleep				: 1;
	DWORD			confuse				: 1;
	DWORD			charming			: 1;
	DWORD			stun				: 1;
	DWORD			invisible			: 1;
	DWORD			blur				: 1;
	DWORD			freeze				: 1;
	DWORD			brokenArmor			: 1;
	DWORD			brokenWeapon		: 1;
	DWORD			sitDown				: 1;
	DWORD			protectingEvil		: 1;	
	DWORD			magicCarpetRider1	: 1;	
	DWORD			magicCarpetRider2	: 1;
	DWORD			mirrorMereMist		: 1;
	DWORD			displacement		: 1;
	DWORD			sanctuary			: 1;
	DWORD			holdPerson			: 1;
	DWORD			holdMonster			: 1;
	DWORD			evangelism			: 1;
	DWORD			burst				: 1;
};

//	≈¨∂Û¿Ãæ∆Æ∞° ±‚æÅEÿµ— « ø‰∞° ¿÷¥¬ √÷º“«—¿« ¡§∫∏
class	CSimplePlayerInfo
{
public:
	DWORD			bf2Kind				: 2;
	DWORD			bf8RestHp			: 8;
	DWORD			bf16MaxHp			: 16;
	DWORD			bf2Team				: 2;
	DWORD			bf2RideDog			: 2;	//	∞≥∏¶ ≈∏∞ÅE¿÷¥Ÿ.
	DWORD			bf1IsSyncHeroInfo	: 1;
	DWORD			bf1IsHide			: 1;
//	32∫Ò∆Æ

	DWORD			bf4Weapon			: 4;
	DWORD			bf8WeaponEffect		: 8;
	DWORD			bf4Shield			: 4;
	DWORD			bf2Armor			: 2;
	DWORD			bf3ArmorPalette		: 3;
	DWORD			bf11SerialInField	: 11;			//	« µÂæ»ø°º≠ «√∑π¿Ãæ˚‹« Ω√∏ÆæÅE//	32∫Ò∆Æ

	DWORD			bf14FirstAidHP		: 14;
	DWORD			bf10Job				: 10;
	DWORD			bf5MagicCarpetShape	: 5;
	DWORD			bf3OperatorLevel	: 3;
//	32∫Ò∆Æ

	DWORD			bf3Direct			: 3;
	DWORD			bf5Scale			: 5;
	DWORD			bf10Level			: 10;
	DWORD			bf4BlockerShape		: 4;
	DWORD			bf1IsEventMob		: 1;
	DWORD			bf1IsCopyActor		: 1;
	DWORD			bf1IsMarionette		: 1;
	DWORD			bf3GuildPetType		: 3;
	DWORD			bf1IsUnionPartyUser	: 1;
//	28∫Ò∆Æ

	DWORD			bf12GuildSerial		: 12;
	DWORD			bf5MiniPetType		: 5;
	DWORD			bf5MiniPetType2		: 5;
	DWORD			bf4RebirthCount		: 4;
	DWORD			bf1IsSoundOfLedersBellsLeader	:	1;	//	27∫Ò∆Æ

	WORD			wX,wY;					//	¿ßƒ°
	LONGLONG		dwEnchantedMask;		//	¿Œ√¶∆ºµÅE∏∂Ω∫≈©
	uEnchantedImage	enchantedImage;
	WORD			wItemEffect;

	char			strName[dNAME_LENGTH];
};

//
//	≈¨∂Û¿Ãæ∆Æ∞° ±‚æÅEÿµ— « ø‰∞° ¿÷¥¬ √÷º“«—¿« ¡§∫∏
class	CVerySimplePlayerInfo
{
public:
	DWORD			bf2Kind				: 2;
	DWORD			bf3Direct			: 3;
	DWORD			bf4Weapon			: 4;
	DWORD			bf8WeaponEffect		: 8;
	DWORD			bf4Shield			: 4;
	DWORD			bf2Armor			: 2;
	DWORD			bf3ArmorPalette		: 3;
	DWORD			bf2Team				: 2;
	DWORD			bf1IsEventMob		: 1;
	DWORD			bf1IsHide			: 1;
	DWORD			bf1IsCopyActor		: 1;
	DWORD			bf1IsMarionette		: 1;
//	32∫Ò∆Æ

	DWORD			bf11SerialInField	: 11;	//	« µÂæ»ø°º≠ «√∑π¿Ãæ˚‹« Ω√∏ÆæÅE	
	DWORD			bf5Scale			: 5;
	DWORD			bf3OperatorLevel	: 3;
	DWORD			bf2RideDog			: 2;	//	∞≥∏¶ ≈∏∞ÅE¿÷¥Ÿ.
	DWORD			bf1IsDeath			: 1;	//	µÿ¡≥≥ƒ?
	DWORD			bf10Job				: 10;
//	32∫Ò∆Æ

	DWORD			bf4MagicCarpetShape	:	4;
	DWORD			bf4BlockerShape		:	4;
	DWORD			bf3GuildPetType		:	3;
	DWORD			bf5MiniPetType		:	5;
	DWORD			bf5MiniPetType2		:	5;
	DWORD			bf1IsUnionPartyUser	:	1;
	DWORD			bf4RebirthCount		:	4;
	DWORD			bf1IsSoundOfLedersBellsLeader	:	1;	//	27∫Ò∫∆


	WORD			wX,wY;					//	¿ßƒ°
	LONGLONG		dwEnchantedMask;		//	¿Œ√¶∆ºµÅE∏∂Ω∫≈©
	uEnchantedImage	enchantedImage;
	WORD			wItemEffect;
};


class	CSimpleMonsterInfo
{
public:
	DWORD			bf2Kind				: 2;		//	«√∑π¿Ãæ˚œƒ?
	DWORD			bf10FirstAidHP		: 10;
	DWORD			bf10Level			: 10;
	DWORD			bf8Scale			: 8;
	DWORD			bf2Team				: 2;	//	∆¿
//	32∫Ò∆Æ

	DWORD			bf3Direct			: 3;	//	º≠ ¿÷¥¬ πÊ«ÅE	
	DWORD			bf1IsRegen			: 1;	//	∏Æ¡®µ»∞≈≥ƒ?
	DWORD			bf11SerialInField	: 11;	//	« µÂæ»ø°º≠ Ω√∏ÆæÅE	
	DWORD			bf1IsPet			: 1;	//	∆ÅEÃ¥Ÿ.
	DWORD			bf1RideTamer		: 1;
	DWORD			bf11Tamer			: 11;
	DWORD			bf4BlockerShape		: 4;
//	32∫Ò∆Æ

	DWORD			bf8RestHp			: 8;
	DWORD			bf24MaxHp			: 24;
//	32∫Ò∆Æ

	WORD			bf11Job				: 11;
	WORD			bf1IsCopyActor		: 1;
	WORD			bf1IsMarionette		: 1;
	WORD			bf1IsBoss			: 1;
	WORD			bf1IsHideName		: 1;	//	¿Ã∏ß º˚±‚±ÅE//	15

	WORD			wX,wY;				//	¿ßƒ°
	LONGLONG		dwEnchantedMask;	//	¿Œ√¶∆ºµÅE∏∂Ω∫≈©
	uEnchantedImage	enchantedImage;

	char			strName[dNAME_LENGTH];
};

class	CVerySimpleMonsterInfo
{
public:
	DWORD			bf2Kind				: 2;	//	«√∑π¿Ãæ˚œƒ?
	DWORD			bf3Direct			: 3;	//	º≠ ¿÷¥¬ πÊ«ÅE	
	DWORD			bf11SerialInField	: 11;	//	« µÂæ»ø°º≠ Ω√∏ÆæÅE	
	DWORD			bf8Scale			: 8;
	DWORD			bf2Team				: 2;	//	∆¿
	DWORD			bf1IsMove			: 1;	//	¿Ãµø¡ﬂ¿Ã¥Ÿ.
	DWORD			bf1IsPet			: 1;	//	∆ÅE	
	DWORD			bf1RideTamer		: 1;
	DWORD			bf1IsDeath			: 1;	//	µÿ¡≥≥ƒ?	//	30

	WORD			bf11Job				:	11;
	WORD			bf1IsCopyActor		:	1;
	WORD			bf1IsMarionette		:	1;
	WORD			bf1IsBoss			:	1;
	WORD			bf1IsHideName		:	1;	//	¿Ã∏ß º˚±‚±ÅE//	15

	WORD			wX,wY;						//	¿ßƒ°
	LONGLONG		dwEnchantedMask;		//	¿Œ√¶∆ºµÅE∏∂Ω∫≈©
	uEnchantedImage	enchantedImage;
};


class	CSimpleNpcInfo
{
public:
	DWORD			bf2Kind				: 2;	//	«√∑π¿Ãæ˚œƒ?
	DWORD			bf6ActorKind		: 6;	//	æ◊≈Õ ¡æ∑ÅE	
	DWORD			bf3Direct			: 3;	//	º≠ ¿÷¥¬ πÊ«ÅE	
	DWORD			bf11SerialInField	: 11;	//	« µÂæ»ø°º≠ Ω√∏ÆæÅE	
	DWORD			bf2Team				: 2;	//	∆¿
	DWORD			bf8Scale			: 8;
	
	WORD			bf11Job				:	11;
	WORD			bf1IsTalker			:	1;
	WORD			bf1IsHideName		:	1;	//	¿Ã∏ß º˚±‚±ÅE//	4∫Ò∆Æ ≥≤¿Ω

	WORD			wX,wY;						//	¿ßƒ°

	char			strName[dNAME_LENGTH];
};

class	CVerySimpleNpcInfo
{
public:
	DWORD			bf2Kind				:	2;	//	«√∑π¿Ãæ˚œƒ?
	DWORD			bf11SerialInField	:	11;	//	« µÂæ»ø°º≠ Ω√∏ÆæÅE	
	DWORD			bf11Job				:	11;
	DWORD			bf3Direct			:	3;	//	º≠ ¿÷¥¬ πÊ«ÅE	
	DWORD			bf1IsTalker			:	1;	//	28 bit
	DWORD			bf1IsHideName		:	1;	//	¿Ã∏ß º˚±‚±ÅE
	WORD			bf6ActorKind		:   6;	//	æ◊≈Õ ¡æ∑ÅE	
	WORD			bf2Team				:   2;	//	∆¿
	WORD			bf8Scale			:   8;  //	
	WORD			wX,wY;						//	¿ßƒ°
};


class cAVATAR_INFO
{
public:
	WORD			m_wAvatarIndex;
	char			m_strName[dNAME_LENGTH];		//	H ¿Ã∏ß - EMPTY - ¥ÅE“πÆ¿⁄ ±∏∫– æ¯±∏
	WORD			m_wJob;				//	¡˜æÅE	
	WORD			m_wLevel;			//	∑π∫ß
	WORD			m_wWeapon,m_wShield,m_wArmor;
	WORD			m_wLastField;		//	∏∂¡ˆ∏∑¿∏∑Œ ¿÷¥ÅE« µÅE-> ¿Ã∞…∑Œ æ∆∑° ¡÷º“µµ æÚæ˚ÿ≠ ¿˙¿Â«—¥Ÿ.
	char			m_strIP[dIP_SIZE];	//	∏∂¡ˆ∏∑¿∏∑Œ ¡¢º”«ÿ ¿÷¥ÅE∞‘¿”º≠πÅE¡÷º“
};

//
//	∞‘¿”º≠πˆ∞° ø˘µÂº≠πˆø° æ∆¿Ã≈€ ª˝º∫ Ω≈∞˙‘¶ «“∂ß « ø‰«— ¡§∫∏
class 	cCREATE_ITEM_INFO
{
public:
	WORD	m_wItem,m_wSerial;	// æ∆¿Ã≈€ ±‚∫ª ¿Œµ¶Ω∫øÕ « µÂø°º≠ µÂ∂ÅEæ∆¿Ã≈€ Ω√∏ÆæÅE
};

class 	cDROPPED_ITEM_FIRST_INFO
{
public:
};

//
//	ø˘µÂº≠πˆø°º≠ ∞‘¿”º≠πˆø° æ∆¿Ã≈€ ª˝º∫∞·∞ÅE∫∏≥æ∂ß « ø‰«— ¡§∫∏
class	cRESULT_CREATE_ITEM_INFO
{
public:
	WORD	m_wSerial;
	DWORD	m_dwRegisteredSerial;
};	


//
//	∆«∏≈¥ÅEÅEæ∆¿Ã≈€
class	CSellingAgentItem
{
public:
	CItemDefine		m_item;
	DWORD			m_dwPrice;
	char			m_strOwnerName[dNAME_LENGTH];
};

#endif