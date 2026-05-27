#include "pieceItem.h"
#include "setItem.h"
#include "CSetCustomItem.h"

cPieceItemManager	g_pieceItemManager;

void
cPieceItem::reset()
{
	m_wSerial			=	0xffff;
	m_wEquipLevel		=	0;

	memset(&m_item,0,sizeof(m_item));
	m_item.m_wBaseItem	=	0xffff;
	memset(m_item.m_aPrefix,0xff,sizeof(m_item.m_aPrefix));

	m_bf1IsSetItem		=	FALSE;
	m_bf11SetGroupIndex	=	1;
	m_bf4SetItemIndex	=	0;
	m_wPieceCount		=	4;

	for(int i=0;i<dMAX_PIECE_ITEM_PIECE_COUNT;i++)
		m_aPiece[i].reset();

	m_strName[0]		=	NULL;
}

void
cPieceItem::copy(cPieceItem *_lpItem)
{
	memcpy(this,_lpItem,sizeof(cPieceItem));
}

BOOL
cPieceItem::generateItem(cITEM	*_lpItem)
{
	if (m_bf1IsSetItem	==	FALSE)
	{
		if (m_item.m_wBaseItem	==	0xffff)
			return	FALSE;

		memcpy(_lpItem,&m_item,sizeof(CItemBaseInfo));
	}
	else
	{
		cItemSet	*lpGroup	=	g_itemSetManager.get(m_bf11SetGroupIndex);

		if (!lpGroup)
			return	FALSE;

		cSetItem		*lpSetItem	=	lpGroup->getItem(m_bf4SetItemIndex);

		if (!lpSetItem)
			return	FALSE;

		return	CSetCustomItem::GenerateItemBySetItem(_lpItem,lpSetItem);
	}

	return	TRUE;
}


void
cPieceItemManager::build()
{
	m_den.rebuild();

	if (getCount() == 0)
	{
		cPieceItem	pieceItem;

		pieceItem.reset();

		strcpy(pieceItem.m_strName,"it's valid one");

		add(&pieceItem);
	}
}

int
cPieceItemManager::add(cPieceItem *_lpPiece)
{
	m_den.setFirst();

	while(1)
	{
		cPieceItem *lpInfo	=	m_den.getNext();

		if (!lpInfo)
			break;

		if (stricmp(lpInfo->m_strName,_lpPiece->m_strName) == 0)
		{
			ERRMSG("error!!","이미 같은 이름의 조각 아이템이 있습니다.");

			return	0xffff;
		}
	}

	return	m_den.add(_lpPiece);
}

void
cPieceItemManager::replace(int _iSerial,cPieceItem *_lpPiece)
{
	m_den.setFirst();

	while(1)
	{
		cPieceItem	*lpInfo	=	m_den.getNext();

		if (!lpInfo || _iSerial == lpInfo->m_wSerial)
			break;

		if (stricmp(lpInfo->m_strName,_lpPiece->m_strName) == 0)
		{
			ERRMSG("error!!","이미 같은 이름의 조각 아이템 그룹이 있습니다.");
			return;
		}
	}

	m_den.replace(_iSerial,_lpPiece);
}

cPieceItem*
cPieceItemManager::getByName(char *_lpstrName)
{
	m_den.setFirst();

	while(1)
	{
		cPieceItem	*lpInfo	=	m_den.getNext();

		if (!lpInfo)
			break;

		if (stricmp(lpInfo->m_strName,_lpstrName) == 0)
			return	lpInfo;
	}

	return	NULL;
}
