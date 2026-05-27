#ifndef _classHERO_H
#define	_classHERO_H

#include "cNEMO.H"
#include "cJOB.H"
#include "ItemSet.H"
#include "CGame.H"
#include "cACTION.H"
#include "CSkill.H"
#include "CPlayerStatus.h"
#include "cPACKET.H"
#include "cQUEST.H"
#include "CAgent.H"
#include "CFieldItem.H"
#include "CItemPrefix.H"
#include "CGamePlay.H"
#include "CQuest.h"
#include "definePacketData.h"
#include "CTitle.h"
#include "CPet.h"
#include "summonBeast.h"
#include "cArea.h"
#include "userCustomDataDefine.h"
#include "guild_skill_define.h"
#include "MiniPet.h"

class	cHero_mini_pet;

#define	dTWINKLE_ITEM_COUNT	4
//
//	Add Freind Result 친구 추가 결과
enum
{
	eAFR_SUCCESS,	//	성공
	eAFR_FAILED_BY_TOO_MANY,	//	너무 많아요.
	eAFR_FAILED_BY_EXIST_SAME_NAME,	//	이미 같은 이름이 존재해요.
	eAFR_FAILED_BY_UNKNWON_REASON,	//	이유를 알 수 없어요. -_-
};

//getNonameCoupleRing()
//	Premium Service Level
enum
{
	ePSL_NORMAL,
	ePSL_LITE_LITE,
	ePSL_LITE_PREMIUM,
	ePSL_LITE_PLATINUM,
	ePSL_LITE,
	ePSL_PREMIUM,
	ePSL_PLATINUM,

//	위랑은 별개의 구분
	ePSL_ADD_PORTAL,
	ePSL_EXCEPT_EXP_PORTAL,
};

#define	dOLD_MAX_FRIEND_COUNT				100	//	등록 가능한 친구의 수
#define	dMAX_FRIEND_GROUP_COUNT				10
#define	dBATTLE_STATUS_TIMER				10	//	일단 전투를 시작하면 10초간 교전 상태가 된다.
#define	dWAIT_FOR_LOGOUT_TIME				10	//	일단 전투를 시작하면 10초간 교전 상태가 된다.

#define	dLOW_CP_LEVEL_FOR_TRANSFORMATIION	2
#define	dSEND_HERO_ACTION_PERIOD			(dSYNC_FPS/2)
#define	dDEFAULT_CP_FLOAT_TIME				(dSYNC_FPS*3/2)
#define	dWAIT_MOVE_AREA_RESULT_TIME			(dSYNC_FPS*2)
#define	dSEND_HERO_PICK_ITEM_PERIOD			(dSYNC_FPS/2)
#define	dSEND_DISARM_TRAP_PERIOD			dSYNC_FPS
#define	dWAIT_ACTION_RESULT_TIME			1000

#define	dMASK_QUICK_SLOT_ITEM				0x8000

#define	dSHOUT_PERIOD						30	//	30초에 한번씩 외칠 수 있다.
#define	dSHOUT_SHOUT_COUNT_BY_ONE_PERIOD	3	//	3번

#define	dOPERATOR_BOTTOM_LEVEL				3
#define	dMASTER_OPERATOR_BOTTOM_LEVEL		4

#define dAWAKENTYPE_NONE		0
#define dAWAKENTYPE_50			1
#define dAWAKENTYPE_100			2

class	CActor;
class	CGuildSkillEffectInfo;

//
//	액터 데이터(서버에 저장할때 이 데이터를 저장한다.)
class	CPlayerSaveData	:	public CJobBasicDataDefine , public CPlayerEquipment
{
public:
	char				m_strId[dID_LENGTH];//ID
	char				m_strName[dNAME_LENGTH];//이름

	WORD				m_wJob;				//	현재 직업
	int					m_iGold;			//	돈
	WORD				m_wLevelPoint;		//	레벨업 포인트

	WORD				m_wCurrentField;	//현재 필드
	int					m_iXPos,m_iYPos;
	WORD				m_wLastVillage;		//마지막으로 떠나온 마을

	WORD				m_wGuildSerial;
	WORD				m_wGuildRank;
	WORD				m_wBonusSkillPoint;

	WORD				m_bf1IsDenyCommunity							:	1;
	WORD				m_bf1IsAddPetSystemBy20040916					:	1;	//	펫 시스템 시작
	WORD				m_bf1IsReadyToMakeGuild							:	1;
	WORD				m_bf1IsAddFailedPremiumItemInfoAndResetWayPoint	:	1;
	WORD				m_bf1IsResetPremiumFieldInItem					:	1;
	WORD				m_bf1IsBeginPremiumService						:	1;
	WORD				m_bf1IsReceiveRiceCakeSoup						:	1;	//	빈거
	WORD				m_bf1IsRestoreTime20040520						:	1;	//	빈거
	WORD				m_bf1IsIncorrectConnectByGuildBattle			:	1;
	WORD				m_bf1IsResetHidePitchmanShop20041102			:	1;	//	빈거
	WORD				m_bf3UseBoostCount								:	3;
	WORD				m_bf1AddResetSkillStateScroll					:	1;
	WORD				m_bf1IsHidePitchmanShop							:	1;
	WORD				m_bfSpareField1									:	1;

	cAbility			m_aAbility[dABILITY_COUNT];	//어빌러티
	CPlayerTitleInfo	m_aTitle[dMAX_PLAYER_TITLE_COUNT];
	tsProcessQuestField	m_aProcessQuest[dMAX_PROCESS_QUEST_COUNT];
	CPlayerWayPointInfo	m_aWayPoint[dMAX_PLAYER_WAY_POINT_COUNT];
};

//
//	SSF - Special Skill Field
#define	dSSF_COMPLETE_PROTECTION	1

//
//	저장 데이터 + 필드 이동후에도 필요한 데이터 + 추가 저장 데이터 
class	CPlayerData	:	public CPlayerSaveData,public CUpkeepPlayerData
{
public:
	cPet				m_aPet[dOWN_PET_COUNT];
	cSummonBeast		m_aSummonBeast[dOWN_SUMMON_BEAST_COUNT];

	DWORD				m_bf10NoMannerPoint				:	10;
	DWORD				m_bf6ExtraInventorySize			:	6;
	DWORD				m_bf1IsReadyToSelectGuildMark	:	1;
	DWORD				m_bf3GreateGodGrace				:	3;	//	거신의 은총 - 믿고 잇는 거신의 종류.

	WORD				m_wRemainSecondForUseGateGlove;
	
	BYTE				m_aCompleteQuest[dQUEST_BUFFER_SIZE];
	BYTE				m_aCompleteArbeit[dARBEIT_BUFFER_SIZE];
	BYTE				m_aCompleteArbeit2[dARBEIT_BUFFER_SIZE];
	cItem				m_aExtraInventory[dEXTRA_INVENTORY_SIZE];

	WORD				m_wServerSign;
	WORD				m_wRedStonePoint;
	WORD				m_wBoostExpSecondByTreasureMapEvent,m_wBoostGoldSecondByTreasureMapEvent;

	WORD				m_wBonusStatePoint;
	WORD				m_awQuestCountDownTimer[dMAX_PROCESS_QUEST_COUNT];

	WORD				m_wProcessChapterIndex;
	WORD				m_wProcessHighQuestIndex;
	WORD				m_awSkillCoolTime[c_iCoolTimeSkillCount];

	int					m_iTodayWithdrawGoldInGuild;
	BYTE				m_abTodayWithdrawItemCount[4];

	BYTE				m_aCompleteQuest2[dQUEST_BUFFER_EXTEND_SIZE];

	WORD				m_wIncreaseFinalDamageTime;
	WORD				m_wIncreaseFinalDamage;
	WORD				m_wActiveMiniPet,m_wActiveMiniPet2;
	WORD				m_wIsActiveReversalFrame;

	WORD				m_wReversalFrameYear,m_wReversalFrameMonth,m_wReversalFrameDay,m_wReversalFrameHour,m_wReversalFrameMinute;	//	25
	WORD				m_wMiniPetCombiSkill;

	BYTE				m_bIncreaseMiniPetFireDamage	;
	BYTE				m_bIncreaseMiniPetWaterDamage	;
	BYTE				m_bIncreaseMiniPetWindDamage	;
	BYTE				m_bIncreaseMiniPetEarthDamage	;
	BYTE				m_bIncreaseMiniPetLightDamage	;
	BYTE				m_bIncreaseMiniPetDarkDamage	;
	BYTE				m_bIncreaseMiniPetMagicDamage	;
	BYTE				m_bIncreaseMiniPetSkillActiveChance	;
	BYTE				m_bIncreaseMiniPetBasicSkillLevel	;
	BYTE				m_bIncreaseMiniPetSkillLevel	;
	BYTE				m_bIncreaseMiniPetMagicDamageVsBoss	;

	BYTE				m_bCorrectMiniPetBasicSkillLevel;
	BYTE				m_bCorrectMiniPetSkillLevel;
	BYTE				m_bCorrectMiniPetSkillActiveChance;
	BYTE				m_bCorrectMiniPetSkillDamage;

	cMiniPet			m_aMiniPet[c_iMiniPetCount];

	WORD				m_bf4CH5StateBonusCount	:	4;
	WORD				m_bf4CH5SkillBonusCount	:	4;
	WORD				m_bf4CH5HPBonusCount	:	4;
	WORD				m_bf4CH5CPBonusCount	:	4;

	UINT				m_bf4CH5PeneltyHPBonusCount		:	4;
	UINT				m_bf4CH5PeneltyCPBonusCount		:	4;
	UINT				m_bf5BardTitleCoolTime			:	5;
	UINT				m_bf4LevelOfLatestReceivedCH5EquipLevelBonus	:	4;	//	마지막으로
	UINT				m_bf3GuildDungeonPlayTime		:	3;	//	20

	WORD				m_awItemCoolTime[dITEM_COOL_TIME_BUFFER_SIZE];

	UINT				m_bf4EatFireMiniPetCount	:	4;
	UINT				m_bf4EatWaterMiniPetCount	:	4;
	UINT				m_bf4EatWindMiniPetCount	:	4;
	UINT				m_bf4EatEarthMiniPetCount	:	4;
	UINT				m_bf4EatLightMiniPetCount	:	4;
	UINT				m_bf4EatDarkMiniPetCount	:	4;
	UINT				m_bf4RebirthCount			:	4;	//	전생 횟수
	UINT				m_bf4UseSealSphereOfDawnCount:	4;	//	새벽의 봉인구 사용 횟수
};

