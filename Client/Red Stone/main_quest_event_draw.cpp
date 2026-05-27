#include "CWindowInterface.H"
#include "mainQuest.H"

enum
{
	eMQI_MAIN_QUEST_BANNER,
	eMQI_CHAPTER_BANNER,
	eMQI_CHAPTER_BANNER_SHADOW,
	eMQI_CHAPTER_BANNER_NUMBER_1,
	eMQI_CHAPTER_BANNER_NUMBER_2,
	eMQI_CHAPTER_BANNER_NUMBER_3,
	eMQI_CHAPTER_BANNER_NUMBER_4,
	eMQI_CHAPTER_BANNER_NUMBER_5,
	eMQI_CHAPTER_BANNER_NUMBER_6,
	eMQI_CHAPTER_BANNER_NUMBER_7,
	eMQI_CHAPTER_BANNER_NUMBER_8,
	eMQI_ENTER_NORMAL,
	eMQI_ENTER_LIGHT,
	eMQI_ENTER_SHADOW,
};

const	int	c_iMQE_BannerVerticalPos			=	208;
const	int	c_iMQE_TitleVerticalPos				=	290;
const	int	c_iMQE_TextChapterVerticalPos		=	360;
const	int	c_iMQE_TextChapterIndexVerticalPos	=	380;

void
cMainQuestEvent::drawFadeInChapterName()
{
	drawDisplayChapterName();
}

void
cMainQuestEvent::drawDisplayChapterName()
{
	g_sprMainQuestInterface.Put(g_iScreenWidth/2,c_iMQE_BannerVerticalPos,eMQI_MAIN_QUEST_BANNER);

	s_textBoldLarge30.put(m_posChapterName.x,m_posChapterName.y,WHITE,g_chapterManager.getChapterName(m_iChapter));
	s_textBold14.cPut(g_iScreenWidth/2,c_iMQE_TextChapterVerticalPos,WHITE,"chapter");
	s_textLarge50.cPut(g_iScreenWidth/2,c_iMQE_TextChapterIndexVerticalPos,WHITE,"%d",m_iChapter+1);

}

void
cMainQuestEvent::drawFadeOutChapterNameInterface()
{
	drawDisplayChapterName();
}

void
cMainQuestEvent::drawChapterInterface()
{
	cDRAW::Clear();

	int iRate = 100;
	int iAddX = 0;
	if (g_iScreenWidth != 800)
	{
		iRate = (int)(g_iScreenWidth*100)/800;
		iAddX = g_iScreenWidth-800;
	}

	g_anmMainQuestImage.Put(0, 0, m_iChapter+1,0,0,iRate,iRate,dPUT_ALPHA_BLENDING,m_iChapterImageFadeValue);
	g_anmMainQuestImage.Put(0, 0, 0, 0, 0, iRate, iRate);

	g_sprMainQuestInterface.Put(0,5,eMQI_CHAPTER_BANNER);
	g_sprMainQuestInterface.Put(0,5,eMQI_CHAPTER_BANNER_NUMBER_1+m_iChapter);
	g_sprMainQuestInterface.Put(0,5,eMQI_CHAPTER_BANNER_SHADOW,100,100,dPUT_HALF_BLENDING);

	s_textBoldLarge30.put(m_posChapterName.x,m_posChapterName.y,WHITE,g_chapterManager.getChapterName(m_iChapter));

	if	(m_dwDisplayChapterCommentValue)
	{
		char	*lpstrChapterComment	=	g_chapterManager.getChapterComment(m_iChapter,m_dwDisplayChapterCommentValue);

		s_textBold16M.disableFitRightEdge();
		s_textBold16M.setLineGap(11);
		s_textBold16M.put(78+iAddX/2, 97, 644, g_iScreenHeight, 0, WHITE, lpstrChapterComment);
#ifndef	_FOR_ENG
		s_textBold16M.enableFitRightEdge();
#endif
		s_textBold16M.setLineGap(2);
	}
}

void
cMainQuestEvent::drawFadeInChapterInterface()
{
	drawChapterInterface();
}

