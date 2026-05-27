#include "itemSet.h"
#include "CItemPrefix.h"
#include "cProject.h"

cItemSetManager	g_itemSetManager;

char*
cSetItem::getComment()
{
	int		iSize;
	WORD	wSize;

	cFILE	file;

	g_project.setProjectFolder();

	if	(!file.Open(dITEM_FILE_NAME,"rb"))
		return	NULL;

	file.Skip(8);			//	국가정보,아이템 수
	file.Read(&iSize,4);			//	텍스트 데이터가 저장된 위치

	iSize	+=	m_dwCommentAddress;
	
	file.Seek(iSize,SEEK_SET);
	file.Read(&wSize,2);			//	텍스트 데이터가 저장된 위치
	file.Read(cBasicItem::s_strComment,wSize);	//	텍스트 데이터가 저장된 위치
	file.Close();

	SetLogFolder();

	return	cBasicItem::s_strComment;
}

BOOL
cSetItem::generateItem(cItem *_lpItem,int _iItemSet)
{
	int		i,j;

	if (m_wBaseItem	==	0xffff)
		return FALSE;

	if (m_bf1IsCustomItem)
	{
		memcpy(_lpItem,this,sizeof(CItemBaseInfo));

		cSpecialItem	*lpSpecialItem				=	(cSpecialItem	*)_lpItem;

		lpSpecialItem->m_bf1IsSpecialItem			=	TRUE;
		lpSpecialItem->m_bf1IsUnknown				=	FALSE;
		lpSpecialItem->m_bf1IsSetItem				=	TRUE;
		lpSpecialItem->m_bf10EquipLevel				=	0;
		lpSpecialItem->m_bf11SValue					=	_iItemSet;
		lpSpecialItem->m_bf1IsReversionItem			=	FALSE;
		lpSpecialItem->m_bf1IsReversionItemWhenEquip=	FALSE;
		lpSpecialItem->m_isBroken					=	FALSE;

		return	TRUE;
	}

	cBasicItem	*lpBasicItem	=	&g_aBasicItem[m_wBaseItem];

	_lpItem->m_wBaseItem		=	m_wBaseItem;
	_lpItem->m_bCount			=	m_bCount;
	_lpItem->m_bDurability		=	(BYTE)lpBasicItem->getDurability();

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
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue+1;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*m_wGenerateQuality/100,iMaxValue);
		}

//	옵션 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue+1;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*m_wGenerateQuality/100,iMaxValue);
		}

		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_USE_JEWEL)
			_lpItem->m_bDurability	=	_lpItem->m_aOption[iReferenceIndex];
	}

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		int	iPrefix	=	m_aPrefix[i].m_wPrefix;

		if (iPrefix	==	0xffff)
			break;

		_lpItem->m_aPrefix[i].m_wPrefix		=	iPrefix;

		cITEM_PREFIX	*lpPrefix			=	&g_aItemPrefix[iPrefix];

		for (j=0;j<2;j++)
		{
			int	iMinValue,iMaxValue,iRange;
			
			iMinValue	=	lpPrefix->m_aValue[j][0];
			iMaxValue	=	lpPrefix->m_aValue[j][1];
			iMaxValue	=	max(iMaxValue,iMinValue);
			iRange		=	iMaxValue-iMinValue+1;

			_lpItem->m_aPrefix[i].m_aValue[j]	=	min(iMinValue+iRange*m_aPrefixGenerateQuality[i]/100,iMaxValue);
		}
	}

	return	TRUE;
}	

void
cItemSet::reset()
{
	m_wSerial	=	0xffff;
}

int
cItemSet::getItemCount()
{
	int	iCount	=	0;

	for (int i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
	{
		if (m_aItem[i].m_wBaseItem	!=	0xffff)
			iCount	++;
	}

	return	iCount;
}

cSetItem*
cItemSet::getItem(int _iSlot)
{
	return	(cSetItem*)&m_aItem[_iSlot];
}

cUniqueData*	
cItemSet::getSetEffect(int _iSetItemCount,int _iEffectIndex)
{
	if	(_iSetItemCount	<=	0	||	_iSetItemCount	>	dMAX_SET_ITEM_EFFECT_GRADE_COUNT	||
		_iEffectIndex	<	0	||	_iEffectIndex	>=	dMAX_SET_ITEM_EFFECT_COUNT)
		return	NULL;

	if (m_aSetEffect[_iSetItemCount-1][_iEffectIndex].m_wEffect	==	0xffff)
		return	NULL;

	return	&m_aSetEffect[_iSetItemCount-1][_iEffectIndex];
}

int
cItemSet::getSetEffectCount(int _iSetItemCount)
{
	if	(_iSetItemCount	<=	0	||	_iSetItemCount	>	dMAX_SET_ITEM_EFFECT_GRADE_COUNT	)
		return	0;

	int	iCount	=	0;

	for (int i=0;i<dMAX_SET_ITEM_EFFECT_COUNT;i++)
	{
		if (m_aSetEffect[_iSetItemCount-1][i].m_wEffect	!=	0xffff)
			iCount++;
	}

	return	iCount;
}

cSetItem*
cItemSet::getItemByBaseItem(int _iBaseItem)
{
	for (int i=0;i<getItemCount();i++)
		if (_iBaseItem == m_aItem[i].m_wBaseItem)
			return	(cSetItem*)&m_aItem[i];

	return	NULL;
}

char*
cItemSet::getComment()
{
	int		iSize;
	WORD	wSize;

	cFILE	file;

	g_project.setProjectFolder();

	if	(!file.Open(dITEM_FILE_NAME,"rb"))
		return	NULL;

	file.Skip(8);			//	국가정보,아이템 수
	file.Read(&iSize,4);			//	텍스트 데이터가 저장된 위치

	iSize	+=	m_dwCommentAddress;
	
	file.Seek(iSize,SEEK_SET);
	file.Read(&wSize,2);			//	텍스트 데이터가 저장된 위치
	file.Read(cBasicItem::s_strComment,wSize);	//	텍스트 데이터가 저장된 위치
	file.Close();

	SetLogFolder();

	return	cBasicItem::s_strComment;
}

void
cItemSetManager::build()
{
	for (int i=0;i<m_iCount;i++)
		for (int iSetItem=0;iSetItem<dMAX_ITEM_COUNT_IN_SET;iSetItem++)
			m_pItemSet[i].m_aItem[iSetItem].m_pItemComment	=	NULL;
}


