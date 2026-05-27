#include "arca.h"
#include "cACTOR.h"
#include "cPACKET_LOGSERVER.h"
#include "cFIELD.h"
#include "cGAME.h"
#include "secretDungeonDefine.h"
#include "CKarma.h"
#include "pieceItem.h"
#include "itemSet.h"
#include "packetManager.H"
#include "actor_mini_pet.h"

cArcaManager	g_arcaManager;

cFIELD*
cArca::getField()
{
	return	g_game.getFieldByUniqueSerial(m_wFieldSerial,0xffffffff,m_bf8InstanceField,m_bf4InstanceFieldFloor);
}

void
cArca::resetForTreasureMap(int _iX,int _iY)
{
	reset();

	m_wSerial			=	0x7fff;
	m_pos.x				=	_iX;
	m_pos.y				=	_iY;
	m_bSecretLevel		=	0;
	m_bf1IsDestroyAble	=	FALSE;
	m_bLockLevel		=	FALSE;
}

void
cArca::operateKarmaWork(int _iWork)
{
	switch(_iWork)
	{
		case	eADTWK_REVEAL	:
			m_bSecretLevel	=	0;
			break;

		case	eADTWK_HIDE		:
			m_bSecretLevel	=	255;
			break;

		case	eADTWK_LOCK		:
			m_bLockLevel	=	255;
			break;

		case	eADTWK_UNLOCK	:
			m_bLockLevel	=	0;
			break;
	}
}

int
cArca::getItemCount()
{
	int	iCount	=	0;

	for (int	i=0;i<dMAX_ITEM_COUNT_IN_ARCA;i++)
		if (m_aItem[i].m_wBaseItem	!=	0xffff)
			iCount++;

	return	iCount;
}

void
cArca::addDropGold(int _iDropGold,cACTOR *_lpMonster)
{
	int	iDropItemValue	=	6-getItemCount();

	if (iDropItemValue	<=	0)
		return;

	m_dwOwnGold			=	_iDropGold*iDropItemValue*10;
}

void
cArca::addItem(cItem *_lpItem)
{
	for (int	i=0;i<dMAX_ITEM_COUNT_IN_ARCA;i++)
	{
		if	(m_aItem[i].m_wBaseItem	!=	0xffff)
			continue;

		memcpy(&m_aItem[i],_lpItem,sizeof(cItem));
		break;
	}
}

