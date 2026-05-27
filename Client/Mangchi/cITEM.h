#ifndef _classITEM_H
#define	_classITEM_H

#include "SFC.H"
#include "cIMAGE.H"
#include "CItemDefine.H"
#include "CItemEffectDefine.H"
#include "playerJobCommon.H"

#define	dITEM_PRICE_BY_PRICE_VALUE	0	//	가격수치로 처리
#define	dITEM_COMPUTE_METHOD1		1	//	수치1*가격
#define	dITEM_COMPUTE_METHOD1		1	//	수치2*가격
#define	dITEM_COMPUTE_METHOD2		2	//	수치1*수치2/가격
#define	dITEM_COMPUTE_METHOD3		3	//	(수치1+1)*가격
#define	dITEM_COMPUTE_METHOD4		4	//	(수치2+1)*가격

#define	dMAX_ITEM_ADDITIONAL_DATA	500

#define	dICON_SHAPE_HEIGHT			34
#define	dICON_SHAPE_WIDTH			34

#define	dVALID_ITEM					0xffffffff

#define	dDROP_ITEM_PATTERN_COUNT	10


#define	dITEM_KIND_ALL				0xffff		//	모든 종류의 아이템
#define	dITEM_KIND_WASTING			0xfffe		//	소모성 아이템


#define	dMAX_ITEM_COUNT				40

#define	dITEM_KEEP_GRADE_NORMAL		0			//	소모성 아이템
#define	dITEM_KEEP_GRADE_SAVE		1			//	장비 가능한 아이템

#define	dSHAPE_SWORD				0			//	일반 칼
#define	dSHAPE_LEATHER_SHIELD		1			//	가죽 방패
#define	dSHAPE_BROAD_SWORD			2			//	대검
#define	dSHAPE_LEATHER_ARMOR		3			//	가죽 갑옷

#define	dMONSTER_ITEM_COUNT			10			//	몬스터가 가지고 있을 수 있는 아이템의 최대 수


#define	dITEM_UNIQUE_DATA_COUNT			6		//	아이템 효과의 수
#define	dBELT_ITEM_COUNT				4
#define	dITEM_EFFECT_VALUE_COUNT		8
#define	dGENERATE_ITEM_DATA_COUNT		4
#define	dGENERATE_ITEM_DATA_VALUE_COUNT	4
#define	dITEM_PREFIX_COUNT				3

#pragma pack(2)

class	cITEM_EFFECT
{
public:
	WORD	m_wEffect;
	short	m_aValue[dITEM_EFFECT_VALUE_COUNT];

			cITEM_EFFECT()
			{
				m_wEffect	=	0xffff;
			}

	void	reset()	
			{
				memset(this,0,sizeof(cITEM_EFFECT));
				m_wEffect	=	0xffff;
			}

	char*	getComment(BOOL _isSpecial=FALSE);
};

class	cITEM_PREFIX_INFO
{
public:
	WORD		m_wPrefix;
	BYTE		m_aValue[2];
};

extern	int		g_aStackableItem[];

class	cBASIC_ITEM : public CBasicItemData
{
public:
	DWORD					m_dwCommentSize;					//	아이템 설명의 사이즈(텍스트는 따로 모아서 보관한다. 너무 많다. --)
	char					*m_pItemComment;


							cBASIC_ITEM()
							{
								m_pItemComment				=	NULL;
								m_dwCommentSize				=	0;
								reset();
							}

							~cBASIC_ITEM()	{	pKILL(m_pItemComment);	}

	int						getValidUniqueDataIndex()
							{
								for (int i=0;i<dITEM_UNIQUE_DATA_COUNT;i++)
									if (m_aUniqueData[i].m_wEffect	==	0xffff)	return	i;

								return	0xffff;
							}

