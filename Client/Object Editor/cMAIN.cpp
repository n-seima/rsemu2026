#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "cMAIN.H"
#include "cANM.H"
#include "cTEXT.H"
#include "cSET.H"
#include "cTRANS.H"

LPCTSTR		lpszAppName			= "OBJECTEDITOR";
LPCTSTR		lpszTitle			= "오브젝트 에디터";
LPCTSTR		lpszToolbar			= "오브젝트 에디터 툴바";
LPCTSTR		regCFG				= "하늘 환경 데이터 저장 화일 v1.0";

cMAIN		_MAIN;
BOOL		bVIRGIN	=	TRUE;

BYTE		keyBuff[256];
BOOL		lbDown	=	FALSE;
BOOL		rbDown	=	FALSE;

cfgData		_cfg;
cSPRITE		sdDATA;
cBMP		*wallPaper	=	NULL;
cANIMATION	WEAPON;
cANIMATION	BASEIMAGE;
cBLOCKINFO	CRASHES;
cBLOCKINFO	BLOCKS;
cBLOCKINFO	OVERLAPBLOCKS;
BOOL		g_isDrawGrid=	TRUE;
int			g_iBlockSize=	1;

/***********************************************************************************************
************************************************************************************************
									초기화 클래스..
************************************************************************************************
***********************************************************************************************/

BOOL		cMAIN::bACTIVEAPP	=	FALSE;

TBBUTTON tbButtons[] = 
{	{ 0 , IDM_NEW		, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0}, 
	{ 0 ,		0		, TBSTATE_ENABLED, TBSTYLE_SEP						, 0, 0L, 0},
	{ 1 , IDM_LOAD		, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0}, 
	{ 10, IDM_LOADBMP	, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0}, 
	{ 2 , IDM_SAVE		, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0},
	{ 0 ,		0		, TBSTATE_ENABLED, TBSTYLE_SEP						, 0, 0L, 0},
	{ 3 , IDM_ZOOMIN	, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0}, 
	{ 4 , IDM_ZOOMOUT	, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0},
	{ 0 ,		0		, TBSTATE_ENABLED, TBSTYLE_SEP						, 0, 0L, 0},
	{ 5 , IDM_SET_SPRITE, TBSTATE_ENABLED|TBSTATE_CHECKED, TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP, 0, 0L, 0}, 
	{ 6 , IDM_SET_FRAME	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP, 0, 0L, 0},
	{ 8 , IDM_SET_CRASH	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP, 0, 0L, 0},
	{19 , IDM_SET_BLOCK	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP, 0, 0L, 0}, 
	{20 , IDM_SET_OVERLAP,TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP, 0, 0L, 0}, 
	{ 7 , IDM_GET_IMAGE	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP, 0, 0L, 0},
	{ 0 ,		0		, TBSTATE_ENABLED, TBSTYLE_SEP						, 0, 0L, 0},
	{ 9 , IDM_MULTI_IMG	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK		, 0, 0L, 0}, 
	{18 , IDM_SPOID		, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0}, 
	{ 0 ,		0		, TBSTATE_ENABLED, TBSTYLE_SEP						, 0, 0L, 0},
	{16 , IDM_SETTOOL	, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0}, 
	{17 , IDM_SETANM	, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0}, 
};

char *strToolbar[]	=
{	"새로운 화일",
	"",
	"불러오기(F3)","이미지 불러오기(F4)","저장하기(F2)",
	"",
	"확대(+)","축소(-)",
	"",
	"스프라이트 편집","에니메이션 편집","충돌박스 설정","블럭 지정","반투명 캐릭터 오버랩 영역","이미지 얻어오기",
	"",
	"여러개의 이미지 잡기","투명컬러 설정",
	"",
	"에디터 설정","에니메이션 설정",
	"",
	"end"
};

#define	dDATA_rbd			0
#define	dDATA_SD			1
#define	dDATA_ID			2
#define	dDATA_PHF			3
#define	dDATA_PEF			4

char *strExt[]	=	{	"rbd","sd","id","phf","pef"};

#define	IDM_CHANGEANM			10000
#define	IDM_CHANGEDIRECT		20000
#define	IDM_BG_ANM				30000

cMAIN::cMAIN()
{	Lock			=	FALSE;
	BGAnm			=	0xffff;
}

cMAIN::~cMAIN()
{	SaveEnviroment();
	Close();
}

BOOL
cMAIN::New()
{
	g_isPlay	=	FALSE;
	cANM::s_iFPS	=	8;

	if (!bVIRGIN)
	{	switch(cMSG::YESNOCANCEL("데이터를 초기화 합니다.","저장하지 않은 데이터가 있습니다.\n\n데이터를 저장하시겠습니까?"))
		{	case	IDYES		:
				if (!SaveAs()) return FALSE;
				break;

			case	IDNO		:
				break;

			case	IDCANCEL	:
				return FALSE;
		}
	}

	strcpy(FName,"");
	cANM::Reset();
	BGAnm	=	0xffff;
	changeMode(dWORK_SET_SPRITE);
	Draw();
	WEAPON.Reset();
	BASEIMAGE.Reset();
	CRASHES.Reset();
	BLOCKS.Reset();
	OVERLAPBLOCKS.Reset();
	CrashBox.Set(0,0,0,0);
	m_editInfo.reset();

	return TRUE;
}

