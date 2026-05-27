#ifndef _classMAP_H
#define _classMAP_H

#include "cNEMO.H"
#include "cOBJECT.H"
#include "CGamePlay.H"
#include "cTILE.H"
#include "cPROJECT.H"
#include "cAREA.H"
#include "doorDefine.H"

#define	dMAX_LINK_MONSTER_COUNT	46

const	int	c_iMaxFieldActorCount	=	2048;

#define dMAX_HUNTING_AREA_MONSTER_COUNT 20
#define dTILE_WIDTH 64
#define dTILE_HEIGHT 32
#define dSAVEROAD_FIELDINDEX	40
#define dSAVEROAD_COUNT			dMAX_PROCESS_QUEST_COUNT*5

class cSAVEROAD
{
public:
	int			m_iQuestListIndex;
	int			m_iQuestLinkFieldIndex;
	int			m_aiFieldIndex[dSAVEROAD_FIELDINDEX];
	BOOL		m_bLinkField;

	void reset()
	{
		m_iQuestListIndex = 0xffff;
		m_iQuestLinkFieldIndex = 0xffff;
		for(int i=0 ; i<dSAVEROAD_FIELDINDEX ; i++)
			m_aiFieldIndex[i] = 0xffff;
		m_bLinkField = FALSE;	
	}
};

enum
{
	eMM_CLOSEBTN_NORMAL,
	eMM_CLOSEBTN_OVER,
	eMM_CLOSEBTN_DOWN,
};
//
//	맵에 저장되어 있는 액터 데이터
class	cBASE_NPC_INFO
{
public:
	int					m_iSerial;
	WORD				m_wCharacter;
	WORD				m_wActorKind;

	WORD				m_wDirect;			// 어느쪽을 보고 있을꺼냐?
	WORD				m_wIsChangeDirect;	// 서 있는 동안 방향을 바꿀거냐?
	int					m_iRegenerationCycle;// 리젠 주기

	WORD				m_wMoveArea;	// 행동 영역
	WORD				m_wPatrolRoute;	// 순찰 루트(없으면 행동 반경 안에서 얼레 벌레~)
	WORD				m_wWanderCycle;	// 0 안움직인다. 10단계로 이동 주기 설정
	WORD				m_wBattleInclination;// 전투 성향.

	WORD				m_bf1IsLeader		:	1;	//  리더
	WORD				m_bf1IsGuildCrest	:	1;	//  길드 문장
	WORD				m_bf1IsGuildObject	:	1;	//	길드 오브젝트

	WORD				m_wWard;		// 보호해야할 인물
	WORD				m_wLinkMonsterCount;
	WORD				m_wLinkType;	//	쌍방향/단방향 링크
	WORD				m_awLinkMonster[dMAX_LINK_MONSTER_COUNT];//	링크되어 있는 몹
	WORD				m_wGenerateAroundPosX1;
	WORD				m_wGenerateAroundPosY1;
	WORD				m_wGenerateAroundPosX2;
	WORD				m_wGenerateAroundPosY2;
	WORD				m_wGenerateMethod;

	CPos				m_pos;			// 위치
	char				m_strName[30];
	WORD				m_wIsBlockToAutoRegen;
	
	BOOL				isNotMoveNpc()	{if (m_wMoveArea	==	dMAX_AREA) return	TRUE;return	FALSE;}

};	//	cBASE_NPC_INFO


#define	dMAP_TILE_MAP		0
#define	dMAP_TONG_MAP		1

#define	dBORDER_ACTOR			10000
#define	dBORDER_DUMMY			5000

#define	dMAX_MAP_SIZE			512*512
#define	dMAX_MAP_COUNT			2048

#define	dMAP_MARGIN				80		//	이 만큼 여분으로 더 검색한다.(건물및 오브젝트 출력)

