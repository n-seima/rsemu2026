#include "CWindowInterface.H"
#include "mainQuest.H"

const	int	c_iMQE_TitleVerticalPos				=	290;

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

BOOL
cMainQuestEvent::updateWaitEagle()
{
	m_iFadeValueForFlyEagle	-=	50;

	m_iFadeValueForFlyEagle	=	max(1600,m_iFadeValueForFlyEagle);

	if	(timeGetTime()	>=	m_dwWaitForNextStepTime	)
	{
		m_iStep					=	eMQES_FADE_OUT_GAME_SCREEN;
		m_dwWaitForNextStepTime	=	0;
		g_canvas.fadeOut(0,1);
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFadeOutGameScreenForEpilog()
{
	if	(g_canvas.isCompleteFadeOut())
	{
		if	(m_iEpilogImageIndex)
			m_iStep						=	eMQES_FADE_IN_EPILOG_IMAGE;
		else
			m_iStep						=	eMQES_FADE_IN_EPILOG;

		g_canvas.fadeIn(0,1);

		m_dwWaitForNextStepTime		=	0;
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFadeInEpilogImage()
{
	if	(m_dwWaitForNextStepTime	==	0)
	{
		if	(g_canvas.isCompleteFadeIn())
			m_dwWaitForNextStepTime		=	timeGetTime()+1000;

		return	TRUE;
	}

	if	(timeGetTime()	>=	m_dwWaitForNextStepTime	)
	{
		m_iStep								=	eMQES_DISPLAY_EPILOG_IMAGE;
		m_dwLastUpdateTimeChapterChar		=	0;
		m_dwWaitForNextStepTime				=	timeGetTime()+1000*m_iDisplayEpilogImageTime;

		return	TRUE;
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateDisplayEpilogImage()
{
	if	(m_dwWaitForNextStepTime	==	0)
	{
		if	(m_dwDisplayChapterCommentValue	>=	m_dwChapterCommentSize)
			m_dwWaitForNextStepTime			=	timeGetTime()+1500;		//	2.5초간 딜레이

		return	TRUE;
	}

	if	(timeGetTime()	>=	m_dwWaitForNextStepTime	)
	{
		m_iStep					=	eMQES_FADE_OUT_EPILOG_IMAGE;
		m_dwWaitForNextStepTime	=	0;
		g_canvas.fadeOut(0,1);

		return	TRUE;
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFadeOutEpilogImage()
{
	if	(g_canvas.isCompleteFadeOut())
	{
		m_iStep						=	eMQES_FADE_IN_EPILOG;

		g_canvas.fadeIn(0,1);

		m_dwWaitForNextStepTime		=	0;
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFadeInEpilog()
{
	if	(m_dwWaitForNextStepTime	==	0)
	{
		if	(g_canvas.isCompleteFadeIn())
			m_dwWaitForNextStepTime		=	timeGetTime()+1000;

		m_iChapterImageFadeValue--;

		m_iChapterImageFadeValue	=	max(m_iChapterImageFadeValue,16);

		return	TRUE;
	}

	if	(timeGetTime()	>=	m_dwWaitForNextStepTime	)
	{
		m_iStep								=	eMQES_DISPLAY_EPILOG_TEXT;
		m_dwLastUpdateTimeChapterChar		=	0;
		m_dwWaitForNextStepTime				=	0;

		m_dwChapterCommentSize				=	g_chapterManager.getChapterEpilogCommentSize(m_iChapter);

		return	TRUE;
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateDisplayEpilogText()
{
	m_iChapterImageFadeValue--;

	m_iChapterImageFadeValue	=	max(m_iChapterImageFadeValue,16);

	if	(timeGetTime()	>=	m_dwLastUpdateTimeChapterChar	+	50)
	{
		m_dwLastUpdateTimeChapterChar	=	timeGetTime();
		m_dwDisplayChapterCommentValue++;
	}

	if	(m_dwWaitForNextStepTime	==	0)
	{
		if	(m_dwDisplayChapterCommentValue	>=	m_dwChapterCommentSize)
			m_dwWaitForNextStepTime			=	timeGetTime()+1500;		//	2.5초간 딜레이

		return	TRUE;
	}

	if	(timeGetTime()	>=	m_dwWaitForNextStepTime	)
	{
		m_bIsHideCursor						=	FALSE;
		m_iStep								=	eMQES_WAIT_TO_EPILOG_CLOSE;

		return	TRUE;
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateWaitEpilogClose()
{
	if	(s_isKbhit)
	{
		m_iStep								=	eMQES_FADE_OUT_EPILOG_SCREEN;
		g_canvas.fadeOut(0,1);
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFadeOutEpilogScreen()
{
	if	(g_canvas.isCompleteFadeOut())
	{
		g_canvas.fadeIn(0,1);
		m_iStep						=	eMQES_FADE_IN_GAME_SCREEN;
		s_bIsFullScreenInterface	=	FALSE;
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFadeOutGameScreen()
{
	if	(g_canvas.isCompleteFadeOut())
	{
		int	iTextWidth	=	s_textBoldLarge30.getPixelSize(g_chapterManager.getChapterName(m_iChapter));
		
		m_posChapterNameBegin.Set(g_iScreenWidth/2-iTextWidth/2,c_iMQE_TitleVerticalPos);
		m_posChapterNameDest.Set(142,16);

		m_posChapterName			=	m_posChapterNameBegin;
		s_bIsFullScreenInterface	=	TRUE;

		m_iStep						=	eMQES_FADE_IN_CHAPTER_NAME;
		g_canvas.fadeIn(0,1);
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFadeInChapterName()
{
	if	(g_canvas.isCompleteFadeIn())
	{
		m_iStep						=	eMQES_DISPLAY_CHAPTER_NAME;
		m_dwWaitForNextStepTime		=	timeGetTime()+1000;
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateDisplayChapterName()
{
	if	(timeGetTime()	>=	m_dwWaitForNextStepTime	)	//	1초간 화면 출력
	{
		g_canvas.fadeOut(0,1);
		m_iStep			=	eMQES_FADE_OUT_CHAPTER_NAME_INTERFACE;	//	
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFadeOutChapterNameInterface()
{
	if	(g_canvas.isCompleteFadeOut())
	{
		g_canvas.fadeIn(0,1);
		m_iStep						=	eMQES_FADE_IN_CHATER_INTERFACE;
		m_posChapterName			=	m_posChapterNameDest;
		m_dwWaitForNextStepTime		=	0;

		return	TRUE;
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFadeInChapterInterface()
{
	if	(m_dwWaitForNextStepTime	==	0)
	{
		if	(g_canvas.isCompleteFadeIn())
			m_dwWaitForNextStepTime			=	timeGetTime()+1000;		//	2.5초간 딜레이

		return	TRUE;
	}

	if	(timeGetTime()	>=	m_dwWaitForNextStepTime	)
	{
		m_iStep						=	eMQES_FADE_OUT_CHAPTER_IMAGE;
		m_iChapterImageFadeValue	=	32;
		m_dwWaitForNextStepTime		=	0;

		return	TRUE;
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFadeOutChapterImage()
{
	if	(m_dwWaitForNextStepTime	==	0)
	{
		if	(m_iChapterImageFadeValue	==	16)
			m_dwWaitForNextStepTime			=	timeGetTime()+1000;		//	2.5초간 딜레이

		m_iChapterImageFadeValue--;

		return	TRUE;
	}

	if	(timeGetTime()	>=	m_dwWaitForNextStepTime	)
	{
		int	iLineThick	=	s_textBold18.getLineThick();
		s_textBold18.setValidLineHeight(iLineThick);

		m_dwChapterCommentSize			=	g_chapterManager.getChapterCommentSize(m_iChapter);
		m_dwDisplayChapterCommentValue	=	1;
		m_iStep							=	eMQES_DISPLAY_CHAPTER_CONTENTS;
		m_dwLastUpdateTimeChapterChar	=	timeGetTime();
		m_dwWaitForNextStepTime			=	0;

		return	TRUE;
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateDisplayChapterContents()
{
	if	(timeGetTime()	>=	m_dwLastUpdateTimeChapterChar	+	50)
	{
		m_dwLastUpdateTimeChapterChar	=	timeGetTime();
		m_dwDisplayChapterCommentValue++;
	}

	if	(m_dwWaitForNextStepTime	==	0)
	{
		if	(m_dwDisplayChapterCommentValue	>=	m_dwChapterCommentSize)
			m_dwWaitForNextStepTime			=	timeGetTime()+1500;		//	2.5초간 딜레이

		return	TRUE;
	}

	if	(timeGetTime()	>=	m_dwWaitForNextStepTime	)
	{
		m_bIsHideCursor						=	FALSE;
		m_iStep								=	eMQES_WAIT_TO_CLOSE;

		return	TRUE;
	}


	return	TRUE;
}

BOOL
cMainQuestEvent::updateWaitToClose()
{
	if	(s_isKbhit)
	{
		m_iStep								=	eMQES_FADE_OUT_EVENT_SCREEN;
		g_canvas.fadeOut(0,1);
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFadeOutEventScreen()
{
	if	(g_canvas.isCompleteFadeOut())
	{
		g_canvas.fadeIn(0,1);
		m_iStep						=	eMQES_FADE_IN_GAME_SCREEN;
		s_bIsFullScreenInterface	=	FALSE;
	}

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFadeInGameScreen()
{
	if	(g_canvas.isCompleteFadeIn())
		close();

	return	TRUE;
}

BOOL
cMainQuestEvent::updateFinish()
{
	return	TRUE;
}