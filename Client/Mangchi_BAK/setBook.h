#ifndef _classSetBook_h
#define	_classSetBook_h

#include "cSet.H"
#include "book.H"

class	cSetBook	:	public cSET
{
public:
	static	cBookInfo				s_editBook;
	static	cBookCategory			s_editCategory;
	static	cBookPageInfo			s_editPage;
	static	int						s_iSelectBook,s_iSelectCategory,s_iSelectPage;
	static	HFONT					s_hFont;

	static	HWND					s_hWnd;
	static	cLISTCONTROL			s_listBook;		//	책 리스트
	static	cLISTCONTROL			s_listCategory;	//	카테고리 리스트
	static	cLISTCONTROL			s_listPage;		//	페이지 리스트

	static	BOOL					Open(HWND _hWnd);	//	시작
	static	void					InitData(HWND _hDlg);
	static	void					Draw(HWND hDlg);

	static	void					AddBook(HWND hDlg);
	static	void					ApplyItemComment(HWND hDlg);
	static	void					ModifyBookName(HWND hDlg);
	static	void					RemoveBook(HWND hDlg);
	static	void					ChangeBook(HWND hDlg,int _iBook);
	static	void					UpdateBookList(HWND hDlg,int _iSelect);

	static	void					AddCategory(HWND hDlg);
	static	void					ModifyCategoryName(HWND hDlg);
	static	void					RemoveCategory(HWND hDlg);
	static	void					ChangeCategory(HWND hDlg,int _iBook);
	static	void					UpdateCategoryList(HWND hDlg,int _iSelect);

	static	void					AddPage(HWND hDlg,BOOL _bIsInsert=FALSE);
	static	void					ModifyPage(HWND hDlg);
	static	void					RemovePage(HWND hDlg);
	static	void					ChangePage(HWND hDlg,int _iPage);
	static	void					UpdatePageList(HWND hDlg,int _iSelect);

	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcBookList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK		ProcBookCategoryList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK		ProcBookPageList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
};


#endif