class	cUserCustomDataSaveInPC
{
public:
	WORD				m_wCurrentQuickSkillSlot;
	WORD				m_bf1IsRunning				:	1;
	WORD				m_bf1IsSubAttackAbilityOn	:	1;
	BYTE				m_abIsExpandFriendGroup[10];
	BYTE				m_aBuffer[90];

	void				reset()
	{
		memset(this,0,sizeof(cUserCustomDataSaveInPC));
	}

	cUserCustomDataSaveInPC*	getUserCustomDataSaveInPC()
	{
		return	this;
	}
};

class	cUserCustomData		:	public cUserCustomDataDefine,public cUserCustomDataSaveInPC
{
public:
	cUserCustomData*	getUserCustomData()
	{
		return	this;
	}

	void				reset()
	{
		cUserCustomDataDefine::reset();
		cUserCustomDataSaveInPC::reset();
	}
};

class	CPlayerFieldData	:	public cUserCustomData
{
public:
	int					m_iSerial;	//	액터 슬롯
	int					m_iZoneSerial;				//	존에서의 시리얼
	int					m_iPlayerSerial;			//	플레이어의 시리얼(서버에서 관리하는 시리얼)
	BOOL				m_bIsRequreFullInfo;

	WORD				m_wWeeklyDuelRank,m_wDailyDuelRank,m_wTrialGameRank;
	cDuelRecord			m_duelRecord;

	DWORD				m_dwLastReceiveCommandTime;
	WORD				m_wWantSummonedBeastIndex,m_wNonBattleTime;
	char				m_strGuildName[dGUILD_NAME_LENGTH];
	WORD				m_wRankInGuildForSkill,m_wGuildHallLevel;

	WORD				m_wItemSlotCount;
	WORD				m_wItemCount;				//	아이템의 수
	WORD				m_wTitleDistinguisherValue;
	cRECT				m_rectSight;
	int					m_iPPS;
	WORD				m_wBlocker;//	블록커 HP,블록커 유지시간.
	WORD				m_wSlaveMonster;
	WORD				m_wMagicCarpetShape,m_wBCS_Serial;
	BOOL				m_bIsEmulation;

	WORD				m_bf1IsExistSquirrelCarpet			:	1;
	WORD				m_bf1IsExistTurttleCarpet			:	1;
	WORD				m_bf1IsReadyToAreaWork				:	1;
	WORD				m_bf1IsReadyToEventAreaWork			:	1;
	WORD				m_bf1IsWaitReleaseLeftButtonForAreaWork	:	1;
	WORD				m_bf1IsWaitDoorWorkResult			:	1;
	WORD				m_bf1IsWaitArcaWorkResult			:	1;
	WORD				m_bf1IsWaitTrapWorkResult			:	1;
	WORD				m_bf1IsUseAbleBulletSubstituteArrow	:	1;
	WORD				m_bf1IsWaitToReceiveDuelRecord		:	1;	//	결투 전적 정보 기다리는중
	WORD				m_bf1IsReceivedDuelRecord			:	1;	//	결투 전적 정보 받았다.

	WORD				m_wPitchmanShopSerial,m_wPitchmanShopSize;
	WORD				m_wDeathPenelty;

	DWORD				m_bf1IsWaitGuildInfo			:	1;	//	길드 정보 기다리는중
	DWORD				m_bf1IsHideWeapon1				:	1;	//	첫번째 무기 숨김?
	DWORD				m_bf1IsHideWeapon2				:	1;	//	두번째 무기 숨김?
	DWORD				m_bf1IsNotFocusStatus			:	1;	//	포커스를 받지 않는 상태
	DWORD				m_bf1IsReloadingBeltItem		:	1;	//	리로드 中...
	DWORD				m_bf1IsWaitTeleportResult		:	1;	//	오브 사용 결과 기다리는중
	DWORD				m_bf1IsWaitShoppingCommandResult:	1;	//	쇼핑몰 커맨드 결과 기다리는중
	DWORD				m_bf1IsWaitGuildCommandResult	:	1;	//	길드 커맨드 결과 기다리는중
	DWORD				m_bf1IsWaitSimpleGuildInfo		:	1;	//	간략화한 길드 정보 기다리는중

	DWORD				m_bf1IsOwnedPitchmanShopSignBoard1	: 1;
	DWORD				m_bf1IsOwnedPitchmanShopSignBoard2	: 1;
	DWORD				m_bf1IsOwnedPitchmanShopSignBoard3	: 1;
	DWORD				m_bf1IsOwnedPitchmanShopSignBoard4	: 1;
	DWORD				m_bf1IsOwnedPitchmanShopSignBoard5	: 1;
	DWORD				m_bf1IsOwnedPitchmanShopLoudSpeaker	: 1;
	DWORD				m_bf1IsOwnedPitchmanColorPen		: 1;
	DWORD				m_bf1IsOwnedPitchmanShopBoldPen		: 1;
	DWORD				m_bf1IsOwnedPitchmanShopLongPen		: 1;
	DWORD				m_bf1IsOwnedPitchmanShopTwinkle		: 1;

	DWORD				m_bf1IsReceiveOtherPlayerItemData	:	1;
	DWORD				m_bf1IsReadyToUseSoundOfLeadersBell	:	1;
	DWORD				m_bf1IsDisplayNameWithSoundOfLeadersBell:	1;
	DWORD				m_bf1IsUseAbleSoundOfLeadersBellStatus	:	1;

	char				m_strMessageForSoundOfLeadersBell[128];

	WORD				m_wRestraintTimeByLogout,m_wLogoutWork;
	WORD				m_wRestraintTimeByAttackAction;
	WORD				m_wRestraintTimeByBattle;
	WORD				m_wLimitEquipmentLevelOverPoint,m_wLimitEquipmentLevelOverPointByCH5;
	WORD				m_wPreminumServiceLevel;
	WORD				m_wIsProcessInitData,m_wIsReadyToFireChargedMiniPetSkill,m_wIsReadyToFireChargedMiniPetSkill2;

	char				m_strOtherPlayerId[dID_LENGTH];
	char				m_strOtherPlayerName[dNAME_LENGTH];

	char				m_acIsUseAbleItem[dEQUIPMENT_PART_COUNT];
	BOOL				m_bIsUseTempBufferForCheckUseableItem;

	inline WORD			isWalking()		{return m_isWalking;}
	CPos				m_posClick;
	inline CPos			GetposClick()	{return m_posClick;}

	WORD				m_isWalking;	//	걷는중
	WORD				m_wEngageTarget;	//	교전상대
	DWORD				m_dwLastShoutTime;	//	마지막으로 외친시간.
	WORD				m_wShoutCount;		//	외친 횟수
	DWORD				m_dwLastChatTime;
	WORD				m_wPrincessWeaponOwner;

	CPos				m_posLast;
	int					m_iWaitMoveResultTime;
	int					m_iDirect;	//	방향
	int					m_iRangeOfDefaultAttack;	//	기본 공격의 공격 범위
	BOOL				m_isTransAble;	//	변신 가능하냐?

	WORD				m_wCPFloatTime;

	WORD				m_wBattleInclination;	//	기본 AI
	WORD				m_wTeam;	//	팀

	WORD				m_wTransformationJob,m_wSpentCPForTransformation;

	WORD				m_wWaitMoveAreaResultTime;//	이동하기로 예약된 영역
	WORD				m_wWaitGuildMarkInfoTime;
	WORD				m_wUseItem,m_wTargetItem;

	WORD				m_wOperatorLevel;	//	운영자 레벨
	WORD				m_wDeathPeneltySecond;
	
	WORD				m_wDruggistTitleFactor,m_wAlchemyTitleFactor;
	WORD				m_wSurvivalInstinctsTitleTriggerHP,m_wSurvivalInstinctsTitleCriticalChance;
	WORD				m_wCampingMasterTitleFactor,m_wCourtesyTitleValue;
	WORD				m_wCorrectFieldElementalResistanceByTitle,m_wReduceDeathPeneltyValueByTitle;

	int					m_iBookedDroppingItemSlot,m_iBookedPickItem;

	WORD				m_wCaughtFieldTrap,m_wWorkFieldTrap,m_wWorkDoor;

	WORD				m_wBardTitleValue,m_wBardTitleValueTime;

//	회복률 저하
	WORD				m_wDeclientRecoverHPEfficientTime,m_wDeclientRecoverHPEfficient;

	cMiniPet			*m_lpMiniPet;

	DWORD				m_bf1IsNetCafeUser						:	1;
	DWORD				m_bf1IsHaveSummonMagicCarpetItem		:	1;
};



class	CPlayerOperateStatus	:	public CPlayerData,public cPLAYER_STATUS,public CPlayerFieldData,public CGamePlay
{
public:
};

class	CHero	:	public CPlayerOperateStatus
{
public:
	inline	void			setWantSummonedBeastData(int _iBeastIndex)
	{
		m_wWantSummonedBeastIndex	=	_iBeastIndex;
	}

	inline	int				getRevisionStrength()
	{
		return	getStrength()-getStrength(TRUE);
	}
	inline	int				getRevisionIntelligence()
	{
		return	getIntelligence()-getIntelligence(TRUE);
	}
	inline	int				getRevisionWisdom()
	{
		return	getWisdom()-getWisdom(TRUE);
	}
	inline	int				getRevisionCharisma()
	{
		return	getCharisma()-getCharisma(TRUE);
	}
	inline	int				getRevisionLuck()
	{
		return	getLuck()-getLuck(TRUE);
	}
	inline	int				getRevisionAgility()
	{
		return	getAgility()-getAgility(TRUE);
	}
	inline	int				getRevisionConstitution()
	{
		return	getConstitution()-getConstitution(TRUE);
	}
	int						getAfterRebirthLevel();
	BOOL					isFixedStrength();
	BOOL					isFixedConstitution();
	BOOL					isFixedIntelligence();
	BOOL					isFixedCharisma();
	BOOL					isFixedWisdom();
	BOOL					isFixedAgility();
	BOOL					isFixedLuck();

	int						getGuildDisplayIndex();
	int						getGuildInfOriginArryPlace(char* _strName);

	int						getLevel(BOOL _bIsWantBaseValue=FALSE);
	int						getStrength(BOOL _bIsWantBaseValue=FALSE,BOOL _bIsAskByReversal=FALSE);
	int						getConstitution(BOOL _bIsWantBaseValue=FALSE,BOOL _bIsAskByReversal=FALSE);
	int						getIntelligence(BOOL _bIsWantBaseValue=FALSE,BOOL _bIsAskByReversal=FALSE);
	int						getCharisma(BOOL _bIsWantBaseValue=FALSE,BOOL _bIsAskByReversal=FALSE);
	int						getWisdom(BOOL _bIsWantBaseValue=FALSE,BOOL _bIsAskByReversal=FALSE);
	int						getAgility(BOOL _bIsWantBaseValue=FALSE,BOOL _bIsAskByReversal=FALSE);
	int						getLuck(BOOL _bIsWantBaseValue=FALSE,BOOL _bIsAskByReversal=FALSE);

