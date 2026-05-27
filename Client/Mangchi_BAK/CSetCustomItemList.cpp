#include "CSetCustomItem.h"
#include "resource.h"
#include "cDRAW.h"
#include "cTEXT.h"
#include "cITEM_PREFIX_EFFECT.H"
#define WM_MOUSEWHEEL       0x020A	// 09.08.11 
CCustomItem				CSetCustomItemList::s_itemEdit;	//	편집중인 상점
HWND					CSetCustomItemList::s_hWnd;
cDIBWND					CSetCustomItemList::s_dibItemList;
cDIBWND					CSetCustomItemList::s_dibItemInfo;
int						CSetCustomItemList::s_iItemListWndPos;
int						CSetCustomItemList::s_iSelectItem;
cSCROLL_BAR				CSetCustomItemList::s_scrollItemInfo;

int						CSetCustomItemList::s_iItemListWidth;
int						CSetCustomItemList::s_iItemListHeight;
int						CSetCustomItemList::s_iLineCountPerPage;
int						CSetCustomItemList::s_iLineCount;
cSCROLL_BAR				CSetCustomItemList::s_sbItemList;
cRECT					CSetCustomItemList::s_rectItemList;
CCustomItem*			CSetCustomItemList::s_lpSelectItem		=	NULL;
CCustomItemManager*		CSetCustomItemList::s_lpItemManager		=	NULL;
std::deque<int>			CSetCustomItemList::s_deqFilterIndex;		// 09.08.19
//
//	시작
void
CSetCustomItemList::Open(HWND _hWnd,CCustomItemManager* _lpCustomItemManager)
{
	s_lpItemManager	=	_lpCustomItemManager;

	DialogBox( s_hInst, "SET_CUSTOM_ITEM_LIST",_hWnd,(DLGPROC)ProcDlg);
}

//
//	박스 초기화
void
CSetCustomItemList::InitBox(HWND _hDlg)//09.08.19
{
	s_hWnd	=	_hDlg;
	s_deqFilterIndex.clear();
	for(int i=0;i<s_lpItemManager->getSize();++i)
	{
		CCustomItem	*lpGetItem	=	s_lpItemManager->getItem(i);
		
		if(!lpGetItem)
			continue;
		s_deqFilterIndex.push_back(i);		// 09.08.19

	}
//	컨텐츠 윈도우 초기화
	{
		HWND	hItemList	=	GetDlgItem(_hDlg,IDC_ITEM_LIST);

		s_dibItemList.lpszRegister	=	"CUSTOM_ITEM_LIST_WINDOW";
		s_dibItemList.Init(hItemList,(WNDPROC)ProcItemListWnd);

		s_sbItemList.Init(hItemList);		//	스크롤바 초기화
		s_sbItemList.InitHeight();

		s_iItemListWidth	=	s_dibItemList.width();		//	아이템 리스트 너비
		s_iItemListHeight	=	s_dibItemList.height();		//	아이템 리스트 높이
		s_iLineCountPerPage	=	(s_iItemListHeight-10)	/	(dICON_SHAPE_HEIGHT+8);
		s_iLineCount		=	s_lpItemManager->getCount();

		s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

		DrawItemListWnd(_hDlg);
	}

//	반응 윈도우 초기화
	{
		HWND	hItemInfo	=	GetDlgItem(_hDlg,IDC_ITEM_INFO);

		s_dibItemInfo.lpszRegister	=	"CUSTOM_ITEM_INFO_WINDOW";
		s_dibItemInfo.Init(hItemInfo,(WNDPROC)ProcItemInfoWnd);

		DrawItemInfoWnd(_hDlg);
	}
}

BOOL
CSetCustomItemList::DecodeTextScript(char *_strScript)
{
	cDECODER	decoder;

	if (strlen(_strScript)	<=	5)
		return	FALSE;

	s_itemEdit.reset();

	decoder.UploadBuffer(_strScript,NULL);

	char			*lpStream;
	int				iValidCount		=	0;
	int				iGrade			=	0;

	while(1)
	{	
		lpStream	=	decoder.GetStream('	');

		iGrade++;

		if (!lpStream)
		{
			iValidCount++;
		
			if (iValidCount	>=	20)
				break;

			continue;
		}

		int	iIndex			=	iGrade;
		int	iPrefixIndex	=	0;

		switch(iIndex)
		{
			case	4	:
				iPrefixIndex	=	0;
				break;
			case	5	:
				iPrefixIndex	=	1;
				break;
			case	6	:
				iPrefixIndex	=	2;
				break;
		}


		switch(iIndex)
		{
			case	1	:
			{
				int	iIndex	=	atoi(lpStream);

				s_itemEdit.m_dwSerial	=	iIndex;
				break;
			}
			case	2	:
			{
				int	iIndex	=	atoi(lpStream);

				strcpy(s_itemEdit.m_strName,lpStream);
				break;
			}

			case	3	:
			{
				cBASIC_ITEM	*lpBasicItem	=	cBASIC_ITEM::Get(lpStream);

				if (!lpBasicItem)
					return	ERRMSG("에러!!","'%s' 라는 아이템을 찾을 수 없습니다.",lpStream);

				s_itemEdit.m_wBaseItem	=	lpBasicItem->m_iSerial;
					
				break;
			}

			case	4	:
			case	5	:
			case	6	:
			{
				cDECODER	dec;

				dec.UploadBuffer(lpStream,NULL);

				char			*lpPrefix1	=	dec.GetStream('|');
				cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.getPrefix(lpPrefix1);

				if (!lpPrefix)
					return	ERRMSG("에러!!","이상한 접두사 구분 코드!!!!! [%s]",lpPrefix1);

				s_itemEdit.m_aPrefix[iPrefixIndex].m_wPrefix		=	lpPrefix->m_wSerial;
				s_itemEdit.m_aPrefixGenerateQuality[iPrefixIndex]	=	dec.GetNumber();
				break;
			}
		}
	}

	return	TRUE;
}

