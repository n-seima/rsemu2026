#include "cMAIN.H"
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "cTEXT.H"
#include "cDRAW.H"

#include "cTOOLWND.H"
#include "cOBJECT.H"
#include "cBUILDING.H"
#include "cAREA.H"
#include "cSET.H"
#include "cMESSAGE.H"
#include "cKARMA.H"
#include "cPROJECT.H"
#include "cACTOR.H"
#include "cSKILL.H"
#include "CSetShop.H"
#include "CSetDialogList.H"
#include "CSetCustomItem.H"
#include "CTitle.H"
#include "setArca.H"
#include "trapSkill.H"
#include "setSetItem.h"
#include "setPieceItem.h"
#include "setSecretDungeon.h"
#include "setBook.h"
#include "setMainQuest.h"
#include "command.h"
#include "set_character_pattern.h"
#include "svn.h"
#include "cSetDialogNpcData.h"				//		------------------------- 추가
#include "CSetDialogMonster.h"					//	11.23 추가
#include "cCompareBox.h"

#include <winuser.h>

CBGPlay			g_bgPlayer;

LPCTSTR			lpszAppName		= "MANGCHI";
LPCTSTR			lpszTitle		= "Mangchi II for Red Stone (The Lunatic Story)";

HFONT			g_hEditFont;

cMAIN			MAIN;
cRECT_SIZE		rectMAPWND,rectTOOLWND;
int				g_iWorkMode		=	dWORK_EDIT;				//	현재 작업중인 모드	//임시
int				g_iPreWorkMode	=	dWORK_TILE;				//	이전에 작업했던 모드
BOOL			g_bIsLocalizing	=	FALSE;

int				g_iDlgAlpha	=	DLG_ALPHAMAX;			//	NPC 편집 대화상자 반투명도	전역변수 추가
int				g_iDlgMonsterAlpha	=	DLG_ALPHAMAX;			//	09.11.23 추가 몬스터 편집 대화상자 반투명도
cSPRITE			RESOURCE,g_Sprite;

HANDLE			ThreadHandle; // 스레드 핸들
DWORD			ThreadAddr;          // 스레드 주소

HCURSOR			g_hAreaCursor;		//	커서들
HCURSOR			g_hEditCursor;
HCURSOR			g_hNormalCursor;

////////////////////////////////////////////////////////////////////////////////
//								각종 옵션
////////////////////////////////////////////////////////////////////////////////
BOOL			g_isCHECK_BLOCK		=	FALSE;
BOOL			g_isAutoSave		=	FALSE;
BOOL			g_isGRID			=	FALSE;
BOOL			g_bIsOnBuildingLayer=	TRUE;
BOOL			g_bIsOnObjectLayer	=	TRUE;
BOOL			g_bIsOnAreaLayer	=	TRUE;
BOOL			g_bIsOnUnitLayer	=	TRUE;
BOOL			g_bIsOnDoorLayer	=	TRUE;
BOOL			g_isLOCKED			=	FALSE;
BOOL			g_bIsDisplayTileIndex	=	FALSE;
char			g_strSVNPath[1024];

////////////////////////////////////////////////////////////////////////////////
//									툴바
////////////////////////////////////////////////////////////////////////////////


TBBUTTON tbButtons[] = 
{
	{ 0, IDM_NEW_MAP	, TBSTATE_ENABLED, TBSTYLE_BUTTON				, 0, 0L, 0}, 
	{12, IDM_RESET_MAP	, TBSTATE_ENABLED, TBSTYLE_BUTTON				, 0, 0L, 0}, 
	{ 0,		0		, TBSTATE_ENABLED, TBSTYLE_SEP					, 0, 0L, 0},
	{ 1, IDM_LOAD_MAP	, TBSTATE_ENABLED, TBSTYLE_BUTTON				, 0, 0L, 0}, 
	{11, IDM_ADD_MAP	, TBSTATE_ENABLED, TBSTYLE_BUTTON				, 0, 0L, 0}, 
	{ 2, IDM_SAVE_MAP	, TBSTATE_ENABLED, TBSTYLE_BUTTON				, 0, 0L, 0},
	{10, IDM_SAVE_MAPS	, TBSTATE_ENABLED, TBSTYLE_BUTTON				, 0, 0L, 0},
	{ 0,		0		, TBSTATE_ENABLED, TBSTYLE_SEP					, 0, 0L, 0},
	{ 4, IDM_HOUSELAYER	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK	, 0, 0L, 0},
	{ 5, IDM_OBJECTLAYER, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK	, 0, 0L, 0},
	{ 6, IDM_UNITLAYER	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK	, 0, 0L, 0},
	{ 8, IDM_AREALAYER	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK	, 0, 0L, 0},
	{16, IDM_DOOR_LAYER	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK	, 0, 0L, 0},
	{ 0,		0		, TBSTATE_ENABLED, TBSTYLE_SEP					, 0, 0L, 0},
	{ 7, IDM_CHECKBLOCK	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK	, 0, 0L, 0},
	{ 9, IDM_GRID		, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK	, 0, 0L, 0},
	{13, IDM_SCROLL		, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK	, 0, 0L, 0},
	{15, IDM_DISPLAY_TILE_INDEX	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK	, 0, 0L, 0},
	{17, IDM_BLOCK_DRAW_TILE	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK	, 0, 0L, 0},
	{ 0,		0		, TBSTATE_ENABLED, TBSTYLE_SEP					, 0, 0L, 0},
	{ 3, IDM_QUICK_LAUNCH, TBSTATE_ENABLED,TBSTYLE_BUTTON				, 0, 0L, 0},
	{ 0,		0		, TBSTATE_ENABLED, TBSTYLE_SEP					, 0, 0L, 0},
	{18, IDM_SET_CANNOT_MOVE_TILE, TBSTATE_ENABLED, TBSTYLE_BUTTON	, 0, 0L, 0},
	{ 0,		0		, TBSTATE_ENABLED, TBSTYLE_SEP					, 0, 0L, 0},
	{19, IDM_STATE_AUTO_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK	, 0, 0L, 0},
	//	{10, IDM_HORIZON	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK	, 0, 0L, 0},
	{20 ,IDM_MEMOVIEW	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK, 0, 0L, 0},
	{200,		0		, TBSTATE_ENABLED, TBSTYLE_SEP					, 0, 0L, 0},
	{170,		0		, TBSTATE_ENABLED, TBSTYLE_SEP					, 0, 0L, 0},
	{21 ,IDM_OBJECTUPDATE,TBSTATE_ENABLED, TBSTYLE_BUTTON				, 0, 0L, 0},
};

static	char *strToolbar[]	=
  {
"Add a new map", "Initialize the map you are currently working on",
   "",
   "Load", "Add map", "Save", "Save all maps you are working on",
   "",
   "Building layer", "Object layer", "Unit layer", "Area and other information", "Moon layer",
   "",
   "Show impassable areas", "Show grid", "Scroll screen with mouse", "Show tile index", "Prohibit drawing",
   "",
   "Run Map",
   "",
   "Set unmovable tiles",
   "",
   "Temporary file save function",
   "memo",
   "",
   "",
   "Object Search",
   "end"
  };

// 모니터 정보를 구하기위한...
#define SM_CXMAXIMIZED          61
#define SM_CYMAXIMIZED          62
#define SM_XVIRTUALSCREEN       76
#define SM_YVIRTUALSCREEN       77
#define SM_CXVIRTUALSCREEN      78
#define SM_CYVIRTUALSCREEN      79
#define SM_CMONITORS            80
#define SM_SAMEDISPLAYFORMAT    81



////////////////////////////////////////////////////////////////////////////////
//									초기화 클래스..
////////////////////////////////////////////////////////////////////////////////

cMAIN::cMAIN()
		: m_pMinimapWnd(NULL)								// 09.07.01    미니맵보기 업데이트.  미니맵 포인터 초기화.
		,m_pHelpWindow(NULL)	// insu add - HelpWindow 초기화
{
	strcpy(m_strMapFName,"");
	strcpy(m_strExportMinimapFolder,"");
	strcpy(m_strLocalizeFolder,"");
	
	g_clipConversationContainer.reset();
	g_clipConversation.init(256,TRUE);
	
	//휠.
	m_iMapScale[eMAP_SCALE_5]	= 5;
	m_iMapScale[eMAP_SCALE_10]	= 10;
	m_iMapScale[eMAP_SCALE_15]	= 15;
	m_iMapScale[eMAP_SCALE_20]	= 20;
	m_iMapScale[eMAP_SCALE_25]	= 25;
	m_iMapScale[eMAP_SCALE_30]	= 30;
	m_iMapScale[eMAP_SCALE_35]	= 35;
	m_iMapScale[eMAP_SCALE_40]	= 40;
	m_iMapScale[eMAP_SCALE_45]	= 45;
	m_iMapScale[eMAP_SCALE_50]	= 50;
	m_iMapScale[eMAP_SCALE_100] = 100;
	m_bBuffer			= FALSE;
	m_bLButtonDown			= FALSE;
}

