#include "setPieceItem.h"
#include "setItem.h"
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

HWND					cSetPieceItem::s_hWnd;
cLISTCONTROL			cSetPieceItem::s_listItemPiece;
cDIBWND					cSetPieceItem::s_dibPieceItemList;
cDIBWND					cSetPieceItem::s_dibItemInfo;
cSCROLL_BAR				cSetPieceItem::s_sbPieceItemList;
BOOL					cSetPieceItem::s_bIsLostVirgin;
cPieceItem				cSetPieceItem::s_makePieceItem;

int						cSetPieceItem::s_iSelectPieceItem	=	1;
int						cSetPieceItem::s_iSelectItemPiece;
std::deque<int>			cSetPieceItem::s_deqFilterIndex;

BOOL
cSetPieceItem::Open(HWND _hWnd)//	시작
{
	if (DialogBox(s_hInst, "PICEITEM_LIST",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	FALSE;

	return	TRUE;
}

void
cSetPieceItem::InitDlg(HWND _hDlg)	//	시작		09.08.17
{
	s_hWnd	=	_hDlg;

	s_deqFilterIndex.clear();

	for(int i=0;i<g_pieceItemManager.getMaxCount(); ++i)
	{
		
		cPieceItem	*lpPieceItem	=	g_pieceItemManager.get(i);
		if(!lpPieceItem)
			continue;
		s_deqFilterIndex.push_back(i);

	}


	if (g_pieceItemManager.get(s_iSelectPieceItem)	==	NULL)
	{
		cPieceItem	*lpPieceItem	=	g_pieceItemManager.getFirst(1);

		if (lpPieceItem)
			s_iSelectPieceItem	=	lpPieceItem->m_wSerial;
	}

	{
		HWND	hPieceItemList	=	GetDlgItem(_hDlg,IDC_PIECE_ITEM_LIST);

		s_dibPieceItemList.lpszRegister	=	"PIECE_ITEM_LIST";
		s_dibPieceItemList.Init(hPieceItemList,(WNDPROC)ProcPieceItemListWnd);

		s_sbPieceItemList.Init(hPieceItemList);		//	스크롤바 초기화
		s_sbPieceItemList.InitHeight();

		DrawPieceItemList(_hDlg);
	}

//	조각 리스트
	{
		HWND	hPieceList	=	GetDlgItem(_hDlg,IDC_ITEM_PIECE_LIST);

		s_listItemPiece.Init(hPieceList	,9,FALSE,NULL);		//	3개의 컬럼

		s_listItemPiece.AddColumn("No"	,6*4);
		s_listItemPiece.AddColumn("D/L"	,6*5,eLCOM_CENTER);
		s_listItemPiece.AddColumn("D/C"	,6*10,eLCOM_CENTER);
		s_listItemPiece.AddColumn("거래",6*6,eLCOM_CENTER);
		s_listItemPiece.AddColumn("퀘스트",6*8,eLCOM_CENTER);
		s_listItemPiece.AddColumn("임몬",6*6,eLCOM_CENTER);
		s_listItemPiece.AddColumn("임보",6*6,eLCOM_CENTER);
		s_listItemPiece.AddColumn("필드",6*6,eLCOM_CENTER);
		s_listItemPiece.AddColumn("상자",6*6,eLCOM_CENTER);

		ResetItemPieceListBox(_hDlg);
	}	//	접두사 리스트

	s_dibItemInfo.lpszRegister		=	"ITEM_INFO";
	s_dibItemInfo.Init(GetDlgItem(_hDlg,IDC_ITEM_INFO),(WNDPROC)ProcItemInfoWnd);

	Draw(_hDlg);
}

void
cSetPieceItem::Draw(HWND _hDlg)
{
	DrawPieceItemList(_hDlg);
	DrawItemPieceList(_hDlg);
	DrawItemInfo(_hDlg);
}

void
cSetPieceItem::DrawPieceItemList(HWND _hDlg)		// 09.08.19
{
	int	iItemListWidth		=	s_dibPieceItemList.width();		//	아이템 리스트 너비
	int	iItemListHeight		=	s_dibPieceItemList.height();		//	아이템 리스트 높이
	int	iLineCountPerPage	=	(iItemListHeight-10) / (dICON_SHAPE_HEIGHT+8);
	int	iLineCount			=	s_deqFilterIndex.size();

	s_sbPieceItemList.SetHeight(iLineCount,iLineCountPerPage);

	int	x=4,y=5;

	s_dibPieceItemList.Active();

	y				=	(cDRAW::HEIGHT-iLineCountPerPage*(dICON_SHAPE_HEIGHT+8))/2;

	if (iLineCount	>=	s_deqFilterIndex.size())
		EnableWindow(GetDlgItem(_hDlg,IDC_ADD_PIECE_ITEM),FALSE);
	else
		EnableWindow(GetDlgItem(_hDlg,IDC_ADD_PIECE_ITEM),TRUE);

	if (iLineCount	<	1)
	{
		cDRAW::Fill(_SKYBLUE);

		cTEXT::PushShadow(FALSE);
		cTEXT::Put(5,5,0,dMSG_ITEM_NOT_EXIST);
		cTEXT::PopShadow();

		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT_PIECE_ITEM),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE_PIECE_ITEM),FALSE);

		s_dibPieceItemList.Draw();

		return;
	}

	EnableWindow(GetDlgItem(_hDlg,IDC_EDIT_PIECE_ITEM),TRUE);
	EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE_PIECE_ITEM),TRUE);

	cDRAW::Fill(_SKYBLUE);

	int		iIndex		=	s_sbPieceItemList.m_iHeightPos;
	int		iCount		=	0;


	
	for(int i=iIndex;i<s_deqFilterIndex.size();++i)
	{
		if(s_deqFilterIndex[i] == 0)
			continue;

		cPieceItem*	lpPieceItem = g_pieceItemManager.get(s_deqFilterIndex[i]);
		
		if (!lpPieceItem	||	lpPieceItem->m_wSerial	==	0xffff)
			continue;
		
		if (iCount	>=	iLineCountPerPage	)
			break;
		cITEM		item;
		
		if (!lpPieceItem->generateItem(&item))
		{
			
			g_pieceItemManager.remove(s_deqFilterIndex[i]);
			continue;
		}

		cBASIC_ITEM	*lpItem	=	&g_aBasicItem[item.m_wBaseItem];

		iCount++;


		if (s_deqFilterIndex[i]	==	s_iSelectPieceItem)
		{
			cDRAW::Fill		(0			,x  ,y+2,iItemListWidth-2,y+dICON_SHAPE_HEIGHT+6);
			cDRAW::Fill		(_BLUE		,x-1,y+1,iItemListWidth-3,y+dICON_SHAPE_HEIGHT+5);
			cDRAW::Box		(_DEEPGRAY	,x-1,y+1,iItemListWidth-3,y+dICON_SHAPE_HEIGHT+5);

			g_smiIconItem.put(lpItem->m_wIconShape,x+1,y+4);

			cTEXT::Put		(x+1+36	,y+4+2	,WHITE,_ms("[%3d] %s",s_deqFilterIndex[i],lpPieceItem->m_strName));

			if (lpPieceItem->m_wEquipLevel)
				cTEXT::Put		(x+1+36	,y+4+18	,WHITE,_ms("%s [착용제한레벨 LV %d]",g_strItemCategory[lpItem->m_wKind],lpPieceItem->m_wEquipLevel));
			else
				cTEXT::Put		(x+1+36	,y+4+18	,WHITE,g_strItemCategory[lpItem->m_wKind]);
		}
		else
		{
			cDRAW::Fill		(0			,x  ,y+2,iItemListWidth-2,y+dICON_SHAPE_HEIGHT+6);
			cDRAW::Fill		(_DEEPGRAY	,x-2,y  ,iItemListWidth-4,y+dICON_SHAPE_HEIGHT+4);
			cDRAW::Box		(0			,x-2,y  ,iItemListWidth-4,y+dICON_SHAPE_HEIGHT+4);

			g_smiIconItem.put(lpItem->m_wIconShape,x	,y+3);

			cTEXT::Put		(x+36	,y+3+2	,LTGRAY,_ms("[%3d] %s",s_deqFilterIndex[i],lpPieceItem->m_strName));
			if (lpPieceItem->m_wEquipLevel)
				cTEXT::Put		(x+36	,y+3+18	,LTGRAY,_ms("%s [착용제한레벨 LV %d]",g_strItemCategory[lpItem->m_wKind],lpPieceItem->m_wEquipLevel));
			else
				cTEXT::Put		(x+36	,y+3+18	,LTGRAY,g_strItemCategory[lpItem->m_wKind]);
		}

		y	+=	(dICON_SHAPE_HEIGHT+8);


	}

	s_dibPieceItemList.Draw();
}