	int						getMinipetAwakenType(int _iSlot = -1);

	inline	int				getActionSpeed()	{return	m_actionSpeed.getCorrectedValue();}//액션 속도 보정
	inline	int				getMoveSpeed()		{return	max(m_moveSpeed.getCorrectedValue(),-99);}//이동 속도 보정

	inline	int				getDodgeChance()	{return	m_dodgeChance.getCorrectedValue();}//회피 보정
	inline	int				getHitChance()		{return	m_hitChance.getCorrectedValue();}//명중 보정
	inline	void			firstAid(int _iFirstAidHP)	//	응급처치
	{
		m_iFirstAidHP	+=	_iFirstAidHP;

		int	iTime		=	3000000 /  m_iFirstAidHP/10000 *getMaxHP();
		iTime			=	min(iTime,dMAXIMUM_FIRST_AID_TIME);
		m_wFirstAidTime	=	iTime;
	}

//	MAX HP = [{기본 HP + 건강 * (직업 HP 건강 보정치) + 레벨 * (직업 HP 레벨 보정치)} * 아이템,기술에 의한 보정율 총합] + 아이템, 기술에 의한 보정치 총합
	int						getMaxHP();	//	최대 체력

//	MAX CP = [{기본 CP + 카리스마 * (직업 CP 카리스마 보정치) + 레벨 * (직업 CP 레벨 보정치)} * 아이템, 기술에 의한 보정율 총합] + 아이템, 기술에 의한 보정치 총합
	int						getMaxCP();	//	최대 CP
	void					reduceCP();
	inline	void			reduceCP(int _iCP)	//	CP 빼기
	{	
		if(m_wMadnessTime)		// 광란상태.
			return;
		if (m_wMaintainMaxCPTime)	
		{	
			m_iCP	=	getMaxCP();	
			return;
		}

		m_iCP	=	m_iCP-_iCP;
		m_iCP	=	min(m_iCP,getMaxCP());
	}
	
	void					increaseCP(int _iCP);	//	CP 더하기
	inline	void			regenHP()	//	체력 올라감
	{
		if (m_regenHPPerSecond.getCorrectedValue()	==	0)
			return;

		if (m_iHP	>=	getMaxHP())
			return;

		int	iRecoverHP	=	(m_regenHPPerSecond.getCorrectedValue()/dSYNC_FPS+1);

		m_iHP	+=	iRecoverHP;
		m_iHP	=	min(m_iHP,getMaxHP());
	}

//
//	방어력을 구한다.
	int					getDefensivePower(BOOL _bIsWantBaseValue=FALSE);
//
//	보조 마법같은것의 영향으로 방어력이 올라갔냐?
	BOOL				isPlusDefense();
//	보조 마법같은것의 영향으로 방어력이 떨어졌냐?
	BOOL				isMinusDefense();


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CHeroPet.cpp
	void				partingWithPet(int _iIndex,BOOL _bIsMiniPet,BOOL _bIsRequireMessage);
	void				setPetInfo(int _iIndex,cPetDefine *_lpPet,int _iSerial,int _iMinAttackPower,int _iMaxAttackPower,int _iDefensePower);
	void				ridingDog(BOOL _bIsRiding,int _iGrade,int _iX,int _iY);
	void				setSummonBeastInfo(int _iIndex,int _iSerial,int _iLevel,int _iGrade,int _iType);

	void				changeCommand(int _iPetIndex,int _iCommand,int _iX,int _iY,int _iProtectActor);
	void				syncPetInfo(int _iPetIndex,int _iRemainHP,int _iMaxHP,int _iMaxAttackPower,int _iMinAttackPower,int _iDefensePower,int _iStrength,int _iIntelligence,BOOL _bIsSummonBeast);
	void				petWaitCommand(int _iPetIndex);
	void				makeBreedingRecordBook(int _iPetIndex);
	void				changePetName(int _iPetIndex,char *_lpstrName,BOOL _bIsMiniPet,int _iResetItemIndex);
	int					getPetIndex(int _iSerial);
	int					getPetCount();
	BOOL				isPet(int _iSerial);
	cPet*				getPet(int _iIndex);
	CActor*				getPetActor(int _iIndex);

	int					getSummonBeastIndex(int _iSerial);
	int					getSummonBeastCount();
	BOOL				isSummonBeastActor(int _iSerial,int _iType=0xffff);
	cSummonBeast*		getSummonBeast(int _iIndex);
	CActor*				getSummonBeastActor(int _iIndex);
	cSummonBeast*		getKelby();
	cSummonBeast*		getSummonBeastByType(int _iType,int _iLimitGrade=-1);
//┃CHeroPet.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃유저 커스텀 데이터
//┃user_custom_data.cpp
	DWORD				getRandValue();
	void				saveUserData();	//	유저 데이터 저장
	void				saveUserData(cFILE *_lpFile,cUserCustomData *_lpData);
	void				readLastPlayAvatarName(char *_lpstrName);
	void				readUserData(char *_lpstrName=NULL);	//	유저 데이터 읽기
	void				readUserData(cFILE *_lpFile,cUserCustomData *_lpData);

	void				readOldUserData();
	void				readOldUserData(cFILE *_lpFile);

	void				correctCustomData();	//	잘못된 데이터가 있으면 교정 시킨다.
	void				setCustomData(cUserCustomDataDefine *_lpData,int _iFriendCount);	//	유저 커스텀 데이터 설정
	void				setFriendListOnlineStatus(cFriendListOnlineInfoBase *_lpInfo);	//	친구 온라인 상태 설정

	int					getFriendCountByName(char *_lpstrName);
	char*				getFriendName(int _iIndex);
	cFriendInfoDefine*	getFriend(int _iIndex);
	cFriendInfoDefine*	getFriend(char *_lpstrName);

	BOOL				isFriend(char *_lpstrName);
	BOOL				addFriend(char *_lpstrName,WORD _wJob,BOOL _bIsByRecover=FALSE);
	int					getFriendCountByGroup(int _iGroup,int *_lpiFriendCount=NULL,int *_lpiActiveFriendCount=NULL);	//	그룹으로 친구 수 체크
	BOOL				removeFriendByName(char *_lpstrName);
	BOOL				setFriendJob(char *_lpstrName,WORD _wJob);
	void				disconnectFriend(char *_lpstrName);
	void				connectFriend(char *_lpstrName,int _iJob);

	void				sendChangeUserSkillSetting(cUserCustomDataDefine::eUSER_SKILL_SET _iType,int _iValue1,int _iValue2=-1,int _iValue3=-1,int _iValue4=-1);
	void				sendRemoveFriend(char *_lpstrName);
	void				sendChangeFriendGroupName(int _iGroup,char *_lpstrName);
	void				sendChangeFriendGroup(int _iGroup,char *_lpstrName);
//┃user_custom_data.cpp
//┃유저 커스텀 데이터
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃아이템 효과
//┃CHeroItemEffect.cpp

	// 툴팁 관련한 변수.. 반복적으로 툴팁생성을 막기 위해..-,.-; 마지막에 툴팁을 생성한 아이템시리얼과 체크섬 그리고 케릭터 체크섬을 저장하는 변수들.
	DWORD				m_dwGeneatedToolTipSerial;	
	DWORD				m_dwGeneatedToolTipHeroCheckSum;	
	int					m_iGeneatedToolTipItemCheckSum;	
	int					m_iGeneatedToolTipBaseSerial;

	void				releaseItemReversion(int _iUseItem,int _iTargetItem);
	void				receiveOtherPlayerItemData(char *_lpstrId,char *_lpstrName);
	void				resetOtherPlayerItemData();

	BOOL				useItem(int _iInventorySlot,int _iTarget,BOOL _isBeltSlotItem);
	BOOL				useItemToItem(int _iInventorySlot,int _iTargetItem);
	BOOL				useNormalItem(cItem *_lpItem,BOOL _bIsBeltItem);
	BOOL				useJewel(cItem *_lpItem,BOOL _isBeltSlotItem);

	inline	void		resetItem(int _iInventorySlot)	{m_aItems[_iInventorySlot].reset();}	//	아이템 리셋
	void				decreaseItemCount(cItem *_lpItem,BOOL _bIsBeltItem);
	void				operateEquipmentBasicData(int _iEffect,int _iValue1,int _iValue2);
	void				operatePrefixData(int _iEffect,int _iValue1,int _iValue2,int _iValue3,BOOL _bIsUnique=FALSE);
	void				operateEquipmentData(BOOL _bIsRequireCheckBeltItem=TRUE , BOOL _bIsBySkillPointReset = FALSE);
	int					getEquippedSetItemCount(int _iItemSet);
	cItem*				getEquippedSetItem(int _iItemSet,int _iIndex);
	cItem*				getEquippedSetItemByBaseItem(int _iItemSet,int _iBaseItem);
	cItem*				getSetItemInInventory(int _iItemSet,int _iIndex);
	cItem*				getItemByOption(int _iOption);
	int					getItemIndexByOption(int _iOption);

	void				operateItemSetEffect(int _iItemSet);
	void				operateItemSetEffect();
	void				operateSpecialItemInInventory();//	장비창에 있는 특수 아이템들 처리
	BOOL				applyEquipmentAbility(int _iPart,cItem *_lpItem);
	void				rebuildBasicData();
	inline	void		operateEquipmentPrefix(CItemPrefixInfo *_lpPrefixData)//	장비에 붙은 접두사 정보 처리
	{
		operatePrefixData(g_aItemPrefix[_lpPrefixData->m_wPrefix].m_wType,_lpPrefixData->m_aValue[0],_lpPrefixData->m_aValue[1],g_aItemPrefix[_lpPrefixData->m_wPrefix].m_sValue3);
	}
	inline	void		operateEquipmentUniqueData(cUniqueData *_lpUniqueData)//	장비에 붙은 유니크 정보 처리
	{
		operatePrefixData(_lpUniqueData->m_wEffect,_lpUniqueData->m_aValue[0],_lpUniqueData->m_aValue[1],_lpUniqueData->m_aValue[2],TRUE);
	}
	char*				getAfterEquipInfo(char *_lpstrString,cItem *_lpItem);

