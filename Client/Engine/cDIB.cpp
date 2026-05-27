#include "CDib.H"
#include <commctrl.h>
#include "cDRAW.H"
#include "cPROFILE.H"
#include "cTALKBOX.h"

CDib::CDib()
{	m_hDibWnd		=	NULL;
	m_hBM			=	NULL;
	m_bIsActive	=	FALSE;

	m_hDC			=	0;
}

CDib::~CDib()
{	close();
}

void
CDib::destroy()
{
	close();
}

CDib::CDib(HWND hwnd)
{	m_hBM		=	NULL;
	init(hwnd);
}

BOOL
CDib::init(HWND hwnd,int _iMinWidth,int _iMinHeight)
{	
	g_iPixelFormat	=	dRGB555;

	close();

	if (m_hBM)
	{	
		DeleteObject(m_hBM);
		m_hBM	=	NULL;
	}

	m_hDibWnd	=	hwnd;

	if (!IsWindow(m_hDibWnd)) return FALSE;

	BITMAPINFO bmInfo;

	m_rectView.clientFrame(m_hDibWnd);

	m_iWidth	=	m_rectView.x2;
	m_iHeight	=	m_rectView.y2;

	if	(_iMinWidth)
		m_iWidth	=	max(_iMinWidth,m_iWidth);
	if	(_iMinHeight)
		m_iHeight	=	max(_iMinHeight,m_iHeight);

	if (m_iWidth%4!=0)
		m_iWidth	=	m_iWidth/4*4+4;

	m_hDC		=	GetDC(m_hDibWnd);

	bmInfo.bmiHeader.biSize				= sizeof (BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth			= m_iWidth;
	bmInfo.bmiHeader.biHeight			= -m_iHeight;
	bmInfo.bmiHeader.biPlanes			= 1;
	bmInfo.bmiHeader.biBitCount			= 16;
	bmInfo.bmiHeader.biCompression		= BI_RGB;
	bmInfo.bmiHeader.biSizeImage		= 0;
	bmInfo.bmiHeader.biXPelsPerMeter	= 0;
	bmInfo.bmiHeader.biYPelsPerMeter	= 0;
	bmInfo.bmiHeader.biClrUsed			= 0;
	bmInfo.bmiHeader.biClrImportant		= 0;

	m_hBM = CreateDIBSection (m_hDC , (BITMAPINFO *)&bmInfo, DIB_RGB_COLORS, (VOID **)&m_p2SCREEN, NULL, 0);

	ReleaseDC(m_hDibWnd , m_hDC );

	RGBmix		=	RGB555mix;

	return TRUE;
}

void
CDib::fadeIn(WORD _wDestColor,int _iSpeed)
{
	m_iFadeScreenValue	=	0;
	m_iFadeScreenStatus	=	dFADE_IN;
	m_wFadeColor		=	_wDestColor;
	m_iFadeScreenSpeed	=	_iSpeed;
	m_bIsCompleteFadeOut=	FALSE;
}

void
CDib::setScreenFilter(WORD	_wDestColor,int _iDepth)
{
	cancelFadeScreen();

	m_wFadeColor		=	_wDestColor;
	m_iFadeScreenValue	=	32*_iDepth/100;
}

void
CDib::fadeOut(WORD _wDestColor,int _iSpeed)
{
	m_iFadeScreenValue	=	32;
	m_iFadeScreenStatus	=	dFADE_OUT;
	m_wFadeColor		=	_wDestColor;
	m_iFadeScreenSpeed	=	_iSpeed;
	m_bIsCompleteFadeIn	=	FALSE;
}

void
CDib::cancelFadeScreen()
{
	m_iFadeScreenStatus	=	dFADE_NONE;
	m_bIsCompleteFadeIn	=	FALSE;
}

BOOL
CDib::isCompleteFadeOut()
{
	if	(m_bIsCompleteFadeOut)
	{
		m_bIsCompleteFadeOut	=	FALSE;

		return	TRUE;
	}

	return	FALSE;
}

BOOL
CDib::isCompleteFadeIn()
{
	if	(m_bIsCompleteFadeIn)
	{
		m_bIsCompleteFadeIn	=	FALSE;

		return	TRUE;
	}

	return	FALSE;
}

//
//	페이드 인/아웃을 위한거.. -o-
void
CDib::operateFadeScreen()
{
	{
		if	(m_iFadeScreenValue		== 32	)
		{
			if	(m_iFadeScreenStatus	==	dFADE_IN)
				m_bIsCompleteFadeIn		=	TRUE;

			return;
		}

		if  (m_iFadeScreenValue		== 0	)
		{
			if	(m_iFadeScreenStatus	==	dFADE_OUT)
				m_bIsCompleteFadeOut	=	TRUE;

			memset(m_p2SCREEN,m_wFadeColor,m_iWidth*m_iHeight*2);

			return;
		}
	}

	UINT	*lpScreen		=	(UINT *)m_p2SCREEN;
	
	CRY(m_iFadeScreenValue	<0	,"Error in CDib::operateFadeScreen - m_iFadeScreenValue value's too low");
	CRY(m_iFadeScreenValue	>32	,"Error in CDib::operateFadeScreen - m_iFadeScreenValue value's too high");

	UINT	dwFirst,dwSecond;
	UINT	dwDestColor		=	((m_wFadeColor<<16)+m_wFadeColor);
	int		iDestAlpha		=	32-m_iFadeScreenValue;
	int		iAddress		=	0;

	dwFirst		=	dwDestColor&d4AM_MASK_555_1;
	dwFirst		>>=	5;
	dwFirst		*=	iDestAlpha;
	dwFirst		&=	d4AM_MASK_555_1;

	dwSecond	=	dwDestColor&d4AM_MASK_555_2;
	dwSecond	*=	iDestAlpha;
	dwSecond	>>=	5;
	dwSecond	&=	d4AM_MASK_555_2;
	dwDestColor	=	dwFirst + dwSecond;

	for (int y=0;y<m_iHeight;y++,iAddress+=m_iWidth/2)
		for (int i=0;i<m_iWidth/2;i++)
		{
			dwFirst		=	lpScreen[iAddress+i]&d4AM_MASK_555_1;
			dwFirst		>>=	5;
			dwFirst		*=	m_iFadeScreenValue;
			dwFirst		&=	d4AM_MASK_555_1;

			dwSecond	=	lpScreen[iAddress+i]&d4AM_MASK_555_2;
			dwSecond	*=	m_iFadeScreenValue;
			dwSecond	>>=	5;
			dwSecond	&=	d4AM_MASK_555_2;

			lpScreen[iAddress+i]	=	(dwFirst + dwSecond) + dwDestColor;
		}
}

BOOL
CDib::flip()
{
	if	(!IsWindow(m_hDibWnd)	)
		return FALSE;
	if	(!m_bIsActive			)
		return FALSE;

	if	(m_iFadeScreenStatus)
		operateFadeScreen();

	RECT		rectDest;
	PAINTSTRUCT ps;

	BeginPaint(m_hDibWnd, &ps);
	GetClientRect(m_hDibWnd, &rectDest);

	int nindex;
	HDC	hdc	=	GetDC(m_hDibWnd);

	
	HRGN hCombineRgn, hExCombineRgn,hRgn;
	hCombineRgn = CreateRectRgn ( 0,0,0,0 ) ;

	for ( nindex=0; nindex<g_rectEditControlList.size(); nindex ++ )
	{
		cEditBoxClippingInfo	info;

		info	=	g_rectEditControlList[nindex];
		if (info.m_bIsEnable	==	FALSE)
			continue;
		hRgn = CreateRectRgn ( info.m_rect.x1, info.m_rect.y1, info.m_rect.x2, info.m_rect.y2 ) ;
		CombineRgn ( hCombineRgn, hCombineRgn, hRgn, RGN_OR);
		DeleteObject ( hRgn );
	}

	hExCombineRgn = CreateRectRgn ( 0,0,0,0 ) ;
	for ( nindex=0; nindex<g_EditControlExcludeList.size(); nindex++ )
	{
		cRECT rect;
		rect= g_EditControlExcludeList[nindex];
		hRgn = CreateRectRgn ( rect.x1, rect.y1, rect.x2, rect.y2 ) ;
		CombineRgn ( hExCombineRgn, hExCombineRgn, hRgn, RGN_OR);
		DeleteObject ( hRgn );
	}

	CombineRgn ( hCombineRgn, hCombineRgn, hExCombineRgn, RGN_DIFF );
	ExtSelectClipRgn ( hdc, hCombineRgn, RGN_DIFF );
	DeleteObject ( hExCombineRgn );


	if (m_isShakeScreen	==	FALSE)
		goto pass_label;

	if (m_iShakeX == 0)
	{	
		if (m_iShakeY < 0)
			BitBlt(hdc, 0,m_iHeight+m_iShakeY,m_iWidth,m_iHeight, m_hDC, 0, 0, BLACKNESS);
		else
			BitBlt(hdc, 0,0,m_iWidth,m_iShakeY, m_hDC, 0, 0, BLACKNESS);

		goto pass_label;
	}
	if (m_iShakeY == 0)
	{	
		if (m_iShakeX < 0)
			BitBlt(hdc, m_iWidth+m_iShakeX,0,m_iWidth,m_iHeight, m_hDC, 0, 0, BLACKNESS);
		else
			BitBlt(hdc, 0,0,m_iShakeX,m_iHeight, m_hDC, 0, 0, BLACKNESS);

		goto pass_label;
	}

	if (m_iShakeX < 0 && m_iShakeY < 0)
	{	
		BitBlt(hdc, m_iWidth+m_iShakeX,0,m_iWidth,m_iHeight, m_hDC, 0, 0, BLACKNESS);
		BitBlt(hdc, 0,m_iHeight+m_iShakeY,m_iWidth,m_iHeight, m_hDC, 0, 0, BLACKNESS);
		goto pass_label;
	}
	if (m_iShakeX < 0 && m_iShakeY > 0)
	{
		BitBlt(hdc, m_iWidth+m_iShakeX,0,m_iWidth,m_iHeight, m_hDC, 0, 0, BLACKNESS);
		BitBlt(hdc, 0,0,m_iWidth,m_iShakeY, m_hDC, 0, 0, BLACKNESS);
		goto pass_label;
	}
	if (m_iShakeX > 0 && m_iShakeY < 0)
	{
		BitBlt(hdc, 0,0,m_iShakeX,m_iHeight, m_hDC, 0, 0, BLACKNESS);
		BitBlt(hdc, 0,m_iHeight+m_iShakeY,m_iWidth,m_iHeight, m_hDC, 0, 0, BLACKNESS);
		goto pass_label;
	}
	if (m_iShakeX > 0 && m_iShakeY > 0)
	{
		BitBlt(hdc, 0,0,m_iShakeX,m_iHeight, m_hDC, 0, 0, BLACKNESS);
		BitBlt(hdc, 0,0,m_iWidth,m_iShakeY, m_hDC, 0, 0, BLACKNESS);
		goto pass_label;
	}

pass_label:

	BitBlt(hdc, m_iShakeX, m_iShakeY, rectDest.right, rectDest.bottom, m_hDC, 0, 0, SRCCOPY);
	DeleteObject ( hCombineRgn );
	ReleaseDC(m_hDibWnd,hdc);

	active(FALSE);

	EndPaint(m_hDibWnd, &ps);

	m_iShakeX	=	0,m_iShakeY	=	0,m_isShakeScreen=FALSE;

	return TRUE;
}

BOOL
CDib::active(BOOL _bIsActive)
{
	if	(!IsWindow(m_hDibWnd))
		return FALSE;

	if	(m_bIsActive == _bIsActive)
		return FALSE;

	if	(_bIsActive)
	{
		cDRAW::Active(m_p2SCREEN,m_iWidth,m_iHeight);

		m_hDC				= CreateCompatibleDC(0);
		m_hDefaultBitmap	= (HBITMAP)SelectObject(m_hDC, m_hBM);

		m_bIsActive			= TRUE;

		return TRUE;
	}

	SelectObject(m_hDC, m_hDefaultBitmap);
	DeleteObject(m_hDefaultBitmap);
	DeleteDC(m_hDC);

	m_hDC		=	0;

	m_bIsActive	=	FALSE;

	return TRUE;
}

//
//	종료
void
CDib::close()
{
	active(FALSE);

	if (m_hBM)
	{	
		DeleteObject(m_hBM);
		m_hBM	=	NULL;
	}
}	//	CDib::close()

void
CDib::shake(int _iXs,int _iYs)
{
	m_isShakeScreen	=	TRUE;
	m_iShakeX		=	_iXs;
	m_iShakeY		=	_iYs;
}

//
//	업뎃
void
CDib::update()
{
	//	페이드 인/아웃 관련 업데이트 
	{
		if	(m_iFadeScreenStatus	==	dFADE_IN)
		{
			m_iFadeScreenValue	+=	m_iFadeScreenSpeed;

			m_iFadeScreenValue	=	min(m_iFadeScreenValue,32);
		}

		if	(m_iFadeScreenStatus	==	dFADE_OUT)
		{
			m_iFadeScreenValue	-=	m_iFadeScreenSpeed;

			m_iFadeScreenValue	=	max(m_iFadeScreenValue,0);
		}
	}
}