#include "CShop.h"
#include "cMAP.h"

//
//	생성자
CShopItem::CShopItem()
{
	reset();
}	//	CShopItem::CShopItem()

//
//	초기화
void
CShopItem::reset()
{
	memset(this,0,sizeof(CShopItem));
	memset(m_aPrefix,0xff,sizeof(m_aPrefix));

	m_wBaseItem			=	0xffff;

	m_bCount			=	1;
	m_wSerial			=	0xffff;
	m_wGenerateQuality	=	100;
	m_wPrefixQuality	=	50;
	m_bf1IsRandomPrefix	=	FALSE;	//	접두사가 랜덤으로 붙을껴?
	m_bf15UpkeepHour	=	0;
	m_bf10ChangeLimitLevel=	0;		//	접두사 재 설정 주기
	m_dwPrice			=	0;		//	가격.. 가격이 0이면 아이템 자체의 가격 계산식으로 가격을 설정한다.
}	//	CShopItem::reset()

//
//	초기화
void
CShopItem::copy(CShopItem *_lpItem)
{
	memcpy(this,_lpItem,sizeof(CShopItem));
}	//	CShopItem::copy(CShopItem *_lpItem)


CShop::CShop()
{
	m_denItem.init(dMAX_SHOP_ITEM_COUNT,TRUE);
	m_wPriceFactor		=	25;
	m_TokkenShop		=	0xffff;
}	//	CShop::CShop()

CShop::~CShop()
{
	m_denItem.close();
}	//	CShop::~CShop()

//
//	 아이템 위치 변경
BOOL
CShop::changeItemPlace(int _iFirstItem,int _iSecondItem)
{
	if (_iFirstItem	==	_iSecondItem)	return	FALSE;
	if (_iFirstItem	>=	getCount()	||	_iFirstItem	<	0)	return	FALSE;
	if (_iSecondItem>=	getCount()	||	_iSecondItem<	0)	return	FALSE;

	CShopItem	*lpFirstItem	=	m_denItem.get(_iFirstItem);
	CShopItem	*lpSecondItem	=	m_denItem.get(_iSecondItem);
	CShopItem	temp;

	temp.copy(lpFirstItem);
	lpFirstItem->copy(lpSecondItem);
	lpSecondItem->copy(&temp);	

	lpFirstItem->m_wSerial	=	_iFirstItem;
	lpSecondItem->m_wSerial	=	_iSecondItem;

	return	TRUE;
}	//	CShop::changeItemPlace(int _iFirstItem,int _iSecondItem)

//
//초기화
void
CShop::reset()
{
	m_wSerial		=	0xffff;
	m_wPriceFactor	=	25;
	m_dwLimitPrice	=	10000;
	m_TokkenShop	=	0xffff;
	strcpy(m_strName,"");
	m_denItem.reset();
}	//	CShop::reset()

//
//아이템 추가
int
CShop::addItem(CShopItem *_lpItem)
{
	return	m_denItem.add(_lpItem);
}

//
//아이템 끼워넣기
BOOL
CShop::insertItem(CShopItem *_lpItem)
{
	return m_denItem.insert(_lpItem->m_wSerial,_lpItem);
}	//	CShop::insertItem(CShopItem *_lpItem)

//
//아이템 제거
BOOL
CShop::removeItem(int _iIndex)
{
	return	m_denItem.remove(_iIndex);
}//	CShop::removeItem(int _iIndex)

//
// 복사
void
CShop::copy(CShop *_lpShop)
{	// 마지막 수정일 : 09.10.16
	reset();

	strcpy(m_strName,_lpShop->m_strName);//	가게 이름
	m_wPriceFactor	=	_lpShop->m_wPriceFactor;
	m_dwLimitPrice	=	_lpShop->m_dwLimitPrice;
	m_bf1IsDuelPointShop=	_lpShop->m_bf1IsDuelPointShop;
	m_bf1IsLevelPerGoldShop	= _lpShop->m_bf1IsLevelPerGoldShop;
	m_bf15LevelPerGoldValue	= _lpShop->m_bf15LevelPerGoldValue;
	m_bf1IsNotBuyItem		= _lpShop->m_bf1IsNotBuyItem;	// 상점 구입 불가.
	m_TokkenShop		=	_lpShop->m_TokkenShop;

	for (int i=0;i<dMAX_SHOP_ITEM_COUNT;i++)
	{
		CShopItem	*lpItem	=	_lpShop->get(i);

		if (!lpItem)	continue;

		m_denItem.insert(lpItem->m_wSerial,lpItem);
	}
}	//	CShop::copy(CShop *_lpShop)

CShopManager::CShopManager()
{
	m_iCount	=	0;
	m_iRookie	=	0;

	for (int i=0;i<dMAX_SHOP;i++)
		m_aShop[i].m_wSerial	=	0xffff;
}	//	CShop::CShop()

CShopManager::~CShopManager()
{

}	//	CShop::~CShop()

//
//	초기화
void
CShopManager::reset()
{
	m_iCount	=	0;
	m_iRookie	=	0;

	for (int i=0;i<dMAX_SHOP;i++)
		m_aShop[i].reset();
}	//	CShopManager::reset()

//
//	상점 추가
BOOL
CShopManager::addShop(CShop *_lpShop)
{
	if	(m_iCount	>=	dMAX_SHOP)
		return	FALSE;

	int	iRookie	=	m_iRookie;

	m_aShop[m_iRookie].copy(_lpShop);
	m_aShop[m_iRookie].m_wSerial	=	m_iRookie;

	for (int i=iRookie+1;i<dMAX_SHOP;i++)
	{
		if (m_aShop[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}
	}

	m_iCount++;

	g_lpActmap->LostVirgin();

	return	iRookie;
}	//	CShopManager::addShop(CShop *_lpShop);

//
//	상점 끼워넣기-_-
BOOL
CShopManager::insertShop(CShop *_lpShop,int _iSerial)
{
	if (_iSerial==0xffff)	_iSerial=_lpShop->m_wSerial;

	if (m_aShop[_iSerial].m_wSerial	!=	0xffff)
		return	ERRMSG("error in CShopManager::insertShop","이미 등록된 상점 자리에 끼워넣기를 하려고 했어요오~ -o-");

	m_aShop[_iSerial].copy(_lpShop);
	m_aShop[_iSerial].m_wSerial	=	_iSerial;

	if (_iSerial	==	m_iRookie)
	{
		for (int i=_iSerial+1;i<dMAX_SHOP;i++)
		{
			if (m_aShop[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;
				break;
			}
		}
	}

	m_iCount++;

	return	TRUE;
}	//	CShopManager::insertShop(CShop *_lpShop,int _iSerial)

//
//	상점 제거
BOOL
CShopManager::removeShop(int _iIndex)
{
	if	(_iIndex < 0 || _iIndex >= dMAX_SHOP)
		return	FALSE;
	if	(m_aShop[_iIndex].m_wSerial	==	0xffff)
		return	FALSE;

	m_aShop[_iIndex].reset();

	if (_iIndex	<=	m_iRookie)	m_iRookie	=	_iIndex;

	m_iCount--;

	return	TRUE;
}	//	CShopManager::removeShop(int _iIndex)

//
//	카운트와 루키를 점검한다.
void
CShopManager::rebuild()
{
	m_iCount	=	0;
	m_iRookie	=	0xffff;

	for (int i=0;i<dMAX_SHOP;i++)
	{
		if (m_aShop[i].m_wSerial	==	0xffff)
		{
			if (m_iRookie==0xffff)	m_iRookie	=	i;
			continue;
		}

		m_iCount++;
	}
}	//	CShopManager::rebuild()