#ifndef _classTOOLTIP_H
#define _classTOOLTIP_H

#include "cIMAGE.H"
#include "cTEXT.H"

#define	dDEFAULT_LIMIT_WIDTH			250
#define	dDEFAULT_TOOLTIP_TEXT_LENGTH	2096
#define	dDEFAULT_TOOLTIP_HORZ_SPACE		2
#define	dDEFAULT_TOOLTIP_VERT_SPACE		2

//
//	Tooltip Fit Method
enum
{
	eTFM_NONE,		//	정렬 안함
	eTFM_LEFT,		//	박스의 오른쪽을 좌표의 왼쪽에 맞춰서 찍어줌
	eTFM_RIGHT,		//	박스의 왼쪽을 좌표의 오른쪽에 맞춰서 찍어줌
	eTFM_BOTTOM,	//	박스의 위쪽을 좌표의 아래쪽에 맞춰서 찍어줌
	eTFM_CENTER,	//	중앙에 찍기
	eTFM_AUTO,		//	자동으로 보이는 위치 찾기
};

//
//	툴팁 정보
class	CTooltipInfo
{
public:
	DWORD			m_dwTextColor;
	WORD			m_wIsTextPutShadow;
	WORD			m_wPannelColor;
	WORD			m_wIsHalfblendingPannel;
	WORD			m_wHorzSpace,m_wVertSpace;	//	가로 여분,세로 여분
	int				m_iLimitWidth;

	int				m_iLeftTop,m_iRightTop,m_iLeftBottom,m_iRightBottom;
	int				m_iTopBorder,m_iBottomBorder,m_iLeftBorder,m_iRightBorder;
	int				m_iTextLength;
	WORD			m_wFitMethod;	//	중악에 찍기?

	CTextDC			*m_lpText;
	cSPRITE			*m_lpSprite;

					CTooltipInfo()
					{
						m_iLeftTop					=	-1;
						m_iRightTop					=	-1;
						m_iLeftBottom				=	-1;
						m_iRightBottom				=	-1;
						m_iTopBorder				=	-1;
						m_iBottomBorder				=	-1;
						m_iLeftBorder				=	-1;
						m_iRightBorder				=	-1;

						m_wHorzSpace				=	dDEFAULT_TOOLTIP_HORZ_SPACE;
						m_wVertSpace				=	dDEFAULT_TOOLTIP_VERT_SPACE;	//	가로 여분,세로 여분
						m_iLimitWidth				=	dDEFAULT_LIMIT_WIDTH;

						m_wPannelColor				=	0;
						m_dwTextColor				=	WHITE;
						m_wIsTextPutShadow			=	TRUE;
						m_wIsHalfblendingPannel		=	TRUE;

						m_lpText					=	NULL;
						m_lpSprite					=	NULL;
						m_iTextLength				=	dDEFAULT_TOOLTIP_TEXT_LENGTH;
						m_wFitMethod				=	eTFM_NONE;
					}
};

class	CTooltip	:	public CTooltipInfo
{
public:
	DWORD		m_dwPopupTime,m_dwUpkeepTime;
	cRECT		m_rectActiveArea,m_rectTextFrame,m_rectFrame;	//	툴팁 체크 영역,툴팁 프레임
	WORD		*m_lpImage;
	char		*m_pString;	//	내용
	int			m_iWidth,m_iHeight,m_iTextFrameWidth,m_iTextFrameHeight;
	BOOL		m_isPopuped,m_isEnable;
	int			m_iLeftEdgeWidth,m_iRightEdgeWidth,m_iTopEdgeHeight,m_iBottomEdgeHeight,m_iTopBorderHeight,m_iBottomBorderHeight,m_iLeftBorderWidth,m_iRightBorderWidth;

				CTooltip();
				~CTooltip();

	int			getBordersWidth(){return m_iLeftEdgeWidth+m_iRightEdgeWidth;}
	int			getBordersHeight(){return m_iTopEdgeHeight+m_iBottomEdgeHeight;}

	void		reset();	//	리쉣
	BOOL		init(CTooltipInfo *_lpInfo);	//	초기화
	void		popupTip(cRECT *_lpRect,int _iX,int _iY,CTextDC *_lpText,DWORD _dwTextColor,char *_strString,int _iFitMethod=eTFM_NONE,DWORD _dwTime=0xffffffff);	//	띄우기
	void		setImage(WORD *_lpImage);

	BOOL		isFitCenter()	{if (m_wFitMethod	==	eTFM_CENTER	)	return	TRUE;return	FALSE;}
	BOOL		isFitLeft()		{if (m_wFitMethod	==	eTFM_LEFT	)	return	TRUE;return	FALSE;}
	BOOL		isFitBottom()	{if (m_wFitMethod	==	eTFM_BOTTOM	)	return	TRUE;return	FALSE;}
	BOOL		isFitAuto()		{if (m_wFitMethod	==	eTFM_AUTO	)	return	TRUE;return	FALSE;}
	BOOL		isFitRight()	{if (m_wFitMethod	==	eTFM_RIGHT	)	return	TRUE;return	FALSE;}

	void		hide();	//	숨기기
	void		update(int _iX,int _iY);	//	툴팁이 떠 있다면 영역을 검사하고, 벗어 났으면 제거 -o-
	void		put();	//	출력
	void		put(WORD _wPannelColor,BOOL _bIsHalfBlending=TRUE)	//	출력
	{
		WORD	wColor			=	m_wPannelColor;
		BOOL	bIsHalfblending	=	m_wIsHalfblendingPannel;

		m_wPannelColor			=	_wPannelColor;
		m_wIsHalfblendingPannel	=	_bIsHalfBlending;

		put();

		m_wPannelColor			=	wColor;
		m_wIsHalfblendingPannel	=	bIsHalfblending;
	}

	void		drawBox(cRECT *_lpRect,WORD _wBoxColor=0,int _iAlphaFactor=16,BOOL _isAddBorderSize=FALSE,BOOL _isAddSpareSize=FALSE);	//	이따금... 박스만 필요한 경우 박스만 그려준다.
	void		drawBoxAddBorderSize(cRECT *_lpRect,WORD _wBoxColor=0,int _iAlphaFactor=16);

	void		drawFrame(cRECT *_lpRect,BOOL _isAddBorderSize);	//	프레임 그려주기
	void		drawFrameAddBorderSize(cRECT *_lpRect);	//	둘래를 포함한 프레임 그리기
	void		drawTextBox(cRECT *_lpRect,DWORD _dwTextColor,WORD _wBoxColor,char *_lpstrText,BOOL _bIsPutShadow=TRUE,int _iAlpha=16);

	void		setEnable(BOOL _isEnable);	//	가능
	void		enable();	//	가능
	void		disable();	//	불가

	void		setShadowStatus(BOOL _bIsPut)	{m_wIsTextPutShadow	=	_bIsPut;}
};

#endif