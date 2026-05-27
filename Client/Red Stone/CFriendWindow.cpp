#include "CWindowInterface.H"
#include "CGamePlay.H"
#include "CHero.H"
#include "CActor.H"
#include "CSound.H"
#include "CMessage.H"
#include <time.h>

cFriendWindow	g_gwFriend;

#define	dFRIEND_NAME_GAB				16
#define	dFRIEND_BAR_HEIGHT				16
#define	dFRIEND_BAR_GAB					3

#define	dFRIEND_WINDOW_EDGE_WIDTH_SIZE	10
#define	dFRIEND_WINDOW_EDGE_HEIGHT_SIZE	8

#define	dFRIEND_WINDOW_BOTTOM_HEIGHT		40
#define	dFRIEND_WINDOW_GROUP_NAME_HEIGHT	(dFRIEND_BAR_HEIGHT+dFRIEND_BAR_GAB)

enum
{
	eFBM_ADD_FRIEND,
	eFBM_REMOVE_FRIEND,
	eFBM_FIND_FRIEND,
	eFBM_DO_WHISPER,
	eFBM_RECOVER_LIST,
};



//
//친구 인터페이스 열기
void
cFriendWindow::open()
{
	if (s_iPopupInterface	==	ePIW_FRIEND)
	{
		ClosePopupWindow();

		return;
	}

	ClosePopupWindow();
	
	s_iPopupInterface		=	ePIW_FRIEND;
	s_propose.m_work.isReadyToAddFriend	=	FALSE;

	int	iWidth				=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleBarHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_FRIEND);

	m_posFriendWindow.x		=	g_iScreenWidth-iWidth;
	m_posFriendWindow.y		=	iTitleBarHeight+g_iCorrectBI_Y/2;
	int	iX					=	m_posFriendWindow.x;
	int	iY					=	m_posFriendWindow.y;

	s_rectInterfaceSize.set(g_iScreenWidth-iWidth, m_posFriendWindow.y-iTitleBarHeight, g_iScreenWidth, m_posFriendWindow.y+g_sprInterface.getSpriteHeight(eWINDOW));

	s_bmBottomMenu.resetChecker();
	s_bmBottomMenu.check(eBIM_MENU_FRIEND,TRUE);

	int	iBarHeight	=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);
	int	iBarWidth	=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);
	int	iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight=	g_sprInterface.getSpriteHeight(eWINDOW);

	s_bmButton.reset();

	iX				=	m_posFriendWindow.x+iWindowWidth-6-iBarWidth;
	iY				=	m_posFriendWindow.y+iWindowHeight-9-iBarHeight;
	m_iSelectFriend	=	0xffff;

	s_bmButton.addImageBar(dMSG_DO_WHISPER,eFBM_DO_WHISPER,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
	iX				-=	(iBarWidth+2);
	s_bmButton.addImageBar(dMSG_FIND_FRIEND,eFBM_FIND_FRIEND,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
	iX				-=	(iBarWidth+2);
	s_bmButton.addImageBar(dMSG_REMOVE_FRIEND,eFBM_REMOVE_FRIEND,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
	iX				-=	(iBarWidth+2);
	s_bmButton.addImageBar(dMSG_ADD_FRIEND,eFBM_ADD_FRIEND,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);

	s_bmButton.setClicktAllBarMargin(1,1);
	s_bmButton.setAllBarTextColor(0,FALSE);

	s_sbPopupWindow.set(m_posFriendWindow.x+iWindowWidth-16,m_posFriendWindow.y+6,iWindowHeight-dFRIEND_WINDOW_BOTTOM_HEIGHT);
	s_sbPopupWindow.setPos(0);

	resetFriendList();
	InitPopupWindowCloseButton();
}

//
//스킬 창 닫기
void
cFriendWindow::close()
{
	m_iFriendWindowScrollBarPos	=	s_sbPopupWindow.m_iCurrentPos;
	s_propose.m_work.isReadyToAddFriend		=	FALSE;

	cINPDEV::ReleaseExclusive();
	s_bmBottomMenu.check(eBIM_MENU_FRIEND,FALSE);
	s_bmButton.reset();
}

//
//	
void
cFriendWindow::resetFriendList()
{
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iFriendCount	=	g_hero.getFriendCount();

	m_iDisplayFriendCount	=	(iWindowHeight-dFRIEND_WINDOW_EDGE_HEIGHT_SIZE)/(dFRIEND_BAR_HEIGHT+dFRIEND_BAR_GAB)-1;
	s_sbPopupWindow.setSize(1,2);

	if	(iFriendCount		==	0)
	{
		s_bmButton.disable(eFBM_FIND_FRIEND);
		s_bmButton.disable(eFBM_DO_WHISPER);

		return;
	}

	if	(m_iSelectFriend	!=	0xffff	&&	m_iSelectFriend	>=	iFriendCount)
		m_iSelectFriend		=	iFriendCount-1;

	s_sbPopupWindow.setSize(getLineCount(),m_iDisplayFriendCount);
	s_sbPopupWindow.setPos(m_iFriendWindowScrollBarPos);

	s_bmButton.enable(eFBM_REMOVE_FRIEND);
	s_bmButton.enable(eFBM_FIND_FRIEND);
	s_bmButton.enable(eFBM_DO_WHISPER);
}

//
//아이템 인터페이스 그리기
void
cFriendWindow::draw()
{
	if (s_iPopupInterface	!=	ePIW_FRIEND)
		return;
	if (s_propose.m_work.isReadyToAddFriend)
		return;

	int		iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iX,iY;

	iX		=	m_posFriendWindow.x;
	iY		=	m_posFriendWindow.y;

	if (g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill	 (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface.Put(iX,iY,eWINDOW);
	g_sprInterface.Put(iX,g_iCorrectBI_Y/2 ,eWINDOW_TITLE_FRIEND);

	s_bmButton.draw();
	drawFriendList();
	s_sbPopupWindow.draw();
	
	g_sprInterface.Put(iX+7,463+g_iCorrectBI_Y/2		,eWINDOW_SEPERATE_BAR);	//	경계

}


//
//	친구 리스트 그리기
void
cFriendWindow::drawFriendList()
{
	int		i;
	int		iX,iY;
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iIconWidth		=	g_aJobIcon[0].m_sprite.getSpriteWidth(0)+2;
	int		iCount			=	0;
	int		iDisplayContextCount=	0;

	iX		=	m_posFriendWindow.x+dFRIEND_WINDOW_EDGE_WIDTH_SIZE+dFRIEND_NAME_GAB;
	iY		=	m_posFriendWindow.y+dFRIEND_WINDOW_EDGE_HEIGHT_SIZE;
/*
	for (i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		sprintf(g_hero.m_aFriendList[i].m_strName,"친구 #%.2d",i+1);

		g_hero.m_aFriendList[i].m_bf4Group	=	i/5;
		g_hero.m_aFriendList[i].m_bf8Job	=	i%14;
	}

	for	(i=0;i<dMAX_FRIEND_GROUP_COUNT;i++)
		sprintf(g_hero.m_strGroupName[i],"그룹 #%.2d",i+1);
*/
	for	(int iGroup=0;iGroup<dMAX_FRIEND_GROUP_COUNT;iGroup++)
	{
		int	iFriendCount,iActiveFriendCount;

		g_hero.getFriendCountByGroup(iGroup,&iFriendCount,&iActiveFriendCount);

		if	(iFriendCount	==	0)
			continue;

		iDisplayContextCount++;

		if	(iDisplayContextCount	>	s_sbPopupWindow.m_iCurrentPos)
		{
			iX		-=	dFRIEND_NAME_GAB;

			DWORD	dwTextColor	=	WHITE;

			s_ttCommon.m_dwPopupTime	=	s_ttCommon.m_dwPopupTime;

			if	(g_hero.m_abIsExpandFriendGroup[iGroup])
				g_sprInterface2.Put(iX+8,iY+10,eFWI_MINUS);
			else
				g_sprInterface2.Put(iX+8,iY+10,eFWI_PLUS);

			if	(iGroup	==	m_iFocusedGroup)
				dwTextColor	=	LTORANGE;

			s_text.put(iX+18,iY+4,dwTextColor,"%s [%d/%d]<n>",g_hero.m_strGroupName[iGroup],iActiveFriendCount,iFriendCount);

			iX		+=	dFRIEND_NAME_GAB;
			iY		+=	dFRIEND_WINDOW_GROUP_NAME_HEIGHT;
		}

		if	(iDisplayContextCount	>=	m_iDisplayFriendCount+s_sbPopupWindow.m_iCurrentPos)
			return;

		if	(g_hero.m_abIsExpandFriendGroup[iGroup]	==	FALSE)
			continue;

		for (i=0;i<dMAX_FRIEND_COUNT;i++)
		{
			int					iFriendIndex	=	i;
			cFriendInfoDefine	*lpFriendInfo	=	g_hero.getFriend(iFriendIndex);

			if	(!lpFriendInfo	||	lpFriendInfo->m_bf4Group	!=	iGroup)
				continue;

			iDisplayContextCount++;

			if	(iDisplayContextCount	<=	s_sbPopupWindow.m_iCurrentPos)
				continue;



			if	(iFriendIndex != m_iSelectFriend)
			{
				if	(iFriendIndex != m_iFocusFriend)
					s_text.put(iX+iIconWidth,iY+2,LTGRAY,lpFriendInfo->m_strName);
				else
					s_text.put(iX+iIconWidth,iY+2,GREEN,lpFriendInfo->m_strName);
			}
			else
			{
				iY	--;

				cDRAW::FillHB(_LTBLUE,iX-4,iY-1,iX+iWindowWidth-48,iY+16);

				s_text.put(iX+iIconWidth,iY+2,LTYELLOW,lpFriendInfo->m_strName);
				iY	++;
			}

			cANM	*lpIcon			=	&g_aJobIcon[lpFriendInfo->m_bf8Job];
			WORD	*lpPalette		=	lpIcon->getOrgPalette();
			WORD	awPlt[256];

			if	(lpFriendInfo->m_bf1IsOnline	==	FALSE)
			{
				EffectPalette(dPLT_EFFECT_GRAY,lpPalette,awPlt,80,0);

				lpIcon->setPalette(awPlt);
				lpIcon->PutReg(iX,iY,0,0,0);
				lpIcon->m_sprite.restorePalette();
			}
			else
			{
				int	iFPS		=	lpIcon->GetFPS(0);
				int	iFrameCount	=	lpIcon->GetFrameCount(0);

				lpIcon->PutReg(iX,iY,0,0,((s_iFrameCounter+iY)*iFPS/dSYNC_FPS)%iFrameCount);
			}


			iY	+=	(dFRIEND_BAR_HEIGHT+dFRIEND_BAR_GAB);

			if	(iDisplayContextCount	>=	m_iDisplayFriendCount+s_sbPopupWindow.m_iCurrentPos)
				return;
		}

	}
}

void
cFriendWindow::getFocusedFriend(int	*_lpiFocusedFriend,int *_lpiFocusedGroup)
{
	(*_lpiFocusedFriend)	=	0xffff;
	(*_lpiFocusedGroup)		=	0xffff;

	if	(cINPDEV::IsExclusive())
		return;

	int		i;
	int		iX,iY;
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iCount			=	0;
	int		iDisplayContextCount=	0;
	cRECT	rect;

	iX		=	m_posFriendWindow.x+dFRIEND_WINDOW_EDGE_WIDTH_SIZE+dFRIEND_NAME_GAB;
	iY		=	m_posFriendWindow.y+dFRIEND_WINDOW_EDGE_HEIGHT_SIZE;

	for	(int iGroup=0;iGroup<dMAX_FRIEND_GROUP_COUNT;iGroup++)
	{
		int	iFriendCount,iActiveFriendCount;

		g_hero.getFriendCountByGroup(iGroup,&iFriendCount,&iActiveFriendCount);

		if	(iFriendCount	==	0)
			continue;

		iDisplayContextCount++;

		if	(iDisplayContextCount	>	s_sbPopupWindow.m_iCurrentPos)
		{
			rect.set(iX-dFRIEND_NAME_GAB,iY-2,iX+iWindowWidth-dFRIEND_NAME_GAB-45,iY+18);

			if	(rect.isIn(s_posMouse.x,s_posMouse.y))
				(*_lpiFocusedGroup)	=	iGroup;

			iY		+=	dFRIEND_WINDOW_GROUP_NAME_HEIGHT;
		}

		if	(iDisplayContextCount	>=	m_iDisplayFriendCount+s_sbPopupWindow.m_iCurrentPos)
			return;

		if	(g_hero.m_abIsExpandFriendGroup[iGroup]	==	FALSE)
			continue;

		for (i=0;i<dMAX_FRIEND_COUNT;i++)
		{
			cFriendInfoDefine*	lpFriendInfo	=	g_hero.getFriend(i);

			if	(!lpFriendInfo	||	lpFriendInfo->m_bf4Group	!=	iGroup)
				continue;

			iDisplayContextCount++;

			if	(iDisplayContextCount	<=	s_sbPopupWindow.m_iCurrentPos)
				continue;

			rect.set(iX,iY-2,iX+iWindowWidth-dFRIEND_NAME_GAB-45,iY+18);

			if	(rect.isIn(s_posMouse.x,s_posMouse.y))
				(*_lpiFocusedFriend)	=	i;

			iY	+=	(dFRIEND_BAR_HEIGHT+dFRIEND_BAR_GAB);

			if	(iDisplayContextCount	>=	m_iDisplayFriendCount+s_sbPopupWindow.m_iCurrentPos)
				return;
		}
	}

	return;
}

int
cFriendWindow::getLineCount()
{
	int	iDisplayContextCount	=	0;

	for	(int iGroup=0;iGroup<dMAX_FRIEND_GROUP_COUNT;iGroup++)
	{
		int	iFriendCount,iActiveFriendCount;

		g_hero.getFriendCountByGroup(iGroup,&iFriendCount,&iActiveFriendCount);

		if	(iFriendCount	==	0)
			continue;

		iDisplayContextCount++;

		if	(g_hero.m_abIsExpandFriendGroup[iGroup]	==	FALSE)
			continue;

		for (int	i=0;i<dMAX_FRIEND_COUNT;i++)
		{
			cFriendInfoDefine*	lpFriendInfo	=	g_hero.getFriend(i);

			if	(!lpFriendInfo	||	lpFriendInfo->m_bf4Group	!=	iGroup)
				continue;

			iDisplayContextCount++;
		}
	}

	return	iDisplayContextCount;
}

//
//	아이템 윈도우에 팝업 메뉴 처리
void 
cFriendWindow::operatePopupMenu()
{
	if	(cINPDEV::IsExclusive(eMOUSE_STATE_POPUP_MENU))
		return;

	if	(!s_isClickedRightButton		)
		return;
	if	(m_iFocusedGroup	==	0xffff	&&	m_iFocusFriend	==	0xffff)
		return;

	m_iSelectFriend		=	m_iFocusFriend;
	m_iSelectedGroup	=	m_iFocusedGroup;

	if	(s_pmCommon.isPopuped())
	{
		s_pmCommon.reset();

		return;
	}

	if	(m_iSelectFriend	!=	0xffff)
		strcpy(m_strSelectFriendName,g_hero.m_aFriendList[m_iSelectFriend].m_strName);

	if	(m_iSelectedGroup	!=	0xffff)
		strcpy(m_strSelectGroupName,g_hero.m_strGroupName[m_iSelectedGroup]);

	s_pmCommon.popup(s_posMouse.x,s_posMouse.y);
	s_ttCommon.hide();

	if	(m_iFocusFriend	!=	0xffff)
	{
		for	(int iGroup=0;iGroup<dMAX_FRIEND_GROUP_COUNT;iGroup++)
		{
			if	(iGroup	==	g_hero.m_aFriendList[m_iFocusFriend].m_bf4Group)
				continue;

			char	strText[64];

			sprintf(strText,dMSG_CHANGE_FRIEND_GROUP_FORM,g_hero.m_strGroupName[iGroup]);

			s_pmCommon.addMenu(ePM_CHANGE_FRIEND_GROUP+iGroup,strText);
		}
	}

	if	(m_iFocusedGroup	!=	0xffff)
		s_pmCommon.addMenu(ePM_CHANGE_FRIEND_GROUP_NAME,dMSG_CHANGE_FRIEND_GROUP_NAME);

	if (s_pmCommon.getCount())
		cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);
	else
		s_pmCommon.reset();
}

void
cFriendWindow::recoverOldFriendList()
{
	BYTE	*pBuffer	=	new BYTE [sizeof(CHero)];
	CHero	*lpOldHero	=	(CHero*)pBuffer;

	{
		memcpy(lpOldHero,&g_hero,sizeof(g_hero));
//		strcpy(lpOldHero->m_strId,"mw1006");
//		strcpy(lpOldHero->m_strName,"1234565");
		for	(int iFriend=0;iFriend<dMAX_FRIEND_COUNT;iFriend++)
			lpOldHero->m_aFriendList[iFriend].reset();
		
		lpOldHero->readOldUserData();
	}

	int		iRecoverFriendCount	=	0;

	for	(int iFriend=0;iFriend<dMAX_FRIEND_COUNT;iFriend++)
	{
		cFriendInfoDefine	*lpFriend	=	&lpOldHero->m_aFriendList[iFriend];

		if	(lpFriend->m_strName[0]	==	NULL)
			break;

		if	(g_hero.isFriend(lpFriend->m_strName) || strlen(lpFriend->m_strName) > dNAME_LENGTH-1)
			continue;

		iRecoverFriendCount++;

//	약간의 암호화된 사인을 서버에 보낸다. 어차피 임시니까.. -_-a
		{
			WORD	wCheckValue	=	0;
			int		iLength		=	strlen(lpFriend->m_strName);
			int		iServerSign	=	g_hero.m_wServerSign;

			for (int i=0;i<iLength;i++)
			{
				int	iValue		=	(BYTE)lpFriend->m_strName[i];

				if	(iValue	<	'a')
					iValue		+=	'a'-'A';

				wCheckValue		+=	iValue+iServerSign;

				switch(i%4)
				{
					case	0	:
						iServerSign		+=	iValue/2;
						break;
					case	1	:
						iServerSign		+=	iValue/3;
						break;
					case	2	:
						iServerSign		+=	iValue/4;
						break;
					case	3	:
						iServerSign		+=	iValue/5;
						break;
				}
			}

			s_agent.sendRequestAddFriend(lpFriend->m_strName,TRUE,wCheckValue,lpFriend->m_bf8Job);
			Sleep(10);
		}
	}

	delete [] pBuffer;

	if	(iRecoverFriendCount)
	{
		char	strText[256];

		sprintf(strText,dMSG_RECOVER_FRIEND_LIST_FORM,iRecoverFriendCount);

		g_msgBox.cPopup("",strText,dMSG_OK);
	}
	else
		g_msgBox.cPopup("",dMSG_NOT_EXIST_RECOVER_FRIEND_LIST,dMSG_OK);
}

//
//스킬 인터페이스 업데이트
void
cFriendWindow::update()
{
	if	(s_iPopupInterface	!=	ePIW_FRIEND)
		return;

	if	(s_propose.m_work.isReadyToAddFriend	)
		return;

	s_rectInterfaceSize.x1 = g_iScreenWidth-g_sprInterface.getSpriteWidth(eWINDOW);
	s_rectInterfaceSize.x2 = g_iScreenWidth;;

	if	(!cINPDEV::IsExclusive(eMOUSE_STATE_SCROLL_BAR))
	{
		if	(s_sbPopupWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
			cINPDEV::SetExclusive(eMOUSE_STATE_SCROLL_BAR);
		else
			cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SCROLL_BAR);

		if(s_rectInterfaceSize.isIn(s_posMouse.x,s_posMouse.y))
		{
			if (s_iWheelValue<0)
				s_sbPopupWindow.setPos(s_sbPopupWindow.m_iCurrentPos+1);
			if (s_iWheelValue>0)
				s_sbPopupWindow.setPos(s_sbPopupWindow.m_iCurrentPos-1);
		}

		m_iFriendWindowScrollBarPos	=	s_sbPopupWindow.m_iCurrentPos;
	}

	getFocusedFriend(&m_iFocusFriend,&m_iFocusedGroup);

	DWORD	dwSelectMenu	=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	if	(s_bmButton.isPressed())
		cINPDEV::SetExclusive(eMOUSE_STATE_FRIEND_WINDOW_BUTTONS);
	else
		cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_FRIEND_WINDOW_BUTTONS);

	if	(cINPDEV::IsExclusive()	==	FALSE)
	{
		if	(s_isTouchLeftButton)
		{
			m_iSelectFriend	=	m_iFocusFriend;

			if	(m_iFocusedGroup	!=	0xffff)
			{
				if	(g_hero.m_abIsExpandFriendGroup[m_iFocusedGroup])
					g_hero.m_abIsExpandFriendGroup[m_iFocusedGroup]	=	FALSE;
				else
					g_hero.m_abIsExpandFriendGroup[m_iFocusedGroup]	=	TRUE;

				resetFriendList();
			}
		}

		operatePopupMenu();
	}

	switch(dwSelectMenu)
	{
		case	eFBM_ADD_FRIEND		:
			s_propose.m_work.isReadyToAddFriend	=	TRUE;
			s_rectInterfaceSize.set(0, 0, 0, 0);
			cINPDEV::SetExclusive(eMOUSE_STATE_PROPOSE_TO_WORK);
			break;

		case	eFBM_REMOVE_FRIEND	:
		{
			if (m_iSelectFriend	==	0xffff)
			{
				if	(!s_inputChatting.isEnable())
					EnableChattingBar(TRUE);

				s_inputChatting.setStr(0,_ms("@%s %s",dMSG_REMOVE_MY_NAME_IN_TARGET_FRIEND_LIST,dMSG_REMOVE_MY_NAME_IN_TARGET_FRIEND_LIST_COMMENT));

				s_inputChatting.setSelect(0,strlen(dMSG_REMOVE_MY_NAME_IN_TARGET_FRIEND_LIST)+2,-1);
			}
			else
			{
				char	*lpstrFriendName	=	g_hero.getFriendName(m_iSelectFriend);

				if (!lpstrFriendName)
					break;

				g_msgBox.cPopup("",_ms(dMSG_ARE_YOU_REALLY_REMOVE_THAT_FRIEND_FORM,lpstrFriendName),dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_REMOVE_FRIEND,eMBI_CANCEL);
			}
			break;
		}

		case	eFBM_FIND_FRIEND	:
		{
			char	*lpstrFriendName	=	g_hero.getFriendName(m_iSelectFriend);

			if (!lpstrFriendName)
				break;
			
			s_agent.sendFindActor(lpstrFriendName);
			break;
		}

		case	eFBM_DO_WHISPER		:
		{
			char	*lpFriendName	=	g_hero.getFriendName(m_iSelectFriend);

			if (!lpFriendName)
				break;

			EnableChattingBar(TRUE);
			s_inputChatting.reset();
			s_inputChatting.setStr(0,_ms("/%s ",lpFriendName));
			break;
		}
	}
}


//
//	친구 추가 요청
void
cFriendWindow::askAddFriend(char *_lpstrAsker)
{
	if (g_msgBox.isPoped())
		return;

	char	strMessage[256];

	strcpy(m_strAddFriendAskerName,_lpstrAsker);
	sprintf(strMessage,dMSG_REQEUST_ADD_FRIEND_FORM,_lpstrAsker);

	g_msgBox.cPopup(dMSG_REQEUST_ADD_FRIEND_MESSAGE,strMessage,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_ACCEPT_ADD_FRIEND_REQUEST,eMBI_DECLINE_ADD_FRIEND_REQUEST);
}

//	친구 제거 요청
void
cFriendWindow::askRemoveFriend(char *_lpstrAsker)
{
	if	(g_hero.removeFriendByName(_lpstrAsker))
	{
		s_agent.sendRemoveMyNameInTargetFriendListResult(_lpstrAsker,eRNITFLR_SUCCESS);

		char	strMessage[256];

		sprintf(strMessage,dMSG_REMOVE_FRIEND_BY_ASK_FORM,_lpstrAsker,_lpstrAsker);

		g_msgBox.cPopup("",strMessage,dMSG_OK);
	}
	else
		s_agent.sendRemoveMyNameInTargetFriendListResult(_lpstrAsker,eRNITFLR_NOT_EXIST);
}