BOOL
CSetCustomItemList::DecodeTextScriptFile(char *_strFile)
{
	cDECODER	decoder;

	if (!decoder.Upload(_strFile,NULL))
		return	FALSE;

	int	iOverlappedCount	=	0;

	g_customItem.reset();

	while(1)
	{
		char	*strOneLine	=	decoder.GetStream(13);

		if (!strOneLine)
			break;

		int	iPoint	=	0;

		if (strOneLine[0]	==	10)
			iPoint++;

		if (DecodeTextScript(strOneLine+iPoint))
		{
			int	iIndex				=	s_itemEdit.m_dwSerial;
			s_itemEdit.m_dwSerial	=	1;
			
			if (!g_customItem.insert(iIndex,&s_itemEdit))
			{
				ERRMSG("커스텀 아이템 이름이 중복 됩니다.","%s",s_itemEdit.m_strName);
				iOverlappedCount++;
			}
		}
	}

	cMSG::Put("데이터 화일 분석 결과","%d개의 커스텀 아이템을 추가 했습니다.\n\n%d개의 아이템 이름이 중복 되었습니다.",g_customItem.getCount(),iOverlappedCount);

	return	TRUE;
}


void
CSetCustomItemList::Loading(HWND _hDlg)
{
	if (s_lpItemManager == &g_premiumItem)
		return;

	char	strFn[512]	=	"";
	char	strFolder[512] = "";

	if (!cFILE::LoadBox(strFn,"텍스트 화일 {*.txt}\0*.txt\0\0","txt",NULL,"커스텀 아이템 스크립트 데이터 파일 불러오기"))
	{
		SetLogFolder();
		return;
	}

	DecodeTextScriptFile(strFn);

	s_iLineCount	=	s_lpItemManager->getCount();

	DrawItemListWnd(_hDlg);
	DrawItemInfoWnd(_hDlg);
	SetLogFolder();
}

//
//최초 분기 추가
void
CSetCustomItemList::AddItem(HWND _hDlg)		// 09.08.19
{
	CCustomItem	*lpItem	=	CSetCustomItem::Open(_hDlg);

	if (!lpItem)
		return;

	int	iSerial			=	s_lpItemManager->add(lpItem);

	if (iSerial	==	0xffff)	return;

	s_iLineCount		=	s_lpItemManager->getCount();
	s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

	s_iSelectItem		=	iSerial;
	
	if (s_lpItemManager == &g_lpActmap->m_customItem)
		g_lpActmap->LostVirgin();

	FilterCustomItem(_hDlg);
}

void
CSetCustomItemList::ExportItemList(HWND _hDlg)
{
	char	strFileName[512];

	sprintf(strFileName,"%s/premium item list.txt",_logFolder);
	cFILE	file;

	if	(!file.Open(strFileName,"wb"))
		return;

	for (int i=0;i<s_lpItemManager->getSize();i++)
	{
		CCustomItem	*lpCustomItem	=	s_lpItemManager->getItem(i);

		if	(!lpCustomItem)
			continue;

		cBASIC_ITEM	*lpBasicItem	=	&g_aBasicItem[lpCustomItem->m_wBaseItem];

		file.writeStreamCR	("#%.3d	%s	%s",i,lpCustomItem->m_strName,lpBasicItem->m_pItemComment);
	}

	file.Close();

	cMSG::Put("추출 완료","%s",strFileName);

	ShellExecute(NULL,"open",strFileName,NULL,NULL,SW_SHOWNORMAL);
}

//
//최초 분기 추가
void
CSetCustomItemList::InsertItem(HWND _hDlg)		// 09.08.19
{
	CCustomItem	*lpItem	=	CSetCustomItem::Open(_hDlg);

	if	(!lpItem)
		return;

	int	iSerial			=	s_lpItemManager->insert(s_iSelectItem,lpItem);

	if	(iSerial	==	0xffff)
		return;

	s_iLineCount		=	s_lpItemManager->getCount();
	s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

	s_iSelectItem		=	iSerial;


	if (s_lpItemManager == &g_lpActmap->m_customItem)
		g_lpActmap->LostVirgin();
	FilterCustomItem(_hDlg);
}


