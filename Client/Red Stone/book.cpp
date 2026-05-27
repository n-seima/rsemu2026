#include "book.h"
#include "cItem.h"
#include "cProject.h"

cBookManager g_book;

char	cBookManager::s_strComment[1024*4];

char*
cBookManager::getBookName(int _iBook)
{
	return	m_aBook[_iBook].m_strBookName;
}

char*
cBookManager::getCategoryName(int _iBook,int _iCategory)
{
	return	m_aBook[_iBook].m_aCategory[_iCategory].m_strCategoryName;
}

cBookInfoDefine*
cBookManager::getBook(int _iBook)
{
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

int
cBookManager::getPageInBook(int _iBook,int _iCategory,int _iPage)
{
	int	iPageCount	=	0;

	cBookInfoDefine	*lpBook	=	&m_aBook[_iBook];

	for	(int i=0;i<_iCategory;i++)
		iPageCount	+=	lpBook->m_aCategory[i].m_iPageCount;

	return	iPageCount+_iPage;
}

int
cBookManager::getCategoryByPage(int _iBook,int _iPage)
{
	int	iPageCount	=	0;

	cBookInfoDefine	*lpBook	=	&m_aBook[_iBook];

	for	(int i=0;i<lpBook->m_wCategoryCount;i++)
	{
		iPageCount	+=	lpBook->m_aCategory[i].m_iPageCount;

		if	(iPageCount	>	_iPage)
			return	i;
	}

	return	0;
}

int
cBookManager::getPageByPage(int _iBook,int _iPage)
{
	int	iPageCount	=	0;

	cBookInfoDefine	*lpBook	=	&m_aBook[_iBook];

	for	(int i=0;i<lpBook->m_wCategoryCount;i++)
	{
		iPageCount	+=	lpBook->m_aCategory[i].m_iPageCount;

		if	(iPageCount	>	_iPage)
		{
			iPageCount	-=	lpBook->m_aCategory[i].m_iPageCount;

			return	_iPage-iPageCount;
		}
	}

	return	0;
}

int
cBookManager::getTotalPageCount(int _iBook)
{
	int	iPageCount	=	0;

	cBookInfoDefine	*lpBook	=	&m_aBook[_iBook];

	for	(int i=0;i<lpBook->m_wCategoryCount;i++)
		iPageCount	+=	lpBook->m_aCategory[i].m_iPageCount;

	return	iPageCount;
}

char*
cBookManager::getBookItemComment(int _iBook)
{
	SetLogFolder();
	
	cFILE	file;

	if	(!file.Open("data/scenario/red stone.rpd","rb"))
	{
		return	" ";
	}

	file.Seek(m_aBook[_iBook].m_dwTextPos);
	file.Read(s_strComment,m_aBook[_iBook].m_wTextLength);
	file.Close();

	return	s_strComment;
}

char*
cBookManager::getBookPageContents(int _iBook,int _iCategory,int _iPage)
{
	SetLogFolder();
	
	cFILE	file;

	if	(!file.Open("data/scenario/red stone.rpd","rb"))
	{
		return	" ";
	}

	cBookPageInfoDefine	*lpPage	=	&m_aBook[_iBook].m_aCategory[_iCategory].m_aPage[_iPage];

	file.Seek(lpPage->m_dwTextPos);
	file.Read(s_strComment,lpPage->m_wTextLength);
	file.Close();

	return	s_strComment;
}

void
cBookManager::makeVerticalCategoryName(char *_lpstrSrc,char *_lpstrDest)
{
	int		iPos					=	0;
	BOOL	bIsMeetCarraigeReturn	=	FALSE;

	for	(int	i=0;_lpstrSrc[i];i++)
	{
		if	(_lpstrSrc[i]	==	13	||	_lpstrSrc[i]	==	10)
		{
			bIsMeetCarraigeReturn	=	TRUE;
			continue;
		}

		if	(bIsMeetCarraigeReturn	&&	_lpstrSrc[i]	==	' ')
			continue;

		bIsMeetCarraigeReturn	=	FALSE;
		_lpstrDest[iPos++]		=	_lpstrSrc[i];
	}

	_lpstrDest[iPos]	=	NULL;
}
