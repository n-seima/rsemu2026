#ifndef	_classMAP_H
#define	_classMAP_H

#pragma warning(disable: 4786)
#pragma comment(lib,"ws2_32.lib")

#include "SFC.H"
#include "cIMAGE.H"
#include "cBARMENU.H"
#include "cAREA.H"
#include "cOBJECT.H"
#include "cKARMA.H"
#include "cCHARACTER.H"
#include "cVALUE.H"
#include "cBUILDING.H"
#include "cACTOR.H"
#include "CPattnCtrl.hpp"
#include "CShop.h"
#include "patrolRoad.h"
#include "pattern.h"

#define	dBLOCKED_SIZE		1

#define	dSHARE_TILE			1
#define	dSHARE_TILES		(dSHARE_TILE*dSHARE_TILE)
#define	dTILE_WIDTH			64
#define	dTILE_HEIGHT		32

#define	dTILE_REAL_WIDTH	(dTILE_WIDTH/dSHARE_TILE)
#define	dTILE_REAL_HEIGHT	(dTILE_HEIGHT/dSHARE_TILE)

//
//	Tile Set
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
	eTS_ROOM				,	//마을 - 집시촌
	eTS_GUILD_HALL			,	//길드 홀
	eTS_COUNT					//마을 - 집시촌
};

// 10.01.12 
struct MEMO_INFO
{
	POINT	pos;
	POINT	size;
	TCHAR	*text;
	char	tmfile[256];
	char	tmofile[256];
};


class	CBlockedTileInfo
{
public:
	int				m_iBegin,m_iEnd;

	inline	BOOL	isInrange(int _iIndex)	
	{
		if (_iIndex >= m_iBegin && _iIndex <= m_iEnd)	return	TRUE;
		return	FALSE;
	}
};

#define	dMAX_BLOCKED_INFO_COUNT		40

class	CBlockedTileInfoBundle
{
public:
	int					m_iBlockedInfoCount,m_iCrashedInfoCount;

	CBlockedTileInfo	m_aBlockedInfo[dMAX_BLOCKED_INFO_COUNT];
	CBlockedTileInfo	m_aCrashedInfo[dMAX_BLOCKED_INFO_COUNT];


						CBlockedTileInfoBundle()
						{
							m_iBlockedInfoCount	=	0;
							m_iCrashedInfoCount	=	0;
						}

	inline	BOOL		isMissileBlockedTile(int _iIndex)
	{
		for (int i=0;i<m_iBlockedInfoCount;i++)
			if (m_aBlockedInfo[i].isInrange(_iIndex))	return	TRUE;

		return	FALSE;
	}

	inline	BOOL		isBlockedTile(int _iIndex)
	{
		for (int i=0;i<m_iCrashedInfoCount;i++)
			if (m_aCrashedInfo[i].isInrange(_iIndex))	return	TRUE;

		return	FALSE;
	}

	BOOL				addBlockedInfo(int _iBegin,int _iEnd)
	{
		if (m_iBlockedInfoCount	>=	dMAX_BLOCKED_INFO_COUNT)	return	FALSE;

		m_aBlockedInfo[m_iBlockedInfoCount].m_iBegin	=	_iBegin;
		m_aBlockedInfo[m_iBlockedInfoCount].m_iEnd		=	_iEnd;

		m_iBlockedInfoCount++;

		return	TRUE;
	}

	BOOL				addCrashedInfo(int _iBegin,int _iEnd)
	{
		if (m_iCrashedInfoCount	>=	dMAX_BLOCKED_INFO_COUNT)	return	FALSE;

		m_aCrashedInfo[m_iCrashedInfoCount].m_iBegin	=	_iBegin;
		m_aCrashedInfo[m_iCrashedInfoCount].m_iEnd		=	_iEnd;

		m_iCrashedInfoCount++;

		return	TRUE;
	}
};

extern	CBlockedTileInfoBundle	g_aBlockedInfo[eTS_COUNT];

#define	dMAX_MAP			5
#define	dMAX_MAP_WIDTH		512
#define	dMAX_MAP_HEIGHT		512

const	DWORD	dMAX_MAP_SIZE		=	dMAX_MAP_WIDTH*dMAX_MAP_HEIGHT*dSHARE_TILES;

#define	dCRASH_VALID		0xff
#define	dCRASH_TILE			0xfe