//
//선택된 최초 분기 편집
void
CSetCustomItemList::EditItem(HWND _hDlg)												//09.08.19
{
	CCustomItem	*lpGetItem	=	s_lpItemManager->getItem(s_iSelectItem);

	if (!lpGetItem)	return;

	CCustomItem	*lpItem		=	CSetCustomItem::Open(_hDlg,lpGetItem);

	if (!lpItem)	return;

	memcpy(lpGetItem,lpItem,sizeof(CCustomItem));

	s_iLineCount		=	s_lpItemManager->getCount();
	s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

	if (s_lpItemManager == &g_lpActmap->m_customItem)
		g_lpActmap->LostVirgin();
	FilterCustomItem(_hDlg);
}

//
//선택된 최초 분기 제거
void
CSetCustomItemList::RemoveItem(HWND _hDlg)		// 09.08.19
{
	s_lpItemManager->remove(s_iSelectItem);
	s_iLineCount		=	s_lpItemManager->getCount();
	s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

	int	iCheckItemSlot	=	s_iSelectItem;

	s_iSelectItem		=	0xffff;

	for (int i=iCheckItemSlot;i<s_lpItemManager->getSize();i++)
	{
		CCustomItem	*lpCustomItem	=	s_lpItemManager->getItem(i);

		if (!lpCustomItem)	continue;

		s_iSelectItem	=	i;

		break;
	}

	if (s_iSelectItem	==	0xffff)
		for (int i=0;i<s_lpItemManager->getSize();i++)
		{
			CCustomItem	*lpCustomItem	=	s_lpItemManager->getItem(i);

			if (!lpCustomItem)	continue;

			s_iSelectItem	=	i;

			break;
		}

	if (s_lpItemManager == &g_lpActmap->m_customItem)
		g_lpActmap->LostVirgin();
	FilterCustomItem(_hDlg);
}

//
//	아이템 리스트 윈도우 그리기
void
CSetCustomItemList::DrawItemListWnd(HWND _hDlg)		//09.08.19
{

	int	x=4,y=5;

	s_dibItemList.Active();

	y					=	(cDRAW::HEIGHT-s_iLineCountPerPage*(dICON_SHAPE_HEIGHT+8))/2;

	s_rectItemList.Set(0,0,cDRAW::WIDTH,cDRAW::HEIGHT);

	if (s_iLineCount	>=	s_lpItemManager->getSize())
		EnableWindow(GetDlgItem(_hDlg,IDC_ADD),FALSE);
	else
		EnableWindow(GetDlgItem(_hDlg,IDC_ADD),TRUE);

	if (s_iLineCount	<=	0)
	{
		cDRAW::Fill(_SKYBLUE);

		cTEXT::PushShadow(FALSE);
		cTEXT::Put(5,5,0,dMSG_ITEM_NOT_EXIST);
		cTEXT::PopShadow();

		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE),FALSE);

		s_dibItemList.Draw();

		return;
	}

	EnableWindow(GetDlgItem(_hDlg,IDC_EDIT),TRUE);
	EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE),TRUE);

	cDRAW::Fill(_SKYBLUE);

	int		iIndex		=	0;
	int		iCount		=	0;

	for (int i=0;i<s_deqFilterIndex.size();++i)		//09.08.19
	{
		CCustomItem	*lpCustomItem	=	s_lpItemManager->getItem(s_deqFilterIndex[i]);
		if	(!lpCustomItem)
			continue;

		if	(iCount	>=	s_iLineCountPerPage	)
			break;

		iIndex++;

		if	(iIndex	<=	s_sbItemList.m_iHeightPos)
			continue;

		iCount++;
		
		cBASIC_ITEM	*lpItem	=	&g_aBasicItem[lpCustomItem->m_wBaseItem];

		if (s_deqFilterIndex[i] ==	s_iSelectItem)
		{
			cDRAW::Fill		(0			,x  ,y+2,s_iItemListWidth-2,y+dICON_SHAPE_HEIGHT+6);
			cDRAW::Fill		(_BLUE		,x-1,y+1,s_iItemListWidth-3,y+dICON_SHAPE_HEIGHT+5);
			cDRAW::Box		(_DEEPGRAY	,x-1,y+1,s_iItemListWidth-3,y+dICON_SHAPE_HEIGHT+5);

			g_smiIconItem.put(lpItem->m_wIconShape,x+1,y+4);

			cTEXT::Put		(x+1+36	,y+4+2	,WHITE,_ms("[%3d] %s",s_deqFilterIndex[i],lpCustomItem->m_strName));
			cTEXT::Put		(x+1+36	,y+4+18	,WHITE,g_strItemCategory[lpItem->m_wKind]);
		}
		else
		{
			cDRAW::Fill		(0			,x  ,y+2,s_iItemListWidth-2,y+dICON_SHAPE_HEIGHT+6);
			cDRAW::Fill		(_DEEPGRAY	,x-2,y  ,s_iItemListWidth-4,y+dICON_SHAPE_HEIGHT+4);
			cDRAW::Box		(0			,x-2,y  ,s_iItemListWidth-4,y+dICON_SHAPE_HEIGHT+4);

			g_smiIconItem.put(lpItem->m_wIconShape,x	,y+3);

			cTEXT::Put		(x+36	,y+3+2	,LTGRAY,_ms("[%3d] %s",s_deqFilterIndex[i],lpCustomItem->m_strName));
			cTEXT::Put		(x+36	,y+3+18	,LTGRAY,g_strItemCategory[lpItem->m_wKind]);
		}

		y	+=	(dICON_SHAPE_HEIGHT+8);
	}

	s_dibItemList.Draw();
}

