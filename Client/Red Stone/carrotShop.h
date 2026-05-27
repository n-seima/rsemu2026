#ifndef _classCarrotShop_H_
#define	_classCarrotShop_H_

#include "CGamePlay.H"
#include "carrotShopDefine.H"


#define CARROT_TCOLOR1	(RGB(223,175,66))
#define CARROT_TCOLOR2	(RGB(255,215,129))
#define CARROT_TCOLOR3	(RGB(255,126,209))
#define CARROT_TCOLOR4	(RGB(255,178,227))

#define CARROT_TOPCATE_COUNT	4
#define CARROT_WISHLIST_LIMITCOUNT	10
#define CARROT_SEARCHRESULT_SHOWCOUNT	5
	
class	cCarrotShopInfo	:	public cCarrotShopDefine
{
public:
	void			reset();

	void			load();
	void			save();

	int				getCategoryItemCount(int _iCategory);

	void			updateInfo(int _iCategory,cCarrotShopCategoryInfo1 *_lpData,cCarrotShopCategoryInfo2 *_lpCategoryInfo);
};

class	cCarrotShop	:	public CGamePlay
{
public:
	// ┌─────────────────────────────┐
	// │                      화면 출력 좌표                      │
	// └─────────────────────────────┘
	int				iPutPosX;			// insu add .. 화면에 찍을 x좌표 
	int				iPutPosY;			// insu add .. 화면에 찍을 y좌표
	int				m_iWindowWidth;		// insu add .. 당근상점 윈도우 사이즈
	int				m_iWindowHeight;	// insu add .. 당근상점 윈도우 사이즈


	// ┌─────────────────────────────┐
	// │                     최상위 카테고리                      │
	// └─────────────────────────────┘
	cRECT			m_aTopCategoryPos[CARROT_TOPCATE_COUNT];	// insu add .. 최상위카테고리 위치정보 - 카테고리는 4개로 고정
	int				m_iTopCategoryCnt;		// insu add .. 최상위카테고리 갯수 
	int				m_iFocusTopCategory;	// insu add .. 최상위카테고리 포커스 정보
	int				m_iSelectTopCategory;	// insu add .. 선택된 최상위카테고리
	
	int				GetFocusTopCategory();	// insu add .. 최상위 카테고리의 어느 버튼에 마우스가 위치하고 있는지 찾아낸다.
	void			DrawTopCategory();		// insu add .. 최상위 카테고리를 출력

	// ┌─────────────────────────────┐
	// │                     두번째 카테고리                      │
	// └─────────────────────────────┘
	cRECT			m_aSecCategoryPos[dMAX_CARROT_SHOP_CATEGORY_COUNT];	// insu add .. 두번째카테고리 위치정보 - 두번째 카테고리는 일단 20개로 고정되어 있음
	int				m_iFocusSecCategory;								// insu add .. 두번째 카테고리 포커스 정보
	int				m_iSelectSecCategory;								// insu add .. 선택된 두번째 카테고리
	CImageScrollBar	m_sbScrollBar;										// insu add .. 스크롤 바 
	cRECT			m_aBuyBtnPos[dMAX_CARROT_SHOP_ITEM_COUNT];			// insu add .. 구입 버튼 위치정보
	cRECT			m_aPresentBtnPos[dMAX_CARROT_SHOP_ITEM_COUNT];		// insu add .. 선물 버튼 위치정보 
	cRECT			m_aWishBtnPos[dMAX_CARROT_SHOP_ITEM_COUNT];			// insu add .. 위시리스트 버튼 위치정보
	cRECT			m_aToolTipPos[dMAX_CARROT_SHOP_ITEM_COUNT];			// insu add .. 아이템 툴팁을 띄우기 위한 조건 위치정보
	cRECT			m_aItemBoxPos[dMAX_CARROT_SHOP_ITEM_COUNT];			// 아이템 박스의 위치정보
	int				GetItemBoxIndex(int _iItemCount);					// 아이템 박스의 포커스정보를 얻어옴
	int				m_iFocusItemBoxIndex;								// 포커스가 간 아이템박스 인덱스

