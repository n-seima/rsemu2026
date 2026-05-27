#ifndef _classFIELD_H
#define _classFIELD_H

#include "SFC.H"
#include "cACTOR.H"
#include "cArea.H"
#include "cServer.H"
#include "CShop.h"
#include "CTrade.h"
#include "CBank.h"
#include "cPACKET_DBSERVER.h"
#include "cPACKET_DBSERVER.h"
#include "CLocalPartion.h"
#include "CPortal.h"
#include "trap.h"
#include "doorDefine.h"
#include "arca.h"
#include "banner.h"
#include "pattern.h"
#include "duel.h"

const	int	c_iBossZoneResistenceLimit	=	75;

enum
{
	eFT_NONE,
	eFT_BATTLE_FIELD,	//	전투 필드
	eFT_VILLAGE,		//	마을
	eFT_INNER_BUILDING,	//	건물 내부
};

#define	dMAX_ARCA_COUNT_IN_FIELD						256

#define	dFORECAST_VISIBLE_ACTOR_WIDTH					450
#define	dFORECAST_VISIBLE_ACTOR_HEIGHT					300
#define	dFORECAST_VISIBLE_ACTOR_WIDTH_FOR_MOVE_ACTION	600
#define	dFORECAST_VISIBLE_ACTOR_HEIGHT_FOR_MOVE_ACTION	400
#define	dVISIBLE_OUT_ACTOR_WIDTH						700
#define	dVISIBLE_OUT_ACTOR_HEIGHT						500

#define	dMONSTER_REGEN_RANGE							1600
#define	dMAX_FIELD_COUNT								1024

#define	dMAX_NPC_COUNT						1024
#define	dMAX_REGEN_COUNT_BY_ONE_TIME		20
#define	dMAX_BOOKING_COUNT_BY_JOIN_PLAYER	50

#define	dTILE_XS					64
#define	dTILE_BYTE_XS				128
#define	dTILE_YS					32
#define	dTILE_SIZE_SHIFT			11
#define	dTILE_XSIZE_SHIFT			6
#define	dTILE_YSIZE_SHIFT			5

#define	dOBJECT_DATA_SIZE			12

#define	dBORDER_ACTOR				10000
#define	dBORDER_DUMMY				5000

#define	dMASK_BUILDING				0x8000	//	건물
#define	dMASK_OBJECT				0x4000	//	오브젝트
#define	dMASK_BLOCKED				0x2000	//	막힘
#define	dMASK_CHARACTER				0x1000	//	캐릭터
#define	dMASK_GET_CHARACTER			0xFFF	//	캐릭터를 구하자

#define	dMAP_NAME_LENGTH			64

#define	dMAP_FIELD					0		//	일반적인 필드
#define	dMAP_VILLAGE				1		//	마을이다.
#define	dMAP_BRIDGE					2		//	다리다... 가 아니고.. -_-;; 실제 맵을 로딩하지 않고 트리거만 로당한다.

#define	dMAX_PLAYER_IN_FIELD		1024	//	필드안에 최대 플래이어 수
#define	dMAX_MONSTER_IN_FIELD		1024	//	필드안에 최대 몬스터 수
#define	dMAX_NPC_IN_FIELD			256		//	
#define	dMAX_PLAYER_IN_SCREEN		256		//	화면안에 보이는 최대 플래이어 수
#define	dMAX_ACTOR_IN_SCREEN		512		//	화면안에 보이는 최대 플래이어 수


#define	dPERIOD_DROPPED_ITEM_AND_REGEN_NPC	10
#define	dMAX_BGM_COUNT_IN_MAP				30

class	cInstanceField;
enum
{			// 마지막 수정일 : 09.10.06 
	eWEATHER_FINE,		// 맑음 +_+
	eWEATHER_SNOW,		// 눈 +_+
	eWEATHER_RAIN,		// 비...
};
//
//	맵의 기본정보
class	cMapInfo050527
{
public:
	int					m_iWidth,m_iHeight;
	char				m_strName[dMAP_NAME_LENGTH];//	맵 이름(머 "바보들의 평원")
	int					m_iTileSet;
	DWORD				m_bf4FieldType			: 4;
	DWORD				m_bf1IsPremiumZone		: 1;
	DWORD				m_bf1IsGuildBattleZone	: 1;
	DWORD				m_bf1IsGuildBattleLobby	: 1;
	DWORD				m_bf1IsLocked			: 1;
	DWORD				m_bf1IsOXQuizField		: 1;
	DWORD				m_bf1IsOXQuizGateVillage: 1;
	DWORD				m_bf1IsHiddenPortal		: 1;	//	포탈 모양만 숨긴다.
	DWORD				m_bf4MinimapDisplayLevel: 4;	//	미니맵 표시 레벨
	DWORD				m_bf1IsCanNotMemoryField: 1;	//	필드 기억 못함
	DWORD				m_bf1IsHalfSize			: 1;	//	절반 사이즈 맵
	DWORD				m_bf1IsWordQuizField	: 1;	//	낱말 퀴즈 필드
	DWORD				m_bf1IsWordQuizVillage	: 1;	//	낱말 퀴즈 필드 마을
	DWORD				m_bf1IsBlockToWarpField	: 1;
	DWORD				m_bf1IsBossZone			: 1;	//	보스존
	DWORD				m_bf1IsGuildHall		: 1;
	DWORD				m_bf1Is1LevelGuildHall	: 1;
	DWORD				m_bf1IsGuildPointBattleField	:	1;
	DWORD				m_bf1IsSiegeWarfareField	:	1;
	DWORD				m_bf1IsCanNotCallCarpetField:	1;
	DWORD				m_bf1IsCanNotUseCallingSkill:	1;
	//JBC		필드몬스터의 길찾기 능력 가동 여부 		08-09-18
	DWORD				m_bf1IsUseFindWay		: 1;	//	길찾기 기능 사용 여부 
	DWORD				m_bf1IsSetBossMap		: 1;	
	DWORD				m_bf1IsGuildDungeonMap		: 1;	//	길드 던젼 맵//30
	DWORD				m_bf1IsNotUseSkillBeforeTrans	: 1;	//31	몬스터변신 전 스킬 사용 불가
	DWORD				m_bf1IsNotOpenPitchManShop	: 1;	//32	노점상 불가

	int					m_iSerial;					//	프로젝트에서 관리할 시리얼... 아직은 없다.
	WORD				m_aBgmList[dMAX_BGM_COUNT_IN_MAP];
	WORD				m_wLinkSecretDungeon;
	WORD				m_bf1IsExistSecretDungeonGate	: 1;	// 수정
	WORD				m_bf3WeatherType		: 3;			//	4
	WORD				m_bf1IsNight			: 1;			//	5
	WORD				:0;

	short				m_sCorrectFireResistance;
	short				m_sCorrectWaterResistance;
	short				m_sCorrectWindResistance;
	short				m_sCorrectEarthResistance;
	short				m_sCorrectLightResistance;
	short				m_sCorrectDarkResistance;
};

class	cMAP_INFO : public cMapInfo050527
{
public:
	short				m_sCorrectMonsterFireResistance;			//	불 저항력 0% 100.00%  
	short				m_sCorrectMonsterWaterResistance;			//	물 저항력 0% 100.00%  
	short				m_sCorrectMonsterWindResistance;			//	바람 저항력 0% 100.00%  
	short				m_sCorrectMonsterEarthResistance;			//	대지 저항력 0% 100.00%  
	short				m_sCorrectMonsterLightResistance;			//	빛 저항력 0% 100.00%
	short				m_sCorrectMonsterDarkResistance;			//	어둠 저항력 0% 100.00%

	short				m_sCorrectMonsterBlindResistance;			//	장님 저항
	short				m_sCorrectMonsterPoisonResistance;			//	중독 저항
	short				m_sCorrectMonsterSleepResistance;			//	잠 저항
	short				m_sCorrectMonsterColdResistance;			//	콜드 저항
	short				m_sCorrectMonsterFreezeResistance;			//	프리즈 저항
	short				m_sCorrectMonsterStunResistance;			//	경직 저항
	short				m_sCorrectMonsterStoneResistance;			//	석화 저항
	short				m_sCorrectMonsterConfuseResistance;		//	혼란 저항
	short				m_sCorrectMonsterCharmingResistance;		//	챠밍 저항
    short				m_sCorrectMonsterCriticalResistance;
    short				m_sCorrectMonsterCrushResistance;

	short				m_sCorrectMonsterBadStatusResistance;		//	상태 이상공격에 대한 저항 0% 100.00%
	short				m_sCorrectMonsterDeclinePowerResistance;	//	능력치 저하계에 대한 저항 0% 100.00%
	short				m_sCorrectMonsterCurseResistance;			//	저주에 대한 저항 0% 100.00%
};


#define	dTEAM_COUNT							3
#define	dMAX_READY_TO_REBIRTH_ACTOR_COUNT	256

class	cBookedRemoveActorInfo
{
public:
	WORD	m_wSerial;
	WORD	m_bf1IsSleep	:	1;
};

class	cReadyToRebirthActorInfo
{
public:
	WORD	m_wSerial,m_wRemainSecond;
};

#define	dBOOKED_READY_TO_REMOVE_ACTOR_COUNT	128

class	cFieldWordQuiz
{
public:
	class	cWordQuizActorInfo
	{
	public:
		WORD	m_wScore;
		WORD	m_bf1IsLoser		:	1;
		WORD	m_bf1IsActive		:	1;
		WORD	m_bf1IsWaitSolution	:	1;

		void	beginQuiz()
		{
			reset();

			m_bf1IsActive	=	TRUE;
		}

		void	produceQuiz()
		{
			if	(m_bf1IsActive	&&	m_bf1IsLoser	==	FALSE)
				m_bf1IsWaitSolution	=	TRUE;
		}

		void	addScore(int _iScore)
		{
			m_wScore			+=	_iScore;
			m_bf1IsWaitSolution	=	FALSE;
		}

		void	reset()
		{
			m_wScore			=	0;
			m_bf1IsLoser		=	FALSE;
			m_bf1IsActive		=	FALSE;
			m_bf1IsWaitSolution	=	FALSE;
		}
	};

	WORD						m_wWordQuizRound,m_wWordQuizRoundForRemainActor,m_wWordQuizSortActorCount;
	cWordQuizActorInfo			*m_pWordActorInfo;
	cIndexForQSort				*m_pSortWordQuizActor;
};

//	필드의 액터-_-들을 좀 더 효율적으로 관리하기 위해서..
class	CFieldActorControl
{
public:
	CLocalPartionInfo			*m_pLocalPartition;
	CLocalSmallPartionLinkInfo	*m_pPartLinkInfo;

	WORD						m_iLimitPlayerCount;
	int							m_iLocalPartitionCount;
	int							m_iPartHeight,m_iPartWidth,m_iWidthPartCount,m_iHeightPartCount;
	int							m_iSaveMonsterCount,m_iSaveNPCCount;

	int							m_iAlivePlayerCount,m_iRealPlayerCount,m_iActorCount,m_aiActorCount[dTEAM_COUNT];
	int							m_iStandActorCount;			//	몬스터의 수
	int							m_iBookedRemoveActorCount;	//	제거 예약 액터의 수
	int							m_iReadyToRebirthActorCount;
	int							m_iBookedReadyToRemoveActorCount;

	WORD						m_awBookedReadyToRemoveActorList[dBOOKED_READY_TO_REMOVE_ACTOR_COUNT];
	WORD						m_awActorList[dTEAM_COUNT][dMAX_PLAYER_IN_FIELD];	//	
	WORD						m_awStandActorList[dMAX_ACTOR_IN_FIELD];
	cBookedRemoveActorInfo		m_aBookedRemoveActor[dMAX_ACTOR_IN_FIELD];
	cReadyToRebirthActorInfo	m_aReadyToRebirthActor[dMAX_READY_TO_REBIRTH_ACTOR_COUNT];
	BYTE						m_abIsReadyToRebirthActor[dMAX_ACTOR_IN_FIELD];
};

