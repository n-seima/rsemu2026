#ifndef _classITEM_H
#define _classITEM_H

#include <winsock2.h>
#include <windows.h>
#include "CItemDefine.h"
#include "CItemEffectDefine.h"

#define	dITEM_INDEX_GOLD_BAR				4237

#define	dBORDER_OF_ITEM_AND_EQUIPMENT		100
#define	dBORDER_OF_BELT_FOR_USE_ITEM		100
#define	dDEFAULT_BELT_ITEM_RELOAD_TIME		8

#define	dDROPPING_ITEM_COUNT				10		//	떨어드리는 아이템 패턴

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

const	c_aCheckPartForProveEnableSex[16]	=	{16,16,16,16,16,16,16,16,17,17,17,17,17,17,17,17};	//	직업의 성별

class	cSetItem;

#pragma pack(2)

class	cBASIC_ITEM : public CBasicItemData
{
public:
							cBASIC_ITEM()
							{
								m_iSerial	=	0xffff;
							}

	BOOL					isAbvailJobItem();

	inline	BOOL			isLimitEquipItem()
	{
		if (m_wRequireAllignment+m_wRequireLevel+m_wRequireStrength+m_wRequireDexterity+m_wRequireConstitution+m_wRequireWisdom+m_wRequireIntelligence+m_wRequireCharisma+m_wRequireLuck)
			return	TRUE;

		if ((1-isAllJobItem()) + isManExclusiveItem() + isWomanExclusiveItem())	return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isAllJobItem()	//	모든 직업이 사용 가능한 아이템
	{
		int	iSum	=	0;

		for (int i=0;i<16;i++)	iSum	+=	m_aEnableJob[i];

		if (iSum	==	16)	return	TRUE;

		return	FALSE;
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


class	cITEM : public CItemDefine
{
public:

							cITEM()							{reset();}

	char*					getName();								//	아이템 이름

	inline	int				getPrefixCount()
	{
		int	iPrefixCount	=	0;

		for (int i=0;i<dITEM_PREFIX_COUNT;i++)
		{
			if (m_aPrefix[i].m_wPrefix	>=	s_iPrefixCount)	break;

			iPrefixCount++;
		}

		return	iPrefixCount;
	}

	BOOL					isEnableJob(int _iJob);

	int						getNounPrefixCount();
	int						getAdjectivePrefixCount();
	int						getRequireLevelByPrefix();	//	접두사로 인해 착용/사용에 필요한 레벨
	char*					getPrefixComment(int _iPrefixIndex);	//	_iPrefixIndex번째 접두사의 코멘트 얻기
	BOOL					isExtraItem();
	BOOL					isDXItem();
	
	cSetItem*				getSetItem();		//	세트 아이템 얻기
	char*					getBookName();
	cBookItemDefine*		getBook();
	BOOL					isBook();
	BOOL					isAddPageToBookItem();
	cBookPageItemDefine*	getAddPageToBookItem();
	BOOL					isAddCategoryToBookItem();
	cBookCategoryItemDefine*	getAddCategoryToBookItem();
	int						getBeginPageInBook();
	int						getEndPageInBook();

	int						getRequireLevelForEquip();

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
		if (!_lpItem)
			reset();
		else
			memcpy(this,_lpItem,sizeof(cITEM));
	}//	아이템 복사
	inline	void			copy(cITEM *_lpItem)
	{
		if (!_lpItem)
			reset();
		else
			memcpy(this,_lpItem,sizeof(cITEM));
	}//	아이템 복사
	inline	void			swapItem(cITEM *_lpItem)//바꿔치기
	{
		cITEM	tempItem;

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
		if (m_aPrefix[0].m_wPrefix	<	s_iPrefixCount)	return	FALSE;	//	레어.. 다 -o-

		cBASIC_ITEM	*lpItem		=	getBasicItem();

		if (!lpItem)
			return	FALSE;

		if (lpItem->m_aUniqueData[0].m_wEffect	!=	0xffff)	return	FALSE;	//	유니크... 다 -o-

		return	FALSE;
	}
	inline	BOOL			isRareItem()
	{
		if (m_aPrefix[0].m_wPrefix	<	s_iPrefixCount)	
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isUniqueItem()//이건 유니크냐?
	{
		cBASIC_ITEM	*lpItem		=	getBasicItem();

		if (!lpItem)
			return	FALSE;


		if (lpItem->m_aUniqueData[0].m_wEffect	!=	0xffff)	return	TRUE;	//	유니크... 다 -o-

		return	FALSE;
	}

	inline	cSpecialItem*	getSpecialItem()
	{
		if	(m_bf1IsSpecialItem	==	FALSE)
			return	NULL;

		return	(cSpecialItem*)this;
	}

	inline	BOOL			checkFirstEffect(int _iEffect)
	{
		cBASIC_ITEM	*lpItem	=	getBasicItem();

		if	(!lpItem)	
			return	FALSE;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	_iEffect)
			return	TRUE;

		return	FALSE;
	}

	
	inline	BOOL			isCoupleRing()
	{
		cBASIC_ITEM	*lpItem	=	getBasicItem();

		if (!lpItem)	
			return	FALSE;

		if (lpItem->m_aGenerateData[0].m_wEffect	==	eIE_COUPLE_RING)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL				isSecretDungeonKey()
	{
		cBASIC_ITEM			*lpItem	=	getBasicItem();

		if (!lpItem)	
			return	FALSE;

		if (lpItem->m_aGenerateData[0].m_wEffect	==	eIE_SECRET_DUNGEON_KEY)
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

	inline	cBASIC_ITEM		*getBasicItem()
	{
		if (m_wBaseItem	>=	s_iCount)	return	NULL;

		return	&g_aBasicItem[m_wBaseItem];
	}
	BOOL					isIncorrectItem();
	int						getIncorrectPrefixCount();

	static	int			s_iCount,s_iPrefixCount;

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

extern	cItemSetManager	g_itemSetManager;
extern	int				g_iBreedingRecordBook;

#endif