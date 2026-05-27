#include <math.h>
#include <time.h>

#include "cFIELD.H"
#include "cParty.H"
#include "cServer.H"
#include "cPACKET_WORLDSERVER.H"
#include "debugCode.H"
#include "CGame.H"
#include "CEvent.H"
#include "secretDungeon.H"
#include "treasureMapDefine.H"
#include "packetManager.H"

int	aiArcaDropChance[]	=	
{
	10,
	10,
	10,
	20,
	60,
	100,
	170,
	300,
	500,
	1000,
};
BOOL
cFIELD::isDropUniqueChance(int _iItemType,int _iTargetLevel,int _iKillerLuck,int _iKillerCorrectUnique, BOOL _bIsWordEventTime,BOOL _bIsGuildMasterGrace)
{
	BOOL			isUnique	=	FALSE;
	cBasicItem		basicItem;

	basicItem.m_wKind = _iItemType;

// 20240906 rekunn Unique drop chance
//	유니크 아이템 출현 확률	= 0.0095% * (1 + 캐릭터의 장비에 의한 희귀 아이템 확률 증가 보정치 합 + 운 * 0.0019) * (몬스터 레벨 ^ 0.99) * 0.01 * 2 * 해당 아이템의 드랍 보정치: [max 0.5%]
	if (basicItem.isEquipmentItem() ||	_iItemType	==	eIK_ALL_WEAPON)	// 변경.
	{
		double	dbValue			=	0.05*(2+_iKillerCorrectUnique*0.04f+_iKillerLuck*0.05);
		dbValue					*=	100000;
		dbValue					=	dbValue*g_iCorrectItemDropChanceByGoldRush/100;
//		dbValue					=	dbValue*g_iBoostItemDropChanceByLeadersSoundofBell/100;

		if	(_bIsWordEventTime)
			dbValue = dbValue * 2;

		if	(!_bIsWordEventTime && _bIsGuildMasterGrace)
			dbValue += dbValue / 2;

		if (dbValue > 700000)
        dbValue = 700000; // 20240904 rekunn 7%
		
		if (largeRandom(10000000) < dbValue)
        return TRUE;
	}

	return	FALSE;
}

BOOL
cFIELD::isDropRareChance(int _iItemType,int _iTargetLevel,int _iCorrectItemDropRate,int _iKillerLuck,int _iKillerCorrectUnique,int _iKillerCorrectRare,int _iCorrectMagicItemDropChance , BOOL _bIsWordEventTime , BOOL _bIsGuildMasterGrace)
{
	int		iLevelValue		=	(int)pow((double)_iTargetLevel,0.5f);
	int		iRareDropChance	=	2*(100+_iKillerCorrectUnique+_iKillerCorrectRare+_iKillerLuck)*iLevelValue/10*g_iItemFactor;
	cBasicItem		basicItem;

	basicItem.m_wKind = _iItemType;

	if	(_iCorrectItemDropRate)
		iRareDropChance		=	iRareDropChance*_iCorrectItemDropRate/100;

	iRareDropChance			=	iRareDropChance*g_iCorrectItemDropChanceByGoldRush/100;
//	iRareDropChance			=	iRareDropChance*g_iBoostItemDropChanceByLeadersSoundofBell/100;
	iRareDropChance			+=	iRareDropChance*_iCorrectMagicItemDropChance/100;

	if	(_bIsWordEventTime)
		iRareDropChance = 2 *  iRareDropChance;

	if	(_bIsWordEventTime)
		iRareDropChance = 2 *  iRareDropChance;

	if	(!_bIsWordEventTime && _bIsGuildMasterGrace)
		iRareDropChance +=  iRareDropChance / 2;

	if	(iRareDropChance	>	9500)
		iRareDropChance	=	9500;

	int		iRandValue	=	largeRandom(10000);

	if	(basicItem.isAttachAblePrefix()	&&	iRandValue	<	iRareDropChance			)
		return	TRUE;
	if	(_iItemType	==	eIK_JEWEL)
		return	TRUE;

	return	FALSE;	
}



BOOL
cFIELD::makeDropItem(cACTOR *_lpActor,cACTOR *_lpKiller,CDroppedItem *_lpDroppedItem,cDroppingItem	*_lpDroppingItem,int _iDropLevel,int _iOwner,BOOL _bIsDropUnique,BOOL _bIsDropRare)
{
	if	(_lpDroppingItem->m_wItemType	==	0xffff	)
		return FALSE;

	CDroppedItem	droppedItem;

	g_im.generateItemByType(&_lpDroppedItem->m_item,_lpDroppingItem->m_wItemType,_iDropLevel,_lpDroppingItem->m_bf8Count,_bIsDropUnique);

	cBasicItem	*lpBasicItem	=	_lpDroppedItem->m_item.getBasicItem();

	if	(!lpBasicItem)
		return FALSE;

	if (m_bIsGuildBattleField)
	{
		if	(lpBasicItem->m_attr.isGuildBattleItem==	FALSE)
			return FALSE;
	}
	else
	{
		if	(lpBasicItem->m_attr.isGuildBattleItem==	TRUE)
			return FALSE;
	}

	if	(_lpDroppedItem->m_item.isIncorrectJobItem())	//	아직 나오지 않은 직업의 아이템이다.
		return FALSE;

//	확장 아이템 출현 확률 = 1.9% * (1 + 캐릭터의 장비에 의한 마법 아이템 & 희귀 아이템 확률 증가 보정치 합 + 운 * 0.01) * (몬스터 레벨 ^ 0.5) * 0.1  : [max 75%]
//	확장 아이템 출현 확률 = 2%   * (1 + 캐릭터의 장비에 의한 마법 아이템 & 희귀 아이템 확률 증가 보정치 합 + 운 * 0.01) * (몬스터 레벨 ^ 0.5) * 0.1  : [max 95%]
	{
		if (_bIsDropRare)
			g_im.attachPrefixByDropLevel(&_lpDroppedItem->m_item,_iDropLevel,FALSE,FALSE);
		else
		{
			if	(!_bIsDropUnique)
			{// 변경.
				if(lpBasicItem->isAttachAblePrefix())	// 수정
				{	// 접두사를 붙일 수 있는 아이템..
					//				    '기본 아이템'은 25% 확률로 '좋은 아이템'의 형태로 떨어질 수 있다.
					//					'좋은 아이템'은 '기본 아이템'에 1~5레벨의 접두사(1503, 2414,4001번 제외)가 1개만 붙은 아이템이다.
					if	(random(100) < 25)
						g_im.makeGoodItem(&_lpDroppedItem->m_item,dITEM_LITTLE_GOOD_PREFIX_DROP_LEVEL);
				}
			}
		}
	}

	_lpDroppedItem->m_wOwner	=	_iOwner;
	_lpDroppedItem->m_pos.x		=	_lpActor->m_pos.x;
	_lpDroppedItem->m_pos.y		=	_lpActor->m_pos.y;

	if	(_iOwner	==	0xffff)
		_lpDroppedItem->m_wSealedTime	=	0; 

	_lpDroppedItem->m_item.reviseIncorrectPrefix();

	if	(_lpDroppedItem->m_item.reviseIncorrectData(FALSE))
		return FALSE;

	return	TRUE;
}