//
//	OX Quiz Process Status
enum
{
	eOIPS_SLEEP,
	eOIPS_READY,
	eOIPS_WAIT_QUESTION,	//	문제 기다리는 
	eOIPS_FINISH,			//	문제 기다리는 
	eOIPS_COUNTDOWN,		//	카운트 다운중
};

//
//	Word Quiz Process Status
enum
{
	eWQPS_SLEEP,			//	잔다.
	eWQPS_READY,			//	대기중
	eWQPS_WAIT_QUESTION,	//	문제 기다리는 중
	eWQPS_COUNTDOWN,		//	카운트 다운중
	eWQPS_FINISH,			//	문제 기다리는 
};

const	int	c_iFieldValueCount	=	50;

class	CGuildBattleInfo;

class	cDoorObjectInfo	:	public cDoorObjectSimpleInfo
{
public:
	int		m_iPosValue;
	WORD	m_wStatus;
};

class	cDoorManager
{
public:
	int					m_iDoorCount;
	cDoorObjectInfo		m_aDoorList[dMAX_DOOR_OBJECT_IN_MAP];

						cDoorManager();

	void				reset();

	BOOL				isFocusedDoor(cRECT *_lpRect);
	void				changeStatus(cRECT *_lpRect,int _iStatus);
	BOOL				getDoorStatus(int _iAddress);
};

//
//	필드 클래스
class cFIELD	:	public CFieldActorControl,public cFieldWordQuiz
{
public:
	class	COXQuiz
	{
	public:
		WORD				m_wStatus;
		WORD				m_wQuestionCount;
		WORD				m_wReviveLoserBattleCount;
		WORD				m_wWinnerCount;
		WORD				m_wComerCount;
		WORD				m_wIsSaveResult;
	};
	class	cWordQuiz
	{
	public:
		WORD				m_wStatus;
		WORD				m_wQuestionCount;
		WORD				m_wWinnerCount;
		WORD				m_wComerCount;
		WORD				m_wIsSaveResult;
		WORD				m_wLevel;
	};

	cWordQuiz				m_wordQuizStatus;
	COXQuiz					m_oxQuizStatus;	//	OX 퀴즈 상태

	WORD					m_wGateField;

	int						m_aiFieldValue[c_iFieldValueCount];

	BOOL					m_bIsProcessEvent,m_bIsBlockBattle;
	CPortalManager			m_portal;
	WORD					m_wTrapCount,m_awTrapList[dMAX_TRAP_COUNT_IN_FIELD];
	WORD					m_wLinkSecretDungeon;
	WORD					m_wIsExistSecretDungeonGate;
	WORD					m_wIsBossZone;
	WORD					m_wIsGuildHall,m_wHallOwnGuild;
	BOOL					m_bIs1LevelGuildHall,m_bIsGuildPointBattleField,m_bIsSiegeWarfareField,m_bIsCanNotCallCarpetField,m_bIsCanNotUseCalling , m_bIsUseFindWay;	//길찾기 사용 여부
	BOOL					m_bIsSetBossMap;
	BOOL					m_bIsGuildDungeonMap;
	int						m_iBossZoneResistenceLimit;

	BOOL					m_bIsWantPrintTimeKarmaInfo;
	CKarmaContainer			m_kc;
	cCharacterPatternPack	*m_pCPP;	//	캐릭터 패턴 팩

	CRITICAL_SECTION		m_csPacketManager,m_csActorManager;	//	패킷 큐를 위한 메니져. -_-a

	int						m_iSerial;							//	맵의 시리얼
	int						m_iUniqueSerial;
	int						m_iTextDataLocate;					//	이 맵에 텍스트 데이터이ㅡ 위치
	int						m_iCharacterCount;					//	이맵에서 영웅을 제외한 캐릭터의 수
	int						m_iRookieActor,m_iSavedActorCount;	//	액터의 수,빈자리
	int						m_iUpdatePlayerIndex;

	short					m_sCorrectFireResistance;
	short					m_sCorrectWaterResistance;
	short					m_sCorrectWindResistance;
	short					m_sCorrectEarthResistance;
	short					m_sCorrectLightResistance;
	short					m_sCorrectDarkResistance;

	short					m_sCorrectMonsterFireResistance;			//	불 저항력 0% 100.00%  
	short					m_sCorrectMonsterWaterResistance;			//	물 저항력 0% 100.00%  
	short					m_sCorrectMonsterWindResistance;			//	바람 저항력 0% 100.00%  
	short					m_sCorrectMonsterEarthResistance;			//	대지 저항력 0% 100.00%  
	short					m_sCorrectMonsterLightResistance;			//	빛 저항력 0% 100.00%
	short					m_sCorrectMonsterDarkResistance;			//	어둠 저항력 0% 100.00%

	short					m_sCorrectMonsterBlindResistance;			//	장님 저항
	short					m_sCorrectMonsterPoisonResistance;			//	중독 저항
	short					m_sCorrectMonsterSleepResistance;			//	잠 저항
	short					m_sCorrectMonsterColdResistance;			//	콜드 저항
	short					m_sCorrectMonsterFreezeResistance;			//	프리즈 저항
	short					m_sCorrectMonsterStunResistance;			//	경직 저항
	short					m_sCorrectMonsterStoneResistance;			//	석화 저항
	short					m_sCorrectMonsterConfuseResistance;			//	혼란 저항
	short					m_sCorrectMonsterCharmingResistance;		//	챠밍 저항
    short					m_sCorrectMonsterCriticalResistance;
    short					m_sCorrectMonsterCrushResistance;

	int						m_iRemainStategyPoint,m_iSiegeWarfareValue;
	

	short					m_sCorrectMonsterBadStatusResistance;		//	상태 이상공격에 대한 저항 0% 100.00%
	short					m_sCorrectMonsterDeclinePowerResistance;	//	능력치 저하계에 대한 저항 0% 100.00%
	short					m_sCorrectMonsterCurseResistance;			//	저주에 대한 저항 0% 100.00%

	int						m_iWidth,m_iHeight;					//	맵의 가로 세로 사이즈
	int						m_iPixelWidth,m_iPixelHeight;		//	맵의 가로 세로 사이즈(픽셸단위)
	char					m_strName[64];						//	맵 이름
	char					m_strFileName[64];					//	맵화일 이름
	int						m_iUpdateTime;
	WORD					m_wIsGBS_Field;	//	Guild,Boss,Secret
	BOOL					m_wIsPremiumZone,m_bIsGuildBattleField,m_wIsGuildBattleLobby,m_wIsCanNotMemoryField,m_wIsVillage;
	WORD					m_wIsOXQuizField,m_wIsOXQuizVillage;
	WORD					m_wIsWordQuizField,m_wIsWordQuizVillage;	//	낱말 퀴즈 필드
	WORD					m_wIsBlockToWarpField;
	WORD					m_wIsGuildLobby;

	WORD					m_wFieldType,m_wTileSet,m_wMinimapDisplayLevel;

	BOOL					m_bIsReadyToClose;
	WORD					m_wIsRegenMonster;
	WORD					m_wCorrectExperience;

	WORD					m_wRemainMinuteForCheckSecretDungeonParty;
	WORD					m_wRemainMinuteForRemoveSecretDungeonKeyCount;
	DWORD					m_dwBookedSecretDungeonKeyParty;

	cPACKET_QUE				m_queReceivePacket,m_queSendPacket;	//	받아서 저장한 패킷의 큐와 보내야 될 패킷을 저장한 큐
	int						m_iWorkPacketCount;

	cArea					*m_pArea;								//	영역.. 들 -_-a
	CDroppedItemManager		m_droppedItem;
	CActiveSkillManager		m_skillManager;						//	스킬 메니져
	CShopManager			m_shop;								//	상점
	CTradeManager			m_tradeManager;						//	트레이드 관리자
	CCustomItemManager		m_customItemList;

	cCHARACTER				m_aCharacter[dCHARACTER_COUNT];		//	이 맵에서 활동 가능한 캐릭터
	cNPC					*m_apNpcGenerateData[dMAX_NPC_COUNT];	//	필드에 몹이나 NPC 생성 정보
	cACTOR					*m_alpActor[dMAX_ACTOR_IN_FIELD];	//	실제로 게임이 돌아갈때의 액터들 정보(이넘으로 게임 돌린다.
	BYTE					*m_pMap;							//	충돌 정보나 각종 정보
	int						m_iFrameCounter;					//	프레임 카운터
	WORD					m_wWeather;							// 날씨~~~ 09.10.06
	int						m_iWeatherSecond;					// 날씨 지속시간 ~~ 09.10.07
	BOOL					m_bIsNotUseSkillBeforeTrans;		//	몬스터변신 전 스킬 사용 불가
	BOOL					m_bIsNotOpenPitchManShop;			//	노점상열기불가
	BOOL					m_bWeatherType;						//	날씨타입
	BOOL					m_bIsNight;							//	밤인가?
	static	int				s_iFileVersion;

							cFIELD();
							~cFIELD();


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃초기 설정
	void					init();								//	초기화
	void					reset();							//	리셋
	void					close();	
	
	static	void			ReadKarmaContainer(cFILE *_lpFile,CKarmaContainer *_lpKC,BOOL _bIsRequireDecode);
	static	void			ReadKarma(cFILE *_lpFile,CKarma *_lpKarma,BOOL _bIsRequireDecode);
	static	void			ReadKarmaItem(cFILE *_lpFile,CKarmaItem *_lpKarma,BOOL _bIsRequireDecode);

	void					readCC(cFILE *_lpFile,CConversationContainer *_lpCC,int _iSpeechCount,BOOL _bIsRequireDecode);//NPC 대화 데이터 로딩
	BOOL					load(char *_strFn,int _iUniqueSerial=-1,int _iGateField=0xffff,int _iDuelType=0xffff);	//	맵 데이터 로딩
	BOOL					loadGuildHall(CGuild	*_lpGuild);	//	길드 홀 로딩
	BOOL					loadGuildHall(int _iHallLevel);	
//┃초기 설정
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃업데이트
	void					operateStoredPacket();				//	패킷큐 처리
	void					uploadPacketToWorkBuffer();
	void					updateGame();						//	작업만 업데이트
	void					exportSkillList();	
//┃업데이트
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	void					rebuildRedstonePointEffect();			//	붉은 보석 포인트 효과 리빌드
	void					operateBreakParty(int _iPartySerial);//	_iPartySerial 파티가 해제 되었다고 통보한다.
	void					updateMonsterBeatInfo();

	BOOL					isExistTrafficObjectToOpenPitchmanShop(int _iX,int _iY);
	BOOL					isExistTrafficObjectToOpenPortal(int _iX,int _iY);
	BOOL					isVillage();
	BOOL					isSecretDungeon();
	BOOL					isEventField();	//	이벤트 필드냐?
	BOOL					isSpecialField();	//	특수 필드냐?
	BOOL					isBlockedToTeleportField();
	BOOL					isCanNotUseTeleportEffectField();
	BOOL					isExistDownrangeEnemy(int _iXPos,int _iYPos,int _iRange,int _iTeam);	//	_iRange안에 적이 있냐?
	BOOL					addReadyToRebirthTimeActor(int _iSerial);
	void					updateReadyToRebirthTimeActor();
	BOOL					checkIncorrectInfo();
	int						getMapDataVersion(char *_strHeader);

