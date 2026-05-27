#include "CPartyWindow.H"
#include "CHero.H"
#include "CActor.H"
#include "CMessage.H"
#include "partyDefine.H"
#include "cMAP.h"
enum
{
	ePWI_CHECK_CIRCLE_VALID	=	134,
	ePWI_JOB_ICON_KNIGHT	=	118,
};

#define	dPARTY_MEMBER_BAR_HEIGHT				46
#define	dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT_POS	108
#define	dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT		80
#define	dPARTY_WINDOW_SETTING_MENU_TEXT_THIN	14
#define	dPARTY_WINDOW_PREFERENCE_JOB_ICON_THIN	16

CPartyWindow	g_gwParty;

CPartyWindow::CPartyWindow()
{
	m_iPartyBarmenuStatus	=	ePBMS_NOTHING;
}

//
//	파티창 열기
void
CPartyWindow::open()
{
	if (s_iPopupInterface	==	ePIW_PARTY)
	{
		ClosePopupWindow();

		return;
	}

	ClosePopupWindow();

	m_bmPartyMenuButton.init(&g_sprInterface,10);

	int	iWindowWidth			=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight			=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_PARTY);

	s_iPopupInterface			=	ePIW_PARTY;
	s_partyInfo.resetPartyList();

	m_posPartyWindow.x			=	g_iScreenWidth-iWindowWidth;
	m_posPartyWindow.y			=	iWindowTitleHeight+g_iCorrectBI_Y/2;
	m_iPartyBarmenuStatus		=	ePBMS_NOTHING;
	s_partyInfo.m_wSelectMember	=	0;
	s_propose.m_work.isProposeToParty	=	FALSE;

	s_rectInterfaceSize.set(g_iScreenWidth-iWindowWidth, g_iCorrectBI_Y/2, g_iScreenWidth, iWindowTitleHeight+g_iCorrectBI_Y/2+iWindowHeight);

	s_sbMiddleWindow.set(m_posPartyWindow.x+iWindowWidth-16,m_posPartyWindow.y+iWindowHeight-dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT_POS,dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT);
	s_sbMiddleWindow.setPos(0);
	s_sbMiddleWindow.setSize(dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT+dPARTY_WINDOW_PREFERENCE_JOB_ICON_THIN*4+16*5 + 5,dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT);

	m_bmPartyMenuButton.reset();

	update();

	if(g_lpHero->isPartyLeader() && g_iSeasonVariable >= dSEASON_VARIABLE_PARTY_UNION)			//파티 연합은 시즌 2이상에서만 발동한다.
		m_iPartyBarmenuStatus = ePBMS_PARTY_UNION;

	s_bmBottomMenu.resetChecker();
	s_bmBottomMenu.check(eBIM_MENU_PARTY,TRUE);

	InitPopupWindowCloseButton();

	updateBarmenu();
}

//
//
void
CPartyWindow::close()
{
	s_rectInterfaceSize.set(0, 0, 0, 0);
	s_propose.m_work.isProposeToParty=	FALSE;
	m_bIsOpenOld		=	FALSE;
	s_bmBottomMenu.check(eBIM_MENU_PARTY,FALSE);
	cINPDEV::ReleaseExclusive();
}

//
//	캐릭터 이름 출력
void
CPartyWindow::drawMemberName(int _iX,int _iY,int _iMemberIndex,DWORD _dwTextColor)
{
	_iX		+=	14;

	char	strName[64];

	CPartyMemberInfo	*lpMember	=	s_partyInfo.getMember(_iMemberIndex);

	strcpy(strName,lpMember->m_strName);

	if	(lpMember->m_wIsDisconnected)
		g_sprInterface.Put(_iX  ,_iY,ePWI_HP_GAUGE_EMPTY);		//5
	else
	if	(lpMember->m_wSerial	==	0xffff)
		g_sprInterface.Put(_iX  ,_iY,ePWI_HP_GAUGE_DISABLE);		//5
	else
	{
		int		iBarWidth		=	g_sprInterface.getSpriteWidth(ePWI_HP_GAUGE_EMPTY);
		int		iNameWidth		=	30+(iBarWidth-30)*lpMember->m_wHP/100;
		int		iGaugeIndex		=	ePWI_HP_GAUGE1;
		int		iBarImage		=	ePWI_HP_GAUGE1+8*lpMember->m_wHP/100;

		iBarImage		=	min(iBarImage,ePWI_HP_GAUGE7);
		iBarImage		=	ePWI_HP_GAUGE1+ePWI_HP_GAUGE7-iBarImage;

		g_sprInterface.Put(_iX  ,_iY,ePWI_HP_GAUGE_EMPTY);		//5
		g_sprInterface.putClipedImage(_iX,_iY,iBarImage,iNameWidth);
	}

//	직업 아이콘
	{
		int		iFPS			=	g_aJobIcon[lpMember->m_wJob].GetFPS(0);
		int		iFrameCount		=	g_aJobIcon[lpMember->m_wJob].GetFrameCount(0);
		g_aJobIcon[lpMember->m_wJob].PutReg(_iX+2,_iY+1,0,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount);
		g_sprInterface.Put(_iX	,_iY,ePWI_JOB_ICON_FRAME);
	}

	s_text.put(_iX+36,_iY+3,_dwTextColor,strName);
}

