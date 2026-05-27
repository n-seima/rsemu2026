#include "cOBJECT.H"
#include "cMAP.H"
#include "CActor.H"

int		cOBJECT::s_aObjectCheckInfo[100][3];

int		g_iEventObjectGrade	=	0xffff;

cOBJECT::cOBJECT()
{
	reset();
}

cOBJECT::~cOBJECT()
{
	close();
}

void
cOBJECT::reset()
{
	m_iTinyObjectImageCount		=	0;
	m_iFixedObjectImageCount	=	0;
	m_iBuildingImageCount		=	0;

	m_iTileSet					=	0xffff;
	m_isMCU						=	FALSE;

	m_iGS_Nar					=	219;
	m_iGS_Nen					=	220;
	m_iGS_Sul					=	221;
	m_iGS_Kemen					=	222;
	m_iGS_Lark					=	223;
	m_iGS_Siroio				=	224;

	memset(m_abGuildStatueStatus,0,sizeof(m_abGuildStatueStatus));
}

void
cOBJECT::close()
{
	for (int i=0;i<dOBJECT_IMAGE_COUNT;i++)
	{
		if	(i	==	428)
			i	=	i;

		m_aTinyObjectImage[i].Reset();
		m_aFixedObjectImage[i].Reset();
		m_aBuildingImage[i].Reset();
	}

	reset();
}

//
//	오브젝트 데이터를 로딩한다. (rso,rfo)
BOOL
cOBJECT::loadImageData(int _iTileSet,int _iScale,BOOL _bIsLoadOnlyExistObject,cNUX *_lpNux,cMCU *_lpMcu)
{
	g_iEventObjectGrade		=	0xffff;
	_bIsLoadOnlyExistObject	=	TRUE;

	if	(CGamePlay::s_currentTime.m_wMonth	==	12	&&	CGamePlay::s_currentTime.m_wDay	>=	21	&&	CGamePlay::s_currentTime.m_wDay	<=	31)
		g_iEventObjectGrade	=	0;

#ifdef	_FOR_JAPAN
	g_iEventObjectGrade	=	0xffff;
	
	if	(CGamePlay::s_currentTime.m_wMonth	==	12)
	{
		if	(CGamePlay::s_currentTime.m_wDay	>=	15	&&	CGamePlay::s_currentTime.m_wDay	<=	25)
		{
			g_iEventObjectGrade	=	0;

			if	(CGamePlay::s_currentTime.m_wDay	==	15	&&	CGamePlay::s_currentTime.m_wHour	<	14)
				g_iEventObjectGrade	=	0xffff;
		}
	}
#endif

	close();	//	오브젝트 데이터를 초기화 한다.

	int		i;
	char	strFileName[256];

	m_iTileSet					=	_iTileSet;
	m_bIsCheckObjectInfoFailed	=	FALSE;

	char	strDir[512];

	GetCurrentDirectory(512,strDir);


	//	모든 오브젝트를 로딩한다.
	{
		if (!_lpNux)
		{
			if (!SetCurrentDirectory(_ms("data/mapset/%s/Objects",g_strTileName[m_iTileSet])))
				return ERRMSG("Error in cOBJECT::loadObjectData","Object folder not found");
		}

		for (i=0;i<s_aObjectCheckInfo[m_iTileSet][0];i++)
		{
			BOOL	bIsLoad	=	TRUE;

			if	(g_iEventObjectGrade	!=	0xffff)
			{
				sprintf(strFileName,"e%.2d_sn__object_%.4d.rso",i,g_iEventObjectGrade);

				if (!IsFile(strFileName))
					sprintf(strFileName,"sn__object_%.4d.rso",i);
			}
			else
				sprintf(strFileName,"sn__object_%.4d.rso",i);

			if	(_bIsLoadOnlyExistObject && m_aExistFixedObject[m_iFixedObjectImageCount] == FALSE)
				bIsLoad	=	FALSE;

			GetCurrentDirectory(512,strDir);

			if	(IsFile(strFileName)	==	TRUE)
			{
				if (bIsLoad)
				{
					m_aFixedObjectImage[i].Load(strFileName,TRUE,_lpNux,_lpMcu);
					m_aFixedObjectImage[i].setScale(_iScale);
				}

				m_iFixedObjectImageCount++;
			}
			else
			{
				m_bIsCheckObjectInfoFailed	=	TRUE;
				g_eh.addStaticLog("ms [%d] %s",s_aObjectCheckInfo[m_iTileSet][0],strFileName);
			}
		}

		for (i=0;i<s_aObjectCheckInfo[m_iTileSet][1];i++)
		{
			BOOL	bIsLoad	=	TRUE;

			if (g_iEventObjectGrade	!=	0xffff)
			{
				sprintf(strFileName,"e%.2d_sn__object_%.4d.rfo",i,g_iEventObjectGrade);

				if (!IsFile(strFileName))
					sprintf(strFileName,"sn__object_%.4d.rfo",i);
			}
			else
				sprintf(strFileName,"sn__object_%.4d.rfo",i);

			if (_bIsLoadOnlyExistObject && m_aExistTinyObject[m_iTinyObjectImageCount] == FALSE)
				bIsLoad	=	FALSE;

			if (IsFile(strFileName)	==	TRUE)
			{
				if (bIsLoad)
				{
					m_aTinyObjectImage[i].Load(strFileName,TRUE,_lpNux,_lpMcu);
					m_aTinyObjectImage[i].setScale(_iScale);
				}

				m_iTinyObjectImageCount++;
			}
			else
			{
				m_bIsCheckObjectInfoFailed	=	TRUE;
				g_eh.addStaticLog("mf [%d] %s",s_aObjectCheckInfo[m_iTileSet][1],strFileName);
			}
		}

		if (!_lpNux)
			SetLogFolder();
	}

	//	모든 건물을 로딩한다.
	{
		if (!_lpNux)
		{
			SetLogFolder();

			if (!SetCurrentDirectory(_ms("data/mapset/%s/Buildings",g_strTileName[m_iTileSet])))
				return ERRMSG("Error in cOBJECT::loadObjectData","building folder not found");
		}

		for (i=0;i<s_aObjectCheckInfo[m_iTileSet][2];i++)
		{
			sprintf(strFileName,"sn__building_%.4d.rbd",i);

			if (IsFile(strFileName))
			{
				m_aBuildingImage[i	].Load(strFileName,TRUE,_lpNux,_lpMcu);
				m_aBuildingImage[i	].setScale(_iScale);

				m_iBuildingImageCount++;
			}
			else
			{
				m_bIsCheckObjectInfoFailed	=	TRUE;
				g_eh.addStaticLog("mb %s",s_aObjectCheckInfo[m_iTileSet][2],strFileName);
			}
		}

		if (!_lpNux)
			SetLogFolder();
	}

	return TRUE;
}