BOOL
cFIELD::isDropItemChance(CDroppedItem *_lpDroppedItem,cDroppingItem	*_lpDroppingItem,cACTOR *_lpKiller)
{
	int		iDropRate			=	_lpDroppingItem->m_dwDroppingRate*g_iItemFactor;

	if	(_lpKiller)
	{
		iDropRate	=	iDropRate*(_lpKiller->m_wCorrectItemDropRate+_lpKiller->getItemDropChanceBonusByA())/100;
		iDropRate	+=	iDropRate*_lpKiller->m_bonusItemDropChance.getCorrectedValue()/100;

		if(_lpKiller->m_wWordEventTime)
			iDropRate	= 2 * iDropRate;
	}

	iDropRate		=	iDropRate*g_iCorrectItemDropChanceByGoldRush/100;
	iDropRate		=	iDropRate*g_iBoostItemDropChanceByLeadersSoundofBell/100;
	if(_lpKiller && _lpKiller->m_wBE_IncreaseItemDropChance)		// 이벤트 풍선..!
	{
		iDropRate		*= _lpKiller->m_wBE_IncreaseItemDropChance;
		_lpKiller->m_wBE_IncreaseItemDropChance = 0;
	}
	if	(largeRandom(10000)>	iDropRate)
		return FALSE;	//	아이템 드랍 확률

	cItem		*lpItem		=	&_lpDroppedItem->m_item;
	cBasicItem	*lpBaseItem	=	lpItem->getBasicItem();

	int	iReTryFactor		=	0;

//(※ 딜럭스 유니크에 얼티미트 접두사가 붙은 슈퍼 얼티미트 딜럭스 유니크 아이템은 위의 규칙을 통과한 후 특별히 1/27의 드랍 굴림을 한 번 더 해서 그 아이템의 드랍 여부를 결정한다. 실패하면 그 아이템은 출현하지 않는다.) <- 딜럭스 유니크 작성 후 추후 적용 예정																																																										
	if	(lpItem->isDXUniqueItem()	&&	lpItem->isUltimateItem())
		iReTryFactor	=	27;
	else
//	(※ 딜럭스 유니크에 딜럭스 접두사가 붙은 슈퍼 더블 딜럭스 유니크 아이템은 위의 규칙을 통과한 후 특별히 1/27의 드랍 굴림을 한 번 더 해서 그 아이템의 드랍 여부를 결정한다. 실패하면 그 아이템은 출현하지 않는다.) <- 딜럭스 유니크 작성 후 추후 적용 예정																																																										
	if	(lpItem->isSuperUniqueDXItem())
		iReTryFactor	=	27;
	else
//※ 노말 유니크에 얼티미트 접두사가 붙은 슈퍼 얼티미트 유니크 아이템은 위의 규칙을 통과한 후 특별히 1/9의 드랍 굴림을 한 번 더 해서 그 아이템의 드랍 여부를 결정한다. 실패하면 그 아이템은 출현하지 않는다.																																																										
	if	(lpItem->isUniqueItem() && lpItem->isUltimateItem())
		iReTryFactor	=	9;
	else
//※ 노말 유니크에 딜럭스 접두사가 붙은 슈퍼 딜럭스 유니크 아이템은 위의 규칙을 통과한 후 특별히 1/9의 드랍 굴림을 한 번 더 해서 그 아이템의 드랍 여부를 결정한다. 실패하면 그 아이템은 출현하지 않는다.																																																										
	if	(lpItem->isUniqueItem() && lpItem->isDXItem())
		iReTryFactor	=	27;

	if	(iReTryFactor)
		if	(random(iReTryFactor) != 0)
			return FALSE;

	return	TRUE;
}

int
cFIELD::droppingItem(cACTOR *_lpActor,cACTOR *_lpKiller,CDroppedItem *_lpDroppedItem,cDroppingItem	*_lpDroppingItem,int _iDropLevel,int _iOwner,BOOL _bIsDropUnique,BOOL _bIsDropRare)
{
	if	(!makeDropItem(_lpActor,_lpKiller,_lpDroppedItem,_lpDroppingItem,_iDropLevel,_iOwner,_bIsDropUnique,_bIsDropRare))
		return	0xffff;

	if	(isDropItemChance(_lpDroppedItem,_lpDroppingItem,_lpKiller)	==	FALSE)
		return	0xffff;

	int	iDroppedItem	=	m_droppedItem.add(_lpDroppedItem);

	if	(iDroppedItem	==	0xffff)	//	필드에 떨굴 수 있는 아이템 한계-_-를 넘어섰다.
	{
		removeOldestDroppedItem(10);	//	가장 오래된 아이템 10개를 증발-_-시킨다.

		iDroppedItem	=	m_droppedItem.add(_lpDroppedItem);

		if (iDroppedItem==	0xffff)
			return 0xffff;
	}

	return	iDroppedItem;
}

