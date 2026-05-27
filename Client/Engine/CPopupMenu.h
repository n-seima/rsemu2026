#ifndef _classPOPUP_MENU_H_
#define	_classPOPUP_MENU_H_

#include "SFC.H"
#include "CText.H"
#include "cIMAGE.H"
#include "CTooltip.h"

#define	dDEFAULT_POPUP_MENU_TEXT_LENGTH		40
#define	dDEFAULT_POPUP_MENU_COUNT			10
#define	dDEFAULT_POPUP_MENU_LINE_SPACE		4

//
//	팝업 정보
class	CPopupMenuInfo
{
public:
	DWORD			m_dwTextColor,m_dwActiveTextColor;
	WORD			m_wIsActiveTextPutShadow,m_wIsTextPutShadow;
	WORD			m_wBarColor,m_wPannelColor;
	WORD			m_wIsHalfblendingBar,m_wIsHalfblendingPannel;
	int				m_iCountOfMaxMenu,m_iTextLength;
	WORD			m_wLineSpace;
	CTooltip		*m_lpttFrame;	//	툴팁 프레임만 쓴다.
	CTextDC			*m_lpText;

					CPopupMenuInfo()
					{
						m_wBarColor					=	_WHITE;
						m_wPannelColor				=	0;

						m_wIsHalfblendingBar		=	FALSE;
						m_wIsHalfblendingPannel		=	TRUE;

						m_dwActiveTextColor			=	0;
						m_dwTextColor				=	WHITE;

						m_wIsActiveTextPutShadow	=	FALSE;
						m_wIsTextPutShadow			=	TRUE;

						m_iTextLength				=	dDEFAULT_POPUP_MENU_TEXT_LENGTH;
						m_iCountOfMaxMenu			=	dDEFAULT_POPUP_MENU_COUNT;
						m_wLineSpace				=	dDEFAULT_POPUP_MENU_LINE_SPACE;
						m_lpttFrame					=	NULL;
						m_lpText					=	NULL;
					}
};	//	class	CPopupMenuInfo

class	CPopupMenu	:	public CPopupMenuInfo
{
	class	cMENU_ITEM
	{
	public:
		int				m_iMenuId;
		DWORD			m_dwTextColor;
		char			*m_pString;
		cRECT			m_rectSelect;

						cMENU_ITEM()
						{
							m_pString	=	NULL;
							reset();
						}

						~cMENU_ITEM()
						{
							pKILL(m_pString);
						}

		inline	void	reset()
		{
			m_iMenuId	=	-1;
		}
	};

	BOOL			m_isPopuped;
	
	cMENU_ITEM		*m_pMenu;

	cRECT			m_rectFrame;
	int				m_iCount,m_iTextFrameWidth,m_iTextFrameHeight;
	int				m_iFocusMenu;	//	포커스가 간 메뉴

public:

					CPopupMenu()	
					{
						m_pMenu				=	NULL;
						reset();
					}

					~CPopupMenu()	
					{
						close();
					}

	inline	void	reset()
	{
		m_isPopuped			=	FALSE;
		m_iCount			=	0;
		m_iTextFrameWidth	=	0;
		m_iTextFrameHeight	=	0;
	}

	inline	int		getCount()
	{
		return	m_iCount;
	}
	int				update(int _iX,int _iY,BOOL _bIsClickedLeftButton,BOOL _bIsLeftButtonPressed,BOOL _bIsClickedRightButton);	//	업데이트

	void			close()
	{
		pKILL(m_pMenu);
	}

	inline	BOOL	isPopuped()
	{
		return	m_isPopuped;
	}

	BOOL			init(CPopupMenuInfo *_lpInfo);	//	초기화

	BOOL			popup(int _iX,int _iY);	//	팝업 
	BOOL			addMenu(int _iMenu,char *_lpstrText);	//	메뉴 추가

	void			draw();			//	그려주기
	void			drawPannel();	//	판넬 그려주기
};


#endif