//
//	아이템 정보 윈도우 그리기
void
CSetCustomItemList::DrawItemInfoWnd(HWND _hDlg,cDIBWND *_lpDibWnd,CCustomItem *_lpCustomItem,cSetItem *_lpSetItem,cPieceItem *_lpPieceItem)		//09.08.19
{
	if (_lpDibWnd	==	NULL)
		_lpDibWnd	=	&s_dibItemInfo;

	int	x=5,y=5;

	//	아이템 설명을 그려준다.
	_lpDibWnd->Active();

	CCustomItem	*lpCustomItem	=	NULL;

	if (_lpCustomItem	)	
		lpCustomItem	=	_lpCustomItem;
	else{
		if(s_lpItemManager)
			lpCustomItem	=	s_lpItemManager->getItem(s_iSelectItem);	//09.08.17
	}
	

	cITEM		item;

	if (_lpPieceItem)
		_lpPieceItem->generateItem(&item);
	else
	if	(_lpSetItem)
		CSetCustomItem::GenerateItemBySetItem(&item,_lpSetItem);
	else
	if	(lpCustomItem)
		CSetCustomItem::GenerateItemByCustomItem(&item,lpCustomItem);

	cITEM		*lpItem			=	(cITEM *)&item;

	if (lpItem->m_wBaseItem == 0xffff)
	{
		s_scrollItemInfo.SetHeight(1,2);

		cDRAW::Fill(0);
		_lpDibWnd->Draw();

		return;
	}

	cBASIC_ITEM	*lpBasicItem	=	&g_aBasicItem[lpItem->m_wBaseItem];

	cDRAW::Fill(0);

	cTEXT::SetBoldFont();
	cTEXT::Put(x,y,LTYELLOW,"━━ 아이템 이름");
	cTEXT::SetNormalFont();
	y+=16;

	char	*lpstrName	=	lpItem->getName();

			if (lpItem->isSuperUniqueItem())cTEXT::FitPut(x+20,y,400,100,GOLDYELLOW		,lpstrName);
	else	if (lpItem->isUniqueItem())		cTEXT::FitPut(x+20,y,400,100,GOLDYELLOW		,lpstrName);
	else	if (lpItem->isExtraItem())		cTEXT::FitPut(x+20,y,400,100,LTCYAN			,lpstrName);
	else	if (lpItem->isRareItem())		cTEXT::FitPut(x+20,y,400,100,BTBLUE			,lpstrName);
	else									cTEXT::FitPut(x+20,y,400,100,WHITE			,lpstrName);

	y+=24;

	cTEXT::SetBoldFont();
	cTEXT::Put(x,y,LTYELLOW,"━━ 아이템 이미지");
	cTEXT::SetNormalFont();
//	착용 이미지

	if (lpBasicItem->m_wEquippedShape	!=	0xffff)
		cTEXT::Put(x+20,y+=16,WHITE,_ms("착용 이미지 : %s",g_strViewEquipmentName[lpBasicItem->m_wKind][lpBasicItem->m_wEquippedShape]));

//	드랍 이미지
	cTEXT::Put(x+20,y+=16,WHITE,"필드 이미지 : ");
	cDRAW::Fill(_BLUE,x+20+14*6,y,x+20+14*6+80,y+50);
	g_sprFieldShape.FitPut(x+20+14*6,y,80,50,lpBasicItem->m_wFieldShape);


	y+=60;

	cTEXT::SetBoldFont();
	cTEXT::Put(x,y,LTYELLOW,"━━ 아이템 정보");
	cTEXT::SetNormalFont();

	x+=20;

	if (lpBasicItem->isDamageItem())
		cTEXT::Put(x,y+=16,WHITE,_ms("최소 데미지 : %-4d    최고 데미지 : %d",lpBasicItem->m_wMinDamage,lpBasicItem->m_wMaxDamage));

	if(lpBasicItem->m_wSpeed)
		cTEXT::Put(x,y+=16,WHITE,_ms("공 격 속 도 : %d.%.2d",lpBasicItem->m_wSpeed/100,lpBasicItem->m_wSpeed%100));

	if(lpBasicItem->m_wRange)
		cTEXT::Put(x,y+=16,WHITE,_ms("사 정 거 리 : %d Pixel",lpBasicItem->m_wRange));

	if(lpBasicItem->m_wDamageRange)
		cTEXT::Put(x,y+=16,WHITE,_ms("데미지 범위 : %d Pixel",lpBasicItem->m_wDamageRange));

	if (_lpSetItem)
	{
		cSTRING		enable;

		if (_lpSetItem->m_bf1IsDropInTempDungeonMonster	)
			enable.Add("[임던 몬스터]");
		if (_lpSetItem->m_bf1IsDropInTempDungeonArca	)
			enable.Add("[임던 상자]");
		if (_lpSetItem->m_bf1IsDropInNormalFieldMonster	)
			enable.Add("[필드 몬스터]");
		if (_lpSetItem->m_bf1IsDropInNormalFieldArca	)
			enable.Add("[필드 상자]");
		if (_lpSetItem->m_bf1IsDropInQuest				)
			enable.Add("[퀘스트 보상]");

		if (enable.String  && strlen(enable.String) > 2)
			cTEXT::Put(x,y+=16,LTGREEN,_ms("%s",enable.String));
	}
	cTEXT::Put(x,y+=16,LTCYAN,    _ms("드 랍 레 벨 : %4d    스 택 제 한 : %d개     내구력 : %d",lpBasicItem->m_wDropLevel,lpBasicItem->m_wStackLimit,lpBasicItem->m_wDurability));

	int	iIndex	=	0;
	int	iWidth	=	_lpDibWnd->width()- 14;

	y+=16;

	if (lpBasicItem->m_aGenerateData[0].m_wEffect	!=	0xffff)
	{
		while(1)
		{
			if (iIndex	>=	4)
				break;

			if (lpBasicItem->m_aGenerateData[iIndex].m_wEffect	==	0xffff)
				break;

			char	*lpComment	=	lpBasicItem->getDataComment(iIndex,TRUE,lpItem->m_aOption[0],lpItem->m_aOption[1]);
			int		iLineCount	=	cTEXT::GetLines(iWidth-x,_ms("%d. %s",iIndex+1,lpComment));

			cTEXT::FitPut(x,y,iWidth-x,iLineCount*8*2,WHITE,_ms("%d. %s",iIndex+1,lpComment));

			y	+=	iLineCount*14;
			iIndex++;
		}
	}

	if(lpCustomItem)
	{
		y+=10;

		x-=20;
		cTEXT::SetBoldFont();
		cTEXT::Put(x,y,LTYELLOW,"━━ 프리미엄 아이템 정보");
		cTEXT::SetNormalFont();
		x+=20;

		cTEXT::Put(x,y+=16,LTGREEN,_ms("아이템 갯수 : %d개",lpItem->m_bCount));

		if (lpCustomItem->m_bf20TermOfValidate	==	0)
			cTEXT::Put(x,y+=16,LTGREEN,"사용   기간 : 제한 없음");
		else
		{
			WORD	wDayHour=	24*60;
			int		iDay	=	lpCustomItem->m_bf20TermOfValidate/wDayHour;
			int		iHour	=	(lpCustomItem->m_bf20TermOfValidate%wDayHour)/60;
			int		iMinute	=	lpCustomItem->m_bf20TermOfValidate%60;

			cTEXT::Put(x,y+=16,LTGREEN,_ms("사용   기간 : %d일 %d시간 %d분",iDay,iHour,iMinute));
		}
	}

	x-=20;
	y+=24;

	if (lpBasicItem->m_aUniqueData[0].m_wEffect	!=	0xffff)
	{
		cTEXT::SetBoldFont();
		cTEXT::Put(x,y,LTYELLOW,"━━ 유니크 정보");
		cTEXT::SetNormalFont();

		iIndex	=	0;
		y+=16;
		x+=20;

		while(1)
		{
			if (iIndex	>=	6)	break;
			
			if (lpBasicItem->m_aUniqueData[iIndex].m_wEffect	==	0xffff)	break;

			char	*lpComment	=	lpBasicItem->m_aUniqueData[iIndex].getComment(TRUE);

			int		iLineCount	=	cTEXT::GetLines(iWidth-x,_ms("%d. %s",iIndex+1,lpComment));

			cTEXT::FitPut(x,y,iWidth-x,iLineCount*8*2,WHITE,_ms("%d. %s",iIndex+1,lpComment));

			y	+=	iLineCount*14;
			iIndex++;
		}
		x-=20;
		y+=10;
	}

	if (lpItem->m_aPrefix[0].m_wPrefix	!=	0xffff)
	{
		cTEXT::SetBoldFont();
		cTEXT::Put(x,y,LTYELLOW,"━━ 접두사 정보");
		cTEXT::SetNormalFont();

		iIndex	=	0;
		y+=16;
		x+=20;

		while(1)
		{
			if (iIndex	>=	dITEM_PREFIX_COUNT)
				break;

			int	iPrefix	=	lpItem->m_aPrefix[iIndex].m_wPrefix;

			if (iPrefix	==	0xffff)	break;

			cITEM_PREFIX		*lpPrefix	=	g_itemPrefix.get(iPrefix);
			cITEM_PREFIX_INFO	prefixInfo;

			prefixInfo.m_wPrefix	=	iPrefix;

			for (int i=0;i<2;i++)
			{
				int	iMaxValue			=	lpPrefix->m_aValue[i][1];
				int	iMinValue			=	lpPrefix->m_aValue[i][0];

				int	iRange				=	iMaxValue-iMinValue+1;
				prefixInfo.m_aValue[i]	=	lpItem->m_aPrefix[iIndex].m_aValue[i];
			}

			char	*lpComment	=	g_itemPrefix.getComment(&prefixInfo);
			int		iLineCount	=	cTEXT::GetLines(iWidth-x,_ms("%d. %s",iIndex+1,lpComment));

			cTEXT::FitPut(x,y,iWidth-x,iLineCount*8*2,WHITE,_ms("%d. %s",iIndex+1,lpComment));

			y	+=	iLineCount*14;
			iIndex++;
		}
		x-=20;
		y+=10;
	}

	cTEXT::SetBoldFont();
	cTEXT::Put(x,y,LTYELLOW,"━━ 아이템 착용/사용 가능 직업");
	cTEXT::SetNormalFont();
	x+=20;

	if	(_lpSetItem && _lpSetItem->m_uiEnableJobMask)
	{
		cSTRING		job;

		for (int i=0;i<dPLAYER_JOB_COUNT;i++)
		{
			if	(_lpSetItem->isEnableJob(i))	
				job.Add("%s,",g_aDefaultJobName[i]);
		}

		int	iLength	=	strlen(job.String);

		if (job.String[iLength-1]	==	',')
			job.String[iLength-1]	=	NULL;

		int	iLineCount	=	cTEXT::GetLines(iWidth-x,job.String);

		cTEXT::FitPut(x,y+=16,iWidth-x,iLineCount*8*2,LTCYAN,job.String);

		y	+=	iLineCount*8*2;
		y	+=	10;
	}
	else
	if (lpBasicItem->isEnableAllJob())
	{
		cTEXT::Put(x,y+=16,WHITE,"직업 제한 없음");
		y	+=	24;
	}
	else
	if (lpBasicItem->isExclusiveMaleJob())
	{
		cTEXT::Put(x,y+=16,WHITE,"남성 캐릭터만 가능");
		y	+=	24;
	}
	else
	if (lpBasicItem->isExclusiveFemaleJob())
	{
		cTEXT::Put(x,y+=16,WHITE,"여성 캐릭터만 가능");
		y	+=	24;
	}
	else
	{
		cSTRING		job;

		if	(lpBasicItem->m_bf1IsMaleJobEquipAble)
			job.Add("남성/");
		if	(lpBasicItem->m_bf1IsFemaleJobEquipAble)
			job.Add("여성/");

		for (int i=0;i<dPLAYER_JOB_COUNT;i++)	
			if (lpBasicItem->isEquipAbleJob(i))	
				job.Add("%s/",g_aDefaultJobName[i]);

		int	iLength	=	strlen(job.String);

		if (job.String[iLength-1]	==	'/')
			job.String[iLength-1]	=	NULL;

		int	iLineCount	=	cTEXT::GetLines(iWidth-x,job.String);

		cTEXT::FitPut(x,y+=16,iWidth-x,iLineCount*8*2,WHITE,job.String);

		y	+=	iLineCount*8*2;
		y	+=	10;
	}
	x-=20;

	int	iLimitCount	=	0;

	cTEXT::SetBoldFont();
	cTEXT::Put(x,y,LTYELLOW,"━━ 아이템 착용/사용 제한");
	cTEXT::SetNormalFont();
	x+=20;

	if (lpBasicItem->m_wRequireAllignment	!=	0)
	{
		iLimitCount++;
		cTEXT::Put(x,y+=16,WHITE,_ms("성향제한 : %s성향 캐릭터만 착용/사용 가능",g_strAllignment[lpBasicItem->m_wRequireAllignment]));
	}

	{
		DWORD	dwTextColor	=	WHITE;

		int		iEquipLevel		=	lpBasicItem->m_wRequireLevel;
		int		iEquipMaxLevel	=	lpBasicItem->m_wRequireMaxLevel;
		if (_lpPieceItem&&	_lpPieceItem->m_wEquipLevel)
		{
			iEquipLevel	=	_lpPieceItem->m_wEquipLevel;
			dwTextColor	=	LTGREEN;
		}

		if (iEquipLevel)
		{
			if(iEquipMaxLevel)
				cTEXT::Put(x,y+=16,dwTextColor,_ms("    레벨 : %d ~ %d",iEquipLevel, iEquipMaxLevel));//	사용/장비를 위한 최소 레벨
			else
				cTEXT::Put(x,y+=16,dwTextColor,_ms("    레벨 : %d",iEquipLevel));//	사용/장비를 위한 최소 레벨
			iLimitCount++;
		}
	}
	if (lpBasicItem->m_wRequireStrength)
	{
		cTEXT::Put(x,y+=16,WHITE,_ms("      힘 : %d",lpBasicItem->m_wRequireStrength));//	사용/장비를 위한 최소 힘
		iLimitCount++;
	}
	if (lpBasicItem->m_wRequireDexterity)
	{
		cTEXT::Put(x,y+=16,WHITE,_ms("  민첩성 : %d",lpBasicItem->m_wRequireDexterity));//	사용/장비를 위한 최소 민첩성
		iLimitCount++;
	}
	if (lpBasicItem->m_wRequireConstitution)
	{
		cTEXT::Put(x,y+=16,WHITE,_ms("    건강 : %d",lpBasicItem->m_wRequireConstitution));//	사용/장비를 위한 최소 체질
		iLimitCount++;
	}
	if (lpBasicItem->m_wRequireWisdom)
	{
		cTEXT::Put(x,y+=16,WHITE,_ms("    지혜 : %d",lpBasicItem->m_wRequireWisdom));//	사용/장비를 위한 최소 지혜
		iLimitCount++;
	}
	if (lpBasicItem->m_wRequireIntelligence)
	{
		cTEXT::Put(x,y+=16,WHITE,_ms("    지식 : %d",lpBasicItem->m_wRequireIntelligence));//	사용/장비를 위한 최소 지식
		iLimitCount++;
	}
	if (lpBasicItem->m_wRequireCharisma)
	{	
		cTEXT::Put(x,y+=16,WHITE,_ms("카리스마 : %d",lpBasicItem->m_wRequireCharisma));//	사용/장비를 위한 최소 카리스마
		iLimitCount++;
	}
	if (lpBasicItem->m_wRequireLuck)
	{	
		cTEXT::Put(x,y+=16,WHITE,_ms("      운 : %d",lpBasicItem->m_wRequireLuck));//	사용/장비를 위한 최소 운
		iLimitCount++;
	}

	
	if (lpBasicItem->m_wEquipUseLimitContents	!=	0xffff)
	{
		iLimitCount++;
		cSTRING	limit;

		limit.Add("%8s",g_strEquipUseLimitContents[lpBasicItem->m_wEquipUseLimitContents]);

		switch(lpBasicItem->m_wEquipUseLimitMethod)
		{
			case	0xffff	:
				limit.Add(" : %d 이상이어야 한다.",lpBasicItem->m_wEquipUseLimitValue);
				break;

			case	0	:
				if(lpBasicItem->m_aValue[0][0]==lpBasicItem->m_aValue[0][1])
						limit.Add(" : %d",lpBasicItem->m_aValue[0][0]*lpBasicItem->m_wEquipUseLimitValue);
				else	limit.Add(" : [%d~%d]*%d",lpBasicItem->m_aValue[0][0],lpBasicItem->m_aValue[0][1],lpBasicItem->m_wEquipUseLimitValue);
				break;
			case	1	:
				if(lpBasicItem->m_aValue[1][0]==lpBasicItem->m_aValue[1][1])
						limit.Add(" : %d",lpBasicItem->m_aValue[1][0]*lpBasicItem->m_wEquipUseLimitValue);
				else	limit.Add(" : [%d~%d]*%d",lpBasicItem->m_aValue[1][0],lpBasicItem->m_aValue[1][1],lpBasicItem->m_wEquipUseLimitValue);
				break;
			case	2	:
				if(lpBasicItem->m_aValue[0][0]==lpBasicItem->m_aValue[0][1])
						limit.Add(" : %d",lpBasicItem->m_aValue[0][0]/lpBasicItem->m_wEquipUseLimitValue);
				else	limit.Add(" : [%d~%d]/%d",lpBasicItem->m_aValue[0][0],lpBasicItem->m_aValue[0][1],lpBasicItem->m_wEquipUseLimitValue);
				break;
			case	3	:
				if(lpBasicItem->m_aValue[1][0]==lpBasicItem->m_aValue[1][1])
						limit.Add(" : %d",lpBasicItem->m_aValue[1][0]/lpBasicItem->m_wEquipUseLimitValue);
				else	limit.Add(" : [%d~%d]/%d",lpBasicItem->m_aValue[1][0],lpBasicItem->m_aValue[1][1],lpBasicItem->m_wEquipUseLimitValue);
				break;
		}

		cTEXT::Put(x,y+=16,WHITE,limit.String);//	사용/장비를 위한 최소 운
	}

	if (iLimitCount	==	0)	cTEXT::Put(x,y+=16,WHITE,"제한없음");


	x-=20;
	cTEXT::SetBoldFont();
	cTEXT::Put(x,y+=24,LTYELLOW,"━━ 아이템 설명");
	cTEXT::SetNormalFont();
	x+=20;

	if	(_lpSetItem)
		cTEXT::FitPut(x,y+=16,iWidth-x,100,WHITE,_lpSetItem->m_pItemComment);
	else
		cTEXT::FitPut(x,y+=16,iWidth-x,100,WHITE,lpBasicItem->m_pItemComment);

	_lpDibWnd->Draw();
}

