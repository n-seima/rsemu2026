#include "CWindowInterface.H"
#include "CGamePlayInterface.H"
#include "CActor.H"
#include "CShoppingCart.H"
#include "CMessage.H"
#include "CLoadTextCode.H"

#ifdef _DEBUG

BOOL			g_bIsHideDamageAndHideName	=	FALSE;
BOOL			g_bIsMovieMode				=	FALSE;
CDebugWindow	g_gwDebug;
#define			dDEBUG_WINDOW_BOTTOM_HEIGHT	56
//
//	Debug Window Menu Step
enum
{
	eDWMS_MAIN,
	eDWMS_SELECT_FIELD,
	eDWMS_SELECT_COSTUME,

	eDWMS_MOVE_SELECTED_FIELD,
	eDWMS_CHANGE_SELECTED_COSTUME,
	eDWMS_CANCEL,

};

void
CDebugWindow::cContentsInfo::copy(cContentsInfo *_lpInfo)
{
	memcpy(this,_lpInfo,sizeof(cContentsInfo));
}

void
CDebugWindow::cContentsInfo::reset()
{
	m_wSerial	=	0xffff;
}

void
CDebugWindow::open()
{
	if (s_iPopupInterface	==	ePIW_DEBUG)
	{
		ClosePopupWindow();

		return;
	}

	m_den.init(dMAX_DEBUG_CONTENTS_LIST_COUNT);

	m_iMenuStep				=	eDWMS_MAIN;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.
	m_iDisplayContentsCount	=	30;
	m_iListScrollBarPos		=	0;

	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_TRADE);

	s_iPopupInterface		=	ePIW_DEBUG;
	m_rect.x1				=	g_iScreenWidth-iWindowWidth;
	m_rect.y1				=	iTitleHeight;
	m_rect.x2				=	g_iScreenWidth;
	m_rect.y2				=	iTitleHeight+iWindowHeight;
	s_rectInterfaceSize.set(g_iScreenWidth-iWindowWidth, iTitleHeight, g_iScreenWidth, iTitleHeight+iWindowHeight);

	int	iX	=	m_rect.x1+16;
	int	iY	=	m_rect.y1+10,i;

	m_bmMain.init();

	for (i=0;;i++)
	{
		if (g_aOperatorCommandInfo[i].m_wCommand	==	0xffff)
			break;

		m_bmMain.addTextBar(g_aOperatorCommandInfo[i].m_strComment,g_aOperatorCommandInfo[i].m_wCommand,iX,iY,iWindowWidth-30,16);

		iY	+=	18;
	}

	s_sbPopupWindow.set(m_rect.x1+iWindowWidth-16,m_rect.y1+18,iWindowHeight-dDEBUG_WINDOW_BOTTOM_HEIGHT);
	s_sbPopupWindow.setPos(0);

	m_iFieldCount	=	0;

	for (i=0;i<dMAX_MAP_COUNT;i++)
	{
		if (g_aMapSimpleInfo[i].m_wSerial	!=	0xffff)
			m_iFieldCount++;
	}
}

void
CDebugWindow::close()
{
}

