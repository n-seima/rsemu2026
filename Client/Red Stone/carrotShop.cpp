#include "CWindowInterface.H"
#include "CGamePlayInterface.H"
#include "CActor.H"
#include "CMessage.H"

cCarrotShop		g_gwCarrotShop;
cCarrotShopInfo	g_carrotShop;

#define	dSHOP_ITEM_HEIGHT				40
#define	dSHOP_INTERFACE_TITLE_HEIGHT	6
#define	dSHOP_INTERFACE_BOTTOM			26
#define	dSHOP_INTERFACE_HEIGHT_GAP		(dSHOP_INTERFACE_TITLE_HEIGHT+dSHOP_INTERFACE_BOTTOM+6)

#define dSHOP_TOPCATE_BTN_MARGIN		5	// 최상위카테고리버튼과 border이미지 사이 공백
#define dSHOP_TOPCATE_BTN_BYMARGIN		5	// 최상위카테고리 버튼 사이 공백
#define dSHOP_DIVIDELINE_YMARGIN		2	// 구분선이미지 위 아래로 들어갈 공백 
#define dSHOP_WND_LEFTMARGIN			20	// 당근상점 왼쪽공백 

#define dSHOP_SECCATE_BTNXMARGIN		7	// 두번째 카테고리 버튼 사이 공백 ([] <-> [])
#define dSHOP_SECCATE_1LINE_CATENUM		5	// 두번째 카테고리에서 전체,인기 뒤 한줄에 출력될 카테고리 수

// ┌─────────────┐
// │┌─┐  아이템이름        │
// ││  │                    │
// │└─┘ 당근이미지 당근갯수│ 
// └─────────────┘
int iLargeBoxXMargin = 5;	// 큰박스 사이 X간격 
int iLargeBoxYMargin = 5;	// 큰박스 사이 Y간격 
int iSmallBoxMargin  = 5;	// 큰박스와 작은박스 사이 공백
int iBoxinMargin	 = 3;	// 박스안 쓰이는 공백 
int iOneLineItemNum  = 3;	// 한줄에 표시될 아이템 갯수
int iLineNum		 = 4;	// 몇줄을 출력할 것인지?

int iPutItemCount = 0;	// 몇개를 출력했는지 저장해두는 변수, 버튼의 위치정보를 저장할 때 배열의 인덱스값을 위해 쓰임 


enum
{
	eCSWI_CARROT	=	6,
		eCSWI_TITLE		=	67,
		eCSWI_FIRST_BAR	=	72,
};
void
cCarrotShopInfo::reset()
{
	m_wCategoryCount	=	0;
	memset(this,0,sizeof(cCarrotShopDefine));
}

int
cCarrotShopInfo::getCategoryItemCount(int _iCategory)
{
	cCarrotShopCategoryInfoDefine	*lpCategory	=	&m_aData[_iCategory];
	
	int	iCount	=	0;
	
	for (int i=0;i<lpCategory->m_wItemCount;i++)
	{
		if	(lpCategory->m_aItemList[i].m_bf1IsTestItem && g_bIsTestServer == FALSE)
			continue;
		
		iCount++;
	}
	
	return	iCount;
}

void
cCarrotShopInfo::updateInfo(int _iCategory,cCarrotShopCategoryInfo1 *_lpData,cCarrotShopCategoryInfo2 *_lpCategoryInfo)
{
	cCarrotShopCategoryInfoDefine	*lpCategory	=	&m_aData[_iCategory];
	
	memcpy(lpCategory,_lpData,sizeof(cCarrotShopCategoryInfo1));
	memcpy(lpCategory->getCategoryInfo(),_lpCategoryInfo,sizeof(cCarrotShopCategoryInfo2));
	
	memset(lpCategory->m_aItemList+lpCategory->m_wItemCount,0,sizeof(cCarrotShopItemDefine)*(dMAX_CARROT_SHOP_ITEM_COUNT-lpCategory->m_wItemCount));
}

void
cCarrotShopInfo::load()
{
	cFILE	file;
	
	if	(!file.Open("data/carrotShop.dat","rb"))
		return;
	
	file.Read(this,sizeof(cCarrotShopInfo));

	file.Close();
// insu add
#ifndef _DEBUG
  	if (!g_bIsTestServer)
  	{
		for(int i=0 ; i<g_carrotShop.m_wCategoryCount ; i++)
		{
			cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[i];
			if (lpCategory->m_bf1IsTestCategory)
			{
				m_wCategoryCount--;
				for(int j=i ; j<=g_carrotShop.m_wCategoryCount ; j++)
					g_carrotShop.m_aData[j] = g_carrotShop.m_aData[j+1];
			}

			for (int j=0 ; j<lpCategory->m_wItemCount ; j++)
			{
				if (lpCategory->m_aItemList[j].m_bf1IsTestItem)
				{
					lpCategory->m_wItemCount--;
					for (int k=j ; k<=lpCategory->m_wItemCount ; k++)
						lpCategory->m_aItemList[k] = lpCategory->m_aItemList[k+1];
					j--;
				}
			}
		}
  	}
#endif
// insu add end
}

void
cCarrotShopInfo::save()
{
	// 	{
	// 		cFILE	file;
	// 
	// 		if	(!file.Open("data/carrotShop.dat","rb"))
	// 			return;
	// 
	// 		cCarrotShopInfo	carrot;
	// 
	// 		file.Read(&carrot,sizeof(cCarrotShopInfo));
	// 
	// 		file.Close();
	// 
	// 		for (int i=0;i<dMAX_CARROT_SHOP_CATEGORY_COUNT;i++)
	// 		{
	// 			BYTE*	lpCurCate	=	(BYTE*)&m_aData[i];
	// 			BYTE*	lpSavedCate	=	(BYTE*)&carrot.m_aData[i];
	// 			
	// 			for (int j=0;j<sizeof(cCarrotShopCategoryInfoDefine);j++)
	// 			{
	// 				if	(lpCurCate[j] != lpSavedCate[j] )
	// 					j	=	j;
	// 			}
	// 		}
	// 	}
	
	cFILE	file;
	
	if	(!file.Open("data/carrotShop.dat","wb"))
		return;
	
	file.Write(this,sizeof(cCarrotShopInfo));
	
	file.Close();
}

void
cCarrotShop::open()
{	
#ifndef _DEBUG	
	// 테스트 카테고리를 삭제한다.
	if (!g_bIsTestServer)
	{
		for(int i=0 ; i<g_carrotShop.m_wCategoryCount ; i++)
		{
			cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[i];			
			if (lpCategory->m_bf1IsTestCategory)
			{
				g_carrotShop.m_wCategoryCount--;
				for(int j=i ; j<=g_carrotShop.m_wCategoryCount ; j++)
					g_carrotShop.m_aData[j] = g_carrotShop.m_aData[j+1];
			}
			// 테스트 아이템 삭제
			for (int j=0 ; j<lpCategory->m_wItemCount ; j++)
			{
				if (lpCategory->m_aItemList[j].m_bf1IsTestItem)
				{
					lpCategory->m_wItemCount--;
					for (int k=j ; k<=lpCategory->m_wItemCount ; k++)
						lpCategory->m_aItemList[k] = lpCategory->m_aItemList[k+1];
					j--;
				}
			}
		}
	}	
#endif

	m_binputState = true;
	
	m_iSelectItem		=	-1;	//	구입을 위해 선택한 아이템
	m_iFocusItem		=	-1;	//	포커스가 간 아이템
//	m_iBuyItem			=	-1;
	m_iScrollBarPos		=	0;
	m_iCurrentCategory	=	0;
	m_wIsWaitResult		=	FALSE;
	
	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();
	
	s_lpInventoryWindowEquipmentList =	g_hero.m_aEquip;
	s_lpInventoryWindowItemList		 =	g_hero.m_aItems;
	s_lpGold						 =	&g_hero.m_iGold;
	s_iPopupInterface				 =	ePIW_CARROT_SHOP;

	if(!g_config.b_bIsNewCarrotShop)
		openOld();
	
	if(g_config.b_bIsNewCarrotShop)
		openNew();
}


void
cCarrotShop::openOld()
{
	for (int i=g_carrotShop.m_wCategoryCount-1;i>=0;i--)
		if	(g_carrotShop.m_aData[i].m_bf1IsTestCategory == FALSE || g_bIsTestServer)
			m_iCurrentCategory	=	i;			
		
	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_SHOP);
		
	m_rect.x1				=	g_iScreenWidth-iWindowWidth*2;
	m_rect.y1				=	iTitleHeight+g_iCorrectBI_Y/2;
	m_rect.x2				=	g_iScreenWidth-iWindowWidth;
	m_rect.y2				=	iTitleHeight+iWindowHeight+g_iCorrectBI_Y/2;
	
	s_bmMiddleWindowButton.reset();
	
	s_bmMiddleWindowButton.addImageBar(dMSG_BUY,IDOK		,m_rect.x1+197,471+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.setClickMargin(IDOK,1,1);
	s_bmMiddleWindowButton.setAllBarTextColor(0,FALSE);
	
	s_sbMiddleWindow.set(m_rect.x1+iWindowWidth-16,m_rect.y1+dSHOP_INTERFACE_TITLE_HEIGHT,iWindowHeight-dSHOP_INTERFACE_HEIGHT_GAP);
	s_sbMiddleWindow.setPos(m_iScrollBarPos);
	
	memset(m_aCategoryInfo,0,sizeof(m_aCategoryInfo));
	
	SetInventoryWindowPos();
	InitPopupWindowCloseButton();
	return;
}



// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃              모든 카테고리의 아이템 갯수를 리턴한다.               ┃
// ┃                                                                    ┃
// ┃ BOOL _bNew  : 참이면 신규아이템의 갯수만을 리턴한다.               ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
int
cCarrotShop::getAllCategoryItemCount(BOOL _bNew)
{
	cCarrotShopCategoryInfoDefine	*lpCategory;
	int iResult = 0;

	for (int i=0 ; i<g_carrotShop.m_wCategoryCount ; i++)
	{
		lpCategory	=	&g_carrotShop.m_aData[i];

		if (_bNew)
		{
			for(int j=0 ; j< lpCategory->m_wItemCount ; j++)
			{
				if(lpCategory->m_aItemList[j].m_bf1IsHotItem)
					iResult++;
			}
		}

		if (!_bNew)
			iResult	+=	lpCategory->m_wItemCount;
	}

	return iResult;
}



void
cCarrotShop::InitScrollBar()
{
	// --------------- 스크롤바 관련 
	if	(!m_sbScrollBar.m_lpSprite)	//	초기화가 안되었으면 초기화 해라
		m_sbScrollBar.init(&g_sprInterface, dIMAGE_SCROLL_BAR_VERTICAL,
		eSCROLLBAR_VERTICAL_NORMAL_UP_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_UP_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_UP_BUTTON,0xffff,
		eSCROLLBAR_VERTICAL_NORMAL_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_DOWN_BUTTON,0xffff,
		eSCROLLBAR_VERTICAL_NORMAL_BODY_TOP,	eSCROLLBAR_VERTICAL_NORMAL_BODY,		eSCROLLBAR_VERTICAL_NORMAL_BODY_BOTTOM,
		eSCROLLBAR_VERTICAL_ACTIVE_BODY_TOP,	eSCROLLBAR_VERTICAL_ACTIVE_BODY,		eSCROLLBAR_VERTICAL_ACTIVE_BODY_BOTTOM);
	
	
	// 초기값은 무조건 전체아이템 이므로 이것만큼 스크롤바를 설정해준다.
	int iScrollValue = (m_iTotalItemCount%iOneLineItemNum)? m_iTotalItemCount/iOneLineItemNum+1 : m_iTotalItemCount/iOneLineItemNum;
	m_sbScrollBar.setSize(iScrollValue, 4);
	m_sbScrollBar.setPos(0);
	
	// 스크롤바가 화면에 위치할 좌표관련 설정
	int itempY=0;
	if(g_carrotShop.m_wCategoryCount > 0 && g_carrotShop.m_wCategoryCount < dSHOP_SECCATE_1LINE_CATENUM) 
		itempY = iPutPosY+110-(g_sprInterface2.getSpriteHeight(eCARROT_SECSBTN1NORMAL) + dSHOP_DIVIDELINE_YMARGIN*2);
	else if(g_carrotShop.m_wCategoryCount > dSHOP_SECCATE_1LINE_CATENUM && g_carrotShop.m_wCategoryCount < dSHOP_SECCATE_1LINE_CATENUM*2+4)
		itempY = iPutPosY+110;
	else
		itempY = iPutPosY+110+(g_sprInterface2.getSpriteHeight(eCARROT_SECSBTN1NORMAL) + dSHOP_DIVIDELINE_YMARGIN*2);
	
	m_sbScrollBar.set(iPutPosX+g_sprInterface2.getSpriteWidth(eCARROT_MAINWND_TITLE)-g_sprInterface.getSpriteWidth(eSCROLLBAR_VERTICAL_NORMAL_UP_BUTTON)-3,	itempY,	305);
	
	if	(!m_sbSearchScrollBar.m_lpSprite)	//	초기화가 안되었으면 초기화 해라
		m_sbSearchScrollBar.init(&g_sprInterface, dIMAGE_SCROLL_BAR_VERTICAL,
		eSCROLLBAR_VERTICAL_NORMAL_UP_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_UP_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_UP_BUTTON,0xffff,
		eSCROLLBAR_VERTICAL_NORMAL_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_DOWN_BUTTON,0xffff,
		eSCROLLBAR_VERTICAL_NORMAL_BODY_TOP,	eSCROLLBAR_VERTICAL_NORMAL_BODY,		eSCROLLBAR_VERTICAL_NORMAL_BODY_BOTTOM,
		eSCROLLBAR_VERTICAL_ACTIVE_BODY_TOP,	eSCROLLBAR_VERTICAL_ACTIVE_BODY,		eSCROLLBAR_VERTICAL_ACTIVE_BODY_BOTTOM);
	
	m_sbSearchScrollBar.set(iPutPosX + (g_sprInterface2.getSpriteWidth(eCARROT_TOPBTN_NORMAL)+dSHOP_TOPCATE_BTN_BYMARGIN)*4 + g_sprInterface2.getSpriteWidth(eCARROT_MAGNIFIER) + dSHOP_TOPCATE_BTN_BYMARGIN*3 + dSHOP_WND_LEFTMARGIN +g_sprInterface2.getSpriteWidth(eCARROT_SEARCHBOX) - g_sprInterface.getSpriteWidth(eSCROLLBAR_VERTICAL_NORMAL_UP_BUTTON),
		iPutPosY+40, g_sprInterface2.getSpriteHeight(eCARROT_SEARCHBOX_RESULT)+2);
}


void
cCarrotShop::close()
{
	ClearSearch(false, true, false, true);
	m_inputSearch.close();
	
	cINPDEV::ReleaseExclusive();	
	
	s_iPopupInterface	=	ePIW_NONE;
	s_rectInterfaceSize.set(0, 0, 0, 0);
	m_iTotalPayCarrot = 0;
	ClosePopupWindow();
	/*// existing code
	cINPDEV::ReleaseExclusive();
	
	  CloseInventoryWindow();
	  
		s_iPopupInterface	=	ePIW_NONE;
	*/
}
BOOL
cCarrotShop::isOpened()
{
	if	(s_iPopupInterface		==	ePIW_CARROT_SHOP)
		return	TRUE;
	
	return	FALSE;
}


