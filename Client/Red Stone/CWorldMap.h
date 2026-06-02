#ifndef _classWorldMap_h
#define	_classWorldMap_h

#include "CGamePlay.h"

#define	dMAX_LINK_DUNGEON_COUNT				10
#define	dMAX_MAIN_FIELD_COUNT				200
#define	dMAX_WM_ROAD_POINT_COUNT			20
#define	dMAX_WM_MAX_DUNGEON_COUNT			50
#define	dWM_FIELD_NAME_LENGTH				64
#define	dWM_DUNGEON_LEVEL_COMMENT_LENGTH	20
#define	dMAX_ROAD_INFO_COUNT				100
#define	dMAX_DUNGEON_INFO_COUNT				200
#define	dMAX_WM_MAX_ROAD_NODE_COUNT			10
#define	dMAX_WM_NODE_COUNT					1024
#define dWM_BASIC_WINDOW_WIDTH					225
#define	dWM_BASE_WIDTH						800
#define	dWM_BASE_HEIGHT						600

inline	int
GetWorldMapScaleRate()
{
	int	iRateX	=	(g_iScreenWidth*100)/dWM_BASE_WIDTH;
	int	iRateY	=	(g_iScreenHeight*100)/dWM_BASE_HEIGHT;

	return	iRateX < iRateY ? iRateX : iRateY;
}

inline	int
GetWorldMapOffsetX()
{
	int	iRate	=	GetWorldMapScaleRate();

	return	(g_iScreenWidth-((dWM_BASE_WIDTH*iRate)/100))/2;
}

inline	int
GetWorldMapOffsetY()
{
	int	iRate	=	GetWorldMapScaleRate();

	return	(g_iScreenHeight-((dWM_BASE_HEIGHT*iRate)/100))/2;
}

inline	int
GetWorldMapX(int _iX)
{
	return	GetWorldMapOffsetX()+((_iX*GetWorldMapScaleRate())/100);
}

inline	int
GetWorldMapY(int _iY)
{
	return	GetWorldMapOffsetY()+((_iY*GetWorldMapScaleRate())/100);
}

enum
{
	eWMM_FIELD_INFO	=	1000,
	eWMM_PARTY_INFO,
	eWMM_QUEST_INFO,
	eWMM_GUILD_INFO,
	eWMM_FRIEND_INFO,
	eWMM_CUCKOO_INFO,
	eWMM_RETURN_TO_GAME,
};

class	cWM_mainFieldInfo
{
public:
	WORD	m_wSerial;
	WORD	m_wFieldSerial;
	char	m_strFileHeader[20];
	cRECT	m_rectFrame;
	WORD	m_wLinkDungeonCount;
	WORD	m_awLinkDungeon[dMAX_LINK_DUNGEON_COUNT];
	WORD	m_wIsNew;
	CPos	m_posFlagPos;
	WORD	m_wVillageShape;
	WORD	m_wLowLevel,m_wTopLevel;
};

class	cWM_roadInfo
{
public:
	WORD	m_wSerial;
	WORD	m_wBeginIndex;
	WORD	m_wEndIndex;
	WORD	m_wRoadLength;
	WORD	m_wRoadPointCount;
	CPos	m_aposRoadPoint[dMAX_WM_ROAD_POINT_COUNT];
};

class	cWM_nodeInfo
{
public:
	WORD	m_wSerial;
	WORD	m_wNodeCount;
	WORD	m_awLinkNode[dMAX_WM_MAX_ROAD_NODE_COUNT];

			cWM_nodeInfo()
			{
				reset();
			}

	void	reset()
	{
		m_wNodeCount	=	0;
	}

	BOOL	addNode(int _iField)
	{
		if (m_wNodeCount	>=	dMAX_WM_MAX_ROAD_NODE_COUNT)
			return	FALSE;

		for (int i=0;i<m_wNodeCount;i++)
			if (m_awLinkNode[i]	==	_iField)
				return	TRUE;

		m_awLinkNode[m_wNodeCount]	=	_iField;

		m_wNodeCount++;

		return	TRUE;
	}
};

class	cWM_road
{
public:
	WORD	m_wNodeCount;
	WORD	m_awNodeList[dMAX_WM_NODE_COUNT];

			cWM_road()
			{
				m_wNodeCount	=	0;
			}
			
	BOOL	isExistNode(int _iField)
	{
		for (int i=0;i<m_wNodeCount;i++)
			if (m_awNodeList[i]	==	_iField)
				return	TRUE;

		return	FALSE;
	}

	void	addNode(int _iField)
	{
		if (m_wNodeCount	>=	dMAX_WM_NODE_COUNT)
			return;

		m_awNodeList[m_wNodeCount++]	=	_iField;
	}

};

class	cWM_dungeonInfo
{
public:
	WORD	m_wSerial;
	WORD	m_wDungeonCount;
	WORD	m_awDungeonList[dMAX_WM_MAX_DUNGEON_COUNT];
	WORD	m_wIsNew;
	WORD	m_wLowLevel,m_wHighLevel;
};

