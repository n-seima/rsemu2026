#ifndef _classTITLE_H
#define _classTITLE_H

#include "CTitleDefine.H"

class	CTitle : public CTitleInfo
{
public:
	int					getRequireLevel(int _iLevel);
	int					getEnableLevel(int _iLevel,int _iOwnersLevel);
};

class	CTitleManager
{
public:
	int					m_iTitleCount;
	CTitle				m_aTitles[dMAX_TITLE_COUNT];

	CTitleManager()
	{
		m_iTitleCount	=	0;
	}

	BOOL				init();
	CTitle*				getTitleByName(char *_lpstrTitleName);
	inline	CTitle*		getTitle(int _iTitle)
	{
		if (_iTitle	>=	dMAX_TITLE_COUNT || _iTitle < 0	)	return	NULL;
		if (m_aTitles[_iTitle].m_wSerial == 0xffff		)	return	NULL;

		return	&m_aTitles[_iTitle];
	}
};

extern	CTitleManager	g_titleManager;

#endif