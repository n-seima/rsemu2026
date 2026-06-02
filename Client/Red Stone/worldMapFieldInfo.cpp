#include "CWindowInterface.h"
#include "CHero.h"
#include "CParty.h"
#include "cMAP.h"
#include "CMessage.h"

enum
{
	eWM_ROAD_POINT		=	51,
	eWM_FLAG_SHAPE_CAMP	=	54,
	eWM_FLAG_SHAPE_TENT		,
	eWM_FLAG_SHAPE_VILLAGE	,
	eWM_FLAG_SHAPE_CASTLE	,
};

int	l_aExceptionField[]	=	{	153,0,	989,74	,	-1};	//	예외 필드 이 필드들은 여차저차 이유로 정상적인 방법으로 월드맵 필드를 찾을 수 없다

//
//	읽어들인 Road 정보를 바탕으로 노드를 설정한ㄷ.
void
cWorldMapFieldInfo::buildNode()
{
	int	i=0;

	for (i=0;i<m_iRoadInfoCount;i++)
	{
		cWM_roadInfo	*lpInfo	=	&m_aRoadList[i];

		if (lpInfo->m_wBeginIndex	>=	dMAX_WM_NODE_COUNT	||
			lpInfo->m_wEndIndex		>=	dMAX_WM_NODE_COUNT	||
			lpInfo->m_wBeginIndex	>=	m_iFieldInfoCount	||
			lpInfo->m_wEndIndex		>=	m_iFieldInfoCount)
			continue;

		m_aNodeList[lpInfo->m_wBeginIndex].addNode(lpInfo->m_wEndIndex);
		m_aNodeList[lpInfo->m_wEndIndex].addNode(lpInfo->m_wBeginIndex);
	}
}

//
//	_iPreField	:	시작필드
//	_iNextField	:	다음 필드
//	_iPreField~_iNextField 까지의 길 정보를 표시한다.
void
cWorldMapFieldInfo::drawRoad(int _iPreField,int _iNextField)
{
	int		i,j;

	for (i=0;i<m_iRoadInfoCount;i++)
	{
		cWM_roadInfo	*lpInfo	=	&m_aRoadList[i];

		if ((lpInfo->m_wBeginIndex	==	_iPreField	&&	lpInfo->m_wEndIndex	==	_iNextField)	||	
			(lpInfo->m_wEndIndex	==	_iPreField	&&	lpInfo->m_wBeginIndex==	_iNextField))
		{
			for (j=0;j<lpInfo->m_wRoadPointCount;j++)
				g_sprWorldMap.Put(lpInfo->m_aposRoadPoint[j].x,lpInfo->m_aposRoadPoint[j].y,eWM_ROAD_POINT);

			return;
		}
	}
}

//
//	_iBegin			:	시작 필드
//	_iEnd			:	도착 필드
//	_lpRoad			:	현재 추적중인 길의 정보
//	_lpResultRoad	:	찾고 있는 길중 최단거리 길의 정보
//	_iBegin~_iEnd까지의 최단 거리를 구한다. 재귀 호출을 사용해 모든 경우의 수를 다 검색한다.
void
cWorldMapFieldInfo::getShortestNode(int _iBegin,int _iEnd,cWM_road *_lpRoad,cWM_road *_lpResultRoad)
{
	int	i;

	if (!_lpRoad || !_lpResultRoad)
		return;
	if (_iBegin	<	0	||	_iBegin	>=	dMAX_WM_NODE_COUNT	||
		_iEnd	<	0	||	_iEnd	>=	dMAX_WM_NODE_COUNT)
		return;
	if (_iBegin	>=	m_iFieldInfoCount	||	_iEnd	>=	m_iFieldInfoCount)
		return;

	_lpRoad->addNode(_iBegin);

	for (i=0;i<m_aNodeList[_iBegin].m_wNodeCount;i++)
	{
		int	iNextNode	=	m_aNodeList[_iBegin].m_awLinkNode[i];

		if (_lpRoad->isExistNode(iNextNode))
			continue;

		if (iNextNode	==	_iEnd)
		{
			_lpRoad->addNode(iNextNode);

			if (getRoadLength(_lpRoad) < getRoadLength(_lpResultRoad))
				memcpy(_lpResultRoad,_lpRoad,sizeof(cWM_road));

			_lpRoad->m_wNodeCount--;

			continue;
		}

		getShortestNode(iNextNode,_iEnd,_lpRoad,_lpResultRoad);
	}

	_lpRoad->m_wNodeCount--;
}

