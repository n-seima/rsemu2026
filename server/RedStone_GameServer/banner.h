#ifndef	_classBanner_
#define	_classBanner_

#include	"bannerDefine.h"

class	cFIELD;

class	cBanner	:	public cBannerDefine
{
public:
	cFIELD		*m_lpField;

	void		copy(cBanner *_lpBanner);
	void		reset();
};

class	cBannerManager
{
public:
	cDen<cBanner>		m_den;

						cBannerManager()
						{
							m_den.init(dMAX_BANNER_COUNT_IN_SERVER);
						}

						~cBannerManager()
						{
						}

	cBanner*			get(int _iIndex);
	int					add(cBanner *_lpBanner);
	BOOL				remove(int _iIndex);

	int					generate(cFIELD *_lpField,int _iX,int _iY,int _iShape,BOOL _bIsVertical,char *_lpstrTitle,char *_lpstrText);
	void				update();
	void				operateOncePerMinute();
};

extern	cBannerManager	g_bannerManager;

#endif