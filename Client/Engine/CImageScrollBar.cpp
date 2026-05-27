#include "CImageScrollBar.h"

#define	dBM_SCROLL_BAR_DECREASE_BUTTON	10000
#define	dBM_SCROLL_BAR_INCREASE_BUTTON	10001

CImageScrollBar::CImageScrollBar()
{
	m_iUpdateTime	=	0;
	m_lpSprite		=	NULL;
	m_isGrabbedBody	=	FALSE;
	m_isLockButton	=	FALSE;
}

//
//	초기화
BOOL
CImageScrollBar::init(cSPRITE *_lpSprite		,int _iType,
						int _iNormalUpButton	,int _iActiveUpButton		,int _iPressedUpButton	,int _iDisabledUpButton,
						int	_iNormalDownButton	,int _iActiveDownButton		,int _iPressedDownButton,int _iDisabledDownButton,
						int	_iNormalScrollBarTop,int _iNormalScrollBarBody	,int _iNormalScrollBarBottom,
						int	_iActiveScrollBarTop,int _iActiveScrollBarBody	,int _iActiveScrollBarBottom)
{
	m_lpSprite				=	_lpSprite;
	m_iType					=	_iType;

	m_iCurrentPos			=	0;

	m_iNormalUpButton		=	_iNormalUpButton;
	m_iActiveUpButton		=	_iActiveUpButton;
	m_iPressedUpButton		=	_iPressedUpButton;
	m_iDisabledUpButton		=	_iDisabledUpButton;
	m_iNormalDownButton		=	_iNormalDownButton;
	m_iActiveDownButton		=	_iActiveDownButton;
	m_iPressedDownButton	=	_iPressedDownButton;
	m_iDisabledDownButton	=	_iDisabledDownButton;
	m_iNormalScrollBarTop	=	_iNormalScrollBarTop;
	m_iNormalScrollBarBody	=	_iNormalScrollBarBody;
	m_iNormalScrollBarBottom=	_iNormalScrollBarBottom;
	m_iActiveScrollBarTop	=	_iActiveScrollBarTop;
	m_iActiveScrollBarBody	=	_iActiveScrollBarBody;
	m_iActiveScrollBarBottom=	_iActiveScrollBarBottom;

	m_iScrollBarTop			=	_iNormalScrollBarTop;
	m_iScrollBarBody		=	_iNormalScrollBarBody;
	m_iScrollBarBottom		=	_iNormalScrollBarBottom;

	m_iUpButton			=	m_iNormalUpButton;
	if (m_iUpButton		==	0xffff)	m_iUpButton		=	m_iActiveUpButton;
	if (m_iUpButton		==	0xffff)	m_iUpButton		=	m_iPressedUpButton;

	m_iDownButton		=	m_iNormalDownButton;
	if (m_iDownButton	==	0xffff)	m_iDownButton	=	m_iActiveDownButton;
	if (m_iDownButton	==	0xffff)	m_iDownButton	=	m_iPressedDownButton;

	WORD	*lpButton	=	_lpSprite->Get16(m_iUpButton);

	if (m_iScrollBarTop	!=	0xffff)
	{
		WORD	*lpCap	=	_lpSprite->Get16(m_iScrollBarTop);

		if (m_iType	==	dIMAGE_SCROLL_BAR_VERTICAL)	m_iBodyCapSize	=	lpCap[1];
		else										m_iBodyCapSize	=	lpCap[0];
	}
	else	m_iBodyCapSize	=	0;

	m_iButtonWidth	=	lpButton[0];
	m_iButtonHeight	=	lpButton[1];

//	스크롤바 버튼
	{
		m_button.init(m_lpSprite,2);

		m_button.addImageBar("",dBM_SCROLL_BAR_DECREASE_BUTTON,0,0,m_iNormalUpButton,m_iActiveUpButton,m_iPressedUpButton,0xffff,0xffff);

		if (m_iType	==	dIMAGE_SCROLL_BAR_VERTICAL)	
				m_button.addImageBar("",dBM_SCROLL_BAR_INCREASE_BUTTON,0,m_iButtonHeight+m_iBodyCapSize*2,m_iNormalDownButton,m_iActiveDownButton,m_iPressedDownButton,0xffff,0xffff);
		else	m_button.addImageBar("",dBM_SCROLL_BAR_INCREASE_BUTTON,m_iButtonWidth+m_iBodyCapSize*2,0,m_iNormalDownButton,m_iActiveDownButton,m_iPressedDownButton,0xffff,0xffff);
	}	//	스크롤바 버튼

	return	TRUE;
}

