#ifndef	_classBookGPW_h
#define	_classBookGPW_h

#include "CGamePlay.H"
#include "cItem.H"

enum
{
	eBW_INDEX,
	eBW_CONTENTS,
};


enum
{
	eBI_BOOK,
	eBI_COVER_FRAME,
	eBI_INDEX_FRAME,
	eBI_VALID_PAGE,
	eBI_CHAPTER_DECORATION,
	eBI_LEFT_PAGE_PANNEL,
	eBI_RIGHT_PAGE_PANNEL,

	eBI_INDEX_NORMAL,
	eBI_INDEX_LIGHT,
	eBI_INDEX_SHADOW,

	eBI_EXIT_NORMAL,
	eBI_EXIT_LIGHT,
	eBI_EXIT_SHADOW,
	eBI_PRE_PAGE_NORMAL,
	eBI_PRE_PAGE_LIGHT,
	eBI_PRE_PAGE_SHADOW,
	eBI_NEXT_PAGE_NORMAL,
	eBI_NEXT_PAGE_LIGHT,
	eBI_NEXT_PAGE_SHADOW,
	eBI_BOOK_NAME_DECORATION,
};

enum
{
	eBM_INDEX,
	eBM_EXIT,
	eBM_PREVIOUS_PAGE,
	eBM_NEXT_PAGE,
};

class	cBookGPW	:	public CGamePlay
{
public:
	BOOL			m_bIsInit;
	cItem			m_itemBook;
	int				m_iBook;
	int				m_iWhere;
	int				m_iCurrentCategory;
	int				m_iCurrentPage;
	int				m_iFocusIndex;
	cBARMENU		m_button;
	int				m_iRateX;
	int				m_iRateY;
	int				m_iAddX;

					cBookGPW();

	void			open(int _iItemSlot);
	void			close();

	void			update();
	int				getFocusIndex();
	void			draw();
	void			drawPage();
	void			drawIndex();

	BOOL			isOpened();
};

extern	cBookGPW	g_gwBook;

#endif
