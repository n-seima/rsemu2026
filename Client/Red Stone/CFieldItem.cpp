#include "CFieldItem.H"
#include "cMAP.H"
#include "CImageData.H"
#include "CHero.H"
#include "CActor.H"
#include "CMessage.H"

cFieldItem	g_fieldItem;

#define	dFIELD_ITEM_NAME_BOX_HEIGHT		19
#define	dFIELD_ITEM_NAME_BOX_ADD_WIDTH	10

cRECT	l_aRect[100];

class	cNameBox
{
public:
	cRECT	m_crashBox;
	char	m_strText[128];
	int		m_iSerial;
	int		m_iType;
};

cNameBox	l_aNameTagBox[1024];
int			l_iNameTagBoxCount;

char*
cDroppedItem::getName()
{
	static	char	strName[256];

	if	(m_wBaseItem	==	dITEM_MONEY)
	{
		if (m_dwCount	<	0xffff)
			sprintf(strName,dMSG_GOLD_FORM,m_dwCount);
		else
			strcpy(strName,dMSG_MANY_GOLD);
	}
	else
	{
		char	strColor[32];

		if	(getBasicItem()->m_attr.isExtraItem)
			strcpy(strColor,"CTBLUE");
		else
		switch(m_wType)
		{
			case	eMIT_NORMAL			:
				strcpy(strColor,"WHITE");
//				dwTextColor	=	dINC_NORMAL;
				break;
			case	eMIT_RARE			:
				strcpy(strColor,"BTBLUE");
//				dwTextColor	=	dINC_RARE;
				break;
			case	eMIT_LITTLE_GOOD	:
				strcpy(strColor,"PURPLE");
//				dwTextColor	=	dINC_LITTLE_GOOD;
				break;
			case	eMIT_UNIQUE			:
				strcpy(strColor,"GOLDYELLOW");
//				dwTextColor	=	dINC_UNIQUE;
				break;
			case	eMIT_SUPER_UNIQUE	:
				strcpy(strColor,"LTPURPLE");
//				dwTextColor	=	LTPURPLE;
				break;

			case	eMIT_PRINCESS_WEAPON	:
				strcpy(strColor,"LTPINK");
//				dwTextColor	=	LTPURPLE;
				break;
		}

		if	(m_dwCount	>	1	&&	getBasicItem()->m_attr.isExtraItem	==	FALSE)
			sprintf(strName,dMSG_ITEMS_FORM,strColor,getBasicItem()->m_strName,m_dwCount);
		else
			sprintf(strName,"<c:%s>%s<n>",strColor,getBasicItem()->m_strName);
	}

	return	strName;
}

void
cDroppedItem::put(int _iDx,int _iDy,BOOL _isOutline)
{
	int	iShape		=	g_aBasicItem[m_wBaseItem].m_wFieldShape;
	int	iScale		=	100;

	if	(iShape		==	0xffff)
		return;

	if (m_wBaseItem	==	dITEM_MONEY)
	{
				if (m_dwCount	>=	1000)	iShape+=3;
		else	if (m_dwCount	>=	100)	iShape+=2;
		else	if (m_dwCount	>=	10)		iShape+=1;
	}

	int	iX	=	m_pos.x-_iDx;
	int	iY	=	m_pos.y-_iDy;

	if (g_bIsHalfSize)
	{
		iX		=	(m_pos.x-_iDx*2)/2;
		iY		=	(m_pos.y-_iDy*2)/2;
		iScale	=	50;
	}

	g_sprFieldItem.PutShadow(iX,iY,iShape,iScale,iScale);
	g_sprFieldItem.Put(iX,iY,iShape,iScale,iScale);

	if (_isOutline)
		g_sprFieldItem.PutLayer(iX,iY,iShape,_LTGREEN,iScale,iScale);
}


cFieldItem::cFieldItem()
{
	init();
	
	reset();
}

cFieldItem::~cFieldItem()
{
	close();
}

//
//리셋
void
cFieldItem::reset()
{
	m_den.reset();

	m_iFocusItem				=	0xffff;
	m_iPickItem					=	0xffff;

	m_iFrameCounter				=	0xffff;
	m_iFrame					=	0;

	m_iRemainTimeForShinigItem	=	0;
	m_bIsShowDroppedItemName	=	FALSE;
}

//
//_iSerial제거
void
cFieldItem::remove(int _iSerial)
{
	m_den.remove(_iSerial);

	if (m_iFocusItem	==	_iSerial)
	{
		m_iFocusItem		=	0xffff;
		m_iPickItem			=	0xffff;
		s_iFocusFieldItem	=	0xffff;
	}
}