//	업뎃
BOOL
CImageScrollBar::update(int _iX,int _iY,BYTE *_aKeyMap,BOOL _isLeftTouch,BOOL _isLeftPressed)
{
	BOOL	isEvent	=	FALSE;

	if (m_iPageSize	>=	m_iSize)
	{
		m_iCurrentPos	=	0;

		return	FALSE;
	}

	DWORD	dwMenu	=	0xffffffff;

	if (!m_isGrabbedBody)
		dwMenu=	m_button.updatePressed(_iX,_iY,_aKeyMap,_isLeftPressed);

	switch(dwMenu)
	{
		case	dBM_SCROLL_BAR_DECREASE_BUTTON	:
			m_isLockButton	=	TRUE;

			if (m_iUpdateTime	==	0	&&	_isLeftTouch)
			{
				m_iUpdateTime	=	timeGetTime();
				setPos(m_iCurrentPos-1);
				break;
			}

			if (m_iUpdateTime	!=	0)
				if (timeGetTime()	>=	(DWORD)m_iUpdateTime+500)
					setPos(m_iCurrentPos-1);
			break;

		case	dBM_SCROLL_BAR_INCREASE_BUTTON	:
			m_isLockButton	=	TRUE;

			if (m_iUpdateTime	==	0	&&	_isLeftTouch)
			{
				m_iUpdateTime	=	timeGetTime();
				setPos(m_iCurrentPos+1);
				break;
			}

			if (m_iUpdateTime	!=	0)
				if (timeGetTime()	>=	(DWORD)m_iUpdateTime+500)
					setPos(m_iCurrentPos+1);
			break;

		default	:
			m_iUpdateTime	=	0;
			break;
	}

	if (!_isLeftPressed			)	m_isLockButton	=	FALSE;
	if (m_isLockButton			)	return	TRUE;

	int		iBodySize	=	min(m_iBodySize,m_iBodySize*m_iPageSize/m_iSize);
	LONGLONG	llPos	=	m_iCurrentPos;
	llPos				*=	(m_iBodySize-iBodySize);
	llPos				/=	(m_iSize-m_iPageSize);
	int		iBodyPos	=	(int)llPos;

	cRECT	rectBody,rectBodyBar;

	if (m_iType	==	dIMAGE_SCROLL_BAR_VERTICAL)	
	{
		rectBody.set(m_pos.x,m_pos.y+m_iButtonHeight,m_pos.x+m_iButtonWidth,m_pos.y+m_iButtonHeight+m_iBodySize+m_iBodyCapSize*2);
		rectBodyBar.set(m_pos.x,m_pos.y+m_iButtonHeight+iBodyPos,m_pos.x+m_iButtonWidth,m_pos.y+m_iButtonHeight+iBodyPos+iBodySize+m_iBodyCapSize*2);
	}
	else
	{	
		rectBody.set(m_pos.x+m_iButtonWidth,m_pos.y,m_pos.x+m_iButtonWidth+m_iBodySize+m_iBodyCapSize*2,m_pos.y+m_iButtonHeight);
		rectBodyBar.set(m_pos.x+m_iButtonWidth+iBodyPos,m_pos.y,m_pos.x+m_iButtonWidth+iBodyPos+iBodySize+m_iBodyCapSize*2,m_pos.y+m_iButtonHeight);
	}

	if (!_isLeftPressed)
	{
		m_isGrabbedBody	=	FALSE;
		m_isPressedSpace=	FALSE;
	}

	if (rectBodyBar.isIn(_iX,_iY))
	{
		if (m_iActiveScrollBarTop	!=	0xffff)	m_iScrollBarTop		=	m_iActiveScrollBarTop;
		if (m_iActiveScrollBarBody	!=	0xffff)	m_iScrollBarBody	=	m_iActiveScrollBarBody;
		if (m_iActiveScrollBarBottom!=	0xffff)	m_iScrollBarBottom	=	m_iActiveScrollBarBottom;

		if (_isLeftPressed)
		{
			if (!m_isGrabbedBody)
			{
				m_isGrabbedBody		=	TRUE;
				m_posGrabbed.x		=	_iX;
				m_posGrabbed.y		=	_iY;
				m_iGrabbedPos		=	m_iCurrentPos;

				if (m_iType	==	dIMAGE_SCROLL_BAR_VERTICAL)	
						m_iHeadSpace	=	m_posGrabbed.y-(m_pos.y+m_iButtonHeight)-(m_posGrabbed.y-(m_pos.y+m_iButtonHeight+iBodyPos));
				else	m_iHeadSpace	=	m_posGrabbed.x-(m_pos.x+m_iButtonWidth)-(m_posGrabbed.x-(m_pos.x+m_iButtonWidth+iBodyPos));
			}
		}
	}
	else
	if (!m_isGrabbedBody)	//	스크롤바 말고..
	{
		m_iScrollBarTop		=	m_iNormalScrollBarTop;
		m_iScrollBarBody	=	m_iNormalScrollBarBody;
		m_iScrollBarBottom	=	m_iNormalScrollBarBottom;

		if (_isLeftTouch	&&	rectBody.isIn(_iX,_iY))
		{
			if (m_iType	==	dIMAGE_SCROLL_BAR_VERTICAL)	
			{
				if (_iY	<	m_pos.y+m_iButtonHeight+iBodyPos)	setPos(m_iCurrentPos-m_iPageSize+1);
				else											setPos(m_iCurrentPos+m_iPageSize-1);
			}
			else
			{
				if (_iX	<	m_pos.x+m_iButtonWidth+iBodyPos)	setPos(m_iCurrentPos-m_iPageSize+1);
				else											setPos(m_iCurrentPos+m_iPageSize-1);
			}

			m_isPressedSpace=	TRUE;
		}

		isEvent	=	m_isPressedSpace;
	}

	if (m_isGrabbedBody)
	{
		if (m_iType	==	dIMAGE_SCROLL_BAR_VERTICAL)	
		{
			int		iMoveSize	=	_iY-m_posGrabbed.y;

			if (iMoveSize	<	0)
			{
				if (m_iHeadSpace	<=	0)	iMoveSize	=	m_iGrabbedPos;
				else
				{
					LONGLONG	llPos	=	m_iGrabbedPos;

					iMoveSize	=	-iMoveSize;
					iMoveSize	=	min(iMoveSize,m_iHeadSpace);
					llPos		=	iMoveSize*llPos/m_iHeadSpace;
					iMoveSize	=	(int)llPos;
				}

				setPos(m_iGrabbedPos-iMoveSize);
			}
			else
			{
				int	iSpace	=	m_iBodySize-(iBodySize+m_iHeadSpace);

				if (iSpace	<=	0)	setPos(m_iSize);
				else
				{
					LONGLONG	llPos	=	m_iSize;
					iMoveSize	=	min(iMoveSize,iSpace);
					llPos		=	iMoveSize*(llPos-m_iGrabbedPos-m_iPageSize)/iSpace;
					iMoveSize	=	(int)llPos;

					setPos(m_iGrabbedPos+iMoveSize);
				}
			}
		}

		if (m_iType	==	dIMAGE_SCROLL_BAR_HORIZON)	
		{
			int	iMoveSize	=	_iX-m_posGrabbed.x;

			if (iMoveSize	<	0)
			{
				if (m_iHeadSpace	<=	0)	iMoveSize	=	m_iGrabbedPos;
				else
				{
					LONGLONG	llPos	=	m_iGrabbedPos;

					iMoveSize	=	-iMoveSize;
					iMoveSize	=	min(iMoveSize,m_iHeadSpace);
					llPos		=	iMoveSize*llPos/m_iHeadSpace;
					iMoveSize	=	(int)llPos;
				}

				setPos(m_iGrabbedPos-iMoveSize);
			}
			else
			{
				if (iMoveSize	>	0)
					iMoveSize	=	iMoveSize;

				int	iSpace	=	m_iBodySize-(iBodySize+m_iHeadSpace);

				if (iSpace	<=	0)	setPos(m_iSize);
				else
				{
					LONGLONG	llPos	=	m_iSize;
					iMoveSize	=	min(iMoveSize,iSpace);
					llPos		=	iMoveSize*(llPos-m_iGrabbedPos-m_iPageSize)/iSpace;		
					iMoveSize	=	(int)llPos;

					setPos(m_iGrabbedPos+iMoveSize);
				}
			}
		}
		
		isEvent	=	TRUE;
	}

	return	isEvent;
}	//	CImageScrollBar::update(int _iX,int _iY,BOOL _isPressedButton)