void
cCarrotShop::openNew()
{
	cCarrotShopCategoryInfoDefine	*lpCategory;
	cItem 							item;
	m_iTopCategoryCnt		= 4;		// 최상위 카테고리수 4
	
	m_iSelectTopCategory	= 0;	// .. 초기값은 아이템구매 카테고리로 ..
	m_iSelectSecCategory	= 0;
	
	m_iSelectSecCategory	= 0;
	m_iFocusSecCategory		= 0;
	
	m_iWishListCount		= 0;
	m_iTotalPayCarrot		= 0;
	
	m_iSearchItemIndexCount = 0xffff;	
	m_iSearchCount			= 0;
	m_bSelectSearchBoxItem	= false;
	m_iNewItemCount			= getAllCategoryItemCount(TRUE);	// 신규아이템 갯수
	m_iTotalItemCount		= getAllCategoryItemCount();		// 전체아이템 갯수 
	m_bShowResultState		= false;
	m_iFocusItemBoxIndex	= 0xffff;
	m_iWindowWidth			= g_sprInterface2.getSpriteWidth(eCARROT_MAINWND_TITLE);
	m_iWindowHeight			= g_sprInterface2.getSpriteHeight(eCARROT_MAINWND_TITLE)+g_sprInterface2.getSpriteHeight(eCARROT_MAINWND_BORDER);
	iPutPosX				= (g_iScreenWidth/2) - (m_iWindowWidth/2);
	iPutPosY				= ((g_iScreenHeight-g_sprGameBottomInterface.getSpriteHeight(0))/2) - (m_iWindowHeight/2);
	
	int iRandomHotItem 		= random(m_iNewItemCount);
	int iCnt 				= 0;
	
	// 윈도우 영역좌표 설정.. 마우스입력을 막을 좌표
	s_rectInterfaceSize.set(iPutPosX, iPutPosY, iPutPosX+m_iWindowWidth, iPutPosY+m_iWindowHeight);
	
	// --------------- 에디트박스 관련 
	m_inputSearch.init(s_hWnd, s_hInst, &s_text, 1);
	m_inputSearch.set(	iPutPosX + (g_sprInterface2.getSpriteWidth(eCARROT_TOPBTN_NORMAL)+dSHOP_TOPCATE_BTN_BYMARGIN)*4 + g_sprInterface2.getSpriteWidth(eCARROT_MAGNIFIER) + dSHOP_TOPCATE_BTN_BYMARGIN*3 + dSHOP_WND_LEFTMARGIN, // 박스찍히는 x좌표 
		iPutPosY+g_sprInterface2.getSpriteHeight(eCARROT_MAINWND_TITLE)+dSHOP_DIVIDELINE_YMARGIN+5,		// 박스찍히는 y좌표
		0, 
		iPutPosX + (g_sprInterface2.getSpriteWidth(eCARROT_TOPBTN_NORMAL)+dSHOP_TOPCATE_BTN_BYMARGIN)*4 + g_sprInterface2.getSpriteWidth(eCARROT_MAGNIFIER) + dSHOP_TOPCATE_BTN_BYMARGIN*3 + dSHOP_WND_LEFTMARGIN+2, // 박스찍히는 x좌표 
		iPutPosY+g_sprInterface2.getSpriteHeight(eCARROT_MAINWND_TITLE)+dSHOP_DIVIDELINE_YMARGIN+5+2,		// 글자가 찍히는 위치(보통 위와 같음)
		g_sprInterface2.getSpriteWidth(eCARROT_SEARCHBOX)-4, g_sprInterface2.getSpriteHeight(eCARROT_SEARCHBOX)-4,	// 에디트박스 넓이, 높이 
		dITEM_NAME_LENGTH, "");				// 문자열 길이제한 
	
	m_inputSearch.setColor(BLACK, WHITE);	// (글자색상, 바탕색상)
	
	strcpy(m_strRecommendItemName, dMSG_CARROTSHOP_RECOMMENDSEARCH);
	for( int i=0 ; i<g_carrotShop.m_wCategoryCount ; i++)
	{
		BOOL bbreak = FALSE;
		lpCategory	=	&g_carrotShop.m_aData[i];
		for(int j=0 ; j< lpCategory->m_wItemCount ; j++)
		{
			if(lpCategory->m_aItemList[j].m_bf1IsHotItem)
			{
				if(iCnt == iRandomHotItem)
				{
					CCustomItem	*lpCustomItem	=	g_premiumItem.getItem(lpCategory->m_aItemList[j].m_wPremiunItemIndex);
					if(!lpCustomItem->generateItem(&item))
						return;
					cBasicItem *lpBaseItem = item.getBasicItem();

					memset(m_strRecommendItemName, '\0', dITEM_NAME_LENGTH);
					strcpy(m_strRecommendItemName, dMSG_CARROTSHOP_RECOMMENDSEARCH);

					strcat(m_strRecommendItemName, lpBaseItem->m_strName);
					m_inputSearch.setStr(0, m_strRecommendItemName);
					bbreak = TRUE;
					break;
				}
				else
					iCnt++;
			}
		}
		if (bbreak)
			break;
	}
	
	InitScrollBar();
		
	memset(m_aCategoryInfo,0,sizeof(m_aCategoryInfo));	
}


int
cCarrotShop::getFocusItem()
{
	int		iX,iY,iCount	=	0;
	int		iLineCount		=	0;
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iCategoryBarHeight	=	g_sprInterface2.getSpriteHeight(eCSWI_FIRST_BAR);
	
	iX		=	m_rect.x1;
	iY		=	m_rect.y1;
	
	iY		+=	dSHOP_INTERFACE_TITLE_HEIGHT;
	iY		+=	m_iCategoryLineCount*(iCategoryBarHeight+1)-1;
	
	int	i,iPutCount	=	0;
	
	//	그릴수 있는 아이템 수 계산
	{
		int	iHeight	=	iWindowHeight-dSHOP_INTERFACE_HEIGHT_GAP-m_iCategoryLineCount*(iCategoryBarHeight+1)-1;
		
		iLineCount	=	iHeight/dSHOP_ITEM_HEIGHT;
		
		iY			+=	(iHeight-iLineCount*dSHOP_ITEM_HEIGHT)/2;
	}
	
	cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[m_iCurrentCategory];
	
	int	iAvailLineCount	=	0;
	for (i=0;i<lpCategory->m_wItemCount && iPutCount<iLineCount;i++)
	{
		if	(g_bIsTestServer == FALSE && lpCategory->m_aItemList[i].m_bf1IsTestItem)
			continue;
		
		if	(iAvailLineCount < s_sbMiddleWindow.m_iCurrentPos)
		{
			iAvailLineCount++;
			continue;
		}
		
		iAvailLineCount++;
		
		cItem	item;
		
		CCustomItem	*lpCustomItem	=	g_premiumItem.getItem(lpCategory->m_aItemList[i].m_wPremiunItemIndex);
		
		if	(!lpCustomItem)
			continue;
		
		if	(!lpCustomItem->generateItem(&item))
			continue;
		
		int	iPutX	=	iX+10;
		int	iPutY	=	iY+iPutCount*dSHOP_ITEM_HEIGHT;
		
		cRECT	rectSelect;
		
		rectSelect.set(iPutX-2,iPutY-2,iPutX+245-6,iPutY+42-2);
		
		if	(rectSelect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(rectSelect.x1,rectSelect.y1,rectSelect.x1+dITEM_SHAPE_WIDTH+2,rectSelect.y1+dITEM_SHAPE_HEIGHT+2);
			
			return	i;
		}
		
		iPutCount++;
	}
	
	return	0xffff;
}
// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃                  소문자를 대문자로 변환한다.			             ┃
// ┃                                                     			     ┃
// ┃char* _str			- 해당 문자열									 ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
void
cCarrotShop::Str_toupper(char* _str)
{
	char* p = _str;
	
	while(*p)
	{
		if((*p & 0x80) == 0x80)
		{			
			p+=2;
		}
		else
		{
			if(p[0]>97 && p[0]<122)
				p[0] -= 32;			
			p++;
		}
	}
}
// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃                  문자열의 길이를 리턴한다.			             ┃
// ┃                                                     			     ┃
// ┃char* _str			- 해당 문자열									 ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
int
cCarrotShop::Str_len(char* _str)
{	
	char* p = _str;
	int iLen = 0;
	bool bCombination = false;
	
	while(*p)
	{
		if((*p & 0x80) == 0x80)
		{			
			p+=2;
			iLen++;			
		}
		else
		{
			if(bCombination == true)
			{
				iLen++;
				bCombination = false;
			}
			p++;
			iLen++;
		}
	}
	if(bCombination == true)
		iLen++;
	
	return iLen;
}
void
cCarrotShop::update()
{
	if	(!isOpened())
		return;
	
	if (g_config.b_bIsNewCarrotShop)
		updateNew();
	if(!g_config.b_bIsNewCarrotShop)
	{
		// -- existing code
		if	(!cINPDEV::IsExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR))
		{
			if (s_sbMiddleWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
				cINPDEV::SetExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR);
			else
				cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SHOP_SCROLL_BAR);
		}
		
		OperateInventoryWindow();
		
		if	(cINPDEV::IsExclusive(eMOUSE_STATE_POPUP_MENU))
			return;
		
		m_iFocusItem	=	getFocusItem();
		m_iFocusCategory=	getFocusCategory();
		
		cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[m_iCurrentCategory];
		
		if	(m_iFocusItem	!=	0xffff)
		{
			if (s_rectTooltip.isIn(s_posMouse.x,s_posMouse.y))
			{
				s_rectTooltip.add(dITEM_SHAPE_HEIGHT+8,8);
				
				cItem	item;
				
				if	(g_premiumItem.generateItem(lpCategory->m_aItemList[m_iFocusItem].m_wPremiunItemIndex,&item))
				{
// 					item.m_bCount=	lpCategory->m_aItemList[m_iFocusItem].m_bf8ItemCount;
					
					char	*strComment	=	item.getItemTooltip(-1,-1,TRUE);
					
					strComment			=	g_hero.getAfterEquipInfo(strComment,&item);
					
					s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_NONE);
				}
			}
		}
		
		if	(s_isTouchLeftButton && !s_bmMiddleWindowButton.isPressed() && m_iFocusItem != 0xffff)
			m_iSelectItem	=	m_iFocusItem;
		
		if	(s_isClickedLeftButton && !s_bmMiddleWindowButton.isPressed() && m_iFocusCategory != 0xffff)
		{
			m_iCurrentCategory	=	m_iFocusCategory;
			m_iSelectItem		=	-1;
		}
		
		if	(m_iSelectItem	!=	-1)	//	구입 버튼
		{
			DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);
			
			switch(dwMenu)
			{
			case	IDOK	:
				{
					if	(m_wIsWaitResult)
					{
						AddSystemMessage(LTYELLOW,dMSG_WAIT_FOR_RESULT_OF_BUY_ITEM);
						break;
					}
					
					cItem		item;
					CCustomItem	*lpCustomItem	=	g_premiumItem.getItem(lpCategory->m_aItemList[m_iSelectItem].m_wPremiunItemIndex);
					
					if	(!lpCustomItem)
						break;
					
					if	(!lpCustomItem->generateItem(&item))
						break;
					cBasicItem*	lpBasicItem	=	item.getBasicItem();
					if(!lpBasicItem)
						break;
					
					if(lpBasicItem->m_attr.isBadge)
					{
						int iBedgeCount	=	g_hero.getBadgeCountInInventory() ;
						if(iBedgeCount >= dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
						{
							g_msgBox.cPopup("",dMSG_CAN_HAVE_BEDGE_UNTIL_THREE,dMSG_OK);
							break;
						}
					}
					if (lpBasicItem->m_attr.isCanNotOwnSameItem)
					{
						if (g_hero.getItemByBaseItemIndex(item.m_wBaseItem, TRUE))
						{
							g_msgBox.cPopup("",dMSG_CAN_NOT_OWN_SAME_ITEM,dMSG_OK);
							break;
						}
					}
// 					item.m_bCount=	lpCategory->m_aItemList[m_iSelectItem].m_bf8ItemCount;
					
					int		iPrice	=	lpCategory->m_aItemList[m_iSelectItem].m_wCarrotCount;
					char	strText[512];

					sprintf(strText,dMSG_DO_YOU_WANT_BUY_FOLLOW_CARROT_SHOP_ITEM_FORM,item.getName(),item.m_bCount,iPrice,"");

					switch(g_iCarrotShopType)
					{
						case	eCarrotShop_AllItemToInventory	:
							if	(item.isLimitTermItem())
								sprintf(strText,dMSG_DO_YOU_WANT_BUY_FOLLOW_CARROT_SHOP_ITEM_FORM,item.getName(),item.m_bCount,iPrice,dCARROT_SHOP_ITEM_TERM_WARNING);
							break;

						case	eCarrotShop_IDPublicItemToCart	:
							if	(lpBasicItem->m_wIsIDPublicItem	==	FALSE	&&	item.isLimitTermItem())
								sprintf(strText,dMSG_DO_YOU_WANT_BUY_FOLLOW_CARROT_SHOP_ITEM_FORM,item.getName(),item.m_bCount,iPrice,dCARROT_SHOP_ITEM_TERM_WARNING);
							break;
					}

					m_iBuyItem		=	m_iSelectItem;
					m_iBuyCategory	=	m_iCurrentCategory;
					
					g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
					g_msgBox.setButtonId(eMBI_BUY_CARROT_SHOP_ITEM);	//	길드전이 끝나서 마을로 돌아가기
					
					break;
				}
			}
		}
	}
}


void
cCarrotShop::updateNew()
{
	s_bIsBookedDisableChattingBox	=	TRUE;
	
	int iResult = 0xffff;
	cCarrotShopCategoryInfoDefine	*lpCategory = &g_carrotShop.m_aData[m_iSelectSecCategory];	
	
	// 닫기 버튼 이벤트 
	if(s_isClickedLeftButton && CloseBtnPos.isIn(s_posMouse.x, s_posMouse.y))
		close();
	
	// 마우스포인터가 카테고리 버튼의 어느위치에 있는지 찾는다.	
	m_iFocusTopCategory	= GetFocusTopCategory();	// 최상위카테고리의 어디에 있는지 확인
	m_iFocusSecCategory = GetFocusSecCategory();	// 두번째카테고리의 어디에 있는지 확인
	
	// 아이템 툴팁 관련
	updateItemTooltip(m_iSelectSecCategory);
	
	// 카테고리버튼을 클릭했는지 체크	
	if	(s_isTouchLeftButton && m_iFocusTopCategory != 0xffff && !g_msgBox.isPoped())
	{
		m_iSelectTopCategory = m_iFocusTopCategory;
		
		if(m_iSelectTopCategory == 1)
		{
			close();
			g_msgBox.cPopup("",dMSG_ARE_YOU_OPEN_SHOPPING_CART,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_OPEN_SHOPPING_CART);			
		}
		if(m_iSelectTopCategory == 2)
		{
			g_msgBox.cPopup("", dMSG_READY, dMSG_OK);
			m_iSelectTopCategory = 0;
		}
		if(m_iSelectTopCategory == 3)
		{
			g_msgBox.cPopup("", dMSG_CARROTSHOP_NOTYET, dMSG_OK);
			m_iSelectTopCategory = 0;
		}
		ClearSearch(false, true, false, true);
		m_iSearchItemIndexCount = 0xffff;
	}	
	if	(s_isTouchLeftButton && m_iFocusSecCategory != 0xffff && !m_rtSearchResultArea.isIn(s_posMouse.x,s_posMouse.y) && !g_msgBox.isPoped())
	{
		m_iSelectSecCategory = m_iFocusSecCategory;
		
		lpCategory	=	&g_carrotShop.m_aData[m_iSelectSecCategory];
		
		ScrollBarSet();
		
		ClearSearch(false, true, false, true);
		m_iSearchItemIndexCount = 0xffff;
	}
	
	// 스크롤바 업데이트
	m_sbScrollBar.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);
	
	// 아이템 구입버튼 처리
	iResult = 0xffff;
	if (s_isTouchLeftButton && !g_msgBox.isPoped() && !g_msgBox.isPoped())
		updateBuyBtnEvent(m_iSelectSecCategory);
		
	// 아이템 선물버튼 처리 - 지금은 기능구현 안됨
	if (s_isTouchLeftButton && !g_msgBox.isPoped() && !g_msgBox.isPoped())
		updatePresentBtnEvent(m_iSelectSecCategory);
	
	// 아이템 위시리스트버튼 처리
	if (s_isTouchLeftButton && !g_msgBox.isPoped() && !g_msgBox.isPoped())
		updateWishListBtnEvent(m_iSelectSecCategory);
		
	// 위시리스트부분 버튼 이벤트
	if (s_isTouchLeftButton && m_WishBuyBtnPos.isIn(s_posMouse.x,s_posMouse.y) && !g_msgBox.isPoped())
	{
		WishBuyBtnEvent();
	}
	if (s_isTouchLeftButton && m_WishPresentBtnPos.isIn(s_posMouse.x,s_posMouse.y) && !g_msgBox.isPoped())
	{
		g_msgBox.cPopup("", dMSG_READY, dMSG_OK);
	}
	if (s_isTouchLeftButton && m_WishDeleteBtnPos.isIn(s_posMouse.x,s_posMouse.y) && !g_msgBox.isPoped())
	{
		m_iWishListCount = 0;		// 위시리스트 개수 지우고 
		m_iTotalPayCarrot = 0;		// 결제 필요 당근 개수 지우고 
	}

	// 위시리스트 아이템 직접 클릭해서 Delete
	if(m_iWishListCount && s_isTouchLeftButton && !g_msgBox.isPoped())
	{
		for(int i=0 ; i<m_iWishListCount ; i++)
		{
			if(m_WishListItemPos[i].isIn(s_posMouse.x, s_posMouse.y))
				DeleteWishList(i);
		}
	}

	// 검색에디트박스 클릭하면 초기화
	if(s_isTouchLeftButton && m_rtEditBoxArea.isIn(s_posMouse.x, s_posMouse.y) && !strcmp(m_strSearchValue, m_strRecommendItemName))
		m_inputSearch.setStr(0, "");
			
	// 검색에디트박스 관련
	if(s_isTouchLeftButton && m_rtEditBoxArea.isIn(s_posMouse.x, s_posMouse.y) && !m_binputState)
	{
		int iX			= m_rtEditBoxArea.x1;
		int iY			= m_rtEditBoxArea.y1;
		int iBoxWidth	= g_sprInterface2.getSpriteWidth(eCARROT_SEARCHBOX);
		int iBoxHeight	= g_sprInterface2.getSpriteHeight(eCARROT_SEARCHBOX);
		m_inputSearch.init(s_hWnd, s_hInst, &s_text, 1);
		m_inputSearch.set(iX, iY, 0, iX+2, iY+2, iBoxWidth-4, iBoxHeight-4,	dITEM_NAME_LENGTH, "");
		m_binputState = true;
	}
	if(!m_binputState && m_inputSearch.m_iCount)
		m_inputSearch.close();				
	
	// 아이템 창 스크롤 관련
	if (m_sbScrollBar.m_iSize)
	{
		if (s_iWheelValue<0 && m_sbScrollBar.m_iCurrentPos < m_sbScrollBar.m_iSize-m_sbScrollBar.m_iPageSize)
			m_sbScrollBar.setPos(m_sbScrollBar.m_iCurrentPos+=1);
		if (s_iWheelValue>0)
		{
			if (s_iWheelValue>0 && m_sbScrollBar.m_iCurrentPos > 0)
				m_sbScrollBar.setPos(m_sbScrollBar.m_iCurrentPos-=1);		
		}
	}

	// 아이템박스 포커스 관련
	if (!g_msgBox.isPoped())
	{
		if (m_iSelectSecCategory == 0)
			m_iFocusItemBoxIndex	=	GetItemBoxIndex(m_iTotalItemCount);
		if (m_iSelectSecCategory == 1)
			m_iFocusItemBoxIndex	=	GetItemBoxIndex(m_iNewItemCount);
		if (m_iSelectSecCategory > 1)
		{
			lpCategory				=	&g_carrotShop.m_aData[m_iSelectSecCategory-2];
			m_iFocusItemBoxIndex	=	GetItemBoxIndex(lpCategory->m_wItemCount);
		}
	}

	// Search EditBox 자동완성기능 검사
	updateSearchBox();				
}


