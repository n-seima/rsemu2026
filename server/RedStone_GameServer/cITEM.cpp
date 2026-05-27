#include	"cItem.H"
#include	"cFIELD.H"
#include	"CShop.H"
#include	"CGame.H"
#include	"debugCode.H"
#include	"pieceItem.H"
#include	"itemSet.H"
#include	"packetManager.H"
#include	"book.H"

cBasicItem				g_aBasicItem[dBASIC_ITEM_COUNT];
CItemManager			g_im;			//	¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½

cITEM_PREFIX			g_aItemPrefix[dMAX_ITEM_PREFIX_COUNT];
cITEM_PREFIX_MANAGER	g_itemPrefix;
CCustomItemManager		g_premiumItemManager;
CCustomItemManager		g_customItem;
DWORD					l_adwCheckItemList[1024][2];
int						l_iCheckItemCount=0;

//	¿½¿½¿½¿½´¿? ¿½¿½¿½¿½¿½¿½ ¿½¿½¿?
int
g_aCommonDropItemList[]	=
{
	eIK_HELM						,
	eIK_GLOVE						,
	eIK_BELT						,
	eIK_BOOTS						,
	eIK_ARMOR						,

	eIK_ONEHANDED_SWORD				,
	eIK_SHIELD						,
	eIK_TWOHANDED_SWORD				,
	eIK_STAFF						,
	eIK_TOOTH						,
	eIK_MACE						,
	eIK_WING						,
	eIK_KNIFE						,
	eIK_BOW							,
	eIK_ARROW						,
	eIK_SPEAR						,
	eIK_FLUTE						,
	eIK_SLING						,
	eIK_BULLET						,
	eIK_MAGICAL_ROD					,
	eIK_WHIP						,
	eIK_SCYTHE						,
	eIK_ARM_WEAPON				,
	eIK_BOOK						,
	-1
};
int
g_aGenerateRandomSpecialItemList[dGENERATE_RANDOM_SPECIAL_ITEM_LIST_COUNT]	=
{
eIK_HELM						,//"¿½¿½¿?",
eIK_CROWN						,//"¿½¿½",
eIK_GLOVE						,//"¿½°",
eIK_GLOVE_STYLE					,//"¿½°¿½¿½¿?",
eIK_CLAW						,//"¿½¿½¿½¿½",
eIK_BRACELET					,//"¿½¿½¿½¿½",
eIK_BELT						,//"¿½¿½Æ®",
eIK_BOOTS						,//"¿½¿½¿½¿½",
eIK_NECKLACE					,//"¿½¿½¿½¿½¿?",
eIK_RING						,//"¿½¿½¿½¿½",

eIK_EARING						,//"¿½°¿½¿½¿½",
eIK_CLOAK						,//"¿½¿½¿½¿½",
eIK_BROOCH						,//"¿½¿½¿½?",
eIK_ARM_TATOO					,//"¿½¹¿½¿½¿½",
eIK_SHOLDER_TATOO				,//"¿½¿½¿½¿½¿½¿½¿?",
eIK_CROSS						,//"¿½¿½¿½°¿½",
eIK_ARMOR						,//"¿½¿½¿½°¿½¿½",
eIK_EXCLUSIVE_ARMOR				,//"¿½¿½¿½°¿½¿½",
eIK_ONEHANDED_SWORD				,//"¿½¼°¿½",
eIK_SHIELD						,//"¿½¿½¿½¿½",

eIK_TWOHANDED_SWORD				,//"¿½¿½°¿?",
eIK_STAFF						,//"¿½¿½¿½¿½¿½¿½",
eIK_TOOTH						,//"¿½¿½",
eIK_MACE						,//"¿½¿½¿½½¿½",
eIK_WING						,//"¿½¿½¿½¿½",
eIK_KNIFE						,//"¿½¿½",
eIK_BOW							,//"°",
eIK_ARROW						,//"­¿½¿½",
eIK_SPEAR						,//"",
eIK_FLUTE						,//"¿½¸¿½",

eIK_SLING						,//"¿½¿½¿½¿½",
eIK_BULLET						,//"º",
eIK_MAGICAL_ROD					,//"¿½¿½¿½¿½¿½¿½",
eIK_WHIP						,//"¤¿½¿½",
eIK_SCYTHE						,	// "¿½¿½",
eIK_ARM_WEAPON				,//¿½¿¿½¿½¿½.
eIK_BOOK						,	//	
};

const	int		c_aUltimateItemDurability[]	=	
{
	3000,	//0	¿½¿½¿?	
	3000,	//1	¿½¿½	
	4000,	//2	¿½°	
	4000,	//3	¿½° ¿½¿½¿?	
	2400,	//4	¿½¿½¿½¿½	
	4200,	//5	¿½¿½¿½¿½	
	2600,	//6	¿½¿½Æ®	
	2400,	//7	¿½¿½¿½¿½	
	4600,	//8	¿½¿½¿½¿½¿?	
	5400,	//9	¿½¿½¿½¿½	
	6000,	//10	¿½°¿½¿½¿½	
	5000,	//11	¿½¿½¿½¿½	
	4200,	//12	¿½¿½¿½?,
	4200,	//13	¿½¿½ ¿½¿½¿½¿½,
	4200,	//14	¿½¿½¿? ¿½¿½¿½¿½,
	4200,	//15	¿½¿½¿½°¿½,
	2200,	//16	¿½¿½¿½¿½ ¿½¿½¿½¿½	
	2200,	//17	¿½¿½¿½¿½ ¿½¿½¿½¿½	
	6000,	//18	¿½¿½ ¿½¿½ ¿½¿½	
	2000,	//19	¿½¿½¿½¿½	
	6000,	//20	¿½¿½¿? ¿½¿½	
	6000,	//21	¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½	
	6000,	//22	¿½¿½	
	6000,	//23	¿½¿½¿½½¿½	
	6000,	//24	¿½¿½¿½¿½	
	7000,	//25	¿½¿½	
	7600,	//26	°	
	7000,	//27	­¿½¿½	
	6000,	//28		
	6000,	//29	¿½¸¿½	
	7600,	//30	¿½¿½¿½¿½	
	7000,	//31	¿½¿½¿½¿½ º	
	6000,	//32	¿½¿½¿½¿½¿½¿½	
	6000,	//33	¤¿½¿½	
	0x7fffffff,	//34 ¿½¿½¿½¿½
	0x7fffffff,	//35 ¼¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//36 ¿½¿½¡ ¿½¿½¿½¿½
	0x7fffffff,	//37 ¿½¿½¿½¿½ ¿½¿½¿? ¿½¿½¿½¿½
	0x7fffffff,	//38 ¿½¿½¿½¿½ ¿½¿½¿? ¿½¿½¿½¿½
	0x7fffffff,	//39 ¡¿½¿½¿?
	0x7fffffff,	//40 ¿½¿½¿½¿½ ¸¿½¿½¿½¿½
	0x7fffffff,	//41 ¿½¿½¿½¿½
	0x7fffffff,	//42 ¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//43 ¿½¿½¿? ¿½·¸¿½¿½
	0x7fffffff,	//44 ¿½¿½¿½¿½ ¿½¿½/¿½¿½¸
	0x7fffffff,	//45 ¿½·¿½¿½¿½¿½¿½¿?
	0x7fffffff,	//46 ¿½¿½­¿½¿½
	0x7fffffff,	//47 ¿½¿½¿½¿½ ¿½¿½¿½¿½
	0x7fffffff,	//48 ¿½º¿½Æ® ¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//49 ¿½¿½¿½¿½Æ® ¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//50 ¿½¿½¿½¿½¿½¾¿½ ¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//51 ¿½¿½Æ® ¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//52 ¿½¿½¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//53 ¿½¿½¿? ¿½¿½¿½¿½
	6000,	//¿½¿½	
	6000,	//¿½¿¿½¿½¿½.	
	6000,	//	
};

const	int	c_aDX_ItemDurability[]	=
{
	1500,	//0	¿½¿½¿?	
	1500,	//1	¿½¿½	
	2000,	//2	¿½°	
	2000,	//3	¿½° ¿½¿½¿?	
	1200,	//4	¿½¿½¿½¿½	
	2100,	//5	¿½¿½¿½¿½	
	1300,	//6	¿½¿½Æ®	
	1200,	//7	¿½¿½¿½¿½	
	2300,	//8	¿½¿½¿½¿½¿?	
	2700,	//9	¿½¿½¿½¿½	
	3000,	//10	¿½°¿½¿½¿½	
	2500,	//11	¿½¿½¿½¿½
	3000,	//12	¿½¿½¿½?,
	3000,	//13	¿½¿½ ¿½¿½¿½¿½,
	3000,	//14	¿½¿½¿? ¿½¿½¿½¿½,
	3000,	//15	¿½¿½¿½°¿½,
	1100,	//16	¿½¿½¿½¿½ ¿½¿½¿½¿½	
	1100,	//17	¿½¿½¿½¿½ ¿½¿½¿½¿½	
	3000,	//18	¿½¿½ ¿½¿½ ¿½¿½	
	1000,	//19	¿½¿½¿½¿½	
	3000,	//20	¿½¿½¿? ¿½¿½	
	3000,	//21	¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½	
	3000,	//22	¿½¿½	
	3000,	//23	¿½¿½¿½½¿½	
	3000,	//24	¿½¿½¿½¿½	
	3000,	//25	¿½¿½	
	3800,	//26	°	
	3000,	//27	­¿½¿½	
	3000,	//28		
	3000,	//29	¿½¸¿½	
	3800,	//30	¿½¿½¿½¿½	
	3000,	//31	¿½¿½¿½¿½ º	
	3000,	//32	¿½¿½¿½¿½¿½¿½	
	3000,	//33	¤¿½¿½	
	0x7fffffff,	//34 ¿½¿½¿½¿½
	0x7fffffff,	//35 ¼¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//36 ¿½¿½¡ ¿½¿½¿½¿½
	0x7fffffff,	//37 ¿½¿½¿½¿½ ¿½¿½¿? ¿½¿½¿½¿½
	0x7fffffff,	//38 ¿½¿½¿½¿½ ¿½¿½¿? ¿½¿½¿½¿½
	0x7fffffff,	//39 ¡¿½¿½¿?
	0x7fffffff,	//40 ¿½¿½¿½¿½ ¸¿½¿½¿½¿½
	0x7fffffff,	//41 ¿½¿½¿½¿½
	0x7fffffff,	//42 ¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//43 ¿½¿½¿? ¿½·¸¿½¿½
	0x7fffffff,	//44 ¿½¿½¿½¿½ ¿½¿½/¿½¿½¸
	0x7fffffff,	//45 ¿½·¿½¿½¿½¿½¿½¿?
	0x7fffffff,	//46 ¿½¿½­¿½¿½
	0x7fffffff,	//47 ¿½¿½¿½¿½ ¿½¿½¿½¿½
	0x7fffffff,	//48 ¿½º¿½Æ® ¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//49 ¿½¿½¿½¿½Æ® ¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//50 ¿½¿½¿½¿½¿½¾¿½ ¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//51 ¿½¿½Æ® ¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//52 ¿½¿½¿½¿½¿½¿½¿½¿½
	0x7fffffff,	//53 ¿½¿½¿? ¿½¿½¿½¿½
	3000,	//54	¿½¿½	
	3000,	//55	¿½¿¿½¿½¿½
	3000,	//56	
};



//
//	¿½¿½¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿?
DWORD
cITEM_PREFIX::getPriceValue(int _iValue1,int _iValue2)
{
	DWORD	dwPrefixValue	=	0;

	switch(m_wComputePriceMethod)
	{
		case	eCIPP_PRICE							:	//	¿½¿½¿½¿½¿½¿½¿½¿½ ³¿½¿½
			dwPrefixValue	=	m_dwPrice*100;
			break;

		case	eCIPP_VALUE1						:	//	¿½¿½¡1
			dwPrefixValue	=	_iValue1*100;
			break;

		case	eCIPP_VALUE1_MUL_PRICE				:	//	¿½¿½¡1 * ¿½¿½¿½¿½
			dwPrefixValue	=	_iValue1*100*m_dwPrice;
			break;

		case	eCIPP_VALUE1_DIV_PRICE				:	//	¿½¿½¡1 / ¿½¿½¿½¿½
			dwPrefixValue	=	_iValue1*100/m_dwPrice;
			break;

		case	eCIPP_VALUE1_MUL_PRICE_DIV_100		:	//	¿½¿½¡1*¿½¿½¿½¿½/100
			dwPrefixValue	=	_iValue1*m_dwPrice;//*100/100;
			break;

		case	eCIPP_PRICE_MUL_VALUE2_DIV_VALUE1	:	//	¿½¿½¿½¿½*¿½¿½¡2/¿½¿½¡1
			dwPrefixValue	=	m_dwPrice*_iValue2*100/_iValue1;
			break;
	}

	return	dwPrefixValue;
}

BOOL
cItem::Init()
{
	cFILE	file;

	if (file.Open("data/checkItemList.dat","rb"))
	{
		file.Read(&l_iCheckItemCount,4);
		file.Read(l_adwCheckItemList,l_iCheckItemCount*8);
		file.Close();
	}

	return	TRUE;
}