void
cMainQuestEvent::drawFadeOutChapterImage()
{
	drawChapterInterface();
}

void
cMainQuestEvent::drawDisplayChapterContents()
{
	drawChapterInterface();
}

void
cMainQuestEvent::drawWaitToClose()
{
	drawChapterInterface();

	int iAddX = 0;
	int iAddY = 0;
	int iReduceY = 55;
	if (g_iScreenWidth != 800)
	{
		int iRate = (int)(g_iScreenWidth*100)/800;
		iReduceY = (iReduceY*iRate)/100;
		iAddX = g_iScreenWidth-800;
		iAddY = g_iScreenHeight-600;
	}

	if	(timeGetTime()%1000 < 500)
	{
		g_sprMainQuestInterface.Put(644+iAddX,g_iScreenHeight-iReduceY/*545*/,eMQI_ENTER_NORMAL,100,100);
		g_sprMainQuestInterface.Put(644+iAddX,g_iScreenHeight-iReduceY/*545*/,eMQI_ENTER_SHADOW,100,100,dPUT_HALF_BLENDING);
	}
	else
	{
		g_sprMainQuestInterface.Put(644+iAddX,g_iScreenHeight-iReduceY/*545*/,eMQI_ENTER_LIGHT,100,100);
		g_sprMainQuestInterface.Put(644+iAddX,g_iScreenHeight-iReduceY/*545*/,eMQI_ENTER_SHADOW,100,100,dPUT_HALF_BLENDING);
	}
}

void
cMainQuestEvent::drawFadeOutEventScreen()
{
	drawWaitToClose();
}

void
cMainQuestEvent::drawFadeInGameScreen()
{
}

void
cMainQuestEvent::drawFinish()
{
}


void
cMainQuestEvent::drawEpilog()
{
	cDRAW::Clear();

	g_anmMainQuestImage.Put(0,0,m_iChapter+1,0,1,100,100,dPUT_ALPHA_BLENDING,m_iChapterImageFadeValue);
	g_anmMainQuestImage.Put(0,0,0,0,0);

	if	(m_dwDisplayChapterCommentValue)
	{
		char	*lpstrChapterComment	=	g_chapterManager.getChapterEpilogComment(m_iChapter,m_dwDisplayChapterCommentValue);

		s_textBold16M.disableFitRightEdge();
		s_textBold16M.setLineGap(6);
		s_textBold16M.put(100,120,600,g_iScreenHeight,0,WHITE,lpstrChapterComment);
		s_textBold16M.setLineGap(2);
#ifndef	_FOR_ENG
		s_textBold16M.enableFitRightEdge();
#endif
	}
}

void
cMainQuestEvent::drawFadeInEpilogImage()
{
	drawDisplayEpilogImage();
}

void
cMainQuestEvent::drawDisplayEpilogImage()
{
	cDRAW::Clear();

	g_sprMainQuestEnding.Put(g_iScreenWidth/2,g_iScreenHeight/2,m_iEpilogImageIndex-1);
}

void
cMainQuestEvent::drawFadeOutEpilogImage()
{
	drawDisplayEpilogImage();
}


void
cMainQuestEvent::drawFadeInEpilog()
{
	drawEpilog();
}

void
cMainQuestEvent::drawDisplayEpilogText()
{
	drawEpilog();
}

void
cMainQuestEvent::drawWaitEpilogClose()
{
	drawEpilog();

	if	(timeGetTime()%1000 < 500)
	{
		g_sprMainQuestInterface.Put(644,545,eMQI_ENTER_NORMAL,100,100);
		g_sprMainQuestInterface.Put(644,545,eMQI_ENTER_SHADOW,100,100,dPUT_HALF_BLENDING);
	}
	else
	{
		g_sprMainQuestInterface.Put(644,545,eMQI_ENTER_LIGHT,100,100);
		g_sprMainQuestInterface.Put(644,545,eMQI_ENTER_SHADOW,100,100,dPUT_HALF_BLENDING);
	}
}

void
cMainQuestEvent::drawFadeOutEpilogScreen()
{
	drawWaitEpilogClose();
}