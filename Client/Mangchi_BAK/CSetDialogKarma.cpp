#include "cMAIN.H"
#include "cDRAW.h"
#include "cMAP.h"
#include "cTEXT.h"
#include "CKarma.H"
#include "CSetDialogKarma.h"

HWND				CSetDialogKarma::s_hWnd					= NULL;
HWND				CSetDialogKarma::s_hKarmaTab			= NULL;
HWND				CSetDialogKarma::s_hKarmaTabDlg			= NULL;
HWND				CSetDialogKarma::s_hTrigger				= NULL;
HWND				CSetDialogKarma::s_hReaction			= NULL;

BOOL				CSetDialogKarma::s_bIsEnableDialog		= FALSE;

int					CSetDialogKarma::s_iLastTab				= 0;
int					CSetDialogKarma::s_iCopyTriggerCount	= 0;
int					CSetDialogKarma::s_iCopyReactionCount	= 0;

//
// 대화 편집 카르마 생성
CKarma *
CSetDialogKarma::AddKarma(HWND _hwnd,CKarma *_lpEditKarma,int _iType)
{
	cKARMA_WND::s_iEditType			=	_iType;
	
	cKARMA_WND::s_lpkcEdit			=	_lpEditKarma;

	cKARMA_WND::s_iTriggerWndPos	=	0;
	cKARMA_WND::s_iReactionWndPos	=	0;

	g_iLastReaction		=	g_iSelectReaction;
	g_iLastTrigger		=	g_iSelectTrigger;

	int	result;

	s_bIsEnableDialog		= TRUE;
	s_iLastTab				= 0;
	s_iCopyTriggerCount		= 0;
	s_iCopyReactionCount	= 0;

	result	=	DialogBox( s_hInst,"SET_DIALOG_SPEECH_KARMA",_hwnd,(DLGPROC)s_ProcSetDialog);

	s_bIsEnableDialog	= FALSE;

	if	(result	==	IDCANCEL)
		return NULL;

	return	&g_EditKarma;
}

//
// 대화 편집 카르마 초기화
void
CSetDialogKarma::InitAddKarmaDlg(HWND _hwnd)
{
	s_hWnd = _hwnd;

	cKARMA_WND::InitAddKarmaDlg(s_hWnd);
	
	s_hKarmaTab = CreateWindow(WC_TABCONTROL, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0, 0, 0, 0, s_hWnd, (HMENU)0, s_hInst, NULL);

	HFONT hFont = CreateFont(11, 0, 0, 0, 0, 0, 0, 0,HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("돋움"));
	SendMessage(s_hKarmaTab, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE,0));

	TCITEM tie;
	tie.mask = TCIF_TEXT;
	tie.pszText = "조건";
	TabCtrl_InsertItem(s_hKarmaTab, 0, &tie);
	tie.pszText = "반응";
	TabCtrl_InsertItem(s_hKarmaTab, 1, &tie);

	HRSRC hRc;
	HGLOBAL hGlb;
	DLGTEMPLATEEX *dTem;
	RECT trt = {0,0,0, 0};
	
	hRc = FindResource(NULL, "PAGE_TRIGGER", RT_DIALOG);
	hGlb = LoadResource(NULL, hRc);
	dTem = (DLGTEMPLATEEX*)LockResource(hGlb);
	trt.right = dTem->cx;
	trt.bottom = dTem->cy;

	hRc = FindResource(NULL, "PAGE_REACTION", RT_DIALOG);
	hGlb = LoadResource(NULL, hRc);
	dTem = (DLGTEMPLATEEX*)LockResource(hGlb);
	trt.right = max(trt.right, dTem->cx);
	trt.bottom =max(trt.bottom, dTem->cy);
	
	s_hKarmaTabDlg = CreateDialog(s_hInst, "PAGE_TRIGGER", s_hWnd,s_ProcPageKarmaTrigger);
	MapDialogRect(s_hKarmaTabDlg, &trt);
	DestroyWindow(s_hKarmaTabDlg);

	TabCtrl_AdjustRect(s_hKarmaTab, TRUE, &trt);

	OffsetRect(&trt, -trt.left, -trt.top);
	SetWindowPos(s_hKarmaTab, NULL, 5, 370, 950, 377, SWP_NOZORDER);

	s_hKarmaTabDlg  = CreateDialog(s_hInst, "PAGE_TRIGGER", s_hWnd, s_ProcPageKarmaTrigger);
	ShowWindow(s_hKarmaTabDlg, SW_SHOW);
}

