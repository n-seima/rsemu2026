#include "CGamePlay.H"
#include "cMAP.H"
#include "CActor.H"
#include "CMessage.H"
#include "CWindowInterface.H"
#include "dialog_box.H"

cRECT		CGamePlay::s_rectMinimapFrame;
cRECT		CGamePlay::s_rectMapFrame;
cRECT		CGamePlay::s_rectTitle;
cRECT		CGamePlay::s_rectBorderFrame;

cBARMENU	CGamePlay::s_bmMinimap;
cBARMENU	CGamePlay::s_bmAddMinimap;

#define		dMINIMAP_SCALE_STEP_COUNT				6
#define		dMINIMAP_BAR_FRAME_WIDTH				120

CPos		l_aMMWSize[dMINIMAP_SCALE_STEP_COUNT]	=	{/*120*/170,70,/*120*/170,80,/*160*/170,100,200,120,240,dMINIMAP_BAR_FRAME_WIDTH,280,160};

#define		dMINIMAP_FRAME_GAP_WITH_IMAGE	1

//
//	미니맵 초기화
void
CGamePlay::InitMinimap()
{
	g_map.m_bOptionState = FALSE;
	g_map.m_bFogmapState = FALSE;
	g_map.m_bQuestListState = FALSE;
	g_map.m_iQuestListCloseBtnState = eMM_CLOSEBTN_NORMAL;
	g_map.m_iSelectQuest = dMAX_PROCESS_QUEST_COUNT;


	if (g_config.m_iMinimapWindowScale	>=	dMINIMAP_SCALE_STEP_COUNT)
		g_config.m_iMinimapWindowScale=0;

	s_rectMinimapFrame.x1	=	0xffff;

	s_bmMinimap.init(&g_sprInterface,5);//

	s_bmMinimap.addImageBar("",eMMTB_ZOOM_PLUS		,0,0,eMMI_RESIZE_WINDOW_NORMAL	,eMMI_RESIZE_WINDOW_LIGHT	,eMMI_RESIZE_WINDOW_PRESSED	,0xffff,0xffff);
	s_bmMinimap.addImageBar("",eMMTB_ZOOM_MINUS		,0,0,eMMI_RESIZE_WINDOW_NORMAL	,eMMI_RESIZE_WINDOW_LIGHT	,eMMI_RESIZE_WINDOW_PRESSED	,0xffff,0xffff);
	s_bmMinimap.addImageBar("",eMMTB_TOGGLE			,0,0,eMMI_CLOSE_NORMAL			,eMMI_CLOSE_LIGHT			,eMMI_CLOSE_PRESSED			,0xffff,0xffff);
	s_bmMinimap.addImageBar("",eMMTB_QUESTION_MARK	,0,0,eMMI_QUESTION_MARK_NORMAL	,eMMI_QUESTION_MARK_LIGHT	,eMMI_QUESTION_MARK_PRESSED	,0xffff,0xffff);
	s_bmMinimap.addImageBar("",eMMTB_WORLD_MAP		,0,0,eMMI_WORLD_MAP_NORMAL		,eMMI_WORLD_MAP_LIGHT		,eMMI_WORLD_MAP_PRESSED		,0xffff,0xffff);

	g_map.m_iFogmapAlpha = 20;
	g_map.m_iRBtnClickTime = 0;
	g_map.m_bMoveFogmap = FALSE;

	s_bmAddMinimap.init(&g_sprInterface2, 3);
	s_bmAddMinimap.addImageBar("",eMMTB_QUESTLIST		,0,0,eMMI_QUESTLIST_NORMAL		,eMMI_QUESTLIST_LIGHT		,eMMI_QUESTLIST_PRESSED		,0xffff,0xffff);
	s_bmAddMinimap.addImageBar("",eMMTB_FOGMAP			,0,0,eMMI_FOGMAP_NORMAL			,eMMI_FOGMAP_LIGHT			,eMMI_FOGMAP_PRESSED		,0xffff,0xffff);
	s_bmAddMinimap.addImageBar("",eMMTB_OPTION			,0,0,eMMI_MINIMAP_OPTION_NORMAL	,eMMI_MINIMAP_OPTION_LIGHT	,eMMI_MINIMAP_OPTION_PRESSED,0xffff,0xffff);

	g_map.m_dwResultOptionwindow = 0xffffffff;
	int iCheckBoxHeight = g_sprInterface.getSpriteHeight(eSMWI_CHECK_BOX)+1;
	g_map.m_bmCheckBox.init(&g_sprInterface, 3, dBAR_CHECK, eBTOM_BAR_RIGHT);

	g_map.m_bmCheckBox.addImageBar("Quest NPC",		eMMOPTION_QUEST_NPC,	 0, 0,					eSMWI_CHECK_BOX, eSMWI_CHECK_BOX, eSMWI_CHECK_BOX, eSMWI_CHECK_BOX_CHECKED);
	g_map.m_bmCheckBox.addImageBar("Quest FIELD",	eMMOPTION_QUEST_FIELD,	 0, iCheckBoxHeight,	eSMWI_CHECK_BOX, eSMWI_CHECK_BOX, eSMWI_CHECK_BOX, eSMWI_CHECK_BOX_CHECKED);
	g_map.m_bmCheckBox.addImageBar("Quest MONSTER", eMMOPTION_QUEST_MONSTER, 0, iCheckBoxHeight*2,  eSMWI_CHECK_BOX, eSMWI_CHECK_BOX, eSMWI_CHECK_BOX, eSMWI_CHECK_BOX_CHECKED);
	

	g_map.m_bmCheckBox.check(eMMOPTION_QUEST_NPC, TRUE);
	g_map.m_bmCheckBox.check(eMMOPTION_QUEST_FIELD, TRUE);
	g_map.m_bmCheckBox.check(eMMOPTION_QUEST_MONSTER, TRUE);

	g_map.m_iFocusQuest = 0xffff;
}

