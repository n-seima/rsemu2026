#include "CGamePlay.H"
#include "CHero.H"
#include "CActor.H"
#include "CGamePlayInterface.H"
#include "CMessage.H"
#include "CWindowInterface.H"
#include "commonClass.H"

cHelpScreen	g_gwHelp;

//char				*g_strHelp[10];
cSimpleHelpTextInfo	g_astrSimpleHelp[10];

#define		dHELP_BOX_TOP					50
#define		dHELP_BOX_HEIGHT				370
#define		dEVENT_FIELD_HELP_BOX_HEIGHT	520
#define		dHELP_BOX_WIDTH					460

BOOL
cHelpScreen::readSimpleText()
{
	for (int i=0;i<10;i++)
		g_astrSimpleHelp[i].reset();

	if	(m_iHelpType	==	eHT_WORD_QUIZ	||	m_iHelpType	==	eHT_OX_QUIZ)
	{
		cFILE	file;

		if	(m_iHelpType	==	eHT_WORD_QUIZ)
			if	(!file.Open("data/wordQuizHelp.txt","rb"))
				return	FALSE;
		if	(m_iHelpType	==	eHT_OX_QUIZ)
			if	(!file.Open("data/oxQuizHelp.txt","rb"))
				return	FALSE;

		g_astrSimpleHelp[0].m_pstrText	=	new char [file.Length+1];

		file.Read(g_astrSimpleHelp[0].m_pstrText,file.Length);

		g_astrSimpleHelp[0].m_pstrText[file.Length]	=	NULL;

		file.Close();

		return	TRUE;
	}

	CDecoder	decoder;

	if	(!decoder.Upload("data/help.txt",NULL))
		return	FALSE;

	int		iIndex		=	0;

	while(1)
	{
		char	*lpstrText	=	decoder.getWord();

		if	(STRICMP(lpstrText,"__end")	==	0)
			break;
		if	(STRICMP(lpstrText,"isEndCode")	==	0)
			break;

		if	(STRICMP(lpstrText,"tab")	==	0)
		{
			lpstrText		=	decoder.getStream();
			strcpy(g_astrSimpleHelp[iIndex].m_strTitle,lpstrText);
			lpstrText		=	decoder.getStream();
			g_astrSimpleHelp[iIndex].setText(lpstrText);

			iIndex++;
			continue;
		}
	}

	return	TRUE;
}

void
cHelpScreen::open()
{
	if	(s_bIsOXQuizField)
	{
		m_iHelpType		=	eHT_OX_QUIZ;
		m_iHelpBoxHeight=	dEVENT_FIELD_HELP_BOX_HEIGHT;
	}
	else
	if	(s_bIsWordQuizField)
	{
		m_iHelpType		=	eHT_WORD_QUIZ;
		m_iHelpBoxHeight=	dEVENT_FIELD_HELP_BOX_HEIGHT;
	}
	else
	{
		m_iHelpType		=	eHT_GAME;
		m_iHelpBoxHeight=	dHELP_BOX_HEIGHT;
	}


	if	(!readSimpleText())
		return;
	
	m_iHelpPixelHeight	=	0;
	m_iSelectSimpleHelp	=	0;

	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();
	
	s_iPopupInterface	=	ePIW_HELP_SCREEN;

	if	(m_iHelpType	==	eHT_GAME)
	{
		g_anmHelp.Load("data/interface/game/help.sad",FALSE,NULL,NULL);
		s_bmRadioBox.reset();

		int	iIndex	=	0;
		int	iX		=	(g_iScreenWidth/2-dHELP_BOX_WIDTH/2)+10,iY=g_iCorrectBI_Y;

		while(1)
		{
			if	(g_astrSimpleHelp[iIndex].m_pstrText	==	NULL)
				break;

			int	iDx	=	0;

			for (int i=0;i<iIndex;i++)
				iDx	+=	(s_text.getPixelSize(g_astrSimpleHelp[iIndex].m_strTitle)+30);

			s_bmRadioBox.addImageBar(g_astrSimpleHelp[iIndex].m_strTitle,iIndex,iX+iDx	,dHELP_BOX_TOP+10 +g_iCorrectBI_Y/2,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);

			iIndex++;
		}

		s_bmRadioBox.check(0);
	}

	int		iX		=	g_iScreenWidth/2-dHELP_BOX_WIDTH/2,iY	=	dHELP_BOX_TOP+g_iCorrectBI_Y/2;
	cRECT	rect;

	rect.set(iX+10,iY+40,iX+dHELP_BOX_WIDTH-20,iY+m_iHelpBoxHeight-5);

	m_iSelectSimpleHelp	=	0;
	s_text.setValidLineHeight(6);
	m_iHelpPixelHeight	=	s_text.getTextHeight(rect.getWidth(),g_astrSimpleHelp[0].m_pstrText);
	s_text.setValidLineHeight();

	if	(m_iHelpType	==	eHT_GAME)
	{
		s_sbPopupWindow.set(iX+dHELP_BOX_WIDTH-16,iY+35,m_iHelpBoxHeight-40);
		s_sbPopupWindow.setSize(m_iHelpPixelHeight+20,m_iHelpBoxHeight-40);
		s_sbPopupWindow.setPos(0);
	}
	else
	{
		s_sbPopupWindow.set(iX+dHELP_BOX_WIDTH-16,iY+10,m_iHelpBoxHeight-15);
		s_sbPopupWindow.setSize(m_iHelpPixelHeight+20,m_iHelpBoxHeight-15);
		s_sbPopupWindow.setPos(0);
	}
}


