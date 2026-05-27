#ifndef	_classSHAKE_H
#define	_classSHAKE_H

#include "cNEMO.H"
#include "CGame.H" 

#define		dSHAKE_COUNT		100

class	CShakeInfo
{
public:
	WORD			m_wSerial;
	DWORD			m_dwBeginTime;
	DWORD			m_dwUpkeepTime;
	int				m_iSI;			//	진도 Seismic Intensity
	int				m_iRange;

	CPos			m_pos;			//	진원

					CShakeInfo()
					{
						m_wSerial	=	0xffff;
					}

	BOOL			update(DWORD _dwCurrentTime);
};

class	CShake	:	public CGame
{
public:
	CShakeInfo		m_aShake[dSHAKE_COUNT];
	DWORD			m_dwUpdateCount;

					CShake();

	void			close();

	int				m_iCount,m_iNewSerial;

	BOOL			add(int x,int y,int iIntensity,DWORD dwUpkeepTime,int iRange);
	void			update();
	void			remove(int index);
	int				getMySeismicIntensity();
};

extern	CShake		g_shaker;

#endif