#include "CItem.H"
#include "CItemEffect.H"
#include "CItemPrefix.H"
#include "cJOB.H"
#include "cPROJECT.H"
#include "cMESSAGE.H"
#include "CHero.H"
#include "cMAP.H"
#include "itemSet.H"
#include "pieceItem.H"
#include "secretDungeon.H"
#include "book.H"
#include "hero_mini_pet.H"

const	int	c_iUltimateItemGenerateDurability=	75;
const	int	c_iUltimateItemGenerateDurabilityRange=	25;

cBasicItem			g_aBasicItem[dBASIC_ITEM_COUNT];
cItem				g_itemGold;	//	금.
cItem				g_itemCarrot;	//	금.
CCustomItemManager	g_premiumItem;
cBasicItem			*g_alpDuelBuffItem[4];

char				g_astrItemPackListName[dMAX_ITEM_PACK_COUNT][32];

cSTRING				cItem::s_strTooltip;
DWORD				cItem::s_dwGeneatedToolTipSerial	=	0xffffffff;
int					cItem::s_iGeneatedToolTipCheckSum	=	0xffffffff;
int					cItem::s_iGeneatedToolTipBaseSerial	=	0xffff;
int					cItem::s_iCoupleRing	=	0xffff;
int					g_iBasicItemCount		=	0;

char	cBasicItem::s_strComment[2048];

//
//	코멘트 얻어 오기
char*
cBasicItem::getComment()
{
	int		iSize;

	cFILE	file;

	g_project.setProjectFolder();

	if	(!file.Open(dITEM_FILE_NAME,"rb"))
		return	NULL;

	file.Skip(8);			//	국가정보,아이템 수
	file.Read(&iSize,4);	//	텍스트 데이터가 저장된 위치

	iSize	+=	m_dwCommentAddress;
	
	file.Seek(iSize,SEEK_SET);
	file.Read(&iSize,4);			//	텍스트 데이터가 저장된 위치
	file.Read(s_strComment,iSize);			//	텍스트 데이터가 저장된 위치
	file.Close();

	SetLogFolder();

	return	s_strComment;
}

//
//	착용한 모양과 직업을 기본으로 해서 아이템 구하기
int
cBasicItem::GetItemByEquippedShape(int _iShape,int _iJob,int _iEquipPart)
{
	for (int i=0;i<dBASIC_ITEM_COUNT;i++)
	{
		cBasicItem	*lpItem	=	&g_aBasicItem[i];

		if (lpItem->m_iSerial	>=	dBASIC_ITEM_COUNT	||	lpItem->m_iSerial	<	0)
			continue;

		if (lpItem->isEnableJob(_iJob) && lpItem->m_wEquippedShape == _iShape && c_aItemEquipPlace[lpItem->m_wKind]==_iEquipPart)
			return i;
	}

	return	0xffff;
}

//
//	가격 인자 얻기
DWORD
cITEM_PREFIX::getPriceValue(int _iValue1,int _iValue2)
{
	DWORD	dwPrefixValue	=	0;

	switch(m_wComputePriceMethod)
	{
		case	eCIPP_PRICE							:	//	가격으로 처리
			dwPrefixValue	=	m_dwPrice*100;
			break;

		case	eCIPP_VALUE1						:	//	수치1
			dwPrefixValue	=	_iValue1*100;
			break;

		case	eCIPP_VALUE1_MUL_PRICE				:	//	수치1 * 가격
			dwPrefixValue	=	_iValue1*100*m_dwPrice;
			break;

		case	eCIPP_VALUE1_DIV_PRICE				:	//	수치1 / 가격
			dwPrefixValue	=	_iValue1*100/m_dwPrice;
			break;

		case	eCIPP_VALUE1_MUL_PRICE_DIV_100		:	//	수치1*가격/100
			dwPrefixValue	=	_iValue1*m_dwPrice;//*100/100;
			break;

		case	eCIPP_PRICE_MUL_VALUE2_DIV_VALUE1	:	//	가격*수치2/수치1
			dwPrefixValue	=	m_dwPrice*_iValue2*100/_iValue1;
			break;
	}

	return	dwPrefixValue;
}

BOOL
cItem::isBlockLightPocket()
{
	return	checkFirstEffect(eIE_SEAL_MINIPET);
}

BOOL
cItem::isCanNotFeedToPetItem()
{
	cBasicItem	*lpBasicItem	=	getBasicItem(TRUE);

	if	(!lpBasicItem)
		return	TRUE;

	if	(getSpecialItem() && getSpecialItem()->m_bf1IsFreeUseItem)
		return	TRUE;

	return	lpBasicItem->m_bf1IsBlockToFeedPet;
}

BOOL
cItem::isUpgradeAbleItem(int _iPrefixSlot)
{
	return	upgradeItem(_iPrefixSlot,0,0,TRUE);
}

BOOL
cItem::upgradeItem(int _iPrefix,int _iValue1,int _iValue2,BOOL _bIsWantTest)
{
	int	j;

	cBasicItem	*lpBasicItem	=	getBasicItem(TRUE);

	if	(!lpBasicItem)
		return	FALSE;

	int	iPrefix	=	m_aPrefix[_iPrefix].m_wPrefix;

	if	(iPrefix==	0xffff)
		return	FALSE;

	cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

	for (j=0;j<2;j++)
	{

		int	iMinValue,iMaxValue;
		
		iMinValue	=	lpPrefix->m_aValue[j][0];
		iMaxValue	=	lpPrefix->m_aValue[j][1];
		iMaxValue	=	max(iMaxValue,iMinValue);

		if	(m_aPrefix[_iPrefix].m_aValue[j]	<	iMaxValue)
		{
			if	(_bIsWantTest)
				return	TRUE;
		}

		if	(_bIsWantTest	==	FALSE)
		{
			if	(j	==	0)
				m_aPrefix[_iPrefix].m_aValue[j]	=	_iValue1;
			else
				m_aPrefix[_iPrefix].m_aValue[j]	=	_iValue2;
		}
	}

	return	FALSE;
}

//
//	무한 탄환이냐?
BOOL
cItem::isInfinityBullet()
{
	int			i;
	cBasicItem	*lpBasicItem	=	getBasicItem(TRUE);

	if	(!lpBasicItem)
		return	FALSE;

	if	(isPrincessWeapon())
		return	TRUE;

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
		if	(g_aItemPrefix[m_aPrefix[i].m_wPrefix].m_wType==	eIP_INFINITY_BULLET)
			return	TRUE;
	}

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
		if	(lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_INFINITY_BULLET)	
			return	TRUE;

	for (i=0;i<dITEM_UNIQUE_DATA_COUNT;i++)
		if	(lpBasicItem->m_aUniqueData[i].m_wEffect		==	eIP_INFINITY_BULLET)	
			return	TRUE;

	return	FALSE;
}

BOOL
cItem::isQuestItem()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if (lpBasicItem->m_questIndex	== dNOT_QUEST_ITEM)
		return	FALSE;

	return	TRUE;
}

cBasicItem*
cItem::getBasicItem(BOOL _bIsWantPrincessWeaponBasicItem)
{
	if	(m_wBaseItem	>=	dBASIC_ITEM_COUNT)
		return	NULL;

	if	(_bIsWantPrincessWeaponBasicItem	&&	isPrincessWeapon())
		return	&g_aBasicItem[getPrincessWeapon()->m_wBasicItem];

	return	&g_aBasicItem[m_wBaseItem];
}

DWORD
cItem::getNameColor()	//	출력시 이름 컬러
{
	if	(isExceptionItem())
		return	dINC_NORMAL;

	if	(isSecretDungeonKey() || isBook() || isAddPageToBookItem())
		return	CTBLUE;

	if	(isSuperUniqueItem() || getBasicItem()->isEternalItem())
		return dINC_UNIQUE;
	if	(isUniqueItem())
		return dINC_UNIQUE;
	else
	if	(isRareItem())
		return dINC_RARE;
	else
	if	(isLittleGoodItem())
		return	dINC_LITTLE_GOOD;

	return dINC_NORMAL;
}

BOOL
cItem::isNormalItem()//이건 보통 아이템이냐
{	
	if (isExceptionItem())
		return	TRUE;

	if (m_aPrefix[0].m_wPrefix	!=	0xffff)
		return	FALSE;	//	레어.. 다 -o-

	cBasicItem	*lpItem		=	getBasicItem();
	CRY(!lpItem,"cry in cItem::isNormalItem - 크아아!! 빈 아이템을 왜 검색하는겨!!");

	if (lpItem->m_aUniqueData[0].m_wEffect	!=	0xffff)
		return	FALSE;	//	유니크... 다 -o-

	return	FALSE;
}

BOOL
cItem::isLittleGoodItem()
{
	if	(isExceptionItem())
		return	FALSE;

	if	(m_aPrefix[0].m_wPrefix	==	0xffff)	
		return	FALSE;

	BOOL	bIsLittleGood	=	FALSE;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if	(m_aPrefix[i].m_wPrefix	==	0xffff)	
			break;

		if	(g_aItemPrefix[m_aPrefix[i].m_wPrefix].m_wDiscernmentCode	>=	10000)
			bIsLittleGood	=	TRUE;
		else
			return	FALSE;
	}

	return	bIsLittleGood;
}
	
BOOL
cItem::isRareItem()	
{
	if	(isExceptionItem())
		return	FALSE;

	if	(isSetItem())
		return	FALSE;

	if	(m_aPrefix[0].m_wPrefix	==	0xffff)	
		return	FALSE;

	if	(isLittleGoodItem())
		return	FALSE;

	return	TRUE;
}
BOOL
cItem::isUniqueItem()//이건 유니크냐?
{
	if	(isExceptionItem())
		return	FALSE;

	if	(isExtraItem())
		return	FALSE;
	
	cBasicItem	*lpItem		=	getBasicItem();
	CRY(!lpItem,"cry in cItem::isNormalItem - 크아아!! 빈 아이템을 왜 검색하는겨!!");


	return lpItem->isUnique();


}

BOOL
cItem::isSuperUniqueItem()//이건 유니크냐?
{
	if	(isExceptionItem())
		return	FALSE;
	
	cBasicItem	*lpItem		=	getBasicItem();
	CRY(!lpItem,"cry in cItem::isNormalItem - 크아아!! 빈 아이템을 왜 검색하는겨!!");

	if(lpItem->isUnique() == FALSE)
		return FALSE;

	if (m_aPrefix[0].m_wPrefix	==	0xffff)	
		return	FALSE;

	return	TRUE;
}
int
cItem::getUnlockDoorLevel()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if (!lpItem)
		return	FALSE;

	if (lpItem->m_wKind	!=	eIK_KEY)
		return	0;

	int	iEffect	=	lpItem->m_aGenerateData[0].m_wEffect;

	if (iEffect	==	eIE_OPEN_DOOR	||	iEffect	==	eIE_OPEN_ARCA_AND_DOOR)
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

int
cItem::getFullRepairPrice()
{
	if	(isExceptionItem())
		return	0;

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if (!lpBasicItem)	
		return	0;

	if	(isBreakdownAbleItem()	==	FALSE)
		return	0;

	int	iDamagedPoint	=	lpBasicItem->getDurability() - m_bDurability;

	if (iDamagedPoint	<=	0)
		return	0;

	LONGLONG	llPrice	=	getPrice(m_bCount);
	
	if	(isDXItem())
		llPrice		=	llPrice*iDamagedPoint/lpBasicItem->getDurability()/dDX_ITEM_REPAIR_FACTOR;
	else
		llPrice		=	llPrice*iDamagedPoint/lpBasicItem->getDurability()/dULTIMATE_ITEM_REPAIR_FACTOR;

	int	iPrice	=	min((int)llPrice,0x7fffffff);

	return	max(iPrice,1);
}