void
CDebugWindow::changeTab(int _iTab)
{
	m_iMenuStep	=	_iTab;

	if (m_iMenuStep	==	eDWMS_SELECT_FIELD)
	{
		m_bmSub.init(&g_sprInterface);

		m_bmSub.addImageBar(dMSG_SELECT,eDWMS_MOVE_SELECTED_FIELD	,m_rect.x1+130,470,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
		m_bmSub.addImageBar(dMSG_CANCEL,eDWMS_CANCEL			,m_rect.x1+200,470,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);

		m_bmSub.setClickMargin(eDWMS_MOVE_SELECTED_FIELD,1,1);
		m_bmSub.setClickMargin(eDWMS_CANCEL,1,1);
		m_bmSub.setAllBarTextColor(0,FALSE);

		m_den.reset();

		for (int i=0;i<dMAX_MAP_COUNT;i++)
		{
			if (g_aMapSimpleInfo[i].m_wSerial	==	0xffff)
				continue;

			DWORD	dwColor	=	LTGRAY;

			if (g_aMapSimpleInfo[i].m_bf1IsPremiumZone)
				dwColor	=	GREEN;

			cContentsInfo	contents;

			contents.m_wValue	=	i;
			contents.m_dwColor	=	dwColor;
			sprintf(contents.m_strText,"[%.4d]%s",i,g_aMapSimpleInfo[i].m_strName);

			m_den.add(&contents);
		}
	}

	if (m_iMenuStep	==	eDWMS_SELECT_COSTUME)
	{
		m_bmSub.init(&g_sprInterface);

		m_bmSub.addImageBar(dMSG_SELECT,eDWMS_CHANGE_SELECTED_COSTUME,m_rect.x1+130,470,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
		m_bmSub.addImageBar(dMSG_CANCEL,eDWMS_CANCEL				,m_rect.x1+200,470,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);

		m_bmSub.setClickMargin(eDWMS_CHANGE_SELECTED_COSTUME,1,1);
		m_bmSub.setClickMargin(eDWMS_CANCEL,1,1);
		m_bmSub.setAllBarTextColor(0,FALSE);

		m_den.reset();

		for (int i=0;i<g_loaderText.m_iEventMobCount;i++)
		{
			cContentsInfo	contents;

			contents.m_dwColor	=	LTGRAY;
			contents.m_wValue	=	g_aEventMobList[i].m_wBody;

			sprintf(contents.m_strText,"%s",g_aEventMobList[i].m_strMobName);
			
			m_den.add(&contents);
		}
	}

	s_sbPopupWindow.setSize(m_den.getCount(),m_iDisplayContentsCount);
	s_sbPopupWindow.setPos(m_iListScrollBarPos);
	m_iSelectContents	=	-1;
}

void
CDebugWindow::updateMainTab()
{
	DWORD	dwMenu		=	m_bmMain.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	COperatorCommandInfo	*lpCommand;
	CG_OPERATOR_COMMAND		packet;

	if	(dwMenu	!=	0xffffffff)
		lpCommand	=	&g_aOperatorCommandInfo[dwMenu];

	switch(dwMenu)
	{
		case	eOCM_RECALL			://플래이어 소환"
		case	eOCM_WARP			://플래이어에게로 이동"	
		case	eOCM_DRAWING_ROOM	://취조실로 보내기"		
		case	eOCM_JAIL			://감옥으로 보내기"		
		case	eOCM_BANISH			://쫒아내기"			
		case	eOCM_TRACE_ACTOR	:
		case	eOCM_ASK_ITEM_DATA	:
			if	(!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			s_inputChatting.setStr(0,_ms("\\%s %s",lpCommand->m_strCommand,dMSG_PLEASE_INPUT_TARGET_PLAYER_NAME));
			s_inputChatting.setSelect(0,strlen(lpCommand->m_strCommand)+2,-1);
			break;

		case	eOCM_MOVE_FIELD		://필드 이동"
			changeTab(eDWMS_SELECT_FIELD);
			break;

		case	eOCM_COSTUME		://코스튬
			changeTab(eDWMS_SELECT_COSTUME);
			break;

		case	eOCM_REGEN_FIELD_MOB	:
		{
			packet.base.set(sizeof(packet),dCG_OPERATOR_COMMAND);
			packet.wCommand	=	eOC_REGEN_FIELD_MOB;
			s_agent.sendOperatorCommand(&packet);
			break;
		}

		case	eOCM_HIDE_NAME_AND_HIDE_DAMAGE	:
		{
#ifdef	_DEBUG
			g_bIsHideDamageAndHideName	=	1-g_bIsHideDamageAndHideName;
#endif
			break;
		}

		case	eOCM_HIDING				:
		{
			packet.base.set(sizeof(packet),dCG_OPERATOR_COMMAND);
			packet.wCommand	=	eOC_HIDE;
			s_agent.sendOperatorCommand(&packet);
			break;
		}

		case	eOCM_SPRINKLE_GIFT		:
		{
			packet.base.set(sizeof(packet),dCG_OPERATOR_COMMAND);
			packet.wCommand	=	eOC_SPRINKLE_GIFT;
			s_agent.sendOperatorCommand(&packet);
			break;
		}

		case	eOCM_CONTROL_MONSTER://NPC 지배"			
		{
			packet.wCommand	=	eOC_CONTROL_MONSTER;
			packet.wX		=	s_iFocusActor;
			s_agent.sendOperatorCommand(&packet);
			break;
		}

		case	eOCM_CHANGE_LEVEL	://레벨변경"
			if (!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			s_inputChatting.setStr(0,_ms("%s%s",lpCommand->m_strCommand,dMSG_PLEASE_INPUT_LEVEL));
			s_inputChatting.setSelect(0,strlen(lpCommand->m_strCommand)+1,-1);
			break;
		case	eOCM_CHANGE_OWN_GOLD://소지금 변경"
			if (!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			s_inputChatting.setStr(0,_ms("%s%s",lpCommand->m_strCommand,dMSG_PLEASE_INPUT_GOLD));
			s_inputChatting.setSelect(0,strlen(lpCommand->m_strCommand)+1,-1);
			break;

		case	eOCM_CHANGE_LOCATE	://좌표 이동"
			if (!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			s_inputChatting.setStr(0,_ms("\\%s %s",lpCommand->m_strCommand,dMSG_PLEASE_INPUT_MOVE_LOCATE));
			s_inputChatting.setSelect(0,strlen(lpCommand->m_strCommand)+2,-1);
			break;

		case	eOCM_RECOVE_HP_CP	://체력 CP 모두 회복"
			s_agent.sendChatMessage(lpCommand->m_strCommand,dCHAT_CHAT);
			break;

		case	eOCM_SHOW_TILE_INFO	://타일정보 보기"		
			s_isPaintBlockedTile	=	1-s_isPaintBlockedTile;
			break;
		case	eOCM_MOVIE_MODE	:
		{
#ifdef	_DEBUG
			g_bIsMovieMode	=	1-g_bIsMovieMode;
#endif
			break;
		}
		case	eOCM_SUMMON_EVENT_MONSTER	:
		{
#ifdef	_DEBUG
			operateSummonEventMonster();
#endif
			break;
		}
	}
}

int
CDebugWindow::getFocusedContents()
{
	int	iX				=	m_rect.x1+12;
	int	iY				=	m_rect.y1+26;
	int	iDisplayCount	=	0;

	for (int i=0;i<m_den.getCount() && iDisplayCount < m_iDisplayContentsCount+m_iListScrollBarPos;i++)
	{
		if (iDisplayCount	<	m_iListScrollBarPos)
		{
			iDisplayCount++;
			continue;
		}

		cRECT	rect;

		rect.set(iX,iY-2,g_iScreenWidth,iY+12);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
			return	m_den.get(i)->m_wSerial;

		iDisplayCount++;

		iY	+=	14;
	}

	return	0xffff;
}

void
CDebugWindow::updateFieldListTab()
{
	if (!cINPDEV::IsExclusive(eMOUSE_STATE_SCROLL_BAR))
	{
		if (s_sbPopupWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
				cINPDEV::SetExclusive(eMOUSE_STATE_SCROLL_BAR);
		else	cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SCROLL_BAR);

		m_iListScrollBarPos	=	s_sbPopupWindow.m_iCurrentPos;
	}

	if (cINPDEV::IsExclusive(eMOUSE_STATE_PITCHMAN_SHOP_BUTTON))
		return;

	DWORD	dwMenu		=	m_bmSub.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
		case	eDWMS_MOVE_SELECTED_FIELD		:
		{
			if (m_iSelectContents == 0xffff)
				break;

			CG_OPERATOR_COMMAND	packet;

			packet.base.set(sizeof(packet),dCG_OPERATOR_COMMAND);

			cContentsInfo	*lpContents	=	m_den.get(m_iSelectContents);

			if (!lpContents)
				break;

			packet.wCommand	=	eOC_MOVE_FIELD;
			packet.wX		=	lpContents->m_wValue;

			s_agent.sendOperatorCommand(&packet);

			break;
		}

		case	eDWMS_CHANGE_SELECTED_COSTUME	:
		{
			if (m_iSelectContents == 0xffff)
				break;

			CG_OPERATOR_COMMAND	packet;

			packet.base.set(sizeof(packet),dCG_OPERATOR_COMMAND);

			packet.wCommand	=	eOC_TRANS_TO_EVENT_MOB;

			cContentsInfo	*lpContents	=	m_den.get(m_iSelectContents);

			if (!lpContents)
				break;

			packet.wX		=	m_iSelectContents;
			packet.wY		=	lpContents->m_wValue;

			s_agent.sendOperatorCommand(&packet);

			break;
		}

		case	eDWMS_CANCEL				:
			changeTab(eDWMS_MAIN);
			return;
	}

	if (m_bmMain.isPressed())
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_PITCHMAN_SHOP_BUTTON);
		return;
	}

	if (cINPDEV::IsExclusive())
	{
		m_iFocusedLine	=	0xffff;

		return;
	}

	m_iFocusedLine	=	getFocusedContents();

	if (s_isTouchLeftButton && m_iFocusedLine	!=	0xffff)
		m_iSelectContents	=	m_iFocusedLine;
}

//
//	스테이터스 윈도우 업데이트
void
CDebugWindow::update()
{
	if (s_iPopupInterface!=	ePIW_DEBUG)
		return;

	switch(m_iMenuStep)
	{
		case	eDWMS_MAIN			:
			updateMainTab();
			break;
		case	eDWMS_SELECT_FIELD	:
		case	eDWMS_SELECT_COSTUME:
			updateFieldListTab();
			break;
	}
}

void
CDebugWindow::drawListTab()
{
	int	iX				=	m_rect.x1+12;
	int	iY				=	m_rect.y1+8;
	int	iDisplayCount	=	0;

	s_text.put(iX,iY,WHITE,dMSG_PLEASE_SELECT_MOVE_FIELD);

	iY	=	m_rect.y1+26;

	for (int i=0;i<m_den.getCount() && iDisplayCount < m_iDisplayContentsCount+m_iListScrollBarPos;i++)
	{
		if (iDisplayCount	<	m_iListScrollBarPos)
		{
			iDisplayCount++;
			continue;
		}

		cContentsInfo	*lpInfo	=	m_den.get(i);

		iDisplayCount++;

		DWORD	dwColor	=	lpInfo->m_dwColor;

		if (i == m_iSelectContents)
		{
			dwColor	=	LTYELLOW;
			cDRAW::Fill(_LTBLUE,iX-2,iY-2,m_rect.x2 - 20,iY+12);
		}
		else
		if (i == m_iFocusedLine)
			dwColor	=	LTGREEN;

		s_sText.put(iX,iY,dwColor,"%s",lpInfo->m_strText);

		iY	+=	14;
	}

	g_sprInterface.Put(m_rect.x1+7,461		,eWINDOW_SEPERATE_BAR);	//	경계

	s_sbPopupWindow.draw();
	m_bmSub.draw();
}

void
CDebugWindow::drawMainTab()
{
	int	iX	=	m_rect.x1+16;
	int	iY	=	m_rect.y1+10;

	m_bmMain.draw();
}

//	스테이터스 윈도우 그려주기
void
CDebugWindow::draw()
{
	if (s_iPopupInterface!=	ePIW_DEBUG)
		return;

	int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);

	int		iX,iY;

	iX		=	m_rect.x1;
	iY		=	m_rect.y1;

	if (g_config.m_bIsInfoWindowHalfblending)	cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else										cDRAW::Fill	 (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface.Put(iX,iY,eWINDOW);
	g_sprInterface.Put(iX,0 ,eWINDOW_TITLE_SYSTEM);

	switch(m_iMenuStep)
	{
		case	eDWMS_MAIN			:
			drawMainTab();
			break;

		case	eDWMS_SELECT_FIELD		:
		case	eDWMS_SELECT_COSTUME	:
			drawListTab();
			break;
	}
}

BOOL
CDebugWindow::isOpened()
{
	if (s_iPopupInterface!=	ePIW_DEBUG)
		return	FALSE;

	return	TRUE;
}

#endif