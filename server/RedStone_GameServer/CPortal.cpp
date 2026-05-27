#include "CPortal.h"
#include "CActor.h"
#include "CField.h"

CPortalManager::CPortalManager()
{
	reset();
}

void
CPortalManager::reset()
{
	m_iPortalCount	=	0;

	for (int i=0;i<dMAX_PORTAL_IN_FIELD;i++)
		m_aPortal[i].m_wOwner	=	0xffff;
}

int
CPortalManager::addPortal(cACTOR *_lpActor,int _iVillage,int _iRemainTime,int _iX,int _iY,DWORD _dwPartyUniqueSerial)
{
	if	(m_iPortalCount	>=	dMAX_PORTAL_IN_FIELD)
		return	0xffff;

	int	iSerial	=	0xffff;

	for (int i=0;i<dMAX_PORTAL_IN_FIELD;i++)
		if	(m_aPortal[i].m_wOwner	==	0xffff)
		{
			iSerial	=	i;
			break;
		}

	m_aPortal[iSerial].m_wField				=	_iVillage;
	m_aPortal[iSerial].m_wX					=	_lpActor->m_pos.x;
	m_aPortal[iSerial].m_wY					=	_lpActor->m_pos.y;
	m_aPortal[iSerial].m_wTargetX			=	_iX;
	m_aPortal[iSerial].m_wTargetY			=	_iY;
	m_aPortal[iSerial].m_wRemainTime		=	_iRemainTime;
	m_aPortal[iSerial].m_wOwner				=	_lpActor->m_wSerialInField;
	m_aPortal[iSerial].m_dwPartyUniqueSerial=	_dwPartyUniqueSerial;
	m_aPortal[iSerial].m_dwCreatorNameHashCode=	_lpActor->m_dwNameHashCode;

	m_iPortalCount++;

	return	iSerial;
}

void
CPortalManager::update(cFIELD *_lpField)
{
	for (int i=0;i<dMAX_PORTAL_IN_FIELD;i++)
	{
		if (m_aPortal[i].m_wOwner	==	0xffff)
			continue;

		m_aPortal[i].m_wRemainTime--;

		if (m_aPortal[i].m_wRemainTime	==	0)
		{
			removePortal(i);

			_lpField->addPacketRemovePortal(i);
		}
	}
}

int
CPortalManager::removeActor(int _iSerial)
{
	for (int i=0;i<dMAX_PORTAL_IN_FIELD;i++)
		if (m_aPortal[i].m_wOwner	==	_iSerial)
		{
			removePortal(i);

			return	i;
		}

	return	0xffff;
}

BOOL
CPortalManager::removePortal(int _iSerial)
{
	if	(m_iPortalCount	<=	0)
		return	FALSE;

	if (m_aPortal[_iSerial].m_wOwner	==	0xffff)
		return	FALSE;

	m_aPortal[_iSerial].m_wOwner	=	0xffff;

	m_iPortalCount--;

	return	TRUE;
}