BOOL
CGamePlay::GetMMCPosToReal(CPos *_lpPos)
{
	if (s_iPopupInterface)
		return	FALSE;

	if (!s_rectMapFrame.isIn(s_posMouse.x,s_posMouse.y))
		return	FALSE;

	if(g_config.m_iMinimapWindowScale	>=	100)
		return	FALSE;

	if	(g_hero.getTitleLevel(eTN_CARTOGRAPHER)	<	s_iMinimapDisplayLevel)
		return	FALSE;

	return	g_map.getMMCPosToReal(_lpPos);
}

//
//	미니맵 그리기
void
CGamePlay::DrawMinimap()
{
	if	(s_iPopupInterface && s_iPopupInterface	!=	ePIW_HELP_SCREEN)
		return;

	if	(g_dialogBox.isTalking())
		return;
#ifdef	_DEBUG

	if	(g_bIsMovieMode)
		return;
#endif
	int		iGap				=	dMINIMAP_FRAME_GAP_WITH_IMAGE;
	int		iCartographerLevel	=	g_hero.getTitleLevel(eTN_CARTOGRAPHER);
	CTitle	*lpTitle			=	g_titleManager.getTitle(eTN_CARTOGRAPHER);
	BOOL	bIsViewMinimap		=	TRUE;

	if	(iCartographerLevel	<	s_iMinimapDisplayLevel)
		bIsViewMinimap		=	FALSE;

// insu add	

	if(g_map.m_bFogmapState)
		g_map.DrawFogMap();
// insu add end

	if (g_map.m_bQuestListState)
	{
		g_map.drawQuestList();

		if(g_map.m_iFocusQuest != 0xffff && g_map.m_iFocusQuest != dMAX_PROCESS_QUEST_COUNT)
			g_map.drawQuestListDetail(g_map.m_iFocusQuest, g_map.m_QuestListArea[g_map.m_iFocusQuest].x1, g_map.m_QuestListArea[g_map.m_iFocusQuest].y2+2);
	}

	if	(g_config.m_iMinimapWindowScale	<	100)
	{
		BOOL	bIsHalfBlending	=	TRUE;

		if (s_rectMapFrame.isIn(s_posMouse.x,s_posMouse.y))	
			bIsHalfBlending	=	FALSE;

		switch(g_config.m_iMinimapOutputMethod)
		{
			case	0	:	bIsHalfBlending	=	FALSE;
				break;
			case	1	:	bIsHalfBlending	=	TRUE;
				break;
			case	3	:	bIsHalfBlending	=	1-bIsHalfBlending;
				break;
		}

		if (bIsViewMinimap && g_map.drawMinimap(bIsHalfBlending,bIsViewMinimap))
		{
			cTGA	*lpMinmap	=	&g_map.m_tgaMinimap;
			CPos	posScaleBar;
			SIZE	sizeScaleBar;

			cDRAW::Fill(0,s_rectMapFrame.x1-iGap,s_rectMapFrame.y1-iGap,s_rectMapFrame.x2+iGap,s_rectMapFrame.y1);
			cDRAW::Fill(0,s_rectMapFrame.x1-iGap,s_rectMapFrame.y2,s_rectMapFrame.x2+iGap,s_rectMapFrame.y2+iGap);
			cDRAW::Fill(0,s_rectMapFrame.x1-iGap,s_rectMapFrame.y1-iGap,s_rectMapFrame.x1,s_rectMapFrame.y2+iGap);
			cDRAW::Fill(0,s_rectMapFrame.x2,s_rectMapFrame.y1-iGap,s_rectMapFrame.x2+iGap,s_rectMapFrame.y2+iGap);

			sizeScaleBar.cx	=	s_rectMapFrame.getWidth()*s_rectMapFrame.getWidth()/lpMinmap->m_iWidth;
			sizeScaleBar.cy	=	s_rectMapFrame.getHeight()*s_rectMapFrame.getHeight()/lpMinmap->m_iHeight;

			CPos	posFocus;

			g_am.getFocusPos(&posFocus);

			if (sizeScaleBar.cx	<	lpMinmap->m_iWidth)
			{
				posScaleBar.x	=	posFocus.x*(s_rectMapFrame.getWidth()-sizeScaleBar.cx)/g_map.m_iPixelWidth;
				cDRAW::XLine(_LTGREEN,s_rectMapFrame.x1 + posScaleBar.x,s_rectMapFrame.x1 + posScaleBar.x+sizeScaleBar.cx,s_rectMapFrame.y1-iGap);
			}

			if (sizeScaleBar.cy	<	lpMinmap->m_iHeight)
			{
				posScaleBar.y	=	posFocus.y*(s_rectMapFrame.getHeight()-sizeScaleBar.cy)/g_map.m_iPixelHeight;
				cDRAW::YLine(_LTGREEN,s_rectMapFrame.y1 + posScaleBar.y,s_rectMapFrame.y1 + posScaleBar.y+sizeScaleBar.cy,s_rectMapFrame.x1-iGap);
			}

			s_ttCommon.drawFrame(&s_rectBorderFrame,FALSE);
		}
	}

	s_rectTitle.x1	=	min(620+g_iScreenWidth-800,s_rectTitle.x1);

#if	defined(_DEBUG)	||	defined(_FOR_USA)
	if	(s_bIsObserverMode	==	FALSE)
	{
		cRECT	rect;

		memcpy(&rect,&s_rectTitle,sizeof(cRECT));

		rect.x2	=	rect.x1-2;
		rect.x1	-=	94;

		s_ttCommon.drawBox(&rect,0,16);
	}
#endif
	s_ttCommon.drawBox(&s_rectTitle,0,16);

	s_bmMinimap.draw();
	// insu add
	s_bmAddMinimap.draw();
	// insu add end	
	int		iXCoordinateImageWidth	=	g_sprInterface.getSpriteWidth(eMMI_ZOOM_PLUS_NORMAL);
	CPos	posCoordinate;

	posCoordinate.x	=	s_rectTitle.x1+s_ttCommon.m_iLeftBorderWidth;
	posCoordinate.y	=	s_rectTitle.y1+s_ttCommon.m_iTopBorderHeight;

#if	defined(_DEBUG)	||	defined(_FOR_USA)
	if	(s_bIsObserverMode	==	FALSE)
		s_text.put(posCoordinate.x+2-94,posCoordinate.y+2,WHITE,"%.2d/%.2d/%.2d %.2d:%.2d",
					s_currentTime.m_wYear-2000,s_currentTime.m_wMonth,s_currentTime.m_wDay,s_currentTime.m_wHour,s_currentTime.m_wMinute);

#endif

	s_text.put(posCoordinate.x+2,posCoordinate.y+2,WHITE,"%d,%d",g_lpHero->m_pos.mx,g_lpHero->m_pos.my);
	
	if	(g_config.m_iMinimapWindowScale	<	100 && bIsViewMinimap)
		g_map.drawEventNpcToolTip();

//	if (g_config.m_iMinimapWindowScale<100 && g_config.m_iMinimapWindowScale>0)
//		s_text.put(g_iScreenWidth-iXCoordinateImageWidth*4-2-8,posCoordinate.y+2,WHITE,"x%d",g_config.m_iMinimapWindowScale+1);

	if (g_map.m_bOptionState)
		g_map.drawMinimapOption();
}