//
//	
int
CSetCustomItemList::SelectItem(int _iX,int _iY)		//09.08.17
{
	int	x=5,y=5;

	if	(s_iLineCount	<=	0)	
		return	0xffff;

	y					=	(s_iItemListHeight	-	s_iLineCountPerPage	*(dICON_SHAPE_HEIGHT+8))/2;

	int		iIndex		=	0;
	int		iCount		=	0;

	if	(!s_rectItemList.isIN(_iX,_iY))
		return	0xffff;

	for (int i=iIndex;i<s_deqFilterIndex.size();++i) //09.08.19
	{
			CCustomItem	*lpCustomItem	=	s_lpItemManager->getItem(s_deqFilterIndex[i]);
		if	(!lpCustomItem)
			continue;

		if	(iCount	>=	s_iLineCountPerPage	)
			break;

		iIndex++;

		if	(iIndex	<=	s_sbItemList.m_iHeightPos)
			continue;

		cBASIC_ITEM	*lpItem	=	&g_aBasicItem[lpCustomItem->m_wBaseItem];

		iCount++;

		if (_iY	>=	y	&&	_iY	<=	y	+	dICON_SHAPE_HEIGHT+8)	return	s_deqFilterIndex[i];

		y	+=	(dICON_SHAPE_HEIGHT+8);
	}

	return	0xffff;
}
void
CSetCustomItemList::FilterCustomItem(HWND _hDlg)	// 09.08.19
{
		
	char strFilter[256] = "";
	GetDlgItemText(_hDlg,IDC_EDIT_CUSTOM_FILTER,strFilter,256);
	int iFilterNameLen	= strlen(strFilter);
	s_deqFilterIndex.clear();

	if(iFilterNameLen<=0)
	{
		InitBox(_hDlg);
		
		return;

	}
	
	int y =5;
	int x =5;	
	cDRAW::Fill(_SKYBLUE);

	
	for (int i=0;i<s_lpItemManager->getSize();++i)
	{
		
		CCustomItem *lpCustomItem = s_lpItemManager->getItem(i);
		
		if (!lpCustomItem)
			continue;


		int iItemNameLen	= strlen(lpCustomItem->m_strName);
		int iLenGap			= iItemNameLen - iFilterNameLen;

		if(iLenGap<0)
			continue;
		
		for(int iName=0;iName<=iLenGap;++iName)
		{
			if(strnicmp(strFilter,&lpCustomItem->m_strName[iName], iFilterNameLen) == 0 )
			{
				s_deqFilterIndex.push_back(i);
				break;		

			}
		}
		
	}
	
	DrawItemListWnd(s_hWnd);
	DrawItemInfoWnd(s_hWnd);


}
//
//	작업 완료
BOOL
CSetCustomItemList::FinishWork(HWND _hDlg)
{
	return	TRUE;
}	//	CSetCustomItemList::FinishWork(HWND _hDlg)

