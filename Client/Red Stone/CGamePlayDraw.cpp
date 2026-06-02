#include "CGamePlay.H"
#include "cMAP.H"
#include "CSkill.H"
#include "CHitEffect.H"
#include "effectManager.H"
#include "cSHAKE.H"
#include "CFieldItem.H"
#include "cMESSAGE.H"
#include "CAgent.H"
#include "CHero.H"
#include "CActor.H"
#include "CGamePlayInterface.H"
#include "CBank.H"
#include "CWindowInterface.H"
#include "CShoppingCart.H"
#include "CEvent.H"
#include "fireCracker.h"
#include "bm_event.h"
#include "dialog_box.h"
#include "commonUI.h"
#include "duel.h"
#include "book_MonsterDictionary.h"

WORD	*l_lpReferenceScreen		=	NULL;
int		l_iReferenceScreenSize	=	0;

WORD *
GetReferenceScreen()
{
	int	iScreenSize	=	cDRAW::WIDTH*cDRAW::HEIGHT;

	if	(iScreenSize	<=	0)
		iScreenSize	=	800*600;

	if	(l_iReferenceScreenSize	<	iScreenSize)
	{
		if	(l_lpReferenceScreen)
			delete [] l_lpReferenceScreen;

		l_lpReferenceScreen		=	new WORD [iScreenSize];
		l_iReferenceScreenSize	=	iScreenSize;
	}

	return	l_lpReferenceScreen;
}

enum
{
	eBLIND_STATUS_REFERENCE_IMAGE_OLD			=	55,
	eBLIND_STATUS_REFERENCE_IMAGE_VERY_SMALL	=	55,
 	eBLIND_STATUS_REFERENCE_IMAGE_SMALL			=	187,
 	eBLIND_STATUS_REFERENCE_IMAGE_MIDDLE		=	188,
 	eBLIND_STATUS_REFERENCE_IMAGE_LARGE			=	189,
	eBLIND_STATUS_REFERENCE_IMAGE_VERY_LARGE	=	190,
};

void
CGamePlay::DrawFullScreenInterface()
{
	switch(s_iPopupInterface)
	{
		case	ePIW_WORLD_MAP				:
			g_gwWorldMap.draw();
			break;

		case	ePIW_MAIN_QUEST_EVENT	:
			g_gwMainQuestEvent.draw();
			break;

		case	ePIW_BOOK					:
			g_gwBook.draw();	
			break;
		case	ePIW_BOOK_MONSTER_DICTIONARY:
			g_gwBookMonDic.draw();
			break;
	}
}

void
CGamePlay::DrawInterfaceWindow()
{
	//	팝업 윈도우 그려주기
	if	(s_iPopupInterface	==	ePIW_NONE)
		return;

	switch(s_iPopupInterface)
	{
		case	ePIW_INVENTORY	:
			DrawInventoryWindow();
			break;

		case	ePIW_STATUS		:
			DrawStatusWindow();
			break;
		case	ePIW_COMMON_LIST	:
			g_gwCommonList.draw();
			break;
		case	ePIW_COMMON_UI	:
			g_commonUI.draw();
			break;

		case	ePIW_SKILL				:
			g_gwSkill.draw();
			g_gwEasySkillTree.draw();
			break;

		case	ePIW_QUEST				:
			g_gwQuest.draw();
			break;

		case	ePIW_FRIEND				:
			g_gwFriend.draw();
			break;

		case	ePIW_PARTY				:
			g_gwParty.draw();
			break;

		case	ePIW_REMOVE_PREFIX		:
			g_gwRemovePrefix.draw();
			break;

		case	ePIW_MERGE_ITEM			:
			g_gwMergeItem.draw();
			break;

		case	ePIW_JUDGE_TO_ITEM		:
			g_gwJudgeToItem.draw();
			break;

		case	ePIW_SYSTEM				:
			DrawSystemMenu();
			break;

		case	ePIW_READY_TO_MAKE_GUILD:
			g_gwMakeGuild.draw();
			break;

		case	ePIW_BANK				:
			g_gwBank.draw();
			break;

		case	ePIW_GUILD_INVENTORY	:
			g_gwGuildInventory.draw();
			break;

		case	ePIW_GUILD_FEED_PET_WINDOW	:
			g_gwFeedGuildPet.draw();
			break;

		case	ePIW_PUT_BANNER			:
			g_gwPutBannerWnd.draw();
			break;

		case	ePIW_SHOPPING_CART		:
			g_gwCart.draw();
			break;

		case	ePIW_GUILD				:
			g_gwGuild.draw();
			break;

		case	ePIW_GUILD_BATTLE_SCHEDULER	:
			g_gwGuildBattleScheduler.draw();
			break;

		case	ePIW_GUILD_HALL_LIST		:
			g_gwGuildHallList.draw();
			break;
		case	ePIW_GUILD_HONOR_POINT_LIST	:
			g_gwHonorPointList.draw();
			break;
		case	ePIW_GUILD_RELATED_PLACE_LIST	:
			g_gwGuildRelatedPlaceList.draw();
			break;

		case	ePIW_GUILD_MARK_COMPOSER	:
			g_gwGuildMarkComposer.draw();
			break;

		case	ePIW_SHOP					:
			g_gwShop.draw();	//	상점
			break;

		case	ePIW_CARROT_SHOP			:
			if(g_config.b_bIsNewCarrotShop)
				g_gwCarrotShop.draw();	//	당근 상점
			else
				g_gwCarrotShop.draw_Original();
			break;
		case	ePIW_SELECT_ITEM_IN_PACK	:
			g_gwSelectItemInPack.draw();	//	당근 상점
			break;

		case	ePIW_BOTTOMLESS_BOX			:
			g_gwBottomlessBoxWindow.draw();	
			break;

		case	ePIW_REPAIR_ITEM			:
			g_gwRepairItem.draw();
			break;

		case	ePIW_SELECT_VILLAGE			:
			g_gwSelectVillage.draw();
			break;

#ifdef	_DEBUG
		case	ePIW_DEBUG					:
			g_gwDebug.draw();	//	상점
			break;
#endif
		case	ePIW_HELP_SCREEN			:
			g_gwHelp.draw();
			break;

		case	ePIW_PITCHMAN_SHOP			:
			g_gwPitchmanShop.draw();	//	상점
			break;

		case	ePIW_WORLD_MAP				:
			g_gwWorldMap.draw();
			break;

		case	ePIW_MAIN_QUEST_EVENT	:
			g_gwMainQuestEvent.draw();
			break;
		case	ePIW_UPGRADE_SHOP	:		//  업그레이드 샵 그리기~~+_+ 09.08.25
			g_gwUpgradeShop.draw();
			break;
		case	ePIW_COPY_OR_CUT_PREFIX	:		//  복사 , 잘라내기창  그리기
			g_gwCopyOrCutPrefix.draw();
			break;
	}
}