	int						getValidGenerateDataIndex()
							{
								for (int i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
									if (m_aGenerateData[i].m_wEffect	==	0xffff)	return	i;

								return	0xffff;
							}
	inline	BOOL			isEquipment()
	{
		if (m_wKind	<=	eIK_WHIP)	return	TRUE;

		return	FALSE;
	}
	

	inline	BOOL			isStackAble()
	{
		int	iIndex	=	0;

		while(1)
		{
			if (g_aStackableItem[iIndex]	==	0xffff	)	return	FALSE;
			if (g_aStackableItem[iIndex]	==	m_wKind	)	return	TRUE;

			iIndex++;
		}

		return	FALSE;
	}
	
	inline	BOOL			isDamageItem()	//	... 무기냐?
	{
		if (m_wMaxDamage	==	0)	return	FALSE;

		return	TRUE;
		//무기냐-_-?
	}
	inline	BOOL			isUnique()	//	유니크
	{
		if (c_aItemEquipPlace[m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[m_wKind] == 0xffff)		//  변경..
			return	FALSE;
		if (isExtraItem())	
			return	FALSE;
		if (m_aUniqueData[0].m_wEffect	!=	0xffff)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isDXUnique()	//	유니크
	{
		if (!isUnique())
			return	FALSE;
		if (!isDX())
			return	FALSE;

		return	TRUE;
	}
	inline	BOOL			isDX()	//	유니크
	{
		if (c_aItemEquipPlace[m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[m_wKind] == 0xffff)		//  변경..
			return	FALSE;
		if (isExtraItem())	
			return	FALSE;
		if (m_attr.isDXItem)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isExtraItem(BOOL _bIsExceptChargeItem=FALSE)	//	유니크
	{
		if (m_attr.isExtraItem)	
		{
			if (_bIsExceptChargeItem)
				if (isChargeExtraItem())	
					return	FALSE;

			return	TRUE;
		}

		return	FALSE;
	}
	BOOL					isInfinityBullet();
	BOOL					addInfinityBulletOption();

	char*					getItemInfo();

	inline	BOOL			isEnableAllJob()
	{
		if	(!m_bf1IsMaleJobEquipAble)
			return	FALSE;
		if	(!m_bf1IsFemaleJobEquipAble)
			return	FALSE;

		for (int i=0;i<dPLAYER_JOB_COUNT;i++)
			if	(!isEquipAbleJob(i))
				return	FALSE;

		return	TRUE;
	}

	inline	BOOL			isExclusiveMaleJob()
	{
		if	(!m_bf1IsMaleJobEquipAble)
			return	FALSE;
		if	(m_bf1IsFemaleJobEquipAble)
			return	FALSE;

		for (int i=0;i<dPLAYER_JOB_COUNT;i++)
			if	(!isEquipAbleJob(i))
				return	FALSE;

		return	TRUE;
	}

	inline	BOOL			isExclusiveFemaleJob()
	{
		if	(m_bf1IsMaleJobEquipAble)
			return	FALSE;
		if	(!m_bf1IsFemaleJobEquipAble)
			return	FALSE;

		for (int i=0;i<dPLAYER_JOB_COUNT;i++)
			if	(!isEquipAbleJob(i))
				return	FALSE;

		return	TRUE;
	}

	inline	BOOL			setAllJobEnable()
	{
		for (int i=0;i<dPLAYER_JOB_COUNT;i++)
			m_uiEquipAbleJobMask	+=	1<<i;

		m_bf1IsFemaleJobEquipAble	=	1;
		m_bf1IsMaleJobEquipAble		=	1;

		return	TRUE;
	}

	char*					getDataComment(int _iIndex,BOOL _isSpecial=FALSE,int _iValue1=-1,int _iValue2=-1);//data 코멘트
	char*					getComment();						//	m_dwCommentAddress을 고려해서 어쩌구 저쩌구 한다.
	void					setComment(char *_strComment);		//	m_pItemComment를 설정한다.
	void					copy(cBASIC_ITEM	*lpItem);
	void					copy(int _iIndex);
	void					reset();
	BOOL					isSame(cBASIC_ITEM* _lpBasicItemData);
///////////////////
//	정적 멤버들
///////////////////
	static	int				s_iCount;
	static	int				s_aCountOfCategory[dCOUNT_OF_ITEM_KIND];

	static	void			Reset(cBASIC_ITEM *_lpBI=NULL);
	static	void			Arrange(cBASIC_ITEM *_lpBI=NULL,int _iVersion=-1);

	static	int				Add(cBASIC_ITEM *_lpItem);
	static	BOOL			Remove(int _iIndex);
	static	BOOL			Remove(char *_strName);
	static	BOOL			IsExist(int _iIndex,int _iKind=dITEM_KIND_ALL);
	static	int				GetLastIndex(cBASIC_ITEM *_lpBI=NULL);

	static	cBASIC_ITEM*	Get(char *_strName,cBASIC_ITEM *_lpBI=NULL);
	static	cBASIC_ITEM*	GetItemByKind(int _iKind,int *_iBaseIndex);
	static	cBASIC_ITEM*	GetItemByKind(int _iKind,int _iJob,int *_iBaseIndex);
	static	int				GetCountByKind(int _iKind);
	static	void			ResetUniqueItem();
	static	int				GetItemKind(char *_strName);
	static	int				GetUniqueCount();//	유니크 아이템의 갯수 리턴
};

class	cITEM : public CItemDefine
{
public:

							cITEM()
							{
								reset();
							}

	inline	void			reset()	//	초기화
	{
		CItemDefine::reset();
	}

	void					copy(cITEM *_lpItem)
							{
								memcpy(this,_lpItem,sizeof(cITEM));
							}

	int						getMaxDurability();
	char*					getName(BOOL _bIsWantColor=TRUE,BOOL _bIsWantIcon=TRUE);
	cBASIC_ITEM*			getBasicItem();

	inline	BOOL			isBreedingRecordBook()
	{
		cBASIC_ITEM	*lpItem	=	getBasicItem();

		if (!lpItem)	
			return	FALSE;

		if (lpItem->m_aGenerateData[0].m_wEffect	==	eIE_BREEDING_RECORD)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isExtraItem(BOOL _bIsExceptChargeItem=FALSE)	//	엑스트라 아이템
	{
		cBASIC_ITEM	*lpItem	=	getBasicItem();

		if (!lpItem)	
			return	FALSE;

		return	lpItem->isExtraItem();
	}

	BOOL					isPremiumItem();
	BOOL					isSuperUniqueItem();//이건  슈퍼유니크냐?
	BOOL					isUniqueItem();//이건 유니크냐?
	BOOL					isRareItem();	
	BOOL					isLittleGoodItem();
	BOOL					isNormalItem();//이건 보통 아이템이냐
	BOOL					isItemPack();

	int						getNounPrefixCount();
	int						getAdjectivePrefixCount();

	inline	int				getPrefixCount()
	{
		int	iPrefixCount	=	0;

		for (int i=0;i<dITEM_PREFIX_COUNT;i++)
		{
			if (m_aPrefix[i].m_wPrefix	==	0xffff)	break;

			iPrefixCount++;
		}

		return	iPrefixCount;
	}

		

	static	void			Init();
	static	void			Reset();
};

#define	dMAX_CUSTOM_ITEM_COUNT	100
#define	dMAX_INIT_CUSTOM_ITEM_COUNT	10000

class	CCustomItem	: public CCustomItemDefine
{
public:
	cBASIC_ITEM*	getBasicItem()
	{
		return (cBASIC_ITEM *)this;
	}
	inline	void	reset()	//	초기화
	{
		memset(this,0,sizeof(CItemBaseInfo));
		memset(m_aPrefix,0xff,sizeof(m_aPrefix));

		m_wBaseItem				=	0xffff;

		m_bf20TermOfValidate	=	0;
		m_bf1IsSellInCarrotShop	=	0;

		m_wIsAttachRandomPrefix	=	FALSE;
		m_wDropLevel			=	10;
		m_wGenerateQuality		=	100;

		strcpy(m_strName,"no name");

		for (int i=0;i<dITEM_PREFIX_COUNT;i++)
			m_aPrefixGenerateQuality[i]	=	50;
	}
};

class	CCustomItemManager
{
	int						m_iSize;
	int						m_iCount;
	int						m_iRookie;
	CCustomItem				*m_pItems;

public:

							CCustomItemManager()
							{
								m_pItems	=	NULL;
								reset();
							}

							~CCustomItemManager()
							{
								pKILL(m_pItems);
							}

	void					init(int _iSize=dMAX_CUSTOM_ITEM_COUNT)
	{
		m_iSize		=	_iSize;

		pKILL(m_pItems);
		m_pItems	=	new CCustomItem [_iSize];

		reset();
	}
	inline	int				getSize()	{return	m_iSize;}
	
	void					reset()
	{
		m_iRookie	=	0;
		m_iCount	=	0;

		for (int i=0;i<m_iSize;i++)
			m_pItems[i].reset();
	}

	void					rebuild()
	{
		m_iRookie	=	0xffff;
		m_iCount	=	0;

		for (int i=0;i<m_iSize;i++)
		{
			if (m_pItems[i].m_wBaseItem	==	0xffff)
			{
				if (m_iRookie	==	0xffff)	m_iRookie	=	i;

				continue;
			}

			m_iCount++;
		}

		if (m_iRookie	==	0xffff)	m_iRookie	=	0;
	}

	int						add(CCustomItem *_lpItem)
	{
		int		iAddSerial	=	m_iRookie;

		if (m_iCount >= m_iSize)
			return	0xffff;

		memcpy(&m_pItems[m_iRookie],_lpItem,sizeof(CCustomItem));

		m_iCount++;

		for (int i=m_iRookie+1;i<m_iSize;i++)
			if (m_pItems[i].m_wBaseItem == 0xffff)
			{
				m_iRookie	=	i;
				break;
			}

		return	iAddSerial;
	}

	int						insert(int _iIndex,CCustomItem *_lpItem);
	BOOL					remove(int _iIndex)
	{
		if (_iIndex < 0 ||  _iIndex >= m_iSize)	return	FALSE;
		if (m_pItems[_iIndex].m_wBaseItem == 0xffff		)	return	FALSE;

		m_iCount--;
		m_pItems[_iIndex].m_wBaseItem		=	0xffff;

		if	(_iIndex<m_iRookie)	m_iRookie	=	_iIndex;

		return	TRUE;
	}

	inline	int				getCount()	{return m_iCount;}
	inline	int				getLastIndex()
	{
		for (int i=m_iSize;i;--i)
		{
			if (m_pItems[i-1].m_wBaseItem	!=	0xffff)
				return	i;
		}

		return	0;
	}
	inline	CCustomItem*	getItemBay(){return	m_pItems;}
	inline	CCustomItem*	getItem(int	_iIndex)
	{
		if (_iIndex < 0 ||  _iIndex >= m_iSize		)	return	NULL;
		if (m_pItems[_iIndex].m_wBaseItem == 0xffff	)	return	NULL;

		return	&m_pItems[_iIndex];
	}
};

#pragma pack()

extern	cSPRITE		g_sprFieldShape;
extern	cSMI		g_smiIconItem;
extern	char		*g_strEquipmentPart[];	//	아이템 장착 파트
extern	char		*g_strItemCategory[];	//	아이템 분류
extern	char		*g_strItemKeepGrade[];	//	아이템 보관 등급
extern	cBASIC_ITEM	g_aBasicItem[dBASIC_ITEM_COUNT];
extern	char		*g_strPriceComputeMethod[];

extern	char		*g_strEquipUseLimitContents[];	//	사용/착용 제한을 계산하는 능력치
extern	char		*g_strEquipUseLimitMethod[];	//	사용/착용 제한을 계산하는 방법
extern	char		*g_strItemPrefixPriceComputeMethod[];//	레어 옵션의 가격 계산 방법
extern	char		g_strViewEquipmentName[80][20][40];//외부로 드러나는 장비의 이름들
extern	char		*g_strPremiumServiceLevel[];	//	프리미엄 서비스 등급
extern	char		*g_strPrefixOutputEffect[];		//	강제 이펙트 관련 

extern	CCustomItemManager	g_premiumItem,g_customItem;

#endif