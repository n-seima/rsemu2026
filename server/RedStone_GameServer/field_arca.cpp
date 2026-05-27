#include "cFIELD.h"
#include "arca.h"
#include "pieceItem.h"
#include "itemSet.h"
#include "cGame.h"
#include "secretDungeon.h"

BOOL
cFIELD::isArcaFull()
{
	if	(m_bIsReadyToClose	||	m_wArcaCount	>=	dMAX_ARCA_COUNT_IN_FIELD)
		return	TRUE;

	return	FALSE;
}

void
cFIELD::addActiveArca(int _iSerial)
{
	int		i;
	cArca	*lpArca	=	g_arcaManager.get(_iSerial);

	if	(lpArca)
		addSendUpdateArcaInfo(lpArca);
	else
		return;

	for (i=0;i<dMAX_ARCA_COUNT_IN_FIELD;i++)
		if (m_awArcaList[i]	==	_iSerial)
			return;

	for (i=0;i<dMAX_ARCA_COUNT_IN_FIELD;i++)
		if (m_awArcaList[i]	==	0xffff)
		{
			m_awArcaList[i]	=	_iSerial;
			m_wArcaCount++;
			return;
		}
}

void
cFIELD::removeActiveArca(int _iSerial)
{
	cArca	*lpArca	=	g_arcaManager.get(_iSerial);

	if	(lpArca)
	{
		SG_ARCA_WORK	packet;

		packet.base.set(sizeof(packet),dSG_ARCA_WORK);
		packet.wArcaIndex	=	_iSerial;
		packet.wWork		=	eAW_REMOVE;

		addSendPacket((ALL_MSG*)&packet,0xffff);
	}

	for (int i=0;i<dMAX_ARCA_COUNT_IN_FIELD;i++)
		if	(m_awArcaList[i]	==	_iSerial)
		{
			m_awArcaList[i]	=	0xffff;
			m_wArcaCount--;
			return;
		}
}

void
cFIELD::droppingItemInArcaByMonster(cACTOR *_lpActor,cACTOR *_lpKiller,cDroppingItem *_lpDroppingItem,int _iDropLevel,cArca *_lpArca,cItem *_lpItem)
{
	if	(!_lpKiller || !_lpActor)
		return;

	BOOL	bIsSecretDungeon=	FALSE;
	BOOL	bIsItemPiece	=	FALSE;
	BOOL	bIsUnique		=	FALSE;
	BOOL	bIsSetItem		=	FALSE;
	BOOL	bIsRare			=	FALSE;

	if	(m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG)
		bIsSecretDungeon	=	TRUE;

///	if	(g_bIsWantPrintTriggerCondition)
//		printf("아이템 조각 확률 %d％ ",66);

	if	(random(100) < 66)
	{
//		if	(g_bIsWantPrintTriggerCondition)
//			printf("성공\n");
		bIsItemPiece	=	TRUE;
	}
	else
	{
//		if	(g_bIsWantPrintTriggerCondition)
//			printf("실패\n");
	}
		
	if	(bIsItemPiece)
	{
		if	(random(100) < 5)
		{
//			if	(g_bIsWantPrintTriggerCondition)
//				printf("유니크 조각 으로 판정\n");

			bIsUnique	=	TRUE;
		}
		else
		if	(g_bIsBlockSetItem	==	FALSE	&&	random(100) < 33)
		{
//			if	(g_bIsWantPrintTriggerCondition)
//				printf("세트 아이템 조각 으로 판정\n");

			bIsSetItem	=	TRUE;
		}

		cItem	itemPiece;

		if	(g_pieceItemManager.generateItemPiece(bIsUnique,bIsSetItem,_iDropLevel,_lpDroppingItem->m_wItemType,&itemPiece,bIsSecretDungeon,FALSE,_lpActor->m_wCurrentField))
		{
			_lpArca->addItem(&itemPiece);

//			if	(g_bIsWantPrintTriggerCondition)
//				printf("드랍 레벨 체크 성공!![드랍레벨 %d]\n",_iDropLevel);
		}
		else
		{
//			if	(g_bIsWantPrintTriggerCondition)
//				printf("드랍 레벨 체크 실패!![드랍레벨 %d]\n",_iDropLevel);
		}
		
		return;
	}

	if (_lpItem->isUniqueItem() == FALSE)
	{
		int	iDropSetItemChance	=	100+_lpKiller->getLuck()+(_lpActor->m_iLevel*10);		//	1/10000 %

		iDropSetItemChance		=	min(iDropSetItemChance,200000);

//		if	(g_bIsWantPrintTriggerCondition)
//			printf("세트 아이템이 떨어질 확률 %d.%d％\n",iDropSetItemChance/10000,iDropSetItemChance%100);
		
		if	(largeRandom(1000000)	<	iDropSetItemChance)
		{
//			if	(g_bIsWantPrintTriggerCondition)
//				printf("조각이 아닌 세트 아이템이 떨어짐\n");

			cItem	setItem;

			if	(g_itemSetManager.generateRandomSetItem(_lpDroppingItem->m_wItemType,_iDropLevel,&setItem,bIsSecretDungeon,TRUE))
			{
				_lpArca->addItem(&setItem);

//				if	(g_bIsWantPrintTriggerCondition)
//					printf("드랍 레벨 체크 성공!![드랍레벨 %d]\n",_iDropLevel);

				return;
			}
			else
			{
//				if	(g_bIsWantPrintTriggerCondition)
//					printf("드랍 레벨 체크 실패!![드랍레벨 %d]\n",_iDropLevel);
			}
		}
	}

//	if	(g_bIsWantPrintTriggerCondition)
//		printf("일반 아이템 상자에 추가\n");
	
	_lpArca->addItem(_lpItem);
}

