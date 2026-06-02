#include "CShop.h"
#include "CGame.h"
#include "debugCode.h"
#include "CEvent.h"

cPegeionPostShop	g_pigeonPostShop;
DWORD	l_adwBadgeOfTraderItemList[dBADGE_OF_TRADER_ITEM_LIST_COUNT][8] = 		// 수정
{	// 시리얼 , 가격, 개수, 임의 생성옵션사용, 옵션1 min, 옵션1 max, 옵션2 min, 옵션2 max 
	{3	,	15000	,	10	, FALSE	, 0,0,0,0},		// 라지 힐링 포션
	{4	,	200000	,	1	, FALSE	, 0,0,0,0},		// 풀 힐링 포션
	{7	,	10000	,	10	, FALSE	, 0,0,0,0},		// 라지 차징 포션
	{8	,	200000	,	1	, FALSE	, 0,0,0,0},		// 풀 차징 포션
	{12	,	1400	,	1	, TRUE	, 40,40,40,40},		// 꽃
	{38	,	1000	,	1	, FALSE	, 0,0,0,0},		// 마을 귀환 두루마리

};

DWORD			
CShopManager::getBadgeOfTraderItemSerial(int _iIndex)
{
	return l_adwBadgeOfTraderItemList[_iIndex][0];
}
DWORD			
CShopManager::getBadgeOfTraderItemPrice(int _iIndex)
{
	return l_adwBadgeOfTraderItemList[_iIndex][1];
}

DWORD			
CShopManager::getBadgeOfTraderItemCount(int _iIndex)
{
	return l_adwBadgeOfTraderItemList[_iIndex][2];
}

DWORD			
CShopManager::getIsBadgeOfTraderItemOption(int _iIndex)			// 추가
{
	return l_adwBadgeOfTraderItemList[_iIndex][3];
}
DWORD			
CShopManager::getBadgeOfTraderItemOption1Min(int _iIndex)		// 추가
{
	return l_adwBadgeOfTraderItemList[_iIndex][4];
}
DWORD			
CShopManager::getBadgeOfTraderItemOption1Max(int _iIndex)		// 추가
{
	return l_adwBadgeOfTraderItemList[_iIndex][5];
}
DWORD			
CShopManager::getBadgeOfTraderItemOption2Min(int _iIndex)		// 추가
{
	return l_adwBadgeOfTraderItemList[_iIndex][6];
}
DWORD			
CShopManager::getBadgeOfTraderItemOption2Max(int _iIndex)		// 추가
{
	return l_adwBadgeOfTraderItemList[_iIndex][7];
}
BOOL
CShop::getSellItemInfo(int _iIndex,cItem *_lpItem)
{
	_lpItem->reset();

	if	(_iIndex < 0 || _iIndex >= m_wItemCount)
		return	FALSE;
	
	_lpItem->copy(&m_pSellItemInfo[_iIndex]);

	if	(m_pGenerateItemInfo[_iIndex].m_bf15UpkeepHour)
	{
		int	iUpkeepTime		=	m_pGenerateItemInfo[_iIndex].m_bf15UpkeepHour;
		iUpkeepTime			=	iUpkeepTime*60;

		_lpItem->m_year		=	EncodeItemExpireYear(g_currentTime.m_wYear);
		_lpItem->m_month	=	g_currentTime.m_wMonth;
		_lpItem->m_day		=	g_currentTime.m_wDay;
		_lpItem->m_hour		=	g_currentTime.m_wHour;
		_lpItem->m_minute	=	g_currentTime.m_wMinute;

		_lpItem->increaseMinute(iUpkeepTime);
	}

	return	TRUE;
}

//
//	판매할 아이템 생성
void
CShop::generateItem()
{
	int	i,iIndex	=	0;
	int	iCount		=	m_wOrgItemCount;
	m_wItemCount	=	m_wOrgItemCount;

	for (i=0;i<iCount;i++)
	{
		CShopGenerateItemInfo	*lpGenerateItem	=	&m_pGenerateItemInfo[i];
		cItem					*lpSellItem		=	&m_pSellItemInfo[iIndex];

		lpSellItem->reset();
		memcpy(lpSellItem,lpGenerateItem,sizeof(CItemBaseInfo));

		if (lpSellItem->isIncorrectJobItem())
		{
			m_wItemCount--;
			continue;
		}

		iIndex++;

		g_im.generateShopItem(lpSellItem,lpGenerateItem);

		if (lpGenerateItem->m_dwPrice	==	0)	//	가격.. 가격이 0이면 아이템 자체의 가격 계산식으로 가격을 설정한다.
			lpSellItem->m_dwSerial	=	lpSellItem->getPrice(lpSellItem->m_bCount);
		else
			lpSellItem->m_dwSerial	=	lpGenerateItem->m_dwPrice;



		g_eventManager.operateItemByItemSaleEvent(lpSellItem);

		if	(g_bIsTestServer && g_bIsUseRealPrice == FALSE)
			lpSellItem->m_dwSerial		=	1;
	}
}

BOOL
cPegeionPostShop::init()
{
	cFILE	file;

	if	(!file.Open("data/pigeionPostShop.dat","rb"))
		return	FALSE;

	file.Read(&g_pigeonPostShop,sizeof(g_pigeonPostShop));
	file.Close();

	return	TRUE;
}

cPegeionPostShopItem*
cPegeionPostShop::get(int _iIndex)
{
	if	(_iIndex	<	0	||	_iIndex	>=	m_iItemCount)
		return	NULL;

	if	(m_aItemList[_iIndex].m_wBasicItem	==	0xffff)
		return	NULL;

	return	&m_aItemList[_iIndex];
}

BOOL
cPegeionPostShop::getItemForShop(cItem *_lpItem,int _iIndex,int _iCharge)
{
	cPegeionPostShopItem*	lpPostItem	=	get(_iIndex);

	if	(!lpPostItem)
		return	FALSE;

	_lpItem->reset();
	_lpItem->m_wBaseItem	=	lpPostItem->m_wBasicItem;
	_lpItem->m_dwSerial		=	lpPostItem->m_iPrice+lpPostItem->m_iPrice*_iCharge/100;
	_lpItem->m_aOption[0]	=	(BYTE)lpPostItem->m_awOption[0];
	_lpItem->m_aOption[1]	=	(BYTE)lpPostItem->m_awOption[1];
	_lpItem->m_bCount		=	(BYTE)lpPostItem->m_wCount;
	_lpItem->m_bDurability	=	1;

	return	TRUE;
}
