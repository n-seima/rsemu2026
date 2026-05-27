#ifndef	_classBanner_
#define	_classBanner_

#include "cNemo.h"
#include "bannerDefine.h"
#include "CGamePlay.h"

#define	dSORT_OBJECT_BORDER_BANNER	80000

class	cBanner	:	public cBannerDefine
{
public:
	DWORD		m_dwPopupTextTime;
	void		copy(cBanner *_lpBanner);
	void		reset();
	void		putVertical(int _iX,int _iY,BOOL _bIsFocused);
	void		putHorizon(int _iX,int _iY,BOOL _bIsFocused);
	void		put(int _iX,int _iY,BOOL _bIsFocused);

	BOOL		isFocus(int _iX,int _iY);
	BOOL		isFocusVertical(int _iX,int _iY);
	BOOL		isFocusHorizon(int _iX,int _iY);
	void		showText();
};

class	cBannerManager : public CGamePlay
{
public:
	WORD				m_wIsWaitBannerText;
	DWORD				m_dwAskBannerTextTime;

	int					m_iFocusBanner;
	cBanner				m_aBannerList[dMAX_BANNER_COUNT_IN_FIELD];

						cBannerManager()
						{
						}

						~cBannerManager()
						{
						}

	void				reset();
	cBanner*			get(int _iIndex);
	BOOL				add(cBanner *_lpBanner);
	BOOL				remove(int _iIndex);
	void				update();

	BOOL				generate(int _iSerial,int _iX,int _iY,int _iShape,BOOL _bIsVertical,char *_lpstrTitle);
	void				setText(int _iSerial,char *_lpstrText);

	int					getViewBanners(cINDEX *_lpIndexes,cRECT *_lpRectViewport);
	void				drawBanner(int _iIndex,int _iDX,int _iDY);
	void				strcpyVerticalBannerTitle(char *_lpstrText,char *_lpstrMessage);

	void				setAskBannerTextTime();
	BOOL				isAbleToAskBannerText();
};

extern	cBannerManager	g_bannerManager;

#endif