//
//	필드에 떨어진 아이템 모두 그리기
void
cFieldItem::draw(int _iDx,int _iDy)
{
	int	iCount=0,i;

	for (i=0;i<dFIELD_ITEM_COUNT && iCount < m_den.getCount();i++)
	{
		cDroppedItem	*lpDroppedItem	=	m_den.get(i);

		if (lpDroppedItem	==	NULL)
			continue;

		if (g_rectViewPort.isIn(lpDroppedItem->m_pos.x,lpDroppedItem->m_pos.y))
			lpDroppedItem->put(_iDx,_iDy,FALSE);

		iCount++;
	}
}

//
//	필드에 떨어진 아이템 모두 그리기
void
cFieldItem::drawAllFieldItemName(int _iDx,int _iDy)
{
	for (int i=0,iCount=0;i<dFIELD_ITEM_COUNT && iCount < m_den.getCount();i++)
	{
		cDroppedItem	*lpDroppedItem	=	m_den.get(i);

		if (lpDroppedItem	==	NULL)
			continue;

		if (!g_rectViewPort.isIn(lpDroppedItem->m_pos.x,lpDroppedItem->m_pos.y))
			continue;

		if	(	lpDroppedItem->m_rect.x1	==	0	&&	
				lpDroppedItem->m_rect.y1	==	0	&&	
				lpDroppedItem->m_rect.x2	==	0	&&	
				lpDroppedItem->m_rect.y2	==	0)
			continue;


		cRECT	rect;

		int		iX		=	GetScaledXPos(lpDroppedItem->m_rect.x1);
		int		iY		=	GetScaledYPos(lpDroppedItem->m_rect.y1);
		int		iX2		=	GetScaledXPos(lpDroppedItem->m_rect.x2);
		int		iY2		=	GetScaledYPos(lpDroppedItem->m_rect.y2);

		rect.set(iX,iY,iX2,iY2);

		if	(m_iFocusItem	==	i)
		{
			s_ttDialogFrame.drawBox(&rect,0xffff,32);
			s_text.pushShadowStatus(FALSE);
			s_text.colorChangeOff();
			s_text.blinkTextOn();
			s_text.cPut(rect.x1+rect.getWidth()/2,rect.y1+3,0,lpDroppedItem->getName());
			s_text.blinkTextOff();
			s_text.colorChangeOn();
			s_text.popShadowStatus();
		}
		else
		{
			s_ttCommon.drawBox(&rect,0,32);
			s_text.cPut(rect.x1+rect.getWidth()/2,rect.y1+3,WHITE,lpDroppedItem->getName());
		}

		iCount++;
	}
}

//
//	필드에 떨어진 아이템 모두 그리기


void
cFieldItem::addPaint(int _iDx,int _iDy)
{
	int	iCount=0,i;

	if (m_iFocusItem	!=	0xffff)	//	포커스가 있는 아이템은 한번 더 찍어준다.
	{
		cDroppedItem	*lpDroppedItem	=	m_den.get(m_iFocusItem);

		if (lpDroppedItem	==	NULL)
			m_iFocusItem	=	0xffff;
		else
			lpDroppedItem->put(_iDx,_iDy,TRUE);
	}

	if(m_bIsShowDroppedItemName + g_config.m_bf1IsShowDropItem)
	{
		drawAllFieldItemName(_iDx,_iDy);
		return;
	}

	if (m_iFocusItem	!=	0xffff)
	{
		cDroppedItem	*lpFocusItem	=	m_den.get(m_iFocusItem);

		if (lpFocusItem)
		{
			char	strName[256];
			cRECT	rect;

			strcpy(strName,lpFocusItem->getName());

			int		iPixelSize	=	s_text.getPixelSize(strName)+dFIELD_ITEM_NAME_BOX_ADD_WIDTH;
			int		iX			=	lpFocusItem->m_pos.x;
			int		iY			=	lpFocusItem->m_pos.y;

			iY		=	iY/dFIELD_ITEM_NAME_BOX_HEIGHT*dFIELD_ITEM_NAME_BOX_HEIGHT;
			iX		-=	iPixelSize/2;
			iY		-=	dFIELD_ITEM_NAME_BOX_HEIGHT/2;

			iX		=	GetScaledXPos(iX);
			iY		=	GetScaledYPos(iY);

			rect.set(iX,iY,iX+iPixelSize,iY+dFIELD_ITEM_NAME_BOX_HEIGHT-2);

			s_ttCommon.drawBox(&rect,0,32);
			s_text.cPut(rect.x1+rect.getWidth()/2,rect.y1+3,WHITE,strName);
		}
	}

	if (m_iFrameCounter	==	0xffff)
		return;

	for (i=0;i<dFIELD_ITEM_COUNT && iCount < m_den.getCount();i++)	//	반짝이
	{
		cDroppedItem	*lpDroppedItem	=	m_den.get(i);

		if (lpDroppedItem	==	NULL)
			continue;

		if (!g_rectViewPort.isIn(lpDroppedItem->m_pos.x,lpDroppedItem->m_pos.y))
			continue;

		int	x		=	GetScaledXPos(lpDroppedItem->m_pos.x);
		int	y		=	GetScaledYPos(lpDroppedItem->m_pos.y);

		g_anmEtc.PutReg(x,y,eEANM_FIELD_ITEM_SHINING,0,m_iFrame);

		iCount++;
	}
}

