#include "cFIELD.h"
#include "banner.h"
#include "cGame.h"

BOOL
cFIELD::isBannerFull()
{
	if	(m_bIsReadyToClose	||	m_wBannerCount	>=	dMAX_BANNER_COUNT_IN_FIELD)
		return	TRUE;

	return	FALSE;
}

void
cFIELD::addSendUpdateBannerInfo(cBanner *_lpBanner)
{
	SG_BANNER_INFO	packet;

	packet.base.set(sizeof(packet),dSG_BANNER_INFO);

	packet.bannerInfo.m_wBannerSerial=	_lpBanner->m_wSerial;
	packet.bannerInfo.m_wX			=	_lpBanner->m_wX;
	packet.bannerInfo.m_wY			=	_lpBanner->m_wY;
	packet.bannerInfo.m_bf15Shape	=	_lpBanner->m_bf15Shape;
	packet.bannerInfo.m_bf1IsVertical=	_lpBanner->m_bf1IsVertical;
	strcpy(packet.bannerInfo.m_strTitle,_lpBanner->m_strTitle);

	addSendPacket((ALL_MSG*)&packet,0xffff);
}

BOOL
cFIELD::isExistCrushBanner(int _iX,int _iY)
{
	for (int i=0;i<dMAX_BANNER_COUNT_IN_FIELD;i++)
	{
		if	(m_awBannerList[i]	==	0xffff)
			continue;

		cBanner	*lpBanner	=	g_bannerManager.get(m_awBannerList[i]);

		if	(!lpBanner)
		{
			m_awBannerList[i]=	0xffff;

			if	(m_wBannerCount)
				m_wBannerCount--;

			continue;
		}

		int	iRange	=	GetRange(lpBanner->m_wX,lpBanner->m_wY,_iX,_iY);

		if	(iRange	<	100*100)
			return	TRUE;
	}

	return	FALSE;
}

void
cFIELD::addActiveBanner(int _iSerial)
{
	int		i;
	cBanner	*lpBanner	=	g_bannerManager.get(_iSerial);

	if	(lpBanner)
		addSendUpdateBannerInfo(lpBanner);
	else
		return;

	for (i=0;i<dMAX_BANNER_COUNT_IN_FIELD;i++)
		if (m_awBannerList[i]	==	_iSerial)
			return;

	for (i=0;i<dMAX_BANNER_COUNT_IN_FIELD;i++)
		if (m_awBannerList[i]	==	0xffff)
		{
			m_awBannerList[i]	=	_iSerial;
			m_wBannerCount++;
			return;
		}
}

void
cFIELD::removeActiveBanner(int _iSerial)
{
	cBanner	*lpBanner	=	g_bannerManager.get(_iSerial);

	addSendEtcWork(0xffff,eEW_REMOVE_BANNER,_iSerial);

	for (int i=0;i<dMAX_BANNER_COUNT_IN_FIELD;i++)
		if	(m_awBannerList[i]	==	_iSerial)
		{
			m_awBannerList[i]	=	0xffff;

			if	(m_wBannerCount)
				m_wBannerCount--;
			return;
		}
}
