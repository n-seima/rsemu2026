#include "CGamePlay.H"
#include "CHero.H"
#include "CActor.H"
#include "CSound.H"
#include "CMessage.H"
#include "CWindowInterface.H"
#include "mainQuest.H"

#define	dQW_PROCESS_QUEST_LIST_GAP		20
#define	dQW_QUEST_CONTENTS_WIDTH		197



// 드롭다운리스트 텍스트
enum
{
	eDL_ALL,
	eDL_DO_NOT_COMPLETE,
	eDL_COMPLETE,
};

// 선택한 탭
enum
{
	eQW_TAB_NORMAL,
	eQW_TAB_MAIN,
	eQW_TAB_HISTORY,
};

#define dSELECT_QUEST_COLOR				LTYELLOW
#define dFOCUS_QUEST_COLOR				GOLDYELLOW
#define dCOMPLETE_QUEST_COLOR			GRAY
#define dDO_NOT_COMPLETE_QUEST_COLOR	WHITE
#define dPROCESS_QUEST_COLOR			LTGREEN

int iYgap = 20;     // 텍스트높이 포함 다음줄 표시까지의 픽셀


enum
{
	eWINDOW_BIG			=	90,
	eQW_NEW_LINE		=	91,
	eQW_FRAME			=	0,
	eWINDOW_TITLE_QUEST	=	1,

	eTAB_FIRST_BAR		=	72,
};

CQuestWindow	g_gwQuest;

CQuestWindow::CQuestWindow()
{
	m_iSelectTab	=	eSQT_NORMAL;
	m_iFocusTab		=	-1;
}

//
//시스템 메뉴 열기
void
CQuestWindow::open()
{
	if	(s_iPopupInterface	==	ePIW_QUEST)
	{
		ClosePopupWindow();

		return;
	}

	ClosePopupWindow();

	s_iPopupInterface		=	ePIW_QUEST;
	m_iSelectTab			=	eSQT_NORMAL;

	int	iWindowWidth		=	g_sprInterface2.getSpriteWidth(eWINDOW_BIG);
	int	iWindowHeight		=	g_sprInterface2.getSpriteHeight(eWINDOW_BIG);
	int	iTitleBarHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_INVENTORY);
	int	iBarHeight			=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);
	int	iBarWidth			=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);

	s_rectInterfaceSize.set(g_iScreenWidth-iWindowWidth, g_iCorrectBI_Y/2, g_iScreenWidth, g_iCorrectBI_Y/2+iWindowHeight+iTitleBarHeight);
	m_posQuestWindow.x		=	g_iScreenWidth-iWindowWidth;
	m_posQuestWindow.y		=	iTitleBarHeight+g_iCorrectBI_Y/2;

	s_bmButton.reset();

	int	iX					=	m_posQuestWindow.x+(iWindowWidth-6)/2-iBarWidth/2;
	int	iY					=	m_posQuestWindow.y+iWindowHeight-9-iBarHeight;

	s_bmButton.addImageBar(dMSG_CANCEL,1,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);

	s_bmButton.setClicktAllBarMargin(1,1);
	s_bmButton.setAllBarTextColor(0,FALSE);

	s_bmBottomMenu.resetChecker();
	s_bmBottomMenu.check(eBIM_MENU_QUEST,TRUE);

	InitPopupWindowCloseButton();


	CQuest	*lpQuestManager = &g_quest;

	m_iSelectRadioBtn     = 0;
	m_iFocusChapterBtn    = -1;
	m_iFocusPartBtn	      = -1;
	m_iSelectHistoryQuest = -1;
	m_iProcessMainQuestIndex = -1;
	m_iProcessQuestPartInQuestCount = 0;

	for(int i=0 ; i<dMAX_PROCESS_QUEST_COUNT ; i++)
	{
		int	iQuestIndex	=	g_hero.m_aProcessQuest[i].questIndex;
		if (g_hero.m_aProcessQuest[i].questKind	== eQT_NORMAL)
		{
			m_iSelectHistoryQuest = iQuestIndex;
			m_iProcessMainQuestIndex = iQuestIndex;
			break;
		}
	}
	memset(m_wMainQuestTotalCount, 0, sizeof(WORD)*c_iMaxChapterCount*c_iMaxHighQuestCount);
	memset(m_wMainBonusQuestCount, 0, sizeof(WORD)*c_iMaxChapterCount*c_iMaxHighQuestCount);
	m_iNormalQuestCnt = 0;
	m_iNormalQuestCompleteCnt = 0;
	m_iFocusNormalQuest = -1;
	m_iTitleQuestCnt  = 0;
	m_iTitleQuestCompleteCnt = 0;
	m_iSkillQuestCnt = 0;
	m_iSkillQuestCompleteCnt = 0;
	m_iSelectNormalQuest = -1;
	m_iSelectNormalQuestType = -1;
	m_iFocusDropDownBtn = -1;
	m_iTitleQuestSelectQuest = -1;
	m_iTitleQuestSelectQuestType = -1;

	for(i=0 ; i<c_iMaxChapterCount ; i++)
		m_bChapterBtnState[i] = TRUE;
	for(i=0 ; i<c_iMaxChapterCount ; i++)
		for(int j=0 ; j<c_iMaxHighQuestCount ; j++)
			m_bPartBtnState[i][j] = TRUE;
	for(i=0 ; i<dHISTORY_TITLE_DROPDOWN_BUTTON_COUNT ; i++)
		m_bDropDownBtnState[i] = TRUE;

	setMainQuestGroup();

	// 퀘스트리스트 출력영역 설정
	// 왠만하면 cRECT의 높이가 iYgap 으로 나눠 떨어지도록 설정하는것이 좋다.
	m_rtQuestListArea.set(s_rectInterfaceSize.x1+12, m_posQuestWindow.y+50, s_rectInterfaceSize.x2-10, m_posQuestWindow.y+390);

	// 히스토리 필터 위치 설정
	m_rtHistoryFilter.set(s_rectInterfaceSize.x2-80, m_posQuestWindow.y+28, s_rectInterfaceSize.x2-10, m_posQuestWindow.y+46);

	// 스크롤바 셋팅
	if	(!m_sbScrollBar.m_lpSprite)
	{
		m_sbScrollBar.init(&g_sprInterface, dIMAGE_SCROLL_BAR_VERTICAL,
		eSCROLLBAR_VERTICAL_NORMAL_UP_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_UP_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_UP_BUTTON,0xffff,
		eSCROLLBAR_VERTICAL_NORMAL_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_DOWN_BUTTON,0xffff,
		eSCROLLBAR_VERTICAL_NORMAL_BODY_TOP,	eSCROLLBAR_VERTICAL_NORMAL_BODY,		eSCROLLBAR_VERTICAL_NORMAL_BODY_BOTTOM,
		eSCROLLBAR_VERTICAL_ACTIVE_BODY_TOP,	eSCROLLBAR_VERTICAL_ACTIVE_BODY,		eSCROLLBAR_VERTICAL_ACTIVE_BODY_BOTTOM);

		m_sbScrollBar.setSize(0, 0);
		m_sbScrollBar.setPos(0);		
	}	
	m_sbScrollBar.set(m_rtQuestListArea.x2-15, m_rtQuestListArea.y1+3, m_rtQuestListArea.getHeight()-4);
	setScrollBarSize(&m_rtQuestListArea);
	
	m_dlmenu.init(m_rtHistoryFilter.x1, m_rtHistoryFilter.y1, m_rtHistoryFilter.getWidth(), &s_text, &s_ttCommon, &g_sprInterface, 90, 91, 92);
	m_dlmenu.addTextList(dMSG_CARROTSHOP_CATEGORY_ALL);
	m_dlmenu.addTextList(dMSG_DO_NOT_COMPLETE);
	m_dlmenu.addTextList(dMSG_COMPLETE);
}

//
//창 닫아
void
CQuestWindow::close()
{
	cINPDEV::ReleaseExclusive();
	s_bmBottomMenu.check(eBIM_MENU_QUEST,FALSE);
	s_bmButton.reset();
}

//
//	포커스가 간 퀘스트
int
CQuestWindow::getFocusedQuest()
{
	if	(m_iSelectTab	!=	eSQT_NORMAL)
		return	0xffff;

	int		i;

	int		iX		=	m_posQuestWindow.x+30;
	int		iY		=	m_posQuestWindow.y+14;

	iY		+=	g_sprInterface2.getSpriteHeight(eTAB_FIRST_BAR);

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (g_hero.m_aProcessQuest[i].isActive	==	FALSE		)	continue;

		cRECT	rect;

		rect.set(iX,iY,g_iScreenWidth-10,iY+dQW_PROCESS_QUEST_LIST_GAP-2);

		iY	+=	dQW_PROCESS_QUEST_LIST_GAP;

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return		i;
	}

	return	0xffff;
}

int
CQuestWindow::getFocusedTab()
{
	int	iWindowWidth = g_sprInterface2.getSpriteWidth(eWINDOW_BIG);
	int	iTabWidth    = (iWindowWidth-14)/3;
	int	iX           = m_posQuestWindow.x+8;
	int iY           = m_posQuestWindow.y+4;
	
	for (int i=0 ; i<3 ; i++)
	{
		cRECT rect={iX, iY, iX+iTabWidth, iY+g_sprInterface2.getSpriteHeight(eTAB_FIRST_BAR)};

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
			return i;
		
		iX += (iTabWidth+2);
	}

	return	-1;
}

void
CQuestWindow::drawTab()
{
	int	iWindowWidth = g_sprInterface2.getSpriteWidth(eWINDOW_BIG);
	int	iTabWidth    = (iWindowWidth-14)/3;
	int	iX           = m_posQuestWindow.x+8;
	int iY           = m_posQuestWindow.y+4;

	for (int i=0 ; i<3 ; i++)	// 탭 갯수
	{
		int		iImage	=	eTAB_FIRST_BAR;
		DWORD	dwColor	=	0;

		if	(i	==	m_iSelectTab)
		{
			iImage	+=	3;
			dwColor	=	LTBLUE;
		}
		else
		if	(i	==	m_iFocusTab)
			iImage	+=	6;

		char	*lpstrMessage;

		if	(i == 0)
			lpstrMessage	=	dMSG_NORMAL_QUEST;
		if  (i == 1)
			lpstrMessage	=	dMSG_MAIN_QUEST;
		if  (i == 2)
			lpstrMessage	=	dMSG_QUEST_HISTORY;

		g_sprInterface2.put3PieceBar(iX+4, iY, iImage, iTabWidth-8);
		
		s_text.pushShadowStatus(FALSE);
		s_text.cPut(iX+iTabWidth/2, iY+4, dwColor, lpstrMessage);
		s_text.popShadowStatus();

		iX += (iTabWidth+2);
	}
}

