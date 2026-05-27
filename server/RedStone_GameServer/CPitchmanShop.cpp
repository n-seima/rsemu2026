#include <math.h>
#include "CPitchmanShop.h"
#include "CActor.h"
#include "CField.h"
#include "CGame.h"
#include "cPACKET_GAMESERVER.h"

CPitchmanShopManager	g_pitchmanShopManager;

void
CPitchmanShop::reset()
{
	m_wSerial			=	0xffff;

	m_wSignboardShape	=	0;
	m_wTextColor		=	0;
	m_wIsBold			=	FALSE;
	m_wIsShadowText		=	FALSE;
	m_wStatus			=	ePSS_READY;

	memset(m_aItem,0xff,sizeof(m_aItem));
}


void
CPitchmanShop::copy(CPitchmanShop *_lpData)
{
	memcpy(this,_lpData,sizeof(CPitchmanShop));
}

cACTOR*
CPitchmanShop::getOwner()
{
	cACTOR	*lpActor			=	g_game.getActor(m_iPitchmanSerial);

	if (!lpActor	||	lpActor->m_dwNameHashCode!=	m_dwPitchmanHashCode)
		return	NULL;

	return	lpActor;
}

BOOL
CPitchmanShop::getSimpleShopInfo(CSimplePitchmanShopInfo *_lpInfo)
{
	cACTOR	*lpActor			=	g_game.getActor(m_iPitchmanSerial);

	if (!lpActor	||	lpActor->m_dwNameHashCode!=	m_dwPitchmanHashCode)
		return	FALSE;

	_lpInfo->m_bf10Serial		=	m_wSerial;
	_lpInfo->m_bf6SignboardShape=	m_wSignboardShape;
	_lpInfo->m_wTextColor		=	m_wTextColor;
	_lpInfo->m_wX				=	m_pos.x;
	_lpInfo->m_wY				=	m_pos.y;
	_lpInfo->m_bf1IsShadowText	=	m_wIsShadowText;
	_lpInfo->m_bf1IsBold		=	m_wIsBold;
	_lpInfo->m_bf2Status		=	m_wStatus;
	_lpInfo->m_bf12Owner		=	lpActor->m_wSerialInField;

	return	TRUE;
}

BOOL
CPitchmanShop::getInfo(CPitchmanShopForClient *_lpInfo)	//09.08.21
{
	cACTOR	*lpActor			=	g_game.getActor(m_iPitchmanSerial);

	if	(!lpActor	||	lpActor->m_dwNameHashCode!=	m_dwPitchmanHashCode)
		return	FALSE;

	_lpInfo->m_wSerial			=	m_wSerial;
	_lpInfo->m_bf4SignboardShape=	m_wSignboardShape;
	_lpInfo->m_wTextColor		=	m_wTextColor;
	_lpInfo->m_bf1IsBold		=	m_wIsBold;
	_lpInfo->m_bf1IsShadowText	=	m_wIsShadowText;
	_lpInfo->m_bf4ItemCount		=	0;

	strcpy(_lpInfo->m_strName,m_strName);
	strcpy(_lpInfo->m_strOwnerName,lpActor->m_strName);

	for (int i=0;i<dPITCHMAN_SHOP_INVENTORY_SIZE;i++)
	{
		int	iInventorySlot		=	m_aItem[i].m_wSlotIndex;

		if	(iInventorySlot		==	0xffff)
			continue;

		cItem	*lpItem			=	lpActor->getInventoryItem(iInventorySlot);

		if	(!lpItem)
			return	FALSE;

		CPitchmanShopItemInfoForClient	*lpSlot	=	&_lpInfo->m_aItem[_lpInfo->m_bf4ItemCount];

		memcpy(&lpSlot->m_item,lpItem,sizeof(cItem));
		lpSlot->m_wSlot				=	i;
		lpSlot->m_dwPrice			=	m_aItem[i].m_dwPrice;
		lpSlot->m_bDenomination		=	m_aItem[i].m_bDenomination;

		_lpInfo->m_bf4ItemCount++;
	}

	return	TRUE;
}

