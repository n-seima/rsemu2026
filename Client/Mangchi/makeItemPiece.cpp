#include "setPieceItem.h"
#include "cMAIN.H"
#include "cMESSAGE.H"
#include "cSET.H"
#include "cDRAW.H"
#include "cITEM.H"
#include "cTEXT.H"
#include "cITEM_PREFIX_EFFECT.H"
#include "cITEM_EFFECT.H"
#include "cPROJECT.H"
#include "resource.h"
#include "CSetCustomItem.h"

HWND				cMakeItemPiece::s_hWnd;
cItemPieceDefine	cMakeItemPiece::s_makeItemPiece;

BOOL
cMakeItemPiece::Open(HWND _hWnd,cItemPieceDefine *_lpEdit)	//	시작
{
	if (_lpEdit)
		memcpy(&s_makeItemPiece,_lpEdit,sizeof(cItemPieceDefine));
	else
		s_makeItemPiece.reset();

	if (DialogBox(s_hInst, "PICEITEM_PIECE",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	FALSE;

	return	TRUE;
}

void
cMakeItemPiece::InitDlg(HWND _hDlg)//	시작
{
	SetDlgItemInt(_hDlg,IDC_DROP_LEVEL,s_makeItemPiece.m_wDropLevel	,FALSE);
	SetDlgItemText(_hDlg,IDC_PIECE_DROP_CHANCE,_ms("%d.%.2d",s_makeItemPiece.m_wDropChance/100,s_makeItemPiece.m_wDropChance%100));

	CheckDlgButton(_hDlg,IDC_IS_REVERSION,s_makeItemPiece.m_bf1IsReversion);
	CheckDlgButton(_hDlg,IDC_IS_IM_DUN_MONSTER		,s_makeItemPiece.m_bf1IsDropInTempDungeonMonster	);
	CheckDlgButton(_hDlg,IDC_IS_IM_DUN_ARCA			,s_makeItemPiece.m_bf1IsDropInTempDungeonArca		);
	CheckDlgButton(_hDlg,IDC_IS_NORMAL_FIELD_MONSTER,s_makeItemPiece.m_bf1IsDropInNormalFieldMonster	);
	CheckDlgButton(_hDlg,IDC_IS_NORMAL_FIELD_ARCA	,s_makeItemPiece.m_bf1IsDropInNormalFieldArca		);
	CheckDlgButton(_hDlg,IDC_IS_QUEST_REWARD		,s_makeItemPiece.m_bf1IsDropInQuest					);
}

BOOL
cMakeItemPiece::FinishWork(HWND _hDlg)
{
	BOOL	bTemp;
	char	strText[256];

	s_makeItemPiece.m_wDropLevel						=	GetDlgItemInt(_hDlg,IDC_DROP_LEVEL,&bTemp	,FALSE);

	GetDlgItemText(_hDlg,IDC_PIECE_DROP_CHANCE,strText,200);

	s_makeItemPiece.m_wDropChance						=	StringToNumber(strText,100,2);

	s_makeItemPiece.m_bf1IsReversion					=	IsDlgButtonChecked(_hDlg,IDC_IS_REVERSION			);
	s_makeItemPiece.m_bf1IsDropInTempDungeonMonster		=	IsDlgButtonChecked(_hDlg,IDC_IS_IM_DUN_MONSTER		);
	s_makeItemPiece.m_bf1IsDropInTempDungeonArca		=	IsDlgButtonChecked(_hDlg,IDC_IS_IM_DUN_ARCA			);
	s_makeItemPiece.m_bf1IsDropInNormalFieldMonster		=	IsDlgButtonChecked(_hDlg,IDC_IS_NORMAL_FIELD_MONSTER);
	s_makeItemPiece.m_bf1IsDropInNormalFieldArca		=	IsDlgButtonChecked(_hDlg,IDC_IS_NORMAL_FIELD_ARCA	);
	s_makeItemPiece.m_bf1IsDropInQuest					=	IsDlgButtonChecked(_hDlg,IDC_IS_QUEST_REWARD		);

	return	TRUE;
}


LRESULT CALLBACK
cMakeItemPiece::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG	:
			InitDlg(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;

		case	WM_COMMAND	:
			switch( LOWORD( wParam ) )
            {
				case	IDOK					:
				{
					if (FinishWork(hDlg))
						EndDialog(hDlg,IDOK);
					break;
				}

				case	IDCANCEL				:
					EndDialog(hDlg,IDCANCEL);
					break;
			}
			break;
	}

	return (0L);
}