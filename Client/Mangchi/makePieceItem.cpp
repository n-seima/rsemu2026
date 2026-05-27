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
#include "setPieceItem.h"
#include "setMakeItem.h"

HWND					cMakePieceItem::s_hWnd;
cPieceItem				cMakePieceItem::s_makePieceItem;
cLISTCONTROL			cMakePieceItem::s_listItemPiece;
cDIBWND					cMakePieceItem::s_dibItemInfo;
int						cMakePieceItem::s_iSelectItemPiece	=	0;

BOOL
cMakePieceItem::Open(HWND _hWnd,cPieceItem	*_lpEdit)	//	시작
{
	if (_lpEdit)
		s_makePieceItem.copy(_lpEdit);
	else
		s_makePieceItem.reset();

	if (DialogBox(s_hInst, "PICEITEM",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	FALSE;

	return	TRUE;
}

void
cMakePieceItem::InitDlg(HWND _hDlg)//	시작
{
	int	i;

	CheckDlgButton(_hDlg,IDC_IS_SET_ITEM,s_makePieceItem.m_bf1IsSetItem);
	SetDlgItemText(_hDlg,IDC_NAME,s_makePieceItem.m_strName);

//	조각 개수
	{
		HWND	hPieceCount	=	GetDlgItem(_hDlg,IDC_PIECE_COUNT);

		for (i=2;i<=dMAX_PIECE_ITEM_PIECE_COUNT;i++)
			cCOMBOBOX::Add(hPieceCount, _ms("%2d piece item", i), i);

		cCOMBOBOX::SelectByData(hPieceCount,s_makePieceItem.m_wPieceCount);
	}

//	세트 아이템
	{
		HWND	hItemSet		=	GetDlgItem(_hDlg,IDC_SET_ITEM_GROUP);
		HWND	hSetItem		=	GetDlgItem(_hDlg,IDC_SET_ITEM);

		for (i=1;i<g_itemSetManager.getMaxCount();i++)
		{
			cItemSet	*lpGroup	=	g_itemSetManager.get(i);

			if (!lpGroup)
				continue;

			cCOMBOBOX::Add(hItemSet,lpGroup->m_strName,i);
		}

		cCOMBOBOX::SelectByData(hItemSet,s_makePieceItem.m_bf11SetGroupIndex);

		cItemSet	*lpGroup	=	g_itemSetManager.get(s_makePieceItem.m_bf11SetGroupIndex);

		if (lpGroup)
		{
			for (i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
			{
				if (lpGroup->m_aItem[i].m_wBaseItem	==	0xffff)
					continue;

				cBASIC_ITEM	*lpBasicItem	=	&g_aBasicItem[lpGroup->m_aItem[i].m_wBaseItem];

				cCOMBOBOX::Add(hSetItem,lpBasicItem->m_strName,i);
			}

			cCOMBOBOX::SelectByData(hSetItem,s_makePieceItem.m_bf4SetItemIndex);
		}
	}


//	조각 리스트
	{
		HWND	hPieceList	=	GetDlgItem(_hDlg,IDC_PIECE_LIST);

		s_listItemPiece.Init(hPieceList	,9,FALSE,ProcItemPieceListWnd);		//	3개의 컬럼

		s_listItemPiece.AddColumn("No"	,6*4);
		s_listItemPiece.AddColumn("D/L"	,6*5,eLCOM_CENTER);
		s_listItemPiece.AddColumn("D/C"	,6*10,eLCOM_CENTER);
		s_listItemPiece.AddColumn("Transaction", 6 * 6, eLCOM_CENTER);
		s_listItemPiece.AddColumn("Quest", 6 * 8, eLCOM_CENTER);
		s_listItemPiece.AddColumn("Immon", 6 * 6, eLCOM_CENTER);
		s_listItemPiece.AddColumn("Imbo", 6 * 6, eLCOM_CENTER);
		s_listItemPiece.AddColumn("Field", 6 * 6, eLCOM_CENTER);
		s_listItemPiece.AddColumn("Box", 6 * 6, eLCOM_CENTER);
	}	//	접두사 리스트

	SetDlgItemInt(_hDlg,IDC_ITEM_EQUIP_LIMIT_LEVEL,s_makePieceItem.m_wEquipLevel,FALSE);

	s_dibItemInfo.lpszRegister		=	"ITEM_INFO";
	s_dibItemInfo.Init(GetDlgItem(_hDlg,IDC_DETAIL_ITEM_INFO),(WNDPROC)ProcItemInfoWnd);

	ResetItemPieceListBox(_hDlg);	
	DrawItemPieceList(_hDlg);
	DrawItemInfo(_hDlg);
}

void
cMakePieceItem::DrawItemInfo(HWND _hDlg)
{
	if (s_makePieceItem.m_bf1IsSetItem)
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_MAKE_ITEM),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_SET_ITEM_GROUP),TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_SET_ITEM),TRUE);
	}
	else
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_MAKE_ITEM),TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_SET_ITEM_GROUP),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_SET_ITEM),FALSE);

	}

	CSetCustomItemList::DrawItemInfoWnd(_hDlg,&s_dibItemInfo,NULL,NULL,&s_makePieceItem);
}