void
CPartyWindow::drawPartyInfo()
{
	drawRequestJoinPartyUserList();

	int		iX,iY,i,iCount=0;

	int		iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iBarHeight	=	dPARTY_MEMBER_BAR_HEIGHT;
	int		iBarWidth	=	256;
	DWORD	dwNameTextColor;

	iX		=	m_posPartyWindow.x+9;
	iY		=	m_posPartyWindow.y+2;

	for (i=0;i<s_partyInfo.m_wMemberCount;i++)
	{
		CPartyMemberInfo	*lpMember	=	s_partyInfo.getMember(i%s_partyInfo.m_wMemberCount);

		if	(s_partyInfo.m_wSelectMember==	i)
		{
			g_sprInterface.Put(iX-1,iY+4,ePWI_SELECT_PANNEL,100,100,dPUT_HALF_BLENDING);
			dwNameTextColor		=	LTYELLOW;
		}
		else
			dwNameTextColor		=	WHITE;

		if (lpMember->m_wSerial	==	0xfff)
			dwNameTextColor		=	GRAY;

		drawMemberName(iX+6,iY+7 ,i,dwNameTextColor);

		if	(s_partyInfo.m_bf3PartyObject	==	ePO_EXPLORATION)
			s_text.put(iX+22,iY+30,dwNameTextColor,dMSG_EXPLORATION_COUNT_FORM,g_pstrHeroJobName[lpMember->m_wJob],lpMember->m_wLevel,lpMember->m_wPlaySecretDungeonCount);
		else
		if	(g_bIsDuelServer)
			s_text.put(iX+22,iY+30,dwNameTextColor,"%s(%s %d)",g_pstrHeroJobName[lpMember->m_wJob],dMSG_WINNING_POINT,lpMember->m_iDuelVP);
		else
			s_text.put(iX+22,iY+30,dwNameTextColor,"%s(Lv%d)",g_pstrHeroJobName[lpMember->m_wJob],lpMember->m_wLevel);

		g_sprInterface.Put(iX+8,iY+34,eGBSW_1+i);

		iY	+=	iBarHeight;
		g_sprInterface.Put(iX-2,iY-2,ePWI_BORDER);
	}

	g_sprInterface.Put(iX-2,m_posPartyWindow.y+iBarHeight*8,ePWI_BORDER);

	iX					=	m_posPartyWindow.x+iWindowWidth/2-20;
	iY					=	m_posPartyWindow.y+iWindowHeight-dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT_POS;

	s_text.setClipArea(m_posPartyWindow.x,iY,iX+iWindowWidth,iY+dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT-2);
	cDRAW::SetClippginArea(m_posPartyWindow.x,iY,iX+iWindowWidth,iY+dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT-2);

	iY					=	m_posPartyWindow.y+dPARTY_MEMBER_BAR_HEIGHT*8+10+4 - s_sbMiddleWindow.m_iCurrentPos;

	DWORD	dwTextColor	=	GREEN;
	char	*lpstrText;

	{
		if (m_iFocusPartyStatus	==	ePS_PARTY_STATUS)
			dwTextColor	=	LTGREEN;
		else
			dwTextColor	=	GREEN;
		
		s_text.putRF(iX,iY,WHITE,dMSG_PARTY_STATUS);
		lpstrText	=	s_partyInfo.getPartyStatusString();
		s_text.put(iX+6,iY,dwTextColor,lpstrText);
	}

	{
		iY	+=	dPARTY_WINDOW_SETTING_MENU_TEXT_THIN;

		if (m_iFocusPartyStatus	==	ePS_GOLD_SHARING_METHOD)
			dwTextColor	=	LTGREEN;
		else
			dwTextColor	=	GREEN;
		
		s_text.putRF(iX,iY,WHITE,dMSG_SHARING_GOLD_METHOD);
		lpstrText	=	s_partyInfo.getGoldSharingMethodString();
		s_text.put(iX+6,iY,dwTextColor,lpstrText);
	}

	{
		iY	+=	dPARTY_WINDOW_SETTING_MENU_TEXT_THIN;

		if (m_iFocusPartyStatus	==	ePS_ITEM_SHARING_METHOD)
			dwTextColor	=	LTGREEN;
		else
			dwTextColor	=	GREEN;

		s_text.putRF(iX,iY,WHITE,dMSG_SHARING_ITEM_METHOD);

		lpstrText	=	s_partyInfo.getItemSharingMethodString();

		s_text.put(iX+6,iY,dwTextColor,lpstrText);
	}

	{
		iY	+=	dPARTY_WINDOW_SETTING_MENU_TEXT_THIN;

		if	(m_iFocusPartyStatus	==	ePS_PARTY_OBJECT)
			dwTextColor	=	LTGREEN;
		else
			dwTextColor	=	GREEN;

		s_text.putRF(iX,iY,WHITE,dMSG_PARTY_OBJECT);

		lpstrText	=	s_partyInfo.getPartyObjectString();

		s_text.put(iX+6,iY,dwTextColor,lpstrText);
	}

//	파티 멤버 모집 레벨
	{
		iY	+=	dPARTY_WINDOW_SETTING_MENU_TEXT_THIN;

		if	(m_iFocusPartyStatus	==	ePS_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE)
			dwTextColor	=	LTGREEN;
		else
			dwTextColor	=	GREEN;

		s_text.putRF(iX,iY,WHITE,dMSG_RECRUIT_PARTY_MEMBER_LIMIT_LEVEL);

		lpstrText	=	s_partyInfo.getPartyObjectString();

		s_text.put(iX+6,iY,dwTextColor,dMSG_RECRUIT_PARTY_MEMBER_LEVEL_RANGE_FORM,s_partyInfo.m_wRecruitMemberRange);
	}

	if	(m_iFocusPartyStatus	==	ePS_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE)
	{
		int		iAverageLevel	=	s_partyInfo.getAverageLevel();
		int		iMinLevel		=	max(iAverageLevel-s_partyInfo.m_wRecruitMemberRange,1);
		int		iMaxLevel		=	min(iAverageLevel+s_partyInfo.m_wRecruitMemberRange,1000);

		char	strTip[256];

		sprintf(strTip,"%s %d~%d",dMSG_RECRUIT_PARTY_MEMBER_LIMIT_LEVEL,iMinLevel,iMaxLevel);

		CPos	pos;

		pos.x	=	m_posPartyWindow.x+iWindowWidth/2+6-30;
		pos.y	=	iY-26;

		cRECT	rect;

		rect.set(pos.x,pos.y,pos.x+100,pos.y+20);

		s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strTip);
		
//		s_partyInfo.getAvarageLevel();
	}

