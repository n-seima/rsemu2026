#ifndef _classShop_h
#define	_classShop_h

#include "SFC.H"
#include "cITEM.H"

#define	dSHOP_NAME_LENGTH		60
#define	dMAX_SHOP_ITEM_COUNT	100
#define	dMAX_SHOP				40

class	CShopItem	:	public CItemBaseInfo
{
public:
	WORD		m_wSerial;
	WORD		m_bf1IsRandomPrefix		:	1;
	WORD		m_bf15UpkeepHour		:	15;
	WORD		m_bf10ChangeLimitLevel	:	10;
	WORD				:	0;

	WORD		m_wGenerateQuality,m_wPrefixQuality;
	DWORD		m_dwPrice;			//	가격.. 가격이 0이면 아이템 자체의 가격 계산식으로 가격을 설정한다.

				CShopItem();

	void		reset();			//	초기화
	void		copy(CShopItem *_lpItem);

	inline	int	getPrefixCount()
	{
		int	iCount	=0;

		for	(int i=0;i<dITEM_PREFIX_COUNT;i++)
			if	(m_aPrefix[i].m_wPrefix	!=	0xffff)
				iCount++;

		return	iCount;
	}
	BOOL		isSame(CShopItem* _lpShopItem)
	{
		if(!((CItemBaseInfo*)this)->isSame((CItemBaseInfo*)_lpShopItem))
			return FALSE;
		
		if(m_wSerial != _lpShopItem->m_wSerial)
			return FALSE;
		if(m_bf1IsRandomPrefix != _lpShopItem->m_bf1IsRandomPrefix)
			return FALSE;
		if(m_bf15UpkeepHour != _lpShopItem->m_bf15UpkeepHour)
			return FALSE;
		if(m_bf10ChangeLimitLevel != _lpShopItem->m_bf10ChangeLimitLevel)
			return FALSE;
		if(m_wGenerateQuality != _lpShopItem->m_wGenerateQuality)
			return FALSE;
		if(m_wPrefixQuality != _lpShopItem->m_wPrefixQuality)
			return FALSE;
		if(m_dwPrice != _lpShopItem->m_dwPrice)
			return FALSE;

		return TRUE;
	}
};	//	class	CShopItem	:	public cITEM

class	cShopNameInfo
{	// 마지막 수정일 : 09.10.16
public:
	char						m_strName[60-4];//	가게 이름
	DWORD				m_bf1IsDuelPointShop	:	1;
	DWORD				m_bf1IsLevelPerGoldShop	:	1;
	DWORD				m_bf15LevelPerGoldValue	:	15;
	DWORD				m_bf1IsNotBuyItem		:	1;		// 상점 구입 불가.
	DWORD				:	0;			
};

//
//	가게-o-r~~
class	CShop
{	// 마지막 수정일 : 09.10.16
public:
	WORD				m_wSerial;//	샵 시리얼
	WORD				m_wPriceFactor;	//	게이머가 상점에 아이템을 팔때 몇%의 가격으로 쳐주는가?
	char				m_strName[dSHOP_NAME_LENGTH-4];//	가게 이름

	DWORD				m_bf1IsDuelPointShop	:	1;	//1
	DWORD				m_bf1IsLevelPerGoldShop	:	1;	//2
	DWORD				m_bf15LevelPerGoldValue	:	15;	//17	
	DWORD				m_bf1IsNotBuyItem		:	1;	//18 상점 구입불가.
	DWORD				:	0;

	DWORD				m_dwLimitPrice;					//	게이머가 상점에 아이템을 팔때 최고 가격
	
	WORD				m_TokkenShop;					//	토큰샵정보.
	cDEN<CShopItem>		m_denItem;

						CShop();
						~CShop();

	inline	CShopItem	*get(int _iIndex)	{return m_denItem.get(_iIndex);}
	inline	int			getMaxCount()		{return	m_denItem.getMaxCount();}// 최대 카운트
	inline	int			getCount()			{return	m_denItem.getCount();}// 카운트

	void				reset();//초기화
	int					addItem(CShopItem *_lpItem);//아이템 추가
	BOOL				insertItem(CShopItem *_lpItem);//아이템 추가
	BOOL				removeItem(int _iIndex);//아이템 제거

	void				copy(CShop *_lpShop); // 복사
	BOOL				changeItemPlace(int _iPre,int _iNext);	//	 아이템 위치 변경
};	//	class	CShop

//
//	상점
class	CShopManager
{
public:
	int				m_iCount,m_iRookie;
	CShop			m_aShop[dMAX_SHOP];

					CShopManager();
					~CShopManager();
	
	inline	CShop	*get(int _iIndex)	
	{
		if	(_iIndex < 0 || _iIndex >= dMAX_SHOP)
			return NULL;
		if	(m_aShop[_iIndex].m_wSerial	==	0xffff)
			return	NULL;

		return &m_aShop[_iIndex];
	}

	inline	int		getCount()		{return	m_iCount;}// 카운트

	void			reset();
	BOOL			addShop(CShop *_lpShop);
	BOOL			insertShop(CShop *_lpShop,int _iSerial=0xffff);
	BOOL			removeShop(int _iIndex);

	void			rebuild();	//	카운트와 루키를 점검한다.

};	//	class	CShop

#endif