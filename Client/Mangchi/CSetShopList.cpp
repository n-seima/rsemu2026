#include "CSetShop.h"
#include "resource.h"
#include "cDRAW.h"
#include "cTEXT.h"


HWND					CSetShopList::s_hWnd;
CShopManager			*CSetShopList::s_lpEditShoplist;	//	편집중인 상점
cDIBWND					CSetShopList::s_dibShopInfo;
cLISTCONTROL			CSetShopList::s_listShop;	//	접두사 리스트
int						CSetShopList::s_iSelectShop	=	0;
cSCROLL_BAR				CSetShopList::s_sbItemList;
int						CSetShopList::s_iItemListWidth;		//	아이템 리스트 너비
int						CSetShopList::s_iItemListHeight;		//	아이템 리스트 높이
int						CSetShopList::s_iLineCountPerPage;
int						CSetShopList::s_iLineCount;
cRECT					CSetShopList::s_rectItemList;
BOOL					CSetShopList::s_isSort = TRUE;				//TRUE면 인덱스 정렬 FALSE면 이름 정렬.
CShop*					CSetShopList::s_CopyShop;
//
//	시작
void
CSetShopList::Init(HWND hwnd,CShopManager *_lpShoplist)
{
	s_lpEditShoplist	=	_lpShoplist;

	DialogBox( s_hInst, "SET_SHOP_LIST",hwnd,(DLGPROC)ProcDlg);
}	//	CSetShopList::Init(HWND hwnd)

//
//	박스 초기화
void
CSetShopList::InitBox(HWND _hDlg)
{
	s_hWnd	=	_hDlg;

//	컨텐츠 윈도우 초기화
	{
		HWND	hShopList	=	GetDlgItem(_hDlg,IDC_SHOP_LIST);

		s_listShop.Init(hShopList	,1,FALSE,ProcShopListEvent);		//	3개의 컬럼
		s_listShop.AddColumn("상점 이름",6*80);
		s_listShop.HideColumn();


		ResetShopListBox(_hDlg);
	}

//	반응 윈도우 초기화
	{
		HWND	hShopInfo	=	GetDlgItem(_hDlg,IDC_SHOP_INFO);

		s_dibShopInfo.lpszRegister	=	"SHOP_INFO_WINDOW";
		s_dibShopInfo.Init(hShopInfo,(WNDPROC)ProcShopInfoWnd);

		s_sbItemList.Init(hShopInfo);		//	스크롤바 초기화
		s_sbItemList.InitHeight();

		s_iItemListWidth	=	s_dibShopInfo.width();		//	아이템 리스트 너비
		s_iItemListHeight	=	s_dibShopInfo.height();		//	아이템 리스트 높이
		s_iLineCountPerPage	=	(s_iItemListHeight-10)	/	(dICON_SHAPE_HEIGHT+8);

		s_sbItemList.SetHeight(2,1);

	}

	CheckDlgButton(_hDlg , IDC_SHOP_LIST_SORT_INDEX , TRUE);
	CheckDlgButton(_hDlg , IDC_SHOP_LIST_SORT_NAME , FALSE);
	
	DrawShopListWnd(_hDlg);
	DrawShopInfoWnd(_hDlg);
}	//	CSetShopList::InitBox(HWND _hDlg)

//
//	상점 리스트 그려주기
void
CSetShopList::DrawShopListWnd(HWND _hDlg)
{
	s_listShop.Draw();
}	//	CSetShopList::DrawShopListWnd(HWND _hDlg)

