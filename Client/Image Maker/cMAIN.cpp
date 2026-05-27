#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "cMAIN.H"
#include "cANM.H"
#include "cTEXT.H"
#include "cSET.H"
#include "cANGLE.H"

LPCTSTR		lpszAppName			= "BADA";
LPCTSTR		lpszTitle			= "이미지 추출기 - 바다";
LPCTSTR		lpszToolbar			= "이미지 추출기 툴바";
LPCTSTR		regCFG				= "하늘 환경 데이터 저장 화일 v1.0";

cMAIN		_MAIN;
BOOL		bVIRGIN	=	TRUE;

char		l_dataFolder[6][256];
BYTE		keyBuff[256];
BOOL		lbDown				=	FALSE;
BOOL		rbDown				=	FALSE;
BOOL		bPLAYANM			=	FALSE;
BOOL		bPUTRELEASEPOINT	=	FALSE;

cfgData		_cfg;
cSPRITE		sdDATA;

cBMP		*wallPaper	=	NULL;

cANIMATION	addDATA[6];

cSTRINGS	*imageFOLDERS		=	NULL;
int			ImageFolder[1024];
int			ImageFolderCount	=	0;
int			CurrentImageFolder	=	0;
int			CurrentData			=	0;
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
};

char *strToolbar[]	=
{	"새로운 화일",
	"",
	"불러오기(F3)","이미지 불러오기(F4)","저장하기(F2)",
	"end"
};

#define	dDATA_SAD			0
#define	dDATA_SD			1
#define	dDATA_ID			2
#define	dDATA_PHF			3
#define	dDATA_PEF			4

char *strExt[]	=	{	"sad","sd","rtd","phf","pef"};

#define	IDM_CHANGEANM			10000
#define	IDM_CHANGEDIRECT		20000
#define	IDM_BG_ANM				30000


cMAIN::cMAIN()
{	Lock			=	TRUE;
	BGAnm			=	0xffff;
	memset(ImageFolder,0,1024*4);
}

cMAIN::~cMAIN()
{	SaveEnviroment();
	Close();
}

BOOL
cMAIN::NewData()
{
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
	BGAnm	=	0xffff;

	cIMG::Reset();

	KILL(imageFOLDERS);
	memset(ImageFolder,0,1024*4);
	ImageFolderCount	=	0;
	CurrentImageFolder	=	0;
	Draw();

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

	fp=fopen("hanul.cfg","wb");

	if (!fp) return	cMSG::Put("환경 저장 에러!!","'hanul.cfg' 화일 쓰기 에러입니다.");

	strcpy(_cfg.reg,regCFG);
	_cfg.WIDTH	=	WIDTH	,	_cfg.HEIGHT	=	HEIGHT;

	fwrite(&_cfg,sizeof(_cfg),1,fp);

	fwrite(&currentExt,4,1,fp);
	fwrite(&_BMPWND.curExt,4,1,fp);
	fwrite(l_dataFolder[0],256,1,fp);
	

	fclose(fp);

	return TRUE;
}

BOOL
cMAIN::LoadEnviroment()
{	FILE *fp;

	strcpy(l_dataFolder[0],"");

	fp=fopen("hanul.cfg","rb");

	if (!fp)
	{	return	FALSE;
	}

	fread(&_cfg,sizeof(_cfg),1,fp);

	if (strcmp(_cfg.reg,regCFG)!=0)
	{	fclose(fp);
		return _cfg.setDefault();
	}

	fread(&currentExt,4,1,fp);
	fread(&_BMPWND.curExt,4,1,fp);
	fread(l_dataFolder[0],256,1,fp);

	fclose(fp);

	return TRUE;
}

void
cMAIN::resetBorder()
{	_cfg.border.x	=	width()/32*18;
	_cfg.border.y	=	height()-TOOLBAR.bottom();
}

