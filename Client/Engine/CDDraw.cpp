//-----------------------------------------------------	
// 이 클래스에 관한 것은 CDDraw.H의 머리를 참조 하길..
//-----------------------------------------------------

#include "CDDraw.h"
#include "cDRAW.H"
#include "cPROFILE.H"
#include "cExceptionHandler.H"

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//	CSurface 클래스 - 서피스를 관리한다.
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

CSurface::CSurface()
{
	m_lpDDS		=	NULL;
	m_lpScreen	=	NULL;
}

CSurface::~CSurface()
{
	Destroy();
}

BOOL
CSurface::Init(LPDIRECTDRAW7 lpdd,DDSURFACEDESC2	*ddsd,int width,int height)
{
	m_iWidth			=	width;
	m_iHeight			=	height;

	HRESULT	hr	=	lpdd->CreateSurface (ddsd,&m_lpDDS, NULL);

	if ( hr != DD_OK )	return	ERRMSG(hr);

	return TRUE;
}

void
CSurface::Destroy()
{
	RELEASE(m_lpDDS);
}

BOOL
CSurface::Restore()
{
	int failed	=	0;

	while (m_lpDDS->IsLost() != DD_OK)
	{
		failed++;

		HRESULT result	=	m_lpDDS->Restore();

		if (failed	>=	100)
			return FALSE;
	}

	return TRUE;
}

BOOL
CSurface::GetAttachedSurface(DDSCAPS2	*_lpDdscaps,CSurface *surface)
{
	HRESULT	hr	=	m_lpDDS->GetAttachedSurface(_lpDdscaps, &surface->m_lpDDS);

	if ( hr != DD_OK )	return	ERRMSG(hr);

	surface->m_iWidth	=	m_iWidth;
	surface->m_iHeight	=	m_iHeight;

	return TRUE;
}

void
CSurface::Clear(WORD fill)
{
	DDBLTFX ddBltFx;
	ddBltFx.dwSize		= sizeof(DDBLTFX);
	ddBltFx.dwFillColor = fill;

	m_lpDDS->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddBltFx);
}

void
CSurface::Fill(WORD fill,int x,int y,int xs,int ys)
{
	DDBLTFX ddBltFx;
	ddBltFx.dwSize		= sizeof(DDBLTFX);
	ddBltFx.dwFillColor = fill;

	RECT rect;
	SetRect(&rect,x,y,x+xs,y+ys);

	m_lpDDS->Blt(&rect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddBltFx);
}

BOOL
CSurface::BltFast(CSurface *srcSurface,int x,int y,int x1,int y1,int x2,int y2)
{
	int	xs	=	x2-x1+1;
	int	ys	=	y2-y1+1;

	if (x + xs >= m_iWidth)	x2	-=	(x + xs - m_iWidth );
	if (y + ys >= m_iHeight)	y2	-=	(y + ys - m_iHeight);

	if (x2	>= srcSurface->m_iWidth)	x2=srcSurface->m_iWidth -1;
	if (y2	>= srcSurface->m_iHeight)	y2=srcSurface->m_iHeight-1;

	RECT	rect;
	SetRect(&rect,x1,y1,x2,y2);

	HRESULT	hr	=	m_lpDDS->BltFast(x,y,srcSurface->m_lpDDS,&rect,DDBLTFAST_WAIT);
	
	if(hr == DDERR_SURFACELOST)
	{	
		Restore();
		return FALSE;
	}
	
//	//TEST(hr != DD_OK,g_eh.addStaticLog(hr,"CSurface::BltFast(CSurface *srcSurface,int x,int y,int x1,int y1,int x2,int y2)"));

	return TRUE;
}

BOOL
CSurface::BltFast(CSurface *srcSurface,int x1,int y1,int x2,int y2)
{
	RECT	rect;

	if (x1	< 	m_iWidth)	x1	=	0;
	if (y1	< 	m_iHeight)	y1	=	0;
	if (x2	>=	m_iWidth)	x2	=	m_iWidth-1;
	if (y2	>=	m_iHeight)	y2	=	m_iHeight-1;

	SetRect(&rect,x1,y1,x2,y2);

	HRESULT	hr	=	m_lpDDS->BltFast(x1,y1,srcSurface->m_lpDDS,&rect,DDBLTFAST_WAIT);

	if(hr == DDERR_SURFACELOST)
	{	
		Restore();
		return FALSE;
	}

	//TEST(hr!=DD_OK,g_eh.addStaticLog(hr,"CSurface::BltFast(CSurface *srcSurface,int x1,int y1,int x2,int y2)"));

	return TRUE;
}

