#pragma	once

#include	<windows.h>

#define	dMAX_READ_LOG_COUNT	100000

class	cDuelFilter
{
public:
	int			m_iDuelSerialCount;
	int			m_aiDuelSerial[dMAX_READ_LOG_COUNT];

	cDuelFilter()
	{
		m_iDuelSerialCount	=	0;
	}

	void		resetDuelSerialList()
	{
		m_iDuelSerialCount	=	0;
	}

	BOOL		isExistDuelSerial(int _iDuelSerial,BOOL _bIsByAdd=FALSE)
	{
		if	(m_iDuelSerialCount	==	0 && _bIsByAdd == FALSE)
			return	TRUE;

		for (int i=m_iDuelSerialCount;i;)
		{
			--i;

			if	(m_aiDuelSerial[i]	==	_iDuelSerial)
				return	TRUE;
		}

		return	FALSE;
	}

	BOOL		addSearchDuelSerial(int _iDuelSerial)
	{
		if	(isExistDuelSerial(_iDuelSerial,TRUE)	||	m_iDuelSerialCount	>=	dMAX_READ_LOG_COUNT)
			return	FALSE;

		m_aiDuelSerial[m_iDuelSerialCount++]	=	_iDuelSerial;

		return	TRUE;
	}
};

extern	cDuelFilter		g_duelFilter;