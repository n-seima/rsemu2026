/*
*/
#ifndef _classMAIN_H
#define _classMAIN_H

#pragma warning(disable: 4786)

#include "SFC.H"
#include "cWINDOW.H"
#include "cIMAGE.H"
#include "cBARMENU.H"
#include "CBGPlay.H"
#include "cMAPWND.H"
#include <commctrl.h>
#include <deque>
#include "MinimapWindow.h"
#include "URdoBuffer.h"
#include "cMemoWindow.h"
#include "resource.h"
#include "CHelpWindow.h"
#include "CImageData.h"
//	현재 작업중인 모드
#define	dWORK_TILE				0
#define	dWORK_STATIC_OBJECT		1
#define	dWORK_FREE_OBJECT		2
#define	dWORK_BUILDING			3
#define	dWORK_AREA				4
#define	dWORK_CHARACTER			5
#define	dWORK_EDIT				6
#define	dWORK_ROAD				7
#define	dWORK_CANNOTMOVETILE	8

#define dWORK_COPYORCUT			9				//ds 임시

//	툴바의 메뉴
#define	IDM_HOUSELAYER		1000
#define	IDM_OBJECTLAYER		1001
#define	IDM_UNITLAYER		1002
#define	IDM_AREALAYER		1003
#define	IDM_DOOR_LAYER		1004

#define	IDM_CHECKBLOCK		1005
#define	IDM_GRID			1006
#define	IDM_SCROLL			1007
#define	IDM_QUICK_LAUNCH	1008
#define	IDM_DISPLAY_TILE_INDEX	1009
#define WM_MOUSEWHEEL       0x020A

#define	IDM_BLOCK_DRAW_TILE	1010
#define IDM_OBJECTUPDATE	1011
#define IDM_OBJECTFIND		1012

// 10.01.12
#define IDM_MEMOVIEW		1013

// 대화상자 알파값
#define	DLG_ALPHAMIN			70					//	------------------- 추가 
#define	DLG_ALPHAMAX			255					//	------------------- 추가
// 휠.. define..enum

enum {
	eMAP_SCALE_5	,
	eMAP_SCALE_10	,
	eMAP_SCALE_15	,
	eMAP_SCALE_20	,
	eMAP_SCALE_25	,
	eMAP_SCALE_30	,
	eMAP_SCALE_35	,
	eMAP_SCALE_40	,
	eMAP_SCALE_45	,
	eMAP_SCALE_50	,
	eMAP_SCALE_100	,
	eMAP_SCALE_CNT	,
};


// 페이지 : 목록/검색
enum {
	ePAGE_LIST		,
	ePAGE_FIND_LIST	,
};

// 몬스터 정보 페이지
enum {
	ePAGE_MONSTER_INFO	,
	ePAGE_MONSTER_EVENT	,
	ePAGE_MONSTER_JOB	,
	ePAGE_MONSTER_DEATH	,
};

// 대화편집 콘텐츠
enum {
	eTYPE_CONTENTS_DEFAULT	,
	eTYPE_CONTENTS_ADD	,
	eTYPE_CONTENTS_EDIT	,
};

// 페이지 구조체 
struct DLGTEMPLATEEX{
    WORD dlgVer;
    WORD signature;
    DWORD helpID;
    DWORD exStyle;
    DWORD style;
    WORD cDlgItems;
    short x;
    short y;
    short cx;
    short cy;
};

// 투명 스크롤바
#define WS_EX_LAYERED		0x00080000
#define LWA_ALPHA		0x00000002
typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);


class	cCONFIG
{
public:
	char	PrjFolder[512];		//	프로젝트 폴더
	char	MapFolder[512];		//	맵 폴더
	char	TongMapImageFolder[512];
	char	TongMapDataFolder[512];
	char	PrjFileName[128];
	char	MapFileName[128];

