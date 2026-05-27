#include <time.h>
#include "CTrade.h"
#include "CField.h"
#include "CGame.h"
#include "cPACKET_LOGSERVER.h"
#include "packetManager.H"

//
//	트레이드 상대를 얻어온다.
cACTOR*
CTradeBox::getTradeMate(cACTOR	*_lpAsker)
{
	if (m_wSerial	==	0xffff)
		return	NULL;

	if (!_lpAsker)
		return	NULL;

	int	iMateSerial	=	0xffff;

	if (_lpAsker->m_wSerialInField	==	m_aTraderSerial[0])
		iMateSerial	=	m_aTraderSerial[1];

	if (_lpAsker->m_wSerialInField	==	m_aTraderSerial[1])
		iMateSerial	=	m_aTraderSerial[0];

	if (iMateSerial	==	0xffff)
		return	NULL;

	return	_lpAsker->m_lpField->getTestedActor(iMateSerial);
}

//
//	초기화
void
CTradeBox::reset()
{
	if (m_lpField)
		for (int i=0;i<2;i++)
		{
			if (m_aTraderSerial[i]	!=	0xffff	&&	m_lpField)
			{
				cACTOR	*lpTrader	=	m_lpField->getActor(m_aTraderSerial[i]);

				if (lpTrader)
					lpTrader->m_wTradeBoxSerial	=	0xffff;
			}
		}

	m_wSerial			=	0xffff;
	m_aTradeStatus[0]	=	eTS_OPEN;
	m_aTradeStatus[1]	=	eTS_OPEN;
	m_iaTradeGold[0]	=	0;
	m_iaTradeGold[1]	=	0;
	m_lpField			=	NULL;
	m_aTraderSerial[0]	=	0xffff;
	m_aTraderSerial[1]	=	0xffff;

	memset(m_aTradeItem,0xff,sizeof(m_aTradeItem));
}

//
//	초기화?
void
CTradeBox::init(int _iSerial,cFIELD *_lpField,char *_strName1,char *_strName2,int _iTrader1,int _iTrader2)
{
	reset();

	strcpy(m_strName[0],_strName1);
	strcpy(m_strName[1],_strName2);

	m_aTraderSerial[0]	=	_iTrader1;
	m_aTraderSerial[1]	=	_iTrader2;

	m_lpField			=	_lpField;
	m_wSerial			=	_iSerial;
}

//
//	적절한 상태인지 확인한다.
BOOL
CTradeBox::check()
{
	for (int i=0;i<2;i++)
	{
		cACTOR	*lpTrader	=	m_lpField->getActor(m_aTraderSerial[i]);

		if (m_lpField->checkActor(lpTrader)==FALSE		)
			return	FALSE;
		if (lpTrader->isDeath()							)
			return	FALSE;
		if (lpTrader->m_wTradeBoxSerial	!=	m_wSerial	)
			return	FALSE;
		if (STRCMP(m_strName[i],lpTrader->m_strName)!=0)
			return	FALSE;
		if	(lpTrader->m_iGold	<	m_iaTradeGold[i]	)
			return	FALSE;

		for (int iSlot=0;iSlot<dTRADE_ITEM_COUNT;iSlot++)
		{
			int	iInventorySlot	=	m_aTradeItem[i][iSlot].m_wSlot;

			if (iInventorySlot	==	0xffff)
				continue;

			cItem	*lpItem		=	lpTrader->getInventoryItem(iInventorySlot);

			if	(!lpItem)
				return	FALSE;

			if	(lpItem->isExceptionItem()	==	FALSE)
				if	(!lpItem->isExceptionItem() && lpItem->m_bCount	<	m_aTradeItem[i][iSlot].m_wCount)
					return	FALSE;
		}
	}

	return	TRUE;
}

//
//	이름으로 트레이더 인덱스 구함
int
CTradeBox::getTraderIndex(char *_lpstrName)
{
	int	iIndex	=	0xffff;

	if (STRCMP(m_strName[0],_lpstrName)	==	0)
		iIndex	=	0;
	if (STRCMP(m_strName[1],_lpstrName)	==	0)
		iIndex	=	1;

	return	iIndex;
}

