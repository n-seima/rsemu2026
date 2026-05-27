#ifndef _classDDRAW_H
#define _classDDRAW_H

#define D3D_OVERLOADS

#include "SFC.H"
#include <ddraw.h>
#include <d3d.h>
#define	INITGUID


#pragma comment(lib,"ddraw.lib")	// Windows direct draw Library
#pragma comment(lib,"dxguid.lib")	// 
#pragma comment(lib,"winmm.lib")	// 

//
//	Direct Draw Flip Method
enum
{
	eDDFM_FLIP,
	eDDFM_BLTFAST
};

#define	dFADE_NONE	0
#define	dFADE_IN	1
#define	dFADE_OUT	2

class CSurface
{
public:
	LPDIRECTDRAWSURFACE7	m_lpDDS;

	int						m_iWidth,m_iHeight,m_iSurfaceWidth;
	WORD					*m_lpScreen;

							CSurface();
							~CSurface();

	WORD*					Lock();
	void					Unlock();

	BOOL					Init(LPDIRECTDRAW7 lpdd,DDSURFACEDESC2	*ddsd,int width,int height);
	BOOL					GetAttachedSurface(DDSCAPS2	*ddscaps,CSurface *surface);

	BOOL					Restore();
	void					Destroy();

	void					Clear(WORD color);
	void					Fill(WORD color,int x,int y,int xs,int ys);
	BOOL					BltFast(CSurface *srcSurface,int x,int y,int x1,int y1,int x2,int y2);
	BOOL					BltFast(CSurface *srcSurface,int x1,int y1,int x2,int y2);
	BOOL					BltFast(CSurface *srcSurface);
	BOOL					BltFast(DWORD dwX,DWORD dwY,CSurface *surface,RECT* lpSrcRect,DWORD dwTrans);
 
	BOOL					Blt(CSurface *srcSurface,int x,int y,int x1,int y1,int x2,int y2);
	BOOL					Blt(CSurface *srcSurface,int x1,int y1,int x2,int y2);
	BOOL					Blt(CSurface *srcSurface,RECT *srcRect=NULL);
	BOOL					Blt(RECT *destRect,CSurface *surface,RECT *srcRect,DWORD flag,LPDDBLTFX lpDDBltFx);

	BOOL					Flip();

	BOOL					SetColorKey(WORD low,WORD high);

	HRESULT					GetSurfaceDesc(DDSURFACEDESC2 *ddsd);
	HRESULT					SetClipper(LPDIRECTDRAWCLIPPER	lpClipper);

	BOOL					GetDC(HDC *hDC);
	BOOL					ReleaseDC(HDC hDC);

};

class	CDDraw
{
public:
	CSurface				m_DDSBack;
	CSurface				m_DDSFront;
	CSurface				m_DDSBuffer;

	LPDIRECTDRAW7			m_lpDD;			// 다이렉트 드로우 오브젝트

	WORD					*m_lp2Screen;
	int						m_iWidth,m_iHeight,m_iBpp;

	HWND					m_hWnd;			// 윈도우 핸들..

	BOOL					m_isActive,m_isShakeScreen,m_isFullScreen,m_bIsSurpportAGP,m_bIsCompleteFadeOut,m_bIsCompleteFadeIn;
	int						m_iShakeX,m_iShakeY,m_iFlipMethod;
	int						m_iFadeScreenStatus,m_iFadeScreenValue,m_iFadeScreenSpeed;
	WORD					m_wFadeColor;

							CDDraw();
							~CDDraw();

	BOOL					active();
	void					unlock();
	BOOL					restore();
	BOOL					isSupportDX8();

	void					fadeIn(WORD _wDestColor=0,int _iSpeed=4);
	void					fadeOut(WORD _wDestColor=0,int _iSpeed=4);
	void					cancelFadeScreen();
	void					setScreenFilter(WORD _wDestColor,int _iDepth);

	BOOL					isCompleteFadeOut();
	BOOL					isCompleteFadeIn();

	inline	int				getFadeScreenStatus()	{return	m_iFadeScreenStatus;}
	inline	int				getFadeScreenValue()	{return	m_iFadeScreenValue;}
	void					update();

	BOOL					init(HWND hwnd,LPDIRECTDRAW7 _lpDD,int xs=800,int ys=600,int bpp=16,BOOL bfull=TRUE,BOOL _bIsAGP=FALSE);
	void					destroy();
	BOOL					initDDraw();

	BOOL					operateFadeScreen();
	BOOL 					draw();
	void 					flip(void);
	void					restoreAllSurface();

	void					shake(int xs,int ys);
	BOOL					getDC(HDC *hDC);
	BOOL					releaseDC(HDC hDC);
	BOOL					getDCForText(HDC *hDC);
	BOOL					releaseDCForText(HDC hDC);
};

#endif