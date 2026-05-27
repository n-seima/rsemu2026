#include <time.h>
#include "cFIELD.H"
#include "cGAME.H"
#include "CEvent.H"
#include "packetManager.H"
#include "cGuild.H"

//
//NPC 선택
void
cFIELD::selectNpc(CG_SELECT_NPC *_lpPacket,int _iSerial)
{
	cACTOR	*lpTarget	=	getTestedActor(_lpPacket->wNpcSerial);
	cACTOR	*lpAsker	=	getTestedActor(_iSerial);
	
	if	(!lpAsker || !lpTarget)
		return;

	if	(!lpAsker->isTargetInAttackRange(lpTarget,dEVENT_RANGE_WITH_NPC))
	{
		SG_EVENT_FAILED		packet;

		packet.base.set(sizeof(SG_EVENT_FAILED),dSG_EVENT_FAILED);

		packet.wOwner	=	_lpPacket->wNpcSerial;
		packet.wReason	=	eREF_TOO_FAR;

		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);
		
		return;
	}

	if	(!lpTarget->isNpc())
		return;//	 NPC와의 이벤트를 요청했는데.. 대상이 NPC가 아니다.

	if	(!lpTarget->eventWith(lpAsker))
	{
		SG_EVENT_FAILED		packet;

		packet.base.set(sizeof(SG_EVENT_FAILED),dSG_EVENT_FAILED);

		packet.wOwner	=	_lpPacket->wNpcSerial;
		packet.wReason	=	eREF_NOT_OWNED_EVENT;

		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);
	}
}