void
cSetPieceItem::DrawItemPieceList(HWND _hWnd)
{
	s_listItemPiece.Draw();
}

void
cSetPieceItem::DrawItemInfo(HWND _hDlg)
{
	cPieceItem	*lpPieceItem	=	g_pieceItemManager.get(s_iSelectPieceItem);

	if (!lpPieceItem || s_iSelectPieceItem <= 0)
	{
		s_dibItemInfo.Active();
		cDRAW::Fill(0);
		s_dibItemInfo.Draw();
		return;
	}

	cITEM	item;

	if (!lpPieceItem->generateItem(&item))
		return;

	CSetCustomItemList::DrawItemInfoWnd(_hDlg,&s_dibItemInfo,NULL,NULL,lpPieceItem);
}

//
//	기본 아이템 목록 설정
void
cSetPieceItem::ResetItemPieceListBox(HWND hwnd)
{
	s_listItemPiece.ResetItems();
	
	cPieceItem	*lpPieceItem	=	g_pieceItemManager.get(s_iSelectPieceItem);

	if (!lpPieceItem || s_iSelectPieceItem <= 0)
		return;

	int		iIndex;
	char	*strOX[]	=	{"X","O"};

	for (iIndex=0;iIndex<lpPieceItem->m_wPieceCount;iIndex++)
	{
		cItemPieceDefine	*lpPiece	=	&lpPieceItem->m_aPiece[iIndex];

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
cSetPieceItem::AddPieceItem(HWND _hDlg)		//09.08.20
{
	if (!cMakePieceItem::Open(_hDlg,NULL))
		return;

	s_iSelectPieceItem	=	g_pieceItemManager.add(&cMakePieceItem::s_makePieceItem);

	ResetItemPieceListBox(_hDlg);
	FilterPieceItem(_hDlg);
}

void
cSetPieceItem::RemovePieceItem(HWND _hDlg)//09.08.20
{
	if (g_pieceItemManager.getCount() <=1 )
		return;

	if (cMSG::YESNO("조각 아이템 제거","선택한 조각 아이템을 제거 하시겠습니까?") == IDNO)
		return;

	g_pieceItemManager.remove(s_iSelectPieceItem);

	cPieceItem	*lpPieceItem	=	g_pieceItemManager.getFirst(s_iSelectPieceItem);

	if (lpPieceItem)
		s_iSelectPieceItem		=	lpPieceItem->m_wSerial;
	else
		s_iSelectPieceItem		=	g_pieceItemManager.getLastIndex();

	ResetItemPieceListBox(_hDlg);
	FilterPieceItem(_hDlg);
}

void
cSetPieceItem::EditPieceItem(HWND _hDlg)//09.08.20
{
	if (g_pieceItemManager.getCount() <=1 )
		return;

	cPieceItem	*lpPieceItem	=	g_pieceItemManager.get(s_iSelectPieceItem);

	if (!lpPieceItem)
		return;

	if (!cMakePieceItem::Open(_hDlg,lpPieceItem))
		return;

	g_pieceItemManager.replace(lpPieceItem->m_wSerial,&cMakePieceItem::s_makePieceItem);
	
	ResetItemPieceListBox(_hDlg);
	FilterPieceItem(_hDlg);
}
//
//	
int
cSetPieceItem::SelectPieceItem(int _iX,int _iY)		//09.08.17
{
	int	x=5,y=5;

	if (s_deqFilterIndex.size()	<	1)
		return	0xffff;

	int	iItemListWidth		=	s_dibPieceItemList.width();		//	아이템 리스트 너비
	int	iItemListHeight		=	s_dibPieceItemList.height();		//	아이템 리스트 높이
	int	iLineCountPerPage	=	(iItemListHeight-10) / (dICON_SHAPE_HEIGHT+8);
	int	iLineCount			=	s_deqFilterIndex.size();

	y						=	(iItemListHeight	-	iLineCountPerPage	*(dICON_SHAPE_HEIGHT+8))/2;

	int		iIndex			=	s_sbPieceItemList.m_iHeightPos;
	int		iCount			=	0;

	if (_iX	<=	0	||	_iX	>=	iItemListHeight)
		return	0xffff;

	for (int i=iIndex;i<s_deqFilterIndex.size();i++)
	{
		if (s_deqFilterIndex[i]	==	0)
			continue;

		cPieceItem	*lpPieceItem	=	g_pieceItemManager.get(s_deqFilterIndex[i]);

		if (!lpPieceItem)
			continue;

		if (iCount	>=	iLineCountPerPage	)
			break;

		iCount++;

		if (_iY	>=	y	&&	_iY	<=	y	+	dICON_SHAPE_HEIGHT+8)
			return	s_deqFilterIndex[i];

		y	+=	(dICON_SHAPE_HEIGHT+8);
	}

	return	0xffff;
}

void
cSetPieceItem::FilterPieceItem(HWND _hDlg)		//09.08.17
{

	char strFilter[256] = "";
	GetDlgItemText(_hDlg,IDC_EDIT_PIECEITEM_FILTER,strFilter,256);
	int iFilterNameLen	= strlen(strFilter);
	s_deqFilterIndex.clear();

	if(iFilterNameLen<=0)
	{
		InitDlg(_hDlg);
		return;
	}

	int	iCount	=	0;

	for (int i=0;i<g_pieceItemManager.getMaxCount() && iCount<g_pieceItemManager.getCount();++i)
	{

		cPieceItem	*lpPieceItem	=	g_pieceItemManager.get(i);
		if(!lpPieceItem)
			continue;
		++iCount;
		int iItemNameLen	= strlen(lpPieceItem->m_strName);
		int iLenGap			= iItemNameLen - iFilterNameLen;
		if(iLenGap<0)
			continue;
		
		for(int iName=0;iName<=iLenGap;++iName)
		{
			if(strnicmp(strFilter,&lpPieceItem->m_strName[iName], iFilterNameLen) == 0 )
			{
				s_deqFilterIndex.push_back(i);
				break;		

			}
		}
		
	}
		Draw(s_hWnd);

}

LRESULT CALLBACK
cSetPieceItem::ProcItemInfoWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

		default				:	return( DefWindowProc( hDlg, uMsg, wParam, lParam ) );
	}

	return (0L);
}