//	파티 선호 직업
	{
		iY	+=	16;
		iX	=	m_posPartyWindow.x+10;

		if	(iY	>	m_posPartyWindow.y+iWindowHeight-dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT_POS	&&
			iY	<	m_posPartyWindow.y+iWindowHeight-dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT_POS+dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT)
			cDRAW::XLineHB(_WHITE,iX,iX+iWindowWidth-28,iY-2,1);

		iY	+=	2;

		s_text.put(iX,iY,GOLDYELLOW,dMSG_PREFERENCE_JOB);

		iY	+=	16;

		for	(int iJob=0;iJob<dPLAYER_JOB_COUNT-1  ;iJob++)	
		{
			CPos	pos;

			pos.x	=	iX+(iJob%4) * 56+16;
			pos.y	=	iY+iJob/4*dPARTY_WINDOW_PREFERENCE_JOB_ICON_THIN;

			int	iMask		=	1;
			iMask			<<=	iJob;
			int	iIsChecked	=	FALSE;

			if	(s_partyInfo.m_dwPreferenceJobMask&iMask)
				iIsChecked	=	TRUE;

			g_sprInterface2.Put(pos.x,pos.y+4,ePWI_CHECK_CIRCLE_VALID+iIsChecked);

			cANM	*lpIcon			=	&g_aJobIcon[iJob];
			WORD	*lpPalette		=	lpIcon->getOrgPalette();
			WORD	awPlt[256];


			if	(iIsChecked	==	FALSE)
			{
				EffectPalette(dPLT_EFFECT_GRAY,lpPalette,awPlt,80,0);
				lpIcon->setPalette(awPlt);

				lpIcon->PutReg(pos.x+10,pos.y-1,0,0,0);
				lpIcon->m_sprite.restorePalette();
			}
			else
			{
				int		iFPS			=	lpIcon->GetFPS(0);
				int		iFrameCount		=	lpIcon->GetFrameCount(0);
				lpIcon->PutReg(pos.x+10,pos.y-1,0,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount);
			}
		}
	}