void
CGamePlay::DrawInfoForGuildBattle()
{
	int	iLength1		=	s_bText.getPixelSize(g_guildBattleInfo.m_strTeamName[0])+5;
	int	iLength2		=	s_bText.getPixelSize(g_guildBattleInfo.m_strTeamName[1])+5;
	int	iLength			=	max(iLength1,iLength2);
	int	iInterfaceWidth	=	iLength+80;

	cRECT	rect;

	rect.set(0,0,iInterfaceWidth+5,2+34);

	s_ttCommon.drawBox(&rect);

	s_bText.putRF(rect.x1+iLength+5,rect.y1+5,WHITE,g_guildBattleInfo.m_strTeamName[0]);
	s_bText.putRF(rect.x1+iLength+5,rect.y1+18,WHITE,g_guildBattleInfo.m_strTeamName[1]);

	s_bText.put	(rect.x1+iLength+5,rect.y1+5,WHITE," %s <c:LTYELLOW>%d<n>",dMSG_WINNING_POINT,g_guildBattleInfo.m_iTeam1KillPoint);
	s_bText.put	(rect.x1+iLength+5,rect.y1+18,WHITE," %s <c:LTYELLOW>%d<n>",dMSG_WINNING_POINT,g_guildBattleInfo.m_iTeam2KillPoint);

	BOOL	bIsDisplayRemainTime	=	FALSE;
	int		iRemainTime	=	g_duelManager.m_uiRemainBattleTime;

	if	(g_bIsDuelServer && g_duelManager.m_uiBeginBattleTime)
		bIsDisplayRemainTime	=	TRUE;
	else
	if	(g_hero.m_wOperatorLevel	>=	4	&&	g_iRemainGuildBattleTime	>	0)
	{
		bIsDisplayRemainTime	=	TRUE;
		iRemainTime				=	g_iRemainGuildBattleTime;
	}

	if	(bIsDisplayRemainTime)
	{
		char	strMessageForm[128],strNumber[32];

		{
			if (iRemainTime	>=	60*60)
				sprintf(strNumber,dMSG_HOUR_MINUTE_SECOND_FORM,iRemainTime/60/60,(iRemainTime/60)%60,iRemainTime%60);
			else
			if (iRemainTime	>=	60)
				sprintf(strNumber,dMSG_MINUTE_SECOND_FORM,iRemainTime/60,iRemainTime%60);
			else
				sprintf(strNumber,dMSG_SECOND_FORM,iRemainTime);

			sprintf(strMessageForm,"%s %s",dMSG_REMAIN_TIME,strNumber);
		}

		{
			int	iX=cDRAW::WIDTH/2,iY=8,iWidth=80,iHeight=18;

			cRECT	rect;

			rect.set(iX-iWidth,iY-5,iX+iWidth,iY+iHeight);
			s_ttCommon.drawBox(&rect);

			s_textBold14M.cPut(iX,iY,WHITE,"%s <c:LTYELLOW>%s<n>",dMSG_REMAIN_TIME,strNumber);
		}
	}
}