void
cFIELD::resetSecretDungeonKeyReceiver()
{
	m_wRemainMinuteForCheckSecretDungeonParty		=	dSERECT_DUNGEON_KEY_REGEN_PERIOD+1;
	m_wRemainMinuteForRemoveSecretDungeonKeyCount	=	dSERECT_DUNGEON_KEY_REGEN_PERIOD+1;
	m_dwBookedSecretDungeonKeyParty					=	0xffffffff;
}

void
cFIELD::dropSecretDungeonKey(cACTOR *_lpKiller,int _iX,int _iY)
{
	if	(!_lpKiller || m_dwBookedSecretDungeonKeyParty == 0xffffffff)
		return;

	cParty	*lpParty	=	_lpKiller->getParty();

	if	(!lpParty	|| lpParty->m_dwUniqueSerial != m_dwBookedSecretDungeonKeyParty)
		return;

	if	(g_pIfManager->isFullSecretDungeon(m_wLinkSecretDungeon,TRUE))
		return;

	cSecretDungeon	*lpDungeon	=	g_pSdManager->get(m_wLinkSecretDungeon);

	if	(!lpDungeon)
		return;

	if	(lpDungeon->m_bf1IsTestDungeon)
		if	(g_bIsTestServer	==	FALSE)
			return;
	
	cACTOR			*lpLeader	=	lpParty->getActorByMemberIndex(0);

	if	(!lpLeader || lpLeader->m_wCurrentField != m_iUniqueSerial)
	{
		resetSecretDungeonKeyReceiver();
		return;
	}

	if	(lpLeader->isIdlePlayer() || lpLeader->isDeath())
	{
		resetSecretDungeonKeyReceiver();
		return;
	}

	int	iRange		=	GetOvalRange(lpLeader->m_pos.x,lpLeader->m_pos.y,_iX,_iY);

	if	(iRange		>=	dMINIMUM_AVAIL_PARTY_RANGE)
	{
		resetSecretDungeonKeyReceiver();
		return;
	}

	if	(lpLeader->getValidInventorySlot()	==	FALSE)
	{
		resetSecretDungeonKeyReceiver();

		return;
	}

	if	(lpLeader->getItemByBasicItem(g_im.m_wSecretDungeonKey))
	{
		resetSecretDungeonKeyReceiver();

		return;
	}

	cItem	item,itemLog;

	g_im.generateItemByBaseItem(&item,g_im.m_wSecretDungeonKey,1);

	item.m_year		=	g_currentTime.m_wYear-2000;
	item.m_month	=	g_currentTime.m_wMonth;
	item.m_day		=	g_currentTime.m_wDay;
	item.m_hour		=	g_currentTime.m_wHour;
	item.m_minute	=	g_currentTime.m_wMinute;
	item.increaseMinute(dSERECT_DUNGEON_KEY_REGEN_PERIOD);
	item.m_aPrefix[0].m_wPrefix	=	m_wLinkSecretDungeon;

	itemLog.copy(&item);

	lpLeader->sendAddItem(&item);

	if	(itemLog.isMoneyItem())
		CLOG("goldIOError","dropSecretDungeonKey");

	lpLeader->addItemToInventory(&item);
	lpParty->sendPickItemMessage(lpLeader,&itemLog);

	m_wRemainMinuteForCheckSecretDungeonParty		=	dSERECT_DUNGEON_KEY_REGEN_PERIOD+1;
	m_wRemainMinuteForRemoveSecretDungeonKeyCount	=	dSERECT_DUNGEON_KEY_REGEN_PERIOD+1;
	m_dwBookedSecretDungeonKeyParty					=	0xffffffff;

	lpDungeon->m_wActiveKeyCount++;
}


void
cFIELD::dropEventSummonMonsterItem(cACTOR *_lpEventSummonMonster)
{
	if(!_lpEventSummonMonster->m_bf1IsEventSummonMonster)
		return;

	cNPC		*lpNpc		=	m_apNpcGenerateData[_lpEventSummonMonster->m_wNpcActorSaveSerial];
	int			iOwner		=	lpNpc->getWinningHitter();

	cACTOR		*lpOwner	=	getTestedActor(iOwner);

	if	(lpOwner)
	{
		cACTOR	*lpLord		=	lpOwner->getControlLord();

		if	(lpLord)
			iOwner			=	lpLord->m_wSerialInField;
	}



	WORD	wDropKind	=	_lpEventSummonMonster->m_dropInfoOfSummonEventMonster.m_wKind;
	WORD	wDropItem	=	_lpEventSummonMonster->m_dropInfoOfSummonEventMonster.m_wDropItem;
	WORD	wDropChance	=	_lpEventSummonMonster->m_dropInfoOfSummonEventMonster.m_wDropChance;
	WORD	wDropCount	=	_lpEventSummonMonster->m_dropInfoOfSummonEventMonster.m_wDropCount;
	
	if(wDropItem != 0xffff && random(100) < wDropChance )
	{
		switch(wDropKind)
		{
		case	eSEM_NORMAL_DROP	:
			{
				SERVERStoWORLD_CREATE_ITEM	packet;
				
				packet.base.set(sizeof(cMSG_BASE_TYPE_FORWORLD)+4,dSERVERStoWORLD_CREATE_ITEM);
				packet.wCount		=	0;
				packet.wFieldSerial	=	m_iSerial;

				for(int i=0;i<wDropCount;++i)
				{
					CDroppedItem	droppedItem;
					droppedItem.reset();
					BOOL	bIsSuccessGenerateItem = TRUE;
					
					if(_lpEventSummonMonster->m_dropInfoOfSummonEventMonster.m_aiOption[0] !=0xffff)
						bIsSuccessGenerateItem	=	g_im.generateItemByBaseItem(&droppedItem.m_item,wDropItem  ,1,-1,_lpEventSummonMonster->m_dropInfoOfSummonEventMonster.m_aiOption);
					else
						bIsSuccessGenerateItem	=	g_im.generateItemByBaseItem(&droppedItem.m_item,wDropItem ,1);
					
					if(!bIsSuccessGenerateItem)
						break;
					
					if(_lpEventSummonMonster->m_dropInfoOfSummonEventMonster.m_bIsOwner)
						droppedItem.m_wOwner	=	iOwner;
					else
						droppedItem.m_wOwner	=	0xffff;
					
					droppedItem.m_pos.x		=	_lpEventSummonMonster->m_pos.x;
					droppedItem.m_pos.y		=	_lpEventSummonMonster->m_pos.y;
					
					if	(droppedItem.m_wOwner	==	0xffff)
						droppedItem.m_wSealedTime	=	0; 
					
					int	iDroppedItem	=	m_droppedItem.add(&droppedItem);
					
					if	(iDroppedItem	==	0xffff)	//	필드에 떨굴 수 있는 아이템 한계-_-를 넘어섰다.
					{
						removeOldestDroppedItem(10);	//	가장 오래된 아이템 10개를 증발-_-시킨다.
						
						iDroppedItem	=	m_droppedItem.add(&droppedItem);
						
						if	(iDroppedItem==	0xffff)
							continue;
					}
					packet.aCreateItem[packet.wCount].m_wItem	=	droppedItem.m_item.m_wBaseItem;
					packet.aCreateItem[packet.wCount].m_wSerial	=	iDroppedItem;
					packet.wCount++;	
					if(packet.wCount == dDROPPING_ITEM_COUNT+1)
					{
						packet.base.wSize	+=	sizeof(cCREATE_ITEM_INFO)*packet.wCount;
						
						SEND_WOLRD_PACKET(&packet);	

						packet.wCount		=	0;
						memset(packet.aCreateItem,0,sizeof(packet.aCreateItem));
					}
				}

				if	(packet.wCount)
				{
					packet.base.wSize	+=	sizeof(cCREATE_ITEM_INFO)*packet.wCount;
					SEND_WOLRD_PACKET(&packet);	
					return;
				}

				break;
			}
		case	eSEM_INVENTORY_DROP	:
			{
				for(int i=0;i<wDropCount;++i)
				{
					if(_lpEventSummonMonster->m_dropInfoOfSummonEventMonster.m_aiOption[0] !=0xffff)
						giveEventItemToBeater(_lpEventSummonMonster, wDropItem,_lpEventSummonMonster->m_dropInfoOfSummonEventMonster.m_aiOption,eGET_GOLD_BY_BEAT_MONSTER_EVENT);
					else
						giveEventItemToBeater(_lpEventSummonMonster, wDropItem,NULL,eGET_GOLD_BY_BEAT_MONSTER_EVENT);
				}
				break;
			}
		default :
			return;
		};
	}
	
}

