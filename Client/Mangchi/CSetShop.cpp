#include "CSetShop.h"
#include "resource.h"
#include "cDRAW.h"
#include "cTEXT.h"
#include "cITEM.h"

CShop					CSetShop::s_shopEdit;	//	편집중인 상점
HWND					CSetShop::s_hWnd;
cDIBWND					CSetShop::s_dibItemList;
cDIBWND					CSetShop::s_dibItemInfo;
cDIBWND					CSetShop::s_dibTokkenItemInfo;
int						CSetShop::s_iItemListWndPos;
int						CSetShop::s_iSelectItem;
cSCROLL_BAR				CSetShop::s_scrollItemInfo;

int						CSetShop::s_iItemListWidth;
int						CSetShop::s_iItemListHeight;
int						CSetShop::s_iLineCountPerPage;
int						CSetShop::s_iLineCount;
cSCROLL_BAR				CSetShop::s_sbItemList;
cRECT					CSetShop::s_rectItemList;
cBASIC_ITEM*			CSetShop::s_lpSelectItem		=	NULL;

//
//	시작
CShop*
CSetShop::Init(HWND hwnd,CShop *_lpEditShop)
{
	s_shopEdit.reset();

	if (_lpEditShop)	s_shopEdit.copy(_lpEditShop);

	if (DialogBox( s_hInst, "SET_SHOP",hwnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	NULL;

	return	&s_shopEdit;
}	//	CSetShopListList::Init(HWND hwnd)

//
//	박스 초기화
void
CSetShop::InitBox(HWND _hDlg)
{
	s_hWnd	=	_hDlg;

//	컨텐츠 윈도우 초기화
	{
		HWND	hItemList	=	GetDlgItem(_hDlg,IDC_ITEM_LIST);

		s_dibItemList.lpszRegister	=	"ITEM_LIST_WINDOW";
		s_dibItemList.Init(hItemList,(WNDPROC)ProcItemListWnd);

		s_sbItemList.Init(hItemList);		//	스크롤바 초기화
		s_sbItemList.InitHeight();

		s_iItemListWidth	=	s_dibItemList.width();		//	아이템 리스트 너비
		s_iItemListHeight	=	s_dibItemList.height();		//	아이템 리스트 높이
		s_iLineCountPerPage	=	(s_iItemListHeight-10)	/	(dICON_SHAPE_HEIGHT+8);
		s_iLineCount		=	s_shopEdit.getCount();

		s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

		DrawItemListWnd(_hDlg);
	}

//	반응 윈도우 초기화
	{
		HWND	hItemInfo	=	GetDlgItem(_hDlg,IDC_ITEM_INFO);

		s_dibItemInfo.lpszRegister	=	"SHOP_INFO_WINDOW";
		s_dibItemInfo.Init(hItemInfo,(WNDPROC)ProcItemInfoWnd);

		DrawItemInfoWnd(_hDlg);
	}

	HWND	hTokkenShop	=	GetDlgItem(_hDlg , IDC_COMBO_TOKKEN);
	{
		//s_dibTokkenItemInfo.lpszRegister	=	"TOKKEN_WNIDOW";
		//s_dibTokkenItemInfo.Init(hTokkenShop ,(WNDPROC) ProcTokkenItemInfoWnd);

		for (int i = 0 ; i < dBASIC_ITEM_COUNT ; i++)
		{
			cBASIC_ITEM*	 lpBasicItem	=	&g_aBasicItem[i];

			if(!lpBasicItem)
				continue;
			
			cCOMBOBOX::Add(hTokkenShop,lpBasicItem->m_strName);
		}
	}


	SetDlgItemText(_hDlg,IDC_SHOP_NAME,s_shopEdit.m_strName);
	SetDlgItemInt(_hDlg,IDC_SHOP_PRICE_FACTOR,s_shopEdit.m_wPriceFactor,FALSE);
	SetDlgItemInt(_hDlg,IDC_SHOP_LIMIT_PRICE,s_shopEdit.m_dwLimitPrice,FALSE);
	SetDlgItemInt(_hDlg,IDC_SHOP_PRICE_FACTOR2,s_shopEdit.m_bf15LevelPerGoldValue,FALSE);
	CheckDlgButton(_hDlg,IDC_NOT_BUY_ITEM,s_shopEdit.m_bf1IsNotBuyItem);		// 상점 구입불가.
	if(s_shopEdit.m_TokkenShop != 0xffff)	//초기값이 아니면 값을 세팅한다.
		CheckDlgButton(_hDlg,IDC_USE_TOKKEN_SHOP,TRUE);
		
	SendMessage(hTokkenShop , CB_SETCURSEL , s_shopEdit.m_TokkenShop , 0);

	CheckDlgButton(_hDlg,IDC_DUEL_POINT_SHOP,s_shopEdit.m_bf1IsDuelPointShop);
	CheckDlgButton(_hDlg,IDC_LEVEL_PER_GOLD_SHOP,s_shopEdit.m_bf1IsLevelPerGoldShop);
}	//	CSetShopList::InitBox(HWND _hDlg)

//
//최초 분기 추가
void
CSetShop::AddItem(HWND _hDlg)
{
	CShopItem	*lpItem	=	CSetShopItem::Init(_hDlg);

	if (!lpItem)	return;

	int	iSerial			=	s_shopEdit.addItem(lpItem);

	if (iSerial	==	0xffff)	return;

	s_iLineCount		=	s_shopEdit.getCount();
	s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

	s_iSelectItem		=	iSerial;

	DrawItemListWnd(_hDlg);
	DrawItemInfoWnd(_hDlg);
}

//
//선택된 최초 분기 편집
void
CSetShop::EditItem(HWND _hDlg)
{
	CShopItem	*lpGetItem	=	s_shopEdit.get(s_iSelectItem);

	if (!lpGetItem)	return;

	CShopItem	*lpItem		=	CSetShopItem::Init(_hDlg,lpGetItem);

	if (!lpItem)	return;

	memcpy(lpGetItem,lpItem,sizeof(CShopItem));

	s_iLineCount		=	s_shopEdit.getCount();
	s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

	DrawItemListWnd(_hDlg);
	DrawItemInfoWnd(_hDlg);
}//CSetShop::EditItem(HWND _hDlg)

//
//선택된 최초 분기 제거
void
CSetShop::RemoveItem(HWND _hDlg)
{
	s_shopEdit.removeItem(s_iSelectItem);

	s_iLineCount		=	s_shopEdit.getCount();
	s_sbItemList.SetHeight(s_iLineCount,s_iLineCountPerPage);

	int	iCheckItemSlot	=	s_iSelectItem;

	s_iSelectItem		=	0xffff;

	for (int i=iCheckItemSlot;i<dMAX_SHOP_ITEM_COUNT;i++)
	{
		CShopItem	*lpShopItem	=	s_shopEdit.get(i);

		if (!lpShopItem)	continue;

		s_iSelectItem	=	i;

		break;
	}

	if (s_iSelectItem	==	0xffff)
		for (int i=0;i<dMAX_SHOP_ITEM_COUNT;i++)
		{
			CShopItem	*lpShopItem	=	s_shopEdit.get(i);

			if (!lpShopItem)	continue;

			s_iSelectItem	=	i;

			break;
		}

	DrawItemListWnd(_hDlg);
	DrawItemInfoWnd(_hDlg);
}	//	CSetShop::RemoveItem(HWND _hDlg)

//
//	아이템 리스트 윈도우 그리기
void
CSetShop::DrawItemListWnd(HWND _hDlg)
{

	int	x=4,y=5;

	s_dibItemList.Active();

	y					=	(cDRAW::HEIGHT-s_iLineCountPerPage*(dICON_SHAPE_HEIGHT+8))/2;

	s_rectItemList.Set(0,0,cDRAW::WIDTH,cDRAW::HEIGHT);

	if (s_iLineCount	>=	dMAX_SHOP_ITEM_COUNT)
			EnableWindow(GetDlgItem(_hDlg,IDC_ADD),FALSE);
	else	EnableWindow(GetDlgItem(_hDlg,IDC_ADD),TRUE);

	if (s_iLineCount	<=	0)
	{
		cDRAW::Fill(_SKYBLUE);

		cTEXT::PushShadow(FALSE);
		cTEXT::Put(5,5,0,dMSG_ITEM_NOT_EXIST);
		cTEXT::PopShadow();

		EnableWindow(GetDlgItem(_hDlg,IDOK),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE),FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(_hDlg,IDOK),TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT),TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE),TRUE);

		cDRAW::Fill(_SKYBLUE);

		int		iIndex		=	s_sbItemList.m_iHeightPos;
		int		iCount		=	0;

		for (int i=iIndex;i<dMAX_SHOP_ITEM_COUNT;i++)
		{
			CShopItem	*lpShopItem	=	s_shopEdit.get(i);

			if (!lpShopItem)	continue;

			if (iCount	>=	s_iLineCountPerPage	)	break;

			cBASIC_ITEM	*lpItem	=	&g_aBasicItem[lpShopItem->m_wBaseItem];

			iIndex++;
			iCount++;

			if (i==	s_iSelectItem)
			{
				cDRAW::Fill		(0			,x  ,y+2,s_iItemListWidth-2,y+dICON_SHAPE_HEIGHT+6);
				cDRAW::Fill		(_LTBLUE	,x-1,y+1,s_iItemListWidth-3,y+dICON_SHAPE_HEIGHT+5);
				cDRAW::Box		(_DEEPGRAY	,x-1,y+1,s_iItemListWidth-3,y+dICON_SHAPE_HEIGHT+5);

				g_smiIconItem.put(lpItem->m_wIconShape,x+1,y+4);

				if (lpItem->isUnique())
				{
					cTEXT::Put(x+1+36,y+4+2,GOLDYELLOW,"★");
					cTEXT::Put(x+1+36,y+4+2,GOLDYELLOW,_ms("   %s",lpItem->m_strName));
				}
				else
				if (lpItem->isExtraItem())
				{
					cTEXT::Put(x+1+36,y+4+2,LTCYAN,"♠");
					cTEXT::Put(x+1+36,y+4+2,LTCYAN,_ms("   %s",lpItem->m_strName));
				}
				else
					cTEXT::Put(x+1+36,y+4+2,WHITE,lpItem->m_strName);

				cTEXT::Put		(x+1+36,y+4+18,LTGRAY,g_strItemCategory[lpItem->m_wKind]);
			}
			else
			{
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
			}

			y	+=	(dICON_SHAPE_HEIGHT+8);
		}
	}

	s_dibItemList.Draw();
	
}	//	CSetShop::DrawItemListWnd(HWND _hDlg)

