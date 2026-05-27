//zemper07@nownuri.net
//나무모
#include <stdio.h>

#include "cMAIN.H"
#include "cMESSAGE.H"
#include "cDRAW.H"
#include "cTEXT.H"
#include "cPICTURE.H"
#include <mmsystem.h>

cWND		*cMAIN::pWND			=	NULL;
cDIBWND		*cMAIN::pDIBWND			=	NULL;

cLISTVIEW	*cMAIN::hLIST			=	NULL;
cSTRINGS	*cMAIN::pSTRINGS		=	NULL;

HWND		cMAIN::hWND				=	NULL;
HWND		cMAIN::hPROGRESS		=	NULL;

HINSTANCE	cMAIN::hINST;
int			cMAIN::WIDTH			=	0;
int			cMAIN::HEIGHT			=	0;
DWORD		cMAIN::TotalSize		=	0;
char		cMAIN::ArcFile[512];
char		cMAIN::SmiFolder[512];
char		cMAIN::ArcFolder[512]	=	{0,};
char		cMAIN::ExtractFolder[512]=	{0,};
int			cMAIN::Work				=	0;

#ifdef	_FOR_ENG 
char		l_strProgressDlg[]		=	"PROGRESS_E";
static char	*lpszAppName			=	"NUX_E";
static char	*lpszTitle				=	"Patch Manager - Nux -";
#else
char		l_strProgressDlg[]		=	"PROGRESS";
static char	*lpszAppName			=	"NUX";
static char	*lpszTitle				=	"데이터 관리 메니저 - Nux -";
#endif

cRECT_SIZE	outFrame;

BOOL		bRUNAPPLICATION	=	TRUE,bACTIVEAPP	=	FALSE;

BOOL
cMAIN::Init(HINSTANCE hInst)
{	if (FindWindow(lpszAppName,lpszTitle))	return FALSE;

	pWND	=	new cWND();

	outFrame.Set(100,100,400,120);

	Init();

	if (!pWND->Init(0,lpszAppName,lpszTitle,WS_OVERLAPPED|WS_SYSMENU|WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_THICKFRAME,
					outFrame.x,outFrame.y,outFrame.xs,outFrame.ys,
					NULL,NULL,hInst,NULL,
					SW_SHOW,
					(WNDPROC)WNDProc,(HBRUSH)(GetStockObject( GRAY_BRUSH ))
					)
		)
		return FALSE;

	hWND	=	pWND->hWND;
	hINST	=	hInst;

	hLIST	=	new cLISTVIEW();
	pSTRINGS=	new cSTRINGS();

	hLIST->Init(hWND,hINST,dIDM_LISTBOX ,0,0,pWND->ClientWidth(),pWND->ClientHeight(),
				LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|WS_VSCROLL,
				WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_BORDER | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER);

	hLIST->AddImage("FOLDER");
	hLIST->AddImage("FILE");

#ifdef	_FOR_ENG
	hLIST->AddColumn(0,140,"File");
	hLIST->AddColumn(1,420,"Directory");
#else
	hLIST->AddColumn(0,140,"화일");
	hLIST->AddColumn(1,420,"경로");
#endif

	DragAcceptFiles(hWND,TRUE);			//	드래그 화일을 받는다.

	Resize();

	return TRUE;
}

BOOL
cMAIN::Init()
{	FILE *fp;

	fp=fopen("nux.cfg","rb");

	if (!fp) return FALSE;

	fread(&outFrame,sizeof(cRECT_SIZE),1,fp);
	fread(ArcFolder,sizeof(ArcFolder),1,fp);
	fread(ExtractFolder,sizeof(ExtractFolder),1,fp);

	fclose(fp);

	return TRUE;
}

BOOL
cMAIN::Exit()
{	if (!bRUNAPPLICATION) return FALSE;

	bRUNAPPLICATION	=	FALSE;

	SetCurrentDirectory(_logFolder);

	FILE *fp;

	fp=fopen("nux.cfg","wb");

	if (fp)
	{	fwrite(&outFrame,sizeof(cRECT_SIZE),1,fp);
		fwrite(ArcFolder,sizeof(ArcFolder),1,fp);
		fwrite(ExtractFolder,sizeof(ExtractFolder),1,fp);

		fclose(fp);
	}

	PostQuitMessage(0);

	return TRUE;
}