BOOL
cArca::generateForMonsterDrop(cACTOR *_lpMonster,int _iOwner)
{
	if	(_lpMonster->m_wSerialInField	==	0xffff	||	_lpMonster->m_lpField	==	NULL)
		return	FALSE;

	reset();

	if	(_iOwner	!=	0xffff)
	{
		if (_iOwner	>=	0x8000)
			m_wOwnerParty		=	_iOwner	-	0x8000;
		else
			m_wOwner			=	_iOwner;
	}

	m_wLifeTime				=	dDROPPED_ITEM_LIFE_TIME;	//	dDROPPED_ITEM_LIFE_TIME*10초 (10초에 한번씩 줄임)
	m_wSealedTime			=	dDROPPED_ITEM_SEALED_TIME;	//	40초

	m_bf8InstanceField		=	0;
	m_bf4InstanceFieldFloor	=	0;
	m_iUniqueFieldSerial	=	_lpMonster->m_lpField->m_iUniqueSerial;

	if	(_lpMonster->m_lpField->m_iUniqueSerial	<	dINSTANCE_FIELD_FLAG)
		m_wFieldSerial			=	_lpMonster->m_lpField->m_iUniqueSerial;
	else
	{
		m_wFieldSerial			=	_lpMonster->m_lpField->m_wGateField+dINSTANCE_FIELD_FLAG;
		m_bf8InstanceField		=	_lpMonster->m_lpField->m_iUniqueSerial/dINSTANCE_FIELD_FLAG;
		m_bf4InstanceFieldFloor	=	_lpMonster->m_lpField->m_iUniqueSerial%dINSTANCE_FIELD_FLAG;
	}

	m_pos.Set(_lpMonster->m_pos.x,_lpMonster->m_pos.y);//어디에?

	m_iMaxDurability		=	(20+10*_lpMonster->getLevel())*100;
	m_iDurability			=	m_iMaxDurability;
	m_iResistance			=	(2*_lpMonster->getLevel())*100;

	if (random(100) < 60)
		m_bf1IsDestroyAble	=	TRUE;
	else
		m_bf1IsDestroyAble	=	FALSE;

	m_bf10DestroyAbleLevel	=	_lpMonster->getLevel();

	m_bSecretLevel			=	0;
	m_bLockLevel			=	0;

	if	(_lpMonster->getLevel()	>	50)
	{
		if (random(100) < 2)
		{
//			2% 확률로 0.17Lv±0.17Lv
			int	iLockLevel	=	17*_lpMonster->getLevel()  + random(17*2) - 17;
			m_bLockLevel	=	iLockLevel/100;
		}
	}

	if	(_lpMonster->getLevel()	>	200)
	{
		if (random(100) < 2)
		{
//			2% 확률로 0.17Lv±0.17Lv
			int	iSecretLevel=	17*_lpMonster->getLevel()  + random(17*2) - 17;
			m_bSecretLevel	=	iSecretLevel/100;
		}
	}
//	보유 금액	몬스터 데이터 계산식 * (6-보유 아이템 개수)* 100 	보유 아이템 개수: 상단에서 확률 체크 후 걸려든 아이템 개수
	m_dwOwnGold				=	0;
	m_wTrapLevel			=	0;//_lpMonster->getLevel();

	m_wDetectTrapLevel		=	random(_lpMonster->getLevel()/20-_lpMonster->getLevel()/7)+_lpMonster->getLevel()/7;
	m_wDisarmTrapLevel		=	random(_lpMonster->getLevel()/20-_lpMonster->getLevel()/7)+_lpMonster->getLevel()/7;

	if	(random(100) < 1)
		m_awTrapActiveChance[0]	=	100;
	else
		m_awTrapActiveChance[0]	=	0;

	if	(random(100) < 3)
		m_awTrapActiveChance[1]	=	100;
	else
		m_awTrapActiveChance[1]	=	0;

	if	(m_awTrapActiveChance[0]+m_awTrapActiveChance[1] == 0)
		m_wTrapLevel			=	0;

	if	(random(2)	==	0)
		m_bf2TrapTarget		=	eTT_TARGET;
	else
		m_bf2TrapTarget		=	eTT_RANGE;

	for (int	i=0;i<dMAX_ITEM_COUNT_IN_ARCA;i++)
		m_aItem[i].reset();

	return	TRUE;
}