//
//	가격을 계산한다.
DWORD
cItem::getPrice(int _iCount)
{
//	정석으로 하면 아이템 1개의 가격을 계산한후에 곱하기 갯수를 해야 겠지만,
//	계산식중 나누기 10을 하는 녀석이 있어서(0이 나올수도 있다.)
//	기본 가격과 더해주는 가격에 먼저 갯수를 곱해주기로 한다.
	if	(isExceptionItem())
		return	FALSE;

	DWORD	dwPrice;
	int		i;

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if (!lpBasicItem)
		return	0;

	dwPrice	=	lpBasicItem->m_dwPrice;

	switch(lpBasicItem->m_wPriceComputeMethod)
	{
		case	eCBIP_PRICE							:	//	가격으로 처리
			break;

		case	eCBIP_VALUE1						:	//	수치1
			dwPrice	=	m_aOption[0];
			break;

		case	eCBIP_PRICE_MUL_VALUE1				:	//	가격 * 수치1
			dwPrice	*=	m_aOption[0];
			break;

		case	eCBIP_PRICE_MUL_VALUE2				:	//	가격 * 수치2
			dwPrice	*=	m_aOption[1];
			break;

		case	eCBIP_PRICE_MUL_VALUE1_PLUS1		:	//	가격 * (수치1 +1)
			dwPrice	*=	(m_aOption[0]+1);
			break;

		case	eCBIP_VALUE1_MUL_VALUE2_DIV_PRICE	:	//	수치1*수치2/가격
			dwPrice	=	m_aOption[0]*m_aOption[1]/dwPrice;
			break;
		case	eCBIP_PRICE_DIV_10					:	//	가격 / 10
			dwPrice	=	dwPrice*_iCount/10;
			break;
		case	eCBIP_VALUE1_MUL_VALUE2_MUL_PRICE	:	//	수치1*수치2*가격
			dwPrice	=	m_aOption[0]*m_aOption[1]*dwPrice;
			break;
	}

	dwPrice	=	max(dwPrice,1);

	if (lpBasicItem->m_wPriceComputeMethod	!=	eCBIP_PRICE_DIV_10)	dwPrice	*=	_iCount;	//	/10 하는 넘땜시 미리 *카운트 해준다.

	DWORD	dwAddPrice	=	0,dwMultiPrice	=	0;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		CItemPrefixInfo	*lpData		=	&m_aPrefix[i];

		if (lpData->m_wPrefix	==	0xffff)	break;

		cITEM_PREFIX	*lpPrefix		=	&g_aItemPrefix[lpData->m_wPrefix];
		DWORD			dwPrefixValue	=	lpPrefix->getPriceValue(lpData->m_aValue[0],lpData->m_aValue[1]);

		if (lpPrefix->m_wPriceFactor	==	0)	dwAddPrice	+=	dwPrefixValue;
		else									dwMultiPrice+=	dwPrefixValue;
	}

	dwMultiPrice/=	100;
	dwAddPrice	=	dwAddPrice*_iCount/100;

	dwPrice		=	(dwPrice+dwAddPrice)*(1+dwMultiPrice);
	
	if	(isUniqueItem())
		dwPrice	=	dwPrice*lpBasicItem->m_wPriceValue/100;

	return	dwPrice;
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

	if	(lpItem->m_bf1IsUltimate == TRUE)
		return	TRUE;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if	(m_aPrefix[i].m_wPrefix	==	0xffff)
			break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if	(lpPrefix->m_wIsUltimatePrefix)
			return	TRUE;
	}

	return	FALSE;
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
cItem::isExtraItem(BOOL _bIsExceptChargeItem)
{
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if	(isExceptionItem())
		return	FALSE;

	if	(lpItem->m_attr.isExtraItem == TRUE)
	{
		if	(_bIsExceptChargeItem)
			if	(lpItem->isChargeExtraItem())
				return	FALSE;

		return	TRUE;
	}

	return	FALSE;
}


//
//JBC	각성의 씨앗을 확인 한다. 08-12-31
BOOL
cItem::isMiniPetAwakenItem50()
{
	cBasicItem	*lpBaseItem	=	getBasicItem();
	
	if (!lpBaseItem)
		return	FALSE;
	
	if (lpBaseItem->m_aGenerateData[0].m_wEffect	==	eIE_MINIPET_AWAKEN50)
		return	TRUE;
	
	return	FALSE;
}

// 
// JBC	각성의 열매를 확인 한다. 08-12-31
BOOL
cItem::isMiniPetAwakenItem100()
{
	cBasicItem	*lpBaseItem	=	getBasicItem();
	
	if (!lpBaseItem)
		return	FALSE;
	
	if (lpBaseItem->m_aGenerateData[0].m_wEffect	==	eIE_MINIPET_AWAKEN100)
		return	TRUE;
	
	return	FALSE;
}



BOOL
cItem::isPolisher()
{
	cBasicItem	*lpBaseItem	=	getBasicItem();

	if (!lpBaseItem)
		return	FALSE;

	if (lpBaseItem->m_aGenerateData[0].m_wEffect	==	eIE_EXTEND_MINIPET_TIME)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::isMassivePolisher()
{
	cBasicItem	*lpBaseItem	=	getBasicItem();

	if (!lpBaseItem)
		return	FALSE;

	if (lpBaseItem->m_aGenerateData[0].m_wEffect	==	eIE_EXTEND_ALL_MINIPET_TIME)
		return	TRUE;

	return	FALSE;
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
cItem::isBreakdownItem()
{
	if	(isExceptionItem())
		return	FALSE;

	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if (c_aItemEquipPlace[lpItem->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpItem->m_wKind] == 0xffff)		//  변경..
		return	FALSE;

	if	(isBreakdownAbleItem() == FALSE)
		return	FALSE;

	if	(lpItem->getDurability() > m_bDurability)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::isManExclusiveItem()
{
	cSetItem		*lpSetItem		=	getSetItem();

	if	(!lpSetItem || lpSetItem->m_uiEnableJobMask	==	0)
	{
		cBasicItem		*lpBasicItem	=	getBasicItem(TRUE);

		return	lpBasicItem->isManExclusiveItem();
	}

	return	FALSE;
}

BOOL
cItem::isWomanExclusiveItem()
{
	cSetItem		*lpSetItem		=	getSetItem();

	if	(!lpSetItem || lpSetItem->m_uiEnableJobMask	==	0)
	{
		cBasicItem		*lpBasicItem	=	getBasicItem(TRUE);

		return	lpBasicItem->isWomanExclusiveItem();
	}

	return	FALSE;
}

BOOL
cItem::isEnableJob(int _iJob)
{
	cSetItem		*lpSetItem		=	getSetItem();

	if	(!lpSetItem || lpSetItem->m_uiEnableJobMask	==	0)
	{
		cBasicItem		*lpBasicItem	=	getBasicItem(TRUE);

		return	lpBasicItem->isEnableJob(_iJob);
	}

	return	lpSetItem->isEnableJob(_iJob);
}

BOOL
cItem::isAllJobItem()
{
	cSetItem		*lpSetItem		=	getSetItem();

	if	(!lpSetItem || lpSetItem->m_uiEnableJobMask	==	0)
	{
		cBasicItem		*lpBasicItem	=	getBasicItem(TRUE);

		return	lpBasicItem->isAllJobItem();
	}

	for(int i=0 ; i<dPLAYER_JOB_COUNT ; ++i)
	{
		if(lpSetItem->isEnableJob(i) == FALSE)
			return FALSE;
	}

	return	TRUE;
}

BOOL
cItem::isLimitJobItem()
{
	cSetItem		*lpSetItem		=	getSetItem();

	if	(!lpSetItem || lpSetItem->m_uiEnableJobMask	==	0)
	{
		cBasicItem		*lpBasicItem	=	getBasicItem(TRUE);

		if	(lpBasicItem->isAllJobItem()	==	FALSE)
			return	TRUE;

		if	(lpBasicItem->isManExclusiveItem() + lpBasicItem->isWomanExclusiveItem())
			return	TRUE;

		return	FALSE;
	}

	return	TRUE;
}

BOOL
cItem::isUnknownItem()
{
	cSpecialItem	*lpSpecialItem	=	getSpecialItem();

	if	(!lpSpecialItem)
		return	FALSE;

	return	lpSpecialItem->m_bf1IsUnknown;
}

BOOL
cItem::isShopSpecialItem()
{

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

	cSpecialItem	*lpSpecialItem	=	getSpecialItem();

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
cItem::isItemPiece()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_ITEM_PIECE)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::isSetItem()
{
	if	(getSetItem())
		return	TRUE;

	return	FALSE;
}

int
cItem::getJudgePrice()
{
	cPieceItem	*lpPiece	=	getPieceItem();

	if	(lpPiece)
		return	lpPiece->getJudgePrice(m_aOption[0]);

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	0;

	return	lpBasicItem->m_wDropLevel*100;
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
cItem::isPresentCoupleRing()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_PRESENT_COUPLE_RING)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::isNotDefineCoupleCoupleRing()
{
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_NOT_DEFINE_COUPLE_COUPLE_RING)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::isCoupleRing(BOOL _bIsCheckUseAble)
{
	cBasicItem	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_COUPLE_RING)
	{
		if	(_bIsCheckUseAble)
		{
			if	(getCoupleRing()->m_strCoupleName[0]	==	NULL)
				return	FALSE;
		}

		return	TRUE;
	}

	return	FALSE;
}

BOOL
cItem::isSecretDungeonKey()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if (!lpBasicItem)
		return	FALSE;

	if (lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_SECRET_DUNGEON_KEY)
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

	if (!lpSpecialItem || !isItemPiece())
		return	NULL;

	return	g_pieceItemManager.get(lpSpecialItem->m_bf11SValue);
}

cItemSet*
cItem::getItemSet()
{
	cSpecialItem	*lpSpecialItem	=	getSpecialItem();

	if (!lpSpecialItem || lpSpecialItem->m_bf1IsSetItem == FALSE)
		return	NULL;

	return	g_itemSetManager.get(lpSpecialItem->m_bf11SValue);
}

cPrincessWeaponDefine*
cItem::getPrincessWeapon()
{
	if	(!isPrincessWeapon())
		return	NULL;

	return	(cPrincessWeaponDefine*)this;
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
//	접두사로 인해 착용/사용에 필요한 레벨
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

	if (iMaxPrefixIndex	==	0)	iFirst	=	1,iSecond	=	2;
	if (iMaxPrefixIndex	==	1)	iFirst	=	0,iSecond	=	2;
	if (iMaxPrefixIndex	==	2)	iFirst	=	0,iSecond	=	1;

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

//
//	명사의 수
int
cItem::getNounPrefixCount()
{
	if	(isExceptionItem())
		return	0;

	int	iPrefixCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)		break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if (lpPrefix->m_wPrefixForm	==	0)	iPrefixCount++;
	}

	return	iPrefixCount;
}

//
//	형용사의 수
int
cItem::getAdjectivePrefixCount()
{
	if	(isExceptionItem())
		return	0;

	int	iPrefixCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)		break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if (lpPrefix->m_wPrefixForm	==	1)	iPrefixCount++;
	}

	return	iPrefixCount;
}

char*
cItem::getBookName()
{
	cBookItemDefine			*lpBook		=	getBook();
	cBookPageItemDefine		*lpPage		=	getAddPageToBookItem();
	cBookCategoryItemDefine	*lpCategory	=	getAddCategoryToBookItem();

	int	iBookIndex	=	-1;

	if	(lpBook)
		iBookIndex	=	lpBook->m_wBookIndex;
	if	(lpPage)
		iBookIndex	=	lpPage->m_wBookIndex;
	if	(lpCategory)
		iBookIndex	=	lpCategory->m_wBookIndex;

	if	(iBookIndex	!=	-1)
		return	g_book.getBookName(iBookIndex);

	return	"";
}

int
cItem::getBeginPageInBook()
{
	cBookPageItemDefine	*lpPage	=	getAddPageToBookItem();

	if	(!lpPage)
		return	0;

	return	g_book.getPageInBook(lpPage->m_wBookIndex,lpPage->m_wCategory,lpPage->m_wBeginPage);
}

int
cItem::getEndPageInBook()
{
	cBookPageItemDefine	*lpPage	=	getAddPageToBookItem();

	if	(!lpPage)
		return	0;

	return	g_book.getPageInBook(lpPage->m_wBookIndex,lpPage->m_wCategory,lpPage->m_wEndPage);
}

int
cItem::getAvailPageCount()
{
	cBookItemDefine	*lpBookItem	=	getBook();

	if	(!lpBookItem)
		return	0;

	cBookInfoDefine	*lpBook		=	g_book.getBook(lpBookItem->m_wBookIndex);

	int				iPageCount	=	0;

	for	(int i=0;i<lpBook->m_wCategoryCount;i++)
	{
		for	(int j=0;j<lpBook->m_aCategory[i].m_iPageCount;j++)
		{
			int	iMask	=	1<<j;

			if	((lpBookItem->m_aCategoryInfo[i]&iMask) == iMask)
				iPageCount++;
		}
	}

	return	iPageCount;
}

int
cItem::getTotalPageCount()
{
	cBookItemDefine	*lpBookItem	=	getBook();

	if	(!lpBookItem)
		return	0;

	cBookInfoDefine	*lpBook		=	g_book.getBook(lpBookItem->m_wBookIndex);

	int		iTotalPageCount		=	max(g_book.getTotalPageCount(lpBookItem->m_wBookIndex),1);

	return	iTotalPageCount;
}

