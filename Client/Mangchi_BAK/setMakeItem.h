#ifndef _classSetMakeItem_h
#define	_classSetMakeItem_h

#include "cSET.H"
#include "cITEM.H"

class	cSetMakeItem	:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	cITEM					s_itemEdit;	//	편집중인 상점

	static	int						s_iPrefix1Kind,s_iPrefix2Kind,s_iPrefix3Kind,s_iItemKind;

	static	cITEM*					Open(HWND _hWnd,cITEM *_lpEditItem=NULL);	//	시작
	static	void					InitBox(HWND _hDlg);
	static	void					FillPrefixCombo(HWND _hDlg,int _iIndex);
	static	void					FillItemCombo(HWND _hDlg);
	static	void					FillItemData(HWND _hDlg);

	static	void					SelchangePrefix(HWND _hDlg,int _iPrefix,int _iPrefixIndex);
	static	void					SelchangeItem(HWND _hDlg,int _iItem);
	static	BOOL					GetItemInfo(HWND _hDlg,cITEM *_lpItem);

	static	void					PlaceItemToViewAble(HWND _hWnd,int _iItem);

	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
};

#endif