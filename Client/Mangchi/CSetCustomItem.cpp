#include "CSetCustomItem.h"
#include "resource.h"
#include "cDRAW.h"
#include "cTEXT.h"
#include "CItem_Effect.h"
#include "setMakeItem.h"

#define WM_MOUSEWHEEL       0x020A	// 09.08.11

HWND					CSetCustomItem::s_hWnd;

CCustomItem				CSetCustomItem::s_itemEdit;	//	편집중인 상점
cSetItem				CSetCustomItem::s_editSetItem;
cLISTCONTROL			CSetCustomItem::s_listRegistPrefix;	//	접두사 리스트
int						CSetCustomItem::s_iPrefixListWndPos;
int						CSetCustomItem::s_iSelectRegistPrefix	=	0xffff;
BOOL					CSetCustomItem::s_bIsMakeSetItem		=	FALSE;
BOOL					CSetCustomItem::s_bIsCustomItem			=	FALSE;

cLISTCONTROL			CSetCustomItem::s_listPrefix;	//	접두사 리스트
int						CSetCustomItem::s_iSelectPrefix;
int						CSetCustomItem::s_iSelectPrefixType;

cDIBWND					CSetCustomItem::s_dibBasicItemList;
cDIBWND					CSetCustomItem::s_dibItemInfo;
int						CSetCustomItem::s_iSelectItem;
int						CSetCustomItem::s_iSelectItemCategory	=	eIK_HELM;
int						CSetCustomItem::s_iItemListWidth;
int						CSetCustomItem::s_iItemListHeight;
int						CSetCustomItem::s_iLineCountPerPage;
int						CSetCustomItem::s_iLineCount;
cSCROLL_BAR				CSetCustomItem::s_sbItemList;
cRECT					CSetCustomItem::s_rectItemList;
BOOL					CSetCustomItem::s_bIsSortItemList;
std::deque<int>			CSetCustomItem::s_deqFilterIndex;		// 10.02.12


