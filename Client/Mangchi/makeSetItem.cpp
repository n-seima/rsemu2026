#include "setSetItem.h"
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

HWND					cMakeItemSet::s_hWnd;
cItemSet				cMakeItemSet::s_makeItemSet;
cDIBWND					cMakeItemSet::s_dibSetEffectData;
cDIBWND					cMakeItemSet::s_dibSetItemList;
cDIBWND					cMakeItemSet::s_dibSetItemInfo;
cSCROLL_BAR				cMakeItemSet::s_sbSetItemEffectList;

int						cMakeItemSet::s_iSelectSetGroup		=	0;
int						cMakeItemSet::s_iSelectSetCount		=	0;
int						cMakeItemSet::s_iSelectSetEffect	=	0;
int						cMakeItemSet::s_iSelectSetItem		=	0;
cRECT					cMakeItemSet::s_rectSetItemList;

int						cMakeItemSet::s_iSetEffectDlgHeight;

BOOL
cMakeItemSet::Open(HWND _hWnd,cItemSet *_lpEditGroup)//	시작
{
	if (_lpEditGroup)
		s_makeItemSet.copy(_lpEditGroup);
	else
		s_makeItemSet.reset();

	if (DialogBox(s_hInst, "SETITEM_MAKE_GROUP",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	FALSE;

	return	TRUE;
}

void
cMakeItemSet::InitDlg(HWND _hWnd)
{
	s_hWnd	=	_hWnd;

	HWND	hComoboEffctByCount	=	GetDlgItem(_hWnd,IDC_SET_ITEM_COUNT);

	for (int i=0;i<dMAX_SET_ITEM_EFFECT_GRADE_COUNT;i++)
		cCOMBOBOX::Add(hComoboEffctByCount, _ms("Effect when collecting %2d items", i + 1), i);

	cCOMBOBOX::Select(hComoboEffctByCount,0);

	HWND	hSetItemList			=	GetDlgItem(_hWnd,IDC_SET_ITEM_LIST);

	s_dibSetItemList.lpszRegister	=	"SET_ITEM_LIST_WINDOW";
	s_dibSetItemList.Init(hSetItemList,(WNDPROC)ProcSetItemListDlg);

	HWND	hSetItemInfo			=	GetDlgItem(_hWnd,IDC_DETAIL_ITEM_INFO);
	s_dibSetItemInfo.lpszRegister	=	"ITEM_DETAIL_INFO";
	s_dibSetItemInfo.Init(hSetItemInfo,(WNDPROC)ProcSetItemInfoWnd);

	InitSetEffectListWnd(_hWnd);
	
	Draw(_hWnd);
}

void
cMakeItemSet::InitSetEffectListWnd(HWND _hWnd)
{
	HWND	hSetEffectDataWnd		=	GetDlgItem(_hWnd,IDC_EFFECT_LIST);

	s_dibSetEffectData.lpszRegister	=	"Red Stone Set Effect DibWnd";
	s_dibSetEffectData.Init	(hSetEffectDataWnd,(WNDPROC)ProcSetEffectDlg);

	s_sbSetItemEffectList.Init(hSetEffectDataWnd);		//	스크롤바 초기화
	s_sbSetItemEffectList.InitHeight();

	s_iSetEffectDlgHeight		=	s_dibSetEffectData.height();	//	아이템 리스트 높이

	s_sbSetItemEffectList.SetHeight(s_iSetEffectDlgHeight,s_dibSetEffectData.height());
}

void
cMakeItemSet::Draw(HWND _hDlg)
{
	SetDlgItemText(_hDlg,IDC_NAME,s_makeItemSet.m_strName);
	SetDlgItemText(_hDlg,IDC_COMMENT,s_makeItemSet.m_pItemComment);
	SetDlgItemInt(_hDlg,IDC_EQUIP_LEVEL,s_makeItemSet.m_wEqiupLevel,FALSE);

	DrawSetItemListWnd(_hDlg);
	DrawSetEffect(_hDlg);
	DrawSelectSetItemInfoWnd(_hDlg);
}

void
cMakeItemSet::DrawSelectSetItemInfoWnd(HWND _hDlg)
{
	cSetItem	item;

	item.reset();

	cSetItem	*lpItem	=	s_makeItemSet.getItem(s_iSelectSetItem);

	if (!lpItem)
		lpItem	=	&item;

	CSetCustomItemList::DrawItemInfoWnd(_hDlg,&s_dibSetItemInfo,NULL,lpItem);
}
//
//	아이템 리스트 윈도우 그리기
void
cMakeItemSet::DrawSetItemListWnd(HWND _hDlg)
{
	int	x=4,y=5;

	s_dibSetItemList.Active();

	s_rectSetItemList.Set(0,0,cDRAW::WIDTH,cDRAW::HEIGHT);

	if (s_makeItemSet.getItemCount()	>=	dMAX_ITEM_COUNT_IN_SET)
		EnableWindow(GetDlgItem(_hDlg,IDC_ADD_ITEM),FALSE);
	else	
		EnableWindow(GetDlgItem(_hDlg,IDC_ADD_ITEM),TRUE);

	if (s_makeItemSet.getItemCount()	<=	0)
	{
		cDRAW::Fill(_SKYBLUE);

		cTEXT::PushShadow(FALSE);
		cTEXT::Put(5,5,0,dMSG_ITEM_NOT_EXIST);
		cTEXT::PopShadow();

		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE_ITEM),FALSE);

		s_dibSetItemList.Draw();

		return;
	}

	EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE_ITEM),TRUE);

	cDRAW::Fill(_SKYBLUE);

	int		iCount		=	0;
	int		iWidth		=	s_dibSetItemList.width();

	for (int i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
	{
		cSetItem	*lpSetItem		=	(cSetItem	*)&s_makeItemSet.m_aItem[i];

		if (lpSetItem->m_wBaseItem	==	0xffff)
			break;

		cBASIC_ITEM	*lpItem	=	&g_aBasicItem[lpSetItem->m_wBaseItem];

		iCount++;

		if (i==	s_iSelectSetItem)
		{
			cDRAW::Fill		(0			,x  ,y+2,iWidth-2,y+dICON_SHAPE_HEIGHT+6);
			cDRAW::Fill		(_BLUE		,x-1,y+1,iWidth-3,y+dICON_SHAPE_HEIGHT+5);
			cDRAW::Box		(_DEEPGRAY	,x-1,y+1,iWidth-3,y+dICON_SHAPE_HEIGHT+5);

			g_smiIconItem.put(lpItem->m_wIconShape,x+1,y+4);

			cTEXT::Put		(x+1+36	,y+4+2	,WHITE,_ms("[%.2d] %s",i+1,lpSetItem->m_strName));
			cTEXT::Put		(x+1+36	,y+4+18	,WHITE,g_strItemCategory[lpItem->m_wKind]);
		}
		else
		{
			cDRAW::Fill		(0			,x  ,y+2,iWidth-2,y+dICON_SHAPE_HEIGHT+6);
			cDRAW::Fill		(_DEEPGRAY	,x-2,y  ,iWidth-4,y+dICON_SHAPE_HEIGHT+4);
			cDRAW::Box		(0			,x-2,y  ,iWidth-4,y+dICON_SHAPE_HEIGHT+4);

			g_smiIconItem.put(lpItem->m_wIconShape,x	,y+3);

			cTEXT::Put		(x+36	,y+3+2	,LTGRAY,_ms("[%.2d] %s",i+1,lpSetItem->m_strName));
			cTEXT::Put		(x+36	,y+3+18	,LTGRAY,g_strItemCategory[lpItem->m_wKind]);
		}

		y	+=	(dICON_SHAPE_HEIGHT+8);
	}

	s_dibSetItemList.Draw();
}