/*
선택한 화일들을 이미지로 저장한다.. 쓰읍 이게 왜 여기에.. --;
*/

BOOL
cMAIN::SaveAsImage()
{	char	fn[256];
	int		i;

	fn[0] = 0;

	if (!pSTRINGS			) return FALSE;
	if (pSTRINGS->Count < 1	) return FALSE;
	pSTRINGS->Sort();

	OPENFILENAME	ofn;

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize	=	sizeof(OPENFILENAME);
	ofn.hwndOwner	=	hWND;
	ofn.nMaxFile	=	sizeof(fn);
	ofn.lpstrFile	=	fn;
	ofn.Flags		=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	ofn.lpstrFilter =	"Sonaki Merged Image data file. {*.smi}\0*.smi\0";
	ofn.lpstrDefExt	=	"smi";
	ofn.lpstrTitle	=	"그림 화일들을 합쳐진 이미지 화일로 생성합니다.";


	SetCurrentDirectory(SmiFolder);
	if (!GetSaveFileName(&ofn)) return FALSE;
	GetCurrentDirectory(sizeof(SmiFolder),SmiFolder);

	TotalSize	=	0;

	for (i=0;i<pSTRINGS->Count;i++)	TotalSize	+=	cFILE::Size(pSTRINGS->GetStr(i));

	cBMP	*bmp	=	NULL;

	//	좀 위험한 코드 나중에 수정할것
	cFILE	file;
	DWORD	*pLocate;
	int		iCount	=	0,iAllSize	=	0;

	pLocate	=	new DWORD [pSTRINGS->Count+1];
	
	if (!file.Open(fn,"wb")) return FALSE;

	file.Write(&pSTRINGS->Count,4);
	file.Write(&pSTRINGS->Count,4);

	for (i=0;i<pSTRINGS->Count;i++)	file.Write(&i,4);

	cBMP	imgBmp;

	for (i=0;i<pSTRINGS->Count;i++)
	{	
		if (imgBmp.Load(pSTRINGS->GetStr(i)))
		{
			pLocate[iCount]	=	file.GetPos();
			WORD	iWidth	=	imgBmp.Width;
			WORD	iHeight	=	imgBmp.Height;
			file.Write(&iWidth,2);
			file.Write(&iHeight,2);
			file.Write(imgBmp.p16IMAGE,imgBmp.Height*imgBmp.Width*2);

			iAllSize	+=	2+imgBmp.Height*imgBmp.Width;

			iCount++;
		}
	}

	file.Rewind();

	file.Write(&iCount,4);
	file.Write(&iAllSize,4);
	file.Write(pLocate,4*iCount);

	file.Close();

	cMSG::Put("이미지 화일이 완성 되었습니다.","[%s]폴더에\n\n\"%s\"화일을 생성 하였습니다.\n\n%d개의 이미지를 병합 하였습니다.",
											SmiFolder,_exportFileName(fn),pSTRINGS->Count);

	SetCurrentDirectory(_logFolder);

//	Work		=	dWORK_NOTHING;

	return TRUE;
}

BOOL
cMAIN::Run()			//	윈도우가 활성화 중일때 계속 업데이트 시켜준다.
{	if (!bRUNAPPLICATION) return FALSE;

	return TRUE;
}

void
cMAIN::New()
{	
#ifdef	_FOR_ENG
	if	(cMSG::YESNO("Reset the list","Continue?")==IDNO)
		return;
#else
	if	(cMSG::YESNO("리스트를 초기화 시킵니다.","계속하시겠습니까?")==IDNO)
		return;
#endif

	pSTRINGS->Reset();
	hLIST->Reset();
}


