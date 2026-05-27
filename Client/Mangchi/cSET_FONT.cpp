#include	"cSET.H"
#include	"cMAIN.H"
#include	"cAREA.H"
#include	"cTEXT.H"
#include	"cPROJECT.H"
#include	"resource.h"
#include	"cMAPWND.h"
#include	"CSetDialog.h"
#include	"cDRAW.h"
#include	"cTEXT.h"

HWND		cSET_FONT::s_hWnd									= NULL;
HWND		cSET_FONT::s_hSliderR								= NULL;
HWND		cSET_FONT::s_hSliderG								= NULL;
HWND		cSET_FONT::s_hSliderB								= NULL;

COLORREF	cSET_FONT::s_ColorOption[dCOLOR_OPTION_MAX];
COLORREF	cSET_FONT::s_OldColorOption[dCOLOR_OPTION_MAX];
COLORREF	cSET_FONT::s_SampleColor							= RGB(0, 0, 0);
HDC			cSET_FONT::s_Paintdc;
HDC			cSET_FONT::s_BackPaintdc;
HBITMAP		cSET_FONT::s_hbackbitmap;
HBITMAP		cSET_FONT::s_holdbackbitmap;
HBRUSH		cSET_FONT::s_MyBrush;
HBRUSH		cSET_FONT::s_OldBrush;
cDIBWND		cSET_FONT::s_dibSampleText;

int			cSET_FONT::s_iColorOptionCount						= 0 ;
int			cSET_FONT::s_iSelectFont							= 0 ;


char	*strColorList[]	=
{
"file name",
"map name",
"Number of areas",
"Number of characters",
"temporary storage"			,
"No drawing",
"tile index",
"grid",
"DOOR",
"NPC name",
"Monster Name",
"NPC classification",
"Leader/Protege",
"skin"				,
"Object Serial",
"Area name",
"General area",
"notice board"			,
"door"				,
"entrance"			,
"Map to load",
"Connected places",
"Hidden Portal",
"Hidden internal movement portal",
"General traffic portal",
"trap"				,
"Event area",
"treasure box"			,
"Name display area",
"Monster Realm",
"Monster Zone Information",
"Rest of the Area",
};

void
cSET_FONT::InitFont(HWND hwnd)
{
	if(s_hWnd != NULL)
		return;

	CreateDialog(s_hInst, "SET_FONT_OPTION", hwnd, (DLGPROC)s_procSetFont);
}

void
cSET_FONT::InitFontData(HWND hWnd)
{
	if(s_iColorOptionCount == 0)
	{
		// Color Setting
		ResetColorOption();
		SaveFontData();
	}
	
	s_iColorOptionCount	= 32;

	// ListBox
	HWND	hTextList	=	GetDlgItem(hWnd,IDC_TEXT_LIST);
	
	cLISTBOX::Reset(hTextList);
		
	for(int i=0; i<s_iColorOptionCount; i++)
		cLISTBOX::Add(hTextList,strColorList[i],i);
		
	cLISTBOX::Select(hTextList, 0);
	
	// Slider Color Option
	s_hSliderR	= GetDlgItem(hWnd, IDC_SLIDER_R);
	s_hSliderG	= GetDlgItem(hWnd, IDC_SLIDER_G);
	s_hSliderB	= GetDlgItem(hWnd, IDC_SLIDER_B);
			
	s_SampleColor = s_ColorOption[eCOLOR_FILENAME];

	INT R = GetRValue(s_SampleColor);
	INT G = GetGValue(s_SampleColor);
	INT B = GetBValue(s_SampleColor);
	
	SendMessage(s_hSliderR, TBM_SETRANGE, FALSE, MAKELONG(dSLIDER_MIN_POS,dSLIDER_MAX_POS));
	SendMessage(s_hSliderR, TBM_SETPOS, TRUE, R);
	SendMessage(s_hSliderG, TBM_SETRANGE, FALSE, MAKELONG(dSLIDER_MIN_POS,dSLIDER_MAX_POS));
	SendMessage(s_hSliderG, TBM_SETPOS, TRUE, G);
	SendMessage(s_hSliderB, TBM_SETRANGE, FALSE, MAKELONG(dSLIDER_MIN_POS,dSLIDER_MAX_POS));
	SendMessage(s_hSliderB, TBM_SETPOS, TRUE, B);

	// Sample Text
	HWND	hSampleText	=	GetDlgItem(hWnd, IDC_SAMPLE_TEXT);

	s_dibSampleText.lpszRegister	=	"SAMPLETEXT_WINDOW";
	s_dibSampleText.Init(hSampleText,(WNDPROC)s_ProcSampleTextWnd);

	DrawSampleText();
}