//
//	_iEnd			:	도착 필드
//	현재 필드에서~_iEnd까지의 최단 거리를 구해서 m_resultPath에 저장한다.
BOOL
cWorldMapFieldInfo::makePathTo(int _iEnd)
{
	cWM_road	road;

	m_resultPath.m_wNodeCount	=	0;

	if (m_iCurrentFieldIndex	<	0	||	m_iCurrentFieldIndex	>=	m_iFieldInfoCount	||
		_iEnd				<	0	||	_iEnd				>=	m_iFieldInfoCount)
		return	FALSE;

	getShortestNode(m_iCurrentFieldIndex,_iEnd,&road,&m_resultPath);

	return	TRUE;
}

//
//	월드맵과 관련된 정보들을 읽어온다.
BOOL
cWorldMapFieldInfo::readData()
{
	CDecoder	decoder;
	BOOL		bIsEnd;
	int			i;
	int			aiList[100],iCount;

	m_iFieldInfoCount			=	0;
	m_iRoadInfoCount			=	0;
	m_iDungeonInfoCount			=	0;
	m_iSelectFieldIndex			=	-1;
	m_iFocusFieldIndex			=	-1;
	m_iSelectFieldSerial		=	-1;
	m_resultPath.m_wNodeCount	=	0;

	for (i=0;i<dMAX_WM_NODE_COUNT;i++)
		m_aNodeList[i].reset();

	if	(!decoder.Upload("data/interface/game/wm_field.csv",NULL))
		return	FALSE;

	while(1)
	{
		cWM_mainFieldInfo	info;

		bIsEnd				=	FALSE;

		info.m_wSerial		=	decoder.getCSVNumber(&bIsEnd);
		info.m_wFieldSerial	=	decoder.getCSVNumber(&bIsEnd);

		char	*lpstrHeader=	decoder.getCSVWord(&bIsEnd);

		if (lpstrHeader)
			strcpy(info.m_strFileHeader,lpstrHeader);
		
		iCount				=	decoder.getCSVNumberList(aiList,&bIsEnd);
		info.m_rectFrame.set(GetWorldMapX(aiList[0]),GetWorldMapY(aiList[1]),GetWorldMapX(aiList[2]-2),GetWorldMapY(aiList[3]-2));

		iCount				=	decoder.getCSVNumberList(aiList,&bIsEnd);

		info.m_wLinkDungeonCount	=	min(iCount,dMAX_LINK_DUNGEON_COUNT);

		for (i=0;i<info.m_wLinkDungeonCount;i++)
			info.m_awLinkDungeon[i]	=	aiList[i];

		info.m_wIsNew		=	decoder.getCSVNumber(&bIsEnd);
		iCount				=	decoder.getCSVNumberList(aiList,&bIsEnd);
		info.m_posFlagPos.Set(GetWorldMapX(aiList[0]),GetWorldMapY(aiList[1]));
		info.m_wVillageShape=	decoder.getCSVNumber(&bIsEnd);
		iCount				=	decoder.getCSVNumberList(aiList,&bIsEnd);
		info.m_wLowLevel	=	aiList[0];
		info.m_wTopLevel	=	aiList[1];

		if (bIsEnd	==	TRUE)
			break;

		if (m_iFieldInfoCount	<	dMAX_MAIN_FIELD_COUNT)
		{
			memcpy(&m_aFieldList[m_iFieldInfoCount],&info,sizeof(info));
			m_iFieldInfoCount++;
		}
	}

	if (!decoder.Upload("data/interface/game/wm_dungeon.csv",NULL))
		return	FALSE;

	while(1)
	{
		cWM_dungeonInfo		dungeonInfo;

		bIsEnd						=	FALSE;

		dungeonInfo.m_wSerial		=	decoder.getCSVNumber(&bIsEnd);
		iCount						=	decoder.getCSVNumberList(aiList,&bIsEnd);
		dungeonInfo.m_wDungeonCount	=	min(iCount,dMAX_WM_MAX_DUNGEON_COUNT);

		for (i=0;i<dungeonInfo.m_wDungeonCount;i++)
			dungeonInfo.m_awDungeonList[i]	=	aiList[i];

		iCount						=	decoder.getCSVNumberList(aiList,&bIsEnd);
		dungeonInfo.m_wLowLevel		=	aiList[0];
		dungeonInfo.m_wHighLevel	=	aiList[1];
		dungeonInfo.m_wIsNew		=	decoder.getCSVNumber(&bIsEnd);

		if (bIsEnd	==	TRUE)
			break;

		if (m_iDungeonInfoCount	<	dMAX_DUNGEON_INFO_COUNT)
		{
			memcpy(&m_aDungeonList[m_iDungeonInfoCount],&dungeonInfo,sizeof(dungeonInfo));
			m_iDungeonInfoCount++;
		}
	}

	if (!decoder.Upload("data/interface/game/wm_road.csv",NULL))
		return	FALSE;
	
	while(1)
	{
		cWM_roadInfo	roadInfo;

		bIsEnd						=	FALSE;

		roadInfo.m_wSerial			=	decoder.getCSVNumber(&bIsEnd);
		roadInfo.m_wBeginIndex		=	decoder.getCSVNumber(&bIsEnd);
		roadInfo.m_wEndIndex		=	decoder.getCSVNumber(&bIsEnd);
		roadInfo.m_wRoadLength		=	decoder.getCSVNumber(&bIsEnd);
		iCount						=	decoder.getCSVNumberList(aiList,&bIsEnd);

		int	iRoadPointCount	=	min(iCount/2,dMAX_WM_ROAD_POINT_COUNT);

		for (i=0;i<iRoadPointCount;i++)
		{
			roadInfo.m_aposRoadPoint[i].x	=	GetWorldMapX(aiList[i*2	]);
			roadInfo.m_aposRoadPoint[i].y	=	GetWorldMapY(aiList[i*2+1]);
		}

		roadInfo.m_wRoadPointCount	=	iRoadPointCount;

		if (bIsEnd	==	TRUE)
			break;

		if (m_iRoadInfoCount	<	dMAX_ROAD_INFO_COUNT)
		{
			memcpy(&m_aRoadList[m_iRoadInfoCount],&roadInfo,sizeof(roadInfo));
			m_iRoadInfoCount++;
		}
	}

	buildNode();

	return	TRUE;
}

