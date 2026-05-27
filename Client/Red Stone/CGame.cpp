#include "CGame.H"
#include "cGAME_INIT.H"
#include "cGAME_OPENING.H"
#include "cGAME_LOGO.H"
#include "cGAME_TITLE.H"
#include "cGAME_OPTION.H"
#include "cGAME_SELECT_AVATAR.H"
#include "cGAME_CREATE_AVATAR.H"
#include "CGamePlay.H"
#include "cGAME_ENDDING.H"
#include "cGAME_CREDIT.H"
#include "cGAME_CLOSE.H"
#include "cGAME_CONNECT_GAME_SERVER.H"
#include "cMESSAGE.H"
#include "CLoadTextCode.h"

#include "cMAIN.H"
#include "cMAP.H"
#include "CActor.H"
#include "CSkill.H"
#include "CImageData.H"
#include "CAgent.H"
#include "CSound.H"
#include "CColorEffect.H"
#include "tdlanguage.h"
#include "CSound.H"
#include "cEvent.H"
#include "cGuild.H"
#include "fireCracker.H"
#include "BattleLog.h"
#include "mess_sign.h"
#include "Clog.h"

CProfiler			g_profiler;

//cCURSOR				CGame::s_cursor;

BOOL				CGame::s_isOperatedMsgbox;
COGGPlayer			CGame::s_oggPlayer;
CSound				CGame::s_soundPlayer;
int					CGame::s_iFadeStep	=	2;
DWORD				CGame::s_dwPasswordAuthenticationCode;

cTIMER				CGame::s_timer;	//	프레임 스킵을 위한 타이머.
int					CGame::s_iLastSecondCount	=	0;
int					CGame::s_iSecondCount		=	0;
char				CGame::s_strHTTP_IP[256];

BOOL				g_bIsObserverModeForDuel	=	FALSE;
BOOL				g_bIsHalfSizeObserver		=	FALSE;

//	게임 메모리 관리
cTALK				CGame::s_inputBox;			//	텍스트 입력
cTALK				CGame::s_inputBox2;			//	텍스트 입력

cMCU				CGame::s_mcuStatic;			//	한번 로딩하고 변경하지 않는 데이터
cMCU				CGame::s_mcuData;			//	오브젝트,건물,맵,기타등등,유동적인 데이터
cMCU				CGame::s_mcuFieldImage;		//	타일/오브젝트/건물(레퍼런스 s_mcuData)
cMCU				CGame::s_mcuBodyImage;		//	NPC,몬스터(레퍼런스 s_mcuData)

BOOL				cGAME_INTERFACE_STATUS::s_isViewInterface	=	TRUE;

BOOL				cGAME_INPDEV_STATE::s_isHeroFocusOn		=	TRUE;		//	주인공에게 포커스 맞추기
BOOL				cGAME_INPDEV_STATE::s_isSmoothFocusing	=	TRUE;		//	부드럽게 포커스 맞추기 on!!

int					cGAME_INPDEV_STATE::s_iFocusActor		=	0xffff;			//	포커스 캐릭터
int					cGAME_INPDEV_STATE::s_iFocusArea		=	0xffff;			//	포커스 영역
int					cGAME_INPDEV_STATE::s_iFocusAreaStack	=	0xffff;
int					cGAME_INPDEV_STATE::s_iFocusItem		=	0xffff;			//	포커스 아이템
int					cGAME_INPDEV_STATE::s_iFocusFieldItem	=	0xffff;			//	포커스 필드 아이템
int					cGAME_INPDEV_STATE::s_iFocusSkill		=	0xffff;			//	포커스 스킬
int					cGAME_INPDEV_STATE::s_iFocusTrap		=	0xffff;			//	포커스 함정
int					cGAME_INPDEV_STATE::s_iFocusEquipment	=	0xffff;			//	포커스 장비
int					cGAME_INPDEV_STATE::s_iFocusBeltSlot	=	0xffff;

int					g_iCountDownForSelfDestruction	=	0;

//	게임 모드와 플레이 모드

int			CGame::s_iMode			=	eGAME_INITIALIZING;		//	현재 게임모드
int			CGame::s_iBookingMode	=	eGAME_INITIALIZING;
int			CGame::s_iPreMode		=	eGAME_INITIALIZING;
int			CGame::s_iDetailMode	=	0;