//
//	아이템 정보 윈도우 그리기
void
CSetShop::DrawItemInfoWnd(HWND _hDlg)
{
	int	x=5,y=5;

	//	아이템 설명을 그려준다.
	s_dibItemInfo.Active();

	if (s_iLineCount	<=	0)
	{
		s_scrollItemInfo.SetHeight(1,2);

		cDRAW::Fill(0);
		s_dibItemInfo.Draw();

		return;
	}

	CShopItem	*lpItem	=	s_shopEdit.get(s_iSelectItem);

	if (!lpItem	)
	{
		s_scrollItemInfo.SetHeight(1,2);

		cDRAW::Fill(0);
		s_dibItemInfo.Draw();

		return;
	}

	s_lpSelectItem	=	&g_aBasicItem[lpItem->m_wBaseItem];

	cDRAW::Fill(0);

	cTEXT::SetBoldFont();
	cTEXT::Put(x, y, LTYELLOW, "━━ Item image");
	cTEXT::SetNormalFont();
	// Wear image

	if (s_lpSelectItem->m_wEquippedShape != 0xffff)
		cTEXT::Put(x + 20, y += 16, WHITE, _ms("Wear Image : %s", g_strViewEquipmentName[s_lpSelectItem->m_wKind][s_lpSelectItem->m_wEquippedShape]));

	// Drop image
	cTEXT::Put(x + 20, y += 16, WHITE, "Field image: ");
	cDRAW::Fill(_BLUE, x + 20 + 14 * 6, y, x + 20 + 14 * 6 + 80, y + 50);
	g_sprFieldShape.FitPut(x + 20 + 14 * 6, y, 80, 50, s_lpSelectItem->m_wFieldShape);


	y+=60;

	cTEXT::SetBoldFont();
	cTEXT::Put(x, y, GOLDYELLOW, "━━ Information for a store item");
	cTEXT::SetNormalFont();

	if (lpItem->m_dwPrice == 0)
		cTEXT::Put(x, y += 16, LTGREEN, " Item price: apply existing price formula");
	else cTEXT::Put(x, y += 16, LTGREEN, _ms(" Item price : %dGold", lpItem->m_dwPrice));

	cTEXT::Put(x, y += 16, LTGREEN, _ms("Item generate quality : %d％", lpItem->m_wGenerateQuality));

	if (lpItem->m_bf10ChangeLimitLevel)
	{
	cTEXT::Put(x, y += 16, LTGREEN, _ms("Wear level : %d hours", lpItem->m_bf10ChangeLimitLevel));
	// Prefix change interval
	}
	else
	{
	//	접두사
		int	iPrefixCount	=	lpItem->getPrefixCount();

		if (iPrefixCount)
		{
			cTEXT::Put(x, y += 16, LTGREEN, _ms("Prefix generation quality : %d％", lpItem->m_wPrefixQuality));

			for (int i=0;i<iPrefixCount;i++)
			{
				cITEM_PREFIX *lpPrefix	=	g_itemPrefix.get(lpItem->m_aPrefix[i].m_wPrefix);

				cTEXT::Put(x, y += 16, LTGREEN, _ms(" Prefix %d : %s", i + 1, lpPrefix->m_str1stPrefix));
			}
		}
	}

	y+=24;

	cTEXT::SetBoldFont();
	cTEXT::Put(x,y,LTYELLOW,"━━ 아이템 정보");
	cTEXT::SetNormalFont();

	x+=20;

	if (s_lpSelectItem->m_aValue[0][1])
	{
		if (s_lpSelectItem->m_aValue[0][0] == s_lpSelectItem->m_aValue[0][1])
			cTEXT::Put(x, y += 16, WHITE, _ms("Generated figure 1 : %d", s_lpSelectItem->m_aValue[0][0]));
		else cTEXT::Put(x, y += 16, WHITE, _ms("Generated figures 1 : %d~%d", s_lpSelectItem->m_aValue[0][0], s_lpSelectItem->m_aValue[0][1]));
	}
	if (s_lpSelectItem->m_aValue[1][1])
	{
		if (s_lpSelectItem->m_aValue[1][0] == s_lpSelectItem->m_aValue[1][1])
			cTEXT::Put(x, y += 16, WHITE, _ms("Generated figure 2 : %d", s_lpSelectItem->m_aValue[1][0]));
		else cTEXT::Put(x, y += 16, WHITE, _ms("Generated numbers 2 : %d~%d", s_lpSelectItem->m_aValue[1][0], s_lpSelectItem->m_aValue[1][1]));
	}

	if (s_lpSelectItem->isDamageItem())
	{
		cTEXT::Put(x, y += 16, WHITE, _ms("Min Damage : %d", s_lpSelectItem->m_wMinDamage));
		cTEXT::Put(x, y += 16, WHITE, _ms("Max Damage : %d", s_lpSelectItem->m_wMaxDamage));
	}

	if (s_lpSelectItem->m_wSpeed)
		cTEXT::Put(x, y += 16, WHITE, _ms(" Attack Speed : %d.%.2d", s_lpSelectItem->m_wSpeed / 100, s_lpSelectItem->m_wSpeed % 100));

	if (s_lpSelectItem->m_wRange)
		cTEXT::Put(x, y += 16, WHITE, _ms(" Range : %d Pixel", s_lpSelectItem->m_wRange));

	if (s_lpSelectItem->m_wDamageRange)
		cTEXT::Put(x, y += 16, WHITE, _ms("Damage Range : %d Pixel", s_lpSelectItem->m_wDamageRange));

	switch(s_lpSelectItem->m_wPriceComputeMethod)
	{
		case	0	:
			cTEXT::Put(x, y += 16, WHITE, _ms(" Price : %d", s_lpSelectItem->m_dwPrice));
			break;
		case 1:
			if (s_lpSelectItem->m_aValue[0][0] == s_lpSelectItem->m_aValue[0][1])
				cTEXT::Put(x, y += 16, WHITE, _ms(" Price : %d", s_lpSelectItem->m_aValue[0][0]));
			else cTEXT::Put(x, y += 16, WHITE, _ms(" Price : %d~%d", s_lpSelectItem->m_aValue[0][0], s_lpSelectItem->m_aValue[0][1]));
			break;
		case 2:
			if (s_lpSelectItem->m_aValue[0][0] == s_lpSelectItem->m_aValue[0][1])
				cTEXT::Put(x, y += 16, WHITE, _ms(" Price : %d*%d", s_lpSelectItem->m_dwPrice, s_lpSelectItem->m_aValue[0][0]));
			else cTEXT::Put(x, y += 16, WHITE, _ms(" Price : %d*[%d~%d]", s_lpSelectItem->m_dwPrice, s_lpSelectItem->m_aValue[0][0], s_lpSelectItem->m_aValue[0][1]));
			break;
		case 3:
			if (s_lpSelectItem->m_aValue[1][0] == s_lpSelectItem->m_aValue[1][1])
				cTEXT::Put(x, y += 16, WHITE, _ms(" Price : %d*%d", s_lpSelectItem->m_dwPrice, s_lpSelectItem->m_aValue[1][0]));
			else cTEXT::Put(x, y += 16, WHITE, _ms(" Price : %d*[%d~%d]", s_lpSelectItem->m_dwPrice, s_lpSelectItem->m_aValue[1][0], s_lpSelectItem->m_aValue[1][1]));
			break;
		case 4:
			if (s_lpSelectItem->m_aValue[0][0] == s_lpSelectItem->m_aValue[0][1])
				cTEXT::Put(x, y += 16, WHITE, _ms(" Price : %d*(%d+1)", s_lpSelectItem->m_dwPrice, s_lpSelectItem->m_aValue[0][0]));
			else cTEXT::Put(x, y += 16, WHITE, _ms(" Price : %d*([%d~%d]+1)", s_lpSelectItem->m_dwPrice, s_lpSelectItem->m_aValue[0][0], s_lpSelectItem->m_aValue[0][1]));
			break;

		case	5	:
		{
			char	strValue1[32],strValue2[32];

			if (s_lpSelectItem->m_aValue[0][0]==s_lpSelectItem->m_aValue[0][1])	sprintf(strValue1,"%d",s_lpSelectItem->m_aValue[0][0]);
			else																sprintf(strValue1,"[%d~%d]",s_lpSelectItem->m_aValue[0][0],s_lpSelectItem->m_aValue[0][1]);

			if (s_lpSelectItem->m_aValue[1][0]==s_lpSelectItem->m_aValue[1][1])	sprintf(strValue2,"%d",s_lpSelectItem->m_aValue[1][0]);
			else																sprintf(strValue2,"[%d~%d]",s_lpSelectItem->m_aValue[1][0],s_lpSelectItem->m_aValue[1][1]);

			cTEXT::Put(x, y += 16, WHITE, _ms(" Price : %s*%s/%d", strValue1, strValue2, s_lpSelectItem->m_dwPrice)); 
			break; 

		} case 6:			
			cTEXT::Put(x, y += 16, WHITE, _ms(" Price : %d won for 10 pieces", s_lpSelectItem->m_dwPrice));
			break;

		case	7	:
			char	strValue1[32],strValue2[32];

			if (s_lpSelectItem->m_aValue[0][0]==s_lpSelectItem->m_aValue[0][1])	sprintf(strValue1,"%d",s_lpSelectItem->m_aValue[0][0]);
			else																sprintf(strValue1,"[%d~%d]",s_lpSelectItem->m_aValue[0][0],s_lpSelectItem->m_aValue[0][1]);

			if (s_lpSelectItem->m_aValue[1][0]==s_lpSelectItem->m_aValue[1][1])	sprintf(strValue2,"%d",s_lpSelectItem->m_aValue[1][0]);
			else																sprintf(strValue2,"[%d~%d]",s_lpSelectItem->m_aValue[1][0],s_lpSelectItem->m_aValue[1][1]);

			cTEXT::Put(x, y += 16, WHITE, _ms(" Price : %s*%s*%d", strValue1, strValue2, s_lpSelectItem->m_dwPrice));
			break;
	}

	cTEXT::Put(x, y += 16, WHITE, _ms(" Drop Level : %d", s_lpSelectItem->m_wDropLevel));

	cTEXT::Put(x, y += 16, WHITE, _ms(" Stack limit : %d", s_lpSelectItem->m_wStackLimit));
	cTEXT::Put(x, y += 16, WHITE, _ms(" Durability : %d", s_lpSelectItem->m_wDurability));

	x-=20;

	int	iIndex	=	0;
	int	iWidth	=	s_dibItemInfo.width()- 14;

	y+=24;

	if (s_lpSelectItem->m_aGenerateData[0].m_wEffect	!=	0xffff)
	{
		cTEXT::SetBoldFont();
		cTEXT::Put(x, y, LTYELLOW, "━━ Item creation information");
		cTEXT::SetNormalFont();

		y+=16;
		x+=20;

		while(1)
		{
			if (iIndex	>=	4)	break;

			if (s_lpSelectItem->m_aGenerateData[iIndex].m_wEffect	==	0xffff)	break;

			char	*lpComment	=	s_lpSelectItem->getDataComment(iIndex,TRUE);
			int		iLineCount	=	cTEXT::GetLines(iWidth-x,_ms("%d. %s",iIndex+1,lpComment));

			cTEXT::FitPut(x,y,iWidth-x,iLineCount*8*2,WHITE,_ms("%d. %s",iIndex+1,lpComment));

			y	+=	iLineCount*14;
			iIndex++;
		}

		x-=20;
		y+=10;
	}

	if (s_lpSelectItem->m_aUniqueData[0].m_wEffect	!=	0xffff)
	{
		cTEXT::SetBoldFont();
		cTEXT::Put(x, y, LTYELLOW, "━━ Unique information");
		cTEXT::SetNormalFont();

		iIndex	=	0;
		y+=16;
		x+=20;

		while(1)
		{
			if (iIndex	>=	6)	break;
			
			if (s_lpSelectItem->m_aUniqueData[iIndex].m_wEffect	==	0xffff)	break;

			char	*lpComment	=	s_lpSelectItem->m_aUniqueData[iIndex].getComment(TRUE);

			int		iLineCount	=	cTEXT::GetLines(iWidth-x,_ms("%d. %s",iIndex+1,lpComment));

			cTEXT::FitPut(x,y,iWidth-x,iLineCount*8*2,WHITE,_ms("%d. %s",iIndex+1,lpComment));

			y	+=	iLineCount*14;
			iIndex++;
		}
		x-=20;
		y+=10;
	}

	cTEXT::SetBoldFont();
	cTEXT::Put(x, y, LTYELLOW, "━━ Item can be worn/used by");
	cTEXT::SetNormalFont();
	x+=20;

	if (s_lpSelectItem->isEnableAllJob())
	{
		cTEXT::Put(x, y += 16, WHITE, "No job restrictions");
		y	+=	24;
	}
	else
	if (s_lpSelectItem->isExclusiveMaleJob())
	{
		cTEXT::Put(x, y += 16, WHITE, "Male characters only");
		y	+=	24;
	}
	else
	if (s_lpSelectItem->isExclusiveFemaleJob())
	{
		cTEXT::Put(x, y += 16, WHITE, "Female character only");
		y	+=	24;
	}
	else
	{
		cSTRING		job;


		if (s_lpSelectItem->m_bf1IsMaleJobEquipAble) job.Add("Male/");
		if (s_lpSelectItem->m_bf1IsFemaleJobEquipAble) job.Add("Female/");

		for (int i=0;i<dPLAYER_JOB_COUNT;i++)	if (s_lpSelectItem->isEquipAbleJob(i))	job.Add("%s/",g_aDefaultJobName[i]);

		int	iLineCount	=	cTEXT::GetLines(iWidth-x,job.String);

		cTEXT::FitPut(x,y+=16,iWidth-x,iLineCount*8*2,WHITE,job.String);

		y	+=	iLineCount*8*2;
		y	+=	10;
	}
	x-=20;

	int	iLimitCount	=	0;

	cTEXT::SetBoldFont();
	cTEXT::Put(x, y, LTYELLOW, "━━ Restrictions on wearing/using this item");
	cTEXT::SetNormalFont();
	x += 20;

	if (s_lpSelectItem->m_wRequireAllignment != 0)
	{
		iLimitCount++;
		cTEXT::Put(x, y += 16, WHITE, _ms("Disposition restriction : can only be worn/used by %s disposition character", g_strAllignment[s_lpSelectItem->m_wRequireAllignment]));
	}
	if (s_lpSelectItem->m_wRequireLevel)
	{
		if (s_lpSelectItem->m_wRequireMaxLevel)
			cTEXT::Put(x, y += 16, WHITE, _ms(" Level : %d ~ %d", s_lpSelectItem->m_wRequireLevel, s_lpSelectItem->m_wRequireMaxLevel));// Minimum level for use/equipment
		else
			cTEXT::Put(x, y += 16, WHITE, _ms(" Level : %d", s_lpSelectItem->m_wRequireLevel));// Minimum level for use/equipment
		iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireStrength)
	{
		cTEXT::Put(x, y += 16, WHITE, _ms(" Strength : %d", s_lpSelectItem->m_wRequireStrength));// Minimum strength for use/equipment
		iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireDexterity)
	{
	cTEXT::Put(x, y += 16, WHITE, _ms(" Agility : %d", s_lpSelectItem->m_wRequireDexterity));// Minimum agility for use/equipment
	iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireConstitution)
	{
		cTEXT::Put(x, y += 16, WHITE, _ms(" Health : %d", s_lpSelectItem->m_wRequireConstitution));// minimum constitution for use/equipment
		iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireWisdom)
	{
		cTEXT::Put(x, y += 16, WHITE, _ms(" Wisdom : %d", s_lpSelectItem->m_wRequireWisdom));// Minimum wisdom for use/equipment
		iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireIntelligence)
	{
		cTEXT::Put(x, y += 16, WHITE, _ms(" Knowledge : %d", s_lpSelectItem->m_wRequireIntelligence));// Minimum knowledge for use/equipment
		iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireCharisma)
	{
		cTEXT::Put(x, y += 16, WHITE, _ms("Charisma : %d", s_lpSelectItem->m_wRequireCharisma));// Minimum Charisma for use/equipment
		iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireLuck)
	{
		cTEXT::Put(x, y += 16, WHITE, _ms(" Luck : %d", s_lpSelectItem->m_wRequireLuck));// Minimum Luck for use/equipment
		iLimitCount++;
	}

	
	if (s_lpSelectItem->m_wEquipUseLimitContents	!=	0xffff)
	{
		iLimitCount++;
		cSTRING	limit;

		limit.Add("%8s",g_strEquipUseLimitContents[s_lpSelectItem->m_wEquipUseLimitContents]);

		switch(s_lpSelectItem->m_wEquipUseLimitMethod)
		{
			case	0xffff	:
				limit.Add(" : Must be at least %d.", s_lpSelectItem->m_wEquipUseLimitValue);
				break;

			case	0	:
				if(s_lpSelectItem->m_aValue[0][0]==s_lpSelectItem->m_aValue[0][1])
						limit.Add(" : %d",s_lpSelectItem->m_aValue[0][0]*s_lpSelectItem->m_wEquipUseLimitValue);
				else	limit.Add(" : [%d~%d]*%d",s_lpSelectItem->m_aValue[0][0],s_lpSelectItem->m_aValue[0][1],s_lpSelectItem->m_wEquipUseLimitValue);
				break;
			case	1	:
				if(s_lpSelectItem->m_aValue[1][0]==s_lpSelectItem->m_aValue[1][1])
						limit.Add(" : %d",s_lpSelectItem->m_aValue[1][0]*s_lpSelectItem->m_wEquipUseLimitValue);
				else	limit.Add(" : [%d~%d]*%d",s_lpSelectItem->m_aValue[1][0],s_lpSelectItem->m_aValue[1][1],s_lpSelectItem->m_wEquipUseLimitValue);
				break;
			case	2	:
				if(s_lpSelectItem->m_aValue[0][0]==s_lpSelectItem->m_aValue[0][1])
						limit.Add(" : %d",s_lpSelectItem->m_aValue[0][0]/s_lpSelectItem->m_wEquipUseLimitValue);
				else	limit.Add(" : [%d~%d]/%d",s_lpSelectItem->m_aValue[0][0],s_lpSelectItem->m_aValue[0][1],s_lpSelectItem->m_wEquipUseLimitValue);
				break;
			case	3	:
				if(s_lpSelectItem->m_aValue[1][0]==s_lpSelectItem->m_aValue[1][1])
						limit.Add(" : %d",s_lpSelectItem->m_aValue[1][0]/s_lpSelectItem->m_wEquipUseLimitValue);
				else	limit.Add(" : [%d~%d]/%d",s_lpSelectItem->m_aValue[1][0],s_lpSelectItem->m_aValue[1][1],s_lpSelectItem->m_wEquipUseLimitValue);
				break;
		}

		cTEXT::Put(x, y += 16, WHITE, limit.String);// minimum luck for use/equipment
	}

	if (iLimitCount == 0) cTEXT::Put(x, y += 16, WHITE, "no limit");


	x -= 20;
	cTEXT::SetBoldFont();
	cTEXT::Put(x, y += 24, LTYELLOW, "━━ Item Description");
	cTEXT::SetNormalFont();
	x+=20;
	cTEXT::FitPut(x,y+=16,iWidth-x,100,WHITE,s_lpSelectItem->m_pItemComment);

	s_dibItemInfo.Draw();
}	//	CSetShop::DrawItemInfoWnd(HWND _hDlg);