void
CQuestWindow::drawNormalQuestList()
{
	if	(m_iSelectTab	!=	eSQT_NORMAL)
		return;

	int		i;
	int		iX,iY;
	int		iQuestCount		=	g_hero.getProcessQeustCount();

	iX		=	m_posQuestWindow.x+15;
	iY		=	m_posQuestWindow.y+14;

	iY		+=	g_sprInterface2.getSpriteHeight(eTAB_FIRST_BAR);

	if	(iQuestCount	<=	0)
	{
		s_text.cPut(iX+dQW_QUEST_CONTENTS_WIDTH,iY,WHITE,dMSG_NOT_EXIST_PROCESS_QUEST);

		return;
	}

	CQuest	*lpQuestManager;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if	(g_hero.m_aProcessQuest[i].isActive		==	FALSE		)
			continue;
		if	(g_hero.m_aProcessQuest[i].questKind	==	eQT_NORMAL	)
			lpQuestManager	=	&g_quest;
		if	(g_hero.m_aProcessQuest[i].questKind	==	eQT_ARBEIT	)
			lpQuestManager	=	&g_questArbeit;

		int			iQuestIndex	=	g_hero.m_aProcessQuest[i].questIndex;
		CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);
		char		strQuestName[128];

		if	(lpQuest->m_wQuestKind	==	eQK_MAIN_STORY)
			strcpy(strQuestName,dMSG_PROCESS_MAIN_QUEST);
		else
		{
			if	(g_hero.m_aProcessQuest[i].isComplete)
				sprintf(strQuestName,"%s (<c:LTGREEN>C<n>)",lpQuest->m_strName);
			else
				strcpy(strQuestName,lpQuest->m_strName);
		}

		if	(i	==	m_iSelectQuest)
		{
			int	iWidth	=	s_text.getPixelSize(strQuestName);
			int	iGab	=	((s_iFrameCounter/6)%5);

			s_text.cPut(iX+dQW_QUEST_CONTENTS_WIDTH,iY,LTYELLOW,strQuestName);

			g_sprInterface.Put(iX+dQW_QUEST_CONTENTS_WIDTH-(iWidth/2+4+iGab),iY-3,eQW_LEFT_HAND);
			g_sprInterface.Put(iX+dQW_QUEST_CONTENTS_WIDTH+(iWidth/2+4+iGab),iY-3,eQW_RIGHT_HAND);
		}
		else
		{
			if	(i	==	m_iFocusQuest)
				s_text.cPut(iX+dQW_QUEST_CONTENTS_WIDTH,iY,GOLDYELLOW,strQuestName);
			else
				s_text.cPut(iX+dQW_QUEST_CONTENTS_WIDTH,iY,LTGRAY,strQuestName);
		}

		iY						+=	dQW_PROCESS_QUEST_LIST_GAP;
	}

	for (i=iQuestCount;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		s_text.cPut(iX+dQW_QUEST_CONTENTS_WIDTH,iY,GRAY,"- EMPTY -");
		iY					+=	dQW_PROCESS_QUEST_LIST_GAP;
	}
}

void
CQuestWindow::drawMainQuestInfo()
{
	if	(m_iSelectTab	==	eSQT_NORMAL)
		return;

	if	(m_iSelectQuest	==	-1)
		return;

	int		iX,iY;

	iX		=	m_posQuestWindow.x+15;
	iY		=	m_posQuestWindow.y+14;

	iY		+=	g_sprInterface2.getSpriteHeight(eTAB_FIRST_BAR);

	CQuestItem	*lpQuest;
	int			iQuestIndex	=	g_hero.m_aProcessQuest[m_iSelectQuest].questIndex;

	if	(g_hero.m_aProcessQuest[m_iSelectQuest].questKind	==	eQT_NORMAL	)
		lpQuest	=	g_quest.getQuest(iQuestIndex);
	else
		lpQuest	=	g_questArbeit.getQuest(iQuestIndex);

	if	(lpQuest->m_wQuestKind	!=	eQK_MAIN_STORY)
		return;

	iY		+=	10;

	char	*lpstrChapterName	=	g_chapterManager.getChapterName(g_hero.m_wProcessChapterIndex);
	char	*lpstrHighQuestName	=	g_chapterManager.getHighQuestName(g_hero.m_wProcessChapterIndex,g_hero.m_wProcessHighQuestIndex);

	if	(!lpstrChapterName)
		lpstrChapterName	=	"valid";

	if	(!lpstrHighQuestName)
		lpstrHighQuestName	=	"valid";

	{
		s_textBold16M.cPut(iX+dQW_QUEST_CONTENTS_WIDTH,iY+2,WHITE,"<c:CTBLUE>%s %d<n> %s","CHAPTER",g_hero.m_wProcessChapterIndex+1,lpstrChapterName);

		iY						+=	dQW_PROCESS_QUEST_LIST_GAP+30;
	}

	{
		s_textBold14M.cPut(iX+dQW_QUEST_CONTENTS_WIDTH,iY-4,WHITE,"<c:BTBLUE>PART %d<n>",g_hero.m_wProcessHighQuestIndex+1);
		s_textBold14M.cPut(iX+dQW_QUEST_CONTENTS_WIDTH,iY-3+22,WHITE,lpstrHighQuestName);
	}

	{
		iY						+=	dQW_PROCESS_QUEST_LIST_GAP;
		g_sprInterface2.Put(iX+15,iY-6,eQW_NEW_LINE);
		iY						+=	dQW_PROCESS_QUEST_LIST_GAP+4;

		s_textBold13M.cPut(iX+dQW_QUEST_CONTENTS_WIDTH,iY,LTYELLOW,lpQuest->m_strName);

		int	iWidth	=	s_textBold13M.getPixelSize(lpQuest->m_strName);
		int	iGab	=	((s_iFrameCounter/6)%5);

		g_sprInterface.Put(iX+dQW_QUEST_CONTENTS_WIDTH-(iWidth/2+4+iGab),iY-3,eQW_LEFT_HAND);
		g_sprInterface.Put(iX+dQW_QUEST_CONTENTS_WIDTH+(iWidth/2+4+iGab),iY-3,eQW_RIGHT_HAND);
	}
}

