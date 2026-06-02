#include "cMAP.H"
#include "cDRAW.H"
#include "cMAPWND.H"
#include "cTEXT.H"
#include "cBUILDING.H"
#include "cMESSAGE.H"
#include "cPROJECT.H"
#include "cTOOLWND.H"
#include "CShop.H"
#include "doorDefine.H"
#include "secretDungeon.h"
#include "svn.h"
#include "tchar.h"
#include "svn.h"
#include "cMAIN.h"
#include "field_common.h"
#include "mess_sign.h"
#include "localize.h"

WORD		*l_pInfo	=	NULL;
int			g_iSelectMap	=	0;
cMAP		g_aMap[dMAX_MAP];
cMAP		*g_lpActmap		=	NULL;		//	현재 활성화된 맵
cBARMENU	g_bmMapList;
WORD		**cMAP::s_pTile;
int			l_iScale	=	-10;

CBlockedTileInfoBundle	g_aBlockedInfo[eTS_COUNT];

int		l_iVersion		=	dCURRENT_MAP_VERSION;

static void
CopyStringField(char *dest,int destSize,const char *src)
{
	if (!src || !src[0]) return;

	strncpy(dest,src,destSize-1);
	dest[destSize-1]	=	NULL;
	RemoveQuotationMark(dest);
}

static BOOL
MakeOfficialMapPath(const char *srcPath,char *destPath,int destSize)
{
	char	*pSuffix	=	strstr((char *)srcPath,"Scenario\\Red Stone\\Map\\");

	if (!pSuffix) return FALSE;

	_snprintf(destPath,destSize-1,"C:\\Program Files (x86)\\GameON\\RED STONE\\Data\\%s",pSuffix);
	destPath[destSize-1]	=	NULL;

	return	(GetFileAttributes(destPath) != INVALID_FILE_ATTRIBUTES);
}

static void
ApplyOfficialJapaneseNames(char *srcPath,cMAP *targetMap)
{
	char	officialPath[1024];

	if (!MakeOfficialMapPath(srcPath,officialPath,sizeof(officialPath))) return;

	int		versionBackup	=	l_iVersion;
	cMAP	*pOfficialMap	=	new cMAP;

	if (pOfficialMap && cMAP::Load(officialPath,0xffff,pOfficialMap))
	{
		CopyStringField(targetMap->m_strName,sizeof(targetMap->m_strName),pOfficialMap->m_strName);

		for (int i=0;i<dMAX_ACTOR_COUNT;i++)
		{
			if (!targetMap->m_setActor.isAvail(i)) continue;
			if (!pOfficialMap->m_setActor.isAvail(i)) continue;

			CopyStringField(targetMap->m_setActor.m_aActor[i].m_strName,
							sizeof(targetMap->m_setActor.m_aActor[i].m_strName),
							pOfficialMap->m_setActor.m_aActor[i].m_strName);
		}

		for (int i=0;i<dMAX_AREA;i++)
		{
			if (!targetMap->m_pArea->IsAvail(i)) continue;
			if (!pOfficialMap->m_pArea->IsAvail(i)) continue;

			CopyStringField(targetMap->m_pArea->m_aArea[i].m_strName,
							sizeof(targetMap->m_pArea->m_aArea[i].m_strName),
							pOfficialMap->m_pArea->m_aArea[i].m_strName);

			CopyStringField(targetMap->m_pArea->m_aArea[i].m_strMoveGateName,
							sizeof(targetMap->m_pArea->m_aArea[i].m_strMoveGateName),
							pOfficialMap->m_pArea->m_aArea[i].m_strMoveGateName);
		}
	}

	pKILL(pOfficialMap);

	l_iVersion	=	versionBackup;
}

cMAP::cMAP()
{
	m_pObjectMap	=	NULL;		//	오브젝트 혹은 건물
	m_pBuildingMap	=	NULL;
	m_pCrashMap		=	NULL;

	m_pArea			=	NULL;
	m_pPattern		=	NULL;				//	타일 패턴 컨트롤
	m_iTileSet		=	0;

	Reset();
}


cMAP::~cMAP()	
{
	Reset();

	if (s_pTile)
		for (int i=0;i<10000;i++)
			pKILL(s_pTile[i]);

	pKILL(s_pTile);
}


void
cMAP::Reset()
{
	_free(m_pObjectMap);
	_free(m_pBuildingMap);
	_free(m_pCrashMap);

	KILL(m_pPattern);
	KILL(m_pArea);

	m_setCharacter.reset();
	m_setActor.reset();

	m_iSerial					=	0xffff;
	memset(m_aBgmList,0xff,sizeof(m_aBgmList));

	m_iSelectTile				=	0;
	m_iSelectBuilding			=	0;
	m_iSlctArea					=	0;
	m_iSelectTeam				=	0;		//	선택한 팀
	m_iTileSet					=	0;

	m_bf1IsHiddenPortal			=	FALSE;	//	포탈 모양만 숨긴다.
	m_bf4MinimapDisplayLevel	=	0;	//	미니맵 표시 레벨

	m_pos.x						=	0;
	m_pos.y						=	0;
	m_lpEditAddonObject			=	NULL;
	m_lpEditAddonObjectBuilding	=	NULL;

	m_sCorrectFireResistance	=	0;
	m_sCorrectWaterResistance	=	0;
	m_sCorrectWindResistance	=	0;
	m_sCorrectEarthResistance	=	0;
	m_sCorrectLightResistance	=	0;
	m_sCorrectDarkResistance	=	0;
	m_bIsLockedBySVN			=	FALSE;

	setPixelPos();

	m_isTileExpand		=	TRUE;
	SetVirgin();

	strcpy(m_strFName,"");

	{
		for (int i=0;i<50;i++)
			sprintf(m_astrFieldValue[i],"field value #%.2d",i);
	}


	m_building.close();
	m_object.close();
	m_shop.reset();
	m_customItem.init();
}

//
//	블랭크 타일,오버랩 타일 정보를 그려준다.
void
cMAP::RemarkBlankAndOverlapInfo(WORD *_lpMapInfo)
{
	m_lpTileMap	=	_lpMapInfo;

	remarkAllBuildingInfo(FALSE,TRUE,TRUE);
	remarkAllObjectInfo(FALSE,TRUE,TRUE);
}	//	cMAP::RemarkBlankAndOverlapInfo(WORD *_lpMapInfo)

//
//	블랭크 타일,오버랩 타일 정보를 그려준다.
void
cMAP::RemarkBrigeObject(WORD *_lpMapInfo)
{
	m_lpTileMap	=	_lpMapInfo;

	remarkAllBridgeInfo();
}	//	cMAP::RemarkBlankAndOverlapInfo(WORD *_lpMapInfo)

//
//	크래쉬 맵 정보를 그려준다.
void
cMAP::RemarkCrashMap()
{
	MSGOUT("Reset all building collision maps");
	remarkAllBuildingInfo(TRUE,FALSE,FALSE);
	MSGOUT("Reset all object collision maps");
	remarkAllObjectInfo(TRUE,FALSE,FALSE);
	MSGOUT("End of collision map reset");
}	//	cMAP::RemarkCrashMap()

//
//	맵 둘레에 충돌마크(?)를 그려준다.
void
cMAP::RoundCrash()
{
	memset(m_pCrashMap  											,1,m_iRealWidth*dBLOCKED_SIZE);
	memset(m_pCrashMap+m_iRealWidth * (m_iRealHeight-dBLOCKED_SIZE)	,1,m_iRealWidth*dBLOCKED_SIZE);

	for (int y=0;y<m_iRealHeight;y++)
	{
		for (int i=0;i<dBLOCKED_SIZE;i++)
		{
			m_pCrashMap[y*m_iRealWidth+i		]	=	1;
			m_pCrashMap[(y+1)*m_iRealWidth -1-i	]	=	1;
		}
	}
}	//	cMAP::RoundCrash()

//
//	맵 데이터를 리셋하고 초기화 한다.
void
cMAP::Create()
{	
	m_iRealWidth	=	m_iWidth	*dSHARE_TILE;
	m_iRealHeight	=	m_iHeight	*dSHARE_TILE;

	m_pObjectMap	=	_new(WORD,m_iRealWidth*m_iRealHeight,_ms("%s - cMAP::m_pObjectMap"		,m_strName));
	m_pBuildingMap	=	_new(BYTE,m_iRealWidth*m_iRealHeight,_ms("%s - cMAP::m_pBuildingMap"	,m_strName));
	m_pCrashMap		=	_new(BYTE,m_iRealWidth*m_iRealHeight,_ms("%s - cMAP::m_pCrashMap"		,m_strName));

	memset(m_pObjectMap		,0xff	,m_iRealWidth*m_iRealHeight*2);
	memset(m_pBuildingMap	,0xff	,m_iRealWidth*m_iRealHeight);
	memset(m_pCrashMap		,0		,m_iRealWidth*m_iRealHeight);

	m_pArea			=	new cAREA();	//	영역
	m_pArea->Reset();

	m_pPattern		=	new CPattnCtrl();	//	타일 패턴 컨트롤러 생성
}	//	cMAP::Create()

//
//	xs,ys 사이즈로 맵을 생성
void
cMAP::Init(int xs,int ys)
{
	m_iWidth		=	xs;
	m_iHeight		=	ys;
	m_iRealWidth	=	xs*dSHARE_TILE;
	m_iRealHeight	=	ys*dSHARE_TILE;

	Reset();
	Create();		//	데이터 초기화

	memset(m_pObjectMap		,0xffff		,m_iRealWidth*m_iRealHeight*2);	//	오브젝트 맵
	memset(m_pBuildingMap	,(BYTE)0xff	,m_iRealWidth*m_iRealHeight);	//	건물 맵
	memset(m_pCrashMap		,(BYTE)0	,m_iRealWidth*m_iRealHeight);	//	충돌 맵
	RoundCrash();													//	맵의 둘레에 이동불가 체크

/*
	m_pPattern->PutPattnIndex(100, 100);


	m_pPattern->PutCliff(int x, int y);								//	절벽을 새로 만든다.
	m_pPattern->PutTile(int x, int y, BYTE flag, BOOL IsExpand);	//	IsExpand는 절벽이 있을 경우 처리에 관한것이다. true라면 확장을 하고 false라면 깍아내린다.
	//	flag 타일 종류
	//	IsExpand 1 - 확장  0 깍기

	void		GetNormalFlag(BYTE *lpbFlag, int *iFlagCount);
	//	lpbFlag	패턴 번호
	//	iFlagCount 패턴의 수


	WORD	*m_pPatternIndex,*m_pHeightPattern;

	LPPattn		GetFlagFromPattn(BYTE flag);	//	GetNormalFlag(BYTE *lpbFlag, int *iFlagCount); 이걸로 얻어온거



//	m_pPatternIndex	=	new WORD [width*height];
//	m_pHeightPattern=	new WORD [width*height];

//저장할때
	GetPattnIndex(m_pWidthPattern,m_pHeightPattern);	//	맵 타일정보와 맵 타일 높이 정보를 얻어 온다.
//	m_pWidthPattern,m_pHeightPattern
//	void		PutPattnIndex(int XRange, int YRange, LPWORD lpwPattnIndex=NULL, LPWORD lpwHeight=NULL);
*/
	m_pos.x		=	0;
	m_pos.y		=	0;

	setPixelPos();
}	//	cMAP::Init(int xs,int ys)


//
//	xs,ys 사이즈로 tile세트로 생성
BOOL
cMAP::Init(int xs,int ys,int tile,int serial)
{
	Init(xs,ys);		//	데이터 초기화

	m_iTileSet		=	tile;

	if (!loadMapsetImage()		)	return FALSE;

//	for (int j=0;j<m_iHeight;j++)		//	랜덤하게 채워줌
//		for (int i=0;i<m_iWidth;i++)
//			SetTile(i,j,0);

	m_iSerial		=	serial;

	return TRUE;
}	//	cMAP::Init(int xs,int ys,int tile,int serial)

//
//	타일셋에 맞춰 오브젝트 들을 불러들인다.
BOOL
cMAP::loadMapsetImage()
{
	if (!m_pPattern->Load(_ms("%s\\data\\mapset\\%s\\tile",_logFolder,g_strTileSet[m_iTileSet*2])))
		return	ERRMSG("Error in Load Tile","can not find follow file\n\n");

	m_pPattern->Init(m_iWidth, m_iHeight);
	m_pPattern->GetNormalFlag(m_aTilePattern,&m_iPatternCount);

	if (!m_object.init(m_iTileSet,NULL))	return	FALSE;
	if (!m_building.init(m_iTileSet,NULL))	return	FALSE;

	return	TRUE;
}	//	cMAP::loadMapsetImage()

//
//	맵 초기화
BOOL
cMAP::Init(cMAP_INFO *mapInfo,int serial)
{
	cMAP_INFO temp;

	memcpy(&temp,this,sizeof(cMAP_INFO));
	memcpy(this,mapInfo,sizeof(cMAP_INFO));

	if (Init(mapInfo->m_iWidth,mapInfo->m_iHeight,mapInfo->m_iTileSet,serial))	return TRUE;
	else
	{
		memcpy(this,&temp,sizeof(cMAP_INFO));
		return	FALSE;
	}
}	//	cMAP::Init(cMAP_INFO *mapInfo,int serial)

//
//
BOOL
cMAP::Resize(char* _strName,int _iX, int _iY, int _iWidth,int _iHeight,int _iMonsterCount)
{
	if (m_iWidth > _iWidth || m_iHeight > _iHeight)
	{
		switch(cMSG::YESNO(dMSG_RESET_MAP,dMSG_WARNING_LOST_DATA))
		{	case	IDYES		:
				break;

			case	IDNO		:
				return FALSE;

		}
	}

	int	iRealWidth			=	_iWidth * dSHARE_TILE;		// 타일 넓이
	int	iRealHeight			=	_iHeight* dSHARE_TILE;		// 타일 넓이

	strcpy(m_strName,_strName);

	WORD	*pObjMap;		//	오브젝트
	BYTE	*pBuildMap;		//	건물
	BYTE	*pCrashMap;		//	블럭정보
	cRECT	rectSize( _iX, _iY, _iWidth + _iX, _iHeight + _iY);

	pObjMap		=	_new(WORD,iRealWidth*iRealHeight,_ms("%s - cMAP::m_pObjectMap",m_strName));
	pBuildMap	=	_new(BYTE,iRealWidth*iRealHeight,_ms("%s - cMAP::m_pBuildingMap",m_strName));
	pCrashMap	=	_new(BYTE,iRealWidth*iRealHeight,_ms("%s - cMAP::m_pCrashMap",m_strName));



	if(rectSize.x1 > m_iRealWidth || rectSize.y1 > m_iRealHeight)
	{	
		memset(pObjMap	,(BYTE)0xff	,iRealWidth*m_iRealHeight*2);
		memset(pBuildMap,(BYTE)0xff	,iRealWidth*m_iRealHeight);
		memset(pCrashMap,(BYTE)0	,iRealWidth*m_iRealHeight);
	}
	else
	{
		
		for(int y=rectSize.y1;y<rectSize.y2;++y)
		{
			int	iSrcAddr	=	y * m_iRealWidth + rectSize.x1;
			int	iDestAddr	=	(y-rectSize.y1) * iRealWidth;
			
			int iWidth		=	min(rectSize.x2, m_iRealWidth) - rectSize.x1;
			
			if (y < m_iRealHeight)
			{			
				memcpy(pObjMap	+iDestAddr,m_pObjectMap	+iSrcAddr,iWidth*2);
				memcpy(pBuildMap+iDestAddr,m_pBuildingMap+iSrcAddr,iWidth  );
				memcpy(pCrashMap+iDestAddr,m_pCrashMap	+iSrcAddr,iWidth  );
				
				if(rectSize.x2 > m_iRealWidth)
				{
					iWidth	=	rectSize.x2 - m_iRealWidth;
					iDestAddr	+=	m_iRealWidth - rectSize.x1;
					memset(pObjMap	+iDestAddr,(BYTE)0xff	,iWidth*2);
					memset(pBuildMap+iDestAddr,(BYTE)0xff	,iWidth);
					memset(pCrashMap+iDestAddr,(BYTE)0		,iWidth);
				}
			}
			else
			{
				memset(pObjMap	+iDestAddr,(BYTE)0xff	,iRealWidth*2);
				memset(pBuildMap+iDestAddr,(BYTE)0xff	,iRealWidth);
				memset(pCrashMap+iDestAddr,(BYTE)0		,iRealWidth);				
			}
		}
	}

	_free(m_pObjectMap);
	_free(m_pBuildingMap);
	_free(m_pCrashMap);

	m_pObjectMap	=	pObjMap;
	m_pBuildingMap	=	pBuildMap;
	m_pCrashMap		=	pCrashMap;

	m_iWidth		=	_iWidth;
	m_iHeight		=	_iHeight;

	m_iRealWidth	=	iRealWidth;
	m_iRealHeight	=	iRealHeight;

	m_pPattern->Resize( _iX, _iY, m_iWidth,m_iHeight);

	Select(g_iSelectMap);

	RoundCrash();

	pKILL(g_MAPWND.m_pSelectData);	//	 선택한 데이터가 있으면 해제 시킨다..

	for(int i=0; i<dMAX_ACTOR_COUNT; ++i)
	{
		
		cACTOR* pAc = m_setActor.getActor(i);

		if(!pAc)
			continue;
		
		int iX	=	_iX*dTILE_WIDTH/dSHARE_TILE;
		int iY	=	_iY*dTILE_HEIGHT/dSHARE_TILE;

		pAc->m_pos.x	-=	iX;
		pAc->m_pos.y	-=	iY;

		int	iMX			=	pAc->m_pos.x/(dTILE_WIDTH/dSHARE_TILE);
		int	iMY			=	pAc->m_pos.y/(dTILE_HEIGHT/dSHARE_TILE);

		if(iMX < 0 ||iMY< 0 || iMX > m_iWidth ||  iMY > m_iHeight)
			m_setActor.remove(i);
		
	}

	for (int iObject = 0;iObject<dMAX_OBJECT;++iObject)
	{

		cTINY_OBJECT_INFO	*lpTinyObject	=	m_object.getTinyObject(iObject);
		if(lpTinyObject)
		{
			int iX	=	_iX*dTILE_WIDTH/dSHARE_TILE;
			int iY	=	_iY*dTILE_HEIGHT/dSHARE_TILE;
			
			lpTinyObject->m_pos.x	-=	iX;
			lpTinyObject->m_pos.y	-=	iY;
			
			int	iMX			=	lpTinyObject->m_pos.x/(dTILE_WIDTH/dSHARE_TILE);
			int	iMY			=	lpTinyObject->m_pos.y/(dTILE_HEIGHT/dSHARE_TILE);
			
			if(iMX < 0 ||iMY< 0 || iMX > m_iWidth ||  iMY > m_iHeight)
				m_object.removeTinyObject(iObject);
			
		}

		cFIXED_OBJECT_INFO	*lpFixedObject	=	m_object.getFixedObject(iObject);
		if(lpFixedObject)
		{

			int iX	=	_iX*dTILE_WIDTH/dSHARE_TILE;
			int iY	=	_iY*dTILE_HEIGHT/dSHARE_TILE;
			
			lpFixedObject->m_pos.x	-=	iX;
			lpFixedObject->m_pos.y	-=	iY;
			
			int	iMX			=	lpFixedObject->m_pos.x/(dTILE_WIDTH/dSHARE_TILE);
			int	iMY			=	lpFixedObject->m_pos.y/(dTILE_HEIGHT/dSHARE_TILE);
			if(iMX < 0 ||iMY< 0 || iMX > m_iWidth ||  iMY > m_iHeight)
				m_object.removeFixedObject(iObject);
		}

	}

	for(int iArea = 0;iArea < dMAX_AREA; ++iArea)
	{
		if(!m_pArea->IsAvail(iArea))
			continue;

		int iX	=	_iX*dTILE_WIDTH/dSHARE_TILE;
		int iY	=	_iY*dTILE_HEIGHT/dSHARE_TILE;
		
		m_pArea->m_aArea[iArea].move(-iX,-iY);

		if(m_pArea->m_aArea[iArea].x2 < 0|| m_pArea->m_aArea[iArea].y2 < 0 ||m_pArea->m_aArea[iArea].x1 > m_iWidth*dTILE_WIDTH/dSHARE_TILE || m_pArea->m_aArea[iArea].y1 > m_iHeight*dTILE_HEIGHT/dSHARE_TILE)
			m_pArea->Del(iArea);
		
	}
	int iBuildingCount = m_building.m_den.getCount();
	for(int iBuilding = 0;iBuilding<iBuildingCount;++iBuilding)
	{
		cBUILDING_INFO*	lpBuilding			= m_building.m_den.get(iBuilding);

		if(!lpBuilding)
			continue;

		int iX	=	_iX*dTILE_WIDTH/dSHARE_TILE;
		int iY	=	_iY*dTILE_HEIGHT/dSHARE_TILE;

		lpBuilding->m_pos.x -= iX;
		lpBuilding->m_pos.y -= iY;

		int	iMX			=	lpBuilding->m_pos.x/(dTILE_WIDTH/dSHARE_TILE);
		int	iMY			=	lpBuilding->m_pos.y/(dTILE_HEIGHT/dSHARE_TILE);
		if(iMX < 0 ||iMY< 0 || iMX > m_iWidth ||  iMY > m_iHeight)
			m_building.m_den.remove(iBuilding);
	}
	
	//	오브젝트 영역 등을 체크해서 맵 밖으로나간건 날려줘야 한다.

	LostVirgin();

	return TRUE;
}	//	cMAP::Resize(char *strName,int iWidth,int iHeight,int	iFieldType)