BOOL
cMAIN::Exit()
{	if (!bVIRGIN)
	{	switch(cMSG::YESNOCANCEL("에디터를 종료합니다.","저장하지 않은 데이터가 있습니다.\n\n데이터를 저장하시겠습니까?"))
		{	case	IDYES		:
				if (!SaveAs()) return FALSE;
				return TRUE;

			case	IDNO		:
				return TRUE;

			case	IDCANCEL	:
				return FALSE;

		}
	}

	return TRUE;
}

BOOL
cMAIN::SaveEnviroment()
{	FILE *fp;

	SetCurrentDirectory(curDir);

	fp=fopen("config.cfg","wb");

	if (!fp) return	cMSG::Put("환경 저장 에러!!","'config.cfg' 화일 쓰기 에러입니다.");

	strcpy(_cfg.reg,regCFG);
	_cfg.WIDTH	=	WIDTH	,	_cfg.HEIGHT	=	HEIGHT;

	fwrite(&_cfg,sizeof(_cfg),1,fp);

	strcpy(cANM::FName,_exportFileName(cANM::FName));
	fwrite(cANM::FName,sizeof(cANM::FName),1,fp);
	fwrite(&cANM::BPP,4,1,fp);
	fwrite(&currentExt,4,1,fp);
	fwrite(&_BMPWND.curExt,4,1,fp);

	fwrite(&cSET::_iImageType			,4,1,fp);
	fwrite(&cSET::_iShadowType			,4,1,fp);
	fwrite(&cSET::_iFileType			,4,1,fp);
	fwrite(&cSET::_isIncludeSubfolder	,4,1,fp);
	fwrite(cSET::_strSaveFolder			,512,1,fp);
	fwrite(cSET::_strImageFolder		,512,1,fp);
	fwrite(m_editInfo.m_strFolder		,sizeof(m_editInfo.m_strFolder),1,fp);
	fwrite(&g_isDrawGrid				,4,1,fp);
	

	fclose(fp);

	return TRUE;
}

BOOL
cMAIN::LoadEnviroment()
{	FILE *fp;

	fp=fopen("config.cfg","rb");

	if (!fp)
	{	strcpy(cANM::FName,"");
		return	FALSE;
	}

	fread(&_cfg,sizeof(_cfg),1,fp);

	if (strcmp(_cfg.reg,regCFG)!=0)
	{	fclose(fp);
		return _cfg.setDefault();
	}

	fread(cANM::FName,sizeof(cANM::FName),1,fp);
	fread(&cANM::BPP,4,1,fp);
	fread(&currentExt,4,1,fp);
	fread(&_BMPWND.curExt,4,1,fp);
	if (currentExt<=0)	currentExt	=	1;
	if (currentExt> 2)	currentExt	=	2;
	cANM::LoadAniSet(cANM::FName);

	fread(&cSET::_iImageType			,4,1,fp);
	fread(&cSET::_iShadowType			,4,1,fp);
	fread(&cSET::_iFileType				,4,1,fp);
	fread(&cSET::_isIncludeSubfolder	,4,1,fp);
	fread(cSET::_strSaveFolder			,512,1,fp);
	fread(cSET::_strImageFolder			,512,1,fp);
	fread(m_editInfo.m_strFolder		,sizeof(m_editInfo.m_strFolder),1,fp);
	fread(&g_isDrawGrid					,4,1,fp);

	fclose(fp);

	return TRUE;
}

void
cMAIN::resetBorder()
{	_cfg.border.x	=	width()/32*18;
	_cfg.border.y	=	(height())/64*44;
}

BOOL
cMAIN::Init(HINSTANCE hInst)
{	int	i;
	GetCurrentDirectory(250,curDir);

	cANM::SetAnmType(dANI_CUSTOM);
	currentExt	=	0;

	LoadEnviroment();

	workMode	=	dWORK_SET_SPRITE;
	hINST		=	hInst;
	strcpy(FName,"");

	if (!cTEXT::Init("data\\small.phf","data\\small.pef"))	return FALSE;
	if (!sdDATA.Load("data\\blocks.sd"))	return ERRMSG("can not find 'data\\blocks.sd' File");
	cTEXT::setMulti(FALSE);
	cTEXT::SetFontWidth(14,7,7);
	cTEXT::OutLine(TRUE,0);

	wallPaper	=	new cBMP();
	if (!wallPaper->Load("back.bmp",TRUE))	KILL(wallPaper);


	InitCommonControls();
	WIDTH		=	GetSystemMetrics(SM_CXSCREEN);
	HEIGHT		=	GetSystemMetrics(SM_CYSCREEN);

	if (!cWND::Init(0,lpszAppName,lpszTitle,WS_OVERLAPPEDWINDOW - WS_THICKFRAME,
					0,0,WIDTH,HEIGHT,
					NULL,NULL,hInst,NULL,
					SW_SHOWMAXIMIZED,
					(WNDPROC)WndProc
					)
		)
		return FALSE;

	if (WIDTH != _cfg.WIDTH	||	HEIGHT != _cfg.HEIGHT)	resetBorder();

	cDRAW::Init();
	InitWnds();

	_WORKWND.ResetZero();
	Draw();

	handCursor	=	LoadCursor(hINST,"CURSOR_HAND");
	spoidCursor	=	LoadCursor(hINST,"CURSOR_SPOID");
	nullCursor	=	LoadCursor(hINST,"CURSOR_NULL");
	defaultCursor=	LoadCursor(NULL,IDC_ARROW);

	resetToolbar();

	bVIRGIN		=	TRUE;

	BLOCKS.Reset();
	CRASHES.Reset();
	OVERLAPBLOCKS.Reset();

	for (i=0;i<dMAX_ANM;i++)	ANM[i].ALPHA	=	dPUT_NORMAL;

	TOOLBAR.Resize(wWidth()+100,30);

	return TRUE;
}

