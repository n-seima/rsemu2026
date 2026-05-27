#include "cGAME_INIT.H"
#include "cJOB.H"
#include "cCOMPRESS.H"
#include "CSkill.H"
#include "cItem.H"
#include "CHero.H"
#include "cPROJECT.H"
#include "CActor.H"
#include "CSound.H"
#include "CGamePlayInterface.H"
#include "CTitle.H"
#include "CMessage.H"
#include "CGuildSkill.H"
#include "CGuild.H"
#include "trap.H"
#include "trapSkill.H"
#include "firecracker.h"
#include "version.h"

enum
{
	eLGSDS_LOADING,
	eLGSDS_COMPLETE,
	eLGSDS_ERROR,
};


cTHREAD		cGAME_INIT::s_thLoading;
int			cGAME_INIT::s_iDataLoadingStatus	=	eLGSDS_LOADING;
cSPRITE		cGAME_INIT::s_sprLoadImage;

#define		dMAKE_GAME_INIT_LOG	1
BOOL		l_bIsConvertObjectFileName			=	FALSE;
BOOL		l_bIsConvertAnmationFile			=	FALSE;

#ifdef	_FOR_KOREA
char		*lpstrRegionInterfaceFile			=	"data/interface/game/interface_korea.sd";
#elif	_FOR_CHINA
char		*lpstrRegionInterfaceFile			=	"data/interface/game/interface_china.sd";
#elif	_FOR_JAPAN
char		*lpstrRegionInterfaceFile			=	"data/interface/game/interface_japan.sd";
#elif	_FOR_INTERNATIONAL
char		*lpstrRegionInterfaceFile			=	"data/interface/game/interface_international.sd";
#elif	_FOR_THAI
char		*lpstrRegionInterfaceFile			=	"data/interface/game/interface_thai.sd";
#elif	_FOR_USA
char		*lpstrRegionInterfaceFile			=	"data/interface/game/interface_USA.sd";
#endif