//
//	유니크 아이템 데이터
void
cMakeItemSet::DrawSetEffect(HWND _hWnd)
{
//	아이템 효과
	s_dibSetEffectData.Active();

	cDRAW::Fill(_SKYBLUE);

	int		x =4,y=4-s_sbSetItemEffectList.m_iHeightPos;

	int		iWidth			=	s_dibSetEffectData.width() - 34;
	int		iLineHeight		=	14;
	int		iCount			=	0;

	s_iSetEffectDlgHeight	=	0;
	s_iSetEffectDlgHeight	=	4;

	for (int iSetItemCount=0;iSetItemCount<dMAX_ITEM_COUNT_IN_SET;iSetItemCount++)
	{
		BOOL	bIsPutSetItemCount	=	FALSE;

		int		iCurrentCount		=	0;

		for (int i=0;i<dMAX_SET_ITEM_EFFECT_COUNT;i++)
		{
			cUniqueData	*lpEffect	=	&s_makeItemSet.m_aSetEffect[iSetItemCount][i];

			if (lpEffect->m_wEffect	==	0xffff)
				break;

			iCount++;
			iCurrentCount++;

			char	*lpComment	=	lpEffect->getComment(TRUE);
			int		iLineCount	=	cTEXT::GetLines(iWidth,lpComment);

			x	=	5;

			if (bIsPutSetItemCount	==	FALSE)
			{
				cDRAW::Fill(0,x-2,y-2,x+10+6,y+iLineHeight);
				cTEXT::Put(x,y,LTGREEN,_ms("%.2d",iSetItemCount+1));

				bIsPutSetItemCount	=	TRUE;
			}

			x	=	25;

			if (i	==	s_iSelectSetEffect && iSetItemCount	==	s_iSelectSetCount)
			{
				cDRAW::Fill		(_LTBLUE,x-2,y-2,x+iWidth+4,y+iLineCount*iLineHeight);
				cDRAW::Fill		(0,x-2,y+iLineCount*iLineHeight+1,x+iWidth+4,y+iLineCount*iLineHeight+1);
				cTEXT::FitPut	(x+2,y,iWidth,iLineCount*8*2,LTYELLOW,lpComment);
			}
			else
			{	
				if (iSetItemCount	==	s_iSelectSetCount)
				{
					cDRAW::Fill		(0 ,x-2,y-1,x+iWidth+4,y+iLineCount*iLineHeight+2);
					cTEXT::FitPut	(x+2,y+1,iWidth,iLineCount*8*2,WHITE,lpComment);
				}
				else
				{
					cDRAW::Fill		(_DEEPGRAY,x-2,y-2,x+iWidth+4,y+iLineCount*iLineHeight+2);
					cTEXT::FitPut	(x+2,y+1,iWidth,iLineCount*8*2,WHITE,lpComment);
				}
			}

			y						+=	iLineCount*iLineHeight+2;
			s_iSetEffectDlgHeight	+=	iLineCount*iLineHeight+2;
		}

		if (iCurrentCount)
		{
			y						+=	4;
			s_iSetEffectDlgHeight	+=	4;
		}
	}

	s_sbSetItemEffectList.SetHeight(s_iSetEffectDlgHeight,s_dibSetEffectData.height());

	s_dibSetEffectData.Draw();
}

