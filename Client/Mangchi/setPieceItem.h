#ifndef _classSetPieceItem_h
#define	_classSetPieceItem_h

#include "cSET.H"
#include "pieceItem.H"
#include <deque>

class	cMakeItemPiece:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	cItemPieceDefine		s_makeItemPiece;

	static	BOOL					Open(HWND _hWnd,cItemPieceDefine *_lpEdit);	//	시작
	static	void					InitDlg(HWND _hWnd);	//	시작

	static	BOOL					FinishWork(HWND _hDlg);

	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
};

class	cMakePieceItem	:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	cPieceItem				s_makePieceItem;
	static	cDIBWND					s_dibItemInfo;
	static	cLISTCONTROL			s_listItemPiece;
	static	int						s_iSelectItemPiece;


	static	BOOL					Open(HWND _hWnd,cPieceItem	*_lpEdit);	//	시작
	static	void					InitDlg(HWND _hWnd);	//	시작

	static	void					DrawItemPieceList(HWND _hWnd);
	static	void					DrawItemInfo(HWND _hWnd);

	static	void					ResetItemPieceListBox(HWND hwnd);

	static	void					AddItemPiece(HWND _hDlg);
	static	void					RemoveItemPiece(HWND _hDlg);
	static	void					EditItemPiece(HWND _hDlg);

	static	void					ChangeSetItem(HWND _hDlg,int _iItem);
	static	void					ChangePieceCount(HWND hDlg,int _iCount);
	static	void					ChangeItemSet(HWND _hDlg,int _iGroup);
	static	BOOL					FinishWork(HWND _hDlg);

	static	LRESULT CALLBACK		ProcItemPieceListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcItemInfoWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
};

class	cSetPieceItem	:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	cLISTCONTROL			s_listItemPiece;
	static	cDIBWND					s_dibPieceItemList,s_dibItemInfo;
	static	cSCROLL_BAR				s_sbPieceItemList;
	static	BOOL					s_bIsLostVirgin;
	static	cPieceItem				s_makePieceItem;
	static	int						s_iSelectPieceItem,s_iSelectItemPiece;
	static	std::deque<int>			s_deqFilterIndex;				// 09.08.17
	static	BOOL					Open(HWND _hWnd);	//	시작
	static	void					InitDlg(HWND _hWnd);	//	시작

	static	void					Draw(HWND _hDlg);
	static	void					DrawPieceItemList(HWND _hWnd);
	static	void					DrawItemPieceList(HWND _hWnd);
	static	void					DrawItemInfo(HWND _hWnd);

	static	void					ResetItemPieceListBox(HWND hwnd);
	static	int						SelectPieceItem(int _iX,int _iY);

	static	void					AddPieceItem(HWND _hDlg);
	static	void					RemovePieceItem(HWND _hDlg);
	static	void					EditPieceItem(HWND _hDlg);
	static	void					FilterPieceItem(HWND _hDlg);		// 09.08.17
	
	static	LRESULT CALLBACK		ProcPieceItemListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcItemPieceListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcItemInfoWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);

	static	BOOL					DecodeTextScriptFile(char *_lpstrFn);
	static	int						DecodeTextScript(char *_lpstrText);
	static	void					LoadingTextScript(HWND _hDlg);
};

#endif