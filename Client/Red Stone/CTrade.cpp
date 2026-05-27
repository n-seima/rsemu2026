#include "CTrade.H"
#include "CActor.h"
#include "CHero.h"

//
//	트레이드 박스 초기화
void
CTradeBox::reset()
{
	memset(m_aItems,0xff,sizeof(m_aItems));
	m_iGold		=	0;
	m_wStatus	=	eTS_OPEN;
	m_wSerial	=	0xffff;
	m_strName[0]=	NULL;
}

//
//	초기화
void
CTradeManager::reset()
{
	m_myTradeBox.reset();
	m_mateTradeBox.reset();
	m_bIsActive			=	FALSE;
	m_wTradeBoxSerial	=	0xffff;
}

//
//	트레이드 시작
void
CTradeManager::beginTrade(CActor *_lpMate,int _iTradeBoxSerial)
{
	reset();

	m_mateTradeBox.m_wSerial	=	_lpMate->m_wSerial;
	strcpy(m_mateTradeBox.m_strName,_lpMate->m_strName);

	m_myTradeBox.m_wSerial		=	g_lpHero->m_wSerial;
	strcpy(m_myTradeBox.m_strName,g_lpHero->m_strName);

	m_bIsActive			=	TRUE;
	m_wTradeBoxSerial	=	_iTradeBoxSerial;

	strcpy(m_strLastestTradeUser,_lpMate->m_strName);
}

//
//	트레이드 상태를 초기화 한다.(전부 오픈 상태로..)
void
CTradeManager::resetTradeStatus()
{
	m_mateTradeBox.m_wStatus=	eTS_OPEN;
	m_myTradeBox.m_wStatus	=	eTS_OPEN;
}

//
//	상대방 아이템 더하기
BOOL
CTradeManager::addMateItem(cItem *_lpItem)
{
	resetTradeStatus();

	for (int i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		if (m_mateTradeBox.m_aItems[i].m_wBaseItem	!=	0xffff)	continue;

		memcpy(&m_mateTradeBox.m_aItems[i],_lpItem,sizeof(cItem));

		return	TRUE;
	}

	return	FALSE;
}

//
//	여기 cItem 의 m_wBaseItem은 인벤토리에 슬롯을 가르킨다.
BOOL
CTradeManager::addMyItem(cItem *_lpItem)
{
	resetTradeStatus();

	for (int i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		if	(m_myTradeBox.m_aItems[i].m_wBaseItem	!=	0xffff)
			continue;

		memcpy(&m_myTradeBox.m_aItems[i],_lpItem,sizeof(cItem));

		return	TRUE;
	}

	return	FALSE;
}

//
//	거래 아이템 데이터 변경
BOOL
CTradeManager::changeTradeItemDataByMe(int _iTradeSlot,cItem *_lpItem)
{
	resetTradeStatus();

	if (m_myTradeBox.m_aItems[_iTradeSlot].m_wBaseItem	!=	_lpItem->m_wBaseItem)	return	FALSE;

	m_myTradeBox.m_aItems[_iTradeSlot].m_bCount	=	_lpItem->m_bCount;

	return	TRUE;
}

//
//	거래 아이템 데이터 변경
BOOL
CTradeManager::changeTradeItemDataByMate(int _iTradeSlot,cItem *_lpItem)
{
	resetTradeStatus();

	if (m_mateTradeBox.m_aItems[_iTradeSlot].m_wBaseItem	!=	_lpItem->m_wBaseItem)	return	FALSE;

	m_mateTradeBox.m_aItems[_iTradeSlot].m_bCount	=	_lpItem->m_bCount;

	return	TRUE;
}

//
//	상대방 아이템 빼기
BOOL
CTradeManager::removeItem(int _iTrader,int _iTradeBoxSlot)
{
	resetTradeStatus();

	if	(_iTradeBoxSlot	<	0	||	_iTradeBoxSlot	>=	dTRADE_ITEM_COUNT)
		return	FALSE;

	if	(_iTrader	==	m_myTradeBox.m_wSerial	)
	{
		int	iMoveItemCount	=	dTRADE_ITEM_COUNT-_iTradeBoxSlot-1;

		if	(iMoveItemCount>0)
			memcpy(&m_myTradeBox.m_aItems[_iTradeBoxSlot],&m_myTradeBox.m_aItems[_iTradeBoxSlot+1],sizeof(cItem)*iMoveItemCount);

		m_myTradeBox.m_aItems[dTRADE_ITEM_COUNT-1].m_wBaseItem	=	0xffff;

		return	TRUE;
	}

	if (_iTrader	==	m_mateTradeBox.m_wSerial)
	{
		int	iMoveItemCount	=	dTRADE_ITEM_COUNT-_iTradeBoxSlot-1;

		if	(iMoveItemCount>0)
			memcpy(&m_mateTradeBox.m_aItems[_iTradeBoxSlot],&m_mateTradeBox.m_aItems[_iTradeBoxSlot+1],sizeof(cItem)*iMoveItemCount);

		m_mateTradeBox.m_aItems[dTRADE_ITEM_COUNT-1].m_wBaseItem	=	0xffff;

		return	TRUE;
	}

	return	FALSE;
}