int
CPitchmanShop::addItem(int _iInventorySlot,int _iShopSlot,int _iPrice, BYTE _bDenomination)		// 09.08.21
{
	if (_iShopSlot < 0 || _iShopSlot >= dPITCHMAN_SHOP_INVENTORY_SIZE)
		return	ePSM_FAILED;

	if (_iInventorySlot < 0 || _iInventorySlot >= dOWN_ITEM_COUNT)
		return	ePSM_FAILED;

	for (int i=0;i<dPITCHMAN_SHOP_INVENTORY_SIZE;i++)
	{
		if (m_aItem[i].m_wSlotIndex	==	0xffff)
			continue;

		if (m_aItem[i].m_wSlotIndex	==	_iInventorySlot)
			return	ePSM_FAILED_REGIST_ITEM_BY_ALREADY_REGISTED;
	}
	
	cACTOR	*lpActor	=	getOwner();

	if (!lpActor || lpActor->m_dwNameHashCode != m_dwPitchmanHashCode)
		return	ePSM_FAILED;

	cItem	*lpItem		=	lpActor->getInventoryItem(_iInventorySlot);

	if (!lpItem)
		return	ePSM_FAILED;

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if (!lpBasicItem)
		return	ePSM_FAILED;

	if (lpBasicItem->m_attr.isCanNotTradeItem)
		return	ePSM_FAILED_BY_CAN_NOT_TRADE_ITEM;
	
	if (lpItem->isReversionItem())
		return	ePSM_FAILED_BY_CAN_NOT_TRADE_ITEM_IS_REVERSION_ITEM;

	m_aItem[_iShopSlot].m_wSlotIndex	=	_iInventorySlot;
	m_aItem[_iShopSlot].m_dwPrice		=	_iPrice;
	m_aItem[_iShopSlot].m_bDenomination	=	_bDenomination;

	return	ePSM_OK;
}
int
CPitchmanShop::removeItem(int _iShopSlot)
{
	if (_iShopSlot < 0 || _iShopSlot >= dPITCHMAN_SHOP_INVENTORY_SIZE)
		return	ePSM_FAILED;

	m_aItem[_iShopSlot].m_wSlotIndex	=	0xffff;

	return	ePSM_OK;
}

int
CPitchmanShop::changePitchmanShopPlace(int _iShopSlot1,int _iShopSlot2)
{
	if (_iShopSlot1 < 0 || _iShopSlot1 >= dPITCHMAN_SHOP_INVENTORY_SIZE)
		return	ePSM_FAILED;

	if (_iShopSlot2 < 0 || _iShopSlot2 >= dPITCHMAN_SHOP_INVENTORY_SIZE)
		return	ePSM_FAILED;

	CPitchmanShopItemInfo	item;

	memcpy(&item				,&m_aItem[_iShopSlot1]	,sizeof(CPitchmanShopItemInfo));
	memcpy(&m_aItem[_iShopSlot1],&m_aItem[_iShopSlot2]	,sizeof(CPitchmanShopItemInfo));
	memcpy(&m_aItem[_iShopSlot2],&item					,sizeof(CPitchmanShopItemInfo));

	return	ePSM_OK;
}

int
CPitchmanShop::changeShopInfo(int _iStatus,char *_lpstrName,int _iSignboardShape,WORD _wTextColor,BOOL _bIsBold,BOOL _bIsShadowText)
{
	strcpy(m_strName,_lpstrName);
	m_wSignboardShape	=	_iSignboardShape;
	m_wTextColor		=	_wTextColor;
	m_wIsBold			=	_bIsBold;
	m_wStatus			=	_iStatus;
	m_wIsShadowText		=	_bIsShadowText;

	return	ePSM_OK;
}

void
CPitchmanShopManager::reset()
{
	m_den.reset();

	for (int i=0;i<getMaxCount();i++)
		m_den.notCheckedget(i)->reset();
}


//	액터가 나간다. 노점상 철거
BOOL
CPitchmanShopManager::outActor(int _iSlot,DWORD _dwHashCode)
{
	int	iCount	=	0;

	CPitchmanShop	*lpMan	=	m_den.get(_iSlot);

	if	(!lpMan)
		return FALSE;

	if	(lpMan->m_dwPitchmanHashCode != _dwHashCode)
		return FALSE;

	return	remove(_iSlot);
}

int
CPitchmanShopManager::addShop(cACTOR *_lpActor)
{
	CPitchmanShop	shop;

	shop.reset();

	strcpy(shop.m_strName,_lpActor->m_strName);

	shop.m_iPitchmanSerial		=	_lpActor->m_iZoneSerial;
	shop.m_dwPitchmanHashCode	=	_lpActor->m_dwNameHashCode;
	shop.m_wFieldSerial			=	_lpActor->m_lpField->m_iUniqueSerial;
	shop.m_wActorItemChecksum	=	_lpActor->getItemCheckSum();
	shop.m_wSize				=	dPITCHMAN_SHOP_MINIMUM_SIZE;
	shop.m_pos.x				=	_lpActor->m_pos.x;
	shop.m_pos.y				=	_lpActor->m_pos.y-10;

	int	iSerial	=	add(&shop);

	return	iSerial;
}

