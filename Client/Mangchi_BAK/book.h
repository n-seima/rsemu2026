#ifndef	_classBook_h
#define	_classBook_h

#include "book_info.H"

class	cBookManager
{
public:
	int				m_iBookCount;
	cBookInfo		m_aBook[c_iBookCount];

					cBookManager();
					~cBookManager();

	void			resetBookManager();
	BOOL			addBook(char *_lpstrBookName,BOOL _bIsBlockToDestroy,BOOL _bIsBlockToTrade,BOOL _bIsBlockToStore);
	BOOL			modifyBookData(int _iBookIndex,char *_lpstrBookName,BOOL _bIsBlockToDestroy,BOOL _bIsBlockToTrade,BOOL _bIsBlockToStore);
	BOOL			setBookItemComment(int _iBookIndex,char *_lpstrText);
	BOOL			removeBook(int _iBookIndex);

	int				getPageCount(int _iBookIndex,int _iCategory,int _iPage=0);
	cBookInfo*		getBook(int _iBookIndex);
	cBookCategory*	getCategory(int _iBookIndex,int _iCategoryIndex);
	cBookPageInfo*	getPage(int _iBookIndex,int _iCategoryIndex,int _iPageIndex);
};

extern	cBookManager	g_book;

#endif