//
//스테이터스 인터페이스 그리기
void
CQuestWindow::draw()
{
	int		i;

	if	(s_iPopupInterface	!=	ePIW_QUEST)
		return;

	if	(m_iSelectQuest		!=	-1)
		if	(g_hero.m_aProcessQuest[m_iSelectQuest].isActive	==	FALSE)
			m_iSelectQuest	=	-1;

	int		iQuestCount		=	g_hero.getProcessQeustCount();
	int		iWindowWidth	=	g_sprInterface2.getSpriteWidth(eWINDOW_BIG);
	int		iWindowHeight	=	g_sprInterface2.getSpriteHeight(eWINDOW_BIG);

	int		iX,iY;

	iX		=	m_posQuestWindow.x;
	iY		=	m_posQuestWindow.y;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+iWindowWidth-4,iY+482-8);
	else
		cDRAW::Fill	 (0,iX+8,iY+6,iX+iWindowWidth-4,iY+482-8);

	if (m_iSelectTab != eQW_TAB_HISTORY)
		g_sprRegionInterface.Put(iX+9	,iY+10+g_sprInterface2.getSpriteHeight(eTAB_FIRST_BAR)	,eQW_FRAME);
	g_sprInterface2.Put(iX	,iY		,eWINDOW_BIG);
	g_sprRegionInterface.Put(iX	,g_iCorrectBI_Y/2	,eWINDOW_TITLE_QUEST);

	drawTab();
	iX		=	m_posQuestWindow.x+30;
	iY		=	m_posQuestWindow.y+14;

	iY		+=	g_sprInterface2.getSpriteHeight(eTAB_FIRST_BAR);
	if (m_iSelectTab != eQW_TAB_HISTORY)
	{
		if	(iQuestCount	<=	0)
		{
			s_text.cPut(iX+dQW_QUEST_CONTENTS_WIDTH,iY,WHITE,dMSG_NOT_EXIST_PROCESS_QUEST);
			
			return;
		}
		
#ifdef	_FOR_ENG
		iY		+=	5;
		iX		-=	10;
#endif
		
		drawNormalQuestList();
		drawMainQuestInfo();
		
		if	(m_iSelectQuest	==	-1)
			return;
		
		iY		+=	dMAX_PROCESS_QUEST_COUNT*dQW_PROCESS_QUEST_LIST_GAP;
		iY		+=	16;
		
		tsProcessQuestField	*lpQuestInfo	=	&g_hero.m_aProcessQuest[m_iSelectQuest];
		CQuestItem			*lpSelectQuest;
		int					iProcessLevel	=	lpQuestInfo->questProcessLevel;
		
		if (lpQuestInfo->questKind	==	eQT_NORMAL	)
			lpSelectQuest	=	g_quest.getQuest(lpQuestInfo->questIndex);
		if (lpQuestInfo->questKind	==	eQT_ARBEIT	)
			lpSelectQuest	=	g_questArbeit.getQuest(lpQuestInfo->questIndex);
		
		CQuestItem::cQuestInfo	*lpQuestText	=	lpSelectQuest->getInfo(iProcessLevel);
		
		int		iTextFrontGab		=	8,iLineCount=0;
		int		iCommentWidth		=	g_iScreenWidth-(iX+iTextFrontGab+10);
		int		iLineSize			=	s_text.m_iFontHeight+s_text.m_iLineGap;
		char	*lpstrClientName	=	lpSelectQuest->getClientName(iProcessLevel);
		char	*lpstrQuestValue	=	lpSelectQuest->getValueString(iProcessLevel);
		int		iContentsGab		=	4;
		
		if	(lpstrClientName)	//	퀘스트 의뢰자
		{
			int		iClientField	=	lpSelectQuest->getClientField(iProcessLevel);
			
			s_text.put(iX,iY,CTBLUE,dMSG_QUEST_CLIENT_INFO);
			iY	+=	(dQW_PROCESS_QUEST_LIST_GAP-6);
			g_sprInterface2.Put(iX,iY,eQW_NEW_LINE);
			iY	+=	4;
			char	strClientInfo[256];
			
			if (iClientField	==	0xffff)
				strcpy(strClientInfo,lpstrClientName);
			else
				sprintf(strClientInfo,"<c:WHITE>%s<n> %s",g_aMapSimpleInfo[iClientField].m_strName,lpstrClientName);
			
			s_text.put(iX+iTextFrontGab,iY,iCommentWidth,100,0,WHITE,strClientInfo);
			
			iLineCount	=	s_text.getLineCount(iCommentWidth,strClientInfo);
			iY			+=	(iLineSize*iLineCount+iContentsGab);
		}
		
		if (lpstrQuestValue)	//	퀘스트 수치에 따른 설명
		{
			char	*lpstrText	=	_ms(lpstrQuestValue,lpQuestInfo->questValue);
			
			s_text.put(iX,iY,CTBLUE,dMSG_PROCESS_STATUS);
			iY	+=	(dQW_PROCESS_QUEST_LIST_GAP-6);
			g_sprInterface2.Put(iX,iY,eQW_NEW_LINE);
			
			iY	+=	4;
			s_text.put(iX+iTextFrontGab,iY,iCommentWidth,100,0,WHITE,lpstrText);
			
			iLineCount	=	s_text.getLineCount(iCommentWidth,lpstrText);
			iY	+=	(iLineSize*iLineCount+iContentsGab);
		}
		
		if (lpQuestText->m_wLinkNpcCount)
		{
			s_text.put(iX,iY,CTBLUE,dMSG_LINK_QUEST_NPC);
			iY			+=	(dQW_PROCESS_QUEST_LIST_GAP-6);
			g_sprInterface2.Put(iX,iY,eQW_NEW_LINE);
			iY			+=	4;
			
			for (i=0;i<lpQuestText->m_wLinkNpcCount;i++)
			{
				int		iField		=	lpQuestText->m_pNpcList[i].m_wField;
				char	*lpstrName	=	lpQuestText->m_pNpcList[i].m_pStrName;
				char	strString[256];
				
				if (iField	==	0xffff)
					strcpy(strString,lpstrName);
				else
					sprintf(strString,"<c:WHITE>%s<n> %s",g_aMapSimpleInfo[iField].m_strName,lpstrName);
				
				s_text.put(iX+iTextFrontGab,iY,iCommentWidth,100,0,LTYELLOW,strString);
				
				iLineCount	=	s_text.getLineCount(iCommentWidth,strString);
				iY			+=	iLineSize*iLineCount;
			}
			
			iY	+=	iContentsGab;
		}
		
		if (lpQuestText->m_wTargetMonsterCount)
		{
			s_text.put(iX,iY,CTBLUE,dMSG_LINK_QUEST_MONSTER);
			iY			+=	(dQW_PROCESS_QUEST_LIST_GAP-6);
			g_sprInterface2.Put(iX,iY,eQW_NEW_LINE);
			iY			+=	4;
			
			for (i=0;i<lpQuestText->m_wTargetMonsterCount;i++)
			{
				int		iField		=	lpQuestText->m_pTargetMonsterList[i].m_wField;
				char	*lpstrName	=	lpQuestText->m_pTargetMonsterList[i].m_pStrName;
				char	strString[256];
				
				if (iField	==	0xffff)
					strcpy(strString,lpstrName);
				else
					sprintf(strString,"<c:WHITE>%s<n> %s",g_aMapSimpleInfo[iField].m_strName,lpstrName);
				
				s_text.put(iX+iTextFrontGab,iY,iCommentWidth,100,0,LTYELLOW,strString);
				
				iLineCount	=	s_text.getLineCount(iCommentWidth,strString);
				iY			+=	iLineSize*iLineCount;
			}
			
			iY	+=	iContentsGab;
		}
		
		if (lpQuestText->m_wRequireItemCount)
		{
			s_text.put(iX,iY,CTBLUE,dMSG_LINK_QUEST_ITEM);
			iY			+=	(dQW_PROCESS_QUEST_LIST_GAP-6);
			g_sprInterface2.Put(iX,iY,eQW_NEW_LINE);
			iY			+=	4;
			
			for (i=0;i<lpQuestText->m_wRequireItemCount;i++)
			{
				int		iItem		=	lpQuestText->m_pRequireItemList[i*2];
				int		iItemCount	=	lpQuestText->m_pRequireItemList[i*2+1];
				int		iCount		=	g_hero.getItemCountInInventory(iItem);
				DWORD	dwColor		=	WHITE;
				
				if (iCount	>=	iItemCount)
					dwColor	=	LTGREEN;
				
				s_text.put(iX+iTextFrontGab,iY,WHITE,"%s - %d/<c:LTYELLOW>%d<n>",g_aBasicItem[iItem].m_strName,iCount,iItemCount);
				iY	+=	iLineSize;
			}
			
			iY	+=	iContentsGab;
		}
		
		s_text.put(iX,iY,CTBLUE,dMSG_QUEST_INFO);
		iY	+=	(dQW_PROCESS_QUEST_LIST_GAP-6);
		g_sprInterface2.Put(iX,iY,eQW_NEW_LINE);
		iY	+=	4;
		
		char	*lpstrExplain	=	lpSelectQuest->getExplain(iProcessLevel,lpQuestInfo->isComplete);
		
		s_text.put(iX+iTextFrontGab,iY,iCommentWidth,iWindowHeight,0,WHITE,lpstrExplain);
		
		if	(m_iSelectQuest	!=	0xffff)
		{
			iX	=	m_posQuestWindow.x+(iWindowWidth-6)/2;
			iY	=	m_posQuestWindow.y+iWindowHeight-9-16 - 32;
			
			//시간 정보가 필요한 퀘스트다.
			if(g_hero.m_awQuestCountDownTimer[m_iSelectQuest] != 0xffff)
			{			
				s_text.cPut(iX,iY,LTYELLOW,_ms(dMSG_QUEST_REMAIN_TIME , 
					g_hero.m_awQuestCountDownTimer[m_iSelectQuest] / 60, 
					g_hero.m_awQuestCountDownTimer[m_iSelectQuest] % 60));
			}
			
			CQuestItem	*lpQuest		=	g_hero.getQuest(m_iSelectQuest);
			
			if (lpQuest && lpQuest->m_isEnableCancel)
				s_bmButton.draw();
			else
			{
				s_text.cPut(iX,iY + 32,LTYELLOW,dMSG_CAN_NOT_CANCEL_QUEST);
			}
		}
	}
	if (m_iSelectTab == eQW_TAB_HISTORY)
	{
		s_text.pushShadowStatus(FALSE);
		
		int	iWindowWidth = g_sprInterface2.getSpriteWidth(eWINDOW_BIG)-14;
		cRECT rtPut={s_rectInterfaceSize.x1+12, m_posQuestWindow.y+400, s_rectInterfaceSize.x2-10, m_posQuestWindow.y+470};
		
		s_bmCategory.draw();		// 라디오버튼 출력
		m_sbScrollBar.draw();		// 스크롤바 출력
		
		if (m_iSelectRadioBtn == eQM_MAIN_QUEST)
		{
			DrawMainQuestHistory(&m_rtQuestListArea); // 메인정보
			DrawQuestSimpleInfo(&rtPut, eQT_NORMAL, m_iSelectHistoryQuest);		// 하단 정보
		}
		
		if (m_iSelectRadioBtn == eQM_NORMAL_QUEST)
		{
			DrawNormalQuestHistory(&m_rtQuestListArea); // 메인정보
			DrawHistoryFilter(&m_rtHistoryFilter); // 필터
			DrawQuestSimpleInfo(&rtPut, m_iSelectNormalQuestType, m_iSelectNormalQuest);		// 하단 정보
		}
		
		if (m_iSelectRadioBtn == eQM_TITLE_QUEST)
		{
			DrawTitleQuestHistory(&m_rtQuestListArea); // 메인정보
			DrawHistoryFilter(&m_rtHistoryFilter); // 필터
			DrawQuestSimpleInfo(&rtPut, m_iTitleQuestSelectQuestType, m_iTitleQuestSelectQuest);		// 하단 정보
		}
		
		s_text.popShadowStatus();
	}
}

void
CQuestWindow::selectTab(int _iTab)
{
	int	iSelectQuest	=	-1;

	if (_iTab != eQW_TAB_HISTORY)
	{
		for (int	i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
		{
			if	(g_hero.m_aProcessQuest[i].isActive	==	FALSE		)
				continue;
			
			tsProcessQuestField	*lpQuestInfo	=	&g_hero.m_aProcessQuest[i];
			int					iQuestIndex	=	lpQuestInfo->questIndex;
			CQuestItem			*lpQuest;
			
			if	(lpQuestInfo->questKind	==	eQT_NORMAL	)
				lpQuest	=	g_quest.getQuest(iQuestIndex);
			if	(lpQuestInfo->questKind	==	eQT_ARBEIT	)
				lpQuest	=	g_questArbeit.getQuest(iQuestIndex);
			
			if	(_iTab	==	eSQT_MAIN)
			{
				if	(lpQuest->m_wQuestKind	==	eQK_MAIN_STORY)
				{
					iSelectQuest	=	i;
					break;
				}
			}
			else
			{
				if	(lpQuest->m_wQuestKind	!=	eQK_MAIN_STORY)
				{
					iSelectQuest	=	i;
					break;
				}
			}
		}
		
		if	(iSelectQuest	==	-1)
		{
			m_iSelectTab = eQW_TAB_NORMAL;
			return;
		}
		
		m_iSelectQuest	=	iSelectQuest;
		m_iSelectTab	=	_iTab;
	}
	if (_iTab == eQW_TAB_HISTORY)
	{
		m_iSelectTab	=	_iTab;
		cRECT rtTemp = {s_rectInterfaceSize.x1+12, m_posQuestWindow.y+32, s_rectInterfaceSize.x2-100, m_posQuestWindow.y+45};		
		setRadioBtn(&rtTemp);
	}
}


void
CQuestWindow::selectQuest(int _iQuestSlot)
{
	m_iSelectQuest	=	_iQuestSlot;

	CQuestItem	*lpQuest;
	int			iQuestIndex	=	g_hero.m_aProcessQuest[_iQuestSlot].questIndex;

	if	(g_hero.m_aProcessQuest[_iQuestSlot].questKind	==	eQT_NORMAL	)
		lpQuest	=	g_quest.getQuest(iQuestIndex);
	else
		lpQuest	=	g_questArbeit.getQuest(iQuestIndex);

	if	(lpQuest->m_wQuestKind	==	eQK_MAIN_STORY)
		m_iSelectTab			=	eSQT_MAIN;
}
//
// 업데이트
void
CQuestWindow::update()
{
	int		i;

	if	(s_iPopupInterface	!=	ePIW_QUEST)
		return;

	int	iQuestCount	=	g_hero.getProcessQeustCount();

	if	(m_iSelectQuest	<	0 || m_iSelectQuest >=	dMAX_PROCESS_QUEST_COUNT)
		for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
		{
			if	(g_hero.m_aProcessQuest[i].isActive	==	FALSE		)
				continue;

			selectQuest(i);
			break;
		}

	m_iFocusTab		=	getFocusedTab();
	m_iFocusQuest	=	getFocusedQuest();

	if	(s_isTouchLeftButton)
	{
		if	(m_iFocusQuest	!=	0xffff)
			selectQuest(m_iFocusQuest);

		if	(m_iFocusTab	!=	-1)
			selectTab(m_iFocusTab);
	}

	if (m_iSelectTab == eQW_TAB_HISTORY)
	{
		BOOL bdlmenuActive = m_dlmenu.isActive();
		int idlmenuSelectList = m_dlmenu.getSelectList();
		m_dlmenu.update(s_posMouse.x, s_posMouse.y, s_isTouchLeftButton, s_isClickedLeftButton);
		if (idlmenuSelectList != m_dlmenu.getSelectList())
				setScrollBarSize(&m_rtQuestListArea);

		if (!bdlmenuActive)
		{			
			DWORD dwSelectMenu	=	s_bmCategory.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed,TRUE);
			if (dwSelectMenu != -1)
			{			
				m_iSelectRadioBtn =	dwSelectMenu;
				s_bmCategory.check(m_iSelectRadioBtn, TRUE);
				setScrollBarSize(&m_rtQuestListArea);
			}
			
			m_sbScrollBar.update(cINPDEV::s_posMouse.x, cINPDEV::s_posMouse.y, cINPDEV::s_aKey, cINPDEV::s_isTouchLeftButton, cINPDEV::s_isLeftButtonPressed);
			
			if (m_rtQuestListArea.isIn(cINPDEV::s_posMouse.x, cINPDEV::s_posMouse.y) && cINPDEV::s_iWheelValue<0)
				m_sbScrollBar.setPos(m_sbScrollBar.m_iCurrentPos+=1);
			if (m_rtQuestListArea.isIn(cINPDEV::s_posMouse.x, cINPDEV::s_posMouse.y) && cINPDEV::s_iWheelValue>0 && m_sbScrollBar.m_iCurrentPos>0)
				m_sbScrollBar.setPos(m_sbScrollBar.m_iCurrentPos-=1);
			
			if (m_iSelectRadioBtn == eQM_MAIN_QUEST)
			{
				FindMainQuestChapterBtn();
				UpdateMainQuestChapterBtn();
				FindMainQuestPartBtn();
				UpdateMainQuestPartBtn();
				UpdateMainQuestIndexBtn();
			}
			if (m_iSelectRadioBtn == eQM_NORMAL_QUEST)
			{			
				FindNormalQuestBtn();
				UpdateNormalQuestBtn();			
			}
			if (m_iSelectRadioBtn == eQM_TITLE_QUEST)
			{
				FindTitleQuestDropDownBtn();
				UpdateTitleQuestDropDownBtn();
				UpdateTitleQuestQuest();
			}
		}
	}

	if	(m_iSelectQuest	!=	0xffff)
	{
		CQuestItem	*lpQuest		=	g_hero.getQuest(m_iSelectQuest);

		if	(lpQuest && lpQuest->m_isEnableCancel)
		{
			DWORD	dwSelectMenu	=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

			if	(s_bmButton.isPressed())
				cINPDEV::SetExclusive(eMOUSE_STATE_FRIEND_WINDOW_BUTTONS);
			else
				cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_FRIEND_WINDOW_BUTTONS);

			if (dwSelectMenu	==	1)
			{
				CQuest				*lpQuestManager;
				tsProcessQuestField	*lpQuestInfo	=	&g_hero.m_aProcessQuest[m_iSelectQuest];

				if	(lpQuestInfo->questKind	==	eQT_NORMAL	)
					lpQuestManager	=	&g_quest;
				if	(lpQuestInfo->questKind	==	eQT_ARBEIT	)
					lpQuestManager	=	&g_questArbeit;

				CQuestItem	*lpQuest	=	lpQuestManager->getQuest(lpQuestInfo->questIndex);

				g_msgBox.cPopup("",dMSG_ARE_YOU_REALLY_CANCEL_THAT_QUEST,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_CANCEL_SELECT_QUEST,eMBI_CANCEL);
			}
		}
	}

	if	(m_iSelectQuest	>=	0 && m_iSelectQuest < dMAX_PROCESS_QUEST_COUNT)
		if	(g_hero.m_aProcessQuest[m_iSelectQuest].isActive	==	FALSE)
			m_iSelectQuest	=	-1;
}

