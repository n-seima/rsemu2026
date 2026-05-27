#include "cTEXT.H"
#include "cOBJECT.H"
#include "cMAIN.H"
#include "cMAPWND.H"
#include "cDRAW.H"
#include "cSET.H"

cOBJECT			*g_lpObject			=	NULL;
int				g_iFreeObject		=	0;
int				g_iSelectFidexedObject		=	0;

//COPY_BLOCK_SAVE * cOBJECT::m_ptagBlockSave			=	NULL;

//
//	생성자. -o- (선행자가 아니다!!
cOBJECT::cOBJECT()
{
	m_pFixedObjectImage			=	NULL;
	m_pTinyObjectImage			=	NULL;

	reset();
}	//	cOBJECT::cOBJECT()

//
//	초기화...
void
cOBJECT::reset()
{
	m_iFixedObjectImageCount=	0;
	m_iTinyObjectImageCount	=	0;
	m_iTileSet				=	0;
	m_iSelectFixedObject	=	0;
	m_iSelectTinyObject		=	0;
	m_iSelectAddonObject	=	0;
	m_ptagBlockSave			=	NULL;
	m_ptagTinyBlockSave		=	NULL;
	m_pSelectFixedObjectList	=	NULL;
	m_pSelectTinyObjectList		=	NULL;

	m_denTinyObject.init(dMAX_OBJECT);
	m_denFixedObject.init(dMAX_OBJECT);
}	//	cOBJECT::reset()

//
//	닫아!!
void
cOBJECT::close()
{
	m_denTinyObject.close();
	m_denFixedObject.close();

	pKILL(m_pFixedObjectImage);
	pKILL(m_pTinyObjectImage);

	pKILL(m_pSelectFixedObjectList);
	pKILL(m_pSelectTinyObjectList);
	pKILL(m_ptagBlockSave);
	pKILL(m_ptagTinyBlockSave);
}	//	cOBJECT::reset()

/********************************************************************
	Object 폴더의 오브젝트들을 불러 들인다.
********************************************************************/

//
//	초기화... 나... 나두!! -_-???
BOOL
cOBJECT::init(int tileSet,cNUX *nux)
{
	close();	//	각종 데이터 초기화
	reset();

	m_iTileSet	=	tileSet;

	if (!SetCurrentDirectory(_ms("%s\\data\\mapset\\%s\\Objects",_logFolder,g_strTileSet[tileSet*2])))
		return ERRMSG("Error in cOBJECT::Init - Object folder not found");

	int	iObjectDataCount	=	0;	//	총 오브젝트의 수

	m_pFixedObjectImage		=	new cOBJ_ANM [1024];
	m_pTinyObjectImage		=	new cOBJ_ANM [1024];

	iObjectDataCount	=	0;

	int	i;

	if	(stricmp(g_strTileSet[tileSet*2],"GuildHall")	==	0)
	{
		{
			cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rso");

			if	(lpFiles)
			{
				for	(int i=0;i<lpFiles->Count;i++)
				{
					cSTRING	*lpFile	=	lpFiles->Pick(i);

					if (m_pFixedObjectImage[m_iFixedObjectImageCount].Load(lpFile->String,TRUE,nux))
					{
						iObjectDataCount++;
						m_iFixedObjectImageCount++;
					}
				}
			}
		}
		{
			cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rfo");

			if	(lpFiles)
			{
				for	(int i=0;i<lpFiles->Count;i++)
				{
					cSTRING	*lpFile	=	lpFiles->Pick(i);

					if (m_pTinyObjectImage[m_iTinyObjectImageCount].Load(lpFile->String,TRUE,nux))
					{
						iObjectDataCount++;
						m_iTinyObjectImageCount++;
					}
				}
			}
		}
	}
	else
	{
		for (i=0;;i++)
		{	
			char	strFileName[256];

			sprintf(strFileName,"sn__object_%.4d.rso",i);

			if (m_pFixedObjectImage[m_iFixedObjectImageCount].Load(strFileName,TRUE,nux))
			{
				iObjectDataCount++;
				m_iFixedObjectImageCount++;
			}
			else
				break;
		}

		for (i=0;;i++)
		{
			char	strFileName[256];

			sprintf(strFileName,"sn__object_%.4d.rfo",i);

			if (m_pTinyObjectImage[m_iTinyObjectImageCount].Load(strFileName,TRUE,nux))
			{
				iObjectDataCount++;
				m_iTinyObjectImageCount++;
			}
			else
				break;
		}
	}

	SetLogFolder();				//	로그 폴더

	MSGOUT("'%d' numbers STATIC Object.",m_iFixedObjectImageCount);
	MSGOUT("'%d' numbers FREE   Object.",m_iTinyObjectImageCount);
	MSGOUT("Total '%d' numbers OBJECT Loaded.\n",iObjectDataCount);

	m_pSelectFixedObjectList	=	new BYTE [m_iFixedObjectImageCount];
	m_pSelectTinyObjectList		=	new BYTE [m_iTinyObjectImageCount];

	memset(m_pSelectFixedObjectList,0,m_iFixedObjectImageCount);
	memset(m_pSelectTinyObjectList,0,m_iTinyObjectImageCount);

	m_iSelectFixedObjectCount	=	0;
	m_iSelectTinyObjectCount	=	0;
	g_iFreeObject				=	0;
	g_iSelectFidexedObject		=	0;

	return TRUE;
}	//	cOBJECT::Init(int tileSet,cNUX *nux)

