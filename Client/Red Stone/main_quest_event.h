#ifndef	_classMainQuestEvent_h
#define	_classMainQuestEvent_h

#include "CGamePlay.h"

enum
{
	eMQES_NONE								,	//	암것도 안하고 있다.
	eMQES_FLY_EAGLE							,	//	독수리 날다!
	eMQES_FADE_OUT_GAME_SCREEN				,	//	화면을 페이드 아웃 시킨다.
	eMQES_FADE_IN_CHAPTER_NAME				,	//	챕터 이름 페이드 인
	eMQES_DISPLAY_CHAPTER_NAME				,	//	챕터 이름 표시
	eMQES_FADE_OUT_CHAPTER_NAME_INTERFACE	,	//	챕터 이름 인터페이스 페이드 아웃
	eMQES_FADE_IN_CHATER_INTERFACE			,	//	챕터 이미지 페이드 인
	eMQES_FADE_OUT_CHAPTER_IMAGE			,	//	챕터 이미지 페이드 아웃
	eMQES_DISPLAY_CHAPTER_CONTENTS			,	//	챕터 내용 표시
	eMQES_WAIT_TO_CLOSE						,	//	종료를 기다림
	eMQES_FADE_OUT_EVENT_SCREEN				,	//	화면을 페이드 아웃 시킨다.
	eMQES_FADE_IN_GAME_SCREEN				,	//	화면을 페이드 아웃 시킨다.
	eMQES_FINISH							,	//	화면을 페이드 인 시킨다.

	eMQES_FADE_OUT_GAME_SCREEN_FOR_EPILOG	,

	eMQES_FADE_IN_EPILOG_IMAGE				,
	eMQES_DISPLAY_EPILOG_IMAGE				,
	eMQES_FADE_OUT_EPILOG_IMAGE				,

	eMQES_FADE_IN_EPILOG					,
	eMQES_DISPLAY_EPILOG_TEXT				,
	eMQES_WAIT_TO_EPILOG_CLOSE				,
	eMQES_FADE_OUT_EPILOG_SCREEN			,

};

class	cMainQuestEvent	 : public CGamePlay
{
public:
	int						m_iChapter;
	int						m_iStep;
	int						m_iChapterImageFadeValue;
	int						m_iFadeValueForFlyEagle;

	int						m_iEpilogImageIndex,m_iDisplayEpilogImageTime;

	DWORD					m_bIsHideCursor;
	DWORD					m_dwDisplayChapterCommentValue,m_dwChapterCommentSize,m_dwLastUpdateTimeChapterChar;
	DWORD					m_dwWaitForNextStepTime;
	BOOL					m_bIsImageLoaded;
	CPos					m_posChapterName,m_posChapterNameBegin,m_posChapterNameDest;

	BOOL					open(int _iChapter);
	BOOL					openEpilog(int _iChapter,int _iImage,int _iDisplayImageTime);
	void					close();

	void					draw();
 
	BOOL					update();
	BOOL					updateFadeOutGameScreen();
	BOOL					updateWaitEagle();
	BOOL					updateFadeInEventScreen();
	BOOL					updateFadeInChapterName();
	BOOL					updateDisplayChapterName();
	BOOL					updateFadeOutChapterNameInterface();
	BOOL					updateFadeInChapterInterface();
	BOOL					updateFadeOutChapterImage();
	BOOL					updateDisplayChapterContents();
	BOOL					updateWaitToClose();
	BOOL					updateFadeOutEventScreen();
	BOOL					updateFadeInGameScreen();
	BOOL					updateFinish();

	void					drawFadeInChapterName();
	void					drawDisplayChapterName();
	void					drawFadeOutChapterNameInterface();
	void					drawChapterInterface();
	void					drawFadeInChapterInterface();
	void					drawFadeOutChapterImage();
	void					drawDisplayChapterContents();
	void					drawWaitToClose();
	void					drawFadeOutEventScreen();
	void					drawFadeInGameScreen();
	void					drawFinish();


	BOOL					updateFadeOutGameScreenForEpilog();

	BOOL					updateFadeInEpilogImage();
	BOOL					updateDisplayEpilogImage();
	BOOL					updateFadeOutEpilogImage();

	BOOL					updateFadeInEpilog();
	BOOL					updateDisplayEpilogText();
	BOOL					updateWaitEpilogClose();
	BOOL					updateFadeOutEpilogScreen();

	void					drawEpilog();
	void					drawFadeOutGameScreenForEpilog();

	void					drawFadeInEpilogImage();
	void					drawDisplayEpilogImage();
	void					drawFadeOutEpilogImage();

	void					drawFadeInEpilog();
	void					drawDisplayEpilogText();
	void					drawWaitEpilogClose();
	void					drawFadeOutEpilogScreen();

	BOOL					isOpened();
};

extern	cMainQuestEvent		g_gwMainQuestEvent;

#endif