BOOL
CQuestWindow::isOpened()
{
	if (s_iPopupInterface	==	ePIW_QUEST)	
		return TRUE;

	return	FALSE;
}





// insu function add
//┌---------------------------------------------------------------┐
//│ 일반퀘스트의 퀘스트 마우스 위치를 찾음                        │
//└---------------------------------------------------------------┘
void
CQuestWindow::FindNormalQuestBtn()
{
	int isize = m_vtQuestArea.size();
	if (!isize)
		return;
	
	for(int i=0 ; i<isize ; i++)
	{
		if (m_vtQuestArea[i].isIn(s_posMouse.x,s_posMouse.y))
		{
			m_iFocusNormalQuest = i;
			return;
		}
	}
	m_iFocusNormalQuest = -1;
}
// insu function add end



// insu function add
//┌---------------------------------------------------------------┐
//│ 일반퀘스트의 퀘스트 클릭 이벤트처리                           │
//└---------------------------------------------------------------┘
void
CQuestWindow::UpdateNormalQuestBtn()
{
	int isize = m_vtQuestArea.size();
	if (!isize)
		return;
	
	if (s_isTouchLeftButton)
	{
		if (m_iFocusNormalQuest != -1)
		{
			m_iSelectNormalQuest = m_vtQuestIndex[m_iFocusNormalQuest];
			m_iSelectNormalQuestType = m_vtQuestType[m_iFocusNormalQuest];
		}
	}
}
// insu function add end




// insu function add
//┌---------------------------------------------------------------┐
//│ 메인퀘스트의 퀘스트를 클릭했을 때 이벤트처리                  │
//└---------------------------------------------------------------┘
void
CQuestWindow::UpdateMainQuestIndexBtn()
{
	int isize = m_vtQuestArea.size();
	if (!isize)
		return;
	
	if (s_isTouchLeftButton)
	{
		for(int i=0 ; i<isize ; i++)
		{
			if (m_vtQuestArea[i].isIn(s_posMouse.x,s_posMouse.y))
			{
				m_iSelectHistoryQuest = m_vtQuestIndex[i];
				return;
			}
		}
	}
}
// insu function add end


// insu function add
//┌---------------------------------------------------------------┐
//│ 스크롤바 사이즈 셋팅                                          │
//│                                                               │
//│ cRECT* _rtPut       :  퀘스트리스트 출력 사각 영역            │
//└---------------------------------------------------------------┘
void
CQuestWindow::setScrollBarSize(cRECT* _rtPut)
{	
	CQuest	*lpQuestManager = &g_quest;
	int iTotalLineCnt = 0;
	if (m_iSelectRadioBtn == eQM_MAIN_QUEST)
	{
		if (g_hero.m_wProcessChapterIndex == 0xffff || g_hero.m_wProcessHighQuestIndex == 0xffff)
		{
			m_sbScrollBar.setSize(0, 0);
			m_sbScrollBar.setPos(0);
			return;
		}
		
		for(int i=0 ; i<g_hero.m_wProcessChapterIndex+1 ; i++)
		{
			int iIcon = eFWI_PLUS;
			if (m_bChapterBtnState[i])
				iIcon = eFWI_MINUS;
			iTotalLineCnt++;
			if (iIcon == eFWI_MINUS)
			{
				for(int j=0 ; j<g_chapterManager.getPartCount(i); j++)
				{
					CQuestItem	*lpQuestItem = lpQuestManager->getQuest(m_iProcessMainQuestIndex);

					if (i == g_hero.m_wProcessChapterIndex && j == g_hero.m_wProcessHighQuestIndex+1)
					{
						m_sbScrollBar.setSize(iTotalLineCnt, (_rtPut->getHeight()-20)/iYgap);
						return;
					}

					iIcon = eFWI_PLUS;
					if (m_bPartBtnState[i][j])
						iIcon = eFWI_MINUS;
					iTotalLineCnt++;
					if (iIcon == eFWI_MINUS)
					{
						for(int k=0 ; k<lpQuestManager->GetQuestCount() ; k++)
						{
							CQuestItem	*lpQuestItem = lpQuestManager->getQuest(k);
							if (lpQuestItem->m_bf6ChapterNumber && lpQuestItem->m_bf6PartNumber && lpQuestItem->m_bf6ChapterNumber == i+1 && lpQuestItem->m_bf6PartNumber == j+1 && g_hero.isProcessQuest(eQT_NORMAL, k))
							{
								iTotalLineCnt++;
								break;
							}
							if (lpQuestItem->m_bf6ChapterNumber && lpQuestItem->m_bf6PartNumber && lpQuestItem->m_bf6ChapterNumber == i+1 && lpQuestItem->m_bf6PartNumber == j+1)
							{
								iTotalLineCnt++;
							}
						}
					}
				}
			}
		}
		m_sbScrollBar.setSize(iTotalLineCnt, (_rtPut->getHeight()-20)/iYgap);
	}
	if (m_iSelectRadioBtn != eQM_MAIN_QUEST)
	{
		m_iNormalQuestCnt			= 0;
		m_iTitleQuestCnt			= 0;
		m_iNormalQuestCompleteCnt	= 0;
		m_iTitleQuestCompleteCnt	= 0;
		m_iSkillQuestCnt			= 0;
		m_iSkillQuestCompleteCnt	= 0;

		int iQuestCount = lpQuestManager->GetQuestCount();

		for(int i=0 ; i<iQuestCount ; i++)
		{
			CQuestItem	*lpQuestItem = lpQuestManager->getQuest(i);
			if (m_iSelectRadioBtn == eQM_NORMAL_QUEST)
			{
				if (lpQuestItem->m_wQuestKind != eQK_NORMAL)
					continue;
			}
			if (m_iSelectRadioBtn == eQM_TITLE_QUEST)
			{
				if (lpQuestItem->m_wQuestKind == eQK_NORMAL || lpQuestItem->m_wQuestKind == eQK_MAIN_STORY || lpQuestItem->m_wQuestKind == eQK_GUILD)
					continue;
			}
			if (lpQuestItem->m_wBeginNPC_Field == 0xffff) // 이 옵션으로 활성화된 퀘스트인지 아닌지 체크한다.
				continue;
			if (!lpQuestItem->isEnableJob(g_hero.m_wJob)) // 현재 직업에 맞는 퀘스트만 출력한다.
				continue;
			if (g_hero.m_iLevel < lpQuestItem->m_wMinLimitLevel) // 최소수행레벨 보다 클때 진행
				continue;
			if (lpQuestItem->m_bf1IsDisableQuest) // 보여질 퀘스트만 출력
				continue;
			if (lpQuestItem->m_bf6ChapterNumber || lpQuestItem->m_bf6PartNumber)
				continue;

			if (m_iSelectRadioBtn == eQM_NORMAL_QUEST)
			{
				m_iNormalQuestCnt++;
				if (g_hero.isCompleteQuest(eQT_NORMAL, i))
					m_iNormalQuestCompleteCnt++;
			}
			if (m_iSelectRadioBtn == eQM_TITLE_QUEST && lpQuestItem->m_wQuestKind == eQK_TITLE)
			{				
				m_iTitleQuestCnt++;
				if (g_hero.isCompleteQuest(eQT_NORMAL, i))
					m_iTitleQuestCompleteCnt++;
			}
			if (m_iSelectRadioBtn == eQM_TITLE_QUEST && lpQuestItem->m_wQuestKind == eQK_SKILL_MASTER)
			{
				m_iSkillQuestCnt++;
				if (g_hero.isCompleteQuest(eQT_NORMAL, i))
					m_iSkillQuestCompleteCnt++;
			}
		}

		lpQuestManager = &g_questArbeit;
		iQuestCount = lpQuestManager->GetQuestCount();

		for(i=0 ; i<iQuestCount ; i++)
		{
			CQuestItem	*lpQuestItem = lpQuestManager->getQuest(i);
			if (m_iSelectRadioBtn == eQM_NORMAL_QUEST)
			{
				if (lpQuestItem->m_wQuestKind != eQK_NORMAL)
					continue;
			}
			if (m_iSelectRadioBtn == eQM_TITLE_QUEST)
			{
				if (lpQuestItem->m_wQuestKind == eQK_NORMAL || lpQuestItem->m_wQuestKind == eQK_MAIN_STORY || lpQuestItem->m_wQuestKind == eQK_GUILD)
					continue;
			}
			if (lpQuestItem->m_wBeginNPC_Field == 0xffff) // 이 옵션으로 활성화된 퀘스트인지 아닌지 체크한다.
				continue;
			if (!lpQuestItem->isEnableJob(g_hero.m_wJob)) // 현재 직업에 맞는 퀘스트만 출력한다.
				continue;
			if (g_hero.m_iLevel < lpQuestItem->m_wMinLimitLevel) // 최소수행레벨 보다 클때 진행
				continue;
			if (lpQuestItem->m_bf1IsDisableQuest) // 보여질 퀘스트만 출력
				continue;
			if (lpQuestItem->m_bf6ChapterNumber || lpQuestItem->m_bf6PartNumber)
				continue;

			if (m_iSelectRadioBtn == eQM_NORMAL_QUEST)
			{
				m_iNormalQuestCnt++;
				if (g_hero.isCompleteQuest(eQT_ARBEIT, i))
					m_iNormalQuestCompleteCnt++;
			}
			if (m_iSelectRadioBtn == eQM_TITLE_QUEST && lpQuestItem->m_wQuestKind == eQK_TITLE)
			{				
				m_iTitleQuestCnt++;
				if (g_hero.isCompleteQuest(eQT_ARBEIT, i))
					m_iTitleQuestCompleteCnt++;
			}
			if (m_iSelectRadioBtn == eQM_TITLE_QUEST && lpQuestItem->m_wQuestKind == eQK_SKILL_MASTER)
			{
				m_iSkillQuestCnt++;
				if (g_hero.isCompleteQuest(eQT_ARBEIT, i))
					m_iSkillQuestCompleteCnt++;
			}
		}

		int iCnt = m_iNormalQuestCnt+1;
		if (m_iSelectRadioBtn == eQM_NORMAL_QUEST)
		{			
			if (m_dlmenu.getSelectList() == eDL_DO_NOT_COMPLETE)
				iCnt -= m_iNormalQuestCompleteCnt;
			if (m_dlmenu.getSelectList() == eDL_COMPLETE)
				iCnt = m_iNormalQuestCompleteCnt+1;
			
			m_sbScrollBar.setSize(iCnt, (_rtPut->getHeight()-20)/iYgap);
		}

		// 칭호퀘스트의 경우 드롭다운버튼이 참인지 거짓인지 확인한뒤 스크롤바 셋팅한다.
		iCnt = 3;
		if (m_bDropDownBtnState[1])
		{
			iCnt += m_iSkillQuestCnt;
			if (m_dlmenu.getSelectList() == eDL_DO_NOT_COMPLETE)
				iCnt -= m_iSkillQuestCompleteCnt;
			if (m_dlmenu.getSelectList() == eDL_COMPLETE)
				iCnt = m_iSkillQuestCompleteCnt+3;
		}
		if (m_bDropDownBtnState[2])
		{
			iCnt += m_iTitleQuestCnt;
			if (m_dlmenu.getSelectList() == eDL_DO_NOT_COMPLETE)
				iCnt -= m_iTitleQuestCompleteCnt;
			if (m_dlmenu.getSelectList() == eDL_COMPLETE)
				iCnt = m_iTitleQuestCompleteCnt+3;
		}
		if (!m_bDropDownBtnState[0])
			iCnt = 3;
		if (m_iSelectRadioBtn == eQM_TITLE_QUEST)
			m_sbScrollBar.setSize(iCnt, (_rtPut->getHeight()-20)/iYgap);		
	}
	// m_sbScrollBar.setPos(0);
}