void
cFieldItem::getNotCrashRectangle(cRECT *_lpRect,int _iCount,int _iDirect)
{
	for (int i=0,iCount=0;i<dFIELD_ITEM_COUNT && iCount<_iCount;i++)
	{
		cDroppedItem	*lpItem	=	m_den.get(i);

		if (!lpItem)
			continue;

		iCount++;

		if (!g_rectViewPort.isIn(lpItem->m_pos.x,lpItem->m_pos.y))
			continue;

		if (_lpRect->isCrash(&lpItem->m_rect))
		{
			int		iX,iY;

			if (_iDirect	==	0)
			{
				iY		=	lpItem->m_rect.y1-dFIELD_ITEM_NAME_BOX_HEIGHT;
				_lpRect->set(_lpRect->x1,iY,_lpRect->x2,iY+_lpRect->getHeight()-1);
			}
			else
			if (_iDirect	==	1)
			{
				iY		=	lpItem->m_rect.y1+dFIELD_ITEM_NAME_BOX_HEIGHT;
				_lpRect->set(_lpRect->x1,iY,_lpRect->x2,iY+_lpRect->getHeight()-1);
			}
			else
			if (_iDirect	==	2)
			{
				iX	=	lpItem->m_rect.x1-_lpRect->getWidth()-2;
				_lpRect->set(iX,_lpRect->y1,iX+_lpRect->getWidth(),_lpRect->y2);
			}
			else
			if (_iDirect	==	3)
			{
				iX	=	lpItem->m_rect.x2+2;
				_lpRect->set(iX,_lpRect->y1,iX+_lpRect->getWidth(),_lpRect->y2);
			}

			getNotCrashRectangle(_lpRect,_iCount,_iDirect);
		}
	}
}


cDroppedItem*
cFieldItem::pickNearestItem(int _iX,int _iY)
{
	int				iCloseItem	=	0xffff;
	DWORD			dwMinRange	=	0xffffffff;
	cDroppedItem	*lpItem;

	for (int i=0,iCount=0;i<dFIELD_ITEM_COUNT && iCount<m_den.getCount();i++)
	{
		lpItem	=	m_den.get(i);

		if	(!lpItem)
			continue;

		iCount++;

		int	iRange	=	GetOvalRange(_iX,_iY,lpItem->m_pos.x,lpItem->m_pos.y);

		if (dwMinRange	>	(DWORD)iRange)
		{
			dwMinRange	=	iRange;
			iCloseItem	=	i;
		}
	}

	if (iCloseItem	!=	0xffff)
		return	m_den.get(iCloseItem);

	return	NULL;
}


BOOL
cFieldItem::isCrashRectangle(cRECT *_lpRect,int _iCount)
{
	for (int i=0,iCount=0;i<dFIELD_ITEM_COUNT && iCount<_iCount;i++)
	{
		cDroppedItem	*lpItem	=	m_den.get(i);

		if (!lpItem)
			continue;

		if (!g_rectViewPort.isIn(lpItem->m_pos.x,lpItem->m_pos.y))
			continue;

		if (_lpRect->isCrash(&lpItem->m_rect))
			return	TRUE;

		iCount++;
	}

	return	FALSE;
}