int
cOBJECT::getSelectedTinyObjectCount()				//09.07.28  선택한 tinyobjectcount 추가..
{
	int	iSelectObjectCount	=	0;
	int	iObjectCount		=	m_denTinyObject.getCount();

	for (int iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < iObjectCount;iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		iCount++;

		if(!lpObject->m_isSelected)
			continue;

		iSelectObjectCount++;
	}

	return	iSelectObjectCount;
}

//////////////////////////////////////////////////////////////////////////////
//	걸리적 거리는 오브젝트
//////////////////////////////////////////////////////////////////////////////
//
//등록된 오브젝트 찍기-o-
void
cOBJECT::putFixedObject(cFIXED_OBJECT_INFO *_lpObject,int _iX,int _iY)
{
	if (g_MAPWND.m_bIsWantDrawSelectBox	==	FALSE)
	{
		if (_lpObject->m_wIsPutShadow)
			putFixedObjectShadow(_lpObject->m_wObject,_iX,_iY);
		
		putFixedObject(_lpObject->m_wObject,_iX,_iY);		//	실제로 있는지 체크 해야 한다.

		if (_lpObject->m_isSelected)	putFixedObjectLayer(_lpObject->m_wObject,_iX,_iY);

		for (int i=0;i<dMAX_ADD_ON_OBJECT_COUNT;i++)
		{
			int	iAddonObject	=	_lpObject->m_aAddonObject[i].m_wObject;
			if (iAddonObject	==	0xffff)	break;

			putTinyObjectShadow(iAddonObject,_iX+_lpObject->m_aAddonObject[i].m_sDx,_iY+_lpObject->m_aAddonObject[i].m_sDy);
			putTinyObject(iAddonObject,_iX+_lpObject->m_aAddonObject[i].m_sDx,_iY+_lpObject->m_aAddonObject[i].m_sDy);

			if (m_iSelectAddonObject==	i	&&	_lpObject->m_isSelected)
				putTinyObjectLayer(iAddonObject,_iX+_lpObject->m_aAddonObject[i].m_sDx,_iY+_lpObject->m_aAddonObject[i].m_sDy);
		}

		// 09.12.18
		cTEXT::Put(_iX,_iY,cSET_FONT::s_OldColorOption[eCOLOR_OBJECT_SERIAL],_ms("%d",_lpObject->m_wSerial));
		//cTEXT::Put(_iX,_iY,WHITE,_ms("%d",_lpObject->m_wSerial));
	}

	if (g_MAPWND.bLININGBOX)
		cDRAW::Fill(_LTRED,_iX-1,_iY-1,_iX+1,_iY+1);
}

//
//등록된 오브젝트 찍기-o-
void
cOBJECT::putFixedObjectScale(cFIXED_OBJECT_INFO *_lpObject,int _iX,int _iY,int _iScale)
{
	if (_lpObject->m_wIsPutShadow)	putFixedObjectShadowScale(_lpObject->m_wObject,_iX,_iY,_iScale);
	putFixedObjectScale(_lpObject->m_wObject,_iX,_iY,_iScale);		//	실제로 있는지 체크 해야 한다.

	if (_lpObject->m_isSelected)	putFixedObjectLayerScale(_lpObject->m_wObject,_iX,_iY,_iScale);

	for (int i=0;i<dMAX_ADD_ON_OBJECT_COUNT;i++)
	{
		int	iAddonObject	=	_lpObject->m_aAddonObject[i].m_wObject;
		if (iAddonObject	==	0xffff)	break;

		putTinyObjectScale(iAddonObject,_iX+_lpObject->m_aAddonObject[i].m_sDx*_iScale/100,_iY+_lpObject->m_aAddonObject[i].m_sDy*_iScale/100,_iScale);
	}
}

//
//등록된 오브젝트 찍기-o-
void
cOBJECT::putFixedObjectScale(cFIXED_OBJECT_INFO *_lpObject,int _iX,int _iY,int _iTBSize,int _iScale)
{
	if (_lpObject->m_wIsPutShadow)	putFixedObjectShadowScale(_lpObject->m_wObject,_iX,_iY,_iScale);
	putFixedObjectScale(_lpObject->m_wObject,_iX,_iY,_iScale);		//	실제로 있는지 체크 해야 한다.

	if (_lpObject->m_isSelected)	putFixedObjectLayerScale(_lpObject->m_wObject,_iX,_iY,_iScale);

	for (int i=0;i<dMAX_ADD_ON_OBJECT_COUNT;i++)
	{
		int	iAddonObject	=	_lpObject->m_aAddonObject[i].m_wObject;
		if (iAddonObject	==	0xffff)	break;

		putTinyObjectScale(iAddonObject,_iX+_lpObject->m_aAddonObject[i].m_sDx*_iScale/100,_iY+_lpObject->m_aAddonObject[i].m_sDy*_iScale/100,_iScale);
	}
}

void
cOBJECT::putFixedObject(int index,int x,int y,BOOL red)
{	if (index >= m_iFixedObjectImageCount ||  index < 0) return;

	if (red)	m_pFixedObjectImage[index].PutRed(x,y);
	else		m_pFixedObjectImage[index].Put(x,y);
}

void
cOBJECT::putFixedObjectScale(int index,int x,int y,int _iScale)
{	if (index >= m_iFixedObjectImageCount ||  index < 0) return;

	m_pFixedObjectImage[index].PutScale(x,y,_iScale);
}

void
cOBJECT::putFixedObjectLayer(int index,int x,int y)
{	if (index >= m_iFixedObjectImageCount ||  index < 0) return;

	m_pFixedObjectImage[index].PutLayer(x,y);
}