void
cCarrotShop::updateItemTooltip(int _iSelectSecCategory)
{
	cCarrotShopCategoryInfoDefine* lpCategory = NULL;
	int iResult = 0;
	int iCount	= 0;
	
	if (_iSelectSecCategory == 0)
	{
		iCount = m_iTotalItemCount;
		iResult = GetClickBuyBntIndex(iCount);
	}
	if (_iSelectSecCategory == 1)
	{
		iCount = m_iNewItemCount;
		iResult = GetClickBuyBntIndex(iCount);
	}
	if (_iSelectSecCategory > 1)
	{
		lpCategory	=	&g_carrotShop.m_aData[m_iSelectSecCategory-2];
		iResult = GetClickBuyBntIndex(lpCategory->m_wItemCount);
	}
	
	if (_iSelectSecCategory == 0 || _iSelectSecCategory == 1)
		iResult=GetToolTipIndex(iCount);
	if (_iSelectSecCategory > 1)
		iResult=GetToolTipIndex(lpCategory->m_wItemCount);

	if(iResult != 0xffff)
	{
		if( m_bSelectSearchBoxItem == false) // 검색결과 없다.
		{
			if (_iSelectSecCategory <= 1)
			{
				int icnt = 0;
				bool bBreak = false;
				for(int i=0; i<g_carrotShop.m_wCategoryCount ; i++)
				{
					lpCategory	=	&g_carrotShop.m_aData[i];
					for(int j=0; j<g_carrotShop.m_aData[i].m_wItemCount ; j++)
					{
						if(iResult == icnt)
						{
							int iX = m_aToolTipPos[iResult].x1;
							int iY = m_aToolTipPos[iResult].y2;							
							int iIndex = lpCategory->m_aItemList[j].m_wPremiunItemIndex;
							bBreak = DrawToolTip(iX, iY, iIndex);
							break;
						}
						else
							icnt++;
					}
					if(bBreak)
						break;
				}
			}
			if (_iSelectSecCategory > 1)
			{
				if(iResult != 0xffff)
				{
					int iX = m_aToolTipPos[iResult].x1;
					int iY = m_aToolTipPos[iResult].y2;
					int iIndex = lpCategory->m_aItemList[iResult].m_wPremiunItemIndex;
					DrawToolTip(iX, iY, iIndex);
				}
			}
		}
		else	// 검색결과 있다.
		{
			DrawResultToolTip();
		}
	}
}



void
cCarrotShop::updateBuyBtnEvent(int _iSelectSecCategory)
{
	cCarrotShopCategoryInfoDefine*	lpCategory = &g_carrotShop.m_aData[_iSelectSecCategory];
	int iResult = 0;
	int iCount	= 0;

	if (_iSelectSecCategory == 0)
	{
		iCount = m_iTotalItemCount;
		iResult = GetClickBuyBntIndex(iCount);
	}
	if (_iSelectSecCategory == 1)
	{
		iCount = m_iNewItemCount;
		iResult = GetClickBuyBntIndex(iCount);
	}
	if (_iSelectSecCategory > 1)
	{
		lpCategory	=	&g_carrotShop.m_aData[m_iSelectSecCategory-2];
		iResult = GetClickBuyBntIndex(lpCategory->m_wItemCount);
	}

	if(m_aBuyBtnPos[iResult].isIn(s_posMouse.x,s_posMouse.y))
	{
		if( m_bSelectSearchBoxItem == false) // 검색결과 없다.
		{
			if (_iSelectSecCategory <= 1)
			{
				int icnt = 0;
				bool bBreak = false;
				for(int i=0; i<g_carrotShop.m_wCategoryCount ; i++)
				{
					lpCategory	=	&g_carrotShop.m_aData[i];
					for(int j=0; j<g_carrotShop.m_aData[i].m_wItemCount ; j++)
					{
						if(iResult == icnt)
						{
							bBreak = BuyBtnEvent(lpCategory, i, j);
							break;
						}
						else
							icnt++;
					}
					if(bBreak)
						break;
				}
			}
			if (_iSelectSecCategory > 1)
			{
				lpCategory	=	&g_carrotShop.m_aData[m_iSelectSecCategory-2];
				if(iResult != 0xffff)
					BuyBtnEvent(lpCategory, m_iSelectSecCategory-2, iResult);
			}
		}
		else	// 검색결과 있다.
		{
			if(m_iReturnState == 1)
			{
				if(m_aBuyBtnPos[0].isIn(s_posMouse.x,s_posMouse.y))
				{
					lpCategory = &g_carrotShop.m_aData[m_aiSearchCateIndex[0]];				
					BuyBtnEvent(lpCategory, m_aiSearchCateIndex[0], m_aiSearchItemIndex[0]);
				}
			}
			if(m_iReturnState == 2)
			{
				for(int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<m_iSearchCount ; i++)
				{
					if(m_aBuyBtnPos[i].isIn(s_posMouse.x,s_posMouse.y))
					{
						m_iSelectSearchBoxItem = i;		// insu add
						lpCategory = &g_carrotShop.m_aData[m_aiSearchCateIndex[i]];				
						BuyBtnEvent(lpCategory, m_aiSearchCateIndex[i], m_aiSearchItemIndex[i]);
						break;
					}							
				}
			}
			if(m_iReturnState == 3)
			{
				if(m_aBuyBtnPos[m_iSelectSearchBoxItem].isIn(s_posMouse.x,s_posMouse.y))
				{
					lpCategory = &g_carrotShop.m_aData[m_aiSearchCateIndex[m_iSelectSearchBoxItem]];				
					BuyBtnEvent(lpCategory, m_aiSearchCateIndex[m_iSelectSearchBoxItem], m_aiSearchItemIndex[m_iSelectSearchBoxItem]);
				}
			}
		}
	}
}





void
cCarrotShop::updatePresentBtnEvent(int _iSelectSecCategory)
{
	cCarrotShopCategoryInfoDefine*	lpCategory = &g_carrotShop.m_aData[_iSelectSecCategory];
	int iResult = 0xffff;
	if (_iSelectSecCategory == 0)	// 전체
		iResult=GetClickPresentBntIndex(m_iTotalItemCount);
	if (_iSelectSecCategory == 1) // 신규
		iResult=GetClickPresentBntIndex(m_iNewItemCount);
	if (_iSelectSecCategory > 1)	// 나머지 카테고리
	{
		lpCategory	=	&g_carrotShop.m_aData[m_iSelectSecCategory-2];
		iResult		=	GetClickPresentBntIndex(lpCategory->m_wItemCount);
	}
	
	if( m_bSelectSearchBoxItem == true) // 검색결과 있다.
	{				
		if(m_iReturnState == 1)
		{
			if(iResult == 0 && m_aPresentBtnPos[iResult].isIn(s_posMouse.x,s_posMouse.y))
			{
				g_msgBox.cPopup("", dMSG_READY, dMSG_OK);
			}
		} 
		if(m_iReturnState == 2)
		{
			if(iResult < m_iSearchCount && m_aPresentBtnPos[iResult].isIn(s_posMouse.x,s_posMouse.y))
			{
				g_msgBox.cPopup("", dMSG_READY, dMSG_OK);
			}
		}
		if(m_iReturnState == 3)
		{
			if(m_aPresentBtnPos[m_iSelectSearchBoxItem].isIn(s_posMouse.x,s_posMouse.y))
			{
				g_msgBox.cPopup("", dMSG_READY, dMSG_OK);
			}
		}
	}
	else	// 검색결과 없다 
	{
		if(iResult != 0xffff)
		{
			// 선물버튼을 눌렀을때의 처리
			g_msgBox.cPopup("", dMSG_READY, dMSG_OK);
		}
	}
}



void
cCarrotShop::updateWishListBtnEvent(int _iSelectSecCategory)
{
	cCarrotShopCategoryInfoDefine*	lpCategory = &g_carrotShop.m_aData[_iSelectSecCategory];
	int iResult = 0xffff;

	if (m_iSelectSecCategory == 0)
		iResult		=	GetClickWishBntIndex(m_iTotalItemCount);
	if (m_iSelectSecCategory == 1)
		iResult		=	GetClickWishBntIndex(m_iNewItemCount);
	if (m_iSelectSecCategory > 1)
	{
		lpCategory	=	&g_carrotShop.m_aData[m_iSelectSecCategory-2];
		iResult		=	GetClickWishBntIndex(lpCategory->m_wItemCount);
	}

	if(iResult == 0xffff)
		return;
	
	if(m_iWishListCount == 10)	// 10개라면 리턴!
	{
		AddSystemMessage(LTYELLOW, dMSG_CARROTSHOP_WISHLIST_FULL);
		g_msgBox.cPopup("", dMSG_CARROTSHOP_WISHLIST_FULL, dMSG_YES);
		return;
	}

	if(!m_bSelectSearchBoxItem)	// 서치바 결과가 없을때 
	{
		CCustomItem *lpCustomItem	= NULL;
		int			f8ItemCount		= 0;
		int			icnt			= 0;
		
		if (m_iSelectSecCategory > 1)
		{
			lpCategory			=	&g_carrotShop.m_aData[m_iSelectSecCategory-2];
			lpCustomItem		=	g_premiumItem.getItem(lpCategory->m_aItemList[iResult].m_wPremiunItemIndex);
			f8ItemCount			=	lpCustomItem->m_bCount;
// 			f8ItemCount			=	lpCategory->m_aItemList[iResult].m_bf8ItemCount;
			m_iTotalPayCarrot	+=	lpCategory->m_aItemList[iResult].m_wCarrotCount;
			m_aiListItemCarrot[m_iWishListCount]	=	lpCategory->m_aItemList[iResult].m_wCarrotCount;
		}
		if (m_iSelectSecCategory <= 1)
		{			
			for(int i=0; i<g_carrotShop.m_wCategoryCount ; i++)
			{
				lpCategory	=	&g_carrotShop.m_aData[i];
				for(int j=0; j<g_carrotShop.m_aData[i].m_wItemCount ; j++)
				{
					if(iResult == icnt)
					{
						lpCustomItem = g_premiumItem.getItem(lpCategory->m_aItemList[j].m_wPremiunItemIndex);
						f8ItemCount = lpCustomItem->m_bCount;
// 						f8ItemCount = lpCategory->m_aItemList[j].m_bf8ItemCount;
						m_iTotalPayCarrot += lpCategory->m_aItemList[j].m_wCarrotCount;
						m_aiListItemCarrot[m_iWishListCount] = lpCategory->m_aItemList[j].m_wCarrotCount;
						break;
					}
					else
						icnt++;
				}
				if(lpCustomItem != NULL)
					break;
			}						
		}

		if(!lpCustomItem)
			return;		
		if(!lpCustomItem->generateItem(&m_aWishListItem[m_iWishListCount]))
			return;	
		
		m_aWishListItem[m_iWishListCount].m_bCount = f8ItemCount;		
		m_iWishListCount++;
	}

	if(m_bSelectSearchBoxItem)	// 서치바 결과가 있을때
	{
		BOOL	bCorrect	= FALSE;
		int		iIndex		= 0;

		if(m_iReturnState == 1 && m_aWishBtnPos[0].isIn(s_posMouse.x,s_posMouse.y))
		{
			bCorrect	= TRUE;
			iIndex		= m_iSelectSearchBoxItem;
		}		
		if(m_iReturnState == 2 && iResult < m_iSearchCount)
		{
			bCorrect	= TRUE;
			iIndex		= iResult;
		}		
		if(m_iReturnState == 3 && m_aWishBtnPos[m_iSelectSearchBoxItem].isIn(s_posMouse.x,s_posMouse.y))
		{
			bCorrect	= TRUE;
			iIndex		= m_iSelectSearchBoxItem;
		}
					
		if (bCorrect)
		{
			lpCategory = &g_carrotShop.m_aData[m_aiSearchCateIndex[iIndex]];
			CCustomItem *lpCustomItem = g_premiumItem.getItem(lpCategory->m_aItemList[m_aiSearchItemIndex[iIndex]].m_wPremiunItemIndex);
				
			if(!lpCustomItem)
				return;
			
			if(!lpCustomItem->generateItem(&m_aWishListItem[m_iWishListCount]))
				return;
						
			m_iTotalPayCarrot += lpCategory->m_aItemList[m_aiSearchItemIndex[iIndex]].m_wCarrotCount;
			m_aiListItemCarrot[m_iWishListCount] = lpCategory->m_aItemList[m_aiSearchItemIndex[iIndex]].m_wCarrotCount;
			m_iWishListCount++;		// 위시리스트 카운트를 하나 증가시킨다.
		}
	}
}