enum
{
	eTS_GRASSLAND			,	//초원
	eTS_MOUNTAINS			,	//산악
	eTS_DESERT				,	//사막
	eTS_SAVANA				,	//사바나
	eTS_CAVE				,	//동굴
	eTS_DUNGEON				,	//던젼
	eTS_TOWER				,	//타워
	eTS_MINE				,	//광산
	eTS_HELL				,	//지하세계(지옥)
	eTS_HEAVEN				,	//천상계(천국)
	eTS_BRUNENSTIG			,	//마을 - 브룬넨슈티그
	eTS_BIGAEPLE			,	//마을 - 비개플
	eTS_AUGUSTA				,	//마을 - 아우구스타
	eTS_BRIDGEHEAD			,	//마을 - 브리지헤드
	eTS_MOUNTAINS_VILLAGE	,	//마을 - 산악마을
	eTS_ARIAN				,	//마을 - 아리안
	eTS_RUINED_CITY			,	//마을 - 폐허
	eTS_FARMHOUSE			,	//마을 - 농가
	eTS_GYPSY				,	//마을 - 집시촌
	eTS_ROOM				,	//방
	eTS_GUILD_HALL			,	//방
	eTS_COUNT					//수
};

#define	dMASK_BUILDING				0x8000	//	건물
#define	dMASK_OBJECT				0x4000	//	오브젝트
#define	dMASK_BLOCKED				0x2000	//	막힘
#define	dMASK_MISSLIE_BLOCK_OBJECT	0x1000	//	미사일 블럭
#define	dMASK_MISSLIE_BLOCK_TILE	0x0800	//	미사일 블럭 타일

#define	dVIEWPORT_MARGIN		400		//	뷰포트 마진

#define	dMAP_NAME_LENGTH		64
#define	dMAX_BGM_COUNT_IN_MAP	30

enum
{
	eFT_NONE,
	eFT_BATTLE_FIELD,	//	전투 필드
	eFT_VILLAGE,		//	마을
	eFT_INNER_BUILDING,	//	건물 내부
};


class	CMapSimpleInfo
{
public:
	WORD				m_wSerial;
	WORD				m_wWidth,m_wHeight;
	WORD				m_bf8Type			:	8;
	WORD				m_bf1IsPremiumZone	:	1;
	char				m_strName[dMAP_NAME_LENGTH];
	char				m_strFileName[dMAP_NAME_LENGTH];
};

class	CMapLocationInfo
{
public:
	WORD				m_wSerial;

	int					x,y,z;
	WORD				m_wType;
};

class	cMapBaseInfo20050527
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
	DWORD				m_bf1IsEventField		: 1;
	DWORD				m_bf1IsOXQuizGateVillage: 1;
	DWORD				m_bf1IsHiddenPortal		: 1;	//	포탈 모양만 숨긴다.
	DWORD				m_bf4MinimapDisplayLevel: 4;	//	미니맵 표시 레벨
	DWORD				m_bf1IsCanNotMemoryZone	: 1;	//	장소 기억 안되는 장소
	DWORD				m_bf1IsHalfSize			: 1;	//	미니맵 표시 레벨
	DWORD				m_bf1IsWordQuizField	: 1;	//	미니맵 표시 레벨
	DWORD				m_bf1IsWordQuizVillage	: 1;
	DWORD				m_bf1IsBlockToWarpField	: 1;
	DWORD				m_bf1IsBossZone			: 1;
	DWORD				m_bf1IsGuildHall		: 1;
	DWORD				m_bf1Is1LevelGuildHall	: 1;
	DWORD				m_bf1Is1GuildPointBattleField	: 1;
	DWORD				m_bf1IsSiegeWarfareField	:	1;	//25
	DWORD				m_bf1IsCanNotCallCarpetField:	1;	//26
	DWORD				m_bf1IsCanNotUseCallingSkill:	1;	//27
	//JBC		필드몬스터의 길찾기 능력 가동 여부 		08-09-18
	DWORD				m_bf1IsUseFindWay		: 1;	//28	길찾기 기능 사용 여부 
	DWORD				m_bf1IsSetBossMap		: 1;	//29
	DWORD				m_bf1IsGuildDungeonMap		: 1;	//30	길드 던젼 맵//30
	DWORD				m_bf1IsNotUseSkillBeforeTrans	: 1;	//31	몬스터변신 전 스킬 사용 불가
	DWORD				m_bf1IsNotOpenPitchManShop	: 1;	//32	노점상 불가
	int					m_iSerial;					//	프로젝트에서 관리할 시리얼... 아직은 없다.
	WORD				m_aBgmList[dMAX_BGM_COUNT_IN_MAP];
	WORD				m_wLinkSecretDungeon;
	WORD				m_bf1IsExistSecretDungeonGate	: 1;
	WORD				m_bf3WeatherType		: 3;
	WORD				m_bf1IsNight			: 1;			//	5
	WORD				:0;
	short				m_sCorrectFireResistance;
	short				m_sCorrectWaterResistance;
	short				m_sCorrectWindResistance;
	short				m_sCorrectEarthResistance;
	short				m_sCorrectLightResistance;
	short				m_sCorrectDarkResistance;
};