//
//	액터로 트레이더 인덱스 구함
int
CTradeBox::getTraderIndex(cACTOR *_lpTrader)
{
	int	iIndex	=	0xffff;

	if (!_lpTrader)	return	0xffff;

	if (STRCMP(m_strName[0],_lpTrader->m_strName)	==	0)
		iIndex	=	0;
	if (STRCMP(m_strName[1],_lpTrader->m_strName)	==	0)
		iIndex	=	1;

	if (_lpTrader->m_wSerialInField	!=	m_aTraderSerial[iIndex])	return	0xffff;

	return	iIndex;
}

//
//	이름으로 트레이더 인덱스 구함
int
CTradeBox::getTradeItemSlotIndex(int _iTraderIndex,int _iItemSlot)
{
	for (int i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		if (m_aTradeItem[_iTraderIndex][i].m_wSlot	==	0xffff	)	continue;
		if (m_aTradeItem[_iTraderIndex][i].m_wSlot	==	_iItemSlot)	return	i;
	}

	return	0xffff;
}

cACTOR*
CTradeBox::getMate(cACTOR *_lpActor)
{
	for (int i=0;i<2;i++)
	{
		cACTOR	*lpTrader	=	m_lpField->getActor(m_aTraderSerial[i]);

		if (lpTrader->m_wSerialInField	!=	_lpActor->m_wSerialInField)
			return	lpTrader;
	}

	return	NULL;
}