BOOL
cArca::generate(cArcaInfo *_lpArcaInfo,cFIELD *_lpField,int _iX,int _iY)
{
	if	(!_lpField)
	{
		CLOG("arca","cArca::generate - invalid field");

		return	FALSE;
	}

	reset();

	m_bf8InstanceField		=	0;
	m_bf4InstanceFieldFloor	=	0;
	m_iUniqueFieldSerial	=	_lpField->m_iUniqueSerial;

	if	(_lpField->m_iUniqueSerial	<	dINSTANCE_FIELD_FLAG)
		m_wFieldSerial			=	_lpField->m_iUniqueSerial;
	else
	{
		m_wFieldSerial			=	_lpField->m_wGateField+dINSTANCE_FIELD_FLAG;
		m_bf8InstanceField		=	_lpField->m_iUniqueSerial/dINSTANCE_FIELD_FLAG;
		m_bf4InstanceFieldFloor	=	_lpField->m_iUniqueSerial%dINSTANCE_FIELD_FLAG;
	}

	BOOL	bIsSecretDungeon=	FALSE;

	if	(_lpField->m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG)
		bIsSecretDungeon	=	TRUE;

	m_pos.Set(_iX,_iY);//어디에?

	m_iDurability			=	_lpArcaInfo->m_iDurability;
	m_iMaxDurability		=	_lpArcaInfo->m_iDurability;
	m_iResistance			=	_lpArcaInfo->m_iResistance;

	m_bf1IsDestroyAble		=	_lpArcaInfo->m_bf1IsDestroyAble		;
	m_bf10DestroyAbleLevel	=	_lpArcaInfo->m_bf10DestroyAbleLevel	;

	m_bSecretLevel			=	_lpArcaInfo->m_bSecretLevel			;
	m_bLockLevel			=	_lpArcaInfo->m_bLockLevel			;

	m_dwOwnGold				=	_lpArcaInfo->m_dwOwnGold			;

	m_wTrapLevel			=	_lpArcaInfo->m_wTrapLevel			;
	m_wDetectTrapLevel		=	_lpArcaInfo->m_wDetectTrapLevel		;
	m_wDisarmTrapLevel		=	_lpArcaInfo->m_wDisarmTrapLevel		;

	m_awTrapSkill[0]		=	_lpArcaInfo->m_wTrap1Skill			;
	m_awTrapActiveChance[0]	=	_lpArcaInfo->m_wTrap1ActiveChance	;
	m_awTrapSkill[1]		=	_lpArcaInfo->m_wTrap2Skill			;
	m_awTrapActiveChance[1]	=	_lpArcaInfo->m_wTrap2ActiveChance	;
	m_bf2TrapTarget			=	_lpArcaInfo->m_bf2TrapTarget;

	if (_lpArcaInfo->m_wTrap1ActiveChance)
	{
		if (random(100) < _lpArcaInfo->m_wTrap1ActiveChance)
			m_awTrapActiveChance[0]	=	100;
		else
			m_awTrapActiveChance[0]	=	0xffff;
	}
	if (_lpArcaInfo->m_wTrap2ActiveChance)
	{
		if (random(100) < _lpArcaInfo->m_wTrap2ActiveChance)
			m_awTrapActiveChance[1]	=	100;
		else
			m_awTrapActiveChance[1]	=	0xffff;
	}

	if (m_awTrapActiveChance[0]	==	0xffff	&&	m_awTrapActiveChance[1]	==	0xffff)
		m_wTrapLevel			=	0;

	int	iGenItemCount		=	0,i;

	for (i=0;i<dMAX_ITEM_COUNT_IN_ARCA;i++)
		m_aItem[i].reset();

	for (i=0;i<dMAX_ITEM_COUNT_IN_ARCA;i++)
	{
		cDroppingItem	*lpDroppingItem	=	&_lpArcaInfo->m_aItem[i];
		cItem			item;

		item.m_dwSerial	=	largeRandom(0x7ffffff0)+10;

		if	(lpDroppingItem->m_wItemType	==	0xffff)
			continue;

		if	(random(10000)	>=	(int)lpDroppingItem->m_dwDroppingRate)
			continue;

		if	(lpDroppingItem->m_bf1IsCustomItem)
		{
			CCustomItem	*lpCustomItem	=	g_customItem.getItem(lpDroppingItem->m_wItemType);

			if	(!lpCustomItem)
				continue;

			if	(!g_im.generateItemByCustomItem(&item,lpCustomItem))
				continue;

			memcpy(&m_aItem[iGenItemCount],&item,sizeof(CItemDefine));
			iGenItemCount++;
		}
		else
		if	(lpDroppingItem->m_bf1IsItemPiece)
		{
			cDroppingItemForItemPiece	*lpDropInfo	=	(cDroppingItemForItemPiece	*)lpDroppingItem;

			if	(g_im.generateItemPiece(&m_aItem[iGenItemCount],lpDropInfo->m_wPieceItem,lpDropInfo->m_bf4ItemPiece,bIsSecretDungeon,TRUE,_lpField->m_iUniqueSerial))
				iGenItemCount++;
		}
		else
		if	(lpDroppingItem->m_bf1IsSetItem)
		{
			cDroppingItemForSetItem	*lpSetItemDropInfo	=	(cDroppingItemForSetItem	*)lpDroppingItem;

			if	(g_itemSetManager.generateSetItem(&m_aItem[iGenItemCount],lpSetItemDropInfo->wItemSet,lpSetItemDropInfo->m_bf4SetItemIndex))
				iGenItemCount++;
		}
		else
		{
			if	(g_im.generateArcaItem(&m_aItem[iGenItemCount],lpDroppingItem->m_wItemType,_lpArcaInfo->m_wItemDropLevel,lpDroppingItem->m_bf8Count,100))
				iGenItemCount++;
		}
	}
	
	if	(iGenItemCount)
		return	TRUE;

	return	FALSE;
}

