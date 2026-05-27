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
	bQUICKOPEN	=	TRUE;
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
{	if (bGETSHADOW)	return loadShadow(fname);

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
	{	if (bQUICKOPEN)	return MSGOUT("Error in cBMPWND::loadBmp - '%s' 화일이 %dbit 이미지 데이터가 아닙니다.",fname,cANM::BPP);
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
	{	transColor	=	pBMP->p8IMAGE[4];
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

	if (bQUICKOPEN)
	{	MakeImage(0,0,pBMP->Width-1,pBMP->Height-1);
		cIMG::SetCURRENT(0);
		MSG msg;

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

	if (!((cIMAGE *)temp)->Load(fname))	return FALSE;

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
{	OPENFILENAME	ofn;

	strcpy(BMPName,"");

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	_MAIN.hWND;
	ofn.lpstrFilter		=	"tga Files {*.tga}\0*.tga\0bmp Files {*.bmp}\0*.bmp\0pcx Files {*.pcx}\0*.pcx\0\0";
	ofn.lpstrFile		=	BMPName;
	ofn.nMaxFile		=	sizeof(BMPName);
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK | OFN_ENABLETEMPLATE | OFN_EXPLORER | OFN_ENABLEINCLUDENOTIFY | OFN_ALLOWMULTISELECT;
	ofn.lpTemplateName	=	"QUICK_OPEN";
	ofn.lpstrDefExt		=	"tga";
	ofn.hInstance		=	_MAIN.hINST;
	ofn.lpstrTitle		=	"이미지 화일 불러오기.";
	ofn.lpfnHook		=	hookOpen;
	ofn.nFilterIndex	=	curExt;
	ofn.lpstrInitialDir	=	_cfg.BMPDir;

	if (!GetOpenFileName(&ofn)) return FALSE;
	if (!loadBmp(BMPName)) return FALSE;

	GetCurrentDirectory(255,_cfg.BMPDir);

	if (bGETSHADOW)
	{	_MAIN.Draw();
		_MAIN.Update();
		return TRUE;
	}

	if (hWND) InvalidateRect(hWND,NULL,TRUE);
	Init(_MAIN.hINST,_MAIN.hWND,&_MAIN.bmpWnd);
	cDRAW::Fill(0);
	Draw();
	_MAIN.workMode	=	dWORK_GET_IMAGE;
	lbDown			=	TRUE;

	return TRUE;
}

BOOL
cBMPWND::OPENSELECT()
{	int index	=	0,fpos=0,size=strlen(_BMPWND.BMPName),count=0;
	char fname[256];

	if (size == 0) return FALSE;

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
	{	index =	0;
		count=0;

		cSTRINGS	files;

		if (multi)
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
		else	files.Add(BMPName);

		files.Sort();

		cSTRING	*str	=	files.Pick(0);

		while(str->String)
		{	if (loadBmp(str->String)) totalFile++;
			str	=	str->pNEXT;
		}

		_MAIN.Draw();

		if (totalImage	>	1)
		cMSG::Put("이미지 가공 완료!!",
				"총 %d개의 화일을 열어 %d개의 이미지를 생성 했습니다.\n\n 08bit 데이터 화일 : %d 개\n 16bit 데이터 화일 : %d 개",totalFile,totalImage,file8,file16);

		return FALSE;
	}

	return TRUE;
}

void
cBMPWND::OPENALL()
{	int count	=	0;
	GetCurrentDirectory(255,_cfg.BMPDir);

	totalImage	=	0;
	totalFile	=	0;
	file8		=	0;
	file16		=	0;

	char	ext[10]	=	"*.tga";

	if (curExt	==	2)	strcpy(ext,"*.bmp");
	if (curExt	==	3)	strcpy(ext,"*.pcx");

	if (_cfg.bINCLUDESUB)
	{	EnableWindow(hPROGRESS	,TRUE);
		EnableWindow(hSTATUS	,TRUE);
		EnableWindow(hINFO		,TRUE);

		SendMessage(hSTATUS,WM_SETTEXT,0,(LPARAM)"하위 폴더 검색중");

		cSTRINGS	*folders	=	cFOLDER::GetFolders(_cfg.BMPDir);

		if (!folders)	return;

		cSTRING	*folder		=	folders->Pick(0);

		for (int i=0;i<folders->Count;i++)
		{	char cur[256];
			GetCurrentDirectory(255,cur);
			MSGOUT("******* Folder -> %s *********",cur);
			SendMessage(hSTATUS,WM_SETTEXT,0,(LPARAM)_ms("총 %d개의 폴더중 %d번째 폴더의 화일 변환중..",folders->Count,count++ + 1));
			cSTRINGS*	files	=	cFOLDER::GetFiles(ext);//	화일들을 얻어 온다.

			if (files)
			{	bQUICKOPEN	=	TRUE;

				SendMessage( hPROGRESS, PBM_SETPOS, 0, 0 );
				SendMessage( hPROGRESS, PBM_SETRANGE, 0, MAKELPARAM(0,files->Count));

				cSTRING	*file	=	files->Pick(0);

				for (int j=0;j<files->Count;j++)
				{	SendMessage( hPROGRESS, PBM_SETPOS, j, 0 );
					SendMessage(hINFO,WM_SETTEXT,0,(LPARAM)_ms("%s\\%s",cur,file->String));
					if (loadBmp(file->String))	totalFile++;
					MSGOUT("File : %s",file->String);
					file	=	file->pNEXT;
				}

				bQUICKOPEN	=	FALSE;
			} else MSGOUT("File not found");

			
			KILL(files);

			folder	=	folder->pNEXT;

			SetCurrentDirectory(folder->String);
		}

		KILL(folders);

		SetCurrentDirectory(_cfg.BMPDir);
		cMSG::Put("이미지 가공 완료!!",
			"총 %d개의 화일을 열어 %d개의 이미지를 생성 했습니다.\n\n 08bit 데이터 화일 : %d 개\n 16bit 데이터 화일 : %d 개",totalFile,totalImage,file8,file16);
		SendMessage( hPROGRESS, PBM_SETPOS, 0, 0 );
		EnableWindow(hPROGRESS	,FALSE);
		EnableWindow(hSTATUS	,FALSE);
		EnableWindow(hINFO		,FALSE);
		_MAIN.Draw();
		_MAIN.Update();
		return;
	}

	cSTRINGS	*files	=	cFOLDER::GetFiles(ext);

	if (files)
	{	EnableWindow(hPROGRESS	,TRUE);
		EnableWindow(hSTATUS	,TRUE);
		EnableWindow(hINFO		,TRUE);

		bQUICKOPEN	=	TRUE;

		SendMessage(hSTATUS,WM_SETTEXT,0,(LPARAM)"이미지 변환중..");
		SendMessage( hPROGRESS, PBM_SETPOS, 0, 0 );
		SendMessage( hPROGRESS, PBM_SETRANGE, 0, MAKELPARAM(0,files->Count));

		cSTRING	*file	=	files->Pick(0);

		for (int i=0;i<files->Count;i++)
		{	if (loadBmp(file->String)) totalFile++;
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
	}

	_MAIN.Draw();
	_MAIN.Update();

}

//	이미지 처리 클래스..
BOOL
cBMPWND::MakeImageRect(int x1,int y1,int x2,int y2,LPRECT rect)
{	int i,j;

	if (x1 > x2 ) swap( x1, x2);
	if (y1 > y2 ) swap( y1, y2);

	SetRect(rect,0,0,0,0);

	if (pBMP->p16IMAGE)
	{	for (j=x1;j<=x2;j++) for (i=y1;i<=y2;i++)
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
	{	for (j=x1;j<=x2;j++) for (i=y1;i<=y2;i++)
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

	if(rect->left == rect->right || rect->top == rect->bottom) return FALSE;
	if (rect->right - rect->left  +1 < 1 || rect->bottom - rect->top +1 < 1 ) return FALSE;

	return TRUE;
}


void
cBMPWND::MakeImage(int x1,int y1,int x2,int y2)
{	int		i=0,j=0;
	BOOL	bfindcolor=TRUE,bfindspace=FALSE;
	int		RowCount=0,LineCount=0,Row[1000],Line[1000];

	if (x1 > x2 ) swap( x1, x2);
	if (y1 > y2 ) swap( y1, y2);

	int	xs=x2-x1+1,ys=y2-y1+1;

	if (xs<1 || ys<1)
	{
		workMode	=	dMODE_NORMAL;
		return;
	}

	Row	[RowCount++	]=x1,Line[LineCount++]=y1,Row	[RowCount++	]=x2,Line[LineCount++]=y2;

//	격자 만들기 종료..

//	이미지 만들기..
	ImageCount=0;

	for (i=0;i<LineCount-1;i++)
		for (j=0;j<RowCount-1;j++)
			if (MakeImageRect(Row[j],Line[i],Row[j+1],Line[i+1],&Images[ImageCount]))	ImageCount++;

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

	if (bQUICKOPEN)	GetImage();
	else			workMode	=	dMODE_WAITBUTTON;
}

void
cBMPWND::GetImage()
{	int i,k,xs,ys;

	for (k=0;k<ImageCount;k++)
	{	xs=Images[k].right -Images[k].left+1;
		ys=Images[k].bottom-Images[k].top +1;
		int addr=Images[k].left+Images[k].top*pBMP->Width;

		if (xs < 1 || ys < 1 )
		{	cMSG::Put("1","1");

			continue;		//	넘 작음 날려 버린다..
		}

		if (pBMP->p8IMAGE)
		{	BYTE *data	=	new BYTE [xs*ys+100];

			if (!data) continue;

			for (i=0;i<ys;i++,addr+=pBMP->Width)	memcpy(data+i*xs,pBMP->p8IMAGE+addr+4,xs);

			if (bQUICKOPEN)
			{
				int xp,yp;
				xp=pBMP->Width /2-Images[k].left;
				yp=pBMP->Height/2-Images[k].top;

				if (bGETSHADOW)	cIMG::AddShadow	(data,xs,ys,xp,yp);
				else			cIMG::Add		(data,xs,ys,xp,yp);
			}
			else	cIMG::Add(data,xs,ys);

			delete [] data;
		}
		else
		{	WORD *data	=	new WORD [xs*ys];

			if (!data) continue;

			for (i=0;i<ys;i++,addr+=pBMP->Width)	memcpy(data+i*xs,pBMP->p16IMAGE+addr+2,xs*2);

			if (bQUICKOPEN)
			{	int xp,yp;
				xp=pBMP->Width /2-Images[k].left;
				yp=pBMP->Height/2-Images[k].top;

				if (bGETSHADOW)	cIMG::AddShadow(data,xs,ys,xp,yp);
				else			cIMG::Add(data,xs,ys,xp,yp);
			}
			else	cIMG::Add(data,xs,ys);

			delete [] data;
		}

		totalImage++;

	}

	ImageCount	=	0;
	workMode	=	dMODE_NORMAL;

	_IMGWND.ViewEnd();
	_IMGWND.Draw();	
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
#define IDB_CANCEL		1001

UINT APIENTRY 
cBMPWND::hookOpen(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)
{	static	HWND	hWnd	=	NULL;

	switch(msg)
	{	case WM_COMMAND	:
			switch(wParam)
			{	case IDB_OPEN_BMP	:
					bGETSHADOW	=	FALSE;
					GetDlgItemText(hWnd,dGET_FILE_NAME,_BMPWND.BMPName,sizeof(_BMPWND.BMPName));
					if (_BMPWND.OPENSELECT())	PostMessage(hWnd,WM_COMMAND,IDOK,0);
					break;

				case IDB_CANCEL		:
					_MAIN.Draw();
					EndDialog(hWnd,IDCANCEL);
					break;

				case IDM_OPEN_SHADOW	:
					bGETSHADOW			=	TRUE;
					GetDlgItemText(hWnd,dGET_FILE_NAME,_BMPWND.BMPName,sizeof(_BMPWND.BMPName));
					if (_BMPWND.OPENSELECT())	PostMessage(hWnd,WM_COMMAND,IDOK,0);
					break;

				case IDC_QUICK_LOAD	:
					_BMPWND.bQUICKOPEN	=	IsDlgButtonChecked(hdlg,IDC_QUICK_LOAD);
					break;
			}
			break;


		case WM_NOTIFY		:
/*
			switch( ((LPOFNOTIFY)lParam)->hdr.code)
			{	case	CDN_INITDONE	:
				{	hWnd	=	((NMHDR*)lParam)->hwndFrom;
					DestroyWindow(GetDlgItem(hWnd,IDOK));

					_BMPWND.buttonOpen.Init("열기(&O)",hdlg,_MAIN.hINST,BS_DEFPUSHBUTTON,IDB_OPEN_BMP,389,162,88,26);

					CheckDlgButton(hdlg,IDC_INCLUDE_SUB,_cfg.bINCLUDESUB);
					CheckDlgButton(hdlg,IDC_GET_MULTI,_cfg.bMULTI);
					CheckDlgButton(hdlg,IDC_GET_SHADOW,bGETSHADOW);

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
*/
			switch( ((LPOFNOTIFY)lParam)->hdr.code)
			{	case	CDN_INITDONE	:
				{	hWnd	=	((NMHDR*)lParam)->hwndFrom;
					DestroyWindow(GetDlgItem(hWnd,IDOK));
					DestroyWindow(GetDlgItem(hWnd,IDCANCEL));

					_BMPWND.buttonOpen.Init("열기(이미지)",hdlg,_MAIN.hINST,BS_DEFPUSHBUTTON,IDB_OPEN_BMP,393,162,88,26);
					_BMPWND.buttonClose.Init("Cancel",hdlg,_MAIN.hINST,0,IDB_CANCEL,393,190,88,26);

					CheckDlgButton(hdlg,IDC_INCLUDE_SUB,_cfg.bINCLUDESUB);
					CheckDlgButton(hdlg,IDC_QUICK_LOAD,_BMPWND.bQUICKOPEN);

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