#include "CSetShop.h"
#include "resource.h"
#include "cDRAW.h"
#include "cTEXT.h"
#define WM_MOUSEWHEEL	0x020A
HWND					CSetShopItem::s_hWnd;

CShopItem CSetShopItem::s_shopitemEdit; // Store being edited
cLISTCONTROL CSetShopItem::s_listRegistPrefix; // prefix list
int CSetShopItem::s_iPrefixListWndPos;
int CSetShopItem::s_iSelectRegistPrefix = 0xffff;

cLISTCONTROL CSetShopItem::s_listPrefix; // prefix list
int						CSetShopItem::s_iSelectPrefix;
int						CSetShopItem::s_iSelectPrefixType;

cDIBWND					CSetShopItem::s_dibBasicItemList;
int						CSetShopItem::s_iSelectItem;
int						CSetShopItem::s_iSelectItemCategory	=	eIK_HELM;
int						CSetShopItem::s_iItemListWidth;
int						CSetShopItem::s_iItemListHeight;
int						CSetShopItem::s_iLineCountPerPage;
int						CSetShopItem::s_iLineCount;
cSCROLL_BAR				CSetShopItem::s_sbItemList;
cRECT					CSetShopItem::s_rectItemList;
BOOL					CSetShopItem::s_bIsSortItemList;
std::deque<int>			CSetShopItem::s_deqFilterIndex;		// 10.02.12

CShopItem*
CSetShopItem::Init(HWND hwnd,CShopItem *_lpEditItem)
{
	s_shopitemEdit.reset();
	s_deqFilterIndex.clear();					// 12.02.12

	if (_lpEditItem)
	{
		memcpy(&s_shopitemEdit,_lpEditItem,sizeof(CShopItem));

		cBASIC_ITEM	*lpItem		=	&g_aBasicItem[_lpEditItem->m_wBaseItem];

		s_iSelectItem			=	_lpEditItem->m_wBaseItem;
		s_iSelectItemCategory	=	lpItem->m_wKind;
	}
	else	s_iSelectItem		=	0;

	if (DialogBox( s_hInst, "SET_SHOP_ITEM_DATA",hwnd,(DLGPROC)ProcDlg)==IDCANCEL)	return	NULL;

	return	&s_shopitemEdit;
}	//	CSetShopItem::Init(HWND hwnd)

