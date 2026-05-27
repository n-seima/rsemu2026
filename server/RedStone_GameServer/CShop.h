#ifndef _classShop_h
#define	_classShop_h

#include "SFC.H"
#include "cItem.H"

#define	dSHOP_NAME_LENGTH		32
#define	dMAX_SHOP_ITEM_COUNT	100
#define	dMAX_SHOP				20

//
//	보관 하고 있어야 할 데이터
class	CShopGenerateItemInfo	:	public CItemBaseInfo
{
public:
	WORD		m_wSerial;
	WORD		m_bf1IsRandomPrefix	:	1;
	WORD		m_bf15UpkeepHour	:	15;
	WORD		m_bf10ChangeLimitLevel	:	10;
	WORD					:	0;
	WORD		m_wGenerateQuality,m_wPrefixQuality;
	DWORD		m_dwPrice;			//	가격.. 가격이 0이면 아이템 자체의 가격 계산식으로 가격을 설정한다.

				CShopGenerateItemInfo()
				{
					reset();
				}

	void		reset()			//	초기화
	{
		memset(this,0,sizeof(CShopGenerateItemInfo));
		memset(m_aPrefix,0xff,sizeof(m_aPrefix));

		m_wBaseItem			=	0xffff;

		m_bCount			=	1;
		m_wSerial			=	0xffff;
		m_wGenerateQuality	=	100;
		m_wPrefixQuality	=	50;
		m_bf1IsRandomPrefix	=	FALSE;	//	접두사가 랜덤으로 붙을껴?
		m_bf15UpkeepHour	=	0;
		m_bf10ChangeLimitLevel	=	0;
		m_dwPrice			=	0;		//	가격.. 가격이 0이면 아이템 자체의 가격 계산식으로 가격을 설정한다.
	}
	
	inline	int	getPrefixCount()
	{
		int	iCount	=0;

		for (int i=0;i<dITEM_PREFIX_COUNT;i++)
			if (m_aPrefix[i].m_wPrefix	!=	0xffff)	iCount++;

		return	iCount;
	}
};	//	class	CShopItem	:	public cItem

class	cShopNameInfo
{	// 마지막 수정일 : 09.10.26
public:
	char						m_strName[60-4];//	가게 이름
	DWORD						m_bf1IsDuelPointShop	:	1;	// 1
	DWORD						m_bf1IsLevelPerGoldShop	:	1;	// 2
	DWORD						m_bf15LevelPerGoldValue	:	15;	// 17
	DWORD						m_bf1IsNotBuyItem		:	1;	// 18	아이템 구입 불가.
	DWORD								:	0;
};
//
//	가게-o-r~~
class	CShop
{// 마지막 수정일 : 09.10.26
public:
	WORD						m_wSerial,m_wItemCount,m_wPriceFactor;//	샵 시리얼
	char						m_strName[60-4];//	가게 이름
	DWORD						m_bf1IsDuelPointShop	:	1;
	DWORD						m_bf1IsLevelPerGoldShop	:	1;
	DWORD						m_bf15LevelPerGoldValue	:	15;
	DWORD						m_bf1IsNotBuyItem		:	1;	// 18	아이템 구입 불가.
	DWORD								:	0;
	WORD						m_wOrgItemCount;

	DWORD						m_dwLimitPrice;
	cItem						*m_pSellItemInfo;		//	판매 할 아이템 정보
	CShopGenerateItemInfo		*m_pGenerateItemInfo;	//	아이템 생성 정보
	WORD						m_TokenShop;			//	토큰 샵인가?	0xffff가 초기 값이다.

								CShop()
								{
									m_pSellItemInfo		=	NULL;
									m_pGenerateItemInfo	=	NULL;
									m_wItemCount		=	0;
									m_TokenShop		=	0xffff;
								}	//	CShop::CShop()

								~CShop()
								{
									reset();
								}	//	CShop::~CShop()

	void							reset()//초기화
	{
		m_wSerial		=	0xffff;
		m_TokenShop	=	0xffff;
		pKILL(m_pSellItemInfo);
		pKILL(m_pGenerateItemInfo);
		strcpy(m_strName,"");

		m_bf1IsDuelPointShop	=	FALSE;
		m_bf1IsLevelPerGoldShop	=	0;
		m_bf15LevelPerGoldValue	=	0;
		m_bf1IsNotBuyItem		=	FALSE;
	}

	inline	CShopGenerateItemInfo	*getGenerateItemInfo(int _iIndex)
	{
		if (_iIndex < 0 || _iIndex >= m_wItemCount)	return	NULL;

		return	&m_pGenerateItemInfo[_iIndex];
	}

	BOOL							getSellItemInfo(int _iIndex,cItem *_lpItem);
	void							alloc(int _iItemCount=0xffff)
	{
		if	(_iItemCount==0xffff)
			_iItemCount = m_wItemCount;

		m_wItemCount	=	_iItemCount;

		pKILL(m_pSellItemInfo);
		pKILL(m_pGenerateItemInfo);

		m_pGenerateItemInfo	=	new CShopGenerateItemInfo	[_iItemCount];
		m_pSellItemInfo		=	new cItem [_iItemCount];
	}
	void							generateItem();	//	판매 할 아이템 생성
};	//	class	CShop
#define dBADGE_OF_TRADER_ITEM_LIST_COUNT 6
//
//	상점
class	CShopManager
{
public:
	int				m_iCount;
	CShop			*m_pShop;

					CShopManager()
					{
						m_iCount	=	0;

						m_pShop		=	NULL;
					}

					~CShopManager()
					{
						reset();
					}	//	CShop::~CShop()
	
	inline	CShop	*get(int _iIndex)	
	{
		for (int i=0;i<m_iCount;i++)
			if (m_pShop[i].m_wSerial==_iIndex)	return	&m_pShop[i];

		return	NULL;
	}

	void			generateItem()
	{
		for (int i=0;i<m_iCount;i++)
			if (m_pShop[i].m_wSerial!=0xffff)	m_pShop[i].generateItem();
	}

	void			reset()
	{
		m_iCount	=	0;

		pKILL(m_pShop);
	}
	DWORD			getBadgeOfTraderItemSerial(int _iIndex);
	DWORD			getBadgeOfTraderItemPrice(int _iIndex);
	DWORD			getBadgeOfTraderItemCount(int _iIndex);
	DWORD			getIsBadgeOfTraderItemOption(int _iIndex);
	DWORD			getBadgeOfTraderItemOption1Min(int _iIndex);
	DWORD			getBadgeOfTraderItemOption1Max(int _iIndex);
	DWORD			getBadgeOfTraderItemOption2Min(int _iIndex);
	DWORD			getBadgeOfTraderItemOption2Max(int _iIndex);
};	//	class	CShop

class	cPegeionPostShopItem
{
public:
	WORD		m_wBasicItem;
	WORD		m_wCount;
	int			m_iPrice;
	WORD		m_awOption[2];

	cPegeionPostShopItem()
	{
		m_wBasicItem	=	0xffff;
	}
};

class	cPegeionPostShop
{
public:
	int						m_iItemCount;
	cPegeionPostShopItem	m_aItemList[100];

	BOOL					init();
	cPegeionPostShopItem*	get(int _iIndex);
	BOOL					getItemForShop(cItem *_lpItem,int _iIndex,int _iCharge);
};

extern	cPegeionPostShop	g_pigeonPostShop;

#endif