/*
	개				  요 :  Direct Draw 를 기반으로 한 윈도우용 그래픽 라이브러리.
							본 클래스는 윈도우 95기반의 16bit 컬러 전용 그래픽 라이브러리입니다.

	제한			사항 :	본 클래스를 제작자의 허락 없이 어떤식으로도 전용하는것을 금합니다.

	주의			사항 :	

	필요	  라이브러리 :	ddraw.lib<ddraw.h>,dxguid.lib

	작성			날짜 :	몰라.. 오늘은.. 98.04.10
	첫번째	 	    갱신 :	98.05.16 윈도우창에서도 돌아 갈수 있도록 손봄.
							후위면서피스 포인터 외에 전위면 서피스 포인터 첨가..
							RGBmix 라는 RGB565,RGB555,BGR565 모드 공용 컬러 믹서기 추가..
							colWHITE ~ colDEEPPURPLE 까지 필셸 포맷에 관계 없는 컬러 정의 추가..
							640X480 전용 계산루틴 첨가..

	X번째		  갱신 :	다 날려 버리다!!
							Primary Suface만 생성하고, 후위면은 버퍼에 생성한다.
							Flipping시에 알파블렌딩을 이용한 FadeIn,FadeOut
	X+1번째		  갱신 :	또 다 날려 버리다!!
							Primary Suface와 백서피스 생성.. 백서피스는 비됴 메모리에 2개를 생성한다.
							그리고 시스템 메모리에도 알파블랜딩을 위한 버퍼 하나 생성..
*/

#ifndef _classCANVAS_H
#define _classCANVAS_H

#define D3D_OVERLOADS

#include "SFC.H"
#include "CDib.H"
#include "CDDraw.H"

//
//	Canvas Device
enum
{
	eCD_DIRECTDRAW,
	eCD_AGP,
	eCD_DIB,
};

#define	dFADE_NONE	0
#define	dFADE_IN	1
#define	dFADE_OUT	2

class CCanvas
{
	static	BOOL			s_bIsExist;

public:
	CDib					m_dib;
	CDDraw					m_ddraw;

	LPDIRECTDRAW7			m_lpDD;			// 다이렉트 드로우 오브젝트
	HWND					m_hWnd;			// 윈도우 핸들..

	int						m_iWidth,m_iHeight,m_iBpp;
	BOOL					m_isActive,m_isFullScreen;
	int						m_iOutputDevice;

							CCanvas();
							~CCanvas();

	BOOL					active();

	void					fadeIn(WORD _wDestColor=0,int _iSpeed=4);
	void					fadeOut(WORD _wDestColor=0,int _iSpeed=4);
	void					cancelFadeScreen();
	void					updateEffect();
	void					operateFadeScreen(WORD *_lpScreen,WORD *_lpDestScreen,int _iAlpha,int _iSurfaceWidth,WORD _wColor);
	int						getFadeStatus()
	{
		if	(m_iOutputDevice == eCD_DIB)
			return	m_dib.m_iFadeScreenStatus;

		return	m_ddraw.m_iFadeScreenStatus;
	}

	inline	BOOL			isCompleteFadeOut()
	{
		if (getFadeStatus()	!=	dFADE_OUT)
		{
			m_ddraw.m_iFadeScreenStatus	=	dFADE_NONE;
			m_dib.m_iFadeScreenStatus	=	dFADE_NONE;

			return	TRUE;
		}

		if (m_iOutputDevice == eCD_DIB)	return	m_dib.isCompleteFadeOut();

		return	m_ddraw.isCompleteFadeOut();
	}

	inline	BOOL			isCompleteFadeIn()
	{
		if	(getFadeStatus()	!=	dFADE_IN)
		{
			m_ddraw.m_iFadeScreenStatus	=	dFADE_NONE;
			m_dib.m_iFadeScreenStatus	=	dFADE_NONE;

			return	TRUE;
		}

		if	(m_iOutputDevice == eCD_DIB)
			return	m_dib.isCompleteFadeIn();

		return	m_ddraw.isCompleteFadeIn();
	}

	inline	int				getFadeScreenStatus()
	{
		if	(m_iOutputDevice == eCD_DIB)
			return	m_dib.getFadeScreenStatus();

		return	m_ddraw.getFadeScreenStatus();
	}
	inline	int				getFadeScreenValue()
	{
		if (m_iOutputDevice == eCD_DIB)
			return	m_dib.getFadeScreenValue();

		return	m_ddraw.getFadeScreenValue();
	}

	BOOL					init(HWND hwnd,int xs=800,int ys=600,int bpp=16,BOOL bfull=TRUE,int _iDevice=eCD_DIRECTDRAW);
	BOOL					loadCursor(char *fn);
	void					destroy();

	void					draw(void);
	void					shake(int xs,int ys);
	BOOL					getDC(HDC *hDC);
	BOOL					releaseDC(HDC hDC);
	BOOL					getDCForText(HDC *hDC);
	BOOL					releaseDCForText(HDC hDC);

	void					setScreenFilter(WORD	_wDestColor,int _iDepth);
};

extern	CCanvas		g_canvas;

#endif