LRESULT CALLBACK
cSetPieceItem::ProcPieceItemListWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)		// 09.08.11
{
	switch(uMsg)
	{
		case WM_PAINT			:
		{
			PAINTSTRUCT ps;

			BeginPaint( hDlg, &ps );

			s_dibPieceItemList.Active();
			s_dibPieceItemList.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hDlg, &ps);
			break;
		}

		case WM_LBUTTONDOWN		:
		{
			int item		=	SelectPieceItem(LOWORD(lParam),HIWORD(lParam));

			if(item			==	0xffff		)
				break;
			if(item			==	s_iSelectPieceItem)
				break;

			s_iSelectPieceItem	=	item;

			ResetItemPieceListBox(s_hWnd);

			Draw(s_hWnd);
			SetFocus(hDlg);
			break;
		}
		case WM_MOUSEWHEEL :
			{
				
				int zDelta = ((short)HIWORD(wParam));
				
				
				if	(zDelta>=0)
				{
					s_sbPieceItemList.VScroll(SB_LINEUP);
				}
				else{
					s_sbPieceItemList.VScroll(SB_LINEDOWN);
				}
				
			Draw(s_hWnd);
				break;
			}
		case WM_VSCROLL			:
			if (s_sbPieceItemList.VScroll(wParam))
				DrawPieceItemList(s_hWnd);
			break;

		default					:
			return( DefWindowProc( hDlg, uMsg, wParam, lParam ) );
	}

	return (0L);
}
//
//	다얄로그 박스 프로시져
LRESULT CALLBACK
cSetPieceItem::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
					EndDialog(hDlg,IDOK);
					break;
				}

				case	IDCANCEL				:
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_LOADING	:
				{
					LoadingTextScript(hDlg);
					break;
				}

				case	IDC_ADD_PIECE_ITEM		:
				{
					AddPieceItem(hDlg);
					break;
				}

				case	IDC_REMOVE_PIECE_ITEM	:
				{
					RemovePieceItem(hDlg);
					break;
				}

				case	IDC_EDIT_PIECE_ITEM	:
				{
					EditPieceItem(hDlg);
					break;
				}
				case	IDC_EDIT_PIECEITEM_FILTER:		//09.08.17
					{
						if	(HIWORD(wParam) != EN_CHANGE)
							break;
						FilterPieceItem(hDlg);
						break;
						
					}
			}
			break;
	}

	return (0L);
}