CCustomItem*
CSetCustomItem::Open(HWND _hWnd,CCustomItem *_lpEditItem)
{
	s_itemEdit.reset();
	s_deqFilterIndex.clear();						// 10.02.12


	if (_lpEditItem)
	{
		memcpy(&s_itemEdit,_lpEditItem,sizeof(CCustomItem));

		cBASIC_ITEM	*lpItem		=	&g_aBasicItem[_lpEditItem->m_wBaseItem];

		s_iSelectItem			=	_lpEditItem->m_wBaseItem;
		s_iSelectItemCategory	=	lpItem->m_wKind;
	}
	else
	{
		s_itemEdit.reset();
		s_iSelectItem		=	0;
	}

	s_bIsCustomItem			=	s_itemEdit.m_bf1IsCustomItem;
	s_bIsMakeSetItem		=	FALSE;

	if (DialogBox( s_hInst, "SET_CUSTOM_ITEM",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	NULL;

	return	&s_itemEdit;
}

cSetItem*
CSetCustomItem::OpenForSetItem(HWND _hWnd,cSetItem *_lpEditItem)
{
	s_editSetItem.reset();
	s_deqFilterIndex.clear();						// 10.02.12

	if (_lpEditItem)
	{
		memcpy(&s_editSetItem,_lpEditItem,sizeof(cSetItem)-4);

		s_editSetItem.setComment(_lpEditItem->m_pItemComment);

		cBASIC_ITEM	*lpItem			=	&g_aBasicItem[_lpEditItem->m_wBaseItem];

		s_iSelectItem				=	_lpEditItem->m_wBaseItem;
		s_iSelectItemCategory		=	lpItem->m_wKind;
	}
	else
	{
		s_editSetItem.reset();
		s_editSetItem.setComment("valid");
		s_iSelectItem				=	0;

		cBASIC_ITEM	*lpItem			=	cBASIC_ITEM::GetItemByKind(s_iSelectItemCategory,&s_iSelectItem);

		if (lpItem)
		{
			s_iSelectItem				=	lpItem->m_iSerial;

			s_editSetItem.m_wBaseItem	=	s_iSelectItem;
		}
	}

	s_bIsCustomItem			=	s_itemEdit.m_bf1IsCustomItem;
	s_bIsMakeSetItem		=	TRUE;

	if (DialogBox( s_hInst, "SETITEM",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	NULL;

	return	&s_editSetItem;
}

//
//
void
CSetCustomItem::InitBox(HWND _hDlg)
{
	CCustomItem	tempForSetItem;
	CCustomItem	*lpEditItem	=	&s_itemEdit;


	if	(s_bIsMakeSetItem)
	{
		memcpy(&tempForSetItem,&s_editSetItem,sizeof(tempForSetItem));

		tempForSetItem.m_bf1IsCustomItem	=	s_editSetItem.m_bf1IsCustomItem;
		memcpy(&tempForSetItem.m_wIsAttachRandomPrefix,&s_editSetItem.m_wDropChance,(dITEM_PREFIX_COUNT+3)*sizeof(WORD));
		strcpy(tempForSetItem.m_strName,s_editSetItem.m_strName);

		lpEditItem	=	(CCustomItem	*)&tempForSetItem;
	}

	s_hWnd	=	_hDlg;

//	아이템 분류 
	{
		HWND	hComboItemCategory		=	GetDlgItem(_hDlg,IDC_ITEM_CATEGORY);

		cCOMBOBOX::Add(hComboItemCategory,dMSG_ALL_KIND_ITEM,dITEM_KIND_ALL);

		for (int iIndex=0;;iIndex++)
		{
			if (strcmp(g_strItemCategory[iIndex],"end")==0)
				break;

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

		HWND	hItemInfo	=	GetDlgItem(_hDlg,IDC_DETAIL_ITEM_INFO);
		s_dibItemInfo.lpszRegister		=	"IDC_DETAIL_ITEM_INFO";
		s_dibItemInfo.Init(hItemInfo,(WNDPROC)ProcItemInfoWnd);

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
//--------------------------------------------------------------------	등록된 접두사 윈도우 초기화
	{
		HWND	hRegistPrefixList	=	GetDlgItem(_hDlg,IDC_REGIST_PREFIX_LIST);

		s_listRegistPrefix.Init(hRegistPrefixList,6,FALSE,ProcRegistPrefixListEvent);		//	3개의 컬럼
		s_listRegistPrefix.AddColumn("Identification Code", 6 * 10);
		s_listRegistPrefix.AddColumn("Primary Prefix", 6 * 12);
		s_listRegistPrefix.AddColumn("Prefix Type", 6 * 20);
		s_listRegistPrefix.AddColumn("Quality", 6 * 5);
		s_listRegistPrefix.AddColumn("numeric1", 6 * 7);
		s_listRegistPrefix.AddColumn("Numbers2", 6 * 7);

		ResetRegistPrefixListBox(_hDlg);
		DrawRegistPrefixListWnd(_hDlg);

		SetDlgItemInt(_hDlg,IDC_DROP_LEVEL,lpEditItem->m_wDropLevel,FALSE);
	}

//	접두사 타입
	if	(s_bIsMakeSetItem	==	FALSE)
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
	if	(s_bIsMakeSetItem	==	FALSE)
	{
		HWND	hPrefixList			=	GetDlgItem(_hDlg,IDC_PREFIX_LIST);

		s_listPrefix.Init(hPrefixList,3,FALSE,ProcPrefixListEvent);		//	3개의 컬럼
		s_listPrefix.AddColumn("Identification Code", 6 * 10);
		s_listPrefix.AddColumn("Primary Prefix", 6 * 20);
		s_listPrefix.AddColumn("Prefix Type", 6 * 32);

		ResetPrefixListBox(_hDlg);
		DrawPrefixListWnd(_hDlg);
	}

//	이름
	SetDlgItemText(_hDlg,IDC_NAME,lpEditItem->m_strName);

//	갯수
	if	(s_bIsMakeSetItem)
	{
		SetDlgItemText(_hDlg,IDC_COMMENT,s_editSetItem.m_pItemComment);
	}
	else
		SetDlgItemInt(_hDlg,IDC_COUNT,max(lpEditItem->m_bCount,1),FALSE);

	if (s_bIsMakeSetItem	==	FALSE)
	{
//	유효기간
		{
			WORD	wDayHour	=	24*60;

			SetDlgItemInt(_hDlg,IDC_VALIDATE_DAY	,lpEditItem->m_bf20TermOfValidate/wDayHour			,FALSE);
			SetDlgItemInt(_hDlg,IDC_VALIDATE_HOUR	,(lpEditItem->m_bf20TermOfValidate%wDayHour)/60	,FALSE);
			SetDlgItemInt(_hDlg,IDC_VALIDATE_MINUTE	,lpEditItem->m_bf20TermOfValidate%60				,FALSE);
		}

//	접두사 관련 설정
		{
			if (lpEditItem->m_wIsAttachRandomPrefix)
				CheckDlgButton(_hDlg,IDC_RANDOM_PREFIX,TRUE);
			else
				CheckDlgButton(_hDlg,IDC_RANDOM_PREFIX,FALSE);
		}

//	당근 상점 관련
		{
			if (lpEditItem->m_bf1IsSellInCarrotShop)
				CheckDlgButton(_hDlg,IDC_SELL_IN_CARROT_SHOP,TRUE);
			else
				CheckDlgButton(_hDlg,IDC_SELL_IN_CARROT_SHOP,FALSE);
		}
	}

	if (s_bIsMakeSetItem	==	TRUE)
	{
		s_editSetItem.m_bf1IsCustomItem	=	TRUE;
	}
	else
	{
		CheckDlgButton(_hDlg,IDC_IS_CUSTOM_ITEM,FALSE);

		if (lpEditItem->m_bf1IsCustomItem)
			CheckDlgButton(_hDlg,IDC_IS_CUSTOM_ITEM,TRUE);
	}

//	아이템 퀄리티	
	{
		SetDlgItemInt(_hDlg,IDC_GENERATE_QUALITY,lpEditItem->m_wGenerateQuality	,FALSE);
		SetDlgItemInt(_hDlg,IDC_PREFIX_QUALITY	,50								,FALSE);

		SetDlgItemInt(_hDlg,IDC_OPTION1,lpEditItem->m_aOption[0]	,FALSE);
		SetDlgItemInt(_hDlg,IDC_OPTION2,lpEditItem->m_aOption[1]	,FALSE);
	}

	if	(s_bIsMakeSetItem)
	{
		s_bIsCustomItem						=	s_editSetItem.m_bf1IsCustomItem;

		int	iJob	=	0;

		for (iJob=0;;iJob++)
		{
			int	iID	=	g_aiEnableJobIDCList[iJob];

			if	(iID==	-1)
				break;
			
			CheckDlgButton(_hDlg,iID,s_editSetItem.isEnableJob(iJob)	);
		}

		CheckDlgButton(_hDlg,IDC_IS_QUEST_REWARD,s_editSetItem.m_bf1IsDropInQuest);
		CheckDlgButton(_hDlg,IDC_IS_NORMAL_FIELD_ARCA,s_editSetItem.m_bf1IsDropInNormalFieldArca);
		CheckDlgButton(_hDlg,IDC_IS_NORMAL_FIELD_MONSTER,s_editSetItem.m_bf1IsDropInNormalFieldMonster);
		CheckDlgButton(_hDlg,IDC_IS_IM_DUN_ARCA,s_editSetItem.m_bf1IsDropInTempDungeonArca);
		CheckDlgButton(_hDlg,IDC_IS_IM_DUN_MONSTER,s_editSetItem.m_bf1IsDropInTempDungeonMonster);

		SetDlgItemText(_hDlg,IDC_DROP_CHANCE,_ms("%d.%.2d",s_editSetItem.m_wDropChance/100,s_editSetItem.m_wDropChance%100));

		HWND	hComboItemReversionMethod	=	GetDlgItem(_hDlg,IDC_REVERSION_METHOD);

		for (int i=0;;i++)
		{
			if (stricmp(g_strItemReversionMethod[i],"end") == 0)
				break;

			cCOMBOBOX::Add(hComboItemReversionMethod,g_strItemReversionMethod[i],i);
		}

		cCOMBOBOX::SelectByData(hComboItemReversionMethod,s_editSetItem.m_bf4ReversionType);
	}

	ChangeCustomItemSetting(_hDlg);
	DrawItemInfoWnd(_hDlg);
}

void
CSetCustomItem::ChangeCustomItemSetting(HWND _hDlg)
{
//	아이템 퀄리티
	EnableWindow(GetDlgItem(_hDlg,IDC_GENERATE_QUALITY),TRUE);
	EnableWindow(GetDlgItem(_hDlg,IDC_PREFIX_QUALITY),TRUE);
	EnableWindow(GetDlgItem(_hDlg,IDC_OPTION1),TRUE);
	EnableWindow(GetDlgItem(_hDlg,IDC_OPTION2),TRUE);
	EnableWindow(GetDlgItem(_hDlg,IDC_MAKE_CUSTOM_ITEM),TRUE);

	if (s_bIsCustomItem)
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_GENERATE_QUALITY),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_PREFIX_QUALITY),FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_MAKE_CUSTOM_ITEM),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_OPTION1),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_OPTION2),FALSE);
	}

	ResetRegistPrefixListBox(_hDlg);
	DrawRegistPrefixListWnd(_hDlg);
}