void
cMAP::FreeSelect()
{
}

//
//	스케일드 타일 빌드
void
cMAP::buildScaledTile(int _iScale)
{
	if (_iScale	==	100)	return;
	
	int	i;
	int	iWidth	=	dTILE_WIDTH*_iScale/100;
	int	iHeight	=	dTILE_HEIGHT*_iScale/100;

	if (iWidth*100/_iScale	!=	dTILE_WIDTH)	iWidth++;
	if (iHeight*100/_iScale	!=	dTILE_HEIGHT)	iHeight++;

	if (s_pTile)
		for (i=0;i<10000;i++)
			pKILL(s_pTile[i]);

	pKILL(s_pTile);

	s_pTile	=	new WORD* [10000];

	WORD	*lpScreen	=	cDRAW::p2SCREEN;
	int		iScreenWidth=	cDRAW::WIDTH;
	int		iScreenHeight=	cDRAW::HEIGHT;
	for (i=0;i<10000;i++)
	{
		LPPattn	lpTilePattern	=	m_pPattern->GetPattn(i);

		s_pTile[i]				=	new WORD [iWidth*iHeight];

		cDRAW::Active(s_pTile[i],iWidth,iHeight);
		cDRAW::PutImageScale(0,0,dTILE_WIDTH,dTILE_HEIGHT,(WORD *)(lpTilePattern->lpImage),_iScale,_iScale);
	}

	cDRAW::Active(lpScreen,iScreenWidth,iScreenHeight);
}	//	cMAP::buildScaledTile(int _iScale)

//
//	현재 맵을 그려준다.
void
cMAP::draw(int _iScale)
{
	int		x,y;

	if (!m_pPattern)
		return;

//	int		iIndex1			=	m_pPattern->GetMapIndex(97,150);	
//	int		iIndex2			=	m_pPattern->GetMapIndex(73,126);	

//	m_pPattern->SetMapIndex(97,143,iIndex1);
//	m_pPattern->SetMapIndex(76,108,iIndex2);

	if (_iScale	== 100)
	{
		for (y	=	m_pos.y/dSHARE_TILE;y<g_MAPWND.TPH/dSHARE_TILE+1+m_pos.y;y++)
		{
//			if (y >= m_iRealHeight && y > g_MAPWND.TPH)	break;

			for (x=m_pos.x/dSHARE_TILE;x<g_MAPWND.TPW/dSHARE_TILE+1+m_pos.x;x++)
			{
				int		xp=(x-m_pos.x/dSHARE_TILE)*dTILE_WIDTH;
				int		yp=g_iTBSize+(y-m_pos.y/dSHARE_TILE)*dTILE_HEIGHT;

				int		iIndex			=	m_pPattern->GetMapIndex(x,y);	
				LPPattn	lpTilePattern	=	m_pPattern->GetPattn(iIndex);

				if (x < m_iRealWidth && y < m_iRealHeight)
					cDRAW::PutImage(xp,yp,dTILE_WIDTH,dTILE_HEIGHT,(WORD *)(lpTilePattern->lpImage));
				else
					cDRAW::Fill(0,xp,yp,xp+dTILE_WIDTH,yp+dTILE_HEIGHT);
			}
		}
		return;
	}

	int	iWidth	=	dTILE_WIDTH*_iScale/100;
	int	iHeight	=	dTILE_HEIGHT*_iScale/100;

	if (iWidth*100/_iScale	!=	dTILE_WIDTH)	iWidth++;
	if (iHeight*100/_iScale	!=	dTILE_HEIGHT)	iHeight++;

	if (!s_pTile || l_iScale!=_iScale)
	{
		l_iScale	=	_iScale;
		buildScaledTile(_iScale);
	}

	for (y	=	m_pos.y/dSHARE_TILE;y<g_MAPWND.TPH/dSHARE_TILE+2+m_pos.y;y++)
	{
		for (x=m_pos.x/dSHARE_TILE;x<g_MAPWND.TPW/dSHARE_TILE+2+m_pos.x;x++)
		{
			int		xp=(x-m_pos.x/dSHARE_TILE)*dTILE_WIDTH*_iScale/100;
			int		yp=g_iTBSize+(y-m_pos.y/dSHARE_TILE)*dTILE_HEIGHT*_iScale/100;

			int		iIndex			=	m_pPattern->GetMapIndex(x,y);	

			if (x < m_iRealWidth && y < m_iRealHeight)
			{
				cDRAW::PutImage(xp,yp,iWidth,iHeight,s_pTile[iIndex]);
//				cDRAW::PutImageScale(xp,yp,dTILE_WIDTH,dTILE_HEIGHT,(WORD *)(lpTilePattern->lpImage),_iScale,_iScale);
			}
			else
				cDRAW::Fill(0,xp,yp,xp+dTILE_WIDTH*_iScale/100+1,yp+dTILE_HEIGHT*_iScale/100+1);
		}
	}
}	//	cMAP::draw()

//
//	현재 맵을 그려준다.
void
cMAP::drawWholeImage(int _iScale)
{
	int	iWidth	=	dTILE_WIDTH*_iScale/100;
	int	iHeight	=	dTILE_HEIGHT*_iScale/100;

	if (iWidth*100/_iScale	!=	dTILE_WIDTH)	iWidth++;
	if (iHeight*100/_iScale	!=	dTILE_HEIGHT)	iHeight++;

	l_iScale	=	-10;

	buildScaledTile(_iScale);

	int		x,y;

	if (!m_pPattern)	return;

	for (y=0;y<m_iHeight;y++)
	{	
		for (x=0;x<m_iWidth;x++)
		{
			int		xp=x*dTILE_WIDTH*_iScale/100;
			int		yp=y*dTILE_HEIGHT*_iScale/100;

			int		iIndex			=	m_pPattern->GetMapIndex(x,y);	

//			if (_iScale*dTILE_WIDTH/100 < iXS)	_iScale++;
			cDRAW::PutImage(xp,yp,iWidth,iHeight,s_pTile[iIndex]);
//			cDRAW::PutImageScale(xp,yp,dTILE_WIDTH,dTILE_HEIGHT,(WORD *)(lpTilePattern->lpImage),_iScale,_iScale);
		}
	}
}	//	cMAP::draw()


WORD
cMAP::GetTile(int x,int y)
{
	return	0;
}

//
//	타일을 찍어준다.
void
cMAP::PutTile(int x,int y,WORD tile,BOOL _isExpand)
{
	if (x < 0 || y <0 || x >= m_iRealWidth || y >= m_iRealHeight)	return;

	m_pPattern->PutTile(x/dSHARE_TILE,y/dSHARE_TILE,(BYTE)m_aTilePattern[tile],_isExpand);	

	return;
}	//	cMAP::PutTile(int x,int y,WORD tile,BOOL _isExpand)

//
//	절벽을 세운다.
void
cMAP::BuildCliff(int x,int y)
{
	if (x < 0 || y <0 || x >= m_iRealWidth || y >= m_iRealHeight)	return;

	m_pPattern->PutCliff(x/dSHARE_TILE,y/dSHARE_TILE);	//	IsExpand는 절벽이 있을 경우 처리에 관한것이다. true라면 확장을 하고 false라면 깍아내린다.

	return;
}	//	cMAP::BuildCliff(int x,int y)

BOOL
cMAP::SetCrash(int x,int y,BYTE block)
{	if (x<0 || y<0 || x>=m_iRealWidth || y>=m_iRealHeight) return FALSE;

	m_pCrashMap[x+y*m_iRealWidth] = block;

	return TRUE;
}

BOOL
cMAP::SetBlank(int x,int y,BYTE block)
{	if (x<0 || y<0 || x>=m_iRealWidth || y>=m_iRealHeight) return FALSE;

	if (block)	m_lpTileMap[x+y*m_iRealWidth]	|= 0x8000;
	else		m_lpTileMap[x+y*m_iRealWidth]	&= 0x7fff;

	return TRUE;
}

BOOL
cMAP::SetOverlap(int x,int y,BYTE block)
{
	if (x<0 || y<0 || x>=m_iRealWidth || y>=m_iRealHeight) return FALSE;
	if (block)	m_lpTileMap[x+y*m_iRealWidth]	|= 0x4000;
	else		m_lpTileMap[x+y*m_iRealWidth]	&= 0xbfff;

	return TRUE;
}

//
//	x,y에 타일이 지나갈 수 없는 곳인지 체크한다.
BOOL
cMAP::isCrashTile(int x,int y)
{	
	if (x < 0 || x >= m_iRealWidth || y < 0 || y >= m_iRealHeight	) return TRUE;
	if (m_pCrashMap[x+y*m_iRealWidth]								) return TRUE;	//	크래쉬 맵을 체크하고..
	if (isBlockedTile(x,y)											) return TRUE;	//	타일 자체를 체크한다.

	return FALSE;
}

//
//맵 위치 설정
void
cMAP::setPos(int x,int y)
{
	x	=	min(x,m_iRealWidth - g_MAPWND.TPW);
	y	=	min(y,m_iRealHeight- g_MAPWND.TPH);
	x	=	max(x,0);
	y	=	max(y,0);

	x	=	x/dSHARE_TILE*dSHARE_TILE;
	y	=	y/dSHARE_TILE*dSHARE_TILE;

	m_pos.Set(x,y);

	setPixelPos();
}	//	cMAP::setPos(int x,int y)

BOOL
cMAP::IsBlank(int x,int y)
{
//	if (x<0 || y<0 || x>=m_iRealWidth || y>=m_iRealHeight) return FALSE;
//	if (m_pTileMap[x+y*m_iRealWidth] & 0x8000) return TRUE;

	return FALSE;
}

BOOL
cMAP::IsOverlap(int x,int y)
{
//	if (x<0 || y<0 || x>=m_iRealWidth || y>=m_iRealHeight) return FALSE;
//	if (m_pTileMap[x+y*m_iRealWidth] & 0x4000) return TRUE;

	return FALSE;
}

BOOL
cMAP::isMissileBlockedTile(int x,int y)
{
	if (x<0 || y<0 || x>=m_iRealWidth || y>=m_iRealHeight) return TRUE;

	int	iIndex	=	m_pPattern->GetMapIndex(x/dSHARE_TILE*dSHARE_TILE,y/dSHARE_TILE*dSHARE_TILE);

	return	g_aBlockedInfo[m_iTileSet].isMissileBlockedTile(iIndex);
}

//	미사일 공격을 막는 오브젝트 영역에 타일
BOOL
cMAP::isMisslieBlockedObjectTile(int x,int y)
{
	if (m_pCrashMap[x+y*m_iRealWidth]	==	2)
		return	TRUE;

	return	FALSE;
}

//
//	캐릭터가 지나갈 수 없는 타일이다.
BOOL
cMAP::isBlockedTile(int x,int y)
{
	if (x<0 || y<0 || x>=m_iRealWidth || y>=m_iRealHeight) return TRUE;

	int	iIndex	=	m_pPattern->GetMapIndex(x/dSHARE_TILE*dSHARE_TILE,y/dSHARE_TILE*dSHARE_TILE);

	if	(m_iTileSet	==	eTS_BRUNENSTIG	)	//마을 - 브룬넨슈티그
	{
		int iTile	=	0;

		int	aExceptTile[200]	=	
		{
			1545,1613,1611,1545,1603,1684,1660,1684,1675,1655,1665,1675,1650,1675,1563,1553,
			1675,1680,1601,1597,1537,1603,1607,1599,1605,1607,1541,1625,1635,1645,1630,1543,
			1640,1670,1548,1568,1625,1620,1539,1615,1617,1609,1619,1615,1617,1609,1619,1919,
			1904,1871,1865,1867,1912,1908,1896,1919,1875,1873,1814,1810,1810,1916,1859,1803,
			1795,1805,1861,1888,1822,1826,1900,1880,1855,1853,1793,1869,1806,1884,1876,1801,
			1797,1830,1818,1799,1857,1801,1797,1830,1818,1799,1857,1892,1863,1549,1554,1569,
			1564,1547,1558,1559,1573,1574,0xffff
		};

		while(aExceptTile[iTile]!=0xffff)
		{
			if (aExceptTile[iTile]	==	iIndex)	return	FALSE;
			iTile++;
		}
	}

	return	g_aBlockedInfo[m_iTileSet].isBlockedTile(iIndex);
}
/*
	switch(m_iTileSet)
	{
		case	dTILESET_GRASSLAND			:	//초원
		{
			if	(iIndex	>=	1000	&&	iIndex	<=	1300)	return	TRUE;
			if	(iIndex	>=	2200)	return	TRUE;
			break;
		}

		case	dTILESET_MOUNTAINS			:	//산악
		{
			if	(iIndex	>=	2300	)	return	TRUE;
			if	(iIndex	>=	1000	&&	iIndex	<=	1200)	return	TRUE;
			break;
		}

		case	dTILESET_DESERT				:	//사막
		{
			break;
		}

		case	dTILESET_SAVANA				:	//사바나
		{
			break;
		}

		case	dTILESET_CAVE				:	//동굴
		{
			if	(iIndex	>=	200	)	return	TRUE;
			break;
		}

		case	dTILESET_DUNGEON			:	//던젼
		{
			if	(iIndex	>=	200	)	return	TRUE;
			break;
		}

		case	dTILESET_TOWER				:	//타워
		{
			break;
		}

		case	dTILESET_MINE				:	//광산
		{
			if	(iIndex	>=	500	)	return	TRUE;
			break;
		}

		case	dTILESET_HELL				:	//지하세계(지옥)
		{
			break;
		}

		case	dTILESET_HEAVEN				:	//천상계(천국)
		{
			break;
		}

		case	dTILESET_BRUNENSTIG			:	//마을 - 브룬넨슈티그
		{
			int iTile	=	0;

			int	aExceptTile[200]	=	
			{
				1545,1613,1611,1545,1603,1684,1660,1684,1675,1655,1665,1675,1650,1675,1563,1553,
				1675,1680,1601,1597,1537,1603,1607,1599,1605,1607,1541,1625,1635,1645,1630,1543,
				1640,1670,1548,1568,1625,1620,1539,1615,1617,1609,1619,1615,1617,1609,1619,1919,
				1904,1871,1865,1867,1912,1908,1896,1919,1875,1873,1814,1810,1810,1916,1859,1803,
				1795,1805,1861,1888,1822,1826,1900,1880,1855,1853,1793,1869,1806,1884,1876,1801,
				1797,1830,1818,1799,1857,1801,1797,1830,1818,1799,1857,1892,1863,1549,1554,1569,
				1564,1547,1558,1559,1573,1574,0xffff
			};

			while(aExceptTile[iTile]!=0xffff)
			{
				if (aExceptTile[iTile]	==	iIndex)	return	FALSE;
				iTile++;
			}

			if	(iIndex	>=	1000	&&	iIndex	<=	2000	)	return	TRUE;
			
			break;
		}

		case	dTILESET_BIGAEPLE			:	//마을 - 비개플
		{
			break;
		}

		case	dTILESET_AUGUSTA			:	//마을 - 아우구스타
		{
			break;
		}

		case	dTILESET_BRIDGEHEAD			:	//마을 - 브리지헤드
		{
			break;
		}

		case	dTILESET_MOUNTAINS_VILLAGE	:	//마을 - 산악마을
		{
			if	(iIndex	>=	2300 && iIndex	<=	2814)	return	TRUE;
			if	(iIndex	>=	3300 )	return	TRUE;
			break;
		}

		case	dTILESET_ARIAN				:	//마을 - 아리안
		{
			break;
		}

		case	dTILESET_RUINED_CITY		:	//마을 - 폐허
		{
			break;
		}

		case	dTILESET_FARMHOUSE			:	//마을 - 농가
		{
			break;
		}

		case	dTILESET_GYPSY				:	//마을 - 집시촌
		{
			break;
		}

		case	dTILESET_ROOM				:	//마을 - 집시촌
		{
			if	(iIndex	>=	0x0e00	&& iIndex	<=	0x0e0a)	return	TRUE;
			break;
		}

	}

	return FALSE;
}	//	cMAP::isMissileBlockedTile(int x,int y)
*/