BOOL
cFIELD::operateArcaWorkInAreaByKarma(int _iArea,int _iWork)
{
	cAreaInfo	*lpWorkArea	=	getArea(_iArea);

	if (!lpWorkArea)
		return	FALSE;

	cRECT		*lpWorkRect	=	(cRECT	*)&lpWorkArea->x1;

	for (int i=0;i<dMAX_AREA;i++)
	{
		cAreaInfo	*lpArea	=	getArea(_iArea);

		if (!lpArea || lpArea->m_wKind	!=	dAREA_ARCA)
			continue;

		cRECT	*lpRect		=	(cRECT	*)&lpArea->x1;

		if (!lpWorkRect->Crash(lpRect))
			continue;

		cAreaArcaInfo	*lpArcaArea	=	(cAreaArcaInfo	*)lpArea;

		if (lpArcaArea->m_wLinkActiveArca	==	0xffff)
		{
			if (_iWork	==	eADTWK_REMOVE)
				continue;

			if (_iWork	==	eADTWK_REGEN)
			{
				lpArcaArea->generate();
				continue;
			}
		}

		if (_iWork	==	eADTWK_REGEN)
			continue;

		if (_iWork	==	eADTWK_REMOVE)
		{
			g_arcaManager.remove(lpArcaArea->m_wLinkActiveArca);
			continue;
		}

		cArca	*lpArca	=	g_arcaManager.get(lpArcaArea->m_wLinkActiveArca);

		if (!lpArca)
			continue;

		lpArca->operateKarmaWork(_iWork);
		addSendUpdateArcaInfo(lpArca);
	}

	return	TRUE;
}

BOOL
cFIELD::getTreasureMapPoint(cPOINT *_lpPos)
{
	int		i=100;
	
	BOOL	bIsFindNearestArea	=	FALSE;

	if	(random(100)	<=	30)
		bIsFindNearestArea	=	TRUE;

	if	(bIsFindNearestArea)
	{
		int	iMinRange	=	0x7fffffff;
		int	iMinRangeArea=	-1;

		for(i=m_pArea->m_wMonsterActiveAreaCount;i;)
		{
			--i;
			int			iArea	=	m_pArea->m_awMonsterActiveAreaList[i];
			cAreaInfo	*lpArea	=	getArea(iArea);

			if	(!lpArea)
				continue;

			cPOINT		posCenter;

			lpArea->getCenterPos(&posCenter);
			
			if	(isBlocked(posCenter.x/dTILE_XS,posCenter.y/dTILE_YS))
				continue;

			int	iCurRange	=	GetRange(_lpPos->x,_lpPos->y,posCenter.x,posCenter.y);

			if	(iCurRange	<=	iMinRange)
			{
				iMinRange	=	iCurRange;
				iMinRangeArea=	iArea;
			}
		}

		if	(iMinRangeArea	!=	-1)
		{
			cAreaInfo	*lpArea	=	getArea(iMinRangeArea);

			if	(lpArea)
			{
				lpArea->getCenterPos(_lpPos);
				return	TRUE;
			}
		}
	}

	for(;i;--i)
	{
		int			iArea	=	random(m_pArea->m_wMonsterActiveAreaCount);
		cAreaInfo	*lpArea	=	getArea(m_pArea->m_awMonsterActiveAreaList[iArea]);

		if	(!lpArea)
			continue;

		lpArea->getCenterPos(_lpPos);

		if	(isBlocked(_lpPos->x/dTILE_XS,_lpPos->y/dTILE_YS))
			continue;

		return	TRUE;
	}

	return	FALSE;
}