//
//	오브젝트/건물 리스트를 로딩한다.
BOOL
cOBJECT::readObjectListInMap(cFILE *_lpFile,BOOL _bIsBuildExistObjectList)
{
	_bIsBuildExistObjectList	=	TRUE;
	int	i;

	memset(m_aExistTinyObject,0xff,sizeof(m_aExistTinyObject));
	memset(m_aExistFixedObject,0xff,sizeof(m_aExistFixedObject));

//	쪼매난 오브젝트
	{
		_lpFile->Read(&m_iTinyObjectCount	,4					);	//	오브젝트 수
		_lpFile->Read(&m_aRapes				,100,sizeof(cINDEX)	);	//	오브젝트 정렬 정보
		_lpFile->Read(&m_aStands			,100,sizeof(cINDEX)	);	//
		_lpFile->Read(&m_aFloats			,100,sizeof(cINDEX)	);	//

		_lpFile->Read(m_aTinyObjectList,m_iTinyObjectCount,sizeof(cTINY_OBJECT_INFO));	//	오브젝트 불러오기
	}	//	쪼매난 오브젝트

//	고정된 오브젝트
	{
		_lpFile->Read(&m_iFixedObjectCount	,4	);	//	오브젝트 수

		for (i=0;i<m_iFixedObjectCount;i++)
		{
			cSAVE_FIXED_OBJECT_INFO	saveObject;

			_lpFile->Read(&saveObject,sizeof(cSAVE_FIXED_OBJECT_INFO));		//	 x+y+object

			cFIXED_OBJECT_INFO	*lpInfo	=	&m_aFixedObjectList[saveObject.m_wSerial];
			memcpy(lpInfo,&saveObject.m_wObject,sizeof(cFIXED_OBJECT_INFO)-4);

			lpInfo->m_bf1IsActive	=	FALSE;
			lpInfo->m_bf1IsOpened	=	FALSE;
			lpInfo->m_bf1IsDisarmed	=	TRUE;
			lpInfo->m_bf1IsUnlocked	=	FALSE;
			lpInfo->m_bf1IsHide		=	TRUE;
			lpInfo->m_bf1IsDoor		=	FALSE;
			lpInfo->m_wImageStatus	=	eOIS_NORMAL;
			lpInfo->m_wUpkeepTime	=	0;
			lpInfo->m_bf1IsFocused	=	FALSE;
			lpInfo->m_wX			=	saveObject.m_pos.x;
			lpInfo->m_wY			=	saveObject.m_pos.y;
		}
	}

	if (_bIsBuildExistObjectList)
	{
		int	iCount	=	0;

		for (i=0;i<m_iTinyObjectCount;i++)
			m_aExistTinyObject[m_aTinyObjectList[i].m_wObject]	=	TRUE;
		for (i=0;iCount<m_iFixedObjectCount && i < dMAX_FIXED_OBJECT_COUNT;i++)
		{
			if	(m_aFixedObjectList[i].m_wObject	==	0xffff)
				continue;

			iCount++;

			for (int j=0;j<dMAX_ADD_ON_OBJECT_COUNT;j++)
			{
				int	iTinyObject	=	m_aFixedObjectList[i].m_aAddonObject[j].m_wObject;

				if (iTinyObject	!=	0xffff)
					m_aExistTinyObject[iTinyObject]	=	TRUE;
			}

			m_aExistFixedObject[m_aFixedObjectList[i].m_wObject]=	TRUE;
		}
	}

//	건물 -o-
	{
		_lpFile->Read(&m_iBuildingCount	,4	);	//	오브젝트 수

		for (i=0;i<m_iBuildingCount;i++)
		{
			cSAVE_BUILDING_INFO	saveBuilding;

			_lpFile->Read(&saveBuilding,sizeof(cSAVE_BUILDING_INFO));		//	 x+y+object

			memcpy(&m_aBuildingList[saveBuilding.m_wSerial],&saveBuilding.m_wBuilding,sizeof(cBUILDING_INFO));

			if (_bIsBuildExistObjectList)
				for (int j=0;j<dMAX_ADDON_OBJECT_COUNT_ON_BUILDING;j++)
				{
					int	iTinyObject	=	m_aBuildingList[saveBuilding.m_wSerial].m_aAddonObject[j].m_wObject;

					if (iTinyObject	!=	0xffff)
						m_aExistTinyObject[iTinyObject]	=	TRUE;
				}
		}

	}	//	건물 -o-

	return TRUE;
}

