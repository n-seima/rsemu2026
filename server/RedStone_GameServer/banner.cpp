#include	"banner.h"
#include	"cField.h"
#include	"debugCode.h"

cBannerManager	g_bannerManager;

void
cBanner::copy(cBanner *_lpBanner)
{
	memcpy(this,_lpBanner,sizeof(cBannerDefine));
	m_lpField	=	_lpBanner->m_lpField;
}

void
cBanner::reset()
{
	m_wSerial	=	0xffff;
	m_lpField	=	NULL;
}

cBanner*
cBannerManager::get(int _iIndex)
{
	return	m_den.get(_iIndex);
}

int
cBannerManager::add(cBanner *_lpBanner)
{
	return	m_den.add(_lpBanner);
}

BOOL
cBannerManager::remove(int _iIndex)
{
	cBanner	*lpBanner	=	m_den.get(_iIndex);

	if	(!lpBanner)
		return	FALSE;

	if	(lpBanner->m_lpField)
		lpBanner->m_lpField->removeActiveBanner(_iIndex);

	return	m_den.remove(_iIndex);
}

int
cBannerManager::generate(cFIELD *_lpField,int _iX,int _iY,int _iShape,BOOL _bIsVertical,char *_lpstrTitle,char *_lpstrText)
{
	cBanner	banner;

	banner.m_lpField		=	_lpField;
	banner.m_wX				=	_iX;
	banner.m_wY				=	_iY;
	banner.m_wRemainMinute	=	dBANNER_UPKEEP_MINUTE;
	banner.m_bf15Shape		=	_iShape;
	banner.m_bf1IsVertical	=	_bIsVertical;
	
	strcpy(banner.m_strTitle,_lpstrTitle);
	strcpy(banner.m_strText,_lpstrText);

	if	(_lpField->isBannerFull())
		return	0xffff;

	int	iSerial			=	add(&banner);

	if	(iSerial		==	0xffff)
		return	0xffff;

	_lpField->addActiveBanner(iSerial);

	return	TRUE;
}

void
cBannerManager::update()
{
}

void
cBannerManager::operateOncePerMinute()
{
	if	(g_bIsTestServer	==	FALSE)
		return;

	int	iOldCount	=	m_den.getCount(),iCount=0,i;

	for(i=0;i < m_den.getMaxCount() && iCount < iOldCount;i++)
	{
		cBanner	*lpBanner	=	m_den.get(i);

		if	(!lpBanner)
			continue;

		iCount++;

		lpBanner->m_wRemainMinute--;

		if	(lpBanner->m_wRemainMinute	==	0)
			remove(i);
	}
}