int
cItem::getCompleteBookPencentage()
{
	cBookItemDefine	*lpBookItem	=	getBook();

	if	(!lpBookItem)
		return	0;

	cBookInfoDefine	*lpBook		=	g_book.getBook(lpBookItem->m_wBookIndex);

	int			iPageCount		=	0;
	int			iTotalPageCount	=	0;

	for	(int i=0;i<lpBook->m_wCategoryCount;i++)
	{
		for	(int j=0;j<lpBook->m_aCategory[i].m_iPageCount;j++)
		{
			int	iMask	=	1<<j;

			if	((lpBookItem->m_aCategoryInfo[i]&iMask) == iMask)
				iPageCount++;
		}

		iTotalPageCount	+=	lpBook->m_aCategory[i].m_iPageCount;
	}

	iPageCount		-=	lpBook->m_wCategoryCount;
	iTotalPageCount	-=	lpBook->m_wCategoryCount;
	iPageCount		=	max(iPageCount,0);
	iTotalPageCount	=	max(iTotalPageCount,1);

	return	iPageCount*100/iTotalPageCount;
}

BOOL
cItem::isAvailPage(int _iCategory,int _iPage)
{
	cBookItemDefine	*lpBookItem	=	getBook();

	if	(!lpBookItem)
		return	FALSE;

	cBookInfoDefine	*lpBook	=	g_book.getBook(lpBookItem->m_wBookIndex);
	int				iMask	=	1<<_iPage;

	if	((lpBookItem->m_aCategoryInfo[_iCategory]&iMask) == iMask)
		return	TRUE;

	return	FALSE;
}

BOOL
cItem::isAvailPage(int _iPage)
{
	cBookItemDefine	*lpBookItem	=	getBook();

	if	(!lpBookItem)
		return	FALSE;

	cBookInfoDefine	*lpBook	=	g_book.getBook(lpBookItem->m_wBookIndex);
	int				iPageCount	=	0;

	for	(int i=0;i<lpBook->m_wCategoryCount;i++)
	{
		if	(_iPage	<	iPageCount+lpBook->m_aCategory[i].m_iPageCount)
		{
			if	(_iPage-iPageCount	==	0)
				return	TRUE;

			int	iMask	=	1<<(_iPage-iPageCount);

			if	((lpBookItem->m_aCategoryInfo[i]&iMask) == iMask)
				return	TRUE;

			return	FALSE;
		}

		iPageCount	+=	lpBook->m_aCategory[i].m_iPageCount;
	}

	return	FALSE;
}

char*
cItem::getCategoryName()
{
	cBookCategoryItemDefine	*lpCategory	=	getAddCategoryToBookItem();

	if	(!lpCategory)
		return	"";

	return	g_book.getCategoryName(lpCategory->m_wBookIndex,lpCategory->m_wBeginCategory);
}

//
//	아이템 이름
char*
cItem::getName(BOOL _bIsWantColor)
{
	static	cSTRING		s_strItemName;

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	NULL;

	s_strItemName.Reset();

	if	(isItemPiece() == FALSE && isUnknownItem())
	{
		s_strItemName.Add(dMSG_UNKNOWN_ITEM);

		return	s_strItemName.String;
	}

	char	strBaseItemName[dITEM_NAME_LENGTH];

	strcpy(strBaseItemName,lpBasicItem->m_strName);

	if	(isItemPack())
	{
		if	(g_astrItemPackListName[m_aOption[0]][0])
		{
			strcpy(strBaseItemName,g_astrItemPackListName[m_aOption[0]]);
#ifdef	_DEBUG
			if	(strBaseItemName[0] == '?')
				sprintf(strBaseItemName,"[?] It's use List Pack Index %d",m_aOption[0]);
#endif
		}
		else
		{
//			strcpy(strBaseItemName,"???????");
			CGamePlay::AddAskItemPackName(m_aOption[0]);
		}
	}

	cSetItem	*lpSetItem	=	getSetItem();

	if	(lpSetItem)
	{
		if	(_bIsWantColor)
			s_strItemName.Add("<c:CTBLUE>%s<n>",lpSetItem->m_strName);
		else
			s_strItemName.Add("%s",lpSetItem->m_strName);

		return	s_strItemName.String;
	}

	if	(isPrincessWeapon())
	{
		if	(_bIsWantColor)
			s_strItemName.Add("<c:LTPINK>%s<n> <c:GOLDYELLOW>%s<n>",dMSG_PRINCESS_TRANS_WEAPON,getBasicItem(TRUE)->m_strName);
		else
			s_strItemName.Add("%s %s",dMSG_PRINCESS_TRANS_WEAPON,getBasicItem(TRUE)->m_strName);

		return	s_strItemName.String;
	}
	else
	if	(isBook())
	{
		if	(_bIsWantColor)
			s_strItemName.Add("<c:CTBLUE>%s<n>",getBookName());
		else
			s_strItemName.Add("%s",getBookName());

		return	s_strItemName.String;
	}
	else
	if	(isAddPageToBookItem())
	{
		int	iBeginPage	=	getBeginPageInBook()+1;
		int	iEndPage	=	getEndPageInBook()+1;

		if	(_bIsWantColor)
		{
			if	(iBeginPage	==	iEndPage)
				s_strItemName.Add("<c:CTBLUE>%s[%s %d]<n>",getBookName(),dMSG_PAGE,iBeginPage);
			else
				s_strItemName.Add("<c:CTBLUE>%s[%s %d~%d]<n>",getBookName(),dMSG_PAGE,iBeginPage,iEndPage);
		}
		else
		{
			if	(iBeginPage	==	iEndPage)
				s_strItemName.Add("%s[%s %d]",getBookName(),dMSG_PAGE,iBeginPage);
			else
				s_strItemName.Add("%s[%s %d~%d]",getBookName(),dMSG_PAGE,iBeginPage,iEndPage);
		}

		return	s_strItemName.String;
	}
	else
	if	(isSecretDungeonKey()		)
	{
		cSecretDungeon	*lpDungeon	=	g_sdManager.get(m_aPrefix[0].m_wPrefix);

		if	(lpDungeon)
		{
			if	(_bIsWantColor)
				s_strItemName.Add("<c:CTBLUE>%s ",lpDungeon->m_strName);
			else
				s_strItemName.Add("%s ",lpDungeon->m_strName);
		}

		if	(_bIsWantColor)
			s_strItemName.Add("<c:CTBLUE>%s<n>",lpBasicItem->m_strName);
		else
			s_strItemName.Add("%s",lpBasicItem->m_strName);

		return	s_strItemName.String;
	}
	else
	if	(isBreedingRecordBook()		)
	{
		if	(_bIsWantColor)
			s_strItemName.Add("<c:CTBLUE>%s<n>",strBaseItemName);

		return	s_strItemName.String;
	}
	else
	if	(isExceptionItem() && lpBasicItem->isEternalItem() == FALSE)
	{
		if	(_bIsWantColor)
			s_strItemName.Add("<c:CTBLUE>%s<n>",strBaseItemName);

		return	s_strItemName.String;
	}
	else
	if	(isExtraItem()		)
	{
		if (_bIsWantColor)
			s_strItemName.Add("<c:CTBLUE>%s<n>",strBaseItemName);

		return	s_strItemName.String;
	}
	else
	if (m_isBroken					)
	{
		if (_bIsWantColor)
			s_strItemName.Add("<c:LTRED>%s<n> ",dMSG_BROKEN);
	}
	else
	if (m_bf1IsSpecialItem	==	FALSE	&&	m_isCanNotAttachPrefixItem	)
	{
		if (_bIsWantColor)
			s_strItemName.Add("<c:LTRED>%s<n> ",dMSG_CAN_NOT_IMPROVE);
	}

	if	(lpBasicItem->m_wKind == eIK_PREMIUM_ITEM || (!isRareItem() && !isUniqueItem()&& !isLittleGoodItem() && !lpBasicItem->isEternalItem()) )
	{
		s_strItemName.Add(strBaseItemName);
		if(lpBasicItem->m_bf1IsRingOfInfinity && getCarvingLevel())
		{
		s_strItemName.Add("<c:LTGREEN>Lv %d<n>]", getCarvingLevel());
		}
		return	s_strItemName.String;
	}

	if (isRareItem())
	{
		if (_bIsWantColor)
			s_strItemName.Add("<c:LTGREEN>");
	}
	if (isLittleGoodItem())
	{
		if (_bIsWantColor)
			s_strItemName.Add("<c:GREEN>");
	}

//	형용사 2차 + 형용사 2차 + 형용사 1차 + 아이템
//	형용사 2차 + 형용사 1차 + 명사 1차 + 아이템
//	형용사 1차 + 명사 2차 + 명사 1차 + 아이템
//	명사 2차 + 명사 2차 + 명사 1차 + 아이템
	int	iPrefixCount	=	getPrefixCount();
	int	iNounCount		=	getNounPrefixCount();

//	접두사가 하나 일때
	if (iPrefixCount	==	1)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[0].m_wPrefix];

		s_strItemName.Add("%s ",lpPrefix->m_str1stPrefix);
	}

//	접두사가 둘 일때
	if (iPrefixCount	==	2)
	{
		cITEM_PREFIX	*lpPrefix1	=	&g_aItemPrefix[m_aPrefix[0].m_wPrefix];
		cITEM_PREFIX	*lpPrefix2	=	&g_aItemPrefix[m_aPrefix[1].m_wPrefix];

		if (iNounCount	==	0)	//	둘다 형용사
		{
			s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
			s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
		}

		if (iNounCount	==	1)
		{
			if (lpPrefix1->m_wPrefixForm	==	0)	//	명사
			{
				s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
				s_strItemName.Add("%s ",lpPrefix1->m_str1stPrefix);
			}
			else	//	형용사
			{
				s_strItemName.Add("%s ",lpPrefix1->m_str1stPrefix);
				s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
			}
		}

		if (iNounCount	==	2)	//	둘다 명사
		{
			s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
			s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
		}
	}

//	접두사가 셋일때
	if (iPrefixCount	==	3)
	{
		cITEM_PREFIX	*lpPrefix1	=	&g_aItemPrefix[m_aPrefix[0].m_wPrefix];
		cITEM_PREFIX	*lpPrefix2	=	&g_aItemPrefix[m_aPrefix[1].m_wPrefix];
		cITEM_PREFIX	*lpPrefix3	=	&g_aItemPrefix[m_aPrefix[2].m_wPrefix];

		if (iNounCount	==	0)	//	셋 다 형용사
		{
			s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
			s_strItemName.Add("%s ",lpPrefix2->m_str2stPrefix);
			s_strItemName.Add("%s ",lpPrefix3->m_str1stPrefix);
		}

		if (iNounCount	==	1)	//	명사가 하나
		{
			if (lpPrefix1->m_wPrefixForm	==	0)	//	명사
			{
				s_strItemName.Add("%s ",lpPrefix3->m_str2stPrefix);
				s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
				s_strItemName.Add("%s ",lpPrefix1->m_str1stPrefix);
			}

			if (lpPrefix2->m_wPrefixForm	==	0)	//	명사
			{
				s_strItemName.Add("%s ",lpPrefix3->m_str2stPrefix);
				s_strItemName.Add("%s ",lpPrefix1->m_str1stPrefix);
				s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
			}

			if (lpPrefix3->m_wPrefixForm	==	0)	//	명사
			{
				s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
				s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
				s_strItemName.Add("%s ",lpPrefix3->m_str1stPrefix);
			}
		}

		if (iNounCount	==	2)	//	형용사가 하나
		{
			if (lpPrefix1->m_wPrefixForm	==	1)	//	형용사
			{
				s_strItemName.Add("%s ",lpPrefix1->m_str1stPrefix);
				s_strItemName.Add("%s ",lpPrefix2->m_str2stPrefix);
				s_strItemName.Add("%s ",lpPrefix3->m_str1stPrefix);
			}

			if (lpPrefix2->m_wPrefixForm	==	1)	//	형용사
			{
				s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
				s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
				s_strItemName.Add("%s ",lpPrefix3->m_str1stPrefix);
			}

			if (lpPrefix3->m_wPrefixForm	==	1)	//	형용사
			{
				s_strItemName.Add("%s ",lpPrefix3->m_str1stPrefix);
				s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
				s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
			}
		}

		if (iNounCount	==	3)	//	셋 다 명사
		{
			s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
			s_strItemName.Add("%s ",lpPrefix2->m_str2stPrefix);
			s_strItemName.Add("%s ",lpPrefix3->m_str1stPrefix);
		}
	}

	if (lpBasicItem->isEternalItem())
	{
		if	(_bIsWantColor)
			s_strItemName.Add("<c:GOLDYELLOW>%s<n>",strBaseItemName);
		else
		{
			s_strItemName.Add(strBaseItemName);
			s_strItemName.Insert("<c:GOLDYELLOW>[E] <n>");
		}
	}
	else
	if (isSuperUniqueItem())
	{
		if (_bIsWantColor)
			s_strItemName.Add("<c:GOLDYELLOW>%s<n>",strBaseItemName);
		else
		{
			s_strItemName.Add(strBaseItemName);
			s_strItemName.Insert("<c:GOLDYELLOW>[S] <n>");
		}
	}
	else
	if (isUniqueItem())
	{
		if (_bIsWantColor)
			s_strItemName.Add("<c:GOLDYELLOW>%s<n>",strBaseItemName);
		else
		{
			s_strItemName.Add(strBaseItemName);
			s_strItemName.Insert("<c:GOLDYELLOW>[U] <n>");
		}
	}
	else
	if (isRareItem())
	{
		if (_bIsWantColor)
			s_strItemName.Add("<c:BTBLUE>%s<n>",strBaseItemName);
		else
		{
			s_strItemName.Add(strBaseItemName);
			s_strItemName.Insert("<c:BTBLUE>[R] <n>");
		}
	}
	else
	if (isLittleGoodItem())
	{
		if (_bIsWantColor)
			s_strItemName.Add("<c:PURPLE>%s<n>",strBaseItemName);
		else
		{
			s_strItemName.Add(strBaseItemName);
			s_strItemName.Insert("<c:PURPLE>[R] <n>");
		}
	}
	else
	{
		if (_bIsWantColor)
			s_strItemName.Add("<c:WHITE>%s<n>",strBaseItemName);
		else
			s_strItemName.Add(strBaseItemName);
	}

	if	(isDXItem() && lpBasicItem->m_attr.isDXItem	==	FALSE)
		s_strItemName.Insert("<c:LTCYAN>DX<n> ");
	if	(isUltimateItem() && lpBasicItem->m_bf1IsUltimate	==	FALSE)
		s_strItemName.Insert("<c:LTCYAN>Ultimate<n> ");

	if(lpBasicItem->m_bf1IsRingOfInfinity && getCarvingLevel())
	{
		s_strItemName.Add("<c:LTGREEN>Lv %d<n>]", getCarvingLevel());
	}

	return	s_strItemName.String;
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
//
//	보석 툴팁
char*
cItem::getJewelTooltip(int _iRemainTime)
{
	char				*strComment;
	int					iIndex	=	0;
	cBasicItem			*lpBasicItem=	getBasicItem();
	BOOL				isSpecial	=	FALSE;

	s_strTooltip.Reset();

	if (m_aPrefix[0].m_wPrefix	!=	0xffff)
	{
		isSpecial	=	TRUE;

		for (iIndex=0;m_aPrefix[iIndex].m_wPrefix!=0xffff && iIndex<dITEM_PREFIX_COUNT;iIndex++)
		{
			strComment	=	g_itemPrefix.getComment(&m_aPrefix[iIndex]);
			s_strTooltip.Add("- %s\n",strComment);
		}
	}

	s_strTooltip.Add("\n");
	s_strTooltip.Add("> %s <c:LTGREEN>%d<n>%s\n",dMSG_REMAIN_TIME,_iRemainTime,dMSG_SECOND);

	return	s_strTooltip.String;
}