void
cSpecialItem::reset()
{
	m_isBroken						=	FALSE;
	m_bf1IsSpecialItem				=	TRUE;

	m_bf1IsReversionItem			=	FALSE;
	m_bf1IsReversionItemWhenEquip	=	FALSE;
	m_bf1IsSetItem					=	FALSE;	//	¿½¿½Æ® ¿½¿½¿½¿½¿½¿½¿½´¿½.
	m_bf1IsUnknown					=	FALSE;	//	¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½ ¿½ð¸°´¿½.

	m_bf11SValue					=	0;	//	¿½¿½Æ® ¿½¿½¿½¿½¿½¿½¿½³¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½
	m_bf10EquipLevel				=	0;	//	¿½¿½¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½
}

BOOL
cItem::isQuestItem()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(lpBasicItem->m_questIndex	== dNOT_QUEST_ITEM)
		return	FALSE;

	return	TRUE;
}

//
//
cBasicItem*
cItem::getBasicItem(BOOL _bIsWantPrincessWeapon)
{
	if	(m_wBaseItem	>=	dBASIC_ITEM_COUNT)
		return	NULL;

	if	(g_aBasicItem[m_wBaseItem].m_iSerial	==	0xffff)
		return	NULL;

	if	(_bIsWantPrincessWeapon	&&	isPrincessWeapon())
		return	&g_aBasicItem[getPrincessWeapon()->m_wBasicItem];

	return	&g_aBasicItem[m_wBaseItem];
}

int
cItem::getExpandInventorySize()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if (lpBasicItem	==	NULL)
		return	0;

	if (lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_EXPAND_INVENTORY_BAG)
		return	m_aOption[0];

	return	0;
}

int
cItem::getCurrentDurability()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if (!isBreakdownAbleItem())
		return	100;

	int	iFullDurability			=	lpBasicItem->getDurability();

	if	(iFullDurability	==	0)
		return	100;

	int	iValue					=	m_bDurability*100/iFullDurability;

	return	iValue;
}
BOOL
cItem::isExistSpecificEffect(int _iEffect)
{
	cBasicItem			*lpBasicItem=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;iGenData++)
		if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	_iEffect)
			return	TRUE;


	return	FALSE;
}


BOOL
cItem::increaseCarvingLevel(int _iValue)
{
	cBasicItem*	lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;++iGenData)
	{
		if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	eIE_CARVING_LEVEL)
		{
			if(lpBasicItem->m_bf5LimitUpgradeRingOfInfinity < m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[0]] + _iValue)
			{
				m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[0]] = lpBasicItem->m_bf5LimitUpgradeRingOfInfinity;
			}
			else
			{
				m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[0]] += _iValue;
			}
			
			break;
		}
	}

	return TRUE;
}

BOOL
cItem::setCarvingLevel(int _iValue)
{
	cBasicItem*	lpBasicItem	=	getBasicItem();
	
	if	(!lpBasicItem)
		return	FALSE;
	
	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;++iGenData)
	{
		if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	eIE_CARVING_LEVEL)
		{
			if(lpBasicItem->m_bf5LimitUpgradeRingOfInfinity < _iValue)
			{
				m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[0]] = lpBasicItem->m_bf5LimitUpgradeRingOfInfinity;
			}
			else
			{
				m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[0]] = _iValue;
			}
			break;
		}
	}

	return TRUE;
}

int
cItem::getCarvingLevel()
{

	cBasicItem*	lpBasicItem	=	getBasicItem();
	int			iLevel		=	0;

	if	(!lpBasicItem)
		return	0;

	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;++iGenData)
	{
		if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	eIE_CARVING_LEVEL)
		{
			iLevel = m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[0]];
			break;
		}
	}

	return iLevel;
}

int
cItem::getSetCarvingLevel()
{

	cBasicItem*	lpBasicItem	=	getBasicItem();
	int			iValue		=	0;

	if	(!lpBasicItem)
		return	0;

	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;++iGenData)
	{
		if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	eIE_SET_CARVING_LEVEL)
		{
			iValue = m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[1]];
			break;
		}
	}

	return iValue;
}

int
cItem::getSetCarvingChance()
{

	cBasicItem*	lpBasicItem	=	getBasicItem();
	int			iValue		=	0;

	if	(!lpBasicItem)
		return	0;

	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;++iGenData)
	{
		if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	eIE_SET_CARVING_LEVEL)
		{
			iValue = m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[0]];
			break;
		}
	}

	return iValue;
}
int
cItem::getIncreaseCarvingChance()
{

	cBasicItem*	lpBasicItem	=	getBasicItem();
	int			iValue		=	0;

	if	(!lpBasicItem)
		return	0;

	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;++iGenData)
	{
		if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	eIE_INCREASE_CARVING_LEVEL)
		{
			iValue = m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[0]];
			break;
		}
	}

	return iValue;
}

int
cItem::getIncreaseCarvingValue()
{

	cBasicItem*	lpBasicItem	=	getBasicItem();
	int			iValue		=	0;

	if	(!lpBasicItem)
		return	0;

	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;++iGenData)
	{
		if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	eIE_INCREASE_CARVING_LEVEL)
		{
			iValue = m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[1]];
			break;
		}
	}

	return iValue;
}

int
cItem::getIncorrectPrefixCount()
{
	if	(m_wBaseItem	==	0xffff)
		return	0;

	if	(isExceptionItem())
		return	0;

	BOOL	iIncorrectPrefixCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)	
			break;

		if (m_aPrefix[i].m_wPrefix	>=	g_itemPrefix.m_iCount)
			return	0;
		
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];
		cBasicItem		*lpBasicItem=	getBasicItem();

		if (!lpBasicItem)
			return	0;

		if (lpPrefix->isStickAbleItem(lpBasicItem->m_wKind) == FALSE)
			iIncorrectPrefixCount++;
	}

	return	iIncorrectPrefixCount;
}

BOOL
cItem::makeMagicCarpetEmblem()
{
	reset();

	int	iEmblem	=	-1;

	for (int i=0;i<dBASIC_ITEM_COUNT;i++)
	{
		if (g_aBasicItem[i].m_iSerial	==	0xfffffffff)
			continue;

		if (g_aBasicItem[i].m_attr.isMagicCarpetEmblem)
		{
			iEmblem	=	i;
			break;
		}
	}

	if (iEmblem	==	-1)
		return	FALSE;

	m_wBaseItem	=	iEmblem;
	m_aOption[0]=	1;

	return	TRUE;
}

void
cItem::removeItemInCheckList()
{
	int	iCount	=	l_iCheckItemCount;

	for (int i=0;i<l_iCheckItemCount;)
	{
		if (m_wBaseItem	==	l_adwCheckItemList[i][0]	&&	m_dwSerial	==	l_adwCheckItemList[i][1])
		{
			memcpy(&l_adwCheckItemList[i][0],&l_adwCheckItemList[i+1][0],8*(l_iCheckItemCount-i-1));

			l_iCheckItemCount--;
		}
		else	i++;
	}

	cFILE	file;

	if (file.Open("data/checkItemList.dat","wb"))
	{
		file.Write(&l_iCheckItemCount,4);
		file.Write(l_adwCheckItemList,l_iCheckItemCount*8);
		file.Close();
	}
}

BOOL
cItem::isCheckListItem()
{
	for (int i=0;i<l_iCheckItemCount;i++)
	{
		if (m_wBaseItem	==	l_adwCheckItemList[i][0]	&&
			m_dwSerial	==	l_adwCheckItemList[i][1])
			return	TRUE;
	}

	return	FALSE;
}

int
cItem::reviseIncorrectPrefix()
{
	cBasicItem		*lpBasicItem=	getBasicItem();

	if (!lpBasicItem)
		return	0;

	if (isExceptionItem())
		return	0;

	int	iIncorrectPrefixCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)	
			break;

		if (m_aPrefix[i].m_wPrefix	>=	g_itemPrefix.m_iCount)
			return	0;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if (lpPrefix->isStickAbleItem(lpBasicItem->m_wKind) == FALSE)
		{
			m_aPrefix[i].m_wPrefix	=	0xffff;

			for (int j=i;j<dITEM_PREFIX_COUNT-1;j++)
				memcpy(&m_aPrefix[j],&m_aPrefix[j+1],sizeof(CItemPrefixInfo));

			memset(&m_aPrefix[dITEM_PREFIX_COUNT-1],0xff,sizeof(CItemPrefixInfo));

			iIncorrectPrefixCount++;
		}
		else	i++;
	}

	return	iIncorrectPrefixCount;
}
BOOL
cItem::isCanUseLevel(int _iLevel, int _iLimitEquipmentLevelOverPoint)
{
	cBasicItem	*lpBasicItem		=	getBasicItem(TRUE);

	if	(!lpBasicItem)
		return	FALSE;

	
	if(getRequireLevel() > _iLevel+_iLimitEquipmentLevelOverPoint || (lpBasicItem->m_wRequireMaxLevel && lpBasicItem->m_wRequireMaxLevel < _iLevel))
		return FALSE;

	return	TRUE;
}
BOOL
cItem::isNormalItem()//¿½°¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½¿½³¿½
{	
	if	(isExceptionItem())
		return	TRUE;

	if	(m_aPrefix[0].m_wPrefix	!=	0xffff)
		return	FALSE;	//	¿½¿½¿½¿½.. ¿½¿½ -o-

	cBasicItem	*lpItem		=	getBasicItem();
	CRY(!lpItem,"cry in cItem::isNormalItem - ¿½Æ¾¿½!! ¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½ ¿½¿½¿½´°¿½!!");

	if (lpItem->m_aUniqueData[0].m_wEffect	!=	0xffff)
		return	FALSE;	//	¿½¿½¿½¿½... ¿½¿½ -o-

	return	TRUE;
}

BOOL
cItem::isLittleGoodItem()
{	//  ¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½ ¼........09.09.10
	if	(isExceptionItem())
		return	FALSE;

	if	(isExtraItem())
		return	FALSE;

	if	(m_aPrefix[0].m_wPrefix	==	0xffff)	
		return	FALSE;

	if(isUniqueItem())
		return FALSE;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)	
			break;

		if	(g_aItemPrefix[m_aPrefix[i].m_wPrefix].m_wDiscernmentCode	>=	10000)
			return	TRUE;
		else
			return	FALSE;
	}

	return	FALSE;
}
	
BOOL
cItem::isRareItem()	
{
	if (isExceptionItem())
		return	FALSE;

	if	(isExtraItem())
		return	FALSE;

	if	(m_aPrefix[0].m_wPrefix	==	0xffff)	
		return	FALSE;

	if (isLittleGoodItem())
		return	FALSE;

	return	TRUE;
}
BOOL
cItem::isUniqueItem()//¿½°¿½ ¿½¿½¿½¿½¿½¿½?
{
	if	(isExceptionItem())
		return	FALSE;

	if	(isExtraItem())
		return	FALSE;
	
	cBasicItem	*lpItem		=	getBasicItem();
	CRY(!lpItem,"cry in cItem::isNormalItem - ¿½Æ¾¿½!! ¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½ ¿½¿½¿½´°¿½!!");


	return lpItem->isUnique();
}

BOOL
cItem::isDXUniqueItem()//¿½°¿½ DX ¿½¿½¿½¿½¿½¿½?
{
	if	(!isUniqueItem())
		return	FALSE;

	if	(isExtraItem())
		return	FALSE;

	if	(getBasicItem()->m_attr.isDXItem)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::isSuperUniqueItem()//¿½°¿½ ¿½¿½¿½¿½¿½¿½?
{
	if	(isExceptionItem())
		return	FALSE;

	if	(isExtraItem())
		return	FALSE;
	
	cBasicItem	*lpItem		=	getBasicItem();
	CRY(!lpItem,"cry in cItem::isNormalItem - ¿½Æ¾¿½!! ¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½ ¿½¿½¿½´°¿½!!");

	if(lpItem->isUnique() == FALSE)
		return FALSE;

	if (m_aPrefix[0].m_wPrefix	==	0xffff)	
		return	FALSE;

	return	TRUE;
}

BOOL
cItem::isSuperUniqueDXItem()
{
	if	(isExceptionItem())
		return	FALSE;

	if	(isExtraItem())
		return	FALSE;
	
	cBasicItem	*lpItem		=	getBasicItem();
	CRY(!lpItem,"cry in cItem::isNormalItem - ¿½Æ¾¿½!! ¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½ ¿½¿½¿½´°¿½!!");

	if(lpItem->isUnique() == FALSE)
		return FALSE;

	if	(m_aPrefix[0].m_wPrefix	==	0xffff)	
		return	FALSE;

	if	(lpItem->m_attr.isDXItem	==	FALSE)
		return	FALSE;
	if	(lpItem->m_bf1IsUltimate)
		return	FALSE;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)
			continue;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if (lpPrefix->m_wIsDXPrefix)
			return	TRUE;
	}

	return	TRUE;
}

