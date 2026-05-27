#include "CShoppingCart.h"

CShoppingCart	g_shoppingCart;

void
CShoppingCart::open(tsBuyLst *_lpCartInfo,int _iItemCount,int _iSerial)
{
	if (_iItemCount > 0)
		memcpy(m_aItemList,_lpCartInfo,sizeof(tsBuyLst)*_iItemCount);
	
	m_wItemCount	=	_iItemCount;
	m_wSerial		=	_iSerial;
}


DWORD
CShoppingCart::getCheckSum()
{
	int		i;
	BYTE	*lpCartBuffer	=	(BYTE *)m_aItemList;
	int		iCartBufferSize	=	m_wItemCount*sizeof(tsBuyLst);

	m_dwCheckSum			=	0;

	for (i=0;i<iCartBufferSize;i++)
	{
		if	(i%2)
			m_dwCheckSum	+=	lpCartBuffer[i];
		else
			m_dwCheckSum	-=	lpCartBuffer[i];
	}

	return	m_dwCheckSum;
}


tsBuyLst*
CShoppingCart::getCartItem(int _iSlot)
{
	if(_iSlot <0 || _iSlot >= m_wItemCount)	return	NULL;

	return	&m_aItemList[_iSlot];
}

void
CShoppingCart::close()
{
	m_wSerial	=	0xffff;
}	//	CShoppingCart::close()

BOOL
CShoppingCart::generateItem(cItem *_lpItem,int _iSlot)
{
	if	(_iSlot < 0 || _iSlot	>= m_wItemCount)
		return	FALSE;

	tsBuyLst	*lpCartItem		=	&m_aItemList[_iSlot];

	if	(lpCartItem->dwItemNo	==	0xffffffff)
		return	FALSE;

	CCustomItem	*lpCustomItem	=	g_premiumItem.getItem(lpCartItem->dwItemNo);

	if	(!lpCustomItem)
		return	FALSE;

	if	(!lpCustomItem->generateItem(_lpItem))
		return	FALSE;

	_lpItem->m_year		=	lpCartItem->timeEndDate.year;
	_lpItem->m_month	=	lpCartItem->timeEndDate.month;
	_lpItem->m_day		=	lpCartItem->timeEndDate.day;
	_lpItem->m_hour		=	lpCartItem->timeEndDate.hour;
	_lpItem->m_minute	=	lpCartItem->timeEndDate.minute;

	return	TRUE;
}