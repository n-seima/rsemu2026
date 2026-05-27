#ifndef _classITEM_H
#define _classITEM_H

#include	"SFC.H"
#include	"trap.H"
#include	"cNPC.H"
#include	"CItemEffectDefine.H"
#include	"CItemDefine.h"

/*
//
//	Use Item Result
enum
{
	eUIR_OK								,//	성공!!
	eUIR_ROTTERY_TICKET_KWANG			,//	꽝인디유... -o-
	eUIR_ROTTERY_TICKET_OK				,//	당첨.. ㅠ.ㅜ

	eUIR_FAILED							,//	실패.. -o-
	eUIR_NO_MORE_ATTACH_PREFIX			,//	더이상 접두사를 붙일수 없어!!
	eUIR_IS_NOT_WEAPON					,//	무기가 아니다!!
	eUIR_NO_MORE_IMPROVE_WEAPON_DAMAGE	,//	이 이상 무기의 성능을 향상 시킬 수 없다.
	eUIR_IS_NOT_ARMOR					,//	갑옷이 아니다.
	eUIR_NO_MORE_IMPROVE_ARMOR_DEFENSE	,//	더 이상 갑옷의 성능을 향상 시킬 수 없다.
	eUIR_CAN_NOT_USE_ITEM				,//	사용할 수 없는 아이템이다.
	eUIR_CAN_NOT_USE_ON_DEST_ITEM		,//	거기에다 사용 할 수 없다!!
	eUIR_LACK_ABILITY					,//	이 아이템을 사용하기에는 능력이 부족하다.	
	eUIR_DEATH_BY_REACTION				,//	부작용으로 죽게 된다.-_-
	eUIR_STRANGE_OPTION_ITEM			,//	이상한 옵션이 붙어 있다.
	eUIR_IS_NOT_CONSUME_ITEM			,//	소비형 아이템이 아니다.
	eUIR_IS_NOT_CORRECT_STATUS			,//	그 아이템을 쓸 만한 상황이 아녀.. -o-
	eUIR_NOT_EMBODY_ITEM				,//	아직 구현이 안된 아이템

	eUIR_WRONG_ITEM_DATA				,//	아이템 데이터가 맞지 않는다.
	eUIR_TOO_FAR						,//	너무 멀다.
};
*/


class cITEM_PREFIX_MANAGER
{
public:
	int				m_iCount;

	int				getPrefixByDropLevel(int _iDropLevel,int _iGroup,int _iItemType,BOOL _bIsSpecialDropChance=FALSE);	//	접두사를 골라라~
	int				getLittleGoodPrefix(int _iDropLevel,int _iGroup,int _iItemType);	//	쪼까 좋은 접두사
	int				getPrefixByDiscernmentCode(int _iDiscernmentCode);	//	식별 코드로 접두사 찾기
	BOOL			revisePrefixInfo(CItemPrefixInfo *_lpPrefixInfo,int _iDiscernmentCode=0x7fffffff);
	int				getPrefixByType(int _iType,int _iLevel);
	int				getPrefixLevel(int _iPrefix);
	int				getPrefixCountByType(int _iType);
	BOOL			levelUpPrefix(CItemPrefixInfo *_lpPrefixInfo,int _iItemKind);
};

#pragma pack(2)

class	cBasicItem : public CBasicItemData
{
public:
		cBasicItem()
		{
			m_iSerial	=	-1;
		}
};

#pragma pack()

extern	cITEM_PREFIX_MANAGER	g_itemPrefix;
extern	cITEM_PREFIX			g_aItemPrefix[dMAX_ITEM_PREFIX_COUNT];
extern	cBasicItem				g_aBasicItem[dBASIC_ITEM_COUNT];

enum
{
	eRIR_NOTHING,
	eRIR_REVISE_INCORRECT_DATA,
	eRIR_REVISE_INCORRECT_PREFIX,
};
enum
{
	eSEM_NORMAL_DROP,			//	일반드랍
	eSEM_INVENTORY_DROP,		//	죽었을경우 케릭터 인벤토리에 드랍
	eSEM_ATTACK_INVENTORY_DROP,	//	때릴때마다 인벤토리 드랍
};
#pragma	pack(1)