			cCONFIG()
			{	strcpy(PrjFolder,"");
				strcpy(MapFolder,"");
				strcpy(TongMapImageFolder,"");
				strcpy(TongMapDataFolder,"");
				strcpy(PrjFileName,"noname.mpd");
				strcpy(MapFileName,"");
			}
};

// class cDevInfo
// {
// private:
// 	DEVMODE devMode;
// public:
// 	inline	POINT	GetPosDisplaySetting()
// 	{
// 		POINT	ptTemp;
// 		ptTemp.x = devMode.dmPelsWidth;
// 		ptTemp.y = devMode.dmPelsHeight;
// 		return ptTemp;
// 	}
// 	void	SetDevInfo()
// 	{
// 		if(EnumDisplaySettings(NULL , ENUM_CURRENT_SETTINGS , &devMode) == 0)
// 			MessageBox(hWND , "Error" , "보조 모니터에서 사용하지 마세요..ㅠ.ㅠ;" , MB_OK);
// 	}
// };

class cMAIN	: public cWND , public cCONFIG 
{
public:
	HMENU				hMENU;
	cTOOLBAR			TOOLBAR;									//	툴바..
	int					WIDTH,HEIGHT;
	char				m_strMapFName[256];
	char				m_strLocalizeFolder[256];
	
	int					m_iMapScale[eMAP_SCALE_CNT];						// 휠..
	URdoBuffer			m_UrdoBuffer[dMAX_MAP];
	
	MinimapWindow*		m_pMinimapWnd;							// 09.07.01  미니맵추가....
	BOOL				m_bBuffer;							// 09.07.15 버퍼..체크.
	BOOL				m_bLButtonDown;							//  09.07.15 L버튼누른상태.
	CHelpWindow*		m_pHelpWindow;	// insu add
						cMAIN();
						~cMAIN();

	BOOL				LoadConfig();
	BOOL				SaveConfig();

	BOOL				Init(HINSTANCE hInst);
	void				InitWnds();
	void				SetWnds();

	BOOL				New();
	BOOL				NewMap();
	BOOL				ResetMap();
	BOOL				SetMap();

	BOOL				Run();
	BOOL				Exit();	
	void				Resize();

	void				ChangeMode(int mode);
	void				Draw();

	void				BeginThread();
	void				EndThread();

	BOOL				SaveMaps();
	void				quickLaunch();

	char				m_strExportMinimapFolder[512];

	void				checkToolbarButton();

	BOOL				exportAllText(HWND hwnd,BOOL _bIsWantSomeMap=FALSE);
	BOOL				exportMonsterInfo(HWND hwnd);
	BOOL				writeDropItemInfo(cFILE *_lpFile);
	BOOL				exportAllMapDropItemInfo(HWND hwnd);
	BOOL				exportNpcInfo(HWND hwnd);
	BOOL				exportCurrentFieldNpcInfo(HWND hwnd);
	BOOL				forceSyncText(HWND hwnd);
	BOOL				forceSyncCommonText(HWND hwnd);
	BOOL				exportItemInfo(HWND hwnd);
	BOOL				exportExpReward(HWND hwnd);
	BOOL				exportKarma(HWND hwnd);
	BOOL				exportShopInfo(HWND hwnd);
	BOOL				exportMiniMapImage(HWND hwnd,int _iImageScale,BOOL _bIsAuto=FALSE);
	BOOL				localizing(HWND hwnd,BOOL _bIsWantPart=FALSE);
	BOOL				exportLoclalizeMapList(HWND hwnd);
	BOOL				selectLocalizeFolder(HWND hwnd);
	BOOL				importGameText();
	BOOL				compareLocalizeText(HWND hwnd,BOOL _bIsWantCheck=FALSE);
	BOOL				modifyText(HWND hwnd);
	BOOL				operateTemplateWork(HWND hwnd);
	BOOL				onAddMap(HINSTANCE _hInst, HWND _hWnd);			// 09.07.02  추가.	
	BOOL				onLoadMap(HINSTANCE _hInst, HWND _hWnd);		// 09.07.02  추가.