class	cMAP_INFO : public cMapBaseInfo20050527
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

class	cDoorObjectInfo	:	public cDoorObjectSimpleInfo
{
public:
	int				m_iPosValue;
	WORD			m_bf1IsActive	:	1;		//	이 문은 현재 활성화 되어 있다.
	WORD			m_bf1IsOpened	:	1;
	WORD			m_bf1IsDisarmed	:	1;
	WORD			m_bf1IsUnlocked	:	1;
	WORD			m_bf1IsHide		:	1;
	WORD			m_bf1IsDetectDoor:	1;
	WORD			m_bf1IsDetectTrap:	1;

					cDoorObjectInfo()
					{
						reset();
					}

	void			reset()
	{
		m_bf1IsActive	=	FALSE;		//	이 문은 현재 활성화 되어 있다.
		m_bf1IsOpened	=	FALSE;
		m_bf1IsDisarmed	=	FALSE;
		m_bf1IsUnlocked	=	FALSE;
		m_bf1IsHide		=	FALSE;
		m_bf1IsDetectDoor=	FALSE;
		m_bf1IsDetectTrap=	FALSE;
	}
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
};

#define dMAX_SNOW 50
class cSnow
{		// 마지막 수정일 : 09.10.07
		// 눈 !!
public:

	int		m_iStartX;
	int		m_iStartY;

	int		m_iBeforPlayerX;
	int		m_iBeforPlayerY;

	float	m_fCurrentX;
	float	m_fCurrentY;
	int		m_posEndX;
	int		m_posEndY;

	float	m_fScale;
	
	float	m_fMoveDistanceX;
	float	m_fMoveDistanceY;
	
	BOOL	m_bCreate;

	cSnow();
	virtual ~cSnow(){}

	// 10.02.10 수정   virtual
	virtual void create(int _iWidth, int _iHeight);
	virtual void update();

	virtual void reset();

};
// 10.02.10 추가 비
enum 
{
	eRAIN_CREATE,
	eRAIN_STATE,
	eRAIN_ANM
};

#define dMAX_RAIN		200
#define dCHECK_RAIN_ANM	5
class cRain : public cSnow
{
public:

	WORD	m_wState, m_wImage;
	int		m_iFrame, m_iFrameCheck, m_iFrameCount;
	
	cRain::cRain(){	reset();	}
	void create(int _iWidth, int _iHeight);
	void update();
	void reset();

};
class cMAP	:	public	cMAP_INFO , public CGamePlay
{		// 마지막 수정일 : 09.10.19
	int					m_iTextDataLocate;
	int					m_iLoadedTileSet;
	cTILE				m_tile;							//	타일
	BOOL				m_bIsRoom;
	static	cSTRINGS	*s_lpstrMapList;

public:
	char				m_strFileName[64];
	int					m_iFileVersion;
	int					m_iSavedActorCount;
	
