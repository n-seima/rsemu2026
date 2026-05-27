#ifndef _classSoundEffect_h
#define	_classSoundEffect_h

#include "SFC.H"

#define	dMAX_BGM_COUNT	100

class	CBgmInfo
{
public:
	char	m_strFileName[64];
	char	m_strTitle[64];
};

extern	CBgmInfo	g_aBgm[dMAX_BGM_COUNT];

#endif