BOOL
cItem::isSuperUniqueUltimateItem()
{
	if	(isExceptionItem())
		return	FALSE;

	if	(isExtraItem())
		return	FALSE;
	
	cBasicItem	*lpItem		=	getBasicItem();
	CRY(!lpItem,"cry in cItem::isNormalItem - ¿½Æ¾¿½!! ¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½ ¿½¿½¿½´°¿½!!");

	if(lpItem->isUnique() == FALSE)
		return FALSE;

	if	(m_aPrefix[0].m_wPrefix	==	0xffff)	
		return	FALSE;

	if	(lpItem->m_bf1IsUltimate==	FALSE)
		return	FALSE;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)
			continue;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if	(lpPrefix->m_wIsUltimatePrefix)
			return	TRUE;
	}

	return	TRUE;
}


BOOL
cItem::isPremiumItem()
{
	if	(isBreedingRecordBook()	||	isBlockLightPocket())
		return	FALSE;

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(lpBasicItem->m_wKind	==	eIK_PREMIUM_ITEM)
		return	TRUE;

	return	FALSE;
}

//	¿½¿½Æ® ¿½¿½¿½±¿½
BOOL
cItem::decreaseCount(int _iCount,BOOL _isBeltItem)
{
	if	(isExceptionItem()	||	isExtraItem() || isMagicCarpetEmblem())
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

BOOL
cItem::addItem(cItem *_lpItem,int _iLimitCount)		//	¿½¿½¿½¿½¿½¿½ ¿½¿½¡¿½¿½..?
{
	if	(isExtraItem() || isMagicCarpetEmblem() || isExceptionItem())
		return FALSE;

	if	(_lpItem->m_bCount	<=	0)
		return FALSE;

	if	(_iLimitCount	==	0xffff)	
		_iLimitCount	=	getStackLimit();

	int	iSpareCount	=	_iLimitCount-m_bCount;

	if	(_lpItem->m_bCount	<	iSpareCount)
		iSpareCount	=	_lpItem->m_bCount;

	if	(iSpareCount==	0)
		return	FALSE;

	int	iDurability	=	m_bCount;
	iDurability		*=	m_bDurability;
	iDurability		=	iDurability+iSpareCount*_lpItem->m_bDurability;

	m_bCount		+=	iSpareCount;
	iDurability		/=	m_bCount;
	m_bDurability	=	iDurability;

	_lpItem->decreaseCount(iSpareCount);

	return	TRUE;
}

BOOL
cItem::isExistSameTypePrefix(int _iPrefix)
{
	if (isExceptionItem())
		return	FALSE;

	int	iPrefixType	=	g_aItemPrefix[_iPrefix].m_wType;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)	continue;

		if (g_aItemPrefix[m_aPrefix[i].m_wPrefix].m_wType	==	iPrefixType)	return	TRUE;
	}

	return	FALSE;
}

int
cItem::checkIncorrectItem(cACTOR *_lpOwner,int _iItemSlot,int *_lpiSameItemCount,int *_lpiReviseIncorrectPrefixCount)
{
	if	(isExceptionItem())
		return	eII_CORRECT;

	if	(isExpiredItem())
		return	eII_EXPIRED;

	BOOL	bIsBelt	=	FALSE;

	if	(_iItemSlot >= dEQUIP_BELT_SLOT1+dBORDER_OF_ITEM_AND_EQUIPMENT)
		bIsBelt	=	TRUE;

	if	(reviseIncorrectData(bIsBelt)	==	eRIR_REVISE_INCORRECT_PREFIX)
		*_lpiReviseIncorrectPrefixCount++;

	if	(m_wBaseItem	==	0xffff	)
		return	eII_CORRECT;

	if	(m_wBaseItem	==	0)
		return	eII_IS_MONEY;

	cBasicItem	*lpBasicItem=	getBasicItem();

	if	(lpBasicItem->m_questIndex!=	dNOT_QUEST_ITEM && lpBasicItem->m_questIndex	<	dNOT_QUEST_ITEM )	//	¿½¿½¿½¿½Æ® ¿½¿½¿½¿½¿½¿½¿½´¿½.
		if	(_lpOwner->getProcessQuest(lpBasicItem->m_questKind,lpBasicItem->m_questIndex) == NULL)
			return	eII_INVALID_QUEST;

	if	(m_dwSerial	!=	0)
	{
		int	iSameSerialItem	=	_lpOwner->findSameSerialItem(m_dwSerial,_iItemSlot);

		if	(iSameSerialItem	!=	0xffff)
		{
			*_lpiSameItemCount++;

			return	eII_DOUBLE_ITEM;
		}
	}

	return	eII_CORRECT;
}

BOOL
cItem::getGuildStatueExporterInfo(int *_lpiType,int *_lpiLevel)
{
	cBasicItem	*lpBasicItem=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_GUILD_STATUE_PIECE_EXPORTER)
	{
		*_lpiType	=	m_aOption[0];
		*_lpiLevel	=	m_aOption[1];

		return	TRUE;
	}

	return	FALSE;
}

BOOL
cItem::checkItemCount()
{
	if	(isExtraItem() || isExceptionItem())
		return	FALSE;
	
	if (m_bCount	<=	0)
	{
		reset();
		return	TRUE;
	}

	return	FALSE;
}

void
cItem::increaseMinute(int _iMinute)
{
	if	(isExceptionItem())
		return;

	if	(isSpecialItem())
		return;

	for (int iTime=0;iTime<_iMinute;iTime++)
	{
		m_minute++;

		if (m_minute	>=	60)
		{
			m_minute	=	0;
			m_hour++;

			if (m_hour	>=	24)
			{
				m_hour	=	0;
				
				int	iLastDay=	GetLastDay(m_year,m_month);

				if (m_day+1	>	(DWORD)iLastDay)
				{
					m_day	=	1;
					m_month++;

					if (m_month	>	12)
					{
						m_month	=	1;
						m_year++;
					}
				}
				else	m_day++;
			}
		}
	}
}

//
//	¿½¿½¿½¿½ º¿½³¿½?
BOOL
cItem::isInfinityBullet()
{
	int			i;
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(lpBasicItem->isEternalItem())
		return	TRUE;

	if	(isExceptionItem())
		return	FALSE;

	if	(isExtraItem())
		return	TRUE;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if	(m_aPrefix[i].m_wPrefix	==	0xffff)
			continue;

		if	(g_aItemPrefix[m_aPrefix[i].m_wPrefix].m_wType	==	eIP_INFINITY_BULLET)
			return	TRUE;
	}

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
		if	(lpBasicItem->m_aGenerateData[i].m_wEffect		==	eIE_INFINITY_BULLET)	
			return	TRUE;

	for (i=0;i<dITEM_UNIQUE_DATA_COUNT;i++)
		if	(lpBasicItem->m_aUniqueData[i].m_wEffect		==	eIP_INFINITY_BULLET)	
			return	TRUE;

	return	FALSE;
}

int
cItem::getRequireLevel()
{
	cBasicItem	*lpBasicItem		=	getBasicItem(TRUE);

	if	(!lpBasicItem)
		return	0;

	int				iRequireLevel	=	lpBasicItem->m_wRequireLevel+getRequireLevelByPrefix();

	cSpecialItem	*lpSpecialItem	=	getSpecialItem();

	if	(!lpSpecialItem || lpSpecialItem->m_bf10EquipLevel == 0 || lpSpecialItem && !lpSpecialItem->m_bf1IsSetItem )
		return	iRequireLevel;

	return	lpSpecialItem->m_bf10EquipLevel;
}

//
//	¿½¿½¿½¿½¿? ¿½¿½¿½¿½ ¿½¿½¿½¿½/¿½¿½¿? ¿½¿¿½¿½¿½ ¿½¿½¿½¿½
int
cItem::getRequireLevelByPrefix()
{
	if	(isExceptionItem())
		return	0;

	int	iPrefixLevel	=	0,i;
	int	aPrefixLevel[3]	=	{0,0,0};
	int	iMaxPrefixIndex	=	0;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		CItemPrefixInfo	*lpPrefixInfo	=	&m_aPrefix[i];

		if (lpPrefixInfo->m_wPrefix	==	0xffff)
			break;

		cITEM_PREFIX		*lpPrefix		=	&g_aItemPrefix[lpPrefixInfo->m_wPrefix];

		if (lpPrefix->m_wDropLevel	>	iPrefixLevel)
		{
			iPrefixLevel	=	lpPrefix->m_wDropLevel;
			iMaxPrefixIndex	=	i;
		}

		aPrefixLevel[i]	=	lpPrefix->m_wDropLevel;
	}

	int	iFirst,iSecond;

	if	(iMaxPrefixIndex	==	0)
		iFirst	=	1,iSecond	=	2;
	if	(iMaxPrefixIndex	==	1)
		iFirst	=	0,iSecond	=	2;
	if	(iMaxPrefixIndex	==	2)
		iFirst	=	0,iSecond	=	1;

	if	(isUltimateItem())
	{
		if	(aPrefixLevel[iFirst] && aPrefixLevel[iSecond])
		{
			if	(aPrefixLevel[iFirst] > aPrefixLevel[iSecond])
				iPrefixLevel	=	iPrefixLevel*75/100+aPrefixLevel[iFirst]*25/100+aPrefixLevel[iSecond]*5/100;
			else
				iPrefixLevel	=	iPrefixLevel*75/100+aPrefixLevel[iSecond]*25/100+aPrefixLevel[iFirst]*5/100;
		}

		if (aPrefixLevel[iFirst] && aPrefixLevel[iSecond]==0)
		{
			iPrefixLevel	=	iPrefixLevel*75/100+	aPrefixLevel[iFirst]*25/100+aPrefixLevel[iSecond]*5/100;
		}
	}
	else
	{
		if	(aPrefixLevel[iFirst] && aPrefixLevel[iSecond])
		{
			if	(aPrefixLevel[iFirst] > aPrefixLevel[iSecond])
				iPrefixLevel	=	iPrefixLevel+aPrefixLevel[iFirst]*2/3+aPrefixLevel[iSecond]/3;
			else
				iPrefixLevel	=	iPrefixLevel+aPrefixLevel[iSecond]*2/3+aPrefixLevel[iFirst]/3;
		}

		if (aPrefixLevel[iFirst] && aPrefixLevel[iSecond]==0)
		{
			iPrefixLevel	=	iPrefixLevel+	aPrefixLevel[iFirst]*2/3+aPrefixLevel[iSecond]/3;
		}
	}

	return	iPrefixLevel;
}

int
cItem::getJudgePrice()
{
	if	(isUnknownItem()	==	FALSE)
		return	0;

	cPieceItem	*lpPiece	=	getPieceItem();

	if	(lpPiece)
		return	lpPiece->getJudgePrice(m_aOption[0]);

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	0;

	return	lpBasicItem->m_wDropLevel*100;
}

//
//	¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½´¿?.
DWORD
cItem::getPrice(int _iCount)
{
//	¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¸¿½ ¿½¿½¿½¿½¿½¿½ 1¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½¿¿? ¿½¿½¿½±¿½ ¿½¿½¿½¿½¿½¿½ ¿½Ø¾¿½ ¿½¿½¿½¿½¿½¿½,
//	¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ 10¿½¿½ ¿½´¿½ ¿½¼®¿½¿½ ¿½¾¼­(0¿½¿½ ¿½¿½¿½¼¿½¿½¿½ ¿½´¿½.)
//	¿½º ¿½¿½¿½°¿½ ¿½¿½¿½¿½¿½´¿½ ¿½¿½¿½¿¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½±¿½¿? ¿½´¿½.

	DWORD	dwPrice;
	int		i;

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	0;

	if	(isExceptionItem())
		return	0;

	dwPrice	=	lpBasicItem->m_dwPrice;

	switch(lpBasicItem->m_wPriceComputeMethod)
	{
		case	eCBIP_PRICE							:	//	¿½¿½¿½¿½¿½¿½¿½¿½ ³¿½¿½
			break;

		case	eCBIP_VALUE1						:	//	¿½¿½¡1
			dwPrice	=	m_aOption[0];
			break;

		case	eCBIP_PRICE_MUL_VALUE1				:	//	¿½¿½¿½¿½ * ¿½¿½¡1
			dwPrice	*=	m_aOption[0];
			break;

		case	eCBIP_PRICE_MUL_VALUE2				:	//	¿½¿½¿½¿½ * ¿½¿½¡2
			dwPrice	*=	m_aOption[1];
			break;

		case	eCBIP_PRICE_MUL_VALUE1_PLUS1		:	//	¿½¿½¿½¿½ * (¿½¿½¡1 +1)
			dwPrice	*=	(m_aOption[0]+1);
			break;

		case	eCBIP_VALUE1_MUL_VALUE2_DIV_PRICE	:	//	¿½¿½¡1*¿½¿½¡2/¿½¿½¿½¿½
			dwPrice	=	m_aOption[0]*m_aOption[1]/dwPrice;
			break;
		case	eCBIP_PRICE_DIV_10					:	//	¿½¿½¿½¿½ / 10
			dwPrice	=	dwPrice*_iCount/10;
			break;
		case	eCBIP_VALUE1_MUL_VALUE2_MUL_PRICE	:	//	¿½¿½¡1*¿½¿½¡2*¿½¿½¿½¿½
			dwPrice	=	m_aOption[0]*m_aOption[1]*dwPrice;
			break;
	}

	dwPrice	=	max(dwPrice,1);

	if (lpBasicItem->m_wPriceComputeMethod	!=	eCBIP_PRICE_DIV_10)
		dwPrice	*=	_iCount;	//	/10 ¿½´¿½ ¿½¶¿½¿½¿½ ¿½¸¿½ *¿½¿½Æ® ¿½¿½¿½Ø´¿½.

	DWORD	dwAddPrice	=	0,dwMultiPrice	=	0;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		CItemPrefixInfo	*lpData		=	&m_aPrefix[i];

		if	(lpData->m_wPrefix	==	0xffff)
			break;

		cITEM_PREFIX	*lpPrefix		=	&g_aItemPrefix[lpData->m_wPrefix];
		DWORD			dwPrefixValue	=	lpPrefix->getPriceValue(lpData->m_aValue[0],lpData->m_aValue[1]);

		if	(lpPrefix->m_wPriceFactor	==	0)
			dwAddPrice	+=	dwPrefixValue;
		else
			dwMultiPrice+=	dwPrefixValue;
	}

	dwMultiPrice/=	100;
	dwAddPrice	=	dwAddPrice*_iCount/100;

	dwPrice		=	(dwPrice+dwAddPrice)*(1+dwMultiPrice);
	
	if (isUniqueItem())
		dwPrice	=	dwPrice*lpBasicItem->m_wPriceValue/100;

	return	dwPrice;
}

