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
#define	dABILITY_COUNT						52	//엉雷러티
#define	dMAX_SERVER_COUNT					20
#define	dDROPPING_ITEM_COUNT				10	//	떨엉珞리는 아이템 패턴
#define	dITEM_MONEY							0	//	돈 -o-
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
#define dPARTYNAME_LENGTH					22	//	파티이름 길이
#define dPARTY_MAXPLAYER					8	//	파티의 최�E인원�E
#define dPARTYLIST_COUNT					8	//	한번에 보내는 파티 리스트 �E

#define dUNION_PARTY_COUNT					8	//	파티이름 길이
#define dMAX_UNION_PARTY_COUNT				256	//	파티이름 길이
#define dUNION_PARTYNAME_LENGTH				22	//	파티이름 길이
#define dMAX_PLAYER_TITLE_COUNT				50

#define	dDISSOLUTION_PARTY_AT_MOVE_FIELD	0xfffe

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃플레이�E데이터 정의
class	CJobBasicDataDefine	//	기본 직업에 필요한 데이터
{
public:
	int				m_iLevel;							//	레벨
	int				m_iExperience;						//	경험치
	DWORD			m_dwSkillExperience;				//	스킬 경험치

	int				m_iHP;								//	현�E체력			10.00	10,000.00  
	int				m_iMaxHP;							//	최�E체력			10.00	10,000.00  
	int				m_iCP;								//	현�E  파�E게이�E  1.00	 5,000.00  
	int				m_iMaxCP;							//	맥시멈 파�E게이�E10.00	10,000.00  
	WORD			m_wCorrectMaxHPFactor;				//	최�EHP 계�E커� 필요한 HP 인자
	WORD			m_wCorrectMaxHPConstitutionFactor;	//	최�EHP 계�E커� 필요한 건강 인자

	short			m_sStrength;						//	�E		5 1,000
	short			m_sAgility;							//	민첩성		5 1,000
	short			m_sConstitution;					//	건강		5 1,000
	short			m_sWisdom;							//	지혜		5 1,000
	short			m_sIntelligence;					//	지식		5 1,000
	short			m_sCharisma;						//	카리스마	5 1,000
	short			m_sLuck;							//	�E		5 1,000
	short			m_sSight;							//	시야

	short			m_sMinDamage;						//	최소 데미�E	
	short			m_sMaxDamage;						//	최�E데미�E	
	short			m_sDefensivePower;					//	방엉澹

	short			m_sAllignment;						//	성�E-100.00~100.00(absoulte evil,evil,neutral,good,absoulte good)

	short			m_sFireResistance;					//	불 저항력 0% 100.00%  
	short			m_sWaterResistance;					//	물 저항력 0% 100.00%  
	short			m_sWindResistance;					//	바�E저항력 0% 100.00%  
	short			m_sEarthResistance;					//	�EE저항력 0% 100.00%  
	short			m_sLightResistance;					//	�E저항력 0% 100.00%
	short			m_sDarkResistance;					//	엉喫 저항력 0% 100.00%

	short			m_sBlindResistance;					//	장님 저항
	short			m_sPoisonResistance;				//	중독 저항
	short			m_sSleepResistance;					//	�E저항
	short			m_sColdResistance;					//	콜�E저항
	short			m_sFreezeResistance;				//	프리�E저항
	short			m_sStunResistance;					//	경�E저항
	short			m_sStoneResistance;					//	석화 저항
	short			m_sConfuseResistance;				//	혼�E저항
	short			m_sCharmingResistance;				//	챠밍 저항

	short			m_sBadStatusResistance;				//	상태 이상공격에 �E� 저항 0% 100.00%
	short			m_sDeclinePowerResistance;			//	능력치 저하계에 �E� 저항 0% 100.00%
	short			m_sCurseResistance;					//	저주에 �E� 저항 0% 100.00%
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
	WORD					m_wBaseItem;					//	아이템 인덱스
	BYTE					m_bCount;						//	숫자
	BYTE					m_bDurability;					//	내구력
	BYTE					m_aOption[2];					//	생성될때 붙은 옵션
	cP_ITEM_PREFIX_INFO		m_aPrefix[dITEM_PREFIX_COUNT];	//	추가효�E3개
};