void
CGamePlay::DrawInfoForDuelObserver()
{
	int		iRemainTime	=	g_duelManager.m_uiRemainBattleTime;

	if	(g_hero.m_wOperatorLevel	>=	4	&&	g_iRemainGuildBattleTime	>	0)
		iRemainTime				=	g_iRemainGuildBattleTime;

	if	(iRemainTime	==	0 || iRemainTime	==	0xffff)
		return;

	char	strTeam1Text[128],strTeam2Text[128];

	sprintf(strTeam1Text,"<c:32,160,224>%s<n> %s <c:LTYELLOW>%.2d<n>",g_guildBattleInfo.m_strTeamName[0],dMSG_WINNING_POINT,g_guildBattleInfo.m_iTeam1KillPoint);
	sprintf(strTeam2Text,"<c:216,56,56>%s<n> %s <c:LTYELLOW>%.2d<n>",g_guildBattleInfo.m_strTeamName[1],dMSG_WINNING_POINT,g_guildBattleInfo.m_iTeam2KillPoint);

	int	iLength1		=	s_bText.getPixelSize(strTeam1Text)+5;
	int	iLength2		=	s_bText.getPixelSize(strTeam2Text)+5;
	int	iLength			=	max(iLength1,iLength2);
	int	iInterfaceWidth	=	iLength+80;

	char	strMessageForm[128],strNumber[32];

	{
		if (iRemainTime	>=	60*60)
			sprintf(strNumber,dMSG_HOUR_MINUTE_SECOND_FORM,iRemainTime/60/60,(iRemainTime/60)%60,iRemainTime%60);
		else
		if (iRemainTime	>=	60)
			sprintf(strNumber,dMSG_MINUTE_SECOND_FORM,iRemainTime/60,iRemainTime%60);
		else
			sprintf(strNumber,dMSG_SECOND_FORM,iRemainTime);

		sprintf(strMessageForm,"%s %s",dMSG_REMAIN_TIME,strNumber);
	}

	{
		int	iX	=	cDRAW::WIDTH/2,iY=8,iWidth=80,iHeight=48;
		iWidth	=	max(iWidth,iInterfaceWidth/2);

		cRECT	rect;

		rect.set(iX-iWidth,iY-5,iX+iWidth,iY+iHeight);
		s_ttCommon.drawBox(&rect);

		s_textBold14M.cPut(iX,iY,WHITE,"%s <c:LTYELLOW>%s<n>",dMSG_REMAIN_TIME,strNumber);
	}

	s_bText.pushShadowStatus(FALSE);

	s_bText.colorChangeOff();

	int	iDelta	=	1;
	s_bText.cPut	(g_iScreenWidth/2-iDelta,22+5-iDelta,0,strTeam1Text);
	s_bText.cPut	(g_iScreenWidth/2+iDelta,22+5+iDelta,0,strTeam1Text);
	s_bText.cPut	(g_iScreenWidth/2-iDelta,22+5+iDelta,0,strTeam1Text);
	s_bText.cPut	(g_iScreenWidth/2+iDelta,22+5-iDelta,0,strTeam1Text);

	s_bText.cPut	(g_iScreenWidth/2-iDelta,22+18-iDelta,0,strTeam2Text);
	s_bText.cPut	(g_iScreenWidth/2+iDelta,22+18+iDelta,0,strTeam2Text);
	s_bText.cPut	(g_iScreenWidth/2-iDelta,22+18+iDelta,0,strTeam2Text);
	s_bText.cPut	(g_iScreenWidth/2+iDelta,22+18-iDelta,0,strTeam2Text);

	s_bText.colorChangeOn();

	s_bText.cPut	(g_iScreenWidth/2,22+5,WHITE,strTeam1Text);
	s_bText.cPut	(g_iScreenWidth/2,22+18,WHITE,strTeam2Text);

	s_bText.popShadowStatus();		
}

void
CGamePlay::DrawInfo()
{
	if	(s_bIsGuildBattleField	&&	g_guildBattleInfo.m_bIsBeginBattle)
	{
		if	(g_bIsObserverModeForDuel || g_hero.m_wOperatorLevel>=4)
			DrawInfoForDuelObserver();
		else
			DrawInfoForGuildBattle();

		return;
	}

	if	(s_bIsGuildPointBattleField)
	{
		int	iLength			=	s_bText.getPixelSize(dMSG_GUILD_HONOR_POINT)+5;
		int	iInterfaceWidth	=	iLength+80;

		cRECT	rect;

		rect.set(0,0,iInterfaceWidth+5,20);

		s_ttCommon.drawBox(&rect);

		s_bText.put	(rect.x1+5,rect.y1+5,WHITE," %s <c:LTYELLOW>%d<n>",dMSG_GUILD_HONOR_POINT,g_guild.m_iGuildHonorPoint);
	}
}

void
CGamePlay::DrawImportantInfo()
{
	DrawBoostExpAndGoldEventInfo();	//	공지 출력
	DrawNotice();	//	공지 출력
	DrawWarningMessage();
}

BOOL				
CGamePlay::drawKarmaDrawImage()
{
	
	for(int i=0;i<dMAX_KARMA_DRAW_IMAGE;++i)
	{
		if(m_karmaDrawImage[i].m_wIndex == 0xffff)
			continue;
		
		g_sprInterface2.Put(m_karmaDrawImage[i].m_posCurrent.x,m_karmaDrawImage[i].m_posCurrent.y,m_karmaDrawImage[i].m_wIndex);
	}
	return TRUE;
}