cMAIN::~cMAIN()
{
	if(m_pMinimapWnd) delete m_pMinimapWnd;			// 09.07.01    미니맵보기 업데이트.  미니맵 포인터 제거.
	if(m_pHelpWindow) delete m_pHelpWindow;		// insu add - HelpWindow 제거
	
	for(int i=0;i<dMAX_MAP;++i){					// 09.07.15  타일버퍼제거.
		
		m_UrdoBuffer[i].deleteBuffer();
		
	}
	
}



void	cMAIN::CreateMiniMap(){						// 09.07.01    미니맵보기 업데이트.  미니맵창생성.
	
	if(m_pMinimapWnd){
		DestroyWindow(m_pMinimapWnd->hWND);
		delete m_pMinimapWnd;
		m_pMinimapWnd = NULL;	
	}
	
	m_pMinimapWnd = new MinimapWindow;
	
	
	int iMinimapX	= 0;
	int iMinimapY	= 0;
	int iMonitorCnt = GetSystemMetrics(SM_CMONITORS); 
	int iMonitorXS	= GetSystemMetrics(SM_CXMAXIMIZED);
	int iMonitorYS	= GetSystemMetrics(SM_CYMAXIMIZED);
	int iMonitorCXS	= GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int iMonitorCYS	= GetSystemMetrics(SM_CYVIRTUALSCREEN);
	
	if(iMonitorCnt == 2){
		iMinimapX = iMonitorXS-8;
		iMinimapY = 20;
	}else if(iMonitorCnt == 1){
		iMinimapX = iMonitorXS/2;
		iMinimapY = iMonitorYS/3;
	}
	
	if	(!g_lpActmap)
		g_lpActmap	=	&g_aMap[g_iSelectMap];
	
	
	int iScreenWidth	= 1024;
	int iScreenHeight	= 0;
	
	int iWidth	=	g_lpActmap->m_iWidth;
	
	int iHeight =	g_lpActmap->m_iHeight;
	
	int iMinimapScale = ((iScreenWidth * 1.0 / iWidth) / dTILE_WIDTH) * 100;
	
	iScreenWidth	= iWidth  * (dTILE_WIDTH *1.0 * iMinimapScale/100);
	iScreenHeight	= iHeight * (dTILE_HEIGHT *1.0 * iMinimapScale/100) + 25;
	
	m_pMinimapWnd->Init(hINST, hWND,&g_MAPWND,iMinimapX,iMinimapY , iScreenWidth, iScreenHeight, iMinimapScale);
	
}

//
//	설정 로딩
//
BOOL
cMAIN::LoadConfig()
{	// 마지막 수정일 : 09.10.30	
	cFILE file;

	//	MSGOUT("cJOB - %d",sizeof(cJOB));
	//	return	FALSE;
	MSGOUT("L1");

	cDECODER	decode;

	if	(decode.Upload("mangchi.txt",NULL))
	{
		char	*lpstrFontName	=	decode.GetStream();
		int		iSize			=	decode.GetNumber();

		g_hEditFont	=	CreateFont(iSize,0,0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN, lpstrFontName);
	}
	else
		g_hEditFont	=	NULL;

	if	(!file.Open("mangchi.cfg","rb"))	
		return ERRMSG("The mangchi.cfg file does not exist!!");

	file.Read(TongMapImageFolder,512);
	file.Read(TongMapDataFolder,512);

	MSGOUT("L2");
	file.Read(cPROJECT::s_strName,sizeof(cPROJECT::s_strName));
	file.Read(cPROJECT::s_strMapFolder,sizeof(cPROJECT::s_strMapFolder));
	file.Read(&cSET_BASIC_ITEM::s_isSort,sizeof(cSET_BASIC_ITEM::s_isSort));
	file.Read(&cSET_BASIC_ITEM::s_iSelectKind,sizeof(cSET_BASIC_ITEM::s_iSelectKind));
	file.Read(&cSET_BASIC_ITEM::s_iSelectItem,sizeof(cSET_BASIC_ITEM::s_iSelectItem));
	file.Read(m_strExportMinimapFolder,sizeof(m_strExportMinimapFolder));
	file.Read(&g_isAutoSave,sizeof(g_isAutoSave));
	file.Read(&g_iDlgAlpha,sizeof(g_iDlgAlpha));				//		------------------------------ 추가
	file.Read(&g_iDlgMonsterAlpha,sizeof(g_iDlgMonsterAlpha));		// 09.11.23 추가 몬스터 편집 대화상자 투명도

	file.Read(&cSET_FONT::s_iColorOptionCount,sizeof(cSET_FONT::s_iColorOptionCount));

	if(cSET_FONT::s_iColorOptionCount != 0)
	{
		for(int i =0; i<cSET_FONT::s_iColorOptionCount; i++)
			file.Read(&cSET_FONT::s_ColorOption[i],sizeof(cSET_FONT::s_ColorOption[i]));
	}
	
	cSET_FONT::SaveFontData(FALSE);
		

	MSGOUT("L3");
	if	(cSET_BASIC_ITEM::s_iSelectKind	>=	dITEM_KIND_COUNT	||	cSET_BASIC_ITEM::s_iSelectKind	<	0)	
		cSET_BASIC_ITEM::s_iSelectKind	=	dITEM_KIND_ALL;

	file.Close();

	MSGOUT("L4");
	if	(!cPROJECT::LoadProject())
		return ERRMSG("Project loading error!!");

	MSGOUT("L5");
	return TRUE;
}	//	cMAIN::LoadConfig()

BOOL
cMAIN::SaveConfig()
{		// 마지막 수정일 : 09.10.30
	SetLogFolder();
	
	cFILE file;
	
	if (!file.Open("mangchi.cfg","wb"))
		return ERRMSG("Error!! can't write \"mangchi.cfg\" file");
	
	file.Write(TongMapImageFolder,512);
	file.Write(TongMapDataFolder,512);
	file.Write(cPROJECT::s_strName,sizeof(cPROJECT::s_strName));
	file.Write(cPROJECT::s_strMapFolder,sizeof(cPROJECT::s_strMapFolder));
	file.Write(&cSET_BASIC_ITEM::s_isSort,sizeof(cSET_BASIC_ITEM::s_isSort));
	file.Write(&cSET_BASIC_ITEM::s_iSelectKind,sizeof(cSET_BASIC_ITEM::s_iSelectKind));
	file.Write(&cSET_BASIC_ITEM::s_iSelectItem,sizeof(cSET_BASIC_ITEM::s_iSelectItem));
	file.Write(m_strExportMinimapFolder,sizeof(m_strExportMinimapFolder));
	file.Write(&g_isAutoSave,sizeof(g_isAutoSave));
	file.Write(&g_iDlgAlpha,sizeof(g_iDlgAlpha));				//		------------------------- 추가 대화상자 투명값	
	file.Write(&g_iDlgMonsterAlpha,sizeof(g_iDlgMonsterAlpha));	//		09.11.23 추가 몬스터 편집 대화상자 투명도
	
	file.Write(&cSET_FONT::s_iColorOptionCount,sizeof(cSET_FONT::s_iColorOptionCount));

	int i =0;
	for(i =0; i<cSET_FONT::s_iColorOptionCount; i++)
		file.Write(&cSET_FONT::s_ColorOption[i],sizeof(cSET_FONT::s_ColorOption[i]));
		
	file.Close();
	
	SetCurrentDirectory(PrjFolder);
	cPROJECT::SaveProject();
	
	return TRUE;
}

void
cMAIN::BeginThread()
{
}

void
cMAIN::EndThread()
{
}


BOOL
cMAIN::Exit()
{
	int		i;
	BOOL	isVirgin	=	TRUE;
	
	for (i=0;i<dMAX_MAP;i++)
	{
		if	(!g_aMap[i].IsVirgin())
		{
			isVirgin		=	FALSE;
			break;
		}
	}

	for(int iMap=0;iMap<dMAX_MAP;++iMap){		// 09.07.29 UndoRedo
		m_UrdoBuffer[iMap].deleteBuffer();
	}

	g_MAPWND.resetCopyOrCut();					// 09.07.29 복사붙여넣기
	
	if (!isVirgin)
	{
		switch(cMSG::YESNOCANCEL(dMSG_EXIT_MANGCHI,dMSG_DO_YOU_WANT_A_SAVE))
		{
		case	IDYES		:
			{
				for (i=0;i<dMAX_MAP;i++)
				{
					if	(!g_aMap[i].IsVirgin())
					{
						cMAP::Select(i);
						Draw();
						if	(!cPROJECT::SaveMap(g_aMap[i].m_strFName,i))
							return FALSE;
					}
				}
				break;
			}
			
		case	IDNO		:
			break;
			
		case	IDCANCEL	:
			return FALSE;
			
		}
	}
	
	
	EndThread();
	SaveConfig();
	
	cPROJECT::s_kcMonsterDeath.reset();
	cPROJECT::s_kcMonsterGen.reset();

	return TRUE;
}

