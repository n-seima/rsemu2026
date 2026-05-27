#ifndef	_classBook_h
#define	_classBook_h

#include "SFC.h"
#include "book_define.h"

class	cBookItem;

class	cBookManager
{
public:
	static	char		s_strComment[1024*4];
	int					m_iBookCount;
	cBookInfoDefine		m_aBook[c_iBookCount];

	char*				getBookName(int _iBook);
	char*				getCategoryName(int _iBook,int _iCategory);
	char*				getBookItemComment(int _iBook);
	char*				getBookPageContents(int _iBook,int _iCategory,int _iPage);
	int					getTotalPageCount(int _iBook);
	int					getPageInBook(int _iBook,int _iCategory,int _iPage);
	int					getCategoryByPage(int _iBook,int _iPage);
	int					getPageByPage(int _iBook,int _iPage);
	void				makeVerticalCategoryName(char *_lpstrSrc,char *_lpstrDest);


	cBookInfoDefine*	getBook(int _iBook);
};

extern	cBookManager	g_book;

#endif