//
//	기본 아이템 목록 설정
void
CSetCustomItem::ResetRegistPrefixListBox(HWND hwnd)
{
	s_listRegistPrefix.ResetItems();

	int	iIndex;

	CCustomItem	*lpItem	=	&s_itemEdit;

	if	(s_bIsMakeSetItem)
		lpItem	=	(CCustomItem	*)&s_editSetItem;

	for (iIndex=0;iIndex<dITEM_PREFIX_COUNT;iIndex++)
	{
		int	iPrefix	=	lpItem->m_aPrefix[iIndex].m_wPrefix;

		if (iPrefix	==	0xffff)
			break;

		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(iPrefix);

		if (!lpPrefix)
			continue;

		s_listRegistPrefix.AddItem(0,_ms("%.4d",lpPrefix->m_wDiscernmentCode),lpPrefix->m_wSerial);
		s_listRegistPrefix.AddItem(1,lpPrefix->m_str1stPrefix,lpPrefix->m_wSerial);
		s_listRegistPrefix.AddItem(2,g_aItemPrefixType[lpPrefix->m_wType].m_strName,lpPrefix->m_wSerial);

		if (s_bIsCustomItem)
		{
			s_listRegistPrefix.AddItem(3,"-",0);
			s_listRegistPrefix.AddItem(4,_ms("%d",lpItem->m_aPrefix[iIndex].m_aValue[0]),0);
			s_listRegistPrefix.AddItem(5,_ms("%d",lpItem->m_aPrefix[iIndex].m_aValue[1]),0);
		}
		else
		{
			s_listRegistPrefix.AddItem(3,_ms("%d％",lpItem->m_aPrefixGenerateQuality[iIndex]),0);
			s_listRegistPrefix.AddItem(4,"-",0);
			s_listRegistPrefix.AddItem(5,"-",0);
		}

		s_listRegistPrefix.IncreaseLineCounter();
	}

//	s_listRegistPrefix.Sort(0);
	if (!s_listRegistPrefix.Select(0,s_iSelectRegistPrefix))
	{
		if (!s_listRegistPrefix.Select(0))
			s_iSelectRegistPrefix	=	0xffff;
		else
			s_iSelectRegistPrefix	=	0;
	}
}