	BOOL				uploadMainQuestText(void *_lpQuest,char *_lpstrFileName);
	BOOL				uploadBookText(void *_lpQuest,char *_lpstrFileName);
	BOOL				uploadQuestText(void *_lpQuest,char *_lpstrFileName);
	BOOL				upload1KindText(void *_lpItem,char *_lpstrFileName,BOOL _bIsReadCheckSum=TRUE);
	BOOL				upload2KindText(void *_lpItem,char *_lpstrFileName,BOOL _bIsStreamInOneLine=FALSE);
	BOOL				uploadSkillText(void *_lpSkill,char *_lpstrFileName);
	BOOL				uploadSetItemKindText(void *_lpSetItem,char *_lpstrFileName);
	BOOL				uploadProjectKarmaText(void *_lpPK,char *_lpstrFileName);

	BOOL				uploadItemPrefixText(void *_lpItem,BOOL _bIsSource);
	BOOL				linkAllFieldsGate(HWND hwnd,BOOL _bIsSave);
	BOOL				resaveAllField(HWND hwnd);
	BOOL				compareFields(HWND hwnd);


	void				UpdateKey();

	static	LRESULT CALLBACK	s_procSelectTargetLanguage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	static	UINT APIENTRY		hookSave(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam);

	
	BOOL	operateWM_Command(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void	CreateMiniMap();		// 09.07.01  미니맵보기 추가.

	void	resetBuffer(int _iSelectmap);			// 09.07.15 버퍼리셋.

	void	InitBuffer();							// 09.07.15 버퍼시작.
	void	Undo();									// 09.07.15 되돌리기
	void	Redo();									// 09.07.15 앞으로.
	void	undoBufferUpdate();						// 09.07.15

	void	exportActorKarma();						// 09.07.21
	void	exportFieldKarma();						// 09.07.21
	void	exportMonsterKarma();					// 09.07.21
	void	exportAreaKarma();						// 09.07.21
};

//	기타 전역 변수
extern	cMAIN				MAIN;						//	MAIN창이다.

extern	cRECT_SIZE			rectMAPWND,rectTOOLWND;		//	맵윈도우와 툴윈도우 사이즈
extern	cSPRITE				RESOURCE;					//	툴바 이미지 소스
extern	cSPRITE				g_Sprite;					//	각종 이미지 소스

extern	int					g_iWorkMode;					//	현재 작업중인 모드
extern	int					g_iPreWorkMode;				//	이전에 작업했던 모드

extern	BOOL				g_isCHECK_BLOCK;
extern	BOOL				g_isAutoSave;
extern	BOOL				g_isGRID;
extern	BOOL				g_isLOCKED;
extern	BOOL				g_bIsDisplayTileIndex;

extern	BOOL				g_bIsOnBuildingLayer;
extern	BOOL				g_bIsOnObjectLayer;
extern	BOOL				g_bIsOnAreaLayer;
extern	BOOL				g_bIsOnUnitLayer;
extern	BOOL				g_bIsOnDoorLayer;
extern	BOOL				g_bIsLocalizing;

extern	HCURSOR				g_hAreaCursor;		//	커서들
extern	HCURSOR				g_hEditCursor;
extern	HCURSOR				g_hNormalCursor;

//
//	제대로 된 모드냐?
//
inline	BOOL				IsCorrectMode(int _iMode)
{
	if (g_iWorkMode	!=	_iMode	)
	{
		if (g_iWorkMode		!=	dWORK_EDIT	)	return	FALSE;

		if (g_iPreWorkMode	!=	_iMode		)	return	FALSE;
	}

	return	TRUE;
}

extern	CBGPlay			g_bgPlayer;
extern	HFONT			g_hEditFont;
extern	int		g_iDlgAlpha;			//	대화상자 반투명		----------------- 전역 변수 추가
extern	int				g_iDlgMonsterAlpha;		//	09.11.23 수정 몬스터 편집 대화상자 반투명
#endif