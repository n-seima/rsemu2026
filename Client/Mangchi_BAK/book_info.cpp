#include "book.h"

cBookInfo::cBookInfo()
{
	m_wCategoryCount	=	0;
	m_pstrText			=	NULL;
}

void
cBookInfo::copy(cBookInfo *_lpBook)
{
	reset();

	memcpy(this,_lpBook,sizeof(cBookSaveDataDefine));
	
	for	(int i=0;i<m_wCategoryCount;i++)
		getCategory(i)->copy(_lpBook->getCategory(i));
}

void
cBookInfo::reset()
{
	for(int i=0;i<m_wCategoryCount;i++)
		getCategory(i)->reset();

	m_wCategoryCount	=	0;
}

BOOL
cBookInfo::modifyCategoryName(int _iIndex,char *_lpstrText,BOOL _bIsDefault,int _iValue)
{
	if	(_iIndex	>=	m_wCategoryCount)
		return	FALSE;

	strncpy(getCategory(_iIndex)->m_strCategoryName,_lpstrText,c_iBookCategoryNameLength-1);
	getCategory(_iIndex)->m_wIsDefaultCategory	=	_bIsDefault;
	getCategory(_iIndex)->m_wValue				=	_iValue;

	return	TRUE;
}

BOOL
cBookInfo::addCategory(char *_lpstrText,BOOL _bIsDefault,int _iValue)
{
	if	(m_wCategoryCount	>=	c_iMaxBookCategoryCount)
		return	FALSE;

	m_wCategoryCount++;

	cBookCategory	*lpCategory	=	getCategory(m_wCategoryCount-1);

	if	(!lpCategory)
	{
		m_wCategoryCount--;
		return	FALSE;
	}

	lpCategory->reset();
	strncpy(lpCategory->m_strCategoryName,_lpstrText,c_iBookCategoryNameLength-1);
	lpCategory->m_wIsDefaultCategory	=	_bIsDefault;
	lpCategory->m_wValue				=	_iValue;

	return	TRUE;
}

BOOL
cBookInfo::removeCategory(int _iIndex)
{
	if	(_iIndex	>=	m_wCategoryCount	||	_iIndex	<	0)
		return	FALSE;

	for	(int i=_iIndex;i<m_wCategoryCount-1;i++)
		getCategory(i)->copy(getCategory(i+1));

	getCategory(m_wCategoryCount-1)->reset();

	m_wCategoryCount--;

	return	TRUE;
}


void
cBookInfo::setItemComment(char *_lpstr)
{	
	pKILL(m_pstrText);

	if	(_lpstr	== NULL)
	{
		m_wTextLength	=	0;
		return;
	}

	m_wTextLength	=	strlen(_lpstr)+1;
	m_pstrText		=	new char [m_wTextLength];
	strcpy(m_pstrText,_lpstr);
}