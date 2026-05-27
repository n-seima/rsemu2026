#include "cMAP.H"
#include "CActor.H"
#include "ChitEffect.H"
#include "effectManager.H"
#include "cJOB.H"
#include "cAREA.H"
#include "cKARMA.H"
#include "cKARMA_VALUE.H"
#include "CFieldItem.H"
#include "CGuardianPost.H"
#include "CPitchmanShop.H"
#include "CPortal.H"
#include "trap.H"
#include "cMessage.H"
#include "arca.H"
#include "banner.H"
#include "field_common.h"
#include "mess_sign.h"

BOOL				g_bIsGuildBattleField	=	FALSE;
BOOL				g_bIsHalfSize	=	FALSE;
int					g_iGameScale	=	100;

cSTRINGS			*cMAP::s_lpstrMapList;

cRECT				g_rectViewPort;			//	현재 화면에 보이는 영역
cINDEX				l_aSortObject[2048];
cMAP				g_map;
CMapSimpleInfo		g_aMapSimpleInfo[dMAX_MAP_COUNT];
CMapLocationInfo	g_aMapLocationInfo[dMAX_MAP_COUNT];

enum
{			// 마지막 수정일 : 09.10.08 
	eWEATHER_FINE,		// 맑음 +_+
	eWEATHER_SNOW,		// 눈 +_+
	eWEATHER_RAIN,		// 비...
};

cMAP::cMAP()
{	// 마지막 수정일 : 09.10.19
	m_iSerial			=	0xffff;		//	프로젝트에서 관리할 시리얼... 아직은 없다.
	m_iLoadedTileSet	=	-1;
	m_bIsDrawWeather	=	FALSE;
}

cMAP::~cMAP()
{
	close();
}

void
cMAP::close()
{
	m_object.close();
	m_tile.close();
	m_iLoadedTileSet	=	-1;
}

void
cMAP::reset()
{
	m_doors.reset();
// insu add
	m_area.reset();
	for(int i=0 ; i<c_iMaxFieldActorCount ; i++)
	{
		memset(&m_aSaveActor[i], 0, sizeof(cBASE_NPC_INFO));
		m_aSaveActor[i].m_iSerial	=	-1;
	}
	m_iSavedActorCount = 0;
// insu add end

}

//
//	필드에 사용되는 이미지 데이터 로딩
BOOL
cMAP::loadFieldImageData(BOOL _bIsHalfSize,cNUX *_lpNux,cMCU *_lpMcu)
{
	g_iGameScale	=	100;

	BOOL	bIsSameScale	=	FALSE;

	if	(g_bIsHalfSize	==	m_bf1IsHalfSize)
		bIsSameScale	=	TRUE;

	g_bIsHalfSize	=	m_bf1IsHalfSize;
	
	if	(g_hero.m_wOperatorLevel	>=	4 && g_bIsHalfSizeObserver)
		g_bIsHalfSize	=	TRUE;

#ifdef	_IS_DEV_CLIENT
//	g_bIsHalfSize	=	TRUE;
#endif

	if	(g_bIsHalfSize)
		g_iGameScale/=	2;

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("load minimap");

//	미니맵 로딩
	{
		g_project.setMapFolder();

		m_tgaMinimap.close();
		if (!m_tgaMinimap.Load(_ms("%s.tga",m_strFileName),TRUE))
			g_eh.addStaticLog("minimap load fail [%s.tga]",m_strFileName);

		SetLogFolder();
	}

//	if	(m_iLoadedTileSet==	m_iTileSet && !m_bIsRoom	&&	bIsSameScale	==	TRUE)
//		return	TRUE;

	m_iLoadedTileSet	=	m_iTileSet;

	_lpMcu->reset();	//	초기화

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("load tile");

	if	(!m_tile.load(m_iTileSet	,g_bIsHalfSize,_lpNux,_lpMcu))
	{
		g_eh.addStaticLog("tile image failed!!");

		return	FALSE;	//	타일     데이터 로드
	}

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("load object");

	if	(!m_object.loadImageData(m_iTileSet	,g_iGameScale,m_bIsRoom,_lpNux,_lpMcu))
	{
		g_eh.addStaticLog("object image failed!!");

		return	FALSE;	//	오브젝트 데이터 로드
	}

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("build info");

	buildInfo();

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("complete loadFieldImageData");

	return	TRUE;
}

//
//	_strHeader을 기준으로 이 맵 화일의 버젼을 얻어온다.
int
cMAP::getMapDataVersion(char *_strHeader)
{
	m_iFileVersion	=	GetMapDataVersion(_strHeader);

	return	m_iFileVersion;
}

//
//	카르마 읽기
void
cMAP::skipKarmaItem(cFILE *_lpFile,BOOL _bIsDecodeKaramData)
{
	class	CKarmaItemBasicInfo
	{
	public:
		WORD		m_wItem;
		int			m_aValue[6];
		WORD		m_wStringSize	:	15;
		WORD		m_bf1IsAbsolute	:	1;
	};

	CKarmaItemBasicInfo	karmaItem;

	_lpFile->Read(&karmaItem,sizeof(karmaItem));

	if	(_bIsDecodeKaramData)
		cMessSign::DecodeDataBuffer2(&karmaItem,sizeof(CKarmaItemBasicInfo));

	if	(karmaItem.m_wStringSize	>	0)
		_lpFile->Skip(karmaItem.m_wStringSize);
}

//
//	카르마 읽기
void
cMAP::skipKarma(cFILE *_lpFile,BOOL _bIsDecodeKaramData)
{
	class CKarmaBasicInfo
	{
	public:
		WORD				m_wSerial;			//	카르마 시리얼
		WORD				m_isWantAndCondition		: 1;	
		WORD				m_isWantNotComplete			: 1;
		WORD				m_bf1IsDisable				: 1;
		WORD				m_bf1IsForEnterFieldPlayer	: 1;

		WORD				m_wReactionCount,m_wTriggerCount;		//	조건의 수,반응의 수
		WORD				m_wTitleLength;
		WORD				m_wActivateChance;
		
		WORD				m_wActiveCount;
		WORD				m_wActivePeriod;
	};

	CKarmaBasicInfo	karma;
	int		i;

	if	(m_iFileVersion	>=	eVUI_26_ADD_DUNGEON_VALUE_AND_KARMA_VALUE)
		_lpFile->Read(&karma,sizeof(CKarmaBasicInfo));
	else
	if	(m_iFileVersion		>=	eVUI_20_ADD_KARMA_OCCUR_CHANCE)
		_lpFile->Read(&karma,sizeof(CKarmaBasicInfo)-4);
	else	
		_lpFile->Read(&karma,sizeof(CKarmaBasicInfo)-2-4);

	if	(_bIsDecodeKaramData)
		cMessSign::DecodeDataBuffer2(&karma,sizeof(CKarmaBasicInfo));

	if	(karma.m_wTitleLength	>	0)
		_lpFile->Skip(karma.m_wTitleLength);

	for (i=0;i<karma.m_wTriggerCount;i++)
		skipKarmaItem(_lpFile,_bIsDecodeKaramData);

	for (i=0;i<karma.m_wReactionCount;i++)
		skipKarmaItem(_lpFile,_bIsDecodeKaramData);

}

void
cMAP::skipKarmaContainer(cFILE *_lpFile,BOOL _bIsDecodeKaramData)
{
	WORD	wKarmaCount,wTitleLength;

	_lpFile->Read(&wKarmaCount,2);
	_lpFile->Read(&wTitleLength,2);

	if	(wTitleLength)
		_lpFile->Skip(wTitleLength);

	for (int iKarma=0;iKarma<wKarmaCount;iKarma++)
		skipKarma(_lpFile,_bIsDecodeKaramData);
}

//
//	Npc의 대화 데이터 읽기
void
cMAP::skipCC(cFILE *_lpFile,int _iSpeechCount,BOOL _bIsDecodeKaramData)
{
	_lpFile->Skip(4);	//	박스 크기와 사이즈

	class	CConversationBasicInfo
	{
	public:
		WORD					m_wSerial;			//	시리얼
		WORD					m_wSpeechLength;	//	대사 길이
		WORD					m_wContentsCount;	//	선택 항목 수
		WORD					m_wTriggerCount;	//	이 대화가 처음에 시작될 조건 수

		WORD					m_isWantAndCondition: 1;
		WORD					m_isWantNotComplete	: 1;
		WORD					m_spareValue		: 14;

		WORD					m_wIsNoSpeech;		//	대화 없음
	};

	class	CKarmaContainerBasicElement
	{
	public:
		WORD				m_wSerial;	//	대사 길이
		WORD				m_wTitleLength;	//	대사 길이
		WORD				m_wKarmaCount;	//	이 대화가 처음에 시작될 조건 수
	};

	for (int j=0;j<_iSpeechCount;j++)
	{
		CConversationBasicInfo	speech;

		_lpFile->Read(&speech,sizeof(CConversationBasicInfo));//	기본 정보

		if	(_bIsDecodeKaramData)
			cMessSign::DecodeDataBuffer2(&speech,sizeof(CConversationBasicInfo));

		_lpFile->Skip(speech.m_wSpeechLength);//대사 읽기

		for (int iTrigger=0;iTrigger<speech.m_wTriggerCount;iTrigger++)
			skipKarmaItem(_lpFile,_bIsDecodeKaramData);

		CKarmaContainerBasicElement	kc;

		for (int iContent=0;iContent<speech.m_wContentsCount;iContent++)
		{
			_lpFile->Read(&kc.m_wKarmaCount,2);
			_lpFile->Read(&kc.m_wTitleLength,2);

			_lpFile->Skip(kc.m_wTitleLength);

			for (int iKarma=0;iKarma<kc.m_wKarmaCount;iKarma++)
				skipKarma(_lpFile,_bIsDecodeKaramData);
		}
	}
}

//
//	맵 로딩
BOOL
cMAP::load(char *_lpstrFn,cNUX *_lpNux)
{
	strcpy(m_strFileName,_lpstrFn);

	int	iSerial	=	atoi(_lpstrFn+1);
	int	iNation	=	eNationKorea;

	if	(iSerial	>=	900)
		g_bIsGuildBattleField	=	TRUE;
	else
		g_bIsGuildBattleField	=	FALSE;

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("load map");

	g_project.setMapFolder();

	char	strFolder[512];

	GetCurrentDirectory(512,strFolder);

	int		iFileSize,iCount=0;
	cFILE	*pFile;
	BOOL	bIsExist	=	IsFile(_lpstrFn);

	if	(_lpNux)
		pFile	=	_lpNux->Open(_lpstrFn);
	else
	{
		pFile	=	new cFILE();

		if	(!pFile->Open(_lpstrFn,"rb"))
			KILL(pFile);
	}

	if	(!pFile)
	{
		SetLogFolder();

		g_eh.addStaticLog("failed to load map. '%s' file not found",_lpstrFn);

		return FALSE;
	}

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("read header");

//	헤더 읽기
	{
		char	strHeader[60];
		pFile->Read(&iFileSize			,4);	//	화일 사이즈
		pFile->Read(&m_iTextDataLocate	,4);	//	텍스트 데이터의 위치
		pFile->Read(strHeader			,60);	//	텍스트 데이터의 위치

		getMapDataVersion(strHeader);
	}

	BOOL	bIsDecodeKaramData	=	FALSE;
	
	if	(m_iFileVersion>=	eVVI_49_ENCRYT_MORE)
		bIsDecodeKaramData	=	TRUE;

	reset();									//	데이터 초기화

//	맵 정보 읽어옴	
	if	(m_iFileVersion>=	eVUI_30_ADD_CORECT_MONSTER_RESISTANCE)
		pFile->Read(this	,sizeof(cMAP_INFO));		//	cMAP_INFO 인자만 읽어 들인다. 주의해서 사용할것..
	else
	{
		memset(this,0,sizeof(cMAP_INFO));
		pFile->Read(this	,sizeof(cMapBaseInfo20050527));		//	cMAP_INFO 인자만 읽어 들인다. 주의해서 사용할것..
	}

	if	(m_iFileVersion>=	eVVI_51_ENCRYT2)
	{
		pFile->Read(&iNation,4);	//	영역 정보 위치
		cMessSign::SetDataEncodeTable(iNation);
	}
	else
		cMessSign::SetDataEncodeTable(-1);

	pFile->Seek(4);	//	영역 정보 위치

	m_iSerial		=	atoi(_lpstrFn+1);

	m_bIsRoom		=	FALSE;

	if	(m_iTileSet	==	eTS_ROOM	||	m_iTileSet	==	eTS_GUILD_HALL)
		m_bIsRoom	=	TRUE;

//	읽어온 정보를 바탕으로 몇가지 정보 설정
	{
		m_iPixelWidth		=	m_iWidth<<dTILE_XSIZE_SHIFT;
		m_iPixelHeight		=	m_iHeight<<dTILE_YSIZE_SHIFT;		//	픽셸 단위 사이즈
	}	//	읽어온 정보를 바탕으로 몇가지 정보 설정

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("read data");

//	맵 데이터 로딩
	{
		pFile->Read(m_aMap	,m_iWidth*m_iHeight,2);	//	타일정보
		pFile->Seek(m_iWidth*m_iHeight*2,SEEK_CUR);	//	높이 정보(툴에서만 사용한다)
		pFile->Read(m_aInfo	,m_iWidth*m_iHeight,2);	//	건물,오브젝트,블럭 정보
	}	//	맵 데이터 로딩

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("read door list");

	if	(m_iFileVersion	>=	eVUI_29_ADD_DOOR_LIST)
	{
		pFile->Read(&m_doors.m_iDoorCount,4);

		for (int i=0;i<m_doors.m_iDoorCount;i++)
		{
			cDoorObjectInfo	*lpDoor	=	&m_doors.m_aDoorList[i];

			pFile->Read(lpDoor,sizeof(cDoorObjectSimpleInfo));

			lpDoor->m_iPosValue	=	lpDoor->m_wX+lpDoor->m_wY*m_iWidth;
			lpDoor->reset();
		}
	}
//	맵데이터
	{
		pFile->Read(m_aBlockInfo,m_iWidth*m_iHeight);
	}	//	서버에서 쓰는 맵데이터

////////////////////////////////////////////////////////////////////////////////////////////////
//	커스텀 아이템 스킵
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		DWORD	dwSkipLocate;
		pFile->Read(&dwSkipLocate,4);	//	스킵 데이터 위치
		pFile->Seek(dwSkipLocate,SEEK_SET);
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	캐릭터 데이터
////////////////////////////////////////////////////////////////////////////////////////////////

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("read character data");

	{
		memset(g_jm.m_aJobList,0xff,sizeof(g_jm.m_aJobList));
		pFile->Read(&g_jm.m_iCharacterDataCount,	4);	//	저장된 캐릭터의 수

		if	(m_iFileVersion	>=	eVVI_48_ENCRYT)
			cMessSign::DecodeDataBuffer2(&g_jm.m_iCharacterDataCount,4);

		pFile->Read(g_jm.m_aJobList,2*g_jm.m_iCharacterDataCount);	//	저장된 캐릭터의 수

		if	(m_iFileVersion	>=	eVVI_48_ENCRYT)
			cMessSign::DecodeDataBuffer2(g_jm.m_aJobList,2*g_jm.m_iCharacterDataCount);

		DWORD	dwSkipPoint;
		pFile->Read(&dwSkipPoint,4);	//	캐릭터 데이터를 스킵할 곳
		pFile->Seek(dwSkipPoint,SEEK_SET);
/*
		int	iAvailCharacterEventCount;

		pFile->Read(&iAvailCharacterEventCount,4);

		for (int iEvent=0;iEvent<iAvailCharacterEventCount;iEvent++)
		{
			file.Skip(4);

			skipKarmaContainer(pFile);
		}*/
	}	//	캐릭터 데이터

////////////////////////////////////////////////////////////////////////////////////////////////
//	액터
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		pFile->Read(&m_iSavedActorCount	,4);//	읽기 싫을때 점프할 위치
		pFile->Read(&m_iSavedActorCount	,4);//	액터의 수

		if	(m_iFileVersion	>=	eVVI_48_ENCRYT)
			cMessSign::DecodeDataBuffer2(&m_iSavedActorCount	,4);

		for (int i=0;i<m_iSavedActorCount;i++)
		{
			cBASE_NPC_INFO	existActor;
			WORD					wSpeechCount;

			pFile->Read(&existActor,sizeof(cBASE_NPC_INFO));	//	액터 데이터 로딩
			pFile->Read(&wSpeechCount,2);	//	액터 데이터 로딩
			
			if	(m_iFileVersion	>=	eVVI_48_ENCRYT)
				cMessSign::DecodeDataBuffer2(&existActor,sizeof(cBASE_NPC_INFO));
			
			memset(&m_aSaveActor[existActor.m_iSerial], 0, sizeof(cBASE_NPC_INFO));
					
			memcpy(&m_aSaveActor[existActor.m_iSerial],&existActor,sizeof(cBASE_NPC_INFO));
			
			if	(wSpeechCount	>	0)
				skipCC(pFile,wSpeechCount,bIsDecodeKaramData);
		}
	}	//	액터