	cAREA				m_area;
	cOBJECT				m_object;						//	오브젝트 정보
	cTGA				m_tgaMinimap;
	cDoorManager		m_doors;
	cSnow				m_snow[dMAX_SNOW];				// 눈!!
	cRain				m_rain[dMAX_RAIN];				// 비 10.02.10 추가
	c4POINT				m_pos,m_posView;				//	맵의 위치
	CPos				m_posMove,m_posMM;				//	타일 보정
	int					m_iPixelWidth,m_iPixelHeight;	//	픽셸 단위 사이즈
	int					m_iTPH,m_iTPW;					//	화면에 들어 가는 타일 수

	BYTE				m_aBlockInfo[dMAX_MAP_SIZE];						//	충돌 정보나 각종 정보
	WORD				m_aInfo[dMAX_MAP_SIZE];						//	충돌 정보나 각종 정보
	WORD				m_aMap[dMAX_MAP_SIZE];						//	맵 타일 정보

	WORD				m_awCloseMapList[2048];
	int					m_iCloseMapCount;

	BOOL				m_bIsSkippingDraw;
	BOOL				m_bIsDrawWeather;			// 날씨 그리기..!
	cBASE_NPC_INFO		m_aSaveActor[c_iMaxFieldActorCount];
	
	vector<int>			m_vtPortaldestIndex;
	int					m_iQLinkFieldIndex[dSAVEROAD_COUNT];

						cMAP();
						~cMAP();
	
	WORD				m_wWeather;		// 기후....... 09.10.07 



	int					getDungeonFirstFloor(const int _iMapIndex);
	BOOL				isPortalDest(int _iIndex);
	void				setQuestLinkFieldIndex();


	inline	void		resetSnow()
	{// 눈 초기화~~
		for(int i=0;i<dMAX_SNOW; ++i)
		{
			m_snow[i].reset();
		}
	}

	void				drawWeather();	// 기후 출력~~		09.10.07
	void				drawWeatherData();			// 맵에서 저장한 날씨. 카르마(drawWeather)로 다른날씨를 적용할 경우 실행안됨
	inline	BOOL		isCloseMap(int _iSerial)
	{
		for (int i=0;i<m_iCloseMapCount;i++)
			if (m_awCloseMapList[i]	==	_iSerial)
				return	TRUE;

		return	FALSE;
	}

	void				skippingDraw()
	{
		m_bIsSkippingDraw	=	TRUE;
	}

	void				cancelSkippingDraw()
	{
		m_bIsSkippingDraw	=	FALSE;
	}

	BOOL				isSkippingDraw()
	{
		return	m_bIsSkippingDraw;
	}

	void				readCloseMapList();
	void				buildInfo();
	BOOL				isEventArea(cAreaEvent *_lpArea,int _iX,int _iY);
	BOOL				isFocusDoor(cAreaDoorInfo *_lpArea,int _iX,int _iY);
	void				operateDoorWork();
	void				operateEventObjectWork();
	void				setDoorCrashMap(int _iDoor,BOOL _bIsClear);
	void				setCrashMap(int x,int y,BYTE *_lpCrash,BOOL _bIsClear);
	void				changeDoorStatus(int _iArea,BOOL _bIsSleep,BOOL _bIsHide,BOOL _bIsLocked,BOOL _bIsTrap,BOOL _bIsClose,BOOL _bIsActiveByKarma);
	void				getLinkObjectWithDoor(cAreaDoorInfo *_lpDoor);
	void				getLinkObject(cAreaEvent *_lpArea);

	void				close();
	void				reset();
	
	BOOL				load(char *_lpstrFn,cNUX *_lpNux=NULL);
	void				skipKarmaContainer(cFILE *_lpFile,BOOL _bIsDecodeKaramData);
	void				skipKarmaItem(cFILE *_lpFile,BOOL _bIsDecodeKaramData);
	void				skipKarma(cFILE *_lpFile,BOOL _bIsDecodeKaramData);
	void				skipCC(cFILE *_lpFile,int _iSpeechCount,BOOL _bIsDecodeKaramData);