BOOL
CSurface::BltFast(CSurface *srcSurface)
{
	HRESULT	hr	=	m_lpDDS->BltFast(0,0,srcSurface->m_lpDDS,NULL,DDBLTFAST_WAIT);

	if(hr == DDERR_SURFACELOST)
	{	
		Restore();

		g_eh.addStaticLog("CSurface::BltFast");

		return FALSE;
	}

	//TEST(hr!=DD_OK	,g_eh.addStaticLog(hr,"CSurface::BltFast(CSurface *srcSurface)"));

	return TRUE;
}

BOOL
CSurface::BltFast(DWORD dwX,DWORD dwY,CSurface *surface,RECT* lpSrcRect,DWORD dwTrans)
{
	HRESULT	hr	=	m_lpDDS->BltFast(dwX,dwY,surface->m_lpDDS,lpSrcRect,dwTrans);

	if(hr == DDERR_SURFACELOST)
	{	
		Restore();
		return FALSE;
	}

	//TEST(hr!=DD_OK	,g_eh.addStaticLog(hr,"CSurface::BltFast(DWORD dwX,DWORD dwY,CSurface *surface,RECT* lpSrcRect,DWORD dwTrans)"));

	return TRUE;
}

BOOL
CSurface::Blt(CSurface *srcSurface,int x,int y,int x1,int y1,int x2,int y2)
{
	RECT	srcRect,destRect;
	
	int	xs	=	x2-x1+1;
	int	ys	=	y2-y1+1;

	if (x1	< 	0					)	x2+=x1,x1=0;
	if (y1	< 	0					)	y2+=y1,y1=0;
	if (x	< 	0					)	xs+=x ,x =0;
	if (y	< 	0					)	ys+=y ,y =0;

	if (x + xs >= m_iWidth				)	xs	-=	(x + xs - m_iWidth );
	if (y + ys >= m_iHeight			)	ys	-=	(y + ys - m_iHeight);
	if (x2	>= srcSurface->m_iWidth	)	x2=srcSurface->m_iWidth -1;
	if (y2	>= srcSurface->m_iHeight	)	y2=srcSurface->m_iHeight-1;

	SetRect(&srcRect,x,y,x+xs-1,y+ys-1);
	SetRect(&destRect,x1,y1,x2,y2);

	if (destRect.right	< 0			)	return TRUE;
	if (destRect.bottom	< 0			)	return TRUE;
	if (srcRect.right	< 0			)	return TRUE;
	if (srcRect.bottom	< 0			)	return TRUE;

	HRESULT	hr	=	m_lpDDS->Blt(&srcRect, srcSurface->m_lpDDS,&destRect, DDBLT_WAIT, NULL );

	if(hr == DDERR_SURFACELOST)
	{	
		Restore();
		return FALSE;
	}

	//TEST(hr!=DD_OK	,g_eh.addStaticLog(hr,"CSurface::Blt(CSurface *srcSurface,int x,int y,int x1,int y1,int x2,int y2)"));

	return TRUE;
}

BOOL
CSurface::Blt(CSurface *srcSurface,int x1,int y1,int x2,int y2)
{
	RECT	rect;

	if (x1	< 	0					)	x1	=	0;
	if (y1	< 	0					)	y1	=	0;
	if (y2	< 	0					)	return TRUE;
	if (x2	< 	0					)	return TRUE;

	if (x2	>= srcSurface->m_iWidth	)	x2=srcSurface->m_iWidth -1;
	if (y2	>= srcSurface->m_iHeight	)	y2=srcSurface->m_iHeight-1;

	SetRect(&rect,x1,y1,x2,y2);

	HRESULT	hr	=	m_lpDDS->Blt(&rect, srcSurface->m_lpDDS,&rect, DDBLT_WAIT, NULL );

	if(hr == DDERR_SURFACELOST)
	{	
		Restore();
		return FALSE;
	}

	//TEST(hr!=DD_OK	,g_eh.addStaticLog(hr,"CSurface::Blt(CSurface *srcSurface,int x1,int y1,int x2,int y2)"));

	return TRUE;
}