//
//	그려주기
void
CImageScrollBar::draw()
{
	if (m_iType	==	dIMAGE_SCROLL_BAR_VERTICAL)	
	{
		WORD	*lpBody	=	m_lpSprite->Get16(m_iScrollBarBody);
		int		iWidth	=	lpBody[0];
		int		iXP		=	(short)lpBody[2];

		cDRAW::FillHB(RGB24To16(RGB(168,122,51)),m_pos.x-iXP,m_pos.y+m_iButtonHeight,m_pos.x-iXP+iWidth-1,m_pos.y+m_iButtonHeight+m_iBodySize+m_iBodyCapSize*2);
	}
	else
	{	
		WORD	*lpBody	=	m_lpSprite->Get16(m_iScrollBarBody);
		int		iHeight	=	lpBody[1];
		int		iYP		=	(short)lpBody[3];

		cDRAW::FillHB(RGB24To16(RGB(168,122,51)),m_pos.x+m_iButtonWidth,m_pos.y-iYP,m_pos.x+m_iButtonWidth+m_iBodySize+m_iBodyCapSize*2,m_pos.y-iYP+iHeight-1);
	}

	m_button.draw();	//	버튼 그리기

	if (m_iPageSize	>=	m_iSize)	return;

	int	iBodySize	=	min(m_iBodySize,m_iBodySize*m_iPageSize/m_iSize);

	LONGLONG	iPos=	m_iCurrentPos;

	iPos	=	(m_iBodySize-iBodySize)*iPos/(m_iSize-m_iPageSize);

	int	iBodyPos	=	(int)iPos;

//	스크롤바 몸통 그리기
	{
		if (m_iScrollBarTop	!=	0xffff)
		{
			if (m_iType	==	dIMAGE_SCROLL_BAR_VERTICAL)		//	캡
			{
				m_lpSprite->Put(m_pos.x,m_pos.y+m_iButtonHeight+iBodyPos,m_iScrollBarTop);
				m_lpSprite->Put(m_pos.x,m_pos.y+m_iButtonHeight+m_iBodyCapSize+iBodyPos+iBodySize,m_iScrollBarBottom);
			}
			else
			{
				m_lpSprite->Put(m_pos.x+m_iButtonWidth+iBodyPos,m_pos.y,m_iScrollBarTop);
				m_lpSprite->Put(m_pos.x+m_iButtonWidth+m_iBodyCapSize+iBodyPos+iBodySize,m_pos.y,m_iScrollBarBottom);
			}
		}

		if (m_iType	==	dIMAGE_SCROLL_BAR_VERTICAL)	
				m_lpSprite->clipPut(m_pos.x,m_pos.y+iBodyPos+m_iButtonHeight+m_iBodyCapSize,m_iScrollBarBody,0xffff,iBodySize);
		else	m_lpSprite->clipPut(m_pos.x+iBodyPos+m_iButtonWidth+m_iBodyCapSize,m_pos.y,m_iScrollBarBody,iBodySize,0xffff);
	}
}	//	CImageScrollBar::draw()