//
//
void
CSetShopItem::InitBox(HWND _hDlg)
{
	s_hWnd	=	_hDlg;

//	아이템 분류 
	{
		HWND	hComboItemCategory		=	GetDlgItem(_hDlg,IDC_ITEM_CATEGORY);

		cCOMBOBOX::Add(hComboItemCategory,dMSG_ALL_KIND_ITEM,dITEM_KIND_ALL);

		for (int iIndex=0;;iIndex++)
		{
			if (strcmp(g_strItemCategory[iIndex],"end")==0)	break;

			cCOMBOBOX::Add(hComboItemCategory,g_strItemCategory[iIndex],iIndex);
		}

		cCOMBOBOX::SelectByData(hComboItemCategory,s_iSelectItemCategory);

	}
//	기본 아이템 리스트 윈도우 초기화 
	{
		MoveWindow(GetDlgItem(_hDlg,IDC_LIST),-10000,-10000,100,100,FALSE);
		MoveWindow(GetDlgItem(_hDlg,IDC_NOT_SORT_LIST),-10000,-10000,100,100,FALSE);

		HWND	hItemList	=	GetDlgItem(_hDlg,IDC_BASIC_ITEM_LIST);

		s_dibBasicItemList.lpszRegister	=	"BASIC_ITEM_LIST_WINDOW";
		s_dibBasicItemList.Init(hItemList,(WNDPROC)ProcBasicItemListWnd);

		s_sbItemList.Init(hItemList);		//	스크롤바 초기화
		s_sbItemList.InitHeight();

		s_iItemListWidth				=	s_dibBasicItemList.width();		//	아이템 리스트 너비
		s_iItemListHeight				=	s_dibBasicItemList.height();		//	아이템 리스트 높이
		s_iLineCountPerPage				=	(s_iItemListHeight-10)	/	(dICON_SHAPE_HEIGHT+8);
		s_iLineCount					=	cBASIC_ITEM::GetCountByKind(s_iSelectItemCategory);

		s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

		ResetItemListBox(_hDlg,TRUE);
		PlaceItemToViewAble(_hDlg,s_iSelectItem);
		DrawBasicItemListWnd(_hDlg);
	}

//	등록된 접두사 윈도우 초기화
	{
		HWND	hRegistPrefixList	=	GetDlgItem(_hDlg,IDC_REGIST_PREFIX_LIST);

		s_listRegistPrefix.Init(hRegistPrefixList,3,FALSE,ProcRegistPrefixListEvent);		//	3개의 컬럼
		s_listRegistPrefix.AddColumn("Identification Code", 6 * 10);
		s_listRegistPrefix.AddColumn("Primary prefix", 6 * 20);
		s_listRegistPrefix.AddColumn("Prefix type", 6 * 32);

		ResetRegistPrefixListBox(_hDlg);
		DrawRegistPrefixListWnd(_hDlg);
	}

//	접두사 타입
	{
		HWND	hComboItemPrefixType	=	GetDlgItem(_hDlg,IDC_ITEM_PREFIX_TYPE);

		cCOMBOBOX::Add(hComboItemPrefixType,dMSG_ALL ,0xffff);

		for (int iIndex=0;;iIndex++)
		{
			if (g_aItemPrefixType[iIndex].m_wType	==	0xffff)	break;

			cCOMBOBOX::Add(hComboItemPrefixType,g_aItemPrefixType[iIndex].m_strName,g_aItemPrefixType[iIndex].m_wType);
		}

		cCOMBOBOX::SelectByData(hComboItemPrefixType,s_iSelectPrefixType);
	}	//	접두사 타입
	
//	접두사 윈도우 초기화
	{
		HWND	hPrefixList			=	GetDlgItem(_hDlg,IDC_PREFIX_LIST);

		s_listPrefix.Init(hPrefixList,3,FALSE,ProcPrefixListEvent);		//	3개의 컬럼
		s_listPrefix.AddColumn("Identification code", 6 * 10);
		s_listPrefix.AddColumn("Primary prefix", 6 * 20);
		s_listPrefix.AddColumn("Prefix type", 6 * 32);

		ResetPrefixListBox(_hDlg);
		DrawPrefixListWnd(_hDlg);
	}

//	갯수
	{
		SetDlgItemInt(_hDlg,IDC_COUNT,s_shopitemEdit.m_bCount,FALSE);
		SetDlgItemInt(_hDlg,IDC_UPKEEP_HOUR,s_shopitemEdit.m_bf15UpkeepHour,FALSE);
	}


//	가격 관련 설정
	{
		if (s_shopitemEdit.m_dwPrice	==	0)	CheckDlgButton(_hDlg,IDC_USE_ORIGINAL,TRUE);
		else									CheckDlgButton(_hDlg,IDC_USE_ORIGINAL,FALSE);

		SetDlgItemInt(_hDlg,IDC_PRICE,s_shopitemEdit.m_dwPrice,FALSE);
	}

//	접두사 관련 설정
	{
		if (s_shopitemEdit.m_bf1IsRandomPrefix)	CheckDlgButton(_hDlg,IDC_RANDOM_PREFIX,TRUE);
		else									CheckDlgButton(_hDlg,IDC_RANDOM_PREFIX,FALSE);

	}

//	아이템 제한 레벨
	SetDlgItemInt(_hDlg,IDC_LIMIT_LEVEL,s_shopitemEdit.m_bf10ChangeLimitLevel,FALSE);	

//	아이템 퀄리티	
	{
		SetDlgItemInt(_hDlg,IDC_GENERATE_QUALITY,s_shopitemEdit.m_wGenerateQuality,FALSE);
		SetDlgItemInt(_hDlg,IDC_PREFIX_QUALITY,s_shopitemEdit.m_wPrefixQuality,FALSE);
	}
}	//	CSetShopItem::InitBox(HWND _hDlg)