void
cMAIN::SetMenu()
{	HMENU	hMENU;

	hMENU	=	GetSubMenu(GetMenu(hWND),1);
	CheckMenuRadioItem(hMENU,IDM_BASE_FORM,IDM_ADD_FORM3,IDM_BASE_FORM+cANM::CurAnm,MF_BYCOMMAND);
}

BOOL
cMAIN::Init(HWND hwnd)
{	cTIMER::Init(20);

	TOOLBAR.Init(hwnd,hINST,(UINT)lpszToolbar,21,tbButtons, TB_BITMAP);
	TOOLBAR.Disable(IDM_ZOOMOUT);

	return TRUE;
}

void
cMAIN::InitWndRect()
{
	mainWnd.Set(hWND);
//	mainWnd.toScreen(hWND);
//	mainWnd.addTop(TOOLBAR.bottom()+3);

	workWnd.left	(0);
	workWnd.top		(TOOLBAR.bottom()+3);
	workWnd.right	(_cfg.border.x);
	workWnd.bottom	(_cfg.border.y-(TOOLBAR.bottom()+3));

	imgWnd.left		(_cfg.border.x+8);
	imgWnd.top   	(TOOLBAR.bottom()+3);
	imgWnd.right	(mainWnd.width()-_cfg.border.x-8);
	imgWnd.bottom	(_cfg.border.y-(TOOLBAR.bottom()+3));

	frmWnd.left		(0);
	frmWnd.top		(_cfg.border.y+8);
	frmWnd.right	(mainWnd.width());
	frmWnd.bottom	(mainWnd.height()-_cfg.border.y - 8);

	bmpWnd.left		(0);
	bmpWnd.top		(TOOLBAR.bottom()+3);
	bmpWnd.right	(_cfg.border.x);
	bmpWnd.bottom	(_cfg.border.y-(TOOLBAR.bottom()+3));
}

void
cMAIN::InitWnds()
{	InitWndRect();

	_WORKWND.Init(hINST,hWND,&workWnd);
	_IMGWND.Init(hINST,hWND,&imgWnd);
	_FRMWND.Init(hINST,hWND,&frmWnd);

	HORZ.Init(hWND,hINST,dSPLIT_HORZ,"HORZSPLIT",0,_cfg.border.y,width(),8);
	VERT.Init(hWND,hINST,dSPLIT_VERT,"VERTSPLIT",_cfg.border.x,TOOLBAR.bottom()+3,8,_cfg.border.y-(TOOLBAR.bottom()+3));
}

void
cMAIN::Update()
{	SetMenu();
	resetToolbar();
}

void
cMAIN::Draw()
{	if (workMode==dWORK_GET_IMAGE)	_BMPWND.Draw();
	else							_WORKWND.Draw();
	_IMGWND.Draw();
	_FRMWND.Draw();
}

void
cMAIN::Move(int dx,int dy,BOOL shift,BOOL control)
{	switch(workMode)
	{	case	dWORK_SET_SPRITE	:
		case	dWORK_SET_BLOCK		:
		case	dWORK_SET_CRASH		:
		case	dWORK_SET_OVERLAP	:
			cIMG::Reform(dx,dy,shift,control);
			break;

		case	dWORK_SET_FRAME		:
			cIMG::Reform(dx,dy,FALSE,control);
			break;
	}
}

void
cMAIN::keyThread()
{	BOOL	shift=FALSE,repaint=FALSE,control=FALSE;
	static	int		pressTime=0,move=1,keyPress=FALSE;
	static	DWORD	lastTime=timeGetTime();

	int		mx=0,my=0;
	int		gab	=	40;

	if(pressTime < 5) gab = 150;

	if(workMode==dWORK_GET_IMAGE) return;

	if (keyBuff[VK_SHIFT	]&0x80	)	shift	=	TRUE;
	if (keyBuff[VK_CONTROL	]&0x80	)
	{
		control	=	TRUE;
		_WORKWND.Draw(TRUE);
	}
	
	//	스프라이트 위치 교정

	if (lastTime+gab > timeGetTime() && keyPress) return;

	lastTime	=	timeGetTime();

	keyPress	=	FALSE;

	if (keyBuff[VK_LEFT		]&0x80	)
	{	keyPress	=	TRUE,repaint	|=	1;
		mx	=	move;
	}
	if (keyBuff[VK_RIGHT	]&0x80	)
	{	keyPress	=	TRUE,repaint	|=	1;
		mx	=	-move;
	}
	if (keyBuff[VK_DOWN		]&0x80	)
	{	keyPress	=	TRUE,repaint	|=	1;
		my	=-move;
	}
	if (keyBuff[VK_UP		]&0x80	)
	{	keyPress	=	TRUE,repaint	|=	1;
		my	=move;
	}

	//	스프라이트 위치교정 가속..
	if	(keyPress)
	{	Move(mx,my,shift,control);
		pressTime++;
			 if (pressTime < 5 )	move=1;
		else if (pressTime < 8  )	move=2;
		else if (pressTime < 10 )	move=3;
		else if (pressTime < 12 )	move=4;
		else if (pressTime < 14 )	move=5;
		else						move=8;
	}
	else	pressTime=0,move=1;

	if (repaint & 1)	_WORKWND.Draw();
}

