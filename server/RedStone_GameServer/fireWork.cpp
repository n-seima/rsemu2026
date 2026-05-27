#include "fireWork.h"
#include "cGame.h"
#include "cField.h"
#include "cACTOR.h"

cFireWorkManager	g_fireWorkManager;

void
cFireWorkItem::copy(cFireWorkItem *_lpBanner)
{
	memcpy(this,_lpBanner,sizeof(cFireWorkItem));
}

void
cFireWorkItem::reset()
{
	m_wSerial	=	0xffff;
}

BOOL
cFireWorkItem::fireFireWork(int _iX,int _iY,int _iDestX,int _iDestY)
{
	if	(m_iOwnerZoneSerial		!=	0xffffffff)
		if	(m_wRemainFireWorkTime	==	0)
			m_wRemainFireWorkTime	=	m_wFireWorkUpkeepTime;

	int	iOwnerSerial	=	0xffff;
		
	if	(m_iOwnerZoneSerial		!=	0xffffffff)
	{
		cACTOR	*lpOwner	=	g_game.getActor(m_iOwnerZoneSerial);

		if	(lpOwner	&&	lpOwner->m_dwNameHashCode	!=	m_dwOwnerNameHashCode)
			lpOwner		=	NULL;

		if	(lpOwner)
			iOwnerSerial	=	lpOwner->m_wSerialInField;
	}
		
	cFIELD	*lpField	=	g_game.getFieldByUniqueSerial(m_wUniqueFieldSerial);

	lpField->addSendEtcWork(iOwnerSerial,eEW_FIRE_FIRE_WORK,_iX,_iY,_iDestX,_iDestY,m_wFireWorkType);

	m_wLastPosX			=	_iX;
	m_wLastPosY			=	_iY;

	if	(m_wRemainFireWorkCount)
		m_wRemainFireWorkCount--;

	if	(m_wRemainFireWorkCount	==	0)
		return	TRUE;
	
	return	FALSE;
}

BOOL
cFireWorkItem::updateFireWork()
{
	if	(m_wRemainFireWorkCount	==	0)
		return	TRUE;
	
	cACTOR	*lpOwner=	NULL;
		
	if	(m_iOwnerZoneSerial		!=	0xffffffff)
	{
		lpOwner	=	g_game.getActor(m_iOwnerZoneSerial);

		if	(lpOwner	&&	lpOwner->m_dwNameHashCode	!=	m_dwOwnerNameHashCode)
			lpOwner		=	NULL;

		if	(!lpOwner)
			m_iOwnerZoneSerial	=	0xffffffff;
	}

	if	(m_wRemainFireWorkTime)
	{
		m_wRemainFireWorkTime--;

		if	(m_wRemainFireWorkTime	==	0)
		{
			m_iOwnerZoneSerial	=	0xffffffff;

			if	(lpOwner)
				lpOwner->sendEtcWork(eEW_END_FIRE_WORK_TIME);

			return	FALSE;
		}
	}

	if	(m_iOwnerZoneSerial	==	0xffffffff)
	{
		int	iPosX	=	random(400)-200+m_wLastPosX;
		int	iPosY	=	random(400)-200+m_wLastPosY;

		if	(fireFireWork(m_wLastPosX,m_wLastPosY,iPosX,iPosY))
		{
			if	(lpOwner)
				lpOwner->sendEtcWork(eEW_END_FIRE_WORK_TIME);

			return	TRUE;
		}
	}

	return	FALSE;
}

void
cFireWorkManager::reset()
{
	m_den.reset();
}

cFireWorkItem*
cFireWorkManager::get(int _iIndex)
{
	return	m_den.get(_iIndex);
}

cFireWorkItem*
cFireWorkManager::getByActor(int _iZoneSerial)
{
	int	i;
	int	iOldCount	=	m_den.getCount(),iCount=0;

	for(i=0;i < m_den.getMaxCount() && iCount < iOldCount;i++)
	{
		cFireWorkItem	*lpFireWork	=	m_den.get(i);

		if	(!lpFireWork)
			continue;

		iCount++;

		if	(lpFireWork->m_iOwnerZoneSerial	==	_iZoneSerial)
			return	lpFireWork;
	}

	return	NULL;
}

int
cFireWorkManager::add(cACTOR *_lpOwner,int _iKind,int _iUpkeepTime)
{
	cFireWorkItem	fireWork;

	fireWork.m_wLastPosX			=	_lpOwner->m_pos.x;
	fireWork.m_wLastPosY			=	_lpOwner->m_pos.y;
	fireWork.m_iOwnerZoneSerial		=	_lpOwner->m_iZoneSerial;
	fireWork.m_dwOwnerNameHashCode	=	_lpOwner->m_dwNameHashCode;

	fireWork.m_wFireWorkType		=	_iKind;
	fireWork.m_wFireWorkUpkeepTime	=	_iUpkeepTime;
	fireWork.m_wRemainFireWorkTime	=	0;
	fireWork.m_wRemainFireWorkCount	=	_iUpkeepTime;
	fireWork.m_wUniqueFieldSerial	=	_lpOwner->m_wCurrentField;

	int	iSerial						=	m_den.add(&fireWork);

	if	(iSerial	==	0xffff)
		return	0xffff;

	_lpOwner->sendEtcWork(eEW_USE_FIRE_WORK,iSerial,_iKind,_iUpkeepTime);

	return	iSerial;
}

BOOL
cFireWorkManager::remove(int _iIndex)
{
	return	m_den.remove(_iIndex);
}

void
cFireWorkManager::operateOncePerSecond()
{
	int	i;
	int	iOldCount	=	m_den.getCount(),iCount=0;

	for(i=0;i < m_den.getMaxCount() && iCount < iOldCount;i++)
	{
		cFireWorkItem	*lpFireWork	=	m_den.get(i);

		if	(!lpFireWork)
			continue;

		iCount++;

		if	(lpFireWork->updateFireWork())
			remove(i);
	}
}

BOOL
cFireWorkManager::fireFireWork(cACTOR *_lpOwner,int _iSerial,int _iX,int _iY,int _iDestX,int _iDestY)
{
	cFireWorkItem	*lpFireWorkItem	=	get(_iSerial);

	if	(!lpFireWorkItem || lpFireWorkItem->m_iOwnerZoneSerial != _lpOwner->m_iZoneSerial || lpFireWorkItem->m_dwOwnerNameHashCode != _lpOwner->m_dwNameHashCode)
	{
		_lpOwner->sendEtcWork(eEW_END_FIRE_WORK_TIME);
		return	FALSE;
	}

	if	(lpFireWorkItem->fireFireWork(_iX,_iY,_iDestX,_iDestY))
		_lpOwner->sendEtcWork(eEW_END_FIRE_WORK_TIME);

	return	TRUE;
}