void
cFIELD::summonEventMonster(CG_SUMMON_EVENT_MONSTER *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)	
		return;

	if(!lpActor->isOperator())
		return;

	if(m_iCharacterCount >= dCHARACTER_COUNT )
		return;
	if(m_iSavedActorCount >= dMAX_NPC_COUNT)
		return;
	if(_lpPacket->wIndex >= dMAX_JOB_COUNT)
		return;

	for (int i=m_iSavedActorCount;i<dMAX_NPC_COUNT;++i)
	{
		cNPC*	lpNpc	=	m_apNpcGenerateData[i];

		if(!lpNpc)
		{

			m_apNpcGenerateData[i] = new cNPC;
			cNPC*	lpAddNpc = m_apNpcGenerateData[i];

			if(lpAddNpc)
			{

				cCHARACTER	*lpCharacter		= NULL;
				int iCharacterSerial = 0;
				for(int j=0;j<dCHARACTER_COUNT ;++j)
				{
					if(m_aCharacter[j].m_wSerial == 0xffff)
					{
						lpCharacter = &m_aCharacter[j];
						iCharacterSerial = j;
						break;
					}
				}

				if(!lpCharacter)
					return;

				if(lpCharacter->m_wSerial != 0xffff)
					return;

	
				memset(lpCharacter,0,sizeof(CCharacterBaseInfo));	
				lpCharacter->m_wBaseJob				=	_lpPacket->wIndex ;

				cJOB* lpJob = lpCharacter->getJob();
				if(!lpJob)
				{
					lpCharacter->m_wBaseJob = 0xffff;
					return;
				}
				lpCharacter->m_wSerial				=	iCharacterSerial;
				lpCharacter->m_wCorrectHP			=	_lpPacket->bf10CorrectHP ;
				lpCharacter->m_wCorrectAP			=	_lpPacket->bf10CorrectAP;
				lpCharacter->m_wMaxLevel			=	_lpPacket->bf10MaxLevel ;
				lpCharacter->m_wMinLevel			=	_lpPacket->bf10MinLevel ;
				lpCharacter->m_wScaleX				=	_lpPacket->bf8ScaleX ;
				lpCharacter->m_wScaleY				=	_lpPacket->bf8ScaleY ;
				lpCharacter->m_wCorrectExp			=	_lpPacket->bf10CorrectEXP;
				lpCharacter->m_bf1IsBlockToTame		=	TRUE;
				lpCharacter->m_wCorrectFinalDamage	=	_lpPacket->bf8CorrectFinalDamage * 100;
				lpCharacter->m_bf1IsNotDropGold		=	_lpPacket->bf1IsDefaultGoldDrop;
				lpCharacter->m_bf1IsNotDropItem		=	_lpPacket->bf1IsDefaultItemDrop;
				lpCharacter->m_bf1IsNotExp			=	FALSE;
				lpCharacter->m_bf1IsImmunePhysicalDamage	=	_lpPacket->m_bf1IsImmunePhysicalDamage;
				lpCharacter->m_bf1IsImmuneMagicDamage		=	_lpPacket->m_bf1IsImmuneMagicDamage;
				memset(lpAddNpc,0,sizeof(cBASE_NPC_INFO));

				lpAddNpc->reset();
				lpAddNpc->m_wIsBlockToAutoRegen = TRUE;
				lpAddNpc->m_wIsChangeDirect = TRUE;
				strcpy(lpAddNpc->m_strName, _lpPacket->strName);
				lpAddNpc->m_wActorKind	= eAK_MONSTER;
				lpAddNpc->m_wPatrolRoute	=	0xffff;
				lpAddNpc->m_wWanderCycle	=	30;
				lpAddNpc->m_wCharacter	= lpCharacter->m_wSerial;
				lpAddNpc->m_iSerial = i;
				lpAddNpc->m_wDirect = random(8);

				++m_iCharacterCount;	
				++m_iSavedActorCount;
				++m_iSaveMonsterCount;

				cACTOR* lpSummonActor = 	generateNpc(lpAddNpc,FALSE, FALSE, lpActor->m_pos.x,lpActor->m_pos.y);	

				if(lpSummonActor)
				{
					lpSummonActor->m_bf1IsEventSummonMonster = TRUE;
					lpSummonActor->m_dropInfoOfSummonEventMonster.m_wDropChance =	_lpPacket->bf8DropItemChance;
					lpSummonActor->m_dropInfoOfSummonEventMonster.m_wDropItem	=	_lpPacket->wDropItemIndex;
					lpSummonActor->m_dropInfoOfSummonEventMonster.m_wKind 		=	_lpPacket->bf4DropKind;
					lpSummonActor->m_dropInfoOfSummonEventMonster.m_wDropCount	=	_lpPacket->bf7DropItemCount;
					lpSummonActor->m_dropInfoOfSummonEventMonster.m_bIsOwner	=	_lpPacket->bf1IsOwner;
					memcpy(lpSummonActor->m_dropInfoOfSummonEventMonster.m_aiOption ,_lpPacket->aiOption,sizeof(lpSummonActor->m_dropInfoOfSummonEventMonster.m_aiOption));

					if(_lpPacket->bf8MagicResistance )
					{
						lpSummonActor->m_fireResistance.m_sFixValue 	= _lpPacket->bf8MagicResistance;			
						lpSummonActor->m_waterResistance.m_sFixValue	= _lpPacket->bf8MagicResistance;			
						lpSummonActor->m_windResistance.m_sFixValue		= _lpPacket->bf8MagicResistance;			
						lpSummonActor->m_earthResistance.m_sFixValue	= _lpPacket->bf8MagicResistance;			
						lpSummonActor->m_lightResistance.m_sFixValue	= _lpPacket->bf8MagicResistance;			
						lpSummonActor->m_darkResistance.m_sFixValue		= _lpPacket->bf8MagicResistance;			
						lpSummonActor->m_allMagicResistance.m_sFixValue	= _lpPacket->bf8MagicResistance;		
					}
					if(_lpPacket->bf8StrangeResistance)
					{
						lpSummonActor->m_bFreezeResistance				= _lpPacket->bf8StrangeResistance;		//	상태 이상 저항
						lpSummonActor->m_bColdResistance				= _lpPacket->bf8StrangeResistance;		//	상태 이상 저항
						lpSummonActor->m_bBlindResistance				= _lpPacket->bf8StrangeResistance;		//	상태 이상 저항
						lpSummonActor->m_bConfuseResistance				= _lpPacket->bf8StrangeResistance;		//	상태 이상 저항
						lpSummonActor->m_charmingResistance.m_sFixValue = _lpPacket->bf8StrangeResistance;
						lpSummonActor->m_bStoneResistance				= _lpPacket->bf8StrangeResistance;		//	상태 이상 저항
						lpSummonActor->m_bPoisonResistance				= _lpPacket->bf8StrangeResistance;		//	상태 이상 저항
						lpSummonActor->m_bSleepResistance				= _lpPacket->bf8StrangeResistance;		//	상태 이상 저항
						
						lpSummonActor->m_bCriticalHitResistance			= _lpPacket->bf8StrangeResistance;		//	크리티컬 저항
						lpSummonActor->m_bCrushBlowResistance			= _lpPacket->bf8StrangeResistance;		//	크러쉬 블로우 저항
						lpSummonActor->m_bKnockBackResistance			= _lpPacket->bf8StrangeResistance;		//	녹아웃 저항
						lpSummonActor->m_bInstanceKillResistance		= _lpPacket->bf8StrangeResistance;		//	즉사 저항
					}
				}
				break;
			}
		}
	}
}
//
//	타겟을 체크하고 죽었으면 킬 메시지를.. 없으면 리무브 메시지를 날려준다.
void
cFIELD::selectSpeech(CG_SELECT_SPEECH *_lpPacket,int _iSerial)
{
	g_debugSign.m_iTempStep	=	0;

	cACTOR	*lpTarget	=	getTestedActor(_lpPacket->wSpeaker);
	cACTOR	*lpAsker	=	getTestedActor(_iSerial);

	if (!lpTarget || !lpAsker)
		return;
	
	if (_lpPacket->wEventArea	!=	0xffff)
	{
		cAreaInfo	*lpArea	=	getArea(_lpPacket->wEventArea);

		if (!lpArea)
			return;

		lpArea->selectSpeech(lpAsker,_lpPacket->wSpeechIndex,_lpPacket->wSelectSpeech);
	}
	else
		lpTarget->selectSpeech(lpAsker,_lpPacket->wSpeechIndex,_lpPacket->wSelectSpeech);

	g_debugSign.m_iTempStep	=	-1;
}