//
//	기본 아이템 목록 설정
void
CSetCustomItem::ResetPrefixListBox(HWND hwnd)
{
	s_listPrefix.ResetItems();

	int	iIndex,iCount;

	for (iIndex=0,iCount=0;iIndex<dMAX_ITEM_PREFIX_COUNT && iCount < g_itemPrefix.getCount();iIndex++)
	{
		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(iIndex);

		if (!lpPrefix)
			continue;

		iCount++;

		if (s_iSelectPrefixType	!=	0xffff	&&	lpPrefix->m_wType	!=	s_iSelectPrefixType)
			continue;

		s_listPrefix.AddItem(0,_ms("%.4d",lpPrefix->m_wDiscernmentCode),lpPrefix->m_wSerial);
		s_listPrefix.AddItem(1,lpPrefix->m_str1stPrefix,lpPrefix->m_wSerial);
		s_listPrefix.AddItem(2,g_aItemPrefixType[lpPrefix->m_wType].m_strName,lpPrefix->m_wSerial);
		s_listPrefix.IncreaseLineCounter();
	}

	s_listPrefix.Sort(0);

	if (!s_listPrefix.selectByData(0,s_iSelectPrefix))
	{
		if (!s_listPrefix.Select(0))
			s_iSelectPrefix	=	0xffff;
		else
			s_iSelectPrefix	=	s_listPrefix.GetData(0,0);
	}
}

//
//	기본 아이템 목록 설정
void
CSetCustomItem::ResetItemListBox(HWND hwnd,BOOL _bReset)
{
	HWND	hItemList		=	GetDlgItem(hwnd,IDC_LIST);
	HWND	hNotSortList	=	GetDlgItem(hwnd,IDC_NOT_SORT_LIST);
	int		iIndex			=	0;
	s_deqFilterIndex.clear();						// 10.02.12

	s_iLineCount			=	cBASIC_ITEM::GetCountByKind(s_iSelectItemCategory);
	s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

	if (!s_bIsSortItemList)
		hItemList	=	hNotSortList;

	if (_bReset)
	{
		cLISTBOX::Reset(hItemList);

		while(1)
		{
			cBASIC_ITEM	*lpItem	=	cBASIC_ITEM::GetItemByKind(s_iSelectItemCategory,&iIndex);

			if(!lpItem)
				break;
			s_deqFilterIndex.push_back(iIndex-1);			// 10.02.12
			cLISTBOX::Add(hItemList,lpItem->m_strName,lpItem->m_iSerial);
		}
	}
	//	아이템 설명을 그려준다.
}
// 10.02.12
// 아이템 검색
void
CSetCustomItem::FilterCustomItem(HWND _hDlg)
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
CSetCustomItem::PlaceItemToViewAble(HWND _hWnd,int _iItem)
{
	int		iIndex		=	s_sbItemList.m_iHeightPos;
	HWND	hListBox	=	GetDlgItem(_hWnd,IDC_LIST);
	HWND	hNotSortList=	GetDlgItem(_hWnd,IDC_NOT_SORT_LIST);

	if (!s_bIsSortItemList)
		hListBox	=	hNotSortList;

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
}

//
//	기본 아이템 리스트 그려주기
void
CSetCustomItem::DrawBasicItemListWnd(HWND _hDlg)
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
}

//
//	접두사리스트 그려주기
void
CSetCustomItem::DrawRegistPrefixListWnd(HWND _hDlg)
{
	s_listRegistPrefix.Draw();

	int	i,iCount=0;

	CCustomItem	*lpItem	=	&s_itemEdit;

	if	(s_bIsMakeSetItem)
		lpItem	=	(CCustomItem	*)&s_editSetItem;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)	
		if (lpItem->m_aPrefix[i].m_wPrefix	!=	0xffff)	
			iCount++;

	if (iCount	>=	dITEM_PREFIX_COUNT)
		EnableWindow(GetDlgItem(s_hWnd,IDC_ADD_PREFIX),FALSE);
	else
		EnableWindow(GetDlgItem(s_hWnd,IDC_ADD_PREFIX),TRUE);

	if (iCount	<=	0				)
		EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE_PREFIX),FALSE);
	else
		EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE_PREFIX),TRUE);

}

//
//	접두사리스트 그려주기
void
CSetCustomItem::DrawPrefixListWnd(HWND _hDlg)
{
	s_listPrefix.Draw();
}

//
//	아이템 정보 윈도우 그리기
void
CSetCustomItem::DrawItemInfoWnd(HWND _hDlg)
{
	if (s_bIsMakeSetItem	==	FALSE)
		CSetCustomItemList::DrawItemInfoWnd(_hDlg,&s_dibItemInfo,&s_itemEdit);
	else
		CSetCustomItemList::DrawItemInfoWnd(_hDlg,&s_dibItemInfo,NULL,&s_editSetItem);
}

//
//	
int
CSetCustomItem::SelectItem(int _iX,int _iY)
{
	int	x=5,y=5;

	if (s_iLineCount	<=	0)
		return	0xffff;

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

		if (!strName						)
			break;
		if (iCount	>=	s_iLineCountPerPage	)
			break;

		iIndex++;
		iCount++;

		if (_iY	>=	y	&&	_iY	<=	y	+	dICON_SHAPE_HEIGHT+8)	return	iSerial;

		y	+=	(dICON_SHAPE_HEIGHT+8);
	}

	return	0xffff;
}