// insu function add end



// insu function add
//┌---------------------------------------------------------------┐
//│ 마우스가 메인퀘스트의 파트버튼의 어디에 있는지 찾는다.        │
//└---------------------------------------------------------------┘
void
CQuestWindow::FindMainQuestPartBtn()
{
	int isize = m_vtPartArea.size();
	if (!isize)
		return;

	for (int i=0 ; i<isize ; i++)
	{
		if (m_vtPartArea[i].isIn(s_posMouse.x,s_posMouse.y))
		{
			m_iFocusPartBtn = i;
			return;
		}
	}
	m_iFocusPartBtn = -1;	// 아무것도 없을 때
}
// insu function add end




// insu function add
//┌---------------------------------------------------------------┐
//│ 메인퀘스트의 파트버튼을 클릭했을 때 이벤트처리                │
//└---------------------------------------------------------------┘
void
CQuestWindow::UpdateMainQuestPartBtn()
{
	int isize = m_vtPartArea.size();
	
	if (m_iFocusPartBtn == -1)
		return;
	
	if (!isize)
		return;
	
	if (s_isTouchLeftButton)
	{
		m_bPartBtnState[m_vtChapterofPartIndex[m_iFocusPartBtn]][m_vtPartIndex[m_iFocusPartBtn]] = 1-m_bPartBtnState[m_vtChapterofPartIndex[m_iFocusPartBtn]][m_vtPartIndex[m_iFocusPartBtn]];
		setScrollBarSize(&m_rtQuestListArea);
	}
}
// insu function add end




// insu function add
//┌---------------------------------------------------------------┐
//│ 마우스가 메인퀘스트의 챕터버튼의 어디에 있는지 찾는다.        │
//└---------------------------------------------------------------┘
void
CQuestWindow::FindMainQuestChapterBtn()
{
	int isize = m_vtChapterArea.size();
	if (!isize)
		return;

	for (int i=0 ; i<isize ; i++)
	{
		if (m_vtChapterArea[i].isIn(s_posMouse.x,s_posMouse.y))
		{
			m_iFocusChapterBtn = i;
			return;
		}
	}
	m_iFocusChapterBtn = -1;	// 아무것도 없을 때
}
// insu function add end


// insu function add
//┌---------------------------------------------------------------┐
//│ 메인퀘스트의 챕터버튼을 클릭했을 때 이벤트처리                │
//└---------------------------------------------------------------┘
void
CQuestWindow::UpdateMainQuestChapterBtn()
{
	int isize = m_vtChapterArea.size();

	if (m_iFocusChapterBtn == -1)
		return;
	
	if (!isize)
		return;

	if (s_isTouchLeftButton)
	{
		m_bChapterBtnState[m_vtChapterIndex[m_iFocusChapterBtn]] = 1-m_bChapterBtnState[m_vtChapterIndex[m_iFocusChapterBtn]];
		setScrollBarSize(&m_rtQuestListArea);
	}
}
// insu function add end




// insu function add
//┌---------------------------------------------------------------┐
//│ 메인 퀘스트를 그룹별로 정리한다.                              │
//└---------------------------------------------------------------┘
void
CQuestWindow::setMainQuestGroup()
{
	CQuest	*lpQuestManager = &g_quest;
	int iQuestCount		= lpQuestManager->GetQuestCount();
	
	int iChapter = -1;
	int iPart = -1;

	WORD wQuarterCnt[c_iMaxChapterCount][c_iMaxHighQuestCount];
	memset(wQuarterCnt, 0, sizeof(WORD)*c_iMaxChapterCount*c_iMaxHighQuestCount);
	
	for(int i=0 ; i<iQuestCount ; i++)
	{
		CQuestItem	*lpQuestItem = lpQuestManager->getQuest(i);
		if (lpQuestItem->m_wQuestType == eQT_NORMAL && lpQuestItem->m_bf6ChapterNumber && lpQuestItem->m_bf6PartNumber)
		{
			if (!lpQuestItem->m_bf3Quarter)
				m_wMainQuestTotalCount[lpQuestItem->m_bf6ChapterNumber-1][lpQuestItem->m_bf6PartNumber-1]++;
			if (m_iProcessMainQuestIndex == i)
			{
				iChapter = lpQuestItem->m_bf6ChapterNumber;
				iPart = lpQuestItem->m_bf6PartNumber;
			}
		}
	}

	for(i=0 ; i<c_iMaxChapterCount ; i++)
	{
		for(int j=0 ; j<c_iMaxHighQuestCount ; j++)
		{
			if (wQuarterCnt[i][j] == 1)
				m_wMainBonusQuestCount[i][j]++;
		}
	}

	// 현재 진행중인 메인퀘스트 파트 내 퀘스트갯수를 구한다.
	for(i=0 ; i<iQuestCount ; i++)
	{
		CQuestItem	*lpQuestItem = lpQuestManager->getQuest(i);
		if (iChapter == lpQuestItem->m_bf6ChapterNumber && iPart == lpQuestItem->m_bf6PartNumber)
		{			
			if (i == m_iProcessMainQuestIndex)
				break;
			m_iProcessQuestPartInQuestCount++;
		}
	}
}

// insu function add end




// insu function add
//┌---------------------------------------------------------------┐
//│ 메인 퀘스트 네이밍규칙에 따라 앞번호를 리턴한다.              │
//└---------------------------------------------------------------┘
int
CQuestWindow::getPartQuestNum(char* str)
{
	int iFirstNum = atoi(&str[0]);
	int iSecondNum = atoi(&str[1]);

	if (!iSecondNum)
		if (str[1] == '.')
			return iFirstNum;
	if (iSecondNum && iSecondNum > 0 && iSecondNum < 10)
	{
		char Buffer[3];
		memset(Buffer, '\0', 3);
		strncpy(Buffer, str, 2);
		return atoi(Buffer);
	}
	return -1;
}
// insu function add end




// insu function add
//┌---------------------------------------------------------------┐
//│ 칭호퀘스트의 퀘스트 버튼을 클릭했을 때 이벤트처리             │
//└---------------------------------------------------------------┘
void
CQuestWindow::UpdateTitleQuestQuest()
{
	int isize = m_vtQuestArea.size();
	if (!isize)
		return;

	if (s_isTouchLeftButton)
	{
		for(int i=0 ; i<isize ; i++)
		{
			if (m_vtQuestArea[i].isIn(s_posMouse.x,s_posMouse.y))
			{
				m_iTitleQuestSelectQuest = m_vtQuestIndex[i];
				m_iTitleQuestSelectQuestType = m_vtQuestType[i];
				return;
			}
		}
	}
}
// insu function add end



// insu function add
//┌---------------------------------------------------------------┐
//│ 마우스가 칭호퀘스트의 드롭다운 버튼의 어디에 있는지 찾는다.   │
//└---------------------------------------------------------------┘
void
CQuestWindow::FindTitleQuestDropDownBtn()
{
	int isize = m_vtDropDownBtnArea.size();
	if (!isize)
		return;

	for (int i=0 ; i<isize ; i++)
	{
		if (m_vtDropDownBtnArea[i].isIn(s_posMouse.x,s_posMouse.y))
		{
			m_iFocusDropDownBtn = i;
			return;
		}
	}
	m_iFocusDropDownBtn = -1;	// 아무것도 없을 때
}
// insu function add end




// insu function add
//┌---------------------------------------------------------------┐
//│ 칭호퀘스트의 드롭다운 버튼을 클릭했을 때 이벤트처리           │
//└---------------------------------------------------------------┘
void
CQuestWindow::UpdateTitleQuestDropDownBtn()
{
	int isize = m_vtDropDownBtnArea.size();

	if (m_iFocusDropDownBtn == -1)
		return;
	
	if (!isize)
		return;

	if (s_isTouchLeftButton)
	{
		m_bDropDownBtnState[m_iFocusDropDownBtn] = 1-m_bDropDownBtnState[m_iFocusDropDownBtn];
		setScrollBarSize(&m_rtQuestListArea);
	}
}
// insu function add end