int			CGame::s_iStep			=	dSTEP_INIT;
int			CGame::s_iBookingStep	=	dSTEP_INIT;

BOOL		CGame::s_isSnapShot		=	FALSE;					//	스크린샷을 잡아라.
c4POINT		CGame::s_posCursor;							//	마우스 커서의 실 좌표(맵 위치랑 다 더한거)
int			CGame::s_iSelectServer	=	0;

char		g_strLaunchFolder[MAX_PATH];	//	레지스트리 말고 클라이언트 실행 폴더
int			g_iWorldServerType;

//	텍스트 출력 관련
CTextDC		CGame::s_text;
CTextDC		CGame::s_textForMsgbox;
CTextDC		CGame::s_text2;
CTextDC		CGame::s_textChat;
CTextDC		CGame::s_sText;
CTextDC		CGame::s_bText;
CTextDC		CGame::s_textBold16;
CTextDC		CGame::s_textBold18;
CTextDC		CGame::s_textLarge50;
CTextDC		CGame::s_textLarge32;
CTextDC		CGame::s_textBold14;
CTextDC		CGame::s_textBoldLarge30;
CTextDC		CGame::s_text14;
CTextDC		CGame::s_textBold13;
CTextDC		CGame::s_text13;
CTextDC		CGame::s_text13M;
CTextDC		CGame::s_textBold13M;
CTextDC		CGame::s_textBold14M;
CTextDC		CGame::s_textBold16M;
CTextDC		CGame::s_textBold24M;
CTextDC		CGame::s_text13MA90;
CTextDC		CGame::s_textBold18M;

//	네트워크 메시지 전달
CAgent		CGame::s_agent;
cBARMENU	CGame::s_bmMenu;
cBARMENU	CGame::s_bmButton;

BOOL		g_bIsTestClient	=	FALSE;
BOOL		CGame::s_bIsFirstLogin = FALSE;		// 처음 로그인했다..
////////////////////////////////////////////////////////////////////////////////////////
//		최초의 초기화 각종 데이터를 로딩한다.
////////////////////////////////////////////////////////////////////////////////////////

