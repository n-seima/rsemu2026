#ifndef	_classBookInfo_h
#define	_classBookInfo_h

#include "book_category.H"

class	cBookInfo	:	public cBookInfoDefine
{
public:
					cBookInfo();

	void			copy(cBookInfo *_lpBook);
	void			reset();
	BOOL			modifyCategoryName(int _iIndex,char *_lpstrText,BOOL _bIsDefault,int _iValue);
	BOOL			addCategory(char *_lpstrText,BOOL _bIsDefault,int _iValue);
	BOOL			removeCategory(int _iIndex);
	void			setItemComment(char *_lpstrText);

	inline	cBookCategory*	getCategory(int _iIndex)
	{
		if	(_iIndex	<	0	||	_iIndex	>=	m_wCategoryCount)
			return	NULL;
		
		return	(cBookCategory*)&m_aCategory[_iIndex];
	}
};

#endif