	int					getOperateItemSetList(int* _lpItemSetList);		// 처리할 세트아이템의 갯수와 리스트를 얻는다.
	void				removeItemSetEffect(int* _lpItemSetList,int* _lpItemSetCount, int _iItemSetListCount);	// 케릭터스텟에 적용된 세트아이템옵션을 지운다.
	void				transToMonster(WORD	_wEffect, WORD _wTime,	WORD _wLevel);
//┃CHeroItemEffect.cpp
//┃아이템 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	BOOL				applySkillExtraEffects(CActor *_lpAttacker,cAbility *_lpAbility, DWORD _dwKeepTime);		//강제 시간 변수 추가  09.10.14 
	BOOL				applySkillExtraEffect(CActor *_lpAttacker,cAbility *_lpAbility,CSkillExtraEffectInfo *_lpEffect, DWORD _dwKeepTime);		//강제 시간 변수 추가  09.10.14 

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 상태 처리
//┃CHeroStatus.cpp
	void				correctValueEffectDuelServer(int* _iValue , CSkill* _lpSkill);
	void				declineRecoverHPEfficient(int _iTime,int _iEfficient);
	void				buildPower(BOOL _bIsRequireCheckBeltItem=TRUE , BOOL _bIsBySkillPointReset = FALSE, BOOL _bIsOperatePassiveSkill = TRUE);
	BOOL				setInterruptingArmor(int _iCurrentLevel,int _iTime);
	void				setReversalPowerTime(int _iTime);
	inline	void		setRestraintTimeByAttackAction()		{m_wRestraintTimeByBattle=dBATTLE_STATUS_TIMER,m_wRestraintTimeByAttackAction=dBATTLE_STATUS_TIMER;}//	전투에 의해 구속된 시간
	inline	void		setRestraintTimeByBattle()				{m_wRestraintTimeByBattle=dBATTLE_STATUS_TIMER;}//	전투에 의해 구속된 시간
	inline	void		setRestraintTimeByLogout(int _iTime)	{m_wRestraintTimeByLogout=_iTime;}//	전투에 의해 구속된 시간

	inline	void		operateRecoverHP();
	void				updateStatus();
	void				healHP(int _iRecoverHP)					{m_iRecoverHP		+=	_iRecoverHP;}
	void				forcedHealHP(int _iRecoverHP)			{m_iForcedRecoverHP	+=	_iRecoverHP;}
	void				setImpulseTime(int _iImpulseTime)		{m_wImpulseTime		=	_iImpulseTime;}

	void				resetTemporaryMagicEffect();
	void				decreaseAidMagicTime();
	void				operateOncePerSecondWork();