//
//	기본적인 게임 초기화 작업(프로그래스바 진행이 필요 없다.)
BOOL
CGame::Init()
{
// 	{
// 		cFILE	file;
// 
// 		if	(file.Open("data/jobData2.dat","rb"))
// 		{
// 			char	strMac[512];
// 			int		iLen	=	file.Length-1;
// 
// 			file.Read(strMac,file.Length);
// 			file.Close();
// 
// 			int		aiTable[32]	=	{21,-78,119,-111,112,27,-96,27,83,-12,9,33};
// 
// 			for (int i=0;i<iLen;i++)
// 				strMac[i]	-=	aiTable[i%12];
// 
// 			s_agent.m_strMacAddress	=	strMac;
// 		}
// 	}

	g_esm.init();

	ResetNetworkEvent();

#ifdef _DEBUG
	g_canvas.init(s_hWnd,g_iScreenWidth,g_iScreenHeight,16,s_isFullScreen,eCD_DIB);//	화면 모드 초기화
	g_BattleLog.Init();
#else
	g_canvas.init(s_hWnd,g_iScreenWidth,g_iScreenHeight,16,s_isFullScreen,eCD_DIB);//g_config.m_iOutputDevice);//	화면 모드 초기화
#endif

	g_colorEffect.init();

	cINPDEV::s_hInpdevWnd	=	s_hWnd;
	cINPDEV::SetMouseActiveArea(0,0,g_iScreenWidth,g_iScreenHeight);

	//	텍스트 초기화
	{
		int	iMinSize	=	11;

#ifdef	_FOR_THAI
		iMinSize	=	13;
#endif

		CText::Init(s_hWnd,g_strFontName);
		
		s_text.selectFont(dWINDOW_FONT_NORMAL);
#ifdef	_FOR_JAPAN
		s_sText.selectFont(dWINDOW_FONT_SMALL);
#else
		s_sText.selectFont(dWINDOW_FONT_NORMAL);
#endif
		s_sText.setShadowMargin(1);
		s_bText.selectFont(dWINDOW_FONT_BOLD);

		s_textLarge32.selectFont(max(iMinSize,32),g_strFontName,FW_BOLD,TRUE);
		s_textBold16.selectFont(max(iMinSize,16),g_strPitchmanShopTextName,FW_BOLD);
		s_textBold14.selectFont(max(iMinSize,14),g_strPitchmanShopTextName,FW_BOLD);
		s_text14.selectFont(max(iMinSize,14),g_strFontName,FW_NORMAL);
		s_textBold13.selectFont(max(iMinSize,13),g_strPitchmanShopTextName,FW_BOLD);
		s_text13.selectFont(max(iMinSize,13),g_strPitchmanShopTextName);

		s_textBoldLarge30.selectFont(max(iMinSize,30),g_strCheckFontName,FW_BOLD);
		s_textBoldLarge30.setShadowMargin(2);
		s_textBold18.selectFont(max(iMinSize,18),g_strCheckFontName,FW_BOLD);
		s_textLarge50.selectFont(max(iMinSize,50),g_strCheckFontName,FW_BOLD);
		s_textLarge50.setShadowMargin(4);
		s_text13M.selectFont(max(iMinSize,13),g_strCheckFontName);
		s_text13MA90.selectFont(max(iMinSize,13),g_strCheckFontName,FW_NORMAL,FALSE,FALSE,900);
		s_textBold13M.selectFont(max(iMinSize,13),g_strCheckFontName,FW_BOLD);
		s_textBold14M.selectFont(max(iMinSize,14),g_strCheckFontName,FW_BOLD);
		s_textBold16M.selectFont(max(iMinSize,16),g_strCheckFontName,FW_BOLD);
		s_textBold18M.selectFont(max(iMinSize,18),g_strCheckFontName,FW_BOLD);
		s_textBold24M.selectFont(max(iMinSize,24),g_strCheckFontName,FW_BOLD);

#ifdef	_FOR_ENG
		s_text2.selectFont(max(iMinSize,13),g_strCheckFontName);
		s_textForMsgbox.selectFont(max(iMinSize,13),g_strCheckFontName);
		s_textChat.selectFont(max(iMinSize,13),g_strCheckFontName);
#else
		s_text2.selectFont(max(iMinSize,12),g_strCheckFontName);
		s_textForMsgbox.selectFont(max(iMinSize,12),g_strCheckFontName);
		s_textChat.selectFont(max(iMinSize,12),g_strCheckFontName);
#endif
	}

	cNOTICE::SetTextDevice(&s_bText);
	s_agent.init(s_hWnd,30);			//	윈속/패킷 초기화

	SetLogFolder();

	for (int i=0;i<dMAX_AVATAR_COUNT;i++)
		s_aAvatarList[i].m_wJob	=	0xffff;

	s_soundPlayer.init(s_hWnd, SOUND_STEREO, SOUND_44K, SOUND_16BIT);
	s_soundPlayer.setDefaultVolume(g_config.m_iSoundVolume*10);

	s_oggPlayer.init(s_hWnd,&s_soundPlayer,44100, 44100/2, 8800, 1);	//	bg플레이어 초기화
	s_oggPlayer.setVolume(g_config.m_iMusicVolume*10);
	s_oggPlayer.setSmoothChange(TRUE);
	s_oggPlayer.setFadeSpeed(200);
	s_oggPlayer.setPlayOption(SOUND_PLAY_REPEAT);

	CGamePlay::PlayBgm("01 Title-Legend of Red Stone.ogg");
	g_esm.setActive(g_config.m_bIsSoundOn);

	return	TRUE;
}