void
cMAIN::checkToolbarButton()
{
	if (g_lpActmap->m_bf1IsLocked)
		TOOLBAR.Check(IDM_BLOCK_DRAW_TILE	,TRUE);
	else
		TOOLBAR.Check(IDM_BLOCK_DRAW_TILE	,FALSE);
}

//	BYTE	aBuffer1[1024*1024];
//	BYTE	aBuffer2[1024*1024];
/*
#define THIS_FILE          __FILE__
#define ASSERT(f) \
do \
{ \
if (!(f) && AfxAssertFailedLine(THIS_FILE, __LINE__)) \
AfxDebugBreak(); \
} while (0) \

  #define VERIFY(f)          ASSERT(f)
  #define ASSERT_VALID(pOb)  (::AfxAssertValidObject(pOb, __FILE__, __LINE__))
*/

template <class A> 
inline A* SSafeAlloc(int _nCount)
{
	a = new A [_nCount];
	//  c = a;
	//    /a = b;
	//  b = c;
}

void
__SVN(char *_lpstrParameter)
{
	STARTUPINFOA		si;
	PROCESS_INFORMATION pi;
	
	ZeroMemory( &si, sizeof( STARTUPINFO)); 
	ZeroMemory( &pi, sizeof( PROCESS_INFORMATION)); 
	
	si.cb			=	sizeof( STARTUPINFO); 
	si.dwFlags		=	STARTF_USESHOWWINDOW; 
	si.wShowWindow	=	SW_SHOWNORMAL;
	
	char	strPara[1024];
	
	sprintf(strPara," %s",_lpstrParameter);
	
	CreateProcess(g_strSVNPath,strPara,NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi); 
	
	if	(pi.hProcess) 
	{
		DWORD	dwExitCode	=	STILL_ACTIVE; 
		
		while(dwExitCode	==	STILL_ACTIVE) 
		{
			WaitForSingleObject( pi.hProcess, 1); 
			GetExitCodeProcess( pi.hProcess, &dwExitCode);
			Sleep(1);
		}
	}
}
//
//	윈도우들 초기화
BOOL
cMAIN::Init(HINSTANCE hInst)
{
	InitCommonControls();
	
	//	g_svn.chkSvnInfomation();
	
	WIDTH		=	GetSystemMetrics(SM_CXSCREEN);
	HEIGHT		=	GetSystemMetrics(SM_CYSCREEN);
	
#ifdef	_ISCONVERTOR
	selectLocalizeFolder(NULL);
	
	char	strFolder[512];
	
	sprintf(strFolder,"localize/%s",MAIN.m_strLocalizeFolder);
	
	SetCurrentDirectory(strFolder);
	
	GetCurrentDirectory(sizeof(_logFolder),_logFolder);
#endif
	
	int	iSumFile=	IsFile("dev data/mangchi/resource.sd")+IsFile("dev data/mangchi/sprite.sd")+IsFile("data/skill.dat");
	
	if	(iSumFile	<	3)
	{
		CRegKey		reg;
		
		int	iResult	=	reg.Open( HKEY_CURRENT_USER, "software\\L&K Logic Korea\\Red Stone for dev");

		if	(iResult	!= ERROR_SUCCESS)
			iResult	=	reg.Open( HKEY_CURRENT_USER, "software\\L&K Logic Korea\\Red Stone");

		if	(iResult	!= ERROR_SUCCESS)
			iResult	=	reg.Open( HKEY_LOCAL_MACHINE, "software\\L&K Logic Korea\\Red Stone");

		if (iResult	==	ERROR_SUCCESS)
		{	
			DWORD	count	=	sizeof(_logFolder);
			if (reg.QueryValue((LPTSTR )_logFolder,"Path",&count) == ERROR_SUCCESS)
			{	if (count	<=	0)
			return ERRMSG("Error in cMAIN::Init","can not find registry data of Red Stone");
			}
			else	return ERRMSG("Error in cMAIN::Init","can not find registry data of Red Stone");
		}
		else	return ERRMSG("Error in cMAIN::Init","can not find registry data of Red Stone");
		
		if (!SetCurrentDirectory(_logFolder))
			return ERRMSG("error!!","can not find folder '%s'",_logFolder);
		
		if (!IsFile("dev data/mangchi/resource.sd"))
			return ERRMSG("error!!","can not find file '%s/dev data/mangchi/resource.sd'",_logFolder);
		if (!IsFile("dev data/mangchi/sprite.sd"))
			return ERRMSG("error!!","can not find file '%s/dev data/mangchi/sprite.sd'",_logFolder);
	}
	
	/*
	{
	CRegKey		reg;
	
	  if	(reg.Open( HKEY_LOCAL_MACHINE, "software\\TortoiseSVN") == ERROR_SUCCESS)
	  {
	  DWORD	count	=	sizeof(g_strSVNPath);
	  
		if	(reg.QueryValue((LPTSTR )g_strSVNPath,"ProcPath",&count) == ERROR_SUCCESS)
		{	
		if	(count	<=	0)
		return ERRMSG("Error in cMAIN::Init","can not find registry data of TortoiseSVN");
		}
		else
		return ERRMSG("Error in cMAIN::Init","can not find registry data of TortoiseSVN");
		}
		else
		return ERRMSG("Error in cMAIN::Init","can not find registry data of TortoiseSVN");
		
		  }
	*/		
	MSGOUT("********** class size Report ***********");
	MSGOUT("cACTOR       %d",sizeof(cACTOR));
	MSGOUT("cITEM        %d",sizeof(cITEM));
	MSGOUT("cACTOR_SET   %d",sizeof(cACTOR_SET));
	MSGOUT("cJOB         %d",sizeof(cJOB));
	MSGOUT("g_aBasicItem %d",sizeof(g_aBasicItem));
	MSGOUT("cCHARACTER   %d",sizeof(cCHARACTER));
	
	/*
	{
	cFILE	file1,file2;
	int		iFile1Size,iFile2Size;
	
	  if	(file1.Open("Red Stone 01.exe","rb"))
	  {
	  iFile1Size	=	file1.Length;
	  file1.Read(aBuffer1,file1.Length);
	  file1.Close();
	  }
	  
		if	(file2.Open("Red Stone 02.exe","rb"))
		{
		iFile2Size	=	file2.Length;
		file2.Read(aBuffer2,file2.Length);
		file2.Close();
		}
		
		  if	(iFile1Size	==	iFile2Size)
		  for	(int i=0;i<iFile2Size;i++)
		  if	(aBuffer1[i]	!=	aBuffer2[i])
		  aBuffer1[i]		=	aBuffer1[i];
		  }
	*/
		
	CKarma::RESET();
	cITEM::Init();
	cDRAW::Init();
	cPROJECT::LoadBlockedTileInfo();

	if	(!g_titleManager.loadData()							)
		return	FALSE;
	
	MSGOUT("1");
	if	(!RESOURCE.Load("dev data/mangchi/resource.sd")		)
		return	FALSE;
	if	(!g_Sprite.Load("dev data/mangchi/sprite.sd")		)
		return	FALSE;
	if	(!g_anmGate.Load("data/interface/game/gateAnm.sad")	)
		return	FALSE;
	
	MSGOUT("2");
	if (!cSKILL::Init("data/skill.dat")						)
		return	FALSE;
	g_trapSkillManager.init();
	cJOB::Init();

	if	(!g_em.init())
		return FALSE;
	
	SetLogFolder();
	
	MSGOUT("3");
	g_bmMapList.Init(dMAX_MAP,dBAR_RADIO);		//	바메뉴 초기화
	MSGOUT("4");
	//	cMAP::Init(100,100,0,"Default Map");	//	컨피그 로딩 실패!!
	
	if	(!LoadConfig())		//	config 데이터를 불러 온다.
	{	//	초기화에 실패했다.
		if	(!cPROJECT::NewProject(hInst,NULL,TRUE))
			return FALSE;
	}

	cSET_FONT::InitFont(hWND);
	
	MSGOUT("5");
	if (g_aMap[0].m_iSerial	==	0xffff)
	{
		cMAP::Init(100,100,0,"Default Map");	//	컨피그 로딩 실패!!
		cMAP::Select(0);
		g_aMap[0].LoadMemoFile();
	}
	
	if (!cWND::Init(0	,lpszAppName,lpszTitle,WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN |WS_CLIPSIBLINGS,
		0	,0,WIDTH,HEIGHT,
		NULL,NULL,hInst,NULL,
		SW_SHOWMAXIMIZED,
		(WNDPROC)WndProc
		)
		)
		return FALSE;
	// 10.01.12
	cMemoWindow::CreateMemo(hInst, hWND);
	cMemoWindow::SetMemo(&g_lpActmap->m_sMemoInfo);
	
	MSGOUT("6");
	cSET::Init(hInst,hWND);
	
	char	strTitle[1024];
	
	sprintf(strTitle,"Mangchi II for Red Stone - %s",_logFolder);
	
	SetWindowText(hWND,strTitle);
	
	g_hNormalCursor	=	LoadCursor(NULL,IDC_ARROW);
	g_hAreaCursor	=	LoadCursor(hINST,"CURSOR_AREA");
	g_hEditCursor	=	LoadCursor(hINST,"CURSOR_EDIT");
	
	MSGOUT("7");
	InitWnds();
	
	TOOLBAR.Check(IDM_SCROLL		,TRUE);
	TOOLBAR.Check(IDM_HOUSELAYER	,TRUE);
	TOOLBAR.Check(IDM_OBJECTLAYER	,TRUE);
	TOOLBAR.Check(IDM_UNITLAYER		,TRUE);
	TOOLBAR.Check(IDM_AREALAYER		,TRUE);
	TOOLBAR.Check(IDM_DOOR_LAYER	,TRUE);
	TOOLBAR.Check(IDM_STATE_AUTO_SAVE,g_isAutoSave);
	
	InitBuffer();
		
	//	__SVN("/command:cleanup /path:\"D:/svn_common/rs_online/1.txt\" /notempfile");
	
	
	return	SAFETY;
}	//	cMAIN::Init(HINSTANCE hInst)

