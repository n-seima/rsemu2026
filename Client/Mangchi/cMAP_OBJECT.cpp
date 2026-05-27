#include "cMAP.H"

//
//	선택된 오브젝트들 제거
void
cMAP::removeSelectedObject()
{
	int	iIndex=0,iCount=0;
	int	iObjectCount	=	m_object.getTinyObjectCount();

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < iObjectCount;iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_object.getTinyObject(iIndex);

		if(!lpObject)	continue;

		iCount++;

		if(!lpObject->m_isSelected)	continue;

		int	iObject	=	lpObject->m_wObject;

		if (m_object.isFixedObject(iObject))
		{
			int	mx	=	lpObject->m_pos.x/(dTILE_WIDTH/dSHARE_TILE);
			int	my	=	lpObject->m_pos.y/(dTILE_HEIGHT/dSHARE_TILE);

			BYTE	*lpCrash	=	m_object.getTinyObjectCrashInfo(iObject);

			setCrashMap(mx,my,lpCrash,NULL,NULL,TRUE);
		}
	}

	iObjectCount	=	m_object.getFixedObjectCount();

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < iObjectCount;iIndex++)
	{
		cFIXED_OBJECT_INFO	*lpObject	=	m_object.getFixedObject(iIndex);

		if(!lpObject)	continue;

		iCount++;

		if(!lpObject->m_isSelected)	continue;

		int	iObject	=	lpObject->m_wObject;

		int	mx	=	lpObject->m_pos.x/(dTILE_WIDTH/dSHARE_TILE);
		int	my	=	lpObject->m_pos.y/(dTILE_HEIGHT/dSHARE_TILE);

		BYTE	*lpCrash	=	m_object.getFixedObjectCrashInfo(iObject);

		setCrashMap(mx,my,lpCrash,NULL,NULL,TRUE);
		setObject(0xffff,mx,my);
	}

	m_object.removeSelectedAll();
}	//	cMAP::removeSelectedObject()


//
//	크래쉬,블랭크,오버랩 정보를 전부 다시 설정한다.
void
cMAP::remarkAllObjectInfo(BOOL _isBuildCrashMap,BOOL _isBuildBlankMap,BOOL _isBuildOverlapMap)
{
	BYTE	*lpCrash	=	NULL;
	BYTE	*lpBlank	=	NULL;
	BYTE	*lpOverlap	=	NULL;

	int		iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_object.getTinyObjectCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_object.getTinyObject(iIndex);

		if(!lpObject)	continue;

		iCount++;

		int	iObject	=	lpObject->m_wObject;

		if (m_object.isFixedObject(iObject))
		{
			int		mx			=	lpObject->m_pos.x/(dTILE_WIDTH/dSHARE_TILE);
			int		my			=	lpObject->m_pos.y/(dTILE_HEIGHT/dSHARE_TILE);

			if (_isBuildCrashMap)	lpCrash		=	m_object.getTinyObjectCrashInfo(iObject);
			if (_isBuildBlankMap)	lpBlank		=	m_object.getTinyObjectBlankInfo(iObject);
			if (_isBuildOverlapMap)	lpOverlap	=	m_object.getTinyObjectOverlapInfo(iObject);

			setCrashMap(mx,my,lpCrash,lpBlank,lpOverlap,FALSE);
		}
	}

//	m_object.rebuildFixedObjectList();
	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_object.getFixedObjectCount();iIndex++)
	{
		cFIXED_OBJECT_INFO	*lpObject	=	m_object.getFixedObject(iIndex);
		if(!lpObject)
			continue;

		cOBJ_ANM			*lpImage	=	m_object.getFixedObjectImage(lpObject->m_wObject);

		iCount++;

		int	iObject	=	lpObject->m_wObject;

		int	mx	=	lpObject->m_pos.x/(dTILE_WIDTH/dSHARE_TILE);
		int	my	=	lpObject->m_pos.y/(dTILE_HEIGHT/dSHARE_TILE);

		if (_isBuildCrashMap)	lpCrash		=	m_object.getFixedObjectCrashInfo(iObject);
		if (_isBuildBlankMap)	lpBlank		=	m_object.getFixedObjectBlankInfo(iObject);
		if (_isBuildOverlapMap)	lpOverlap	=	m_object.getFixedObjectOverlapInfo(iObject);

		setCrashMap(mx,my,lpCrash,lpBlank,lpOverlap,FALSE,lpImage->IsMissileBlockObject());
		setObject(iIndex,mx,my);
	}
}

