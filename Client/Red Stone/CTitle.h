#ifndef _classTITLE_H
#define	_classTITLE_H

#include "CTitleDefine.h"
#include "commonClass.H"

class	CTitle : public CTitleInfo
{
public:

	void		reset();
	int			getRequireLevel(int _iLevel);
	int			getEnableLevel(int _iTitleLevel,int _iOwnersLevel);
	char*		getTooltip(int _iTitleLevel,int _iOwnersLevel);
	char*		getEffectComment(int _iTitle,int *_lpiValues);
};

class	CTitleManager
{
public:
	int					m_iTitleCount;
	CTitle				m_aTitles[dMAX_TITLE_COUNT];

	void				reset();
	BOOL				init();
	BOOL				loadData();
	BOOL				saveData();
	BOOL				addTitle(CTitle *_lpTitle);

	inline	CTitle*		getTitle(int _iTitle)
	{
		if (_iTitle	>=	dMAX_TITLE_COUNT || _iTitle < 0	)	return	NULL;
		if (m_aTitles[_iTitle].m_wSerial == 0xffff		)	return	NULL;

		return	&m_aTitles[_iTitle];
	}

	inline	CTitle*		getTitle(char *_lpstrName)
	{
		for (int i=0;i<dMAX_TITLE_COUNT;i++)
		{
			if (m_aTitles[i].m_wSerial	==	0xffff)	continue;

			if (STRICMP(m_aTitles[i].m_strName,_lpstrName)	==	0)
				return	&m_aTitles[i];
		}

		return	NULL;
	}

};

extern	CTitleManager		g_titleManager;

#endif