//
//	상점 정보 그려주기
void
CSetShopList::DrawShopInfoWnd(HWND _hDlg)
{

	CShop	*lpShop	=	s_lpEditShoplist->get(s_iSelectShop);

	int	x=4,y=5;

	s_dibShopInfo.Active();

	y					=	(cDRAW::HEIGHT-s_iLineCountPerPage*(dICON_SHAPE_HEIGHT+8))/2;

	s_rectItemList.Set(0,0,cDRAW::WIDTH,cDRAW::HEIGHT);

	if (!lpShop)
	{
		cDRAW::Fill(_SKYBLUE);

		cTEXT::PushShadow(FALSE);
		cTEXT::Put(5,5,0,dMSG_ITEM_NOT_EXIST);
		cTEXT::PopShadow();
	}
	else
	{
		cDRAW::Fill(_SKYBLUE);

		int		iIndex		=	s_sbItemList.m_iHeightPos,iCount=0;

		s_iLineCount		=	lpShop->getCount();
		s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage	);

		for (int i=iIndex;i<dMAX_SHOP_ITEM_COUNT;i++)
		{
			CShopItem	*lpShopItem	=	lpShop->get(i);

			if (!lpShopItem)	continue;

			if (iCount	>=	s_iLineCountPerPage	)	break;

			cBASIC_ITEM	*lpItem	=	&g_aBasicItem[lpShopItem->m_wBaseItem];

			iIndex++;
			iCount++;

			cDRAW::Fill		(0			,x  ,y+2,s_iItemListWidth-2,y+dICON_SHAPE_HEIGHT+6);
			cDRAW::Fill		(_DEEPGRAY	,x-2,y  ,s_iItemListWidth-4,y+dICON_SHAPE_HEIGHT+4);
			cDRAW::Box		(0			,x-2,y  ,s_iItemListWidth-4,y+dICON_SHAPE_HEIGHT+4);
			g_smiIconItem.put	(lpItem->m_wIconShape,x	,y+3);

			if (lpItem->isUnique())
			{
				cTEXT::Put(x+36,y+3+2,GOLDYELLOW,"★");
				cTEXT::Put(x+36,y+3+2,GOLDYELLOW,_ms("   %s",lpItem->m_strName));
			}
			else
			if (lpItem->isExtraItem())
			{
				cTEXT::Put(x+36,y+3+2,LTCYAN,"♠");
				cTEXT::Put(x+36,y+3+2,LTCYAN,_ms("   %s",lpItem->m_strName));
			}
			else
				cTEXT::Put(x+36	,y+3+2,WHITE,lpItem->m_strName);

			cTEXT::Put		(x+36	,y+3+18,LTGRAY,g_strItemCategory[lpItem->m_wKind]);

			y	+=	(dICON_SHAPE_HEIGHT+8);
		}
	}

	s_dibShopInfo.Draw();

}	//	CSetShopList::DrawShopInfoWnd(HWND _hDlg)

//
//	기본 아이템 목록 설정
void
CSetShopList::ResetShopListBox(HWND hwnd)
{
	if(s_lpEditShoplist->m_iCount	<=	0)
	{
		EnableWindow(GetDlgItem(hwnd,IDC_REMOVE),FALSE);
		EnableWindow(GetDlgItem(hwnd,IDC_EDIT),FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(hwnd,IDC_REMOVE),TRUE);
		EnableWindow(GetDlgItem(hwnd,IDC_EDIT),TRUE);
	}

	if	(s_lpEditShoplist->m_iCount	>=	dMAX_SHOP)
		EnableWindow(GetDlgItem(hwnd,IDC_ADD),FALSE);
	else
		EnableWindow(GetDlgItem(hwnd,IDC_ADD),TRUE);

	s_listShop.ResetItems();

	int	iIndex;

	for (iIndex=0;iIndex<dMAX_SHOP;iIndex++)
	{
		CShop	*lpShop	=	s_lpEditShoplist->get(iIndex);

		if	(!lpShop)
			continue;

		//JBC	삼점 정렬 관련 08-08-18
		if(s_isSort)
			s_listShop.AddItem(0,_ms("[%d]%s",lpShop->m_wSerial,lpShop->m_strName),iIndex);
		else
			s_listShop.AddItem(0,_ms("%s",lpShop->m_strName),iIndex);

		s_listShop.IncreaseLineCounter();
	}

	if	(!s_listShop.selectByData(0,s_iSelectShop))
	{
		if	(!s_listShop.Select(0))
			s_iSelectShop	=	0xffff;
		else
			s_iSelectShop	=	s_listShop.GetData(0);
	}
}	//	cSET_ITEM_PREFIX::ResetPrefixListBox(HWND hwnd,BOOL _bReset)


//
//	상점 추가
void
CSetShopList::AddShop(HWND _hDlg)
{
	CShop	*lpShop	=	CSetShop::Init(_hDlg);

	if (!lpShop)	return;

	int	iSerial	=	s_lpEditShoplist->addShop(lpShop);

	if (iSerial	==	0xffff)	return;

	s_iSelectShop=	iSerial;

	ResetShopListBox(_hDlg);
	DrawShopListWnd(_hDlg);
	DrawShopInfoWnd(_hDlg);
}	//	CSetShopList::AddShop(HWND _hDlg)