//
//	뷰포트 안에 오브젝트들을 indexes로 리턴..
int
cOBJECT::getViewObjects(cINDEX *_lpIndexList,cRECT *_lpRectViewPort)
{
	int iStart	=-1,iEnd=-1,i,iCount=0;

	for (i=0;i<99;i++)
	{
		if (m_aStands[i+1].Value >= _lpRectViewPort->y1)
		{
			iStart	=	m_aStands[i].Index;
			break;
		}
	}

	if (iStart	<= -1)	return 0;

	for (i=99;i>0;i--)
		if (m_aStands[i-1].Value <= _lpRectViewPort->y2)
		{	iEnd	=	m_aStands[i].Index;
			break;
		}

	for (i=iStart;i<=iEnd;i++)
	{
		if	(i		>=	m_iTinyObjectCount)	break;

		int x		=	m_aTinyObjectList[i].m_pos.x;
		int y		=	m_aTinyObjectList[i].m_pos.y;

		if (x > _lpRectViewPort->x1 && x < _lpRectViewPort->x2)	
		{
			_lpIndexList[iCount].Value	=	y;
			_lpIndexList[iCount].Index	=	i;

			iCount++;
		}
	}

	return	iCount;
}

//
//	바닥에 깔리는 오브젝트들을 그려준다.
void
cOBJECT::drawRapes(int dx,int dy)
{
	int iStart	=-1,iEnd=-1,i;

	for (i=0;i<99;i++)
	{
		if (m_aRapes[i+1].Value >= g_rectViewPort.y1)
		{
			iStart	=	m_aRapes[i].Index;
			break;
		}
	}

	if (iStart	== -1)
		return;

	for (i=99;i>0;i--)
		if (m_aRapes[i-1].Value <= g_rectViewPort.y2)
		{
			iEnd	=	m_aRapes[i].Index;
			break;
		}

	for (i=iStart;i<=iEnd;i++)
	{
		if	(i		>=	m_iTinyObjectCount)
			break;

		int x		=	m_aTinyObjectList[i].m_pos.x;
		int y		=	m_aTinyObjectList[i].m_pos.y;
		int object	=	m_aTinyObjectList[i].m_wObject;

		if (object	>=	m_iTinyObjectImageCount)
			object	=	object;

		if (x > g_rectViewPort.x1 && x < g_rectViewPort.x2)	
		{
			if (g_bIsHalfSize)
			{
				x	=	(x	-	dx*2)/2;
				y	=	(y	-	dy*2)/2;
			}
			else
			{
				x	=	(x	-	dx);
				y	=	(y	-	dy);
			}

			m_aTinyObjectImage[object].PutShadow(x,y);
			m_aTinyObjectImage[object].Put		(x,y);
		}
	}
}