//
//	게임 종료
BOOL
CGame::Close()
{
	CloseSpeedHackChecker();

	g_im.close();
	g_jm.close();
	g_map.close();
	s_agent.close();
	s_bmMenu.close();
	s_oggPlayer.close();
	g_logChat.close();

	g_canvas.destroy();
	CText::Destroy();

#ifdef	dUSE_MCU
	s_mcuStatic	.close();		//	인터페이스
	s_mcuData	.close();		//	오브젝트,건물,맵,기타등등
#endif

	CGamePlay::SaveConfigData();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
//		업데이트
////////////////////////////////////////////////////////////////////////////////////////

//
//	팝업알림 상자 업데이트
BOOL
CGame::UpdateMsgbox()
{
	if	(!g_msgBox.isPoped())
		return	FALSE;

	g_msgBox.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	if	(g_msgBox.isWaitPacket())
	{
		if	(s_agent.isStoredPacket(g_msgBox.getWaitPacket()))
		{
			g_msgBox.close();
			g_msgBox.setResult(dMSGBOX_RECEIVE_WAIT_PACKET);
		}
	}

	switch(g_msgBox.getResult())
	{
		case	eMBI_EXIT_TO_WINDOW_BY_NGPL	:
			s_isRunApplication	=	FALSE;
			break;
	}

	return	TRUE;
}

//
//	업데이트
BOOL
CGame::Update()
{
	g_iWorldX	=	g_map.m_pos.x;
	g_iWorldY	=	g_map.m_pos.y;

	g_eh.resetLog();

	cINPDEV::Update();			//	입력장치 업데이트

	if	(s_aKey[KEY_LALT]	&& s_aTouched[KEY_TAB]	)	//	외출?
	{
		SetForegroundWindow(GetDesktopWindow());
		return TRUE;
	}

	if	(s_aTouched[KEY_F12]	|| s_aTouched[KEY_SNAPSHOT])
	{
		s_isSnapShot	=	TRUE;

		if	(s_iMode	<=	eGAME_CREATE_AVATAR	||	s_iStep	!=	dSTEP_MAIN)
			s_isSnapShot=	FALSE;
	}

	s_iFrameCounter++;
	g_iImageFrameCounter	=	s_iFrameCounter;

	if (s_iFrameCounter	%	dUPDATE_PERIOD	==	0)
		s_isUpdateTimingByUpdatePeriod	=	TRUE;
	else
		s_isUpdateTimingByUpdatePeriod	=	FALSE;

	s_iSecondCount		=	timeGetTime()/1000;

	if	(s_iSecondCount	!=	s_iLastSecondCount)
	{
		s_bIs1Second	=	TRUE;
		s_iFrameCounterFor1SecondWork	=	0;

		if	(g_iBoostGoldSecondByTreasureMapEventInServer)
			g_iBoostGoldSecondByTreasureMapEventInServer--;
		if	(g_iBoostExpSecondByTreasureMapEventInServer)
			g_iBoostExpSecondByTreasureMapEventInServer--;
	}
	else
	{
		s_iFrameCounterFor1SecondWork	++;
		s_bIs1Second					=	FALSE;
	}

	s_iLastSecondCount	=	s_iSecondCount;
	s_agent.sendLifeCode();	//	라이프 코드 전송

	cTIMER::UpdateFPS();						//	FPS 업데이트
	g_canvas.updateEffect();
	g_lng.update( (s_isLeftButtonPressed ) ? true : false , (s_isRightButtonPressed) ? true : false );

	if	(s_wSpeedHackUserCountdown)
	{
		s_wSpeedHackUserCountdown--;

		if (s_wSpeedHackUserCountdown	==	0)
		{
			ERRMSG(dMSG_ERROR,dMSG_FOUND_SPEED_HACK);

			return	FALSE;
		}
	}

#ifdef	_USE_NPGL
	if	(g_dwLastCheckGameHackTime)
	{
		if	(g_dwLastCheckGameHackTime+3*1000	<	timeGetTime())
			s_isRunApplication	=	FALSE;
	}
	else
	if	(g_bIsUseNPGL	&&	g_bIsExistGGError)
	{
		g_dwLastCheckGameHackTime	=	timeGetTime();
		g_msgBox.cPopup("",g_strGGError,dMSG_OK);
		g_msgBox.setButtonId(eMBI_EXIT_TO_WINDOW_BY_NGPL);
	}
#endif

//	메시지 박스 처리
	{
		s_isOperatedMsgbox	=	UpdateMsgbox();

		if (!g_msgBox.isExclusive())	UpdateBookedWork();	//	메시지 박스가 떠 있으면 작업 진행이 안된다.

		if (s_isOperatedMsgbox)
		{
			if (g_canvas.getFadeScreenStatus()	==	dFADE_OUT)
			{
				g_canvas.fadeIn();
			}
		}

		if (g_msgBox.isFreeze())	//	메시지 박스 고정 상태
		{
			Draw();

			return	TRUE;
		}
	}	//	메시지 박스 처리

	s_ttCommon.update(s_posMouse.x,s_posMouse.y);	//	툴팁 처리

	switch(s_iMode)
	{
		case	eGAME_INITIALIZING			:	if (!cGAME_INIT::Run())					return	FALSE;break;//	초기화
		case	eGAME_OPENING				:	if (!cGAME_OPENING::Run())				return	FALSE;break;//	오프닝
		case	eGAME_LOGO					:	if (!cGAME_LOGO::Run())					return	FALSE;break;//	오프닝
		case	eGAME_TITLE					:	if (!cGAME_TITLE::Run())				return	FALSE;break;//	타이틀 화면
		case	eGAME_OPTION				:	if (!cGAME_OPTION::Run())				return	FALSE;break;//	옵션
		case	eGAME_SELECT_AVATAR			:	if (!cGAME_SELECT_AVATAR::Run())		return	FALSE;break;//	아바타 선택
		case	eGAME_CREATE_AVATAR			:	if (!cGAME_CREATE_AVATAR::Run())		return	FALSE;break;//	아바타 생성
		case	eGAME_CONNECT_GAME_SERVER	:	if (!cGAME_CONNECT_GAME_SERVER::Run())	return	FALSE;break;//	아바타 생성
		case	eGAME_PLAY					:	if (!CGamePlay::Run())					return	FALSE;break;//	멀티 플레이
		case	eGAME_ENDDING				:	if (!cGAME_ENDDING::Run())				return	FALSE;break;//	엔딩
		case	eGAME_CREDIT				:	if (!cGAME_CREDIT::Run())				return	FALSE;break;//	크레딧
		case	eGAME_CLOSE					:	if (!cGAME_CLOSE::Run())				return	FALSE;break;//	게임 끝내기
	}

	if	(s_timer.isSkipFrame()			)
		return	TRUE;	//	프레임 스킵이면... return~~

	BOOL	bIsDrawResult	=	Draw();

	return	bIsDrawResult;
}

////////////////////////////////////////////////////////////////////////////////////////
//		화면 업데이트
////////////////////////////////////////////////////////////////////////////////////////
//
//	그려주기
BOOL
CGame::Draw()
{
	if	(g_bIsTerminalPC)
	{
		static	int	iCounter	=	0;

		iCounter++;

		if	(iCounter % 8 != 0)
			return	TRUE;
	}
	else
	{
#ifndef	_DEBUG
		if	(!s_isActiveApplication)
			return	TRUE;
#endif
	}

	if	(!g_canvas.active())
		return	TRUE;	//	캔버스 액티브~~

	if	(!g_msgBox.isFreeze())
	{
		switch(s_iMode)
		{
			case	eGAME_INITIALIZING			:	cGAME_INIT::Draw();break;
			case	eGAME_OPENING				:	cGAME_OPENING::Draw();break;
			case	eGAME_LOGO					:	cGAME_LOGO::Draw();break;
			case	eGAME_TITLE					:	cGAME_TITLE::Draw();break;
			case	eGAME_OPTION				:	cGAME_OPTION::Draw();break;
			case	eGAME_SELECT_AVATAR			:	cGAME_SELECT_AVATAR::Draw();break;
			case	eGAME_CREATE_AVATAR			:	cGAME_CREATE_AVATAR::Draw();break;
			case	eGAME_CONNECT_GAME_SERVER	:	cGAME_CONNECT_GAME_SERVER::Draw();break;
			case	eGAME_PLAY					:	CGamePlay::Draw();break;
			case	eGAME_ENDDING				:	cGAME_ENDDING::Draw();break;
			case	eGAME_CREDIT				:	cGAME_CREDIT::Draw();break;
			case	eGAME_CLOSE					:	cGAME_CLOSE::Draw();break;
		}
	}

	if	(s_pmCommon.isPopuped()	==	FALSE)
		s_ttCommon.put();

	CGamePlay::DrawChattingBar();
	g_msgBox.draw();	//	팝업 박스를 그려준다.
	CGamePlay::s_UpgradeFieldTitle.draw();		// 09.09.02  강화 성공실패 타이틀보여주기 ~~

//	스크린샷
	if	(s_isSnapShot)
	{
		s_isSnapShot=	FALSE;

		int		count	=	0;

		char	strFileName[64],strFullPath[512];

		CreateDirectory(dMSG_SCREEN_SHOT,NULL);
		SetCurrentDirectory(dMSG_SCREEN_SHOT);

		CTimeInfo	currentTime;

		currentTime.update();

		while(count < 1000)
		{
			sprintf(strFileName,"RedStone %.2d.%.2d.%.2d[%.2d].bmp",currentTime.m_wYear%100,currentTime.m_wMonth,currentTime.m_wDay,count++);

			if (cBMP::Save(strFileName,cDRAW::p2SCREEN,g_iScreenWidth,g_iScreenHeight,FALSE))
				break;
		}

		SetLogFolder();

//		s_mcuData.tempFree();

		if (s_iMode	==	eGAME_PLAY)
		{
			sprintf(strFullPath,"<c:WHITE>%s/<n>%s",dMSG_SCREEN_SHOT,strFileName);
			CGamePlay::AddSystemMessage(LTGREEN,strFullPath);
		}
	}	//	스크린샷
/*
	{
		int		iTooSlowIndex		=	0;
		int		iTooSlowTime		=	0;
		char	strOutput[1024];

		for (int i=0;i<g_profiler.m_iNumbers;i++)
		{
			sprintf(strOutput,"%30s : Total %s MS,Hit Count %8d,This Time %s,Average %s MS,%s%%",
							g_profiler.getName(i),g_profiler.getTotalTime(i),g_profiler.getHitCount(i),
							g_profiler.getTime(i),g_profiler.getAverageTime(i),g_profiler.getPercent(i));

			if (iTooSlowTime < g_profiler.m_aProfiles[i].m_dwTime)
			{
				iTooSlowTime	= g_profiler.m_aProfiles[i].m_dwTime;
				iTooSlowIndex	=	i;
			}

			s_text.put(10,10+i*20,WHITE,strOutput);
		}

		sprintf(strOutput,"%30s : Total %s MS,Hit Count %8d,This Time %s,Average %s MS,%s%%",
						g_profiler.getName(iTooSlowIndex),g_profiler.getTotalTime(iTooSlowIndex),g_profiler.getHitCount(iTooSlowIndex),
						g_profiler.getTime(iTooSlowIndex),g_profiler.getAverageTime(iTooSlowIndex),g_profiler.getPercent(iTooSlowIndex));

		s_text.put(10,10+i*20,WHITE,strOutput);
	}
*/
//	s_cursor.put(s_posMouse.x,s_posMouse.y);

//	s_text.put(10,10,WHITE,"FPS %u",s_timer.s_iFps);

	g_canvas.draw();

	return TRUE;
}	//	CGame::Draw()

//
//	어플리케이션을 비활성화 하려 한다.
void
CGame::TryDisactiveApp()
{
}

char	*l_strHeroAnmFileName[]	=
{

	"Knight.sad"				,	"",
	"Warrior.sad"				,	"",
	"Wizard.sad"				,	"",
	"Werewolf.sad"				,	"",
	"Priest.sad"				,	"",
	"FallenAngel.sad"			,	"",
	"Rogue.sad"					,	"",
	"Fighter.sad"				,	"",
	"MagicLancer.sad"			,	"",
	"MagicArcher.sad"			,	"",
	"BeastTamer.sad"			,	"",
	"summoner.sad"				,	"",
	"Princess.sad"				,	"",
	"MagicalGirl.sad"			,	"",
	"NecroMancer.sad"			,	"",
	"Devil.sad"					,	"",
	"SoulBringer.sad"				,	"",
	"Champion.sad"				,	"",
	"Opticalist.sad"				,	"",
	"BeastMan.sad"				,	"",
};



cANM	CGame::s_anmHero;
cANM	CGame::s_anmHero2;
int		CGame::s_iLoadedHeroImageJob	=	-1;

//
//아바타 정보를 그려준다.
void
CGame::DrawHeroPicture(int _iX,int _iY,int _iJob)
{
	if	(g_bIsTerminalPC || _iJob == -1)
		return;

	if	(s_iLoadedHeroImageJob	!=	-2)
	{
		if	(s_iLoadedHeroImageJob	!=	_iJob)
		{
			if	(!s_anmHero.Load(_ms("data/interface/heros/%s",l_strHeroAnmFileName[_iJob*2]),TRUE))
				s_iLoadedHeroImageJob	=	-2;
			if	(l_strHeroAnmFileName[_iJob*2+1][0])
			{
				if	(!s_anmHero2.Load(_ms("data/interface/heros/%s",l_strHeroAnmFileName[_iJob*2+1]),TRUE))
					s_iLoadedHeroImageJob	=	-2;
			}

			if	(s_iLoadedHeroImageJob	!=	-2)
				s_iLoadedHeroImageJob	=	_iJob;

			s_timer.resetFrameSkip();
		}

		if	(s_iLoadedHeroImageJob	!=	-2)
		{
			int	iFPS		=	s_anmHero.GetFPS(0);
			int	iFrameCount	=	s_anmHero.GetFrameCount(0);
			int	iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;

			if	(l_strHeroAnmFileName[_iJob*2+1][0])
				_iX	=	230+g_iCorrectBI_X/2,_iY	=	300+g_iCorrectBI_Y/2;
			else
				_iX	=	230+g_iCorrectBI_X/2,_iY	=	470+g_iCorrectBI_Y/2;

			s_anmHero.Put(_iX,_iY,0,0,iFrame);

			if	(l_strHeroAnmFileName[_iJob*2+1][0])
				s_anmHero2.Put(_iX,_iY,0,0,iFrame);
		}
	}
}

void
CGame::UpdateBookedWork()					//	예약된 작업 업데이트
{	// 마지막 수정일 : 09.10.19
	if	(s_iStep	==	dSTEP_CLOSE)	//	step이 닫기 일때만..
	{
		s_iMode			=	s_iBookingMode;
		s_iBookingStep	=	dSTEP_INIT;
		s_iStep			=	dSTEP_INIT;
		g_map.m_bIsDrawWeather = TRUE;
	}

	s_iStep		=	s_iBookingStep;	//	예약된 스텝으로 진행
}
void
CGame::BookingMode(int _iMode,int _iStep,int _iFade)	//	게임 모드 예약
{
	if	(_iStep		!=	0xffff)
		BookingStep(_iStep);

	s_iBookingMode	=	_iMode;
	s_iPreMode		=	s_iMode;

	if	(_iFade	==	dFADE_OUT)
		g_canvas.fadeOut(0,s_iFadeStep);
	else
	if	(_iFade	==	dFADE_IN)
		g_canvas.fadeIn(0,s_iFadeStep);
	else
	{
		if	(g_canvas.getFadeScreenStatus()	==	dFADE_IN)
			if	(g_canvas.isCompleteFadeIn())
				g_canvas.cancelFadeScreen();

		if	(g_canvas.getFadeScreenStatus()	==	dFADE_OUT)
			if	(g_canvas.isCompleteFadeOut())
				g_canvas.cancelFadeScreen();
	}
}

void
CGame::BookingStep(int _iStep,int _iFade)
{
	s_iBookingStep	=	_iStep;

	if	(_iFade	==	dFADE_OUT)
		g_canvas.fadeOut(0,s_iFadeStep);
	else
	if	(_iFade	==	dFADE_IN)
		g_canvas.fadeIn(0,s_iFadeStep);
	else
	{
		if	(g_canvas.getFadeScreenStatus()	==	dFADE_IN)
			if	(g_canvas.isCompleteFadeIn())
				g_canvas.cancelFadeScreen();

		if	(g_canvas.getFadeScreenStatus()	==	dFADE_OUT)
			if	(g_canvas.isCompleteFadeOut())
				g_canvas.cancelFadeScreen();
	}
}

void
CGame::SetStep(int _iStep)
{
	s_iStep	=	_iStep;
}

char*
CGame::GetWorldName(int _iWorldIndex)
{
	for (int i=0;i<dMAX_SERVER_COUNT;i++)
	{
		if	(STRICMP(s_aServerList[i].m_strName,"")	==	0	)
			continue;

		if	(s_aServerList[i].m_wServerPlaceInList	==	_iWorldIndex)
			return	s_aServerList[i].m_strName;
	}

	return	NULL;
}