class	cWorldMapFieldInfo
{
public:
	cWM_mainFieldInfo		m_aFieldList[dMAX_MAIN_FIELD_COUNT];
	cWM_roadInfo			m_aRoadList[dMAX_ROAD_INFO_COUNT];
	cWM_dungeonInfo			m_aDungeonList[dMAX_DUNGEON_INFO_COUNT];
	cWM_nodeInfo			m_aNodeList[dMAX_WM_NODE_COUNT];
	int						m_iFieldInfoCount,m_iRoadInfoCount,m_iDungeonInfoCount;
	cWM_road				m_resultPath;
	int						m_iCurrentField,m_iSurfaceField,m_iCurrentFieldIndex,m_iSurfaceFieldIndex,m_iSelectFieldIndex,m_iFocusFieldIndex;
	int						m_iWorldmapMenu;
	int						m_iSelectFieldSerial;

	void					getShortestNode(int _iBegin,int _iEnd,cWM_road *_lpRoad,cWM_road *_lpResultRoad);

	BOOL					readData();
	BOOL					makePathTo(int _iEnd);

	void					drawFieldFrame(int _iMode);
	void					drawRoad(int _iBeginField,int _iEndField);
	void					buildNode();

	int						getRoadLength(cWM_road *_lpRoad);
	int						getRoadLength(int _iPreField,int _iNextField);
	int						getSurfaceField(int _iField);		//	표면 필드
	void					setCurrentField(int _iCurrentField);

	int						getFocusedField(int _iX,int _iY);	//	포커스가 간 필드

	BOOL					isLinkedDungeon(int _iField,int _iDungeonSerial);
	void					update(int _iX,int _iY,BOOL _bIsTouchLeftButton,BOOL _bIsForGateGlove=FALSE);

};



class	CMiniQuestInfo
{
public:
	char					m_strName[104];
	int						m_iFieldNameHeight;
	cRECT					m_rtQuestpos[dMAX_PROCESS_QUEST_COUNT];			// 퀘스트리스트 위치정보!
	cSTRING					m_strQuestName[dMAX_PROCESS_QUEST_COUNT];		// 퀘스트 이름
	cSTRING					m_strQuestInfo[dMAX_PROCESS_QUEST_COUNT];		// 퀘스트 정보
};

class	CWorldMap : public CGamePlay
{
public:
	int						m_iSelectFieldSerial;
	int						m_iUseItemSlotIndex;
	int						m_iPreFocusField;		// 이전 포커스 필드 (update 횟수를 줄이기 위해서 필요)
	CPos					m_posPut;				// 화면에 기본창을 찍어줄 위치
	CPos					m_posQuest;				// 퀘스트정보를 찍어줄 위치
	cSTRING					m_strDungeonInfo;		// 연결된 던전 정보

	CMiniQuestInfo			m_MiniQuestInfo;

	BOOL					m_bDetailView;				// 마우스클릭시 계속 정보를 보여줄 것인지 체크할 변수
	int						m_iViewField;				// 마우스클릭시 필드
	int						m_iDetailFocusQuest;		// 포커스가 간 퀘스트
	vector<int>				m_vtiQFIndexList;

	void					setQuestField();
	
	int						getInfoSurfaceField();
	int						getInfoCurrentField();
	void					UpdateTotalInfo();			// 기본정보를 update 한다.
	void					DrawTotalInfo();			// 기본정보를 출력한다.
	void					DrawQuestInfo();			// 퀘스트정보를 출력한다.
	BOOL					open(BOOL _bIsForGateGlove=FALSE,int _iItemSlotIndex=-1);
	void					close();
	void					draw();

	void					drawFieldInfoTab();
	void					drawPartyInfoTab();
	void					drawQuestInfoTab();

	void					updateFieldInfoTab();
	void					updatePartyInfoTab();
	void					updateQuestInfoTab();

	BOOL					update();
	BOOL					isOpened();
	int						getInfoSurfaceFieldIndex(int _iFieldindex);
	BOOL					setInforeadData();
	void					setInfoCurrentField(WORD _wField);
	int						getInfoCurrentFieldIndex();
	void					getInfoShortestNode(int _iBegin,int _iEnd,cWM_road *_lpRoad,cWM_road *_lpResultRoad);
	int						getParentFieldIndex(int _iIndex);
private:
	BOOL					m_bIsForGateGlove;
	cWorldMapFieldInfo		m_info;
	cRECT					m_rectInfoFrame;
	cBARMENU				m_bmRadioButton;
	int						m_iWorldmapMenu;
	int						m_iSelectPartyMember,m_iFocusPartyMember;
	int						m_iSelectQuest,m_iFocusQuest;
};

extern	CWorldMap	g_gwWorldMap;

#endif