//
//	하늘을 나는(?) 오브젝트들을 그려준다.
void	
cOBJECT::drawFloatObject(int dx,int dy)
{
	int iStart	=-1,iEnd=-1,i;

	for (i=0;i<99;i++)
	{
		if (m_aFloats[i+1].Value >= g_rectViewPort.y1)
		{
			iStart	=	m_aFloats[i].Index;
			break;
		}
	}

	if (iStart	== -1)	return;

	for (i=99;i>0;i--)
		if (m_aFloats[i-1].Value <= g_rectViewPort.y2)
		{	iEnd	=	m_aFloats[i].Index;
			break;
		}

	for (i=iStart;i<=iEnd;i++)
	{
		if	(i		>=	m_iTinyObjectCount)	break;

		int x		=	m_aTinyObjectList[i].m_pos.x;
		int y		=	m_aTinyObjectList[i].m_pos.y;
		int object	=	m_aTinyObjectList[i].m_wObject;

		if (x > g_rectViewPort.x1 && x < g_rectViewPort.x2)	
		{
			m_aTinyObjectImage[object].PutShadow(x-dx,y-dy);
			m_aTinyObjectImage[object].Put		(x-dx,y-dy);
		}
	}
}


//
//	오브젝트 데이터를 로딩한다. (rso,rfo)
BOOL
cOBJECT::ConvertFileName()
{
	int		iTileSet	=	0;
	char	strNewFile[256];

	while(1)
	{
		if (STRICMP(g_strTileName[iTileSet],"end")==0)
			break;

		SetLogFolder();

		if (!SetCurrentDirectory(_ms("data/mapset/%s/Objects",g_strTileName[iTileSet])))
			return ERRMSG("Error in cOBJECT::ConvertFileName","Object folder not found");

		cSTRINGS	*pFixedObjectFileList	=	cFOLDER::GetFiles("*.rso",NULL,TRUE);//	화일들을 얻어 온다.
		cSTRINGS	*pTinyObjectFileList	=	cFOLDER::GetFiles("*.rfo",NULL,TRUE);//	화일들을 얻어 온다.

		if (pFixedObjectFileList)
		{
			cSTRING *fname =	pFixedObjectFileList->Pick();

			int		iIndex	=	0;

			for (int i=0;i<pFixedObjectFileList->Count;i++)
			{
				if (strstr(fname->String,"sn__object_")==NULL)
				{
					sprintf(strNewFile,"sn__object_%.4d.rso",iIndex);

					if (STRICMP(fname->String,strNewFile)!=0)
					{
						sprintf(strNewFile,"%s/data/mapset/%s/Objects/sn__object_%.4d.rso",g_strLogFolder,g_strTileName[iTileSet],iIndex);

						MoveFile(fname->String,strNewFile);
					}

					DeleteFile(fname->String);

					iIndex++;
				}

				fname	=	fname->pNEXT;
			}
		}

		if (pTinyObjectFileList)
		{
			cSTRING *fname =	pTinyObjectFileList->Pick();

			int		iIndex	=	0;

			for (int i=0;i<pTinyObjectFileList->Count;i++)
			{
				if (strstr(fname->String,"sn__object_")==NULL)
				{
					sprintf(strNewFile,"sn__object_%.4d.rfo",iIndex);

					if (STRICMP(fname->String,strNewFile)	!=	0)
					{
						sprintf(strNewFile,"%s/data/mapset/%s/Objects/sn__object_%.4d.rfo",g_strLogFolder,g_strTileName[iTileSet],iIndex);
						MoveFile(fname->String,strNewFile);
					}

					DeleteFile(fname->String);

					iIndex++;
				}

				fname	=	fname->pNEXT;
			}
		}

		SetLogFolder();

		if (!SetCurrentDirectory(_ms("data/mapset/%s/Buildings",g_strTileName[iTileSet])))
			return ERRMSG("Error in cOBJECT::ConvertFileName","building folder not found");

		cSTRINGS	*pBuildingFileList	=	cFOLDER::GetFiles("*.rbd",NULL,TRUE);//	화일들을 얻어 온다.

		if (pBuildingFileList	)
		{
			cSTRING		*fname	=	pBuildingFileList->Pick();

			int		iIndex	=	0;

			for (int i=0;i<pBuildingFileList->Count;i++)
			{
				if (strstr(fname->String,"sn__building_")==NULL)
				{
					sprintf(strNewFile,"sn__building_%.4d.rbd",iIndex);

					if (STRICMP(fname->String,strNewFile)!=0)
					{
						sprintf(strNewFile,"%s/data/mapset/%s/Buildings/sn__building_%.4d.rbd",g_strLogFolder,g_strTileName[iTileSet],iIndex);
						MoveFile(fname->String,strNewFile);
					}

					DeleteFile(fname->String);

					iIndex++;
				}

				fname	=	fname->pNEXT;
			}
		}

		KILL(pBuildingFileList);
		KILL(pFixedObjectFileList);
		KILL(pTinyObjectFileList);

		iTileSet++;
	}

	SetLogFolder();

	cFILE	file;

	file.Open("data/checkObject.dat","wb");
	file.Write(strNewFile,12);
	file.Close();

	return TRUE;
}