//
//	기본 아이템 목록 설정
void
CSetShopItem::ResetRegistPrefixListBox(HWND hwnd)
{
	s_listRegistPrefix.ResetItems();

	int	iIndex;

	for (iIndex=0;iIndex<dITEM_PREFIX_COUNT;iIndex++)
	{
		int	iPrefix	=	s_shopitemEdit.m_aPrefix[iIndex].m_wPrefix;

		if (iPrefix	==	0xffff) break;

		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(iPrefix);

		if (!lpPrefix)	continue;

		s_listRegistPrefix.AddItem(0,_ms("%.4d",lpPrefix->m_wDiscernmentCode),lpPrefix->m_wSerial);
		s_listRegistPrefix.AddItem(1,lpPrefix->m_str1stPrefix,lpPrefix->m_wSerial);
		s_listRegistPrefix.AddItem(2,g_aItemPrefixType[lpPrefix->m_wType].m_strName,lpPrefix->m_wSerial);
		s_listRegistPrefix.IncreaseLineCounter();
	}

	if (!s_listRegistPrefix.Select(0,s_iSelectRegistPrefix))
	{
		if (!s_listRegistPrefix.Select(0))	s_iSelectRegistPrefix	=	0xffff;
		else								s_iSelectRegistPrefix	=	0;
	}
}	//	cSET_ITEM_PREFIX::ResetPrefixListBox(HWND hwnd,BOOL _bReset)

//
//	기본 아이템 목록 설정
void
CSetShopItem::ResetPrefixListBox(HWND hwnd)
{
	s_listPrefix.ResetItems();

	int	iIndex,iCount;

	for (iIndex=0,iCount=0;iIndex<dMAX_ITEM_PREFIX_COUNT && iCount < g_itemPrefix.getCount();iIndex++)
	{
		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(iIndex);

		if (!lpPrefix)	continue;

		iCount++;

		if (s_iSelectPrefixType	!=	0xffff	&&	lpPrefix->m_wType	!=	s_iSelectPrefixType)	continue;

		s_listPrefix.AddItem(0,_ms("%.4d",lpPrefix->m_wDiscernmentCode),lpPrefix->m_wSerial);
		s_listPrefix.AddItem(1,lpPrefix->m_str1stPrefix,lpPrefix->m_wSerial);
		s_listPrefix.AddItem(2,g_aItemPrefixType[lpPrefix->m_wType].m_strName,lpPrefix->m_wSerial);
		s_listPrefix.IncreaseLineCounter();
	}

	s_listPrefix.Sort(0);

	if (!s_listPrefix.selectByData(0,s_iSelectPrefix))
	{
		if (!s_listPrefix.Select(0))	s_iSelectPrefix	=	0xffff;
		else							s_iSelectPrefix	=	s_listPrefix.GetData(0,0);
	}
}	//	cSET_ITEM_PREFIX::ResetPrefixListBox(HWND hwnd,BOOL _bReset)