BOOL
cMAIN::Add(char *name)
{	if (!pSTRINGS->Add(name,TRUE)) return FALSE;

	if (IsFolder(name))
	{	int index = hLIST->AddItem(0,_exportFileName(name),0,0xffff);
		hLIST->SetItem(index,1,_exportFolderName(name));
	}
	else
	{	int index = hLIST->AddItem(0,_exportFileName(name),1);
		hLIST->SetItem(index,1,_exportFolderName(name));
	}

	hLIST->Sort(0,dSORT_TEXT,ASC);

	return TRUE;
}

BOOL
cMAIN::Remove(int index)
{	char *name	 = hLIST->GetText(index,0);

	if (!name) return FALSE;

	char	fullName[512];

	strcpy(fullName,hLIST->GetText(index,1));
	strcpy(fullName,_ms("%s\\%s",fullName,hLIST->GetText(index,0)));
	
	if (!  pSTRINGS->Remove(fullName)   ) return FALSE;

	hLIST->DelItem(index);

	return TRUE;
}

/*****************************************************************************
								압축하기
*****************************************************************************/
BOOL
cMAIN::Press(BOOL append,BOOL merge)
{	ArcFile[0] = 0;

	if (!pSTRINGS			) return FALSE;
	if (pSTRINGS->Count < 1	) return FALSE;

	OPENFILENAME	ofn;

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize	=	sizeof(OPENFILENAME);
	ofn.hwndOwner	=	hWND;
	ofn.nMaxFile	=	sizeof(ArcFile);
	ofn.lpstrFile	=	ArcFile;
	ofn.Flags		=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

#ifdef	_FOR_ENG
	if (append)
	{
		ofn.Flags		=	OFN_HIDEREADONLY;
		ofn.lpstrFilter =	"Attach data to .exe file {*.exe}\0*.exe\0Attach data to fiel {*.*}\0*.*\n\n";
		ofn.lpstrDefExt	=	"exe";
		ofn.lpstrTitle	=	"compress data and attatch to select file";
	}
	else
	{
		ofn.lpstrFilter =	"NUX Pressed Data File. {*.nux}\0*.nux\0";
		ofn.lpstrDefExt	=	"nux";
		ofn.lpstrTitle	=	"compress data and build nux file";
	}
#else
	if (append)
	{
		ofn.Flags		=	OFN_HIDEREADONLY;
		ofn.lpstrFilter =	"실행 화일 뒤에 붙이기 {*.exe}\0*.exe\0임의의 화일 뒤에 붙이기. {*.*}\0*.*\n\n";
		ofn.lpstrDefExt	=	"exe";
		ofn.lpstrTitle	=	"화일들을 압축하여 화일 뒤에 붙입니다.";
	}
	else
	{
		ofn.lpstrFilter =	"NUX Pressed Data File. {*.nux}\0*.nux\0";
		ofn.lpstrDefExt	=	"nux";
		ofn.lpstrTitle	=	"화일들을 압축하여 Nux 화일을 생성합니다.";
	}
#endif

	SetCurrentDirectory(ArcFolder);
	if (!GetSaveFileName(&ofn)) return FALSE;

	GetCurrentDirectory(sizeof(ArcFolder),ArcFolder);

	if (append)
	{	cFILE	file;

		file.Open(ArcFile,"rb");

		if	(file.Length	>=	10*1024*1024)
		{
			file.Close();

			cMSG::Error("Error!!","Core file size too big!!\n\nOver 10M");

			return	FALSE;
		}

		strcpy(ArcFile,_ms("%s_",ArcFile));
		file.Copy(ArcFile);
		file.Close();


	}


	TotalSize	=	0;
	int i;
	for (i=0;i<pSTRINGS->Count;i++)
	{	if (IsFolder(pSTRINGS->GetStr(i)))	TotalSize	+=	cFOLDER::GetFolderSize("*.*",pSTRINGS->GetStr(i),TRUE);
		else								TotalSize	+=	cFILE::Size(pSTRINGS->GetStr(i));
	}

	Work		=	dWORK_PRESS;

	hPROGRESS	=	CreateDialog( hINST, l_strProgressDlg,hWND,(DLGPROC)PRESSProc);

	cNUX::ProcessPress	=	ProcessPress;

	if (append)
	{	if (!cNUX::PressAppend(ArcFile,merge))
			return	FALSE;
	}
	else
	{
		if (!cNUX::PressCreate(ArcFile,merge))
#ifdef	_FOR_ENG
			return	cMSG::Error("Error!!","It's root directory!!");
#else
			return	cMSG::Error("Nux 화일 생성 에러!!","드라이브의 루트에 화일을 생성 할 수 없습니다.");
#endif
	}

	for (i=0;i<pSTRINGS->Count;i++)
	{	char *fn= pSTRINGS->GetStr(i);
		if (!IsFolder(fn))	if (!cNUX::PressFile(fn)) break;
	}

	for (i=0;i<pSTRINGS->Count;i++)
	{	char *fn= pSTRINGS->GetStr(i);
		if (IsFolder(fn))	if (!cNUX::PressFile(fn)) break;
	}

	SendMessage(hPROGRESS,WM_CLOSE,0,0);

	cNUX::PressClose();

	if (!cNUX::bSTOP)
#ifdef	_FOR_ENG
		cMSG::Put("work complete","directory : %s\n\nfile : \"%s\"\n\nfile count : %d",
											ArcFolder,_exportFileName(ArcFile),cNUX::FileCount);
#else
		cMSG::Put("Nux 화일이 완성 되었습니다.","[%s]폴더에\n\n\"%s\"화일을 생성 하였습니다.\n\n%d개의 화일을 압축하였습니다.",
											ArcFolder,_exportFileName(ArcFile),cNUX::FileCount);
#endif

	SetCurrentDirectory(_logFolder);

	Work		=	dWORK_NOTHING;

	return TRUE;
}

