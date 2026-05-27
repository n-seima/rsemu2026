#ifndef _classPortal_h
#define _classPortal_h

#include "SFC.H"
#include "CGamePlay.H"

#define	dMAX_PORTAL_COUNT	100
#define	dBORDER_PORTAL		40000

class	CPortalInfo
{
public:
	WORD	m_wSerial,m_wOwner,m_wX,m_wY,m_wField,m_wIsTownPortal;
};

class	CPortalManager : public CGamePlay
{
public:
	int				m_iFocusPortal,m_iMovePortal,m_iEnterPortal;
	int				m_iWaitEnterPortalTime;
	
	CPortalInfo		m_aPortal[dMAX_PORTAL_COUNT];

	void			reset();

	int				getViewPortal(cINDEX *_lpIndexes,cRECT *_lpRectViewport);
	BOOL			isFocus(int _iSerial,int _iX,int _iY);

	void			update();
	int				getFocusPortal();

	void			draw(int _iSerial);
	BOOL			remove(int _iSerial);
	BOOL			add(int _iSerial,int _iField,int _iOwner,int _iX,int _iY,BOOL _bIsTownPortal);

};

extern	CPortalManager	g_portal;
extern	char	g_strMinimapCheckNpc[20];

#endif