//
//
void
CGamePlay::BuildMinimapFrameInfo()
{
	int		iGap			=	dMINIMAP_FRAME_GAP_WITH_IMAGE;
	int		iTitleBarHeight	=	20;

	if (s_rectMinimapFrame.getWidth() > (DWORD)g_map.m_tgaMinimap.m_iWidth)	
		s_rectMinimapFrame.x1	+=	(s_rectMinimapFrame.getWidth() -g_map.m_tgaMinimap.m_iWidth);
	if (s_rectMinimapFrame.getHeight()> (DWORD)g_map.m_tgaMinimap.m_iHeight)
		s_rectMinimapFrame.y2	-=	(s_rectMinimapFrame.getHeight()-g_map.m_tgaMinimap.m_iHeight);

	s_rectMinimapFrame.x1	-=	(s_ttCommon.m_iLeftBorderWidth+iGap);
	s_rectMinimapFrame.y1	-=	(s_ttCommon.m_iLeftBorderWidth+iGap);
	s_rectMinimapFrame.x2	+=	(s_ttCommon.m_iRightBorderWidth+iGap-1);
	s_rectMinimapFrame.y2	+=	(s_ttCommon.m_iBottomBorderHeight+iGap-1)+iTitleBarHeight;

	if (s_rectMinimapFrame.x1	<	0				)	
		s_rectMinimapFrame.add(-s_rectMinimapFrame.x1,0);
	if (s_rectMinimapFrame.x2	>=	g_iScreenWidth	)	
		s_rectMinimapFrame.add(-(s_rectMinimapFrame.x2-g_iScreenWidth+1),0);
	if (s_rectMinimapFrame.y1	<	0				)	
		s_rectMinimapFrame.add(0,-s_rectMinimapFrame.y1);
	if (s_rectMinimapFrame.y2	>=	g_iScreenHeight	)	
		s_rectMinimapFrame.add(-(s_rectMinimapFrame.y2-g_iScreenHeight+1),0);

	memcpy(&s_rectTitle,&s_rectMinimapFrame,sizeof(cRECT));

	s_rectTitle.y2		=	s_rectTitle.y1	+	iTitleBarHeight;
	memcpy(&s_rectBorderFrame,&s_rectMinimapFrame,sizeof(cRECT));
	s_rectBorderFrame.y1	+=	iTitleBarHeight;
	
	memcpy(&s_rectMapFrame,&s_rectBorderFrame,sizeof(cRECT));

	s_rectMapFrame.x1	+=	(s_ttCommon.m_iLeftBorderWidth+iGap);
	s_rectMapFrame.y1	+=	(s_ttCommon.m_iTopBorderHeight+iGap);
	s_rectMapFrame.x2	-=	(s_ttCommon.m_iRightBorderWidth+iGap-1);
	s_rectMapFrame.y2	-=	(s_ttCommon.m_iBottomBorderHeight+iGap-1);

	CPos	posButton;
	int		iWindowSizeButtonWidth	=	g_sprInterface.getSpriteWidth(eMMI_RESIZE_WINDOW_NORMAL);
	int		iCloseButtonWidth		=	g_sprInterface.getSpriteWidth(eMMI_CLOSE_NORMAL);
	int		iMenuCount				=	3;

	iMenuCount++;
	posButton.x						=	s_rectTitle.x2-(iWindowSizeButtonWidth*iMenuCount+iCloseButtonWidth+s_ttCommon.m_iRightBorderWidth)+1;
	posButton.y						=	s_rectTitle.y1+s_ttCommon.m_iTopBorderHeight;

	s_bmAddMinimap.resetImageBar(	eMMTB_QUESTLIST		,"",eMMTB_QUESTLIST		,posButton.x-(iWindowSizeButtonWidth*2),posButton.y,eMMI_QUESTLIST_NORMAL,eMMI_QUESTLIST_LIGHT,eMMI_QUESTLIST_PRESSED);
	s_bmAddMinimap.resetImageBar(	eMMTB_FOGMAP		,"",eMMTB_FOGMAP		,posButton.x-(iWindowSizeButtonWidth*3),posButton.y,eMMI_FOGMAP_NORMAL,eMMI_FOGMAP_LIGHT,eMMI_FOGMAP_PRESSED);
	s_bmAddMinimap.resetImageBar(	eMMTB_OPTION		,"",eMMTB_OPTION		,posButton.x+(iWindowSizeButtonWidth*0),posButton.y,eMMI_MINIMAP_OPTION_NORMAL,eMMI_MINIMAP_OPTION_LIGHT,eMMI_MINIMAP_OPTION_PRESSED);

	s_bmMinimap.resetImageBar(	eMMTB_WORLD_MAP		,"",eMMTB_WORLD_MAP		,posButton.x-iWindowSizeButtonWidth*1,posButton.y,eMMI_WORLD_MAP_NORMAL,eMMI_WORLD_MAP_LIGHT,eMMI_WORLD_MAP_PRESSED);
	s_bmMinimap.resetImageBar(	eMMTB_QUESTION_MARK	,"",eMMTB_QUESTION_MARK	,posButton.x+iWindowSizeButtonWidth*1,posButton.y,eMMI_QUESTION_MARK_NORMAL,eMMI_QUESTION_MARK_LIGHT,eMMI_QUESTION_MARK_PRESSED);
	s_bmMinimap.resetImageBar(	eMMTB_ZOOM_PLUS		,"",eMMTB_ZOOM_PLUS		,posButton.x+iWindowSizeButtonWidth*2,posButton.y,eMMI_ZOOM_PLUS_NORMAL,eMMI_ZOOM_PLUS_LIGHT,eMMI_ZOOM_PLUS_PRESSED);
	s_bmMinimap.resetImageBar(	eMMTB_ZOOM_MINUS	,"",eMMTB_ZOOM_MINUS	,posButton.x+iWindowSizeButtonWidth*3,posButton.y,eMMI_ZOOM_MINUS_NORMAL,eMMI_ZOOM_MINUS_LIGHT,eMMI_ZOOM_MINUS_PRESSED);
	
	if (g_config.m_iMinimapWindowScale	<	100)
		s_bmMinimap.resetImageBar(	eMMTB_TOGGLE		,"",eMMTB_TOGGLE		,posButton.x+iWindowSizeButtonWidth*4,posButton.y,eMMI_CLOSE_NORMAL,eMMI_CLOSE_LIGHT,eMMI_CLOSE_PRESSED);
	else
		s_bmMinimap.resetImageBar(	eMMTB_TOGGLE		,"",eMMTB_TOGGLE		,posButton.x+iWindowSizeButtonWidth*4,posButton.y,eMMI_RESIZE_WINDOW_NORMAL,eMMI_RESIZE_WINDOW_LIGHT,eMMI_RESIZE_WINDOW_PRESSED);

	int iPutPosX=s_rectMapFrame.x1, iPutPosY=s_rectMapFrame.y1;
	iPutPosX -= (g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)+5);
	iPutPosY -= 2;

