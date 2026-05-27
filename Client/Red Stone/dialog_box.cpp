#include	"dialog_box.h"
#include	"CHero.H"
#include	"CActor.H"
#include	"CWindowInterface.H"
#include	"CMessage.H"

cDialogBox		g_dialogBox;

#define	dDIALOG_FACE_WIDTH					170
#define	dSELECT_SPEECH_SPARE_WIDTH			12
#define	dDIALOG_FRAME_HEIGHT_MARGIN			6
#define	dDIALOG_AND_SELECT_SPEECH_MARGIN	8

enum
{
	eDB_TITLE_LEFT_CAP	=	82,
	eDBI_TITLE			=	136,
	eDBI_NAME_LEFT	,
	eDBI_NAME_CENTER,
	eDBI_NAME_RIGHT	,
	eDBI_DECO_LT,
	eDBI_DECO_RT,
	eDBI_DECO_LD,
	eDBI_DECO_RD,
	eDBI_SEPERATE_BAR,

	eDBI_X_NORMAL,
	eDBI_X_PRESSED,
	eDBI_X_ACTIVE,

	eFRAME_X1	=	471,
	eFRAME_Y1	=	0,
	eFRAME_X2	=	799,
	eFRAME_Y2	=	490,
};

cDialogBox::cDialogBox()
{
	reset();
}

void
cDialogBox::reset()
{
	m_iTalker		=	-1;
	m_iSpeechHeight	=	-1;
	m_iFocusSpeech	=	-1;
	m_bIsReleasedMouseButton	=	FALSE;
	m_bDoNotCloseDialogBox		=	FALSE;
	m_iAddWidth					=	0;
	m_posOldView.Set(0,0);
}

//
//	복합대화 시작
void
cDialogBox::beginComplexDialog(int _iTalker,int _iEventArea)
{
	CActor	*lpActor		=	g_am.getTestedActor(_iTalker);

	if	(!lpActor)
		return;

	lpActor->m_wTalkTime	=	0;	//	뭔가 말하고 있었다면 취소~

	if	(m_speech.m_wContentsCount	<=	0	&&	m_speech.m_wDialogBoxSize	==	2)
	{
		lpActor	=	g_am.getTestedActor(_iTalker);

		if	(!lpActor)
			return;

		closeComplexDialog();

		lpActor->talk(m_speech.getSpeech());

		return;
	}

	if	(lpActor->isPlayer()	==	FALSE)
		if	(g_lpHero)
			lpActor->setDirect(lpActor->getDirect(g_lpHero->m_pos.x,g_lpHero->m_pos.y,TRUE));

	s_sbPopupWindow.setPos(0);

	g_hero.lockOff();
	g_gwShop.close();

	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	m_posEventPosition.x		=	g_lpHero->m_pos.x;
	m_posEventPosition.y		=	g_lpHero->m_pos.y;
	m_iTalker					=	_iTalker;
	m_wEventArea				=	_iEventArea;
	m_iSpeechHeight				=	-1;
	m_bIsReleasedMouseButton	=	FALSE;

	int		iFaceWidth			=	0;//dDIALOG_FACE_WIDTH+10;

	switch(m_speech.m_wDialogBoxSize)
	{
		case	0	:
		case	3	:
			m_iSelectSpeechLineGap	=	6;
			m_iSpeechLineGap		=	4;
			m_iDialogBoxWidth=g_iScreenWidth-180,m_iDialogBoxHeight=220;
			m_iLineGap				=	4;
			break;

		case	1	:
			m_iSelectSpeechLineGap	=	6;
			m_iSpeechLineGap		=	4;
			m_iDialogBoxWidth=g_iScreenWidth-280,m_iDialogBoxHeight=60;
			m_iLineGap				=	4;
			break;

		case	2	:
			m_iSelectSpeechLineGap	=	2;
			m_iSpeechLineGap		=	1;
			m_iDialogBoxWidth=240,m_iDialogBoxHeight=150,iFaceWidth=4;
			m_iLineGap				=	4;
			break;
	}

	if	(m_wEventArea	!=	0xffff	&&	m_speech.m_wDialogBoxSize	!=	3)
		m_iDialogBoxWidth=300,m_iDialogBoxHeight=160;

	setDialogBoxFrame(m_iDialogBoxWidth,m_iDialogBoxHeight,iFaceWidth);

	if	(m_speech.m_wDialogBoxSize	!=	2	&&	m_wEventArea	==	0xffff)
	{
		s_rectInterfaceSize.x1 = g_iScreenWidth-eFRAME_X1-g_iCorrectBI_X;
		s_rectInterfaceSize.x2 = g_iScreenWidth;
	}
	if	(m_speech.m_wDialogBoxSize	==	3)
	{
		s_rectInterfaceSize.x1 = g_iScreenWidth-eFRAME_X1-g_iCorrectBI_X;
		s_rectInterfaceSize.x2 = g_iScreenWidth;
	}

	if	(g_iCorrectBI_X)
		s_rectInterfaceSize.set(0, 0, 0, 0);
}

