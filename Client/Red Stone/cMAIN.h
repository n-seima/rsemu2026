#ifndef _classMAIN_H
#define _classMAIN_H

#include "cNEMO.H"
//#include "../VirtualizerSDK.h"

// Link VirtualizerSDK library into our project.
//#pragma comment(lib, "../VirtualizerSDK32.lib")



#ifndef	_DEBUG
	#ifdef	_FOR_KOREA
		#define	_USE_XTRAP	1
	#endif
#endif

#ifdef	_USE_NPGL
#pragma comment(lib, "NPGameLib.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include	"NPGameLib.h"
extern		CNPGameLib	*g_npgl;
extern		BOOL		g_bIsExistGGError;
extern		char		g_strGGError[256];
#endif

class cOPTION
{
public:
	static	BOOL				s_isPutFPS;			//	FPS를찍어 주냐?
	static	BOOL				s_isPutCoordinate;	//	각종 좌표
	static	BOOL				s_isFullScreen;
	static	BOOL				s_isPutMemory;
	static	BOOL				s_isPutProfile;

	static	BOOL				s_bIsShopDebugMenu;
	static	BOOL				s_isPutInfo;
	static	BOOL				s_isPaintBlockedTile;
	static	BOOL				s_isPutActorInfo;		//	캐릭터의 정보
	static	BOOL				s_isPutRangeInfo;		//	범위 정보를 표시한다.
	static	BOOL				s_isExclusiveMode;

	static	int					s_iReservedFps;
};

class cMAIN : public cOPTION
{
public:
	static	cWND				*s_pWnd;
	static	HWND				s_hWnd;
	static	HINSTANCE			s_hInst;
	static	BOOL				s_isRunApplication;
	static	BOOL				s_isActiveApplication;
	static	WORD				s_wPacketSecurityCode;

	static	HCURSOR				s_hCurrentCursor;
	static	HCURSOR				s_hAttackCursor;
	static	HCURSOR				s_hWaitCursor;
	static	HCURSOR				s_hViewCursor;
	static	HCURSOR				s_hDialogCursor;
	static	HCURSOR				s_hGoldCursor;
	static	HCURSOR				s_hMoveFieldCursor;
	static	HCURSOR				s_hNormalCursor;
	static	HCURSOR				s_hOpenBoxCursor;
	static	HCURSOR				s_hPickCursor;
	static	HCURSOR				s_hPlayerCursor;
	static	HCURSOR				s_hPointDialogCursor;
	static	HCURSOR				s_hDisarmCursor;
	static	HCURSOR				s_hEventCursor;
	static	HCURSOR				s_hUnlockCursor;
	static	HCURSOR				s_hOpenDoorCursor;

	static	HCURSOR				s_hWarningCursor;
	static	HANDLE				s_hMutex;

	static	BOOL				ReadConfigScript();			//	config.txt 화일을 읽는다.
	static	BOOL				InitGG();
	static	BOOL				InitHS();
	static	BOOL				Init(HINSTANCE hInst);	//	초기화 윈도우및 기타..
	static	BOOL				InitForChina(HINSTANCE hInst);
	static	BOOL				CheckKeyLogger();

	static	BOOL				Exit(BOOL _bIsNow=FALSE);					//	끝내기..
	static	BOOL				Run();					//	돌아라~~
	static	BOOL				Render();				//	화면 그려줘!!
	static	void				Reset();
	static	LRESULT CALLBACK	WNDProc(HWND,UINT,WPARAM,LPARAM);


	static	void				SetLoginServerIP();
	static	void				SetHost();
	static	BOOL				CheckMutex();
	static	void				CheckTestClient(char *_lpstrCheckCommandLine);
	static	BOOL				OperateDevClientWork();
	static	BOOL				CheckPara(char *_lpstrCommandLine,char *_lpstrCheckCommandLine,DWORD _dwBeginTime);

};

extern	DWORD	g_dwClientVersion;
extern	int		g_iSeasonVariable;

#define dSEASON_VARIABLE_PVP_JOB_FACTOR 									1				//결투장 직업 보정 
#define dSEASON_VARIABLE_PVP_REFORM 										1				//결투장 세부 규칙 변경(운굴림,이속 증가 등)
#define dSEASON_VARIABLE_MINIPET_POWER_UP_2 								1				//미니펫 강화 2차
#define dSEASON_VARIABLE_GUILD_REFORM										3				//길드 개편
#define dSEASON_VARIABLE_MINIPET_FEED_INTERFACE								10				//미니펫 한번에 먹이기 인터페이스
#define dSEASON_VARIABLE_PARTY_UNION										100				//파티 연합

extern	BOOL	g_bIsUseConsole;
extern	BOOL	g_bIsDevPC;
extern	BOOL	g_bIsTerminalPC;
extern	BOOL	g_bIsPortalUser;
extern	BOOL	g_bIsUseNPGL;
extern	DWORD	g_dwLastCheckGameHackTime;
extern	DWORD	g_dwMainThreadID;

extern	int		g_iServiceID;
extern	char	g_strUserID[20];
extern	char	g_strTokken[64];

#endif