//
//	기본 아이템 목록 설정
void
CSetShopItem::ResetItemListBox(HWND hwnd,BOOL _bReset)
{
	HWND	hItemList		=	GetDlgItem(hwnd,IDC_LIST);
	HWND	hNotSortList	=	GetDlgItem(hwnd,IDC_NOT_SORT_LIST);
	int		iIndex			=	0;
	s_deqFilterIndex.clear();					// 12.02.12

	s_iLineCount			=	cBASIC_ITEM::GetCountByKind(s_iSelectItemCategory);
	s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

	if (!s_bIsSortItemList)	hItemList	=	hNotSortList;

	if (_bReset)
	{
		cLISTBOX::Reset(hItemList);

		while(1)
		{
			cBASIC_ITEM	*lpItem	=	cBASIC_ITEM::GetItemByKind(s_iSelectItemCategory,&iIndex);

			if(!lpItem)	break;

			s_deqFilterIndex.push_back(iIndex-1);		// 12.02.12

			cLISTBOX::Add(hItemList,lpItem->m_strName,lpItem->m_iSerial);
		}
	}

	//	아이템 설명을 그려준다.
}	//	cSET_BASIC_ITEM::ResetItemListBox(HWND hwnd)
// 10.02.11 추가   아이템 검색
void
CSetShopItem::FilterCustomItem(HWND _hDlg)
{
	char strFilter[256] = "";
	GetDlgItemText(_hDlg,IDC_EDIT_CUSTOM_FILTER,strFilter,256);
	int iFilterNameLen	= strlen(strFilter);
		
	if(iFilterNameLen<=0)
	{
		ResetItemListBox(_hDlg, TRUE);
		DrawBasicItemListWnd(_hDlg);
		PlaceItemToViewAble(_hDlg,s_iSelectItem);
		DrawBasicItemListWnd(_hDlg);
		return;
	}

	HWND	hItemList		=	GetDlgItem(_hDlg,IDC_LIST);
	HWND	hNotSortList	=	GetDlgItem(_hDlg,IDC_NOT_SORT_LIST);
	
	if (!s_bIsSortItemList)	hItemList	=	hNotSortList;

	cLISTBOX::Reset(hItemList);
	int iCount = 0;

	for (int i=0;i<s_deqFilterIndex.size();++i)
	{
		cBASIC_ITEM	*lpItem		=	&g_aBasicItem[s_deqFilterIndex[i]];
		
		if(!lpItem)
			continue;

		int iItemNameLen	= strlen(lpItem->m_strName);
		int iLenGap			= iItemNameLen - iFilterNameLen;

		if(iLenGap<0)
			continue;

		for(int iName=0;iName<=iLenGap;++iName)
		{
			if(strnicmp(strFilter,&lpItem->m_strName[iName], iFilterNameLen) == 0 )
			{
				iCount++;
				cLISTBOX::Add(hItemList,lpItem->m_strName,lpItem->m_iSerial);
				break;		
			}
		}
	}

	s_iLineCount			=	iCount;
	s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

	DrawBasicItemListWnd(_hDlg);
	PlaceItemToViewAble(_hDlg,s_iSelectItem);
	DrawBasicItemListWnd(_hDlg);
}
//
//	_iItem을 보이게 스크롤바의 위치를 조절한다.
void
CSetShopItem::PlaceItemToViewAble(HWND _hWnd,int _iItem)
{
	int		iIndex		=	s_sbItemList.m_iHeightPos;
	HWND	hListBox	=	GetDlgItem(_hWnd,IDC_LIST);
	HWND	hNotSortList=	GetDlgItem(_hWnd,IDC_NOT_SORT_LIST);

	if (!s_bIsSortItemList)	hListBox	=	hNotSortList;

	int		iItemPos	=	cLISTBOX::GetIndexByData(hListBox,_iItem);

	if (iItemPos	!=	CB_ERR)
	{
		if (iItemPos	<	iIndex)
		{
			s_sbItemList.SetHeightPos(iItemPos);
		}

		if (iItemPos	>	iIndex+s_iLineCountPerPage-1)
		{
			iIndex	=	iItemPos-s_iLineCountPerPage+1;
			s_sbItemList.SetHeightPos(iIndex);
		}
	}
}	//	cSET_BASIC_ITEM::PlaceItemToViewAble(HWND _hWnd,int _iItem)

