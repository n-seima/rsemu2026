/********************************************************************************

	class CANVAS DIB version	창모드 플밍을 위한것이다.
								범용적이지만 속도가 느리기 때문에 게임은 무리이고
								에디터 제작에 사용한다.

********************************************************************************/
#ifndef _classDIB_H_
#define _classDIB_H_

#include "SFC.H"

#define	dFADE_NONE	0
#define	dFADE_IN	1
#define	dFADE_OUT	2

class CDib
{
public:
	HWND					m_hDibWnd;
	HDC						m_hDC;
	WORD					*m_p2SCREEN;
	HBITMAP					m_hBM;
	cRECT					m_rectView;
	HBITMAP					m_hDefaultBitmap;
	BOOL					m_bIsActive;			//	활성화 상태
	HFONT					m_hFont;				//	폰트

	BOOL					m_isShakeScreen,m_bIsCompleteFadeOut,m_bIsCompleteFadeIn;
	int						m_iShakeX,m_iShakeY;
	int						m_iFadeScreenStatus,m_iFadeScreenValue,m_iFadeScreenSpeed;
	WORD					m_wFadeColor;

	int						m_iWidth,m_iHeight;

							CDib();
							CDib(HWND hwnd);
							~CDib();

	inline	int				getFadeScreenStatus()	{return	m_iFadeScreenStatus;}
	inline	int				getFadeScreenValue()	{return	m_iFadeScreenValue;}
	void					fadeIn(WORD _wDestColor=0,int _iSpeed=4);
	void					fadeOut(WORD _wDestColor=0,int _iSpeed=4);
	void					cancelFadeScreen();
	void					setScreenFilter(WORD _wDestColor,int _iDepth);

	BOOL					isCompleteFadeOut();
	BOOL					isCompleteFadeIn();

	BOOL					init(HWND hwnd,int _iMinWidth=0,int _iMinHeight=0);
	void					destroy();
	BOOL					flip();
	BOOL					active(BOOL _bIsActive);
	void					close();
	void					update();
	void					operateFadeScreen();

	void					shake(int _iXs,int _iYs);
};

#endif