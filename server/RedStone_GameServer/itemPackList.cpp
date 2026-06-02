#include	"itemPackList.h"
#include	"cGame.h"

cItemPackList	g_itemPackList;

void
cItemPackList::init()
{
	reset();
}

void
cItemPackList::reset()
{
	m_bIsReceiveAllInfo	=	FALSE;

	for (int i=0;i<dMAX_ITEM_PACK_COUNT;i++)
		m_aPacks[i].reset();
}

BOOL
cItemPackList::generatePackItem(cItem *_lpItem,int _iPack,int _iCount)
{
	cItemPackInfo *lpPack	=	&m_aPacks[_iPack];

	if	(lpPack->m_wLinkItem==	0xffff)
	{
		CLOG("itemPack","generatePackItem - is invalid pack : %d",_iPack);

		return	FALSE;
	}

	_lpItem->reset();

	_lpItem->m_wBaseItem	=	lpPack->m_wLinkItem;
	_lpItem->m_dwSerial		=	largeRandom(0x7ffffff0)+10;
	_lpItem->m_bCount		=	_iCount;
	_lpItem->m_aOption[0]	=	_iPack;

	_lpItem->m_bCount		=	min(_iCount,_lpItem->getStackLimit());

	return	TRUE;
}

void
cItemPackList::receivePack(cItemPackInfo *_lpPack)
{
	printf("- receive item pack #%.3d info\n",_lpPack->m_wSerial);
	memcpy(&m_aPacks[_lpPack->m_wSerial],_lpPack,sizeof(cItemPackInfo));
}

BOOL
cItemPackList::checkReceiveAllInfo()
{
	return	m_bIsReceiveAllInfo;
}

void
cItemPackList::resetCurrentItemDropCount(int _iPack)
{
	cItemPackInfo	*lpPack	=	&m_aPacks[_iPack];

	for (int i=0;i<dMAX_ITEM_IN_ITEM_PACK;i++)
		lpPack->m_aItems[i].m_wCurrentCount	=	0;
}

void
cItemPackList::updateItemDropCount(int _iPack,int _iItem,int _iCount)
{
	m_aPacks[_iPack].m_aItems[_iItem].m_wCurrentCount	=	_iCount;
}

int
cItemPackList::getKwangItem(int _iPackSerial)
{
	cItemPackInfo *_lpPack	=	&m_aPacks[_iPackSerial];

	for (int i=0;i<_lpPack->m_wCount;i++)
	{
		if (_lpPack->m_aItems[i].m_dwFrequency	==	0)
			return	i;
	}

	return	-1;
}

BOOL
cItemPackList::getItem(cItem *_lpResultItem,int _iPackSerial,int _iItem,int _iPrefix,BOOL _bIsTest)
{
	if	(_iPackSerial < 0 || _iPackSerial >= dMAX_ITEM_PACK_COUNT)
	{
		CLOG("itemPack","getRandomItem - _iPackSerial : %d",_iPackSerial);

		return	FALSE;
	}

	cItemPackInfo *_lpPack	=	&m_aPacks[_iPackSerial];

	if	(_lpPack->m_wLinkItem==	0xffff)
	{
		CLOG("itemPack","getRandomItem - is invalid pack : %d",_iPackSerial);

		return	FALSE;
	}

	if	(_iItem	>=	_lpPack->m_wCount)
		return	FALSE;

	int			iIndex	=	_iItem;
	cItemInPack	*lpSrcItem;
	int			iLoopCount=0;

	lpSrcItem	=	&_lpPack->m_aItems[iIndex];

	memcpy(_lpResultItem,&_lpPack->m_aItems[iIndex],sizeof(cItem));

	cBasicItem	*lpBasicItem	=	NULL;

	if	(_bIsTest	==	FALSE)
		g_game.sendWPIncreasePackItemCount(_iPackSerial,iIndex);

	{
		lpBasicItem						=	((cItem *)lpSrcItem)->getBasicItem();

		_lpResultItem->m_bDurability	=	lpBasicItem->getDurability();
		_lpResultItem->m_dwSerial		=	largeRandom(0x7ffffff0)+10;

		if	(lpSrcItem->m_bf1IsBlockToTrade)
		{
			_lpResultItem->m_bf1IsSpecialItem	=	TRUE;
			cSpecialItem	*lpSpecialItem		=	_lpResultItem->getSpecialItem();

			lpSpecialItem->reset();
			lpSpecialItem->m_bf1IsReversionItem	=	TRUE;
		}
		else
		if	(lpBasicItem->m_attr.isExtraItem	&&	lpBasicItem->isChargeExtraItem() == FALSE)
		{
			_lpResultItem->m_year	=	EncodeItemExpireYear(g_currentTime.m_wYear);
			_lpResultItem->m_month	=	g_currentTime.m_wMonth;
			_lpResultItem->m_day	=	g_currentTime.m_wDay;
			_lpResultItem->m_hour	=	g_currentTime.m_wHour;
			_lpResultItem->m_minute	=	g_currentTime.m_wMinute;

			_lpResultItem->increaseMinute(dITEM_FULL_CHARGE_TIME);
		}

		if	(_iPrefix	>=	0	&&	_iPrefix	<	dITEM_PREFIX_COUNT)
		{
			memset(_lpResultItem->m_aPrefix,0xff,sizeof(_lpResultItem->m_aPrefix));
			memcpy(&_lpResultItem->m_aPrefix[0],&lpSrcItem->m_aPrefix[_iPrefix],sizeof(CItemPrefixInfo));
		}
	}

	return	TRUE;
}