//
//	각 필드의 영역 표시를 해준다.
void
cWorldMapFieldInfo::drawFieldFrame(int _iMode)
{
	int		i;
	cRECT	rect;

	DWORD	dwCurrentTime	=	timeGetTime()/5;
	int		iGradeValue		=	400;

	for (i=0;i<m_iFieldInfoCount;i++)
	{

		cWM_mainFieldInfo	*lpFieldInfo	=	&m_aFieldList[i];

		memcpy(&rect,&lpFieldInfo->m_rectFrame,sizeof(cRECT));
		
		int	iValue	=	dwCurrentTime%iGradeValue;

		if (iValue	>=	iGradeValue/2)
			iValue	=	iGradeValue-iValue;

		if (i	==	m_iCurrentFieldIndex || i	==	m_iSelectFieldIndex)
			cDRAW::BrightBox(200+iValue,&rect);
		else
			cDRAW::BrightBox(180,&rect);

		rect.x1++;
		rect.y1++;
		rect.x2--;
		rect.y2--;

		if (i	==	m_iCurrentFieldIndex || i	==	m_iSelectFieldIndex)
			cDRAW::FillBright(140+iValue,&rect);
		
		else
		if ((m_resultPath.isExistNode(i) || i	==	m_iFocusFieldIndex))
			cDRAW::FillBright(200,&rect);
		else
			cDRAW::FillBright(120,&rect);
		
	}

	for (i=0;i<m_resultPath.m_wNodeCount-1;i++)
	{
		drawRoad(m_resultPath.m_awNodeList[i],m_resultPath.m_awNodeList[i+1]);
	}

	for (i=0;i<m_iFieldInfoCount;i++)
	{
		cWM_mainFieldInfo	*lpFieldInfo	=	&m_aFieldList[i];

		if (lpFieldInfo->m_wVillageShape)
			g_sprWorldMap.Put(lpFieldInfo->m_posFlagPos.x,lpFieldInfo->m_posFlagPos.y,eWM_FLAG_SHAPE_CAMP-1+lpFieldInfo->m_wVillageShape);

		if (i	==	m_iSelectFieldIndex)
			g_sprWorldMap.Put(lpFieldInfo->m_posFlagPos.x,lpFieldInfo->m_posFlagPos.y,52);
	}

	for( i=0 ; i<g_gwWorldMap.m_vtiQFIndexList.size() ; i++)
	{

		if (g_gwWorldMap.m_vtiQFIndexList[i] == -1)
			continue;
		if (g_gwWorldMap.m_vtiQFIndexList[i] < 0 || g_gwWorldMap.m_vtiQFIndexList[i] >= m_iFieldInfoCount)
			continue;

		cWM_mainFieldInfo	*lpFieldInfo	=	&m_aFieldList[g_gwWorldMap.m_vtiQFIndexList[i]];
		memcpy(&rect,&lpFieldInfo->m_rectFrame,sizeof(cRECT));

		g_sprWorldMap.Put(rect.x1, rect.y1, 37, 50, 50);
		int iFPS		=	g_anmEtc.GetFPS(eEANM_MINIMAP_ARROW_DOWN);
		int iFrameCount	=	g_anmEtc.GetFrameCount(eEANM_MINIMAP_ARROW_DOWN);
		int iFrame		=	(CGameInfo::s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;
		int iY = iFrame/2;
		
		g_anmEtc.PutReg(rect.x1-g_anmEtc.getSpriteWidth(eEANM_MINIMAP_ARROW_DOWN, 0, 0)/2-1,
			rect.y1-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_DOWN, 0, 0)-iY,
			eEANM_MINIMAP_ARROW_DOWN,0,iFrame);	
	}
}

