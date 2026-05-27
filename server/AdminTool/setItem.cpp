#include "cItem.h"

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

cSetItem*
cItemSet::getItemByBaseItem(int _iBaseItem)
{
	for (int i=0;i<getItemCount();i++)
		if (_iBaseItem == m_aItem[i].m_wBaseItem)
			return	(cSetItem*)&m_aItem[i];

	return	NULL;
}

void
cItemSetManager::build()
{
	m_den.rebuild();
	m_den.remove(0);

	for (int i=0;i<m_den.getMaxCount();i++)
	{
		cItemSet	*lpItemSet	=	m_den.get(i);

		if	(!lpItemSet)
			continue;

		for (int iSetItem=0;iSetItem<dMAX_ITEM_COUNT_IN_SET;iSetItem++)
			lpItemSet->m_aItem[iSetItem].m_pItemComment	=	NULL;
	}
}