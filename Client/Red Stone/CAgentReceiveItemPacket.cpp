#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CFieldItem.H"
#include "CMessage.H"
#include "CWindowInterface.H"
#include "hero_mini_pet.H"

//
//아이템 떨궈짐
void
cPACKET_HANDLER::DroppingItems(SERVER_PACKETS *_lpPacket)
{
	SG_DROPPING_ITEMS	*lpPacket	=	&_lpPacket->mSG_DROPPING_ITEMS;

	for (int i=0;i<lpPacket->wCount;i++)
	{
		cDroppedItem		droppedItem;

		cP_DROPPED_ITEM		*lpDropInfo	=	&lpPacket->aDroppedItem[i];

		droppedItem.m_dwCount	=	lpDropInfo->m_wCount;
		droppedItem.m_wSerial	=	lpDropInfo->m_bf10Serial;
		droppedItem.m_wBaseItem	=	lpDropInfo->m_bf14BaseItem;
		droppedItem.m_wType		=	lpDropInfo->m_bf4Type;

		droppedItem.m_pos.x		=	lpDropInfo->wX;	//	어디에 떨어져 있냐?
		droppedItem.m_pos.y		=	lpDropInfo->wY;	//어디에 떨어져 있냐?

		g_fieldItem.insert(&droppedItem);
	}
}

void
cPACKET_HANDLER::ReceiveItemWorkResult(SERVER_PACKETS *_lpPacket)
{
	SG_ITEM_WORK_RESULT	*lpPacket	=	(SG_ITEM_WORK_RESULT *)_lpPacket;

	s_agent.setOperateItemData(FALSE);
	
	switch(lpPacket->wResult)
	{
		case	eRIWR_ITEM_SYNC_FAILED	:
			CGamePlay::AddSystemMessage(LTRED,dMSG_ITEM_DATA_NOT_MATCHED_PLEASE_RETRY);
			s_agent.sendAskItemData();
			g_gwRepairItem.close();
			break;

		case	eRIWR_FAILED			:
			g_gwRepairItem.close();
			break;
	}
}

void
cPACKET_HANDLER::ReceiveRepairItem(SERVER_PACKETS *_lpPacket)
{
	SG_REPAIR_ITEM	*lpPacket	=	(SG_REPAIR_ITEM *)_lpPacket;

	s_agent.setOperateItemData(FALSE);

	cItem	*lpItem			=	g_hero.getInventoryItem(lpPacket->bPart);

	lpItem->m_bDurability	=	lpPacket->bDurability;
	g_hero.m_iGold			-=	lpPacket->dwRepairPrice;

	CGamePlay::AddSystemMessage(LTGREEN,dMSG_REPAIR_ITEM_COMMENT);
}

void
cPACKET_HANDLER::ReceiveOpenRepairItemWindow(SERVER_PACKETS *_lpPacket)
{
	SG_OPEN_REPAIR_ITEM_WINDOW	*lpPacket	=	(SG_OPEN_REPAIR_ITEM_WINDOW *)_lpPacket;

	g_gwRepairItem.open(lpPacket->wNpcSerial);
}
void
cPACKET_HANDLER::ReceiveOpenItemDisjointingWindow(SERVER_PACKETS *_lpPacket)
{
	SG_OPEN_DISJOINTING_ITEM_WINDOW	*lpPacket	=	(SG_OPEN_DISJOINTING_ITEM_WINDOW *)_lpPacket;

	g_gwCommonList.reset();
	
	g_gwCommonList.setOkButtonText(dMSG_DISJOINTING_ITEM_TOP_TEXT);
	g_gwCommonList.setTitleText(dMSG_DISJOINTING_ITEM_BUTTON_TEXT);
	g_gwCommonList.setValue(0,0);
	
	g_gwCommonList.open(eCLW_SELECT_DISJOINTING_ITEM,FALSE,TRUE);

	for (int i=0;i<dOWN_ITEM_COUNT+g_hero.m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	g_hero.getInventoryItem(i);
		
		if	(!lpItem || lpItem->isExceptionItem() || lpItem->isSetItem())	
			continue;
		cBasicItem* lpBasic = lpItem->getBasicItem();
		if(!lpBasic)
			continue;
		if(c_aItemEquipPlace[lpItem->getItemType()] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpItem->getItemType()] == 0xffff)		//  변경..
			continue;
		
		
		
		if	(!lpItem->isUniqueItem())
			continue;

		if(lpItem->isExpirationData())
			continue;

		if(lpBasic->m_wExpireYear  != 0)		// 기본 아이템 설정된 기간제 아이템은 제외
			continue;	
	
		g_gwCommonList.addContents(_ms("<c:LTORANGE>%s<n>",lpItem->getBasicItem()->m_strName),i,-1,TRUE);
	}

}


//
//장비 해제 결과 받음
void
cPACKET_HANDLER::ReceiveStripEquipmentResult(SERVER_PACKETS *_lpPacket)
{
	SG_STRIP_EQUIPMENT_RESULT	*lpPacket	=	&_lpPacket->mSG_STRIP_EQUIPMENT_RESULT;

	s_agent.setOperateItemData(FALSE);

	switch(lpPacket->wResult)	//	결과
	{
		case	eUIR_OK								:	//	ok -o-!
		{
			g_hero.stripEquipment(lpPacket->wPart);				//	버리기로 예약해둔 아이템 제거
			// 10.01.11 추가
			g_hero.changeQuickSlotStripItem();

			break;
		}

		case	eUIR_WRONG_ITEM_DATA				:	//	얼래.. 뭔가 이상한디.. -_-a
			CGamePlay::AddSystemMessage(LTRED,dMSG_ITEM_DATA_NOT_MATCHED_PLEASE_RETRY);
			s_agent.sendAskItemData();
			break;

		case	eUSR_CAN_NOT_STRIP_EQUIPMENT_WHEN_ACTION	:
			g_lpHero->warning(dMSG_CAN_NOT_STRIP_OR_EQUIP_EQUIPMENT_WHEN_ACTION);
			break;

		case	eUPR_PICK_ITEM_IS_COPY_ITEM			:
			CGamePlay::AddSystemMessage(LTRED,dMSG_IS_COPY_ITEM);
			break;

		case	eUSR_PLEASE_PICK_WEAPON				:
			CGamePlay::AddSystemMessage(LTRED,dMSG_PLEASE_PICK_THROW_WEAPON);
			break;

		case	eUSR_CAN_NOT_STRIP_SHIELD_STATUS	:
			CGamePlay::AddSystemMessage(LTRED,dMSG_CAN_NOT_STRIP_SHIELD_STATUS);
			break;
		case	eUSR_CAN_NOT_STRIP_WEAPON_STATUS	:
			CGamePlay::AddSystemMessage(LTRED,dMSG_CAN_NOT_STRIP_WEAPON_STATUS);
			break;

		case	eUSR_CAN_NOT_CHANGE_SHIELD_STATUS	:
			CGamePlay::AddSystemMessage(LTRED,dMSG_CAN_NOT_CHANGE_SHIELD_STATUS);
			break;
		case	eUSR_CAN_NOT_CHANGE_WEAPON_STATUS	:
			CGamePlay::AddSystemMessage(LTRED,dMSG_CAN_NOT_CHANGE_WEAPON_STATUS);
			break;


	}
	// 10.01.14
	g_hero.m_bUseSkillSlotItem = FALSE;
}

void
cPACKET_HANDLER::ReceiveDecreaseItemCount(SERVER_PACKETS *_lpPacket)
{
	SG_DECREASE_ITEM_COUNT	*lpPacket	=	&_lpPacket->mSG_DECREASE_ITEM_COUNT;

	g_hero.m_aItems[lpPacket->wItemSlot].decreaseCount(lpPacket->wCount);
}

void
cPACKET_HANDLER::ReceiveAddItem(SERVER_PACKETS *_lpPacket)
{
	SG_ADD_ITEM				*lpPacket	=	&_lpPacket->mSG_ADD_ITEM;

	cItem	*lpItem	=	g_hero.getValidInventorySlot(),itemAdd;

	itemAdd.copy((cItem*)&lpPacket->item);

	if (!g_hero.addItemToInventory((cItem*)&lpPacket->item))
	{
		s_agent.sendAskItemData();
		return;
	}

	g_hero.buildPower();

	switch(lpPacket->wReason)
	{
		case	eIAR_NORMAL							:
			CGamePlay::AddSystemMessage(WHITE,dMSG_RECEIVE_ITEM_BY_QUEST_FORM,itemAdd.getName());
			break;

		case	eIAR_WITHDRAW_FROM_GUILD_INVENTORY	:
			CGamePlay::AddSystemMessage(WHITE,dMSG_WITHDRAW_ITEM_FROM_GUILD_INVENTORY_FORM,itemAdd.getName());
			break;

		case	eIAR_CH5_REWARD_COPY	:
			CGamePlay::AddSystemMessage(WHITE,dMSG_COPY_ITEM_BY_CH5_REWARD_FORM,itemAdd.getName());
			break;
			
	}
}