BOOL
CPitchmanShopManager::closeShop(int _iSerial)
{
	remove(_iSerial);

	return	TRUE;
}

BOOL
CPitchmanShopManager::isExistNearestShop(int _iX,int _iY,int _iFieldSerial)
{
	int	i,iCount=0;
	int	iOldCount	=	getCount();

	for (i=0;i<m_den.getMaxCount() && iCount < iOldCount;i++)
	{
		CPitchmanShop	*lpShop	=	get(i);

		if	(!lpShop)
			continue;

		if	(lpShop->m_wFieldSerial	!=	_iFieldSerial)
			continue;

		if	(lpShop->getOwner()	==	NULL)
		{
			closeShop(i);
			continue;
		}
		
		int	iRange	=	GetOvalRange(_iX,_iY,lpShop->m_pos.x,lpShop->m_pos.y);

		iRange		=	(int)sqrt(iRange);

		if	(iRange	<=	dPITCHMAN_SHOP_SIZE)
			return	TRUE;

		iCount++;
	}

	return	FALSE;
}

BOOL
CPitchmanShopManager::getSimpleShopInfo(CSimplePitchmanShopInfo *_lpInfo,int _iSerial)
{
	CPitchmanShop	*lpShop		=	get(_iSerial);

	if (!lpShop)
		return	FALSE;

	return	lpShop->getSimpleShopInfo(_lpInfo);
}

BOOL
CPitchmanShopManager::getShopInfo(CPitchmanShopForClient *_lpInfo,int _iSerial)
{
	CPitchmanShop	*lpShop		=	get(_iSerial);

	if (!lpShop)
		return	FALSE;

	return	lpShop->getInfo(_lpInfo);
}

inline	BOOL
Is2ByteChar(char _char)
{
#ifdef	_FOR_JAPAN
	if	((BYTE)_char	>=	0xa1 && (BYTE)_char <=0xdf)
		return	FALSE;
#endif

	if (_char	<	0)
		return	TRUE;

	return	FALSE;
}

inline	BOOL
Is2ByteChar(char *_lpstrText,int _iPoint)
{
	int	iPoint	=	0;

	while(_lpstrText[iPoint])
	{
		if (Is2ByteChar(_lpstrText[iPoint]))
		{
			if (_iPoint	==	iPoint || _iPoint	==	iPoint+1)
				return	TRUE;

			iPoint++;
		}

		iPoint++;
	}

	return	FALSE;
}

inline	BOOL	
IsIncludeString(char *_lpstrText,char *_lpstrWord)
{
	int	iWordLength	=	strlen(_lpstrWord);

	for (int i=0;i<iWordLength;i++)
	{
		if	(_lpstrText[i] ==	NULL			)
			return	FALSE;
		if	(_lpstrText[i] !=	_lpstrWord[i]	)
			return	FALSE;
	}

	return	TRUE;
}

//
//	2바이트 문자 체크를 위한거.. 대충 그런거다.. - -
//	차라리 유니코드로 변환후 찾는게 빠를지도..
BOOL	
sfcStrstr(char *_lpstrText,char *_lpstrWord)
{
	int	iLength		=	strlen(_lpstrText),i;
	int	iWordLength	=	strlen(_lpstrWord);

	for (i=0;i<iLength;)
	{
		if	(IsIncludeString(_lpstrText+i,_lpstrWord))
			return	TRUE;

		if	(Is2ByteChar(_lpstrText[i]))
			i++;

		i++;
	}

	return	FALSE;
}