#ifndef _DEBUG
	iPutPosX = s_rectTitle.x1-(g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)+10);
	iPutPosY = 0;
#endif

	if (g_map.m_tgaMinimap.m_p16IMAGE == NULL)
	{
		iPutPosX = s_rectTitle.x2-g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD);
		iPutPosY = s_rectTitle.y2+2;
	}

	for(int i=1 ; i<=dMAX_PROCESS_QUEST_COUNT ; i++)
	{
		g_map.m_QuestListArea[i-1].set(iPutPosX, 
									iPutPosY+g_sprInterface.getSpriteHeight(eWINDOW_TITLE_QUEST_OLD)*i, 
									iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD),
									iPutPosY+g_sprInterface.getSpriteHeight(eWINDOW_TITLE_QUEST_OLD)*i+g_sprInterface.getSpriteHeight(eWINDOW_TITLE_QUEST_OLD));
	}
	UpdateMinimapQuestList();
	g_map.FindQuestLinkField();
	g_map.m_QuestListArea[dMAX_PROCESS_QUEST_COUNT].set(g_map.m_QuestListArea[dMAX_PROCESS_QUEST_COUNT-1].x1,
														g_map.m_QuestListArea[dMAX_PROCESS_QUEST_COUNT-1].y2,
														g_map.m_QuestListArea[dMAX_PROCESS_QUEST_COUNT-1].x2,
														g_map.m_QuestListArea[dMAX_PROCESS_QUEST_COUNT-1].y2+g_sprInterface.getSpriteHeight(eWINDOW_TITLE_QUEST_OLD));

	g_map.m_rectQuestListCloseBtn.set(	iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)-g_sprInterface.getSpriteWidth(eWINDOW_CLOSE_ACTIVE),
										iPutPosY,
										iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD),										
										iPutPosY+g_sprInterface.getSpriteHeight(eWINDOW_TITLE_QUEST_OLD));
