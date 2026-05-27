/*************************************************************************************************
	cACTOR.H

	Red Stone 캐릭터 관리 클래스

	작성  날짜 : 2000.12.24(... 그래!! 크리스마스 이브다!! 크리스마시 이브에 일하고 있다!! 떫냐? 크흑...
	작  성  자 : 이영찬
	개      요 : 모든 캐릭터,몬스터,NPC의 기본이 되는 클래스다. 당근 무쟈게 중요하다.

*************************************************************************************************/

#ifndef _classACTOR_H
#define	_classACTOR_H

#include "SFC.h"
#include "cJOB.H"
#include "cItem.H"
#include "CActiveSkill.H"
#include "CActorStatus.H"
#include "cPROTRACTOR.H"
#include "cParty.H"
#include "CQuest.H"
#include "CPitchmanShop.H"

#include "Client.h"
#include "cPACKET_GAMESERVER.h"
#include "cArea.h"

#include "cNPC.h"
#include "CTitle.h"
#include "summonBeast.h"
#include "pattern.h"
#include "miniPet.h"
#include <math.H>

class	cActor_miniPet;

#define	dMAX_OWN_GOLD			2000000000
#define	dMAX_OWN_BANK_GOLD		4000000000
#define	dPLAYER_ENCRYPT_CODE_UPDATE_PERIOD	(60*30)	//	30분에 한번 업데이트

#define	dMAX_AI_TARGET_COUNT	20
#define	dCHECK_QUEST_EXP_BUFFER_SIZE	5

#define	dDEFAULT_DIRECT_COUNT	8
//
//	Actor Basic Power
enum
{
	eABP_LEVEL				,
	eABP_STRENGTH			,
	eABP_DEXTERITY			,
	eABP_CONSTITUTION		,
	eABP_WISDOM				,
	eABP_INTELLIGENCE		,
	eABP_CHARISMA			,
	eABP_LUCK				,
	eABP_ATTACK				,
	eABP_DEFENSE			,
	eABP_FIRE_RESIST		,
	eABP_WATER_RESIST		,
	eABP_WIND_RESIST		,
	eABP_EARTH_RESIST		,
	eABP_LIGHT_RESIST		,
	eABP_DARK_RESIST		,
	eABP_MAGIC_RESIST		,
};

#ifdef	_IS_TOOL
#define	dREVISE_ACTION_SPEED_FOR_TEST			4
#else
#define	dREVISE_ACTION_SPEED_FOR_TEST			1
#endif

#define	dPLAYER_JUMP_FLY_TIME					dSYNC_FPS
#define	dWAIT_LOGOUT_RESULT_TIME				(dSYNC_FPS*60)
#define	dWAIT_MOVE_FIELD_RESULT_TIME			(dSYNC_FPS*60)
#define	dLIMIT_ACTOR_MOVE_SPEED					800
#define	dWAIT_FOR_BOOKED_REMOVE_PLAYER_TIME		(dSYNC_FPS*2)

#define	dLIMIT_WAIT_CLIENT_TIME					(60*2)		//	2분간 안오면 처리 -o-
#define	dPENELTY_TIME_FOR_SPEEDHACKUSER			(60*10)		//	10분
#define	dPENELTY_TIME_FOR_PACKETHACKINGUSER		(60*30)		//	30분
#define	dASK_CART_INFO_PERIOD					(1000*30)	//	30초

#define	dMONSTER_MISSING_TARGET_RANGE			1000000		//	몬스터가 타겟을 잃어 버리는 거리
#define	dMONSTER_MISSING_TAUNT_TARGET_RANGE		(250*250)	//	몬스터가 타겟을 잃어 버리는 거리
#define	dLOW_CP_LEVEL_FOR_TRANSFORMATIION		2

#define	dDEFAULT_WANDER_RANGE					200
#define	dDEFAULT_CP_FLOAT_TIME					(dSYNC_FPS*3/2)

#define	dBLOCKING_FACTOR						10
#define	dSHOUT_PERIOD							30
#define	dSHOUT_COUNT_BY_ONE_PERIOD				3
#define	dRETURN_DAMAGE_RANGE					150

#define	dENGAGE_TARGET_COUNT					4

#define	dSTONE_DAMAGE_FACTOR					3

#define	dABSOLUTE_EVIL_EXP_FACTOR				10
#define	dEVIL_EXP_FACTOR						5
#define	dACTOR_DATA_CHECK_PERIOD				(60*dSYNC_FPS)
#define	dCRUSH_BLOW_FACTOR						3

#define	dRECEIVE_PLAYER_MOVE_PACKET_PERIOD		dSYNC_FPS

#define	dWAIT_BANK_DATE_PERIOD					60	//	1분

#define	dMAX_TAME_MONSTER_COUNT					2
#define	dMAX_SUMMON_BEAST_COUNT					2

//GGG 이펙트 타입
#define dGGG_EFFECT_PLAYTYPE_NONE				0x0000
#define dGGG_EFFECT_PLAYTYPE_POINT_UP			0x0001
#define dGGG_EFFECT_PLAYTYPE_HIT				0x0002
#define dGGG_EFFECT_PLAYTYPE_HIT_BY				0x0004

//미니펫 각성 관련
#define dAWAKENTYPE_NONE		0
#define dAWAKENTYPE_50			1
#define dAWAKENTYPE_100			2

enum
{
	ePUSS_NONE,
	ePUSS_READY,
	ePUSS_FIRE,
};
enum
{
	eTM_TAME,
	eTM_BRAINWASHING,
};

//
//	Result of Update Move
enum
{
	eRUM_MOVE,
	eRUM_ARRIVE,
	eRUM_BLOCKED,
};
//
//	Check Last Save Data Result
enum
{
	eCLSDR_NOTHING,
	eCLSDR_UPDATE,
	eCLSDR_INCORRECT_FIELD,
};

enum
{
	eGET_GOLD_BY_TREASURE_BOX,
	eGET_GOLD_BY_ADD_ITEM,
	eGET_GOLD_BY_STEAL_GOLD,
	eGET_GOLD_BY_SELL_ITEM,
	eGET_GOLD_BY_PICK_DROPPED_GOLD,
	eGET_GOLD_BY_KARMA,
	eGET_GOLD_BY_BEAT_MONSTER_EVENT,
	eGET_GOLD_BY_LOTTOBOX,
	eGET_GOLD_BY_COMMAND,

	eGET_GOLD_IGNORE,	//	금괴 구매의 경우 금액이 +-가 되므로 굳이 기록할 필요 없다.
	eGET_GOLD_TYPE_COUNT
};

enum
{
	eSPENT_GOLD_BY_LEARN_GUILD_SKILL,
	eSPENT_GOLD_BY_ADD_GUILD_RELATED_PLACE,
	eSPENT_GOLD_BY_REMOVE_ITEM,
	eSPENT_GOLD_BY_JUDGE_ITEM,
	eSPENT_GOLD_BY_MERGE_ITEM,
	eSPENT_GOLD_BY_BUY_PIGEON_POST_SHOP_ITEM,
	eSPENT_GOLD_BY_UPGRADE_ETERNAL_ITEM,
	eSPENT_GOLD_BY_UPGRADE_CRAVING_LEVEL,
	eSPENT_GOLD_BY_STEAL_SKILL,
	eSPENT_GOLD_BY_RECOVER_DEATH_PENELTY,
	eSPENT_GOLD_BY_BANK_TRANSACTUION_CHARGE,
	eSPENT_GOLD_BY_MAKE_GOLDBAR,
	eSPENT_GOLD_BY_MAKE_GOLDBAR_CHARGE,
	eSPENT_GOLD_BY_REPAIR_ITEM,
	eSPENT_GOLD_BY_KARMA,
	eSPENT_GOLD_BY_BUY_TOKENSHOP_ITEM,
	eSPENT_GOLD_BY_REQUEST_GUILD_BATTLE,
	eSPENT_GOLDBAR_BY_KARMA,
	eSPENT_GOLD_BY_BUY_ITEM,
	eSPENT_GOLD_BY_COMMAND,
	
	eSPENT_GOLD_IGNORE,
	eSPENT_GOLD_TYPE_COUNT,
};
//
//	Check Actor Data Result
enum
{
	eCADR_ISOK,
	eCADR_IS_NPC,
	eCADR_IS_NPC2,
	eCADR_VALID_SERIAL,
	eCADR_INCORRECT_JOB,
	eCADR_INCORRECT_ITEM_SLOT_COUNT,
	eCADR_INCORRECT_LEVEL,
	eCADR_INCORRECT_SKILL_DATA,
	eCADR_MISMATCH_SKILL_AND_JOB,
	eCADR_MISMATCH_NAME_BY_CLIENT,
	eCADR_MISMATCH_ID_BY_CLIENT,
	eCADR_INCORRECT_STATE_POINT,
	eCADR_INCORRECT_PET_INFO,
};

//
//	Administrator Level
enum
{
	eAL_NORMAL			=	1,	//	삼성 관계자
	eAL_TESTER			=	2,	//	테스터 - 게임은 가능 하지만 아이템 관련 행동 불가
	eAL_OPERATOR		=	3,	//	관리자 
	eAL_BOSS			=	4,	//	보스
	eAL_MASTER_OPERATOR	=	5,	//	운영팀장
	eAL_SYSTEM_OPERATOR	=	6,	//	영찬이나 지현이
};


//
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

#define	dEVENT_RANGE_WITH_NPC					350

#define	dBI_PASSIVE_SOLO						0	// 성향. 0 맞으면 때린다. 주위에 누군가 맞고 있던 어쨌던 신경 안쓴다.
#define	dBI_PASSIVE_AGGRESSIVE					1	//       1 맞으면 때린다. 주위에 누군가 맞고 있으면 덤빈다.(링크 안되어 있어도/상대가 어떻든..)
#define	dBI_AGGRESSIVE_SOLO						2	//       3 근처에 오면 공격한다. 주위에 누군가 맞고 있던 어쨌던 신경 안쓴다.
#define	dBI_AGGRESSIVE_AGGRESSIVE				3	//       4 근처에 오면 공격한다. 주위에 누군가 맞고 있으면 덤빈다.(링크 안되어 있어도/상대가 어떻든..)
#define	dBI_NONE								10	//       없어!!

#define	dPOSITION_UPDATE_PERIOD					(dSYNC_FPS/4)

#define d4WAY_DIRECT_COUNT						4

#define d4WAY_DIRECTION_N						0
#define d4WAY_DIRECTION_E						1
#define d4WAY_DIRECTION_S						2
#define d4WAY_DIRECTION_W						3

//JBC	길찾기 추가.	08-09-16
class cFindOtherWayByBC
{
public:
	c4POINT	m_aPos[d4WAY_DIRECT_COUNT];								//4방향의 위치를 구하여 저장할 포인트.
	BOOL	m_aisCheck[d4WAY_DIRECT_COUNT];							//이방향으로 가는 되는지를 체크..
	int		m_aiDistance[d4WAY_DIRECT_COUNT];						//방향의 거리를 저장...
	int		m_iLastDirecion;										//마지막으로 온 방향을 저장 왔다갓다 안하게 하기 위해.
	cPOINT	m_PosLast;												//루프 방지를 위해서.
	
	void	getMovePos(cACTOR* _Actor);
	BOOL	getAllPath(cACTOR* _Actor , int _iX , int _iY , int iNowPath = 0);
	inline void	Reset()	{
		for (int i = 0 ; i < d4WAY_DIRECT_COUNT ; i++)
		{
			memset(m_aPos , 0 , sizeof(c4POINT) * d4WAY_DIRECT_COUNT);
			memset(m_aisCheck , 0 , sizeof(BOOL) * d4WAY_DIRECT_COUNT);
			memset(m_aiDistance , 0 , sizeof(int) * d4WAY_DIRECT_COUNT);
		}
	}
	
	cFindOtherWayByBC();
	~cFindOtherWayByBC();
};

class	CContinuousAttackedPeneltyInfo
{
public:
	int		m_iBeginLevel,m_iEndLevel;
	int		m_iCorrectPhysicalDamage;
	int		m_iCorrectHitChance;
	int		m_iCorrectMagicAttackPower;
};

class	CCorrectIntelligenceFactor
{
public:
	int		m_iLevel;
	float	m_fValue;
};

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃전방 선언
class	cFIELD;		//	필드
class	CClient;
class	CGuild;
class	CGuildSkillEffectInfo;
#define	dMAX_GUILD_SKILL_EFFECT_COUNT	6
//┃전방 선언
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 최대 수치
#define	dMAX_ACTOR_IN_FIELD			2048
#define	dMAX_MONSTER				256
//┃각종 최대 수치
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#define	dMINIMUM_HIT_RATE			10		//	최소 명중률
#define	dMAXIMUM_HIT_RATE			95		//	최고 명중률

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃공통 에니메이션 분류
#define	dACT_WALK				0		//	걷는다.
#define	dACT_RUN				1		//	뛴다.
#define	dACT_READY				2		//	정지(전투중)
#define	dACT_HIT				3		//	맞았다.
#define	dACT_CHANGE1			4		//	무기 바꿈
#define	dACT_CHANGE2			5		//	무기 바꿈
#define	dACT_DEAD				6		//	죽음
#define	dACT_SITDOWN			7		//	앉기
#define	dACT_ACTION_1			8		//	액션 1
#define	dACT_ACTION_2			9		//	액션 2
#define	dACT_ACTION_3			10		//	액션 3
#define	dACT_ACTION_4			11		//	액션 4
#define	dACT_ACTION_5			12		//	액션 5
#define	dACT_ACTION_6			13		//	액션 6
#define	dACT_ACTION_7			14		//	액션 7
#define	dACT_ACTION_8			15		//	액션 8
#define	dACT_ACTION_9			16		//	액션 9
#define	dACT_ACTION_10			17		//	액션 10
#define	dACT_ACTION_11			18		//	액션 11
#define	dACT_ACTION_12			19		//	액션 12
#define	dACT_ACTION_13			20		//	액션 13
#define	dACT_ACTION_14			21		//	액션 14
#define	dACT_ACTION_15			22		//	액션 15
#define	dACT_ACTION_16			23		//	액션 16

#define	dACT_RIDE_DOG_MOVE		12		//	개타고 이동
#define	dACT_RIDE_DOG_READY		13		//	개타고 정지
#define	dACT_RIDE_DOG_ATTACK	14		//	개타고 공격

#define	dACT_MONSTER_BLOCKING	16		//	액션 9

#define	dACT_RIDE_DOG_ACTION_COUNT	3	//	개타고 하는 액션 수

const	int	ct_aBlockingAction[16]	=
{
	13,17,
	dACT_HIT,dACT_HIT,
	12,dACT_HIT,
	23,23,
	14,dACT_HIT,
	dACT_HIT,dACT_HIT,
	dACT_HIT,dACT_HIT,
	dACT_HIT,dACT_HIT,
};

#define	dACT_DEFENSE			14

#define	dACT_DEFAULT_ATTACK		100		//	기본 공격 액션
#define	dACT_DEFAULT_MAGIC		101		//	기본 공격 액션

#define	dACT_BLOCKER			19

#define	dDEFAULT_DEATH_COUNT		(dSYNC_FPS*10)
#define	dSUMMONED_BEAST_DEATH_COUNT	(dSYNC_FPS*30)
#define	dPLAYER_DEATH_COUNT			0x7fff
#define	dPET_DEATH_COUNT			(dSYNC_FPS*60)	//	1분후에 부활한다.


#define	dDIRECT_N		0
#define	dDIRECT_NE		1
#define	dDIRECT_E		2
#define	dDIRECT_SE		3
#define	dDIRECT_S		4
#define	dDIRECT_SW		5
#define	dDIRECT_W		6
#define	dDIRECT_NW		7
#define	dDIRECT_RANDOM	8

//┃공통 에니메이션 분류
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


#define	dRANGE_OF_MOVE_FAILED		100


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃AI
#define	dAI_VARIATION				10

#define	dSTATUS_MOVE				0
#define	dSTATUS_ATTACK				1
#define	dSTATUS_AI					2
#define	dSTATUS_ARRANGE				3
//┃AI
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#define	dHIT_EFFECT_COUNT			5

#define	dMAX_CHARACTER				128		//	최대 캐릭터 수(몬스터 포함)

#define	dTEAM_PLAYER				0		//	플레이어
#define	dTEAM_MONSTER				1		//	몬스터
#define	dTEAM_NPC					2		//	NPC

#define	dCLOSE_AFTER_IMAGE			0
#define	dFADE_AFTER_IMAGE			1
#define	dPUT_AFTER_IMAGE			2
#define	dPUT_AFTER_IMAGE_MANUAL		3

#define	dAFTER_IMAGE_COUNT			8

#define	dLOCK_ON_LOCATE				0x7fff

#define	dMONSTER_GRADE_UNDERING		0	//	최하위 스킬은... 하나 밖에 없고.. 하는 짓도 단순하다.
#define	dMONSTER_GRADE_THROWER		1	//	기본적으로 근접 공격 스킬이 하나 있고, 장거리 공격 스킬이 있다.
#define	dMONSTER_GRADE_MAGIC_USER	2	//	기본적으로 근접 공격 스킬이 하나 있고, 마법이 몇개 있다.
#define	dMONSTER_GRADE_MIDDLE_BOSS	3	//	중간 보스.. 다..
#define	dMONSTER_GRADE_BOSS			4	//	보스급이다.


#define	dPLAYER_UPDATE_TIME			8	//	1초에 4번
#define	dMONSTER_UPDATE_TIME		16	//	1초에 2번

#define	dDEATH_REALLY				1	//	진짜로 죽었어 -_-;;
#define	dDEATH_FAINTING				2	//	기절한거다.

#ifdef	_FOR_INTER
#define	dID_LENGTH					64
#else
#define	dID_LENGTH					20
#endif

#define	dNAME_LENGTH				18

#define	dTILE_XSIZE_SHIFT			6
#define	dTILE_YSIZE_SHIFT			5

enum eGREATEGODGRACE
{
	eGREATEGODGRACE_NONE,
	eGREATEGODGRACE_FIRE,
	eGREATEGODGRACE_WATER,
	eGREATEGODGRACE_WIND,
	eGREATEGODGRACE_EARTH,
	eGREATEGODGRACE_LIGHT,
	eGREATEGODGRACE_DARK,
};

//
//	액터 데이터(서버에 저장할때 이 데이터를 저장한다.)
class	CPlayerSaveData	:	public CJobBasicDataDefine , public CActorEquipment
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

	WORD				m_bf1IsRenewalRenewItemPeriodSystem				:	1;	//	빈거
	WORD				m_bf1IsAddQuestCountDownTimer20055027			:	1;	//	빈거

	WORD				m_bf1IsIncorrectConnectByGuildBattle			:	1;
	WORD				m_bf1IsResetHidePitchmanShop20041102			:	1;	//	빈거
	WORD				m_bf3UseBoostCount								:	3;
	WORD				m_bf1AddResetSkillStateScroll					:	1;
	WORD				m_bf1IsHidePitchmanShop							:	1;
	WORD				m_bf1AddData2005_03_03							:	1;
																		//	16
	cAbility			m_aAbility[dABILITY_COUNT];	//어빌러티
	CPlayerTitleInfo	m_aTitle[dMAX_PLAYER_TITLE_COUNT];
	tsProcessQuestField	m_aProcessQuest[dMAX_PROCESS_QUEST_COUNT];
	CPlayerWayPointInfo	m_aWayPoint[dMAX_PLAYER_WAY_POINT_COUNT];

//	서버에서만 필요한 정보
	DWORD				m_dwPlayTime;		//	플레이 타임 분 단위
	DWORD				m_bf12DeathPeneltySecond		: 12;	//	죽음으로 인한 능력치 페널티 타임
	DWORD				m_bf6ProhibitCommunityMinute	: 6;
	DWORD				m_bf5LastUpdateDay				: 5;
	DWORD				m_bf1WaitForRecallForKelby		: 1;
	DWORD				m_bf1WaitForRecallForSwellfer	: 1;
	DWORD				m_bf1WaitForRecallForWindy		: 1;
	DWORD				m_bf1WaitForRecallForHedger		: 1;
	DWORD				m_bf4RemainTimeForUseBreedingRecordBook	:	4;	
	DWORD				m_bf1IsCheckLottoBoxEvent		:	1;
														//	31
	DWORD				m_dwSaveCount;
	DWORD				m_dwPeneltyTime;
	
	WORD				m_bf1IsPlayOnGuildBattle		:	1;	//	길드전 중이다
	WORD				m_bf5GuildBattleDay				:	5;	//	길드전 입장한 날
	WORD				m_bf1IsTeleportToCouple			:	1;	//	커플링을 사용해 텔레포트중이다.
	WORD				m_bf7UseCoupleRingSlot			:	7;	//	사용한 커플링 아이템 슬롯
	WORD				m_bf1CancelQuestWork			:	1;
	WORD				m_bf1CancelQuestWork2			:	1;
														//	16

	WORD				m_wAvatarIndex;				//	아바타 인덱스
	WORD				m_wRenewPremiumItemCounter;	//

	BYTE				m_aCompleteQuest[dQUEST_BUFFER_SIZE];

	WORD				m_bf10ESD_Time			:	10;	//	enter secret dungeon time
	WORD				m_bf6ExtraInventorySize	:	6;
														//	16;

	UTime				m_timeGuildJoinTime;		//	길드 가입한 시간
	CItemDefine			m_itemFailedPremiumItem;
	cPetDefine			m_aPet[dOWN_PET_COUNT];

	WORD				m_bf8ModifyDataCounter	:	8;
	WORD				m_bf5ActiveMiniPet2		:	5;
	WORD				m_bf3GreateGodGrace		:	3;
//	16;

	BYTE				m_bRemainSecondForUseGateGlove;

	BYTE				m_bf1IsMoveFieldByDeath		:	1;	//	죽어서 마을로 돌아 왔다.
	BYTE				m_bf1IsMoveFieldByTeleport	:	1;	//	죽어서 마을로 돌아 왔다.
	BYTE				m_bf1IsAddIncreaseLastDamageBy20070508	:	1;
	BYTE				m_bf1IsChangePetJobData	:	1;
	BYTE				m_bf1IsAddCH5System		:	1;
	BYTE				m_bf3EventCounter		:	3;

	UTime				m_timeLastNomannerPointTime;
	BYTE				m_aCompleteArbeit[dARBEIT_BUFFER_SIZE];

	cItem				m_aExtraInventory[dEXTRA_INVENTORY_SIZE];
	WORD				m_wBonusStatePoint;

	DWORD				m_bf5GiveANoMannerPointDay	:	5;
	DWORD				m_bf8InstanceField			:	8;
	DWORD				m_bf4InstanceFieldFloor		:	4;
	DWORD				m_bf4SecretDungeonPlayCount	:	4;
	DWORD				m_bf1IsAnimatePartnerStatus	:	1;
	DWORD				m_bf1IsResetRedStonePoint	:	1;
	DWORD				m_bf1IsAddAboutMainQuestInfo:	1;
	DWORD				m_bf1IsSaveUserCustomData	:	1;
	DWORD				m_bf1IsReadyToSelectGuildMark:	1;
	DWORD				m_bf1IsAddMiniPetSystem		:	1;
	DWORD				m_bf5ActiveMiniPet			:	5;	//	32

	WORD				m_awQuestCountDownTimer[dMAX_PROCESS_QUEST_COUNT];

	WORD				m_wRedStonePoint;
	WORD				m_wBoostExpSecondByTreasureMapEvent,m_wBoostGoldSecondByTreasureMapEvent;

	WORD				m_wProcessChapterIndex;
	WORD				m_wProcessHighQuestIndex;

	WORD				m_awMainQuestValue[10];
	BYTE				m_abMainQuestSwitchStatus[12];

	WORD				m_awSkillCoolTime[c_iCoolTimeSkillCount];

	int					m_iTodayWithdrawGoldInGuild;
	BYTE				m_abTodayWithdrawItemCount[4];

	BYTE				m_aCompleteQuest2[dQUEST_BUFFER_EXTEND_SIZE];

	WORD				m_wIncreaseFinalDamageTime;
	WORD				m_wIncreaseFinalDamage;

	cMiniPet			m_aMiniPet[c_iMiniPetCount];

	UINT				m_reversalFrameYear	:	5;
	UINT				m_reversalFrameMonth:	4;
	UINT				m_reversalFrameDay	:	5;
	UINT				m_reversalFrameHour	:	5;
	UINT				m_reversalFrameMinute:	6;	//	25
	UINT				m_bf1IsLevelDown	:	1;
	
//	2007 11 02 챕터 5 추가를 위해 추가한 데이터들
	WORD				m_bf4CH5StateBonusCount	:	4;
	WORD				m_bf4CH5SkillBonusCount	:	4;
	WORD				m_bf4CH5HPBonusCount	:	4;
	WORD				m_bf4CH5CPBonusCount	:	4;

	UINT				m_bf4CH5PeneltyHPBonusCount		:	4;
	UINT				m_bf4CH5PeneltyCPBonusCount		:	4;
	UINT				m_bf5BardTitleCoolTime			:	5;
	UINT				m_bf4LevelOfLatestReceivedCH5EquipLevelBonus	:	4;//17
	UINT				m_bf3GuildDungeonPlayTime		:	3;//20
	UINT				m_bf1IsTeleportToPointAround	:	1;	//21 필드이동했을때 이동한 지점 주위에 도착한다.

	WORD				m_awItemCoolTime[dITEM_COOL_TIME_BUFFER_SIZE];

	UINT				m_bf4EatFireMiniPetCount	:	4;
	UINT				m_bf4EatWaterMiniPetCount	:	4;
	UINT				m_bf4EatWindMiniPetCount	:	4;
	UINT				m_bf4EatEarthMiniPetCount	:	4;
	UINT				m_bf4EatLightMiniPetCount	:	4;
	UINT				m_bf4EatDarkMiniPetCount	:	4;	//	24
	UINT				m_bf4RebirthCount			:	4;	//	전생 횟수
	UINT				m_bf4UseSealSphereOfDawnCount:	4;	//	새벽의 봉인구 사용 횟수

	BYTE				m_aCompleteArbeit2[dARBEIT_BUFFER_SIZE];

	UINT				m_bf6LastTransAvatarToGVGYear	:	6;	//	마지막으로 전송한 연도
	UINT				m_bf4LastTransAvatarToGVGMonth	:	4;
	UINT				m_bf5LastTransAvatarToGVGDay	:	5;
	
	void				reset();	//	초기화
	void				initializeJobData(char *_strId,char *_strName,int _iJob);	//	_iJob을 이용해 기본 데이터를 설정한다.

};	//	class	CPlayerData

#define	dSSF_COMPLETE_PROTECTION	1
#define	dSSF_SHIMERING_SHIELD		2

//
//	액터 데이터(필드 이동시에도 필요)
class	CPlayerData	:	public CPlayerSaveData,public CUpkeepPlayerData
{
public:
						CPlayerData()
						{
							reset();
						}

	inline	void		reset()
	{
		CUpkeepPlayerData::reset();
		CPlayerSaveData::reset();
	}
};	//	class	CPlayerData

//
//	Ai State
enum
{
	eAS_READY				,	//	대기
	eAS_MOVE				,	//	이동
	eAS_PATROL				,	//	순찰
	eAS_TM_READY			,	//	조련된 몬스터 대기
	eAS_TM_TRACE			,	//	조련된 몬스터 추적
	eAS_TM_PICK_ITEM		,	//	조련된 몬스터 아이템 집기!!

	eAS_SAGE_MASTER_TRACE	,	//	세이지 마스터 스토킹
	