//
//아이템 제거 -o-
void
cPACKET_HANDLER::ReceiveRemoveItem(SERVER_PACKETS *_lpPacket)
{
	SG_REMOVE_ITEM			*lpPacket	=	&_lpPacket->mSG_REMOVE_ITEM;

	if	(lpPacket->bf1IsSlot	==	FALSE)
	{
		g_hero.removeItemByBaseItemIndex(lpPacket->wItem,lpPacket->bf14Count,lpPacket->bf1IsIncludeEquipment);

		cBasicItem	*lpBasicItem	=	&g_aBasicItem[lpPacket->wItem];

		g_hero.buildPower();

		switch(lpPacket->wReason)
		{
			case	eIAR_NORMAL							:
				CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,lpPacket->bf14Count,lpBasicItem->m_strName);
				break;

			case	eRIR_STORE_TO_GUILD_INVENTORY	:
				CGamePlay::AddSystemMessage(WHITE,dMSG_STORE_ITEM_TO_GUILD_INVENTORY_FORM,lpBasicItem->m_strName);
				break;
				
			case	eRIR_DISJOINTING	:
				CGamePlay::AddSystemMessage(WHITE,dMSG_DISJOINTING_ITEM_SUCCESS);
				break;
		}

	}
	else
	{
		cItem	*lpItem	=	g_hero.getInventoryItem(lpPacket->wItem);

		if	(lpItem)
		{
			switch(lpPacket->wReason)
			{
				case	eIAR_NORMAL							:
					CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,lpPacket->bf14Count,lpItem->getName());
					break;

				case	eRIR_STORE_TO_GUILD_INVENTORY	:
					CGamePlay::AddSystemMessage(WHITE,dMSG_STORE_ITEM_TO_GUILD_INVENTORY_FORM,lpItem->getName());
					break;

				case	eRIR_DISJOINTING	:
					CGamePlay::AddSystemMessage(WHITE,dMSG_DISJOINTING_ITEM_SUCCESS);
					break;
			}

			lpItem->decreaseCount(lpPacket->bf14Count);
		}
	}
}

void
cPACKET_HANDLER::ReceiveAddGold(SERVER_PACKETS *_lpPacket)
{
	SG_ADD_GOLD				*lpPacket		=	&_lpPacket->mSG_ADD_GOLD;

	g_hero.m_iGold	+=	lpPacket->iAddGold;

	switch(lpPacket->wReason)
	{
		case	eAGR_NORMAL							:
			if	(lpPacket->iAddGold	>	0)
				CGamePlay::AddSystemMessage(WHITE,dMSG_RECEIVE_GOLD_BY_QUEST_FORM,CGamePlay::GetMoneyString(lpPacket->iAddGold));
			else
				CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_GOLD_BY_QUEST_FORM,CGamePlay::GetMoneyString(-lpPacket->iAddGold));
			break;

		case	eAGR_STORE_TO_GUILD_INVENTORY	:
			CGamePlay::AddSystemMessage(WHITE,dMSG_STORE_GOLD_TO_GUILD_INVENTORY_FORM,CGamePlay::GetMoneyString(-lpPacket->iAddGold));
			break;
		case	eAGR_WITHDRAW_FROM_GUILD_INVENTORY	:
			CGamePlay::AddSystemMessage(WHITE,dMSG_WITHDRAW_GOLD_FROM_GUILD_INVENTORY_FORM,CGamePlay::GetMoneyString(lpPacket->iAddGold));
			break;
	}
}

void
cPACKET_HANDLER::ReceiveSetItemUniqueSerial(SERVER_PACKETS *_lpPacket)
{
	SG_SET_ITEM_UNIQUE_SERIAL	*lpPacket	=	&_lpPacket->mSG_SET_ITEM_UNIQUE_SERIAL;

	if	(lpPacket->wSlot >= dBORDER_OF_ITEM_AND_EQUIPMENT)
		g_hero.m_aEquip[lpPacket->wSlot-dBORDER_OF_ITEM_AND_EQUIPMENT].m_dwSerial	=	lpPacket->dwSerial;
	else
	if	(lpPacket->wSlot >= dOWN_ITEM_COUNT)
		g_hero.m_aExtraInventory[lpPacket->wSlot-dOWN_ITEM_COUNT].m_dwSerial	=	lpPacket->dwSerial;
	else
		g_hero.m_aItems[lpPacket->wSlot].m_dwSerial	=	lpPacket->dwSerial;
}

//
//	
void
cPACKET_HANDLER::ReceiveUpdateItemData(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_ITEM_DATA		*lpPacket	=	&_lpPacket->mSG_UPDATE_ITEM_DATA;
	cItem					*lpItem		=	g_hero.getInventoryItem(lpPacket->wSlot,FALSE,TRUE);

	if	(lpPacket->wIsTwinkle)
		if	(lpPacket->wSlot < dBORDER_OF_ITEM_AND_EQUIPMENT)
			g_hero.addTwinkleItem(lpPacket->wSlot);

	if	(lpPacket->item.m_wBaseItem	==	0xffff)
		CGamePlay::AddSystemMessage(WHITE,dMSG_DISAPPEAR_ITEM_FORM,lpItem->getName());

	memcpy(lpItem,&lpPacket->item,sizeof(cItem));

	g_hero.buildPower();
}

void
cPACKET_HANDLER::ReceiveUpdateWayPointInfo(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_WAY_POINT_INFO	*lpPacket	=	&_lpPacket->mSG_UPDATE_WAY_POINT_INFO;

	g_hero.m_aWayPoint[lpPacket->wWayPointSlot].m_bf12Field	=	lpPacket->wField;
	g_hero.m_aWayPoint[lpPacket->wWayPointSlot].m_bf10XPos	=	lpPacket->wXPos;
	g_hero.m_aWayPoint[lpPacket->wWayPointSlot].m_bf10YPos	=	lpPacket->wYPos;
}


void
cPACKET_HANDLER::ReceiveCureActorByPotion(SERVER_PACKETS *_lpPacket)
{
	SG_CURE_ACTOR_BY_POTION		*lpPacket	=	&_lpPacket->mSG_CURE_ACTOR_BY_POTION;

	CActor	*lpActor	=	g_am.getTestedActor(lpPacket->wActor);

	lpActor->m_iHP		+=	lpPacket->iHealPoint;
}

void
cPACKET_HANDLER::ReceiveExpiredItem(SERVER_PACKETS *_lpPacket)
{
	SG_EXPIRED_ITEM				*lpPacket	=	&_lpPacket->mSG_EXPIRED_ITEM;

	g_hero.operateExpiredItem(lpPacket->wSlot,lpPacket->wItem);
}

void
cPACKET_HANDLER::ReceiveExpiredItemList(SERVER_PACKETS *_lpPacket)
{
	SG_EXPIRED_ITEM_LIST		*lpPacket	=	&_lpPacket->mSG_EXPIRED_ITEM_LIST;

	g_hero.operateExpiredItemList(lpPacket->wItemCount,(WORD*)lpPacket->awItemList);
}

void
cPACKET_HANDLER::ReceiveUseOrbResult(SERVER_PACKETS *_lpPacket)
{
	SG_USE_ORB_RESULT			*lpPacket	=	&_lpPacket->mSG_USE_ORB_RESULT;

	g_hero.m_bf1IsWaitTeleportResult	=	FALSE;

	switch(lpPacket->wResult)
	{
		case	eUOR_SUCCESS			:			//	성공
			break;

		case	eUOR_SUCCESS_WRITTEN	:
		{
			g_hero.m_aWayPoint[lpPacket->wSlot].m_bf12Field	=	lpPacket->wRememberField;
			g_hero.m_aWayPoint[lpPacket->wSlot].m_bf10XPos	=	lpPacket->wRememberXPos;
			g_hero.m_aWayPoint[lpPacket->wSlot].m_bf10YPos	=	lpPacket->wRememberYPos;
			break;
		}

		case	eUOR_CAN_NOT_WRITTEN_FIELD	:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UOR_CAN_NOT_WRITTEN_FIELD);
			break;
		}

		case	eUOR_CAN_NOT_USE_AT_THIS_FIELD	:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UOR_CAN_NOT_USE_AT_THIS_FIELD);
			break;
		}

		case	eUOR_INCORRECT_COMMAND	:	//	잘못된 명령이다.
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_INCORRECT_COMMAND);
			break;
		}

		case	eUOR_NOT_WRITTEN_ORB	:	//	기록 되지 않은 오브다
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_NOT_SETTING_WAY_POINT);
			break;
		}
	}
}