	int						getMasterInstanceFieldSerial();
	cInstanceField*			getMasterInstanceField();
	void					checkSecretDungeonParty();

	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃정보 구함
	BOOL					isCanCallMagicCarpetField();
	inline	BOOL			isIncorrectPos(int _iX,int _iY)
	{
		if (_iX	<	dTILE_XS	||	_iX	>=	m_iPixelWidth	-	dTILE_XS	||
			_iY	<	dTILE_YS	||	_iY	>=	m_iPixelHeight	-	dTILE_YS	)
			return	TRUE;

		return	FALSE;
	}
	BOOL					getFreePos(c4POINT *_lpPos);	//	저 좌표에서 빈자리를 찾아라
	inline	cAreaInfo*		getArea(int _iAreaIndex)
	{
		return	m_pArea->getArea(_iAreaIndex);
	}
	inline	BOOL			isBlocked(int _iX,int _iY)				//	_iX,_iY에... 블럭-_-?
	{
		if	(_iX < 0 || _iY < 0 || _iX >= m_iWidth || _iY >= m_iHeight)
			return	TRUE;

		if	(m_pMap[_iY*m_iWidth + _iX])
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isValidSerial(int _iSerial)
	{
		if (_iSerial < 0 || _iSerial >= dMAX_ACTOR_IN_FIELD)
			return TRUE;

		return	FALSE;
	}

	cCHARACTER*				getCharacterByJob(int _iJobShape);

//┃정보 구함
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃캐릭터 패턴
//┃field_character_pattern.cpp
	cCharacterPattern*		getCharacterPattern(int _iPattern);
//┃field_character_pattern.cpp
//┃캐릭터 패턴
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃함정 관련
//┃trapInField.cpp
	BOOL					getNotCrashPlaceForTrap(cPOINT *_lpPos,int _iSkill,int _iRange);
	BOOL					isNotCrashPlaceBySpecificSkillTrap(int _iSkill,int _iRange,int _iX,int _iY,int *_lpiMinRange);
	BOOL					operateTrapWorkInAreaByKarma(int _iArea,int _iWork);
	BOOL					isTrapFull();
	cTrap*					getTrap(int _iIndex);
	BOOL					addTrap(cTrap *_lpTrap);	//	함정 추가
	BOOL					removeTrap(int _iSerial);	//	함정 제거
	BOOL					disarmTrapsOwnedOutActor(int _iOutActor);	//	액터가 나간다.
	void					updateTrap();
	int						getExistTrapCount(cACTOR *_lpCaster,int _iSkill);
	cTrap*					getCaughtTrap(cACTOR *_lpActor);
	BOOL					explosionTrap(cACTOR *_lpCaster,cTrap *_lpTrap,cACTOR *_lpTarget,int _iType=eETT_NORMAL);
	void					explosionTrapSkill(cACTOR *_lpTarget,cTrapSkill *_lpTrap,int _iTargetMethod,int _iTrapLevel);
	void					checkFieldTrap(cACTOR *_lpActor);
//┃trapInField.cpp
//┃함정 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃영역 관련
//┃field_area.cpp
	void					moveActorToSafePosInArea(int _iArea);
//┃field_area.cpp
//┃영역 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃util_field.cpp
	DWORD					m_adwUniquePlayerNameHashCodeList[10000];
	LONGLONG				m_llExpSum;
	CTimeInfo				m_beginTime;

	inline	void			inlineAddExpSum(int _iExp)
	{
		m_llExpSum	+=	_iExp;
	}

	inline	void			inlineAddUniquePlayUser(DWORD _dwNameHash)
	{
		for	(int i=10000;i;)
		{
			--i;

			if	(m_adwUniquePlayerNameHashCodeList[i]	==	0xffffffff)
			{
				m_adwUniquePlayerNameHashCodeList[i]	=	_dwNameHash;
				break;
			}

			if	(m_adwUniquePlayerNameHashCodeList[i]	==	_dwNameHash)
				break;
		}
	}
	void					resetUserPlayStatics();
	int						getUniqueUserCount();
//┃util_field.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃배너 관련
//┃banner_field.cpp
	WORD					m_wBannerCount;
	WORD					m_awBannerList[dMAX_BANNER_COUNT_IN_FIELD];
	
	BOOL					isBannerFull();
	void					addActiveBanner(int _iSerial);
	void					addSendUpdateBannerInfo(cBanner *_lpBanner);
	BOOL					isExistCrushBanner(int _iX,int _iY);
	void					removeActiveBanner(int _iSerial);
//┃banner_field.cpp
//┃배너 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃보물 상자 관련
//┃field_arca.cpp
	WORD					m_wArcaCount;
	WORD					m_awArcaList[dMAX_ARCA_COUNT_IN_FIELD];
	static	char			s_strServerVersion[64];
	
	BOOL					operateArcaWorkInAreaByKarma(int _iArea,int _iWork);
	BOOL					isArcaFull();
	void					addActiveArca(int _iSerial);
	void					removeActiveArca(int _iSerial);
	void					droppingItemInArcaByMonster(cACTOR *_lpActor,cACTOR *_lpKiller,cDroppingItem *_lpDroppingItem,int _iDropLevel,cArca *_lpArca,cItem *_lpItem);
	BOOL					getTreasureMapPoint(cPOINT *_lpPos);

//┃field_arca.cpp
//┃보물 상자 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃문 관련
//┃field_door.cpp
	cDoorManager			*m_pDoors;
	
	BOOL					operateDoorWorkInAreaByKarma(int _iArea,int _iWork);
	void					changeDoorCrashMap(cAreaDoorInfo *_lpDoor,BOOL _bIsOpen);
	void					getLinkObjectWithDoor(cAreaDoorInfo *_lpDoor);
	void					setDoorCrashMap(int _iDoor,BOOL _bIsClear);
	void					setCrashMap(int x,int y,BYTE *_lpCrash,BOOL _bIsClear);
//┃field_door.cpp
//┃문 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃OX퀴즈 관련
//┃CFieldOXQuiz.cpp
	void					readyOXQuiz();	//	OX 퀴즈 준비
	void					beginOXQuiz();	//	OX 퀴즈 시작
	void					producedOxQuizQuestion();	//	OX 퀴즈 문제 제출
	void					operateOXQuizResult();	//	OX 퀴즈 결과 처리
	void					finishOxQuiz();	//	OX퀴즈 마무리
	void					checkOXQuizWinner();
	void					endOXQuiz();	//	OX퀴즈 종료
	void					operateWordQuizAnswer(cACTOR *_lpActor,char *_lpstrAnswer);

	void					launchOXQuizLoserReviveBattle();	//	OX 퀴즈 패자 부활전
//┃CFieldOXQuiz.cpp
//┃OX퀴즈 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃낱말 퀴즈 관련
//┃word_quiz_field.cpp
	void					readyWordQuiz();	//	낱말 퀴즈 준비
	void					beginWordQuiz();	//	낱말 퀴즈 시작
	void					producedWordQuizQuestion();	//	낱말 퀴즈 문제 제출
	void					changeWordQuizRound(BOOL _bIsFinishEvent=FALSE);
	void					sortWordQuizPlayerScore();
	void					operateWordQuizResult(int _iLoserCount=0,int _iSurvivorCount=0);	//	낱말 퀴즈 결과 처리
	void					finishWordQuiz();	//	낱말 퀴즈 마무리
	void					checkWordQuizWinner();
	void					endWordQuiz();	//	낱말 퀴즈 종료

	void					launchWordQuizLoserReviveBattle();	//	낱말 퀴즈 패자 부활전
//┃word_quiz_field.cpp
//┃낱말 퀴즈 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃지역 분할 관련
//┃CFieldLocalPartition.cpp
	BOOL					checkLocalPartPlayerForMonster(int _iPart);
	void					addActorAtLocalPart(int _iIndex,int _iTeam,int _iSmallPart);
	void					removeActorAtLocalPart(int _iIndex,int _iTeam,int _iSmallPart);
	void					changeSmallLocalPart(int _iSerial,int _iTeam,int _iOldPart,int _iNewPart);
	inline	int				getCurrentLocalPart(int _iX,int _iY)
	{
		if	(m_iLocalPartitionCount	==	1)
			return	0;

		int	iX	=	min(_iX/m_iPartWidth,m_iWidthPartCount);
		int	iY	=	min(_iY/m_iPartHeight,m_iHeightPartCount);

		iX		=	max(iX-1,0);
		iY		=	max(iY-1,0);
		iX		=	min(iX,m_iWidthPartCount-3);
		iY		=	min(iY,m_iHeightPartCount-3);

		return	iX+iY*(m_iWidthPartCount-2);
	}
	inline	int				getCurrentSmallLocalPart(int _iX,int _iY)
	{
		if (m_iLocalPartitionCount	==	1)
			return	0;
		
		int	iX	=	min(_iX/m_iPartWidth,m_iWidthPartCount-1);
		int	iY	=	min(_iY/m_iPartHeight,m_iHeightPartCount-1);

		return	iX+iY*m_iWidthPartCount;
	}
//┃CFieldLocalPartition.cpp
//┃지역 분할 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃결투 관련
//┃duelForField.cpp
	cDuelApplicantInfo		m_aDuelTeam[2];
	cDuelBattler			m_aDuelBattler[2][5];
	CTimeInfo				m_beginDuelTime;
	WORD					m_wDuelMatchStep,m_wDuelType,m_wReadyForDuelCounter,m_wDuelBattleTimeCounter,m_wWaitForExitTime,m_wWaitForEnterDuelFieldTime;
	int						m_iDuelSerial;
	WORD					m_awKillCount[2];
	BOOL					m_bIsTrialGameBattle;

	void					clsoeDuelField();
	void					setDuelMatchInfo(cDuelApplicantInfo *_lpTeam1,cDuelApplicantInfo *_lpTeam2,BOOL _bIsTrialGame=FALSE);
	int						getTeamInDuelField(char *_lpstrName);
	void					updateDuelField();
	bool					checkReadyToBeginDuelMatch();	//	결투 시작 준비가 되었는지 확인
	void					sendBeginDuelLog();
	void					sendEndDuelLog();

	bool					killDuelBattler(cACTOR *_lpKiller,cACTOR *_lpTarget);	//	결투 쫑이면 true 리턴
	void					sendDuelResult();
	void					operateDuelBattleResult();	//	결투 결과 처리
	cDuelBattler*			getDuelBattler(cACTOR *_lpActor);	//	결투 하는 사람찾기


//┃duelForField.cpp
//┃지역 분할 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃길드 관련
//┃CFieldGuild.cpp
	class	CGuildBattleGuildInfo
	{
	public:
		WORD	m_wSerial;
		int		m_iKillPoint;
	};

	CGuildBattleGuildInfo	m_aBattleGuild[2];			//	배틀 길드	
	BOOL					m_bIsOperateGuildBattle;
	int						m_iRemainGuildBattleTime;
	WORD					m_wGuildBattleStatus,m_wIsIncorrectGuildBattle,m_wGuildBattleFinished,m_wGuildBattleFieldOrder;		//	길드전 상태
	WORD					m_wWaitToExterminatedGuildStatusTime;

	BOOL					checkInsightGuildGuardian(int _iCheckRange,int _iX,int _iY);
	int						getGuildGuardianCount();	//	길드 가디언 수 얻기
	cACTOR*					findNearestGuildGuardian(int _iX,int _iY,int _iRange);	//	_iRange안에 길드 가디언 찾기
	void					setBattleGuild(int _iGuild1Serial,int _iGuild2Serial,int _iFieldOrder=0);	//	길드전 설정
	void					beginBattle();											//	길드전 시작
	void					changeGuildBattleStatus(int _iStatus,int _iBattleOrder);//	길드 전투 상태를 변경
	int						getTeamInGuildBattleField(int _iGuildSerial,char *_lpstrName=NULL);
	int						getGuildMemberCount(int _iGuildSerial,BOOL _bIsWantAliveMember=FALSE);	//	길드 멤버 수 얻기
	cACTOR*					getTopLevelGuildMember(int _iGuildSerial,BOOL _bIsWantAliveMember=FALSE);
	CGuild*					getHallOwnGuild();
	void					banishGuildHallPlayer();

	void					initGuildBattle(int _iBattleOrder);
	void					readyToGuildBattle();
	void					beginGuildBattle(int _iBattleTime=-1);
	void					finishGuildBattle();
	void					endGuildBattle();

	void					operateGuildBattleWorkPerSecond();								//	길드전 관련 업데이트

	void					addGuildBattleKillPoint(cACTOR *_lpKiller,cACTOR *_lpTarget);		//	킬포인트 ㄷ허줘~

	void					warpGuildBattleLobbyPlayerToBattleField(int _iTimeOrder,int _iGuild1=-1,int _iGuild2=-1);
	void					checkExterminatedGuild();
//┃CFieldGuild.cpp
//┃길드 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃파티 처리
//┃CFieldParty.cpp
	void					leaveParty(int _iActorSerial);	//	파티 떠나기...
//┃CFieldParty.cpp
//┃파티 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃카르마 처리
//┃karma_field.cpp
	void					operateKarma();	//	카르마 처리
//┃karma_field.cpp
//┃카르마 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃액터 추가 제거
//┃CFieldAddActor.cpp
	inline	void			resetStandActorList(){m_iStandActorCount=0;}
	inline	void			addStandActor(int _iActor)
	{
		m_awStandActorList[m_iStandActorCount++]	=	_iActor;
	}

	inline	BOOL			addBookedRemoveActor(int _iActor,BOOL _bIsSleep=FALSE)
	{
		if (m_iBookedRemoveActorCount	>=	dMAX_ACTOR_IN_FIELD)
			return	FALSE;

		m_aBookedRemoveActor[m_iBookedRemoveActorCount].m_wSerial		=	_iActor;
		m_aBookedRemoveActor[m_iBookedRemoveActorCount].m_bf1IsSleep	=	_bIsSleep;

		m_iBookedRemoveActorCount++;

		return	TRUE;
	}
	
	void					regenMonsterInArea(int _iArea,int _iCharacter);
	void					operateReadyToRemoveActorList();
	BOOL					addToReadyToRemoveActorList(int _iActor);
	void					addActorAtList(int _iIndex,int _iTeam,int _iSmallPart);
	BOOL					removeActorInList(int _iIndex,int _iTeam,int _iSmallPart);

	int						getTeam(int _iGuildSerial,int _iActorKind,char *_lpstrName);
	cACTOR*					addActor(int _iTeam,int _iActorKind,int _iX,int _iY);//액터 추가(빈자리 찾아서 넣자!!)
	BOOL					removeActor(int _iSerial,BOOL _bIsSleep=FALSE);//액터 제거

	void					checkConnectedPlayer(char *_lpstrName,BOOL *_lpbIsCannotDisconnect);	//	이미 접속해 있나 확인한다.

	cACTOR*					joinPlayer(CPlayerData *_lpData,int _iClientSerial,BOOL _isFirst=FALSE,cActorStatusUpkeep *_lpUpkeepData=NULL);//플레이어참가
	cACTOR*					startPlayer(CPlayerData *_lpData,int _iClientSerial,cActorStatusUpkeep *_lpUpkeepData);//시작 영역에 플래이어를 추가한다.

	void					killTeam(int _iTeam);//모든 몬스터 제거
	void					linkMonsterInSameArea();
	void					generateAllNPC();//모든 NPC를 생성합니다.

	void					regenAllMob();	
	int						getLinkAreaMonsterCount(int _iArea);
	cACTOR*					generateNpc(cNPC *_lpNpc,BOOL _isFirst=FALSE,BOOL _bIsByKarma=FALSE,int _iX=0,int _iY=0);//몬스터 생성
	cACTOR*					generateMonsterByCharacter(cCHARACTER *_lpCharacter,int _iLevel,int _iX,int _iY);
	BOOL					generatePet(cACTOR *_lpTamer,int _iPetIndex,BOOL _bIsUnseal);
	BOOL					generateSummonBeast(cACTOR *_lpSummoner,int _iLevel,int _iRemainHP,int _iType,int _iGrade,int _iX,int _iY,BOOL _bIsSummon);

	void					operateRegenNpc();			//	NPC 생성 타임!!
	void					operateOncePerSecondWork();	//	1초에 한번 업데이트 되는 작업 처리
	void					operateOncePer10SecondWork();//	10초에 한번씩 처리해야될 작업	
	void					operateOncePerMinuteWork();	//	1분에 한번씩 처리해야 할 작업
	void					operateOncePerHourWork();
	void					increasePlayerPlayTime();	//	플레이어의 게임 플레이시간을 증가 시켜준다.
	BOOL					addPortal(cACTOR *_lpCaster,int _iField,int _iRemainTime,int _iX=-1,int _iY=-1);

	BOOL					transferField(cACTOR *_lpActorData,int _iLocate);//플레이어의 필드 이동참가
	BOOL					importActor(cNPC *_lpActorData,int _iX,int _iY);//맵에서 부른 캐릭터 추가

	void					rebuildActorList();	//	액터 리스트 재 설정
//┃CFieldAddActor.cpp
//┃액터 추가 제거
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃액터 처리
//┃CFieldActor.cpp
 	BOOL					findValidPos(cPOINT *_lpPos);//_lpPos를 중심으로 빈자리 찾기
	BOOL					findEnterancePos(cPOINT *_lpPos,cAreaInfo *_lpAreaInfo);//_lpAreaInfo안에 입장가능 공간 찾기
	BOOL					findEnterancePos(cPOINT *_lpPos,cRECT _rect);//rect안에 입장가능 공간 찾기
	void					findEnterancePosToPointAround(cPOINT *_lpPos,int _iRange);	//	일정 좌표 주위로 입장 가능한 공간 찾기
	void					findEnterancePosToPointAroundTile(cPOINT *_lpPos,int _iTileRange);//	일정 좌표 주위로 입장 가능한 공간 찾기
	int						removeNpcValidBeaterInfo();
	inline	cACTOR**		getActorList()			{return	m_alpActor;}	//액터 리스트
	inline	cACTOR*			getActor(int _iActor)	
	{
		if (_iActor<0 || _iActor>= dMAX_ACTOR_IN_FIELD)
			return NULL;

		if (!m_alpActor[_iActor] || m_alpActor[_iActor]->m_wSerialInField	==	0xffff)
			return	NULL;

		return	m_alpActor[_iActor];
	}//액터
	inline	cACTOR*			getTestedActor(int _iSerial)
	{
		return	getActor(_iSerial);
	}

	inline	int				getActorCount()			{return	m_iActorCount;}	//액터 카운트
	cACTOR*					findNeareastPlayer(int _iLocalPart,int _iX,int _iY,int _iSight,cACTOR *_lpClient=NULL);//	가장 근접해 있는 플레이어를 찾는다.
	cACTOR *				findNotUnderAttackeActor(cACTOR *_lpClient);
	int						getInRangeEnemyCount(cACTOR *_lpClient,int _iRange);	
	int						getInRangeEnemy(int _iX,int _iY,int _iTean,int _iRange,WORD *_lpwList,int _iExistTargetCount,int _iBufferSize);
	cACTOR*					findNeareastEnemy(int _iOwnTeam,int _iLocalPart,int _iX,int _iY,int _iSight,cACTOR *_lpClient=NULL,BOOL _bIsIgnoreSleepyActor=FALSE);//	가장 근접해 있는 플레이어를 찾는다.
	cACTOR*					findBestFarEnemy(int _iOwnTeam,int _iLocalPart,int _iX,int _iY,int _iSight,cACTOR *_lpClient);
	cACTOR*					findNeareastEnemyForChainLightning(int _iOwnTeam,int _iLocalPart,int _iX,int _iY,int _iSight,cACTOR *_lpClient,int _iExceptActorCount,WORD *_lpwExceptActorList);
	cACTOR*					findRandomTargetInInsightEnemy(cACTOR *_lpClient);
	cACTOR*					findRandomTargetInInsightEnemy(int _iOwnTeam,int _iLocalPart,int _iX,int _iY,int _iSight,cACTOR *_lpClient=NULL);

	cACTOR*					findNeareastActor(int _iLocalPart,int _iExceptionSerial,int _iX,int _iY,int _iSight,int _iTeam=-1);//	가장 근접해 있는 누군가를 찾는다.
	cACTOR*					findNeareastActor(int _iLocalPart, WORD* _awExceptionSerialList,int _iExceptionSerialCount,int _iX,int _iY,int _iSight,int _iTeam=-1);//	가장 근접해 있는 누군가를 찾는다.
	int						getInSightOwnerTeamActorCount(int _iX,int _iY,cACTOR *_lpActor,int _iSight,BOOL _bIsExceptOwner,BOOL _bIsWantDeathActor);
	int						getInSightOwnerTeamActorList(WORD *_lpwActorList,int _iBufferSize,int _iX,int _iY,cACTOR *_lpActor,int _iSight,BOOL _bIsExceptOwner,BOOL _bIsWantDeathActor);
	int						getInSightEnemyActorList(WORD *_lpwActorList,int _iBufferSize,int _iX,int _iY,cACTOR *_lpActor,int _iSight,BOOL _bIsWantDeathActor,BOOL _bIsCheckBlock,cAbility *_lpAbility);
	int						getEnemyActorListForChainLightning(WORD *_lpwActorList,cACTOR *_lpClient,int _iSight,int _iWantCount);
	int						getInSightEnemyActorCount(int _iX,int _iY,int _iLocalPart,int _iTeam,int _iSight);

	void					changeMonsterInfo(cACTOR *_lpAsker,int _iSerial,int _iJob,int _iLevel);
	inline	int				getPartyMemberCount(int _iTeam,int _iParty)
	{
		int	iCount	=	0,i;

		for (i=0;i<m_aiActorCount[_iTeam];i++)	
		{
			cACTOR	*lpActor	=	getActor(m_awActorList[_iTeam][i]);

			if (!lpActor)
				continue;

			if (lpActor->getPartySerial()	==	_iParty)
				iCount++;
		}

		return	iCount;
	}
	BOOL					giveExpToParty(int _iHitterMaxLevel,cACTOR *_lpKiller,int _iPartyiBeater,BOOL _bIsExistSlugger,int _iMerit,int _iExp,cACTOR *_lpTarget);
	void					giveExpToActor(int _iHitterMaxLevel,int _iSerial,BOOL _bIsExistSlugger,int _iMerit,int _iExp,cACTOR *_lpTarget,cNPC *_lpNpc);

	BOOL					kill(cACTOR *_lpKiller,cACTOR *_lpTarget,BOOL _bIsByKarma=FALSE);//_lpKiller가 _lpTarget을 죽였다.
	void					readyToRemoveActorWork(int _iSerial);
	void					operateForDeathActorWork(cACTOR	*_lpOuter);
	void					updateBorderActorPositionInfo(cACTOR *_lpActor,int _iX,int _iY);//외곽에 액터들 위치 업데이트
	int						getSameJobCount(cACTOR *_lpActor,int _iRange);
	BOOL					changeActorsPos(int _iArea1,int _iArea2,int _iTarget,int _iWhere,cACTOR	*_lpAsker);
	BOOL					isPVP(cACTOR *_lpActor1,cACTOR *_lpActor2);
	int						getCopyActorCount(cACTOR *_lpActor);
	int						getShutInMagicBoxTargetCount(cACTOR *_lpActor);
	int						getIllusionCount(cACTOR *_lpCaster);
	int						getPOSTargetCount(cACTOR *_lpCaster);
	int						getPetForceList(cACTOR *_lpPet,WORD *_lpwList,int _iSize);	//	펫 입장에서 아군 리스트	

	cNPC*					getValidNpcByJob(int _iJob);
	cACTOR*					getActorByJob(int _iJob,int _iIndex=0);
	cACTOR*					getActorByBody(int _iBody);
	int						getMirrorImageActorCount();
//┃CFieldActor.cpp
//┃액터 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃예약작업(싱크를 맞추기 위해.. -o-)
//┃CFieldBooking.cpp
	BOOL					bookingMakeSkillZone(cACTOR *_lpCaster,cAbility *_lpAbility);
	void					bookingGeneratePet(char *_lpstrTamerName,int _iPetIndex,BOOL _bIsUnseal);
	void					bookingGenerateSummonBeast(int _iX,int _iY,char *_lpstrTamerName,int _iType,int _iGrade,int _iLevel,int _iRemainHP,BOOL _bIsSummon);
	void					bookingMonsterTame(char *_lpstrCasterName,int _iTamedLevel,int _iTamedLoyalty,int _iTargetFieldSerial,int _iTargetZoneSerial);
	void					bookingDeathActor(int _iKillerSerial,int _iSerial,int _iZoneSerial,BOOL _bIsRelease,BOOL _bIsByKarma=FALSE);	//	액터 죽이기 예약
	BOOL					bookingReadyToExit(int _iSerial,char *_lpstrName);	//플레이어 제거 준비 예약
	BOOL					bookingExitPlayer(int _iSerial,char *_lpstrName);	//플레이어 제거 예약
	BOOL					bookingToExitGameForMoveField(int _iSerial,char *_lpstrName);	// 필드 이동 예약
	void					bookingMakeDummy(int _iX,int _iY,int _iSerial,int _iUpkeepTime,int _iAddHPPercentage);	//	더미 만들기 예약
	void					appointmentMakeIllusion(int _iSkill,int _iX,int _iY,int _iLordSerial,DWORD _dwLordNameHashCode,int _iUpkeepTime,int _iHP,BOOL _bIsFirstTargetIllusion,int _iIllusionJob);
	void					bookingCopyActor(int _iX,int _iY,int _iLord,int _iTarget,int _iUpkeepTime,int _iHP);	//	캐릭터 복사 예약
	void					bookingDisplacement(cACTOR *_lpCaster,cACTOR *_lpTarget,BOOL _bIsRelease);
	void					bookingCharming(cACTOR *_lpCaster,cACTOR *_lpTarget,BOOL _bIsRelease,int _iTime);
	void					bookingIntervalShooter(int _iSkillIndex,cACTOR *_lpCaster,cACTOR *_lpTarget);
//┃CFieldBooking.cpp
//┃예약작업(싱크를 맞추기 위해.. -o-)
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃예약작업(싱크를 맞추기 위해.. -o-)
//┃CFieldOperateBookedWork.cpp
	void					operateBookedPlayerToJoin();//참가 예약된넘들 처리
	void					operateBookedExitGame(BW_EXIT_GAME *_lpPacket,int _iSerial);//게임에서 나가기
	void					operateBookedExitGameForMoveField(BW_EXIT_GAME_FOR_MOVE_FIELD *_lpPacket,int _iSerial);
	void					operateReadyToExit(BW_READY_TO_EXIT *_lpPacket,int _iSerial);
	void					operateBookedDeathActor(BW_DEATH_ACTOR *_lpPacket,int _iSerial);//	죽은 액터 처리
	void					operateBookedMakeDummy(BW_MAKE_DUMMY *_lpPacket,int _iSerial);//	더미 만들기
	void					operateAppointmentMakeIllusion(BW_MAKE_ILLUSION *_lpPacket,int _iSerial);//	환영 만들기
	void					operateBookedCopyActor(BW_COPY_ACTOR *_lpPacket,int _iSerial);
	void					operateBookedDisplacement(BW_DISPLACEMENT *_lpPacket,int _iSerial);
	void					operateBookedCharming(BW_CHARMING *_lpPacket,int _iSerial);
	void					operateBookedTame(BW_MONSTER_TAME *_lpPacket);
	void					operateBookedGeneratePet(BW_GENERATE_PET *_lpPacket);
	void					operateBookedGenerateSummonBeast(BW_GENERATE_SUMMON_BEAST *_lpPacket);
	void					operateBookedIntervalShooter(BW_INTERVAL_SHOOTER *_lpPacket);
	void					operateMakeSkillZone(BW_MAKE_SKILL_ZONE *_lpPacket);
//┃CFieldOperateBookedWork.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	inline	cACTOR*			getPlayerByName(char *_lpstrName)
	{
		DWORD	dwNameHashCode	=	GetHashCode(_lpstrName);

		for (int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
		{
			for (int iActor=m_aiActorCount[iTeam];iActor;)
			{
				--iActor;

				cACTOR	*lpActor	=	m_alpActor[m_awActorList[iTeam][iActor]];

				if	(lpActor	==	NULL || !lpActor->isPlayer())
					continue;

				if	(lpActor->m_dwNameHashCode	!=	dwNameHashCode)
					continue;

				if	(STRCMP(lpActor->m_strName,_lpstrName) == 0)
					return	lpActor;
			}
		}

		return	NULL;
	}

	inline	cACTOR*			getPlayerByName(char *_lpstrName,DWORD _dwNameHashCode)
	{
		for (int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
		{
			for (int iActor=m_aiActorCount[iTeam];iActor;)
			{
				--iActor;

				cACTOR	*lpActor	=	m_alpActor[m_awActorList[iTeam][iActor]];

				if	(lpActor	==	NULL || !lpActor->isPlayer())
					continue;

				if	(lpActor->m_dwNameHashCode	!=	_dwNameHashCode)
					continue;

				if	(STRCMP(lpActor->m_strName,_lpstrName) == 0)
					return	lpActor;
			}
		}

		return	NULL;
	}

	inline	cACTOR*			getPlayerByID(char *_lpstrID,DWORD _dwIDHashCode)
	{
		for (int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
		{
			for (int iActor=m_aiActorCount[iTeam];iActor;)
			{
				--iActor;

				cACTOR	*lpActor	=	m_alpActor[m_awActorList[iTeam][iActor]];

				if	(lpActor	==	NULL || !lpActor->isPlayer())
					continue;

				if	(lpActor->m_dwIDHashCode	!=	_dwIDHashCode)
					continue;

				if	(STRCMP(lpActor->m_strId,_lpstrID) == 0)
					return	lpActor;
			}
		}

		return	NULL;
	}

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃패킷 처리
//┃cFIELD_OPERATE_PACKET.cpp
	inline	BOOL			checkTargetAndKill(int _iCaster,int _iTarget)	//	타겟을 체크하고 죽었으면 킬 메시지를.. 없으면 리무브 메시지를 날려준다.
	{
		if (!checkPlayerAndKill(_iCaster))
			return	FALSE;

		cACTOR	*lpCaster	=	getActor(_iCaster);

		if	(isValidSerial(_iTarget))
		{
			lpCaster->sendRemoveActor(_iTarget);
			return	FALSE;
		}

		cACTOR	*lpTarget	=	getActor(_iTarget);

		if	(!lpTarget)
		{
			lpCaster->sendRemoveActor(_iTarget);
			return	FALSE;
		}

		if (lpTarget->isDeath())
		{
//			sendKillActorMessage(lpCaster,_iTarget);
			return	FALSE;
		}

		return	TRUE;
	}

	inline	BOOL			checkTargetAndKill(cACTOR *_lpCaster,cACTOR *_lpTarget)//	타겟의 상태를 확인한다.
	{
		if (!checkPlayerAndKill(_lpCaster))	return	FALSE;

		if (!_lpTarget)
		{
			_lpCaster->stopBattle();

			return	FALSE;
		}

		if (_lpTarget->m_wSerialInField == 0xffff)
		{
			_lpCaster->sendRemoveActor(_lpTarget->m_wSerialInField);

			return	FALSE;
		}

		if (_lpTarget->isDeath())
		{
//			sendKillActorMessage(_lpCaster,_lpTarget->m_wSerialInField);
			return	FALSE;
		}

		return	TRUE;
	}

	inline	BOOL			checkPlayerAndKill(int _iPlayerSerial)	//	플레이어를 체크하고 죽었으면 킬 메시지를 날려준다.
	{
		cACTOR	*lpPlayer	=	getActor(_iPlayerSerial);

		if (!lpPlayer)
			return FALSE;

		if (lpPlayer->isDeath())
		{
			lpPlayer->sendKillMessage();

			return	FALSE;
		}

		return	TRUE;
	}
	
	inline	BOOL			checkActor(int _iPlayerSerial)	//	플레이어를 체크한다.
	{
		cACTOR	*lpPlayer	=	getActor(_iPlayerSerial);

		if (!lpPlayer)
			return FALSE;

		return	TRUE;
	}

	inline	BOOL			checkActor(cACTOR *_lpActor)	//	플레이어를 체크한다.
	{
		if (!_lpActor || _lpActor->m_wSerialInField==0xffff)	return	FALSE;

		return	TRUE;
	}

	inline	BOOL			checkPlayerAndKill(cACTOR *_lpPlayer)	//	플레이어를 체크하고 죽었으면 킬 메시지를 날려준다.
	{
		if (!_lpPlayer								)	return	FALSE;
		if (isValidSerial(_lpPlayer->m_wSerialInField))	return	FALSE;

		if (_lpPlayer->isDeath())
		{
			_lpPlayer->sendKillMessage();

			return	FALSE;
		}

		return	TRUE;
	}


	BOOL					addReceivePacket(ALL_MSG *_lpPacket,int _iSerial,BOOL _bIsBooking=FALSE);	//	패킷 추가
	
	void					movePlayer(CG_MOVE *_lpPacket,int _iSerial);	//	플레이어 이동
	void					receiveReportCurrentPos(CG_REPORT_CURRENT_POS *_lpPacket,int _iSerial);
	void					stopPlayer(CG_STOP *_lpPacket,int _iSerial);//플레이어 정지
	void					toggleSitdown(CG_TOGGLE_SITDOWN *_lpPacket,int _iSerial);//앉기 토글
	void					moveField(CG_MOVE_FIELD *_lpPacket,int _iSerial);//필드 이동
	void					setMoveAbility(CG_SET_MOVE_ABILITY *_lpPacket,int _iSerial);//플레이어 뛰기/정지 토글
	void					transformation(CG_TRANSFORMATION *_lpPacket,int _iSerial,int _iReason=eTR_BY_SKILL,BOOL _bIsRequreAfterMotion=FALSE);//변쉰!!
	void					receiveActionToObject(CG_ACTION_TO_OBJECT *_lpPacket,int _iSerial);
	int						actionToActor(CG_ACTION_TO_ACTOR *_lpPacket,int _iSerial,BOOL _isFirst=TRUE);//플레이어 액션(to 액터)
	void					actionToGround(CG_ACTION_TO_GROUND *_lpPacket,int _iSerial,BOOL _isFirst=TRUE);//플레이어 액션(to 대지)
	void					actionQuick(CG_ACTION_QUICK *_lpPacket,int _iSerial,BOOL _isFirst=TRUE);//플레이어 액션(to 대지)

	void					engageBattle(CG_ENGAGE_BATTLE *_lpPacket,int _iSerial);//교전!!
	void					stopBattle(CG_STOP_BATTLE *_lpPacket,int _iSerial);
	void					increaseCharacterState(CG_INCREASE_STATE *_lpPacket,int _iSerial);//스탯 증가
	void					increaseAbilityLevel(CG_INCREASE_ABILITY_LEVEL *_lpPacket,int _iSerial);//어빌러티 레벨 증가
	void					receiveAskActorInfo(CG_ASK_ACTOR_INFO *_lpPacket,int _iSerial);//몬스터의 정보 요청

	void					receiveAskItemPackName(CG_ASK_ITEM_PACK_NAME *_lpPacket,int _iSerial);
	void					receiveAskUseSoundOfLeadersBell(CG_USE_SOUND_OF_LEADERS_BELL *_lpPacket,int _iSerial);
	void					pickItem(CG_PICK_ITEM *_lpPacket,int _iSerial);			//	필드에 떨어진 아이템 줍기
	BOOL					pickItem(int _iItemSerial,int _iCheckSum,int _iSerial,BOOL _bIsByServer=FALSE);
	void					repairItem(CG_REPAIR_ITEM *_lpPacket,int _iSerial);
	void					equipItem(CG_EQUIP_ITEM *_lpPacket,int _iSerial);		//	아이템 장비
	void					reloadBeltItem(CG_RELOAD_BELT_ITEM *_lpPacket,int _iSerial);		//	아이템 리로드
	void					useItem(CG_USE_ITEM *_lpPacket,int _iSerial);			//	아이템 사용
	void					useItemToItem(CG_USE_ITEM_TO_ITEM *_lpPacket,int _iSerial);			//	아이템 사용
	void					useBeltItem(CG_USE_BELT_ITEM *_lpPacket,int _iSerial);	//	벨트에 아이템을 사용한다.
	void					dropItem(CG_DROP_ITEM *_lpPacket,int _iSerial);	//	벨트에 아이템을 사용한다.

	void					stripEquipment(CG_STRIP_EQUIPMENT *_lpPacket,int _iSerial);	//	장비 해제
	void					changeItemPlace(CG_CHANGE_ITEM_PLACE *_lpPacket,int _iSerial);	//	아이템 교환
	void					destroyItem(CG_DESTROY_ITEM *_lpPacket,int _iSerial);
	
	void					askItemData(int _iSerial);	//	뭔가 잘못 되었다!! 아이템 데이터를 요청한다.
	void					askInventoryData(int _iSerial);	//	뭔가 잘못 되었다!! 아이템 데이터를 요청한다.
	void					askEquipmentData(int _iSerial);	//	뭔가 잘못 되었다!! 아이템 데이터를 요청한다.
	void					levelUp(int _iSerial);//	언넘이 레벨업을 했다.

	void					receiveAskInfoActorList(CG_ASK_INFO_ACTOR_LIST *_lpPacket,int _iSerial);//액터 데이터 보정(주로 클라이언트의 요구에 의해..)
	void					receiveObscurityActorList(CG_OBSCURITY_ACTOR_LIST *_lpPacket,int _iSerial);
	void					receiveMisplacedActorList(CG_MISPLACED_ACTOR_LIST *_lpPacket,int _iSerial);	//	위치가 잘못된(?) 액터 리스트를 받았다.
	void					receiveAskPlayerInfo(CG_ASK_PLAYER_INFO *_lpPacket,int _iSerial);
	void					receiveCheckPlayerCheckSum(CG_CHECK_PLAYER_CHECK_SUM *_lpPacket,int _iSerial);
	void					receiveChangeDoorStatus(CG_CHANGE_DOOR_STATUS* _lpPacket,int _iSerial);
	void					receiveTouchEventArea(CG_TOUCH_EVENT_AREA *_lpPacket,int _iSerial);
	void					receiveArcaWork(CG_ARCA_WORK *_lpPacket,int _iSerial);
	void					receiveAskArcaInfo(CG_ASK_ARCA_INFO *_lpPacket,int _iSerial);
	void					receiveDisarmFieldTrap(CG_DISARM_FIELD_TRAP *_lpPacket,int _iSerial);
	void					receivePutBanner(CG_PUT_BANNER *_lpPacket,int _iSerial);
	void					receiveAskBannerText(CG_ASK_BANNER_TEXT *_lpPacket,int _iSerial);
	void					receiveSearchTradeStationItem(CG_SEARCH_RELAY_STATION_ITEM *_lpPacket,int _iSerial);
	void					receiveAskDoorInfo(CG_ASK_DOOR_INFO *_lpPacket,int _iSerial);

	void					receiveBuyCarrotShopItem(CG_BUY_CARROT_SHOP_ITEM *_lpPacket,int _iSerial);

	void					receiveReturnToVillage(CG_RETURN_TO_VILLAGE *_lpPacket,int _iSerial);	//	마을로 돌아가기
	void					receiveOperatorCommand(CG_OPERATOR_COMMAND *_lpPacket,int _iSerial);	//	운영자 명령 처리
	void					receiveChangePetName(CG_CHANGE_PET_NAME *_lpPacket,int _iSerial);	//	펫 이름 변경
	void					receivePartingWithPet(CG_PARTING_WITH_PET *_lpPacket,int _iSerial);
	void					receiveAskActorEnchantInfo(CG_ASK_ACTOR_ENCHANT_INFO *_lpPacket,int _iSerial);
	void					warpFieldByGateGlove(CG_WARP_FIELD_BY_GATE_GLOVE *_lpPacket,int _iSerial ,BOOL _isNetCafeUser = FALSE);
	void					receiveRemovePrefix(CG_REMOVE_PREFIX *_lpPacket,int _iSerial);
	void					receiveJudgeToItem(CG_JUDGE_TO_ITEM *_lpPacket,int _iSerial);
	void					receiveEtcWork(CG_ETC_WORK *_lpPacket,int _iSerial);
	void					receiveXTrapPacket(CG_XTRAP_PACKET *_lpPacket,int _iSerial);
	
	void					receiveMergeItem(CG_MERGE_ITEM *_lpPacket,int _iSerial);
	void					receiveGGAuthData(CG_GG_AUTH_DATA *_lpPacket,int _iSerial);

	void					setReactionStatus(CG_SET_REACTION_STATUS *_lpPacket,int _iSerial);	//	리액션 상태 설정
	void					setPassiveSkillStatus(CG_TURN_ON_PASSIVE_SKILL *_lpPacket,int _iSerial);	//	리액션 상태 설정
	void					exitGame(CG_EXIT_GAME *_lpPacket,int _iSerial);
//┃cFIELD_OPERATE_PACKET.cpp
//┃패킷 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃트레이드 패킷 처리
//┃CFieldOperateTradePacket.cpp
	BOOL					checkTradeBox(int _iSerial);

	void					operateRequestTradePacket(CG_REQUEST_TRADE *_lpPacket,int _iSerial);
	void					operateAcceptTradePacket(CG_TRADE_REPLY *_lpPacket,int _iSerial);
	void					operateCancelTradePacket(CG_CANCEL_TRADE *_lpPacket,int _iSerial);
	void					operateAddTradeItemPacket(CG_ADD_TRADE_ITEM *_lpPacket,int _iSerial);
	void					operateSetTradeItemPacket(CG_CHANGE_TRADE_ITEM_DATA *_lpPacket,int _iSerial);
	void					operateRemoveTradeItemPacket(CG_REMOVE_TRADE_ITEM *_lpPacket,int _iSerial);
	void					operateSetTradeGoldPacket(CG_SET_TRADE_GOLD *_lpPacket,int _iSerial);
	void					operateReadyTradePacket(CG_READY_TRADE *_lpPacket,int _iSerial);
	void					operatePermitTradePacket(CG_PERMIT_TRADE *_lpPacket,int _iSerial);
//┃CFieldOperateTradePacket.cpp
//┃트레이드 패킷 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃이벤트 패킷 처리
//┃CFieldEventPacket.cpp
	void					selectNpc(CG_SELECT_NPC *_lpPacket,int _iSerial);//NPC 선택
	void					selectSpeech(CG_SELECT_SPEECH *_lpPacket,int _iSerial);//대화 선택
	void					buyItem(CG_BUY_ITEM *_lpPacket,int _iSerial);//아이템 구입
	void					buyGuildShopItem(CG_BUY_ITEM *_lpPacket,int _iSerial);
	void					buyBadgeOfTraderShopItem(CG_BUY_ITEM *_lpPacket,int _iSerial);	// 거래상인의 배지 상점 아이템구입
	void					sellItem(CG_SELL_ITEM *_lpPacket,int _iSerial);//아이템 판메
	void					cancelQuest(CG_CANCEL_QUEST *_lpPacket,int _iSerial);//퀘스트 취소
	void					summonEventMonster(CG_SUMMON_EVENT_MONSTER *_lpPacket,int _iSerial);// 몬스터 소환
	void					sendFailedSellItem(cACTOR *_lpActor,int _iReason);
	void					sendEventTimeUpdate();	//	이벤트 활성화 관련정보..
//┃CFieldEventPacket.cpp
//┃이벤트 패킷 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃노점상
//┃CFieldPitchmanShopPacket.cpp
	void					receiveAskOpenThePitchmanShop(CG_ASK_OPEN_THE_PITCHMAN_SHOP *_lpPacket,int _iSerial);
	void					receiveAddPitchmanShopItem(CG_ADD_PITCHMAN_SHOP_ITEM *_lpPacket,int _iSerial);
	void					receiveRemovePitchmanShopItem(CG_REMOVE_PITCHMAN_SHOP_ITEM *_lpPacket,int _iSerial);
	void					receiveChangePitchmanShopInfo(CG_CHANGE_PITCHMAN_SHOP_INFO *_lpPacket,int _iSerial);
	void					receiveChangePitchmanShopItemPlace(CG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE *_lpPacket,int _iSerial);
	void					receiveClosePitchmanShop(CG_CLOSE_PITCHMAN_SHOP *_lpPacket,int _iSerial);
	void					receiveBuyPitchmanShopItem(CG_BUY_PITCHMAN_SHOP_ITEM *_lpPacket,int _iSerial);
	void					receiveAskPitchmanShopInfo(CG_ASK_PITCHMAN_SHOP_INFO *_lpPacket,int _iSerial);
	void					receiveAskSimplePithmanShopInfoList(CG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST *_lpPacket,int _iSerial);
//┃CFieldPitchmanShopPacket.cpp
//┃노점상
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃은행 패킷 처리
//┃CFieldBankPacket.cpp
	void					storeItemToTheBank(CG_STORE_ITEM_TO_THE_BANK *_lpPacket,int _iSerial);
	void					storeGoldToTheBank(CG_STORE_GOLD_TO_THE_BANK *_lpPacket,int _iSerial);
	void					withdrawItemFromTheBank(CG_WITHDRAW_ITEM_FROM_THE_BANK *_lpPacket,int _iSerial);
	void					withdrawGoldFromTheBank(CG_WITHDRAW_GOLD_FROM_THE_BANK *_lpPacket,int _iSerial);
	void					moveBankItem(CG_MOVE_BANK_ITEM *_lpPacket,int _iSerial);
	void					moveInventoryItemWhenBankTransaction(CG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION *_lpPacket,int _iSerial);
	void					finishBankTransaction(CG_FINISH_BANK_TRANSACTION *_lpPacket,int _iSerial);
	void					cancelBankTransaction(CG_CANCEL_BANK_TRANSACTION *_lpPacket,int _iSerial);
//┃은행 패킷 처리
//┃CFieldBankPacket.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃커뮤니티 패킷 처리
//┃CFieldCommunityPacket.cpp
	void					chat(CG_CHAT *_lpPacket,int _iSerial);//채팅
	void					receiveAskPartyListMessage(CG_ASK_PARTY_LIST *_lpPacket,int _iSerial);
	void					receiveAskPartyInfoMessage(CG_ASK_PARTY_INFO *_lpPacket,int _iSerial);
	void					receiveAskJoinPartyMessage(CG_ASK_JOIN_PARTY *_lpPacket,int _iSerial);
	void					receiveChangePartyStatus(CG_CHANGE_PARTY_STATUS *_lpPacket,int _iSerial);
	void					receiveFindUserMessage(CG_FIND_USER *_lpPacket,int _iSerial);
	void					receiveRequestAddFriendMessage(CG_REQUEST_ADD_FRIEND *_lpPacket,int _iSerial);
	void					receiveReplyForAddFriendMessage(CG_REPLY_FOR_ADD_FRIEND *_lpPacket,int _iSerial);
	void					receiveRemoveNameInTargetFriendList(CG_REMOVE_NAME_IN_TARGET_FRIEND_LIST* _lpPacket,int _iSerial);
	void					receiveRemoveNameInTargetFriendListResult(CG_REMOVE_NAME_IN_TARGET_FRIEND_LIST_RESULT* _lpPacket,int _iSerial);

	void					receiveAskJoinPartyReplyMessage(CG_ASK_JOIN_PARTY_REPLY *_lpPacket,int _iSerial);
	void					receiveLeavePartyMessage(CG_LEAVE_PARTY *_lpPacket,int _iSerial);
	void					receivePartyLeaderWork(CG_PARTY_LEADER_WORK *_lpPacket,int _iSerial);
	void					receiveChangePartyNameMessage(CG_CHANGE_PARTY_NAME *_lpPacket,int _iSerial);

	void					sendPartyInfo(cACTOR *_lpActor,BOOL _bIsCreate);	//	파티 정보 전송

	void					receiveChangeCommunityStatus(CG_CHANGE_COMMUNITY_STATUS *_lpPacket,int _iSerial);
	void					receiveCreateGuild(CG_CREATE_GUILD *_lpPacket,int _iSerial);
	void					receiveAskGuildInfo(CG_ASK_GUILD_INFO *_lpPacket,int _iSerial);
	void					receiveAskGuildMemberList(CG_ASK_GUILD_MEMBER_LIST *_lpPacket,int _iSerial);

	BOOL					sendGuildMemberNullList(cACTOR *_lpActor);

	void					receiveAskDissoulutionGuild(int _iSerial);
	void					receiveDebugMessage(CG_DEBUG_MESSAGE *_lpPacket,int _iSerial);
	void					receiveAskJoinGuild(CG_ASK_JOIN_GUILD *_lpPacket,int _iSerial);
	void					receiveReplyAskJoinGuild(CG_REPLY_ASK_JOIN_GUILD *_lpPacket,int _iSerial);

	void					receiveAskChangeGuildNotice(CG_ASK_CHANGE_GUILD_NOTICE	*_lpPacket,int _iSerial);
	void					receiveAskChangeGuildExpTax(CG_ASK_CHANGE_GUILD_EXP_TAX	*_lpPacket,int _iSerial);
	void					receiveGuildMemberWork(CG_GUILD_MEMBER_WORK *_lpPacket,int _iSerial);
	void					receiveAskDismissSubGuildMaster(CG_ASK_DISMISS_SUB_GUILD_MASTER	*_lpPacket,int _iSerial);
	void					receiveAskAppointmentSubGuildMaster(CG_ASK_APPOINTMENT_SUB_GUILD_MASTER	*_lpPacket,int _iSerial);
	void					receiveAskDismissGuildCongress(CG_ASK_DISMISS_GUILD_CONGRESS	*_lpPacket,int _iSerial);
	void					receiveAskAppoitmentGuildCongress(CG_ASK_APPOINTMENT_GUILD_CONGRESS	*_lpPacket,int _iSerial);
	
	void					receiveAskBanishGuildMember(CG_ASK_BANISH_GUILD_MEMBER	*_lpPacket,int _iSerial);
	void					receiveAskLeaveGuild(CG_ASK_LEAVE_GUILD	*_lpPacket,int _iSerial);
	void					receiveAskChangeGuildMaster(CG_ASK_CHANGE_GUILD_MASTER *_lpPacket,int _iSerial);
	void					receiveAskRegistRegularMember(CG_REGIST_REGULAR_GUILD_MEMBER *_lpPacket,int _iSerial);
	void					receiveReadyToMakeGuild(CG_READY_TO_MAKE_GUILD *_lpPacket,int _iSerial);
	void					receiveIncreaseGuildSkillLevel(CG_INCREASE_GUILD_SKILL_LEVEL *_lpPacket,int _iSerial);
	void					receiveAskGuildBattleSchedule(CG_ASK_GUILD_BATTLE_SCHEDULE *_lpPacket,int _iSerial);
	void					receiveRequestGuildBattle(CG_REQUEST_GUILD_BATTLE *_lpPacket,int _iSerial);
	void					receiveCancelGuildBattle(CG_CANCEL_GUILD_BATTLE *_lpPacket,int _iSerial);
	void					receiveCloseGuildBattleScheduler(int _iSerial);
	void					receiveSelectGuildMark(CG_SELECT_GUILD_MARK	*_lpPacket,int _iSerial);////	길드 마크 선택
	void					receiveAskGuildMarkInfo(CG_ASK_GUILD_MARK_INFO *_lpPacket,int _iSerial);
	void					receiveAskSimpleGuildInfo(CG_ASK_SIMPLE_GUILD_INFO* _lpPacket,int _iSerial);
	void					receiveGuildCheckSum(CG_GUILD_CHECK_SUM* _lpPacket,int _iSerial);

	void					receiveCallMagicCarpet(CG_CALL_MAGIC_CARPET *_lpPacket,int _iSerial);
	void					receiveChangeEmblemShape(CG_CHANGE_EMBLEM_SHAPE *_lpPacket,int _iSerial);
	void					receiveRememberOrbLocation(CG_REMEMBER_ORB_LOCATION *_lpPacket,int _iSerial);
	void					receiveWarpToNearVillage(CG_WARP_TO_NEAR_VILLAGE *_lpPacket,int _iSerial);
	void					receiveAskRecoverDeathPenelty(CG_RECOVER_DEATH_PENELTY *_lpPacket,int _iSerial);
	void					receiveEnterPortal(CG_ENTER_PORTAL *_lpPacket,int _iSerial);
	void					receiveTeleportToOrbLocation(CG_TELEPORT_TO_ORB_LOCATION *_lpPacket,int _iSerial);
//┃CFieldCommunityPacket.cpp
//┃커뮤니티 패킷 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃카트 패킷
//┃CFieldCartPacket.cpp
	void					receiveAskCartOpen(CG_CART_OPEN *_lpPacket,int _iSerial);
	void					receiveWithdrawItemFromTheCart(CG_WITHDRAW_ITEM_FROM_THE_CART *_lpPacket,int _iSerial);
	void					receiveCloseCart(CG_CLOSE_CART *_lpPacket,int _iSerial);
	void					receiveAskDestroyCartItem(CG_DESTROY_CART_ITEM *_lpPacket,int _iSerial);
	void					receiveOpenCarrotShop(CG_OPEN_CARROT_SHOP *_lpPacket,int _iSerial);
//┃CFieldCartPacket.cpp
//┃카트 패킷
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃전송할 패킷 추가
//┃CFieldAddSendPacket.cpp
	inline	BOOL			addSendPacket(void *_lpPacket,int _iSerial)	{return	m_queSendPacket.addPacket((ALL_MSG *)_lpPacket,_iSerial);}//패킷 추가
	BOOL					sendPacketToAllOperator(ALL_MSG *_lpPacket,int _iLevel=-1);	// 모든 운영자에게 패킷 전송
	BOOL					sendPacketToAllPlayer(void *_lpPacket);
	void					sendPacketToInAreaActor(ALL_MSG *_lpMsg,int _iArea);
	void					sendSimpleInfoToInAreaActor(int _iArea, cACTOR* _lpAsker);
	void					sendUseItemResult(int _iClientSerial,WORD _wSerialInField,WORD _wResult);

	void					addSendUpdateArcaInfo(cArca *_lpArca);
	void					addSendStrikeLightningWinder(cACTOR *_lpCaster,cACTOR *_lpTarget,CHitInfo *_lpHitInfo,cAbility *_lpAbility);
	void					addSendStrikeMultiMissile(cACTOR *_lpCaster,cACTOR *_lpTarget,CHitInfo *_lpHitInfo,cAbility *_lpAbility);	// 멀티미사일.
	void					addSendFieldTrapWork(int _iSerial,int _iStatus,int _iSpareValue=0);
	void					addSendChangeDoorStatus(int _iArea,BOOL _bIsSleep,BOOL _bIsHide,BOOL _bIsLocked,BOOL _bIsTrap,BOOL _bIsClose,BOOL _bIsActiveByKarma);
	void					addSendDoorMessage(int _iDoor,int _iKey,int _iSkill,int _iMessage,cACTOR *_lpCaster);

	void					addPacketRemovePortal(int _iPortalSerial);
	void					addPacketAddPortal(int _iPortalSerial,int _iX,int _iY,int _iOwnerSerial,int _iField,BOOL _bIsTownPortal);
	void					addSendAddTrap(cTrap *_lpTrap);
	void					addSendRemoveTrap(int _iSerial);

	void					addSendRemoveActor(int _iSerial,BOOL _bIsHide=FALSE);
	void					addSendGuildBattleInfo(int _iTarget=0xffff);

	void					addSendOccurEffect(int _iX,int _iY,int _iEffect,int _iScale=100);
	void					addSendOccurEffectBySkill(int _iX,int _iY,int _iSkill,int _iEffect);
	void					addSendEventMessage(char *_lpstrText,int _iType, int _iTime = 30 , BOOL _bEffectflickeringly = TRUE);

	void					sendHideWeapon(int _iWho,cAbility *_lpAbility,BOOL _bIsHideOtherJobWeapon);//누군가가 무기를 버린다.

	void					addSendAttackToObject(cACTOR *_lpCaster,int _iTarget,int _iObjectType,int _iRemainDurability);
	void					sendBasicAttack(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility);//기본 공격
	void					sendContinuousHitAttack(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility);//연속 히트 시키는 공격
	void					sendActionToGroup(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility);
	void					sendActionToGroupStickedBit(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility);
	void					sendBouncingMissile(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility);
	void					sendWideMissile(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility);

	void					addSendEtcWork(int _iSerial,int _iWork,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0);
	void					sendEtcWorkInArea(int _iArea,int _iWork,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0);
	void					sendIntervaslShooterAttack(cACTOR *_lpCaster,cAbility *_lpAbility,int _iShotCount);
	void					sendLandMarkerAttack(cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility,int _iShotCount);//랜드 마커 공격

	void					sendContinuousHitAttackByValue(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,int _iPhysicalDamage,int _iMagicDamage,int _iShotCount,int _iMissCount,int _iCriticalCount,int _iDoubleCriticalCount,int _iMinPhysicalDamage,int _iMinMagicDamage,int _iBlockingCount,int _iInstanceKillCount,int _iNoActionBlockingCount,int _iDancingBlockCount, int _iHardBlowCount);//연속 히트 시키는 공격
	void					sendBunshineAttack(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,int _iPhysicalDamage,int _iMagicDamage,int _iShotCount,int _iMissCount,int _iCriticalCount,int _iDoubleCriticalCount,int _iMinPhysicalDamage,int _iMinMagicDamage,int _iBlockingCount,int _iInstanceKillCount,int _iNoActionBlockingCount,int _iDancingBlockCount, int _iHardBlowCount);
	void					sendScimitarCuttingAttack(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,int _iPhysicalDamage,int _iMagicDamage,int _iShotCount,int _iMissCount,int _iRange,int _iCriticalCount,int _iDoubleCriticalCount,int _iMinPhysicalDamage,int _iMinMagicDamage,int _iBlockingCount,int _iInstanceKillCount,int _iNoActionBlockingCount,int _iDancingBlockCount, int _iHardBlowCount);

	void					sendIllusionMissileAttack(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility);//환상 미사일 공격
	void					sendStrikeIntervalShooter(cACTOR *_lpTarget,cAbility *_lpAbility,int _iPhysicalDamage,int _iMagicDamage,int _iShotCount);

	void					sendActionToActor(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,CHitInfo *_lpHitInfo=NULL);//액터 공격
	void					sendActionToGround(cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility);//걍 공격.. -o-
	void					sendActionQuick(cACTOR *_lpCaster,cAbility *_lpAbility,int _iDirect=8);//	퀵 액션을 사용했다.
	void					sendHitThornDamage(int _iAttacker,int _iTarget,int _iDamage,int _iRemainCP ,BOOL _isDisplayMagicDamage = FALSE);//	스파이크 데미지 전송
	void					sendAbsorbDamage(int _iActorSerial,int _iAbsorbDamage);	//	데미지 흡수

	void					sendReleaseShimmeringShield(int _iCaster,int _iTarget);//	시머링 실드 취소
	void					sendSetShimmeringShield(int _iCaster,int _iReceiver,int _iRemainCP,int _iShieldShape,BOOL _bIsShield);//	시머링 실드 설정
	void					sendCureActorMessage(int _iTarget,int _iSkill,int _iHealPoint);//치유 메시지 전송

	void					sendFloatMachine(int _iTarget,int _iSkill);//	머신을 띄운다.
	void					sendReleaseMachine(int _iTarget);//	머신을 제거한다.
	void					sendMachineMissile(int _iCaster,int _iTarget,int _iSkill,CHitInfo *_lpHitInfo,int _iPetOrder=0);//미사일 쐈다!!
	void					sendFireTailChaser(int _iCaster,int _iTarget,int _iSkill,int _iShotCount,int _iShotPeriod,int _iDamage,int _iCorrectDamage);//	테일 체이서 발사!!

	void					sendMakeActionForm(int _iCaster,int _iTarget,cAbility *_lpAbility,CHitInfo *_lpHitInfo=NULL);//	_iCaster가 _iTarget에게 _lpAbility를 한다.
	void					sendUseSkillToActor(int _iCaster,int _iTarget,cAbility *_lpAbility,CHitInfo *_lpHitInfo);//	_iCaster가 _iTarget에게 _lpAbility를 한다.
	void					sendUseSkillToGround(int _iCaster,int _iPosX,int _iPosY,cAbility *_lpAbility);//	_iCaster가 _iTarget에게 _lpAbility를 한다.

	void					addActiveReactionSkillMessage(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility);
	void					sendAttackToActorResult(int _iResult,cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility);
	void					sendAttackToGroundResult(int _iResult,cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility);
	void					sendActiveSkillEffect(int _iCaster,int _iTarget,int _iSkill,int _iCP,int _iPhysicalDamage,int _iMagicDamage);	//	특정 스킬 효과가 발동!!
	void					sendContinuousActionToGroup(cACTOR *_lpCaster,cAbility *_lpAbility);		// 분신 연속 공격
//	void					sendKillActorMessage(cACTOR *_lpPlayer,int _iTarget);	//	_iTarget가 죽었다고  _lpPalyer에게 보내준다.
//┃CFieldAddSendPacket.cpp
//┃전송할 패킷 추가
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	void					sendChangeSpecialSkillField(cACTOR *_lpCaster,cAbility *_lpAbility=NULL);//	스훼셜 스킬 휠드가 변견되었다.
	void					sendChargeCP(cACTOR *_lpActor,int _iCP,int _iSkill);//	플레이어에게 CP충전했다고 메시지 전송

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃패킷 처리
//┃CFieldSendStoredPacket.cpp
	BOOL					sendFirstMeetingData(cACTOR *_lpActor);//	필드에 처음 참가 했을때 정보 전송
	void					sendExistFieldItem(cACTOR *_lpActor);//	필드에 존재하는 아이템 정보를 전송한다.
	void					sendMinimumActorInfoList(cACTOR *_lpActor);//	필드에 있는 액터의 시리얼 리스트를 보낸다.
	void					sendExistTrapList(cACTOR *_lpActor);
	void					sendExistArcaList(cACTOR *_lpActor);
	void					sendExistBannerList(cACTOR *_lpActor);
	void					sendExistFireWallList(cACTOR *_lpActor);
	void					sendDoorsStatus(cACTOR *_lpActor);
	void					sendExistPitchmanShopList(cACTOR *_lpActor);
	void					sendExistPortalList(cACTOR *_lpActor);
	BOOL					sendStoredPacketsToPlayer(cACTOR *_lpActor);
	void					sendPacketToSameLocalPlayer(int _iLocal,cRECT *_lpRect,void *_lpPacket,int _iPacketSize);
	void					operateDeliverPacket();
//┃CFieldSendStoredPacket.cpp
//┃패킷 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃아이템 관련
//┃CFiledItem.cpp
	void					dropSecretDungeonKey(cACTOR *_lpKiller,int _iX,int _iY);
	void					resetSecretDungeonKeyReceiver();
	BOOL					isDropUniqueChance(int _iItemType,int _iTargetLevel,int _iKillerLuck,int _iKillerCorrectUnique , BOOL _bIsWordEventTime,BOOL _bIsGuildMasterGrace);
	BOOL					isDropRareChance(int _iItemType,int _iTargetLevel,int _iCorrectItemDropRate,int _iKillerLuck,int _iKillerCorrectUnique,
		int _iKillerCorrectRare,int _iCorrectMagicItemDropChance , BOOL _bIsWordEventTime, BOOL _bIsGuildMasterGrace);
	BOOL					isDropItemChance(CDroppedItem *_lpDroppedItem,cDroppingItem	*_lpDroppingItem,cACTOR *_lpKiller);

	void					droppingItem(cACTOR *_lpActor,cACTOR *_lpKiller);//아이템 떨구기..
	int						droppingItem(cACTOR *_lpActor,cACTOR *_lpKiller,CDroppedItem *_lpDroppedItem,cDroppingItem	*_lpDroppingItem,int _iDropLevel,int _iOwner,BOOL _bIsDropUnique,BOOL _bIsDropRare);
	BOOL					makeDropItem(cACTOR *_lpActor,cACTOR *_lpKiller,CDroppedItem *_lpDroppedItem,cDroppingItem	*_lpDroppingItem,int _iDropLevel,int _iOwner,BOOL _bIsDropUnique,BOOL _bIsDropRare);
	void					removeOldestDroppedItem(int _iCount, BOOL _bIsIgnoreErrMsg = FALSE);//가장 오래된 아이템 _iCount개 없애기
	void					operateDroppedItem();//필드에 떨궈진 아이템 카운트 감소 시켜서 처리
	void					dropItem(CDroppedItem *_lpDroppedItem);
	void					giveEventItemToBeater(cACTOR* _lpActor,WORD	_wItemIndex, int* _lpOption,int _iMethod);
	void					dropEventSummonMonsterItem(cACTOR *_lpEventSummonMonster);	//	 이벤트 소환 몬스터 아이템 드랍
//┃CFiledItem.cpp
//┃아이템 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃지형 관련
//┃field_terrain.cpp
	BOOL					makeBlock(int _iX1,int _iY1,int _iX2,int _iY2);
	BOOL					removeBlock(int _iX1,int _iY1,int _iX2,int _iY2);
//┃field_terrain.cpp
//┃지형 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃스킬 관련
//┃CFiledSkill.cpp
	inline	BOOL			isBlockedWay(int _iMx,int _iMy,int _iDestMx,int _iDestMy);
	BOOL					isMissileBlockedLine(int _iX,int _iY,int _iDestX,int _iDestY);

	BOOL					monsterBombExplosion(cAbility *_lpAbility,cACTOR *_lpCaster,int _iRange,int _iPoisonDamage,int _iPoisonDamageTime,int _iDarkDamagePercentage);
	BOOL					monsterBomb(cAbility *_lpAbility,cACTOR *_lpCaster,int _iRange,int _iPoisonDamage,int _iPoisonDamageTime,int _iDarkDamagePercentage,int _iBombRange);
	BOOL					wideAidAttack(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,int _iX,int _iY);//
	BOOL					strikeWhirlWind(cACTOR *_lpCaster,cAbility *_lpAbility,BOOL bIsWantGetCP=TRUE, BOOL *_bIsContinuousGetCP = NULL);//_lpCaster가 회오리형 타격을 입힌다.
	BOOL					strikeArcShape(cACTOR *_lpCaster,cAbility *_lpAbility,int _iX,int _iY,int _iAngle,int _iHitAngleRange);
	int						strikeExplosionSkill(cACTOR *_lpCaster,cACTOR *_lpTarget,int _iX,int _iY,cAbility *_lpAbility,BOOL *_bIsGetCP,
												int _iLimitCount=dMAX_TARGET_FOR_WIDE_AREA_ATTACK,CActiveSkillDamagedActorInfo *_lpInfo=NULL,int _iStrikeIndex=0);//익스플로전 스킬 사용
	void					getBouncingLinearTarget(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,CActiveSkillDamagedActorInfo *_lpInfo,int _iMaxCount);
	void					getBigBulletTarget(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,CActiveSkillDamagedActorInfo *_lpInfo,int _iMaxCount);	
	int						getExplosionSkillDamage(cACTOR *_lpCaster,cACTOR *_lpTarget,int _iX,int _iY,cAbility *_lpAbility,int _iLimitCount,CDamageActorInfoTrailer *_lpDamageInfo);
	BOOL					strikeLinerChainExplosion(cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility,WORD *_lpDamagedActorList);
	BOOL					strikePiercingMissile(cACTOR *_lpCaster,cAbility *_lpAbility,cACTOR *_lpTarget,WORD *_lpDamagedActorList);//_lpCaster가 관통미사일형 타격을 입힌다.
	BOOL					strikeLaser(cACTOR *_lpCaster,cAbility *_lpAbility,cPOINT *_lpX1Y1,cPOINT *_lpX2Y1,cPOINT *_lpX1Y2,cPOINT *_lpX2Y2);//레이저 처럼 타격을 준다.
	BOOL					strikeWideAreaDamageSkill(cACTOR *_lpCaster,cAbility *_lpAbility,cPOINT *_lpPos,int _iRange,
														WORD *_lpDamagedActorList,int _iListSize,BOOL *_bIsGetdCp,CHitInfo *_lpDamageSum=NULL);	//	범위 데미지

	BOOL					strikeWideAreaMagicDamage(cACTOR *_lpCaster,int _iX,int _iY,BOOL _bIsWantApplyEE,cAbility *_lpAbility,
														int _iRange,int _iFireDamage,int _iWaterDamage,int _iWindDamage,int _iEarthDamage,int _iLightDamage,int _iDarkDamage,
														int _iTeam=-1,int *_lpiDeathTargetCount=NULL,BOOL _bIsByMiniPet=FALSE);
	
	BOOL					thrustActor(int _iX,int _iY,cACTOR *_lpCaster,cACTOR *_lpTarget,int _iTeam,cAbility *_lpAbility,int _iRange,int _iKnockBackDistance,int _iAngleToTarget=0xffff);
	BOOL					thrust(cACTOR *_lpCaster,int _iRange,int _iKnockBackDistance,int _iMinRange=0, BOOL _bIsKnockBackResistance = FALSE);	
	BOOL					releaseDummy(cACTOR *_lpActor);	//	더미 제거
	BOOL					castIcyStalagmite(cACTOR *_lpActor,cAbility *_lpAbility,int _iStatus=-1);	//	아이시 스탈라그마이트 제거
	void					operateAuraSkill(cAbility *_lpAbility,int _iX,int _iY,int _iRange,cACTOR *_lpCaster,BOOL _bIsIncreaseCP=TRUE);
	void					operateTitleEffect(int _iTitle,cACTOR *_lpCaster,int _iRange);

	BOOL					operateRushSkill(cACTOR *_lpCaster,cACTOR *_lpTarget,int _iX,int _iY,cAbility *_lpAbility,cPOINT *_lpPosArrive,CActiveSkillDamagedActorInfo *_lpInfo,BOOL _bIsIllusion=FALSE);
	BOOL					getRushPoint(cACTOR *_lpCaster,cACTOR *_lpTarget,int _iX,int _iY,cAbility *_lpAbility,cPOINT *_lpPosArrive);
	BOOL					operateSkillExtraEffectForOwnTeam(int _iX,int _iY,cACTOR *_lpCaster,cAbility *_lpAbility);

	int						getMirrorTowerReceiverCount(cACTOR *_lpCaster);
	int						operateMirrorTower(cACTOR *_lpAttacker,cACTOR *_lpReceiver,cAbility *_lpAbility,int _iDamage);

	void					displacement(cACTOR *_lpCaster,cACTOR *_lpTarget);
	void					releaseDisplacement(cACTOR *_lpCaster,cACTOR *_lpTarget);
	void					stopAllTargetedAction(cACTOR *_lpTarget,BOOL _bIsByInvisible=FALSE,BOOL _bIsReleaseFocus=FALSE);
	void					stopAllHostileAction(cACTOR *_lpTarget);
	void					changeTeam(cACTOR *_lpActor,int _iTeam);
	int						detectingEvil(cACTOR *_lpCaster,int _iRange,int _iLimitCount,int _iLimitLevel,int _iLimitType);

	BOOL					thrustCenter(cACTOR *_lpCaster,int _iThrustCenterX, int _iThrustCenterY, int _iRange,int _iMinRange = 0);
//┃CFiledSkill.cpp
//┃아이템 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃길드전 관련 필드
//┃field_guild_battle.cpp
	BOOL					m_bIsProcessSiegeWarfare;
	void					beginSiegeWarfare();	//	공성전 성문 열기
	void					reportSuccessToDefense();	//	공성전 성문 열기
	void					reportSuccessToInvade();	//	공성전 성공
	void					destroyGuildStategyPoint();
	void					resetSiegeWarfareStatus();
//┃field_guild_battle.cpp
//┃길드전 관련 필드
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃스킬 관련
//┃CFieldHealSkill.cpp
	void					cureRangedPartyMember(cACTOR *_lpCaster,cAbility *_lpAbility,int _iHealPoint,int _iX,int _iY,int _iCureRange,int _iParty);	//	범위 안에 파티원 치료
//┃CFieldHealSkill.cpp
//┃스킬 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};	//	class cFIELD

class	CFieldDistanceInfo
{
public:
	WORD	m_wSerial;
	int		x,y,z;
	WORD	m_wType;
};

class	CFieldInfoManager
{
public:
	int					m_iFieldCount;
	CFieldDistanceInfo	m_aField[dMAX_FIELD_COUNT];

	BOOL				isVillage(int _iField);
	int					getDistance(int _iField1,int _iField2);
};

extern	CFieldInfoManager	g_fieldInfo;

extern	void			FillChattingBuffer(SG_CHAT *_lpstrPacket,char *_lpstrName,char *_lpstrMessage);

#endif