BOOL
cItem::isLimitTermItem()
{

	if	(isExpireCoupleRing())
		return TRUE;

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


//
//
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

int
cItem::getExpandInventorySize()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(lpBasicItem	==	NULL)
		return	0;

	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_EXPAND_INVENTORY_BAG)
		return	m_aOption[0];

	return	0;
}


char*
cItem::getMiniPetPouchTooltip()
{
	char				*strComment;
	int					iIndex	=	0;
	cBasicItem			*lpBasicItem=	getBasicItem();
	BOOL				isSpecial	=	FALSE;
	BOOL				bIsWeapon	=	isWeapon(lpBasicItem);
	BOOL				bIsExpireItem=	FALSE;

	s_strTooltip.Reset();
	s_strTooltip.Add("%s\n",getName());

	cBlockLightPocket	*lpPocket	=	(cBlockLightPocket	*)this;

	cMiniPet	miniPet;

	miniPet.unseal(this);

	s_strTooltip.Add("\n");
	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_ITEM_BASIC_INFO);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_LEVEL,lpPocket->m_bf7Level);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_EXP,lpPocket->m_bf16Exp);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_SKILL_POINT,lpPocket->m_bf7SkillPoint);
	s_strTooltip.Add("- %s <c:LTYELLOW>%s<n>\n",dMSG_MINIPET_EVOLUTION_GRADE,g_aMiniPetTypeInfo[lpPocket->m_bf5Type].m_strName);
	//JBC	차광 주머니 씨앗 열매 툴팁	09-01-05
	if (lpPocket->m_bf1isUseAwakenItem100)
		s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_MINI_PET_AWAKEN_ITEM100);
	else if (lpPocket->m_bf1isUseAwakenItem50)
		s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_MINI_PET_AWAKEN_ITEM50);


	{
		UTime	timeExpired;

		timeExpired.m_dwValue	=	0;

		timeExpired.year	=	lpPocket->m_bf5ExpireYear;
		timeExpired.month	=	lpPocket->m_bf4ExpireMonth;
		timeExpired.day		=	lpPocket->m_bf5ExpireDay;
		timeExpired.hour	=	lpPocket->m_bf5ExpireHour;
		timeExpired.minute	=	lpPocket->m_bf6ExpireMinute;
		
		s_strTooltip.Add(dMSG_EXPIRED_DAY_FORM,timeExpired.year+2000,timeExpired.month,timeExpired.day,timeExpired.hour,timeExpired.minute);
		s_strTooltip.Add(dMSG_MINIPET_UNSEAL_EXPIRE_TIME_RULE);
	}

	s_strTooltip.Add("\n");

	{
		s_strTooltip.Add("<c:LTCYAN><%s><n>\n- ",dMSG_SKILL_INFO);

		int	iCount	=	0;

		for(int i=0;i<c_iMiniPetSkillCount;i++)
		{
			cMiniPetAbilityInfo	*lpAbility	=	&miniPet.m_aAbility[i];

			if	(lpAbility->m_bf4Level	==	0)
				continue;

			if	(iCount)
				s_strTooltip.Add(",");

			CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

			s_strTooltip.Add("[%s:Lv %d]",lpSkill->m_strName,lpAbility->m_bf4Level);

			iCount++;
		}

		s_strTooltip.Add("\n\n");
	}
	
	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_MINIPET_GROW_RATE);

	int	iTotal	=	lpPocket->m_iVitalPoint+lpPocket->m_iEnergyPoint+lpPocket->m_iMineralPoint;
	int	iVital	=	0,iEnergy=0,iMineral=0;

	if	(iTotal)
	{
		iVital	=	((LONGLONG)lpPocket->m_iVitalPoint)*100/iTotal;
		iEnergy	=	((LONGLONG)lpPocket->m_iEnergyPoint)*100/iTotal;
		iMineral=	((LONGLONG)lpPocket->m_iMineralPoint)*100/iTotal;
	}

	s_strTooltip.Add("- %s <c:LTYELLOW>%d%s<n>\n",dMSG_MINIPET_BIO,iVital,dMSG_PERCENTAGE);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d%s<n>\n",dMSG_MINIPET_ENERGY,iEnergy,dMSG_PERCENTAGE);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d%s<n>\n",dMSG_MINIPET_MINERAL,iMineral,dMSG_PERCENTAGE);
	s_strTooltip.Add("\n");

	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_COMMENT);

	strComment	=	lpBasicItem->getComment();
	s_strTooltip.Add("- %s\n",strComment);

	return	s_strTooltip.String;
}

char*
cItem::getBreedingRecordBookTooltip()
{	
	char				*strComment;
	int					iIndex	=	0;
	cBasicItem			*lpBasicItem=	getBasicItem();
	BOOL				isSpecial	=	FALSE;
	BOOL				bIsWeapon	=	isWeapon(lpBasicItem);
	BOOL				bIsExpireItem=	FALSE;

	cPetDefine	petInfo;

	{
		cBreedingRecordBookDefine	*lpBook	=	(cBreedingRecordBookDefine	*)(((char *)this)+6);

		if	(lpBook->isAvailInfo(m_dwSerial))
			lpBook->makePetInfo(&petInfo,m_dwSerial);
		else
		{
			memcpy(&petInfo,&m_bCount,sizeof(cPetDefine)-4);
			petInfo.m_dwExperience		=	m_dwSerial;
		}
	}

	s_strTooltip.Reset();

	s_strTooltip.Add("%s\n",getName());

	s_strTooltip.Add("\n");
	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_ITEM_BASIC_INFO);
	char	strName[dPET_NAME_LENGTH+1];
	memcpy(strName,petInfo.m_strName,dPET_NAME_LENGTH);
	strName[dPET_NAME_LENGTH]	=	NULL;

	int	iJobIndex	=	petInfo.getJobIndex()+100;

	s_strTooltip.Add("- %s <c:LTYELLOW>%s<n>\n",dMSG_NAME,strName);
	s_strTooltip.Add("- %s <c:LTYELLOW>%s<n>\n",dMSG_JOB,g_jm.m_aJob[iJobIndex].m_strName);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_LEVEL,petInfo.m_bf10Level);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_EXP,petInfo.m_dwExperience);

	s_strTooltip.Add("\n");

	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_COMMENT);

	strComment	=	lpBasicItem->getComment();
	s_strTooltip.Add("- %s\n",strComment);

	int	iLimitJob		=	(1-lpBasicItem->isAllJobItem()) +	lpBasicItem->isManExclusiveItem() + lpBasicItem->isWomanExclusiveItem();

	s_strTooltip.Add("\n<c:LTCYAN><%s><n>\n",dMSG_USEABLE_JOB);

	if (!lpBasicItem->isAllJobItem())
	{
		for (int i=0;i<dPLAYER_JOB_COUNT;i++)	
			if (lpBasicItem->isEnableJob(i))
				s_strTooltip.Add("- %s\n",g_pstrHeroJobName[i]);
	}

	if (lpBasicItem->isManExclusiveItem())	s_strTooltip.Add("- %s\n",dMSG_MAN_EXCLUSIVE_ITEM);
	if (lpBasicItem->isWomanExclusiveItem())s_strTooltip.Add("- %s\n",dMSG_WOMAN_EXCLUSIVE_ITEM);

	return	s_strTooltip.String;
}

BOOL
cItem::isWeapon(cBasicItem *_lpBasicItem)
{
	if	(!_lpBasicItem)
		return	FALSE;

	BOOL	bIsWeapon	=	FALSE;

	if	(_lpBasicItem->m_wMinDamage>0	||	_lpBasicItem->m_wMaxDamage>0	)
	{
		bIsWeapon	=	TRUE;

		if	(_lpBasicItem->m_wKind	==	eIK_GLOVE)
		{
			bIsWeapon	=	FALSE;

			if	(g_hero.m_wJob	==	dJOB_FIGHTER || g_hero.m_wJob	==	dJOB_WEREWOLF)
				bIsWeapon	=	TRUE;
		}

		if	(_lpBasicItem->m_wKind	==	eIK_BOOTS)
		{
			bIsWeapon	=	FALSE;

			if	(g_hero.m_wJob	==	dJOB_FIGHTER)
				bIsWeapon	=	TRUE;
		}
	}

	return	bIsWeapon;
}

BOOL
cItem::isWeapon()
{
	cBasicItem	*lpBasicItem=	getBasicItem(TRUE);

	return	isWeapon(lpBasicItem);
}

int
cItem::getCoolTime()
{
	return	getCoolTime(getBasicItem(TRUE));
}