#pragma	pack()

#pragma	pack(2)

class	cP_ACTOR_EQUIPMENT
{
public:
	WORD			m_wItemCount;			//	아이템의 �E	
	cP_ITEM			m_aEquip[dEQUIPMENT_PART_COUNT];
	cP_ITEM			m_aItems[dOWN_ITEM_COUNT];
};

//
//	필드에 떨엉�E아이템
class	cP_DROPPED_ITEM
{
public:
	DWORD			m_bf10Serial	:	10;
	DWORD			m_bf14BaseItem	:	14;
	DWORD			m_bf4Type		:	4;	//	타입
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
	eOS_NORMAL		=	1,	//	�E�
	eOS_HIDE		=	2,	//	잠�E_-
	eOS_ANNOUNCE	=	3,	//	광�E.
};

//
//	Operater Level
enum
{
	eOL_0			=	0,	//	일반 플레이�E	eOL_1			=	1,	//	마스터(�E�-_-)
	eOL_2			=	2,	//	직�E	eOL_3			=	3,	//	알바생
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
		DWORD	year		:	6;	//	연
		DWORD	month		:	4;	//	�E		
		DWORD	day			:	5;	//	일
		DWORD	hour		:	5;	//	시
		DWORD	minute		:	6;	//	분
		DWORD	second		:	6;	//	초
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
	ePS_FREE	,	//	공짜 유�E	
	ePS_LEVEL_1	,	//	레벨 1
	ePS_LEVEL_2	,	//	레벨 2
	ePS_LEVEL_3	,	//	레벨 3
};

class	CPlayerSaveDataForClientDefine :	public CJobBasicDataDefine , public CPlayerEquipmentDataDefine
{
public:
	char				m_strId[dID_LENGTH];//ID
	char				m_strName[dNAME_LENGTH];//이름

	WORD				m_wJob;				//	현�E직�E	
	int					m_iGold;			//	돈
	WORD				m_wLevelPoint;		//	레벨�E포인트

	WORD				m_wCurrentField;	//현�E필�E
	int					m_iXPos,m_iYPos;
	WORD				m_wLastVillage;		//마지막으로 떠나온 마을

	WORD				m_wGuildSerial;
	WORD				m_wGuildRank;
	WORD				m_wBonusSkillPoint;

	WORD				m_bf1IsDenyCommunity							:	1;
	WORD				m_bf1IsAddPetSystemBy20040916					:	1;	//	�E시스템 시작
	WORD				m_bf1IsReadyToMakeGuild							:	1;
	WORD				m_bf1IsAddFailedPremiumItemInfoAndResetWayPoint	:	1;
	WORD				m_bf1IsResetPremiumFieldInItem					:	1;
	WORD				m_bf1IsBeginPremiumService						:	1;

	WORD				m_bf1IsReceiveRiceCakeSoup						:	1;
	WORD				m_bf1IsAddQuestCountDownTimer20055027			:	1;	//	빈거

	WORD				m_bf1IsIncorrectConnectByGuildBattle			:	1;
	WORD				m_bf1IsResetHidePitchmanShop20041102			:	1;	//	빈거
	WORD				m_bf3UseBoostCount								:	3;
	WORD				m_bf1AddResetSkillStateScroll					:	1;
	WORD				m_bf1IsHidePitchmanShop							:	1;
	WORD				m_bf1AddData2005_03_03							:	1;	//	16 모두 사�E�

	CAbilityDefine		m_aAbility[dABILITY_COUNT];	//엉雷러티
	CPlayerTitleInfo	m_aTitle[dMAX_PLAYER_TITLE_COUNT];
	tsProcessQuestField	m_aProcessQuest[dMAX_PROCESS_QUEST_COUNT];
	CPlayerWayPointInfo	m_aWayPoint[dMAX_PLAYER_WAY_POINT_COUNT];
};

class	CPlayerSaveDataForServerDefine	:	public CPlayerSaveDataForClientDefine
{
public:
	DWORD				m_dwPlayTime;					//	플레이 타임 분 단위