//
//	액터가 킬러한테 죽어 아이템을 떨군다.
void
cFIELD::droppingItem(cACTOR *_lpActor,cACTOR *_lpKiller)
{
	//	_lpKiller은 getControlLord()으로 가공되어 들어온다. 

	BOOL	bIsNoItem=FALSE,bIsNoGold=FALSE;
	BOOL	bIsInrangeTarget=	FALSE;

	if	(_lpActor)
	{
		cCHARACTER	*lpCharacter	=	_lpActor->getCharacter();

		if	(lpCharacter	&&	lpCharacter->m_bf1IsNotDropItem)
			bIsNoItem	=	TRUE;
		if	(lpCharacter	&&	lpCharacter->m_bf1IsNotDropGold)
			bIsNoGold	=	TRUE;
	}

	if	(_lpKiller &&	_lpActor)
		bIsInrangeTarget	=	g_pPartyManager->isGoodParty(_lpActor,_lpKiller);

	if	(_lpKiller	&&	_lpKiller->m_bf1IsChanceToGetTreasureMap	&&	_lpActor)
	{
		_lpKiller->m_bf1IsChanceToGetTreasureMap	=	FALSE;

		if	(_lpKiller->getItemByBasicItem(g_im.m_wTreasureMap)	==	NULL	&&	_lpKiller->m_wTreasureBoxXPos	==	0xffff)
		{
			cItem	item;

			if	(g_im.generateTreasureMap(&item))
			{
				cItem	*lpValidSlot	=	_lpKiller->getValidInventorySlot();

				if	(lpValidSlot)
				{
					_lpKiller->sendAddItem(&item);

					if	(item.isMoneyItem())
						CLOG("goldIOError","cFIELD::droppingItem(");

					_lpKiller->addItemToInventory(&item);
				}
			}
		}
	}

//	bIsInrangeTarget		=	FALSE;

	cNPC		*lpNpc		=	m_apNpcGenerateData[_lpActor->m_wNpcActorSaveSerial];
	cCHARACTER	*lpCharacter=	&m_aCharacter[lpNpc->m_wCharacter];
	cJOB		*lpJob		=	lpCharacter->getJob();
	int			iDropMoney	=	0;
	int			iOwner		=	lpNpc->getWinningHitter();

	cACTOR		*lpOwner	=	getTestedActor(iOwner);

	if	(lpOwner)
	{
		cACTOR	*lpLord		=	lpOwner->getControlLord();

		if	(lpLord)
			iOwner			=	lpLord->m_wSerialInField;
	}

	SERVERStoWORLD_CREATE_ITEM	packet;

	packet.base.set(sizeof(cMSG_BASE_TYPE_FORWORLD)+4,dSERVERStoWORLD_CREATE_ITEM);
	packet.wCount		=	0;
	packet.wFieldSerial	=	m_iSerial;

	if	(_lpKiller	&&	m_wIsExistSecretDungeonGate	&&	m_wLinkSecretDungeon	!=	0xffff)
		dropSecretDungeonKey(_lpKiller,_lpActor->m_pos.x,_lpActor->m_pos.y);

	//
	//	때린 파티 5개 6*10
	//	때린 넘   5개
	//	아이템 주인 정하기.. --a
	//
	//	돈 떨어뜨림

	//	이거 월드서버로 보내는 패킷으로 대체.. -_-a

	int	iGoldDropChance	=	lpJob->m_wMoneyDroppingRate*g_iCorrectItemDropChanceByGoldRush/100;
	iGoldDropChance		=	iGoldDropChance*g_iBoostItemDropChanceByLeadersSoundofBell/100;

	if	(_lpKiller)
		iGoldDropChance	+=	_lpKiller->m_bCorrectGoldDropChance;

	if	(bIsNoGold	==	FALSE	&&	m_bIsGuildBattleField	==	FALSE	&&	random(100)	<	iGoldDropChance)
	{
		int			iGoldCorrectValue	=	random(dDROPPING_GOLD_RANGE*2)-dDROPPING_GOLD_RANGE;

		LONGLONG	llDropGold;

		llDropGold	=	lpJob->m_wGold+_lpActor->m_iLevel*lpJob->m_wGoldPerLevel/100;
		llDropGold	=	llDropGold+llDropGold*(100+iGoldCorrectValue)/100;

		llDropGold	=	max(llDropGold,1);
		llDropGold	*=	g_iItemFactor;
		llDropGold	+=	g_eventManager.getPlusDropGold();
		llDropGold	=	llDropGold*g_iCorrectItemDropChanceByGoldRush/100;
		llDropGold	=	llDropGold*g_iBoostItemDropChanceByLeadersSoundofBell/100;

		if	(_lpKiller)
			if	(_lpKiller->m_wWordEventTime)
				llDropGold	=	2*llDropGold;


		if	(g_iBoostGoldSecondByTreasureMapEventInServer	||	(_lpKiller	&&	_lpKiller->m_wBoostGoldSecondByTreasureMapEvent))
			llDropGold	=	llDropGold*dBOOST_GOLD_BY_TREASURE_MAP/100;

		iDropMoney	=	(int)min(llDropGold,dLIMIT_DROP_GOLD);
	}

	int	iKillerLevel				=	0;
	int	iKillerLuck					=	0;
	int	iKillerCorrectRare			=	0;
	int	iKillerUnique				=	0;
	int	iKillerCorrectItemDropRate	=	0;
	int	iKillerCorrectMagicItemDropChance	=	0;

	if	(_lpKiller &&	!_lpKiller->isDeath())
	{
		iKillerCorrectItemDropRate	=	_lpKiller->m_wCorrectItemDropRate;
		iKillerLevel		=	_lpKiller->m_iLevel;
		iKillerLuck			=	_lpKiller->getLuck();
		iKillerCorrectRare	=	_lpKiller->m_bCorrectRareItemDropRate;
		iKillerUnique		=	_lpKiller->m_bCorrectUniqueItemDropRate;
		iKillerCorrectMagicItemDropChance	=	_lpKiller->m_bCorrectMagicItemDropChance;
	}

	int	iDroppingItemCount	=	0;
	int	iDropLevel			=	_lpActor->m_iLevel;
	int	iDropLevelBonus		=	0;

	if	(_lpKiller)
		iDropLevel			+=	_lpKiller->getItemDropLevelBonusByA();

	if	(_lpKiller && _lpKiller->getPartySerial()	!=	0xffff)
	{
		cParty	*lpParty	=	g_pPartyManager->get(_lpKiller->getPartySerial());

		if	(lpParty)
			lpParty->m_dwLastTimeToDropItem	=	timeGetTime();

		cParty	party;

		if	(g_pPartyManager->getParty(&party,_lpKiller->getPartySerial()))
		{
			int	iValue	=	0;

			iDropLevel		+=	party.getAverageLevel(_lpKiller->m_wCurrentField,&_lpKiller->m_pos);

			if	(bIsInrangeTarget)
				iDropLevelBonus	=	5*party.getMemberCountForGiveExp(_lpKiller->m_wCurrentField,&_lpKiller->m_pos,&iValue);

			iDropLevel		/=	2;
			iDropLevel		+=	iDropLevelBonus;
		}
	}
	else
	if	(_lpKiller)
	{
		iDropLevel		+=	_lpKiller->m_iLevel;
		iDropLevel		/=	2;
	}

	iDropLevel		=	min(iDropLevel,_lpActor->m_iLevel+iDropLevelBonus);

	int	iDropArcaChance		=	aiArcaDropChance[lpJob->m_wMonsterLevel];

	if	(m_bIsGuildBattleField	==	TRUE	||	isArcaFull())
		iDropArcaChance		=	0;

	cArca	arca;

	if	(iDropArcaChance)
		if	(arca.generateForMonsterDrop(_lpActor,iOwner)	==	FALSE)
			iDropArcaChance			=	0;

	dropEventSummonMonsterItem(_lpActor);

	if	(bIsNoItem	==	FALSE)
	{
		for (int i=0;i<dDROPPING_ITEM_COUNT && iDroppingItemCount < lpJob->getDroppingItemCount();i++)
		{
			BOOL isWordEventTime = FALSE;
			if(_lpKiller)
				isWordEventTime = _lpKiller->m_wWordEventTime;

			BOOL isGuildMasterGrace = FALSE;
			if(_lpKiller)
				isWordEventTime = _lpKiller->m_wGuildMasterGrace;

			BOOL			bIsDropUnique	=	isDropUniqueChance(lpJob->m_aDroppingItem[i].m_wItemType,_lpActor->m_iLevel,iKillerLuck,iKillerUnique , isWordEventTime , isGuildMasterGrace);
			BOOL			bIsDropRare		=	isDropRareChance(lpJob->m_aDroppingItem[i].m_wItemType,_lpActor->m_iLevel,iKillerCorrectItemDropRate,iKillerLuck,iKillerUnique,iKillerCorrectRare,iKillerCorrectMagicItemDropChance , isWordEventTime , isGuildMasterGrace);
			CDroppedItem	droppedItem;

			if	(makeDropItem(_lpActor,_lpKiller,&droppedItem,&lpJob->m_aDroppingItem[i],iDropLevel,iOwner,bIsDropUnique,bIsDropRare)	==	FALSE)
				continue;

			if	(iDropArcaChance)
			{
				int	iDropChance		=	random(10000);

				if	(iDropChance < iDropArcaChance)
				{
					droppingItemInArcaByMonster(_lpActor,_lpKiller,&lpJob->m_aDroppingItem[i],iDropLevel,&arca,&droppedItem.m_item);

					continue;
				}
			}

			{
				cDroppingItem	droppingItem;

				memcpy(&droppingItem,&lpJob->m_aDroppingItem[i],sizeof(droppingItem));

				if	(bIsInrangeTarget)
					droppingItem.m_dwDroppingRate	+=	droppingItem.m_dwDroppingRate/10;

				if	(isDropItemChance(&droppedItem,&droppingItem,_lpKiller)	==	FALSE)
					continue;
			}

			int	iDroppedItem	=	m_droppedItem.add(&droppedItem);

			if	(iDroppedItem	==	0xffff)	//	필드에 떨굴 수 있는 아이템 한계-_-를 넘어섰다.
			{
				removeOldestDroppedItem(10);	//	가장 오래된 아이템 10개를 증발-_-시킨다.

				iDroppedItem	=	m_droppedItem.add(&droppedItem);

				if	(iDroppedItem==	0xffff)
					continue;
			}

			packet.aCreateItem[packet.wCount].m_wItem	=	droppedItem.m_item.m_wBaseItem;
			packet.aCreateItem[packet.wCount].m_wSerial	=	iDroppedItem;
			packet.wCount++;

			iDroppingItemCount++;
		}

		if	(iDroppingItemCount	==	0	&&	iKillerLevel	>	0	&&	iKillerLevel	<	60)
		{
			int		iCorrect	=	(int)pow(iKillerLevel,0.5f);
			int		iDropChance	=	50*100/max(iCorrect,1);

			if	(iDropChance	&&	largeRandom(10000)	<	iDropChance)
			{
				cDroppingItem	dropItemInfo;
				CDroppedItem	droppedItem;
				BOOL			isWordEventTime = FALSE;

				if(_lpKiller)
					if(_lpKiller->m_wWordEventTime)
						isWordEventTime = TRUE;

				BOOL			isGuildMasterGrace = FALSE;
				
				if(_lpKiller)
					if(_lpKiller->m_wGuildMasterGrace)
						isGuildMasterGrace = TRUE;

				dropItemInfo.m_wItemType		=	g_aCommonDropItemList[random(g_im.m_iCommonDropItemCount)];
				dropItemInfo.m_bf8Count			=	1;
				dropItemInfo.m_dwDroppingRate	=	10000;	//	떨어뜨릴 확률

				BOOL	bIsDropUnique			=	isDropUniqueChance(dropItemInfo.m_wItemType,_lpActor->m_iLevel,iKillerLuck,iKillerUnique , isWordEventTime , isGuildMasterGrace);
				BOOL	bIsDropRare				=	isDropRareChance(dropItemInfo.m_wItemType,_lpActor->m_iLevel,iKillerCorrectItemDropRate,iKillerLuck,iKillerUnique,iKillerCorrectRare,iKillerCorrectMagicItemDropChance, isWordEventTime , isGuildMasterGrace);
				int		iDroppedItem			=	droppingItem(_lpActor,_lpKiller,&droppedItem,&dropItemInfo,iDropLevel,iOwner,bIsDropUnique,bIsDropRare);

				if	(iDroppedItem	!=	0xffff)
				{
					packet.aCreateItem[packet.wCount].m_wItem	=	droppedItem.m_item.m_wBaseItem;
					packet.aCreateItem[packet.wCount].m_wSerial	=	iDroppedItem;
					packet.wCount++;

					iDroppingItemCount++;
				}
			}
		}
	}

	if	(_lpActor	&&	_lpKiller)
	{
		int		iExp				=	_lpActor->m_iLevel*10;
		float	fDropChance			=	1.0f-(abs(_lpKiller->m_iLevel-_lpActor->m_iLevel)*0.1f*1.0f);
		float	fCorrectChance[3]	=	{0.0f,0.0f,0.0f};
		int		aiItems[3];

		aiItems[0]	=	g_im.m_wFlakeOfDoingOne;
		aiItems[1]	=	g_im.m_wFeatherOfThinnkingOne;
		aiItems[2]	=	g_im.m_wToyOfSinandRin;

		cJOB	*lpJob			=	_lpActor->getJob();

		if	(_lpActor->m_wIsBoss)
		{
			fDropChance			*=	16;
		}
		else
		if	(lpJob)
		{
			if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_BOSS1)
			{
				fDropChance		*=	4;
				fCorrectChance[2]	=	2.0f;
			}
			else
			if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_SEMI_BOSS1)
			{
				fDropChance		*=	2;
				fCorrectChance[1]=	1.5f;
			}
			else
			{
				fCorrectChance[0]=	1.0f;
			}
		}

		for	(int i=0;i<3;i++)
		{
			int	iChance	=	(int)(fDropChance*100+fCorrectChance[i]*100);

			if	(iDroppingItemCount	>=	dDROPPING_ITEM_COUNT)
				break;

			if	(random(10000)	>=	iChance)
				continue;

			CDroppedItem	droppedItem;

			droppedItem.m_item.reset();
			droppedItem.m_item.m_wBaseItem	=	aiItems[i];
			droppedItem.m_item.m_bCount		=	1;
			droppedItem.m_item.m_aOption[0]	=	(iExp&0xff00)>>8;
			droppedItem.m_item.m_aOption[1]	=	iExp&0x00ff;

			int	iDroppedItem	=	m_droppedItem.add(&droppedItem);

			if	(iDroppedItem	==	0xffff)	//	필드에 떨굴 수 있는 아이템 한계-_-를 넘어섰다.
			{
				removeOldestDroppedItem(10);	//	가장 오래된 아이템 10개를 증발-_-시킨다.

				iDroppedItem	=	m_droppedItem.add(&droppedItem);

				if	(iDroppedItem==	0xffff)
					continue;
			}

			packet.aCreateItem[packet.wCount].m_wItem	=	droppedItem.m_item.m_wBaseItem;
			packet.aCreateItem[packet.wCount].m_wSerial	=	iDroppedItem;
			packet.wCount++;

			iDroppingItemCount++;
		}
	}

	if	(arca.getItemCount())
	{
		arca.addDropGold(iDropMoney,_lpActor);

		int	iSerial	=	g_arcaManager.add(&arca);

		if	(iSerial	!=	0xffff)
			addActiveArca(iSerial);
	}
	else
	if	(iDropMoney)
	{
		CDroppedItem	droppedItem;

		droppedItem.m_item.m_wBaseItem	=	dITEM_MONEY;
		droppedItem.m_item.m_dwSerial	=	iDropMoney;
		droppedItem.m_wOwner			=	iOwner;
		droppedItem.m_pos.x				=	_lpActor->m_pos.x;
		droppedItem.m_pos.y				=	_lpActor->m_pos.y;

		if	(iOwner	==	0xffff)
			droppedItem.m_wSealedTime	=	0;
		
		int	iDroppedItem	=	m_droppedItem.add(&droppedItem);

		if (iDroppedItem	==	0xffff)	//	필드에 떨굴 수 있는 아이템 한계-_-를 넘어섰다.
		{
			removeOldestDroppedItem(10);	//	가장 오래된 아이템 10개를 증발-_-시킨다.

			iDroppedItem	=	m_droppedItem.add(&droppedItem);

			if (iDroppedItem	==	0xffff)
				return;
		}

		packet.aCreateItem[packet.wCount].m_wItem	=	dITEM_MONEY;
		packet.aCreateItem[packet.wCount].m_wSerial	=	iDroppedItem;

		packet.wCount++;
	}

	if	(packet.wCount	==	0)
		return;

	packet.base.wSize	+=	sizeof(cCREATE_ITEM_INFO)*packet.wCount;

	SEND_WOLRD_PACKET(&packet);
}