//
//	다얄로그 박스 프로시져
LRESULT CALLBACK
CSetCustomItemList::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				case	IDC_LOADING_TEXT		:
					Loading(hDlg);
					break;
				case	IDC_ADD					:
					AddItem(hDlg);
					break;

				case	IDC_INSERT				:
					InsertItem(hDlg);
					break;

				case	IDC_EDIT				:
					EditItem(hDlg);
					break;

				case	IDC_REMOVE				:
					RemoveItem(hDlg);
					break;

				case	IDC_EXPORT				:
					ExportItemList(hDlg);
					break;

				case	IDOK					:
					if	(FinishWork(hDlg))
						EndDialog(hDlg,IDOK);
					break;

				case	IDCANCEL				:
					EndDialog(hDlg,IDCANCEL);
					break;
				case IDC_EDIT_CUSTOM_FILTER	:	//09.08.17
					
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

LRESULT CALLBACK
CSetCustomItemList::ProcItemListWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)		// 09.08.11
{	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hDlg, &ps );

			s_dibItemList.Active();
			s_dibItemList.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hDlg, &ps);
			break;
		}


		case WM_LBUTTONDOWN	:
		{
			int item		=	SelectItem(LOWORD(lParam),HIWORD(lParam));
			
			if(item			==	0xffff		)	break;
			if(item			==	s_iSelectItem)	break;
			
			s_iSelectItem	=	item;
			
			DrawItemListWnd(s_hWnd);
			DrawItemInfoWnd(s_hWnd);
			SetFocus(hDlg);
			break;
		}
		case WM_MOUSEWHEEL :
			{
				
				int zDelta = ((short)HIWORD(wParam));
				
				
				if	(zDelta>=0)
				{
					s_sbItemList.VScroll(SB_LINEUP);
				}
				else{
					s_sbItemList.VScroll(SB_LINEDOWN);
				}
				
				DrawItemListWnd(s_hWnd);
				DrawItemInfoWnd(s_hWnd);
				break;
			}
		case WM_VSCROLL			:
			if (s_sbItemList.VScroll(wParam))	DrawItemListWnd(s_hWnd);
			break;


		default				:	return( DefWindowProc( hDlg, uMsg, wParam, lParam ) );
	}

	return (0L);
}

LRESULT CALLBACK
CSetCustomItemList::ProcItemInfoWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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