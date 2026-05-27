#include "CWindowInterface.H"
#include "cActor.H"
#include "cSound.H"

cMainQuestEvent		g_gwMainQuestEvent;

#ifdef	_FOR_JAPAN
char	*l_lpstrMainQuestImageFile	=	"data/interface/game/main_quest_image_japan.sad";
#else
char	*l_lpstrMainQuestImageFile	=	"data/interface/game/main_quest_image.sad";
#endif

BOOL
cMainQuestEvent::openEpilog(int _iChapter,int _iImage,int _iDisplayImageTime)
{
	if	(m_bIsImageLoaded	==	FALSE)
	{
		if	(!g_anmMainQuestImage.Load(l_lpstrMainQuestImageFile,FALSE))
			return FALSE;
		if	(!g_sprMainQuestInterface.Load("data/interface/game/main_quest.sd",FALSE))
			return FALSE;
	}

	ClosePopupWindow();

	m_iChapter						=	_iChapter;
	s_iPopupInterface				=	ePIW_MAIN_QUEST_EVENT;
	m_iStep							=	eMQES_FADE_OUT_GAME_SCREEN_FOR_EPILOG;
	m_bIsImageLoaded				=	TRUE;
	m_dwDisplayChapterCommentValue	=	0;
	m_dwWaitForNextStepTime			=	0;
	m_iChapterImageFadeValue		=	64;
	m_bIsHideCursor					=	TRUE;
	m_iEpilogImageIndex				=	_iImage;
	m_iDisplayEpilogImageTime		=	_iDisplayImageTime;

	s_oggPlayer.stop();
	g_esm.play("main ending0.wav");

	g_canvas.fadeOut(0,1);

	return	TRUE;
}

BOOL
cMainQuestEvent::open(int _iChapter)
{
	if	(!g_lpHero)
		return	FALSE;

	if	(m_bIsImageLoaded	==	FALSE)
	{
		if	(!g_anmMainQuestImage.Load(l_lpstrMainQuestImageFile,FALSE))
			return FALSE;
		if	(!g_sprMainQuestInterface.Load("data/interface/game/main_quest.sd",FALSE))
			return FALSE;
	}

	ClosePopupWindow();

	m_iChapter			=	_iChapter;
	s_iPopupInterface	=	ePIW_MAIN_QUEST_EVENT;
	m_iStep				=	eMQES_FLY_EAGLE;
	m_bIsImageLoaded	=	TRUE;
	m_dwDisplayChapterCommentValue	=	0;
	m_dwWaitForNextStepTime			=	timeGetTime()+8500;
	m_iChapterImageFadeValue		=	64;
	m_bIsHideCursor					=	TRUE;
	m_iFadeValueForFlyEagle			=	3200;

	s_oggPlayer.stop();

	g_esm.play("Main start.wav");
	g_esm.play("main open0.wav");
	g_lpHero->stop();
	g_lpHero->setAnm(dACT_READY);
	g_lpHero->setDirect(dDIRECT_SW);

	g_floatEffect.add(g_lpHero->m_pos.x,g_lpHero->m_pos.y,g_im.m_wMainQuestEagle);

	return	TRUE;
}

void
cMainQuestEvent::close()
{
	s_iPopupInterface			=	ePIW_NONE;
	s_bIsFullScreenInterface	=	FALSE;
	m_iStep						=	eMQES_NONE;

	s_agent.sendEtcWork(eCEW_END_EVENT);

	PlayBgm();
}

