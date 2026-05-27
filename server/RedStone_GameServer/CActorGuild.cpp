#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "CGame.H"
#include "CDataSave.H"
#include "CGuild.h"
#include "instance_guild_battle_manager.h"
#include "packetManager.h"
#include "booking_work.h"
#include "secretDungeon.h"
#include "jobDefine.h"

int		l_aiGuildDungeonEnterTerms[10][9] = 
{	// 길드홀 입장가능 레벨, 필요아이템	,	입장가능 요일 일, 월, 화, 수, 목, 금, 토	
	{0,	0xffff								,	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},	
	{1,	dITEM_INDEX_GUILD_DUNGEON_TICKET_B1,	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
	{2,	dITEM_INDEX_GUILD_DUNGEON_TICKET_B2,	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
	{2,	dITEM_INDEX_GUILD_DUNGEON_TICKET_B3,	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
	{3,	dITEM_INDEX_GUILD_DUNGEON_TICKET_B4,	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
	{3,	dITEM_INDEX_GUILD_DUNGEON_TICKET_B5,	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
	{4,	dITEM_INDEX_GUILD_DUNGEON_TICKET_B6,	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
	{4,	dITEM_INDEX_GUILD_DUNGEON_TICKET_B7,	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
	{5,	0xffff,									FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
	{1,	0xffff,									TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE},

};


//
//	길드 맹글기 준비
BOOL
cACTOR::readyToMakeGuild()
{
	if	(m_wIsReadyToMakeGuild)
		return	FALSE;

	if	(m_wGuildSerial	!=	0xffff)
	{
		sendGuildWorkMessage(eGWM_YOU_ARE_ALREADY_GUILD_MEMBER);

		return FALSE;
	}

	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
	{
		char	strGuildName[dGUILD_NAME_LENGTH];
		
		memcpy(strGuildName,m_aWayPoint,sizeof(strGuildName));

		if	(g_guildManager.getByName(strGuildName))
		{
			m_bf1IsReadyToMakeGuild	=	FALSE;

			immediatelySendSaveDataToDBMemory();

			m_lpField->receiveAskJoinGuild(NULL,m_wSerialInField);

			return	FALSE;
		}
		
		g_game.sendWPCreateGuild(this,strGuildName,"");

		return	TRUE;
	}

	m_wIsReadyToMakeGuild	=	TRUE;
	m_bf1IsReadyToMakeGuild	=	TRUE;

	immediatelySendSaveDataToDBMemory();

	return	sendReadyToMakeGuild();
}

//
//	길드 스케쥴러 오픈
BOOL
cACTOR::openGuildBattleScheduler()
{
	if	(!isGuildMaster())
	{
		sendRegistServerMessage(eRSM_GUILD_ONLY_GUILD_MASTER_CAN_OPEN_GUILD_BATTLE_SCHEDULER);

		return	FALSE;
	}

	if	(g_gbsToday.isBlockToRequestBattleTime())
	{
		sendGuildWorkMessage(eGWM_GUILD_BATTLE_SLEEP_TIME);

		return TRUE;
	}


	m_wIsOpendGuildBattleScheduler	=	TRUE;

	return	sendOpenGuildBattleScheduler();
}

BOOL
cACTOR::askOpenGuildMarkComposer()
{
	if	(!isGuildMaster())
	{
		sendRegistServerMessage(eRSM_GUILD_ONLY_GUILD_MASTER_CAN_OPEN_GUILD_MARK_COMPOSER);

		m_bf1IsReadyToSelectGuildMark	=	FALSE;

		return	FALSE;
	}

	if	(!m_bf1IsReadyToSelectGuildMark)
		return	FALSE;

	return	sendOpenGuildMarkComposer();
}

//
//	길드 콤포져 인터페이스 오픈
BOOL
cACTOR::openGuildMarkComposer()
{
	if	(!isGuildMaster())
	{
		sendRegistServerMessage(eRSM_GUILD_ONLY_GUILD_MASTER_CAN_OPEN_GUILD_MARK_COMPOSER);

		return	FALSE;
	}

	m_bf1IsReadyToSelectGuildMark	=	TRUE;

	return	sendOpenGuildMarkComposer();
}

//
//	길드 생성되고나서 해야할 첫번째 작업 - 부길마 등록 -o-
void
cACTOR::createGuildFirstWork(WORD _wGuildSerial,char *_lpstrSubGuildMasterName,char *_lpstrSubMasterName)
{
	m_wIsReadyToMakeGuild	=	FALSE;
	m_bf1IsReadyToMakeGuild	=	FALSE;
}

//
//	길드 얻기
CGuild*
cACTOR::getGuild()
{
	return	g_guildManager.getBySerial(m_wGuildSerial);
}

void
cACTOR::warpToRelatedPlaceOfGuild(int _iHallIndex)
{
	if	(m_bf1IsOpenRelatedPlaceOfGuild	==	FALSE)
		return;

	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return;

	if	(_iHallIndex	<	0	||	_iHallIndex	>=	lpGuild->m_wRelatedPlaceCount	||	lpGuild->m_awGuildVillage[_iHallIndex]	==	0xffff)
		return;

	int	iVillage		=	lpGuild->m_awGuildVillage[_iHallIndex];

	g_pMoveFieldUserManager->booking(this,NULL,iVillage,0x7ffe);
}

void
cACTOR::returnToGuildHallByEndGuildPointBattle()
{
	if	(!m_lpField->m_bIsGuildPointBattleField	&&	!m_lpField->m_bIsSiegeWarfareField)	//	길드전 필드가 아니다.
		return;

	int	iOldHP	=	m_iHP;

	m_iHP	=	getMaxHP();

	if	(!warpToGuildHall())
		m_iHP	=	iOldHP;
}

void
cACTOR::returnToGuildHallByEndBattle()
{
	if	(!m_lpField->m_bIsSiegeWarfareField)	//	길드전 필드가 아니다.
		return;

	resetRestraintTimeByBattle();

	m_iHP	=	getMaxHP();

	warpToGuildHall();
}

//	길드 펫 대여 요청
void
cACTOR::askRentGuildPet(int _iPetType,char *_lpstrMemberName)
{
	if	(!isGuildMaster())
		return;

	CGuild*		lpGuild		=	getGuild();

	if	(!lpGuild)
		return;

	CGuildMan	*lpMember	=	lpGuild->getMember(_lpstrMemberName);

	if	(!lpMember)
		return;

	int	iLimitCount	=	0;

	switch(_iPetType)
	{
		case	eGUILD_PET_OBITIAN	:
			iLimitCount	=	lpGuild->m_wObitianCount;
			break;
		case	eGUILD_PET_DASH_BLADE	:
			iLimitCount	=	lpGuild->m_wDashBladeCount;
			break;
		case	eGUILD_PET_ASTRO_BOW	:
			iLimitCount	=	lpGuild->m_wAstralBowCount;
			break;
		case	eGUILD_PET_CRISTAL_WATER	:
			iLimitCount	=	lpGuild->m_wCristalWaterCount;
			break;
	}

	int	iRentedPetCount	=	lpGuild->getRentedPetCount(_iPetType);

	if	(iRentedPetCount	>=	iLimitCount)
	{
		sendEtcWork(eEW_RENT_GUILD_PET_COUNT_OVER,iLimitCount,_iPetType);

		return;
	}

	g_game.sendWPSimpleWork(eSWGW_RENT_GUILD_PET,m_wGuildSerial,_iPetType,lpMember->wSerial);
}

//	길드 펫 반환
void
cACTOR::askRetrievalRentedGuildPet(char *_lpstrMemberName)
{
	if	(!isGuildMaster())
		return;

	CGuild*	lpGuild	=	getGuild();

	if	(!lpGuild)
		return;

	CGuildMan	*lpMember	=	lpGuild->getMember(_lpstrMemberName);

	if	(!lpMember	||	lpMember->wSerial	==	0xffff)
		return;

	g_game.sendWPSimpleWork(eSWGW_RETRIEVAL_RENTED_GUILD_PET,m_wGuildSerial,lpMember->wSerial);
}

void
cACTOR::tryToGuildPointBattle()
{
	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return;

	cParty	*lpParty	=	getParty();

	if	(!lpParty	||	lpParty->isOnField(-1,m_wCurrentField)	==	FALSE)
	{
		sendRegistServerMessage(eRSM_TO_TRY_GUILD_POINT_BATTLE_GATHER_PARTY_MEMBER_IN_SAME_FIELD);
		return;
	}

	if	(g_bIsTestServer	==	FALSE)
		if	(lpGuild->m_bf2TryGuildPointBattleCountPerWeek	>=	c_iLimitOfTryGuildPointBattlePerWeek)
		{
			sendRegistServerMessage(eRSM_COUNT_OVER_TO_TRY_GUILD_POINT_BATTLE);

			return;
		}

	for	(int i=0;i<lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);

		if	(!lpActor	||	lpActor->m_wGuildSerial	!=	m_wGuildSerial)
		{
			sendRegistServerMessage(eRSM_TO_TRY_GUILD_POINT_BATTLE_GATHER_PARTY_MEMBER_IN_SAME_FIELD);

			return;
		}
	}
	
	if	(g_bIsTestServer	==	FALSE)
		if	(m_lpField->m_bIs1LevelGuildHall	==	FALSE	&&	m_lpField->m_wHallOwnGuild	!=	m_wGuildSerial)
			return;

	int		iIFSerial=	g_pIfManager->m_iRookie;
	int		iResult	=	g_pIfManager->create(g_pSdManager->m_wGuildPointBattleSecretDungeon,lpParty,m_lpField->m_iUniqueSerial,0);

	SG_REGIST_MESSAGE	packet;

	packet.base.set(sizeof(packet),dSG_REGIST_MESSAGE);
	packet.wType	=	eSM_SYSTEM;

	if	(iResult	==	eCSDR_FULL)
	{
		packet.wMessage	=	eRSM_TO_MANY_GUILD_TO_TRY_GUILD_POINT_BATTLE;

		lpParty->sendPacketToAllMember(&packet,packet.base.wSize);
		return;
	}

	if	(iResult	==	eCSDR_ALREADY_USE_SECRET_DUNGEON	||	iResult	!=	eCSDR_SUCCESS)
	{
		packet.wMessage	=	eRSM_FAILED_TO_TRY_GUILD_POINT_BATTLE;

		lpParty->sendPacketToAllMember(&packet,packet.base.wSize);
		return;
	}

	for	(i=0;i<lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

		if	(!lpMember)
			continue;

		lpMember->enterToTheInstanceField(iIFSerial,0,-1,-1,TRUE);
	}

	lpGuild->m_bf1IsMoveToPointBattleField	=	TRUE;
//	g_game.sendWPSimpleWork(eSWGW_INCREASE_TRY_GUILD_POINT_BATTLE_COUNT,lpGuild->m_wSerial);
}


void
cACTOR::tryToGuildDungeon(int _iPlayLevel)
{
	int i;

	CGuild	*lpGuild	=	getGuild();
	
	if	(!lpGuild)
		return;

	int	iCosmicItemCount	=	0;
	cParty	*lpParty		=	getParty();
	
	if	(!lpParty	||	lpParty->isOnField(-1,m_wCurrentField)	==	FALSE)
	{
		sendRegistServerMessage(eRSM_GUILD_DUNGEN_ERROR_GATHER_PARTY_MEMBER_IN_SAME_FIELD);
		return;
	}

	for	(i=0;i<lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);
		
		if	(!lpActor	||	lpActor->m_wGuildSerial	!=	m_wGuildSerial)
		{
			sendRegistServerMessage(eRSM_GUILD_DUNGEN_ERROR_GATHER_PARTY_MEMBER_IN_SAME_FIELD);
			
			return;
		}

		if	(lpActor->m_bf3GuildDungeonPlayTime >=	c_iLimitOfTryGuildDungenPerDay+lpActor->m_wCorrectGuildDungeonPlayCount)
		{
			sendRegistServerMessage(eRSM_COUNT_OVER_TO_TRY_GUILD_DUNGEN);

			return;
		}
		
		if(lpActor->isExistItemEnteringGuildDungeon())
			++iCosmicItemCount;
	}

	if	(m_lpField->m_bIs1LevelGuildHall	==	FALSE	&&	m_lpField->m_wHallOwnGuild	!=	m_wGuildSerial)
		return;

	SG_REGIST_MESSAGE	packet;

	packet.base.set(sizeof(packet),dSG_REGIST_MESSAGE);
	packet.wType	=	eSM_SYSTEM;


	if(lpParty->m_wMemberCount > iCosmicItemCount && _iPlayLevel != 0)
	{ 

		int	iTicket = lpParty->getCountOfMemberOwnedItem(l_aiGuildDungeonEnterTerms[_iPlayLevel][1], FALSE);
		int iPossibleGuildHallLevel = l_aiGuildDungeonEnterTerms[_iPlayLevel][0];
		int iGuildLevel			=	lpGuild->m_wHallLevel;

		if(lpGuild->m_wHallLevel == 5 && _iPlayLevel == 9)
		{
			iTicket =  lpParty->m_wMemberCount;	
		}
		
		if(iPossibleGuildHallLevel > iGuildLevel || lpParty->m_wMemberCount > iTicket || !l_aiGuildDungeonEnterTerms[_iPlayLevel][g_currentTime.m_wWeekDay+2])
		{//	길드홀 가능 레벨 체크.. 티켓 체크.. 요일 체크...

			packet.wMessage	=	eRSM_FAILED_TO_TRY_GUILD_DUNGEN;
			
			lpParty->sendPacketToAllMember(&packet,packet.base.wSize);
			return;
		}
	}

	int		iIFSerial=	g_pIfManager->m_iRookie;
	int		iResult	=	g_pIfManager->create(g_pSdManager->m_wGuildDungeon[_iPlayLevel],lpParty,m_lpField->m_iUniqueSerial,0);
	


	if	(iResult	==	eCSDR_FULL)
	{
		packet.wMessage	=	eRSM_TO_MANY_GUILD_TO_TRY_GUILD_DUNGEN;
		
		lpParty->sendPacketToAllMember(&packet,packet.base.wSize);
		return;
	}

	if	(iResult	==	eCSDR_ALREADY_USE_SECRET_DUNGEON	||	iResult	!=	eCSDR_SUCCESS)
	{
		packet.wMessage	=	eRSM_FAILED_TO_TRY_GUILD_DUNGEN;
		
		lpParty->sendPacketToAllMember(&packet,packet.base.wSize);
		return;
	}

	for	(i=0;i<lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);
		
		if	(!lpMember)
			continue;
		
		lpMember->m_bf1IsJoinGuildDungeon	=	 TRUE;		// 길드던전으로 입장한다
		lpMember->enterToTheInstanceField(iIFSerial,0,-1,-1,TRUE);

		g_game.sendPlayGuildDungeonLog(lpGuild,lpMember,_iPlayLevel);

		g_pPartyManager->sendWPChangeMemberInfo(getPartySerial(),lpMember->m_strName,lpMember->m_iLevel,lpMember->m_bf4SecretDungeonPlayCount,lpMember->m_wJob,lpMember->m_bf3GuildDungeonPlayTime);
	}
}


BOOL
cACTOR::retryToGuildDungeon()
{	// 파티원이 있는 길드비던으로 재입장을 시도한다.

	CGuild	*lpGuild	=	getGuild();
	
	if	(!lpGuild)
		return FALSE;
	
	cParty	*lpParty	=	getParty();
	if	(!lpParty)
		return FALSE;

	if	(m_bf3GuildDungeonPlayTime >=	c_iLimitOfTryGuildDungenPerDay+m_wCorrectGuildDungeonPlayCount)
	{
		sendRegistServerMessage(eRSM_COUNT_OVER_TO_TRY_GUILD_DUNGEN);
		
		return FALSE;
	}
	
	int iIFSerial = 0xffff;
	
	for(int i=1;i<dMAX_IF_COUNT;++i)
	{
		cInstanceField*	lpIF =	g_pIfManager->get(i);

		if(!lpIF)
			continue;
		if(!lpIF->m_bISGuildDungeonFiled)
			continue;
		cParty	*lpIFParty = lpIF->getParty();
		if(!lpIFParty)
			continue;
	
		if(strcmp(lpParty->m_strPartyName , lpIFParty->m_strPartyName) != 0 )
			continue;

		for(int j=0;j<dPARTY_MAXPLAYER;++j)
		{
			cACTOR* lpMember = lpParty->getActorByMemberIndex(j);
			if(!lpMember)
				continue;
			if(lpMember->m_dwNameHashCode == m_dwNameHashCode)
			{
				iIFSerial = lpIF->m_wSerial;
				break;
			}

		}
		if(iIFSerial != 0xffff)
			break;
	}

	if(iIFSerial == 0xffff)
		return FALSE;
	m_bf1IsJoinGuildDungeon	=	 TRUE;		// 길드던전으로 입장한다
	enterToTheInstanceField(iIFSerial,0,-1,-1,TRUE);

		return TRUE;
}

BOOL
cACTOR::warpToGuildHall(int _iGuildSerial)
{
	CGuild	*lpGuild;

	BOOL	bIsOwnGuild	=	FALSE;

	if	(_iGuildSerial	==	-1)
	{
		_iGuildSerial	=	m_wGuildSerial;
		bIsOwnGuild		=	TRUE;
	}
	else
	if	(_iGuildSerial	==	m_wGuildSerial)
		bIsOwnGuild		=	TRUE;
	
	if	(m_lpField->m_iUniqueSerial	==	27)
		return	FALSE;
	
	if	(m_lpField->m_wHallOwnGuild	==	_iGuildSerial)
		return	FALSE;

	if	(bIsOwnGuild	&&	getRestraintTimeByBattle())
	{
		sendEtcWork(eEW_CAN_NOT_RETURN_TO_GUILD_HALL_DURING_BATTLE,getRestraintTimeByBattle());

		return	FALSE;
	}

	lpGuild	=	g_guildManager.getBySerial(_iGuildSerial);

	if	(!lpGuild	||	lpGuild->m_wHallLevel	==	0	||	(lpGuild->m_wHallLevel	==	1	&&	!bIsOwnGuild)		)
	{
		sendRegistServerMessage(eRSM_CAN_NOT_WARP_OWN_GUILD_HALL_BY_VALID,eSM_SYSTEM);

		return	FALSE;
	}

	if	(lpGuild->m_bf1IsPermitVisitor	==	FALSE	&&	!bIsOwnGuild)
	{
		sendRegistServerMessage(eRSM_TARGET_GUILD_HALL_IS_CLOSE_TO_STRANGER,eSM_SYSTEM);

		return	FALSE;
	}

	int	iFieldSerial	=	g_guildManager.getGuildHallFieldSerial(lpGuild);

	if	(iFieldSerial	==	-1)
	{
		sendRegistServerMessage(eRSM_CAN_NOT_WARP_OWN_GUILD_HALL_BY_VALID_FIELD_SERIAL,eSM_SYSTEM);

		return	FALSE;
	}

	g_pMoveFieldUserManager->booking(this,NULL,iFieldSerial,0x7fff);
	
	return	TRUE;
}

void
cACTOR::storeGuildInventoryItem(int _iInventorySlot,int _iGuildInventorySlot)
{
	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return;

	int	iStorage	=	lpGuild->getStorageIndexBySlot(_iGuildInventorySlot);

	if	(iStorage	==	0xffff)
		return;

	if	(lpGuild->m_aInventory[_iGuildInventorySlot].m_wBaseItem	!=	0xffff)
		return;

	if	(_iInventorySlot	<	0)
		return;
	if	(_iInventorySlot	>=	m_wItemSlotCount &&	_iInventorySlot < dOWN_ITEM_COUNT)
		return;
	if	(_iInventorySlot	>=	dOWN_ITEM_COUNT	+	m_bf6ExtraInventorySize)
		return;

	cItem	*lpItem	=	getInventoryItem(_iInventorySlot);

	if	(!lpItem)
		return;
	
	if	(lpItem->isCanNotTradeItem() || lpItem->isReversionItem() || lpItem->isExceptionItem() || lpItem->isExtraItem())
	{
		sendRegistServerMessage(eRSM_CAN_NOT_STORE_ITEM_IN_GUILD_INVENTORY,eSM_SYSTEM);
		return;
	}

	if	(lpItem->m_wBaseItem	==	dITEM_INDEX_GOLD_BAR)
	{
		LONGLONG	llStoreGold	=	lpItem->m_bCount;

		llStoreGold				*=	100000000;
		m_llTradeSpentGold		-=	llStoreGold;
	}

	g_game.sendWPStoreGuildInventoryItem(m_wGuildSerial,_iGuildInventorySlot,lpItem,m_strName,_iInventorySlot);

	sendRemoveItemLog(lpItem,eRI_BY_STORE_GUILD);

	lpItem->reset();

	immediatelySendSaveDataToDBMemory(TRUE);
}

void
cACTOR::withdrawGuildInventoryItem(int _iInventorySlot,int _iGuildInventorySlot ,BOOL _bIsByOCM , CGuild* _lpGuild)
{
	CGuild	*lpGuild	=	getGuild();

	if (_bIsByOCM)
		lpGuild = _lpGuild;

	if	(!lpGuild)
		return;

	if(!_bIsByOCM)
	{
		int		iStorage	=	lpGuild->getStorageIndexBySlot(_iGuildInventorySlot);
		
		if	(iStorage	==	0xffff)
			return;

		if	(isGuildMaster()	==	FALSE)
		{
			int	iPermitCount	=	lpGuild->m_aWithdrawItemCountPerDay[iStorage][m_wGuildRank-1];
			
			if	(m_abTodayWithdrawItemCount[iStorage]	>=	iPermitCount)
			{
				sendRegistServerMessage(eRSM_CAN_NOT_WITHDRAW_GUILD_ITEM_BY_CHECK_PERMIT_COUNT,eSM_SYSTEM);
				return;
			}
		}
	}

	if	(_iInventorySlot	<	0)
		return;
	if	(_iInventorySlot	>=	m_wItemSlotCount &&	_iInventorySlot < dOWN_ITEM_COUNT)
		return;
	if	(_iInventorySlot	>=	dOWN_ITEM_COUNT	+	m_bf6ExtraInventorySize)
		return;
	if	(getValidInventorySlot()	==	NULL)
		return;

	cItem	*lpItem			=	getInventoryItem(_iInventorySlot);

	if	(lpItem)
		return;

	lpItem	=	(cItem *)&lpGuild->m_aInventory[_iGuildInventorySlot];
	
	if	(lpItem->m_wBaseItem	==	0xffff)	//	빈자리다.
		return;

	if	(lpItem->m_wBaseItem	==	dITEM_INDEX_GOLD_BAR)
	{
		LONGLONG	llWithdrawGold	=	lpItem->m_bCount;

		llWithdrawGold			*=	100000000;
		m_llTradeGettingGold	+=	llWithdrawGold;
	}


	g_game.sendWPWithdrawGuildInventoryItem(lpGuild->m_wSerial,_iGuildInventorySlot,m_strName,lpItem);
}


void
cACTOR::storeGuildInventoryGold(int _iGold)
{
	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return;

	if	(m_iGold	<	_iGold || _iGold < 0)
		return;
	
	if	(lpGuild->m_llGuildGold+_iGold	>	2000000000)
	{
		sendRegistServerMessage(eRSM_TEMP_LIMIT_GOLD_IN_GUILD_INVENTORY,eSM_SYSTEM);

		return;
	}


	cItem	item;

	item.reset();
	item.m_wBaseItem	=	dITEM_MONEY;
	item.m_dwSerial		=	_iGold;

	sendRemoveItemLog(&item,eRI_BY_STORE_GUILD);

	m_iGold				-=	_iGold;
	m_llTradeSpentGold	-=	_iGold;

	g_game.sendWPStoreGuildInventoryGold(m_wGuildSerial,_iGold,m_strName);

	immediatelySendSaveDataToDBMemory(TRUE);
}

void
cACTOR::withdrawGuildInventoryGold(int _iGold)
{
	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild	||	lpGuild->m_llGuildGold	<	_iGold)
		return;

	if	(isGuildMaster()	==	FALSE)
	{
		int			iPermitGold		=	lpGuild->m_aiWithdrawGoldLimitPerDay[m_wGuildRank-1];
		LONGLONG	llUseGold		=	m_iTodayWithdrawGoldInGuild;
		llUseGold					+=	_iGold;

		if	(llUseGold	>	iPermitGold)
		{
			sendRegistServerMessage(eRSM_CAN_NOT_WITHDRAW_GUILD_GOLD_BY_CHECK_PERMIT_GOLD,eSM_SYSTEM);
			return;
		}
	}

	LONGLONG	llGold	=	m_iGold;

	llGold	+=	_iGold;

	if	(llGold	>	dMAX_OWN_GOLD)
		return;

	m_llTradeGettingGold	+=	_iGold;

	g_game.sendWPWithdrawGuildInventoryGold(m_wGuildSerial,_iGold,m_strName);
}

void
cACTOR::changeGuildInventoryItemPlace(int _iSlot1,int _iSlot2)
{
	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return;

	if	(_iSlot1	==	_iSlot2	||	_iSlot1	>=	lpGuild->m_wGuildInventorySize	||	_iSlot2	>=	lpGuild->m_wGuildInventorySize	||	
		_iSlot1	<	0	||	_iSlot2	<	0)
		return;

	g_game.sendWPChangeGuildInventoryItemPlace(m_wGuildSerial,_iSlot1,_iSlot2,m_strName);
}

BOOL
cACTOR::openGuildShop()
{
	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return	FALSE;

	if	(m_lpField->m_bIs1LevelGuildHall	==	FALSE	&&	m_lpField->m_wHallOwnGuild	!=	m_wGuildSerial)
		return	FALSE;

	if	(lpGuild->m_wSellEnchantItemCount	<=	0)
	{
		sendEtcWork(eEW_CAN_NOT_OPEN_GUILD_SHOP_BY_ITEM_ZERO);

		return	FALSE;
	}

	SG_OPEN_GUILD_SHOP	packet;

	packet.base.set(sizeof(packet),dSG_OPEN_GUILD_SHOP);
	packet.wDC_Factor		=	lpGuild->m_wDecreaseGuildShopItemPrice;

	packet.wSellItemCount	=	lpGuild->m_wSellEnchantItemCount;
	memcpy(packet.awSellItemList,lpGuild->m_awSellEnchantItem,sizeof(packet.awSellItemList));

	packet.base.wSize		=	sizeof(packet)-sizeof(packet.awSellItemList)+sizeof(WORD)*lpGuild->m_wSellEnchantItemCount;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}


void
cACTOR::openFeedGuildPetAndGuardianWindow()
{
	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return;

	SG_GUILD_PET_AND_GUARDIAN_EXP	packet;

	packet.base.set(sizeof(packet),dSG_GUILD_PET_AND_GUARDIAN_EXP);

	memcpy(packet.aPetInfo,lpGuild->m_aGuildPet,sizeof(lpGuild->m_aGuildPet));
	memcpy(packet.aGuardianInfo,lpGuild->m_aGuildGuardian,sizeof(lpGuild->m_aGuildGuardian));

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	sendEtcWork(eEW_OPEN_FEED_GUILD_GUARDIAN_AND_PET_WINDOW);
}

void
cACTOR::openGuildInventory()
{
	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return;

	m_bf1IsOpenGuildInventory	=	TRUE;

	SG_OPEN_GUILD_INVENTORY	packet;

	packet.wCount	=	0;

	memcpy(packet.aiWithdrawGoldLimitPerDay,lpGuild->m_aiWithdrawGoldLimitPerDay,sizeof(packet.aiWithdrawGoldLimitPerDay));
	memcpy(packet.aWithdrawItemCountPerDay,lpGuild->m_aWithdrawItemCountPerDay,sizeof(packet.aWithdrawItemCountPerDay));
	//	직위별 하루에 꺼낼 수 있는 아이템 수 제한
	memcpy(packet.abInventorySize,lpGuild->m_abInventorySize,sizeof(packet.abInventorySize));	
	packet.llGuildGold	=	lpGuild->m_llGuildGold;

	//	인벤토리 사이즈

	for	(int i=0;i<c_iGuildInventorySize;i++)
	{
		if	(lpGuild->m_aInventory[i].m_wBaseItem	==	0xffff)
			continue;

		cGuildInventoryItemForPacket	*lpItem	=	&packet.aItems[packet.wCount];

		memcpy(lpItem,&lpGuild->m_aInventory[i],sizeof(CItemDefine));

		((cItem*)lpItem)->reviseIncorrectData(FALSE);

		lpItem->m_wSlot	=	i;
		packet.wCount++;
	}

	packet.base.set(sizeof(packet),dSG_OPEN_GUILD_INVENTORY);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aItems)+sizeof(cGuildInventoryItemForPacket)*packet.wCount;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

BOOL
cACTOR::openGuildHonorPointWindow(int _iLevel)
{
	cIndexForQSort	aGuildList[dGUILD_MAX_COUNT];
	int	iGuildCount	=	0;
	int	iLimitCount	=	30;

	for (int iGuild=dGUILD_MAX_COUNT;iGuild;--iGuild)
	{
		CGuild	*lpGuild	=	g_guildManager.getBySerial(iGuild);

		if	(lpGuild	==	NULL	||	lpGuild->m_wSerial	==	0xffff	)
			continue;

		if	(lpGuild->m_wHallLevel	!=	_iLevel	||	lpGuild->m_iHonorPoint	==	0)
			continue;

		aGuildList[iGuildCount].m_iIndex	=	iGuild;
		aGuildList[iGuildCount].m_iValue	=	lpGuild->m_iHonorPoint;
		iGuildCount++;
	}

	qsort((void *)aGuildList,iGuildCount,sizeof(cIndexForQSort),(int(*)(const void *,const void *))DesCompare);

	iGuildCount	=	min(iGuildCount,iLimitCount);

	SG_GUILD_HONOR_POINT_LIST	packet;

	packet.base.set(sizeof(packet),dSG_GUILD_HONOR_POINT_LIST);
	packet.wCount	=	iGuildCount;
	packet.wHallLevel=	_iLevel;

	for	(int i=0;i<iGuildCount;i++)
	{
		CGuild	*lpGuild			=	g_guildManager.getBySerial(aGuildList[i].m_iIndex);

		packet.aList[i].m_iHonorPoint	=	lpGuild->m_iHonorPoint;
		strcpy(packet.aList[i].m_strGuildName,lpGuild->m_strName);
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aList)+sizeof(packet.aList[0])*iGuildCount;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

BOOL
cACTOR::openRelatedPlaceListOfGuild()
{
	m_bf1IsOpenRelatedPlaceOfGuild	=	TRUE;

	SG_OPEN_RELATED_PLACE_LIST	packet;

	packet.base.set(sizeof(packet),dSG_OPEN_RELATED_PLACE_LIST);

	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return	FALSE;

	packet.wCount		=	lpGuild->m_wRelatedPlaceCount;
	memcpy(packet.awPlaceList,lpGuild->m_awGuildVillage,lpGuild->m_wRelatedPlaceCount*sizeof(WORD));

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

void
cACTOR::openGuildHallList()
{
	m_bf1IsOpenGuildHallList	=	TRUE;

	SG_OPEN_GUILD_HALL_LIST	packet;

	packet.base.set(sizeof(packet),dSG_OPEN_GUILD_HALL_LIST);

	memset(packet.abBuffer,0,sizeof(packet.abBuffer));
	packet.wCount		=	0;

	int	iBufferPoint	=	0;

	for	(int iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
		for	(int iHall=0;iHall<g_aiHallCountByHallLevel[iHallLevel];iHall++)
		{
			char			*lpCurrentPoint	=	packet.abBuffer;
			cGuildHallInfo	*lpHallInfo		=	g_guildManager.getHall(iHallLevel,iHall);
			CGuild			*lpGuild		=	g_guildManager.getByIndex(lpHallInfo->m_wOwnGuild);

			if	(lpHallInfo->m_wOwnGuild	==	0xffff	||	!lpGuild)
				continue;

			lpCurrentPoint[iBufferPoint]	=	strlen(lpGuild->m_strName);
			memcpy(lpCurrentPoint+iBufferPoint+1,lpGuild->m_strName,lpCurrentPoint[iBufferPoint]);
			iBufferPoint					+=	lpCurrentPoint[iBufferPoint]+1;
			lpCurrentPoint[iBufferPoint]	=	lpHallInfo->m_bf3HallLevel;
			iBufferPoint++;
			lpCurrentPoint[iBufferPoint]	=	lpGuild->m_bf1IsPermitVisitor;
			iBufferPoint++;
			memcpy(lpCurrentPoint+iBufferPoint,&lpHallInfo->m_wOwnGuild,sizeof(WORD));
			iBufferPoint	+=	2;

			packet.wCount++;
		}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.abBuffer)+iBufferPoint;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	길드 스킬 레벨 중가
void
cACTOR::increaseGuildSkillLevel(int _iSkill,BOOL _bIsStatueSkill)
{
	CGuild	*lpGuild	=	getGuild();

	if	(lpGuild == NULL || !isGuildMaster())
	{
		sendGuildWorkMessage(eGWM_YOU_ARE_CAN_NOT_CHANGE_SKILL_LEVEL);
		return;
	}

	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);

	if	(lpSkill	==	NULL)
	{
		sendGuildWorkMessage(eGWM_INCORRECT_SKILL_INDEX);
		return;
	}

	if	(_bIsStatueSkill	==	FALSE)
	{
		if	(lpGuild->m_wMemberCount	>	lpGuild->m_wMaxMember)
		{
			if	(lpSkill->m_aEffect[0].m_wEffect	!=	eGSE_INCREASE_MAX_GUILD_MEMBER_COUNT)
			{
				sendGuildWorkMessage(eGWM_LACK_INCREASE_MEMBER_COUNT_SKILL_LEVEL);
				return;
			}
		}

		if	(lpSkill->m_bf1IsStatueSkill)
			return;
	}

	if	(lpSkill->m_wTopLevel	==	0)
	{
		sendGuildWorkMessage(eGWM_INCORRECT_SKILL_INDEX);
		return;
	}

	if	(!lpGuild->isUpgradeAbleSkill(_iSkill,this))
	{
		sendGuildWorkMessage(eGWM_CAN_NOT_INCREASE_LEVEL_SKILL);
		return;
	}

	lpGuild->m_wIsWaitGuildWorkResult	=	TRUE;

	if	(lpSkill->m_iRequireGold)
	{
		m_iGold	-=	lpSkill->m_iRequireGold;

		LONGLONG	llGold	=	lpSkill->m_iRequireGold;

		updateGoldLog(-llGold,eSPENT_GOLD_BY_LEARN_GUILD_SKILL);

		sendAddGold(-lpSkill->m_iRequireGold);
	}
	
	g_game.sendWPIncreaseGuildSkillLevel(m_wGuildSerial,_iSkill);
}

void
cACTOR::transGuildToGVGServer()
{
	if	(m_wWaitTransGVGResultTime)
	{
		sendEtcWork(eEW_TRANS_GUILD_TO_GVG_RESULT,eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_WAIT_FOR_RESULT);
		return;
	}

	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
		return;

	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return;
	
	if	(!isGuildMaster())
		return;

	SERVERStoWORLD_TRANS_GUILD_TO_GVG_SERVER	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_TRANS_AVATAR_TO_GVG_SERVER);

	packet.wMasterLevel	=	m_iLevel;
	packet.wMasterJob	=	m_wJob;

	memcpy(packet.strMasterName,m_strName,sizeof(m_strName));
	memcpy(packet.strMasterId,m_strId,sizeof(m_strId));

	memcpy(packet.guildData.m_strName,lpGuild->m_strName,sizeof(packet.guildData.m_strName));
	memcpy(&packet.guildData.m_guildMarkInfo,&lpGuild->m_guildMarkInfo,sizeof(lpGuild->m_guildMarkInfo));						//	길드 마크
	memcpy(packet.guildData.m_abGuildSkill,lpGuild->m_abGuildSkill,sizeof(lpGuild->m_abGuildSkill));

	packet.guildData.m_wLevel						=	lpGuild->m_wLevel;								//	길드 레벨
	packet.guildData.m_wGuildPoint					=	lpGuild->m_wGuildPoint;							//	스킬 포인트
	packet.guildData.m_bf1IsTwinkle					=	lpGuild->m_bf1IsTwinkle;	//	길드 마크가 반짝 거린다.
	packet.guildData.m_bf6BonusGuildPoint			=	lpGuild->m_bf6BonusGuildPoint;
	packet.guildData.m_bf6PeneltyBonusGuildPoint	=	lpGuild->m_bf6PeneltyBonusGuildPoint;

	packet.iWorldIndex	=	-1;	//	게임서버->월드,로그인서버->월드 양쪽 모두에 쓰는 패킷 월드 인덱스를 -1로 세팅 함으로 구분한다.

	packet.base.set(sizeof(packet),dSERVERStoWORLD_TRANS_GUILD_TO_GVG_SERVER);

	if	(!SEND_WOLRD_PACKET(&packet))
	{
		sendEtcWork(eEW_TRANS_GUILD_TO_GVG_RESULT,eTRAN_GUILD_TO_GVG_RESULT_FAILED);

		return;
	}

	m_wWaitTransGVGResultTime	=	60;	//	60초동안 전송 결과를 기다린다. 이 동안에는 로그 아웃도 안된다.
}

void
cACTOR::transAvatarToGVGServer()
{
	if	(m_wWaitTransGVGResultTime)
	{
		sendEtcWork(eEW_TRANS_AVATAR_TO_GVG_RESULT,eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_WAIT_FOR_RESULT);
		return;
	}

	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
		return;

	CTimeInfo	lastTime;

	lastTime.m_wYear	=	m_bf6LastTransAvatarToGVGYear+2000;	//	마지막으로 전송한 연도
	lastTime.m_wMonth	=	m_bf4LastTransAvatarToGVGMonth;
	lastTime.m_wDay		=	m_bf5LastTransAvatarToGVGDay;
	lastTime.m_wHour	=	0;
	lastTime.m_wMinute	=	0;
	lastTime.m_wSecond	=	0;

	int	iLastDayValue		=	lastTime.getDayValue();
	int	iCurrentDayValue	=	g_currentTime.getDayValue();

// 	if	(g_bIsTestServer	==	FALSE &&	iCurrentDayValue	<	iLastDayValue	+	g_iTransToGVGPeriod	&&	isTester() == FALSE)
// 	{
// 		lastTime.increaseDay(g_iTransToGVGPeriod);
// 		sendEtcWork(eEW_TRANS_AVATAR_TO_GVG_RESULT,eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_PERIOD,g_iWorldServerIndex,lastTime.m_wYear,lastTime.m_wMonth,lastTime.m_wDay);
// 
// 		return;
// 	}

	SERVERStoWORLD_TRANS_AVATAR_TO_GVG_SERVER	packet;

	memcpy(&packet.avatarData,this,sizeof(packet.avatarData));

	packet.avatarData.m_wCurrentField	=	801;
	packet.avatarData.m_wLastVillage	=	801;
	packet.avatarData.m_iXPos			=	-1;
	packet.avatarData.m_iYPos			=	-1;

	{
		packet.avatarData.m_bf4RemainTimeForUseBreedingRecordBook	=	0;

		for(int i=0;i<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)
		{
			cItem*	lpItem;

			if	(i	<	dOWN_ITEM_COUNT)
				lpItem	=	(cItem*	)&packet.avatarData.m_aItems[i];
			else
				lpItem	=	(cItem*	)&packet.avatarData.m_aExtraInventory[i-dOWN_ITEM_COUNT];

			if	(!lpItem->isBreedingRecordBook())
				continue;

			char						*lpData	=	(char *)lpItem;
			cBreedingRecordBookDefine	*lpBook	=	(cBreedingRecordBookDefine	*)(lpData+6);

			if	(lpBook->isAvailInfo(lpItem->m_dwSerial))
			{
				lpBook->m_bf4TamerSlot	=	0;
			}
			else
			{
				cPetDefine	*lpPetInfo	=	(cPetDefine	*)&lpItem->m_bCount;

				lpPetInfo->m_bf2TamerSlot=	0;
			}
		}
	}

	{
		CGuild	*lpGuild	=	getGuild();

		if	(!lpGuild)
		{
			sendEtcWork(eEW_TRANS_AVATAR_TO_GVG_RESULT,eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_NOT_OWNED_GUILD);
			return;
		}

		char	strNewGuildName[dGUILD_NAME_LENGTH];

		strcpy(strNewGuildName,lpGuild->m_strName);

		int	iNameLength	=	strlen(strNewGuildName);

		if	(iNameLength >= dGUILD_NAME_LENGTH-2)
		{
			strNewGuildName[iNameLength]	=	'A'+g_iWorldServerIndex;
			strNewGuildName[iNameLength+1]	=	NULL;
		}
		else
		{
			strNewGuildName[iNameLength]	=	'_';
			strNewGuildName[iNameLength+1]	=	'A'+g_iWorldServerIndex;
			strNewGuildName[iNameLength+2]	=	NULL;
		}

		if	(isGuildMaster())
			packet.avatarData.m_bf1IsReadyToMakeGuild	=	TRUE;

		memset(packet.avatarData.m_aProcessQuest,0,sizeof(packet.avatarData.m_aProcessQuest));
		
// 		packet.avatarData.m_aProcessQuest
// 		packet.avatarData.m_aCompleteQuest
// 		packet.avatarData.m_aCompleteArbeit
// 		packet.avatarData.m_aCompleteQuest2
// 		packet.avatarData.m_aCompleteArbeit2
// 		packet.avatarData.m_aWayPoint

		memcpy(packet.avatarData.m_aWayPoint,strNewGuildName,sizeof(strNewGuildName));	//	길드 생성시 이 이름을 사용하도록 한다.
	}

	packet.iWorldIndex	=	-1;	//	게임서버->월드,로그인서버->월드 양쪽 모두에 쓰는 패킷 월드 인덱스를 -1로 세팅 함으로 구분한다.

	packet.base.set(sizeof(packet),dSERVERStoWORLD_TRANS_AVATAR_TO_GVG_SERVER);

	if	(!SEND_WOLRD_PACKET(&packet))
	{
		sendEtcWork(eEW_TRANS_AVATAR_TO_GVG_RESULT,eTRAN_AVATAR_TO_GVG_RESULT_FAILED);

		return;
	}

	m_wWaitTransGVGResultTime	=	60;	//	60초동안 전송 결과를 기다린다. 이 동안에는 로그 아웃도 안된다.
}

BOOL
cACTOR::resetGuildSkillPoint(BOOL _bIsByCh5)
{
	if	(isGuildMaster()	==	FALSE)
	{
		sendRegistServerMessage(eRSM_PERMIT_ONLY_GUILD_MASTER_WORK);
		
		return	FALSE;
	}

	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return	FALSE;

	if	(lpGuild->m_wGuildPoint	>=	lpGuild->getFullSkillPoint())	//	이미 초기화된 상태
	{
		sendRegistServerMessage(eRSM_VALID_GUILD_SKILL_LEVEL);
		
		return	FALSE;
	}

	if	(_bIsByCh5)
		g_game.sendWPIncreaseGuildSkillLevel(m_wGuildSerial,0xfffd);
	else
		g_game.sendWPIncreaseGuildSkillLevel(m_wGuildSerial,0xfffe);

	return	TRUE;
}

BOOL
cACTOR::unsummonGuildGuardian()
{
	CGuild		*lpGuild		=	getGuild();

	if	(!lpGuild	||	isGuildMaster()	==	FALSE	||	m_wTeam	!=	0	||	m_lpField->m_bIsSiegeWarfareField	==	FALSE)
		return	FALSE;

	cACTOR		*lpGuildGuardian	=	m_lpField->findNearestGuildGuardian(m_pos.x,m_pos.y,300);
	int			iExistGuardianCount	=	m_lpField->getGuildGuardianCount();

	if	(!lpGuildGuardian)
		sendEtcWork(eEW_CAN_FIND_GUILD_GUARDIAN_IN_SIGHT);
	else
	{
		m_lpField->addBookedRemoveActor(lpGuildGuardian->m_wSerialInField);
		m_lpField->addSendRemoveActor(lpGuildGuardian->m_wSerialInField);

		sendEtcWork(eEW_REMOVE_NEAREST_GUILD_GUARDIAN,iExistGuardianCount-1,lpGuild->m_wMaxGuardianCount);
	}

	return	TRUE;
}

BOOL
cACTOR::addRelatedPlace(int _iField)
{
	CGuild	*lpGuild	=	getGuild();

	if	(isGuildMaster()	==	FALSE	||	lpGuild	==	NULL)
		return	FALSE;

	if	(lpGuild->isRelatedPlace(_iField))
		return	FALSE;

	int	iCount			=	lpGuild->getRelatedPlaceCount();
	int	aiGold[]		=	{0,100000000,120000000,140000000,160000000,180000000,200000000};

	if	(iCount	>=	lpGuild->getMaxRelatedPlaceCount())
		return	FALSE;

	if	(m_iGold	<	aiGold[iCount])
		return	FALSE;

	cItem	item;

	item.reset();
	item.m_wBaseItem	=	dITEM_MONEY;
	item.m_dwSerial		=	aiGold[iCount];

	sendRemoveItemLog(&item,eRI_BY_ADD_RELATED_PLACE,_iField);

	m_iGold	-=	aiGold[iCount];
	m_iGold	=	max(m_iGold,0);

	LONGLONG	llUseGold	=	aiGold[iCount];

	updateGoldLog(-llUseGold,eSPENT_GOLD_BY_ADD_GUILD_RELATED_PLACE);

	g_game.sendWPAddRelatedPlaceOfGuild(lpGuild->m_wSerial,_iField,aiGold[iCount],TRUE,m_strName);

	lpGuild->m_wTryAddRelatedPlaceField	=	0xffff;

	return	TRUE;
}

void
cACTOR::askToggleOpenguildHallStatus()
{
	if	(!isGuildMaster())
		return;

	g_game.sendWPSimpleWork(eSWGW_TOGGLE_GUILD_HALL_OPEN_STATUS,m_wGuildSerial);
}


int 
cACTOR::askActiveGuildMasterGraceEffect()
{
	if	(!isGuildMaster())
		return eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR1;

	CGuild*	lpGuild	= getGuild();

	if	(!lpGuild || g_iSeasonVariable < dSEASON_VARIABLE_GUILD_REFORM)
		return eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR4;

	if	(lpGuild->m_wHallLevel < 4)			//마스터 은총은 길드홀 4렙이상 에서 발동
	//if(lpGuild->m_wHallLevel < 4 && !g_bIsTestServer)			//마스터 은총은 길드홀 4렙이상 에서 발동
		return eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR3;

 	if	(lpGuild->m_bf1IsUseGuildMasterGrace)
 		return eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR2;
	
	g_game.sendWPSimpleWork(eSWGW_ACTIVE_GUILD_MASTER_GRACE,m_wGuildSerial);
	g_game.sendGuildMasterGraceLog(lpGuild , lpGuild->m_bf1IsUseGuildMasterGrace + 1, this);

	return 0;
}


void
cACTOR::feedItemToGuildPet(int _iType,int _iItemSlot)
{
	if	(_iType	<	0	||	_iType	>=	dGUILD_PET_TYPE_COUNT+dGUILD_GUARDIAN_TYPE_COUNT)
		return;

	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return;
	if	(m_lpField->m_bIs1LevelGuildHall	==	FALSE	&&	m_lpField->m_wHallOwnGuild	!=	m_wGuildSerial)
		return;
	if	(_iItemSlot	<	0	||	_iItemSlot	>=	m_wItemSlotCount)
		return;

	cItem	*lpItem	=	&m_aItems[_iItemSlot];

	if	(lpItem->isCanNotFeedToPetItem())
		return;
	if	(lpItem->isExpirationData())
		return;
	int		iFeedExp=	lpItem->getExpForGuildPet();

	if	(_iType	>=	dGUILD_PET_TYPE_COUNT)
	{
		_iType	-=	dGUILD_PET_TYPE_COUNT;

		if	(lpGuild->m_aGuildGuardian[_iType].m_dwExperience	+	iFeedExp	>	c_iMaxGuildPetExp)
		{
			sendEtcWork(eEW_CAN_NOT_FEED_ITEM_BY_OVER_MAX_GUILD_PET_EXP);
			return;
		}

		int	iExpDuringWeek	=	min(lpGuild->m_aGuildGuardian[_iType].m_wExpDuringWeek+iFeedExp,0xffff);
		lpGuild->m_aGuildGuardian[_iType].m_wExpDuringWeek	=	iExpDuringWeek;
		lpGuild->m_aGuildGuardian[_iType].m_dwExperience	+=	iFeedExp;

		sendRemoveItemLog(lpItem,eRI_BY_FEED_TO_GUILD_PET);

		lpItem->reset();

		WORD	wExp1	=	(WORD)(lpGuild->m_aGuildGuardian[_iType].m_dwExperience	>>	16);
		WORD	wExp2	=	(WORD)(lpGuild->m_aGuildGuardian[_iType].m_dwExperience	&	0xffff);

		sendEtcWork(eEW_FEED_ITEM_TO_GUILD_PET,_iType+dGUILD_PET_TYPE_COUNT,_iItemSlot,iFeedExp,wExp1,wExp2,iExpDuringWeek);

		g_game.sendWPSimpleWork(eSWGW_INCREASE_PET_EXP,m_wGuildSerial,_iType+dGUILD_PET_TYPE_COUNT,iFeedExp);

		return;
	}

	if	(lpGuild->m_aGuildPet[_iType].m_dwExperience	+	iFeedExp	>	c_iMaxGuildPetExp)
	{
		sendEtcWork(eEW_CAN_NOT_FEED_ITEM_BY_OVER_MAX_GUILD_PET_EXP);
		return;
	}

	int	iExpDuringWeek	=	min(lpGuild->m_aGuildPet[_iType].m_wExpDuringWeek	+	iFeedExp,0xffff);
	lpGuild->m_aGuildPet[_iType].m_wExpDuringWeek	=	iExpDuringWeek;
	lpGuild->m_aGuildPet[_iType].m_dwExperience		+=	iFeedExp;

	sendRemoveItemLog(lpItem,eRI_BY_FEED_TO_GUILD_PET);

	lpItem->reset();

	WORD	wExp1	=	(WORD)(lpGuild->m_aGuildPet[_iType].m_dwExperience	>>	16);
	WORD	wExp2	=	(WORD)(lpGuild->m_aGuildPet[_iType].m_dwExperience	&	0xffff);
	
	sendEtcWork(eEW_FEED_ITEM_TO_GUILD_PET,_iType,_iItemSlot,iFeedExp,wExp1,wExp2,iExpDuringWeek);

	g_game.sendWPSimpleWork(eSWGW_INCREASE_PET_EXP,m_wGuildSerial,_iType,iFeedExp);
}

BOOL
cACTOR::summonGuildGuardian(int _iType)
{
	if	(_iType	<=	0	||	_iType	>	eGUILD_GUARDIAN_HOLLOW_KNIGHT)
		return	FALSE;

	cCHARACTER	*lpCharacter	=	m_lpField->getCharacterByJob(_iType-1+dJOB_MONSTER_ShieldCraft);

	if	(!lpCharacter)
		return	FALSE;

	CGuild		*lpGuild		=	getGuild();

	if	(!lpGuild	||	isGuildMaster()	==	FALSE)
		return	FALSE;

	if	(m_lpField->m_bIsSiegeWarfareField	==	FALSE)
	{
		sendEtcWork(eEW_SGG_IS_USE_ABLE_IN_GUILD_BATTLE);
		return	FALSE;
	}

	if	(m_wTeam	!=	0)
	{
		sendEtcWork(eEW_SGG_IS_USE_ABLE_DEFENSE_GUILD);
		return	FALSE;
	}

	int	iExistGuardianCount	=	m_lpField->getGuildGuardianCount();

	if	(iExistGuardianCount	>=	lpGuild->m_wMaxGuardianCount)
	{
		sendEtcWork(eEW_TOO_MANY_SUMMON_GUILD_GUARDIAN,lpGuild->m_wMaxGuardianCount);
		return	FALSE;
	}

	int	iX,iY;

	iX	=	m_pos.x;
	iY	=	m_pos.y+40;

	if	(m_lpField->isBlocked(iX,iY))
		iY	=	m_pos.y;

	if	(m_lpField->checkInsightGuildGuardian(800,iX,iY))
	{
		sendEtcWork(eEW_EXIST_GUILD_GUARDIAN_OR_CREST_IN_TO_CLOSE_RANGE);	//	너무 가까운 곳에 길드 가디언이나 길드 문장이 있다.
		return	FALSE;
	}

	cACTOR	*lpActor	=	m_lpField->generateMonsterByCharacter(lpCharacter,lpGuild->m_awGuardianLevel[_iType],iX,iY);

	if	(!lpActor)
		return	FALSE;

	lpActor->m_sSight				+=	lpGuild->m_wIncreaseGuardianSight;
	lpActor->m_posFeignDeath.x		=	lpActor->m_pos.x;
	lpActor->m_posFeignDeath.y		=	lpActor->m_pos.y;
	lpActor->m_wBattleInclination	=	dBI_AGGRESSIVE_SOLO;
	lpActor->m_iGuildGuardianType	=	_iType;
	lpActor->m_aAbility[0].m_wLevel	=	lpActor->m_iLevel;

	lpActor->setMonsterReaction(TRUE);

	sendEtcWork(eEW_SUMMON_GUILD_GUARDIAN,iExistGuardianCount+1,lpGuild->m_wMaxGuardianCount,_iType);

	return	TRUE;
}

void
cACTOR::operateTransToGVGResult(WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT	*_lpPacket)
{
	m_wWaitTransGVGResultTime		=	1;

	if	(_lpPacket->wResult		==	eTRAN_AVATAR_TO_GVG_RESULT_SUCCESS)
	{
		m_bf6LastTransAvatarToGVGYear	=	g_currentTime.m_wYear-2000;	//	마지막으로 전송한 연도
		m_bf4LastTransAvatarToGVGMonth	=	g_currentTime.m_wMonth;
		m_bf5LastTransAvatarToGVGDay	=	g_currentTime.m_wDay;

		immediatelySendSaveDataToDBMemory();

		SG_GVG_WORK_SUCCESS	packet;

		packet.base.set(sizeof(packet),dSG_GVG_WORK_SUCCESS);
		packet.wResult	=	_lpPacket->wResult;
		packet.wIsTransAvatar	=	TRUE;
		strcpy(packet.strName,_lpPacket->strNewName);

		sendTransAvatarLog(_lpPacket->strNewName);

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
	}

	sendEtcWork(eEW_TRANS_AVATAR_TO_GVG_RESULT,_lpPacket->wResult,g_iWorldServerIndex,m_bf6LastTransAvatarToGVGYear,m_bf4LastTransAvatarToGVGMonth,m_bf5LastTransAvatarToGVGDay);
}

void
cACTOR::operateTransGuildToGVGResult(WORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT	*_lpPacket)
{
	m_wWaitTransGVGResultTime	=	1;

	if	(_lpPacket->wResult		==	eTRAN_GUILD_TO_GVG_RESULT_SUCCESS)
	{
		SG_GVG_WORK_SUCCESS	packet;

		packet.base.set(sizeof(packet),dSG_GVG_WORK_SUCCESS);
		packet.wResult			=	_lpPacket->wResult;
		packet.wIsTransAvatar	=	FALSE;
		strcpy(packet.strName,_lpPacket->strNewGuildName);

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
		
		g_game.sendTransGuildToGVGLog(getGuild(),this,_lpPacket->strNewGuildName);
	}

	sendEtcWork(eEW_TRANS_GUILD_TO_GVG_RESULT,_lpPacket->wResult,g_iWorldServerIndex,m_bf6LastTransAvatarToGVGYear,m_bf4LastTransAvatarToGVGMonth,m_bf5LastTransAvatarToGVGDay);
}