//
//	이런저런 설정
void
CImageScrollBar::setSize(int _iSize,int _iPageSize)
{
	m_iSize			=	_iSize;
	m_iPageSize		=	_iPageSize;

	if (m_iCurrentPos>	m_iSize-m_iPageSize	)	m_iCurrentPos	=	m_iSize-m_iPageSize;
	if (m_iSize		<=	m_iPageSize			)	m_iCurrentPos	=	0;
}	//	CImageScrollBar::set(int _iX,int _iY,int _iSize,int _iPageSize)

//
//	이미지 위치 관련 설정
void
CImageScrollBar::set(int _iX,int _iY,int _iSize)
{
	m_pos.x			=	_iX;
	m_pos.y			=	_iY;
	m_iImageSize	=	_iSize;

	if (m_iType	==	dIMAGE_SCROLL_BAR_VERTICAL)	m_iBodySize		=	m_iImageSize-m_iButtonHeight*2-m_iBodyCapSize*2-1;
	else										m_iBodySize		=	m_iImageSize-m_iButtonWidth*2-m_iBodyCapSize*2-1;

	m_button.setCorrectPos(dBM_SCROLL_BAR_DECREASE_BUTTON,m_pos.x,m_pos.y);

	if (m_iType	==	dIMAGE_SCROLL_BAR_VERTICAL)	m_button.setCorrectPos(dBM_SCROLL_BAR_INCREASE_BUTTON,m_pos.x,m_pos.y+m_iBodySize);
	else										m_button.setCorrectPos(dBM_SCROLL_BAR_INCREASE_BUTTON,m_pos.x+m_iBodySize,m_pos.y);
}	//	CImageScrollBar::set(int _iX,int _iY,int _iSize)

//
//	위치 설정
void
CImageScrollBar::setPos(int _iPos)
{
	if (_iPos	>	m_iSize-m_iPageSize	)	_iPos	=	m_iSize-m_iPageSize;
	if (_iPos	<	0					)	_iPos	=	0;

	m_iCurrentPos	=	_iPos;
}	//CImageScrollBar::setPos(int _iPos)