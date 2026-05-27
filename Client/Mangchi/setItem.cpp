#include "setItem.h"

cItemSetManager	g_itemSetManager;

BOOL
cItemSet::isDefault()
{
	if (getItemCount()	==	0	&&	getSetEffectCount()	==	0)
		return	TRUE;

	return	FALSE;
}

BOOL
cItemSet::isSame(cItemSet *_lpGroup, BOOL bIsByteCompare)
{
	if(bIsByteCompare)
	{
		
		BYTE	*lpData1	=	(BYTE	*)_lpGroup;
		BYTE	*lpData2	=	(BYTE	*)this;
		
		for (int i=2;i<sizeof(cItemSet);i++)
		{
			if (lpData1[i]	!=	lpData2[i])
				return	FALSE;
		}
		
		return	TRUE;
	}

	if(m_wCommentSize != _lpGroup->m_wCommentSize)
		return FALSE;

	if(((cItemSetDefine*)this)->isSame( _lpGroup))
		return FALSE;

	
	return TRUE;
}

BOOL
cItemSet::isFullItem()
{
	if (getItemCount()	>=	dMAX_ITEM_COUNT_IN_SET)
		return	TRUE;

	return	FALSE;
}

cSetItem*
cItemSet::getItem(int _iItemIndex)
{
	if	(_iItemIndex	<	0	||	_iItemIndex	>=	dMAX_ITEM_COUNT_IN_SET)
		return	NULL;

	if (m_aItem[_iItemIndex].m_wBaseItem	==	0xffff)
		return	NULL;

	return	(cSetItem*)&m_aItem[_iItemIndex];
}

cUniqueData*	
cItemSet::getSetEffect(int _iSetCount,int _iEffectIndex)
{
	if	(_iSetCount		<	0	||	_iSetCount	>=	dMAX_SET_ITEM_EFFECT_GRADE_COUNT	||
		_iEffectIndex	<	0	||	_iEffectIndex>=	dMAX_SET_ITEM_EFFECT_COUNT)
		return	NULL;

	if (m_aSetEffect[_iSetCount][_iEffectIndex].m_wEffect	==	0xffff)
		return	NULL;

	return	&m_aSetEffect[_iSetCount][_iEffectIndex];
}


BOOL
cItemSet::isAvailSetEffect(int _iSetCount,int _iEffectIndex)
{
	if	(_iSetCount		<	0	||	_iSetCount	>=	dMAX_SET_ITEM_EFFECT_GRADE_COUNT	||
		_iEffectIndex	<	0	||	_iEffectIndex>=	dMAX_SET_ITEM_EFFECT_COUNT)
		return	FALSE;

	if (m_aSetEffect[_iSetCount][_iEffectIndex].m_wEffect	==	0xffff)
		return	FALSE;

	return	TRUE;
}

BOOL
cItemSet::isFullSetEffectCount(int _iSetCount)
{
	if (getSetEffectCount(_iSetCount)	>=	dMAX_SET_ITEM_EFFECT_COUNT)
		return	TRUE;

	return	FALSE;
}