////////////////////////////////////////////////////////////////////////////////////////////////
//	영역 불러오기
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		m_area.reset();

		pFile->Read(&m_area.m_iCount,4);										//	영역의 수

		if	(m_iFileVersion	>=	eVVI_48_ENCRYT)
			cMessSign::DecodeDataBuffer2(&m_area.m_iCount,4);

		for (int i=0;i<m_area.m_iCount;i++)
		{
			cAREA_SAVE_DATA	saveArea;
			int				iLocate;
			char			*pstrText	=	NULL;

			if (m_iFileVersion	>=	eVUI_28_EXPAND_AREA_DATA)
			{
				if (m_iFileVersion	>=	eVVI_43_ADD_SEASON_VARIABLE)
				{
					pFile->Read(&saveArea,sizeof(cAREA_SAVE_DATA));
				}
				else
				{
					pFile->Read(&saveArea,sizeof(cAREA_SAVE_DATA) - 2);
					saveArea.m_wSeasonVariable = 0;
				}
				//pFile->Read(&saveArea,sizeof(cAREA_SAVE_DATA));		//
				pFile->Read(&iLocate,4);	//	카르마 컨테이너 스킵
				pFile->Seek(iLocate,SEEK_SET);	//	카르마 컨테이너 스킵
			}
			else
			{
				pFile->Read(&saveArea,sizeof(cAREA_OLD_SAVE_DATA));		//
				saveArea.m_wSeasonVariable = 0;
			}

			if	(m_iFileVersion	>=	eVVI_48_ENCRYT)
				cMessSign::DecodeDataBuffer2(&saveArea,sizeof(cAREA_SAVE_DATA));

			pFile->Read(&iLocate,4);				//	텍스트의 위치

			if	(iLocate	!=	0xffffffff)
			{
				iLocate	+=	m_iTextDataLocate;

				int	iTextSize;
				int	iCurLocate	=	pFile->Locate();	//	현재 위치

				pFile->Seek(iLocate,SEEK_SET);		//	문장의 위치로 이동
				pFile->Read(&iTextSize,4);					//	사이즈

				pstrText		=	new char [iTextSize+1];

				pFile->Read(pstrText,iTextSize);
				pFile->Seek(iCurLocate,SEEK_SET);
			}

			if(g_iSeasonVariable >= saveArea.m_wSeasonVariable)
				m_area.insert(saveArea.m_wSerial,(cAreaInfo *)&saveArea,m_bf1IsGuildBattleLobby,pstrText);

			pKILL(pstrText);
		}
	}	//	영역 불러오기

////////////////////////////////////////////////////////////////////////////////////////////////
//	상점
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		DWORD	dwSkipLocate;

		pFile->Read(&dwSkipLocate,4);	//	상점의 수
		pFile->Seek(dwSkipLocate,SEEK_SET);	//	상점의 수
	}
	
////////////////////////////////////////////////////////////////////////////////////////////////
//	오브젝트
////////////////////////////////////////////////////////////////////////////////////////////////

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("read object");

	m_object.readObjectListInMap(pFile,m_bIsRoom);

	KILL(pFile);

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("build area info");

	m_area.buildInfo();

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("complete load map");

	SetLogFolder();

	if	(m_bf1IsGuildHall)
		m_object.m_bIsGuildHall	=	TRUE;
	else
		m_object.m_bIsGuildHall	=	FALSE;

	return TRUE;
}

void
cMAP::drawFieldTrap(int _iDetectLevel)
{
	for (int i=0;i<m_area.m_wTrapAreaCount;i++)
	{
		cAreaTrapInfo	*lpTrapArea	=	m_area.getTrapArea(i);

		if	(!lpTrapArea->m_wIsActive || _iDetectLevel < lpTrapArea->m_wDetectTrapLevel)
			continue;

		cRECT	rect;

		rect.x1	=	GetScaledXPos(lpTrapArea->x1);
		rect.y1	=	GetScaledYPos(lpTrapArea->y1);
		rect.x2	=	GetScaledXPos(lpTrapArea->x2);
		rect.y2	=	GetScaledYPos(lpTrapArea->y2);

		cDRAW::Box(_LTRED,&rect);
		cDRAW::FillHB(_LTRED,&rect);
	}
}

//
//	맵.. 타일만으로 이루어진 맵만 그려준다.
void
cMAP::drawMap()
{
	if	(g_bIsHalfSize)
	{
		drawHalfMap();
		return;
	}
	
	int mx,my;
	int tile;
	int	x,y;
	int	addr	=	0,addSize=0;
	int	nwCount	=	1,nCount,neCount	=	1,wCount,eCount,swCount	=	1,sCount,seCount	=	1;
	int	tpw,tph;
	int	tileXSize=0,tileYSize=0,tileXRemain=0,tileYRemain=0;

	m_iTPW		=	cDRAW::WIDTH  / dTILE_XS + 1;		//	화면에 나오는 타일수 (가로
	m_iTPH		=	cDRAW::HEIGHT / dTILE_YS + 1;		//	화면에 나오는 타일수 (세로
	m_posMove.x	=	m_pos.x % dTILE_XS;					//	타일크기를 벗어난 이동 픽셸
	m_posMove.y	=	m_pos.y % dTILE_YS;
	m_pos.mx	=	m_pos.x >> dTILE_XSIZE_SHIFT;		//	타일단위 위치
	m_pos.my	=	m_pos.y >> dTILE_YSIZE_SHIFT;

	if (m_posMove.x != 0)	tileXSize	=	dTILE_XS - m_posMove.x;
	if (m_posMove.y != 0)	tileYSize	=	dTILE_YS - m_posMove.y;

	//JBC 옵져버 모드로 전환 08-07-15
	if (g_bObserverMode)
		g_am.SetViewPortLargeMargin();
	else
		g_rectViewPort.set(	m_pos.x - dVIEWPORT_MARGIN,
						m_pos.y - dVIEWPORT_MARGIN,
						m_pos.x + cDRAW::WIDTH	+ dVIEWPORT_MARGIN,
						m_pos.y + cDRAW::HEIGHT + dVIEWPORT_MARGIN);

	if (g_bIsTerminalPC)
	{
		cDRAW::Fill(0);
		drawBlockedTile();	//	못지나가는 타일 표시

		m_area.drawGateWay(m_pos.x,m_pos.y,s_iFrameCounter);
		g_am.drawDeathActors(&g_rectViewPort);	//	뷰포트 안에 캐릭터들을 그린다.
		g_fieldItem.draw(m_pos.x,m_pos.y);

		return;
	}

//	cDRAW::Fill(0);
//	return;

	//	뷰포트 화면에서 100정도의 여유를 준다.

	mx			=	m_pos.mx;
	my			=	m_pos.my;

	tpw			=	(cDRAW::WIDTH - tileXSize)	>>	dTILE_XSIZE_SHIFT;	//	클리핑 안하고 찍는 타일의 수
	tph			=	(cDRAW::HEIGHT- tileYSize)	>>	dTILE_YSIZE_SHIFT;

	tileXRemain	=	cDRAW::WIDTH - (tileXSize + (tpw<<dTILE_XSIZE_SHIFT) );
	tileYRemain	=	cDRAW::HEIGHT- (tileYSize + (tph<<dTILE_YSIZE_SHIFT) );


	if (m_posMove.x == 0)	
	{	nwCount	=	0;
		swCount	=	0;
		 wCount	=	0;
	}
	else	wCount	=	tph;

	if (m_posMove.y == 0)
	{
		nCount	=	0;
		nwCount	=	0;
		neCount	=	0;
	}
	else	nCount	=	tpw;

	if (tileXRemain	== 0)
	{	neCount	=	0;
		seCount	=	0;
		 eCount	=	0;
	}
	else	eCount	=	tph;
	

	if (tileYRemain	== 0)
	{	sCount	=	0;
		seCount	=	0;
		swCount	=	0;
	}
	else	sCount	=	tpw;

	{
	////	상단
		//	NW
		int	index	=	mx + my * m_iWidth;

		if (nwCount > 0)
		{	tile	=	m_aMap[index];

			if (tile < 0x8000)	m_tile.put16NW(m_posMove.x,m_posMove.y,tile&0x3fff,cDRAW::p2SCREEN+addr);
		}

		//	N
		if (nCount > 0)
		{
			if (nwCount > 0 )
			{	index	=	mx + my * m_iWidth + 1;
				addr	=	tileXSize;
			}
			else
			{	index	=	mx + my * m_iWidth;
				addr	=	0;
			}

			for (x = 0 ; x < nCount ; x++)
			{	tile	=	m_aMap[index];

				if (tile < 0x8000)	m_tile.put16N(m_posMove.y,tile&0x3fff,cDRAW::p2SCREEN+addr);

				addr	+=	dTILE_XS;
				index++;
			}
		}

		//	NE nCount 가 0 이면 이 넘도 0이다. 고로 index나 addr은 바꿀 필요가 없다.
		if (neCount > 0)
		{	tile	=	m_aMap[index];

			if (tile < 0x8000)	m_tile.put16NE(tileXRemain,m_posMove.y,tile&0x3fff,cDRAW::p2SCREEN+addr);
								//	mPUTTILECLIPNE(addr,tile&0x3fff,tileXRemain,m_posMove.y);
		}

	////	중단
		//	W
		if (wCount > 0)
		{
			if (nCount !=	0)	my	=	m_pos.my+1;
			else				my	=	m_pos.my;

			mx		=	m_pos.mx;

			addr	=	tileYSize*cDRAW::WIDTH;
			index	=	mx + my * m_iWidth;
			addSize	=	dTILE_YS*cDRAW::WIDTH;

			for (y = 0 ; y < wCount ; y++)
			{	tile	=	m_aMap[index];

				if (tile < 0x8000)	m_tile.put16W(m_posMove.x,tile&0x3fff,cDRAW::p2SCREEN+addr);
	//					mPUTTILECLIPW(addr,tile&0x3fff,m_posMove.x);

				addr	+=	addSize;
				index	+=	m_iWidth;
			}
		}

		//	E
		if (eCount > 0)
		{
			if (wCount	!=	0)	mx	=	m_pos.mx+tpw+1;
			else				mx	=	m_pos.mx+tpw;
			if (nCount	!=	0)	my	=	m_pos.my+1;
			else				my	=	m_pos.my;

			addr	=	tileXSize + (tpw<<dTILE_XSIZE_SHIFT) + tileYSize*cDRAW::WIDTH;
			index	=	mx + my * m_iWidth;

			addSize	=	dTILE_YS*cDRAW::WIDTH;

			for (y = 0 ; y < eCount ; y++)
			{	tile	=	m_aMap[index];

				if (tile < 0x8000)	m_tile.put16E(tileXRemain,tile&0x3fff,cDRAW::p2SCREEN+addr);

				addr	+=	addSize;
				index	+=	m_iWidth;
			}
		}

	////	하단
		//	SW
		if (swCount > 0)
		{
			mx	=	m_pos.mx;

			if (nCount	!=	0)	my	=	m_pos.my+tph+1;
			else				my	=	m_pos.my+tph;

			addr	=	(tileYSize+(tph<<dTILE_YSIZE_SHIFT))*cDRAW::WIDTH;

			index	=	mx + my * m_iWidth;
			tile	=	m_aMap[index];

			if (tile < 0x8000)
				m_tile.put16SW(m_posMove.x,tileYRemain,tile&0x3fff,cDRAW::p2SCREEN+addr);
		}

		//	S
		if (sCount > 0)
		{
			if (wCount	!=	0)	mx	=	m_pos.mx+1;
			else				mx	=	m_pos.mx;
			if (nCount	!=	0)	my	=	m_pos.my+tph+1;
			else				my	=	m_pos.my+tph;

			addr	=	tileXSize + (tileYSize+(tph<<dTILE_YSIZE_SHIFT))*cDRAW::WIDTH;
			index	=	mx + my * m_iWidth;

			for (x = 0 ; x < sCount ; x++)
			{	tile	=	m_aMap[index];

				if (tile < 0x8000)	m_tile.put16S(tileYRemain,tile&0x3fff,cDRAW::p2SCREEN+addr);

				addr	+=	dTILE_XS;
				index	++;
			}
		}

		//	SE
		if (seCount > 0)
		{
			if (wCount	!=	0)	mx	=	m_pos.mx+tpw+1;
			else				mx	=	m_pos.mx+tpw;
			if (nCount	!=	0)	my	=	m_pos.my+tph+1;
			else				my	=	m_pos.my+tph;

			addr	=	tileXSize + (tpw << dTILE_XSIZE_SHIFT) + (tileYSize+(tph<<dTILE_YSIZE_SHIFT))*cDRAW::WIDTH;
			index	=	mx + my * m_iWidth;

			tile	=	m_aMap[index];

			if (tile < 0x8000)	m_tile.put16SE(tileXRemain,tileYRemain,tile&0x3fff,cDRAW::p2SCREEN+addr);
		}

		//	멀쩡한 타일들

		if (wCount	!=	0)	mx	=	m_pos.mx+1;
		else				mx	=	m_pos.mx;
		if (nCount	!=	0)	my	=	m_pos.my+1;
		else				my	=	m_pos.my;

		index	=	mx + my * m_iWidth;
		addr	=	tileXSize + tileYSize*cDRAW::WIDTH;
		addSize	=	dTILE_YS*cDRAW::WIDTH;

		for (y = 0 ; y < tph ; y++)
		{	int	temp_addr	=	addr;
			int	temp_index	=	index;

			for (x = 0 ; x < tpw ; x++)
			{	int tile	=	m_aMap[index+x];

				if (tile < 0x8000)	
					m_tile.put16(tile&0x3fff,cDRAW::p2SCREEN+addr);

				addr += dTILE_XS;
			}
			addr	=	temp_addr + addSize;
			index	=	temp_index+	m_iWidth;
		}
	}

	m_object.drawRapes(m_pos.x,m_pos.y);	//	먼저 바닥에 깔리는 오브젝트
	g_trapManager.drawFlatTraps(&g_rectViewPort,m_pos.x,m_pos.y);
	m_area.drawGateWay(m_pos.x,m_pos.y,s_iFrameCounter);

	g_am.drawDeathActors(&g_rectViewPort);	//	뷰포트 안에 캐릭터들을 그린다.

	g_fieldItem.draw(m_pos.x,m_pos.y);

	return;
	
	{
		int	tpw		=	cDRAW::WIDTH	>>	dTILE_XSIZE_SHIFT;	//	클리핑 안하고 찍는 타일의 수
		int	tph		=	cDRAW::HEIGHT	>>	dTILE_YSIZE_SHIFT;

		for (int i= 0;i<m_iTPH;i++)
			for (int j= 0;j<m_iTPW;j++)
			{
				int	mx	=	m_pos.mx+j;
				int	my	=	m_pos.my+i;
				int tile=	m_aMap[mx + my * m_iWidth];

				int	xp	=	j*dTILE_XS-(m_pos.x%dTILE_XS);
				int	yp	=	i*dTILE_YS-(m_pos.y%dTILE_YS);

				s_text.put(xp,yp,WHITE,"%d",tile);
			}
	}
}

