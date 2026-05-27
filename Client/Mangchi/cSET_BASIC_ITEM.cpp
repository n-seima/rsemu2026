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
#include "rsComm.h"

int				cSET_BASIC_ITEM::s_iSelectKind		=	eIK_HELM;
int				cSET_BASIC_ITEM::s_iSelectItem		=	0xffffffff;
int				cSET_BASIC_ITEM::s_iLinePos			=	0;
int				cSET_BASIC_ITEM::s_iLineCount		=	0;
int				cSET_BASIC_ITEM::s_iLineCountPerPage=	0;
int				cSET_BASIC_ITEM::s_iItemListWidth	=	0;
int				cSET_BASIC_ITEM::s_iItemListHeight	=	0;	//	아이템 리스트 박스 너비와 높이
BOOL			cSET_BASIC_ITEM::s_isSort			=	TRUE;
BOOL			cSET_BASIC_ITEM::s_isUniqueFirst	=	TRUE;
BOOL			cSET_BASIC_ITEM::s_bIsShowSerial	=	FALSE;

cBASIC_ITEM*	cSET_BASIC_ITEM::s_lpSelectItem		=	NULL;
cBASIC_ITEM		cSET_BASIC_ITEM::s_clipItem;

cRECT			cSET_BASIC_ITEM::s_rectItemList;
HWND			cSET_BASIC_ITEM::s_hWnd;
cSCROLL_BAR		cSET_BASIC_ITEM::s_ScrollBar;
cSCROLL_BAR		cSET_BASIC_ITEM::s_scrollItemInfo;

cDIBWND			cSET_BASIC_ITEM::s_dibItemList;
cDIBWND			cSET_BASIC_ITEM::s_dibItemInfo;
BOOL			cSET_BASIC_ITEM::s_isInitializing	=	TRUE;


int				l_iFieldItemShapeCount	=	0;
int				l_iEquippedItemShapeCount	=	0;
int				l_iIconItemShapeCount	=	0;

#define			dQUEST_ITEM_KIND_BORDER				10000

//
//	기본 아이템 목록 설정
BOOL
cSET_BASIC_ITEM::Init(HWND hwnd)
{
	int	result	=	DialogBox( MAIN.hINST,"SET_ITEM_LIST",hwnd,(DLGPROC)s_procSetBasicItemList);

	cPROJECT::SaveItem();

	s_makeItem.reset();

	return	result;
}	//	cSET_BASIC_ITEM::Init(HWND hwnd)

//
//	기본 아이템 목록 설정
void
cSET_BASIC_ITEM::InitItemListBox(HWND hwnd)
{
	s_hWnd				=	hwnd;
	//	아이템 리스트를 그리고
	HWND	hComboItemCategory		=	GetDlgItem(hwnd,IDC_ITEM_CATEGORY);

	HWND	hItemListWnd			=	GetDlgItem(hwnd,IDC_BASIC_ITEM_LIST);
	HWND	hItemExplainWnd			=	GetDlgItem(hwnd,IDC_ITEM_DETAIL);

	MoveWindow(GetDlgItem(hwnd,IDC_LIST),-10000,-10000,100,100,FALSE);
	MoveWindow(GetDlgItem(hwnd,IDC_NOT_SORT_LIST),-10000,-10000,100,100,FALSE);

	s_dibItemList.lpszRegister		=	"Red Stone Basic Item List DibWnd";
	s_dibItemInfo.lpszRegister		=	"Red Stone Basic Item Explain DibWnd";

	s_dibItemList.Init	(hItemListWnd	,(WNDPROC)s_procItemListWnd);
	s_dibItemInfo.Init	(hItemExplainWnd,(WNDPROC)s_procItemInfoWnd);


	s_ScrollBar.Init(hItemListWnd);		//	스크롤바 초기화
	s_ScrollBar.InitHeight();
	s_scrollItemInfo.Init(hItemExplainWnd);		//	스크롤바 초기화
	s_scrollItemInfo.InitHeight();

	s_iItemListWidth				=	s_dibItemList.width();		//	아이템 리스트 너비
	s_iItemListHeight				=	s_dibItemList.height();		//	아이템 리스트 높이
	s_iLineCountPerPage				=	(s_iItemListHeight-10)	/	(dICON_SHAPE_HEIGHT+8);
	s_iLineCount					=	cBASIC_ITEM::GetCountByKind(s_iSelectKind);

	s_ScrollBar.SetHeight(s_iLineCount,s_iLineCountPerPage);

	int		iIndex	=	0;

	cCOMBOBOX::Add(hComboItemCategory,dMSG_ALL_KIND_ITEM,dITEM_KIND_ALL);

	while(1)
	{
		if (strcmp(g_strItemCategory[iIndex],"end")==0)	break;

		cCOMBOBOX::Add(hComboItemCategory,g_strItemCategory[iIndex],iIndex);

		iIndex++;
	}

	cCOMBOBOX::SelectByData(hComboItemCategory,s_iSelectKind);

	if (s_isSort)	CheckDlgButton(hwnd,IDC_SORT,TRUE);
	else			CheckDlgButton(hwnd,IDC_SORT,FALSE);

	ResetItemListBox(hwnd,TRUE);
	PlaceItemToViewAble(hwnd,s_iSelectItem);
	DrawItemList(hwnd);

	SetFocus(GetDlgItem(hwnd,IDOK));
}


//
//	기본 아이템 목록 설정
void
cSET_BASIC_ITEM::ResetItemListBox(HWND hwnd,BOOL _bReset)
{
	HWND	hItemList		=	GetDlgItem(hwnd,IDC_LIST);
	HWND	hNotSortList	=	GetDlgItem(hwnd,IDC_NOT_SORT_LIST);
	int		iIndex			=	0;

	s_iLineCount			=	cBASIC_ITEM::GetCountByKind(s_iSelectKind);
	s_ScrollBar.SetHeight(s_iLineCount,s_iLineCountPerPage);

	if (!s_isSort)	hItemList	=	hNotSortList;

	if (_bReset)
	{
		cLISTBOX::Reset(hItemList);

		while(1)
		{
			cBASIC_ITEM	*lpItem	=	cBASIC_ITEM::GetItemByKind(s_iSelectKind,&iIndex);

			if(!lpItem)	break;

			cLISTBOX::Add(hItemList,lpItem->m_strName,lpItem->m_iSerial);
		}
	}

	//	아이템 설명을 그려준다.
}

//
//	_iItem을 보이게 스크롤바의 위치를 조절한다.
void
cSET_BASIC_ITEM::PlaceItemToViewAble(HWND _hWnd,int _iItem)
{
	int		iIndex		=	s_ScrollBar.m_iHeightPos;
	HWND	hListBox	=	GetDlgItem(_hWnd,IDC_LIST);
	HWND	hNotSortList=	GetDlgItem(_hWnd,IDC_NOT_SORT_LIST);

	if (!s_isSort)	hListBox	=	hNotSortList;

	int		iItemPos	=	cLISTBOX::GetIndexByData(hListBox,_iItem);

	if (iItemPos	!=	CB_ERR)
	{
		if (iItemPos	<	iIndex)
		{
			s_ScrollBar.SetHeightPos(iItemPos);
		}

		if (iItemPos	>	iIndex+s_iLineCountPerPage-1)
		{
			iIndex	=	iItemPos-s_iLineCountPerPage+1;
			s_ScrollBar.SetHeightPos(iIndex);
		}
	}
}

//
//	아이템 리스트를 그려준다.
void
cSET_BASIC_ITEM::DrawItemList(HWND hwnd)
{
	int	x=4,y=5;

	y	-=	s_scrollItemInfo.m_iHeightPos;

	//	아이템 리스트를 그리고
	s_dibItemList.Active();

	y					=	(cDRAW::HEIGHT	-	s_iLineCountPerPage	*(dICON_SHAPE_HEIGHT+8))/2;

	s_rectItemList.Set(0,0,cDRAW::WIDTH,cDRAW::HEIGHT);

	if (s_clipItem.m_iSerial	!=	0xffffffff	&&	(s_clipItem.m_wKind	==	s_iSelectKind	||	s_iSelectKind	==	dITEM_KIND_ALL)	)
			EnableWindow(GetDlgItem(hwnd,IDC_PASTE),TRUE);
	else	EnableWindow(GetDlgItem(hwnd,IDC_PASTE),FALSE);

	if (s_iLineCount	<=	0)
	{
		cDRAW::Fill(_SKYBLUE);

		cTEXT::PushShadow(FALSE);
		cTEXT::Put(5,5,0,dMSG_ITEM_NOT_EXIST);
		cTEXT::PopShadow();

		EnableWindow(GetDlgItem(hwnd,IDC_COPY),FALSE);
		EnableWindow(GetDlgItem(hwnd,IDC_EDIT),FALSE);
		EnableWindow(GetDlgItem(hwnd,IDC_REMOVE),FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(hwnd,IDC_COPY),TRUE);
		EnableWindow(GetDlgItem(hwnd,IDC_EDIT),TRUE);
		EnableWindow(GetDlgItem(hwnd,IDC_REMOVE),TRUE);

		cDRAW::Fill(_SKYBLUE);

		int		iIndex		=	s_ScrollBar.m_iHeightPos;
		int		iCount		=	0;

		HWND	hListBox	=	GetDlgItem(hwnd,IDC_LIST);
		HWND	hNotSortList=	GetDlgItem(hwnd,IDC_NOT_SORT_LIST);

		if (!s_isSort)	hListBox	=	hNotSortList;

		if (!cBASIC_ITEM::IsExist(s_iSelectItem,s_iSelectKind))	s_iSelectItem	=	cLISTBOX::GetData(hListBox,0);

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
					if (s_bIsShowSerial)
							cTEXT::Put(x+1+36,y+4+2,GOLDYELLOW,_ms(" [%.4d]%s",g_aBasicItem[iSerial].m_iSerial,strName));
					else	cTEXT::Put(x+1+36,y+4+2,GOLDYELLOW,_ms(" %s",strName));
				}
				else
				if (g_aBasicItem[iSerial].isExtraItem())
				{
					cTEXT::Put(x+1+36,y+4+2,GOLDYELLOW,"♠");
					if (s_bIsShowSerial)
							cTEXT::Put(x+1+36,y+4+2,LTCYAN,_ms(" [%.4d]%s",g_aBasicItem[iSerial].m_iSerial,strName));
					else	cTEXT::Put(x+1+36,y+4+2,LTCYAN,_ms(" %s",strName));
				}
				else
				{
					if (s_bIsShowSerial)
							cTEXT::Put(x+1+36,y+4+2,WHITE,_ms("[%.4d]%s",g_aBasicItem[iSerial].m_iSerial,strName));
					else	cTEXT::Put(x+1+36,y+4+2,WHITE,strName);
				}

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
					if (s_bIsShowSerial)
							cTEXT::Put(x+36,y+3+2,GOLDYELLOW,_ms(" [%.4d]%s",g_aBasicItem[iSerial].m_iSerial,strName));
					else	cTEXT::Put(x+36,y+3+2,GOLDYELLOW,_ms(" %s",strName));
				}
				else
				if (g_aBasicItem[iSerial].isExtraItem())
				{
					cTEXT::Put(x+36,y+3+2,GOLDYELLOW,"♠");
					if (s_bIsShowSerial)
							cTEXT::Put(x+36,y+3+2,LTCYAN,_ms(" [%.4d]%s",g_aBasicItem[iSerial].m_iSerial,strName));
					else	cTEXT::Put(x+36,y+3+2,LTCYAN,_ms(" %s",strName));
				}
				else
				{
					if (s_bIsShowSerial)
							cTEXT::Put(x+36	,y+3+2,WHITE,_ms("[%.4d]%s",g_aBasicItem[iSerial].m_iSerial,strName));
					else	cTEXT::Put(x+36	,y+3+2,WHITE,strName);
				}

				cTEXT::Put		(x+36	,y+3+18,LTGRAY,g_strItemCategory[g_aBasicItem[iSerial].m_wKind]);
			}

			y	+=	(dICON_SHAPE_HEIGHT+8);
		}
	}

	s_dibItemList.Draw();

	DrawItemInfo();
}

//
//	아이템 정보를 그려준다.
void
cSET_BASIC_ITEM::DrawItemInfo()
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

	s_lpSelectItem	=	&g_aBasicItem[s_iSelectItem];

	cDRAW::Fill(0);

	cTEXT::SetBoldFont();
	cTEXT::Put(x,y,LTYELLOW,"---- Item Image");
	cTEXT::SetNormalFont();
//	착용 이미지

	if (s_lpSelectItem->m_wEquippedShape	!=	0xffff)
		cTEXT::Put(x+20,y+=16,WHITE,_ms("Equipped image: %s",g_strViewEquipmentName[s_lpSelectItem->m_wKind][s_lpSelectItem->m_wEquippedShape]));

//	드랍 이미지
	cTEXT::Put(x+20,y+=16,WHITE,"Field image: ");
	cDRAW::Fill(_BLUE,x+20+14*6,y,x+20+14*6+80,y+50);
	g_sprFieldShape.FitPut(x+20+14*6,y,80,50,s_lpSelectItem->m_wFieldShape);


	y+=60;

	cTEXT::SetBoldFont();
	cTEXT::Put(x,y,LTYELLOW,"---- Item Information");
	cTEXT::SetNormalFont();

	x+=20;

	if (s_lpSelectItem->m_aValue[0][1])
	{
		if (s_lpSelectItem->m_aValue[0][0]==s_lpSelectItem->m_aValue[0][1])	
				cTEXT::Put(x,y+=16,WHITE,_ms("Created number 1: %d",s_lpSelectItem->m_aValue[0][0]));
		else	cTEXT::Put(x,y+=16,WHITE,_ms("Created number 1: %d~%d",s_lpSelectItem->m_aValue[0][0],s_lpSelectItem->m_aValue[0][1]));
	}
	if (s_lpSelectItem->m_aValue[1][1])
	{
		if (s_lpSelectItem->m_aValue[1][0]==s_lpSelectItem->m_aValue[1][1])	
				cTEXT::Put(x,y+=16,WHITE,_ms("Created value 2: %d",s_lpSelectItem->m_aValue[1][0]));
		else	cTEXT::Put(x,y+=16,WHITE,_ms("Created number 2: %d~%d",s_lpSelectItem->m_aValue[1][0],s_lpSelectItem->m_aValue[1][1]));
	}

	if (s_lpSelectItem->isDamageItem())
	{
		cTEXT::Put(x,y+=16,WHITE,_ms("Minimum damage: %d",s_lpSelectItem->m_wMinDamage));
		cTEXT::Put(x,y+=16,WHITE,_ms("Max Damage: %d",s_lpSelectItem->m_wMaxDamage));
	}

	if(s_lpSelectItem->m_wSpeed)
		cTEXT::Put(x,y+=16,WHITE,_ms(" Attack Speed: %d.%.2d",s_lpSelectItem->m_wSpeed/100,s_lpSelectItem->m_wSpeed%100));

	if(s_lpSelectItem->m_wRange)
		cTEXT::Put(x,y+=16,WHITE,_ms("Range: %d Pixel",s_lpSelectItem->m_wRange));

	if(s_lpSelectItem->m_wDamageRange)
		cTEXT::Put(x,y+=16,WHITE,_ms("Damage range: %d Pixel",s_lpSelectItem->m_wDamageRange));

	switch(s_lpSelectItem->m_wPriceComputeMethod)
	{
		case	0	:
			cTEXT::Put(x,y+=16,WHITE,_ms(" Price : %d",s_lpSelectItem->m_dwPrice));
			break;
		case	1	:
			if (s_lpSelectItem->m_aValue[0][0]==s_lpSelectItem->m_aValue[0][1])
					cTEXT::Put(x,y+=16,WHITE,_ms(" Price : %d",s_lpSelectItem->m_aValue[0][0]));
			else	cTEXT::Put(x,y+=16,WHITE,_ms(" Price: %d~%d",s_lpSelectItem->m_aValue[0][0],s_lpSelectItem->m_aValue[0][1]));
			break;
		case	2	:
			if (s_lpSelectItem->m_aValue[0][0]==s_lpSelectItem->m_aValue[0][1])
					cTEXT::Put(x,y+=16,WHITE,_ms(" Price : %d*%d",s_lpSelectItem->m_dwPrice,s_lpSelectItem->m_aValue[0][0]));
			else	cTEXT::Put(x,y+=16,WHITE,_ms(" Price : %d*[%d~%d]",s_lpSelectItem->m_dwPrice,s_lpSelectItem->m_aValue[0][0],s_lpSelectItem->m_aValue[0][1]));
			break;
		case	3	:
			if (s_lpSelectItem->m_aValue[1][0]==s_lpSelectItem->m_aValue[1][1])
					cTEXT::Put(x,y+=16,WHITE,_ms(" Price : %d*%d",s_lpSelectItem->m_dwPrice,s_lpSelectItem->m_aValue[1][0]));
			else	cTEXT::Put(x,y+=16,WHITE,_ms(" Price : %d*[%d~%d]",s_lpSelectItem->m_dwPrice,s_lpSelectItem->m_aValue[1][0],s_lpSelectItem->m_aValue[1][1]));
			break;
		case	4	:
			if (s_lpSelectItem->m_aValue[0][0]==s_lpSelectItem->m_aValue[0][1])
					cTEXT::Put(x,y+=16,WHITE,_ms(" Price : %d*(%d+1)",s_lpSelectItem->m_dwPrice,s_lpSelectItem->m_aValue[0][0]));
			else	cTEXT::Put(x,y+=16,WHITE,_ms(" Price: %d*([%d~%d]+1)",s_lpSelectItem->m_dwPrice,s_lpSelectItem->m_aValue[0][0],s_lpSelectItem->m_aValue[0][1])) ;
			break;

		case	5	:
		{
			char	strValue1[32],strValue2[32];

			if (s_lpSelectItem->m_aValue[0][0]==s_lpSelectItem->m_aValue[0][1])	sprintf(strValue1,"%d",s_lpSelectItem->m_aValue[0][0]);
			else																sprintf(strValue1,"[%d~%d]",s_lpSelectItem->m_aValue[0][0],s_lpSelectItem->m_aValue[0][1]);

			if (s_lpSelectItem->m_aValue[1][0]==s_lpSelectItem->m_aValue[1][1])	sprintf(strValue2,"%d",s_lpSelectItem->m_aValue[1][0]);
			else																sprintf(strValue2,"[%d~%d]",s_lpSelectItem->m_aValue[1][0],s_lpSelectItem->m_aValue[1][1]);

			cTEXT::Put(x,y+=16,WHITE,_ms(" Price : %s*%s/%d",strValue1,strValue2,s_lpSelectItem->m_dwPrice));
			break;
		}

		case	6	:
			cTEXT::Put(x,y+=16,WHITE,_ms(" Price: %d won for 10 pieces",s_lpSelectItem->m_dwPrice));
			break;

		case	7	:
			char	strValue1[32],strValue2[32];

			if (s_lpSelectItem->m_aValue[0][0]==s_lpSelectItem->m_aValue[0][1])	sprintf(strValue1,"%d",s_lpSelectItem->m_aValue[0][0]);
			else																sprintf(strValue1,"[%d~%d]",s_lpSelectItem->m_aValue[0][0],s_lpSelectItem->m_aValue[0][1]);

			if (s_lpSelectItem->m_aValue[1][0]==s_lpSelectItem->m_aValue[1][1])	sprintf(strValue2,"%d",s_lpSelectItem->m_aValue[1][0]);
			else																sprintf(strValue2,"[%d~%d]",s_lpSelectItem->m_aValue[1][0],s_lpSelectItem->m_aValue[1][1]);

			cTEXT::Put(x,y+=16,WHITE,_ms(" Price : %s*%s*%d",strValue1,strValue2,s_lpSelectItem->m_dwPrice));
			break;
	}

	cTEXT::Put(x,y+=16,WHITE,_ms(" Drop level: %d",s_lpSelectItem->m_wDropLevel));

	cTEXT::Put(x,y+=16,WHITE,_ms(" Stack limit: %d",s_lpSelectItem->m_wStackLimit));
	cTEXT::Put(x,y+=16,WHITE,_ms("Durability: %d",s_lpSelectItem->m_wDurability));
	cTEXT::Put(x,y+=16,WHITE, _ms("Current step: %d", s_lpSelectItem->m_bf4EternalItemCurrentLevel));		//  현재 단계 09.08.25
	cTEXT::Put(x,y+=16, WHITE, _ms(" Next level item serial: %d", s_lpSelectItem->m_wNextEternalItemSerial));// 다음단계아이템시리얼 09.08.25
	cTEXT::Put(x,y+=16, WHITE, _ms(" Enhancement panel item serial: %d", s_lpSelectItem->m_wUpgradePenaltyItemSerial));// 강화패널티아이템시리얼 09.08.26
	x-=20;

	int	iIndex	=	0;
	int	iWidth	=	s_dibItemInfo.width()- 14;

	y+=24;

	if (s_lpSelectItem->m_aGenerateData[0].m_wEffect	!=	0xffff)
	{
		cTEXT::SetBoldFont();
		cTEXT::Put(x,y,LTYELLOW,"---- Item creation information");
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
		cTEXT::Put(x,y,LTYELLOW,"---- Unique information");
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
	cTEXT::Put(x,y,LTYELLOW,"---- Professions that can wear/use items");
	cTEXT::SetNormalFont();
	x+=20;

	if (s_lpSelectItem->isEnableAllJob())
	{
		cTEXT::Put(x,y+=16,WHITE,"No job restrictions");
		y	+=	24;
	}
	else
	if (s_lpSelectItem->isExclusiveMaleJob())
	{
		cTEXT::Put(x,y+=16,WHITE,"Male characters only");
		y	+=	24;
	}
	else
	if (s_lpSelectItem->isExclusiveFemaleJob())
	{
		cTEXT::Put(x,y+=16,WHITE,"Female characters only");
		y	+=	24;
	}
	else
	{
		cSTRING		job;

		if (s_lpSelectItem->m_bf1IsMaleJobEquipAble)
			job.Add("남성/");
		if (s_lpSelectItem->m_bf1IsFemaleJobEquipAble)
			job.Add("여성/");

		for (int i=0;i<dPLAYER_JOB_COUNT;i++)
			if (s_lpSelectItem->isEquipAbleJob(i))	
				job.Add("%s/",g_aDefaultJobName[i]);

		int	iLineCount	=	cTEXT::GetLines(iWidth-x,job.String);

		cTEXT::FitPut(x,y+=16,iWidth-x,iLineCount*8*2,WHITE,job.String);

		y	+=	iLineCount*8*2;
		y	+=	10;
	}
	x-=20;

	int	iLimitCount	=	0;

	cTEXT::SetBoldFont();
	cTEXT::Put(x,y,LTYELLOW,"---- Item wearing/use restrictions");
	cTEXT::SetNormalFont();
	x+=20;

	if (s_lpSelectItem->m_wRequireAllignment	!=	0)
	{
		iLimitCount++;
		cTEXT::Put(x,y+=16,WHITE,_ms("Alignment restriction: Only %s alignment characters can wear/use", g_strAllignment[s_lpSelectItem->m_wRequireAllignment]));
	}

	if (s_lpSelectItem->m_wRequireLevel)
	{
		if(s_lpSelectItem->m_wRequireMaxLevel)
			cTEXT::Put(x,y+=16,WHITE,_ms(" Level: %d to %d", s_lpSelectItem->m_wRequireLevel,s_lpSelectItem->m_wRequireMaxLevel));// Minimum level for use/equipment
		else
			cTEXT::Put(x,y+=16,WHITE,_ms(" Level : %d",s_lpSelectItem->m_wRequireLevel));// Minimum level for use/equipment
		iLimitCount++;
	}


	if (s_lpSelectItem->m_wRequireStrength)
	{
		cTEXT::Put(x,y+=16,WHITE,_ms(" Strength : %d",s_lpSelectItem->m_wRequireStrength));// Minimum strength for use/equipment
		iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireDexterity)
	{
		cTEXT::Put(x,y+=16,WHITE,_ms(" Dexterity : %d",s_lpSelectItem->m_wRequireDexterity));// Minimum Dexterity for use/equipment
		iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireConstitution)
	{
		cTEXT::Put(x,y+=16,WHITE,_ms(" Health : %d",s_lpSelectItem->m_wRequireConstitution));// Minimum constitution for use/equipment
		iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireWisdom)
	{
		cTEXT::Put(x,y+=16,WHITE,_ms(" Wisdom : %d",s_lpSelectItem->m_wRequireWisdom));// Minimum wisdom for use/equipment
		iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireIntelligence)
	{
		cTEXT::Put(x,y+=16,WHITE,_ms(" Knowledge : %d",s_lpSelectItem->m_wRequireIntelligence));// Minimum knowledge for use/equipment
		iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireCharisma)
	{	
		cTEXT::Put(x,y+=16,WHITE,_ms("Charisma: %d",s_lpSelectItem->m_wRequireCharisma));// Minimum charisma for use/equipment
		iLimitCount++;
	}
	if (s_lpSelectItem->m_wRequireLuck)
	{	
		cTEXT::Put(x,y+=16,WHITE,_ms(" Luck : %d",s_lpSelectItem->m_wRequireLuck));// Minimum Luck for use/equipment
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
				limit.Add(" : %d 이상이어야 한다.",s_lpSelectItem->m_wEquipUseLimitValue);
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

		cTEXT::Put(x,y+=16,WHITE,limit.String);//	사용/장비를 위한 최소 운
	}

	if (iLimitCount	==	0)	cTEXT::Put(x,y+=16,WHITE,"No limit");
	// VV 09.08.11 착용제한
	x-=20;
	cTEXT::SetBoldFont();
	cTEXT::Put(x,y+=24, LTYELLOW, "---- Limit the number of items worn");
	cTEXT::SetNormalFont();
	x+=20;
	if(s_lpSelectItem->m_bf3WearLimit){
		char strWearLimit[32] = "";
		sprintf(strWearLimit,"개수 : %d", s_lpSelectItem->m_bf3WearLimit);
		cTEXT::Put(x,y+=16, WHITE, strWearLimit);	
	}else{
		cTEXT::Put(x,y+=16, WHITE, "No limit");	
	}
	// AA 09.08.11 착용제한

	x-=20;
	cTEXT::SetBoldFont();
	cTEXT::Put(x,y+=24,LTYELLOW,"---- Item Description");
	cTEXT::SetNormalFont();
	x+=20;
	cTEXT::FitPut(x,y+=16,iWidth-x,100,WHITE,s_lpSelectItem->m_pItemComment);

	s_dibItemInfo.Draw();
}