BOOL
cSetPieceItem::DecodeTextScriptFile(char *_lpstrFn)
{
	cDECODER	decoder;

	if (!decoder.Upload(_lpstrFn,NULL))
		return	FALSE;

	int	iOverlappedCount	=	0;

	g_pieceItemManager.reset();
	g_pieceItemManager.build();

	while(1)
	{
		char	*strOneLine	=	decoder.GetStream('@');

		if (!strOneLine)
			break;

		int	iPoint	=	0;

		if (strOneLine[iPoint]	==	13)
			iPoint++;

		if (strOneLine[iPoint]	==	10)
			iPoint++;

		if (DecodeTextScript(strOneLine+iPoint))
		{
			int	iSerial	=	g_pieceItemManager.add(&s_makePieceItem);

			if (iSerial	==	0xffff)
			{
				ERRMSG("조각 아이템 이름이 이름이 중복 됩니다.","%s",s_makePieceItem.m_strName);
				iOverlappedCount++;
			}
		}
	}

	cMSG::Put("데이터 화일 분석 결과","%d개의 조각 아이템을 추가 했습니다.\n\n%d개의 조각 아이템 이름이 중복 되었습니다.",g_pieceItemManager.getCount()-1,iOverlappedCount);

	return	TRUE;
}

BOOL
cSetPieceItem::DecodeTextScript(char *_lpstrText)
{
	cDECODER	decoder;

	int	iLen	=	strlen(_lpstrText);

	if (iLen	<=	5)
		return	FALSE;

	for (;iLen;iLen--)
	{
		if (_lpstrText[iLen-1]	==	13)
			_lpstrText[iLen-1]	=	'	';
	}

	decoder.UploadBuffer(_lpstrText,NULL);

	char			*lpStream;
	int				iValidCount				=	0;
	int				iGrade					=	0;
	int				iPieceCounter			=	0;
	int				iReversionPieceCount	=	0;
	int				iReversionPieceCounter	=	0;

	while(1)
	{	
		lpStream	=	decoder.GetStream('	',10);

		iGrade++;

		if (iGrade	>=	150)
			break;

		if	(!lpStream)
		{
			if (iGrade	<=	1)
				return	FALSE;

			continue;
		}

		if	(lpStream[0]	==	10)
			lpStream	=	lpStream+1;

		iValidCount		=	0;

		int	iIndex		=	iGrade;
		int	iValue		=	0;

		if	(iIndex	>=	6	&&	iIndex	<=	11)
		{
			int				iCase		=	(iIndex-6)%2;
			int				iPrefixIndex=	(iIndex-6)/2;
			CItemPrefixInfo	*lpPrefixInfo=	&s_makePieceItem.m_item.m_aPrefix[iPrefixIndex];

			switch(iCase)
			{
				case	0	:
				{
					cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.getPrefix(lpStream);

					if (!lpPrefix)
						return	ERRMSG("에러!!","'%s' 라는 접두사는 없습니다.",lpStream);

					lpPrefixInfo->m_wPrefix		=	lpPrefix->m_wSerial;
					break;
				}

				case	1	:
				{
					cDECODER	dec;

					dec.UploadBuffer(lpStream,NULL);

					lpPrefixInfo->m_aValue[0]	=	dec.GetNumber();
					lpPrefixInfo->m_aValue[1]	=	dec.GetNumber(FALSE);

					break;						
				}
			}
		}
		else
		if (iIndex	>=	15	&&	iIndex	<	15+25*4)
		{
			int					iPieceIndex	=	(iIndex-15)/4;
			int					iCase		=	(iIndex-15)%4;

			cItemPieceDefine	*lpPiece	=	&s_makePieceItem.m_aPiece[iPieceIndex];

			switch(iCase)
			{
				case	0	:
					if (lpStream[0]	==	'"')
						lpPiece->m_wDropLevel	=	atoi(lpStream+1);
					else
						lpPiece->m_wDropLevel	=	atoi(lpStream);
					break;
				case	1	:
					lpPiece->m_wDropChance		=	StringToNumber(lpStream,100,2);
					break;
				case	2	:
					if (stricmp(lpStream,"x")==0)
					{
						lpPiece->m_bf1IsReversion	=	TRUE;
						iReversionPieceCounter++;
					}
					else
						lpPiece->m_bf1IsReversion	=	FALSE;
					break;
				case	3	:
					if (lpStream[0]	==	'1')
						lpPiece->m_bf1IsDropInQuest					=	TRUE;
					if (lpStream[1]	==	'1')
						lpPiece->m_bf1IsDropInTempDungeonMonster	=	TRUE;
					if (lpStream[2]	==	'1')
						lpPiece->m_bf1IsDropInTempDungeonArca		=	TRUE;
					if (lpStream[3]	==	'1')
						lpPiece->m_bf1IsDropInNormalFieldMonster	=	TRUE;
					if (lpStream[4]	==	'1')
						lpPiece->m_bf1IsDropInNormalFieldArca		=	TRUE;
					iPieceCounter++;
					break;
			}
		}
		else
		switch(iIndex)
		{
			case	1	:
			{
				iValue	=	atoi(lpStream);

				s_makePieceItem.reset();
				s_makePieceItem.m_wSerial	=	iValue;
				break;
			}

			case	2	:
			{
				strcpy(s_makePieceItem.m_strName,lpStream);
				break;
			}

			case	3	:
			{
				if (strstr(lpStream,":"))
				{
					cDECODER	dec;

					dec.UploadBuffer(lpStream,NULL);

					char	*lpstrName	=	dec.GetStream(':');
					
					cItemSet	*lpItemSet			=	g_itemSetManager.getByName(lpstrName);

					if	(!lpItemSet)
						return	ERRMSG("에러!!","'%s' 라는 아이템 세트는 없습니다.",lpStream);

					s_makePieceItem.m_bf1IsSetItem		=	TRUE;
					s_makePieceItem.m_bf11SetGroupIndex	=	lpItemSet->m_wSerial;
					s_makePieceItem.m_bf4SetItemIndex	=	dec.GetNumber();

					if (s_makePieceItem.m_bf4SetItemIndex	>=	lpItemSet->getItemCount())
						return	ERRMSG("에러!!","'%s' 세트는 %d개의 아이템이 등록되어 있습니다.",lpItemSet->getItemCount());
				}
				else
				{
					cBASIC_ITEM	*lpItem	=	cBASIC_ITEM::Get(lpStream);

					if (!lpItem)
						return	ERRMSG("에러!!","'%s' 라는 아이템은 없습니다.",lpStream);

					CCustomItem		customItem;

					customItem.m_wBaseItem			=	lpItem->m_iSerial;
					customItem.m_bCount				=	1;
					customItem.m_wGenerateQuality	=	100;

					s_makePieceItem.m_bf1IsSetItem	=	FALSE;

					CSetCustomItem::GenerateItemByCustomItem((cITEM *)&s_makePieceItem.m_item,&customItem);
				}
				break;
			}

			case	5	:
			{
				s_makePieceItem.m_wEquipLevel	=	atoi(lpStream);
				break;
			}

			case	13	:
			{
				s_makePieceItem.m_wPieceCount	=	atoi(lpStream);

				break;
			}

			case	14	:
			{
				iReversionPieceCount			=	atoi(lpStream);

				break;
			}
		}
	}

	if (iReversionPieceCount	!=	iReversionPieceCounter)
		return	ERRMSG("조각 아이템 제작","에러!! 설정된 거래 불가 조각 개수와 실제 거래 불가 개수에 차이가 있습니다.\n\n%s) %d:%d",s_makePieceItem.m_strName,iReversionPieceCount,iReversionPieceCounter);

	if (iPieceCounter			!=	s_makePieceItem.m_wPieceCount)
		return	ERRMSG("조각 아이템 제작","에러!! 설정된 조각 개수와 실제 개수에 차이가 있습니다.\n\n%s) %d:%d",s_makePieceItem.m_strName,iPieceCounter,s_makePieceItem.m_wPieceCount);

	return	TRUE;
}

void
cSetPieceItem::LoadingTextScript(HWND _hDlg)
{
	char	strFn[512]	=	"";
	char	strFolder[512] = "";

	if (!cFILE::LoadBox(strFn,"텍스트 화일 {*.txt}\0*.txt\0\0","txt",NULL,"조각 아이템 텍스트 스크립트 데이터 파일 불러오기"))
	{
		SetLogFolder();
		return;
	}

	DecodeTextScriptFile(strFn);

	SetLogFolder();

	Draw(_hDlg);
}