//
// 트리거 초기화
void
CSetDialogKarma::InitTrigger(HWND _hdlg)
{
	HWND	hWnd			=	GetDlgItem(s_hTrigger,IDC_CONTENTS);
	SendMessage(hWnd, WM_SETREDRAW, 0, 0);

	if (g_EditKarma.m_pTrigger	==	NULL)	
		g_EditKarma.reset(TRUE);

	g_EditKarma.m_wTriggerCount <= 0  ? cKARMA_WND::s_isEditTrigger = FALSE : cKARMA_WND::s_isEditTrigger = TRUE;

	cKARMA_WND::InitAddTriggerDlg(_hdlg, 12, 15);
	SendMessage(hWnd, WM_SETREDRAW, 1, 0);
}

//
// 반응 초기화
void
CSetDialogKarma::InitReaction(HWND _hdlg)
{
	HWND	hWnd	=	GetDlgItem(s_hReaction,IDC_CONTENTS);
	SendMessage(hWnd, WM_SETREDRAW, 0, 0);

	if (g_EditKarma.m_pReaction	==	NULL)	
		g_EditKarma.reset(TRUE);

	g_EditKarma.m_wReactionCount <= 0 ? cKARMA_WND::s_isEditReaction = FALSE : cKARMA_WND::s_isEditReaction = TRUE;
	
	cKARMA_WND::InitAddReactionDlg(_hdlg, 12, 15);
	SendMessage(hWnd, WM_SETREDRAW, 1, 0);
}

//
// 탭 변경
BOOL
CSetDialogKarma::ChangeKarmaTab(int _type)
{
	if(s_iLastTab == _type)
		return FALSE;

	TabCtrl_SetCurSel(s_hKarmaTab, _type);

	if(s_hKarmaTabDlg)	
		DestroyWindow(s_hKarmaTabDlg);

	if(_type== 0)
		s_hKarmaTabDlg = CreateDialog(s_hInst, "PAGE_TRIGGER" ,s_hWnd,s_ProcPageKarmaTrigger);
	if(_type == 1)
		s_hKarmaTabDlg = CreateDialog(s_hInst, "PAGE_REACTION",s_hWnd,s_ProcPageKarmaReaction);
	
	ShowWindow(s_hKarmaTabDlg, SW_SHOW);
	

	s_iLastTab = _type;
	return TRUE;
}

//
// 기본 Proc "SET_DIALOG_SPEECH_KARMA"
LRESULT CALLBACK
CSetDialogKarma::s_ProcSetDialog(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		InitAddKarmaDlg(hwnd);
		break;
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE:
			{
				int seltab = TabCtrl_GetCurSel(s_hKarmaTab);
				
				if(s_iLastTab != seltab)
				{
					if(s_hKarmaTabDlg)	
						DestroyWindow(s_hKarmaTabDlg);

					if(seltab == 0)
						s_hKarmaTabDlg = CreateDialog(s_hInst, "PAGE_TRIGGER" ,hwnd,s_ProcPageKarmaTrigger);
					if(seltab == 1)
						s_hKarmaTabDlg = CreateDialog(s_hInst, "PAGE_REACTION",hwnd,s_ProcPageKarmaReaction);

					ShowWindow(s_hKarmaTabDlg, SW_SHOW);
				}
				s_iLastTab = seltab;
			}
			break;
		}
		break;
	case WM_COMMAND		:
		switch( LOWORD( wParam ) )
		{
		case IDOK:
			if (cKARMA_WND::ReadAddKarmaDlg(hwnd))
				EndDialog(hwnd,IDOK);
			break;
		case IDCANCEL:
			EndDialog(hwnd,IDCANCEL);
			break;
		case	IDC_AND				:
		case	IDC_OR				:
			CheckRadioButton(hwnd,IDC_OR,IDC_AND,LOWORD( wParam ));
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd,IDCANCEL);
		break;	
	}
	return (0L);
}