//
//	
int
cSET_BASIC_ITEM::SelectItem(int _iX,int _iY)
{
	int	x=5,y=5;

	if	(s_iLineCount	<=	0)
		return	0xffff;

	y					=	(s_iItemListHeight	-	s_iLineCountPerPage	*(dICON_SHAPE_HEIGHT+8))/2;

	int		iIndex		=	s_ScrollBar.m_iHeightPos;
	int		iCount		=	0;

	HWND	hListBox	=	GetDlgItem(s_hWnd,IDC_LIST);
	HWND	hNotSortList=	GetDlgItem(s_hWnd,IDC_NOT_SORT_LIST);

	if	(!s_isSort)
		hListBox	=	hNotSortList;

	if	(!s_rectItemList.isIN(_iX,_iY))
		return	0xffff;

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
}

//
//	일반 아이템 스크립트 문장(?)을 해석 한다.
BOOL
cSET_BASIC_ITEM::DecodeItemScriptFile(HWND _hDlg)
{
	char	strFn[512]	=	"";
	char	strFolder[512] = "";

	if (!cFILE::LoadBox(strFn, "Text file {*.txt}\0*.txt\0\0", "txt", NULL, "Load item script data file"))
	{
		SetLogFolder();
		return	FALSE;
	}

	if (cBASIC_ITEM::s_iCount > 0)
		if (cMSG::YESNO(_ms("Decode file '%s'.", strFn), "Do you want to initialize existing item data before decoding?") == IDYES)
			cBASIC_ITEM::Reset();

	if (!DecodeItemScriptFile(_hDlg,strFn))	return	FALSE;

	return	TRUE;
}

//
//화일을 분석해서 유니크 아이템으로 등록한다.
BOOL
cSET_BASIC_ITEM::DecodeItemScriptFile(HWND _hDlg,char *_strFName)
{
	cDECODER	decoder;

	if (!decoder.Upload(_strFName,NULL))	return	FALSE;

	int		iCount	=	cBASIC_ITEM::s_iCount,iErrorCount=0,iOverlappedItemCount=0,iAddCount=0;
	BOOL	isCry	=	TRUE;

	l_iFieldItemShapeCount	=	0;
	l_iIconItemShapeCount	=	0;
	l_iEquippedItemShapeCount=	0;

	while(1)
	{
		char	*strOneLine	=	decoder.GetStream(13);

		if (!strOneLine)	break;

		if (!DecodeItemScript(strOneLine,isCry))
		{	iErrorCount++;

			if (iErrorCount ==	3 )
			{
				cMSG::Put("Item data file analysis","Further errors will not be displayed.");
				isCry	=	FALSE;
			}

			continue;
		}

		if (s_makeItem.m_iSerial	==	dVALID_ITEM)	continue;

//		g_aBasicItem[iAddCount].m_wEquippedShape	=	s_makeItem.m_wEquippedShape;iAddCount++;continue;

		if (cBASIC_ITEM::Add(&s_makeItem)	==	0xffffffff)	
		{
			iOverlappedItemCount++;
			iErrorCount++;
			continue;
		}
	}

	ResetItemListBox(_hDlg,TRUE);

	DrawItemList(_hDlg);

	cMSG::Put("Item data file analysis results","%d items were added.\n\nProblems occurred with %d items.\n\n%d item names were duplicated." ,cBASIC_ITEM::s_iCount-iCount,iErrorCount,iOverlappedItemCount);

	return	TRUE;
}


//
//	스크립트 문장(?)을 해석 한다.
BOOL
cSET_BASIC_ITEM::DecodeUniqueScript(char *_strScript,BOOL _isCry)
{
	cDECODER	decoder;

	s_makeItem.reset();

	if (strlen(_strScript)	<=	80)	return	TRUE;

	decoder.UploadBuffer(_strScript,NULL);

	char	*lpStream,strName[64];
	int		iValidCount	=	0;
	int		iGrade		=	0;

	while(1)
	{	
		lpStream	=	decoder.GetStream('	');

		if (!lpStream)
		{
			iValidCount++;

			if (iValidCount	>=	20)	break;

			iGrade++;

			continue;
		}

		switch(iGrade)
		{
//			case	0	:	break;	//	인덱스 무시~
			case	1	:	//	이름
			{
				
				strcpy(strName,lpStream);
				break;
			}

//			case	2	:	break;	//	영문 이름

			case	3	:	//	기본 아이템 이름
			{
				cBASIC_ITEM	*lpItem	=	cBASIC_ITEM::Get(lpStream);

				if (!lpItem )
				{
					if (_isCry)	ERRMSG("유니크 아이템 스크립트 화일 디코딩 에러!!","'%s'의 기본이 되는 아이템 '%s'를 찾을 수 없습니다.",strName,lpStream);

					return	FALSE;
				}

				s_makeItem.copy(lpItem);
				strcpy(s_makeItem.m_strName,strName);
				break;
			}

			case	4	:	//	아이템 설명
				s_makeItem.setComment(lpStream);
				break;

			case	5	:
			case	6	:
			case	7	:
			case	8	:
			case	9	:
			case	10	:
			{
				int	iIndex	=	atoi(lpStream);

				cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.getIndexByDiscernmentCode(iIndex);

				if (!lpPrefix)
				{
					 if (_isCry)	ERRMSG("유니크 아이템 스크립트 분석 에러!!","인식번호 %d를 갖는 접두사가 없습니다.",iIndex);

					return	FALSE;
				}

				lpStream	=	decoder.GetStream('	');
				int	iValue	=	atoi(lpStream);

				s_makeItem.m_aUniqueData[iGrade-5].m_wEffect	=	lpPrefix->m_wType;
				s_makeItem.m_aUniqueData[iGrade-5].m_aValue[0]	=	iValue;

				if (strstr(lpStream,"max"))
					lpPrefix->getMaxValue(s_makeItem.m_aUniqueData[iGrade-5].m_aValue);

				if (strstr(lpStream,","))
				{
					lpStream	=	strstr(lpStream,",");
					iValue		=	atoi(lpStream+1);
					s_makeItem.m_aUniqueData[iGrade-5].m_aValue[1]	=	iValue;
				}
				else
				{
					lpStream	=	strstr(lpStream,"/");

					if (lpStream)
					{
						s_makeItem.m_aUniqueData[iGrade-5].m_aValue[0]	=	atoi(lpStream+1);
						s_makeItem.m_aUniqueData[iGrade-5].m_aValue[1]	=	iValue;
					}
				}
				break;
			}
		}

		iGrade++;
	}

	if	(!s_makeItem.isStackAble())
		s_makeItem.m_wStackLimit	=	1;

	if	(s_makeItem.m_wStackLimit	>	255)
		s_makeItem.m_wStackLimit	=	255;

	return	TRUE;
}

//
//
BOOL
cSET_BASIC_ITEM::DecodeItemScript(char *_strScript,BOOL _isCry)
{
	cDECODER	decoder;

	s_makeItem.reset();

	if (strlen(_strScript)	<=	80)	return	TRUE;

	decoder.UploadBuffer(_strScript,NULL);

	char	*lpStream;
	int		iValidCount	=	0;
	int		iGrade		=	0;
	int		iMinValue1=0,iMinValue2=0,iMaxDamage=0;

	static	int	Count=	0;

	Count++;
	if (Count==102)
	{
		Count++;
	}

	while(1)
	{	
		lpStream	=	decoder.GetStream('	');

		if (!lpStream)
		{
			iValidCount++;

			if (iValidCount	>=	20)	break;

			iGrade++;

			continue;
		}

		switch(iGrade)
		{
			case	1	:	//	분류
			{
				int	iKind	=	cBASIC_ITEM::GetItemKind(lpStream);

				if (iKind	==	0xffff)
				{
					if (_isCry)	cMSG::Error("아이템 정보 분석 에러!!","다음 아이템 분류를 찾을 수 없습니다.\n\n'%s'",lpStream);

					return	FALSE;
				}

				s_makeItem.m_wKind	=	iKind;
				break;
			}

			case	2	:	//	이름
			{
				strcpy(s_makeItem.m_strName,lpStream);
				break;
			}
			
			case	4	:	//	아이템 설명
				s_makeItem.setComment(lpStream);
				break;

			case	5	:	//	최소 데미지
			{
				int	iValue	=	atoi(lpStream);
				s_makeItem.m_wMinDamage	=	iValue;
				break;
			}

			case	6	:	//	최고 데미지
			{
				int	iValue	=	atoi(lpStream);

				iMaxDamage	=	iValue;
				s_makeItem.m_wMaxDamage	=	iValue;
				break;
			}

			case	7	:	//	수치1 최소값
			{
				int	iValue	=	atoi(lpStream);

				iMinValue1	=	iValue;
				s_makeItem.m_aValue[0][0]	=	iValue;
				break;
			}

			case	8	:	//	수치1 최대값
			{
				int	iValue	=	atoi(lpStream);

				if(iValue	==	0)	iValue	=	iMinValue1;

				s_makeItem.m_aValue[0][1]	=	iValue;
				break;
			}

			case	9	:	//	수치2 최소값
			{
				int	iValue	=	atoi(lpStream);

				iMinValue2	=	iValue;
				s_makeItem.m_aValue[1][0]	=	iValue;
				break;
			}

			case	10	:	//	수치2 최대값
			{
				int	iValue	=	atoi(lpStream);

				if(iValue	==	0)	iValue	=	iMinValue2;

				s_makeItem.m_aValue[1][1]	=	iValue;
				break;
			}

			case	11	:	//	공격 속도
			{
				s_makeItem.m_wSpeed	=	StringToNumber(lpStream,2,2);
				break;
			}

			case	12	:	//	사거리
			{
				s_makeItem.m_wRange	=	atoi(lpStream);
				break;
			}

			case	13	:	//	데미지 범위
			{
				s_makeItem.m_wDamageRange	=	atoi(lpStream);
				break;
			}

			case	16	:	//	가격
			{
				int	iValue	=	0;

				if (strstr(lpStream,"[수치1]*[수치2]/"))
				{
					lpStream	=	strstr(lpStream,"/");
					iValue		=	atoi(lpStream+1);

					s_makeItem.m_dwPrice				=	iValue;
					s_makeItem.m_wPriceComputeMethod	=	5;
					break;
				}

				if (strstr(lpStream,"[수치1]+1"))
				{
					iValue		=	atoi(lpStream);

					s_makeItem.m_dwPrice				=	iValue;
					s_makeItem.m_wPriceComputeMethod	=	4;
					break;
				}

				if (strstr(lpStream,"[수치2]*"))
				{
					lpStream	=	strstr(lpStream,"*");
					iValue		=	atoi(lpStream+1);

					s_makeItem.m_dwPrice				=	iValue;
					s_makeItem.m_wPriceComputeMethod	=	3;
					break;
				}

				if (strstr(lpStream,"[수치1]*"))
				{
					lpStream	=	strstr(lpStream,"*");
					iValue		=	atoi(lpStream+1);

					s_makeItem.m_dwPrice				=	iValue;
					s_makeItem.m_wPriceComputeMethod	=	2;
					break;
				}

				if (strstr(lpStream,"[수치1]"))
				{
					s_makeItem.m_wPriceComputeMethod	=	1;
					break;
				}

				if (strstr(lpStream,"/10개"))
				{
					iValue		=	atoi(lpStream);

					s_makeItem.m_dwPrice				=	iValue;
					s_makeItem.m_wPriceComputeMethod	=	6;
					break;
				}

				iValue		=	atoi(lpStream);

				s_makeItem.m_dwPrice				=	iValue;
				s_makeItem.m_wPriceComputeMethod	=	0;
				break;
			}

			case	17	:
			{
				if (stricmp(lpStream,"제작")==0)
				{
					s_makeItem.m_wFieldShape	=	l_iFieldItemShapeCount;
					l_iFieldItemShapeCount++;
				}
				else
				{
					int	iIndex	=	atoi(lpStream);
					s_makeItem.m_wFieldShape	=	g_aBasicItem[iIndex-1].m_wFieldShape;
				}
				break;//	필드
			}

			case	18	:
			{
				if (stricmp(lpStream,"-")==0)	break;

				if (stricmp(lpStream,"제작")==0)
				{
					int	i	=	0;

					while(1)
					{
						if	(stricmp(g_strViewEquipmentName[s_makeItem.m_wKind][i],"end")==0)	break;
						if	(stricmp(g_strViewEquipmentName[s_makeItem.m_wKind][i],s_makeItem.m_strName)==0)
						{
							s_makeItem.m_wEquippedShape	=	i;
							break;
						}

						i++;
					}
				}
				else
				{
					int	iIndex	=	atoi(lpStream);
					s_makeItem.m_wEquippedShape	=	g_aBasicItem[iIndex-1].m_wEquippedShape;
				}
				break;//	장비
			}

			case	19	:
			{
				if (stricmp(lpStream,"제작")==0)
				{
					s_makeItem.m_wIconShape	=	l_iIconItemShapeCount;
					l_iIconItemShapeCount++;
				}
				else
				if (stricmp(lpStream,"제작(7)")==0)
				{
					s_makeItem.m_wIconShape	=	l_iIconItemShapeCount;
					l_iIconItemShapeCount+=7;
				}
				else
				{
					int	iIndex	=	atoi(lpStream);
					s_makeItem.m_wIconShape	=	g_aBasicItem[iIndex-1].m_wIconShape;
				}
				break;//	아이콘
			}
			case	20	:	break;//	비고

			case	21	:	//	드랍레벨
			{
				s_makeItem.m_wDropLevel	=	atoi(lpStream);
				break;
			}
		}

		iGrade++;
	}

	s_makeItem.m_iSerial	=	TRUE;

	if	(!s_makeItem.isStackAble())
		s_makeItem.m_wStackLimit	=	1;

	return	TRUE;
}

//
//
LRESULT CALLBACK 
cSET_BASIC_ITEM::s_procSetBasicItemList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)		// 09.08.06 휠 추가 !
{	switch(message)
	{	case WM_INITDIALOG	:
			InitItemListBox(hDlg);
			break;
		case WM_MOUSEWHEEL:
			{
				int zDelta = ((short)HIWORD(wParam));
				HWND	hItemListWnd			=	GetDlgItem(hDlg,IDC_BASIC_ITEM_LIST);

				if	(zDelta>=0)
				{
					SendMessage(hItemListWnd, WM_VSCROLL,SB_LINEUP,0);
				}
				else
				{
					SendMessage(hItemListWnd, WM_VSCROLL,SB_LINEDOWN,0);
				}				
				
				break;
			}

		case WM_CLOSE		:
			EndDialog(hDlg,FALSE);
			break;

		case WM_COMMAND :
		{
			switch( LOWORD( wParam ) )
            {
				case	IDC_PREVIOUS_ITEM	:
				{
					HWND	hList	=	GetDlgItem(hDlg,IDC_LIST);
					if (!s_isSort)	hList=	GetDlgItem(hDlg,IDC_NOT_SORT_LIST);

					int	iIndex	=	cLISTBOX::GetIndexByData(hList,s_iSelectItem);
					int	iData	=	cLISTBOX::GetData(hList,iIndex-1);
					if (iData	==	LB_ERR)	break;

					s_ScrollBar.up();
					s_iSelectItem	=	iData;
					DrawItemList(hDlg);
					break;
				}

				case	IDC_LOAD_ITEM_DATA	:
				{
					break;
				}

				case	IDC_NEXT_ITEM	:
				{
					HWND	hList	=	GetDlgItem(hDlg,IDC_LIST);
					if (!s_isSort)	hList=	GetDlgItem(hDlg,IDC_NOT_SORT_LIST);

					int	iIndex	=	cLISTBOX::GetIndexByData(hList,s_iSelectItem);
					int	iData	=	cLISTBOX::GetData(hList,iIndex+1);

					if	(iData	==	LB_ERR)
						break;

					s_ScrollBar.down();
					s_iSelectItem	=	iData;
					DrawItemList(hDlg);
					break;
				}

				case	IDOK				:
					EndDialog(hDlg,TRUE);
					break;

				case	IDCANCEL			:
					EndDialog(hDlg,FALSE);
					break;

				case	IDC_SORT			:
				{
					s_isSort	=	IsDlgButtonChecked(hDlg,IDC_SORT);
					char strFilter[256] = "";
					GetDlgItemText(hDlg,IDC_SEARCH_ITEM,strFilter,256);
					
					if(strFilter != NULL)
					{
						SetDlgItemText(hDlg,IDC_SEARCH_ITEM,strFilter);
						
					}
					else
					{
						ResetItemListBox(hDlg,TRUE);
						s_scrollItemInfo.SetHeightPos(0);
						ResetItemListBox(hDlg);
						PlaceItemToViewAble(hDlg,s_iSelectItem);
						DrawItemList(hDlg);
						
					}	
					
					break;
				}

				case	IDC_SHOW_SERIAL		:
				{
					s_bIsShowSerial	=	IsDlgButtonChecked(hDlg,IDC_SHOW_SERIAL);
					char strFilter[256] = "";
					GetDlgItemText(hDlg,IDC_SEARCH_ITEM,strFilter,256);
					
					if(strFilter != NULL)
					{
						SetDlgItemText(hDlg,IDC_SEARCH_ITEM,strFilter);
						
					}
					else
					{
						ResetItemListBox(hDlg,TRUE);
						s_scrollItemInfo.SetHeightPos(0);
						ResetItemListBox(hDlg);
						PlaceItemToViewAble(hDlg,s_iSelectItem);
						DrawItemList(hDlg);
						
					}					
					break;
				}

				case	IDC_LOAD_UNIQUE_SCRIPT		:
					DecodeUniqueScriptFile(hDlg);
					break;

				case	IDC_LOAD_ITEM_SCRIPT		:
					DecodeItemScriptFile(hDlg);
					break;

				case	IDC_COPY			:
				{
					if (!cBASIC_ITEM::IsExist(s_iSelectItem))	break;

					s_clipItem.copy(&g_aBasicItem[s_iSelectItem]);
					EnableWindow(GetDlgItem(hDlg,IDC_PASTE),TRUE);
					break;
				}

				case	IDC_PASTE			:
				{
					cBASIC_ITEM		tempItem;

					tempItem.copy(&s_clipItem);

					for (int i=2;i<1000;i++)
					{
						if(!cBASIC_ITEM::Get(_ms("%s[%d]",s_clipItem.m_strName,i)))
						{
							strcpy(tempItem.m_strName,_ms("%s[%d]",s_clipItem.m_strName,i));

							break;
						}
					}

					if (i	==	1000)	break;

					int	iSerial	=	cBASIC_ITEM::Add(&tempItem);

					if (iSerial	!=	0xffffffff)
					{
						s_iSelectItem	=	iSerial;

						if (!s_isSort)	cLISTBOX::Add(GetDlgItem(hDlg,IDC_NOT_SORT_LIST),tempItem.m_strName,iSerial);
						else			cLISTBOX::Add(GetDlgItem(hDlg,IDC_LIST),tempItem.m_strName,iSerial);
						ResetItemListBox(hDlg);
						PlaceItemToViewAble(hDlg,s_iSelectItem);
						DrawItemList(hDlg);
					}
					break;
				}

				case	IDC_ADD				:					//	아이템 추가
				{
					if (s_iSelectKind	==	dITEM_KIND_ALL)
					{
						cMSG::Error(dMSG_BASE_ITEM_ADD_ERROR,dMSG_SELECT_ITEM_KIND);

						break;
					}

					s_iEditItemKind	=	s_iSelectKind;

					if (!Add(hDlg))	break;

					int	iSerial	=	cBASIC_ITEM::Add(&s_makeItem);

					if (iSerial	==	0xffffffff)	break;

					if (s_iEditItemKind	!=	s_iSelectKind)
					{
						s_iSelectKind	=	s_iEditItemKind;
						ResetItemListBox(hDlg,TRUE);
					}
					else
					{
						if (!s_isSort)	cLISTBOX::Add(GetDlgItem(hDlg,IDC_NOT_SORT_LIST),s_makeItem.m_strName,iSerial);
						else			cLISTBOX::Add(GetDlgItem(hDlg,IDC_LIST),s_makeItem.m_strName,iSerial);
					}

					s_scrollItemInfo.SetHeightPos(0);
					s_iSelectItem	=	iSerial;
					ResetItemListBox(hDlg);
					PlaceItemToViewAble(hDlg,s_iSelectItem);
					DrawItemList(hDlg);

					cPROJECT::SaveItem();
					break;
				}

				case	IDC_EDIT			:					//	아이템 정보 수정
				{
					if (!cBASIC_ITEM::IsExist(s_iSelectItem))	break;

					s_iEditItemKind	=	g_aBasicItem[s_iSelectItem].m_wKind;

					if (Add(hDlg,s_iSelectItem))
					{
						s_scrollItemInfo.SetHeightPos(0);

						g_aBasicItem[s_iSelectItem].copy(&s_makeItem);

						ResetItemListBox(hDlg,TRUE);
						PlaceItemToViewAble(hDlg,s_iSelectItem);
						DrawItemList(hDlg);

						cPROJECT::SaveItem();
					}

					break;
				}


				case	IDC_REMOVE			:
				{
					if (!cBASIC_ITEM::IsExist(s_iSelectItem))	break;

					if (cMSG::YESNO(dMSG_REALLY_REMOVE_FOLLOW_ITEM,g_aBasicItem[s_iSelectItem].m_strName)	==	IDNO)
						break;

					HWND	hList		=	GetDlgItem(hDlg,IDC_LIST);
					HWND	hNotSortList=	GetDlgItem(hDlg,IDC_NOT_SORT_LIST);

					if (!s_isSort)	hList=	hNotSortList;

					int	iIndex	=	cLISTBOX::GetIndexByData(hList,s_iSelectItem);

					cBASIC_ITEM::Remove(s_iSelectItem);
					cLISTBOX::RemoveByData(hList,s_iSelectItem);

					iIndex	=	cLISTBOX::GetData(hList,iIndex);

					if (iIndex	!=	LB_ERR)	s_iSelectItem	=	iIndex;

					ResetItemListBox(hDlg);
					PlaceItemToViewAble(hDlg,s_iSelectItem);
					DrawItemList(hDlg);

					break;
				}

				case	IDC_ITEM_CATEGORY	:		//09.07.30 수정
				{
					if	(HIWORD(wParam)	!=	CBN_SELCHANGE)
						break;

					int	iKind		=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_ITEM_CATEGORY));

					if	(s_iSelectKind	==	iKind)
						break;

					char strFilter[256] = "";
					GetDlgItemText(hDlg,IDC_SEARCH_ITEM,strFilter,256);
					s_iSelectKind	=	iKind;

					if	(strFilter != NULL)
					{
						SetDlgItemText(hDlg,IDC_SEARCH_ITEM,strFilter);
					}
					else
					{
						ResetItemListBox(hDlg,TRUE);
						DrawItemList(hDlg);
						PlaceItemToViewAble(hDlg,s_iSelectItem);
						DrawItemList(hDlg);	
						
					}							

					break;
				}

				case IDC_SEARCH_ITEM	:		//09.07.30 추가
				{
					if	(HIWORD(wParam) != EN_CHANGE)
						break;

					char strFilter[256] = "";
					GetDlgItemText(hDlg,IDC_SEARCH_ITEM,strFilter,256);
					int iFilterNameLen	= strlen(strFilter);

					HWND	hItemList		=	GetDlgItem(hDlg,IDC_LIST);
					HWND	hNotSortList	=	GetDlgItem(hDlg,IDC_NOT_SORT_LIST);

					if	(!s_isSort)	
						hItemList	=	hNotSortList;

					cLISTBOX::Reset(hItemList);	

					if	(iFilterNameLen<=0)
					{
						s_iSelectKind	= cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_ITEM_CATEGORY));
						ResetItemListBox(hDlg,TRUE);
						PlaceItemToViewAble(hDlg,s_iSelectItem);	

						DrawItemList(hDlg);

						break;
					}

					int iCount = 0;
					int iIndex = 0;

					while(1)
					{
						cBASIC_ITEM	*lpItem	=	cBASIC_ITEM::GetItemByKind(s_iSelectKind,&iIndex);
						
						if(!lpItem)	break;

						int iItemNameLen	= strlen(lpItem->m_strName);
						int iLenGap			= iItemNameLen - iFilterNameLen;
						
						if(iLenGap<0)
							continue;
						
						for(int iName=0;iName<=iLenGap;++iName)
						{
							if(strnicmp(strFilter,&lpItem->m_strName[iName], iFilterNameLen) == 0 )
							{
								cLISTBOX::Add(hItemList,lpItem->m_strName,lpItem->m_iSerial);
								++iCount;
								break;
							}
						}			
						
					}
				
					s_iLineCount			= iCount;							
					s_ScrollBar.SetHeight(s_iLineCount,s_iLineCountPerPage);
					s_ScrollBar.SetHeightPos(0);	
					DrawItemList(hDlg);

					break;
				}

			}

			break;
		}
	}

	return (0L);
}