//
//	아이템 제거
void
cPACKET_HANDLER::ReceiveDestroyItemResult(SERVER_PACKETS *_lpPacket)
{
	SG_DESTROY_ITEM_RESULT	*lpPacket	=	&_lpPacket->mSG_DESTROY_ITEM_RESULT;

	switch(lpPacket->wResult)
	{
		case	eUIR_OK						:	//	성공
		{
			cItem	*lpItem	=	g_hero.getInventoryItem(lpPacket->wSlot);

			if (lpItem)
				lpItem->reset();

			g_hero.buildPower();
			break;
		}

		case	eUDIR_CAN_NOT_DESTROY_ITEM	:	//	파괴 불가 아이템
		{
			CGamePlay::WarningMessage(dMSG_CAN_NOT_DESTROY_ITEM);
			break;
		}

		case	eUDIR_VAILD_ITEM			:
			CGamePlay::WarningMessage(dMSG_ITEM_DATA_NOT_MATCHED_PLEASE_RETRY);
			s_agent.sendAskItemData();
			break;
	}
}

//	아이템 팩 이름
void
cPACKET_HANDLER::ReceiveItemPackName(SERVER_PACKETS *_lpPacket)
{
	SG_ITEM_PACK_NAME	*lpPacket	=	(SG_ITEM_PACK_NAME	*)_lpPacket;

	if (strlen(lpPacket->strPackName) <= 0)
		strcpy(g_astrItemPackListName[lpPacket->wPackIndex],"????????");
	else
		strcpy(g_astrItemPackListName[lpPacket->wPackIndex],lpPacket->strPackName);
}

//	결투 정보 받음
void
cPACKET_HANDLER::ReceiveDuelRecord(SERVER_PACKETS *_lpPacket)
{
	SG_DUEL_RECORD	*lpPacket	=	(SG_DUEL_RECORD	*)_lpPacket;

	memcpy(&g_hero.m_duelRecord,&lpPacket->record,sizeof(cDuelRecord));

	g_hero.m_bf1IsWaitToReceiveDuelRecord	=	FALSE;
	g_hero.m_bf1IsReceivedDuelRecord		=	TRUE;

	g_hero.m_wWeeklyDuelRank	=	lpPacket->wWeeklyRank	;
	g_hero.m_wDailyDuelRank		=	lpPacket->wDailyRank	;
	g_hero.m_wTrialGameRank		=	lpPacket->wTrialGameRank		;
}

//	미니펫 정보 업데이트

void
cPACKET_HANDLER::ReceiveMiniPetInfo(SERVER_PACKETS *_lpPacket)
{
	SG_MINI_PET_INFO	*lpPacket	=	(SG_MINI_PET_INFO	*)_lpPacket;

	int			iOldLevel=-1,iOldExp=-1;
	cMiniPet	*lpMiniPet	=	NULL;
	int			iOldMiniPetCount	=	g_hero.iMiniPet()->getMiniPetCount();

	if	(lpPacket->wPetIndex	<	c_iMiniPetCount)
	{
		lpMiniPet	=	&g_hero.m_aMiniPet[lpPacket->wPetIndex];

		iOldLevel	=	lpMiniPet->m_bf7Level;
		iOldExp		=	lpMiniPet->m_bf16Exp;

		memcpy(lpMiniPet,&lpPacket->miniPet,sizeof(lpPacket->miniPet));
	}

	g_hero.m_wIsReadyToFireChargedMiniPetSkill	=	lpPacket->bf1Charge1;
	g_hero.m_wIsReadyToFireChargedMiniPetSkill2	=	lpPacket->bf1Charge2;

	if	(lpPacket->wMethod	==	eUMIM_DISACTIVATE)
		g_hero.iMiniPet()->playFX(eMPFX_DISACTIVATE,lpPacket->wIncreaseExp);
	if	(lpPacket->wMethod	==	eUMIM_ACTIVATE)
		g_hero.iMiniPet()->playFX(eMPFX_ACTIVATE,lpPacket->bf5ActivePet);

	if	(lpPacket->wMethod	==	eUMIM_EXPIRED)
		CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CURRENT_MINIPET_GOTO_REST_BY_EXPIRED);

	switch(lpPacket->wMethod)
	{
		case	eUMIM_CREATE			:
		case	eUMIM_UNSEAL_MINIPET	:
		case	eUMIM_EXPIRED			:
			g_config.m_bf1IsShowMiniPetWindow	=	TRUE;
			break;
	}

	g_hero.m_wActiveMiniPet	=	lpPacket->bf5ActivePet;
	g_hero.m_wActiveMiniPet2=	lpPacket->bf5ActivePet2;
	g_hero.syncHeroApprearance();

	if	(lpPacket->wMethod	==	eUMIM_MIXED)
		g_msgBox.cPopup("",_ms(dMSG_CREATE_MINIPET_WITH_MIXER_FORM,g_aMiniPetTypeInfo[lpPacket->miniPet.m_bf5Type].m_strName),dMSG_OK);
	
	if	(lpPacket->wMethod	==	eUMIM_CREATE)
		CGamePlay::AddSystemMessage(WHITE,dMSG_CREATE_MINIPET_FORM,g_aMiniPetTypeInfo[lpPacket->miniPet.m_bf5Type].m_strName);

	if	(lpPacket->wMethod	==	eUMIM_RESET_MINIPET_NAME)
		CGamePlay::AddSystemMessage(WHITE,dMSG_RESET_SELECTED_MINIPET_NAME_THAN_ABLE_TO_CHANGE_NAME);

	if	(lpPacket->wMethod	==	eUMIM_INCREASE_SKILL_LEVEL)
		g_gwMiniPetStatus.receiveResult();

	if	(lpPacket->wMethod	==	eUMIM_FEEDING)
	{
		BOOL	bIsPlayed	=	FALSE;

		if	(iOldLevel	!=	-1)
		{
			CGamePlay::AddSystemMessage(WHITE,dMSG_INCREASE_MINIPET_EXP_FORM,lpPacket->wIncreaseExp);

			if	(lpMiniPet->m_bf7Level	>	iOldLevel)
			{
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_INCREASE_MINIPET_LEVEL);
				bIsPlayed	+=	g_hero.iMiniPet()->playFX(eMPFX_LEVEL_UP,lpPacket->wPetIndex);
			}
		}

		if	(bIsPlayed	==	0)
			g_hero.iMiniPet()->playFX(eMPFX_EAT_LQ_ITEM+lpPacket->bf4FeedType,lpPacket->wPetIndex);
	}

	if	(lpPacket->wMethod	==	eUMIM_FEED_AWAKEN_ITEM)
	{
		lpMiniPet->m_bf1MinipetAwaken50		= lpPacket->bf1IsUseMiniPetAwakenItem50;
		lpMiniPet->m_bf1MinipetAwaken100	= lpPacket->bf1IsUseMiniPetAwakenItem100;

		if(lpPacket->bf1IsUseMiniPetAwakenItem100)
			CGamePlay::AddSystemMessage(WHITE,dMSG_USE_MINI_PET_AWAKEN_ITEM100);
		else if(lpPacket->bf1IsUseMiniPetAwakenItem50)
			CGamePlay::AddSystemMessage(WHITE,dMSG_USE_MINI_PET_AWAKEN_ITEM50);
	}

	if	(lpPacket->wMethod	==	eUMIM_RESET_SKILL)
	{
		lpMiniPet->resetSkill(lpPacket->wPetIndex);
		CGamePlay::AddSystemMessage(WHITE,dMSG_RESET_MINIPET_SKILL);
	}
	if	(lpPacket->wMethod	==		eUMIM_FAIL_RESET_SKILL)
	{
		g_msgBox.cPopup("",dMSG_THERE_ARE_NOTHING_TO_RESET_ABLE_SKILL,dMSG_OK);
	}	
	if	(lpPacket->wMethod	==		eUMIM_CHANGE_TYPE)
	{
		lpMiniPet->resetSkill(lpPacket->wPetIndex);
		CGamePlay::AddSystemMessage(WHITE,dMSG_RESET_MINIPET_SKILL);
	}
	if	(lpPacket->bf1IsEvolution)
		CGamePlay::AddSystemMessage(WHITE,dMSG_EVLOUTION_MINIPET_FORM,g_aMiniPetTypeInfo[lpPacket->miniPet.m_bf5Type].m_strName);

	if	(iOldMiniPetCount	==	0)
		g_gwMiniPetStatus.open();
	else
		g_gwMiniPetStatus.modifyInfo();


	g_hero.buildPower();
}
//	접두사 제거
void
cPACKET_HANDLER::ReceivePrefix(SERVER_PACKETS *_lpPacket)
{
	SG_REMOVE_PREFIX	*lpPacket	=	(SG_REMOVE_PREFIX	*)_lpPacket;

	g_hero.removeItemPrefix(lpPacket->wUseItem,lpPacket->wDestItem,lpPacket->wPrefixIndex);

	g_gwRemovePrefix.close();
}