BOOL
cItem::isIncreaseSelectPowerItem()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	for	(int	i=0;i<dGENERATE_ITEM_DATA_COUNT;++i)
	{
		if	(lpItem->m_aGenerateData[i].m_wEffect	==	eIE_INCREASE_SELECT_POWER)
			return	TRUE;
		if	(lpItem->m_aGenerateData[i].m_wEffect	==	eIE_INCREASE_FIX_SELECT_POWER)
			return	TRUE;
	}

	return	FALSE;
}


BOOL
cItem::isBottomlessBox()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	for	(int	i=0;i<dGENERATE_ITEM_DATA_COUNT;++i)
	{
		if	(lpItem->m_aGenerateData[i].m_wEffect	==	eIE_MIX_ITEM_BOX)
			return	TRUE;
	}

	return	FALSE;
}

BOOL
cItem::isCureStrangeStatusItem()
{
	return	isOwnSpecificEffectItem(eIE_RECOVER_ALL_STRANGE_STATUS);
}

BOOL
cItem::isCurePoisonStatusItem()
{
	return	isOwnSpecificEffectItem(eIE_HEAL_POSION);
}

BOOL
cItem::isCureDeclinePowerStatusItem()
{
	return	isOwnSpecificEffectItem(eIE_RECOVER_ALL_DECLINE_STATUS);
}

BOOL
cItem::isCureCurseStatusItem()
{
	return	isOwnSpecificEffectItem(eIE_RECOVER_ALL_CURSE_STATUS);
}

BOOL
cItem::isNeutralizeWrongStatusItem()
{
	return	isOwnSpecificEffectItem(eIE_HEAL_STRANGE_STATUS);
}

BOOL
cItem::isCureAllWrongStatusItem()
{
	return	isOwnSpecificEffectItem(eIE_RECOVER_ALL_BADSTATUS);
}

BOOL
cItem::isLimitTermItem()		//	¿½° ¿½¿½¿½¿½¿½¿½ ¿½´¿½ ¿½¿½¿½¿½¿½¿½
{
	if	(isExceptionItem())
		return	FALSE;
	if	(isExtraItem())
		return	FALSE;
	if	(m_bf1IsSpecialItem)
		return	FALSE;

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return FALSE;

	if	(lpBasicItem->m_wExpireYear)
		return	TRUE;

	if	(m_year)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::isExpiredItem()
{
	if	(isExceptionItem())
		return	FALSE;
	if	(isExtraItem())
		return	FALSE;
	if	(isSpecialItem())
		return	FALSE;

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if(!lpBasicItem)
		return FALSE;

	int			iYear			=	g_currentTime.m_wYear-2000;
	int			iMonth			=	g_currentTime.m_wMonth;
	int			iDay			=	g_currentTime.m_wDay;
	int			iHour			=	g_currentTime.m_wHour;
	int			iMinute			=	g_currentTime.m_wMinute;

//	¿½¿½¿¿½° ¼
	if	(iYear	>	0)
	{
		int		iCurrentTimeValue	=	iYear*1000000						+iMonth*10000						+iDay*100						+iHour;
		int		iExpireTimeValue	=	(lpBasicItem->m_wExpireYear-2000)*1000000  +lpBasicItem->m_wExpireMonth*10000  +lpBasicItem->m_wExpireDay*100  +lpBasicItem->m_wExpireHour;

		if	(iExpireTimeValue > 0)
		{
			if	(iCurrentTimeValue > iExpireTimeValue)
				return TRUE;
		}

		iCurrentTimeValue	=	iYear*100000000	+iMonth*1000000	+iDay*10000	+iHour*100	+iMinute;
		iExpireTimeValue	=	m_year*100000000+m_month*1000000+m_day*10000+m_hour*100	+m_minute;

		if	(iExpireTimeValue != 0 && iCurrentTimeValue > iExpireTimeValue)
			return TRUE;
	}

	return	FALSE;
}

BOOL
cItem::reviseSetItemIncorrectData()
{
	cSetItem*	lpSetItem	=	getSetItem();

	if	(!lpSetItem)
		return	FALSE;

	cItemSet*	lpItemSet	=	getItemSet();

	if	(!lpItemSet)
		return	FALSE;

	//	2010¿½¿½ 2¿½¿½ 9¿½¿½
	//	¿½¿½¿½¿½¿½¿½ ¿½¿½Æ® ¿½¿½¿½¿½¿½¿½ ¿½·±¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿? ¿½¿½¿½¿½¿½±¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½
	//	.. ¿½¿½ ¿½¿½ ¿½¿½¿½´¿½. ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½
	if	(lpItemSet->m_wSerial	<	163	||	lpItemSet->m_wSerial	>	170)
		return	FALSE;

	cItem	item;

	lpSetItem->generateItem(&item,lpItemSet->m_wSerial);

	if	(memcmp(item.m_aPrefix,m_aPrefix,sizeof(m_aPrefix))	==	0)
		return	FALSE;

	BYTE	aOptionValuePush[2],bDurability=m_bDurability;

	memcpy(aOptionValuePush,m_aOption,sizeof(m_aOption));	//	¿½¿½Æ® ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿? ¿½¿½¿½´¿½ ¿½¼¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿? ¿½Ð´¿½.

	lpSetItem->generateItem(this,lpItemSet->m_wSerial);

	memcpy(m_aOption,aOptionValuePush,sizeof(m_aOption));	//	¿½¼¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½

	m_bDurability	=	bDurability;

	return	TRUE;
}

//
//	¿½ß¸¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½
int
cItem::reviseIncorrectData(BOOL _bIsBelt)
{
	int		j,k;
	BOOL	bIsReveise	=	FALSE;

	if (m_wBaseItem	==	0xffff				)
		return	FALSE;

	if (m_wBaseItem	>=	dBASIC_ITEM_COUNT	)
	{
		reset();
		return	TRUE;
	}
	
	if	(isExceptionItem())
		return	FALSE;

	if	(reviseSetItemIncorrectData())
		return	TRUE;

	if	(!isExtraItem() && m_bCount	==	0	)
	{
		if	(!_bIsBelt)
		{
			reset();

			return	TRUE;
		}
	}

//	¿½ß¸¿½¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½
	if	(isInfinityBullet()		)
		m_bCount	=	1;

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(m_dwSerial	==	0xffffffff)	
		m_dwSerial	=	1;

	if	(lpBasicItem->m_attr.isDurabilityToUseCount		)	//	¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½Æ®¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿?
	{
		if	(m_bDurability	==	0)
		{
			reset();

			return TRUE;
		}

		if	(m_bDurability	>	lpBasicItem->getDurability())
		{
			reset();

			return TRUE;
		}
	}

	for (j=0;j<dGENERATE_ITEM_DATA_COUNT;j++)
	{
		cITEM_GENERATE_DATA	*lpGenerateData	=	&lpBasicItem->m_aGenerateData[j];

		if	(lpGenerateData->m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex,iMinValue,iMaxValue,iRange;

		iReferenceIndex	=	lpGenerateData->m_aValue[0];
		iMinValue		=	min(lpBasicItem->m_aValue[iReferenceIndex][0],255);
		iMaxValue		=	min(lpBasicItem->m_aValue[iReferenceIndex][1],255);
		iMaxValue		=	max(iMaxValue,iMinValue);
		if(	lpGenerateData->m_wEffect == eIE_CARVING_LEVEL)			// ¿½¿½¿½·¿½¿½¿½¿½¸¿½..
		{
			if(m_aOption[iReferenceIndex] > lpBasicItem->m_bf5LimitUpgradeRingOfInfinity)
				m_aOption[iReferenceIndex]	=	1;

			break;
		}
		iRange			=	iMaxValue-iMinValue+1;

		if	(m_aOption[iReferenceIndex]	>	iMaxValue	||	m_aOption[iReferenceIndex]	<	iMinValue)
		{
			m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange),iMaxValue);
			bIsReveise		=	TRUE;
		}

		if (lpGenerateData->m_wEffect	==	eIE_USE_JEWEL)
		{
			if (m_bDurability	>	m_aOption[1])	
			{
				m_bDurability	=	m_aOption[1];
				bIsReveise		=	TRUE;
			}
		}
	}

	for (j=0;j<dITEM_PREFIX_COUNT;j++)
	{
		int	iPrefix	=	m_aPrefix[j].m_wPrefix;

		if (m_aPrefix[j].m_wPrefix	>=	g_itemPrefix.m_iCount)
		{
			m_aPrefix[j].m_wPrefix	=	0xffff;
			iPrefix					=	m_aPrefix[j].m_wPrefix;
		}

		if (iPrefix	!=	0xffff)
		{
			cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

			for (k=0;k<2;k++)
			{
				int	iMinValue,iMaxValue,iRange;

				iMinValue	=	min(lpPrefix->m_aValue[k][0],255);
				iMaxValue	=	min(lpPrefix->m_aValue[k][1],255);
				iMaxValue	=	max(iMaxValue,iMinValue);
				iRange		=	iMaxValue-iMinValue+1;

				if (m_aPrefix[j].m_aValue[k]	>	iMaxValue	||	m_aPrefix[j].m_aValue[k]	<	iMinValue)
				{
					bIsReveise	=	TRUE;
					m_aPrefix[j].m_aValue[k]	=	min(iMinValue+random(iRange),iMaxValue);
				}
			}
		}
	}

	if (!isExtraItem() && !isMagicCarpetEmblem())
		if (m_bCount>lpBasicItem->m_wStackLimit)
		{
			m_bCount	=	(BYTE)lpBasicItem->m_wStackLimit;
			bIsReveise	=	TRUE;
		}

	if (bIsReveise)
		CLOG("revise item data",lpBasicItem->m_strName);

	return	bIsReveise;
}
int
cItem::getEffectMiniPetFeedType()
{

	cBasicItem*	lpBasicItem	=	getBasicItem();
	int			iValue		=	0xffff;

	if	(!lpBasicItem)
		return	iValue;

	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;++iGenData)
	{
		if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	eIE_MINIPET_FEED)
		{
			iValue = m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[0]];
			break;
		}
	}

	return iValue;
}
int
cItem::getMiniPetFeedExp()
{

	cBasicItem*	lpBasicItem	=	getBasicItem();
	int			iValue		=	0xffff;

	if	(!lpBasicItem)
		return	iValue;

	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;++iGenData)
	{
		if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	eIE_MINIPET_FEED)
		{
			iValue = m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[1]];
			break;
		}
	}

	return iValue;
}
int
cItem::getSetMiniPetType()
{

	cBasicItem*	lpBasicItem	=	getBasicItem();
	int			iValue		=	0xffff;

	if	(!lpBasicItem)
		return	iValue;

	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;++iGenData)
	{
		if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	eIE_SET_MINIPET_TYPE)
		{
			iValue = m_aOption[lpBasicItem->m_aGenerateData[iGenData].m_aValue[0]];
			break;
		}
	}

	return iValue;
}
int
cItem::getFixedStrength()
{
	if (isExceptionItem())
		return	0xffff;

	int	j,iValue	=	0;

	for (j=0;j<dITEM_PREFIX_COUNT;j++)
	{
		int	iPrefix	=	m_aPrefix[j].m_wPrefix;

		if (m_aPrefix[j].m_wPrefix	!=	eIP_FIXING_STRENGTH)
			continue;

		iValue	=	max(iValue,m_aPrefix[j].m_aValue[0]);
	}

	if (iValue	==	0)
		iValue	=	0xffff;

	return	iValue;
}