//
//	아이템 리스트 윈도우 dib 프로콜
LRESULT CALLBACK
CSetCustomItem::ProcBasicItemListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)// 09.08.11
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

			if (s_bIsMakeSetItem)
				s_editSetItem.m_wBaseItem	=	s_iSelectItem;
			else
				s_itemEdit.m_wBaseItem		=	s_iSelectItem;

			DrawBasicItemListWnd(s_hWnd);
			DrawItemInfoWnd(s_hWnd);
			SetFocus(hwnd);
			break;
		}
		case WM_MOUSEWHEEL :		// 09.08.11
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
				DrawItemInfoWnd(s_hWnd);
				break;
				
				
			}

		case WM_VSCROLL			:
		{
			if (s_sbItemList.VScroll(wParam))	
				DrawBasicItemListWnd(s_hWnd);
			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}

LRESULT CALLBACK
CSetCustomItem::ProcItemInfoWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hDlg, &ps );

			s_dibItemInfo.Active();
			s_dibItemInfo.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hDlg, &ps);
			break;
		}

		default				:	return( DefWindowProc( hDlg, uMsg, wParam, lParam ) );
	}

	return (0L);
}

//
//	접두사 제거
void
CSetCustomItem::RemovePrefix(HWND _hDlg)
{
	CCustomItem	*lpItem	=	&s_itemEdit;

	if	(s_bIsMakeSetItem)
		lpItem	=	(CCustomItem	*)&s_editSetItem;

	if (lpItem->m_aPrefix[s_iSelectRegistPrefix].m_wPrefix	==	0xffff)
		return;

	int	i;

	for (i=s_iSelectRegistPrefix;i<dITEM_PREFIX_COUNT-1;i++)
	{
		lpItem->m_aPrefix[i].m_wPrefix		=	lpItem->m_aPrefix[i+1].m_wPrefix;
		lpItem->m_aPrefixGenerateQuality[i]	=	lpItem->m_aPrefixGenerateQuality[i+1];
	}

	lpItem->m_aPrefix[dITEM_PREFIX_COUNT-1].m_wPrefix	=	0xffff;

	ResetRegistPrefixListBox(_hDlg);
	DrawRegistPrefixListWnd(_hDlg);
	DrawItemInfoWnd(_hDlg);
}

//
//	접두사 추가
void
CSetCustomItem::AddPrefix(HWND _hDlg)
{
	cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(s_iSelectPrefix);
	if (!lpPrefix)
		return;

	CCustomItem	*lpItem	=	&s_itemEdit;

	if	(s_bIsMakeSetItem)
		lpItem	=	(CCustomItem	*)&s_editSetItem;

	int	iValidPrefixSlot	=	0xffff,i;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (lpItem->m_aPrefix[i].m_wPrefix	==	0xffff)
		{
			iValidPrefixSlot	=	i;
			break;
		}
	}

	if (iValidPrefixSlot	==	0xffff)
		return;

	BOOL	temp;
	int		iPrefixQuality				=	GetDlgItemInt(_hDlg,IDC_PREFIX_QUALITY,&temp,FALSE);

	if (!temp)
		return;

	cBASIC_ITEM		*lpBasicItem=	&g_aBasicItem[s_iSelectItem];

	if (!lpPrefix->isStickAbleItem(lpBasicItem->m_wKind))
		if (cMSG::YESNO(dMSG_OCCUR_PROBLEM,dMSG_SET_CUSTOM_ITEM_ERROR_FORM,lpPrefix->m_str1stPrefix)==IDNO)	
			return;

	lpItem->m_aPrefix[iValidPrefixSlot].m_wPrefix		=	lpPrefix->m_wSerial;
	lpItem->m_aPrefixGenerateQuality[iValidPrefixSlot]	=	iPrefixQuality;

	ResetRegistPrefixListBox(_hDlg);
	DrawRegistPrefixListWnd(_hDlg);
	DrawItemInfoWnd(_hDlg);
}

//
//	커스텀 아이템을 기반으로 아이템 생성
BOOL
CSetCustomItem::GenerateItemByCustomItem(cITEM *_lpItem,CCustomItem *_lpCustomItem)
{
	int		i,j;

	if (_lpCustomItem->m_wBaseItem	==	0xffff)
		return FALSE;

	if (_lpCustomItem->m_bf1IsCustomItem)
	{
		memcpy(_lpItem,_lpCustomItem,sizeof(CItemBaseInfo));

		return TRUE;
	}

	cBASIC_ITEM	*lpBasicItem	=	&g_aBasicItem[_lpCustomItem->m_wBaseItem];

	_lpItem->m_wBaseItem		=	_lpCustomItem->m_wBaseItem;
	_lpItem->m_bCount			=	_lpCustomItem->m_bCount;
	_lpItem->m_bDurability		=	(BYTE)lpBasicItem->m_wDurability;

	if (_lpItem->isItemPack())
		_lpItem->m_aOption[0]	=	(BYTE)_lpCustomItem->m_wGenerateQuality;
	else
	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex,iRange,iMinValue,iMaxValue;

//	옵션 1
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue+1;
			int	iValue		=	

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*_lpCustomItem->m_wGenerateQuality/100,iMaxValue);
		}