void
cFIELD::giveEventItemToBeater(cACTOR* _lpActor,WORD	_wItemIndex, int* _lpOption,int _iMethod)
{
	cNPC* lpNpc	=	_lpActor->getNpc();
	if(!lpNpc)
		return;
	
	for(int i=0;i<dMAX_BEATER;++i)
	{
		int iBeater	=	lpNpc->m_aBeater[i];
		if	(iBeater	==	0xffff)
			break;
		if (iBeater	>=	dPARTY_FLAG)
		{
			iBeater	-=	dPARTY_FLAG;
			cParty*	lpParty = g_pPartyManager->get(iBeater);
			if(!lpParty )
				continue;

			
			for(int j=0;j<lpParty->getMemberCount();++j)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);
				
				if	(!lpMember)
					continue;

				lpMember->addItemWithOption(_wItemIndex,1,_lpOption,_iMethod);
				
			}
		}
		else
		{
			cACTOR	*lpActor	=	getTestedActor(iBeater);
			
			if(!lpActor)
				continue;
			lpActor->addItemWithOption(_wItemIndex,1,_lpOption,_iMethod);
		}
	}
}
//
//	가장 오래된 아이템  _iCount개 제거

void
cFIELD::removeOldestDroppedItem(int _iCount, BOOL _bIsIgnoreErrMsg)
{
	int	iIndex,iCounter,iRemoveItemCount=0,iCount=m_droppedItem.getCount();
	int	iRemoveTime	=	20;

	SG_REMOVE_DROPPED_ITEMS	packet;

	packet.base.set(sizeof(SG_REMOVE_DROPPED_ITEMS),dSG_REMOVE_DROPPED_ITEMS);
	packet.wCount	=	0;

	while(packet.wCount	==	0)
	{
		for (iIndex=0,iCounter=0;iIndex<dMAX_DROPPED_ITEM && iCounter < iCount ;iIndex++)
		{
			CDroppedItem	*lpItem	=	m_droppedItem.get(iIndex);

			if (!lpItem)	continue;

			iCounter++;

			if (lpItem->m_wDeathCounter	<=	iRemoveTime)
			{
				m_droppedItem.remove(iIndex);
				packet.aRemoveItem[packet.wCount++]	=	iIndex;

				if (packet.wCount	>=	_iCount	||	packet.wCount	>=	dMAX_OPERATE_DROPPED_ITEM)	break;
			}
		}

		if (iRemoveTime > dDROPPED_ITEM_LIFE_TIME)
			break;

		iRemoveTime	+=	20;
	}

	if (packet.wCount	==	0	&& !_bIsIgnoreErrMsg)
	{
		if (g_bIsTestServer)
		{
			ERRMSG("크아아","영찬이한테연락주세요2");
		}

		return;
	}

	packet.base.wSize	=	sizeof(packet) -sizeof(packet.aRemoveItem)  + 2*packet.wCount;

	addSendPacket((ALL_MSG *)&packet,0);
}