int
cItem::getCoolTime(cBasicItem	*_lpBasicItem)
{
	if	(_lpBasicItem->m_wCoolTimeSecond	==	0)
		return	0;

	return	g_hero.m_awItemCoolTime[_lpBasicItem->m_bCoolTimeIndex];
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

char*
cItem::getItemTooltip(int _iPieceIndex,int _iPieceCount,BOOL _bIsWantAfterEquipInfo)
{
	static	char	strItemToolTip[2048];
	
	char*	lpstrToolTip	=	_getItemTooltip(_iPieceIndex,_iPieceCount,_bIsWantAfterEquipInfo);

	if	(!lpstrToolTip)
		return	"";

	strcpy(strItemToolTip,lpstrToolTip);

	return	strItemToolTip;
}
//
//	툴팁을 얻어온다.
char*
cItem::_getItemTooltip(int _iPieceIndex,int _iPieceCount,BOOL _bIsWantAfterEquipInfo)
{
	if(m_dwSerial	==	s_dwGeneatedToolTipSerial)
	{
		if(getCheckSum() ==	s_iGeneatedToolTipCheckSum)
		{
			cBasicItem	*lpBasicItem=	getBasicItem(TRUE);

			if(lpBasicItem && s_iGeneatedToolTipBaseSerial ==  lpBasicItem->m_iSerial && s_strTooltip.Length)
				return s_strTooltip.String;
		}
	}

	//	마지막에 툴팁을 생성한 아이템의 시리얼과 체크섬... 
	s_dwGeneatedToolTipSerial	=	m_dwSerial;
	s_iGeneatedToolTipCheckSum	=	getCheckSum();

	if	(isBreedingRecordBook())
		return	getBreedingRecordBookTooltip();

	if	(checkFirstEffect(eIE_MINIPET_POUCH))
		return	getMiniPetPouchTooltip();

	s_strTooltip.Reset();

	cPieceItem	*lpPieceItem=	getPieceItem();
	cItemSet	*lpItemSet	=	getItemSet();
	cSetItem	*lpSetItem	=	getSetItem();
	cBasicItem	*lpBasicItem=	getBasicItem(TRUE);
	char		strText[1024];

	if	(!lpSetItem)
		lpItemSet	=	NULL;

	if	(isUnknownItem()	==	FALSE)
	{
		if	(lpPieceItem)
		{
			cItem		item;

			if	(lpPieceItem->generateItem(&item))
			{
				cSpecialItem	*lpSPItem		=	item.getSpecialItem();
				cSpecialItem	*lpSrcSPItem	=	getSpecialItem();

				if	(lpSPItem	&&	lpSrcSPItem)
				{
					lpSPItem->m_bf1IsSetItem				=	lpSrcSPItem->m_bf1IsSetItem;
					lpSPItem->m_bf1IsReversionItem			=	lpSrcSPItem->m_bf1IsReversionItem;
					lpSPItem->m_bf1IsReversionItemWhenEquip	=	lpSrcSPItem->m_bf1IsReversionItemWhenEquip;
					lpSPItem->m_isBroken					=	FALSE;
				}

				return	item.getItemTooltip(m_aOption[0]+1,lpPieceItem->m_wPieceCount,_bIsWantAfterEquipInfo);
			}
		}
	}
	else
	{
		if	(lpPieceItem	==	NULL)
		{
			s_strTooltip.Add("- %s\n\n- <c:LTYELLOW>%s<n>\n",dMSG_UNKNOWN_ITEM,dMSG_UNKNOWN_ITEM_COMMENT);

			return	s_strTooltip.String;
		}
	}

	if	(!lpBasicItem)
		return	"incorrect item data";

	s_iGeneatedToolTipBaseSerial	=	lpBasicItem->m_iSerial;
	
	char	*strComment;
	int		iIndex	=	0;
	BOOL	isSpecial	=	FALSE;
	BOOL	bIsWeapon	=	isWeapon(lpBasicItem);
	BOOL	bIsExpireItem=	FALSE;
	int		iEfficiency	=	100;
	int		iCoolTime	=	getCoolTime(lpBasicItem);
	cItem	item;

	item.copy(this);

	if (_iPieceIndex	!=	-1)
	{
		s_strTooltip.Add("%s\n\n",getName());
		s_strTooltip.Add("<c:LTPINK>%s No. %d<n>\n",dMSG_ITEM_PIECE,_iPieceIndex);
		s_strTooltip.Add(dMSG_PIECE_ITEM_MERGE_CONDITION_FORM,_iPieceCount);
		
//		s_strTooltip.Add("<c:LTPINK>%s No. %d<n> [%d/%d]\n",dMSG_ITEM_PIECE,_iPieceIndex,_iPieceIndex,_iPieceCount);
	}
	else
	if	(lpItemSet	&&	lpSetItem)
		s_strTooltip.Add("<c:LTGREEN>%s<n>(<c:BTBLUE>%s<n>)\n",lpSetItem->m_strName,lpItemSet->m_strName);
	else
		s_strTooltip.Add("%s\n",getName());

	bIsExpireItem	=	isLimitTermItem();

	if	(bIsExpireItem || bIsWeapon || lpBasicItem->m_aGenerateData[0].m_wEffect	!=	0xffff || 
		isDXItem() || isUltimateItem() || isMagicCarpetEmblem()	||	iCoolTime)
	{
		s_strTooltip.Add("\n");
		s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_ITEM_BASIC_INFO);

		if	(isCanNotTradeItem())
			s_strTooltip.Add("<c:LTPINK>- %s<n>\n",dMSG_IS_CAN_NOT_TRADE_ITEM);
		else
		if	(isReversionItemWhenEauip())
			s_strTooltip.Add("<c:LTPINK>- %s<n>\n",dMSG_IS_REVERSION_WHEN_EQUIP);
		
		if	(iCoolTime)
			s_strTooltip.Add("- %s <c:LTYELLOW>%d%s<n>\n",dMSG_COOL_TIME,iCoolTime,dMSG_SECOND);

		if	(lpBasicItem->m_bf3WearLimit)
		{		//09.08.12
			int iCount = g_hero.getSameEquipItemCount(this);

			s_strTooltip.Add("- <c:LTPINK>%s(<n><c:LTYELLOW>%d/%d<n><c:LTPINK>)<n>\n", dMSG_WEAR_LIMIT,iCount, lpBasicItem->m_bf3WearLimit);
		}

		if	(isMagicCarpetEmblem())
		{
			cMagicCarpetEmblemDefine	*lpEmblem	=	(cMagicCarpetEmblemDefine	*)this;

			switch(lpEmblem->m_aOption[0])
			{
				case	eMCS_NORMAL		:
					s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_MC_NON_ACTIVE_EMBLEM);
					break;
				case	eMCS_WILD_CAT	:
					s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_MC_WILD_CAT_ACTIVATED);
					break;
				case	eMCS_TURTLE		:
					s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_MC_TURTLE_ACTIVATED);
					break;
				case	eMCS_SQUIRREL	:
					s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_MC_SQUIRREL_ACTIVATED);
					break;
				case	eMCS_DRAKE		:
					s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_MC_DRAKE_ACTIVATED);
					break;
				case	eMCS_RED_DRAKE		:
					s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_MC_RED_DRAKE_ACTIVATED);
					break;
				case	eMCS_GEAR		:
					s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_MC_GEAR_ACTIVATED);
					break;
			}

			if	(lpEmblem->m_bf1IsTurtleCarpet)
				s_strTooltip.Add("- %s\n",dMSG_MC_SUMMON_TURTLE_COMMENT);
			if	(lpEmblem->m_bf1IsSquirrelCarpet)
				s_strTooltip.Add("- %s\n",dMSG_MC_SUMMON_SQUIRREL_COMMENT);
			if	(lpEmblem->m_bf1IsDrakeCarpet)
				s_strTooltip.Add("- %s\n",dMSG_MC_SUMMON_DRAKE_COMMENT);
			if	(lpEmblem->m_bf1IsRedDrakeCarpet)
				s_strTooltip.Add("- %s\n",dMSG_MC_SUMMON_RED_DRAKE_COMMENT);
			if	(lpEmblem->m_bf1IsGearCarpet)
				s_strTooltip.Add("- %s\n",dMSG_MC_SUMMON_GEAR_COMMENT);
			if	(lpEmblem->m_bf1IsWildCat)
				s_strTooltip.Add("- %s\n",dMSG_MC_SUMMON_WILD_CAT_COMMENT);
		}

		if	(bIsExpireItem)
		{
			UTime	timeExpired;

			timeExpired.m_dwValue	=	0;

			if	(lpBasicItem->m_wExpireYear >= 2000)
			{
				timeExpired.year	=	lpBasicItem->m_wExpireYear-2000;
				timeExpired.month	=	lpBasicItem->m_wExpireMonth;
				timeExpired.day		=	lpBasicItem->m_wExpireDay;
				timeExpired.hour	=	lpBasicItem->m_wExpireHour;
				timeExpired.minute	=	0;
				timeExpired.second	=	0;
			}

			if	(m_year)
			{
				UTime	time;

				time.year	=	DecodeItemExpireYearOffset(m_year,CGamePlay::s_currentTime.m_wYear);
				time.month	=	m_month;
				time.day	=	m_day;
				time.hour	=	m_hour;
				time.minute	=	m_minute;
				time.second	=	0;

				if	(timeExpired.m_dwValue	==	0)
					timeExpired.m_dwValue	=	time.m_dwValue;
				else
				{
					if (time.m_dwValue	<	timeExpired.m_dwValue)
						timeExpired.m_dwValue	=	time.m_dwValue;
				}
			}
			if	(isExpireCoupleRing())
			{
				timeExpired.year	=	((cCoupleRingDefine*)this)->m_year;
				timeExpired.month	=	((cCoupleRingDefine*)this)->m_month;
				timeExpired.day		=	((cCoupleRingDefine*)this)->m_day;
				timeExpired.hour	=	0;
				timeExpired.minute	=	0;
				timeExpired.second	=	0;
			}

			if	(timeExpired.year)
				s_strTooltip.Add(dMSG_EXPIRED_DAY_FORM,timeExpired.year+2000,timeExpired.month,timeExpired.day,timeExpired.hour,timeExpired.minute);
		}

		if	(isExtraItem())
		{
			cExtraItemDefine*	lpExtraInfo	=	(cExtraItemDefine	*)this;

			if	(lpBasicItem->m_wExtraGrade	!=	0xffff)
				s_strTooltip.Add("- <c:LTYELLOW>%s<n> %s\n",g_pstrExtraItemGrade[lpBasicItem->m_wExtraGrade],dMSG_EXTRA_ITEM);
			
			if	(lpBasicItem->isChargeExtraItem()	==	FALSE)
			{
				int	iRemainMana	=	lpExtraInfo->getRemainTimeEfficency();

				lpExtraInfo->updateCurrentEfficency();

				if	(lpExtraInfo->m_sCurrentEfficiency	>=	100)
					s_strTooltip.Add("- %s <c:LTYELLOW>%d%s<n> [%s <c:LTGREEN>%d%s<n>]\n",
										dMSG_CHARGE_MANA_POWER		,iRemainMana,dMSG_PERCENTAGE,
										dMSG_EXTRA_ITEM_EFFICIENCY	,lpExtraInfo->m_sCurrentEfficiency,dMSG_PERCENTAGE);
				else
				if	(lpExtraInfo->m_sCurrentEfficiency	<	0)
					s_strTooltip.Add("- %s <c:RED>%d%s<n> [%s <c:RED>- <n>]\n",
										dMSG_CHARGE_MANA_POWER		,iRemainMana,dMSG_PERCENTAGE,
										dMSG_EXTRA_ITEM_EFFICIENCY);
				else
					s_strTooltip.Add("- %s <c:ORANGE>%d%s<n> [%s <c:ORANGE>%d%s<n>]\n",
										dMSG_CHARGE_MANA_POWER		,iRemainMana,dMSG_PERCENTAGE,
										dMSG_EXTRA_ITEM_EFFICIENCY	,lpExtraInfo->m_sCurrentEfficiency,dMSG_PERCENTAGE);

				iEfficiency	=	lpExtraInfo->m_sCurrentEfficiency;

				if	(lpExtraInfo->m_sCurrentEfficiency	<	0)
				{
					cItem::GenerateItemByBaseItem(&item,lpBasicItem->m_wBaseItem,1,100);
					lpBasicItem	=	item.getBasicItem();
				}
			}
		}

		if	(isBreakdownAbleItem() && lpBasicItem->m_attr.isDurabilityToUseCount == FALSE)
		{
			int	iDurability	=	m_bDurability*100/lpBasicItem->getDurability();

			if	(iDurability	!=	0)
				s_strTooltip.Add("- %s <c:LTYELLOW>%d%s<n>\n",dMSG_DURABILITY,iDurability,dMSG_PERCENTAGE);
			else
				s_strTooltip.Add("- <c:LTRED>%s<n>\n",dMSG_BROKEN_ITEM_REQUIRE_REAPIR);
		}

		if	(bIsWeapon)
		{
			s_strTooltip.Add("- %s<c:LTYELLOW> %d~%d<n>",dMSG_ATTACK_POWER,lpBasicItem->m_wMinDamage,lpBasicItem->m_wMaxDamage);

			if	(lpBasicItem->m_wSpeed)
				s_strTooltip.Add("(<c:LTYELLOW>%d.%.2d<n>%s)\n",lpBasicItem->m_wSpeed/100,lpBasicItem->m_wSpeed%100,dMSG_SECOND);
			else
				s_strTooltip.Add("\n");

			if	(lpBasicItem->m_wRange)	s_strTooltip.Add("- %s<c:LTYELLOW> %d<n>\n",dMSG_ATTACK_RANGE,lpBasicItem->m_wRange);
		}

		if	(lpBasicItem->m_attr.isBeginnerItem)
			s_strTooltip.Add("- <c:LTGREEN>%s<n>\n",dMSG_BEGINNER_ITEM_COMMENT);

		if	(isUnknownItem())
			s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_UNKNOWN_ITEM_COMMENT);

		while(1 && iIndex < dGENERATE_ITEM_DATA_COUNT)
		{
			if	(lpBasicItem->m_aGenerateData[iIndex].m_wEffect	==	0xffff)
				break;

			strComment	=	g_itemGenerateDataDecoder.getComment(&lpBasicItem->m_aGenerateData[iIndex],this , lpBasicItem);

			if	(strComment)
			{
				int	iEffect		=	lpBasicItem->m_aGenerateData[iIndex].m_wEffect;
// 
// 				if	(iEffect	==	eIE_NAME_STONE)
// 				{
// 					cNameStone	*lpNameStone	=	(cNameStone	*)this;
// 
// 					lpNameStone	=	lpNameStone;
// 
// 					char*	lpstrWorldName	=	CGame::GetWorldName(lpNameStone->m_wTargetWorld);
// 
// 					if	(!lpstrWorldName)
// 						lpstrWorldName	=	"unknown world";
// 
// 					sprintf(strText,"서버명 <c:LTYELLOW>%s<n>\n- 캐릭터명 <c:LTYELLOW>%s<n>",lpstrWorldName,lpNameStone->m_strName);
// 
// 					strComment	=	strText;
// 				}
// 				else
				if	(iEffect	==	eIE_TIME_STAMP)
				{
					sprintf(strText,dMSG_YMDHM_FORM,DecodeItemExpireFullYear(m_year,CGamePlay::s_currentTime.m_wYear),m_month,m_day,m_hour,m_minute);

					strComment	=	strText;
				}
				else
				if	(iEffect	==	eIE_BOOK)
				{
					int	iPercentage	=	getCompleteBookPencentage();

					sprintf(strText,dMSG_COMPLETE_PERCENTAGE_FORM,iPercentage);

					strComment	=	strText;
				}
				else
				if	(iEffect	==	eIE_ADD_PAGE)
				{
					cBookPageItemDefine	*lpPage	=	getAddPageToBookItem();

					int		iBeginPage			=	g_book.getPageInBook(lpPage->m_wBookIndex,lpPage->m_wCategory,lpPage->m_wBeginPage)+1;
					int		iEndPage			=	g_book.getPageInBook(lpPage->m_wBookIndex,lpPage->m_wCategory,lpPage->m_wEndPage)+1;

					if	(iBeginPage	==	iEndPage)
						sprintf(strText,"%s <c:LTYELLOW>%d<n>",dMSG_PAGE,iBeginPage);
					else
						sprintf(strText,"%s <c:LTYELLOW>%d~%d<n>",dMSG_PAGE,iBeginPage,iEndPage);

					strComment	=	strText;
				}
				else
				if	(iEffect	==	eIE_PUT_VERTICAL_BANNER)
				{
					sprintf(strText,dMSG_VERTICAL_BANNER_FORM,g_lpstrBannerType[m_aOption[0]]);

					strComment	=	strText;
				}
				else
				if	(iEffect	==	eIE_PUT_HORIZON_BANNER)
				{
					sprintf(strText,dMSG_HORIZON_BANNER_FORM,g_lpstrBannerType[m_aOption[0]]);

					strComment	=	strText;
				}
				else
				if	(iEffect	==	eIE_JUMP_FIELD_USE_WORLD_MAP)
				{
					if	(g_hero.m_wRemainSecondForUseGateGlove)
					{
						sprintf(strText,dMSG_REMAIN_TIME_FOR_USE_GATE_GLOVE_FORM,g_hero.m_wRemainSecondForUseGateGlove);

						strComment	=	strText;
					}
				}
				else
				if	(iEffect	==	eIE_COUPLE_RING)
				{
					cCoupleRingDefine	*lpCoupleRing	=	getCoupleRing();

					sprintf(strText,dMSG_COUPLE_RING_COMMENT_FORM,lpCoupleRing->getCoupleName());

					strComment	=	strText;
				}
				else
				if	(iEffect	==	eIE_INCREASE_SELECT_POWER)
				{
					sprintf(strText,dMSG_ISPI_COMMENT_FORM,g_pstrEquipUseLimitContents[m_aOption[1]+1],m_aOption[0]);

					strComment	=	strText;
				}
				else
				if	(STRICMP(strComment,"isWayPoint")==0)
				{
					int		iWayPointIndex	=	item.m_aOption[0];
					int		iField			=	g_hero.m_aWayPoint[iWayPointIndex].m_bf12Field;
					int		iX				=	g_hero.m_aWayPoint[iWayPointIndex].m_bf10XPos;
					int		iY				=	g_hero.m_aWayPoint[iWayPointIndex].m_bf10YPos;

					strcpy(strText,dMSG_NOT_SETTING_WAY_POINT);

					if	(g_iWorldServerType	!=	eSERVER_TYPE_GVG)
					{
						if (iField	<	dINSTANCE_FIELD_FLAG)
						{
							char	*lpstrFieldName	=	cMAP::GetFieldName(iField);

							if (lpstrFieldName)
								sprintf(strText,dMSG_WAY_POINT_INFO_FORM,lpstrFieldName,iX,iY);
						}
					}
					
					strComment	=	strText;
				}
				else if (STRICMP(strComment,"isItemPiece")==0)
					strComment	=	NULL;
				else if	(iEffect	==	eIE_INCREASE_FIX_SELECT_POWER)
				{	//	 아이템 옵션 선택
					if(m_aOption[1] != 0)
					{
						sprintf(strText,dMSG_ISPI_COMMENT_FORM,g_pstrEquipUseLimitContents[m_aOption[1]],m_aOption[0]);
						
						strComment	=	strText;
					}
				}
				
				if (strComment)
					s_strTooltip.Add("- %s\n",strComment);
			}

			iIndex++;
		}

		if	(isPrincessWeapon())
		{
			cPrincessWeaponDefine	*lpWeapon	=	getPrincessWeapon();

			if	(lpWeapon->m_bFireDamage)
				s_strTooltip.Add(dMSG_PTW_FIRE_DAMAGE_FORM	,lpWeapon->m_bFireDamage);
			if	(lpWeapon->m_bWaterDamage)
				s_strTooltip.Add(dMSG_PTW_WATER_DAMAGE_FORM	,lpWeapon->m_bWaterDamage);
			if	(lpWeapon->m_bWindDamage)
				s_strTooltip.Add(dMSG_PTW_WIND_DAMAGE_FORM	,lpWeapon->m_bWindDamage);
			if	(lpWeapon->m_bEarthDamage)
				s_strTooltip.Add(dMSG_PTW_EARTH_DAMAGE_FORM	,lpWeapon->m_bEarthDamage);
			if	(lpWeapon->m_bLightDamage)
				s_strTooltip.Add(dMSG_PTW_LIGHT_DAMAGE_FORM	,lpWeapon->m_bLightDamage);
			if	(lpWeapon->m_bDarkDamage)
				s_strTooltip.Add(dMSG_PTW_DARK_DAMAGE_FORM	,lpWeapon->m_bDarkDamage);

			if	(lpWeapon->m_wAddDamagePercentage)
				s_strTooltip.Add(dMSG_PTW_INCRASE_DAMAGE_PERCENTAGE_FORM	,lpWeapon->m_wAddDamagePercentage);
		}

		if	(g_hero.iMiniPet()->getMiniPetCount())
		{
			int		iExp	=	getExpForMiniPet();

			if	(iExp)
			{
				s_strTooltip.Add("- %s <c:LTYELLOW>",dMSG_MINIPET_FEED_TYPE);	//텍스트
				int		iFeedType=	getMiniPetFeedType();

				switch(iFeedType)
				{
					case	nsMiniPetStyle::Bio		:
						s_strTooltip.Add(dMSG_MINIPET_FEED_TYPE_BIO);	//텍스트
						break;
					case	nsMiniPetStyle::Energy	:
						s_strTooltip.Add(dMSG_MINIPET_FEED_TYPE_ENERGY);	//텍스트
						break;
					case	nsMiniPetStyle::Mineral	:
						s_strTooltip.Add(dMSG_MINIPET_FEED_TYPE_MINERAL);	//텍스트
						break;
				}
				s_strTooltip.Add("<n>\n");	//텍스트
			}
		}

	}

	if	(iEfficiency	>	0)
		if	(lpBasicItem->m_aUniqueData[0].m_wEffect	!=	0xffff)
		{
			isSpecial	=	TRUE;
			cUniqueData	uniqueData;

			for (iIndex=0;lpBasicItem->m_aUniqueData[iIndex].m_wEffect!=0xffff && iIndex<dITEM_UNIQUE_DATA_COUNT;iIndex++)
			{

				if(lpBasicItem->m_aUniqueData[iIndex].m_wEffect == eIP_BUFF_SKILL_CASTING)
					continue;
				memcpy(&uniqueData,&lpBasicItem->m_aUniqueData[iIndex],sizeof(cUniqueData));

				for (int i=0;i<dITEM_UNIQUE_DATA_VALUE_COUNT;i++)
					uniqueData.m_aValue[i]	=	uniqueData.m_aValue[i]*iEfficiency/100;
				
				strComment	=	g_itemPrefix.getComment(&uniqueData);
				s_strTooltip.Add("- %s\n",strComment);
			}
		}

	if	(isExceptionItem() == FALSE && m_aPrefix[0].m_wPrefix	!=	0xffff)
	{
		isSpecial	=	TRUE;

		for (iIndex=0;m_aPrefix[iIndex].m_wPrefix!=0xffff && iIndex<dITEM_PREFIX_COUNT;iIndex++)
		{
			strComment	=	g_itemPrefix.getComment(&m_aPrefix[iIndex]);
			s_strTooltip.Add("- %s\n",strComment);
		}
	}

	if	(!isInfinityBullet() && (lpBasicItem->m_wKind == eIK_ARROW || lpBasicItem->m_wKind == eIK_BULLET || lpBasicItem->m_wKind == eIK_KNIFE))
		s_strTooltip.Add("- <c:LTPINK>%s<n>\n",dMSG_ITS_NOT_INFINITY_BULLET);

	s_strTooltip.Add("\n");

	if	(lpItemSet)
	{
		int	iSetItemCount	=	g_hero.getEquippedSetItemCount(lpItemSet->m_wSerial),i;
		int	iSetEffectCount	=	lpItemSet->getSetEffectCount(iSetItemCount);

		s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_SET_ITEM_INFO);

		for	(i=0;i<lpItemSet->getItemCount();i++)
		{
			if	(lpItemSet->m_aItem[i].m_wBaseItem	==	0xffff)
				break;

			char	*lpstrColor;

			if	(g_hero.getEquippedSetItem(lpItemSet->m_wSerial,i))
				lpstrColor	=	"<c:LTGREEN>";
			else
			if	(g_hero.getSetItemInInventory(lpItemSet->m_wSerial,i))
				lpstrColor	=	"<c:DKGREEN>";
			else
				lpstrColor	=	"<c:GRAY>";

			s_strTooltip.Add("  %s%s<n>\n",lpstrColor,lpItemSet->m_aItem[i].m_strName);
		}

		for	(i=0;i<iSetEffectCount;i++)
		{
			cUniqueData	*lpEffect	=	lpItemSet->getSetEffect(iSetItemCount,i);

			strComment				=	g_itemPrefix.getComment(lpEffect);

			s_strTooltip.Add("- %s\n",strComment);
		}

		s_strTooltip.Add("\n");
	}

	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_COMMENT);

	if	(lpItemSet)
	{
		s_strTooltip.Add("- %s\n",lpItemSet->getComment());
		s_strTooltip.Add("- %s\n",lpSetItem->getComment());
	}
	else
	{
		CQuestItem	*lpQuest	=	getAssignedQuest();

		if	(lpQuest)
			s_strTooltip.Add("-<c:CTPURPLE> %s<n> %s\n",lpQuest->m_strName,dMSG_QUEST_ITEM);

		if	(isBook())
		{
			char	*lpstrBookName		=	getBookName();

			strComment	=	g_book.getBookItemComment(getBook()->m_wBookIndex);
		}
		else
			strComment	=	lpBasicItem->getComment();

		s_strTooltip.Add("- %s\n",strComment);
	}

	if	(isExceptionItem()	==	FALSE)
	{
		if	(m_isBroken)
			s_strTooltip.Add("- %s\n",dMSG_BROKEN_ITEM_COMMENT);
		if	(m_bf1IsSpecialItem	==	FALSE	&&	m_isCanNotAttachPrefixItem && getPrefixCount() < dITEM_PREFIX_COUNT)
			s_strTooltip.Add("- %s\n",dMSG_CAN_NOT_IMPROVE_ITEM_COMMENT);
	}

	int	iRequireLevel	=	getRequireLevel();