	int				GetFocusSecCategory();						// insu add .. 두번째 카테고리의 어느 버튼에 마우스가 위치하고 있는지 찾아낸다.
	void			DrawItemBuyCategory();						// insu add .. (최상위에서)아이템구입 카테고리 클릭시 출력
	void			DrawItemList();								// insu add .. 아이템 리스트 출력
	void			DrawItem(cItem* _Item, int _iCategoryIndex, int _ItemListIndex, int _iPutPosX, int _iPutPosY, bool _bHot, cRECT* _BuyBtnPos, cRECT* _PresentBtnPos, cRECT* _WishBtnPos, cRECT* _ToolTipPos, cRECT* _ItemBoxPos);// insu add .. 하나의 아이템 출력 
	void			DrawAllItem();								// insu add .. 전체 카테고리 클릭시 출력되는 내용
	int				GetClickBuyBntIndex(int _iItemCount);		// insu add .. 리스트내 어떤 구입버튼을 눌렀는지 찾아낸다.
	int				GetClickPresentBntIndex(int _iItemCount);	// insu add .. 리스트내 어떤 선물버튼을 눌렀는지 찾아낸다.
	int				GetClickWishBntIndex(int _iItemCount);		// insu add .. 리스트내 어떤 위시리스트버튼을 눌렀는지 찾아낸다.
	int				GetToolTipIndex(int _iItemCount);			// insu add .. 툴팁위치정보에 마우스 포인트가 있는지 찾아낸다.
	bool			DrawToolTip(int _iPutPosX, int _iPutPosY, int _iItemIndex);	// insu add .. 툴팁을 그려요.
	void			DrawCategoryBtn(cRECT* _Pos, int _iBtnIndex, char* _strBtnName, int _iPutPosX, int _iPutPosY, int _iSelectBtnImgIndex, int _iOverBtnImgIndex, int _iNormalBtnImgIndex);
																// insu add .. 카테고리 버튼 이미지 출력
	void			ScrollBarSet();								// insu add .. 아이템갯수에 따라 스크롤바를 셋팅한다.
	void			DrawResultToolTip();						// insu add .. 검색결과가 있을때 툴팁을 상황에 따라 보여준다.

	// ┌─────────────────────────────┐
	// │                        위시 리스트                       │
	// └─────────────────────────────┘
	int				m_iWishListCount;		// insu add .. 위시리스트가 채워져 양
	int				m_iTotalPayCarrot;		// insu add .. 결제 필요 당근 개수
	cRECT			m_WishBuyBtnPos;		// insu add .. 위시리스트내 구입버튼
	cRECT			m_WishPresentBtnPos;	// insu add .. 위시리스트내 선물버튼
	cRECT			m_WishDeleteBtnPos;		// insu add .. 위시리스트내 모두지우기버튼
	int				m_aiListItemCarrot[CARROT_WISHLIST_LIMITCOUNT];	// insu add .. 위시리스트내 아이템 해당 당근수
	cRECT			m_WishListItemPos[CARROT_WISHLIST_LIMITCOUNT];	// insu add .. 위시리스트내 아이템목록좌표
	cItem			m_aWishListItem[CARROT_WISHLIST_LIMITCOUNT];	// insu add .. 위시리스트에 저장된 아이템정보

	void			DrawWishList();							// insu add .. 위시 리스트 출력
	void			DeleteWishList(int _iListIndex);		// insu add .. 위시리스트 삭제
	bool			WishBuyBtnEvent();						// insu add .. 위시리스트 구입
	