//	옵션 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue+1;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*_lpCustomItem->m_wGenerateQuality/100,iMaxValue);
		}

		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_USE_JEWEL)
			_lpItem->m_bDurability	=	_lpItem->m_aOption[iReferenceIndex];
	}

	if	(_lpCustomItem->m_wIsAttachRandomPrefix)
		return	FALSE;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		int	iPrefix	=	_lpCustomItem->m_aPrefix[i].m_wPrefix;

		if (iPrefix	==	0xffff)	break;

		_lpItem->m_aPrefix[i].m_wPrefix		=	iPrefix;

		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(iPrefix);

		for (j=0;j<2;j++)
		{
			int	iMinValue,iMaxValue,iRange;
			
			iMinValue	=	lpPrefix->m_aValue[j][0];
			iMaxValue	=	lpPrefix->m_aValue[j][1];
			iMaxValue	=	max(iMaxValue,iMinValue);
			iRange		=	iMaxValue-iMinValue;

			_lpItem->m_aPrefix[i].m_aValue[j]	=	min(iMinValue+iRange*_lpCustomItem->m_aPrefixGenerateQuality[i]/100,iMaxValue);
		}
	}

	return	TRUE;
}

//
//	커스텀 아이템을 기반으로 아이템 생성
BOOL
CSetCustomItem::GenerateItemBySetItem(cITEM *_lpItem,cSetItem	*_lpSetItem)
{
	int		i,j;

	if (_lpSetItem->m_wBaseItem	==	0xffff)
		return FALSE;

	if (_lpSetItem->m_bf1IsCustomItem)
	{
		memcpy(_lpItem,_lpSetItem,sizeof(CItemBaseInfo));

		return TRUE;
	}

	cBASIC_ITEM	*lpBasicItem	=	&g_aBasicItem[_lpSetItem->m_wBaseItem];

	_lpItem->m_wBaseItem		=	_lpSetItem->m_wBaseItem;
	_lpItem->m_bCount			=	_lpSetItem->m_bCount;
	_lpItem->m_bDurability		=	(BYTE)lpBasicItem->m_wDurability;

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex,iRange,iMinValue,iMaxValue;

//	옵션 1
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue+1;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*_lpSetItem->m_wGenerateQuality/100,iMaxValue);
		}

//	옵션 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue+1;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*_lpSetItem->m_wGenerateQuality/100,iMaxValue);
		}

		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_USE_JEWEL)
			_lpItem->m_bDurability	=	_lpItem->m_aOption[iReferenceIndex];
	}

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		int	iPrefix	=	_lpSetItem->m_aPrefix[i].m_wPrefix;

		if (iPrefix	==	0xffff)	break;

		_lpItem->m_aPrefix[i].m_wPrefix		=	iPrefix;

		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(iPrefix);

		for (j=0;j<2;j++)
		{
			int	iMinValue,iMaxValue,iRange;
			
			iMinValue	=	lpPrefix->m_aValue[j][0];
			iMaxValue	=	lpPrefix->m_aValue[j][1];
			iMaxValue	=	max(iMaxValue,iMinValue);
			iRange		=	iMaxValue-iMinValue+1;

			_lpItem->m_aPrefix[i].m_aValue[j]	=	min(iMinValue+iRange*_lpSetItem->m_aPrefixGenerateQuality[i]/100,iMaxValue);
		}
	}

	return	TRUE;
}