//
//	아이템 팔기 실패 메시지를 플레이어에게 전송
void
cFIELD::sendFailedSellItem(cACTOR *_lpActor,int _iReason)
{
	SG_FAILED_SELL_ITEM	packetFailed;

	packetFailed.base.set(sizeof(SG_FAILED_SELL_ITEM),dSG_FAILED_SELL_ITEM);
	packetFailed.wReason	=	_iReason;

	g_userPM.add(_lpActor->m_iClientSerial,&packetFailed,packetFailed.base.wSize);
}


//
//	아이템 구입
void
cFIELD::buyGuildShopItem(CG_BUY_ITEM *_lpPacket,int _iSerial)
{
	cACTOR	*lpClient	=	getTestedActor(_iSerial);

	if	(!lpClient)
		return;

	CGuild	*lpGuild	=	lpClient->getGuild();

	if	(!lpGuild	||	lpGuild->m_wHallLevel	==	0)
		return;

	if	(!lpGuild->isSellItem(_lpPacket->wSelectItem))
		return;

	if	(g_bIsTestServer	==	FALSE)
		if	(m_bIs1LevelGuildHall	==	FALSE)
			if	(m_wHallOwnGuild	!=	lpGuild->m_wSerial)
				return;

	cItem	itemSell;

	g_im.generateItemByBaseItem(&itemSell,_lpPacket->wSelectItem,1);

	if	(itemSell.m_wBaseItem	==	0xffff)
		return;

	if	(_lpPacket->wBuyCount	>	lpClient->getRemainInventorySlotCount())
	{
		BOOL	bIsStackAble	=	FALSE;

		if	(itemSell.isStackAbleItem()	&&	_lpPacket->wBuyCount	==	1)
		{
			cItem	*lpStackItem	=	lpClient->getItemByBasicItem(itemSell.m_wBaseItem);

			if	(lpStackItem	&&	lpStackItem->getStackLimit() > itemSell.m_bCount+lpStackItem->m_bCount)
				bIsStackAble	=	TRUE;
		}

		if	(bIsStackAble	==	FALSE)
		{
			lpClient->sendFailedBuyItem(eFRBI_INVENTORY_FULL);
			return;
		}
	}

	itemSell.m_dwSerial		=	itemSell.getPrice(1);

	LONGLONG	llPrice		=	itemSell.m_dwSerial;
	llPrice					*=	_lpPacket->wBuyCount;

	if	((DWORD)lpClient->m_iGold	<	llPrice)	//	돈이 모자라네.. -o-
	{
		lpClient->sendFailedBuyItem(eFRBI_LACK_OF_GOLD);
		return;
	}

	{
//	길드 홀 수수료 걷기
		LONGLONG	llCharge	=	llPrice;
		llCharge				=	llCharge*lpGuild->m_wHallLevel/100;

		int	iCharge				=	(int)llCharge;

		g_game.sendWPAddGuildGold(lpGuild->m_wSerial,iCharge);
	}

	SERVERStoWORLD_CREATE_SELL_ITEM	packet;

	packet.base.set(sizeof(SERVERStoWORLD_CREATE_SELL_ITEM),dSERVERStoWORLD_CREATE_SELL_ITEM);

	packet.wIsDuelPointShop	=	FALSE;
	packet.iSerialInServer	=	lpClient->m_iZoneSerial;	//	고객 -o-
	packet.dwPrice			=	itemSell.m_dwSerial;		//	가격
	packet.wCount			=	_lpPacket->wBuyCount;
	packet.wWhere			=	eBUY_ITEM_IN_GUILD_SHOP;
	packet.wIsTokenShop	=	0xffff;// 토큰상점 아님..
	strcpy(packet.strName,lpClient->m_strName);				//	이름
	memcpy(&packet.item,&itemSell,sizeof(cItem));			//	판매할 아이템

	SEND_WOLRD_PACKET(&packet);
}

