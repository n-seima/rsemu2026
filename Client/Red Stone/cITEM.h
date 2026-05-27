#ifndef _classITEM_H
#define _classITEM_H

#include "cNEMO.H"
#include "cWEAPON.H"
#include "CItemDefine.h"
#include "CItemEffectDefine.H"
#include "CQuest.H"
#include "playerJobCommon.H"

//	INC	- Item Name Color
#define	dINC_NORMAL							WHITE
#define	dINC_SUPER_UNIQUE					GOLDYELLOW
#define	dINC_UNIQUE							GOLDYELLOW
#define	dINC_RARE							BTBLUE
#define	dINC_LITTLE_GOOD					PURPLE

#define	dBORDER_OF_ITEM_AND_EQUIPMENT		100
#define	dBORDER_OF_BELT_FOR_USE_ITEM		100
#define	dDEFAULT_BELT_ITEM_RELOAD_TIME		8

#define	dDEFENSIVE_EQUIPMENT_COUNT			5
#define	dMAX_ITEM_ADDITIONAL_DATA			500

#define	dITEM_SHAPE_HEIGHT					34
#define	dITEM_SHAPE_WIDTH					34
#define	dITEM_SMALL_SHAPE_HEIGHT			24
#define	dITEM_SMALL_SHAPE_WIDTH				24
#define	dDEFAULT_SHIELD_RIGIDITY_TIME		4			//	디폴트 방패 경직 시간(방패로 블럭 했을때 그 자세 그대로 굳어-_-져있는 시간)

#define	dITEM_DRAG_GOLD						0xfffe
#define	dITEM_MONEY							0		//	돈 -o-
#define	dITEM_CARROT						1834	//	당근
#define	dITEM_FIELD_SHAPE_MONEY				152		//	돈 모양 -o-
#define	dITEM_COUNT_OF_MONEY_FIELD_SHAPE	4		//	돈 모양 -o-

#define	dITEM_KIND_ALL					0xffff		//	모든 종류의 아이템
#define	dITEM_KIND_WASTING				0xfffe		//	모든 종류의 아이템


#define	dOWN_ITEM_COUNT					42

#define	dMONSTER_ITEM_COUNT				10			//	몬스터가 가지고 있을 수 있는 아이템의 최대 수

#define	dITEM_UNIQUE_DATA_COUNT			6			//	유니크 데이터 수
#define	dITEM_UNIQUE_DATA_VALUE_COUNT	8
#define	dGENERATE_ITEM_DATA_COUNT		4
#define	dGENERATE_ITEM_DATA_VALUE_COUNT	4
#define	dITEM_PREFIX_COUNT				3

const	c_aCheckPartForProveEnableSex[16]	=	{16,16,16,16,16,16,16,16,17,17,17,17,17,17,17,17};	//	직업의 성별

#pragma pack(2)

class	cBasicItem : public CBasicItemData
{
public:
							cBasicItem()
							{
								m_iSerial	=	0xffff;
							}

	char					*getComment();
	
	static	int				GetItemByEquippedShape(int _iShape,int _iJob,int _iEquipPart);	//	착용한 모양과 직업을 기본으로 해서 아이템 구하기
	static	char			s_strComment[2048];
};

extern	cBasicItem	g_aBasicItem[dBASIC_ITEM_COUNT];
extern	int			g_iBasicItemCount;

#pragma	pack()

#pragma	pack(1)

class	cSpecialItem	:	public cSpecialItemDefine
{
};

class	cSetItem;
class	cItemSet;
class	cPieceItem;