//
//	상점 편집
void
CSetShopList::EditShop(HWND _hDlg)
{
	CShop	*lpShop	=	s_lpEditShoplist->get(s_iSelectShop);

	if (!lpShop)	return;

	CShop	*lpEditShop	=	CSetShop::Init(_hDlg,lpShop);

	if (!lpEditShop)	return;

	lpShop->copy(lpEditShop);
	lpShop->m_wSerial	=	s_iSelectShop;

	ResetShopListBox(_hDlg);
	DrawShopListWnd(_hDlg);
	DrawShopInfoWnd(_hDlg);
}	//	CSetShopList::EditShop(HWND _hDlg)

//
//	상점 제거
void
CSetShopList::RemoveShop(HWND _hDlg)
{
	s_lpEditShoplist->removeShop(s_iSelectShop);

	ResetShopListBox(_hDlg);
	DrawShopListWnd(_hDlg);
	DrawShopInfoWnd(_hDlg);
}	//	CSetShopList::RemoveShop(HWND _hDlg)

// 상점 복사
BOOL
CSetShopList::CopyShop()
{
	s_CopyShop = &s_lpEditShoplist->m_aShop[s_iSelectShop];

	if (!s_CopyShop)
		return FALSE;

	return TRUE;
}



//
// 상점 붙여넣기
BOOL
CSetShopList::PasteShop()
{
	if (!s_CopyShop)	return FALSE;

	int	iSerial	=	s_lpEditShoplist->addShop(s_CopyShop);

	s_iSelectShop=	iSerial;

	ResetShopListBox(s_hWnd);
	DrawShopListWnd(s_hWnd);
	DrawShopInfoWnd(s_hWnd);

	return TRUE;
}
//
//	다얄로그 박스 프로시져
LRESULT CALLBACK
CSetShopList::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{	case WM_INITDIALOG	:
			InitBox(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;

		case	WM_COMMAND	:
			switch( LOWORD( wParam ) )
            {
				case IDC_SHOPCOPY				:
					if(!CopyShop())
						MessageBox(NULL, "Copy Error", "", NULL);
					break;
				case IDC_SHOPPASTE				:
					if(!PasteShop())
						MessageBox(NULL, "Paste Error", "Please copy the first!", NULL);
					break;
				case	IDC_SHOP_LIST_SORT_INDEX:				//JBC 상점 인덱스로 정렬. 08-08-18
					s_isSort = TRUE;				
					ResetShopListBox(hDlg);
					s_iSelectShop	=	s_listShop.GetSelectData();
					DrawShopListWnd(hDlg);
					DrawShopInfoWnd(hDlg);
					break;
				case	IDC_SHOP_LIST_SORT_NAME:				//JBC 상점 이름으로. 08-08-18
					{
						char strName[256];
						s_isSort = FALSE;
						ResetShopListBox(hDlg);
						strcpy(strName , s_listShop.m_pColumns[0].m_Strings.GetStr(s_iSelectShop));
						s_listShop.Sort();
						s_listShop.select(0 , strName);
						DrawShopListWnd(hDlg);
						DrawShopInfoWnd(hDlg);
					}					
					break;
				case	IDC_ADD					:
					AddShop(hDlg);
					break;

				case	IDC_EDIT				:
					EditShop(hDlg);
					break;

				case	IDC_REMOVE				:
					RemoveShop(hDlg);
					break;

				case	IDOK					:
					EndDialog(hDlg,IDOK);
					break;

				case	IDCANCEL				:
					EndDialog(hDlg,IDCANCEL);
					break;
			}
			break;
	}

	return (0L);
}	//	CSetShopList::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)

//
//	상점 리스트 이벤트 처리
LRESULT CALLBACK
CSetShopList::ProcShopListEvent(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN	:
		{
			s_iSelectShop	=	s_listShop.GetSelectData();
			DrawShopListWnd(hDlg);
			DrawShopInfoWnd(hDlg);
			break;
		}
	}

	return (0L);
}	//	CSetShopList::ProcShopListEvent(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)

LRESULT CALLBACK
CSetShopList::ProcShopInfoWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hDlg, &ps );

			s_dibShopInfo.Active();
			s_dibShopInfo.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hDlg, &ps);
			break;
		}

		case WM_VSCROLL			:
			if (s_sbItemList.VScroll(wParam))	DrawShopInfoWnd(s_hWnd);
			break;

		default				:	return( DefWindowProc( hDlg, uMsg, wParam, lParam ) );
	}

	return (0L);
}	//	CSetSpeech::ProcContestsListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