BOOL
cMAIN::getEditObjectInfo()
{
	char	*folder	=	cFOLDER::Tree(hWND,TRUE,"오브젝트들이 있는 폴더를 선택해 주세요.",m_editInfo.m_strFolder);

	if (!folder)	return	FALSE;

	if (!IsFolder(folder))
		return	ERRMSG("error in  cMAIN::getEditObjectInfo()","다음 폴더를 찾을 수 없습니다.\n\n%s",folder);

	strcpy(m_editInfo.m_strFolder	,folder);

	SetCurrentDirectory(folder);

	cSTRINGS	*rfoFiles	=	cFOLDER::GetFiles("*.rfo");
	cSTRINGS	*rsoFiles	=	cFOLDER::GetFiles("*.rso");

	if (rfoFiles)
	{
		cSTRING	*strFile	=	rfoFiles->Pick(0);

		for (int i=0;i<rfoFiles->Count;i++)
		{
			strcpy(m_editInfo.m_strFiles[m_editInfo.m_iCount++],strFile->String);

			strFile	=	strFile->pNEXT;
		}
	}

	if (rsoFiles)
	{
		cSTRING	*strFile	=	rsoFiles->Pick(0);

		for (int i=0;i<rsoFiles->Count;i++)
		{
			strcpy(m_editInfo.m_strFiles[m_editInfo.m_iCount++],strFile->String);

			strFile	=	strFile->pNEXT;
		}
	}

	KILL(rsoFiles);
	KILL(rfoFiles);


	if (m_editInfo.m_iCount	<=	0)	return	FALSE;

	m_editInfo.m_iEditFile	=	0;

	cANM::LoadSAD(m_editInfo.getEditFileName());

	Draw();
	SetCurrentDirectory(curDir);
	return	TRUE;
}	//	cMAIN::getEditObjectInfo()

BOOL
cMAIN::editPreviousFile()
{
	if (!m_editInfo.isActive())	return	getEditObjectInfo();

	m_editInfo.setFolder();
	cANM::SaveSAD(m_editInfo.getEditFileName());
	SetCurrentDirectory(curDir);

	if (!m_editInfo.previous())	return	FALSE;

	m_editInfo.setFolder();

	cANM::LoadSAD(m_editInfo.getEditFileName());

	SetCurrentDirectory(curDir);
	Draw();

	return	TRUE;
}

BOOL
cMAIN::editNextFile()
{
	if (!m_editInfo.isActive())	return	getEditObjectInfo();

	m_editInfo.setFolder();

	char	*fn	=	m_editInfo.getEditFileName();

	cANM::SaveSAD(fn);
	SetCurrentDirectory(curDir);

	if (!m_editInfo.next())	return	FALSE;

	m_editInfo.setFolder();

	fn	=	m_editInfo.getEditFileName();

	cANM::LoadSAD(fn);

	SetCurrentDirectory(curDir);
	Draw();

	return	TRUE;
}

void
cMAIN::updateKey(WPARAM key)
{	BOOL	repaint	=	FALSE;

	if(workMode	==	dWORK_GET_IMAGE) return;

	switch(key)
	{	case	VK_ADD		:	_WORKWND.ZoomIn()	;repaint|=1;break;
		case	VK_SUBTRACT	:	_WORKWND.ZoomOut()	;repaint|=1;break;
		case	VK_F1		:
			editPreviousFile();
			break;
		case	VK_F2		:
			editNextFile();
			break;

		case	'Z'			:	//if (workMode==dWORK_SET_FRAME)	{	repaint|=7	;cANM::MinusCurrentFrame();}
								//if (workMode==dWORK_SET_SPRITE)	{	repaint|=3	;cANM::MinusCurrentImage();}
								repaint|=3	;cANM::MinusCurrentImage();
								if (workMode==dWORK_SET_FRAME)	if (cIMG::Current	==	0)	cANM::MinusCurrentImage();
								break;

		case	'X'			:	repaint|=3	;cANM::PlusCurrentImage();
								if (workMode==dWORK_SET_FRAME)	if (cIMG::Current	==	0)	cANM::PlusCurrentImage();
								break;

		case	'Q'			:	if (workMode!=dWORK_SET_FRAME) break;
								repaint|=7;cANM::MinusCurrentAnm();		break;
		case	'W'			:	if (workMode!=dWORK_SET_FRAME) break;
								repaint|=7;cANM::PlusCurrentAnm();		break;
		case	'S'			:	if (workMode!=dWORK_SET_FRAME) break;
								if (cANM::AnmType!=	dANI_CUSTOM		)
									if (ANM[cANM::CurAnm].directCount	>	1)
									{	repaint|=7;cANM::PlusCurrentDirect();	}
								break;

		case	'A'			:	if (workMode!=dWORK_SET_FRAME) break;
								if (cANM::AnmType!=	dANI_CUSTOM		)
									if (ANM[cANM::CurAnm].directCount	>	1)
										{	repaint|=7;cANM::MinusCurrentDirect();	}
								break;

		case	'C'			:	if (workMode!=dWORK_SET_FRAME) break;
								cANM::SetBack(cANM::CurFrame);
								Draw();
								break;

		case	'V'			:	if (workMode!=dWORK_SET_FRAME) break;
								cANM::SetFront(cANM::CurFrame);
								Draw();
								break;

//		case	' '			:	if (workMode==dWORK_SET_SPRITE)	changeMode(dWORK_SET_FRAME);
///								else							changeMode(dWORK_SET_SPRITE);
//								break;
	}

	if (repaint & 1)	_WORKWND.Draw();
	if (repaint & 2)	_IMGWND.Draw();
	if (repaint & 4)	_FRMWND.Draw();
	resetToolbar();
}