void
cOBJECT::putFixedObjectLayerScale(int index,int x,int y,int _iScale)
{	if (index >= m_iFixedObjectImageCount ||  index < 0) return;

	m_pFixedObjectImage[index].PutLayerScale(x,y,_iScale);
}

void
cOBJECT::putFixedObjectShadow(int index,int x,int y)
{	if (index >= m_iFixedObjectImageCount ||  index < 0) return;

	m_pFixedObjectImage[index].PutShadow(x,y);
}

void
cOBJECT::putFixedObjectShadowScale(int index,int x,int y,int _iScale)
{	if (index >= m_iFixedObjectImageCount ||  index < 0) return;

	m_pFixedObjectImage[index].PutShadowScale(x,y,_iScale);
}

void
cOBJECT::putFixedObject(int index,int x,int y,int xs,int ys)
{	if (index >= m_iFixedObjectImageCount) return;

	m_pFixedObjectImage[index].FitPut(x,y,xs,ys);
}


//////////////////////////////////////////////////////////////////////////////
//	밟히는 오브젝트
//////////////////////////////////////////////////////////////////////////////
void
cOBJECT::putTinyObject(int _iObject,int x,int y,BOOL red)
{
	if (_iObject < 0 || _iObject>= m_iTinyObjectImageCount)
		return;

	cOBJ_ANM	*lpObject	=	&m_pTinyObjectImage[_iObject];

	if (g_MAPWND.m_bIsWantDrawSelectBox	==	FALSE)
	{
		if (red)	lpObject->PutRed(x,y);
		else		lpObject->Put(x,y);
	}

	if (g_MAPWND.bLININGBOX)
	{
		cRECT	objectCrash	=	lpObject->Crash;

		objectCrash.Move(x,y);

		cDRAW::Box(_LTRED,objectCrash.x1,objectCrash.y1,objectCrash.x2,objectCrash.y2);
	}
}

void
cOBJECT::putTinyObjectScale(int _iObject,int x,int y,int _iScale)
{
	if (_iObject < 0 || _iObject>= m_iTinyObjectImageCount) return;

	m_pTinyObjectImage[_iObject].PutScale(x,y,_iScale);
}

void
cOBJECT::putTinyObject(int _iObject,int x,int y,int xs,int ys)
{
	if (_iObject < 0 || _iObject>= m_iTinyObjectImageCount) return;

	m_pTinyObjectImage[_iObject].FitPut(x,y,xs,ys);
}

void
cOBJECT::putTinyObjectLayer(int _iObject,int x,int y)
{
	if (_iObject < 0 || _iObject>= m_iTinyObjectImageCount) return;

	m_pTinyObjectImage[_iObject].PutLayer(x,y);
}

void
cOBJECT::putTinyObjectLayerScale(int _iObject,int x,int y,int _iScale)
{
	if (_iObject < 0 || _iObject>= m_iTinyObjectImageCount) return;

	m_pTinyObjectImage[_iObject].PutLayerScale(x,y,_iScale);
}

void
cOBJECT::putTinyObjectShadow(int _iObject,int x,int y)
{
	if (_iObject < 0 || _iObject>= m_iTinyObjectImageCount) return;

	m_pTinyObjectImage[_iObject].PutShadow(x,y);
}

void
cOBJECT::putTinyObjectShadowScale(int _iObject,int x,int y,int _iScale)
{
	if (_iObject < 0 || _iObject>= m_iTinyObjectImageCount) return;

	m_pTinyObjectImage[_iObject].PutShadowScale(x,y,_iScale);
}

//
//	선택 취소!!
void
cOBJECT::freeSelect()
{
	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		lpObject->m_isSelected	=	FALSE;

		iCount++;
	}

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denFixedObject.getCount();iIndex++)
	{
		cFIXED_OBJECT_INFO	*lpObject	=	m_denFixedObject.get(iIndex);

		if(!lpObject)	continue;

		lpObject->m_isSelected	=	FALSE;

		iCount++;
	}
}

//
//	선택-o-
void
cOBJECT::select(cRECT *_lpRect,BOOL _bIsSelectSameObjectWithSelectedObject)		// 09.08.03
{
	WORD	wSelectedObject	=	0xffff;

	if (_bIsSelectSameObjectWithSelectedObject)	
		wSelectedObject	=	getSelectedObject();
	
	cRECT	objectCrash;

	if ((!KEY[dKEY_SHIFT]) && (!KEY[dKEY_CONTROL]))	freeSelect();

	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		if (wSelectedObject	!=	0xffff)
			if (wSelectedObject	!=	lpObject->m_wObject)	
				continue;

		{
			objectCrash	=	m_pTinyObjectImage[lpObject->m_wObject].Crash;
			objectCrash.Move(lpObject->m_pos.x,lpObject->m_pos.y);

			if (_lpRect->Crash(&objectCrash))
			{	
				if (KEY[dKEY_CONTROL])	
					lpObject->m_isSelected	=	FALSE;
				else					
					lpObject->m_isSelected	=	TRUE;
			}
		}

		iCount++;
	}

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denFixedObject.getCount();iIndex++)
	{
		cFIXED_OBJECT_INFO	*lpObject	=	m_denFixedObject.get(iIndex);

		if(!lpObject)	continue;

		if (wSelectedObject	!=	0xffff)
		{
			if (wSelectedObject	>= 0x8000  &&  wSelectedObject - 0x8000 != lpObject->m_wObject)	
				continue;
		}

		if (_lpRect->isIN(lpObject->m_pos.x+dTILE_WIDTH/2,lpObject->m_pos.y+dTILE_HEIGHT/2))
		{
			if (KEY[dKEY_CONTROL])	
				lpObject->m_isSelected	=	FALSE;
			else					
				lpObject->m_isSelected	=	TRUE;
		}

		iCount++;
	}
}