void
CGamePlay::DrawNormalGameScreen()
{
	static	BYTE	bSendIncorrectPrefix=	TRUE;

	if	(g_aItemPrefix[0].m_wSerial		==	0xffff)
	{
		g_project.resetItemPrefixData();

		if (bSendIncorrectPrefix)	
			s_agent.sendDebugMessage("reset by incorrect prefix draw");

		bSendIncorrectPrefix	=	FALSE;
	}

	WORD	*lpScreen	=	cDRAW::p2SCREEN;

	CPos	posHero;

	if (g_lpHero)
	{
		if( (g_lpHero->isBlind() || g_lpHero->isNewBlindVerySmall()|| g_lpHero->isNewBlindSmall() || g_lpHero->isNewBlindMiddle() || 
			g_lpHero->isNewBlindLarge() || g_lpHero->isNewBlindVeryLarge())&& g_lpHero->m_wOperatorLevel < 4)
		{
			g_am.getFocusPos(&posHero);
			
			posHero.x	-=	g_map.m_pos.x;
			posHero.y	-=	g_map.m_pos.y;
			
			cDRAW::SetClippginArea(posHero.x-150,posHero.y-75,posHero.x+150,posHero.y+75);
			
			lpScreen		=	GetReferenceScreen();
			
		}
//		else if(g_map.m_bf1IsNight)
//		{
//			g_am.getFocusPos(&posHero);
//			
//			posHero.x	-=	g_map.m_pos.x;
//			posHero.y	-=	g_map.m_pos.y;
//			
//			cDRAW::SetClippginArea(posHero.x-150,posHero.y-75,posHero.x+150,posHero.y+75);			
//		}
	}

	if	(s_bIsHideInterface || g_iFieldScreenHeight	!=	800)
	{
		cDRAW::PushInfo();
		cDRAW::Active(lpScreen,cDRAW::WIDTH,cDRAW::HEIGHT);
	}
	else
	{
		cDRAW::PushInfo();
		cDRAW::Active(lpScreen,cDRAW::WIDTH,g_iFieldScreenHeight);
	}

	if	(g_map.isSkippingDraw()	==	FALSE)
	{
		g_map.drawMap();		// 타일과 바닥에 깔리는 오브젝트,캐릭터,아이템들을 찍어준다.
		g_map.drawFieldTrap(g_hero.m_detectTrap.getCorrectedValue());
		g_sm.drawBottomImage();	//	스킬

		g_rapeEffect.putAll();
		g_map.drawObject();		// 오브젝트,건물,캐릭터를 그려준다.
		g_floatEffect.putAll();
		g_sm.draw();			//	스킬
		g_map.drawWeather();		// 날씨 그리기
		g_map.drawWeatherData();	// 맵에 저장된 날씨 그리기 10.02.10 추가
	}

#ifdef dDEBUG_MODE
	DrawDebugInformation();	//	디버그 정보 그림
#endif

	if	(s_bIsEventField)
	{
		cDRAW::FillHB(0,4,g_iScreenHeight-22,260,g_iScreenHeight-4);

		s_text.put(6,g_iScreenHeight-18,WHITE,dMSG_QUIZ_EVENT_FIELD_KEY_HELP);
	}

//	if	(s_bIsHideInterface	==	FALSE)
	cDRAW::PopInfo();

	if (g_lpHero)
	{
		if((g_lpHero->isBlind() || g_lpHero->isNewBlindVerySmall()|| g_lpHero->isNewBlindSmall() || g_lpHero->isNewBlindMiddle() || 
			g_lpHero->isNewBlindLarge() || g_lpHero->isNewBlindVeryLarge())&& g_lpHero->m_wOperatorLevel < 4)
		{

			cDRAW::Active(cDRAW::p2SCREEN,cDRAW::WIDTH,cDRAW::HEIGHT);
			
			cDRAW::Clear();
			
			int iMaskSize = g_lpHero->getMaskSize();
			
			int iImage = eBLIND_STATUS_REFERENCE_IMAGE_OLD;
			
			if(g_lpHero->isNewBlindVeryLarge())
				iImage = eBLIND_STATUS_REFERENCE_IMAGE_VERY_LARGE;
			
			if(g_lpHero->isNewBlindLarge())
				iImage = eBLIND_STATUS_REFERENCE_IMAGE_LARGE;
			
			if(g_lpHero->isNewBlindMiddle())
				iImage = eBLIND_STATUS_REFERENCE_IMAGE_MIDDLE;
			
			if(g_lpHero->isNewBlindSmall())
				iImage = eBLIND_STATUS_REFERENCE_IMAGE_SMALL;
			
			if(g_lpHero->isNewBlindVerySmall())
				iImage = eBLIND_STATUS_REFERENCE_IMAGE_VERY_SMALL;
			
			//g_sprInterface2.Put(100 , 200, eBLIND_STATUS_REFERENCE_IMAGE_SMALL);
			
			g_sprInterface2.putReferenceImage(posHero.x,posHero.y,iImage,GetReferenceScreen());			
			//g_lpHero->m_wIsAddPaint = FALSE;
		}
// 		else if(g_map.m_bf1IsNight)
// 		{
// 			memcpy(l_aReferenceScreen,cDRAW::p2SCREEN, sizeof(WORD) * (g_iScreenWidth * g_iScreenHeight) );
// 
// 			cDRAW::FillAB(0,0,0,cDRAW::WIDTH,cDRAW::HEIGHT,16);
// 
// 			g_sprInterface2.putReferenceImage(posHero.x,posHero.y,eI2_SMALL_OVAL,l_aReferenceScreen);
// 		}

	}

	operateScreenEffect();		//게임 화면을 제어하는 상태를 처리.

	g_bossMonsterEvent.draw();

	DrawBottomInterface();// 하단 인터페이스 그려주기

	if	(g_bossMonsterEvent.isAvail()	==	FALSE)
	{
		if	(g_bIsObserverModeForDuel	==	FALSE)
			DrawMinimap();	//	미니맵 그리기
		DrawChatMessageBox();	//	채팅 박스
		DrawInterfaceWindow();
		DrawSystemMessageBox();	//	시스템 메시지 박스

		g_dialogBox.drawComplexDialog();	//	대화

		DrawPartyMemberNameAtField();
		DrawTradeBox();

		if	(s_iPopupInterface	!=	ePIW_NONE && s_iPopupInterface != ePIW_CARROT_SHOP && s_iPopupInterface !=ePIW_HELP_SCREEN)	
			DrawPopupWindowCloseButton();

		s_pmCommon.draw();	//	일반적인 팝업 메뉴 그려주기
		DrawDragObject();	//	드래그 오브젝트 그리기
		g_itemCountWindow.draw();//	아이템 수 
		s_fieldTitle.draw();	//	필드 이름

		DrawProposeWorks();
	}
	
	if	(s_tlSystem.getLineCount() == 0	&&	s_bIsEventField	==	FALSE)
	{
BOOL	bIsMovieMode	=	FALSE;
#ifdef	_DEBUG
		bIsMovieMode	=	g_bIsMovieMode;
#endif
		
		if(!bIsMovieMode)
		{
			if	(g_bIsTestServer && s_bIsObserverMode	==	FALSE)
			{
				if	((s_iFrameCounter%(dSYNC_FPS*2)) < (dSYNC_FPS*2)*3/4)
				{
					g_sprInterface.Put(5,5,eETC_TEST_SERVER_FRAME,100,100,dPUT_HALF_BLENDING);
					g_sprInterface.Put(5,5,eETC_TEST_SERVER,100,100,dPUT_SOFT);
				}
			}
			
#ifdef	_DEBUG
			
			if	(s_bIsObserverMode	==	FALSE)
				if	(g_hero.m_wOperatorLevel	>=	2)
				{
					if	(g_bIsTestServer)			
						s_text.put(5,30,WHITE,"%s [%s] %dms/%dms [%d]",dMSG_OPERATOR_MENU_COMMENT,s_strGameServerAddress,s_agent.m_dwCurrentReturnPacketSpeed,s_agent.m_dwReturnPacketSpeed,g_sm.m_iActiveSkillCount);
					else
						s_text.put(5,5,WHITE,"%s [%s] %dms/%dms [%d]",dMSG_OPERATOR_MENU_COMMENT,s_strGameServerAddress,s_agent.m_dwCurrentReturnPacketSpeed,s_agent.m_dwReturnPacketSpeed,g_sm.m_iActiveSkillCount);
					
					g_am.findFocusActor();
					if(g_am.s_iFocusActor != 0xffff)
						s_text.put(5,50,WHITE,"ACTOR SERIAL = %d " , g_am.s_iFocusActor);
					
					
					// 				int		iResult	=	s_pmCommon.update(s_posMouse.x,s_posMouse.y,TRUE,FALSE,FALSE);
					// 				if(iResult != -1)
					// 					s_text.put(5,50,WHITE,"ACTOR SERIAL = %d " , iResult);
					//s_text.put(5,50,WHITE,"MOUSE POS = %d , %d" , s_posMouse.x , s_posMouse.y);
				}

#endif
		}
	}

	g_eventCountDown.draw();
	g_countDown.draw();

	if	(s_bIsWordQuizField	&&	g_eventWordQuiz.m_bIsWaitBegin	==	FALSE)
	{
		int		iY		=	40;
		int		iHeight	=	40;

		if	(g_eventWordQuiz.m_wTop10Count	==	0)
			iHeight		=	14;

		if	(g_hero.isOperator())
			iHeight		+=	56;

		cRECT	rect;

		rect.set(0,iY,220,iY+18*g_eventWordQuiz.m_wTop10Count+8+iHeight+4);

		s_ttCommon.drawBox(&rect);

		iY		+=	6;

		if	(g_eventWordQuiz.m_bIsEndEvent)
			s_textBold14M.cPut(rect.getWidth()/2,iY,WHITE,dMSG_FINAL_RANK);
		else
			s_textBold14M.cPut(rect.getWidth()/2,iY,WHITE,"Round %d",g_eventWordQuiz.m_wRound+1);

		if	(g_eventWordQuiz.m_wTop10Count)
		{
			iY		+=	22;

			cDRAW::Line(0xffff,10,iY-6,190,iY-6);

			for	(int i=0;i<g_eventWordQuiz.m_wTop10Count;i++)
			{
				s_textBold13M.put(6,iY,WHITE,dMSG_RANK_FORM,i+1);
				s_textBold13M.putRF(160,iY,WHITE,"<c:LTGREEN>%s<n>",g_eventWordQuiz.m_strTop10List[i]);
				s_textBold13M.putRF(204,iY,WHITE,dMSG_SCORE_FORM,g_eventWordQuiz.m_awTop10Score[i]);

				iY	+=	18;
			}

			cDRAW::Line(0xffff,10,iY,190,iY);

			s_textBold13M.cPut(rect.getWidth()/2,iY+6,WHITE,dMSG_MY_SCORE_FORM,g_eventWordQuiz.m_wScore);
		}

		if	(g_hero.isOperator())
		{
			iY	+=	22;
			cDRAW::Line(0xffff,10,iY,190,iY);

			s_text.cPut(rect.getWidth()/2,iY+6,WHITE,"User Count <c:LTYELLOW>%d<n>",g_eventWordQuiz.m_wCommerCount);
			s_text.cPut(rect.getWidth()/2,iY+20,WHITE,"Active User Count <c:LTYELLOW>%d<n>",g_eventWordQuiz.m_wLiveUserCount);
			s_text.cPut(rect.getWidth()/2,iY+34,WHITE,"Scorer Count <c:LTYELLOW>%d<n>",g_eventWordQuiz.m_wScorerCount);
		}
	}

	if	(s_bIsOXQuizField && g_hero.isOperator())
	{
		int	iY	=	0;

		if	(g_eventOxQuiz.m_bIsWaitBegin)
		{
			iY	=	100;

			cDRAW::FillHB(0,0,iY,200,iY+35);

			s_text.putRF(40,iY+4 ,WHITE,dMSG_STATUS);
			s_text.put	(46,iY+4 ,WHITE,dMSG_WAIT_BEGIN_OX_QUIZ);

			s_text.putRF(40,iY+20,WHITE,dMSG_COMER);
			s_text.put	(46,iY+20,WHITE,"<c:LTYELLOW>%-3d<n>",g_am.m_wOXQuizSurvivorCount);
		}
		else
		{
			iY	=	2;

			cDRAW::FillHB(0,0,iY,160,iY+68);

			if	(g_am.m_wOXQuizBattleCount)
			{
				s_text.putRF(40,iY+4 ,WHITE,dMSG_STATUS);
				s_text.put	(46,iY+4 ,WHITE,dMSG_PROCESS_REVIVE_OX_QUIZ_LOSER_BATTLE_FORM,g_am.m_wOXQuizBattleCount);
			}
			else
			{
				s_text.putRF(40,iY+4 ,WHITE,dMSG_STATUS );
				s_text.put  (46,iY+4 ,WHITE,dMSG_PROCESS_OX_QUIZ);
			}

			s_text.putRF(40,iY+20,WHITE,dMSG_SURVIVER);
			s_text.put	(46,iY+20,WHITE,"<c:YELLOW>%-3d<n> [in Server <c:LTGREEN>%-3d<n>]",g_am.m_wOXQuizSurvivorCount,g_am.m_wOXQuizSurvivorCountInServer);

			s_text.putRF(40,iY+36,WHITE,dMSG_LOSER);
			s_text.put	(46,iY+36,WHITE,"<c:YELLOW>%-3d<n>",g_am.m_wOXQuizLoserCount);

			s_text.putRF(40,iY+52,WHITE,dMSG_WINNER);
			s_text.put	(46,iY+52,WHITE,"<c:LTGREEN>%-3d<n>",g_am.m_wOXQuizWinnerCount);
		}
	}

	//g_sprInterface2.putReferenceFogEffect(posHero.x , posHero.y , 500);

	g_fireCrackerManager.draw(g_map.m_pos.x,g_map.m_pos.y);
	drawKarmaDrawImage();
}