//	윈도우들 위치 설정
void
cMAIN::SetWnds()
{
	rectTOOLWND.ClientFrame(hWND);
	rectTOOLWND.y	+=	TOOLBAR.height()+3;
	rectTOOLWND.ys	-=	TOOLBAR.height()+3;
	rectTOOLWND.xs	=	228;
	
	rectMAPWND.ClientFrame(hWND);
	rectMAPWND.y	+=	TOOLBAR.height()+3;
	rectMAPWND.ys	-=	TOOLBAR.height()+3;
	rectMAPWND.x	+=	rectTOOLWND.xs;
	rectMAPWND.xs	-=	rectTOOLWND.xs;
	
	TOOLBAR.Resize(wWidth(),30);
}

void
cMAIN::InitWnds()
{
	SetWnds();
	
	TOOLWND.Init(hINST,hWND,&rectTOOLWND);
	g_MAPWND.Init(hINST,hWND,&rectMAPWND);
	
	
	
	//g_posDisplayMax.Set(devMode.dmPelsWidth , devMode.dmPelsHeight);
	
	Draw();
}	
void cMAIN::InitBuffer(){			// 09.07.15 버퍼 시작.
	
	for (int i =0;i<dMAX_MAP;i++){

		if (g_aMap[i].m_iSerial	!=	0xffff)	{
			m_UrdoBuffer[i].Init(&g_aMap[i]);
			m_UrdoBuffer[i].addBuffer();
			
		}
	}
	
}
void cMAIN::Undo()
{									// 09.07.15 되돌리기
	m_UrdoBuffer[g_iSelectMap].Undo();
	g_MAPWND.resetCopyOrCut();
	Draw();
}

void cMAIN::Redo()
{									// 09.07.15 앞으로.
	m_UrdoBuffer[g_iSelectMap].Redo();
	g_MAPWND.resetCopyOrCut();
	Draw();
}

void cMAIN::resetBuffer(int _iSelectmap)
{	// 09.07.15
	m_UrdoBuffer[g_iSelectMap].deleteBuffer();
	m_UrdoBuffer[g_iSelectMap].Init(&g_aMap[_iSelectmap]);
	m_UrdoBuffer[g_iSelectMap].addBuffer();
	
}

void
cMAIN::undoBufferUpdate()
{	// 09.07.15
	if	(g_lpActmap->m_bf1IsLocked)
		return ;

	if	((client.isIN(g_MAPWND.Mouse.x,g_MAPWND.Mouse.y) && g_MAPWND.Mouse.y > g_iTBSize)|| KEY[dKEY_ALT]&&LBUTTON)
	{
		if	(m_bBuffer == TRUE || TOUCHKEY[dKEY_DELETE])
		{
			//if	(m_UrdoBuffer[g_iSelectMap].isEqualBuffer() == TRUE)
			if (TRUE)
			{
				m_bBuffer = FALSE;
				return;
			}

			m_UrdoBuffer[g_iSelectMap].addBuffer();

			if	(g_isAutoSave == TRUE)
			{
				static	DWORD	dwLastSaveTime	=	0;

				DWORD	dwCurrentTime	=	timeGetTime();

				//	10초에 한번씩 저장
				//	할건데... 일단 계속 저장하게 해 보고 별 문제 없으면 계속 저장하게 하자.
//				if	(dwCurrentTime	>=	dwLastSaveTime+1000*10	||	dwCurrentTime	<	dwLastSaveTime)
				{
					cMAP::AutoSave(g_lpActmap->m_strFName,g_iSelectMap);

					dwLastSaveTime	=	dwCurrentTime;
				}
			}
		}
	}

	m_bBuffer = FALSE;
}

void
cMAIN::Resize()
{	
	SetWnds();
	
	TOOLWND.Resize((cRECT *)&rectTOOLWND);
	g_MAPWND.Resize((cRECT *)&rectMAPWND);
	
	Draw();
}	//	cMAIN::Resize()

//
//	키보드 업데이트
//
void
cMAIN::UpdateKey()
{
	if (g_iWorkMode	==	dWORK_TILE)
	{
		int	old	=	g_iBrushSize;
		if (TOUCHKEY[dKEY_ADD]		)	g_iBrushSize++;
		if (TOUCHKEY[dKEY_SUBTRACT]	)	g_iBrushSize--;
		
		g_iBrushSize	=	max(g_iBrushSize,1);
		g_iBrushSize	=	min(g_iBrushSize,5);
		
		if (old !=	g_iBrushSize)	{
			g_MAPWND.Draw();
		}
	}
	
	if	(TOUCHKEY[0x52])
	{			// 09.07.10  R 키 건물레이어
		
		g_bIsOnBuildingLayer=	!TOOLBAR.isCheck(IDM_HOUSELAYER);
		
		TOOLBAR.Check(IDM_HOUSELAYER,g_bIsOnBuildingLayer); 
		Draw();
		
	}else
	if	(TOUCHKEY[0x54])
	{	// 09.07.10  T 키 오브젝트레이어
		g_bIsOnObjectLayer	=	!TOOLBAR.isCheck(IDM_OBJECTLAYER);
		TOOLBAR.Check(IDM_OBJECTLAYER,g_bIsOnObjectLayer); 
		Draw();
		
	}
	else
	if	(TOUCHKEY[0x59])
	{	// 09.07.10  Y 키 유닛레이어
		g_bIsOnUnitLayer	=	!TOOLBAR.isCheck(IDM_UNITLAYER);
		TOOLBAR.Check(IDM_UNITLAYER,g_bIsOnUnitLayer);
		Draw();
		
	}
	else
	if	(TOUCHKEY[0x55])
	{	// 09.07.10  U 키 영역레이어
		g_bIsOnAreaLayer	=	!TOOLBAR.isCheck(IDM_AREALAYER);
		TOOLBAR.Check(IDM_AREALAYER,g_bIsOnAreaLayer);
		Draw();
		
	}
	else
	if	(TOUCHKEY[0x49])
	{	// 09.07.10  I 키 문레이어
		g_bIsOnDoorLayer	= !TOOLBAR.isCheck(IDM_DOOR_LAYER);
		TOOLBAR.Check(IDM_DOOR_LAYER,g_bIsOnDoorLayer);
		Draw();
		
	}else
	if	(TOUCHKEY[0x46])
	{	// 09.07.10  f 키 문레이어
		g_isCHECK_BLOCK	=	!TOOLBAR.isCheck(IDM_CHECKBLOCK);
		TOOLBAR.Check(IDM_CHECKBLOCK,g_isCHECK_BLOCK);
		Draw();
	}else
	if	(TOUCHKEY[0x47])
	{	// 09.07.10  g 키 문레이어
		g_isGRID		=	!TOOLBAR.isCheck(IDM_GRID);
		TOOLBAR.Check(IDM_GRID,g_isGRID);
		Draw();
	}else
	if	(TOUCHKEY[0x48])
	{	// 09.07.10  h 키 문레이어
		g_isLOCKED		=	1-TOOLBAR.isCheck(IDM_SCROLL);
		TOOLBAR.Check(IDM_SCROLL,g_isLOCKED);
		Draw();
	}else
	if	(TOUCHKEY[0x4a])
	{	// 09.07.10  j 키 문레이어
		g_bIsDisplayTileIndex		=	!TOOLBAR.isCheck(IDM_DISPLAY_TILE_INDEX);
		
		TOOLBAR.Check(IDM_DISPLAY_TILE_INDEX,g_bIsDisplayTileIndex);
		Draw();
	}else 
	if	(TOUCHKEY[0x4b])
	{	// 09.07.10  k키 문레이어
		
		g_lpActmap->m_bf1IsLocked	=	!TOOLBAR.isCheck(IDM_BLOCK_DRAW_TILE);
		TOOLBAR.Check(IDM_BLOCK_DRAW_TILE,g_lpActmap->m_bf1IsLocked);
		Draw();
	}
	else
	if	(TOUCHKEY[0x4c])
	{		//09. 07. 24 L 이동불가타일설정
		g_isCHECK_BLOCK	=	!TOOLBAR.isCheck(IDM_CHECKBLOCK);

		if	(g_isCHECK_BLOCK == TRUE)
			MAIN.ChangeMode(dWORK_CANNOTMOVETILE);
		else
		for(int i=0;i<TOOLWND.m_WorkButton.Count;++i)
		{
			if	(TOOLWND.m_WorkButton.IsCheck(i + dTB_TILE) != TRUE)
				continue;

			TOOLWND.m_WorkButton.Check(i + dTB_TILE);
			g_iWorkMode = g_iPreWorkMode;
			TOOLWND.ChangeMode(i);
			MAIN.ChangeMode(i);
			break;
		}

		TOOLBAR.Check(IDM_CHECKBLOCK,g_isCHECK_BLOCK);
		Draw();		
	}
	else
	if	(TOUCHKEY[dKEY_ESCAPE])
	{	// 09.07.30  esc 키 수정
		g_MAPWND.resetCopyOrCut();
		g_lpActmap->m_object.freeSelect();
		g_lpActmap->m_setActor.freeSelect();
		
		ChangeMode(g_iPreWorkMode);

		g_MAPWND.Draw();	
		
	}
	
	if(m_bLButtonDown == TRUE && !g_lpActmap->m_bf1IsLocked)
	{			// 09.07.15  마우스 키..undo를 위한.
		if(LBUTTON == FALSE)
		{
			m_bLButtonDown = FALSE;
			m_bBuffer = TRUE;
		}
	}else
	{
		if(LBUTTON)
		{
			m_bLButtonDown = TRUE;
		}
		m_bBuffer	= FALSE;
	}
			
			
}	//	cMAIN::UpdateKey()