//
//	필드에 떨궈진 아이템 처리
void
cFIELD::operateDroppedItem()
{
	int	iIndex,iCounter,iRemoveItemCount=0,iCount=m_droppedItem.getCount();

	if (iCount	<=	0)
		return;

	SG_REMOVE_DROPPED_ITEMS	packet;

	packet.base.set(sizeof(SG_REMOVE_DROPPED_ITEMS),dSG_REMOVE_DROPPED_ITEMS);
	packet.wCount	=	0;

	CDroppedItem	*lpDroppedItemList	=	m_droppedItem.m_den.getList();

	for (iIndex=0,iCounter=0;iIndex<dMAX_DROPPED_ITEM && iCounter < iCount ;iIndex++)
	{
		CDroppedItem	*lpDroppedItem	=	&lpDroppedItemList[iIndex];

		if (lpDroppedItem->m_wSerial	==	0xffff)
			continue;

		iCounter++;

		if (lpDroppedItem->m_wSealedTime	)
			lpDroppedItem->m_wSealedTime--;
		if (lpDroppedItem->m_wDeathCounter	)
			lpDroppedItem->m_wDeathCounter--;

		if (lpDroppedItem->m_wDeathCounter	==	0)
		{
			m_droppedItem.remove(iIndex);
			packet.aRemoveItem[packet.wCount++]	=	iIndex;

			if (packet.wCount	>=	dMAX_OPERATE_DROPPED_ITEM)	break;
		}
	}

	if (packet.wCount)
	{
		packet.base.wSize	=	sizeof(packet) -sizeof(packet.aRemoveItem) + 2*packet.wCount;

		addSendPacket((ALL_MSG *)&packet,0);
	}
}