//
//	아이템 리스트 윈도우 dib 프로콜
LRESULT CALLBACK
cSET_BASIC_ITEM::s_procItemListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibItemList.Active();
			s_dibItemList.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_LBUTTONDOWN	:
		{
			int item		=	SelectItem(LOWORD(lParam),HIWORD(lParam));

			if(item			==	0xffff		)	break;
			if(item			==	s_iSelectItem)	break;
			

			s_iSelectItem	=	item;

			s_scrollItemInfo.SetHeightPos(0);
			SetFocus(s_hWnd);

			DrawItemList(s_hWnd);

			break;
		}

		case WM_LBUTTONDBLCLK	:
		{
			if (!cBASIC_ITEM::IsExist(s_iSelectItem))	break;

			s_iEditItemKind	=	g_aBasicItem[s_iSelectItem].m_wKind;

			if (Add(s_hWnd,s_iSelectItem))
			{
				s_scrollItemInfo.SetHeightPos(0);

				g_aBasicItem[s_iSelectItem].copy(&s_makeItem);

				ResetItemListBox(s_hWnd,TRUE);
				PlaceItemToViewAble(s_hWnd,s_iSelectItem);
				DrawItemList(s_hWnd);

				cPROJECT::SaveItem();
			}

			break;
		}

//		case WM_MOUSEWHEEL		:
//			cMSG::Test();
//			break;

		case WM_VSCROLL			:
		{
			if (s_ScrollBar.VScroll(wParam))	DrawItemList(s_hWnd);
			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}


//
//	아이템 정보 윈
LRESULT CALLBACK
cSET_BASIC_ITEM::s_procItemInfoWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibItemInfo.Active();
			s_dibItemInfo.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_VSCROLL			:
		{
			if (s_scrollItemInfo.VScroll(wParam))	DrawItemInfo();
			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}


///////////////////////////////////////////////////////////////////////////////////////
//	아이템 추가
///////////////////////////////////////////////////////////////////////////////////////


cDIBWND		cSET_BASIC_ITEM::s_dibItemGenerateData;		//	아이템 효과 dib
cDIBWND		cSET_BASIC_ITEM::s_dibUniqueItemData;
cDIBWND		cSET_BASIC_ITEM::s_dibIconShape;		//	아이템 모양 dib
cDIBWND		cSET_BASIC_ITEM::s_dibFieldShape;
cDIBWND		cSET_BASIC_ITEM::s_dibEquippedShape;

cBASIC_ITEM	cSET_BASIC_ITEM::s_makeItem;
BOOL		cSET_BASIC_ITEM::s_bEditItem		=	FALSE;
int			cSET_BASIC_ITEM::s_iSelectGerateData=	0;
int			cSET_BASIC_ITEM::s_iSelectUniqueData=	0;
int			cSET_BASIC_ITEM::s_iEditItemKind	=	0;

//
//	아이템을 추가한다.
BOOL
cSET_BASIC_ITEM::Add(HWND hwnd,int _iEditItem)
{
	s_bEditItem	=	FALSE;

	if (_iEditItem	!=	0xffff)
	{	s_makeItem.copy(_iEditItem);
		s_bEditItem	=	TRUE;
	}

	int	result	=	DialogBox( MAIN.hINST,"SET_ITEM_DATA",hwnd,(DLGPROC)s_procAddBasicItem);

	return	result;
}	//	cSET_BASIC_ITEM::Add(HWND hwnd)

//
//	아이템을 추가 박스 초기화.
//
void
cSET_BASIC_ITEM::InitItemAddBox(HWND hwnd)			// 09.08.07
{

	int		iIndex					=	0;

	s_isInitializing				=	TRUE;
	s_iSelectGerateData				=	0;	//	선택된 아이템 효과
	s_iSelectUniqueData				=	0;

	SetWindowText(hwnd,_ms("%s : %s",dMSG_KIND,g_strItemCategory[s_iEditItemKind]));

	HWND	hItemGenerateDataWnd	=	GetDlgItem(hwnd,IDC_ITEM_GENERATE_DATA);
	HWND	hItemUniqueDataWnd		=	GetDlgItem(hwnd,IDC_UNIQUE_DATA);
	HWND	hItemShapeWnd			=	GetDlgItem(hwnd,IDC_ITEM_SHAPE);
	HWND	hItemEquippedShape		=	GetDlgItem(hwnd,IDC_ITEM_EQUIPPED_SHAPE);
	HWND	hItemFieldShape			=	GetDlgItem(hwnd,IDC_ITEM_FIELD_SHAPE);

	if (!s_bEditItem)
		s_makeItem.reset();

	s_dibItemGenerateData.lpszRegister	=	"Red Stone Basic Item Generate Data DibWnd";
	s_dibUniqueItemData.lpszRegister	=	"Red Stone Basic Item Unique Data DibWnd";
	s_dibIconShape.lpszRegister			=	"Red Stone Basic Item Shape DibWnd";
	s_dibFieldShape.lpszRegister		=	"Red Stone Basic Item Field Shape DibWnd";
	s_dibEquippedShape.lpszRegister		=	"Red Stone Basic Item Equipped Shape DibWnd";

	s_dibItemGenerateData.Init	(hItemGenerateDataWnd,(WNDPROC)s_procItemGerateDataWnd);
	s_dibUniqueItemData.Init	(hItemUniqueDataWnd	,(WNDPROC)s_procUniqueItemDataWnd);
	s_dibIconShape.Init			(hItemShapeWnd		,(WNDPROC)s_procItemShapeWnd);
	s_dibFieldShape.Init		(hItemFieldShape	,(WNDPROC)s_procFieldShapeWnd);
	s_dibEquippedShape.Init		(hItemEquippedShape	,(WNDPROC)s_procEquippedShapeWnd);

	{
		HWND	hComboExGrade	=	GetDlgItem(hwnd,IDC_EX_GRADE);
		char	*strExtraGrade[]=	{"10등급","9등급","8등급","7등급","6등급","5등급","4등급","3등급","2등급","1등급","특등급","절대급","무한급","end"};

		cCOMBOBOX::Add(hComboExGrade,"등급 없음",0xffff);

		for (int i=0;;i++)
		{
			if (stricmp(strExtraGrade[i],"end")==0)
				break;

			cCOMBOBOX::Add(hComboExGrade,strExtraGrade[i],i);
		}

		cCOMBOBOX::SelectByData(hComboExGrade,s_makeItem.m_wExtraGrade);
	}

	//	사용/착용 가능 성향
	HWND	hComboAllignment		=	GetDlgItem(hwnd,IDC_ALLIGNMENT);

	for (iIndex=0;iIndex<6;iIndex++)
		cCOMBOBOX::Add(hComboAllignment,g_strAllignment[iIndex],iIndex);

//	아이템 분류
	{
		HWND	hComboBaseItem		=	GetDlgItem(hwnd,IDC_BASE_ITEM);

		int		iIndex	=	0;

		cCOMBOBOX::Add(hComboBaseItem,"not",0xffff);

		for (iIndex=0;iIndex<dBASIC_ITEM_COUNT;iIndex++)
		{
			if (g_aBasicItem[iIndex].m_iSerial	==	0xfffffff)
				continue;

			if (g_aBasicItem[iIndex].m_wKind	!=	s_iEditItemKind)
				continue;

			cCOMBOBOX::Add(hComboBaseItem,g_aBasicItem[iIndex].m_strName,iIndex);
		}

		cCOMBOBOX::SelectByData(hComboBaseItem,s_makeItem.m_wBaseItem);
	}

//	퀘스트
	{
		HWND	hComboQuest			=	GetDlgItem(hwnd,IDC_QUEST);

		int		iIndex	=	0;

		cCOMBOBOX::Add(hComboQuest," ",dNOT_QUEST_ITEM);
		for (iIndex=0;;iIndex++)
		{
			CQuestItem*	lpQuest	=	g_quest.getQuest(iIndex);

			if (!lpQuest)	break;
			if (lpQuest->m_wQuestType == eQT_DEATH)	continue;

			cCOMBOBOX::Add(hComboQuest,_ms("N] %s",lpQuest->m_strName),iIndex);
		}

		for (iIndex=0;;iIndex++)
		{
			CQuestItem*	lpQuest	=	g_questArbeit.getQuest(iIndex);

			if (!lpQuest)	break;
			if (lpQuest->m_wQuestType == eQT_DEATH)	continue;

			cCOMBOBOX::Add(hComboQuest,_ms("A] %s",lpQuest->m_strName),iIndex+dQUEST_ITEM_KIND_BORDER);
		}
	}

//	장비 등급
	{
		HWND	hComboEquipGrade	=	GetDlgItem(hwnd,IDC_EQUIP_GRADE);

		iIndex	=	0;

		cCOMBOBOX::Add(hComboEquipGrade," ",0xffff);

		while(1)
		{
			if (stricmp(g_strViewEquipmentName[s_iEditItemKind][iIndex],"end")==0)	break;
			cCOMBOBOX::Add(hComboEquipGrade,g_strViewEquipmentName[s_iEditItemKind][iIndex],iIndex);

			iIndex++;
		}
	}

//	팔레트 리스트
	{
		HWND	hComboPaletteList	=	GetDlgItem(hwnd,IDC_PALETTE_LIST);

		iIndex	=	0;

		for (int i=1;i<=5;i++)
			cCOMBOBOX::Add(hComboPaletteList,_ms("Palette #%.2d",i),i-1);

		cCOMBOBOX::SelectByData(hComboPaletteList,0);
	}
	
	HWND	hComboRequirePowerContents	=	GetDlgItem(hwnd,IDC_EQUIP_GRADE);

//	가격 계산식
	{
		HWND	hComboPriceComputeMethod	=	GetDlgItem(hwnd,IDC_PRICE_COMPUTE_METHOD);

		for (iIndex =0;;iIndex++)
		{
			if (stricmp(g_strPriceComputeMethod[iIndex],"end")==0)	break;
			cCOMBOBOX::Add(hComboPriceComputeMethod,g_strPriceComputeMethod[iIndex],iIndex);
		}
	}

//	프리미엄 레벨
	{
		HWND	hComboPremiumServiceLevel	=	GetDlgItem(hwnd,IDC_PREMIUM_LEVEL);

		for (iIndex=0;;iIndex++)
		{
			if (stricmp(g_strPremiumServiceLevel[iIndex],"end")==0)	break;

			cCOMBOBOX::Add(hComboPremiumServiceLevel,g_strPremiumServiceLevel[iIndex],iIndex);
		}
	}

	{
		HWND	hComboOutputEffect	=	GetDlgItem(hwnd,IDC_COMPULSION_EFFECT);

		cCOMBOBOX::Add(hComboOutputEffect,"효과 없음",0);
		
		for (iIndex=0;;iIndex++)
		{
			if (stricmp(g_strPrefixOutputEffect[iIndex],"end")==0)	break;
			
			cCOMBOBOX::Add(hComboOutputEffect,g_strPrefixOutputEffect[iIndex],iIndex);
		}
	}

	{
		HWND	hComboItemEffect	=	GetDlgItem(hwnd,IDC_ITEM_EFFECT);
		
		cCOMBOBOX::Add(hComboItemEffect,"효과 없음",0);
		//	0 이 디폴트.. 효과없음...
		//	
		for (int i = 1;i <= g_em.m_iEffectCount;++i)
		{
			// 이팩트 이미지는 .. 0부터 시작해야하기때문에 .. i-1 했음..
			cCOMBOBOX::Add(hComboItemEffect,g_em.m_aInfo[i-1].m_strImageFileName,i);
		}
	}

//	프리미엄 레벨
	{
		HWND	hComboPremiumServiceLevel	=	GetDlgItem(hwnd,IDC_PREMIUM_GRADE);

		for (iIndex=0;;iIndex++)
		{
			if	(stricmp(g_strPremiumServiceLevel[iIndex],"end")==0)
				break;

			cCOMBOBOX::Add(hComboPremiumServiceLevel,g_strPremiumServiceLevel[iIndex],iIndex);
		}
	}

//	착용/사용 제한
	{
		HWND	hComboEquipUseLimitContents	=	GetDlgItem(hwnd,IDC_EQUIP_USE_LIMIT_CONTENTS);
		HWND	hComboEquipUseLimitMethod	=	GetDlgItem(hwnd,IDC_EQUIP_USE_LIMIT_COMPUTE_METHOD);

		cCOMBOBOX::Add(hComboEquipUseLimitMethod," ",0xffff);
		for (iIndex=0;;iIndex++)
		{
			if (stricmp(g_strEquipUseLimitMethod[iIndex],"end")==0)	break;
			cCOMBOBOX::Add(hComboEquipUseLimitMethod,g_strEquipUseLimitMethod[iIndex],iIndex);
		}

		cCOMBOBOX::Add(hComboEquipUseLimitContents," ",0xffff);
		for (iIndex=0;;iIndex++)
		{
			if (stricmp(g_strEquipUseLimitContents[iIndex],"end")==0)	break;
			cCOMBOBOX::Add(hComboEquipUseLimitContents,g_strEquipUseLimitContents[iIndex],iIndex);
		}
	}

	// VV 09.08.11 착용제한
	HWND hComboWearLimit	=	GetDlgItem(hwnd, IDC_WEAR_LIMIT_COUNT);		
	for(int i=0;i<8;++i){	
		char strInt[2] = "";
		strInt[0]	= 48 + i;
		cCOMBOBOX::Add(hComboWearLimit, strInt,i);
	}
	cCOMBOBOX::Select(hComboWearLimit, s_makeItem.m_bf3WearLimit);
	// AA 09.08.11 착용제한
	// AA 09.08.25 현재 단계
	HWND hComboEternalCurrentLevel	=	GetDlgItem(hwnd, IDC_ETERNAL_ITEM_CURRENT_LEVEL);		
	for(int j=0;j<=10;++j){	
		cCOMBOBOX::Add(hComboEternalCurrentLevel,  _ms("%d",j),j);
	}
	cCOMBOBOX::Select(hComboEternalCurrentLevel, s_makeItem.m_bf4EternalItemCurrentLevel);
	// AA 09.08.25 현재 단계

	HWND hComboItemRebirthCount	=	GetDlgItem(hwnd, IDC_ITEM_RIBIRTH_COUNT);		
	for(int k=0;k<=5;++k){	
		cCOMBOBOX::Add(hComboItemRebirthCount,  _ms("%d",k),k);
	}
	cCOMBOBOX::Select(hComboItemRebirthCount, s_makeItem.m_bf4ItemRebirthCount);

//	내용물 채우기
	FillItemAddBox(hwnd);
	DrawAddItemWnd(hwnd);
	SetCheckDataList(hwnd);
	s_isInitializing	=	FALSE;
}	//	cSET_BASIC_ITEM::InitItemAddBox(HWND hwnd)