cPOINT
cOBJECT::GetLeftBouttomPoint(int _iCount)
{
	if(!m_ptagBlockSave)
		return cPOINT(0 , 0);

	int iMaxX = 100000;			//충분히 큰수.
	int iMaxY = 0;

	for(int i = 0 ; i <_iCount ; i++)
	{
		if(iMaxX > m_ptagBlockSave[i].iX)
		{
			iMaxX = m_ptagBlockSave[i].iX;
			iMaxY = m_ptagBlockSave[i].iY;
		}
	}

	return cPOINT(iMaxX , iMaxY);
}


BOOL
cOBJECT::PasteStaticObject(int _iCount)
{
	if(!m_ptagBlockSave)	
		return	FALSE;

	for(int i = 0 ; i < _iCount ; i++)
	{
		if (g_lpActmap->m_object.getFixedObjectCount()	<	dMAX_OBJECT_COUNT)
		{
			cPOINT	PosLeftBouttom = GetLeftBouttomPoint(_iCount);

			int		x	=	(m_ptagBlockSave[i].iX - PosLeftBouttom.x + g_MAPWND.Cursor.x + g_MAPWND.m_posWorld.x)*100/g_iScale/dTILE_WIDTH;
			int		y	=	((m_ptagBlockSave[i].iY) - PosLeftBouttom.y + g_MAPWND.Cursor.y+ g_MAPWND.m_posWorld.y)*100/g_iScale/dTILE_HEIGHT;
			
			g_lpActmap->buildObject(m_ptagBlockSave[i].iObject,x,y + 1);
			
			g_lpActmap->LostVirgin();
		}
	}

	return TRUE;
}


//JBC	정적 오브젝트를 복사한다.. 08-08-13
int
cOBJECT::CopyStaticObject()
{
	//if(!IsCorrectMode(dWORK_STATIC_OBJECT))		return FALSE;

	int		iIndex=0,iCount=0;
	WORD	wSelectedObject	=	0xffff;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denFixedObject.getCount();iIndex++)
	{
		cFIXED_OBJECT_INFO	*lpObject	= m_denFixedObject.get(iIndex);
		
		if	(!lpObject)
			continue;
		
		if (wSelectedObject	!=	0xffff)
		{
			if	(wSelectedObject	>= 0x8000  &&  wSelectedObject - 0x8000 != lpObject->m_wObject)	
				continue;
		}
		
		if	(lpObject->m_isSelected == TRUE)
		{
			iCount++;
		}		
	}

	if	(iCount == 0)
		return 0;

	pKILL(m_ptagBlockSave);

	m_ptagBlockSave = new COPY_BLOCK_SAVE [iCount];

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denFixedObject.getCount();iIndex++)
	{
		cFIXED_OBJECT_INFO	*lpObject	=  m_denFixedObject.get(iIndex);
		
		if	(!lpObject)
			continue;
		
		if	(wSelectedObject	!=	0xffff)
		{
			if	(wSelectedObject	>= 0x8000  &&  wSelectedObject - 0x8000 != lpObject->m_wObject)	
				continue;
		}
		
		if	(lpObject->m_isSelected == TRUE)
		{
			m_ptagBlockSave[iCount].iObject = lpObject->m_wObject;
			m_ptagBlockSave[iCount].iX		= lpObject->m_pos.x;
			m_ptagBlockSave[iCount].iY		= lpObject->m_pos.y;
			iCount++;
		}		
	}

	return iCount;
}


BOOL
cOBJECT::isCrashTinyObject(int object,int x,int y)
{
	if (x < 0 || y <0)
		return TRUE;

	if (m_pTinyObjectImage[object].IsFixed())
	{
		x	=	(x/(dTILE_WIDTH/dSHARE_TILE))*(dTILE_WIDTH/dSHARE_TILE)	+	dTILE_WIDTH/dSHARE_TILE/2;
		y	=	(y/(dTILE_HEIGHT/dSHARE_TILE))*(dTILE_HEIGHT/dSHARE_TILE)+	dTILE_HEIGHT/dSHARE_TILE/2;
	}

	cRECT	crash	=	m_pTinyObjectImage[object].Crash,box;
	crash.Move(x,y);

	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if	(!lpObject)
			continue;

		if	(lpObject->m_pos.x	<	x - 300)	goto	NOT_CRASH_LABEL;
		if	(lpObject->m_pos.x	>	x + 300)	goto	NOT_CRASH_LABEL;
		if	(lpObject->m_pos.y	<	y - 300)	goto	NOT_CRASH_LABEL;
		if	(lpObject->m_pos.y	>	y + 300)	goto	NOT_CRASH_LABEL;

		box	=	m_pTinyObjectImage[lpObject->m_wObject].Crash;
		box.Move(lpObject->m_pos.x,lpObject->m_pos.y);

		if	(crash.Crash(&box))
			return TRUE;

NOT_CRASH_LABEL:
		iCount++;
	}

	return FALSE;
}