BOOL
cMAIN::Run()
{	static BOOL resizeHorz=FALSE,resizeVert=FALSE;

	if (bVIRGIN || cIMG::Count==0	)	TOOLBAR.Disable	(IDM_SAVE);
	else								TOOLBAR.Enable	(IDM_SAVE);

	if (VERT.bMoving)	{	resizeVert=TRUE;return TRUE;}
	if (resizeVert)
	{	cRECT rect;
		rect.OutFrame(VERT.hWND);

		int	y			=	_cfg.border.y;
		_cfg.border.MousePos(hWND);
		_cfg.border.y	=	y;

		ResizeWnds(0,0);
		SetFocus(hWND);
	}
	resizeVert	=	FALSE;

	if (HORZ.bMoving)	{	resizeHorz=TRUE;return TRUE;}
	if (resizeHorz)
	{	cRECT rect;
		rect.OutFrame(HORZ.hWND);

		int	x			=	_cfg.border.x;
		_cfg.border.MousePos(hWND);
		_cfg.border.x	=	x;

		ResizeWnds(0,0);
		SetFocus(hWND);
	}
	resizeHorz	=	FALSE;

	GetKeyboardState(keyBuff);

	switch(workMode)
	{	case	dWORK_SET_SPRITE	:
		case	dWORK_SET_FRAME		:
		case	dWORK_SET_CRASH		:
		case	dWORK_SET_BLOCK		:
		case	dWORK_SET_OVERLAP	:
			keyThread();
			_IMGWND.Run();
			_WORKWND.Run();
			_FRMWND.Run();
			break;

		case	dWORK_GET_IMAGE		:
			_BMPWND.Run();
			break;
	}

	return TRUE;
}

void
cMAIN::ResizeWnds(WPARAM wParam, LPARAM lParam)
{	if (!hWND) return;

	if (WIDTH	!=	GetSystemMetrics(SM_CXSCREEN) ||HEIGHT	!=	GetSystemMetrics(SM_CYSCREEN))	resetBorder();

	InitWndRect();

	_WORKWND.Resize(workWnd,FALSE);
	_WORKWND.ResetZero();
	_BMPWND.Resize(bmpWnd,FALSE);
	_IMGWND.Resize(imgWnd,FALSE);
	_FRMWND.Resize(frmWnd,FALSE);

	cRECT	rect;

	rect.Set(hWND);

	if (rect.x2>100)
	{	HORZ.Init(hWND,hINST,dSPLIT_HORZ,"HORZSPLIT",0,_cfg.border.y,width(),8);
		VERT.Init(hWND,hINST,dSPLIT_VERT,"VERTSPLIT",_cfg.border.x,TOOLBAR.bottom()+3,8,_cfg.border.y-TOOLBAR.bottom()-3);
	}

	WIDTH	=	GetSystemMetrics(SM_CXSCREEN);
	HEIGHT	=	GetSystemMetrics(SM_CYSCREEN);

	Draw();

//	InvalidateRect(hWND,NULL,TRUE);
}