//
//	
int
CSetShop::SelectItem(int _iX,int _iY)
{
	int	x=5,y=5;

	if (s_iLineCount	<=	0)	return	0xffff;

	y					=	(s_iItemListHeight	-	s_iLineCountPerPage	*(dICON_SHAPE_HEIGHT+8))/2;

	int		iIndex		=	s_sbItemList.m_iHeightPos;
	int		iCount		=	0;

	if (!s_rectItemList.isIN(_iX,_iY))	return	0xffff;

	for (int i=iIndex;i<dMAX_SHOP_ITEM_COUNT;i++)
	{
		CShopItem	*lpShopItem	=	s_shopEdit.get(i);

		if (!lpShopItem)	continue;

		if (iCount	>=	s_iLineCountPerPage	)	break;

		cBASIC_ITEM	*lpItem	=	&g_aBasicItem[lpShopItem->m_wBaseItem];

		iIndex++;
		iCount++;

		if (_iY	>=	y	&&	_iY	<=	y	+	dICON_SHAPE_HEIGHT+8)	return	i;

		y	+=	(dICON_SHAPE_HEIGHT+8);
	}

	return	0xffff;
}	//	cSET_BASIC_ITEM::SelectItem(HWND hwnd,int x,int y)

//
//	아이템 위로
void
CSetShop::UpItem(HWND _hDlg)
{
	if(s_iSelectItem >= s_shopEdit.getCount() || s_iSelectItem <= 0)	return;

	s_shopEdit.changeItemPlace(s_iSelectItem,s_iSelectItem-1);

	s_iSelectItem	=	s_iSelectItem-1;
	g_lpActmap->LostVirgin();

	DrawItemListWnd(_hDlg);
}	//	CSetShop::UpItem(HWND _hDlg)

