#include "cBMPWND.H"
#include "cANM.H"
#include "cIMGWND.H"
#include "cFRMWND.H"
#include "cWORKWND.H"
#include "cMAIN.H"
#include "cTEXT.H"

int	totalImage		=	0;
int	totalFile		=	0;
int file8			=	0;
int file16			=	0;

WORD	*backUp2	=	NULL,*horz	=NULL,*vert	=NULL;
int		hCursor2	=	0;
HCURSOR	hCursor		=	NULL;
BOOL	bGETSHADOW	=	FALSE;
BOOL	bOPENFILES	=	FALSE;
BOOL	g_isGetImageAndShadow	=	FALSE;
cBMPWND	_BMPWND;

cBMPWND::cBMPWND()
		:cDIBWND()
{	pBMP		=	NULL;
	workMode	=	dMODE_NORMAL;
	SetRect(&ImageRect,-1,-1,-1,-1);
	strcpy(BMPName,"nemo.tga");
	rotateCOLOR	=	0;
	XPos		=	0;
	YPos		=	0;
	bSPOID		=	FALSE;
	curExt		=	1;
}

cBMPWND::~cBMPWND()
{
}

BOOL
cBMPWND::Init(HINSTANCE hInst,HWND hWnd,cRECT *rect)
{	if (!pBMP)	return OPEN();
	if (hWND) return TRUE;

	_WORKWND.Close();
	lpszRegister	=	"BMPWINDOW";

	if (!cDIBWND::Init(	0,(LPTSTR)lpszRegister,(LPTSTR)lpszRegister,
						WS_CHILD|WS_VSCROLL|WS_HSCROLL,
						rect->x1,rect->y1,rect->x2,rect->y2,
						hWnd,NULL,hInst, 0L,
						SW_SHOW,(WNDPROC)WNDProc
						)
		)
		return FALSE;

	lbDown			=	TRUE;

	_MAIN.workMode	=	dWORK_GET_IMAGE;

	Draw();

	return TRUE;
}

void 
cBMPWND::VScroll(WORD wScroll )
{	SCROLLINFO si;

	si.cbSize= sizeof( SCROLLINFO );
	si.fMask =SIF_ALL;
	GetScrollInfo( hWND, SB_VERT, &si );

	switch( wScroll )
	{	case SB_LINEDOWN :
			if ( YPos <= (int)(si.nMax - si.nPage) )	YPos += 1;
			break;

		case SB_LINEUP :
			if ( YPos > 0 ) YPos -= 1;
			break;

		case SB_THUMBTRACK :  // 스크롤바를 잡구 움직였을때..
			GetScrollInfo( hWND, SB_VERT, &si );
			YPos = si.nTrackPos;
			break;
	}

	if (si.nPos != YPos )
	{	si.fMask = SIF_POS;
		si.nPos  = YPos;
		SetScrollInfo( hWND, SB_VERT, &si, TRUE );
		Draw();
	}
}

void 
cBMPWND::HScroll(WORD wScroll )
{	SCROLLINFO si;

	si.cbSize= sizeof( SCROLLINFO );
	si.fMask =SIF_ALL;
	GetScrollInfo( hWND, SB_HORZ, &si );

	switch( wScroll )
	{	case SB_LINEDOWN :
			if ( XPos <= (int)(si.nMax - si.nPage) )	XPos += 1;
			break;

		case SB_LINEUP :
			if ( XPos > 0 ) XPos -= 1;
			break;

		case SB_THUMBTRACK :  // 스크롤바를 잡구 움직였을때..
			GetScrollInfo( hWND, SB_HORZ, &si );
			XPos = si.nTrackPos;
			break;
	}

	if (si.nPos != XPos )
	{	si.fMask = SIF_POS;
		si.nPos  = XPos;
		SetScrollInfo( hWND, SB_HORZ, &si, TRUE );
		Draw();
	}
}

void
cBMPWND::CLOSEWND()
{	Close();
	_WORKWND.Init(_MAIN.hINST,_MAIN.hWND,&_MAIN.workWnd);
	cIMG::SetCURRENT(0);
}

void
cBMPWND::SpoidColor()
{	bSPOID	=	1	-	bSPOID;

	if (!bSPOID	)	
	{	SetCursor(_MAIN.defaultCursor);
		return;
	}

	hCursor	=	_MAIN.spoidCursor;

	Draw();
}

void
cBMPWND::Draw()
{	int i;

	if (!pBMP		)	return;
	if (!Active()	)	return;

	SCROLLINFO si;

	if (pBMP->Width - width() > 0 )
	{	si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		si.nMin		=	0;
		si.nMax		=	pBMP->Width-1;
		si.nPage	=	width();
		si.nPos		=	XPos;
		EnableScrollBar(hWND,SB_HORZ,ESB_ENABLE_BOTH);
		SetScrollInfo(hWND,SB_HORZ,&si,TRUE);

		if (XPos > pBMP->Width - width())  XPos	=	pBMP->Width - width();
	}
	else 
	{	EnableScrollBar(hWND,SB_HORZ,ESB_DISABLE_BOTH);
		XPos		=	0;
		cDRAW::Fill(0,pBMP->Width,0,width(),height());
	}

	if (pBMP->Height - height() > 0 )
	{	si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		si.nMin		=	0;
		si.nMax		=	pBMP->Height-1;
		si.nPage	=	height();
		si.nPos		=	YPos;
		EnableScrollBar(hWND,SB_VERT,ESB_ENABLE_BOTH);
		SetScrollInfo(hWND,SB_VERT,&si,TRUE);

		if (YPos > pBMP->Height - height())  YPos	=	pBMP->Height - height();
	}
	else
	{	EnableScrollBar(hWND,SB_VERT,ESB_DISABLE_BOTH);
		YPos		=	0;
		cDRAW::Fill(0,0,pBMP->Height,min(pBMP->Width,width()),height());
	}

	pBMP->Put(-XPos,-YPos);

	if (!bMOVE && !bSPOID)
	{	switch(workMode)
		{	case	dMODE_NORMAL		:
				if (Mouse.y < pBMP->Height && Mouse.x < pBMP->Width)
				{	cDRAW::XLine(_gCOL[1][rotateCOLOR],0,pBMP->Width -1,Mouse.y);
					cDRAW::YLine(_gCOL[1][rotateCOLOR],0,pBMP->Height-1,Mouse.x);
				}
				break;

			case	dMODE_RESIZEBOX		:
				cDRAW::Box(_gCOL[1][rotateCOLOR],ImageRect.left-XPos,ImageRect.top-YPos,ImageRect.right-XPos,ImageRect.bottom-YPos);
				break;

			case	dMODE_WAITBUTTON	:
			{	WORD color;

				for (i=0;i<ImageCount;i++)
					cDRAW::Box(_gCOL[1][rotateCOLOR],Images[i].left-XPos,Images[i].top-YPos,Images[i].right-XPos,Images[i].bottom-YPos);
				if (timeGetTime()%1000 < 500)	color = 0x7fff;
				else							color = RGBmix(31,63,0);

				cTEXT::OutLine(TRUE,0);
				cTEXT::Put(4,4 ,color,"선택한 이미지가 맞으면 마우스 왼쪽버튼을",32);
				cTEXT::Put(4,18,color,"누르시고 틀리면 오른쪽버튼을 눌러 주세요.",32);
				cTEXT::OutLine(TRUE,0);
				break;
			}
		}

		rotateCOLOR++;
		if (rotateCOLOR >= 16)	rotateCOLOR=0;
	}

	cDIBWND::Draw();
}