	int					getMapDataVersion(char *_strHeader);

	BOOL				loadFieldImageData(BOOL _bIsHalfSize,cNUX *_lpNux=NULL,cMCU *_lpMcu=NULL);
	BOOL				loadBodyImageData(cNUX *_lpNux=NULL,cMCU *_lpMcu=NULL);

	BOOL				checkMissileBlockedLine(int _iX,int _iY,int _iDestX,int _iDestY);
	BOOL				isMissileBlockedLine(int _iX,int _iY,int _iDestX,int _iDestY);
	BOOL				isBlockedWay(int _iMx,int _iMy,int _iDestMx,int _iDestMy);

	int					getRoad(int _iX1,int _iY1,int _iX2,int _iY2,CPos *_lpRoad,int _iTerm,int _iLimit,CPos *_lpPosLast);
	void				makeBlock(int _iX1,int _iY1,int _iX2,int _iY2);
	void				removeBlock(int _iX1,int _iY1,int _iX2,int _iY2);

	void				drawMap();			//	맵을 그린다
	void				drawHalfMap();
	void				drawFieldTrap(int _iDetectLevel);
	void				drawObjectForTerminal();
	void				drawObject();		//	맵위에 오브젝트들을 그린다.
	void				drawObjectHalfSize();
	void				drawBlockedTile();	//	못지나가는 타일 표시
	void				drawGrid();			//	격자 표시
	void				DrawEventObjectName(); // 09.08.11	이벤트오브젝트 이름찍기.

	int					getCorrectMagicResistanceLevel(int _iValue);
	int					getCorrectFireResistanceLevel();
	int					getCorrectWaterResistanceLevel();
	int					getCorrectWindResistanceLevel();
	int					getCorrectEarthResistanceLevel();
	int					getCorrectLightResistanceLevel();
	int					getCorrectDarkResistanceLevel();

	inline	void		setPos(int x,int y)
						{	m_pos.x	=	x;
							m_pos.y	=	y;

							ClipValue(m_pos.x,0,m_iPixelWidth	-g_iScreenWidth -1);
							ClipValue(m_pos.y,0,m_iPixelHeight	-g_iScreenHeight-1);
						}

	inline	void		setAttr	(int x,int y,int attr)	{m_aInfo[x+y*m_iWidth]		=	attr;}
	inline	BOOL		isBlank(int x,int y)			{if (m_aBlockInfo[x+y*m_iWidth]	==	0) return TRUE;	return FALSE;}
	inline	BOOL		isBlocked(int x,int y)			{if (m_aBlockInfo[x+y*m_iWidth]) return TRUE;	return FALSE;}
	inline	BOOL		isRoom()						{return m_bIsRoom;}
	inline	BOOL		isMissileBlocked(int x,int y)	
	{
		int	iAttr	=	m_aInfo[x+y*m_iWidth];

		if (iAttr&dMASK_MISSLIE_BLOCK_TILE	) return TRUE;
		if (iAttr&dMASK_MISSLIE_BLOCK_OBJECT) return TRUE;
		if (iAttr&dMASK_BUILDING			) return TRUE;
		if (iAttr	==	2					) return TRUE;

		return FALSE;
	}

	static	BOOL		Init(cNUX *_lpNux,BOOL _bIsWantExportList=FALSE);

	static	char*		GetFieldName(char *_lpstrFileName);
	static	char*		GetFieldName(int _iIndex);
	static	BOOL		GetFieldInfo(cMAP_INFO *_lpMapInfo,char *_lpstrFileName,cNUX *_lpNux);

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃미니맵
//┃miniMap.cpp
	int					m_iMMFrameWidth,m_iMMFrameHeight;
	int					m_iMMDx,m_iMMDy;
	cRECT				s_mmFrame;
	BOOL				m_bFogmapState;
	cBARMENU			m_bmCheckBox;		// 미니맵 옵션 체크박스