void
cHelpScreen::close()
{
	cINPDEV::ReleaseExclusive();
	s_iPopupInterface	=	ePIW_NONE;
	s_bmRadioBox.reset();
}


void
cHelpScreen::draw()
{
	if	(s_iPopupInterface	!=	ePIW_HELP_SCREEN)
		return;

	cRECT	rect;

	if	(m_iHelpType	==	eHT_GAME)
	{
		g_anmHelp.m_sprite.PutShadow(g_iCorrectBI_X,g_iCorrectBI_Y,0);
		g_anmHelp.m_sprite.Put(g_iCorrectBI_X,g_iCorrectBI_Y,0);

		int	iIndex	=	0;

		while(1)
		{
			cHelpTextInfo	*lpInfo	=	&g_aHelpTextInfo[iIndex];

			if	(lpInfo->m_wTextOutputMethod	==	0xffff)
				break;

			int	iDX	=	0;

			if	(lpInfo->m_wTextShape==	eHTS_BOX)
			{
				cRECT	rect;

				int		iPixelSize	=	s_text.getPixelSize(lpInfo->m_strText)+10;
				int		iX			=	lpInfo->m_wX+g_iCorrectBI_X;
				int		iY			=	lpInfo->m_wY+g_iCorrectBI_Y;

				if	(lpInfo->m_wAnm	!=	0xffff)
					iDX			=	12;

				if	(lpInfo->m_wTextOutputMethod	==	eHTP_CENTER)
					iX		-=	(iPixelSize/2+iDX);
				if	(lpInfo->m_wTextOutputMethod	==	eHTP_LEFT)
					iX		-=	3;
				if	(lpInfo->m_wTextOutputMethod	==	eHTP_RIGHT)
					iX		-=	(iPixelSize+iDX-5);

				if	(lpInfo->m_wAnm	!=	0xffff)
					iPixelSize	+=	12;

				iY		-=	3;

				rect.set(iX,iY,iX+iPixelSize,iY+19-2);

				s_ttCommon.drawBox(&rect,0,32);

				if (lpInfo->m_wAnm	!=	0xffff)
					g_anmHelp.PutReg(iX+4,iY+3,lpInfo->m_wAnm,0,s_iFrameCounter/16%2);
			}

			
			s_text.pushShadowStatus(FALSE);
			if	(lpInfo->m_wTextOutputMethod	==	eHTP_CENTER)
				s_text.cPut(lpInfo->m_wX+g_iCorrectBI_X,lpInfo->m_wY+g_iCorrectBI_Y,WHITE,lpInfo->m_strText);
			if	(lpInfo->m_wTextOutputMethod	==	eHTP_LEFT)
				s_text.put(lpInfo->m_wX+iDX+g_iCorrectBI_X,lpInfo->m_wY+g_iCorrectBI_Y,WHITE,lpInfo->m_strText);
			if	(lpInfo->m_wTextOutputMethod	==	eHTP_RIGHT)
				s_text.putRF(lpInfo->m_wX+g_iCorrectBI_X,lpInfo->m_wY+g_iCorrectBI_Y,WHITE,lpInfo->m_strText);
			s_text.popShadowStatus();

			iIndex++;
		}
	}

	if	(m_iHelpType	==	eHT_WORD_QUIZ	||	m_iHelpType	==	eHT_OX_QUIZ)
	{
		int	iX		=	g_iScreenWidth/2-dHELP_BOX_WIDTH/2,iY	=	dHELP_BOX_TOP+g_iCorrectBI_Y/2;

		rect.set(iX,iY,iX+dHELP_BOX_WIDTH,iY+m_iHelpBoxHeight);

		s_ttCommon.drawBox(&rect,0,32);
		rect.set(iX+10,iY+10,iX+dHELP_BOX_WIDTH-20,iY+m_iHelpBoxHeight-10);

		s_text.setClipArea(&rect);
		s_text.setValidLineHeight(6);

		int	iLength	=	strlen(g_astrSimpleHelp[0].m_pstrText);

		s_text.put(rect.x1,rect.y1-s_sbPopupWindow.m_iCurrentPos,rect.getWidth(),0xffff,0,WHITE,g_astrSimpleHelp[0].m_pstrText);
		s_text.setValidLineHeight();
		s_text.setClipArea(0,0,g_iScreenWidth,g_iScreenHeight);

		cDRAW::Line(0xffff,iX+10,iY+30,iX+dHELP_BOX_WIDTH-10,iY+30);


// 		s_text.setClipArea(&rect);
// 		s_text.setValidLineHeight(6);
// 		int	iLength	=	strlen(g_astrSimpleHelp[iMenu].m_pstrText);
// 		s_text.put(rect.x1,rect.y1-s_sbPopupWindow.m_iCurrentPos,rect.getWidth(),0xffff,0,WHITE,g_astrSimpleHelp[iMenu].m_pstrText);
// 		s_text.setValidLineHeight();
// 		s_text.setClipArea(0,0,g_iScreenWidth,g_iScreenHeight);
// 
// 		s_sbPopupWindow.draw();
// 		s_bmRadioBox.draw();
	}
	else
	{
		int	iMenu	=	s_bmRadioBox.getCheckedMenu();

		int	iX		=	g_iScreenWidth/2-dHELP_BOX_WIDTH/2,iY	=	dHELP_BOX_TOP+g_iCorrectBI_Y/2;

		rect.set(iX,iY,iX+dHELP_BOX_WIDTH,iY+m_iHelpBoxHeight);

		s_ttCommon.drawBox(&rect,0,32);

		cDRAW::Line(0xffff,iX+10,iY+30,iX+dHELP_BOX_WIDTH-10,iY+30);

		rect.set(iX+10,iY+40,iX+dHELP_BOX_WIDTH-10,iY+m_iHelpBoxHeight-10);

		s_text.setClipArea(&rect);
		s_text.setValidLineHeight(6);
		int	iLength	=	strlen(g_astrSimpleHelp[iMenu].m_pstrText);
		s_text.put(rect.x1,rect.y1-s_sbPopupWindow.m_iCurrentPos,rect.getWidth(),0xffff,0,WHITE,g_astrSimpleHelp[iMenu].m_pstrText);
		s_text.setValidLineHeight();
		s_text.setClipArea(0,0,g_iScreenWidth,g_iScreenHeight);

		s_bmRadioBox.draw();
	}

	s_sbPopupWindow.draw();
}