void
cGAME_INIT::LoadData(void* _lpData)
{
#ifdef	_DEBUG
	cMAP::Init(NULL,TRUE);
#else
	cMAP::Init(NULL);
#endif

	g_eh.addStaticLog("LoadData - begin");

	if	(!IsFile("data/convertImageData.dat"))
	{
		l_bIsConvertAnmationFile	=	TRUE;
		if	(!g_im.convertImageData051116())
		{
			s_iDataLoadingStatus	=	eLGSDS_ERROR;
			return;
		}
		cFILE	file;

		file.Open("data/convertImageData.dat","wb");
		file.Write(&l_bIsConvertAnmationFile,4);
		file.Close();

		l_bIsConvertAnmationFile	=	FALSE;
	}

	if	(0)
//	if	(IsFile("resourceManager"))
	{
		CopyFile("resourceManager","t.exe",FALSE);

		OSVERSIONINFO		osvi=	{NULL,};

		osvi.dwOSVersionInfoSize=	sizeof(OSVERSIONINFO);

		GetVersionEx(&osvi);

		SHELLEXECUTEINFO	sei	=	{NULL,};
		sei.cbSize				=	sizeof(SHELLEXECUTEINFO);

		if	(osvi.dwMajorVersion	==	6)
		{
			sei.lpVerb		=	"runas";
			sei.lpFile		=	"t.exe";

			sei.nShow		=	SW_SHOWNORMAL;
			sei.lpParameters=	NULL;

			ShellExecuteEx(&sei);
		}
		else
		{
			ShellExecute(NULL,"open","t.exe",NULL,NULL,SW_SHOWNORMAL);
		}

		while(IsFile("t.exe"))
		{
			Sleep(1);
			DeleteFile("t.exe");
		}
	}


#ifdef	_FOR_KOREA
	if (IsFile("data/checkObject.dat")==FALSE)
	{
		l_bIsConvertObjectFileName	=	TRUE;
		cOBJECT::ConvertFileName();
		l_bIsConvertObjectFileName	=	FALSE;
	}
#endif

	g_eh.addStaticLog("LoadData - LoadCheckObjectInfo");

	if	(!cOBJECT::LoadCheckObjectInfo())
	{
		g_eh.addStaticLog("LoadCheckObjectInfo failed");

		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		return;
	}

#ifdef	_IS_DEV_CLIENT
	cMAP::Init(NULL,TRUE);
#else
	cMAP::Init(NULL);
#endif

	g_eh.addStaticLog("LoadData - g_sprInterface.Load");

	if	(!g_sprInterface.Load("data/interface/game/interface.sd",TRUE,NULL,mMcuStatic))
	{
		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		g_eh.addStaticLog("can not find 'interface.sd' file");

		return;
	}

	if	(!g_sprInterface2.Load("data/interface/game/interface2.sd",TRUE,NULL,mMcuStatic))
	{
		g_eh.addStaticLog("can not find 'interface2.sd' file");

		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		return;
	}

	if	(!g_sprRegionInterface.Load(lpstrRegionInterfaceFile,TRUE,NULL,mMcuStatic))
	{
		g_eh.addStaticLog("can not find %s file",lpstrRegionInterfaceFile);

		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		return;
	}

//	팝업 박스 설정
	{
		cRECT	rect;

		rect.set(g_iScreenWidth/2-140,g_iScreenHeight/20*7,g_iScreenWidth/2+140,g_iScreenHeight/20*11);

		CMsgBoxInfo	info;

		info.reset();

		info.m_wType				=	eMBT_TYPE2;
		info.m_wNormalButtonImage	=	eBMBI_NORMAL;
		info.m_wActiveButtonImage	=	eBMBI_ACTIVE;
		info.m_wPushButtonImage		=	eBMBI_PRESSED;
		info.m_wDisableButtonImage	=	eBMBI_DISABLE;
		info.m_dwBarTextColor		=	0;
		info.m_wIsPutBarTextShadow	=	FALSE;

		info.m_wTitleBarImage		=	eMB_TITLE_PATTERN;
		info.m_wBottomBarImage		=	eMB_TITLE_PATTERN;
		info.m_wTitleBarLeftImage	=	eMB_TITLE_BAR_LEFT_PATTERN;
		info.m_wTitleBarRightImage	=	eMB_TITLE_BAR_RIGHT_PATTERN;
		info.m_wBottomBarLeftImage	=	eMB_TITLE_BAR_LEFT_PATTERN;
		info.m_wBottomBarRightImage	=	eMB_TITLE_BAR_RIGHT_PATTERN;

//	외곽 프레임
		info.m_wLeftTopImage		=	eROUND_BOX_FRAME_LEFT_TOP;
		info.m_wRightTopImage		=	eROUND_BOX_FRAME_RIGHT_TOP;
		info.m_wLeftBottomImage		=	eROUND_BOX_FRAME_LEFT_BOTTOM;
		info.m_wRightBottomImage	=	eROUND_BOX_FRAME_RIGHT_BOTTOM;
		info.m_wTopBorderImage		=	eROUND_BOX_FRAME_TOP_BORDER;
		info.m_wBottomBorderImage	=	eROUND_BOX_FRAME_BOTTOM_BORDER;
		info.m_wLeftBorderImage		=	eROUND_BOX_FRAME_LEFT_BORDER;
		info.m_wRightBorderImage	=	eROUND_BOX_FRAME_RIGHT_BORDER;

		info.m_wIconImportant		=	eMB_IMPORTANT_ICON;
		info.m_wIconWarning			=	eMB_WARNING_ICON;

		info.m_wTitleFontSize		=	16;
		info.m_dwTitleFontStyle		=	FW_BOLD;

		info.m_wTitleTextShadowMargin=	4;
		info.m_wButtonMargin		=	16;
		info.m_wTitleTextVerticalMargin=	0;
		info.m_wBarMenuVerticalMargin=	2;
		info.m_wTextBoxHorizonMargin=	25;
		info.m_wShadowMargin		=	10;
		info.m_wLeftFrameMargin		=	1;
		info.m_wRightFrameMargin	=	1;
		info.m_wTopFrameMargin		=	3;
		info.m_wBottomFrameMargin	=	4;

		strcpy(info.m_strTitleFontName,g_strMessageBoxTitleFontName);

		info.m_wPannelColor			=	0;

		g_msgBox.init(&s_text,&g_sprInterface,&info,&rect);
	}	//	팝업 박스 설정

	g_eh.addStaticLog("LoadData - g_project.load");

	g_project.load("red stone.rpd",NULL);

	g_eh.addStaticLog("LoadData - g_am.close");
	g_am.close();

//	이미지 데이터 초기화
	{
		if	(!g_im.init(NULL,mMcuStatic))	//	각종 데이터 초기화
		{
			s_iDataLoadingStatus	=	eLGSDS_ERROR;
			return;
		}
	}	//	이미지 데이터 초기화

	g_eh.addStaticLog("LoadData - g_sm.init");

	if (!g_sm.initFighterData())
	{
		g_eh.addStaticLog("failed to load fighter skill data");

		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		return;
	}
	if (!g_sm.init("skill.txt"))
	{
		g_eh.addStaticLog("failed to load skill data");

		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		return;
	}
	if (!g_trapSkillManager.init("dev data/scripter/trapSkill.txt"))
	{
		g_eh.addStaticLog("failed to load trap skill data");

		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		return;
	}
#ifdef	_IS_DEV_CLIENT
#ifdef	_FOR_KOREA
	if (g_bIsDevPC)
	{
		if (!g_sm.init("skill.txt",TRUE))
		{
			s_iDataLoadingStatus	=	eLGSDS_ERROR;

			return;
		}

		g_sm.loadData("data/skill.dat");
	}
#endif
#endif
	g_eh.addStaticLog("LoadData - g_titleManager.init");

	if (!g_titleManager.init())
	{
		g_eh.addStaticLog("failed to init title data");

		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		return;
	}
	if (!g_gsManager.init())
	{
		g_eh.addStaticLog("failed to init guild skill");

		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		return;
	}
	if (!g_guildMarkPackManager.init())
	{
		g_eh.addStaticLog("failed to init guild mark pack");

		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		return;
	}

	if	(!g_jm.readDefaultEquipmentData())
	{
		g_eh.addStaticLog("failed to read default equipment");

		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		return;
	}

	if	(!g_pigeonPostShop.init())
	{
		g_eh.addStaticLog("failed to read pigeon post shop info");

		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		return;
	}

	if	(!g_fireCrackerManager.init())
	{
		g_eh.addStaticLog("failed to read fire cracker sound info");

		s_iDataLoadingStatus	=	eLGSDS_ERROR;

		return;
	}

	g_eh.addStaticLog("LoadData - g_guildManager.init");
	g_guildManager.init();	//	길드 마크 정보 초기화

	SetLogFolder();

//	툴팁 초기화
	{
		CTooltipInfo	ttInfo;

		ttInfo.m_iLeftTop				=	eTOOLTIP_LEFT_TOP;
		ttInfo.m_iRightTop				=	eTOOLTIP_RIGHT_TOP;
		ttInfo.m_iLeftBottom			=	eTOOLTIP_LEFT_BOTTOM;
		ttInfo.m_iRightBottom			=	eTOOLTIP_RIGHT_BOTTOM;
		ttInfo.m_iTopBorder				=	eTOOLTIP_TOP_BORDER;
		ttInfo.m_iBottomBorder			=	eTOOLTIP_BOTTOM_BORDER;
		ttInfo.m_iLeftBorder			=	eTOOLTIP_VERTICAL_BORDER;
		ttInfo.m_iRightBorder			=	eTOOLTIP_VERTICAL_BORDER;
		ttInfo.m_lpSprite				=	&g_sprInterface;
		ttInfo.m_lpText					=	&s_text;

		ttInfo.m_dwTextColor			=	WHITE;
		ttInfo.m_wIsTextPutShadow		=	FALSE;
		ttInfo.m_wPannelColor			=	_BLACK;
		ttInfo.m_wIsHalfblendingPannel	=	FALSE;
		ttInfo.m_iLimitWidth			=	300;

//	나머지는 디폴트 수치를 이용
//		ttInfo.m_wHorzSpace		
//		ttInfo.m_wVertSpace		
//		ttInfo.m_iTextLength	
//		ttInfo.m_bIsPutCenter	
		s_ttCommon.init(&ttInfo);

		ttInfo.m_iLeftTop				=	eDIALOG_FRAME_LEFT_TOP;
		ttInfo.m_iRightTop				=	eDIALOG_FRAME_RIGHT_TOP;
		ttInfo.m_iLeftBottom			=	eDIALOG_FRAME_LEFT_BOTTOM;
		ttInfo.m_iRightBottom			=	eDIALOG_FRAME_RIGHT_BOTTOM;
		ttInfo.m_iTopBorder				=	eDIALOG_FRAME_TOP_BORDER;
		ttInfo.m_iBottomBorder			=	eDIALOG_FRAME_BOTTOM_BORDER;
		ttInfo.m_iLeftBorder			=	eDIALOG_FRAME_LEFT_BORDER;
		ttInfo.m_iRightBorder			=	eDIALOG_FRAME_RIGHT_BORDER;

		s_ttDialogFrame.init(&ttInfo);

		ttInfo.m_iLeftTop				=	eROUND_BOX_FRAME_LEFT_TOP;
		ttInfo.m_iRightTop				=	eROUND_BOX_FRAME_RIGHT_TOP;
		ttInfo.m_iLeftBottom			=	eROUND_BOX_FRAME_LEFT_BOTTOM;
		ttInfo.m_iRightBottom			=	eROUND_BOX_FRAME_RIGHT_BOTTOM;
		ttInfo.m_iTopBorder				=	eROUND_BOX_FRAME_TOP_BORDER;
		ttInfo.m_iBottomBorder			=	eROUND_BOX_FRAME_BOTTOM_BORDER;
		ttInfo.m_iLeftBorder			=	eROUND_BOX_FRAME_LEFT_BORDER;
		ttInfo.m_iRightBorder			=	eROUND_BOX_FRAME_RIGHT_BORDER;

		s_ttFrame.init(&ttInfo);
	}

//	팝업 메뉴 초기화
	{
		CPopupMenuInfo	pmInfo;

		pmInfo.m_dwTextColor			=	WHITE;
		pmInfo.m_dwActiveTextColor		=	WHITE;
		pmInfo.m_wIsActiveTextPutShadow	=	TRUE;
		pmInfo.m_wIsTextPutShadow		=	TRUE;

		pmInfo.m_wBarColor				=	RGB24To16(0,100,200);//_WHITE;//RGB24To16(210,123,0);
		pmInfo.m_wPannelColor			=	_BLACK;//RGB24To16(118,81,1);
		pmInfo.m_wIsHalfblendingBar		=	FALSE;
		pmInfo.m_wIsHalfblendingPannel	=	TRUE;
		pmInfo.m_lpttFrame				=	&s_ttCommon;	//	툴팁 프레임만 쓴다.
		pmInfo.m_lpText					=	&s_text;
		pmInfo.m_iTextLength			=	256;

//	나머지는 디폴트 수치를 이용
		pmInfo.m_iCountOfMaxMenu		=	20;
//		pmInfo.m_wLineSpace
		s_pmCommon.init(&pmInfo);
	}

	g_eh.addStaticLog("LoadData - g_itemCountWindow.init");
	g_itemCountWindow.init();	//	아이템 카운트 윈도우 초기화
	g_trapManager.init();
	g_eh.addStaticLog("LoadData - g_hero.reset");
	g_hero.reset();

	int	iRemainStaticMemory	=	s_mcuStatic.getRemainMemorySize();

	g_eh.addStaticLog("remain static memory %d.%.3d mb",iRemainStaticMemory/1024/1024,min((iRemainStaticMemory/1024)%1024,999));

	s_iDataLoadingStatus	=	eLGSDS_COMPLETE;

#ifdef	_IS_DEV_CLIENT
	if	(IsFile("dev data/_devWork"))
	{
		g_guildMarkPackManager.initForDev();
		g_sm.initForDev();
		g_guildMarkPackManager.initForDev();
		g_pigeonPostShop.initForDev();
		g_fireCrackerManager.initForDev();
		g_esm.initForDev();

		MoveFile("dev data/_devWork","dev data/__devWork");
	}
#endif
}