//
//	브릿지 정보 설정
void
cMAP::remarkAllBridgeInfo()
{
	BYTE	*lpCrash	=	NULL;

	int		iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_object.getTinyObjectCount();iIndex++)
	{
		cTINY_OBJECT_INFO	*lpObject	=	m_object.getTinyObject(iIndex);

		if	(!lpObject)
			continue;

		iCount++;

		int	iObject	=	lpObject->m_wObject;

		if	(!m_object.isFixedObject(iObject))
			continue;
		if	(!m_object.isBridgeObject(iObject))
			continue;

		int		mx			=	lpObject->m_pos.x/(dTILE_WIDTH/dSHARE_TILE);
		int		my			=	lpObject->m_pos.y/(dTILE_HEIGHT/dSHARE_TILE);

		lpCrash		=	m_object.getTinyObjectCrashInfo(iObject);

		setBridgeMap(mx,my,lpCrash);
	}
}	//	cMAP::remarkAllBridgeInfo()

//
//이 오브젝트를 _iMx,_iMy에 찍으려고 하는데.. 충돌 하냐?
BOOL
cMAP::isCrashFixedObject(int _iObject,int _iMx,int _iMy)
{
	if (_iMx < 0 || _iMy < 0 || _iMx >= m_iRealWidth || _iMy >= m_iRealHeight) return TRUE;

	BYTE	*lpCrash	=	m_object.getFixedObjectCrashInfo(_iObject);

	if (!isBuildAblePos(_iMx,_iMy,lpCrash)) return TRUE;

	return	FALSE;
}	//	cMAP::isCrashFixedObject(int _iObject,int _iMx,int _iMy)

//
//	오브젝트 추가
BOOL
cMAP::buildObject(int _iObject,int _iX,int _iY)
{
	if (_iX < 0 || _iY < 0 || _iX >= m_iRealWidth || _iY >= m_iRealHeight) return FALSE;

	if (m_object.m_iFixedObjectImageCount	<=	0)	return	FALSE;

	_iObject				=	m_object.getSelectFixedObject(_iObject);

	BYTE	*lpCrashInfo	=	m_object.getFixedObjectCrashInfo(_iObject);

	cOBJ_ANM	*lpObject	=	m_object.getFixedObjectImage(_iObject);

	if (!isBuildAblePos(_iX,_iY,lpCrashInfo))	return	FALSE;

	int	iObjectSerial		=	m_object.addFixedObject(_iObject,_iX*dTILE_WIDTH,_iY*dTILE_HEIGHT);

	if	(iObjectSerial		==	0xffff)	return	FALSE;

	BOOL	bIsMissileBlockObject	=	lpObject->IsMissileBlockObject();

	if (!setCrashMap(_iX,_iY,lpCrashInfo,NULL,NULL,FALSE,bIsMissileBlockObject))
	{
		setCrashMap(_iX,_iY,lpCrashInfo,NULL,NULL,TRUE,bIsMissileBlockObject);

		m_object.removeFixedObject(iObjectSerial);

		return FALSE;
	}

	setObject(iObjectSerial,_iX,_iY);

	return TRUE;
}	//	cMAP::buildObject(int _iObject,int _iX,int _iY)

//
//	쬐그만 오브젝트 추가
BOOL
cMAP::addTinyObject(int _iObject,int _iX,int _iY)
{
	int	iMx	=	_iX/dTILE_WIDTH;
	int	iMy	=	_iY/dTILE_HEIGHT;

	if (iMx < 0 || iMy < 0 || iMx >= m_iRealWidth || iMy >= m_iRealHeight) return FALSE;

	_iObject				=	m_object.getSelectTinyObject(_iObject);

	if (m_object.isFixedObject(_iObject))
	{
		_iX	=	iMx*dTILE_WIDTH/dSHARE_TILE + dTILE_WIDTH /dSHARE_TILE /2;
		_iY	=	iMy*dTILE_HEIGHT/dSHARE_TILE+ dTILE_HEIGHT/dSHARE_TILE /2;

		BYTE	*lpCrashInfo	=	m_object.getTinyObjectCrashInfo(_iObject);

		if (!isBuildAblePos(iMx,iMy,lpCrashInfo))	return	FALSE;
	}

	int	iObjectSerial		=	m_object.addTinyObject(_iObject,_iX,_iY);

	if	(iObjectSerial		==	0xffff)	return	FALSE;

	if (m_object.isFixedObject(_iObject))
	{
		BYTE	*lpCrashInfo	=	m_object.getTinyObjectCrashInfo(_iObject);

		if (!setCrashMap(iMx,iMy,lpCrashInfo,NULL,NULL,FALSE))
		{
			setCrashMap(iMx,iMy,lpCrashInfo,NULL,NULL,TRUE);

			m_object.removeTinyObject(iObjectSerial);

			return FALSE;
		}
	}

	LostVirgin();

	return TRUE;
}	//	cMAP::addTinyObject(int _iObject,int _iX,int _iY)