//
//	작업 마무리
BOOL
CSetCustomItem::FinishWork(HWND _hDlg,BOOL _bIsRequireMessage)
{
	BOOL			temp;
	int				i;
	cBASIC_ITEM		*lpBasicItem	=	&g_aBasicItem[s_iSelectItem];
	CCustomItem		*lpItem			=	&s_itemEdit;

	if	(s_bIsMakeSetItem)
		lpItem	=	(CCustomItem	*)&s_editSetItem;

	if (s_bIsMakeSetItem	==	FALSE)
	{
		lpItem->m_wIsAttachRandomPrefix	=	IsDlgButtonChecked(_hDlg,IDC_RANDOM_PREFIX);
		lpItem->m_bf1IsSellInCarrotShop	=	IsDlgButtonChecked(_hDlg,IDC_SELL_IN_CARROT_SHOP);
		lpItem->m_bf1IsCustomItem		=	IsDlgButtonChecked(_hDlg,IDC_IS_CUSTOM_ITEM);
		lpItem->m_wGenerateQuality		=	GetDlgItemInt(_hDlg,IDC_GENERATE_QUALITY,&temp,FALSE);
		lpItem->m_wDropLevel			=	GetDlgItemInt(_hDlg,IDC_DROP_LEVEL,&temp,FALSE);
		lpItem->m_wBaseItem				=	s_iSelectItem;
		GetDlgItemText(_hDlg,IDC_NAME,lpItem->m_strName,28);
	}
	else
	{
		s_editSetItem.m_bf1IsCustomItem	=	TRUE;
		s_editSetItem.m_wGenerateQuality=	GetDlgItemInt(_hDlg,IDC_GENERATE_QUALITY,&temp,FALSE);
		s_editSetItem.m_wDropLevel		=	GetDlgItemInt(_hDlg,IDC_DROP_LEVEL,&temp,FALSE);
		s_editSetItem.m_wBaseItem		=	s_iSelectItem;

		GetDlgItemText(_hDlg,IDC_NAME,s_editSetItem.m_strName,28);
	}

	//	이름

	if	(s_bIsMakeSetItem	==	FALSE)
	{
		int	iCount							=	GetDlgItemInt(_hDlg,IDC_COUNT	,&temp,FALSE);

		if	(_bIsRequireMessage	&& iCount	> lpBasicItem->m_wStackLimit)
			return	ERRMSG(dMSG_OCCUR_PROBLEM,dMSG_CURRENT_ITEM_STACK_ABLE_ITEM_COUNT_FORM,lpBasicItem->m_wStackLimit);

		lpItem->m_bCount					=	max(min(iCount,255),1);
	}
	else
	{
		s_editSetItem.m_bCount		=	1;

		char	strComment[1024];

		GetDlgItemText(_hDlg,IDC_COMMENT,strComment,1024);
		s_editSetItem.setComment(strComment);
	}

	BOOL	bIsCustom	=	FALSE;

	if (s_bIsMakeSetItem	==	FALSE)
	{
		if (lpItem->m_bf1IsCustomItem)
			bIsCustom	=	TRUE;
	}
	else
	{
		if (s_editSetItem.m_bf1IsCustomItem)
			bIsCustom	=	TRUE;
	}

	if (_bIsRequireMessage	&&	bIsCustom)
	{
		if (lpItem->m_aOption[0]	<	lpBasicItem->m_aValue[0][0])
			return	cMSG::Put("error","incorrect Option Value");
		if (lpItem->m_aOption[0]	>	lpBasicItem->m_aValue[0][1])
			return	cMSG::Put("error","incorrect Option Value");
		if (lpItem->m_aOption[1]	<	lpBasicItem->m_aValue[1][0])
			return	cMSG::Put("error","incorrect Option Value");
		if (lpItem->m_aOption[1]	>	lpBasicItem->m_aValue[1][1])
			return	cMSG::Put("error","incorrect Option Value");
	}

//	유효기간
	if	(s_bIsMakeSetItem	==	FALSE)
	{
		WORD	wDayHour	=	24*60;
		int		iDay		=	GetDlgItemInt(_hDlg,IDC_VALIDATE_DAY	,&temp,FALSE);
		int		iHour		=	GetDlgItemInt(_hDlg,IDC_VALIDATE_HOUR	,&temp,FALSE);
		int		iMinute		=	GetDlgItemInt(_hDlg,IDC_VALIDATE_MINUTE	,&temp,FALSE);

		lpItem->m_bf20TermOfValidate	=	iDay*wDayHour + iHour*60 + iMinute;
	}

	if	(s_bIsMakeSetItem	==	FALSE)
		if (lpItem->m_wIsAttachRandomPrefix)
		{
			for (i=0;i<dITEM_PREFIX_COUNT;i++)	
				lpItem->m_aPrefix[i].m_wPrefix	=	0xffff;

			return	TRUE;
		}

//	붙일 수 있는 접두사인지 체크
	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		int	iPrefix	=	lpItem->m_aPrefix[i].m_wPrefix;

		if (iPrefix	==	0xffff)
			break;

		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(iPrefix);

		if (_bIsRequireMessage	&& bIsCustom)
		{
			if (lpItem->m_aPrefix[i].m_aValue[0]	<	lpPrefix->m_aValue[0][0])
				return	cMSG::Put("error","incorrect Prefix Value");
			if (lpItem->m_aPrefix[i].m_aValue[0]	>	lpPrefix->m_aValue[0][1])
				return	cMSG::Put("error","incorrect Prefix Value");
			if (lpItem->m_aPrefix[i].m_aValue[1]	<	lpPrefix->m_aValue[1][0])
				return	cMSG::Put("error","incorrect Prefix Value");
			if (lpItem->m_aPrefix[i].m_aValue[1]	>	lpPrefix->m_aValue[1][1])
				return	cMSG::Put("error","incorrect Prefix Value");
		}

		if (!lpPrefix->isStickAbleItem(lpBasicItem->m_wKind))
			if (cMSG::YESNO(dMSG_OCCUR_PROBLEM,dMSG_SET_CUSTOM_ITEM_ERROR_FORM,lpPrefix->m_str1stPrefix)==IDNO)
				return FALSE;
	}

	if (s_bIsMakeSetItem)
	{
		char	strDropChance[20];

		GetDlgItemText(_hDlg,IDC_DROP_CHANCE,strDropChance,20);

		s_editSetItem.m_wDropChance						=	StringToNumber(strDropChance,100,2);;

		s_editSetItem.m_bf1IsDropInQuest				=	IsDlgButtonChecked(_hDlg,IDC_IS_QUEST_REWARD);
		s_editSetItem.m_bf1IsDropInNormalFieldArca		=	IsDlgButtonChecked(_hDlg,IDC_IS_NORMAL_FIELD_ARCA);
		s_editSetItem.m_bf1IsDropInNormalFieldMonster	=	IsDlgButtonChecked(_hDlg,IDC_IS_NORMAL_FIELD_MONSTER);
		s_editSetItem.m_bf1IsDropInTempDungeonArca		=	IsDlgButtonChecked(_hDlg,IDC_IS_IM_DUN_ARCA);
		s_editSetItem.m_bf1IsDropInTempDungeonMonster	=	IsDlgButtonChecked(_hDlg,IDC_IS_IM_DUN_MONSTER);

		s_editSetItem.m_bf4ReversionType				=	cCOMBOBOX::GetData(GetDlgItem(_hDlg,IDC_REVERSION_METHOD));

		int	iJob	=	0;

		s_editSetItem.m_uiEnableJobMask	=	0;

		for (iJob=0;;iJob++)
		{
			int	iID	=	g_aiEnableJobIDCList[iJob];

			if	(iID==	-1)
				break;

			if	(IsDlgButtonChecked(_hDlg,iID))
				s_editSetItem.m_uiEnableJobMask	+=	(1<<iJob);
		}
	}

