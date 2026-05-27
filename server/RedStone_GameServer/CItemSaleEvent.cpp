#include "CEvent.h"
#include "CGame.h"
#include "CDecoder.h"
#include "GameDLL.H"

//
//	아이템 추가
void
CItemSaleEvent::addItem(int _iItem,int _iGold,int _iType)
{
	if (m_iItemCount	>=	dMAX_SALE_ITEM_EVENT_ITEM_COUNT)
	{
		printf("Too Many Sale Event Item\n");

		return;
	}

	m_aItemInfo[m_iItemCount].m_dwPrice		=	_iGold;
	m_aItemInfo[m_iItemCount].m_wItemIndex	=	_iItem;
	m_aItemInfo[m_iItemCount].m_wMethod		=	_iType;

	m_iItemCount++;
}

//
//	지금 아이템 세일 기간이냐?
BOOL
CItemSaleEvent::isItemSaleTerm()
{
	DWORD	dwCurrentTimeValue	=	(g_currentTime.m_wYear-2000)*60*32*12*60+ g_currentTime.m_wMonth*60*32*60+ g_currentTime.m_wDay*60*60+g_currentTime.m_wHour*60+g_currentTime.m_wMinute;
	DWORD	dwBeginTimeValue	=	m_tiBeginTime.m_wYear*60*32*12*60+ m_tiBeginTime.m_wMonth*60*32*60+ m_tiBeginTime.m_wDay*60*60+m_tiBeginTime.m_wHour*60+m_tiBeginTime.m_wMinute;
	DWORD	dwEndTimeValue		=	m_tiEndTime.m_wYear*60*32*12*60+   m_tiEndTime.m_wMonth*60*32*60+   m_tiEndTime.m_wDay*60*60+  m_tiEndTime.m_wHour*60+  m_tiEndTime.m_wMinute;

	if (dwCurrentTimeValue >= dwBeginTimeValue && dwCurrentTimeValue <= dwEndTimeValue)
		return	TRUE;

	return	FALSE;
}

//	판매 아이템 찾기
CSaleItemInfo*
CItemSaleEvent::getSaleItem(int _iItem)
{
	for (int i=0;i<m_iItemCount;i++)
	{
		if (_iItem	==	m_aItemInfo[i].m_wItemIndex)
			return	&m_aItemInfo[i];
	}

	return	NULL;
}

//
//	아이템 세일 이벤트 동안의 아이템 처리
void
CItemSaleEvent::operateItemByItemSaleEvent(cItem *_lpItem)
{
	if	(!isItemSaleTerm())
		return;

	CSaleItemInfo	*lpItemSaleInfo	=	getSaleItem(_lpItem->m_wBaseItem);

	if	(!lpItemSaleInfo)
		return;

	if	(lpItemSaleInfo->m_wMethod	==	eSIM_SET)
		_lpItem->m_dwSerial	=	lpItemSaleInfo->m_dwPrice*_lpItem->m_bCount;

	_lpItem->m_year		=	m_tiEndTime.m_wYear;	//	유효기간
	_lpItem->m_month	=	m_tiEndTime.m_wMonth;
	_lpItem->m_day		=	m_tiEndTime.m_wDay;
	_lpItem->m_hour		=	m_tiEndTime.m_wHour;
	_lpItem->m_minute	=	m_tiEndTime.m_wMinute;
}

//	아이템 가격 얻기
int
CItemSaleEvent::getItemPriceByItemSaleEvent(cItem *_lpItem,DWORD _dwPrice)
{
	CSaleItemInfo	*lpItemSaleInfo	=	getSaleItem(_lpItem->m_wBaseItem);

	if (!lpItemSaleInfo)
		return _dwPrice;

	if (_lpItem->m_year	!= m_tiEndTime.m_wYear || _lpItem->m_month != m_tiEndTime.m_wMonth || _lpItem->m_day	!= m_tiEndTime.m_wDay || 
		_lpItem->m_hour != m_tiEndTime.m_wHour || _lpItem->m_minute != m_tiEndTime.m_wMinute)
		return _dwPrice;	//	이벤트 기간에 산 아이템은 아니다.

	if (lpItemSaleInfo->m_wMethod	==	eSIM_SET)
		return	lpItemSaleInfo->m_dwPrice;;

	return _dwPrice;
}

void
CItemSaleEvent::readData()
{
	cDECODER	decoder;
	char		strFolder[512];

	GetCurrentDirectory(sizeof(strFolder),strFolder);
	SetLogFolder();

	reset();

	if (!decoder.Upload("data/event.dat",NULL))
	{
		FPSetCurrentFolder(strFolder);

		return;
	}

	while(1)
	{
		char	*strCode	=	decoder.GetWord();

		if	(!strCode)
			break;

		if (STRICMP(strCode,"event")==0)
		{
			strCode	=	decoder.GetWord();

			if (STRICMP(strCode,"control_item_price")!=0)
				decoder.FindCloseCode();

			continue;
		}

		if (STRICMP(strCode,"beginTime")==0)
		{
			m_tiBeginTime.m_wYear	=	decoder.GetNumber();
			m_tiBeginTime.m_wMonth	=	decoder.GetNumber();
			m_tiBeginTime.m_wDay	=	decoder.GetNumber();
			m_tiBeginTime.m_wHour	=	decoder.GetNumber();
			m_tiBeginTime.m_wMinute	=	decoder.GetNumber();

			if	(m_tiBeginTime.m_wYear>=2000)
				m_tiBeginTime.m_wYear-=	2000;

			continue;
		}

		if (STRICMP(strCode,"endTime")==0)
		{
			m_tiEndTime.m_wYear		=	decoder.GetNumber();
			m_tiEndTime.m_wMonth	=	decoder.GetNumber();
			m_tiEndTime.m_wDay		=	decoder.GetNumber();
			m_tiEndTime.m_wHour		=	decoder.GetNumber();
			m_tiEndTime.m_wMinute	=	decoder.GetNumber();

			if	(m_tiEndTime.m_wYear>=	2000)
				m_tiEndTime.m_wYear	-=	2000;
			continue;
		}

		if (STRICMP(strCode,"set_item_price")==0)
		{
			char		*lpstrItemName	=	decoder.GetStream();
			cBasicItem	*lpItem			=	g_im.getItem(lpstrItemName);
			int			iGold			=	decoder.GetNumber();

			if (!lpItem)
			{
				printf("Can not find follow Item : %s\n",lpstrItemName);

				continue;
			}

			addItem(lpItem->m_iSerial,iGold,eSIM_SET);

			continue;
		}

		if (STRICMP(strCode,"end")==0)	
			break;
	}

	g_game.rebuildAllShopInfo();
}

//	초기화
void
CItemSaleEvent::reset()
{
	m_iItemCount	=	0;
	
	memset(&m_tiBeginTime,0,sizeof(m_tiBeginTime));
	memset(&m_tiEndTime,0,sizeof(m_tiEndTime));
}

//	업데이트
void
CItemSaleEvent::update()
{
	BOOL	bIsActive	=	isItemSaleTerm();

	if (m_bIsActive	!=	bIsActive)
	{
		g_game.rebuildAllShopInfo();
		m_bIsActive	=	bIsActive;
	}
}