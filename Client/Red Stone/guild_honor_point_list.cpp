#include	"CWindowInterface.H"
#include	"cMessage.h"
#include	"cHERO.h"
#include	"cPACKET_GAMESERVER.h"

cGuildHonorPointList	g_gwHonorPointList;

BOOL
cGuildHonorPointList::isOpened()	//	열렸냐?
{
	if	(s_iPopupInterface	==	ePIW_GUILD_HONOR_POINT_LIST)
		return	TRUE;

	return	FALSE;
}

void
cGuildHonorPointList::open(void *_lpBuffer)	//	열기
{
	SG_GUILD_HONOR_POINT_LIST	*lpPacket	=	(SG_GUILD_HONOR_POINT_LIST	*)_lpBuffer;

	m_wListCount		=	lpPacket->wCount;
	m_wHallLevel		=	lpPacket->wHallLevel;
	memcpy(m_aList,lpPacket->aList,sizeof(m_aList));

	if	(s_iPopupInterface	==	ePIW_GUILD_HONOR_POINT_LIST)
	{
		ClosePopupWindow();
		return;
	}

	ClosePopupWindow();

	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_TRADE);

	s_iPopupInterface		=	ePIW_GUILD_HONOR_POINT_LIST;
	s_rectInterfaceSize.x1	=	g_iScreenWidth-iWindowWidth;
	s_rectInterfaceSize.x2	=	g_iScreenWidth;

	m_rect.x1				=	g_iScreenWidth-iWindowWidth;
	m_rect.y1				=	iTitleHeight;
	m_rect.x2				=	g_iScreenWidth;
	m_rect.y2				=	iTitleHeight+iWindowHeight;

	{
		s_bmButton.reset();

		s_bmButton.addImageBar(dMSG_CLOSE	,IDCANCEL	,m_rect.x1+196,472,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
		s_bmButton.setClickMargin(IDCANCEL,1,1);
		s_bmButton.setAllBarTextColor(0,FALSE);
	}
}

void
cGuildHonorPointList::close()	//창 닫기
{
	if	(s_iPopupInterface	!=	ePIW_GUILD_HONOR_POINT_LIST)	
		return;

	cINPDEV::ReleaseExclusive();
	m_bmButton.reset();
	s_iPopupInterface		=	ePIW_NONE;
	s_rectInterfaceSize.set(0, 0, 0, 0);
}


void
cGuildHonorPointList::draw()		//그리기
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
	g_sprInterface2.Put(iX	,0	,66);

	iX				=	m_rect.x1+12;
	iY				=	m_rect.y1+8;

	s_text.put(iX,iY,LTYELLOW,dMSG_LIST_OF_GUILD_HALL_LEVEL_FORM,m_wHallLevel);

	iY				+=	18;

	for (int i=0;i<m_wListCount;i++)
	{
		DWORD	dwColor	=	WHITE;

		s_text.put(iX,iY,dwColor,dMSG_GUILD_HONOR_POINT_FORM,i+1,m_aList[i].m_strGuildName,m_aList[i].m_iHonorPoint);

		iY	+=	14;
	}

	g_sprInterface.Put(m_rect.x1+7,466		,eWINDOW_SEPERATE_BAR);	//	경계

	s_bmButton.draw();
}

void
cGuildHonorPointList::update()	//업데이트
{
	if	(s_iPopupInterface	!=	ePIW_GUILD_HONOR_POINT_LIST)
		return;

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_PITCHMAN_SHOP_BUTTON))
		return;

	DWORD	dwMenu		=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
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
}