//
//	아이템을 추가 박스 내용 채우기
void
cSET_BASIC_ITEM::FillItemAddBox(HWND hwnd)
{
	if (s_bEditItem)
	{
		SetDlgItemText(hwnd,IDC_NAME	,s_makeItem.m_strName		);
		SetDlgItemText(hwnd,IDC_COMMENT	,s_makeItem.m_pItemComment	);
	}

//	사용/착용 가능 직업
	{
		for (int iJob=0;;iJob++)
		{
			int	iID	=	g_aiEnableJobIDCList[iJob];

			if	(iID	==	-1)
				break;

			if	(s_makeItem.isEquipAbleJob(iJob))
				CheckDlgButton(hwnd,iID,TRUE);
			else
				CheckDlgButton(hwnd,iID,FALSE);
		}

		if	(s_makeItem.m_bf1IsMaleJobEquipAble)
			CheckDlgButton(hwnd,IDC_MALE,TRUE);
		if	(s_makeItem.m_bf1IsFemaleJobEquipAble)
			CheckDlgButton(hwnd,IDC_FEMALE,TRUE);
	}

	CheckDlgButton(hwnd,IDC_CAN_NOT_DROPPING,s_makeItem.m_attr.isCanNotDropItem);
	CheckDlgButton(hwnd,IDC_CAN_NOT_TRADE,s_makeItem.m_attr.isCanNotTradeItem);
	CheckDlgButton(hwnd,IDC_DESTROY_ENCHANT_FAILED_RING,s_makeItem.m_attr.isDestroyRingAtFailedEnchant);
	CheckDlgButton(hwnd,IDC_UPGRADE_PREFIX_LEVEL,s_makeItem.m_attr.isUpgradePrefixLevel);
	
	CheckDlgButton(hwnd,IDC_USE_TO_ITEM,s_makeItem.m_attr.isUseToItem);
	CheckDlgButton(hwnd,IDC_EXCLUSIVE_TO_NORMAL_ITEM,s_makeItem.m_attr.isExclusiveToNormalItem);
	CheckDlgButton(hwnd,IDC_EXCLUSIVE_TO_UNIQUE_ITEM,s_makeItem.m_attr.isExclusiveToUniqueItem);
	CheckDlgButton(hwnd,IDC_BLOCK_TO_ENCHANT,s_makeItem.m_bf1IsBlockToEnchant);

	CheckDlgButton(hwnd,IDC_DURABILITY_TO_USE_COUNT,s_makeItem.m_attr.isDurabilityToUseCount);
	CheckDlgButton(hwnd,IDC_USE_TO_CHARACTER,s_makeItem.m_attr.isUseToCharacter);
	CheckDlgButton(hwnd,IDC_CAN_NOT_SELL,s_makeItem.m_attr.isCanNotSelllItem);
	CheckDlgButton(hwnd,IDC_ID_PUBLIC_ITEM,s_makeItem.m_wIsIDPublicItem);
	CheckDlgButton(hwnd,IDC_EXPIRED_BY_MOVE_FIELD,s_makeItem.m_bf1IsDestroyWhenMoveField);
	CheckDlgButton(hwnd,IDC_USE_TO_CORPSE,s_makeItem.m_attr.isUseToCorpse);
	CheckDlgButton(hwnd,IDC_CAN_USE_CORPSE_STATUS,s_makeItem.m_attr.isCanUseCorpseStatus);
	CheckDlgButton(hwnd,IDC_IS_BADGE,s_makeItem.m_attr.isBadge);
	CheckDlgButton(hwnd,IDC_IS_EFFECT_IN_INVENTORY,s_makeItem.m_attr.isEffectInInventoryItem);
	CheckDlgButton(hwnd,IDC_IGNORE_SAME_ITEM,s_makeItem.m_attr.isCanNotOwnSameItem);
	CheckDlgButton(hwnd,IDC_GUILD_BATTLE_ITEM,s_makeItem.m_attr.isGuildBattleItem);
	CheckDlgButton(hwnd,IDC_REMAIN_USE_LOG,s_makeItem.m_attr.isRequireSaveUseLog);
	CheckDlgButton(hwnd,IDC_DX_ITEM,s_makeItem.m_attr.isDXItem);
	CheckDlgButton(hwnd,IDC_ULTRA_ITEM,s_makeItem.m_bf1IsUltimate);
	CheckDlgButton(hwnd,IDC_IS_USE_BULLET,s_makeItem.m_attr.isUseAbleBulletSubstituteArrow);
	CheckDlgButton(hwnd,IDC_IS_EXTRA_ITEM,s_makeItem.m_attr.isExtraItem);
	CheckDlgButton(hwnd,IDC_BEGINNER_ITEM,s_makeItem.m_attr.isBeginnerItem);

	CheckDlgButton(hwnd,IDC_EQUIPABLE_TO_BELT,s_makeItem.m_attr.isEquipableToBelt);
	CheckDlgButton(hwnd,IDC_CAN_NOT_USEABLE_ITEM,s_makeItem.m_attr.isCanNotUseableItem);
	CheckDlgButton(hwnd,IDC_CAN_NOT_DESTROY_ITEM,s_makeItem.m_attr.isCanNotDestroyItem);
	CheckDlgButton(hwnd,IDC_CAN_NOT_STORE_TO_BANK,s_makeItem.m_attr.isCanNotStoreBank);

	CheckDlgButton(hwnd,IDC_IS_CARPET_EMBLEM,s_makeItem.m_attr.isMagicCarpetEmblem);
	CheckDlgButton(hwnd,IDC_IS_MERGE_CARPET_EMBLEM,s_makeItem.m_attr.isMergeToCarpetEmblem);

	CheckDlgButton(hwnd,IDC_USE_TO_MINI_PET,s_makeItem.m_attr.isUseToMiniPet);

	CheckDlgButton(hwnd,IDC_REMEBER_PLACE_0,s_makeItem.m_bf1RemeberPlace0);
	CheckDlgButton(hwnd,IDC_REMEBER_PLACE_1,s_makeItem.m_bf1RemeberPlace1);
	CheckDlgButton(hwnd,IDC_REMEBER_PLACE_2,s_makeItem.m_bf1RemeberPlace2);

	CheckDlgButton(hwnd,IDC_BUY_ONLY_GUILD_MASTER,s_makeItem.m_bf1IsBuyOnlyGuildMaster);
	CheckDlgButton(hwnd,IDC_USE_ONLY_GUILD_MASTER,s_makeItem.m_bf1IsUseOnlyGuildMaster);
	CheckDlgButton(hwnd,IDC_CAN_NOT_FEED_TO_PET,s_makeItem.m_bf1IsBlockToFeedPet);
	CheckDlgButton(hwnd,IDC_FREE_TELEPORT,s_makeItem.m_bf1IsFreeTeleport);
	CheckDlgButton(hwnd,IDC_REQUEST_SUMMON_CARPET,s_makeItem.m_bf1IsRequestSummonCarpet);
	CheckDlgButton(hwnd,IDC_CAN_SUMMON_CARPET,s_makeItem.m_bf1IsCanSummonMagicCarpet);
	CheckDlgButton(hwnd,IDC_FREE_PAST_PORTAL,s_makeItem.m_bf1IsFreePastPortal);
	CheckDlgButton(hwnd,IDC_HALF_PAST_PORTAL,s_makeItem.m_bf1IsHalfPastPortal);

	CheckDlgButton(hwnd,IDC_IS_POTION	,s_makeItem.m_bf1IsUseAbleByThrowPotionSkill);
	CheckDlgButton(hwnd,IDC_IS_FLOWER	,s_makeItem.m_bf1IsUseAbleByThrowFlowerSkill);
	CheckDlgButton(hwnd,IDC_IS_DRUG		,s_makeItem.m_bf1IsUseAbleByThrowDrugSkill);
	CheckDlgButton(hwnd,IDC_IS_CANDY	,s_makeItem.m_bf1IsUseAbleByThrowCandySkill);

	CheckDlgButton(hwnd,IDC_EXCEPTION_ITEM,s_makeItem.m_bIsExceptionItem);
	CheckDlgButton(hwnd,IDC_OCCUR_EAT_EFFECT,s_makeItem.m_bf1IsOccurEatPotionEffect);

//	CheckDlgButton(hwnd,IDC_ADD_ENTRY_GUILD_DUNGEON,s_makeItem.m_bf1AddEntryGuildDungeon);
	CheckDlgButton(hwnd,IDC_MYSTIC_STONE_ADD_GAIN,s_makeItem.m_bf1IsAddGetMysticStone);
	CheckDlgButton(hwnd,IDC_TANTILIS_RELIC_ADD_GAIN,s_makeItem.m_bf1IsAddGetTantalissRelic);
	CheckDlgButton(hwnd,IDC_IGNORE_GUILD_HALL_LEVEL_FOR_ENTRY_TANTALIS_EXILE,s_makeItem.m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile);
	CheckDlgButton(hwnd,IDC_INCREASE_INFINITY_SERIOUS_UPGRADE_CHANCE,s_makeItem.m_bf1IsInfinitySeriousUpgradeChanceUp);
	
	CheckDlgButton(hwnd,IDC_ADD_GUILD_INFO_REMAIN_GAIN_LOG,s_makeItem.m_bf1IsRequireLogWithGuildInfo);
	
	CheckDlgButton(hwnd,IDC_IS_ETERNAL,s_makeItem.m_bf1IsEternalItem);		// 09.08.25
	CheckDlgButton(hwnd,IDC_ETERNAL_ITEM_UPGRADE_NOT_PENALTY,s_makeItem.m_bf1EternalItemUpgradeNotPenalty);		// 09.08.25
	CheckDlgButton(hwnd,IDC_UPGRADE_MATERIALS_ITEM,s_makeItem.m_bf1IsUpgradeMaterials);		// 09.08.26
	CheckDlgButton(hwnd,IDC_ITEM_NOT_USE_PET,s_makeItem.m_bf1IsItemNotUsePet);		// 09.09.04	
	CheckDlgButton(hwnd,IDC_USE_SHOP,s_makeItem.m_bf1IsUseShop);			//  상점 이용하기
	CheckDlgButton(hwnd,IDC_USE_BANK,s_makeItem.m_bf1IsUseBank);			// 은행 이용하기
	CheckDlgButton(hwnd,IDC_IS_NOT_CONSUME,s_makeItem.m_bf1IsNotConsume);			// 아이템 소모 안됨
	CheckDlgButton(hwnd,IDC_USE_ITEM_AFTER_TRANSFORMATION,s_makeItem.m_bf1IsUseItemAfterTransformation);			// 몬스터 변신 후 사용

	CheckDlgButton(hwnd,IDC_IS_RING_OF_INFINITY,s_makeItem.m_bf1IsRingOfInfinity);			// 무한의반지

	SetDlgItemInt(hwnd, IDC_LIMIT_UPGRADE_RING_OF_INFINITY, s_makeItem.m_bf5LimitUpgradeRingOfInfinity, FALSE);		// 무한의 반지 업그레이드 최대치
	CheckDlgButton(hwnd,IDC_USE_TO_ENEMY,s_makeItem.m_bf1IsUseToEnemy);				// 적에게 사용
	CheckDlgButton(hwnd,IDC_USE_AFTER_WEAR,s_makeItem.m_bf1IsUseAfterWear);			// 착용 후 기능 사용
	CheckDlgButton(hwnd,IDC_USE_PREMIUM_INVENTORY,s_makeItem.m_bf1IsUsePremiumInventory);			// 유료아이템 전용 인벤토리 사용
	CheckDlgButton(hwnd,IDC_IS_NOT_APPLY_COPY_EFFECT,s_makeItem.m_bf1IsNotApplyCopyEffect);			// 복사 효과 적용안됨
	CheckDlgButton(hwnd,IDC_IS_NOT_APPLY_REVERSION_EFFECT,s_makeItem.m_bf1IsNotApplyReversionEffect);			// 거래불가해제 효과 적용안됨
	CheckDlgButton(hwnd,IDC_IS_NOT_GET_USED_SKILL,s_makeItem.m_bf1IsNotGetUsedSkill);			// 스킬로 줍기 불가
	CheckDlgButton(hwnd,IDC_IS_NOT_USE_TO_GVG,s_makeItem.m_bf1IsNotUseToGVG);			// GVG에서 사용불가


//	착용 가능한 성향
	cCOMBOBOX::SelectByData(GetDlgItem(hwnd,IDC_ALLIGNMENT)	,s_makeItem.m_wRequireAllignment);

//	아이템 분류
	cCOMBOBOX::SelectByData(GetDlgItem(hwnd,IDC_BASE_ITEM)	,s_makeItem.m_wBaseItem);

//	퀘스트
	if (s_makeItem.m_questIndex	==	dNOT_QUEST_ITEM)
		cCOMBOBOX::SelectByData(GetDlgItem(hwnd,IDC_QUEST)		,dNOT_QUEST_ITEM);
	else
	{
		if (s_makeItem.m_questKind	==	eQT_NORMAL)	
				cCOMBOBOX::SelectByData(GetDlgItem(hwnd,IDC_QUEST)		,s_makeItem.m_questIndex);
		else	cCOMBOBOX::SelectByData(GetDlgItem(hwnd,IDC_QUEST)		,s_makeItem.m_questIndex+dQUEST_ITEM_KIND_BORDER);
	}

//	착용 제한 능력치 계산식
	{
		HWND	hComboEquipUseLimitContents	=	GetDlgItem(hwnd,IDC_EQUIP_USE_LIMIT_CONTENTS);
		HWND	hComboEquipUseLimitMethod	=	GetDlgItem(hwnd,IDC_EQUIP_USE_LIMIT_COMPUTE_METHOD);

		cCOMBOBOX::SelectByData(hComboEquipUseLimitMethod,s_makeItem.m_wEquipUseLimitMethod);		//	가격
		cCOMBOBOX::SelectByData(hComboEquipUseLimitContents,s_makeItem.m_wEquipUseLimitContents);	//	가격

		SetDlgItemInt(hwnd,IDC_EQUIP_USE_LIMIT_VALUE,s_makeItem.m_wEquipUseLimitValue,FALSE);		//	가격
	}

//	프리미엄 레벨
	{
		HWND	hComboPremiumServiceLevel	=	GetDlgItem(hwnd,IDC_PREMIUM_LEVEL);

		cCOMBOBOX::SelectByData(hComboPremiumServiceLevel,s_makeItem.m_wPremiumLevel);		//	가격
	}

//	아이템 강제 이펙트
	{
		HWND	hComboOutputEffect	=	GetDlgItem(hwnd,IDC_COMPULSION_EFFECT);
		
		cCOMBOBOX::SelectByData(hComboOutputEffect,s_makeItem.m_bCompulsionColorizeEffect);		//	가격
	}
//	아이템 이펙트
	{
		HWND	hComboItemEffect	=	GetDlgItem(hwnd,IDC_ITEM_EFFECT);
		
		cCOMBOBOX::SelectByData(hComboItemEffect,s_makeItem.m_wItemEffect);		//	가격
	}

//	장비 등급
	{
		HWND	hComboEquipGrade	=	GetDlgItem(hwnd,IDC_EQUIP_GRADE);

		cCOMBOBOX::SelectByData(hComboEquipGrade,s_makeItem.m_wEquippedShape);
	}

//	팔레트 리스트
	{
		HWND	hComboPaletteList	=	GetDlgItem(hwnd,IDC_PALETTE_LIST);

		cCOMBOBOX::SelectByData(hComboPaletteList,s_makeItem.m_wPaletteIndex);
	}


//	가격 계산식
	{
		HWND	hComboPriceComputeMethod	=	GetDlgItem(hwnd,IDC_PRICE_COMPUTE_METHOD);

		cCOMBOBOX::SelectByData(hComboPriceComputeMethod,s_makeItem.m_wPriceComputeMethod);
	}

//	s_makeItem.m_wFieldShape;							//	필드에서의 모양
//	s_makeItem.m_wIconShape;							//	상태창에서의 아이콘 모양
//	s_makeItem.m_wEquippedShape;						//	착용하고 있을때의 모양

	SetDlgItemInt(hwnd,IDC_PREMIUM_GRADE		,s_makeItem.m_wPremiumItemLevelForWithdraw	,FALSE);	//	프리미엄 등급

	SetDlgItemInt(hwnd,IDC_VALUE1_1				,s_makeItem.m_aValue[0][0]			,FALSE);	//	가격
	SetDlgItemInt(hwnd,IDC_VALUE1_2				,s_makeItem.m_aValue[0][1]			,FALSE);	//	가격
	SetDlgItemInt(hwnd,IDC_VALUE2_1				,s_makeItem.m_aValue[1][0]			,FALSE);	//	가격
	SetDlgItemInt(hwnd,IDC_VALUE2_2				,s_makeItem.m_aValue[1][1]			,FALSE);	//	가격

	SetDlgItemInt(hwnd,IDC_MIN_ENCHANT_SUCCESS_CHANCE	,s_makeItem.m_wEnchantMinChance,FALSE);
	SetDlgItemInt(hwnd,IDC_MAX_ENCHANT_SUCCESS_CHANCE	,s_makeItem.m_wEnchantMaxChance,FALSE);
	SetDlgItemInt(hwnd,IDC_ENCHANT_LIMIT_CODE			,s_makeItem.m_wEnchantLimitPrefixDiscernmentCode	,FALSE);

	SetDlgItemInt(hwnd,IDC_RANGE				,s_makeItem.m_wRange				,FALSE);	//	가격
	SetDlgItemText(hwnd,IDC_SPEED,_ms("%d.%.2d",s_makeItem.m_wSpeed/100,s_makeItem.m_wSpeed%100)); // minimum damage
	SetDlgItemInt(hwnd,IDC_PRICE				,s_makeItem.m_dwPrice				,FALSE);	//	가격
	SetDlgItemInt(hwnd,IDC_PRICE2				,s_makeItem.m_wPriceValue			,FALSE);	//	가격
	SetDlgItemInt(hwnd,IDC_COOL_TIME_SLOT		,s_makeItem.m_bCoolTimeIndex		,FALSE);	//	쿨타임 인덱스
	SetDlgItemInt(hwnd,IDC_COOL_TIME			,s_makeItem.m_wCoolTimeSecond		,FALSE);	//	쿨타임

	SetDlgItemInt(hwnd,IDC_DROP_LEVEL			,s_makeItem.m_wDropLevel			,FALSE);	//	가격
	SetDlgItemText(hwnd,IDC_CORRECT_DROP_CHANCE,_ms("%d.%.1d",s_makeItem.m_wCorrectDropChance/10,s_makeItem.m_wCorrectDropChance%10)); // minimum damage
	SetDlgItemInt(hwnd,IDC_MIN_DAMAGE			,s_makeItem.m_wMinDamage			,FALSE);	//	최소 데미지
	SetDlgItemInt(hwnd,IDC_MAX_DAMAGE			,s_makeItem.m_wMaxDamage			,FALSE);	//	최대 데미지
	SetDlgItemInt(hwnd,IDC_DURABILITY			,s_makeItem.m_wDurability			,FALSE);	//	내구도
	SetDlgItemInt(hwnd,IDC_DAMAGE_RANGE			,s_makeItem.m_wDamageRange			,FALSE);	//	내구도
	SetDlgItemInt(hwnd,IDC_STACK_COUNT			,s_makeItem.m_wStackLimit			,FALSE);	//	무게

	SetDlgItemInt(hwnd,IDC_REQUIRE_LEVEL		,s_makeItem.m_wRequireLevel			,FALSE);	//	사용/장비를 위해 필요한 레벨
	SetDlgItemInt(hwnd,IDC_REQUIRE_STRENGTH		,s_makeItem.m_wRequireStrength		,FALSE);	//	사용/장비를 위해 필요한 힘
	SetDlgItemInt(hwnd,IDC_REQUIRE_DEXTERITY	,s_makeItem.m_wRequireDexterity		,FALSE);	//	사용/장비를 위해 필요한 민첩성
	SetDlgItemInt(hwnd,IDC_REQUIRE_CONSTITUTE	,s_makeItem.m_wRequireConstitution	,FALSE);	//	사용/장비를 위해 필요한 건강
	SetDlgItemInt(hwnd,IDC_REQUIRE_WISDOM		,s_makeItem.m_wRequireWisdom		,FALSE);	//	사용/장비를 위해 필요한 지혜
	SetDlgItemInt(hwnd,IDC_REQUIRE_INTELLIGENCE	,s_makeItem.m_wRequireIntelligence	,FALSE);	//	사용/장비를 위해 필요한 지식
	SetDlgItemInt(hwnd,IDC_REQUIRE_CHARISMA		,s_makeItem.m_wRequireCharisma		,FALSE);	//	사용/장비를 위해 필요한 카리스마
	SetDlgItemInt(hwnd,IDC_REQUIRE_LUCK			,s_makeItem.m_wRequireLuck			,FALSE);	//	사용/장비를 위한 필요한 운

	SetDlgItemInt(hwnd,IDC_EXPIRE_YEAR			,s_makeItem.m_wExpireYear			,FALSE);	//	유효기간 연
	SetDlgItemInt(hwnd,IDC_EXPIRE_MONTH			,s_makeItem.m_wExpireMonth			,FALSE);	//	유효기간 월
	SetDlgItemInt(hwnd,IDC_EXPIRE_DAY			,s_makeItem.m_wExpireDay			,FALSE);	//	유효기간 일
	SetDlgItemInt(hwnd,IDC_EXPIRE_HOUR			,s_makeItem.m_wExpireHour			,FALSE);	//	유효기간 시
	SetDlgItemInt(hwnd, IDC_UPGRADE_BASE_SUCCESS_PERCENT, s_makeItem.m_bf7UpgradeBaseSuccessPercent, FALSE);		// 강화 성공 확률 09.08.25
	SetDlgItemInt(hwnd, IDC_NEXT_ETERNAL_ITEM_SERIAL, s_makeItem.m_wNextEternalItemSerial, FALSE);		// 09.08.25
	SetDlgItemInt(hwnd, IDC_UPGRADE_PENALTY_ITEM_SERIAL, s_makeItem.m_wUpgradePenaltyItemSerial, FALSE);		// 강화패널티 아이템 시리얼09.08.26
	SetDlgItemInt(hwnd, IDC_UPGRADE_COSMICPOWER_SUCCESS_PERCENT, s_makeItem.m_bf7UpgradeCosmicPowerSuccessPercent, FALSE);		// 코스믹강화 성공 확률 09.08.26
	SetDlgItemInt(hwnd, IDC_UPGRADE_MATERIALS_SUCCESS_PERCENT, s_makeItem.m_bf7UpgradeMaterialsSuccessPercent, FALSE);		// 강화제강화 성공 확률 09.08.26
	SetDlgItemInt(hwnd, IDC_MOVE_FIELD_SERIAL, s_makeItem.m_wMoveFieldSerial, FALSE);		// 필드이동 시리얼
	SetDlgItemInt(hwnd,IDC_REQUIRE_MAX_LEVEL		,s_makeItem.m_wRequireMaxLevel			,FALSE);	//	사용/장비를 위해 필요한 최대 레벨
}