//
//	선택된 오브젝트를 지운다.
void
cOBJECT::removeSelectedAll()
{
	int	iIndex=0,iCount=0;
	int	iObjectCount	=	m_denTinyObject.getCount();

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < iObjectCount;iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if	(!lpObject)
			continue;

		iCount++;

		if	(!lpObject->m_isSelected)
			continue;

		m_denTinyObject.remove(iIndex);
	}


	iObjectCount	=	m_denFixedObject.getCount();

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < iObjectCount;iIndex++)
	{
		cFIXED_OBJECT_INFO	*lpObject	=	m_denFixedObject.get(iIndex);

		if	(!lpObject)
			continue;

		iCount++;

		if	(!lpObject->m_isSelected)
			continue;

		m_denFixedObject.remove(iIndex);
	}
}

//
//	선택된 고정 오브젝트의 그림자 상태를 설정한다.
void
cOBJECT::checkSelectFixedObjectShadow(BOOL _bIsCheck)
{
	int	iSelectObjectCount	=	0;
	int	iObjectCount		=	m_denFixedObject.getCount();

	for (int iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < iObjectCount;iIndex++)
	{
		cFIXED_OBJECT_INFO	*lpObject	=	m_denFixedObject.get(iIndex);

		if	(!lpObject)
			continue;

		iCount++;

		if	(!lpObject->m_isSelected)
			continue;

		lpObject->m_wIsPutShadow	=	_bIsCheck;
	}
}

//
//	선택된 고정 오브젝트 수
int
cOBJECT::getSelectedFixedObjectCount()
{
	int	iSelectObjectCount	=	0;
	int	iObjectCount		=	m_denFixedObject.getCount();

	for (int iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < iObjectCount;iIndex++)
	{
		cFIXED_OBJECT_INFO	*lpObject	=	m_denFixedObject.get(iIndex);

		if(!lpObject)	continue;

		iCount++;

		if(!lpObject->m_isSelected)
			continue;

		iSelectObjectCount++;
	}

	return	iSelectObjectCount;
}

//
//	선택된 고정 오브젝트 수
int
cOBJECT::getSelectedObjectCount()
{
	int	iSelectObjectCount	=	getSelectedFixedObjectCount();
	int	iObjectCount		=	m_denTinyObject.getCount();
	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < iObjectCount;iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if	(!lpObject)
			continue;

		iCount++;

		if	(!lpObject->m_isSelected)
			continue;

		iSelectObjectCount++;
	}

	return	iSelectObjectCount;
}


//
//	선택된 오브젝트
int
cOBJECT::getSelectedObject(BOOL _bIsWantObject)
{
	int	iIndex=0,iCount=0;
	int	iObjectCount	=	m_denTinyObject.getCount();

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < iObjectCount;iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)
			continue;

		iCount++;

		if(!lpObject->m_isSelected)
			continue;

		if	(_bIsWantObject)
			return	lpObject->m_wObject;

		return	iIndex;
	}


	iObjectCount	=	m_denFixedObject.getCount();

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < iObjectCount;iIndex++)
	{
		cFIXED_OBJECT_INFO	*lpObject	=	m_denFixedObject.get(iIndex);

		if(!lpObject)
			continue;

		iCount++;

		if(!lpObject->m_isSelected)
			continue;

		if	(_bIsWantObject)
			return	lpObject->m_wObject+0x8000;

		return	iIndex+0x8000;
	}

	return	0xffff;
}

//
//	고정된 오브젝트중 선택된것을 리턴한다.
int
cOBJECT::getSelectFixedObject(int _iObject)
{
	int	iObject	=	_iObject;

	if (m_iSelectFixedObjectCount	>	1)
	{
		int	value	=	random(m_iSelectFixedObjectCount);
		int	count	=	0;

		for (int i=0;i<m_iFixedObjectImageCount;i++)	//	고정된 오브젝트
		{
			if (m_pSelectFixedObjectList[i])
			{
				if (count	==	value)
				{
					iObject	=	i;
					break;
				}

				count++;
			}
		}
	}

	return	iObject;
}

//
//	쬐매난  오브젝트중 선택된것을 리턴한다.
int
cOBJECT::getSelectTinyObject(int _iObject)
{
	int	iObject	=	_iObject;

	if (m_iSelectTinyObjectCount	>	1)
	{
		int	value	=	random(m_iSelectTinyObjectCount);
		int	count	=	0;

		for (int i=0;i<m_iTinyObjectImageCount;i++)	//	고정된 오브젝트
		{
			if (m_pSelectTinyObjectList[i])
			{
				if (count==value)
				{
					iObject	=	i;
					break;
				}

				count++;
			}
		}
	}

	return	iObject;
}

//
//	바닥에 깔리는 오브젝트를 추가한다.
int
cOBJECT::addTinyObject(int _iObject,int _iX,int _iY)
{
	if (m_iTinyObjectImageCount	<=	0)	return	0xffff;

	cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.getRookie();

	if (!lpObject							)	return 0xffff;
	if (isCrashTinyObject(_iObject,_iX,_iY)	)	return 0xffff;	//	충돌~~

	lpObject->m_pos.x	=	_iX;
	lpObject->m_pos.y	=	_iY;
	lpObject->m_wObject	=	_iObject;
	lpObject->m_wSerial	=	m_denTinyObject.getRookieSerial();

	m_denTinyObject.increaseCount();

	return	lpObject->m_wSerial;
}