#define	dMASK_BUILDING				0x8000	//	건물
#define	dMASK_OBJECT				0x4000	//	오브젝트
#define	dMASK_BLOCKED				0x2000	//	막힘
#define	dMASK_MISSLIE_BLOCK_OBJECT	0x1000	//	미사일 블럭
#define	dMASK_MISSLIE_BLOCK_TILE	0x0800	//	미사일 블럭 타일

#define	dMAX_OBJECT_COUNT			2048

enum
{
	eFT_NONE,
	eFT_BATTLE_FIELD,	//	전투 필드
	eFT_VILLAGE,		//	마을
	eFT_INNER_BUILDING,	//	건물 내부
};

#define	dMAP_NAME_LENGTH		64
#define	dMAX_BGM_COUNT_IN_MAP	30

class	CMapBaseInfo
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
	DWORD				m_bf1IsGuildPointBattleField	:	1;
	DWORD				m_bf1IsSiegeWarefareField	:	1;
	DWORD				m_bf1IsCanNotCallCarpetField:	1;
	DWORD				m_bf1IsCanNotUseCallingSkill:	1;
	//JBC		필드몬스터의 길찾기 능력 가동 여부 		08-09-18
	DWORD				m_bf1IsUseFindWay			: 1;	//	길찾기 기능 사용 여부 
	DWORD				m_bf1IsSetBossMap			: 1;	//	세트 보스가 존재 하는 맵이냐?]
	DWORD				m_bf1IsGuildDungeonMap		: 1;	//	길드 던젼 맵//30
	DWORD				m_bf1IsNotUseSkillBeforeTrans	: 1;	//31	몬스터변신 전 스킬 사용 불가
	DWORD				m_bf1IsNotOpenPitchManShop		:	1	;	//32	노점상 열기 불가
	int					m_iLastSaveIP;	//	마지막에 저장한 IP주소	//int					m_iSerial;	//	프로젝트에서 관리할 시리얼... 아직은 없다.

};

class	CMapBaseInfo20031111 : public CMapBaseInfo
{
public:
	WORD				m_aBgmList[dMAX_BGM_COUNT_IN_MAP];
	WORD				m_wLinkSecretDungeon;
	WORD				m_bf1IsExistSecretDungeonGate	: 1;		// 수정
	WORD				m_bf3WeatherType		: 3;		// 날씨 10.02.10 추가 
	WORD				m_bf1IsNight			: 1;		//5
	WORD				:	0;
};
//
//	맵의 기본정보
class	cMapBaseInfo20050527	: public CMapBaseInfo20031111
{
public:
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


class	cMAP	:	public	cMAP_INFO
{
public:
	int					m_iSerial;
	int					m_iRealWidth,m_iRealHeight;
	cBUILDING			m_building;
	cAREA				*m_pArea;
	cOBJECT				m_object;
	cCHARACTER_SET		m_setCharacter;
	cACTOR_SET			m_setActor;
	cVALUE				m_Value;
	CShopManager		m_shop;
	CCustomItemManager	m_customItem;
	cPatrolRoadManager		m_prManager;
	cCharacterPatternPack	m_cpp;

	CPattnCtrl			*m_pPattern;		//	타일 패턴 컨트롤
	BYTE				m_aTilePattern[50];	//	타일 패턴
	int					m_iPatternCount;	//	타일 패턴의 수
	WORD				*m_lpTileMap;		//	블랭크 타일,오버랩 타일 정보 셋팅을 위한 임시 버퍼를 가르키는 포인터
											//	저장할때만 필요하다.

	BYTE				*m_pBuildingMap;		//	건물
	WORD				*m_pObjectMap;			//	오브젝트
	BYTE				*m_pCrashMap;			//	블럭정보

	cPOINT				m_pos,m_posPixel;
	int					m_iSelectTile;
	int					m_iSelectBuilding;
	int					m_iSlctArea;
	int					m_iSelectTeam;		//	선택한 팀

	BOOL				m_isVIRGIN;
	BOOL				m_isTileExpand;		//	타일 확장
	char				m_strFName[512];

	char				m_astrFieldValue[50][32];
	CKarmaContainer		m_karma;

	BOOL				m_bIsLockedBySVN;
	// 10.01.12
	MEMO_INFO			m_sMemoInfo;
						cMAP();
						~cMAP();

	void				Reset();
	void				RemarkBlankAndOverlapInfo(WORD *_lpMapInfo);	//	블랭크 타일,오버랩 타일 정보를 그려준다.
	void				RemarkBrigeObject(WORD *_lpMapInfo);	//	블랭크 타일,오버랩 타일 정보를 그려준다.
	void				RemarkCrashMap();