//
//	메인
void
CGamePlay::DrawMain()
{
	if	(s_bIsFailedToJoinGame || g_hero.m_iLevel	==	0)
		return;

	if	(s_bIsFullScreenInterface)
	{
		DrawFullScreenInterface();
	}
	else
	{
		DrawNormalGameScreen();
	}

	if	(s_bIsBookedDisableChattingBox)
	{
		s_bIsBookedDisableChattingBox	=	FALSE;

		if	(s_inputChatting.isEnable())
			s_inputChatting.disable(TRUE);
	}

	if	(g_gwMainQuestEvent.isOpened() || g_gwBook.isOpened() ||g_gwBookMonDic.isOpened()|| g_bossMonsterEvent.isAvail())
	{
		if	(g_gwMainQuestEvent.isOpened())
		{
			if	(g_gwMainQuestEvent.m_iStep	<=	eMQES_FADE_OUT_GAME_SCREEN)
			{
				int	iFadeStep	=	g_gwMainQuestEvent.m_iFadeValueForFlyEagle/100;

				cDRAW::FillAB(0,0,0,g_iScreenWidth,g_iScreenHeight,32-iFadeStep);

				g_lpHero->put();

				g_floatEffect.putAll();
			}
		}
		return;
	}

	DrawImportantInfo();
	DrawInfo();
#ifdef	_FOR_KOREA
	if(CGamePlay::s_dwRank12AgeTime )
	{
		int iRank12Age = g_sprRegionInterface.getSpriteWidth(eIK_RANK_12AGE);
		g_sprRegionInterface.Put(g_iScreenWidth-iRank12Age-5,10,eIK_RANK_12AGE);
	}
#endif
	return;
}

