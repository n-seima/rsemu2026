#include "cMAP.H"

//
//	선택된 건물들 제거
void
cMAP::removeSelectedBuilding()
{
	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<m_building.getMaxCount() && iCount < m_building.getCount();iIndex++)
	{
		cBUILDING_INFO	*lpBuilding	=	m_building.get(iIndex);

		if	(!lpBuilding)	continue;
		iCount++;

		if	(!lpBuilding->m_isSelected)	continue;

		int	x	=	lpBuilding->m_pos.x/dTILE_WIDTH;
		int	y	=	lpBuilding->m_pos.y/dTILE_HEIGHT;

		BYTE	*lpCrash	=	m_building.getCrashInfo(lpBuilding->m_wBuilding);

		setCrashMap(x,y,lpCrash,NULL,NULL,TRUE);
		setBuilding(x,y,0xff);

		LostVirgin();
	}

	m_building.removeSelectedAll();
}	//	cMAP::removeSelectedBuilding()


//
//	크래쉬,블랭크,오버랩 정보를 전부 다시 설정한다.
void
cMAP::remarkAllBuildingInfo(BOOL _isBuildCrashMap,BOOL _isBuildBlankMap,BOOL _isBuildOverlapMap)
{
	BYTE	*lpCrash	=	NULL;
	BYTE	*lpBlank	=	NULL;
	BYTE	*lpOverlap	=	NULL;

	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<m_building.getMaxCount() && iCount < m_building.getCount();iIndex++)
	{
		cBUILDING_INFO	*lpBuilding	=	m_building.get(iIndex);

		if	(!lpBuilding)	continue;

		int	iBuilding	=	lpBuilding->m_wBuilding;

		int	mx			=	lpBuilding->m_pos.x/dTILE_WIDTH;
		int	my			=	lpBuilding->m_pos.y/dTILE_HEIGHT;

		if (_isBuildCrashMap)	lpCrash		=	m_building.getCrashInfo(iBuilding);
		if (_isBuildBlankMap)	lpBlank		=	m_building.getBlankInfo(iBuilding);
		if (_isBuildOverlapMap)	lpOverlap	=	m_building.getOverlapInfo(iBuilding);

		setCrashMap(mx,my,lpCrash,lpBlank,lpOverlap,FALSE);

		m_pBuildingMap[mx+my*m_iRealWidth]	=	(BYTE)lpBuilding->m_wSerial;
		iCount++;
	}
}	//	cMAP::remarkAllBuildingInfo(BOOL _isBuildCrashMap,BOOL _isBuildBlankMap,BOOL _isBuildOverlapMap)

//
//	이 건물.. _iMx,_iMy에 찍으려고 하는데.. 충돌 하냐?
BOOL
cMAP::isCrashBuilding(int _iMx,int _iMy,int _iBuilding)
{
	if (_iMx < 0 || _iMy < 0 || _iMx >= m_iRealWidth || _iMy >= m_iRealHeight) return TRUE;

	if (getBuildingImageCount()	<=	0)	return	FALSE;

	BYTE	*lpCrash	=	m_building.getCrashInfo(_iBuilding);

	if (!isBuildAblePos(_iMx,_iMy,lpCrash)) return TRUE;

	return FALSE;
}	//	cMAP::isCrashBuilding(int _iMx,int _iMy,int _iBuilding)

//
//	건물 추가
BOOL
cMAP::buildBuilding(int _iX,int _iY,int _iBuilding)
{
	if (_iX < 0 || _iY < 0 || _iX >= m_iRealWidth || _iY >= m_iRealHeight) return FALSE;

	if (getBuildingImageCount()	<=	0)	return	FALSE;

	BYTE	*lpCrashInfo	=	m_building.getCrashInfo(_iBuilding);

	if (!isBuildAblePos(_iX,_iY,lpCrashInfo))	return	FALSE;

	int	iBuildingSerial		=	m_building.add(_iX*dTILE_WIDTH,_iY*dTILE_HEIGHT,_iBuilding);

	if	(iBuildingSerial	==	0xffff)	return	FALSE;

	if (!setCrashMap(_iX,_iY,lpCrashInfo,NULL,NULL,FALSE))
	{	
		_iBuilding	=	getBuilding(_iX,_iY);

		setCrashMap(_iX,_iY,lpCrashInfo,NULL,NULL,TRUE);

		m_building.remove(iBuildingSerial);

		return TRUE;
	}

	setBuilding(_iX,_iY,iBuildingSerial);

	return TRUE;
}	//	cMAP::buildBuilding(int _iX,int _iY,int _iBuilding)