//
//	장비 벗어!!
void
cPACKET_HANDLER::StripEquipment(SERVER_PACKETS *_lpPacket)
{
	SG_STRIP_EQUIPMENT	*lpPacket	=	&_lpPacket->mSG_STRIP_EQUIPMENT;

	g_hero.m_aEquip[lpPacket->wPart].reset();
}

//
//	아이템 떨궈짐dSG_DROPPING_ITEM
void
cPACKET_HANDLER::DroppingItem(SERVER_PACKETS *_lpPacket)
{
	SG_DROPPING_ITEM	*lpPacket	=	&_lpPacket->mSG_DROPPING_ITEM;
	cDroppedItem		droppedItem;

	droppedItem.m_dwCount	=	lpPacket->droppedItem.m_wCount;
	droppedItem.m_wSerial	=	lpPacket->droppedItem.m_bf10Serial;
	droppedItem.m_wBaseItem	=	lpPacket->droppedItem.m_bf14BaseItem;
	droppedItem.m_wType		=	lpPacket->droppedItem.m_bf4Type;

	droppedItem.m_pos.x		=	lpPacket->droppedItem.wX;	//	어디에 떨어져 있냐?
	droppedItem.m_pos.y		=	lpPacket->droppedItem.wY;	//어디에 떨어져 있냐?

	g_fieldItem.insert(&droppedItem);
}

//
//떨어진 아이템들 제거 dSG_REMOVE_DROPPED_ITEMS
void
cPACKET_HANDLER::RemoveDroppedItems(SERVER_PACKETS *_lpPacket)
{
	SG_REMOVE_DROPPED_ITEMS	*lpPacket	=	&_lpPacket->mSG_REMOVE_DROPPED_ITEMS;

	for (int i=0;i<lpPacket->wCount;i++)	g_fieldItem.remove(lpPacket->aRemoveItem[i]);
}

//
//떨어진 아이템 제거 dSG_REMOVE_DROPPED_ITEM
void
cPACKET_HANDLER::RemoveDroppedItem(SERVER_PACKETS *_lpPacket)
{
	SG_REMOVE_DROPPED_ITEM	*lpPacket	=	&_lpPacket->mSG_REMOVE_DROPPED_ITEM;

	g_fieldItem.remove(lpPacket->wRemoveItem);
}

//
//아이템 떨궈짐dSG_CHANGE_DROPPED_ITEM_INFO
void
cPACKET_HANDLER::ChangeDroppedItemInfo(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_DROPPED_ITEM_INFO	*lpPacket	=	&_lpPacket->mSG_CHANGE_DROPPED_ITEM_INFO;

	cDroppedItem	*lpItem	=	g_fieldItem.get(lpPacket->wFieldItem);

	lpItem->m_dwCount	=	min(lpPacket->dwCount,0xffff);
}

//
//아이템 줍기 결과 dSG_PICK_ITEM_RESULT
void
cPACKET_HANDLER::PickItemResult(SERVER_PACKETS *_lpPacket)
{
	SG_PICK_ITEM_RESULT	*lpPacket	=	&_lpPacket->mSG_PICK_ITEM_RESULT;

	s_agent.setOperateItemData(FALSE);
	g_hero.m_iBookedPickItem		=	0xffff;

	switch(lpPacket->wResult)	//	결과
	{
		case	eUAIR_CAN_NOT_OWN_SAME_ITEM		:
		{
			g_msgBox.cPopup("",dMSG_CAN_NOT_OWN_SAME_ITEM,dMSG_OK);
			break;
		}

		case	eUAIR_TOO_MANY_BADGE_ITEM		:
			g_msgBox.cPopup("",_ms(dMSG_TOO_MANY_BADGE_ITEM_FORM,dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY),dMSG_OK);
			break;

		case	eUIR_OK							:	//	ok -o-!
		{
			cItem	*lpItem	=	(cItem *)&lpPacket->item;

			if	(lpItem->m_wBaseItem	==	dITEM_MONEY)
				CGamePlay::AddSystemMessage(WHITE,dMSG_PICK_GOLD_FORM,lpItem->m_dwSerial);
			else
				CGamePlay::AddSystemMessage(WHITE,dMSG_PICK_ITEM_FORM,lpItem->getName());

			g_hero.addItemToInventory(lpItem);
			break;
		}

		case	eUIR_WRONG_ITEM_DATA		:	//	얼래.. 뭔가 이상한디.. -_-a
			CGamePlay::AddSystemMessage(LTRED,dMSG_ITEM_DATA_NOT_MATCHED_PLEASE_RETRY);
			s_agent.sendAskItemData();
			break;

		case	eUPR_INVENTORY_FULL			:	//	인벤토리가 꽉 찼는디.. -o-
			CGamePlay::AddSystemMessage(LTRED,dMSG_PICK_ITEM_INVENTORY_FULL);
			break;

		case	eUPR_CAN_NOT_OWN_MORE_GOLD			:	//	인벤토리가 꽉 찼는디.. -o-
			CGamePlay::AddSystemMessage(LTRED,dMSG_OWN_GOLD_LIMIT);
			break;

		case	eUPR_PICK_ITEM_IS_PRIVATE_ITEM	:	//	임자 있는거야!!
			CGamePlay::AddSystemMessage(LTRED,dMSG_PICK_ITEM_IS_OWNER);
			break;

		case	eUPR_PICK_THROW_WEAPON1		:
			CGamePlay::AddSystemMessage(LTGREEN,dMSG_PICK_THROW_WEAPON);
			g_hero.m_bf1IsHideWeapon1	=	FALSE;
			g_hero.syncHeroApprearance();
			break;

		case	eUPR_PICK_THROW_WEAPON2		:
			CGamePlay::AddSystemMessage(LTGREEN,dMSG_PICK_THROW_WEAPON);
			g_hero.m_bf1IsHideWeapon2	=	FALSE;
			g_hero.syncHeroApprearance();
			break;

		case	eUPR_PICK_ITEM_IS_VALID_ITEM	:
			CGamePlay::AddSystemMessage(LTRED,dMSG_PICK_ITEM_IS_VALID);
			break;

		case	eUPR_PICK_ITEM_IS_PARTY_ITEM	:
			break;

		case	eUPR_QUEST_ITEM					:
			CGamePlay::AddSystemMessage(LTRED,dMSG_PICK_ERROR_QUEST_ITEM);
			break;

		case	eUPR_PICK_ITEM_IS_COPY_ITEM		:
			CGamePlay::AddSystemMessage(LTRED,dMSG_PICK_ERROR_COPY_ITEM);
			break;

		case	eUPR_CAN_NOT_PICK_ITEM_UNDER_DEATH_STATUS	:
			CGamePlay::AddSystemMessage(LTRED,dMSG_CAN_NOT_PICK_ITEM_UNDER_DEATH_STATUS);
			break;

		case	eUPR_FAILED_BY_BAD_STATUS	:
			CGamePlay::AddSystemMessage(LTRED,dMSG_CAN_NOT_ACTION_BY_BAD_STATUS);
			break;
	}

	g_fieldItem.releasePickItem();
}

//
//	파티 소유 골드를 줏었다.
void
cPACKET_HANDLER::ReceivePickPartyGold(SERVER_PACKETS *_lpPacket)
{
	SG_PCIK_PARTY_GOLD	*lpPacket	=	&_lpPacket->mSG_PCIK_PARTY_GOLD;

	g_hero.m_iGold		+=	lpPacket->iGold;

	CGamePlay::AddSystemMessage(CTBLUE,dMSG_PICK_PARTY_GOLD_FORM,lpPacket->iGold);
}

//아이템 내구력
void
cPACKET_HANDLER::ReceiveUpdateItemDurability(SERVER_PACKETS *_lpPacket)
{
	SG_UPDATE_ITEM_DURABILITY	*lpPacket	=	(SG_UPDATE_ITEM_DURABILITY *)_lpPacket;

	cItem		*lpItem			=	g_hero.getEquipment(lpPacket->bPart);

	if	(!lpItem)
		return;

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if (!lpBasicItem)
		return;

	if (lpBasicItem->getDurability()== lpPacket->bDurability)
	{
		CGamePlay::AddSystemMessage(LTGREEN,dMSG_REPAIR_BREAKDOWN_ITEM_FORM,lpItem->getName());
		//	수리됐어요~
	}
	else
	if (lpPacket->bDurability	<	lpItem->m_bDurability)
	{
		CGamePlay::AddSystemMessage(LTRED,dMSG_BREAKDOWN_ITEM_FORM,lpItem->getName());
		//	부숴졌어요~ 
	}

	lpItem->m_bDurability	=	lpPacket->bDurability;

	if	(lpItem->m_bDurability	==	0)
		g_hero.buildPower();
}

