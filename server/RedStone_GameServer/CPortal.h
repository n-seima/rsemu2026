#ifndef _classPortal_h
#define _classPortal_h

#include "SFC.H"
#include "definePacketData.h"
#include "CItem.H"

#define	dMAX_PORTAL_IN_FIELD	100

class	cFIELD;

class	CPortalInfo
{
public:
	WORD	m_wOwner;
	WORD	m_wX,m_wY;
	WORD	m_wTargetX,m_wTargetY,m_wField;
	WORD	m_wRemainTime;
	DWORD	m_dwPartyUniqueSerial;
	DWORD	m_dwCreatorNameHashCode;
};

class	CPortalManager
{
public:
	int			m_iPortalCount;
	CPortalInfo	m_aPortal[dMAX_PORTAL_IN_FIELD];

				CPortalManager();

	void		reset();

	int			addPortal(cACTOR *_lpActor,int _iVillage,int _iRemainTime,int _iTargetX=0xffff,int _iTargetY=0xffff,DWORD _dwPartyUniqueSerial=0xffffffff);
	BOOL		removePortal(int _iSerial);
	int			removeActor(int _iSerial);
	
	void		update(cFIELD *_lpField);
};

#endif