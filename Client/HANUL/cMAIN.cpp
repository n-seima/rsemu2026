#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "cMAIN.H"
#include "cANM.H"
#include "cTEXT.H"
#include "cSET.H"
#include "cANGLE.H"

char	*g_strJobName[]	=	
{
	"knight","warrior","wizard","werewolf","priest","fallen_angel","rogue","fighter",
	"magic_lancer","magic_archer","tamer","summoner","princess","magical_girl","necromancer","devil",
	"end"
};

class	cSlideShow
{
public:
	char				m_strFolder[512];
	char				m_strFiles[1000][64];
	int					m_iCount,m_iEditFile;

	cSlideShow()		{reset();}
	inline	void		reset()
	{
		m_iCount	=	0;
		m_iEditFile	=	0;	
	}

	inline	BOOL		isActive()
	{
		if (m_iCount	>	0)	return	TRUE;

		return	FALSE;
	}

	inline	BOOL		next()
	{
		if (m_iEditFile>=	m_iCount-1)	return	FALSE;

		m_iEditFile++;

		return	TRUE;
	}
	inline	BOOL		previous()
	{
		if (m_iEditFile<=	0		)	return	FALSE;

		m_iEditFile--;
		return	TRUE;
	}

	inline	char*		getEditFileName()
	{
		if (m_iCount	<=	0)	return	NULL;

		return	m_strFiles[m_iEditFile];
	}
	inline	void		setFolder()
	{
		SetCurrentDirectory(m_strFolder);
	}
};

cSlideShow		l_slideShow;


LPCTSTR		lpszAppName			= "HANUL";
LPCTSTR		lpszTitle			= "스프라이트 에디터 - 하늘 -";
LPCTSTR		lpszToolbar			= "스프라이트 에디터 - 하늘 - 툴바";
LPCTSTR		regCFG				= "하늘 환경 데이터 저장 화일 v1.0";

cMAIN		_MAIN;
BOOL		bVIRGIN					=	TRUE;
BOOL		g_bIsVirginLayerData	=	TRUE;

char		l_dataFolder[6][256];
char		l_strPartsDataFolder[eBodyType_count][256];
BYTE		keyBuff[256];
BOOL		lbDown				=	FALSE;
BOOL		rbDown				=	FALSE;
BOOL		bPLAYANM			=	FALSE;
BOOL		bPUTRELEASEPOINT	=	FALSE;
int			g_iSelectParts		=	0;

cfgData		_cfg;
cSPRITE		sdDATA;

cBMP		*wallPaper			=	NULL;
int			g_iLoadPart			=	ePart_none;

cANIMATION	addDATA[6];
cANIMATION	g_aPartsData[eBodyType_count];

cSTRINGS	*imageFOLDERS		=	NULL;
int			ImageFolder[1024];
int			ImageFolderCount	=	0;
int			CurrentImageFolder	=	0;
WORD		g_wAnmDataType		=	0;
WORD		g_wPartsType		=	0;
WORD		g_wPlayerJob		=	0;

BYTE		g_abDrawLayerOrder[32][8][256][eBodyType_count];


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
	{ 11, IDM_LOADSHADOW, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0}, 
	{ 2 , IDM_SAVE		, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0},
	{ 0 ,		0		, TBSTATE_ENABLED, TBSTYLE_SEP						, 0, 0L, 0},
	{ 3 , IDM_ZOOMIN	, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0}, 
	{ 4 , IDM_ZOOMOUT	, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0},
	{ 0 ,		0		, TBSTATE_ENABLED, TBSTYLE_SEP						, 0, 0L, 0},
	{ 5 , IDM_SET_SPRITE, TBSTATE_ENABLED|TBSTATE_CHECKED, TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP, 0, 0L, 0}, 
	{ 6 , IDM_SET_FRAME	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP, 0, 0L, 0},
	{ 8 , IDM_SET_CRASH	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP, 0, 0L, 0},
	{21 , IDM_SET_SELECT, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP, 0, 0L, 0},
	{ 7 , IDM_GET_IMAGE	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP, 0, 0L, 0},
	{ 0 ,		0		, TBSTATE_ENABLED, TBSTYLE_SEP						, 0, 0L, 0},
	{12 , IDM_PRE_ANM	, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0},
	{13 , IDM_NEXT_ANM	, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0},
	{14 , IDM_PRE_DIRECT, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0},
	{15 ,IDM_NEXT_DIRECT, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0},
	{ 0 ,		0		, TBSTATE_ENABLED, TBSTYLE_SEP						, 0, 0L, 0},
	{ 9 , IDM_MULTI_IMG	, TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK		, 0, 0L, 0}, 
	{18 , IDM_SPOID		, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0}, 
	{ 0 ,		0		, TBSTATE_ENABLED, TBSTYLE_SEP						, 0, 0L, 0},
	{16 , IDM_SETTOOL	, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0}, 
	{17 , IDM_SETANM	, TBSTATE_ENABLED, TBSTYLE_BUTTON					, 0, 0L, 0}, 

};