void
cCarrotShop::updateSearchBox()
{
	m_inputSearch.update(s_posMouse.x,s_posMouse.y,s_isClickedLeftButton,s_aTouched[KEY_TAB]);
	m_inputSearch.getStr(0, m_strSearchValue, dITEM_NAME_LENGTH);
	if(Str_len(m_strSearchValue) >= 2)
	{
		if(strcmp(m_strPreSearchValue, m_strSearchValue))
		{
			SearchAutoComplete();			
			if(m_iSearchCount)
				m_bShowResultState = true;
			strcpy(m_strPreSearchValue, m_strSearchValue);
		}
		
		
		// 마우스 클릭 여부도 검사한다.
		if(s_isTouchLeftButton)
		{
			int itemp = (m_iSearchCount > 5)? 5 : m_iSearchCount;
			for(int i=m_sbSearchScrollBar.m_iCurrentPos ; i<m_sbSearchScrollBar.m_iCurrentPos+itemp ; i++)
			{
				if(m_SelectSearchResultBox[i-m_sbSearchScrollBar.m_iCurrentPos].isIn(s_posMouse.x, s_posMouse.y) && m_binputState == TRUE)
				{
					m_bShowResultState = false;
					
					if(m_iSearchCount == 1)
						m_iReturnState = 1;
					else if(m_iSearchCount > 1)
						m_iReturnState = 3;
					
					m_sbScrollBar.setSize(0, 0);
					m_bSelectSearchBoxItem = true;					
					
					m_iSelectSearchBoxItem = i;
					ClearSearch(false, false, true);
					
					break;
				}
			}			
		}
		
		// 루프를 돌면서 몇번째 결과값에 마우스 포인터가 가있는지 검색한다.
		for(int i=0; i< sizeof(m_SelectSearchResultBox)/sizeof(cRECT) ; i++)
			if(m_SelectSearchResultBox[i].isIn(s_posMouse.x, s_posMouse.y) && m_bShowResultState == true)
				m_iSearchItemIndexCount = i;
		
		// 서치바 내에서 ↑↓ 버튼을 눌렀을 때 
		if(s_aTouched[KEY_UP])
		{
			if(m_iSearchItemIndexCount > 0 && m_iSearchItemIndexCount != 0xffff)
			{				
				if(!m_sbSearchScrollBar.m_iCurrentPos)
					m_iSearchItemIndexCount--;
				else
					m_sbSearchScrollBar.setPos(m_sbSearchScrollBar.m_iCurrentPos-1);
			}
			else if(m_iSearchItemIndexCount == 0)
				m_iSearchItemIndexCount = 0xffff;
			
		}
		if(s_aTouched[KEY_DOWN])
		{
			if(m_iSearchItemIndexCount == 0xffff)
				m_iSearchItemIndexCount = 0;
			else
			{
				if(m_iSearchCount > m_iSearchItemIndexCount+1)
				{					
					if(m_iSearchItemIndexCount < 4)
						m_iSearchItemIndexCount++;
					else
						m_sbSearchScrollBar.setPos(m_sbSearchScrollBar.m_iCurrentPos+1);
				}
			}
		}
		// 리턴키 눌렀을 때
		if(s_aTouched[KEY_RETURN])
		{
			m_bShowResultState = false;
			
			
			if(m_iSearchCount == 1) // 결과값이 한개
			{
				m_iReturnState = 1;
				
				m_iSelectSearchBoxItem = 0;				
				m_sbScrollBar.setSize(0, 0);
				ClearSearch(false, true, true, false);
			}
			
			else if(m_iSearchCount > 1)	// 결과값이 여러개
			{				
				if(m_iSearchItemIndexCount == 0xffff)	// 그냥 리턴을 눌렀다
				{
					m_iReturnState = 2;
					
					m_iSelectSearchBoxItem = 0xffff;	// 선택된 값이 없다.. 
					ClearSearch(false, false, true, false);
					
					// 스크롤바 설정(검색결과가 한페이지를 넘어갈수도 있으니까)
					int iScrollValue = (m_iSearchCount%(iOneLineItemNum))? m_iSearchCount/iOneLineItemNum+1 : m_iSearchCount/iOneLineItemNum;
					m_sbScrollBar.setSize(iScrollValue, 4);
				}				
				else	// 무엇인가 선택을 하고 리턴을 눌렀다
				{
					m_iReturnState = 3;
					
					m_iSelectSearchBoxItem = m_iSearchItemIndexCount+m_sbSearchScrollBar.m_iCurrentPos;
					m_sbScrollBar.setSize(0, 0);
					ClearSearch(false, true, true, false);						
				}				
			}			
			else	// 무반응
			{
			}
			m_binputState = false;
		}		
	}
	else	// 두글자를 안넘어가면 초기화
	{
		if(m_iSearchCount)
		{
			// 스크롤바 설정(검색결과로 인해 스크롤바가 바뀔수 있으니까)
			ScrollBarSet();
		}
		m_iReturnState = 0;
		
		memset(m_strPreSearchValue, '\n', sizeof(m_strPreSearchValue));
		ClearSearch(false, true, false, false);
		m_iSearchItemIndexCount = 0xffff;
	}
	
	if(m_iSearchCount)
		m_sbSearchScrollBar.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);
}

int
cCarrotShop::getFocusCategory()
{
	for	(int i=0;i<g_carrotShop.m_wCategoryCount;i++)
	{
		if	(g_bIsTestServer == FALSE && g_carrotShop.m_aData[i].m_bf1IsTestCategory)
			continue;
		
		if	(m_aCategoryInfo[i].isIn(s_posMouse.x,s_posMouse.y))
			return	i;
	}
	
	return	0xffff;
}

void
cCarrotShop::drawCategory()
{
	if	(g_carrotShop.m_wCategoryCount	==	0)
	{
		m_iCategoryLineCount	=	0;
		return;
	}
	
	int		i;
	
	int		iLeftMargin			=	10;
	int		iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iBarHeight			=	g_sprInterface2.getSpriteHeight(eCSWI_FIRST_BAR);
	int		iBarWidth			=	g_sprInterface2.getSpriteWidth(eCSWI_FIRST_BAR);
	int		iX					=	iLeftMargin+iBarWidth;
	int		iY					=	m_rect.y1+	dSHOP_INTERFACE_TITLE_HEIGHT;
	int		aiMarginWidth[dMAX_CARROT_SHOP_CATEGORY_COUNT];
	int		iBarPerLine			=	0;
	int		iCategoryLineCount	=	0;
	
	memset(aiMarginWidth,0,sizeof(aiMarginWidth));
	
	m_iCategoryLineCount	=	1;
	
	for	(i=0;i<g_carrotShop.m_wCategoryCount;i++)
	{
		cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[i];
		
		if	(g_bIsTestServer == FALSE && lpCategory->m_bf1IsTestCategory)
			continue;
		
		int	iWidth	=	s_text.getPixelSize(lpCategory->m_strName);
		
		if	(iX	+	iWidth+iLeftMargin	>=	iWindowWidth)
		{
			aiMarginWidth[iCategoryLineCount]	=	(iWindowWidth-iX)/iBarPerLine;
			iX					=	iLeftMargin+iBarWidth;
			iY					+=	iBarHeight+1;
			iCategoryLineCount++;
			iBarPerLine			=	0;
		}
		
		iX		+=		iWidth+iBarWidth*2;
		
		iBarPerLine++;
	}
	
	m_iCategoryLineCount	=	1;
	iCategoryLineCount		=	0;
	iX						=	iLeftMargin+iBarWidth;
	iY						=	m_rect.y1+	dSHOP_INTERFACE_TITLE_HEIGHT;
	
	for	(i=0;i<g_carrotShop.m_wCategoryCount;i++)
	{
		cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[i];
		
		if	(g_bIsTestServer == FALSE && lpCategory->m_bf1IsTestCategory)
			continue;
		
		int	iWidth	=	s_text.getPixelSize(lpCategory->m_strName);
		
		if	(iX	+	iWidth+iLeftMargin	>=	iWindowWidth)
		{
			iX	=	iLeftMargin+iBarWidth;
			iY	+=	iBarHeight+1;
			m_iCategoryLineCount	++;
			iCategoryLineCount++;
		}
		
		int	iImage	=	eCSWI_FIRST_BAR;
		
		if	(i	==	m_iCurrentCategory)
			iImage	+=	3;
		else
			if	(i	==	m_iFocusCategory)
				iImage	+=	6;
			
			int	iMargin	=	aiMarginWidth[iCategoryLineCount];
			
			m_aCategoryInfo[i].set(iX+m_rect.x1,iY,iX+m_rect.x1+iWidth+iMargin,iY+iBarHeight);
			
			g_sprInterface2.put3PieceBar(iX+m_rect.x1,iY,iImage,iWidth+iMargin);
			s_text.pushShadowStatus(FALSE);
			s_text.put(iX+m_rect.x1+iMargin/2,iY+4,0,lpCategory->m_strName);
			s_text.popShadowStatus();
			
			iX	+=	iWidth+iBarWidth*2+iMargin;
	}
}

void
cCarrotShop::draw()
{
	if (!isOpened())
		return;
	
	// insu modify	.. 주석처리
	//DrawInventoryWindow();		
	
	s_text.pushShadowStatus(FALSE);
	
	if	(g_carrotShop.m_wCategoryCount	==	0)
	{
		m_iCategoryLineCount	=	0;
		return;
	}
	
	
	iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2);
	iPutPosY = ((g_iScreenHeight-g_sprGameBottomInterface.getSpriteHeight(0))/2) - (m_iWindowHeight/2);
	
	// title 출력
	g_sprInterface2.Put(iPutPosX, iPutPosY, eCARROT_MAINWND_TITLE);			
	
	// 닫기버튼 출력
	// 여기에 eMMI_CLOSE_NORMAL,eMMI_CLOSE_PRESSED,eMMI_CLOSE_LIGHT, (eWINDOW_CLOSE_ACTIVE,eWINDOW_CLOSE_PRESSED,)
	CloseBtnPos.set(iPutPosX+g_sprInterface2.getSpriteWidth(eCARROT_MAINWND_TITLE)-g_sprInterface.getSpriteWidth(eMMI_CLOSE_NORMAL)-2,
		iPutPosY+2, 
		iPutPosX+g_sprInterface2.getSpriteWidth(eCARROT_MAINWND_TITLE)-2, 
		iPutPosY+2+g_sprInterface.getSpriteHeight(eMMI_CLOSE_NORMAL));
	if(CloseBtnPos.isIn(s_posMouse.x, s_posMouse.y) && !s_isLeftButtonPressed)
	{
		g_sprInterface.Put(	iPutPosX+g_sprInterface2.getSpriteWidth(eCARROT_MAINWND_TITLE)-g_sprInterface.getSpriteWidth(eWINDOW_CLOSE_ACTIVE)-2, 
			iPutPosY, 
			eWINDOW_CLOSE_ACTIVE);
	}
	else if(CloseBtnPos.isIn(s_posMouse.x, s_posMouse.y) && s_isLeftButtonPressed)
	{
		g_sprInterface.Put(	iPutPosX+g_sprInterface2.getSpriteWidth(eCARROT_MAINWND_TITLE)-g_sprInterface.getSpriteWidth(eWINDOW_CLOSE_ACTIVE)-2, 
			iPutPosY, 
			eWINDOW_CLOSE_PRESSED);
	}
	else
	{
		g_sprInterface2.Put(	iPutPosX+g_sprInterface2.getSpriteWidth(eCARROT_MAINWND_TITLE)-g_sprInterface.getSpriteWidth(eWINDOW_CLOSE_ACTIVE)-2, 
			iPutPosY, 
			eWINDOW_CLOSE_NORMAL);
	}
	iPutPosY += g_sprInterface2.getSpriteHeight(eCARROT_MAINWND_TITLE);
	
	cDRAW::Fill  (0, iPutPosX, iPutPosY, iPutPosX+m_iWindowWidth, iPutPosY+g_sprInterface2.getSpriteHeight(eCARROT_MAINWND_BORDER)-7);
	
	g_sprInterface2.Put(iPutPosX, iPutPosY, eCARROT_MAINWND_BORDER);		// 테두리 출력
	iPutPosY += 5;	// 테두리 윗라인 픽셀 5
	
	
	
	iPutPosX+=dSHOP_WND_LEFTMARGIN;	// 윈도우 왼쪽 공백만큼 증가시킨다.
	
	m_sbScrollBar.draw();
	// --------------------------------------------------	함수로 나눌 부분
	
	DrawTopCategory();
	
	DrawMainWndDivideLine();
	
	DrawItemBuyCategory();
	
	DrawMainWndDivideLine();
	
	if(m_iSelectSecCategory == 0 || m_iSelectSecCategory == 1)
	{		
		DrawAllItem();		
	}
	else
		DrawItemList();
	
	
	// 구분선 출력
	iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2)+6;
	iPutPosY += (g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)+iLargeBoxYMargin)*iLineNum;
	// 스크롤바 라인이 1이상이면 iPutPosY 좌표를 수정한다.
	if(m_sbScrollBar.m_iCurrentPos > 0)
		iPutPosY += (g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)+iLargeBoxYMargin);
	
	g_sprInterface2.Put(iPutPosX, iPutPosY, eCARROT_MAINWND_DIVIDELINE);
	
	iPutPosX += dSHOP_WND_LEFTMARGIN;
	iPutPosY += g_sprInterface2.getSpriteHeight(eCARROT_MAINWND_DIVIDELINE)+dSHOP_DIVIDELINE_YMARGIN;
	
	// 위시리스트 부분 출력
	DrawWishList();
	
	// Search Result 출력 ..
	DrawSearchResult();
	
	s_text.popShadowStatus();
	
	
	return;	
}