	eAS_APPROACH			,	//	접근
	eAS_FIGHT				,	//	싸움
	eAS_TRACE				,	//	추적하라!!
	eAS_CONFUSE				,	//	혼란

	eAS_NONE				,	//	아무것도 안한다.
};

#define	dDEFAULT_WAIT_TIME_FOR_NEXT_MOVE	10000	//	20초

#define	dMOVE_NPC_UPDATE_COUNT_PER_SECOND	dSYNC_FPS
const	int	dMOVE_NPC_UPDATE_PERIOD			=	dSYNC_FPS/dMOVE_NPC_UPDATE_COUNT_PER_SECOND;

#define	dMOVE_UPDATE_TIME					(dSYNC_FPS)
#define	dENEMY_FIND_UPDATE_TIME				(dSYNC_FPS)

#define	dMONSTER_SIGHT_UPDATE_TIME			(dSYNC_FPS/2)
#define	dAPPROACH_TARGET_REFRESH_TIME		(dSYNC_FPS/2)

class	cBOOKED_BATTLE
{
public:
	WORD			m_wTarget;
	cAbility		m_ability;

					cBOOKED_BATTLE()	{reset();}

	inline	void	reset()	{m_wTarget	=	0xffff;}
	inline	void	set(int _iTarget,cAbility *_lpAbility)
	{
		m_wTarget	=	_iTarget;
		m_ability.copy(_lpAbility);
	}
};

enum
{
	eBOOKED_ACTION_TO_ACTOR,
	eBOOKED_ACTION_QUICK,
	eBOOKED_ACTION_TO_GROUND,
};

class	cACTOR_BOOKED_ACTION
{
public:
	WORD		m_wType;
	WORD		m_wTargetSerial;	//	대상 시리얼
	WORD		m_wAttackAbility,m_wEngageAbility,m_wSubAbility;
	cPOINT		m_posTarget;

				cACTOR_BOOKED_ACTION()	{reset();}

	inline void	reset(){m_wType=0xffff,m_wTargetSerial=0xffff;}

	void		setToActorAction(CG_ACTION_TO_ACTOR *_lpPacket)
	{
		m_wType				=	eBOOKED_ACTION_TO_ACTOR;
		m_wTargetSerial		=	_lpPacket->wTargetSerial;	//	대상 시리얼

		m_wAttackAbility	=	_lpPacket->bAbility;
		m_wEngageAbility	=	_lpPacket->bEngageAbility;
		m_wSubAbility		=	_lpPacket->bSubAbility;

		if (m_wSubAbility	==	0xff)	m_wSubAbility	=	0xffff;
	}

	void		setQuickAction(CG_ACTION_QUICK *_lpPacket)
	{
		m_wType				=	eBOOKED_ACTION_QUICK;

		m_wAttackAbility	=	_lpPacket->wAbility;
	}

	void		setToGroundAction(CG_ACTION_TO_GROUND *_lpPacket)
	{
		m_wType				=	eBOOKED_ACTION_TO_GROUND;

		m_wAttackAbility	=	_lpPacket->wAbility;
		m_posTarget.x		=	_lpPacket->wDestPosX;
		m_posTarget.y		=	_lpPacket->wDestPosY;
	}
};

#define	dBATTLE_STATUS_TIMER					10	//	일단 전투를 시작하면 30초간 교전 상태가 된다.
#define	dLOGOUT_WAIT_TIMER						10	//	일단 전투를 시작하면 30초간 교전 상태가 된다.
#define	dSYNC_PLAYER_STATUS_PERIOD				(dSYNC_FPS*5)
#define	dSYNC_PLAYER_STATUS_PERIOD_IN_BATTLE	(dSYNC_FPS)		//	전투중에는 1초마다 싱크 시킨다.

#define	dEXPIRED_ITEM_BUFFER_SIZE				(dOWN_ITEM_COUNT+dEQUIPMENT_PART_COUNT+dEXTRA_INVENTORY_SIZE)
#define	dTAMED_MONSTER_WANDER_CYCLE				30
#define	dTAMED_MONSTER_MOVE_RANGE				300
#define	dTAMED_MONSTER_SIGHT					500
#define	dTAMED_MONSTER_OUT_OF_SIGHT				700
#define	dTAMED_MONSTER_SAFE_RANGE				180
#define	dTAMED_MONSTER_MOVE_UPDATE_TIME			(dSYNC_FPS/2)

enum
{
	eAK_PLAYER	,
	eAK_NPC		,
	eAK_MONSTER	,

	eAK_SUMMON_BEAST	=	100,	//	소환수
	eAK_PET				=	101,	//	펫
};

//
//	AI Type
enum
{
	eAT_MONSTER			,
	eAT_NPC				,
	eAT_SLAVE			,
	eAT_TAMED_MONSTER	,
	eAT_SUMMONED_BEAST	,
};

#define	dMAX_HOSTILE_ENEMY_COUNT	10

class	cActiveMiniPetInfo
{
public:
	WORD					m_wChargeCountForMiniPet,m_wIsCharged;
	WORD					m_wRemainFireCountOfMiniPetChargeSkill,m_wMiniPetChargeSkillTarget,m_wCountOfMiniPetChargeSkill;
	cMiniPetAbilityInfo		m_firedMiniPetChargeAbility;
};
class	cFightEnergy
{	// 투기..
public:
	WORD	m_wTime;
	WORD	m_wState;
	int		m_iValue;
	
	cFightEnergy()
	{
		reset();
	}
	
	~cFightEnergy()
	{
	}
	
	
	void	set(WORD	_wFightEnergyTime, WORD	_wFightEnergyState, int	_iFightEnergyValue)
	{
		m_wTime	= _wFightEnergyTime;
		m_wState = _wFightEnergyState;

		if(isIncreaseAttackCount() || isDecreaseCoolTime() )
		{
			m_iValue = _iFightEnergyValue/100;
		}
		else if(isIncreaseAbsoluteCriticalPercent())
		{	
			m_iValue = _iFightEnergyValue/10;
		}
		else
		{
			m_iValue = _iFightEnergyValue;
		}
	}
	void	reset()
	{
		m_wTime	= 0;
		m_wState = 0;
		m_iValue = 0;
	}
	void	decreaseTime()
	{
		if(m_wTime)
		{
			--m_wTime;
			if(m_wTime == 0)
				reset();
		}
	}
	BOOL	isFightEnergy()
	{
		if(m_wTime)
		{
			if(m_wState)
				return TRUE;
		}
		return FALSE;
	}	
	BOOL	isAddAbsoluteAttackPower()
	{
		if(m_wTime)
		{
			if(m_wState == eSKILL_FEE_ADD_ABSOLUTE_ATTACK_POWER)
				return TRUE;
		}
		return FALSE;
	}
	BOOL	isIncreaseAttackCount()
	{
		if(m_wTime)
		{
			if(m_wState == eSKILL_FEE_INCREASE_ATTACK_COUNT)
				return TRUE;
		}
		return FALSE;
	}
	BOOL	isIncreaseGetCP()
	{
		if(m_wTime)
		{
			if(m_wState == eSKILL_FEE_INCREASE_GET_CP)
				return TRUE;
		}
		return FALSE;
	}
	BOOL	isIncreaseAbsoluteCriticalPercent()
	{
		if(m_wTime)
		{
			if(m_wState == eSKILL_FEE_INCREASE_ABSOLUTE_CRITICAL_PERCENT)
				return TRUE;
		}
		return FALSE;
	}
	BOOL	isDecreaseCoolTime()
	{
		if(m_wTime)
		{
			if(m_wState == eSKILL_FEE_DECREASE_COOL_TIME)
				return TRUE;
		}
		return FALSE;
	}
};


class	CBitInfo
{
public:

	WORD	m_wTargetSerial;
	DWORD	m_dwNameHashCode;
	WORD	m_wTime;	
	
	void	reset()
	{
			m_wTargetSerial		= 0xffff;
			m_dwNameHashCode	= 0xffffffff;
			m_wTime				= 0;
	}
	void	set(WORD _wSerialInField, DWORD _dwNameHashCode, int iTime)
	{

			m_wTargetSerial		= _wSerialInField;
			m_dwNameHashCode	= _dwNameHashCode;
			m_wTime				= iTime;
	}

	BOOL	isEqual(WORD _wSerialInField, DWORD _dwNameHashCode)
	{
		if(m_wTargetSerial == _wSerialInField && m_dwNameHashCode == _dwNameHashCode)
			return TRUE;

		return FALSE;
	}
	
	
};

class	CBitInfoManager	
{
public:

	CBitInfo	m_bitInfo[dMAX_BIT_STICK_ACTOR];				// 비트 정보
	int			m_iCount;

	void	reset()
	{
		m_iCount	=	0;

		for(int i=0;i<dMAX_BIT_STICK_ACTOR;++i)
		{
			m_bitInfo[i].reset();
		}
	}

	BOOL	addBit(WORD _wSerialInField, DWORD _dwNameHashCode, int iTime)
	{
	
		if(m_iCount >= dMAX_BIT_STICK_ACTOR)
			return	FALSE;

		if(getCount(_wSerialInField,_dwNameHashCode) >= dMAX_CAN_USE_BIT_TO_ONE)
			return FALSE;

		m_bitInfo[m_iCount].set(_wSerialInField,_dwNameHashCode,iTime);
		++m_iCount;

		return TRUE;
		
	}

	int	removeBit(WORD _wSerialInField, DWORD _dwNameHashCode, int _iCount)
	{
		int iCount	=	0;
		

		for(int i=0;i<m_iCount;++i)
		{
			if(m_bitInfo[i].isEqual( _wSerialInField, _dwNameHashCode))
			{
				removeBit(i);
				--i;
				++iCount;
				if(iCount>=_iCount)
					break;
			}
		}

		return iCount;
	}
	
	void	removeBit(WORD _wIndex)
	{
		memcpy(&m_bitInfo[_wIndex], &m_bitInfo[m_iCount-1], sizeof(CBitInfo));
		m_bitInfo[m_iCount-1].reset();
		--m_iCount;
	}

	void	decreaseTime()
	{

			for(int i=0;i<m_iCount;++i)
			{

				--m_bitInfo[i].m_wTime;
				if(m_bitInfo[i].m_wTime <= 0)
				{
					removeBit(i);
					--i;
				}
			}
	}
	CBitInfo*	getBitInfo(WORD _wIndex)
	{
		if(_wIndex >= dMAX_BIT_STICK_ACTOR)
			return	NULL;
		
		return &m_bitInfo[_wIndex];
	}
	BOOL	decreaseTime(WORD _wIndex)
	{
		--m_bitInfo[_wIndex].m_wTime;

		if(m_bitInfo[_wIndex].m_wTime <= 0)
		{
			removeBit(_wIndex);
			return TRUE;
		}
		return FALSE;
		
	}
	int		getCount(WORD _wSerialInField, DWORD _dwNameHashCode)
	{
		int	iCount	=	0;
		for(int i=0;i<m_iCount;++i)
		{
			if(m_bitInfo[i].isEqual( _wSerialInField, _dwNameHashCode))
			{
				++iCount;
			}
			
		}
		return iCount;
	}

};

class cDropInfoOfSummonEventMonster
{	// 이벤트 몬스터 드랍아이템 정보
public:
							
	WORD					m_wKind;		// 드랍종류
	WORD					m_wDropItem;	//	드랍아이템
	WORD					m_wDropChance;	// 드랍확률
	WORD					m_wDropCount;	// 드랍개수
	int						m_aiOption[4];	//	드랍아이템 생성 옵션
	BOOL					m_bIsOwner;
	void	reset()
	{
		m_wKind			=	0;	
		m_wDropItem		=	0xffff;
		m_wDropChance	=	0;	
		m_wDropCount	=	0;
		m_bIsOwner		=	FALSE;
		memset(m_aiOption,0xffff,sizeof(m_aiOption));
	}
};

//
//	현재 필드에서만 사용되는 데이터
class	cACTOR_FIELD_DATA
{
public:
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃시리얼들
	int					m_iZoneSerial,m_iTargetZoneSerial;				//	존에서의 시리얼
	int					m_iClientSerial;			//	플레이어의 시리얼(서버에서 관리하는 시리얼)
	WORD				m_wSerialInField;			//	액터 슬롯(필드에서 액터 번호다)
//┃시리얼들
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	BOOL				m_bIsRevisIntCalc;
	BOOL				m_bIsImmunePhysicalDamage,m_bIsImmuneMagicDamage;
	DWORD				m_dwNameHashCode,m_dwIDHashCode;			//	이름의 해쉬 코드
	WORD				m_wServerSign;
	DWORD				m_dwLastSendBigSizeDataTime;
	DWORD				m_dwLastActionTime;
	int					m_iLocalPart,m_iSmallLocalPart,m_iMoveNumber,m_iNotFindPlayerCount;

	BOOL				m_bIsOperateCounter,m_bIsGuildObject,m_bIsGuildCrest;
	int					m_iGuildGuardianType;
	WORD				m_wIsHollowKnight,m_wCurrentLevelDeathPenelty;
	WORD				m_wRemainSecondForUpdateEncryptCode;
	
	c4POINT				m_pos;

	WORD				m_wBattleInclination;		//	기본 AI
	WORD				m_wOrgBattleInclination;
	WORD				m_wRallyTime,m_wCaughtTime;
	WORD				m_wDanceTime;
	WORD				m_wConversionIceTime,m_wFogFormTime,m_wNoMoreFightTime;


	WORD				m_wTeam,m_wOldTeam;					//	팀
	WORD				m_wActorKind;				//	분류
	WORD				m_wActionSpeed;				//	액션 속도
	WORD				m_wScaleX,m_wScaleY;		//	
	WORD				m_wTrueJob;					//	진짜 직업
	WORD				m_wFleeTime,m_wChargeTime;
	WORD				m_wMakeChatLogCounter;
	int					m_iLastLogExp;

	WORD				m_wGuildHallLevel,m_wRankInGuildForSkill;
	WORD				m_wIsReadyToMakeGuild,m_wIsOpendGuildBattleScheduler;	//	길드를 만드는 중이다. 길드전 스케쥴러를 열어둔 상태다.

	WORD				m_wReducePhysicalDamage;	//	물리 대미지 감소

	WORD				m_wUpdateOuterActorInfoPeriod;	
	WORD				m_wIsMoveToDifferentServer;

	LONGLONG			m_dwEnchantedMask;
	WORD				m_wActiveMagicCarpetShape;
	WORD				m_wBoostSpeedByMagicCarpet;

	WORD				m_wUseSkillPosX,m_wUseSkillPosY;
	WORD				m_bf2UseSkillTime			:	2;
	WORD				m_bf14LastestUseSkill		:	14;

	WORD				m_bf2UseItemTime			:	2;
	WORD				m_bf14LastestUseItem		:	14;

	DWORD				m_bf1IsExistDrakeCarpet					:	1;
	DWORD				m_bf1IsExistSquirrelCarpet				:	1;
	DWORD				m_bf1IsExistTurttleCarpet				:	1;
	DWORD				m_bf1IsAvoidThisTimeAttack				:	1;
	DWORD				m_bf1IsUseAbleBulletSubstituteArrow		:	1;
	DWORD				m_bf1IsOperateEventAreaWork				:	1;
	DWORD				m_bf1IsCancelKnockBack					:	1;
	DWORD				m_bf1IsDisableInvisibleByAttack			:	1;

	DWORD				m_bf1IsStolenGold						:	1;
	DWORD				m_bf1IsStolenItem						:	1;	//	10
	DWORD				m_bf1IsInstanceKill						:	1;	//	즉사로 죽었다. 
	DWORD				m_bf1IsOperateReadyToRemoveActorWork	:	1;
	DWORD				m_bf1IsTraceGuide						:	1;
	DWORD				m_bf1IsTakeMinDamageForThisTimeAttack	:	1;
	DWORD				m_bf1IsChanceToGetTreasureMap			:	1;	//	보물 지도를 얻을 수 있는 기회다!!
	DWORD				m_bf1IsExistCopySlave					:	1;	//	복제된 노예? 가 있다.
	DWORD				m_bf1IsDeathByAnimateUndead				:	1;	//	에니메이트 언데드 였다가 죽었다.
	DWORD				m_bf1IsProcessEvent						:	1;	//	이벤트 진행 중이다.
	DWORD				m_bf1IsMirrorImage						:	1;	//	미러 이미지다
	DWORD				m_bf1IsBlockToRevenge					:	1;	//	20 반격 금지
	DWORD				m_bf1IsOpenGuildHallList				:	1;	//  길드홀 리스트를 열었다.
	DWORD				m_bf1IsOpenGuildInventory				:	1;	//  길드 인벤토리 열려 있다.
	DWORD				m_bf1IsOpenRelatedPlaceOfGuild			:	1;	//  길드 연고지 리스트 열림
	DWORD				m_bf1AddEntryGuildDungeon				:	1;	//	
	DWORD				m_bf1IsAddGetMysticStone		:	1;	//	신비석 추가 획득 
	DWORD				m_bf1IsAddGetTantalissRelic		:	1;	//	탄탈리스 유물 추가 획득 
	DWORD				m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile	:	1;	//	길드홀 레벨 제한 없이 탄탈리스 유적 입장 
	DWORD				m_bf1IsInfinitySeriousUpgradeChanceUp	:	1;	//	
	DWORD				m_bf1IsSoundOfLedersBellsLeader			:	1;
	DWORD				m_bf1IsEventSummonMonster				:	1;		//30 이벤트 소환몬스터~


	UINT				m_uiLastReturnToVillageTime;
	WORD				m_wMirrorImageMasterNpcSerial;	//	미러 이미지 뿌려놓고 자는 중이다.
	WORD				m_wBlockedPosValue;
	uEnchantedImage		m_enchantedImage;
	WORD				m_wSwallowCorpesCount;
	WORD				m_wBlockToActionTime;
														//	민망한 변수 - -

	CEnchantedMagicInfo		m_aEnchantedMagic[dMAX_ENCHANTED_MAGIC_COUNT_FOR_CHARACTER];

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Player 전용 데이터
	LONGLONG				m_llLastGold,m_llGettingGold,m_llSpentGold,m_llTradeGettingGold,m_llTradeSpentGold;
	int						m_aiLastReceiveQuestExp[dCHECK_QUEST_EXP_BUFFER_SIZE][2];

	BOOL					m_bIsWaitOrgAvatarData;
	int						m_iExpPeneltyForRebirthPlayer;
	cDuelRecord				m_duelRecord;	//	결투 정보
	WORD					m_wDuelType,m_wDuelSerial,m_wRemainTimeForRebirth;
	WORD					m_wDuelKillCount,m_wDuelDeathCount;
	WORD					m_wWeeklyDuelRank,m_wDailyDuelRank,m_wTrialGameRank;

	WORD					m_wDuelBuffType,m_wDuelBuffTime,m_wDuelBuffCoolTime;
	DWORD					m_dwLastCheckMoveSpeedTime;
	WORD					m_wMovePixelSumForCheckHacker;
	WORD					m_wReceiveMovePacketCounter;
	WORD					m_wEventArea;
	
	WORD					m_wBlockToTransformationTime;

	WORD					m_wIsActiveReversalFrame;
	cActiveMiniPetInfo		m_aActiveMiniPetInfo[2];

	int						m_iCarrotCount;

	WORD					m_bf3TargetObjectType	:	3;
	WORD					m_bf13TargetObject		:	13;

	BOOL					m_bIsReadyToExit;

	WORD					m_wAttackRange,m_wAttackSpeed;

	WORD					m_wHostileEnemyCount;
	WORD					m_awHostileEnemy[dMAX_HOSTILE_ENEMY_COUNT];

	WORD					m_wJumpFlyCounter;

	BOOL					m_bIsApplyHuntingExpBonus;
	WORD					m_wCorrectExp;
	WORD					m_wCorrectItemDropRate;
	WORD					m_wTalkNpc,m_wSelectConversation;

	WORD					m_wPartyExpBonus;

	WORD					m_wItemCount;			//	아이템의 수
	WORD					m_wItemSlotCount;			//	아이템 슬롯 카운트
	WORD					m_wLimitEquipmentLevelOverPoint;
	WORD					m_wLimitEquipmentLevelOverPointByCH5;
	WORD					m_wIsProcessLogout;

	WORD					m_wWaitLogoutResultTime;
	WORD					m_wWaitTransGVGResultTime;
	WORD					m_wWaitMoveFieldResultTime;

	WORD					m_wBankSize,m_wBankCharge,m_wWaitBankDataSaveResultTime;	//	은행 사이즈와 은행 수수료
	WORD					m_wWaitBankDataTime,m_wBankSerial;

	WORD					m_bf1IsProcessRebirth			:	1;	//	전생 진행 중이다.
	WORD					m_bf1IsExistExceptExpPortal		:	1;	//	경험치 보너스만 없는 포탈 보유중

	WORD					m_wCartSerial,m_wWaitDBMessageSecond,m_wIsWaitCartMessage,m_wWaitBuyCarrotShopItemResultTime;
	DWORD					m_dwLastestAskPremiumItemCartInfoTime;

	WORD					m_wPitchmanShopSize,m_wPitchmanShopSerial;	//	은행 사이즈와 은행 수수료
	WORD					m_wShopSerial;

	WORD					m_wReceiveFieldActorData;	//	전송 받은 필드 액터 데이터
	WORD					m_wReceiveFieldItemData;	//	전송 받은 필드 아이템 데이터
	WORD					m_wIsReceivedPitchmanShopList,m_wIsReceivedPortalList;
	WORD					m_iRookieTime;//신입이다.
	short					m_sIsReloadingBeltItem,m_sReloadTime;//리로드 남은 시간
	int						m_iLastSyncHP,m_iLastSyncMaxHP;//마지막으로 클라이언트에 업데이트 했을때의 HP
	WORD					m_wRemainSyncTime;
	WORD					m_wRestraintTimeByBattle,m_wRestraintTimeByLogout,m_wUnderBattleTime;//전투로 인해 구속중인 시간
	WORD					m_wDeathCounterForMiniPet;
	WORD					m_wPeaceTime;	//	전투 안하고 있는 시간
	int						m_iVisibleActorCheckSum;
	WORD					m_wShoutCount;
	int						m_iLastestCheckSumValueForSaveData;
	WORD					m_wBookedRemoveTime;
	WORD					m_wIsHideShield;
	WORD					m_wHideWeapon1Time,m_wHideWeapon2Time;
	WORD					m_wTradeBoxSerial;
	WORD					m_wOperatorLevel,m_wOperatorLevelInAccount;
	WORD					m_wIsIncorrectFieldRequireReturnToVillage;	//	엉뚱한 필드에 들어 왔다.
	WORD					m_wIsIncorrectGuildBattleFieldJoin;	//	잘못된 길드전 필드 참가다.
	DWORD					m_dwLastShoutTime;
	DWORD					m_dwLastChatTime,m_dwLastChatHashCode;
	DWORD					m_dwLastAttackedTime;
	WORD					m_wContinuousAttackedCounter;	//	연속해서 공격을 당한 횟수
	
	DWORD					m_bf1IsOXQuizWinner : 1;			//	OX퀴즈 승자다.
	DWORD					m_bf1IsOXQuizLoser  : 1;			//	OX퀴즈 승자다.
	DWORD					m_bf1IsOwnedPitchmanShopSignBoard1	: 1;
	DWORD					m_bf1IsOwnedPitchmanShopSignBoard2	: 1;
	DWORD					m_bf1IsOwnedPitchmanShopSignBoard3	: 1;
	DWORD					m_bf1IsOwnedPitchmanShopSignBoard4	: 1;
	DWORD					m_bf1IsOwnedPitchmanShopSignBoard5	: 1;
	DWORD					m_bf1IsOwnedPitchmanShopLoudSpeaker	: 1;
	DWORD					m_bf1IsOwnedPitchmanColorPen		: 1;
	DWORD					m_bf1IsOwnedPitchmanShopBoldPen		: 1;
	DWORD					m_bf1IsOwnedPitchmanShopLongPen		: 1;
	DWORD					m_bf1IsOwnedPitchmanShopTwinkle		: 1;
	DWORD					m_bf1IsCancelKnockOut				: 1;	//	녹 아웃 공격 버티기
	DWORD					m_bf1IsImmuneKnockBack				: 1;
	DWORD					m_bf1IsHaveSummonMagicCarpetItem	: 1;

	DWORD					m_dwAverageReceivePacketSizePer10Second;
	DWORD					m_dwReceivePacketSizePer10Second;
	WORD					m_wAverageReceivePacketCountPer10Second;
	DWORD					m_dwReceivePacketCount;

	DWORD					m_dwAverageSendPacketSizePer10Second;
	DWORD					m_dwSendPacketSizePer10Second;
	WORD					m_wAverageSendPacketCountPer10Second;
	DWORD					m_dwSendPacketCount;

	DWORD					m_dwCheckCountOfPacketFlowing;
	DWORD					m_dwLastCheckTimeOfPacketFlowing;

	WORD					m_wMagicCarpetShape;
	WORD					m_wPremiumServiceLevel;	//	프리미엄 서비스 레벨
	WORD					m_wPremiumServiceGrade;	//	프리미엄 서비스 레벨
	WORD					m_wBlockSelectSpeechTime;

	WORD					m_wRequestTradePlayer;
	DWORD					m_dwRequestPartyPlayerNameHashCode;
	WORD					m_wRequestFriendSerial;
	WORD					m_wRequestGuild;
	WORD					m_wCorrectSecretDungeonPlayCount;
	WORD					m_wCorrectGuildDungeonPlayCount;
	WORD					m_wCorrectMiniPetGainExpByFeedItem;

	WORD					m_wIsSendedCorrectPosPacket;
	int						m_iCorrectHPByGuildStategyPoint,m_iIncreaseDamageByGuildStategyPoint,m_iReduceDamageByGuildStategyPoint;

	WORD					m_wTreasureBoxXPos,m_wTreasureBoxYPos;
#ifdef	_USE_NPGL
	int						m_iLastSendGGAuthCodeTime;
#endif
//┃Player 전용 데이터
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃테스트전용  데이터
#ifdef	_DEBUG
	UINT					m_bf1DevIsOneShotOneKill	:	1;	//	한방에 다 죽인다.
	UINT					m_bf1DevIsPerfectHit		:	1;	//	무조건 맞춘다
	UINT					m_bf1DevIsPerfectDodge		:	1;	//	무조건 피한다.
	UINT					m_bf1DevIsOneShotOneDeath	:	1;	//	한방에 죽는다.
	UINT					m_bf1DevIsBlindPerson		:	1;	//	장님... 절대 못 맞춘다. - -
	UINT					m_bf1DevIsVillageDrum		:	1;	//	동네북... 공격 당하면 무조건 맞는다. - -
	UINT					m_bf7DevTargetReduceHPRate	:	7;	//	적 체력 깍는 수준 결정
	UINT					m_bf7DevMyReduceHPRate		:	7;	//	내 체력 깍이는 수준 결정
	UINT					m_bf1DevIsGuildItemDrop		:	1;	//21	길드아이템드랍확률 100%!
	UINT					m_bf1DevIsCarvingSuccess	:	1;	//22  각인 성공확률 100%!
	UINT					m_bf1DevIsCarvingFail		:	1;	//23	각인 실패확률 100%!
	UINT					m_bf1IsSendGoldLog			:	1;	//24	골드 로그
	UINT					m_bf1DevIsDamageLimitRelease	:	1;	//25	대미지제한 해제
#endif

//┃테스트전용  데이터
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃NPC전용  데이터
	WORD					m_wNpcActorSaveSerial;		//	파일에 저장된 npc 액터 시리얼 번호 불변이다
	WORD					m_wNpcMoveSpeed;			//	NPC의 이동 속도
	WORD					m_wJobIndex;				//	직업 인덱스
	WORD					m_wLordOperator;			//	운영자에게 지배 당하고 있다. 운영자는 몹을 몹은 운영자 시리얼을 가르킨다.
	WORD					m_wTauntedToTargetTime;		//	타겟한테 도발-_- 당했다.
	WORD					m_wTauntedToTargetSerial;	//	타운트건놈..
	DWORD					m_dwTauntedToTargetNameHashCode;	//	타운트건놈..
	WORD					m_wTauntedToTargetDamagePercent;	//	대미지감소..
	WORD					m_wIsZinMonster,m_wIsBoss;
	WORD					m_wCorrectFinalDamage;
	WORD					m_wCorrectHP;
	WORD					m_wCorrectAP;
	WORD					m_wCorrectMonsterExp;
	WORD					m_wEventValue;
	WORD					m_wCharacterSerial;	//	캐릭터 시리얼
	BYTE					m_wIsSetBoss;
	cCHARACTER				*m_lpCharacter;