//	아이템 착용 사용 제한
	int	iRequireValue	=	lpBasicItem->m_wRequireLevel+lpBasicItem->m_wRequireStrength+lpBasicItem->m_wRequireDexterity+iRequireLevel+
							lpBasicItem->m_wRequireConstitution+lpBasicItem->m_wRequireWisdom+lpBasicItem->m_wRequireIntelligence+lpBasicItem->m_wRequireCharisma+lpBasicItem->m_wRequireLuck + lpBasicItem->m_wRequireMaxLevel;

	if	(iRequireValue	==	0	&&	lpBasicItem->m_wEquipUseLimitContents	==	0xffff	&&	isLimitJobItem()	==	FALSE)
		return	s_strTooltip.String;

	BOOL	bIsFreeUseItem	=	FALSE;

	if	(getSpecialItem())
		 bIsFreeUseItem	=	getSpecialItem()->m_bf1IsFreeUseItem;

	if	((iRequireValue	>	0 || lpBasicItem->m_wEquipUseLimitContents	!=	0xffff) && bIsFreeUseItem == FALSE)
	{
		s_strTooltip.Add("\n<c:LTCYAN><%s><n>\n",dMSG_REQUIRE_ABILITY);

		char*	lpstrColor;

		if	(lpBasicItem->m_wRequireAllignment)
			s_strTooltip.Add("- %s\n",g_pstrEnableAllignment[lpBasicItem->m_wRequireAllignment]);

		if	(iRequireLevel)
		{
			//08-09-08	툴팁상의 필요레벨에 벳지와 넷카페 유저 특전을 포함시킨 값을 사용하게 수정. JBC
			int iLevel	= g_hero.getLevel();//getLevelIncludeBadge();
			int iLimitEquipmentLevelOverPoint = g_hero.m_wLimitEquipmentLevelOverPoint;
#ifdef _FOR_JAPAN						//넷카페 유저라면 레벨제한 40감소!! 툴팁에서.
			if(g_hero.m_bf1IsNetCafeUser)
				iLimitEquipmentLevelOverPoint	+=	40;
#endif
			if	(!isCanUseLevel(iLevel,iLimitEquipmentLevelOverPoint))
				lpstrColor	=	"<c:LTRED>";
			else
				lpstrColor	=	"";
			if(lpBasicItem->m_wRequireMaxLevel)
				s_strTooltip.Add("- %s%s %d ~ %d<n>\n",lpstrColor,dMSG_LEVEL,iRequireLevel, lpBasicItem->m_wRequireMaxLevel);
			else
				s_strTooltip.Add("- %s%s %d<n>\n",lpstrColor,dMSG_LEVEL,iRequireLevel);
		}

		if	(lpBasicItem->m_wRequireStrength)
		{
			if	(g_hero.getStrength()	<	lpBasicItem->m_wRequireStrength)
				lpstrColor	=	"<c:LTRED>";
			else
				lpstrColor	=	"";

			s_strTooltip.Add("- %s%s %d<n>\n",lpstrColor,dMSG_STRENGTH,lpBasicItem->m_wRequireStrength);
		}
		if	(lpBasicItem->m_wRequireDexterity)
		{
			if	(g_hero.getAgility()	<	lpBasicItem->m_wRequireDexterity)
				lpstrColor	=	"<c:LTRED>";
			else
				lpstrColor	=	"";

			s_strTooltip.Add("- %s%s %d<n>\n",lpstrColor,dMSG_AGILITY,lpBasicItem->m_wRequireDexterity);
		}
		if	(lpBasicItem->m_wRequireConstitution)
		{
			if	(g_hero.getConstitution()	<	lpBasicItem->m_wRequireConstitution)
				lpstrColor	=	"<c:LTRED>";
			else
				lpstrColor	=	"";

			s_strTooltip.Add("- %s%s %d<n>\n",lpstrColor,dMSG_HEALTH,lpBasicItem->m_wRequireConstitution);
		}
		if	(lpBasicItem->m_wRequireWisdom)	
		{
			if	(g_hero.getWisdom()	<	lpBasicItem->m_wRequireWisdom)
				lpstrColor	=	"<c:LTRED>";
			else
				lpstrColor	=	"";

			s_strTooltip.Add("- %s%s %d<n>\n",lpstrColor,dMSG_WISDOM,lpBasicItem->m_wRequireWisdom);
		}
		if	(lpBasicItem->m_wRequireIntelligence)
		{
			if	(g_hero.getIntelligence()	<	lpBasicItem->m_wRequireIntelligence)
				lpstrColor	=	"<c:LTRED>";
			else
				lpstrColor	=	"";

			s_strTooltip.Add("- %s%s %d<n>\n",lpstrColor,dMSG_INTELLIGENCE,lpBasicItem->m_wRequireIntelligence);
		}
		if	(lpBasicItem->m_wRequireCharisma)
		{
			if	(g_hero.getCharisma()	<	lpBasicItem->m_wRequireCharisma)
				lpstrColor	=	"<c:LTRED>";
			else
				lpstrColor	=	"";

			s_strTooltip.Add("- %s%s %d<n>\n",lpstrColor,dMSG_CHARISMA,lpBasicItem->m_wRequireCharisma);
		}
		if	(lpBasicItem->m_wRequireLuck)
		{
			if	(g_hero.getLuck()	<	lpBasicItem->m_wRequireLuck)
				lpstrColor	=	"<c:LTRED>";
			else
				lpstrColor	=	"";

			s_strTooltip.Add("- %s%s %d<n>\n",lpstrColor,dMSG_LUCK,lpBasicItem->m_wRequireLuck);
		}

		if	(lpBasicItem->m_wEquipUseLimitContents	!=	0xffff)
		{
			int	iRequireValue,iCurrentValue;

			switch(lpBasicItem->m_wEquipUseLimitContents)
			{
				case	0	:	//	"레벨",
					iCurrentValue	=	g_hero.getLevel();
					break;
				case	1	:	//	"힘",
					iCurrentValue	=	g_hero.getStrength();
					break;
				case	2	:	//	"민첩성",
					iCurrentValue	=	g_hero.getAgility();
					break;
				case	3	:	//	"건강",
					iCurrentValue	=	g_hero.getConstitution();
					break;
				case	4	:	//	"카리스마",
					iCurrentValue	=	g_hero.getCharisma();
					break;
				case	5	:	//	"지식",
					iCurrentValue	=	g_hero.getIntelligence();
					break;
				case	6	:	//	"지혜",
					iCurrentValue	=	g_hero.getWisdom();
					break;
				case	7	:	//	"행운",
					iCurrentValue	=	g_hero.getLuck();
					break;
			}

			switch(lpBasicItem->m_wEquipUseLimitMethod)
			{
				case	0xffff	:
					iRequireValue	=	lpBasicItem->m_wEquipUseLimitValue;
					break;
				case	0	:
					iRequireValue	=	item.m_aOption[0]*lpBasicItem->m_wEquipUseLimitValue;
					break;
				case	1	:
					iRequireValue	=	item.m_aOption[1]*lpBasicItem->m_wEquipUseLimitValue;
					break;
				case	2	:
					iRequireValue	=	item.m_aOption[0]/lpBasicItem->m_wEquipUseLimitValue;
					break;
				case	3	:
					iRequireValue	=	item.m_aOption[1]/lpBasicItem->m_wEquipUseLimitValue;
					break;
			}

			if	(iCurrentValue	<	iRequireValue)
				lpstrColor	=	"<c:LTRED>";
			else
				lpstrColor	=	"";

			s_strTooltip.Add("- %s%s %d<n>\n",lpstrColor,g_pstrEquipUseLimitContents[lpBasicItem->m_wEquipUseLimitContents],iRequireValue);
		}
	}

	if	(isLimitJobItem()	==	TRUE)
	{	//	 착용 제한이 있따..!
		s_strTooltip.Add("\n<c:LTCYAN><%s><n>\n",dMSG_USEABLE_JOB);		
		if	(!isAllJobItem())
		{// 모든 직업만 체크
			for (int i=0;i<dPLAYER_JOB_COUNT;i++)	// 직업 출력~~ 신규케릭까지!
				if (isEnableJob(i))
				{
					if	(g_hero.m_wJob	!=	i)
						s_strTooltip.Add("- <c:LTRED>%s<n>\n",g_pstrHeroJobName[i]);
					else
						s_strTooltip.Add("- %s\n",g_pstrHeroJobName[i]);
				}
		}
		else
		{// 모든 직업이 전부 착용이 가능하면 성별 체크를 한당.
			if	(isManExclusiveItem())	
			{
				if	(c_aIsMale[g_hero.m_wJob]	==	FALSE)
					s_strTooltip.Add("- <c:LTRED>%s<n>\n",dMSG_MAN_EXCLUSIVE_ITEM);
				else
					s_strTooltip.Add("- %s\n",dMSG_MAN_EXCLUSIVE_ITEM);
			}
			
			if	(isWomanExclusiveItem())
			{
				if	(c_aIsMale[g_hero.m_wJob])
					s_strTooltip.Add("- <c:LTRED>%s<n>\n",dMSG_WOMAN_EXCLUSIVE_ITEM);
				else
					s_strTooltip.Add("- %s\n",dMSG_WOMAN_EXCLUSIVE_ITEM);
			}
		}
	}

	return	s_strTooltip.String;
}
//
//	연관된 퀘스트 얻기
CQuestItem*
cItem::getAssignedQuest()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();
	CQuestItem	*lpQuest		=	NULL;

	if(!lpBasicItem)	return	NULL;

	if (lpBasicItem->m_questIndex	== dNOT_QUEST_ITEM)	return	NULL;

	if (lpBasicItem->m_questKind	==	eQT_NORMAL)	lpQuest	=	g_quest.getQuest(lpBasicItem->m_questIndex);
	if (lpBasicItem->m_questKind	==	eQT_ARBEIT)	lpQuest	=	g_questArbeit.getQuest(lpBasicItem->m_questIndex);

	return	lpQuest;
}