BOOL
cMAIN::Init(HINSTANCE hInst)
{	GetCurrentDirectory(250,curDir);

	currentExt	=	0;

	LoadEnviroment();

	workMode	=	dWORK_SET_SPRITE;
	hINST		=	hInst;
	strcpy(FName,"");

	if (!cTEXT::Init("small.phf","small.pef"))	return FALSE;
	if (!sdDATA.Load("data.dat"))				return FALSE;
	
	cTEXT::setMulti(FALSE);
	cTEXT::SetFontWidth(12,6,6);
	cTEXT::OutLine(TRUE,0);

	wallPaper	=	new cBMP();
	if (!wallPaper->Load("back.bmp",TRUE))	KILL(wallPaper);

	InitCommonControls();
	WIDTH		=	GetSystemMetrics(SM_CXSCREEN);
	HEIGHT		=	GetSystemMetrics(SM_CYSCREEN);

	if (!cWND::Init(0,lpszAppName,lpszTitle,WS_OVERLAPPEDWINDOW,
					0,0,WIDTH,HEIGHT,
					NULL,NULL,hInst,NULL,
					SW_SHOWMAXIMIZED,
					(WNDPROC)WndProc
					)
		)
		return FALSE;

	DragAcceptFiles(hWND,TRUE);			//	드래그 화일을 받는다.

//	if (WIDTH != _cfg.WIDTH	||	HEIGHT != _cfg.HEIGHT)	resetBorder();

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


	TOOLBAR.Resize(wWidth()+100,30);

	return TRUE;

}

void
cMAIN::SetMenu()
{
}

BOOL
cMAIN::Init(HWND hwnd)
{//	cTIMER::Init(20);

	TOOLBAR.Init(hwnd,hINST,(UINT)lpszToolbar,5,tbButtons, TB_BITMAP);

	return TRUE;
}

void
cMAIN::InitWndRect()
{
	mainWnd.Set(hWND);

	_cfg.border.y = min(height() - 20,_cfg.border.y);
	_cfg.border.x = min(width() - 20,_cfg.border.x);

	_cfg.border.y = max(40,_cfg.border.y);
	_cfg.border.x = max(40,_cfg.border.x);

	workWnd.left	(0);
	workWnd.top		(TOOLBAR.bottom()+3);
	workWnd.right	(_cfg.border.x);
	workWnd.bottom	(mainWnd.height()-(TOOLBAR.bottom()+3));

	imgWnd.left		(_cfg.border.x+8);
	imgWnd.top   	(TOOLBAR.bottom()+3);
	imgWnd.right	(mainWnd.width()-_cfg.border.x-8);
	imgWnd.bottom	(mainWnd.height()-(TOOLBAR.bottom()+3));

	bmpWnd.left		(0);
	bmpWnd.top		(TOOLBAR.bottom()+3);
	bmpWnd.right	(_cfg.border.x);
	bmpWnd.bottom	(mainWnd.height()-(TOOLBAR.bottom()+3));
}

void
cMAIN::InitWnds()
{	InitWndRect();

	_WORKWND.Init(hINST,hWND,&workWnd);
	_IMGWND.Init(hINST,hWND,&imgWnd);

	VERT.Init(hWND,hINST,dSPLIT_VERT,"VERTSPLIT",_cfg.border.x,TOOLBAR.bottom()+3,8,mainWnd.height()-(TOOLBAR.bottom()+3));
}

void
cMAIN::Update()
{	SetMenu();
	resetToolbar();
}

void
cMAIN::Draw()
{
	if (workMode==dWORK_GET_IMAGE)	_BMPWND.Draw();
	else							_WORKWND.Draw();
	_IMGWND.Draw();
}