//	파티 설명
	{
		iX	=	m_posPartyWindow.x+10;
		iY	+=	5*dPARTY_WINDOW_PREFERENCE_JOB_ICON_THIN+2;

		if	(iY	>	m_posPartyWindow.y+iWindowHeight-dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT_POS	&&
			iY	<	m_posPartyWindow.y+iWindowHeight-dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT_POS+dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT)
			cDRAW::XLineHB(_WHITE,iX,iX+iWindowWidth-28,iY-2,1);

		iY	+=	2;

		s_text.put(iX,iY,GOLDYELLOW,dMSG_PARTY_EXPLAIN);

		if	(g_lpHero->isPartyLeader())
		{
			int	iWidth	=	s_text.getPixelSize(dMSG_PARTY_EXPLAIN);

			DWORD	dwTextColor;

			if	(m_iFocusPartyStatus	==	ePS_CHANGE_PARTY_PROSPECTUS_TEXT)
				dwTextColor	=	LTGREEN;
			else
				dwTextColor	=	GREEN;

			s_text.put(iX+iWidth+10,iY,dwTextColor,dMSG_CHANGE);
		}

		iY	+=	16;

		s_text.put(iX+16,iY,iBarWidth-40,48,0,WHITE,s_partyInfo.m_strPartyProspectus);
	}

	s_text.releaseClipArea();
	cDRAW::FreeClippginArea();
	s_sbMiddleWindow.draw();
}
//
//	파티창 그리기
void
CPartyWindow::draw()
{
	if	(s_propose.m_work.isProposeToParty)
		return;

	int		iX,iY;
	int		iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight=	g_sprInterface.getSpriteHeight(eWINDOW);

	iX		=	m_posPartyWindow.x;
	iY		=	m_posPartyWindow.y;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+4+iWindowWidth-8,iY+iWindowHeight-8);
	else
		cDRAW::Fill	 (0,iX+8,iY+6,iX+4+iWindowWidth-8,iY+iWindowHeight-8);

	g_sprInterface.Put(iX,iY,eWINDOW);
	g_sprInterface.Put(iX,g_iCorrectBI_Y/2,eWINDOW_TITLE_PARTY);

	iX		=	m_posPartyWindow.x+9;
	iY		=	m_posPartyWindow.y+2;

	m_bmPartyMenuButton.draw();

	if	(g_lpHero->isPartyMember() && m_iPartyBarmenuStatus != ePBMS_PARTY_UNION)
	{
		drawPartyInfo();
		return;
	}
		drawPartyList();
}


