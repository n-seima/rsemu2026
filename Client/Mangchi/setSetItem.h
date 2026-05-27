#ifndef _classSetSetItem_h
#define	_classSetSetItem_h

#include "cSET.H"
#include "setItem.H"

class	cMakeItemSet	:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	cItemSet			s_makeItemSet;
	static	cDIBWND					s_dibSetEffectData,s_dibSetItemList,s_dibSetItemInfo;
	static	cSCROLL_BAR				s_sbSetItemEffectList;

	static	int						s_iSelectSetGroup,s_iSelectSetCount,s_iSelectSetEffect,s_iSelectSetItem;
	static	cRECT					s_rectSetItemList;
	static	int						s_iSetEffectDlgHeight;		//	아이템 리스트 너비

	static	BOOL					Open(HWND _hWnd,cItemSet *_lpEditGroup=NULL);	//	시작
	static	void					InitDlg(HWND _hWnd);	//	시작
	static	void					InitSetEffectListWnd(HWND _hWnd);
	static	void					ResetSetEffectList(HWND _hWnd);
	static	int						SelectSetItemEffect(int _iX,int _iY);
	static	int						SelectSetItem(int _iX,int _iY);

	static	void					Draw(HWND _hDlg);
	static	void					DrawSetEffect(HWND _hWnd);
	static	void					DrawSetItemListWnd(HWND _hWnd);
	static	void					DrawSelectSetItemInfoWnd(HWND _hWnd);

	static	BOOL					FinishWork(HWND _hDlg);

	static	LRESULT CALLBACK		ProcSetEffectDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcSetItemListDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcSetItemInfoWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
};

class	cSetItemSet	:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	cDIBWND					s_dibSetEffectData,s_dibSetItemList;
	static	cLISTCONTROL			s_listItemSet;
	static	cSCROLL_BAR				s_sbSetItemEffectList;
	static	int						s_iDecodeSetItemIndex;
	static	cItemSet				s_makeGroup;
	static	int						s_iSelectSetGroup,s_iSelectSetCount,s_iSelectSetEffect,s_iSelectSetItem;
	static	cRECT					s_rectSetItemList;

	static	int						s_iSetEffectDlgHeight;		//	아이템 리스트 너비

	static	BOOL					Open(HWND _hWnd);	//	시작
	static	void					InitDlg(HWND _hWnd);	//	시작
	static	void					InitSetEffectListWnd(HWND _hWnd);
	static	void					ResetSetEffectList(HWND _hWnd);

	static	int						SelectSetItemEffect(int _iX,int _iY);
	static	int						SelectSetItem(int _iX,int _iY);

	static	void					ResetItemSetListBox(HWND hwnd);

	static	void					Draw(HWND _hDlg);
	static	void					DrawSetEffect(HWND _hWnd);
	static	void					DrawSetItemListWnd(HWND _hWnd);
	static	void					DrawItemSetListWnd(HWND _hDlg);

	static	void					ChangeSetGroup(HWND _hDlg,int _iGroup);
	static	void					FilterSetItem(HWND _hDlg);		// 09.08.18
	static	BOOL					DecodeTextScriptFile(char *_lpstrFn);
	static	int						DecodeTextScript(char *_lpstrText);
	static	void					LoadingTextScript(HWND _hDlg);
	static	void					AddGroup(HWND _hDlg);
	static	void					EditGroup(HWND _hDlg);
	static	void					RemoveGroup(HWND _hDlg);
	
	static	LRESULT CALLBACK		ProcItemSetListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcSetEffectDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcSetItemListDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
};

#endif