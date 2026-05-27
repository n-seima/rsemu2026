#include "setSecretDungeon.h"
#include "resource.h"

cSecretDungeonFloorDefine		cMakeFloor::s_makeFloor;

BOOL
cMakeFloor::Open(HWND _hWnd,cSecretDungeonFloorDefine *_lpEdit)	//	Ω√¿€
{
	memcpy(&s_makeFloor,_lpEdit,sizeof(cSecretDungeonFloorDefine));

	if (DialogBox( s_hInst, "SECRET_DUNGEON_FLOOR",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	FALSE;

	return	TRUE;
}

void
cMakeFloor::InitData(HWND _hDlg)
{
	SetDlgItemInt(_hDlg,IDC_CORRECT_EXP,s_makeFloor.m_wCorrectExperience,FALSE);
	CheckDlgButton(_hDlg,IDC_IS_REGEN_MONSTER,s_makeFloor.m_wIsRegenMonster);
}

BOOL
cMakeFloor::FinishWork(HWND _hDlg)
{
	BOOL	bIsTemp;

	s_makeFloor.m_wCorrectExperience	=	GetDlgItemInt(_hDlg,IDC_CORRECT_EXP,&bIsTemp,FALSE);
	s_makeFloor.m_wIsRegenMonster		=	IsDlgButtonChecked(_hDlg,IDC_IS_REGEN_MONSTER);

	if (!bIsTemp)
		return	FALSE;

	if (s_makeFloor.m_wCorrectExperience	<=	5)
		return	FALSE;

	return	TRUE;

}

LRESULT CALLBACK
cMakeFloor::ProcDlg(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{	
		case WM_INITDIALOG	:
			InitData(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			break;

		case WM_COMMAND		:
			switch( LOWORD( wParam ) )
            {
				case	IDOK			:
					if (FinishWork(hDlg))
						EndDialog(hDlg,IDOK);
					break;

				case	IDCANCEL		:
					EndDialog(hDlg,IDCANCEL);
					break;

				default					:
					break;
			}
			break;
	}

	return (0L);
}
