#include "putBanner.h"
#include "CHero.h"
#include "CMessage.h"
#include "cActor.h"
#include "banner.h"

cPutBannerWnd	g_gwPutBannerWnd;

enum
{
	ePBWBM_EDIT_TITLE,
	ePBWBM_EDIT_COMMENT,
	ePBWBM_CANCEL,
	ePBWBM_PUT_BANNER,
};

cPutBannerWnd::cPutBannerWnd()
{
}

void
cPutBannerWnd::open(int _iBannerItem)//	¿
{
	m_iBannerItem			=	_iBannerItem;

	m_iStatus				=	ePUS_INPUT_TITLE;
	m_strBannerText[0]		=	NULL;
	m_strBannerComment[0]	=	NULL;

	ClosePopupWindow();

	if	(s_iPopupInterface	==	ePIW_PUT_BANNER)
		return;

	s_iPopupInterface		=	ePIW_PUT_BANNER;

	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iTitleBarHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_FRIEND);
	int	iBarHeight			=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);
	int	iBarWidth			=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);

	s_rectInterfaceSize.x1	=	g_iScreenWidth-iWindowWidth;
	s_rectInterfaceSize.x2	=	g_iScreenWidth;

	m_pos.x					=	g_iScreenWidth-iWindowWidth;
	m_pos.y					=	iTitleBarHeight;

	int	iX					=	m_pos.x;
	int	iY					=	0;

	s_bmButton.reset();

	s_bmButton.addImageBar(dMSG_EDIT,ePBWBM_EDIT_TITLE,iX+186,iY+100,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
	s_bmButton.addImageBar(dMSG_EDIT,ePBWBM_EDIT_COMMENT,iX+186,iY+190,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);

	s_bmButton.addImageBar(dMSG_PUT_BANNER,ePBWBM_PUT_BANNER,iX+130,iY+470,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
	s_bmButton.addImageBar(dMSG_CANCEL,ePBWBM_CANCEL,iX+195,iY+470,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);

	s_bmButton.setClicktAllBarMargin(1,1);
	s_bmButton.setAllBarTextColor(0,FALSE);

	if	(!s_inputChatting.isEnable())
		EnableChattingBar(TRUE);

	s_inputChatting.setStr(0,dMSG_PLEASE_INPUT_BANNER_TITLE);
	s_inputChatting.setSelect(0,0,-1);
}

void
cPutBannerWnd::close()
{
	cINPDEV::ReleaseExclusive();

	s_bmButton.reset();
	s_iPopupInterface		=	ePIW_NONE;
	s_rectInterfaceSize.set(0, 0, 0, 0);
}

void
cPutBannerWnd::update()
{
	if	(s_iPopupInterface	!=	ePIW_PUT_BANNER)
		return;

	DWORD	dwSelectMenu	=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	if (s_bmButton.isPressed())
		cINPDEV::SetExclusive(eMOUSE_STATE_FRIEND_WINDOW_BUTTONS);
	else
		cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_FRIEND_WINDOW_BUTTONS);

	switch(dwSelectMenu)
	{
		case	ePBWBM_EDIT_TITLE	:
		{
			if	(!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			if	(strlen(m_strBannerText)	==	0)
			{
				s_inputChatting.setStr(0,dMSG_PLEASE_INPUT_BANNER_TITLE);
				s_inputChatting.setSelect(0,0,-1);
			}
			else
				s_inputChatting.setStr(0,m_strBannerText);

			m_iStatus				=	ePUS_INPUT_TITLE;
			break;
		}

		case	ePBWBM_EDIT_COMMENT	:
		{
			if	(!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			if	(strlen(m_strBannerComment)	==	0)
			{
				s_inputChatting.setStr(0,dMSG_PLEASE_INPUT_BANNER_COMMENT);
				s_inputChatting.setSelect(0,0,-1);
			}
			else
				s_inputChatting.setStr(0,m_strBannerComment);

			m_iStatus				=	ePUS_INPUT_COMMENT;
			break;
		}

		case	ePBWBM_CANCEL		:
		{
			close();
			break;
		}

		case	ePBWBM_PUT_BANNER	:
		{
			if	(strlen(m_strBannerText)	<=	4	||	strlen(m_strBannerComment)	<=	4)
			{
				g_msgBox.cPopup("",dMSG_PLEASE_INPUT_BANNER_TEXT,dMSG_OK);

				break;
			}

			s_agent.sendPutBanner(g_lpHero->m_pos.x,g_lpHero->m_pos.y,m_iBannerItem,m_strBannerText,m_strBannerComment);

			break;
		}
	}	
}

BOOL
cPutBannerWnd::isOpened()
{
	if (s_iPopupInterface	==	ePIW_PUT_BANNER)
		return	TRUE;

	return	FALSE;
}

void
cPutBannerWnd::draw()
{
	if	(s_iPopupInterface	!=	ePIW_PUT_BANNER)
		return;

	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iTitleBarHeight	=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_FRIEND);

	int		iX,iY;

	iX		=	m_pos.x;
	iY		=	iTitleBarHeight;

	if (g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill	 (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface.Put(iX,iY,eWINDOW);
	g_sprInterface2.Put(iX,0,dETC_WND_TITLE_BAR_SPRITE_INDEX);

	iY	=	104;

	if	(m_iStatus	!=	ePUS_INPUT_TITLE	||	timeGetTime()%1000	>=	200)
		s_text.put(iX+16,iY,WHITE,dMSG_BANNER_TITLE);

	iY	+=	20;
	cDRAW::FillHB(_DEEPGRAY,iX+32,iY,g_iScreenWidth-20,iY+18);
	s_text.put(iX+36,iY+3,LTYELLOW,m_strBannerText);

	iY	+=	70;
	if	(m_iStatus	!=	ePUS_INPUT_COMMENT	||	timeGetTime()%1000	>=	300)
		s_text.put(iX+16,iY,WHITE,dMSG_BANNER_COMMNET);

	iY	+=	20;
	cDRAW::FillHB(_DEEPGRAY,iX+32,iY,g_iScreenWidth-20,iY+20*8);
	s_text.put(iX+36,iY+3,g_iScreenWidth-20-(iX+32)-8,100,0,LTYELLOW,m_strBannerComment);

	s_bmButton.draw();
}

void
cPutBannerWnd::updateText(char *_lpstrText)
{
	if	(m_iStatus	==	ePUS_INPUT_TITLE)
	{
		cItem	*lpItem	=	g_hero.getInventoryItem(m_iBannerItem);

		if	(!lpItem)
			return;

		if	(lpItem->isVerticalBanner())
			g_bannerManager.strcpyVerticalBannerTitle(m_strBannerText,s_strChatMessage);
		else
			ustrncpy(m_strBannerText,s_strChatMessage,dBANNER_TITLE_LENGTH-2);

		if	(strlen(m_strBannerComment)	==	0)
		{
			s_inputChatting.setStr(0,dMSG_PLEASE_INPUT_BANNER_COMMENT);
			s_inputChatting.setSelect(0,0,-1);
		}
		else
			s_inputChatting.setStr(0,m_strBannerComment);

		m_iStatus				=	ePUS_INPUT_COMMENT;
	}
	else
	{
		ustrncpy(m_strBannerComment,s_strChatMessage,dBANNER_COMMENT_LENGTH-2);

		if	(strlen(m_strBannerText)	==	0)
		{
			s_inputChatting.setStr(0,dMSG_PLEASE_INPUT_BANNER_TITLE);
			s_inputChatting.setSelect(0,0,-1);
		}
		else
			s_inputChatting.setStr(0,m_strBannerText);

		m_iStatus				=	ePUS_INPUT_TITLE;
	}
}