void
CGamePlay::DrawChattingBar()
{
	if	(s_iMode	!=	eGAME_PLAY)
		return;

	if	(s_iStep	!=	dSTEP_MAIN)
		return;

	if	(s_inputChatting.isEnable())
	{
		int	iInputBarPos=492;

		if	(s_bIsHideInterface)
			iInputBarPos=576;

		g_sprInterface.Put(79+g_iCorrectBI_X,iInputBarPos+g_iCorrectBI_Y,eCII_CHATTING_INPUT);

		if	(g_config.m_bf1IsBattleChat)
			s_bmChatBarMenu.setImageBar(0	,eCII_CHATTING_INPUT_UNLOCK_NORMAL,eCII_CHATTING_INPUT_UNLOCK_ACTIVE,eCII_CHATTING_INPUT_UNLOCK_ACTIVE,0xffff,0xffff);
		else
			s_bmChatBarMenu.setImageBar(0	,eCII_CHATTING_INPUT_LOCK_NORMAL,eCII_CHATTING_INPUT_LOCK_ACTIVE,eCII_CHATTING_INPUT_LOCK_ACTIVE,0xffff,0xffff);

		s_bmChatBarMenu.correctPos(0xffffffff,79+g_iCorrectBI_X,iInputBarPos+g_iCorrectBI_Y);
		s_bmChatBarMenu.draw();

		s_text.pushShadowStatus(FALSE);
		s_inputChatting.draw(BLACK);
		s_text.popShadowStatus();
	}
}