BOOL
CPitchmanShopManager::checkItemFilter(void *_lpSerchPacket,cItem *_lpItem)
{
	CG_SEARCH_RELAY_STATION_ITEM				*lpFilter	=	(CG_SEARCH_RELAY_STATION_ITEM*)_lpSerchPacket;

	BOOL	bIsAndCondition	=	1-lpFilter->bf1IsORCondition;
	int		iMatchCount	=	0;

	if	(lpFilter->bf1MaskUnique)
	{
		if	(_lpItem->isUniqueItem()	==	TRUE)
			iMatchCount++;
		else
		if	(bIsAndCondition)
			return	FALSE;
	}
	else
		iMatchCount++;

	if	(lpFilter->bf1MaskMagic	)
	{
		if	(_lpItem->isRareItem()	==	TRUE)
			iMatchCount++;
		else
		if	(bIsAndCondition)
			return	FALSE;
	}
	else
		iMatchCount++;

	if	(lpFilter->bf1MaskDX)
	{
		if	(_lpItem->isDXItem()	==	TRUE)
			iMatchCount++;
		else
		if	(bIsAndCondition)
			return	FALSE;
	}
	else
		iMatchCount++;

	if	(lpFilter->bf1MaskUltimate)
	{
		if	(_lpItem->isUltimateItem()	==	TRUE)
			iMatchCount++;
		else
		if	(bIsAndCondition)
			return	FALSE;
	}
	else
		iMatchCount++;

	if	(lpFilter->strFilter[0])
	{
		char	*lpstrName	=	_lpItem->getName();

		if	(sfcStrstr(lpstrName,lpFilter->strFilter))
			iMatchCount++;
		else
		if	(bIsAndCondition)
			return	FALSE;
	}
	else
		iMatchCount++;

	if	(iMatchCount	==	0)
		return	TRUE;

	return	FALSE;
}

int
CPitchmanShopManager::serchItem(void *_lpSerchPacket,void *_lpResultPacket)
{
	CG_SEARCH_RELAY_STATION_ITEM				*lpFilter	=	(CG_SEARCH_RELAY_STATION_ITEM*)_lpSerchPacket;
	SG_RESULT_OF_SEARCH_ITEM_IN_RELAY_STATION	*lpResult	=	(SG_RESULT_OF_SEARCH_ITEM_IN_RELAY_STATION	*)_lpResultPacket;

	lpFilter->strFilter[64-1]=	NULL;

	lpResult->wIsFinish		=	FALSE;

	int		iBeginShop			=	lpFilter->wLastShop;
	int		iBeginSlot			=	lpFilter->wLastSlot;
	int		iDefaultBeginSlot	=	0;
	int		iAddValue			=	1;

	BOOL	bIsReverseSearch=	FALSE;

	if	(lpFilter->bf1IsSearchReverse)
	{
		bIsReverseSearch	=	TRUE;
		iDefaultBeginSlot	=	dPITCHMAN_SHOP_INVENTORY_SIZE;
		iAddValue			=	0;
	}

	for (int iShop=lpFilter->wLastShop;;iShop+=iAddValue)
	{
		if	(bIsReverseSearch)
		{
			--iShop;

			if	(iShop	==	0)
				break;
		}
		else
		if	(iShop	>=	dPITCHMAN_SHOP_INVENTORY_SIZE)
			break;


		CPitchmanShop	*lpShop	=	get(iShop);

		if	(!lpShop)
			continue;

		cACTOR	*lpSaler	=	g_game.getActor(lpShop->m_iPitchmanSerial);

		if	(!lpSaler)
			continue;

		for (int iSlot=iBeginSlot;;iSlot+=iAddValue)
		{
			if	(bIsReverseSearch)
			{
				--iSlot;

				if	(iSlot	==	0)
					break;
			}
			else
			if	(iSlot	>=	dPITCHMAN_SHOP_INVENTORY_SIZE)
				break;

			int	iInventorySlot	=	lpShop->m_aItem[iSlot].m_wSlotIndex;

			if	(iInventorySlot	==	0xffff)
				continue;

			cItem	*lpItem			=	lpSaler->getInventoryItem(iInventorySlot);

			if	(!lpItem)
				continue;

			if	(checkItemFilter(_lpSerchPacket,lpItem)	==	FALSE)
				continue;

			CPitchmanShopItemInfoForClient	*lpResultItem	=	&lpResult->m_aItemList[lpResult->wCount];

			memcpy(&lpResultItem->m_item,lpItem,sizeof(cItem));
			lpResultItem->m_wSlot	=	iSlot;
			lpResultItem->m_dwPrice	=	lpShop->m_aItem[iSlot].m_dwPrice;

			lpResult->wShopSerial	=	iShop;
			lpResult->wLastSlot		=	iSlot;

			lpResult->wCount++;

			if	(lpResult->wCount	>=	dRESULT_ITEM_COUNT_OF_SEARCH_ITEM_IN_RELAY_STATION)
				break;
		}

		if	(lpResult->wCount	>=	dRESULT_ITEM_COUNT_OF_SEARCH_ITEM_IN_RELAY_STATION)
			break;

		iBeginSlot	=	iDefaultBeginSlot;
	}

	if	(lpResult->wCount	<	dRESULT_ITEM_COUNT_OF_SEARCH_ITEM_IN_RELAY_STATION)
		lpResult->wIsFinish	=	TRUE;

	return	TRUE;
}