void
cCarrotShop::draw_Original()
{
	if (!isOpened())
		return;
	
	DrawInventoryWindow();
	
	int		iX,iY,iCount		=	0;
	int		iLineCount			=	0;
	int		iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iCategoryBarHeight	=	g_sprInterface2.getSpriteHeight(eCSWI_FIRST_BAR);
	
	iX		=	m_rect.x1;
	iY		=	m_rect.y1;
	
	if (g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill  (0,iX+8,iY+6,iX+266-4,iY+482-8);
	
	g_sprInterface2.Put(iX	,g_iCorrectBI_Y/2		,eCSWI_TITLE);
	g_sprInterface.Put(iX	,iY		,eWINDOW);
	
	drawCategory();
	
	iY		+=	dSHOP_INTERFACE_TITLE_HEIGHT;
	iY		+=	m_iCategoryLineCount*(iCategoryBarHeight+1)-1;
	
	//	스크롤바 정보 재설정
	{
		int	iHeight			=	iWindowHeight-dSHOP_INTERFACE_HEIGHT_GAP-m_iCategoryLineCount*(iCategoryBarHeight+1)-1;
		iLineCount			=	iHeight/dSHOP_ITEM_HEIGHT;
		
		int	iItemCount		=	g_carrotShop.getCategoryItemCount(m_iCurrentCategory);
		int	iTabListHeight	=	m_iCategoryLineCount*(iCategoryBarHeight+1);
		
		s_sbMiddleWindow.setSize(iItemCount,iLineCount);
		s_sbMiddleWindow.set(s_sbMiddleWindow.m_pos.x,m_rect.y1+iTabListHeight+dSHOP_INTERFACE_TITLE_HEIGHT,
			iWindowHeight-dSHOP_INTERFACE_HEIGHT_GAP-iTabListHeight);
		
		iY				+=	(iHeight-iLineCount*dSHOP_ITEM_HEIGHT)/2;
	}
	
	int	i,iPutCount	=	0;
	
	cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[m_iCurrentCategory];
	
	if	(g_carrotShop.m_wCategoryCount	==	0)
		lpCategory->m_wItemCount	=	0;
	
	int	iAvailLineCount	=	0;
	
	for (i=0;i<lpCategory->m_wItemCount&& iPutCount<iLineCount;i++)
	{
		if	(g_bIsTestServer == FALSE && lpCategory->m_aItemList[i].m_bf1IsTestItem)
			continue;
		
		if	(iAvailLineCount < s_sbMiddleWindow.m_iCurrentPos)
		{
			iAvailLineCount++;
			continue;
		}
		
		iAvailLineCount++;
		
		cItem	item;
		
		CCustomItem	*lpCustomItem	=	g_premiumItem.getItem(lpCategory->m_aItemList[i].m_wPremiunItemIndex);
		
		if	(!lpCustomItem)
			continue;
		
		if	(!lpCustomItem->generateItem(&item))
			continue;
		
		cBasicItem	*lpBaseItem	=	item.getBasicItem();
		
// 		item.m_bCount=	lpCategory->m_aItemList[i].m_bf8ItemCount;
		
		int	iPutX	=	iX+10;
		int	iPutY	=	iY+iPutCount*dSHOP_ITEM_HEIGHT;
		
		if (i	==	m_iSelectItem)
		{
			iPutY--;
			cDRAW::Fill(RGB24To16(0,100,200),iPutX-2,iPutY+2,iPutX+245-6,iPutY+42-2);	//	선택한 아이템이다.
		}
		
		g_sprInterface.Put(iPutX+3,iPutY+4,eIWI_ITEM_SLOT_FRAME);
		PutItem(&item,iPutX+3,iPutY+4,TRUE);
		
		char	strItemName[128];
		
		//		strcpy(strItemName,lpBaseItem->m_strName);
#ifdef	_DEBUG
		sprintf(strItemName,"[%2d] <c:LTGREEN>%s<n>",i,lpBaseItem->m_strName);
#else
		strcpy(strItemName,lpBaseItem->m_strName);
#endif
		int		iTextWidth	=	(iPutX+245-6)-(iPutX-2)-4-40;
		
		if (s_text.getLineCount(iTextWidth,strItemName)	>=	2)
		{
			s_text.setLineGap(0);
			s_text.cPut(iPutX+40,iPutY-5,iTextWidth,40,0,WHITE,dFITTEXT_HEIGHT_CENTER,strItemName);
			s_text.setLineGap(2);
			g_sprRegionInterface.Put(iPutX+42,iPutY+6+22,eCSWI_CARROT);
			s_text.put(iPutX+42+16,iPutY+6+22	,LTYELLOW,"%d",lpCategory->m_aItemList[i].m_wCarrotCount);
		}
		else
		{
			s_text.put(iPutX+42,iPutY+6			,WHITE,strItemName);
			g_sprRegionInterface.Put(iPutX+42,iPutY+6+16,eCSWI_CARROT);
			s_text.put(iPutX+42+16,iPutY+6+16	,LTYELLOW,"%d",lpCategory->m_aItemList[i].m_wCarrotCount);
		}
		
		iPutCount++;
		
		if	(i	==	m_iSelectItem)
			iPutY++;
	}
	
	s_bmMiddleWindowButton.draw();
	s_sbMiddleWindow.draw();
	
	g_sprInterface.Put(m_rect.x1+7,463+g_iCorrectBI_Y/2	,eWINDOW_SEPERATE_BAR);	//	경계
	g_sprRegionInterface.Put(m_rect.x1+12,475+g_iCorrectBI_Y/2,eCSWI_CARROT);
	s_text.put(m_rect.x1+28,475+g_iCorrectBI_Y/2,WHITE,"%d",m_iRemainCarrotCount);
}


void
cCarrotShop::buyItem(cItem *_lpItem,int _iRemainCarrotCount)
{
	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();
	
	if	(!lpBasicItem)
		return;
	
	int	iPrice						=	m_iRemainCarrotCount	-	_iRemainCarrotCount;
	
	m_iRemainCarrotCount			=	_iRemainCarrotCount;


	CCustomItem*	lpCustomItem	=	g_premiumItem.getItem(g_carrotShop.m_aData[m_iBuyCategory].m_aItemList[m_iBuyItem].m_wPremiunItemIndex);

	AddSystemMessage(WHITE,dMSG_BUY_CARROT_SHOP_ITEM_FORM,lpBasicItem->m_strName,lpCustomItem->m_bCount,iPrice);
	
	switch(g_iCarrotShopType)
	{
		case	eCarrotShop_AllItemToInventory	:
			break;

		case	eCarrotShop_IDPublicItemToCart	:
			if	(lpBasicItem->m_wIsIDPublicItem)
			{
				AddSystemMessage(LTGREEN,dMSG_STORE_ITEM_TO_CART);
				return;
			}
			break;

		case	eCarrotShop_AllItemToCart	:
			AddSystemMessage(LTGREEN,dMSG_STORE_ITEM_TO_CART);
			return;
	}
	
	cItem		*lpValidSlot	=	g_hero.getValidInventorySlot();
	
	if	(!lpValidSlot)
	{
		close();
		return;
	}
	
	lpValidSlot->copy(_lpItem);
	
	g_hero.buildPower();
}


// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃					    툴팁을 출력한다.			                 ┃
// ┃                                                     			     ┃
// ┃int _iPutPosX			- 찍어줄 x 좌표								 ┃
// ┃int _iPutPosY			- 찍어줄 y 좌표								 ┃
// ┃int _iItemIndex		- 프리미엄아이템 인덱스넘버					 ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
bool
cCarrotShop::DrawToolTip(int _iPutPosX, int _iPutPosY, int _iItemIndex)
{
	cItem item;
	if(g_premiumItem.generateItem(_iItemIndex, &item))
	{
		char* strComment = item.getItemTooltip(-1, -1, TRUE);
		strComment			=	g_hero.getAfterEquipInfo(strComment,&item);		
		
		int itemp1 = (s_ttCommon.m_iWidth)? s_text.getTextHeight(s_ttCommon.m_iWidth, strComment) : s_text.getTextHeight(s_ttCommon.m_iTextFrameWidth, strComment);
		int itemp2 = g_iScreenHeight-g_sprGameBottomInterface.getSpriteHeight(0);
		if(_iPutPosY+itemp1 > itemp2)
		{
			_iPutPosY-= (_iPutPosY+itemp1-itemp2);
		}
		s_ttCommon.popupTip(&s_rectTooltip, _iPutPosX, _iPutPosY, &s_text, WHITE, strComment, eTFM_NONE);
		
		return true;
	}
	return false;
}


void
cCarrotShop::ScrollBarSet()
{
	cCarrotShopCategoryInfoDefine	*lpCategory = &g_carrotShop.m_aData[m_iSelectSecCategory];
	int iScrollValue = (m_iTotalItemCount%iOneLineItemNum)? m_iTotalItemCount/iOneLineItemNum+1 : m_iTotalItemCount/iOneLineItemNum;
	
	if(m_iSelectSecCategory == 0)	// 전체 카테고리 일때 
	{		
		m_sbScrollBar.setSize(iScrollValue, 4);
	}
	else if(m_iSelectSecCategory == 1)	// 신규 카테고리 일때 
	{
		iScrollValue = (m_iNewItemCount%iOneLineItemNum)? m_iNewItemCount/iOneLineItemNum+1 : m_iNewItemCount/iOneLineItemNum;
		m_sbScrollBar.setSize(iScrollValue, 4);
	}
	else	// 나머지 카테고리일때 
	{
		lpCategory	=	&g_carrotShop.m_aData[m_iSelectSecCategory-2];
		
		if(lpCategory->m_wItemCount <= iOneLineItemNum*iLineNum)
		{
			m_sbScrollBar.setSize(0, 0);
		}
		else
		{
			int iScrollLineNum = lpCategory->m_wItemCount/iOneLineItemNum;
			if (lpCategory->m_wItemCount%iOneLineItemNum >  0)
				iScrollLineNum++;
			//iScrollLineNum -= iLineNum;
			m_sbScrollBar.setSize(iScrollLineNum, 4);
		}
	}
	
	m_sbScrollBar.setPos(0);
	m_sbScrollBar.draw();
}


void
cCarrotShop::DrawResultToolTip()
{
	int iResult = GetToolTipIndex(m_iTotalItemCount);
	cCarrotShopCategoryInfoDefine	*lpCategory;
	
	if(m_iReturnState == 1)
	{
		lpCategory = &g_carrotShop.m_aData[m_aiSearchCateIndex[0]];
		if(m_aToolTipPos[0].isIn(s_posMouse.x,s_posMouse.y))
		{
			DrawToolTip(	m_aToolTipPos[0].x1, 
				m_aToolTipPos[0].y1,
				lpCategory->m_aItemList[m_aiSearchItemIndex[0]].m_wPremiunItemIndex);
		}
	}
	if(m_iReturnState == 2)
	{
		if(iResult < m_iSearchCount)
		{
			lpCategory = &g_carrotShop.m_aData[m_aiSearchCateIndex[iResult]];
			//if(m_aToolTipPos[iResult].isIn(s_posMouse.x,s_posMouse.y))
			{
				DrawToolTip(	m_aToolTipPos[iResult].x1, 
					m_aToolTipPos[iResult].y1,
					lpCategory->m_aItemList[m_aiSearchItemIndex[iResult]].m_wPremiunItemIndex);
			}							
		}
	}
	if(m_iReturnState == 3)
	{
		lpCategory = &g_carrotShop.m_aData[m_aiSearchCateIndex[m_iSelectSearchBoxItem]];
		if(m_aToolTipPos[m_iSelectSearchBoxItem].isIn(s_posMouse.x,s_posMouse.y))
		{
			DrawToolTip(	m_aToolTipPos[m_iSelectSearchBoxItem].x1, 
				m_aToolTipPos[m_iSelectSearchBoxItem].y1,
				lpCategory->m_aItemList[m_aiSearchItemIndex[m_iSelectSearchBoxItem]].m_wPremiunItemIndex);
		}
	}
}


void
cCarrotShop::DeleteWishList(int _iListIndex)
{
	m_iTotalPayCarrot -= m_aiListItemCarrot[_iListIndex];	// 해당아이템의 당근수를 깍아주고
	m_iWishListCount--;		// 카운트를 줄여주고
	
	if(_iListIndex != m_iWishListCount)
	{
		for(int i=_iListIndex ; i<m_iWishListCount ; i++)
		{
			m_WishListItemPos[i] = m_WishListItemPos[i+1];		// 위치좌표
			m_aWishListItem[i] = m_aWishListItem[i+1];			// 아이템목록
			m_aiListItemCarrot[i] = m_aiListItemCarrot[i+1];	// 당근수
		}
	}	
}




void
cCarrotShop::ClearSearch(bool _bShowResultState, bool _bSearchCount, bool _bSelectSearchBoxItem, bool _bClearText)
{
	m_bShowResultState = _bShowResultState;		// 검색결과 출력 유무
	
	if(_bSearchCount)							// 검색결과 찾은갯수를 초기화 할것이냐?
		m_iSearchCount = 0;
	
	
	m_bSelectSearchBoxItem = _bSelectSearchBoxItem;				// 검색결과 클릭 유무
	
	m_rtSearchResultArea.set(0, 0, 0, 0);		// 검색결과 영역 초기화
	
	if(_bClearText)
	{
		memset(m_strPreSearchValue, '\0', sizeof(m_strPreSearchValue));		// 검색 텍스트 초기화
		memset(m_strSearchValue, '\0', sizeof(m_strPreSearchValue));
		m_inputSearch.setStr(0, "");
	}
	
}



void
cCarrotShop::DrawTopCategory()
{
	iPutPosY += dSHOP_DIVIDELINE_YMARGIN;	
	// 최상위 카테고리 버튼을 출력한다.
	for(int i=0 ; i<m_iTopCategoryCnt ; i++)
	{
		// 최상위 카테고리 위치좌표를 설정한다.
		m_aTopCategoryPos[i].set(	iPutPosX, 
			iPutPosY, 
			iPutPosX+g_sprInterface2.getSpriteWidth(eCARROT_TOPBTN_NORMAL), 
			iPutPosY+g_sprInterface2.getSpriteHeight(eCARROT_TOPBTN_NORMAL));
		
		iPutPosX += (g_sprInterface2.getSpriteWidth(eCARROT_TOPBTN_NORMAL)+dSHOP_TOPCATE_BTN_BYMARGIN);
		
		// 이미지 출력
		// 여기서 버튼이 선택되었는지, 마우스가 오버되었는지를 확인하여 분기출력해야 한다.
		if(m_iSelectTopCategory == i)	// 선택상태
		{
			g_sprInterface2.Put(m_aTopCategoryPos[i].x1, m_aTopCategoryPos[i].y1, eCARROT_TOPBTN_NORMAL+2);
		}
		else if(m_iFocusTopCategory == i && m_iSelectTopCategory != i)	// 마우스오버상태 
		{
			g_sprInterface2.Put(m_aTopCategoryPos[i].x1, m_aTopCategoryPos[i].y1, eCARROT_TOPBTN_NORMAL+1);	
		}
		else	// 평소상태 
		{
			g_sprInterface2.Put(m_aTopCategoryPos[i].x1, m_aTopCategoryPos[i].y1, eCARROT_TOPBTN_NORMAL);	
		}
		
		// 버튼 글자 출력
		
		
		s_text.cPut(	m_aTopCategoryPos[i].x1, m_aTopCategoryPos[i].y1+1, 
			m_aTopCategoryPos[i].x2-m_aTopCategoryPos[i].x1, m_aTopCategoryPos[i].y2-m_aTopCategoryPos[i].y1,
			0, BLACK, 
			dFITTEXT_CENTER,
			g_pstrCommonMessage[2208+i]);				
	}
	
	iPutPosX += dSHOP_TOPCATE_BTN_BYMARGIN*2;
	g_sprInterface2.Put(iPutPosX, iPutPosY, eCARROT_MAGNIFIER);
	// 아이템 서치 이미지와 텍스트박스 출력해 줘야 한다.
	
	m_inputSearch.active(0, true);
	m_inputSearch.draw(WHITE);
	
	iPutPosX += (g_sprInterface2.getSpriteWidth(eCARROT_MAGNIFIER)+dSHOP_TOPCATE_BTN_BYMARGIN);
	g_sprInterface2.Put(iPutPosX, iPutPosY, eCARROT_SEARCHBOX);	
	m_rtEditBoxArea.set(iPutPosX, iPutPosY, iPutPosX+g_sprInterface2.getSpriteWidth(eCARROT_SEARCHBOX),iPutPosY+g_sprInterface2.getSpriteHeight(eCARROT_SEARCHBOX));
	
	if(!m_binputState && !m_inputSearch.m_iCount)
		s_text.put(m_rtEditBoxArea.x1+2, m_rtEditBoxArea.y1+2, BLACK, m_strPreSearchValue);		// insu modify
}

int
cCarrotShop::GetItemBoxIndex(int _iItemCount)
{
	if(m_bSelectSearchBoxItem == false)
	{
		for(int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<_iItemCount ; i++)
		{
			if (m_aItemBoxPos[i].isIn(s_posMouse.x, s_posMouse.y))
				return i;
		}
	}
	else
	{
		for (int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<_iItemCount ; i++)
		{
			if (m_aItemBoxPos[i].isIn(s_posMouse.x, s_posMouse.y))
				return i;
		}

	}
	return 0xffff;
}

// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃				Second 카테고리의 버튼을 출력한다.				     ┃
// ┃                                                     			     ┃
// ┃cRECT* _Pos				- 버튼의 영역							 ┃
// ┃int _iBtnIndex				- 해당 버튼의 인덱스					 ┃
// ┃char* _strBtnName			- 버튼의 텍스트							 ┃
// ┃int _iPutPosX				- 찍어줄 x 위치							 ┃
// ┃int _iPutPosY				- 찍어줄 y 위치							 ┃
// ┃int _iSelectBtnImgIndex	- 선택시 찍어줄 이미지 인덱스			 ┃
// ┃int _iOverBtnImgIndex		- 마우스 오버시 찍어줄 이미지 인덱스	 ┃
// ┃int _iNormalBtnImgIndex	- 평소상태시 찍어줄 이미지 인덱스		 ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
void
cCarrotShop::DrawCategoryBtn(cRECT* _Pos, int _iBtnIndex, char* _strBtnName, int _iPutPosX, int _iPutPosY, int _iSelectBtnImgIndex, int _iOverBtnImgIndex, int _iNormalBtnImgIndex)
{
	// 영역을 설정
	_Pos->set(	_iPutPosX, _iPutPosY, 
		_iPutPosX+g_sprInterface2.getSpriteWidth(_iSelectBtnImgIndex), 
		_iPutPosY+g_sprInterface2.getSpriteHeight(_iSelectBtnImgIndex));
	
	// 상태별로 버튼이미지 출력
	if(m_iSelectSecCategory == _iBtnIndex)	// 선택상태
	{
		g_sprInterface2.Put(_iPutPosX, _iPutPosY, _iSelectBtnImgIndex);
		s_text.cPut(	_Pos->x1, _Pos->y1+1,	_Pos->x2-_Pos->x1, _Pos->y2-_Pos->y1,
			0, BLACK,	// insu modify
			dFITTEXT_CENTER,
			_strBtnName);
		iPutPosX += (g_sprInterface2.getSpriteWidth(_iNormalBtnImgIndex)+dSHOP_SECCATE_BTNXMARGIN);
	}
	else if(m_iFocusSecCategory == _iBtnIndex && m_iSelectSecCategory != _iBtnIndex)	// 마우스오버상태 
	{
		g_sprInterface2.Put(_iPutPosX, _iPutPosY, _iOverBtnImgIndex);
		s_text.cPut(	_Pos->x1, _Pos->y1+1,	_Pos->x2-_Pos->x1, _Pos->y2-_Pos->y1,
			0, WHITE,
			dFITTEXT_CENTER,
			_strBtnName);
		iPutPosX += (g_sprInterface2.getSpriteWidth(_iNormalBtnImgIndex)+dSHOP_SECCATE_BTNXMARGIN);
	}
	else	// 평소상태
	{
		g_sprInterface2.Put(_iPutPosX, _iPutPosY, _iNormalBtnImgIndex);
		s_text.cPut(	_Pos->x1, _Pos->y1+1, _Pos->x2-_Pos->x1, _Pos->y2-_Pos->y1,
			0, WHITE,
			dFITTEXT_CENTER,
			_strBtnName);
		iPutPosX += (g_sprInterface2.getSpriteWidth(_iNormalBtnImgIndex)+dSHOP_SECCATE_BTNXMARGIN);
	}
}

void
cCarrotShop::DrawItemBuyCategory()
{
	iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2) +6 + dSHOP_WND_LEFTMARGIN;	// add
	
	// 전체, 인기 카테고리는 따로 출력한다.
	DrawCategoryBtn(&m_aSecCategoryPos[0], 0, dMSG_CARROTSHOP_CATEGORY_ALL, iPutPosX, iPutPosY, eCARROT_SECSBTN_CLICK, eCARROT_SECSBTN_UP, eCARROT_SECSBTN1NORMAL);
	DrawCategoryBtn(&m_aSecCategoryPos[1], 1, dMSG_CARROTSHOP_CATEGORY_HOT, iPutPosX, iPutPosY, eCARROT_SECSBTN_CLICK, eCARROT_SECSBTN_UP, eCARROT_SECSBTN1NORMAL);
	
	// 나머지 카테고리
	for(int i=0 ; i<g_carrotShop.m_wCategoryCount ; i++)
	{
		cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[i];
		
		if( i == dSHOP_SECCATE_1LINE_CATENUM || i == (dSHOP_SECCATE_1LINE_CATENUM+2)*2)
		{
			iPutPosY += (g_sprInterface2.getSpriteHeight(eCARROT_SECSBTN1NORMAL) + dSHOP_DIVIDELINE_YMARGIN*2);
			iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2) +6 + dSHOP_WND_LEFTMARGIN;
		} 
		
		DrawCategoryBtn(&m_aSecCategoryPos[i+2], i+2, lpCategory->m_strName, iPutPosX, iPutPosY, eCARROT_SECSBTN_CLICK, eCARROT_SECSBTN_UP, eCARROT_SECSBTN1NORMAL);
	}	
}