void
cHelpScreen::update()
{
	if	(s_iPopupInterface	!=	ePIW_HELP_SCREEN)
		return;

	cRECT	rect;
	int		iX		=	g_iScreenWidth/2-dHELP_BOX_WIDTH/2,iY	=	dHELP_BOX_TOP;

	rect.set(iX+10,iY+40,iX+dHELP_BOX_WIDTH-10,iY+m_iHelpBoxHeight-5);

	if	(m_iHelpType	==	eHT_GAME)
	{
		DWORD	dwMenu	=	s_bmRadioBox.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		if	(dwMenu	!=	0xffffffff	&&	dwMenu	!=	m_iSelectSimpleHelp)
		{
			m_iSelectSimpleHelp	=	dwMenu;
			s_text.setValidLineHeight(6);
			m_iHelpPixelHeight	=	s_text.getTextHeight(rect.getWidth(),g_astrSimpleHelp[dwMenu].m_pstrText);
			s_text.setValidLineHeight();
			s_sbPopupWindow.setSize(m_iHelpPixelHeight+20,m_iHelpBoxHeight-40);
			s_sbPopupWindow.setPos(0);
		}
	}

	if	(rect.isIn(s_posMouse.x,s_posMouse.y))
	{
		if	(s_iWheelValue<0)
			s_sbPopupWindow.setPos(s_sbPopupWindow.m_iCurrentPos+64);
		if	(s_iWheelValue>0)
			s_sbPopupWindow.setPos(s_sbPopupWindow.m_iCurrentPos-64);
	}


	if	(s_sbPopupWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
		cINPDEV::SetExclusive(eMOUSE_STATE_SCROLL_BAR);
	else
		cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SCROLL_BAR);
}

BOOL
cHelpScreen::isOpened()
{
	if	(s_iPopupInterface	==	ePIW_HELP_SCREEN)
		return	TRUE;

	return	FALSE;
}