void
cMAIN::resetToolbar()
{	TOOLBAR.Check(IDM_MULTI_IMG,_cfg.bMULTI);

	TOOLBAR.Check(IDM_GET_IMAGE	,FALSE);
	TOOLBAR.Check(IDM_SET_BLOCK	,FALSE);
	TOOLBAR.Check(IDM_SET_SPRITE,FALSE);
	TOOLBAR.Check(IDM_SET_FRAME	,FALSE);
	TOOLBAR.Check(IDM_SET_CRASH	,FALSE);
	TOOLBAR.Check(IDM_SET_OVERLAP,FALSE);
	
	if (cIMG::Count<=0)
	{	TOOLBAR.Disable(IDM_SAVE);
		TOOLBAR.Disable(IDM_SET_FRAME);
		TOOLBAR.Disable(IDM_SET_CRASH);
		TOOLBAR.Disable(IDM_SET_BLOCK);
		TOOLBAR.Disable(IDM_SET_OVERLAP);
	}
	else
	{	TOOLBAR.Enable(IDM_SAVE);
		TOOLBAR.Enable(IDM_SET_FRAME);
		TOOLBAR.Enable(IDM_SET_CRASH);
		TOOLBAR.Enable(IDM_SET_BLOCK);
		TOOLBAR.Enable(IDM_SET_OVERLAP);
	}

	switch(workMode)
	{	case	dWORK_SET_SPRITE	:
			TOOLBAR.Disable(IDM_PRE_ANM);
			TOOLBAR.Disable(IDM_NEXT_ANM);
			TOOLBAR.Disable(IDM_PRE_DIRECT);
			TOOLBAR.Disable(IDM_NEXT_DIRECT);
			if (_WORKWND.Zoom!=500)	TOOLBAR.Enable(IDM_ZOOMIN);
			if (_WORKWND.Zoom> 50)	TOOLBAR.Enable(IDM_ZOOMOUT);
			TOOLBAR.Check(IDM_SET_SPRITE,TRUE);
			TOOLBAR.Disable(IDM_SPOID);

			comboDIRECT.DISABLE();
			if (cIMG::Count>0)	comboANM.ENABLE();
			break;

		case	dWORK_SET_FRAME		:
			TOOLBAR.Disable(IDM_SPOID);
			if (cIMG::Count> 0)
			{	if (cANM::CurAnm > 0				)	TOOLBAR.Enable(IDM_PRE_ANM);
				else									TOOLBAR.Disable(IDM_PRE_ANM);
				if (cANM::CurAnm <  cANM::AnmCount-1)	TOOLBAR.Enable(IDM_NEXT_ANM);
				else									TOOLBAR.Disable(IDM_NEXT_ANM);
				if (cANM::AnmType!=	dANI_CUSTOM		)
					if (ANM[cANM::CurAnm].anmtype	!= dANI_CUSTOM)
					{	TOOLBAR.Enable(IDM_PRE_DIRECT);
						TOOLBAR.Enable(IDM_NEXT_DIRECT);
					}
					else
					{	TOOLBAR.Disable(IDM_PRE_DIRECT);
						TOOLBAR.Disable(IDM_NEXT_DIRECT);
					}
			}

			if (_WORKWND.Zoom!=500)	TOOLBAR.Enable(IDM_ZOOMIN);
			if (_WORKWND.Zoom> 50)	TOOLBAR.Enable(IDM_ZOOMOUT);
			TOOLBAR.Check(IDM_SET_FRAME,TRUE);
			if (cIMG::Count<=0)	{comboANM.DISABLE();comboDIRECT.DISABLE();}
			else
			{	comboANM.ENABLE();
				if  (cANM::GetAnmType()==dANI_CUSTOM	)	comboDIRECT.DISABLE();
				else										comboDIRECT.ENABLE();
			}
			break;

		case	dWORK_SET_CRASH		:
			TOOLBAR.Disable(IDM_SPOID);
			TOOLBAR.Disable(IDM_PRE_DIRECT);
			TOOLBAR.Disable(IDM_NEXT_DIRECT);
			if (_WORKWND.Zoom!=500)	TOOLBAR.Enable(IDM_ZOOMIN);
			if (_WORKWND.Zoom> 50)	TOOLBAR.Enable(IDM_ZOOMOUT);
			TOOLBAR.Check(IDM_SET_CRASH,TRUE);
			if (cIMG::Count<=0)	comboANM.DISABLE();
			else				comboANM.ENABLE();
			break;

		case	dWORK_SET_BLOCK		:
			TOOLBAR.Disable(IDM_SPOID);
			TOOLBAR.Disable(IDM_PRE_DIRECT);
			TOOLBAR.Disable(IDM_NEXT_DIRECT);
			if (_WORKWND.Zoom!=500)	TOOLBAR.Enable(IDM_ZOOMIN);
			if (_WORKWND.Zoom> 50)	TOOLBAR.Enable(IDM_ZOOMOUT);
			TOOLBAR.Check(IDM_SET_BLOCK,TRUE);
			if (cIMG::Count<=0)	comboANM.DISABLE();
			else				comboANM.ENABLE();
			break;

		case	dWORK_SET_OVERLAP	:
			TOOLBAR.Disable(IDM_SPOID);
			TOOLBAR.Disable(IDM_PRE_DIRECT);
			TOOLBAR.Disable(IDM_NEXT_DIRECT);
			if (_WORKWND.Zoom!=500)	TOOLBAR.Enable(IDM_ZOOMIN);
			if (_WORKWND.Zoom> 50)	TOOLBAR.Enable(IDM_ZOOMOUT);
			TOOLBAR.Check(IDM_SET_OVERLAP,TRUE);
			if (cIMG::Count<=0)	comboANM.DISABLE();
			else				comboANM.ENABLE();
			break;

		case	dWORK_GET_IMAGE		:
			TOOLBAR.Enable(IDM_SPOID);
			TOOLBAR.Disable(IDM_ZOOMIN);
			TOOLBAR.Disable(IDM_ZOOMOUT);
			TOOLBAR.Check(IDM_GET_IMAGE,TRUE);
			if (cIMG::Count>0)	comboANM.ENABLE();
			break;
	}

	TOOLBAR.SetTip(strToolbar,23);

	SetMenu();
}