//
//
BOOL
CSurface::Blt(CSurface *srcSurface,RECT *srcRect)
{
	HRESULT	hr	=	m_lpDDS->Blt(srcRect, srcSurface->m_lpDDS,NULL, DDBLT_WAIT, NULL );

	if(hr == DDERR_SURFACELOST)
	{	
		Restore();
		return FALSE;
	}

	//TEST(hr!=DD_OK	,g_eh.addStaticLog(hr,"CSurface::Blt(CSurface *srcSurface)"));

	return TRUE;
}	//	CSurface::Blt(CSurface *srcSurface,RECT *srcRect)

BOOL
CSurface::Blt(RECT *destRect,CSurface *surface,RECT *srcRect,DWORD flag,LPDDBLTFX lpDDBltFx)
{
	if (destRect->left < 0)
	{	if (destRect->right<0) return TRUE;
		srcRect->left	-=	destRect->left;
		destRect->left	=	0;
	}
	if (destRect->top < 0)
	{	if (destRect->bottom <0) return TRUE;
		srcRect->top	-=	destRect->top;
		destRect->top	=	0;
	}

	if (destRect->right >=	m_iWidth)
	{	srcRect->right	-=	(destRect->right -(m_iWidth-1));
		destRect->right	=	m_iWidth-1;
	}

	if (destRect->bottom >= m_iHeight)
	{	srcRect->bottom	-=	(destRect->bottom-(m_iHeight-1));
		destRect->bottom=	m_iHeight-1;
	}

	HRESULT	hr	=	m_lpDDS->Blt(destRect,surface->m_lpDDS,srcRect,flag,lpDDBltFx);

	if(hr == DDERR_SURFACELOST)
	{	
		Restore();
		return FALSE;
	}


	//TEST(hr!=DD_OK	,g_eh.addStaticLog(hr,"CSurface::Blt(RECT *destRect,CSurface *surface,RECT *srcRect,DWORD flag,LPDDBLTFX lpDDBltFx)"));

	return TRUE;
}

BOOL
CSurface::Flip()
{
	HRESULT	hr	=	m_lpDDS->Flip(NULL,DDFLIP_WAIT);

	if(hr == DDERR_SURFACELOST)
	{	
		Restore();

		return	FALSE;
	}
	
	//TEST(hr!=DD_OK	,g_eh.addStaticLog(hr,"CSurface::Flip()"));

	return TRUE;
}

WORD *
CSurface::Lock()
{	
	DDSURFACEDESC2	ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize	=	sizeof(ddsd);

	HRESULT hr	=	m_lpDDS->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);

	if ( hr != DD_OK)
	{
		if (hr == DDERR_SURFACEBUSY)
		{
			Unlock();

			return	NULL;
		}

		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize	=	sizeof(ddsd);
		HRESULT hr	=	m_lpDDS->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
	}

	if ( hr != DD_OK)	return	NULL;

	m_iSurfaceWidth	=	ddsd.lPitch/2;
	m_lpScreen		=	(WORD *)ddsd.lpSurface;

	return m_lpScreen;  
}	//	CSurface::Lock()

void
CSurface::Unlock()
{
	m_lpDDS->Unlock(NULL);
}


HRESULT
CSurface::GetSurfaceDesc(DDSURFACEDESC2 *ddsd)
{
	return	m_lpDDS->GetSurfaceDesc(ddsd);
}

BOOL
CSurface::GetDC(HDC *hDC)
{
	HRESULT	hr	=	m_lpDDS->GetDC(hDC);

	if	(hr	== DDERR_SURFACELOST)
	{	
		Restore();
		return FALSE;
	}

	//TEST(hr!=DD_OK	,g_eh.addStaticLog(hr,"CSurface::GetDC(HDC *hDC)"));

	return TRUE;
}

BOOL
CSurface::ReleaseDC(HDC hDC)
{
	m_lpDDS->ReleaseDC(hDC);

	return TRUE;
}

BOOL
CSurface::SetColorKey(WORD low,WORD high)
{
	DDCOLORKEY		colorkey;
	colorkey.dwColorSpaceLowValue	=	low;
	colorkey.dwColorSpaceHighValue	=	high;
	
	HRESULT	hr	=	m_lpDDS->SetColorKey(DDCKEY_SRCBLT,&colorkey);

	//TEST(hr!=DD_OK	,ERRMSG(hr,"CSurface::SetColorKey(WORD low,WORD high)"));

	return TRUE;
}

