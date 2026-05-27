#include "cMemoWindow.h"
#include "cDRAW.h"
#include "cMAIN.H"
#include "cTEXT.H"


#define			ID_MEMOEDIT									100
#define			dMEMO_MAXBUFFER								1048576
#define			dMEMO_HEADERYPOS							0
#define			dMEMO_MINWIDTH								50
#define			dMEMO_WIDTH									200
#define			dMEMO_HEIGHT								230

HWND			cMemoWindow::m_hMemo						= NULL;
HWND			cMemoWindow::m_hEdit						= NULL;
HFONT			cMemoWindow::m_hEditFont;
MEMO_INFO		*cMemoWindow::m_pMemoInfo;
BOOL			cMemoWindow::m_bShowWindow					= FALSE;
char			cMemoWindow::m_FileName[512];

//
// 초기화
void		
cMemoWindow::Reset(MEMO_INFO	*_pMemoInfo)
{
	m_pMemoInfo = _pMemoInfo;

	free(m_pMemoInfo->text);
	m_pMemoInfo->text	= NULL;	

	m_pMemoInfo->pos.x	= 1000;
	m_pMemoInfo->pos.y	= 100;
	m_pMemoInfo->size.x	= dMEMO_WIDTH;
	m_pMemoInfo->size.y	= dMEMO_HEIGHT;
}

//
// 메모 윈도우 생성
void		
cMemoWindow::CreateMemo(HINSTANCE _hInst, HWND _hWnd)
{
	if(m_hMemo)
		return;

	WNDCLASS WndClass;
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=_hInst;
	WndClass.lpfnWndProc=WNDProc;
	WndClass.lpszClassName="MEMOWINDOW";
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	
	m_hMemo = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,"MEMOWINDOW","",WS_CAPTION| WS_POPUP | WS_THICKFRAME,
					m_pMemoInfo->pos.x,m_pMemoInfo->pos.y,m_pMemoInfo->size.x,m_pMemoInfo->size.y,NULL,(HMENU)NULL,_hInst,NULL);
	RECT rt;
	GetClientRect(m_hMemo, &rt);

	m_hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,"edit",NULL,WS_CHILD | WS_VISIBLE 
					| ES_AUTOHSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | WS_HSCROLL | WS_VSCROLL,
					rt.top,rt.left,rt.right,rt.bottom,m_hMemo,(HMENU)ID_MEMOEDIT,_hInst,NULL);
	
	SendMessage(m_hEdit, EM_LIMITTEXT, (WPARAM)dMEMO_MAXBUFFER,0);

	m_hEditFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, 
		VARIABLE_PITCH | FF_ROMAN, "돋움");
	
	SendMessage(m_hEdit, WM_SETFONT, (WPARAM)m_hEditFont,MAKELPARAM(FALSE, 0));

	if(m_bShowWindow)
		ShowWindow(m_hMemo, SW_SHOW);
}

//
// 메모 설정
void		
cMemoWindow::SetMemo(MEMO_INFO *_pMemoInfo)
{
	m_pMemoInfo = _pMemoInfo;

	MoveWindow(m_hMemo, m_pMemoInfo->pos.x, m_pMemoInfo->pos.y, m_pMemoInfo->size.x, m_pMemoInfo->size.y,TRUE);

	RECT rt;
	GetClientRect(m_hMemo, &rt);
	MoveWindow(m_hEdit, rt.left, rt.top, rt.right, rt.bottom,TRUE);
	
	if(_pMemoInfo->text != NULL)
		SetWindowText(m_hEdit,_pMemoInfo->text);
	else
		SetWindowText(m_hEdit,"");

	SendMessage(m_hEdit,EM_SETMODIFY,(WPARAM)FALSE,0);
}

//
// 주소
char*
cMemoWindow::ConversionFilename(char *_filename, char *_check)
{
	char dirstr[512];
	strcpy(dirstr, _filename);
	strcpy(m_FileName, ".\\Memo\\");
	char checkname[512];
	strcpy(checkname, strcat(cSTRING::MinusBack(dirstr, "."), _check));
	strcat(m_FileName, checkname);

	return m_FileName;
}

//
// 텍스츠 저장
void
cMemoWindow::SaveText()
{
	free(m_pMemoInfo->text);
	m_pMemoInfo->text = NULL;
	
	int length=GetWindowTextLength(m_hEdit);
	
	if(length > 0)
	{
		m_pMemoInfo->text = (TCHAR *)malloc(length+1);
		GetWindowText(m_hEdit,m_pMemoInfo->text,length+1);
	}
}

LRESULT CALLBACK
cMemoWindow::WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static HDC hdc;
	static HPEN hPen, OldPen;

	switch(uMsg)
	{
	case WM_CREATE:
		{
			hdc = GetDC(hWnd);
			SLWA pSetLayeredWindowAttributes = NULL;  
			HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL"); 
			pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
			SetWindowLong(hWnd, GWL_EXSTYLE,GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
			pSetLayeredWindowAttributes(hWnd, 0, 190, LWA_ALPHA);
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_ERASEBKGND:
		return 0;
	case WM_SIZE:
		MoveWindow(m_hEdit, 0, dMEMO_HEADERYPOS, LOWORD(lParam), HIWORD(lParam),TRUE);
		break;
	case WM_MOVE:
		RECT rt;
		GetWindowRect(hWnd, &rt);
		
		m_pMemoInfo->pos.x	= rt.left;
		m_pMemoInfo->pos.y	= rt.top;

		break;
	case WM_SIZING:
		{	
			RECT rt;
			GetWindowRect(hWnd, &rt);
						
			m_pMemoInfo->pos.x	= rt.left;
			m_pMemoInfo->pos.y	= rt.top;

			m_pMemoInfo->size.x = rt.right - rt.left;
			m_pMemoInfo->size.y = rt.bottom - rt.top;
		}
		break;
	case WM_DESTROY:
		free(m_pMemoInfo->text);
		SelectObject(hdc, OldPen);
		DeleteObject(hPen);
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