void
cMAIN::changeMode(int mode)
{	if	(workMode==mode) return;

	switch(mode)
	{	case	dWORK_SET_FRAME		:
//			cANM::SetCurrentFrame();
			updateKey('X');
			if (workMode==dWORK_GET_IMAGE)	_BMPWND.CLOSEWND();
			break;
		
		case	dWORK_SET_SPRITE	:
		case	dWORK_SET_CRASH		:
		case	dWORK_SET_BLOCK		:
		case	dWORK_SET_OVERLAP	:
			if (workMode==dWORK_GET_IMAGE)	_BMPWND.CLOSEWND();
			break;

		case	dWORK_GET_IMAGE		:
			if (!_BMPWND.Init(_MAIN.hINST,_MAIN.hWND,&_MAIN.bmpWnd))
			{	TOOLBAR.Check(IDM_GET_IMAGE,FALSE);
				TOOLBAR.Check(IDM_SET_SPRITE+workMode-dWORK_SET_SPRITE,TRUE);
				mode	=	workMode;
			}
			break;
	}

	workMode	=	mode;

	resetToolbar();
	
	Draw();
}


BOOL
cMAIN::Save()
{	if (strcmp(FName,"")==0) return SaveAs();

	cANM::SaveSAD(FName);

	Update();

	return TRUE;
}

BOOL
cMAIN::SaveAs()
{	OPENFILENAME	ofn;

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	hWND;
	ofn.lpstrFilter		=	"고정된 오브젝트 데이터 {*.rso}\0*.rso\0자유로운 오브젝트 데이터 {*.rfo}\0*.rfo\0";
	ofn.lpfnHook		=	hookSave;
	ofn.lpstrTitle		=	"오브젝트 저장.";
	ofn.lpstrFile		=	FName;
	ofn.nMaxFile		=	256;
	ofn.lpstrInitialDir	=	_cfg.dataDir[0];
//	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK | OFN_EXPLORER;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK | OFN_EXPLORER | OFN_ENABLEINCLUDENOTIFY;
	ofn.hInstance		=	hINST;
	ofn.nFilterIndex	=	currentExt;
	ofn.lpstrDefExt		=	"rso";

//	ofn.lpTemplateName	=	"SAVE_OBJECT";

	SetCurrentDirectory(_cfg.dataDir[0]);

	if (!GetSaveFileName(&ofn)) return FALSE;

	GetCurrentDirectory(255,_cfg.dataDir[0]);

	char *ext			=	_exportExt(FName);

	if (stricmp(ext,"rso")==0)
	{	currentExt	=	1;
		_cfg.objectType	=	dOBJECT_STATIC;
	}
	else
	{	currentExt	=	2;
		_cfg.objectType	=	dOBJECT_RANDOM;
	}

	return Save();
}

BOOL
cMAIN::Load()
{	OPENFILENAME	ofn;

	SetCurrentDirectory(_cfg.dataDir[0]);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	hWND;
//	ofn.lpstrFilter		=	"오브젝트 데이터 {*.rod}\0*.rod\0\0";
	ofn.lpstrFilter		=	"고정된 오브젝트 데이터 {*.rso}\0*.rso\0자유로운 오브젝트 데이터 {*.rfo}\0*.rfo\0";
	ofn.lpfnHook		=	hookSave;
	ofn.lpstrTitle		=	"오브젝트 불러오기.";
	ofn.lpstrInitialDir	=	_cfg.dataDir[0];
	ofn.lpstrFile		=	FName;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK | OFN_EXPLORER;
	ofn.lpstrDefExt		=	"rso";
	ofn.nFilterIndex	=	currentExt;

	if (!GetOpenFileName(&ofn)) return FALSE;

	GetCurrentDirectory(255,_cfg.dataDir[0]);

	char *ext			=	_exportExt(FName);

	if (stricmp(ext,"rso")==0)
	{	currentExt	=	1;
		_cfg.objectType	=	dOBJECT_STATIC;
	}
	else
	{	currentExt	=	2;
		_cfg.objectType	=	dOBJECT_RANDOM;
	}

	cANM::LoadSAD(FName);

	Update();

	Lock	=	FALSE;

	Draw();

	g_isPlay	=	FALSE;

	return TRUE;
}