	class	cProcessPatternInfo
	{
	public:
		WORD				m_wPattern,m_wStep;
	};

	enum
	{
		eProcessCharacterPatternCount	=	3,
		eProcessValueCount				=	6,
	};

	WORD					m_wProcessPatternLevel,m_wProcessPatternStatus;
	WORD					m_awProcessValue[eProcessValueCount];
	cProcessPatternInfo		m_aProcessPattern[eProcessCharacterPatternCount];

	cDropInfoOfSummonEventMonster	m_dropInfoOfSummonEventMonster;
//┃NPC전용  데이터
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 데이터
	int						m_iMaxHPForDrainLevel;	//	레벨 저하 되기전에 HP
	cRECT					m_rectMoveFrame,m_rectActionFrame;
	cPOINT					m_posDest;					//	목표위치

	BYTE					m_bRealTeam;				//	진짜 팀(유혹을 당했던 어쨌던..)
	char					m_isRunning;				//	뛰는 중이다.
	DWORD					m_dwLastUpdateTime;			//	마지막으로 먼가-_- 업데이트 한 타임
	DWORD					m_dwLastMoveMessageTime;	//	이동중이다. 마지막으로 이동 메시지를 보낸 시기

	WORD					m_wTrapDamageTime;			//	데미지를 입은 시간
	WORD					m_wDamageTrapSerial;		//	데미지를 입힌 함정의 시리얼

	WORD					m_wCPFloatTime;				//	CP가 떠 있는 시간.. -_-??
	WORD					m_wCorrectValueByBardTitle,m_wCorrectValueByBardTitleTime;
	WORD					m_wDruggistTitleFactor,m_wAlchemyTitleFactor,m_wCampingMasterTitleFactor;
	WORD					m_wSurvivalInstinctsTitleTriggerHP,m_wSurvivalInstinctsTitleCriticalChance,m_wSurvivalInstinctsTitleTriggerPropertion;
	WORD					m_wBerserKerTitleBoostPower,m_wBerserKerTitleBoostCount,m_wBerserKerTitleBoostTime;
	WORD					m_wCourtesyTitleValue,m_wCorrectFieldElementalResistanceByTitle;
	WORD					m_wReduceDeathPeneltyValueByTitle;
	WORD					m_wRemainToUseBardTitleTime;
	
	WORD					m_wCorrectHitChanceToHumanByTitle;
	WORD					m_wCorrectHitChanceToUndeadByTitle;
	WORD					m_wCorrectHitChanceToDevilByTitle;
	WORD					m_wCorrectHitChanceToAnimalByTitle;
	WORD					m_wCorrectHitChanceToHolyBeastByTitle;

	WORD					m_wFindGuildStatueKind,m_wFindGuildStatueLevel,m_wLastCheckMinute;
//┃각종 데이터
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃전투 관련 정보
	cACTOR_BOOKED_ACTION	m_bookedAction;
	cBOOKED_BATTLE			m_bookedBattle;				//	예약된 전투

	WORD					m_wEngageRange;				//	사정거리
	WORD					m_bIsDamageFrame;			//	데미지 프레임이다.
	WORD					m_isTriggerFrame;			//	트리거 프레임이다.
	WORD					m_wIsActionFinished;		//	액션이 종료 되었다.
	WORD					m_wIsDontHitAction;
	CAttackInfo				m_attackInfo;				//	히트 결과
	WORD					m_wRemainActionTime;		//	액션이 끝날때까지 남은 시간.
	WORD					m_wRemainBlendingTime;		//	건들지 말것!

	cAbility				m_engageAbility,m_subAbility,m_specialAbility;	//	교전 어빌러티
	BOOL					m_bEngageReactionAbility;			// 09.08.13  카르마발동 예약~된~ 스킬.
	WORD					m_wUseSkill,m_wDefaultSkillSlot;				//	몬스터 기본 스킬 슬롯
	WORD					m_wAttackToObjectType,m_wAttackToObject;
	cPOINT					m_posTarget;				//	지금 설정되어 있는 타겟
	WORD					m_wContinuousHitCount;
	BYTE					m_isGetCPByContinuousHitFirstAttack;	//	연속 공격 카운트,연속 공격 카운트
	BYTE					m_bIllusionAttackFrame;

	int						m_iLastAttackPoint;	//	마지막으로 공격한 포인트
	int						m_iLastDamagePoint;	//	마지막으로 입은 데미지 포인트
	int						m_iLostExperience;

//┃전투 관련 정보
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃AI 관련
	int						m_iOutOfSightRange;			//	목표가 시야에서 벗어나는 거리
	int						m_iReaminSerchEnemyTime;
	WORD					m_wAiState;					//	현재 AI 상태
	int						m_iAiType;
	short					m_sWaitTimeForNextMove;
	WORD					m_wSerchEnemyTime;
	WORD					m_wAiTargetCount;
	WORD					m_wUpkeepTargetTime;
	WORD					m_awAiTargetList[dMAX_AI_TARGET_COUNT];
	BOOL					m_isComeBackToRegenPlace;			//	돌아가야만  할때...

	WORD					m_wApproachTime,m_wAnotherTargetBitMeCount,m_wIsLocalAttack;
//┃AI 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃죽음에 관련된 상태
	int					m_iDeathCount;				//	죽음의 카운트 다운.. -_-a
	WORD				m_wCheckBookingDeathCount;

	WORD				m_wBE_MultiExperience;
	WORD				m_wBE_IncreaseItemDropChance;	// 아이템 드랍률증가..
	WORD				m_wIncreaseInstantExperience;	// 일시적 경험치 획득률 증가.
//┃죽음에 관련된 상태
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃스킬/상태 관련
	WORD				m_wUntimateBarrierTime;
	WORD				m_wUntimateBarrierRange;
	WORD				m_wNotificationOfDeathCounter,m_wNotificationOfDeathCounterCaster,m_wNotificationOfDeathCounterRange,m_wNotificationOfDeathChance;
	WORD				m_wZombieBombPower,m_wZombieBombSkill;
	WORD				m_wRemainShakeTime;		//	건들지 말것!

	WORD				m_aActiveReaction[dACTIVE_REACTION_COUNT];	//	활성화된 리액션들
	WORD				m_aAuraSkillTimer[dACTIVE_REACTION_COUNT];	//	활성화된 리액션들 적용한 시간
	WORD				m_wDummyTime,m_wDummyLord;
	DWORD				m_dwDummyLordNameHashCode;		//	난 더미다!! 주인장
	
	WORD				m_wCopyTime;		//	난 복사됐다.
	WORD				m_wReceiveBlocker,m_wCastBlocker;			//	블록커
	WORD				m_wReceiveWeaponBlocker;
	WORD				m_bf10CastWeaponBlocker		:	10;			//	블록커
	WORD				m_bf1CastWeaponBlockerJob	:	1;
	WORD				m_wDodgeAbility;

//	점프
	WORD				m_wPostureOfDefense;		//	디펜스 스킬 사용중
	WORD				m_wBlockingRigidityTime;	//	방패로 블럭하고 있음.
	WORD				m_wHitActionTime;			//	히트 액션 타임
	char				m_isJumpFly;				//	점프해서 날아가는중-_-이다.

//	빨아 들이기
	WORD				m_wInhaleTime;

//	러쉬
	char				m_isRush;					//	러쉬? 중이다.

//	언덕 스킬
	WORD				m_wHillSkill,m_wHillDefensivePower,m_wHillDodgeCorrect;				//	
	int					m_iHillHP;					//	체력... -_-

//	인터벌 슈터
	WORD				m_wIntervalShooter;			//	인터벌 슈터(시간)

//	유혹
	WORD				m_wCharmingActor;				//	늑대인간의 디스플레이스먼트에서 주인
	DWORD				m_dwCharmingActorHashCode;

//	디스플래이스먼트
	WORD				m_wLordActor;				//	늑대인간의 디스플레이스먼트에서 주인
	DWORD				m_dwLordHashCode;
	WORD				m_wGuideActor;
	DWORD				m_dwGuideHashCode;

	WORD				m_wSlaveActor;				//	종
	WORD				m_wIsFeignDeath;			//	죽은척..?
	WORD				m_wFeignDeathTime;

	WORD				m_wPrincessWeaponPrincess;
	DWORD				m_dwPrincessWeaponPrincessNameHashCode;
	WORD				m_wTransToWeaponTime;
	WORD				m_wPrincessWeaponReceiver;
	DWORD				m_dwPrincessWeaponReceiverNameHashCode;
	WORD				m_wTransToFrogTime;
	
	WORD				m_wDodgeMagicDamageTime,m_wDodgeMagicDamageChance;
	WORD				m_wFlyHeight,m_wFlyTime;

	WORD				m_wTransToFatGirlTime;
	DWORD				m_dwActionTimeUnderFatGirlStatus;
	WORD				m_wActionPerionUnderFatGirlStatus;
	WORD				m_wFatGirlSkill,m_wFatGirlSkillLevel;

	cPOINT				m_posFeignDeath;

//	미러 타워
	WORD				m_wMirrorTowerCaster,m_wMirrorTowerSkill,m_wMirrorTowerCPValue,m_wMirrorTowerFactor,m_wMirrorTowerUpkeepTime,m_wMirrorTowerRange;	//	미러 타워

//	바이트 행잉
	WORD				m_wBiteTarget,m_wBiter;
	
//	캐릭터 복사
	WORD				m_wCopyActorLord;			//	복사한 캐릭터
	DWORD				m_dwCopyActorLordNameHashCode;

//	워프
	DWORD				m_bf1IsReadyToWarpToNearestVillage	: 1;
	DWORD				m_bf1IsReadyToCreateTownPortal		: 1;
	DWORD				m_bf4BlockerShape					: 4;
	DWORD				m_bf2AnimatePartnerCount			: 2;
	DWORD				m_bf1IsTestQuest					: 1;
	DWORD				m_bf1IsSendReapirPartyDisconnectedStatusMessage	:	1;
	DWORD				m_bf1IsReturnDamageByRoot			:	1;
	DWORD				m_bf1IsReadyToBuyPigeonPostItem		:	1;
	DWORD				m_bf6EnablePigeonPostItemCount		:	6;
	DWORD				m_bf8DeliverPrice					:	8;

	WORD				m_wAnimatePartnerTime,m_wAnimatePartnerReviveTime;

	WORD				m_wPortalTime,m_wWarpAbleRange;

//	분신
	WORD				m_wTheOtherSelfCount;
	WORD				m_wUpkeepTheOtherSelfTime;

//	족쇄
	WORD				m_wShakleUpkeepTime;
	int					m_iShakleHP;
	DWORD				m_bf10ShakleCaster						:	10;
	DWORD				m_bf10ShakleSkill						:	10;
	DWORD				m_bf8ShakleSkillLevel					:	8;
	DWORD				m_bf4RemainTimeForDamageByShakleSkill	:	4;

	WORD				m_bf1IsFreezeShakle						:	1;
	WORD				m_bf1IsCriticalShakle					:	1;
	WORD				m_bf1IsSyncDamageShakle					:	1;
	WORD				m_bf7ShakleMagicResistance				:	7;

	WORD				m_wShakleDefensivePower;

//	매직 박스
	WORD				m_wInMagicBoxTime;
	WORD				m_wMagicBoxCasterSerial;
	DWORD				m_dwMagicBoxCasterNameHashCode;
	WORD				m_wKillChanceByMagicBox;

//	환영
	WORD				m_wIllusionLord;

//	마리오네뜨
	WORD				m_wMarionetteCaster,m_wMarionetteTime;
	WORD				m_wCorrectMarionetteStrength;
	WORD				m_wCorrectMarionetteAgility;
	WORD				m_wCorrectMarionetteConstitution;
	WORD				m_wCorrectMarionetteIntelligence;
	WORD				m_wCorrectMarionetteWisdom;
	WORD				m_wCorrectMarionetteLuck;
	WORD				m_wCorrectMarionetteCharisma;
	DWORD				m_dwMarionetteCasterNameHashCode;

//	충동
	WORD				m_wImpulseCaster,m_wRemainImpulseTime,m_wImpulseDamage,m_wImpulseSkill;
	DWORD				m_dwImpulseCasterNameHashCode;

//	피흡혈
	WORD				m_wBloodDrainCaster,m_wBloodDrainSkill;
	DWORD				m_dwBloodDrainCasterNameHashCode;

//	자책
	WORD				m_wSelfReproofCaster,m_wSelfReproofTime,m_wSelfReproofDamage,m_wSelfReproofSkill;
	DWORD				m_dwSelfReproofCasterNameHashCode;

//	고문
	WORD				m_wTortureCaster;
	DWORD				m_dwTortureCasterNameHashCode;

//	악화
	WORD				m_wDegenerateCaster,m_wDegenerateTime,m_wDegeneratePropertion,m_wDegenerateSkill;
	DWORD				m_dwDegenerateCasterNameHashCode;

//	가위
	WORD				m_wTerribleDreamCaster,m_wTerribleDreamTime,m_wTerribleDreamDamage,m_wTerribleDreamSkill,m_wTerribleDreamActivePeriod;
	DWORD				m_dwTerribleDreamCasterNameHashCode;

//	화병
	WORD				m_wHwabyungCaster,m_wHwabyungTime,m_wHwabyungFireDamage,m_wHwabyungDarkDamage,m_wHwabyungSkill;
	DWORD				m_dwHwabyungCasterNameHashCode;

//	음모의 그림자
	WORD				m_wPOS_Target,m_wPOS_Caster,m_wPOS_Time,m_wPOS_FullTime,m_wPOS_Damage,m_wPOS_CasterDamage,m_wPOS_Skill;
	DWORD				m_dwPOS_CasterNameHashCode;

//	죽음의 향기
	WORD				m_wSOD_Time,m_wSOD_Damage,m_wSOD_Skill,m_wSOD_ReduceResistance,m_wSOD_ReduceResistanceTime,m_wSOD_ReduceCurseResistance;

//	회복률 저하
	WORD				m_wDeclientRecoverHPEfficientTime,m_wDeclientRecoverHPEfficient;

//	부활 방해
	WORD				m_wInterruptingReviveTime;

//	계약
	WORD				m_wContractCaster,m_wContractTime,m_wContractOccurPeriod,m_wContractRecoverHP,m_wContractSkill,m_wContractDamageSkill,m_wContractDamageSkillLevel;
	DWORD				m_dwContractCasterNameHashCode;

//	혼의 서약
	WORD				m_wSoulOathTime,m_wSoulOathCP,m_wSoulOathSkill;

//	피의 맹약
	WORD				m_wBloodCompactTime,m_wBloodCompactHP,m_wBloodCompactSkill;

//	불공정 계약
	WORD				m_wInterruptingArmorLevel,m_wInterruptingArmorTime;

//	길드 가디언
	WORD				m_wImmuneStrageStausTime,m_wImmunePhysicalAttackTime,m_wImmuneMagicAttackTime;

//	대미지 풀	
	class	cDamagePoolInfo
	{
	public:
		WORD			m_wRemainTime;
		WORD			m_wEfficient;
		WORD			m_wLinkCount;
		WORD			m_wRange;

		void			set(int _iTime,int _iEfficient,int _iLinkCount,int _iRange)
		{
			if	(m_wRemainTime	>	_iTime)
				return;

			m_wRemainTime	=	max(_iTime,0);
			m_wEfficient	=	max(_iEfficient,0);
			m_wLinkCount	=	max(_iLinkCount,0);
			m_wRange		=	max(_iRange,0);
		}
	};

	cDamagePoolInfo		m_damagePool;
	// 무기해제
	WORD	m_wInterruptingWeaponTime, m_wInterruptingWeaponDeclineDamagePer;
	// 리액션스킬사용금지
	WORD	m_wNotUseActivityReactionSkillTime;
	//	교감상태
	WORD	m_wConsensusTime;
	// 모든스텟증가.
	WORD	m_wIncreaseAllStatusTime, m_wIncreaseAllStatus;
	// 광란
	WORD	m_wMadnessTime;
	// 소울번
	WORD	m_wSoulBurnTime, m_wSoulBurnDamage, m_wSoulBurnSkill, m_wIncreaseSoulBurnDamage, m_wSoulBurnDamageSecond, m_wSoulBurnCaster;
	DWORD	m_dwSoulBurnCasterNameHashCode;
	// 타오르는영혼
	WORD	m_wRageSoulTime, m_wRageSoulDamage, m_wRageSoulDamageGap, m_wRageSoulChance;
	// 언어구속
	WORD	m_wBindingWordsTime, m_wBindingWordsOverLimitLevel;
	// 투기
	cFightEnergy	m_fightEnergy;
	// 돌진
	WORD			m_wRushTime;		// 돌진시간

	WORD				m_aPassiveSkill[dPASSIVE_SKILL_COUNT];	//	패시브 스킬..

	// 투지
	WORD			m_wFightingSpiritTime;	
	int				m_iFightingSpiritValue;
	// 필요상태무시
	WORD			m_wIgnoreNeedStateTime;	
	// 블럭킹무시
	WORD			m_wIgnoreBlockingTime, m_wIgnoreBlockingValue;	

	// 미사일대미지리턴
	WORD			m_wReturnMissileDamageTime, m_wReturnMissileDamageValue;

	// 강타확률증가
	WORD			m_wIncreaseHardBlowTime, m_wHardBlowValue;


	// 분신공격 
	WORD			m_wAttackDoppelgangerCount,  m_wAttackDoppelgangerCaster;
	int				m_iAttackDoppelgangerDamage;
	DWORD			m_dwAttackDoppelgangerCasterNameHashCode;
	DWORD			m_dwActionTimeAttackDoppelganger;
	WORD			m_wActionPerionAttackDoppelganger;
	CAttackInfo		m_attackInfoCHOD;				//	히트 결과
	CBitInfoManager	m_bitInfoManager;
	// 출혈
	WORD			m_wBleedingPeriod;				// 출혈 주기		
	WORD			m_wBleedingTime;				//	지속시간			
	WORD			m_wBleedingCaster;				//	출혈 건놈		
	int				m_iBleedingDamage;				//	출혈대미지

	WORD			m_wElectricShockTime;				//	지속시간			
	WORD			m_wElectricShockDamagePercent;		//	감전효과
	// 즉시힐효과
	WORD			m_wInstanceHealEffectTime;				//	지속시간			
	WORD			m_wInstanceHealEffectPercent;					//	즉시힐효과
	//	언데드화
	WORD			m_wMakeTypeUndeadTime;					//	지속시간
	//	헌혈
	WORD			m_wBloodDonationTime;					//	지속시간
	WORD			m_wBloodDonationPercent;				//	헌혈퍼센트

//┃스킬/상태 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃펫/소환수 관련

//	조련사가 가지고 있어야 할 정보
	WORD				m_awTamedMonsterList[dMAX_TAME_MONSTER_COUNT];
	WORD				m_awSummonBeastList[dOWN_SUMMON_BEAST_COUNT];
	WORD				m_awRemainTimeForRecallSummonBeast[dSUMMON_BEAST_TYPE_COUNT];
	WORD				m_wLastCommandSummonedBeast;

//	소환사/소환수가 가지고 있어야 할 정보
	WORD				m_wRidingDogTime,m_wCorrectMoveSpeedByRidingDog;
	WORD				m_awSBLevel[dSUMMON_BEAST_TYPE_COUNT][2];

//	펫/소환수가 가지고 있어야 할 정보
	WORD				m_wLastMinAttackPower,m_wLastMaxAttackPower,m_iDefensePower;
	WORD				m_wPetLoyaltyValue,m_wPetIndex,m_wUseSpecialAbilityStep,m_wPetReleaseCounter,m_wUseSpecialAbilityTime;
	WORD				m_wTamedMonsterCommand;	//
	DWORD				m_dwLastCommandTime;
	WORD				m_wIsPet,m_wIsSummonBeast,m_wIsReleaseSummonBeast;
	WORD				m_wTamer;
	DWORD				m_dwTamerNameHashCode;
	WORD				m_wSummonBeastType,m_wSummonBeastGrade;
	WORD				m_wObjectItemForCatch,m_wIsReturnToTransferItem;
	DWORD				m_dwObjectItemSerialForCatch;

	WORD				m_wGuildPetType,m_wGuildPetLevel;
	DWORD				m_dwLastActionTimeForGuildPet;
//┃조련한 몬스터 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃행동/프레임 업데이트 관련
	DWORD				m_dwBookedBasicAttackTime,m_dwBookedActionFinishTime;
	BOOL				m_isExclusiveAction;				//	독점행동-_-?
	WORD				m_wNonActionTime;					//	아무런 액션도 취하지 않은 시간.
	int					m_iFrameCounter;					//	프레임 카운터
	int					m_iAnm;
	int					m_iFrame;							//	현재 프레임
	int					m_iFPS;								//	초당 프레임수
	int					m_iFrameCount;						//	프레임 수
	WORD				m_wDirect;							//	현재 방향
	BOOL				m_isAction;							//	액션중이다.
	char				m_isInvincibleSkill;				//	사용하는 동안 무적이 되는 스킬
	WORD				m_wProhibitActionByEnchantedMagic;	//	액션 금지 시간
//┃행동/프레임 업데이트 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃이동 관련
	cPath				m_aPath[dUPDATE_MOVE_POS_PERIOD];
	cPOINT				m_posLastMoved;					//	마지막으로 움직였을때 위치
	WORD				m_wPathIndex;
	WORD				m_wPPS;							//	초당 이동 거리
	BOOL				m_bIsMoving;
	WORD				m_wBlockedStressedTime;
	WORD				m_wBlockedToMoveStress;
	WORD				m_wAngle;
	WORD				m_wStandTime;
	WORD				m_wIsChangedMoveMethod;
	WORD				m_wOverSpeedCounter;
	DWORD				m_dwLastTimeForReceiveMoveMessage;
	WORD				m_wLastClickObject,m_wCurrentClickObject;

//┃이동 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	inline	void	reset()
	{
		WORD Temp = m_wBoostSpeedByMagicCarpet;

		memset(this,0,sizeof(cACTOR_FIELD_DATA));

		m_wBoostSpeedByMagicCarpet = Temp;

		memset(m_aActiveReaction,0xff,sizeof(m_aActiveReaction));
		memset(m_aAuraSkillTimer,0xff,sizeof(m_aAuraSkillTimer));
		memset(m_aEnchantedMagic,0xff,sizeof(m_aEnchantedMagic));

		memset(m_awTamedMonsterList,0xff,sizeof(m_awTamedMonsterList));
		memset(m_awSummonBeastList,0xff,sizeof(m_awSummonBeastList));
		
		m_duelRecord.m_iBattleCount	=	-1;

		m_wCharacterSerial			=	0xffff;
		m_wZombieBombSkill			=	0xffff;
		m_wCorrectHP				=	100;
		m_wCorrectAP				=	100;
		m_wCorrectMonsterExp		=	100;
		m_wEventValue				=	0xffff;
		m_wDuelType					=	0xffff;
		m_wWeeklyDuelRank			=	0xffff;
		m_wDailyDuelRank			=	0xffff;
		m_wTrialGameRank			=	0xffff;
		m_wCorrectMiniPetGainExpByFeedItem	=	1;

		m_wLastClickObject			=	0xffff;
		m_wMirrorImageMasterNpcSerial=	0xffff;
		m_wProcessPatternLevel		=	0xffff;
		m_wProcessPatternStatus		=	0xffff;
		m_wTreasureBoxXPos			=	0xffff;
		m_wTreasureBoxYPos			=	0xffff;
		m_wNotificationOfDeathCounter=	0;
		m_wNotificationOfDeathCounterCaster	=	0xffff;
		m_wCorrectFinalDamage		=	10000;

		m_wMagicCarpetShape			=	1;
		m_wPremiumServiceLevel		=	ePS_FREE;	//	프리미엄 서비스 레벨

		m_wIllusionLord				=	0xffff;

		m_wMarionetteCaster			=	0xffff;
		m_wImpulseCaster			=	0xffff;
		m_wBloodDrainCaster			=	0xffff;
		m_wSelfReproofCaster		=	0xffff;
		m_wDegenerateCaster			=	0xffff;
		m_wTortureCaster			=	0xffff;
		m_wHwabyungCaster			=	0xffff;
		m_wPOS_Caster				=	0xffff;
		m_wPOS_Target				=	0xffff;
		m_wTerribleDreamCaster		=	0xffff;

		m_wPrincessWeaponPrincess	=	0xffff;

		m_wTamer					=	0xffff;
		m_wObjectItemForCatch		=	0xffff;
		m_dwObjectItemSerialForCatch=	0xffffffff;
		m_wPetIndex					=	0xffff;
		
		m_dwRequestPartyPlayerNameHashCode=	0xffff;
		m_wRequestGuild				=	0xffff;
		m_wRequestTradePlayer		=	0xffff;
		m_wRequestFriendSerial		=	0xffff;

		m_iZoneSerial				=	0xffffffff;
		m_iClientSerial				=	0xffffffff;
		m_iTargetZoneSerial			=	0xffffffff;
		m_wBiteTarget				=	0xffff;
		m_wBiter					=	0xffff;
		m_wCopyActorLord			=	0xffff;
		m_wDodgeAbility				=	0xffff;
		m_wSerialInField			=	0xffff;
		m_wLordActor				=	0xffff;
		m_wSlaveActor				=	0xffff;
		m_wRestraintTimeByLogout	=	0xffff;
		m_wMirrorTowerCaster		=	0xffff;
		m_wReceiveFieldActorData	=	0xffff;	//	전송 받은 필드 액터 데이터
		m_wReceiveFieldItemData		=	0xffff;	//	전송 받은 필드 아이템 데이터
		m_wNpcActorSaveSerial		=	0xffff;	//	캐릭터 번호
		m_wUseSkill					=	0xffff;
		m_wAttackToObject			=	0xffff;
		m_wAttackToObjectType		=	0xffff;
		m_wDamageTrapSerial			=	0xffff;	//	데미지를 입힌 함정의 시리얼
		m_wCastBlocker				=	0xffff;//	블록커
		m_wReceiveBlocker			=	0xffff;//	블록커
		m_wReceiveWeaponBlocker		=	0xffff;
		m_bf10CastWeaponBlocker		=	dVALID_BLOCKER_CODE;
		m_wDummyTime				=	0xffff;
		m_wCopyTime					=	0xffff;
		m_wTradeBoxSerial			=	0xffff;
		m_wLordOperator				=	0xffff;
		m_wHillSkill				=	0xffff;
		m_wIntervalShooter			=	0xffff;
		m_wBankSerial				=	0xffff;
		m_wCartSerial				=	0xffff;
		m_wProhibitActionByEnchantedMagic	=	0xffff;
		m_wPitchmanShopSize			=	dPITCHMAN_SHOP_MINIMUM_SIZE;
		m_wShopSerial				=	0xffff;
		m_wPitchmanShopSerial		=	0xffff;
		m_wItemSlotCount			=	dDEFAULT_ITEM_SLOT_COUNT;
		m_wDruggistTitleFactor		=	100;
		m_wCampingMasterTitleFactor	=	100;

		m_engageAbility.reset();
		m_bookedBattle.reset();
		m_bookedAction.reset();

		m_wTauntedToTargetSerial		=	0xffff;
		m_wBleedingCaster				=	0xffff;
		m_bitInfoManager.reset();
		m_dropInfoOfSummonEventMonster.reset();
	}