int
cItem::getFixedAgility()
{
	if (isExceptionItem())
		return	0xffff;

	int	j,iValue	=	0;

	for (j=0;j<dITEM_PREFIX_COUNT;j++)
	{
		int	iPrefix	=	m_aPrefix[j].m_wPrefix;

		if (m_aPrefix[j].m_wPrefix	!=	eIP_FIXING_AGILITY)	continue;

		iValue	=	max(iValue,m_aPrefix[j].m_aValue[0]);
	}

	if (iValue	==	0)	iValue	=	0xffff;

	return	iValue;
}

int
cItem::getFixedWisdom()
{
	if (isExceptionItem())
		return	0xffff;

	int	j,iValue	=	0;

	for (j=0;j<dITEM_PREFIX_COUNT;j++)
	{
		int	iPrefix	=	m_aPrefix[j].m_wPrefix;

		if (m_aPrefix[j].m_wPrefix	!=	eIP_FIXING_WISDOM)	continue;

		iValue	=	max(iValue,m_aPrefix[j].m_aValue[0]);
	}

	if (iValue	==	0)	iValue	=	0xffff;

	return	iValue;
}

int
cItem::getFixedIntelligence()
{
	if (isExceptionItem())
		return	0xffff;

	int	j,iValue	=	0;

	for (j=0;j<dITEM_PREFIX_COUNT;j++)
	{
		int	iPrefix	=	m_aPrefix[j].m_wPrefix;

		if (m_aPrefix[j].m_wPrefix	!=	eIP_FIXING_INTELLIGENCE)	continue;

		iValue	=	max(iValue,m_aPrefix[j].m_aValue[0]);
	}

	if (iValue	==	0)	iValue	=	0xffff;

	return	iValue;
}

int
cItem::getFixedConstitution()
{
	if (isExceptionItem())
		return	0xffff;

	int	j,iValue	=	0;

	for (j=0;j<dITEM_PREFIX_COUNT;j++)
	{
		int	iPrefix	=	m_aPrefix[j].m_wPrefix;

		if (m_aPrefix[j].m_wPrefix	!=	eIP_FIXING_CONSTITUTION)	continue;

		iValue	=	max(iValue,m_aPrefix[j].m_aValue[0]);
	}

	if (iValue	==	0)	iValue	=	0xffff;

	return	iValue;
}

int
cItem::getFixedCharisma()
{
	if (isExceptionItem())
		return	0xffff;

	int	j,iValue	=	0;

	for (j=0;j<dITEM_PREFIX_COUNT;j++)
	{
		int	iPrefix	=	m_aPrefix[j].m_wPrefix;

		if (m_aPrefix[j].m_wPrefix	!=	eIP_FIXING_CHARISMA)	continue;

		iValue	=	max(iValue,m_aPrefix[j].m_aValue[0]);
	}

	if (iValue	==	0)	iValue	=	0xffff;

	return	iValue;
}

int
cItem::getFixedLuck()
{
	if (isExceptionItem())
		return	0xffff;

	int	j,iValue	=	0;

	for (j=0;j<dITEM_PREFIX_COUNT;j++)
	{
		int	iPrefix	=	m_aPrefix[j].m_wPrefix;

		if (m_aPrefix[j].m_wPrefix	!=	eIP_FIXING_LUCK)	continue;

		iValue	=	max(iValue,m_aPrefix[j].m_aValue[0]);
	}

	if (iValue	==	0)	iValue	=	0xffff;

	return	iValue;
}

BOOL
cItem::isStackAbleItem()		//	¿½¿½¿½¿½ ¿½´¿½ ¿½¿½¿½¿½¿½¿½¿½³¿½?
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(isLittleGoodItem() || isSuperUniqueItem()  || isRareItem()  || isUniqueItem()  || isExtraItem() || isSpecialItem() || isExceptionItem())	//	¿½¿½ ¿½¿½ ¿½´¿½ ¿½¿½¿½¿½¿½¿½¿½´¿½.
		return	FALSE;

	if (lpBasicItem->m_wStackLimit > 1)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::isCanNotFeedToPetItem()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)
		return	TRUE;
	
	if	(getSpecialItem() && getSpecialItem()->m_bf1IsFreeUseItem)
		return	TRUE;

	return	lpItem->m_bf1IsBlockToFeedPet;
}

BOOL
cItem::isCanNotDestroyItem()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)
		return	TRUE;

	if	(isShopSpecialItem())
		return	FALSE;

	cBookPageItemDefine	*lpPageItem	=	getAddPageToBookItem();
	cBookItemDefine		*lpBookItem	=	getBook();

	if	(lpPageItem)
	{
		for	(int	i=lpPageItem->m_wBeginPage;i<=lpPageItem->m_wEndPage;i++)
		{
			cBookPageInfoDefine	*lpPage		=	g_book.getPage(lpPageItem->m_wBookIndex,lpPageItem->m_wCategory,i);

			if	(!lpPage)
				return	TRUE;

			if	(lpPage->m_bf1IsBlockDestroyPage)
				return	TRUE;
		}

		return	FALSE;
	}

	if	(lpBookItem)
	{
		cBookInfoDefine	*lpBook			=	g_book.getBook(lpBookItem->m_wBookIndex);

		if	(!lpBook)
			return	TRUE;

		return	lpBook->m_bf1IsBlockDestroy;
	}

	return	lpItem->m_attr.isCanNotDestroyItem;
}


BOOL
cItem::isCanNotStoreBankItem()
{
	cBasicItem		*lpBasicItem		=	getBasicItem();

	if	(!lpBasicItem	||	lpBasicItem->m_attr.isCanNotStoreBank)
		return	TRUE;

	if	(isBreedingRecordBook())
		return	FALSE;

	if	(isShopSpecialItem())
		return	TRUE;

	if	(getSpecialItem() && getSpecialItem()->m_bf1IsFreeUseItem)
		return	TRUE;

	cBookPageItemDefine	*lpPageItem		=	getAddPageToBookItem();
	cBookItemDefine		*lpBookItem		=	getBook();

	if	(lpPageItem)
	{
		for	(int	i=lpPageItem->m_wBeginPage;i<=lpPageItem->m_wEndPage;i++)
		{
			cBookPageInfoDefine	*lpPage		=	g_book.getPage(lpPageItem->m_wBookIndex,lpPageItem->m_wCategory,i);

			if	(!lpPage)
				return	TRUE;

			if	(lpPage->m_bf1IsBlockStoreBank)
				return	TRUE;
		}

		return	FALSE;
	}

	if	(lpBookItem)
	{
		cBookInfoDefine	*lpBook			=	g_book.getBook(lpBookItem->m_wBookIndex);

		if	(!lpBook)
			return	TRUE;

		return	lpBook->m_bf1IsBlockStoreBank;
	}

	return	FALSE;
}

BOOL
cItem::isShopSpecialItem()
{
	if	(isPresentCoupleRing())
		return	FALSE;
	if	(isBreedingRecordBook())
		return	FALSE;

	if	(checkFirstEffect(eIE_MINIPET_POUCH))
		return FALSE;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if	(m_aPrefix[i].m_wPrefix	==	0xffff)
			break;

		if	(g_aItemPrefix[m_aPrefix[i].m_wPrefix].m_wType	==	eIP_IS_SHOP_SPECIAL_ITEM)
			return	TRUE;
	}

	return	FALSE;
}


BOOL
cItem::isCanNotTradeItem()
{
	cBasicItem		*lpItem			=	getBasicItem();

	if	(!lpItem)
		return	TRUE;

	if	(lpItem->m_attr.isCanNotTradeItem)
		return	TRUE;

	if	(isShopSpecialItem()	||	isPrincessWeapon())
		return	TRUE;

	cBookPageItemDefine	*lpPageItem		=	getAddPageToBookItem();
	cBookItemDefine		*lpBookItem		=	getBook();
	cSpecialItem		*lpSpecialItem	=	getSpecialItem();

	if	(lpPageItem)
	{
		for	(int	i=lpPageItem->m_wBeginPage;i<=lpPageItem->m_wEndPage;i++)
		{
			cBookPageInfoDefine	*lpPage		=	g_book.getPage(lpPageItem->m_wBookIndex,lpPageItem->m_wCategory,i);

			if	(!lpPage)
				return	TRUE;

			if	(lpPage->m_bf1IsBlockTradePage)
				return	TRUE;
		}

		return	FALSE;
	}

	if	(lpBookItem)
	{
		cBookInfoDefine	*lpBook			=	g_book.getBook(lpBookItem->m_wBookIndex);

		if	(!lpBook)
			return	TRUE;

		return	lpBook->m_bf1IsBlockTrade;
	}

	if	(!lpSpecialItem)
		return	FALSE;

	return	lpSpecialItem->m_bf1IsReversionItem;
}

BOOL
cItem::isReversionItemWhenEauip()
{
	cSpecialItem	*lpSpecialItem	=	getSpecialItem();

	if	(!lpSpecialItem)
		return	FALSE;

	return	lpSpecialItem->m_bf1IsReversionItemWhenEquip;
}

BOOL
cItem::isItemPack()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if (!lpItem)	
		return	FALSE;

	if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_LOTTERY_ITEM)
		return	TRUE;
	if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_SELECT_ITEM_IN_ITEM_PACK)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::isDXItem()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if	(isExceptionItem())
		return	FALSE;

	if	(lpItem->m_attr.isDXItem == TRUE)
		return	TRUE;

	if	(lpItem->m_bf1IsUltimate)
		return	FALSE;

	BOOL	bIsDXItem	=	FALSE;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)
			break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if	(lpPrefix->m_wIsUltimatePrefix)
			return	FALSE;

		if	(lpPrefix->m_wIsDXPrefix)
			bIsDXItem	=	TRUE;
	}

	return	bIsDXItem;
}

BOOL
cItem::isUltimateItem()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if	(isExceptionItem())
		return	FALSE;

	if	(lpItem->m_bf1IsUltimate	== TRUE)
		return	TRUE;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)
			break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if (lpPrefix->m_wIsUltimatePrefix)
			return	TRUE;
	}

	return	FALSE;
}

BOOL
cItem::isExtraItem()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if	(isExceptionItem())
		return	FALSE;

	if	(lpItem->m_attr.isExtraItem == TRUE)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::makeToSpecialItem()
{
	cSpecialItem	*lpNewItem	=	(cSpecialItem*)this;

	lpNewItem->reset();

	return	TRUE;
}

BOOL
cItem::isSpecialItem()
{
	if	(isExceptionItem())
		return	FALSE;

	return	m_bf1IsSpecialItem;
}

BOOL
cItem::isReversionItem()
{
	if	(isExceptionItem())
		return	FALSE;

	if	(!m_bf1IsSpecialItem)
		return	FALSE;

	cSpecialItemDefine	*lpInfo	=	(cSpecialItemDefine	*)this;

	return	lpInfo->m_bf1IsReversionItem;
}

void
cItem::operateReversionItemWhenEquip()
{
	if	(isExceptionItem())
		return;

	if	(m_bf1IsSpecialItem	==	FALSE)
		return;

	cSpecialItemDefine	*lpInfo	=	(cSpecialItemDefine	*)this;

	if (lpInfo->m_bf1IsReversionItemWhenEquip)
		lpInfo->m_bf1IsReversionItem	=	TRUE;
}

int
cItem::getUnlockDoorLevel()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)
		return	FALSE;

	if	(lpItem->m_wKind	!=	eIK_KEY)
		return	0;

	int	iEffect	=	lpItem->m_aGenerateData[0].m_wEffect;

	if	(iEffect	==	eIE_OPEN_DOOR	||	iEffect	==	eIE_OPEN_ARCA_AND_DOOR)
		return	m_aOption[0];

	return	0;
}

int
cItem::getUnlockArcaLevel()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if (!lpItem)
		return	FALSE;

	if (lpItem->m_wKind	!=	eIK_KEY)
		return	0;

	int	iEffect	=	lpItem->m_aGenerateData[0].m_wEffect;

	if (iEffect	==	eIE_OPEN_ARCA	||	iEffect	==	eIE_OPEN_ARCA_AND_DOOR)
		return	m_aOption[0];

	return	0;
}