class	cItem : public CItemDefine
{
	char*					_getItemTooltip(int _iPieceIndex,int _iPieceCount,BOOL _bIsWantAfterEquipInfo);

public:
							cItem()							{reset();}

						
	char*					getItemTooltip(int _iPieceIndex=-1,int _iPieceCount=-1,BOOL _bIsWantAfterEquipInfo=FALSE);							//	툴팁을 얻어온다.
	char*					getBreedingRecordBookTooltip();
	char*					getMiniPetPouchTooltip();
	char*					getJewelTooltip(int _iRemainTime);
	DWORD					getPrice(int _iCount);					//	가격 계산
	char*					getName(BOOL _bIsWantColor=TRUE);								//	아이템 이름
	char*					getBookName();
	char*					getCategoryName();
	CQuestItem*				getAssignedQuest();
	int						getBeginPageInBook();
	int						getEndPageInBook();
	int						getAvailPageCount();
	int						getTotalPageCount();
	int						getCompleteBookPencentage();
	BOOL					isAvailPage(int _iCategory,int _iPage);
	BOOL					isAvailPage(int _iPage);
	int						getCarvingLevel();
	int						getSetCarvingLevel();
	int						getSetCarvingChance();
	int						getIncreaseCarvingChance();
	BOOL					increaseCarvingLevel(int _iValue);
	BOOL					setCarvingLevel(int _iValue);
	int						getEffectMiniPetFeedType();
	int						getMiniPetFeedExp();
	int						getSetMiniPetType();
	void					increaseMinute(int _iMinute);

	inline	cSpecialItem*	getSpecialItem()
	{
		if	(isExceptionItem())
			return	NULL;
		
		if	(m_bf1IsSpecialItem	==	FALSE)
			return	NULL;

		return	(cSpecialItem*)this;
	}
	inline	cCoupleRingDefine*	getCoupleRing()
	{
		if	(isCoupleRing()	==	FALSE)
			return	NULL;

		return	(cCoupleRingDefine*)this;
	}
	inline	BOOL	isExpireCoupleRing()
	{
		cBasicItem	*lpItem	=	getBasicItem();
		
		if	(!lpItem)	
			return	FALSE;
		
		if	(lpItem->m_aGenerateData[0].m_wEffect	!=	eIE_COUPLE_RING &&
			lpItem->m_aGenerateData[0].m_wEffect	!=	eIE_PRESENT_COUPLE_RING	&&
			lpItem->m_aGenerateData[0].m_wEffect	!=	eIE_NOT_DEFINE_COUPLE_COUPLE_RING	)
			return FALSE;

		if(((cCoupleRingDefine*)this)->m_year)
			return TRUE;

		return FALSE;
	}
	inline	cBookItemDefine	*getBook()
	{
		if	(isBook())
			return	(cBookItemDefine	*)this;

		return	NULL;
	}
	inline	cBookCategoryItemDefine	*getAddCategoryToBookItem()
	{
		if	(isAddCategoryToBookItem())
			return	(cBookCategoryItemDefine	*)this;

		return	NULL;
	}

	inline	cBookPageItemDefine	*getAddPageToBookItem()
	{
		if	(isAddPageToBookItem())
			return	(cBookPageItemDefine	*)this;

		return	NULL;
	}

