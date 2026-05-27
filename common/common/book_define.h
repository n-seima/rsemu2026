#ifndef	_classBookDefine_h
#define	_classBookDefine_h

#include "SFC.H"

const	int	c_iMaxBookPageCount		=	8;
const	int	c_iMaxBookCategoryCount	=	18;
const	int	c_iBookCount			=	255;
const	int	c_iBookNameLength		=	62;
const	int	c_iBookCategoryNameLength=	82;
const	int	c_iMaxBookPageTextLength=	1024*4;
const	int	c_iMaxBookItemComment	=	1024;

class	cBookPageSaveDataDefine
{
public:
	WORD				m_wPageImage;
	WORD				m_wTextLength;
	WORD				m_wValue;
	WORD				m_bf1IsDefaultPage		:	1;
	WORD				m_bf1IsBlockTradePage	:	1;
	WORD				m_bf1IsBlockDestroyPage	:	1;
	WORD				m_bf1IsBlockStoreBank	:	1;
	DWORD				m_dwTextPos;
};

class	cBookPageInfoDefine	:	public	cBookPageSaveDataDefine
{
public:
	char				*m_pstrText;
	
	cBookPageInfoDefine()
	{
		m_pstrText	=	NULL;
	}
	~cBookPageInfoDefine()
	{
		pKILL(m_pstrText);
	}
};

class	cBookCategorySaveDataDefine
{
public:
	int					m_iPageCount;
	char				m_strCategoryName[c_iBookCategoryNameLength];
	WORD				m_wValue;
	WORD				m_wIsDefaultCategory;
};


class	cBookCategoryDefine	:	public cBookCategorySaveDataDefine
{
public:
	cBookPageInfoDefine	m_aPage[c_iMaxBookPageCount];
};

class	cBookSaveDataDefine
{
public:
	WORD				m_wCategoryCount;

	WORD				m_bf1IsBlockTrade		:	1;
	WORD				m_bf1IsBlockDestroy		:	1;
	WORD				m_bf1IsBlockStoreBank	:	1;

	char				m_strBookName[c_iBookNameLength];
	WORD				m_wTextLength;
	DWORD				m_dwTextPos;
};

class	cBookInfoDefine	:	public cBookSaveDataDefine
{
public:
	char				*m_pstrText;
	cBookCategoryDefine	m_aCategory[c_iMaxBookCategoryCount];

	cBookInfoDefine()
	{
		m_pstrText	=	NULL;
	}
	~cBookInfoDefine()
	{
		pKILL(m_pstrText);
	}
};

#endif