//
//	아이템을 추가 박스 내용 얻기
BOOL
cSET_BASIC_ITEM::GetItemAddBoxData(HWND hwnd)			// 09.08.07
{
	cBASIC_ITEM		tempItem;
	BOOL			temp;

	tempItem.copy(&s_makeItem);

	GetDlgItemText(hwnd,IDC_NAME,tempItem.m_strName,sizeof(tempItem.m_strName));

	if (strlen(tempItem.m_strName)	<=	0	)
	{
		ERRMSG(dMSG_BASE_ITEM_ADD_ERROR,dMSG_ITEM_NAME_VALID);
		return	FALSE;
	}

	cBASIC_ITEM*	lpItem	=	cBASIC_ITEM::Get(tempItem.m_strName);

	if (lpItem)
	{
		if (!s_bEditItem	||	lpItem->m_iSerial	!=	s_iSelectItem)
		{
			ERRMSG(dMSG_BASE_ITEM_ADD_ERROR,dMSG_ALREADY_EXIST_SAME_ITEM);
			SetFocus(GetDlgItem(hwnd,IDC_NAME));

			return	FALSE;
		}
	}

	tempItem.m_wIsIDPublicItem									=		s_makeItem.m_wIsIDPublicItem;								
	tempItem.m_attr.isDXItem									=		s_makeItem.m_attr.isDXItem;									
	tempItem.m_bf1IsUltimate									=		s_makeItem.m_bf1IsUltimate;									
	tempItem.m_attr.isGuildBattleItem							=		s_makeItem.m_attr.isGuildBattleItem;							
	tempItem.m_attr.isBeginnerItem								=		s_makeItem.m_attr.isBeginnerItem;							
	tempItem.m_attr.isExtraItem									=		s_makeItem.m_attr.isExtraItem;								
	tempItem.m_attr.isBadge										=		s_makeItem.m_attr.isBadge	;								

	tempItem.m_attr.isCanNotTradeItem							=		s_makeItem.m_attr.isCanNotTradeItem;							
	tempItem.m_attr.isCanNotDropItem							=		s_makeItem.m_attr.isCanNotDropItem;							
	tempItem.m_attr.isCanNotDestroyItem							=		s_makeItem.m_attr.isCanNotDestroyItem;						
	tempItem.m_attr.isCanNotUseableItem							=		s_makeItem.m_attr.isCanNotUseableItem;						
	tempItem.m_attr.isCanNotStoreBank							=		s_makeItem.m_attr.isCanNotStoreBank	;						
	tempItem.m_attr.isCanNotSelllItem							=		s_makeItem.m_attr.isCanNotSelllItem	;						
	tempItem.m_bf1IsBlockToFeedPet								=		s_makeItem.m_bf1IsBlockToFeedPet	;						
	tempItem.m_attr.isUseAbleBulletSubstituteArrow				=		s_makeItem.m_attr.isUseAbleBulletSubstituteArrow;			
	tempItem.m_attr.isDurabilityToUseCount						=		s_makeItem.m_attr.isDurabilityToUseCount	;				

	tempItem.m_attr.isUseToItem									=		s_makeItem.m_attr.isUseToItem	;							
	tempItem.m_attr.isCanUseCorpseStatus						=		s_makeItem.m_attr.isCanUseCorpseStatus	;					
	tempItem.m_attr.isUseToCorpse								=		s_makeItem.m_attr.isUseToCorpse				;				
	tempItem.m_attr.isUseToCharacter							=		s_makeItem.m_attr.isUseToCharacter			;				
	tempItem.m_attr.isUseToMiniPet								=		s_makeItem.m_attr.isUseToMiniPet			;				
	tempItem.m_bf1IsUseOnlyGuildMaster							=		s_makeItem.m_bf1IsUseOnlyGuildMaster		;				
	tempItem.m_bf1IsBuyOnlyGuildMaster							=		s_makeItem.m_bf1IsBuyOnlyGuildMaster		;				
	tempItem.m_bf1IsItemNotUsePet								=		s_makeItem.m_bf1IsItemNotUsePet;		// 09.09.04

	tempItem.m_attr.isEquipableToBelt							=		s_makeItem.m_attr.isEquipableToBelt			;				
	tempItem.m_attr.isEffectInInventoryItem						=		s_makeItem.m_attr.isEffectInInventoryItem	;				
	tempItem.m_bf1IsDestroyWhenMoveField						=		s_makeItem.m_bf1IsDestroyWhenMoveField		;				
	tempItem.m_attr.isCanNotOwnSameItem							=		s_makeItem.m_attr.isCanNotOwnSameItem		;				
	tempItem.m_bIsExceptionItem									=		s_makeItem.m_bIsExceptionItem				;				

	tempItem.m_attr.isRequireSaveUseLog							=		s_makeItem.m_attr.isRequireSaveUseLog		;				
	tempItem.m_bf1IsRequireLogWithGuildInfo						=		s_makeItem.m_bf1IsRequireLogWithGuildInfo	;				

	tempItem.m_bf1IsUseAbleByThrowPotionSkill					=		s_makeItem.m_bf1IsUseAbleByThrowPotionSkill	;				
	tempItem.m_bf1IsUseAbleByThrowDrugSkill						=		s_makeItem.m_bf1IsUseAbleByThrowDrugSkill	;				
	tempItem.m_bf1IsUseAbleByThrowFlowerSkill					=		s_makeItem.m_bf1IsUseAbleByThrowFlowerSkill	;				
	tempItem.m_bf1IsUseAbleByThrowCandySkill					=		s_makeItem.m_bf1IsUseAbleByThrowCandySkill	;				

	tempItem.m_attr.isMagicCarpetEmblem							=		s_makeItem.m_attr.isMagicCarpetEmblem		;				
	tempItem.m_attr.isMergeToCarpetEmblem						=		s_makeItem.m_attr.isMergeToCarpetEmblem		;				
	tempItem.m_bf1IsRequestSummonCarpet							=		s_makeItem.m_bf1IsRequestSummonCarpet		;				
	tempItem.m_bf1IsCanSummonMagicCarpet						=		s_makeItem.m_bf1IsCanSummonMagicCarpet		;				
	tempItem.m_bf1RemeberPlace0									=		s_makeItem.m_bf1RemeberPlace0				;				
	tempItem.m_bf1RemeberPlace1									=		s_makeItem.m_bf1RemeberPlace1				;				
	tempItem.m_bf1RemeberPlace2									=		s_makeItem.m_bf1RemeberPlace2				;				
	tempItem.m_bf1IsFreePastPortal								=		s_makeItem.m_bf1IsFreePastPortal			;				
	tempItem.m_bf1IsHalfPastPortal								=		s_makeItem.m_bf1IsHalfPastPortal			;				
	tempItem.m_bf1IsFreeTeleport								=		s_makeItem.m_bf1IsFreeTeleport				;				
	tempItem.m_bf1IsAddGetMysticStone							=		s_makeItem.m_bf1IsAddGetMysticStone			;				
	tempItem.m_bf1IsAddGetTantalissRelic						=		s_makeItem.m_bf1IsAddGetTantalissRelic		;				
	tempItem.m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile	=		s_makeItem.m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile	;
	tempItem.m_bf1IsInfinitySeriousUpgradeChanceUp				=		s_makeItem.m_bf1IsInfinitySeriousUpgradeChanceUp			;


	tempItem.m_attr.isDestroyRingAtFailedEnchant=	IsDlgButtonChecked(hwnd,IDC_DESTROY_ENCHANT_FAILED_RING);
	tempItem.m_attr.isUpgradePrefixLevel		=	IsDlgButtonChecked(hwnd,IDC_UPGRADE_PREFIX_LEVEL);


	tempItem.m_attr.isExclusiveToNormalItem		=	IsDlgButtonChecked(hwnd,IDC_EXCLUSIVE_TO_NORMAL_ITEM);
	tempItem.m_attr.isExclusiveToUniqueItem		=	IsDlgButtonChecked(hwnd,IDC_EXCLUSIVE_TO_UNIQUE_ITEM);
	tempItem.m_bf1IsBlockToEnchant				=	IsDlgButtonChecked(hwnd,IDC_BLOCK_TO_ENCHANT);
	tempItem.m_bf1IsRingOfInfinity				=	s_makeItem.m_bf1IsRingOfInfinity;		// 무한의 반지
	tempItem.m_bf1IsUseToEnemy					=	s_makeItem.m_bf1IsUseToEnemy;		// 적에게 사용
	tempItem.m_bf1IsUseAfterWear				=	s_makeItem.m_bf1IsUseAfterWear;		// 착용 후 기능 사용
	tempItem.m_bf1IsUsePremiumInventory			=	s_makeItem.m_bf1IsUsePremiumInventory;		//  유료아이템 전용 인벤토리 사용
	tempItem.m_bf1IsNotApplyCopyEffect			=	s_makeItem.m_bf1IsNotApplyCopyEffect;			// 복사 효과 적용안됨
	tempItem.m_bf1IsNotApplyReversionEffect		=	s_makeItem.m_bf1IsNotApplyReversionEffect;	// 거래불가해제 효과 적용안됨
	tempItem.m_bf1IsNotGetUsedSkill				=	s_makeItem.m_bf1IsNotGetUsedSkill;		// 스킬로 줍기 불가
	tempItem.m_bf1IsNotUseToGVG					=	s_makeItem.m_bf1IsNotUseToGVG;			// GVG에서 사용불가

//	tempItem.m_bf1AddEntryGuildDungeon			=	IsDlgButtonChecked(hwnd,IDC_ADD_ENTRY_GUILD_DUNGEON);

	//VV 09.08.11 착용 갯수제한
	char strInt[8] = "";
	GetDlgItemText(hwnd, IDC_WEAR_LIMIT_COUNT, strInt, 2);
	tempItem.m_bf3WearLimit						= atoi(strInt);
	//AA 09.08.11 착용 갯수제한

	GetDlgItemText(hwnd, IDC_ETERNAL_ITEM_CURRENT_LEVEL, strInt, 3);			// 현재단계			09.08.25
	tempItem.m_bf4EternalItemCurrentLevel		= atoi(strInt);

	GetDlgItemText(hwnd, IDC_NEXT_ETERNAL_ITEM_SERIAL, strInt, 8);			// 다음단계아이템시리얼	09.08.25
	tempItem.m_wNextEternalItemSerial		= atoi(strInt);

	GetDlgItemText(hwnd, IDC_UPGRADE_PENALTY_ITEM_SERIAL, strInt, 8);			// 강화 패널티아이템시리얼	09.08.26
	tempItem.m_wUpgradePenaltyItemSerial		= atoi(strInt);
	
	GetDlgItemText(hwnd, IDC_UPGRADE_BASE_SUCCESS_PERCENT, strInt, 8);			// 강화성공확률 09.08.25
	tempItem.m_bf7UpgradeBaseSuccessPercent		= atoi(strInt);

	GetDlgItemText(hwnd, IDC_UPGRADE_COSMICPOWER_SUCCESS_PERCENT, strInt, 8);			// 코스믹강화성공확률 09.08.26
	tempItem.m_bf7UpgradeCosmicPowerSuccessPercent		= atoi(strInt);

	GetDlgItemText(hwnd, IDC_UPGRADE_MATERIALS_SUCCESS_PERCENT, strInt, 8);			// 강화제강화성공확률 09.08.26
	tempItem.m_bf7UpgradeMaterialsSuccessPercent		= atoi(strInt);

	GetDlgItemText(hwnd, IDC_ITEM_RIBIRTH_COUNT, strInt, 2);
	tempItem.m_bf4ItemRebirthCount						= atoi(strInt);

	GetDlgItemText(hwnd, IDC_LIMIT_UPGRADE_RING_OF_INFINITY, strInt, 8);			// 무한의 반지 업그레이드 최대치
	tempItem.m_bf5LimitUpgradeRingOfInfinity		= atoi(strInt);

	tempItem.m_bf1IsEternalItem					=	s_makeItem.m_bf1IsEternalItem;		// 09.08.25
	tempItem.m_bf1EternalItemUpgradeNotPenalty	=	s_makeItem.m_bf1EternalItemUpgradeNotPenalty;	// 09.08.25
	tempItem.m_bf1IsUpgradeMaterials			=	s_makeItem.m_bf1IsUpgradeMaterials;		// 강화제 아이템09.08.26
	tempItem.m_bf1IsUseShop						=	s_makeItem.m_bf1IsUseShop;		// 상점 이용하기
	tempItem.m_bf1IsUseBank						=	s_makeItem.m_bf1IsUseBank;		// 은행 이용하기
	tempItem.m_bf1IsNotConsume					=	s_makeItem.m_bf1IsNotConsume;		// 아이템 소모 안됨
	tempItem.m_bf1IsUseItemAfterTransformation					=	s_makeItem.m_bf1IsUseItemAfterTransformation;		// 몬스터 변신후사용
//	착용시 외향
	{
		HWND	hComboEquipGrade		=	GetDlgItem(hwnd,IDC_EQUIP_GRADE);
		tempItem.m_wEquippedShape		=	cCOMBOBOX::GetData(hComboEquipGrade);
	}

//	팔레트 리스트
	{
		HWND	hComboPaletteList		=	GetDlgItem(hwnd,IDC_PALETTE_LIST);

		tempItem.m_wPaletteIndex		=	cCOMBOBOX::GetData(hComboPaletteList);
	}

//	퀘스트 인덱스
	{
		HWND	hComboQuest				=	GetDlgItem(hwnd,IDC_QUEST);
		int		iQuestIndex				=	cCOMBOBOX::GetData(hComboQuest);

		tempItem.m_questIndex	=	iQuestIndex;
		tempItem.m_questKind	=	eQT_NORMAL;

		if (iQuestIndex	>=	dQUEST_ITEM_KIND_BORDER)
		{
			tempItem.m_questKind	=	eQT_ARBEIT;
			tempItem.m_questIndex	=	iQuestIndex	-	dQUEST_ITEM_KIND_BORDER;
		}
	}

//	가격 계산식
	{
		HWND	hComboPriceComputeMethod=	GetDlgItem(hwnd,IDC_PRICE_COMPUTE_METHOD);
		tempItem.m_wPriceComputeMethod	=	cCOMBOBOX::GetData(hComboPriceComputeMethod);
	}

//	프리미엄 레벨
	{
		HWND	hComboPremiumServiceLevel	=	GetDlgItem(hwnd,IDC_PREMIUM_LEVEL);

		tempItem.m_wPremiumLevel			=	cCOMBOBOX::GetData(hComboPremiumServiceLevel);
	}

//	아이템 강제 이펙트
	{
		HWND	hComboOutputEffect	=	GetDlgItem(hwnd,IDC_COMPULSION_EFFECT);
		
		tempItem.m_bCompulsionColorizeEffect	=	cCOMBOBOX::GetData(hComboOutputEffect);
	}
//	아이템 이펙트
	{
		HWND	hComboItemEffect	=	GetDlgItem(hwnd,IDC_ITEM_EFFECT);
		
		tempItem.m_wItemEffect	=	cCOMBOBOX::GetData(hComboItemEffect);
	}



//	사용/착용 제한
	{
		HWND	hComboAllignment			=	GetDlgItem(hwnd,IDC_ALLIGNMENT);
		HWND	hComboExGrade				=	GetDlgItem(hwnd,IDC_EX_GRADE);
		HWND	hComboEquipUseLimitContents	=	GetDlgItem(hwnd,IDC_EQUIP_USE_LIMIT_CONTENTS);
		HWND	hComboEquipUseLimitMethod	=	GetDlgItem(hwnd,IDC_EQUIP_USE_LIMIT_COMPUTE_METHOD);

//	직업
		{
			tempItem.m_uiEquipAbleJobMask	=	0;

			for (int i=0;;i++)
			{
				if	(g_aiEnableJobIDCList[i]	==	-1)
					break;

				if	(IsDlgButtonChecked(hwnd,g_aiEnableJobIDCList[i]))
					tempItem.m_uiEquipAbleJobMask	+=	(1<<i);
			}

			if	(IsDlgButtonChecked(hwnd,IDC_MALE))	
				tempItem.m_bf1IsMaleJobEquipAble	=	1;
			else
				tempItem.m_bf1IsMaleJobEquipAble	=	0;

			if	(IsDlgButtonChecked(hwnd,IDC_FEMALE))
				tempItem.m_bf1IsFemaleJobEquipAble	=	1;
			else
				tempItem.m_bf1IsFemaleJobEquipAble	=	0;
		}

//	착용 제한 능력치 계산식
		{
			tempItem.m_wEquipUseLimitMethod		=	cCOMBOBOX::GetData(hComboEquipUseLimitMethod);		//	착용/사용 제한 능력치 계산법
			tempItem.m_wEquipUseLimitContents	=	cCOMBOBOX::GetData(hComboEquipUseLimitContents);	//	착용/사용 제한 능력

			tempItem.m_wEquipUseLimitValue		=	GetDlgItemInt(hwnd,IDC_EQUIP_USE_LIMIT_VALUE,&temp,FALSE);	//	가격

			if (!temp || tempItem.m_wEquipUseLimitContents==	0xffff	||	tempItem.m_wEquipUseLimitValue	==	0)
			{
				tempItem.m_wEquipUseLimitMethod	=	0xffff;
				tempItem.m_wEquipUseLimitContents=	0xffff;
				tempItem.m_wEquipUseLimitValue	=	0;
			}
		}

//	착용 가능한 성향
		tempItem.m_wRequireAllignment	=	cCOMBOBOX::GetData(hComboAllignment);
		tempItem.m_wExtraGrade			=	cCOMBOBOX::GetData(hComboExGrade);
	}

	tempItem.m_wRange				=	GetDlgItemInt(hwnd,IDC_RANGE				,&temp,FALSE);	//	가격

	if	(!temp)
		goto LABLE_OCCURED_ERROR;

	{
		char	strText[20];

		GetDlgItemText(hwnd,IDC_SPEED,strText,sizeof(strText));	//	공속

		int	iValue			=	StringToNumber(strText,2,2);

		if	(!temp)
			goto LABLE_OCCURED_ERROR;

		tempItem.m_wSpeed	=	iValue;
	}

	{
		char	strText[20];

		GetDlgItemText(hwnd,IDC_CORRECT_DROP_CHANCE,strText,sizeof(strText));	//	공속

		int	iValue			=	StringToNumber(strText,1000,1);

		if	(!temp)
			goto LABLE_OCCURED_ERROR;

		tempItem.m_wCorrectDropChance	=	iValue;
	}

	tempItem.m_wPremiumItemLevelForWithdraw			=	GetDlgItemInt(hwnd,IDC_PREMIUM_GRADE	,&temp,FALSE);	//	가격
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wEnchantLimitPrefixDiscernmentCode	=	GetDlgItemInt(hwnd,IDC_ENCHANT_LIMIT_CODE	,&temp,FALSE);	//	가격
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wEnchantMinChance	=	GetDlgItemInt(hwnd,IDC_MIN_ENCHANT_SUCCESS_CHANCE	,&temp,FALSE);	//	가격
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wEnchantMaxChance	=	GetDlgItemInt(hwnd,IDC_MAX_ENCHANT_SUCCESS_CHANCE	,&temp,FALSE);	//	가격
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wMoveFieldSerial						=	GetDlgItemInt(hwnd,IDC_MOVE_FIELD_SERIAL	,&temp,FALSE);	//	필드 이동 시리얼
	if (!temp)	goto LABLE_OCCURED_ERROR;

	//	쿨타임
	{
		tempItem.m_bCoolTimeIndex		=	GetDlgItemInt(hwnd,IDC_COOL_TIME_SLOT		,&temp,FALSE);	//	쿨타임 인덱스
		if (!temp)	goto LABLE_OCCURED_ERROR;

		tempItem.m_wCoolTimeSecond		=	GetDlgItemInt(hwnd,IDC_COOL_TIME			,&temp,FALSE);	//	쿨타임
		if (!temp)	goto LABLE_OCCURED_ERROR;

		if	(tempItem.m_bCoolTimeIndex	>=	dITEM_COOL_TIME_BUFFER_SIZE)
		{
			cMSG::Error("쿨타임 인덱스가 이상합니다.","정상 인덱스 범위 0~%d\n\n쿨타임 슬롯을 %d로 보정합니다.",dITEM_COOL_TIME_BUFFER_SIZE-1,dITEM_COOL_TIME_BUFFER_SIZE-1);

			tempItem.m_bCoolTimeIndex	=	dITEM_COOL_TIME_BUFFER_SIZE-1;
		}
		if	(tempItem.m_bCoolTimeIndex	&&	tempItem.m_wCoolTimeSecond	==	0)
		{
			cMSG::Error("쿨타임 인덱스가 이상합니다.","쿨타임 슬롯은 설정되어 있으나 쿨 타임은 설정되어 있지 않습니다.\n\n치명적인 문제는 발생하지 않지만 쿨타임이 적용되지는 않습니다.\n\n쿨타임 슬롯을 0로 수정합니다.");

			tempItem.m_bCoolTimeIndex	=	0;
		}
	}

	tempItem.m_dwPrice				=	GetDlgItemInt(hwnd,IDC_PRICE				,&temp,FALSE);	//	가격
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wPriceValue			=	GetDlgItemInt(hwnd,IDC_PRICE2				,&temp,FALSE);	//	가격
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wMinDamage			=	GetDlgItemInt(hwnd,IDC_MIN_DAMAGE			,&temp,FALSE);	//	최소 데미지
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wMaxDamage			=	GetDlgItemInt(hwnd,IDC_MAX_DAMAGE			,&temp,FALSE);	//	최대 데미지
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wDurability			=	GetDlgItemInt(hwnd,IDC_DURABILITY			,&temp,FALSE);	//	내구도
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wDropLevel			=	GetDlgItemInt(hwnd,IDC_DROP_LEVEL			,&temp,FALSE);	//	내구도
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wDamageRange			=	GetDlgItemInt(hwnd,IDC_DAMAGE_RANGE			,&temp,FALSE);	//	내구도
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wStackLimit			=	GetDlgItemInt(hwnd,IDC_STACK_COUNT			,&temp,FALSE);	//	스택 카운트
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wRequireLevel		=	GetDlgItemInt(hwnd,IDC_REQUIRE_LEVEL		,&temp,FALSE);	//	사용/장비를 위해 필요한 레벨
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wRequireMaxLevel		=	GetDlgItemInt(hwnd,IDC_REQUIRE_MAX_LEVEL		,&temp,FALSE);	//	사용/장비를 위해 필요한 레벨
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wRequireStrength		=	GetDlgItemInt(hwnd,IDC_REQUIRE_STRENGTH		,&temp,FALSE);	//	사용/장비를 위해 필요한 힘
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wRequireDexterity	=	GetDlgItemInt(hwnd,IDC_REQUIRE_DEXTERITY	,&temp,FALSE);	//	사용/장비를 위해 필요한 민첩성
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wRequireConstitution	=	GetDlgItemInt(hwnd,IDC_REQUIRE_CONSTITUTE	,&temp,FALSE);	//	사용/장비를 위해 필요한 건강
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wRequireWisdom		=	GetDlgItemInt(hwnd,IDC_REQUIRE_WISDOM		,&temp,FALSE);	//	사용/장비를 위해 필요한 지혜
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wRequireIntelligence	=	GetDlgItemInt(hwnd,IDC_REQUIRE_INTELLIGENCE	,&temp,FALSE);	//	사용/장비를 위해 필요한 지식
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wRequireCharisma		=	GetDlgItemInt(hwnd,IDC_REQUIRE_CHARISMA		,&temp,FALSE);	//	사용/장비를 위해 필요한 카리스마
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wRequireLuck			=	GetDlgItemInt(hwnd,IDC_REQUIRE_LUCK			,&temp,FALSE);	//	사용/장비를 위한 필요한 운
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wExpireYear			=	GetDlgItemInt(hwnd,IDC_EXPIRE_YEAR			,&temp,FALSE);	//	사용/장비를 위한 필요한 운
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wExpireMonth			=	GetDlgItemInt(hwnd,IDC_EXPIRE_MONTH			,&temp,FALSE);	//	사용/장비를 위한 필요한 운
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wExpireDay			=	GetDlgItemInt(hwnd,IDC_EXPIRE_DAY			,&temp,FALSE);	//	사용/장비를 위한 필요한 운
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_wExpireHour			=	GetDlgItemInt(hwnd,IDC_EXPIRE_HOUR			,&temp,FALSE);	//	사용/장비를 위한 필요한 운
	if (!temp)	goto LABLE_OCCURED_ERROR;

	tempItem.m_aValue[0][0]	=	GetDlgItemInt(hwnd,IDC_VALUE1_1	,&temp,FALSE);	
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_aValue[0][1]	=	GetDlgItemInt(hwnd,IDC_VALUE1_2	,&temp,FALSE);	
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_aValue[1][0]	=	GetDlgItemInt(hwnd,IDC_VALUE2_1	,&temp,FALSE);	
	if (!temp)	goto LABLE_OCCURED_ERROR;
	tempItem.m_aValue[1][1]	=	GetDlgItemInt(hwnd,IDC_VALUE2_2	,&temp,FALSE);	
	if (!temp)	goto LABLE_OCCURED_ERROR;


	if	(tempItem.m_aValue[0][0] > tempItem.m_aValue[0][1])
		MessageBox(NULL ,"아이템 수치1의 관계가 이상합니다." , "ERROR", MB_OK);

	if	(tempItem.m_aValue[1][0] > tempItem.m_aValue[1][1])
		MessageBox(NULL ,"아이템 수치2의 관계가 이상합니다." , "ERROR", MB_OK);
	//tempItem.m_aValue[0][1]	=	max(tempItem.m_aValue[0][0],tempItem.m_aValue[0][1]);
	//tempItem.m_aValue[1][1]	=	max(tempItem.m_aValue[1][0],tempItem.m_aValue[1][1]);

//	아이템 분류
	tempItem.m_wBaseItem	=	cCOMBOBOX::GetData(GetDlgItem(hwnd,IDC_BASE_ITEM));

	if (s_bEditItem)	tempItem.m_wKind=	s_makeItem.m_wKind;
	else				tempItem.m_wKind=	s_iEditItemKind;

	char	strComment[2048];

	GetDlgItemText(hwnd,IDC_COMMENT,strComment,sizeof(strComment));

	if (strlen(strComment)	==	0)
		tempItem.setComment("no comment");
	else
		tempItem.setComment(strComment);

	s_makeItem.copy(&tempItem);

	if (!s_makeItem.isStackAble())
		s_makeItem.m_wStackLimit	=	1;

	if	(s_makeItem.m_wStackLimit	>	255)
	{
		cMSG::Put("Error","The maximum stack number of items is 255.\n\nForce the stack limit to be set to 255.");
		s_makeItem.m_wStackLimit	=	255;
	}

	return	TRUE;

LABLE_OCCURED_ERROR:

	return	ERRMSG(dMSG_BASE_ITEM_ADD_ERROR,dMSG_SOME_DATA_VALID);
}	//	cSET_BASIC_ITEM::FillItemAddBox(HWND hwnd)