	int					m_iOptionBntState;
	int					m_iQuestIndex[dMAX_PROCESS_QUEST_COUNT];	// 몇번째 슬롯의 퀘스트인지 저장
	cRECT				m_rectQuestListCloseBtn;
	int					m_iQuestListCloseBtnState;
	BOOL				m_bQuestListState;
	int					m_iSelectQuest;	
	int					m_iFocusQuest;
	cRECT				m_QuestListArea[dMAX_PROCESS_QUEST_COUNT+1];
	int					m_iFogmapAlpha;
	int					m_iRBtnClickTime;
	int					m_iPutPosX, m_iPutPosY;			// 포그맵 좌상단 출력좌표
	int					m_iFogmapWidth, m_iFogmapHeight;	// 포그맵 넓이 높이
	BOOL				m_bMoveFogmap;

	cRECT				m_rtOptionWnd;		// 옵션창을 띄울 자리  .. m_posOptionWnd -> m_rtOptionWnd 로 바뀜
	DWORD				m_dwResultOptionwindow;
	BOOL				m_bOptionState;
	cSAVEROAD			m_srField[dSAVEROAD_COUNT];

	void				FindQuestLinkField();
	void				drawMinimapOption();
	BOOL				getMMCPosToReal(CPos *_lpPosReal);
	void				updateMinimap(cRECT *_lpFrame,CPos *_lpPosHero);
	BOOL				drawMinimap(BOOL _bIsHalfBlending,BOOL _bIsView);//	미니맵 출력
	BOOL				drawEventNpcToolTip();

	BOOL				DrawFogMap();
	void				drawQuestList();
	void				drawMinimapOffscreenQuestNpcArrow(char* _strName, CPos* _pos, cRECT* _rectForMark);
	void				drawMinimapOnscreenQuestNpcArrow(char* _strName, CPos* _pos, cRECT* _mmFrame, int _iPutPosX=0xffff, int _iPutPosY=0xffff, int _iAlpha2=32);
	void				drawHuntingArea(cRECT* _rectFrame);
	void				drawFogmapHuntingArea();
	int					ChoiceArrowDirection(CPos* _pos, cRECT* _rectForMark);		// 퀘스트관련 맵 범위 밖에 있을때 화살표 방향을 결정

	void				drawQuestLinkDungeonArrow(cRECT* _rectForMark, BOOL _bFogmap = FALSE);
	void				drawQuestListDetail(int _iQuestListIndex, int _iPutPosX, int _iPutPosY);
	void				drawQuestMonsterArea(cRECT* _rectForMark, BOOL _bFogmap = FALSE);
	void				drawQuestLinkDungeonToolTip(cRECT* _rectForMark, cAreaInfo* _lpArea);
//┃miniMap.cpp
//┃미니맵
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

};

extern	cRECT				g_rectViewPort;			//	현재 화면에 보이는 영역
extern	cMAP				g_map;
extern	CMapSimpleInfo		g_aMapSimpleInfo[dMAX_MAP_COUNT];
extern	CMapLocationInfo	g_aMapLocationInfo[dMAX_MAP_COUNT];
extern	BOOL				g_bIsHalfSize;

inline	int	
GetScaledXPos(int _iX)
{
	if	(g_bIsHalfSize)
		return	(_iX-g_map.m_posView.x*2)/2;

	return	_iX	-	g_map.m_pos.x;
}

inline	int	
GetScaledYPos(int _iY)
{
	if	(g_bIsHalfSize)
		return	(_iY-g_map.m_posView.y*2)/2;

	return	_iY	-	g_map.m_pos.y;
}

inline	int	
GetCorrectViewXPos()
{
	if	(g_bIsHalfSize)
		return	(g_map.m_posView.x*2)/2;

	return	g_map.m_pos.x;
}

inline	int	
GetCorrectViewYPos()
{
	if	(g_bIsHalfSize)
		return	(g_map.m_posView.y*2)/2;

	return	g_map.m_pos.y;
}

extern	int		g_iGameScale;
extern	BOOL	g_bIsGuildBattleField;

#endif