	inline	BOOL	isDeath()					{if (m_iDeathCount) return TRUE;return FALSE;}//죽은넘이냐?
};	//	class	cACTOR_FIELD_DATA

class	cACTOR;

//
//	참조 데이터
class	cACTOR_REFERENCE_DATA
{
public:
	cFIELD			*m_lpField;		//x	현재 이 액터가 속해있는 맵
	cACTOR			*m_lpTarget;	//타겟
	cACTOR			*m_lpCurrentTarget;
	cACTOR			*m_lpLastAttacker;

	inline	void	reset()
	{
		m_lpField			=	NULL;					//x	현재 이 액터가 속해있는 맵
		m_lpTarget			=	NULL;
		m_lpCurrentTarget	=	NULL;
		m_lpLastAttacker	=	NULL;
	}
};

class	cACTOR_DATA	:	public CPlayerData,public cACTOR_STATUS,public cACTOR_FIELD_DATA,public cACTOR_REFERENCE_DATA,public cFindOtherWayByBC
{
public:
	inline	cBODY*			getBody()
	{
		if	 (m_bf4Skin)
		{
			if	(m_bf4Skin+eNS_OPERATOR_MALE-1	==	eNS_OPERATOR_MALE)
				return	&g_aBody[dBODY_OPERATOR_ANGEL_MALE];
			if	(m_bf4Skin+eNS_OPERATOR_MALE-1	==	eNS_OPERATOR_FEMALE)
				return	&g_aBody[dBODY_OPERATOR_ANGEL_FEMALE];
		}

		if	(m_wJob >= dJOB_KELBY_1 && m_wJob <= dJOB_HEDGER_3)
			return	&g_aBody[dBODY_KELBY_1+m_wJob-dJOB_KELBY_1];

		if(m_wTransformationTime)
		{
			return 	&g_aBody[m_wTransformationSkin];
		}
		return	&g_aBody[m_wJob];
	}
	
	inline	int				getFPS(int _iAction)	{return	getBody()->getFPS(_iAction);}
	inline	void			setExclusiveAction(BOOL _is){m_isExclusiveAction=_is;}
	inline	void			setInvincibleSkill(BOOL _isValue){m_isInvincibleSkill=_isValue;}//사용하는 동안 무적이 되는 스킬(inline)
	inline	BOOL			isFinishAction(){return m_isAction;}//.. 액숀이 끝났냐?(inline)
	inline	BOOL			isTriggerFrame(){return getBody()->isTrigger(m_iAnm,m_iFrame);}//트리거 프레임 이냐?(inline)
	inline	int				getBodySize()
	{
		return getBody()->m_iCrashSize*m_wScaleX/100;
	}
	
	inline	cJOB			*getJob()
	{
		if (m_wJobIndex	>=	dMAX_JOB_COUNT	||	g_aJob[m_wJobIndex].m_iSerial	==	0xffff)
			return	NULL;

		return &g_aJob[m_wJobIndex];
	}//직업 얻어오기
};


//
//
enum
{
	eBLOCKING_FAILED				,	//	실패
	eBLOCKING						,	//	성공
	eBLOCKING_NO_ACTION				,	//	성공,액션이 없다.
	eBLOCKING_BY_DANCING_BLOCKER	,	//	성공,액션이 없다.
	eBLOCKING_IGNORE				,	// 블럭킹무시!
};	//	블럭킹 결과

class	cActor_miniPet;

class	cACTOR	:	public cACTOR_DATA
{
public:
	//JBC			거신의 은총.		08-10-14
	void					askItemListInItemPack(int _iItemIndex);
	void					setGGGValueForSkillUp();
	BOOL					operateGGG(CHitInfo* _lpHitInfo, cACTOR* _lpHitter);
	BOOL					operateGGG();
	void					operateGGGBattleEffect1(CHitInfo* _lpHitInfo, cACTOR* _lpHitter);
	void					operateGGGBattleEffect2(CHitInfo* _lpHitInfo , cACTOR* _lpHitter);
	void					operateGGGEffectForSkillUp();
	int						GetDecreaseByWaterGGGValue();
	void					addGGG(int _iGrace);
	void					sendGGGSkillUp(BOOL _isEndMessage = FALSE);
	void					sendGetGGG();
	void					SendRemoveGGG();
	void					SendGGGEffectPlay(cACTOR* _lpHitter , WORD _wType = dGGG_EFFECT_PLAYTYPE_NONE , int _iHealPoint = 0);
	BOOL					GetAddMagicDamage(CHitInfo* _lpHitInfo , cACTOR* _lpHitter , DWORD _dwDamge , int _attribute, DWORD* _dwEffectType);
	void					SendGGGInfo();

							cACTOR();

	BOOL					checkFreeTeleportOption();
	BOOL					checkRequestSummonCarpetOption();
	BOOL					checkFreePastPortalOption();
	BOOL					checkHalfPastPortalOption();

	BOOL					checkIncorrectInfo();
	void					setLastUseSkill(int _iSkill);
	int						getLastUseSkill();

	void					setLastUseItem(int _iItem);
	int						getLastUseItem();

	void					setBlockedToTransformationTime(int _iItem);
	int						getBlockedToTransformationTime();

	void					rebuildPlayerData(int _iLevel);	//	특정 레벨로 플레이어 데이터 재 설정
	BOOL					resetSkillPoint();				//	스킬 포인트 재분배
	BOOL					resetStatePoint();				//	스탯 포인트 재분배
	BOOL					resetOneStatePoint(int _iState);	//	스탯 재분이 필요 없는 상황이면 FALSE

	void					resetAllForDuel();
	void					resetOneState(int _iItem,int _iState);
	void					resetOneSkill(int _iItemSlot,int _iAbility);

	BOOL					rebirth(BOOL _bIsCompleteBankWork=FALSE, BOOL _bIsNotResetLevel = FALSE);	//	전생

	void					logout();
	void					bookingReadyToExitGame();
	void					bookingToExitGame();
	void					bookingToExitGameForMoveField();
	void					immediatelyExit(BOOL _bIsSleep=FALSE);
	
	BOOL					increaseExperience(int _dwExperience,BOOL _bIsSendUpdateExperience=TRUE,BOOL _bIsByKarma=FALSE,BOOL _bIsIgnoreBonusExp=FALSE,int _iQuestIndex=0);//	경험치를 확인해서 레벨업 시킨다.
	BOOL					_increaseExperience(int _iExperience, BOOL _bIsSend = TRUE);

	void					increasePetExperience(int _iExperience);
	BOOL					increaseState(int _iState,int _iValue=1);//	스탯치 증가

	void					updateAction();			//	액션 업데이트
	void					bookingActionFinishTime();

	void					updateNpc();			//	NPC 업데이트
	void					updateMonster();		//	몬스터 업데이트
	BOOL					isTiamath();

	void					operateOncePerSecondWorkForMonster();	//	1초에 한번 업데이트

	void					disconnect();

	void					actorDeath(int _iSerial);//	_iSerial인넘이 죽었다.

	void					kill(BOOL _bIsReleaseSummong);								//	죽-_-여
	void					bookingDeath(cACTOR *_lpKiller,BOOL _bIsRelease=FALSE,BOOL _bIsByKarma=FALSE);	//	죽기 예약
	void					bookingAddItem(cItem *_lpItem);
	void					closeTrade();

	cActor_miniPet*			getActor_miniPet()
	{
		return	(cActor_miniPet	*)this;
	}

	void					stop(BOOL _bIsRemoveActionSkill=TRUE);//	정지
	void					stopAction();//액션 정지
	void					blockingAction(int _iRigidityTime);//블럭킹 액션
	BOOL					toggleSitdown(int _iX,int _iY);//	앉기 토글
	void					changePos(int _iX,int _iY,BOOL _bIsInnerTeleport=FALSE);

	BOOL					updateStatusByFrameForMonster();
	void					RemoveInvincibleStateException(cAbility *_lpAbility);

	void					transformation(int _iX,int _iY,int _iJob,int _iSpentCP);	//	변신!!
	void					releaseTransformation();	//	변신 해제 -o-
	BOOL					callMagicCarpet(int _iShape=-1 , BOOL isByKarma = FALSE);
	void					rememberOrbLocation(int _iSlot);
	void					teleportToOrbLocation(int _iSlot);
	void					teleportByInregularMethod(int _iField,int _iPosX,int _iPosY);

	BOOL					readyToExitGame();
	void					readyToRemoveActorWork();
	BOOL					readyToMoveField(int _iDestField,int _iDestInstanceField,int _iDestInstanceFieldFloor);
	BOOL					openTreasureBox();
	void					selectAllignment(int _iItem,int _iValue);
	void					readyToDuel(int _iDuelField);	//	결투 준비

	BOOL					isUnionPartyMember();

	inline	BOOL			isMonster()
	{
		if	(m_wActorKind	==	eAK_MONSTER)
			return	TRUE;
		return	FALSE;
	}
	inline	BOOL			isNpc()
	{
		if	(m_wActorKind	==	eAK_NPC || (m_wActorKind > eAK_MONSTER && m_wActorKind < eAK_SUMMON_BEAST) )
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isPlayer()
	{
		if	(isDummy() || isCopyActor())
			return	FALSE;
		
		if	(m_wActorKind	==	eAK_PLAYER)	
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			getKind()
	{
		if	(m_wAnimatePartnerTime)
			return	eJT_UNDEAD;
		if	(isPlayer())	
			return	eJT_HUMAN;
		if	(isSummonBeast())	
			return	eJT_HOLY_BEAST;

		return	getJob()->m_wKind;
	}

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CActorPlayer.cpp
	LONGLONG				getOwnGoldSum();
	void					checkChangeGold();
	void					updatePlayer();//업데이트
	void					changeEncryptCode();	//	패킷 암호화 코드 변경
	void					operateUpdateByDayWork(BOOL _bIsRequireSendInfo=FALSE);
	void					operateOncePerSecondWorkForPlayer();	//	1초에 한번 업데이트
	void					operateOncePerMinuteWorkForPlayer();	//	
	void					operateOncePerHourWorkForPlayer();	//	

	BOOL					operateDummy();		//	더미 처리
	BOOL					operateCopyActor();
	void					syncPlayerBasicStatus(BOOL _bIsNow=FALSE);	//	플레이어의 기본 상태를 싱크 시켜준다.
	inline	void			resetPlayerBasicStatusSyncValue()		//	플레이어의 기본 상태 싱크를 위한 수치 초기화
	{
		if (m_wRestraintTimeByBattle)
			m_wRemainSyncTime	=	dSYNC_PLAYER_STATUS_PERIOD_IN_BATTLE;	//	5초에 한번씩
		else
			m_wRemainSyncTime	=	dSYNC_PLAYER_STATUS_PERIOD;				//	5초에 한번씩
	}
	inline	BOOL			updateStatusByFrameForPlayer();
	BOOL					addHostileEnemy(int _iSerial);
	void					updateHostileEnemy();
	void					updateLastActionTime();
	BOOL					isIdlePlayer();
	void					askBuyCarrotShopItem(int _iBuyCategory,int _iBuyItemIndex);
	void					buyCarrotShopItem(int _iPremiumItemIndex,int _iItemCount,int _iRemainCarrotCount,int _iPrice);
	void					setPeneltyTime(int _iTime,int _iMethod,int _iValue=0);
	int						enterToTheInstanceField(int _iIFSerial,int _iFloor,int _iX,int _iY,BOOL _bisUnCountSecretDungeonPlayTime = FALSE);
	BOOL					tryToSecretDungeon();
	void					leaveInstanceField(int _iGateField);
//┃CActorPlayer.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CActorPet.cpp
	void					operateGuildPetWork();

	int						addPet(int _iJob,int _iTamedLevel,int _iTamedLoyaloty,cACTOR *_lpTarget);
	int						addPet(cPetDefine *_lpPetInfo);
	void					revisePetJobIndex();
	void					revisePetInfo(cPetDefine *_lpPetInfo);

	void					removePet(int _iPetIndex);
	void					buildAddPetPower();
	void					buildByPetInfo(cACTOR *_lpLord,cPetDefine *_lpPetInfo,int _iPetIndex);

	int						getIncreasePetPowerPerLevel();
	int						getPetCount();
	int						getMaxPetCount();

	cACTOR*					getPetActor(int _iIndex);
	cPetDefine*				getPet(int _iIndex);
	BOOL					transBreedingRecordBook();
	BOOL					unsealPet(cItem *_lpItem);
	BOOL					boostPetPower(int _iBoostPower,int _iUpkeepTime,int _iLimitCount);
	void					changePetName(int _iIndex,char *_lpstrName,BOOL _bIsByClient,BOOL _bIsMiniPet,int _iUseItemIndex=-1);
	void					partingWithPet(int _iIndex,int _iTpye);
	void					updatePetInfo(cACTOR *_lpActor);

	int						getFollowMonsterCount();
	int						getFollowMonsterIndex(int _iSerial);
	inline	BOOL			isPet()	{return	m_wIsPet;}
//┃CActorPet.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CActorSummonBeast.cpp
	int						getSummonBeastCount();
	BOOL					isExistSummonBeast(int _iType);
	inline	BOOL			isSummonBeast()	{return	m_wIsSummonBeast;}
	void					downGradeSummonedBeast();

	void					buildSummonBeast(int _iLevel,int _iRemainHP,int _iType,int _iGrade,int _iTamer,DWORD _dwTamerNameHashCode);
	void					buildSummonBeastBasicPower(cACTOR *_lpTamer);
	int						addSummonBeast(cACTOR *_lpActor);
	void					removeSummonBeast(int _iIndex);
	void					updateSummonBeastInfo(cACTOR *_lpActor);
	cACTOR*					getSummonBeastActor(int _iIndex);
	void					deathSummonBeast(int _iIndex,int _iLevel);
	void					operateCommandSkill(int _iSkill,int _iLevel,cACTOR *_lpTarget,int _iX,int _iY);
	BOOL					callSummonBeast(int _iX,int _iY,int _iType,int _iLevel);	
	BOOL					powerUpSummonBeast(int _iGrade,int _iLevel);	
	BOOL					rideTamer(int _iUpkeepTime,int _iCorrectConcentration,int _iCorrectMoveSpeed);
	cACTOR*					getKelby();
	cACTOR*					getSummonBeastByType(int _iType);
	BOOL					getOffFromDog(BOOL _bIsWantSendToUser=TRUE);
	BOOL					getOffTamer();
	BOOL					catchItem(CDroppedItem *_lpItem);
	void					returnBeastItem();

//┃CActorSummonBeast.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CActorSlave.cpp
	void					setGuide(cACTOR *_lpActor);
	cACTOR*					getGuide();
	cACTOR*					getLord();
	cACTOR*					getMarionetteLord();
	cACTOR*					getCopyActorLord();
	cACTOR*					getCharmingActor();
	cACTOR*					getConfuseCaster();
	cACTOR*					getControlLord();

	void					setTraceGuide();
	void					cancelTraceGuide();

	void					updateSlave();
	void					setAiSlaveReady();
	void					setAiSlaveMove();
	void					setAiSlaveApproach(cACTOR *_lpTarget);
	BOOL					aiSlaveMove();
	BOOL					aiSlaveApproach();
	BOOL					aiSlaveFight();
//┃CActorSlave.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CActorTamedMonster.cpp
	cACTOR*					getTamer();
	BOOL					isTamer(cACTOR *_lpActor);
	void					syncTamedMonsterHP(BOOL _bIsNow=FALSE);

	BOOL					operateTamedMonsterReadyAi();
	BOOL					operateTamedMonsterTraceAi();
	BOOL					operateTamedMonsterPickItemAi();
	void					operateTamedMonsterReactionForUnderAttackLord(cACTOR *_lpAttacker);
	void					updateTamedMonster();

	void					commandTamedMonster(int _iCommand,cACTOR *_lpTarget=NULL,int _iX=-1,int _iY=-1);
	void					setPetUseSpecialAttack(int _iTime);

	void					setAiTamedMonsterTrace();
	void					checkTamedMonsterOutOfSight();
	BOOL					tamedMonsterMoveForTrace(int _iX,int _iY,int _iPPS);
	BOOL					traceTamer();
	
	void					setAiTamedMonsterReady();
	void					setAiTamedMonsterMove();
	void					setAiTamedMonsterFight(cACTOR *_lpTarget);
	void					setAiTamedMonsterApproach(cACTOR *_lpTarget);
	BOOL					aiTamedMonsterMove();
	BOOL					aiTamedMonsterApproach();
	BOOL					aiTamedMonsterFight();
//┃CActorTamedMonster.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CActorQuest.cpp
//┃퀘스트 관련

	BOOL					checkReceiveQuestExp(int _iExp,int _iQuest,int _iQuestArbeit);
	BOOL					setQuestTimer(int _iType,int _iQuestIndex,int _iBeginEnd,int _iTime,BOOL _bIsSendToPlayer);
	BOOL					setQuestCountdonwTimerStatus(int _iType,int _iQuestIndex,int _iStatus,BOOL _bIsSendToPlayer);	//	퀘스트 실패 설정
	BOOL					setQuestCompleteChecker(int _iType,int _iQuestIndex,BOOL _bIsComplete,BOOL _bIsSendToPlayer);	//	퀘스트 완료 체커 설정
	BOOL					setQuestComplete(int _iType,int _iQuestIndex,BOOL _bIsSendToPlayer ,BOOL _bIsPutComplateMessage = TRUE);	//	퀘스트를 완료한 상태로 만든다.
	BOOL					isCompleteQuest(int _iType,int _iQuestIndex);
	BOOL					isProcessAbleQuest(int _iType,int _iQuestIndex);
	BOOL					isCheckedQuestCompleteChecker(int _iType,int _iQuestIndex);

	int						getQuestCountdownTimerStatus(int _iType,int _iQuestIndex);
	BOOL					addProcessQuest(int _iQuestType,int _iQuestIndex,BOOL _bIsSendToPlayer);	//	진행중인 퀘스트에 추가 시킨다.
	BOOL					setQuestProcessLevel(int _iQuestType,int _iQuestIndex,int _iStatus,BOOL _bIsSendToPlayer);	//	퀘스트 상태를 설정한다.
	BOOL					setQuestValue(int _iQuestType,int _iQuestIndex,int _iValue,BOOL _bIsSendToPlayer);
	BOOL					cancelQuest(int _iQuestType,int _iQuestIndex,BOOL _bIsSendToPlayer,BOOL _bIsForQuestComplete=FALSE, BOOL _bIsSendStatistics = TRUE);		//	퀘스트를 취소한다.
	BOOL					cancelQuestByClient(int _iQuestSlot,int _iQuestIndex);
	BOOL					controlQuestValue(int _iQuestType,int _iQuestIndex,int _iMethod,int _iValue,BOOL _bIsSendToPlayer,int _iLimitValue);

	BOOL					isOwnQuest(int _iQuestType,int _iQuestIndex);
	int						getProcessQuestCount(int _iQuestKind=0);
	int						getQuestProcessLevel(int _iQuestType,int _iQuestIndex);
	int						getQuestValue(int _iQuestType,int _iQuestIndex);
	tsProcessQuestField*	getProcessQuest(int _iQuestType,int _iQuestIndex);

	CQuestSimpleInfo*		getQuestSimpleInfo(int _iSlot);

	void					setChapter(int _iChapter,int _iHighQuest);
	void					cancelChapter();

	int						getMainQuestValue(int _iValueIndex);
	void					setMainQuestValue(int _iValueIndex,int _iValue);
	void					addMainQuestValue(int _iValueIndex,int _iValue);

	int						getSwitchStatus(int _iSwitchIndex);
	void					setMainQuestSwitch(int _iSwitchIndex,BOOL _bIsOn);
	void					resetMainQuestValue();
	void					resetMainQuestSwitch();
//┃퀘스트 관련
//┃CActorQuest.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃길드 관련
//┃CActorGuild.cpp
	void					operateTransToGVGResult(WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT	*_lpPacket);
	void					operateTransGuildToGVGResult(WORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT	*_lpPacket);
	void					transAvatarToGVGServer();
	void					transGuildToGVGServer();
	BOOL					resetGuildSkillPoint(BOOL _bIsByCh5=FALSE);
	BOOL					unsummonGuildGuardian();
	BOOL					summonGuildGuardian(int _iType);
	BOOL					addRelatedPlace(int _iField);
	void					askToggleOpenguildHallStatus();
	int						askActiveGuildMasterGraceEffect();
	void					feedItemToGuildPet(int _iType,int _iItemSlot);
	void					storeGuildInventoryItem(int _iInventorySlot,int _iGuildInventorySlot);
	void					withdrawGuildInventoryItem(int _iInventorySlot,int _iGuildInventorySlot,BOOL _bIsByOCM = FALSE ,CGuild* _lpGuild = NULL);
	void					storeGuildInventoryGold(int _iGold);
	void					withdrawGuildInventoryGold(int _iGold);
	void					changeGuildInventoryItemPlace(int _iSlot1,int _iSlot2);

	void					openFeedGuildPetAndGuardianWindow();
	BOOL					openGuildShop();
	BOOL					openRelatedPlaceListOfGuild();
	BOOL					openGuildHonorPointWindow(int _iLevel);
	void					openGuildInventory();
	void					openGuildHallList();
	BOOL					warpToGuildHall(int _iHallIndex=-1);
	void					warpToRelatedPlaceOfGuild(int _iHallIndex);
	void					tryToGuildPointBattle();
	void					tryToGuildDungeon(int _iPlayLevel);
	void					returnToGuildHallByEndGuildPointBattle();
	void					returnToGuildHallByEndBattle();

	void					askRentGuildPet(int _iPetType,char *_lpstrMemberName);//	길드 펫 대여 요청
	void					askRetrievalRentedGuildPet(char *_lpstrMemberName);	//	길드 펫 반환

	void					increaseGuildSkillLevel(int _iSkill,BOOL _bIsStatueSkill=FALSE);	//	길드 스킬 레벨 중가
	BOOL					readyToMakeGuild();	//	길드 만들기 인터페이스 열기
	BOOL					openGuildBattleScheduler();	//	길드 스케쥴러 오픈
	BOOL					askOpenGuildMarkComposer();	//	길드 콤포져 인터페이스 오픈 요청
	BOOL					openGuildMarkComposer();	//	길드 콤포져 인터페이스 오픈
	void					createGuildFirstWork(WORD _wGuildSerial,char *_lpstrSubGuildMasterName,char *_lpstrSubMasterName);
	inline	BOOL			isGuildLeader()
	{
		if	(!isGuildMember())
			return	FALSE;
		if	(m_wGuildRank	==	dGUILD_CLASS_MASTER)
			return	TRUE;
		if	(m_wGuildRank	==	dGUILD_CLASS_SUBMASTER)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isGuildMaster()
	{
		if	(!isGuildMember())
			return	FALSE;
		if	(m_wGuildRank	==	dGUILD_CLASS_MASTER)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isGuildMember()
	{
		if (m_wGuildSerial	!=	0xffff)	return	TRUE;

		return	FALSE;
	}
	CGuild*					getGuild();
	BOOL					retryToGuildDungeon();

//┃CActorGuild.cpp
//┃길드 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃길드 스킬 관련
//┃CActorGuildSkill.cpp
	void					operateGuildSkillData();
	void					operateGuildSkillData(int _iSkill,int _iLevel,int _iEffectLimitCount=dMAX_GUILD_SKILL_EFFECT_COUNT);

	void					operateGSIncreaseHP(int _iSkill,int _iLevel);
	void					operateGSIncreaseCP(int _iSkill,int _iLevel);
	void					operateGSIncreaseHitChance(int _iSkill,int _iLevel);
	void					operateGSIncreaseDodgeChance(int _iSkill,int _iLevel);
	void					operateGSIncreaseStrength(int _iSkill,int _iLevel);
	void					operateGSIncreaseAgility(int _iSkill,int _iLevel);
	void					operateGSIncreaseConstitution(int _iSkill,int _iLevel);
	void					operateGSIncreaseIntelligence(int _iSkill,int _iLevel);
	void					operateGSIncreaseWisdom(int _iSkill,int _iLevel);
	void					operateGSIncreaseCharisma(int _iSkill,int _iLevel);
	void					operateGSIncreaseLuck(int _iSkill,int _iLevel);
	void					operateGSIncreaseJobSkillLevel(int _iSkill,int _iLevel,int _iJob);

	void					operateGSAbsorbMagicDamagePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect);
	void					operateGSIncreaseMagicResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect);

	void					operateGSIncreaseCrushResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect);
	void					operateGSIncreaseCurseResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect);
	void					operateGSIncreaseStrangeStatusResistancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect);
	void					operateGSIncreaseDeclinePowerResitancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect);

	void					operateGSIncreasePhysicalMagicDamagePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect);
	void					operateGSIncreaseDefensivePowerPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect);

	void					operateGSIncreaseActionSpeedPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect);
	void					operateGSIncreaseMoveSpeedPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect);

	void					operateGSIncreaseHitChancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect);
	void					operateGSIncreaseDodgeChancePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo *_lpEffect);
//┃CActorGuildSkill.cpp
//┃길드 스킬 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃은행 관련
//┃CActorBank.cpp
	BOOL					askOpenBank();
	void					sendBankMessage(int _iMessage);
	BOOL					openBank(CBankInfo *_lpBank);
	BOOL					operateRebirthBankWork(CBankInfo *_lpBank);

	BOOL					storeItemToTheBank(int _iBankSerial,int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum);
	BOOL					storeGoldToTheBank(int _iBankSerial,int _iGold,DWORD _dwCheckSum);
	BOOL					withdrawItemFromTheBank(int _iBankSerial,int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum);
	BOOL					withdrawGoldFromTheBank(int _iBankSerial,int _iGold,DWORD _dwCheckSum);
	BOOL					moveBankItem(int _iBankSerial,int _iPos1,int _iPos2,DWORD _dwCheckSum);
	BOOL					moveInventoryItemWhenBankTransaction(int _iBankSerial,int _iPos1,int _iPos2,DWORD _dwCheckSum);
	BOOL					cancelBankTransaction(BOOL _bIsSendCancelMessageToClient=TRUE);	//	은행 거래 취소
	BOOL					askFinishBankTransaction(int _iBankSerial,DWORD _dwCheckSum);	//	거래 종료
	BOOL					finishBankTransaction();	//	거래 종료
	BOOL					closeBank();

	inline	BOOL			isBankTransaction()	{if (m_wBankSerial == 0xffff) return FALSE;return TRUE;}
//┃CActorBank.cpp
//┃은행 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃파티 관련
//┃CActorParty.cpp
	void					leaveParty();	//	파티에서 나왔다.
	void					leavePartyByLogout();
	void					joinParty(int _iPartyIndex);	//	파티에 들어갔다.
	void					checkPartyInfo();
	BOOL					isPartyLeader();	//	파티 리더냐?
	cParty*					getParty();			//	소속 파티 구하기
	BOOL					isPartyMember(cACTOR *_lpActor);
	int						operateDamagePool(int _iDamage,cACTOR *_lpAttacker);
	void					sendJoinParty(int _iAskerLevel,int _iAskerJob,char *_lpstrAskerName,BOOL _bIsJoinPartyApplication=FALSE, BOOL _bIsGuildMemberJoinParty = FALSE);
	void					sendUnionJoinParty(cACTOR* _lpAsker);		//연합 파티 초대
	BOOL					acceptJoinPartyApplication(char *_lpstrAsker,int _iAnswer);
	BOOL					receivePermitJoinPartyApplication(char *_lpstrPartyName,int _iPartySerial);