//
//	기본 아이템 리스트 박스를 그려준다.(-_-?)
//
void
cSET_BASIC_ITEM::DrawAddItemWnd(HWND hwnd)
{
//	아이콘 모양
	s_dibIconShape.Active();

	cDRAW::Fill(0);

	g_smiIconItem.put(s_makeItem.m_wIconShape,11,7);

	s_dibIconShape.Draw();


//	착용시의 모습
	if (s_dibEquippedShape.Active())
	{
		cDRAW::Fill(0);
		
		s_dibEquippedShape.Draw();
	}

//	갑옷,방패,무기
	if (s_dibFieldShape.Active())
	{
		cDRAW::Fill(0);

		g_sprFieldShape.FitPut(0,0,cDRAW::WIDTH,cDRAW::HEIGHT,s_makeItem.m_wFieldShape);

		s_dibFieldShape.Draw();
	}

//	아이템 효과
	DrawItemGerateData();
	DrawUniqueData();
}	//	cSET_BASIC_ITEM::DrawAddItemWnd(HWND hwnd)

//
//	기본 아이템 리스트 박스를 그려준다.(-_-?)
void
cSET_BASIC_ITEM::DrawItemGerateData()
{
//	아이템 효과
	s_dibItemGenerateData.Active();

	cDRAW::Fill(_SKYBLUE);

	int	x =5,y=5;

	int	iWidth	=	s_dibItemGenerateData.width() - 14;

	for (int i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if (s_makeItem.m_aGenerateData[i].m_wEffect	==	0xffff)	break;

		char	*lpComment	=	s_makeItem.getDataComment(i,TRUE);
		
		int		iLineCount	=	cTEXT::GetLines(iWidth,lpComment);

		cDRAW::Fill		(0,x,y,x+iWidth+6,y+iLineCount*14+4);

		if (i	==	s_iSelectGerateData)
		{
			cDRAW::Fill	(_LTBLUE,x-2,y-2,x+iWidth+4,y+iLineCount*14+2);
			cTEXT::FitPut	(x+2,y+1,iWidth,iLineCount*8*2,LTYELLOW,lpComment);
		}
		else
		{	
			cDRAW::Fill		(_DEEPGRAY  ,x-2,y-2,x+iWidth+4,y+iLineCount*14+2);
			cTEXT::FitPut	(x+2,y+1,iWidth,iLineCount*8*2,WHITE,lpComment);
		}

		y	+=	iLineCount*14;
		y	+=	4+4;
	}

	s_dibItemGenerateData.Draw();

}	//	cSET_BASIC_ITEM::DrawAddItemWnd(HWND hwnd)

//
//	유니크 아이템 데이터
void
cSET_BASIC_ITEM::DrawUniqueData()
{
//	아이템 효과
	s_dibUniqueItemData.Active();

	cDRAW::Fill(_SKYBLUE);

	int	x =5,y=5;

	int	iWidth	=	s_dibUniqueItemData.width() - 14;

	for (int i=0;i<dITEM_UNIQUE_DATA_COUNT;i++)
	{
		if (s_makeItem.m_aUniqueData[i].m_wEffect	==	0xffff)	break;

		char	*lpComment	=	s_makeItem.m_aUniqueData[i].getComment(TRUE);
		
		int		iLineCount	=	cTEXT::GetLines(iWidth,lpComment);

		cDRAW::Fill		(0,x,y,x+iWidth+6,y+iLineCount*14+4);

		if (i	==	s_iSelectUniqueData)
		{
			cDRAW::Fill		(_LTBLUE,x-2,y-2,x+iWidth+4,y+iLineCount*14+2);
			cTEXT::FitPut	(x+2,y+1,iWidth,iLineCount*8*2,LTYELLOW,lpComment);
		}
		else
		{	
			cDRAW::Fill		(_DEEPGRAY  ,x-2,y-2,x+iWidth+4,y+iLineCount*14+2);
			cTEXT::FitPut	(x+2,y+1,iWidth,iLineCount*8*2,WHITE,lpComment);
		}

		y	+=	iLineCount*14;
		y	+=	4+4;
	}

	s_dibUniqueItemData.Draw();

}

//
//	아이템 효과 선택
int
cSET_BASIC_ITEM::SelectUniqueData(int _iX,int _iY)
{
//	아이템 효과
	int	x =5,y=5;

	int	iWidth	=	s_dibUniqueItemData.width() - 14;

	if (_iX	<0	||	_iX	>	s_dibUniqueItemData.width())
		return	0xffff;

	for (int i=0;i<dITEM_UNIQUE_DATA_COUNT;i++)
	{
		if (s_makeItem.m_aUniqueData[i].m_wEffect	==	0xffff)	break;

		char	*lpComment	=	s_makeItem.m_aUniqueData[i].getComment(TRUE);

		int		iLineCount	=	cTEXT::GetLines(iWidth,lpComment);

		if (_iY	>=	y	&&	_iY	<=	y+iLineCount*14+8)	return	i;

		y	+=	iLineCount*14;
		y	+=	8;
	}

	return	0xffff;
}

//
//	아이템 효과 선택
int
cSET_BASIC_ITEM::SelectGenerateData(int _iX,int _iY)
{
//	아이템 효과
	int	x =5,y=5;

	int	iWidth	=	s_dibItemGenerateData.width() - 14;

	if (_iX	<0	||	_iX	>	s_dibItemGenerateData.width())	return	0xffff;

	for (int i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if (s_makeItem.m_aGenerateData[i].m_wEffect	==	0xffff)	break;

		char	*lpComment	=	s_makeItem.getDataComment(i,TRUE);

		int		iLineCount	=	cTEXT::GetLines(iWidth,lpComment);

		if (_iY	>=	y	&&	_iY	<=	y+iLineCount*14+8)	return	i;

		y	+=	iLineCount*14;
		y	+=	8;
	}

	return	0xffff;
}	//	cSET_BASIC_ITEM::SelectGenerateData(int _iX,int _iY)

//
//	스크립트 문장(?)을 해석 한다.
BOOL
cSET_BASIC_ITEM::DecodeUniqueScript(HWND _hDlg)
{
	char		strScript[512];

	cDECODER	decoder;

	GetDlgItemText(_hDlg,IDC_SCRIPT,strScript,sizeof(strScript));

	if (strlen(strScript)	<=	80)
		return	cMSG::Error("아이템 정보 분석 에러!!","이 작업은 'Project  ‘붉은 보석’ 아이템.xls'\n\n에 유니크 아이템 세부 수치탭에서 한 라인을 그대로 복사해 넣어야 가능합니다.");

	decoder.UploadBuffer(strScript,NULL);

	char	*lpStream;
	int		iValidCount	=	0;
	int		iGrade		=	0;

	while(1)
	{	
		lpStream	=	decoder.GetStream('	');

		if (!lpStream)
		{
			iValidCount++;

			if (iValidCount	>=	20)	break;

			iGrade++;

			continue;
		}

		switch(iGrade)
		{
//			case	0	:	break;	//	인덱스 무시~
			case	1	:	//	이름
			{
				SetDlgItemText(_hDlg,IDC_NAME,lpStream);
				break;
			}

//			case	2	:	break;	//	영문 이름

			case	3	:	//	기본 아이템 이름
			{
				cBASIC_ITEM	*lpItem	=	cBASIC_ITEM::Get(lpStream);
				if (!lpItem)	return	FALSE;

				int	iSerial	=	0xffff;

				if (s_bEditItem)	iSerial	=	s_makeItem.m_iSerial;

				s_iEditItemKind	=	lpItem->m_wKind;

				s_makeItem.copy(lpItem);

				if (s_bEditItem)	s_makeItem.m_iSerial	=	iSerial;

				GetDlgItemText(_hDlg,IDC_NAME,s_makeItem.m_strName,sizeof(s_makeItem.m_strName));

				s_isInitializing	=	TRUE;
				FillItemAddBox(_hDlg);
				s_isInitializing	=	FALSE;
				break;
			}

			case	4	:	//	아이템 설명
				SetDlgItemText(_hDlg,IDC_COMMENT	,lpStream);
				break;

			case	5	:
			case	6	:
			case	7	:
			case	8	:
			case	9	:
			case	10	:
			{
				int	iIndex	=	atoi(lpStream);

				cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.getIndexByDiscernmentCode(iIndex);

				if (!lpPrefix)
					return	ERRMSG("유니크 아이템 스크립트 분석 에러!!","인식번호 %d를 갖는 접두사가 없습니다.",iIndex);

				lpStream	=	decoder.GetStream('	');

				int	iValue	=	atoi(lpStream);

				s_makeItem.m_aUniqueData[iGrade-5].m_wEffect	=	lpPrefix->m_wType;
				s_makeItem.m_aUniqueData[iGrade-5].m_aValue[0]	=	iValue;

				if (stricmp(lpStream,"max")	==	0)
					lpPrefix->getMaxValue(s_makeItem.m_aUniqueData[iGrade-5].m_aValue);

				lpStream	=	strstr(lpStream,"/");

				if (lpStream)
				{
					s_makeItem.m_aUniqueData[iGrade-5].m_aValue[0]	=	atoi(lpStream+1);
					s_makeItem.m_aUniqueData[iGrade-5].m_aValue[1]	=	iValue;
				}

				DrawUniqueData();
				break;
			}

			case	14	:
			{
				lpStream	=	strstr(lpStream,"/");
				DrawUniqueData();
				break;
			}

		}

		iGrade++;
	}

	return	TRUE;
}

//
//
BOOL
cSET_BASIC_ITEM::DecodeItemScript(HWND _hDlg)
{
	char		strScript[512];

	cDECODER	decoder;

	GetDlgItemText(_hDlg,IDC_SCRIPT,strScript,sizeof(strScript));

	if (strlen(strScript)	<=	80)
		return	cMSG::Error("아이템 정보 분석 에러!!","이 작업은 'Project  ‘붉은 보석’ 아이템.xls'\n\n에 아이템 세부 수치탭에서 한 라인을 그대로 복사해 넣어야 가능합니다.");

	decoder.UploadBuffer(strScript,NULL);

	char	*lpStream;
	int		iValidCount	=	0;
	int		iGrade		=	0;
	int		iMinValue1=0,iMinValue2=0,iMaxDamage=0;

	s_makeItem.reset();

	FillItemAddBox(_hDlg);

	while(1)
	{	
		lpStream	=	decoder.GetStream('	');

		if (!lpStream)
		{
			iValidCount++;

			if (iValidCount	>=	20)	break;

			iGrade++;

			continue;
		}

		switch(iGrade)
		{
//			case	0	:	break;	//	인덱스 무시~
			case	1	:	//	분류
			{
				int	iKind	=	cBASIC_ITEM::GetItemKind(lpStream);

				if (iKind	==	0xffff)
					return	cMSG::Error("아이템 정보 분석 에러!!","다음 아이템 분류를 찾을 수 없습니다.\n\n'%s'",lpStream);

				cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_CATEGORY),iKind);
				break;
			}

			case	2	:	//	이름
			{
				SetDlgItemText(_hDlg,IDC_NAME,lpStream);
				break;
			}
			
//			case	3	:	break;	//	영문 이름

			case	4	:	//	아이템 설명
				SetDlgItemText(_hDlg,IDC_COMMENT	,lpStream);
				break;

			case	5	:	//	최소 데미지
			{
				int	iValue	=	atoi(lpStream);

				SetDlgItemInt(_hDlg,IDC_MIN_DAMAGE,iValue,FALSE);
				break;
			}

			case	6	:	//	최고 데미지
			{
				int	iValue	=	atoi(lpStream);

				SetDlgItemInt(_hDlg,IDC_MAX_DAMAGE,iValue,FALSE);
				iMaxDamage	=	iValue;
				break;
			}

			case	7	:	//	수치1 최소값
			{
				int	iValue	=	atoi(lpStream);

				iMinValue1	=	iValue;
				SetDlgItemInt(_hDlg,IDC_VALUE1_1,iValue,FALSE);
				break;
			}

			case	8	:	//	수치1 최대값
			{
				int	iValue	=	atoi(lpStream);

				if(iValue	==	0)	iValue	=	iMinValue1;

				SetDlgItemInt(_hDlg,IDC_VALUE1_2,iValue,FALSE);
				break;
			}

			case	9	:	//	수치2 최소값
			{
				int	iValue	=	atoi(lpStream);

				iMinValue2	=	iValue;
				SetDlgItemInt(_hDlg,IDC_VALUE2_1,iValue,FALSE);
				break;
			}

			case	10	:	//	수치2 최대값
			{
				int	iValue	=	atoi(lpStream);

				if(iValue	==	0)	iValue	=	iMinValue2;

				SetDlgItemInt(_hDlg,IDC_VALUE2_2,iValue,FALSE);
				break;
			}

			case	11	:	//	공격 속도
			{
				int	iValue1,iValue2=0;

				iValue1	=	atoi(lpStream);

				lpStream=	strstr(lpStream,".");

				if (lpStream)	iValue2	=	atoi(lpStream+1);

				SetDlgItemText(_hDlg,IDC_SPEED,_ms("%d.%d",iValue1,iValue2));
				break;
			}

			case	12	:	//	사거리
			{
				int	iValue;

				iValue	=	atoi(lpStream);

				SetDlgItemInt(_hDlg,IDC_RANGE,iValue,FALSE);
				break;
			}

			case	13	:	//	데미지 범위
			{
				int	iValue;

				iValue	=	atoi(lpStream);

				SetDlgItemInt(_hDlg,IDC_DAMAGE_RANGE,iValue,FALSE);
				break;
			}

//			case	14	:	break;	//	요구 1
//			case	15	:	break;	//	요구 2

			case	16	:	//	가격
			{
				int	iValue	=	0;

				if (strstr(lpStream,"[최대 공격력]"))
				{
					iValue	=	atoi(lpStream);
					iValue	*=	iMaxDamage;

					SetDlgItemInt(_hDlg,IDC_PRICE,iValue,FALSE);
					cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_PRICE_COMPUTE_METHOD),0);
					break;
				}

				if (strstr(lpStream,"[수치1]*[수치2]/"))
				{
					lpStream	=	strstr(lpStream,"/");
					iValue		=	atoi(lpStream+1);

					SetDlgItemInt(_hDlg,IDC_PRICE,iValue,FALSE);
					cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_PRICE_COMPUTE_METHOD),5);
					break;
				}

				if (strstr(lpStream,"/10"))
				{
					iValue		=	atoi(lpStream);

					SetDlgItemInt(_hDlg,IDC_PRICE,iValue,FALSE);
					cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_PRICE_COMPUTE_METHOD),6);
					break;
				}

				if (strstr(lpStream,"[수치1]+1"))
				{
					iValue		=	atoi(lpStream);

					SetDlgItemInt(_hDlg,IDC_PRICE,iValue,FALSE);
					cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_PRICE_COMPUTE_METHOD),4);
					break;
				}

				if (strstr(lpStream,"[수치2]*"))
				{
					lpStream	=	strstr(lpStream,"*");
					iValue		=	atoi(lpStream+1);

					SetDlgItemInt(_hDlg,IDC_PRICE,iValue,FALSE);
					cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_PRICE_COMPUTE_METHOD),3);
					break;
				}

				if (strstr(lpStream,"[수치1]*"))
				{
					lpStream	=	strstr(lpStream,"*");
					iValue		=	atoi(lpStream+1);

					SetDlgItemInt(_hDlg,IDC_PRICE,iValue,FALSE);
					cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_PRICE_COMPUTE_METHOD),2);
					break;
				}

				if (strstr(lpStream,"[수치1]"))
				{
					cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_PRICE_COMPUTE_METHOD),1);
					break;
				}

				iValue		=	atoi(lpStream);

				SetDlgItemInt(_hDlg,IDC_PRICE,iValue,FALSE);
				cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_PRICE_COMPUTE_METHOD),0);

				break;
			}

			case	17	:	break;//	필드
			case	18	:	break;//	무기?
			case	19	:	break;//	아이콘
			case	20	:	break;//	비고

			case	21	:	//	드랍레벨
			{
				int	iValue		=	atoi(lpStream);

				SetDlgItemInt(_hDlg,IDC_DROP_LEVEL,iValue,FALSE);
				break;
			}
		}

		iGrade++;
	}

	SetDlgItemText(_hDlg,IDC_SCRIPT,"");

	return	TRUE;
}

//
//
BOOL
cSET_BASIC_ITEM::CopyItemData(HWND _hDlg)
{
	char	strItem[64];

	GetDlgItemText(_hDlg,IDC_SCRIPT,strItem,sizeof(strItem));

	cBASIC_ITEM	*lpBasicItem	=	cBASIC_ITEM::Get(strItem);

	if (!lpBasicItem)
		return ERRMSG("Error!", "There is no item named '%s'", strItem);

	s_makeItem.reset();

	strcpy(strItem,s_makeItem.m_strName);
	memcpy(&s_makeItem,lpBasicItem,sizeof(cBASIC_ITEM));
	strcpy(s_makeItem.m_strName,strItem);

	FillItemAddBox(_hDlg);
	DrawAddItemWnd(_hDlg);

	return	TRUE;
}

BOOL
cSET_BASIC_ITEM::CopyBaseItemData(HWND _hDlg)
{
	int	iBaseItem	=	cCOMBOBOX::GetData(GetDlgItem(_hDlg,IDC_BASE_ITEM));

	if (iBaseItem	==	0xffff)
		return	cMSG::Put("Error!","Please select an item first.");

	char	strItem[64];

	GetDlgItemText(_hDlg,IDC_NAME,strItem,sizeof(strItem));

	int	iSerial	=	s_makeItem.m_iSerial;
	s_makeItem.reset();
	s_makeItem.copy(&g_aBasicItem[iBaseItem]);
	s_makeItem.m_iSerial	=	iSerial;
	strcpy(s_makeItem.m_strName,strItem);
	s_makeItem.m_wBaseItem	=	iBaseItem;

	FillItemAddBox(_hDlg);
	DrawAddItemWnd(_hDlg);

	return	TRUE;
}

//
//
LRESULT CALLBACK 
cSET_BASIC_ITEM::s_procAddBasicItem(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	
		case WM_INITDIALOG	:
			InitItemAddBox(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,FALSE);
			break;

		case WM_COMMAND :
		{
			switch( LOWORD( wParam ) )
            {
				case	IDC_CHANGE_UNIQUE_DATA			:		//	09.07.30 추가... 유니크 아이템옵션수정.
				{
					int	iIndex	=	s_iSelectUniqueData;
					
					if	(s_makeItem.m_aUniqueData[iIndex].m_wEffect	==	0xffff)
						break;
					
					if	(!cPREFIX_EFFECT::AddEffect(hDlg,(cITEM_EFFECT*)&s_makeItem.m_aUniqueData[iIndex]))
						break;

					memcpy(&s_makeItem.m_aUniqueData[iIndex],&cPREFIX_EFFECT::s_Effect,sizeof(cITEM_EFFECT));
					
					DrawUniqueData();
					break;
				}

				case IDC_UNIQUE_DATA_UP:						// 09.07.27 유니크옵션 이동
				{
					if	(s_iSelectUniqueData == 0)
						return (0L);

					cITEM_EFFECT ItemData;

					memcpy(&ItemData,&s_makeItem.m_aUniqueData[s_iSelectUniqueData],sizeof(cITEM_EFFECT));
					memcpy(&s_makeItem.m_aUniqueData[s_iSelectUniqueData],&s_makeItem.m_aUniqueData[s_iSelectUniqueData-1],sizeof(cITEM_EFFECT));
					memcpy(&s_makeItem.m_aUniqueData[s_iSelectUniqueData-1],&ItemData,sizeof(cITEM_EFFECT));
					--s_iSelectUniqueData;
					DrawUniqueData();
					break;
				}

				case IDC_UNIQUE_DATA_DOWN:						// 09.07.27 유니크옵션 이동
				{
					if	(s_iSelectUniqueData == s_makeItem.getValidUniqueDataIndex() - 1)
						return (0L);

					if	(s_iSelectUniqueData == dITEM_UNIQUE_DATA_COUNT - 1)
						return (0L);

					cITEM_EFFECT ItemData;
				
					memcpy(&ItemData,&s_makeItem.m_aUniqueData[s_iSelectUniqueData],sizeof(cITEM_EFFECT));
					memcpy(&s_makeItem.m_aUniqueData[s_iSelectUniqueData],&s_makeItem.m_aUniqueData[s_iSelectUniqueData+1],sizeof(cITEM_EFFECT));
					memcpy(&s_makeItem.m_aUniqueData[s_iSelectUniqueData+1],&ItemData,sizeof(cITEM_EFFECT));
					++s_iSelectUniqueData;
					DrawUniqueData();
					break;
				}

				case	IDC_COPY_ITEM_DATA		:	//09.08.12
					CopyItemData(hDlg);
					SetCheckDataList(hDlg);
					break;

				case	IDC_COPY_BASE_ITEM_DATA	:		//09.08.12
					CopyBaseItemData(hDlg);
					SetCheckDataList(hDlg);
					break;

				case	IDC_PROCESS_UNIQUE_DATA	:	//09.08.12
					DecodeUniqueScript(hDlg);
					SetCheckDataList(hDlg);
					break;

				case	IDC_PROCESS_ITEM_DATA	:	//09.08.12
					DecodeItemScript(hDlg);
					SetCheckDataList(hDlg);
					break;

				case	IDC_CHECK_ALL		:
				{
					for (int i=0;;i++)
					{
						if	(g_aiEnableJobIDCList[i]==-1)
							break;
						CheckDlgButton(hDlg,g_aiEnableJobIDCList[i],TRUE);
					}
					break;
				}

				case	IDC_DECHECK_ALL		:
				{
					for (int i=0;;i++)
					{
						if	(g_aiEnableJobIDCList[i]==-1)
							break;
						CheckDlgButton(hDlg,g_aiEnableJobIDCList[i],FALSE);
					}
					break;
				}

				case	IDOK				:
					if (GetItemAddBoxData(hDlg))	EndDialog(hDlg,TRUE);
					break;

				case	IDCANCEL			:
					EndDialog(hDlg,FALSE);
					break;

				case	IDC_VALUE1_1		:
				case	IDC_VALUE1_2		:
				case	IDC_VALUE2_1		:
				case	IDC_VALUE2_2		:
				{
					switch (HIWORD(wParam))
                    {
						case	EN_CHANGE	:
						{
							BOOL	temp;

							if (s_isInitializing)	break;

							s_makeItem.m_aValue[0][0]	=	GetDlgItemInt(hDlg,IDC_VALUE1_1	,&temp,FALSE);	
							s_makeItem.m_aValue[0][1]	=	GetDlgItemInt(hDlg,IDC_VALUE1_2	,&temp,FALSE);	
							s_makeItem.m_aValue[1][0]	=	GetDlgItemInt(hDlg,IDC_VALUE2_1	,&temp,FALSE);	
							s_makeItem.m_aValue[1][1]	=	GetDlgItemInt(hDlg,IDC_VALUE2_2	,&temp,FALSE);	

							s_makeItem.m_aValue[0][1]	=	max(s_makeItem.m_aValue[0][0],s_makeItem.m_aValue[0][1]);
							s_makeItem.m_aValue[1][1]	=	max(s_makeItem.m_aValue[1][0],s_makeItem.m_aValue[1][1]);

							DrawItemGerateData();
							break;
						}
					}
					break;
				}


				case	IDC_CHANGE_ICON_SHAPE	:
					if (SelectItemShape(hDlg,dITEM_SHAPE_KIND_ICON))
					{
						s_makeItem.m_wIconShape	=	s_iSelectItemShape;
						DrawAddItemWnd(hDlg);
					}
					break;

				case	IDC_CHANGE_FIELD_SHAPE	:
					if (SelectItemShape(hDlg,dITEM_SHAPE_KIND_FIELD))
					{
						s_makeItem.m_wFieldShape	=	s_iSelectItemShape;
						DrawAddItemWnd(hDlg);
					}
					break;

				case	IDC_ADD_UNIQUE_DATA			:		//	유티크 아이템 효과 추가
				{
					int	iIndex	=	s_makeItem.getValidUniqueDataIndex();

					if (iIndex	==	0xffff)	
					{	ERRMSG(dMSG_ERROR_ADD_ITEM_EFFECT,dMSG_MAX_ITEM_EFFECT_COUNT);
						break;
					}

					if	(!cPREFIX_EFFECT::AddEffect(hDlg))
						break;

					memcpy(&s_makeItem.m_aUniqueData[iIndex],&cPREFIX_EFFECT::s_Effect,sizeof(cITEM_EFFECT));
					s_iSelectUniqueData	=	iIndex;

					DrawUniqueData();
					break;
				}

				case	IDC_REMOVE_UNIQUE_DATA			:	//	유티크 아이템 효과 제거
				{
					if (s_makeItem.m_aUniqueData[s_iSelectUniqueData].m_wEffect	==	0xffff)	break;

					if (cMSG::YESNO(dMSG_REALLY_REMOVE_FOLLOW_ITEM_EFFECT,s_makeItem.m_aUniqueData[s_iSelectUniqueData].getComment(FALSE))	==	IDNO)
						break;

					for (int i=s_iSelectUniqueData;i<dITEM_UNIQUE_DATA_COUNT-1;i++)
						memcpy(s_makeItem.m_aUniqueData+i,s_makeItem.m_aUniqueData+i+1,sizeof(cITEM_EFFECT));

					s_makeItem.m_aUniqueData[dITEM_UNIQUE_DATA_COUNT-1].reset();

					if (s_makeItem.m_aUniqueData[s_iSelectUniqueData].m_wEffect	==	0xffff	&&	s_iSelectUniqueData	>0)
						s_iSelectUniqueData--;

					DrawUniqueData();
					break;
				}

				case	IDC_ADD_GENERATE_DATA			:		//	아이템 효과
				{
					int	iIndex	=	s_makeItem.getValidGenerateDataIndex();

					if (iIndex	==	0xffff)	
					{	ERRMSG(dMSG_ERROR_ADD_ITEM_EFFECT,dMSG_MAX_ITEM_EFFECT_COUNT);
						break;
					}

//					if (!cPREFIX_EFFECT::AddEffect(hDlg,dIE_RECOVER))	break;

					if (!cITEM_GENERATE_EFFECT::AddEffect(hDlg,&s_makeItem))	break;

					memcpy(&s_makeItem.m_aGenerateData[iIndex],&cITEM_GENERATE_EFFECT::s_itemData,sizeof(cITEM_GENERATE_DATA));
					s_iSelectGerateData	=	iIndex;

					DrawItemGerateData();
					break;
				}

				case	IDC_REMOVE_GENERATE_DATA		:		//	아이템 효과
				{
					if (s_makeItem.m_aGenerateData[s_iSelectGerateData].m_wEffect	==	0xffff)	break;

					if (cMSG::YESNO(dMSG_REALLY_REMOVE_FOLLOW_ITEM_EFFECT,s_makeItem.getDataComment(s_iSelectGerateData))	==	IDNO)
						break;

					for (int i=s_iSelectGerateData;i<dGENERATE_ITEM_DATA_COUNT-1;i++)
						memcpy(s_makeItem.m_aGenerateData+i,s_makeItem.m_aGenerateData+i+1,sizeof(cITEM_GENERATE_DATA));

					s_makeItem.m_aGenerateData[dGENERATE_ITEM_DATA_COUNT-1].reset();

					if (s_makeItem.m_aGenerateData[s_iSelectGerateData].m_wEffect	==	0xffff	&&	s_iSelectGerateData	>0)	s_iSelectGerateData--;

					DrawItemGerateData();
					break;
				}

				case IDC_BT_CHECK_DATA_VIEW :		//09.08.07
					
					DialogBox(MAIN.hINST,"IDD_DLG_CHECK_DATA",hDlg,(DLGPROC)s_procItemCheckDataWnd);
					break;

			}
			break;
		}
	}

	return (0L);
}	//	cSET_BASIC_ITEM::s_procAddBasicItem(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)

