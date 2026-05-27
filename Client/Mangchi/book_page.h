#ifndef	_classBookPage_h
#define	_classBookPage_h

#include "book_define.H"

class	cBookPageInfo	:	public cBookPageInfoDefine
{
public:

					cBookPageInfo();
					~cBookPageInfo();

	void			reset();
	void			setText(char *_lpstr);
	void			copy(cBookPageInfo *_lpBook);
	char*			getContents(int _iSize=0);
};

#endif