	void				updateAddonObject();
	void				exportText(BOOL _bIsSD=FALSE,BOOL _bIsExportSomeMap=FALSE);
	void				exportNpcInfoWithDialog();

	static	int			CompareText(char *_lpstrFileName,BOOL _bIsSD=FALSE,BOOL _bIsWantCheck=FALSE);
	static	BOOL		ForceSyncText(char *_lpstrFileName,BOOL _bIsSD=FALSE);
	static	int			ModifyText(char *_lpstrFileName,BOOL _bIsSD=FALSE);
	BOOL				importText(BOOL _bIsSD=FALSE,BOOL _bIsPart=FALSE);


	static	BOOL		UploadTextForCompare(char *_lpstrFileName,void *_lpText);
	inline	int			getCrash(int _iX,int _iY)	{return	m_pCrashMap[_iX+_iY*m_iRealWidth];}
	//JBC	충돌 체크 타일에 맘대로 값을 넣는다 0은 이동 가능 1은 이동 불가능 이다.	08-09-10
	inline	void		setCrash(int _iX,int _iY , int _iInputValue = 1)	{m_pCrashMap[_iX+_iY*m_iRealWidth] = _iInputValue;}

	BOOL				getLock(BOOL _bIsByCommand=FALSE);
	BOOL				releaseLock(BOOL _bIsByCommand=FALSE);
	BOOL				updateSVN(BOOL _bIsByCommand=FALSE);
	BOOL				commitSVN(BOOL _bIsByCommand=FALSE);

	// 10.01.12
	void				LoadMemoFile();
	void				SaveMemoFile();

	// 마지막에 저장한 컴퓨터의 IP 주소 저장
	void				setLastSaveIP();
	char*				getLastSaveIP();
	BOOL				isSameMapInfo(cMAP_INFO* _lpMapInfo);	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃건물 관련
//┃cMAP_BUILDING.cpp
	cBUILDING_INFO		*m_lpEditAddonObjectBuilding;

	inline	BOOL		moveSelectActor(int _iDx,int _iDy)
	{
		if (m_setActor.moveSelectActor(-_iDx,_iDy))
		{
			LostVirgin();

			return	TRUE;
		}

		return	FALSE;
	}

	inline	void		freeSelectedAllBuilding(){m_building.freeSelectedAll();}
	void				removeSelectedBuilding();//	선택된 건물들 제거
	void				remarkAllBuildingInfo(BOOL _isBuildCrashMap,BOOL _isBuildBlankMap,BOOL _isBuildOverlapMap);
	BOOL				isCrashBuilding(int _iMx,int _iMy,int _iBuilding);//이 건물.. _iMx,_iMy에 찍으려고 하는데.. 충돌 하냐?
	BOOL				buildBuilding(int _iX,int _iY,int _iBuilding);
	inline	void		setBuilding(int x,int y,BYTE building)
	{	
		if (x < 0 || y <0 || x >= m_iRealWidth || y >= m_iRealHeight)	return;

		m_pBuildingMap[x+y*m_iRealWidth] = building;
	}
	inline	int			getFocusBuilding(int _iMx,int _iMy){return m_building.getFocusBuilding(_iMx,_iMy);}
	inline	BYTE		getBuilding(int x,int y){return m_pBuildingMap[x+y*m_iRealWidth];}
	inline	cBUILDING_INFO	*getBuilding(int _iIndex){return	m_building.get(_iIndex);}
	inline	void		putRegisteredBuilding(int _iBuilding,int _iX,int _iY){m_building.putRegisteredBuilding(_iBuilding,_iX,_iY,m_object.m_pTinyObjectImage);}
	inline	void		putRegisteredBuildingScale(int _iBuilding,int _iX,int _iY,int _iScale)
	{
		m_building.putRegisteredBuildingScale(_iBuilding,_iX,_iY,m_object.m_pTinyObjectImage,_iScale);
	}
	inline	void		putBuilding(int _iBuilding,int _iX,int _iY,BOOL _isRed){m_building.put(_iBuilding,_iX,_iY,_isRed);}
	inline	void		putBuilding(int _iBuilding,int _iX,int _iY,int _iXs,int _iYs){m_building.put(_iBuilding,_iX,_iY,_iXs,_iYs);}
	inline	void		putBuilding(int _iBuilding,int _iIndex,int _iX,int _iY,int _iXs,int _iYs){m_building.put(_iBuilding,_iIndex,_iX,_iY,_iXs,_iYs);}
	inline	int			getBuildingImageCount(){return	m_building.m_iDataCount;}//빌딩 이미지 수
	inline	void		selectBuilding(cRECT *_lpRect){m_building.select(_lpRect);}
	inline	cOBJ_ANM*	getBuildingImage(int _iIndex){return	m_building.getBuildingImage(_iIndex);}