void
cFIELD::buyBadgeOfTraderShopItem(CG_BUY_ITEM *_lpPacket,int _iSerial)
{
	cACTOR	*lpClient	=	getTestedActor(_iSerial);

	if	(!lpClient)
		return;

	cItem	itemSell;
	itemSell.reset();
	if(m_shop.getIsBadgeOfTraderItemOption(_lpPacket->wSelectItem))
	{
		int iOption[4];
		iOption[0] = m_shop.getBadgeOfTraderItemOption1Min(_lpPacket->wSelectItem);
		iOption[1] = m_shop.getBadgeOfTraderItemOption1Max(_lpPacket->wSelectItem);
		iOption[2] = m_shop.getBadgeOfTraderItemOption2Min(_lpPacket->wSelectItem);
		iOption[3] = m_shop.getBadgeOfTraderItemOption2Max(_lpPacket->wSelectItem);
		g_im.generateItemByBaseItem(&itemSell , m_shop.getBadgeOfTraderItemSerial(_lpPacket->wSelectItem) , m_shop.getBadgeOfTraderItemCount(_lpPacket->wSelectItem), -1, iOption);
	}
	else
	{
		g_im.generateItemByBaseItem(&itemSell , m_shop.getBadgeOfTraderItemSerial(_lpPacket->wSelectItem) , m_shop.getBadgeOfTraderItemCount(_lpPacket->wSelectItem));
	}
	

	if	(itemSell.m_wBaseItem	==	0xffff)	//	그런 아이템
	{
		lpClient->sendFailedBuyItem(eFRBI_NOT_EXIST_ITEM);
		return;
	}
	LONGLONG	llPrice						=	m_shop.getBadgeOfTraderItemPrice(_lpPacket->wSelectItem);


	cBasicItem*	lpBasicitem = itemSell.getBasicItem(TRUE);

	DWORD		PriceForLevelPerGoldShop	=	0;
	
	if(!lpBasicitem)
		PriceForLevelPerGoldShop	=	1;		//베이스 아이템이 없는 아이템이면 1원으로 세팅한다.
	else
		PriceForLevelPerGoldShop	=	lpBasicitem->m_dwPrice;	//레벨당 돈내는 상점을 위해서 생성. 다른 보너스 효과 일체 무시한다.
	

	if	(_lpPacket->wBuyCount	>	lpClient->getRemainInventorySlotCount())
	{
		BOOL	bIsStackAble	=	FALSE;

		if	(itemSell.isStackAbleItem()	&&	_lpPacket->wBuyCount	==	1)
		{
			cItem	*lpStackItem	=	lpClient->getItemByBasicItem(itemSell.m_wBaseItem);

			if	(lpStackItem	&&	lpStackItem->getStackLimit() > itemSell.m_bCount+lpStackItem->m_bCount)
				bIsStackAble	=	TRUE;
		}

		if	(bIsStackAble	==	FALSE)
		{
			lpClient->sendFailedBuyItem(eFRBI_INVENTORY_FULL);
			return;
		}
	}
	
	llPrice	-=	llPrice*lpClient->m_wCourtesyTitleValue/10000;
	
	if	(m_wIsGuildHall)
	{
		CGuild	*lpGuild	=	lpClient->getGuild();

		if	(lpGuild)
			llPrice	-=	llPrice*lpGuild->m_wDecreaseGuildShopItemPrice/100;
	}
	else if(_lpPacket->wIsTokenShop  == 0xffff)
	{
		llPrice	+=	llPrice*lpClient->getTradeBonusByA()/100;
	}

	itemSell.m_dwSerial	=	(DWORD)llPrice;
	
	g_eventManager.operateItemByItemSaleEvent(&itemSell);

//	유효기간 지난 아이템은 판매 안한다.
	{
		cBasicItem	*lpBasicItem	=	itemSell.getBasicItem();
		int		iExpireTimeValue	=	lpBasicItem->m_wExpireYear*1000000  +lpBasicItem->m_wExpireMonth*10000  +lpBasicItem->m_wExpireDay*100  +lpBasicItem->m_wExpireHour;
		int		iCurrentTimeValue	=	g_currentTime.m_wYear*1000000		+g_currentTime.m_wMonth*10000		+g_currentTime.m_wDay*100		+g_currentTime.m_wHour;

		if	(iExpireTimeValue != 0 && iCurrentTimeValue > iExpireTimeValue)
		{
			lpClient->sendFailedBuyItem(eFRBI_UNKONWN);

			return;
		}
	}

	if	((DWORD)lpClient->m_iGold	<	llPrice)	//	돈이 모자라네.. -o-
	{
		lpClient->sendFailedBuyItem(eFRBI_LACK_OF_GOLD);
		return;
	}


	if (_lpPacket->wIsTokenShop  == 0xffff)			//토큰 샵이 아니면.세금을 걷는다.
	{
		CGuild	*lpGuild	=	g_guildManager.getBySerial(m_wHallOwnGuild);

		if	(lpGuild	&&	lpGuild->m_wHallLevel)
		{
	//	길드 홀 수수료 걷기
			LONGLONG	llCharge	=	llPrice;
			llCharge				=	llCharge*lpGuild->m_wHallLevel/100;

			int	iCharge				=	(int)llCharge;

			g_game.sendWPAddGuildGold(lpGuild->m_wSerial,iCharge);
		}
	}

	itemSell.m_dwSerial	=	max(itemSell.m_dwSerial,1);

	llPrice				=	itemSell.m_dwSerial;
	llPrice				*=	_lpPacket->wBuyCount;

	SERVERStoWORLD_CREATE_SELL_ITEM	packet;

	packet.base.set(sizeof(SERVERStoWORLD_CREATE_SELL_ITEM),dSERVERStoWORLD_CREATE_SELL_ITEM);

	packet.wIsDuelPointShop	=	FALSE;
	packet.iSerialInServer	=	lpClient->m_iZoneSerial;	//	고객 -o-
	packet.dwPrice			=	itemSell.m_dwSerial;		//	가격
	packet.wCount			=	_lpPacket->wBuyCount;
	packet.wWhere			=	eBUY_ITEM_IN_BADGE_OF_TRADER_SHOP;
	packet.wIsTokenShop	=	0xffff;// 토큰상점 아님..
	strcpy(packet.strName,lpClient->m_strName);				//	이름
	memcpy(&packet.item,&itemSell,sizeof(cItem));			//	판매할 아이템

	SEND_WOLRD_PACKET(&packet);

}
//
//	아이템 구입
void
cFIELD::buyItem(CG_BUY_ITEM *_lpPacket,int _iSerial)
{
	if	(_lpPacket->wIsGuildShop)
	{
		buyGuildShopItem(_lpPacket,_iSerial);

		return;
	}
	if	(_lpPacket->bf1IsBadgeOfTraderShop)
	{	// 거래상인의 배지상점.
		buyBadgeOfTraderShopItem(_lpPacket, _iSerial);
		return ;
	}

	if	(!checkTargetAndKill(_iSerial,_lpPacket->wOwner))
		return;

	cACTOR	*lpShopKeeper=	getActor(_lpPacket->wOwner);
	cACTOR	*lpClient	=	getActor(_iSerial);
	cNPC	*lpNpc		=	lpShopKeeper->getNpc();

	if	(!lpNpc)	//	NPC가 있는 넘이 아니잖어!!
	{
		lpClient->sendFailedBuyItem(eFRBI_UNKONWN);
		return;
	}

	if	(!lpNpc->isOwnedShop(_lpPacket->wShopSerial))	//	이 상점을 가지고 있냐?
	{
		lpClient->sendFailedBuyItem(eFRBI_NPC_NOT_OWN_THAT_SHOP);
		return;
	}

	if	(!lpClient->isTargetInAttackRange(lpShopKeeper,dEVENT_RANGE_WITH_NPC))
	{
		lpClient->sendFailedBuyItem(eFRBI_TOO_FAR_DISTANCE);
		return;
	}

	CShop	*lpShop	=	m_shop.get(_lpPacket->wShopSerial);

	if	(!lpShop)	//	그런 상점 없셔!!
	{
		lpClient->sendFailedBuyItem(eFRBI_NOT_EXIST_SHOP);
		return;
	}

	if	(lpClient->m_wShopSerial	!=	_lpPacket->wShopSerial)
		return;

	cItem	itemSell;

	itemSell.reset();
	lpShop->getSellItemInfo(_lpPacket->wSelectItem,&itemSell);

	if	(itemSell.m_wBaseItem	==	0xffff)	//	그런 아이템
	{
		lpClient->sendFailedBuyItem(eFRBI_NOT_EXIST_ITEM);
		return;
	}

	LONGLONG	llPrice						=	itemSell.m_dwSerial;

	cBasicItem*	lpBasicitem = itemSell.getBasicItem(TRUE);

	DWORD		PriceForLevelPerGoldShop	=	0;

	if(!lpBasicitem)
		PriceForLevelPerGoldShop	=	1;		//베이스 아이템이 없는 아이템이면 1원으로 세팅한다.
	else
		PriceForLevelPerGoldShop	=	lpBasicitem->m_dwPrice;	//레벨당 돈내는 상점을 위해서 생성. 다른 보너스 효과 일체 무시한다.

	if	(_lpPacket->wBuyCount	>	lpClient->getRemainInventorySlotCount())
	{
		BOOL	bIsStackAble	=	FALSE;

		if	(itemSell.isStackAbleItem()	&&	_lpPacket->wBuyCount	==	1)
		{
			cItem	*lpStackItem	=	lpClient->getItemByBasicItem(itemSell.m_wBaseItem);

			if	(lpStackItem	&&	lpStackItem->getStackLimit() > itemSell.m_bCount+lpStackItem->m_bCount)
				bIsStackAble	=	TRUE;
		}

		if	(bIsStackAble	==	FALSE)
		{
			lpClient->sendFailedBuyItem(eFRBI_INVENTORY_FULL);
			return;
		}
	}
	

	llPrice	-=	llPrice*lpClient->m_wCourtesyTitleValue/10000;
	
	if	(m_wIsGuildHall)
	{
		CGuild	*lpGuild	=	lpClient->getGuild();

		if	(lpGuild)
			llPrice	-=	llPrice*lpGuild->m_wDecreaseGuildShopItemPrice/100;
	}
	else if(lpShop->m_TokenShop == 0xffff)
	{
		llPrice	+=	llPrice*lpClient->getTradeBonusByA()/100;
	}

	itemSell.m_dwSerial	=	(DWORD)llPrice;
	
	g_eventManager.operateItemByItemSaleEvent(&itemSell);

//	유효기간 지난 아이템은 판매 안한다.
	{
		cBasicItem	*lpBasicItem	=	itemSell.getBasicItem();
		int		iExpireTimeValue	=	lpBasicItem->m_wExpireYear*1000000  +lpBasicItem->m_wExpireMonth*10000  +lpBasicItem->m_wExpireDay*100  +lpBasicItem->m_wExpireHour;
		int		iCurrentTimeValue	=	g_currentTime.m_wYear*1000000		+g_currentTime.m_wMonth*10000		+g_currentTime.m_wDay*100		+g_currentTime.m_wHour;

		if	(iExpireTimeValue != 0 && iCurrentTimeValue > iExpireTimeValue)
		{
			lpClient->sendFailedBuyItem(eFRBI_UNKONWN);

			return;
		}
	}

	itemSell.m_dwSerial	=	max(itemSell.m_dwSerial,1);

	llPrice				=	itemSell.m_dwSerial;
	llPrice				*=	_lpPacket->wBuyCount;

	if	(lpShop->m_bf1IsDuelPointShop)
	{
		if	((DWORD)lpClient->m_duelRecord.m_iDuelPoint	<	llPrice)	//	돈이 모자라네.. -o-
		{
			lpClient->sendFailedBuyItem(eFRBI_LACK_OF_GOLD);
			return;
		}
	}
	else
	if	(lpShop->m_bf1IsLevelPerGoldShop)
	{
		if	((DWORD)lpClient->m_iGold <	lpShop->m_bf15LevelPerGoldValue * 100 * PriceForLevelPerGoldShop * lpClient->m_iLevel)	//	돈이 모자라네.. -o-
		{
			lpClient->sendFailedBuyItem(eFRBI_LACK_OF_GOLD);
			return;
		}
	}
	else
	if	(lpShop->m_TokenShop != 0xffff)
	{
		DWORD	dwItemCount = lpClient->getItemCount(lpShop->m_TokenShop , FALSE);
		if	(dwItemCount <	llPrice)	//	토큰이 부족하다.
		{
			lpClient->sendFailedBuyItem(eFRBI_LACK_OF_TOKEN);
			return;
		}

	}
	else
	if	((DWORD)lpClient->m_iGold	<	llPrice)	//	돈이 모자라네.. -o-
	{
		lpClient->sendFailedBuyItem(eFRBI_LACK_OF_GOLD);
		return;
	}

	if (lpShop->m_TokenShop == 0xffff)			//토큰 샵이 아니면.세금을 걷는다.
	{
		CGuild	*lpGuild	=	g_guildManager.getBySerial(m_wHallOwnGuild);

		if	(lpGuild	&&	lpGuild->m_wHallLevel)
		{
	//	길드 홀 수수료 걷기
			LONGLONG	llCharge	=	llPrice;
			llCharge				=	llCharge*lpGuild->m_wHallLevel/100;

			int	iCharge				=	(int)llCharge;

			g_game.sendWPAddGuildGold(lpGuild->m_wSerial,iCharge);
		}
	}

	SERVERStoWORLD_CREATE_SELL_ITEM	packet;

	packet.base.set(sizeof(SERVERStoWORLD_CREATE_SELL_ITEM),dSERVERStoWORLD_CREATE_SELL_ITEM);

	packet.wIsDuelPointShop	=	lpShop->m_bf1IsDuelPointShop;
	packet.iSerialInServer	=	lpClient->m_iZoneSerial;	//	고객 -o-
	if (lpShop->m_bf1IsLevelPerGoldShop)
		packet.dwPrice		=	lpShop->m_bf15LevelPerGoldValue * 100 * PriceForLevelPerGoldShop * lpClient->m_iLevel;
	else
		packet.dwPrice		=	itemSell.m_dwSerial;		//	가격
	packet.wCount			=	_lpPacket->wBuyCount;
	packet.wWhere			=	eBUY_ITEM_IN_SHOP;
	packet.wIsTokenShop	=	lpShop->m_TokenShop;		//	토큰샵

	strcpy(packet.strName,lpClient->m_strName);				//	이름
	memcpy(&packet.item,&itemSell,sizeof(cItem));			//	판매할 아이템

	SEND_WOLRD_PACKET(&packet);
}

