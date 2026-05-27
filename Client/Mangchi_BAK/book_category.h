#ifndef	_classBookCategory_h
#define	_classBookCategory_h

#include "book_page.H"

class	cBookCategory : public cBookCategoryDefine
{
public:
					cBookCategory();

	void			reset();
	BOOL			addPage(char *_lpstrText,int _iPageImage,BOOL _bIsDefault,int _iValue,BOOL _bIsBlockToTrade,BOOL _bIsBlockToDestroy,BOOL _bIsBlockToStoreBank,int _iPage=-1);
	BOOL			modifyPage(int _iIndex,char *_lpstrText,int _iPageImage,BOOL _bIsDefault,int _iValue,BOOL _bIsBlockToTrade,BOOL _bIsBlockToDestroy,BOOL _bIsBlockToStoreBank);
	void			copy(cBookCategory *_lpBook);
	BOOL			removePage(int _iIndex);
	BOOL			changePageCount(int _iPageCount);

	inline	cBookPageInfo*	getPage(int _iPage)
	{
		if	(_iPage	<	0	||	_iPage	>=	m_iPageCount)
			return	NULL;

		return	(cBookPageInfo*)&m_aPage[_iPage];
	}

};

#endif