//
//아이템 장비하기 결과dSG_EQUIP_ITEM_RESULT
void
cPACKET_HANDLER::EquipItemResult(SERVER_PACKETS *_lpPacket)
{
	SG_EQUIP_ITEM_RESULT	*lpPacket	=	&_lpPacket->mSG_EQUIP_ITEM_RESULT;

	s_agent.setOperateItemData(FALSE);

	switch(lpPacket->wResult)	//	결과
	{
		case	eUIR_OK						:	//	ok -o-!
			g_hero.equipItem(lpPacket->wItemSlot,lpPacket->wPart);
			// 10.01.11 추가
			g_hero.changeQuickSlotEquipItem(lpPacket->wPart, lpPacket->wItemSlot);
			break;

		case	eUIR_WRONG_ITEM_DATA		:	//	얼래.. 뭔가 이상한디.. -_-a
			s_agent.sendAskItemData();
			break;

//		case	dITEM_EQUIP_RESULT_WRONG_PLACE		:	//	자리가 틀리잖아!!
//			g_lpHero->warning(dMSG_EQUIP_WRONG_PLACE);
//			break;
		case	eUER_BELT_NOT_FOUND					:	//	벨트가 없다.
			g_lpHero->warning(dMSG_BELT_NOT_FOUND);
			break;

		case	eUER_NOT_RING_PLACE					:	//	링 자리가 아니다.
			g_lpHero->warning(dMSG_WRONG_RING_PLACE);
			break;

		case	eUER_LACK_ABILITY					:	//	능력치가 모자란다.
			g_lpHero->warning(dMSG_LOW_POWER_FOR_EQUIP);
			break;

		case	eUER_NOT_EQUIPMENT_ITEM				:	//	착용 아이템이 아닙니다.
			g_lpHero->warning(dMSG_NOT_EQUIPMENT_ITEM);
			break;

		case	eUER_INVENTORY_FULL					:	//	인벤토리가 꽉 찼습니다.
			g_lpHero->warning(dMSG_INVENTORY_FULL);
			break;

		case	eUER_CAN_NOT_EQUIP_EQUIPMENT_WHEN_ACTION	:	//	전투중에 아이템을 착용 할 수 없어요.
			g_lpHero->warning(dMSG_CAN_NOT_STRIP_OR_EQUIP_EQUIPMENT_WHEN_ACTION);
			break;

		case	eUER_WRONG_PLACE					:
			g_lpHero->warning(dMSG_UER_WRONG_PLACE_ITEM);
			break;

		case	eUER_NOT_FIST_RING_PLACE			:
			s_agent.sendAskItemData();
			break;

		case	eUER_PICK_WEAPON					:
		case	eUSR_PLEASE_PICK_WEAPON				:
			g_lpHero->warning(dMSG_PLEASE_PICK_THROW_WEAPON);
			break;

		case	eUSR_CAN_NOT_CHANGE_SHIELD_STATUS	:
			CGamePlay::AddSystemMessage(LTRED,dMSG_CAN_NOT_CHANGE_SHIELD_STATUS);
			break;
		case	eUSR_CAN_NOT_CHANGE_WEAPON_STATUS	:
			CGamePlay::AddSystemMessage(LTRED,dMSG_CAN_NOT_CHANGE_WEAPON_STATUS);
			break;
		case eUER_WEAR_LIMIT					:		// 09.08.11
			g_lpHero->warning(dMSG_CAN_NOT_WEAR_ANYMORE);
			break;
	}

	// 10.01.14 
	g_hero.m_bUseSkillSlotItem = FALSE;

}

//
//	아이템 리로드 결과
void
cPACKET_HANDLER::ReceiveBeltItemReloadResult(SERVER_PACKETS *_lpPacket)
{
	SG_BELT_ITEM_RELOAD_RESULT	*lpPacket	=	&_lpPacket->mSG_BELT_ITEM_RELOAD_RESULT;

	s_agent.setOperateItemData(FALSE);

	switch(lpPacket->wResult)	//	결과
	{
		case	eUIR_OK						:	//	ok -o-!
			g_hero.m_bf1IsReloadingBeltItem	=	TRUE;
			break;

		case	eURR_FAILED					:	//	리로드 할 아이템이 없슈
			g_lpHero->warning(dMSG_NOT_EXIST_RELOAD_ABLE_ITEM);
			break;

		case	eUIR_WRONG_ITEM_DATA		:	//	얼래.. 뭔가 이상한디.. -_-a
			s_agent.sendAskItemData();
			break;
	}
}

//
//아이템 리로드
void
cPACKET_HANDLER::ReloadBeltItem(SERVER_PACKETS *_lpPacket)
{
	g_hero.reloadBeltSlot();
}


//
//아이템 사용 결과dSG_USE_ITEM_RESULT
void
cPACKET_HANDLER::UseItemResult(SERVER_PACKETS *_lpPacket)
{
	SG_USE_ITEM_RESULT	*lpPacket	=	&_lpPacket->mSG_USE_ITEM_RESULT;

	s_agent.setOperateItemData(FALSE);

	switch(lpPacket->wResult)	//	결과
	{
		case	eUIR_OK								://	성공!!
			g_hero.useItem(lpPacket->wItemSlot,lpPacket->wTarget,FALSE);
			break;

		case	eUIR_ROTTERY_TICKET_KWANG			://	꽝인디유... -o-
			g_hero.resetItem(lpPacket->wItemSlot);
			g_lpHero->warning(dMSG_ROTTERY_TICKET_KWANG);
			break;
		case	eUIR_BE_MANY_CHARACTER_IN_FIELD	:
			g_lpHero->warning(dMSG_BE_MANY_CHARACTER_IN_FIELD);
			break;

		case eUIR_CAN_NOT_USE_IN_FIELD	:
			g_lpHero->warning(dMSG_CAN_NOT_USE_THIS_FIELD_ITEM);
			break;
		default	:
			CGamePlay::OperateItemUseResult(lpPacket->wResult);
			break;
	}
}

//
//	아이템을 아이템에 사용한 결과
void
cPACKET_HANDLER::ReceiveUseItemToItemResult(SERVER_PACKETS *_lpPacket)
{
	SG_USE_ITEM_TO_ITEM_RESULT	*lpPacket	=	&_lpPacket->mSG_USE_ITEM_TO_ITEM_RESULT;

	s_agent.setOperateItemData(FALSE);

	switch(lpPacket->wResult)	//	결과
	{
		case	eUIR_SUCCESS_TO_USE_SEAL_SPHERE_OF_DAWN	:
		{
			g_hero.m_bf4UseSealSphereOfDawnCount++;	//	새벽의 봉인구 사용 횟수 증가
			CGamePlay::AddSystemMessage(LTGREEN,dMSG_SUCCESS_TO_USE_SEAL_SPHERE_OF_DAWN);
			break;
		}
		
		case	eCIR_CH5_REWARD_COPY_ITEM				:
		{
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			break;
		}

		case	eCIR_CH5_REWARD_BROKEN					:
		{
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);

			cItem	*lpItem	=	g_hero.getInventoryItem(lpPacket->bTargetItemSlot);

			if	(lpItem)
				lpItem->m_isBroken	=	TRUE;

			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_BROKEN_ITEM_BY_CH5_REWARD);
			break;
		}

		case	eRIPR_NOT_EXIST_PREFIX_ITEM				:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RIPR_NOT_EXIST_PREFIX_ITEM);
			break;
		case	eRIPR_IS_EXCLUSIVE_NORTMAL_ITEM			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RIPR_IS_EXCLUSIVE_NORTMAL_ITEM);
			break;
		case	eRIPR_IS_EXCLUSIVE_UNIQUE_ITEM			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RIPR_IS_EXCLUSIVE_UNIQUE_ITEM);
			break;

		case	eUIR_OK									:	//	ok -o-!
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			break;

		case	eUIR_SUCCESS_TO_WRITE_BOOK_PAGE			:
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			break;

		case	eUIR_CAN_NOT_USE_ON_DEST_ITEM		://	거기에다 사용 할 수 없다!!
			g_lpHero->warning(dMSG_CAN_NOT_USE_ITEM_ON_DEST_ITEM);
			break;

		case	eUIR_FAILED							://	실패.. -o-
			g_lpHero->warning(dMSG_CAN_NOT_USE_ITEM);
			break;

		case	eUIR_WRONG_ITEM_DATA				:
			s_agent.sendAskItemData();
			break;

		case	eUIR_NOT_EMBODY_ITEM				://	아직 구현이 안된 아이템
			g_lpHero->warning(dMSG_NOT_EMBODY_ITEM);
			break;

		case	eUIR_LACK_ABILITY					://	아직 구현이 안된 아이템
			g_lpHero->warning(dMSG_LACK_ABILITY_ITEM_TO_USE);
			break;

		case	eUIR_FAILED_EVENT20031101			:
			g_lpHero->warning(dMSG_ITEM_USE_FAILED_BY_EVENT20031101);
			break;

		case	eUJMER_OK									://	짐모리스의 인챈트 서비스 성공
			CGamePlay::AddSystemMessage(LTGREEN,dMSG_UJMER_OK									);
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			break;
		case	eUJMER_ITEM_BROKEN							://	짐모리스의 인챈트 서비스 실패
			CGamePlay::AddSystemMessage(LTRED,dMSG_UJMER_ITEM_BROKEN							);
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			break;
		case	eUJMER_ITEM_DESTROYED						://	완전히 망가졌다.
			CGamePlay::AddSystemMessage(LTRED,dMSG_UJMER_ITEM_DESTROYED						);
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			g_shaker.add(g_lpHero->m_pos.x,g_lpHero->m_pos.y,20,1000,0);
			break;

		case	eUJMER_FAILED								://	실패
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_FAILED								);
			break;
		case	eUJMER_CAN_NOT_ATTACH_PREFIX_BY_PREFIX_FULL	://	접두사가 꽉 차서 더 이상 접두사를 붙일 수 없다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_CAN_NOT_ATTACH_PREFIX_BY_PREFIX_FULL	);
			break;
		case	eUJMER_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	://	부숴진 아이템에 접두사를 붙일 수 없다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	);
			break;
		case	eUJMER_CAN_NOT_ATTACH_PREFIX_AT_CURSED_ITEM	://	저주 받은 아이템에는 접두사 추가 불가
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_CAN_NOT_ATTACH_PREFIX_AT_CURSED_ITEM	);
			break;
		case	eUJMER_CAN_NOT_ATTACH_PREFIX_ITEM			://	접두사를 붙일 수 없는 아이템이다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_CAN_NOT_ATTACH_PREFIX_ITEM			);
			break;
		case	eUJMER_TOO_LOW_LEVEL						://	짐모리스의 인챈트 서비스를 이용하기에는 레벨이 너무 낮다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_TOO_LOW_LEVEL						);
			break;
		case	eUJMER_IS_EXCLUSIVE_NOT_UNIQUE_ITEM			://	유니크 아이템이 아닌 아이템을 위한거다./
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_IS_EXCLUSIVE_NOT_UNIQUE_ITEM			);
			break;
		case	eUJMER_IS_EXCLUSIVE_UNIQUE_ITEM				://	유니크 아이템을 위한거다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_IS_EXCLUSIVE_UNIQUE_ITEM				);
			break;
		case	eUJMER_CAN_NOT_ATTACH_THAT_PREFIX_ITEM		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_CAN_NOT_ATTACH_THAT_PREFIX_ITEM				);
			break;