	cBUILDING_INFO *	getEditAddonObjectBuilding();
	BOOL				addAddonObjectOnBuilding(int _iObject,int _iX,int _iY);
	void				cycleSelectAddonObjectOnBuilding();
	void				removeSelectAddonObjectOnBuilding();
	void				moveSelectAddonObjectOnBuilding(int _iDx,int _iDy);

//┃cMAP_BUILDING.cpp
//┃건물 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃맵 정보 추출
//┃exportMapInfo.cpp
	void				exportExpReward(cFILE *_lpFile);
	void				exportKarma(cFILE *_lpFile);
	WORD				GetReactionIndex(CKarmaItem	*_lpReaction);

//┃exportMapInfo.cpp
//┃맵 정보 추출
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃오브젝트 관련
//┃cMAP_OBJECT.cpp
	cFIXED_OBJECT_INFO	*m_lpEditAddonObject;
	
	void				removeSelectedObject();
	void				remarkAllObjectInfo(BOOL _isBuildCrashMap,BOOL _isBuildBlankMap,BOOL _isBuildOverlapMap);
	void				remarkAllBridgeInfo();	//	브릿지 정보 설정
	BOOL				isCrashFixedObject(int _iObject,int _iMx,int _iMy);
	BOOL				buildObject(int _iObject,int _iX,int _iY);
	BOOL				addTinyObject(int _iObject,int _iX,int _iY);
	BOOL				addAddonObject(int _iObject,int _iX,int _iY);
	void				cycleSelectAddonObject();
	void				removeSelectAddonObject();
	void				moveSelectAddonObject(int _iDx,int _iDy);
	cFIXED_OBJECT_INFO	*getEditAddonObject();


	inline	void		setObject(int _iObject,int x,int y)
	{	
		if (x < 0 || y <0 || x >= m_iRealWidth || y >= m_iRealHeight)	return;

		m_pObjectMap[x+y*m_iRealWidth] = _iObject;
	}
	inline	int			getObject(int _iX,int _iY)	{return m_pObjectMap[_iX+_iY*m_iRealWidth];}
	inline	void		selectObject(cRECT *_lpRect){m_object.select(_lpRect);}

//┃cMAP_OBJECT.cpp
//┃오브젝트 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	void				select(cRECT *_lpRect,cRECT *_lpRectScreen);//선택
	void				RoundCrash();
	void				Create();		
	void				Init(int xs,int ys);
	BOOL				Init(int xs,int ys,int tile,int serial);
	BOOL				Init(cMAP_INFO *mapInfo,int serial);
	BOOL				Resize(char* _strName,int _iX, int _iY, int _iWidth,int _iHeight,int	_iFieldType);	//	맵 사이즈및 기타 정보를 재설정 한다.
	void				FreeSelect();
	void				draw(int _iScale=100);
	void				drawWholeImage(int _iScale);
	BOOL				IsActive(){if (m_iSerial==0xffff)return FALSE;return TRUE;}
	BOOL				loadMapsetImage();//맵셋 이미지 로딩

	inline void			setPixelPos()
	{
		m_posPixel.x	=	m_pos.x*dTILE_REAL_WIDTH;
		m_posPixel.y	=	m_pos.y*dTILE_REAL_HEIGHT;
	}


	void				LostVirgin();
	void				SetVirgin()		{m_isVIRGIN	=	TRUE;}
	BOOL				IsVirgin()		{return m_isVIRGIN;}


	WORD				GetTile(int x,int y);
	BYTE				GetObject(int x,int y);
	void				PutTile(int x,int y,WORD tile,BOOL _isExpand);		//	타일 찍기
	void				BuildCliff(int x,int y);				//	절벽찍기

	BOOL				SetCrash(int x,int y,BYTE block);
	BOOL				SetBlank(int x,int y,BYTE block);
	BOOL				SetOverlap(int x,int y,BYTE block);
	BOOL				isCrashTile(int x,int y);
	void				setPos(int x,int y);
	BOOL				IsBlank(int x,int y);
	BOOL				IsOverlap(int x,int y);

	BOOL				isMissileBlockedTile(int x,int y);
	BOOL				isBlockedTile(int x,int y);
	BOOL				isMisslieBlockedObjectTile(int x,int y);	//	미사일 공격을 막는 오브젝트 영역에 타일