//
//	업데이트
//
BOOL
cMAIN::Run()
{
	if	(GetFocus()!=hWND)
		return TRUE;
	
	cINPDEV::Update();
	UpdateKey();
	
	TOOLWND.Run();
	g_MAPWND.Run();
	
	undoBufferUpdate();

	if	(m_pMinimapWnd && IsWindow(m_pMinimapWnd->hWND) == TRUE)
	{
		m_pMinimapWnd->Run();
	}
	
	
	
	return TRUE;
}

//
//	모든 윈도우를 다시 그려준다.
//
void
cMAIN::Draw()
{
	MAIN.checkToolbarButton();
	TOOLWND.Draw();
	g_MAPWND.Draw();
	// ┏09.07.01 미니맵추가 
	if(m_pMinimapWnd && IsWindow(m_pMinimapWnd->hWND) == TRUE){
		m_pMinimapWnd->Draw();
	}
	// ┗
				
				
	
}

//
//	현재 작업중인 모드를 변경한다.
//
void
cMAIN::ChangeMode(int mode)
{
	if (mode == g_iWorkMode)
		return;
	
	g_iPreWorkMode	=	g_iWorkMode;
	g_iWorkMode		=	mode;
	
	TOOLWND.ChangeMode(mode);
	
	Draw();
}

//
//	새로운 맵
//
BOOL
cMAIN::NewMap()
{
	// 10.01.12
	cMemoWindow::SaveText();
	int iResult = cSET::NewMap(hINST,hWND);
	
	// ┏09.07.01 미니맵추가 
	if(iResult && m_pMinimapWnd&&IsWindow(m_pMinimapWnd->hWND)){
		CreateMiniMap();		
	}
	// ┗
	if(iResult == TRUE){					// 09.07.15 버퍼.
		m_UrdoBuffer[g_iSelectMap].deleteBuffer();
		m_UrdoBuffer[g_iSelectMap].Init(&g_aMap[g_iSelectMap]);
		m_UrdoBuffer[g_iSelectMap].addBuffer();
	}
	TOOLWND.ChangeMode(g_iWorkMode);
	Draw();
	
	return TRUE;
}

//
//	맵 초기화
//
BOOL
cMAIN::ResetMap()
{
	int iResult = cSET::ResetMap(hINST,hWND);
	
	
	// ┏09.07.01 미니맵추가 
	if(iResult && m_pMinimapWnd&&IsWindow(m_pMinimapWnd->hWND)){
		CreateMiniMap();		
	}
	// ┗
	
	if(iResult == TRUE){					// 09.07.15 버퍼.
		m_UrdoBuffer[g_iSelectMap].deleteBuffer();
		m_UrdoBuffer[g_iSelectMap].Init(&g_aMap[g_iSelectMap]);
		m_UrdoBuffer[g_iSelectMap].addBuffer();
	}
	
	Draw();
	
	return TRUE;
}

//
//	맵 데이터 설정
//
BOOL
cMAIN::SetMap()
{
	cSET::SetMap(hINST,hWND);
	
	Draw();
	
	return TRUE;
}	//	cMAIN::SetMap()

//
//	편집중인 맵 모두 저장
//
BOOL
cMAIN::SaveMaps()
{
	for (int i=0;i<dMAX_MAP;i++)
	{
		if	(g_aMap[i].IsActive())
		{
			cMAP::Select(i);
			Draw();
			if(!cPROJECT::SaveMap(g_aMap[i].m_strFName,i)) return FALSE;
		}
	}
	// 10.01.12
	cMemoWindow::SetMemo(&g_lpActmap->m_sMemoInfo);
	
	return TRUE;
}	//	cMAIN::SaveMaps()

void
cMAIN::quickLaunch()
{
	cRECT	rect;
	
	rect.Set(0,0,0x7fffffff,0x7fffffff);
	g_lpActmap->selectObject(&rect);
	g_lpActmap->removeSelectedObject();
	g_lpActmap->loadMapsetImage();
	
	
	Draw();
	
	//	ShellExecute(NULL,"open","red stone.exe",NULL,NULL,SW_SHOWNORMAL);
}

//
//	맵 저장
BOOL
cMAIN::exportMiniMapImage(HWND hwnd,int _iImageScale,BOOL _bIsAuto)
{
	char	strName[256];
	
	sprintf(strName,"%s.tga",g_lpActmap->m_strFName);
	
	if	(_bIsAuto==FALSE)
	{
		if (!cFILE::SaveBox(strName,"tga file {*.tga}\0*.tga\0\0","tga",m_strExportMinimapFolder,"미니맵 익스포트"))
		{
			SetLogFolder();
			
			return	FALSE;
		}
	}
	
	cPROJECT::SetMapFolder();
	
	SetCurrentDirectory("..");
	
	GetCurrentDirectory(sizeof(m_strExportMinimapFolder),m_strExportMinimapFolder);
	
	g_MAPWND.drawMapToFile(strName,_iImageScale);
	
	SetLogFolder();
	
	return	TRUE;
}


//
//	
BOOL
cMAIN::resaveAllField(HWND hwnd)
{
	cPROJECT::SetMapFolder();
	
	cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");
	
	SetLogFolder();
	
	if (!lpFiles)
		return	ERRMSG("Error!!", "There is no map.");
	
	for (int i=0;i<lpFiles->Count;i++)
	{
		cSTRING	*lpFile	=	lpFiles->Pick(i);
		
		cPROJECT::SetMapFolder();
		cMAP::Load(lpFile->String,0);
		SetLogFolder();
		Draw();
		Sleep(1);
		
		cPROJECT::SaveMap(lpFile->String,0);
	}
	
	cMSG::Put("Resave all field files", "Completed");
	
	KILL(lpFiles);
	
	return	TRUE;
}	//	cMAIN::resaveAllField(HWND hwnd)

//
//	모든 필드의 출구를 연결 한다.
BOOL
cMAIN::linkAllFieldsGate(HWND hwnd,BOOL _bIsSave)
{
	cPROJECT::SetMapFolder();
	
	cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");
	
	SetLogFolder();
	
	if (!lpFiles)
		return	ERRMSG("Error!!","There is no map. ");
	
	for (int i=0;i<lpFiles->Count;i++)
	{
		cSTRING	*lpFile	=	lpFiles->Pick(i);
		
		cPROJECT::SetMapFolder();
		cMAP::Load(lpFile->String,0);
		SetLogFolder();
		Draw();
		
		if (!g_aMap[0].m_pArea->linkGate())
		{
			KILL(lpFiles);
			ERRMSG("Error!!", "The exit area is not connected properly.");
		}
		
		if (_bIsSave)	cPROJECT::SaveMap(lpFile->String,0);
		else			
		{
			if(!g_aMap[0].m_pArea->checkTwoWayLinkGate(lpFile->String))
			{
				KILL(lpFiles);
				
				return	FALSE;
			}
		}
	}
	
	cMSG::Put("Exit Area Link Operation", "Completed");
	
	KILL(lpFiles);
	
	return	TRUE;
}	//	cMAIN::linkAllFieldsGate(HWND hwnd)

