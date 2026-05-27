#ifndef _classVALUE_H
#define _classVALUE_H

#include "cNEMO.H"

#define	dVALUE_COUNT		1024
#define	dVALID_VALUE		0x7fffffff

#define	dSIZEOF_VALUE_NAME	32

class	cVALUE_ITEM
{
public:
	int		m_iValue;
};

class	cVALUE
{
public:
	int				m_aValue[dVALUE_COUNT];
	int				m_iCount;

					cVALUE()	{Reset();}

	void			Reset();
};

extern	cVALUE		g_ProjectValue,g_LocalValue;

#endif