LRESULT CALLBACK 
cMAIN::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch( uMsg )
	{	case WM_CREATE		:
			return _MAIN.Init(hwnd);

		case WM_ACTIVATEAPP :
			bACTIVEAPP = wParam;
			if (bACTIVEAPP) SetActiveWindow(hwnd);
			break;

		case WM_SIZE		:
			if (bACTIVEAPP)	_MAIN.ResizeWnds(wParam,lParam);
			break;

		case WM_CLOSE		:
			if (_MAIN.Exit())	PostQuitMessage(0);
			UpdateWindow(GetDesktopWindow());
			break;

		case WM_SETCURSOR	:
			SetCursor(_MAIN.defaultCursor);
			break;

		case WM_KEYDOWN		:
			_MAIN.updateKey(wParam);
			break;

		case WM_COMMAND		:
			switch( LOWORD( wParam ) )
            {	case	IDM_LOAD		:
					_MAIN.Load();
					break;

				case	IDM_NEW			:
					_MAIN.New();
					break;

				case	IDM_SAVEAS		:
					_MAIN.SaveAs();
					break;

				case	IDM_SAVE		:
					_MAIN.Save();
					break;

				case	IDM_MULTI_IMG	:
					_cfg.bMULTI	=	1	-	_cfg.bMULTI;
					_MAIN.resetToolbar();
					break;

				case	IDM_LOADBMP		:
					_BMPWND.OPEN();
					_MAIN.resetToolbar();
					break;
				case	IDM_ZOOMIN		:
					_WORKWND.ZoomIn();
					break;
				case	IDM_ZOOMOUT		:
					_WORKWND.ZoomOut();
					break;

				case	IDM_SET_SPRITE	:
					_MAIN.changeMode(dWORK_SET_SPRITE);
					break;
				case	IDM_SET_FRAME	:
					_MAIN.changeMode(dWORK_SET_FRAME);
					break;
				case	IDM_SET_CRASH	:
					_MAIN.changeMode(dWORK_SET_CRASH);
					break;
				case	IDM_GET_IMAGE	:
					_MAIN.changeMode(dWORK_GET_IMAGE);
					break;
				case	IDM_SET_BLOCK	:
					_MAIN.changeMode(dWORK_SET_BLOCK);
					break;

				case	IDM_SET_OVERLAP	:
					_MAIN.changeMode(dWORK_SET_OVERLAP);
					break;

				case	IDM_SETANM		:
					cSET::SetAnm();
					break;

				case	IDM_SETTOOL		:
					cSET::SetTool();
					break;

				case IDM_SPOID			:
					_BMPWND.SpoidColor();
					break;

				case IDM_PROGRAM		:
					cSET::Program();
					break;

				default					:
					if (LOWORD( wParam ) >= IDM_BASE_FORM && LOWORD( wParam ) <= IDM_ADD_FORM3)
					{	if (cIMG::Count <= 0) break;
						HMENU	hMENU	=	GetSubMenu(GetMenu(_MAIN.hWND),1);
						CheckMenuRadioItem(hMENU,IDM_BASE_FORM,IDM_ADD_FORM3,LOWORD( wParam ),MF_BYCOMMAND);
						cANM::SetCurrentAnm(LOWORD( wParam )-IDM_BASE_FORM);
						_MAIN.changeMode(dWORK_SET_FRAME);
						_MAIN.Draw();
						break;
					}
					break;
			}
			break;

		case WM_SYSCOMMAND	:	//	윈도우 움직이는것과.. 사이즈 바꾸는거 막아 버림.. --;;
		{	WINDOWPLACEMENT	place;
			place.length	=	sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hwnd,&place);

			if ((LOWORD(wParam)&0xfff0) == SC_MOVE	) return FALSE;
			if (place.showCmd	==	SW_SHOWMAXIMIZED)	if ((LOWORD(wParam)&0xfff0) == SC_RESTORE) return FALSE;
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
			break;
		}


		default :
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return( 0L );
}

void
cMAIN::reformFname(HWND hwnd)
{	currentExt	=	SendMessage(GetDlgItem(hwnd,dGET_FILE_EXT),CB_GETCURSEL,0,0 );
	GetDlgItemText(hwnd,dGET_FILE_NAME,FName,sizeof(FName));

	char *ext=_exportExt(FName);
	if (!ext) return;
	_setExt(FName,strExt[currentExt]);

	SetDlgItemText(hwnd,dGET_FILE_NAME,FName);
}

UINT APIENTRY 
cMAIN::hookSave(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)
{	switch(msg)
	{	case WM_COMMAND	:
			switch(wParam)
			{	case IDC_STATIC_OBJECT	:
					_cfg.objectType		=	dOBJECT_STATIC;
					break;

				case IDC_RANDOM_OBJECT	:
					_cfg.objectType		=	dOBJECT_RANDOM;
					break;
			}
			break;


		case WM_NOTIFY		:
			if (wParam==0)
			{	switch(((NMHDR*)lParam)->code)
				{	case	CDN_TYPECHANGE	:
						_MAIN.reformFname(((NMHDR*)lParam)->hwndFrom);
						break;
				
					case	CDN_INITDONE	:
					{	HWND hWnd	=	((NMHDR*)lParam)->hwndFrom;

						RECT rect;
						GetWindowRect(hWnd,&rect);
						int width,height;
						width		=	rect.right  -rect.left+1;
						height		=	rect.bottom -rect.top +1;

						rect.left	=	GetSystemMetrics(SM_CXSCREEN)/2-width/2;
						rect.top	=	GetSystemMetrics(SM_CYSCREEN)/2-height/2;
						rect.right	=	rect.left+width;
						rect.bottom	=	rect.top+height;
						MoveWindow( hWnd, rect.left,rect.top,width,height, TRUE );

						if (_cfg.objectType	==	dOBJECT_STATIC)	CheckDlgButton(hdlg,IDC_STATIC_OBJECT,1);
						else									CheckDlgButton(hdlg,IDC_RANDOM_OBJECT,1);
						break;
					}
				}

			}
			break;

		default:
			return FALSE;

	}
	return FALSE;
}