	// ┌─────────────────────────────┐
	// │                          서치 바                         │
	// └─────────────────────────────┘
	int				m_iReturnState;					// insu add .. 리턴키를 눌렀을때 상황
	cTALK			m_inputSearch;					// insu add .. 에디트 박스
	BOOL			m_binputState;
	CImageScrollBar	m_sbSearchScrollBar;			// insu add .. 스크롤 바
	int				m_iSearchItemIndexCount;		// insu add .. 현재 서치 바 결과의 몇번째를 선택했는지 저장해둠
	int				m_iSearchCount;					// insu add .. 서치 바에서 나온 결과물의 수 
	bool			m_bSelectSearchBoxItem;			// insu add .. 현재 서치 바의 클릭유무를 확인한다.
	bool			m_bShowResultState;				// insu add .. 지금 서치결과를 찍어줄 타이밍인지 확인
	int				m_iSelectSearchBoxItem;			// insu add .. 서치 결과 중 클릭한 아이템
	cRECT			m_SelectSearchResultBox[CARROT_SEARCHRESULT_SHOWCOUNT];		// insu add .. 서치바 결과로 한 페이지에 찍을 갯수만큼의 위치정보 - 뭘 선택했는지 구분하기 위해 필요 
	cRECT			m_rtEditBoxArea;				// insu add .. 에디트박스 영역
	cRECT			m_rtSearchResultArea;			// insu add .. 서치결과화면의 영역, 카테고리버튼과 마우스가 겹치지 않도록 하기위해..
	cItem			m_aSearchItem[dMAX_CARROT_SHOP_ITEM_COUNT/2];		// insu add .. 서치한 아이템을 보관.. (굳이 이게 필요할까?생각해보자)
	int				m_aiSearchCateIndex[dMAX_CARROT_SHOP_ITEM_COUNT/2];	// insu add .. 아이템 정보를 뽑아올 아이템의 카테고리인덱스
	int				m_aiSearchItemIndex[dMAX_CARROT_SHOP_ITEM_COUNT/2];	// insu add .. 아이템 정보를 뽑아올 아이템의 인덱스
	char			m_strSearchValue[dITEM_NAME_LENGTH];
	char			m_strPreSearchValue[dITEM_NAME_LENGTH];
	char			m_strRecommendItemName[dITEM_NAME_LENGTH];		// 추천아이템?! 

	void			DrawSearchResult();				// insu add .. 서치바 결과를 출력한
	void			SearchAutoComplete();			// insu add .. 서치바 결과를 찾기 위한 연산
	void			ClearSearch(bool _bShowResultState, bool _bSearchCount, bool _bSelectSearchBoxItem=false, bool _bClearText=false);					// insu add .. 서치바 초기화
	
	// ┌─────────────────────────────┐
	// │                          기   타                         │
	// └─────────────────────────────┘
	int				m_iNewItemCount;			// 신규 카테고리 클릭시 검색되는 아이템수
	int				m_iTotalItemCount;			// 전체 카테고리 클릭시 아이템수
	cRECT			CloseBtnPos;				// 닫기버튼 위치정보
	

	void			DrawMainWndDivideLine();	// 선을 그어 보아요~
	bool			BuyBtnEvent(cCarrotShopCategoryInfoDefine* _lpCategory, int _iCateIndex, int _iItemIndex);		// 구입 버튼 이벤트
	void			DrawCarrotText(int _iPutPosX, int _iPutPosY, int _color, char* _strText, int _iLimitSizeX);	// 텍스트 출력

	void			draw_Original();

	void			openNew();
	void			updateNew();
	void			updateItemTooltip(int _iSelectSecCategory);
	void			updateBuyBtnEvent(int _iSelectSecCategory);
	void			updatePresentBtnEvent(int _iSelectSecCategory);
	void			updateWishListBtnEvent(int _iSelectSecCategory);
	void			updateSearchBox();

	void			openOld();
	int				getAllCategoryItemCount(BOOL _bNew=FALSE);
	void			InitScrollBar();

	// ----- existing code
	cRECT			m_aCategoryInfo[dMAX_CARROT_SHOP_CATEGORY_COUNT];
	int				m_iSelectItem;	//	구입을 위해 선택한 아이템
	int				m_iFocusItem,m_iFocusCategory;	//	포커스가 간 아이템
	cRECT			m_rect;
	int				m_iBuyItem,m_iBuyCategory;
	int				m_iScrollBarPos,m_iCurrentCategory,m_iCategoryLineCount;
	WORD			m_wIsWaitResult;
	int				m_iRemainCarrotCount;

	void			open();
	void			close();

	void			update();
	void			draw();
	void			drawCategory();

	int				getFocusCategory();
	int				getFocusItem();
	void			buyItem(cItem *_lpItem,int _iRemainCarrotCount);

	BOOL			isOpened();

	void			Str_toupper(char* _str);
	int				Str_len(char* _str);
};

extern	cCarrotShop		g_gwCarrotShop;
extern	cCarrotShopInfo	g_carrotShop;

#endif