//커스텀 아이템 정보 출력
	if	(s_bIsMakeSetItem	==	FALSE)
	{
		cITEM	item;

		item.reset();

		if (!GenerateItemByCustomItem(&item,&s_itemEdit))
			return	TRUE;

		cBASIC_ITEM	*lpBasicItem	=	&g_aBasicItem[item.m_wBaseItem];

		item.m_bDurability			=	(BYTE)lpBasicItem->m_wDurability;

		HANDLE		hMem;
		LPVOID		lpData;

		OpenClipboard(NULL);
		EmptyClipboard();

		hMem	=	GlobalAlloc(GHND,1024);
		lpData	=	GlobalLock(hMem);

		sprintf((char *)lpData,"[%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d",
							item.m_wBaseItem,item.m_dwSerial,item.m_bCount,item.m_bDurability,
							item.m_aOption[0],item.m_aOption[1],
							item.m_aPrefix[0].m_wPrefix,item.m_aPrefix[0].m_aValue[0],item.m_aPrefix[0].m_aValue[1],
							item.m_aPrefix[1].m_wPrefix,item.m_aPrefix[1].m_aValue[0],item.m_aPrefix[1].m_aValue[1],
							item.m_aPrefix[2].m_wPrefix,item.m_aPrefix[2].m_aValue[0],item.m_aPrefix[2].m_aValue[1],
							60);	
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT,hMem);
		CloseClipboard();

		MSGOUT("%s / [%d:%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d",lpItem->m_strName,item.m_wBaseItem,item.m_dwSerial,
						item.m_bCount,item.m_bDurability,
						item.m_aOption[0],item.m_aOption[1],
						item.m_aPrefix[0].m_wPrefix,item.m_aPrefix[0].m_aValue[0],item.m_aPrefix[0].m_aValue[1],
						item.m_aPrefix[1].m_wPrefix,item.m_aPrefix[1].m_aValue[0],item.m_aPrefix[1].m_aValue[1],
						item.m_aPrefix[2].m_wPrefix,item.m_aPrefix[2].m_aValue[0],item.m_aPrefix[2].m_aValue[1],
						60);	

	}

	return	TRUE;
}

//
//	다얄로그 박스 프로시져
LRESULT CALLBACK
CSetCustomItem::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{	
		case WM_INITDIALOG	:
			InitBox(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;

		case	WM_COMMAND	:
			switch( LOWORD( wParam ) )
            {
				case	IDOK					:
					if (FinishWork(hDlg))
						EndDialog(hDlg,IDOK);
					break;

				case	IDCANCEL				:
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_REMOVE_PREFIX		:
					RemovePrefix(hDlg);
					break;

				case	IDC_ADD_PREFIX			:
					AddPrefix(hDlg);
					break;

				case	IDC_IS_CUSTOM_ITEM		:
				{
					s_bIsCustomItem	=	IsDlgButtonChecked(hDlg,IDC_IS_CUSTOM_ITEM);

					ChangeCustomItemSetting(hDlg);

					break;
				}

				case	IDC_MAKE_CUSTOM_ITEM	:
				{
					FinishWork(hDlg,FALSE);

					cITEM	item;

					if (s_bIsMakeSetItem)
						item.copy((cITEM*)&s_editSetItem);
					else
						item.copy((cITEM*)&s_itemEdit);

					cITEM	*lpResult	=	cSetMakeItem::Open(hDlg,&item);

					if (lpResult)
					{
						cBASIC_ITEM	*lpItem;

						if (s_bIsMakeSetItem)
						{
							memcpy(&s_editSetItem,lpResult,sizeof(CItemBaseInfo));
							lpItem		=	&g_aBasicItem[lpResult->m_wBaseItem];
						}
						else
						{
							memcpy(&s_itemEdit,lpResult,sizeof(CItemBaseInfo));
							lpItem		=	&g_aBasicItem[lpResult->m_wBaseItem];
						}

						s_iSelectItem			=	lpResult->m_wBaseItem;
						s_iSelectItemCategory	=	lpItem->m_wKind;

						InitBox(hDlg);
					}
					break;
				}

				case	IDC_COPY_NAME			:
				{
					if (s_iSelectItem >0 && s_iSelectItem < dBASIC_ITEM_COUNT)
					{
						cBASIC_ITEM		*lpBasicItem		=	&g_aBasicItem[s_iSelectItem];

						SetDlgItemText(hDlg,IDC_NAME,lpBasicItem->m_strName);
					}
					break;

				}

				case	IDC_ITEM_CATEGORY		:
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
}

//
//
LRESULT CALLBACK 
CSetCustomItem::ProcPrefixListEvent(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)		// 09.08.11
{
	switch(message)
	{
	case WM_LBUTTONDOWN	:
		{
			s_iSelectPrefix	=	s_listPrefix.GetSelectData();
			DrawPrefixListWnd(hDlg);
			SetFocus(hDlg);
			break;
		}
	case WM_MOUSEWHEEL :
		{
			
			int zDelta = ((short)HIWORD(wParam));
			
			
			if	(zDelta>=0)
			{
				s_listPrefix.VScroll(SB_LINEUP);
			}
			else{
				s_listPrefix.VScroll(SB_LINEDOWN);
			}
			
			DrawPrefixListWnd(hDlg);
			break;
		}
		
		
	}

	return (0L);
}

//
//
LRESULT CALLBACK 
CSetCustomItem::ProcRegistPrefixListEvent(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
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
}