//
//	에드온 오브젝트를 붙일 준비가된 건물-_-
cBUILDING_INFO *
cMAP::getEditAddonObjectBuilding()
{
	int	iIndex,iCount;
	int	iSelectBuilding=0xffff,iSelectBuildingCount=0;

	cBUILDING_INFO *lpBuilding;

	for (iIndex=0,iCount=0;iIndex<dMAX_BUILDING_COUNT_IN_MAP && iCount < m_building.getCount();iIndex++)
	{
		lpBuilding	=	m_building.get(iIndex);

		if(!lpBuilding)	continue;

		iCount++;

		if (lpBuilding->m_isSelected)
		{
			iSelectBuildingCount++;
			iSelectBuilding	=	iIndex;

			if (iSelectBuildingCount	>	1)	return NULL;
		}
	}

	if (iSelectBuilding			==	0xffff	)	return NULL;

	lpBuilding	=	m_building.get(iSelectBuilding);

	return	lpBuilding;
}	//	cMAP::getEditAddonObjectBuilding()


//
//	에드온 오브젝트 추가.. -_-a
BOOL
cMAP::addAddonObjectOnBuilding(int _iObject,int _iX,int _iY)
{
	cBUILDING_INFO	*lpBuilding	=	m_lpEditAddonObjectBuilding;
	
	if (!lpBuilding)	return	FALSE;

	LostVirgin();

	lpBuilding->addAddonObject(_iObject,_iX-dTILE_WIDTH/2,_iY-dTILE_HEIGHT/2);

	return	TRUE;
}	//	cMAP::addAddonObject(int _iObject,int _iX,int _iY)

//
//돌려~
void
cMAP::cycleSelectAddonObjectOnBuilding()
{
	cBUILDING_INFO	*lpBuilding	=	m_lpEditAddonObjectBuilding;
	
	if (!lpBuilding)	return;

	m_building.m_iSelectAddonObject++;

	if (m_building.m_iSelectAddonObject	>=	dMAX_ADDON_OBJECT_COUNT_ON_BUILDING)	
	{
		m_building.m_iSelectAddonObject	=	0;
		return;
	}

	if	(lpBuilding->m_aAddonObject[m_building.m_iSelectAddonObject].m_wObject	==	0xffff)
	{
		m_building.m_iSelectAddonObject	=	0;
		return;
	}
}	//	cMAP::cycleSelectAddonObject()

//
//	선택된 애드온-_- 오브젝트 제거
void
cMAP::removeSelectAddonObjectOnBuilding()
{
	if (m_building.m_iSelectAddonObject	>=	dMAX_ADDON_OBJECT_COUNT_ON_BUILDING)	return;

	cBUILDING_INFO	*lpBuilding	=	m_lpEditAddonObjectBuilding;
	
	if (!lpBuilding)	return;

	if	(lpBuilding->m_aAddonObject[m_building.m_iSelectAddonObject].m_wObject	==	0xffff)	return;

	LostVirgin();

	lpBuilding->removeAddonObject(m_building.m_iSelectAddonObject);

	if	(lpBuilding->m_aAddonObject[m_building.m_iSelectAddonObject].m_wObject	==	0xffff)
	{
		m_building.m_iSelectAddonObject	=	0;
		return;
	}
}	//	cMAP::removeSelectAddonObject()

//
//	이동
void
cMAP::moveSelectAddonObjectOnBuilding(int _iDx,int _iDy)
{
	if (m_building.m_iSelectAddonObject	>=	dMAX_ADDON_OBJECT_COUNT_ON_BUILDING)	return;

	cBUILDING_INFO	*lpBuilding	=	m_lpEditAddonObjectBuilding;
	
	if (!lpBuilding)	return;

	if	(lpBuilding->m_aAddonObject[m_building.m_iSelectAddonObject].m_wObject	==	0xffff)	return;

	LostVirgin();

	lpBuilding->moveAddonObject(m_building.m_iSelectAddonObject,_iDx,_iDy);
}	//	cMAP::moveSelectAddonObject(int _iDx,int _iDy)