void
cMakePieceItem::DrawItemPieceList(HWND _hDlg)
{
	s_listItemPiece.Draw();
}


void
cMakePieceItem::EditItemPiece(HWND _hDlg)
{
	int	iSelectList	=	s_listItemPiece.GetSelectLine();

	if (iSelectList	<=	-1)
		return;

	cItemPieceDefine	*lpPiece	=	&s_makePieceItem.m_aPiece[iSelectList];

	if (cMakeItemPiece::Open(_hDlg,lpPiece))
	{
		memcpy(lpPiece,&cMakeItemPiece::s_makeItemPiece,sizeof(cItemPieceDefine));

		ResetItemPieceListBox(_hDlg);
		DrawItemPieceList(_hDlg);
	}
}

//
//	기본 아이템 목록 설정
void
cMakePieceItem::ResetItemPieceListBox(HWND hwnd)
{
	s_listItemPiece.ResetItems();

	int		iIndex;
	char	*strOX[]	=	{"X","O"};


	for (iIndex=0;iIndex<s_makePieceItem.m_wPieceCount;iIndex++)
	{
		cItemPieceDefine	*lpPiece	=	&s_makePieceItem.m_aPiece[iIndex];

		s_listItemPiece.AddItem(0,_ms("%.2d",iIndex+1),iIndex);
		s_listItemPiece.AddItem(1,_ms("%d",lpPiece->m_wDropLevel));
		s_listItemPiece.AddItem(2,_ms("%d.%.2d％",lpPiece->m_wDropChance/100,lpPiece->m_wDropChance%100));

		s_listItemPiece.AddItem(3,strOX[1-lpPiece->m_bf1IsReversion					]);
		s_listItemPiece.AddItem(4,strOX[lpPiece->m_bf1IsDropInQuest					]);
		s_listItemPiece.AddItem(5,strOX[lpPiece->m_bf1IsDropInTempDungeonMonster	]);
		s_listItemPiece.AddItem(6,strOX[lpPiece->m_bf1IsDropInTempDungeonArca		]);
		s_listItemPiece.AddItem(7,strOX[lpPiece->m_bf1IsDropInNormalFieldMonster	]);
		s_listItemPiece.AddItem(8,strOX[lpPiece->m_bf1IsDropInNormalFieldArca		]);

		s_listItemPiece.IncreaseLineCounter();
	}

	s_listItemPiece.Sort(0);

	if (!s_listItemPiece.selectByData(0,s_iSelectItemPiece))
	{
		if (!s_listItemPiece.Select(0))
			s_iSelectItemPiece	=	0xffff;
		else
			s_iSelectItemPiece	=	s_listItemPiece.GetData(0,0);
	}
}

void
cMakePieceItem::ChangeSetItem(HWND _hDlg,int _iItem)
{
	cItemSet	*lpGroup	=	g_itemSetManager.get(s_makePieceItem.m_bf11SetGroupIndex);

	if (!lpGroup	||	_iItem	>=	lpGroup->getItemCount())
		return;

	s_makePieceItem.m_bf4SetItemIndex	=	_iItem;

	DrawItemInfo(_hDlg);
}