void
cMAP::drawHalfMap()
{
	m_pos.mx		=	m_pos.x >> dTILE_XSIZE_SHIFT;		//	타일단위 위치
	m_pos.my		=	m_pos.y >> dTILE_YSIZE_SHIFT;

	m_posView.mx	=	m_posView.x >> dTILE_XSIZE_SHIFT_H;		//	타일단위 위치
	m_posView.my	=	m_posView.y >> dTILE_YSIZE_SHIFT_H;
	
	int mx,my;
	int tile;
	int	x,y;
	int	addr	=	0,addSize=0;
	int	nwCount	=	1,nCount,neCount	=	1,wCount,eCount,swCount	=	1,sCount,seCount	=	1;
	int	tpw,tph;
	int	tileXSize=0,tileYSize=0,tileXRemain=0,tileYRemain=0;

	m_iTPW		=	cDRAW::WIDTH  / dTILE_XS_H + 1;		//	화면에 나오는 타일수 (가로
	m_iTPH		=	cDRAW::HEIGHT / dTILE_YS_H + 1;		//	화면에 나오는 타일수 (세로
	m_posMove.x	=	m_posView.x % dTILE_XS_H;					//	타일크기를 벗어난 이동 픽셸
	m_posMove.y	=	m_posView.y % dTILE_YS_H;

	if (m_posMove.x != 0)
		tileXSize	=	dTILE_XS_H - m_posMove.x;
	if (m_posMove.y != 0)
		tileYSize	=	dTILE_YS_H - m_posMove.y;

	if (g_bObserverMode)
		g_am.SetViewPortLargeMargin();
	else
		g_rectViewPort.set(	m_posView.x*2 - dVIEWPORT_MARGIN*2,
						m_posView.y*2 - dVIEWPORT_MARGIN*2,
						m_posView.x*2 + cDRAW::WIDTH*2  + dVIEWPORT_MARGIN*2,
						m_posView.y*2 + cDRAW::HEIGHT*2 + dVIEWPORT_MARGIN*2);

	if (g_bIsTerminalPC)
	{
		cDRAW::Fill(0);
		drawBlockedTile();	//	못지나가는 타일 표시

		m_area.drawGateWay(m_pos.x,m_pos.y,s_iFrameCounter);
		g_am.drawDeathActors(&g_rectViewPort);	//	뷰포트 안에 캐릭터들을 그린다.
		g_fieldItem.draw(m_pos.x,m_pos.y);

		return;
	}

	mx			=	m_posView.mx;
	my			=	m_posView.my;

	tpw			=	(cDRAW::WIDTH - tileXSize)	>>	dTILE_XSIZE_SHIFT_H;	//	클리핑 안하고 찍는 타일의 수
	tph			=	(cDRAW::HEIGHT- tileYSize)	>>	dTILE_YSIZE_SHIFT_H;

	tileXRemain	=	cDRAW::WIDTH - (tileXSize + (tpw<<dTILE_XSIZE_SHIFT_H) );
	tileYRemain	=	cDRAW::HEIGHT- (tileYSize + (tph<<dTILE_YSIZE_SHIFT_H) );


	if (m_posMove.x == 0)	
	{
		nwCount	=	0;
		swCount	=	0;
		wCount	=	0;
	}
	else
		wCount	=	tph;

	if (m_posMove.y == 0)
	{
		nCount	=	0;
		nwCount	=	0;
		neCount	=	0;
	}
	else
		nCount	=	tpw;

	if (tileXRemain	== 0)
	{	
		neCount	=	0;
		seCount	=	0;
		eCount	=	0;
	}
	else
		eCount	=	tph;
	

	if (tileYRemain	== 0)
	{
		sCount	=	0;
		seCount	=	0;
		swCount	=	0;
	}
	else
		sCount	=	tpw;

	{
	////	상단
		//	NW
		int	index	=	mx + my * m_iWidth;

		if (nwCount > 0)
		{	
			tile	=	m_aMap[index];

			if (tile < 0x8000)
				m_tile.put16NWh(m_posMove.x,m_posMove.y,tile&0x3fff,cDRAW::p2SCREEN+addr);
		}

		//	N
		if (nCount > 0)
		{
			if (nwCount > 0 )
			{
				index	=	mx + my * m_iWidth + 1;
				addr	=	tileXSize;
			}
			else
			{
				index	=	mx + my * m_iWidth;
				addr	=	0;
			}

			for (x = 0 ; x < nCount ; x++)
			{	
				tile	=	m_aMap[index];

				if (tile < 0x8000)
					m_tile.put16Nh(m_posMove.y,tile&0x3fff,cDRAW::p2SCREEN+addr);

				addr	+=	dTILE_XS_H;
				index++;
			}
		}

		//	NE nCount 가 0 이면 이 넘도 0이다. 고로 index나 addr은 바꿀 필요가 없다.
		if (neCount > 0)
		{
			tile	=	m_aMap[index];

			if (tile < 0x8000)
				m_tile.put16NEh(tileXRemain,m_posMove.y,tile&0x3fff,cDRAW::p2SCREEN+addr);
		}

	////	중단
		//	W
		if (wCount > 0)
		{
			if (nCount !=	0)
				my	=	m_posView.my+1;
			else
				my	=	m_posView.my;

			mx		=	m_posView.mx;

			addr	=	tileYSize*cDRAW::WIDTH;
			index	=	mx + my * m_iWidth;
			addSize	=	dTILE_YS_H*cDRAW::WIDTH;

			for (y = 0 ; y < wCount ; y++)
			{	
				tile	=	m_aMap[index];

				if (tile < 0x8000)
					m_tile.put16Wh(m_posMove.x,tile&0x3fff,cDRAW::p2SCREEN+addr);

				addr	+=	addSize;
				index	+=	m_iWidth;
			}
		}

		//	E
		if (eCount > 0)
		{
			if (wCount	!=	0)
				mx	=	m_posView.mx+tpw+1;
			else
				mx	=	m_posView.mx+tpw;
			if (nCount	!=	0)
				my	=	m_posView.my+1;
			else
				my	=	m_posView.my;

			addr	=	tileXSize + (tpw<<dTILE_XSIZE_SHIFT_H) + tileYSize*cDRAW::WIDTH;
			index	=	mx + my * m_iWidth;

			addSize	=	dTILE_YS_H*cDRAW::WIDTH;

			for (y = 0 ; y < eCount ; y++)
			{
				tile	=	m_aMap[index];

				if (tile < 0x8000)
					m_tile.put16Eh(tileXRemain,tile&0x3fff,cDRAW::p2SCREEN+addr);

				addr	+=	addSize;
				index	+=	m_iWidth;
			}
		}

	////	하단
		//	SW
		if (swCount > 0)
		{
			mx	=	m_posView.mx;

			if (nCount	!=	0)
				my	=	m_posView.my+tph+1;
			else
				my	=	m_posView.my+tph;

			addr	=	(tileYSize+(tph<<dTILE_YSIZE_SHIFT_H))*cDRAW::WIDTH;

			index	=	mx + my * m_iWidth;
			tile	=	m_aMap[index];

			if (tile < 0x8000)
				m_tile.put16SWh(m_posMove.x,tileYRemain,tile&0x3fff,cDRAW::p2SCREEN+addr);
		}

		//	S
		if (sCount > 0)
		{
			if (wCount	!=	0)
				mx	=	m_posView.mx+1;
			else
				mx	=	m_posView.mx;
			if (nCount	!=	0)
				my	=	m_posView.my+tph+1;
			else
				my	=	m_posView.my+tph;

			addr	=	tileXSize + (tileYSize+(tph<<dTILE_YSIZE_SHIFT_H))*cDRAW::WIDTH;
			index	=	mx + my * m_iWidth;

			for (x = 0 ; x < sCount ; x++)
			{
				tile	=	m_aMap[index];

				if (tile < 0x8000)	
					m_tile.put16Sh(tileYRemain,tile&0x3fff,cDRAW::p2SCREEN+addr);

				addr	+=	dTILE_XS_H;
				index	++;
			}
		}

		//	SE
		if (seCount > 0)
		{
			if (wCount	!=	0)
				mx	=	m_posView.mx+tpw+1;
			else
				mx	=	m_posView.mx+tpw;
			if (nCount	!=	0)
				my	=	m_posView.my+tph+1;
			else
				my	=	m_posView.my+tph;

			addr	=	tileXSize + (tpw << dTILE_XSIZE_SHIFT_H) + (tileYSize+(tph<<dTILE_YSIZE_SHIFT_H))*cDRAW::WIDTH;
			index	=	mx + my * m_iWidth;

			tile	=	m_aMap[index];

			if (tile < 0x8000)
				m_tile.put16SEh(tileXRemain,tileYRemain,tile&0x3fff,cDRAW::p2SCREEN+addr);
		}

		//	멀쩡한 타일들

		if (wCount	!=	0)
			mx	=	m_posView.mx+1;
		else
			mx	=	m_posView.mx;
		if (nCount	!=	0)
			my	=	m_posView.my+1;
		else
			my	=	m_posView.my;

		index	=	mx + my * m_iWidth;
		addr	=	tileXSize + tileYSize*cDRAW::WIDTH;
		addSize	=	dTILE_YS_H*cDRAW::WIDTH;

		for (y = 0 ; y < tph ; y++)
		{
			int	temp_addr	=	addr;
			int	temp_index	=	index;

			for (x = 0 ; x < tpw ; x++)
			{	int tile	=	m_aMap[index+x];

				if (tile < 0x8000)	
					m_tile.put16h(tile&0x3fff,cDRAW::p2SCREEN+addr);

				addr += dTILE_XS_H;
			}

			addr	=	temp_addr + addSize;
			index	=	temp_index+	m_iWidth;
		}
	}

	m_object.drawRapes(m_posView.x,m_posView.y);	//	먼저 바닥에 깔리는 오브젝트
	g_trapManager.drawFlatTraps(&g_rectViewPort,m_posView.x,m_posView.y);
	m_area.drawGateWay(m_posView.x,m_posView.y,s_iFrameCounter);

	g_am.drawDeathActors(&g_rectViewPort);	//	뷰포트 안에 캐릭터들을 그린다.

	g_fieldItem.draw(m_posView.x,m_posView.y);
}