BOOL CALLBACK
CSetDialogKarma::s_ProcPageKarmaTrigger(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT prt;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		s_hTrigger = hDlg;
		GetWindowRect(s_hKarmaTab, &prt);
		TabCtrl_AdjustRect(s_hKarmaTab, FALSE, &prt);
		ScreenToClient(hDlg, (LPPOINT)&prt);
		SetWindowPos(hDlg, HWND_TOP, prt.left, prt.top, 0, 0, SWP_NOSIZE);

		InitTrigger(hDlg);
		break;
	case WM_VSCROLL :
		cSPIN::setValue((HWND)lParam,HIWORD( wParam ));
		break;	
	case WM_COMMAND :
		{
			switch( LOWORD( wParam ) )
            {
				case	IDC_CONTENTS		:
					cKARMA_WND::SelectTriggerItem(hDlg, 12, 15);
					break;
				case	IDC_NEW				:
					cKARMA_WND::AddCopyTrigger(hDlg);
					break;
				case	IDC_EDIT			:
					if (g_EditKarma.m_wTriggerCount > 0)
						cKARMA_WND::EditCopyTrigger(hDlg);
					break;
				case	IDC_COPY	:
					if (g_EditKarma.m_wTriggerCount > 0)
					{
						s_iCopyTriggerCount	= 1;
						cKARMA_WND::CopyTrigger(hDlg);
					}
					break;
				case	IDC_PASTE	:
					if( s_iCopyTriggerCount == 1)
						cKARMA_WND::PasteTrigger(hDlg);
					break;
				case	IDC_REMOVE	:
					if (g_EditKarma.m_wTriggerCount > 0)
						cKARMA_WND::RemoveTrigger(hDlg);
					break;
			}
		}
		break;
	}
	return (0L);
}

BOOL CALLBACK
CSetDialogKarma::s_ProcPageKarmaReaction(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT prt;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		s_hReaction = hDlg;
		GetWindowRect(s_hKarmaTab, &prt);
		TabCtrl_AdjustRect(s_hKarmaTab, FALSE, &prt);
		ScreenToClient(hDlg, (LPPOINT)&prt);
		SetWindowPos(hDlg, HWND_TOP, prt.left, prt.top, 0, 0, SWP_NOSIZE);

		InitReaction(hDlg);
		break;
	case WM_VSCROLL :
		cSPIN::setValue((HWND)lParam,HIWORD( wParam ));
		break;	
	case WM_COMMAND :
		{
			switch( LOWORD( wParam ) )
            {
				case	IDC_CONTENTS		:
					cKARMA_WND::SelectReactionItem(hDlg, 12, 15);
					break;
				case	IDC_NEW				:
					cKARMA_WND::AddCopyReaction(hDlg);

					if (g_EditKarma.m_wReactionCount > 0)
						EnableWindow(GetDlgItem(s_hWnd,IDOK),TRUE);
					else	
						EnableWindow(GetDlgItem(s_hWnd,IDOK),FALSE);
					break;
				case	IDC_EDIT			:
					if (g_EditKarma.m_wReactionCount > 0)
							cKARMA_WND::EditCopyReaction(hDlg);
					break;
				case	IDC_COPY	:
					if (g_EditKarma.m_wReactionCount > 0)
					{
						cKARMA_WND::CopyReaction(hDlg);
						s_iCopyReactionCount	= 1;
					}
					break;
				case	IDC_PASTE	:
					if( s_iCopyReactionCount == 1)
						cKARMA_WND::PasteReaction(hDlg);
					break;
				case	IDC_REMOVE	:
					if (g_EditKarma.m_wReactionCount > 0)
						cKARMA_WND::RemoveReaction(hDlg);
					break;
				case	IDC_UP		:
					cKARMA_WND::UpReaction(hDlg);
					break;
				case IDC_DOWN		:
					cKARMA_WND::DownReaction(hDlg);
					break;
			}
		}
		break;
	}
	return (0L);
}
