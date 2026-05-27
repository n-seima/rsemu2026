#include	"CWindowInterface.h"
#include	"cMessage.h"
#include	"cHERO.h"
#include	"cMap.h"

cGuildRelatedPlaceList	g_gwGuildRelatedPlaceList;

cGuildRelatedPlaceList::cGuildRelatedPlaceList()
{
}

BOOL
cGuildRelatedPlaceList::isOpened()	//	열렸냐?
{
	if	(s_iPopupInterface	==	ePIW_GUILD_RELATED_PLACE_LIST)
		return	TRUE;

	return	FALSE;
}

void
cGuildRelatedPlaceList::open(WORD *_lpPlaceList,int _iCount)	//	열기
{
	int	iBufferPoint	=	0;
	m_wIsWaitSelectResult=	FALSE;

	if	(s_iPopupInterface	==	ePIW_GUILD_RELATED_PLACE_LIST)
	{
		ClosePopupWindow();
		return;
	}

	ClosePopupWindow();

	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_TRADE);

	s_iPopupInterface		=	ePIW_GUILD_RELATED_PLACE_LIST;
	s_rectInterfaceSize.x1	=	g_iScreenWidth-iWindowWidth;
	s_rectInterfaceSize.x2	=	g_iScreenWidth;

	m_rect.x1				=	g_iScreenWidth-iWindowWidth;
	m_rect.y1				=	iTitleHeight;
	m_rect.x2				=	g_iScreenWidth;
	m_rect.y2				=	iTitleHeight+iWindowHeight;

	memcpy(m_awPlaceList,_lpPlaceList,sizeof(WORD)*_iCount);
	m_wPlaceCount			=	_iCount;

	{
		s_bmButton.reset();

		s_bmButton.addImageBar(dMSG_SELECT	,IDOK		,m_rect.x1+130,472,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
		s_bmButton.addImageBar(dMSG_CLOSE	,IDCANCEL	,m_rect.x1+196,472,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
		s_bmButton.setClickMargin(IDOK,1,1);
		s_bmButton.setClickMargin(IDCANCEL,1,1);
		s_bmButton.setAllBarTextColor(0,FALSE);
	}
}

void
cGuildRelatedPlaceList::close()	//창 닫기
{
	if	(s_iPopupInterface	!=	ePIW_GUILD_RELATED_PLACE_LIST)	
		return;

	cINPDEV::ReleaseExclusive();
	m_bmButton.reset();
	s_iPopupInterface		=	ePIW_NONE;
	s_rectInterfaceSize.set(0, 0, 0, 0);

	s_agent.sendEtcWork(eCEW_CLOSE_GUILD_HALL_LIST);
}


void
cGuildRelatedPlaceList::draw()		//그리기
{
	int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);

	int		iX,iY;

	iX		=	m_rect.x1;
	iY		=	m_rect.y1;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill	 (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface.Put(iX,iY,eWINDOW);
	g_sprInterface2.Put(iX	,g_iCorrectBI_Y/2	,66);

	iX				=	m_rect.x1+14;
	iY				=	m_rect.y1+16;

	for (int i=0;i<m_wPlaceCount;i++)
	{
		DWORD	dwColor	=	LTGRAY;

		if	(i	==	m_iSelectPlace)
		{
			dwColor	=	LTYELLOW;
			cDRAW::Fill(_LTBLUE,iX-2,iY-1,m_rect.x2 - 10,iY+14);
		}
		else
		if	(i	==	m_iFocusPlace)
			dwColor	=	LTGREEN;

		if	(m_awPlaceList[i]	==	0xffff)
			continue;

		char	*lpstrFieldName	=	cMAP::GetFieldName(m_awPlaceList[i]);

		s_text13.put(iX,iY,dwColor,"%s",lpstrFieldName);

		iY	+=	18;
	}

	g_sprInterface.Put(m_rect.x1+7,466		,eWINDOW_SEPERATE_BAR);	//	경계

	s_bmButton.draw();
}

int
cGuildRelatedPlaceList::getFocusPlace()
{
	int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);

	int	iX,iY;

	iX					=	m_rect.x1+14;
	iY					=	m_rect.y1+16;

	for (int i=0;i<m_wPlaceCount;i++)
	{
		cRECT	rect;

		rect.set(iX-2,iY-1,m_rect.x2 - 10,iY+11);

		if	(m_awPlaceList[i]	==	0xffff)
			continue;

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return	i;

		iY	+=	18;
	}

	return	-1;
}

void
cGuildRelatedPlaceList::moveToSelectPlace()
{
	m_wIsWaitSelectResult	=	TRUE;
	s_agent.sendEtcWork(eCEW_SELECT_RELATED_PLACE,m_iAppointmentPlace);
}

void
cGuildRelatedPlaceList::update()	//업데이트
{
	if	(s_iPopupInterface	!=	ePIW_GUILD_RELATED_PLACE_LIST)
		return;

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_PITCHMAN_SHOP_BUTTON))
		return;

	DWORD	dwMenu		=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
		case	IDOK		:
		{
			if	(m_wIsWaitSelectResult	||	m_iSelectPlace	==	-1)
				break;

			char	strText[512];
			char	*lpstrFieldName	=	cMAP::GetFieldName(m_awPlaceList[m_iSelectPlace]);

			sprintf(strText,dMSG_DO_YOU_WANT_MOVE_TO_SELECT_PLACE_FORM,lpstrFieldName);

			g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_MOVE_TO_SELECT_RELATED_PLACE);

			m_iAppointmentPlace	=	m_iSelectPlace;
			break;
		}

		case	IDCANCEL	:
			close();
			return;
	}

	if	(s_bmButton.isPressed())
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_PITCHMAN_SHOP_BUTTON);
		return;
	}
	else
	if	(GetExclusive()	==	eMOUSE_STATE_PITCHMAN_SHOP_BUTTON)
		ReleaseExclusive();

	if	(cINPDEV::IsExclusive())
	{
		m_iFocusPlace	=	0xffff;

		return;
	}

	m_iFocusPlace	=	getFocusPlace();

	if	(s_isTouchLeftButton && m_iFocusPlace	!=	0xffff)
		m_iSelectPlace	=	m_iFocusPlace;
}