//
//	기본 아이템 리스트 그려주기
void
CSetShopItem::DrawBasicItemListWnd(HWND _hDlg)
{
	int	x=4,y=5;

	s_dibBasicItemList.Active();

	y					=	(cDRAW::HEIGHT-s_iLineCountPerPage*(dICON_SHAPE_HEIGHT+8))/2;

	s_rectItemList.Set(0,0,cDRAW::WIDTH,cDRAW::HEIGHT);

	if (s_iLineCount	<=	0)
	{
		cDRAW::Fill(_SKYBLUE);

		cTEXT::PushShadow(FALSE);
		cTEXT::Put(5,5,0,dMSG_ITEM_NOT_EXIST);
		cTEXT::PopShadow();

		EnableWindow(GetDlgItem(_hDlg,IDC_COPY),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE),FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_COPY),TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT),TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE),TRUE);

		cDRAW::Fill(_SKYBLUE);

		int		iIndex		=	s_sbItemList.m_iHeightPos;
		int		iCount		=	0;

		HWND	hListBox	=	GetDlgItem(_hDlg,IDC_LIST);
		HWND	hNotSortList=	GetDlgItem(_hDlg,IDC_NOT_SORT_LIST);

		if (!s_bIsSortItemList)	hListBox	=	hNotSortList;

		if (!cBASIC_ITEM::IsExist(s_iSelectItem,s_iSelectItemCategory))	s_iSelectItem	=	cLISTBOX::GetData(hListBox,0);

		while(1)
		{
			char	*strName	=	cLISTBOX::GetText(hListBox,iIndex);
			int		iSerial		=	cLISTBOX::GetData(hListBox,iIndex);

			if (!strName						)	break;
			if (iCount	>=	s_iLineCountPerPage	)	break;

			iIndex++;
			iCount++;

			if (iSerial	==	s_iSelectItem)
			{
				cDRAW::Fill		(0			,x  ,y+2,s_iItemListWidth-2,y+dICON_SHAPE_HEIGHT+6);
				cDRAW::Fill		(_LTBLUE	,x-1,y+1,s_iItemListWidth-3,y+dICON_SHAPE_HEIGHT+5);
				cDRAW::Box		(_DEEPGRAY	,x-1,y+1,s_iItemListWidth-3,y+dICON_SHAPE_HEIGHT+5);

				g_smiIconItem.put(g_aBasicItem[iSerial].m_wIconShape,x+1,y+4);

				if (g_aBasicItem[iSerial].isUnique())
				{
					cTEXT::Put(x+1+36,y+4+2,GOLDYELLOW,"★");
					cTEXT::Put(x+1+36,y+4+2,GOLDYELLOW,_ms("   %s",strName));
				}
				else
				if (g_aBasicItem[iSerial].isExtraItem())
				{
					cTEXT::Put(x+1+36,y+4+2,LTCYAN,"♠");
					cTEXT::Put(x+1+36,y+4+2,LTCYAN,_ms("   %s",strName));
				}
				else
					cTEXT::Put(x+1+36,y+4+2,WHITE,strName);
				
				cTEXT::Put		(x+1+36,y+4+18,LTGRAY,g_strItemCategory[g_aBasicItem[iSerial].m_wKind]);
			}
			else
			{
				cDRAW::Fill		(0			,x  ,y+2,s_iItemListWidth-2,y+dICON_SHAPE_HEIGHT+6);
				cDRAW::Fill		(_DEEPGRAY	,x-2,y  ,s_iItemListWidth-4,y+dICON_SHAPE_HEIGHT+4);
				cDRAW::Box		(0			,x-2,y  ,s_iItemListWidth-4,y+dICON_SHAPE_HEIGHT+4);
				g_smiIconItem.put	(g_aBasicItem[iSerial].m_wIconShape,x	,y+3);

				if (g_aBasicItem[iSerial].isUnique())
				{
					cTEXT::Put(x+36,y+3+2,GOLDYELLOW,"★");
					cTEXT::Put(x+36,y+3+2,GOLDYELLOW,_ms("   %s",strName));
				}
				else
				if (g_aBasicItem[iSerial].isExtraItem())
				{
					cTEXT::Put(x+36,y+3+2,LTCYAN,"♠");
					cTEXT::Put(x+36,y+3+2,LTCYAN,_ms("   %s",strName));
				}
				else
					cTEXT::Put(x+36	,y+3+2,WHITE,strName);

				cTEXT::Put		(x+36	,y+3+18,LTGRAY,g_strItemCategory[g_aBasicItem[iSerial].m_wKind]);
			}

			y	+=	(dICON_SHAPE_HEIGHT+8);
		}
	}

	s_dibBasicItemList.Draw();
}	//	CSetShopItem::DrawBasicItemListWnd(HWND _hDlg)

//
//	접두사리스트 그려주기
void
CSetShopItem::DrawRegistPrefixListWnd(HWND _hDlg)
{
	s_listRegistPrefix.Draw();
	int	i,iCount=0;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)	
		if (s_shopitemEdit.m_aPrefix[i].m_wPrefix	!=	0xffff)	
			iCount++;

	if (iCount	>=	dITEM_PREFIX_COUNT)	EnableWindow(GetDlgItem(s_hWnd,IDC_ADD_PREFIX),FALSE);
	else								EnableWindow(GetDlgItem(s_hWnd,IDC_ADD_PREFIX),TRUE);
	if (iCount	<=	0				)	EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE_PREFIX),FALSE);
	else								EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE_PREFIX),TRUE);

}//CSetShopItem::DrawRegistPrefixListWnd(HWND _hDlg)

//
//	접두사리스트 그려주기
void
CSetShopItem::DrawPrefixListWnd(HWND _hDlg)
{
	s_listPrefix.Draw();
}//CSetShopItem::DrawPrefixListWnd(HWND _hDlg)