// insu function add
//┌---------------------------------------------------------------┐
//│ 칭호 퀘스트의 히스토리를 출력한다.                            │
//│                                                               │
//│ cRECT* _rtPut       :  출력 사각 영역                         │
//└---------------------------------------------------------------┘
void
CQuestWindow::DrawTitleQuestHistory(cRECT* _rtPut)
{
	// set
	m_vtDropDownBtnArea.clear();
	m_vtQuestArea.clear();
	m_vtQuestIndex.clear();
	m_vtQuestType.clear();

	int iX = _rtPut->x1+10;
	int iY = _rtPut->y1+10;
	int iPutStartPoint = m_sbScrollBar.m_iCurrentPos;	// 이 값이 0이 되면 출력하기 시작한다.
	BOOL bDropDownArea = (BOOL)m_vtDropDownBtnArea.size();		// 이 값이 거짓이면 퀘스트버튼의 영역을 다시 설정한다.

	// draw
	s_ttCommon.drawFrame(_rtPut, FALSE);
	
	int iIcon = eFWI_PLUS;
	if (m_bDropDownBtnState[0] == TRUE)
		iIcon = eFWI_MINUS;
	
	if (!bDropDownArea && !iPutStartPoint)
	{
		cRECT rtTemp = {iX, iY, _rtPut->x2-30, iY+17};
		m_vtDropDownBtnArea.push_back(rtTemp);
	}
	else
	{
		cRECT rtTemp = {-1, -1, -1, -1};
		m_vtDropDownBtnArea.push_back(rtTemp);
	}

	if (!iPutStartPoint)
	{
		g_sprInterface2.Put(iX+8, iY+7, iIcon);
		s_text.put(iX+20, iY+2, WHITE, "%s %s", dMSG_TITLE, dMSG_WM_QUEST);
		iY += iYgap;
	}
	if (iPutStartPoint)
			iPutStartPoint--;

	if (iY+iYgap > _rtPut->y2-10)
		return;

	if (iIcon == eFWI_MINUS)
	{
		// 스킬 마스터 부분
		iIcon = eFWI_PLUS;
		if (m_bDropDownBtnState[1] == TRUE)
			iIcon = eFWI_MINUS;

		if (!bDropDownArea && !iPutStartPoint)
		{
			cRECT rtTemp = {iX+10, iY, _rtPut->x2-30, iY+17};
			m_vtDropDownBtnArea.push_back(rtTemp);
		}
		else
		{
			cRECT rtTemp = {-1, -1, -1, -1};
			m_vtDropDownBtnArea.push_back(rtTemp);
		}

		if (!iPutStartPoint)
		{
			g_sprInterface2.Put(iX+18, iY+7, iIcon);
			s_text.put(iX+30, iY+2, WHITE, "%s %s", dMSG_SKILL_MASTER, dMSG_WM_QUEST);
			iY += iYgap;
		}
		
		
		if (iPutStartPoint)
			iPutStartPoint--;

		if (iY+iYgap > _rtPut->y2-10)
			return;

		if (iIcon == eFWI_MINUS)
		{
			DrawTitleQuestHistoryDetail(_rtPut, &iX, &iY, eQK_SKILL_MASTER, &iPutStartPoint, bDropDownArea);
		}

		// 공통 칭호 부분
		iIcon = eFWI_PLUS;
		if (m_bDropDownBtnState[2] == TRUE)
			iIcon = eFWI_MINUS;

		if (!bDropDownArea && !iPutStartPoint)
		{
			cRECT rtTemp = {iX+10, iY, _rtPut->x2-30, iY+17};
			m_vtDropDownBtnArea.push_back(rtTemp);
		}

		if (!iPutStartPoint)
		{
			g_sprInterface2.Put(iX+18, iY+7, iIcon);
			s_text.put(iX+30, iY+2, WHITE, "%s %s %s", dMSG_COMMON, dMSG_TITLE, dMSG_WM_QUEST);
			iY += iYgap;
		}
		else
		{
			cRECT rtTemp = {-1, -1, -1, -1};
			m_vtDropDownBtnArea.push_back(rtTemp);
		}

		if (iPutStartPoint)
			iPutStartPoint--;

		if (iY+iYgap > _rtPut->y2-10)
			return;

		if (iIcon == eFWI_MINUS)
		{
			DrawTitleQuestHistoryDetail(_rtPut, &iX, &iY, eQK_TITLE, &iPutStartPoint, bDropDownArea);
		}
	}
}
// insu function add end


// insu function add
//┌---------------------------------------------------------------┐
//│ DrawTitleQuestHistory() 안에 실제 퀘스트리스트를 출력한다.    │
//│ 메인퀘스트 파일 안에도 칭호퀘스트가 소수 존재하기 때문에      │
//│ 두번 체크해야 한다.                                           │
//│ 관련 변수는 일반퀘스트를 출력할때 쓰던 변수들을 사용한다.     │
//│ m_vtQuestArea, m_vtQuestIndex, m_vtQuestType                  │
//│                                                               │
//│ cRECT* _rtPut       :  출력 사각 영역                         │
//│ int* _iY            :  y축 출력 지점                          │
//│ int _iQuestKind     :  퀘스트종류(스킬이나 칭호?)             │
//│                (다른종류가 들어와도 상관없지만 여기선 불필요) │
//│ int* _iPutStartPoint:  스크롤바에 따른 출력지점               │
//│ BOOL _bDropDownArea :  다시 영역을 설정할꺼냐?!               │ 
//└---------------------------------------------------------------┘
void
CQuestWindow::DrawTitleQuestHistoryDetail(cRECT* _rtPut, int* _iX, int* _iY, int _iQuestKind, int* _iPutStartPoint, BOOL _bDropDownArea)
{
	CQuest	*lpQuestManager = &g_quest;
	for(int i=0 ; i<lpQuestManager->GetQuestCount() ; i++)
	{
		CQuestItem	*lpQuestItem = lpQuestManager->getQuest(i);
		if (_iQuestKind == eQK_SKILL_MASTER)
			if (lpQuestItem->m_wQuestKind != eQK_SKILL_MASTER)
				continue;
		if (_iQuestKind == eQM_TITLE_QUEST)
			if (lpQuestItem->m_wQuestKind != eQK_TITLE)
				continue;
		if (lpQuestItem->m_wBeginNPC_Field == 0xffff) // 이 옵션으로 활성화된 퀘스트인지 아닌지 체크한다.
			continue;
		if (!lpQuestItem->isEnableJob(g_hero.m_wJob)) // 현재 직업에 맞는 퀘스트만 출력한다.
			continue;
		if (g_hero.m_iLevel < lpQuestItem->m_wMinLimitLevel) // 최소수행레벨 보다 클때 진행
			continue;
		if (lpQuestItem->m_bf1IsDisableQuest) // 보여질 퀘스트만 출력
			continue;
		if (m_dlmenu.getSelectList() == eDL_DO_NOT_COMPLETE && g_hero.isCompleteQuest(eQT_NORMAL, i))
			continue;
		if (m_dlmenu.getSelectList() == eDL_COMPLETE && !g_hero.isCompleteQuest(eQT_NORMAL, i))
			continue;

		DWORD dwColor = dCOMPLETE_QUEST_COLOR;
		char* str = dMSG_COMPLETE;
		if (!g_hero.isCompleteQuest(eQT_NORMAL, i))
		{
			dwColor = dDO_NOT_COMPLETE_QUEST_COLOR; 
			str = dMSG_DO_NOT_COMPLETE;
		}
		if (g_hero.isProcessQuest(eQT_NORMAL, i))
		{
			dwColor = dPROCESS_QUEST_COLOR;
			str = dMSG_PROCESS;
		}
		if (m_iTitleQuestSelectQuestType == eQT_NORMAL && m_iTitleQuestSelectQuest == i)
			dwColor = dSELECT_QUEST_COLOR;
		
		if(!_bDropDownArea && !*_iPutStartPoint)
		{
			cRECT rtTemp = {*_iX+30, *_iY, _rtPut->x2-30, *_iY+17};
			m_vtQuestArea.push_back(rtTemp);
			m_vtQuestIndex.push_back(i);
			m_vtQuestType.push_back(eQT_NORMAL);
		}

		if (!*_iPutStartPoint)
		{
			cRECT rtTemp = {*_iX+30, *_iY, _rtPut->x2-30, *_iY+17};
			if (dwColor != LTYELLOW && rtTemp.isIn(s_posMouse.x,s_posMouse.y))
				dwColor = dFOCUS_QUEST_COLOR;
			s_text.put(*_iX+30, *_iY+2, dwColor, "%s (%s)", lpQuestItem->getQuestName(), str);
			*_iY += iYgap;
			if (m_iTitleQuestSelectQuest == -1)
			{
				m_iTitleQuestSelectQuest = i;
				m_iTitleQuestSelectQuestType = eQT_NORMAL;
			}
		}
		if (*_iPutStartPoint)
		{
			(*_iPutStartPoint)--;
			continue;
		}
		
		if (*_iY+iYgap > _rtPut->y2-10)
			return;
	}
	lpQuestManager = &g_questArbeit;
	for(i=0 ; i<lpQuestManager->GetQuestCount() ; i++)
	{
		CQuestItem	*lpQuestItem = lpQuestManager->getQuest(i);
		if (_iQuestKind == eQK_SKILL_MASTER)
			if (lpQuestItem->m_wQuestKind != eQK_SKILL_MASTER)
				continue;
		if (_iQuestKind == eQM_TITLE_QUEST)
			if (lpQuestItem->m_wQuestKind != eQK_TITLE)
				continue;
		if (lpQuestItem->m_wBeginNPC_Field == 0xffff) // 이 옵션으로 활성화된 퀘스트인지 아닌지 체크한다.
			continue;
		if (!lpQuestItem->isEnableJob(g_hero.m_wJob)) // 현재 직업에 맞는 퀘스트만 출력한다.
			continue;
		if (g_hero.m_iLevel < lpQuestItem->m_wMinLimitLevel) // 최소수행레벨 보다 클때 진행
			continue;
		if (lpQuestItem->m_bf1IsDisableQuest) // 보여질 퀘스트만 출력
			continue;
		if (m_dlmenu.getSelectList() == eDL_DO_NOT_COMPLETE && g_hero.isCompleteQuest(eQT_ARBEIT, i))
			continue;
		if (m_dlmenu.getSelectList() == eDL_COMPLETE && !g_hero.isCompleteQuest(eQT_ARBEIT, i))
			continue;

		DWORD dwColor = dCOMPLETE_QUEST_COLOR;
		char* str =dMSG_COMPLETE;
		if (!g_hero.isCompleteQuest(eQT_ARBEIT, i))
		{
			dwColor = dDO_NOT_COMPLETE_QUEST_COLOR; 
			str = dMSG_DO_NOT_COMPLETE;
		}
		if (g_hero.isProcessQuest(eQT_ARBEIT, i))
		{
			dwColor = dPROCESS_QUEST_COLOR;
			str = dMSG_PROCESS;
		}
		if (m_iTitleQuestSelectQuestType == eQT_ARBEIT && m_iTitleQuestSelectQuest == i)
			dwColor = LTYELLOW;
		
		if(!_bDropDownArea && !*_iPutStartPoint)
		{
			cRECT rtTemp = {*_iX+30, *_iY, _rtPut->x2-30, *_iY+17};
			m_vtQuestArea.push_back(rtTemp);
			m_vtQuestIndex.push_back(i);
			m_vtQuestType.push_back(eQT_ARBEIT);
		}

		if(!*_iPutStartPoint)
		{
			cRECT rtTemp = {*_iX+30, *_iY, _rtPut->x2-30, *_iY+17};
			if (dwColor != dSELECT_QUEST_COLOR && rtTemp.isIn(s_posMouse.x,s_posMouse.y))
				dwColor = dFOCUS_QUEST_COLOR;
			s_text.put(*_iX+30, *_iY+2, dwColor, "%s (%s)", lpQuestItem->getQuestName(), str);
			*_iY += iYgap;
			if (m_iTitleQuestSelectQuest == -1)
			{
				m_iTitleQuestSelectQuest = i;
				m_iTitleQuestSelectQuestType = eQT_ARBEIT;
			}
		}
		if(*_iPutStartPoint)
		{
			(*_iPutStartPoint)--;
			continue;
		}
		
		if (*_iY+iYgap > _rtPut->y2-10)
			return;
	}
}
// insu function add end


