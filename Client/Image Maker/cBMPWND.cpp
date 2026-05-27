#include "cBMPWND.H"
#include "cANM.H"
#include "cIMGWND.H"
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


//
//	화일 입출력 클래스
BOOL
cBMPWND::loadBmp(char *fname)
{
	void	*temp;

	if (stricmp(_exportExt(fname),"tga")==0)	temp	=	new cTGA();
	else
	if (stricmp(_exportExt(fname),"bmp")==0)	temp	=	new cBMP();
	else
	if (stricmp(_exportExt(fname),"pcx")==0)	temp	=	new cPCX();
	else	return FALSE;

	if (!((cIMAGE *)temp)->Load(fname,FALSE))	return FALSE;

	if (((cIMAGE *)temp)->BPP	==	24	)	((cIMAGE *)temp)->BPP=16;

	KILL(pBMP);
	pBMP		=	(cIMAGE *)temp;

	if (!pBMP->p16IMAGE && !pBMP->p8IMAGE)	return	FALSE;

	MSG	msg;

	if (pBMP->BPP	==	8)
	{
		memcpy(cANM::s_awPlt,pBMP->pPLT,512);

		if (_cfg.bMULTI)
		{
			int	iXC,iYC,iX,iY,iCount=0;

			iXC	=	pBMP->Width/_cfg.m_wImageWidth;
			iYC	=	pBMP->Height/_cfg.m_wImageHeight;

			BYTE	*pBuffer	=	new BYTE [_cfg.m_wImageWidth*_cfg.m_wImageHeight+4];
			char	strName[256];
			int		iBufferAddress,iImageAddress;

			*(WORD *)pBuffer		=	_cfg.m_wImageWidth;
			*(WORD *)(pBuffer+2)	=	_cfg.m_wImageHeight;

			for (iY=0;iY<iYC;iY++)
			{
				for (iX=0;iX<iXC;iX++)
				{
					for (int i=0;i<_cfg.m_wImageHeight;i++)
					{
						iBufferAddress	=	4+i*_cfg.m_wImageWidth;
						iImageAddress	=	4+(iY*_cfg.m_wImageHeight*pBMP->Width)+(iX*_cfg.m_wImageWidth)+i*pBMP->Width;

						memcpy(pBuffer+iBufferAddress,pBMP->p8IMAGE+iImageAddress,_cfg.m_wImageWidth);
					}

					sprintf(strName,"%s%.4d",_exportFileName(fname),iCount);
					cIMG::AddImage(strName,pBuffer,_cfg.m_wImageWidth,_cfg.m_wImageHeight);
					iCount++;
				}
			}

			pKILL(pBuffer);
		}
		else	cIMG::AddImage(_exportFileName(fname),pBMP->p8IMAGE,pBMP->Width,pBMP->Height);
	}
	else
	{
		if (_cfg.bMULTI)
		{
			int	iXC,iYC,iX,iY,iCount=0;

			iXC	=	pBMP->Width/_cfg.m_wImageWidth;
			iYC	=	pBMP->Height/_cfg.m_wImageHeight;

			WORD	*pBuffer	=	new WORD [_cfg.m_wImageWidth*_cfg.m_wImageHeight+2];
			char	strName[256];
			int		iBufferAddress,iImageAddress;

			pBuffer[0]	=	_cfg.m_wImageWidth;
			pBuffer[1]	=	_cfg.m_wImageHeight;

			for (iY=0;iY<iYC;iY++)
			{
				for (iX=0;iX<iXC;iX++)
				{
					for (int i=0;i<_cfg.m_wImageHeight;i++)
					{
						iBufferAddress	=	2+i*_cfg.m_wImageWidth;
						iImageAddress	=	2+(iY*_cfg.m_wImageHeight*pBMP->Width)+(iX*_cfg.m_wImageWidth)+i*pBMP->Width;

						memcpy(pBuffer+iBufferAddress,pBMP->p16IMAGE+iImageAddress,_cfg.m_wImageWidth*2);
					}

					sprintf(strName,"%s%.4d",_exportFileName(fname),iCount);
					cIMG::AddImage(strName,pBuffer,_cfg.m_wImageWidth,_cfg.m_wImageHeight);
					iCount++;
				}
			}

			pKILL(pBuffer);
		}
		else	cIMG::AddImage(_exportFileName(fname),pBMP->p16IMAGE,pBMP->Width,pBMP->Height);
	}

	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{	TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}

	_MAIN.Update();

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
	ofn.lpstrDefExt		=	"tga";
	ofn.hInstance		=	_MAIN.hINST;
	ofn.lpTemplateName	=	"QUICK_OPEN";
	ofn.lpstrTitle		=	"이미지 데이터 추출";

	ofn.lpfnHook		=	hookOpen;
	ofn.nFilterIndex	=	curExt;
	ofn.lpstrInitialDir	=	_cfg.BMPDir;

	bOPENFILES			=	FALSE;

	if (!GetOpenFileName(&ofn))
	{	bOPENFILES			=	FALSE;
		return FALSE;
	}

	if (!loadBmp(BMPName))	return FALSE;

	GetCurrentDirectory(255,_cfg.BMPDir);

	if (hWND) InvalidateRect(hWND,NULL,TRUE);

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

	BOOL	virgin	=	FALSE;
	BOOL	folder0	=	FALSE;

	if (_cfg.bINCLUDESUB)
	{	EnableWindow(hPROGRESS	,TRUE);
		EnableWindow(hSTATUS	,TRUE);
		EnableWindow(hINFO		,TRUE);

		SendMessage(hSTATUS,WM_SETTEXT,0,(LPARAM)"하위 폴더 검색중");

		cSTRINGS	*baseFolders	=	NULL;
		baseFolders			=	new cSTRINGS();
		baseFolders->Add(_cfg.BMPDir);

		for (int f=0;f<baseFolders->Count;f++)
		{	cSTRINGS	*folders			=	cFOLDER::GetFolders(baseFolders->GetStr(f));

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

		if (!g_isGetImageAndShadow)
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
	{
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

		SendMessage( hPROGRESS, PBM_SETPOS, 0, 0 );
		EnableWindow(hPROGRESS	,FALSE);
		EnableWindow(hSTATUS	,FALSE);
		EnableWindow(hINFO		,FALSE);

		KILL(files);
	}

	_MAIN.Draw();
	_MAIN.Update();

}

BOOL
cBMPWND::Run()
{
	return TRUE;
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
				{
					BOOL	temp;

					_cfg.m_wImageWidth	=	GetDlgItemInt(hdlg,IDC_XS,&temp,FALSE);
					_cfg.m_wImageHeight	=	GetDlgItemInt(hdlg,IDC_YS,&temp,FALSE);

					bOPENFILES	=	TRUE;
					_BMPWND.OPENALL();
					bOPENFILES	=	FALSE;
					break;
				}
				
				case IDB_OPEN_BMP	:
				{
					BOOL	temp;

					_cfg.m_wImageWidth	=	GetDlgItemInt(hdlg,IDC_XS,&temp,FALSE);
					_cfg.m_wImageHeight	=	GetDlgItemInt(hdlg,IDC_YS,&temp,FALSE);

					bOPENFILES	=	TRUE;
					GetDlgItemText(hWnd,dGET_FILE_NAME,_BMPWND.BMPName,sizeof(_BMPWND.BMPName));
					if (_BMPWND.OPENSELECT())	PostMessage(hWnd,WM_COMMAND,IDOK,0);
					_MAIN.Draw();
					bOPENFILES	=	FALSE;
					break;
				}

				case IDB_QUICKOPEN	:
				{
					BOOL	temp;

					_cfg.m_wImageWidth	=	GetDlgItemInt(hdlg,IDC_XS,&temp,FALSE);
					_cfg.m_wImageHeight	=	GetDlgItemInt(hdlg,IDC_YS,&temp,FALSE);

					_BMPWND.bQUICKOPEN	=	TRUE;
					bOPENFILES	=	TRUE;
					GetDlgItemText(hWnd,dGET_FILE_NAME,_BMPWND.BMPName,sizeof(_BMPWND.BMPName));
					_BMPWND.OPENSELECT();
					bOPENFILES	=	FALSE;
					break;
				}

				case IDCANCEL			:
					EndDialog(hWnd,IDCANCEL);
					break;


				case IDC_INCLUDE_SUB	:
					_cfg.bINCLUDESUB	=	IsDlgButtonChecked(hdlg,IDC_INCLUDE_SUB);
					break;

				case IDC_MULTI_IMAGE	:
					_cfg.bMULTI			=	IsDlgButtonChecked(hdlg,IDC_MULTI_IMAGE);
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

					{
						_BMPWND.buttonOpen.Init("열기(&O)",hdlg,_MAIN.hINST,BS_PUSHBUTTON,IDB_OPEN_BMP,393,162,88,26);
						_BMPWND.buttonClose.Init("취소",hdlg,_MAIN.hINST,BS_PUSHBUTTON,IDCANCEL,393,190,88,26);

						CheckDlgButton(hdlg,IDC_INCLUDE_SUB,_cfg.bINCLUDESUB);
						CheckDlgButton(hdlg,IDC_MULTI_IMAGE,_cfg.bMULTI);
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

					_cfg.m_wImageWidth	=	SetDlgItemInt(hdlg,IDC_XS,34,FALSE);
					_cfg.m_wImageHeight	=	SetDlgItemInt(hdlg,IDC_YS,34,FALSE);

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
}	//	cBMPWND::hookOpen(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)