//
//	오브젝트 데이터를 로딩한다. (rso,rfo)
BOOL
cOBJECT::ExportCheckObjectInfo()
{
	int		iTileSet	=	0,iDoorCount	=	0;

	cFILE	file,fileDoor;

	file.Open("data/checkObjectData.dat","wb");
	fileDoor.Open("data/doorObjectCrashMap.dat","wb");

	fileDoor.Write(&iTileSet,4);
	fileDoor.Write(&iDoorCount,4);

	memset(s_aObjectCheckInfo,0,sizeof(s_aObjectCheckInfo));

	while(1)
	{
		if (STRICMP(g_strTileName[iTileSet],"end")==0)
			break;

		SetLogFolder();

		if (!SetCurrentDirectory(_ms("data/mapset/%s/Objects",g_strTileName[iTileSet])))
			return ERRMSG("Error in cOBJECT::ConvertFileName","Object folder not found");

		cSTRINGS	*pFixedObjectFileList	=	cFOLDER::GetFiles("*.rso",NULL,TRUE);//	화일들을 얻어 온다.
		cSTRINGS	*pTinyObjectFileList	=	cFOLDER::GetFiles("*.rfo",NULL,TRUE);//	화일들을 얻어 온다.

		int	iFixedObjectCount	=	0;
		int	iTinyObjectCount	=	0;
		int	iBuildingCount		=	0;

		if (pFixedObjectFileList)
		{
			cSTRING *fname =	pFixedObjectFileList->Pick();

			int		iIndex	=	0;
 
			for (int i=0;i<pFixedObjectFileList->Count;i++)
			{
				if (strstr(fname->String,"sn__object_") && fname->String[0] == 's')
				{
					iFixedObjectCount++;

					BYTE	aCrashBuffer[256*256];

					int		iBufferSize	=	cOBJ_ANM::GetDoorCrashMap(fname->String,aCrashBuffer,NULL);

					if (iBufferSize)
					{
						fileDoor.Write(&iTileSet,4);
						fileDoor.Write(&i,4);
						fileDoor.Write(&iBufferSize,4);
						fileDoor.Write(aCrashBuffer,iBufferSize);
						iDoorCount++;
					}
				}

				fname	=	fname->pNEXT;
			}
		}

		if (pTinyObjectFileList)
		{
			cSTRING *fname =	pTinyObjectFileList->Pick();

			int		iIndex	=	0;

			for (int i=0;i<pTinyObjectFileList->Count;i++)
			{
				if (strstr(fname->String,"sn__object_")  && fname->String[0] == 's')
					iTinyObjectCount++;

				fname	=	fname->pNEXT;
			}
		}

		SetLogFolder();

		if (!SetCurrentDirectory(_ms("data/mapset/%s/Buildings",g_strTileName[iTileSet])))
			return ERRMSG("Error in cOBJECT::ConvertFileName","building folder not found");

		cSTRINGS	*pBuildingFileList	=	cFOLDER::GetFiles("*.rbd",NULL,TRUE);//	화일들을 얻어 온다.

		if (pBuildingFileList	)
		{
			cSTRING		*fname	=	pBuildingFileList->Pick();

			int		iIndex	=	0;

			for (int i=0;i<pBuildingFileList->Count;i++)
			{
				if (strstr(fname->String,"sn__building_"))
					iBuildingCount++;

				fname	=	fname->pNEXT;
			}
		}

		KILL(pBuildingFileList);
		KILL(pFixedObjectFileList);
		KILL(pTinyObjectFileList);

		if (iFixedObjectCount	>=	256)
			iFixedObjectCount	=	iFixedObjectCount;

		s_aObjectCheckInfo[iTileSet][0]	=	iFixedObjectCount;
		s_aObjectCheckInfo[iTileSet][1]	=	iTinyObjectCount;
		s_aObjectCheckInfo[iTileSet][2]	=	iBuildingCount;

		iTileSet++;
	}

	file.Write(s_aObjectCheckInfo,sizeof(s_aObjectCheckInfo));
	file.Close();

	fileDoor.Seek(0,SEEK_SET);
	fileDoor.Write(&iTileSet,4);
	fileDoor.Write(&iDoorCount,4);

	fileDoor.Close();

	SetLogFolder();

	return TRUE;
}