void
cCarrotShop::DrawMainWndDivideLine()
{
	// 구분선 출력 ----> eCARROT_MAINWND_DIVIDELINE
	
	iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2) +6;	// 테두리 선의 픽셀값(6)
	iPutPosY += g_sprInterface2.getSpriteHeight(eCARROT_TOPBTN_NORMAL)+dSHOP_DIVIDELINE_YMARGIN;
	
	g_sprInterface2.Put(iPutPosX, iPutPosY, eCARROT_MAINWND_DIVIDELINE);
	
	iPutPosX += dSHOP_WND_LEFTMARGIN;
	iPutPosY += g_sprInterface2.getSpriteHeight(eCARROT_MAINWND_DIVIDELINE)+dSHOP_DIVIDELINE_YMARGIN;
}


void
cCarrotShop::DrawAllItem()	// 전체카테고리 출력
{
	iPutItemCount = 0;
	
	iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2) +6 + dSHOP_WND_LEFTMARGIN;
	
	// 스크롤바를 출력해주고 
	m_sbScrollBar.draw();
	
	// 스크롤바 라인이 1이상이면 iPutPosY 좌표를 수정하고 
	if(m_sbScrollBar.m_iCurrentPos > 0)
		iPutPosY -= (g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)+iLargeBoxYMargin);
	
	int itempPutPosY = iPutPosY;	// y좌표가 아이템갯수에 따라 유동적으로 변하기때문에 임시로 하나 생성한다.
	
	if(m_sbScrollBar.m_iCurrentPos)
		itempPutPosY += (g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)+iLargeBoxYMargin);
	
	
	if(m_bSelectSearchBoxItem == true && m_iReturnState == 1)	// 결과값 한개인 경우
	{
		DrawItem(	&m_aSearchItem[0], 
			m_aiSearchCateIndex[0], m_aiSearchItemIndex[0], 
			iPutPosX, itempPutPosY, 
			g_carrotShop.m_aData[m_aiSearchCateIndex[0]].m_aItemList[m_aiSearchItemIndex[0]].m_bf1IsHotItem,
			&m_aBuyBtnPos[0], 
			&m_aPresentBtnPos[0], 
			&m_aWishBtnPos[0],
			&m_aToolTipPos[0], &m_aItemBoxPos[0]);
		return;		
	}
	
	if(m_bSelectSearchBoxItem == true && m_iReturnState == 2)	// 검색결과가 여러개 선택안하고 바로 리턴키 
	{		
		// 스크롤바 라인이 1이상이면 iPutPosY 좌표를 수정하고 
		//if(m_sbScrollBar.m_iCurrentPos > 0)
		//	itempPutPosY -= (g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)+iLargeBoxYMargin);
		
		for(int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<m_iSearchCount ; i++)
		{
			if(i%iOneLineItemNum == 0 && i != m_sbScrollBar.m_iCurrentPos*iOneLineItemNum)	// 한줄에 표시될 개수를 넘어가면 출력좌표를 다시 설정해준다.
			{
				iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2) +6 + dSHOP_WND_LEFTMARGIN;
				itempPutPosY += (g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)+iLargeBoxYMargin);	
			}
			
			if(i > m_sbScrollBar.m_iCurrentPos*iOneLineItemNum + iOneLineItemNum*iLineNum-1) // 한 페이지를 넘어가면 그만 찍어준다.
				return;
			
			DrawItem(	&m_aSearchItem[i], 
				m_aiSearchCateIndex[i], m_aiSearchItemIndex[i], 
				iPutPosX, itempPutPosY, 
				g_carrotShop.m_aData[m_aiSearchCateIndex[i]].m_aItemList[m_aiSearchItemIndex[i]].m_bf1IsHotItem,
				&m_aBuyBtnPos[i], 
				&m_aPresentBtnPos[i], 
				&m_aWishBtnPos[i],
				&m_aToolTipPos[i], &m_aItemBoxPos[i]);
			iPutPosX += g_sprInterface2.getSpriteWidth(eCARROT_ITEMWND_BORDER)+iLargeBoxXMargin;
		}
		return;
	}
	
	if(m_bSelectSearchBoxItem == true && m_iReturnState == 3)	// 검색결과가 여러개 무엇인가 선택하고 리턴키. 하나만 출력해주면 된다.
	{
		DrawItem(	&m_aSearchItem[m_iSelectSearchBoxItem], 
			m_aiSearchCateIndex[m_iSelectSearchBoxItem], m_aiSearchItemIndex[m_iSelectSearchBoxItem], 
			iPutPosX, itempPutPosY, 
			g_carrotShop.m_aData[m_aiSearchCateIndex[m_iSelectSearchBoxItem]].m_aItemList[m_aiSearchItemIndex[m_iSelectSearchBoxItem]].m_bf1IsHotItem,
			&m_aBuyBtnPos[m_iSelectSearchBoxItem], 
			&m_aPresentBtnPos[m_iSelectSearchBoxItem], 
			&m_aWishBtnPos[m_iSelectSearchBoxItem],
			&m_aToolTipPos[m_iSelectSearchBoxItem], &m_aItemBoxPos[m_iSelectSearchBoxItem]);
		return;
	}
	// 검색결과가 없을때
	// 일단 신규 아이템부터 출력해주고 .. 
	int iItemCount = 0;	// 스크롤바에 따라 찍어주는 아이템이 다르기 때문에 아이템갯수가 필요하다.
	for(int i=0 ; i<g_carrotShop.m_wCategoryCount ; i++)
	{		
		cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[i];
		for(int j=0; j<lpCategory->m_wItemCount ; j++)
		{
			if(lpCategory->m_aItemList[j].m_bf1IsHotItem) // 신규아이템이다!
			{
				iItemCount++;
				if(m_sbScrollBar.m_iCurrentPos*iOneLineItemNum >= iItemCount) // 스크롤바위치*한줄에찍는아이템갯수가 아이템수에 맞을때까지 넘긴다.
					continue;
				
				if(iPutItemCount > iOneLineItemNum*iLineNum-1) // 한 페이지를 넘어가면 그만 찍어준다.
					return;
				
				cItem item;
				CCustomItem *lpCustomItem = g_premiumItem.getItem(lpCategory->m_aItemList[j].m_wPremiunItemIndex);
				
				if(!lpCustomItem)
					return; // continue;
				
				if(!lpCustomItem->generateItem(&item))
					return; // continue;
				
				if(iPutItemCount%iOneLineItemNum == 0 && iPutItemCount != 0)	// 한줄에 표시될 개수를 넘어가면 출력좌표를 다시 설정해준다.
				{
					iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2) +6 + dSHOP_WND_LEFTMARGIN;
					itempPutPosY += (g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)+iLargeBoxYMargin);	
				}
				
				DrawItem(	&item, i, j, iPutPosX, itempPutPosY, 
					true,
					&m_aBuyBtnPos[m_sbScrollBar.m_iCurrentPos*iOneLineItemNum+iPutItemCount], 
					&m_aPresentBtnPos[m_sbScrollBar.m_iCurrentPos*iOneLineItemNum+iPutItemCount], 
					&m_aWishBtnPos[m_sbScrollBar.m_iCurrentPos*iOneLineItemNum+iPutItemCount],
					&m_aToolTipPos[m_sbScrollBar.m_iCurrentPos*iOneLineItemNum+iPutItemCount],
					&m_aItemBoxPos[m_sbScrollBar.m_iCurrentPos*iOneLineItemNum+iPutItemCount]);
				
				iPutPosX += g_sprInterface2.getSpriteWidth(eCARROT_ITEMWND_BORDER)+iLargeBoxXMargin;
				
				iPutItemCount++;
			}
		}
	}
	// 신규 아이템 출력완료

	if (m_iSelectSecCategory == 1)	// 신규 카테고리라면 나머지를 출력할 필요가 없다. 과감하게 리턴!
		return;

	// 나머지 아이템 출력 시작 ..
	for( i=0 ; i<g_carrotShop.m_wCategoryCount ; i++)
	{
		cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[i];
		
		for(int j=0; j<lpCategory->m_wItemCount ; j++)
		{
			if(!lpCategory->m_aItemList[j].m_bf1IsHotItem) // 신규아이템이 아니다!
			{
				iItemCount++;
				if(m_sbScrollBar.m_iCurrentPos*iOneLineItemNum >= iItemCount)	// 스크롤바위치*한줄에찍는아이템갯수가 아이템수에 맞을때까지 넘긴다.
					continue;
				
				if(iPutItemCount > iOneLineItemNum*iLineNum-1) // 한 페이지를 넘어가면 그만 찍어준다.
					continue;
				
				cItem item;
				CCustomItem *lpCustomItem = g_premiumItem.getItem(lpCategory->m_aItemList[j].m_wPremiunItemIndex);
				
				if(!lpCustomItem)
					return;
				
				if(!lpCustomItem->generateItem(&item))
					return;
				
				if(iPutItemCount%iOneLineItemNum == 0 && iPutItemCount != 0)	// 한줄에 표시될 개수를 넘어가면 출력좌표를 다시 설정해준다.
				{
					iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2) +6 + dSHOP_WND_LEFTMARGIN;
					itempPutPosY += (g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)+iLargeBoxYMargin);						
				}					
				DrawItem(	&item, i, j, iPutPosX, itempPutPosY, 
					false,
					&m_aBuyBtnPos[m_sbScrollBar.m_iCurrentPos*iOneLineItemNum+iPutItemCount], 
					&m_aPresentBtnPos[m_sbScrollBar.m_iCurrentPos*iOneLineItemNum+iPutItemCount], 
					&m_aWishBtnPos[m_sbScrollBar.m_iCurrentPos*iOneLineItemNum+iPutItemCount],
					&m_aToolTipPos[m_sbScrollBar.m_iCurrentPos*iOneLineItemNum+iPutItemCount],
					&m_aItemBoxPos[m_sbScrollBar.m_iCurrentPos*iOneLineItemNum+iPutItemCount]);
				
				iPutPosX += g_sprInterface2.getSpriteWidth(eCARROT_ITEMWND_BORDER)+iLargeBoxXMargin;
				
				iPutItemCount++;
			}
		}
	}
	return;
}


// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃         하나의 아이템을 박스쳐서 출력한다. 버튼포함			     ┃
// ┃                                                     			     ┃
// ┃cItem* _Item			- 해당 아이템								 ┃
// ┃int _iCategoryIndex	- 해당 아이템의 카테고리 인덱스				 ┃
// ┃int _ItemListIndex		- 해당 아이템의 아이템리스트 인덱스			 ┃
// ┃int _iPutPosX			- 찍어줄 x 위치								 ┃
// ┃int _iPutPosY			- 찍어줄 y 위치								 ┃
// ┃cRECT _BuyBtnPos		- 구입버튼의 영역을 저장할 사각형좌표		 ┃
// ┃cRECT _PresentBtnPos	- 선물버튼의 영역을 저장할 사각형좌표		 ┃
// ┃cRECT _WishBtnPos		- 위시리스트버튼의 영역을 저장할 사각형 좌표 ┃
// ┃cRECT* _ToolTipPos		- 아이템 툴팁띄우기 위한 조건좌표			 ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

void
cCarrotShop::DrawItem(cItem* _Item, int _iCategoryIndex, int _ItemListIndex, int _iPutPosX, int _iPutPosY, bool _bHot, cRECT* _BuyBtnPos, cRECT* _PresentBtnPos, cRECT* _WishBtnPos, cRECT* _ToolTipPos, cRECT* _ItemBoxPos)
{
	cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[_iCategoryIndex];
	
	cBasicItem *lpBaseItem = _Item->getBasicItem();
	
// 	_Item->m_bCount = lpCategory->m_aItemList[_ItemListIndex].m_bf8ItemCount;
	
	_ItemBoxPos->set(_iPutPosX+1, _iPutPosY+1, _iPutPosX+g_sprInterface2.getSpriteWidth(eCARROT_ITEMWND_BORDER)-2, _iPutPosY+g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)-2);
	if (_iPutPosX+1 == m_aItemBoxPos[m_iFocusItemBoxIndex].x1 && _iPutPosY+1 == m_aItemBoxPos[m_iFocusItemBoxIndex].y1)
		cDRAW::Fill(_LTBLUE, _iPutPosX+1, _iPutPosY+1, _iPutPosX+g_sprInterface2.getSpriteWidth(eCARROT_ITEMWND_BORDER)-2, _iPutPosY+g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)-2);	

	g_sprInterface2.Put(_iPutPosX, _iPutPosY, eCARROT_ITEMWND_BORDER);			// 큰 박스
	PutItem(_Item, _iPutPosX+iSmallBoxMargin, _iPutPosY+iSmallBoxMargin, TRUE);		// 해당 아이템 이미지
	g_sprInterface.Put(_iPutPosX+iSmallBoxMargin, _iPutPosY+iSmallBoxMargin, eIWI_ITEM_SLOT_FRAME);		// 아이템이미지를 둘러쌀 작은박스모양 이미지
	_ToolTipPos->set(	_iPutPosX+iSmallBoxMargin, 
		_iPutPosY+iSmallBoxMargin,
		_iPutPosX+iSmallBoxMargin+g_sprInterface.getSpriteWidth(eIWI_ITEM_SLOT_FRAME),
		_iPutPosY+iSmallBoxMargin+g_sprInterface.getSpriteHeight(eIWI_ITEM_SLOT_FRAME));
	
	// 아이템명 출력
	DrawCarrotText(	_iPutPosX+g_sprInterface.getSpriteHeight(eIWI_ITEM_SLOT_FRAME)+(iBoxinMargin*2),
		_iPutPosY+(iBoxinMargin*2), 
		WHITE, lpBaseItem->m_strName, 130);
	
	// 당근이미지 출력
	g_sprRegionInterface.Put(	_iPutPosX+g_sprInterface.getSpriteHeight(eIWI_ITEM_SLOT_FRAME)+(iBoxinMargin*2), 
								_iPutPosY+(iBoxinMargin*9), 
								eCSWI_CARROT);
	
	// 신규 아이템이라면 hot아이콘 출력 
	if(_bHot)
		g_sprInterface2.Put(_iPutPosX-10, _iPutPosY, eCARROT_HOT_ICON1);
	
	// 당근 가격 출력
	s_text.put( _iPutPosX+g_sprInterface.getSpriteHeight(eIWI_ITEM_SLOT_FRAME)+(iBoxinMargin*8), 
		_iPutPosY+(iBoxinMargin*9), 
		WHITE, "%d", lpCategory->m_aItemList[_ItemListIndex].m_wCarrotCount);		
	// 구분선 출력
	g_sprInterface2.Put(_iPutPosX+1, _iPutPosY+40, eCARROT_ITEMWND_DIVIDELINE);
				
	// 버튼 3개 출력, 버튼마다 위치정보를 저장한다.

	_iPutPosY += 48;
	// 구입버튼			
	_BuyBtnPos->set(_iPutPosX+12, _iPutPosY, 
		_iPutPosX + 12 + g_sprInterface2.getSpriteWidth(eCARROT_ITEMSBTN_NORMAL), 
		_iPutPosY + g_sprInterface2.getSpriteHeight(eCARROT_ITEMSBTN_NORMAL));
	if(!_BuyBtnPos->isIn(s_posMouse.x,s_posMouse.y))
		g_sprInterface2.Put(_iPutPosX+12, _iPutPosY, eCARROT_ITEMSBTN_NORMAL);
	else
		g_sprInterface2.Put(_iPutPosX+12, _iPutPosY, eCARROT_ITEMSBTN_UP);
	s_text.cPut(	_BuyBtnPos->x1, _BuyBtnPos->y1+1,
		_BuyBtnPos->x2-_BuyBtnPos->x1, _BuyBtnPos->y2-_BuyBtnPos->y1,
		0, CARROT_TCOLOR2,
		dFITTEXT_CENTER,
		dMSG_CARROTSHOP_BUY);
	
	
	// 선물버튼			
	_PresentBtnPos->set(_iPutPosX+54, _iPutPosY, 
		_iPutPosX + 54 + g_sprInterface2.getSpriteWidth(eCARROT_ITEMSBTN_NORMAL), 
		_iPutPosY + g_sprInterface2.getSpriteHeight(eCARROT_ITEMSBTN_NORMAL));
	if(!_PresentBtnPos->isIn(s_posMouse.x,s_posMouse.y))
		g_sprInterface2.Put(_iPutPosX+54, _iPutPosY, eCARROT_ITEMSBTN_NORMAL);
	else
		g_sprInterface2.Put(_iPutPosX+54, _iPutPosY, eCARROT_ITEMSBTN_UP);
	s_text.cPut(	_PresentBtnPos->x1, _PresentBtnPos->y1+1,
		_PresentBtnPos->x2-_PresentBtnPos->x1, _PresentBtnPos->y2-_PresentBtnPos->y1,
		0, CARROT_TCOLOR2,
		dFITTEXT_CENTER,
		dMSG_CARROTSHOP_PRESENT);
	
	// 위시리스트버튼			
	_WishBtnPos->set(_iPutPosX+96, _iPutPosY, 
		_iPutPosX + 96 + g_sprInterface2.getSpriteWidth(eCARROT_ITEMLBTN_NORMAL), 
		_iPutPosY + g_sprInterface2.getSpriteHeight(eCARROT_ITEMLBTN_NORMAL));
	if(!_WishBtnPos->isIn(s_posMouse.x,s_posMouse.y))
		g_sprInterface2.Put(_iPutPosX+96, _iPutPosY, eCARROT_ITEMLBTN_NORMAL);
	else
		g_sprInterface2.Put(_iPutPosX+96, _iPutPosY, eCARROT_ITEMLBTN_UP);
	s_text.cPut(	_WishBtnPos->x1, _WishBtnPos->y1+1,
		_WishBtnPos->x2-_WishBtnPos->x1, _WishBtnPos->y2-_WishBtnPos->y1,
		0, CARROT_TCOLOR2,
		dFITTEXT_CENTER,
		dMSG_CARROTSHOP_WISHLIST);
	
	_iPutPosX += (g_sprInterface2.getSpriteWidth(eCARROT_ITEMWND_BORDER)+iLargeBoxXMargin);
	_iPutPosY -= 47;
}




