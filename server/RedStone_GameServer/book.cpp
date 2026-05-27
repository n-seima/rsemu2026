#include "book.h"
#include "cItem.h"

cBookManager g_book;

void
cBookManager::writeBook(cBookItem	*_lpBookItem,int _iBook)
{
	cBookInfoDefine	*lpBook		=	&m_aBook[_iBook];

	memset(_lpBookItem->m_aCategoryInfo,0,sizeof(_lpBookItem->m_aCategoryInfo));
	
	_lpBookItem->m_wBookIndex	=	_iBook;
	
	for (int iCategory = 0;iCategory<lpBook->m_wCategoryCount;iCategory++)
	{
		cBookCategoryDefine	*lpCategory	=	&lpBook->m_aCategory[iCategory];

		for (int iPage =0 ;iPage<lpCategory->m_iPageCount;iPage++)
		{
			if	(lpCategory->m_aPage[iPage].m_bf1IsDefaultPage)
				_lpBookItem->m_aCategoryInfo[iCategory] |= (1<<iPage);
		}
	}
}

cBookInfoDefine*
cBookManager::getBook(int _iBook)
{
	if	(_iBook	<	0	||	_iBook	>	m_iBookCount)
		return	NULL;

	return	&m_aBook[_iBook];
}


cBookCategoryDefine*
cBookManager::getCategory(int _iBook,int _iCategory)
{
	cBookInfoDefine*	lpBook	=	getBook(_iBook);

	if	(!lpBook || _iCategory	>=	lpBook->m_wCategoryCount)
		return	NULL;

	return	&lpBook->m_aCategory[_iCategory];
}

cBookPageInfoDefine*
cBookManager::getPage(int _iBook,int _iCategory,int _iPage)
{
	cBookCategoryDefine*	lpCategory	=	getCategory(_iBook,_iCategory);

	if	(!lpCategory|| _iPage	>=	lpCategory->m_iPageCount)
		return	NULL;

	return	&lpCategory->m_aPage[_iPage];
}