	DWORD				m_bf12DeathPeneltySecond		: 12;	//	죽음으로 인한 능력치 페널티 타임
	DWORD				m_bf6ProhibitCommunityMinute	: 6;
	DWORD				m_bf5LastUpdateDay				: 5;
	DWORD				m_bf1WaitForRecallForKelby		: 1;
	DWORD				m_bf1WaitForRecallForSwellfer	: 1;
	DWORD				m_bf1WaitForRecallForWindy		: 1;
	DWORD				m_bf1WaitForRecallForHedger		: 1;
	DWORD				m_bf4RemainTimeForUseBreedingRecordBook	:	4;

	DWORD				m_dwSaveCount;
	DWORD				m_dwPeneltyTime;

	WORD				m_bf1IsPlayOnGuildBattle		:	1;	//	길드�E중이다
	WORD				m_bf5GuildBattleDay				:	5;	//	길드�E입장한 날
	WORD				m_bf1IsTeleportToCouple			:	1;	//	커플링을 사�E� 텔레포트중이다.
	WORD				m_bf7UseCoupleRingSlot			:	7;	//	사�E� 커플링 아이템 슬롯
	WORD				m_bf1CancelQuestWork			:	1;
	WORD				m_bf1CancelQuestWork2			:	1;	//	16

	WORD				m_wAvatarIndex;				//	아바타 인덱스
	WORD				m_wRequitalCounter;			//	프리미�E아이템 보�E카웝訴

	BYTE				m_aCompleteQuest[dQUEST_BUFFER_SIZE];

	WORD				m_bf10ESD_Time			:	10;	//	enter secret dungeon time
	WORD				m_bf6ExtraInventorySize	:	6;	//	16

	UTime				m_timeGuildJoinTime;		//	길�E가입한 시간
	CItemDefine			m_itemFailedPremiumItem;

	cPetDefine			m_aPet[dOWN_PET_COUNT];

	WORD				m_bf8ModifyDataCounter	:	8;
	WORD				m_bf5ActiveMiniPet2		:	5;
	WORD				m_bf3GreateGodGrace		:	3;

	BYTE				m_bRemainSecondForUseGateGlove;
	BYTE				m_bf1IsMoveFieldByDeath		:	1;	//	죽엉幕 마을로 돌아 왔다.
	BYTE				m_bf1IsMoveFieldByTeleport	:	1;	//	죽엉幕 마을로 돌아 왔다.
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
	DWORD				m_bf5ActiveMiniPet			:	5;	//	현�E활성화된 미니�E
	WORD				m_awQuestCountDownTimer[dMAX_PROCESS_QUEST_COUNT];

	WORD				m_wRedStonePoint;
	WORD				m_wBoostExpSecondByTreasureMapEvent,m_wBoostGoldSecondByTreasureMapEvent;

	WORD				m_wProcessChapterIndex;	//	진�E중인 메인 퀘스트 챕터 인덱스. GVG 서�E캐릭터는 이 정보가 1이�E이름에 '_' 가 안붙�E바로 알파벳이 붙은 경�E�.
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
	UINT				m_bf1IsLevelDown	:	1;	//	26	저장할때 레벨 다�E허가

//	2007 11 02 챕터 5 추가를 위해 추가한 데이터�E	
	WORD				m_bf4CH5StateBonusCount	:	4;
	WORD				m_bf4CH5SkillBonusCount	:	4;
	WORD				m_bf4CH5HPBonusCount	:	4;
	WORD				m_bf4CH5CPBonusCount	:	4;

	UINT				m_bf4CH5PeneltyHPBonusCount		:	4;
	UINT				m_bf4CH5PeneltyCPBonusCount		:	4;
	UINT				m_bf5BardTitleCoolTime			:	5;
	UINT				m_bf4LevelOfLatestReceivedCH5EquipLevelBonus	:	4;	//	마지막으로 챕터 5 장�E레벨 보정 부�E받았을 때 칭호 레벨
	UINT				m_bf3GuildDungeonPlayTime		:	3;	//	20
	UINT				:	0;	//	

	WORD				m_awItemCoolTime[dITEM_COOL_TIME_BUFFER_SIZE];
	
