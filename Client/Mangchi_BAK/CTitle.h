#ifndef _classTITLE_H
#define	_classTITLE_H

#include "CTitleDefine.h"

class	CTitle : public CTitleInfo
{
public:

	void		reset();
	int			getRequireLevel(int _iLevel);
	int			getEnableLevel(int _iTitleLevel,int _iOwnersLevel);
};

class	CTitleManager
{
public:
	int					m_iTitleCount;
	CTitle				m_aTitles[dMAX_TITLE_COUNT];

	BOOL				loadData(char *_lpstrfn=NULL);
	BOOL				saveData(char *_lpstrfn);

	inline	CTitle*		getTitle(int _iTitle)
	{
		if (_iTitle	>=	dMAX_TITLE_COUNT || _iTitle < 0	)	return	NULL;
		if (m_aTitles[_iTitle].m_wSerial == 0xffff		)	return	NULL;

		return	&m_aTitles[_iTitle];
	}
};

extern	CTitleManager	g_titleManager;

#endif