	inline	void		addAbsorbFireDamage(int _iValue)		{m_bAbsorbFireDamage	=	min(m_bAbsorbFireDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	불 속성 공격 흡수
	inline	void		addAbsorbWaterDamage(int _iValue)		{m_bAbsorbWaterDamage	=	min(m_bAbsorbWaterDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	물 속성 공격 흡수
	inline	void		addAbsorbWindDamage(int _iValue)		{m_bAbsorbWindDamage	=	min(m_bAbsorbWindDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	바람 속성 공격 흡수
	inline	void		addAbsorbEarthDamage(int _iValue)		{m_bAbsorbEarthDamage	=	min(m_bAbsorbEarthDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	대지 속성 공격 흡수
	inline	void		addAbsorbLightDamage(int _iValue)		{m_bAbsorbLightDamage	=	min(m_bAbsorbLightDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	빛 속성 공격 흡수
	inline	void		addAbsorbDarkDamage(int _iValue)		{m_bAbsorbDarkDamage	=	min(m_bAbsorbDarkDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	어둠 속성 공격 흡수
	inline	void		addAbsorbMagicDamage(int _iValue)		{m_bAbsorbMagicDamage	=	min(m_bAbsorbMagicDamage+_iValue,dMAXIMUM_ABSORB_VALUE);}//	모든 마법 속성 공격 흡수

	inline	void		addFreezeResistance(int _iValue)		{m_bFreezeResistance	=	min(m_bFreezeResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	프리즈 공격에 대한 저항
	inline	void		addColdResistance(int _iValue)			{m_bColdResistance		=	min(m_bColdResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	콜드 공격에 대한 저항
	inline	void		addStunResistance(int _iValue)			{m_bStunResistance		=	min(m_bStunResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	스턴 공격에 대한 저항
	inline	void		addConfuseResistance(int _iValue)		{m_bConfuseResistance	=	min(m_bConfuseResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	혼란 공격에 대한 저항
	inline	void		addCharmingResistance(int _iValue)		{m_bCharmingResistance	=	min(m_bCharmingResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	차밍 공격에 대한 저항
	inline	void		addStoneResistance(int _iValue)			{m_bStoneResistance		=	min(m_bStoneResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	석화 공격에 대한 저항
	inline	void		addInstanceKillResistance(int _iValue)	{m_bInstanceKillResistance	=	min(m_bInstanceKillResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	즉사 공격에 대한 저항
	inline	void		addPoisonResistance(int _iValue)		{m_bPoisonResistance	=	min(m_bPoisonResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	중독 공격에 대한 저항
	inline	void		addSleepResistance(int _iValue)			{m_bSleepResistance		=	min(m_bSleepResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	잠 공격에 대한 저항
	inline	void		addCurseResistance(int _iValue)			{m_curseResistance.m_sValue			=	min(m_curseResistance.m_sValue+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//"저주 상태에 대한 저항"
	inline	void		addDeclinePowerResistance(int _iValue)	{m_declinePowerResistance.m_sValue	=	min(m_declinePowerResistance.m_sValue+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//"저하 상태에 대한 저항"
	inline	void		addStrangeStatusResistance(int _iValue)	{m_strangeStatusResistance.m_sValue	=	min(m_strangeStatusResistance.m_sValue+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//"이상 상태에 대한 저항"
	inline	void		addAllBadStatusResistance(int _iValue)	
	{	
		addCurseResistance(_iValue);
		addDeclinePowerResistance(_iValue);
		addStrangeStatusResistance(_iValue);
	}
	inline	void		addKnockoutResistance(int _iValue)		{m_bKnockoutResistance	=	min(m_bKnockoutResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	녹백/넘어짐 공격에 대한 저항
	inline	void		addCriticalHitResistance(int _iValue)	{m_bCriticalHitResistance	=	min(m_bCriticalHitResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	치명타 공격에 대한 저항
	inline	void		addCrushBlowResistance(int _iValue)		{m_bCrushBlowResistance	=	min(m_bCrushBlowResistance+_iValue,dMAXIMUM_RESISTANCE_VALUE);}//	결정타 공격에 대한 저항

	inline	void		addReturnDamageRate(int _iValue)		{m_returnDamageRate.addValue(_iValue,dMAXIMUM_RETURN_DAMAGE);}//	받은 데미지 일부 돌려 줌
	inline	void		addDamageToCPRate(int _iValue)			{m_bDamageToCPRate		=	min(m_bDamageToCPRate+_iValue,dMAXIMUM_DAMAGE_TO_CP_RATE);}//	받은 데미지 CP 변환

	inline	void		addBlockingChance(int _iValue)				{m_blockingChance.addValue(_iValue,dMAXIMUM_BLOCKING_CHANCE);}//	"방패 블럭률"
	inline	void		addFirstAidPercentage(int _iValue)			{m_bFirstAidPercentage=min(m_bFirstAidPercentage+_iValue,dMAXIMUM_FIRST_AID_RATE);}//	타격 응급 처치
	inline	void		addRegenHPPerSecond(int _iValue)			{m_regenHPPerSecond.m_sValue=min(m_regenHPPerSecond.m_sValue+_iValue,dMAXIMUM_REGEN_HP);}//	체력 리제네레이션
	inline	void		addResurrectionChance(int _iValue)			{m_bResurrectionChance=min(m_bResurrectionChance+_iValue,dMAXIMUM_RESURRECTION_CHANCE);}//	부활 확률

	inline	void		addCorrectCriticalHitChance(int _iValue)	{m_bCorrectCriticalHitChance=min(m_bCorrectCriticalHitChance+_iValue,dMAXIMUM_CRITICAL_HIT_CHANCE);};//	치명타 발동 확률
	inline	void		addCorrectCrushBlowChance(int _iValue)		{m_bCorrectCrushBlowChance=min(m_bCorrectCrushBlowChance+_iValue,dMAXIMUM_CRUSH_BLOW_CHANCE);};//	치명타 발동 확률//	결정타 발동 확률
	inline	void		addCorrectHPRecoverSpeed(int _iValue)		{m_bCorrectHPRecoverSpeed=min(m_bCorrectHPRecoverSpeed+_iValue,dMAXIMUM_RECOVER_SPEED_BY_POTION);}//	물약 회복 속도 조절
	inline	void		addCorrectKnockoutAttackChance(int _iValue)	{m_bCorrectKnockoutAttackChance=min(m_bCorrectKnockoutAttackChance+_iValue,dMAXIMUM_KNOCKOUT_ATTACK_CHANCE);}//	녹아웃
	inline	void		addCorrectTransformationSpeed(int _iValue)	{m_bCorrectTransformationSpeed=min(m_bCorrectTransformationSpeed+_iValue,dMAXIMUM_TRANSFORMATION_SPEED);}//	무기 교환 속도
	inline	void		addCorrectRareItemDropRate(int _iValue)		{m_bCorrectRareItemDropRate=min(m_bCorrectRareItemDropRate+_iValue,dMAXIMUM_RARE_ITEM_DROP_RATE);}//	마법 아이템 확률 증가
	inline	void		addCorrectUniqueItemDropRate(int _iValue)	{m_bCorrectUniqueItemDropRate=min(m_bCorrectUniqueItemDropRate+_iValue,dMAXIMUM_UNIQUE_ITEM_DROP_RATE);}//	유니크 아이템 확률 증가
	inline	void		addCorrectReloadSpeed(int _iValue)			{m_bCorrectReloadSpeed=min(m_bCorrectReloadSpeed+_iValue,dMAXIMUM_CORRECT_RELOAD_SPEED);}//	"벨트에 아이템 채우는 시간 감소"

	inline	void		addLifeStealPecentage(int _iValue)			{m_wLifeStealPecentage=min(m_wLifeStealPecentage+_iValue,dMAXIMUM_LIFE_STEAL_PERCENTAGE);}//	체력 스틸
	inline	void		addGetBonusCPPercentage(int _iValue)		{m_bGetBonusCPPercentage=min(m_bGetBonusCPPercentage+_iValue,dMAXIMUM_BONUS_CP);}//	보너스 CP
	inline	void		addTargetFleeChance(int _iValue)			{m_bTargetFleeChance=min(m_bTargetFleeChance+_iValue,dMAXIMUM_MAKE_FLEE_CHANCE);}//	도망
	inline	void		addInstanceKillChance(int _iChance)			{m_bInstanceKillChance	=min(_iChance+m_bInstanceKillChance,dMAXIMUM_INSTANCE_KILL_CHANCE);}//	"즉사"

	void				addMakeBerserk(int _iChance,int _iTime);
	void				addMakeStone(int _iChance,int _iTime);
	void				addMakeSleep(int _iChance,int _iTime);
	void				addMakeFirstTarget(int _iChance,int _iTime);
	void				addMakeSimpleton(int _iChance,int _iTime);
	void				addMakeArmorBreak(int _iChance,int _iTime);
	void				addMakeWeaponBreak(int _iChance,int _iTime);
	void				addMakeConfuse(int _iChance,int _iTime);
	void				addMakeCharmed(int _iChance,int _iTime);
	void				addMakeStun(int _iChance,int _iTime);
	void				addMakeCold(int _iChance,int _iTime);
	void				addMakeFreeze(int _iChance,int _iTime);
	void				addMakeBlind(int _iChance,int _iTime);
	void				addAddPoisonDamage(int _iDamage,int _iTime);
	void				addAddPoisonDamage(int _iMinDamage,int _iMaxDamage,int _iTime);
	void				addAddFireDamage(int _iDamage);
	void				addAddWaterDamage(int _iDamage);
	void				addAddWindDamage(int _iDamage);
	void				addAddLightDamage(int _iDamage);
	void				addAddDarkDamage(int _iDamage);
	void				addAddFireDamage(int _iMinDamage,int _iMaxDamage);
	void				addAddWaterDamage(int _iMinDamage,int _iMaxDamage);
	void				addAddWindDamage(int _iMinDamage,int _iMaxDamage);
	void				addAddLightDamage(int _iMinDamage,int _iMaxDamage);
	void				addAddDarkDamage(int _iMinDamage,int _iMaxDamage);
	// VV 09.08.07 마법추가데미지 DX
	void				addAddNaturalDamageDX(int _iClass, int _iDamage);
	void				addAddNaturalDamageDX(int _iClass, int _iMinDamage, int _iMaxDamage);
	// AA 09.08.07 마법추가데미지 DX

	void				addAddDamageToUndead(int _iDamage);
	void				addAddDamageToAnimal(int _iDamage);
	void				addAddDamageToHuman(int _iDamage);
	void				addAddDamageToHolyAnimal(int _iDamage);
	void				addAddDamageToDemon(int _iDamage);
	BOOL				setInterruptingWeapon(int _iTime);
//┃CHeroStatus.cpp
//┃각종 상태 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 이상상태
//┃CHeroWrongStatus.cpp
	BOOL					isStrangeStatus();					//	상태 이상 상태
	BOOL					isDeclinePowerStatus();				//	능력치 저하 상태
	BOOL					isCurseStatus();					//	저주 상태
	BOOL					isWrongStatus();					//	뭔가 이상한 상태

	void					cureStrangeStatus(int _iEfficient=100);	//	이상상태 치료
	void					cureDeclinePowerStatus();			//	저하계 치료
	void					cureCurseStatus();					//	저주 치료
	void					cureWrongStatus();					//	상태이상 치료
	void					revive();							//	부활

	BOOL					cure(int _iSkill,int _iHealHP);		//	스킬에 의한 치료
	BOOL					absorbDamage(int _iHealHP);					//	걍 치료
//┃CHeroWrongStatus.cpp
//┃각종 이상상태
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃아이템 관련
//┃CHeroItem.cpp
	void				changeISPISetting(int _iItemSlot,int _iValue);
	void				useBottomlessBox(int _iBottomlessBox,int _iItem1,int _iItem2);

	int					m_aiTwinkleItem[dTWINKLE_ITEM_COUNT];
	int					m_aiTwinkleItemFrame[dTWINKLE_ITEM_COUNT];

	void				addTwinkleItem(int _iSlot);
	int					getTwinkleFrame(int _iSlot);
	void				updateTwinkleItemFrame();

	cItem*				getFirstPotion();
	cItem*				getFirstDrug();
	cItem*				getFirstFlower();
	cItem*				getFirstCandy();

	BOOL				isInterruptingArmor(cItem *_lpItem);

	BOOL				judgeItem(int _iItem,int _iPrice);
	BOOL				mergeItem(int _iItem,int _iPrice);
	BOOL				removeItemPrefix(int _iUseItem,int _iDestItem,int _iPrefixIndex);
	cItem*				getSecretDungeonKey(int _iDungeonSerial);
	cItem*				getNonameCoupleRing();
	void				getBreakdownItemList(int *_lpiItemList,int *_lpiItemCount);
	int					findSpecificEffectItem(int _iEffect,BOOL _bIsIncludeCheckBelt=FALSE);

	cItem*				getInventoryItem(int _iSlot,BOOL _bIsIgnoreTrade=FALSE,BOOL _bIsWantSlot=FALSE);	//	인벤토리 아이템 얻어오기
	BOOL				isUseAbleItem(cItem *_lpItem,BOOL _bIsCheckSecondJob=FALSE,BOOL _bIsOnlyCheckJob=FALSE);	//	착용/사용 가능한 아이템이냐?
	cItem*				getValidInventorySlot();	//	아이템 빈자리 얻기
	void				arrangeInventory(BOOL _bIsEnterField=FALSE);
	void				syncHeroApprearance();	//	외모 싱크.. *-_-*
	int					getWeaponColorizeEffect();	//	무기의 컬러라이즈 이펙트


	WORD				getItemCheckSum();	//	첵섬을 구한다.
	BOOL				addItemToInventory(cItem *_lpItem);	//	인벤토리에 아이템을 추가한다
	char				*getJobName();	//	직업 이름 리턴
	cItem*				getSameItem(cItem *_lpItem);	//	같은 아이템을 찾아서 리턴한다.
	int					getSameInventoryItemCount(cItem* _lpItem);//09.08.11	같은 아이템의 갯수를 리턴한다.
	int					getSameEquipItemCount(cItem* _lpItem);		//09.08.11	같은 아이템의 갯수를 리턴한다.

	cItem*				getStackAbleItem(cItem *_lpItem);	//	겹칠 수 있는 아이템을 찾아서 리턴한다.
	cItem*				getSameKindItem(cItem *_lpItem);	//	같은 부류의 아이템을 찾아서 리턴한다.

	BOOL				reloadBeltSlot();//벨트 슬롯에 아이템을 리로드-o-한다.

	BOOL				equipItem(cItem *_lpItem,int _iPart);//	_iPart에 아이템 착용
	BOOL				equipItem(int _iItemSlot,int _iPart);//	_iPart에 아이템 착용
	BOOL				stripEquipment(int _iPart);	//	장비 벗기
	BOOL				changeItemPlace(int _iSrcItem,int _iDestItem);//	아이템 위치 변경
	BOOL				isEquipAbleItem(cItem *_lpItem,int _iPart);	//	착용할 수 있는 아이템이냐?
	BOOL				isPickAbleItem(cItem *_lpItem);	//	줏을 수 있는 아이템이냐?

	BOOL				askUseItem(int _iItemIndex);								//	아이템 사용 요청
	void				useWorldMapTeleport(int _iItemSlot);	//	월드맵 텔레포트 아이템을 사용했다.
	void				askUseSoundOfLeadersBell(int _iItemSlot);	//	인도자의 종소리 사용 요청(최쵸)
	void				askUseSoundOfLeadersBell();	//	인도자의 종소리 사용 요청(재청)
	void				useSoundOfLeadersBell(BOOL _bIsTest);	//	인도자의 종소리 사용(진짜!!)

	void				askUseReflectingMixer(int _iItemSlot);	//	리플렉팅 믹서 사용 요청
	void				askUseChainOfRestraint(int _iItemSlot);	//	속박의 사슬 사용


	void				useChangeAllignmentValueItem(int _iItemSlot);	//	성향 수치 변경 아이템 사용
	void				useUpgradeTitleItem(int _iItemSlot);
	void				useImproveItemPowerItem(int _iItemSlot);	//	아이템 파워 증가 아이템 사용
	void				useImproveItemPrefixPowerItem(int _iItemSlot);
	void				useReleaseReversionItem(int _iItemSlot);
	void				useCH5ReverseStatePointItem(int _iItemSlot);
	void				useCH5ReverseSkillPointItem(int _iItemSlot);

	BOOL				changeDamagePrefix20031117(cItem *_lpItem,cSTRING *_lpString);
	BOOL				askUseItemToItem(int _iItemIndex,int _iTargetItem);			//	아이템 사용 요청
	BOOL				askUseItemToCharacter(int _iItemIndex,int _iActorSerial);	//	아이템 사용 요청

	void				prepareDroppingItem(int _iItemIndex);
	BOOL				sendDroppingItem(int _iItemSlot,int _iCount);

	BOOL				operateBookedWorkWithItemCount(int _iCount);//	아이템 갯수와 관련 있는 작업 처리
	void				cancelBookedWorkWithItemCount();//	아이템 갯수와 관련 있는 작업 취소

	BOOL				sendEquipItem(int _iItemIndex,int _iPart,BOOL _bIsBeltItem=FALSE);//	_iPart에 아이템 착용
	BOOL				sendUseBeltItem(int _iItemSlot);	//	벨트 아이템 사용
	BOOL				sendBeltItemReload();	//	벨트 아이템 리로드 메시지 전송
	BOOL				isAvailReloadBeltSlot();//	리로드 할 벨트 아이템이 있냐?
	BOOL				isConsumeItem(int _iItemSlot);	//	소비성 아이템이냐?

	cItem*				getItem(int _iIndex);	//	아이템 얻기
	cItem*				getItemByBaseItemIndex(int _iItem,BOOL _bIsIncludeEquipment);	//	아이템 얻기
	int					getMinipetFeedTypeCount(int iFeedType , BOOL _bCheckExtraBag = FALSE);
	int					getEquipItemBySlotIndex(int _iItemSlot);					// 10.01.07   장비창에 있는 아이템이 스킬 등록되었는지 여부
	void				askDestroyItem(int _iItemIndex);		// 아이템 뿌시라

	inline	void		setInventoryItem(int _iItemSlot,cItem *_lpItem)//	아이템 설정?-o-
	{
		m_aItems[_iItemSlot].copy(_lpItem);
	}
	void				decreaseBullet(cAbility *_lpAbility);	//	탄환이 필요한 스킬이었다면 탄환을 줄인다.
	
	inline	cItem		*getEquipment(int _iPart)	//	_iPart장비 리턴
	{
		if	(_iPart == dEQUIP_WEAPON && m_wJob%2 && m_wJob != dJOB_SUMMONER)	
			_iPart	=	dEQUIP_WEAPON_2;
		if	(m_aEquip[_iPart].m_wBaseItem!=	0xffff	)
			return	&m_aEquip[_iPart];

		return	NULL;
	}

	inline	cItem		*getBeltItem(int _iSlot)
	{
		return	&m_aEquip[_iSlot+dEQUIP_BELT_SLOT1];
	}	//	벨트 아이템 구하기

	inline	cItem		*getBullet()	//	탄환!! -o-
	{
		if	(m_wJob	>=	dPLAYER_JOB_COUNT)
			return	NULL;

		if	(!c_aUseBulletJob[m_wJob])
			return	NULL;

		if	(m_aEquip[dEQUIP_SHIELD_BULLET].m_wBaseItem	!=	0xffff)
			return	&m_aEquip[dEQUIP_SHIELD_BULLET];

		return	NULL;
	}

	inline	cItem		*getWeapon(BOOL _bIsCheckVisible=FALSE)	//	무기! -o-
	{
		if	(_bIsCheckVisible && (!c_aUseWeaponJob[m_wJob] || m_wJob	>=	dPLAYER_JOB_COUNT))
			return	NULL;

		int	iPart	=	dEQUIP_WEAPON;

		if	(m_wJob%2	&&	m_wJob	!=	dJOB_SUMMONER)
			iPart	=	dEQUIP_WEAPON_2;

		if	(iPart == dEQUIP_WEAPON	&& m_bf1IsHideWeapon1)	
			return	NULL;
		if	(iPart == dEQUIP_WEAPON_2&& m_bf1IsHideWeapon2)	
			return	NULL;

		if	(m_aEquip[iPart].m_wBaseItem	!=	0xffff)	
			return	&m_aEquip[iPart];

		return	NULL;
	}

	inline	cItem			*getOtherJobWeapon()//	다른 직업의 무기
	{
		if	(m_wJob	>=	dPLAYER_JOB_COUNT)
			return	NULL;

		int	iJob	=	m_wJob;
		int	iPart	=	dEQUIP_WEAPON;

		if (iJob%2)
			iJob--;
		else
			iJob++;
		
		if	(!c_aUseWeaponJob[iJob])
			return	NULL;

		if	(iJob%2	&&	iJob	!=	dJOB_SUMMONER)
			iPart	=	dEQUIP_WEAPON_2;

		if	(iPart == dEQUIP_WEAPON	&& m_bf1IsHideWeapon1)
			return	NULL;
		if	(iPart == dEQUIP_WEAPON_2&& m_bf1IsHideWeapon2)
			return	NULL;

		if	(m_aEquip[iPart].m_wBaseItem	!=	0xffff)	
			return	&m_aEquip[iPart];

		return	NULL;
	}

	inline	cItem			*getShield()	//	방패! -o-
	{
		if	(m_wJob	>=	dPLAYER_JOB_COUNT)
			return	NULL;

		if	(!c_aUseShieldJob[m_wJob])
			return	NULL;

		if	(m_aEquip[dEQUIP_SHIELD_BULLET].m_wBaseItem	!=	0xffff)	
			if	(m_aEquip[dEQUIP_SHIELD_BULLET].getBasicItem()->m_wKind	==	eIK_SHIELD)
				return	&m_aEquip[dEQUIP_SHIELD_BULLET];

		return	NULL;
	}

	inline	cItem			*getArmor()
	{
		return	getEquipment(dEQUIP_ARMOR);
	}
	inline	cItem*			getEmptyItemSlot()	//	비어 있는 아이템 슬롯을 찾아서 리턴한다.
	{
		int	i;

		for (i=0;i<m_wItemSlotCount;i++)
			if	(m_aItems[i].m_wBaseItem	==	0xffff)
				return	&m_aItems[i];

		for (i=0;i<m_bf6ExtraInventorySize;i++)
			if	(m_aExtraInventory[i].m_wBaseItem	==	0xffff)	
				return	&m_aExtraInventory[i];

		return	NULL;
	}

	inline	void			computeItemCount()//아이템의 수 계산.
	{
		int	i;

		m_wItemCount	=	0;

		for (i=0;i<m_wItemSlotCount;i++)
			if	(m_aItems[i].m_wBaseItem	!=	0xffff)
				m_wItemCount++;

		for (i=0;i<m_bf6ExtraInventorySize;i++)
			if	(m_aExtraInventory[i].m_wBaseItem	!=	0xffff)
				m_wItemCount++;
	}

	inline	void			decreaseBeltItemCount(int _iSlot)	//	벨트 아이템 사용 했소!!
	{
		m_aEquip[_iSlot+dEQUIP_BELT_SLOT1].decreaseCount(1,TRUE);
	}

	inline	void			removeItem(int _iSlot,int _iCount)	//	버리기로 예약해둔 아이템 제거		 09.09.07 확장인벤까지 포함 !
	{
		if	(_iSlot	==	dITEM_DRAG_GOLD)	
			m_iGold	-=	_iCount;
		else
		{
			if (_iSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
			{
				m_aEquip[_iSlot-dBORDER_OF_ITEM_AND_EQUIPMENT].decreaseCount(_iCount);
				buildPower();
			}else if	(_iSlot	>=	dOWN_ITEM_COUNT)
				m_aExtraInventory[_iSlot-dOWN_ITEM_COUNT].decreaseCount(_iCount);
			else
				m_aItems[_iSlot].decreaseCount(_iCount);
		}
		arrangeInventory();
	}

	int					getEmptyInventorySlotCount();
	BOOL				removeItemByBaseItemIndex(int _iItem,int _iCount,BOOL _bIsIncludeEquipment=FALSE);
	int					removeQuestItem(int _iQuestKind,int _iQuestIndex);

	void				operateExpiredItem(int _iSlot,int _iItem);
	void				operateExpiredItemList(int _iCount,WORD *_lpItemList);

	int					getUnlockArcaKeySlot(int _iLockLevel);
	int					getUnlockDoorKeySlot(int _iLockLevel);

	cItem*				getUnlockArcaKey(int _iLockLevel);
	cItem*				getUnlockDoorKey(int _iLockLevel);

	int					getJimMorysEnchantServiceSuccessChance(cItem *_lpEnchantItem);
	int					getEnchantItemSuccessChanceByUniqueInfo(cItem *_lpItem);

	void				pickNearestItem();

	BOOL				isInterruptingWeapon(cItem *_lpItem);	// 무기해제
	BOOL				askIncreaseCarvingLevel(cItem* _lpItem, cItem* _lpTargetItem);
	BOOL				askSetCarvingLevel(cItem* _lpItem, cItem* _lpTargetItem);
	BOOL				setItemUseToTarget(cItem* _lpItem , int _iItemSlot);
	int					getBadgeCountInInventory();	//	배지 수 세기
//━━ 서버에 허락-_-메시지를 기다리고 있는 넘들... 허락이 떨어졌따!!
	BOOL				pickFieldItem();	//	현재 락온된-_- 필드 아이템을 줍는다.
//┃CHeroItem.cpp
//┃아이템 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃함정 관련
//┃hero_trap.cpp
	BOOL				tryDisarmTrap();
//┃hero_trap.cpp
//┃함정 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃문 관련
//┃hero_area.cpp
	void				operateEventAreaWork(cAreaEvent *_lpArea);
	void				operateArcaAreaWork(cAreaArcaInfo *_lpArea);
	void				operateTrapAreaWork(cAreaTrapInfo *_lpArea);
	void				operateDoorWork(cAreaDoorInfo *_lpDoor);

	BOOL				isVisibleDoor(cAreaDoorInfo *_lpDoor);
	BOOL				isVisibleDoorTrap(cAreaDoorInfo *_lpDoor);
	BOOL				isVisibleTrap(cAreaTrapInfo *_lpTrap);
//┃hero_area.cpp
//┃문 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 정보
//┃CHeroInfo.cpp
	int					getLevelIncludeBadge();		//JBC	배지류 포함한 레벨 조사. 08-09-08
	int					getExpBonusByA();
	int					getFireResistanceBonusByA();
	int					getWaterResistanceBonusByA();
	int					getWindResistanceBonusByA();
	int					getEarthResistanceBonusByA();
	int					getLightResistanceBonusByA();
	int					getDarkResistanceBonusByA();
	int					getResistanceBonusByA(int _iAttr);
	int					getTradeBonusByA();

	int					getItemDropChanceBonusByA();
	int					getItemDropLevelBonusByA();

	int					getRedDevilFactor();

	int					getResetAbleStatePoint(int _iPower,int *_lpiCurrentPoint);
	int					getResetAbleSkillPoint(int _iAbility,int *_lpiAbleLevel);

	inline	cHero_mini_pet*	iMiniPet()
	{
		return	(cHero_mini_pet*)this;
	}
	inline	int			getDetectTrapLevel()
	{
		return	m_detectSecretObject.getCorrectedValue();
	}
	inline	int			getReleaseTrasformationCp()	{return	-m_wisdom.getCorrectedValue()*100;}
	DWORD				getCheckSum();
	BOOL				isOwnForce(CActor *_lpActor);
	BOOL				isEnemy(CActor *_lpActor);
	void				setIsRequireFullInfo(BOOL _bSet)	{m_bIsRequreFullInfo	=	_bSet;}
	BOOL				isRequireFullInfo()		{return	m_bIsRequreFullInfo;}

	inline	BOOL		isGuildLeader()
	{
		if (!isGuildMember())	return	FALSE;
		if (m_wGuildRank	==	dGUILD_CLASS_MASTER)	return	TRUE;
		if (m_wGuildRank	==	dGUILD_CLASS_SUBMASTER)	return	TRUE;

		return	FALSE;
	}
	inline	BOOL		isGuildMaster()
	{
		if (!isGuildMember())	return	FALSE;
		if (m_wGuildRank	==	dGUILD_CLASS_MASTER)	return	TRUE;

		return	FALSE;
	}
	inline	BOOL		isGuildMember()
	{
		if (m_wGuildSerial	!=	0xffff)	return	TRUE;

		return	FALSE;
	}
	
	inline	BOOL		isNotFocusStatus()		{return m_bf1IsNotFocusStatus;}
	inline	void		setNotFocusStatus()		{m_bf1IsNotFocusStatus=TRUE;}
	inline	void		offNotFocusStatus()		{m_bf1IsNotFocusStatus=FALSE;}

	inline	BOOL		isOperator()			{if (m_wOperatorLevel >= dOPERATOR_BOTTOM_LEVEL) return	TRUE;return FALSE;}
	int					getFireResistance()	;
	int					getWaterResistance();
	int					getWindResistance()	;	
	int					getEarthResistance();	
	int					getLightResistance();	
	int					getDarkResistance()	;	

	int					getStrengtheningFireDamage(int _iCorrectByTitle, BOOL _bIsPureMagicDamageSkill = TRUE);			// 09.09.28
	int					getStrengtheningWaterDamage(int _iCorrectByTitle, BOOL _bIsPureMagicDamageSkill = TRUE);			// 09.09.28
	int					getStrengtheningWindDamage(int _iCorrectByTitle, BOOL _bIsPureMagicDamageSkill = TRUE);			// 09.09.28
	int					getStrengtheningEarthDamage(int _iCorrectByTitle, BOOL _bIsPureMagicDamageSkill = TRUE);			// 09.09.28
	int					getStrengtheningLightDamage(int _iCorrectByTitle, BOOL _bIsPureMagicDamageSkill = TRUE);			// 09.09.28
	int					getStrengtheningDarkDamage(int _iCorrectByTitle, BOOL _bIsPureMagicDamageSkill = TRUE);			// 09.09.28

	int					getWeakningTargetFireResistance();
	int					getWeakningTargetWaterResistance();
	int					getWeakningTargetWindResistance();
	int					getWeakningTargetEarthResistance();
	int					getWeakningTargetLightResistance();
	int					getWeakningTargetDarkResistance();

	int					getCurrentLevelDeathPeneltyValue();
	int					getCurrentLevelDeathPeneltyRecoverPrice();
	float				getCorrectIntelligenceFactorForMagicDamage(CSkill *_lpSkill = NULL);		// 수정.

	inline	BOOL		isAvailAbility(int _iAbility)
	{
		if (_iAbility	>=	dABILITY_COUNT || _iAbility < 0)	return	FALSE;

		return	isAvailAbility(&m_aAbility[_iAbility]);
	}
	inline	BOOL		isAvailAbility(cAbility *_lpAbility)
	{
		if (_lpAbility->m_wSkill	==	0xffff	)
			return	FALSE;
		if (_lpAbility->m_wLevel	<=	0		)
			return	FALSE;

		return	TRUE;
	}
	inline	cAbility*	getAbility(int _iAbility)
	{
		if (_iAbility	>=	dABILITY_COUNT	||	_iAbility < 0	)
			return	NULL;

		return	&m_aAbility[_iAbility];
	}
	BOOL	inline		isFirstJob(){return 1-(m_wJob)%2;}	//	첫번째 직업인겨?
	BOOL	inline		isSecondJob(){return (m_wJob)%2;}		//	두번째 직업인겨?
	inline	int			getAnotherJob()	//	또 다른 직업 -o-
	{
		if	(m_wJob%2)
			return	m_wJob-1;
		
		return	m_wJob+1;
	}

	int					getCorrectSkillLevel(cAbility *_lpAbility);
	cAbility*			getAbilityBySkill(int _iSkill);//_iSkill인 어빌러티를 찾는다.
	int					getAbilityIndexBySkill(int _iSkill);//_iSkill인 어빌러티를 찾는다.
	void				setLevel(int _iLevel);	//	레벨 설정
	DWORD				getExpForLevelUp();//	레벨업에 필요한 경험치
	DWORD				getCurrentLevelExp();//	현재 레벨의 경험치
	DWORD				getRemainExpForLevelUp();//	레벨업 까지 남은 경험치
	int					getRemainExpForSkillLevelUp(int _iAbility);//	스킬 레벨업 까지 남은 경험치
	BOOL				isLevelUpAbleAbility(int _iAbility);//	레벨업 가능한 어빌러티다.
	BOOL				isLevelUpAbleAbility(cAbility *_lpAbility);//	레벨업 가능한 어빌러티다.
	BOOL				isReactionSkill(int _iAbility);//리액션 스킬이냐?
	int					isUseAbleSkill(int _iAbility);//	사용 가능한 스킬이냐?
	void				increaseSkillLevel(int _iAbility,int _iLevel,DWORD _dwRemainSkillPoint);//스킬 레벨이 올랐다.
	int					getAttackPower(cAbility *_lpAbility,BOOL _isMin);	//	공격력을 구한다.
	int					getWeaponDamage(BOOL _isMin);	//	공격력을 구한다.
	inline	int			getItemSlotCount()	{return	m_wItemSlotCount;}

	int					getAttackRange(int _iAbility);//	사정거리를 구한다.
	int					getAttackRange(cAbility *_lpAbility);//	사정거리를 구한다.
	inline	int			getCPLevel()		{return	GetCPLevel(m_iCP);}	//	현재 CP 레벨을 구한다.
	int					isAbleToTransformation();	//	지금 변신 가능하냐?
	BOOL				isAbleToSitdown();	//	지금 앉을 수 있냐?
	BOOL				isAbleToChangeMoveMethod();//이동 방법 변경 가능?
	int					getSkillPointSumByLevel();
	int					getSkillPointSum();
	int					getItemCountInInventory(int _iItem);
	BOOL				checkWaitGetGVGAvatarOrgData();
//┃CHeroInfo.cpp
//┃각종 정보
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃길드 스킬 처리
//┃CHeroGuildSkill
	void				operateGuildSkillData();

	int					operateGuildSkillData(int _iSkill,int _iLevel,BOOL _bIsGetValue=FALSE,int _iEffectLimitCount=dMAX_GUILD_SKILL_EFFECT_COUNT);
	int					operateGSIncreaseHP(int _iSkill,int _iLevel,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseCP(int _iSkill,int _iLevel,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseHitChance(int _iSkill,int _iLevel,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseDodgeChance(int _iSkill,int _iLevel,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseStrength(int _iSkill,int _iLevel,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseAgility(int _iSkill,int _iLevel,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseConstitution(int _iSkill,int _iLevel,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseIntelligence(int _iSkill,int _iLevel,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseWisdom(int _iSkill,int _iLevel,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseCharisma(int _iSkill,int _iLevel,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseLuck(int _iSkill,int _iLevel,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseJobSkillLevel(int _iSkill,int _iLevel,int _iJob,BOOL _bIsGetValue=FALSE);

	int					operateGSAbsorbMagicDamagePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseMagicResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseCrushResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseCurseResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseStrangeStatusResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseDeclinePowerResitancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreasePhysicalMagicDamagePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseDefensivePowerPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseActionSpeedPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseMoveSpeedPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseHitChancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue=FALSE);
	int					operateGSIncreaseDodgeChancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect,BOOL _bIsGetValue=FALSE);

//┃CHeroGuildSkill
//┃길드 스킬 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃인터페이스 관련
//	int					m_iState;					//	입력 상태
//	BOOL				m_isLockedMouseButton;		//	마우스 버튼이 잠겼-_-?다

//	inline	BOOL		isLockedMouseButton()	{return	m_isLockedMouseButton;}
//	inline	void		lockMouseButton()		{m_isLockedMouseButton	=	TRUE;}
//	inline	void		unlockMouseButton()		{m_isLockedMouseButton	=	FALSE;}
//┃인터페이스 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//	공격 예약이된 어빌러티와 타겟 혹은 위치
	BOOL				m_bIsLockedByLeft;
	WORD				m_wLockedAbility;
	WORD				m_wLockedTarget;			//	락온된 타겟
	WORD				m_wActionToLockedTargetCount;
	CPos				m_posDest;					//	지금 설정되어 있는 타겟 위치

	int					m_iLastActionTime;			//	마지막으로 행동한 타임
	int					m_iLastMovedTime;			//	마지막으로 이동한 타임
	DWORD				m_dwLastReportTime;
	int					m_iLastTransformationTime;	//	마지막으로 변신한 시간

	WORD				m_wLeftAbility,m_wSubAbility,m_wRightAbility,m_wSupportAbility;
	DWORD				m_dwQuickSlotDelayTime;			//  퀵슬롯 딜레이..	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃서버에 요구해서 받을 데이터
//┃서버에 요구해서 받을 데이터
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

						CHero();

	void				init();
	void				reset(BOOL _bIsFirst=TRUE);
	void				transformation();	//	변신
	void				encodeClientData(char *_lpstrId,BYTE *_lpData,int _iDataSize);
	void				readyTransformation(int _iJob,int _iSpentCP){m_wTransformationJob=_iJob,m_wSpentCPForTransformation=_iSpentCP;}//변신준비

	BOOL				walking(BOOL _bIsIgnoreKey);
	void				checkStop();
	BOOL				moveTo(int _iX,int _iY);	//	_iX,_iY로 이동

	void				rebirth(BOOL _bIsNotResetLevel);
	void				resetStatePoint();
	void				resetOneStatePoint(int _iState);
	BOOL				checkResetAbleState(int _iState);
	
	void				resetSkillPoint();
	void				resetAllForDuel();
	void				resetOneState(int _iItemSlot,int _iPower,int _iValue,int _iLevelPoint);
	void				resetOneSkill(int _iItemSlot,int _iAbility,int _iLevel,int _iSkillExperience);

	BOOL				checkLastActionTime(int _iCheckTime);

	BOOL				moveToArea();//출구로 이동
	void				sendReportCurrentPos();


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃서버에 메시지 전송
	void				sendStop(BOOL _bIsCheckChangePosPacketSendCount=TRUE);					//	정지 시키고 정지 패킷을 보낸다.
	BOOL				sendMoveDestPos();			//	목표 위치로 이동한다고 메시지 전송
	void				sendTransformation();		//	변신하고파!!
	void				sendSitdown();				//	앉기/서기 토글
	void				changeMoveAbility();
//┃서버에 메시지 전송
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CHeroTitle.cpp
//┃타이틀 처리
	CPlayerTitleInfo*	getTitleByIndex(int _iTitle);
	CPlayerTitleInfo*	getTitleByTitleEffect(int _iTitleEffect);
	int					getTitleLevel(int _iTitle);

	void				operateTitleData();
	void				operateTitleData(CPlayerTitleInfo *_lpTitleInfo);

	void				operateTitleDistinguisher(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleSkillMaster(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitlePorter(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleCityWalker(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleCampingMaster(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleSurvivalInstincts(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleDealer(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleBankClient(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleCourtesy(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleDruggist(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleEsoteric(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleAlchemy(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleElementalMaster(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleAdventurer(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleInclinePower(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleInclineMagicResistance(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void				operateTitleNaturalEnemy(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);

	BOOL				getDiscermentMonsterInfo(int *_lpiLimitLevel,int *_lpiEnableType,int *_lpiLimitRank);
	void				discermentMonster(CActor *_lpActor,int *_lpiLevel,int *_lpiType,int *_lpiGrade);
//┃타이틀 처리
//┃CHeroTitle.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CHeroInterface.cpp
	// 10.01.11 추가
	int					m_iEquipItemSlot, m_iStripItemSlot, m_iStripIndex;
	// 10.01.14	
	BOOL				m_bUseSkillSlotItem;
	inline	BOOL			isActiveSurvivalInstinctTitle()
	{
		if (m_wSurvivalInstinctsTitleTriggerHP	==	0)
			return	FALSE;

		if (m_iHP	<=	m_wSurvivalInstinctsTitleTriggerHP)
			return	TRUE;

		return	FALSE;
	}

	int					findLeftAbility();
	void				setLeftAbility(int _iAbility);
	void				setRightAbility(int _iAbility);
	void				setSupportAbility(int _iAbility);
	void				setSubAbility(int _iAbility);
	void				setQuickSlotAbility(int _iAbility,int _iSlot);
	void				initQuickSlotItem();							// 10.01.07 스킬슬롯에 아이템 초기화
	void				setQuickSlotItem(int _iItemSlot,int _iSlot);				// 10.01.07 스킬슬롯에 아이템 설정
	void				deleteQuickSlotItem(int _iSlot);					// 10.01.07 스킬슬롯에 아이템 삭제
	BOOL				setEquipItem(int _iItem, int _iSlot = 0xffff, BOOL _bSlotCheck = FALSE);// 장비장착시 스킬슬롯 검색 및 설정				수정
	void				stripEquipItem(int _iItem);						// 10.01.07 장비해제시 스킬슬롯 검색 및 설정

	// 10.01.11 추가  
	void				changeQuickSlotEquipItem(int _iEquip, int _iStrip);
	void				changeQuickSlotStripItem();


	inline	cAbility	*getLeftAbility()			{return getAbility(m_wLeftAbility);}
	inline	cAbility	*getSubAbility()			{return getAbility(m_wSubAbility);}
	inline	cAbility	*getRightAbility()			{return getAbility(m_wRightAbility);}
	inline	cAbility	*getSupportAbility()		{return getAbility(m_wSupportAbility);}
	inline	cAbility	*getLockedAbility()			{return getAbility(m_wLockedAbility);}
	inline	cAbility	*getQuickSkill(int _iSlot)	{return getAbility(m_aQuickSkill[m_wCurrentQuickSkillSlot][_iSlot][(m_wJob)%2]);}
	void				correctQuickSlotData();	//	스킬 슬롯 데이터를 확인 보정한다.
	void				rotateQuickSlot();	//	퀵스킬 슬롯 돌리기
	int					getQuickSlotSkill(int _iSlot);
	void				setQuickSlotSkill(int _iSlot,int _iSkill);
//┃CHeroInterface.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CHeroBattle.cpp
	int					getRangeOfDefaultAttack();	//	기본 공격의 공격 범위를 구한다.

	void				positiveAttack(int _iTarget,int _iAbility);//사정 거리가 안되면 접근해서 공격한다.
	void				attackEngagedTarget();

	void				lockOn(int _iTarget,int _iAbility,BOOL _bIsLockedByLeft=FALSE);// 락 온
	inline	void		lockOff()	{m_wLockedTarget=0xffff;m_wLockedAbility	=	0xffff;}
	inline	BOOL		isLockOn()	{if (m_wLockedTarget	!=	0xffff)	return TRUE;return FALSE;}	//	락 걸은 타겟이 있냐?
	inline	void		stopBattle()	//	전투 중지
	{
		if (m_wEngageTarget	==	0xffff)
			return;

		m_wEngageTarget	=	0xffff;

		s_agent.sendStopBattle();
	}

	inline	void		releaseEngagedTarget()	{m_wEngageTarget	=	0xffff;}
	inline	void		engageTarget(int _iTarget)	{m_wEngageTarget	=	_iTarget;}
	inline	BOOL		isEngage()	{if	(m_wEngageTarget	!=	0xffff	)	return	TRUE;return	FALSE;}
	BOOL				isEngageTarget(int _iSerial);
	BOOL				isPetEngageTarget(int _iSerial);
//┃CHeroBattle.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CHeroSkill.cpp
	BOOL				isVisibleObject(int _iSecretLevel);
	BOOL				isVisibleTrap(int _iTrapLevel);
	void				setReactionStatus(int _iAbility,BOOL _isOn);//	리액션 켜기
	void				setReactionAllOff();	/// 모든 리액션 끄기..   추가.
	BOOL				isActiveReaction(int _iAbility);//	켜진 리액션이냐?
	void				releaseAllPassiveSkill();	//	
	void				sendTurnOnPassiveSkill();	//	패시브 스킬들 켜기
	void				turnOnPassiveSkill(BYTE *_lpList);	//	패시브 스킬들 켜기
	cAbility*			getIcyStalagmiteSkill();
	cAbility*			getRidingDogSkill();
	int					getMasterSkillDifficultySum(int _iJob);

	BOOL				getCompleteProtectionAbility(cAbility *_lpAbility);
	int					getDefensePowerByPassiveSkill();
	int					getDefensePercentageByPassiveSkill();
	cAbility*			getSpecialFeatureSkill(int _iFeature);
	cAbility*			getDisarmTrapAbility();
	cAbility*			getUnlockDoorSkill();
	cAbility*			getUnlockArcaSkill();

	inline	BOOL		isActiveSpecialSkill(int _iField)	//	컴플리트 프로텍션이 발동중이냐?
	{
		if (m_wSpecialSkillField & _iField)	return	TRUE;

		return	FALSE;
	}
	inline	BOOL		isActiveCompleteProtection(){return	isActiveSpecialSkill(dSSF_COMPLETE_PROTECTION);}

	void				setLeftSkill();
	cAbility*			getDefaultSkill();	//	언제나 쓸 수 있는 디폴트 스킬
	int					getCoolTime(cAbility *_lpAbility);
	int					getValuePassiveSkill(int _iKind, int _iValueIndex = 1);	// 패시브스킬의 값을 얻는다.
	void				operatePassiveSkill();		// 패시브스킬들저장..

//┃CHeroSkill.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CHeroUseSkill.cpp
	BOOL				isUseSkillState(CSkill* _lpSkill); // 사용 가능한 상태인가..
	BOOL				useQuickSlotSkill(int _iSlot);	//	퀵 스킬 사용
	BOOL				useSkillToTarget(int _iTarget,int _iAbility);	//	공격
	BOOL				useSkillToGround(int _iX,int _iY,int _iAbility);	//	공격
	void				useSkillToLockedTarget();	//	락 걸린 타겟을 공격
	void				operateUseSkillFailedMessage(int _iReason);
	void				actionToLockedTarget(int _iActionToLockedTargetCount);
//┃CHeroUseSkill.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CHeroQuest.cpp
	void					setChapter(int _iChapter,int _iHighQuest);
	int						getProcessQeustCount();	//	진행중인 퀘스트의 수
	tsProcessQuestField*	getQuestInfo(int _iSlotIndex);	
	CQuestItem*				getQuest(int _iSlotIndex);
	BOOL					cancelQuest(int _iSlotIndex);
	CQuestItem*				getProprietyQuest(int *_lpiLastProprietyQuestIndex,int _iQuestType,char *_lpstrNpcName,int _iNpcActorSerial);
	CQuestItem*				getProprietyQuest(char *_lpstrNpcName,int _iNpcActorSerial);
	BOOL					isProcessQeustClient(char *_lpstrNpcName,int _iNpcActorSerial);
	BOOL					isNpcAboutProcessQuest(char *_lpstrNpcName,int _iNpcActorSerial);
	BOOL					isMonsterAboutProcessQuest(char *_lpstrNpcName,int _iNpcActorSerial);

	BOOL					isCompleteQuest(int _iQuestType,int _iQuestIndex);
	BOOL					isProcessQuest(int _iQuestType, int _iQuestIndex);
	BOOL					SaveCompleteQuest(int _iQuestType,int _iQuestIndex);
//┃CHeroQuest.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

};

extern	CHero		g_hero;
extern	CHero		g_controlMonster;

enum
{
	eMOUSE_STATE_CHANGE_ABILITY				,
	eMOUSE_STATE_MOVE_TO_GATE				,
	eMOUSE_STATE_ACTION						,
	eMOUSE_STATE_MOVE						,
	eMOUSE_STATE_MOVE_TO_PORTAL				,
	eMOUSE_STATE_PICK_FIELD_ITEM			,
	eMOUSE_STATE_SYSTEM_DIALOG				,
	eMOUSE_STATE_POPUP_MENU					,
	eMOUSE_STATE_BOTTOM_INTERFACE			,
	eMOUSE_STATE_INTERFACE					,
	eMOUSE_STATE_EXCLUSIVE_BY_INVENTORY_WINDOW,
	eMOUSE_STATE_EXCLUSIVE_BY_TRADE_BOX		,
	eMOUSE_STATE_ITEM_DRAG					,
	eMOUSE_STATE_TRADE_ITEM_DRAG			,
	eMOUSE_STATE_PITCHMAN_SHOP_ITEM_DRAG	,
	eMOUSE_STATE_BANK_ITEM_DRAG				,
	eMOUSE_STATE_GUILD_INVENTORY_ITEM_DRAG				,
	eMOUSE_STATE_GET_ITEM_COUNT				,
	eMOUSE_STATE_SKILL_DRAG					,
	eMOUSE_STATE_SCROLL_BAR					,
	eMOUSE_STATE_SHOP_SCROLL_BAR			,
	eMOUSE_STATE_CHATTING_BOX_SCROLL_BAR	,
	eMOUSE_STATE_SYSTEM_MENU				,
	eMOUSE_STATE_MINIMAP					,
	eMOUSE_STATE_PROPOSE_TO_WORK			,
	eMOUSE_STATE_CHATTING_BUTTONS			,
	eMOUSE_STATE_FRIEND_WINDOW_BUTTONS		,
	eMOUSE_STATE_GUILD_WINDOW_BUTTONS		,
	eMOUSE_STATE_BANK_WINDOW_BUTTONS		,
	eMOUSE_STATE_GUILD_INVENTORY_WINDOW_BUTTONS		,
	eMOUSE_STATE_CART_ITEM_DRAG				,
	eMOUSE_STATE_CART_WINDOW_BUTTONS		,
	eMOUSE_STATE_PLUS_BUTTONS				,
	eMOUSE_STATE_GUILD_BATTLE_SCHEDULER		,
	eMOUSE_STATE_GUILD_MARK_COMPOSER		,
	eMOUSE_STATE_PITCHMAN_SHOP_BUTTON		,
	eMOUSE_STATE_DISARM_TRAP				,
	eMOUSE_STATE_UPGRADE_SHOP_DRAG			,	// 09.09.01
	eMOUSE_STATE_OPTION						,	// insu add
};


extern	DWORD	g_aExpTable[1000];

#endif