//
//	오브젝트 데이터를 로딩한다. (rso,rfo)
BOOL
cOBJECT::LoadCheckObjectInfo()
{
	cFILE	file;

	if(!file.Open("data/checkObjectData.dat","rb"))
		return	FALSE;

	file.Read(s_aObjectCheckInfo,sizeof(s_aObjectCheckInfo));
	file.Close();

	return TRUE;
}

void
cOBJECT::changeDoorStatus(int _iObjectIndex,BOOL _bIsSleep,BOOL _bIsHide,BOOL _bIsLocked,BOOL _bIsTrap,BOOL _bIsClose,BOOL _bIsDetectedDoor,BOOL _bIsDetectedTrap,BOOL _bIsLinkSecretDungeon)
{
	cFIXED_OBJECT_INFO	*lpInfo	=	&m_aFixedObjectList[_iObjectIndex];

	lpInfo->m_bf1IsActive			=	1-_bIsSleep;
	lpInfo->m_bf1IsOpened			=	1-_bIsClose;
	lpInfo->m_bf1IsDisarmed			=	1-_bIsTrap;
	lpInfo->m_bf1IsUnlocked			=	1-_bIsLocked;
	lpInfo->m_bf1IsHide				=	_bIsHide;
	lpInfo->m_bf1IsDetectedDoor		=	_bIsDetectedDoor;
	lpInfo->m_bf1IsDetectedTrap		=	_bIsDetectedTrap;
	lpInfo->m_bf1IsDoor				=	TRUE;
	lpInfo->m_bf1IsLinkSecretDungeon=	_bIsLinkSecretDungeon;
}

void
cOBJECT::changeImageStatus(int _iObjectIndex,int _iStatus,int _iUpkeepTime,BOOL _bIsDecreaseUpkeepTime)
{
	if (m_aFixedObjectList[_iObjectIndex].m_wUpkeepTime	==	0)
	{
		m_aFixedObjectList[_iObjectIndex].m_wImageStatus	=	_iStatus;
		m_aFixedObjectList[_iObjectIndex].m_wUpkeepTime		=	_iUpkeepTime;
	}

	if (_bIsDecreaseUpkeepTime	&&	m_aFixedObjectList[_iObjectIndex].m_wUpkeepTime)
		m_aFixedObjectList[_iObjectIndex].m_wUpkeepTime--;
}

void
cOBJECT::setFocusObject(int _iObjectIndex,BOOL _bIsFocus)
{
	if (_iObjectIndex	<	0	||	_iObjectIndex	>=	dMAX_FIXED_OBJECT_COUNT)
		return;

	m_aFixedObjectList[_iObjectIndex].m_bf1IsFocused	=	_bIsFocus;
}