void
cFieldItem::getSuitableRectangle(cDroppedItem *_lpDroppedItem,int _iCount)
{
	char	strName[256];
	cRECT	rect,*lpRect;
	int		iDirect,i;
	CPos	pos;

	strcpy(strName,_lpDroppedItem->getName());

	int		iPixelSize	=	_lpDroppedItem->m_wPixelWidth;

	if	(iPixelSize	==	0)
	{
		iPixelSize	=	s_text.getPixelSize(strName)+dFIELD_ITEM_NAME_BOX_ADD_WIDTH;
		_lpDroppedItem->m_wPixelWidth	=	iPixelSize;
	}

	int		iX			=	_lpDroppedItem->m_pos.x;
	int		iY			=	_lpDroppedItem->m_pos.y;

	iY		=	iY/dFIELD_ITEM_NAME_BOX_HEIGHT*dFIELD_ITEM_NAME_BOX_HEIGHT;
	iX		-=	iPixelSize/2;
	iY		-=	dFIELD_ITEM_NAME_BOX_HEIGHT/2;

	lpRect	=	&_lpDroppedItem->m_rect;

	lpRect->set(iX,iY,iX+iPixelSize,iY+dFIELD_ITEM_NAME_BOX_HEIGHT-2);

	if	(!isCrashRectangle(lpRect,_iCount))
		return;
	
	lpRect->getCenter(&pos);

	m_iFindRectangleCount	=	0;

	for (iDirect=0;iDirect<4;iDirect++)
	{
		rect.set(lpRect);

		getNotCrashRectangle(&rect,_iCount,iDirect);

		l_aRect[m_iFindRectangleCount++].set(&rect);
	}

	for (iY=1;iY<40;iY++)
	{
		rect.set(lpRect);

		rect.y1	=	rect.y1-dFIELD_ITEM_NAME_BOX_HEIGHT*iY;
		rect.y2	=	rect.y2-dFIELD_ITEM_NAME_BOX_HEIGHT*iY;

		if	(!isCrashRectangle(&rect,_iCount))
			break;

		for (iDirect=2;iDirect<4;iDirect++)
		{
			rect.set(lpRect);

			rect.y1	=	rect.y1-dFIELD_ITEM_NAME_BOX_HEIGHT*iY;
			rect.y2	=	rect.y2-dFIELD_ITEM_NAME_BOX_HEIGHT*iY;

			getNotCrashRectangle(&rect,_iCount,iDirect);
			l_aRect[m_iFindRectangleCount++].set(&rect);
		}
	}

	for (iY=1;iY<40;iY++)
	{
		rect.set(lpRect);

		rect.y1	=	rect.y1+dFIELD_ITEM_NAME_BOX_HEIGHT*iY;
		rect.y2	=	rect.y2+dFIELD_ITEM_NAME_BOX_HEIGHT*iY;

		if (!isCrashRectangle(&rect,_iCount))
			break;

		for (iDirect=2;iDirect<4;iDirect++)
		{
			rect.set(lpRect);

			rect.y1	=	rect.y1+dFIELD_ITEM_NAME_BOX_HEIGHT*iY;
			rect.y2	=	rect.y2+dFIELD_ITEM_NAME_BOX_HEIGHT*iY;

			getNotCrashRectangle(&rect,_iCount,iDirect);
			l_aRect[m_iFindRectangleCount++].set(&rect);
		}
	}

	int	iRect=0,iMinRange=0x7fffffff;

	for (i=0;i<m_iFindRectangleCount;i++)
	{
		CPos	posCenter;

		l_aRect[i].getCenter(&posCenter);

		int		iRange	=	GetOvalRange(pos.x,pos.y,posCenter.x,posCenter.y);

		if (iRange	<	iMinRange)
		{
			iMinRange	=	iRange;
			iRect		=	i;
		}
	}

	lpRect->set(&l_aRect[iRect]);
}


void
cFieldItem::updateNameTagBox()
{
	l_iNameTagBoxCount	=	0;

	CActor	*alpActorList[1024];

	int	iActorCount	=	g_am.getScreenActorList(alpActorList,1024),i,iCount=0,iIndex=0;

	for (i=0;i<iActorCount;i++)
	{
		if	(alpActorList[i]->m_strName[0]	==	NULL)
		{
			g_am.addToAskInfoActorList(alpActorList[i]->m_wSerial);
			continue;
		}

		cNameBox	*lpNameTagBox	=	&l_aNameTagBox[l_iNameTagBoxCount++];

		strcpy(lpNameTagBox->m_strText,alpActorList[i]->m_strName);
		lpNameTagBox->m_iSerial	=	alpActorList[i]->m_wSerial;
		lpNameTagBox->m_iType	=	0;
	}

	for (i=0;i<dFIELD_ITEM_COUNT && iCount < m_den.getCount();i++)
	{
		cDroppedItem	*lpDroppedItem	=	m_den.get(i);

		if	(lpDroppedItem	==	NULL)
			continue;

		iCount++;

		if	(!g_rectViewPort.isIn(lpDroppedItem->m_pos.x,lpDroppedItem->m_pos.y))
			continue;

		cNameBox	*lpNameTagBox	=	&l_aNameTagBox[l_iNameTagBoxCount++];

		strcpy(lpNameTagBox->m_strText,lpDroppedItem->getName());
		lpNameTagBox->m_iSerial	=	i;
		lpNameTagBox->m_iType	=	1;
	}
}