int
cItemSet::getSetEffectCount(int _iSetCount)
{
	int	iCount	=	0;

	if (_iSetCount	==	-1)
	{
		for (int iSetCount=0;iSetCount<dMAX_SET_ITEM_EFFECT_GRADE_COUNT;iSetCount++)
			for (int i=0;i<dMAX_SET_ITEM_EFFECT_COUNT;i++)
			{
				if (m_aSetEffect[iSetCount][i].m_wEffect	!=	0xffff)
					iCount++;
			}
	}
	else
	for (int i=0;i<dMAX_SET_ITEM_EFFECT_COUNT;i++)
	{
		if (m_aSetEffect[_iSetCount][i].m_wEffect	!=	0xffff)
			iCount++;
	}

	return	iCount;
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

void
cItemSet::replaceItem(int _iIndex,cSetItem *_lpItem)
{
	memcpy(&m_aItem[_iIndex],_lpItem,sizeof(cSetItemDefine)-4);

	cSetItem	*lpItem	=	(cSetItem	*)&m_aItem[_iIndex];

	lpItem->setComment(_lpItem->m_pItemComment);
}

BOOL	
cItemSet::addItem(cSetItem *_lpItem)
{
	int	i;

	for (i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
	{
		if (m_aItem[i].m_wBaseItem	==	_lpItem->m_wBaseItem)
			return	ERRMSG("에러!!","이미 같은 종류의 아이템이 존재합니다.");
	}

	for (i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
	{
		if (m_aItem[i].m_wBaseItem	==	0xffff)
		{
			memcpy(&m_aItem[i],_lpItem,sizeof(cSetItemDefine)-4);

			cSetItem	*lpItem	=	(cSetItem	*)&m_aItem[i];

			lpItem->setComment(_lpItem->m_pItemComment);

			return	TRUE;
		}
	}

	return	FALSE;
}

BOOL	
cItemSet::removeItem(int _iIndex)
{
	m_aItem[_iIndex].reset();

	for (int i=_iIndex;i<dMAX_ITEM_COUNT_IN_SET;i++)
		memcpy(&m_aItem[i],&m_aItem[i+1],sizeof(cSetItemDefine));

	m_aItem[dMAX_ITEM_COUNT_IN_SET-1].reset();

	return	TRUE;
}

void	
cItemSet::setName(char *_lpstrName)
{
	strcpy(m_strName,_lpstrName);
}

void
cItemSet::replaceSetEffect(int _iCount,int _iEffectIndex,cUniqueData *_lpEffect)
{
	memcpy(&m_aSetEffect[_iCount][_iEffectIndex],_lpEffect,sizeof(cUniqueData));
}

BOOL	
cItemSet::addSetEffect(int _iCount,cUniqueData *_lpEffect)
{
	for (int i=0;i<dMAX_SET_ITEM_EFFECT_COUNT;i++)
	{
		if (m_aSetEffect[_iCount][i].m_wEffect	==	0xffff)
		{
			memcpy(&m_aSetEffect[_iCount][i],_lpEffect,sizeof(cUniqueData));

			return	TRUE;
		}
	}

	return	FALSE;
}

BOOL	
cItemSet::removeSetEffect(int _iCount,int _iIndex)
{
	m_aSetEffect[_iCount][_iIndex].reset();

	for (int i=_iIndex;i<dMAX_SET_ITEM_EFFECT_COUNT-1;i++)
		memcpy(&m_aSetEffect[_iCount][i],&m_aSetEffect[_iCount][i+1],sizeof(cUniqueData));

	m_aSetEffect[_iCount][dMAX_SET_ITEM_EFFECT_COUNT-1].reset();

	return	FALSE;
}

void	
cItemSet::reset()
{
	memset(this,0,sizeof(cItemSet));

	m_wSerial	=	0xffff;

	for(int i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
		m_aItem[i].reset();

	memset(m_aSetEffect,0xff,sizeof(m_aSetEffect));

	strcpy(m_strName,"NONAME");

	setComment("valid");

}

void
cItemSet::copy(cItemSet *_lpGroup)
{
	for(int	i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
		m_aItem[i].copy(&_lpGroup->m_aItem[i]);

	memcpy(this,_lpGroup,sizeof(cItemSetDefine)-sizeof(m_aItem));
	setComment(_lpGroup->m_pItemComment);
}

int	
cItemSetManager::add(cItemSet *_lpItemSet)
{
	m_den.setFirst();

	while(1)
	{
		cItemSet	*lpInfo	=	m_den.getNext();

		if (!lpInfo)
			break;

		if (stricmp(lpInfo->m_strName,_lpItemSet->m_strName) == 0)
		{
			ERRMSG("error!!","이미 같은 이름의 아이템 세트가 있습니다.");

			return	0xffff;
		}
	}

	return	m_den.add(_lpItemSet);
}

void
cItemSetManager::replace(int _iSerial,cItemSet *_lpItemSet)
{
	m_den.setFirst();

	while(1)
	{
		cItemSet	*lpInfo	=	m_den.getNext();

		if (!lpInfo || _iSerial == lpInfo->m_wSerial)
			break;

		if (stricmp(lpInfo->m_strName,_lpItemSet->m_strName) == 0)
		{
			ERRMSG("error!!","이미 같은 이름의 아이템 세트가 있습니다.");
			return;
		}
	}

	m_den.replace(_iSerial,_lpItemSet);
}

void
cItemSetManager::copy(cItemSetManager *_lpData)
{
	reset();

	_lpData->m_den.setFirst();

	while(1)
	{
		cItemSet	*lpInfo	=	_lpData->m_den.getNext();

		if	(!lpInfo)
			break;

		replace(lpInfo->m_wSerial,lpInfo);
	}

	rebuild();
}

void
cItemSetManager::build()
{
	m_den.rebuild();

	if (getCount() == 0)
	{
		cItemSet	group;

		group.reset();

		strcpy(group.m_strName,"it's valid one");

		add(&group);
	}
}

cItemSet*
cItemSetManager::getByName(char *_lpstrName)
{
	m_den.setFirst();

	while(1)
	{
		cItemSet	*lpInfo	=	m_den.getNext();

		if (!lpInfo)
			break;

		if (stricmp(lpInfo->m_strName,_lpstrName) == 0)
			return	lpInfo;
	}

	return	NULL;
}