//
//	
int
CSetShopItem::SelectItem(int _iX,int _iY)
{
	int	x=5,y=5;

	if (s_iLineCount	<=	0)	return	0xffff;

	y					=	(s_iItemListHeight	-	s_iLineCountPerPage	*(dICON_SHAPE_HEIGHT+8))/2;

	int		iIndex		=	s_sbItemList.m_iHeightPos;
	int		iCount		=	0;

	HWND	hListBox	=	GetDlgItem(s_hWnd,IDC_LIST);
	HWND	hNotSortList=	GetDlgItem(s_hWnd,IDC_NOT_SORT_LIST);

	if (!s_bIsSortItemList)	hListBox	=	hNotSortList;

	if (!s_rectItemList.isIN(_iX,_iY))	return	0xffff;

	while(1)
	{
		char	*strName	=	cLISTBOX::GetText(hListBox,iIndex);
		int		iSerial		=	cLISTBOX::GetData(hListBox,iIndex);

		if (!strName						)	break;
		if (iCount	>=	s_iLineCountPerPage	)	break;

		iIndex++;
		iCount++;

		if (_iY	>=	y	&&	_iY	<=	y	+	dICON_SHAPE_HEIGHT+8)	return	iSerial;

		y	+=	(dICON_SHAPE_HEIGHT+8);
	}

	return	0xffff;
}	//	cSET_BASIC_ITEM::SelectItem(HWND hwnd,int x,int y)


//
//	아이템 리스트 윈도우 dib 프로콜
LRESULT CALLBACK
CSetShopItem::ProcBasicItemListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)		// 09.08.12
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibBasicItemList.Active();
			s_dibBasicItemList.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_LBUTTONDOWN	:
		{
			int item		=	SelectItem(LOWORD(lParam),HIWORD(lParam));

			if(item			==	0xffff		)	break;
			if(item			==	s_iSelectItem)	break;

			s_iSelectItem	=	item;

			DrawBasicItemListWnd(s_hWnd);
			SetFocus(hwnd);
			break;
		}

		case WM_MOUSEWHEEL		:
			{
				int zDelta = ((short)HIWORD(wParam));
				
				
				if	(zDelta>=0)
				{
					s_sbItemList.VScroll(SB_LINEUP);
				}
				else{
					s_sbItemList.VScroll(SB_LINEDOWN);
				}
				DrawBasicItemListWnd(s_hWnd);
				
				break;
			}
		case WM_VSCROLL			:
		{
			if (s_sbItemList.VScroll(wParam))	DrawBasicItemListWnd(s_hWnd);
			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}
	//	cSET_BASIC_ITEM::s_procItemListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)

//
//	접두사 제거
void
CSetShopItem::RemovePrefix(HWND _hDlg)
{
	if (s_shopitemEdit.m_aPrefix[s_iSelectRegistPrefix].m_wPrefix	==	0xffff)	return;

	int	i;

	for (i=s_iSelectRegistPrefix;i<dITEM_PREFIX_COUNT-1;i++)	s_shopitemEdit.m_aPrefix[i].m_wPrefix	=	s_shopitemEdit.m_aPrefix[i+1].m_wPrefix;
	s_shopitemEdit.m_aPrefix[dITEM_PREFIX_COUNT-1].m_wPrefix	=	0xffff;

	ResetRegistPrefixListBox(_hDlg);
	DrawRegistPrefixListWnd(_hDlg);
}	//	CSetShopItem::RemovePrefix(HWND _hDlg)

//
//	접두사 추가
void
CSetShopItem::AddPrefix(HWND _hDlg)
{
	cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(s_iSelectPrefix);
	if (!lpPrefix)	return;

	int	iValidPrefixSlot	=	0xffff,i;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (s_shopitemEdit.m_aPrefix[i].m_wPrefix	==	0xffff)
		{
			iValidPrefixSlot	=	i;
			break;
		}
	}

	if (iValidPrefixSlot	==	0xffff)	return;

	s_shopitemEdit.m_aPrefix[iValidPrefixSlot].m_wPrefix	=	lpPrefix->m_wSerial;


	ResetRegistPrefixListBox(_hDlg);
	DrawRegistPrefixListWnd(_hDlg);
}	//	CSetShopItem::AddPrefix(HWND _hDlg)