BOOL
cItem::isMagicCarpetEmblem()
{
	cBasicItem	*lpBaseItem	=	getBasicItem();

	if (!lpBaseItem)
		return	FALSE;

	if (lpBaseItem->m_attr.isMagicCarpetEmblem)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::isMergeEmblemItem()
{
	cBasicItem	*lpBaseItem	=	getBasicItem();

	if (!lpBaseItem)
		return	FALSE;

	if (lpBaseItem->m_attr.isMergeToCarpetEmblem)
		return	TRUE;

	return	FALSE;
}

int
cItem::getCheckSum()	//	¼¿½¿½ ¿½¿½¿½±¿½
{
	if	(m_wBaseItem	==	0xffff)
		return	0;

	if	(isUnknownItem())
		return	-1;

	int	iSum=	0;

	iSum	+=	m_dwSerial;
	iSum	+=	m_wBaseItem;							//	¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½

	if	(isExtraItem()	==	FALSE)
	{
		iSum	+=	m_bCount;								//	¿½¿½¿½¿½
		iSum	+=	m_bDurability;							//	¿½¿½¿½¿½¿½¿½
	}

	iSum	+=	m_aOption[0];							//	¿½¿½¿½¿½¿½¶¿½ ¿½¿½¿½¿½ ¿½¼¿½
	iSum	+=	m_aOption[1];							//	¿½¿½¿½¿½¿½¶¿½ ¿½¿½¿½¿½ ¿½¼¿½

	for (int iPrefix=0;iPrefix<dITEM_PREFIX_COUNT;iPrefix++)
	{
		iSum	+=	m_aPrefix[iPrefix].m_wPrefix;
		iSum	+=	m_aPrefix[iPrefix].m_aValue[0];
		iSum	+=	m_aPrefix[iPrefix].m_aValue[1];
	}

	iSum	+=	m_isBroken			;	//	¿½½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½
	iSum	+=	m_bf1IsSpecialItem	;	//	¿½¿½¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½
	iSum	+=	m_wayPointIndex		;	//	¿½¿½¿½¿½ ¿½¿½¿½¿½Æ® ¿½¿½¿½¿½

	iSum	+=	m_isCanNotAttachPrefixItem;	//	¿½¿½ ¿½¿½ ¿½¿½¿½¸ ¿½¿½¿½¿½ ¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½
	iSum	+=	m_year			;	//	¿½¿½¿¿½°
	iSum	+=	m_month			;
	iSum	+=	m_day			;
	iSum	+=	m_hour			;
	iSum	+=	m_minute		;

	return	iSum;
}

BOOL
cItem::isIncorrectJobItem()
{
#ifdef	_FOR_KOREA
	return	FALSE;
#endif
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if	(isExceptionItem())
		return	FALSE;

	for (int i=0;i<dPLAYER_JOB_COUNT;i++)
	{
		if	(g_aPlayer1LvSetting[i].m_wStatus)
			if	(lpItem->isEnableJob(i))
				return	FALSE;
	}

	return	TRUE;
}

BOOL
cItem::isUnknownItem()
{
	cSpecialItem	*lpSpecialItem	=	getSpecialItem();

	if (!lpSpecialItem)
		return	FALSE;

	return	lpSpecialItem->m_bf1IsUnknown;
}

BOOL
cItem::isItemPiece()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if (!lpBasicItem)
		return	FALSE;

	if (lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_ITEM_PIECE)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::generatePieceItem(cItem *_lpItem)
{
	cPieceItem*	lpPieceItem	=	getPieceItem();

	if (!lpPieceItem)
		return	FALSE;

	return	lpPieceItem->generateItem(_lpItem);
}

cPieceItem*
cItem::getPieceItem()
{
	cSpecialItem	*lpSpecialItem	=	getSpecialItem();

	if	(!lpSpecialItem)
		return	NULL;

	return	g_pieceItemManager.get(lpSpecialItem->m_bf11SValue);
}

cPrincessWeaponDefine*
cItem::getPrincessWeapon()
{
	if	(!isPrincessWeapon())
		return	NULL;

	return	(cPrincessWeaponDefine*)this;
}

cItemSet*
cItem::getItemSet()
{
	cSpecialItem	*lpSpecialItem	=	getSpecialItem();

	if	(!lpSpecialItem || lpSpecialItem->m_bf1IsSetItem == FALSE)
		return	NULL;

	return	g_itemSetManager.get(lpSpecialItem->m_bf11SValue);
}

cSetItem*
cItem::getSetItem()
{
	cSpecialItem	*lpSpecialItem	=	getSpecialItem();

	if	(!lpSpecialItem || lpSpecialItem->m_bf1IsSetItem == FALSE)
		return	NULL;

	cItemSet		*lpItemSet		=	g_itemSetManager.get(lpSpecialItem->m_bf11SValue);

	if	(!lpItemSet)
		return	NULL;

	return	lpItemSet->getItemByBaseItem(m_wBaseItem);
}

BOOL
cItem::isEnableJob(int _iJob)
{
	cSetItem		*lpSetItem		=	getSetItem();

	if	(!lpSetItem	||	lpSetItem->m_uiEnableJobMask == 0)
	{
		cBasicItem		*lpBasicItem	=	getBasicItem(TRUE);

		return	lpBasicItem->isEnableJob(_iJob);
	}

	return	lpSetItem->isEnableJob(_iJob);
}

int
cItem::getFullRepairPrice()
{
	if (isExceptionItem())
		return	0;

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)	
		return	0;

	if	(isBreakdownAbleItem()	==	FALSE)
		return	0;

	int	iDamagedPoint	=	lpBasicItem->getDurability()	-	m_bDurability;

	if	(iDamagedPoint	<=	0)
		return	0;

	LONGLONG	llPrice	=	getPrice(m_bCount);

	if	(isDXItem())
		llPrice	=	llPrice*iDamagedPoint/lpBasicItem->getDurability()/dDX_ITEM_REPAIR_FACTOR;
	else
		llPrice	=	llPrice*iDamagedPoint/lpBasicItem->getDurability()/dULTIMATE_ITEM_REPAIR_FACTOR;

	int	iPrice	=	min((int)llPrice,0x7fffffff);

	return	max(iPrice,1);
}

BOOL
cItem::updateExtraItemEfficency()
{
	cExtraItemDefine	*lpItem	=	(cExtraItemDefine	*)this;

	return	lpItem->updateCurrentEfficency();
}

//
//	¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½
int
cITEM_PREFIX_MANAGER::getPrefixByDropLevel(int _iDropLevel,int _iGroup,int _iItemType,BOOL _bIsSpecialDropChance)
{
	int i,iPrefix,iCount=0,aPrefix[dMAX_ITEM_PREFIX_COUNT];

	if (_iItemType	==	eIK_MAGIC_JEWEL)
		_iItemType	=	eIK_JEWEL;

	for (i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[i];

		if (lpPrefix->m_wSerial	==	0xffff)
			continue;

		if (lpPrefix->m_wType	==	eIP_CHARGE_SKILL	)
			continue;
		if (lpPrefix->m_wType	==	eIP_INCREASE_UNIQUE_ITEM_DROP_CHANCE)
			continue;
		if (lpPrefix->m_wDiscernmentCode	>=	10000	)
			continue;

		if	(lpPrefix->m_wDropLevel	<=	_iDropLevel)
			if	(lpPrefix->m_aEnableSet[_iGroup])
				if	(lpPrefix->isStickAbleItem(_iItemType))
					aPrefix[iCount++]	=	i;
	}

	if	(iCount	==	0)
		return	-1;

	for (i=0;i<10;i++)
	{
		int	iIndex		=	random(iCount);
		iPrefix			=	aPrefix[iIndex];

		int	iDropChance	=	g_aItemPrefix[iPrefix].m_wCorrectDropChance;

		if	(random(100) < iDropChance)
			break;
		else
		{
			iPrefix	=	0xffff;

			if	(iCount	>=	2)
				aPrefix[iIndex]	=	aPrefix[iCount-1];

			iCount--;

			if (iCount	<=	0)
				break;
		}
	}

	if	(iPrefix	!=	0xffff)
	{
		if	(g_aItemPrefix[iPrefix].m_wIsDXPrefix)
		{
			if	(random(3) != 0)
				iPrefix	=	0xffff;
		}
//	¿½¿½ ¿½¿½¿½¿½ ¿½¿½Æ¼¿½¿½Æ® ¿½¿½¿½¿½ ¿½¿½ ¿½¿½¿½¿½¿½¿½ 1/9¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿? ®¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½´¿½. ¿½¿½¿½¼­ ¿½¿½¿½¿½¿½¸¿½ ¿½Ø´¿½ ¿½¿½Æ¼¿½¿½Æ® ¿½¿½¿½¸ ¿½¿½¿½¿½¿½¿½ ¿½¸¿½ ¿½¿½¿½¸¸¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½´¿?.																																																										
		if	(g_aItemPrefix[iPrefix].m_wIsUltimatePrefix)
		{
#ifdef	dBLOCKED_TO_DROP_ULTIMATE_ITEM
			return	0xffff;
#endif

#ifdef	dIS_DROP_ULTIMATE_PREFIX_ONLY_TEST_SERVER
			if	(!g_bIsTestServer)
				return	0xffff;
#endif
			if	(random(9) != 0)
				iPrefix	=	0xffff;
		}
	}

	return	iPrefix;
}

//
//	¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½
int
cITEM_PREFIX_MANAGER::getLittleGoodPrefix(int _iDropLevel,int _iGroup,int _iItemType)
{
	int i,iPrefix,iCount=0,aPrefix[dMAX_ITEM_PREFIX_COUNT];

	if (_iItemType	==	eIK_MAGIC_JEWEL)	_iItemType	=	eIK_JEWEL;

	for (i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[i];

		if (lpPrefix->m_wSerial			==	0xffff)		continue;
		if (lpPrefix->m_wDiscernmentCode<	10000)		continue;

		if (lpPrefix->m_wType	==	eIP_CHARGE_SKILL)	continue;

		if (lpPrefix->m_wDropLevel	<=	_iDropLevel)
			if (lpPrefix->m_aEnableSet[_iGroup])
				if (lpPrefix->isStickAbleItem(_iItemType))
					aPrefix[iCount++]	=	i;
	}

	if (iCount	==	0)	return	-1;

	iPrefix	=	random(iCount);
	iPrefix	=	aPrefix[iPrefix];

	return	iPrefix;
}

//
//	¿½º¿½ ¿½¿½¿? ¿½¿½¿½¿½ ¿½¿½
int
cITEM_PREFIX_MANAGER::getPrefixByDiscernmentCode(int _iDiscernmentCode)
{
	int	i;

	for (i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[i];

		if	(lpPrefix->m_wDiscernmentCode==	_iDiscernmentCode)
			return	i;
	}

	return	0xffff;
}

