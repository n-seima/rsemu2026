//-----------------------------------------------------	
// 이 클래스에 관한 것은 CCanvas.H의 머리를 참조 하길..
//-----------------------------------------------------

#include "CCanvas.h"
#include "cDRAW.H"
#include "cPROFILE.H"
#include "CText.H"

CCanvas	g_canvas;

BOOL	CCanvas::s_bIsExist	=	FALSE;

CCanvas::CCanvas()
{
	if (s_bIsExist)
		ERRMSG("error in CCanvas::CCanvas()","캔버스를 두개 이상 설정하면 안되요!!");

	s_bIsExist		=	TRUE;
	m_isActive		=	FALSE;
	m_isFullScreen	=	FALSE;
	m_lpDD			=	NULL;		// 다이렉트 드로우 오브젝트
	m_hWnd			=	NULL;		// 윈도우 핸들..
}

//	클래스 소멸..
CCanvas::~CCanvas()
{	destroy();
}

/***************************************************************************
void	Destroy()
	클래스 내에 객체 소멸..

***************************************************************************/

void
CCanvas::destroy()
{
	if(m_isActive)
    {
		m_lpDD->SetCooperativeLevel( m_hWnd, DDSCL_NORMAL );
		m_lpDD->RestoreDisplayMode();

		m_ddraw.destroy();
		m_dib.destroy();

		RELEASE(m_lpDD);

		m_isActive	=	FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////////
//BOOL CCanvas::Init()
//	캔버스를 생성 한다...

//return value:
//	성공시 TRUE를 실패시 FALSE 를 반환 한다.

////////////////////////////////////////////////////////////////////////////////////

BOOL 
CCanvas::init(HWND hwnd,int xs,int ys,int bpp,BOOL bfull,int _iDevice)
{
	if (ERRORED)	return FALSE;

	if (!m_ddraw.isSupportDX8())	return ERRMSG("이 프로그램을 실행시키기 위해서는 다이렉트 엑스 8.0 이상이 필요합니다.");

	HRESULT	hr;

	hr	=	DirectDrawCreateEx ( NULL, (void **)&m_lpDD, IID_IDirectDraw7, NULL );

	if ( hr != DD_OK )	return	ERRMSG(hr);

	m_iOutputDevice	=	_iDevice;
	m_hWnd			=	hwnd;
	m_iWidth		=	xs;
	m_iHeight		=	ys;
	m_iBpp			=	bpp;
	m_isFullScreen	=	bfull;

	if (_iDevice == eCD_DIRECTDRAW || _iDevice == eCD_AGP)
	{
		BOOL	bIsSupportAGP	=	FALSE;

		if (_iDevice == eCD_AGP)	bIsSupportAGP	=	TRUE;	

		if (!m_ddraw.init(hwnd,m_lpDD,xs,ys,bpp,bfull,bIsSupportAGP))	return	FALSE;
	}

	if (_iDevice == eCD_DIB)
	{
		if (m_isFullScreen)
		{
			hr	=	m_lpDD->SetCooperativeLevel ( m_hWnd, DDSCL_EXCLUSIVE	| DDSCL_FULLSCREEN);
			if ( hr != DD_OK )	return	ERRMSG(hr);

			hr	=	m_lpDD->SetDisplayMode(m_iWidth, m_iHeight, m_iBpp, 0, 0 );
			if ( hr != DD_OK )	return	ERRMSG(hr,"CDDraw::m_lpDD->SetDisplayMode");
		}
       
		m_dib.init(hwnd,m_iWidth,m_iHeight);
	}

	cDRAW::Init();

	return TRUE;
}

void
CCanvas::shake(int _iXs,int _iYs)
{
	m_ddraw.shake(_iXs,_iYs);
	m_dib.shake(_iXs,_iYs);
}

BOOL
CCanvas::getDC(HDC *_hDC)
{
	if (m_iOutputDevice == eCD_DIB)
	{
		*_hDC	=	m_dib.m_hDC;

		return TRUE;
	}

	return	m_ddraw.getDC(_hDC);
}

//
//	릴리즈
BOOL
CCanvas::releaseDC(HDC hDC)
{
	if (m_iOutputDevice == eCD_DIB)	return TRUE;

	return	m_ddraw.releaseDC(hDC);
}

//
//	텍스트를 출력을 위해 DC 얻기
BOOL
CCanvas::getDCForText(HDC *_hDC)
{
	if (m_iOutputDevice == eCD_DIB)
	{
		if (*_hDC	==	0)
		{
			*_hDC	=	m_dib.m_hDC;
			return	TRUE;
		}

		return FALSE;
	}

	return	m_ddraw.getDCForText(_hDC);
}

//
//	릴리즈
BOOL
CCanvas::releaseDCForText(HDC hDC)
{
	if	(m_iOutputDevice == eCD_DIB)
		return FALSE;

	return	m_ddraw.releaseDCForText(hDC);
}

//
//	훼이드 인
void
CCanvas::fadeIn(WORD _wDestColor,int _iSpeed)
{
	m_ddraw.fadeIn(_wDestColor,_iSpeed);
	m_dib.fadeIn(_wDestColor,_iSpeed);
}

//
//	훼이드 아웃
void
CCanvas::fadeOut(WORD _wDestColor,int _iSpeed)
{
	m_ddraw.fadeOut(_wDestColor,_iSpeed);
	m_dib.fadeOut(_wDestColor,_iSpeed);
}

void
CCanvas::cancelFadeScreen()
{
	m_ddraw.cancelFadeScreen();
	m_dib.cancelFadeScreen();
}

void
CCanvas::updateEffect()
{
	m_ddraw.update();
	m_dib.update();
}

void
CCanvas::setScreenFilter(WORD	_wDestColor,int _iDepth)
{
	m_dib.setScreenFilter(_wDestColor,_iDepth);
	m_ddraw.setScreenFilter(_wDestColor,_iDepth);
}

BOOL
CCanvas::active()
{
	BOOL	bIsActtive;

	if (m_iOutputDevice == eCD_DIB)	bIsActtive	=	m_dib.active(TRUE);
	else							bIsActtive	=	m_ddraw.active();

	if (!bIsActtive)	return	FALSE;

	CText::ReadyForOutput();

	return	TRUE;
}

//
//	플립
void
CCanvas::draw()
{
	CText::OutputEnd();

	if (m_iOutputDevice == eCD_DIB)	
	{
		m_dib.flip();

		return;
	}

	m_ddraw.flip();
}