void
cCarrotShop::DrawItemList()
{
	if(m_bSelectSearchBoxItem == false)		// 검색결과 없다.
	{
		// 스크롤에 따라 해당 카테고리 아이템 리스트를 출력한다.
		cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[m_iSelectSecCategory-2];
		
		// 스크롤바를 출력해주고 
		m_sbScrollBar.draw();
		
		// 스크롤바 라인이 1이상이면 iPutPosY 좌표를 수정하고 
		if(m_sbScrollBar.m_iCurrentPos > 0)
			iPutPosY -= (g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)+iLargeBoxYMargin);
		
		int itempPutPosY = iPutPosY;	// y좌표가 아이템갯수에 따라 유동적으로 변하기때문에 임시로 하나 생성한다.
		
		// 아이템리스트를 출력해준다.
		for(int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<lpCategory->m_wItemCount ; i++)
		{		
			cItem item;
			CCustomItem *lpCustomItem = g_premiumItem.getItem(lpCategory->m_aItemList[i].m_wPremiunItemIndex);
			
			if(!lpCustomItem)
				return;
			
			if(!lpCustomItem->generateItem(&item))
				return;
			
			if(i+1 > (iOneLineItemNum*iLineNum) + (m_sbScrollBar.m_iCurrentPos*iOneLineItemNum))
				break;
			
			cBasicItem *lpBaseItem = item.getBasicItem();
			
			item.m_bCount	=	g_premiumItem.getItem(lpCategory->m_aItemList[i].m_wPremiunItemIndex)->m_bCount;
			
			if( i%iOneLineItemNum == 0 && i != 0)	// 한줄에 표시될 개수를 넘어가면 출력좌표를 다시 설정해준다.
			{
				iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2) +6 + dSHOP_WND_LEFTMARGIN;
				itempPutPosY += (g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)+iLargeBoxYMargin);
			}
			
			DrawItem(&item, m_iSelectSecCategory-2, i, iPutPosX, itempPutPosY, lpCategory->m_aItemList[i].m_bf1IsHotItem, &m_aBuyBtnPos[i], &m_aPresentBtnPos[i], &m_aWishBtnPos[i], &m_aToolTipPos[i], &m_aItemBoxPos[i]);
			iPutPosX += (g_sprInterface2.getSpriteWidth(eCARROT_ITEMWND_BORDER)+iLargeBoxXMargin);			
		}
	}
	else		// 검색결과 있다.
	{
		cCarrotShopCategoryInfoDefine	*lpCategory	=	&g_carrotShop.m_aData[m_iSelectSecCategory-2];
		int itempPutPosY = iPutPosY;
		
		if(m_bSelectSearchBoxItem == true && m_iReturnState == 1)	// 결과값 한개인 경우
		{
			DrawItem(	&m_aSearchItem[0], 
				m_aiSearchCateIndex[0], m_aiSearchItemIndex[0], 
				iPutPosX, itempPutPosY,
				g_carrotShop.m_aData[m_aiSearchCateIndex[0]].m_aItemList[m_aiSearchItemIndex[0]].m_bf1IsHotItem,
				&m_aBuyBtnPos[0], 
				&m_aPresentBtnPos[0], 
				&m_aWishBtnPos[0],
				&m_aToolTipPos[0],
				&m_aItemBoxPos[0]);
			return;		
		}
		
		if(m_bSelectSearchBoxItem == true && m_iReturnState == 2)	// 검색결과가 여러개 선택안하고 바로 리턴키 
		{		
			for(int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<m_iSearchCount ; i++)
			{
				if(i%iOneLineItemNum == 0 && i != 0)	// 한줄에 표시될 개수를 넘어가면 출력좌표를 다시 설정해준다.
				{
					iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2) +6 + dSHOP_WND_LEFTMARGIN;
					itempPutPosY += (g_sprInterface2.getSpriteHeight(eCARROT_ITEMWND_BORDER)+iLargeBoxYMargin);	
				}
				
				if(i > m_sbScrollBar.m_iCurrentPos*iOneLineItemNum + iOneLineItemNum*iLineNum-1) // 한 페이지를 넘어가면 그만 찍어준다.
					return;
				
				DrawItem(	&m_aSearchItem[i], 
					m_aiSearchCateIndex[i], m_aiSearchItemIndex[i], 
					iPutPosX, itempPutPosY, 
					g_carrotShop.m_aData[m_aiSearchCateIndex[i]].m_aItemList[m_aiSearchItemIndex[i]].m_bf1IsHotItem,
					&m_aBuyBtnPos[i], 
					&m_aPresentBtnPos[i], 
					&m_aWishBtnPos[i],
					&m_aToolTipPos[i],
					&m_aItemBoxPos[i]);
				iPutPosX += g_sprInterface2.getSpriteWidth(eCARROT_ITEMWND_BORDER)+iLargeBoxXMargin;
			}
			return;
		}
		
		if(m_bSelectSearchBoxItem == true && m_iReturnState == 3)	// 검색결과가 여러개 무엇인가 선택하고 리턴키. 하나만 출력해주면 된다.
		{
			DrawItem(	&m_aSearchItem[m_iSelectSearchBoxItem],
				m_aiSearchCateIndex[m_iSelectSearchBoxItem], m_aiSearchItemIndex[m_iSelectSearchBoxItem], 
				iPutPosX, itempPutPosY, 
				g_carrotShop.m_aData[m_aiSearchCateIndex[m_iSelectSearchBoxItem]].m_aItemList[m_aiSearchItemIndex[m_iSelectSearchBoxItem]].m_bf1IsHotItem,
				&m_aBuyBtnPos[m_iSelectSearchBoxItem], 
				&m_aPresentBtnPos[m_iSelectSearchBoxItem], 
				&m_aWishBtnPos[m_iSelectSearchBoxItem],
				&m_aToolTipPos[m_iSelectSearchBoxItem],
				&m_aItemBoxPos[m_iSelectSearchBoxItem]);
			return;
		}
	}
}



void
cCarrotShop::DrawWishList()
{
	// 위시 리스트 부분 출력
	int iWishListCnt = 10;
	int iWiishBoxYMargin = 1;
	
	s_text.put(iPutPosX, iPutPosY, CARROT_TCOLOR3, "%s", dMSG_CARROTSHOP_WISHLIST);		// "위시 리스트"	
	
	s_text.put(iPutPosX+(g_sprInterface.getSpriteWidth(eIWI_ITEM_SLOT_FRAME)+iWiishBoxYMargin)*iWishListCnt+10, iPutPosY, CTBLUE, "%s", dMSG_CARROTSHOP_REMAINCARROT);	// "현재 보유 당근 개수"
	g_sprRegionInterface.Put(iPutPosX+(g_sprInterface.getSpriteWidth(eIWI_ITEM_SLOT_FRAME)+iWiishBoxYMargin)*iWishListCnt+120, iPutPosY, eCSWI_CARROT);		// 당근이미지 출력
	s_text.put(iPutPosX+(g_sprInterface.getSpriteWidth(eIWI_ITEM_SLOT_FRAME)+iWiishBoxYMargin)*iWishListCnt+120+g_sprRegionInterface.getSpriteWidth(eCSWI_CARROT)+8, iPutPosY ,WHITE, "%d", m_iRemainCarrotCount);	// 당근 개수
	
	s_text.put(iPutPosX+(g_sprInterface.getSpriteWidth(eIWI_ITEM_SLOT_FRAME)+iWiishBoxYMargin)*iWishListCnt+10, iPutPosY+20, CARROT_TCOLOR4, "%s", dMSG_CARROTSHOP_BILLCARROT);	// "결제 필요 당근 개수"
	g_sprRegionInterface.Put(iPutPosX+(g_sprInterface.getSpriteWidth(eIWI_ITEM_SLOT_FRAME)+iWiishBoxYMargin)*iWishListCnt+120, iPutPosY+20, eCSWI_CARROT);		// 당근이미지 출력
	s_text.put(iPutPosX+(g_sprInterface.getSpriteWidth(eIWI_ITEM_SLOT_FRAME)+iWiishBoxYMargin)*iWishListCnt+120+g_sprRegionInterface.getSpriteWidth(eCSWI_CARROT)+8, iPutPosY+20 ,WHITE, "%d", m_iTotalPayCarrot);	// 당근 개수
	
	
	
	iPutPosY += 15;
	int i=0 ;
	for(i=0 ; i<iWishListCnt ; i++)
	{
		g_sprInterface.Put(iPutPosX, iPutPosY, eIWI_ITEM_SLOT_FRAME);		// 아이템이미지를 둘러쌀 작은박스모양 이미지
		iPutPosX += g_sprInterface.getSpriteWidth(eIWI_ITEM_SLOT_FRAME)+iWiishBoxYMargin;
	}
	
	iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2) + dSHOP_WND_LEFTMARGIN;
	
	for(i=0 ; i<m_iWishListCount ; i++)
	{
		PutItem(&m_aWishListItem[i], iPutPosX+6, iPutPosY, TRUE);		// 해당 아이템 이미지
		m_WishListItemPos[i].set(	iPutPosX+6, 
			iPutPosY, 
			iPutPosX+6+g_sprInterface.getSpriteWidth(eIWI_ITEM_SLOT_FRAME), 
			iPutPosY+g_sprInterface.getSpriteHeight(eIWI_ITEM_SLOT_FRAME));
		iPutPosX += g_sprInterface.getSpriteWidth(eIWI_ITEM_SLOT_FRAME)+iWiishBoxYMargin;
	}
	
	for(i=0 ; i<m_iWishListCount ; i++)		// 마우스위치에 따라 x박스 출력
	{
		if(m_WishListItemPos[i].isIn(s_posMouse.x, s_posMouse.y) && !g_msgBox.isPoped())
			g_sprInterface2.Put(m_WishListItemPos[i].x1-2, m_WishListItemPos[i].y1-1, eCARROT_WISHLIST_DELETE);
	}
	
	
	// 위시리스트 버튼 출력
	iPutPosX = (g_iScreenWidth/2) - (m_iWindowWidth/2) + dSHOP_WND_LEFTMARGIN;
	iPutPosX += (g_sprInterface.getSpriteWidth(eIWI_ITEM_SLOT_FRAME)+iWiishBoxYMargin)*iWishListCnt;
	iPutPosY += 20;
	
	// 구입버튼	
	m_WishBuyBtnPos.set(iPutPosX+12, iPutPosY, 
		iPutPosX+12+ g_sprInterface2.getSpriteWidth(eCARROT_ITEMSBTN_NORMAL), 
		iPutPosY+g_sprInterface2.getSpriteHeight(eCARROT_ITEMSBTN_NORMAL));
	if(!m_WishBuyBtnPos.isIn(s_posMouse.x,s_posMouse.y))
		g_sprInterface2.Put(iPutPosX+12, iPutPosY, eCARROT_ITEMSBTN_NORMAL);
	else
		g_sprInterface2.Put(iPutPosX+12, iPutPosY, eCARROT_ITEMSBTN_UP);
	s_text.cPut(	m_WishBuyBtnPos.x1, m_WishBuyBtnPos.y1+1,
								m_WishBuyBtnPos.x2-m_WishBuyBtnPos.x1, m_WishBuyBtnPos.y2-m_WishBuyBtnPos.y1,
								0, CARROT_TCOLOR2,
								dFITTEXT_CENTER,
								dMSG_CARROTSHOP_BUY);
	
	// 선물버튼
	
	m_WishPresentBtnPos.set(iPutPosX+54, iPutPosY, 
		iPutPosX+54+g_sprInterface2.getSpriteWidth(eCARROT_ITEMSBTN_NORMAL), 
		iPutPosY+g_sprInterface2.getSpriteHeight(eCARROT_ITEMSBTN_NORMAL));
	if(!m_WishPresentBtnPos.isIn(s_posMouse.x,s_posMouse.y))
		g_sprInterface2.Put(iPutPosX+54, iPutPosY, eCARROT_ITEMSBTN_NORMAL);
	else
		g_sprInterface2.Put(iPutPosX+54, iPutPosY, eCARROT_ITEMSBTN_UP);
	s_text.cPut(	m_WishPresentBtnPos.x1, m_WishPresentBtnPos.y1+1,
								m_WishPresentBtnPos.x2-m_WishPresentBtnPos.x1, m_WishPresentBtnPos.y2-m_WishPresentBtnPos.y1,
								0, CARROT_TCOLOR2,
								dFITTEXT_CENTER,
								dMSG_CARROTSHOP_PRESENT);
	
	// 모두 비우기버튼 
	
	m_WishDeleteBtnPos.set(iPutPosX+96, iPutPosY, 
		iPutPosX+96+g_sprInterface2.getSpriteWidth(eCARROT_ITEMLBTN_NORMAL), 
		iPutPosY+g_sprInterface2.getSpriteHeight(eCARROT_ITEMLBTN_NORMAL));
	if(!m_WishDeleteBtnPos.isIn(s_posMouse.x,s_posMouse.y))
		g_sprInterface2.Put(iPutPosX+96, iPutPosY, eCARROT_ITEMLBTN_NORMAL);
	else
		g_sprInterface2.Put(iPutPosX+96, iPutPosY, eCARROT_ITEMLBTN_UP);
	s_text.cPut(	m_WishDeleteBtnPos.x1, m_WishDeleteBtnPos.y1+1,
		m_WishDeleteBtnPos.x2-m_WishDeleteBtnPos.x1, m_WishDeleteBtnPos.y2-m_WishDeleteBtnPos.y1,
		0, CARROT_TCOLOR2,
		dFITTEXT_CENTER,
		dMSG_CARROTSHOP_WISHLIST_DELETE);
}