HRESULT
CSurface::SetClipper(LPDIRECTDRAWCLIPPER	lpClipper)
{
	return	m_lpDDS->SetClipper( lpClipper );
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//	CDDraw 클래스 - Direct Draw를 관리한다.
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

BOOL
CDDraw::isSupportDX8()
{  
	HINSTANCE	hD3D8DLL = LoadLibrary( "D3D8.DLL" );

    if( hD3D8DLL == NULL )	return FALSE;

    FreeLibrary( hD3D8DLL );

    return TRUE;
}

CDDraw::CDDraw()
{
	m_isActive				=	FALSE;
	m_isShakeScreen			=	FALSE;
	m_isFullScreen			=	FALSE;
	m_bIsSurpportAGP		=	FALSE;
	m_iShakeX				=	0;
	m_iShakeY				=	0;
	m_lp2Screen				=	NULL;
	m_iFadeScreenStatus		=	dFADE_NONE;
	m_iFadeScreenValue		=	0;
	m_iFadeScreenSpeed		=	4;
	m_iFlipMethod			=	eDDFM_FLIP;
	m_wFadeColor			=	0;

	m_lpDD					=	NULL;		// 다이렉트 드로우 오브젝트
	m_hWnd					=	NULL;		// 윈도우 핸들..
}

//	클래스 소멸..
CDDraw::~CDDraw()
{
	destroy();
}

/***************************************************************************
void	Destroy()
	클래스 내에 객체 소멸..

***************************************************************************/

void
CDDraw::destroy()
{
	if(m_isActive)
    {	
		m_lpDD->SetCooperativeLevel( m_hWnd, DDSCL_NORMAL );
		m_lpDD->RestoreDisplayMode();

		m_DDSBuffer.Destroy();
		m_DDSBack.Destroy();
     	m_DDSFront.Destroy();
		RELEASE(m_lpDD);
		m_isActive	=	FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////////
//BOOL CDDraw::Init()
//	캔버스를 생성 한다...

//return value:
//	성공시 TRUE를 실패시 FALSE 를 반환 한다.

////////////////////////////////////////////////////////////////////////////////////

BOOL 
CDDraw::init(HWND hwnd,LPDIRECTDRAW7 _lpDD,int xs,int ys,int bpp,BOOL bfull,BOOL _bIsSurpportAGP)
{
	if (ERRORED)
		return FALSE;

	if (!isSupportDX8())
		return ERRMSG("이 프로그램을 실행시키기 위해서는 다이렉트 엑스 8.0 이상이 필요합니다.");

	m_lpDD			=	_lpDD;
	m_bIsSurpportAGP=	_bIsSurpportAGP;
	m_hWnd			=	hwnd;
	m_iWidth		=	xs;
	m_iHeight		=	ys;
	m_iBpp			=	bpp;
	m_isFullScreen	=	bfull;
	m_iShakeX		=	0;
	m_iShakeY		=	0;

	if (!initDDraw()) return FALSE;

	return TRUE;
}

/***************************************************************************
BOOL CDDraw::InitDDRaw()
	Direct Draw를 초기화 시킨다.

return value:
	성공시 TRUE를..
	실패시 메시지와 함께 FALSE 를 반환 한다.

***************************************************************************/


BOOL 
CDDraw::initDDraw()
{	
	DDSCAPS2		ddscaps = { DDSCAPS_BACKBUFFER, 0, 0, 0 };
	DDSURFACEDESC2	ddsd;
	HRESULT			hr;

	if (m_isFullScreen)
	{
//		m_iFlipMethod	=	eDDFM_BLTFAST;

		if (m_iFlipMethod	==	eDDFM_FLIP)
		{
			hr =	m_lpDD->SetCooperativeLevel(m_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
			if ( hr != DD_OK )	return	ERRMSG(hr,"CDDraw::m_lpDD->SetCooperativeLevel");

			hr =	m_lpDD->SetDisplayMode(m_iWidth, m_iHeight, m_iBpp, 0, 0 );
			if ( hr != DD_OK )	return	ERRMSG(hr,"CDDraw::m_lpDD->SetDisplayMode");

			ZeroMemory ( &ddsd, sizeof ( DDSURFACEDESC2 ) );
			ddsd.dwSize				=	sizeof ( DDSURFACEDESC2 );
			ddsd.dwFlags			=	DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
			ddsd.ddsCaps.dwCaps		=	DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
			ddsd.dwBackBufferCount	=	2;

			if (!m_DDSFront.Init(m_lpDD,&ddsd,m_iWidth,m_iHeight)	)	return FALSE;

			if (!m_DDSFront.GetAttachedSurface(&ddscaps,&m_DDSBack)	)	return FALSE;
			g_eh.addStaticLog ("Succeed!! Direct Draw Initialized : [%dx%dx%d]\n",m_iWidth,m_iHeight,m_iBpp);
		}

		if (m_iFlipMethod	==	eDDFM_BLTFAST)
		{
			hr					=	m_lpDD->SetCooperativeLevel ( m_hWnd, DDSCL_NORMAL);
			if ( hr != DD_OK )	return	ERRMSG(hr);

			hr =	m_lpDD->SetDisplayMode ( m_iWidth, m_iHeight, m_iBpp, 0, 0 );
			if ( hr != DD_OK )	return	ERRMSG(hr,"CDDraw::m_lpDD->SetDisplayMode");

			ZeroMemory ( &ddsd, sizeof ( DDSURFACEDESC2 ) );
			ddsd.dwSize				=	sizeof ( DDSURFACEDESC2 );
			ddsd.dwFlags			=	DDSD_CAPS;
			ddsd.ddsCaps.dwCaps		=	DDSCAPS_PRIMARYSURFACE;

			if (!m_DDSFront.Init(m_lpDD,&ddsd,m_iWidth,m_iHeight)			)	return FALSE;

			ddsd.dwFlags			= DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
			ddsd.ddsCaps.dwCaps		= DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			ddsd.dwWidth			= m_iWidth;
			ddsd.dwHeight			= m_iHeight;
			if (!m_DDSBack.Init(m_lpDD,&ddsd,m_iWidth,m_iHeight)			)	return FALSE;
		}
	}
	else
	{
		hr					=	m_lpDD->SetCooperativeLevel ( m_hWnd, DDSCL_NORMAL);
		if ( hr != DD_OK )	return	ERRMSG(hr);

		ZeroMemory ( &ddsd, sizeof ( DDSURFACEDESC2 ) );
		ddsd.dwSize				=	sizeof ( DDSURFACEDESC2 );
		ddsd.dwFlags			=	DDSD_CAPS;
		ddsd.ddsCaps.dwCaps		=	DDSCAPS_PRIMARYSURFACE;

		if (!m_DDSFront.Init(m_lpDD,&ddsd,m_iWidth,m_iHeight)			)	return FALSE;

		ddsd.dwFlags			= DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
		ddsd.ddsCaps.dwCaps		= DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwWidth			= m_iWidth;
		ddsd.dwHeight			= m_iHeight;
		if (!m_DDSBack.Init(m_lpDD,&ddsd,m_iWidth,m_iHeight)			)	return FALSE;

		LPDIRECTDRAWCLIPPER		pcClipper;

		if( FAILED( hr = m_lpDD->CreateClipper( 0, &pcClipper, NULL ) ) )
			return ERRMSG(hr);

		if( FAILED( hr = pcClipper->SetHWnd( 0, m_hWnd) ) )
		{
			pcClipper->Release();
			return ERRMSG(hr);
		}

		if( FAILED( hr = m_DDSFront.SetClipper( pcClipper ) ) )
		{	pcClipper->Release();
			return ERRMSG(hr);
		}

		pcClipper->Release();
	}

	ZeroMemory ( &ddsd, sizeof ( DDSURFACEDESC2 ) );
	ddsd.dwSize			=	sizeof ( DDSURFACEDESC2 );
	ddsd.dwFlags		=	DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;

	if (m_bIsSurpportAGP &&	m_isFullScreen)	
			ddsd.ddsCaps.dwCaps	=	DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
	else	ddsd.ddsCaps.dwCaps	=	DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;

	ddsd.dwWidth		=	m_iWidth;
	ddsd.dwHeight		=	m_iHeight;

	if (!m_DDSBuffer.Init(m_lpDD,&ddsd,m_iWidth,m_iHeight)			)	return FALSE;

	CSurface	DDSTemp;

	ZeroMemory ( &ddsd, sizeof ( DDSURFACEDESC2 ) );
	ddsd.dwSize			=	sizeof ( DDSURFACEDESC2 );
	ddsd.dwFlags		=	DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.ddsCaps.dwCaps	=	DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth		=	16;
	ddsd.dwHeight		=	16;

	if (!DDSTemp.Init(m_lpDD,&ddsd,16,16)	)	return FALSE;

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize		= sizeof( ddsd );
    ddsd.dwFlags	= DDSD_PIXELFORMAT | DDSD_PITCH;
    HRESULT rval	= DDSTemp.GetSurfaceDesc(&ddsd);

	if(rval==DD_OK)
	{	if (ddsd.ddpfPixelFormat.dwRBitMask == 0xF800) 
		{	g_iPixelFormat=	dRGB565;
			RGBmix		=	RGB565mix;
			g_eh.addStaticLog("Pixel Format : RGB565");
		}
		if (ddsd.ddpfPixelFormat.dwRBitMask == 0x7C00) 
		{	g_iPixelFormat=	dRGB555;
			RGBmix		=	RGB555mix;
			g_eh.addStaticLog("Pixel Format : RGB555");
		}
		if (ddsd.ddpfPixelFormat.dwRBitMask == 0x001F) 
		{	g_iPixelFormat=	dBGR565;
			RGBmix		=	BGR565mix;
			g_eh.addStaticLog("Pixel Format : BGR565\n");
		}
	}	else ERRMSG(rval);


	DDSTemp.Destroy();

	m_isActive			=	TRUE;
	g_iRealPixelFormat	=	g_iPixelFormat;

	return TRUE;
}

void
CDDraw::restoreAllSurface()
{
	m_DDSFront.Restore();
	m_DDSBack.Restore();
	m_DDSBuffer.Restore();
}

void
CDDraw::shake(int _iXs,int _iYs)
{
	m_isShakeScreen	=	TRUE;
	m_iShakeX		=	_iXs;
	m_iShakeY		=	_iYs;
}

BOOL
CDDraw::active()
{
	m_lp2Screen	=	m_DDSBuffer.Lock();

	if (!m_lp2Screen)
	{
		restoreAllSurface();
		
		g_eh.addStaticLog("lock failed");
		return FALSE;
	}

	cDRAW::Active(m_lp2Screen,m_DDSBuffer.m_iSurfaceWidth,m_iHeight);

	return TRUE;
}

void
CDDraw::unlock()
{
	m_DDSBuffer.Unlock();
}

BOOL
CDDraw::getDC(HDC *hDC)
{
	return	m_DDSBuffer.GetDC(hDC);
}

BOOL
CDDraw::releaseDC(HDC hDC)
{
	return	m_DDSBuffer.ReleaseDC(hDC);
}

BOOL
CDDraw::getDCForText(HDC *hDC)
{
	unlock();

	return	m_DDSBuffer.GetDC(hDC);
}

BOOL
CDDraw::releaseDCForText(HDC hDC)
{
	BOOL	bIsResult	=	m_DDSBuffer.ReleaseDC(hDC);

	active();

	return	bIsResult;
}

//
//서피스 복구.
BOOL 
CDDraw::restore()
{
	HRESULT	hResult	=	m_lpDD->RestoreAllSurfaces();

	if (hResult	!=	DD_OK)
	{
		MSGOUT("CDDraw::restore",hResult);

		return FALSE;
	}

	return TRUE;
}

void
CDDraw::update()
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

void
CDDraw::fadeIn(WORD _wDestColor,int _iSpeed)
{
	m_iFadeScreenValue	=	0;
	m_iFadeScreenStatus	=	dFADE_IN;
	m_wFadeColor		=	_wDestColor;
	m_iFadeScreenSpeed	=	_iSpeed;
	m_bIsCompleteFadeOut=	FALSE;
}

void
CDDraw::fadeOut(WORD _wDestColor,int _iSpeed)
{
	m_iFadeScreenValue	=	32;
	m_iFadeScreenStatus	=	dFADE_OUT;
	m_wFadeColor		=	_wDestColor;
	m_iFadeScreenSpeed	=	_iSpeed;
	m_bIsCompleteFadeIn	=	FALSE;
}

void
CDDraw::cancelFadeScreen()
{
	m_iFadeScreenStatus	=	dFADE_NONE;
	m_bIsCompleteFadeIn	=	FALSE;
}

void
CDDraw::setScreenFilter(WORD	_wDestColor,int _iDepth)
{
	cancelFadeScreen();

	m_wFadeColor		=	_wDestColor;
	m_iFadeScreenValue	=	32*_iDepth/100;
}

BOOL
CDDraw::isCompleteFadeOut()
{
	if	(m_bIsCompleteFadeOut)
	{
		m_bIsCompleteFadeOut	=	FALSE;

		return	TRUE;
	}

	return	FALSE;
}

BOOL
CDDraw::isCompleteFadeIn()
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
BOOL
CDDraw::operateFadeScreen()
{
	{
		if	(m_iFadeScreenValue		== 32	)
		{
			if	(m_iFadeScreenStatus	==	dFADE_IN)
				m_bIsCompleteFadeIn		=	TRUE;

			return	TRUE;
		}

		if  (m_iFadeScreenValue		== 0	)
		{
			if	(m_iFadeScreenStatus	==	dFADE_OUT)
				m_bIsCompleteFadeOut	=	TRUE;

			m_DDSFront.Clear(m_wFadeColor);

			return	TRUE;
		}
	}

	CRY(m_iFadeScreenValue	<0	,"Error in CDDraw::Draw - m_iFadeScreenValue value's too low");
	CRY(m_iFadeScreenValue	>32	,"Error in CDDraw::Draw - m_iFadeScreenValue value's too high");

	UINT	dwFirst,dwSecond;
	UINT	*lp4DestScreen	=	(UINT *)m_DDSBack.Lock();
	UINT	*lp4SrcScreen	=	(UINT *)m_DDSBuffer.m_lpScreen;
	int		iSurfaceWidth	=	m_DDSBack.m_iSurfaceWidth/2;
	int		iWidth			=	m_iWidth/2;
	UINT	dwDestColor		=	((m_wFadeColor<<16)+m_wFadeColor);
	int		iDestAlpha		=	32-m_iFadeScreenValue;
	int		iBufferAddress	=	0;
	int		iSurfaceAddress	=	0;

	if	(!lp4DestScreen)	return	FALSE;

	if (g_iPixelFormat==dRGB555)
	{
		dwFirst		=	dwDestColor&d4AM_MASK_555_1;
		dwFirst		>>=	5;
		dwFirst		*=	iDestAlpha;
		dwFirst		&=	d4AM_MASK_555_1;

		dwSecond	=	dwDestColor&d4AM_MASK_555_2;
		dwSecond	*=	iDestAlpha;
		dwSecond	>>=	5;
		dwSecond	&=	d4AM_MASK_555_2;
		dwDestColor	=	dwFirst + dwSecond;

		iBufferAddress	=	m_iHeight*iWidth;
		iSurfaceAddress	=	m_iHeight*iSurfaceWidth;

		for (int y=m_iHeight;y;--y)
		{
			iBufferAddress	-=	iWidth;
			iSurfaceAddress	-=	iSurfaceWidth;

			for (int x=iWidth;x;)
			{
				--x;

				dwFirst		=	lp4SrcScreen[iBufferAddress+x]&d4AM_MASK_555_1;
				dwFirst		>>=	5;
				dwFirst		*=	m_iFadeScreenValue;
				dwFirst		&=	d4AM_MASK_555_1;

				dwSecond	=	lp4SrcScreen[iBufferAddress+x]&d4AM_MASK_555_2;
				dwSecond	*=	m_iFadeScreenValue;
				dwSecond	>>=	5;
				dwSecond	&=	d4AM_MASK_555_2;

				lp4DestScreen[iSurfaceAddress+x]	=	(dwFirst + dwSecond) + dwDestColor;
			}
		}
	}
	else
	{	
		dwFirst		=	dwDestColor&d4AM_MASK_565_1;
		dwFirst		>>=	5;
		dwFirst		*=	iDestAlpha;
		dwFirst		&=	d4AM_MASK_565_1;

		dwSecond	=	dwDestColor&d4AM_MASK_565_2;
		dwSecond	*=	iDestAlpha;
		dwSecond	>>=	5;
		dwSecond	&=	d4AM_MASK_565_2;

		dwDestColor	=	dwFirst + dwSecond;

		iBufferAddress	=	m_iHeight*iWidth;
		iSurfaceAddress	=	m_iHeight*iSurfaceWidth;

		for (int y=m_iHeight;y;--y)
		{
			iBufferAddress	-=	iWidth;
			iSurfaceAddress	-=	iSurfaceWidth;

			for (int x=iWidth;x;)
			{
				--x;

				dwFirst		=	lp4SrcScreen[iBufferAddress+x]&d4AM_MASK_565_1;
				dwFirst		>>=	5;
				dwFirst		*=	m_iFadeScreenValue;
				dwFirst		&=	d4AM_MASK_565_1;

				dwSecond	=	lp4SrcScreen[iBufferAddress+x]&d4AM_MASK_565_2;
				dwSecond	*=	m_iFadeScreenValue;
				dwSecond	>>=	5;
				dwSecond	&=	d4AM_MASK_565_2;

				lp4DestScreen[iSurfaceAddress+x]	=	(dwFirst + dwSecond) + dwDestColor;
			}
		}
	}

	m_DDSBack.Unlock();

	unlock();

	return	TRUE;
}

BOOL
CDDraw::draw(void)
{
	if (m_isShakeScreen)
	{
		if (m_iShakeX == 0 && m_iShakeY == 0)
			goto pasm_label;
		if (m_iShakeX == 0)
		{	
			if (m_iShakeY < 0)
				m_DDSBack.Fill(0,0,m_iHeight+m_iShakeY,m_iWidth,m_iHeight);
			else
				m_DDSBack.Fill(0,0,0,m_iWidth,m_iShakeY);
			goto pasm_label;
		}
		if (m_iShakeY == 0)
		{	
			if (m_iShakeX < 0)
				m_DDSBack.Fill(0,m_iWidth+m_iShakeX,0,m_iWidth,m_iHeight);
			else
				m_DDSBack.Fill(0,0,0,m_iShakeX,m_iHeight);

			goto pasm_label;
		}

		if (m_iShakeX < 0 && m_iShakeY < 0)
		{	
			m_DDSBack.Fill(0,m_iWidth+m_iShakeX,0,m_iWidth,m_iHeight);
			m_DDSBack.Fill(0,0,m_iHeight+m_iShakeY,m_iWidth,m_iHeight);
			goto pasm_label;
		}
		if (m_iShakeX < 0 && m_iShakeY > 0)
		{	
			m_DDSBack.Fill(0,m_iWidth+m_iShakeX,0,m_iWidth,m_iHeight);
			m_DDSBack.Fill(0,0,0,m_iWidth,m_iShakeY);
			goto pasm_label;
		}
		if (m_iShakeX > 0 && m_iShakeY < 0)
		{	
			m_DDSBack.Fill(0,0,0,m_iShakeX,m_iHeight);
			m_DDSBack.Fill(0,0,m_iHeight+m_iShakeY,m_iWidth,m_iHeight);
			goto pasm_label;
		}
		if (m_iShakeX > 0 && m_iShakeY > 0)
		{	
			m_DDSBack.Fill(0,0,0,m_iShakeX,m_iHeight);
			m_DDSBack.Fill(0,0,0,m_iWidth,m_iShakeY);
			goto pasm_label;
		}
	}

pasm_label:

	unlock();

	if (m_isShakeScreen)
	{	
		int x,y,x1,y1,x2,y2;

		x	=	max(m_iShakeX,0);
		y	=	max(m_iShakeY,0);
		x1	=	-(min(m_iShakeX,0));
		y1	=	-(min(m_iShakeY,0));
		x2	=	m_iWidth-1 - x1;
		y2	=	m_iHeight-1- y1;
		m_DDSBack.BltFast(&m_DDSBuffer,x,y,x1,y1,x2,y2);
	}
	else
		m_DDSBack.BltFast(&m_DDSBuffer);

	m_iShakeX	=	0,m_iShakeY	=	0,m_isShakeScreen=FALSE;

	return TRUE;
}

void
CDDraw::flip()
{
	if	(m_iFadeScreenStatus	==	dFADE_NONE)
		draw();
	else
	{
		if	(!operateFadeScreen())
			return;
	}

	if (m_isFullScreen)
	{
		if (m_iFlipMethod	==	eDDFM_FLIP	)	m_DDSFront.Flip();
		if (m_iFlipMethod	==	eDDFM_BLTFAST)	m_DDSFront.BltFast(&m_DDSBack);
	}
	else
	{
		static	RECT	rect;
		static	int		isFirst		=	TRUE;

		if (isFirst)
		{
			GetClientRect(m_hWnd,&rect);
			DWORD	dwExStyle	=	GetWindowLong(m_hWnd,GWL_EXSTYLE);
			DWORD	dwStyle		=	GetWindowLong(m_hWnd,GWL_STYLE);

			RECT tmp;
			SetRect(&tmp,100,100,200,200);

			AdjustWindowRectEx( &tmp,dwStyle,FALSE,dwExStyle);

			rect.left	+=	(100-tmp.left);
			rect.top	+=	(100-tmp.top);
			rect.right	+=	rect.left;
			rect.bottom	+=	rect.top;

			isFirst	=	FALSE;
		}

		RECT	outFrame;
		GetWindowRect(m_hWnd,&outFrame);

		outFrame.left	+=	rect.left;
		outFrame.right	=	outFrame.left+m_iWidth;
		outFrame.top	+=	rect.top;
		outFrame.bottom	=	outFrame.top+m_iHeight;


		m_DDSFront.Blt(&m_DDSBack,&outFrame);
//		Blt(&m_DDSBack,10,20,rect.left,rect.top,rect.right,rect.bottom);
	}
}