/*****************************************************************************
								합치기
*****************************************************************************/
BOOL
cMAIN::Merge(BOOL append)
{	ArcFile[0] = 0;

	if (!pSTRINGS			) return FALSE;
	if (pSTRINGS->Count < 1	) return FALSE;

	OPENFILENAME	ofn;

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize	=	sizeof(OPENFILENAME);
	ofn.hwndOwner	=	hWND;
	ofn.nMaxFile	=	sizeof(ArcFile);
	ofn.lpstrFile	=	ArcFile;
	ofn.Flags		=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

#ifdef	_FOR_ENG
	if (append)
	{
		ofn.Flags		=	OFN_HIDEREADONLY;
		ofn.lpstrFilter =	"Attach data to .exe file {*.exe}\0*.exe\0Attach data to fiel {*.*}\0*.*\n\n";
		ofn.lpstrDefExt	=	"exe";
		ofn.lpstrTitle	=	"compress data and attatch to select file";
	}
	else
	{
		ofn.lpstrFilter =	"NUX Pressed Data File. {*.nux}\0*.nux\0";
		ofn.lpstrDefExt	=	"nux";
		ofn.lpstrTitle	=	"compress data and build nux file";
	}
#else
	if (append)
	{
		ofn.Flags		=	OFN_HIDEREADONLY;
		ofn.lpstrFilter =	"실행 화일 뒤에 붙이기 {*.exe}\0*.exe\0임의의 화일 뒤에 붙이기. {*.*}\0*.*\n\n";
		ofn.lpstrDefExt	=	"exe";
		ofn.lpstrTitle	=	"화일들을 압축하여 화일 뒤에 붙입니다.";
	}
	else
	{
		ofn.lpstrFilter =	"NUX Pressed Data File. {*.nux}\0*.nux\0";
		ofn.lpstrDefExt	=	"nux";
		ofn.lpstrTitle	=	"화일들을 압축하여 Nux 화일을 생성합니다.";
	}
#endif

	SetCurrentDirectory(ArcFolder);
	if (!GetSaveFileName(&ofn)) return FALSE;
	GetCurrentDirectory(sizeof(ArcFolder),ArcFolder);

	if (append)
	{	cFILE	file;

		file.Open(ArcFile,"rb");
		strcpy(ArcFile,_ms("%s_",ArcFile));
		file.Copy(ArcFile);
		file.Close();
	}

	int i;

	for (i=0;i<pSTRINGS->Count;i++)
	{	if (IsFolder(pSTRINGS->GetStr(i)))
				TotalSize	+=	cFOLDER::GetFolderSize("*.*",pSTRINGS->GetStr(i),TRUE);
		else	TotalSize	+=	cFILE::Size(pSTRINGS->GetStr(i));
	}

	Work				=	dWORK_PRESS;

	if (append)	
	{	if (!cNUX::MergeAppend(ArcFile))
			return	FALSE;
	}
	else
	{	if (!cNUX::MergeCreate(ArcFile))
#ifdef	_FOR_ENG
			return	cMSG::Error("error!!","it's root folder!!");
#else
			return	cMSG::Error("Nux 화일 생성 에러!!","드라이브의 루트에 화일을 생성 할 수 없습니다.");
#endif
	}

	hPROGRESS			=	CreateDialog( hINST, l_strProgressDlg,hWND,(DLGPROC)PRESSProc);

	cNUX::ProcessPress	=	ProcessPress;

	for (i=0;i<pSTRINGS->Count;i++)
	{	char *fn= pSTRINGS->GetStr(i);
		if (!IsFolder(fn))	if (!cNUX::MergeFile(fn)) break;
	}

	for (i=0;i<pSTRINGS->Count;i++)
	{	char *fn= pSTRINGS->GetStr(i);
		if (IsFolder(fn))	if (!cNUX::MergeFolder(fn,TRUE,TRUE)) break;
	}

	SendMessage(hPROGRESS,WM_CLOSE,0,0);

	cNUX::MergeClose();

#ifdef	_FOR_ENG
	if (!cNUX::bSTOP)
		cMSG::Put("비압축 Nux화일을 만들었습니다.","[%s]폴더에\n\n\"%s\"화일을 생성 하였습니다.\n\n%d개의 화일을 압축하였습니다.",
											ArcFolder,_exportFileName(ArcFile),cNUX::FileCount);
#else
	if	(!cNUX::bSTOP)
		cMSG::Put("make a Nux file","folder [%s]\n\nfile name : \"%s\"\n\nfile count : %d",
											ArcFolder,_exportFileName(ArcFile),cNUX::FileCount);
#endif

	SetCurrentDirectory(_logFolder);

	Work		=	dWORK_NOTHING;

	return TRUE;
}