BOOL
cITEM_PREFIX_MANAGER::revisePrefixInfo(CItemPrefixInfo *_lpPrefixInfo,int _iDiscernmentCode)
{
	cITEM_PREFIX	*lpCurrentPrefix	=	&g_aItemPrefix[_lpPrefixInfo->m_wPrefix];

	int	iMXVI	=	1;
	int	iMNVI	=	0;

	if	(lpCurrentPrefix->m_aValue[iMXVI][0] == 0)
		iMXVI	=	0;

//	¿½¿½¡¿½¿½ ¿½¿½¿½¿½ ¿½¿½¡¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½.
	if	(_lpPrefixInfo->m_aValue[iMXVI] < lpCurrentPrefix->m_aValue[iMXVI][0] || _lpPrefixInfo->m_aValue[iMXVI] > lpCurrentPrefix->m_aValue[iMXVI][1])
	{
		int		i;
		BOOL	bIsFinded	=	FALSE;
		int		iLowValue=0x7fffffff,iLowValuePrefix=-1,iHighValuePrefix=-1,iHighValue=0;

		for (i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
		{
			cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[i];

			if	(lpPrefix->m_wDiscernmentCode	>	_iDiscernmentCode)
				continue;

			if	(lpCurrentPrefix->m_wType	==	lpPrefix->m_wType)
			{
				if	(lpPrefix->m_aValue[iMXVI][0]	<=	iLowValue)
				{
					iLowValue		=	lpPrefix->m_aValue[iMXVI][0];
					iLowValuePrefix	=	i;
				}
				if	(lpPrefix->m_aValue[iMXVI][1]	>=	iHighValue)
				{
					iHighValue		=	lpPrefix->m_aValue[iMXVI][1];
					iHighValuePrefix=	i;
				}
				if	(_lpPrefixInfo->m_aValue[iMXVI] >= lpPrefix->m_aValue[iMXVI][0] && _lpPrefixInfo->m_aValue[iMXVI] <= lpPrefix->m_aValue[iMXVI][1])
				{
					bIsFinded					=	TRUE;
					_lpPrefixInfo->m_wPrefix	=	i;

					break;
				}
			}
		}

		if	(bIsFinded	==	FALSE)
		{
			if	(_lpPrefixInfo->m_aValue[iMXVI]	<	iLowValue)
			{
				_lpPrefixInfo->m_wPrefix		=	iLowValuePrefix;
				_lpPrefixInfo->m_aValue[iMXVI]	=	iLowValue;
			}
			else
			if	(_lpPrefixInfo->m_aValue[iMXVI]	>	iHighValue)
			{
				_lpPrefixInfo->m_wPrefix		=	iHighValuePrefix;
				_lpPrefixInfo->m_aValue[iMXVI]	=	iHighValue;
			}
		}
	}

	if	(iMXVI)
	{
		if	(_lpPrefixInfo->m_aValue[iMNVI] <	lpCurrentPrefix->m_aValue[iMNVI][0])
			_lpPrefixInfo->m_aValue[iMNVI]	=	(BYTE)lpCurrentPrefix->m_aValue[iMNVI][0];

		if	(_lpPrefixInfo->m_aValue[iMNVI]	>	lpCurrentPrefix->m_aValue[iMNVI][1])
			_lpPrefixInfo->m_aValue[iMNVI]	=	(BYTE)lpCurrentPrefix->m_aValue[iMNVI][1];
	}

	return	TRUE;
}

int
cITEM_PREFIX_MANAGER::getPrefixByType(int _iType,int _iLevel)
{
	int	iCount	=	0,i;

	for (i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[i];

		if	(lpPrefix->m_wDiscernmentCode	>=	10000)
			continue;

		if	(lpPrefix->m_wType	==	_iType)
		{
			if	(_iLevel	==	iCount)
				return	i;

			iCount++;
		}
	}

	return	0xffff;
}

int
cITEM_PREFIX_MANAGER::getPrefixLevel(int _iPrefix)
{
	int	iCount	=	0,i;
	int	iType	=	g_aItemPrefix[_iPrefix].m_wType;

	for (i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[i];

		if	(lpPrefix->m_wDiscernmentCode	>=	10000)
			continue;

		if	(_iPrefix			==	i		)
			return	iCount;
		if	(lpPrefix->m_wType	==	iType	)
			iCount++;
	}

	return	0xffff;
}

int
cITEM_PREFIX_MANAGER::getPrefixCountByType(int _iType)
{
	int	iCount	=	0,i;

	for (i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[i];

		if	(lpPrefix->m_wDiscernmentCode	>=	10000)
			continue;

		if	(lpPrefix->m_wType	==	_iType)
			iCount++;
	}

	return	iCount;
}

BOOL
cITEM_PREFIX_MANAGER::levelUpPrefix(CItemPrefixInfo *_lpPrefixInfo,int _iItemKind)
{
	int	iPrefix	=	_lpPrefixInfo->m_wPrefix;

	if	(g_aItemPrefix[iPrefix].m_wDiscernmentCode	>=	10000)
		return	FALSE;

	int	iType	=	g_aItemPrefix[iPrefix].m_wType;
	int	iLevel	=	getPrefixLevel(iPrefix);

	if	(iLevel	==	0xffff)
		return	FALSE;

	iLevel++;
	int	iNextPrefix	=	getPrefixByType(iType,iLevel);

	if	(iNextPrefix	==	0xffff)
		return	FALSE;

	cITEM_PREFIX	*lpOldPrefix	=	&g_aItemPrefix[iPrefix];
	cITEM_PREFIX	*lpNextPrefix	=	&g_aItemPrefix[iNextPrefix];

	if	(!lpNextPrefix->isStickAbleItem(_iItemKind))
		return	FALSE;


	{
		int	iOldKind	=	0;
		int	iCurrentKind=	0;

		if	(lpOldPrefix->m_wIsDXPrefix)
			iOldKind	=	1;
		if	(lpOldPrefix->m_wIsUltimatePrefix)
			iOldKind	=	2;
		if	(lpNextPrefix->m_wIsDXPrefix)
			iCurrentKind=	1;
		if	(lpNextPrefix->m_wIsUltimatePrefix)
			iCurrentKind=	2;

		if	(iOldKind	!=	iCurrentKind)
			return	FALSE;
	}


	_lpPrefixInfo->m_wPrefix	=	iNextPrefix;

	int	aiOldValue[2];

	aiOldValue[0]	=	_lpPrefixInfo->m_aValue[0];
	aiOldValue[1]	=	_lpPrefixInfo->m_aValue[1];

	for (int j=0;j<2;j++)
	{
		int	iMinValue,iMaxValue,iRange;
		
		iMinValue	=	lpNextPrefix->m_aValue[j][0];
		iMaxValue	=	lpNextPrefix->m_aValue[j][1];
		iMaxValue	=	max(iMaxValue,iMinValue);
		iRange		=	iMaxValue-iMinValue+1;

		_lpPrefixInfo->m_aValue[j]	=	min(random(iRange)+iMinValue,iMaxValue);

		if	(lpOldPrefix->m_aValue[j][0]	<	iMinValue)
		{
			if	(_lpPrefixInfo->m_aValue[j]	<	aiOldValue[j])
				_lpPrefixInfo->m_aValue[j]	=	aiOldValue[j]+1;
		}
		else
		if	(lpOldPrefix->m_aValue[j][0]	>	iMinValue)
		{
			if	(_lpPrefixInfo->m_aValue[j]	>	aiOldValue[j])
				_lpPrefixInfo->m_aValue[j]	=	aiOldValue[j]-1;
		}
	}

	return	TRUE;
}

BOOL
CDroppedItem::isCatchableTreasure(int _iSerial,int _iParty)
{
	if (m_wDropMethod	!=	eDIM_MONSTER_DROP	)
		return	FALSE;

	if (m_wSealedTime	==	0		)
		return	TRUE;

	if (m_wOwner		==	0xffff	)
		return	TRUE;

	if (m_wOwner		>=	dPARTY_FLAG)
	{
		if (m_wOwner-dPARTY_FLAG==	_iParty)
			return	TRUE;

		return	FALSE;
	}

	if (m_wOwner	==	_iSerial)
		return	TRUE;

	return	FALSE;
}

//
//	¿½¿½¿½°¿½ ¿½¿½¿½¿½¿½¿½. ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½.
void
CDroppedItemManager::outActor(int _iActorSerial,int _iPartySerial,BOOL _isPartyLeave,cFIELD *_lpField)
{
	int	iDroppedItemCount	=	getCount();

	for (int i=0,iCount=0;i<dMAX_DROPPED_ITEM && iCount < iDroppedItemCount;i++)
	{
		CDroppedItem	*lpDroppedItem	=	get(i);

		if (!lpDroppedItem)
			continue;

		iCount++;

		int	iOwner	=	lpDroppedItem->m_wOwner;

		if (iOwner	>=	dPARTY_FLAG)
		{
			iOwner	-=	dPARTY_FLAG;

			if	(iOwner	==	_iPartySerial)
				if	(_isPartyLeave)
					lpDroppedItem->m_wSealedTime	=	0;
		}
		else
		{
			if (lpDroppedItem->m_wOwner	==	_iActorSerial	)
			{
				lpDroppedItem->m_wSealedTime	=	0;

				if (lpDroppedItem->m_wDropMethod	==	eDIM_DUMMY)
				{
					SG_REMOVE_DROPPED_ITEM	packet;

					packet.base.set(sizeof(packet),dSG_REMOVE_DROPPED_ITEM);
					packet.wRemoveItem	=	i;

					_lpField->addSendPacket((ALL_MSG*)&packet,0xffff);

					remove(i);
				}
			}
		}
	}
}


//
//	¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½?
void
CDroppedItemManager::removeHideWeapon(cACTOR *_lpActor,cFIELD *_lpField,WORD _wBaseItem,DWORD _dwSerial,int _iPart)
{
	int	iDroppedItemCount	=	getCount();

	for (int i=0,iCount=0;i<dMAX_DROPPED_ITEM && iCount < iDroppedItemCount;i++)
	{
		CDroppedItem	*lpDroppedItem	=	get(i);

		if (!lpDroppedItem)
			continue;
		if (lpDroppedItem->m_wDropMethod	!=	eDIM_DUMMY					)
			continue;
		if (lpDroppedItem->m_wOwner			!=	_lpActor->m_wSerialInField	)
			continue;

		iCount++;

		if (lpDroppedItem->m_item.m_wBaseItem == _wBaseItem && lpDroppedItem->m_item.m_dwSerial	== _dwSerial)
		{
			{
				SG_HIDE_EQUIPMENT	packet;

				packet.base.set(sizeof(SG_HIDE_EQUIPMENT),dSG_HIDE_EQUIPMENT);
				packet.wPart	=	_iPart;
				packet.wMethod	=	1;

				g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
			}

			{
				SG_REMOVE_DROPPED_ITEM	packet;

				packet.base.set(sizeof(packet),dSG_REMOVE_DROPPED_ITEM);
				packet.wRemoveItem	=	i;

				_lpField->addSendPacket((ALL_MSG*)&packet,0xffff);
			}

			remove(i);
		}
	}
}

CDroppedItem*
CDroppedItemManager::getNeareastItem(cACTOR *_lpActor,int _iRange,int _iValue,BOOL _bIsByMiniPet,BOOL _bIsItemCollector)
{
	int	iDroppedItemCount	=	getCount();
	int	iMinRange			=	0x7fffffff;
	int	iReturnItem			=	0xffff;

	_iRange	*=	_iRange;

	cACTOR	*lpTamer		=	_lpActor->getTamer();

	if	(_bIsByMiniPet)
		lpTamer	=	_lpActor;

	if	(!lpTamer)
		return	NULL;

	for (int i=0,iCount=0;i<dMAX_DROPPED_ITEM && iCount < iDroppedItemCount;i++)
	{
		CDroppedItem	*lpDroppedItem	=	get(i);

		if	(!lpDroppedItem)
			continue;
		if	(!lpDroppedItem->isCatchableTreasure(lpTamer->m_wSerialInField,lpTamer->getPartySerial()))	//	¿½°¿½¿½¿½ ¿½Æ´¿½¿½¾¿½!!
			continue;

		iCount++;

		int	iCurrentRange	=	GetOvalRange(lpDroppedItem->m_pos.x,lpDroppedItem->m_pos.y,_lpActor->m_pos.x,_lpActor->m_pos.y);

		if	(iCurrentRange	>=	_iRange)
			continue;

		if	(iCurrentRange	>	iMinRange)
			continue;

		cItem	*lpItem	=	&lpDroppedItem->m_item;

		if	(lpItem->m_wBaseItem	!= dITEM_MONEY)
		{
			if	(_bIsByMiniPet	&&	_bIsItemCollector	==	FALSE)
				continue;

			cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

			if	(!lpBasicItem)
				continue;

			if(lpBasicItem->m_bf1IsNotGetUsedSkill)		// ¿½¿½³¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½±¿½ ¿½°¿½¿½¸¿½..
				continue;

//1~10Lv:¿½¿½, 11~20Lv:¿½¸¿½°,21~35Lv:¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½¿?,35~50Lv:¿½¸¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½,51Lv~:¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½.
			
			BOOL	bIsExhaustItem	=	1-lpBasicItem->m_attr.isCanNotUseableItem;

			if	(_bIsByMiniPet)
			{
				if	(lpItem->isSuperUniqueItem()	&&	_iValue	<	5)
					continue;
				if	(lpItem->isUniqueItem()	&&	_iValue	<	4)
					continue;
				// if	(lpItem->isRareItem()	&&	_iValue	<	3) // 20240906 rekunn minipet test
				// 	continue;
				// if	(bIsExhaustItem == FALSE &&	_iValue	<	2)	//	¿½¹¿½ ¿½¿½¿½¿½¿½¿½
				// 	continue;
//	1Lv:¿½¸¿½°,2Lv:¿½¿½¿½¿½¿½¿½¿½¿½¿?,3Lv:¿½¿½¿½¿½¿½¿½¿½¿½¿?,4Lv:¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½,5Lv:¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½. ¿½¿½ ¿½¿½¿? ¿½½¿½¿½¿½ ¿½¿½¿½¿½¿½¸¿½ ¿½Ø´¿½
			}
			else
			{
				if	(lpItem->isNormalItem() || lpItem->isLittleGoodItem())	//	¿½¹¿½ ¿½¿½¿½¿½¿½¿½
					continue;

				if	(lpItem->isSuperUniqueItem()	&&	_iValue	<	51)
					continue;

				if	(lpItem->isUniqueItem()	&&	_iValue	<	35)
					continue;

				if	(lpItem->isRareItem()	&&	_iValue	<	21)	//	36 ¿½¿½ ¿½¿½¿½¿½
					continue;

				if	(_iValue	<	11)	//	11 ¿½¿½ ¿½º¿½ ¿½¿½¿½¿½¿½¿½
					continue;
			}
		}
		else
		{
			if	(_bIsByMiniPet	&&	_bIsItemCollector)
				continue;

			if	(_bIsByMiniPet)
			{
				int	aiGoldLimit[]	=	{0,10,20,40,80,100000000};

				_iValue	=	min(_iValue,5);

				if	((int)lpItem->m_dwSerial	>	aiGoldLimit[_iValue])
					continue;
			}
		}

		if	(_lpActor->m_lpField->isMissileBlockedLine(_lpActor->m_pos.x,_lpActor->m_pos.y,lpDroppedItem->m_pos.x,lpDroppedItem->m_pos.y))
			continue;

		iMinRange	=	iCurrentRange;
		iReturnItem	=	i;
	}

	if (iReturnItem == 0xffff)
		return	NULL;

	return	get(iReturnItem);
}

void
cItem::changeToCoupleRing(char *_lpstrTargetName)
{
	m_wBaseItem	=	g_im.m_wCoupleRing;

	if	(_lpstrTargetName)
		getCoupleRing()->setCoupleName(_lpstrTargetName);

	cBasicItem	*lpBasicItem	=	getBasicItem();

	for (int i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if	(lpBasicItem->m_aGenerateData[i].m_wEffect	!=	eIE_INCREASE_SELECT_POWER)
			continue;

		int	iReferenceIndex,iRange,iMinValue,iMaxValue;

//	¿½¼¿½ 1
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue+1;

			m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange),iMaxValue);
		}

//	¿½¼¿½ 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue+1;

			m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange),iMaxValue);
		}
	}
}