#define	dMAX_ADD_EFFECT_COUNT	3

class	cPieceItem;
class	cItemSet;
class	cSetItem;

class	cSpecialItem	:	public cSpecialItemDefine
{
public:
	void					reset();
};

class	cBookItem		:	public cBookItemDefine
{
public:
	void					resetBook();
	void					writeCategory(int _iCategory);	//	카테고리 쓰기
	void					writePage(int _iCategory,int _iPage);	//	페이지 쓰기
	void					erasePage(int _iCategory,int _iPage);	//	페이지 쓰기
	BOOL					isWrittenCategory(int _iCategory,int _iPageCount=0);	//	이미 쓰여져 있는 카테고리이다
	BOOL					isWrittenPage(int _iCategory,int _iPage);	//	이미 쓰여진 페이지다
	int						getCompletePercentage();
};

class	cItem : public CItemDefine
{
public:
							cItem()							{reset();}

	int						getJudgePrice();
	DWORD					getPrice(int _iCount);	//	가격을 계산한다.
	BOOL					isExpiredItem();		//	기간 이 끝난 아이템
	BOOL					isLimitTermItem();			//	기간 제한이 있는 아이템
	BOOL					isExpiredCoupleRing();		//	기간제 커플링.

	BOOL					isIncreaseSelectPowerItem();
	BOOL					isBottomlessBox();
	BOOL					isCureStrangeStatusItem();
	BOOL					isCurePoisonStatusItem();
	BOOL					isCureDeclinePowerStatusItem();
	BOOL					isCureCurseStatusItem();
	BOOL					isCureAllWrongStatusItem();
	BOOL					isNeutralizeWrongStatusItem();
	inline	BOOL			isOwnSpecificEffectItem(int _iEffect)
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		for	(int i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
			if	(lpItem->m_aGenerateData[i].m_wEffect	==	_iEffect)
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

	inline	BOOL			isCoupleRing()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_COUPLE_RING)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isTimeStamp()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_TIME_STAMP)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isPresentCoupleRing()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_PRESENT_COUPLE_RING)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isNotDefineCoupleCoupleRing()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_NOT_DEFINE_COUPLE_COUPLE_RING)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isSecretDungeonKey()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_SECRET_DUNGEON_KEY)
			return	TRUE;

		return	FALSE;
	}
	
	inline	BOOL			isGateGlove()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if (!lpItem)	
			return	FALSE;

		if (lpItem->m_aGenerateData[0].m_wEffect	==	eIE_JUMP_FIELD_USE_WORLD_MAP)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isTransToOperatorBook()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_TRANS_TO_OPERATOR)
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

	inline	BOOL			isTreasureMap()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_TREASURE_MAP)
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
	
	inline	BOOL			isPrincessWeapon()
	{
		if	(m_wBaseItem	>=	0xffff)
			return	NULL;

		cBasicItem	*lpItem	=	&g_aBasicItem[m_wBaseItem];

		if	(!lpItem || lpItem->m_iSerial	==	0xffff)	
			return	FALSE;

		if (lpItem->m_aGenerateData[0].m_wEffect	==	eIE_PRINCESS_WEAPON)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isEventBalloon()
	{
		if	(m_wBaseItem	>=	0xffff)
			return	NULL;

		cBasicItem	*lpItem	=	&g_aBasicItem[m_wBaseItem];

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_EVENT_BALLOON)
			return	TRUE;
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_EVENT_BALLOON_INC_ITEM_DROP_CHANCE)
			return	TRUE;
		return	FALSE;
	}

	inline	BOOL			isBreedingRecordBook()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if (!lpItem)	
			return	FALSE;

		if (lpItem->m_aGenerateData[0].m_wEffect	==	eIE_BREEDING_RECORD)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isBlockLightPocket()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if (!lpItem)	
			return	FALSE;

		if (lpItem->m_aGenerateData[0].m_wEffect	==	eIE_SEAL_MINIPET)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isBreakdownAbleItem()
	{
		if	(isPrincessWeapon())
			return	FALSE;

		if	(isDXItem() + isUltimateItem())
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isBreakdownItem()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;
		
		if(c_aItemEquipPlace[lpItem->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpItem->m_wKind] == 0xffff)		//  변경..
			return	FALSE;

		if	(isBreakdownAbleItem()	==	FALSE)
			return	FALSE;

		if	(lpItem->getDurability() > m_bDurability)
			return	TRUE;

		return	FALSE;
	}
	inline	void			fullRepairDurability()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return;

		if	(isBreakdownAbleItem()	==	FALSE)
			return;

		m_bDurability	=	(BYTE)lpItem->getDurability();
	}

	BOOL					upgrade(int _iSlot);

	int						getCurrentDurability();
	int						getIncorrectPrefixCount();
	int						reviseIncorrectPrefix();	
	BOOL					isCheckListItem();	
	BOOL					isIncorrectJobItem();
	void					removeItemInCheckList();
	BOOL					makeMagicCarpetEmblem();

	int						getFullRepairPrice();
	BOOL					updateExtraItemEfficency();

	int						reviseIncorrectData(BOOL _bIsBelt=FALSE);
	BOOL					reviseSetItemIncorrectData();

	inline	int				getItemMagicalType()		//	아이템의 마법 타입-_-?
	{
		if (isExceptionItem())
			return	eMIT_NORMAL;

		if (isSuperUniqueItem())
			return eMIT_SUPER_UNIQUE;
		if (isUniqueItem())
			return eMIT_UNIQUE;
		if (isRareItem())
			return eMIT_RARE;
		if (isLittleGoodItem())
			return eMIT_LITTLE_GOOD;

		return	eMIT_NORMAL;
	}

	BOOL					isItemPack();
	BOOL					isDXItem();
	BOOL					isUltimateItem();	
	BOOL					isExtraItem();
	BOOL					isSpecialItem();
	BOOL					makeToSpecialItem();
	BOOL					isReversionItem();
	BOOL					isMagicCarpetEmblem();
	BOOL					isMergeEmblemItem();
	int						getCheckSum();	//	체크섬 구하기
	int						getUnlockDoorLevel();
	int						getUnlockArcaLevel();

	int						getExpForGuildPet();	//	길드 펫 한테 줄때 경험치
	int						getExpForMiniPet();
	int						getMiniPetFeedType();
	BOOL					isMiniPetFeed();
	BOOL					isMassivePolisher();
	BOOL					isPolisher();
	BOOL					isMiniPetAwakenItem50();
	BOOL					isMiniPetAwakenItem100();
	

	inline	BOOL			checkFirstEffect(int _iEffect)
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	_iEffect)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isGuildHallPortal()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_GUILD_HALL_PORTAL)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isOXQuizTicket()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_OX_QUIZ_TICKET)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isGuildBattleItem()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if (!lpItem)	
			return	FALSE;

		return	lpItem->m_attr.isGuildBattleItem;
	}

	inline	BOOL			is4FingerRing()
	{
		return	FALSE;
	}

	inline	int				getPrefixCount()
	{
		if (isExceptionItem())
			return	0;

		int	iPrefixCount	=	0;

		for (iPrefixCount=0;iPrefixCount<dITEM_PREFIX_COUNT;)
		{
			if (m_aPrefix[iPrefixCount].m_wPrefix	!=	0xffff)
				iPrefixCount++;
			else
				break;
		}

		return	iPrefixCount;
	}

	BOOL					isCanNotFeedToPetItem();
	BOOL					isCanNotStoreBankItem();
	BOOL					isCanNotTradeItem();
	BOOL					isReversionItemWhenEauip();
	BOOL					isShopSpecialItem();	//	상점제 특수 아이템
	inline	BOOL			isCanNotDropItem()
	{
		cBasicItem	*lpItem	=	getBasicItem();

		if (!lpItem)
			return	TRUE;

		return	lpItem->m_attr.isCanNotDropItem;
	}
	BOOL					isCanNotDestroyItem();
	inline	int				getItemType()	//	기본 타입
	{
		CRY(getBasicItem()	==	NULL,"cry in cItem::getStackLimit - 비... 비었어~~");

		return	getBasicItem()->m_wKind;
	}
	inline	int				getStackLimit()	//	몇개나 겹칠 수 있는겨?
	{
		cBasicItem	*lpItem		=	getBasicItem();
		CRY(!lpItem,"cry in cItem::getStackLimit - 크아아!! 빈 아이템을 왜 검색하는겨!!");

		return	lpItem->m_wStackLimit;
	}

	BOOL					isInfinityBullet();		//	무한 탄환이냐?

	int						getFixedStrength();
	int						getFixedAgility();
	int						getFixedWisdom();
	int						getFixedIntelligence();
	int						getFixedConstitution();
	int						getFixedCharisma();
	int						getFixedLuck();

	char*					getName();

	BOOL					isStackAbleItem();		//	겹칠수 있는 아이템이냐?
	
	inline	BOOL			isBeltStackAbleItem()		//	벨트에 넣을 수 있는 아이템이냐?
	{
		cBasicItem	*lpBasicItem	=	getBasicItem();

		if (!lpBasicItem)
			return	FALSE;

		return	lpBasicItem->m_attr.isEquipableToBelt;
	}

	inline	BOOL			isConsumeItem()
	{
		cBasicItem	*lpBasicItem	=	getBasicItem();

		if (!lpBasicItem)
			return	FALSE;

		return	1-lpBasicItem->m_attr.isCanNotUseableItem;
	}
	
	inline	int				isFull(int _iCount=0xffff)		//	꽉 찼냐?
	{
		if (isExceptionItem())
			return	TRUE;

		cBasicItem	*lpItem		=	getBasicItem();
		CRY(!lpItem,"cry in cItem::getStackLimit - 크아아!! 빈 아이템을 왜 검색하는겨!!");

		if (_iCount	==	0xffff)	_iCount	=	lpItem->m_wStackLimit;

		if (_iCount	<=	m_bCount)
			return	TRUE;

		return	FALSE;
	}

	inline	int				getItemSpeed()	{return	getBasicItem(TRUE)->m_wSpeed;}
	inline	void			set(cItem *_lpItem)	{memcpy(this,_lpItem,sizeof(cItem));}//	아이템 복 사
	inline	void			copy(cItem *_lpItem){memcpy(this,_lpItem,sizeof(cItem));}//	아이템 복 사
	inline	void			swapItem(cItem *_lpItem)//바꿔치기
	{	
		cItem	tempItem;

		tempItem.copy(this);
		this->copy(_lpItem);
		_lpItem->copy(&tempItem);
	}

	inline	BOOL			isSameItem(cItem *_lpItem)	//	똑같은 아이템이다.(당근 시리얼 빼고..)
	{
		if (isExceptionItem())
			return	FALSE;

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

	inline	BOOL			isExceptionItem()	
	{
		cBasicItem	*lpBasicItem	=	getBasicItem();

		if	(!lpBasicItem)
			return	FALSE;

		return	lpBasicItem->m_bIsExceptionItem;
	}

	inline	BOOL			isExactlySameMiniPetPouch(cItem *_lpItem)	//	완전히 똑같은 아이템이다.
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

	inline	BOOL			isExactlySameItem(cItem *_lpItem)	//	완전히 똑같은 아이템이다.
	{
		if	(isExceptionItem())
			return	FALSE;

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

	inline	void			reset()	//	초기화
	{
		CItemDefine::reset();
	}

	inline	cBookItem		*getBook()
	{
		if	(isBook())
			return	(cBookItem	*)this;

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

	inline	cCoupleRingDefine	*getCoupleRing()
	{
		if	(isCoupleRing())
			return	(cCoupleRingDefine	*)this;

		return	NULL;
	}
	inline	cCoupleRingDefine	*getPresentCoupleRing()
	{
		if	(isPresentCoupleRing())
			return	(cCoupleRingDefine	*)this;

		return	NULL;
	}
	inline	cCoupleRingDefine	*getNotDefineCoupleCoupleRing()
	{
		if	(isNotDefineCoupleCoupleRing())
			return	(cCoupleRingDefine	*)this;

		return	NULL;
	}
	
	inline	cSpecialItem*	getSpecialItem()
	{
		if	(isExceptionItem())
			return	NULL;

		if	(m_bf1IsSpecialItem	==	FALSE)
			return	NULL;

		return	(cSpecialItem*)this;
	}

	BOOL					isUnknownItem();
	BOOL					isItemPiece();
	void					changeToCoupleRing(char *_lpstrTargetName=NULL);
	BOOL					generatePieceItem(cItem *_lpItem);
	cPieceItem*				getPieceItem();
	cItemSet*				getItemSet();
	cPrincessWeaponDefine*	getPrincessWeapon();	
	cSetItem*				getSetItem();
	BOOL					isEnableJob(int _iJob);

	int						getRequireLevel();
	int						getRequireLevelByPrefix();
	void					operateReversionItemWhenEquip();
	BOOL					isCanUseLevel(int _iLevel, int _iLimitEquipmentLevelOverPoint = 0);
	BOOL					isQuestItem();
	BOOL					isPremiumItem();
	BOOL					isSuperUniqueItem();//이건  슈퍼유니크냐?
	BOOL					isSuperUniqueDXItem();	//	유니크DX에 DX 접두사가 붙었다.
	BOOL					isSuperUniqueUltimateItem();	//	얼티미트 유니크에 얼티미트 접두사가 붙었다.
	BOOL					isUniqueItem();//이건 유니크냐?
	BOOL					isDXUniqueItem();//이건 유니크냐?
	BOOL					isRareItem();	
	BOOL					isLittleGoodItem();
	BOOL					isNormalItem();//이건 보통 아이템이냐

	cBasicItem*				getBasicItem(BOOL _bIsWantPrincessWeapon=FALSE);
	int						getExpandInventorySize();
	void					increaseMinute(int _iMinute);
	BOOL					checkItemCount();
	BOOL					getGuildStatueExporterInfo(int *_lpiType,int *_lpiLevel);
	BOOL					isExistSpecificEffect(int _iEffect);
	BOOL					increaseCarvingLevel(int _iValue);
	BOOL					setCarvingLevel(int _iValue);
	int						getCarvingLevel();
	int						getSetCarvingLevel();
	int						getSetCarvingChance();
	int						getIncreaseCarvingChance();
	int						getIncreaseCarvingValue();
	int						getEffectMiniPetFeedType();
	int						getMiniPetFeedExp();
	int						getSetMiniPetType();
enum
{
	eII_CORRECT,
	eII_EXPIRED,
	eII_IS_MONEY,
	eII_INVALID_QUEST,
	eII_DOUBLE_ITEM,
};
	int						checkIncorrectItem(cACTOR *_lpActor,int _iItemSlot,int *_lpiSameItemCount,int *_lpiReviseIncorrectPrefixCount);
	BOOL					isExistSameTypePrefix(int _iPrefix);
	inline	BOOL			isCorrectStatusItem()
	{
		if (isExceptionItem())
			return	TRUE;

		if (m_isBroken+m_isCanNotAttachPrefixItem)
			return	FALSE;

		return	TRUE;
	}

	BOOL					decreaseCount(int _iCount,BOOL _isBeltItem=FALSE);	//	카운트 줄이기
	BOOL					addItem(cItem *_lpItem,int _iLimitCount=0xffff);	//	아이템 합치기..?

	static	BOOL			Init();
};

class	CCustomItem : public CCustomItemDefine
{
public:
};

#pragma	pack()

#pragma	pack(2)

class	CActorEquipment
{
public:
	cItem			m_aEquip[dEQUIPMENT_PART_COUNT];
	cItem			m_aItems[dOWN_ITEM_COUNT];

					CActorEquipment()	{reset();}

	inline	void	reset()
	{
		int		i;

		for (i=0;i<dEQUIPMENT_PART_COUNT;i++)	m_aEquip[i].reset();
		for (i=0;i<dOWN_ITEM_COUNT;i++)			m_aItems[i].reset();
	}
};

class	CShopGenerateItemInfo;

enum
{
	eAPR_SUCCESS,
	eAPR_EXIST_SAME_PREFIX,
	eAPR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM,
	eAPR_CAN_NOT_ATTACH_PREFIX_ITEM,
	eAPR_PREFIX_FULL,
	eAPR_FAILED,
};

const	int	c_iMaxWorkKindCount	=	50;

class	CItemManager
{
public:
	int				m_iItemCount;

	int				m_iCommonDropItemCount;
	int				m_iBreedingRecordItem,m_iBlockLightPocket;
	int				m_iNameStoneItem;
	int				m_iPieceItem;
	WORD			m_wSecretDungeonKey,m_wTreasureMap;
	WORD			m_wCoupleRing,m_wPresentCoupleRing,m_wNotDefineCoupleCoupleRing;
	WORD			m_wBook,m_wBookCategory,m_wBookPage,m_wPrincessWeapon;
	WORD			m_wFlakeOfDoingOne,m_wFeatherOfThinnkingOne,m_wToyOfSinandRin;
	WORD			m_wCH5BonusHPAmulet,m_wCH5BonusCPAmulet,m_wCH5OverPowerItemAmulet;
	WORD			m_wCH5RedBottomlessBox;
	WORD			m_awDuelBuffItem[4];

	WORD			m_awGuildStatueElement[6][5];	//	 종류 레벨

	int				m_aItemListByType[eIK_END][dMAX_ITEM_COUNT_BY_KIND];
	int				m_aItemCountByType[eIK_END];

	BOOL			generateCh5BonusRedBottomlessBox(cItem *_lpItem);
	BOOL			generateCh5BonusHPAmulet(cItem *_lpItem,int _iTitleLevel);
	BOOL			generateCh5BonusCPAmulet(cItem *_lpItem,int _iTitleLevel);
	BOOL			generateCh5BonusOverEquipLevelAmulet(cItem *_lpItem,int _iTitleLevel);

	void			build();										//	로딩된 아이템 데이터를 정리한다.
	int				getItemByDropLevel(int _iType,int _iDropLevel,BOOL _isUnique);	//	드랍레벨과 타입을 기본으로 해서 아이템을 기본 아이템의 인덱스를 얻는다.
	int				getUniqueItemByDropLevel(int _iDropLevel,BOOL _bIsDX,int _iCheckJob=-1);	//	드랍레벨과 타입을 기본으로 해서 아이템을 기본 아이템의 인덱스를 얻는다.
	int				getTopLevelNormalWeapon(int _iJob,int _iLevel);	//	착용에 필요한 레벨을 체크해서 노말 아이템중 가장 높은 레벨 무기 검색
	BOOL			generateItemByType(cItem *_lpItem,int _iType,int _iDropLevel,int _iCount,BOOL _isUnique);//	드랍레벨과 타입을 기본으로 해서 노말 아이템을 생성한다.
	BOOL			generateArcaItem(cItem *_lpItem,int _iType,int _iDropLevel,int _iCount,int _iArcaLevel);//	드랍레벨과 타입을 기본으로 해서 노말 아이템을 생성한다.
	BOOL			generateItemByBaseItem(cItem *_lpItem,int _iBaseItem,int _iCount,int _iEfficiency=-1, int* _iOption = NULL);//	드랍레벨과 타입을 기본으로 해서 노말 아이템을 생성한다.
	BOOL			generateItemByCustomItem(cItem *_lpItem,CCustomItem *_lpCustomItem);
	BOOL			generatePremiumItem(cItem *_lpItem,int _iPremiumItemIndex);
	BOOL			generateItemPiece(cItem *_lpItem,int _iSelectPieceItem,int _iDropItemPiece,BOOL _bIsInSecretDungeon,BOOL _bIsInBox,int _iFieldIndex,BOOL _bIsByQuest=FALSE);
	BOOL			generateTreasureMap(cItem *_lpItem);
	BOOL			setPrincessWeaponOptionValue(cItem *_lpItem,int _iBaseItem);

	BOOL			generateShopItem(cItem *_lpItem,CShopGenerateItemInfo *_lpFenerateItemInfo);
	BOOL			generateRandomSpecialItem(cItem *_lpItem,int _iDropLevel,int _iChance,BOOL _isRare,BOOL _isUnique,int _iItemType=-1);	//	랜덤하게 특수 아이템을 생성한다.

	void			attachPrefixByDropLevel(cItem *_lpItem,int _iDropLevel,BOOL _bIsRare=FALSE,BOOL _bIsSpecialDropChance=FALSE);//접두사 붙이기!!
	BOOL			attachCh5PrefixByDropLevel(cItem *_lpItem,int _iDropLevel,int _iPrefixCount);
	int				attachPrefix(cItem *_lpItem,int _iPrefix, BYTE _bValue1 = 0, BYTE _bValue2 = 0);	//	접두사 붙이기
	void			makeGoodItem(cItem *_lpItem,int _iDropLevel);	//	접두사 붙이기
	BOOL			attachRandomPrefix(cItem *_lpItem,int _iWhere);	//	특정 위치에 랜덤하게 접두사 붙이기

	BOOL			generateBook(cItem *_lpItem,int _iBookIndex);
	BOOL			generateBookCategory(cItem *_lpItem,int _iBookIndex,int _iBeginCategory,int _iEndCategory);
	BOOL			generateBookPage(cItem *_lpItem,int _iBookIndex,int _iCategory,int _iBeginPage,int _iEndPage);

	cBasicItem*		getItem(char *_strItemName);		//	_strItemName이란 이름을 가진 아이템을 찾는다.
	int				getItemIndex(char *_strItemName);	//	_strItemName이란 이름을 가진 아이템을 찾는다.
	
	BOOL			getCh5HeavenRewardUnique(cItem *_lpResultItem,int _iTitleLevel,int _iJob);

};

// ITEM kieruzikan
#define	dDROPPED_ITEM_LIFE_TIME		4 // 20240905 rekunn 12
#define	dDROPPED_ITEM_SEALED_TIME	2 // 20240905 rekunn 4

enum
{
	eDIM_MONSTER_DROP,
	eDIM_PLAYER_DROP,
	eDIM_DUMMY,
	eDIM_QUEST_DROP,
};

//
//	필드에 떨어진 아이템
class	CDroppedItem
{
public:
	WORD			m_wSerial;//시리얼
	cItem			m_item;//아이템
	cPOINT			m_pos;//어디에 떨어져 있냐?
	WORD			m_wSealedTime;//봉인된 시간
	WORD			m_wDeathCounter;//사라질때까지 카운트 다운
	WORD			m_wOwner;//쥔
	WORD			m_wDropMethod;

					CDroppedItem()	{reset();}

	inline	void	reset()
	{
		m_wDeathCounter	=	dDROPPED_ITEM_LIFE_TIME;	//	dDROPPED_ITEM_LIFE_TIME*10초 (10초에 한번씩 줄임)
		m_wSealedTime	=	dDROPPED_ITEM_SEALED_TIME;	//	40초
		m_wSerial		=	0xffff;//시리얼
		m_wOwner		=	0xffff;
		m_wDropMethod	=	eDIM_MONSTER_DROP;
		m_item.reset();
	}

	inline	void	copy(CDroppedItem *_lpItem)
	{
		memcpy(this,_lpItem,sizeof(CDroppedItem));
	}

	BOOL			isCatchableTreasure(int _iSerial,int _iParty);
	inline	BOOL	isOwner(int _iSerial,int _iParty)
	{
		if (m_wSealedTime	==	0		)
			return	TRUE;
		if (m_wOwner		==	0xffff	)
			return	TRUE;

		if (m_wOwner		>=	dPARTY_FLAG)
		{
			if (m_wOwner-dPARTY_FLAG	==	_iParty)
				return	TRUE;

			return	FALSE;
		}

		if (m_wOwner	==	_iSerial)
			return	TRUE;

		return	FALSE;
	}

	inline	int		getOwnerParty()
	{
		if (m_wSealedTime	==	0		)
			return	0xffff;
		if (m_wOwner		==	0xffff	)
			return	0xffff;

		if (m_wOwner		>=	dPARTY_FLAG)
			return	m_wOwner-dPARTY_FLAG;

		return	0xffff;
	}

};	//	class	CDroppedItem

#define	dMAX_DROPPED_ITEM			1000

class	cFIELD;

class	CDroppedItemManager
{
public:
	cDen<CDroppedItem>		m_den;

	inline	void			init()	{m_den.init(dMAX_DROPPED_ITEM);}//초기화
	inline	void			reset()	{m_den.reset();}//초기화
	inline	void			close()	{m_den.close();}//초기화
	inline	int				add(CDroppedItem *_lpItem){return m_den.add(_lpItem);}//추가
	inline	BOOL			remove(int _iSerial){return m_den.remove(_iSerial);}//제거
	inline	int				getCount(){return m_den.getCount();}//카운트
	inline	void			rebuild(){m_den.rebuild();}
	inline	CDroppedItem*	get(int _iIndex){return	m_den.get(_iIndex);}
	void					outActor(int _iActorSerial,int _iPartySerial,BOOL _isPartyLeave,cFIELD *_lpField);//	액터가 나간다. 아이템의 소유를 변경해 주자.
	void					removeHideWeapon(cACTOR *_lpActor,cFIELD *_lpField,WORD _wBaseItem,DWORD _dwSerial,int _iPart);
	CDroppedItem*			getNeareastItem(cACTOR *_lpActor,int _iRange,int _iValue,BOOL _bIsByMiniPet=FALSE,BOOL _bIsItemCollector=FALSE);
};

extern	CItemManager	g_im;			//	아이템 관리

#define	dMAX_CUSTOM_ITEM_COUNT	100

#define	dWAY_POINT_COUNT		10

class	CCustomItemManager
{
public:
	int						m_iCount;
	int						m_iSize;
	WORD					m_awTeleportStoneIndex[dWAY_POINT_COUNT];
	CCustomItem				*m_pItems;

							CCustomItemManager()
							{
								m_pItems	=	NULL;
							}
							~CCustomItemManager()
							{
								pKILL(m_pItems);
							}

	BOOL					generateTeleportStone(cItem *_lpItem,int _iWayPoint);
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
		if (_iIndex < 0 ||  _iIndex >= m_iSize)	return	NULL;
		if (m_pItems[_iIndex].m_wBaseItem == 0xffff		)	return	NULL;

		return	&m_pItems[_iIndex];
	}

	void			reset()
	{
		m_iCount	=	0;

		pKILL(m_pItems);
	}
	void			build();
};


#pragma pack()

extern	const	int		c_aDX_ItemDurability[];
extern	const	int		c_aUltimateItemDurability[];

const	int	c_aWeaponTypeItem[]	=	
{
	eIK_ONEHANDED_SWORD				,//"한손검",
	eIK_TWOHANDED_SWORD				,//"양손검",
	eIK_STAFF						,//"지팡이",
	eIK_TOOTH						,//"이빨",
	eIK_MACE						,//"메이스",
	eIK_WING						,//"날개",
	eIK_KNIFE						,//"단도",
	eIK_BOW							,//"활",
	eIK_SPEAR						,//"창",
	eIK_FLUTE						,//"피리",
	eIK_SLING						,//"슬링",
	eIK_MAGICAL_ROD					,//"마술봉",
	eIK_WHIP						,//"채찍",
	eIK_SCYTHE						,//"낫",
	eIK_ARM_WEAPON				,// "암웨폰",
	eIK_BOOK						,// "책",
};

#define	dWEAPON_TYPE_ITEM_COUNT		16			// 책까지..
#define dGENERATE_RANDOM_SPECIAL_ITEM_LIST_COUNT 37		// 책까지.. ...37개..
extern	CCustomItemManager	g_premiumItemManager,g_customItem;
extern	int					g_aCommonDropItemList[];
extern	int					g_aGenerateRandomSpecialItemList[dGENERATE_RANDOM_SPECIAL_ITEM_LIST_COUNT];

#endif