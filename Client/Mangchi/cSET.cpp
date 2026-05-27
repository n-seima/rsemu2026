#include "resource.H"
#include "cSET.H"
#include "cMAPWND.H"
#include "cMAIN.H"
#include "cMESSAGE.H"
#include "patrolRoad.H"

BOOL		cSET::_isAddMap		=	FALSE;
HINSTANCE	cSET::s_hInst;
HWND		cSET::s_hMasterWnd	=	NULL;

void
cSET::Init(HINSTANCE hinst,HWND hwnd)
{
	s_hInst		=	hinst;
	s_hMasterWnd=	hwnd;

}

BOOL
cSET::NewMap(HINSTANCE hinst,HWND hwnd)
{
	_isAddMap		=	TRUE;

	int availMap	=	cMAP::GetValidMap();

	if (availMap	==	0xffff)	return	ERRMSG(dMSG_MAP_LOAD_ERROR,dMSG_TOO_MANY_MAP);

	// 10.01.12
	if( DialogBox( hinst, "NEW_MAP",hwnd,(DLGPROC)cNEWMAP::WndProc) == IDOK)
	{
		cMemoWindow::Reset(&g_lpActmap->m_sMemoInfo);
		cMemoWindow::SetMemo(&g_lpActmap->m_sMemoInfo);
		// 10.01.15
		cSET_AREA_EDIT::InitAreaDataBox();
		cSET_AREA_EDIT::ResetAreData();
	}

	return TRUE;
}

BOOL
cSET::ResetMap(HINSTANCE hinst,HWND hwnd)
{
	_isAddMap		=	FALSE;

	if( DialogBox( hinst, "NEW_MAP",hwnd,(DLGPROC)cNEWMAP::WndProc) == IDOK)
	{
		cMemoWindow::Reset(&g_lpActmap->m_sMemoInfo);
		cMemoWindow::SetMemo(&g_lpActmap->m_sMemoInfo);
		// 10.01.15
		cSET_AREA_EDIT::InitAreaDataBox();
		cSET_AREA_EDIT::ResetAreData();


	}
	return TRUE;
}

BOOL
cSET::SetMap(HINSTANCE hinst,HWND hwnd)
{
	DialogBox( hinst, "SET_MAP",hwnd,(DLGPROC)cSETMAP::WndProc);
//	return	ERRMSG(dMSG_ERROR_CHANGE_MAP_SETTING,dMSG_CAN_NOT_CHANGE_SETTING);
//	DialogBox( hinst, "SET_MAP",hwnd,(DLGPROC)cSETMAP::WndProc);

	return TRUE;
}

void
cSetCorrectTile::Open(HWND _hWnd)
{
	DialogBox( s_hInst, "CORRECT_TILE",_hWnd,(DLGPROC)ProcDlg);
}

BOOL
cSetCorrectTile::FinishWork(HWND _hWnd)
{
	cPOINT	posDest,posSrc;
	BOOL	bTemp;

	posDest.x	=	GetDlgItemInt(_hWnd,IDC_TARGET_X,&bTemp	,FALSE);
	if (!bTemp)
		return	FALSE;

	posDest.y	=	GetDlgItemInt(_hWnd,IDC_TARGET_Y,&bTemp	,FALSE);
	if (!bTemp)
		return	FALSE;

	posSrc.x	=	GetDlgItemInt(_hWnd,IDC_SOURCE_X,&bTemp	,FALSE);
	if (!bTemp)
		return	FALSE;

	posSrc.y	=	GetDlgItemInt(_hWnd,IDC_SOURCE_Y,&bTemp	,FALSE);
	if (!bTemp)
		return	FALSE;

	if (posSrc.x	<=	1	||	posDest.x	<=	1)
		return	FALSE;
	if (posSrc.x	>=	g_lpActmap->m_iWidth-2	||	posDest.x	>=	g_lpActmap->m_iWidth-2)
		return	FALSE;
	if (posSrc.y	<=	1	||	posDest.y	<=	1)
		return	FALSE;
	if (posSrc.y	>=	g_lpActmap->m_iHeight-2	||	posDest.y	>=	g_lpActmap->m_iHeight-2)
		return	FALSE;

	int	iIndex	=	g_lpActmap->m_pPattern->GetMapIndex(posSrc.x,posSrc.y);	

	g_lpActmap->m_pPattern->SetMapIndex(posDest.x,posDest.y,iIndex);
	g_lpActmap->LostVirgin();

	g_MAPWND.Draw();

	return	TRUE;
}