BOOL
cItem::isExpiredCoupleRing()
{

	cBasicItem	*lpItem	=	getBasicItem();
	
	if	(!lpItem)
		return	FALSE;
	
	if	(lpItem->m_aGenerateData[0].m_wEffect	!=	eIE_COUPLE_RING &&
		lpItem->m_aGenerateData[0].m_wEffect	!=	eIE_PRESENT_COUPLE_RING	&&
		lpItem->m_aGenerateData[0].m_wEffect	!=	eIE_NOT_DEFINE_COUPLE_COUPLE_RING	)
		return FALSE;
	
	if(((cCoupleRingDefine*)this)->m_year == 0)
		return FALSE;
	
		
	int			iYear			=	g_currentTime.m_wYear-2000;
	int			iMonth			=	g_currentTime.m_wMonth;
	int			iDay			=	g_currentTime.m_wDay;
	int			iHour			=	g_currentTime.m_wHour;
	int			iMinute			=	g_currentTime.m_wMinute;

	double		dCurrentTimeValue	=	iYear*1000000						+iMonth*10000						+iDay*100						+iHour + iMinute/100.0;
	int		iExpireTimeValue	=	(((cCoupleRingDefine*)this)->m_year)*1000000  +((cCoupleRingDefine*)this)->m_month*10000  +((cCoupleRingDefine*)this)->m_day*100;
	
	if(iExpireTimeValue<dCurrentTimeValue)
		return TRUE;

	return FALSE;
}


//	¿½¿½¿? ¿½¿½ ¿½¿½¿½¿½ ¿½¶¿½ ¿½¿½¿½¿½¡
int
cItem::getExpForGuildPet()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	0;

	if(c_aItemEquipPlace[lpBasicItem->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpBasicItem->m_wKind] == 0xffff)		//  ¿½¿½¿½¿½..
		return	0;

	if	(isUniqueItem())
		return	0;

	if	(getSetItem())
		return	0;

	if	(isPremiumItem())
		return	0;

	if	(isExtraItem())
		return	0;

	if	(m_bCount	==	0)
		return	0;

	int	iLevel		=	getRequireLevel();
	int	iStackLimit	=	getStackLimit();
	int	iExp		=	iLevel/10;

	if	(iStackLimit	==	0)
		return	0;
	
	if	(iStackLimit	>	1)
		iExp	=	(m_bCount*100/iStackLimit*iLevel/100)/10;

	return	iExp;
//5. ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¸¿½ ¿½Ø´¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ "¿½¿½¿½°¡¿½¿½ ¿½¿½¿½¿½/10" ¿½¿½­¿½¿½ ¿½¿½¿½¿½¡¿½¿½ ¿½¿½´¿?.					
//   ¿½¿½. ¿½¿½¿½¿½ ¿½¿½ ¿½´¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ '¿½¿½¿?'¿½¿½ ¿½¿½¿½¿½¿½´¿½.(¿½¿½, ¿½¿½Æ® ¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½°¿½)					
// ¿½¿½. ¿½¿½¿½´¿½ ¿½°¿½, ­¿½¿½, ¿½¿½¿½¿½º ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¡¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ "((¿½¿½¿½¿½ ¿½¿½¿½¿½/¿½´¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½)*¿½¿½¿½¿½ ¿½¿½¿½·¿½¿½¿½)/10"¿½¿½¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½´¿?.					
//   ¿½¿½. Gold ¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿? ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½×´¿½¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½Æ´¿½ ¿½¿½¿½´¿½.					
//        Exp = Gold/1000					
}

BOOL
cItem::isMassivePolisher()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_EXTEND_ALL_MINIPET_TIME)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::isPolisher()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_EXTEND_MINIPET_TIME)
		return	TRUE;

	return	FALSE;
}

//JBC	¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½ ®¿½¿½	09-01-02
BOOL
cItem::isMiniPetAwakenItem50()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();
	
	if	(!lpBasicItem)
		return	FALSE;
	
	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_MINIPET_AWAKEN50)
		return	TRUE;
	
	return	FALSE;
}


//JBC	¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½ ®¿½¿½	09-01-02
BOOL
cItem::isMiniPetAwakenItem100()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();
	
	if	(!lpBasicItem)
		return	FALSE;
	
	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_MINIPET_AWAKEN100)
		return	TRUE;
	
	return	FALSE;
}

BOOL
cItem::isMiniPetFeed()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_FLAKE_OF_DOING_ONE)
		return	TRUE;
	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_FEATHER_OF_THINKING_ONE)
		return	TRUE;
	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_TOY_OF_SIN_AND_RIN)
		return	TRUE;

	return	FALSE;
}


int
cItem::getMiniPetFeedType()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	-1;

	int	iFeedType	=	-1;
	int	iKind		=	lpBasicItem->m_wKind;

	if(isExistSpecificEffect(eIE_MINIPET_FEED))
	{
		if(getEffectMiniPetFeedType())
			iFeedType	=	getEffectMiniPetFeedType();
	}
	else if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_FLAKE_OF_DOING_ONE)
		iFeedType	=	nsMiniPetStyle::Bio;
	else
	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_FEATHER_OF_THINKING_ONE)
		iFeedType	=	nsMiniPetStyle::Energy;
	else
	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_TOY_OF_SIN_AND_RIN)
		iFeedType	=	nsMiniPetStyle::Mineral;
	else
	{
//	¿½¿½¼¿½¿½ ¿½¿½¿½¿½¿½¿½¿? ¿½¿½¾¿½¿½¿½¿½¿? : ¿½¿½¿½¿½¿?, ¿½¿½¿½¿½¿½´¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½
//	¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½¿? ¿½¿½¾¿½¿½¿½¿½¿? : ¿½¿½¿½¿½ ¿½¿½¿½¿½, ¿½¿½¿½¿½ ¿½¿½¿½¿½, ¿½¿½¿?, ¿½°, ¿½¿½Æ®, ¿½¿½¿½¿½, ¿½¿½¿½¿½, ¿½¿½¿½¿½¿½´¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½
//	¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½¿? ¿½¿½¾¿½¿½¿½¿½¿? : ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½(º¿½¿½¿½¿½ ¿½¿½¿½¿½), ¿½°¿½ ¿½¿½¿½¿½ ¿½³­¿½¿½.

		iFeedType	=	nsMiniPetStyle::Mineral;

		int	aiItemTypeList[2][20]	=
		{
			{
				eIK_EXCLUSIVE_ARMOR				,//"¿½¿½¿½°¿½¿½",
				eIK_ONEHANDED_SWORD				,//"¿½¼°¿½",
				eIK_TWOHANDED_SWORD				,//"¿½¿½°¿?",
				eIK_STAFF						,//"¿½¿½¿½¿½¿½¿½",
				eIK_TOOTH						,//"¿½¿½",
				eIK_MACE						,//"¿½¿½¿½½¿½",
				eIK_WING						,//"¿½¿½¿½¿½",
				eIK_KNIFE						,//"¿½¿½",
				eIK_BOW							,//"°",
				eIK_ARROW						,//"­¿½¿½",
				eIK_SPEAR						,//"",
				eIK_FLUTE						,//"¿½¸¿½",
				eIK_SLING						,//"¿½¿½¿½¿½",
				eIK_BULLET						,//"º",
				eIK_MAGICAL_ROD					,//"¿½¿½¿½¿½¿½¿½",
				eIK_WHIP						,//"¤¿½¿½",
				eIK_SCYTHE						,// ¿½¿½..
				eIK_ARM_WEAPON				,	// ¿½¿½¿½¿½¿½¿½¿½¿½¿½¿½,
				eIK_BOOK						,// "",
				-1,
			},
			{
				eIK_ARMOR,eIK_EXCLUSIVE_ARMOR,eIK_HELM,eIK_GLOVE,eIK_BELT,eIK_BOOTS,eIK_CLOAK,-1,
			},
		};

		for	(int iFeed=nsMiniPetStyle::Bio;iFeed<=nsMiniPetStyle::Energy;iFeed++)
		{
			for	(int iItemKind=0;;iItemKind++)
			{
				int	iCheckKind	=	aiItemTypeList[iFeed-nsMiniPetStyle::Bio][iItemKind];

				if	(iCheckKind	==	iKind)
					iFeedType	=	iFeed;

				if	(iCheckKind	==	-1)
					break;
			}
		}
	}


	return	iFeedType;
}

int
cItem::getExpForMiniPet()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	0;

	int	iExp		=	0;

	if(isExistSpecificEffect(eIE_MINIPET_FEED))
	{
		iExp	=	getMiniPetFeedExp() * 10000;
	}
	else if	(isMiniPetFeed())
	{
		iExp	=	m_aOption[0];
		iExp	=	iExp*255+m_aOption[1];
	}
	else
	{
		if(c_aItemEquipPlace[lpBasicItem->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpBasicItem->m_wKind] == 0xffff)		//  ¿½¿½¿½¿½..
			return	0;

		if	(getSetItem())
			return	0;

		if	(isPremiumItem())
			return	0;

		if	(isExtraItem())
			return	0;

		if	(m_bCount	==	0)
			return	0;

		if	(getStackLimit()	>	1)
			return	0;

		int	iLevel		=	getRequireLevel();

		iExp		=	iLevel;

		if	(isRareItem())
			iExp		*=	2;
		if	(isUniqueItem())
			iExp		*=	20;
	}

//	¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¸¿½ ¿½Ø´¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½­¿½¿½ ¿½¿½¿½¿½¡¿½¿½ ¿½¿½¿½¿½ ¿½¿½´¿?.
//	¿½¿½¿½¿½ ¿½¿½¿½¿½¿½°¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½¿½¿½ ¿½¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½ 2¿½¿½, 20¿½¿½¿½¿½ ¿½¿½¿½¿½¡¿½¿½ ¿½¿½´¿?.
//	¿½¿½, ¿½¿½¿½¿½¿½´¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¿½¿½¿½¿½´¿½ ¿½¿½¿½¿½ ¿½¿½¿½¿½, ¿½°¿½ ¿½¿½¿½¿½ ¿½³­¿½¿½¿½¿½ ¿½Æ·¿½¿½¿½ ¿½¿½¿½¿½¿½¿½ ¿½¸¸­¿½¿½ ¿½¿½¿½¿½¡¿½¿½ ¿½¿½¿½¿½ ¿½¿½´¿?.
	
	return	iExp;
}


BOOL
cItem::upgrade(int _iSlot)
{
	int	j;

	cBasicItem	*lpBasicItem	=	getBasicItem(TRUE);

	if	(!lpBasicItem)
		return	FALSE;

	int	iPrefix	=	m_aPrefix[_iSlot].m_wPrefix;

	if	(iPrefix==	0xffff)
		return	FALSE;

	cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

	for (j=0;j<2;j++)
	{

		int	iMinValue,iMaxValue;
		
		iMinValue	=	lpPrefix->m_aValue[j][0];
		iMaxValue	=	lpPrefix->m_aValue[j][1];
		iMaxValue	=	max(iMaxValue,iMinValue);

		m_aPrefix[_iSlot].m_aValue[j]	=	iMinValue+random(iMaxValue-iMinValue+1);
	}

/*
	for (j=0;j<2;j++)
	{
		int	iMinValue,iMaxValue;
		
		iMinValue	=	lpPrefix->m_aValue[j][0];
		iMaxValue	=	lpPrefix->m_aValue[j][1];

		if	(iMinValue	>	iMaxValue)
		{
			int	iTemp	=	iMinValue;
			iMinValue	=	iMaxValue;
			iMaxValue	=	iTemp;
		}

		int	iRange		=	iMaxValue-iMinValue+1;

		m_aPrefix[_iSlot].m_aValue[j]	=	iMinValue+(timeGetTime()%iRange);
	}
*/

	return	TRUE;
}

//
//	¿½¿½¿½¿½¿½¿½ ¿½¸¿½
char*
cItem::getName()
{
	static	cSTRING		s_strItemName;

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	NULL;

	s_strItemName.Reset();

	if	(isItemPiece() == FALSE && isUnknownItem())
		return	NULL;

	char	strBaseItemName[dITEM_NAME_LENGTH];

	strcpy(strBaseItemName,lpBasicItem->m_strName);

	if	(isItemPack())
		return	NULL;

	cSetItem	*lpSetItem	=	getSetItem();

	if	(lpSetItem)
	{
		s_strItemName.Add("%s",lpSetItem->m_strName);

		return	s_strItemName.String;
	}

	if	(isPrincessWeapon() || isBook() || isAddPageToBookItem() || isSecretDungeonKey() || isBreedingRecordBook() || isExceptionItem())
		return	NULL;
	if	(isExtraItem()		)
		return	s_strItemName.String;

	if	(lpBasicItem->m_wKind == eIK_PREMIUM_ITEM || (!isRareItem() && !isUniqueItem()&& !isLittleGoodItem()) )
	{
		s_strItemName.Add(strBaseItemName);

		return	s_strItemName.String;
	}

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if	(m_aPrefix[i].m_wPrefix	==	0xffff)
			continue;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		s_strItemName.Add("%s ",lpPrefix->m_str1stPrefix);
	}

	s_strItemName.Add(strBaseItemName);

	return	s_strItemName.String;
}
