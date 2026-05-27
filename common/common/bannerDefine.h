#ifndef	_bannerDefine_
#define	_bannerDefine_

#include "sfc.h"

#define	dBANNER_UPKEEP_MINUTE		60
#define	dBANNER_TITLE_LENGTH		26
#define	dBANNER_COMMENT_LENGTH		102
#define	dMAX_BANNER_COUNT_IN_FIELD	100
#define	dMAX_BANNER_COUNT_IN_SERVER	10000

class cBannerSimpleInfo
{
public:
	WORD			m_wBannerSerial;
	WORD			m_wX,m_wY;
	WORD			m_bf15Shape		:	15;
	WORD			m_bf1IsVertical	:	1;
	char			m_strTitle[dBANNER_TITLE_LENGTH];
};

class	cBannerDefine
{
public:
	WORD			m_wSerial;
	WORD			m_wX,m_wY;
	WORD			m_wRemainMinute;

	WORD			m_bf15Shape		:	15;
	WORD			m_bf1IsVertical	:	1;

	char			m_strTitle[dBANNER_TITLE_LENGTH];
	char			m_strText[dBANNER_COMMENT_LENGTH];

	cBannerDefine()
	{
		m_wSerial	=	0xffff;
	}
};

#endif