//	화일 입출력 클래스

BOOL
cBMPWND::loadBmp(char *fname)
{
	if (bGETSHADOW)	return loadShadow(fname);

	void	*temp;

	if (stricmp(_exportExt(fname),"tga")==0)	temp	=	new cTGA();
	else
	if (stricmp(_exportExt(fname),"bmp")==0)	temp	=	new cBMP();
	else
	if (stricmp(_exportExt(fname),"pcx")==0)	temp	=	new cPCX();
	else	return FALSE;

	if (!((cIMAGE *)temp)->Load(fname))	return FALSE;

	if (((cIMAGE *)temp)->BPP	==	24	)	((cIMAGE *)temp)->BPP=16;
	if (((cIMAGE *)temp)->BPP	!=	cANM::BPP && cIMG::Count > 0)
	{	//if (bQUICKOPEN)	return MSGOUT("Error in cBMPWND::loadBmp - '%s' 화일이 %dbit 이미지 데이터가 아닙니다.",fname,cANM::BPP);
		return ERRMSG("Error in cBMPWND::loadBmp","'%s' 화일이 %dbit 이미지 데이터가 아닙니다.",fname,cANM::BPP);
	}

	KILL(pBMP);
	pBMP		=	(cIMAGE *)temp;
	cANM::BPP	=	pBMP->BPP;

	if (pBMP->p16IMAGE)
	{	transColor	=	pBMP->p16IMAGE[2];
		file16++;
	}
	else
	{
		transColor			=	pBMP->p8IMAGE[4];
		g_bTransColorIndex	=	(BOOL)transColor;

		file8++;
		if (cIMG::Count > 0)
		{	for (int i=0;i<256;i++)
			{	if (cANM::PLT[i] != pBMP->pPLT[i])
				{	if (bQUICKOPEN)	MSGOUT("Error in cBMPWND::loadBmp - '%s' 화일의 팔레트가 기존에 팔레트와 일치하지 않습니다.",fname);
					else			ERRMSG("Error in cBMPWND::loadBmp","'%s' 화일의 팔레트가 기존에 팔레트와 일치하지 않습니다.",fname);
					goto LABEL_PLT_COPY;
				}
			}
		}
LABEL_PLT_COPY:
		memcpy(cANM::PLT	,pBMP->pPLT		,256*2);
	}

	MSG	msg;

	if (bQUICKOPEN)
	{
		MakeImage(0,0,pBMP->Width-1,pBMP->Height-1);
		cIMG::SetCURRENT(0);

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{	TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
		_MAIN.Update();
	}

	return TRUE;
}

BOOL
cBMPWND::loadShadow(char *fname)
{	BOOL	back	=	_cfg.bMULTI;
	BOOL	bquick	=	bQUICKOPEN;

	_cfg.bMULTI		=	FALSE;
	bQUICKOPEN		=	TRUE;


	void	*temp;

	if (stricmp(_exportExt(fname),"tga")==0)	temp	=	new cTGA();
	else
	if (stricmp(_exportExt(fname),"bmp")==0)	temp	=	new cBMP();
	else
	if (stricmp(_exportExt(fname),"pcx")==0)	temp	=	new cPCX();
	else	return FALSE;

	if (!((cIMAGE *)temp)->Load(fname))
		return MSGOUT("can not load shadow image '%s'",fname);

	if (((cIMAGE *)temp)->BPP	==	24	)	((cIMAGE *)temp)->BPP=16;

	KILL(pBMP);

	pBMP		=	(cIMAGE *)temp;

	if (pBMP->p16IMAGE)
	{	transColor	=	pBMP->p16IMAGE[2];
		file16++;
	}
	else
	{	transColor	=	pBMP->p8IMAGE[4];
		file8++;
	}

	MakeImage(0,0,pBMP->Width-1,pBMP->Height-1);

	cIMG::SetCURRENT(0);

	MSG msg;

	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{	TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}

	_cfg.bMULTI	=	back;
	bQUICKOPEN	=	bquick;

	return TRUE;
}

BOOL
cBMPWND::OPEN()
{
	OPENFILENAME	ofn;

	strcpy(BMPName,"");

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	_MAIN.hWND;
	ofn.lpstrFilter		=	"tga Files {*.tga}\0*.tga\0bmp Files {*.bmp}\0*.bmp\0pcx Files {*.pcx}\0*.pcx\0\0";
	ofn.lpstrFile		=	BMPName;
	ofn.nMaxFile		=	sizeof(BMPName);
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK | OFN_ENABLETEMPLATE | OFN_EXPLORER | OFN_ENABLEINCLUDENOTIFY | OFN_ALLOWMULTISELECT;
	ofn.lpstrDefExt		=	"tga";
	ofn.hInstance		=	_MAIN.hINST;

	BOOL	isMulti,isIncludeSub;

	if	(g_iLoadPart)
	{
		if	(g_iLoadPart	!=	ePart_shadow)
			ofn.lpstrTitle		=	"캐릭터 이미지 데이터 모두 추출";
		else
			ofn.lpstrTitle		=	"캐릭터 그림자 데이터 추출";

		ofn.lpTemplateName	=	"QUICK_OPEN2";
		isMulti				=	_cfg.bMULTI;
		isIncludeSub		=	_cfg.bINCLUDESUB;
		_cfg.bMULTI			=	FALSE;
		_cfg.bINCLUDESUB	=	TRUE;
	}
	else
	if (g_isGetImageAndShadow)
	{
		ofn.lpstrTitle		=	"이미지/그림자 데이터 모두 추출";
		ofn.lpTemplateName	=	"QUICK_OPEN2";
		isMulti				=	_cfg.bMULTI;
		isIncludeSub		=	_cfg.bINCLUDESUB;
		_cfg.bMULTI			=	FALSE;
		_cfg.bINCLUDESUB	=	TRUE;
	}
	else
	{
		ofn.lpTemplateName	=	"QUICK_OPEN";

		if	(bGETSHADOW)
			ofn.lpstrTitle		=	"그림자 데이터 추출";
		else
			ofn.lpstrTitle		=	"이미지 데이터 추출";
	}

	ofn.lpfnHook		=	hookOpen;
	ofn.nFilterIndex	=	curExt;
	ofn.lpstrInitialDir	=	_cfg.BMPDir;

	bOPENFILES			=	FALSE;

	if (!GetOpenFileName(&ofn))
	{	
		bOPENFILES			=	FALSE;

		if	(g_isGetImageAndShadow || g_iLoadPart)
		{
			_cfg.bMULTI			=	isMulti;
			_cfg.bINCLUDESUB	=	isIncludeSub;
		}

		return FALSE;
	}

	if (!loadBmp(BMPName))
	{
		if (g_isGetImageAndShadow || g_iLoadPart)
		{
			_cfg.bMULTI			=	isMulti;
			_cfg.bINCLUDESUB	=	isIncludeSub;
		}
		return FALSE;
	}

	GetCurrentDirectory(255,_cfg.BMPDir);

	if (bGETSHADOW)
	{	
		_MAIN.Draw();
		_MAIN.Update();

		if (g_isGetImageAndShadow || g_iLoadPart)
		{
			_cfg.bMULTI			=	isMulti;
			_cfg.bINCLUDESUB	=	isIncludeSub;
		}

		return TRUE;
	}

	if	(hWND)
		InvalidateRect(hWND,NULL,TRUE);

	Init(_MAIN.hINST,_MAIN.hWND,&_MAIN.bmpWnd);
	cDRAW::Fill(0);
	Draw();
	_MAIN.workMode	=	dWORK_GET_IMAGE;
	lbDown			=	TRUE;

	if	(g_isGetImageAndShadow || g_iLoadPart)
	{
		_cfg.bMULTI			=	isMulti;
		_cfg.bINCLUDESUB	=	isIncludeSub;
	}

	return TRUE;
}

BOOL
cBMPWND::OPENSELECT()
{	int index	=	0,fpos=0,size=strlen(_BMPWND.BMPName),count=0;
	char fname[256];

	totalImage	=	0;
	totalFile	=	0;
	file8		=	0;
	file16		=	0;

	GetCurrentDirectory(255,_cfg.BMPDir);

	BOOL	multi	=	FALSE;
	for (index=0;index<size;index++) 
		if (_BMPWND.BMPName[index] == '"')
		{	bQUICKOPEN	=	TRUE;
			multi		=	TRUE;
			break;
		}

	if (bQUICKOPEN)
	{	ImageFolder[0]		=	0;

		if (!bGETSHADOW)
		{	if (imageFOLDERS)
			{	if (strcmp(imageFOLDERS->GetStr(0),"폴더 없음")!=0)	
					if (strcmp(imageFOLDERS->GetStr(ImageFolderCount-1),"추가된 이미지")!=0)
					{	if (imageFOLDERS->Count  > ImageFolderCount)
						{	cSTRING	*str	=	imageFOLDERS->Pick(ImageFolderCount);
							str->Set("추가된 이미지");
						}	else imageFOLDERS->Add("추가된 이미지");

						ImageFolderCount++;
					}
			}

			if (!imageFOLDERS)
			{	imageFOLDERS		=	new cSTRINGS();
				imageFOLDERS->Add("폴더 없음");
				ImageFolderCount	=	1;
			}
		}

		index =	0;
		EnableWindow(hPROGRESS	,TRUE);
		EnableWindow(hSTATUS	,TRUE);
		EnableWindow(hINFO		,TRUE);

		count=0;

		cSTRINGS	files;

		if(multi)
		{
			while(index<size-2)
			{	fpos=0;
				while(BMPName[index++]!='"');
				while(BMPName[index]!='"') fname[fpos++]=BMPName[index++];
				index++;
				fname[fpos]=NULL;
				files.Add(fname);
			}
		}
		else
		{
			files.Add(BMPName);
		}

		files.Sort();

		SendMessage(hSTATUS,WM_SETTEXT,0,(LPARAM)"이미지 변환중..");
		SendMessage( hPROGRESS, PBM_SETPOS, 0, 0 );
		SendMessage( hPROGRESS, PBM_SETRANGE, 0, MAKELPARAM(0,count-1));

		cSTRING	*str	=	files.Pick(0);

		while(str->String)
		{	if (loadBmp(str->String)) totalFile++;
			else						return	FALSE;
			SendMessage( hPROGRESS, PBM_SETPOS, index, 0 );
			SendMessage(hINFO,WM_SETTEXT,0,(LPARAM)_ms("'%s'화일을 변환중..",str->String));
			str	=	str->pNEXT;
		}

		bQUICKOPEN	=	FALSE;
		_MAIN.Draw();
		cMSG::Put("이미지 가공 완료!!",
				"총 %d개의 화일을 열어 %d개의 이미지를 생성 했습니다.\n\n 08bit 데이터 화일 : %d 개\n 16bit 데이터 화일 : %d 개",totalFile,totalImage,file8,file16);

		SendMessage( hPROGRESS, PBM_SETPOS, 0, 0 );
		EnableWindow(hPROGRESS	,FALSE);
		EnableWindow(hSTATUS	,FALSE);
		EnableWindow(hINFO		,FALSE);

		if (!bGETSHADOW)
		{	ImageFolder[ImageFolderCount]	=	cIMG::Count;
			ImageFolder[ImageFolderCount+1]	=	0;
			ImageFolder[ImageFolderCount+2]	=	cIMG::Count;
		}

		return FALSE;
	}

	return TRUE;
}

void
cBMPWND::openImageAndShadow()
{
	if (!IsFolder("image"))
	{
		cMSG::Error("이미지 로딩 에러!!","'image'라는 이름의 폴더를 찾을 수 없습니다.");
		return;
	}

	if	(!IsFolder("shadows") && !IsFolder("shadow"))
	{
		cMSG::Error("이미지 로딩 에러!!","'shadows'라는 이름의 폴더를 찾을 수 없습니다.");
		return;
	}

	SetCurrentDirectory("image");
	bGETSHADOW	=	FALSE;
	OPENALL();
	int	iFileCount	=	totalFile;
	int	iImageCount	=	totalImage;

	if	(!SetCurrentDirectory("../shadows"))
		SetCurrentDirectory("../shadow");

	bGETSHADOW	=	TRUE;

	OPENALL();
	SetCurrentDirectory("..");
	GetCurrentDirectory(255,_cfg.BMPDir);

	int	iShadowCount=	totalImage;

	iFileCount		+=	totalFile;

	cMSG::Put("이미지 가공 완료!!",
		"총 %d개의 화일을 열었습니다.\n\n 이미지 : %d 개\n 그림자 : %d 개",iFileCount,iImageCount,iShadowCount);
}

void
cBMPWND::operateLoadCharacterImage()
{
	GetCurrentDirectory(255,_cfg.BMPDir);

	char	*strPartsName[]	=	{"none","head","head","body","body","hand","hand","foot","foot","weapon","shield","end"};

	int	iJob	=	-1,i;
	int	iParts	=	-1;

	for (i=0;;i++)
	{
		if	(strstr(_cfg.BMPDir,g_strJobName[i]))
		{
			iJob	=	i;
			break;
		}

		if	(strcmp(g_strJobName[i],"end")==0)
			break;
	}

	if	(iJob	==	-1)
	{
		cMSG::Error("캐릭터 파츠 이미지 오픈 에러!!","폴더 구조 이상으로 직업을 확인 할 수 없습니다.");
		return;
	}

	char	strPartsDetailName[64];

	for (i=0;;i++)
	{
		if	(strstr(_cfg.BMPDir,strPartsName[i]))
		{
			strcpy(strPartsDetailName,strPartsName[i]);
			iParts	=	i;

			if	(iParts	<=	eBodyType_bodyF)
			{
				if	(strstr(_cfg.BMPDir,"front") )
				{
					sprintf(strPartsDetailName,"%s front",strPartsName[i]);
					iParts++;
				}
				else
					sprintf(strPartsDetailName,"%s back",strPartsName[i]);
			}
			else
			if	(iParts	<=	eBodyType_legR)
			{
				if	(strstr(_cfg.BMPDir,"right") )
				{
					sprintf(strPartsDetailName,"%s right",strPartsName[i]);
					iParts++;
				}
				else
					sprintf(strPartsDetailName,"%s left",strPartsName[i]);
			}
			break;
		}

		if	(strcmp(strPartsName[i],"end")==0)
			break;
	}

	if	(iParts	==	-1)
	{
		cMSG::Error("캐릭터 파츠 이미지 오픈 에러!!","폴더 구조 이상으로 파츠를 확인 할 수 없습니다.");
		return;
	}

	if	(cMSG::YESNO("아래 정보가 맞습니까?","직업 : %s\n\n파츠 : %s",g_strJobName[iJob],strPartsDetailName)	==	IDNO)
		return;

	g_wAnmDataType	=	0;
	g_wPartsType	=	iParts;
	g_wPlayerJob	=	iJob+1;

	OPENALL();
}

void
cBMPWND::OPENALL()
{
	int count	=	0;

	GetCurrentDirectory(255,_cfg.BMPDir);

	totalImage	=	0;
	totalFile	=	0;
	file8		=	0;
	file16		=	0;

	char	ext[10]	=	"*.tga";

	if (curExt	==	2)	strcpy(ext,"*.bmp");
	if (curExt	==	3)	strcpy(ext,"*.pcx");

	BOOL	virgin	=	FALSE;
	BOOL	folder0	=	FALSE;

	if (_cfg.bINCLUDESUB)
	{	EnableWindow(hPROGRESS	,TRUE);
		EnableWindow(hSTATUS	,TRUE);
		EnableWindow(hINFO		,TRUE);

		SendMessage(hSTATUS,WM_SETTEXT,0,(LPARAM)"하위 폴더 검색중");

		cSTRINGS	*baseFolders	=	NULL;

		if (bGETSHADOW)
		{	baseFolders			=	cFOLDER::GetFolders(_cfg.BMPDir,FALSE);
			if(!baseFolders)	
			{	baseFolders			=	new cSTRINGS();
				baseFolders->Add(_cfg.BMPDir);
			}
		}
		else
		{	if (imageFOLDERS)
			{	if (strcmp(imageFOLDERS->GetStr(0),"폴더 없음")!=0)	
					if (strcmp(imageFOLDERS->GetStr(ImageFolderCount-1),"추가된 이미지")!=0)
					{	if (imageFOLDERS->Count  > ImageFolderCount)
						{	cSTRING	*str	=	imageFOLDERS->Pick(ImageFolderCount);
							str->Set("추가된 이미지");
						}	else imageFOLDERS->Add("추가된 이미지");
						ImageFolderCount++;
					}

				baseFolders			=	new cSTRINGS();
				baseFolders->Add(_cfg.BMPDir);
			}

			if (!imageFOLDERS)
			{	imageFOLDERS		=	new cSTRINGS();
				ImageFolderCount	=	0;
				virgin				=	TRUE;
				baseFolders			=	cFOLDER::GetFolders(_cfg.BMPDir,FALSE);
			}

			if (!baseFolders)
			{	imageFOLDERS		=	new cSTRINGS();
				baseFolders			=	new cSTRINGS();
				baseFolders->Add(_cfg.BMPDir);
				ImageFolder[0]		=	0;
				folder0				=	TRUE;
			}
		}

		for (int f=0;f<baseFolders->Count;f++)
		{
			cSTRINGS	*folders			=	cFOLDER::GetFolders(baseFolders->GetStr(f));

			if (virgin && !bGETSHADOW)
			{	imageFOLDERS->Add(baseFolders->GetStr(f));
				ImageFolder[ImageFolderCount]	=	cIMG::Count;
				ImageFolderCount++;
			}

			if (!folders)	continue;

			cSTRING		*folder		=	folders->Pick(0);

			MSGOUT(folder->String);

			SetCurrentDirectory(folder->String);

			for (int i=0;i<folders->Count;i++)
			{	char cur[1024];

				GetCurrentDirectory(1024,cur);
				char	foldername[512];

				strcpy(foldername,_exportName(cur));

				if	(strcmp(foldername,"그림자"		)==0)
					continue;
				if	(strcmp(foldername,"반전 그림자")==0)
					continue;

				SendMessage(hSTATUS,WM_SETTEXT,0,(LPARAM)_ms("총 %d개의 폴더중 %d번째 폴더의 화일 변환중..",baseFolders->Count,f + 1));
				cSTRINGS*	files	=	cFOLDER::GetFiles(ext);//	화일들을 얻어 온다.

				if (files)
				{
					bQUICKOPEN	=	TRUE;
					if (!bOPENFILES)
					{
						if (!bGETSHADOW)
						{	ImageFolder[ImageFolderCount]	=	cIMG::Count;
							ImageFolder[ImageFolderCount+1]	=	0;
							ImageFolder[ImageFolderCount+2]	=	cIMG::Count;
						}

						cMSG::Put("작업이 중지 되었습니다.","총 %d개의 화일을 열어 %d개의 이미지를 생성 했습니다.\n\n 08bit 데이터 화일 : %d 개\n 16bit 데이터 화일 : %d 개",totalFile,totalImage,file8,file16);
						_MAIN.Draw();
						return;
					}

					SendMessage( hPROGRESS, PBM_SETPOS, 0, 0 );
					SendMessage( hPROGRESS, PBM_SETRANGE, 0, MAKELPARAM(0,files->Count));

					cSTRING	*file	=	files->Pick(0);

					for (int j=0;j<files->Count;j++)
					{	SendMessage( hPROGRESS, PBM_SETPOS, j, 0 );
						SendMessage(hINFO,WM_SETTEXT,0,(LPARAM)_ms("%s\\%s",cur,file->String));

						if (loadBmp(file->String))	totalFile++;
						else	return;

						file	=	file->pNEXT;
					}

					bQUICKOPEN	=	FALSE;
				}
				
				KILL(files);

				folder	=	folder->pNEXT;

				SetCurrentDirectory(folder->String);
			}
			KILL(folders);

			SetCurrentDirectory(_cfg.BMPDir);
		}

		SetCurrentDirectory(_cfg.BMPDir);

		KILL(baseFolders);

		if (folder0)
		{	cSTRING	*str	=	imageFOLDERS->Pick(0);
			str->Set("폴더 없음");
		}

		_MAIN.Draw();
		_MAIN.Update();

		if	(!g_isGetImageAndShadow || g_iLoadPart)
		{
			if (bGETSHADOW)
				cMSG::Put("이미지 가공 완료!!",
				"총 %d개의 화일을 열어 %d개의 그림자를 생성 했습니다.\n\n 현재 그림자 : %d 개\n 현재 이미지 : %d 개",totalFile,totalImage,totalImage,cIMG::Count);
			else
			cMSG::Put("이미지 가공 완료!!",
				"총 %d개의 화일을 열어 %d개의 이미지를 생성 했습니다.\n\n 08bit 데이터 화일 : %d 개\n 16bit 데이터 화일 : %d 개",totalFile,totalImage,file8,file16);
		}
		SendMessage( hPROGRESS, PBM_SETPOS, 0, 0 );
		EnableWindow(hPROGRESS	,FALSE);
		EnableWindow(hSTATUS	,FALSE);
		EnableWindow(hINFO		,FALSE);
		ImageFolder[ImageFolderCount]	=	cIMG::Count;
		ImageFolder[ImageFolderCount+1]	=	0;
		ImageFolder[ImageFolderCount+2]	=	cIMG::Count;

		return;
	}

	cSTRINGS	*files	=	cFOLDER::GetFiles(ext);

	if (files)
	{	if (!bGETSHADOW)
		{	ImageFolder[0]		=	0;

			if (imageFOLDERS)
			{	if (strcmp(imageFOLDERS->GetStr(0),"폴더 없음")!=0)	
					if (strcmp(imageFOLDERS->GetStr(ImageFolderCount-1),"추가된 이미지")!=0)
					{	if (imageFOLDERS->Count  > ImageFolderCount)
						{	cSTRING	*str	=	imageFOLDERS->Pick(ImageFolderCount);
							str->Set("추가된 이미지");
						}	else imageFOLDERS->Add("추가된 이미지");
						ImageFolderCount++;
					}
			}

			if (!imageFOLDERS)
			{	imageFOLDERS		=	new cSTRINGS();
				imageFOLDERS->Add(_cfg.BMPDir);
				ImageFolderCount	=	1;
			}
		}

		EnableWindow(hPROGRESS	,TRUE);
		EnableWindow(hSTATUS	,TRUE);
		EnableWindow(hINFO		,TRUE);

		bQUICKOPEN	=	TRUE;

		SendMessage(hSTATUS,WM_SETTEXT,0,(LPARAM)"이미지 변환중..");
		SendMessage( hPROGRESS, PBM_SETPOS, 0, 0 );
		SendMessage( hPROGRESS, PBM_SETRANGE, 0, MAKELPARAM(0,files->Count));

		cSTRING	*file	=	files->Pick(0);

		for (int i=0;i<files->Count;i++)
		{	if (loadBmp(file->String)) totalFile++;
			else						return;
			SendMessage( hPROGRESS, PBM_SETPOS, i, 0 );
			SendMessage(hINFO,WM_SETTEXT,0,(LPARAM)_ms("'%s'화일을 변환중..",file->String));
			file	=	file->pNEXT;
		}

		bQUICKOPEN	=	FALSE;

		cMSG::Put("이미지 가공 완료!!",
				"총 %d개의 화일을 열어 %d개의 이미지를 생성 했습니다.\n\n 08bit 데이터 화일 : %d 개\n 16bit 데이터 화일 : %d 개",totalFile,totalImage,file8,file16);
		SendMessage( hPROGRESS, PBM_SETPOS, 0, 0 );
		EnableWindow(hPROGRESS	,FALSE);
		EnableWindow(hSTATUS	,FALSE);
		EnableWindow(hINFO		,FALSE);

		KILL(files);

		if (!bGETSHADOW)
		{	ImageFolder[ImageFolderCount]	=	cIMG::Count;
			ImageFolder[ImageFolderCount+1]	=	0;
			ImageFolder[ImageFolderCount+2]	=	cIMG::Count;
		}
	}

	_MAIN.Draw();
	_MAIN.Update();
}

//	이미지 처리 클래스..
BOOL
cBMPWND::MakeImageRect(int x1,int y1,int x2,int y2,LPRECT rect)
{	
	int i,j;

	if	(x1 > x2 )
		swap( x1, x2);
	if	(y1 > y2 )
		swap( y1, y2);

	SetRect(rect,0,0,0,0);

	if (pBMP->p16IMAGE)
	{	
		for (j=x1;j<=x2;j++) for (i=y1;i<=y2;i++)
				if (pBMP->p16IMAGE[i*pBMP->Width+j+2]!=transColor)
				{	rect->left=j	,	j=x2+1	,	i=y2+1;
					break;
				}

		for (j=x2;j>=x1;j--) for (i=y1;i<=y2;i++)
				if (pBMP->p16IMAGE[i*pBMP->Width+j+2]!=transColor)
				{	rect->right=j	,	j=x1-1	,	i=y2+1;	
					break;
				}

		for (j=y1;j<=y2;j++) for (i=x1;i<=x2;i++)
				if (pBMP->p16IMAGE[j*pBMP->Width+i+2]!=transColor)
				{	rect->top=j		,	j=y2+1	,	i=x2+1;
					break;
				}

		for (j=y2;j>=y1;j--) for (i=x1;i<=x2;i++)
				if (pBMP->p16IMAGE[j*pBMP->Width+i+2]!=transColor)
				{	rect->bottom=j	,	j=y1-1	,	i=x2+1;
					break;
				}
	}
	else
	{	
		for (j=x1;j<=x2;j++) for (i=y1;i<=y2;i++)
				if (pBMP->p8IMAGE[i*pBMP->Width+j+4]!=transColor)
				{	rect->left=j	,	j=x2+1	,	i=y2+1;
					break;
				}

		for (j=x2;j>=x1;j--) for (i=y1;i<=y2;i++)
				if (pBMP->p8IMAGE[i*pBMP->Width+j+4]!=transColor)
				{	rect->right=j	,	j=x1-1	,	i=y2+1;	
					break;
				}

		for (j=y1;j<=y2;j++) for (i=x1;i<=x2;i++)
				if (pBMP->p8IMAGE[j*pBMP->Width+i+4]!=transColor)
				{	rect->top=j		,	j=y2+1	,	i=x2+1;
					break;
				}

		for (j=y2;j>=y1;j--) for (i=x1;i<=x2;i++)
				if (pBMP->p8IMAGE[j*pBMP->Width+i+4]!=transColor)
				{	rect->bottom=j	,	j=y1-1	,	i=x2+1;
					break;
				}
	}

	if	(rect->left == 0	&&	rect->right ==	0	&&	rect->top ==	0	&&	rect->bottom== 0)
		return FALSE;
	if	(rect->right - rect->left  +1 < 1 || rect->bottom - rect->top +1 < 1 )
		return FALSE;

	return TRUE;
}

//	이미지 처리 클래스..
BOOL
cBMPWND::CheckImageVerticalFrame()
{
	int		iWidth	=	pBMP->Width;
	int		iHeight	=	pBMP->Height;
	WORD	*lpImage=	pBMP->p16IMAGE+2;
	int		x,y;

	for (x=0;x<iWidth;x++)
	{
		int	iTop=iHeight,iBottom=0;

		for (y=0;y<iHeight;y++)
			if (lpImage[y*pBMP->Width+x]	!=	transColor)
			{
				iTop	=	y;
				break;
			}

		for (y=iHeight-1;y>=0;y--)
			if (lpImage[y*pBMP->Width+x]	!=	transColor)
			{
				iBottom	=	y;
				break;
			}

		MSGOUT("{%3d  ,%3d,%3d},",x,iTop,iBottom);
	}

	return TRUE;
}

void
cBMPWND::MakeImage(int x1,int y1,int x2,int y2)
{	
	int		i=0,j=0;
	BOOL	bfindcolor=TRUE,bfindspace=FALSE;
	int		RowCount=0,LineCount=0,Row[1000],Line[1000];

	if	(x1 > x2 )
		swap( x1, x2);
	if	(y1 > y2 )
		swap( y1, y2);

	int	xs=x2-x1+1,ys=y2-y1+1;

	if (xs<1 || ys<1)
	{	
		workMode	=	dMODE_NORMAL;
		return;
	}

//	격자 만들기..
	if (_cfg.bMULTI)
	{	
		Row	[RowCount++	]=x1;
		Line[LineCount++]=y1;

		for (i=x1;i<x2;i++)
		{	bfindcolor=TRUE;

			if (pBMP->p16IMAGE)
			{	for (j=y1;j<y2;j++)
					if(pBMP->p16IMAGE[i+j*pBMP->Width+2]!=transColor)
					{	bfindcolor	=FALSE;
						j			=y2;
						if (!bfindspace) bfindspace=TRUE;
					}
			}
			else
			{	for (j=y1;j<y2;j++)
					if(pBMP->p8IMAGE[i+j*pBMP->Width+4]!=transColor)
					{	bfindcolor	=FALSE;
						j			=y2;
						if (!bfindspace) bfindspace=TRUE;
					}
			}

			if (bfindspace&&bfindcolor)
			{	Row[RowCount++]	=i;
				bfindspace		=FALSE;
			}
		}

		Row[RowCount++]	=x2-1;
		bfindspace		=FALSE;

		for (i=y1;i<y2;i++)
		{	bfindcolor	=	TRUE;
			if (pBMP->p16IMAGE)
			{	for (j=x1;j<x2;j++)
					if(pBMP->p16IMAGE[j+i*pBMP->Width+2]!=transColor)
					{	bfindcolor=FALSE;
						if (!bfindspace) bfindspace=TRUE,j=x2;
					}
			}
			else
			{	for (j=x1;j<x2;j++)
					if(pBMP->p8IMAGE[j+i*pBMP->Width+4]!=transColor)
					{	bfindcolor=FALSE;
						if (!bfindspace) bfindspace=TRUE,j=x2;
					}
			}

			if (bfindspace&&bfindcolor)	Line[LineCount++]=i,bfindspace=FALSE;
		}

		Line[LineCount++]=y2-1;
	}
	else
		Row	[RowCount++	]=x1,Line[LineCount++]=y1,Row	[RowCount++	]=x2,Line[LineCount++]=y2;

//	격자 만들기 종료..

//	이미지 만들기..
	ImageCount=0;

	for (i=0;i<LineCount-1;i++)
		for (j=0;j<RowCount-1;j++)
			if	(MakeImageRect(Row[j],Line[i],Row[j+1],Line[i+1],&Images[ImageCount]))
				ImageCount++;

	if (ImageCount == 0 )
	{	
		if (bQUICKOPEN)
		{	BYTE	data[20];

			memset(data,0,20);

			if (pBMP->p16IMAGE)
			{	if (bGETSHADOW)	cIMG::AddShadow	((WORD *)data,0,0,0,0);
				else			cIMG::Add		((WORD *)data,0,0,0,0);
			}
			else
			{	if (bGETSHADOW)	cIMG::AddShadow	(data,0,0,0,0);
				else			cIMG::Add		(data,0,0,0,0);
			}
			totalImage++;
		}

		workMode	=	dMODE_NORMAL;
		return;
	}

	if	(bQUICKOPEN)
		GetImage();
	else
		workMode	=	dMODE_WAITBUTTON;
}

void
cBMPWND::GetImage()
{
	int i,k,xs,ys;

	static	BYTE *data	=	NULL;

	if	(!data)
		data	=	new BYTE [1024*768*3];

	for (k=0;k<ImageCount;k++)
	{	
		xs=Images[k].right -Images[k].left+1;
		ys=Images[k].bottom-Images[k].top +1;

		int addr	=	Images[k].left+Images[k].top*pBMP->Width;

		if (xs < 1 || ys < 1 )
		{
			ERRMSG("이미지가 비었습니다.","추가 하고 싶으시면... 얘기하세요. --;;");

			continue;		//	넘 작음 날려 버린다..
		}

		if (pBMP->p8IMAGE)
		{
			for (i=0;i<ys;i++,addr+=pBMP->Width)
				memcpy(data+i*xs,pBMP->p8IMAGE+addr+4,xs);

			if	(bQUICKOPEN && !_cfg.bMULTI)
			{	
				int xp,yp;
				xp=pBMP->Width /2-Images[k].left;
				yp=pBMP->Height/2-Images[k].top;

				if	(bGETSHADOW)
					cIMG::AddShadow	(data,xs,ys,xp,yp);
				else
					cIMG::Add		(data,xs,ys,xp,yp);
			}
			else
				cIMG::Add(data,xs,ys);
		}
		else
		{	
			WORD *p16data	=	(WORD *)data;

			for (i=0;i<ys;i++,addr+=pBMP->Width)
				memcpy(p16data+i*xs,pBMP->p16IMAGE+addr+2,xs*2);

			if	(bQUICKOPEN && !_cfg.bMULTI)
			{	
				int xp,yp;
				xp=pBMP->Width /2-Images[k].left;
				yp=pBMP->Height/2-Images[k].top;

				if	(bGETSHADOW)
					cIMG::AddShadow(p16data,xs,ys,xp,yp);
				else
					cIMG::Add(p16data,xs,ys,xp,yp);
			}
			else
				cIMG::Add(p16data,xs,ys);
		}

		totalImage++;

	}
	ImageCount	=	0;
	workMode	=	dMODE_NORMAL;

	if (!bQUICKOPEN)
	{	if (ImageFolder[ImageFolderCount] != cIMG::Count)
		{	if (imageFOLDERS)
			{	if (strcmp(imageFOLDERS->GetStr(0),"폴더 없음")!=0)	
					if (strcmp(imageFOLDERS->GetStr(ImageFolderCount-1),"추가된 이미지")!=0)
					{	if (imageFOLDERS->Count  > ImageFolderCount)
						{	cSTRING	*str	=	imageFOLDERS->Pick(ImageFolderCount);
							str->Set("추가된 이미지");
						}	else imageFOLDERS->Add("추가된 이미지");
						ImageFolderCount++;
					}
			}
			else
			{	imageFOLDERS	=	new cSTRINGS();
				imageFOLDERS->Add("폴더 없음");
				ImageFolder[0]	=	0;
				ImageFolderCount=	1;
			}

			ImageFolder[ImageFolderCount] = cIMG::Count;
		}
		
		_IMGWND.ViewEnd();
		_IMGWND.Draw();	
	}
}

BOOL
cBMPWND::Run()
{	static	BOOL	inWindow=TRUE;
	static	cPOINT	old,oldPos;

	if (!pBMP || !hWND) return FALSE;


	if ((keyBuff[VK_LBUTTON	] & 0x80) == 0 ) lbDown	=	FALSE;
	if ((keyBuff[VK_RBUTTON	] & 0x80) == 0 ) rbDown	=	FALSE;

	cRECT rect;

	rect.Set(0,0,min(width(),pBMP->Width),min(height(),pBMP->Height));

	Mouse.MousePos();
	if (outFrame.isIN(Mouse.x,Mouse.y))
	{	Mouse.MousePos(hWND);
		if (rect.isIN(Mouse.x,Mouse.y))		SetCursor(hCursor);
		else								SetCursor(_MAIN.defaultCursor);
	}

	Mouse.MousePos(hWND);

	if (bSPOID)
	{	if (!lbDown)
		{	if (keyBuff[VK_LBUTTON	] & 0x80 )
			{	bSPOID		=	FALSE;
				if (pBMP->p16IMAGE)	transColor	=	pBMP->p16IMAGE[Mouse.x + Mouse.y*pBMP->Width+2];
				else				transColor	=	pBMP->p8IMAGE[Mouse.x + Mouse.y*pBMP->Width+4];
				lbDown		=	TRUE;
				hCursor		=	NULL;
			}
		}

		return TRUE;
	}

	switch(workMode)
	{	case	dMODE_NORMAL	:
			if (Mouse.x < 0 || Mouse.y  < 0 || Mouse.x >= pBMP->Width || Mouse.y >= pBMP->Height )
			{	bMOVE	=	FALSE;
				if (inWindow)	Draw();
				inWindow	=	FALSE;
				return TRUE;
			}

			if (keyBuff[VK_SPACE	] & 0x80) 
			{	if (width() < pBMP->Width || height() < pBMP->Height)
				{	if (bMOVE	==	FALSE)
					{	bMOVE	=	TRUE;
						hCursor	=	_MAIN.handCursor;
						Draw();
					}
					bMOVE	=	TRUE;
				}
			}
			else
			{	if (bMOVE)	hCursor	=	NULL;
				bMOVE	=	FALSE;
				old.Set(-1,-1);
			}

			inWindow		=	TRUE;

			if (keyBuff[VK_LBUTTON] & 0x80)
			{	if (bMOVE)
				{	static int ox=0,oy=0;
					if (old.x==-1)	old=Mouse,oldPos=Mouse,ox=XPos,oy=YPos;

					if (!oldPos.match(Mouse))
					{	if (pBMP->Width > width())
						{	XPos	=	ox	-(Mouse.x-old.x);
							XPos	=	max(0,XPos);
							XPos	=	min(pBMP->Width -width() ,XPos);
						}
						if (pBMP->Height> height())
						{	YPos	=	oy	-(Mouse.y-old.y);
							YPos	=	max(0,YPos);
							YPos	=	min(pBMP->Height-height() ,YPos);
						}
						Draw();
					}
					oldPos	=	Mouse;
				}
				else
				{	if (lbDown	==	FALSE)
					{	lbDown			=	TRUE;
						ImageRect.left	=	Mouse.x+XPos;
						ImageRect.top	=	Mouse.y+YPos;
						ImageRect.right	=	Mouse.x+XPos;
						ImageRect.bottom=	Mouse.y+YPos;
						workMode		=	dMODE_RESIZEBOX;
					}
				}
			}
			else
			{	lbDown	=	FALSE;
				old.Set(-1,-1);
			}
			break;

		case	dMODE_RESIZEBOX	:
			if (Mouse.x <  0				) Mouse.x=0;
			if (Mouse.y <  0				) Mouse.y=0;
			if (Mouse.x >= pBMP->Width		) Mouse.x=pBMP->Width -1;
			if (Mouse.y >= pBMP->Height		) Mouse.y=pBMP->Height-1;
			SetCursorPos(Mouse.x+wLeft(),Mouse.y+wTop());
			if (keyBuff[VK_LBUTTON] & 0x80	) ImageRect.right = Mouse.x+XPos,ImageRect.bottom= Mouse.y+YPos;
			else
			{	ImageRect.bottom	=	min(ImageRect.bottom,pBMP->Height-1);
				ImageRect.right		=	min(ImageRect.right,pBMP->Width-1);
				MakeImage(ImageRect.left,ImageRect.top,ImageRect.right,ImageRect.bottom);
			}
			break;

		case	dMODE_WAITBUTTON	:
			if (Mouse.x <  0				) Mouse.x=0;
			if (Mouse.y <  0				) Mouse.y=0;
			if (Mouse.x >= pBMP->Width	) Mouse.x=pBMP->Width -1;
			if (Mouse.y >= pBMP->Height	) Mouse.y=pBMP->Height-1;
			SetCursorPos(Mouse.x+wLeft(),Mouse.y+wTop());
			if (lbDown || rbDown) break;
			if (keyBuff[VK_LBUTTON] & 0x80 )	
			{	GetImage();
				lbDown	=	TRUE;
				_MAIN.Update();
			}

			if (keyBuff[VK_RBUTTON] & 0x80 )	workMode	=	dMODE_NORMAL,rbDown	=	TRUE;
			break;
	}

	if (!bMOVE)		Draw();

	return TRUE;
}

void
cBMPWND::PopupMenu()
{
	return;
	CheckImageVerticalFrame();
}

LRESULT CALLBACK 
cBMPWND::WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{	case WM_SYSCOMMAND		:
			if ((LOWORD(wParam)&0xfff0) == SC_MOVE) return FALSE;
			return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
			break;

		case WM_LBUTTONDBLCLK	:
			_BMPWND.MakeImage(0,0,_BMPWND.pBMP->Width-1,_BMPWND.pBMP->Height-1);
			_BMPWND.workMode	=	dMODE_WAITBUTTON;
			lbDown				=	TRUE;
			_BMPWND.Draw();
			break;

		case WM_RBUTTONDOWN		:
//			_BMPWND.CheckImageVerticalFrame();
//			cMSG::Put("세로 외곡 프레임 검색","완료");
			break;

		case WM_CLOSE			:
		case WM_DESTROY			:
			return (0L);

		case WM_SETCURSOR		:
			break;

		case WM_VSCROLL			:
			_BMPWND.VScroll(wParam);
			break;

		case WM_HSCROLL			:
			_BMPWND.HScroll(wParam);
			break;

		default :
			return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}

#define IDB_OPEN_BMP	1000
#define IDB_QUICKOPEN	1001

UINT APIENTRY 
cBMPWND::hookOpen(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)
{	static	HWND	hWnd	=	NULL;

	switch(msg)
	{	case WM_COMMAND	:
			switch(wParam)
			{
				case IDB_OPEN_ALL	:
					bOPENFILES	=	TRUE;

					if	(g_iLoadPart)
						_BMPWND.operateLoadCharacterImage();
					else
					if	(g_isGetImageAndShadow)
						_BMPWND.openImageAndShadow();
					else
						_BMPWND.OPENALL();

					bOPENFILES	=	FALSE;
					break;

				case IDB_OPEN_BMP	:
					bOPENFILES	=	TRUE;
					GetDlgItemText(hWnd,dGET_FILE_NAME,_BMPWND.BMPName,sizeof(_BMPWND.BMPName));
					if (_BMPWND.OPENSELECT())	PostMessage(hWnd,WM_COMMAND,IDOK,0);
					bOPENFILES	=	FALSE;
					break;

				case IDB_QUICKOPEN	:
					_BMPWND.bQUICKOPEN	=	TRUE;
					bOPENFILES	=	TRUE;
					GetDlgItemText(hWnd,dGET_FILE_NAME,_BMPWND.BMPName,sizeof(_BMPWND.BMPName));
					_BMPWND.OPENSELECT();
					bOPENFILES	=	FALSE;
					break;

				case IDCANCEL		:
					EndDialog(hWnd,IDCANCEL);
					break;

				case IDC_GET_MULTI	:
					_cfg.bMULTI			=	IsDlgButtonChecked(hdlg,IDC_GET_MULTI);
					break;

//				case IDC_GET_SHADOW	:
//					bGETSHADOW			=	IsDlgButtonChecked(hdlg,IDC_GET_SHADOW);
//					break;

				case IDC_INCLUDE_SUB:
					_cfg.bINCLUDESUB	=	IsDlgButtonChecked(hdlg,IDC_INCLUDE_SUB);
					break;
			}
			break;

		case WM_NOTIFY		:
			switch( ((LPOFNOTIFY)lParam)->hdr.code)
			{	case	CDN_INITDONE	:
				{
					hWnd	=	((NMHDR*)lParam)->hwndFrom;
					DestroyWindow(GetDlgItem(hWnd,IDOK));
					DestroyWindow(GetDlgItem(hWnd,IDCANCEL));

					if (g_isGetImageAndShadow || g_iLoadPart)
					{
						_BMPWND.buttonOpen.Init("Go!!",hdlg,_MAIN.hINST,BS_DEFPUSHBUTTON,IDB_OPEN_ALL,393,162,88,26);
						_BMPWND.buttonClose.Init("취소",hdlg,_MAIN.hINST,BS_PUSHBUTTON,IDCANCEL,393,190,88,26);
					}
					else
					{
						_BMPWND.buttonOpen.Init("열기(&O)",hdlg,_MAIN.hINST,BS_PUSHBUTTON,IDB_OPEN_BMP,393,162,88,26);
						_BMPWND.buttonClose.Init("Quick Open",hdlg,_MAIN.hINST,BS_PUSHBUTTON,IDB_QUICKOPEN,393,190,88,26);

						CheckDlgButton(hdlg,IDC_INCLUDE_SUB,_cfg.bINCLUDESUB);
						CheckDlgButton(hdlg,IDC_GET_MULTI,_cfg.bMULTI);
					}
//					CheckDlgButton(hdlg,IDC_GET_SHADOW,bGETSHADOW);

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

					_BMPWND.hPROGRESS	=	GetDlgItem(hdlg,IDP_PROGRESS);
					_BMPWND.hSTATUS		=	GetDlgItem(hdlg,IDC_STATUS);
					_BMPWND.hINFO		=	GetDlgItem(hdlg,IDC_INFO);
					EnableWindow(_BMPWND.hPROGRESS	,FALSE);
					EnableWindow(_BMPWND.hSTATUS	,FALSE);
					EnableWindow(_BMPWND.hINFO		,FALSE);
					break;
				}
				case	CDN_SELCHANGE	:
				{	_BMPWND.curExt	=	((LPOFNOTIFY)lParam)->lpOFN->nFilterIndex;
					break;
				}
			}
			break;

		default:
			return FALSE;

	}

	return FALSE;
}