// insu add & modify end

	g_map.updateMinimap(&s_rectMapFrame,&g_am.m_posFocus);
	g_map.setQuestLinkFieldIndex();
}


void
CGamePlay::UpdateMinimapQuestList()
{	
	// 몇번째 슬롯의 퀘스트인지 확인한다.
	int iCnt = 0;
	for(int i=0 ; i<dMAX_PROCESS_QUEST_COUNT ; i++)
		g_map.m_iQuestIndex[i] = 0xffff;
	
	for(i=0 ; i<dMAX_PROCESS_QUEST_COUNT ; i++)
	{		
		if (g_hero.m_aProcessQuest[i].isActive == TRUE)
		{
			g_map.m_iQuestIndex[iCnt] = i;
			iCnt++;
		}
	}

	for( i=iCnt; i<dMAX_PROCESS_QUEST_COUNT ; i++)
		g_map.m_iQuestIndex[i] = 0xffff;
}

//
//	미니맵 업데이트
void
CGamePlay::UpdateMinimap()
{
	if (s_iPopupInterface				)
		return;

	g_map.updateMinimap(&s_rectMapFrame,&g_am.m_posFocus);

	int	iMaxMinimapSize				=	dMINIMAP_SCALE_STEP_COUNT;

	if (s_iMinimapDisplayLevel)
	{
		int		iCartographerLevel	=	g_hero.getTitleLevel(eTN_CARTOGRAPHER);
		CTitle	*lpTitle			=	g_titleManager.getTitle(eTN_CARTOGRAPHER);

		if (lpTitle)
		{
			int	iCurrentMinimapWindowScale	=	g_config.m_iMinimapWindowScale;
			int	iMinimapScale				=	abs(g_config.m_iMinimapWindowScale);

			if (iCartographerLevel)
			{
				iMaxMinimapSize	=	lpTitle->m_aEffect[0].m_aiValue[iCartographerLevel-1][1];
				iMinimapScale	=	min(iMinimapScale,iMaxMinimapSize-1);
			}
			else
			{
				iMaxMinimapSize	=	0;
				iMinimapScale	=	0;
			}

			if (g_config.m_iMinimapWindowScale	>=	100)
				iMinimapScale	+=	100;

			g_config.m_iMinimapWindowScale	=	iMinimapScale;

			if (iCurrentMinimapWindowScale	!=	g_config.m_iMinimapWindowScale)
			{
				int	iSize	=	g_config.m_iMinimapWindowScale;

				if (iSize>=100)	
					s_rectMinimapFrame.set(g_iScreenWidth-dMINIMAP_BAR_FRAME_WIDTH,0,g_iScreenWidth,l_aMMWSize[0].y);
				else
					s_rectMinimapFrame.set(g_iScreenWidth-l_aMMWSize[iSize].x,0,g_iScreenWidth,l_aMMWSize[iSize].y);

				BuildMinimapFrameInfo();
			}
		}

		cRECT	rect;

		rect.set(&s_rectMinimapFrame);

		rect.y2	=	20;

		if (s_iMinimapDisplayLevel > iCartographerLevel && rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			rect.add(0,20);
			char	strText[256];

			sprintf(strText,dMSG_REQUIRE_CARTOGRAPHER_LEVEL_FORM,s_iMinimapDisplayLevel);

			s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strText,eTFM_RIGHT);
		}
	}


	if (s_rectMinimapFrame.x1	==	0xffff)
	{
		int	iSize	=	g_config.m_iMinimapWindowScale;
		if (iSize>=100)	s_rectMinimapFrame.set(g_iScreenWidth-dMINIMAP_BAR_FRAME_WIDTH,0,g_iScreenWidth,l_aMMWSize[0].y);
		else			s_rectMinimapFrame.set(g_iScreenWidth-l_aMMWSize[iSize].x,0,g_iScreenWidth,l_aMMWSize[iSize].y);

		BuildMinimapFrameInfo();
		// 포그맵 출력좌표와 넓이를 계산한다.
		// insu add modify
		if (g_map.m_tgaMinimap.m_p16IMAGE != NULL)
		{
			g_map.m_iPutPosX = g_iScreenWidth/2 - g_map.m_tgaMinimap.m_p16IMAGE[0]/2;
			g_map.m_iPutPosY = g_iScreenHeight/2 - g_map.m_tgaMinimap.m_p16IMAGE[1]/2;
		
			g_map.m_iFogmapWidth = g_map.m_tgaMinimap.m_p16IMAGE[0];
			g_map.m_iFogmapHeight = g_map.m_tgaMinimap.m_p16IMAGE[1];
		}
		//i nsu modify end
	}

	g_map.m_bmCheckBox.update(s_posMouse.x, s_posMouse.y, s_aKey, s_isTouchLeftButton, s_isLeftButtonPressed);
	
	g_map.m_dwResultOptionwindow = 0xffffffff;

	if(g_map.m_bOptionState == TRUE)
		if (g_map.m_rtOptionWnd.isIn(s_posMouse.x, s_posMouse.y))
			g_map.m_dwResultOptionwindow = 1;
	
	if ( g_map.m_dwResultOptionwindow == 1 )
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_OPTION);	
	}
	if (cINPDEV::GetExclusive() == eMOUSE_STATE_OPTION && !g_map.m_rtOptionWnd.isIn(s_posMouse.x, s_posMouse.y))
	{
		cINPDEV::ReleaseExclusive();	
	}	

	if (cINPDEV::IsExclusive(eMOUSE_STATE_MINIMAP))
		return;