//
//	고정된 오브젝트를 추가한다.
int
cOBJECT::addFixedObject(int _iObject,int _iX,int _iY)
{
	if (m_iFixedObjectImageCount<=	0)	return	0xffff;

	cFIXED_OBJECT_INFO	*lpObject	=	m_denFixedObject.getRookie();

	if (!lpObject)	return	0xffff;

	lpObject->reset();
	lpObject->m_pos.x	=	_iX;
	lpObject->m_pos.y	=	_iY;
	lpObject->m_wObject	=	_iObject;
	lpObject->m_wSerial	=	m_denFixedObject.getRookieSerial();

	m_denFixedObject.increaseCount();

	return	lpObject->m_wSerial;
}


static	cINDEX			LineUpObject[dMAX_OBJECT];
cSAVE_TINY_OBJECT_INFO	LineUpedObject[dMAX_OBJECT];
cINDEX					LineUpRape[100];
cINDEX					LineUpNormal[100];
cINDEX					LineUpFloat[100];

BOOL
cOBJECT::lineUpSaveObject(int index)
{
	int		mapHeight	=	g_lpActmap->m_iRealHeight*dTILE_REAL_HEIGHT;

	int		iRapeObjectCount	=	0;
	int		iNormalObjectCount	=	0;
	int		iFloatObjectCount	=	0;

	memset(LineUpRape,0xff,sizeof(LineUpRape));
	memset(LineUpNormal,0xff,sizeof(LineUpNormal));
	memset(LineUpFloat,0xff,sizeof(LineUpFloat));

	//	바닥에 깔리는 오브젝트 정렬
	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		if (m_pTinyObjectImage[lpObject->m_wObject].GetAttr()	==	dOBJECT_RAPE)
		{	LineUpObject[iRapeObjectCount].Index	=	iIndex;
			LineUpObject[iRapeObjectCount].Value	=	lpObject->m_pos.y;
			iRapeObjectCount++;
		}

		iCount++;
	}

	if (iRapeObjectCount>0)
	{
		qsort((void *)LineUpObject,iRapeObjectCount,sizeof(cINDEX),(int(*)(const void *,const void *))Compare);

		{
			for (int i=0;i<iRapeObjectCount;i++)
			{
				cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(LineUpObject[i].Index);
				LineUpedObject[i].m_pos.x		=	lpObject->m_pos.x;
				LineUpedObject[i].m_pos.y		=	lpObject->m_pos.y;
				LineUpedObject[i].m_wObject		=	lpObject->m_wObject;
			}
		}
		{
			for (int i=0;i<100;i++)
			{	LineUpRape[i].Index				=	i*iRapeObjectCount/100;
				LineUpRape[i].Value				=	LineUpedObject[i*iRapeObjectCount/100].m_pos.y;
			}
		}
		LineUpRape[99].Index				=	iRapeObjectCount-1;
		LineUpRape[99].Value				=	mapHeight;
	}

	//	일반 오브젝트 정렬
	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		if (m_pTinyObjectImage[lpObject->m_wObject].GetAttr()	==	dOBJECT_NORMAL)
		{	LineUpObject[iNormalObjectCount].Index	=	iIndex;
			LineUpObject[iNormalObjectCount].Value	=	lpObject->m_pos.y;
			iNormalObjectCount++;
		}

		iCount++;
	}

	if (iNormalObjectCount>0)
	{
		qsort((void *)LineUpObject,iNormalObjectCount,sizeof(cINDEX),(int(*)(const void *,const void *))Compare);
		
		{
			for (int i=0;i<iNormalObjectCount;i++)
			{
				cTINY_OBJECT_INFO	*lpObject				=	m_denTinyObject.get(LineUpObject[i].Index);
				LineUpedObject[iRapeObjectCount+i].m_pos.x	=	lpObject->m_pos.x;
				LineUpedObject[iRapeObjectCount+i].m_pos.y	=	lpObject->m_pos.y;
				LineUpedObject[iRapeObjectCount+i].m_wObject=	lpObject->m_wObject;
			}
		}
		{
			for (int i=0;i<100;i++)
			{	LineUpNormal[i].Index	=	iRapeObjectCount+i*iNormalObjectCount/100;
				LineUpNormal[i].Value	=	LineUpedObject[iRapeObjectCount+i*iNormalObjectCount/100].m_pos.y;
			}
		}
		LineUpNormal[99].Index		=	iRapeObjectCount+iNormalObjectCount-1;
		LineUpNormal[99].Value		=	mapHeight;
	}

	//	떠다니는 오브젝트 정렬
	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		if (m_pTinyObjectImage[lpObject->m_wObject].GetAttr()	==	dOBJECT_FLOAT)
		{	LineUpObject[iFloatObjectCount].Index	=	iIndex;
			LineUpObject[iFloatObjectCount].Value	=	lpObject->m_pos.y;
			iFloatObjectCount++;
		}

		iCount++;
	}

	if (iFloatObjectCount>0)
	{
		qsort((void *)LineUpObject,iFloatObjectCount,sizeof(cINDEX),(int(*)(const void *,const void *))Compare);

		{
			for (int i=0;i<iFloatObjectCount;i++)
			{
				cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(LineUpObject[i].Index);
				LineUpedObject[iRapeObjectCount+iNormalObjectCount+i].m_pos.x	=	lpObject->m_pos.x;
				LineUpedObject[iRapeObjectCount+iNormalObjectCount+i].m_pos.y	=	lpObject->m_pos.y;
				LineUpedObject[iRapeObjectCount+iNormalObjectCount+i].m_wObject	=	lpObject->m_wObject;
			}
		}
		{
			for (int i=0;i<100;i++)
			{	LineUpFloat[i].Index	=	iNormalObjectCount+iRapeObjectCount+i*iFloatObjectCount/100;
				LineUpFloat[i].Value	=	LineUpedObject[iNormalObjectCount+iRapeObjectCount+i*iFloatObjectCount/100].m_pos.y;
			}
		}
		LineUpFloat[99].Index		=	iNormalObjectCount+iRapeObjectCount+iFloatObjectCount-1;
		LineUpFloat[99].Value		=	mapHeight;
	}

	return TRUE;
}