BOOL
cMAIN::compareFields(HWND hwnd)
{
	char	strFolder1[512],strFolder2[512];
	
	sprintf(strFolder1,"%s/folder1",_logFolder);
	sprintf(strFolder2,"%s/folder2",_logFolder);
	
	if (!IsFolder(strFolder1))	return	ERRMSG("Error!!","Cannot find folder %s.",strFolder1);
	if (!IsFolder(strFolder2))	return	ERRMSG("Error!!","Cannot find folder %s.",strFolder2);
	
	SetCurrentDirectory(strFolder1);
	
	cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");
	
	if (!lpFiles)	return	ERRMSG("Error!!","There is no map. ");
	
	BYTE	*pBlockedArea1,*pBlockedArea2;
	cPOINT	map1Size,map2Size;
	
	for (int i=0;i<lpFiles->Count;i++)
	{
		pBlockedArea1	=	NULL;
		pBlockedArea2	=	NULL;
		
		cSTRING	*lpFile	=	lpFiles->Pick(i);
		
		char	strFile1[512],strFile2[512];
		
		sprintf(strFile1,"%s/%s",strFolder1,lpFile->String);
		sprintf(strFile2,"%s/%s",strFolder2,lpFile->String);
		
		pBlockedArea1	=	cMAP::LoadBlock(strFile1,&map1Size);
		pBlockedArea2	=	cMAP::LoadBlock(strFile2,&map2Size);
		
		if (!pBlockedArea1	||	!pBlockedArea2)
		{
			pKILL(pBlockedArea1);
			pKILL(pBlockedArea2);
			KILL(lpFiles);
			return FALSE;
		}
		
		if (map1Size.x != map2Size.x || map1Size.y != map2Size.y )
		{
			ERRMSG("Error!!","The field size of '%s' is different.",lpFile->String);
			
			pKILL(pBlockedArea1);
			pKILL(pBlockedArea2);
			
			KILL(lpFiles);
			return	FALSE;
		}
		
		for (int iY=0;iY<map1Size.y;iY++)
			for (int iX=0;iX<map1Size.x;iX++)
			{
				int	iAddr	=	iX+iY*map1Size.x;
				
				if (pBlockedArea1[iAddr] != pBlockedArea2[iAddr])
				{
					ERRMSG("Error!!","The tile properties of %d and %d of '%s' are different.",lpFile->String,iX,iY);
					
					iY	=	map1Size.y;
					iX	=	map1Size.x;
				}
			}
			
			pKILL(pBlockedArea1);
			pKILL(pBlockedArea2);
			
			//		cMSG::Put("작업 진행 상황","'%s' 이상 없음",lpFile->String);
	}
	
	KILL(lpFiles);
	
	cMSG::Put("Operation completed", "Operation completed");
	
	return	TRUE;
}	//	cMAIN::compareFields(HWND hwnd)
BOOL	cMAIN::onLoadMap(HINSTANCE _hInst, HWND _hWnd){			// 09.07.02  추가.
	BOOL iResult = 	cPROJECT::LoadMap(_hInst, _hWnd);
	if(iResult == TRUE){
		if(m_pMinimapWnd&&IsWindow(m_pMinimapWnd->hWND)){
			CreateMiniMap();		
		}
		m_UrdoBuffer[g_iSelectMap].deleteBuffer();					// 09.07.15 버퍼.
		m_UrdoBuffer[g_iSelectMap].Init(&g_aMap[g_iSelectMap]);		// 09.07.15 버퍼.
		m_UrdoBuffer[g_iSelectMap].addBuffer();						// 09.07.15 버퍼.
		
	}
	MAIN.Draw();
	return iResult;
	
}

BOOL	cMAIN::onAddMap(HINSTANCE _hInst, HWND _hWnd){			// 09.07.02  추가.
	
	BOOL iResult = cPROJECT::AddMap(MAIN.hINST,MAIN.hWND);
	if(iResult == TRUE){
		if(m_pMinimapWnd&&IsWindow(m_pMinimapWnd->hWND)){
			CreateMiniMap();		
		}
		m_UrdoBuffer[g_iSelectMap].deleteBuffer();						// 09.07.15 버퍼.
		m_UrdoBuffer[g_iSelectMap].Init(&g_aMap[g_iSelectMap]);			// 09.07.15 버퍼.
		m_UrdoBuffer[g_iSelectMap].addBuffer();							// 09.07.15 버퍼.
		
	}
	MAIN.Draw();
	return iResult;
	
}