cFIXED_OBJECT_INFO *
cMAP::getEditAddonObject()
{
	int	iIndex,iCount;
	int	iSelectObject=0xffff,iSelectObjectCount=0;

	cFIXED_OBJECT_INFO	*lpObject;

	for (iIndex=0,iCount=0;iIndex<dMAX_OBJECT&& iCount < m_object.getFixedObjectCount();iIndex++)
	{
		lpObject	=	m_object.getFixedObject(iIndex);

		if(!lpObject)	continue;

		iCount++;

		if (lpObject->m_isSelected)
		{
			iSelectObjectCount++;
			iSelectObject	=	iIndex;

			if (iSelectObjectCount	>	1)	return NULL;
		}
	}

	if (iSelectObject==0xffff)	return NULL;

	if (iSelectObjectCount	<=	0)	return NULL;

	lpObject	=	m_object.getFixedObject(iSelectObject);

	return	lpObject;
}	//	cMAP::getEditAddonObject()

//
//	에드온 오브젝트 추가.. -_-a
BOOL
cMAP::addAddonObject(int _iObject,int _iX,int _iY)
{
	cFIXED_OBJECT_INFO	*lpObject	=	m_lpEditAddonObject;

	if (!lpObject)
		return	addAddonObjectOnBuilding(_iObject,_iX,_iY);

	LostVirgin();

	lpObject->addAddonObject(_iObject,_iX-dTILE_WIDTH/2,_iY-dTILE_HEIGHT/2);

	return	TRUE;
}	//	cMAP::addAddonObject(int _iObject,int _iX,int _iY)

//
//돌려~
void
cMAP::cycleSelectAddonObject()
{
	cFIXED_OBJECT_INFO	*lpObject=m_lpEditAddonObject;

	if (!lpObject)
	{
		cycleSelectAddonObjectOnBuilding();
		return;
	}

	m_object.m_iSelectAddonObject++;

	if (m_object.m_iSelectAddonObject	>=	dMAX_ADDON_IMAGE_COUNT)	
	{
		m_object.m_iSelectAddonObject	=	0;
		return;
	}

	if	(lpObject->m_aAddonObject[m_object.m_iSelectAddonObject].m_wObject	==	0xffff)
	{
		m_object.m_iSelectAddonObject	=	0;
		return;
	}
}	//	cMAP::cycleSelectAddonObject()

//
//	선택된 애드온-_- 오브젝트 제거
void
cMAP::removeSelectAddonObject()
{
	if (m_object.m_iSelectAddonObject	>=	dMAX_ADDON_IMAGE_COUNT)
	{
		removeSelectAddonObjectOnBuilding();
		return;
	}

	cFIXED_OBJECT_INFO	*lpObject=m_lpEditAddonObject;

	if (!lpObject)
	{
		removeSelectAddonObjectOnBuilding();
		return;
	}

	if	(lpObject->m_aAddonObject[m_object.m_iSelectAddonObject].m_wObject	==	0xffff)	return;

	LostVirgin();

	lpObject->removeAddonObject(m_object.m_iSelectAddonObject);

	if	(lpObject->m_aAddonObject[m_object.m_iSelectAddonObject].m_wObject	==	0xffff)
	{
		m_object.m_iSelectAddonObject	=	0;
		return;
	}
}	//	cMAP::removeSelectAddonObject()

//
//	이동
void
cMAP::moveSelectAddonObject(int _iDx,int _iDy)
{
	if (m_object.m_iSelectAddonObject	>=	dMAX_ADDON_IMAGE_COUNT)
	{
		moveSelectAddonObjectOnBuilding(_iDx,_iDy);
		return;
	}

	cFIXED_OBJECT_INFO	*lpObject=m_lpEditAddonObject;

	if (!m_lpEditAddonObject)
	{
		moveSelectAddonObjectOnBuilding(_iDx,_iDy);
		return;
	}

	LostVirgin();

	if	(lpObject->m_aAddonObject[m_object.m_iSelectAddonObject].m_wObject	==	0xffff)	return;

	lpObject->moveAddonObject(m_object.m_iSelectAddonObject,_iDx,_iDy);

}	//	cMAP::moveSelectAddonObject(int _iDx,int _iDy)