//
//	_lpDroppedItem 떨구기
void
cFIELD::dropItem(CDroppedItem *_lpDroppedItem)
{
	int	iSerial	=	m_droppedItem.add(_lpDroppedItem);

	if (iSerial	==	0xffff)	//	필드에 떨굴 수 있는 아이템 한계-_-를 넘어섰다.
	{
		removeOldestDroppedItem(10);	//	가장 오래된 아이템 10개를 증발-_-시킨다.

		iSerial	=	m_droppedItem.add(_lpDroppedItem);

		if (iSerial	==	0xffff)
			return;
	}

	if (_lpDroppedItem->m_item.m_wBaseItem	==	0xffff)
		return;

	SG_DROPPING_ITEM	sgDroppingItem;

	sgDroppingItem.base.set(sizeof(SG_DROPPING_ITEM),dSG_DROPPING_ITEM);
	sgDroppingItem.droppedItem.m_bf10Serial		=	iSerial;//시리얼
	
	if	(_lpDroppedItem->m_item.isPrincessWeapon())
	{
		cPrincessWeaponDefine	*lpWeapon			=	_lpDroppedItem->m_item.getPrincessWeapon();

		if	(!lpWeapon)
			return;

		sgDroppingItem.droppedItem.m_bf14BaseItem	=	lpWeapon->m_wBasicItem;
		sgDroppingItem.droppedItem.m_wCount			=	1;
		sgDroppingItem.droppedItem.m_bf4Type		=	eMIT_PRINCESS_WEAPON;
	}
	else
	{
		sgDroppingItem.droppedItem.m_bf14BaseItem	=	_lpDroppedItem->m_item.m_wBaseItem;
		sgDroppingItem.droppedItem.m_wCount			=	_lpDroppedItem->m_item.m_bCount;
		sgDroppingItem.droppedItem.m_bf4Type		=	_lpDroppedItem->m_item.getItemMagicalType();
	}

	sgDroppingItem.droppedItem.wX				=	_lpDroppedItem->m_pos.x;//어디에 떨어져 있냐?
	sgDroppingItem.droppedItem.wY				=	_lpDroppedItem->m_pos.y;//어디에 떨어져 있냐?

	if (_lpDroppedItem->m_item.m_wBaseItem	==	dITEM_MONEY)	
		sgDroppingItem.droppedItem.m_wCount		=	(WORD)min(_lpDroppedItem->m_item.m_dwSerial,0xffff);

	addSendPacket((ALL_MSG*)&sgDroppingItem,_lpDroppedItem->m_wOwner);
}