BOOL
cArca::checkOwner(cACTOR *_lpActor)
{
	cFIELD	*lpField	=	getField();

	if	(!lpField)
		return	FALSE;

	if	(lpField->m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG)
	{
		if ((lpField->m_iUniqueSerial/dINSTANCE_FIELD_FLAG)	!=	(WORD)_lpActor->m_bf8InstanceField		||	
			(lpField->m_iUniqueSerial%dINSTANCE_FIELD_FLAG)	!=	(WORD)_lpActor->m_bf4InstanceFieldFloor	||
			lpField->m_wGateField								!=	_lpActor->m_wCurrentField-dINSTANCE_FIELD_FLAG
			)
		{
			_lpActor->sendArcaWork(m_wSerial,eAW_IS_FAILED);
			return	FALSE;
		}
	}
	else
	if (lpField->m_iUniqueSerial	!=	_lpActor->m_wCurrentField)
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_IS_FAILED);
		return	FALSE;
	}

	cRECT	rect;

	rect.Set(m_pos.x-64,m_pos.y-64,m_pos.x+64,m_pos.y+32);

	if (!rect.Isin(_lpActor->m_pos.x,_lpActor->m_pos.y))
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_IS_FAILED);
		return	FALSE;
	}

	if (m_wOwner	==	0xffff	&&	m_wOwnerParty	==	0xffff)
		return	TRUE;

	if (m_wOwner	!=	0xffff	&&	m_wOwner	!=	_lpActor->m_wSerialInField)
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_IS_OWNED_OTHER);
		return	FALSE;
	}

	if (m_wOwnerParty	!=	0xffff	&&	m_wOwnerParty	!=	_lpActor->getPartySerial())
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_IS_OWNED_OTHER);
		return	FALSE;
	}

	return	TRUE;
}

