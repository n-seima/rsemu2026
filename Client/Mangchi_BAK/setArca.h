#ifndef _classSetArca_h
#define	_classSetArca_h

#include "cSET.H"
#include "cITEM.H"
#include "arca.H"

class	cMakeArca	:	public cSET
{
public:
	static	cArcaInfo				s_makeArca;
	static	HWND					s_hWnd;
	static	cLISTCONTROL			s_listDropItemList;	//	드롭 아이템 리스트
	static	cLISTCONTROL			s_listAddItemList;	//	추가 가능 아이템 리스트
	static	int						s_iSelectItemSet,s_iSelectPieceItem;

	static	BOOL					Open(HWND _hWnd,cArcaInfo *_lpEdit);	//	시작
	static	void					InitData(HWND _hDlg);
	static	void					UpdateAddItemList(HWND hDlg);
	static	void					Draw(HWND hDlg);

	static	void					ChangeItemSet(HWND hDlg,int _iGroup);
	static	void					ChangePieceItem(HWND hDlg,int _iPieceItem);

	static	BOOL					FinishWork(HWND hDlg);

	static	void					FillArcaInfo(HWND hDlg,BOOL _bIsClear=FALSE);
	static	void					RemoveGenItem(HWND hDlg);
	static	void					AddItemKindToGenItem(HWND hDlg);
	static	void					AddCustomItemKindToGenItem(HWND hDlg);
	static	void					AddSetItemToGenItem(HWND hDlg);
	static	void					AddItemPieceToGenItem(HWND hDlg);
	static	void					UpdateGenItemList(HWND hDlg,int _iSelect);


	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);

	static	LRESULT CALLBACK		ProcArcaList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK		ProcDropItemList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
};

class	cSetArca	:	public cSET
{
public:
	static	cArcaInfo				s_makeArca,s_clipArca;
	static	HWND					s_hWnd;
	static	cLISTCONTROL			s_listArca;	//	드롭 아이템 리스트
	static	cLISTCONTROL			s_listDropItemList;	//	드롭 아이템 리스트

	static	BOOL					Open(HWND _hWnd);	//	시작
	static	void					InitData(HWND _hDlg);
	static	void					UpdateAddItemList(HWND hDlg);
	static	void					Draw(HWND hDlg);

	static	void					AddArca(HWND hDlg);
	static	void					EditArca(HWND hDlg);
	static	void					UpdateArca(HWND hDlg);
	static	void					PasteArca(HWND hDlg);
	static	void					RemoveArca(HWND hDlg);
	static	void					CopyArca(HWND hDlg);
	static	void					FilterArca(HWND _hDlg);		//09.08.17
	static	void					ChangeItemSet(HWND hDlg,int _iGroup);
	static	void					ChangePieceItem(HWND hDlg,int _iPieceItem);

	static	void					GetArcaInfo(HWND hDlg,cArcaInfo *_lpArca);

	static	BOOL					DecodeTextScriptFile(char *_lpstrFn);
	static	BOOL					DecodeTextScript(char *_lpstrText);
	static	void					Loading(HWND hDlg);
	static	void					FillArcaInfo(HWND hDlg,BOOL _bIsClear=FALSE);
	static	void					UpdateGenItemList(HWND hDlg,int _iSelect);
	static	void					UpdateArcaList(HWND hDlg,int _iSelect);


	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);

	static	LRESULT CALLBACK		ProcArcaList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK		ProcDropItemList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
};

#endif