char *strToolbar[]	=
{ "New Whale",
	"",
	"Load (F3)", "Load Image (F4)", "Load Shadow (F5)", "Save (F2)",
	"",
	"Zoom in (+)", "Zoom out (-)",
	"",
	"Edit Sprite", "Edit Animation", "Set Collision Box/Damage Box", "Set Selection Box", "Get Image",
	"",
	"Previous Animation (Q)", "Next Animation (W)", "Rotate Counterclockwise (A)", "Rotate Clockwise (S)",
	"",
	"Grab multiple images", "Set transparent color",
	"",
	"Editor Settings", "Animation Settings",
	"",
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
{	
	Lock			=	TRUE;
	BGAnm			=	0xffff;
	memset(ImageFolder,0,1024*4);

	resetPlayerLayerData();
}

cMAIN::~cMAIN()
{	SaveEnviroment();
	Close();
}

BOOL
cMAIN::New()
{	if (!bVIRGIN)
	{	switch(cMSG::YESNOCANCEL("Initializing data.","You have unsaved data.\n\nDo you want to save the data?"))
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

	addDATA[0].Close();
	addDATA[1].Close();
	addDATA[2].Close();
	addDATA[3].Close();
	addDATA[4].Close();
	addDATA[5].Close();

	for (int i=0;i<eBodyType_count;i++)
		g_aPartsData[i].Close();

	resetPlayerLayerData();

	KILL(imageFOLDERS);
	memset(ImageFolder,0,1024*4);
	ImageFolderCount	=	0;
	CurrentImageFolder	=	0;
	g_iSelectParts		=	0;
	g_wAnmDataType		=	0;
	g_wPartsType		=	0;
	g_wPlayerJob		=	0;
	bVIRGIN				=	TRUE;

	return TRUE;
}

BOOL
cMAIN::Exit()
{	if (!bVIRGIN)
	{	switch(cMSG::YESNOCANCEL("Exiting the editor.","You have unsaved data.\n\nDo you want to save the data?"))
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

	if (!fp) return	cMSG::Put("Environment save error!!","'hanul.cfg' is a Japanese write error.");

	strcpy(_cfg.reg,regCFG);
	_cfg.WIDTH	=	WIDTH	,	_cfg.HEIGHT	=	HEIGHT;

	fwrite(&_cfg,sizeof(_cfg),1,fp);

	strcpy(cANM::FName,_exportFileName(cANM::FName));
	fwrite(cANM::FName,sizeof(cANM::FName),1,fp);
	fwrite(&cANM::BPP,4,1,fp);
	fwrite(&currentExt,4,1,fp);
	fwrite(&_BMPWND.curExt,4,1,fp);
	fwrite(l_dataFolder[0],256,1,fp);
	fwrite(l_dataFolder[1],256,1,fp);
	fwrite(l_dataFolder[2],256,1,fp);
	fwrite(l_dataFolder[3],256,1,fp);
	fwrite(l_dataFolder[4],256,1,fp);
	fwrite(l_dataFolder[5],256,1,fp);

	{
		int	iCheckSum	=	0x12345678;

		fwrite(&iCheckSum,sizeof(int),1,fp);
		fwrite(l_strPartsDataFolder,sizeof(l_strPartsDataFolder),1,fp);
	}
	fclose(fp);

	return TRUE;
}

BOOL
cMAIN::LoadEnviroment()
{	FILE *fp;

	strcpy(l_dataFolder[0],"");
	strcpy(l_dataFolder[1],"");
	strcpy(l_dataFolder[2],"");
	strcpy(l_dataFolder[3],"");
	strcpy(l_dataFolder[4],"");
	strcpy(l_dataFolder[5],"");

	fp=fopen("hanul.cfg","rb");

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
	cANM::LoadAniSet(cANM::FName);
	fread(l_dataFolder[0],256,1,fp);
	fread(l_dataFolder[1],256,1,fp);
	fread(l_dataFolder[2],256,1,fp);
	fread(l_dataFolder[3],256,1,fp);
	fread(l_dataFolder[4],256,1,fp);
	fread(l_dataFolder[5],256,1,fp);

	{
		int	iCheckSum;

		fread(&iCheckSum,sizeof(int),1,fp);

		if	(iCheckSum	==	0x12345678)
			fread(l_strPartsDataFolder,sizeof(l_strPartsDataFolder),1,fp);
	}

	fclose(fp);

	return TRUE;
}

void
cMAIN::resetBorder()
{	_cfg.border.x	=	width()/32*18;
	_cfg.border.y	=	(height()-TOOLBAR.bottom())/64*44;
}

void
Draw8BitSprite(int x,int y,BYTE *sprite,BYTE *_lpScreen,int _iWidth)
{
	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j;
	int   count,bufferPoint=8;
	int   size;

	xs=*(short *)(sprite  );
	ys=*(short *)(sprite+2);
	x-=*(short *)(sprite+4);	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;

	int addr=y*_iWidth;

	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)	*(_lpScreen+addr+xPoint+j)	=	*(sprite+bufferPoint+j);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr	+=	_iWidth;
	}
}

void
Draw16BitSprite(int x,int y,WORD *sprite,WORD *_lpScreen,int _iWidth)
{
	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i;
	int   count,bufferPoint=8;
	int   size;

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x	-=	xs/2;	//	위치 교정..
	y	-=	ys/2;	//	위치 교정..

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;

	int addr=y*_iWidth;

	for(;y1<y2;y1++)
	{
		count	=	sprite[bufferPoint++];

		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			int	beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (size > beginX )
				memcpy(_lpScreen+addr+xPoint+beginX,sprite+bufferPoint+beginX,(size-beginX)<<1);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=_iWidth;
	}

}

BOOL
cMAIN::exportImages()
{
	int i;

	if	(cIMG::Count<=0)
		return cMSG::Error("이미지 익스포트 에러!!","로딩된 이미지가 없습니다.");

	if	(cANM::BPP	==	16)
	{
		WORD	*pBuffer;

		pBuffer		=	new WORD [800*600+2];

		pBuffer[0]	=	800;
		pBuffer[1]	=	600;

		cIMG	*lpImg		=	cIMG::pHEAD->pNEXT;

		SetCurrentDirectory("c:\\");
		char	strFolderName[512];
		sprintf(strFolderName,"c:\\__hanul export image\\%s",_exportName(FName));

		cFOLDER::Create(strFolderName,TRUE);

		g_bTransColorIndex	=	2;

		char	strFolder[512];

		GetCurrentDirectory(512,strFolder);

		for (i=0;i<cIMG::Count;i++)
		{
			memset(pBuffer+2,(WORD)0,800*600);

			Draw16BitSprite(400,300,lpImg->pSPRITE,pBuffer+2,800);

			lpImg	=	lpImg->pNEXT;

			cTGA::Save(_ms("%.4d.tga",i),pBuffer,TRUE);
		}
	}
	else
	{
		BYTE	*pBuffer;

		pBuffer		=	new BYTE [800*600];

		cIMG	*lpImg		=	cIMG::pHEAD->pNEXT;

		SetCurrentDirectory("c:\\");
		char	strFolderName[512];
		sprintf(strFolderName,"c:\\__hanul export image\\%s",_exportName(FName));

		cFOLDER::Create(strFolderName,TRUE);

		g_bTransColorIndex	=	2;

		BYTE	aPalette[256*3];

		for (i=0;i<256;i++)
		{
			aPalette[i*3+0]	=	(cANM::PLT[i]>>10)*8;
			aPalette[i*3+1]	=	((cANM::PLT[i]>>5) & 31)*8;
			aPalette[i*3+2]	=	(cANM::PLT[i]&31)*8;
		}

		char	strFolder[512];

		GetCurrentDirectory(512,strFolder);

		for (i=0;i<cIMG::Count;i++)
		{
			memset(pBuffer,g_bTransColorIndex,800*600);

			Draw8BitSprite(400,300,lpImg->p8SPRITE,pBuffer,800);
	//		Draw16BitSprite

			lpImg	=	lpImg->pNEXT;

			cPCX::Save(_ms("%.4d.pcx",i),pBuffer,800,600,aPalette);
		}
	}


	SetCurrentDirectory(_MAIN.curDir);

	cMSG::Put("Image Extraction Job","Completed");

	return	TRUE;
}

BOOL
cMAIN::Init(HINSTANCE hInst)
{	
	GetCurrentDirectory(250,curDir);

	cANM::SetAnmType(dANI_CUSTOM);
	currentExt	=	0;

	LoadEnviroment();

	workMode	=	dWORK_SET_SPRITE;
	hINST		=	hInst;
	strcpy(FName,"");

	if	(!cTEXT::Init("small.phf","small.pef"))
		return FALSE;
	if	(!sdDATA.Load("data.dat"))
		return FALSE;
	
	cTEXT::setMulti(FALSE);
	cTEXT::SetFontWidth(12,6,6);
	cTEXT::OutLine(TRUE,0);

	wallPaper	=	new cBMP();

	if	(!wallPaper->Load("back.bmp",TRUE))
		KILL(wallPaper);

	InitCommonControls();

	WIDTH		=	GetSystemMetrics(SM_CXSCREEN);
	HEIGHT		=	GetSystemMetrics(SM_CYSCREEN);

	if	(!cWND::Init(0,lpszAppName,lpszTitle,WS_OVERLAPPEDWINDOW| WS_CLIPCHILDREN |WS_CLIPSIBLINGS,
					0,0,WIDTH,HEIGHT,
					NULL,NULL,hInst,NULL,
					SW_SHOWMAXIMIZED,
					(WNDPROC)WndProc
					)
		)
		return FALSE;

	if	(WIDTH != _cfg.WIDTH	||	HEIGHT != _cfg.HEIGHT)
		resetBorder();

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

	WORD	data[5];

	cIMG::Add(data,0,0,0,0);

	imageFOLDERS	=	new cSTRINGS();
	imageFOLDERS->Add("No Folder");
	ImageFolder[0]	=	0;
	ImageFolder[1]	=	1;
	ImageFolder[2]	=	0;
	ImageFolder[3]	=	1;
	ImageFolderCount=	1;
	CurrentImageFolder	=	0;

	return TRUE;
}

void
cMAIN::SetMenu()
{
	HMENU	hMENU	=	GetMenu(hWND);

	for (int i=0;i<17;i++)
		CheckMenuItem(hMENU,ID_JOB_NONE+i,MF_BYCOMMAND|MF_UNCHECKED);

	CheckMenuItem(hMENU,ID_JOB_NONE+g_wPlayerJob,MF_BYCOMMAND|MF_CHECKED);
}

BOOL
cMAIN::Init(HWND hwnd)
{//	cTIMER::Init(20);

	TOOLBAR.Init(hwnd,hINST,(UINT)lpszToolbar,26,tbButtons, TB_BITMAP);
	TOOLBAR.Disable(IDM_ZOOMOUT);

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
{	
	if	(workMode==dWORK_GET_IMAGE)
		_BMPWND.Draw();
	else
		_WORKWND.Draw();
	
	_IMGWND.Draw();
	_FRMWND.Draw();
}

void
cMAIN::Move(int dx,int dy,BOOL shift,BOOL control,BOOL _bIsPressedAlt)
{	
	switch(workMode)
	{	
		case	dWORK_SET_SPRITE	:
			cIMG::Reform(dx,dy,shift,control);
			break;

		case	dWORK_SET_FRAME		:
			if	(ANM[cANM::CurAnm].bSaveRefitPos)	
				cANM::MovePos(dx,dy,shift);
			else
				cIMG::Reform(dx,dy,(shift|_bIsPressedAlt)*2,control,_bIsPressedAlt);
			//	2번 현 에니메이션의 모든 프레임의 스프라이트 위치 이동..
			break;

		case	dWORK_SET_CRASH		:
			CRASHBOX.move(-dx,-dy);
			bVIRGIN		=	FALSE;
			_WORKWND.Draw();
			break;

		case	dWORK_SET_SELECT	:
			g_rectSelect.move(-dx,-dy);
			bVIRGIN		=	FALSE;
			_WORKWND.Draw();
			break;
	}
}

void
cMAIN::keyThread()
{	
	BOOL	shift=FALSE,repaint=FALSE,control=FALSE,bIsAlt=FALSE;
	static	int		pressTime=0,move=1,keyPress=FALSE;
	static	DWORD	lastTime=timeGetTime();

	int		mx=0,my=0;
	int		gab	=	40;

	if	(pressTime < 5)
		gab = 150;

	if	(workMode==dWORK_GET_IMAGE)
		return;

	if	(keyBuff[VK_SHIFT	]&0x80	)
		shift	=	TRUE;
	if	(keyBuff[VK_CONTROL	]&0x80	)
		control	=	TRUE;
	if	(keyBuff[VK_LMENU	]&0x80	)
		bIsAlt	=	TRUE;
	
	//	스프라이트 위치 교정

	if	(lastTime+gab > timeGetTime() && keyPress)
		return;

	lastTime	=	timeGetTime();

	keyPress	=	FALSE;

	if	(keyBuff[VK_LEFT		]&0x80	)
	{
		keyPress	=	TRUE,repaint	|=	1;
		mx	=	move;
	}
	if	(keyBuff[VK_RIGHT	]&0x80	)
	{
		keyPress	=	TRUE,repaint	|=	1;
		mx	=	-move;
	}
	if	(keyBuff[VK_DOWN		]&0x80	)
	{
		keyPress	=	TRUE,repaint	|=	1;
		my	=-move;
	}
	if	(keyBuff[VK_UP		]&0x80	)
	{
		keyPress	=	TRUE,repaint	|=	1;
		my	=move;
	}

	//	스프라이트 위치교정 가속..
	if	(keyPress)
	{
		Move(mx,my,shift,control,bIsAlt);

		pressTime++;

			 if (pressTime < 5 )	move=1;
		else if (pressTime < 8  )	move=2;
		else if (pressTime < 10 )	move=3;
		else if (pressTime < 12 )	move=4;
		else if (pressTime < 14 )	move=5;
		else						move=8;
	}
	else
		pressTime=0,move=1;

	if	(repaint & 1)
		_WORKWND.Draw();
}

BOOL
cMAIN::getSlideShowInfo()
{
	char	*folder	=	cFOLDER::Tree(hWND,TRUE,"Select the folder containing the files you want to view.",_cfg.dataDir[0]);

	if	(!folder)
		return	FALSE;

	if	(!IsFolder(folder))
		return	ERRMSG("error in  cMAIN::getEditObjectInfo()","The following folder could not be found\n\n%s",folder);

	strcpy(l_slideShow.m_strFolder	,folder);

	SetCurrentDirectory(folder);

	cSTRINGS	*sadFiles	=	cFOLDER::GetFiles("*.sad");

	if (sadFiles)
	{
		cSTRING	*strFile	=	sadFiles->Pick(0);

		for (int i=0;i<sadFiles->Count;i++)
		{
			strcpy(l_slideShow.m_strFiles[l_slideShow.m_iCount++],strFile->String);

			strFile	=	strFile->pNEXT;
		}
	}

	KILL(sadFiles);

	if	(l_slideShow.m_iCount	<=	0)
		return	FALSE;

	l_slideShow.m_iEditFile	=	0;

	cANM::LoadSAD(l_slideShow.getEditFileName());

	Draw();
	SetCurrentDirectory(curDir);

	return	TRUE;
}

BOOL
cMAIN::updateSlideShow(BOOL _bIsPrevious)
{
	if	(!l_slideShow.isActive())
		return	getSlideShowInfo();

	l_slideShow.setFolder();

	SetCurrentDirectory(curDir);

	if	(_bIsPrevious)
	{
		if	(!l_slideShow.previous())
			return	FALSE;
	}
	else
	{
		if	(!l_slideShow.next())
			return	FALSE;
	}

	l_slideShow.setFolder();

	strcpy(FName,l_slideShow.getEditFileName());
	SetWindowText( hWND, FName);
	cANM::LoadSAD(l_slideShow.getEditFileName());

	SetCurrentDirectory(curDir);
	Draw();

	return	TRUE;
}

void
cMAIN::changePlayerBodyLayer(int _iDirect)
{
	int	aiLinkLayer[eBodyType_count];
	
	memset(aiLinkLayer,0,sizeof(aiLinkLayer));

	aiLinkLayer[eBodyType_headB]	=	1;
	aiLinkLayer[eBodyType_headF]	=	-1;
	aiLinkLayer[eBodyType_bodyB]	=	1;
	aiLinkLayer[eBodyType_bodyF]	=	-1;

	int		anm				=	cANM::CurAnm;
	int		dir				=	cANM::CurDirect;
	int		frm				=	cANM::CurFrame;
	int		iLayer			=	g_iSelectParts-1;
	int		iOrder			=	g_abDrawLayerOrder[anm][dir][frm][iLayer];
	int		iLinkedLayerValue=	aiLinkLayer[g_iSelectParts];

	if	(_iDirect==	-1	&&	iOrder	<=	0)	//	레이어 수치 감소
		return;

	if	(_iDirect==	1	&&	iOrder	>=	c_iBodyLayerCount-1)	//	레이어 수치 증가
		return;

	int		iTargetOrder			=	iOrder+_iDirect;
	int		iCorrectOrder			=	1;
	int		iCorrectMoveLayerOrder	=	1;

	if	(iLinkedLayerValue	==	_iDirect)
		iTargetOrder	+=	_iDirect;
	if	(iLinkedLayerValue)
		iCorrectOrder++;

	if	(_iDirect==	-1 && iTargetOrder	<	0)
		return;
	if	(_iDirect==	1 && iTargetOrder	>	c_iBodyLayerCount-1)
		return;

	for (int i=0;i<c_iBodyLayerCount;i++)
	{
		if	(g_abDrawLayerOrder[anm][dir][frm][i]	!=	iTargetOrder)
			continue;

		g_abDrawLayerOrder[anm][dir][frm][i]	+=	-1*_iDirect*iCorrectOrder;

		if	(aiLinkLayer[i+1])
		{
			iCorrectMoveLayerOrder	=	2;
			g_abDrawLayerOrder[anm][dir][frm][i+aiLinkLayer[i+1]]+=	-1*_iDirect*iCorrectOrder;
		}

		break;
	}

	g_abDrawLayerOrder[anm][dir][frm][iLayer]	+=	_iDirect*iCorrectMoveLayerOrder;

	if	(iLinkedLayerValue	!=	0)
		g_abDrawLayerOrder[anm][dir][frm][iLayer+iLinkedLayerValue]	+=	_iDirect*iCorrectMoveLayerOrder;

	Draw();

	bVIRGIN			=	FALSE;
}

void
cMAIN::updateKey(WPARAM key)
{	BOOL	repaint	=	FALSE;
	int		i;

	if	(workMode	==	dWORK_GET_IMAGE)
		return;

	if	(_MAIN.workMode	==	dWORK_SET_FRAME && g_wAnmDataType	==	dDATA_CHARACTER && g_wPlayerJob && g_iSelectParts)
	{
		int	iDirect	=	0;

		if	(key == 0xbd)	//	'-'키 레이어 수치 감소
			iDirect	=	-1;
		if	(key == 0xbb)	//	'+'키 레이어 수치 감소
			iDirect	=	1;

		changePlayerBodyLayer(iDirect);
	}

	switch(key)
	{
		case	VK_ADD		:
			_WORKWND.ZoomIn()	;
			repaint|=1;
			break;
			
		case	VK_SUBTRACT	:
			_WORKWND.ZoomOut()	;
			repaint|=1;
			break;

		case	VK_F3		:
			{
				_MAIN.updateSlideShow(TRUE);
				break;
			}
		case	VK_F4		:
			{
				_MAIN.updateSlideShow(FALSE);
				break;
			}
		case	VK_F1		:
			if	(ANM[cANM::CurAnm].bSaveReleasePoint	)
			{
				ANM[cANM::CurAnm].Release[cANM::CurDirect].x	=	(_WORKWND.Mouse.x - _WORKWND.Zero.x)*100/_WORKWND.Zoom;
				ANM[cANM::CurAnm].Release[cANM::CurDirect].y	=	(_WORKWND.Mouse.y - _WORKWND.Zero.y)*100/_WORKWND.Zoom;
				bVIRGIN		=	FALSE;
				_WORKWND.Draw();
			}
			break;

		case	VK_F2		:
			_FRMWND.IncAllIndex();
			break;

		case	'Z'			:	if (workMode==dWORK_SET_FRAME)	{	repaint|=7	;cANM::MinusCurrentFrame();}
								else							{	repaint|=3	;cANM::MinusCurrentImage();}
								break;

		case	'X'			:	if (workMode==dWORK_SET_FRAME)	{	repaint|=7	;cANM::PlusCurrentFrame();	}
								else							{	repaint|=3	;cANM::PlusCurrentImage();	}
								break;

		case	'Q'			:	if (workMode!=dWORK_SET_FRAME) _MAIN.changeMode(dWORK_SET_FRAME);
								repaint|=7;cANM::MinusCurrentAnm();		break;
		case	'W'			:	if (workMode!=dWORK_SET_FRAME) _MAIN.changeMode(dWORK_SET_FRAME);
								repaint|=7;cANM::PlusCurrentAnm();		break;
		case	'S'			:	if (workMode!=dWORK_SET_FRAME) _MAIN.changeMode(dWORK_SET_FRAME);
								if (cANM::AnmType!=	dANI_CUSTOM		)
									if (ANM[cANM::CurAnm].directCount	>	1)
									{	repaint|=7;cANM::PlusCurrentDirect();	}
								break;

		case	'A'			:	if (workMode!=dWORK_SET_FRAME) _MAIN.changeMode(dWORK_SET_FRAME);
								if (cANM::AnmType!=	dANI_CUSTOM		)
									if (ANM[cANM::CurAnm].directCount	>	1)
										{	repaint|=7;cANM::MinusCurrentDirect();	}
								break;

		case	'D'			:	if (workMode!=dWORK_SET_FRAME) break;
								for (i=0;i<dMAX_FRAME;i++)	if (IsSelectedFrame[i])	cANM::DamageFrame(i);
								Draw();
								break;

		case	'B'			:	if (workMode!=dWORK_SET_FRAME) break;
								for (i=0;i<dMAX_FRAME;i++)	if (IsSelectedFrame[i])	cANM::SetBack(i);
								Draw();
								break;

		case	'T'			:	if (workMode!=dWORK_SET_FRAME) break;
								for (i=0;i<dMAX_FRAME;i++)	if (IsSelectedFrame[i])	cANM::TriggerFrame(i);
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

	if (HORZ.bMoving)	{	resizeHorz=TRUE;return TRUE;}
	if (resizeHorz)
	{
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
		case	dWORK_SET_SELECT	:
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
{
	if	(!hWND || !bACTIVEAPP)
		return;

	if	(WIDTH	!=	GetSystemMetrics(SM_CXSCREEN) ||HEIGHT	!=	GetSystemMetrics(SM_CYSCREEN))
		resetBorder();

	InitWndRect();
	
	if	(workWnd.height()	<	60	||	workWnd.width()	<	60	)
		return;

	_WORKWND.Resize(workWnd,FALSE);
	_WORKWND.ResetZero();
	_BMPWND.Resize(bmpWnd,FALSE);
	_IMGWND.Resize(imgWnd,FALSE);
	_FRMWND.Resize(frmWnd,FALSE);

	TOOLBAR.Resize(wWidth()+100,30);

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

//
//	스프라이트 위치 리셋
void
cMAIN::resetSpritePos()
{
	cIMG	*lpImg	=	cIMG::Get(0);

	for (int i=0;i<cIMG::Count;i++)
	{
		lpImg->rePos(0,0);
		lpImg=lpImg->pNEXT;
	}

	Draw();
}	//	cMAIN::resetSpritePos()

void
cMAIN::resetToolbar()
{	TOOLBAR.Check(IDM_MULTI_IMG,_cfg.bMULTI);

	TOOLBAR.Check(IDM_GET_IMAGE	,FALSE);
	TOOLBAR.Check(IDM_SET_SPRITE,FALSE);
	TOOLBAR.Check(IDM_SET_FRAME	,FALSE);
	TOOLBAR.Check(IDM_SET_CRASH	,FALSE);
	TOOLBAR.Check(IDM_SET_SELECT,FALSE);

	if (cIMG::Count<=0)
	{
		TOOLBAR.Disable(IDM_SAVE);
		TOOLBAR.Disable(IDM_SET_FRAME);
		TOOLBAR.Disable(IDM_SET_CRASH);
		TOOLBAR.Disable(IDM_SET_SELECT);
	}
	else
	{	
		TOOLBAR.Enable(IDM_SAVE);
		TOOLBAR.Enable(IDM_SET_FRAME);
		TOOLBAR.Enable(IDM_SET_CRASH);
		TOOLBAR.Enable(IDM_SET_SELECT);
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

		case	dWORK_SET_SELECT	:
			TOOLBAR.Disable(IDM_SPOID);
			TOOLBAR.Disable(IDM_PRE_DIRECT);
			TOOLBAR.Disable(IDM_NEXT_DIRECT);
			if (_WORKWND.Zoom!=500)	TOOLBAR.Enable(IDM_ZOOMIN);
			if (_WORKWND.Zoom> 50)	TOOLBAR.Enable(IDM_ZOOMOUT);
			TOOLBAR.Check(IDM_SET_SELECT,TRUE);
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

	TOOLBAR.SetTip(strToolbar,22);

	SetMenu();
}

void
cMAIN::changeMode(int mode)
{	if	(workMode==mode) return;

	switch(mode)
	{	case	dWORK_SET_FRAME		:
			cANM::SetCurrentFrame();
			if (workMode==dWORK_GET_IMAGE)	_BMPWND.CLOSEWND();
			break;
		
		case	dWORK_SET_SPRITE	:
		case	dWORK_SET_CRASH		:
		case	dWORK_SET_SELECT	:
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
cMAIN::Save(BOOL _bIsSaveCodeSprite)
{	
	if	(strcmp(FName,"")	==	0)
		return SaveAs();
	
	switch(currentExt)
	{
		case	dDATA_SAD	:
			cANM::SaveSAD(FName);
			break;

		case	dDATA_SD	:
			cANM::SaveSD(FName,_bIsSaveCodeSprite);
			break;

		case	dDATA_ID	:
			break;

		case	dDATA_PHF	:
			cANM::SaveAsFont(FName,0);
			break;

		case	dDATA_PEF	:
			cANM::SaveAsFont(FName,1);
			break;
	}

	Update();

	return TRUE;
}

BOOL
cMAIN::SaveAs()
{	int i;

	if (cIMG::Count<=0)
	{	return cMSG::Error("데이터 저장 에러!!","저장할 이미지가 없습니다.");
	}

	OPENFILENAME	ofn;

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	hWND;
	ofn.lpstrFilter		=	"Animation Data {*.sad}\0*.sad\0스프라이트 데이터 {*.sd}\0*.sd\0\0";
	ofn.lpfnHook		=	hookSave;
	ofn.lpstrTitle		=	"데이터 저장.";
	ofn.lpstrFile		=	FName;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER;
	ofn.lpTemplateName	=	"QUICK_OPEN";
	ofn.lpstrDefExt		=	"sad";
	ofn.nFilterIndex	=	currentExt+1;
	ofn.lpstrInitialDir	=	_cfg.dataDir[currentExt];

	if (!GetSaveFileName(&ofn)) return FALSE;

	char *ext		=	_exportExt(FName);

	for (i=0;i<5;i++)
		if (strcmp(ext,strExt[i])==0)
		{	currentExt	=	i;
			break;
		}

	GetCurrentDirectory(255,_cfg.dataDir[currentExt]);

	return Save();
}

BOOL
cMAIN::SaveLayer()
{	
	OPENFILENAME	ofn;

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	hWND;
	ofn.lpstrFilter		=	"레이어 데이터 {*.ld}\0*.ld\0\0";
	ofn.lpfnHook		=	hookSave;
	ofn.lpstrTitle		=	"데이터 저장.";
	ofn.lpstrFile		=	FName;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER;
	ofn.lpstrDefExt		=	"ld";
	ofn.nFilterIndex	=	currentExt+1;

	SetCurrentDirectory(_cfg.dataDir[currentExt]);

	if	(!GetSaveFileName(&ofn))
		return FALSE;

	cANM::SaveAsFont(FName,0);

	return TRUE;
}

BOOL
cMAIN::LoadData(int what,BOOL _bIsParts)
{	
	char	fn[256]	=	"";

	if	(_bIsParts)
	{
		char	strPartType[eBodyType_count][128]	=
		{	
			"",
"head_back",
			"head_front",
			"body_back",
			"body_front",
			"left arm",
			"right arm",
			"left foot",
			"right foot",
			"weapon",
			"shield",
		};

		char	strText[128];

		sprintf(strText,"%s Selecting Parts",strPartType[what]);

		if	(!cFILE::LoadBox(fn,"Red Stone Animation Data {*.sad}\0*.sad\0\0","sad",l_strPartsDataFolder[what],strText))
			return FALSE;

		g_aPartsData[what].Load(fn);

		GetCurrentDirectory(255,l_strPartsDataFolder[what]);

		if	(g_aPartsData[what].Count	!=	cIMG::Count)
		{
			g_aPartsData[what].Close();

			ERRMSG(_ms("%s Loading errors",strPartType[what]),"You must first make sure that the number of images is exactly the same as the data you are working with:\n\nThe number of images in the loaded image is %d.",g_aPartsData[what].Count);

			Draw();

			return	FALSE;
		}

		if	(g_aPartsData[what].m_wParts	!=	what)
		{	
			g_aPartsData[what].Close();

			ERRMSG(_ms("%s Loading errors",strPartType[what]),"This file is not %s.",strPartType[what]);

			Draw();

			return	FALSE;
		}

		Draw();

		return	TRUE;
	}

	char	datatype[6][256]	=
{ "Character Data",
		"Weapon data",
		"Weapon effect data",
		"Shield data",
		"shield effect data",
		"magic effect data"
	};

	if	(cIMG::Count == 0)
		return ERRMSG("데이터 Loading errors","First, you need to have data to work with.");

	if	(!cFILE::LoadBox(fn,"Red Stone Animation Data {*.sad}\0*.sad\0\0","sad",l_dataFolder[what],"Load data"))
		return FALSE;

	addDATA[what].Load(fn);

	GetCurrentDirectory(255,l_dataFolder[what]);

	if	(addDATA[what].Count	!=	cIMG::Count)
		ERRMSG(_ms("%s Loading errors",datatype[what]),"You must first make sure that the number of images is exactly the same as the data you are working with:\n\nThe number of images in the loaded image is %d.",addDATA[what].Count);
	
	if (addDATA[what].Kind	!=	what)
	{	
		ERRMSG(_ms("%s Loading errors",datatype[what]),"This file is not %s.",datatype[what]);

		addDATA[what].Close();
	}

	Draw();

	return TRUE;
}

BOOL
cMAIN::Import(BOOL _isOperateSitDown)
{	
	char	fn[256]	=	"";

	if	(cIMG::Count == 0)
		return ERRMSG("데이터 로딩 에러","먼저 작업중인 데이터가 있어야 합니다.");

	if	(!cFILE::LoadBox(fn,"Animation Data {*.sad}\0*.sad\0\0","sad",l_dataFolder[g_wAnmDataType],"Load data"))
		return FALSE;

	cANM::Import(fn,_isOperateSitDown);

//	addDATA[what].Load(fn);
	GetCurrentDirectory(255,l_dataFolder[g_wAnmDataType]);
//	if (addDATA[what].Count	!=	cIMG::Count)
//		ERRMSG("데이터 로딩 에러","먼저 작업중인 데이터와 이미지의 수가 완전히 같아야 합니다.\n\n로딩한 화일의 이미지 수는 %d개 입니다.",addDATA[what].Count);
	
	Draw();

	return TRUE;
}

BOOL
cMAIN::Load(BOOL _isAddSitdown)
{	int i;

	if (!bVIRGIN)
	{	switch(cMSG::YESNOCANCEL("You have unsaved data.","You have unsaved data.\n\nDo you want to save the data?"))
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
	ofn.lpstrFilter		=	"Animation Data {*.sad}\0*.sad\0스프라이트 데이터 {*.sd}\0*.sd\0\0";
	ofn.lpfnHook		=	hookSave;
	ofn.lpstrTitle		=	"데이터 불러오기.";
	ofn.lpstrFile		=	FName;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER;
	ofn.lpstrDefExt		=	"sad";
	ofn.nFilterIndex	=	currentExt+1;
	ofn.lpstrInitialDir	=	_cfg.dataDir[currentExt];

	if	(!GetOpenFileName(&ofn))
		return FALSE;

	char *ext			=	_exportExt(FName);

	for (i=0;i<5;i++)
		if (strcmp(ext,strExt[i])==0)
		{	currentExt	=	i;
			break;
		}

	GetCurrentDirectory(255,_cfg.dataDir[currentExt]);

	switch(currentExt)
	{	case	dDATA_SAD	:
			cANM::LoadSAD(FName,_isAddSitdown, hWND);
			break;
		case	dDATA_SD	:
			cANM::LoadSD(FName);
			break;

		case	dDATA_ID	:
			break;

	}

	Update();

	Lock	=	TRUE;

	Draw();

	return TRUE;
}


void
cMAIN::ResaveANMFiles()
{
	char	*folder	=	cFOLDER::Tree(hWND,TRUE,"Select the folder containing the files you want to convert.");	//	폴더트리 쉘

	if (!folder)	return;
/*
	cSTRINGS	*lpFiles=	cFOLDER::GetFiles("*.sad");

	if (!lpFiles)
	{
		cMSG::Error("컨버트 작업 에러!!","'%s' 폴더에서 '*.sad' 화일을 찾을 수 없습니다.");

		return;
	}

	cSTRING		*lpFile	=	lpFiles->Pick();

	while(lpFile->String)
	{
		if (cANM::LoadSAD(lpFile->String))	cANM::SaveSAD(lpFile->String);

		lpFile	=	lpFile->pNEXT;
	}

	KILL(lpFiles);
*/
	cSTRINGS	*baseFolders	=	NULL;
	cSTRING		*curFolder		=	NULL;

	baseFolders					=	cFOLDER::GetFolders(folder);

	if (!baseFolders)	cMSG::Error("컨버트 작업 에러!!","'%s' 폴더에서 폴더를 찾을 수 없습니다.",folder);

	curFolder					=	baseFolders->Pick(0);

	SetCurrentDirectory(curFolder->String);

	int	iFileCount				=	0;

	for (int f=0;f<baseFolders->Count;f++)
	{
		MSGOUT("\n--------- Folder %s -------",curFolder->String);

		cSTRINGS	*lpFiles=	cFOLDER::GetFiles("*.sad");

		if (lpFiles)
		{
			cSTRING		*lpFile	=	lpFiles->Pick();

			while(lpFile->String)
			{
				if (cANM::LoadSAD(lpFile->String))
				{
					MSGOUT(lpFile->String);
					Draw();

					cANM::SaveSAD(lpFile->String,FALSE);

					iFileCount++;
				}
				lpFile	=	lpFile->pNEXT;
			}

			KILL(lpFiles);
		}

		curFolder	=	curFolder->pNEXT;

		SetCurrentDirectory(curFolder->String);
	}

	cMSG::Put("Conversion job completed","Converted %d images.",iFileCount);
}

void
cMAIN::resetPlayerLayerData()
{
	for (int i=0;i<30;i++)
		for (int j=0;j<8;j++)
			for (int k=0;k<128;k++)
				for (int l=0;l<eBodyType_count;l++)
					g_abDrawLayerOrder[i][j][k][l]	=	l;
}

void
cMAIN::loadPlayerLayerData(BOOL _bByMenu)
{
	if	(g_wPlayerJob	==	0)
	{
		cMSG::Error("경고!","편집중인 플레이어 직업이 없습니다.");

		return;
	}

	char	strFileName[256];

	sprintf(strFileName,"%s.lod",g_strJobName[g_wPlayerJob-1]);

	FILE *fp;

	SetCurrentDirectory(curDir);

	if	(!SetCurrentDirectory("layer order data"))
	{
		if	(_bByMenu)
			cMSG::Error("경고!","[layer order data] 폴더를 찾을 수 없습니다.\n\n툴의 하위 폴더에 있어야 합니다.");

		return;
	}

	fp	=	fopen(strFileName,"rb");

	if	(!fp)
	{
		if	(_bByMenu)
			cMSG::Error("경고!","%s 파일을 찾을 수 없습니다.",strFileName);

		return;
	}

	resetPlayerLayerData();

	int	iAnmCount	=	cANM::AnmCount;

	fread(&iAnmCount,4,1	,fp);			//	기본 공격

	for (int i=0;i<iAnmCount;i++)	//	에니 수
	{
		int	iFrameCount;
		fread(&iFrameCount,sizeof(int),1,fp);	//	프레임 수
	
		if	(iFrameCount <= 0)
			continue;

		int	iDirectCount;

		fread(&iDirectCount,4	,1,fp);	//	방향 수

		for (int j=0;j<iDirectCount;j++)
		{
			for (int k=0;k<iFrameCount;k++)
				fread(g_abDrawLayerOrder[i][j][k],eBodyType_count-1,1,fp);
		}
	}

	fclose(fp);

	g_bIsVirginLayerData	=	TRUE;

	if	(_bByMenu)
		cMSG::Put("Done","Loading layer output order information for player class [%s].",g_strJobName[g_wPlayerJob-1]);
}

void
cMAIN::savePlayerLayerData()
{
	if	(g_wPlayerJob	==	0)
	{
		cMSG::Error("경고!","편집중인 플레이어 직업이 없습니다.");

		return;
	}

	char	strFileName[256];

	sprintf(strFileName,"%s.lod",g_strJobName[g_wPlayerJob-1]);

	FILE *fp;

	SetCurrentDirectory(curDir);
	CreateDirectory("layer order data",NULL);
	SetCurrentDirectory("layer order data");

	fp	=	fopen(strFileName,"wb");

	fwrite(&cANM::AnmCount,4,1	,fp);			//	기본 공격

	for (int i=0;i<cANM::AnmCount;i++)	//	에니 수
	{
		fwrite(&ANM[i].FrameCount		,sizeof(int),1,fp);	//	프레임 수
	
		if	(ANM[i].FrameCount <= 0)
			continue;

		fwrite(&ANM[i].directCount		,4	,1,fp);	//	방향 수

		for (int j=0;j<ANM[i].directCount;j++)
		{
			for (int k=0;k<ANM[i].FrameCount;k++)
				fwrite(g_abDrawLayerOrder[i][j][k],eBodyType_count-1,1,fp);
		}
	}

	fclose(fp);

	g_bIsVirginLayerData	=	TRUE;
}


LRESULT CALLBACK 
cMAIN::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uMsg )
	{	
		case WM_CREATE		:
			return _MAIN.Init(hwnd);

		case WM_ACTIVATEAPP :
			bACTIVEAPP = wParam;

			if	(bACTIVEAPP)
				SetActiveWindow(hwnd);

			break;

		case WM_SIZE		:
			if	(bACTIVEAPP)
				_MAIN.ResizeWnds(wParam,lParam);
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
            {
				case	IDM_SAVE_PLAYER_PARTS_LAYER	:
				{
					_MAIN.savePlayerLayerData();

					break;
				}
				case	IDM_LOAD_PLAYER_PARTS_LAYER	:
				{
					_MAIN.loadPlayerLayerData(TRUE);

					break;
				}
				case	IDM_LOAD_PARTS_IMAGE	:
				{
					g_iLoadPart	=	1;
					_BMPWND.OPEN();
					_MAIN.resetToolbar();
					break;
				}

				case	IDM_IMPORT			:
					_MAIN.Import();
					break;

				case	IDM_IMPORT2			:
					_MAIN.Import(TRUE);
					break;

				case	IDM_LOADEFFECT		:
					_MAIN.LoadData(dDATA_EFFECT);
					break;

				case	IDM_LOADSHIELD		:
					_MAIN.LoadData(dDATA_SHIELD);
					break;

				case	IDM_LOADWEAPON		:
					_MAIN.LoadData(dDATA_WEAPON);
					break;

				case	IDM_LOADWEAPONEFFECT:
					_MAIN.LoadData(dDATA_WEAPONEFFECT);
					break;

				case	IDM_LOADSHIELDEFFECT:
					_MAIN.LoadData(dDATA_SHIELDEFFECT);
					break;

				case	IDM_LOAD			:
					_MAIN.Load();
					break;

				case	IDM_NEW				:
					_MAIN.New();
					break;

				case	IDM_SAVELAYER		:
					_MAIN.SaveLayer();
					break;

				case	IDM_SAVEAS			:
					_MAIN.SaveAs();
					break;

				case	IDM_SAVE_CODE_SPRITE	:
					_MAIN.Save(TRUE);
					break;
				case	IDM_SAVE				:
					_MAIN.Save();
					break;

				case	IDM_PRE_ANM			:
					wParam	=	'Q';
					_MAIN.updateKey(wParam);
					break;
				case	IDM_NEXT_ANM		:
					wParam	=	'W';
					_MAIN.updateKey(wParam);
					break;
				case	IDM_PRE_DIRECT		:
					wParam	=	'A';
					_MAIN.updateKey(wParam);
					break;
				case	IDM_NEXT_DIRECT		:
					wParam	=	'S';
					_MAIN.updateKey(wParam);
					break;

				case	IDM_MULTI_IMG		:
					_cfg.bMULTI	=	1	-	_cfg.bMULTI;
					_MAIN.resetToolbar();
					break;

				case	IDM_LOADBMP			:
					bGETSHADOW	=	FALSE;
					g_isGetImageAndShadow	=	FALSE;
					_BMPWND.OPEN();
					_MAIN.resetToolbar();
					break;

				case	IDM_LOADSHADOW		:
					bGETSHADOW	=	TRUE;
					g_isGetImageAndShadow	=	FALSE;
					_BMPWND.OPEN();
					_MAIN.resetToolbar();
					break;

				case	IDC_EXPORT	:	
					_MAIN.exportImages();
					break;

				case	IDM_LOAD_SET		:
					g_isGetImageAndShadow	=	TRUE;
					_BMPWND.OPEN();
					_MAIN.resetToolbar();
					break;

				case	IDM_RESET_POS		:
					_MAIN.resetSpritePos();
					break;

				case	IDM_RESAVE			:
					_MAIN.ResaveANMFiles();
					break;

				case	IDM_ZOOMIN			:
					_WORKWND.ZoomIn();
					break;
				case	IDM_ZOOMOUT			:
					_WORKWND.ZoomOut();
					break;

				case	IDM_SET_SPRITE		:
					_MAIN.changeMode(dWORK_SET_SPRITE);
					break;
				case	IDM_SET_FRAME		:
					_MAIN.changeMode(dWORK_SET_FRAME);
					break;
				case	IDM_SET_CRASH		:
					_MAIN.changeMode(dWORK_SET_CRASH);
					break;
				case	IDM_SET_SELECT		:
					_MAIN.changeMode(dWORK_SET_SELECT);
					break;
				case	IDM_GET_IMAGE		:
					_MAIN.changeMode(dWORK_GET_IMAGE);
					break;

				case	IDM_SETANM			:
					cSET::SetAnm();
					break;

				case	IDM_SETTOOL			:
					cSET::SetTool();
					break;

				case IDM_SPOID				:
					_BMPWND.SpoidColor();
					break;

				default						:
					if (LOWORD( wParam ) >= IDM_BG_ANM && LOWORD( wParam ) < IDM_BG_ANM+cANM::AnmCount)
					{	HMENU	hMENU	=	GetSubMenu(GetMenu(_MAIN.hWND),2);
						int	index	=	LOWORD( wParam ) - IDM_BG_ANM;

						if (_MAIN.BGAnm!=0xffff)
							CheckMenuItem(hMENU,IDM_BG_ANM+_MAIN.BGAnm,MF_BYCOMMAND|MF_UNCHECKED);

						if (index == _MAIN.BGAnm)
						{	_MAIN.BGAnm	=	0xffff;
							_MAIN.Draw();
							break;
						}
						_MAIN.BGAnm	=	index;
						_MAIN.Draw();
						CheckMenuItem(hMENU,IDM_BG_ANM+_MAIN.BGAnm,MF_BYCOMMAND|MF_CHECKED);
						break;
					}

					if (LOWORD( wParam ) >= IDM_CHANGEDIRECT && LOWORD( wParam ) < IDM_CHANGEDIRECT+cANM::GetDirCount())
					{	HMENU	hMENU	=	GetSubMenu(GetMenu(_MAIN.hWND),3);
						CheckMenuRadioItem(hMENU,IDM_CHANGEDIRECT,IDM_CHANGEDIRECT+cANM::GetDirCount()-1,LOWORD( wParam ),MF_BYCOMMAND);
						cANM::SetCurrentDirect(LOWORD( wParam )-IDM_CHANGEDIRECT);
						_MAIN.changeMode(dWORK_SET_FRAME);
						_MAIN.Draw();
						break;
					}
					break;
			}
			break;

// 		case WM_SYSCOMMAND	:	//	윈도우 움직이는것과.. 사이즈 바꾸는거 막아 버림.. --;;
// 		{	
// 			WINDOWPLACEMENT	place;
// 			place.length	=	sizeof(WINDOWPLACEMENT);
// 			GetWindowPlacement(hwnd,&place);
// 
// 			if ((LOWORD(wParam)&0xfff0) == SC_MOVE	) return FALSE;
// 			if (place.showCmd	==	SW_SHOWMAXIMIZED)	if ((LOWORD(wParam)&0xfff0) == SC_RESTORE) return FALSE;
// 			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
// 			break;
// 		}

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