BOOL
cArca::open(cACTOR *_lpActor)
{
	if	(checkOwner(_lpActor)	==	FALSE)
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_IS_HIDE);
		return FALSE;
	}

	if	(!_lpActor->isVisibleObject(m_bSecretLevel))
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_IS_HIDE);

		return FALSE;
	}

	if	(m_wTrapLevel)
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_IS_TRAP);

		return FALSE;
	}

	if	(m_bLockLevel)
	{
		tryUnlock(_lpActor,FALSE);

		return FALSE;
	}

	cFIELD	*lpField	=	getField();

	if	(!lpField)
		return	FALSE;

	if	(g_bIsDuelServer)
	{
		cItem	item;

		item.copy(&m_aItem[0]);

		if	(m_aItem[0].m_wBaseItem	!=	0xffff)
		{
			_lpActor->eatItem(&item);
			_lpActor->sendEtcWork(eEW_EAT_DUEL_FIELD_ARCA_ITEM,m_aItem[0].m_wBaseItem);

			SG_USE_ITEM	sgUseItem;

			sgUseItem.base.set(sizeof(sgUseItem),dSG_USE_ITEM);
			sgUseItem.wWho	=	_lpActor->m_wSerialInField;
			sgUseItem.bf1IsUseEffectToHero	=	FALSE;
			sgUseItem.bf1IsDisplayMsg		=	FALSE;
			sgUseItem.base.wSize			=	sizeof(sgUseItem)-sizeof(sgUseItem.strCaster);

			memcpy(&sgUseItem.item,&m_aItem[0],sizeof(sgUseItem.item));

			lpField->addSendPacket((ALL_MSG*)&sgUseItem,_lpActor->m_wSerialInField);
		}
		return	TRUE;
	}

	{
		SG_ARCA_WORK		packet;

		packet.base.set(sizeof(packet),dSG_ARCA_WORK);
		packet.wWork		=	eAW_OPEN;
		packet.wArcaIndex	=	m_wSerial;
		packet.wValue1		=	_lpActor->m_wSerialInField;

		lpField->addSendPacket((ALL_MSG*)&packet,0xffff);
	}
	
	SG_PICK_ITEM_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_PICK_ITEM_RESULT);

	packet.wResult		=	eUIR_OK;

	cItem	pickingItem,itemLog;
	
	cParty	*lpParty	=	_lpActor->getParty();

	for (int i=0;i<dMAX_ITEM_COUNT_IN_ARCA;i++)
	{
		if (m_aItem[i].m_wBaseItem	==	0xffff)
			continue;

		pickingItem.copy(&m_aItem[i]);
		pickingItem.m_dwSerial	=	random(0x7ffffff0)+10;
		itemLog.copy(&pickingItem);

		cACTOR	*lpPicker	=	_lpActor;

		if (lpParty)
			lpPicker	=	lpParty->getThisTimePicker(lpField->m_iUniqueSerial,&m_pos,_lpActor,&pickingItem);

		if (lpPicker->getValidInventorySlot()	==	NULL)
		{
			lpPicker->sendArcaWork(m_wSerial,eAW_FULL_INVENTORY);
			return FALSE;
		}

		int	iAddResult;

		if	(pickingItem.isMoneyItem())
			iAddResult	=	lpPicker->addGoldItemToInventory(&pickingItem,eGET_GOLD_BY_PICK_DROPPED_GOLD);
		else
			iAddResult	=	lpPicker->addItemToInventory(&pickingItem);

		if	(iAddResult	==	eUIR_OK)
		{
			memcpy(&packet.item,&itemLog,sizeof(CItemDefine));

			g_userPM.add(lpPicker->m_iClientSerial,&packet,packet.base.wSize);

			if (itemLog.isUniqueItem() || itemLog.isRareItem())
				lpPicker->sendPickItemLog(&itemLog,ePIM_ARCA);

			if (m_wOwnerParty	!=	0xffff)
			{
				cParty	party;

				g_pPartyManager->getParty(&party,m_wOwnerParty);

				party.sendPickItemMessage(lpPicker,&itemLog);
			}

			m_aItem[i].reset();
		}
	}

	if	(m_dwOwnGold	==	0)
		return	TRUE;

	itemLog.reset();
	itemLog.m_bCount	=	1;
	itemLog.m_wBaseItem	=	dITEM_MONEY;
	itemLog.m_dwSerial	=	m_dwOwnGold;

	int		iRemainGold	=	0;

	if (lpParty)
	{
		lpParty->sharingGold(lpField->m_iUniqueSerial,&m_pos,_lpActor,&itemLog);

		if	(itemLog.m_wBaseItem	==	0xffff)
			m_dwOwnGold	=	0;
		else
			m_dwOwnGold	=	itemLog.m_dwSerial;
	}
	else
	{
		DWORD	dwSumGold	=	_lpActor->m_iGold	+	m_dwOwnGold;
		int		iGetGold	=	m_dwOwnGold;

		if (dwSumGold	>=	0x7fffffff)
		{
			iGetGold	=	0x7fffffff-_lpActor->m_iGold;
			iRemainGold	=	m_dwOwnGold-iGetGold;
		}

		itemLog.m_dwSerial	=	iGetGold;
		_lpActor->m_iGold	+=	iGetGold;
		m_dwOwnGold			=	iRemainGold;

		_lpActor->updateGoldLog(iGetGold,eGET_GOLD_BY_PICK_DROPPED_GOLD);

		_lpActor->sendPickItemLog(&itemLog,ePIM_ARCA);

		memcpy(&packet.item,&itemLog,sizeof(CItemDefine));

		g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}

	if	(m_dwOwnGold)
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_FULL_GOLD);

		return FALSE;
	}

	return	TRUE;
}