LRESULT CALLBACK
cSetCorrectTile::ProcDlg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG	:
		{
			SetFocus(GetDlgItem(hwnd,IDC_TARGET_X));
			break;
		}

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
				{
					if (!FinishWork(hwnd))
						ERRMSG("에러!!","부적절한 좌표");
					break;
				}

				case	IDCANCEL			:
				{
					EndDialog(hwnd,IDCANCEL);
					break;
				}
			}
			break;
	}

	return (0L);
}

cPatrolRoad		cSetPatrolRoad::s_patrolRoad;

BOOL
cSetPatrolRoad::Open(HWND _hWnd,cPatrolRoad*	_lpRoad)
{
	if (_lpRoad)
		s_patrolRoad.copy(_lpRoad);
	else
		s_patrolRoad.reset();

	if (DialogBox( s_hInst, "PATROL_ROAD",_hWnd,(DLGPROC)ProcDlg)	==	IDOK)
		return	TRUE;

	return	FALSE;
}

BOOL
cSetPatrolRoad::FinishWork(HWND _hWnd)
{
	BOOL	bIsTemp;

	GetDlgItemText(_hWnd,IDC_NAME,s_patrolRoad.m_strName,30);
	s_patrolRoad.m_wStayTime	=	GetDlgItemInt(_hWnd,IDC_STAY_NODE_TIME,&bIsTemp,FALSE);
	s_patrolRoad.m_wIsReturnRoad=	IsDlgButtonChecked(_hWnd,IDC_IS_RETURN);

	if (g_lpActmap->m_prManager.add(&s_patrolRoad,TRUE)==0xffff)
		return	FALSE;

	return	TRUE;
}

LRESULT CALLBACK
cSetPatrolRoad::ProcDlg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG	:
		{
			SetDlgItemText(hwnd,IDC_NAME,s_patrolRoad.m_strName);
			SetDlgItemInt(hwnd,IDC_STAY_NODE_TIME,s_patrolRoad.m_wStayTime,FALSE);
			CheckDlgButton(hwnd,IDC_IS_RETURN,s_patrolRoad.m_wIsReturnRoad);
			SetFocus(GetDlgItem(hwnd,IDC_NAME));
			break;
		}

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
				{
					if (!FinishWork(hwnd))
						break;

					EndDialog(hwnd,IDOK);
					break;
				}

				case	IDCANCEL			:
				{
					EndDialog(hwnd,IDCANCEL);
					break;
				}
			}
			break;
	}

	return (0L);
}



char				*cHelpDialog::s_pText	=	NULL;
char				cHelpDialog::s_strTitle[256];

BOOL
cHelpDialog::Open(HWND _hWnd,char *_lpstrDialogName,char *_lpstrTitle,char *_lpstrFn)
{
	strcpy(s_strTitle,_lpstrTitle);

	SetCurrentDirectory("dev data/mangchi");

	cFILE	file;

	if	(!file.Open(_lpstrFn,"rb"))
	{
		SetLogFolder();
		return	TRUE;
	}
	SetLogFolder();

	s_pText	=	new char [file.Length+1];

	file.Read(s_pText,file.Length);
	s_pText[file.Length]	=	NULL;
	file.Close();


	DialogBox( s_hInst, _lpstrDialogName,_hWnd,(DLGPROC)ProcDlg);

	pKILL(s_pText);

	return	TRUE;
}

BOOL
cHelpDialog::Init(HWND _hWnd)
{
	SetDlgItemText(_hWnd,IDC_TEXT,s_pText);
	SetWindowText(_hWnd,s_strTitle);

	return	TRUE;
}

LRESULT CALLBACK
cHelpDialog::ProcDlg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG	:
		{
			Init(hwnd);
			break;
		}

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDCANCEL			:
				case	IDOK				:
				{
					EndDialog(hwnd,IDOK);
					break;
				}
			}
			break;
	}

	return (0L);
}