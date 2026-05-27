#include "cFIELD.h"
#include "objectImage.h"

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
		if (rect.Isin(m_aDoorList[i].m_wX,m_aDoorList[i].m_wY))
			return	TRUE;

	return	FALSE;
}

BOOL
cDoorManager::getDoorStatus(int _iAddress)
{
	for (int i=0;i<m_iDoorCount;i++)
	{
		if (m_aDoorList[i].m_iPosValue	==	_iAddress)
			return	m_aDoorList[i].m_wStatus;
	}

	return	-1;
}

void
cFIELD::changeDoorCrashMap(cAreaDoorInfo *_lpDoor,BOOL _bIsOpen)
{
	for (int i=0;i<dLINK_WITH_AREA_DOOR_COUNT;i++)
	{
		int	iObject	=	_lpDoor->m_awLinkObject[i];

		if (iObject	==	0xffff)
			continue;

		if(_bIsOpen)
			setDoorCrashMap(iObject,TRUE);
		else
			setDoorCrashMap(iObject,FALSE);
	}

	if (_bIsOpen	==	FALSE)
		moveActorToSafePosInArea(_lpDoor->m_wSerial);
}

void
cFIELD::getLinkObjectWithDoor(cAreaDoorInfo *_lpDoor)
{
	int		iCount	=	0;
	cRECT	rect;

	rect.x1		=	_lpDoor->x1>>dTILE_XSIZE_SHIFT;
	rect.y1		=	_lpDoor->y1>>dTILE_YSIZE_SHIFT;
	rect.x2		=	_lpDoor->x2>>dTILE_XSIZE_SHIFT;
	rect.y2		=	_lpDoor->y2>>dTILE_YSIZE_SHIFT;

	memset(_lpDoor->m_awLinkObject,0xff,sizeof(_lpDoor->m_awLinkObject));
	
	if (m_pDoors)
		for (int i=0;i<m_pDoors->m_iDoorCount;i++)
		{
			cDoorObjectInfo	*lpDoor	=	&m_pDoors->m_aDoorList[i];

			if (rect.Isin(lpDoor->m_wX,lpDoor->m_wY))
			{
				_lpDoor->m_awLinkObject[iCount++]	=	i;

				if (iCount	>=	dLINK_WITH_AREA_DOOR_COUNT)
					return;
			}
		}
}


//	BLANK 타일과,BLOCKED 타일을 설정한다.
void
cFIELD::setDoorCrashMap(int _iDoor,BOOL _bIsClear)
{	
	cDoorObjectInfo		*lpDoor		=	&m_pDoors->m_aDoorList[_iDoor];
	BYTE				*lpbCrashInfo=	g_ociManager.m_aData[m_wTileSet][lpDoor->m_wObjectImage].m_pCrashInfo;

	setCrashMap(lpDoor->m_wX,lpDoor->m_wY,lpbCrashInfo,_bIsClear);
}

//	BLANK 타일과,BLOCKED 타일을 설정한다.
void
cFIELD::setCrashMap(int x,int y,BYTE *_lpCrash,BOOL _bIsClear)
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
				int	iAddress		=	xPoint+pos+(yp+j)*m_iWidth;

				m_pMap[iAddress]	=	1-_bIsClear;
			}

			iBufferPoint	+=	size;
			xPoint			+=	size;
		}
	}
}

BOOL
cFIELD::operateDoorWorkInAreaByKarma(int _iArea,int _iWork)
{
	cAreaInfo	*lpWorkArea	=	getArea(_iArea);

	if	(!lpWorkArea)
		return	FALSE;

	cRECT		*lpWorkRect	=	(cRECT	*)&lpWorkArea->x1;

	for (int i=0;i<dMAX_AREA;i++)
	{
		cAreaInfo	*lpArea	=	getArea(i);

		if (!lpArea || lpArea->m_wKind	!=	dAREA_DOOR)
			continue;

		cRECT	*lpRect		=	(cRECT	*)&lpArea->x1;

		if (!lpWorkRect->Crash(lpRect))
			continue;

		cAreaDoorInfo	*lpDoorArea	=	(cAreaDoorInfo	*)lpArea;

		if (_iWork	==	eADTWK_ENABLE_DOOR)
		{
			lpDoorArea->m_bf1IsActiveByKarma	=	FALSE;
			lpDoorArea->sendDoorStatus();
			continue;
		}

		if (_iWork	==	eADTWK_DISABLE_DOOR)
		{
			lpDoorArea->m_bf1IsActiveByKarma	=	TRUE;
			lpDoorArea->sendDoorStatus();
			continue;
		}

		if (lpDoorArea->m_bf1IsActive	==	FALSE)
		{
			if (_iWork	==	eADTWK_REMOVE)
				continue;

			if (_iWork	==	eADTWK_REGEN)
			{
				lpDoorArea->appear(0xffff);
				lpDoorArea->sendDoorStatus();
				continue;
			}
		}

		if	(_iWork	==	eADTWK_REGEN)
			continue;

		if	(_iWork	==	eADTWK_REMOVE)
		{
			lpDoorArea->disapper(0xffff);
			lpDoorArea->sendDoorStatus();
			continue;
		}

		switch(_iWork)
		{
			case	eADTWK_LOCK		:
				lpDoorArea->lock(255);
				lpDoorArea->sendDoorStatus();
				break;

			case	eADTWK_UNLOCK	:
				lpDoorArea->unlock(0xffff);
				lpDoorArea->sendDoorStatus();
				break;

			case	eADTWK_CLOSE	:
//				lpDoorArea->m_bf1IsActiveByKarma	=	TRUE;
				lpDoorArea->close();
				lpDoorArea->sendDoorStatus();
				break;
			case	eADTWK_OPEN		:
//				lpDoorArea->m_bf1IsActiveByKarma	=	TRUE;
				lpDoorArea->open();
				lpDoorArea->sendDoorStatus();
				break;
		}
	}

	return	TRUE;
}