//
//	
LRESULT CALLBACK
cSET_BASIC_ITEM::s_procItemShapeWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibIconShape.Active();
			s_dibIconShape.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}


//
//	
LRESULT CALLBACK
cSET_BASIC_ITEM::s_procItemGerateDataWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibItemGenerateData.Active();
			s_dibItemGenerateData.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_LBUTTONDOWN	:
		{
			int item		=	SelectGenerateData(LOWORD(lParam),HIWORD(lParam));

			if	(item		==	0xffff)	break;

			s_iSelectGerateData	=	item;

			DrawItemGerateData();
			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}

//
//	
LRESULT CALLBACK
cSET_BASIC_ITEM::s_procUniqueItemDataWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibUniqueItemData.Active();
			s_dibUniqueItemData.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_LBUTTONDOWN	:
		{
			int item		=	SelectUniqueData(LOWORD(lParam),HIWORD(lParam));

			if	(item		==	0xffff)
				break;

			s_iSelectUniqueData	=	item;

			DrawUniqueData();
			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}

//	
//
LRESULT CALLBACK
cSET_BASIC_ITEM::s_procFieldShapeWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibFieldShape.Active();
			s_dibFieldShape.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}

//	
//
LRESULT CALLBACK
cSET_BASIC_ITEM::s_procEquippedShapeWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibEquippedShape.Active();
			s_dibEquippedShape.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	아이템 모양 선택
///////////////////////////////////////////////////////////////////////////////////////////////////////////

cDIBWND		cSET_BASIC_ITEM::s_dibItemShapeList;
int			cSET_BASIC_ITEM::s_iSelectShapeKind		=	0;
int			cSET_BASIC_ITEM::s_iSelectItemShape		=	0;
cSCROLL_BAR	cSET_BASIC_ITEM::s_scrollItemShapeList;
int			cSET_BASIC_ITEM::s_iItemWidth;
int			cSET_BASIC_ITEM::s_iItemHeight;
int			cSET_BASIC_ITEM::s_iItemShapeListWidth;
int			cSET_BASIC_ITEM::s_iItemShapeListHeight;
int			cSET_BASIC_ITEM::s_iISPW;
int			cSET_BASIC_ITEM::s_iISPH;
int			cSET_BASIC_ITEM::s_iItemShapeLineCount	=	0;
int			cSET_BASIC_ITEM::s_iItemCount			=	0;

//
//	아이템의 표현되는 모양을 선택한다.
BOOL
cSET_BASIC_ITEM::SelectItemShape(HWND _hWnd,int _iSelectShapeKind)
{
	s_iSelectShapeKind	=	_iSelectShapeKind;

	switch	(s_iSelectShapeKind)
	{
		case	dITEM_SHAPE_KIND_ICON		:
			s_iSelectItemShape	=	s_makeItem.m_wIconShape;
			break;

		case	dITEM_SHAPE_KIND_FIELD		:
			s_iSelectItemShape	=	s_makeItem.m_wFieldShape;
			break;
	}

	if (DialogBox(MAIN.hINST,"SELECT_ITEM_SHAPE",_hWnd,(DLGPROC)s_procSelectItemShape)	==	IDOK)
		return	TRUE;

	return	FALSE;
}

//
//	아이템 모양 선택 초기화
void
cSET_BASIC_ITEM::InitItemShapeList(HWND _hDlg)
{
	HWND	hItemShapeListWnd	=	GetDlgItem(_hDlg,IDC_ITEM_LIST);

	s_dibItemShapeList.lpszRegister		=	"Red Stone Item Shape List DibWnd";
	s_dibItemShapeList.Init(hItemShapeListWnd	,(WNDPROC)s_procItemShapeListWnd);

	s_scrollItemShapeList.Init(hItemShapeListWnd);		//	스크롤바 초기화
	s_scrollItemShapeList.InitHeight();		//	스크롤바 초기화

//	s_scrollItemShapeList.SetHeight(3,2);		//	스크롤바 초기화

	switch(s_iSelectShapeKind)
	{
		case	dITEM_SHAPE_KIND_ICON		:
			s_iItemWidth	=	dICON_SHAPE_HEIGHT;
			s_iItemHeight	=	dICON_SHAPE_WIDTH;
			s_iItemCount	=	g_smiIconItem.m_iCount;
			break;

		case	dITEM_SHAPE_KIND_FIELD		:
			s_iItemWidth	=	dICON_SHAPE_HEIGHT*2;
			s_iItemHeight	=	dICON_SHAPE_HEIGHT*2;
			s_iItemCount	=	g_sprFieldShape.Count;
			break;
	}

	s_iItemShapeListWidth	=	s_dibItemShapeList.width();			//	아이템 리스트 너비
	s_iItemShapeListHeight	=	s_dibItemShapeList.height();		//	아이템 리스트 높이

	s_iISPW	=	(s_iItemShapeListWidth -4)/(s_iItemWidth +4);
	s_iISPH	=	(s_iItemShapeListHeight-4)/(s_iItemHeight+4);

	if (s_iItemCount%s_iISPW	==	0)
			s_iItemShapeLineCount	=	s_iItemCount/s_iISPW;
	else	s_iItemShapeLineCount	=	s_iItemCount/s_iISPW+1;

	s_scrollItemShapeList.SetHeight(s_iItemShapeLineCount,s_iISPH);		//	스크롤바 초기화

	DrawItemShapeList(_hDlg);
}

//
//	아이템 모양 선택
//
int
cSET_BASIC_ITEM::SelectItemShape(int _iX,int _iY)
{
	int		x,y,iWidth,iHeight,iIndex	=	0;

	iIndex	=	s_scrollItemShapeList.m_iHeightPos	*	s_iISPW;

	x	=	(s_iItemShapeListWidth -(s_iISPW*(s_iItemWidth +4)))/2;
	y	=	(s_iItemShapeListHeight-(s_iISPH*(s_iItemHeight+4)))/2;

	for (iHeight=0;iHeight<s_iISPH;iHeight++)
	{
		x	=	(s_iItemShapeListWidth -(s_iISPW*(s_iItemWidth +4)))/2;

		for (iWidth=0;iWidth<s_iISPW;iWidth++)
		{
			if (iIndex	>=	s_iItemCount)	return	0xffffffff;

			if (_iX >= x && _iX <= x+s_iItemWidth +2 && _iY >= y && _iY <= y+s_iItemHeight +2)
				return	iIndex;

			x	+=	(s_iItemWidth +4);

			iIndex++;
		}

		y	+=	(s_iItemHeight +4);
	}

	return	0xffffffff;
}


//
//	아이템 모양 리스트를 그려 줍니다.
//
void
cSET_BASIC_ITEM::DrawItemShapeList(HWND _hDlg)
{
	int		x,y,iWidth,iHeight,iIndex	=	0;
	BOOL	bOut	=	FALSE;

	iIndex	=	s_scrollItemShapeList.m_iHeightPos	*	s_iISPW;

	x	=	(s_iItemShapeListWidth -(s_iISPW*(s_iItemWidth +4)))/2;
	y	=	(s_iItemShapeListHeight-(s_iISPH*(s_iItemHeight+4)))/2;

	s_dibItemShapeList.Active();

	cDRAW::Fill(0);

	for (iHeight=0;iHeight<s_iISPH;iHeight++)
	{
		x	=	(s_iItemShapeListWidth -(s_iISPW*(s_iItemWidth +4)))/2;

		for (iWidth=0;iWidth<s_iISPW;iWidth++)
		{

			if (iIndex	==	s_iSelectItemShape)	x++,y++;

			if (iIndex	>=	s_iItemCount)
			{
				bOut	=	TRUE;
				break;
			}
			char	strText[32] = "";
			sprintf(strText,"%d", iIndex	);
			switch(s_iSelectShapeKind)
			{
				case	dITEM_SHAPE_KIND_ICON		:
					g_smiIconItem.put(iIndex,x+1,y+1);
					cTEXT::FitPut(x+1,y+2,s_iItemWidth,s_iItemHeight,BTBLUE,strText);
					break;

				case	dITEM_SHAPE_KIND_FIELD		:
					g_sprFieldShape.FitPut(x+1,y+1,s_iItemWidth,s_iItemHeight,iIndex);
					cTEXT::FitPut(x+1,y+2,s_iItemWidth,s_iItemHeight,BTBLUE,strText);
					break;
			}

			if (iIndex	==	s_iSelectItemShape)
					cDRAW::Box(_LTGREEN,x,y,x+s_iItemWidth +2,y+s_iItemHeight +2);
			else	cDRAW::Box(_WHITE,x,y,x+s_iItemWidth +2,y+s_iItemHeight +2);

			if (iIndex	==	s_iSelectItemShape)	x--,y--;

			iIndex++;

			x	+=	(s_iItemWidth +4);
		}

		if (bOut)	break;

		y	+=	(s_iItemHeight +4);
	}

	s_dibItemShapeList.Draw();
}
LRESULT CALLBACK
cSET_BASIC_ITEM::s_procSelectItemShape(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	case WM_INITDIALOG	:
			InitItemShapeList(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,FALSE);
			break;

		case WM_COMMAND :
		{
			switch( LOWORD( wParam ) )
            {
				case	IDOK						:
					EndDialog(hDlg,TRUE);
					break;

				case	IDCANCEL					:
					EndDialog(hDlg,FALSE);
					break;

			}
			break;
		}
	}

	return (0L);
}

//
//	아이템 모양 리스트 윈도우
LRESULT CALLBACK
cSET_BASIC_ITEM::s_procItemShapeListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibItemShapeList.Active();
			s_dibItemShapeList.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_LBUTTONDOWN	:
		{
			int item		=	SelectItemShape(LOWORD(lParam),HIWORD(lParam));

			if	(item		==	0xffffffff)	break;

			s_iSelectItemShape	=	item;

			DrawItemShapeList(hwnd);

			break;
		}

		case WM_VSCROLL			:
		{
			if (s_scrollItemShapeList.VScroll(wParam))	DrawItemShapeList(hwnd);
			break;
		}


		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}