void
cArca::unlock(cACTOR *_lpActor)
{
	_lpActor->addSendCloseRangeArcaWork(m_wSerial,eAW_UNLOCK_BY_SKILL,0xffff,_lpActor->m_strName);
	m_bLockLevel	=	0;
}

void
cArca::tryUnlock(cACTOR *_lpActor,BOOL _bIsWantUseKey)
{
	if	(checkOwner(_lpActor)	==	FALSE)
		return;

	if (m_bLockLevel	==	0)
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_UNLOCK_FAILED_BY_ALREADY_UNLOCKED);

		return;
	}

	if (!_lpActor->isUnlockAbleArcaBySkill(m_bLockLevel) && !_lpActor->isUnlockAbleArcaByKey(m_bLockLevel))
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_CAN_NOT_UNLOCK);

		return;
	}

	if (_lpActor->isUnlockAbleArcaBySkill(m_bLockLevel))
	{
		cAbility	*lpAbility	=	_lpActor->getUnlockArcaSkill();

		int			iResult		=	_lpActor->actionToObject(lpAbility,eTARGET_OBJECT_ARCA,m_wSerial);

		if (iResult	==	eAR_OK)
			_lpActor->sendArcaWork(m_wSerial,eAW_TRY_UNLOCK_BY_SKILL);
		else
		if (iResult	==	eAR_LOW_CP)
			_lpActor->sendArcaWork(m_wSerial,eAW_TRY_UNLOCK_FAILED_BY_LOW_CP);
		else
			_lpActor->sendArcaWork(m_wSerial,eAW_TRY_UNLOCK_FAILED_BY_BUSY);

		return;
	}

	int		iKey	=	_lpActor->getUnlockArcaKeySlot(m_bLockLevel);

	if (iKey	!=	0xffff)
	{
		if (!_bIsWantUseKey)
		{
			_lpActor->sendArcaWork(m_wSerial,eAW_DO_YOU_WANT_UNLOCK_ARCA_BY_USE_KEY,iKey);
			return;
		}

		int		iKey	=	_lpActor->getUnlockArcaKeySlot(m_bLockLevel);
		cItem	*lpKey	=	_lpActor->getInventoryItem(iKey);

		_lpActor->addSendCloseRangeArcaWork(m_wSerial,eAW_UNLOCK_BY_KEY,iKey,_lpActor->m_strName);
		lpKey->decreaseCount(1);

		m_bLockLevel	=	0;

		return;
	}

	_lpActor->sendArcaWork(m_wSerial,eAW_CAN_NOT_UNLOCK);
}

void
cArca::attack(cACTOR *_lpActor)
{
	if	(checkOwner(_lpActor)	==	FALSE)
		return;

	if (m_bf1IsDestroyAble	==	FALSE)
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_ATTACK_ARCA_FAILED_BY_INVINCIBLE);
		return;
	}

	if (_lpActor->m_iLevel < m_bf10DestroyAbleLevel)
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_ATTACK_ARCA_FAILED_BY_LOW_LEVEL,m_bf10DestroyAbleLevel);
		return;
	}

	int	iResult	=	_lpActor->attackToObject(eATTACK_OBJECT_ARCA,m_wSerial);

	if (iResult	==	eAR_ALREADY_OPEN_ARCA)
		_lpActor->sendArcaWork(m_wSerial,eAW_ATTACK_FAILED_BY_ALREADY_UNLOCKED);
	else
	if (iResult	!=	eAR_OK)
		_lpActor->sendArcaWork(m_wSerial,eAW_ATTACK_ARCA_FAILED);
}