void
cOBJECT::putFixedObject(int _iIndex,int _iX,int _iY,BOOL _bIsWantPutOnlyRape,BOOL _bIsPutShadow,BOOL _bIsPutImage)
{
	cFIXED_OBJECT_INFO	*lpObject	=	&m_aFixedObjectList[_iIndex];
	BOOL				bIsShining	=	lpObject->m_bf1IsFocused;
	BOOL				bIsTrap		=	FALSE;
	BOOL				bIsHide		=	FALSE;
	int					iObjectImage=	lpObject->m_wObject;

	if (_bIsWantPutOnlyRape)
	{
		if (m_aFixedObjectImage[iObjectImage].getPutMethod()	!=	eOPM_RAPE)
			return;
	}
	else
	{
		if (m_aFixedObjectImage[iObjectImage].getPutMethod()	==	eOPM_RAPE)
			return;
	}

	int					iImageIndex	=	0;


	// 10.02.25 이벤트 영역에 연결된 고정 오브젝트 이미지 변경
	for(int iEventArea=0;iEventArea<dMAX_AREA ;++iEventArea)
  	{
  		cAreaInfo	*lpArea	=	g_map.m_area.getArea(iEventArea);
  		
  		if(!lpArea)
  			break;
  		if(lpArea->m_wSerial == 0xffff)
  			break;
  		
  		if(!lpArea->isEventArea())
  			continue;
  		
  		cAreaEvent			*lpEventArea=	(cAreaEvent *)lpArea;
  
  		if(!lpEventArea)
  			continue;
  		
  		if(lpEventArea->m_wLinkObject == _iIndex)
  		{
			if(!lpEventArea->m_bf1ChangeObject)
				break;

			// Time Check
			if( (lpEventArea->m_bf7StartYear == 0 || lpEventArea->m_bf7StartYear -1 <= CGamePlay::s_currentTime.m_wYear - 2000) && ((lpEventArea->m_bf7EndYear == 0 || lpEventArea->m_bf7EndYear -1 >= CGamePlay::s_currentTime.m_wYear - 2000)))
				if( (lpEventArea->m_bf4StartMonth == 0 || lpEventArea->m_bf4StartMonth -1 <= CGamePlay::s_currentTime.m_wMonth) &&	((lpEventArea->m_bf4EndMonth == 0 || lpEventArea->m_bf4EndMonth -1 >= CGamePlay::s_currentTime.m_wMonth)))
					if( (lpEventArea->m_bf5StartDay == 0 || lpEventArea->m_bf5StartDay -1 <= CGamePlay::s_currentTime.m_wDay) && ((lpEventArea->m_bf5EndDay == 0 || lpEventArea->m_bf5EndDay -1 >= CGamePlay::s_currentTime.m_wDay)))
						if( (lpEventArea->m_bf5StartHour == 0 || lpEventArea->m_bf5StartHour -1 <= CGamePlay::s_currentTime.m_wHour) &&	((lpEventArea->m_bf5EndHour == 0 || lpEventArea->m_bf5EndHour -1 >= CGamePlay::s_currentTime.m_wHour)))
							if( (lpEventArea->m_bf6StartMinute == 0 || lpEventArea->m_bf6StartMinute -1 <= CGamePlay::s_currentTime.m_wMinute) &&	((lpEventArea->m_bf6EndMinute == 0 || lpEventArea->m_bf6EndMinute -1 > CGamePlay::s_currentTime.m_wMinute)))
								iObjectImage = lpEventArea->m_iChangeObjectIndex;					
			break;
  		}
  	}

	// 10.02.22   자리이동..
	WORD				aTempPlt[256],aPlt[256];
	WORD				*lpOrgPlt	=	m_aFixedObjectImage[iObjectImage].m_pPlt;


	if (lpObject->m_bf1IsDoor	&&	_bIsPutImage)
	{
		if	(lpObject->m_bf1IsOpened)
			iImageIndex	=	1;

		if	(lpObject->m_bf1IsDisarmed	==	FALSE	&&	lpObject->m_bf1IsDetectedTrap)
			bIsTrap		=	TRUE;

		if	(lpObject->m_bf1IsLinkSecretDungeon	&&	lpObject->m_bf1IsOpened	==	FALSE	&&	bIsTrap	==	FALSE)
			bIsHide	=	TRUE;

		if	(lpObject->m_bf1IsOpened	==	FALSE	&&	lpObject->m_bf1IsHide)
		{
			if (lpObject->m_bf1IsDetectedDoor)
			{
				if (lpObject->m_bf1IsOpened	==	FALSE)
					bIsHide		=	TRUE;
			}
			else
			{
				iImageIndex	=	0;
				bIsShining	=	FALSE;
				bIsTrap		=	FALSE;
				bIsHide		=	FALSE;
			}
		}

		if	(lpObject->m_bf1IsActive	==	FALSE)
		{
			iImageIndex	=	0;
			bIsShining	=	FALSE;
			bIsTrap		=	FALSE;
			bIsHide		=	FALSE;
		}

		if	(bIsTrap	+	bIsHide	+	bIsShining)
			memcpy(aPlt,lpOrgPlt,sizeof(aPlt));

		if (bIsTrap)
		{
			EffectPalette(dPLT_EFFECT_RED,aPlt,aTempPlt,190,0);
			memcpy(aPlt,aTempPlt,sizeof(aPlt));
			m_aFixedObjectImage[iObjectImage].setPalette(aPlt);
		}
		else
		if (bIsHide)
		{
			EffectPalette(dPLT_EFFECT_BLUE,aPlt,aTempPlt,195,0);
			memcpy(aPlt,aTempPlt,sizeof(aPlt));
			m_aFixedObjectImage[iObjectImage].setPalette(aPlt);
		}

		if (bIsShining)
		{
			int	iValue	=	g_iImageFrameCounter*200/100%100*2;

			if (iValue	>	100)
				iValue	=	100*2-iValue;

			EffectPalette(dPLT_EFFECT_BRIGHT,aPlt,aTempPlt,100+iValue,0);

			m_aFixedObjectImage[iObjectImage].setPalette(aTempPlt);
		}

		if	(_bIsPutShadow	&&	lpObject->m_wIsPutShadow)
			m_aFixedObjectImage[iObjectImage].PutShadow(iImageIndex,_iX,_iY);

		m_aFixedObjectImage[iObjectImage].Put(iImageIndex,_iX,_iY);
		m_aFixedObjectImage[iObjectImage].restorePalette();
	}
	else
	{
		if	(_bIsPutShadow	&&	lpObject->m_wIsPutShadow)
			m_aFixedObjectImage[iObjectImage].PutShadow(iImageIndex,_iX,_iY);

		if	(_bIsPutImage)
		{
			if	(bIsShining)
				memcpy(aPlt,lpOrgPlt,sizeof(aPlt));

			if	(bIsShining)
			{
				int	iValue	=	g_iImageFrameCounter*200/100%100*2;

				if (iValue	>	100)
					iValue	=	100*2-iValue;

				EffectPalette(dPLT_EFFECT_BRIGHT,aPlt,aTempPlt,100+iValue,0);

				m_aFixedObjectImage[iObjectImage].setPalette(aTempPlt);
			}

			m_aFixedObjectImage[iObjectImage].Put(_iX,_iY);
			m_aFixedObjectImage[iObjectImage].restorePalette();
		}
	}

	for (int i=0;i<dMAX_ADD_ON_OBJECT_COUNT;i++)
	{
		int	iAddonObject	=	lpObject->m_aAddonObject[i].m_wObject;

		if	(iAddonObject	==	0xffff)
			break;

		int	iDX	=	lpObject->m_aAddonObject[i].m_sDx*g_iGameScale/100;
		int	iDY	=	lpObject->m_aAddonObject[i].m_sDy*g_iGameScale/100;

		if	(_bIsPutShadow)
			m_aTinyObjectImage[iAddonObject].PutShadow(0,_iX+iDX,_iY+iDY);

		if	(_bIsPutImage)
			m_aTinyObjectImage[iAddonObject].Put(_iX+iDX,_iY+iDY);
	}

	if	(m_bIsGuildHall)
		for	(int iGSIndex=0;iGSIndex<6;iGSIndex++)
		{
			if	(iObjectImage	==	m_iGS_Nar+iGSIndex)
			{
				for	(int iParts=0;iParts<5;iParts++)
					if	(m_abGuildStatueStatus[iGSIndex][iParts])
						m_aFixedObjectImage[iObjectImage].Put(iParts+1,_iX,_iY);
			}
		}
}