void
CGamePlay::DrawProposeWorks()
{
	if	(s_propose.m_dwValue == 0)
		return;

	cRECT	rect;
	int		iRed	=	0;//128*dCOS_TABLE[s_iFrameCounter*2%180]/256;
	int		iGreen	=	0;//128*dCOS_TABLE[s_iFrameCounter*2%180]/256;
	int		iBlue	=	255*dCOS_TABLE[s_iFrameCounter*2%180]/256;

	DWORD	dwColor	=	RGB(iRed,iGreen,iBlue);
	rect.set(s_posMouse.x+8,s_posMouse.y+16,s_posMouse.x+160,s_posMouse.y+56);

	char	*lpstrMessage	=	NULL;

	if	(s_propose.m_work.isProposeToUnionParty		)
		lpstrMessage	=	dMSG_PLASE_SELECT_UNION_PARTY_PLAYER;
	else
	if	(s_propose.m_work.isProposeToParty			)
		lpstrMessage	=	dMSG_PLEASE_SELECT_PROPOSE_PARTY_PLAYER;
	else
	if	(s_propose.m_work.isProposeToTrade			)
		lpstrMessage	=	dMSG_PLEASE_SELECT_PROPOSE_TRADE_PLAYER;
	else
	if	(s_propose.m_work.isReadyToAddFriend		)
		lpstrMessage	=	dMSG_PLEASE_SELECT_ADD_TO_FRIEND_CHARACTER;
	else
	if	(s_propose.m_work.isReadyUseItemToItem		)
		lpstrMessage	=	dMSG_PLEASE_SELECT_ITEM_USE_ITEM;
	else
	if	(s_propose.m_work.isReadyUseItemToCharacter	)
		lpstrMessage	=	dMSG_PLEASE_SELECT_ITEM_USE_ITEM;
	else
	if	(s_propose.m_work.isReadyAskJoinGuild	)
		lpstrMessage	=	dMSG_PLEASE_SELECT_GUILD_MASTER;
	else
	if	(s_propose.m_work.isReadyUseItemToCorpse	)
		lpstrMessage	=	dMSG_PLEASE_SELECT_CORPSE_PLAYER_FOR_WANT_REVIVE;
	else	
	if	(s_propose.m_work.isReadyUseItemToEnemy	)
		lpstrMessage	=	dMSG_PLEASE_TO_TARGET;

	if	(lpstrMessage)
	{
		int	iWidth	=	s_ttDialogFrame.m_iLeftEdgeWidth+s_ttDialogFrame.m_iRightEdgeWidth+s_ttDialogFrame.m_wHorzSpace*2;
		int	iHeight	=	s_ttDialogFrame.m_lpText->getTextHeight(rect.getWidth()-iWidth,lpstrMessage);
		iHeight		+=	(s_ttDialogFrame.m_iTopEdgeHeight+s_ttDialogFrame.m_wVertSpace)+(s_ttDialogFrame.m_iBottomEdgeHeight+s_ttDialogFrame.m_wVertSpace);
		rect.y2		=	rect.y1+iHeight;

		s_ttDialogFrame.drawTextBox(&rect,dwColor,0xffff,lpstrMessage,FALSE,32);
	}
}