void
ExceptionHandlerLauncher(void *_lpInfo)
{
#ifdef	_FOR_KOREA
	char	strAddress[1024];

	sprintf(strAddress,"http://www.redgem.co.kr/event/C_RPT.aspx?uid=%s",g_hero.m_strId);

	ShellExecute(NULL,"open",strAddress,NULL,NULL,SW_SHOWNORMAL);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	업데이트
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL
cGAME_INIT::UpdateInit()
{
	g_eh.setHeader(_ms("[version %d.%.4d] [date:%s] [rev:%d]",g_dwClientVersion/10000,g_dwClientVersion%10000,g_strBuildDate,g_iRevision));
	g_eh.setLauncher(ExceptionHandlerLauncher);

//	CreateDirectory("report",NULL);

/*
	char	strFileName[256];
#ifdef	_DEBUG
	sprintf(strFileName,"%s/report/red stone(debug).rpt",g_strLogFolder);
#else
	sprintf(strFileName,"%s/report/red stone.rpt",g_strLogFolder);
#endif

	g_eh.setLogFileName(strFileName);
*/	

	g_eh.addStaticLog("dUSE_MCU");
#ifdef	dUSE_MCU


	int		iStaticMemory	=	d1MEGA*300;
	int		iDataMemory		=	d1MEGA*80;

	BOOL	bIsSlimMemory	=	FALSE;

/*	if	(GetTotalMemory()	<	256*1024*1024)
		g_im.setSlimMemoryMode();
	if	(GetTotalMemory()	<	128*1024*1024)
		g_im.setSuperSlimMemoryMode();
*/
	if	(g_config.m_bf1IsSlimMemoryMode)
		g_im.setSlimMemoryMode();

	if	(g_im.isSuperSlimMemoryMode())
	{
		iStaticMemory	=	d1MEGA*80;
		iDataMemory		=	d1MEGA*40;
	}
	else
	if	(g_im.isSlimMemoryMode())
	{
		iStaticMemory	=	d1MEGA*200;
		iDataMemory		=	d1MEGA*40;
	}


	s_mcuStatic.init(iStaticMemory	,"s_mcuStatic");//	고정된 데이터
	s_mcuData.init(iDataMemory	,"s_mcuData");
	s_mcuFieldImage.init(&s_mcuData	,"s_mcuFieldImage",FALSE);//오브젝트,건물,맵,기타등등
	s_mcuBodyImage.init(&s_mcuData	,"s_mcuBodyImage",TRUE);//NPC,몬스터
#endif

	g_eh.addStaticLog("s_sprLoadImage.Load");
	if	(!s_sprLoadImage.Load("data/interface/loadingRedstone.sd",TRUE,NULL,&s_mcuData))
		return	FALSE;
	g_eh.addStaticLog("g_sprLoading.Load");
	if	(!g_sprLoading.Load("data/interface/loading.sd",TRUE,NULL,&s_mcuStatic))
		return	FALSE;
	if	(!g_sprLoading2.Load("data/interface/loading2.sd",TRUE,NULL,&s_mcuStatic))
		return	FALSE;

	g_eh.addStaticLog("BSC");
	BeginSpeedHackChecker();

//	int	x	=	0;int	y	=	100/x;

//	각종 데이터 로딩을 위한 쓰레드
	g_eh.addStaticLog("s_thLoading.Create");
	s_thLoading.Create(s_hWnd,LoadData);
//	LoadData(NULL);
	BookingStep(dSTEP_MAIN);

	return	SAFETY;
}

BOOL
cGAME_INIT::UpdateReady()
{
	BookingStep(dSTEP_MAIN);

	return	TRUE;
}

BOOL
cGAME_INIT::UpdateMain()
{
	if (s_iDataLoadingStatus == eLGSDS_ERROR)
		return	FALSE;
	if (s_iDataLoadingStatus != eLGSDS_COMPLETE)
		return	TRUE;

	s_thLoading.Close();

	g_config.m_dwStaticMemorySize	=	s_mcuStatic.getFilledMemorySize()/1000+1;

	s_timer.setFPS(dSYNC_FPS-2);

	BookingStep(dSTEP_FINISH,dFADE_OUT);

	return	TRUE;
}

BOOL
cGAME_INIT::UpdateFinish()
{
	if (g_canvas.isCompleteFadeOut())	
		BookingStep(dSTEP_CLOSE);

	return	TRUE;
}

BOOL
cGAME_INIT::UpdateClose()
{
	BookingMode(eGAME_LOGO);

	return	TRUE;
}


BOOL
cGAME_INIT::Run()
{
	switch(s_iStep)
	{
		case	dSTEP_INIT					:	return	UpdateInit();
		case	dSTEP_READY					:	return	UpdateReady();
		case	dSTEP_MAIN					:	return	UpdateMain();
		case	dSTEP_FINISH				:	return	UpdateFinish();
		case	dSTEP_CLOSE					:	return	UpdateClose();
	}

	return	FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	화면에 출력
////////////////////////////////////////////////////////////////////////////////////////////////

void
cGAME_INIT::DrawInit()
{
	cDRAW::Clear();
}

void
cGAME_INIT::DrawReady()
{
	cDRAW::Clear();
}

void
cGAME_INIT::DrawMain()
{
	WORD	aFollowLinesVertialPos[]	=
	{
		286,311,273,317,267,323,260,327,257,333,252,337,247,347,244,351,241,354,239,356,
		236,359,233,361,232,365,231,366,226,368,223,371,222,374,220,376,219,378,213,380,
		210,382,208,384,205,386,203,389,202,391,199,392,195,394,194,396,193,397,190,400,
		189,401,187,402,185,404,183,405,181,406,180,407,178,408,177,410,175,412,174,413,
		173,414,171,415,169,416,167,418,165,419,163,420,161,422,160,423,157,424,155,425,
		152,426,151,427,150,428,146,429,145,429,143,430,142,431,141,432,140,433,139,434,
		138,435,136,435,134,436,133,437,132,438,131,438,130,439,129,440,128,441,127,441,
		126,442,124,442,123,443,122,444,121,444,120,445,119,446,118,447,117,448,116,448,
		115,448,114,448,113,449,111,449,110,450,108,451,107,451,106,452,105,452,104,453,
		103,453,102,453,101,454,101,454,100,455,100,455,100,456, 99,457, 98,457, 98,457,
		 98,457, 97,457, 97,458, 97,459, 96,459, 95,460, 95,461, 94,461, 93,461, 93,462,
		 92,462, 91,462, 91,462, 90,463, 90,463, 89,464, 89,464, 88,464, 88,465, 87,466,
		 87,466, 86,466, 85,467, 84,467, 84,467, 84,468, 84,468, 83,468, 82,469, 82,469,
		 81,469, 80,469, 80,469, 80,470, 79,470, 79,470, 79,471, 78,471, 78,471, 77,471,
		 77,471, 76,471, 76,472, 76,472, 75,472, 74,472, 74,472, 74,472, 73,472, 73,472,
		 73,472, 73,472, 73,473, 73,474, 73,474, 73,474, 73,474, 73,474, 73,474, 73,474,
		 73,474, 73,474, 73,474, 74,474, 74,474, 74,474, 74,474, 74,474, 74,474, 74,474,
		 74,474, 74,473, 74,472, 74,472, 74,472, 74,472, 74,472, 74,472, 75,472, 75,472,
		 75,472, 75,472, 75,472, 75,472, 75,472, 75,471, 75,471, 75,471, 76,471, 76,471,
		 76,471, 76,471, 77,471, 77,471, 77,470, 78,470, 78,470, 78,470, 79,469, 79,469,
		 79,469, 79,469, 80,468, 81,468, 82,468, 82,467, 83,467, 84,467, 85,466, 85,466,
		 86,465, 86,465, 86,465, 87,465, 88,465, 88,464, 89,463, 90,463, 90,463, 90,462,
		 91,461, 92,461, 92,461, 93,461, 93,460, 94,460, 95,460, 96,460, 96,460, 97,459,
		 97,458, 98,458, 99,458, 99,457,100,457,100,456,101,456,102,456,103,455,104,455,
		105,454,105,454,106,453,107,452,108,452,109,451,110,451,111,450,112,449,113,449,
		114,448,115,448,116,447,117,447,118,446,119,445,120,445,121,444,122,443,123,443,
		124,442,125,441,126,441,127,440,128,439,129,438,130,438,131,437,132,436,133,435,
		134,434,136,433,138,432,139,431,141,430,142,429,144,428,145,427,147,426,148,425,
		149,424,151,423,152,421,155,420,157,419,158,418,159,417,162,416,163,415,164,413,
		166,412,168,411,169,409,171,408,173,406,176,405,177,404,179,402,181,400,182,399,
		184,397,185,396,188,394,189,393,193,390,195,389,196,388,197,386,200,383,203,380,
		205,377,207,376,210,373,214,371,217,368,219,366,222,363,228,361,232,359,236,355,
		238,351,241,349,252,344,262,341,292,336,293,331,294,326,301,318
	};

	cDRAW::Clear();
	int	dwFilledMemory	=	s_mcuStatic.getFilledMemorySize()/1000;
	int	iFillRate		=	min(dwFilledMemory*1000/g_config.m_dwStaticMemorySize,1000);
	int	iWidth			=	iFillRate*800/1000;

	if (g_bIsTerminalPC)
	{
		s_text.put(10,10,WHITE,"%d％",iFillRate);
/*
		cRECT	rect;

		rect.set(0,0,200,200);
		
		CGamePlay::s_tlChat.init(&rect,&s_text,256,256);	//	초기화
//		CGamePlay::s_tlChat.addColorText(WHITE,"aaaaa a%s","?");
//		CGamePlay::s_tlChat.addColorText(WHITE,"<c:LTGREEN>%s<n> %s","ne1mo","?");
		CGamePlay::s_tlChat.addColorText(WHITE,"<c:LTGREEN>%s<n> %c%c","마창",-108,-93,-36);
*/
		return;
	}

	if (s_sprLoadImage.getSpriteWidth(0) < g_iScreenWidth)
	{
		int i = 0;
		int iMaxi = (g_iScreenWidth-s_sprLoadImage.getSpriteWidth(0))/2;
		while(1)
		{
			s_sprLoadImage.putClipedImage(i, g_iScreenHeight/2, 0, 150);
			if (i > iMaxi)
				break;
			i += 150;
		}
	}

	s_sprLoadImage.Put(g_iCorrectBI_X/2,g_iScreenHeight/2,0);

	if (s_sprLoadImage.getSpriteWidth(0) < g_iScreenWidth)
	{
		int iMaxi = (g_iScreenWidth-s_sprLoadImage.getSpriteWidth(0))/2;
		int i = g_iScreenWidth-iMaxi-80; // 80은 이미지 끝부분이 다른부분과 달라 보정해준 수치

		while(1)
		{
			s_sprLoadImage.putClipedImage(i, g_iScreenHeight/2, 0, 150);
			if (i >= g_iScreenWidth)
				break;
			i += 150;
		}
	}

	int iWidthAdd = (g_iScreenWidth-s_sprLoadImage.getSpriteWidth(0))/2;

	if (s_sprLoadImage.getSpriteWidth(0) < g_iScreenWidth)
	{
		int iMaxi = (g_iScreenWidth-s_sprLoadImage.getSpriteWidth(0))/2;
		if (iWidth < iMaxi && iWidth < g_iCorrectBI_X/2)
			s_sprLoadImage.putClipedImage(0, g_iScreenHeight/2, 1, iWidth);

		if (iWidth >= g_iCorrectBI_X/2)
		{
			int i=0;
			int iViewWidth = 150;
			while(1)
			{
				if (i+150 >= g_iCorrectBI_X/2)
					iViewWidth -= (i+150-g_iCorrectBI_X/2);
				if (i >= g_iCorrectBI_X/2)
					break;
				s_sprLoadImage.putClipedImage(i, g_iScreenHeight/2, 1, iViewWidth);
				i += 150;
			}
			s_sprLoadImage.putClipedImage(g_iCorrectBI_X/2,g_iScreenHeight/2,1,iWidth-g_iCorrectBI_X/2);
		}

		if (iWidth > s_sprLoadImage.getSpriteWidth(0)-30)
		{			
			int iAddposX = 0;
			if (iWidth >= s_sprLoadImage.getSpriteWidth(0)-30)
			{
				// add
				iAddposX = (g_iScreenWidth-iWidth)/30;
				iAddposX *= (iWidth-(s_sprLoadImage.getSpriteWidth(0)-30));
				// add end
			}
			/*
				iAddposX = iWidthAdd*2/3;
			if (iWidth >= s_sprLoadImage.getSpriteWidth(0)-20)
				iAddposX = iWidthAdd*2/2;
			if (iWidth >= s_sprLoadImage.getSpriteWidth(0)-10)
				iAddposX = iWidthAdd*2;
			*/
			
			int i= s_sprLoadImage.getSpriteWidth(0)-30;
			int iViewWidth = 150;
			while(1)
			{
				iViewWidth = 150;
				if (i+150 >= iWidth+iAddposX)
					iViewWidth -= ((i+150)-(iWidth+iAddposX));
				if (i >= s_sprLoadImage.getSpriteWidth(0)-30+iAddposX)
					break;
				s_sprLoadImage.putClipedImage(i, g_iScreenHeight/2, 1, iViewWidth);
				i += 150;
			}
		}
	}	
	else
		s_sprLoadImage.putClipedImage(g_iCorrectBI_X/2,g_iScreenHeight/2,1,iWidth);

//	232,559
	DrawLoadingText();

	int	iFrame	=	(timeGetTime()/60)%15+2;

	if (s_sprLoadImage.getSpriteWidth(0) < g_iScreenWidth)
	{
		iWidthAdd = (g_iScreenWidth-s_sprLoadImage.getSpriteWidth(0))/2;
		if (iWidth < 232+iWidthAdd || iWidth > 559+iWidthAdd )
		{
			int iAddposX = 0;
			if (iWidth >= s_sprLoadImage.getSpriteWidth(0)-30)
			{
				// add
				iAddposX = (g_iScreenWidth-iWidth)/30;
				iAddposX *= (iWidth-(s_sprLoadImage.getSpriteWidth(0)-30));
				// add end
			}
			/*
				iAddposX = iWidthAdd*2/3;
			if (iWidth >= s_sprLoadImage.getSpriteWidth(0)-20)
				iAddposX = iWidthAdd*2/2;
			if (iWidth >= s_sprLoadImage.getSpriteWidth(0)-10)
				iAddposX = iWidthAdd*2;
			*/

			s_sprLoadImage.Put(iWidth+iAddposX, g_iScreenHeight/2+1, iFrame, 100, 100, dPUT_EXTREAM_LIGHT);
		}
		else
		{
			int	iIndex	=	iWidth-(232+iWidthAdd);
			
			s_sprLoadImage.Put(iWidth, aFollowLinesVertialPos[iIndex*2  ]+g_iCorrectBI_Y/2, iFrame,100,100,dPUT_EXTREAM_LIGHT);
			s_sprLoadImage.Put(iWidth, aFollowLinesVertialPos[iIndex*2+1]+g_iCorrectBI_Y/2, iFrame,100,100,dPUT_EXTREAM_LIGHT);
		}
	}
	else
	{
		if	(iWidth	<	232	||	iWidth>	559)
			s_sprLoadImage.Put(iWidth+g_iCorrectBI_X/2,g_iScreenHeight/2+1,iFrame,100,100,dPUT_EXTREAM_LIGHT);
		else
		{
			int	iIndex	=	iWidth-232;

			s_sprLoadImage.Put(iWidth+g_iCorrectBI_X/2,aFollowLinesVertialPos[iIndex*2  ]+g_iCorrectBI_Y/2,iFrame,100,100,dPUT_EXTREAM_LIGHT);
			s_sprLoadImage.Put(iWidth+g_iCorrectBI_X/2,aFollowLinesVertialPos[iIndex*2+1]+g_iCorrectBI_Y/2,iFrame,100,100,dPUT_EXTREAM_LIGHT);
		}
	}

	if	(l_bIsConvertObjectFileName)
		s_text.put(10,580,WHITE,"오브젝트 변환중입니다. 시스템에 따라 1분~10분가량 소요될수 있습니다.");

	if	(l_bIsConvertAnmationFile)
		s_text.put(10,580,WHITE,dMSG_CONVERT_ANIMATION_DATA);
}

void
cGAME_INIT::DrawFinish()
{
	DrawMain();
}

void
cGAME_INIT::DrawClose()
{
	cDRAW::Clear();
}

BOOL
cGAME_INIT::Draw()
{
	switch(s_iStep)
	{
		case	dSTEP_INIT		:DrawInit();break;
		case	dSTEP_READY		:DrawReady();break;
		case	dSTEP_MAIN		:DrawMain();break;
		case	dSTEP_FINISH	:DrawFinish();break;
		case	dSTEP_CLOSE		:DrawClose();break;
	}

	return	TRUE;
}