void
cSET_BASIC_ITEM::GetItemCheckData(HWND _hDlg)
{


	s_makeItem.m_wIsIDPublicItem									=	IsDlgButtonChecked(_hDlg,IDC_ID_PUBLIC_ITEM);
	s_makeItem.m_attr.isDXItem										=	IsDlgButtonChecked(_hDlg,IDC_DX_ITEM					);
	s_makeItem.m_bf1IsUltimate										=	IsDlgButtonChecked(_hDlg,IDC_ULTRA_ITEM				);
	s_makeItem.m_attr.isGuildBattleItem								=	IsDlgButtonChecked(_hDlg,IDC_GUILD_BATTLE_ITEM		);
	s_makeItem.m_attr.isBeginnerItem								=	IsDlgButtonChecked(_hDlg,IDC_BEGINNER_ITEM			);
	s_makeItem.m_attr.isExtraItem									=	IsDlgButtonChecked(_hDlg,IDC_IS_EXTRA_ITEM			);
	s_makeItem.m_attr.isBadge										=	IsDlgButtonChecked(_hDlg,IDC_IS_BADGE				);
	s_makeItem.m_bf1IsEternalItem									=	IsDlgButtonChecked(_hDlg,IDC_IS_ETERNAL				);		// 09.08.25
	s_makeItem.m_attr.isCanNotTradeItem								=	IsDlgButtonChecked(_hDlg,IDC_CAN_NOT_TRADE);
	s_makeItem.m_attr.isCanNotDropItem								=	IsDlgButtonChecked(_hDlg,IDC_CAN_NOT_DROPPING);
	s_makeItem.m_attr.isCanNotDestroyItem							=	IsDlgButtonChecked(_hDlg,IDC_CAN_NOT_DESTROY_ITEM	);
	s_makeItem.m_attr.isCanNotUseableItem							=	IsDlgButtonChecked(_hDlg,IDC_CAN_NOT_USEABLE_ITEM	);
	s_makeItem.m_attr.isCanNotStoreBank								=	IsDlgButtonChecked(_hDlg,IDC_CAN_NOT_STORE_TO_BANK	);
	s_makeItem.m_attr.isCanNotSelllItem								=	IsDlgButtonChecked(_hDlg,IDC_CAN_NOT_SELL);
	s_makeItem.m_bf1IsBlockToFeedPet								=	IsDlgButtonChecked(_hDlg,IDC_CAN_NOT_FEED_TO_PET	);
	s_makeItem.m_attr.isUseAbleBulletSubstituteArrow				=	IsDlgButtonChecked(_hDlg,IDC_IS_USE_BULLET			);
	s_makeItem.m_attr.isDurabilityToUseCount						=	IsDlgButtonChecked(_hDlg,IDC_DURABILITY_TO_USE_COUNT);

	s_makeItem.m_attr.isUseToItem									=	IsDlgButtonChecked(_hDlg,IDC_USE_TO_ITEM);
	s_makeItem.m_attr.isCanUseCorpseStatus							=	IsDlgButtonChecked(_hDlg,IDC_CAN_USE_CORPSE_STATUS	);
	s_makeItem.m_attr.isUseToCorpse									=	IsDlgButtonChecked(_hDlg,IDC_USE_TO_CORPSE			);
	s_makeItem.m_attr.isUseToCharacter								=	IsDlgButtonChecked(_hDlg,IDC_USE_TO_CHARACTER);
	s_makeItem.m_attr.isUseToMiniPet								=	IsDlgButtonChecked(_hDlg,IDC_USE_TO_MINI_PET);
	s_makeItem.m_bf1IsUseOnlyGuildMaster							=	IsDlgButtonChecked(_hDlg,IDC_USE_ONLY_GUILD_MASTER	);
	s_makeItem.m_bf1IsBuyOnlyGuildMaster							=	IsDlgButtonChecked(_hDlg,IDC_BUY_ONLY_GUILD_MASTER	);
	s_makeItem.m_bf1IsItemNotUsePet									=	IsDlgButtonChecked(_hDlg,IDC_ITEM_NOT_USE_PET);		//09.09.04

	s_makeItem.m_attr.isEquipableToBelt								=	IsDlgButtonChecked(_hDlg,IDC_EQUIPABLE_TO_BELT		);	
	s_makeItem.m_attr.isEffectInInventoryItem						=	IsDlgButtonChecked(_hDlg,IDC_IS_EFFECT_IN_INVENTORY	);
	s_makeItem.m_bf1IsDestroyWhenMoveField							=	IsDlgButtonChecked(_hDlg,IDC_EXPIRED_BY_MOVE_FIELD);
	s_makeItem.m_attr.isCanNotOwnSameItem							=	IsDlgButtonChecked(_hDlg,IDC_IGNORE_SAME_ITEM		);
	s_makeItem.m_bIsExceptionItem									=	IsDlgButtonChecked(_hDlg,IDC_EXCEPTION_ITEM);
	s_makeItem.m_bf1IsOccurEatPotionEffect							=	IsDlgButtonChecked(_hDlg,IDC_OCCUR_EAT_EFFECT);

	s_makeItem.m_attr.isRequireSaveUseLog							=	IsDlgButtonChecked(_hDlg,IDC_REMAIN_USE_LOG			);
	s_makeItem.m_bf1IsRequireLogWithGuildInfo						=	IsDlgButtonChecked(_hDlg,IDC_ADD_GUILD_INFO_REMAIN_GAIN_LOG);

	s_makeItem.m_bf1IsUseAbleByThrowPotionSkill						=	IsDlgButtonChecked(_hDlg,IDC_IS_POTION	);
	s_makeItem.m_bf1IsUseAbleByThrowDrugSkill						=	IsDlgButtonChecked(_hDlg,IDC_IS_DRUG		);
	s_makeItem.m_bf1IsUseAbleByThrowFlowerSkill						=	IsDlgButtonChecked(_hDlg,IDC_IS_FLOWER	);
	s_makeItem.m_bf1IsUseAbleByThrowCandySkill						=	IsDlgButtonChecked(_hDlg,IDC_IS_CANDY	);

	s_makeItem.m_attr.isMagicCarpetEmblem							=	IsDlgButtonChecked(_hDlg,IDC_IS_CARPET_EMBLEM		);
	s_makeItem.m_attr.isMergeToCarpetEmblem							=	IsDlgButtonChecked(_hDlg,IDC_IS_MERGE_CARPET_EMBLEM	);
	s_makeItem.m_bf1IsRequestSummonCarpet							=	IsDlgButtonChecked(_hDlg,IDC_REQUEST_SUMMON_CARPET	);
	s_makeItem.m_bf1IsCanSummonMagicCarpet							=	IsDlgButtonChecked(_hDlg,IDC_CAN_SUMMON_CARPET	);
	s_makeItem.m_bf1RemeberPlace0									=	IsDlgButtonChecked(_hDlg,IDC_REMEBER_PLACE_0);
	s_makeItem.m_bf1RemeberPlace1									=	IsDlgButtonChecked(_hDlg,IDC_REMEBER_PLACE_1);
	s_makeItem.m_bf1RemeberPlace2									=	IsDlgButtonChecked(_hDlg,IDC_REMEBER_PLACE_2);
	s_makeItem.m_bf1IsFreePastPortal								=	IsDlgButtonChecked(_hDlg,IDC_FREE_PAST_PORTAL	);
	s_makeItem.m_bf1IsHalfPastPortal								=	IsDlgButtonChecked(_hDlg,IDC_HALF_PAST_PORTAL	);
	s_makeItem.m_bf1IsFreeTeleport									=	IsDlgButtonChecked(_hDlg,IDC_FREE_TELEPORT	);
	s_makeItem.m_bf1IsAddGetMysticStone								=	IsDlgButtonChecked(_hDlg,IDC_MYSTIC_STONE_ADD_GAIN);
	s_makeItem.m_bf1IsAddGetTantalissRelic							=	IsDlgButtonChecked(_hDlg,IDC_TANTILIS_RELIC_ADD_GAIN);
	s_makeItem.m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile		=	IsDlgButtonChecked(_hDlg,IDC_IGNORE_GUILD_HALL_LEVEL_FOR_ENTRY_TANTALIS_EXILE);
	s_makeItem.m_bf1IsInfinitySeriousUpgradeChanceUp				=	IsDlgButtonChecked(_hDlg,IDC_INCREASE_INFINITY_SERIOUS_UPGRADE_CHANCE);
	s_makeItem.m_bf1EternalItemUpgradeNotPenalty					=	IsDlgButtonChecked(_hDlg,IDC_ETERNAL_ITEM_UPGRADE_NOT_PENALTY);		//09.08.25
	s_makeItem.m_bf1IsUpgradeMaterials								=	IsDlgButtonChecked(_hDlg,IDC_UPGRADE_MATERIALS_ITEM);		//09.08.26
	s_makeItem.m_bf1IsUseShop										=	IsDlgButtonChecked(_hDlg,IDC_USE_SHOP);		// 상점 이용하기
	s_makeItem.m_bf1IsUseBank										=	IsDlgButtonChecked(_hDlg,IDC_USE_BANK);		// 은행 이용하기
	s_makeItem.m_bf1IsNotConsume									=	IsDlgButtonChecked(_hDlg,IDC_IS_NOT_CONSUME);		// 아이템 소모 안됨
	s_makeItem.m_bf1IsUseItemAfterTransformation 					=	IsDlgButtonChecked(_hDlg,IDC_USE_ITEM_AFTER_TRANSFORMATION);		// 아이템 소모 안됨
	s_makeItem.m_bf1IsRingOfInfinity 								=	IsDlgButtonChecked(_hDlg,IDC_IS_RING_OF_INFINITY);		// 무한의 반지
	s_makeItem.m_bf1IsUseToEnemy									=	IsDlgButtonChecked(_hDlg,IDC_USE_TO_ENEMY);			// 적에게 사용
	s_makeItem.m_bf1IsUseAfterWear									=	IsDlgButtonChecked(_hDlg,IDC_USE_AFTER_WEAR);		// 착용 후 기능 사용
	s_makeItem.m_bf1IsUsePremiumInventory							=	IsDlgButtonChecked(_hDlg,IDC_USE_PREMIUM_INVENTORY);		//  유료아이템 전용 인벤토리 사용
	s_makeItem.m_bf1IsNotApplyCopyEffect							=	IsDlgButtonChecked(_hDlg,IDC_IS_NOT_APPLY_COPY_EFFECT);				// 복사 효과 적용안됨
	s_makeItem.m_bf1IsNotApplyReversionEffect						=	IsDlgButtonChecked(_hDlg,IDC_IS_NOT_APPLY_REVERSION_EFFECT);			// 거래불가해제 효과 적용안됨
	s_makeItem.m_bf1IsNotGetUsedSkill								=	IsDlgButtonChecked(_hDlg,IDC_IS_NOT_GET_USED_SKILL);			// 스킬로 줍기 불가
	s_makeItem.m_bf1IsNotUseToGVG									=	IsDlgButtonChecked(_hDlg,IDC_IS_NOT_USE_TO_GVG);			// GVG에서 사용불가

}
void
cSET_BASIC_ITEM::InitCheckData(HWND _hDlg)	
{


	CheckDlgButton(_hDlg,IDC_ID_PUBLIC_ITEM,s_makeItem.m_wIsIDPublicItem);
	CheckDlgButton(_hDlg,IDC_DX_ITEM,s_makeItem.m_attr.isDXItem);
	CheckDlgButton(_hDlg,IDC_ULTRA_ITEM,s_makeItem.m_bf1IsUltimate);
	CheckDlgButton(_hDlg,IDC_GUILD_BATTLE_ITEM,s_makeItem.m_attr.isGuildBattleItem);
	CheckDlgButton(_hDlg,IDC_BEGINNER_ITEM,s_makeItem.m_attr.isBeginnerItem);
	CheckDlgButton(_hDlg,IDC_IS_EXTRA_ITEM,s_makeItem.m_attr.isExtraItem);
	CheckDlgButton(_hDlg,IDC_IS_BADGE,s_makeItem.m_attr.isBadge);
	CheckDlgButton(_hDlg,IDC_IS_ETERNAL,s_makeItem.m_bf1IsEternalItem);		//09.08.25

	CheckDlgButton(_hDlg,IDC_CAN_NOT_TRADE,s_makeItem.m_attr.isCanNotTradeItem);
	CheckDlgButton(_hDlg,IDC_CAN_NOT_DROPPING,s_makeItem.m_attr.isCanNotDropItem);
	CheckDlgButton(_hDlg,IDC_CAN_NOT_DESTROY_ITEM,s_makeItem.m_attr.isCanNotDestroyItem);
	CheckDlgButton(_hDlg,IDC_CAN_NOT_USEABLE_ITEM,s_makeItem.m_attr.isCanNotUseableItem);
	CheckDlgButton(_hDlg,IDC_CAN_NOT_STORE_TO_BANK,s_makeItem.m_attr.isCanNotStoreBank);
	CheckDlgButton(_hDlg,IDC_CAN_NOT_SELL,s_makeItem.m_attr.isCanNotSelllItem);
	CheckDlgButton(_hDlg,IDC_CAN_NOT_FEED_TO_PET,s_makeItem.m_bf1IsBlockToFeedPet);
	CheckDlgButton(_hDlg,IDC_IS_USE_BULLET,s_makeItem.m_attr.isUseAbleBulletSubstituteArrow);
	CheckDlgButton(_hDlg,IDC_DURABILITY_TO_USE_COUNT,s_makeItem.m_attr.isDurabilityToUseCount);

	CheckDlgButton(_hDlg,IDC_USE_TO_ITEM,s_makeItem.m_attr.isUseToItem);
	CheckDlgButton(_hDlg,IDC_CAN_USE_CORPSE_STATUS,s_makeItem.m_attr.isCanUseCorpseStatus);
	CheckDlgButton(_hDlg,IDC_USE_TO_CORPSE,s_makeItem.m_attr.isUseToCorpse);
	CheckDlgButton(_hDlg,IDC_USE_TO_CHARACTER,s_makeItem.m_attr.isUseToCharacter);
	CheckDlgButton(_hDlg,IDC_USE_TO_MINI_PET,s_makeItem.m_attr.isUseToMiniPet);
	CheckDlgButton(_hDlg,IDC_USE_ONLY_GUILD_MASTER,s_makeItem.m_bf1IsUseOnlyGuildMaster);
	CheckDlgButton(_hDlg,IDC_BUY_ONLY_GUILD_MASTER,s_makeItem.m_bf1IsBuyOnlyGuildMaster);
	CheckDlgButton(_hDlg,IDC_ITEM_NOT_USE_PET,s_makeItem.m_bf1IsItemNotUsePet); // 09.09.04

	CheckDlgButton(_hDlg,IDC_EQUIPABLE_TO_BELT,s_makeItem.m_attr.isEquipableToBelt);
	CheckDlgButton(_hDlg,IDC_IS_EFFECT_IN_INVENTORY,s_makeItem.m_attr.isEffectInInventoryItem);
	CheckDlgButton(_hDlg,IDC_EXPIRED_BY_MOVE_FIELD,s_makeItem.m_bf1IsDestroyWhenMoveField);
	CheckDlgButton(_hDlg,IDC_IGNORE_SAME_ITEM,s_makeItem.m_attr.isCanNotOwnSameItem);
	CheckDlgButton(_hDlg,IDC_EXCEPTION_ITEM,s_makeItem.m_bIsExceptionItem);
	CheckDlgButton(_hDlg,IDC_OCCUR_EAT_EFFECT,s_makeItem.m_bf1IsOccurEatPotionEffect);

	CheckDlgButton(_hDlg,IDC_REMAIN_USE_LOG,s_makeItem.m_attr.isRequireSaveUseLog);
	CheckDlgButton(_hDlg,IDC_ADD_GUILD_INFO_REMAIN_GAIN_LOG,s_makeItem.m_bf1IsRequireLogWithGuildInfo);

	CheckDlgButton(_hDlg,IDC_IS_POTION	,s_makeItem.m_bf1IsUseAbleByThrowPotionSkill);
	CheckDlgButton(_hDlg,IDC_IS_DRUG		,s_makeItem.m_bf1IsUseAbleByThrowDrugSkill);
	CheckDlgButton(_hDlg,IDC_IS_FLOWER	,s_makeItem.m_bf1IsUseAbleByThrowFlowerSkill);
	CheckDlgButton(_hDlg,IDC_IS_CANDY	,s_makeItem.m_bf1IsUseAbleByThrowCandySkill);

	CheckDlgButton(_hDlg,IDC_IS_CARPET_EMBLEM,s_makeItem.m_attr.isMagicCarpetEmblem);
	CheckDlgButton(_hDlg,IDC_IS_MERGE_CARPET_EMBLEM,s_makeItem.m_attr.isMergeToCarpetEmblem);
	CheckDlgButton(_hDlg,IDC_REQUEST_SUMMON_CARPET,s_makeItem.m_bf1IsRequestSummonCarpet);
	CheckDlgButton(_hDlg,IDC_CAN_SUMMON_CARPET,s_makeItem.m_bf1IsCanSummonMagicCarpet);
	CheckDlgButton(_hDlg,IDC_REMEBER_PLACE_0,s_makeItem.m_bf1RemeberPlace0);
	CheckDlgButton(_hDlg,IDC_REMEBER_PLACE_1,s_makeItem.m_bf1RemeberPlace1);
	CheckDlgButton(_hDlg,IDC_REMEBER_PLACE_2,s_makeItem.m_bf1RemeberPlace2);
	CheckDlgButton(_hDlg,IDC_FREE_PAST_PORTAL,s_makeItem.m_bf1IsFreePastPortal);
	CheckDlgButton(_hDlg,IDC_HALF_PAST_PORTAL,s_makeItem.m_bf1IsHalfPastPortal);
	CheckDlgButton(_hDlg,IDC_FREE_TELEPORT,s_makeItem.m_bf1IsFreeTeleport);
	CheckDlgButton(_hDlg,IDC_MYSTIC_STONE_ADD_GAIN,s_makeItem.m_bf1IsAddGetMysticStone);
	CheckDlgButton(_hDlg,IDC_TANTILIS_RELIC_ADD_GAIN,s_makeItem.m_bf1IsAddGetTantalissRelic);
	CheckDlgButton(_hDlg,IDC_IGNORE_GUILD_HALL_LEVEL_FOR_ENTRY_TANTALIS_EXILE,s_makeItem.m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile);
	CheckDlgButton(_hDlg,IDC_INCREASE_INFINITY_SERIOUS_UPGRADE_CHANCE,s_makeItem.m_bf1IsInfinitySeriousUpgradeChanceUp);
	CheckDlgButton(_hDlg,IDC_ETERNAL_ITEM_UPGRADE_NOT_PENALTY,s_makeItem.m_bf1EternalItemUpgradeNotPenalty); // 09.08.25
	CheckDlgButton(_hDlg,IDC_UPGRADE_MATERIALS_ITEM,s_makeItem.m_bf1IsUpgradeMaterials); // 09.08.26
	CheckDlgButton(_hDlg,IDC_USE_SHOP,s_makeItem.m_bf1IsUseShop); //	상점 이용하기
	CheckDlgButton(_hDlg,IDC_USE_BANK,s_makeItem.m_bf1IsUseBank); //	은행 이용하기
	CheckDlgButton(_hDlg,IDC_IS_NOT_CONSUME,s_makeItem.m_bf1IsNotConsume); 	// 아이템 소모 안됨
	CheckDlgButton(_hDlg,IDC_USE_ITEM_AFTER_TRANSFORMATION,s_makeItem.m_bf1IsUseItemAfterTransformation);		// 몬스터 변신후사용
	CheckDlgButton(_hDlg,IDC_IS_RING_OF_INFINITY,s_makeItem.m_bf1IsRingOfInfinity);		// 무한의반지
	CheckDlgButton(_hDlg,IDC_USE_TO_ENEMY,s_makeItem.m_bf1IsUseToEnemy);	// 적에게 사용
	CheckDlgButton(_hDlg,IDC_USE_AFTER_WEAR,s_makeItem.m_bf1IsUseAfterWear);	// 착용 후 기능 사용
	CheckDlgButton(_hDlg,IDC_USE_PREMIUM_INVENTORY,s_makeItem.m_bf1IsUsePremiumInventory);	//  유료아이템 전용 인벤토리 사용
	CheckDlgButton(_hDlg,IDC_IS_NOT_APPLY_COPY_EFFECT,s_makeItem.m_bf1IsNotApplyCopyEffect);	// 복사 효과 적용안됨
	CheckDlgButton(_hDlg,IDC_IS_NOT_APPLY_REVERSION_EFFECT,s_makeItem.m_bf1IsNotApplyReversionEffect);	// 거래불가해제 효과 적용안됨
	CheckDlgButton(_hDlg,IDC_IS_NOT_GET_USED_SKILL,s_makeItem.m_bf1IsNotGetUsedSkill);		// 스킬로 줍기 불가
	CheckDlgButton(_hDlg,IDC_IS_NOT_USE_TO_GVG,s_makeItem.m_bf1IsNotUseToGVG);				// GVG에서 사용불가
}

void
cSET_BASIC_ITEM::SetCheckDataList(HWND _hDlg)					// 09.08.07
{

	HWND hListBox = GetDlgItem(_hDlg,IDC_LB_CHECK_DATA);
	cLISTBOX::Reset(hListBox);
	if(s_makeItem.m_wIsIDPublicItem){
		cLISTBOX::Add(hListBox,"Account Sharing");
	}

	if(s_makeItem.m_attr.isDXItem){
		cLISTBOX::Add(hListBox,"DX");
	}

	if(s_makeItem.m_bf1IsUltimate){
		cLISTBOX::Add(hListBox,"Ultimate");
	}

	if(s_makeItem.m_attr.isGuildBattleItem){
		cLISTBOX::Add(hListBox,"Guild War");
	}

	if(s_makeItem.m_attr.isBeginnerItem){
		cLISTBOX::Add(hListBox,"For Beginners");
	}

	if(s_makeItem.m_attr.isExtraItem){
		cLISTBOX::Add(hListBox,"Extra");
	}

	if(s_makeItem.m_attr.isBadge){
		cLISTBOX::Add(hListBox,"Badge");
	}

	if(s_makeItem.m_bf1IsEternalItem){		// 09.08.25
		cLISTBOX::Add(hListBox,"Eternal");
	}

	if(s_makeItem.m_attr.isCanNotTradeItem){
		cLISTBOX::Add(hListBox,"Trade not possible");
	}

	if(s_makeItem.m_attr.isCanNotDropItem){
		cLISTBOX::Add(hListBox,"Cannot be discarded");
	}
	
	if(s_makeItem.m_attr.isCanNotDestroyItem){
		cLISTBOX::Add(hListBox,"Indestructible");
	}
	
	if(s_makeItem.m_attr.isCanNotUseableItem){
		cLISTBOX::Add(hListBox,"Not available");
	}
	
	if(s_makeItem.m_attr.isCanNotStoreBank){
		cLISTBOX::Add(hListBox,"No bank storage");
	}

	if(s_makeItem.m_attr.isCanNotSelllItem){
		cLISTBOX::Add(hListBox,"No store sales");
	}

	if(s_makeItem.m_bf1IsBlockToFeedPet){
		cLISTBOX::Add(hListBox,"Pet feeding not allowed");
	}
								
	if(s_makeItem.m_attr.isUseAbleBulletSubstituteArrow){
		cLISTBOX::Add(hListBox,"Use bullets instead of arrows");
	}
	
	if(s_makeItem.m_attr.isDurabilityToUseCount){
		cLISTBOX::Add(hListBox,"Durability -> Number of uses");
	}
	
	if(s_makeItem.m_attr.isUseToItem){
		cLISTBOX::Add(hListBox,"Use for item");
	}
	
	if(s_makeItem.m_attr.isCanUseCorpseStatus){
		cLISTBOX::Add(hListBox,"Use after death");
	}

	if(s_makeItem.m_attr.isUseToCorpse){
		cLISTBOX::Add(hListBox,"Use on corpses");
	}
	
	if(s_makeItem.m_attr.isUseToCharacter){
		cLISTBOX::Add(hListBox,"Use for character");
	}
	
	if(s_makeItem.m_attr.isUseToMiniPet){
		cLISTBOX::Add(hListBox,"Use for minipets");
	}
	
	if(s_makeItem.m_bf1IsUseOnlyGuildMaster){
		cLISTBOX::Add(hListBox,"Only Gilma can be used");
	}
								
	if(s_makeItem.m_bf1IsBuyOnlyGuildMaster){
		cLISTBOX::Add(hListBox,"Only Gilma can be purchased");
	}							

	if(s_makeItem.m_bf1IsItemNotUsePet){				// 09.09.04
		cLISTBOX::Add(hListBox,"Pets/summons cannot be used");
	}	
	
	
	if (s_makeItem.m_attr.isEquipableToBelt) {
		cLISTBOX::Add(hListBox, "Can be worn on belt");
	}
	if (s_makeItem.m_attr.isEffectInInventoryItem) {
		cLISTBOX::Add(hListBox, "Exercise effect in inventory");
	}
	if (s_makeItem.m_bf1IsDestroyWhenMoveField) {
		cLISTBOX::Add(hListBox, "Destroy when moving field");
	}
	if (s_makeItem.m_attr.isCanNotOwnSameItem) {
		cLISTBOX::Add(hListBox, "Ignore same items");
	}
	if (s_makeItem.m_bIsExceptionItem) {
		cLISTBOX::Add(hListBox, "Exception Item");
	}

	if (s_makeItem.m_bf1IsOccurEatPotionEffect) {
		cLISTBOX::Add(hListBox, "Eating potion effect occurs");
	}

	if (s_makeItem.m_attr.isRequireSaveUseLog) {
		cLISTBOX::Add(hListBox, "Leave usage log");
	}
	if (s_makeItem.m_bf1IsRequireLogWithGuildInfo) {
		cLISTBOX::Add(hListBox, "Add guild information to log");
	}

	if (s_makeItem.m_bf1IsUseAbleByThrowPotionSkill) {
		cLISTBOX::Add(hListBox, "Potion");
	}
	if (s_makeItem.m_bf1IsUseAbleByThrowDrugSkill) {
		cLISTBOX::Add(hListBox, "Treatment");
	}
	if (s_makeItem.m_bf1IsUseAbleByThrowFlowerSkill) {
		cLISTBOX::Add(hListBox, "flower");
	}
	if (s_makeItem.m_bf1IsUseAbleByThrowCandySkill) {
		cLISTBOX::Add(hListBox, "candy, sweets, cake");
	}

	if (s_makeItem.m_attr.isMagicCarpetEmblem) {
		cLISTBOX::Add(hListBox, "Carpet Emblem");
	}
	if (s_makeItem.m_attr.isMergeToCarpetEmblem) {
		cLISTBOX::Add(hListBox, "Merge into carpet emblem when used");
	}
	if (s_makeItem.m_bf1IsRequestSummonCarpet) {
		cLISTBOX::Add(hListBox, "Carpet Summon Request");
	}
	if (s_makeItem.m_bf1IsCanSummonMagicCarpet) {
		cLISTBOX::Add(hListBox, "Carpet can be summoned");
	}
	if (s_makeItem.m_bf1RemeberPlace0) {
		cLISTBOX::Add(hListBox, "Place Memory");
	}
	if (s_makeItem.m_bf1RemeberPlace1) {
		cLISTBOX::Add(hListBox, "Place Memory 1");
	}
	if (s_makeItem.m_bf1RemeberPlace2) {
		cLISTBOX::Add(hListBox, "Place Memory 8");
	}
	if (s_makeItem.m_bf1IsFreePastPortal) {
		cLISTBOX::Add(hListBox, "Free Fast Portal");
	}
	if (s_makeItem.m_bf1IsHalfPastPortal) {
		cLISTBOX::Add(hListBox, "Half Price Fast Portal");
	}
	if (s_makeItem.m_bf1IsFreeTeleport) {
		cLISTBOX::Add(hListBox, "Free Town Teleport");
	}
	if (s_makeItem.m_bf1IsAddGetMysticStone) {
		cLISTBOX::Add(hListBox, "Acquire additional secret stones");
	}
	if (s_makeItem.m_bf1IsAddGetTantalissRelic) {
		cLISTBOX::Add(hListBox, "Acquire additional Tantilis artifacts");
	}
	if (s_makeItem.m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile) {
		cLISTBOX::Add(hListBox, "Enter the guild dungeon without restrictions");
	}
	if (s_makeItem.m_bf1IsInfinitySeriousUpgradeChanceUp) {
		cLISTBOX::Add(hListBox, "Increased probability of Eternal Series evolution");
	}

	if (s_makeItem.m_bf1EternalItemUpgradeNotPenalty) { // 09.08.25
		cLISTBOX::Add(hListBox, "Remove eternal item enhancement penalty");
	}
	if (s_makeItem.m_bf1IsUpgradeMaterials) { // 09.08.26
		cLISTBOX::Add(hListBox, "Enhancer");
	}
	if (s_makeItem.m_bf1IsUseShop) { // Use the store
		cLISTBOX::Add(hListBox, "Use the store");
	}
	if (s_makeItem.m_bf1IsUseBank) { // Use bank
		cLISTBOX::Add(hListBox, "Use Bank");
	}

	if (s_makeItem.m_bf1IsNotConsume) // "Not consumed when used"
		cLISTBOX::Add(hListBox, "Not consumed when used");

	if (s_makeItem.m_bf1IsUseItemAfterTransformation) // Use after monster transformation
		cLISTBOX::Add(hListBox, "Use after transforming monster");

	if (s_makeItem.m_bf1IsRingOfInfinity) // Ring of Infinity
		cLISTBOX::Add(hListBox, "Infinity Ring");

	if (s_makeItem.m_bf1IsUseToEnemy) // Use on enemies
		cLISTBOX::Add(hListBox, "Use on enemies");

	if (s_makeItem.m_bf1IsUseAfterWear) // Use function after wear
		cLISTBOX::Add(hListBox, "Enable function after wearing");

	if (s_makeItem.m_bf1IsUsePremiumInventory) // Use inventory only for paid items
		cLISTBOX::Add(hListBox, "Use inventory only for paid items");

	if (s_makeItem.m_bf1IsNotApplyCopyEffect) // Copy effect not applied
		cLISTBOX::Add(hListBox, "Copy effect not applied");

	if (s_makeItem.m_bf1IsNotApplyReversionEffect) // Transaction cancellation effect is not applied
		cLISTBOX::Add(hListBox, "Trade disablement effect not applied");

	if (s_makeItem.m_bf1IsNotGetUsedSkill) //Cannot be picked up with skill
		cLISTBOX::Add(hListBox, "Cannot be picked up with skills");

	if (s_makeItem.m_bf1IsNotUseToGVG) // Cannot be used in GVG
		cLISTBOX::Add(hListBox, "Not available in GVG");

}
LRESULT CALLBACK
cSET_BASIC_ITEM::s_procItemCheckDataWnd(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)		// 09.08.07
{
	switch(message)
	{	
	case WM_INITDIALOG	:
		InitCheckData(hDlg);
		
		break;
		
	case WM_CLOSE		:
		EndDialog(hDlg,FALSE);
		break;
		
	case WM_COMMAND :
		{
			switch( LOWORD( wParam ) )
            {
			case	IDOK :
				GetItemCheckData(hDlg);
				SetCheckDataList(GetParent(hDlg));
				EndDialog(hDlg,TRUE);
				break;
			}
			break;
		}
	}
	
	return (0L);
	
}
