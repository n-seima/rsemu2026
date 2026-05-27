#ifndef _classSetShop_h
#define	_classSetShop_h

#include "cSET.H"
#include "CShop.H"
#include <deque>

class	CSetShopItem	:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	CShopItem				s_shopitemEdit;	//	편집중인 상점
	static	cLISTCONTROL			s_listRegistPrefix,s_listPrefix;	//	접두사 리스트

	static	int						s_iPrefixListWndPos;
	static	int						s_iSelectRegistPrefix,s_iSelectPrefix;
	static	int						s_iSelectPrefixType;

//
	static	cDIBWND					s_dibBasicItemList;	//	선택한 상점의 정보
	static	int						s_iSelectItem;
	static	int						s_iSelectItemCategory;
	static	int						s_iItemListWidth,s_iItemListHeight,s_iLineCountPerPage,s_iLineCount;
	static	cSCROLL_BAR				s_sbItemList;
	static	cRECT					s_rectItemList;
	static	BOOL					s_bIsSortItemList;
	static	std::deque<int>			s_deqFilterIndex;				// 10.02.12
//
	
	static	CShopItem*				Init(HWND hwnd,CShopItem *_lpEditItem=NULL);	//	시작
	static	void					InitBox(HWND _hDlg);
	static	void					ResetItemListBox(HWND hwnd,BOOL _bReset);
	static	void					ResetPrefixListBox(HWND hwnd);
	static	void					ResetRegistPrefixListBox(HWND hwnd);

	static	void					PlaceItemToViewAble(HWND _hWnd,int _iItem);


	static	int						SelectItem(int _iX,int _iY);

	static	void					AddPrefix(HWND _hDlg);//최초 분기 추가
	static	void					RemovePrefix(HWND _hDlg);//선택된 최초 분기 제거

	static	void					DrawBasicItemListWnd(HWND _hDlg);
	static	void					DrawRegistPrefixListWnd(HWND _hDlg);
	static	void					DrawPrefixListWnd(HWND _hDlg);
	static	BOOL					FinishWork(HWND _hDlg);//	작업 마무리
	static	void				FilterCustomItem(HWND _hDlg);			// 10.02.12

	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcBasicItemListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcRegistPrefixListEvent(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcPrefixListEvent(HWND,UINT,WPARAM,LPARAM);
};

class	CSetShop		:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	CShop					s_shopEdit;	//	편집중인 상점
	static	cDIBWND					s_dibItemList,s_dibItemInfo,s_dibTokkenItemInfo;	//	선택한 상점의 정보

	static	int						s_iItemListWndPos;
	static	int						s_iSelectItem;
	static	cBASIC_ITEM*			s_lpSelectItem;

	static	int						s_iItemListWidth;
	static	int						s_iItemListHeight;
	static	int						s_iLineCountPerPage;
	static	int						s_iLineCount;
	static	cSCROLL_BAR				s_sbItemList,s_scrollItemInfo;
	static	cRECT					s_rectItemList;
	static	BOOL					s_bIsSortItemList;


	static	CShop*					Init(HWND hwnd,CShop *_lpEditShop=NULL);	//	시작
	static	void					InitBox(HWND _hDlg);

	static	void					AddItem(HWND _hDlg);//최초 분기 추가
	static	void					EditItem(HWND _hDlg);//선택된 최초 분기 편집
	static	void					RemoveItem(HWND _hDlg);//선택된 최초 분기 제거
	static	void					UpItem(HWND _hDlg);//아이템 위로~ ^o^
	static	void					DownItem(HWND _hDlg);//아이템 아래로~ -o-
	static	void					DrawItemListWnd(HWND _hDlg);
	static	void					DrawItemInfoWnd(HWND _hDlg);
	
	static	int						SelectItem(int _iX,int _iY);
	static	BOOL					FinishWork(HWND _hDlg);


	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcItemListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcItemInfoWnd(HWND,UINT,WPARAM,LPARAM);
};

class	CSetShopList		:	public cSET
{
public:
	static	HWND					s_hWnd;

	static	CShopManager			*s_lpEditShoplist;	//	편집중인 상점
	static	cDIBWND					s_dibShopInfo;	//	선택한 상점의 정보
	static	cLISTCONTROL			s_listShop;	//	상점 리스트

	static	int						s_iSelectShop;
	static	cSCROLL_BAR				s_sbItemList;
	static	cRECT					s_rectItemList;
	static	int						s_iItemListWidth,s_iItemListHeight;		//	아이템 리스트 높이
	static	int						s_iLineCountPerPage,s_iLineCount;
	static	BOOL					s_isSort;


	static	CShop*					s_CopyShop;		// 복사중인 shop정보

	static	BOOL					CopyShop();		// insu add .. shop 복사 
	static	BOOL					PasteShop();		// insu add .. shop 붙여넣기 

	static	void					Init(HWND hwnd,CShopManager	*_lpShoplist);	//	시작
	static	void					InitBox(HWND _hDlg);
	static	void					ResetShopListBox(HWND hwnd);

	static	int						SelectShop(HWND hwnd);	//	컨텐츠 선택
	static	int						GetSelectShop(HWND hwnd);	//	선택된 컨텐츠 얻기

	static	void					AddShop(HWND _hDlg);//최초 분기 추가
	static	void					EditShop(HWND _hDlg);//선택된 최초 분기 편집
	static	void					RemoveShop(HWND _hDlg);//선택된 최초 분기 제거

	static	void					DrawShopListWnd(HWND _hDlg);
	static	void					DrawShopInfoWnd(HWND _hDlg);

	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcShopListEvent(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcShopInfoWnd(HWND,UINT,WPARAM,LPARAM);

};

#endif