void
cSET_FONT::ResetColorOption()
{
	s_OldColorOption[eCOLOR_FILENAME]			= LTYELLOW;
	s_OldColorOption[eCOLOR_MAPNAME]			= LTGREEN;
	s_OldColorOption[eCOLOR_AREA_COUNT]			= LTYELLOW;
	s_OldColorOption[eCOLOR_CHARACTER_COUNT]	= LTGREEN;
	s_OldColorOption[eCOLOR_SAVE_YESNO]			= LTGREEN;
	s_OldColorOption[eCOLOR_MAP_DRAW]			= LTRED;
	s_OldColorOption[eCOLOR_TILE_INDEX]			= WHITE;
	s_OldColorOption[eCOLOR_GIRD]				= WHITE;
	s_OldColorOption[eCOLOR_DOOR]				= WHITE;
	s_OldColorOption[eCOLOR_NPC_NAME]			= WHITE;
	s_OldColorOption[eCOLOR_MONSTER_NAME]		= WHITE;
	s_OldColorOption[eCOLOR_NPC_TYPE]			= RGB(255,128,255);
	s_OldColorOption[eCOLOR_LEADER]				= LTYELLOW;
	s_OldColorOption[eCOLOR_SKIN]				= RGB(255,128,255);
	s_OldColorOption[eCOLOR_OBJECT_SERIAL]		= WHITE;		
	s_OldColorOption[eCOLOR_AREA_NAME]			= WHITE;		
	s_OldColorOption[eCOLOR_AREA_NORMAL]		= WHITE;		
	s_OldColorOption[eCOLOR_AREA_NOTICE_BOARD]	= LTYELLOW;		
	s_OldColorOption[eCOLOR_AREA_DOOR]			= LTYELLOW;		
	s_OldColorOption[eCOLOR_PORTAL_EXIT]		= WHITE;		
	s_OldColorOption[eCOLOR_PORTAL_LOAD_MAP]	= YELLOW;		
	s_OldColorOption[eCOLOR_PORTAL_CMAP]		= LTYELLOW;		
	s_OldColorOption[eCOLOR_PORTAL_HIDDEN]		= LTYELLOW;		
	s_OldColorOption[eCOLOR_PORTAL_HIDDEN_IN]	= LTYELLOW;		
	s_OldColorOption[eCOLOR_PORTAL_ONE_WAY]		= LTYELLOW;		
	s_OldColorOption[eCOLOR_AREA_TRAP]			= LTYELLOW;		
	s_OldColorOption[eCOLOR_EVENT_AREA]			= LTYELLOW;		
	s_OldColorOption[eCOLOR_ARCA]				= LTYELLOW;		
	s_OldColorOption[eCOLOR_NAME_AREA]			= LTCYAN;
	s_OldColorOption[eCOLOR_MONSTER_ACTIVE]		= LTYELLOW;
	s_OldColorOption[eCOLOR_MONSTER_INFO]		= LTYELLOW;
	s_OldColorOption[eCOLOR_AREA_ETC]			= LTYELLOW;
}

void
cSET_FONT::SaveFontData(BOOL _type)
{
	for(int i=0; i<s_iColorOptionCount; i++)
	{
		if(_type == TRUE)
			s_ColorOption[i]	= s_OldColorOption[i];
		else
			s_OldColorOption[i]	= s_ColorOption[i];
	}
}

void
cSET_FONT::SaveOldData()
{
	INT R = GetRValue(s_SampleColor);
	INT G = GetGValue(s_SampleColor);
	INT B = GetBValue(s_SampleColor);

	s_OldColorOption[s_iSelectFont] = RGB(R,G,B);
	
	DrawSampleText();
}

void
cSET_FONT::ShowDialog()
{
	for(int i=0; i<s_iColorOptionCount; i++)
			s_OldColorOption[i] = s_ColorOption[i];
	
	s_iSelectFont	= 0;

	INT R = GetRValue(s_OldColorOption[s_iSelectFont]);
	INT G = GetGValue(s_OldColorOption[s_iSelectFont]);
	INT B = GetBValue(s_OldColorOption[s_iSelectFont]);

	s_SampleColor = RGB(R,G,B);

	HWND	hTextList	=	GetDlgItem(s_hWnd,IDC_TEXT_LIST);
	cLISTBOX::Select(hTextList, s_iSelectFont);

	DrawSampleText();
	ShowWindow(s_hWnd, SW_SHOW);
}

void
cSET_FONT::DrawSampleText()
{
	if (!s_dibSampleText.Active()) 
		return;
	
	cDRAW::Fill(_BLACK);
	
	cTEXT::Put(20,10,s_OldColorOption[s_iSelectFont], "ººº¸¼® / Red Stone / 12345");
	
	s_dibSampleText.Draw();
}