void
cArca::tryDisarmTrap(cACTOR *_lpActor)
{
	if	(m_wTrapLevel	==	0)
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_DISARM_FAILED_BY_ALREADY_DISARMED);

		return;
	}

	if	(!_lpActor->isDisarmAbleTrap(m_wDisarmTrapLevel))
	{
		_lpActor->sendArcaWork(m_wSerial,eAW_DISARM_FAILED_BY_NOT_EXIST_DISARM_SKILL);

		return;
	}

	cAbility	*lpDisarmAbility	=	_lpActor->getDisarmTrapAbility();

	int	iResult	=	_lpActor->actionToObject(lpDisarmAbility,eTARGET_OBJECT_ARCA,m_wSerial);

	if	(iResult	==	eAR_OK)
		_lpActor->sendArcaWork(m_wSerial,eAW_TRY_DISARM_BY_SKILL);
	else
	if	(iResult	==	eAR_LOW_CP)
		_lpActor->sendArcaWork(m_wSerial,eAW_TRY_DISARM_FAILED_BY_LOW_CP);
	else
		_lpActor->sendArcaWork(m_wSerial,eAW_TRY_DISARM_FAILED_BY_BUSY);
}

void
cArca::disarmTrap(cACTOR *_lpActor)
{
	if	(checkOwner(_lpActor)	==	FALSE)
		return;

	m_wTrapLevel		=	0;
	m_wDetectTrapLevel	=	0;
	m_wDisarmTrapLevel	=	0;
}

void
cArca::explosionTrap(cACTOR *_lpActor)
{
	_lpActor->addSendCloseRangeArcaWork(m_wSerial,eAW_EXPLOSION_TRAP);

	if	(random(100)	>=	_lpActor->getTrapEvadeCahnceByTitle())
		for (int i=0;i<2;i++)
		{
			cTrapSkill*	lpTrap	=	NULL;

			if (m_awTrapActiveChance[i]	==	0)
				lpTrap	=	g_trapSkillManager.getSkill(m_awTrapSkill[i]);
			else
			{
				if (random(100) < m_awTrapActiveChance[i])
					lpTrap	=	g_trapSkillManager.getRandomSkill(i);
			}

			if	(!lpTrap)
				continue;

			_lpActor->m_lpField->explosionTrapSkill(_lpActor,lpTrap,m_bf2TrapTarget,m_wTrapLevel);
		}

	m_wDetectTrapLevel	=	0;
	m_wTrapLevel		=	0;
	m_wDisarmTrapLevel	=	0;
}

void
cArca::getDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo)
{
	int	iAttackPower	=	_lpAttacker->getPhysicalAttackPower(_lpAbility,0,FALSE,FALSE,NULL,FALSE,NULL)/100;
	int	iMagicPower		=	_lpAttacker->getMagicAttackPower(_lpAbility)/100;
	int	iCorrectDamage	=	_lpAttacker->getBonusAttackDoorPowerByTitle()+100;
	int	iDamage			=	0;

	_lpAttacker->iMiniPet()->operateDestroyObjectSkill(&iAttackPower,&iMagicPower);

	iAttackPower		=	iAttackPower*iCorrectDamage/100;
	iMagicPower			=	iMagicPower*iCorrectDamage/100;

	int	iPhysicalDamage	=	iAttackPower-m_iResistance;
	int	iMagicDamage	=	iMagicPower-m_iResistance;

	if	(iAttackPower)
		iPhysicalDamage	=	max(iPhysicalDamage,1);
	else
		iPhysicalDamage	=	0;

	if	(iMagicPower)
		iMagicDamage	=	max(iMagicDamage,1);
	else
		iMagicDamage	=	0;

	_lpHitInfo->m_dwAbsorbDamage	=	0;
	_lpHitInfo->m_dwPhysicalDamage	=	iPhysicalDamage*100;
	_lpHitInfo->m_dwMagicDamage		=	iMagicDamage*100;
	_lpHitInfo->setHit();
}