	UINT				m_bf4EatFireMiniPetCount	:	4;
	UINT				m_bf4EatWaterMiniPetCount	:	4;
	UINT				m_bf4EatWindMiniPetCount	:	4;
	UINT				m_bf4EatEarthMiniPetCount	:	4;
	UINT				m_bf4EatLightMiniPetCount	:	4;
	UINT				m_bf4EatDarkMiniPetCount	:	4;
	UINT				m_bf4RebirthCount			:	4;	//	�E� 횟�E	
	UINT				m_bf4UseSealSphereOfDawnCount:	4;	//	새벽의 봉인구 사�E횟�E
	BYTE				m_aCompleteArbeit2[dARBEIT_BUFFER_SIZE];

	UINT				m_bf6LastTransAvatarToGVGYear		:	6;	//	마지막으로 �E徘� 연도
	UINT				m_bf4LastTransAvatarToGVGMonth	:	4;
	UINT				m_bf5LastTransAvatarToGVGDay		:	5;

};

class	cPLAYER_SAVE_DATA_BASE_FOR_CLIENT	:	public CJobBasicDataDefine , public cP_ACTOR_EQUIPMENT
{
public:
	char				m_strId[dID_LENGTH];//ID
	char				m_strName[dNAME_LENGTH];//이름
	WORD				m_wAvatarIndex;//아바타 인덱스

	WORD				m_wJob;//현�E직�E
	int					m_iGold;
	WORD				m_wLevelPoint;//레벨�E포인트

	WORD				m_wLastVillage;//마지막으로 떠나온 마을
	WORD				m_wCurrentField;//현�E필�E
	cP_4POINT			m_pos;//위치

	DWORD				m_dwPlayTime;	//	플레이 타임 분 단위
	int					m_iStolenExperience;
	WORD				m_wGuildSerial;
	WORD				m_wGuildRank;
	BYTE				m_bCurrentLevelDeathCount;
	BYTE				m_bDummyValue;
	char				m_strDummy[6];	//	20바이트 (dNAME_LENGTH +2) - 8 m_dwPlayTime,m_wGuildSerial,m_wGuildRank

	CAbilityDefine		m_aAbility[dABILITY_COUNT];	//엉雷러티

	CPlayerTitleInfo	m_aTitle[dMAX_PLAYER_TITLE_COUNT];
	WORD				m_wBonusSkillPoint;
	tsSpecialField		m_sfInfo;
	DWORD				m_dwSaveCount;

	tsProcessQuestField	m_aProcessQuest[dMAX_PROCESS_QUEST_COUNT];

};

class	cPLAYER_SAVE_DATA_FOR_SERVER	:	public cPLAYER_SAVE_DATA_BASE_FOR_CLIENT
{
public:
	DWORD				m_dwEventCheckerCounter;	//	이벤트 체커
	DWORD				m_dwPeneltyTime;
	BYTE				m_aCompleteQuest[dQUEST_BUFFER_SIZE];
};

//
//	액터 데이터(서버에 저장할때 이 데이터를 저장한다.)
class	cP_PLAYER_SAVE_DATA	:	public cPLAYER_SAVE_DATA_FOR_SERVER
{
public:
};

class	cACTOR_FACTOR_TYPE_FOR_SAVE
{
public:
	short			m_sValue;		//	장비한 상태에서의 수치
	short			m_sFixValue;

	short			m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	WORD			m_wTime;		//	유�E시간
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
//	필드를 이동하거나 해도 유지되는 데이터�E
class	CUpkeepPlayerData
{
private:
	WORD					m_wParty;
public:
	WORD					m_wSpecialSkillField;	//	특�E스킬 필�E_-?
	WORD					m_wMachine;
	WORD					m_bf10Costume					:	10;
	WORD					m_bf1IsEvilSpritOfChristmas		:	1;
	WORD					m_bf1IsHide						:	1;
	DWORD					m_bf4Skin						:	4;			//16

