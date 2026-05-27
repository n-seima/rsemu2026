#ifndef _classIMAGE_SCROLL_BAR_
#define	_classIMAGE_SCROLL_BAR_

#include "SFC.H"
#include "cIMAGE.H"
#include "bar_menu.H"

#define	dIMAGE_SCROLL_BAR_VERTICAL	0
#define	dIMAGE_SCROLL_BAR_HORIZON	1

class CImageScrollBar
{
public:
	int			m_iUpdateTime;
	cSPRITE		*m_lpSprite;
	cBarMenu	m_button;
	int			m_iNormalUpButton,m_iActiveUpButton,m_iPressedUpButton,m_iDisabledUpButton;
	int			m_iNormalDownButton,m_iActiveDownButton,m_iPressedDownButton,m_iDisabledDownButton;
	int			m_iNormalScrollBarTop,m_iNormalScrollBarBody,m_iNormalScrollBarBottom;
	int			m_iActiveScrollBarTop,m_iActiveScrollBarBody,m_iActiveScrollBarBottom;
	int			m_iUpButton,m_iDownButton;
	int			m_iScrollBarTop,m_iScrollBarBody,m_iScrollBarBottom;
	BOOL		m_isGrabbedBody,m_isLockButton,m_isPressedSpace;
	int			m_iGrabbedPos;
	cPOINT		m_posGrabbed;

	cPOINT		m_pos;
	int			m_iImageSize,m_iButtonWidth,m_iButtonHeight,m_iBodySize,m_iBodyCapSize;
	int			m_iPageSize,m_iSize,m_iCurrentPos;
	int			m_iHeadSpace;	//	Thumb를 쥐고 움직을때 앞 혹은 위쪽에 여백

	int			m_iType;

				CImageScrollBar();

	BOOL		init(cSPRITE *_lpSprite		,int _iType,	//	초기화
					int _iNormalUpButton	,int _iActiveUpButton		,int _iPressedUpButton	,int _iDisabledUpButton,
					int	_iNormalDownButton	,int _iActiveDownButton		,int _iPressedDownButton,int _iDisabledDownButton,
					int	_iNormalScrollBarTop,int _iNormalScrollBarBody	,int _iNormalScrollBarBottom,
					int	_iActiveScrollBarTop,int _iActiveScrollBarBody	,int _iActiveScrollBarBottom);
	BOOL		update(int _iX,int _iY,BYTE *_aKeyMap,BOOL _isLeftTouch,BOOL _isLeftPressed);
	void		draw();

	inline BOOL	isVisible()	//	페이지 사이즈보다 사이즈가 크면 안보여도 된다.
	{
		if (m_iPageSize >= m_iSize)	return	FALSE;

		return	TRUE;
	}

	void		setSize(int _iSize,int _iPageSize);
	void		set(int _iX,int _iY,int _iSize);
	void		setPos(int _iPos);
};

#endif