	inline	BOOL			isPrincessWeapon()
	{
		if	(m_wBaseItem	>=	dBASIC_ITEM_COUNT)
			return	NULL;

		cBasicItem	*lpItem	=	&g_aBasicItem[m_wBaseItem];

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_PRINCESS_WEAPON)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL		isGuildHallPortal()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if (!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_GUILD_HALL_PORTAL)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isBook()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_BOOK)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isAddCategoryToBookItem()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_ADD_CATEGORY)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isAddPageToBookItem()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_ADD_PAGE)
			return	TRUE;

		return	FALSE;
	}

	void					changeToCoupleRing(char *_lpstrTargetName=NULL);
	BOOL					isShopSpecialItem();
	BOOL					isCanNotTradeItem();
	BOOL					isReversionItemWhenEauip();
	BOOL					isUnknownItem();
	BOOL					isItemPiece();
	BOOL					isSetItem();
	int						getJudgePrice();
	BOOL					isSecretDungeonKey();
	BOOL					isCoupleRing(BOOL _bIsCheckUseAble=FALSE);

	BOOL					isBottomlessBox();
	BOOL					isCanNotDestroyItem();
	BOOL					isPresentCoupleRing();
	BOOL					isNotDefineCoupleCoupleRing();

	BOOL					isIncreaseSelectPowerItem();
	BOOL					generatePieceItem(cItem *_lpItem);
	cPieceItem*				getPieceItem();
	cItemSet*				getItemSet();
	cSetItem*				getSetItem();
	cPrincessWeaponDefine*	getPrincessWeapon();
	int						getWeaponColorizeEffect();				//	무기의 컬러라이즈 효과
	int						getArmorColorizeEffect();				//	장비의 컬러라이즈 효과
	BOOL					isWeapon(cBasicItem *_lpBasicItem);
	BOOL					isWeapon();

	BOOL					isLimitTermItem();
	BOOL					isExistSpecificEffect(int _iEffect);
	int						getExpandInventorySize();
	int						getCoolTime();
	int						getCoolTime(cBasicItem	*_lpBasicItem);

	inline	BOOL			isExceptionItem()
	{
		cBasicItem	*lpBasicItem	=	getBasicItem();

		if	(!lpBasicItem)
			return	FALSE;

		return	lpBasicItem->m_bIsExceptionItem;
	}


	inline	int				getPrefixCount()
	{
		if	(isExceptionItem())
			return	0;

		int	iPrefixCount	=	0;

		for (int i=0;i<dITEM_PREFIX_COUNT;i++)
		{
			if (m_aPrefix[i].m_wPrefix	==	0xffff)	break;

			iPrefixCount++;
		}

		return	iPrefixCount;
	}
	int						getNounPrefixCount();
	int						getAdjectivePrefixCount();

	inline	int				getIconShape()
	{
		CRY(getBasicItem()	==	NULL,"cry in cItem::getIconShape - 비... 비었어~~");

		return	getBasicItem(TRUE)->m_wIconShape;
	}

	inline	int				getFieldShape()
	{
		CRY(getBasicItem()	==	NULL,"cry in cItem::getFieldShape - 비... 비었어~~");

		return	getBasicItem()->m_wFieldShape;
	}

	inline	int				getItemType()	//	기본 타입
	{
		CRY(getBasicItem()	==	NULL,"cry in cItem::getStackLimit - 비... 비었어~~");

		return	getBasicItem(TRUE)->m_wKind;
	}


	BOOL					isUpgradeAbleItem(int _iPrefixSlot);
	BOOL					upgradeItem(int _iPrefix,int _iValue1,int _iValue2,BOOL _bIsWantTest=FALSE);	//	업그레이드는 안하고 가능한지만 확인

	BOOL					isManExclusiveItem();
	BOOL					isWomanExclusiveItem();
	BOOL					isEnableJob(int _iJob);
	BOOL					isAllJobItem();
	BOOL					isLimitJobItem();

	int						getRequireLevelByPrefix();	//	접두사로 인해 착용/사용에 필요한 레벨
	int						getRequireLevel();			//	착용/사용에 필요한 레벨
	BOOL					isCanUseLevel(int _iLevel, int _iLimitEquipmentLevelOverPoint = 0);
	BOOL					isCanNotFeedToPetItem();
	BOOL					isInfinityBullet();		//	무한 탄환이냐?
	int						getFullRepairPrice();
	int						getUnlockDoorLevel();
	int						getUnlockArcaLevel();
	BOOL					isBlockLightPocket();

	inline	BOOL			checkFirstEffect(int _iEffect)
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	_iEffect)
			return	TRUE;

		return	FALSE;
	}

	BOOL					isSealedMiniPet();

	inline	BOOL			isBreedingRecordBook()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_BREEDING_RECORD)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isGateGlove()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_JUMP_FIELD_USE_WORLD_MAP)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isVerticalBanner()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_PUT_VERTICAL_BANNER)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isHorizonBanner()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_PUT_HORIZON_BANNER)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isBroken()
	{
		if	(isExceptionItem())
			return	FALSE;

		return	m_isBroken;
	}

	inline	BOOL			isBreakdownAbleItem()
	{
		if	(isDXItem() + isUltimateItem())
			return	TRUE;

		return	FALSE;
	}

	BOOL					isDXItem();
	BOOL					isUltimateItem();
	BOOL					isItemPack();
	BOOL					isExtraItem(BOOL _bIsExceptChargeItem=FALSE);
	BOOL					isMagicCarpetEmblem();

	BOOL					isPolisher();
	BOOL					isMiniPetAwakenItem50();
	BOOL					isMiniPetAwakenItem100();
	BOOL					isMassivePolisher();

	int						getExtraItemEfficiency();
	BOOL					isBreakdownItem();
	inline	BOOL			isConsumeItem()
	{
		cBasicItem	*lpBasicItem	=	getBasicItem();

		if (!lpBasicItem)
			return	FALSE;

		return	1-lpBasicItem->m_attr.isCanNotUseableItem;
	}
	inline	BOOL			isBeltStackAbleItem()		//	벨트에 넣을 수 있는 아이템이냐?
	{
		cBasicItem	*lpBasicItem	=	getBasicItem();

		if (!lpBasicItem)
			return	FALSE;

		return	lpBasicItem->m_attr.isEquipableToBelt;
	}
	inline	BOOL			isCanNotDropItem()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if (!lpItem)
			return	TRUE;

		return	lpItem->m_attr.isCanNotDropItem;
	}
	inline	BOOL			isStackAbleItem()		//	겹칠수 있는 아이템이냐?
	{
		cBasicItem	*lpBasicItem	=	getBasicItem();

		if (!lpBasicItem)
			return	FALSE;

		if (lpBasicItem->m_wStackLimit > 1)	return	TRUE;

		return	FALSE;
	}
	inline	int				getStackLimit()	//	몇개나 겹칠 수 있는겨?
	{
		cBasicItem	*lpItem		=	getBasicItem();
		CRY(!lpItem,"cry in cItem::getStackLimit - 크아아!! 빈 아이템을 왜 검색하는겨!!");

		return	lpItem->m_wStackLimit;
	}

	inline	int				isFull(int _iCount=0xffff)		//	꽉 찼냐?
	{
		if	(isExceptionItem())
			return	TRUE;

		cBasicItem	*lpItem		=	getBasicItem();
		CRY(!lpItem,"cry in cItem::getStackLimit - 크아아!! 빈 아이템을 왜 검색하는겨!!");

		if (_iCount	==	0xffff)	_iCount	=	lpItem->m_wStackLimit;

		if (_iCount	<=	m_bCount)	return	TRUE;

		return	FALSE;
	}

	inline	void			setBasicItem(cBasicItem *_lpItem,int _iCount,DWORD _dwSerial)	//	기본 아이템 설정
	{
		m_wBaseItem		=	_lpItem->m_iSerial;
		m_bCount		=	_iCount;
		m_bDurability	=	(BYTE)_lpItem->getDurability();
		m_dwSerial		=	_dwSerial;
	}

	inline	void			set(cItem *_lpItem)
	{
		if (!_lpItem)	reset();
		else			memcpy(this,_lpItem,sizeof(cItem));
	}//	아이템 복사
	inline	void			copy(cItem *_lpItem)
	{
		if (!_lpItem)	reset();
		else			memcpy(this,_lpItem,sizeof(cItem));
	}//	아이템 복사
	inline	void			copy(CItemBaseInfo *_lpItem)
	{
		reset();
		if (_lpItem)	memcpy(this,_lpItem,sizeof(CItemBaseInfo));
	}//	아이템 복사
	inline	void			swapItem(cItem *_lpItem)//바꿔치기
	{	cItem	tempItem;

		tempItem.copy(this);
		this->copy(_lpItem);
		_lpItem->copy(&tempItem);
	}

	inline	BOOL			isSameItem(cItem *_lpItem)	//	똑같은 아이템이다.(당근 시리얼 빼고..)
	{
		if (isInfinityBullet())
			return	FALSE;
		
		if (m_wBaseItem				!=	_lpItem->m_wBaseItem				)	return	FALSE;
		if (m_aOption[0]			!=	_lpItem->m_aOption[0]				)	return	FALSE;
		if (m_aOption[1]			!=	_lpItem->m_aOption[1]				)	return	FALSE;
		if (m_aPrefix[0].m_wPrefix	!=	_lpItem->m_aPrefix[0].m_wPrefix		)	return	FALSE;
		if (m_aPrefix[0].m_aValue[0]!=	_lpItem->m_aPrefix[0].m_aValue[0]	)	return	FALSE;
		if (m_aPrefix[0].m_aValue[1]!=	_lpItem->m_aPrefix[0].m_aValue[1]	)	return	FALSE;
		if (m_aPrefix[1].m_wPrefix	!=	_lpItem->m_aPrefix[1].m_wPrefix		)	return	FALSE;
		if (m_aPrefix[1].m_aValue[0]!=	_lpItem->m_aPrefix[1].m_aValue[0]	)	return	FALSE;
		if (m_aPrefix[1].m_aValue[1]!=	_lpItem->m_aPrefix[1].m_aValue[1]	)	return	FALSE;
		if (m_aPrefix[2].m_wPrefix	!=	_lpItem->m_aPrefix[2].m_wPrefix		)	return	FALSE;
		if (m_aPrefix[2].m_aValue[0]!=	_lpItem->m_aPrefix[2].m_aValue[0]	)	return	FALSE;
		if (m_aPrefix[2].m_aValue[1]!=	_lpItem->m_aPrefix[2].m_aValue[1]	)	return	FALSE;

		if (((CItemDefineForValue *)this)->m_dwValue	!=	((CItemDefineForValue *)_lpItem)->m_dwValue)	return	FALSE;

		return	TRUE;
	}

	inline	BOOL			isExactlySameItem(cItem *_lpItem)	//	완전히 똑같은 아이템이다.(당근 시리얼 빼고..)
	{
		if (m_wBaseItem				!=	_lpItem->m_wBaseItem				)	return	FALSE;
		if (m_dwSerial				!=	_lpItem->m_dwSerial					)	return	FALSE;
		if (m_aOption[0]			!=	_lpItem->m_aOption[0]				)	return	FALSE;
		if (m_aOption[1]			!=	_lpItem->m_aOption[1]				)	return	FALSE;
		if (m_aPrefix[0].m_wPrefix	!=	_lpItem->m_aPrefix[0].m_wPrefix		)	return	FALSE;
		if (m_aPrefix[0].m_aValue[0]!=	_lpItem->m_aPrefix[0].m_aValue[0]	)	return	FALSE;
		if (m_aPrefix[0].m_aValue[1]!=	_lpItem->m_aPrefix[0].m_aValue[1]	)	return	FALSE;
		if (m_aPrefix[1].m_wPrefix	!=	_lpItem->m_aPrefix[1].m_wPrefix		)	return	FALSE;
		if (m_aPrefix[1].m_aValue[0]!=	_lpItem->m_aPrefix[1].m_aValue[0]	)	return	FALSE;
		if (m_aPrefix[1].m_aValue[1]!=	_lpItem->m_aPrefix[1].m_aValue[1]	)	return	FALSE;
		if (m_aPrefix[2].m_wPrefix	!=	_lpItem->m_aPrefix[2].m_wPrefix		)	return	FALSE;
		if (m_aPrefix[2].m_aValue[0]!=	_lpItem->m_aPrefix[2].m_aValue[0]	)	return	FALSE;
		if (m_aPrefix[2].m_aValue[1]!=	_lpItem->m_aPrefix[2].m_aValue[1]	)	return	FALSE;

		if (((CItemDefineForValue *)this)->m_dwValue	!=	((CItemDefineForValue *)_lpItem)->m_dwValue)	return	FALSE;

		return	TRUE;
	}

	inline	BOOL			is4FingerRing()
	{
		if (m_wBaseItem	== 115)	return	TRUE;
		if (m_wBaseItem	== 116)	return	TRUE;
		if (m_wBaseItem	== 335)	return	TRUE;
		if (m_wBaseItem	== 336)	return	TRUE;

		return	FALSE;		
	}
	
	inline	void			reset()	//	초기화
	{
		CItemDefine::reset();
	}

	BOOL					isQuestItem();
	BOOL					isPremiumItem();
	BOOL					isSuperUniqueItem();//이건  슈퍼유니크냐?
	BOOL					isUniqueItem();//이건 유니크냐?
	BOOL					isRareItem();	
	BOOL					isLittleGoodItem();
	BOOL					isNormalItem();//이건 보통 아이템이냐

	DWORD					getNameColor();	//	출력시 이름 컬러
	cBasicItem*				getBasicItem(BOOL _bIsWantPrincessWeaponBasicItem=FALSE);

	inline	BOOL			decreaseCount(int _iCount,BOOL _isBeltItem=FALSE)	//	카운트 줄이기
	{
		if	(isExtraItem() || isMagicCarpetEmblem() || isExceptionItem())
		{
			reset();
			return	TRUE;
		}

		if	(_iCount>m_bCount)
			m_bCount	=	0;
		else
		{
			m_bCount	-=	_iCount;
			_iCount		=	0;
		}

		if	(m_bCount==0	&&	!_isBeltItem)
			reset();

		if	(_iCount	==	0	)
			return	TRUE;

		return	FALSE;
	}

	BOOL					addItem(cItem *_lpItem,int _iLimitCount=0xffff);	//	아이템 합치기..?
	int						getCheckSum();	//	체크섬 구하기
	int						getExpForGuildPet();
	int						getExpForMiniPet();
	BOOL					isMiniPetFeed();	//	미니펫 먹이냐?
	int						getMiniPetFeedType();

	static		void		BuildData();
	static		void		Reset();
	static		int			GetItemIndexByName(char *_lpstrName);
	static		void		GenerateItemByBaseItem(cItem *_lpItem,int _iBaseItem,int _iCount,int _iEfficiency=-1);
	static		cSTRING		s_strTooltip;
	static		DWORD		s_dwGeneatedToolTipSerial;
	static		int			s_iGeneatedToolTipCheckSum;
	static		int			s_iGeneatedToolTipBaseSerial;
	static		int			s_iCoupleRing;

};

