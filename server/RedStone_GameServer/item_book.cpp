#include "cItem.h"
#include "book.h"

void
cBookItem::resetBook()
{
	memset(m_aCategoryInfo,0,sizeof(m_aCategoryInfo));
}

void
cBookItem::writeCategory(int _iCategory)
{
	m_aCategoryInfo[_iCategory]	=	0xff;
}

void
cBookItem::writePage(int _iCategory,int _iPage)
{
	m_aCategoryInfo[_iCategory]	|=	(1<<_iPage);
}

void
cBookItem::erasePage(int _iCategory,int _iPage)
{
	int	iMask	=	(1<<_iPage);

	if	((m_aCategoryInfo[_iCategory]&iMask)== iMask)
		m_aCategoryInfo[_iCategory]	-=	iMask;
}

BOOL
cBookItem::isWrittenPage(int _iCategory,int _iPage)
{
	int	iPageCheckValue	=	1<<_iPage;

	if	(m_aCategoryInfo[_iCategory] & iPageCheckValue)
		return	TRUE;

	return	FALSE;
}

//
//	이미 쓰여져 있는 카테고리이다
BOOL
cBookItem::isWrittenCategory(int _iCategory,int _iPageCount)
{
	if	(_iPageCount	==	0)
	{
		cBookInfoDefine	*lpBook	=	g_book.getBook(m_wBookIndex);

		if	(!lpBook)
			return	FALSE;

		_iPageCount	=	lpBook->m_aCategory[_iCategory].m_iPageCount;
	}

	for	(int i=0;i<_iPageCount;i++)
	{
		if	(isWrittenPage(_iCategory,i)	==	FALSE)
			return	FALSE;
	}

	return	TRUE;
}

int
cBookItem::getCompletePercentage()
{
	cBookInfoDefine	*lpBook		=	g_book.getBook(m_wBookIndex);

	int			iPageCount		=	0;
	int			iTotalPageCount	=	0;

	for	(int i=0;i<lpBook->m_wCategoryCount;i++)
	{
		for	(int j=0;j<lpBook->m_aCategory[i].m_iPageCount;j++)
		{
			int	iMask	=	1<<j;

			if	((m_aCategoryInfo[i]&iMask) == iMask)
				iPageCount++;
		}

		iTotalPageCount	+=	lpBook->m_aCategory[i].m_iPageCount;
	}

	iPageCount		-=	lpBook->m_wCategoryCount;
	iTotalPageCount	-=	lpBook->m_wCategoryCount;
	iPageCount		=	max(iPageCount,0);
	iTotalPageCount	=	max(iTotalPageCount,1);

	return	iPageCount*100/iTotalPageCount;
}