void
cMakePieceItem::ChangeItemSet(HWND _hDlg,int _iGroup)
{
	HWND	hItemSet	=	GetDlgItem(_hDlg,IDC_SET_ITEM_GROUP);
	HWND	hSetItem		=	GetDlgItem(_hDlg,IDC_SET_ITEM);

	if	(s_makePieceItem.m_bf11SetGroupIndex	==	_iGroup)
		return;

	cCOMBOBOX::Reset(hSetItem);

	cItemSet	*lpGroup	=	g_itemSetManager.get(_iGroup);

	if (!lpGroup)
		return;

	s_makePieceItem.m_bf4SetItemIndex	=	0;
	s_makePieceItem.m_bf11SetGroupIndex	=	_iGroup;

	for (int i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
	{
		if (lpGroup->m_aItem[i].m_wBaseItem	==	0xffff)
			continue;

		cBASIC_ITEM	*lpBasicItem	=	&g_aBasicItem[lpGroup->m_aItem[i].m_wBaseItem];

		cCOMBOBOX::Add(hSetItem,lpBasicItem->m_strName,i);
	}

	cCOMBOBOX::SelectByData(hSetItem,s_makePieceItem.m_bf4SetItemIndex);

	DrawItemInfo(_hDlg);
}

void
cMakePieceItem::ChangePieceCount(HWND _hDlg,int _iCount)
{
	int	i;

	for (i=s_makePieceItem.m_wPieceCount;i<_iCount;i++)
		s_makePieceItem.m_aPiece[i].reset();

	for (i=_iCount;i<s_makePieceItem.m_wPieceCount;i++)
		s_makePieceItem.m_aPiece[i].reset();

	s_makePieceItem.m_wPieceCount	=	_iCount;

	ResetItemPieceListBox(_hDlg);
	DrawItemPieceList(_hDlg);
}

BOOL
cMakePieceItem::FinishWork(HWND _hDlg)
{
	BOOL	bTemp;

	GetDlgItemText(_hDlg,IDC_NAME,s_makePieceItem.m_strName,30);

	if (strlen(s_makePieceItem.m_strName) < 1)
		return ERRMSG("Craft sculpting item", "The sculpting item name is too short.");

	cPieceItem* lpPiece = g_pieceItemManager.getByName(s_makePieceItem.m_strName);

	if (lpPiece)
		if (s_makePieceItem.m_wSerial != lpPiece->m_wSerial)
			return ERRMSG("Create sculpture item", "Error!! There is already a sculpture item with the same name.");

	cITEM item;

	if (!s_makePieceItem.generateItem(&item))
		return ERRMSG("Create sculptural item", "No completed item has been set.");

	s_makePieceItem.m_wEquipLevel		=	GetDlgItemInt(_hDlg,IDC_ITEM_EQUIP_LIMIT_LEVEL,&bTemp,FALSE);
	s_makePieceItem.m_bf1IsSetItem		=	IsDlgButtonChecked(_hDlg,IDC_IS_SET_ITEM);

	s_makePieceItem.m_bf4SetItemIndex	=	cCOMBOBOX::GetData(GetDlgItem(_hDlg,IDC_SET_ITEM_GROUP));
	s_makePieceItem.m_bf11SetGroupIndex	=	cCOMBOBOX::GetData(GetDlgItem(_hDlg,IDC_SET_ITEM));

	return	TRUE;
}

//
//
LRESULT CALLBACK 
cMakePieceItem::ProcItemPieceListWnd(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_LBUTTONDOWN	:
		{
			s_iSelectItemPiece	=	s_listItemPiece.GetSelectData();
//			Draw(hDlg);
			break;
		}
	}

	return (0L);
}

LRESULT CALLBACK
cMakePieceItem::ProcItemInfoWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{
			PAINTSTRUCT ps;

			BeginPaint( hDlg, &ps );

			s_dibItemInfo.Active();
			s_dibItemInfo.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hDlg, &ps);
			break;
		}

		default				:
			return( DefWindowProc( hDlg, uMsg, wParam, lParam ) );
	}

	return (0L);
}

LRESULT CALLBACK
cMakePieceItem::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

				case	IDCANCEL					:
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_MAKE_ITEM				:
				{
					cITEM	*lpResult	=	cSetMakeItem::Open(hDlg,(cITEM *)&s_makePieceItem.m_item);

					if (lpResult)
					{
						memcpy(&s_makePieceItem.m_item,lpResult,sizeof(s_makePieceItem.m_item));
						DrawItemInfo(hDlg);
					}
					break;
				}

				case	IDC_SET_ITEM_GROUP			:
				{
					if (HIWORD(wParam)	!=	CBN_SELCHANGE)
						break;

					int	iGroup	=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_SET_ITEM_GROUP));

					ChangeItemSet(hDlg,iGroup);
					break;
				}
				case	IDC_SET_ITEM	:
				{
					if (HIWORD(wParam)	!=	CBN_SELCHANGE)
						break;
					int	iSetItem	=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_SET_ITEM));

					ChangeSetItem(hDlg,iSetItem);
					break;
				}
				case	IDC_PIECE_COUNT				:
				{
					if (HIWORD(wParam)	!=	CBN_SELCHANGE)
						break;

					int	iCount	=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_PIECE_COUNT));

					ChangePieceCount(hDlg,iCount);
					break;
				}

				case	IDC_IS_SET_ITEM	:
				{
					s_makePieceItem.m_bf1IsSetItem	=	IsDlgButtonChecked(hDlg,IDC_IS_SET_ITEM);
					DrawItemInfo(hDlg);
					break;
				}

				case	IDC_EDIT_PIECE	:
				{
					EditItemPiece(hDlg);
					break;
				}
			}
			break;
	}

	return (0L);
}