void
cMAP::DrawEventObjectName()		
{
	int		minY	=	max(m_pos.my - dMAP_MARGIN,0);
	int		minX	=	max(m_pos.mx - dMAP_MARGIN,0);
	int		maxY	=	min(m_pos.my + m_iTPH + dMAP_MARGIN,m_iHeight-1);
	int		maxX	=	min(m_pos.mx + m_iTPW + dMAP_MARGIN,m_iWidth -1);

	int		bx		=	(minX << dTILE_XSIZE_SHIFT) - m_pos.x + dTILE_XS/2;
	int		by		=	(minY << dTILE_YSIZE_SHIFT) - m_pos.y + dTILE_YS/2;
	int		x	=	bx;
	int		y	=	by;


	for (int my = minY; my < maxY; my++)
	{
		int index	=	my	*	m_iWidth;

		for (int mx = minX; mx < maxX; mx++)
		{
			int value	=	m_aInfo[index+mx];

			int iFixedIndex = -1;
			

			if	(value & dMASK_OBJECT){

				iFixedIndex = value - dMASK_OBJECT;

			}else if(value & dMASK_MISSLIE_BLOCK_OBJECT){
				
				iFixedIndex = value - dMASK_MISSLIE_BLOCK_OBJECT;
								
			}
			
			if(iFixedIndex != -1){
				cFIXED_OBJECT_INFO* lpObject = &m_object.m_aFixedObjectList[iFixedIndex];
				if(lpObject->m_bf1IsFocused){	
					
					
					int iAreaIndex = m_area.getFocusArea(x + m_pos.x,y + m_pos.y,TRUE);
					
					if(iAreaIndex != 0xffff)
					{
						cOBJ_ANM* lpAnm = &m_object.m_aFixedObjectImage[lpObject->m_wObject];
						cRECT rt;
						
						lpAnm->getRect(0,&rt);
						cAreaInfo*	lpArea			=	m_area.getArea(iAreaIndex);

						if(lpArea && lpArea->isEventArea())
						{
							cAreaEvent	*lpEventArea	=	(cAreaEvent	*)lpArea;
							
							
							if(lpEventArea && lpEventArea->m_bf1Npc)
							{
								char* strName = lpEventArea->m_strName;
								int iNameWidth	= 	s_sText.getPixelSize(lpEventArea->m_strName);
								
								int iX = x  + rt.x1 + (lpAnm->m_iWidth/2) ; 
								int iY = y + rt.y1 - 20;
								
								g_sprNameBar.putClipedImage(iX - iNameWidth/2			,iY,eNBI_GRAY_LEFT  ,iNameWidth);		
								g_sprNameBar.putClipedImage(iX - iNameWidth/2+iNameWidth,iY,eNBI_GRAY_LEFT+1,iNameWidth);
								g_sprNameBar.putClipedImage(iX - iNameWidth/2			,iY,eNBI_GRAY_LEFT+2,iNameWidth);	
								s_sText.cPut(iX,iY + 2 ,WHITE, strName);
								
								
							}
						}
					}
				}
				
			}
			x	+=	dTILE_XS;
			
		}

		x		=	bx;
		y		+=	dTILE_YS;
	}


}
//
//	맵위에 올라가는 각종 오브젝트들을 찍어준다.(캐릭터,오브젝트 어쩌구... 저쩌구..)
void
cMAP::drawObject()
{
	if	(g_bIsHalfSize)
	{
		drawObjectHalfSize();
		return;
	}

	if	(g_bIsTerminalPC)
	{
		drawObjectForTerminal();
		return;
	}

	int		iObjectCount	=	0;		//	소트해야할 객체 수, 일단은 오브젝트와 캐릭터
	int		value,index,mx,my,x,y;
	int		minY	=	max(m_pos.my - dMAP_MARGIN,0);
	int		minX	=	max(m_pos.mx - dMAP_MARGIN,0);
	int		maxY	=	min(m_pos.my + m_iTPH + dMAP_MARGIN,m_iHeight-1);
	int		maxX	=	min(m_pos.mx + m_iTPW + dMAP_MARGIN,m_iWidth -1);

	int		bx		=	(minX << dTILE_XSIZE_SHIFT) - m_pos.x + dTILE_XS/2;
	int		by		=	(minY << dTILE_YSIZE_SHIFT) - m_pos.y + dTILE_YS/2;

	x	=	bx;
	y	=	by;

	//	오브젝트를 리스트에 넣음

	iObjectCount	=	m_object.getViewObjects(l_aSortObject,&g_rectViewPort);			//	뷰포트 안에 오브젝트들을 구한다.
	iObjectCount	+=	g_am.getViewActors(&l_aSortObject[iObjectCount],&g_rectViewPort);		//	뷰포트 안에 캐릭터들을 구한다.
	iObjectCount	+=	g_gpManager.getViewPost(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	뷰포트 안에 포스트를 구한다.
	iObjectCount	+=	g_pitchmanShop.getViewShop(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	뷰포트 안에 포스트를 구한다.
	iObjectCount	+=	g_portal.getViewPortal(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	뷰포트 안에 포스트를 구한다.
	iObjectCount	+=	g_standEffect.getViewEffect(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	뷰포트 안에 이펙트
	iObjectCount	+=	g_trapManager.getViewTraps(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	정렬해서 출력해야할 함정들
	iObjectCount	+=	g_arcaManager.getViewArcas(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	정렬해서 출력해야할 함정들
	iObjectCount	+=	g_bannerManager.getViewBanners(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	정렬해서 출력해야할 함정들

	qsort((void *)l_aSortObject,iObjectCount,sizeof(cINDEX),(int(*)(const void *,const void *))Compare);

	l_aSortObject[iObjectCount].Value	=	0x7fffffff;				//	마지막 인덱스에 최대 값을 넣어 준다.

	//	캐릭터를 리스트에 넣음

	int	iObjectCounter	=	0;

	for (my = minY; my < maxY; my++)
	{
		index	=	my	*	m_iWidth;

		for (mx = minX; mx < maxX; mx++)
		{
			value	=	m_aInfo[index+mx];

			if	(value & dMASK_OBJECT)
				m_object.putFixedObject(value - dMASK_OBJECT,x,y,TRUE);
			else
			if	(value & dMASK_MISSLIE_BLOCK_OBJECT)
				m_object.putFixedObject(value - dMASK_MISSLIE_BLOCK_OBJECT,x,y,TRUE);

			x	+=	dTILE_XS;
		}

		x		=	bx;
		y		+=	dTILE_YS;
	}

	x	=	bx;
	y	=	by;

	for (my = minY; my < maxY; my++)
	{
		index	=	my	*	m_iWidth;

		while(l_aSortObject[iObjectCounter].Value <=	y + m_pos.y)
		{
			int	objectIndex	=	l_aSortObject[iObjectCounter].Index;

			if	(objectIndex >= dSORT_OBJECT_BORDER_BANNER)		//	캐릭터다
			{	
				objectIndex	-=	dSORT_OBJECT_BORDER_BANNER;

				g_bannerManager.drawBanner(objectIndex,m_pos.x,m_pos.y);
			}
			else							//	바닥에 소트 되는 오브젝트
			if	(objectIndex >= dSORT_OBJECT_BORDER_ARCA)		//	캐릭터다
			{	
				objectIndex	-=	dSORT_OBJECT_BORDER_ARCA;

				g_arcaManager.drawArca(objectIndex,m_pos.x,m_pos.y);
			}
			else							//	바닥에 소트 되는 오브젝트
			if	(objectIndex >= dBORDER_TRAP)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_TRAP;

				g_trapManager.drawTrap(objectIndex,m_pos.x,m_pos.y);
			}
			else							//	바닥에 소트 되는 오브젝트
			if	(objectIndex >= dBORDER_EFFECT)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_EFFECT;

				g_standEffect.put(objectIndex);
			}
			else							//	바닥에 소트 되는 오브젝트
			if	(objectIndex >= dBORDER_PORTAL)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_PORTAL;

				g_portal.draw(objectIndex);
			}
			else							//	바닥에 소트 되는 오브젝트
			if	(objectIndex >= dBORDER_PITCHMAN_SHOP)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_PITCHMAN_SHOP;

				g_pitchmanShop.draw(objectIndex);
			}
			else							//	바닥에 소트 되는 오브젝트
			if	(objectIndex >= dBORDER_GUARDIAN_POST)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_GUARDIAN_POST;

				g_gpManager.m_aPost[objectIndex].put();
			}
			else							//	바닥에 소트 되는 오브젝트
			if	(objectIndex >= dBORDER_ACTOR)		//	캐릭터다
			{	objectIndex	-=	dBORDER_ACTOR;

				g_aActor[objectIndex].put();
			}
			else							//	바닥에 소트 되는 오브젝트
			{
				m_object.putTinyObject(objectIndex,m_pos.x,m_pos.y);
			}

			iObjectCounter++;
		}

		for (mx = minX; mx < maxX; mx++)
		{
			value	=	m_aInfo[index+mx];

			if	(value & dMASK_OBJECT)
				m_object.putFixedObject(value - dMASK_OBJECT,x,y,FALSE,TRUE,FALSE);
			else
			if	(value & dMASK_MISSLIE_BLOCK_OBJECT)
				m_object.putFixedObject(value - dMASK_MISSLIE_BLOCK_OBJECT,x,y,FALSE,TRUE,FALSE);

			x	+=	dTILE_XS;
		}

		x		=	bx;

		for (mx = minX; mx < maxX; mx++)
		{
			value	=	m_aInfo[index+mx];

			if	(value & dMASK_OBJECT)
				m_object.putFixedObject(value - dMASK_OBJECT,x,y,FALSE,FALSE,TRUE);
			else
			if	(value & dMASK_MISSLIE_BLOCK_OBJECT)
				m_object.putFixedObject(value - dMASK_MISSLIE_BLOCK_OBJECT,x,y,FALSE,FALSE,TRUE);
			else
			if	(value & dMASK_BUILDING)
				m_object.putBuilding(value - dMASK_BUILDING,x,y);

			x	+=	dTILE_XS;
		}

		x		=	bx;
		y		+=	dTILE_YS;
	}

	m_object.drawFloatObject(m_pos.x,m_pos.y);

	int		i;

	for (i= 0; i< dOWN_PET_COUNT; i++)
	{
		CActor	*lpPet;

		lpPet	=	g_hero.getPetActor(i);

		if (lpPet)
			lpPet->addPaint();
	}

	for (i= 0; i< dOWN_SUMMON_BEAST_COUNT; i++)
	{
		CActor	*lpSummonBeast;

		lpSummonBeast	=	g_hero.getSummonBeastActor(i);

		if (lpSummonBeast)
			lpSummonBeast->addPaint();
	}

	iObjectCount	+=	g_portal.getViewPortal(&l_aSortObject[iObjectCount],&g_rectViewPort);
	m_area.drawGateWay(m_pos.x,m_pos.y,s_iFrameCounter,TRUE);

	for (i= 0; i< iObjectCount; i++)
	{
		int	objectIndex	=	l_aSortObject[i].Index;

		if (objectIndex >= dBORDER_ACTOR && objectIndex < dBORDER_GUARDIAN_POST)		//	캐릭터다
		{	
			objectIndex	-=	dBORDER_ACTOR;

			CActor	*lpActor	=	&g_aActor[objectIndex];

			if	(lpActor->isRequireAddPaint())
				lpActor->addPaint();

			lpActor->drawPetMark();
		}
	}

	DrawEventObjectName();		//09.08.11
	g_fieldItem.addPaint(m_pos.x,m_pos.y);
}

//
//	맵위에 올라가는 각종 오브젝트들을 찍어준다.(캐릭터,오브젝트 어쩌구... 저쩌구..)
void
cMAP::drawObjectHalfSize()
{
	if	(g_bIsTerminalPC)
	{
		drawObjectForTerminal();
		return;
	}

	int		iObjectCount	=	0;		//	소트해야할 객체 수, 일단은 오브젝트와 캐릭터
	int		value,index,mx,my,x,y;
	int		minY	=	max(m_posView.my - dMAP_MARGIN*2,0);
	int		minX	=	max(m_posView.mx - dMAP_MARGIN*2,0);
	int		maxY	=	min(m_posView.my + m_iTPH + dMAP_MARGIN*2,m_iHeight-1);
	int		maxX	=	min(m_posView.mx + m_iTPW + dMAP_MARGIN*2,m_iWidth -1);

	int		bx		=	(minX << dTILE_XSIZE_SHIFT_H) - m_pos.x + dTILE_XS_H/2;
	int		by		=	(minY << dTILE_YSIZE_SHIFT_H) - m_pos.y + dTILE_YS_H/2;

	x	=	bx;
	y	=	by;

	int		iVerticalBorder	=	0;

	{
		int		iMinY		=	max(m_pos.my - dMAP_MARGIN,0);
		iVerticalBorder		=	(iMinY << dTILE_YSIZE_SHIFT) - m_pos.y + dTILE_YS/2;
	}

	//	오브젝트를 리스트에 넣음

	iObjectCount	=	m_object.getViewObjects(l_aSortObject,&g_rectViewPort);			//	뷰포트 안에 오브젝트들을 구한다.
	iObjectCount	+=	g_am.getViewActors(&l_aSortObject[iObjectCount],&g_rectViewPort);		//	뷰포트 안에 캐릭터들을 구한다.
	iObjectCount	+=	g_gpManager.getViewPost(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	뷰포트 안에 포스트를 구한다.
	iObjectCount	+=	g_pitchmanShop.getViewShop(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	뷰포트 안에 포스트를 구한다.
	iObjectCount	+=	g_portal.getViewPortal(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	뷰포트 안에 포스트를 구한다.
	iObjectCount	+=	g_standEffect.getViewEffect(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	뷰포트 안에 이펙트
	iObjectCount	+=	g_trapManager.getViewTraps(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	정렬해서 출력해야할 함정들
	iObjectCount	+=	g_arcaManager.getViewArcas(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	정렬해서 출력해야할 함정들
	iObjectCount	+=	g_bannerManager.getViewBanners(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	정렬해서 출력해야할 함정들

	qsort((void *)l_aSortObject,iObjectCount,sizeof(cINDEX),(int(*)(const void *,const void *))Compare);

	l_aSortObject[iObjectCount].Value	=	0x7fffffff;				//	마지막 인덱스에 최대 값을 넣어 준다.

	//	캐릭터를 리스트에 넣음

	int	iObjectCounter	=	0;
	int	iRealY			=	by;

	for (my = minY; my < maxY; my++)
	{
		index	=	my	*	m_iWidth;
		y		=	(my*dTILE_YS-m_posView.y*2)/2+dTILE_YS_H/2;

		while(GetScaledYPos(l_aSortObject[iObjectCounter].Value)	<=	y)
		{	
			int	objectIndex	=	l_aSortObject[iObjectCounter].Index;

			if (objectIndex >= dSORT_OBJECT_BORDER_BANNER)		//	캐릭터다
			{	
				objectIndex	-=	dSORT_OBJECT_BORDER_BANNER;

				g_bannerManager.drawBanner(objectIndex,m_pos.x,m_pos.y);
			}
			else							//	바닥에 소트 되는 오브젝트
			if (objectIndex >= dSORT_OBJECT_BORDER_ARCA)		//	캐릭터다
			{	
				objectIndex	-=	dSORT_OBJECT_BORDER_ARCA;

				g_arcaManager.drawArca(objectIndex,m_pos.x,m_pos.y);
			}
			else							//	바닥에 소트 되는 오브젝트
			if (objectIndex >= dBORDER_TRAP)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_TRAP;

				g_trapManager.drawTrap(objectIndex,m_pos.x,m_pos.y);
			}
			else							//	바닥에 소트 되는 오브젝트
			if (objectIndex >= dBORDER_EFFECT)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_EFFECT;

				g_standEffect.put(objectIndex);
			}
			else							//	바닥에 소트 되는 오브젝트
			if (objectIndex >= dBORDER_PORTAL)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_PORTAL;

				g_portal.draw(objectIndex);
			}
			else							//	바닥에 소트 되는 오브젝트
			if (objectIndex >= dBORDER_PITCHMAN_SHOP)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_PITCHMAN_SHOP;

				g_pitchmanShop.draw(objectIndex);
			}
			else							//	바닥에 소트 되는 오브젝트
			if (objectIndex >= dBORDER_GUARDIAN_POST)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_GUARDIAN_POST;

				g_gpManager.m_aPost[objectIndex].put();
			}
			else							//	바닥에 소트 되는 오브젝트
			if (objectIndex >= dBORDER_ACTOR)		//	캐릭터다
			{
				objectIndex	-=	dBORDER_ACTOR;

				g_aActor[objectIndex].put();
			}
			else							//	바닥에 소트 되는 오브젝트
			{
				m_object.putTinyObject(objectIndex,m_pos.x,m_pos.y);
			}

			iObjectCounter++;
		}

		iVerticalBorder	+=	dTILE_YS;
		iRealY			+=	dTILE_YS;

		for (mx = minX; mx < maxX; mx++)
		{	
			value	=	m_aInfo[index+mx];

			x		=	(mx*dTILE_XS-m_posView.x*2)/2+dTILE_XS_H/2;

			if (value & dMASK_OBJECT)
			{
				m_object.putFixedObject(value - dMASK_OBJECT,x,y);
			}
			else
			if (value & dMASK_MISSLIE_BLOCK_OBJECT)
			{
				m_object.putFixedObject(value - dMASK_MISSLIE_BLOCK_OBJECT,x,y);
			}
			else
			if (value & dMASK_BUILDING)
			{
				m_object.putBuilding(value - dMASK_BUILDING,x,y);
			}
		}
	}

	m_object.drawFloatObject(m_pos.x,m_pos.y);

	int		i;

	for (i= 0; i< dOWN_PET_COUNT; i++)
	{
		CActor	*lpPet;

		lpPet	=	g_hero.getPetActor(i);

		if (lpPet)
			lpPet->addPaint();
	}

	for (i= 0; i< dOWN_SUMMON_BEAST_COUNT; i++)
	{
		CActor	*lpSummonBeast;

		lpSummonBeast	=	g_hero.getSummonBeastActor(i);

		if (lpSummonBeast)
			lpSummonBeast->addPaint();
	}

	for (i= 0; i< iObjectCount; i++)
	{
		int	objectIndex	=	l_aSortObject[i].Index;

		if (objectIndex >= dBORDER_ACTOR && objectIndex < dBORDER_GUARDIAN_POST)		//	캐릭터다
		{	
			objectIndex	-=	dBORDER_ACTOR;

			CActor	*lpActor	=	&g_aActor[objectIndex];

			if (lpActor->isRequireAddPaint())
				lpActor->addPaint();

			lpActor->drawPetMark();
		}
	}

	g_fieldItem.addPaint(m_pos.x,m_pos.y);
}

//
//	맵위에 올라가는 각종 오브젝트들을 찍어준다.(캐릭터,오브젝트 어쩌구... 저쩌구..)
void
cMAP::drawObjectForTerminal()
{
	int		iObjectCount	=	0;		//	소트해야할 객체 수, 일단은 오브젝트와 캐릭터
	int		index,my,x,y;
	int		minY	=	max(m_pos.my - dMAP_MARGIN,0);
	int		minX	=	max(m_pos.mx - dMAP_MARGIN,0);
	int		maxY	=	min(m_pos.my + m_iTPH + dMAP_MARGIN,m_iHeight-1);
	int		maxX	=	min(m_pos.mx + m_iTPW + dMAP_MARGIN,m_iWidth -1);

	int		bx		=	(minX << dTILE_XSIZE_SHIFT) - m_pos.x + dTILE_XS/2;
	int		by		=	(minY << dTILE_YSIZE_SHIFT) - m_pos.y + dTILE_YS/2;

	x	=	bx;
	y	=	by;

	//	오브젝트를 리스트에 넣음

	iObjectCount		+=	g_am.getViewActors(&l_aSortObject[iObjectCount],&g_rectViewPort);		//	뷰포트 안에 캐릭터들을 구한다.
	iObjectCount		+=	g_gpManager.getViewPost(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	뷰포트 안에 포스트를 구한다.
	iObjectCount		+=	g_pitchmanShop.getViewShop(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	뷰포트 안에 포스트를 구한다.
	iObjectCount		+=	g_portal.getViewPortal(&l_aSortObject[iObjectCount],&g_rectViewPort);	//	뷰포트 안에 포스트를 구한다.

	qsort((void *)l_aSortObject,iObjectCount,sizeof(cINDEX),(int(*)(const void *,const void *))Compare);

	l_aSortObject[iObjectCount].Value	=	0x7fffffff;				//	마지막 인덱스에 최대 값을 넣어 준다.

	//	캐릭터를 리스트에 넣음

	int	iObjectCounter	=	0;

	for (my = minY; my < maxY; my++)
	{	index	=	my	*	m_iWidth;
	
		while(l_aSortObject[iObjectCounter].Value <=	y + m_pos.y)
		{	
			int	objectIndex	=	l_aSortObject[iObjectCounter].Index;

			if (objectIndex >= dBORDER_PORTAL)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_PORTAL;

				g_portal.draw(objectIndex);
			}
			else							//	바닥에 소트 되는 오브젝트
			if (objectIndex >= dBORDER_PITCHMAN_SHOP)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_PITCHMAN_SHOP;

				g_pitchmanShop.draw(objectIndex);
			}
			else							//	바닥에 소트 되는 오브젝트
			if (objectIndex >= dBORDER_GUARDIAN_POST)		//	캐릭터다
			{	
				objectIndex	-=	dBORDER_GUARDIAN_POST;

				g_gpManager.m_aPost[objectIndex].put();
			}
			else							//	바닥에 소트 되는 오브젝트
			if (objectIndex >= dBORDER_ACTOR)		//	캐릭터다
			{	objectIndex	-=	dBORDER_ACTOR;

				g_aActor[objectIndex].put();
			}
			else							//	바닥에 소트 되는 오브젝트
			{	
				m_object.putTinyObject(objectIndex,m_pos.x,m_pos.y);
			}

			iObjectCounter++;
		}

		x		=	bx;
		y		+=	dTILE_YS;
	}

	int		i;

	for (i= 0; i< dOWN_PET_COUNT; i++)
	{
		CActor	*lpPet;

		lpPet	=	g_hero.getPetActor(i);

		if (lpPet)
			lpPet->addPaint();
	}

	for (i= 0; i< dOWN_SUMMON_BEAST_COUNT; i++)
	{
		CActor	*lpSummonBeast;

		lpSummonBeast	=	g_hero.getSummonBeastActor(i);

		if (lpSummonBeast)
			lpSummonBeast->addPaint();
	}

	for (i= 0; i< iObjectCount; i++)
	{
		int	objectIndex	=	l_aSortObject[i].Index;

		if (objectIndex >= dBORDER_ACTOR && objectIndex < dBORDER_GUARDIAN_POST)		//	캐릭터다
		{	
			objectIndex	-=	dBORDER_ACTOR;

			CActor	*lpActor	=	&g_aActor[objectIndex];
			BOOL	bIsHideTile	=	g_map.m_aMap[lpActor->m_pos.mx+lpActor->m_pos.my*g_map.m_iWidth] & 0x4000;

			if (lpActor->isControlMonster())
				continue;

			if (bIsHideTile || lpActor->isVisibleName() || lpActor->m_wTalkTime)
				lpActor->addPaint();

			lpActor->drawPetMark();
		}
	}

	g_fieldItem.addPaint(m_pos.x,m_pos.y);
}

//
//	_iDestMx,_iDestMy으로 이동하는데 막히냐?
BOOL
cMAP::isBlockedWay(int _iMx,int _iMy,int _iDestMx,int _iDestMy)
{
	int	iAddress	=	_iDestMx+_iDestMy*g_map.m_iWidth;
	int	iCheckYValue=	-m_iWidth;
	int	iCheckXValue=	-1;

	if (_iDestMx	>=	g_map.m_iWidth	-1	||
		_iDestMy	>=	g_map.m_iHeight	-1	||
		_iDestMx	<	0	||	_iDestMy	<	0)
		return	TRUE;

	if	(m_aBlockInfo[iAddress])
		return	TRUE;
	if	(_iMx	==	_iDestMx	||	_iMy	==	_iDestMy		)
		return	FALSE;

//	2칸 이상 이동하는지 체크
	{
		int	iDx	=	_iMx - _iDestMx;
		int	iDy	=	_iMy - _iDestMy;

		if	(iDx	<	0)	iDx	=	-iDx;
		if	(iDy	<	0)	iDy	=	-iDy;

		if	(iDx	+	iDy	>	2)
			return	TRUE;
	}

	if	(_iMx	>	_iDestMx)
		iCheckXValue=	1;

	if	(_iMy	>	_iDestMy)
		iCheckYValue=	-iCheckYValue;

	if	(g_bIsTestServer)
	{
		if	(m_aBlockInfo[iAddress+iCheckYValue	]	&&	m_aBlockInfo[iAddress+iCheckXValue	])
			return	TRUE;
	}
	else
	{
		if	(m_aBlockInfo[iAddress+iCheckYValue	]	||	m_aBlockInfo[iAddress+iCheckXValue	])
			return	TRUE;
	}

	return	FALSE;
}

BOOL
cMAP::checkMissileBlockedLine(int _iX,int _iY,int _iDestX,int _iDestY)
{
	CPos	aRoad[100],posLast;

	if	(_iX < 0 || _iY < 0 || _iDestX < 0 || _iDestY < 0)
		return	TRUE;

	int		iDestMX	=	_iDestX/dTILE_XS;
	int		iDestMY	=	_iDestY/dTILE_YS;

	while(1)
	{
		int	iCount	=	getRoad(_iX,_iY,_iDestX,_iDestY,aRoad,4,100,&posLast);

		if	(iCount	<	1)
			return	FALSE;

		if	(iCount	==	1)
			return	isBlocked(aRoad[0].x,aRoad[0].y);

		for (int i=iCount-1;i;)
		{
			--i;

			if	(isBlockedWay(aRoad[i].x,aRoad[i].y,aRoad[i+1].x,aRoad[i+1].y))
				return	TRUE;
		}

		_iX	=	posLast.x;
		_iY	=	posLast.y;

		if	(_iX	==	iDestMX	&&	_iY	==	iDestMY)
			return	FALSE;
	}

	return	FALSE;
}

//
//	공격할때 막히냐?
BOOL
cMAP::isMissileBlockedLine(int _iX,int _iY,int _iDestX,int _iDestY)
{
	if	(checkMissileBlockedLine(_iDestX,_iDestY,_iX,_iY))
		return	TRUE;

	return	checkMissileBlockedLine(_iX,_iY,_iDestX,_iDestY);
}

void
cMAP::makeBlock(int _iX1,int _iY1,int _iX2,int _iY2)
{
	CPos	aRoad[100];

	int		iNodeCount	=	GetLinerPath(_iX1,_iY1,_iX2,_iY2,aRoad,100);

	for(int i=iNodeCount;i;)
	{
		--i;

		int	iAddress	=	aRoad[i].x+aRoad[i].y*m_iWidth;

		m_aBlockInfo[iAddress]	=	1;
	}
}

void
cMAP::removeBlock(int _iX1,int _iY1,int _iX2,int _iY2)
{
	CPos	aRoad[100];

	int		iNodeCount	=	GetLinerPath(_iX1,_iY1,_iX2,_iY2,aRoad,100);

	for(int i=iNodeCount;i;)
	{
		--i;

		int	iAddress	=	aRoad[i].x+aRoad[i].y*m_iWidth;

		m_aBlockInfo[iAddress]	=	0;
	}
}

//
//	_iX1,_iY1에서 _iX2,_iY2까지 픽셸 단위로 길을 찾는다.
int
cMAP::getRoad(int _iX1,int _iY1,int _iX2,int _iY2,CPos *_lpRoad,int _iTerm,int _iLimit,CPos *_lpPosLast)
{
	// 기울기값
	int		i;
	int		iRoadCount	=	0;		//	경유하는 포인트의 수
	int		iWidth,iHeight,iDeltaX,iDeltaY;
	CPos	posM;

    if (_iX1 > _iX2)
		iWidth	=	_iX1	-	_iX2,iDeltaX		=	-1;
	else
		iWidth	=	_iX2	-	_iX1,iDeltaX		=	1;

    if (_iY1 > _iY2)
		iHeight	=	_iY1	-	_iY2,iDeltaY		=	-1;
	else
		iHeight	=	_iY2	-	_iY1,iDeltaY		=	1;

	_lpRoad[0].x	=	_iX1/dTILE_XS;
	_lpRoad[0].y	=	_iY1/dTILE_YS;

	int	dPr,dPru,P;

	posM.Set(_lpRoad[0].x,_lpRoad[0].y);

	iRoadCount++;

	if	(_iX2	==	_iX1)
	{
		for (i=0;i<iHeight;i++)
		{
			if	(i%_iTerm	==	0)
			{
				if	(posM.x	!=	_iX1/dTILE_XS	||	posM.y	!=	_iY1/dTILE_YS)
				{
					posM.x	=	_iX1/dTILE_XS;
					posM.y	=	_iY1/dTILE_YS;

					_lpRoad[iRoadCount].x	=	posM.x;
					_lpRoad[iRoadCount].y	=	posM.y;
					iRoadCount++;

					if	(iRoadCount	>=	_iLimit)
					{
						_lpPosLast->Set(_iX1,_iY1);

						return	iRoadCount;
					}
				}
			}
			_iY1	+=	iDeltaY;
		}

		goto	label_success_return;
	}

	if (_iY2 == _iY1)
	{
		for (i=0;i<iWidth;i++)
		{
			if (i%_iTerm	==	0)
			{
				if	(posM.x	!=	_iX1/dTILE_XS	||	posM.y	!=	_iY1/dTILE_YS)
				{
					posM.x	=	_iX1/dTILE_XS;
					posM.y	=	_iY1/dTILE_YS;

					_lpRoad[iRoadCount].x	=	posM.x;
					_lpRoad[iRoadCount].y	=	posM.y;
					iRoadCount++;

					if	(iRoadCount	>=	_iLimit)
					{
						_lpPosLast->Set(_iX1,_iY1);

						return	iRoadCount;
					}

				}
			}
			_iX1	+=	iDeltaX;
		}

		goto	label_success_return;
	}

    if (iWidth >= iHeight)
    {           
        dPr 	= iHeight<<1;   						// amount to increment decision if right is chosen (always)
        dPru 	= dPr - (iWidth<<1);				// amount to increment decision if up is chosen
        P 		= dPr - iWidth;						// decision variable start value
        
        for (; iWidth>=0; iWidth--)						// process each point in the line one at a time (just use iWidth)
        {
			if (iWidth % _iTerm	==	0)
			{
				if (posM.x	!=	_iX1/dTILE_XS	||	posM.y	!=	_iY1/dTILE_YS)
				{
					posM.x	=	_iX1/dTILE_XS;
					posM.y	=	_iY1/dTILE_YS;

					_lpRoad[iRoadCount].x	=	posM.x;
					_lpRoad[iRoadCount].y	=	posM.y;
					iRoadCount++;

					if (iRoadCount	>=	_iLimit)
					{
						_lpPosLast->Set(_iX1,_iY1);

						return	iRoadCount;
					}
				}
			}

            if (P > 0)                              // is the pixel going right AND up?
            {	
				_iX1	+=iDeltaX;					// increment independent variable
                _iY1	+=iDeltaY; 					// increment dependent variable
                P		+=dPru;						// increment decision (for up)
            }
            else									// is the pixel just going right?
            {   
				_iX1	+=iDeltaX;					// increment independent variable
                P		+=dPr;						// increment decision (for right)
            }
        }
    }
    else
    {
        dPr 	= iWidth<<1;   							// amount to increment decision if right is chosen (always)
        dPru 	= dPr - (iHeight<<1);    				// amount to increment decision if up is chosen
        P 		= dPr - iHeight;							// decision variable start value
        
        for (; iHeight>=0; iHeight--)							// process each point in the line one at a time (just use iHeight)
        {
			if ((iHeight % _iTerm)	==	0)
			{
				if (posM.x	!=	_iX1/dTILE_XS	||	posM.y	!=	_iY1/dTILE_YS)
				{
					posM.x	=	_iX1/dTILE_XS;
					posM.y	=	_iY1/dTILE_YS;

					_lpRoad[iRoadCount].x	=	posM.x;
					_lpRoad[iRoadCount].y	=	posM.y;
					iRoadCount++;

					if (iRoadCount	>=	_iLimit)	
					{
						_lpPosLast->Set(_iX1,_iY1);

						return	iRoadCount;
					}
				}
			}

            if (P > 0)                              // is the pixel going up AND right?
            { 
                _iX1	+=iDeltaX; 					// increment dependent variable
                _iY1	+=iDeltaY;					// increment independent variable
                P	+=dPru;						// increment decision (for up)
            }
            else									// is the pixel just going up?
            {
                _iY1	+=iDeltaY;					// increment independent variable
                P	+=dPr;						// increment decision (for right)
            }
        }		
   }		

label_success_return:

	if	(iRoadCount && iRoadCount < _iLimit)
	{
		CPos	*lpLastPos	=	&_lpRoad[iRoadCount-1];

		if	(	lpLastPos->x	!=	_iX2/dTILE_XS	||	lpLastPos->y	!=	_iY2/dTILE_YS)
		{
			_lpRoad[iRoadCount].x	=	_iX2/dTILE_XS;
			_lpRoad[iRoadCount].y	=	_iY2/dTILE_YS;

			iRoadCount++;
		}
	}

	_lpPosLast->Set(_iX2,_iY2);
	
	return	iRoadCount;
}

int
cMAP::getCorrectMagicResistanceLevel(int _iValue)
{
	int	l_aRange[]	=
	{
		1,10,
		11,20,
		21,30,
		31,40,
		41,50,
		51,60,
		61,70,
		71,90,
		91,120,
		121,150,
		151,200,
	};

	for (int i=0;i<10;i++)
		if (_iValue >= l_aRange[i*2] && _iValue <= l_aRange[i*2+1])
			return	i+1;

	return	0;
}

int
cMAP::getCorrectFireResistanceLevel()
{
	return	getCorrectMagicResistanceLevel(m_sCorrectFireResistance);
}

int
cMAP::getCorrectWaterResistanceLevel()
{
	return	getCorrectMagicResistanceLevel(m_sCorrectWaterResistance);
}

int
cMAP::getCorrectWindResistanceLevel()
{
	return	getCorrectMagicResistanceLevel(m_sCorrectWindResistance);
}

int
cMAP::getCorrectEarthResistanceLevel()
{
	return	getCorrectMagicResistanceLevel(m_sCorrectEarthResistance);
}

int
cMAP::getCorrectLightResistanceLevel()
{
	return	getCorrectMagicResistanceLevel(m_sCorrectLightResistance);
}

int
cMAP::getCorrectDarkResistanceLevel()
{
	return	getCorrectMagicResistanceLevel(m_sCorrectDarkResistance);
}

char*
cMAP::GetFieldName(int _iIndex)
{
	if	(_iIndex	<	0	||	_iIndex	>=	dMAX_MAP_COUNT	||	g_aMapSimpleInfo[_iIndex].m_wSerial	==	0xffff)
		return	dMSG_SECRET_DUNGEON;

	return	g_aMapSimpleInfo[_iIndex].m_strName;
}

char*
cMAP::GetFieldName(char *_lpstrFileName)
{
	int		iIndex		=	atoi(_lpstrFileName+1);

	return	GetFieldName(iIndex);
}

BOOL
cMAP::GetFieldInfo(cMAP_INFO *_lpMapInfo,char *_lpstrFileName,cNUX *_lpNux)
{
	cFILE	*pFile;

	if (_lpNux)	pFile	=	_lpNux->Open(_lpstrFileName);
	else
	{
		pFile	=	new cFILE();
		if (!pFile->Open(_lpstrFileName,"rb"))
		{
			KILL(pFile);

			return	FALSE;
		}
	}

	int		iFileSize;

//	헤더 읽기
	{
		pFile->Read(&iFileSize	,4);	//	화일 사이즈
		pFile->Read(&iFileSize	,4);	//	텍스트 데이터의 위치
		pFile->Seek(60);						//	화일 헤더 건너뛰기..
	}	//	헤더 읽기

//	맵 정보 읽어옴	
	pFile->Read(_lpMapInfo,sizeof(cMAP_INFO));		//	cMAP_INFO 인자만 읽어 들인다. 주의해서 사용할것..

	KILL(pFile);

	return	TRUE;
}

void
cMAP::readCloseMapList()
{
	cFILE	file;

#ifdef	_IS_DEV_CLIENT
#ifdef	_FOR_KOREA
	char	*strCloseMapListScriptFile[]	=	
	{
		"data/Scripter/closeMapListForChina.txt",
		"data/Scripter/closeMapListForJapan.txt",
		"data/Scripter/closeMapList.txt",
		"end",
	};
	char	*strCloseMapListFile[]	=
	{
		"../Red stone for China/data/closeMapListForChina.dat",
		"../Red stone for Japan/data/closeMapListForJapan.dat",
		"data/closeMapList.dat"
	};

	
	CDecoder	decoder;
	int			iLanguage	=	0;

	while(1 && g_bIsDevPC)
	{
		m_iCloseMapCount	=	0;

		if	(STRICMP(strCloseMapListScriptFile[iLanguage],"end")==0)
			break;

		if	(decoder.Upload(strCloseMapListScriptFile[iLanguage]))
		{
			while(1)
			{
				BOOL	bIsCorrect;

				int		iField	=	decoder.getNumber(&bIsCorrect);

				if (bIsCorrect != 1)
					break;

				m_awCloseMapList[m_iCloseMapCount++]	=	iField;
			}
		}

		if	(file.Open(strCloseMapListFile[iLanguage],"wb"))
		{
			file.Write(&m_iCloseMapCount,4,1);
			file.Write(m_awCloseMapList,m_iCloseMapCount*2);

			file.Close();
		}

		iLanguage++;
	}
#endif
#endif


#ifdef	_FOR_CHINA
	char	*lpstrClosemapListFile		=	"data/closeMapListForChina.dat";
#elif	_FOR_JAPAN
	char	*lpstrClosemapListFile		=	"data/closeMapListForJapan.dat";
#else
	char	*lpstrClosemapListFile		=	"data/closeMapList.dat";
#endif

	m_iCloseMapCount	=	0;

	if (file.Open(lpstrClosemapListFile,"rb"))
	{
		file.Read(&m_iCloseMapCount,4,1);
		file.Read(m_awCloseMapList,m_iCloseMapCount*2);

		file.Close();
	}

}

BOOL
cMAP::Init(cNUX *_lpNux,BOOL _bIsWantExportList)
{
	//	필요 없는 파일 삭제
	{
		char	*strDeleteMap[]	=	
		{
			"[503]weapon_test.rmd",
			"[999]Guild_Ruined Sbain Fortress_0.rmd",
			"[173]T18_L02.rmd",
			"[damel]000.rmd",
			"[990]Guild_square_0.rmd.tga",
			"[991]Guild_Desert_0.rmd.tga",
			"[992]Guild_Mountains_0.rmd.tga",
			"[993]Guild_Cross counter_0.rmd.tga",
			"[994]Guild_Ruined Sbain Fortress_0.rmd.tga",
			"[996]Guild_Desert_1.rmd.tga",
			"[997]Guild_Mountains_1.rmd.tga",
			"[998]Guild_Cross counter_1.rmd.tga",
			"[999]Guild_Ruined Sbain Fortress_1.rmd.tga",
			"end"
		};

		for (int i=0;;i++)
		{
			if	(strcmp(strDeleteMap[i],"end")==0)
				break;

			char	strFile[512];

			sprintf(strFile,"data/scenario/red stone/map/%s",strDeleteMap[i]);

			if	(!IsFile(strFile))
				continue;

			SetFileAttributes(strFile,FILE_ATTRIBUTE_ARCHIVE);

			DeleteFile(strFile);
		}
	}
	
	{
		char	*strOrgMap[]	=	
		{
			"[990]Guild_square_0.rmd",
			"[991]Guild_Desert_0.rmd",
			"[992]Guild_Mountains_0.rmd",
			"[993]Guild_Cross counter_0.rmd",
			"[994]Guild_Ruined Sbain Fortress_0.rmd",
			"[995]Guild_square_1.rmd",
			"[996]Guild_Desert_1.rmd",
			"[997]Guild_Mountains_1.rmd",
			"[998]Guild_Cross counter_1.rmd",
			"[999]Guild_Ruined Sbain Fortress_1.rmd",
			"end"
		};

		char	*strChangeMap[]	=	
		{
			"[990]Guild_vs_00.rmd",
			"[991]Guild_vs_01.rmd",
			"[992]Guild_vs_02.rmd",
			"[993]Guild_vs_03.rmd",
			"[994]Guild_vs_04.rmd",
			"[995]Guild_vs_05.rmd",
			"[996]Guild_vs_06.rmd",
			"[997]Guild_vs_07.rmd",
			"[998]Guild_vs_08.rmd",
			"[999]Guild_vs_09.rmd",
			"end"
		};

		for (int i=0;;i++)
		{
			if	(strcmp(strChangeMap[i],"end")==0)
				break;

			char	strOrgFile[512],strChangeFile[512];

			{
				sprintf(strOrgFile,"data/scenario/red stone/map/%s",strOrgMap[i]);
				sprintf(strChangeFile,"data/scenario/red stone/map/%s",strChangeMap[i]);

				if	(!IsFile(strOrgFile))
					continue;

				SetFileAttributes(strOrgFile,FILE_ATTRIBUTE_ARCHIVE);

				if	(IsFile(strChangeFile))
				{
					DeleteFile(strOrgFile);
					continue;
				}
				else
					MoveFile(strOrgFile,strChangeFile);
			}

			{
				sprintf(strOrgFile,"data/scenario/red stone/map/%s.tga",strOrgMap[i]);
				sprintf(strChangeFile,"data/scenario/red stone/map/%s.tga",strChangeMap[i]);

				SetFileAttributes(strOrgFile,FILE_ATTRIBUTE_ARCHIVE);

				if	(IsFile(strChangeFile))
				{
					DeleteFile(strOrgFile);
					continue;
				}
				else
					MoveFile(strOrgFile,strChangeFile);
			}
		}
	}

	DeleteFile("data/scenario/red stone/map/[173]T18_L02.rmd");
	cFILE	file;
	int		i;

	cFILE	logFile;

#ifndef _IS_DEV_CLIENT

	if (!file.Open("data/mapList.dat","rb"))
		return	ERRMSG("error!!","can not find 'data/mapList.dat'");

	file.Read(g_aMapSimpleInfo,sizeof(g_aMapSimpleInfo));
	file.Close();

	if (!file.Open("data/mapLocationInfo.dat","rb"))
		return	ERRMSG("error!!","can not find 'data/mapLocationInfo.dat'");

	file.Read(g_aMapLocationInfo,sizeof(g_aMapLocationInfo));
	file.Close();

	return	TRUE;
#endif

	if	(!_bIsWantExportList)
	{
		if	(!file.Open("data/mapList.dat","rb"))
			return	ERRMSG("error!!","can not find 'data/mapList.dat'");

		file.Read(g_aMapSimpleInfo,sizeof(g_aMapSimpleInfo));
		file.Close();

		logFile.Open("mapList.txt","wt");

		for (i=0;i<dMAX_MAP_COUNT;i++)
		{
			char	*strPre[]	=	{"[N]","[P]"};

			logFile.writeStream("%s[%.4d] %s\n",strPre[g_aMapSimpleInfo[i].m_bf1IsPremiumZone],i,g_aMapSimpleInfo[i].m_strName);
		}

		logFile.Close();

		if (!file.Open("data/mapLocationInfo.dat","rb"))
			return	ERRMSG("error!!","can not find 'data/mapLocationInfo.dat'");

		file.Read(g_aMapLocationInfo,sizeof(g_aMapLocationInfo));
		file.Close();

		if (!g_bIsDevPC)
			return	TRUE;
	}
	else
	{
		g_project.readCloseQuestList();
		g_map.readCloseMapList();

		logFile.Open("mapList.txt","wt",TRUE);

		for (i=0;i<dMAX_MAP_COUNT;i++)
		{
			strcpy(g_aMapSimpleInfo[i].m_strName,"valid");
			g_aMapSimpleInfo[i].m_wSerial	=	0xffff;
		}

		cSTRINGS	*lpFileList	=	cFOLDER::GetFiles("*.rmd","data/scenario/red stone/map");

		if (!lpFileList		)
			return	FALSE;

		cSTRING		*lpFile			=	lpFileList->Pick();

		SetCurrentDirectory("data/scenario/red stone/map");

		while(lpFile)
		{
			if	(lpFile->String == NULL)	
				break;

			if	(lpFile->String[0]	!=	'['	&&	lpFile->String[0]	!=	'{')
			{
				lpFile		=	lpFile->pNEXT;
				continue;
			}

			cMAP_INFO	mapInfo;
			int			iIndex		=	atoi(lpFile->String+1);

			if	(GetFieldInfo(&mapInfo,lpFile->String,_lpNux))
			{	
				g_aMapSimpleInfo[iIndex].m_wWidth			=	mapInfo.m_iWidth;
				g_aMapSimpleInfo[iIndex].m_wHeight			=	mapInfo.m_iHeight;
				strcpy(g_aMapSimpleInfo[iIndex].m_strName,mapInfo.m_strName);
				strcpy(g_aMapSimpleInfo[iIndex].m_strFileName,lpFile->String);
				g_aMapSimpleInfo[iIndex].m_wSerial			=	iIndex;
				g_aMapSimpleInfo[iIndex].m_bf8Type			=	mapInfo.m_bf4FieldType;
				g_aMapSimpleInfo[iIndex].m_bf1IsPremiumZone	=	mapInfo.m_bf1IsPremiumZone;

				if	(mapInfo.m_bf1IsPremiumZone)
					logFile.writeStream("[P]%30s - %s\n",lpFile->String,mapInfo.m_strName);
				else
					logFile.writeStream("[N]%30s - %s\n",lpFile->String,mapInfo.m_strName);
			}
			else
			{
				cMSG::Test(iIndex);
			}

			lpFile		=	lpFile->pNEXT;
		}

		SetLogFolder();

		SetCurrentDirectory("data");	

		file.Open("mapList.dat","wb");
		file.Write(g_aMapSimpleInfo,sizeof(g_aMapSimpleInfo));
		file.Close();
		logFile.Close();

		SetLogFolder();

		CDecoder	decoder;

		if	(!decoder.Upload("dev data/scripter/mapLocationInfo.txt",NULL))
		{
			cMSG::Put("error!!","can not find 'data/scripter/mapLocationInfo.txt'");

			if (!file.Open("data/mapLocationInfo.dat","rb"))
				return	ERRMSG("error!!","can not find 'data/mapLocationInfo.dat'");

			file.Read(g_aMapLocationInfo,sizeof(g_aMapLocationInfo));
			file.Close();
		}
		else
		{
			memset(g_aMapLocationInfo,0xff,sizeof(g_aMapLocationInfo));

			int	iMapCount	=	0;

			while(1)
			{
				char	*lpstrWord	=	decoder.getWord();

				if	(stricmp(lpstrWord,"field")	!=	0)
					lpstrWord	=	lpstrWord;

				BOOL	bIsCorrect;
				int	iSerial	=	decoder.getNumber(&bIsCorrect);
				if (!bIsCorrect)
					break;
				int	iX		=	decoder.getNumber(&bIsCorrect);
				if (!bIsCorrect)
					break;
				int	iY		=	decoder.getNumber(&bIsCorrect);
				if (!bIsCorrect)
					break;
				int	iZ		=	decoder.getNumber(&bIsCorrect);
				if (!bIsCorrect)
					break;

				g_aMapLocationInfo[iSerial].m_wSerial	=	iSerial;
				g_aMapLocationInfo[iSerial].x			=	iX;
				g_aMapLocationInfo[iSerial].y			=	iY;
				g_aMapLocationInfo[iSerial].z			=	iZ;
				g_aMapLocationInfo[iSerial].m_wType		=	g_aMapSimpleInfo[iSerial].m_bf8Type;

				iMapCount++;
			}

			if (!file.Open("data/mapLocationInfo.dat","wb"))
				return	FALSE;

			file.Write(g_aMapLocationInfo,sizeof(g_aMapLocationInfo));
			file.Close();
		}
	}


/*
	class	CPacketFlowingInfo
	{
	public:
		int		m_iPlayerCount;
		int		m_iReceiveSize;
		int		m_iSendSize;
		int		m_iReceiveCount;
		int		m_iSendCount;
		int		m_iCount;
	};

	CPacketFlowingInfo	aInfo[24],*lpInfo;

	memset(aInfo,0,sizeof(aInfo));

	CDecoder	decoder;
	int			iHour;
	
	if (decoder.Upload("data/scripter/_패킷 전송량.txt",NULL))
		while(1)
		{
			char	*lpstrWord	=	decoder.getWord();

			if (STRICMP(lpstrWord,"end")==0)
				break;

			if (STRICMP(lpstrWord,"hour")==0)
			{
				iHour	=	decoder.getNumber();

				if (iHour	<0	||	iHour	>=24)
					break;

				lpInfo	=	&aInfo[iHour];
				continue;
			}

			if (STRICMP(lpstrWord,"Receive")==0)
			{
				int		iReceiveSize,iReceiveCount,iSendSize,iSendCount;
				char	*lpstrMapName;

				iReceiveSize	=	decoder.getNumber();
				iReceiveCount	=	decoder.getNumber();
				decoder.getWord();
				iSendSize		=	decoder.getNumber();
				iSendCount		=	decoder.getNumber();
				lpstrMapName	=	decoder.getStream();

				lpInfo->m_iReceiveSize		+=	iReceiveSize	;
				lpInfo->m_iReceiveCount		+=	iReceiveCount	;
				lpInfo->m_iSendSize			+=	iSendSize		;
				lpInfo->m_iSendCount		+=	iSendCount		;
				lpInfo->m_iCount++;

				continue;
			}
		}
	if (decoder.Upload("data/scripter/_필드당인원수.txt",NULL))
	{
		while(1)
		{
			char	*lpstrWord	=	decoder.getWord();

			if (STRICMP(lpstrWord,"end")==0)
				break;

			if (STRICMP(lpstrWord,"hour")==0)
			{
				iHour	=	decoder.getNumber();

				if (iHour	<0	||	iHour	>=24)
					break;

				lpInfo	=	&aInfo[iHour];
				continue;
			}

			if (STRICMP(lpstrWord,"field")==0)
			{
				int	iPlayerCount	=	decoder.getNumber();

				lpInfo->m_iPlayerCount		+=	iPlayerCount;

				decoder.getWord();
				decoder.getNumber();
				
				continue;
			}
		}

		file.Open("패킷 정보.txt","wb");

		int	iTotalReceiveSize=0,iTotalReceiveCount=0,iTotalPlayerCount=0;

		for (i = 0; i<24;i++)
		{
			file.writeStream("hour [ %3d / %4d ] send [ %5d byte / %4d ] receive [ %5d byte / %4d ]\n",i,aInfo[i].m_iPlayerCount,
				aInfo[i].m_iSendSize/lpInfo->m_iCount,aInfo[i].m_iSendCount/lpInfo->m_iCount,aInfo[i].m_iReceiveSize/lpInfo->m_iCount,aInfo[i].m_iReceiveCount/lpInfo->m_iCount);

			iTotalPlayerCount	+=	aInfo[i].m_iPlayerCount;

			iTotalReceiveCount	+=	aInfo[i].m_iReceiveSize;
		}

		file.Close();
	}
*/
	return	TRUE;
}

void
cMAP::buildInfo()
{
}

void
cMAP::getLinkObjectWithDoor(cAreaDoorInfo *_lpDoor)
{
	int		iCount	=	0;
	cRECT	rect;

	rect.x1		=	_lpDoor->x1>>dTILE_XSIZE_SHIFT;
	rect.y1		=	_lpDoor->y1>>dTILE_YSIZE_SHIFT;
	rect.x2		=	_lpDoor->x2>>dTILE_XSIZE_SHIFT;
	rect.y2		=	_lpDoor->y2>>dTILE_YSIZE_SHIFT;

	memset(_lpDoor->m_awLinkObject,0xff,sizeof(_lpDoor->m_awLinkObject));

	for (int i=0;i<m_doors.m_iDoorCount;i++)
	{
		cDoorObjectInfo	*lpDoor	=	&m_doors.m_aDoorList[i];

		if (rect.isIn(lpDoor->m_wX,lpDoor->m_wY))
		{
			_lpDoor->m_awLinkObject[iCount++]	=	i;

			if	(iCount	>=	dLINK_WITH_AREA_DOOR_COUNT)
				return;
		}
	}
}

void
cMAP::getLinkObject(cAreaEvent *_lpArea)
{
	if	(_lpArea->m_wLinkObject	==	0xffff	||	_lpArea->m_wLinkObject	>	dMAX_FIXED_OBJECT_COUNT)
	{
		cMSG::Put("error!! in event area","valid link object");
		return;
	}
}

void
cMAP::changeDoorStatus(int _iDoor,BOOL _bIsSleep,BOOL _bIsHide,BOOL _bIsLocked,BOOL _bIsTrap,BOOL _bIsClose,BOOL _bIsActiveByKarma)
{
	cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)&m_area.m_aArea[_iDoor];

	BOOL	bIsDetectDoor	=	g_hero.isVisibleDoor(lpDoor);
	BOOL	bIsDetectTrap	=	g_hero.isVisibleDoorTrap(lpDoor);

	lpDoor->m_bf1IsActive	=	1-_bIsSleep;
	lpDoor->m_bf1IsOpened	=	1-_bIsClose;
	lpDoor->m_bf1IsDisarmed	=	1-_bIsTrap;
	lpDoor->m_bf1IsUnlocked	=	1-_bIsLocked;
	lpDoor->m_bf1IsHide		=	_bIsHide;
	lpDoor->m_bf1IsDetectDoor=	bIsDetectDoor;
	lpDoor->m_bf1IsDetectTrap=	bIsDetectTrap;
	lpDoor->m_bf1IsActiveByKarma=	_bIsActiveByKarma;

	for (int i=0;i<dLINK_WITH_AREA_DOOR_COUNT;i++)
	{
		int	iObject	=	lpDoor->m_awLinkObject[i];

		if (iObject	==	0xffff)
			return;

		cDoorObjectInfo	*lpDoorObject	=	&m_doors.m_aDoorList[iObject];

		lpDoorObject->m_bf1IsActive		=	1-_bIsSleep;
		lpDoorObject->m_bf1IsOpened		=	1-_bIsClose;
		lpDoorObject->m_bf1IsDisarmed	=	1-_bIsTrap;
		lpDoorObject->m_bf1IsUnlocked	=	1-_bIsLocked;
		lpDoorObject->m_bf1IsHide		=	_bIsHide;
		lpDoorObject->m_bf1IsDetectDoor	=	bIsDetectDoor;
		lpDoorObject->m_bf1IsDetectTrap	=	bIsDetectTrap;

		BOOL	bIsLinkSecretDungeon	=	FALSE;

		if	(lpDoor->m_wLinkSecretDungeon	&&	lpDoor->m_wLinkSecretDungeon	!=	0xffff	&&	lpDoor->m_wLinkSecretDungeon)
			bIsLinkSecretDungeon	=	TRUE;
		
		m_object.changeDoorStatus(lpDoorObject->m_wObjectIndex,_bIsSleep,_bIsHide,_bIsLocked,_bIsTrap,_bIsClose,bIsDetectDoor,bIsDetectTrap,bIsLinkSecretDungeon);

		if(_bIsClose)
			setDoorCrashMap(iObject,FALSE);
		else
			setDoorCrashMap(iObject,TRUE);
	}
}

BOOL
cMAP::isEventArea(cAreaEvent *_lpEventArea,int _iX,int _iY)
{
	if	(_lpEventArea->m_wLinkObject	==	0xffff)
		return	FALSE;

	cFIXED_OBJECT_INFO	*lpInfo	=	&m_object.m_aFixedObjectList[_lpEventArea->m_wLinkObject];
	cOBJ_ANM			*lpObject=	m_object.getFixedObjectImage(lpInfo->m_wObject);

	if	(!lpObject)
		return	FALSE;

	return	lpObject->isIn(0,_iX,_iY,GetScaledXPos(lpInfo->m_wX)+dTILE_XS*g_iGameScale/100/2,GetScaledYPos(lpInfo->m_wY)+dTILE_YS*g_iGameScale/100/2);
}

void
cMAP::operateEventObjectWork()
{
	int			i;
	cAreaEvent	*lpEventArea;

	for (i=0;i<m_area.m_wTouhableAreaCount;i++)
	{
		cAreaInfo	*lpArea		=	m_area.getTouchableArea(i);

		if	(lpArea->m_wKind	!=	dAREA_EVENT_AREA)
			continue;

		cAreaEvent	*lpEventArea=	(cAreaEvent	*)lpArea;

		m_object.setFocusObject(lpEventArea->m_wLinkObject,FALSE);
	}

	if	(m_area.m_iFocusedArea	==	0xffff)
		return;

	cAreaInfo	*lpFocusedArea	=	&m_area.m_aArea[m_area.m_iFocusedArea];

	if	(!lpFocusedArea->isEventArea()	&&	!lpFocusedArea->isGuildCrest()	)
		return;

	lpEventArea			=	(cAreaEvent	*)lpFocusedArea;

	m_object.setFocusObject(lpEventArea->m_wLinkObject,TRUE);
}

BOOL
cMAP::isFocusDoor(cAreaDoorInfo *_lpArea,int _iX,int _iY)
{
	for (int i=0;i<dLINK_WITH_AREA_DOOR_COUNT;i++)
	{
		int	iObject	=	_lpArea->m_awLinkObject[i];

		if	(iObject	==	0xffff)
			continue;

		cDoorObjectInfo	*lpDoor	=	&m_doors.m_aDoorList[iObject];
		cOBJ_ANM		*lpObj	=	m_object.getFixedObjectImage(lpDoor->m_wObjectImage);

		if	(lpObj->isIn(0,_iX,_iY,(lpDoor->m_wX<<dTILE_XSIZE_SHIFT)-m_pos.x+dTILE_XS/2,(lpDoor->m_wY<<dTILE_YSIZE_SHIFT)-m_pos.y+dTILE_YS/2))
			return	TRUE;
	}

	return	FALSE;
}

void
cMAP::operateDoorWork()
{
	int	i;

	for (i=0;i<m_doors.m_iDoorCount;i++)
	{
		m_object.changeImageStatus(m_doors.m_aDoorList[i].m_wObjectIndex,eOIS_NORMAL,0,TRUE);
		m_object.setFocusObject(m_doors.m_aDoorList[i].m_wObjectIndex,FALSE);
	}

	if	(m_area.m_iFocusedArea	==	0xffff)
		return;

	cAreaInfo		*lpFocusedArea	=	&m_area.m_aArea[m_area.m_iFocusedArea];

	if	(!lpFocusedArea->isDoor())
		return;

	cAreaDoorInfo	*lpDoorArea		=	(cAreaDoorInfo	*)lpFocusedArea;

	for (i=0;i<dLINK_WITH_AREA_DOOR_COUNT;i++)
	{
		int	iObject	=	lpDoorArea->m_awLinkObject[i];

		if	(iObject	==	0xffff)
			continue;

		cDoorObjectInfo	*lpDoor	=	&m_doors.m_aDoorList[iObject];

		m_object.setFocusObject(lpDoor->m_wObjectIndex,TRUE);
	}

	BOOL	bIsOccurPopupMenuEvent	=	FALSE;
	
	if	(s_isClickedRightButton)
	{
		if	(lpDoorArea->m_bf1IsOpened	==	FALSE)
			if	(lpFocusedArea->isIn(g_lpHero->m_pos.x,g_lpHero->m_pos.y))
				bIsOccurPopupMenuEvent	=	TRUE;
	}

	if	(s_isRightButtonPressed && bIsOccurPopupMenuEvent == FALSE)
	{
		if	(lpDoorArea->m_bf1IsOpened	==	FALSE)
		{
			if	(!lpFocusedArea->isIn(g_lpHero->m_pos.x,g_lpHero->m_pos.y))
			{
				CPos	posCenter;

				lpFocusedArea->getCenter(&posCenter);

				GetTargetPos(&posCenter,g_lpHero->m_pos.x,g_lpHero->m_pos.y,60);

				g_hero.moveTo(posCenter.x,posCenter.y);
			}
		}
	}

	if	(bIsOccurPopupMenuEvent)
	{
		if	(cINPDEV::IsExclusive(eMOUSE_STATE_POPUP_MENU))
			return;

		if	(s_pmCommon.isPopuped())
		{
			s_pmCommon.reset();

			return;
		}

		g_hero.m_wWorkDoor	=	lpFocusedArea->m_wSerial;

		s_pmCommon.popup(s_posMouse.x,s_posMouse.y);
		s_ttCommon.hide();

		s_pmCommon.addMenu(ePM_ATTACK_DOOR,dMSG_ATTACK);
		cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);
	}
}

//	BLANK 타일과,BLOCKED 타일을 설정한다.
void
cMAP::setDoorCrashMap(int _iDoor,BOOL _bIsClear)
{	
	cDoorObjectInfo		*lpDoor		=	&m_doors.m_aDoorList[_iDoor];
	cOBJ_ANM			*lpObj		=	m_object.getFixedObjectImage(lpDoor->m_wObjectImage);

	if	(!lpObj)
	{
		cMSG::Put("error","valid object %d",lpDoor->m_wObjectImage);
		return;
	}

	if	(!lpObj->getCrashMap())
	{
		cMSG::Put("error","invalid crash map info %d",lpDoor->m_wObjectImage);
		return;
	}

	setCrashMap(lpDoor->m_wX,lpDoor->m_wY,lpObj->getCrashMap(),_bIsClear);
}

//	BLANK 타일과,BLOCKED 타일을 설정한다.
void
cMAP::setCrashMap(int x,int y,BYTE *_lpCrash,BOOL _bIsClear)
{	
	int ys			=	256;
	int xp			=	x-128;
	int yp			=	y-128;
	int	iBufferPoint=	0,count,xPoint,size;
	int	i,j;

	iBufferPoint	=	0;

	for(j=0;j<ys;j++)
	{
		count	=	_lpCrash[iBufferPoint++];
		xPoint	=	xp;

		for(i=0;i<count;i++)
		{	
			xPoint	+=	_lpCrash[iBufferPoint++];
			size	=	_lpCrash[iBufferPoint++];

			for (int pos=0;pos<size;pos++)
			{
				int	iAddress			=	xPoint+pos+(yp+j)*m_iWidth;

				m_aBlockInfo[iAddress]	=	1-_bIsClear;
			}

			iBufferPoint	+=	size;
			xPoint			+=	size;
		}
	}
}


cDoorManager::cDoorManager()
{
	reset();
}

void
cDoorManager::reset()
{
	m_iDoorCount	=	0;
}

BOOL
cDoorManager::isFocusedDoor(cRECT *_lpRect)
{
	cRECT	rect;

	rect.x1	=	_lpRect->x1>>dTILE_XSIZE_SHIFT;
	rect.y1	=	_lpRect->y1>>dTILE_YSIZE_SHIFT;
	rect.x2	=	_lpRect->x2>>dTILE_XSIZE_SHIFT;
	rect.y2	=	_lpRect->y2>>dTILE_YSIZE_SHIFT;

	for (int i=0;i<m_iDoorCount;i++)
		if (rect.isIn(m_aDoorList[i].m_wX,m_aDoorList[i].m_wY))
			return	TRUE;

	return	FALSE;
}


void
cMAP::drawWeather()
{		// 마지막 수정일 : 09.10.19
	if(!g_lpHero)
		return;
	if(s_iStep != dSTEP_MAIN)
		return;
	 if(!m_bIsDrawWeather)
		 return;
	switch(m_wWeather)
	{
	case eWEATHER_FINE:
		{
			for(int i=0;i<dMAX_SNOW;++i)
			{
				if(!m_snow[i].m_bCreate)
					continue;
				m_snow[i].update();
				g_im.putEffect(g_im.m_wSnowImage, m_snow[i].m_fCurrentX,m_snow[i].m_fCurrentY, 0,0,0,m_snow[i].m_fScale,m_snow[i].m_fScale);
			}
			break;
		}
	case eWEATHER_SNOW:
		{

		for(int i=0;i<dMAX_SNOW;++i)
		{
			if(!m_snow[i].m_bCreate)
				m_snow[i].create(g_iScreenWidth, g_iScreenHeight);

			m_snow[i].update();
			g_im.putEffect(g_im.m_wSnowImage, m_snow[i].m_fCurrentX,m_snow[i].m_fCurrentY, 0,0,0,m_snow[i].m_fScale,m_snow[i].m_fScale);
		}
		break;

		}
		// 10.02.10. 추가 
	case eWEATHER_RAIN:
		{
			for(int i=0;i<dMAX_RAIN;++i)
			{
				if(m_rain[i].m_wState == eRAIN_CREATE)
				{
					if(!m_rain[i].m_bCreate)
					{
						m_rain[i].create(g_iScreenWidth, g_iScreenHeight);
						m_rain[i].m_wState = eRAIN_STATE;
						continue;
					}
				}
				else if(m_rain[i].m_wState == eRAIN_STATE)
				{
					m_rain[i].update();
					
					if(m_rain[i].m_wImage  == 0)
						g_anmLongRain.PutReg(m_rain[i].m_fCurrentX,m_rain[i].m_fCurrentY,0,0,0, 100, 125);
					if(m_rain[i].m_wImage  == 1)
						g_anmShortRain.PutReg(m_rain[i].m_fCurrentX,m_rain[i].m_fCurrentY,0,0,0, 100, 125);
				}
				else if(m_rain[i].m_wState == eRAIN_ANM)
				{
					m_rain[i].update();
					g_anmBombRain.PutReg(m_rain[i].m_fCurrentX,m_rain[i].m_fCurrentY,0,0,m_rain[i].m_iFrame);
				}
			}
			break;
		}	
	}
}
// 10.02.10 추가  날씨 그리기
void
cMAP::drawWeatherData()
{
	if(!g_lpHero)
		return;
	if(s_iStep != dSTEP_MAIN)
		return;
	 if(!m_bIsDrawWeather)
		 return;
	 if(m_wWeather != eWEATHER_FINE)
		 return;

	 switch(g_map.m_bf3WeatherType)
	 {
	 case eWEATHER_FINE:
		 break;
	 case eWEATHER_SNOW:
		 {
			 for(int i=0;i<dMAX_SNOW;++i)
			 {
				 if(!m_snow[i].m_bCreate)
					 m_snow[i].create(g_iScreenWidth, g_iScreenHeight);
				 
				 m_snow[i].update();
				 g_im.putEffect(g_im.m_wSnowImage, m_snow[i].m_fCurrentX,m_snow[i].m_fCurrentY, 0,0,0,m_snow[i].m_fScale,m_snow[i].m_fScale);
			 }
			 break;
		 }
		 break;
	 case eWEATHER_RAIN:
		 {
			for(int i=0;i<dMAX_RAIN;++i)
			{
				if(m_rain[i].m_wState == eRAIN_CREATE)
				{
					if(!m_rain[i].m_bCreate)
					{
						m_rain[i].create(g_iScreenWidth, g_iScreenHeight);
						m_rain[i].m_wState = eRAIN_STATE;
						continue;
					}
				}
				else if(m_rain[i].m_wState == eRAIN_STATE)
				{
					m_rain[i].update();
					
					if(m_rain[i].m_wImage  == 0)
						g_anmLongRain.PutReg(m_rain[i].m_fCurrentX,m_rain[i].m_fCurrentY,0,0,0,100,125);
					
					if(m_rain[i].m_wImage  == 1)
						g_anmShortRain.PutReg(m_rain[i].m_fCurrentX,m_rain[i].m_fCurrentY,0,0,0,100,125);
				}
				else if(m_rain[i].m_wState == eRAIN_ANM)
				{
					m_rain[i].update();
					g_anmBombRain.PutReg(m_rain[i].m_fCurrentX,m_rain[i].m_fCurrentY,0,0,m_rain[i].m_iFrame);
				}
			}
			break;
		 }
	 }
}
cSnow::cSnow()
{
	reset();
}



void
cSnow::create(int _iWidth, int _iHeight)
{
	m_iStartX = random(_iWidth);
	m_iStartY = random(_iHeight*0.75);
	
	m_fCurrentX = m_iStartX;
	m_fCurrentY = m_iStartY;
	m_iBeforPlayerX  = g_map.m_pos.x;
	m_iBeforPlayerY  = g_map.m_pos.y;
	m_posEndX = _iWidth;
	m_posEndY = m_iStartY+(_iHeight*0.25)+random(50);
	
	m_fScale = random(50) + 50.0f;
	
	m_bCreate = TRUE;
	
	m_fMoveDistanceX = max(0.1,random(10) / 10.0f);
	m_fMoveDistanceY =(random(5) / 10.0f) + 0.6f;
	
}
void 
cSnow::update()
{
	
	m_fCurrentX += m_fMoveDistanceX +m_iBeforPlayerX-g_map.m_pos.x;
	
	m_fCurrentY += m_fMoveDistanceY+m_iBeforPlayerY-g_map.m_pos.y;
	
	m_posEndX += m_iBeforPlayerX-g_map.m_pos.x;
	m_posEndY += m_iBeforPlayerY-g_map.m_pos.y;
	
	if(m_fCurrentY> m_posEndY ||  m_fCurrentY < 0|| m_fCurrentX > m_posEndX || m_fCurrentX < 0)
		m_bCreate = FALSE;
	
	m_iBeforPlayerX  = g_map.m_pos.x;
	m_iBeforPlayerY  = g_map.m_pos.y;
}

void 
cSnow::reset()
{
	
	m_iStartX = 0;
	m_iStartY = 0;
	
	m_iBeforPlayerX	= 0;
	m_iBeforPlayerY	= 0;
	m_fCurrentX = 0;
	m_fCurrentY = 0;
	m_posEndX	= 0;
	m_posEndY = 0;
	
	m_fScale= 0;
	
	m_fMoveDistanceX = 0;
	m_fMoveDistanceY = 0;
	
	m_bCreate = FALSE;
	
}
// 10.02.10 추가
void 
cRain::create(int _iWidth, int _iHeight)
{
	m_iStartX = random(_iWidth * 1.5f);
	m_iStartY = -random(_iHeight *0.5f);

	m_fCurrentX = m_iStartX;
	m_fCurrentY = m_iStartY;
	m_iBeforPlayerX  = g_map.m_pos.x;
	m_iBeforPlayerY  = g_map.m_pos.y;
	m_posEndX = _iWidth;
	m_posEndY = m_iStartY+random(_iHeight*1.0f);
	
	m_fScale = random(350) + 150.0f;
	
	m_bCreate = TRUE;
	
	m_fMoveDistanceX = -2.0f;
	m_fMoveDistanceY = 10.0f;

	m_wImage = random(2);
	m_iFrameCheck = 5;
}

// 10.02.10 추가
void 
cRain::update()
{
	if(m_wState == eRAIN_STATE)
	{
		m_fCurrentX += m_fMoveDistanceX +m_iBeforPlayerX-g_map.m_pos.x;
		m_fCurrentY += m_fMoveDistanceY+m_iBeforPlayerY-g_map.m_pos.y;
		
		m_posEndX += m_iBeforPlayerX-g_map.m_pos.x;
		m_posEndY += m_iBeforPlayerY-g_map.m_pos.y;

		if(m_fCurrentY> m_posEndY || m_fCurrentX < 0)
		{
			m_wState  = eRAIN_ANM;
			m_bCreate = FALSE;
		}
	}
	
	if(m_wState == eRAIN_ANM)
	{
		m_fCurrentX += m_iBeforPlayerX-g_map.m_pos.x;
		m_fCurrentY += m_iBeforPlayerY-g_map.m_pos.y;
		
		m_posEndX += m_iBeforPlayerX-g_map.m_pos.x;
		m_posEndY += m_iBeforPlayerY-g_map.m_pos.y;

		if(m_iFrameCount++ > m_iFrameCheck)
		{
			m_iFrameCount = 0;
			m_iFrame++;
		}
		
		if(m_iFrame >= dCHECK_RAIN_ANM)
		{
			m_wState		= eRAIN_CREATE;
			m_iFrameCount	= 0;
			m_iFrame		= 0;
		}
	}
	m_iBeforPlayerX  = g_map.m_pos.x;
	m_iBeforPlayerY  = g_map.m_pos.y;
}

// 10.02.10 추가
void 
cRain::reset()
{
	m_iStartX			= 0;
	m_iStartY			= 0;
	m_iBeforPlayerX		= 0;
	m_iBeforPlayerY		= 0;
	m_fCurrentX			= 0;
	m_fCurrentY			= 0;
	m_posEndX			= 0;
	m_posEndY			= 0;
	m_fScale			= 0;
	m_fMoveDistanceX	= 0;
	m_fMoveDistanceY	= 0;
	m_bCreate			= FALSE;
	m_wState			= eRAIN_CREATE;
	m_wImage			= 0;
	m_iFrame			= 0;
	m_iFrameCheck		= 5;
	m_iFrameCount		= 0;
}