//
//	여기에 뭔가를 세울 수 있냐?
BOOL
cMAP::isBuildAblePos(int x,int y,BYTE *crash)
{	int xs			=	256;
	int ys			=	256;
	int xp			=	x-128;
	int yp			=	y-128;
	int	bufferPoint	=	0,count,xPoint,size;

	if (getBuilding(x,y)!=0xff)	return FALSE;	//	건물 인덱스 저장 위치다. -_-
	if (getObject(x,y)!=0xffff)	return FALSE;	//	오브젝트 인덱스 저장 위치다. -_-

	return	TRUE;

	for(int j=0;j<ys;j++)
	{	count	=	crash[bufferPoint++];
		xPoint	=	xp;

		for(int i=0;i<count;i++)
		{	xPoint	+=	crash[bufferPoint++];
			size	=	crash[bufferPoint++];

			for (int pos=0;pos<size;pos++)
			{	
				if (isCrashTile(xPoint+pos,yp+j))	//	충돌하기는 하는데..
				{
					if (getBuilding(xPoint+pos,yp+j)!=0xff)	return FALSE;	//	건물 인덱스 저장 위치다. -_-
					if (getObject(xPoint+pos,yp+j)!=0xffff)	return FALSE;	//	오브젝트 인덱스 저장 위치다. -_-
				}
			}

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
	}

	return TRUE;
}	//	cMAP::isBuildAblePos(int x,int y,BYTE *crash)

//	BLANK 타일과,BLOCKED 타일을 설정한다.
void
cMAP::PutCrashBlock(int x,int y,int dx,int dy,BYTE	*crash,WORD color)
{
	int xs			=	256;
	int ys			=	256;
	int xp			=	x-128;
	int yp			=	y-128;
	int	bufferPoint	=	0,count,xPoint,size;

	for(int j=0;j<ys;j++)
	{	count	=	crash[bufferPoint++];
		xPoint	=	xp;

		for(int i=0;i<count;i++)
		{	xPoint	+=	crash[bufferPoint++];
			size	=	crash[bufferPoint++];

			for (int pos=0;pos<size;pos++)
			{	
				x	=	(xPoint+pos	)*dTILE_REAL_WIDTH	-	dx;
				y	=	(yp+j		)*dTILE_REAL_HEIGHT	-	dy;

				cDRAW::FillHB(color,x,y,x+dTILE_REAL_WIDTH-1,y+dTILE_REAL_HEIGHT-1);
			}

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
	}
}

//
//	브릿지 맵
void
cMAP::setBridgeMap(int x,int y,BYTE *crash)
{	
	int xs			=	256;
	int ys			=	256;
	int xp			=	x-128;
	int yp			=	y-128;
	int	bufferPoint	=	0,count,xPoint,size;
	int	i,j;

	if (crash)
	{
		for(j=0;j<ys;j++)
		{
			count	=	crash[bufferPoint++];
			xPoint	=	xp;

			for(i=0;i<count;i++)
			{
				xPoint	+=	crash[bufferPoint++];
				size	=	crash[bufferPoint++];

				for (int pos=0;pos<size;pos++)
				{
					int	iX	=	xPoint+pos;
					int	iY	=	yp+j;

					if	(iX < 0 || iX >= m_iRealWidth || iY < 0 || iY >= m_iRealHeight)
						continue;

					int	iAddress	=	iX+iY*m_iRealWidth;

					if	(m_lpTileMap[iAddress]	&	dMASK_OBJECT)
						continue;
					if	(m_lpTileMap[iAddress]	&	dMASK_MISSLIE_BLOCK_OBJECT)
						continue;
					
					m_lpTileMap[iAddress] = 0;
				}

				bufferPoint	+=	size;
				xPoint		+=	size;
			}
		}
	}
}	//	cMAP::setBridgeMap(int x,int y,BYTE *crash);

//	BLANK 타일과,BLOCKED 타일을 설정한다.
BOOL
cMAP::setCrashMap(int x,int y,BYTE *crash,BYTE *block,BYTE *overlap,BOOL clear,BOOL _bIsMissileBlocked)
{	
	int xs			=	256;
	int ys			=	256;
	int xp			=	x-128;
	int yp			=	y-128;
	int	bufferPoint	=	0,count,xPoint,size;
	int	i,j;

	if (crash)
	{
		for(j=0;j<ys;j++)
		{	count	=	crash[bufferPoint++];
			xPoint	=	xp;

			for(i=0;i<count;i++)
			{	xPoint	+=	crash[bufferPoint++];
				size	=	crash[bufferPoint++];

				for (int pos=0;pos<size;pos++)
				{	if (clear)
					{	if (!SetCrash(xPoint+pos,yp+j,0))
							return FALSE;
					}
					else
					{
						int	iSetValue	=	crash[bufferPoint+pos];

						if (_bIsMissileBlocked && iSetValue )	iSetValue	=	2;

						if (!SetCrash(xPoint+pos,yp+j,iSetValue))	return FALSE;
					}
				}

				bufferPoint	+=	size;
				xPoint		+=	size;
			}
		}
	}

	if (block)
	{
		bufferPoint	=	0;

		for(j=0;j<ys;j++)
		{	count	=	block[bufferPoint++];
			xPoint	=	xp;

			for(int i=0;i<count;i++)
			{	xPoint	+=	block[bufferPoint++];
				size	=	block[bufferPoint++];

				for (int pos=0;pos<size;pos++)
				{	if (clear)	SetBlank(xPoint+pos,yp+j,0);
					else		SetBlank(xPoint+pos,yp+j,1);
				}

				bufferPoint	+=	size;
				xPoint		+=	size;
			}
		}
	}

	if (overlap)
	{
		bufferPoint	=	0;

		for(j=0;j<ys;j++)
		{	count	=	overlap[bufferPoint++];
			xPoint	=	xp;

			for(int i=0;i<count;i++)
			{	xPoint	+=	overlap[bufferPoint++];
				size	=	overlap[bufferPoint++];

				for (int pos=0;pos<size;pos++)
				{	if (clear)	SetOverlap(xPoint+pos,yp+j,0);
					else		SetOverlap(xPoint+pos,yp+j,1);
				}

				bufferPoint	+=	size;
				xPoint		+=	size;
			}
		}
	}

	return TRUE;
}

//
//	... 한다.(줴길.. 기억도 안나네..;;)
BOOL
cMAP::SetBlankMap(int x,int y,BYTE *block,BOOL clear)
{	int xs			=	256;
	int ys			=	256;
	int xp			=	x-128;
	int yp			=	y-128;
	int	bufferPoint	=	0,count,xPoint,size;

	bufferPoint	=	0;

	for(int j=0;j<ys;j++)
	{	count	=	block[bufferPoint++];
		xPoint	=	xp;

		for(int i=0;i<count;i++)
		{	xPoint	+=	block[bufferPoint++];
			size	=	block[bufferPoint++];

			for (int pos=0;pos<size;pos++)
			{	if (clear)	SetBlank(xPoint+pos,yp+j,0);
				else		SetBlank(xPoint+pos,yp+j,1);
			}

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
	}

/*	bufferPoint	=	0;

	for(j=0;j<ys;j++)
	{	count	=	block[bufferPoint++];
		xPoint	=	xp;

		for(int i=0;i<count;i++)
		{	xPoint	+=	block[bufferPoint++];
			size	=	block[bufferPoint++];

			for (int pos=0;pos<size;pos++)
			{	if (clear)	SetOverlap(xPoint+pos,yp+j,0);
				else		SetOverlap(xPoint+pos,yp+j,1);
			}

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
	}
*/
	return TRUE;
}	//	cMAP::SetBlankMap(int x,int y,BYTE *block,BOOL clear)

/////////////////////////////////////////////////////////////////////////
//	정적 멤버
/////////////////////////////////////////////////////////////////////////

BOOL
cMAP::Init(int xs,int ys,int tile,char *name)
{
//	g_bmMapList.Init(dMAX_MAP,dBAR_RADIO);		//	바메뉴 초기화

	cMAP_INFO mapInfo;

	mapInfo.m_iWidth	=	xs;
	mapInfo.m_iHeight	=	ys;
	mapInfo.m_iTileSet	=	tile;
	strcpy(mapInfo.m_strName,name);

	cMAP::Add(&mapInfo);

	return	TRUE;
}

int
cMAP::GetValidMap()
{
	int	i,iNewSerial	=	0xffff;

	for (i =0;i<dMAX_MAP;i++)
		if (g_aMap[i].m_iSerial	==	0xffff)
		{
			iNewSerial	=	i;
			break;
		}

	return	iNewSerial;
}

int
cMAP::GetFirstMap()
{
	int	i;

	for (i =0;i<dMAX_MAP;i++)
		if (g_aMap[i].m_iSerial	!=	0xffff)
			return i;

	return	0xffff;
}

BOOL
cMAP::Add(cMAP_INFO *mapInfo)
{
	int	iNewSerial	=	GetValidMap();

	if( iNewSerial==0xffff)
		return cMSG::Put("Error in cMAP::Add",dMSG_TOO_MANY_MAP);

	if (g_aMap[iNewSerial].Init(mapInfo,iNewSerial))
	{
		int x	=	g_bmMapList.GetRightPos();

		char	name[32];

		ustrncpy(name,g_aMap[iNewSerial].m_strName,20);

		g_bmMapList.Add(name,iNewSerial,x+3,4);

		Select(iNewSerial);

		return TRUE;
	}

	return FALSE;
}

//
//	리셋
BOOL
cMAP::Reset(cMAP_INFO *mapInfo,int index)
{
	if (g_aMap[index].Init(mapInfo,index))
	{
		char	name[32];

		ustrncpy(name,g_aMap[index].m_strName,20);

		g_bmMapList.Set(index,name);

		Select(index);

		return TRUE;
	}

	return FALSE;
}	//	cMAP::Reset(cMAP_INFO *mapInfo,int index)

//
//	영역안에 오브젝트,건물,캐릭터 선택
void
cMAP::select(cRECT *_lpRect,cRECT *_lpRectScreen)
{
	m_setActor.select(_lpRect,_lpRectScreen);
	selectObject(_lpRect);
	selectBuilding(_lpRect);

	updateAddonObject();
}	//	cMAP::select(cRECT *_lpRect,cRECT *_lpRectScreen)

//
//	_iIndex번째 맵을 선택한다.
BOOL
cMAP::Select(WORD _iIndex)
{
	if (s_pTile)
		for (int i=0;i<10000;i++)
			pKILL(s_pTile[i]);

	pKILL(s_pTile);

	if (_iIndex						>=	dMAX_MAP)	return FALSE;
	if (g_aMap[_iIndex].IsActive()	==	FALSE	)
	{
		if (g_iSelectMap>=dMAX_MAP)	g_iSelectMap=0;

		if (g_aMap[g_iSelectMap].IsActive())	return	FALSE;

		for (int i=0;i<dMAX_MAP;i++)
		{
			g_iSelectMap	=	i;

			if (g_aMap[g_iSelectMap].IsActive())	return	FALSE;
		}

		return FALSE;
	}

	g_lpActmap		=	&g_aMap[_iIndex];
	g_iSelectMap	=	_iIndex;

	TOOLWND.endEditBlockOfBuilding();

	g_lpActmap->m_pArea->Select();

	g_bmMapList.Check(_iIndex,TRUE);

	g_lpActmap->m_setCharacter.reBuild();
	g_lpActmap->m_setActor.reBuild();
	g_lpActmap->updateAddonObject();

	return TRUE;
}	//	cMAP::Select(WORD _iIndex)

//
//	_iIndex번째 맵을 닫는다.
void
cMAP::Close(int _iIndex)
{
	g_aMap[_iIndex].Reset();

	g_bmMapList.Remove(_iIndex);

	_iIndex	=	GetFirstMap();

	Select(_iIndex);
}	//	cMAP::Close(int index)

//
//	_strFn의 맵 정보를 읽어온다.
BOOL
cMAP::GetMapInfo(char *_strFn,cMAP_INFO	*_lpMapInfo)
{
	cFILE	file;
	int		iDummy;
	char	strHeader[61];

	if (!file.Open(_strFn,"rb"))	return FALSE;

	file.Read(&iDummy,4);					//	화일 사이즈
	file.Read(&iDummy,4);					//	텍스트 데이터의 위치
	file.Read(strHeader,60);				//	헤더

	file.Read(_lpMapInfo,sizeof(cMAP_INFO));//	맵 정보

	//	그전에.. 헤더 함 확인해 주자. -_-a
	return	TRUE;
}	//	cMAP::GetMapInfo(char *_strFn,cMAP_INFO	*_lpMapInfo)

//
//	_strHeader을 기준으로 이 맵 화일의 버젼을 얻어온다.
int
cMAP::GetVersion(char *_strHeader)
{
	l_iVersion	=	GetMapDataVersion(_strHeader);

	return	l_iVersion;
}

//
//	맵 화일이냐?
BOOL
cMAP::IsMapFile(char *_strFn)
{
	cFILE	file;
	int		l_iVersion	=	0xffff;
	char	strHeader[61];

	if (!file.Open(_strFn,"rb"))	return FALSE;

	file.Seek(8,SEEK_SET);					//	화일 사이즈
	file.Read(strHeader,60);				//	헤더
	file.Close();

	if (GetVersion(strHeader)	==	0xffff)
		return	FALSE;

	return	TRUE;
}

//
//	카르마 컨테이너 저장
void
cMAP::SaveKarmaContainer(cFILE *_lpFile,CKarmaContainer *_lpKC)
{
	_lpFile->Write(&_lpKC->m_wKarmaCount,2);
	_lpFile->Write(&_lpKC->m_wTitleLength,2);

	if (_lpKC->m_wTitleLength)
	{
		RemoveQuotationMark(_lpKC->m_pstrTitle);
		cMessSign::EncodeDataBuffer2(_lpKC->m_pstrTitle,_lpKC->m_wTitleLength);
		_lpFile->Write(_lpKC->m_pstrTitle,_lpKC->m_wTitleLength);
		cMessSign::DecodeDataBuffer2(_lpKC->m_pstrTitle,_lpKC->m_wTitleLength);
	}

	for (int iKarma=0;iKarma<_lpKC->m_wKarmaCount;iKarma++)
		SaveKarma(_lpFile,&_lpKC->m_pKarma[iKarma]);
}

//
//	카르마 컨테이너 읽기
void
cMAP::ReadKarmaContainer(cFILE *_lpFile,CKarmaContainer *_lpKC,BOOL _bIsDecode)
{
	_lpFile->Read(&_lpKC->m_wKarmaCount,2);
	_lpFile->Read(&_lpKC->m_wTitleLength,2);

	_lpKC->m_wSerial	=	0xffff;
	_lpKC->m_pKarma		=	new CKarma	[_lpKC->m_wKarmaCount];

	if (_lpKC->m_wTitleLength)
	{
		pKILL(_lpKC->m_pstrTitle);
		_lpKC->m_pstrTitle	=	new char	[_lpKC->m_wTitleLength];
		_lpFile->Read(_lpKC->m_pstrTitle,_lpKC->m_wTitleLength);

		if	(_bIsDecode)
			cMessSign::DecodeDataBuffer2(_lpKC->m_pstrTitle,_lpKC->m_wTitleLength);

		RemoveQuotationMark(_lpKC->m_pstrTitle);
	}

	for (int iKarma=0;iKarma<_lpKC->m_wKarmaCount;iKarma++)
		ReadKarma(_lpFile,&_lpKC->m_pKarma[iKarma],_bIsDecode);
}

//
//	카르마 저장하기
void
cMAP::SaveKarma(cFILE *_lpFile,CKarma *_lpKarma)
{
	int	i;

	cMessSign::EncodeDataBuffer2(_lpKarma,sizeof(CKarmaBasicInfo));
	_lpFile->Write(_lpKarma,sizeof(CKarmaBasicInfo));
	cMessSign::DecodeDataBuffer2(_lpKarma,sizeof(CKarmaBasicInfo));

	if (_lpKarma->m_pstrTitle)
	{
		if (_lpKarma->m_wTitleLength	== 0)
			ERRMSG("error in cMAP::SaveKarma", "... For some reason, the string size is 0, but the string exists.. -_-?");

		RemoveQuotationMark(_lpKarma->m_pstrTitle);

		cMessSign::EncodeDataBuffer2(_lpKarma->m_pstrTitle,_lpKarma->m_wTitleLength);
		_lpFile->Write(_lpKarma->m_pstrTitle,_lpKarma->m_wTitleLength);
		cMessSign::DecodeDataBuffer2(_lpKarma->m_pstrTitle,_lpKarma->m_wTitleLength);
	}

	for (i=0;i<_lpKarma->m_wTriggerCount;i++)	SaveKarmaItem(_lpFile,&_lpKarma->m_pTrigger[i]);
	for (i=0;i<_lpKarma->m_wReactionCount;i++)	SaveKarmaItem(_lpFile,&_lpKarma->m_pReaction[i]);
}

//
//	카르마 저장하기
void
cMAP::ReadKarma(cFILE *_lpFile,CKarma *_lpKarma,BOOL _bIsDecode)
{
	int	i;

	_lpKarma->m_wActiveCount	=	0;
	_lpKarma->m_wActivePeriod	=	0;

	if	(l_iVersion	>=	eVUI_26_ADD_DUNGEON_VALUE_AND_KARMA_VALUE)
		_lpFile->Read(_lpKarma,sizeof(CKarmaBasicInfo));
	else
	if	(l_iVersion	>=	eVUI_20_ADD_KARMA_OCCUR_CHANCE)
		_lpFile->Read(_lpKarma,sizeof(CKarmaBasicInfo)-4);
	else
		_lpFile->Read(_lpKarma,sizeof(CKarmaBasicInfo)-2-4);

	if	(_bIsDecode)
		cMessSign::DecodeDataBuffer2(_lpKarma,sizeof(CKarmaBasicInfo));

	if	(l_iVersion	<	eVUI_22_ADD_TRIGGER_ACTIVE_CONDITION)
	{
		WORD	isAnd					=	*(WORD *)(&_lpKarma->m_wSerial+2);

		_lpKarma->m_isWantAndCondition	=	min(isAnd,1);
		_lpKarma->m_isWantNotComplete	=	FALSE;
	}

	if (_lpKarma->m_wTitleLength)
	{
		pKILL(_lpKarma->m_pstrTitle);

		_lpKarma->m_pstrTitle	=	new char [_lpKarma->m_wTitleLength];

		_lpFile->Read(_lpKarma->m_pstrTitle,_lpKarma->m_wTitleLength);

		if	(_bIsDecode)
			cMessSign::DecodeDataBuffer2(_lpKarma->m_pstrTitle,_lpKarma->m_wTitleLength);

		RemoveQuotationMark(_lpKarma->m_pstrTitle);
	}

	_lpKarma->allocBuffer();

	for (i=0;i<_lpKarma->m_wTriggerCount;i++)
		ReadKarmaItem(_lpFile,&_lpKarma->m_pTrigger[i],_bIsDecode);

	for (i=0;i<_lpKarma->m_wReactionCount;i++)
		ReadKarmaItem(_lpFile,&_lpKarma->m_pReaction[i],_bIsDecode);
}


//
//	카르마 저장하기
void
cMAP::SaveKarmaItem(cFILE *_lpFile,CKarmaItem *_lpKarma)
{
	cMessSign::EncodeDataBuffer2(_lpKarma,sizeof(CKarmaItemBasicInfo));
	_lpFile->Write(_lpKarma,sizeof(CKarmaItemBasicInfo));
	cMessSign::DecodeDataBuffer2(_lpKarma,sizeof(CKarmaItemBasicInfo));

	if (_lpKarma->m_strString)
	{
		if (_lpKarma->m_wStringSize	== 0)
			ERRMSG("error in cMAP::SaveKarmaItem","... For some reason, the string size is 0, but the string exists.. -_-?");

		RemoveQuotationMark(_lpKarma->m_strString);
		cMessSign::EncodeDataBuffer2(_lpKarma->m_strString,_lpKarma->m_wStringSize);
		_lpFile->Write(_lpKarma->m_strString,_lpKarma->m_wStringSize);
		cMessSign::DecodeDataBuffer2(_lpKarma->m_strString,_lpKarma->m_wStringSize);
	}
}

//
//	카르마 읽기
void
cMAP::ReadKarmaItem(cFILE *_lpFile,CKarmaItem *_lpKarma,BOOL _bIsDecodeKaramData)
{
	_lpFile->Read(_lpKarma,sizeof(CKarmaItemBasicInfo));

	if	(_bIsDecodeKaramData)
		cMessSign::DecodeDataBuffer2(_lpKarma,sizeof(CKarmaItemBasicInfo));

	if (_lpKarma->m_wStringSize	>	0)
	{
		_lpKarma->m_strString	=	new char [_lpKarma->m_wStringSize];

		_lpFile->Read(_lpKarma->m_strString,_lpKarma->m_wStringSize);

		if	(_bIsDecodeKaramData)
			cMessSign::DecodeDataBuffer2(_lpKarma->m_strString,_lpKarma->m_wStringSize);

		RemoveQuotationMark(_lpKarma->m_strString);
	}
}

void
cMAP::WriteCC(cFILE *_lpFile,CConversationContainer *_lpcc)
{
	if	(!_lpcc)
	{
		WORD	wCount	=	0;

		_lpFile->Write(&wCount,2);
		return;
	}

	_lpFile->Write(&_lpcc->m_wConversationCount,2);

	if (_lpcc->m_wConversationCount	==	0)
		return;

	_lpFile->Write(&_lpcc->m_wDialogBoxSize,2);
	_lpFile->Write(&_lpcc->m_wIsRandomChoiceDialg,2);

	for (int j=0;j<_lpcc->m_wConversationCount;j++)
	{
		CConversation	*lpSpeech	=	&_lpcc->m_pConversation[j];

		cMessSign::EncodeDataBuffer2(lpSpeech,sizeof(CConversationBasicInfo));
		_lpFile->Write(lpSpeech,sizeof(CConversationBasicInfo));//	시리얼
		cMessSign::DecodeDataBuffer2(lpSpeech,sizeof(CConversationBasicInfo));

		if (lpSpeech->m_pstrSpeech)
		{
			RemoveQuotationMark(lpSpeech->m_pstrSpeech);

			cMessSign::EncodeDataBuffer2(lpSpeech->m_pstrSpeech,lpSpeech->m_wSpeechLength);
			_lpFile->Write(lpSpeech->m_pstrSpeech,lpSpeech->m_wSpeechLength);//	텍스트 길이
			cMessSign::DecodeDataBuffer2(lpSpeech->m_pstrSpeech,lpSpeech->m_wSpeechLength);
		}

		for (int iTrigger=0;iTrigger<lpSpeech->m_wTriggerCount;iTrigger++)
			SaveKarmaItem(_lpFile,&lpSpeech->m_pTrigger[iTrigger]);

		for (int iContent=0;iContent<lpSpeech->m_wContentsCount;iContent++)
		{
			CKarmaContainer	*lpkc	=	&lpSpeech->m_pContents[iContent];

			SaveKarmaContainer(_lpFile,lpkc);
		}
	}
}

//
//	카르마 읽기
void
cMAP::ReadCC(cFILE *_lpFile,CConversationContainer	*_lpCC,int _iCount,BOOL _bIsDecodeKaramData)
{
	int j;
	CConversationContainer	*lpcc=	_lpCC;

	lpcc->m_wConversationCount	=	_iCount;
	lpcc->m_pConversation		=	new CConversation [_iCount];

	if	(l_iVersion	>=	eVUI_08_ADD_DBOX_SIZE_AND_NO_SPEECH	)
		_lpFile->Read(&lpcc->m_wDialogBoxSize,2);

	if	(l_iVersion	>=	eVUI_14_ADD_RANDOM_CHOICE_DIALOG	)
		_lpFile->Read(&lpcc->m_wIsRandomChoiceDialg,2);
	else
		lpcc->m_wIsRandomChoiceDialg	=	FALSE;

	for (j=0;j<lpcc->m_wConversationCount;j++)
	{
		CConversation	*lpSpeech	=	&lpcc->m_pConversation[j];

		_lpFile->Read(lpSpeech,sizeof(CConversationBasicInfo));//	기본 정보

		if	(_bIsDecodeKaramData)
			cMessSign::DecodeDataBuffer2(lpSpeech,sizeof(CConversationBasicInfo));

		if	(l_iVersion	<	eVUI_08_ADD_DBOX_SIZE_AND_NO_SPEECH)
		{
			lpSpeech->m_wIsNoSpeech			=	FALSE;
			lpSpeech->m_isWantAndCondition	=	TRUE;
			lpSpeech->m_isWantNotComplete	=	FALSE;
			lpSpeech->m_spareValue			=	FALSE;
		}
		else
		if	(l_iVersion	<	eVUI_22_ADD_TRIGGER_ACTIVE_CONDITION)
		{
			WORD	isAnd					=	*(WORD*)(&lpSpeech->m_wTriggerCount+2);
			lpSpeech->m_isWantAndCondition	=	min(1,isAnd);
			lpSpeech->m_isWantNotComplete	=	FALSE;
			lpSpeech->m_spareValue			=	FALSE;
		}

		lpSpeech->allocSpeechMemory();	//	메모리 할당
		lpSpeech->allocTriggerMemory();	//	메모리 할당
		lpSpeech->allocContentsMemory();	//	메모리 할당

		_lpFile->Read(lpSpeech->m_pstrSpeech,lpSpeech->m_wSpeechLength);//대사 읽기

		if	(_bIsDecodeKaramData)
			cMessSign::DecodeDataBuffer2(lpSpeech->m_pstrSpeech,lpSpeech->m_wSpeechLength);

		RemoveQuotationMark(lpSpeech->m_pstrSpeech);

		for (int iTrigger=0;iTrigger<lpSpeech->m_wTriggerCount;iTrigger++)
			ReadKarmaItem(_lpFile,&lpSpeech->m_pTrigger[iTrigger],_bIsDecodeKaramData);

		if (l_iVersion < eVUI_13_CHANGE_DIALG_SELECT_SPEECH)
		{
			for (int iContent=0;iContent<lpSpeech->m_wContentsCount;iContent++)
			{
				CKarmaContainer	*lpContents	=	&lpSpeech->m_pContents[iContent];
				WORD			wReactionCount;

				lpContents->m_wKarmaCount	=	1;

				_lpFile->Read(&wReactionCount,2);
				_lpFile->Read(&lpContents->m_wTitleLength,2);

				pKILL(lpContents->m_pstrTitle);
				pKILL(lpContents->m_pKarma);

				lpContents->m_pstrTitle	=	new	char	[lpContents->m_wTitleLength];
				lpContents->m_pKarma	=	new	CKarma	[lpContents->m_wKarmaCount];

				CKarma	*lpKarma		=	&lpContents->m_pKarma[0];

				_lpFile->Read(lpContents->m_pstrTitle,lpContents->m_wTitleLength);
				RemoveQuotationMark(lpContents->m_pstrTitle);

				lpKarma->m_wTriggerCount		=	0;
				lpKarma->m_wReactionCount		=	wReactionCount;
				lpKarma->m_isWantAndCondition	=	TRUE;
				lpKarma->m_isWantNotComplete	=	FALSE;

				lpKarma->m_wActivateChance		=	100;
				lpKarma->setTitle("default");
				lpKarma->m_wSerial				=	0;

				for (int iReaction=0;iReaction<wReactionCount;iReaction++)
				{
					ReadKarmaItem(_lpFile,&lpKarma->m_pReaction[iReaction],_bIsDecodeKaramData);
					if	(lpKarma->m_pReaction[iReaction].m_wItem	==	eREACTION_DISPLAY_MAIN_QUEST_EPILOG)
						iReaction	=	iReaction;
				}
			}
		}
		else
		for (int iContent=0;iContent<lpSpeech->m_wContentsCount;iContent++)
		{
			CKarmaContainer	*lpkc	=	&lpSpeech->m_pContents[iContent];

			ReadKarmaContainer(_lpFile,lpkc,_bIsDecodeKaramData);
		}
	}
}

BOOL
cMAP::AutoSave(char *fn,int index,char *_lpstrSaveFileName)	//	09.07.24자동저장
{
	char	strFolder[512],strFileName[512];

	sprintf(strFolder,"%s\\data\\Scenario\\Red stone\\map\\AutoSave\\expansion",_logFolder);
	cFOLDER::Create(strFolder);

	sprintf(strFolder,"%s\\data\\Scenario\\Red stone\\map\\AutoSave",_logFolder);
	sprintf(strFileName,"%s\\%s",strFolder,fn);

	Save(strFileName,index,_lpstrSaveFileName,TRUE,strFolder);

	SetLogFolder();

	return	TRUE;
}


void
cMAP::setLastSaveIP()
{
	WORD wVersion;
	WSADATA wsaData;
	char name[255];
	std::string strIP;

	PHOSTENT hostinfo;
	wVersion = MAKEWORD(2,0);
	if(WSAStartup(wVersion, &wsaData) == 0) 
	{
		if(gethostname(name,sizeof(name)) == 0) 
		{
			if((hostinfo = gethostbyname(name)) != NULL) 
				strIP = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
		}
		WSACleanup();
	}

	int iLocation						= 0;
	std::string::size_type idxBefore	= 0;
	std::string::size_type idxNew		= 0;
	int iIP								= 0;
	int	iLastSaveIP						= 0;

	while(!strIP.empty() && iLocation != 4 && idxNew != std::string::npos)
	{	//	IP 주소 저장!
		idxNew	=	strIP.find('.',idxBefore);

		if(idxNew == std::string::npos)
			iIP = atoi(strIP.substr(idxBefore).c_str());
		else
			iIP = atoi(strIP.substr(idxBefore, idxNew).c_str());

		iLastSaveIP +=		iIP<<(24-8*iLocation);

		idxBefore = idxNew+1;

		++iLocation;
	}

	m_iLastSaveIP	=	iLastSaveIP;
}


char*
cMAP::getLastSaveIP()
{

	static	char	strLastSaveIP[32] = "";

	memset(strLastSaveIP,0,sizeof(char)*32);

	int iLastSaveIP	=	m_iLastSaveIP ;
	for(int i=0;i<4;++i)
	{
		BYTE	bIP	=	iLastSaveIP >> (24-8*i);
		char	strIP[4]	=	"";
		strcat(strLastSaveIP,ultoa(bIP,strIP,10));
		if(i!=3)
			strcat(strLastSaveIP,".");
	}

	return strLastSaveIP;

}



//
//	index에 맵 데이터를 저장한다.
BOOL
cMAP::Save(char *fn,int index,char *_lpstrSaveFileName,BOOL _bIsTempSave,char *_lpstrTargetFolder,BOOL _bIsLocalize)
{
	if	(_bIsTempSave	==	FALSE)
		if	(CheckSVNStatus(fn)	==	FALSE)
			return	FALSE;

	cMessSign::SetDataEncodeTable(eNationJapan);

	if	(_bIsLocalize)
		cMessSign::SetDataEncodeTable(g_iLocalizeNation);

	cFILE	file,fileText;
	DWORD	size,dwAreaDataSavePointSavePoint;

	WORD	*pMap;				//	실제로 저장이 될 타일 정보
	WORD	*pPattern;			//	패턴 정보
	WORD	*pHeightPattern;	//	높이 정보

	int		i=0,j=0,iCount=0,iTextLocate=0,iTextSize=0;
	int		iKarmaInfoLocate,iSaveKarmaInfoLocate;

	if	(index==0xffff)
		index	=	g_iSelectMap;

	char	strTempMap[512],strTempData[512];

	if	(_lpstrTargetFolder)
	{
		sprintf(strTempMap,"%s\\~~tempMap.map",_lpstrTargetFolder);
		sprintf(strTempData,"%s\\textData.tmp",_lpstrTargetFolder);
	}
	else
	{
		strcpy(strTempMap,"~~tempMap.map");
		strcpy(strTempData,"textData.tmp");
	}

	if	(!file.Open(strTempMap,"wb"))
		return cMSG::Put("Error in cMAP::Save","'%s' file write Error!!",fn);

	if	(!fileText.Open(strTempData,"wb"))
		return cMSG::Put("Error in cMAP::Save","'textData.tmp' file write Error!!");

	fileText.Write(&i,4);

	cMAP		*lpMap				=	&g_aMap[index];		//	맵 정보
	cAREA		*lpArea				=	lpMap->m_pArea;			//	영역 세트
	cOBJECT		*lpObject			=	&lpMap->m_object;		//	오브젝트 세트
	cACTOR_SET	*lpActorSet			=	&lpMap->m_setActor;		//	캐릭터 세트
	cVALUE		*lpValue			=	&lpMap->m_Value;		//	카르마 값

	lpMap->m_iWidth					=	lpMap->m_iRealWidth;//	저장할때 사이즈는 다르다.
	lpMap->m_iHeight				=	lpMap->m_iRealHeight;

	lpMap->setLastSaveIP();			//	현재 컴퓨터의 IP 주소를 저장한다.(누가 마지막에 저장했는지 알기 위해 ㅋ)

	file.Write(&i,4);								//	화일 사이즈
	file.Write(&i,4);								//	텍스트 데이터의 위치

	RemoveQuotationMark(lpMap->m_strName);

	char*	strMapFileVersion	=	GetMapDataVersion();

	file.Write(strMapFileVersion,56);		//	헤더

	iSaveKarmaInfoLocate			=	file.Locate();
	file.Write(&iKarmaInfoLocate,4);		//	헤더
	file.Write(lpMap,sizeof(cMAP_INFO));			//	맵 정보

	{
		int		iLocalizeNation	= eNationJapan;
		
		if	(_bIsLocalize)
			iLocalizeNation	=	g_iLocalizeNation;
		
		file.Write(&iLocalizeNation,sizeof(iLocalizeNation));
	}


	dwAreaDataSavePointSavePoint	=	file.Locate();
	file.Write(&i,4);								//	영역 데이터 위치

	if (lpMap->m_aBgmList[0] ==	0xffff	)
	{
		if	(g_bIsLocalizing	==	FALSE)
			ERRMSG("BGM!!!!!","BGM is not set.!!\n\n%s",fn);
	}
	if	(lpMap->m_bf4FieldType==	0		)
	{
		if	(g_bIsLocalizing	==	FALSE)
			ERRMSG("WHW!!!!!","Field type is not set.!!\n\n%s",fn);
	}

	lpMap->m_iWidth		=	lpMap->m_iWidth		/	dSHARE_TILE;
	lpMap->m_iHeight	=	lpMap->m_iHeight	/	dSHARE_TILE;


////////////////////////////////////////////////////////////////////////////////////////////////
//	맵 타일 정보 저장
//	패턴 데이터에서 타일 정보와 높이 데이터를 뽑아내서 이러쿵저러쿵-_- 변환 한다.
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		pPattern		=	new WORD [lpMap->m_iWidth*lpMap->m_iHeight];
		pHeightPattern	=	new WORD [lpMap->m_iWidth*lpMap->m_iHeight];
		pMap			=	new WORD [lpMap->m_iRealWidth*lpMap->m_iRealHeight];

		lpMap->m_pPattern->GetPattnIndex(pPattern,pHeightPattern);

		for (int y=0;y<lpMap->m_iHeight;y++)
			for (int x=0;x<lpMap->m_iWidth;x++)
			{
				int	iIndex	=	pPattern[x+y*lpMap->m_iWidth];

				iIndex		*=	dSHARE_TILES;

				for (int i=0;i<dSHARE_TILE;i++)
					for (int j=0;j<dSHARE_TILE;j++)
						pMap[x*dSHARE_TILE+(y*dSHARE_TILE+i)*lpMap->m_iRealWidth+j	]	=	iIndex+i+j*dSHARE_TILES;
			}

		lpMap->RemarkBlankAndOverlapInfo(pMap);	//	pMap에 블랭크와 오버랩 정보를 그려준다.
		lpMap->RoundCrash();

		file.Write(pMap					,lpMap->m_iRealWidth*lpMap->m_iRealHeight*2);	//	타일맵정보
		file.Write(pHeightPattern		,lpMap->m_iWidth*lpMap->m_iHeight*2);	//	높이 정보(툴에서만 쓴다)

		pKILL(pPattern);		//	패턴은 변환 후에 날려 버린다.
		pKILL(pMap);			//	저장후에 날려 버림
		pKILL(pHeightPattern);	//	저장후에 날려 버림
	}

	if (!l_pInfo)
		l_pInfo	=	_new(WORD,dMAX_MAP_SIZE,"cMAP::Save(INFO)");

	int						iDoorCount	=	0;
	cDoorObjectSimpleInfo	aDoorList[dMAX_DOOR_OBJECT_IN_MAP];

	for (int y=0;y<lpMap->m_iRealHeight;y++)
		for (int x=0;x<lpMap->m_iRealWidth;x++)
		{	
			int	addr	=	x+y*lpMap->m_iRealWidth;

			l_pInfo[addr]	=	0;	//	디폴트는 비어 있다.

			WORD value = lpMap->m_pBuildingMap[addr];

			if	(value != 0xff)
			{	
				l_pInfo[addr]	=	value;
				l_pInfo[addr]	|=	dMASK_BUILDING;

				continue;
			}

			value = lpMap->m_pObjectMap[addr];

			if (value != 0xffff)
			{
				int	iMask	=	dMASK_OBJECT;

				cFIXED_OBJECT_INFO	*lpObjectInfo	=	lpMap->m_object.getFixedObject(value);

				if	(lpMap->m_object.getFixedObjectImage(lpObjectInfo->m_wObject)->IsMissileBlockObject())
					iMask	=	dMASK_MISSLIE_BLOCK_OBJECT;

				l_pInfo[addr]	=	value;
				l_pInfo[addr]	|=	iMask;

				if	(lpMap->m_object.m_pFixedObjectImage[lpObjectInfo->m_wObject].isDoor())
				{
					if	(lpArea->isExistDoorAreaPos(x*dTILE_WIDTH+dTILE_WIDTH/2,y*dTILE_HEIGHT+dTILE_HEIGHT/2))
					{
						if (iDoorCount	<	dMAX_DOOR_OBJECT_IN_MAP)
						{
							aDoorList[iDoorCount].m_wX			=	x;
							aDoorList[iDoorCount].m_wY			=	y;
							aDoorList[iDoorCount].m_wObjectIndex=	value;
							aDoorList[iDoorCount].m_wObjectImage=	lpObjectInfo->m_wObject;

							iDoorCount++;
						}
						else
						{
							ERRMSG("error in save map","Too many statements. (max %d)\n\n%s",dMAX_DOOR_IN_MAP,fn);
						}
					}
				}
				continue;
			}

			value = lpMap->m_pCrashMap[addr];

			if (value)
			{
				l_pInfo[addr]	=	value;

						if (lpMap->isMissileBlockedTile(x,y)		)	l_pInfo[addr]	|=	dMASK_MISSLIE_BLOCK_TILE;
				else	if (lpMap->isMisslieBlockedObjectTile(x,y)	)	l_pInfo[addr]	|=	dMASK_MISSLIE_BLOCK_TILE;
				else													l_pInfo[addr]	|=	dMASK_BLOCKED;

				continue;
			}

			value = lpMap->isBlockedTile(x,y);

			if (value)
			{	
				l_pInfo[addr]	=	value;

				if (lpMap->isMissileBlockedTile(x,y))	l_pInfo[addr]	|=	dMASK_MISSLIE_BLOCK_TILE;
				else									l_pInfo[addr]	|=	dMASK_BLOCKED;
				continue;
			}
		}

	lpMap->RemarkBrigeObject(l_pInfo);	//	pMap에 블랭크와 오버랩 정보를 그려준다.
	file.Write(l_pInfo	,lpMap->m_iRealWidth*lpMap->m_iRealHeight,2);	//	충돌,건물,오브젝트 정보

////////////////////////////////////////////////////////////////////////////////////////////////
//	문 정보 저장
////////////////////////////////////////////////////////////////////////////////////////////////
	file.Write(&iDoorCount,4);	//	문 갯수
	file.Write(aDoorList,sizeof(cDoorObjectSimpleInfo)*iDoorCount);	//	문 갯수

////////////////////////////////////////////////////////////////////////////////////////////////
//	서버에서 쓰는 맵 데이터 저장
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		BYTE	*pWriteBuffer;

		pWriteBuffer=	new	BYTE [lpMap->m_iRealWidth*lpMap->m_iRealHeight];

		memset(pWriteBuffer,0,lpMap->m_iRealWidth*lpMap->m_iRealHeight);

		BYTE	bFlag	=	0;
		int		iAddress=	0;

		for (int y=0;y<lpMap->m_iRealHeight;y++)
		{
			bFlag	=	0;

			for (int x=0;x<lpMap->m_iRealWidth;x++,iAddress++)
			{
				if (l_pInfo[iAddress])
				{
					pWriteBuffer[y*lpMap->m_iRealWidth + x]	=	1;
				}
			}
		}

		file.Write(pWriteBuffer,lpMap->m_iRealWidth*lpMap->m_iRealHeight);	//	충돌,건물,오브젝트 정보
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	커스텀 아이템 저장
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		DWORD		dwLocate,dwBookedLocate	=	file.Locate();
		CCustomItem	*lpItems				=	lpMap->m_customItem.getItemBay();

		file.Write(&iCount,4);	//	스킵 데이터 위치
		file.Write(lpItems,dMAX_CUSTOM_ITEM_COUNT*sizeof(CCustomItem));
		dwLocate		=	file.Locate();
		file.Seek(dwBookedLocate,SEEK_SET);
		file.Write(&dwLocate,4);
		file.Seek(dwLocate,SEEK_SET);
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	캐릭터 저장
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		int	iDataSize	=	sizeof(CCharacterBaseInfo);

		//	클라에서만 쓰는 데이터

		int	iCharacterCount	=	lpMap->m_setCharacter.m_iCount;

		cMessSign::EncodeDataBuffer2(&iCharacterCount,4);
		file.Write(&iCharacterCount,4);		//	NPC+Monster 직업의 수

		WORD	awJobList[dJOB_COUNT];

		for (i=0,iCount=0;i<dJOB_COUNT && iCount < lpMap->m_setCharacter.m_iCount;i++)
		{
			if (lpMap->m_setCharacter.isAvailCharacter(i))
			{
				cCHARACTER	*lpCharacter	=	&lpMap->m_setCharacter.m_aCharacter[i];

				awJobList[iCount++]	=	lpCharacter->m_wBaseJob;
			}
		}

		cMessSign::EncodeDataBuffer2(awJobList,2*iCount);
		file.Write(awJobList,2*iCount);		//	NPC+Monster 직업의 수

		//	클라에서만 쓰는 데이터

		DWORD	dwLocate	=	file.Locate();
		file.Write(&i	,4);	//	클라에서 서버에서 쓰는 데이터를 스킵 하기 위한 포인트

		file.Write(&iDataSize	,4);		//	NPC+Monster 직업의 수

		for (i=0,iCount=0;i<dJOB_COUNT && iCount < lpMap->m_setCharacter.m_iCount;i++)
		{
			if (lpMap->m_setCharacter.isAvailCharacter(i))
			{
				cCHARACTER	*lpCharacter				=	&lpMap->m_setCharacter.m_aCharacter[i];
				CCharacterBaseInfo	saveCharacterData;

				memcpy(&saveCharacterData,lpCharacter,sizeof(CCharacterBaseInfo));
				cMessSign::EncodeDataBuffer2(&saveCharacterData,sizeof(CCharacterBaseInfo));
				file.Write(&saveCharacterData,sizeof(CCharacterBaseInfo));

				int			iAvailCharacterEventCount	=	lpCharacter->getAvailConditionCount();

				file.Write(&iAvailCharacterEventCount,4);

				for (int iEvent=0;iEvent<eMECC_COUNT && iAvailCharacterEventCount;iEvent++)
				{
					CKarmaContainer	*lpkc	=	lpCharacter->m_pEvent[iEvent];

					if (!lpkc)	continue;

					file.Write(&iEvent,4);

					SaveKarmaContainer(&file,lpkc);
				}

				iCount++;
			}
		}

		DWORD	dwWriteLocate	=	file.Locate();

		file.Seek(dwLocate,SEEK_SET);
		file.Write(&dwWriteLocate,4);	//	스킵할 위치
		file.Seek(dwWriteLocate,SEEK_SET);
	}	//	캐릭터 저장


////////////////////////////////////////////////////////////////////////////////////////////////
//	액터 저장
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		int		iActorCount	=	lpActorSet->getCorrectCount();
		DWORD	dwLocate	=	file.Locate();

		file.Write(&iActorCount	,4);	//	스킵할 위치
		cMessSign::EncodeDataBuffer2(&iActorCount	,4);
		file.Write(&iActorCount	,4);	//	액터의 수

		int		iIncorrectKarma=	0;

		for (i=0;i<dMAX_ACTOR_COUNT;i++)
		{
			if	(!lpActorSet->isAvail(i))
				continue;

			cACTOR	*lpActor	=	lpActorSet->getActor(i);

			if	(lpActor->isMisplaced())
				continue;

			if	(lpActor->m_wIsBlockToAutoRegen	==	0xcccc)
				lpActor->m_wIsBlockToAutoRegen	=	FALSE;

			if	(lpActor->m_pCC)
				iIncorrectKarma	+=	lpActor->m_pCC->checkIncorrectKarma(lpActor);

			if	(strlen(lpActor->m_strName)	>=	dNAME_LENGTH)
				ERRMSG("Character name is too long!!","%s\n\n%s",fn,lpActor->m_strName);

			cACTOR_SAVE_DATA	saveActor;

			memcpy(&saveActor,lpActor,sizeof(cACTOR_SAVE_DATA));
			cMessSign::EncodeDataBuffer2(&saveActor,sizeof(cACTOR_SAVE_DATA));

			file.Write(&saveActor,sizeof(cACTOR_SAVE_DATA));	//	저장한다.

			WriteCC(&file,lpActor->m_pCC);
		}

		if	(iIncorrectKarma)
			cMSG::Error("맵 파일 저장 에러","%s 파일에서 %d개의 카르마 오류가 발견 되었습니다.",fn,iIncorrectKarma);

		DWORD	dwWriteLocate	=	file.Locate();
		file.Seek(dwLocate,SEEK_SET);
		file.Write(&dwWriteLocate,4);	//	스킵할 위치
		file.Seek(dwWriteLocate,SEEK_SET);
	}	//	액터 저장
////////////////////////////////////////////////////////////////////////////////////////////////
//	영역
////////////////////////////////////////////////////////////////////////////////////////////////

	DWORD	dwAreaDataSavePoint	=	file.Locate();

	if	(_bIsLocalize	==	FALSE)
	{
		if (!lpArea->linkGate())	//	게이트들 연결
		{
			if	(g_bIsLocalizing	==	FALSE)
				cMSG::Put("Field save warning!!","There is an unconnected exit area in file '%s'.",fn);
		}
	}

	{
		int	iAreaCount	=	lpArea->m_iCount;

		cMessSign::EncodeDataBuffer2(&iAreaCount,4);

		file.Write(&iAreaCount,4);	//	영역의 수
	}

	for (i=0,iCount=0;i<dMAX_AREA && iCount<lpArea->m_iCount ;i++)
	{
		if	(lpArea->IsAvail(i))
		{
			int		iLocate			=	fileText.Locate();
			int		iSkipKarmaLocate=	0;
			char	*lpString		=	lpArea->GetString(i);

			if	(lpArea->m_aArea[i].isEventArea()	||	lpArea->m_aArea[i].isGuildCrest())
			{
				cAreaEvent	*lpEventArea	=	(cAreaEvent	*)&lpArea->m_aArea[i];

				if	(lpEventArea->m_wLinkObject	==	0xffff)
					cMSG::Put("Field save warning!!","There is no linked object in area '%s'.",lpEventArea->m_strName);
			}
			RemoveQuotationMark(lpArea->m_aArea[i].m_strName);

			cAREA_SAVE_DATA	saveAreaData;

			memcpy(&saveAreaData,&lpArea->m_aArea[i],sizeof(cAREA_SAVE_DATA));
			cMessSign::EncodeDataBuffer2(&saveAreaData,sizeof(cAREA_SAVE_DATA));
			file.Write(&saveAreaData,sizeof(cAREA_SAVE_DATA));	//	영역 값

			int		iSKL_Point		=	file.Locate();
			file.Write(&iSkipKarmaLocate,4);									//	텍스트의 위치

			WriteCC(&file,&lpArea->m_aArea[i].m_cc);

			iSkipKarmaLocate		=	file.Locate();

			file.Seek(iSKL_Point,SEEK_SET);
			file.Write(&iSkipKarmaLocate,4);									//	텍스트의 위치
			file.Seek(iSkipKarmaLocate,SEEK_SET);

			if (lpString)	//	텍스트가 있으면..
			{
				size	=	lpArea->m_aArea[i].m_wValue;

				file.Write(&iLocate,4);									//	텍스트의 위치
				fileText.Write(&size,4);	//	문장의 사이즈
				fileText.Write(lpString,size);
			}
			else
			{
				iLocate	=	0xffffffff;
				file.Write(&iLocate,4);									//	텍스트의 위치
			}

			iCount++;
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	상점
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		WORD	wShopCount		=	lpMap->m_shop.getCount();
		DWORD	dwShopInfoLocate=	file.Locate(),dwSkipLocate;
		
		file.Write(&dwShopInfoLocate,4);	//	상점의 수
		file.Write(&wShopCount,2);	//	상점의 수

		for (i=0;wShopCount>0 && i<dMAX_SHOP;i++)
		{
			CShop	*lpShop		=	lpMap->m_shop.get(i);

			if	(!lpShop)
				continue;

			wShopCount--;

			WORD	wItemCount	=	lpShop->getCount();

			RemoveQuotationMark(lpShop->m_strName);

			cShopNameInfo	sInfo;

			strcpy(sInfo.m_strName,lpShop->m_strName);
			sInfo.m_bf1IsDuelPointShop	=	lpShop->m_bf1IsDuelPointShop;
			sInfo.m_bf1IsLevelPerGoldShop		=	lpShop->m_bf1IsLevelPerGoldShop;
			sInfo.m_bf15LevelPerGoldValue		=	lpShop->m_bf15LevelPerGoldValue;
			sInfo.m_bf1IsNotBuyItem				=	lpShop->m_bf1IsNotBuyItem;	// 아이템 구입 불가.

			file.Write(&sInfo,sizeof(sInfo));//	상점 이름
			file.Write(&lpShop->m_dwLimitPrice,4);//가격 인자
			file.Write(&lpShop->m_wSerial,2);//	상점 시리얼
			file.Write(&lpShop->m_wPriceFactor,2);//가격 인자
			file.Write(&lpShop->m_TokkenShop,2);//가격 인자

			file.Write(&wItemCount,2);//아이템의 수

			for (j=0;wItemCount>0;j++)
			{
				CShopItem	*lpItem	=	lpShop->get(j);

				if	(lpItem)
				{
					wItemCount--;

					file.Write(lpItem,sizeof(CShopItem));
				}
			}
		}


		dwSkipLocate	=	file.Locate();
		file.Seek(dwShopInfoLocate,SEEK_SET);
		file.Write(&dwSkipLocate,4);
		file.Seek(dwSkipLocate,SEEK_SET);
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	오브젝트
////////////////////////////////////////////////////////////////////////////////////////////////

//	유동
	{
		lpObject->lineUpSaveObject(index);

		int iObjcetCount	=	lpObject->getTinyObjectCount();

		file.Write(&iObjcetCount	,4		);	//	오브젝트 수
		file.Write(&LineUpRape		,8*100	);	//	오브젝트 수
		file.Write(&LineUpNormal	,8*100	);	//	오브젝트 수
		file.Write(&LineUpFloat		,8*100	);	//	오브젝트 수

		file.Write(LineUpedObject,sizeof(cSAVE_TINY_OBJECT_INFO)*iObjcetCount );	//	오브젝트 저장
	}
//	유동

//	고정
	{
		lpObject->m_denFixedObject.rebuild();

		int iObjcetCount		=	lpObject->getFixedObjectCount();

		file.Write(&iObjcetCount	,4		);	//	오브젝트 수

		for (int iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < iObjcetCount;iIndex++)
		{
			cFIXED_OBJECT_INFO	*lpSaveObject	=	lpObject->getFixedObject(iIndex);

			if(!lpSaveObject)
				continue;

			iCount++;

			if	(lpSaveObject->m_wObject	==	94)
				lpSaveObject->m_wObject	=	94;

			file.Write(lpSaveObject,sizeof(cSAVE_FIXED_OBJECT_INFO));	//	오브젝트 저장
		}
	}
//	고정

////////////////////////////////////////////////////////////////////////////////////////////////
//	건물
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		int	iCount	=	lpMap->m_building.getCount();

		file.Write(&iCount		,4		);					//	건물 수

		for (i=0;i<lpMap->m_building.getMaxCount();i++)
		{
			cBUILDING_INFO	*lpBuilding	=	lpMap->m_building.get(i);

			if (!lpBuilding)	continue;

			file.Write(lpBuilding,sizeof(cSAVE_BUILDING_INFO));	//	건물 정보
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	기타정보
////////////////////////////////////////////////////////////////////////////////////////////////
	file.Write(&lpMap->m_pos,sizeof(cPOINT));	//	현재 위치

////////////////////////////////////////////////////////////////////////////////////////////////
//	카르마
////////////////////////////////////////////////////////////////////////////////////////////////
	iKarmaInfoLocate	=	file.Locate();
	file.Seek(iSaveKarmaInfoLocate,SEEK_SET);
	file.Write(&iKarmaInfoLocate,4);
	file.Seek(iKarmaInfoLocate,SEEK_SET);

	SaveKarmaContainer(&file,&lpMap->m_karma);

////////////////////////////////////////////////////////////////////////////////////////////////
//	캐릭터 패턴
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		int	iWriteSkipSizeLocate	=	file.Locate();

		file.Write(&iWriteSkipSizeLocate,4);
		file.Write(&lpMap->m_cpp.m_wCount,2);

		for	(int i=0;i<c_iCharacterPatternCountInField;i++)
		{
			cCharacterPattern	*lpPattern	=	lpMap->m_cpp.get(i);

			if	(!lpPattern)
				continue;

			file.Write(lpPattern,sizeof(cCharacterPattern)-sizeof(lpPattern->m_aPattern)+sizeof(cCharacterPatternPiece)*lpPattern->m_wCount);
		}

		int	iCurLocate		=	file.Locate();

		file.Seek(iWriteSkipSizeLocate,SEEK_SET);
		file.Write(&iCurLocate,4);
		file.Seek(iCurLocate,SEEK_SET);
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	필드 수치
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		int	iWriteSkipSizeLocate	=	file.Locate();

		file.Write(&iWriteSkipSizeLocate,4);
		file.Write(lpMap->m_astrFieldValue,sizeof(lpMap->m_astrFieldValue));

		int	iCurLocate		=	file.Locate();

		file.Seek(iWriteSkipSizeLocate,SEEK_SET);
		file.Write(&iCurLocate,4);
		file.Seek(iCurLocate,SEEK_SET);
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	텍스트 데이터를 뒤에 붙임
////////////////////////////////////////////////////////////////////////////////////////////////

	fileText.Close();						//	일단 닫고

	fileText.Open(strTempData,"rb");		//	다시 열고..

	char	*buffer;
	int	iTextDataLength	=	fileText.Length;

	buffer	=	new char [iTextDataLength];

	fileText.Read(buffer,iTextDataLength);	//	버퍼에다가 전부 읽어오고..
	fileText.Close();

	DeleteFile(strTempData);				//	지워버린다.

	size	=	file.Locate();				//	현재 위치를 얻어오고..
	file.Seek(4,SEEK_SET);	
	file.Write(&size,4);					//	텍스트 데이터가 들어갈 위치를 저장하고..
	file.Seek(size,SEEK_SET);				//	원래 위치로 돌아 와서..
	file.Write(buffer,iTextDataLength);		//	버퍼의 내용을 저장한다.

////////////////////////////////////////////////////////////////////////////////////////////////
//	이 화일의 사이즈
////////////////////////////////////////////////////////////////////////////////////////////////

	size	=	file.Locate();
	file.Seek(0,SEEK_SET);
	file.Write(&size,4);

	file.Seek(dwAreaDataSavePointSavePoint,SEEK_SET);
	file.Write(&dwAreaDataSavePoint,4);								//	영역 데이터 위치

	file.Close();

	if	(_bIsTempSave	==	FALSE)
	{
		lpMap->SetVirgin();
		strcpy(lpMap->m_strFName,cPROJECT::ExportMapFileName(fn));
	}

	if (_lpstrSaveFileName)
	{
		DeleteFile(_lpstrSaveFileName);
		MoveFile(strTempMap,_lpstrSaveFileName);
	}
	else
	{
		DeleteFile(fn);
		MoveFile(strTempMap,fn);
	}


  	// 10.01.12
  	g_lpActmap->SaveMemoFile();

	return	TRUE;
}

//
//	맵 화일을 index에 불러 들인다.
BOOL
cMAP::Load(char *fn,int index, cMAP* lpMapDataLoad)
{
	cFILE	file;
	int		i=0,j=0,iCount=0,iTexFiletLocate=0,iTextLocate=0,iTextSize=0,iBufferSize=1024;
	char	strHeader[70];
	char	*pBuffer;
	char	name[32];
	DWORD	dwAreaDataSavePoint;

	if (!lpMapDataLoad && index==0xffff)
		index	=	g_iSelectMap;

	if	(!file.Open(fn,"rb"))
		return cMSG::Put("Error in cMAP::Load","'%s' file not found!!",fn);

	if	(!lpMapDataLoad && index ==	g_iSelectMap)		//	현재 활성화된 맵에다 불러오기를 하려고 한다.
		g_lpActmap	=	NULL;			//	현재 활성화된 맵

	MSGOUT("Map loading %s",fn);

	cMAP	*lpMap	=	NULL;
	
	if(lpMapDataLoad)		//	 데이터만 로딩한당! 
		lpMap	=	lpMapDataLoad;
	else
		lpMap	=	&g_aMap[index];

	lpMap->Reset();

	file.Read(&i,4);								//	화일 사이즈
	file.Read(&iTexFiletLocate,4);					//	텍스트 데이터의 위치
	file.Read(strHeader,60);							//	헤더

	GetVersion(strHeader);	//	버젼 구하기

	BOOL	bIsDecodeKaramData	=	FALSE;

	if	(l_iVersion	>=	eVVI_49_ENCRYT_MORE)	
		bIsDecodeKaramData	=	TRUE;

	if (l_iVersion	<	eVUI_10_ADD_BGM)	
	{
		file.Read(lpMap,sizeof(CMapBaseInfo));				//	맵 정보
	}
	else
	if (l_iVersion	<	eVUI_21_ADD_BGM_LIST)
	{
		int	iBgm;
		memset(lpMap,0,sizeof(cMAP_INFO));
		file.Read(lpMap,sizeof(CMapBaseInfo));				//	맵 정보
		file.Read(&iBgm,4);				//	맵 정보
		lpMap->m_aBgmList[0]	=	iBgm-1;
	}
	else
	if (l_iVersion	<	eVUI_24_ADD_CORRECT_MAGIC_RESISTANCE)
	{
		memset(lpMap,0,sizeof(cMAP_INFO));
		file.Read(lpMap,sizeof(CMapBaseInfo20031111));				//	맵 정보
	}
	else
	if (l_iVersion	<	eVUI_30_ADD_CORECT_MONSTER_RESISTANCE)
	{
		memset(lpMap,0,sizeof(cMAP_INFO));
		file.Read(lpMap,sizeof(cMapBaseInfo20050527));				//	맵 정보
	}
	else
		file.Read(lpMap,sizeof(cMAP_INFO));				//	맵 정보

	int		iLocalizeNation	= eNationJapan;

	if	(l_iVersion	>=	eVVI_51_ENCRYT2)
	{
		file.Read(&iLocalizeNation,sizeof(g_iLocalizeNation));
		cMessSign::SetDataEncodeTable(iLocalizeNation);
	}
	else
		cMessSign::SetDataEncodeTable(-1);

	if (l_iVersion	>=	eVUI_17_ADD_AREA_SAVE_POINT)
		file.Read(&dwAreaDataSavePoint,4);	//	영역 데이터 위치

	pBuffer	=	new char [iBufferSize];				//	텍스트 데이터를 읽어들일 버퍼

	lpMap->m_iRealWidth		=	lpMap->m_iWidth;		//	맵 사이즈
	lpMap->m_iRealHeight	=	lpMap->m_iHeight;		//	맵 사이즈
	lpMap->m_iWidth			/=	dSHARE_TILE;			//	보여지는 타일 사이즈에서의 맵 사이즈.
	lpMap->m_iHeight		/=	dSHARE_TILE;			//	보여지는 타일 사이즈에서의 맵 사이즈.

	lpMap->Create();									//	위에 정보를 바탕으로 맵을 생성한다.

	RemoveQuotationMark(lpMap->m_strName);

//	
	cAREA		*lpArea		=	lpMap->m_pArea;			//	영역 정보
	cOBJECT		*lpObject	=	&lpMap->m_object;		//	바닥에 깔리는 오브젝트 정보
	cACTOR_SET	*lpActorSet	=	&lpMap->m_setActor;

	lpMap->loadMapsetImage();

////////////////////////////////////////////////////////////////////////////////////////////////
//	타일 정보를 읽어와 패턴 데이터에서  와 높이 데이터를 뽑아내서 이러쿵저러쿵-_- 변환 한다.
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		WORD *pPattern		=	new WORD [lpMap->m_iWidth*lpMap->m_iHeight];
		WORD *pHeightPattern=	new WORD [lpMap->m_iWidth*lpMap->m_iHeight];
		WORD *pMap			=	new WORD [lpMap->m_iRealWidth*lpMap->m_iRealHeight];

		file.Read(pMap			,lpMap->m_iRealWidth*lpMap->m_iRealHeight*2);	//	타일맵정보
		file.Read(pHeightPattern,lpMap->m_iWidth*lpMap->m_iHeight*2);			//	높이 정보(툴에서만 쓴다)

		for (int y=0;y<lpMap->m_iHeight;y++)
			for (int x=0;x<lpMap->m_iWidth;x++)
			{
				int	iTile	=	(pMap[x*dSHARE_TILE+y*dSHARE_TILE*lpMap->m_iRealWidth	]&0x3fff)/dSHARE_TILES;

				pPattern[x+y*lpMap->m_iWidth]	=	iTile;
			}

		lpMap->m_pPattern->PutPattnIndex(lpMap->m_iWidth,lpMap->m_iHeight,0,pPattern,pHeightPattern);

		pKILL(pPattern);		//	패턴은 변환 후에 날려 버린다.
		pKILL(pMap);			//	저장후에 날려 버림
		pKILL(pHeightPattern);	//	저장후에 날려 버림
	}	//	타일 정보를 읽어와 패턴 데이터에서  와 높이 데이터를 뽑아내서 이러쿵저러쿵-_- 변환 한다.

////////////////////////////////////////////////////////////////////////////////////////////////
//	타일 속성 로딩
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		if (!l_pInfo)	l_pInfo	=	_new(WORD,dMAX_MAP_SIZE,"cMAP::Load(INFO)");

		file.Read(l_pInfo	,lpMap->m_iRealWidth*lpMap->m_iRealHeight,2);	//	건물,오브젝트,블럭

		for (int y=0;y<lpMap->m_iRealHeight;y++)
			for (int x=0;x<lpMap->m_iRealWidth;x++)
			{	
				int		addr	=	x+y*lpMap->m_iRealWidth;
				WORD	value	=	l_pInfo[addr] & dMASK_BLOCKED;
				
				if(value)
					lpMap->SetCrash(x , y , 1);
				
// 				if (value & dMASK_BUILDING)
// 				{
// //					lpMap->m_pBuildingMap[addr]	=	value & 0xff;
// 
// 					continue;
// 				}
			}

		lpMap->RoundCrash();
	}	//	타일 속성 로딩


////////////////////////////////////////////////////////////////////////////////////////////////
//	문 정보
////////////////////////////////////////////////////////////////////////////////////////////////
	if (l_iVersion	>=	eVUI_29_ADD_DOOR_LIST)
	{
		int		iDoorCount	=	0;

		file.Read(&iDoorCount,4);	//	문 갯수
		file.Seek(sizeof(cDoorObjectSimpleInfo)*iDoorCount,SEEK_CUR);	//	문 갯수
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	서버에서 쓰는 맵데이터 스킵~
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		file.Seek(lpMap->m_iRealWidth*lpMap->m_iRealHeight,SEEK_CUR);
	}	//	서버에서 쓰는 맵데이터 스킵~

////////////////////////////////////////////////////////////////////////////////////////////////
//	커스텀 아이템 로딩
////////////////////////////////////////////////////////////////////////////////////////////////

	if	(l_iVersion >= eVUI_19_ADD_CUSTOM_ITEM)
	{
		file.Read(&iCount,4);	//	스킵 데이터 위치
		CCustomItem	*lpItems	=	lpMap->m_customItem.getItemBay();

		if	(l_iVersion >= eVUI_23_ADD_CUSTOM_ITEM_NAME)
			file.Read(lpItems,dMAX_CUSTOM_ITEM_COUNT*sizeof(CCustomItem));
		else
		{
			for (i=0;i<dMAX_CUSTOM_ITEM_COUNT;i++)
			{
				strcpy(lpItems[i].m_strName,"no name");

				file.Read(&lpItems[i],sizeof(cITEM));
				file.Read(&lpItems[i].m_wIsAttachRandomPrefix,2);
				file.Read(&lpItems[i].m_wDropLevel,2);
				file.Read(&lpItems[i].m_wGenerateQuality,2);
				file.Read(&lpItems[i].m_aPrefixGenerateQuality,sizeof(lpItems[i].m_aPrefixGenerateQuality));
			}
		}

		lpMap->m_customItem.rebuild();
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	직업 데이터
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		cCHARACTER		saveCharacter;

		lpMap->m_setCharacter.reset();

		int		iDataSize	=	sizeof(CCharacterBaseInfo_old_before090717);

		file.Read(&lpMap->m_setCharacter.m_iCount			,4);	//	NPC+몬스터 직업의 수

		if	(l_iVersion	>=	eVVI_48_ENCRYT)
			cMessSign::DecodeDataBuffer2(&lpMap->m_setCharacter.m_iCount,4);

		if	(l_iVersion	>=	eVUI_18_ADD_CHARACTER_JOB_LIST)
		{
			file.Seek(2*lpMap->m_setCharacter.m_iCount,SEEK_CUR);	//	캐릭터의 직업 리스트
			file.Seek(4,SEEK_CUR);	//	스킵 포인트
		}

		if	(l_iVersion >=	eVVI_45_ADD_MORE_CHARACTER_DATA)
			file.Read(&iDataSize,4);	//	데이터 사이즈

		for (i=0;i<lpMap->m_setCharacter.m_iCount;i++)
		{
			saveCharacter.reset();

			if	(l_iVersion >=	eVVI_45_ADD_MORE_CHARACTER_DATA)
				file.Read(&saveCharacter,iDataSize);
			else
			{
				if	(l_iVersion >=	eVUI_27_ADD_CHARACTER_DATA)
					file.Read(&saveCharacter,sizeof(CCharacterBaseInfo_old_before090717));
				else
					file.Read(&saveCharacter,sizeof(CCharacterBaseInfo_old_before090717)-4);
				
				saveCharacter.m_bf1IsBlockToTame	=	FALSE;
			}

			if	(l_iVersion	>=	eVVI_48_ENCRYT)
				cMessSign::DecodeDataBuffer2(&saveCharacter,iDataSize);

			if	(l_iVersion	<	eVVI_46_REVISE_BLOCK_TO_TAME)
				saveCharacter.m_bf1IsBlockToTame	=	FALSE;

			if	(l_iVersion <	eVUI_34_ADD_CHARACTER_SKIN)
				saveCharacter.m_bf8Skin	=	0;

			if	(l_iVersion	<	eVUI_41_ADD_REVISE_INT)
				saveCharacter.m_bf1IsReviseIntelligence	=	FALSE;
			
			if	(l_iVersion	<	eVUI_40_ADD_DAMAGE_IMMUNE)
			{
				saveCharacter.m_bf1IsImmuneMagicDamage	=	FALSE;
				saveCharacter.m_bf1IsImmunePhysicalDamage=	FALSE;
			}

			if	(l_iVersion <	eVUI_35_ADD_BOSS_ZONE)
			{
				saveCharacter.m_wCorrectFinalDamage	=	10000;
				saveCharacter.m_bf1IsImmuneStun		=	FALSE;
				saveCharacter.m_bf1IsImmuneStone	=	FALSE;
				saveCharacter.m_bf1IsImmuneCharming	=	FALSE;
				saveCharacter.m_bf1IsImmuneFreeze	=	FALSE;
				saveCharacter.m_bf1IsImmuneKnockBack=	FALSE;
			}

			if	(l_iVersion < eVUI_38_ADD_CORRECT_CHARACTER_POWER)
			{
				saveCharacter.m_wCorrectExp	=	100;
				saveCharacter.m_wCorrectHP	=	100;
				saveCharacter.m_wCorrectAP	=	100;
			}

			if	(l_iVersion >= eVUI_16_ADD_CHARACTER_EVENT)
			{
				int	iSaveEventConditionCount;

				file.Read(&iSaveEventConditionCount,4);

				for (int j=0;j<iSaveEventConditionCount;j++)
				{
					int	iEvent;

					file.Read(&iEvent,4);

					saveCharacter.m_pEvent[iEvent]	=	new CKarmaContainer();

					ReadKarmaContainer(&file,saveCharacter.m_pEvent[iEvent],bIsDecodeKaramData);
				}
			}

			if (saveCharacter.m_wScaleX	<=	0 || saveCharacter.m_wScaleX >=	255)
				saveCharacter.m_wScaleX	=	100;
			if (saveCharacter.m_wScaleY	<=	0 || saveCharacter.m_wScaleY >= 255)
				saveCharacter.m_wScaleY	=	100;

			int	index	=	saveCharacter.m_wSerial;

			lpMap->m_setCharacter.m_aCharacter[index].copy(&saveCharacter);
		}

		lpMap->m_setCharacter.setSelectCharacter();
	}	//	직업 데이터

////////////////////////////////////////////////////////////////////////////////////////////////
//	필드에 몹/NPC
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		lpActorSet->reset();

		if (l_iVersion	>=	eVUI_04_SAVE_ACTOR_DATA_SKIP_POINT)
			file.Seek(4,SEEK_CUR);

		file.Read(&lpActorSet->m_iCount	,4);		//	액터의 수

		if	(l_iVersion	>=	eVVI_48_ENCRYT)
			cMessSign::DecodeDataBuffer2(&lpActorSet->m_iCount	,4);

		for (i=0;i<lpActorSet->m_iCount;i++)
		{
			cACTOR	temp,*lpActor;
			WORD	wConversationCount;

			if (l_iVersion	>=	eVUI_09_ADD_ACTOR_NAME)
				file.Read(&temp,sizeof(cACTOR_SAVE_DATA));	//	필드 캐릭터의 저장 데이터
			else
			{
				file.Read(&temp,sizeof(cACTOR_SAVE_DATA)-sizeof(temp.m_strName));	//	필드 캐릭터의 저장 데이터

				cCHARACTER	*lpCharacer	=	&lpMap->m_setCharacter.m_aCharacter[temp.m_wCharacter];

				RemoveQuotationMark(lpCharacer->m_strName);
				strcpy(temp.m_strName,lpCharacer->m_strName);
			}


			if	(l_iVersion	>=	eVVI_48_ENCRYT)
				cMessSign::DecodeDataBuffer2(&temp,sizeof(cACTOR_SAVE_DATA));

			if	(l_iVersion	<	eVVI_47_ADD_HIDE_NAME_BAR_OPTION)
				temp.m_bf1IsHideName	=	FALSE;

			lpActor	=	&lpActorSet->m_aActor[temp.m_iSerial];
			
			memcpy(lpActor,&temp,sizeof(cACTOR_SAVE_DATA));

			lpActor->m_wJob	=	lpMap->m_setCharacter.m_aCharacter[lpActor->m_wCharacter].m_wBaseJob;

			if	(l_iVersion	<	eVUI_37_ADD_BLOCK_TO_AUTO_REGEN)
				lpActor->m_wIsBlockToAutoRegen	=	FALSE;

			if	(l_iVersion	<	eVUI_03_ACTOR_SPEECH_DATA)
				continue;

			file.Read(&wConversationCount,2);

			if	(wConversationCount	==	0)
				continue;

			lpActor->m_pCC	=	new CConversationContainer();

			ReadCC(&file,lpActor->m_pCC,wConversationCount,bIsDecodeKaramData);
		}
	}	//	캐릭터 불러오기




////////////////////////////////////////////////////////////////////////////////////////////////
//	영역 불러오기
////////////////////////////////////////////////////////////////////////////////////////////////

	{
		lpArea->Reset();

		file.Read(&lpArea->m_iCount,4);										//	영역의 수

		if	(l_iVersion	>=	eVVI_48_ENCRYT)
			cMessSign::DecodeDataBuffer2(&lpArea->m_iCount,4);

		for (i=0;i<lpArea->m_iCount;i++)
		{
			cAREA_INFO				Area;
			int						iLocate;
			CConversationContainer	cc;

			if	(l_iVersion	>=	eVUI_28_EXPAND_AREA_DATA)
			{
				if	(l_iVersion	>=	eVVI_43_ADD_SEASON_VARIABLE)
					file.Read(&Area,sizeof(cAREA_SAVE_DATA));		//
				else
				{
					file.Read(&Area,sizeof(cAREA_SAVE_DATA) - 2);		//
					Area.m_wSeasonVariable = 0;
				}

				if	(l_iVersion	>=	eVVI_48_ENCRYT)
					cMessSign::DecodeDataBuffer2(&Area,sizeof(cAREA_SAVE_DATA));

				file.Read(&iLocate,4);				//	텍스트의 위치

				if (l_iVersion	<	eVUI_32_ADD_AREA_CC)
					file.Seek(iLocate,SEEK_SET);				//	텍스트의 위치
				else
				{
					WORD	wConversationCount;

					file.Read(&wConversationCount,2);

					if	(wConversationCount)
						ReadCC(&file,&cc,wConversationCount,bIsDecodeKaramData);
				}
			}
			else
			if	(l_iVersion	>=	eVUI_11_ADD_MOVE_GATE_NAME)
				file.Read(&Area,sizeof(cAREA_OLD_SAVE_DATA));		//
			else
			{
				file.Read(&Area,sizeof(cAREA_OLD_SAVE_DATA)-dSIZEOF_AREA_NAME);		//
				Area.m_strMoveGateName[0]	=	NULL;
			}

			file.Read(&iLocate,4);				//	텍스트의 위치

			if	(iLocate	!=	0xffffffff)
			{
				iLocate	+=	iTexFiletLocate;

				int	size;
				int	iCurLocate	=	file.Locate();	//	현재 위치

				file.Seek(iLocate,SEEK_SET);		//	문장의 위치로 이동
				file.Read(&size,4);					//	사이즈

				if (size	 > iBufferSize)
				{
					iBufferSize	=	size;

					pKILL(pBuffer);

					pBuffer		=	new char [iBufferSize];
				}

				file.Read(pBuffer,size);
				file.Seek(iCurLocate,SEEK_SET);

				Area.setString(pBuffer);
			}

			RemoveQuotationMark(Area.m_strName);

			lpArea->Add(&Area,&cc);
		}

		lpArea->SetNewArea();
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	상점 정보(새로운 위치)
////////////////////////////////////////////////////////////////////////////////////////////////
	if	(l_iVersion>=eVUI_05_CHANGE_SHOP_DATA_SAVE_LOCATE)
	{
		WORD	wShopCount;

		if	(l_iVersion	>=	eVUI_06_SAVE_SHOP_DATA_SKIP_POINT)
			file.Seek(4,SEEK_CUR);	//	스킵 정보 스킵

		file.Read(&wShopCount,2);	//	상점의 수

		for (i=0;i<wShopCount;i++)
		{
			CShop			shopTemp;
			WORD			wItemCount;
			cShopNameInfo	sInfo;

			file.Read(&sInfo,sizeof(sInfo));	//	상점 이름

			if	(l_iVersion	<	eVUI_42_ADD_DUEL_SERVER)
			{
				sInfo.m_bf1IsDuelPointShop		=	FALSE;
				sInfo.m_bf1IsLevelPerGoldShop	=	FALSE;
				sInfo.m_bf15LevelPerGoldValue	=	FALSE;
				sInfo.m_bf1IsNotBuyItem			=	FALSE;
			}	

			strcpy(shopTemp.m_strName,sInfo.m_strName);	//	상점 이름
			shopTemp.m_bf1IsDuelPointShop	=	sInfo.m_bf1IsDuelPointShop;
			shopTemp.m_bf1IsLevelPerGoldShop		=	sInfo.m_bf1IsLevelPerGoldShop;
			shopTemp.m_bf15LevelPerGoldValue		=	sInfo.m_bf15LevelPerGoldValue;
			shopTemp.m_bf1IsNotBuyItem				=	sInfo.m_bf1IsNotBuyItem;	// 아이템 구입 불가.

			RemoveQuotationMark(shopTemp.m_strName);
			file.Read(&shopTemp.m_dwLimitPrice,4);	//	구입 최고 가격

			if	(shopTemp.m_dwLimitPrice	<	5000 || shopTemp.m_dwLimitPrice	> 1000000)
				shopTemp.m_dwLimitPrice	=	10000;

			file.Read(&shopTemp.m_wSerial,2);	//	상점 시리얼

			if	(l_iVersion>=eVUI_07_SAVE_SHOP_PRICE_FACTOR)	
				file.Read(&shopTemp.m_wPriceFactor,2);	//	가격 인자

			if	(l_iVersion>=eVVI_44_ADD_TOKEN_SHOP)
				file.Read(&shopTemp.m_TokkenShop,2);	//	토큰 샵

			file.Read(&wItemCount,2);	//	판매 아이템의 수

			for (j=0;j<wItemCount;j++)
			{
				CShopItem	shopitemTemp;

				file.Read(&shopitemTemp,sizeof(CShopItem));

				if	(l_iVersion	<	eVUI_42_ADD_DUEL_SERVER)
				{
					shopitemTemp.m_bf10ChangeLimitLevel	=	0;
				}

				cBASIC_ITEM	*lpBasicItem	=	((cITEM *)&shopitemTemp)->getBasicItem();

				if( lpBasicItem->m_wStackLimit > 1)
				{
					if (shopitemTemp.m_aOption[0] == 0xff) shopitemTemp.m_aOption[0]	=	0;
					if (shopitemTemp.m_aOption[1] == 0xff) shopitemTemp.m_aOption[1]	=	0;

					for (int iPrefix=0;iPrefix<dITEM_PREFIX_COUNT;iPrefix++)
						if (shopitemTemp.m_aPrefix[iPrefix].m_wPrefix == 0xffff)
						{
							shopitemTemp.m_aPrefix[iPrefix].m_aValue[0]=	0xff;
							shopitemTemp.m_aPrefix[iPrefix].m_aValue[1]=	0xff;
						}
				}

				shopTemp.addItem(&shopitemTemp);
			}

			lpMap->m_shop.insertShop(&shopTemp);
		}
	}


////////////////////////////////////////////////////////////////////////////////////////////////
//	오브젝트 불러오기
////////////////////////////////////////////////////////////////////////////////////////////////
	MSGOUT("Object Loading - 1");
	{
		int	iObjectCount;
		file.Read(&iObjectCount ,4	);	//	오브젝트 수
		file.Seek(8*100*3,SEEK_CUR);	//	게임중에서 빠른 정렬을 위해 쓰는 데이터


		for (i=0;i<iObjectCount;i++)
		{
			cTINY_OBJECT_INFO *lpInfo	=	lpObject->m_denTinyObject.getRookie();

			file.Read(lpInfo,sizeof(cSAVE_TINY_OBJECT_INFO));		//	 x+y+object
			lpInfo->m_wSerial			=	lpObject->m_denTinyObject.getRookieSerial();

			if (lpInfo->m_wObject		>=	lpMap->m_object.m_iTinyObjectImageCount)
			{
				file.Close();

				return	ERRMSG("Error!!", "A floor object that is not in the mapset has been recorded.\n\nIndex %d", lpInfo->m_wObject);
			}

			lpObject->m_denTinyObject.increaseCount();
		}
	}	//	오브젝트 불러오기

	MSGOUT("Object Loading - 2");
	{
		int	iObjectCount;
		file.Read(&iObjectCount ,4	);	//	오브젝트 수

		if(iObjectCount	>=	dMAX_OBJECT_COUNT)
			cMSG::Put("What do you think? The number of objects is no joke. -o-", "%s", fn);

		for (i=0;i<iObjectCount;i++)
		{
			cFIXED_OBJECT_INFO	object;

			if (l_iVersion >= eVUI_15_ADD_OBJECT_SHADOW_CHECK)
				file.Read(&object,sizeof(cSAVE_FIXED_OBJECT_INFO));		//	 x+y+object
			else
			{
				cSAVE_FIXED_OBJECT_INFO_OLD_031009	objectOld;

				file.Read(&objectOld,sizeof(objectOld));		//	 x+y+object

				memcpy(&object,&objectOld,sizeof(cSAVE_FIXED_OBJECT_INFO));

				object.m_wIsPutShadow	=	TRUE;
			}

			if (object.m_wSerial	>=	lpObject->m_denFixedObject.getMaxCount())
				continue;

			if (object.m_wObject	>=	lpMap->m_object.m_iFixedObjectImageCount)
			{
				file.Close();

				return	cMSG::Put("Error!!", "A fixed object that is not in the mapset has been recorded.\n\nIndex %d", object.m_wObject);
			}

			lpObject->m_denFixedObject.insert(object.m_wSerial,&object);
		}
	}	//	오브젝트 불러오기

	MSGOUT("Loading Building");
////////////////////////////////////////////////////////////////////////////////////////////////
//	건물
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		int	iCount;

		file.Read(&iCount		,4		);					//	건물 수

		for (i=0;i<iCount;i++)
		{
			cBUILDING_INFO		saveBuilding;

			saveBuilding.reset();

			file.Read(&saveBuilding,sizeof(cSAVE_BUILDING_INFO));	//	건물 정보

			if (saveBuilding.m_wBuilding	>=	lpMap->m_building.m_iDataCount)
			{
				file.Close();

				return	ERRMSG("Error!!","There is a building in the map set that is not in the map set.\n\nIndex %d",saveBuilding.m_wBuilding);
			}

			for (int iIndex	= 0;iIndex<saveBuilding.m_wAddonBlockCount;iIndex++)
			{
				if(saveBuilding.m_aAddonBlock[iIndex]>=lpMap->m_building.m_pData[saveBuilding.m_wBuilding].Count)
				{
					saveBuilding.m_wAddonBlockCount	=	0;
					continue;
				}
			}

			lpMap->m_building.m_den.insert(saveBuilding.m_wSerial,&saveBuilding);
		}
	}

	MSGOUT("Store - 2");

////////////////////////////////////////////////////////////////////////////////////////////////
//	상점 정보(옛날 위치)
////////////////////////////////////////////////////////////////////////////////////////////////
	if (l_iVersion>=eVUI_02_SHOP_INFO	&&	l_iVersion<eVUI_05_CHANGE_SHOP_DATA_SAVE_LOCATE)
	{
		WORD	wShopCount;

		file.Read(&wShopCount,2);	//	상점의 수

		for (i=0;i<wShopCount;i++)
		{
			CShop	shopTemp;
			WORD	wItemCount;
			cShopNameInfo	sInfo;

			file.Read(&sInfo,sizeof(sInfo));	//	상점 이름

			strcpy(shopTemp.m_strName,sInfo.m_strName);
			shopTemp.m_bf1IsDuelPointShop		=	FALSE;
			shopTemp.m_bf1IsLevelPerGoldShop	=	FALSE;
			shopTemp.m_bf15LevelPerGoldValue	=	0;
			shopTemp.m_bf1IsNotBuyItem			=	FALSE;

			RemoveQuotationMark(shopTemp.m_strName);
			file.Read(&shopTemp.m_wSerial,2);	//	상점 시리얼
			file.Read(&wItemCount,2);	//	판매 아이템의 수

			for (j=0;j<wItemCount;j++)
			{
				CShopItem	shopitemTemp;

				file.Read(&shopitemTemp,sizeof(CShopItem));

				shopTemp.insertItem(&shopitemTemp);
			}

			lpMap->m_shop.insertShop(&shopTemp);
		}
	}

	MSGOUT("Other");

////////////////////////////////////////////////////////////////////////////////////////////////
//	기타 정보
////////////////////////////////////////////////////////////////////////////////////////////////
	if (l_iVersion	>=	eVUI_12_ADD_CURRENT_MAP_POS)
		file.Read(&lpMap->m_pos,sizeof(cPOINT));	//	현재 위치

	lpMap->setPos(lpMap->m_pos.x,lpMap->m_pos.y);

////////////////////////////////////////////////////////////////////////////////////////////////
//	카르마
////////////////////////////////////////////////////////////////////////////////////////////////
	MSGOUT("Karma");
	if	(l_iVersion	>=	eVUI_25_ADD_KARMA)
		ReadKarmaContainer(&file,&lpMap->m_karma,bIsDecodeKaramData);

	if	(l_iVersion	>=	eVUI_26_ADD_DUNGEON_VALUE_AND_KARMA_VALUE && l_iVersion	<	eVUI_33_REMOVE_DUNGEON_VALUE)
		file.Seek(dMAX_DUNGEON_VALUE_COUNT*dDUNGEON_VALUE_TEXT_LENGTH,SEEK_CUR);	//	현재 위치

	MSGOUT("character pattern");
////////////////////////////////////////////////////////////////////////////////////////////////
//	캐릭터 패턴
////////////////////////////////////////////////////////////////////////////////////////////////
	if	(l_iVersion	>=	eVUI_36_ADD_CHARACTER_PATTERN)
	{
		lpMap->m_cpp.clear();

		file.Read(&i,4);

		WORD	wCount;
		file.Read(&wCount,2);

		for	(i=0;i<wCount;i++)
		{
			cCharacterPattern	pattern;

			file.Read(&pattern,4);

			int	iReadBufferSize	=	sizeof(pattern)-sizeof(pattern.m_aPattern)-4+sizeof(cCharacterPatternPiece)*pattern.m_wCount;

			file.Read(pattern.m_strName,iReadBufferSize);

			lpMap->m_cpp.add(&pattern,pattern.m_wSerial);
		}
	}

	if	(l_iVersion	>=	eVUI_39_ADD_FIELD_VALUE)
	{
		int	iWriteSkipSizeLocate;

		file.Read(&iWriteSkipSizeLocate,4);
		file.Read(lpMap->m_astrFieldValue,sizeof(lpMap->m_astrFieldValue));
	}
	else
	{
		for (int i=0;i<50;i++)
			sprintf(lpMap->m_astrFieldValue[i],"field value #%.2d",i);
	}

	MSGOUT("Finish");
// 마무리
	{
		if (!lpMapDataLoad)
			ApplyOfficialJapaneseNames(fn,lpMap);

		pKILL(pBuffer);				//	텍스트 데이터를 읽어들일 버퍼 해제

		file.Close();
		if(!lpMapDataLoad)
		{
			//		strcpy(lpMap->m_strFName,fn);
			strcpy(lpMap->m_strFName,cPROJECT::ExportMapFileName(fn));
			
			lpMap->m_iSerial		=	index;
			
			ustrncpy(name,lpMap->m_strName,20);
			
			MSGOUT("Remove quotes");
			RemoveQuotationMark(name);
			
			if	(!g_bmMapList.Set(index,name))
			{
				int x	=	g_bmMapList.GetRightPos();
				if(index == 0)
					x += 100;
				char	name[32];
				
				ustrncpy(name,lpMap->m_strName,20);
				RemoveQuotationMark(name);
				g_bmMapList.Add(name,index,x+3,4);
			}
			
			MSGOUT("Select map");
			
			cMAP::Select(index);
			
			lpMap->RemarkCrashMap();
			
			lpMap->SetVirgin();
		}
	}

	MSGOUT("Map loading complete");
	lpMap->m_bIsLockedBySVN	=	FALSE;
	// 10.01.12
	if(!lpMapDataLoad)
		lpMap->LoadMemoFile();
	return TRUE;
}