//	무기의 컬러라이즈 효과
int
cItem::getWeaponColorizeEffect()
{
	cBasicItem *lpWeponBasicItem = getBasicItem(TRUE);

	if(!lpWeponBasicItem)
		return 0xffff;

	if	 (m_wBaseItem	==	0xffff)
		return	0xffff;

	int	aEffect[dITEM_PREFIX_COUNT],i,iEffectCount=0,iEffect=0xffff;

	if	(lpWeponBasicItem->m_bCompulsionColorizeEffect)
 		aEffect[iEffectCount++]	=	lpWeponBasicItem->m_bCompulsionColorizeEffect;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)
			break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if (lpPrefix->m_bWeaponEffect	!=	0xff)
			aEffect[iEffectCount++]	=	lpPrefix->m_bWeaponEffect;
	}

	if (iEffectCount	==	0)
		return	0xffff;

	if (iEffectCount	==	1)
		return	aEffect[0];

	for (i=0;i<iEffectCount;i++)
	{
		if (iEffect	==	0xffff)
			iEffect	=	aEffect[i];
		else
		{
			if (aEffect[i]%3 > iEffect%3)
				iEffect	=	aEffect[i];
		}
	}

	return	iEffect;
}



void
cItem::changeToCoupleRing(char *_lpstrTargetName)
{
	m_wBaseItem	=	s_iCoupleRing;
	
	if	(_lpstrTargetName)
		getCoupleRing()->setCoupleName(_lpstrTargetName);

	cBasicItem	*lpBasicItem	=	getBasicItem();

	for (int i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if	(lpBasicItem->m_aGenerateData[i].m_wEffect	!=	eIE_INCREASE_SELECT_POWER)
			continue;

		int	iReferenceIndex,iRange,iMinValue,iMaxValue;

//	옵션 1
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange+1),iMaxValue);
		}

//	옵션 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange+1),iMaxValue);
		}
	}
}

BOOL
cItem::addItem(cItem *_lpItem,int _iLimitCount)	//	아이템 합치기..?
{
	if	(isExtraItem() || isMagicCarpetEmblem() || isExceptionItem())
		return	FALSE;

	if	(_lpItem->m_bCount	<=	0)
		return	FALSE;

	if	(_iLimitCount	==	0xffff)
		_iLimitCount	=	getStackLimit();

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	int	iSpareCount	=	_iLimitCount-m_bCount;

	if	(iSpareCount	<=	0)
		return	FALSE;

	if	(_lpItem->m_bCount	<	iSpareCount)
		iSpareCount	=	_lpItem->m_bCount;

	int	iDurability	=	m_bCount;
	iDurability		*=	m_bDurability;
	iDurability		=	iDurability+iSpareCount*_lpItem->m_bDurability;

	m_bCount		+=	iSpareCount;
	iDurability		/=	m_bCount;
	m_bDurability	=	iDurability;

	_lpItem->decreaseCount(iSpareCount);

	return	TRUE;
}

int
cItem::getCheckSum()	//	체크섬 구하기
{
	if	(m_wBaseItem	==	0xffff)
		return	0;

	if	(isUnknownItem() )
		return	-1;

	int	iSum=	0;

	iSum	+=	m_dwSerial;
	iSum	+=	m_wBaseItem;							//	아이템 인덱스

	if (isExtraItem()	==	FALSE)
	{
		iSum	+=	m_bCount;								//	숫자
		iSum	+=	m_bDurability;							//	내구력
	}

	iSum	+=	m_aOption[0];							//	생성될때 붙은 옵션
	iSum	+=	m_aOption[1];							//	생성될때 붙은 옵션

	for (int iPrefix=0;iPrefix<dITEM_PREFIX_COUNT;iPrefix++)
	{
		iSum	+=	m_aPrefix[iPrefix].m_wPrefix;
		iSum	+=	m_aPrefix[iPrefix].m_aValue[0];
		iSum	+=	m_aPrefix[iPrefix].m_aValue[1];
	}

	iSum	+=	m_isBroken			;	//	부숴진 아이템
	iSum	+=	m_bf1IsSpecialItem	;	//	저주 받은 아이템
	iSum	+=	m_wayPointIndex		;	//	웨이 포인트 인덱스

	iSum	+=	m_isCanNotAttachPrefixItem;	//	더 이상 접두사를 붙일 수 없는 아이템
	iSum	+=	m_year			;	//	유효기간
	iSum	+=	m_month			;
	iSum	+=	m_day			;
	iSum	+=	m_hour			;
	iSum	+=	m_minute		;

	return	iSum;
}