void
cOBJECT::putTinyObject(int _iIndex,int _iDx,int _iDy)
{
	_iDx	=	GetScaledXPos(m_aTinyObjectList[_iIndex].m_pos.x);
	_iDy	=	GetScaledYPos(m_aTinyObjectList[_iIndex].m_pos.y);

	m_aTinyObjectImage[m_aTinyObjectList[_iIndex].m_wObject].PutShadow	(_iDx,_iDy);
	m_aTinyObjectImage[m_aTinyObjectList[_iIndex].m_wObject].Put		(_iDx,_iDy);
}

void
cOBJECT::putBuilding(int _iIndex,int _iX,int _iY)
{
	cBUILDING_INFO	*lpBuilding	=	&m_aBuildingList[_iIndex];
	int				i;

	if (lpBuilding->m_isPutShadow)	//	건물 그림자(블럭 포함)
	{
		m_aBuildingImage[lpBuilding->m_wBuilding].PutShadow(0,_iX,_iY);
		for (i=0;i<lpBuilding->m_wAddonBlockCount;i++)	
			m_aBuildingImage[lpBuilding->m_wBuilding].PutShadow(lpBuilding->m_aAddonBlock[i],_iX,_iY);
	}

	m_aBuildingImage[lpBuilding->m_wBuilding].Put(0,_iX,_iY);	//	본체-_-?

	for (i=0;i<lpBuilding->m_wAddonBlockCount;i++)	//	추가 블럭
		m_aBuildingImage[lpBuilding->m_wBuilding].Put(lpBuilding->m_aAddonBlock[i],_iX,_iY);

	for (i=0;i<lpBuilding->m_wAddonObjectCount;i++)	//	추가 오브젝트
	{
		int	iObject	=	lpBuilding->m_aAddonObject[i].m_wObject;

		if (lpBuilding->m_isPutShadow)
			m_aTinyObjectImage[iObject].PutShadow(0,_iX+lpBuilding->m_aAddonObject[i].m_sDx,_iY+lpBuilding->m_aAddonObject[i].m_sDy);

		m_aTinyObjectImage[iObject].Put(_iX+lpBuilding->m_aAddonObject[i].m_sDx,_iY+lpBuilding->m_aAddonObject[i].m_sDy);
	}
}