//
//	_iPreField	:	시작 필드
//	_iNextField	:	다음 필드
//	_iPreField와 _iNextField 까지의 거리를 구한다.
int
cWorldMapFieldInfo::getRoadLength(int _iPreField,int _iNextField)
{
	int		i;

	for (i=0;i<m_iRoadInfoCount;i++)
	{
		cWM_roadInfo	*lpInfo	=	&m_aRoadList[i];

		if ((lpInfo->m_wBeginIndex	==	_iPreField	&&	lpInfo->m_wEndIndex	==	_iNextField)	||
			(lpInfo->m_wEndIndex	==	_iPreField	&&	lpInfo->m_wBeginIndex==	_iNextField))
			return	lpInfo->m_wRoadLength;
	}

	return	0;
}

//
//	_lpRoad	:	길
//	길(노드 리스트)의 길이를 구한다.
int
cWorldMapFieldInfo::getRoadLength(cWM_road *_lpRoad)
{
	int	iRoadLength	=	0;

	if (_lpRoad->m_wNodeCount	==	0)
		return	0x7fffffff;

	for (int i=0;i<_lpRoad->m_wNodeCount-1;i++)
		iRoadLength	+=	getRoadLength(_lpRoad->m_awNodeList[i],_lpRoad->m_awNodeList[i+1]);

	return	iRoadLength;
}

//
//	_iField	:	필드의 고유 시리얼
//	해당 필드의 표면 필드를 구한다.
int
cWorldMapFieldInfo::getSurfaceField(int _iField)
{
	int	i;

	if (_iField	<	0	||	_iField	>=	dMAX_MAP_COUNT)
		return	-1;

	for (i=0;i<m_iFieldInfoCount;i++)
		if (m_aFieldList[i].m_wFieldSerial	==	_iField	||	isLinkedDungeon(i,_iField))
			return	i;

	char	strFileName[64];

	if (strlen(g_aMapSimpleInfo[_iField].m_strFileName)	<	8)
		return	-1;

	strncpy(strFileName,g_aMapSimpleInfo[_iField].m_strFileName+5,3);

	strFileName[3]		=	NULL;

	for (i=0;i<m_iFieldInfoCount;i++)
		if (STRICMP(m_aFieldList[i].m_strFileHeader,strFileName)==0)
			return	i;

	for (i=0;;i+=2)
	{
		if	(l_aExceptionField[i]	==	-1)
			break;

		if	(l_aExceptionField[i]	==	_iField)
			return	l_aExceptionField[i+1];
	}

	return	-1;
}