//
//	필드 드랍된 아이템 업데이트
void
cFieldItem::update()
{
//	샤이닝 효과를 돌리기까지 남은 시간
	if (m_iRemainTimeForShinigItem	<=	0)
	{
		m_iFrameCounter				=	0;
		m_iFrame					=	0;
		m_iRemainTimeForShinigItem	=	dFIELD_ITEM_SHINING_PERIOD;
	}

//	샤이닝효과 프레임 업데이트
	if (m_iFrameCounter	!=	0xffff)
	{
		m_iFrameCounter	+=	g_anmEtc.GetFPS(eEANM_FIELD_ITEM_SHINING);

		if (m_iFrameCounter	>=	dSYNC_FPS)
		{
			m_iFrameCounter	-=	dSYNC_FPS;
			m_iFrame++;

			if (m_iFrame	>=	g_anmEtc.GetFrameCount(eEANM_FIELD_ITEM_SHINING))
				m_iFrameCounter	=	0xffff;
		}
	}

	m_iRemainTimeForShinigItem--;

	if	(m_bIsShowDroppedItemName+ g_config.m_bf1IsShowDropItem)
	{
		int	i,iCount=0,iIndex=0;

		for (i=0;i<dFIELD_ITEM_COUNT && iCount < m_den.getCount();i++)
		{
			cDroppedItem	*lpDroppedItem	=	m_den.get(i);

			if (lpDroppedItem	==	NULL)
				continue;

			iCount++;

			if (!g_rectViewPort.isIn(lpDroppedItem->m_pos.x,lpDroppedItem->m_pos.y))
				continue;

			getSuitableRectangle(lpDroppedItem,iIndex);

			iIndex++;
		}
	}
}


//
//	업데이트
BOOL
cFieldItem::findFocusItem(int _iX,int _iY,BOOL _bIsFindFocus)
{
	int	iCount	=	0,i;

	if (m_iPickItem	!=	0xffff)
	{
		cDroppedItem	*lpDroppedItem	=	m_den.get(m_iPickItem);

		if (lpDroppedItem	==	NULL)	//	잡기로 예약한 아이템이 사라졌다. -o-;;;
		{
			s_iFocusFieldItem	=	0xffff;
			releasePickItem();
			cINPDEV::ReleaseExclusive();
		}
		else
			return	TRUE;
	}

	if (cINPDEV::IsExclusive())
		return	FALSE;

	m_iFocusItem		=	0xffff;
	s_iFocusFieldItem	=	0xffff;

	if (s_bIsIgnoreAllFocus)
		return	FALSE;

	if (s_iFocusActor	!=	0xffff)	
	{
		if	(!g_aActor[s_iFocusActor].isNpc() &&!g_aActor[s_iFocusActor].isDeath() &&  !g_aActor[s_iFocusActor].isPlayer() && !g_aActor[s_iFocusActor].isPet() && !g_aActor[s_iFocusActor].isSummonBeast())
			return	FALSE;
	}	
	else	
		if (!IsValidFocus())
			return	FALSE;

	if (_bIsFindFocus)
		for (i=0;i<dFIELD_ITEM_COUNT && iCount < m_den.getCount();i++)
		{
			cDroppedItem	*lpDroppedItem	=	m_den.get(i);

			if (lpDroppedItem	==	NULL)
				continue;

			if (!g_rectViewPort.isIn(lpDroppedItem->m_pos.x,lpDroppedItem->m_pos.y))
				continue;

			BOOL	bIsFocused	=	FALSE;

			if (m_bIsShowDroppedItemName + g_config.m_bf1IsShowDropItem)
			{
				if (lpDroppedItem->m_rect.isIn(_iX,_iY))
					bIsFocused	=	TRUE;
			}
			else
			if (lpDroppedItem->isFocus(_iX,_iY))
			{
				bIsFocused	=	TRUE;
			}

			if (bIsFocused)
			{
				m_iFocusItem		=	i;
				s_iFocusFieldItem	=	m_iFocusItem;

				s_iFocusActor	=	0xffff;

				return	TRUE;
			}

			iCount++;
		}

	return	FALSE;
}