//
//	작업 마무리
BOOL
CSetShopItem::FinishWork(HWND _hDlg)
{
	BOOL	temp;

	s_shopitemEdit.m_bf1IsRandomPrefix	=	IsDlgButtonChecked(_hDlg,IDC_RANDOM_PREFIX);
	s_shopitemEdit.m_bf10ChangeLimitLevel=	GetDlgItemInt(_hDlg,IDC_LIMIT_LEVEL,&temp,FALSE);

	s_shopitemEdit.m_wGenerateQuality	=	GetDlgItemInt(_hDlg,IDC_GENERATE_QUALITY,&temp,FALSE);
	s_shopitemEdit.m_wPrefixQuality		=	GetDlgItemInt(_hDlg,IDC_PREFIX_QUALITY,&temp,FALSE);
	s_shopitemEdit.m_bf15UpkeepHour		=	GetDlgItemInt(_hDlg,IDC_UPKEEP_HOUR,&temp,FALSE);

	if (IsDlgButtonChecked(_hDlg,IDC_USE_ORIGINAL))
			s_shopitemEdit.m_dwPrice	=	0;
	else	s_shopitemEdit.m_dwPrice	=	GetDlgItemInt(_hDlg,IDC_PRICE	,&temp,FALSE);

	s_shopitemEdit.m_wBaseItem			=	s_iSelectItem;

	int	iCount							=	GetDlgItemInt(_hDlg,IDC_COUNT	,&temp,FALSE);

	s_shopitemEdit.m_bCount				=	min(iCount,255);

	return	TRUE;
}	//	CSetShopItem::FinishWork(HWND _hDlg)



//
//	다얄로그 박스 프로시져
LRESULT CALLBACK
CSetShopItem::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				case	IDOK					:
					if (FinishWork(hDlg))	EndDialog(hDlg,IDOK);
					break;

				case	IDCANCEL				:
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_REMOVE_PREFIX		:
					RemovePrefix(hDlg);
					break;

				case	IDC_ADD_PREFIX		:
					AddPrefix(hDlg);
					break;

				case	IDC_ITEM_CATEGORY	:
				{
					switch (HIWORD(wParam))
                    {	case	CBN_SELCHANGE	:
						{
							int	iKind		=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_ITEM_CATEGORY));

							if (s_iSelectItemCategory	!=	iKind)
							{
								s_iSelectItemCategory	=	iKind;
								ResetItemListBox(hDlg,TRUE);
								DrawBasicItemListWnd(hDlg);
								PlaceItemToViewAble(hDlg,s_iSelectItem);
								DrawBasicItemListWnd(hDlg);
							}
							break;
						}
					}
					break;
				}
				
				case	IDC_ITEM_PREFIX_TYPE	:
					{
						switch (HIWORD(wParam))
						{
						case	CBN_SELCHANGE	:
							{
								int	iKind		=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_ITEM_PREFIX_TYPE));
								
								if (s_iSelectPrefixType	!=	iKind)
								{
									s_iSelectPrefixType	=	iKind;
									ResetPrefixListBox(hDlg);
									DrawPrefixListWnd(hDlg);
								}
								break;
							}
						}
						break;
					}
				case IDC_EDIT_CUSTOM_FILTER	:	// 10.02.12
					{
						if	(HIWORD(wParam) != EN_CHANGE)
							break;
						
						FilterCustomItem(hDlg);
						break;
					}
			}
			break;
	}

	return (0L);
}	//	CSetShopList::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)

//
//
LRESULT CALLBACK 
CSetShopItem::ProcPrefixListEvent(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_LBUTTONDOWN	:
		{
			s_iSelectPrefix	=	s_listPrefix.GetSelectData();
			DrawPrefixListWnd(hDlg);
			break;
		}
	}

	return (0L);
}	//	CSetShopItem::ProcPrefixListEvent(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)

//
//
LRESULT CALLBACK 
CSetShopItem::ProcRegistPrefixListEvent(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_LBUTTONDOWN	:
		{
			s_iSelectRegistPrefix	=	s_listRegistPrefix.GetSelectLine();
			DrawRegistPrefixListWnd(hDlg);
			break;
		}
	}

	return (0L);
}	//	CSetShopItem::ProcRegistPrefixListEvent(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)