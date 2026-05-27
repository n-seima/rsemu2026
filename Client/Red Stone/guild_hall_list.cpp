#include "CWindowInterface.H"
#include	"cMessage.h"
#include	"cHERO.h"

cGuildHallList	g_gwGuildHallList;

cGuildHallList::cGuildHallList()
{
}
							
BOOL
cGuildHallList::isOpened()	//	열렸냐?
{
	if	(s_iPopupInterface	==	ePIW_GUILD_HALL_LIST)
		return	TRUE;

	return	FALSE;
}

void
cGuildHallList::open(char *_lpBuffer,int _iCount)	//	열기
{
	int	iBufferPoint	=	0;
	m_wHallCount		=	_iCount;
	m_wIsWaitSelectResult=	FALSE;

	if	(s_iPopupInterface	==	ePIW_GUILD_HALL_LIST)
	{
		ClosePopupWindow();
		return;
	}

	ClosePopupWindow();

	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_TRADE);

	s_iPopupInterface		=	ePIW_GUILD_HALL_LIST;
	s_rectInterfaceSize.x1	=	g_iScreenWidth-iWindowWidth;
	s_rectInterfaceSize.x2	=	g_iScreenWidth;

	m_rect.x1				=	g_iScreenWidth-iWindowWidth;
	m_rect.y1				=	iTitleHeight;
	m_rect.x2				=	g_iScreenWidth;
	m_rect.y2				=	iTitleHeight+iWindowHeight;

	for	(int i=0;i<_iCount;i++)
	{
		int	iLength		=	_lpBuffer[iBufferPoint];

		memcpy(m_aHallList[i].m_strGuildName,_lpBuffer+iBufferPoint+1,iLength);
		m_aHallList[i].m_strGuildName[iLength]	=	NULL;

		iBufferPoint	+=	(iLength+1);

		m_aHallList[i].m_bf3HallLevel			=	_lpBuffer[iBufferPoint++];
		m_aHallList[i].m_bf1IsPermitVisitor		=	_lpBuffer[iBufferPoint++];
		memcpy(&m_aHallList[i].m_wGuild,_lpBuffer+iBufferPoint,sizeof(WORD));
		
		iBufferPoint	+=	2;
	}

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
cGuildHallList::close()	//창 닫기
{
	if	(s_iPopupInterface	!=	ePIW_GUILD_HALL_LIST)	
		return;

	cINPDEV::ReleaseExclusive();
	m_bmButton.reset();
	s_iPopupInterface		=	ePIW_NONE;
	s_rectInterfaceSize.set(0, 0, 0, 0);

	s_agent.sendEtcWork(eCEW_CLOSE_GUILD_HALL_LIST);
}


void
cGuildHallList::draw()		//그리기
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

	char	*strOpenClose[]	=	{"close","open"};

	for (int i=0;i<m_wHallCount;i++)
	{
		DWORD	dwColor	=	LTGRAY;

		if	(i	==	m_iSelectHall)
		{
			dwColor	=	LTYELLOW;
			cDRAW::Fill(_LTBLUE,iX-2,iY-1,m_rect.x2 - 10,iY+11);
		}
		else
		if	(i	==	m_iFocusHall)
			dwColor	=	LTGREEN;

		s_sText.put(iX,iY,dwColor,"Hall LV %d - %s <%s> ",m_aHallList[i].m_bf3HallLevel,m_aHallList[i].m_strGuildName,strOpenClose[m_aHallList[i].m_bf1IsPermitVisitor]);

		iY	+=	11;
	}

	g_sprInterface.Put(m_rect.x1+7,466		,eWINDOW_SEPERATE_BAR);	//	경계

	s_bmButton.draw();
}

int
cGuildHallList::getFocusHallIndex()
{
	int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);

	int	iX,iY;

	iX					=	m_rect.x1+12;
	iY					=	m_rect.y1+8;

	char	*strOpenClose[]	=	{"close","open"};

	for (int i=0;i<m_wHallCount;i++)
	{
		DWORD	dwColor	=	LTGRAY;

		cRECT	rect;

		rect.set(iX-2,iY-1,m_rect.x2 - 10,iY+11);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return	i;

		iY	+=	11;
	}

	return	-1;
}

void
cGuildHallList::moveToSelectGuildHall()
{
	m_wIsWaitSelectResult	=	2*dSYNC_FPS;
	s_agent.sendEtcWork(eCEW_SELECT_GUILD_HALL,m_iAppointmentGuild);
}

void
cGuildHallList::update()	//업데이트
{
	if	(s_iPopupInterface	!=	ePIW_GUILD_HALL_LIST)
		return;

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_PITCHMAN_SHOP_BUTTON))
		return;

	if	(m_wIsWaitSelectResult)
		m_wIsWaitSelectResult--;

	DWORD	dwMenu		=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
		case	IDOK		:
		{
			if	(m_wIsWaitSelectResult	||	m_iSelectHall	==	-1)
				break;

			char	strText[512];

			sprintf(strText,dMSG_DO_YOU_WANT_MOVE_TO_SELECT_GUILD_HALL_FORM,m_aHallList[m_iSelectHall].m_strGuildName,m_aHallList[m_iSelectHall].m_bf3HallLevel);

			g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_MOVE_TO_SELECT_GUILD_HALL);

			m_iAppointmentGuild	=	m_aHallList[m_iSelectHall].m_wGuild;
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
		m_iFocusHall	=	0xffff;

		return;
	}

	m_iFocusHall	=	getFocusHallIndex();

	if	(s_isTouchLeftButton && m_iFocusHall	!=	0xffff)
		m_iSelectHall	=	m_iFocusHall;
}