void
cItem::increaseMinute(int _iMinute)
{
	if	(isExceptionItem())
		return;

	if	(getSpecialItem())
		return;

	CTimeInfo	timeInfo;

	timeInfo.m_wYear	=	(WORD)DecodeItemExpireFullYear(m_year,CGamePlay::s_currentTime.m_wYear);
	if	(timeInfo.m_wYear == 0)
		timeInfo.m_wYear	=	CGamePlay::s_currentTime.m_wYear;

	timeInfo.m_wMonth	=	m_month ? (WORD)m_month : CGamePlay::s_currentTime.m_wMonth;
	timeInfo.m_wDay		=	m_day ? (WORD)m_day : CGamePlay::s_currentTime.m_wDay;
	timeInfo.m_wHour	=	(WORD)m_hour;
	timeInfo.m_wMinute	=	(WORD)m_minute;
	timeInfo.m_wSecond	=	0;
	timeInfo.m_wWeekDay	=	0;

	timeInfo.increaseMinute(_iMinute);

	m_year		=	EncodeItemExpireYear(timeInfo.m_wYear);
	m_month		=	timeInfo.m_wMonth;
	m_day		=	timeInfo.m_wDay;
	m_hour		=	timeInfo.m_wHour;
	m_minute	=	timeInfo.m_wMinute;
}

BOOL
cItem::isPremiumItem()
{
	if	(isBreedingRecordBook())
		return	FALSE;

	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(lpBasicItem->m_wKind	==	eIK_PREMIUM_ITEM)
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

	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_FLAKE_OF_DOING_ONE)
		iFeedType	=	nsMiniPetStyle::Bio;
	else
	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_FEATHER_OF_THINKING_ONE)
		iFeedType	=	nsMiniPetStyle::Energy;
	else
	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_TOY_OF_SIN_AND_RIN)
		iFeedType	=	nsMiniPetStyle::Mineral;
	else
	{
//	생체형 성장비율 상승아이템 : 무기류, 움직이는 자의 조각
//	에너지형 성장비율 상승아이템 : 공용 갑옷, 전용 갑옷, 헬멧, 장갑, 벨트, 부츠, 망토, 생각하는 자의 깃털
//	무기질형 성장비율 상승아이템 : 위의 아이템을 제외한 장비아이템(탄환류는 제외), 신과 린의 장난감.

		iFeedType	=	nsMiniPetStyle::Mineral;

		int	aiItemTypeList[2][20]	=
		{
			{
				eIK_EXCLUSIVE_ARMOR				,//"전용갑옷",
				eIK_ONEHANDED_SWORD				,//"한손검",
				eIK_TWOHANDED_SWORD				,//"양손검",
				eIK_STAFF						,//"지팡이",
				eIK_TOOTH						,//"이빨",
				eIK_MACE						,//"메이스",
				eIK_WING						,//"날개",
				eIK_KNIFE						,//"단도",
				eIK_BOW							,//"활",
				eIK_ARROW						,//"화살",
				eIK_SPEAR						,//"창",
				eIK_FLUTE						,//"피리",
				eIK_SLING						,//"슬링",
				eIK_BULLET						,//"탄환",
				eIK_MAGICAL_ROD					,//"마술봉",
				eIK_WHIP						,//"채찍",
				eIK_SCYTHE						,	// 낫.
				eIK_ARM_WEAPON					,	//	암웨폰
				eIK_BOOK						,// "책",
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

	if	(isMiniPetFeed())
	{
		iExp	=	m_aOption[0];
		iExp	=	iExp*255+m_aOption[1];
	}
	else
	{
		if(c_aItemEquipPlace[lpBasicItem->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpBasicItem->m_wKind] == 0xffff)		//  변경..
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

//	아이템을 먹이면 해당 아이템의 착용 레벨만큼의 경험치를 펫이 얻는다.
//	레어 아이템과 유니크 아이템은 원 착용 레벨의 각각 2배, 20배의 경험치를 얻는다.
//	단, 움직이는 자의 조각과 생각하는 자의 깃털, 신과 린의 장난감은 아래에 정해진 양만큼의 경험치를 펫이 얻는다.
	
	return	iExp;
}

//	길드 펫 한테 줄때 경험치
int
cItem::getExpForGuildPet()
{
	cBasicItem	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	0;

		if(c_aItemEquipPlace[lpBasicItem->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpBasicItem->m_wKind] == 0xffff)		//  변경..	
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
//5. 아이템을 먹이면 해당 아이템의 "착용가능 레벨/10" 만큼의 경험치를 얻는다.					
//   ㄱ. 먹일 수 있는 아이템의 종류는 착용 가능한 아이템인 '장비'로 제한한다.(단, 셋트 및 유니크는 불가)					
// ㄴ. 스택되는 단검, 화살, 슬링탄환 등의 아이템의 경험치는 현재 먹일 아이템 "((스택 개수/최대 스택 개수)*착용 제한레벨)/10"으로 별도 계산한다.					
//   ㄷ. Gold 로 아이템을 대신할 수도 있지만 그다지 좋은 선택은 아닐 것이다.					
//        Exp = Gold/1000					
}

//
//	_strItemName이란 이름을 가진 아이템을 찾는다.
int
cItem::GetItemIndexByName(char *_strItemName)
{
	for (int i=0;i<dBASIC_ITEM_COUNT;i++)
	{
		if (g_aBasicItem[i].m_iSerial	>=	dBASIC_ITEM_COUNT	||	g_aBasicItem[i].m_iSerial	<	0	)
			continue;

		if (STRICMP(g_aBasicItem[i].m_strName,_strItemName)	==	0)
			return	i;
	}

	return	0xffff;
}

void
cItem::BuildData()
{
	for (int i=0;i<dBASIC_ITEM_COUNT;i++)
	{
		if	(g_aBasicItem[i].m_iSerial	==	0xffffffff)
			continue;

		if	(g_aBasicItem[i].m_aGenerateData[0].m_wEffect	==	eIE_COUPLE_RING)
			s_iCoupleRing	=	i;

		if	(g_aBasicItem[i].m_aGenerateData[0].m_wEffect	==	eIE_DUEL_BUFF_FIRE)
			g_alpDuelBuffItem[0]	=	&g_aBasicItem[i];
		if	(g_aBasicItem[i].m_aGenerateData[0].m_wEffect	==	eIE_DUEL_BUFF_WATER)
			g_alpDuelBuffItem[1]	=	&g_aBasicItem[i];
		if	(g_aBasicItem[i].m_aGenerateData[0].m_wEffect	==	eIE_DUEL_BUFF_WIND)
			g_alpDuelBuffItem[2]	=	&g_aBasicItem[i];
		if	(g_aBasicItem[i].m_aGenerateData[0].m_wEffect	==	eIE_DUEL_BUFF_EARTH)
			g_alpDuelBuffItem[3]	=	&g_aBasicItem[i];
	}
}

void
cItem::Reset()
{
	memset(g_astrItemPackListName,0,sizeof(g_astrItemPackListName));
}

void
cItem::GenerateItemByBaseItem(cItem *_lpItem,int _iBaseItem,int _iCount,int _iEfficiency)
{
	int		i;

	if	(_iBaseItem	==	0xffff)
		return;

	cBasicItem	*lpBasicItem	=	&g_aBasicItem[_iBaseItem];

	_lpItem->m_wBaseItem		=	_iBaseItem;
	_lpItem->m_bCount			=	_iCount;

	if	(lpBasicItem->m_attr.isDurabilityToUseCount)
		_lpItem->m_bDurability	=	(BYTE)lpBasicItem->getDurability();

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex,iRange,iMinValue,iMaxValue;

//	옵션 1
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			if (_iEfficiency	==	-1)
				_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange+1),iMaxValue);
			else
				_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*_iEfficiency/100,iMaxValue);
		}

//	옵션 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			if (_iEfficiency	==	-1)
				_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange+1),iMaxValue);
			else
				_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*_iEfficiency/100,iMaxValue);
		}

		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_USE_JEWEL)
			_lpItem->m_bDurability	=	_lpItem->m_aOption[iReferenceIndex];
	}

	if	(_lpItem->isInfinityBullet())
		_lpItem->m_bCount	=	1;

	if	(_lpItem->isDXItem())
	{
		int	iDurability			=	random(25)+50;

		_lpItem->m_bDurability	=	lpBasicItem->getDurability()*iDurability/100;
	}

	if	(_lpItem->isUltimateItem())
	{
		int	iDurability			=	random(c_iUltimateItemGenerateDurability)+c_iUltimateItemGenerateDurabilityRange;

		_lpItem->m_bDurability	=	lpBasicItem->getDurability()*iDurability/100;
	}

}

BOOL
CCustomItem::generateItem(cItem *_lpItem)
{
	int		i,j;

	if (m_wBaseItem	==	0xffff)
		return FALSE;

	if (m_bf1IsCustomItem)
	{
		memcpy(_lpItem,this,sizeof(CItemBaseInfo));
		return	TRUE;
	}

	if (m_wBaseItem >= dBASIC_ITEM_COUNT)
		return	FALSE;

	cBasicItem	*lpBasicItem	=	&g_aBasicItem[m_wBaseItem];

	if (lpBasicItem->m_iSerial < 0 || lpBasicItem->m_iSerial >= dBASIC_ITEM_COUNT)
		return	FALSE;

	_lpItem->m_wBaseItem		=	m_wBaseItem;
	_lpItem->m_bCount			=	m_bCount;

	if (lpBasicItem->m_attr.isDurabilityToUseCount)
		m_bDurability	=	(BYTE)lpBasicItem->getDurability();

	if (_lpItem->isItemPack())
		_lpItem->m_aOption[0]	=	(BYTE)m_wGenerateQuality;
	else
	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex,iRange,iMinValue,iMaxValue;

//	옵션 1
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
			if (iReferenceIndex < 0 || iReferenceIndex >= 2)
				continue;
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*m_wGenerateQuality/100,iMaxValue);
		}

//	옵션 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
			if (iReferenceIndex < 0 || iReferenceIndex >= 2)
				continue;
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*m_wGenerateQuality/100,iMaxValue);
		}

		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_USE_JEWEL)
			_lpItem->m_bDurability	=	_lpItem->m_aOption[iReferenceIndex];
	}

	if	(m_wIsAttachRandomPrefix)
	{
//		attachPrefix(_lpItem,m_wDropLevel,TRUE);

		return	TRUE;
	}

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		int	iPrefix	=	m_aPrefix[i].m_wPrefix;

		if (iPrefix	==	0xffff)	break;
		if (iPrefix < 0 || iPrefix >= dMAX_ITEM_PREFIX_COUNT)
			continue;

		_lpItem->m_aPrefix[i].m_wPrefix		=	iPrefix;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

		for (j=0;j<2;j++)
		{
			int	iMinValue,iMaxValue,iRange;
			
			iMinValue	=	lpPrefix->m_aValue[j][0];
			iMaxValue	=	lpPrefix->m_aValue[j][1];
			iMaxValue	=	max(iMaxValue,iMinValue);
			iRange		=	iMaxValue-iMinValue;

			_lpItem->m_aPrefix[i].m_aValue[j]	=	min(iMinValue+iRange*m_aPrefixGenerateQuality[i]/100,iMaxValue);
		}
	}

	if	(m_bf20TermOfValidate)
	{
		_lpItem->m_year		=	EncodeItemExpireYear(CGamePlay::s_currentTime.m_wYear);
		_lpItem->m_month	=	CGamePlay::s_currentTime.m_wMonth;
		_lpItem->m_day		=	CGamePlay::s_currentTime.m_wDay;
		_lpItem->m_hour		=	CGamePlay::s_currentTime.m_wHour;
		_lpItem->m_minute	=	CGamePlay::s_currentTime.m_wMinute;

		_lpItem->increaseMinute(m_bf20TermOfValidate);
	}

	return	TRUE;
}


BOOL
CCustomItemManager::generateItem(int _iItemIndex,cItem *_lpItem)
{
	CCustomItem	*lpCustomItem	=	getItem(_iItemIndex);
	
	if (!lpCustomItem)
		return	FALSE;

	if (!lpCustomItem->generateItem(_lpItem))
		return	FALSE;

	return	TRUE;
}