//┃CActorParty.cpp
//┃파티 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃패턴 관련
//┃actor_pattern.cpp
	BOOL					operatePattern(int _iPattern);
	void					operateNextStepPattern();
	void					processPattern();

	void					pcp_move(cCharacterPatternPiece	*_lpPP);
	void					pcp_delay_time(cCharacterPatternPiece	*_lpPP);
	void					pcp_use_action(cCharacterPatternPiece	*_lpPP);
	void					pcp_use_skill(cCharacterPatternPiece	*_lpPP);
	void					pcp_use_local_skill(cCharacterPatternPiece	*_lpPP);
	void					pcp_set_invicible(cCharacterPatternPiece	*_lpPP);
	void					pcp_call_pattern(cCharacterPatternPiece	*_lpPP);
//┃actor_pattern.cpp
//┃패턴 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃쇼핑카트 관련
//┃CActorCart.cpp
	BOOL					askOpenCart();
	BOOL					askWithdrawItemFromTheCart(int _iCartSerial,int _iCartSlot,int _iInventorySlot,DWORD _dwCheckSum);
	BOOL					askDestroyCartItem(int _iCartSerial,int _iCartSlot,DWORD _dwCheckSum);
	BOOL					openCart(tsBuyLst *_lpCartInfo,int _iItemCount);
	BOOL					closeCart();

	BOOL					withdrawCartItem(int _iCartSlot,int _iInventorySlot);
	BOOL					destroyCartItem(int _iCartSlot);
	BOOL					cancelCart();
	BOOL					failedCart();
//┃CActorCart.cpp
//┃쇼핑카트 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃노점 관련
//┃CActorPitchmanShop.cpp
	int						openPitchmanShop();	//	노점상 개설
	int						addPitchmanShopItem(int _iShopSerial,int _iInventorySlot,int _iShopSlot,DWORD _dwPrice, BYTE _bDenomination); //09.08.21
	int						removePitchmanShopItem(int _iShopSerial,int _iShopSlot);
	int						changePitchmanShopInfo(int _iShopSerial,int _iStatus,char *_lpstrName,int _iSignboardShape,WORD _wTextColor,BOOL _bIsBold,BOOL _bIsShadowText);
	int						changePitchmanShopPlace(int _iShopSerial,int _iShopSlot1,int _iShopSlot2);
	int						buyPitchmanShopItem(int _iShopSerial,int _iShopSlot,cItem *_lpItem,DWORD _dwPrice, BYTE _bDenomination); //09.08.21
	int						sendSoldPitchmanShopItem(char *_lpstrName,int _iShopSlot,DWORD _dwPrice, BYTE _bDenomination);			 //09.08.21
	int						closePitchmanShop(int _iShopSerial);
	int						askPitchmanShopInfo(int _iShopSerial);
	CPitchmanShop*			getPitchmanShop();
	int						closePitchmanShop();
//┃CActorPitchmanShop.cpp
//┃노점 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃오라 관련
//┃CActorAura.cpp
	void					operateInstanceApplyAura();
	void					operateAuraSkill();	//	오러 스킬 처리
	void					operateMonsterAuraSkill();
	BOOL					getActiveAura(cAbility *_lpAbility,int _iIndex);
//┃오라 관련
//┃CActorAura.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃스킬 관련
//┃CActorSkill.cpp
	int						getMasterSkillCount();	//	마스터한 스킬의 수
	cAbility*				getPigeonPostItemSkill();
	cAbility*				getGlareSkill(BOOL _bIsInReaction=FALSE);
	cAbility*				getRidingDogSkill();

	void					hitThrowDrug(cItem *_lpItem,cAbility *_lpAbility,int _iHealHP);
	int						hitThrowPotion(cItem *_lpItem,cAbility *_lpAbility,int _iEfficient);

	void					throwDrug(cACTOR *_lpTarget,cAbility *_lpAbility,cItem *_lpItem,int _iHitRange,int _iHealHP);
	void					throwPotion(cACTOR *_lpTarget,cAbility *_lpAbility,cItem *_lpItem,int _iHitRange,int _iEfficient);
	void					throwCandy(cACTOR *_lpTarget,cAbility *_lpAbility,int _iHitRange);
	void					operateFatGirlStatus();
	void					operateAttackDoppelganger();

	inline	cAbility*		getAbility(int _iAbility)
	{
		if	(_iAbility	>=	dABILITY_COUNT)
			return	NULL;

		return	&m_aAbility[_iAbility];
	}
	inline	cAbility*		getMonsterAbility(int _iAbility)		// 09.08.13
	{
		for(int i=0;i<dMAX_MONSTER_ABILITY_COUNT; ++i){

			cAbility	*lpAbility	=	&m_aAbility[i];

			if(lpAbility->m_wSkill == _iAbility)
			{
				return &m_aAbility[i];
			}


		}
		return	NULL;
	}
	BOOL					getCompleteProtectionAbility(cAbility *_lpAbility);			//	활성화된 블럭킹 스킬을 찾아라!!

	BOOL					checkStatusForUseSkill(CSkill::uCheckStatus	*_lpStatus);
	void					setCoolTime(cAbility *_lpUseAbility);
	int						getCoolTime(cAbility *_lpUseAbility);
	int						isUseAbleSkill(cAbility *_lpAbility);//	사용가능한 스킬이냐?
	BOOL					isCastAbleTarget(cACTOR *_lpTarget,cAbility *_lpAbility);
	cAbility				*getAbilityBySkill(int _iSkill);//_iSkill인 어빌러티를 찾는다.
	int						getAbilityIndexBySkill(int _iSkill);//_iSkill인 어빌러티를 찾는다.
	BOOL					increaseAbilityLevel(int _iAbility);//	어빌러티 레벨 증가

	BOOL					strikeWhirlWind(cAbility *_lpAbility);
	BOOL					strikeLinerChainExplosion(int _iX,int _iY,cAbility *_lpAbility,WORD *_lpDamagedActorList);
	BOOL					strikePiercingMissile(cAbility *_lpAbility,cACTOR *_lpTarget,WORD *_lpDamagedActor=NULL);
	BOOL					strikeLaser(cAbility *_lpAbility,cPOINT *_lpX1Y1,cPOINT *_lpX2Y1,cPOINT *_lpX1Y2,cPOINT *_lpX2Y2);
	int						getSkillPointSum();//	스킬 포인트 합
	void					resetSkillLevel();//	스킬 레벨 초기화
	int						getSkillPointSumByLevel();//레벨에 의한 스킬 포인트의 합

	void					reactionAllOff();		// 리액션스킬 모두 끄기..
	int						getReactionLimitCount();		// 리액션발동수제한 개수.
	int						getPartyReactionLimitCount(); // 파티 리액션발동수제한스킬 개수...
	int						getReactionCount();			// 켜진 리액션 개수 ..
	BOOL					isReactionLimit(int _iAbility);	// 리액션발동수제한스킬인가..?
	int						reactionOn(int _iAbility);	//	리액션 켜기	// 수정.
	BOOL					reactionOff(int _iAbility);	//	리액션 끄기
	BOOL					isActiveReaction(int _iAbility);//	리액션 상태

	inline	BOOL			isActiveSpecialSkill(int _iField)	//	컴플리트 프로텍션이 발동중이냐?
	{
		if (m_wSpecialSkillField & _iField)	return	TRUE;

		return	FALSE;
	}
	inline	void			turnOnSpecialSkillField(int _iField)	{m_wSpecialSkillField	|=	_iField;}
	inline	void			turnOffSpecialSkillField(int _iField)	{if (m_wSpecialSkillField&_iField)	m_wSpecialSkillField-=_iField;}
	inline	void			toggleSpecialSkillField(int _iField)
	{
		if (m_wSpecialSkillField&_iField)	m_wSpecialSkillField-=_iField;
		else								m_wSpecialSkillField+=_iField;
	}

	void					releaseCompleteProtection();	//	컴플리트 프로텍션이 설정되어 있다면 해제 시킨다.
	inline	BOOL			isActiveCompleteProtection(){return	isActiveSpecialSkill(dSSF_COMPLETE_PROTECTION);}
	void					releaseCastShimmeringShield();	//	시전한 시머링 실드 끝내기
	void					releaseReceiveShimmeringShield();	//	시전 받은 시머링 실드 해제
	void					releaseShimmeringShield();	//	시머링 실드 전부 해제
	BOOL					isShimmeringShieldCaster();	//	시머링 실드를 사용중이냐?

	void					releaseAllPassiveSkill();	//	모든 패시브 스킬 해제

	void					floatMachine(int _iSkillSerial,int _iSkill);	//	머신을 띄우고.. 메시지도 전송
	void					shootMachineMissile(int _iTarget,int _iSkill,CHitInfo *_lpHitInfo);//머신 미사일 발사
	void					releaseMachine();	//	머신 제거

	void					castIcyStalagmite(cAbility *_lpAbility,int _iStatus=-1);
	void					releaseIcyStalagmite(int _iStatus);

	void					setIntervalShooter(int _iSkill);
	void					releaseIntervalShooter();

	BOOL					isVisibleObject(int _iSecretLevel);
	BOOL					isVisibleTrap(int _iDetectTrapLevel);

	cAbility*				getSpecialFeatureSkill(int _iFeature);

	BOOL					isVisibleDoor(cAreaDoorInfo *_lpDoor);
	BOOL					isVisibleDoorTrap(cAreaDoorInfo *_lpDoor);

	BOOL					isDisarmAbleTrap(int _iTrapLevel);
	cAbility*				getDisarmTrapAbility();

	BOOL					isUnlockAbleArcaBySkill(int _iLockLevel);
	BOOL					isUnlockAbleDoorBySkill(int _iLockLevel);
	cAbility*				getUnlockDoorSkill();
	cAbility*				getUnlockArcaSkill();

	BOOL					isUnlockAbleDoorByKey(int _iLockLevel);
	BOOL					isUnlockAbleArcaByKey(int _iLockLevel);

	int						getDefaultAttackAbilityIndex();

	int						getUnlockArcaKeySlot(int _iLockLevel);
	int						getUnlockDoorKeySlot(int _iLockLevel);

	cItem*					getUnlockArcaKey(int _iLockLevel);
	cItem*					getUnlockDoorKey(int _iLockLevel);
	BOOL				isUseSkillState(CSkill* _lpSkill);	// 스킬 사용 가능 상태체크.
	void					cancelNeedState(CSkill* _lpSkill);	// 필요상태 해제.
//┃CActorSkill.cpp
//┃스킬 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기타 효과
//┃CActorSkillExtraEffect.cpp
	BOOL					applySkillExtraEffects(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iHitDamage=0,int _iX=0,int _iY=0,BOOL _bIsByAura=FALSE, BOOL _bIsByKarma=FALSE, DWORD _dwKeepTime = 0);		// 09.10.12
	BOOL					applySkillExtraEffect(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iHitDamage,CSkillExtraEffectInfo *_lpEffect,DWORD *_lpUpkeepTime,int _iX,int _iY,BOOL _bIsByAura, BOOL _bCompulsionTime = FALSE);//// 09.10.12	스킬 효과를 적용 시킨다.
	int						applySkillExtraEffectMagicDamage(cACTOR *_lpAttacker,cAbility *_lpAbility , int _iDamage, int _iClass); // 보조효과데미지 증가 계산. 09.09.29
	void					operateAttackMagicShield(cAbility *_lpAbility);
	BOOL					operateDestroyingUndead(cACTOR *_lpAttacker,cAbility *_lpAbility);
	BOOL					operateDestroyingUnholy(cACTOR *_lpAttacker,cAbility *_lpAbility);
	BOOL					thrust(cACTOR *_lpTarget,BOOL _bIsSendPosInfo=FALSE);
	BOOL					thrust(int _iAxisX,int _iAxisY,int _iDistance,BOOL _bIsSendPosInfo=FALSE,int _iAngle=0,BOOL _bIsIgnoreResistance=FALSE);	//	밀려나기.. -o-;;
	BOOL					thrustByAngle(int _iAngle,int _iDistance,BOOL _bIsSendPosInfo=FALSE, BOOL _bIsChangeTargetDirect = FALSE);	//	밀려나기.. -o-;; 수정.
	BOOL					dodgeAction(cACTOR *_lpAttacker,int _iSpentCP);	//	회피 액숀!!
	BOOL					operateAdditionalDodgeEffect(cAbility *_lpAbility);	//	추가 회피 효과 처리
	BOOL					readyWarpNearestVillage(int _iLimitRange,int _iLimitVillageCount,BOOL _bIsCreatePortal,int _iSkill,int _iPortalTime=0);
	BOOL					calling(int _iLimitRange,int _iSkill);
	BOOL					rally(int _iX,int _iY,int _iTime);	
	BOOL					dance(int _iTime);
	BOOL					bounce(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iRange,int _iDamage);
	BOOL					feignDeath(cAbility *_lpAbility,int _iUpkeepTime);
	BOOL					explosionBottle(cACTOR *_lpCaster,cAbility *_lpAbility,int _iEffecient);
	BOOL					jumppingSkipping(cAbility *_lpAbility,int _iUpkeepTime);
	BOOL					trickKick(cAbility *_lpAbility,int _iNotRevengeChance);
	BOOL					transToWeapon(int _iUpkeepTime);
	BOOL					transToFrog(cACTOR *_lpCaster,cAbility *_lpAbility,int _iUpkeepTime);
	BOOL					transToFatGirl(cACTOR *_lpCaster,cAbility *_lpAbility,int _iUpkeepTime,int _iActivePeriod);
	BOOL					transToWeapon(cACTOR *_lpCaster,cAbility *_lpAbility,int _iUpkeepTime,int _iMagicDamageFactor,int _iDamagePercentageFactor);
	BOOL					transToRabbit(cAbility *_lpAbility,int _iUpkeepTime);
	void					releasePrincessWeaponStatus(BOOL _bIsWantCheckReceiverPos=FALSE);
	BOOL					shutInMagicBox(cACTOR *_lpCaster,int _iUpkeepTime,int _iKillChance,int _iLimitCount);
	BOOL					copyActor(cACTOR *_lpAttacker,int _iRemainHP,int _iUpkeepTime,int _iLimitCount);	
	BOOL					releaseDance();
	BOOL					makeTheOtherSelf(int _iTime,int _iCount);
	BOOL					tryDisarmTrap(int _iTargetType,int _iObjectSerial,int _iDisarmLevel,int _iExplosionChance,int _iUpkeepTime,BOOL _bIsSuccess);

	BOOL					setImpulse(cACTOR *_lpCaster,int _iTime,int _iDamage,int _iSkill);
	BOOL					setBloodDrain(cACTOR *_lpCaster,int _iTime,int _iPercentage,int _iSkill);
	BOOL					setSelfReproof(cACTOR *_lpCaster,int _iTime,int _iDamage,int _iSkill);
	BOOL					setTorture(cACTOR *_lpCaster,int _iTime,int _iDamage);
	BOOL					setDegenerate(cACTOR *_lpCaster,int _iTime,int _iDamage,int _iSkill);
	BOOL					setTerribleDream(cACTOR *_lpCaster,int _iTime,int _iDamage,int _iSkill,int _iPeriod);
	BOOL					setHwabyung(cACTOR *_lpCaster,int _iTime,int _iFireDamage,int _iDarkDamage,int _iSkill);
	BOOL					setPlotOfShadow(cACTOR *_lpCaster,int _iTime,int _iDamage,int _iCasterDamage,int _iSkill,int _iLimitTargetCount);
	BOOL					setSmellOfDeath(int _iTime,int _iDarkDamage,int _iReduceResistanceUpkeepTime,int _iReduceResistance,int _iSkill,int _iReduceCurseResistance);

	BOOL					operatePhantomImpulse(cACTOR *_lpCaster,int _iPropertion,int _iSkill);
	BOOL					declineRecoverHPEfficient(int _iTime,int _iEfficient);
	BOOL					setInterrutingRevive(int _iTime);
	BOOL					setSoulOath(int _iUpkeepTime,int _iGetCP,int _iSkill);
	BOOL					setBloodCompact(int _iUpkeepTime,int _iGetHP,int _iSkill);
	BOOL					setInterruptingArmor(int _iCurrentLevel,int _iTime);
	void					correctValueEffectDuelServer(int* _iValue , CSkill* _lpSkill);
	void					operateInhaleEnemy();
	BOOL					setInhaleEnemy(int _iRange,int _iPeriod,int _iPixel,int _iTime);
	BOOL					operateBloodyCross(cAbility *_lpAbility);
	BOOL					setContract(cACTOR *_lpAttacker,int _iUpkeepTime,int _iOccurPeriod,int _iRecoverHP,int _iSkill,int _iDamageSkill,int _iDamageSkillLevel);
	BOOL					operateSwallow(cACTOR *_lpAttacker);

	BOOL					makeMarionette(cACTOR *_lpCaster,int _iTime,int _iBoostPower);
	void					releaseMarionette();

	BOOL					appointmentMakeIllusion(int _iSkill,int _iUpkeepTime,int _iRemainHP,BOOL _bIsFirstTarget,int _iIllusionJob,int _iLimitIllusionCount,int _iX,int _iY);	//	환영 생성
	BOOL					stealGold(cACTOR *_lpTarget);
	BOOL					stealItem(cACTOR *_lpTarget);
	BOOL					makeShakle(int	_iHP,int	_iDP,cACTOR *_lpCaster,cAbility *_lpAbility,int _iUpkeepTime,int iIsFreeze,int iIsCritical,int iIsSyncDamage,int _iMagicResistance);
	void					strikeShakleSkillDamage();
	void					releaseShakle();
	BOOL					isAttackToShakleActor(cACTOR *_lpAttacker);
	void					setInterruptingWeapon(WORD _iTime);
	void					addBit(WORD _wSerialInField, DWORD _dwNameHashCode, int _iCount, int iTime);
	void					removeBit(WORD _wSerialInField, DWORD _dwNameHashCode, int _iCount);

//┃CActorSkillExtraEffect.cpp
//┃기타 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃패시브 스킬 관련
//┃CActorPassiveSkill.cpp
	BOOL					getActiveAdvancedBlockingSkill(cAbility *_lpAbility);	//	블럭킹 향상에 의한 블럭률
	BOOL					getDefenseNockoutPassiveSkill(cAbility *_lpAbility);	//	녹아웃 방어하는 패시브 스킬 찾기
	BOOL					getDefenseStunPassiveSkill(cAbility *_lpAbility);		//	스턴을 방어하는 패시브 스킬 찾기
	BOOL					getDefenseStrangeStatusPassiveSkill(cAbility *_lpAbility);	//	이상상태를 방어하는 패시브 스킬 찾기
	BOOL					getActivatePassiveSkill(cAbility *_lpAbility,int _iActivateTrigger,int _iIndex,BOOL _bIsIgnoreCP=FALSE,cACTOR *_lpAttacker=NULL);	//	발동 가능 패시브 스킬 찾기
	BOOL					getMonsterActivatePassiveSkill(cAbility *_lpAbility,int _iActivateTrigger,int _iIndex,cACTOR *_lpAttacker);	//	발동 가능 패시브 스킬 찾기

	int						getBlockingChanceByAdvancedBlocking();	//	블럭킹 향상에 의한 블럭률
	int						getBlockingChanceByDancingBlocker();	//	댄싱 블럭커에 의한 블럭률
	cAbility*				getDancingBlockerSkill();
	
	int						getStrangeResistanceByPassiveSkill();	//	패시브 스킬에 의한 이상상태 저항
	int						getStunResistanceByPassiveSkill();		//	패시브 스킬에 의한 스턴 저항
	int						getNockOutResistanceByPassiveSkill();	//	패시브 스킬에 의한 녹백 저항
	int						getDefensePercentageByPassiveSkill();	//	패시브 스킬에 의한 방어력 퍼센티지 증가 수치
	int						getDefensePowerByPassiveSkill();		//	패시브 스킬에 의한 방어력 증가 수치
	int						getDodgeChanceByPassiveSkill();			//	패시브 스킬에 의한 회피율

	BOOL					isActiveDancingBlocker();				//	댄싱 블럭커가 활성화 되었냐?

	int						getValuePassiveSkill(int _iKind, int _iValueIndex = 1);	// 패시브스킬의 값을 얻는다.
	void					operatePassiveSkill();		// 패시브스킬들저장..
//┃CActorPassiveSkill.cpp
//┃패시브 스킬 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃리액션 스킬 관련
//┃CActorReactionSkill.cpp
	BOOL					getActiveTailChaser(cAbility *_lpAbility);				//	활성화된 테일 체이서를 찾아라!!
	BOOL					getActiveBlockingSkill(cAbility *_lpAbility);			//	활성화된 블럭킹 스킬을 찾아라!!
	BOOL					getActiveDodgeSkill(cAbility *_lpAbility,int _iCount);	//	활성화된 닫지 스킬
	BOOL					getActiveSwingArising(cAbility *_lpAbility);			//	활성화된 돌려 일어나기
	BOOL					getActiveArmorDisassemler(cAbility *_lpAbility);		//	활성화된 아머 디스어셈블러
	BOOL					getActiveCounterSkill(cAbility *_lpAbility,int _iIndex);//	활성화된 카운터 스킬
	BOOL					getDefenseNockoutReactionSkill(cAbility *_lpAbility);	//	녹아웃 방어하는 스킬 찾기
	BOOL					getDefenseStunReactionSkill(cAbility *_lpAbility);		//	스턴을 방어하는 리액션 스킬 찾기
	BOOL					getTauntReactionSkill(cAbility *_lpAbility);			//	타운트 하는 리액션 스킬 찾기

	BOOL					getActivateReaction(cAbility *_lpAbility,int _iActivateTrigger,int _iIndex,cACTOR *_lpAttacker);
	BOOL					getMonsterActivateReaction(cAbility *_lpAbility,int _iActivateTrigger,int _iIndex,cACTOR *_lpAttacker);

	void					fireTailChaser(cACTOR *_lpTarget,int _iSkill,int _iCount,int _iPeriod,int _iDamage,int _iCorrectDamage);
	void					fireCounterSwingArising(cAbility *_lpAbility);			//	카운터로 스윙어라이징 발동?
	void					fireCounterArmorDisassembler(cACTOR *_lpTarget,cAbility *_lpAbility);	//	카운터로 아머 디스어셈블러 발동
	void					fireCounterReactionSkill(cACTOR *_lpTarget,cAbility *_lpAbility);

	void					operateReactionSkill(int _iTiming,cACTOR *_lpTarget,int *_lpiLimitPhisycalDamage,int *_lpiLimitMagicDamage);	//	리액션 스킬 처리
	void					operateReactionSkill(cACTOR *_lpTarget,cAbility *_lpSkill);	//	리액션 스킬 처리