	void				PutCrashBlock(int x,int y,int dx,int dy,BYTE *crash,WORD color);

	BOOL				isBuildAblePos(int x,int y,BYTE *crash);
	void				setBridgeMap(int x,int y,BYTE *crash);
	BOOL				setCrashMap(int x,int y,BYTE *crash,BYTE *block,BYTE *overlap,BOOL clear,BOOL _bIsMissileBlocked=FALSE);
	BOOL				SetBlankMap(int x,int y,BYTE *block,BOOL clear);

	inline	void		selectTile			(int _iTile){m_iSelectTile			=	_iTile;}
	inline	void		selectBuilding		(int _iBuilding){m_iSelectBuilding	=	_iBuilding;}
	inline	void		selectArea			(int _iArea)	{m_pArea->Select(_iArea);}
	void				buildScaledTile(int _iScale);


	static	WORD		**s_pTile;

	static	BOOL		GetMapInfo(char *_strFn,cMAP_INFO *_lpMapInfo);
	static	BOOL		IsMapFile(char *_strFn);			//	맵 화일이냐?
	static	int			GetVersion(char *_strHeader);		//	버젼 판별

	static	BOOL		Select(WORD map);
	static	BOOL		Add(int xs,int ys,int tile);
	static	BOOL		Add(cMAP_INFO *mapInfo);
	static	BOOL		Reset(cMAP_INFO *mapInfo,int index);

	static	int			GetValidMap();
	static	int			GetFirstMap();
	static	BOOL		Init(int xs,int ys,int tile,char *name);
	static	BOOL		Load(char *fn,int index, cMAP* lpMapDataLoad = NULL);	//	로딩		세번째 인자는 .. 맵로딩할때 데이터들만 세팅한다~!
	static	BOOL		Save(char *fn,int index,char *_lpstrSaveFileName=NULL,BOOL _bIsTempSave=FALSE,char *_lpstrTargetFolder=NULL,BOOL _bIsLocalize=FALSE);	//	로딩
	static	BOOL		AutoSave(char *fn,int index,char *_lpstrSaveFileName=NULL);	//	09.07.24자동저장

	static	void		SaveKarmaContainer(cFILE *_lpFile,CKarmaContainer *_lpKC);
	static	void		ReadKarmaContainer(cFILE *_lpFile,CKarmaContainer *_lpKC,BOOL _bIsUseDecode);
	static	void		SaveKarma(cFILE *_lpFile,CKarma *_lpKarma);
	static	void		ReadKarma(cFILE *_lpFile,CKarma *_lpKarma,BOOL _bIsUseDecode);

	static	void		SaveKarmaItem(cFILE *_lpFile,CKarmaItem *_lpKarma);//	카르마 저장하기
	static	void		ReadKarmaItem(cFILE *_lpFile,CKarmaItem *_lpKarma,BOOL _bIsUseDecode);//	카르마 읽기

	static	void		WriteCC(cFILE *_lpFile,CConversationContainer *_lpcc);
	static	void		ReadCC(cFILE *_lpFile,CConversationContainer *_lpCC,int _iCount,BOOL bIsDecodeKaramData=FALSE);


	static	void		Close(int index);

	class	CPortalAreaInfo
	{
	public:
		WORD	m_wSerial;
		char	m_strName[dSIZEOF_AREA_NAME];
	};

	static	cAREA*		GetArea(char *_lpstrMapname);
	static	BYTE*		LoadBlock(char *_lpstrMapname,cPOINT *_lpSize);
	static	BOOL		GetPortalAreas(char *_lpstrMapname,CPortalAreaInfo *_lpPortalAreas,int *_iCount);
	static	int			GetPortalArea(char *_lpstrMapname,char *_lpstrAreaName);
	static	BOOL		CheckPortalArea(char *_lpstrMapname,char *_lpstrSrcAreaName,char *_lpstrAreaName);
};

extern	char		*g_strMapType[];							//	맵 타입 스트링
extern	cMAP		*g_lpActmap;							//	현재 활성화된 맵
extern	cMAP		g_aMap[dMAX_MAP];
extern	int			g_iSelectMap;
extern	cBARMENU	g_bmMapList;
extern	char		*g_strWeatherType[];					// 날씨 10.02.10 추가

extern	BOOL		IsOpenMap(char *fn,int index=0xffff);	//	열려 있는 맵인지 확인한다.
extern	char		*g_strTileSet[];

#endif