//	유니크 아이템에 인챈트 하기 결과
		case	eUEUIR_OK									://	유니크 아이템 인챈트 서비스 결과 성공
			CGamePlay::AddSystemMessage(LTGREEN,dMSG_UEUIR_OK									);
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			break;
		case	eUEUIR_ITEM_BROKEN							://	뽀게진다.
			CGamePlay::AddSystemMessage(LTRED,dMSG_UEUIR_ITEM_BROKEN							);
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			break;
		case	eUEUIR_ITEM_DESTROYED						://	완전히 망가졌다.
			CGamePlay::AddSystemMessage(LTRED,dMSG_UEUIR_ITEM_DESTROYED						);
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			g_shaker.add(g_lpHero->m_pos.x,g_lpHero->m_pos.y,20,1000,0);
			break;
		case	eUEUIR_CAN_NOT_ENCHANT_BY_PREFIX_FULL		://	유니크 아이템 인챈트 서비스 결과 접두사가 꽉 찼다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UEUIR_CAN_NOT_ENCHANT_BY_PREFIX_FULL		);
			break;
		case	eUEUIR_CAN_NOT_UPGRADE_PREFIX				://	더이상 접두사를 업그레이드 할 수 없다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UEUIR_CAN_NOT_UPGRADE_PREFIX				);
			break;
		case	eUEUIR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	://	부숴진 아이템에 접두사를 붙일 수 없다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UEUIR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	);
			break;
		case	eUEUIR_CAN_NOT_ATTACH_PREFIX_AT_CURSED_ITEM	://	저주 받은 아이템에는 접두사 추가 불가
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UEUIR_CAN_NOT_ATTACH_PREFIX_AT_CURSED_ITEM	);
			break;
		case	eUEUIR_CAN_NOT_ATTACH_PREFIX_ITEM			://	접두사를 붙일 수 없는 아이템이다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UEUIR_CAN_NOT_ATTACH_PREFIX_ITEM			);
			break;
		case	eUEUIR_CAN_NOT_ATTACH_THAT_PREFIX_ITEM		://	그 접두사를 붙일 수 없다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UEUIR_CAN_NOT_ATTACH_THAT_PREFIX_ITEM);
			break;

		case	eUEUIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM			://	유니크 아이템이 아닌 아이템을 위한거다./
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UEUIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM			);
			break;
		case	eUEUIR_IS_EXCLUSIVE_UNIQUE_ITEM				://	유니크 아이템을 위한거다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UEUIR_IS_EXCLUSIVE_UNIQUE_ITEM				);
			break;

		case	eRIR_OK										://	완벽히 성공
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RIR_OK										);
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			break;
		case	eRIR_OK_BUT_REMOVE_PREFIX					://	성공했다. 하지만 접두사가 사라졌다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RIR_OK_BUT_REMOVE_PREFIX					);
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			break;
		case	eRIR_FAILED									://	성공했다. 하지만 접두사가 사라졌다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RIR_FAILED									);
			break;

		case	eRIR_IS_USELESS_ITEM						:	//	접두사를 붙일 수 없는 아이템이 아니다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RIR_USELESS_ITEM);
			break;

		case	eRIR_IS_NOT_INCORRECT_ITEM					:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RIR_IS_NOT_INCORRECT_ITEM);
			break;

		case	eRIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM			://	유니크 아이템이 아닌 아이템을 위한거다./
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM			);
			break;
		case	eRIR_IS_EXCLUSIVE_UNIQUE_ITEM				://	유니크 아이템을 위한거다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RIR_IS_EXCLUSIVE_UNIQUE_ITEM				);
			break;

		case	eUCIR_SUCCESS								:	//	성공
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_EXTRA_ITEM_CHARGE_OK						);
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			break;
		case	eRDIR_SUCCESS								:	//	성공
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RDIR_SUCCESS						);
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			break;

		case	eRDIR_FAILED								:	//	실패
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RDIR_FAILED				);
			break;
		case	eRDIR_CAN_NOT_REPAIR_DEST_ITEM_BY_HIGH_DURABILITY	:	//	내구력이 50%를 넘어서 수리 안된다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RDIR_CAN_NOT_REPAIR_DEST_ITEM_BY_HIGH_DURABILITY				);
			break;
		case	eRDIR_DEST_ITEM_NOT_DX_ITEM					:	//	대상 아이템이 DX 아이템이 아니다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RDIR_DEST_ITEM_NOT_DX_ITEM				);
			break;

		case	eUCIR_FAILED								:	//	실패
			break;
		case	eUCIR_DEST_ITEM_NOT_EXTRA_ITEM				:	//	대상 아이템이 엑스트라 아이템이 아니다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UCIR_DEST_ITEM_NOT_EXTRA_ITEM);
			break;
		case	eUCIR_DEST_ITEM_CAN_NOT_CHARGEABLE			:	//	더 이상 충전 시킬 수 없다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UCIR_DEST_ITEM_CAN_NOT_CHARGEABLE);
			break;
		case	eUCIR_MISMATCH_EX_GRADE						:	//	등급이 맞지 않는다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UCIR_MISMATCH_EX_GRADE);
			break;

		case	eBWR_INCORRECT_BOOK_DATA				:	//	책이 아니다.
			break;
		case	eBWR_SOURCE_ITEM_IS_NOT_BOOK_PAGE		:	//	책 페이지가 아니다.
			break;
		case	eBWR_IS_NOT_DEFIEND_BOOK				:	//	등록된 책이 아니다.
			break;
		case	eBWR_DEST_ITEM_IS_NOT_BOOK				:	//	책이 아니다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_BWR_DEST_ITEM_IS_NOT_BOOK);
			break;
		case	eBWR_IS_NOT_SAME_BOOK					:	//	같은 책이 아니다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_BWR_IS_NOT_SAME_BOOK);
			break;
		case	eBWR_SOURCE_ITEM_IS_EXIST_BOOK_PAGE		:	//	이미 책 페이지가 등록되어 있다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_BWR_SOURCE_ITEM_IS_EXIST_BOOK_PAGE);
			break;
		case	eCAPR_OK									:	// 성공
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_OK);
			g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
			break;
		case	eCAPR_FAILED								:	// 실패!
			{

				cItem		*lpItem			=	g_hero.getInventoryItem(lpPacket->bItemSlot);
				cItem		*lpTargetItem	=	g_hero.getInventoryItem(lpPacket->bTargetItemSlot);

				if(lpItem)
				{
					CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,1,lpItem->getName());
					lpItem->decreaseCount(1);
				}
				if(lpTargetItem)
				{
					CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,1,lpTargetItem->getName());
					lpTargetItem->decreaseCount(1);
				}

				g_shaker.add(g_lpHero->m_pos.x,g_lpHero->m_pos.y,20,1000,0);
				g_msgBox.cPopup("",dMSG_UJMER_ITEM_DESTROYED,dMSG_OK);
				break;
			}
		case	eCAPR_EXIST_SAME_PREFIX						:	// 같은 접두사는 붙일수없어
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_EXIST_SAME_PREFIX);
			break;
		case	eCAPR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	:	// 부서진 아이템도 붙일수 없어
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM);
			break;
		case	eCAPR_CAN_NOT_ATTACH_PREFIX_ITEM			:	// 접두사를 붙일수없는 아이템이양
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UEUIR_CAN_NOT_ATTACH_THAT_PREFIX_ITEM	);
			break;
		case	eCAPR_PREFIX_FULL							:	//	더이상 접두사를 붙일수없네요
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_CAN_NOT_ATTACH_PREFIX_BY_PREFIX_FULL);
			break;
		case	eCAPR_CAN_NOT_ATTACH_PREFIX					:	// 그 접두사는 그 아이템에 붙일수없네요
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UEUIR_CAN_NOT_UPGRADE_PREFIX);
			break;
		case	eCAPR_STRANGE_ITEM							:	// 아이템정보가 이상하네?
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_ITEM_DATA_NOT_MATCHED_PLEASE_RETRY);
			break;
		case	ePCR_OK							:	// 잘라내기 성공
			{
				g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
				cItem	*lpDestItem	=	g_hero.getInventoryItem(lpPacket->bTargetItemSlot);
				if(lpDestItem)
				{				
					for	(int i=g_gwCopyOrCutPrefix.m_iSelectPrefix;i<dITEM_PREFIX_COUNT-1;i++)
						memcpy(&lpDestItem->m_aPrefix[i],&lpDestItem->m_aPrefix[i+1],sizeof(CItemPrefixInfo));	
					
					memset(&lpDestItem->m_aPrefix[dITEM_PREFIX_COUNT-1],0xff,sizeof(CItemPrefixInfo));				
				}
				g_hero.arrangeInventory();
				g_gwCopyOrCutPrefix.close();
				g_hero.OpenInventoryWindow();
				break;
			}
		case	ePCR_INVENTORY_FULL				:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UIR_FAILED_BY_VALID_INVENTORY_SLOT);
			break;
		case	ePCOPR_OK				:
			{		
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_OK);
				cItem		*lpItem			=	g_hero.getInventoryItem(lpPacket->bItemSlot);
				if(lpItem)
					CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,1,lpItem->getName());

				g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);	

				g_gwCopyOrCutPrefix.close();
				g_hero.OpenInventoryWindow();
				g_msgBox.cPopup("",dMSG_UJMER_OK,dMSG_OK);
				
				break;
			}
		case	ePCOPR_FAILED				:
			{
			
				
				cItem		*lpItem			=	g_hero.getInventoryItem(lpPacket->bItemSlot);
				cItem		*lpTargetItem	=	g_hero.getInventoryItem(lpPacket->bTargetItemSlot);
					
				if(lpItem)
				{
					CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,1,lpItem->getName());
					lpItem->decreaseCount(1);
				}
				if(lpTargetItem)
				{
					CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,1,lpTargetItem->getName());
					lpTargetItem->decreaseCount(1);
				}

				g_shaker.add(g_lpHero->m_pos.x,g_lpHero->m_pos.y,20,1000,0);
				g_gwCopyOrCutPrefix.close();
				g_hero.OpenInventoryWindow();
				
				g_msgBox.cPopup("",dMSG_UJMER_ITEM_DESTROYED,dMSG_OK);
				break;
			}
		case	ePCOPR_STRANGE_ITEM				:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_ITEM_DATA_NOT_MATCHED_PLEASE_RETRY);
			break;
		case	eCLR_INCREASE_SUCCESS							:
			{
				break;
			}
		case	eCLR_INCREASE_FAILED							:
			{
				
				g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
				cItem* lpItem = g_hero.getInventoryItem(lpPacket->bTargetItemSlot);
				CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,1,lpItem->getName());
				if(lpItem)
					lpItem->decreaseCount(1);
				
				g_hero.m_wBlackScreenCount = 2;
				g_msgBox.cPopup("",dMSG_CARVING_FAILED,dMSG_OK);
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CARVING_FAILED);
				
				break;
			}
		case eCLR_SET_SUCCESS:
			{
				cItem* lpItem = g_hero.getInventoryItem(lpPacket->bItemSlot);
				cItem* lpTargetItem = g_hero.getInventoryItem(lpPacket->bTargetItemSlot);
				
				if(!lpItem || !lpTargetItem)
					break;
				
				lpTargetItem->setCarvingLevel(lpItem->getSetCarvingLevel());
				g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
				g_msgBox.cPopup("",dMSG_CARVING_SUCCESS,dMSG_OK);
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CARVING_SUCCESS);
				
				break;
			}
		case eCLR_SET_FAILED:
			{
				g_hero.useItemToItem(lpPacket->bItemSlot,lpPacket->bTargetItemSlot);
				cItem* lpItem = g_hero.getInventoryItem(lpPacket->bTargetItemSlot);
				CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,1,lpItem->getName());
				if(lpItem)
					lpItem->decreaseCount(1);
				g_hero.m_wBlackScreenCount = 2;
				g_msgBox.cPopup("",dMSG_CARVING_FAILED,dMSG_OK);
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CARVING_FAILED);
				break;
			}
		case	eCLR_CAN_NOT_CARVE_TO_ITEM:
			{
				g_msgBox.cPopup("",dMSG_CAN_NOT_USE_ITEM_ON_DEST_ITEM,dMSG_OK);
				break;
			}
		case	eCLR_NOT_ENOUGH_TO_METERIALS:
			{
				g_msgBox.cPopup("",dMSG_NOT_MATERIALS,dMSG_OK);
				break;
			}
	}
}