//
//	뷰포트 안에 서있는(y 소트 해야 하는)오브젝트들을 구한다.
int
cOBJECT::getStandObjects(cINDEX *indexes,cRECT *viewport)
{	int start=0,end=-1,count=0;

	viewport->x1	-=	100;
	viewport->y1	-=	100;
	viewport->x2	+=	100;
	viewport->y2	+=	100;

	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		if (isStandObject(lpObject->m_wObject))
			if (viewport->isIN(lpObject->m_pos.x,lpObject->m_pos.y))
			{	
				indexes[count].Value	=	lpObject->m_pos.y;
				indexes[count].Index	=	iIndex;

				count++;
			}

		iCount++;
	}

	viewport->x1	+=	100;
	viewport->y1	+=	100;
	viewport->x2	-=	100;
	viewport->y2	-=	100;

	return count;
}	//	cOBJECT::GetObjects(cINDEX *indexes,cRECT *viewport)

//
//	뷰포트 안에 서있는(y 소트 해야 하는)오브젝트들을 구한다.
int
cOBJECT::getStandObjects(cINDEX *indexes)
{	int start=0,end=-1,count=0;

	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		if (isStandObject(lpObject->m_wObject))
		{	
			indexes[count].Value	=	lpObject->m_pos.y;
			indexes[count].Index	=	iIndex;

			count++;
		}

		iCount++;
	}

	return count;
}	//	cOBJECT::GetObjects(cINDEX *indexes,cRECT *viewport)

//
//	내부
void
cOBJECT::put(int dx,int dy,cTINY_OBJECT_INFO *item)
{
	putTinyObjectShadow(item->m_wObject,item->m_pos.x-dx,item->m_pos.y-dy);
	putTinyObject(item->m_wObject,item->m_pos.x-dx,item->m_pos.y-dy);

	if (item->m_isSelected)
		putTinyObjectLayer(item->m_wObject,item->m_pos.x-dx,item->m_pos.y-dy);
}

//
//	내부
void
cOBJECT::putScale(int dx,int dy,cTINY_OBJECT_INFO *item,int _iScale)
{
	dx	=	(item->m_pos.x-dx)*_iScale/100;
	dy	=	(item->m_pos.y-dy)*_iScale/100;

	putTinyObjectShadowScale(item->m_wObject,dx,dy,_iScale);
	putTinyObjectScale(item->m_wObject		,dx,dy,_iScale);
}

//
//	내부
void
cOBJECT::putScale(int dx,int dy,cTINY_OBJECT_INFO *item,int _iTBSize,int _iScale)
{
	dx	=	(item->m_pos.x-dx)*_iScale/100;
	dy	=	(item->m_pos.y-dy)*_iScale/100+_iTBSize;
	
	putTinyObjectShadowScale(item->m_wObject,dx,dy,_iScale);
	putTinyObjectScale(item->m_wObject,dx,dy,_iScale);

	if (item->m_isSelected)	putTinyObjectLayerScale(item->m_wObject,dx,dy,_iScale);
}	//	cOBJECT::put(int dx,int dy,cTINY_OBJECT_INFO *item)

//
//	찍어!!
void
cOBJECT::put(int dx,int dy,int index)
{
	cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(index);

	dx	=	lpObject->m_pos.x	-	dx;
	dy	=	lpObject->m_pos.y	-	dy;

	putTinyObjectShadow(lpObject->m_wObject,dx,dy);
	putTinyObject(lpObject->m_wObject,dx,dy);

	if (lpObject->m_isSelected)	putTinyObjectLayer(lpObject->m_wObject,dx,dy);
}	//	cOBJECT::put(int dx,int dy,int index)

//
//	찍어!!
void
cOBJECT::putScale(int dx,int dy,int index,int _iScale)
{
	cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(index);

	dx	=	(lpObject->m_pos.x	-	dx)*_iScale/100;
	dy	=	(lpObject->m_pos.y	-	dy)*_iScale/100;

	putTinyObjectShadowScale(lpObject->m_wObject,dx,dy,_iScale);
	putTinyObjectScale(lpObject->m_wObject,dx,dy,_iScale);
}	//	cOBJECT::put(int dx,int dy,int index)

//
//	찍어!!
void
cOBJECT::putScale(int dx,int dy,int index,int _iTBSize,int _iScale)
{
	cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(index);

	dx	=	(lpObject->m_pos.x-dx)*_iScale/100;
	dy	=	(lpObject->m_pos.y-dy)*_iScale/100+_iTBSize;
	//_iTBSize

	putTinyObjectShadowScale(lpObject->m_wObject,dx,dy,_iScale);
	putTinyObjectScale(lpObject->m_wObject,dx,dy,_iScale);
}	//	cOBJECT::put(int dx,int dy,int index)

//
//	바닥에 깔리는 오브젝트와 오버랩되는 오브젝트들을 몽땅 직어준다.
void
cOBJECT::drawAllRapeObject(cRECT *mapClient,int dx,int dy)
{
	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		if (isRapeObject(lpObject->m_wObject))
			if (mapClient->isIN(lpObject->m_pos.x,lpObject->m_pos.y))
				put(dx,dy,lpObject);

		iCount++;
	}
}	//	cOBJECT::DrawAll(cRECT *mapClient,int dx,int dy)