//
//	
int
cMakeItemSet::SelectSetItem(int _iX,int _iY)
{
	int	x=5,y=5;

	if (s_makeItemSet.getItemCount()	<=	0)
		return	0xffff;

	int		iIndex		=	s_sbSetItemEffectList.m_iHeightPos;
	int		iCount		=	0;

	if (!s_rectSetItemList.isIN(_iX,_iY))
		return	0xffff;

	for (int i=iIndex;i<dMAX_ITEM_COUNT_IN_SET;i++)
	{
		cSetItem	*lpSetItem		=	(cSetItem	*)&s_makeItemSet.m_aItem[i];

		if (lpSetItem->m_wBaseItem	==	0xffff)
			break;

		cBASIC_ITEM	*lpItem	=	&g_aBasicItem[lpSetItem->m_wBaseItem];

		iIndex++;
		iCount++;

		if (_iY	>=	y	&&	_iY	<=	y	+	dICON_SHAPE_HEIGHT+8)
			return	i;

		y	+=	(dICON_SHAPE_HEIGHT+8);
	}

	return	0xffff;
}

//
//	아이템 효과 선택
int
cMakeItemSet::SelectSetItemEffect(int _iX,int _iY)
{
	int		x =4,y=4-s_sbSetItemEffectList.m_iHeightPos;

	int		iWidth			=	s_dibSetEffectData.width() - 34;
	int		iLineHeight		=	14;
	int		iCount			=	0;

	for (int iSetItemCount=0;iSetItemCount<dMAX_ITEM_COUNT_IN_SET;iSetItemCount++)
	{
		int		iCurrentCount		=	0;

		for (int i=0;i<dMAX_SET_ITEM_EFFECT_COUNT;i++)
		{
			cUniqueData	*lpEffect	=	&s_makeItemSet.m_aSetEffect[iSetItemCount][i];

			if (lpEffect->m_wEffect	==	0xffff)
				break;

			iCount++;
			iCurrentCount++;

			char	*lpComment	=	lpEffect->getComment(TRUE);
			int		iLineCount	=	cTEXT::GetLines(iWidth,lpComment);

			x	=	5;

			if (_iY	>=	y	&&	_iY	<=	y+iLineCount*iLineHeight+4)
			{
				if (s_iSelectSetCount	!=	iSetItemCount)
				{
					int	iSetCount	=	cCOMBOBOX::SelectByData(GetDlgItem(s_hWnd,IDC_SET_ITEM_COUNT),iSetItemCount);

					s_iSelectSetCount	=	iSetItemCount;
				}

				return	i;
			}

			y						+=	iLineCount*iLineHeight+2;
		}

		if (iCurrentCount)
			y						+=	4;
	}

	return	0xffff;
}