void
cMAP::LoadMemoFile()
{
	cMemoWindow::Reset(&m_sMemoInfo);

	if	(!cPROJECT::SetMapFolder() || m_strFName[0] == NULL)
		return;
	// 10.01.18 추가
	CreateDirectory(".\\Memo", NULL);	
	strcpy(m_sMemoInfo.tmfile,  cMemoWindow::ConversionFilename(m_strFName, ".tm"));
	strcpy(m_sMemoInfo.tmofile, cMemoWindow::ConversionFilename(m_strFName, ".tmo"));
	
	FILE *fp = fopen(m_sMemoInfo.tmofile, "r");
	
	if(fp != NULL)
	{
		fscanf(fp, "%d", &m_sMemoInfo.pos.x);
		fscanf(fp, "%d", &m_sMemoInfo.pos.y);
		fscanf(fp, "%d", &m_sMemoInfo.size.x);
		fscanf(fp, "%d", &m_sMemoInfo.size.y);

		fclose(fp);
	}
	
	HANDLE hFile;
	DWORD dwRead;
	DWORD size;
		
	hFile=CreateFile(m_sMemoInfo.tmfile,GENERIC_READ | GENERIC_WRITE,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile==INVALID_HANDLE_VALUE) {
		return;
	}

	size=GetFileSize(hFile,NULL);
	m_sMemoInfo.text =(TCHAR *)calloc(size+1,1);
	ReadFile(hFile,m_sMemoInfo.text,size,&dwRead,NULL);
	CloseHandle(hFile);
}