//
//복권 아이템 당첨됨dSG_LOTTERY_TICKET_PRIZE_WINNING
void
cPACKET_HANDLER::ReceiveRotteryTicketPrizeWinning(SERVER_PACKETS *_lpPacket)
{
	SG_LOTTERY_TICKET_PRIZE_WINNING	*lpPacket	=	&_lpPacket->mSG_LOTTERY_TICKET_PRIZE_WINNING;

	cItem	*lpItem	=	g_hero.getInventoryItem(lpPacket->wItemSlot,FALSE,TRUE);

	lpItem->copy((cItem *)&lpPacket->itemPrize);

//	g_lpHero->warning(dMSG_ROTTERY_TICKET_PRIZE_WINNING);
	
	s_agent.setOperateItemData(FALSE);

	g_gwSelectItemInPack.close();
	CGamePlay::OpenInventoryWindow();

	g_hero.addTwinkleItem(lpPacket->wItemSlot);
}

//
//벨트 아이템 사용 결과dSG_USE_BELT_ITEM_RESULT
void
cPACKET_HANDLER::UseBeltItemResult(SERVER_PACKETS *_lpPacket)
{
	SG_USE_BELT_ITEM_RESULT	*lpPacket	=	&_lpPacket->mSG_USE_BELT_ITEM_RESULT;

	s_agent.setOperateItemData(FALSE);

	switch(lpPacket->wResult)	//	결과
	{
		case	eUIR_OK						:	//	ok -o-!
			g_hero.useItem(lpPacket->wItemSlot,lpPacket->wTarget,TRUE);
			break;

		case	eUIR_WRONG_ITEM_DATA		:	//	얼래.. 뭔가 이상한디.. -_-a
			s_agent.sendAskItemData();
			break;

		default	:
			CGamePlay::OperateItemUseResult(lpPacket->wResult);
			break;
	}
}

//
//벨트 아이템 사용 결과dSG_USE_ITEM
void
cPACKET_HANDLER::UseItem(SERVER_PACKETS *_lpPacket)
{
	SG_USE_ITEM	*lpPacket	=	&_lpPacket->mSG_USE_ITEM;

	cItem	item;

	memcpy(&item,&lpPacket->item,sizeof(lpPacket->item));

	g_am.attatchItemUseEffect(lpPacket->wWho,&item);

	if	(lpPacket->bf1IsUseEffectToHero)
		g_hero.useNormalItem(&item,TRUE);
	//	item을 사용하면 카운트를 줄인다.
	//	하지만 아이템명을 써야 하기 때문에 줄이면 안되서 벨트 아이템인척한다.(벨트 아이템은 카운트가 줄어도 따로 안한다.)

	if	(lpPacket->bf1IsDisplayMsg)
	{
		char*	lpstrItemName	=	item.getName();

		CGamePlay::AddSystemMessage(WHITE,dMSG_USE_ITEM_WITH_ACTOR_NAME_FORM,lpPacket->strCaster,lpstrItemName);
	}
}