void
cMainQuestEvent::draw()
{
	switch(m_iStep)
	{
		case	eMQES_FADE_IN_CHAPTER_NAME				:	//	챕터 이름 페이드 인
			drawFadeInChapterName();
			break;

		case	eMQES_DISPLAY_CHAPTER_NAME				:	//	챕터 이름 표시
			drawDisplayChapterName();
			break;
		case	eMQES_FADE_OUT_CHAPTER_NAME_INTERFACE	:	//	챕터 이름 인터페이스 페이드 아웃
			drawFadeOutChapterNameInterface();
			break;
		case	eMQES_FADE_IN_CHATER_INTERFACE			:	//	챕터 이미지 페이드 인
			drawFadeInChapterInterface();
			break;
		case	eMQES_FADE_OUT_CHAPTER_IMAGE			:	//	챕터 이미지 페이드 아웃
			drawFadeOutChapterImage();
			break;
		case	eMQES_DISPLAY_CHAPTER_CONTENTS			:	//	챕터 내용 표시
			drawDisplayChapterContents();
			break;
		case	eMQES_WAIT_TO_CLOSE						:	//	종료를 기다림
			drawWaitToClose();
			break;
		case	eMQES_FADE_OUT_EVENT_SCREEN				:	//	화면을 페이드 아웃 시킨다.
			drawFadeOutEventScreen();
			break;
		case	eMQES_FADE_IN_GAME_SCREEN				:	//	화면을 페이드 아웃 시킨다.
			drawFadeInGameScreen();
			break;
		case	eMQES_FINISH							:	//	화면을 페이드 인 시킨다.
			drawFinish();
			break;


		case	eMQES_FADE_IN_EPILOG_IMAGE				:
			drawFadeInEpilogImage();
			break;
		case	eMQES_DISPLAY_EPILOG_IMAGE				:
			drawDisplayEpilogImage();
			break;
		case	eMQES_FADE_OUT_EPILOG_IMAGE				:
			drawFadeOutEpilogImage();
			break;

		case	eMQES_FADE_IN_EPILOG					:
			drawFadeInEpilog();
			break;
		case	eMQES_DISPLAY_EPILOG_TEXT				:
			drawDisplayEpilogText();
			break;
		case	eMQES_WAIT_TO_EPILOG_CLOSE				:
			drawWaitEpilogClose();
			break;
		case	eMQES_FADE_OUT_EPILOG_SCREEN			:
			drawFadeOutEpilogScreen();
			break;
	}
}

BOOL
cMainQuestEvent::update()
{
	if	(s_iPopupInterface	!=	ePIW_MAIN_QUEST_EVENT)
		return	FALSE;

#ifdef	_DEBUG
	if	(s_aTouched[KEY_ESCAPE]	)
	{
		close();
		return	TRUE;
	}
#endif

	switch(m_iStep)
	{
		case	eMQES_FLY_EAGLE							:
			updateWaitEagle();
			break;
		case	eMQES_FADE_OUT_GAME_SCREEN				:	//	화면을 페이드 아웃 시킨다.
			updateFadeOutGameScreen();
			break;

		case	eMQES_FADE_IN_CHAPTER_NAME				:	//	챕터 이름 페이드 인
			updateFadeInChapterName();
			break;

		case	eMQES_DISPLAY_CHAPTER_NAME				:	//	챕터 이름 표시
			updateDisplayChapterName();
			break;
		case	eMQES_FADE_OUT_CHAPTER_NAME_INTERFACE	:	//	챕터 이름 인터페이스 페이드 아웃
			updateFadeOutChapterNameInterface();
			break;
		case	eMQES_FADE_IN_CHATER_INTERFACE			:	//	챕터 이미지 페이드 인
			updateFadeInChapterInterface();
			break;
		case	eMQES_FADE_OUT_CHAPTER_IMAGE			:	//	챕터 이미지 페이드 아웃
			updateFadeOutChapterImage();
			break;
		case	eMQES_DISPLAY_CHAPTER_CONTENTS			:	//	챕터 내용 표시
			updateDisplayChapterContents();
			break;
		case	eMQES_WAIT_TO_CLOSE						:	//	종료를 기다림
			updateWaitToClose();
			break;
		case	eMQES_FADE_OUT_EVENT_SCREEN				:	//	화면을 페이드 아웃 시킨다.
			updateFadeOutEventScreen();
			break;
		case	eMQES_FADE_IN_GAME_SCREEN				:	//	화면을 페이드 아웃 시킨다.
			updateFadeInGameScreen();
			break;
		case	eMQES_FINISH							:	//	화면을 페이드 인 시킨다.
			updateFinish();
			break;

		case	eMQES_FADE_OUT_GAME_SCREEN_FOR_EPILOG	:
			updateFadeOutGameScreenForEpilog();
			break;

		case	eMQES_FADE_IN_EPILOG_IMAGE				:
			updateFadeInEpilogImage();
			break;
		case	eMQES_DISPLAY_EPILOG_IMAGE				:
			updateDisplayEpilogImage();
			break;
		case	eMQES_FADE_OUT_EPILOG_IMAGE				:
			updateFadeOutEpilogImage();
			break;

		case	eMQES_FADE_IN_EPILOG					:
			updateFadeInEpilog();
			break;
		case	eMQES_DISPLAY_EPILOG_TEXT				:
			updateDisplayEpilogText();
			break;
		case	eMQES_WAIT_TO_EPILOG_CLOSE				:
			updateWaitEpilogClose();
			break;
		case	eMQES_FADE_OUT_EPILOG_SCREEN			:
			updateFadeOutEpilogScreen();
			break;
	}
	
	return	TRUE;
}

BOOL
cMainQuestEvent::isOpened()
{
	if	(s_iPopupInterface	!=	ePIW_MAIN_QUEST_EVENT)
		return	FALSE;

	return	TRUE;
}