//┃CActorReactionSkill.cpp
//┃리액션 스킬 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃아이템 처리
//┃CActorItem.cpp
	void					resetAllItem();

	BOOL					checkItemCoolTime(cBasicItem *_lpBasicItem);

	cItem*					getFirstPotion(cAbility *_lpAbility);
	cItem*					getFirstDrug(cACTOR *_lpTarget=NULL);
	cItem*					getFirstFlower();
	cItem*					getFirstCandy();
	cNameStone*				getNameStone();
	int						getOperatorShapeByBook();
	void					releaseItemReversion(int _iUseItem,int _iTargetItem);
	void					levelUpPrefix(int _iUseItem,int _iTargetItem,int _iSlot,BOOL _bIsUnique);
	void					upgradeItem(int _iUseItem,int _iTargetItem,int _iSlot,BOOL _bIsUnique);
	void					disJointingItem(int _iTargetItem);
	int						copyItem(cItem *_lpItem);

	cItem*					getCureAllWrongStatusItem();
	cItem*					getCurePoisonStatusItem();
	cItem*					getCureStrangeStatusItem();
	cItem*					getCureDeclinePowerStatusItem();
	cItem*					getCureCurseStatusItem();

	cItem*					getSecretDungeonKey(int _iDungeonSerial);
	cItem*					getNonameCoupleRing();
	cItem*					getMagicCarpetEmblem();
	int						getMagicCarpetEmblemIndex();

	void					askItemListInItemPackForSelect(int _iItemIndex);
	void					selectItemInPack(int _iItemIndex,int _iItemSlot,int _iPrefix);
	void					useBottomlessBox(int _iBottomlessBox,int _iItem1,int _iItem2);
	void					changeISPISetting(int _iItemIndex,int _iPowerType);
	void					buyPigeonPostItem(int _iItemIndex);
	void					useCoupleRing(int _iItemIndex);
	void					warpFieldByGateGlove(int _iTargetField, BOOL _isNetCafeUser = FALSE,int _iItemSlot=-1);
	BOOL					isOwnGateGlove();

	BOOL					getDamageByBullet(cACTOR *_lpCaster,int *_lpiFireDamage,int *_lpiWaterDamage,int *_lpiWindDamage,int *_lpiEarthDamage,int *_lpiLightDamage,int *_lpiDarkDamage);
	BOOL					removeBook(int _iBook);
	BOOL					removeAddPageItem(int _iBook,int _iCategory,int _iBeginPage,int _iEndPage);
	int						getBookCompletePercentage(int _iBook);
	BOOL					isAvailBookPage(int _iBook,int _iCategory,int _iPage);
	cBookPageItemDefine*	getAddBookPageItem(int _iBook,int _iCategory,int _iPage);
	void					writeBookPage(int _iBook,int _iCategory,int _iPage);
	void					eraseBookPage(int _iBook,int _iCategory,int _iPage);
	BOOL					removePrefx(int _iUseItem,int _iDestItem,int _iPrefixIndex);
	BOOL					judgeItem(int _iItemSlot);
	BOOL					mergeItem(int _iItemSlot);
	int						getCoupleRingIndex(char *_lpstrName);
	BOOL					openRepairItemWindow(cACTOR *_lpOwner);
	BOOL					openDisjointingItemWindow(cACTOR *_lpOwner);

	BOOL					dropItem(int _iX,int _iY,int _iItemSlot,DWORD _dwCount,int _iBaseItem,int _iCheckSum);
	BOOL					isOwnedOXQuizTicket();		//	OX퀴즈 티켓을 가지고 있다.
	BOOL					isOwnGuildHallPortal();
	void					removeOXQuizTicket();	//	OX 퀴즈 티켓 제거
	void					removeGuildBattleItem();
	void					breakdownArmorDurability();
	void					breakdownWeaponDurability();

	int						getWeaponColorizeEffect();	//	무기의 컬러 이펙트
	int						getShieldColorizeEffect();	//	방패의 컬러 이펙트
	int						getArmorColorizeEffect();	//	방어구의 컬러 이펙트
	void					operateSpecialItemEffectInInventory();
	void					operateSpecialItemEffectInInventory(int _iIndex,cItem *_lpItem,int *_lpiBadgeCount,BOOL *_lpbHaveAddPotalItem,BOOL *_lpbExpExceptionItem,BOOL *_lpaIsCanNotSupportByLowAbility);

	void					checkItemValidateTerm();
	BOOL					removeQuestItem(int _iQuestKind,int _iQuestIndex);
	BOOL					removePrincessWeapon(int _iSerial=0xffff,DWORD _dwNameHashCode=0xffffffff);

	int						getItemCount(int _iItem=0xffff,BOOL _bIsIncludeEquippedItem=TRUE);	//	_iItem의 보유 수
	void					setItemUniqueSerial(DWORD _dwSerial);
	void					checkValidSerialItem();
	cItem*					getInventoryItem(int _iSlot,BOOL _bIsWantSlot=FALSE);	//	인벤토리에 아이템 얻어오기
	cItem*					getInventoryItemInData(int _iSlot);	//	CPlayerData에서 아이템을 얻어 온다.

	int						getItemSlotIndex(cItem *_lpItem);
	int						getBlockLightPocketItemIndex();	
	int						getRemainInventorySlotCount();	//	비어 있는 인벤토리 슬롯의 수
	int						decreaseItemCount(cItem *_lpItem,BOOL _isBeltSlotItem);

	BOOL					isUseAbleItem(cItem *_lpItem,BOOL _bIsCheckSecondJob=FALSE);	//	사용 가능한 아이템이냐?
	inline	BOOL			isEquipAbleItem(cItem *_lpItem,BOOL _bIsCheckSecondJob=FALSE){return	isUseAbleItem(_lpItem,_bIsCheckSecondJob);}	//	착용 가능한 아이템이냐?
	cItem*					getValidInventorySlot(BOOL _bIsFullSlot=FALSE);//	빈슬롯 찾아서 리턴~
	int						getValidInventorySlotIndex(BOOL _bIsFullSlot=FALSE);//	빈슬롯 찾아서 리턴~
	void					arrangeInventory();//	아이템 정렬
	void					checkIncorrectItem(int _iWhere);
	int						findSameSerialItem(DWORD _dwSerial,int _iSlot);

	int						getSetItemCount(int _iItemSet);
	void					operateItemSetEffect(int _iItemSet);
	void					operateItemSetEffect();

	BOOL					isInterruptingArmor(cItem  *_lpItem);

	void					operateEquipmentData();	//	장비 데이터를 처리한다.
	BOOL					applyEquipmentAbility(int _iPart,cItem *_lpItem);	//	장비 하나의 능력치를 적용시킨다.

	BOOL					fillBeltSlot(cItem *_lpItem,int _iSlot);//벨트에 내용물을 채운다.
	int						equipItem(cItem *_lpItem,int _iWhere=0xffff,int *_lpiResultPart=NULL);
	BOOL					equipSpareItem(cItem *item);	//	스패어 장비를 입는다.
	BOOL					stripEquipment(int _iPart);	//	어떤 부위의 것을 벗냐?
	BOOL					removeEquipment(int _iWhere);	//	어떤 부위의 것을 제거 하냐
	void					consumedJewel();
	BOOL					changeItemPlace(int _iSrcItem,int _iDestItem);//	두 아이템의 위치를 바꾼다.
	BOOL					destroyItem(int _iItemSlot);
	WORD					getItemCheckSum();	//	체크섬을 구한다.

	cItem*					getSameItem(cItem *_lpItem);	//	같은 아이템을 찾아서 리턴한다.
	int						getSameInventoryItemCount(cItem* _lpItem);//09.08.11	같은 아이템의 갯수를 리턴한다.
	int						getSameEquipItemCount(cItem* _lpItem);		//09.08.11	같은 아이템의 갯수를 리턴한다.
	cItem*					getStackAbleItem(cItem *_lpItem);	//	겹칠 수 있는 아이템을 찾아서 리턴한다.
	cItem*					getSameKindItem(cItem *_lpItem);	//	같은 부류의 아이템을 찾아서 리턴한다.
	int						addItemToInventory(cItem *_lpItem,BOOL _bIsStripItem=FALSE,int _iMethod=eGET_GOLD_BY_ADD_ITEM);	//	인벤토리에 아이템을 추가한다
	int						addGoldItemToInventory(cItem *_lpItem,int _iMethod);
	BOOL					removeItem(int _iItem,int _iCount,int _iIncludeEquipment,int _iReason,int _iRemoveGBReason);		// 09.08.21
	BOOL					_removeItem(int _iItem,int _iCount,int _iIncludeEquipment,int _iReason);		// 09.08.21

	BOOL					isExistItem(cItem *_lpItem,BOOL _bIsStripItem);
	int						reloadBeltSlot();//벨트 슬롯에 아이템을 리로드-o-한다.
	BOOL					operateReloadBeltSlot();//	벨트에 아이템을 리로드 한다.
	BOOL					isAvailReloadBeltSlot();//	리로드 가능하냐?
	void					setBeltItemReloadTime();//	벨트 아이템 리로드 타임 설정
	BOOL					isExhaustItem(int _iItemSlot);	//	소비성 아이템이냐?

	void					throwWeapon(int _iX,int _iY,cAbility *_lpAbility,BOOL _bIsThrowOtherJobWeapon);	//	아이템 떨구기.. 어디다가?
	inline	cItem			*getBeltItem(int _iSlot)	{return	&m_aEquip[_iSlot+dEQUIP_BELT_SLOT1];}	//	벨트 아이템 구하기
	int						getBadgeCount();	//	배지의 수를 리퉈어어언~~

	void					decreaseBullet(cAbility *_lpAbility);	//	탄환이 필요한 스킬이었다면 탄환을 줄인다.
	inline	cItem			*getEquipment(int _iPart)	//	_iPart장비 리턴
	{
		if (_iPart == dEQUIP_WEAPON && m_wJob%2 && m_wJob != dJOB_SUMMONER)	
			_iPart	=	dEQUIP_WEAPON_2;

		if (m_aEquip[_iPart].m_wBaseItem	!=	0xffff)	
			return	&m_aEquip[_iPart];

		return	NULL;
	}

	inline	cItem			*getEquipmentSlot(int _iPart)
	{
		if (_iPart == dEQUIP_WEAPON && m_wJob%2)	
			_iPart	=	dEQUIP_WEAPON_2;

		return	&m_aEquip[_iPart];
	}//	_iPart장비슬롯 리턴

	inline	cItem			*getWeapon()	//	무기! -o-
	{
		if (!c_aUseWeaponJob[m_wJob])	
			return	NULL;

		int	iPart	=	dEQUIP_WEAPON;

		if (m_wJob%2	&&	m_wJob	!=	dJOB_SUMMONER)
			iPart	=	dEQUIP_WEAPON_2;

		if (iPart	==	dEQUIP_WEAPON	&& m_wHideWeapon1Time)
			return	NULL;
		if (iPart	==	dEQUIP_WEAPON_2 && m_wHideWeapon2Time)
			return	NULL;

		if(m_aEquip[iPart].m_wBaseItem	!=	0xffff)
			return	&m_aEquip[iPart];

		return	NULL;
	}

	inline	cItem			*getOtherJobWeapon()//	다른 직업의 무기
	{
		int	iJob	=	m_wJob;
		int	iPart	=	dEQUIP_WEAPON;

		if (iJob%2)
			iJob--;
		else
			iJob++;

		if (!c_aUseWeaponJob[iJob])
			return	NULL;

		if (iJob%2	&&	iJob	!=	dJOB_SUMMONER)
			iPart	=	dEQUIP_WEAPON_2;

		if (iPart	==	dEQUIP_WEAPON	&& m_wHideWeapon1Time)
			return	NULL;
		if (iPart	==	dEQUIP_WEAPON_2 && m_wHideWeapon2Time)
			return	NULL;

		if (m_aEquip[iPart].m_wBaseItem	!=	0xffff)
			return	&m_aEquip[iPart];

		return	NULL;
	}

	inline	cItem			*getBullet()	//	탄환!! -o-
	{
		if (!c_aUseBulletJob[m_wJob])
			return	NULL;

		if (m_aEquip[dEQUIP_SHIELD_BULLET].m_wBaseItem	!=	0xffff)
			return	&m_aEquip[dEQUIP_SHIELD_BULLET];

		return	NULL;
	}

	inline	cItem			*getArmor()	//	방패! -o-
	{
		return	getEquipment(dEQUIP_ARMOR);
	}

	inline	cItem			*getShield()	//	방패! -o-
	{
		if (m_wIsHideShield)				
			return	NULL;
		if (!c_aUseShieldJob[m_wJob])	
			return	NULL;

		if (m_aEquip[dEQUIP_SHIELD_BULLET].m_wBaseItem	!=	0xffff)
			if (m_aEquip[dEQUIP_SHIELD_BULLET].getBasicItem()->m_wKind	==	eIK_SHIELD)
				return	&m_aEquip[dEQUIP_SHIELD_BULLET];

		return	NULL;
	}

	cItem*					getItemByBasicItem(int _iBasicItem,int _iOrder=1);	//	기본 아이템이 _iBasicItem인 아이템을 찾아서 리턴한다.
	int						getItemSlotByBasicItem(int _iBasicItem);	//	기본 아이템이 _iBasicItem인 아이템을 찾아서 리턴한다.
	cItem*					getEmptyItemSlot(int* _iSlot = NULL);	//	비어 있는 아이템 슬롯을 찾아서 리턴한다.
	int						getRemainItemSlotCount();
	void					computeItemCount();//아이템의 수 계산.
	int						getMaxPremiumItemLevel();
	BOOL					isAbleRemeberPlace(int _iSlot);
	int						getStackItemCount(int _iSerial, BOOL _bEQuip);		// 스택아이템 갯수(슬롯갯수가 아니다)를 리턴한다.		//09.08.21
	BOOL					upgradeEternalItem(int _iWeaponSlot, int _iUpgradeItemSlot, int _iUpgradeNormalItemSlot,BOOL _bUpgradePerfect = FALSE );			//  이터널 아이템 업그래이드 ~09.09.08
	int						getInventoryCosmicPowerUpgradeChanceMax();	// 인벤토리에 코스믹파워아이템중 강화확률 제일 높은 것을 리턴.		//09.08.26
	int						getUpgradeSuccessChance(int _iWeaponSlot, int _iUpgradeItemSlot, int _iUpgradeNormalItemSlot);			//  이터널 아이템 업그래이드 확률~09.08.26
	int						getUpgradeMaterialsGold(int _iCurrentItemLevel);							//  아이템 업그래이드 재료 (골드)  09.08.27
	int						getUpgradeMaterialsStoneOfMystery(int _iCurrentItemLevel);							//  아이템 업그래이드 재료 (신비석)  09.08.27
	int						getUpgradeMaterialsTantilless(int _iCurrentItemLevel);							//  아이템 업그래이드 재료 (탄틸리스유물)  09.08.27
	int						getUpgradeMaterialsCrystal(int _iCurrentItemLevel);							//  아이템 업그래이드 재료 (결정석)  09.08.27

	BOOL					isInterruptingWeapon(cItem  *_lpItem);	// 무기해제상태...

	BOOL					copyPrefix(int _iUseItem,int _iDestItem,int _iPrefixIndex);		//접두사 복사하기
	BOOL					cutPrefix(int _iUseItem,int _iDestItem,int _iPrefixIndex);		//접두사 잘라내기

	int						getOperateItemSetList(int* _lpItemSetList);		// 처리할 세트아이템의 갯수와 리스트를 얻는다.
	void					removeItemSetEffect(int* _lpItemSetList,int* _lpItemSetCount, int _iItemSetListCount);	// 케릭터스텟에 적용된 세트아이템옵션을 지운다.

	int						getUpgradeMaterialsGoldForJapan(int _iCurrentItemLevel);							//  아이템 업그래이드 재료 (골드)  일본용
	int						getUpgradeMaterialsStoneOfMysteryForJapan(int _iCurrentItemLevel);							//  아이템 업그래이드 재료 (신비석) 일본용
	int						getUpgradeMaterialsTantillessForJapan(int _iCurrentItemLevel);							//  아이템 업그래이드 재료 (탄틸리스유물)  일본용
	int						getUpgradeMaterialsCrystalForJapan(int _iCurrentItemLevel);							//  아이템 업그래이드 재료 (결정석)   일본용
	int						getEquipItemRebirthCount(int _iRebirthCount, BOOL _isIgnoreRingOfInfinity);// 착용중인 전생회차가 붙은 아이템 개수를 리턴한다.
	BOOL					addItemWithOption(WORD _wItemIndex, int _iCount, int* _lpOption,int _iMethod);
	BOOL					isExistItemEnteringGuildDungeon();
	void					operateBodyEffectItem();
	BOOL					isCanNotUseItemToGVG(int _iItem);
//┃CActorItem.cpp
//┃아이템 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃함정 관련
//┃trapForActor.cpp
	void					checkCaughtInTrap(cTrap *_lpTrap=NULL);
	void					getTrapDamage(cACTOR *_lpOwner,cTrap *_lpTrap,cAbility *_lpAbility,CHitInfo *_lpHitInfo);
	void					getTrapMagicDamage(cACTOR *_lpOwner,cTrap *_lpTrap,cAbility *_lpAbility,CHitInfo *_lpHitInfo);
	BOOL					getTrapPhysicalDamage(cACTOR *_lpOwner,cTrap *_lpTrap,cAbility *_lpAbility,CHitInfo *_lpHitInfo);
	void					damagedByTrap(cACTOR *_lpOwner,CHitInfo *_lpHitInfo,cAbility *_lpAbility,int _iX,int _iY);
	void					tryDisarmTrap(int _iTrapSerial);
//┃trapForActor.cpp
//┃함정 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃장비 효과
//┃CActorEquipmentEffect.cpp
	void					getItemMagicDamage(int _iEffect,int _iValue1,int _iValue2,int *_lpiFireDamage,int *_lpiWaterDamage,int *_lpiWindDamage,int *_lpiEarthDamage,int *_lpiLightDamage,int *_lpiDarkDamage);
	void					getItemPrefixMagicDamage(int _iEffect,int _iValue1,int _iValue2,int *_lpiFireDamage,int *_lpiWaterDamage,int *_lpiWindDamage,int *_lpiEarthDamage,int *_lpiLightDamage,int *_lpiDarkDamage);

	void					operateEquipmentBasicData(int _iEffect,int _iValue1,int _iValue2);//	장비의 기본 데이터 처리

	inline	void			operateEquipmentPrefix(CItemPrefixInfo *_lpPrefixData)//	장비에 붙은 접두사 정보 처리
	{
		operatePrefixData(g_aItemPrefix[_lpPrefixData->m_wPrefix].m_wType,_lpPrefixData->m_aValue[0],_lpPrefixData->m_aValue[1],g_aItemPrefix[_lpPrefixData->m_wPrefix].m_sValue3);
	}
	inline	void			operateEquipmentUniqueData(cUniqueData *_lpUniqueData)//	장비에 붙은 유니크 정보 처리
	{
		operatePrefixData(_lpUniqueData->m_wEffect,_lpUniqueData->m_aValue[0],_lpUniqueData->m_aValue[1],_lpUniqueData->m_aValue[2],TRUE);
	}
	void					operatePrefixData(int _iEffect,int _iValue1,int _iValue2,int _iValue3,BOOL _bIsUnique=FALSE);//	접두사 처리
//┃CActorEquipmentEffect.cpp
//┃장비 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
	//아이템 인덱스로 갯수 만큼 재거
//	BOOL					removeItemByIndex(int _iItem,int _iCount,int _iIncludeEquipment);//	removeItem을 같이 사용
	

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃아이템 효과 - 캐릭터에게 사용
//┃CActorItemUseToCharacter.cpp
	int						useMagicCarpetEmblem(cItem *_lpItem,int _iEffect);
	int						eatItem(cItem *_lpItem,int _iHealEfficient=100, cACTOR* lpUser = NULL);//아이템 효과 처리
	int						castDuelBuff(int _iEffect,int _iValue1);
	int						useJewel(cItem *_lpItem);//	보석 사용
	int						useImproveStateItem(cItem *_lpItem);//	스탯 향상 아이템 사용
	int						useItemToCharacter(cItem *_lpItem,cACTOR *_lpTarget,BOOL _isBeltSlotItem);//	아이템 사용
	BOOL					useMemoryStone(int _iWaypointSlot,int _iPieceCount);
	BOOL					useTeleportStone(int _iWaypointSlot);
	BOOL					usePortalStone(int _iWaypointSlot);
	BOOL					useGuildMarkPack(int _iPack);
	BOOL					useTreasureMap();
	BOOL					useCoupleRingSet(cItem *_lpItem);
	BOOL					useCoupleRing();
	BOOL					useFireWork(int _iKind=0,int _iTime=0);

	BOOL					increaseLastDamage(int _iTime,int _iValue);
	BOOL					increaseHuntExp(int _iTime,int _iValue);
	BOOL					increaseItemDropChance(int _iTime,int _iValue);
	BOOL					clearDeathPenelty();
	void					operateBuffItem(cItem *_lpItem, cACTOR* _lpUser);
//┃CActorItemUseToCharacter.cpp
//┃아이템 효과 - 캐릭터에게 사용
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃아이템 효과 - 아이템에 사용
//┃CActorItemUseToItem.cpp
	int						improveWeaponDamage(int _iValue,cItem *_lpDestItem);//무기 데미지 증가
	int						improveArmorDefense(int _iValue,cItem *_lpDestItem);//갑옷 방어력 증가
	int						attachPrefix(int _iValue1,int _iValue2,cItem *_lpDestItem);//접두사 붙이기!!
	int						changeDamagePrefix20031101(cItem *_lpItem,int _iSrcItemIndex,int _iDestItemIndex);			//	
	int						changeDamagePrefix20031117(cItem *_lpItem,int _iSrcItemIndex,int _iDestItemIndex);

	int						askRemoveItemPrefix(cItem *_lpItem,cItem *_lpDestItem);;

	int						addCategoryToBook(cItem *_lpDestItem,cItem *_lpItem);
	int						sealSphereOfDawn(cItem *_lpDestItem,cItem *_lpItem,int _iSrcItemIndex,int _iDestItemIndex);
	int						addPageToBook(cItem *_lpDestItem,cItem *_lpItem,int _iSrcItemIndex,int _iDestItemIndex);
	int						repairDxItem(cItem *_lpDestItem,int _iRepair);
	int						chargeExtraItem(cItem *_lpChargeKit,cItem *_lpDestItem);
	int						jimMorysEnchantService(cItem *_lpEnchantItem,cItem *_lpDestItem);
	int						enchantItemByUniqueInfo(cItem *_lpItem,cItem *_lpDestItem);//아이템을 아이템에 사용한다.
	int						repairBrokenItem(cItem *_lpItem,cItem *_lpDestItem);
	int						repairCursedItem(cItem *_lpItem,cItem *_lpDestItem);
	int						repairReleaseBlockPrefixStatus(cItem *_lpItem,cItem *_lpDestItem);

	int						unskilledRepairBrokenItem(cItem *_lpItem,cItem *_lpDestItem);

	int						useItemToItem(cItem *_lpItem,cItem *_lpDestItem,int _iSrcItemIndex,int _iDestItemIndex);//아이템을 아이템에 사용한다.
	int						operateEnchantWorkResult(int _iResult,cItem *_lpItem,cItem *_lpDestItem,int _iSrcItemIndex,int _iDestItemIndex);
	int						attachPrefixToItem(cItem *_lpItem, cItem *_lpDestItem,int _iDestItemIndex, int _iValue1);//접두사 붙이기!!
	int						askOpenCopyOrCutPrefix(cItem *_lpItem,cItem *_lpDestItem, BOOL _bIsCut);	// 접두사 복사나 잘라내기 창열기~//┃CActorItemUseToItem.cpp
	int						increaseCarvingLevel(cItem *_lpItem, cItem *_lpDestItem,int _iSrcItemIndex,int _iDestItemIndex);
	int						setCarvingLevel(cItem *_lpItem, cItem *_lpDestItem);