/*****************************************************************************
								풀어주기
*****************************************************************************/

BOOL
cMAIN::Extract()
{	ArcFile[0] = 0;

	OPENFILENAME	ofn;

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize	=	sizeof(OPENFILENAME);
	ofn.hwndOwner	=	hWND;
	ofn.nMaxFile	=	sizeof(ArcFile);
	ofn.Flags		=	OFN_HIDEREADONLY;
	ofn.lpstrFile	=	ArcFile;
	ofn.lpstrFilter =	"NUX Data File. {*.nux;*.exe}\0*.nux;*.exe\0All File. {*.*}\0*.*\0";
	ofn.lpstrDefExt	=	"nux";
#ifdef	_FOR_ENG
	ofn.lpstrTitle	=	"Nux 데이터를 풀어줍니다.";
#else
	ofn.lpstrTitle	=	"Extract Nux data.";
#endif


	SetCurrentDirectory(ArcFolder);

	if (!GetOpenFileName(&ofn))
	{	SetCurrentDirectory(_logFolder);
		return FALSE;
	}

	GetCurrentDirectory(sizeof(ArcFolder),ArcFolder);
	SetCurrentDirectory(ExtractFolder);

#ifdef	_FOR_ENG
	char *folder = cFOLDER::Tree(hWND,TRUE,"Nux화일을 풀어줄 폴더를 선택해 주세요");
#else
	char *folder = cFOLDER::Tree(hWND,TRUE,"select a target folder");
#endif

	if	(!folder)
		return FALSE;

	strcpy(ExtractFolder,folder);

	Work		=	dWORK_EXTRACT;

	hPROGRESS	=	CreateDialog( hINST, l_strProgressDlg,hWND,(DLGPROC)EXTRACTProc);

	cNUX::ProcessExtract	=	ProcessExtract;

	cSTRINGS *files	=	new cSTRINGS();

	BOOL result = cNUX::PressExtract(ArcFile,ExtractFolder,files);

	SendMessage(hPROGRESS,WM_CLOSE,0,0);
	
	if (result)
	{	
		if (!cNUX::bSTOP)
		{	
#ifdef	_FOR_ENG
			cMSG::Put("Extract a Nux file","file \"%s\" extract to \n\n[%s] folder\n\nfile count : %d",
						_exportFileName(ArcFile),ExtractFolder,cNUX::FileCount);
#else
			cMSG::Put("Nux화일의 압축을 풀었습니다.","\"%s\" 화일을\n\n[%s] 폴더에 풀었습니다.\n\n%d개의 화일이 생성 되었습니다.",
						_exportFileName(ArcFile),ExtractFolder,cNUX::FileCount);
#endif
		}
		else
		{	for (int i=0;i<files->Count;i++)
				if (!DeleteFile(files->GetStr(i)))
					cFOLDER::Delete(files->GetStr(i));
		}
	}
	else
	{	for (int i=0;i<files->Count;i++)
			if (!DeleteFile(files->GetStr(i)))
				cFOLDER::Delete(files->GetStr(i));
	}

	cKILL(files);


	SetCurrentDirectory(_logFolder);

	Work		=	dWORK_NOTHING;

	return TRUE;
}