void
cArca::destroyItemByDestroyLock(cACTOR *_lpAttacker)
{
	int	i;

	SG_BROKEN_ARCA_ITEM_BY_ATTACK	packet;

	packet.base.set(sizeof(packet),dSG_BROKEN_ARCA_ITEM_BY_ATTACK);
	packet.wCount	=	0;

	for (i=0;i<dMAX_ITEM_COUNT_IN_ARCA;i++)
	{
		if (m_aItem[i].m_wBaseItem	==	0xffff)
			continue;

//	※ 보물 상자를 파괴하거나 함정이 발동했을 경우에는 그 안에 있는 아이템이 각각 90%-(공격자의 운/40)%의 확률로 파괴됨. 최소치는 50%
		int	iChance	=	90-(_lpAttacker->getLuck()/40);
		iChance		=	max(iChance,50);

		if (random(100) < iChance)
		{
			memcpy(&packet.aItem[packet.wCount++],&m_aItem[i],sizeof(CItemBaseInfo));
			m_aItem[i].reset();
		}
	}

	if (packet.wCount)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aItem)+sizeof(CItemBaseInfo)*packet.wCount;

		g_userPM.add(_lpAttacker->m_iClientSerial,&packet,packet.base.wSize);
	}
}

void
cArca::hitDamage(cACTOR *_lpAttacker,int _iDamage)
{
	m_iDurability	-=	_iDamage;

	if	(m_iDurability	<=	0)
	{
		m_bLockLevel	=	0;

		_lpAttacker->sendArcaWork(m_wSerial,eAW_UNLOCK_BY_ATTACK,m_wSerial,TRUE);

		destroyItemByDestroyLock(_lpAttacker);
	}
}

cArca*
cArcaManager::get(int _iIndex)
{
	cArca*	lpArca	=	m_den.get(_iIndex);

	if	(!lpArca)
		return	NULL;

	cFIELD	*lpField	=	lpArca->getField();

	if	(lpField	==	NULL)
	{
		remove(_iIndex);

		return	NULL;
	}

	return	lpArca;
}

BOOL
cArcaManager::remove(int _iIndex)
{
	cArca	*lpArca		=	m_den.get(_iIndex);

	if	(!lpArca)
		return	FALSE;
	
	cFIELD	*lpField	=	lpArca->getField();

	if	(lpField)
		lpField->removeActiveArca(_iIndex);

	return	m_den.remove(_iIndex);
}

void
cArcaManager::update()
{
}

void
cArcaManager::operateOncePer10SecondWork()
{
}

int
cArcaManager::generate(int _iSourceArcaIndex,cFIELD *_lpField,int _iX,int _iY)
{
	cArca	arca;

	if	(arca.generate(&m_pArcaList[_iSourceArcaIndex],_lpField,_iX,_iY))
		return	add(&arca);

	return	0xffff;
}

void
cArcaManager::open(int _iArcaIndex,cACTOR *_lpActor)
{
	cArca	*lpArca	=	get(_iArcaIndex);

	if (!lpArca)
	{
		_lpActor->sendArcaWork(_iArcaIndex,eAW_IS_FAILED);

		return;
	}
	
	if	(lpArca->open(_lpActor))
		remove(_iArcaIndex);
}

void
cArcaManager::disarmTrap(int _iArcaIndex,cACTOR	*_lpActor)
{
	cArca	*lpArca	=	get(_iArcaIndex);

	if (!lpArca)
	{
		_lpActor->sendArcaWork(_iArcaIndex,eAW_IS_FAILED);

		return;
	}

	lpArca->tryDisarmTrap(_lpActor);
}

void
cArcaManager::tryUnlock(int _iArcaIndex,cACTOR	*_lpActor,BOOL _bIsWantUseKey)
{
	cArca	*lpArca	=	get(_iArcaIndex);

	if (!lpArca)
	{
		_lpActor->sendArcaWork(_iArcaIndex,eAW_IS_FAILED);

		return;
	}
	
	lpArca->tryUnlock(_lpActor,_bIsWantUseKey);
}

void
cArcaManager::attack(int _iArcaIndex,cACTOR	*_lpActor)
{
	cArca	*lpArca	=	get(_iArcaIndex);

	if (!lpArca)
	{
		_lpActor->sendArcaWork(_iArcaIndex,eAW_IS_FAILED);

		return;
	}

	lpArca->attack(_lpActor);
}