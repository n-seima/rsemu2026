#include "book_page.h"

cBookPageInfo::cBookPageInfo()
{
	m_wPageImage	=	0;
	m_wTextLength	=	0;
	m_dwTextPos		=	0;

	m_pstrText		=	NULL;

	reset();
}

cBookPageInfo::~cBookPageInfo()
{
	reset();
}

void
cBookPageInfo::reset()
{
	m_wPageImage			=	0;
	m_wTextLength			=	0;
	m_dwTextPos				=	0;
	m_bf1IsDefaultPage		=	FALSE;
	m_bf1IsBlockTradePage	=	FALSE;
	m_bf1IsBlockDestroyPage	=	FALSE;

	pKILL(m_pstrText);
}

void
cBookPageInfo::setText(char *_lpstr)
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

void
cBookPageInfo::copy(cBookPageInfo *_lpPage)
{
	reset();

	memcpy(this,_lpPage,sizeof(cBookPageSaveDataDefine));

	setText(_lpPage->m_pstrText);
}

char*
cBookPageInfo::getContents(int _iSize)
{
	if	(m_pstrText	==	NULL)
		return	"error!!";

	if	(_iSize	==	0)
		return	m_pstrText;

	static	char	s_strText[256];

	ustrncpy(s_strText,m_pstrText,_iSize);

	return	s_strText;
}