class	CCustomItem : public CCustomItemDefine
{
public:
	BOOL				generateItem(cItem *_lpItem);
};

#pragma	pack()

#pragma	pack(2)

class	CPlayerEquipment
{
public:
	cItem			m_aEquip[dEQUIPMENT_PART_COUNT];
	cItem			m_aItems[dOWN_ITEM_COUNT];

					CPlayerEquipment()	{reset();}

	inline	void	reset()	//	초기화
	{
		int		i;

		for (i=0;i<dEQUIPMENT_PART_COUNT;i++)	m_aEquip[i].reset();
		for (i=0;i<dOWN_ITEM_COUNT;i++)			m_aItems[i].reset();
	}
};

class	CCustomItemManager
{
public:
	int						m_iCount;
	int						m_iSize;
	CCustomItem				*m_pItems;

							CCustomItemManager()
							{
								m_pItems	=	NULL;
							}
							~CCustomItemManager()
							{
								pKILL(m_pItems);
							}

	void					init(int _iSize)
	{
		pKILL(m_pItems);
		m_iSize		=	_iSize;

		if (_iSize	==	0)	return;

		m_pItems	=	new CCustomItem [_iSize];
	}

	inline	CCustomItem*	getItemBay(){return	m_pItems;}
	inline	CCustomItem*	getItem(int	_iIndex)
	{
		if (_iIndex < 0 ||  _iIndex >= m_iSize)	
			return	NULL;
		if (m_pItems[_iIndex].m_wBaseItem == 0xffff		)
			return	NULL;

		return	&m_pItems[_iIndex];
	}

	BOOL					generateItem(int _iItemIndex,cItem *_lpItem);
	void					reset()
	{
		m_iCount	=	0;

		pKILL(m_pItems);
	}

};

#pragma pack()

extern	cItem			g_itemGold;	//	금.
extern	cItem			g_itemCarrot;	//	금.

extern	const	int		c_aEquipPlace[];
extern	const	int		c_aCategoryByEquipment[];

extern	cBasicItem		*g_alpDuelBuffItem[4];

extern	CCustomItemManager	g_premiumItem;

extern	char	g_astrItemPackListName[dMAX_ITEM_PACK_COUNT][32];

#endif