	DWORD					m_bf1IsMoveByChangeGuildHallOwner:	1;	//	길드홀 주인이나 길드홀 모양이 바끔拔幕 강제로 마을로 이동 시켰다.
	DWORD					m_bf1IsMoveByEndOfGuildBattle	:	1;	//	길드�E� 끝나서 강제로 마을로 이동 시켰다.
	DWORD					m_bf1IsMoveByNotGuildBattleTime	:	1;	//	길드�E像� 아니라서 마을로 이동 시켰다.
	DWORD					m_bf1IsMoveByOXQuizProcess		:	1;	//	이미 OX 퀴짊瞼 시작하�E있엉幕 마을로 이동 시켰다.
	DWORD					m_bf1IsMoveByNotOXQuizTime		:	1;	//	OX 퀴�E시간이 아니�E마을로 이동 시켰다.
	DWORD					m_bf1IsMoveByEndOfOXQuiz		:	1;	//	OX 퀴짊瞼 종료되서 마을로 이동 시킴
	DWORD					m_bf1IsBannedSecretDungeon		:	1;			//23

	DWORD					m_bf5MagicCarpetShape			:	5;	//	현�E양탄자 모�E	//28
	DWORD					m_bf1IsJoinGuildDungeon			:	1;	//	길드던�E纛洋求쩝�.		//29

	WORD					m_wBoostPower,m_wBoostTime;
	WORD					m_wWordEventTime;
	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃거신의 은총
	cACTOR_GGG				m_GGG;
	WORD					m_wGuildMasterGrace;
	//┃거신의 은총
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	cSummonBeastDefine		m_aSummonBeast[dOWN_SUMMON_BEAST_COUNT];

	CItemDefine				m_jewel;						//	보석
	WORD					m_wJewelTime;

	WORD							m_wTransformationTime;		// 변신 시간
	WORD							m_wTransformationLevel;		// 변신 레벨
	WORD							m_wTransformationSkin;		// 변신 스킨	
	WORD							m_wTransformationSkill;		// 변신 스킬	