LRESULT CALLBACK		
cSET_FONT::s_procSetFont(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT		rect;

	switch(message)
	{
	case WM_INITDIALOG	:
		{
			s_hWnd		= hDlg;	
			HWND hPaint = GetDlgItem(hDlg, IDC_SAMPLE_COLOR);
			s_Paintdc	= GetDC(hPaint);

			GetClientRect(hPaint, &rect);
					
			s_BackPaintdc		= CreateCompatibleDC(s_Paintdc);
			s_hbackbitmap		= CreateCompatibleBitmap(s_Paintdc, rect.right, rect.bottom);                  
			s_holdbackbitmap	=  (HBITMAP)SelectObject(s_BackPaintdc, s_hbackbitmap);    
			
			ReleaseDC(hPaint, s_Paintdc);
			
			InitFontData(hDlg);
		}
		break;
	case WM_CLOSE:
		ShowWindow(hDlg, SW_HIDE);
		break;
	case WM_PAINT	:
		{
			HWND hPaint = GetDlgItem(hDlg, IDC_SAMPLE_COLOR);
			GetClientRect(hPaint, &rect);
			s_Paintdc = BeginPaint(hPaint, &ps);
							
			s_MyBrush =   CreateSolidBrush(s_SampleColor);
			s_OldBrush = (HBRUSH)SelectObject(s_BackPaintdc, s_MyBrush);

			Rectangle(s_BackPaintdc, rect.left, rect.top, rect.right, rect.bottom);

			BitBlt(s_Paintdc, rect.left, rect.top, rect.right, rect.bottom, s_BackPaintdc, 0, 0, SRCCOPY);

			SelectObject(s_BackPaintdc, s_OldBrush);
			DeleteObject(s_MyBrush);

			EndPaint(hPaint, &ps);
		}
		break;
	case WM_HSCROLL:
		{			
			INT R, G, B;
			
			R = GetRValue(s_SampleColor);
			G = GetGValue(s_SampleColor);
			B = GetBValue(s_SampleColor);

			if ((HWND)lParam == s_hSliderR)
				R = SendMessage(s_hSliderR, TBM_GETPOS,0,0);	
			if ((HWND)lParam == s_hSliderG)
				G = SendMessage(s_hSliderG, TBM_GETPOS,0,0);	
			if ((HWND)lParam == s_hSliderB)
				B = SendMessage(s_hSliderB, TBM_GETPOS,0,0);	

			s_SampleColor = RGB(R,G,B);
			InvalidateRect(hDlg, NULL, FALSE);
		}

		break;
	case WM_COMMAND :
		switch( LOWORD( wParam ) )
		{	
		case IDOK:
			SaveFontData();
			g_MAPWND.Draw();
			ShowWindow(hDlg, SW_HIDE);
			break;
		case IDCANCEL:
			ShowWindow(hDlg, SW_HIDE);
			break;
		case IDC_TEXT_LIST:
			{
				HWND hList = GetDlgItem(hDlg, IDC_TEXT_LIST);
				s_iSelectFont	= cLISTBOX::GetSelect(hList);
			
				INT R = GetRValue(s_OldColorOption[s_iSelectFont]);
				INT G = GetGValue(s_OldColorOption[s_iSelectFont]);
				INT B = GetBValue(s_OldColorOption[s_iSelectFont]);
					
				SendMessage(s_hSliderR, TBM_SETPOS, TRUE, R);
				SendMessage(s_hSliderG, TBM_SETPOS, TRUE, G);
				SendMessage(s_hSliderB, TBM_SETPOS, TRUE, B);
				
				s_SampleColor = RGB(R,G,B);
				InvalidateRect(hDlg, NULL, FALSE);

				DrawSampleText();
				break;
			}
		case 	IDC_UPDATE:
				SaveOldData();				
				break;
		case	IDC_BUTTON_RESET_DEFUALT:
			{
				ResetColorOption();	
				
				INT R = GetRValue(s_OldColorOption[s_iSelectFont]);
				INT G = GetGValue(s_OldColorOption[s_iSelectFont]);
				INT B = GetBValue(s_OldColorOption[s_iSelectFont]);
				
				SendMessage(s_hSliderR, TBM_SETPOS, TRUE, R);
				SendMessage(s_hSliderG, TBM_SETPOS, TRUE, G);
				SendMessage(s_hSliderB, TBM_SETPOS, TRUE, B);
				
				s_SampleColor = RGB(R,G,B);
				InvalidateRect(hDlg, NULL, FALSE);
				
				DrawSampleText();
				break;
			}
		}
		
		break;
	}

	return (0L);
}


LRESULT CALLBACK 
cSET_FONT::s_ProcSampleTextWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_PAINT		:
		{	PAINTSTRUCT ps;
		
		BeginPaint( hwnd, &ps );
		
		s_dibSampleText.Active();
		s_dibSampleText.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);
		
		EndPaint(hwnd, &ps);
		break;
		}

	default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}
	return (0L);
}