void
cMAIN::Move(int dx,int dy,BOOL shift,BOOL control)
{
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
	if (keyBuff[VK_CONTROL	]&0x80	)	control	=	TRUE;
	
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

void
cMAIN::updateKey(WPARAM key)
{	BOOL	repaint	=	FALSE;

	switch(key)
	{
		case	'Z'			:	repaint|=3;
								cIMG::SetCURRENT(cIMG::Current-1);	
								break;

		case	'X'			:	repaint|=3;
								cIMG::SetCURRENT(cIMG::Current+1);	
								break;

		case	VK_ADD		:	_WORKWND.Zoom	=	min(_WORKWND.Zoom+100,1000);repaint|=1;break;
		case	VK_SUBTRACT	:	_WORKWND.Zoom	=	max(_WORKWND.Zoom-100,100);repaint|=1;break;
	}

	if (repaint & 1)	_WORKWND.Draw();
	if (repaint & 2)	_IMGWND.Draw();
	resetToolbar();
}

BOOL
cMAIN::Run()
{
	static BOOL resizeHorz=FALSE,resizeVert=FALSE;
	if (!bACTIVEAPP) return TRUE;

	if (bVIRGIN || cIMG::Count==0	)	TOOLBAR.Disable	(IDM_SAVE);
	else								TOOLBAR.Enable	(IDM_SAVE);

	if (VERT.bMoving)	{	resizeVert=TRUE;return TRUE;}
	if (resizeVert)
	{	int	y			=	_cfg.border.y;
		_cfg.border.MousePos(hWND);
		_cfg.border.y	=	y;

		ResizeWnds(0,0);
		SetFocus(hWND);
	}
	resizeVert	=	FALSE;

	GetKeyboardState(keyBuff);

	switch(workMode)
	{	case	dWORK_SET_SPRITE	:
		case	dWORK_SET_FRAME		:
		case	dWORK_SET_CRASH		:
		case	dWORK_SET_SELECT	:
			keyThread();
			_IMGWND.Run();
			_WORKWND.Run();
			break;

		case	dWORK_GET_IMAGE		:
			_BMPWND.Run();
			break;
	}

	return TRUE;
}

void
cMAIN::ResizeWnds(WPARAM wParam, LPARAM lParam)
{	if (!hWND || !bACTIVEAPP) return;

//	if (WIDTH	!=	GetSystemMetrics(SM_CXSCREEN) ||HEIGHT	!=	GetSystemMetrics(SM_CYSCREEN))	resetBorder();

	InitWndRect();

	_WORKWND.Resize(workWnd,FALSE);
	_WORKWND.ResetZero();
	_BMPWND.Resize(bmpWnd,FALSE);
	_IMGWND.Resize(imgWnd,FALSE);

	TOOLBAR.Resize(wWidth()+100,30);

	cRECT	rect;

	rect.Set(hWND);

	if (rect.x2>100)
		VERT.Init(hWND,hINST,dSPLIT_VERT,"VERTSPLIT",_cfg.border.x,TOOLBAR.bottom()+3,8,mainWnd.height()-TOOLBAR.bottom()-3);

	WIDTH	=	GetSystemMetrics(SM_CXSCREEN);
	HEIGHT	=	GetSystemMetrics(SM_CYSCREEN);

	Draw();

//	InvalidateRect(hWND,NULL,TRUE);
}

void
cMAIN::resetToolbar()
{
	SetMenu();
}

void
cMAIN::changeMode(int mode)
{
}

BOOL
cMAIN::Save()
{
	if (strcmp(FName,"")==0) return SaveAs();
	
	cANM::SaveSmi(FName);

	Update();

	return TRUE;
}

BOOL
cMAIN::SaveAs()
{
	if (cIMG::Count<=0)
	{	return cMSG::Error("데이터 저장 에러!!","저장할 이미지가 없습니다.");
	}

	OPENFILENAME	ofn;

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	hWND;
	ofn.lpstrFilter		=	"이미지 데이터 {*.smi}\0*.smi\0\0";
	ofn.lpfnHook		=	hookSave;
	ofn.lpstrTitle		=	"데이터 저장.";
	ofn.lpstrFile		=	FName;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK | OFN_EXPLORER;
	ofn.lpTemplateName	=	"QUICK_OPEN";
	ofn.lpstrDefExt		=	"smi";
	ofn.nFilterIndex	=	currentExt+1;
	ofn.lpstrInitialDir	=	_cfg.dataDir[currentExt];

	if (!GetSaveFileName(&ofn)) return FALSE;

	GetCurrentDirectory(255,_cfg.dataDir[currentExt]);

	return Save();
}

BOOL
cMAIN::exportImages()
{
	int		i;
	char	strFolderName[512];
	char	strFolder[512];

	if (cIMG::Count<=0)
		return cMSG::Error("이미지 익스포트 에러!!","로딩된 이미지가 없습니다.");

	SetCurrentDirectory("c:\\");
	sprintf(strFolderName,"c:\\__hanul export image\\%s",_exportName(FName));

	cFOLDER::Create(strFolderName,TRUE);

	GetCurrentDirectory(512,strFolder);

	cIMG	*lpImg		=	cIMG::pHEAD->pNEXT;

	for (i=0;i<cIMG::Count;i++)
	{
		int		iWidth	=	lpImg->xs;
		int		iHeight	=	lpImg->ys;

		WORD	*pBuffer;

		pBuffer	=	new WORD [iWidth*iHeight+2];

		cDRAW::Active(pBuffer+2,iWidth,iHeight);
		pBuffer[0]	=	iWidth;
		pBuffer[1]	=	iHeight;

		cDRAW::PutImage(0,0,lpImg->m_p16Image);
		cTGA::Save(_ms("%.4d.tga",i),pBuffer);

		pKILL(pBuffer);

		lpImg	=	lpImg->pNEXT;
	}

	SetCurrentDirectory(_MAIN.curDir);

	cMSG::Put("이미지 추출작업","완료");

	return	TRUE;
}

BOOL
cMAIN::Load()
{
	if (!bVIRGIN)
	{	switch(cMSG::YESNOCANCEL("저장하지 않은 데이터가 있습니다.","저장하지 않은 데이터가 있습니다.\n\n데이터를 저장하시겠습니까?"))
		{	case	IDYES		:
				if (!SaveAs()) return FALSE;
				break;

			case	IDNO		:
				break;

			case	IDCANCEL	:
				return FALSE;
		}
	}

	OPENFILENAME	ofn;

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	hWND;
	ofn.lpstrFilter		=	"이미지 데이터 {*.smi}\0*.smi\0\0";
	ofn.lpfnHook		=	hookSave;
	ofn.lpstrTitle		=	"데이터 불러오기";
	ofn.lpstrFile		=	FName;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK | OFN_EXPLORER;
	ofn.lpstrDefExt		=	"smi";
	ofn.nFilterIndex	=	currentExt+1;
	ofn.lpstrInitialDir	=	_cfg.dataDir[currentExt];

	if (!GetOpenFileName(&ofn)) return FALSE;

	GetCurrentDirectory(255,_cfg.dataDir[currentExt]);

	cANM::LoadSmi(FName);
//	cANM::LoadSAD(FName);

	Update();

	Lock	=	TRUE;

	Draw();

	return TRUE;
}

void
cMAIN::GetDropFiles(HDROP handle)
{	char	file[512];

	int		fileCount	=	DragQueryFile(handle,0xFFFFFFFF,NULL,0),i;
	int		iImageCount	=	0;

	if (fileCount == 0 ) return;

	for (i=0;i<fileCount;i++)
	{	
		DragQueryFile(handle,i,file,sizeof(file));
		if (_BMPWND.loadBmp(file))	iImageCount++;
	}

	cMSG::Put("이미지 가공 완료!!","총 %d개의 화일을 열어 %d개의 이미지를 생성 했습니다.",fileCount,iImageCount);

	Draw();
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

		case WM_DROPFILES					:
			_MAIN.GetDropFiles((HDROP)wParam);
			break;

		case WM_SIZE		:
			if (bACTIVEAPP)	_MAIN.ResizeWnds(wParam,lParam);
			break;

		case WM_CLOSE		:
			if (_MAIN.Exit())	PostQuitMessage(0);
			UpdateWindow(GetDesktopWindow());
			break;

//		case WM_SETCURSOR	:
//			SetCursor(_MAIN.defaultCursor);
//			break;

		case WM_KEYDOWN		:
			_MAIN.updateKey(wParam);
			break;

		case WM_COMMAND		:
			switch( LOWORD( wParam ) )
            {	
				case	IDM_LOAD			:
					_MAIN.Load();
					break;

				case	IDM_NEW				:
					_MAIN.NewData();
					break;

				case	IDM_SAVEAS			:
					_MAIN.SaveAs();
					break;

				case	IDM_SAVE			:
					_MAIN.Save();
					break;

				case	IDM_EXPORT_IMAGE	:
					_MAIN.exportImages();
					break;

				case	IDM_LOADBMP			:
					bGETSHADOW	=	FALSE;
					g_isGetImageAndShadow	=	FALSE;
					_BMPWND.OPEN();
					_MAIN.resetToolbar();
					break;

				case	IDM_SET_SPRITE		:
					_MAIN.changeMode(dWORK_SET_SPRITE);
					break;

				default						:
					break;
			}
			break;

		case WM_SYSCOMMAND	:	//	윈도우 움직이는것과.. 사이즈 바꾸는거 막아 버림.. --;;
		{
			WINDOWPLACEMENT	place;
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
	{	case WM_INITDIALOG	:
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