BOOL
cMakeItemSet::FinishWork(HWND _hDlg)
{
	if (s_makeItemSet.getItemCount() < 2)
		return	ERRMSG("Create set items","Error!! More than one set item is required.");

	if (s_makeItemSet.getSetEffectCount() < 1)
		return	ERRMSG("Create set item","Error!! At least one set effect is required.");

	GetDlgItemText(_hDlg,IDC_NAME,s_makeItemSet.m_strName,30);

	{
		char	strText[1024];
		GetDlgItemText(_hDlg,IDC_COMMENT,strText,1024);
		s_makeItemSet.setComment(strText);
	}

	BOOL			bTemp;

	s_makeItemSet.m_wEqiupLevel	=	GetDlgItemInt(_hDlg,IDC_EQUIP_LEVEL,&bTemp,FALSE);

	cItemSet	*lpGroup		=	g_itemSetManager.getByName(s_makeItemSet.m_strName);

	if (lpGroup)
		if (s_makeItemSet.m_wSerial	!=	lpGroup->m_wSerial)
			return	ERRMSG("Create set item","Error!! There is already a set item with the same name.");

	return	TRUE;
}

LRESULT CALLBACK
cMakeItemSet::ProcSetItemInfoWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{
			PAINTSTRUCT ps;

			BeginPaint( hDlg, &ps );

			s_dibSetItemInfo.Active();
			s_dibSetItemInfo.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hDlg, &ps);
			break;
		}
		default				:
			return( DefWindowProc( hDlg, uMsg, wParam, lParam ) );
	}

	return (0L);

}

LRESULT CALLBACK
cMakeItemSet::ProcSetItemListDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{
			PAINTSTRUCT ps;

			BeginPaint( hDlg, &ps );

			s_dibSetItemList.Active();
			s_dibSetItemList.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hDlg, &ps);
			break;
		}


		case WM_LBUTTONDOWN	:
		{
			int item		=	SelectSetItem(LOWORD(lParam),HIWORD(lParam));

			if(item			==	0xffff		)
				break;
			if(item			==	s_iSelectSetItem)
				break;

			s_iSelectSetItem=	item;

			DrawSetItemListWnd(s_hWnd);
			DrawSelectSetItemInfoWnd(s_hWnd);

			break;
		}

		default				:
			return( DefWindowProc( hDlg, uMsg, wParam, lParam ) );
	}

	return (0L);
}

//
//	
LRESULT CALLBACK
cMakeItemSet::ProcSetEffectDlg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	
			PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibSetEffectData.Active();
			s_dibSetEffectData.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_LBUTTONDOWN	:
		{
			int item	=	SelectSetItemEffect(LOWORD(lParam),HIWORD(lParam));

			if	(item	==	0xffff)
				break;

			s_iSelectSetEffect	=	item;

			DrawSetEffect(hwnd);
			break;
		}

		case WM_VSCROLL			:
			if (s_sbSetItemEffectList.VScroll(wParam))
				DrawSetEffect(s_hWnd);
			break;

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}