void
cDialogBox::setDialogBoxFrame(int _iWidth,int _iHeight,int _iFaceWidth)
{
	CPos	pos;

	m_iDialogBoxWidth	=	_iWidth;
	m_iDialogBoxHeight	=	_iHeight;
	m_iDialogBoxFaceWidth=	4;

	if	(m_wEventArea	!=	0xffff	&&	m_speech.m_wDialogBoxSize	!=	3)
	{
		cAreaInfo	*lpArea	=	g_map.m_area.getArea(m_wEventArea);

		if	(lpArea)
		{
			if	(lpArea->isEventArea())
			{
				cAreaEvent			*lpEventArea=	(cAreaEvent *)lpArea;
				cFIXED_OBJECT_INFO	*lpInfo		=	&g_map.m_object.m_aFixedObjectList[lpEventArea->m_wLinkObject];

				cRECT	rect;
				g_map.m_object.m_aFixedObjectImage[lpInfo->m_wObject].getRect(0,&rect);

				pos.x	=	lpInfo->m_wX-g_map.m_pos.x+dTILE_XS/2;
				pos.y	=	lpInfo->m_wY+rect.y2-g_map.m_pos.y-rect.getHeight()*2/3;
			}
			else
			if	(lpArea->isDoor())
			{
				cAreaDoorInfo		*lpDoorArea	=	(cAreaDoorInfo *)lpArea;
				cDoorObjectInfo		*lpDoorObject=	&g_map.m_doors.m_aDoorList[lpDoorArea->m_awLinkObject[0]];
				int					iIndex		=	lpDoorObject->m_wObjectIndex;
				cFIXED_OBJECT_INFO	*lpInfo		=	&g_map.m_object.m_aFixedObjectList[iIndex];

				cRECT	rect;
				g_map.m_object.m_aFixedObjectImage[lpInfo->m_wObject].getRect(0,&rect);

				pos.x	=	lpInfo->m_wX-g_map.m_pos.x+dTILE_XS/2;
				pos.y	=	lpInfo->m_wY+rect.y2-g_map.m_pos.y-rect.getHeight()*2/3;
			}
		}
	}
	else
	{
		pos.x				=	g_aActor[m_iTalker].m_pos.x-g_map.m_pos.x;
		pos.y				=	g_aActor[m_iTalker].m_pos.y-g_map.m_pos.y-m_iDialogBoxHeight-g_aActor[m_iTalker].getBodyHeight()-14;
		m_posArrow.x		=	pos.x;

		pos.x				-=	40;
	}

	int	iTopBorderSize	=	s_ttDialogFrame.m_iTopEdgeHeight+2;
	int	iRightBorderSize=	s_ttDialogFrame.m_iLeftEdgeWidth+2;

	pos.y	=	max(pos.y,iTopBorderSize);

	if	(m_wEventArea	!=	0xffff	&&	m_speech.m_wDialogBoxSize	!=	3)
	{
		pos.x	=	max(pos.x,m_iDialogBoxWidth/2+iRightBorderSize);
		pos.x	=	min(pos.x,g_iScreenWidth-(m_iDialogBoxWidth/2+iRightBorderSize));

		m_rectDialogBoxFrame.set(pos.x-m_iDialogBoxWidth/2,pos.y-m_iDialogBoxHeight,pos.x+m_iDialogBoxWidth/2,pos.y);
	}
	else
	{
		if	(pos.x + m_iDialogBoxWidth	>=	g_iScreenWidth-iRightBorderSize	)
			pos.x	=	g_iScreenWidth-m_iDialogBoxWidth-iRightBorderSize;

		m_rectDialogBoxFrame.set(pos.x,pos.y,pos.x+m_iDialogBoxWidth,pos.y+m_iDialogBoxHeight);
	}

	int	iTitleHeight	=	dDIALOG_FRAME_HEIGHT_MARGIN;
	int	iTitleBarHeight	=	g_sprInterface2.getSpriteHeight(eDB_TITLE_LEFT_CAP);

	if	(m_speech.m_wDialogBoxSize	==	3	||	(m_speech.m_wDialogBoxSize	!=	2	&&	m_wEventArea	==	0xffff))
		iTitleHeight	+=	(iTitleBarHeight-4);

	m_rectDialogBoxTextFrame.set(m_rectDialogBoxFrame.x1+m_iDialogBoxFaceWidth,m_rectDialogBoxFrame.y1+iTitleHeight,m_rectDialogBoxFrame.x2-24,m_rectDialogBoxFrame.y2-dDIALOG_FRAME_HEIGHT_MARGIN);

	s_sbPopupWindow.set(m_rectDialogBoxFrame.x2-12,m_rectDialogBoxFrame.y1+iTitleBarHeight,m_rectDialogBoxFrame.getHeight()-iTitleBarHeight);

	m_posArrow.y	=	m_rectDialogBoxFrame.y2+1;

	if	(m_speech.m_wDialogBoxSize!=2)
		m_posArrow.x	=	-100;
}