//┃CActorItemUseToItem.cpp
//┃아이템 효과 - 아이템에 사용
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃타이틀
//┃CActorTitle.cpp
	void					checkRedStonePoint();	//	보물 상자 관련 붉은 보석 탐정 칭호 확인
	void					increaseTitleLevelByCH5(WORD *_lpInfo);

	BOOL					addTitle(int _iTitle,int _iTitleLevel,BOOL _bIsSendMessage=TRUE);
	BOOL					removeTitle(int _iTitle,BOOL _bIsSendMessage=TRUE,BOOL _bIsByCH5Reward=FALSE);
	CPlayerTitleInfo*		getTitle(int _iTitle);
	CPlayerTitleInfo*		getTitleByTitleEffect(int _iTitleEffect);

	BOOL					isOwnedTitle(int _iTitle);
	void					operateTitleData();
	void					operateTitleData(CPlayerTitleInfo *_lpTitleInfo);

	void					operateTitleSkillMaster(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitlePorter(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleDealer(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleBankClient(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleCityWalker(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleCampingMaster(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleSurvivalInstincts(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleDruggist(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleEsoteric(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleCourtesy(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleAlchemy(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleBerserker(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleFrontier(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleElementalMaster(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleAdventurer(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleInclinePower(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleInclineMagicResistance(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);
	void					operateTitleNaturalEnemy(CTitleEffectInfo *_lpTitleEffect,int _iTitleLevel);

	void					useTitle(int _iTitle);
	int						getTrapEvadeCahnceByTitle();
	int						getCorrectUnlockLevelByTitle();
	int						getBonusAttackDoorPowerByTitle();

	int						getEnableTitle(int _iTitleEffect,CTitle	*_lpTitle);
	int						getTitleBankClientTax(BOOL _bRealLevel = FALSE);			// 은행 금괴 수수료(%) 리턴~	09.08.19
	inline	BOOL			isActiveSurvivalInstinctTitle()
	{
		if	(m_wSurvivalInstinctsTitleTriggerHP	==	0)
			return	FALSE;

		if	(m_iHP/100	<=	m_wSurvivalInstinctsTitleTriggerHP)
			return	TRUE;

		return	FALSE;
	}
//┃CActorTitle.cpp
//┃타이틀
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃상태 처리
//┃CActorStatus.cpp
	//	피 흡혈 상태 처리
	void					operateBloodDrainStatus(cACTOR *_lpTarget,int _iDamage);
	//	자책감 상태 처리
	void					operateSelfReproofStatus();
	//	충동 상태 처리
	void					operateImpulseStatus();
	//	고문
	void					operateTortureStatus();
	//	악화
	void					operateDegenerateStatus(int _iHealHp);
	//	가위
	void					operateTerribleDream();
	//	화병
	void					operateHwabyung();
	//	음모의 그림자
	void					operatePlotOfShadow(int _iSpentTime);
	//	음모의 그림자 시전자 대미지 입기
	void					operatePlotOfShadowCaster(int _iDamage,int _iSkill);
	//	죽음의 향기
	void					operateSmellOfDeath(cACTOR *_lpCaster);

	//	죽음의 카운트 다운 처리
	void					operateNotificationOfDeathCounter();

	//	계약 처리
	void					operateContract();
	BOOL					releaseContractTime();

	//	혼의 서약
	void					operateSoulOath(cACTOR *_lpKiller);

	//	피의 맹약
	void					operateBloodCompact(cACTOR *_lpKiller);

	void					resetStatusForDeath();
	void					resetStatusForExitField();
	BOOL					setReversalPowerTime(int _iTime);
	BOOL					setCofuseTime(int _iTime,cACTOR *_lpCaster);
	void					getUpkeepStatus(cActorStatusUpkeep *_lpInfo);
	void					setUpkeepStatus(cActorStatusUpkeep *_lpInfo);
	inline void	setRemainActionTime(int _iTime)
	{
		m_wRemainActionTime	=	max(m_wRemainActionTime,_iTime);
	}
	int						addEnchantedMagic(cAbility *_lpAbility,WORD _wTime);
	void					removeEnchantedMagic(int _iSkill);
	void					updateEnchantedMagic(BOOL _bIsIgnoreDeathStatus=FALSE);
	void					correctInvisibleStatus(CVerySimplePlayerInfo *_lpInfo,cACTOR *_lpTarget);
	void					correctInvisibleStatus(CSimplePlayerInfo *_lpInfo,cACTOR *_lpTarget);
	void					correctInvisibleStatus(LONGLONG *_lpdwMask,cACTOR *_lpTarget);
	void					setJumpFlyTime(int _iTime);
	void					resetJumpFlyTime();
	void					charm(cACTOR *_lpCaster,int _iTime,BOOL _bIBooking=TRUE);
	BOOL					isCharmed();
	void					releaseCharm(BOOL _bIBooking=TRUE);

	BOOL					isCanNotCloseGameStatus();
	void					buildPower();

	inline	void			setChargeTime(int _iTime)	{m_wChargeTime	=	_iTime;}

	BOOL					isTamed();
	void					tamed(cACTOR *_lpTamer);
	inline	BOOL			isTamedMonster(int _iSerial)
	{
		for (int i=0;i<dMAX_TAME_MONSTER_COUNT;i++)
		{
			if (m_awTamedMonsterList[i]	==	_iSerial)
				return	TRUE;
		}

		return	FALSE;
	}

	void					regenHPByAidSkill(int _iHealPoint);
	BOOL					recoverHPByAidSkill(int _iSkill,int _iHealPoint);
	void					absorbDamage(int _iTarget,int _iDamage);	//	대미지 흡수
	void					setPoisonDamage(int _iTime,int _iDamage,int _iCaster);

	void					setFreezeTime(int _iTime);
	void					setNightmareTime(int _iTime);
	void					setParalysisTime(int _iTime);

	void					resetTemporaryMagicEffect();
	void					initData();	//	데이터를 초기화 한다.
	void					initDataForPlayer();
	void					buildByGuildStragety(BOOL _bIsRecoverHP=FALSE);
	void					renewPremiumItem(BOOL _bIsNow=FALSE);	//	프리미엄 아이템 보상
	void					rebuildBasicData();//	기본 정보를 리빌드~~
	void					rebuildForUpdateLevel();
	void					decreaseAidMagicTime();
	void					operateRecoverHP();//HP 회복처리
	BOOL					isSitdown();//	앉아있냐?
	void					operateDeathPenelty();
	void					recoverDeathPenelty();
	BOOL					doYouReallyRecoverDeathPenelty();
	void					boostPower(int _iPercentage);

	int						getFreezeResistance();
	int						getColdResistance();
	int						getBlindResistance();
	int						getStunResistance();
	int						getStrangeStatusResistance(BOOL _bIsRequireBonus);	//	상태 이상 저항
	int						getCurseResistance();			//	저주 저항
	int						getDeclinePowerResistance();	//	능력치 저하 저항
	int						getConfuseResistance();			//	혼란
	int						getCharmingResistance();		//	유혹
	int						getStoneResistance();			//	석화
	int						getPoisonResistance();			//	중독
	int						getSleepResistance();			//	잠
	void					reduceHP(int _iDamage,cACTOR *_lpAttacker = NULL,int _iLimitHP=0,BOOL _bIsByDamagePool=FALSE,BOOL _bIsByMiniPet=FALSE);			//	체력 줄어듦


	int						getConcentration();
	int						getFireResistance(int _iCorrectValue=0);
	int						getWaterResistance(int _iCorrectValue=0);
	int						getWindResistance(int _iCorrectValue=0);
	int						getEarthResistance(int _iCorrectValue=0);
	int						getLightResistance(int _iCorrectValue=0);
	int						getDarkResistance(int _iCorrectValue=0);
	int						getAllMagicResistance();

	int						getStrengtheningFireDamage(int _iCorrectByTitle,BOOL _bIsPureMagicDamageSkill);
	int						getStrengtheningWaterDamage(int _iCorrectByTitle,BOOL _bIsPureMagicDamageSkill);
	int						getStrengtheningWindDamage(int _iCorrectByTitle,BOOL _bIsPureMagicDamageSkill);
	int						getStrengtheningEarthDamage(int _iCorrectByTitle,BOOL _bIsPureMagicDamageSkill);
	int						getStrengtheningLightDamage(int _iCorrectByTitle,BOOL _bIsPureMagicDamageSkill);
	int						getStrengtheningDarkDamage(int _iCorrectByTitle,BOOL _bIsPureMagicDamageSkill);

	int						getWeakningTargetFireResistance();
	int						getWeakningTargetWaterResistance();
	int						getWeakningTargetWindResistance();
	int						getWeakningTargetEarthResistance();
	int						getWeakningTargetLightResistance();
	int						getWeakningTargetDarkResistance();

	int						getStrength(BOOL _bIsAskByReversal=FALSE);
	int						getConstitution(BOOL _bIsAskByReversal=FALSE);
	int						getIntelligence(BOOL _bIsAskByReversal=FALSE);
	int						getCharisma(BOOL _bIsAskByReversal=FALSE);
	int						getWisdom(BOOL _bIsAskByReversal=FALSE);
	int						getAgility(BOOL _bIsAskByReversal=FALSE);
	int						getLuck(BOOL _bIsAskByReversal=FALSE);
	
	inline	int				getActionSpeed()	{return	m_actionSpeed.getCorrectedValue();}//액션 속도 보정
	int						getMoveSpeed();

	inline	int				getDodgeChance()	{return	m_dodgeChance.getCorrectedValue();}//회피 보정
	inline	int				getHitChance()		{return	m_hitChance.getCorrectedValue();}//명중 보정
	BOOL					firstAid(int _iFirstAidHP,int _iUpkeepTime=0);
//	MAX HP = [{기본 HP + 건강 * (직업 HP 건강 보정치) + 레벨 * (직업 HP 레벨 보정치)} * 아이템,기술에 의한 보정율 총합] + 아이템, 기술에 의한 보정치 총합
	int						getMaxHP();	//	최대 체력
	int						getRemainHPRate();

//	MAX CP = [{기본 CP + 카리스마 * (직업 CP 카리스마 보정치) + 레벨 * (직업 CP 레벨 보정치)} * 아이템, 기술에 의한 보정율 총합] + 아이템, 기술에 의한 보정치 총합
	int						getMaxCP();	//	최대 CP
	void					reduceCP();
	void					reduceCP(int _iCP,int _iSkill);	//	CP 빼기
//	달릴 때에도 충전 (히트, 행동 모두 포함) 직후 (1.5 + 카리스마/100)초간은 CP가 감소하지 않음
	void					increaseCP(int _iCP)	//	CP 더하기
	{
		if(m_fightEnergy.isIncreaseGetCP())
		{
			_iCP += m_fightEnergy.m_iValue;
		}

		if	(_iCP	==	0)
			return;

		if	(m_wNoMoreFightTime)
			return;

		_iCP			=	_iCP+(_iCP*m_bGetBonusCPPercentage)/100;
		m_iCP			=	min(m_iCP+_iCP,getMaxCP());
		m_wCPFloatTime	=	dDEFAULT_CP_FLOAT_TIME+getCharisma()*dSYNC_FPS/100;
	}

	void					healHP(int _iHealPoint);	//	체력 회복
	BOOL					instanceHealHP(int _iHealPoint);	//	체력 회복
	void					increaseHP(int _iHP);

	inline	void			regenHP()	//	체력 올라감
	{
		if	(m_regenHPPerSecond.getCorrectedValue()	==	0)
			return;

		if	(m_iHP	>=	getMaxHP()	&&	m_iFirstAidHP	==	0)
			return;

		int	iRecoverHP	=	(m_regenHPPerSecond.getCorrectedValue()/dSYNC_FPS+1);

		increaseHP(iRecoverHP);
	}
	void	operateSoulBurn(int _iDamage,int _iSkill);		// 소울번.	

	void	dispellAllBuff();
	void	releaseTransformationMonster();
	void	decreaseBitTime();
//┃CActorStatus.cpp
//┃상태 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃데이터 셋팅
	void					reset();//초기화

	void					setDataByJoinPlayerData(CPlayerData *_lpData,int _iClientSerial,cActorStatusUpkeep *_lpUpkeepData);
	void					setDataByCharacter(cCHARACTER *_lpCharacter,int _iActorKind,int _iLevel,int _iX=-1,int _iY=-1,cFIELD* _lpField = NULL);//캐릭터 정보를 기본으로 액터 데이터 설정
	void					setDataByJob(cJOB *_lpJob,int _iActorKind,int _iLevel,int _iX=-1,int _iY=-1);//캐릭터 정보를 기본으로 액터 데이터 설정
	void					loadActorData(cBASE_NPC_INFO *_lpSaveActorData,cCHARACTER	*_lpCharacter);//맵상에 저장되어 있는 액터 데이터를 불러온다.
	void					setFpsByWeaponSpeed(int _iCorrectSpeed=100);
	void					setFpsByTPA(int _iTPA,int _iCorrectSpeed=100);	//	TPA(Tme Per Action)에 맞춰서 FPS를 설정한다.
	BOOL					setActorPos(int _iX,int _iY,BOOL _bIsSettingLastMovedPos=FALSE,BOOL _bIsSetByServer=FALSE);	//위치 설정
//┃데이터 셋팅
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CActorInfo.cpp
//┃정보
	BOOL					isBossMonster();
	BOOL					isOverSemiBossMonster();
	cActor_miniPet*			iMiniPet();
	int						getCorrectUnlockLevel();
	void					getReleasePos(cPOINT *pos,int _iAnm=-1,int _iDirect=-1);				//	액션 프레임에서 마법이나 기타 이펙트 릴리즈 포인트 구함
	BOOL					isBattleWithTargetParty(cACTOR *_lpActor);
	BOOL					isOnSameField(cACTOR *_lpActor);
	BOOL					isOnSameField(int _iFieldUniqueSerial,int _iGateField);
	BOOL					isSealdAction(int _iSkill=-1);
	BOOL					isSealdMove();
	BOOL					isInArea(int _iArea);
	BOOL					isUsedSkill(int _iArea,int _iSkill);
	BOOL					isUsedItem(int _iItem=-1);
	BOOL					isEventMob();
	int						getAimedMeEnemyCount();
	int						getInRangeEnemyCount(int _iRange);
	inline	int				getLevel()
	{
		int iValue =	m_level.getCorrectedValue();
		if(m_wTransformationTime)
			iValue =  m_wTransformationLevel;
		
		return	min(max(iValue,1),999);
	}
	inline	void			setActionFinishTime(DWORD _dwTime)	{m_dwBookedActionFinishTime	=	_dwTime;}
	inline	void			setBasicAttackTime(DWORD _dwTime)	{m_dwBookedBasicAttackTime	=	_dwTime;}
	inline	BOOL			isDenyCommunity()					{return m_bf1IsDenyCommunity;}
	int						getBasicPowerValue(int _iType);
	inline	int				getReleaseTrasformationCp()	{return	-m_wisdom.getCorrectedValue()*100;}
	inline	BOOL			isSlave()			{if (m_wLordActor	!= 0xffff)	return TRUE;return FALSE;}
	inline	BOOL			isControlActor()	{if (m_wSlaveActor	!= 0xffff)	return TRUE;return FALSE;}
	inline	BOOL			isOperator()		{if (m_wOperatorLevel >= eAL_OPERATOR) return	TRUE;return FALSE;}
	inline	BOOL			isTester()			{if (m_wOperatorLevel ||	m_wOperatorLevelInAccount) return	TRUE;return FALSE;}
	inline	BOOL			isIncorrectClient(CClient *_lpClient=NULL){return 1-isCorrectClient(_lpClient);}	//	이상한넘?
	BOOL					isCorrectClient(CClient *_lpClient=NULL);	//	제대로 된넘?
	int						getCheckSumValueForSaveData();	//	세이브 데이터를 위한 첵섬을 구한다.
	CActiveSkill*			getActiveSkill();	//	현재 사용중인 스킬
	BOOL					isNEPlayer(cACTOR *_lpPlayer);	//	천적 플레이어
	DWORD					getCheckSumForClientData();

	int						getDetectingLevel();
	int						getHidingLevel();

	int						getWeaponAttackRange();	//	무기의 사정거리 얻기
	CClient*				getClient();		//	지현씨가 관리하는 클라이언트 구하기
	int						getBCSerial();		//	커뮤니티 서버 시리얼 구하기 
	cNPC*					getNpc();//NPC얻어오기
	cCHARACTER*				getCharacter();//NPC얻어오기
	int						getDirect(int x,int y,BOOL setAngle);
	void					getSimplePlayerInfo(CSimplePlayerInfo *_lpInfo);	//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
	void					getVerySimplePlayerInfo(CVerySimplePlayerInfo *_lpInfo);	//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
	void					getSimpleMonsterInfo(CSimpleMonsterInfo *_lpInfo);	//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
	void					getVerySimpleMonsterInfo(CVerySimpleMonsterInfo *_lpInfo);	//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
	void					getSimpleNpcInfo(CSimpleNpcInfo *_lpInfo);	//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
	void					getVerySimpleNpcInfo(CVerySimpleNpcInfo *_lpInfo);	//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
	void					getMinumumActorInfo(CMinimumActorInfo *_lpInfo);	//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
	void					getEventNpcInfo(cEventNpcInfo *_lpInfo);
	void					getActorInfoForOuterActor(CActorInfoForOuterActor *_lpInfo);	//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.

	inline	int				getItemSlotCount()		{return m_wItemSlotCount;}	//	아이템 슬롯 카운트
	int						getCorrectSkillLevel(cAbility *_lpAbility);
	inline	BOOL			isFirstJob(){return 1-(m_wJob)%2;}	//	첫번째 직업인겨?
	inline	BOOL			isSecondJob(){return (m_wJob)%2;}		//	두번째 직업인겨?
	inline	int				getAnotherJob()	{if (m_wJob%2)	return	m_wJob -1;return	m_wJob+1;}	//	또 다른 직업 -o-
	inline	BOOL			isDummy(){if (m_wDummyTime	==	0xffff)	return	FALSE;return	TRUE;}
	inline	BOOL			isCopyActor(){if (m_wCopyTime	==	0xffff)	return	FALSE;return	TRUE;}
	int						getRestraintTimeByBattle();//	전투에 의해 구속된 시간
	void					setRestraintTimeByBattle();//	전투에 의해 구속된 시간
	void					resetRestraintTimeByBattle();//	전투에 의해 구속된 시간
	int						getRestraintTimeByLogout()	{return	m_wRestraintTimeByLogout;}//	전투에 의해 구속된 시간
	void					setRestraintTimeByLogout()	{m_wRestraintTimeByLogout=dLOGOUT_WAIT_TIMER;}//	전투에 의해 구속된 시간

	inline	int				getAllignment()		{return	GetAllignment(m_sAllignment);}
	inline	BOOL			isAbsoluteGood()	{if (GetAllignment(m_sAllignment)==dALLIGNMENT_ABSOLUTE_GOOD)	return	TRUE;return	FALSE;}	//	절대 선이냐?
	inline	BOOL			isGood()			{if (GetAllignment(m_sAllignment)==dALLIGNMENT_GOOD			)	return	TRUE;return	FALSE;}	//	선이냐?
	inline	BOOL			isNeutral()			{if (GetAllignment(m_sAllignment)==dALLIGNMENT_NEUTRAL		)	return	TRUE;return	FALSE;}	//	중립이냐?
	inline	BOOL			isEvil()			{if (GetAllignment(m_sAllignment)==dALLIGNMENT_EVIL			)	return	TRUE;return	FALSE;}	//	악이냐?
	inline	BOOL			isAbsoluteEvil()	{if (GetAllignment(m_sAllignment)==dALLIGNMENT_ABSOLUTE_EVIL)	return	TRUE;return	FALSE;}	//	절대 악이냐?
	inline	int				getCPLevel()		{return	GetCPLevel(m_iCP);}	//	현재 CP 레벨을 구한다.
	int						isAbleToTransformation(BOOL _bIgnoreExclusive=FALSE);	//	지금 변신 가능하냐?
	int						getCurrentLevelDeathPeneltySecond();
	int						getFixedLevelDeathPeneltySecondByGuild(int _iRecverTime);
	int						getCurrentLevelDeathPeneltyValue();
	int						getCurrentLevelDeathPeneltyRecoverPrice();
	inline	int				getSpentCPForTransformation()
	{
		if (c_aFreeJobChangeJob[m_wJob])	return	0;

		int	iCP	=	min(max(2000 - getLevel()*100/4,500),m_iCP);

		return	iCP;
	}

	inline	BOOL			isUndead(BOOL	_bIsReal = FALSE)
	{

		if(!_bIsReal && m_wMakeTypeUndeadTime)
			return TRUE;			

		if (getKind()	==	eJT_UNDEAD)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isDemon()	{if (getKind()	==	eJT_DEVIL)	return	TRUE;return	FALSE;}
	inline	BOOL			isHuman()	{if (getKind()	==	eJT_HUMAN)	return	TRUE;return	FALSE;}
	inline	BOOL			isAnimal()	{if (getKind()	==	eJT_ANIMAL)	return	TRUE;return	FALSE;}
	inline	BOOL			isHolyBeast(){if (getKind()==	eJT_HOLY_BEAST)return	TRUE;return	FALSE;}

	inline	BOOL			isEnemy(cACTOR *_lpActor)
	{
		if (!_lpActor) return FALSE;
		if (_lpActor->m_wTeam == m_wTeam) return FALSE;

		return	TRUE;
	}
	inline	BOOL			isOwnForce(cACTOR *_lpActor)
	{
		if (!_lpActor) return FALSE;
		if (_lpActor->m_wTeam == m_wTeam) return TRUE;

		return	FALSE;
	}
	BOOL					isPureMonster();
	BOOL					isDetector();

	int						getExpBonusByA();
	int						getFireResistanceBonusByA();
	int						getWaterResistanceBonusByA();
	int						getWindResistanceBonusByA();
	int						getEarthResistanceBonusByA();
	int						getLightResistanceBonusByA();
	int						getDarkResistanceBonusByA();
	int						getTradeBonusByA();

	int						getItemDropChanceBonusByA();
	int						getItemDropLevelBonusByA();
	int						getBitCount(WORD _wSerialInField, DWORD	_dwNameHashCode);		// 비트갯수
//┃CActorInfo.cpp
//┃정보
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃버프 상태 체크
//┃checkBuffActor.cpp
	int						getBuffedEffectCount(cAbility *_lpAbility);
	BOOL					setInverseBuff(float _fValuePercent, int _iKeepTime);	// 역버프..
	BOOL					isBuffed(CSkillExtraEffect	*_lpExtraEffect,cAbility *_lpAbility);
	BOOL					isLightAdditionalEffect();
//┃checkBuffActor.cpp
//┃버프 상태 체크
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 이상상태
//┃CActorWrongStatus.cpp
	BOOL					isStrangeStatus();					//	상태 이상 상태
	BOOL					isDeclinePowerStatus();				//	능력치 저하 상태
	BOOL					isCurseStatus();					//	저주 상태
	BOOL					isWrongStatus();					//	뭔가 이상한 상태

	void					cureStrangeStatus(int _iEfficient=100);				//	이상상태 치료
	void					cureDeclinePowerStatus();			//	저하계 치료
	void					cureCurseStatus();					//	저주 치료
	void					cureWrongStatus();					//	상태이상 치료

	void					reduceStrangeStatusTime(int _iTime);
	void					reduceCurseTime(int _iTime);
	void					reduceDeclinePowerStatusTime(int _iTime);
	void					reduceWrongStatusTime(int _iTime);					//	상태이상 치료

	void					dispell();
	BOOL					revive(int _iRestoreTimePercentage=0,int _iRestoreHP=0,BOOL _bIsHealByValue=FALSE);			//	부활
	BOOL					perfectReviveAllPartyMember(int _iUpkeepTime,int _iIncreaseRate,cItem *_lpItem,cACTOR *_lpCaster);

	void					animatePartner(int _iTime,int _iReviveTime);

	BOOL					cure(cACTOR *_lpCaster,cAbility *_lpAbility,int _iHealHP,int _iRestoreExpPercentage=0);//	스킬에 의한 치료
//┃CActorWrongStatus.cpp
//┃각종 이상상태
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃이벤트
//┃CActorEvent.cpp
	void					transToEventMob(int _iEvent,int _iEventMob);
	void					sprinkleEventGift(int _iCount);
	void					dropItemForEventMob();

	BOOL					eventWith(cACTOR *_lpAsker);	//	이벤트
	void					closeTalk();
	BOOL					selectSpeech(cACTOR *_lpAsker,int _iConversationIndex,int _iSelectSpeech,BOOL _bIsInnerSelect=FALSE);	//	대사 선택
	void					transToMonster(WORD	_wKind,WORD _wLevel, WORD _wTime);
	int						summonMonster(int _iValue);
//┃CActorEvent.cpp
//┃이벤트
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃공격/타격/액션등등..
//┃CActorBattle.cpp
	CContinuousAttackedPeneltyInfo*		getContinuousAttackedPenelty(int _iCount);
	float					GetDamageFactorByJob();				//결투장에서 직업별로 데미지 펙터값을 구한다.
	BOOL					setTarget(cACTOR *_lpTarget,CSkill *_lpSkill=NULL);	//	타겟 설정
	void					setUpkeepTargetTime(int _iTime);
	void					setEngageAbility(cAbility *_lpAbility,cAbility *_lpSubAbility=NULL);
	void					setCurrentTarget(cACTOR *_lpCurrentTarget);
	BOOL					setFirstBeater(int _iSerial);
	void					addEngageTarget(int _iSerial);
	void					setActionSpeed(cAbility *_lpAbility);//fps와 현재 액션이 끝나는데 필요한 프레임을 얻어서 설정한다.
	int						actionResult(CSkill	*_lpSkill);

	int						getAttackRange(cAbility *_lpAbility);
	int						getHitRange(cAbility *_lpAbility, BOOL _bIsUseWeapon = FALSE);	//	타격 범위
	int						getDamageFrame(int _iTimes=-1);
	int						getFirstTriggerFrame();
	int						getTriggerFrame();
	DWORD					getTriggerTime();
	DWORD					getFirstTriggerTime();
	DWORD					getDamageFrameTime(int _iTimes=-1);
	DWORD					getFrameTime(int _iTimes=-1);

	inline	BOOL			operateBookedWork();	//	예약된 작업들 실행
	void					operatePlayerBattle();	//	플레이어 전투 관련 업데이트
	
	BOOL					revenge(cACTOR *_lpAttacker);//	복수-o-!!
	BOOL					operateDoubleLink(cACTOR *_lpAttacker);
	BOOL					removeTargeting();				//타켓팅을 푼다.

	BOOL					engageBattle(cACTOR *_lpTarget,cAbility *_lpAbility);
	void					stopBattle();//전투중지

	int						attackToObject(int _iObject,int _iSerial);
	int						attackToActor(cACTOR *_lpTarget,cAbility *_lpAbbility);//공격
	int						attackToActorByContinuousHitSkill(cACTOR *_lpTarget,cAbility *_lpAbility);
	int						attackToActorByIllusionAttackSkill(cACTOR *_lpTarget,cAbility *_lpAbility);

	int						attackToActorByBasicAttackSkill(cACTOR *_lpTarget,cAbility *_lpAbility);//기본공격 스킬로 적을 공격한다.
	int						actionToGround(int _iX,int _iY,cAbility *_lpAbbility);//땅에다 대고 공격!! -o-!!
	int						actionQuick(cAbility *_lpAbbility);//퀵 스킬	
	int						actionToObject(cAbility *_lpAbility,int _iObjectType,int _iObjectIndex);

//━━액션/전투 예약
	inline	void			bookingAction(CG_ACTION_TO_ACTOR *_lpPacket)		{m_bookedAction.setToActorAction(_lpPacket);}
	inline	void			bookingQuickAction(CG_ACTION_QUICK *_lpPacket)		{m_bookedAction.setQuickAction(_lpPacket);}
	inline	void			bookingGroundAction(CG_ACTION_TO_GROUND *_lpPacket)	{m_bookedAction.setToGroundAction(_lpPacket);}
	inline	void			bookingBattle(int _iTarget,cAbility *_lpAbility)	{m_bookedBattle.set(_iTarget,_lpAbility);}
	void					emptyBookedAction();
	void					emptyBookedBattle();

	void					action(int _iAction,BOOL _bIsNotChangeFPS=FALSE);//특정 액션 취함
	void					hitAction();//타격 액션을 취한다.
	inline	void			operateBookedHit();//	예약된 타격-_- 실행

	BOOL					isTargetInAttackRange(cACTOR *_lpTarget,int _iRange,int _iMinRange=0);//공격 가능한 거리냐?
	BOOL					isInAttackRange(int _iX,int _iY,int _iRange,int _iMinRange=0);//_iX,_iY부터 _iRange거리 안에 있냐?
	BOOL					hitByHitSkill(cACTOR *_lpAttacker,int _iSkill,int _iLevel,int _iDamage);//맞았다!!
	BOOL					isAttackToManyPersonSkill(cAbility* _lpAbility);	//다팅 피버같이 여렀을 때리는 스킬을 판정한다. 노가다로...

	int						getPhysicalAttackPower(BOOL _bIsWantMaxPower);
	int						getPhysicalAttackPower(cAbility *_lpAbility,int _iLevel,BOOL _bIsPVP,BOOL _bIsBossBattle,CHitInfo *_lpHitInfo,BOOL _bIsWantMinDamage,cACTOR *_lpTarget);//공격력을 구한다.
	int						getMagicAttackPower(cAbility *_lpAbility);

	int						getWeaponDamage(int _iLevel,BOOL _bIsPVP,BOOL _bIsBossBattle,CHitInfo *_lpHitInfo,BOOL _bIsWantMinDamage);
	int						getDefensivePower();//방어력을 구한다.
	inline	BOOL			isHit(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,int _iContinuousAttackIndex);//물리공격 히트판정
	void					operateHitTimeActiveSkill(cACTOR *_lpAttacker,BOOL _bIsHitPhysicalDamage,CHitInfo *_lpHitInfo,cAbility *_lpAbility);

	void					getAdditionalMagicDamage(cACTOR *_lpAttacker,CHitInfo *_lpHitInfo);
	void					getAdditionalMagicDamageToShakle(cACTOR *_lpAttacker,CHitInfo *_lpHitInfo);

	inline	int				getCorrectDamageValue(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iX,int _iY);
	void					correctDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iX,int _iY,BOOL _bIsPlayerVsPlayer,BOOL _bNEP,CHitInfo *_lpHitInfo,int _iContinuousAttackCount);
	void					setLastDamage(cACTOR* _lpAttacker,CHitInfo* _lpHitInfo);

	void					getDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iX,int _iY,CHitInfo *_lpHitInfo,BOOL _bIsApplyPhysicalDamage=TRUE,BOOL _bIsApplyMagicDamage=TRUE,BOOL _bIsApplyAdditionalMagicDamage=TRUE,int _iContinuousAttackCount=1,int _iContinuousAttackIndex=0, BOOL _bIsNormalHit = FALSE);			//	데미지
	void					getDamageToAttackShakle(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,BOOL _bIsApplyPhysicalDamage=TRUE,BOOL _bIsApplyMagicDamage=TRUE,BOOL _bIsApplyAdditionalMagicDamage=TRUE,int _iContinuousAttackCount=1,int _iContinuousAttackIndex=0);			//	데미지

	void					getMachineShotDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo);			//	데미지
	float					getCorrectIntelligenceFactorForMagicDamage(CSkill *_lpSkill=NULL,int _iIntelligence=-1);
	void					getMagicDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo* _lpHitResult);	//	마법 데미지
	void					getMagicDamageForEE(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,
											int _iFireDamage,int _iWaterDamage,int _iWindDamage,int _iEarthDamage,int _iLightDamage,int _iDarkDamage,int _iReduceResistance=0);
	BOOL					getPhysicalDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo* _lpHitResult,int _iContinuousAttackIndex);//	물리 데미지
	void					getMagicDamageToShakle(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo);
	BOOL					getPhysicalDamageToShakle(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,int _iContinuousAttackIndex);

	BOOL					hitBookedDamage(int _iTimes=0);//예약된 데미지-_-를 입힌다.
	void					operateAdditionalAttackEffect(cACTOR *_lpAttacker);
	void					operateCounterEffect(cACTOR *_lpAttacker);

	void					crushMagicBox(cACTOR *_lpAttacker);
	void					attackToShakle(cACTOR *_lpAttacker,int _iPhysicalDamage,int _iMagicDamage,cAbility *_lpAbility);
	BOOL					damagedBy(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iPhysicalDamage,int _iMagicDamage,BOOL _bIsApplyReturnDamage=FALSE,BOOL _bIsApplyAdditionalEffect=TRUE,int _iX=0,int _iY=0,CHitInfo *_lpHitInfo=NULL,BOOL _bIsCounter=FALSE);//누가 데미지를 입혔다.
	void					getContinuoueAttackDamage(cAbility *_lpAbility,cACTOR *_lpTarget,int _iShotCount,int *_lpiPhisycalDamage,int *_lpiMagicalDamage,int *_lpiMissCount,int *_lpiCriticalCount,int *_lpiDoubleCriticalCount,int *_lpiMinPhysicalDamage,int *_lpiMinMagicDamage,int *_lpiBlockingCount,int *_lpiInstanceKillCount,int *_lpiNoActionBlockCount,int *_lpiDancingBlockCount, int* _lpiHardBlowCount);

	void					operateDamageReturn(int _iDamage, cACTOR *_lpAttacker, BOOL _bIsReturnMissileDamage = FALSE);
	void					strikeIcyStalagmite(cAbility *_lpAbility,int _iDamage,int _iPhysicalDamage,cACTOR *_lpAttacker);
	void					operateHitNpcReaction(cACTOR *_lpAttacker,int _iReduceHP);
	void					operateTauntSkill(cACTOR *_lpAttacker,int _iDamagePercent,int _iUpkeepTime);
	void					operatePetTauntSkill(cACTOR *_lpAttacker,int _iUpkeepTime);
	void					operateRevenge(cACTOR *_lpAttacker,cAbility *_lpAbility,BOOL _bIsForecedRevenge=FALSE);

	BOOL					strike(cACTOR *_lpTarget,CHitInfo *_lpHitInfo,cAbility *_lpAbility=NULL,BOOL _bIsApplyAdditionalEffect=TRUE,BOOL _bIsApplyLifeSteal=TRUE,int _iX=0,int _iY=0,BOOL _bIsCounter=FALSE);

	int						blocking(cACTOR *_lpAttacker,cAbility *_lpAbility);//방패로 막을 수 있냐?
	int						getBlockingChance();
	int						getFirstTriggerFrame(int _iAnm);
	int						getDamageFrameInAnm(int _iAnm, int _iTimes=-1);
//┃CActorBattle.cpp
//┃공격/타격/액션등등..
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃오브젝트를 공격함
//┃actor_attack_object.cpp
	void					hitObject();
//┃actor_attack_object.cpp
//┃오브젝트를 공격함
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃이동 관련
//┃CActorMove.cpp
	BOOL					moveToArea(int _iArea,BOOL _bIsToAxis=FALSE,int _iMethod=0);
	BOOL					moveTo(int _iX,int _iY,int _iCorrectSpeed=100,int _iPPS=0);			//	_iX,_iY로 이동
	BOOL					moveDestPos();

	inline	BOOL			makePath(int _iX,int _iY);			//	_iX,_iY까지 패쓰 만들기
	inline	BOOL			findOtherWay(int _iX,int _iY);		//	_iX,_iY까지 갈 수 있는 다른 길이 있나 알아 본다.

// 	inline	BOOL			makePathOther(int _iX,int _iY);
// 	BOOL					getNextPosByDirect(c4POINT* _posNext , int _iDirect);

	int						movePlayer(int _iX,int _iY,int _iDestX,int iDestY);	//	플레이어가 _iX,_iY에서 _iDestX,iDestY로 이동했다.
	BOOL					isSafeRoad(int _iDestX,int _iDestY);

	int						getWalkSpeed();
	void					setMoveAbility(BOOL _isRunning);	//	걷기 뛰기 전환
	void					setMoveSpeed(BOOL _bIsSendToPlayer=FALSE);
	
	int						updateMove(BOOL _bIsSendStopPacket=TRUE);						//	이동관련 업데이트
	int						updateMoveOnlyPos();
	inline void				getNextPos(c4POINT *_lpPos);		//	다음 위치 계산

	BOOL					stop(int _iX,int _iY,BOOL _bIsCheckRange=TRUE,BOOL _bIsStop=TRUE);	//	정지
	inline	BOOL			isBlockedWay(int _iMx,int _iMy);		//	_iMx,_iMy으로 이동하는데 문제가 있냐?

	BOOL					getRandomNearPlace(cPOINT *_lpPos,int _iPartRange,int _iAngle=0xffff);
	void					warpToTargetNearPlace(cACTOR *_lpActor);
	void					resetBlockedMoveStress();
	void					increaseBlockedMoveStress(int _iMX,int _iMY);
	BOOL					isMoveBlockOvercrowding();

	BOOL					checkIsNonTarget(cACTOR *_lpTarget);
	
	BOOL					checkMoveCounter(int _iX,int _iY,char *_lpstrWhere);
	BOOL					checkMoveReport(int _iX,int _iY);
//┃CActorMove.cpp
//┃이동 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Skill AI
//┃actorSkillAI.cpp
	
	BOOL					checkSACS_InSightOwnerForceHP(cSkillAiPatternInfo	*lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_RangeToTarget(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_SelfBuffed(cAbility	*_lpAbility,cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_TargetBuffed(cAbility	*_lpAbility,cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_OwnerForceCountInSight(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_EnemyForceCountInSight(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_EnemyForceCountInTargetAround(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_DeathOwnerForceInSight(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_OnBattle(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget,BOOL _bIsFirst);
	BOOL					checkSACS_EnableCurseAmplify(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_TargetIsPet(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_ActiveChance(cAbility *_lpAbility,cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_CurrentHP(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_Attacked(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_CheckTrapCount(cSkillAiPatternInfo	*_lpInfo);
	BOOL					checkSACS_CheckStatus(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget);
	BOOL					checkSACS_CheckSwallowCorpesCount(cSkillAiPatternInfo	*_lpInfo);
	BOOL					checkSACS_AnimatePartnerCount();

	BOOL					findEngageAbility(cACTOR *_lpTarget,BOOL _bIsFirst=FALSE);	//	교전 스킬 찾기
	void					sortAbility();
	void					setMonsterReaction(BOOL _bIsOn=TRUE);
	BOOL					checkIsSuitableSkill(cAbility *_lpAbility,cACTOR *_lpTarget,BOOL _bIsFirst);
	
	void					operateMonsterCounterSkill(cACTOR *_lpAttacker);

	void					findAiTargetList(cAbility *_lpAbility,cACTOR *_lpTarget);
	void					resetAiTargetList();
	BOOL					addAiTargetList(int _iSerial);
	BOOL					orderingAI_TargetList();

//┃actorSkillAI.cpp
//┃Skill AI
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃세이지 마스터 AI
//┃actor_ai_sage_master.cpp
	BOOL					operateSageMasterTraceAi();
	BOOL					operateSageMasterIronBall();
	BOOL					sageMasterPursuit();	
//┃actor_ai_sage_master.cpp
//┃세이지 마스터 AI
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃AI
//┃CActorAi.cpp
	cACTOR*					findNeareastEnemy();
	BOOL					pursuit(cACTOR *_lpTarget);			//	_iTargetActor을 추적한다.

	BOOL					findEnemy();
	BOOL					reaimTarget(cACTOR *_lpActor=NULL);	//	_lpActor를 제외하고 주변에 타겟을 찾는다.
	void					stopBattleForNPC(BOOL _bIsRequireSendPacket);

	BOOL					operateGuildGuardianAI();
	BOOL					operateAi();
	BOOL					operateReadyAi();
	BOOL					operateMoveAi();
	BOOL					operatePatrolAi();
	BOOL					operateApproachAi();
	BOOL					operateFightAi(BOOL _bIsFirst=FALSE);
	BOOL					operateConfuseAi();	//	혼란 상태를 처리한다.
	BOOL					operateTraceGuide();
	BOOL					operateComeBackToRegenPlaceAi();
	BOOL					traceGuide();

	void					setAiType(int _iType);
	void					setAiState(int _iState);

	void					setAiReady();//AI를 대기중으로 설정
	void					setAiMove();//AI를 이동으로 설정
	void					setAiApproach(cACTOR *_lpTarget,int _iX=0,int _iY=0);//AI를 접근*-_-*으로 설정
	void					setAiTrace(cACTOR *_lpTarget);//AI를 추적-o-으로 설정.
	void					setAiFight(cACTOR *_lpTarget,cAbility *_lpEngageAbility=NULL,int _iX=0,int _iY=0);//AI를 싸움-_-!!으로 설정.
	void					flee(cACTOR *_lpAttacker,int _iUpkeepTime);
//┃AI
//┃CActorAi.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃아이템 로그 전송
//┃CActorSendLog.cpp
	void					updateGoldLog(LONGLONG _llGettingGold,int _iMethod);
	void					sendGoldLog(LONGLONG _llOwnGold);
	void					sendBankGoldLog(LONGLONG _llOwnGold);

	void					sendReleaseMiniPetLog(cMiniPet *_lpMiniPet,int _iWork=-1);
	void					sendFeedItemMiniPetLog(cItem *_lpItem,cMiniPet *_lpMiniPet,int _iIndex);
	void					sendCreateMiniPetLog(cItem *_lpItem,cMiniPet *_lpMiniPet,int _iIndex);
	void					sendSealMiniPetLog(cMiniPet *_lpMiniPet,int _iIndex);
	void					sendUnsealMiniPetLog(cMiniPet *_lpMiniPet,int _iIndex);
	void					sendExtendMiniPetTimeLog(cMiniPet *_lpMiniPet,int _iIndex);
	void					sendRenewItemLog(cItem *_lpItem,int _iTerm);
	void					sendCreateItemLog(cItem *_lpItem,int _iMethod,char *_lpstrGuildName=NULL);
	void					sendUseItemLog(cItem *_lpItem);
	void					sendRemoveItemLog(cItem *_lpItem,int _iMethod,int _iField=-1);
	void					sendPickItemLog(cItem *_lpItem,int _iMethod=0);
	void					sendCreatePremiumItemLog(cItem *_lpItem,int _iMethod,int _iCarrotPrice=0);
	void					sendSellItemLog(cItem *_lpItem,DWORD _dwPrice);	//	아이템 판매 결과
	void					sendBuyItemLog(cItem *_lpItem,DWORD _dwPrice,int _iCount,int _iWhere, WORD _wTokkenItem = 0xffff);	//	아이템 판매 결과	
	void					sendTransAvatarLog(char *_lpstrTransName);	
	void					sendDropItemLog(cItem *_lpItem,int _iMethod=0);
	void					sendExpiredItemLog(cItem *_lpItem);
	void					sendRepairItemLog(cItem *_lpItem,int _iCharge=0);
	void					sendDoubleItemLog(cItem *_lpItem,int _iWhere);
	void					sendEnchantItemLog(cItem *_lpEnchanted,int _iWork,DWORD _dwPrice=0);
	void					sendUpgradeItemLog(int _iWork, cItem *_lpTargetItem,WORD _wUpgradeItemSerial, DWORD _dwGold, WORD _wStoneOfMesteryCount, WORD  _wTantillessCount, WORD _wCrystalCount, WORD _wUpgradeMaterialsItemSerial, WORD _wUpgradePremiumMaterialsItemSerial);		//  업그레이드 아이템~~!!! 09.09.01
	void					sendPitchmanShopTradeLog(cItem *_lpItem,cACTOR *_lpOwner,DWORD _dwPrice,BOOL _bIsByOperator, BYTE _bDenomination);		// 09.08.21
	void					sendPetWorkLog(int _iPetIndex,int _iWork);
	void					sendRestraintMiniPetLog(cMiniPet *_lpMiniPet);
	void					sendRebirthLog(int _iPreLevel,int _iPreExp);
	void					sendSaveLog();
	void					sendHackingLog(int _iReason,int _iValue=0);
	void					sendHackingLog(int _iReason,char *_lpstrReason);
	void					sendTresureMapLog(int _iEvent);	
	void					sendBackServerLog(DWORD _dwCurrentSaveCount,DWORD _dwOldSaveCount);	

	void					sendOperatorChatLog(int _iType,BOOL _bIsListen,char *_lpstrTarget,char *_lpstrText);
	void					sendRemovePlaceRelatedOfGuild(char *_lpstrGuildName,int _iField);
	void					sendLogPlayerLogInOut(BOOL isLogOut = FALSE);
	void					sendLogstatisticsQuest(int _iQuest , int _isStartQuest);
	void					sendCompleteQuest(int _iType,int _iQuest);
	void					sendReceiveQuestRewardExp(int _iRewardExp,int _iRealExp,int _iQuestIndex);
	void					sendWarningQuestExpSerialReceive(int _iRewardExp,int _iQuestIndex);

	void					sendDisconnectDuelFieldLog();

	BOOL					isRequireSendSaveLog();
	void					sendDivideItemLog(cItem *_lpTargetItem,WORD _wBeforDragCount,WORD _wAfterDragCount,WORD _wFocusCount );	// 스택나누기.
	void					sendCarveItemLog(int _iWork, cItem *_lpTargetItem,WORD _wUseItemSerial, WORD _bf8BeforCarvingLevel,WORD _bf8AfterCarvingLevel, DWORD _dwGold,  WORD _wCrystalCount)	;
//┃아이템 로그 전송
//┃CActorSendLog.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃ 패킷 전송
//┃CActorSend.cpp
	void					sendDuelMsg(int _iMsg,int _iOutputType=eSM_SYSTEM,char *_lpstrText=NULL);
	void					sendDuelRanking(int _iType);
	void					sendDuelRecord();
	void					sendBannerText(int _iSerial,char *_lpstrText);
	void					sendBuyCarrotShopItemResult(int _iResult,int _iRemainCarrot,cItem *_lpItem=NULL);
	void					sendSecretDungeonMessage(int _iMessage,int _iValue1=0,int _iValue2=0);
	void					sendFieldTrapWork(int _iTrapIndex,int _iWork,int _iSpareValue=0xffff,BOOL _bIsSendField=FALSE);
	void					addSendCloseRangeFieldTrapWork(int _iTrapIndex,int _iWork,int _iSpareValue=0xffff,char *_lpstrName=NULL);
	void					addSendHitChainedDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iShotCount,int _iMissCount,int _iGap,
													int _iPhysicalDamage,int _iMagicDamage,int _iMinPhysicalDamage,int _iMinMagicDamage,
													int _iCriticalCount,int _iDoubleCriticalCount,int _iInstanceKillCount,
													int _iBlockingCount,int _iNoActionBlockCount,int _iDancingBlockCount, int _iHardBlowCount);
	void					sendArcaWork(int _iArcaIndex,int _iResult,int _iSpareValue=0xffff,BOOL _bIsSendField=FALSE);
	void					addSendCloseRangeArcaWork(int _iArcaIndex,int _iResult,int _iSpareValue=0xffff,char *_lpstrName=NULL);
	void					sendItemPackName(int _iPackIndex);
	void					sendAskRemoveFriend(char *_lpstrClient);
	void					sendAskRemoveFriendResult(char *_lpstrTarget,int _iResult);

	void					sendMoveFailed(int _iReason);
	void					sendAccountInfo();
	void					addSendMove(int _iX,int _iY,int _iDX,int _iDY,BOOL _isContinue);	//	이동 메시지 전송
	void					addSendMoveToParty(int _iX,int _iY,int _iDX,int _iDY);	//	이동 메시지 전송
	void					addNpcMoveMessage(BOOL _isContinue);	//	이동 메시지 전송
	void					addPacketStop();			//	멈췄다.
	void					addPacketMiss(int _iSkill);			//	미스
	void					sendScreenEvent(int _iX , int _iY , int _iTime , int _iWaitTime);	//JBC	스크린 이동 이벤트를 보낸다.08-11-06

	void					sendRedistribute(int _iWork);
	void					sendStopBattle(BOOL _bIsReleaseFocus=FALSE);
	void					sendOpenRepairWindow(int _iNpcSerial);
	void					sendOpenDisjointingWindow(int _iNpcSerial);
	BOOL					sendQuestionForRecoverDeathPenelty();
	void					sendPartyMessage(int _iMessage);
	void					sendRemoveActor(int _iSerial,BOOL _bIsHide=FALSE);
	void					sendGGAuthCode(void *_lpData);

	void					sendPlayerInfo(cACTOR *_lpActor);
	void					sendSuccesToJoin();
	void					sendSettingSeasonVariable();

	void					sendEtcWorkToPartyMember(BOOL _bIsSendSameField,int _iWork,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0);
	void					sendEtcWork(int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6);
	void					sendEtcWork(int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5);
	void					sendEtcWork(int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4);
	void					sendEtcWork(int _iWork,int _iValue1,int _iValue2,int _iValue3);
	void					sendEtcWork(int _iWork,int _iValue1,int _iValue2);
	void					sendEtcWork(int _iWork,int _iValue1);
	void					sendEtcWork(int _iWork);
	void					sendPlaySkillSound(int _iSkill,int _iSound);
	void					sendItemWorkResult(int _iResult);
	void					sendCorrectActorPos(int _iX,int _iY,BOOL _bIsInnerTeleport=FALSE);
	void					sendUseSkillResult(int _iResult);	//	스킬 사용 결과 
	void					sendCurrentPosition(BOOL _bIsInnerTeleport=FALSE);		//	현재 위치를 전송함
	void					sendRidingDog(BOOL _bIsRide,int _iGrade=0,int _iX=0,int _iY=0);
	void					sendPlantGuardianPost(int _iRegSerial,int _iX,int _iY,int _iImage,int _iSkill,int _iLevel);	//	가디언 포스트를 심는다.
	void					sendStrikeGuardianPostAttack(int _iRegSerial,int _iX,int _iY,int _iImage,CDamageActorInfoTrailer *_lpInfo,int _iSkill,int _iLevel);	//	가디언 포스트 공격을 한다.
	void					sendPullUpGuardianPost(int _iRegSerial);	//	가디언 포스트를 뽑아내다.
	void					addSendTalk(char *_lpstrTalk,int _iChatType, WORD _wSendTarget = 0xffff);
	void					addSendSimpleHit(int _iPhysicalDamage,int _iMagicDamage,WORD _wAddHitType);
	void					addSendFirstAidHP();
	void					addSendHitDamage(cACTOR *_lpCaster,int _iPhysicalDamage,int _iMagicDamage,DWORD _dwResultField,int _iSkill);
	void					addSendSkillEffect(cAbility *_lpAbility,int _iImage=0);
	void					addSendCureByPotion(int _iBaseItem,int _iHealHP);

	void					sendDeathPeneltySecond(BOOL _bIsDeathCurrentTime=FALSE,BOOL _bIsRecoverDeathPeleltyTime=FALSE);
	void					sendUseOrbResult(int _iResult,int _iSlot=-1);
	void					sendMoveFieldResult(int _iResult,char *_lpstrAddress=NULL,char *_lpstrMapname=NULL,int _iInstanceFieldFloor=0xffff,int _iFieldSerial=0xffff,int _iGuild=0xffff);
	void					sendMoveFieldNow();
	void					sendLoadFailedCartPremiumItem(cItem *_lpItem,int _iInventorySlot);
	void					sendDestroyCartItemResult(int _iResult,int _iCartSlot);

	void					sendExpiredItem(int _iSlot,int _iItem);
	void					sendItemData();	//	아이템 데이터 전송. 아이템 싱크가 어긋 났을때 전송 시켜준다.
	void					sendWithdrawItemFromCart(int _iInventorySlot,int _iCartSlot);
	void					sendRemoveCartItem(int _iCartSlot);
	void					sendGameClose(char *_lpstrCause);

	void					sendPickPartyItemMessage(char *_lpstrName,cItem *_lpItem,BOOL _bIsSharingGold);
		
	void					sendUpdateBoostCount();
	void					sendUpdateGuildLevel(int _iLevel,DWORD _dwExp,int _iGuildPoint,int _iExpTaxRate);
	void					sendGuildInfo(BOOL _bIsIgnoreSkill);	//	길드 정보 전송
	void					sendSimpleGuildInfo();
	void					sendGuildBattleInfoForMember(BOOL _bIsByClientRequest=FALSE);	//	길드 멤버들을 위한 길드전 정보
	void					sendJoinGuildResult(int _iResult);
	BOOL					sendReadyToMakeGuild();
	BOOL					sendOpenGuildBattleScheduler();	//	길드전 스케쥴러 열기
	BOOL					sendOpenGuildMarkComposer();	//	길드 마크 조합 인터페이스 오픈

	void					sendChangeGuildBattleSchedule(int _iTimeOrder,int _iFieldOrder,CGuildBattleInfo *_lpInfo);	//	길드전 스케쥴 변경됨
	void					sendUpdateGuildBattleInfo(CGuildBattleInfoForMember *_lpToday,CGuildBattleInfoForMember *_lpTmorrow);	//	길드전 스케쥴 변경된걸 길드 멤버에게 전송


	void					sendChangeGuildTaxRate(int _iTax);
	void					sendChangeGuildNotice(char *_lpstrNotice);
	void					sendChangeGuildBattleCountAtThisWeek(int _iCount);	//	이번주에 싸운 횟수 업데이트
	void					sendChangeRankInGuild(int _iRank);
	void					sendUpdateGuildSkillLevel(int _iSkill,int _iLevel,int _iRemainSkillPoint);
	void					sendUpdateGuildBattleCountAtThisWeek(int _iCount);	//	이번주에 길드전 횟수 업데이트

	void					sendMoveState();
	void					sendHitAction();

	void					sendUpdateExp(BOOL _bIsRestore=FALSE);	//	경험치 얻었다.
	void					sendUpdateItemData(int _iSlot,BOOL _bIsWantTwinkle=FALSE);	//	특정 아이템 데이터 업데이트
	void					sendUpdateItemDurability(int _iPart);
	void					sendUpdateWayPointInfo(int _iField,int _iWaypointSlot,int _iXPos,int _iYPos);
	void					sendEndDialg();	//	다얄로그 종료 메시지 전송
	void					sendItemUniqueSerial(int _iSlot,DWORD _dwSerial);

	BOOL					sendSpeech(cACTOR *_lpAsker,CSpeech *_lpSpeech);
	void					sendChargeCP(int _iSkill);				//	CP 충전 메시지

	void					sendQuestChangeInfo(int _iProcessIndex,BOOL _bIsForQuestComplete=FALSE);
	void					sendQuestInfo();			//	퀘스트 정보 전송

	void					sendDecreaseItemCount(int _iSlot,int _iCount);
	void					sendAddItem(cItem *_lpItem,int _iReason=eIAR_NORMAL);//	아이템 추가
	void					sendRemoveItem(int _iItem,int _iCount,BOOL _bIsSlot=FALSE,int _iReason=eRIR_NORMAL,BOOL _bIsIncludeEquipment=FALSE);//	아이템 제거
	void					sendAddGold(int _iGold,int _iReason=eAGR_NORMAL);

	void					sendTransformation(int _iReason=eTR_BY_SKILL,BOOL _bIsWantOnlyAfterMotion=FALSE);	//	변신한다.. -o-
	void					sendAppearanceInfo();	//	외관정보 전송
	void					sendReloadBeltItem();	//	아이템을 리로드 했다.
	void					sendRemainHP(cACTOR *_lpReceiver);

	void					addSendActorMagicEffectStatus(BOOL _isByGGG = FALSE , BOOL _isEnd = FALSE);
	void					sendTeleport(int _iSkill,int _iMethod);
	void					sendApplySkillExtraEffect(int _iCaster,cAbility *_lpAbility, DWORD _dwKeepTime);		// 강제시간변수 추가~09.10.14
	void					sendInstanceKill();
	void					sendFindUserResult(char *_lpstrTargetName,int _iResult,int _iFieldSerial,int _iJob);

	void					sendServerMessage(char *_lpMessage,int _iType=eSM_SYSTEM);
	void					sendBlockedMessage();
	void					sendKillMessage(BOOL _bIsInstanceKill=FALSE,BOOL _bIsReleaseSummon=FALSE);
	void					sendGetTitleMessage(int _iIndex,int _iTitle,int _iTitleLevel);
	void					sendGuildWorkMessage(int _iMessage,int _iValue1=0,int _iValue2=0,char *_lpstrName=NULL);
	void					sendDissolutionGuildMessage();
	void					sendCartMessage(int _iMessage);
	int						sendPitchmanShopMessage(int _iMessage);
	void					sendRemoveTitleMessage(int _iIndex,int _iTitle,int _iTitleLevel);
	void					sendEventMessage(char *_lpstrText,BOOL _bIsIncludePartyMember,int _iChatType, int _iTime = 30, BOOL _bEffectflickeringly = TRUE);
	void					sendRegistServerMessage(int _iMessage=eRSM_DUMMY,int _iType=eSM_MSG_BOX);
	void					sendDissolutionPartyMessage(int _iCause=eDPC_NORMAL);
	void					sendUnionPartyDissolution(int _iCause=dPW_DUP_BY_UNKNOWN);		//연합 파티가 깨졌다고 통보해 준다.	
	void					sendDisplacement(cACTOR *_lpTarget,BOOL _bIsDisplacement);
	void					addSendVerySimpleInfo(cACTOR *_lpReceiver=NULL);
	void					addSendSimpleInfo(cACTOR *_lpReceiver=NULL,BOOL _bIsSummonBeastPowerUp=FALSE,BOOL _bIsSyncHeroInfo=FALSE);
	void					addSendRevive();
	void					addSendPlayerBasicInformation();
	void					sendPlayerBasicInformation();

	void					sendSummonBeastInfo(int _iIndex,BOOL _bIsUpgrade);

	void					sendUpdatePetExp(int _iPetIndex);
	void					sendRemovePet(int _iPetIndex,BOOL _bIsMiniPet=FALSE,BOOL _bIsRequireMessage=TRUE);
	void					sendPetInfo(int _iPetIndex,int _iPetFieldSerial,BOOL _bIsTame=TRUE);
	void					sendChangePetName(int _iPetIndex,char *_lpstrName,BOOL _bIsMiniPet=FALSE,int _iResetItemIndex=-1);
	void					sendMakeBreedingRecordBook(int _iPetIndex);
	void					sendSyncPetInfo(int _iPetIndex,int _iHP,int _iMaxHP,int _iMaxAttackPower,int _iMinAttackPower,int _iDefensePower,int _iStrength,int _iIntelligence,BOOL _bIsSummonBeast);
	void					sendChangeTamerCommand(int _iPetIndex,int _iCommand,cACTOR *_lpTarget,int _iX,int _iY);
	void					sendPetWaitCommand(int _iPetIndex);

	void					sendFailedBuyItem(int _iReason);
	void					sendDoorMessage(int _iArea,int _iKey,int _iSkill,int _iMessage,char *_lpstrCaster=NULL);	//	문 관련 메시지

	BOOL					isTooFastSendBigSizeData();
	void					setLastTimeSendBigSizeData(DWORD _dwTime);

	void					sendOpenBadgeOfTraderShop();		// 거래상인의 배지 상점
//┃CActorSend.cpp
//┃ 패킷 전송
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃ 데이터 체크
//┃CActorCorrectData.cpp
	int						getCurrentStatePointSum();
	int						getCorrectStatePointSum();

	int						checkData(BOOL _bIsCheckSerial);	//	무결성 검사
	int						checkData(CClient *_lpClient,char *_lpWhere,BOOL _bIsCheckSerial);	//	무결성 검사
	int						correctSkill();
//┃CActorCorrectData.cpp
//┃ 데이터 체크
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃데이터 저장
//┃CActorSave.cpp
	BOOL					saveData(BOOL _bIsBackUp=FALSE);
	BOOL					checkClient(CClient *_lpClient,char *_lpstrLog);	//	클라이언트에 문제가 없나 확인한다.

	void					saveToFile();
	void					loadFromFile();

	void					immediatelySendSaveDataToDBMemory(BOOL _bIsRequireLog=FALSE);	//	DB 메모리에 세이브 데이터 전송
	BOOL					sendSaveDataToDBCache();	//	
	BOOL					sendUrgentSaveDataToDBCache(BOOL _bIsForJoinFailed=FALSE);	
	BOOL					sendSaveDataToDBFile(BOOL _bIsTemporaryBackup=FALSE,BOOL _bIsRequireSendLog=TRUE);
	BOOL					saveDataForMoveField(CClient *_lpClient=NULL);	//	필드 이동을 위해 데이터를 저장한다.
	BOOL					saveDataForTerminateServer();	//	서버 종료할때 데이터 저장
//┃CActorSave.cpp
//┃데이터 저장
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃정적 메소드
//┃
	static	void			Init();						//	최초의 초기화
//┃
//┃정적 메소드
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};



inline	int	GetDirect(int _iAngle,int _iDirectCount)
{
	_iAngle		=	_iAngle + 360/_iDirectCount/2;
	
	if (_iAngle	>= 360) _iAngle-= 360;
	_iAngle		=	_iAngle*_iDirectCount/360;

	if (_iAngle	<=	_iDirectCount/4)	return	_iDirectCount/4-_iAngle;
	
	return	_iDirectCount-(_iAngle-_iDirectCount/4);
}

inline	int	GetRootDamage(int _iDamage)
{
	double		dbDamage1=	_iDamage/100;
	double		dbDamage2=	_iDamage%100;

	dbDamage1	=	sqrt(dbDamage1);
	dbDamage2	=	sqrt(dbDamage2);

	return	(int)(dbDamage1*100+dbDamage2);
}

int	GetP2PCorrectDamage(int _iDamage,int _iCount=1 , cACTOR* _lpAttacker = NULL);

extern	int		g_aExpTable[2000]; //20240904 rekunn 1000
extern	DWORD	g_aSkilExpTable[5];

namespace	nsUpdatePlayerData
{
	enum
	{
		ADD_ACTIVE_MINI_PET2	=	2,
		ADD_CHAPTER5,
		ADD_GREATE_GOD_GRACE,			//	거신의 은총과 이벤트 변수 추가.
		ADD_WANT_EVENT_COUNT_RESET,	//	이벤트 변수 리셋
		ADD_GUILD_MASTER_GRACE,		//	길드 마스터 은총 관련
		ADD_ITEM_COOL_TIME	,			//	아이템 쿨타임 추가
		ADD_EAT_MINIPET_COUNT	,		//	미니펫 먹은 회수 추가
		ADD_ARBEIT_BUFFER_SIZE	,		//	
		REVISE_JAPAN_QUEST_DATA,		//	전생 3차 관련 일본 퀘스트 정보 수정
		LastVersion,
	};
};

const	int	c_iUpdatePlayerDataCounter	=	nsUpdatePlayerData::LastVersion-1;

extern	WORD				g_wBuffUpkeepTime;
extern	CPlayerSaveData		g_aDefaultPlayerJobData[dPLAYER_JOB_COUNT];
extern	CJobBasicDataDefine	g_aPlayerJobBaseInfo[];

#endif