//
//	상대방이 건 돈 설정
BOOL
CTradeManager::setGold(int _iTrader,int _iGold)
{
	resetTradeStatus();

	if (_iTrader	==	m_myTradeBox.m_wSerial	)
		m_myTradeBox.m_iGold	=	_iGold;
	else
	if (_iTrader	==	m_mateTradeBox.m_wSerial)
		m_mateTradeBox.m_iGold	=	_iGold;
	else
		return	FALSE;

	return	TRUE;
}

//
//	거래준비 됨
BOOL
CTradeManager::readyTrade(int _iTrader)
{
	if	(_iTrader	==	m_myTradeBox.m_wSerial	)
		m_myTradeBox.m_wStatus		=	eTS_READY;
	else
	if	(_iTrader	==	m_mateTradeBox.m_wSerial)
		m_mateTradeBox.m_wStatus	=	eTS_READY;
	else
		return	FALSE;

	return	TRUE;
}

//
//	거래승인
BOOL
CTradeManager::permitTrade(int _iTrader)
{
	if	(_iTrader	==	m_myTradeBox.m_wSerial	)
		m_myTradeBox.m_wStatus		=	eTS_PERMIT;
	else
	if	(_iTrader	==	m_mateTradeBox.m_wSerial)
		m_mateTradeBox.m_wStatus	=	eTS_PERMIT;
	else
		return	FALSE;

	return	TRUE;
}

//
//	거래중인 아이템이냐?
BOOL
CTradeManager::isTradeItem(int _iSlot)
{
	if (!m_bIsActive)	return	FALSE;

	for (int i=0;i<dTRADE_ITEM_COUNT;i++)
		if (m_myTradeBox.m_aItems[i].m_wBaseItem==	_iSlot)
			return	TRUE;

	return	FALSE;
}

//
//	거래중인 아이템 리턴
BOOL
CTradeManager::getTradeItem(int _iSlot,cItem *_lpItem)
{
	if (!m_bIsActive)	return	FALSE;

	for (int i=0;i<dTRADE_ITEM_COUNT;i++)
		if (m_myTradeBox.m_aItems[i].m_wBaseItem	==	_iSlot)
		{
			cItem	*lpItem	=	g_hero.getInventoryItem(m_myTradeBox.m_aItems[i].m_wBaseItem,TRUE);

			if (!lpItem)	return	FALSE;

			_lpItem->copy(lpItem);
			_lpItem->m_bCount	=	m_myTradeBox.m_aItems[i].m_bCount;

			return	TRUE;
		}

	return	FALSE;
}

//
//	거래 아이템 슬롯?
int
CTradeManager::getTradeItemSlot(int _iSlot)
{
	if (!m_bIsActive)	return	0xffff;

	for (int i=0;i<dTRADE_ITEM_COUNT;i++)
		if (m_myTradeBox.m_aItems[i].m_wBaseItem	==	_iSlot)
			return	i;

	return	0xffff;
}

//
//	거래 아이템 슬롯?
int
CTradeManager::getCheckSum()
{
	int	iSum	=	m_myTradeBox.m_iGold+m_mateTradeBox.m_iGold,i;

	for (i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		int	iInventorySlot	=	m_myTradeBox.m_aItems[i].m_wBaseItem;
		if (iInventorySlot	==	0xffff)	break;

		cItem	*lpItem		=	g_hero.getInventoryItem(iInventorySlot,TRUE);
		cItem	item;

		item.copy(lpItem);
		item.m_bCount		=	m_myTradeBox.m_aItems[i].m_bCount;
		iSum				+=	item.getCheckSum();
	}

	for (i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		cItem	*lpItem		=	&m_mateTradeBox.m_aItems[i];
		if (lpItem->m_wBaseItem	==	0xffff)	break;

		iSum				+=	lpItem->getCheckSum();
	}

	return	iSum;
}