// insu add
	if(s_dwResultPopupwindow == 0xffffffff)
		s_dwResultPopupwindow	=	s_bmAddMinimap.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(s_dwResultPopupwindow)
	{
		case eMMTB_QUESTLIST:
			g_map.m_bQuestListState = (g_map.m_bQuestListState)? FALSE : TRUE;		
			s_dwResultPopupwindow = 0xffffffff;
			break;
		case eMMTB_FOGMAP:
			g_map.m_bFogmapState = (g_map.m_bFogmapState)? FALSE : TRUE;
			s_dwResultPopupwindow = 0xffffffff;
			break;
		case eMMTB_OPTION:
			g_map.m_bOptionState = (g_map.m_bOptionState)? FALSE : TRUE;
			g_map.m_rtOptionWnd.set(0xffff, 0xffff, 0xffff, 0xffff);
			break;
	}
// insu add end
	if (g_map.m_bQuestListState)
		s_bmAddMinimap.setImageBar(eMMTB_QUESTLIST, eMMI_QUESTLIST_PRESSED, eMMI_QUESTLIST_PRESSED, eMMI_QUESTLIST_PRESSED);
	if (!g_map.m_bQuestListState)
		s_bmAddMinimap.setImageBar(eMMTB_QUESTLIST, eMMI_QUESTLIST_NORMAL,eMMI_QUESTLIST_LIGHT,eMMI_QUESTLIST_PRESSED);

	if (g_map.m_bFogmapState)
		s_bmAddMinimap.setImageBar(eMMTB_FOGMAP, eMMI_FOGMAP_PRESSED, eMMI_FOGMAP_PRESSED, eMMI_FOGMAP_PRESSED);
	if (!g_map.m_bFogmapState)
		s_bmAddMinimap.setImageBar(eMMTB_FOGMAP, eMMI_FOGMAP_NORMAL,eMMI_FOGMAP_LIGHT,eMMI_FOGMAP_PRESSED);

	if (g_map.m_bOptionState)
		s_bmAddMinimap.setImageBar(eMMTB_OPTION, eMMI_MINIMAP_OPTION_PRESSED, eMMI_MINIMAP_OPTION_PRESSED, eMMI_MINIMAP_OPTION_PRESSED);
	if (!g_map.m_bOptionState)
		s_bmAddMinimap.setImageBar(eMMTB_OPTION, eMMI_MINIMAP_OPTION_NORMAL,eMMI_MINIMAP_OPTION_LIGHT,eMMI_MINIMAP_OPTION_PRESSED);


	if(s_dwResultPopupwindow == 0xffffffff)
		s_dwResultPopupwindow	=	s_bmMinimap.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	if (s_bmMinimap.isPressed() || s_bmAddMinimap.isPressed())		cINPDEV::SetExclusive(eMOUSE_STATE_MINIMAP);
	else							cINPDEV::ReleaseExclusive();

	switch(s_dwResultPopupwindow)
	{
		case	eMMTB_ZOOM_PLUS			:
			if (g_config.m_iMinimapWindowScale	<	100)
			{
				g_config.m_iMinimapWindowScale++;

				if (g_config.m_iMinimapWindowScale	>=	dMINIMAP_SCALE_STEP_COUNT)	g_config.m_iMinimapWindowScale	=	0;

				if (g_config.m_iMinimapWindowScale	>=	iMaxMinimapSize)
				{
					g_config.m_iMinimapWindowScale--;
					g_msgBox.cPopup("",dMSG_CAN_NOT_INCREASE_MINIMAP_SIZE_REQUIRE_MORE_HIGH_CARTOGRAPHER_TITLE,dMSG_OK);
					break;
				}
			}
			else	g_config.m_iMinimapWindowScale	-=	100;

			s_rectMinimapFrame.set(g_iScreenWidth-l_aMMWSize[g_config.m_iMinimapWindowScale].x,0,g_iScreenWidth,l_aMMWSize[g_config.m_iMinimapWindowScale].y);

			BuildMinimapFrameInfo();
			break;

		case	eMMTB_ZOOM_MINUS		:
			if (g_config.m_iMinimapWindowScale	<	100)
			{
				g_config.m_iMinimapWindowScale--;

				if (g_config.m_iMinimapWindowScale	<	0)
					g_config.m_iMinimapWindowScale	=	iMaxMinimapSize-1;
			}
			else	g_config.m_iMinimapWindowScale	-=	100;

			s_rectMinimapFrame.set(g_iScreenWidth-l_aMMWSize[g_config.m_iMinimapWindowScale].x,0,g_iScreenWidth,l_aMMWSize[g_config.m_iMinimapWindowScale].y);

			BuildMinimapFrameInfo();
			break;

		case	eMMTB_TOGGLE			:
			if (g_config.m_iMinimapWindowScale	>=	100)
			{
				g_config.m_iMinimapWindowScale	-=	100;
				s_rectMinimapFrame.set(g_iScreenWidth-l_aMMWSize[g_config.m_iMinimapWindowScale].x,0,g_iScreenWidth,l_aMMWSize[g_config.m_iMinimapWindowScale].y);
				BuildMinimapFrameInfo();
				break;
			}

			g_config.m_iMinimapWindowScale	+=	100;
			s_rectMinimapFrame.set(g_iScreenWidth-dMINIMAP_BAR_FRAME_WIDTH,0,g_iScreenWidth,l_aMMWSize[0].y);
			BuildMinimapFrameInfo();
			break;

		case	eMMTB_QUESTION_MARK		:
		{
#ifdef _DEBUG
			if	(s_bIsGuildHall)
				s_fieldTitle.init(_ms("Guild Hall : %s[%.4d]",s_strGuildHallOwnerGuildName,s_iFieldSerial));
			else
				s_fieldTitle.init(_ms("%s[%.4d]",g_map.m_strName,s_iFieldSerial));
#else
			if	(s_bIsGuildHall)
				s_fieldTitle.init(_ms("Guild Hall : %s",s_strGuildHallOwnerGuildName));
			else
				s_fieldTitle.init(g_map.m_strName);
#endif
			break;
		}
		
		case	eMMTB_WORLD_MAP			:
			{
				g_gwWorldMap.open();
				break;
			}
		case eMMTB_ZOOM_MAX:			// 09.06.25 test 미니맵 최대
			{
				g_config.m_iMinimapWindowScale = 5;
				s_rectMinimapFrame.set(g_iScreenWidth-l_aMMWSize[g_config.m_iMinimapWindowScale].x,0,g_iScreenWidth,l_aMMWSize[g_config.m_iMinimapWindowScale].y);
				BuildMinimapFrameInfo();
			}
			break;
		case eMMTB_ZOOM_MIN:			// 09.06.25 test 미니맵 최소
			{
				g_config.m_iMinimapWindowScale = 0;
				s_rectMinimapFrame.set(g_iScreenWidth-l_aMMWSize[g_config.m_iMinimapWindowScale].x,0,g_iScreenWidth,l_aMMWSize[g_config.m_iMinimapWindowScale].y);
				BuildMinimapFrameInfo();
			}
			
			break;
	}

	s_dwResultPopupwindow = 0xffffffff;

	if (g_map.m_bQuestListState)
	{
		g_map.m_iFocusQuest = 0xffff;
		for(int i=0 ; i<dMAX_PROCESS_QUEST_COUNT+1 ; i++)
		{
			if (g_map.m_QuestListArea[i].isIn(s_posMouse.x,s_posMouse.y))
			{
				g_map.m_iFocusQuest = i;
				if (s_isClickedLeftButton)
				{
					g_map.m_iSelectQuest = g_map.m_iFocusQuest;
					break;
				}
			}
		}
		
		if (s_isClickedLeftButton)
		{
			if (g_map.m_iQuestListCloseBtnState == eMM_CLOSEBTN_DOWN && g_map.m_bQuestListState == TRUE)
				g_map.m_bQuestListState = FALSE;
		}
	}

	g_map.m_iQuestListCloseBtnState = eMM_CLOSEBTN_NORMAL;

	if (s_isLeftButtonPressed && g_map.m_rectQuestListCloseBtn.isIn(s_posMouse.x,s_posMouse.y))
	{
		g_map.m_iQuestListCloseBtnState = eMM_CLOSEBTN_DOWN;
		return;
	}

	if (!s_isClickedLeftButton && g_map.m_rectQuestListCloseBtn.isIn(s_posMouse.x,s_posMouse.y))
	{
		g_map.m_iQuestListCloseBtnState = eMM_CLOSEBTN_OVER;
		return;
	}

	if (g_map.m_bFogmapState)
	{
		int iLimitTime = 40;	// 이시간동안 맵이동을 할껀지 결정!
		cRECT	rect;
		rect.set(g_map.m_iPutPosX, g_map.m_iPutPosY, g_map.m_iPutPosX+g_map.m_iFogmapWidth, g_map.m_iPutPosY+g_map.m_iFogmapHeight);

		static CPos pos;
		// 포그맵이 활성화된 상태에서 우클릭 꾸욱!
		if (s_isRightButtonPressed && rect.isIn(s_posMouse.x, s_posMouse.y))
		{
			if (g_map.m_iRBtnClickTime <= iLimitTime)
				g_map.m_iRBtnClickTime++;
		}
		else
			g_map.m_iRBtnClickTime = 0;
	
		if (g_map.m_bMoveFogmap == TRUE && s_isTouchRightButton) // 마우스R을 다시 클릭하면..
		{
			g_map.m_bMoveFogmap = FALSE;
			g_map.m_iFogmapAlpha = 20;
			g_map.m_iRBtnClickTime = 0;

			// 벗어난 위치를 수정한다.
			if (g_map.m_iPutPosX+g_map.m_iFogmapWidth > g_iScreenWidth)
				g_map.m_iPutPosX -= (g_map.m_iPutPosX+g_map.m_iFogmapWidth-g_iScreenWidth+2);
			if (g_map.m_iPutPosX < 0)
				g_map.m_iPutPosX = 2;
			if (g_map.m_iPutPosY+g_map.m_iFogmapHeight > g_iScreenHeight-g_sprGameBottomInterface.getSpriteHeight(0))
				g_map.m_iPutPosY -= (g_map.m_iPutPosY+g_map.m_iFogmapHeight-(g_iScreenHeight-g_sprGameBottomInterface.getSpriteHeight(0)));
			if (g_map.m_iPutPosY < 0)
				g_map.m_iPutPosY = 2;
		}

		if (g_map.m_iRBtnClickTime >= 20 && g_map.m_bMoveFogmap == FALSE)
		{
			g_map.m_iFogmapAlpha = 32;
			g_map.m_iPutPosX += 3;
			g_map.m_iPutPosY += 3;
			g_map.m_bMoveFogmap = TRUE;
			
			pos.x = s_posMouse.x - g_map.m_iPutPosX;
			pos.y = s_posMouse.y - g_map.m_iPutPosY;
		}

		if (g_map.m_bMoveFogmap == TRUE && s_isMovedMouse)
		{
			g_map.m_iPutPosX = s_posMouse.x - pos.x;
			g_map.m_iPutPosY = s_posMouse.y - pos.y;
		}
	}
// insu add end
}