// insu function add
//┌---------------------------------------------------------------┐
//│ 메인 퀘스트의 히스토리를 출력한다.                            │
//│                                                               │
//│ cRECT* _rtPut       :  출력 사각 영역                         │
//└---------------------------------------------------------------┘
void
CQuestWindow::DrawMainQuestHistory(cRECT* _rtPut)
{
	// draw
	s_ttCommon.drawFrame(_rtPut, FALSE);

	// set
	m_vtChapterArea.clear();
	m_vtChapterIndex.clear();
	m_vtPartArea.clear();
	m_vtPartIndex.clear();
	m_vtChapterofPartIndex.clear();
	m_vtQuestArea.clear();
	m_vtQuestIndex.clear();	

	CQuest	*lpQuestManager = &g_quest;
	int iX = _rtPut->x1+10;
	int iY = _rtPut->y1+10;	
	BOOL bChapterArea = (BOOL)m_vtChapterArea.size();	// 이 값이 거짓이면 챕터버튼의 영역을 다시 설정한다.
	BOOL bPartArea = (BOOL)m_vtPartArea.size();			// 이 값이 거짓이면 파트버튼의 영역을 다시 설정한다.
	BOOL bQuestArea = (BOOL)m_vtQuestArea.size();		// 이 값이 거짓이면 퀘스트버튼의 영역을 다시 설정한다.

	int iPutStartPoint = m_sbScrollBar.m_iCurrentPos;	// 이 값이 0이 되면 출력하기 시작한다.

	// output chapter 
	if (g_hero.m_wProcessChapterIndex == 0xffff || g_hero.m_wProcessHighQuestIndex == 0xffff)
		return;
	int iChapterIndex = 0;
	int iPartIndex    = 0;

	for(int i=0 ; i<g_hero.m_wProcessChapterIndex+1 ; i++)
	{
		int iIcon = eFWI_PLUS;
		if (m_bChapterBtnState[i])
			iIcon = eFWI_MINUS;

		int iprePutStartPoint = iPutStartPoint;
		if (!iPutStartPoint)
		{
			g_sprInterface2.Put(iX+8, iY+7, iIcon);
			s_text.put(iX+20, iY+2, WHITE, "CHAPTER %d %s", i+1, g_chapterManager.getChapterName(i));
			iY += iYgap;
		}
		if (iPutStartPoint)
			iPutStartPoint--;

		if (!bChapterArea && !iprePutStartPoint)
		{
			cRECT rtTemp = {iX, iY-iYgap, _rtPut->x2-30, iY-iYgap+17};
			m_vtChapterArea.push_back(rtTemp);			
			m_vtChapterIndex.push_back(i);
		}

		if (iY+iYgap > _rtPut->y2-10)
			return;

		// output part	
		if (iIcon == eFWI_MINUS)
		{			
			for(int j=0 ; j<g_chapterManager.getPartCount(i) ; j++)
			{
				CQuestItem	*lpQuestItem = lpQuestManager->getQuest(m_iProcessMainQuestIndex);

				if (i == g_hero.m_wProcessChapterIndex && j == g_hero.m_wProcessHighQuestIndex+1)
					return;		

				iIcon = eFWI_PLUS;
				if (m_bPartBtnState[i][j])
					iIcon = eFWI_MINUS;
				
				iprePutStartPoint = iPutStartPoint;
				if (!iPutStartPoint)
				{
					g_sprInterface2.Put(iX+18, iY+7, iIcon);
					if (j != g_hero.m_wProcessHighQuestIndex || j == g_hero.m_wProcessHighQuestIndex && i != g_hero.m_wProcessChapterIndex)
						s_text.put(iX+30, iY+2, WHITE, "PART %d %s", j+1, NameFiltering(g_chapterManager.getHighQuestName(i, j)));

					if (j == g_hero.m_wProcessHighQuestIndex && i == g_hero.m_wProcessChapterIndex)
					{
						s_text.put(iX+30, iY+2, WHITE, "PART %d %s", j+1, NameFiltering(g_chapterManager.getHighQuestName(i, j)));
					}
					iY += iYgap;
				}
				if (iPutStartPoint)
					iPutStartPoint--;

				if (!bPartArea && !iprePutStartPoint)
				{
					cRECT rtTemp = {iX+10, iY-iYgap, _rtPut->x2-30, iY-iYgap+17};
					m_vtPartArea.push_back(rtTemp);
					m_vtPartIndex.push_back(j);
					m_vtChapterofPartIndex.push_back(i);
				}

				if (iY+iYgap > _rtPut->y2-10)
					return;				
				
				// output Quest
				if (iIcon == eFWI_MINUS)
				{
					for(int k=0 ; k<lpQuestManager->GetQuestCount() ; k++)
					{
						CQuestItem	*lpQuestItem = lpQuestManager->getQuest(k);
						if (lpQuestItem->m_bf6ChapterNumber && lpQuestItem->m_bf6PartNumber && lpQuestItem->m_bf6ChapterNumber == i+1 && lpQuestItem->m_bf6PartNumber == j+1)
						{
							iprePutStartPoint = iPutStartPoint;
							if (!iPutStartPoint)
							{
								DWORD dwColor = dCOMPLETE_QUEST_COLOR;
								char* str = "";
								cRECT rtTemp = {iX+40, iY, _rtPut->x2-30, iY+17};
								if (rtTemp.isIn(s_posMouse.x,s_posMouse.y))
									dwColor = dFOCUS_QUEST_COLOR;
								if (m_iSelectHistoryQuest == k)
									dwColor = dSELECT_QUEST_COLOR;
								if (m_iProcessMainQuestIndex == k)
								{
									str = dMSG_PROCESS;
									dwColor = dPROCESS_QUEST_COLOR;
									s_text.put(iX+40, iY+2, dwColor, "%s (%s)", lpQuestItem->getQuestName(), str);
									iY += iYgap;
								}
								if (m_iProcessMainQuestIndex != k)
								{
									s_text.put(iX+40, iY+2, dwColor, "%s", lpQuestItem->getQuestName());
									iY += iYgap;
								}							
							}
							if (iPutStartPoint)
								iPutStartPoint--;

							if(!bQuestArea && !iprePutStartPoint)
							{
								cRECT rtTemp = {iX+40, iY-iYgap, _rtPut->x2-30, iY-iYgap+17};
								m_vtQuestArea.push_back(rtTemp);
								m_vtQuestIndex.push_back(k);
							}

							if (iY+iYgap > _rtPut->y2-10)
								return;
							if (m_iProcessMainQuestIndex == k)
									return;
						}
					}
				}
			}
		}
	}
}

// insu function add end



// insu function add
//┌---------------------------------------------------------------┐
//│ 메인퀘스트 파트네임에 숫자가 들어간 경우 걸러내기 위해 사용   │
//│                                                               │
//│ 리턴                :  숫자를 뺀 문자열 리턴~                 │
//│ char* _str          :  원본 문자열                            │
//└---------------------------------------------------------------┘
char*
CQuestWindow::NameFiltering(char* _str)
{
	// insu modify
	if (atoi(&_str[0]) || _str[0] == '0')
	// insu modify end
	{
		char* strRe = NameFiltering(&_str[1]);
		if (stricmp(strRe, _str))
			return strRe;
	}
	
	if (_str[0] == '.' && _str[1] == ' ')
		return &_str[2];
	if (_str[0] == '.' && _str[1] != ' ')
		return &_str[1];
	return _str;
}

// insu function add end

// insu function add
//┌---------------------------------------------------------------┐
//│ 일반 퀘스트의 히스토리를 출력한다.                            │
//│ 메인퀘스트 파일 안에도 일반퀘스트가 소수 존재하기 때문에      │
//│ 두번 체크해야 한다.                                           │
//│ 영역정보는 메인퀘스트 히스토리를 출력할 때 사용하던           │
//│ m_vtQuestArea 를 사용한다.                                    │
//│                                                               │
//│ cRECT* _rtPut       :  출력 사각 영역                         │
//└---------------------------------------------------------------┘