//
//	드래그중인 오브젝트를 그려준다.
void
CGamePlay::DrawDragObject()
{
	if (s_iDragItem	!=	0xffff)
	{
		cItem			*lpItem	=	NULL;
		CPos			pos;

		if (s_iDragItem	==	dITEM_DRAG_GOLD)
			lpItem	=	&g_itemGold;
		else
		{
			if	(s_iDragItem>=dBORDER_OF_ITEM_AND_EQUIPMENT)
				lpItem	=	&s_lpInventoryWindowEquipmentList[s_iDragItem-dBORDER_OF_ITEM_AND_EQUIPMENT];
			else
			if	(s_iDragItem>=dOWN_ITEM_COUNT)
				lpItem	=	&g_hero.m_aExtraInventory[s_iDragItem-dOWN_ITEM_COUNT];
			else	
				lpItem	=	&s_lpInventoryWindowItemList[s_iDragItem];
		}

		pos.x	=	s_posMouse.x-dITEM_SHAPE_HEIGHT/2;
		pos.y	=	s_posMouse.y-dITEM_SHAPE_HEIGHT/2;

		g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_FRAME);

		PutItem(lpItem,pos.x,pos.y,FALSE);
	}

	if (s_iDragSkill!= 0xffff)
	{
		CSkill	*lpSkill;

		int	iAbility	=	s_iDragSkill;

		if (s_iDragSkill	>=	1000)
			iAbility	=	g_hero.getQuickSlotSkill(iAbility-1000);

		lpSkill			=	g_hero.m_aAbility[iAbility].getSkill();

		g_sprInterface.Put(s_posMouse.x-dITEM_SHAPE_HEIGHT/2,s_posMouse.y-dITEM_SHAPE_HEIGHT/2,eIWI_ITEM_SLOT_FRAME);
		g_smiIconSkill.put(lpSkill->m_wIconIndex,s_posMouse.x-dITEM_SHAPE_HEIGHT/2,s_posMouse.y-dITEM_SHAPE_HEIGHT/2);
	}

	if (s_iDragTradeItem	!=	0xffff	&&	IsOpenTradeBox())
	{
		cItem	*lpTradeSlot	=	&s_tradeManager.m_myTradeBox.m_aItems[s_iDragTradeItem];
		cItem	*lpItem,item;
		CPos	pos;

		lpItem	=	g_hero.getInventoryItem(lpTradeSlot->m_wBaseItem,TRUE);

		if (lpItem)
		{
			item.copy(lpItem);
			item.m_bCount	=	lpTradeSlot->m_bCount;

			pos.x	=	s_posMouse.x-dITEM_SHAPE_HEIGHT/2;
			pos.y	=	s_posMouse.y-dITEM_SHAPE_HEIGHT/2;
			g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_FRAME);
			PutItem(&item,pos.x,pos.y,FALSE);
		}
	}

	if (g_gwPitchmanShop.isOpened()	&& g_gwPitchmanShop.m_iDragItem	!=	0xffff)
	{
		int		iInventorySlot	=	g_gwPitchmanShop.m_shop.m_aItem[g_gwPitchmanShop.m_iDragItem].m_item.m_wBaseItem;
		cItem	*lpItem			=	g_hero.getInventoryItem(iInventorySlot,TRUE);
		cItem	item;
		CPos	pos;

		if (lpItem)
		{
			pos.x	=	s_posMouse.x-dITEM_SHAPE_HEIGHT/2;
			pos.y	=	s_posMouse.y-dITEM_SHAPE_HEIGHT/2;
			g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_FRAME);
			PutItem(lpItem,pos.x,pos.y,FALSE);
		}
	}

	if (g_gwBank.m_iDragBankItem != 0xffff && s_iPopupInterface == ePIW_BANK)
	{
		cItem	*lpItem = NULL;

		if (g_gwBank.m_iDragBankItem	==	dITEM_DRAG_GOLD)	lpItem	=	&g_itemGold;
		else													lpItem	=	g_bank.getBankItem(g_gwBank.m_iDragBankItem);

		if (lpItem)
		{
			CPos	pos;

			pos.x	=	s_posMouse.x-dITEM_SHAPE_HEIGHT/2;
			pos.y	=	s_posMouse.y-dITEM_SHAPE_HEIGHT/2;
			g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_FRAME);
			PutItem(lpItem,pos.x,pos.y,FALSE);
		}
	}

	if	(g_gwGuildInventory.m_iDragItem != 0xffff && s_iPopupInterface == ePIW_GUILD_INVENTORY)
	{
		cItem	*lpItem = NULL;

		if	(g_gwGuildInventory.m_iDragItem	==	dITEM_DRAG_GOLD)
			lpItem	=	&g_itemGold;
		else
			lpItem	=	g_guild.getItem(g_gwGuildInventory.m_iDragItem);

		if	(lpItem)
		{
			CPos	pos;

			pos.x	=	s_posMouse.x-dITEM_SHAPE_HEIGHT/2;
			pos.y	=	s_posMouse.y-dITEM_SHAPE_HEIGHT/2;
			g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_FRAME);
			PutItem(lpItem,pos.x,pos.y,FALSE);
		}
	}

	if	(g_gwCart.m_iDragCartItem != 0xffff && s_iPopupInterface == ePIW_SHOPPING_CART)
	{
		cItem	item;
		CPos	pos;

		pos.x	=	s_posMouse.x-dITEM_SHAPE_HEIGHT/2;
		pos.y	=	s_posMouse.y-dITEM_SHAPE_HEIGHT/2;
		
		if (g_shoppingCart.generateItem(&item,g_gwCart.m_iDragCartItem))
		{
			g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_FRAME);
			PutItem(&item,pos.x,pos.y,FALSE);
		}
	}
	if	(g_gwUpgradeShop.m_iDragItem != 0xffff && s_iPopupInterface == ePIW_UPGRADE_SHOP)		// 강화창에서 드래그 아이템 그리기~+_ +09.09.01
	{
		
		CPos	pos;
		
		pos.x	=	s_posMouse.x-dITEM_SHAPE_HEIGHT/2;
		pos.y	=	s_posMouse.y-dITEM_SHAPE_HEIGHT/2;
		cItem*	lpItem = g_hero.getInventoryItem(g_gwUpgradeShop.m_iDragItem);
		if (lpItem)
		{
			g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_FRAME);
			PutItem(lpItem,pos.x,pos.y,FALSE);
		}
	}
}

#ifdef dDEBUG_MODE
void
CGamePlay::DrawDebugInformation()
{
	//	정보를 그려준다.
	if (s_isPaintBlockedTile)	g_map.drawBlockedTile();
}
#endif