//
//	공지 전송할것 추가
void
cFIELD::addSendEventMessage(char *_lpstrText,int _iType, int _iTime , BOOL _bEffectflickeringly)
{
	SG_EVENT_NOTICE		packet;

	packet.base.set(sizeof(SG_EVENT_NOTICE),dSG_EVENT_NOTICE);
	packet.wNoticeType	=	_iType;
	packet.wTime		=	_iTime;
	packet.wEffectflickeringly	= _bEffectflickeringly;

	strncpy(packet.strMessage,_lpstrText,sizeof(packet.strMessage)-2);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strMessage)+strlen(packet.strMessage)+1;

	addSendPacket((ALL_MSG*)&packet,0xffff);
}

void
cFIELD::addSendOccurEffect(int _iX,int _iY,int _iEffect,int _iScale)
{
	SG_OCCUR_EFFECT	packet;

	packet.base.set(sizeof(packet),dSG_OCCUR_EFFECT);
	packet.wX		=	_iX;
	packet.wY		=	_iY;
	packet.wEffect	=	_iEffect;
	packet.wScale	=	_iScale;

	addSendPacket((ALL_MSG*)&packet,0xffff);
}

void
cFIELD::addSendOccurEffectBySkill(int _iX,int _iY,int _iSkill,int _iEffect)
{
	if (_iSkill	<	0	||	_iSkill	>=	dMAX_SKILL)
		return;

	CSkill	*lpSkill	=	&g_aSkill[_iSkill];

	if (lpSkill->m_wSerial	==	0xffff)
		return;

	switch(_iEffect)
	{
		case	eSE_HIT			:
			_iEffect	=	lpSkill->m_wHitImage;
			break;
		case	eSE_SKILL		:
			_iEffect	=	lpSkill->m_wSkillImage;
			break;
		case	eSE_HEAL		:
			_iEffect	=	lpSkill->m_wHealImage;
			break;
		case	eSE_CAST		:
			_iEffect	=	lpSkill->m_wCastImage;
			break;
		case	eSE_AID_SKILL	:
			_iEffect	=	lpSkill->m_wAidSkillCastingImage;
			break;
		case	eSE_CASTER_HIT	:
			_iEffect	=	lpSkill->m_wCasterHitImage;
			break;
		case	eSE_EXPLOSION	:
			_iEffect	=	lpSkill->m_wExplosionImage;
			break;
	}

	addSendOccurEffect(_iX,_iY,_iEffect,lpSkill->m_wImageScale);
}


//
//	무기를 벗었다.
void
cFIELD::sendHideWeapon(int _iWho,cAbility *_lpAbility,BOOL _bIsHideOtherJobWeapon)
{
	cItem	*lpWeapon;
	cACTOR	*lpActor=	getActor(_iWho);
	int		iJob	=	lpActor->m_wJob;
	int		iPart;

	if	(!lpActor)
		return;

	if (_bIsHideOtherJobWeapon)
	{
		lpWeapon	=	lpActor->getOtherJobWeapon();

		if	(iJob%2)	
			iJob--;
		else
			iJob++;
	}
	else
		lpWeapon	=	lpActor->getWeapon();

	if	(!lpWeapon)
		return;

	iPart	=	dEQUIP_WEAPON;

	if (iJob%2)	
		iPart	=	dEQUIP_WEAPON_2;

	SG_HIDE_EQUIPMENT	packet;

	packet.base.set(sizeof(SG_HIDE_EQUIPMENT),dSG_HIDE_EQUIPMENT);
	packet.wPart	=	iPart;
	packet.wMethod	=	0;

	int	iHideTime	=	_lpAbility->getHideWeaponTime();

	iHideTime		=	max(iHideTime,2);

	if (iPart	==	dEQUIP_WEAPON)
		lpActor->m_wHideWeapon1Time	=	iHideTime;
	else
		lpActor->m_wHideWeapon2Time	=	iHideTime;

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

	lpActor->sendAppearanceInfo();
}