//
//	다얄로그 박스 프레임 초기화
void
cDialogBox::initDialogBoxFrame()
{
	int		i;

	char	*lpstrSpeech	=	m_speech.getSpeech();
	CActor	*lpSpeaker		=	&g_aActor[m_iTalker];
	int		iNameWidth		=	s_textBold14.getPixelSize(lpSpeaker->m_strName);
	int		iKindWidth		=	s_text.getPixelSize(_ms("[%s]",g_pstrNpcName[lpSpeaker->m_wActorKind]));
	int		iTitleWidth		=	iNameWidth+iKindWidth+6;
	char	strDialogText[1024] ="";
//	가로 사이즈 재 설정
	if	(m_speech.m_wDialogBoxSize	!=	3	&&	(m_speech.m_wDialogBoxSize	==	2	||	m_wEventArea	!=	0xffff))
	{

		memset(strDialogText,0,sizeof(char)*1024);		
		// insu add
		// s_text.replaceText(strDialogText,lpstrSpeech,dMSG_TARGET_NAME, g_hero.m_strName);
		CheckInstruction(lpstrSpeech, strDialogText, 1024);
		// insu add end
		int iMaxWidth	=	s_text.getPixelSize(strDialogText)+m_iDialogBoxFaceWidth;

		for (i=0;i<m_speech.m_wContentsCount;i++)
		{
			memset(strDialogText,0,sizeof(char)*1024);
			lpstrSpeech	=	m_speech.getSelectSpeech(i);
			// insu add
			//s_text.replaceText(strDialogText,lpstrSpeech,dMSG_TARGET_NAME, g_hero.m_strName);
			CheckInstruction(lpstrSpeech, strDialogText, 1024);
			// insu add end

			iMaxWidth	=	max(s_text.getPixelSize(strDialogText)+dSELECT_SPEECH_SPARE_WIDTH+m_iDialogBoxFaceWidth,iMaxWidth);
		}

		iMaxWidth		=	max(iTitleWidth,iMaxWidth);

		if	(m_iDialogBoxWidth > iMaxWidth+26)
			m_iDialogBoxWidth =	iMaxWidth+26;

		setDialogBoxFrame(m_iDialogBoxWidth,m_iDialogBoxHeight,m_iDialogBoxFaceWidth);

		int		iHeight	=	0;

		s_text.setLineGap(m_iSpeechLineGap);
		iHeight		=	s_text.getTextHeight(m_rectDialogBoxTextFrame.getWidth(),m_speech.getSpeech());
		iHeight		+=	dDIALOG_FRAME_HEIGHT_MARGIN;

		s_text.setLineGap(m_iSelectSpeechLineGap);

		for (i=0;i<m_speech.m_wContentsCount;i++)
		{
			lpstrSpeech		=	m_speech.getSelectSpeech(i);
			memset(strDialogText,0,sizeof(char)*1024);

			// insu add
			//s_text.replaceText(strDialogText,lpstrSpeech,dMSG_TARGET_NAME, g_hero.m_strName);
			CheckInstruction(lpstrSpeech, strDialogText, 1024);
			// insu add end

			iHeight			+=	s_text.getTextHeight(m_rectDialogBoxTextFrame.getWidth()-dSELECT_SPEECH_SPARE_WIDTH,strDialogText);
			iHeight			+=	m_iLineGap;
		}

		if	(m_iDialogBoxHeight	>	iHeight+12)
			m_iDialogBoxHeight	=	iHeight+12;

		setDialogBoxFrame(m_iDialogBoxWidth,m_iDialogBoxHeight,m_iDialogBoxFaceWidth);

	//	선택문 영역 설정
		{
			CPos	pos;

			s_text.setLineGap(m_iSpeechLineGap);
			memset(strDialogText,0,sizeof(char)*1024);
			// insu add
			//s_text.replaceText(strDialogText,m_speech.getSpeech(),dMSG_TARGET_NAME, g_hero.m_strName);
			CheckInstruction(m_speech.getSpeech(), strDialogText, 1024);
			// insu add end
			
			m_iSpeechHeight	=	s_text.getTextHeight(m_rectDialogBoxTextFrame.getWidth(),strDialogText);
			pos.y			=	m_rectDialogBoxTextFrame.y1+m_iSpeechHeight+dDIALOG_FRAME_HEIGHT_MARGIN;//+s_text.getFontHeight();

			s_text.setLineGap(m_iSelectSpeechLineGap);

			for (i=0;i<m_speech.m_wContentsCount;i++)
			{
				lpstrSpeech		=	m_speech.getSelectSpeech(i);
				memset(strDialogText,0,sizeof(char)*1024);
				// insu add
				//s_text.replaceText(strDialogText,lpstrSpeech,dMSG_TARGET_NAME, g_hero.m_strName);
				CheckInstruction(lpstrSpeech, strDialogText, 1024);
				// insu add end

				int	iHeight		=	s_text.getTextHeight(m_rectDialogBoxTextFrame.getWidth()-dSELECT_SPEECH_SPARE_WIDTH,strDialogText);

				m_speech.m_arectSelectSpeech[i].x1	=	m_rectDialogBoxTextFrame.x1+dSELECT_SPEECH_SPARE_WIDTH;
				m_speech.m_arectSelectSpeech[i].x2	=	m_rectDialogBoxTextFrame.x2;
				m_speech.m_arectSelectSpeech[i].y1	=	pos.y;
				m_speech.m_arectSelectSpeech[i].y2	=	pos.y+iHeight+m_iLineGap;

				pos.y								+=	iHeight+m_iLineGap;
			}

			s_sbPopupWindow.setSize(pos.y-m_rectDialogBoxTextFrame.y1-2,m_rectDialogBoxTextFrame.getHeight());
		}

	}
	else
	{
		m_iSelectSpeechLineGap	=	2;
		m_iLineGap				=	6;
		
		CActor	*lpSpeaker		=	&g_aActor[m_iTalker];
		char	*lpstrDialog	=	m_speech.getSpeech();
		memset(strDialogText,0,sizeof(char)*1024);
		// insu add
		//s_text.replaceText(strDialogText,lpstrDialog,dMSG_TARGET_NAME, g_hero.m_strName);
		CheckInstruction(lpstrDialog, strDialogText, 1024);
		// insu add end
		
		m_rectDialogBoxFrame.set(471+g_iCorrectBI_X*29/30,g_iCorrectBI_Y/2,799+g_iCorrectBI_X*29/30,490+g_iCorrectBI_Y/2);

		int		iTitleBarHeight			=	g_sprInterface2.getSpriteHeight(eDBI_TITLE);
		int		iSB_Width				=	0;
		int		iDefaultDialogTextHeight=	140;
		int		iMinDialogTextHeight	=	70;
		int		iDialogTextHeight		=	iDefaultDialogTextHeight;
		int		iDialogBoxHeight		=	0;

		int		iTopSpareHeight			=	25;
		int		iBottomSpareHeight		=	25;
		int		iDialogBoxTopSpareHeight=	iTitleBarHeight+25;
		int		iSpareHeight			=	iTopSpareHeight+iBottomSpareHeight;
		CPos	posText;

		posText.x	=	m_rectDialogBoxFrame.x1+25;
		posText.y	=	m_rectDialogBoxFrame.y1+iDialogBoxTopSpareHeight;

		s_text13.setLineGap(m_iSpeechLineGap);

		m_rectDialogBoxTextFrame.set(&m_rectDialogBoxFrame);

		m_rectDialogBoxTextFrame.x1	=	posText.x;
		m_rectDialogBoxTextFrame.x2	-=	25;

		int	iTextHeight		=	s_text13.getTextHeight(m_rectDialogBoxTextFrame.getWidth()-dSELECT_SPEECH_SPARE_WIDTH,strDialogText);

		iDialogTextHeight	=	max(iTextHeight,iMinDialogTextHeight);
		iDialogBoxHeight	=	iDialogTextHeight+iSpareHeight;
		m_iSpeechHeight		=	iDialogTextHeight;

		int	iLineCount		=	s_text13.getLineCount(m_rectDialogBoxTextFrame.getWidth()-dSELECT_SPEECH_SPARE_WIDTH,strDialogText);
		int	iBoxLineCount	=	iDefaultDialogTextHeight/s_text13.getLineThick();

		if	(iLineCount	>	iBoxLineCount)
		{
			iDialogTextHeight	=	iDefaultDialogTextHeight;
			iDialogBoxHeight	=	iDialogTextHeight+iSpareHeight;

			int iAddY = 0;
			if (g_iScreenWidth != 800)
			{
				iAddY = (g_iScreenHeight-600)/2;
			}

			s_sbPopupWindow.set(m_rectDialogBoxFrame.x2-30,iTitleBarHeight+24+iAddY,iDialogBoxHeight-52);
			s_sbPopupWindow.setSize(iLineCount,iBoxLineCount);

			iSB_Width					=	s_sbPopupWindow.m_iButtonWidth-1;
			m_rectDialogBoxTextFrame.x2	-=	iSB_Width;
		}
		else
		{
			s_sbPopupWindow.setSize(1,10);
		}

		m_rectDialogBoxTextFrame.y1	=	posText.y;
		m_rectDialogBoxTextFrame.y2	=	posText.y+iDialogTextHeight;

		if	(iTextHeight	<	iMinDialogTextHeight)
		{
			posText.y					+=	(m_rectDialogBoxTextFrame.getHeight()-iTextHeight)/2;
			m_rectDialogBoxTextFrame.y1	=	posText.y;
		}

//	선택문 영역 설정
		{
			CPos	pos;

			pos.y			=	m_rectDialogBoxTextFrame.y2+iBottomSpareHeight+dDIALOG_FRAME_HEIGHT_MARGIN*2;
			s_text13.setLineGap(m_iSelectSpeechLineGap);

			for (i=0;i<m_speech.m_wContentsCount;i++)
			{

				lpstrSpeech		=	m_speech.getSelectSpeech(i);
				memset(strDialogText,0,sizeof(char)*1024);
				// insu add
				// s_text.replaceText(strDialogText,lpstrSpeech,dMSG_TARGET_NAME, g_hero.m_strName);
				CheckInstruction(lpstrSpeech, strDialogText, 1024);
				// insu add end

				int	iHeight		=	s_text13.getTextHeight(m_rectDialogBoxTextFrame.getWidth()-dSELECT_SPEECH_SPARE_WIDTH,strDialogText);

				m_speech.m_arectSelectSpeech[i].x1	=	m_rectDialogBoxTextFrame.x1+dSELECT_SPEECH_SPARE_WIDTH;
				m_speech.m_arectSelectSpeech[i].x2	=	m_rectDialogBoxTextFrame.x2;
				m_speech.m_arectSelectSpeech[i].y1	=	pos.y;
				m_speech.m_arectSelectSpeech[i].y2	=	pos.y+iHeight+m_iLineGap;

				pos.y								+=	iHeight+m_iLineGap;
			}
		}

		s_text13.setLineGap(2);

		{
			s_bmMiddleWindowButton.reset();

			int	iXPos	=	m_rectDialogBoxTextFrame.x1+m_rectDialogBoxTextFrame.getWidth()/2-g_sprInterface.getSpriteWidth(eBUTTON_NORMAL)/2;

			s_bmMiddleWindowButton.addImageBar(dMSG_CLOSE	,IDOK		,iXPos	,466+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
			s_bmMiddleWindowButton.setClickMargin(IDOK,1,1);
			s_bmMiddleWindowButton.setAllBarTextColor(0,FALSE);
		}
	}
}

//
//	복합대화 종료
void
cDialogBox::closeComplexDialog()
{
	if	(m_iTalker	==	-1)
		return;

	m_iTalker				=	-1;
	m_iFocusSpeech			=	-1;
	s_rectInterfaceSize.set(0, 0, 0, 0);
	m_bDoNotCloseDialogBox	=	FALSE;
	m_iAddWidth				=	0;
	m_posOldView.Set(0,0);
}

//
//복합대화 그려주기

void
cDialogBox::drawComplexDialog()
{
	if	(!isTalking())
		return;

	if	(m_iSpeechHeight	==	-1)
		initDialogBoxFrame();

	if	(m_speech.m_wDialogBoxSize	==	3	||	(m_speech.m_wDialogBoxSize	!=	2	&&	m_wEventArea	==	0xffff))
	{
		CActor	*lpSpeaker		=	&g_aActor[m_iTalker];
		char	*lpstrDialog	=	m_speech.getSpeech();
		char strDialogText[1024] ="";
		// insu add
		// s_text.replaceText(strDialogText,lpstrDialog,dMSG_TARGET_NAME, g_hero.m_strName);
		CheckInstruction(lpstrDialog, strDialogText, 1024);
		// insu add end

		s_ttCommon.drawBox(&m_rectDialogBoxFrame,0,24);
		g_sprInterface2.Put(m_rectDialogBoxFrame.x1,g_iCorrectBI_Y/2,eDBI_TITLE);

		int		iWidth = 0;
		
		if(m_wEventArea !=0xffff)			// 09.09.11  이벤트 영역 오브젝트 큰대화창 이름 표시....
		{
			cAreaInfo	*lpArea	=	g_map.m_area.getArea(m_wEventArea);
			if(lpArea)
			{
				int		iNameWidth		=	s_textBold14.getPixelSize(lpArea->m_strName);
				int		iKindWidth		=	s_text.getPixelSize(_ms("[%s]",g_pstrNpcName[eNPC_NORMAL]));
				iWidth			=	iNameWidth+iKindWidth+2;
				s_textBold14.put(m_rectDialogBoxFrame.x1+m_rectDialogBoxFrame.getWidth()/2-iWidth/2,m_rectDialogBoxFrame.y1-1+8,WHITE,lpArea->m_strName );
				s_text.put(m_rectDialogBoxFrame.x1+m_rectDialogBoxFrame.getWidth()/2-iWidth/2+iNameWidth+2,m_rectDialogBoxFrame.y1+1+8,WHITE,"[%s]", g_pstrNpcName[eNPC_NORMAL]);
			}
		}
		else
		{
			int		iNameWidth		=	s_textBold14.getPixelSize(lpSpeaker->m_strName);
			int		iKindWidth		=	s_text.getPixelSize(_ms("[%s]",g_pstrNpcName[lpSpeaker->m_wActorKind]));
			iWidth			=	iNameWidth+iKindWidth+2;
			s_textBold14.put(m_rectDialogBoxFrame.x1+m_rectDialogBoxFrame.getWidth()/2-iWidth/2,m_rectDialogBoxFrame.y1-1+8,WHITE,lpSpeaker->m_strName);
			s_text.put(m_rectDialogBoxFrame.x1+m_rectDialogBoxFrame.getWidth()/2-iWidth/2+iNameWidth+2,m_rectDialogBoxFrame.y1+1+8,WHITE,"[%s]",g_pstrNpcName[lpSpeaker->m_wActorKind]);
		}

		int		iCenterOfWidth		=	m_rectDialogBoxFrame.x1+m_rectDialogBoxFrame.getWidth()/2;
		int		iTitleBarHeight		=	g_sprInterface2.getSpriteHeight(eDBI_TITLE);
		int		iBottomSpareHeight	=	25;

		if	(s_sbPopupWindow.isVisible())
			s_sbPopupWindow.draw();

		g_sprInterface2.Put(iCenterOfWidth-iWidth/2-5,5+g_iCorrectBI_Y/2,eDBI_NAME_LEFT);
		g_sprInterface2.Put(iCenterOfWidth+iWidth/2+5,5+g_iCorrectBI_Y/2,eDBI_NAME_RIGHT);

		int	iPosSeperateBar	=	m_rectDialogBoxTextFrame.y2+iBottomSpareHeight;

		g_sprInterface2.Put(m_rectDialogBoxFrame.x1+3,iTitleBarHeight+g_iCorrectBI_Y/2,eDBI_DECO_LT);
		g_sprInterface2.Put(m_rectDialogBoxFrame.x2-3,iTitleBarHeight+g_iCorrectBI_Y/2,eDBI_DECO_RT);
		g_sprInterface2.Put(m_rectDialogBoxFrame.x1+3,iPosSeperateBar,eDBI_DECO_LD);
		g_sprInterface2.Put(m_rectDialogBoxFrame.x2-3,iPosSeperateBar,eDBI_DECO_RD);

		g_sprInterface2.Put(m_rectDialogBoxFrame.x1+2,iPosSeperateBar-2,eDBI_SEPERATE_BAR);

//		cDRAW::Box(0x7fff,&m_rectDialogBoxTextFrame);

		s_text13.setLineGap(m_iSpeechLineGap);

	//	대화 출력
		s_text13.put(m_rectDialogBoxTextFrame.x1,m_rectDialogBoxTextFrame.y1,
						m_rectDialogBoxTextFrame.getWidth(),m_rectDialogBoxTextFrame.getHeight(),s_sbPopupWindow.m_iCurrentPos,WHITE,strDialogText);

	//	선택문들 출력
		{
			s_text13.setLineGap(m_iSelectSpeechLineGap);
			s_text13.disableFitRightEdge();

			for (int i=0;i<m_speech.m_wContentsCount;i++)
			{
				cRECT	rect;
				int		iXP	=	12;

				rect.set(&m_speech.m_arectSelectSpeech[i]);

				char*	lpstrSpeech	=	m_speech.getSelectSpeech(i)+7;
				char strSelectSpeechText[1024] ="";
				s_text.someCodeIgnoreOff();	
				CheckInstruction(lpstrSpeech, strSelectSpeechText, 1024);

				lpstrSpeech = strSelectSpeechText;

				if	(lpstrSpeech[1]	==	')')
					lpstrSpeech++;
				if	(lpstrSpeech[0]	==	')')
					lpstrSpeech++;

				if	(i	>=	9)
					iXP	=	20;

				int		iHeight			=	m_rectDialogBoxTextFrame.y2-rect.y1+s_sbPopupWindow.m_iCurrentPos;

//				if	(iHeight	<=	0)
//					break;

//				rect.y1	-=	s_sbPopupWindow.m_iCurrentPos;
//				rect.y2	-=	s_sbPopupWindow.m_iCurrentPos;

				if	(	i==	m_iFocusSpeech)
				{
					cRECT	rectFill;

					if (m_iLineGap	==	0)
						rectFill.set(rect.x1-4,rect.y1-2-m_iLineGap/2,rect.x2+4,rect.y2);
					else
						rectFill.set(rect.x1-4,rect.y1-1-m_iLineGap/2,rect.x2+4,rect.y2-2);

					rectFill.y1	=	max(rectFill.y1,m_rectDialogBoxTextFrame.y1);
					rectFill.y2	=	min(rectFill.y2,m_rectDialogBoxTextFrame.y2);

					cDRAW::Fill(_LTBLUE,rectFill.x1,rectFill.y1,rectFill.x2,rectFill.y2);

					s_text13.put(rect.x1-iXP,rect.y1,LTYELLOW,"<b>%d<b>)",i+1);
					s_text13.put(rect.x1,rect.y1,rect.getWidth(),rect.getHeight(),0,LTYELLOW,lpstrSpeech);
				}
				else
				{
					s_text13.put(rect.x1-iXP,rect.y1,GOLDYELLOW,"<b>%d<b>)",i+1);
					s_text13.put(rect.x1,rect.y1,rect.getWidth(),rect.getHeight(),0,WHITE,lpstrSpeech);
				}
			}
#ifndef	_FOR_ENG
			s_text13.enableFitRightEdge();
#endif
			s_text13.setLineGap(2);
			s_text13.releaseClipArea();
		}

		s_text13.setLineGap(2);
		s_text13.releaseClipArea();

		s_bmMiddleWindowButton.draw();

		return;
	}

	int		i;
	char	*lpstrSpeech	=	m_speech.getSpeech();
	CActor	*lpSpeaker		=	&g_aActor[m_iTalker];
	cRECT	rectDialogFrame;
	cRECT	rectDialogBoxTextFrame;
	int iAddWidth	=	0;

	if (!(s_rectInterfaceSize.x2-s_rectInterfaceSize.x1))
		iAddWidth = m_iAddWidth;
	s_text.setLineGap(m_iSpeechLineGap);

//	대화상자 프레임 초기화
	if	(m_iSpeechHeight	==	-1)
		initDialogBoxFrame();

	rectDialogFrame.set(&m_rectDialogBoxFrame);
	rectDialogBoxTextFrame.set(&m_rectDialogBoxTextFrame);


	rectDialogFrame.x1 += iAddWidth;
	rectDialogFrame.x2 += iAddWidth;
	rectDialogBoxTextFrame.x1 += iAddWidth;
	rectDialogBoxTextFrame.x2 += iAddWidth;

	if	(!s_sbPopupWindow.isVisible())
		rectDialogFrame.x2	-=	16;

	s_ttDialogFrame.drawBox(&rectDialogFrame,RGB24To16(RGB(236,236,236)),32,TRUE);	//	다얄로그 박스 그리기
	g_sprInterface.Put(m_posArrow.x + iAddWidth,m_posArrow.y,eDIALOG_FRAME_ARROW);

	
	if	(m_speech.m_wDialogBoxSize	==	3	||	(m_speech.m_wDialogBoxSize	!=	2	&& m_wEventArea == 0xffff))
	{
		int	iHeadLeftSize	=	g_sprInterface2.getSpriteWidth(eDB_TITLE_LEFT_CAP);

		g_sprInterface2.put3PieceBar(rectDialogFrame.x1+iHeadLeftSize-2,rectDialogFrame.y1-3,eDB_TITLE_LEFT_CAP,rectDialogFrame.getWidth()-iHeadLeftSize*2+4);

		int	iNameWidth		=	s_textBold14.getPixelSize(lpSpeaker->m_strName);
		int	iKindWidth		=	s_text.getPixelSize(_ms("[%s]",g_pstrNpcName[lpSpeaker->m_wActorKind]));
		int	iWidth			=	iNameWidth+iKindWidth+2;

		s_textBold14.put(rectDialogFrame.x1+rectDialogFrame.getWidth()/2-iWidth/2,rectDialogFrame.y1-1,WHITE,lpSpeaker->m_strName);
		s_text.put(rectDialogFrame.x1+rectDialogFrame.getWidth()/2-iWidth/2+iNameWidth+2,rectDialogFrame.y1+1,WHITE,"[%s]",g_pstrNpcName[lpSpeaker->m_wActorKind]);
	}

	s_text.pushShadowStatus(FALSE);	//	그림자 없음
	s_text.setClipArea(&rectDialogBoxTextFrame);
	char	*lpstrDialog	=	m_speech.getSpeech();
	char strDialogText[1024] ="";

	
	// insu add
	//s_text.replaceText(strDialogText,lpstrDialog,dMSG_TARGET_NAME, g_hero.m_strName);
	CheckInstruction(lpstrDialog, strDialogText, 1024);
	// insu add end

//	대화 출력
	s_text.setLineGap(m_iSpeechLineGap);
	s_text.put(rectDialogBoxTextFrame.x1,rectDialogBoxTextFrame.y1-s_sbPopupWindow.m_iCurrentPos,rectDialogBoxTextFrame.getWidth(),rectDialogBoxTextFrame.getHeight(),0,0,strDialogText);

//	선택문들 출력
	{
		s_text.setLineGap(m_iSelectSpeechLineGap);
		s_text.disableFitRightEdge();

		for (i=0;i<m_speech.m_wContentsCount;i++)
		{
			cRECT	rect;

			rect.set(&m_speech.m_arectSelectSpeech[i]);

			lpstrSpeech		=	m_speech.getSelectSpeech(i);
			char strSpeechText[1024] ="";
			// insu add
			//s_text.replaceText(strSpeechText,lpstrSpeech,dMSG_TARGET_NAME, g_hero.m_strName);
			CheckInstruction(lpstrSpeech, strSpeechText, 1024);
			// insu add end

			int	iHeight		=	rectDialogBoxTextFrame.y2-rect.y1+s_sbPopupWindow.m_iCurrentPos;
			
			if	(iHeight	<=	0)
				break;
			rect.x1			+=	iAddWidth;
			rect.x2			+=	iAddWidth;
			rect.y1			-=	s_sbPopupWindow.m_iCurrentPos;
			rect.y2			-=	s_sbPopupWindow.m_iCurrentPos;
			
			if (i==m_iFocusSpeech)
			{
				cRECT	rectFill;
				
				if (m_iLineGap	==	0)
					rectFill.set(rect.x1-4,rect.y1-2-m_iLineGap/2,rect.x2+4,rect.y2);
				else
					rectFill.set(rect.x1-4,rect.y1-1-m_iLineGap/2,rect.x2+4,rect.y2-2);
				
				rectFill.y1	=	max(rectFill.y1,m_rectDialogBoxTextFrame.y1);
				rectFill.y2	=	min(rectFill.y2,m_rectDialogBoxTextFrame.y2);
				
				cDRAW::Fill(_LTBLUE,rectFill.x1,rectFill.y1,rectFill.x2,rectFill.y2);
				
				s_text.put(rect.x1,rect.y1,rect.getWidth(),rect.getHeight(),0,LTYELLOW,strSpeechText);
			}
			else
				s_text.put(rect.x1,rect.y1,rect.getWidth(),rect.getHeight(),0,0,strSpeechText);
		}
#ifndef	_FOR_ENG
		s_text.enableFitRightEdge();
#endif
	}
	
	s_text.setLineGap(2);
	s_text.popShadowStatus();
	s_text.releaseClipArea();

	if	(s_sbPopupWindow.isVisible())
	{
		s_sbPopupWindow.draw();	//	스크롤바
	}
}




//┌---------------------------------------------------------------┐
//│ 대화 메시지에서 명령어를 걸러낸다.                            │
//│ // check start 부터                                           │
//│ // check end 사이에서 명령어를 걸러낸다.                      │
//│                                                               │
//│ char* _lpstrDialog : 대화 메시지                              │
//│ char* _strPut      : 최종 대화 메시지                         │
//│ int _istrPutlen    : _strPut 길이                             │
//└---------------------------------------------------------------┘
void
cDialogBox::CheckInstruction(char* _lpstrDialog, char* _strPut, int _istrPutlen)
{	
	char lpstrDialog[dSPEECH_LENGTH];

	char strInstruction[64];
	memset(strInstruction, NULL, 64);
	memset(_strPut, '\0', _istrPutlen);

	// 원본을 카피한다.
	strncpy(lpstrDialog, _lpstrDialog, strlen(_lpstrDialog));

	int istrIndex = 0;
	BOOL bSave = FALSE;
	int istrlen = strlen(_lpstrDialog);
	for(int i=0 ; i< istrlen ; i++)
	{
		if (lpstrDialog[i] == '<' && !strlen(strInstruction))
			bSave = TRUE;
		if (lpstrDialog[i] == '>' && strlen(strInstruction))
		{
			bSave = FALSE;
			strncat(strInstruction, &lpstrDialog[i], 1);

			// check start
			if (!stricmp(strInstruction, dMSG_TARGET_NAME))
			{
				strcat(_strPut, g_hero.m_strName);
				memset(strInstruction, '\0', 64);
			}
			if (!stricmp(strInstruction, dMSG_TARGET_REBIRTH_LEVEL))
			{
				char strLevel[5]=" ";
				int iResult = g_hero.getAfterRebirthLevel();
				if (iResult != 0xffff)
					wsprintf(strLevel, "%d", iResult);
				strcat(_strPut, strLevel);
				memset(strInstruction, '\0', 64);
			}
			// check end
			
			if (strInstruction != NULL)
			{
				strcat(_strPut, strInstruction);
				memset(strInstruction, '\0', 64);
			}
			
			continue;
		}

		if (!bSave)
		{
			if (lpstrDialog[i] != NULL)
				strncat(_strPut, &lpstrDialog[i], 1);
		}
		else
		{
			strncat(strInstruction, &lpstrDialog[i], 1);
		}
	}
	if (strInstruction)
	{
		strcat(_strPut, strInstruction);
	}
}
// insu function add end

//
//복합대화 업데이트

void
cDialogBox::updateComplexDialog()
{
	if	(!isTalking())
	{
		m_iFocusSpeech	=	-1;
		return;
	}

	if	(m_posEventPosition.x	!=	g_lpHero->m_pos.x	||	m_posEventPosition.y	!=	g_lpHero->m_pos.y)
	{
		closeComplexDialog();

		return;
	}

	//	카르마 인터페이스 창 열기/닫기를 다른 방법으로 사용할 수 있어서 ..임시로 나둠..
	if(m_bDoNotCloseDialogBox )
	{
		{
			CPos	posAxis;
			g_am.getFocusPos(&posAxis);

			CActor* lpActor = g_am.getActor(m_iTalker);
		
			int	x	=	posAxis.x	-	(g_iScreenWidth - (s_rectInterfaceSize.x2-s_rectInterfaceSize.x1))/2;
			ClipValue(x,dTILE_XS,g_map.m_iPixelWidth	-g_iScreenWidth	-1-dTILE_XS);		//	클리핑
			
			x	=	g_map.m_posView.x + (g_map.m_posView.x - m_posOldView.x) - x;

			if((s_rectInterfaceSize.x2-s_rectInterfaceSize.x1))
				m_iAddWidth = x;
						
			int	iTitleHeight	=	dDIALOG_FRAME_HEIGHT_MARGIN;
			int	iTitleBarHeight	=	g_sprInterface2.getSpriteHeight(eDB_TITLE_LEFT_CAP);
			if(!(s_rectInterfaceSize.x2-s_rectInterfaceSize.x1))
				s_sbPopupWindow.set(m_rectDialogBoxFrame.x2+m_iAddWidth-12,m_rectDialogBoxFrame.y1+iTitleBarHeight,m_rectDialogBoxFrame.getHeight()-iTitleBarHeight);
		}
		
	}
	int iAddWidth	=	0;

	if(!(s_rectInterfaceSize.x2-s_rectInterfaceSize.x1))
		iAddWidth = m_iAddWidth;

	m_iFocusSpeech	=	-1;
	
	if	(m_iSpeechHeight	!=	-1)
	{
		if	(s_sbPopupWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
			cINPDEV::SetExclusive(eMOUSE_STATE_SCROLL_BAR);
		else
			cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SCROLL_BAR);

		if	(s_iWheelValue<0)
			s_sbPopupWindow.setPos(s_sbPopupWindow.m_iCurrentPos+10);
		if	(s_iWheelValue>0)
			s_sbPopupWindow.setPos(s_sbPopupWindow.m_iCurrentPos-10);

		if	(cINPDEV::IsExclusive())
			return;

		if	(s_isTouchLeftButton)
			m_bIsReleasedMouseButton	=	TRUE;

		if	(m_speech.m_wContentsCount	<=	0)
		{
			if	(m_bIsReleasedMouseButton	&&	s_isClickedLeftButton)
			{
				closeComplexDialog();

				return;
			}
		}

		for (int i=0;i<m_speech.m_wContentsCount;i++)
		{
			cRECT	rectDialogBoxFrame = m_rectDialogBoxFrame;

			rectDialogBoxFrame.x1 += iAddWidth;
			rectDialogBoxFrame.x2 += iAddWidth;
			CPos	pos = s_posMouse;
			
			if	(rectDialogBoxFrame.isIn(pos.x,pos.y))
			{
				cRECT	rectSelectSpeech	= m_speech.m_arectSelectSpeech[i];
				rectSelectSpeech.x1 += iAddWidth;
				rectSelectSpeech.x2 += iAddWidth;
				if	(rectSelectSpeech.isIn(pos.x,pos.y+s_sbPopupWindow.m_iCurrentPos))
				{
					m_iFocusSpeech	=	i;
					break;
				}
			}
		}
		if	(m_bIsReleasedMouseButton	&&	s_isClickedLeftButton	&&	m_iFocusSpeech	!=	-1)
			selectSpeech(m_iFocusSpeech);	//	대사를 선택했다.

		if	(s_inputChatting.isEnable() == FALSE)
			for (int iKey=KEY_1;iKey<=KEY_9;iKey++)
				if	(s_aTouched[iKey])
				{
					int	iFocus	=	iKey-KEY_1;

					if	(iFocus < m_speech.m_wContentsCount)
						selectSpeech(iFocus);	//	대사를 선택했다.
				}
	
	}

	if	(m_speech.m_wDialogBoxSize	==	3	||	(m_speech.m_wDialogBoxSize	!=	2	&&	m_wEventArea	==	0xffff))
	{
		DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

		if	(dwMenu	==	IDOK)
			closeComplexDialog();
	}
}



//
//	대사를 선택했다.
void
cDialogBox::selectSpeech(int _iSpeech)
{
	s_agent.sendSelectSpeech(m_speech.m_wOwner,m_speech.m_wSpeechIndex,_iSpeech,m_wEventArea);
}