void
CQuestWindow::DrawNormalQuestHistory(cRECT* _rtPut)
{
	// set
	int iX = _rtPut->x1+10;
	int iY = _rtPut->y1+10;
	m_vtQuestArea.clear();
	m_vtQuestIndex.clear();
	m_vtQuestType.clear();

	BOOL bQuestArea = (BOOL)m_vtQuestArea.size();		// 이 값이 거짓이면 퀘스트버튼의 영역을 다시 설정한다.
	int iPutStartPoint = m_sbScrollBar.m_iCurrentPos;	// 이 값이 0이 되면 출력하기 시작한다.
	int iPutCnt = 0;	// 출력한 갯수 두개의 파일에서 퀘스트를 읽어오므로 인덱스값을 찾기위해 필요.

	// draw
	s_ttCommon.drawFrame(_rtPut, FALSE);
	
	CQuest	*lpQuestManager = &g_quest;
	int iQuestCount = lpQuestManager->GetQuestCount();

	g_sprInterface2.Put(iX+6, iY+3, 135);
	int iRate = ((float)m_iNormalQuestCompleteCnt/(float)m_iNormalQuestCnt)*100;
	s_text.put(iX+20, iY+2, WHITE, "%s <c:RED>(%s %d%%)<n>", dMSG_NORMAL_QUEST, dMSG_COMPLETE_RATE, iRate);
	iY += iYgap;

	for(int i=0 ; i<iQuestCount ; i++)
	{
		CQuestItem	*lpQuestItem = lpQuestManager->getQuest(i);
		if (lpQuestItem->m_wQuestKind != eQK_NORMAL) // 일반퀘스트만 출력한다.
			continue;
		if (lpQuestItem->m_wBeginNPC_Field == 0xffff) // 이 옵션으로 활성화된 퀘스트인지 아닌지 체크한다.
			continue;
		if (!lpQuestItem->isEnableJob(g_hero.m_wJob)) // 현재 직업에 맞는 퀘스트만 출력한다.
			continue;
		if (g_hero.m_iLevel < lpQuestItem->m_wMinLimitLevel) // 최소수행레벨 보다 클때 진행
			continue;
		if (lpQuestItem->m_bf1IsDisableQuest) // 보여질 퀘스트만 출력
			continue;
		if (m_dlmenu.getSelectList() == eDL_DO_NOT_COMPLETE && g_hero.isCompleteQuest(eQT_NORMAL, i))
			continue;
		if (m_dlmenu.getSelectList() == eDL_COMPLETE && !g_hero.isCompleteQuest(eQT_NORMAL, i))
			continue;
		if (lpQuestItem->m_bf6ChapterNumber || lpQuestItem->m_bf6PartNumber)
			continue;

		if (iY+iYgap > _rtPut->y2-10)
			return;
		
		if (iPutStartPoint)
		{
			iPutStartPoint--;
			continue;
		}
		
		if (m_iSelectNormalQuest == -1)	// 선택된 퀘스트가 없다면 첫번째 퀘스트로 지정
		{
			m_iSelectNormalQuest = i;
			m_iSelectNormalQuestType = eQT_NORMAL;
		}
		iPutCnt++;
		DWORD dwColor = dCOMPLETE_QUEST_COLOR;
		char* str = dMSG_COMPLETE;
		if (!g_hero.isCompleteQuest(eQT_NORMAL, i))
		{
			dwColor = dDO_NOT_COMPLETE_QUEST_COLOR; 
			str = dMSG_DO_NOT_COMPLETE;
		}
		if (g_hero.isProcessQuest(eQT_NORMAL, i))
		{
			dwColor = dPROCESS_QUEST_COLOR;
			str = dMSG_PROCESS;
		}
		if (m_iFocusNormalQuest == iPutCnt-1)
			dwColor = dFOCUS_QUEST_COLOR;
		if (m_iSelectNormalQuestType == eQT_NORMAL && m_iSelectNormalQuest == i)
			dwColor = dSELECT_QUEST_COLOR;

		s_text.put(iX+30, iY+2, dwColor, "%s (%s)", lpQuestItem->getQuestName(), str);
		if(!bQuestArea)
		{
			cRECT rtTemp = {iX+30, iY, _rtPut->x2-30, iY+17};
			m_vtQuestArea.push_back(rtTemp);
			m_vtQuestIndex.push_back(i);
			m_vtQuestType.push_back(eQT_NORMAL);
		}
		iY += iYgap;		
	}

	lpQuestManager = &g_questArbeit;
	iQuestCount = lpQuestManager->GetQuestCount();
	for(i=0 ; i<iQuestCount ; i++)
	{
		CQuestItem	*lpQuestItem = lpQuestManager->getQuest(i);
		if (lpQuestItem->m_wQuestKind != eQK_NORMAL) // 일반퀘스트만 출력한다.
			continue;
		if (lpQuestItem->m_wBeginNPC_Field == 0xffff) // 이 옵션으로 활성화된 퀘스트인지 아닌지 체크한다.
			continue;
		if (!lpQuestItem->isEnableJob(g_hero.m_wJob)) // 현재 직업에 맞는 퀘스트만 출력한다.
			continue;
		if (g_hero.m_iLevel < lpQuestItem->m_wMinLimitLevel) // 최소수행레벨 보다 클때 진행
			continue;
		if (lpQuestItem->m_bf1IsDisableQuest) // 보여질 퀘스트만 출력
			continue;
		if (m_dlmenu.getSelectList() == eDL_DO_NOT_COMPLETE && g_hero.isCompleteQuest(eQT_ARBEIT, i))
			continue;
		if (m_dlmenu.getSelectList() == eDL_COMPLETE && !g_hero.isCompleteQuest(eQT_ARBEIT, i))
			continue;
		if (lpQuestItem->m_bf6ChapterNumber || lpQuestItem->m_bf6PartNumber)
			continue;


		if (iY+iYgap > _rtPut->y2-10)
			return;
		
		if (iPutStartPoint)
		{
			iPutStartPoint--;
			continue;
		}
		
		if (m_iSelectNormalQuest == -1)	// 선택된 퀘스트가 없다면 첫번째 퀘스트로 지정
		{
			m_iSelectNormalQuest = i;
			m_iSelectNormalQuestType = eQT_ARBEIT;
		}

		iPutCnt++;
		DWORD dwColor = dCOMPLETE_QUEST_COLOR;
		char* str = dMSG_COMPLETE;
		if (!g_hero.isCompleteQuest(eQT_ARBEIT, i))
		{
			dwColor = dDO_NOT_COMPLETE_QUEST_COLOR; 
			str = dMSG_DO_NOT_COMPLETE;
		}
		if (g_hero.isProcessQuest(eQT_ARBEIT, i))
		{
			dwColor = dPROCESS_QUEST_COLOR;
			str = dMSG_PROCESS;
		}
		if (m_iFocusNormalQuest == iPutCnt-1)
			dwColor = dFOCUS_QUEST_COLOR;
		if (m_iSelectNormalQuestType == eQT_ARBEIT && m_iSelectNormalQuest == i)
			dwColor = dSELECT_QUEST_COLOR;

		s_text.put(iX+30, iY+2, dwColor, "%s (%s)", lpQuestItem->getQuestName(), str);
		if(!bQuestArea)
		{
			cRECT rtTemp = {iX+30, iY, _rtPut->x2-30, iY+17};
			m_vtQuestArea.push_back(rtTemp);
			m_vtQuestIndex.push_back(i);
			m_vtQuestType.push_back(eQT_ARBEIT);
		}
		iY += iYgap;		
	}
}
// insu function add end


// insu function add
//┌---------------------------------------------------------------┐
//│ 라디오버튼을 셋팅한다. (메인 일반 칭호)                       │
//│ 시스템메뉴의 라디오버튼 객체를 이용한다.                      │
//│                                                               │
//│ cRECT* rtPut        :  출력 사각 영역                         │
//└---------------------------------------------------------------┘
void
CQuestWindow::setRadioBtn(cRECT* _rtPut)
{	
	int iX = _rtPut->x1;
	int iY = _rtPut->y1;
	int iBtnSpace = 90;

	s_bmCategory.reset();
	s_bmCategory.addImageBar(dMSG_MAIN_QUEST, eQM_MAIN_QUEST, iX, iY, 
		eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);

	iX += iBtnSpace;

	s_bmCategory.addImageBar(dMSG_NORMAL_QUEST, eQM_NORMAL_QUEST, iX, iY, 
		eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);

	iX += iBtnSpace;

	cSTRING str;
	str.Add("%s ", dMSG_TITLE);
	str.Add(dMSG_WM_QUEST);

	s_bmCategory.addImageBar(str.String, eQM_TITLE_QUEST, iX, iY, 
		eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);

	s_bmCategory.check(m_iSelectRadioBtn, TRUE);
}
// insu function add end



// insu function add
//┌---------------------------------------------------------------┐
//│ 매개변수로 들어온 퀘스트의 간단한 정보를 출력해 준다.         │
//│ 출력 내용은 관련NPC, 관련지역, 수행가능레벨이다.              │
//│                                                               │
//│ cRECT* rtPut        :  출력할 사각 영역                       │
//│ int _iQuestType     :  퀘스트 타입(노말 or 알바)              │
//│ int _iQuestIndex    :  퀘스트 인덱스                          │
//└---------------------------------------------------------------┘
void
CQuestWindow::DrawQuestSimpleInfo(cRECT* _rtPut, int _iQuestType, int _iQuestIndex)
{	
	if (_iQuestType != eQT_NORMAL && _iQuestType != eQT_ARBEIT)
		return;

	if (_iQuestIndex == -1)
	{
		s_ttCommon.drawFrame(_rtPut, FALSE);
		s_text.cPut(_rtPut->x1, _rtPut->y1, _rtPut->getWidth(), _rtPut->getHeight(), 0, CTBLUE, dFITTEXT_CENTER, dMSG_NOT_EXIST_PROCESS_QUEST);		
		return;
	}

	if (m_iSelectRadioBtn == eQM_MAIN_QUEST && g_hero.m_wProcessChapterIndex == 0xffff || 
		m_iSelectRadioBtn == eQM_MAIN_QUEST && g_hero.m_wProcessHighQuestIndex == 0xffff)
	{
		s_ttCommon.drawFrame(_rtPut, FALSE);
		s_text.cPut(_rtPut->x1, _rtPut->y1, _rtPut->getWidth(), _rtPut->getHeight(), 0, CTBLUE, dFITTEXT_CENTER, dMSG_NOT_EXIST_PROCESS_QUEST);		
		return;
	}

	int iLinegap = 20;
	CQuest	*lpQuestManager;
	cSTRING strSimpleInfo;

	if (_iQuestType == eQT_NORMAL)
		lpQuestManager = &g_quest;
	if (_iQuestType == eQT_ARBEIT)
		lpQuestManager = &g_questArbeit;
	
	CQuestItem	*lpQuestItem = lpQuestManager->getQuest(_iQuestIndex);
	char	*lpstrNPCName = lpQuestItem->m_strBeginNPCName;
	CQuestItem::cQuestInfo *lpQuestInfo =  lpQuestItem->getInfo(0);

	s_ttCommon.drawFrame(_rtPut, FALSE);

	int iX = _rtPut->x1+10;
	int iY = _rtPut->y1+10;
	
	
	// 관련 npc
	if (m_iSelectRadioBtn == eQM_MAIN_QUEST)
	{
		if (lpQuestInfo->m_wLinkNpcCount)
		{
			s_text.put(iX, iY, WHITE, "<c:CTBLUE>%s<n>           %s", dMSG_LINK_QUEST_NPC, lpQuestInfo->m_pNpcList[0].m_pStrName);
			iY += iLinegap;
		}
	
		if (!lpQuestInfo->m_wLinkNpcCount)
		{
			if (lpQuestItem->m_strBeginNPCName)
			{
				s_text.put(iX, iY, WHITE,  
					"<c:CTBLUE>%s<n>           %s", dMSG_LINK_QUEST_NPC, lpQuestItem->m_strBeginNPCName);
				iY += iLinegap;
			}
		}
	}
	if (m_iSelectRadioBtn != eQM_MAIN_QUEST)
	{
		if (lpstrNPCName != NULL)
		{
			s_text.put(iX, iY, WHITE,  
				"<c:CTBLUE>%s<n>           %s", dMSG_LINK_QUEST_NPC, lpstrNPCName);
			iY += iLinegap;
		}
	}
	
	// 관련 지역
	if (m_iSelectRadioBtn == eQM_MAIN_QUEST)
	{
		if (lpQuestInfo->m_wLinkFieldCount)
		{
			s_text.put(iX, iY, WHITE, "<c:CTBLUE>%s<n>           %s", dMSG_AREA_COMMENT, g_aMapSimpleInfo[lpQuestInfo->m_pLinkField[0]].m_strName);
			iY += iLinegap;
		}
		
		if (!(lpQuestInfo->m_wLinkFieldCount))
		{
			if (lpQuestItem->m_wBeginNPC_Field != 0xffff)
			{
				s_text.put(iX, iY, WHITE,  
					"<c:CTBLUE>%s<n>           %s", dMSG_AREA_COMMENT, g_aMapSimpleInfo[lpQuestItem->m_wBeginNPC_Field].m_strName);
				iY += iLinegap;
			}
		}
	}
	if (m_iSelectRadioBtn != eQM_MAIN_QUEST)
	{
		if (lpQuestItem->m_wBeginNPC_Field != 0xffff)
		{
			s_text.put(iX, iY, WHITE,  
				"<c:CTBLUE>%s<n>           %s", dMSG_AREA_COMMENT, g_aMapSimpleInfo[lpQuestItem->m_wBeginNPC_Field].m_strName);
			iY += iLinegap;
		}
	}

	// 수행 가능 레벨
	if (lpQuestItem->m_wMaxLimitLevel == 1000)
	{
		s_text.put(iX, iY, WHITE, "<c:CTBLUE>%s<n>    %d %s", dMSG_POSSIBLE_LEVEL, lpQuestItem->m_wMinLimitLevel, dMSG_ABOVE);
	}
	if (lpQuestItem->m_wMaxLimitLevel != 1000)
	{
		s_text.put(iX, iY, WHITE, "<c:CTBLUE>%s<n>    %d ~ %d", dMSG_POSSIBLE_LEVEL, lpQuestItem->m_wMinLimitLevel, lpQuestItem->m_wMaxLimitLevel);
		
	}

	iY	+=	dQW_PROCESS_QUEST_LIST_GAP;
}

// insu function add end



// insu function add
//┌---------------------------------------------------------------┐
//│ 필터를 출력한다.                                              │
//│                                                               │
//│ cRECT* rtPut        :  출력할 사각 영역                       │
//└---------------------------------------------------------------┘
void
CQuestWindow::DrawHistoryFilter(cRECT* _rtPut)
{
	m_dlmenu.draw();
}
// insu function add end