//
//	아이템 위로
void
CSetShop::DownItem(HWND _hDlg)
{
	if(s_iSelectItem>=s_shopEdit.getCount()-1 || s_iSelectItem < 0)	return;

	s_shopEdit.changeItemPlace(s_iSelectItem,s_iSelectItem+1);

	s_iSelectItem	=	s_iSelectItem+1;
	g_lpActmap->LostVirgin();

	DrawItemListWnd(_hDlg);
}	//	CSetShop::DownItem(HWND _hDlg)

//
//	작업 완료
BOOL
CSetShop::FinishWork(HWND _hDlg)
{	// 마지막 수정일 : 09.10.16
	BOOL	temp;

	GetDlgItemText(_hDlg,IDC_SHOP_NAME,s_shopEdit.m_strName,sizeof(s_shopEdit.m_strName)-2);
	s_shopEdit.m_wPriceFactor		=	GetDlgItemInt(_hDlg,IDC_SHOP_PRICE_FACTOR,&temp,FALSE);
	s_shopEdit.m_dwLimitPrice		=	GetDlgItemInt(_hDlg,IDC_SHOP_LIMIT_PRICE,&temp,FALSE);
	s_shopEdit.m_bf1IsDuelPointShop	=	IsDlgButtonChecked(_hDlg,IDC_DUEL_POINT_SHOP);
	s_shopEdit.m_bf1IsLevelPerGoldShop	= IsDlgButtonChecked(_hDlg,IDC_LEVEL_PER_GOLD_SHOP);
	s_shopEdit.m_bf15LevelPerGoldValue	= GetDlgItemInt(_hDlg,IDC_SHOP_PRICE_FACTOR2,&temp,FALSE);	
	s_shopEdit.m_bf1IsNotBuyItem		= IsDlgButtonChecked(_hDlg, IDC_NOT_BUY_ITEM);		// 상점 구입 불가.
	s_shopEdit.m_TokkenShop				= (WORD)SendMessage(GetDlgItem(_hDlg , IDC_COMBO_TOKKEN) , CB_GETCURSEL , 0 , 0);
	if(!IsDlgButtonChecked(_hDlg,IDC_USE_TOKKEN_SHOP))
		s_shopEdit.m_TokkenShop	= 0xffff;

	s_shopEdit.m_dwLimitPrice		=	max(s_shopEdit.m_dwLimitPrice,5000);
	s_shopEdit.m_dwLimitPrice		=	min(s_shopEdit.m_dwLimitPrice,1000000);
	s_shopEdit.m_wPriceFactor		=	max(s_shopEdit.m_wPriceFactor,1);
	s_shopEdit.m_wPriceFactor		=	min(s_shopEdit.m_wPriceFactor,500);

	if (stricmp(s_shopEdit.m_strName, "") == 0) strcpy(s_shopEdit.m_strName, "Give me a name!!");

	return	TRUE;
}	//	CSetShop::FinishWork(HWND _hDlg)

//
//	다얄로그 박스 프로시져
LRESULT CALLBACK
CSetShop::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				case	IDC_ADD					:
					AddItem(hDlg);
					break;

				case	IDC_EDIT				:
					EditItem(hDlg);
					break;

				case	IDC_UP					:
					UpItem(hDlg);
					break;

				case	IDC_DOWN				:
					DownItem(hDlg);
					break;

				case	IDC_REMOVE				:
					RemoveItem(hDlg);
					break;

				case	IDOK					:
					if (FinishWork(hDlg))	EndDialog(hDlg,IDOK);
					break;

				case	IDCANCEL				:
					EndDialog(hDlg,IDCANCEL);
					break;
			}
			break;
	}

	return (0L);
}	//	CSetShopList::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)


LRESULT CALLBACK
CSetShop::ProcItemListWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

			break;
		}

		case WM_VSCROLL			:
			if (s_sbItemList.VScroll(wParam))	DrawItemListWnd(s_hWnd);
			break;


		default				:	return( DefWindowProc( hDlg, uMsg, wParam, lParam ) );
	}

	return (0L);
}	//	CSetShop::ProcItemListWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)

LRESULT CALLBACK
CSetShop::ProcItemInfoWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
}	//	CSetShop::ProcItemInfoWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)