//
//	바닥에 깔리는 오브젝트와 오버랩되는 오브젝트들을 몽땅 직어준다.
void
cOBJECT::drawRapeObjectScale(cRECT *mapClient,int dx,int dy,int _iScale)
{
	if (_iScale == 100)	
	{
		drawAllRapeObject(mapClient,dx,dy);
		return;
	}

	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		if (isRapeObject(lpObject->m_wObject))
			if (mapClient->isIN(lpObject->m_pos.x,lpObject->m_pos.y))
				putScale(dx,dy,lpObject,_iScale);

		iCount++;
	}
}	//	cOBJECT::DrawAll(cRECT *mapClient,int dx,int dy)

//
//	바닥에 깔리는 오브젝트와 오버랩되는 오브젝트들을 몽땅 직어준다.
void
cOBJECT::drawAllRapeObjectScale(cRECT *mapClient,int dx,int dy,int _iScale)
{
	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		if (isRapeObject(lpObject->m_wObject))	putScale(dx,dy,lpObject,_iScale);

		iCount++;
	}
}	//	cOBJECT::DrawAll(cRECT *mapClient,int dx,int dy)

//
//	바닥에 깔리는 오브젝트와 오버랩되는 오브젝트들을 몽땅 직어준다.
void
cOBJECT::drawAllRapeObjectScale(cRECT *mapClient,int dx,int dy,int _iTBSize,int _iScale)
{
	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		if (isRapeObject(lpObject->m_wObject))	putScale(dx,dy,lpObject,_iTBSize,_iScale);

		iCount++;
	}
}	//	cOBJECT::DrawAll(cRECT *mapClient,int dx,int dy)

//
//	공중에 떠 있는 오브젝트들을 몽땅 직어준다.
void
cOBJECT::drawAllFloatObject(cRECT *mapClient,int dx,int dy)
{
	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_denTinyObject.getCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_denTinyObject.get(iIndex);

		if(!lpObject)	continue;

		if (isFloatObject(lpObject->m_wObject))
			if (mapClient->isIN(lpObject->m_pos.x,lpObject->m_pos.y))
				put(dx,dy,lpObject);

		iCount++;
	}
}	//	cOBJECT::DrawAllFloat(cRECT *mapClient,int dx,int dy)

//
//	툴윈도우에서 바닥에 고정되는 오브젝트를 선택한다.
void
cOBJECT::selectFixedObject(int object,BOOL shift,BOOL control)
{
	int		i;

	if (object >= m_iFixedObjectImageCount)	return;
	if (!m_pSelectFixedObjectList) return;

	if(shift)
	{
		int iMinObject	=	object;
		int iMaxObject	=	m_iSelectFixedObject;

		if (iMinObject > iMaxObject) swap(iMaxObject,iMinObject);

		memset(m_pSelectFixedObjectList,0,m_iFixedObjectImageCount);

		for (i=iMinObject;i<=iMaxObject;i++)	m_pSelectFixedObjectList[i]	=	1;
	}
	else
	if (control)
	{
		if (object != m_iSelectFixedObject)	m_pSelectFixedObjectList[object]	=	1	-	m_pSelectFixedObjectList[object]	;
	}
	else
	{
		memset(m_pSelectFixedObjectList,0,m_iFixedObjectImageCount);

		m_iSelectFixedObject	=	object;
		g_iSelectFidexedObject	=	m_iSelectFixedObject;
		m_pSelectFixedObjectList[object]	=	1;
	}

	m_iSelectFixedObjectCount=	0;

	for (i=0;i<m_iFixedObjectImageCount;i++)	if (m_pSelectFixedObjectList[i])	m_iSelectFixedObjectCount++;
}

//
//	툴윈도우에서 자유롭게 찍히는 오브젝트를 선택한다.
void
cOBJECT::selectTinyObject(int object,BOOL shift,BOOL control)
{
	int		i;

//	if (object >= m_iTinyObjectImageCount)	return;
	if (!m_pSelectTinyObjectList) return;

	if(shift	&&	object <	m_iTinyObjectImageCount)
	{
		int iMinObject	=	object;
		int iMaxObject	=	m_iSelectTinyObject;

		if (iMinObject > iMaxObject) swap(iMaxObject,iMinObject);

		memset(m_pSelectTinyObjectList,0,m_iTinyObjectImageCount);

		for (i=iMinObject;i<=iMaxObject;i++)	m_pSelectTinyObjectList[i]	=	1;
	}
	else
	if (control	&&	object <	m_iTinyObjectImageCount)
	{
		if (object != m_iSelectTinyObject)	m_pSelectTinyObjectList[object]	=	1	-	m_pSelectTinyObjectList[object];
	}
	else
	{
		memset(m_pSelectTinyObjectList,0,m_iTinyObjectImageCount);
		m_pSelectTinyObjectList[object]	=	1;

		m_iSelectTinyObject	=	object;
		g_iFreeObject	=	m_iSelectTinyObject;
	}

	m_iSelectTinyObjectCount=	0;

	for (i=0;i<m_iTinyObjectImageCount;i++)	if (m_pSelectTinyObjectList[i])	m_iSelectTinyObjectCount++;	//	선택된 오브젝트 수를 파악한다.
}	//	cOBJECT::selectTinyObject(int object,BOOL shift,BOOL control)

//
//	고정된 오브젝트 리스트 리빌드
void
cOBJECT::rebuildFixedObjectList()
{
	m_denFixedObject.removeValidItem();
}