//
//	다얄로그 박스 프로시져
LRESULT CALLBACK
cMakeItemSet::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

				case	IDC_ADD_SET_EFFECT		:
				{
					if (s_makeItemSet.isFullSetEffectCount(s_iSelectSetCount))
					{
						ERRMSG("Error!!","No more set effects can be added.");
						break;
					}

					if (!cPREFIX_EFFECT::AddEffect(hDlg))
						break;

					s_makeItemSet.addSetEffect(s_iSelectSetCount,(cUniqueData*)&cPREFIX_EFFECT::s_Effect);
					s_iSelectSetEffect	=	s_makeItemSet.getSetEffectCount(s_iSelectSetCount)-1;

					DrawSetEffect(hDlg);
					break;
				}

				case	IDC_EDIT_SET_EFFECT	:
				{
					cUniqueData	*lpEffect	=	s_makeItemSet.getSetEffect(s_iSelectSetCount,s_iSelectSetEffect);

					if (!lpEffect)
					{
						ERRMSG("Error!!","Please select a set effect first.");
						break;
					}

					if (!cPREFIX_EFFECT::AddEffect(hDlg,(cITEM_EFFECT*)lpEffect))
						break;

					s_makeItemSet.replaceSetEffect(s_iSelectSetCount,s_iSelectSetEffect,(cUniqueData*)&cPREFIX_EFFECT::s_Effect);

					DrawSetEffect(hDlg);
					break;
				}

				case	IDC_REMOVE_SET_EFFECT	:
				{
					if (s_makeItemSet.getSetEffectCount(s_iSelectSetCount) <= 0 || s_iSelectSetEffect	>= s_makeItemSet.getSetEffectCount(s_iSelectSetCount))
						break;

					if (cMSG::YESNO("Remove set effect.", "Do you want to remove the selected set effect from the list?") == IDNO)
						break;

					s_makeItemSet.removeSetEffect(s_iSelectSetCount,s_iSelectSetEffect);
					s_iSelectSetEffect	=	min(s_iSelectSetEffect,s_makeItemSet.getSetEffectCount(s_iSelectSetCount)-1);
					s_iSelectSetEffect	=	max(s_iSelectSetEffect,0);

					DrawSetEffect(hDlg);
					break;
				}

				case	IDC_ADD_ITEM			:
				{
					if (s_makeItemSet.isFullItem())
					{
						cMSG::Put("WARNING!!", "No more items can be added.");
						break;
					}
					cSetItem*	lpItem	=	CSetCustomItem::OpenForSetItem(hDlg);	//	시작

					if (lpItem)
					{
						if (!s_makeItemSet.addItem(lpItem))
							break;

						s_iSelectSetItem=	s_makeItemSet.getItemCount()-1;
						DrawSetItemListWnd(hDlg);
						DrawSelectSetItemInfoWnd(hDlg);
					}

					break;
				}

				case	IDC_EDIT_ITEM			:
				{
					cSetItem*	lpEditItem	=	s_makeItemSet.getItem(s_iSelectSetItem);

					if (!lpEditItem)
						break;

					cSetItem*	lpItem		=	CSetCustomItem::OpenForSetItem(hDlg,lpEditItem);	//	시작

					if (lpItem)
					{
						s_makeItemSet.replaceItem(s_iSelectSetItem,lpItem);

						DrawSetItemListWnd(hDlg);
						DrawSelectSetItemInfoWnd(hDlg);
					}

					break;
				}


				case	IDC_REMOVE_ITEM			:
				{
					if (s_makeItemSet.getItemCount() <= 0 || s_iSelectSetItem >= s_makeItemSet.getItemCount())
						break;

					if (cMSG::YESNO("Remove item.", "Do you want to remove the selected item from the set item list?") == IDNO)
						break;

					s_makeItemSet.removeItem(s_iSelectSetItem);

					s_iSelectSetItem	=	min(s_iSelectSetItem,s_makeItemSet.getItemCount()-1);
					s_iSelectSetItem	=	max(s_iSelectSetItem,0);

					DrawSetItemListWnd(hDlg);
					break;
				}

				case	IDC_SET_ITEM_COUNT		:
				{
					switch (HIWORD(wParam))
                    {	
						case	CBN_SELCHANGE	:
						{
							int	iSetCount	=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_SET_ITEM_COUNT));

							if (s_iSelectSetCount	!=	iSetCount)
							{
								s_iSelectSetCount	=	iSetCount;

								s_iSelectSetEffect	=	min(s_iSelectSetEffect,s_makeItemSet.getSetEffectCount(s_iSelectSetCount)-1);
								s_iSelectSetEffect	=	max(s_iSelectSetEffect,0);

								DrawSetEffect(hDlg);
							}
							break;
						}
					}
					break;
				}
			}
			break;
	}

	return (0L);
}