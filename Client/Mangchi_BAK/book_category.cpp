#include "book_category.h"

cBookCategory::cBookCategory()
{
	m_iPageCount	=	0;
}

void
cBookCategory::copy(cBookCategory *_lpCategory)
{
	reset();

	memcpy(this,_lpCategory,sizeof(cBookCategorySaveDataDefine));

	for(int i=0;i<m_iPageCount;i++)
		getPage(i)->copy(_lpCategory->getPage(i));
}

void
cBookCategory::reset()
{
	for(int i=0;i<m_iPageCount;i++)
		getPage(i)->reset();

	m_iPageCount		=	0;
	m_wIsDefaultCategory=	FALSE;
}

BOOL
cBookCategory::addPage(char *_lpstrText,int _iPageImage,BOOL _bIsDefault,int _iValue,BOOL _bIsBlockToTrade,BOOL _bIsBlockToDestroy,BOOL _bIsBlockToStoreBank,int _iPage)
{
	if	(m_iPageCount		>=	c_iMaxBookPageCount)
		return	FALSE;

	cBookPageInfo	*lpPage	=	NULL;

	if	(_iPage	==	-1)
	{
		_iPage	=	m_iPageCount;

		m_iPageCount++;
	}
	else
	{
		if	(_iPage	>=	m_iPageCount)
			_iPage	=	m_iPageCount-1;
		
		m_iPageCount++;

		for(int i=m_iPageCount-1;i>_iPage;i--)
			getPage(i)->copy(getPage(i-1));
	}

	lpPage	=	getPage(_iPage);

	if	(!lpPage)
	{
		m_iPageCount--;
		return	FALSE;
	}

	lpPage	=	(cBookPageInfo	*)&m_aPage[_iPage];

	lpPage->reset();
	lpPage->setText(_lpstrText);
	lpPage->m_wPageImage			=	_iPageImage;
	lpPage->m_bf1IsDefaultPage		=	_bIsDefault;
	lpPage->m_wValue				=	_iValue;
	lpPage->m_bf1IsBlockTradePage	=	_bIsBlockToTrade;
	lpPage->m_bf1IsBlockDestroyPage	=	_bIsBlockToDestroy;
	lpPage->m_bf1IsBlockStoreBank	=	_bIsBlockToStoreBank;

	return	TRUE;
}

BOOL
cBookCategory::modifyPage(int _iPageIndex,char *_lpstrText,int _iPageImage,BOOL _bIsDefault,int _iValue,BOOL _bIsBlockToTrade,BOOL _bIsBlockToDestroy,BOOL _bIsBlockToStoreBank)
{
	if	(_iPageIndex	>=	m_iPageCount)
		return	FALSE;

	cBookPageInfo	*lpPage	=	getPage(_iPageIndex);

	if	(!lpPage)
		return	FALSE;

	lpPage->setText(_lpstrText);
	lpPage->m_wPageImage			=	_iPageImage;
	lpPage->m_bf1IsDefaultPage		=	_bIsDefault;
	lpPage->m_wValue				=	_iValue;
	lpPage->m_bf1IsBlockTradePage	=	_bIsBlockToTrade;
	lpPage->m_bf1IsBlockDestroyPage	=	_bIsBlockToDestroy;
	lpPage->m_bf1IsBlockStoreBank	=	_bIsBlockToStoreBank;

	return	TRUE;
}

BOOL
cBookCategory::removePage(int _iIndex)
{
	if	(_iIndex	>=	m_iPageCount	||	_iIndex	<	0)
		return	FALSE;

	for	(int i=_iIndex;i<m_iPageCount-1;i++)
		getPage(i)->copy(getPage(i+1));

	getPage(m_iPageCount-1)->reset();

	m_iPageCount--;

	return	TRUE;
}

BOOL
cBookCategory::changePageCount(int _iPageCount)
{
	if	(_iPageCount	>	c_iMaxBookPageCount)
		return	FALSE;

	int	 i;
	
	for	(i=_iPageCount;i<m_iPageCount;i++)
		getPage(i)->reset();

	for	(i=m_iPageCount;i<_iPageCount;i++)
	{
		getPage(i)->setText("none");
		getPage(i)->m_wPageImage	=	0;
	}

	m_iPageCount	=	_iPageCount;

	return	TRUE;
}