void
cMAP::SaveMemoFile()
{
	if	(!cPROJECT::SetMapFolder())
		return;

	memset(m_sMemoInfo.tmfile, 0, sizeof(m_sMemoInfo.tmfile));
	memset(m_sMemoInfo.tmofile, 0, sizeof(m_sMemoInfo.tmofile));

	strcpy(m_sMemoInfo.tmofile, cMemoWindow::ConversionFilename(m_strFName, ".tmo"));
	strcpy(m_sMemoInfo.tmfile,  cMemoWindow::ConversionFilename(m_strFName, ".tm"));
	
	FILE *fp = fopen(m_sMemoInfo.tmofile, "w");
	
	if(fp != NULL)
	{
		fprintf(fp, "%d ", m_sMemoInfo.pos.x);
		fprintf(fp, "%d ", m_sMemoInfo.pos.y);
		fprintf(fp, "%d ", m_sMemoInfo.size.x);
		fprintf(fp, "%d ", m_sMemoInfo.size.y);

		fclose(fp);
	}
	
	HANDLE hFile;
	DWORD dwWritten;
	
	hFile=CreateFile(m_sMemoInfo.tmfile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if (hFile==INVALID_HANDLE_VALUE) {
		return;
	}
	
	if( m_sMemoInfo.text != NULL )
		WriteFile(hFile,m_sMemoInfo.text,strlen(m_sMemoInfo.text),&dwWritten,NULL);
	
	CloseHandle(hFile);
}
//
//	fn 이라는 이름의 맵이 열려 있는지 확인한다.
BOOL
IsOpenMap(char *fn,int index)
{
	for (int i=0;i<dMAX_MAP;i++)
		if (stricmp(g_aMap[i].m_strFName,fn)	==	0)
		{
			if (i	!=	index)	return	TRUE;
		}

	return	FALSE;
}


void
cMAP::updateAddonObject()
{
	m_lpEditAddonObject			=	getEditAddonObject();
	m_lpEditAddonObjectBuilding	=	getEditAddonObjectBuilding();
}

//
//	포탈 영역 리스트를 얻어 간다. -_-?
BYTE*
cMAP::LoadBlock(char *_lpstrMapname,cPOINT *_lpSize)
{
	cMAP_INFO	map;
	cFILE		file;
	int			i=0,j=0,iCount=0,l_iVersion=0;
	char		header[70];

	cPROJECT::SetMapFolder();

	if (!file.Open(_lpstrMapname,"rb"))
	{
		SetLogFolder();
		cMSG::Put("Error in cMAP::GetPortalAreas","'%s' file not found!!",_lpstrMapname);

		return	NULL;
	}

	file.Read(&i,4);			//	화일 사이즈
	file.Seek(4,SEEK_CUR);		//	텍스트 데이터의 위치
	file.Read(header,60);		//	헤더

	l_iVersion	=	GetVersion(header);

	if (l_iVersion	>=	eVUI_30_ADD_CORECT_MONSTER_RESISTANCE)
		file.Read(&map,sizeof(cMAP_INFO));				//	맵 정보
	else
	{
		memset(&map,0,sizeof(map));
		file.Read(&map,sizeof(cMapBaseInfo20050527));				//	맵 정보
	}

	int	iRealWidth	=	map.m_iWidth	*dSHARE_TILE;
	int	iRealHeight	=	map.m_iHeight	*dSHARE_TILE;

	file.Seek(iRealWidth*iRealHeight*2,SEEK_CUR);	//	타일맵정보
	file.Seek(map.m_iWidth*map.m_iHeight*2,SEEK_CUR);			//	높이 정보(툴에서만 쓴다)
	file.Seek(iRealWidth*iRealHeight*2,SEEK_CUR);	//	건물,오브젝트,블럭

	BYTE	*pBuffer=	new	BYTE [iRealWidth*iRealHeight];

	file.Read(pBuffer,iRealWidth*iRealHeight);

	file.Close();

	_lpSize->x	=	iRealWidth;
	_lpSize->y	=	iRealHeight;

	return	pBuffer;
}	//	cMAP::GetPortalareaInfos(char *_lpstrMapname,CPortalareaInfoInfo *_lpPortalareaInfos,int *_iCount)

//
//	포탈 영역 리스트를 얻어 간다. -_-?
cAREA*
cMAP::GetArea(char *_lpstrMapname)
{
	static	cAREA	areaList;

	cAREA_INFO		*lpSelectArea	=	g_lpSelectArea;

	cMessSigeDoumi	doumi;
	
	areaList.Reset();

	g_lpSelectArea			=	lpSelectArea;

	cMAP_INFO	map;
	cFILE		file;
	int			i=0,j=0,iCount=0,l_iVersion=0,iTexFiletLocate,iBufferSize=1024;
	char		header[70],*pBuffer;

	cPROJECT::SetMapFolder();

	if (!file.Open(_lpstrMapname,"rb"))
	{
		SetLogFolder();

		cMSG::Put("Error in cMAP::GetArea","'%s' file not found!!",_lpstrMapname);

		return	NULL;
	}

	file.Read(&i,4);			//	화일 사이즈
	file.Read(&iTexFiletLocate,4);					//	텍스트 데이터의 위치
	file.Read(header,60);		//	헤더

	pBuffer	=	new char [iBufferSize];				//	텍스트 데이터를 읽어들일 버퍼

	l_iVersion	=	GetVersion(header);

	if (l_iVersion	<	eVUI_10_ADD_BGM)	
		file.Read(&map,sizeof(CMapBaseInfo));				//	맵 정보
	else
	if (l_iVersion	<	eVUI_21_ADD_BGM_LIST)
	{
		int	iBgm;
		file.Read(&map,sizeof(CMapBaseInfo));				//	맵 정보
		file.Read(&iBgm,4);				//	맵 정보
	}
	else
	if (l_iVersion	<	eVUI_24_ADD_CORRECT_MAGIC_RESISTANCE)
		file.Read(&map,sizeof(CMapBaseInfo20031111));				//	맵 정보
	else
	if (l_iVersion	<	eVUI_30_ADD_CORECT_MONSTER_RESISTANCE)
	{
		memset(&map,0,sizeof(map));
		file.Read(&map,sizeof(cMapBaseInfo20050527));				//	맵 정보
	}
	else
		file.Read(&map,sizeof(cMAP_INFO));				//	맵 정보

	int		iLocalizeNation	= eNationJapan;

	if	(l_iVersion	>=	eVVI_51_ENCRYT2)
	{
		file.Read(&iLocalizeNation,sizeof(g_iLocalizeNation));
		cMessSign::SetDataEncodeTable(iLocalizeNation);
	}
	else
		cMessSign::SetDataEncodeTable(-1);

	if (l_iVersion	>=	eVUI_17_ADD_AREA_SAVE_POINT)
	{
		DWORD	dwAreaDataSavePoint;
		
		file.Read(&dwAreaDataSavePoint,4);	//	영역 데이터 위치
		file.Seek(dwAreaDataSavePoint,SEEK_SET);	//	타일맵정보
	}
	else
	{
		int	iRealWidth	=	map.m_iWidth	*dSHARE_TILE;
		int	iRealHeight	=	map.m_iHeight	*dSHARE_TILE;

		file.Seek(iRealWidth*iRealHeight*2,SEEK_CUR);	//	타일맵정보
		file.Seek(map.m_iWidth*map.m_iHeight*2,SEEK_CUR);			//	높이 정보(툴에서만 쓴다)
		file.Seek(iRealWidth*iRealHeight*2,SEEK_CUR);	//	건물,오브젝트,블럭
		file.Seek(iRealWidth*iRealHeight,SEEK_CUR);

		file.Read(&iCount,4);	//	NPC+몬스터 직업의 수

		for (i=0;i<iCount;i++)
		{
			int	iSaveEventCount;

			file.Seek(sizeof(CCharacterBaseInfo),SEEK_CUR);

			if (l_iVersion >= eVUI_16_ADD_CHARACTER_EVENT)	file.Read(&iSaveEventCount,4);
		}

		int	iSkipPoint;
		file.Read(&iSkipPoint,4);
		file.Seek(iSkipPoint,SEEK_SET);		//	액터의 수
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	영역 불러오기
////////////////////////////////////////////////////////////////////////////////////////////////
	
	file.Read(&areaList.m_iCount,4);										//	영역의 수
		int	iAreaListCount	=	areaList.m_iCount;
	if	(l_iVersion	>=	eVVI_48_ENCRYT)
		cMessSign::DecodeDataBuffer2(&iAreaListCount,4);

	for (i=0;i<iAreaListCount;i++)
	{
		cAREA_INFO	Area;
		int			iLocate;

		if (l_iVersion	>=	eVUI_28_EXPAND_AREA_DATA)
		{
			if(l_iVersion	>=	eVVI_43_ADD_SEASON_VARIABLE)
				file.Read(&Area,sizeof(cAREA_SAVE_DATA));		//
			else
			{
				file.Read(&Area,sizeof(cAREA_SAVE_DATA) - 2);		//
				Area.m_wSeasonVariable = 0;
			}

			file.Read(&iLocate,4);				//	텍스트의 위치
			file.Seek(iLocate,SEEK_SET);				//	텍스트의 위치
		}
		else
		if (l_iVersion	>=	eVUI_11_ADD_MOVE_GATE_NAME)
			file.Read(&Area,sizeof(cAREA_OLD_SAVE_DATA));		//
		else
		{
			file.Read(&Area,sizeof(cAREA_OLD_SAVE_DATA)-dSIZEOF_AREA_NAME);		//
			Area.m_strMoveGateName[0]	=	NULL;
		}
		if	(l_iVersion	>=	eVVI_48_ENCRYT)
			cMessSign::DecodeDataBuffer2(&Area,sizeof(cAREA_SAVE_DATA));
		file.Read(&iLocate,4);				//	텍스트의 위치

		if(iLocate	!=	0xffffffff)
		{
			iLocate	+=	iTexFiletLocate;

			int	size;
			int	iCurLocate	=	file.Locate();	//	현재 위치

			file.Seek(iLocate,SEEK_SET);		//	문장의 위치로 이동
			file.Read(&size,4);					//	사이즈

			if (size	 > iBufferSize)
			{
				iBufferSize	=	size;

				pKILL(pBuffer);

				pBuffer		=	new char [iBufferSize];
			}

			file.Read(pBuffer,size);
			file.Seek(iCurLocate,SEEK_SET);

			Area.setString(pBuffer);
		}

		areaList.Add(&Area,NULL);
	}

	areaList.SetNewArea();

	file.Close();

	SetLogFolder();

	return	&areaList;
}

//
//	포탈 영역 리스트를 얻어 간다. -_-?
BOOL
cMAP::GetPortalAreas(char *_lpstrMapname,CPortalAreaInfo *_lpPortalAreas,int *_iCount)
{
	cAREA	*lpArea	=	GetArea(_lpstrMapname);

	if (!lpArea)	return	FALSE;

	int		iCount	=	0;

	for (int i=0;i<dMAX_AREA;i++)
	{
		cAREA_INFO	*lpAreaInfo	=	&lpArea->m_aArea[i];

		if (lpAreaInfo->m_wSerial	==	0xffff)	continue;

		if (lpAreaInfo->isPortal())	
			strcpy(_lpPortalAreas[iCount++].m_strName,lpAreaInfo->m_strName);
	}

	*_iCount	=	iCount;

	return	TRUE;
}	//	cMAP::GetPortalareaInfos(char *_lpstrMapname,CPortalareaInfoInfo *_lpPortalareaInfos,int *_iCount)

//
//	포탈 영역을 리턴~
int
cMAP::GetPortalArea(char *_lpstrMapname,char *_lpstrAreaName)
{
	cAREA	*lpArea	=	GetArea(_lpstrMapname);

	if (!lpArea)	return	FALSE;

	for (int i=0;i<dMAX_AREA;i++)
	{
		cAREA_INFO	*lpAreaInfo	=	&lpArea->m_aArea[i];

		if (lpAreaInfo->m_wSerial	==	0xffff)	continue;

		if (lpAreaInfo->isPortal())
			if (stricmp(lpAreaInfo->m_strName,_lpstrAreaName)==0)	
				return	lpAreaInfo->m_wSerial;
	}

	return	0xffff;
}

BOOL
cMAP::CheckPortalArea(char *_lpstrMapname,char *_lpstrSrcAreaName,char *_lpstrAreaName)
{
	cAREA	*lpArea	=	GetArea(_lpstrMapname);

	if (!lpArea)	return	FALSE;

	for (int i=0;i<dMAX_AREA;i++)
	{
		cAREA_INFO	*lpAreaInfo	=	&lpArea->m_aArea[i];

		if (lpAreaInfo->m_wSerial	==	0xffff)	continue;


		if (lpAreaInfo->isPortal())
		{
			if (stricmp(lpAreaInfo->m_strName,_lpstrAreaName)==0)
			{
				if (stricmp(lpAreaInfo->m_strMoveGateName,_lpstrSrcAreaName)==0)	return	TRUE;

				return	FALSE;
			}
		}
	}

	return	FALSE;
}	//	cMAP::CheckPortalArea(char *_lpstrMapname,char *_lpstrSrcAreaName,char *_lpstrAreaName)

void
cMAP::LostVirgin()
{
	if	(getLock()	==	FALSE)
		cMSG::Put("Warning!!","Locking failed.\n\nThe contents just edited cannot be saved. Please lock first.");

	m_isVIRGIN	=	FALSE;
}

BOOL
cMAP::getLock(BOOL _bIsByCommand)
{
	if	(m_bIsLockedBySVN)
		return	TRUE;

	m_bIsLockedBySVN	=	LockSVN(m_strFName,_bIsByCommand);

	return	m_bIsLockedBySVN;
}

BOOL
cMAP::releaseLock(BOOL _bIsByCommand)
{
	if	(!UnlockSVN(m_strFName,_bIsByCommand))
		return	FALSE;

	m_bIsLockedBySVN	=	FALSE;

	return	TRUE;
}

BOOL
cMAP::updateSVN(BOOL _bIsByCommand)
{
	return	UpdateFileSVN(m_strFName,_bIsByCommand);
}

BOOL
cMAP::commitSVN(BOOL _bIsByCommand)
{
	if	(IsVirgin()	==	FALSE)
		if (cMSG::YESNO("Send to server", "There is unsaved content in the file you are editing.\n\nDo you want to save it and send it to the server?") == IDYES)
			cPROJECT::SaveMap();

	if	(CommitFileSVN(m_strFName,_bIsByCommand)	==	TRUE)
	{
		cMSG::Put("Transfer to current file server","Successful.");

		return	TRUE;
	}
	else
		cMSG::Error("Transfer to current file server", "Failed.");

	return	FALSE;
}