//
//	트레이드 아이템 추가
int
CTradeBox::addTradeItem(cACTOR *_lpTrader,int _iItemSlot,int _iCount)
{
	m_aTradeStatus[0]	=	eTS_OPEN;
	m_aTradeStatus[1]	=	eTS_OPEN;

	int	iIndex	=	getTraderIndex(_lpTrader);
	int	i;

	if	(iIndex	==	0xffff)
		return	eRATI_INCORRECT_TRADER;

	cItem	*lpItem	=	_lpTrader->getInventoryItem(_iItemSlot);

	if	(!lpItem											)
		return	eRATI_VALID_ITEM;

	if	(_iCount	<=	0)
		return	eRATI_INCORRECT_TRADER_ITEM_COUNT;
	if	(lpItem->isExceptionItem())
	{
		if	(_iCount	!=	1)
			return	eRATI_INCORRECT_TRADER_ITEM_COUNT;
	}
	else
	if	(lpItem->isExtraItem()	==	FALSE)
	{
		if	(lpItem->m_bCount	<	_iCount	)
			return	eRATI_INCORRECT_TRADER_ITEM_COUNT;
	}

	if	(getTradeItemSlotIndex(iIndex,_iItemSlot)!=	0xffff	)
		return	eRATI_ALREADY_ADDED_ITEM;

	if	(lpItem->isCanNotTradeItem())
		return	eRATI_CAN_NOT_TRADE_ITEM;

	if	(lpItem->isReversionItem())
		return	eRATI_CAN_NOT_TRADE_ITEM_IS_REVERSION_ITEM;

	if	(lpItem->isPresentCoupleRing())
	{
		for (i=0;i<dTRADE_ITEM_COUNT;i++)
		{
			if	(m_aTradeItem[iIndex][i].m_wSlot	==	0xffff)
				continue;

			if	(_lpTrader->m_aItems[m_aTradeItem[iIndex][i].m_wSlot].isPresentCoupleRing())
				return	eRATI_COUPLE_RING_TRADE_ONLY_ONE_AT_ONE_TIME;
		}
		for (i=0;i<dTRADE_ITEM_COUNT;i++)
		{
			if	(m_aTradeItem[1-iIndex][i].m_wSlot	==	0xffff)
				continue;

			if	(_lpTrader->m_aItems[m_aTradeItem[1-iIndex][i].m_wSlot].isPresentCoupleRing())
				return	eRATI_COUPLE_RING_TRADE_ONLY_ONE_AT_ONE_TIME;
		}
	}

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(lpBasicItem)
	{
		cACTOR	*lpMate	=	getMate(_lpTrader);

		if (lpMate)
		{
			if	(lpBasicItem->m_attr.isCanNotOwnSameItem)
				if (lpMate->getItemByBasicItem(lpItem->m_wBaseItem))
					return	eRATI_CAN_NOT_OWN_SAME_ITEM;

			if (lpBasicItem->m_attr.isBadge)
				if (lpMate->getBadgeCount()	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
					return	eRATI_TOO_MANY_BADGE_ITEM;

		}
	}

	for (i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		if (m_aTradeItem[iIndex][i].m_wSlot	==	0xffff)
		{
			m_aTradeItem[iIndex][i].m_wSlot	=	_iItemSlot;
			m_aTradeItem[iIndex][i].m_wCount=	_iCount;

			return	eRATI_OK;
		}
	}


	return	eRATI_TRADE_BOX_FULL;
}

//
//	거래 아이템 데이터 수정
int
CTradeBox::changeTradeItemData(cACTOR *_lpTrader,int _iTradeSlot,int _iItemSlot,int _iCount)
{
	m_aTradeStatus[0]	=	eTS_OPEN;
	m_aTradeStatus[1]	=	eTS_OPEN;
	
	int	iIndex	=	getTraderIndex(_lpTrader);

	if (iIndex	==	0xffff)	return	eRATI_INCORRECT_TRADER;

	cItem	*lpItem	=	_lpTrader->getInventoryItem(_iItemSlot);

	if	(!lpItem												)
		return	eRATI_VALID_ITEM;

	if	(lpItem->isExceptionItem()	==	FALSE)
		if	(lpItem->m_bCount	<	_iCount						)
			return	eRATI_INCORRECT_TRADER_ITEM_COUNT;

	int	iTradeSlot	=	getTradeItemSlotIndex(iIndex,_iItemSlot);

	if (iTradeSlot==0xffff	||	iTradeSlot	!=	_iTradeSlot)	return	eRATI_INCORRECT_TRADE_SLOT;

	m_aTradeItem[iIndex][iTradeSlot].m_wSlot	=	_iItemSlot;
	m_aTradeItem[iIndex][iTradeSlot].m_wCount	=	_iCount;

	return	eRATI_OK;
}

//
//	트레이드 아이템 제거
int
CTradeBox::removeTradeItem(cACTOR *_lpTrader,int _iTradeBoxSlot)
{
	m_aTradeStatus[0]	=	eTS_OPEN;
	m_aTradeStatus[1]	=	eTS_OPEN;

	int	iIndex	=	getTraderIndex(_lpTrader);

	if (iIndex	==	0xffff)	return	eRRTI_INCORRECT_TRADER;

	if (_iTradeBoxSlot	<	0	||	_iTradeBoxSlot	>=	dTRADE_ITEM_COUNT)	return	eRRTI_INCORRECT_TRADE_BOX_SLOT;
	if (m_aTradeItem[iIndex][_iTradeBoxSlot].m_wSlot==	0xffff			)	return	eRRTI_VALID_TRADE_BOX_SLOT;

	if (_iTradeBoxSlot	<	dTRADE_ITEM_COUNT-1)
		memcpy(&m_aTradeItem[iIndex][_iTradeBoxSlot],&m_aTradeItem[iIndex][_iTradeBoxSlot+1],sizeof(CTradeItem)*(dTRADE_ITEM_COUNT-_iTradeBoxSlot-1));

	m_aTradeItem[iIndex][dTRADE_ITEM_COUNT-1].m_wSlot	=	0xffff;

	return	eRRTI_OK;
}

//
//	골드 설정
BOOL
CTradeBox::setTradeGold(cACTOR *_lpTrader,int _iGold)
{
	m_aTradeStatus[0]	=	eTS_OPEN;
	m_aTradeStatus[1]	=	eTS_OPEN;

	int		iIndex		=	getTraderIndex(_lpTrader);

	cACTOR	*lpMate		=	getTradeMate(_lpTrader);

	if	(iIndex	==	0xffff	||	lpMate	==	NULL)
		return	eRSTG_INCORRECT_TRADER;

	if	(_lpTrader->m_iGold	<	_iGold)
		return	eRSTG_LOW_GOLD;

	LONGLONG	llGold		=	(LONGLONG)lpMate->m_iGold	+	(LONGLONG)_iGold;

	if	(llGold				>	dMAX_OWN_GOLD)
		return	eRSTG_TARGET_GOLD_FULL;

	m_iaTradeGold[iIndex]	=	_iGold;

	return	eRSTG_OK;
}

//
//	트래이드 준비
BOOL
CTradeBox::readyTrade(cACTOR *_lpTrader)
{
	int	iIndex	=	getTraderIndex(_lpTrader);

	if (iIndex	==	0xffff)	return	FALSE;

	m_aTradeStatus[iIndex]	=	eTS_READY;

	return	TRUE;
}

//
//	트래이드 허락
BOOL
CTradeBox::permitTrade(cACTOR *_lpTrader)
{
	int	iIndex	=	getTraderIndex(_lpTrader);

	if (iIndex	==	0xffff)
		return	FALSE;
	if (m_aTradeStatus[0]	<	eTS_READY || m_aTradeStatus[1]	<	eTS_READY)
		return FALSE;

	m_aTradeStatus[iIndex]	=	eTS_PERMIT;

	return	TRUE;
}

//
//	트래이드 준비 완료!!??
BOOL
CTradeBox::isReadyToTrade()
{
	if (m_aTradeStatus[0]==eTS_PERMIT && m_aTradeStatus[1]==eTS_PERMIT)
		return	TRUE;

	return	FALSE;
}

//
//	트래이드 패킷 정보 얻기
BOOL
CTradeBox::getTradePacketInfo(cACTOR *_lpTrader,SG_TRADE *_lpPacket)
{
	int	iIndex	=	getTraderIndex(_lpTrader),i;

	if	(iIndex	==	0xffff)
		return	FALSE;
	if	(m_aTradeStatus[0]!=eTS_PERMIT || m_aTradeStatus[1]!=eTS_PERMIT)
		return	FALSE;	//	둘다 허락 상태

	cACTOR	*lpMate				=	getTradeMate(_lpTrader);

	if	(!lpMate)
		return	FALSE;

	CTradeItem	*lpGiveItem		=	&m_aTradeItem[iIndex][0];
	CTradeItem	*lpReceiveItem	=	&m_aTradeItem[1-iIndex][0];

	_lpPacket->base.set(sizeof(SG_TRADE),dSG_TRADE);
	_lpPacket->dwGiveGold		=	m_iaTradeGold[iIndex];
	_lpPacket->dwReceiveGold	=	m_iaTradeGold[1-iIndex];

	memset(_lpPacket->aGiveItems,0xff,sizeof(_lpPacket->aGiveItems));
	memset(_lpPacket->aReceiveItems,0xff,sizeof(_lpPacket->aReceiveItems));

	_lpPacket->wReceiveItemCount=	0;

	for (i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		int		iItemSlot	=	lpReceiveItem[i].m_wSlot;
		cItem	*lpItem		=	lpMate->getInventoryItem(iItemSlot);

		if	(!lpItem)
			break;

		memcpy(&_lpPacket->aReceiveItems[i],lpItem,sizeof(cItem));

		if	(lpItem->isExceptionItem()	==	FALSE)
			_lpPacket->aReceiveItems[i].m_bCount	=	(BYTE)lpReceiveItem[i].m_wCount;
		_lpPacket->wReceiveItemCount++;
	}

	for (i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		int		iItemSlot	=	lpGiveItem[i].m_wSlot;
		cItem	*lpItem		=	_lpTrader->getInventoryItem(iItemSlot);

		if	(!lpItem)
			break;

		_lpPacket->aGiveItems[i].m_wItemSlot	=	iItemSlot;
		_lpPacket->aGiveItems[i].m_wTradeCount	=	lpGiveItem[i].m_wCount;
	}

	_lpPacket->base.wSize	=	sizeof(SG_TRADE)-sizeof(_lpPacket->aReceiveItems)+sizeof(cItem)*_lpPacket->wReceiveItemCount;

	return	TRUE;
}

//
//	체크섬 구하기
int
CTradeBox::getCheckSum(cACTOR *_lpTrader)
{
	CTradeItem	*lpTradeItems;
	cACTOR		*lpMate	=	getTradeMate(_lpTrader);
	int			iSum	=	0;

	if (!lpMate)	return	-1;
	int	iIndex	=	getTraderIndex(_lpTrader),i;
	if (iIndex	==	0xffff)	return	FALSE;

	lpTradeItems=	&m_aTradeItem[iIndex][0];

	cACTOR	*lpActor=	_lpTrader;

	for (int iLoop=0;iLoop<2;iLoop++)
	{
		iSum	+=	m_iaTradeGold[iIndex];

		for (i=0;i<dTRADE_ITEM_COUNT;i++)
		{
			int	iItemSlot	=	lpTradeItems[i].m_wSlot;

			if	(iItemSlot	==	0xffff)
			{
				i	=	dTRADE_ITEM_COUNT;
				continue;
			}

			cItem	*lpItem	=	lpActor->getInventoryItem(iItemSlot);

			if (!lpItem)	continue;

			cItem	item;
			item.copy(lpItem);

			if	(lpItem->isExceptionItem()	==	FALSE)
				item.m_bCount	=	(BYTE)lpTradeItems[i].m_wCount;

			iSum			+=	item.getCheckSum();
		}

		iIndex		=	1-iIndex;
		lpActor		=	lpMate;
		lpTradeItems=	&m_aTradeItem[iIndex][0];
	}

	return	iSum;
}

//
//	트레이드할 수 있는 상태냐?
int
CTradeBox::isTradeAble(cACTOR *_lpTrader)
{
	cACTOR		*lpMate	=	getTradeMate(_lpTrader);
	CTradeItem	*lpTradeItems;

	if	(!lpMate)
		return	eRT_INCORRECT_TRADER;

	int	iIndex			=	getTraderIndex(_lpTrader),i;
	int	iTradeItemCount	=	0;

	lpTradeItems	=	&m_aTradeItem[iIndex][0];

	for (i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		int		iItemSlot	=	lpTradeItems[i].m_wSlot;

		if	(iItemSlot	==	0xffff)
			break;

		iTradeItemCount++;
	}

	if (lpMate->getRemainInventorySlotCount()	<	iTradeItemCount)
		return	eRT_LOW_SLOT;

	lpTradeItems	=	&m_aTradeItem[1-iIndex][0];
	iTradeItemCount	=	0;

	for (i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		int		iItemSlot	=	lpTradeItems[i].m_wSlot;

		if	(iItemSlot	==	0xffff)
			break;

		iTradeItemCount++;
	}

	if (_lpTrader->getRemainInventorySlotCount()<	iTradeItemCount)
		return	eRT_LOW_SLOT;

	return	eRT_OK;
}

//
//	트레이드 -_-
BOOL
CTradeBox::trade(cACTOR *_lpTrader)
{
	cACTOR		*lpMate	=	getTradeMate(_lpTrader);
	CTradeItem	*lpTradeItems;

	if	(!lpMate)
		return	FALSE;

	int		iIndex		=	getTraderIndex(_lpTrader),i;

	char	strTime[128];
	char	strDate[128];

	_strtime(strTime);
	_strdate(strDate);

	BOOL	bIsExistCoupleRing1	=	FALSE;
	BOOL	bIsExistCoupleRing2	=	FALSE;

//	복 사한 아이템 확인
	{
		lpTradeItems		=	&m_aTradeItem[iIndex][0];

		for (i=0;i<dTRADE_ITEM_COUNT;i++)
		{
			int	iItemSlot	=	lpTradeItems[i].m_wSlot;

			if	(iItemSlot	==	0xffff)
				break;

			cItem		*lpItem		=	_lpTrader->getInventoryItem(iItemSlot);
			cBasicItem	*lpBasic	=	lpItem->getBasicItem();

			if	(!lpItem)
				continue;

			if	(lpMate->isExistItem(lpItem,FALSE))
			{
				_lpTrader->sendDoubleItemLog(lpItem,eDIL_TRADE);
				lpItem->reset();

				return	FALSE;
			}

			if	(lpItem->isPresentCoupleRing())
			{
				lpItem->changeToCoupleRing();

				bIsExistCoupleRing1					=	TRUE;
			}
		}

		lpTradeItems		=	&m_aTradeItem[1-iIndex][0];

		for (i=0;i<dTRADE_ITEM_COUNT;i++)
		{
			int	iItemSlot	=	lpTradeItems[i].m_wSlot;

			if	(iItemSlot	==	0xffff)
				break;

			cItem		*lpItem		=	lpMate->getInventoryItem(iItemSlot);
			cBasicItem	*lpBasic	=	lpItem->getBasicItem();

			if (!lpItem)
				continue;

			if (_lpTrader->isExistItem(lpItem,FALSE))
			{
				lpMate->sendDoubleItemLog(lpItem,eDIL_TRADE);
				lpItem->reset();

				return	FALSE;
			}

			if	(lpItem->isPresentCoupleRing())
			{
				lpItem->changeToCoupleRing();

				bIsExistCoupleRing2					=	TRUE;
			}
		}
	}

	CClient		*lpTraderClient	=	_lpTrader->getClient();
	CClient		*lpMateClient	=	lpMate->getClient();

	BOOL		bIsByOperator	=	FALSE;

//	로그 코드
	LOG_ITEM_TRADE		logTrade;

	logTrade.wTrader1ItemCount	=	0;
	logTrade.bf15Trader2ItemCount=	0;
	logTrade.bf1IsOperator		=	FALSE;

	if	(_lpTrader->isTester()+lpMate->isTester())
		logTrade.bf1IsOperator	=	TRUE;

	{
		time_t tTime;

		struct tm *tmTime;

		tTime	=	time(NULL);
		tmTime	=	localtime(&tTime);
		logTrade.base.set(sizeof(LOG_ITEM_TRADE),dLOG_ITEM_TRADE);

		logTrade.logTime.yy		= tmTime->tm_year%100;
		logTrade.logTime.mm		= tmTime->tm_mon+1;
		logTrade.logTime.dd		= tmTime->tm_mday;
		logTrade.logTime.hour	= tmTime->tm_hour;
		logTrade.logTime.min	= tmTime->tm_min;
	}

//	_lpTrader->lpMate 아이템 전송?
	{
		lpTradeItems		=	&m_aTradeItem[iIndex][0];

		_lpTrader->m_iGold	-=	m_iaTradeGold[iIndex];
		lpMate->m_iGold		+=	m_iaTradeGold[iIndex];

		_lpTrader->m_llTradeSpentGold	-=	m_iaTradeGold[iIndex];
		lpMate->m_llTradeGettingGold	+=	m_iaTradeGold[iIndex];

		strcpy(logTrade.strSenderId		,_lpTrader->m_strId);
		strcpy(logTrade.strSenderName	,_lpTrader->m_strName);
		strcpy(logTrade.strSenderIP		,lpTraderClient->ip);

		logTrade.wFieldUniqueIndexSender=	_lpTrader->m_lpField->m_iUniqueSerial;
		logTrade.dwGoldSender			=	m_iaTradeGold[iIndex];

		for (i=0;i<dTRADE_ITEM_COUNT;i++)
		{
			int	iItemSlot	=	lpTradeItems[i].m_wSlot;

			if	(iItemSlot	==	0xffff)
				break;

			cItem	*lpEmptySlot	=	lpMate->getValidInventorySlot();
			cItem	*lpItem			=	_lpTrader->getInventoryItem(iItemSlot);

			if	(!lpItem)
				continue;

			if	(lpItem->m_wBaseItem == dITEM_INDEX_GOLD_BAR)
			{
				LONGLONG	llTradeGold	=	lpTradeItems[i].m_wCount;
				llTradeGold				*=	100000000;

				lpMate->m_llTradeGettingGold	+=	llTradeGold;
				_lpTrader->m_llTradeSpentGold	-=	llTradeGold;
			}

			cBasicItem	*lpBasic	=	lpItem->getBasicItem();

			memcpy(&logTrade.cItem[logTrade.wTrader1ItemCount++],lpItem,sizeof(cItem));

			lpEmptySlot->copy(lpItem);

			if	(lpItem->isCoupleRing())
			{
				logTrade.cItem[logTrade.wTrader1ItemCount++].m_year	=	lpItem->m_year;
				logTrade.cItem[logTrade.wTrader1ItemCount++].m_month	=	lpItem->m_month;
				logTrade.cItem[logTrade.wTrader1ItemCount++].m_day	=	lpItem->m_day;
				logTrade.cItem[logTrade.wTrader1ItemCount++].m_hour	=	lpItem->m_hour;
				logTrade.cItem[logTrade.wTrader1ItemCount++].m_minute	=	lpItem->m_minute;
			}

			if	(lpItem->isExceptionItem()	==	FALSE)
				lpEmptySlot->m_bCount	=	(BYTE)lpTradeItems[i].m_wCount;

			lpItem->decreaseCount(lpTradeItems[i].m_wCount);
		}
	}

//	lpMate->_lpTrader 아이템 전송?
	{
		iIndex				=	1-iIndex;

		lpTradeItems		=	&m_aTradeItem[iIndex][0];
		lpMate->m_iGold		-=	m_iaTradeGold[iIndex];
		_lpTrader->m_iGold	+=	m_iaTradeGold[iIndex];

		lpMate->m_llTradeSpentGold		-=	m_iaTradeGold[iIndex];
		_lpTrader->m_llTradeGettingGold	+=	m_iaTradeGold[iIndex];


		strcpy(logTrade.strRecverId		,lpMate->m_strId);
		strcpy(logTrade.strRecverName	,lpMate->m_strName);
		strcpy(logTrade.strRecverIP		,lpMateClient->ip);
		logTrade.wFieldUniqueIndexRecver	=	lpMate->m_lpField->m_iUniqueSerial;
		logTrade.dwGoldRecver				=	m_iaTradeGold[iIndex];

		int	iCount	=	0;

		for (i=0;i<dTRADE_ITEM_COUNT;i++)
		{
			int	iItemSlot	=	lpTradeItems[i].m_wSlot;

			if	(iItemSlot	==	0xffff)
				break;

			cItem	*lpEmptySlot	=	_lpTrader->getValidInventorySlot();
			cItem	*lpItem			=	lpMate->getInventoryItem(iItemSlot);

			if	(!lpItem)
				continue;

			if	(lpItem->m_wBaseItem == dITEM_INDEX_GOLD_BAR)
			{
				LONGLONG	llTradeGold	=	lpTradeItems[i].m_wCount;
				llTradeGold				*=	100000000;

				_lpTrader->m_llTradeGettingGold	+=	llTradeGold;
				lpMate->m_llTradeSpentGold		-=	llTradeGold;
			}

			cBasicItem	*lpBasic	=	lpItem->getBasicItem();

			memcpy(&logTrade.cItem[logTrade.wTrader1ItemCount+logTrade.bf15Trader2ItemCount++],lpItem,sizeof(cItem));
			lpEmptySlot->copy(lpItem);

			if	(lpItem->isCoupleRing())
			{
				logTrade.cItem[logTrade.wTrader1ItemCount+logTrade.bf15Trader2ItemCount++].m_year	=	lpItem->m_year;
				logTrade.cItem[logTrade.wTrader1ItemCount+logTrade.bf15Trader2ItemCount++].m_month	=	lpItem->m_month;
				logTrade.cItem[logTrade.wTrader1ItemCount+logTrade.bf15Trader2ItemCount++].m_day	=	lpItem->m_day;
				logTrade.cItem[logTrade.wTrader1ItemCount+logTrade.bf15Trader2ItemCount++].m_hour	=	lpItem->m_hour;
				logTrade.cItem[logTrade.wTrader1ItemCount+logTrade.bf15Trader2ItemCount++].m_minute	=	lpItem->m_minute;

			}
			
			if	(lpItem->isExceptionItem()	==	FALSE)
				lpEmptySlot->m_bCount	=	(BYTE)lpTradeItems[i].m_wCount;

			lpItem->decreaseCount(lpTradeItems[i].m_wCount);
		}
	}

	int	iTradeItemCount	=	logTrade.wTrader1ItemCount+logTrade.bf15Trader2ItemCount;

	logTrade.base.wSize	=	sizeof(logTrade) - sizeof(logTrade.cItem) + sizeof(cItem)*iTradeItemCount;

	for (i=0;i<iTradeItemCount;i++)
		if	(logTrade.cItem[i].m_wBaseItem	>=	g_im.m_iItemCount)
			CLOG("itemLog","LOG_ITEM_TRADE");

	g_LogQue.addPacket((ALL_LOGMSG *)&logTrade);

	if	(bIsExistCoupleRing1)
	{
		cItem	*lpCoupleRing	=	_lpTrader->getNonameCoupleRing();

		if	(lpCoupleRing)
		{
			lpCoupleRing->changeToCoupleRing(lpMate->m_strName);

			_lpTrader->sendEtcWork(eEW_CHANGE_NONAME_COUPLE_RING_NAME_TO_LAST_TRADER);
		}
	}

	if	(bIsExistCoupleRing2)
	{
		cItem	*lpCoupleRing	=	lpMate->getNonameCoupleRing();

		if	(lpCoupleRing)
		{
			lpCoupleRing->changeToCoupleRing(_lpTrader->m_strName);
			lpMate->sendEtcWork(eEW_CHANGE_NONAME_COUPLE_RING_NAME_TO_LAST_TRADER);
		}
	}

	_lpTrader->buildPower();
	lpMate->buildPower();

	_lpTrader->m_wTradeBoxSerial=	0xffff;
	lpMate->m_wTradeBoxSerial	=	0xffff;

	_lpTrader->immediatelySendSaveDataToDBMemory(TRUE);
	lpMate->immediatelySendSaveDataToDBMemory(TRUE);

	return	TRUE;
}

//
//	거래 취소 메시지 전송
void
CTradeBox::sendTradeCancelMessage()
{
	if	(!m_lpField)
		return;

	SG_TRADE_MESSAGE	resultPacket;
	resultPacket.base.set(sizeof(resultPacket),dSG_TRADE_MESSAGE);

	resultPacket.wMessage	=	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS;

	for (int i=0;i<2;i++)
	{
		cACTOR	*lpTrader	=	m_lpField->getActor(m_aTraderSerial[i]);

		if (m_lpField->checkActor(lpTrader)==FALSE		)
			continue;

		g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	
	}
}


CTradeManager::CTradeManager()
{
	m_pTradeBox	=	NULL;
}

CTradeManager::~CTradeManager()
{
	pKILL(m_pTradeBox);
}

//
//	트레이드 초기화
void
CTradeManager::init(int _iMaxTraderCount)
{
	pKILL(m_pTradeBox);

	m_pTradeBox			=	new CTradeBox [_iMaxTraderCount];

	m_wMaxTradeBoxCount	=	_iMaxTraderCount;
	m_wRookieTradeBox		=	0;
	m_wTradeBoxCount		=	0;
}

//
//	트리이더 추가
int
CTradeManager::addTradeBox(cACTOR *_lpTrader1,cACTOR *_lpTrader2)
{
	if	(m_wTradeBoxCount	>=	m_wMaxTradeBoxCount)
		return	0xffff;

	int	iTraderSerial	=	m_wRookieTradeBox;

	m_pTradeBox[iTraderSerial].init(iTraderSerial,_lpTrader1->m_lpField,_lpTrader1->m_strName,_lpTrader2->m_strName,_lpTrader1->m_wSerialInField,_lpTrader2->m_wSerialInField);

	_lpTrader1->m_wTradeBoxSerial	=	iTraderSerial;
	_lpTrader2->m_wTradeBoxSerial	=	iTraderSerial;

	for (int i=m_wRookieTradeBox+1;i<m_wMaxTradeBoxCount;i++)
	{
		if (m_pTradeBox[i].m_wSerial	==	0xffff)
		{
			m_wRookieTradeBox	=	i;

			return	i;
		}
	}

	return	iTraderSerial;
}

//
//	트레이더 제거
BOOL
CTradeManager::removeTradeBox(int _iTraderIndex,char *_lpstrName)
{
	if (m_pTradeBox[_iTraderIndex].m_wSerial									==	0xffff)
		return	FALSE;
	if (_lpstrName	&&	m_pTradeBox[_iTraderIndex].getTraderIndex(_lpstrName)	==	0xffff)
		return	FALSE;

	m_pTradeBox[_iTraderIndex].reset();

	if (_iTraderIndex	<	m_wRookieTradeBox)
		m_wRookieTradeBox	=	_iTraderIndex;

	return	TRUE;
}

//
//	트레이드 박스 얻기
CTradeBox*
CTradeManager::getTradeBox(cACTOR *_lpTrader)
{
	if (!_lpTrader)	return	NULL;

	if (_lpTrader->m_wTradeBoxSerial	>=m_wMaxTradeBoxCount)
		return	NULL;

	if (m_pTradeBox[_lpTrader->m_wTradeBoxSerial].m_wSerial	==	0xffff)
		return	NULL;

	if (m_pTradeBox[_lpTrader->m_wTradeBoxSerial].getTraderIndex(_lpTrader->m_strName)	==	0xffff)
		return	NULL;

	return	&m_pTradeBox[_lpTrader->m_wTradeBoxSerial];
}