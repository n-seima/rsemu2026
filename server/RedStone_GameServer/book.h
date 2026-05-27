#ifndef	_classBook_h
#define	_classBook_h

#include "book_define.h"

class	cBookItem;

class	cBookManager
{
public:
	int					m_iBookCount;
	cBookInfoDefine		m_aBook[c_iBookCount];

	void					writeBook(cBookItem	*_lpBookItem,int _iBook);
	cBookInfoDefine*		getBook(int _iBook);
	cBookCategoryDefine*	getCategory(int _iBook,int _iCategory);
	cBookPageInfoDefine*	getPage(int _iBook,int _iCategory,int _iPage);
};

extern	cBookManager	g_book;

#endif