//
//	아이템 판매
void
cFIELD::sellItem(CG_SELL_ITEM *_lpPacket,int _iSerial)
{
	if (!checkTargetAndKill(_iSerial,_lpPacket->wShopKeeper))
		return;

	cACTOR	*lpShopKeeper=	getActor(_lpPacket->wShopKeeper);
	cACTOR	*lpCustomer	=	getActor(_iSerial);
	cNPC	*lpNpc		=	lpShopKeeper->getNpc();

	if (_lpPacket->wSellItemSlot	>=	dOWN_ITEM_COUNT)
	{
		sendFailedSellItem(lpCustomer,eFRSI_NOT_EXIST_ITEM);
		return;
	}

	cItem		*lpSellItem	=	&lpCustomer->m_aItems[_lpPacket->wSellItemSlot];
	cBasicItem	*lpBasicItem=	lpSellItem->getBasicItem();

	if	(lpSellItem->m_wBaseItem	==	0xffff)
	{
		sendFailedSellItem(lpCustomer,eFRSI_NOT_EXIST_ITEM);
		return;
	}

	if	(lpBasicItem->m_attr.isCanNotSelllItem	||	lpSellItem->isShopSpecialItem())
	{
		sendFailedSellItem(lpCustomer,eFRSI_CAN_NOT_SELL_ITEM);
		return;
	}

	if	(lpSellItem->getSpecialItem() && lpSellItem->getSpecialItem()->m_bf1IsFreeUseItem)
	{
		sendFailedSellItem(lpCustomer,eFRSI_CAN_NOT_SELL_ITEM);
		return;
	}

#ifndef	_FOR_KOREA
	if (lpSellItem->isExtraItem())
	{
		sendFailedSellItem(lpCustomer,eFRSI_CAN_NOT_SELL_ITEM);
		return;
	}
#endif
	
	if(!_lpPacket->bf1IsBadgeOfTraderShop)
	{
		if (!lpNpc)	//	NPC가 있는 넘이 아니잖어!!
		{
			sendFailedSellItem(lpCustomer,eFRSI_UNKONWN);
			return;
		}
		
		if (!lpNpc->isOwnedShop(_lpPacket->wShopSerial))	//	이 상점을 가지고 있냐?
		{
			sendFailedSellItem(lpCustomer,eFRSI_NPC_NOT_OWN_THAT_SHOP);
			return;
		}
		
		if (!lpCustomer->isTargetInAttackRange(lpShopKeeper,dEVENT_RANGE_WITH_NPC))
		{
			sendFailedSellItem(lpCustomer,eFRSI_TOO_FAR_DISTANCE);
			return;
		}
		
		CShop	*lpShop	=	m_shop.get(_lpPacket->wShopSerial);
		
		if (!lpShop)	//	그런 상점 없셔!!
		{
			sendFailedSellItem(lpCustomer,eFRSI_NOT_EXIST_SHOP);
			return;
		}
	}
//	아이템 체크섬이 안 맞는다.
	if (lpCustomer->getItemCheckSum()	!=	_lpPacket->wCheckSum)
	{
		sendFailedSellItem(lpCustomer,eFRSI_NOT_EXIST_ITEM);
		return;
	}

//	기본 아이템이 안 맞는다.
	if (lpSellItem->m_wBaseItem		!=	_lpPacket->wBaseItem)
	{
		sendFailedSellItem(lpCustomer,eFRSI_NOT_EXIST_ITEM);
		return;
	}

	if	(!lpSellItem->isExtraItem())
		if	(_lpPacket->wCount	==	0	||	lpSellItem->m_bCount<_lpPacket->wCount)
		{
			sendFailedSellItem(lpCustomer,eFRSI_NOT_MATCHED_ITEM_COUNT);
			return;
		}
	
	if (lpSellItem->isMagicCarpetEmblem() && _lpPacket->wCount > 1)
	{
		sendFailedSellItem(lpCustomer,eFRSI_NOT_MATCHED_ITEM_COUNT);
		return;
	}
	
//	패킷 전송
	{
		SG_SELL_ITEM	packet;

		packet.base.set(sizeof(SG_SELL_ITEM),dSG_SELL_ITEM);

		DWORD	dwPrice	=	lpSellItem->getPrice(_lpPacket->wCount);

		if	(!lpSellItem->isExtraItem())
		{
			WORD wPriceFactor = 25;
			DWORD dwLimitPrice = 10000;

			if(!_lpPacket->bf1IsBadgeOfTraderShop)
			{
				CShop	*lpShop	=	m_shop.get(_lpPacket->wShopSerial);
				wPriceFactor = lpShop->m_wPriceFactor;
				dwLimitPrice = lpShop->m_dwLimitPrice;
			}
			dwPrice	=	dwPrice*wPriceFactor/100;
			dwPrice	=	min(dwPrice,dwLimitPrice*_lpPacket->wCount);
			dwPrice	=	g_eventManager.getItemPriceByItemSaleEvent(lpSellItem,dwPrice);
		}

		if (lpCustomer->m_iGold	+	dwPrice >=	dMAX_OWN_GOLD)
		{
			sendFailedSellItem(lpCustomer,eFRSI_OWN_TOO_MANY_GOLD);
			return;
		}

		lpCustomer->m_iGold	+=	dwPrice;
		packet.iRemainGold	=	lpCustomer->m_iGold;
		packet.iPrice		=	(int )dwPrice;
		packet.wCount		=	_lpPacket->wCount;
		packet.wSellItemSlot=	_lpPacket->wSellItemSlot;

		lpCustomer->updateGoldLog(dwPrice,eGET_GOLD_BY_SELL_ITEM);

		g_userPM.add(lpCustomer->m_iClientSerial,&packet,packet.base.wSize);

		if (packet.iPrice	>=	1000)
		{
			cItem	item;

			item.copy(lpSellItem);
			item.m_bCount	=	(BYTE)_lpPacket->wCount;
			lpCustomer->sendSellItemLog(&item,dwPrice);	//	로그 전송
		}

		lpSellItem->decreaseCount(_lpPacket->wCount);	//	가지고 있던 아이템 제거
		lpCustomer->arrangeInventory();
	}
}

//
//퀘스트 취소
void
cFIELD::cancelQuest(CG_CANCEL_QUEST *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)	return;

	if	(_lpPacket->wQuestSlot >= dMAX_PROCESS_QUEST_COUNT)
		return;

	SG_CANCEL_QUEST	packet;

	packet.base.set(sizeof(SG_CANCEL_QUEST),dSG_CANCEL_QUEST);

	packet.wQuestSlot	=	_lpPacket->wQuestSlot;

	if	(lpActor->cancelQuestByClient(_lpPacket->wQuestSlot,_lpPacket->wQuestIndex) == FALSE)
		packet.wResult		=	eCQR_FAILED;
	else
	{
		packet.wResult		=	eCQR_SUCCESS;
		lpActor->m_awQuestCountDownTimer[_lpPacket->wQuestSlot] = 0xffff;
	}

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}