BOOL
cMAIN::operateWM_Command(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch( LOWORD( wParam ) )
	{
		case	IDM_SET_PROJECT_KARMA	:
			{
				CKarmaContainer	kc;
				
				kc.copy(&cPROJECT::s_kcMonsterDeath,TRUE);
				
				if	(cKARMA_WND::Open(MAIN.hWND,MAIN.hINST,&kc,TRUE,FALSE))
					cPROJECT::s_kcMonsterDeath.copy(&kc);
				break;
			}
			
		case	IDM_SET_PROJECT_KARMA_MONSTER_GEN	:
			{
				CKarmaContainer	kc;
				
				kc.copy(&cPROJECT::s_kcMonsterGen,TRUE);
				
				if	(cKARMA_WND::Open(MAIN.hWND,MAIN.hINST,&kc,TRUE,FALSE))
					cPROJECT::s_kcMonsterGen.copy(&kc);
				break;
			}
			
		case	IDM_GET_LOCK			:
			g_lpActmap->getLock(TRUE);
			break;
			
		case	IDM_RELEASE_LOCK		:
			g_lpActmap->releaseLock(TRUE);
			break;
			
		case	IDM_UPDATE_CURRENT_FILE	:
			g_lpActmap->updateSVN(TRUE);
			break;
			
		case	IDM_COMMIT				:
			g_lpActmap->commitSVN(TRUE);
			break;
			
		case	IDM_CHARACTER_PATTERN	:
			cSetCharacterPattern::Open(hwnd);
			break;
			
		case	IDM_COMMAND				:
			cCommandDlg::Init(hwnd);
			break;
			
		case	IDM_SET_ID					:
			cSetSecretDungeon::Open(hwnd);
			break;
			
		case	IDM_CORRECT_TILE			:
			cSetCorrectTile::Open(hwnd);
			break;
		case	IDM_OPERATE_TEMPLATE_WORK	:
			MAIN.operateTemplateWork(hwnd);
			break;
			
		case	IDM_EXPORT_EXP			:
			MAIN.exportExpReward(hwnd);
			break;
		case	IDM_EXPORT_KARMA		:
			MAIN.exportKarma(hwnd);
			break;
		case	IDM_EXPORT_ITEM_INFO	:
			MAIN.exportItemInfo(hwnd);
			break;
			
		case	IDM_EXPORT_SHOP_INFO	:
			MAIN.exportShopInfo(hwnd);
			break;
			
		case	IDM_SYNC_TEXT			:
			MAIN.forceSyncCommonText(hwnd);
			break;
			
		case	IDM_EXPORT_MONSTER_INFO	:
			MAIN.exportMonsterInfo(hwnd);
			break;

		case	IDM_EXPORT_ALL_MAP_ITEM_DROP_INFO	:
			MAIN.exportAllMapDropItemInfo(hwnd);
			break;
			
		case	IDM_EXPORT_NPC_INFO		:
			MAIN.exportNpcInfo(hwnd);
			break;

		case	IDM_EXPORT_CURRENT_FIELD_NPC_INFO	:
			MAIN.exportCurrentFieldNpcInfo(hwnd);
			break;
			
		case	IDM_EXPORT_ALL_TEXT	:
			MAIN.exportAllText(hwnd);
			break;
			
		case	IDM_EXPORT_SOME_MAP	:
			MAIN.exportAllText(hwnd,TRUE);
			break;
			
		case	IDM_LOCALIZING			:
			MAIN.localizing(hwnd);
			break;
			
		case	IDM_EXPORT_MAP_LIST		:
			MAIN.exportLoclalizeMapList(hwnd);
			break;
			
		case	IDM_LOCALIZING_PART			:
			MAIN.localizing(hwnd,TRUE);
			break;
			
		case	IDM_UPDATE_NEW_TEXT		:
			MAIN.modifyText(hwnd);
			break;
			
		case	IDM_COMPARE_TEXT		:
			MAIN.compareLocalizeText(hwnd);
			break;
		case	IDM_CHECK_TEXT			:
			MAIN.compareLocalizeText(hwnd,TRUE);
			break;
			
		case	IDM_SET_PROJECT	:
			cSET_PROJECT::Init(hwnd);
			break;
			
		case	IDM_JOB			:
			cJOB_LIST::Init(MAIN.hWND,MAIN.hINST);
			break;
			
		case	IDM_SAVE_PROJECT:	//	프로젝트 저장하기
			cPROJECT::SaveProject(MAIN.hINST,MAIN.hWND);
			break;
			
		case	IDM_LOAD_PROJECT:	//	프로젝트 불러오기
			cPROJECT::LoadProject(MAIN.hINST,MAIN.hWND);
			g_MAPWND.ReBuildAll();
			break;
			
		case	IDM_SAVE_PROJECT_QUICK	:	//	프로젝트 저장하기
			cPROJECT::SaveProject();
			break;
		case	IDM_VIEW_MINIMAP:					// 09.07.01  미니맵 보기 메뉴.  10일 추가수정.
			
			if(MAIN.m_pMinimapWnd){
				DestroyWindow(MAIN.m_pMinimapWnd->hWND);
				delete MAIN.m_pMinimapWnd;
				MAIN.m_pMinimapWnd = NULL;
				
			}else{
				MAIN.CreateMiniMap();
			}
			break;
		case	IDM_EXPORT_MINIMAP_IMAGE_50	:	//	미니맵 추출(50%사이즈).
			MAIN.exportMiniMapImage(hwnd,50);
			break;
			
		case	IDM_EXPORT_MINIMAP_IMAGE_25	:	//	미니맵 추출(25%사이즈).
			MAIN.exportMiniMapImage(hwnd,25);
			break;
		case IDM_MINIMAP_AREA:
			g_MAPWND.m_bAdditionPrintArea = 1 - g_MAPWND.m_bAdditionPrintArea;
			g_MAPWND.Draw();
			break;
		case IDM_MINIMAP_ACTOR:
			g_MAPWND.m_bAdditionPrintActor = 1 - g_MAPWND.m_bAdditionPrintActor;
			g_MAPWND.Draw();
			break;			
		case	IDM_LINK_ALL_MAP_GATE			:
			MAIN.linkAllFieldsGate(hwnd,FALSE);
			break;
			
		case	IDM_LINK_ALL_MAP_GATE_AND_SAVE	:
			MAIN.linkAllFieldsGate(hwnd,TRUE);
			break;
			
		case	IDM_RESAVE_ALL_FIELD			:
			MAIN.resaveAllField(hwnd);
			break;
			
		case	IDM_COMPARE_MAP				:
			MAIN.compareFields(hwnd);
			break;
			
		case	IDM_SET_PIECEITEM			:
			cSetPieceItem::Open(hwnd);
			break;
			
		case	IDM_SET_BASIC_ITEM	:	//	기본 아이템 목록 설정
			{
				cSET_BASIC_ITEM::Init(MAIN.hWND);
				break;
			}
			
		case	IDM_ITEM_PREFIX		:
			{
				cSET_ITEM_PREFIX::Init(MAIN.hWND);
				break;
			}
			
		case	IDM_EDIT_DIALOG		:
			{
				CSetDialogList::Init(hwnd);
			}
			
		case	IDM_HOUSELAYER		:
			{
				g_bIsOnBuildingLayer=	MAIN.TOOLBAR.isCheck(IDM_HOUSELAYER);
				g_MAPWND.Draw();
				
				
				break;
			}
		case	IDM_OBJECTLAYER		:
			{
				g_bIsOnObjectLayer	=	MAIN.TOOLBAR.isCheck(IDM_OBJECTLAYER);
				g_MAPWND.Draw();
				
				break;
			}
		case	IDM_UNITLAYER		:
			{
				g_bIsOnUnitLayer	=	MAIN.TOOLBAR.isCheck(IDM_UNITLAYER);
				g_MAPWND.Draw();
				
				break;
			}
		case	IDM_AREALAYER		:
			{
				g_bIsOnAreaLayer	=	MAIN.TOOLBAR.isCheck(IDM_AREALAYER);
				g_MAPWND.Draw();
				
				break;
			}
		case	IDM_DOOR_LAYER		:
			{
				g_bIsOnDoorLayer	=	MAIN.TOOLBAR.isCheck(IDM_DOOR_LAYER);
				g_MAPWND.Draw();
				
				break;
			}
		case	IDM_OBJECTUPDATE	:
			{
				int Index = GetDlgItemInt(MAIN.TOOLBAR.hWND, IDM_OBJECTFIND, NULL, NULL);
				
				cFIXED_OBJECT_INFO	*lpObject	=	g_lpActmap->m_object.m_denFixedObject.get(Index);

				if(lpObject)
				{
					g_lpActmap->m_object.freeSelect();
					lpObject->m_isSelected = TRUE;

					int x = (int)((lpObject->m_pos.x / dTILE_WIDTH)  - (g_MAPWND.TPW / 2));
					int y = (int)((lpObject->m_pos.y / dTILE_HEIGHT) - (g_MAPWND.TPH / 2));
			
					g_lpActmap->setPos( x,y);
					
					g_MAPWND.Draw();
				}
				SetFocus(hWND);
				break;	
			}
		// 새로 추가한 ID
		case IDM_MEMOVIEW:
			{
				cMemoWindow::m_bShowWindow = MAIN.TOOLBAR.isCheck(IDM_MEMOVIEW);	
				
				if(cMemoWindow::m_bShowWindow)
					ShowWindow(cMemoWindow::m_hMemo, SW_SHOW);
				else
					ShowWindow(cMemoWindow::m_hMemo, SW_HIDE);
				break;
			}		
		case	IDM_REBUILD_CRASH_INFO	:
			cPROJECT::LoadBlockedTileInfo();
			break;
			
		case	IDM_FILL_SELECT_TILE	:
			if	(cMSG::YESNO("필드 타일 초기화","기존의 필드가 선택된 타일로 칠해집니다.\n\n... 진짜루 칠할껴?")==IDNO)
				break;
			
			g_lpActmap->m_pPattern->PutPattnIndex(g_lpActmap->m_iWidth,g_lpActmap->m_iHeight,g_lpActmap->m_iSelectTile,NULL,NULL);
			g_lpActmap->LostVirgin();
			g_MAPWND.Draw();
			
			break;
			
		case	IDM_SAVE_MAPS	:	//	맵들 저장
			// 10.01.12
			cMemoWindow::SaveText();
			MAIN.SaveMaps();
			break;
			
		case	IDM_QUICK_LAUNCH	:
			MAIN.quickLaunch();
			break;
			
		case	IDM_SAVE_MAP	:	//	맵 저장
			// 10.01.12
			cMemoWindow::SaveText();
			cPROJECT::SaveMap();
			break;
			
		case	IDM_SAVEMAP_AS	:	//	맵 저장
			cPROJECT::SaveMap(MAIN.hINST,MAIN.hWND);
			break;
			
		case	IDM_LOAD_MAP	:	//	맵 불러오기

			MAIN.onLoadMap(MAIN.hINST, MAIN.hWND);
			cSetDialogNpcData::ResetNpcListBox(	cSetDialogNpcData::s_iSelOldList);
			CSetDialogMonster::ResetMonsterListBox(	CSetDialogMonster::s_iSelOldList);
			CSetDialogMonster::ResetMonsterInfoBox(	CSetDialogMonster::s_iSelOldInfo);
			break;
		
		case	IDM_ADD_MAP	:	//	맵 불러오기
			// 10.01.12
			cMemoWindow::SaveText();
			MAIN.onAddMap(MAIN.hINST, MAIN.hWND);
			break;
			
		case	IDM_KARMA		:
			{
				CKarmaContainer	kc;
				
				kc.copy(&g_lpActmap->m_karma,TRUE);
				
				if	(cKARMA_WND::Open(MAIN.hWND,MAIN.hINST,&kc,TRUE,FALSE))
					g_lpActmap->m_karma.copy(&kc);
				break;
			}
			
		case	IDM_NEW_MAP		:	//	새로운 맵
			MAIN.NewMap();
			break;
			
		case	IDM_RESET_MAP	:	//	작업중인 맵 초기화
			MAIN.ResetMap();
			break;
			
		case	IDM_SET_MAP		:	//	작업중인 맵 재설정
			MAIN.SetMap();
			break;
			
		case	IDM_CHECKBLOCK	:
			g_isCHECK_BLOCK	=	MAIN.TOOLBAR.isCheck(IDM_CHECKBLOCK);
			g_MAPWND.Draw();
			
			break;
			
		case	IDM_GRID		:
			g_isGRID		=	MAIN.TOOLBAR.isCheck(IDM_GRID);
			g_MAPWND.Draw();
			
			break;
			
		case	IDM_SCROLL		:
			g_isLOCKED		=	1-MAIN.TOOLBAR.isCheck(IDM_SCROLL);
			break;
			
		case	IDM_DISPLAY_TILE_INDEX	:
			g_bIsDisplayTileIndex		=	MAIN.TOOLBAR.isCheck(IDM_DISPLAY_TILE_INDEX);
			g_MAPWND.Draw();
			
			break;
			
		case	IDM_BLOCK_DRAW_TILE		:
			g_lpActmap->m_bf1IsLocked	=	MAIN.TOOLBAR.isCheck(IDM_BLOCK_DRAW_TILE);
			g_MAPWND.Draw();
			
			break;
			
		case	IDM_SET_SHOP				:
			CSetShopList::Init(hwnd,&g_lpActmap->m_shop);
			break;
			
		case	IDM_SCALE_100	:
			g_iScale	=	MAIN.m_iMapScale[eMAP_SCALE_100];
			g_MAPWND.Draw();
			
			break;
		case	IDM_SCALE_50	:
			g_iScale	=	MAIN.m_iMapScale[eMAP_SCALE_50];
			g_MAPWND.Draw();
			
			break;
		case	IDM_SCALE_45	:
			g_iScale	=	MAIN.m_iMapScale[eMAP_SCALE_45];
			g_MAPWND.Draw();
			
			break;
		case	IDM_SCALE_40	:
			g_iScale	=	MAIN.m_iMapScale[eMAP_SCALE_40];
			g_MAPWND.Draw();
			break;
		case	IDM_SCALE_35	:
			g_iScale	=	MAIN.m_iMapScale[eMAP_SCALE_35];
			g_MAPWND.Draw();
			break;
		case	IDM_SCALE_30	:
			g_iScale	=	MAIN.m_iMapScale[eMAP_SCALE_30];
			g_MAPWND.Draw();
			break;
		case	IDM_SCALE_25	:
			g_iScale	=	MAIN.m_iMapScale[eMAP_SCALE_25];
			g_MAPWND.Draw();
			break;
		case	IDM_SCALE_20	:
			g_iScale	=	MAIN.m_iMapScale[eMAP_SCALE_20];
			g_MAPWND.Draw();
			break;
		case	IDM_SCALE_15	:
			g_iScale	=	MAIN.m_iMapScale[eMAP_SCALE_15];
			g_MAPWND.Draw();
			break;
		case	IDM_SCALE_10	:
			g_iScale	=	MAIN.m_iMapScale[eMAP_SCALE_10];
			g_MAPWND.Draw();
			break;
		case	IDM_SCALE_5	:
			g_iScale	=	MAIN.m_iMapScale[eMAP_SCALE_5];
			g_MAPWND.Draw();
			break;
			
		case	IDM_SET_QUEST	:
			cSETQUEST::SetQuest(MAIN.hINST,hwnd);
			break;
			
		case	IDM_SET_BOOK	:
			cSetBook::Open(hwnd);
			break;
			
		case	IDM_MAIN_QUEST	:
			cSetMainQuest::Open(hwnd);
			break;
			
		case	IDM_CUSTOM_ITEM	:
			CSetCustomItemList::Open(hwnd,&g_lpActmap->m_customItem);
			break;
			
		case	IDM_GLOBAL_CUSTOM_ITEM	:
			CSetCustomItemList::Open(hwnd,&g_customItem);
			break;
			
		case	IDM_SET_PREMIUM_ITEM	:
			CSetCustomItemList::Open(hwnd,&g_premiumItem);
			break;
			
		case	IDM_SET_ARACA			:
			cSetArca::Open(hwnd);
			break;
			
		case	IDM_SETITEM				:
			cSetItemSet::Open(hwnd);
			break;
			
		case	IDM_SET_CANNOT_MOVE_TILE:
			g_isCHECK_BLOCK = TRUE;
			MAIN.ChangeMode(dWORK_CANNOTMOVETILE);
			break;
		case	IDM_STATE_AUTO_SAVE:
			g_isAutoSave	=	MAIN.TOOLBAR.isCheck(IDM_STATE_AUTO_SAVE);
			break;
			
		case IDM_EDIT_UNDO:				// 09.07.15  undo
			MAIN.Undo();
			break;
		case IDM_EDIT_REDO:				// 09.07.15  redo
			MAIN.Redo();
			break;

		case IDM_EXPORT_MONSTER:
			MAIN.exportMonsterKarma();
			break;
		case IDM_EXPORT_ACTOR:
			MAIN.exportActorKarma();
			break;
		case IDM_EXPORT_FIELD:
			MAIN.exportFieldKarma();
			break;
		case IDM_EXPORT_AREA:
			MAIN.exportAreaKarma();
			break;
		case IDM_EXPORT_ALLKARMA:
			MAIN.exportMonsterKarma();
			MAIN.exportActorKarma();
			MAIN.exportFieldKarma();
			MAIN.exportAreaKarma();
			break;
		case	IDM_EDIT_NPC:			//			--------------------------- 메뉴에서 새로 만듬(맵 - 설정 - NPC 편집
			{
				cSetDialogNpcData::Init(hwnd);
				cSetDialogNpcData::ShowDialog();					
				break;
			}
		case	IDM_EDIT_MONSTER:
			{
				CSetDialogMonster::Init(hwnd);
				CSetDialogMonster::ShowDialog();
				break;
			}
		case	IDM_SET_FONTCOLOR:
				cSET_FONT::ShowDialog();
				break;
		// 10.01.15
		case	IDM_SET_AREA_EDIT:
				cSET_AREA_EDIT::SetAreaEdit(hwnd);
				cSET_AREA_EDIT::ShowAreaEdit();	
			break;
		case IDM_HELP:
			{
				if(IsWindow(m_pHelpWindow->m_hWndMain))
				{
					DestroyWindow(m_pHelpWindow->m_hWndMain);			
					delete m_pHelpWindow;
					m_pHelpWindow = NULL;
					break;
				}
				m_pHelpWindow = new CHelpWindow;
				m_pHelpWindow->Init(hINST, hWND);
				break;
			}
		case IDM_MOVE_FIELD:
			DialogBox(MAIN.hINST, "MOVE_FIELD", MAIN.hWND, (DLGPROC)g_MAPWND.WNDProcMovefield);
			break;
		case IDM_COMPARE:
			cCompareBox::open(MAIN.hWND, MAIN.hINST);
			break;

	}

	return	TRUE;
}

