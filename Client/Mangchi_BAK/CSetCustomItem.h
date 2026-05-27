#ifndef _classSetCustomItem_h
#define	_classSetCustomItem_h

#include "cSET.H"
#include "cITEM.H"
#include "setItem.H"
#include "pieceItem.H"
#include <deque>
class	CSetCustomItem	:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	CCustomItem				s_itemEdit;	//	편집중인 상점
	static	cSetItem				s_editSetItem;
	static	cLISTCONTROL			s_listRegistPrefix,s_listPrefix;	//	접두사 리스트

	static	int						s_iPrefixListWndPos;
	static	int						s_iSelectRegistPrefix,s_iSelectPrefix;
	static	int						s_iSelectPrefixType;
	static	BOOL					s_bIsMakeSetItem,s_bIsCustomItem;

//
	static	cDIBWND					s_dibItemInfo,s_dibBasicItemList;	//	선택한 상점의 정보
	static	int						s_iSelectItem;
	static	int						s_iSelectItemCategory;
	static	int						s_iItemListWidth,s_iItemListHeight,s_iLineCountPerPage,s_iLineCount;
	static	cSCROLL_BAR				s_sbItemList;
	static	cRECT					s_rectItemList;
	static	BOOL					s_bIsSortItemList;
	static	std::deque<int>			s_deqFilterIndex;				// 10.02.12
//
	static	CCustomItem*			Open(HWND _hWnd,CCustomItem *_lpEditItem=NULL);	//	시작
	static	cSetItem*				OpenForSetItem(HWND _hWnd,cSetItem *_lpEditItem=NULL);	//	시작
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
	static	void					DrawItemInfoWnd(HWND _hDlg);
	static	BOOL					FinishWork(HWND _hDlg,BOOL _bIsRequireMessage=TRUE);//	작업 마무리
	static	BOOL					GenerateItemByCustomItem(cITEM *_lpItem,CCustomItem *_lpCustomItem);
	static	BOOL					GenerateItemBySetItem(cITEM *_lpItem,cSetItem	*_lpSetItem);
	static	void					ChangeCustomItemSetting(HWND hwnd);
	static	void					FilterCustomItem(HWND _hDlg);		// 10.02.12


	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcBasicItemListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcItemInfoWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcRegistPrefixListEvent(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcPrefixListEvent(HWND,UINT,WPARAM,LPARAM);
};

class	CSetCustomItemList		:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	CCustomItem				s_itemEdit;	//	편집중인 상점
	static	cDIBWND					s_dibItemList,s_dibItemInfo;	//	선택한 상점의 정보

	static	int						s_iItemListWndPos;
	static	int						s_iSelectItem;
	static	CCustomItem*			s_lpSelectItem;
	static	CCustomItemManager*		s_lpItemManager;
	static	std::deque<int>			s_deqFilterIndex;				// 09.08.19
	static	int						s_iItemListWidth;
	static	int						s_iItemListHeight;
	static	int						s_iLineCountPerPage;
	static	int						s_iLineCount;
	static	cSCROLL_BAR				s_sbItemList,s_scrollItemInfo;
	static	cRECT					s_rectItemList;

	static	void					Open(HWND _hWnd,CCustomItemManager* _lpCustomItemManager);	//	시작
	static	void					InitBox(HWND _hDlg);

	static	void					ExportItemList(HWND _hDlg);
	static	BOOL					DecodeTextScript(char *_strFile);
	static	BOOL					DecodeTextScriptFile(char *_strFile);
	static	void					Loading(HWND _hDlg);
	static	void					AddItem(HWND _hDlg);//최초 분기 추가
	static	void					InsertItem(HWND _hDlg);
	static	void					EditItem(HWND _hDlg);//선택된 최초 분기 편집
	static	void					RemoveItem(HWND _hDlg);//선택된 최초 분기 제거
	static	void					DrawItemListWnd(HWND _hDlg);
	static	void					DrawItemInfoWnd(HWND _hDlg,cDIBWND *_lpDibWnd=NULL,CCustomItem *_lpCustomItem=NULL,cSetItem *_lpSetItem=NULL,cPieceItem *_lpPieceItem=NULL);
	
	static	int						SelectItem(int _iX,int _iY);
	static	BOOL					FinishWork(HWND _hDlg);
	static	void					FilterCustomItem(HWND _hDlg);	//09.08.17

	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcItemListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcItemInfoWnd(HWND,UINT,WPARAM,LPARAM);
};

#endif