//
//	파티 바 메뉴 업데이트
void
CPartyWindow::updateBarmenu()
{
	int	iX		=	m_posPartyWindow.x;
	int	iY		=	m_posPartyWindow.y;

	if	(g_lpHero->isPartyMember())
	{
		if	(!cINPDEV::IsExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR))
		{
			if	(s_sbMiddleWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
				cINPDEV::SetExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR);
			else
				cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SHOP_SCROLL_BAR);
		}

		
		if (m_iPartyBarmenuStatus	==	ePBMS_PARTY_UNION && m_bIsOpenOld == FALSE)
		{
			int	iBarHeight	=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);
			int	iBarWidth	=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);
			int	iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
			int	iWindowHeight=	g_sprInterface.getSpriteHeight(eWINDOW);
			
			m_bmPartyMenuButton.reset();		
			m_bIsOpenOld = TRUE;
			
			iX				=	m_posPartyWindow.x+iWindowWidth-6-iBarWidth;
			iY				=	m_posPartyWindow.y+iWindowHeight-9-iBarHeight;
			
			m_bmPartyMenuButton.addImageBar(dMSG_NEXT_PARTY_LIST,ePBM_ASK_NEXT_PARTY_LIST,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
			iX				-=	(iBarWidth+2);
			m_bmPartyMenuButton.addImageBar(dMSG_PREVIOUS_PARTY_LIST,ePBM_ASK_PREVIOUS_PARTY_LIST,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
			iX				-=	(iBarWidth+2);
			m_bmPartyMenuButton.addImageBar(dMSG_UNION_PARTY,ePBM_ASK_PARTY_UNION,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
			iX				-=	(iBarWidth+2);
			m_bmPartyMenuButton.addImageBar(dMSG_PARTY_INFO , ePBM_SHOW_PARTY_INFO , iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
			
			m_bmPartyMenuButton.setClicktAllBarMargin(1,1);
			m_bmPartyMenuButton.setAllBarTextColor(0,FALSE);
			
			
		}

		if	(m_iPartyBarmenuStatus	!=	ePBMS_PARTY_INFO && m_iPartyBarmenuStatus != ePBMS_PARTY_UNION)
		{
			int	iBarHeight	=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);
			int	iBarWidth	=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);
			int	iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
			int	iWindowHeight=	g_sprInterface.getSpriteHeight(eWINDOW);
			
			m_bmPartyMenuButton.reset();
			
			iX				=	m_posPartyWindow.x+iWindowWidth-6-iBarWidth;
			iY				=	m_posPartyWindow.y+iWindowHeight-9-iBarHeight;
			
			m_bmPartyMenuButton.addImageBar(dMSG_CHANGE_PARTY_LEADER,ePBM_CHANGE_PARTY_LEADER,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
			iX				-=	(iBarWidth+2);
			m_bmPartyMenuButton.addImageBar(dMSG_LEAVE_PARTY,ePBM_LEAVE_PARTY,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
			iX				-=	(iBarWidth+2);
			m_bmPartyMenuButton.addImageBar(dMSG_BANISH_MEMBER,ePBM_ASK_BANISH_MEMBER,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
			iX				-=	(iBarWidth+2);
			m_bmPartyMenuButton.addImageBar(dMSG_ASK_JOIN_PARTY,ePBM_ASK_JOIN_PARTY,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
			
			m_bmPartyMenuButton.setClicktAllBarMargin(1,1);
			m_bmPartyMenuButton.setAllBarTextColor(0,FALSE);
			
			m_iPartyBarmenuStatus	=	ePBMS_PARTY_INFO;
			
			int	iTextWidth	=	s_text.getPixelSize(dMSG_RAISE_PARTY_MEMBER);
			iX				=	m_posPartyWindow.x+iWindowWidth/2-14;
			iY				=	m_posPartyWindow.y+dPARTY_MEMBER_BAR_HEIGHT*8+10;

		}

		if	(g_lpHero->isPartyLeader())
		{
			if	(s_partyInfo.m_wSelectMember	==	0xffff	||	s_partyInfo.m_wSelectMember	==	0)
			{
				m_bmPartyMenuButton.disable(ePBM_ASK_BANISH_MEMBER);
				m_bmPartyMenuButton.disable(ePBM_CHANGE_PARTY_LEADER);
			}
			else
			{
				m_bmPartyMenuButton.enable(ePBM_ASK_BANISH_MEMBER);
				m_bmPartyMenuButton.enable(ePBM_CHANGE_PARTY_LEADER);
			}
			
			if	(s_partyInfo.isFullParty())
				m_bmPartyMenuButton.disable(ePBM_ASK_JOIN_PARTY);
			else
				m_bmPartyMenuButton.enable(ePBM_ASK_JOIN_PARTY);		
		}
		else
		{
			m_bmPartyMenuButton.disable(ePBM_ASK_BANISH_MEMBER);
			m_bmPartyMenuButton.disable(ePBM_ASK_JOIN_PARTY);
			m_bmPartyMenuButton.disable(ePBM_CHANGE_PARTY_LEADER);
		}
	}
	else
	{
		if	(m_iPartyBarmenuStatus	!=	ePBMS_PARTY_LIST)
		{
			int	iBarHeight	=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);
			int	iBarWidth	=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);
			int	iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
			int	iWindowHeight=	g_sprInterface.getSpriteHeight(eWINDOW);

			m_bmPartyMenuButton.reset();

			iX				=	m_posPartyWindow.x+iWindowWidth-6-iBarWidth;
			iY				=	m_posPartyWindow.y+iWindowHeight-9-iBarHeight;

			m_bmPartyMenuButton.addImageBar(dMSG_NEXT_PARTY_LIST,ePBM_ASK_NEXT_PARTY_LIST,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
			iX				-=	(iBarWidth+2);
			m_bmPartyMenuButton.addImageBar(dMSG_PREVIOUS_PARTY_LIST,ePBM_ASK_PREVIOUS_PARTY_LIST,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
			iX				-=	(iBarWidth+2);
			m_bmPartyMenuButton.addImageBar(dMSG_ASK_JOIN_PARTY,ePBM_ASK_JOIN_PARTY,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);

			m_bmPartyMenuButton.setClicktAllBarMargin(1,1);
			m_bmPartyMenuButton.setAllBarTextColor(0,FALSE);

			m_iPartyBarmenuStatus	=	ePBMS_PARTY_LIST;
		}
	}

	if(g_map.m_bf1IsGuildDungeonMap)
	{
		m_bmPartyMenuButton.disable(ePBM_ASK_BANISH_MEMBER);
		m_bmPartyMenuButton.disable(ePBM_LEAVE_PARTY);
	}

	if	(s_propose.m_work.isProposeToParty)
		m_bmPartyMenuButton.disable(ePBM_ASK_JOIN_PARTY);
	else
	{
		if	(g_lpHero->isPartyLeader() || g_lpHero->isPartyMember() == FALSE)
			m_bmPartyMenuButton.enable(ePBM_ASK_JOIN_PARTY);
	}

	updateBarbutton();
}

void
CPartyWindow::updateBarbutton()
{
	DWORD	dwMenu	=	m_bmPartyMenuButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
	case	ePBM_LEAVE_PARTY				:
		g_msgBox.cPopup("",dMSG_ARE_YOU_REALLY_LEAVE_PARTY,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_LEAVE_PARTY,eMBI_CANCEL);
		break;
		
	case	ePBM_ASK_JOIN_PARTY				:
		{
			if	(s_partyInfo.m_iSelectParty	!=	-1)	//	파티 리스트에서 파티 선택후 파티 참가한다고 하는거다.
			{
				char	*lpstrMasterName	=	s_partyInfo.m_aPartyList[s_partyInfo.m_iSelectParty].m_strMasterName;
				s_agent.sendAskJoinParty(lpstrMasterName,TRUE);
				g_lpHero->warning(_ms(dMSG_PROPOSE_PARTY_FORM,lpstrMasterName));
				break;
			}
			
			s_propose.m_work.isProposeToParty	=	TRUE;
			s_rectInterfaceSize.set(0, 0, 0, 0);
			cINPDEV::SetExclusive(eMOUSE_STATE_PROPOSE_TO_WORK);
			break;
		}
		
	case	ePBM_ASK_BANISH_MEMBER			:
		g_msgBox.cPopup("",dMSG_ARE_YOU_REALLY_BANISH_SELECT_MEMBER,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_BANISH_MEMBER,eMBI_CANCEL);
		break;
		
	case	ePBM_CHANGE_PARTY_LEADER		:
		{
			if	(s_partyInfo.m_aMemberList[s_partyInfo.m_wSelectMember].m_wIsDisconnected)
				break;
			
			g_msgBox.cPopup("",_ms(dMSG_QUESTION_CHANGE_PARTY_LEADER_FORM,s_partyInfo.m_aMemberList[s_partyInfo.m_wSelectMember].m_strName),dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_CHANGE_PARTY_LEADER,eMBI_CANCEL);
			break;
		}
		
	case	ePBM_ASK_NEXT_PARTY_LIST		:
		{
			CB_ASK_OPEN_PARTY_LIST	packet;
			
			packet.base.set(sizeof(packet),dCB_ASK_OPEN_PARTY_LIST);
			packet.bf10LastReceiveParty	=	s_partyInfo.m_iLastPartySerial;
			packet.bf1IsWantNext		=	TRUE;
			
			s_agent.sendBCSPacket(&packet);
			
			break;
		}
	case	ePBM_ASK_PREVIOUS_PARTY_LIST	:
		{
			CB_ASK_OPEN_PARTY_LIST	packet;
			
			packet.base.set(sizeof(packet),dCB_ASK_OPEN_PARTY_LIST);
			packet.bf10LastReceiveParty	=	s_partyInfo.m_iHeadPartySerial;
			packet.bf1IsWantNext		=	FALSE;
			
			s_agent.sendBCSPacket(&packet);
			
			break;
		}
	case	ePBM_SHOW_PARTY_INFO			:
		{
			m_iPartyBarmenuStatus		=		ePBMS_NOTHING;
		}
		break;
	case	ePBM_ASK_PARTY_UNION			:
		{
			s_propose.m_work.isProposeToUnionParty	=	TRUE;
			cINPDEV::SetExclusive(eMOUSE_STATE_PROPOSE_TO_WORK);
			ClosePopupWindow();			
		}
		break;
	}


	if	(m_bmPartyMenuButton.isPressed())
		cINPDEV::SetExclusive(eMOUSE_STATE_INTERFACE);
	else
		cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_INTERFACE);
}

//
//	파티창 업데이터
void
CPartyWindow::update()
{
	int		iX,iY,i,iCount=0;
	int		iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iBarHeight	=	dPARTY_MEMBER_BAR_HEIGHT;
	int		iBarWidth	=	256;

	if	(s_iPopupInterface	!=	ePIW_PARTY)
		return;

	if	(!cINPDEV::IsExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR))
	{
		if	(s_sbMiddleWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
			cINPDEV::SetExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR);
		else
			cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SHOP_SCROLL_BAR);
	}

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_INTERFACE))
		return;

	int	iWindowTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_PARTY);
	if (!s_propose.m_work.isProposeToParty && s_rectInterfaceSize.x1 == 0)
		s_rectInterfaceSize.set(g_iScreenWidth-iWindowWidth, g_iCorrectBI_Y/2, g_iScreenWidth, iWindowTitleHeight+g_iCorrectBI_Y/2+iWindowHeight);

	updateRequestJoinPartyUserList();
	updateBarmenu();

	if	(cINPDEV::IsExclusive())
		return;

	s_partyInfo.m_iFocusParty	=	getFocusParty();

	{
		m_iFocusPartyStatus	=	-1;

		cRECT	rectCheck;

		iX		=	m_posPartyWindow.x;
		iY		=	m_posPartyWindow.y+iWindowHeight-dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT_POS;

		rectCheck.set(iX+4,iY+2,iX+iWindowWidth,iY+dPARTY_WINDOW_BOTTOM_STATUS_HEIGHT);

		if	(rectCheck.isIn(s_posMouse.x,s_posMouse.y))
		{
			iX		=	m_posPartyWindow.x+iWindowWidth/2-10-20;
			iY		=	m_posPartyWindow.y+dPARTY_MEMBER_BAR_HEIGHT*8+10+4-s_sbMiddleWindow.m_iCurrentPos;
			rectCheck.set(iX+4,iY-2,iX+iWindowWidth/2-8,iY+14);
			if (rectCheck.isIn(s_posMouse.x,s_posMouse.y))
				m_iFocusPartyStatus	=	ePS_PARTY_STATUS;
			rectCheck.add(0,dPARTY_WINDOW_SETTING_MENU_TEXT_THIN);
			if (rectCheck.isIn(s_posMouse.x,s_posMouse.y))
				m_iFocusPartyStatus	=	ePS_GOLD_SHARING_METHOD;
			rectCheck.add(0,dPARTY_WINDOW_SETTING_MENU_TEXT_THIN);
			if (rectCheck.isIn(s_posMouse.x,s_posMouse.y))
				m_iFocusPartyStatus	=	ePS_ITEM_SHARING_METHOD;
			rectCheck.add(0,dPARTY_WINDOW_SETTING_MENU_TEXT_THIN);
			if (rectCheck.isIn(s_posMouse.x,s_posMouse.y))
				m_iFocusPartyStatus	=	ePS_PARTY_OBJECT;
			rectCheck.add(0,dPARTY_WINDOW_SETTING_MENU_TEXT_THIN);
			if (rectCheck.isIn(s_posMouse.x,s_posMouse.y))
				m_iFocusPartyStatus	=	ePS_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE;

			iX	=	m_posPartyWindow.x+10;
			iY	=	rectCheck.y1;
			iY	+=	16;
			iY	+=	2;
			iY	+=	16;

			for	(int iJob=0;iJob<dPLAYER_JOB_COUNT-1;iJob++)
			{
				CPos	pos;

				pos.x	=	iX+(iJob%4) * 56+16;
				pos.y	=	iY+iJob/4*dPARTY_WINDOW_PREFERENCE_JOB_ICON_THIN;		// 수정,..

				int	iMask		=	1;
				iMask			<<=	iJob;
				int	iIsChecked	=	FALSE;
				int	iSpriteIndex=	ePWI_CHECK_CIRCLE_VALID;

				int	iButtonWidth=	g_sprInterface2.getSpriteWidth(ePWI_CHECK_CIRCLE_VALID);
				int	iButtonHeight=	g_sprInterface2.getSpriteHeight(ePWI_CHECK_CIRCLE_VALID);

				rectCheck.set(pos.x-2,pos.y,pos.x+56,pos.y+dPARTY_WINDOW_PREFERENCE_JOB_ICON_THIN);

				if	(rectCheck.isIn(s_posMouse.x,s_posMouse.y))
				{
					m_iFocusPartyStatus	=	ePS_PARTY_PREFERENCE_JOB_CHECK_BOX+iJob;
					break;
				}
			}

			iY	+=	5*dPARTY_WINDOW_PREFERENCE_JOB_ICON_THIN+2;
			iY	+=	2;
			iX	=	m_posPartyWindow.x+10;

			int	iTextWidth	=	s_text.getPixelSize(dMSG_PARTY_EXPLAIN)+10;
			int	iTextWidth2	=	s_text.getPixelSize(dMSG_CHANGE);

			rectCheck.set(iX+iTextWidth,iY-2,iX+iTextWidth+iTextWidth2,iY+14);

			if	(rectCheck.isIn(s_posMouse.x,s_posMouse.y))
				m_iFocusPartyStatus	=	ePS_CHANGE_PARTY_PROSPECTUS_TEXT;
		}
	}

	s_isTouchRightButton	=	s_isTouchRightButton;
	
	if	(s_isClickedLeftButton	&&	g_lpHero->isPartyLeader())
	{
		iX				=	m_posPartyWindow.x+iWindowWidth/2+6-30;
		iY				=	m_posPartyWindow.y+dPARTY_MEMBER_BAR_HEIGHT*8+10+4-s_sbMiddleWindow.m_iCurrentPos;

		int	iTextWidth	=	0;

		if	(m_iFocusPartyStatus	==	ePS_PARTY_STATUS)
		{
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_RAISE_PARTY_MEMBER));
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_DO_NOT_RAISE_PARTY_MEMBER));

			s_pmCommon.popup(iX-iTextWidth,iY);

			s_pmCommon.addMenu(ePSBM_PS_RAISE_PARTY_MEMBER,dMSG_RAISE_PARTY_MEMBER);
			s_pmCommon.addMenu(ePSBM_PS_DO_NOT_RAISE_PARTY_MEMBER,dMSG_DO_NOT_RAISE_PARTY_MEMBER);
		}
		else
		if	(m_iFocusPartyStatus	==	ePS_GOLD_SHARING_METHOD)
		{
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_FREE_ACQUISITION));
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_EQUALITY_SHARING));
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_ORDER_ACQUISITION));

			s_pmCommon.popup(iX-iTextWidth,iY+dPARTY_WINDOW_SETTING_MENU_TEXT_THIN);

			s_pmCommon.addMenu(ePSBM_PGSM_FREE,dMSG_FREE_ACQUISITION);
			s_pmCommon.addMenu(ePSBM_PGSM_EQUALITY,dMSG_EQUALITY_SHARING);
			s_pmCommon.addMenu(ePSBM_PGSM_ORDER,dMSG_ORDER_ACQUISITION);
		}
		else
		if	(m_iFocusPartyStatus	==	ePS_ITEM_SHARING_METHOD)
		{
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_FREE_ACQUISITION));
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_RANDOM_ACQUISITION));
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_ORDER_ACQUISITION));
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_FIRST_JOB_RANDOM_ACQUISITION));

			s_pmCommon.popup(iX-iTextWidth,iY+dPARTY_WINDOW_SETTING_MENU_TEXT_THIN*2);

			s_pmCommon.addMenu(ePSBM_PISM_FREE,dMSG_FREE_ACQUISITION);
			s_pmCommon.addMenu(ePSBM_PISM_RANDOM,dMSG_RANDOM_ACQUISITION);
			s_pmCommon.addMenu(ePSBM_PISM_ORDER,dMSG_ORDER_ACQUISITION);
			s_pmCommon.addMenu(ePSBM_PISM_FIRST_JOB_RANDOM,dMSG_FIRST_JOB_RANDOM_ACQUISITION);
	//		s_pmCommon.addMenu(ePSBM_PISM_FIRST_JOB_ORDER,dMSG_FIRST_JOB_ORDER_ACQUISITION);
		}
		else
		if	(m_iFocusPartyStatus	==	ePS_PARTY_OBJECT)
		{
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_HUNTING));
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_EXPLORATION_SECRET_DUNGEON));
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_OPERATE_QUEST));

			s_pmCommon.popup(iX-iTextWidth,iY+dPARTY_WINDOW_SETTING_MENU_TEXT_THIN*3);

			s_pmCommon.addMenu(ePSBM_PO_HUNT,dMSG_HUNTING);
			s_pmCommon.addMenu(ePSBM_PO_EXPLORATION,dMSG_EXPLORATION_SECRET_DUNGEON);
			s_pmCommon.addMenu(ePSBM_PO_QUEST,dMSG_OPERATE_QUEST);
		}
		else
		if	(m_iFocusPartyStatus	==	ePS_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE)
		{
			iTextWidth	=	max(iTextWidth,s_pmCommon.m_lpText->getPixelSize(dMSG_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE));

			s_pmCommon.popup(iX-iTextWidth,iY+dPARTY_WINDOW_SETTING_MENU_TEXT_THIN*4);

			s_pmCommon.addMenu(ePSBM_CHANGE_RECRUIT_PARTY_MEMBER_RANGE,dMSG_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE);
		}
		else
		if	(m_iFocusPartyStatus	==	ePS_CHANGE_PARTY_PROSPECTUS_TEXT)
		{
			if	(!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			s_inputChatting.setStr(0,_ms("@%s %s",dMSG_CHANGE_PARTY_PROSPECTUS_TEXT_KEYWORD,dMSG_CHANGE_PARTY_PROSPECTUS_TEXT_COMMENT));

			s_inputChatting.setSelect(0,strlen(dMSG_CHANGE_PARTY_PROSPECTUS_TEXT_KEYWORD)+2,-1);
		}
		else
		if	(m_iFocusPartyStatus	>=	ePS_PARTY_PREFERENCE_JOB_CHECK_BOX	&&	m_iFocusPartyStatus	<	ePS_PARTY_PREFERENCE_JOB_CHECK_BOX+dPLAYER_JOB_COUNT	)
		{
			int		iJob			=	m_iFocusPartyStatus-ePS_PARTY_PREFERENCE_JOB_CHECK_BOX;
			DWORD	dwMask			=	1<<iJob;
			DWORD	dwOldMask		=	s_partyInfo.m_dwPreferenceJobMask;

			if	(dwOldMask	&	dwMask)
				dwOldMask	-=	dwMask;
			else
				dwOldMask	+=	dwMask;

			s_partyInfo.sendChangPreferenceJobMask(dwOldMask);
		}
	}

	if	(!s_isTouchLeftButton)
		return;

	if	(g_lpHero->isPartyMember())
	{
		iX		=	m_posPartyWindow.x+9;
		iY		=	m_posPartyWindow.y+6;

		for (i=0;i<s_partyInfo.m_wMemberCount;i++)
		{
			cRECT	rectBar;

			rectBar.set(iX,iY,iX+iBarWidth,iY+iBarHeight);

			if	(rectBar.isIn(s_posMouse.x,s_posMouse.y))
				s_partyInfo.m_wSelectMember	=	i;

			iY	+=	iBarHeight;
		}
	}
	else
	{
		s_partyInfo.m_iSelectParty	=	s_partyInfo.m_iFocusParty;
	}
}

BOOL
CPartyWindow::isIn()
{
	if	(s_iPopupInterface	!=	ePIW_PARTY)
		return	FALSE;

	if (s_rectInterfaceSize.isIn(s_posMouse.x, s_posMouse.y))

		return	TRUE;

	return	isInRequestJoinPartyUserList();
}