LRESULT CALLBACK 
cMAIN::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch( uMsg )
	{	
		case WM_CREATE		:
			MAIN.TOOLBAR.Init(hwnd,MAIN.hINST,102	,29,tbButtons, 0, 0, 19, 19,TB_BITMAP);
			MAIN.TOOLBAR.SetTip(strToolbar);
			break;
			
		case WM_ACTIVATEAPP :
			bACTIVEAPP = wParam;
			break;
			
		case	WM_KEYDOWN			:
		case	WM_KEYUP            :     
		case	WM_CHAR             :    
		case	WM_DEADCHAR         :   
		case	WM_SYSKEYDOWN       :  
		case	WM_SYSKEYUP         :    
		case	WM_SYSCHAR          :   
		case	WM_SYSDEADCHAR      :  
		case	WM_KEYLAST          : 
		{
			BYTE	buffer[256];
			GetKeyboardState(buffer);
			break;
		}
			
		case WM_MOUSEWHEEL:					// 축소확대
		{
			int zDelta = ((short)HIWORD(wParam));
			int iScalePos = 0;

			int iMouseX = LOWORD(lParam);

			if (iMouseX < rectTOOLWND.xs)
			{
				if (zDelta <= 0)
					TOOLWND.VScroll(SB_LINEDOWN);
				if (zDelta > 0)
					TOOLWND.VScroll(SB_LINEUP);
				break;
			}

			for(int i = 0; i< eMAP_SCALE_CNT; ++i)
			{
				if	(g_iScale == MAIN.m_iMapScale[i])
				{
					iScalePos = i;
					break;
				}
			}

			if	(zDelta>=0)
			{
				if	(iScalePos != eMAP_SCALE_CNT-1)
				{
					g_iScale = MAIN.m_iMapScale[++iScalePos];
					g_MAPWND.Draw();
				}
			}
			else
			{
				if	(iScalePos != 0)
				{
					g_iScale = MAIN.m_iMapScale[--iScalePos];
					g_MAPWND.Draw();
				}
			}
			break;
			
		}
		case WM_COMMAND		:
			MAIN.operateWM_Command(hwnd,uMsg,wParam,lParam);
			break;
			
		
		case WM_CLOSE		:
			if	(MAIN.Exit())
				PostQuitMessage(0);
			break;
			
		case WM_SIZE		:
		case WM_MOVE		:
			MAIN.Resize();
			break;
			
		default :
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}
	
	return( 0L );
}