void
cCarrotShop::SearchAutoComplete()
{
	int iIndex = 0;
	m_iSearchCount = 0;
	
	if (m_iSelectSecCategory == 0 || m_iSelectSecCategory == 1)	// 전체 카테고리 || 신규 카테고리
	{
		for(int i=0; i<g_carrotShop.m_wCategoryCount ; i++)
		{
			for(int j=0; j<g_carrotShop.m_aData[i].m_wItemCount ; j++)
			{
				CCustomItem *lpCustomItem = g_premiumItem.getItem(g_carrotShop.m_aData[i].m_aItemList[j].m_wPremiunItemIndex);	// 아이템인덱스를 받아온다.
				
				if(!lpCustomItem)
					return;
				
				if(!lpCustomItem->generateItem(&m_aSearchItem[m_iSearchCount]))
					return;
				
				cBasicItem *lpBaseItem = m_aSearchItem[m_iSearchCount].getBasicItem();	// 해당 아이템의 정보를 받아서 
				Str_toupper(m_strSearchValue);
				if(m_iSelectSecCategory == 0 && strstr(lpBaseItem->m_strName, m_strSearchValue))	// 해당 문자열이 있다면 m_iSearchCount를 증가시킨다.
				{
					m_iSearchCount++;
					m_aiSearchCateIndex[iIndex] = i;
					m_aiSearchItemIndex[iIndex] = j;		
					iIndex++;
				}
				
				if(m_iSelectSecCategory == 1 && strstr(lpBaseItem->m_strName, m_strSearchValue) && g_carrotShop.m_aData[i].m_aItemList[j].m_bf1IsHotItem == true)
				{
					m_iSearchCount++;
					m_aiSearchCateIndex[iIndex] = i;
					m_aiSearchItemIndex[iIndex] = j;		
					iIndex++;
				}
			}
		}
	}
	else
	{
		for(int i=0; i<g_carrotShop.m_aData[m_iSelectSecCategory-2].m_wItemCount ; i++)
		{			
			CCustomItem *lpCustomItem = g_premiumItem.getItem(g_carrotShop.m_aData[m_iSelectSecCategory-2].m_aItemList[i].m_wPremiunItemIndex);	// 아이템인덱스를 받아온다.
			
			if(!lpCustomItem)
				return;
			
			if(!lpCustomItem->generateItem(&m_aSearchItem[m_iSearchCount]))
				return;
			
			cBasicItem *lpBaseItem = m_aSearchItem[m_iSearchCount].getBasicItem();	// 해당 아이템의 정보를 받아서 
			Str_toupper(m_strSearchValue);
			if(strstr(lpBaseItem->m_strName, m_strSearchValue))	// 해당 문자열이 있다면 m_iSearchCount를 증가시킨다.
			{
				m_iSearchCount++;
				m_aiSearchCateIndex[iIndex] = m_iSelectSecCategory-2;
				m_aiSearchItemIndex[iIndex] = i;		
				iIndex++;
			}
		}
	}
	
	if(m_iSearchCount >= 5)	// 한페이지에 표시되는 목록수는 5개!
		m_sbSearchScrollBar.setSize((m_iSearchCount-5)*2, m_iSearchCount-5);
	else
		m_sbSearchScrollBar.setSize(0, 0);
	
	m_sbSearchScrollBar.setPos(0);
}




void
cCarrotShop::DrawSearchResult()
{
	if(m_bShowResultState)
	{		
		iPutPosX = 	(g_iScreenWidth/2) - (m_iWindowWidth/2);
		int iAddY = 0;
		if (g_iScreenWidth != 800)
		{
			iAddY = (g_iScreenHeight-600)/2;
		}		

		iPutPosY = 40+iAddY;	//  계산하기 힘들다;; 고정으로..
		
		cDRAW::Fill  (	0xffff, iPutPosX+420+2, iPutPosY, 
			iPutPosX+420+2+g_sprInterface2.getSpriteWidth(eCARROT_SEARCHBOX_RESULT)-2,
			iPutPosY+g_sprInterface2.getSpriteHeight(eCARROT_SEARCHBOX_RESULT)-2);
		m_rtSearchResultArea.set(	iPutPosX+420+2, iPutPosY,
			iPutPosX+420+2+g_sprInterface2.getSpriteWidth(eCARROT_SEARCHBOX_RESULT)-2,
			iPutPosY+g_sprInterface2.getSpriteHeight(eCARROT_SEARCHBOX_RESULT)-2);
		
		g_sprInterface2.Put(iPutPosX+421, iPutPosY, eCARROT_SEARCHBOX_RESULT);	// 테두리를 그려준다.
		
		
		int itempPosX = iPutPosX+420+2;
		int itempPoxY = iPutPosY+6;
		int i=0;
		// 선택되었다는 표시를 그려준다!
		for(i=0; i<5 ; i++)
		{
			if(i == m_iSearchItemIndexCount)
			{
				g_sprInterface2.Put(m_SelectSearchResultBox[i].x1, m_SelectSearchResultBox[i].y1-5, eCARROT_SEARCHBOX_SELECT);						
			}
		}
		
		// 검색된 결과를 출력해준다.
		int iPosX=0;
		for(i=m_sbSearchScrollBar.m_iCurrentPos; i<m_iSearchCount ; i++)
		{
			cBasicItem *lpBaseItem = m_aSearchItem[i].getBasicItem();
			// 마우스 포인터 위치에 따라 이걸 선택하고 있다 라는 포인트를 줘야 한다. eCARROT_SEARCHBOX_SELECT			
			m_SelectSearchResultBox[iPosX].set(	itempPosX,
				itempPoxY+(iPosX*17), 
				itempPosX+g_sprInterface2.getSpriteWidth(eCARROT_SEARCHBOX_SELECT)-g_sprInterface.getSpriteWidth(eSCROLLBAR_VERTICAL_NORMAL_UP_BUTTON),
				itempPoxY+(iPosX*17)+17);
			
			s_text.pushShadowStatus(FALSE);
			//s_text.put(m_SelectSearchResultBox[iPosX].x1+5, m_SelectSearchResultBox[iPosX].y1-2, BLACK, "%s", lpBaseItem->m_strName);
			DrawCarrotText(	m_SelectSearchResultBox[iPosX].x1+5, m_SelectSearchResultBox[iPosX].y1-2,
				BLACK, lpBaseItem->m_strName, 
				m_SelectSearchResultBox[iPosX].x2 - m_SelectSearchResultBox[iPosX].x1);
			s_text.popShadowStatus();
			
			iPosX++;
			
			if(i == m_sbSearchScrollBar.m_iCurrentPos+4)
				break;
		}
		
		m_sbSearchScrollBar.draw();	// 스크롤바를 그려준다.
	}
}


bool
cCarrotShop::WishBuyBtnEvent()
{
	g_msgBox.cPopup("", dMSG_READY, dMSG_OK);
	return true;
	
	if(m_wIsWaitResult)
	{
		AddSystemMessage(LTYELLOW,dMSG_WAIT_FOR_RESULT_OF_BUY_ITEM);
		return false;
	}
	
	if(m_iWishListCount == 0)
	{
		g_msgBox.cPopup("",dMSG_CARROTSHOP_WISHLIST_ZERO,dMSG_YES,dMSG_NO);
		return true;
	}
	
	cItem		item;
	char strText[1024]="\0";
	
	for(int i=0 ; i<m_iWishListCount ; i++)		// 위시리스트 내 아이템 수만큼 반복
	{
		char strTemp[512];
		sprintf(strTemp, dMSG_CARROTSHOP_BUY_SHORT_MENT, m_aWishListItem[i].getName(), m_aWishListItem[i].m_bCount);
		strcat(strText, strTemp);		
	}
	sprintf(strText, dMSG_CARROTSHOP_BUY_LONG_MENT, strText, m_iTotalPayCarrot);
	
	cRECT rectOutput = rectOutput = g_msgBox.getrectOutput();
	int iStrHeight = s_text.getTextHeight(g_msgBox.getrectOutput().x2-g_msgBox.getrectOutput().x1, strText);
	if(iStrHeight > g_msgBox.getrectOutput().y2 - g_msgBox.getrectOutput().y1)
	{			
		g_msgBox.setrectOutput(g_msgBox.getrectOutput().x1, g_msgBox.getrectOutput().y1, g_msgBox.getrectOutput().x2, g_msgBox.getrectOutput().y1+iStrHeight);
	}
	g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
	g_msgBox.setrectOutput(&rectOutput);
	g_msgBox.setButtonId(eMBI_BUY_CARROT_SHOP_ITEM);
	return true;
}

bool
cCarrotShop::BuyBtnEvent(cCarrotShopCategoryInfoDefine* _lpCategory, int _iCateIndex, int _iItemIndex)
{
	if(m_wIsWaitResult)
	{
		if (!g_msgBox.isPoped())
			g_msgBox.cPopup("", dMSG_WAIT_FOR_RESULT_OF_BUY_ITEM, dMSG_OK);
		return false;
	}
	
	cItem		item;
	CCustomItem	*lpCustomItem	=	g_premiumItem.getItem(_lpCategory->m_aItemList[_iItemIndex].m_wPremiunItemIndex);
	
	if	(!lpCustomItem)
	{
		return false;
	}
	
	if	(!lpCustomItem->generateItem(&item))
	{
		return false;
	}
	
// 	item.m_bCount=	g_carrotShop.m_aData[_iCateIndex].m_aItemList[_iItemIndex].m_bf8ItemCount;
	
	int		iPrice	=	_lpCategory->m_aItemList[_iItemIndex].m_wCarrotCount;
	char	strText[512];
	
	sprintf(strText,dMSG_DO_YOU_WANT_BUY_FOLLOW_CARROT_SHOP_ITEM_FORM,item.getName(),item.m_bCount,iPrice,"");

	switch(g_iCarrotShopType)
	{
		case	eCarrotShop_AllItemToInventory	:
			if	(item.isLimitTermItem())
				sprintf(strText,dMSG_DO_YOU_WANT_BUY_FOLLOW_CARROT_SHOP_ITEM_FORM,item.getName(),item.m_bCount,iPrice,dCARROT_SHOP_ITEM_TERM_WARNING);
			break;

		case	eCarrotShop_IDPublicItemToCart	:
			if	(item.getBasicItem()->m_wIsIDPublicItem	==	FALSE	&&	item.isLimitTermItem())
				sprintf(strText,dMSG_DO_YOU_WANT_BUY_FOLLOW_CARROT_SHOP_ITEM_FORM,item.getName(),item.m_bCount,iPrice,dCARROT_SHOP_ITEM_TERM_WARNING);
			break;
	}
	
	if(m_bSelectSearchBoxItem == false)
	{
		m_iBuyItem		=	_iItemIndex;
		m_iBuyCategory	=	_iCateIndex;
	}
	else
	{
		m_iBuyItem		=	m_aiSearchItemIndex[m_iSelectSearchBoxItem];
		m_iBuyCategory	=	m_aiSearchCateIndex[m_iSelectSearchBoxItem];
	}
	
	g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_BUY_CARROT_SHOP_ITEM);

	return true;
}



int
cCarrotShop::GetFocusTopCategory()
{
	for	(int i=0; i<4; i++)
	{
		if	(m_aTopCategoryPos[i].isIn(s_posMouse.x,s_posMouse.y))
			return	i;
	}
	
	return	0xffff;
}


int
cCarrotShop::GetFocusSecCategory()
{
	for	(int i=0;i<g_carrotShop.m_wCategoryCount+2;i++)	// 전체, 신규 카테고리는 포함되어 있는 것이 아니므로 추가.
	{
		if	(m_aSecCategoryPos[i].isIn(s_posMouse.x,s_posMouse.y))
			return	i;
	}
	
	return	0xffff;
}

int
cCarrotShop::GetClickBuyBntIndex(int _iItemCount)
{
	if(m_bSelectSearchBoxItem == false)
	{
		for (int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<_iItemCount ; i++)
		{
			if (m_aBuyBtnPos[i].isIn(s_posMouse.x, s_posMouse.y))
				return i;
		}
	}
	else
	{
		for (int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<_iItemCount ; i++)
		{
			if (m_aBuyBtnPos[i].isIn(s_posMouse.x, s_posMouse.y))
				return i;
		}
	}
	return	0xffff;
}

int
cCarrotShop::GetClickPresentBntIndex(int _iItemCount)
{
	if(m_bSelectSearchBoxItem == false)
	{
		for (int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<_iItemCount ; i++)
		{
			if (m_aPresentBtnPos[i].isIn(s_posMouse.x, s_posMouse.y))
				return i;
		}
	}
	else
	{
		for (int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<_iItemCount ; i++)
		{
			if (m_aPresentBtnPos[i].isIn(s_posMouse.x, s_posMouse.y))
				return i;
		}
	}
	return	0xffff;
}

int
cCarrotShop::GetClickWishBntIndex(int _iItemCount)
{
	if(m_bSelectSearchBoxItem == false)
	{
		for (int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<_iItemCount ; i++)
		{
			if (m_aWishBtnPos[i].isIn(s_posMouse.x, s_posMouse.y))
				return i;
		}
	}
	else
	{
		for (int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<_iItemCount ; i++)
		{
			if (m_aWishBtnPos[i].isIn(s_posMouse.x, s_posMouse.y))
				return i;
		}
	}
	return	0xffff;
}

int
cCarrotShop::GetToolTipIndex(int _iItemCount)
{
	if(m_bSelectSearchBoxItem == false)
	{
		for(int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<_iItemCount ; i++)
		{
			if(m_aToolTipPos[i].isIn(s_posMouse.x, s_posMouse.y))
				return i;
		}
	}
	else
	{
		for (int i=m_sbScrollBar.m_iCurrentPos*iOneLineItemNum ; i<_iItemCount ; i++)
		{
			if (m_aToolTipPos[i].isIn(s_posMouse.x, s_posMouse.y))
				return i;
		}
	}
	return 0xffff;
}

// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃     텍스트 출력시 _iLimitSizeX 값을 넘어가면 ... 으로 출력한다.    ┃
// ┃                                                                    ┃
// ┃int _iPutPosX		- 찍어줄 x 위치									 ┃
// ┃int _iPutPosY		- 찍어줄 y 위치									 ┃
// ┃int _color			- 문자열 기본색상								 ┃
// ┃char* _strText		- 출력할 문자열									 ┃
// ┃int _iLimitSizeX	- 출력한계값									 ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
void
cCarrotShop::DrawCarrotText(int _iPutPosX, int _iPutPosY, int _color, char* _strText, int _iLimitSizeX)
{
	char str[dITEM_NAME_LENGTH];
	char* p = _strText;
	char* pNext = NULL;
	char acTemp[3], acTemp2[3];
	
	memset(str, '\0', dITEM_NAME_LENGTH);
	while(*p)
	{
		memset(acTemp, 0x00, sizeof(char)*3);
		if((*p & 0x80) == 0x80)
		{
			memcpy(acTemp, p, sizeof(char)*2);
			p+=2;
			
			if(*p)
			{
				memset(acTemp2, 0x00, sizeof(char)*3);
				if((*p & 0x80) == 0x80)
				{
					memcpy(acTemp2, p, sizeof(char)*2);
					pNext = p+2;
				}
				else
				{
					memcpy(acTemp2, p, sizeof(char)*1);
					pNext = p+1;
				}
				if(s_text.getPixelSize(str)+s_text.getPixelSize(acTemp)+s_text.getPixelSize(acTemp2) > _iLimitSizeX)
				{
					strcat(str, "...");
					s_text.put( _iPutPosX, _iPutPosY, _color, "%s", str);
					return;
				}
			}
			strcat(str, acTemp);
			
		}
		else
		{
			memcpy(acTemp, p, sizeof(char)*1);
			p++;
			if(*p)
			{
				memset(acTemp2, 0x00, sizeof(char)*3);
				if((*p & 0x80) == 0x80)
				{
					memcpy(acTemp2, p, sizeof(char)*2);
					pNext = p+2;
				}
				else
				{
					memcpy(acTemp2, p, sizeof(char)*1);
					pNext = p+1;
				}
				if(s_text.getPixelSize(str)+s_text.getPixelSize(acTemp)+s_text.getPixelSize(acTemp2) > _iLimitSizeX)
				{
					strcat(str, "...");
					s_text.put( _iPutPosX, _iPutPosY, _color, "%s", str);
					return;
				}
			}
			strcat(str, acTemp);
		}	
	}	
	s_text.put( _iPutPosX, _iPutPosY, _color, "%s", _strText);	
}


