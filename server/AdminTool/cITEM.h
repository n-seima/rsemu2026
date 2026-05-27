#ifndef _classITEM_H
#define _classITEM_H

#include "CItemDefine.h"
#include "CItemEffectDefine.h"
#include "CPetDefine.h"


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
#define	dITEM_FIELD_SHAPE_MONEY				152		//	돈 모양 -o-
#define	dITEM_COUNT_OF_MONEY_FIELD_SHAPE	4		//	돈 모양 -o-

#define	dDROP_ITEM_PATTERN_COUNT			10

#define	dVALID_ITEM							0xffffffff

#define	dEQUIP_WEAPON					0			//	무기
#define	dEQUIP_SHIELD_BULLET			1			//	방패/탄환
#define	dEQUIP_ARMOR					2			//	갑옷
#define	dEQUIP_GLOVE					3			//	장갑
#define	dEQUIP_HELM						4			//	헬멧 종류
#define	dEQUIP_EARING					5			//	귀거리
#define	dEQUIP_NECKLACE					6			//	목걸이
#define	dEQUIP_BELT						7			//	벨트
#define	dEQUIP_SHOES					8			//	신발
#define	dEQUIP_RING						9			//	반지
#define	dEQUIP_RING1					9			//	반지
#define	dEQUIP_RING2					10			//	반지
#define	dEQUIP_RING3					11			//	반지
#define	dEQUIP_RING4					12			//	반지
#define	dEQUIP_RING5					13			//	반지
#define	dEQUIP_RING6					14			//	반지
#define	dEQUIP_RING7					15			//	반지
#define	dEQUIP_RING8					16			//	반지
#define	dEQUIP_WEAPON_2					17			//	무기2
#define	dEQUIP_BELT_SLOT1				18			//	벨트 슬롯 1
#define	dEQUIP_BELT_SLOT2				19			//	벨트 슬롯 2
#define	dEQUIP_BELT_SLOT3				20			//	벨트 슬롯 3
#define	dEQUIP_BELT_SLOT4				21			//	벨트 슬롯 4
#define	dEQUIP_BELT_SLOT5				22			//	벨트 슬롯 5
#define	dEQUIPMENT_PART_COUNT			23

class	cSetItem;

const	BOOL	const_abIsBeltStackAbleItem[]	=	
{
FALSE							,//"헬멧",
FALSE							,//"관",
FALSE							,//"장갑",
FALSE							,//"장갑대용",
FALSE							,//"발톱",
FALSE							,//"팔찌",
FALSE							,//"벨트",
FALSE							,//"부츠",
FALSE							,//"목걸이",
FALSE							,//"반지",
FALSE							,//"귀걸이",
FALSE							,//"망토",
FALSE							,//"공용갑옷",
FALSE							,//"전용갑옷",
FALSE							,//"한손검",
FALSE							,//"방패",
FALSE							,//"양손검",
FALSE							,//"지팡이",
FALSE							,//"이빨",
FALSE							,//"메이스",
FALSE							,//"날개",
FALSE							,//"단도",
FALSE							,//"활",
FALSE							,//"화살",
FALSE							,//"창",
FALSE							,//"피리",
FALSE							,//"슬링",
FALSE							,//"탄환",
FALSE							,//"마술봉",
FALSE							,//"채찍",
TRUE							,//"보석",
TRUE							,//"체력 포션",
TRUE							,//"차징 포션",
TRUE							,//"스탯 향상 포션",
TRUE							,//"성능 향상 포션",
TRUE							,//"치료약",
TRUE							,//"상태 회복제",
FALSE							,//"열쇠",
TRUE							,//"포탈 스톤",
FALSE							,//"필살기 두루말이",
TRUE							,//"먹을 것/기타",
TRUE							,//"능력향상제",
FALSE							,//"강화액",
FALSE							,//"마법 보석",
FALSE							,//"특수 아이템",
FALSE							,//"모든 무기",
};


#define	dITEM_KIND_ALL					0xffff		//	모든 종류의 아이템
#define	dITEM_KIND_WASTING				0xfffe		//	모든 종류의 아이템


#define	dOWN_ITEM_COUNT					42

#define	dMONSTER_ITEM_COUNT				10			//	몬스터가 가지고 있을 수 있는 아이템의 최대 수

#define	dITEM_NAME_LENGTH				50

#define	dITEM_UNIQUE_DATA_COUNT			6			//	유니크 데이터 수
#define	dITEM_UNIQUE_DATA_VALUE_COUNT	8
#define	dGENERATE_ITEM_DATA_COUNT		4
#define	dGENERATE_ITEM_DATA_VALUE_COUNT	4
#define	dITEM_PREFIX_COUNT				3

const	int c_aCheckPartForProveEnableSex[16]	=	{16,16,16,16,16,16,16,16,17,17,17,17,17,17,17,17};	//	직업의 성별

#pragma pack(2)

