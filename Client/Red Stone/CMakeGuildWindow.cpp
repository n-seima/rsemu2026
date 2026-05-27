#include "CWindowInterface.H"
#include "CHero.H"
#include "CMessage.H"
#include "cLoadTextCode.H"

CMakeGuildWindow	g_gwMakeGuild;

//
//	열기
void
CMakeGuildWindow::open(char *_lpstrMemberList)
{
/*
	if (!g_bIsTestServer)
	{
		g_msgBox.cPopup("","2차 길드 테스트가 종료 되었습니다.\n\n다음에 업데이트 될 테스트는 길드 스킬입니다.",dMSG_OK);

		return;
	}
*/
	ClosePopupWindow();

	if	(s_iPopupInterface	==	ePIW_READY_TO_MAKE_GUILD)
		return;

	s_iPopupInterface		=	ePIW_READY_TO_MAKE_GUILD;

	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iTitleBarHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_FRIEND);
	int	iBarHeight			=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);
	int	iBarWidth			=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);

	s_rectInterfaceSize.x1	=	g_iScreenWidth-iWindowWidth;
	s_rectInterfaceSize.x2	=	g_iScreenWidth;

	m_pos.x					=	g_iScreenWidth-iWindowWidth;
	m_pos.y					=	iTitleBarHeight+g_iCorrectBI_Y/2;

	int	iX					=	m_pos.x;
	int	iY					=	m_pos.y+80;

	memset(m_strGuildName,0,sizeof(m_strGuildName));

	for (int i=0;i<dPARTY_MAXPLAYER;i++)
		memcpy(&m_strMemberList[i],_lpstrMemberList+dNAME_LENGTH*i,dNAME_LENGTH);

	m_iFocusMember			=	0xffff;
	m_iSelectSubMaster		=	1;

	m_inputGuildName.init(s_hWnd,s_hInst,&s_text,1);
	m_inputGuildName.set(iX+32+6,iY+4,0,iX+32+6,iY+4,200,13,24-2,"");
	m_inputGuildName.enable(TRUE);
	m_inputGuildName.active(0);

	m_inputGuildName.setColor(0,WHITE);//,TRUE);

	m_iSelectSubMaster		=	1;
	m_iFocusMember			=	0;

	s_bmButton.reset();

	iX						=	m_pos.x+(iWindowWidth-6)/2-iBarWidth/2;
	iY						=	m_pos.y+iWindowHeight-9-iBarHeight;

	s_bmButton.addImageBar(dMSG_DECISION,1,iX,iY-5,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);

	s_bmButton.setClicktAllBarMargin(1,1);
	s_bmButton.setAllBarTextColor(0,FALSE);
}

//
//창 닫기
void
CMakeGuildWindow::close()
{
	cINPDEV::ReleaseExclusive();
	s_bmButton.reset();
	m_inputGuildName.close();
	s_iPopupInterface		=	ePIW_NONE;
	s_rectInterfaceSize.set(0, 0, 0, 0);
}

//
//아이템 인터페이스 그리기
void
CMakeGuildWindow::draw()
{
	int		i;

	if (s_iPopupInterface	!=	ePIW_READY_TO_MAKE_GUILD)
		return;

	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);

	int		iX,iY;

	iX		=	m_pos.x;
	iY		=	m_pos.y;

	if (g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill	 (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface.Put(iX,iY,eWINDOW);
	g_sprInterface.Put(iX,g_iCorrectBI_Y/2,eWINDOW_TITLE_GUILD);

	iY	+=	60;
	s_text.put(iX+16,iY,WHITE,dMSG_PLEASE_INPUT_GUILD_NAME);

	iY	+=	20;
	cDRAW::Fill(_WHITE,iX+32,iY,g_iScreenWidth-20,iY+18);

	iY	+=	80;
	s_text.put(iX+16,iY,WHITE,dMSG_PLEASE_SELECT_SUB_GUILD_MASTER);

	iY	+=	20;
	cDRAW::FillHB(_GRAY,iX+32,iY,g_iScreenWidth-20,iY+20*8);

	m_inputGuildName.draw(CTBLUE);

	iY	+=	4;

	for (i=0;i<dPARTY_MAXPLAYER-1;i++)
	{
		if (m_strMemberList[i][0]	==	NULL)	break;
		int	iMember	=	i+1;

		if (m_iSelectSubMaster	==	iMember)
			s_text.put(iX+38,iY+i*16,LTYELLOW,m_strMemberList[iMember]);
		else
		{
			if (m_iFocusMember	==	iMember)
					s_text.put(iX+38,iY+i*16,LTGREEN,m_strMemberList[iMember]);
			else	s_text.put(iX+38,iY+i*16,LTGRAY,m_strMemberList[iMember]);
		}
	}

	iY	+=	(20*8+4);

	s_text.put(iX+32,iY,g_iScreenWidth-(iX+32+20),100,0,WHITE,dMSG_CREATE_GUILD_COMMENT);

	s_bmButton.draw();
}

//
//	업데이트
void
CMakeGuildWindow::update()
{
	if (s_iPopupInterface	!=	ePIW_READY_TO_MAKE_GUILD)
		return;

	m_inputGuildName.update(s_posMouse.x,s_posMouse.y,s_isClickedLeftButton,s_aTouched[KEY_TAB]);

	m_iFocusMember			=	getFocusedMember();

	if (s_isTouchLeftButton)
		if (m_iFocusMember	!=	0xffff)
			m_iSelectSubMaster	=	m_iFocusMember;

	DWORD		dwSelectMenu	=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	if (s_bmButton.isPressed())
		cINPDEV::SetExclusive(eMOUSE_STATE_FRIEND_WINDOW_BUTTONS);
	else
		cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_FRIEND_WINDOW_BUTTONS);

	if (dwSelectMenu	==	1)
	{
		cFILE	file;

		char	strGuildName[dGUILD_NAME_LENGTH];
		m_inputGuildName.getStr(0,strGuildName,dGUILD_NAME_LENGTH);

		if (!CheckName(strGuildName) || CheckFilter(strGuildName,g_pstrSladerFilter,g_loaderText.m_iSlanderTextCount))
		{
			g_msgBox.cPopup("",dMSG_INCORRECT_GUILD_NAME,dMSG_OK);

			return;
		}

		s_agent.sendMakeGuild(strGuildName,m_strMemberList[m_iSelectSubMaster]);
	}
}

//
//	포커스가 간 멤버 찾기
int
CMakeGuildWindow::getFocusedMember()
{
	int		i;
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);

	int		iX,iY;

	iX		=	m_pos.x;
	iY		=	m_pos.y;

	iY	+=	60;
	iY	+=	20;
	iY	+=	80;
	iY	+=	20;
	iY	+=	4;

	for (i=1;i<dPARTY_MAXPLAYER;i++)
	{
		if (m_strMemberList[i][0]	==	NULL)	break;

		cRECT	rect;

		rect.set(iX+38,iY+(i-1)*16,g_iScreenWidth-10,iY+i*16);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
			return	i;
	}

	return	0xffff;
}	//	CMakeGuildWindow::getFocusedMember()

//
//	열려있다.
BOOL
CMakeGuildWindow::isOpened()
{
	if (s_iPopupInterface	==	ePIW_READY_TO_MAKE_GUILD)	return TRUE;

	return	FALSE;
}