	inline	int				getPartySerial()				{return	m_wParty;}	//	파티에 들엉檄다.
	inline	void			setPartySerial(int _iSerial)	{m_wParty	=	_iSerial;}	//	파티에 들엉檄다.
	inline	char*			getUpkeepPlayerDataPointer()	{return	(char*)this;}
	inline	void			reset()
	{
		for (int i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
			m_aSummonBeast[i].reset();

		m_bf1IsMoveByEndOfGuildBattle	=	FALSE;
		m_bf1IsMoveByNotGuildBattleTime	=	FALSE;
		m_bf1IsMoveByOXQuizProcess		=	FALSE;	//	이미 OX 퀴짊瞼 시작하�E있엉幕 마을로 이동 시켰다.
		m_bf1IsMoveByNotOXQuizTime		=	FALSE;	//	OX 퀴�E시간이 아니�E마을로 이동 시켰다.
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

//┃플레이�E데이터 정의
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//
//	클라이언트 좌표 정보
class	CMinimumActorInfo
{
public:
	WORD			bf11SerialInField	:	11;			//	필드안에서 플레이엉洑 시리�E	
	WORD			bf2Team				:	2;
	WORD			bf1IsNpc			:	1;

	WORD			wNpcActorSerial;

	WORD			wX,wY;
};	//	class	CMinimumActorInfo

//
//	클라이언트 좌표 정보
class	cEventNpcInfo
{
public:
	DWORD			bf11Serial	:	11;		//11	필드안에서 플레이엉洑 시리�E	
	DWORD			bf7Type		:	7;		//18
	DWORD	:0;
	WORD			wX,wY;
};


//
//	웝滂자 정보
class	COperatorInfo
{
public:
	WORD			wSerialInField;			//	필드안에서 웝滂자의 시리�E	
	WORD			wOperatorLevel;
	char			strName[dNAME_LENGTH];
};

//
//	클라이언트 좌표 정보
class	CActorInfoForOuterActor
{
public:
	WORD			wSerialInField;			//	필드안에서 플레이엉洑 시리�E	
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

//	클라이언트가 기�E巒� 필요가 있는 최소한의 정보
class	CSimplePlayerInfo
{
public:
	DWORD			bf2Kind				: 2;
	DWORD			bf8RestHp			: 8;
	DWORD			bf16MaxHp			: 16;
	DWORD			bf2Team				: 2;
	DWORD			bf2RideDog			: 2;	//	개를 타�E있다.
	DWORD			bf1IsSyncHeroInfo	: 1;
	DWORD			bf1IsHide			: 1;
//	32비트

	DWORD			bf4Weapon			: 4;
	DWORD			bf8WeaponEffect		: 8;
	DWORD			bf4Shield			: 4;
	DWORD			bf2Armor			: 2;
	DWORD			bf3ArmorPalette		: 3;
	DWORD			bf11SerialInField	: 11;			//	필드안에서 플레이엉洑 시리�E//	32비트

	DWORD			bf14FirstAidHP		: 14;
	DWORD			bf10Job				: 10;
	DWORD			bf5MagicCarpetShape	: 5;
	DWORD			bf3OperatorLevel	: 3;
//	32비트

	DWORD			bf3Direct			: 3;
	DWORD			bf5Scale			: 5;
	DWORD			bf10Level			: 10;
	DWORD			bf4BlockerShape		: 4;
	DWORD			bf1IsEventMob		: 1;
	DWORD			bf1IsCopyActor		: 1;
	DWORD			bf1IsMarionette		: 1;
	DWORD			bf3GuildPetType		: 3;
	DWORD			bf1IsUnionPartyUser	: 1;
//	28비트

	DWORD			bf12GuildSerial		: 12;
	DWORD			bf5MiniPetType		: 5;
	DWORD			bf5MiniPetType2		: 5;
	DWORD			bf4RebirthCount		: 4;
	DWORD			bf1IsSoundOfLedersBellsLeader	:	1;	//	27비트

	WORD			wX,wY;					//	위치
	LONGLONG		dwEnchantedMask;		//	인챈티�E마스크
	uEnchantedImage	enchantedImage;
	WORD			wItemEffect;

	char			strName[dNAME_LENGTH];
};

//
//	클라이언트가 기�E巒� 필요가 있는 최소한의 정보
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
//	32비트

	DWORD			bf11SerialInField	: 11;	//	필드안에서 플레이엉洑 시리�E	
	DWORD			bf5Scale			: 5;
	DWORD			bf3OperatorLevel	: 3;
	DWORD			bf2RideDog			: 2;	//	개를 타�E있다.
	DWORD			bf1IsDeath			: 1;	//	뒈졌냐?
	DWORD			bf10Job				: 10;
//	32비트

	DWORD			bf4MagicCarpetShape	:	4;
	DWORD			bf4BlockerShape		:	4;
	DWORD			bf3GuildPetType		:	3;
	DWORD			bf5MiniPetType		:	5;
	DWORD			bf5MiniPetType2		:	5;
	DWORD			bf1IsUnionPartyUser	:	1;
	DWORD			bf4RebirthCount		:	4;
	DWORD			bf1IsSoundOfLedersBellsLeader	:	1;	//	27비뵈


	WORD			wX,wY;					//	위치
	LONGLONG		dwEnchantedMask;		//	인챈티�E마스크
	uEnchantedImage	enchantedImage;
	WORD			wItemEffect;
};


class	CSimpleMonsterInfo
{
public:
	DWORD			bf2Kind				: 2;		//	플레이엉謳?
	DWORD			bf10FirstAidHP		: 10;
	DWORD			bf10Level			: 10;
	DWORD			bf8Scale			: 8;
	DWORD			bf2Team				: 2;	//	팀
//	32비트

	DWORD			bf3Direct			: 3;	//	서 있는 방�E	
	DWORD			bf1IsRegen			: 1;	//	리젠된거냐?
	DWORD			bf11SerialInField	: 11;	//	필드안에서 시리�E	
	DWORD			bf1IsPet			: 1;	//	�E甄�.
	DWORD			bf1RideTamer		: 1;
	DWORD			bf11Tamer			: 11;
	DWORD			bf4BlockerShape		: 4;
//	32비트

	DWORD			bf8RestHp			: 8;
	DWORD			bf24MaxHp			: 24;
//	32비트

	WORD			bf11Job				: 11;
	WORD			bf1IsCopyActor		: 1;
	WORD			bf1IsMarionette		: 1;
	WORD			bf1IsBoss			: 1;
	WORD			bf1IsHideName		: 1;	//	이름 숨기�E//	15

	WORD			wX,wY;				//	위치
	LONGLONG		dwEnchantedMask;	//	인챈티�E마스크
	uEnchantedImage	enchantedImage;

	char			strName[dNAME_LENGTH];
};

class	CVerySimpleMonsterInfo
{
public:
	DWORD			bf2Kind				: 2;	//	플레이엉謳?
	DWORD			bf3Direct			: 3;	//	서 있는 방�E	
	DWORD			bf11SerialInField	: 11;	//	필드안에서 시리�E	
	DWORD			bf8Scale			: 8;
	DWORD			bf2Team				: 2;	//	팀
	DWORD			bf1IsMove			: 1;	//	이동중이다.
	DWORD			bf1IsPet			: 1;	//	�E	
	DWORD			bf1RideTamer		: 1;
	DWORD			bf1IsDeath			: 1;	//	뒈졌냐?	//	30

	WORD			bf11Job				:	11;
	WORD			bf1IsCopyActor		:	1;
	WORD			bf1IsMarionette		:	1;
	WORD			bf1IsBoss			:	1;
	WORD			bf1IsHideName		:	1;	//	이름 숨기�E//	15

	WORD			wX,wY;						//	위치
	LONGLONG		dwEnchantedMask;		//	인챈티�E마스크
	uEnchantedImage	enchantedImage;
};


class	CSimpleNpcInfo
{
public:
	DWORD			bf2Kind				: 2;	//	플레이엉謳?
	DWORD			bf6ActorKind		: 6;	//	액터 종�E	
	DWORD			bf3Direct			: 3;	//	서 있는 방�E	
	DWORD			bf11SerialInField	: 11;	//	필드안에서 시리�E	
	DWORD			bf2Team				: 2;	//	팀
	DWORD			bf8Scale			: 8;
	
	WORD			bf11Job				:	11;
	WORD			bf1IsTalker			:	1;
	WORD			bf1IsHideName		:	1;	//	이름 숨기�E//	4비트 남음

	WORD			wX,wY;						//	위치

	char			strName[dNAME_LENGTH];
};

class	CVerySimpleNpcInfo
{
public:
	DWORD			bf2Kind				:	2;	//	플레이엉謳?
	DWORD			bf11SerialInField	:	11;	//	필드안에서 시리�E	
	DWORD			bf11Job				:	11;
	DWORD			bf3Direct			:	3;	//	서 있는 방�E	
	DWORD			bf1IsTalker			:	1;	//	28 bit
	DWORD			bf1IsHideName		:	1;	//	이름 숨기�E
	WORD			bf6ActorKind		:   6;	//	액터 종�E	
	WORD			bf2Team				:   2;	//	팀
	WORD			bf8Scale			:   8;  //	
	WORD			wX,wY;						//	위치
};


class cAVATAR_INFO
{
public:
	WORD			m_wAvatarIndex;
	char			m_strName[dNAME_LENGTH];		//	H 이름 - EMPTY - �E拈�� 구분 없구
	WORD			m_wJob;				//	직�E	
	WORD			m_wLevel;			//	레벨
	WORD			m_wWeapon,m_wShield,m_wArmor;
	WORD			m_wLastField;		//	마지막으로 있�E필�E-> 이걸로 아래 주소도 얻엉幕 저장한다.
	char			m_strIP[dIP_SIZE];	//	마지막으로 접속해 있�E게임서�E주소
};

//
//	게임서버가 월드서버에 아이템 생성 신과濤 할때 필요한 정보
class 	cCREATE_ITEM_INFO
{
public:
	WORD	m_wItem,m_wSerial;	// 아이템 기본 인덱스와 필드에서 드�E아이템 시리�E
};

class 	cDROPPED_ITEM_FIRST_INFO
{
public:
};

//
//	월드서버에서 게임서버에 아이템 생성결�E보낼때 필요한 정보
class	cRESULT_CREATE_ITEM_INFO
{
public:
	WORD	m_wSerial;
	DWORD	m_dwRegisteredSerial;
};	


//
//	판매�EE아이템
class	CSellingAgentItem
{
public:
	CItemDefine		m_item;
	DWORD			m_dwPrice;
	char			m_strOwnerName[dNAME_LENGTH];
};

#endif