void
cMAIN::Remove()
{	int	count		=	hLIST->GetSelectedCount();

	if (count < 1) return;

	int index;

	while ( (index = hLIST->GetSelectItem()) > -1) 
		Remove(index);
}

void
cMAIN::GetDropFiles(HDROP handle)
{	char	file[512];

	int		fileCount	= DragQueryFile(handle,0xFFFFFFFF,NULL,0),i;

	if (fileCount == 0 ) return;

	SetForegroundWindow(hWND);

	for (i=0;i<fileCount;i++)
	{	DragQueryFile(handle,i,file,sizeof(file));
		Add(file);
	}
}

void
cMAIN::Resize()
{	if (hLIST)	hLIST->Resize(0,0,pWND->ClientWidth(),pWND->ClientHeight(),FALSE);

	outFrame.OutFrame(hWND);
}

void
cMAIN::PopupMenu()
{	cPOINT	Pos;
	Pos.SetMousePos();

	HMENU	hMenu		=	CreatePopupMenu();

#ifdef	_FOR_ENG
	AppendMenu(hMenu	,MF_STRING	,dIDM_REMOVE	,"선택한 화일 제거");
	AppendMenu(hMenu	,MF_STRING	,IDM_PRESS_ALL	,"모든 화일 압축하기");
	AppendMenu(hMenu	,MF_STRING	,IDM_SAVE_IMAGE,"선택한 화일 이미지로 저장하기.");
#else
	AppendMenu(hMenu	,MF_STRING	,dIDM_REMOVE	,"Remove Selected file");
	AppendMenu(hMenu	,MF_STRING	,IDM_PRESS_ALL	,"Press all file");
#endif

	TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,Pos.x,Pos.y,0,hWND,NULL);

	DestroyMenu(hMenu);
}
void
cMAIN::ProcessPress()
{	
	if	(Work != dWORK_PRESS)
		return;

	UINT	uiUpdateTime	=	GetTickCount();

	static	UINT	s_uiLastUpdateTime	=	0;
	
	if	(s_uiLastUpdateTime	==	0)
		s_uiLastUpdateTime	=	uiUpdateTime;

	int	iPastTime	=	uiUpdateTime-s_uiLastUpdateTime;

	if	(iPastTime	<	50)
		return;

	s_uiLastUpdateTime	=	uiUpdateTime;

	Sleep(1);

	MSG msg;

	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{	if (IsWindow(hPROGRESS))
		{	if (!IsDialogMessage(hPROGRESS,&msg))
			{	if( msg.message == WM_QUIT) return;
				else
				{	TranslateMessage(&msg); 
					DispatchMessage(&msg);
				}
			}

		}
		else
		if( msg.message == WM_QUIT) return;
		else
		{	TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
	}

	if (pDIBWND)
	{	if (!pDIBWND->Active()) return;

		cDRAW::Fill(0);

		char folder[512];
		char file[128];

		strcpy(folder,_exportFolderName(ArcFile));
		strcpy(file,_exportFileName(ArcFile));

#ifdef	_FOR_ENG
		cTEXT::PutDC(2,8,-1,_ms( "Make file : \"%s\"",file));
#else
		cTEXT::PutDC(2,8,-1,_ms( "\"%s\" 화일을 생성하고 있습니다.",file));
#endif
		cRECT_SIZE rect;

		rect.ClientFrame(pDIBWND->hDIBWND);

		//	전체 압축 진행률
		DWORD ProgressRate	=	0;

		if (TotalSize <= 100)	ProgressRate=	(cNUX::ProcessSize/100)/TotalSize;
		else					ProgressRate=	(cNUX::ProcessSize)/(TotalSize/100);

		cDRAW::Fill(_BLUE		,2,24,2+(rect.xs-6)*ProgressRate/100,40);
		cDRAW::XLine(_DEEPGRAY	,2	,rect.xs-4	,24);
		cDRAW::XLine(_LTGRAY	,2	,rect.xs-4	,40);
		cDRAW::YLine(_DEEPGRAY	,24	,40			,2);
		cDRAW::YLine(_LTGRAY	,24	,40			,rect.xs-4);

		cTEXT::PutDC(rect.xs/2-24,26,-1,_ms("%3d%%",ProgressRate));

		//	화일 압축 진행률
		cTEXT::PutDC(2,50,-1,cNUX::CurrentFile);
		cDRAW::Fill	(_BLUE	    ,2  ,66,2+(rect.xs-6)*cNUX::FileRate/100,82);
		cDRAW::XLine(_DEEPGRAY	,2	,rect.xs-4	,66);
		cDRAW::XLine(_LTGRAY	,2	,rect.xs-4	,82);
		cDRAW::YLine(_DEEPGRAY	,66	,82			,2);
		cDRAW::YLine(_LTGRAY	,66	,82			,rect.xs-4);

		cTEXT::PutDC(rect.xs/2-24,68,-1,_ms("%3d%%",cNUX::FileRate));

		pDIBWND->Draw();
	}
}

void
cMAIN::ProcessExtract()
{	if (Work != dWORK_EXTRACT) return;

	Sleep(1);

	MSG msg;

	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{	if( msg.message == WM_QUIT) return;
		else
		{	TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
	}

	if (pDIBWND)
	{	pDIBWND->Active();

		cDRAW::Fill(0);

		char folder[512];
		char file[128];

		strcpy(folder,_exportFolderName(ArcFile));
		strcpy(file,_exportFileName(ArcFile));

#ifdef	_FOR_ENG
		cTEXT::PutDC(2,8,-1,_ms( "Extract File : \"%s\" ",file));
#else
		cTEXT::PutDC(2,8,-1,_ms( "\"%s\" 화일을 풀고 있습니다.",file));
#endif

		cRECT_SIZE rect;

		rect.ClientFrame(pDIBWND->hDIBWND);

		//	전체 디코딩 진행률
		cDRAW::Fill(_BLUE		,2,24,2+(rect.xs-6)*cNUX::ProcessRate/100,40);
		cDRAW::XLine(_DEEPGRAY	,2	,rect.xs-4	,24);
		cDRAW::XLine(_LTGRAY	,2	,rect.xs-4	,40);
		cDRAW::YLine(_DEEPGRAY	,24	,40			,2);
		cDRAW::YLine(_LTGRAY	,24	,40			,rect.xs-4);

		cTEXT::PutDC(rect.xs/2-24,26,-1,_ms("%3d%%",cNUX::ProcessRate));

		//	화일 디코딩 진행률
		cTEXT::PutDC(2,50,-1,cNUX::CurrentFile);
		cDRAW::Fill	(_BLUE	    ,2,66,2+(rect.xs-6)*cNUX::FileRate/100,82);
		cDRAW::XLine(_DEEPGRAY	,2	,rect.xs-4	,66);
		cDRAW::XLine(_LTGRAY	,2	,rect.xs-4	,82);
		cDRAW::YLine(_DEEPGRAY	,66	,82			,2);
		cDRAW::YLine(_LTGRAY	,66	,82			,rect.xs-4);

		cTEXT::PutDC(rect.xs/2-24,68,-1,_ms("%3d%%",cNUX::FileRate));

		pDIBWND->Draw();
	}
}

LRESULT CALLBACK 
cMAIN::WNDProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch( uMsg )
	{	case	WM_ACTIVATEAPP			:
			bACTIVEAPP = wParam;
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );

		case	WM_CLOSE					:	Exit();	break;

		case	WM_MOVE						:	
		case	WM_SIZE						:	
			Resize();
			break;

		case	WM_COMMAND					:
			switch(LOWORD(wParam))
			{	case	IDM_EXTRACT			:
					Extract();
					break;

				case	IDM_PRESS			:
					Press();
					break;

				case	IDM_PRESS_APPEND	:
					Press(TRUE);
					break;

				case	IDM_NOPRESS			:
					Press(FALSE,TRUE);
					break;

				case	IDM_NOPRESS_APPEND	:
					Press(TRUE,TRUE);
					break;

				case	IDM_MERGE			:
					Merge();
					break;

				case	IDM_MERGE_APPEND	:
					Merge(TRUE);
					break;

				case	IDM_NEW				:
					New();
					break;

				case	IDM_EXIT			:
					Exit();
					break;

				case	dIDM_REMOVE			:
					Remove();
					break;

				case	IDM_SAVE_IMAGE		:
					SaveAsImage();
					break;
			}
			break;

		case WM_DROPFILES					:
			GetDropFiles((HDROP)wParam);
			break;

		case WM_CONTEXTMENU		:
		{	int index = hLIST->GetHitItem();

			if (index == -1) break;

			PopupMenu();
			break;
		}

		default :
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return( 0L );
}

LRESULT CALLBACK
cMAIN::PRESSProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{	case WM_INITDIALOG	:
		{	pDIBWND	=	new cDIBWND();
			pDIBWND->Init(GetDlgItem(hDlg,IDC_SCREEN),DRAWProc);
			pDIBWND->TextSetting(cTEXT::hFONT,TRANSPARENT,255,255,255);
			break;
		}

		case WM_CLOSE		:
			DestroyWindow(hDlg);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {	case	IDCANCEL		:
				{
#ifdef	_FOR_ENG
					if (cMSG::YESNO("stop the work","really?")==IDNO)
#else
					if (cMSG::YESNO("작업중지.","압축 작업을 중지 하시겠습니까?")==IDNO)
#endif
						break;
					cNUX::Stop();
					break;
				}
			}
			break;
	}

	return (0L);
}

LRESULT CALLBACK
cMAIN::EXTRACTProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{	case WM_INITDIALOG	:
		{	pDIBWND	=	new cDIBWND();
			pDIBWND->Init(GetDlgItem(hDlg,IDC_SCREEN),DRAWProc);
			pDIBWND->TextSetting(cTEXT::hFONT,TRANSPARENT,255,255,255);
			break;
		}

		case WM_CLOSE		:
			DestroyWindow(hDlg);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {	case	IDCANCEL		:
				{
#ifdef	_FOR_ENG
					if (cMSG::YESNO("stop the work","really?")==IDNO)
#else
					if (cMSG::YESNO("작업중지.","압축 작업을 중지 하시겠습니까?")==IDNO)
#endif
						break;
					cNUX::Stop();
					break;
				}
			}
			break;
	}

	return (0L);
}


LRESULT CALLBACK 
cMAIN::DRAWProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch( uMsg )
	{	
		case	WM_PAINT	:	
			ProcessPress();
			ProcessExtract();
			break;

		default				:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return( 0L );
}