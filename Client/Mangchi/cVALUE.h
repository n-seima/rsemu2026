#ifndef _classVALUE_H
#define _classVALUE_H

#include	"SFC.H"

#define	dVALUE_COUNT		1024

#define	dVALID_VALUE		0x7fffffff

#define	dVALUE_LOCAL		0
#define	dVALUE_GLOBAL		1

#define	dSIZEOF_VALUE_NAME	32

class	cVALUE_ITEM
{
public:
	int		m_iValue;
	char	m_strName[dSIZEOF_VALUE_NAME];
};

class	cVALUE
{
public:
	cVALUE_ITEM		m_aValue[dVALUE_COUNT];
	int				m_iCount;

					cVALUE	();

	void			Reset	();

	int				Add		(char *strName,int iDefaultValue);
	BOOL			Remove	(int index);
	BOOL			Remove	(char *strName);
	int				GetLastIndex();
	inline	int		GetValue(int index)	{return	m_aValue[index].m_iValue;}
	inline	char*	GetName	(int index)	{return	m_aValue[index].m_strName;}
};

extern	cVALUE		g_ProjectValue,*g_lpValue;

#endif