cItem*
cItemPackList::getRandomItem(int _iPackSerial,BOOL _bIsTest)
{
	if	(_iPackSerial < 0 || _iPackSerial >= dMAX_ITEM_PACK_COUNT)
	{
		CLOG("itemPack","getRandomItem - _iPackSerial : %d",_iPackSerial);

		return	NULL;
	}

	cItemPackInfo *_lpPack	=	&m_aPacks[_iPackSerial];

	if (_lpPack->m_wLinkItem==	0xffff)
	{
		CLOG("itemPack","getRandomItem - is invalid pack : %d",_iPackSerial);

		return	NULL;
	}

	if (_lpPack->m_wCount	==	0)
	{
		CLOG("itemPack","getRandomItem - _lpPack->m_wCount : %d",_lpPack->m_wCount);

		return	NULL;
	}

	int			iIndex;
	cItemInPack	*lpItem;
	int			iLoopCount=0;

	while(1)
	{
		iIndex	=	random(_lpPack->m_wCount);
		lpItem	=	&_lpPack->m_aItems[iIndex];

		if	(lpItem->m_wLimitCountOfServer	==	0	||	lpItem->m_wCurrentCount	<	lpItem->m_wLimitCountOfServer)
			break;

		iLoopCount++;

		if	(iLoopCount	>=	10000)
		{
			lpItem	=	NULL;
			break;
		}
	}

	cBasicItem	*lpBasicItem	=	NULL;

	if	(lpItem)
	{
		int	iChance		=	max(lpItem->m_dwFrequency/_lpPack->m_wCount,1);

		if	(random(iChance)	==	0)
		{
			if	(_bIsTest	==	FALSE)
				g_game.sendWPIncreasePackItemCount(_iPackSerial,iIndex);

			if	(lpItem->m_bf1IsPrizeWinningItem)
			{
				SG_WINNING_PRIZE_LOTTO	packet;

				packet.base.set(sizeof(packet),dSG_WINNING_PRIZE_LOTTO);
				packet.wItem	=	lpItem->m_wBaseItem;

				g_game.sendPacketToAllPlayer((char *)&packet);
			}
		}
		else
			lpItem	=	NULL;
	}

	if	(lpItem	==	NULL)
	{
		iIndex	=	getKwangItem(_iPackSerial);

		if	(iIndex	!=	-1)
		{
			lpItem					=	&_lpPack->m_aItems[iIndex];

			if (_bIsTest	==	FALSE)
				g_game.sendWPIncreasePackItemCount(_iPackSerial,iIndex);
		}
	}

	if	(lpItem)
	{
		lpBasicItem				=	((cItem *)lpItem)->getBasicItem();

		lpItem->m_bDurability	=	lpBasicItem->getDurability();
		lpItem->m_dwSerial		=	largeRandom(0x7ffffff0)+10;

		if	(lpItem->m_bf1IsBlockToTrade)
		{
			cItem			*lpMakeItem		=	(cItem *)lpItem;

			lpMakeItem->m_bf1IsSpecialItem	=	TRUE;
			cSpecialItem	*lpSpecialItem	=	lpMakeItem->getSpecialItem();

			lpSpecialItem->reset();
			lpSpecialItem->m_bf1IsReversionItem	=	TRUE;
		}
		else
		if	(lpBasicItem->m_attr.isExtraItem	&&	lpBasicItem->isChargeExtraItem() == FALSE)
		{
			lpItem->m_year		=	EncodeItemExpireYear(g_currentTime.m_wYear);
			lpItem->m_month		=	g_currentTime.m_wMonth;
			lpItem->m_day		=	g_currentTime.m_wDay;
			lpItem->m_hour		=	g_currentTime.m_wHour;
			lpItem->m_minute	=	g_currentTime.m_wMinute;

			((cItem *)lpItem)->increaseMinute(dITEM_FULL_CHARGE_TIME);
		}

		for	(int iPrefix=0;iPrefix<dITEM_PREFIX_COUNT;iPrefix++)
			if	(lpItem->m_aPrefix[iPrefix].m_wPrefix	==	0xffff)
				memset(lpItem->m_aPrefix[iPrefix].m_aValue,0xff,sizeof(lpItem->m_aPrefix[iPrefix].m_aValue));

		return	(cItem *)lpItem;
	}

	return	NULL;
}

WORD
cItemPackList::getLinkItem(int _iPackSerial)
{
	return	m_aPacks[_iPackSerial].m_wLinkItem;
}

BOOL
cItemPackList::linkItem(cItem *_lpItem)
{
	for (int i=0;i<dMAX_ITEM_PACK_COUNT;i++)
	{
		if	(m_aPacks[i].m_wLinkItem==	0xffff)
			continue;

		if (m_aPacks[i].m_wLinkItem	==	_lpItem->m_wBaseItem)
		{
			_lpItem->m_aOption[0]	=	i;
			return	TRUE;
		}
	}

	return	FALSE;
}