class	cBASIC_ITEM : public CBasicItemData
{ 
public:
							cBASIC_ITEM()
							{
								m_iSerial	=	0xffff;
							}

	char					*getComment();
	
	static	int				GetItemByEquippedShape(int _iShape,int _iJob);	//	착용한 모양과 직업을 기본으로 해서 아이템 구하기
	static	char			s_strComment[2048];
};

extern	cBASIC_ITEM	g_aBasicItem[dBASIC_ITEM_COUNT];

#pragma	pack()

#pragma	pack(1)

class	cSpecialItem	:	public cSpecialItemDefine
{
public:
	void					reset();
};

class	CCustomItem : public CCustomItemDefine
{
public:
};

class	cITEM : public CItemDefine
{
public:
							cITEM()							{reset();}

	inline	BOOL			isExactlySameItem(cITEM *_lpItem)	//	완전히 똑같은 아이템이다.
	{
		if	(isBreedingRecordBook()  || isCoupleRing())
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

	inline	BOOL			isSameItem(cITEM *_lpItem)	//	똑같은 아이템이다.
	{
		if	(isBreedingRecordBook()  || isCoupleRing())
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

		if (((CItemDefineForValue *)this)->m_dwValue	!=	((CItemDefineForValue *)_lpItem)->m_dwValue)
			return	FALSE;

		return	TRUE;
	}

	inline	cSpecialItem*	getSpecialItem()
	{
		if	(m_bf1IsSpecialItem	==	FALSE)
			return	NULL;

		return	(cSpecialItem*)this;
	}

	inline	BOOL			isCoupleRing()
	{
		cBASIC_ITEM	*lpItem	=	getBasicItem();

		if (!lpItem)	
			return	FALSE;

		if (lpItem->m_aGenerateData[0].m_wEffect	==	eIE_COUPLE_RING)
			return	TRUE;
		if (lpItem->m_aGenerateData[0].m_wEffect	==	eIE_PRESENT_COUPLE_RING)
			return	TRUE;
		if (lpItem->m_aGenerateData[0].m_wEffect	==	eIE_NOT_DEFINE_COUPLE_COUPLE_RING)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isBreedingRecordBook()
	{
		cBASIC_ITEM	*lpItem	=	getBasicItem();

		if (!lpItem)	
			return	FALSE;

		if (lpItem->m_aGenerateData[0].m_wEffect	==	eIE_BREEDING_RECORD)
			return	TRUE;

		return	FALSE;
	}

	cBreedingRecordBookDefine*	getBreedingRecordBookPetInfo();
	
	inline	BOOL			isSpecialItem()
	{
		return	m_bf1IsSpecialItem;
	}

	BOOL						isPremiumItem();
	int							getRequireLevelByPrefix();
	int							getRequireLevel();
	cSetItem*					getSetItem();
	char*						getBookName();	

	cBookItemDefine*			getBook();
	BOOL						isBook();
	BOOL						isAddPageToBookItem();
	cBookPageItemDefine*		getAddPageToBookItem();
	BOOL						isAddCategoryToBookItem();
	cBookCategoryItemDefine*	getAddCategoryToBookItem();
	int							getBeginPageInBook();
	int							getEndPageInBook();
	
	void						increaseMinute(int _iMinute);
	BOOL						isIncorrectItem();
	char*						getTooltip();							//	툴팁을 얻어온다.
	char*						getJewelTooltip(int _iRemainTime);
	DWORD						getPrice(int _iCount);					//	가격 계산
	BOOL						isHighLevelMagicOptionItem(int _iLevel = 50);
	char*						getName();								//	아이템 이름
	char*						getExpireDate();
	char*						getItemCode(BOOL _bIsIncludeName=TRUE);
	BOOL						isDXItem();
	void						repairItemIncorrectInfo();

	inline	BOOL				isSecretDungeonKey()
	{
		cBASIC_ITEM			*lpItem	=	getBasicItem();

		if (!lpItem)	
			return	FALSE;

		if (lpItem->m_aGenerateData[0].m_wEffect	==	eIE_SECRET_DUNGEON_KEY)
			return	TRUE;

		return	FALSE;
	}

	inline	int				getCheckSum()
	{
		int	iSum=	0;

		iSum	+=	m_dwSerial;
		iSum	+=	m_wBaseItem;							//	아이템 인덱스
		iSum	+=	m_bCount;								//	숫자
		iSum	+=	m_bDurability;							//	내구력
		iSum	+=	m_aOption[0];							//	생성될때 붙은 옵션
		iSum	+=	m_aOption[1];							//	생성될때 붙은 옵션

		for (int iPrefix=0;iPrefix<dITEM_PREFIX_COUNT;iPrefix++)
		{
			iSum	+=	m_aPrefix[iPrefix].m_wPrefix;
			iSum	+=	m_aPrefix[iPrefix].m_aValue[0];
			iSum	+=	m_aPrefix[iPrefix].m_aValue[1];
		}

		return	iSum;
	}

	int						getPrefixCount();

	BOOL					refitItemData();
	int						getIncorrectPrefixCount();

	int						getNounPrefixCount();
	int						getAdjectivePrefixCount();

	BOOL					isExceptionItem();
	BOOL					isInfinityBullet();		//	무한 탄환이냐?
	BOOL					isExtraItem();	
	inline	BOOL			isBeltStackAbleItem()		//	벨트에 넣을 수 있는 아이템이냐?
	{
		if	(!getBasicItem())
			return	FALSE;
		
		return	const_abIsBeltStackAbleItem[getBasicItem()->m_wKind];
	}

	inline	int				getStackLimit()	//	몇개나 겹칠 수 있는겨?
	{
		cBASIC_ITEM	*lpItem		=	getBasicItem();

		return	lpItem->m_wStackLimit;
	}

	inline	int				isFull(int _iCount=0xffff)		//	꽉 찼냐?
	{
		cBASIC_ITEM	*lpItem		=	getBasicItem();

		if (_iCount	==	0xffff)	_iCount	=	lpItem->m_wStackLimit;

		if (_iCount	<=	m_bCount)	return	TRUE;

		return	FALSE;
	}

	inline	void			setBasicItem(cBASIC_ITEM *_lpItem,int _iCount,DWORD _dwSerial)	//	기본 아이템 설정
	{
		m_wBaseItem		=	_lpItem->m_iSerial;
		m_bCount		=	_iCount;
		m_bDurability	=	(BYTE)_lpItem->m_wDurability;
		m_dwSerial		=	_dwSerial;
	}

	inline	void			set(cITEM *_lpItem)
	{
		if (!_lpItem)	reset();
		else			memcpy(this,_lpItem,sizeof(cITEM));
	}//	아이템 복사
	inline	void			copy(cITEM *_lpItem)
	{
		if (!_lpItem)	reset();
		else			memcpy(this,_lpItem,sizeof(cITEM));
	}//	아이템 복사
	inline	void			swapItem(cITEM *_lpItem)//바꿔치기
	{	cITEM	tempItem;

		tempItem.copy(this);
		this->copy(_lpItem);
		_lpItem->copy(&tempItem);
	}
	

	inline	void			reset()	//	초기화
	{
		CItemDefine::reset();
	}

	inline	BOOL			isNormalItem()//이건 보통 아이템이냐
	{	
		if (m_aPrefix[0].m_wPrefix	!=	0xffff)	return	FALSE;	//	레어.. 다 -o-

		cBASIC_ITEM	*lpItem		=	getBasicItem();

		if (lpItem->m_aUniqueData[0].m_wEffect	!=	0xffff)	return	FALSE;	//	유니크... 다 -o-

		return	FALSE;
	}
	inline	BOOL			isRareItem()	{if (m_aPrefix[0].m_wPrefix	!=	0xffff)	return	TRUE;return	FALSE;}
	inline	BOOL			isUniqueItem()//이건 유니크냐?
	{
		cBASIC_ITEM	*lpItem		=	getBasicItem();

		if (!lpItem)	return	FALSE;

		if (lpItem->m_aUniqueData[0].m_wEffect	!=	0xffff)	return	TRUE;	//	유니크... 다 -o-

		return	FALSE;
	}

	inline	cBASIC_ITEM		*getBasicItem()
	{
		if (m_wBaseItem	>=	dBASIC_ITEM_COUNT)	return	NULL;

		return	&g_aBasicItem[m_wBaseItem];
	}

	static	int			s_iCount,s_iPrefixCount,s_iPremiumItemCount,s_iCustomItemCount;
	static	BOOL		s_bIsFindIncorrectPrefix;

	static	BOOL		ReadItemData();
};

class	cSetItem	:	public	cSetItemDefine
{
public:
	WORD			getJobMask();
	BOOL			generateItem(cITEM *_lpItem,int _iItemSet);
	char*			getComment();
};

class	cItemSet	:	public	cItemSetDefine
{
public:
	void			reset();
	void			copy(cItemSet	*_lpPieceItem)	{}

	int				getItemCount();
	cSetItem*		getItem(int _iSlot);
	cSetItem*		getItemByBaseItem(int _iBaseItem);

	cUniqueData*	getSetEffect(int _iSetItemCount,int _iEffectIndex);
	int				getSetEffectCount(int _iSetItemCount);
	char*			getComment();
};

class	cItemSetManager
{
public:
	cDen<cItemSet>				m_den;

	inline	cItemSet*			get(int _iIndex)
	{
		return	m_den.get(_iIndex);
	}

	inline	void				init(int _iCount)
	{
		m_den.init(_iCount);
	}

	inline	cItemSet*			getBay()
	{
		return	m_den.getList();
	}

	void						build();
};

#pragma	pack()

extern	CCustomItem		g_aPremiumItem[1024];
extern	CCustomItem		g_aCustomItem[1024];
extern	cItemSetManager	g_itemSetManager;

#endif