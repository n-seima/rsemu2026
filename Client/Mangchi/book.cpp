#include "book.h"

cBookManager	g_book;

cBookManager::cBookManager()
{
	resetBookManager();
}

cBookManager::~cBookManager()
{
}

void
cBookManager::resetBookManager()
{
	m_iBookCount	=	0;

	for	(int i=0;i<c_iBookCount;i++)
		m_aBook[i].reset();
}

cBookInfo*
cBookManager::getBook(int _iBookIndex)
{
	if	(_iBookIndex	>=	m_iBookCount)
		return	NULL;

	return	&m_aBook[_iBookIndex];
}

int
cBookManager::getPageCount(int _iBookIndex,int _iCategory,int _iPage)
{
	cBookInfo	*lpBook	=	getBook(_iBookIndex);

	if	(!lpBook)
		return	0;

	int	iPageCount	=	0;

	for	(int i=0;i<_iCategory;i++)
		iPageCount	+=	lpBook->m_aCategory[i].m_iPageCount;

	return	iPageCount+_iPage;
}

cBookCategory*
cBookManager::getCategory(int _iBookIndex,int _iCategoryIndex)
{
	cBookInfo	*lpBook	=	getBook(_iBookIndex);

	if	(!lpBook)
		return	NULL;

	return	lpBook->getCategory(_iCategoryIndex);
}

cBookPageInfo*
cBookManager::getPage(int _iBookIndex,int _iCategoryIndex,int _iPageIndex)
{
	cBookCategory*	lpCategory	=	getCategory(_iBookIndex,_iCategoryIndex);

	if	(!lpCategory)
		return	NULL;

	return	lpCategory->getPage(_iPageIndex);
}

BOOL
cBookManager::addBook(char *_lpstrBookName,BOOL _bIsBlockToDestroy,BOOL _bIsBlockToTrade,BOOL _bIsBlockToStore)
{
	if	(m_iBookCount	>=	c_iBookCount)
		return	FALSE;

	m_aBook[m_iBookCount].reset();
	strncpy(m_aBook[m_iBookCount].m_strBookName,_lpstrBookName,c_iBookNameLength-2);
	m_aBook[m_iBookCount].m_bf1IsBlockTrade	=	_bIsBlockToTrade;
	m_aBook[m_iBookCount].m_bf1IsBlockDestroy=	_bIsBlockToDestroy;
	m_aBook[m_iBookCount].m_bf1IsBlockStoreBank=_bIsBlockToStore;

	m_iBookCount++;

	return	TRUE;
}

BOOL
cBookManager::modifyBookData(int _iBookIndex,char *_lpstrBookName,BOOL _bIsBlockToDestroy,BOOL _bIsBlockToTrade,BOOL _bIsBlockToStore)
{
	if	(_iBookIndex	>=	m_iBookCount)
		return	FALSE;

	strncpy(m_aBook[_iBookIndex].m_strBookName,_lpstrBookName,c_iBookNameLength-2);
	m_aBook[_iBookIndex].m_bf1IsBlockTrade	=	_bIsBlockToTrade;
	m_aBook[_iBookIndex].m_bf1IsBlockDestroy=	_bIsBlockToDestroy;
	m_aBook[_iBookIndex].m_bf1IsBlockStoreBank=	_bIsBlockToStore;

	return	TRUE;
}

BOOL
cBookManager::setBookItemComment(int _iBookIndex,char *_lpstrText)
{
	if	(_iBookIndex	>=	m_iBookCount)
		return	FALSE;

	m_aBook[_iBookIndex].setItemComment(_lpstrText);

	return	TRUE;
}

BOOL
cBookManager::removeBook(int _iIndex)
{
	if	(_iIndex	>=	m_iBookCount	||	_iIndex	<	0)
		return	FALSE;

	for	(int i=_iIndex;i<m_iBookCount-1;i++)
		m_aBook[i].copy(&m_aBook[i+1]);

	m_aBook[m_iBookCount-1].reset();

	m_iBookCount--;

	return	TRUE;
}