//
//	_iCurrentField	:	필드의 고유 시리얼
//	현재 필드를 설정한다.
void
cWorldMapFieldInfo::setCurrentField(int _iCurrentField)
{
	m_iCurrentField		=	_iCurrentField;
	m_iCurrentFieldIndex=	getSurfaceField(_iCurrentField);

	if (m_iCurrentFieldIndex	==	-1)
	{
		char	strFileName[64];

		strncpy(strFileName,g_map.m_strFileName+5,3);

		strFileName[3]		=	NULL;

		for (int i=0;i<m_iFieldInfoCount;i++)
			if (STRICMP(m_aFieldList[i].m_strFileHeader,strFileName)==0)
				m_iCurrentFieldIndex	=	i;
	}

	if (m_iCurrentFieldIndex	==	-1)
	{
		if	(m_iFieldInfoCount	<=	0)
			return;

		m_iCurrentFieldIndex	=	0;
	}

	m_iSelectFieldIndex	=	m_iCurrentFieldIndex;
	m_iSurfaceField		=	m_aFieldList[m_iCurrentFieldIndex].m_wFieldSerial;
}

//
//	_iFieldIndex	:	m_aFieldList 의 인덱스
//	_iDungeonSerial :	던젼의 고유 시리얼
//	던젼이 해당 필드에 링크 되어 있는지 판단한다.
BOOL
cWorldMapFieldInfo::isLinkedDungeon(int _iFieldIndex,int _iDungeonSerial)
{
	if (_iFieldIndex	<	0	||	_iFieldIndex	>=	m_iFieldInfoCount	||
		_iFieldIndex	>=	dMAX_MAIN_FIELD_COUNT)
		return	FALSE;

	cWM_mainFieldInfo	*lpInfo	=	&m_aFieldList[_iFieldIndex];

	int	iLinkDungeonCount	=	min(lpInfo->m_wLinkDungeonCount,dMAX_LINK_DUNGEON_COUNT);

	for (int i=0;i<iLinkDungeonCount;i++)
	{
		int	iDungeonIndex			=	lpInfo->m_awLinkDungeon[i];

		if (iDungeonIndex	<	0	||	iDungeonIndex	>=	m_iDungeonInfoCount	||
			iDungeonIndex	>=	dMAX_DUNGEON_INFO_COUNT)
			continue;

		cWM_dungeonInfo	*lpDungeon	=	&m_aDungeonList[iDungeonIndex];

		int	iDungeonCount	=	min(lpDungeon->m_wDungeonCount,dMAX_WM_MAX_DUNGEON_COUNT);

		for  (int j=0;j<iDungeonCount;j++)
			if (lpDungeon->m_awDungeonList[j]	==	_iDungeonSerial)
				return	TRUE;
	}

	return	FALSE;
}

//
//	포커스가 간 필드
int
cWorldMapFieldInfo::getFocusedField(int _iX,int _iY)
{
	int	i;

	for (i=0;i<m_iFieldInfoCount;i++)
		if (m_aFieldList[i].m_rectFrame.isIn(_iX,_iY))
			return	m_aFieldList[i].m_wFieldSerial;

	return	-1;
}

void
cWorldMapFieldInfo::update(int _iX,int _iY,BOOL _bIsTouchLeftButton,BOOL _bIsForGateGlove)
{
	m_iFocusFieldIndex	=	getFocusedField(_iX,_iY);

	if	(m_iFocusFieldIndex	!=	-1)
		m_iFocusFieldIndex	=	getSurfaceField(m_iFocusFieldIndex);

	if (_bIsTouchLeftButton)
	{
		m_iSelectFieldIndex	=	m_iFocusFieldIndex;

		if	(m_iSelectFieldIndex	!=	-1)
		{
			m_iSelectFieldSerial	=	m_aFieldList[m_iSelectFieldIndex].m_wFieldSerial;
			makePathTo(m_iSelectFieldIndex);
		}
		else
		if	(_bIsForGateGlove	==	FALSE)
		{
			m_resultPath.m_wNodeCount	=	0;
			m_iSelectFieldIndex			=	m_iCurrentFieldIndex;
		}

	}
}