void
cPACKET_HANDLER::OpenSelectItemInPackWindow(SERVER_PACKETS *_lpPacket)
{
	SG_ITEM_PACK_DATA	*lpPacket	=	(SG_ITEM_PACK_DATA	*)_lpPacket;

	g_gwSelectItemInPack.setData(lpPacket->wItem,&lpPacket->packInfo);
	g_gwSelectItemInPack.open();
}

void
cPACKET_HANDLER::OtherPlayerItemData(SERVER_PACKETS *_lpPacket)
{
	SG_OTHER_PLAYER_ITEM_DATA	*lpPacket	=	(SG_OTHER_PLAYER_ITEM_DATA	*)_lpPacket;

	g_hero.m_iGold					=	lpPacket->iGold;
	g_hero.m_bf6ExtraInventorySize	=	lpPacket->bf6ExtraInventorySize;
	g_hero.m_wItemSlotCount			=	dOWN_ITEM_COUNT;

	g_hero.receiveOtherPlayerItemData(lpPacket->strId,lpPacket->strName);

	int	i;

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
		g_hero.m_aEquip[i].reset();

	for (i=0;i<dOWN_ITEM_COUNT;i++)
		g_hero.m_aItems[i].reset();

	for (i=0;i<dEXTRA_INVENTORY_SIZE;i++)
		g_hero.m_aExtraInventory[i].reset();

	for (i=0;i<lpPacket->bf8ItemCount;i++)
	{
		cItemDataForRevise	*lpItem	=	&lpPacket->m_aItems[i];
		int					iSlot	=	lpItem->m_wSlot;

		if (iSlot	<	dOWN_ITEM_COUNT)
		{
			memcpy(&g_hero.m_aItems[iSlot],lpItem,sizeof(cItem));
			continue;
		}

		if (iSlot	<	dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE)
		{
			iSlot	-=	dOWN_ITEM_COUNT;

			memcpy(&g_hero.m_aExtraInventory[iSlot],lpItem,sizeof(cItem));

			if	(g_hero.m_aExtraInventory[11].m_wBaseItem	>=	10000)
				g_hero.m_aExtraInventory[11].m_wBaseItem	=	10000;

			continue;
		}

		if (iSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT	&&	iSlot	<	dBORDER_OF_ITEM_AND_EQUIPMENT+dEQUIPMENT_PART_COUNT)
		{
			iSlot	-=	dBORDER_OF_ITEM_AND_EQUIPMENT;

			memcpy(&g_hero.m_aEquip[iSlot],lpItem,sizeof(cItem));
			continue;
		}
	}
}

//
//아이템 벨트 아이템 사용 결과dSG_ITEM_DATA
void
cPACKET_HANDLER::ItemData(SERVER_PACKETS *_lpPacket)
{
	SG_ITEM_DATA	*lpPacket	=	&_lpPacket->mSG_ITEM_DATA;

	s_agent.setOperateItemData(FALSE);
	g_hero.m_iGold					=	lpPacket->iGold;
	g_hero.m_bf6ExtraInventorySize	=	lpPacket->wExtraInventorySize;
	g_hero.resetOtherPlayerItemData();

	int	i;

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
		g_hero.m_aEquip[i].reset();

	for (i=0;i<dOWN_ITEM_COUNT;i++)
		g_hero.m_aItems[i].reset();

	for (i=0;i<dEXTRA_INVENTORY_SIZE;i++)
		g_hero.m_aExtraInventory[i].reset();

	for (i=0;i<lpPacket->wCount;i++)
	{
		cItemDataForRevise	*lpItem	=	&lpPacket->m_aItems[i];
		int					iSlot	=	lpItem->m_wSlot;

		if (iSlot	<	dOWN_ITEM_COUNT)
		{
			memcpy(&g_hero.m_aItems[iSlot],lpItem,sizeof(cItem));
			continue;
		}

		if (iSlot	<	dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE)
		{
			iSlot	-=	dOWN_ITEM_COUNT;

			memcpy(&g_hero.m_aExtraInventory[iSlot],lpItem,sizeof(cItem));

			if	(g_hero.m_aExtraInventory[11].m_wBaseItem	>=	10000)
				g_hero.m_aExtraInventory[11].m_wBaseItem	=	10000;

			continue;
		}

		if (iSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT	&&	iSlot	<	dBORDER_OF_ITEM_AND_EQUIPMENT+dEQUIPMENT_PART_COUNT)
		{
			iSlot	-=	dBORDER_OF_ITEM_AND_EQUIPMENT;

			memcpy(&g_hero.m_aEquip[iSlot],lpItem,sizeof(cItem));
			continue;
		}
	}

	g_hero.buildPower();
}

//
//아이템 벨트 아이템 사용 결과dSG_ITEM_DATA
void
cPACKET_HANDLER::EquipmentData(SERVER_PACKETS *_lpPacket)
{
	SG_EQUIPMENT_DATA	*lpPacket	=	&_lpPacket->mSG_EQUIPMENT_DATA;

	s_agent.setOperateItemData(FALSE);

	memcpy(g_hero.m_aEquip,lpPacket->m_aEquip,sizeof(g_hero.m_aEquip));

	g_hero.buildPower();
}

//
//인벤토리 데이터만 받음
void
cPACKET_HANDLER::InventoryData(SERVER_PACKETS *_lpPacket)
{
	SG_INVENTORY_DATA	*lpPacket	=	&_lpPacket->mSG_INVENTORY_DATA;

	s_agent.setOperateItemData(FALSE);

	memcpy(g_hero.m_aItems,lpPacket->m_aItems,sizeof(g_hero.m_aItems));

	g_hero.buildPower();
}

//
//특정 캐릭터의 장비(외관에 변화가 있는)에 변화가 있다. dSG_CHANGE_APPEARANCE
void
cPACKET_HANDLER::ChangeAppearance(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_APPEARANCE	*lpPacket	=	&_lpPacket->mSG_CHANGE_APPEARANCE;
	CActor					*lpActor	=	g_am.getTestedActor(lpPacket->wSerial);

	if (!lpActor	||	lpActor->isHero())
		return;	//	내껀 내가 알아서 바꾼다.. -o-!!

	g_am.changeAppearance(lpPacket->wSerial,lpPacket);
}

//
//아이템 버리기 결과dSG_DROP_ITEM_RESULT
void
cPACKET_HANDLER::DropItemResult(SERVER_PACKETS *_lpPacket)
{
	SG_DROP_ITEM_RESULT	*lpPacket	=	&_lpPacket->mSG_DROP_ITEM_RESULT;

	s_agent.setOperateItemData(FALSE);
	g_hero.m_iBookedDroppingItemSlot	=	0xffff;

	switch(lpPacket->wResult)	//	결과
	{
		case	eUIR_OK						:	//	ok -o-!
			g_hero.removeItem(lpPacket->wItemSlot,lpPacket->dwItemCount);				//	버리기로 예약해둔 아이템 제거
			break;

		case	eUIR_WRONG_ITEM_DATA		:	//	얼래.. 뭔가 이상한디.. -_-a
			s_agent.sendAskItemData();
			break;

		case	eUDR_FIELD_FULL				:	//	사용할 수 없다. -o-
			g_lpHero->warning(dMSG_CAN_NOT_DROP_ITEM_FIELD_FULL);
			break;
			
		case	eUDR_CAN_NOT_DROP_ITEM		:	//	사용할 수 없다. -o-
			g_lpHero->warning(dMSG_CAN_NOT_DROP_ITEM);
			break;

		case	eUDR_CAN_NOT_STRIP_EQUIPMENT_WHEN_ACTION	:	//	행동중 장비 벗기 불가
			g_lpHero->warning(dMSG_CAN_NOT_STRIP_OR_EQUIP_EQUIPMENT_WHEN_ACTION);
			break;

		case	eUDR_PLEASE_PICK_WEAPON		:	
			CGamePlay::AddSystemMessage(LTRED,dMSG_PLEASE_PICK_THROW_WEAPON);
			break;
	}
}

//
//아이템 교환
void
cPACKET_HANDLER::ChangeItemPlaceResult(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_ITEM_PLACE_RESULT	*lpPacket	=	&_lpPacket->mSG_CHANGE_ITEM_PLACE_RESULT;

	s_agent.setOperateItemData(FALSE);

	switch(lpPacket->wResult)	//	결과
	{
		case	eUIR_OK						:	//	ok -o-!
			g_hero.changeItemPlace(lpPacket->wSrcSlot,lpPacket->wDestSlot);				//	버리기로 예약해둔 아이템 제거
			break;

		case	eUIR_WRONG_ITEM_DATA		:	//	얼래.. 뭔가 이상한디.. -_-a
			s_agent.sendAskItemData();
			break;
	}
}