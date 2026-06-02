#include "CKarma.h"
#include "cACTOR.H"
#include "CShop.H"
#include "cFIELD.H"
#include "CGame.h"
#include "debugCode.h"
#include "CEvent.h"
#include "secretDungeon.h"
#include "packetManager.H"
#include "booking_work.H"
#include "book.H"
#include "CGuild.H"
#include "itemset.H"
#include "duel.H"
#include "pieceItem.H"

BOOL
CKarma::operateDuelWork(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	int	iWork	=	_lpReaction->m_aValue[0];

	if	(iWork	==	0)
	{
		_lpAsker->sendEtcWork(eEW_OPEN_DUEL_RANKING_WINDOW);
		return	TRUE;
	}

	iWork--;

	if	(!g_pDuelManager->tryDuelMatch(_lpAsker,iWork))
		return	FALSE;

	int	iCount;

	g_pDuelManager->getApplicantCount(iWork,&iCount);

	cParty	*lpParty	=	_lpAsker->getParty();


	SG_TRY_DUEL_BATTLE	packet;

	packet.base.set(sizeof(packet),dSG_TRY_DUEL_BATTLE);

	strcpy(packet.strTrialGameName,g_duelTrialGameManager.m_strTrialGameName);
	
	packet.wType		=	iWork;

	if	(lpParty)
	{
		packet.wApplicantCount	=	0xffff;

		lpParty->sendPacketToAllMember(&packet,packet.base.wSize);
	}

	packet.wApplicantCount		=	iCount;

	g_userPM.add(_lpAsker->m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

//
//	" 대화 [speech]0으로 이동한다."
BOOL
CKarma::selectSpeech(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cACTOR *_lpOwner)
{
	if (!_lpAsker)
		return	FALSE;

	CSpeech					speech;
	int						iIndex		=	_lpReaction->m_aValue[0];
	CConversationContainer	*lpCC		=	NULL;

	if (_lpAsker->m_wEventArea	!=	0xffff)
	{
		cAreaInfo	*lpEventArea=	_lpAsker->m_lpField->getArea(_lpAsker->m_wEventArea);

		if (!lpEventArea)
			return	FALSE;

		lpCC	=	&lpEventArea->m_cc;
	}
	else
	{
		cNPC			*lpNpc		=	_lpOwner->getNpc();

		if (lpNpc)
			lpCC	=	lpNpc->m_pSpeech;
	}

	if (lpCC == NULL || lpCC->m_pConversation == NULL)
		return FALSE;

	if (iIndex	>=	lpCC->m_wConversationCount)
		return	ERRMSG("error in CKarma::selectSpeech","incorrect dialog\n\nfield %s\n\nnpc %s",_lpAsker->m_lpField->m_strName,_lpOwner->m_strName);

	CConversation	*lpDialog	=	&lpCC->m_pConversation[iIndex];

	if (lpDialog->m_wIsNoSpeech)
		return	_lpOwner->selectSpeech(_lpAsker,iIndex,0,TRUE);

	lpDialog->getSpeech(&speech);

	speech.m_wConversationIndex		=	iIndex;
	speech.m_wDialogBoxSize			=	lpCC->m_wDialogBoxSize;
	_lpAsker->m_wSelectConversation	=	iIndex;

	return	_lpOwner->sendSpeech(_lpAsker,&speech);
}

//
//	" 대화를 종료한다."
BOOL
CKarma::endDialog(cACTOR *_lpAsker,cACTOR *_lpOwner)
{
	if (!_lpAsker)
		return	FALSE;

	_lpAsker->closeTalk();

	return	FALSE;
}
BOOL
CKarma::dispellAllBuff(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{

	if (!_lpAsker)
		return	FALSE;

	int	iTargetType	=	_lpReaction->m_aValue[0];			//타켓의 대상
	BOOL	bIsParty	=	FALSE;
	cParty	party;

	if	(_lpAsker->getPartySerial()	!=	0xffff)
	{
		bIsParty = g_pPartyManager->getParty(&party,_lpAsker->getPartySerial());
	}



	switch (iTargetType)
	{
	case 0:					//요청한 사람만...
		_lpAsker->dispellAllBuff();
		_lpAsker->sendEtcWork(eEW_DISPELL);
		break;
	case 1:					//모든 파티원...
		{
			if(!bIsParty)
				break;
			for (int i=0;i<(int)party.getMemberCount();i++)
			{
				cACTOR	*lpMember	=	party.getActorByMemberIndex(i);
				
				if	(!lpMember	||	 !lpMember->isOnSameField(_lpAsker))
					continue;
				lpMember->dispellAllBuff();
				lpMember->sendEtcWork(eEW_DISPELL);
			}
			break;
		}
	case 2:					//	리더
		{
			if(!bIsParty)
				break;
			for (int i=0;i<(int)party.getMemberCount();i++)
			{
				cACTOR	*lpMember	=	party.getActorByMemberIndex(i);
				
				if	(!lpMember	||	 !lpMember->isOnSameField(_lpAsker))
					continue;
				if(party.isLeader(lpMember->m_strName))
				{
					lpMember->dispellAllBuff();
					lpMember->sendEtcWork(eEW_DISPELL);
					break;
				}
			}
			break;
		}
	case 3:					// 아무나 한명
		{
			if(!bIsParty)
				break;
			int iRand = random(party.getMemberCount());
			cACTOR	*lpMember	=	party.getActorByMemberIndex(iRand);
			if	(!lpMember	||	 !lpMember->isOnSameField(_lpAsker))
				break;
			lpMember->dispellAllBuff();
			lpMember->sendEtcWork(eEW_DISPELL);
			break;
		}
	}
	return TRUE;
}

BOOL
CKarma::releaseTransformationMonster(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{

	if (!_lpAsker)
		return	FALSE;

	int	iTargetType	=	_lpReaction->m_aValue[0];			//타켓의 대상
	BOOL	bIsParty	=	FALSE;
	cParty	party;

	if	(_lpAsker->getPartySerial()	!=	0xffff)
	{
		bIsParty = g_pPartyManager->getParty(&party,_lpAsker->getPartySerial());
	}



	switch (iTargetType)
	{
	case 0:					//요청한 사람만...
		_lpAsker->releaseTransformationMonster();
		_lpAsker->sendEtcWork(eEW_END_TRANSFORMATION_MONSTER);
		break;
	case 1:					//모든 파티원...
		{
			if(!bIsParty)
				break;
			for (int i=0;i<(int)party.getMemberCount();i++)
			{
				cACTOR	*lpMember	=	party.getActorByMemberIndex(i);
				
				if	(!lpMember	||	 !lpMember->isOnSameField(_lpAsker))
					continue;
				lpMember->releaseTransformationMonster();
				lpMember->sendEtcWork(eEW_END_TRANSFORMATION_MONSTER);
			}
			break;
		}
	case 2:					//	리더
		{
			if(!bIsParty)
				break;
			for (int i=0;i<(int)party.getMemberCount();i++)
			{
				cACTOR	*lpMember	=	party.getActorByMemberIndex(i);
				
				if	(!lpMember	||	 !lpMember->isOnSameField(_lpAsker))
					continue;
				if(party.isLeader(lpMember->m_strName))
				{
					lpMember->releaseTransformationMonster();
					lpMember->sendEtcWork(eEW_END_TRANSFORMATION_MONSTER);
					break;
				}
			}
			break;
		}
	case 3:					// 아무나 한명
		{
			if(!bIsParty)
				break;
			int iRand = random(party.getMemberCount());
			cACTOR	*lpMember	=	party.getActorByMemberIndex(iRand);
			if	(!lpMember	||	 !lpMember->isOnSameField(_lpAsker))
				break;
			lpMember->releaseTransformationMonster();
			lpMember->sendEtcWork(eEW_END_TRANSFORMATION_MONSTER);
			break;
		}
	}
	
	return TRUE;
}
//
//	" 상점 [shop]0의 거래창을 연다."
BOOL
CKarma::openShop(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cACTOR *_lpOwner)
{// 마지막 수정일 : 09.10.26
	if	(!_lpAsker || !_lpOwner)
		return	FALSE;

	SG_OPEN_SHOP	packet;

	CShop	*lpShop	=	_lpAsker->m_lpField->m_shop.get(_lpReaction->m_aValue[0]);

	if	(!lpShop)
		return	FALSE;

	int	iDC_Factor			=	0;

	if	(_lpAsker->m_lpField->m_wIsGuildHall)
	{
		CGuild	*lpGuild	=	_lpAsker->getGuild();

		if	(lpGuild)
			iDC_Factor		=	lpGuild->m_wDecreaseGuildShopItemPrice;
	}

	packet.bf1IsDuelPointShop	=	lpShop->m_bf1IsDuelPointShop;
	packet.bf15SellItemCount	=	lpShop->m_wItemCount;
	packet.bf1IsLevelPerGoldShop=	lpShop->m_bf1IsLevelPerGoldShop;
	packet.bf15LevelPerGoldValue=	lpShop->m_bf15LevelPerGoldValue;
	packet.wOwner				=	_lpOwner->m_wSerialInField;
	packet.wShopSerial			=	_lpReaction->m_aValue[0];
	packet.wPriceFactor			=	lpShop->m_wPriceFactor;
	packet.dwLimitPrice			=	lpShop->m_dwLimitPrice;
	packet.wTokenItem			=	lpShop->m_TokenShop;
	packet.bf1IsNotBuyItem		=	lpShop->m_bf1IsNotBuyItem;		// 상점 구입 불가.

	strcpy(packet.strName,lpShop->m_strName);

	for (int i=0;i<lpShop->m_wItemCount;i++)
	{
		memcpy(&packet.aItem[i],&lpShop->m_pSellItemInfo[i],sizeof(CItemDefine));

		LONGLONG	llPrice	=	packet.aItem[i].m_dwSerial;
		llPrice				-=	llPrice*_lpAsker->m_wCourtesyTitleValue/10000;
		llPrice				-=	llPrice*iDC_Factor/100;

		if	(_lpAsker->m_lpField->m_wIsGuildHall	==	FALSE)
			llPrice			+=	llPrice*_lpAsker->getTradeBonusByA()/100;

		packet.aItem[i].m_dwSerial	=	(DWORD)llPrice;
		packet.aItem[i].m_dwSerial	=	max(packet.aItem[i].m_dwSerial,1);
	}

	int	iPacketSize	=	sizeof(SG_OPEN_SHOP) - sizeof(packet.aItem) + sizeof(CItemDefine)*lpShop->m_wItemCount;

	packet.base.set(iPacketSize,dSG_OPEN_SHOP);

	g_userPM.add(_lpAsker->m_iClientSerial,&packet,packet.base.wSize);
	
	_lpAsker->m_wShopSerial			=	_lpReaction->m_aValue[0];

	return	TRUE;
}

//
//	은행열기
BOOL
CKarma::openBank(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	return	_lpAsker->askOpenBank();
}

//
//	은행열기
BOOL
CKarma::openRelayStation(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	_lpAsker->sendEtcWork(eEW_OPEN_RELAY_STATION);

	return	TRUE;
}

//
//	길드 생성
BOOL
CKarma::readyToMakeGuild(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	return	_lpAsker->readyToMakeGuild();
}

BOOL
CKarma::moveToGuildHall(cACTOR *_lpAsker,CKarmaItem *_lpReaction)
{
	if	(!_lpAsker)
		return	FALSE;

	int	iMethod	=	_lpReaction->m_aValue[0];

	if	(iMethod	==	1)
		_lpAsker->openGuildHallList();
	else
		_lpAsker->warpToGuildHall();

	return	TRUE;
}

BOOL
CKarma::feedGuildPetAndGuardian(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	if	(_lpAsker->m_wGuildSerial	==	0xffff)
		return	FALSE;

	if	(_lpAsker->m_lpField->m_bIs1LevelGuildHall	==	FALSE	&&	_lpAsker->m_lpField->m_wHallOwnGuild	!=	_lpAsker->m_wGuildSerial)
		return	FALSE;

	_lpAsker->openFeedGuildPetAndGuardianWindow();

	return	TRUE;
}


BOOL
CKarma::setGuildHallRoenGoolWill(CKarmaItem *_lpReaction , cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;
	
	int	iValue				=	_lpReaction->m_aValue[0];
	int	iAddorRemoveorSet	=	_lpReaction->m_aValue[1];
	
	CGuild	*lpGuild	=	_lpAsker->getGuild();
	
	if	(!lpGuild)
		return	FALSE;

	int LastData = lpGuild->m_bf3GuildGoodwillForRoen;

	switch(iAddorRemoveorSet)
	{
		case 0://더한다.
			lpGuild->m_bf3GuildGoodwillForRoen +=	iValue;
			break;

		case 1://뺀다
			lpGuild->m_bf3GuildGoodwillForRoen -=	iValue;
			break;

		case 2://으로 한다.
			lpGuild->m_bf3GuildGoodwillForRoen =	iValue;
			break;
	}

	g_game.sendWPSetGuildHallRoenGoodWillInfo(lpGuild,lpGuild->m_bf3GuildGoodwillForRoen);
	g_game.sendGuildRoenGoodWillLog(lpGuild ,_lpAsker,LastData , lpGuild->m_bf3GuildGoodwillForRoen);
	
	return	TRUE;
}

BOOL
CKarma::setGuildHallLevel(cACTOR *_lpAsker,CKarmaItem *_lpReaction)
{
	if	(!_lpAsker)
		return	FALSE;

	int	iLevel	=	_lpReaction->m_aValue[0];

	if	(g_bIsTestServer	==	FALSE)
		iLevel	=	1;

	CGuild	*lpGuild	=	_lpAsker->getGuild();

	if	(!lpGuild	||	lpGuild->m_wHallLevel)
		return	FALSE;

	if	(_lpAsker->isGuildMaster()	==	FALSE)
		return	FALSE;

	g_game.sendWPSetGuildHallLevel(lpGuild,iLevel);

	return	TRUE;
}

BOOL
CKarma::increaseGuildStatueSkillLevel(cACTOR *_lpAsker,CKarmaItem *_lpReaction)
{
	if	(!_lpAsker)
		return	FALSE;

	CGuild	*lpGuild		=	_lpAsker->getGuild();

	if	(!lpGuild)
		return	FALSE;

	int	iGuildStatueSkill	=	_lpReaction->m_aValue[0];

	_lpAsker->increaseGuildSkillLevel(iGuildStatueSkill,TRUE);

	return	TRUE;
}

BOOL
CKarma::openGuildInventory(cACTOR *_lpAsker)
{
	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
		return	FALSE;

	if	(!_lpAsker)
		return	FALSE;

	CGuild	*lpGuild		=	_lpAsker->getGuild();

	if	(!lpGuild)
		return	FALSE;

	_lpAsker->openGuildInventory();

	return	TRUE;
}

BOOL
CKarma::endGuildPointBattleAndRecordGuildHonorPoint(cACTOR *_lpAsker,CKarmaItem *_lpReaction)
{
	if	(!_lpAsker	||	_lpAsker->m_lpField	==	NULL)
		return	FALSE;

	int		iCompleteFailed	=	_lpReaction->m_aValue[0];

	cInstanceField	*lpIF	=	_lpAsker->m_lpField->getMasterInstanceField();

	if	(!lpIF)
	{
		if	(g_bIsWantPrintTriggerCondition)						
			printf(" completeSecretDungeon여긴 비밀 던전이 아니잖아!!\n");

		return	FALSE;
	}

	CGuild	*lpGuild	=	_lpAsker->getGuild();

	if	(lpGuild)
	{
		g_game.sendWPRecordGuildHonorPoint(_lpAsker->m_wGuildSerial,max(lpIF->m_iGuildHonorPoint,0));

		CLOG("HonorPoint","%s : T %d,P %d,R %d",lpGuild->m_strName,lpIF->m_iTotalGuildHonorPoint,lpIF->m_iPeneltyGuildHonorPoint,lpIF->m_iGuildHonorPoint);
	}

	{
		SG_ETC_WORK	packet;

		packet.base.set(sizeof(packet),dSG_ETC_WORK);

		packet.wWork		=	eEW_END_GUILD_POINT_BATTLE;
		packet.aiValue[0]	=	iCompleteFailed;

		packet.aiValue[1]	=	(lpIF->m_iTotalGuildHonorPoint>>16);
		packet.aiValue[2]	=	(lpIF->m_iTotalGuildHonorPoint&0xffff);
		packet.aiValue[3]	=	(lpIF->m_iPeneltyGuildHonorPoint>>16);
		packet.aiValue[4]	=	(lpIF->m_iPeneltyGuildHonorPoint&0xffff);

		lpIF->sendPacketToAllPlayer((char *)&packet);
	}

	lpIF->complete(0 , 0 , FALSE , TRUE);

	return	TRUE;
}
BOOL
CKarma::beginSiegeWarfareOfGuild(cACTOR *_lpAsker)
{
	if	(!_lpAsker	||	_lpAsker->m_lpField	==	NULL)
		return	TRUE;

	CGuild	*lpGuild	=	_lpAsker->getGuild();

	cSiegeWarfareInfoOfGuild	info;

	if	(!lpGuild	||	!g_guildManager.getSiegeWarFareInfoInCurrentTime(_lpAsker->m_wGuildSerial,&info) && (!g_bIsTestServer))
	{
		_lpAsker->sendRegistServerMessage(eRSM_FAILED_TO_ENTER_GUILD_BATTLE_FIELD_BY_NOT_BATTLE_TIME,eSM_SYSTEM);

		return	TRUE;
	}

	return	g_pMoveFieldUserManager->booking(_lpAsker,NULL,info.m_wBattleField,0);
}

BOOL
CKarma::giveGuildHonorPoint(cACTOR *_lpAsker,CKarmaItem *_lpReaction)
{
	if	(!_lpAsker	||	_lpAsker->m_lpField	==	NULL)
		return	FALSE;

	cInstanceField	*lpIF		=	_lpAsker->m_lpField->getMasterInstanceField();

	if	(!lpIF)
		return	FALSE;

	int	iValue	=	_lpReaction->m_aValue[0];
	int	iMethod	=	_lpReaction->m_aValue[1];
	int	iPoint	=	0;

	if	(iMethod==	0)
		iPoint	=	iValue;
	else
	if	(iMethod==	1)
		iPoint	=	lpIF->m_wCountdownTimer*iValue;

	lpIF->addGuildHonorPoint(iPoint);

	return	TRUE;
}

BOOL
CKarma::tryGuildPointBattle(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	CGuild	*lpGuild	=	_lpAsker->getGuild();

	if	(!lpGuild)
		return	FALSE;

	if	(!_lpAsker->isGuildMaster())
		return	FALSE;

	if	(g_bIsTestServer	==	FALSE)
		if	(lpGuild->m_bf2TryGuildPointBattleCountPerWeek	>=	c_iLimitOfTryGuildPointBattlePerWeek)
		{
			_lpAsker->sendRegistServerMessage(eRSM_COUNT_OVER_TO_TRY_GUILD_POINT_BATTLE);

			return	TRUE;
		}

	cParty	*lpParty	=	_lpAsker->getParty();

	if	(!lpParty	||	lpParty->isOnField(-1,_lpAsker->m_wCurrentField)	==	FALSE)
	{
		_lpAsker->sendRegistServerMessage(eRSM_TO_TRY_GUILD_POINT_BATTLE_GATHER_PARTY_MEMBER_IN_SAME_FIELD);

		return	TRUE;
	}

	for	(int i=0;i<lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);

		if	(!lpActor	||	lpActor->m_wGuildSerial	!=	_lpAsker->m_wGuildSerial)
		{
			_lpAsker->sendRegistServerMessage(eRSM_TO_TRY_GUILD_POINT_BATTLE_GATHER_PARTY_MEMBER_IN_SAME_FIELD);

			return	FALSE;
		}
	}

	if	(g_currentTime.m_wWeekDay	==	5	&&	g_currentTime.m_wHour	>=	22	||	g_currentTime.m_wWeekDay	==	6	&&	g_currentTime.m_wHour	<	6)
	{
		_lpAsker->sendRegistServerMessage(eRSM_IS_NOT_ABLE_TO_TRY_GUILD_POINT_BATTLE_TIME);

		return	FALSE;
	}

	_lpAsker->sendEtcWork(eEW_DO_YOU_WANT_REALLY_BEGIN_GUILD_POINT_BATTLE);

	return	TRUE;
}

BOOL
CKarma::trySecretDungeon(cACTOR *_lpAsker ,CKarmaItem* _lpReaction)
{
	if	(!_lpAsker)
		return	FALSE;

	return	_lpAsker->tryToSecretDungeon();
}

BOOL
CKarma::tryGuildDungen(cACTOR *_lpAsker ,CKarmaItem* _lpReaction)
{
	if	(!_lpAsker)
		return	FALSE;
	
	CGuild	*lpGuild	=	_lpAsker->getGuild();
	int		iPlayLevel	=	_lpReaction->m_aValue[0];
	
	if	(!lpGuild)
		return	FALSE;
	
	cParty	*lpParty	=	_lpAsker->getParty();
	
	if	(!lpParty	||	lpParty->isOnField(-1,_lpAsker->m_wCurrentField)	==	FALSE)
	{
		_lpAsker->sendRegistServerMessage(eRSM_GUILD_DUNGEN_ERROR_GATHER_PARTY_MEMBER_IN_SAME_FIELD);
		
		return	FALSE;
	}

	for	(int i=0;i<lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);
		
		if	(!lpActor	||	lpActor->m_wGuildSerial	!=	_lpAsker->m_wGuildSerial)
		{
			_lpAsker->sendRegistServerMessage(eRSM_GUILD_DUNGEN_ERROR_GATHER_PARTY_MEMBER_IN_SAME_FIELD);
			
			return	FALSE;
		}

		if	(lpActor->m_bf3GuildDungeonPlayTime	>=	c_iLimitOfTryGuildDungenPerDay+lpActor->m_wCorrectGuildDungeonPlayCount)
		{
			_lpAsker->sendRegistServerMessage(eRSM_COUNT_OVER_TO_TRY_GUILD_DUNGEN);
			
			return	FALSE;
		}
	}

	_lpAsker->sendEtcWork(eEW_DO_YOU_WANT_REALLY_BEGIN_GUILD_DUNGEN , iPlayLevel);
	
	return	TRUE;
}

BOOL
CKarma::addRelatedPlaceOfGuild(cACTOR *_lpAsker,CKarmaItem *_lpReaction)
{
	if	(!_lpAsker)
		return	FALSE;

	CGuild	*lpGuild	=	_lpAsker->getGuild();

	if	(!lpGuild)
		return	FALSE;

	int	iAddRemove		=	_lpReaction->m_aValue[0];
	int	iFieldSerial	=	_lpReaction->m_aValue[1];

	int	aiGold[]		=	{0,100000000,120000000,140000000,160000000,180000000,200000000};

	int	iCount			=	lpGuild->getRelatedPlaceCount();

	if	(_lpAsker->isGuildMaster()	==	FALSE)
	{
		_lpAsker->sendGuildWorkMessage(eGWM_YOU_ARE_NOT_GUILD_MASTER);

		return	FALSE;
	}

	if	(iAddRemove		==	0)
	{
		if	(lpGuild->isRelatedPlace(iFieldSerial))
		{
			_lpAsker->sendRegistServerMessage(eRSM_ALREADY_EXIST_SAME_VILLAGE_IN_GUILD_RELATED_PLACE_LIST,eSM_SYSTEM);

			return	FALSE;
		}

		if	(iCount	>=	lpGuild->getMaxRelatedPlaceCount())
		{
			_lpAsker->sendRegistServerMessage(eRSM_FULL_VILLAGE_IN_GUILD_RELATED_PLACE_COUNT,eSM_SYSTEM);

			return	FALSE;
		}

		WORD	wGold1	=	aiGold[iCount]>>16;
		WORD	wGold2	=	aiGold[iCount]&0xffff;

		if	(_lpAsker->m_iGold	<	aiGold[iCount])
		{
			_lpAsker->sendEtcWork(eEW_LOW_GOLD_FOR_ADD_VILLAGE_IN_GUILD_RELATED_PLACE_LIST,wGold1,wGold2);

			return	FALSE;
		}

		lpGuild->m_wTryAddRelatedPlaceField	=	iFieldSerial;

		_lpAsker->sendEtcWork(eEW_QUESTION_FOR_ADD_RELATED_PLACE_OF_GUILD,iFieldSerial,wGold1,wGold2);
	}
	else
	{
		if	(iCount	<=	1)
		{
			_lpAsker->sendRegistServerMessage(eRSM_MINIMUM_COUNT_RULE_OF_GUILD_RELATED_PLACE_LIST,eSM_SYSTEM);

			return	FALSE;
		}
		if	(lpGuild->isRelatedPlace(iFieldSerial)	==	FALSE)
		{
			_lpAsker->sendRegistServerMessage(eRSM_NOT_EXIST_VILLAGE_IN_GUILD_RELATED_PLACE_LIST,eSM_SYSTEM);

			return	FALSE;
		}
		
		_lpAsker->sendRemovePlaceRelatedOfGuild(lpGuild->m_strName,iFieldSerial);

		g_game.sendWPAddRelatedPlaceOfGuild(lpGuild->m_wSerial,iFieldSerial,0,FALSE,_lpAsker->m_strName);
	}

	return	TRUE;
}

BOOL
CKarma::openGuildBattleScheduler(cACTOR *_lpAsker,CKarmaItem *_lpReaction)
{
	if	(g_bIsTestGuildBattle)
		return	TRUE;

	if	(!_lpAsker)
		return	FALSE;

	int	iType	=	_lpReaction->m_aValue[0];

	return	_lpAsker->openGuildBattleScheduler();
}

//
//	길드 마크 콤포져 열기
BOOL
CKarma::openGuildMarkComposer(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;
	
	return	_lpAsker->openGuildMarkComposer();
}


BOOL
CKarma::openRepairItemWindow(cACTOR *_lpOwner,cACTOR *_lpTarget)
{
	if	(!_lpOwner || !_lpTarget)
		return	FALSE;

	return	_lpTarget->openRepairItemWindow(_lpOwner);
}

BOOL
CKarma::setQuestStart(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType)
{
	if (!_lpAsker)
		return	FALSE;

//	" 퀘스트 [quest]0을 진행중인 퀘스트에 추가한다."					,
	int	iQuestIndex	=	_lpReaction->m_aValue[0];
	int	iTarget		=	_lpReaction->m_aValue[1];
//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","end"},	//	eKARMA_KW_QUEST_TARGET

	switch(iTarget)
	{
		case	0	:
			return	_lpAsker->addProcessQuest(_iType,iQuestIndex,TRUE);

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->getMemberCount();i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember	||	!lpMember->isOnSameField(_lpAsker))
					continue;

				lpMember->addProcessQuest(_iType,iQuestIndex,TRUE);
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if (!lpLeader)
				return	FALSE;

			lpLeader->addProcessQuest(_iType,iQuestIndex,TRUE);

			return	TRUE;
		}

		case	3	:
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarma::setQuestComplete(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType)
{
	if (!_lpAsker)
		return	FALSE;

//	" 퀘스트 [quest]0을 완료 상태로 만든다."							,
	int	iQuestIndex	=	_lpReaction->m_aValue[0];
	int	iTarget		=	_lpReaction->m_aValue[1];
	BOOL isHideComplateMessage = _lpReaction->m_aValue[2];
//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","end"},	//	eKARMA_KW_QUEST_TARGET

	switch(iTarget)
	{
		case	0	:
			return	_lpAsker->setQuestComplete(_iType,iQuestIndex,TRUE,isHideComplateMessage);

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<(int)lpParty->getMemberCount();i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember	||	!lpMember->isOnSameField(_lpAsker))
					continue;

				lpMember->setQuestComplete(_iType,iQuestIndex,TRUE , isHideComplateMessage);
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(!lpLeader	||	!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			lpLeader->setQuestComplete(_iType,iQuestIndex,TRUE , isHideComplateMessage);

			return	TRUE;
		}

		case	3	:
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarma::setQuestCountdownTimer(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType)
{
	if (!_lpAsker)
		return	FALSE;

//	" 퀘스트 [quest]0을 [완료 상태로 체크한다.]1"		,
	int	iQuestIndex		=	_lpReaction->m_aValue[0];
	int	iTarget			=	_lpReaction->m_aValue[5];
	int	iBeginEnd		=	_lpReaction->m_aValue[1];
	int	iTime			=	_lpReaction->m_aValue[2]*60*60+_lpReaction->m_aValue[3]*60+_lpReaction->m_aValue[4];

	switch(iTarget)
	{
		case	0	:
			return	_lpAsker->setQuestTimer(_iType,iQuestIndex,iBeginEnd,iTime,TRUE);

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<(int)lpParty->getMemberCount();i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember	||	!lpMember->isOnSameField(_lpAsker))
					continue;

				lpMember->setQuestTimer(_iType,iQuestIndex,iBeginEnd,iTime,TRUE);
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(!lpLeader	||	!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			lpLeader->setQuestTimer(_iType,iQuestIndex,iBeginEnd,iTime,TRUE);

			return	TRUE;
		}

		case	3	:
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarma::setQuestCompleteChecker(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType)
{
	if (!_lpAsker)
		return	FALSE;

//	" 퀘스트 [quest]0을 [완료 상태로 체크한다.]1"		,
	int	iQuestIndex		=	_lpReaction->m_aValue[0];
	int	iTarget			=	_lpReaction->m_aValue[1];
	int	iCompleteCheker	=	_lpReaction->m_aValue[2];
//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","end"},	//	eKARMA_KW_QUEST_TARGET

	switch(iTarget)
	{
		case	0	:
			return	_lpAsker->setQuestCompleteChecker(_iType,iQuestIndex,iCompleteCheker,TRUE);

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<(int)lpParty->getMemberCount();i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember	||	!lpMember->isOnSameField(_lpAsker))
					continue;

				lpMember->setQuestCompleteChecker(_iType,iQuestIndex,iCompleteCheker,TRUE);
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(!lpLeader	||	!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			lpLeader->setQuestCompleteChecker(_iType,iQuestIndex,iCompleteCheker,TRUE);

			return	TRUE;
		}

		case	3	:
			return	TRUE;
	}

	return	FALSE;
}


BOOL
CKarma::setQuestProcessLevel(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType)
{
	if (!_lpAsker)
		return	FALSE;

//	" 퀘스트 [quest]0의 상태를 [quest_process_level]0로 설정한다."		,
	int	iQuestIndex		=	_lpReaction->m_aValue[0];
	int	iProcessLevel	=	_lpReaction->m_aValue[1];
	int	iTarget			=	_lpReaction->m_aValue[2];
//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","모든 파티 멤버(솔로 포함)","파티 리더(솔로 포함)","파티원중 아무나 하나(솔로 포함)","end"},

	if	(iTarget	>=	4	&&	iTarget	<=	6)	//	파티원 모두,파티 리더,파티 원 중 하나 지만 솔로 일경우 솔로에게 설정한다.
	{
		if	(_lpAsker->getParty()	==	NULL)
			iTarget	=	0;	//	솔로로 설정한다.
		else
			iTarget	-=	3;	//	솔로 체크를 제외한 타입이 된다.
	}

	switch(iTarget)
	{
		case	0	:
			return	_lpAsker->setQuestProcessLevel(_iType,iQuestIndex,iProcessLevel,TRUE);

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<(int)lpParty->getMemberCount();i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember	||	!lpMember->isOnSameField(_lpAsker))
					continue;

				lpMember->setQuestProcessLevel(_iType,iQuestIndex,iProcessLevel,TRUE);
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(!lpLeader	||	!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			lpLeader->setQuestProcessLevel(_iType,iQuestIndex,iProcessLevel,TRUE);

			return	TRUE;
		}

		case	3	:
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarma::setQuestValue(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType)
{
	if (!_lpAsker)
		return	FALSE;

//	" 퀘스트 [quest]0의 수치를 [spin]1[0,20000]로 설정한다. 대상 [quest_target]2 체크 영역 [area]3 [include_solo]4"	,

	int	iQuestIndex		=	_lpReaction->m_aValue[0];
	int	iValue			=	_lpReaction->m_aValue[1];
	int	iTarget			=	_lpReaction->m_aValue[2];
//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","모든 파티 멤버(솔로 포함)","파티 리더(솔로 포함)","파티원중 아무나 하나(솔로 포함)","end"},	//	eKARMA_KW_QUEST_TARGET_CHECK_SOLO
	int	iArea			=	_lpReaction->m_aValue[3];

	if	(iTarget	>=	4	&&	iTarget	<=	6)	//	파티원 모두,파티 리더,파티 원 중 하나 지만 솔로 일경우 솔로에게 설정한다.
	{
		if	(_lpAsker->getParty()	==	NULL)
			iTarget	=	0;	//	솔로로 설정한다.
		else
			iTarget	-=	3;	//	솔로 체크를 제외한 타입이 된다.
	}

	cAreaInfo	*lpArea	=	NULL;

	if	(iArea && _lpAsker->m_lpField)
		lpArea	=	_lpAsker->m_lpField->getArea(iArea);

	switch(iTarget)
	{
		case	0	:
		{
			if	(lpArea && !lpArea->isIn(_lpAsker->m_pos.x,_lpAsker->m_pos.y))
				return	TRUE;

			return	_lpAsker->setQuestValue(_iType,iQuestIndex,iValue,TRUE);
		}

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			if	(iArea && _lpAsker->m_lpField)
				cAreaInfo	*lpArea	=	_lpAsker->m_lpField->getArea(iArea);

			for (int i=0;i<(int)lpParty->getMemberCount();i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember	||	!lpMember->isOnSameField(_lpAsker))
					continue;

				if	(lpArea && !lpArea->isIn(lpMember->m_pos.x,lpMember->m_pos.y))
					continue;

				lpMember->setQuestValue(_iType,iQuestIndex,iValue,TRUE);
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(!lpLeader	||	!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			if	(lpArea && !lpArea->isIn(lpLeader->m_pos.x,lpLeader->m_pos.y))
				return	TRUE;

			lpLeader->setQuestValue(_iType,iQuestIndex,iValue,TRUE);

			return	TRUE;
		}

		case	3	:
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarma::controlQuestValue(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType)
{
	if (!_lpAsker)
		return	FALSE;

//	" 퀘스트 [quest]0의 수치를 [spin]1[1,255] [plus_minus]2 대상 [quest_target]3 체크 영역 [area]4 [include_solo]5"	,
	int	iQuestIndex		=	_lpReaction->m_aValue[0];
	int	iValue			=	_lpReaction->m_aValue[1];
	int	iMethod			=	_lpReaction->m_aValue[2];
	int	iTarget			=	_lpReaction->m_aValue[3];
//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","모든 파티 멤버(솔로 포함)","파티 리더(솔로 포함)","파티원중 아무나 하나(솔로 포함)","end"},	//	eKARMA_KW_QUEST_TARGET_CHECK_SOLO
	int	iArea			=	_lpReaction->m_aValue[4];
	int	iMax			=	_lpReaction->m_aValue[5];

	cAreaInfo	*lpArea	=	NULL;

	if	(iArea && _lpAsker->m_lpField)
		lpArea	=	_lpAsker->m_lpField->getArea(iArea);

	if	(iTarget	>=	4	&&	iTarget	<=	6)	//	파티원 모두,파티 리더,파티 원 중 하나 지만 솔로 일경우 솔로에게 설정한다.
	{
		if	(_lpAsker->getParty()	==	NULL)
			iTarget	=	0;
		else
			iTarget	-=	3;
	}

	switch(iTarget)
	{
		case	0	:
		{
			if	(lpArea && !lpArea->isIn(_lpAsker->m_pos.x,_lpAsker->m_pos.y))
				return	TRUE;

			return	_lpAsker->controlQuestValue(_iType,iQuestIndex,iMethod,iValue,TRUE,iMax);
		}

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			for (int i=0;i<(int)lpParty->getMemberCount();i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember	||	!lpMember->isOnSameField(_lpAsker))
					continue;

				if	(lpArea && !lpArea->isIn(lpMember->m_pos.x,lpMember->m_pos.y))
					continue;

				lpMember->controlQuestValue(_iType,iQuestIndex,iMethod,iValue,TRUE,iMax);
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(!lpLeader	||	!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			if	(lpArea && !lpArea->isIn(lpLeader->m_pos.x,lpLeader->m_pos.y))
				return	TRUE;

			lpLeader->controlQuestValue(_iType,iQuestIndex,iMethod,iValue,TRUE,iMax);

			return	TRUE;
		}

		case	3	:
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarma::cancelQuest(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType)
{
	if (!_lpAsker)
		return	FALSE;

//	" 퀘스트 [quest]0를 취소시킨다."	,
	int	iQuestIndex		=	_lpReaction->m_aValue[0];
	int	iTarget			=	_lpReaction->m_aValue[1];

	switch(iTarget)
	{
		case	0	:
			return	_lpAsker->cancelQuest(_iType,iQuestIndex,TRUE);

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<(int)lpParty->getMemberCount();i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember	||	!lpMember->isOnSameField(_lpAsker))
					continue;

				lpMember->cancelQuest(_iType,iQuestIndex,TRUE);
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(!lpLeader	||	!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			lpLeader->cancelQuest(_iType,iQuestIndex,TRUE);

			return	TRUE;
		}

		case	3	:
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarma::talk(CKarmaItem *_lpReaction,cACTOR *_lpOwner)
{
	if	(!_lpOwner)
		return	FALSE;

	int	iMethod	=	_lpReaction->m_aValue[0];

	if	(iMethod	==	1)
		iMethod	=	eCT_TALK;
	else
		iMethod	=	eCT_EVENT_CHAT;

	_lpOwner->addSendTalk(_lpReaction->m_strString,iMethod);

	return	TRUE;	
}

BOOL
CKarma::talk(CKarmaItem *_lpReaction,cFIELD *_lpField, cACTOR *_lpAsker)
{
	if	(!_lpField)
		return	FALSE;

	int	iCharacter	=	_lpReaction->m_aValue[0];
	int	iMethod		=	_lpReaction->m_aValue[1];
	int iTalkRange	=	_lpReaction->m_aValue[3];

	if	(iMethod	==	1)
		iMethod	=	eCT_TALK;
	else
		iMethod	=	eCT_EVENT_CHAT;
	


	
	for (int i=0;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR	*lpActor=	_lpField->getActor(i);
		
		if (lpActor		==	NULL)
			continue;
		
		cNPC	*lpNpc	=	lpActor->getNpc();
		
		if	(lpNpc		==	NULL)
			continue;
		
		if	(lpNpc->m_wCharacter	!=	iCharacter	)
			continue;
		
		switch(iTalkRange)
		{
		case 0 :
		case 1 :
			{
				lpActor->addSendTalk(_lpReaction->m_strString,iMethod);
				break;
			}
		case 2 :
			{
				
				if(!_lpAsker)
					break;
				lpActor->addSendTalk(_lpReaction->m_strString,iMethod,_lpAsker->m_iClientSerial);
				break;
			}
		case 3 :
			{
				if(!_lpAsker)
					break;
				cParty* lpParty = _lpAsker->getParty();

				if(!lpParty)
					break;

				for(int i=0; i<lpParty->getMemberCount();++i)
				{
					cACTOR* lpMember = lpParty->getActorByMemberIndex(i);
					if(!lpMember)
						continue;
					lpActor->addSendTalk(_lpReaction->m_strString,iMethod,lpMember->m_iClientSerial);
				}
				
				break;
			}
		}

	}
	return	TRUE;
}

BOOL
CKarma::shout(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if	(!_lpField)
		return	FALSE;

	int	iCharacter	=	_lpReaction->m_aValue[0];
	int	iMethod		=	_lpReaction->m_aValue[1];

	SG_CHAT	sgChat;

	sgChat.bf11Serial	=	0;
	sgChat.bf6ChatType	=	eCT_EVENT_SHOUT;

	FillChattingBuffer(&sgChat,_lpField->m_aCharacter[iCharacter].m_strName,_lpReaction->m_strString);

	_lpField->addSendPacket((ALL_MSG*)&sgChat,0xffff);

	return	TRUE;
}

BOOL
CKarma::dropSetItem(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget)
{
	int	iSetIndex	=	_lpReaction->m_aValue[0]/0x8000;
	int	iIndex		=	_lpReaction->m_aValue[0]%0x8000;
	int	iTarget		=	_lpReaction->m_aValue[1];
	int	iMethod		=	_lpReaction->m_aValue[2];

	cItem	item;

	if	(!g_itemSetManager.generateSetItem(&item,iSetIndex,iIndex))
	{
		printf("failed to make set item!!\n");
		return	FALSE;
	}

	if	(iMethod	==	0	&&	_lpTarget)
		return	addItemByKaram(_lpTarget,&item,iTarget);

	CDroppedItem	droppedItem;

	int		iOwner	=	0xffff;
	cNPC	*lpNpc	=	_lpOwner->getNpc();

	if	(lpNpc)
	{
		DWORD				aDamage[dMAX_BEATER+1];

		memcpy(aDamage,lpNpc->m_aDamage,sizeof(aDamage));

		lpNpc->calcMerit();

		lpNpc->getWinningHitter();

		cACTOR		*lpActor	=	_lpOwner->m_lpField->getTestedActor(iOwner);

		if	(lpActor)
		{
			iOwner			=	lpActor->m_wSerialInField;
			cACTOR	*lpLord	=	lpActor->getControlLord();

			if	(lpLord)
				iOwner	=	lpLord->m_wSerialInField;
		}

		memcpy(lpNpc->m_aDamage,aDamage,sizeof(aDamage));
	}

	droppedItem.m_item.copy(&item);
	droppedItem.m_wOwner			=	iOwner;
	droppedItem.m_item.m_dwSerial	=	largeRandom(0x7ffffff0)+10;
	droppedItem.m_item.m_bCount		=	1;

	int	iRange	=	20;

	for (int i=0;i<1;i++)
	{
		droppedItem.m_pos.x				=	_lpOwner->m_pos.x+random(iRange*2)-iRange;
		droppedItem.m_pos.y				=	_lpOwner->m_pos.y+random(iRange*2)-iRange;

		_lpOwner->m_lpField->dropItem(&droppedItem);
	}

	return	TRUE;
}


BOOL
CKarma::addItemByKaram(cACTOR *_lpTarget,void *_lpItem)
{
	if	(!_lpTarget)
		return	FALSE;

	cItem *lpItem		=	(cItem *)_lpItem;

	lpItem->m_dwSerial	=	largeRandom(0x7ffffff0)+10;

	if (_lpTarget->getValidInventorySlot() == NULL)
	{
		CDroppedItem	droppedItem;
		cFIELD			*lpField=	_lpTarget->m_lpField;

		memcpy(&droppedItem.m_item,lpItem,sizeof(cItem));
		droppedItem.m_item.m_dwSerial	=	largeRandom(0x7ffffff0)+10;
		droppedItem.m_wOwner			=	_lpTarget->m_wSerialInField;
		droppedItem.m_pos.x				=	_lpTarget->m_pos.x+random(40)-20;
		droppedItem.m_pos.y				=	_lpTarget->m_pos.y+random(40)-20;
		droppedItem.m_wSealedTime		=	6000;

		lpField->dropItem(&droppedItem);

		_lpTarget->sendRegistServerMessage(eRSM_CAN_NOT_RECEIVE_ITEM_BY_INVENTORY_FULL);

		return	TRUE;
	}

	_lpTarget->sendPickItemLog(lpItem,1);
	_lpTarget->sendAddItem(lpItem);

	if	(lpItem->isMoneyItem())
		_lpTarget->addGoldItemToInventory(lpItem,eGET_GOLD_BY_KARMA);
	else
		_lpTarget->addItemToInventory(lpItem,FALSE);

	return	TRUE;
}

BOOL
CKarma::addItemByKaram(cACTOR *_lpTarget,void *_lpItem,int _iTarget)
{
	if	(!_lpTarget)
		return	FALSE;

	cItem *lpItem	=	(cItem *)_lpItem;

	if	(_iTarget	==	0)
		return	addItemByKaram(_lpTarget,_lpItem);

	if	(_iTarget	==	1)
	{
		cParty	*lpParty	=	_lpTarget->getParty();

		if	(lpParty)
		{
			for (int i=0;i<(int)lpParty->getMemberCount();i++)
			{
				cItem	item;
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				memcpy(&item,_lpItem,sizeof(cItem));

				if	(lpMember	&&	lpMember->isOnSameField(_lpTarget))
					addItemByKaram(lpMember,&item);
			}

			return	TRUE;
		}

		return	FALSE;
	}

	if (_iTarget	==	2)
	{
		cParty	*lpParty	=	_lpTarget->getParty();

		if	(lpParty)
		{
			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(lpLeader	&&	lpLeader->isOnSameField(_lpTarget))
				return	addItemByKaram(lpLeader,lpItem);
		}
	}

	if (_iTarget	==	3)
	{
		cParty	*lpParty	=	_lpTarget->getParty();

		if (lpParty)
		{
			int		iMember		=	random((int)lpParty->getMemberCount());

			cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(iMember);

			if	(lpMember	&&	lpMember->isOnSameField(_lpTarget))
				return	addItemByKaram(lpMember,_lpItem);
		}
	}

	return	FALSE;
}



BOOL
CKarma::removeItemWithTarget(cACTOR *_lpTarget,int _iItem,int _iCount,int _iTarget,int _iIncludeEquipment)		//09.08.21
{
	if	(!_lpTarget)
		return	FALSE;

	if	(_iTarget	==	0)
		return	_lpTarget->removeItem(_iItem,_iCount,_iIncludeEquipment,eRI_QUEST,eSPENT_GOLDBAR_BY_KARMA);

	if	(_iTarget	==	1)
	{
		cParty	*lpParty	=	_lpTarget->getParty();

		if (lpParty)
		{
			for (int i=0;i<(int)lpParty->getMemberCount();i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(lpMember	&&	lpMember->isOnSameField(_lpTarget))
					lpMember->removeItem(_iItem,_iCount,_iIncludeEquipment,eRI_QUEST,eSPENT_GOLDBAR_BY_KARMA);
			}

			return	TRUE;
		}

		return	FALSE;
	}

	if	(_iTarget	==	2)
	{
		cParty	*lpParty	=	_lpTarget->getParty();
		
		if	(lpParty)
		{
			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(lpLeader	&&	lpLeader->isOnSameField(_lpTarget))
				return	lpLeader->removeItem(_iItem,_iCount,_iIncludeEquipment,eRI_QUEST,eSPENT_GOLDBAR_BY_KARMA);
		}

		return	FALSE;
	}

	if	(_iTarget	==	3)
	{
		cParty	*lpParty	=	_lpTarget->getParty();

		if (lpParty)
		{
			int		iMember		=	random((int)lpParty->getMemberCount());

			cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(iMember);

			if	(lpMember	&&	lpMember->isOnSameField(_lpTarget))
				return	lpMember->removeItem(_iItem,_iCount,_iIncludeEquipment,eRI_QUEST,eSPENT_GOLDBAR_BY_KARMA);
		}

		return	FALSE;
	}

	return	FALSE;
}


BOOL
CKarma::addItemPiece(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget)	//	아이템 조각 추가
{
//	[item_piece]0의 [spin]1[1,15]번째 조각을 [give_drop]2 대상 [quest_target]3 (드랍은 반드시 사망 트리거에 넣을것. 이때 타겟은 무시함)
	int		iPieceItem	=	_lpReaction->m_aValue[0];
	int		iPiece		=	_lpReaction->m_aValue[1]-1;
	int		iMethod		=	_lpReaction->m_aValue[2];
	int		iTarget		=	_lpReaction->m_aValue[3];

	cPieceItem	*lpPieceItem	=	g_pieceItemManager.get(iPieceItem);

	if	(iPiece	>=	lpPieceItem->m_wPieceCount)
	{
		printf("invalid item piece index %d\n",iPiece);
		return	FALSE;
	}

	cItem	itemPiece;

	if	(!g_im.generateItemPiece(&itemPiece,iPieceItem,iPiece,FALSE,FALSE,0,TRUE))
	{
		printf("failed to make item piece index %d\n",iPiece);

		return	FALSE;
	}

	if	(iMethod	==	0)
		return	addItemByKaram(_lpTarget,(void *)&itemPiece,iTarget);

	CDroppedItem	droppedItem;

	int		iOwner	=	0xffff;
	cNPC	*lpNpc	=	_lpOwner->getNpc();

	if	(lpNpc)
	{
		DWORD				aDamage[dMAX_BEATER+1];

		memcpy(aDamage,lpNpc->m_aDamage,sizeof(aDamage));

		lpNpc->calcMerit();

		lpNpc->getWinningHitter();

		cACTOR		*lpActor	=	_lpOwner->m_lpField->getTestedActor(iOwner);

		if	(lpActor)
		{
			iOwner			=	lpActor->m_wSerialInField;
			cACTOR	*lpLord	=	lpActor->getControlLord();

			if	(lpLord)
				iOwner	=	lpLord->m_wSerialInField;
		}

		memcpy(lpNpc->m_aDamage,aDamage,sizeof(aDamage));
	}

	droppedItem.m_item.copy(&itemPiece);
	droppedItem.m_wOwner			=	iOwner;
	droppedItem.m_item.m_dwSerial	=	largeRandom(0x7ffffff0)+10;
	droppedItem.m_item.m_bCount		=	1;

	int	iRange	=	20;

	for (int i=0;i<1;i++)
	{
		droppedItem.m_pos.x				=	_lpOwner->m_pos.x+random(iRange*2)-iRange;
		droppedItem.m_pos.y				=	_lpOwner->m_pos.y+random(iRange*2)-iRange;

		_lpOwner->m_lpField->dropItem(&droppedItem);
	}

	return	TRUE;
}

//JBC	참조	08-09-02
//
//	아이템 추가
BOOL
CKarma::addItem(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	if (!_lpTarget)
		return	FALSE;

//	" [item]0 [number]1[0,255]개를 [add_remove]2"						,
//	{"더한다.","뺀다.","end"},											//	eKARMA_KW_ADD_REMOVE
	int		iItem	=	_lpReaction->m_aValue[0];
	int		iCount	=	_lpReaction->m_aValue[1];
	int		iMethod	=	_lpReaction->m_aValue[2];
	int		iTarget	=	_lpReaction->m_aValue[3];
	int		iIncludeEquipment	=	_lpReaction->m_aValue[4];

	if (iMethod	==	0)
	{
		cItem	addItem;

		g_im.generateItemByBaseItem(&addItem,iItem,iCount);

		if (addItem.isQuestItem())
		{
			CQuestSimpleInfo	*lpQuest;

			if	(addItem.getBasicItem()->m_questKind	==	0)
				lpQuest	=	g_quest.getQuest(addItem.getBasicItem()->m_questIndex);
			else
				lpQuest	=	g_questArbeit.getQuest(addItem.getBasicItem()->m_questIndex);

			if(lpQuest->m_isCloseQuest)
				return	FALSE;
		}

		addItemByKaram(_lpTarget,(void *)&addItem,iTarget);

		return	TRUE;
	}

	return	removeItemWithTarget(_lpTarget,iItem,iCount,iTarget,iIncludeEquipment);
}

BOOL
CKarma::addItemWithOption(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	if (!_lpTarget)
		return	FALSE;

//		" [item]0 1개를 추가한다. 대상 [quest_target]1 옵션 1[spin]2[1,255]~[spin]3[1,255] 옵션 2[spin]4[1,255]~[spin]5[1,255]",					,
//		//	eREACTION_ADD_ITEM_WITH_OPTION
	int		iItem		=	_lpReaction->m_aValue[0];
	int		iTarget		=	_lpReaction->m_aValue[1];

	int		iOption[4] =	{0,};

	iOption[0]	=	_lpReaction->m_aValue[2];
	iOption[1]	=	_lpReaction->m_aValue[3];
	iOption[2]	=	_lpReaction->m_aValue[4];
	iOption[3]	=	_lpReaction->m_aValue[5];

	cItem	itemAdd;

	g_im.generateItemByBaseItem(&itemAdd,iItem,1,-1,iOption);

	if	(itemAdd.isQuestItem())
	{
		CQuestSimpleInfo	*lpQuest;
		
		if	(itemAdd.getBasicItem()->m_questKind	==	0)
			lpQuest	=	g_quest.getQuest(itemAdd.getBasicItem()->m_questIndex);
		else
			lpQuest	=	g_questArbeit.getQuest(itemAdd.getBasicItem()->m_questIndex);
		
		if(lpQuest->m_isCloseQuest)
			return	FALSE;
	}
	
	addItemByKaram(_lpTarget,(void *)&itemAdd,iTarget);
	
	return	TRUE;

}

BOOL
CKarma::addSetItemRandom(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	if (!_lpTarget)
		return	FALSE;

//		"    세트아이템 랜덤 추가"						,eREACTION_ADD_SET_ITEM_RANDOM			,	
//	" [spin]0[0,10]부위 세트아이템을 랜덤으로 대상 [quest_target]1 [spin]2[0,10]에게 부위 추가.(부위가 0 이면 전체랜덤)",

	int		iSetItemKind		=	_lpReaction->m_aValue[0];
	int		iTarget				=	_lpReaction->m_aValue[1];
	int		iPieceCount			=	_lpReaction->m_aValue[2];

	if(g_itemSetManager.m_iCount == 0)
		return FALSE;

	std::vector<int> vecSetItemList;
	for(int i=1;i<g_itemSetManager.m_iCount; ++i)
	{
		cItemSet	*lpItemSet			=	g_itemSetManager.get(i);
		if(!lpItemSet)
			continue;
		if(lpItemSet->getItemCount() == 0)
			continue;
		if(iSetItemKind != 0 && iSetItemKind != lpItemSet->getItemCount())
			continue;
		
		vecSetItemList.push_back(i);
	}

	int iSetItemListCount = vecSetItemList.size();

	if(iSetItemListCount == 0)
	{
		vecSetItemList.clear();	
		return FALSE;
	}

	int iSetItemValue = vecSetItemList[largeRandom(iSetItemListCount)];
	vecSetItemList.clear();	

	cItemSet *lpItemSet = g_itemSetManager.get(iSetItemValue);
	
	if(!lpItemSet)
		return FALSE;

	iPieceCount = min(iPieceCount, lpItemSet->getItemCount());
	if(iPieceCount == 0)
		iPieceCount = lpItemSet->getItemCount();
	if(iSetItemKind == 0)
		iSetItemKind = lpItemSet->getItemCount();

	for(int k=0;k<iSetItemKind;++k)
	{	
		vecSetItemList.push_back(k);
	}

	int iCount =0;
	for(int j=0;j<iPieceCount;++j)
	{
		int iRandValue = random(iSetItemKind-iCount);
		cItem	item;
		
		if	(!g_itemSetManager.generateSetItem(&item,iSetItemValue,vecSetItemList[iRandValue]))
		{
			printf("failed to make set item!!\n");
			vecSetItemList.clear();
			return	FALSE;
		}

		addItemByKaram(_lpTarget,&item,iTarget);
		vecSetItemList[iRandValue] = vecSetItemList[iSetItemKind-iCount-1];
		++iCount;
	}

	vecSetItemList.clear();

	return TRUE;
}
//
//	특정 아이템 추가
BOOL
CKarma::addSpecificItem(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	if (!_lpTarget)
		return	FALSE;

//	" [specific_item]0를 더해준다."	,
	int			iIndex			=	_lpReaction->m_aValue[0];
	int			iTarget			=	_lpReaction->m_aValue[1];
	cFIELD		*lpField		=	_lpTarget->m_lpField;

	CCustomItem	*lpCustomItem	=	lpField->m_customItemList.getItem(iIndex);

	if (!lpCustomItem)
		return	FALSE;

	cItem	addItem;

	g_im.generateItemByCustomItem(&addItem,lpCustomItem);

	return	addItemByKaram(_lpTarget,(void *)&addItem,iTarget);
}

BOOL
CKarma::addGoldByKarmaMulti(cACTOR *_lpTarget,int _iGold,int _iTarget,int _iReason)
{
	if (!_lpTarget)
		return	FALSE;

	if (_iTarget	==	0)
		return	addGoldByKarma(_lpTarget,_iGold,_iReason);

	if (_iTarget	==	1)
	{
		cParty	*lpParty	=	_lpTarget->getParty();

		if (lpParty)
		{
			for (int i=0;i<(int)lpParty->getMemberCount();i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(lpMember	&&	lpMember->isOnSameField(_lpTarget))
					addGoldByKarma(lpMember,_iGold,_iReason);
			}

			return	TRUE;
		}

		return	FALSE;
	}

	if (_iTarget	==	2)	//	리더에게 준다. 헉 이거 무한 루프 였네.. ;
	{
		cParty	*lpParty	=	_lpTarget->getParty();
		cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

		if	(lpLeader	&&	lpLeader->isOnSameField(_lpTarget))
			return	addGoldByKarma(lpLeader,_iGold,_iReason);
	}

	if (_iTarget	==	3)
	{
		cParty	*lpParty	=	_lpTarget->getParty();

		if (lpParty)
		{
			int		iMember		=	random((int)lpParty->getMemberCount());

			cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(iMember);

			if	(lpMember	&&	lpMember->isOnSameField(_lpTarget))
				return	addGoldByKarma(lpMember,_iGold,_iReason);
		}
	}

	return	FALSE;
}

BOOL
CKarma::addGoldByKarma(cACTOR *_lpTarget,int _iGold,int _iReason)
{
	if (!_lpTarget)
		return	FALSE;

	LONGLONG	llGold	=	_lpTarget->m_iGold;

	llGold	+=	_iGold;

	if (llGold	>	dMAX_OWN_GOLD)
	{
		CDroppedItem	droppedItem;
		cFIELD			*lpField=	_lpTarget->m_lpField;

		droppedItem.m_item.reset();
		droppedItem.m_item.m_wBaseItem	=	dITEM_MONEY;
		droppedItem.m_item.m_dwSerial	=	_iGold;
		droppedItem.m_wOwner			=	_lpTarget->m_wSerialInField;
		droppedItem.m_pos.x				=	_lpTarget->m_pos.x+random(40)-20;
		droppedItem.m_pos.y				=	_lpTarget->m_pos.y+random(40)-20;
		droppedItem.m_wSealedTime		=	6000;

		lpField->dropItem(&droppedItem);

		_lpTarget->sendRegistServerMessage(eRSM_CAN_NOT_RECEIVE_ITEM_BY_INVENTORY_FULL);

		return	TRUE;
	}

	_lpTarget->m_iGold	+=	_iGold;
	_lpTarget->sendAddGold(_iGold);

	if	(_iGold)
	{
		if	(_iReason	==	eADD_KARMA_GOLD_REASON_GOLDBAR)
			_lpTarget->updateGoldLog(-_iGold,eSPENT_GOLD_IGNORE);
		else
			_lpTarget->updateGoldLog(_iGold,eGET_GOLD_BY_KARMA);
	}

	cItem	item;

	item.reset();
	item.m_wBaseItem	=	dITEM_MONEY;
	item.m_dwSerial		=	_iGold;

	_lpTarget->sendPickItemLog(&item,1);

	return	TRUE;
}

BOOL
CKarma::removeGoldByKarma(cACTOR *_lpTarget,int _iGold,int _iReason)
{
	if (!_lpTarget)
		return	FALSE;

	if (_lpTarget->m_iGold	<	_iGold)
		return	FALSE;

	_lpTarget->m_iGold	-=	_iGold;

// 	eADD_KARMA_GOLD_REASON_NOTHING	=	0,
// 	eADD_KARMA_GOLD_REASON_GOLDBAR,
// 	eADD_KARMA_GOLD_REASON_QUEST,
	if	(_iReason	==	eADD_KARMA_GOLD_REASON_GOLDBAR)
		_lpTarget->updateGoldLog(-_iGold,eSPENT_GOLD_IGNORE);
	else
		_lpTarget->updateGoldLog(-_iGold,eSPENT_GOLD_BY_KARMA);

	_lpTarget->sendAddGold(-_iGold);

	cItem	item;

	item.reset();
	item.m_wBaseItem	=	dITEM_MONEY;
	item.m_dwSerial		=	_iGold;

	_lpTarget->sendRemoveItemLog(&item,eRI_QUEST);

	return	TRUE;
}

BOOL
CKarma::removeGoldByKarmaMulti(cACTOR *_lpTarget,int _iGold,int _iTarget,int _iReason)
{
	if	(!_lpTarget)
		return	FALSE;

	if (_iTarget	==	0)
		return	removeGoldByKarma(_lpTarget,_iGold,_iReason);

	if (_iTarget	==	1)
	{
		cParty	*lpParty	=	_lpTarget->getParty();

		if (lpParty)
		{
			for (int i=0;i<(int)lpParty->getMemberCount();i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(lpMember	&&	lpMember->isOnSameField(_lpTarget))
					removeGoldByKarma(lpMember,_iGold,_iReason);
			}

			return	TRUE;
		}

		return	FALSE;
	}

	if (_iTarget	==	2)
	{
		cParty	*lpParty	=	_lpTarget->getParty();
		cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

		if	(lpLeader	&&	lpLeader->isOnSameField(_lpTarget))
			return	removeGoldByKarma(lpLeader,_iGold,_iReason);
	}

	if (_iTarget	==	3)
	{
		cParty	*lpParty	=	_lpTarget->getParty();

		if (lpParty)
		{
			int		iMember		=	random((int)lpParty->getMemberCount());

			cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(iMember);

			if	(lpMember	&&	lpMember->isOnSameField(_lpTarget))
				return	removeGoldByKarma(lpMember,_iGold,_iReason);
		}
	}

	return	FALSE;
}

//
//	돈 추가
BOOL
CKarma::addGold(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if (!_lpAsker)
		return	FALSE;

//	" [number]0[0,2147483647]골드를 [add_remove]1"						,
	int		iGold	=	_lpReaction->m_aValue[0];
	int		iMethod	=	_lpReaction->m_aValue[1];
	int		iTarget	=	_lpReaction->m_aValue[2];
	int		iByLevel=	_lpReaction->m_aValue[3];
	int		iReason	=	_lpReaction->m_aValue[4];
	
	//	{"그냥","금괴","퀘스트","end"},								//	eKARMA_KW_ADD_GOLD_REASON

	if	(iByLevel)
		iGold		*=	_lpAsker->m_iLevel;

	if	(iMethod == 0)
		return	addGoldByKarmaMulti(_lpAsker,iGold,iTarget,iReason);

	if	(iMethod == 1)
		return	removeGoldByKarmaMulti(_lpAsker,iGold,iTarget,iReason);

	return	TRUE;
}

//
//	아이템 드랍
BOOL
CKarma::dropGold(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget)
{
	if (!_lpOwner || !_lpTarget)
		return	FALSE;

//	" [number]0[1,2147483647]골드를 떨어뜨린다."						,
	int				iGold			=	_lpReaction->m_aValue[0];
	CDroppedItem	droppedItem;
	cFIELD			*lpField		=	_lpTarget->m_lpField;

	droppedItem.m_item.m_dwSerial	=	iGold;
	droppedItem.m_item.m_wBaseItem	=	dITEM_MONEY;
	droppedItem.m_wOwner			=	_lpTarget->m_wSerialInField;
	droppedItem.m_pos.x				=	_lpOwner->m_pos.x+random(40)-20;
	droppedItem.m_pos.y				=	_lpOwner->m_pos.y+random(40)-20;
	droppedItem.m_wDropMethod		=	eDIM_QUEST_DROP;

	lpField->dropItem(&droppedItem);

	return	TRUE;
}

//
//	아이템 드랍
BOOL
CKarma::dropSpecificItem(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget)
{
	if (!_lpOwner || !_lpTarget)
		return	FALSE;
//	" [specific_item]0 를 떨어뜨린다."									,
	int				iIndex			=	_lpReaction->m_aValue[0];
	cFIELD			*lpField		=	_lpOwner->m_lpField;

	CCustomItem		*lpCustomItem	=	lpField->m_customItemList.getItem(iIndex);

	if	(!lpCustomItem)
		return	FALSE;

	int				iCount			=	lpCustomItem->m_bCount;

	CDroppedItem	droppedItem;

	g_im.generateItemByCustomItem(&droppedItem.m_item,lpCustomItem);
	droppedItem.m_wOwner			=	0xffff;//_lpTarget->m_wSerialInField;
	droppedItem.m_item.m_dwSerial	=	largeRandom(0x7ffffff0)+10;
	droppedItem.m_item.m_bCount		=	1;

	int	iRange	=	20;

	if	(iCount	>	1)
		iRange		=	50;

	for (int i=0;i<iCount;i++)
	{
		droppedItem.m_pos.x				=	_lpOwner->m_pos.x+random(iRange*2)-iRange;
		droppedItem.m_pos.y				=	_lpOwner->m_pos.y+random(iRange*2)-iRange;

		lpField->dropItem(&droppedItem);
	}

	return	TRUE;
}

//
//	아이템 드랍
BOOL
CKarma::dropItem(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget)
{
	if	(!_lpOwner || !_lpTarget)
		return	FALSE;

//	" [special_item]0 [spin]1[1,255]개를 떨어뜨린다."					,
	int				iItem	=	_lpReaction->m_aValue[0];
	int				iCount	=	_lpReaction->m_aValue[1];
	CDroppedItem	droppedItem;
	cFIELD			*lpField=	_lpTarget->m_lpField;

	g_im.generateItemByBaseItem(&droppedItem.m_item,iItem,1);
	droppedItem.m_item.m_dwSerial	=	largeRandom(0x7ffffff0)+10;
	droppedItem.m_wOwner			=	0xffff;//_lpTarget->m_wSerial;

	if	(_lpTarget)
	{
		droppedItem.m_wSealedTime	=	dDROP_ITEM_SEAL_TIME;	//	5분
		droppedItem.m_wOwner		=	_lpTarget->m_wSerialInField;

		cParty	*lpParty			=	_lpTarget->getParty();

		if	(lpParty)
			droppedItem.m_wOwner	=	dPARTY_FLAG+lpParty->m_wSerial;
	}

	int	iRange	=	20;

	if	(iCount	>	1)
		iRange		=	50;

	for (int i=0;i<iCount;i++)
	{
		droppedItem.m_item.m_dwSerial=	largeRandom(0x7ffffff0)+10;
		droppedItem.m_pos.x			=	_lpOwner->m_pos.x+random(iRange*2)-iRange;
		droppedItem.m_pos.y			=	_lpOwner->m_pos.y+random(iRange*2)-iRange;

		lpField->dropItem(&droppedItem);
	}

	return	TRUE;
}


BOOL
CKarma::dropItemWithOption(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget, BOOL _bIsUnowned)
{
	if	(!_lpOwner || !_lpTarget)
		return	FALSE;
	
//	"    아이템 드롭(옵션)"						,eREACTION_DROP_ITEM_WITH_OPTION			,	
//	" [item]0 [spin]1[1,255]개를 떨어뜨린다. 옵션 1[spin]2[1,255]~[spin]3[1,255] 옵션 2[spin]4[1,255]~[spin]5[1,255]",

	int				iItem	=	_lpReaction->m_aValue[0];
	int				iCount	=	_lpReaction->m_aValue[1];
	int				iOption[4] =	{0,};
	
	iOption[0]	=	_lpReaction->m_aValue[2];
	iOption[1]	=	_lpReaction->m_aValue[3];
	iOption[2]	=	_lpReaction->m_aValue[4];
	iOption[3]	=	_lpReaction->m_aValue[5];

	CDroppedItem	droppedItem;
	cFIELD			*lpField=	_lpTarget->m_lpField;

	g_im.generateItemByBaseItem(&droppedItem.m_item,iItem,1,-1,iOption );
	droppedItem.m_item.m_dwSerial	=	largeRandom(0x7ffffff0)+10;
	droppedItem.m_wOwner			=	0xffff;//_lpTarget->m_wSerial;
	
	if	(!_bIsUnowned && _lpTarget)
	{
		droppedItem.m_wSealedTime	=	dDROP_ITEM_SEAL_TIME;	//	5분
		droppedItem.m_wOwner		=	_lpTarget->m_wSerialInField;

		cParty	*lpParty			=	_lpTarget->getParty();

		if	(lpParty)
			droppedItem.m_wOwner	=	dPARTY_FLAG+lpParty->m_wSerial;
	}

	int	iRange	=	20;

	if	(iCount	>	1)
		iRange		=	50;

	for (int i=0;i<iCount;i++)
	{
		droppedItem.m_item.m_dwSerial=	largeRandom(0x7ffffff0)+10;
		droppedItem.m_pos.x			=	_lpOwner->m_pos.x+random(iRange*2)-iRange;
		droppedItem.m_pos.y			=	_lpOwner->m_pos.y+random(iRange*2)-iRange;

		lpField->dropItem(&droppedItem);
	}

	return	TRUE;
}

BOOL
CKarma::buyGoldBar(CKarmaItem *_lpReaction,cACTOR *_lpTarget)		// 09.08.28 금괴추가~~~~~~~~
{

	int			iCount		= _lpReaction->m_aValue[0];
	int			iTax		= _lpTarget->getTitleBankClientTax();
	LONGLONG	llGold		=	100000000 * iCount;
	LONGLONG	llTaxGold	=	( llGold + (llGold * (iTax/10.0)/100.0) );

	if(llTaxGold<=_lpTarget->m_iGold && _lpTarget->getRemainInventorySlotCount())
	{
		_lpTarget->sendEtcWork(eEW_BUY_GOLD_BAR, iCount,iTax);
	}else{
		_lpTarget->sendEtcWork(eEW_BUY_GOLD_BAR_ERR);
	}
	return TRUE;
				
}
BOOL
CKarma::openUpgradeShop(cACTOR *_lpAsker)		//  강화창리액션~~ 09.08.25
{
	if(!_lpAsker)
		return FALSE;

	_lpAsker->sendEtcWork(eEW_OPEN_UPGRADE_SHOP);

	return TRUE;
}
//
//	시스템 공지
BOOL
CKarma::eventNotice(CKarmaItem *_lpReaction,cACTOR *_lpTarget,cFIELD *_lpField)
{
//	" 다음 메시지를 [notice_range]0 [notice_type]1로 [spin]2[1,30]초간 출력한다.  [effectflickeringly]3"	,
	int		iNoticeRange	=	_lpReaction->m_aValue[0];
	int		iNoticeType		=	_lpReaction->m_aValue[1];
	int		iTime			=	_lpReaction->m_aValue[2];
	int		ieffectflickeringly	=	_lpReaction->m_aValue[3];
	char	*lpstrText		=	_lpReaction->m_strString;
	int		iChatType		=	eCT_TALK;

	switch(iNoticeType)
	{
		case	0	:	//	채팅창 메시지
		{
			iChatType	=	eCT_EVENT_NOTICE_IN_CHAT_BOX;
			break;
		}

		case	1	:	//	시스템 메시지
		{
			iChatType	=	eCT_EVENT_NOTICE_IN_SYSTEM_MESSAGE;
			break;
		}

		case	2	:	//	전체 공지
		{
			iChatType	=	eCT_EVENT_NOTICE_IN_NOTICE_BOX;
			break;
		}
	}
//	{"필드 전체에","서버 전체에","주변에","본인만 듣게","소속 파티만 듣게","end"},	//	eKARMA_KW_NOTICE_RANGE
	switch(iNoticeRange)
	{
		case	0	:	//	"필드 전체에"
			if	(_lpField)
				_lpField->addSendEventMessage(lpstrText,iChatType , iTime , ieffectflickeringly);
			break;
		case	1	:	//	"서버 전체에"
			g_game.sendWPEventMessage(lpstrText,iChatType);
			break;
		case	2	:	//	"본인만 듣게"
			if	(_lpTarget)
				_lpTarget->sendEventMessage(lpstrText,FALSE,iChatType, iTime , ieffectflickeringly);
			break;
		case	3	:	//	"소속 파티만 듣게"
			if	(_lpTarget)
				_lpTarget->sendEventMessage(lpstrText,TRUE,iChatType, iTime , ieffectflickeringly);
			break;
	}

	return	TRUE;
}

void 
CKarma::giveState(cACTOR* _lpAsker , int _iState , int _iSecond,cACTOR *_lpOwner)
{
	if(!_lpAsker)
		return;

	if (!_lpOwner)
		return;
	
	if(_lpOwner->m_wSerialInField == _lpAsker->m_wSerialInField)
		return;

	switch(_iState)
	{
	case 0://스턴
		_lpAsker->m_wStunTime = _iSecond * dSYNC_FPS;
		break;
	case 1://프리즈
		_lpAsker->m_wFreezeTime = _iSecond * dSYNC_FPS;
		break;
	case 2://콜드
		_lpAsker->m_wColdTime = _iSecond;
		break;
	case 3://수면
		_lpAsker->m_wSleepTime = _iSecond;
		break;
	case 4://장님
		_lpAsker->m_wBlindTime = _iSecond;
		break;
	case 5://석화
		_lpAsker->m_wStoneTime = _iSecond;
		break;
	case 6://혼란
		_lpAsker->m_wConfuseTime = _iSecond;
		break;
	case 7://갑옥 파괴
		_lpAsker->m_wBreakedArmorTime = _iSecond;
		break;
	case 8://무기 파괴
		_lpAsker->m_wBreakedWeaponTime = _iSecond;
		break;
	case 9://버서크
		_lpAsker->m_wBerserkTime = _iSecond;
		break;
	}
}

BOOL
CKarma::giveBlindEffect(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if (!_lpAsker)
		return	FALSE;
	
	//" [quest_target]0에게 [blind_Range]1 단계 암전효과를 더한다.(제거 하면 단계에 상관없이 제거 됩니다.)",
	int		iTarget		=	_lpReaction->m_aValue[0];
	int		iBlindRange =	_lpReaction->m_aValue[1];
	
	switch(iTarget)
	{
	case 0:
		_lpAsker->m_wBlindByEvent = iBlindRange;
		break;
	case 1:
		{
			cParty *lpParty = _lpAsker->getParty();
			
			if(!lpParty)
			{
				_lpAsker->m_wBlindByEvent = iBlindRange;
			}
			else
			{
				for (int i = 0 ; i < lpParty->getMemberCount() ; i++)
				{
					cACTOR* lpMember = lpParty->getActorByMemberIndex(i);
					
					if(!lpMember)
						continue;

					lpMember->m_wBlindByEvent = iBlindRange;
				}
			}
			break;
		}
	case 2:
		{
			cParty *lpParty = _lpAsker->getParty();
			
			if(!lpParty)
				break;
			
			cACTOR* lpLeader = lpParty->getActorByMemberIndex(0);
			
			if(!lpLeader)
				break;
			
			lpLeader->m_wBlindByEvent = iBlindRange;
			
			break;
		}
	case 3:
		{
			cParty *lpParty = _lpAsker->getParty();
			
			if(!lpParty)
				break;
			
			int	iMember		=	random((int)lpParty->getMemberCount());
			
			cACTOR* lpMember = lpParty->getActorByMemberIndex(iMember);
			
			if(!lpMember)
				break;
			
			lpMember->m_wBlindByEvent = iBlindRange;
			
			break;
			
		}
	}
	
	return	TRUE;
}

BOOL
CKarma::changeState(CKarmaItem *_lpReaction,cACTOR *_lpAsker ,cACTOR *_lpOwner )
{
	if (!_lpAsker)
		return	FALSE;

	if (!_lpOwner)
		return	FALSE;

	if(_lpOwner->m_wSerialInField == _lpAsker->m_wSerialInField)
		return	FALSE;

//	" [quest_target]0에게 [state]1을 [spin]2[1,10000]초 부여 한다.",
	int		iTarget	=	_lpReaction->m_aValue[0];
	int		iState	=	_lpReaction->m_aValue[1];
	int		iSecond	=	_lpReaction->m_aValue[2];

	switch(iTarget)
	{
		case 0:
			giveState(_lpAsker , iState , iSecond ,_lpOwner);
			break;
		case 1:
		{
			cParty *lpParty = _lpAsker->getParty();
			
			if(!lpParty)
			{
				giveState(_lpAsker , iState , iSecond , _lpOwner);
			}
			else
			{
				for (int i = 0 ; i < lpParty->getMemberCount() ; i++)
				{
					cACTOR* lpMember = lpParty->getActorByMemberIndex(i);
					
					if(!lpMember)
						continue;
					
					giveState(lpMember , iState , iSecond , _lpOwner);
				}
			}
			break;

		}
		case 2:
		{
			cParty *lpParty = _lpAsker->getParty();
			
			if(!lpParty)
				break;
			
			cACTOR* lpLeader = lpParty->getActorByMemberIndex(0);
			
			if(!lpLeader)
				break;
			
			giveState(lpLeader , iState , iSecond ,_lpOwner);
			
			break;
		}
		case 3:
		{
			cParty *lpParty = _lpAsker->getParty();
			
			if(!lpParty)
				break;
			
			int	iMember		=	random((int)lpParty->getMemberCount());
			
			cACTOR* lpMember = lpParty->getActorByMemberIndex(iMember);
			
			if(!lpMember)
				break;
			
			giveState(lpMember , iState , iSecond ,_lpOwner);
			
			break;

		}
	}

	return	TRUE;
}


BOOL
CKarma::changeSightRange(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if	(!_lpField)
		return	FALSE;

//	" [all_area]0 안에 [all_character]1의 시야를 [spin]2[1,10000]으로 변경한다.",
	int	iArea				=	_lpReaction->m_aValue[0],i;
	int	iCharacter			=	_lpReaction->m_aValue[1];
	int	iSight				=	_lpReaction->m_aValue[2];

	cAreaInfo	*lpArea		=	_lpField->getArea(iArea);

	if	(!lpArea)
		return	FALSE;

	for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR	*lpActor=	_lpField->getActor(i);

		if	(lpActor		==	NULL)
			continue;

		cNPC	*lpNpc	=	lpActor->getNpc();

		if	(lpNpc		==	NULL)
			continue;

		if	(iCharacter	!=	dCHARACTER_ALL && lpNpc->m_wCharacter != iCharacter	)
			continue;
		if	(!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y)					)
			continue;

		lpActor->m_sSight	=	iSight;
	}

	return	TRUE;
}

BOOL
CKarma::changeBattleInclination(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if	(!_lpField)
		return	FALSE;

//	" [all_area]0 안에 [all_character]1의 전투 성향을 [bi]2로 설정한다."			,
	int	iArea				=	_lpReaction->m_aValue[0],i;
	int	iCharacter			=	_lpReaction->m_aValue[1];
	int	iBattleInclination	=	_lpReaction->m_aValue[2];

	cAreaInfo	*lpArea		=	_lpField->getArea(iArea);

	if (!lpArea)
		return	FALSE;

	for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR	*lpActor=	_lpField->getActor(i);

		if (lpActor		==	NULL)
			continue;

		cNPC	*lpNpc	=	lpActor->getNpc();

		if (lpNpc		==	NULL)
			continue;

		if (iCharacter	!=	dCHARACTER_ALL && lpNpc->m_wCharacter != iCharacter	)
			continue;
		if (!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y)					)
			continue;

		lpActor->m_wBattleInclination	=	iBattleInclination;

		if	(iBattleInclination	<=	dBI_PASSIVE_AGGRESSIVE)
			lpActor->stopBattleForNPC(TRUE);
	}

	return	TRUE;
}

BOOL
CKarma::moveTo(cFIELD *_lpField,cACTOR *_lpTarget,cACTOR *_lpOwner,CKarmaItem *_lpReaction)
{
	int		iCharacter	=	_lpReaction->m_aValue[0];
	int		iArea		=	_lpReaction->m_aValue[1];
	int		iMethod		=	_lpReaction->m_aValue[2];
	int		iMoveMethod	=	_lpReaction->m_aValue[3],i;

	if	(_lpField	==	NULL)
	{
		if	(_lpTarget)
			_lpField=	_lpTarget->m_lpField;
		if	(_lpOwner)
			_lpField=	_lpOwner->m_lpField;
	}

	if	(!_lpField)
		return	FALSE;

	cAreaInfo	*lpArea	=	_lpField->getArea(iArea);
	cACTOR		*lpActor;

	if	(!lpArea)
		return	FALSE;

	if	(iCharacter	==	dCHARACTER_TARGET_PLAYER)
	{
		lpActor	=	_lpTarget;

		if	(lpActor->m_bIsMoving	&&	lpArea->isIn(lpActor->m_posDest.x,lpActor->m_posDest.y))
			return	FALSE;

		if	(lpActor->isPlayer())
			lpActor->stopBattle();
		else
			lpActor->stopBattleForNPC(TRUE);

		lpActor->moveToArea(iArea,iMoveMethod,iMethod);
	}
	else
	if (iCharacter	==	dCHARACTER_TARGET_PARTY	)
	{
		if	(_lpTarget->getPartySerial()	!=	0xffff)
		{
			cParty	party;

			g_pPartyManager->getParty(&party,_lpTarget->getPartySerial());

			for (i=0;i<(int)party.getMemberCount();i++)
			{
				lpActor	=	party.getActorByMemberIndex(i);

				if	(!lpActor	||	 !lpActor->isOnSameField(_lpTarget))
					continue;

				if	(lpActor->m_bIsMoving	&&	lpArea->isIn(lpActor->m_posDest.x,lpActor->m_posDest.y))
					continue;

				if	(lpActor->isPlayer())
					lpActor->stopBattle();
				else
					lpActor->stopBattleForNPC(TRUE);

				lpActor->moveToArea(iArea,iMoveMethod,iMethod);
			}
		}
		else
		{
			lpActor	=	_lpTarget;

			if	(lpActor->m_bIsMoving	&&	lpArea->isIn(lpActor->m_posDest.x,lpActor->m_posDest.y))
				return	FALSE;

			if	(lpActor->isPlayer())
				lpActor->stopBattle();
			else
				lpActor->stopBattleForNPC(TRUE);

			lpActor->moveToArea(iArea,iMoveMethod,iMethod);
		}
	}
	else
	for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR	*lpActor=	_lpField->getActor(i);

		if	(lpActor		==	NULL)
			continue;

		cNPC	*lpNpc	=	lpActor->getNpc();

		if	(lpNpc		==	NULL)
			continue;

		if	(iCharacter	!=	dCHARACTER_ALL && lpNpc->m_wCharacter != iCharacter	)
			continue;

		if	(lpActor->m_bIsMoving	&&	lpArea->isIn(lpActor->m_posDest.x,lpActor->m_posDest.y))
			continue;

		if	(lpActor->isPlayer())
			lpActor->stopBattle();
		else
			lpActor->stopBattleForNPC(TRUE);

		lpActor->moveToArea(iArea,iMoveMethod,iMethod);
	}

	return	TRUE;
}

//	영역 이동
BOOL
CKarma::moveArea(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if	(!_lpAsker)
	{
		if	(g_bIsWantPrintTriggerCondition)
			printf("영역이동 실패 : 대상이 없대요.\n");

		return	FALSE;
	}

	int	iArea1	=	_lpReaction->m_aValue[0];
	int	iWhere	=	_lpReaction->m_aValue[1];
	int	iTarget	=	_lpReaction->m_aValue[2];
	int	iArea2	=	_lpReaction->m_aValue[3];

	cFIELD		*lpField	=	_lpAsker->m_lpField;

	return	lpField->changeActorsPos(iArea1,iArea2,iTarget,iWhere,_lpAsker);
}

//	OX 퀴즈 결과
BOOL
CKarma::selectOXQuizResult(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if (!_lpField)
		return	FALSE;

	int	iSolution	=	_lpReaction->m_aValue[0];
	int	iTime		=	_lpReaction->m_aValue[1];

//	g_eventManager.setOXQuizSolution(iSolution,iTime);

	SG_START_COUNTDOWN	packet;

	packet.base.set(sizeof(packet),dSG_START_COUNTDOWN);
	packet.wTime	=	iTime;

	_lpField->addSendPacket((ALL_MSG *)&packet,0xffff);

//	" OX 결과 선택"							,eREACTION_SELECT_OX_RESULT	,//	시스템 공지
//	" 문제의 답은 [ox]0이다. 자동으로 [spin]1[1,600]초간 카운트 다운을 시작한다. "	,

	return	TRUE;
}

void
getDamageToCharacterDamage(cACTOR *_lpActor,int _iMethod,int _iMinValue,int _iMaxValue,int _iType,__int64 *_lpiPhysicalDamage,__int64 *_lpiMagicDamage)
{
	if	(_iMethod	==	1)
	{
		int	iMinDamage	=	_lpActor->m_iHP*_iMinValue/100;
		int	iMaxDamage	=	_lpActor->m_iHP*_iMaxValue/100;
		int	iDamage		=	iMinDamage+largeRandom(iMaxDamage-iMinDamage);;

		if	(_iType	==	eRD_PHYSICAL)
			*_lpiPhysicalDamage	=	iDamage;
		if	(_iType	==	eRD_MAGIC)
			*_lpiMagicDamage	=	iDamage;
	}

	if	(_iMethod	==	2)
	{
		int	iMinDamage	=	_lpActor->getMaxHP()*_iMinValue/100;
		int	iMaxDamage	=	_lpActor->getMaxHP()*_iMaxValue/100;
		int	iDamage		=	iMinDamage+largeRandom(iMaxDamage-iMinDamage);;

		if	(_iType	==	eRD_PHYSICAL)
			*_lpiPhysicalDamage	=	iDamage;
		if	(_iType	==	eRD_MAGIC)
			*_lpiMagicDamage	=	iDamage;
	}
}

//
//	대미지 입히기
BOOL
CKarma::damageToCharacter(CKarmaItem *_lpReaction,cACTOR *_lpTarget,cACTOR *_lpOwner)
{
	if	(!_lpTarget	||	_lpTarget->m_wSerialInField	==	0xffff)
		return	FALSE;

//	" [all_area]0 안에 [all_character_target_player]1에게 [number]2[1,1000000]~[number]3[1,1000000] 포인트의 [physical_magic]4 대미지를 입힌다.",
	int	iArea		=	_lpReaction->m_aValue[0],i;
	int	iCharacter	=	_lpReaction->m_aValue[1];
	int	iMinValue	=	_lpReaction->m_aValue[2];
	int	iMaxValue	=	_lpReaction->m_aValue[3];
	int	iMinDamage	=	_lpReaction->m_aValue[2]*100;
	int	iMaxDamage	=	_lpReaction->m_aValue[3]*100;
	int	iType		=	_lpReaction->m_aValue[4];
	int	iMethod		=	_lpReaction->m_aValue[5];
	__int64	iHitDamage,iPhysicalDamage,iMagicDamage;

	if	(iMinValue	>	iMaxValue)
		nsSfc::swap(iMinValue,iMaxValue);

	if	(iMinDamage > iMaxDamage) 
	{
		iHitDamage	=	iMinDamage;
		iMinDamage	=	iMaxDamage;
		iMaxDamage	=	iHitDamage;
	}

	int	iDamageRange=	iMaxDamage-iMinDamage;

	cFIELD		*lpField	=	_lpTarget->m_lpField;
	cAreaInfo	*lpArea		=	lpField->getArea(iArea);

	if	(!lpArea)
		return	FALSE;

	SG_DAMAGE_TO_ACTOR_BY_KARMA	packet;

	packet.base.set(sizeof(packet),dSG_DAMAGE_TO_ACTOR_BY_KARMA);
	packet.wCount		=	0;
	packet.wDamageType	=	iType;

	iHitDamage			=	iMinDamage+random(iDamageRange);
	iPhysicalDamage		=	0,iMagicDamage	=	0;

	if	(iType		==	eRD_PHYSICAL)
		iPhysicalDamage	=	iHitDamage;
	if	(iType		==	eRD_MAGIC	)
		iMagicDamage	=	iHitDamage;

	try
	{
		if	(iCharacter	==	dCHARACTER_SELF)
		{
			if	(lpArea->isIn(_lpOwner->m_pos.x,_lpOwner->m_pos.y))
			{
				if	(_lpOwner->m_wInvincibleTime	==	0)
				{
					getDamageToCharacterDamage(_lpOwner,iMethod,iMinValue,iMaxValue,iType,&iPhysicalDamage,&iMagicDamage);
					_lpOwner->damagedBy(NULL,NULL,iPhysicalDamage,iMagicDamage,FALSE,FALSE);

					packet.aData[packet.wCount].wActor	=	_lpOwner->m_wSerialInField;
					packet.aData[packet.wCount].wDamage	=	max((iPhysicalDamage+iMagicDamage)/100,1);
				}
				else
				{
					packet.aData[packet.wCount].wActor	=	_lpOwner->m_wSerialInField;
					packet.aData[packet.wCount].wDamage	=	0;
				}

				packet.wCount=	1;
			}
		}
		else
		if	(iCharacter	==	dCHARACTER_PLAYER)
		{
			for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
			{
				cACTOR	*lpActor=	lpField->getActor(i);

				if	(lpActor	==	NULL	||	lpActor->isDeath())
					continue;

				if	(lpActor->m_wTeam	>	lpField->m_bIsGuildBattleField)
					continue;

				if	(!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y))
					continue;

				if	(lpActor->m_wInvincibleTime	==	0)
				{
					getDamageToCharacterDamage(lpActor,iMethod,iMinValue,iMaxValue,iType,&iPhysicalDamage,&iMagicDamage);
					lpActor->damagedBy(NULL,NULL,iPhysicalDamage,iMagicDamage,FALSE,FALSE);

					packet.aData[packet.wCount].wActor	=	lpActor->m_wSerialInField;
					packet.aData[packet.wCount].wDamage	=	max((iPhysicalDamage+iMagicDamage)/100,1);;
				}
				else
				{
					packet.aData[packet.wCount].wActor	=	lpActor->m_wSerialInField;
					packet.aData[packet.wCount].wDamage	=	0;
				}

				packet.wCount++;

				if	(packet.wCount	>=	dMAX_TARGET_FOR_DAMAGE_TO_ACTOR_BY_KARMA		)
				{
					lpField->addSendPacket((ALL_MSG*)&packet,lpActor->m_wSerialInField);

					packet.wCount	=	0;
				}
			}
		}
		else
		if	(iCharacter	==	dCHARACTER_TARGET_PLAYER)
		{
			if	(lpArea->isIn(_lpTarget->m_pos.x,_lpTarget->m_pos.y))
			{
				if	(_lpTarget->m_wInvincibleTime	==	0)
				{
					getDamageToCharacterDamage(_lpTarget,iMethod,iMinValue,iMaxValue,iType,&iPhysicalDamage,&iMagicDamage);
					_lpTarget->damagedBy(NULL,NULL,iPhysicalDamage,iMagicDamage,FALSE,FALSE);

					packet.aData[packet.wCount].wActor	=	_lpTarget->m_wSerialInField;
					packet.aData[packet.wCount].wDamage	=	max((iPhysicalDamage+iMagicDamage)/100,1);;
				}
				else
				{
					packet.aData[packet.wCount].wActor	=	_lpTarget->m_wSerialInField;
					packet.aData[packet.wCount].wDamage	=	0;
				}
				
				packet.wCount=	1;
			}
		}
		else
		if	(iCharacter	==	dCHARACTER_TARGET_PARTY	)
		{
			if	(_lpTarget->getPartySerial()	!=	0xffff)
			{
				cParty	party;

				g_pPartyManager->getParty(&party,_lpTarget->getPartySerial());

				for (i=0;i<(int)party.getMemberCount();i++)
				{
					cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

					if	(!lpMember	||	!lpMember->isOnSameField(_lpTarget))
						continue;

					if	(!lpArea->isIn(lpMember->m_pos.x,lpMember->m_pos.y))
						continue;

					if	(lpMember->m_wInvincibleTime	==	0)
					{
						getDamageToCharacterDamage(lpMember,iMethod,iMinValue,iMaxValue,iType,&iPhysicalDamage,&iMagicDamage);

						lpMember->damagedBy(NULL,NULL,iPhysicalDamage,iMagicDamage,FALSE,FALSE);

						packet.aData[packet.wCount].wActor	=	lpMember->m_wSerialInField;
						packet.aData[packet.wCount].wDamage	=	max((iPhysicalDamage+iMagicDamage)/100,1);;
					}
					else
					{
						packet.aData[packet.wCount].wActor	=	lpMember->m_wSerialInField;
						packet.aData[packet.wCount].wDamage	=	0;
					}

					packet.wCount++;
				}
			}
			else
			{
				if	(!lpArea->isIn(_lpTarget->m_pos.x,_lpTarget->m_pos.y))
					return	TRUE;

				if	(_lpTarget->m_wInvincibleTime	==	0)
				{
					getDamageToCharacterDamage(_lpTarget,iMethod,iMinValue,iMaxValue,iType,&iPhysicalDamage,&iMagicDamage);
					_lpTarget->damagedBy(NULL,NULL,iPhysicalDamage,iMagicDamage,FALSE,FALSE);

					packet.aData[packet.wCount].wActor	=	_lpTarget->m_wSerialInField;
					packet.aData[packet.wCount].wDamage	=	max((iPhysicalDamage+iMagicDamage)/100,1);;
				}
				else
				{
					packet.aData[packet.wCount].wActor	=	_lpTarget->m_wSerialInField;
					packet.aData[packet.wCount].wDamage	=	0;
				}

				packet.wCount=	1;
			}
		}
		else
		for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
		{
			cACTOR	*lpActor=	lpField->getActor(i);

			if	(lpActor		==	NULL)
				continue;

			cNPC	*lpNpc	=	lpActor->getNpc();

			if	(lpNpc		==	NULL)
				continue;

			if	(iCharacter	!=	dCHARACTER_ALL && lpNpc->m_wCharacter != iCharacter	)
				continue;
			if	(!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y)					)
				continue;
			
			if	(lpActor->m_wInvincibleTime	==	0)
			{
				getDamageToCharacterDamage(lpActor,iMethod,iMinValue,iMaxValue,iType,&iPhysicalDamage,&iMagicDamage);
				lpActor->damagedBy(NULL,NULL,iPhysicalDamage,iMagicDamage,FALSE,FALSE);

				packet.aData[packet.wCount].wActor	=	lpActor->m_wSerialInField;
				packet.aData[packet.wCount].wDamage	=	max((iPhysicalDamage+iMagicDamage)/100,1);
			}
			else
			{
				packet.aData[packet.wCount].wActor	=	lpActor->m_wSerialInField;
				packet.aData[packet.wCount].wDamage	=	0;
			}

			packet.wCount++;

			if	(packet.wCount	>=	dMAX_TARGET_FOR_DAMAGE_TO_ACTOR_BY_KARMA		)
			{
				lpField->addSendPacket((ALL_MSG*)&packet,_lpTarget->m_wSerialInField);

				packet.wCount	=	0;
			}
		}
	}

	catch(...)
	{
		
	}

	if	(packet.wCount)
	{
		packet.base.set(0,dSG_DAMAGE_TO_ACTOR_BY_KARMA);
		packet.base.wSize	=	sizeof(packet) - sizeof(packet.aData) + packet.wCount * sizeof(CDamageToActorByKarmaE);

		lpField->addSendPacket((ALL_MSG*)&packet,_lpTarget->m_wSerialInField);
	}

	if (_lpTarget->getParty())
	{
	}

	return	TRUE;
}

//	펫 추가
BOOL
CKarma::addPet(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{	// 마지막 수정일 : 09.10.25
	int	iLevel	=	_lpReaction->m_aValue[0];
	int	iJob	=	_lpReaction->m_aValue[1];

	cPetDefine	petInfo;

	petInfo.reset();

	petInfo.setJobIndex(iJob);

	petInfo.m_bf10Level		=	iLevel;
	petInfo.m_bf14Loyalty	=	0;	// 충성도 0으로 .~~
	petInfo.m_bf22CurrentHP	=	4000000;
	petInfo.m_bf2TamerSlot	=	_lpTarget->m_wAvatarIndex;

	strcpy(petInfo.m_strName,"noname");

	int	iPetIndex	=	_lpTarget->addPet(&petInfo);

	if	(iPetIndex	==	0xffff)
		return	FALSE;

	_lpTarget->m_lpField->bookingGeneratePet(_lpTarget->m_strName,iPetIndex,FALSE);

	return	iPetIndex;
}

//	펫 이름 초기화
BOOL
CKarma::resetPetName(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	if (!_lpTarget)
		return	FALSE;

	int	iPetIndex		=	_lpReaction->m_aValue[0];

	_lpTarget->changePetName(iPetIndex,"noname",FALSE,FALSE);

	return	TRUE;
}

BOOL
CKarma::launchRandomKarma(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cACTOR *_lpOwner,BOOL *_lpbIsStopWork,int _iPauseReaction,cFIELD *_lpField,int *_lpiJumpKarma,CKarmaContainer *_lpKC)
{
	if	(!_lpKC)
		return	FALSE;

	int		iBegin		=	_lpReaction->m_aValue[0];
	int		iEnd		=	_lpReaction->m_aValue[1];
	int		iLaunchCount=	_lpReaction->m_aValue[2];
	int		iJumpKarma	=	_lpReaction->m_aValue[3];
	int		iJumpNotJump=	_lpReaction->m_aValue[4];

	if	(iBegin	>	iEnd)
	{
		int	iTemp	=	iBegin;

		iBegin		=	iEnd;
		iEnd		=	iTemp;
	}

	for (;iLaunchCount;iLaunchCount--)
	{
		int	iKarma	=	random(iEnd-iBegin+1)+iBegin;

		if	(iKarma	>=	_lpKC->m_wKarmaCount)
			continue;

		_lpKC->m_pKarma[iKarma].operate(_lpAsker,_lpOwner,_lpbIsStopWork,_lpField,FALSE,_lpiJumpKarma,_lpKC);
	}

	if	(_lpiJumpKarma)
	{
		if	(iJumpNotJump	==	0)
			*_lpiJumpKarma	=	iJumpKarma;
	}

	return	TRUE;
}

BOOL
CKarma::launchRandomReaction(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cACTOR *_lpOwner,BOOL *_lpbIsStopWork,int _iPauseReaction,cFIELD *_lpField,int *_lpiJumpKarma,int *_lpiJumpReaction,CKarmaContainer *_lpKC)
{
	int		iBegin		=	_lpReaction->m_aValue[0];
	int		iEnd		=	_lpReaction->m_aValue[1];
	int		iLaunchCount=	_lpReaction->m_aValue[2];
	int		iJumpReaction=	_lpReaction->m_aValue[3];
	int		iJumpNotJump=	_lpReaction->m_aValue[4];

	if	(iBegin	>	iEnd)
	{
		int	iTemp	=	iBegin;

		iBegin		=	iEnd;
		iEnd		=	iTemp;
	}

	for (;iLaunchCount;iLaunchCount--)
	{
		int	iReaction	=	random(iEnd-iBegin+1)+iBegin;

		if	(iReaction	>=	m_wReactionCount)
			continue;

		operateReactionItem(&m_pReaction[iReaction],&iReaction,_lpAsker,_lpOwner,_lpbIsStopWork,_iPauseReaction,_lpField,_lpiJumpKarma,_lpKC);
	}

	if	(_lpiJumpReaction)
	{
		if	(iJumpNotJump	==	0)	//	지정된 점프로 이동
			*_lpiJumpReaction	=	iJumpReaction;
		else
			*_lpiJumpReaction	=	iEnd+1;
	}

	return	TRUE;
}

//	지연
BOOL
CKarma::delay(CKarmaItem *_lpReaction,int iReactionIndex,cACTOR *_lpOwner,cACTOR *_lpTarget,cFIELD *_lpField)
{
	if	(!_lpOwner)
		_lpOwner = _lpTarget;

	if	(!_lpTarget || !_lpOwner || !_lpField)
		return	FALSE;

	int		iDelayMinute	=	_lpReaction->m_aValue[0];
	int		iDelaySecond	=	_lpReaction->m_aValue[1];
	int		iDelayMilisecond=	_lpReaction->m_aValue[2];
	DWORD	dwReleaseDelayTime=	g_dwCurrentTime+(iDelayMinute*60+iDelaySecond)*1000+iDelayMilisecond;

	g_delayKarmaManager.addDelayKarma(this,iReactionIndex+1,dwReleaseDelayTime,_lpOwner,_lpTarget,_lpField);

	return	TRUE;
}

//
//	경험치 얻기
BOOL
CKarma::getExperience(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	if	(!_lpTarget)
		return	FALSE;

//	" [all_area]0에 [target_player]1의 경험치가 [number]2[1,2147483647] 오른다."	,
	int		iArea			=	_lpReaction->m_aValue[0],i;
	int		iTargetType		=	_lpReaction->m_aValue[1];
	int		iExperience		=	_lpReaction->m_aValue[2];
	int		iCheckPremium	=	_lpReaction->m_aValue[3];
	int		iQuest			=	_lpReaction->m_aValue[4];
	int		iQuestArbeit	=	_lpReaction->m_aValue[5];

	int		iQuestCode		=	iQuest;

	if	(iQuestArbeit)
		iQuestCode			=	dQUEST_AND_ARBEIT_BORDER_VALUE+iQuestArbeit;

	int		iGiveExperience	=	iExperience;
	BOOL	bIgnorePremiumBonus	=	FALSE;

	cFIELD		*lpField	=	_lpTarget->m_lpField;
	cAreaInfo	*lpArea		=	lpField->getArea(iArea);

	if	(iCheckPremium	==	2)
	{
		bIgnorePremiumBonus	=	TRUE;
		iExperience			=	min(iExperience,100);
	}

	if	(!lpArea)
		return	FALSE;

	if	(iTargetType	==	0	||	_lpTarget->getPartySerial()	==	0xffff)
	{
		if	(lpArea->isIn(_lpTarget->m_pos.x,_lpTarget->m_pos.y))
		{
			char	strName[128];

			if	(m_pstrTitle)
				strncpy(strName,m_pstrTitle,120);
			else
				strcpy(strName,"invalid");

			if	(iCheckPremium	==	2)
				iGiveExperience	=	iExperience*g_aExpTable[_lpTarget->m_iLevel]/100;

			_lpTarget->checkReceiveQuestExp(iExperience,iQuest,iQuestArbeit);

			_lpTarget->increaseExperience(iGiveExperience,TRUE,TRUE,bIgnorePremiumBonus,iQuestCode);
		}
	}
	else
	if	(iTargetType	==	1)
	{
		cParty	party;

		g_pPartyManager->getParty(&party,_lpTarget->getPartySerial());

		for (i=0;i<(int)party.getMemberCount();i++)
		{
			cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

			if	(!lpMember || 	!lpMember->isOnSameField(_lpTarget))
				continue;

			if	(lpArea->isIn(lpMember->m_pos.x,lpMember->m_pos.y))
			{
				char	strName[128];

				if	(m_pstrTitle)
					strncpy(strName,m_pstrTitle,120);
				else
					strcpy(strName,"invalid");

				if	(iCheckPremium	==	2)
					iGiveExperience	=	iExperience*g_aExpTable[lpMember->m_iLevel]/100;

				lpMember->checkReceiveQuestExp(iExperience,iQuest,iQuestArbeit);

				lpMember->increaseExperience(iGiveExperience,TRUE,TRUE,bIgnorePremiumBonus,iQuestCode);
			}
		}
	}

	return	TRUE;
}

BOOL
CKarma::getExperience2(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	if	(!_lpTarget)
		return	FALSE;
	
	//	" [all_area]0에 [target_player]1의 경험치가 1레벨 업하는 필요한 경험치의 [number]2[1,100]% 오른다. [apply_premium_bonus]3"	,
	int		iArea			=	_lpReaction->m_aValue[0],i;
	int		iTargetType		=	_lpReaction->m_aValue[1];
	int		iExperience		=	_lpReaction->m_aValue[2];
	int		iCheckPremium	=	_lpReaction->m_aValue[3];

	int		iQuest			=	_lpReaction->m_aValue[4];
	int		iQuestArbeit	=	_lpReaction->m_aValue[5];

	int		iQuestCode		=	iQuest;

	if	(iQuestArbeit)
		iQuestCode			=	dQUEST_AND_ARBEIT_BORDER_VALUE+iQuestArbeit;
	
	int		iGiveExperience	=	iExperience;
	BOOL	bIgnorePremiumBonus	=	FALSE;
	
	cFIELD		*lpField	=	_lpTarget->m_lpField;
	cAreaInfo	*lpArea		=	lpField->getArea(iArea);
	int			iGetExp		=	0;
	
	if	(iCheckPremium	==	2)
	{
		bIgnorePremiumBonus	=	TRUE;
		iExperience			=	min(iExperience,100);
	}
	
	if	(!lpArea)
		return	FALSE;
	
	if	(iTargetType	==	0	||	_lpTarget->getPartySerial()	==	0xffff)
	{
		if	(lpArea->isIn(_lpTarget->m_pos.x,_lpTarget->m_pos.y))
		{
			char	strName[128];
			
			if	(m_pstrTitle)
				strncpy(strName,m_pstrTitle,120);
			else
				strcpy(strName,"invalid");

// 			if	(iCheckPremium	==	2)
// 				iGiveExperience	=	iExperience*g_aExpTable[_lpTarget->m_iLevel]/100;

			iGetExp = 0;

			iGetExp = (g_aExpTable[_lpTarget->m_iLevel] * iGiveExperience) /100;
			
			_lpTarget->checkReceiveQuestExp(iExperience,iQuest,iQuestArbeit);

			_lpTarget->increaseExperience(iGetExp,TRUE,TRUE,bIgnorePremiumBonus,iQuestCode);
		}
	}
	else
	if	(iTargetType	==	1)
	{
		cParty	party;
		
		g_pPartyManager->getParty(&party,_lpTarget->getPartySerial());
		
		for (i=0;i<(int)party.getMemberCount();i++)
		{
			cACTOR	*lpMember	=	party.getActorByMemberIndex(i);
			
			if	(!lpMember || 	!lpMember->isOnSameField(_lpTarget))
				continue;
			
			if	(lpArea->isIn(lpMember->m_pos.x,lpMember->m_pos.y))
			{
				char	strName[128];
				
				if	(m_pstrTitle)
					strncpy(strName,m_pstrTitle,120);
				else
					strcpy(strName,"invalid");
				
// 				if	(iCheckPremium	==	2)
// 					iGiveExperience	=	iExperience*g_aExpTable[lpMember->m_iLevel]/100;

				iGetExp = 0;

				iGetExp = (g_aExpTable[lpMember->m_iLevel] * iGiveExperience) /100;
				
				lpMember->checkReceiveQuestExp(iExperience,iQuest,iQuestArbeit);

				lpMember->increaseExperience(iGetExp,TRUE,TRUE,bIgnorePremiumBonus,iQuestCode);
			}
		}
	}
	
	return	TRUE;
}

//
//	칭호 획득
BOOL
CKarma::getTitle(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	if (!_lpTarget)
		return	FALSE;
//	" [target_player]0가 [spin]1[1,10]레벨 [title]2 칭호를 얻는다."	,
	int	iTargetType		=	_lpReaction->m_aValue[0],i;
	int	iTitleLevel		=	_lpReaction->m_aValue[1];
	int	iTitle			=	_lpReaction->m_aValue[2];
	int	iAddRemove		=	_lpReaction->m_aValue[3];

	cFIELD		*lpField=	_lpTarget->m_lpField;

	if (iTargetType	==	0 || _lpTarget->getPartySerial()	==	0xffff)
	{
		if	(iAddRemove	==	0)
			_lpTarget->addTitle(iTitle,iTitleLevel);
		else
			_lpTarget->removeTitle(iTitle);
	}
	else
	if (iTargetType	==	1)
	{
		cParty	party;

		g_pPartyManager->getParty(&party,_lpTarget->getPartySerial());

		for (i=0;i<(int)party.getMemberCount();i++)
		{
			cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

			if	(!lpMember	||	 !lpMember->isOnSameField(_lpTarget))
				continue;

			if	(iAddRemove	==	0)
				lpMember->addTitle(iTitle,iTitleLevel);
			else
				lpMember->removeTitle(iTitle);
		}
	}

	return	TRUE;
}

//
//	챠징
BOOL
CKarma::chargeCP(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	if (!_lpTarget)
		return	FALSE;
//	" [all_area]0 안에 [all_character_target_player]1의 CP를 [spin]2[1,100]％ 충전 시킨다.",
	int	iArea				=	_lpReaction->m_aValue[0],i;
	int	iCharacter			=	_lpReaction->m_aValue[1];
	int	iChargePercentage	=	_lpReaction->m_aValue[2];

	cFIELD		*lpField	=	_lpTarget->m_lpField;
	cAreaInfo	*lpArea		=	lpField->getArea(iArea);

	if (!lpArea)	return	FALSE;

	if (iCharacter	==	dCHARACTER_TARGET_PLAYER)
	{
		if (lpArea->isIn(_lpTarget->m_pos.x,_lpTarget->m_pos.y))
		{
			_lpTarget->increaseCP(_lpTarget->getMaxCP()*iChargePercentage/100);
			_lpTarget->sendPlayerBasicInformation();
		}
	}
	else
	if (iCharacter	==	dCHARACTER_TARGET_PARTY	)
	{
		if (_lpTarget->getPartySerial()	!=	0xffff)
		{
			cParty	party;

			g_pPartyManager->getParty(&party,_lpTarget->getPartySerial());

			for (i=0;i<(int)party.getMemberCount();i++)
			{
				cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

				if	(!lpMember	||	 !lpMember->isOnSameField(_lpTarget))
					continue;

				if (lpArea->isIn(lpMember->m_pos.x,lpMember->m_pos.y))
				{
					lpMember->increaseCP(lpMember->getMaxCP()*iChargePercentage/100);
					lpMember->sendPlayerBasicInformation();
				}
			}
		}
		else
		{
			if (lpArea->isIn(_lpTarget->m_pos.x,_lpTarget->m_pos.y))
			{
				_lpTarget->increaseCP(_lpTarget->getMaxCP()*iChargePercentage/100);
				_lpTarget->sendPlayerBasicInformation();
			}
		}
	}
	else
	for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR	*lpActor=	lpField->getActor(i);

		if (lpActor		==	NULL)	continue;

		cNPC	*lpNpc	=	lpActor->getNpc();

		if (lpNpc		==	NULL)	continue;

		if (iCharacter	!=	dCHARACTER_ALL && lpNpc->m_wCharacter != iCharacter	)
			continue;
		if (!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y)					)
			continue;

		lpActor->increaseCP(lpActor->getMaxCP()*iChargePercentage/100);
		lpActor->sendPlayerBasicInformation();
	}

	return	TRUE;
}

//
//	체력 회복
BOOL
CKarma::healHP(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	if (!_lpTarget)
		return	FALSE;
//	" [all_area]0 안에 [all_character_target_player]1의 체력을 [spin]2[1,100]％ 회복 시킨다."
	int	iArea				=	_lpReaction->m_aValue[0],i;
	int	iCharacter			=	_lpReaction->m_aValue[1];
	int	iHealPercentage		=	_lpReaction->m_aValue[2];

	cFIELD		*lpField	=	_lpTarget->m_lpField;
	cAreaInfo	*lpArea		=	lpField->getArea(iArea);

	if	(!lpArea)
		return	FALSE;

	if	(iCharacter	==	dCHARACTER_TARGET_PLAYER)
	{
		if	(lpArea->isIn(_lpTarget->m_pos.x,_lpTarget->m_pos.y))
		{
			LONGLONG	llHealHP	=	_lpTarget->getMaxHP();
			llHealHP	=	llHealHP*iHealPercentage/100;

			_lpTarget->instanceHealHP((int)llHealHP);
			_lpTarget->syncPlayerBasicStatus(TRUE);
			_lpTarget->addSendSimpleInfo();
		}
	}
	else
	if	(iCharacter	==	dCHARACTER_TARGET_PARTY	)
	{
		if	(_lpTarget->getPartySerial()	!=	0xffff)
		{
			cParty	party;

			g_pPartyManager->getParty(&party,_lpTarget->getPartySerial());

			for (i=0;i<(int)party.getMemberCount();i++)
			{
				cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

				if	(!lpMember	||	 !lpMember->isOnSameField(_lpTarget))
					continue;

				if	(lpArea->isIn(lpMember->m_pos.x,lpMember->m_pos.y))
				{
					LONGLONG	llHealHP	=	lpMember->getMaxHP();
					llHealHP	=	llHealHP*iHealPercentage/100;

					if	(lpMember->instanceHealHP((int)llHealHP))
					{
						lpMember->syncPlayerBasicStatus(TRUE);
						lpMember->addSendSimpleInfo();
					}
				}
			}
		}
		else
		{
			if	(lpArea->isIn(_lpTarget->m_pos.x,_lpTarget->m_pos.y))
			{
				LONGLONG	llHealHP	=	_lpTarget->getMaxHP();
				llHealHP	=	llHealHP*iHealPercentage/100;

				if	(_lpTarget->instanceHealHP((int)llHealHP))
				{
					_lpTarget->syncPlayerBasicStatus(TRUE);
					_lpTarget->addSendSimpleInfo();
				}
			}
		}
	}
	else
	for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR	*lpActor=	lpField->getActor(i);

		if	(lpActor		==	NULL)
			continue;

		cNPC	*lpNpc	=	lpActor->getNpc();

		if	(lpNpc		==	NULL)
			continue;

		if	(iCharacter	!=	dCHARACTER_ALL && lpNpc->m_wCharacter != iCharacter	)
			continue;
		if	(!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y)					)
			continue;

		LONGLONG	llHealHP	=	lpActor->getMaxHP();
		llHealHP	=	llHealHP*iHealPercentage/100;

		if	(lpActor->instanceHealHP((int)llHealHP))
		{
			lpActor->syncPlayerBasicStatus(TRUE);
			lpActor->addSendSimpleInfo();
		}
	}

	return	TRUE;
}

//
//	체력 회복


BOOL
CKarma::moveField(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	if	(!_lpTarget)
		return	FALSE;
//	" [all_area]0에 [target_player]1를 [field]2의 [number]3[0,4096],[number]4[0,4096]으로 이동시칸다."	,
	int	iArea				=	_lpReaction->m_aValue[0],i;
	int	iTargetType			=	_lpReaction->m_aValue[1];
	int	iField				=	_lpReaction->m_aValue[2];
	int	iX					=	_lpReaction->m_aValue[3]*dTILE_XS;
	int	iY					=	_lpReaction->m_aValue[4]*dTILE_YS;
	int	iGateIndex			=	0x7fff;

	if	(iField	==	906)
	{
		if	(!g_duelTrialGameManager.isTrialGameSeason())
		{
			_lpTarget->sendDuelMsg(eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_SEASON,eSM_WARNING,NULL);
			return	FALSE;
		}

		if	(!g_duelTrialGameManager.getTeamByMember(_lpTarget->m_strId))
		{
			_lpTarget->sendDuelMsg(eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_ENTRY,eSM_WARNING,g_duelTrialGameManager.m_strTrialGameName);
			return	FALSE;
		}
	}

	cFIELD		*lpField	=	_lpTarget->m_lpField;
	cAreaInfo	*lpArea		=	lpField->getArea(iArea);

	if (!lpArea)
		return	FALSE;

	if (iX	==	0)
	{
		iX			=	-1;
		iY			=	-1;
		iGateIndex	=	0xffff;
	}

	if (iTargetType	==	0)
	{
		if	(lpArea->isIn(_lpTarget->m_pos.x,_lpTarget->m_pos.y))
		{
			_lpTarget->m_posDest.x	= iX;
			_lpTarget->m_posDest.y	= iY;
			g_pMoveFieldUserManager->booking(_lpTarget,NULL,iField,iGateIndex,iX,iY);
		}
	}
	else
	if (iTargetType	==	1)
	{
		if (_lpTarget->getPartySerial()	!=	0xffff)
		{
			cParty	party;

			g_pPartyManager->getParty(&party,_lpTarget->getPartySerial());

			for (i=0;i<(int)party.getMemberCount();i++)
			{
				cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

				if	(!lpMember	||	 !lpMember->isOnSameField(_lpTarget))
					continue;

				if	(lpArea->isIn(lpMember->m_pos.x,lpMember->m_pos.y))
				{
					lpMember->m_posDest.x	= iX;
					lpMember->m_posDest.y	= iY;
					g_pMoveFieldUserManager->booking(lpMember,NULL,iField,iGateIndex,iX,iY);
				}
			}
		}
		else
		{
			if	(lpArea->isIn(_lpTarget->m_pos.x,_lpTarget->m_pos.y))
			{
				_lpTarget->m_posDest.x	= iX;
				_lpTarget->m_posDest.y	= iY;
				g_pMoveFieldUserManager->booking(_lpTarget,NULL,iField,iGateIndex,iX,iY);
			}
		}
	}

	return	TRUE;
}

//
//	
BOOL
CKarma::removeCharacter(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	if (!_lpTarget)
		return	FALSE;
//	" [all_area]0에 모든 [all_character]1를 죽인다."
	int	iArea		=	_lpReaction->m_aValue[0],i;
	int	iCharacter	=	_lpReaction->m_aValue[1];

	cFIELD		*lpField	=	_lpTarget->m_lpField;
	cAreaInfo	*lpArea		=	lpField->getArea(iArea);

	if	(!lpArea)
		return	FALSE;

	for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR	*lpActor=	lpField->getActor(i);

		if	(lpActor		==	NULL)
			continue;

		if	(lpActor->isCopyActor() && iCharacter	==	dCHARACTER_ALL )
		{
			lpActor->bookingDeath(NULL,FALSE,TRUE);
			continue;
		}

		cNPC	*lpNpc	=	lpActor->getNpc();

		if	(lpNpc		==	NULL)
			continue;

		if	(iCharacter	!=	dCHARACTER_ALL && lpNpc->m_wCharacter != iCharacter	)
			continue;
		if	(!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y)					)
			continue;

		lpNpc->addDamageBySolo(0,0xffff,lpActor->m_iHP,lpField->m_iFrameCounter);

		lpActor->bookingDeath(NULL,FALSE,TRUE);
	}

	return	TRUE;
}

BOOL
CKarma::setSecretDungeonCountdownTimer(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if	(!_lpField)
		return	FALSE;

	int	iStartEnd	=	_lpReaction->m_aValue[0];
	int	iTime		=	_lpReaction->m_aValue[1]*60*60+_lpReaction->m_aValue[2]*60+_lpReaction->m_aValue[3];
//	" 카운트 다운 타이머를 [start_end_reset]0. [spin]1[0,60]시간 [spin]2[0,60]분 [spin]3[0,60]초",
	cInstanceField	*lpIF		=	_lpField->getMasterInstanceField();

	if (!lpIF)
	{
		if (g_bIsWantPrintTriggerCondition)						
			printf(" setSecretDungeonCountdownTimer여긴 비밀 던전이 아니잖아!!\n");

		return	FALSE;
	}

	if (iStartEnd	==	0)
		lpIF->beginCountdownTimer(iTime);
	else
	if (iStartEnd	==	1)
		lpIF->stopCountdownTimer();
	else
	if (iStartEnd	==	2)
		lpIF->extensionCountdownTimer(iTime);
	else
	if (iStartEnd	==	3)
		lpIF->reductionCountdownTimer(iTime);

	lpIF->sendCountdownRemainTime();

	return	TRUE;
}

//
//스킬을 강제적으로 사용합니다.
BOOL
CKarma::useSkillByCompulsion(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget)	//스킬을 강제적으로 발동 한다.
{
	if	(!_lpOwner)
		return	FALSE;

	CSkill			*lpSKill	=	&g_aSkill[_lpReaction->m_aValue[2]];
	cAbility		Ablilety;
	int				iRookiIndex		= _lpOwner->m_lpField->m_skillManager.m_iRookie;
	CActiveSkill	*lpActiveSkill	= &_lpOwner->m_lpField->m_skillManager.m_aActiveSkill[iRookiIndex];
	int				iTarget			= _lpReaction->m_aValue[1];

	Ablilety.m_wSkill = _lpReaction->m_aValue[2];
	Ablilety.m_wLevel = _lpReaction->m_aValue[3];
	
	memcpy(&lpActiveSkill->m_ability,	&Ablilety	,sizeof(cAbility));
	lpActiveSkill->m_lpCaster		=	_lpOwner;
	lpActiveSkill->m_lpTarget		=	_lpTarget;
	lpActiveSkill->m_lpSkill		=	lpSKill;
	lpActiveSkill->m_wTeam			=	_lpOwner->m_wTeam;
	lpActiveSkill->m_pos.x			=	_lpOwner->m_pos.x;
	lpActiveSkill->m_pos.y			=	_lpOwner->m_pos.y;
	lpActiveSkill->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpActiveSkill->m_posTarget.y	=	_lpTarget->m_pos.y;
	lpActiveSkill->m_wRange			=	1000;

	int iResult =	0;
	int iStatus	=	_lpReaction->m_aValue[4];	
	int iTime	=	_lpReaction->m_aValue[5] * 100;
	cACTOR tempActor;
	
	memcpy(&tempActor , _lpOwner , sizeof(cACTOR));
	
	tempActor.m_strength.m_sFixValue		= iStatus;
	tempActor.m_agility.m_sFixValue			= iStatus;
	tempActor.m_constitution.m_sFixValue	= iStatus;
	tempActor.m_intelligence.m_sFixValue	= iStatus;
	tempActor.m_wisdom.m_sFixValue			= iStatus;
	tempActor.m_charisma.m_sFixValue		= iStatus;
	tempActor.m_luck.m_sFixValue			= iStatus;


	switch(iTarget)
	{
	case 0 :
		{	// 타겟
			_lpTarget->applySkillExtraEffects(&tempActor,&Ablilety,0,_lpTarget->m_pos.x,_lpTarget->m_pos.y , FALSE ,TRUE,iTime);
			break;
		}
	case 1 :// 모든 파티원
		{
			cParty*	lpParty	=	_lpTarget->getParty();
			if(!lpParty)
				break;
			for(int i=0;i<dPARTY_MAXPLAYER;++i)
			{
				cACTOR*	lpMember	=	lpParty->getActorByMemberIndex(i);
				if(!lpMember)
					continue;
				
				lpMember->applySkillExtraEffects(&tempActor,&Ablilety,0,_lpTarget->m_pos.x,_lpTarget->m_pos.y , FALSE ,TRUE,iTime);
				
			}
			break;
		}
	case 2 :// 파티리더
		{
			cParty*	lpParty	=	_lpTarget->getParty();
			if(!lpParty)
				break;
			for(int i=0;i<dPARTY_MAXPLAYER;++i)
			{
				cACTOR*	lpMember	=	lpParty->getActorByMemberIndex(i);
				if(!lpMember)
					continue;

				if(lpParty->isLeader(lpMember->m_strName))
				{
					lpMember->applySkillExtraEffects(&tempActor,&Ablilety,0,_lpTarget->m_pos.x,_lpTarget->m_pos.y , FALSE ,TRUE,iTime);
					break;
				}
			}
			break;
		}
	case 3 :
		{	// 파티원중 아무나

			cParty*	lpParty	=	_lpTarget->getParty();
			if(!lpParty)
				break;
			int iTargetIndex	=	random(lpParty->getMemberCount());
			
			cACTOR*	lpMember	=	lpParty->getActorByMemberIndex(iTargetIndex);

			if(!lpMember)
				break;
			
			lpMember->applySkillExtraEffects(&tempActor,&Ablilety,0,_lpTarget->m_pos.x,_lpTarget->m_pos.y , FALSE ,TRUE,iTime);
			break;

		}
	case 4 :
		{	// 펫
			
			for(int i=0;i<dOWN_PET_COUNT;++i)
			{
				cACTOR*	lpPet	=	_lpTarget->getPetActor(i);
				
				if(lpPet)
					lpPet->applySkillExtraEffects(&tempActor,&Ablilety,0,_lpTarget->m_pos.x,_lpTarget->m_pos.y , FALSE ,TRUE,iTime);
				
			}
			
			break;
		}

	}
	

	/*
	!lpActiveSkill->fire(&iResult);
	
	lpActiveSkill->m_wSerial		=	iRookiIndex;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.
	
	for (int i=iRookiIndex + 1;i<dMAX_ACTIVE_SKILL;i++)	//	다음 스킬 메니져 인덱스 설정
		if (_lpOwner->m_lpField->m_skillManager.m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			_lpOwner->m_lpField->m_skillManager.m_iRookie	=	i;
			break;
		}
		
		_lpOwner->m_lpField->m_skillManager.m_iActiveSkillCount++;	//	카운터 증가
		
		if (_lpOwner->m_lpField->m_skillManager.m_iActiveSkillCount	>	_lpOwner->m_lpField->m_skillManager.m_iMaxSkillCount)
			_lpOwner->m_lpField->m_skillManager.m_iMaxSkillCount	=	_lpOwner->m_lpField->m_skillManager.m_iActiveSkillCount;
	*/
	return	TRUE;
	
}

BOOL
CKarma::openInterface(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	int iInterface			= _lpReaction->m_aValue[0];
	BOOL bIsClose			= _lpReaction->m_aValue[1];
	BOOL bIsCanNotControl	= _lpReaction->m_aValue[2];
	_lpAsker->sendEtcWork(eEW_OPEN_INTERFACE,iInterface,bIsClose,bIsCanNotControl );

	return TRUE;
}

BOOL
CKarma::openFastPortalWindow(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	_lpAsker->sendEtcWork(eEW_OPEN_FAST_PORTAL_WINDOW);

	return TRUE;
}

BOOL
CKarma::offDrawingImageInScreen(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	int iIndex			= _lpReaction->m_aValue[0];
	int iX				= _lpReaction->m_aValue[1];
	int iY				= _lpReaction->m_aValue[2];

	_lpAsker->sendEtcWork(eEW_OFF_DRAWING_IMAGE,iIndex ,iX , iY );

	return TRUE;
}


BOOL
CKarma::drawImageInScreen(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	int iIndex			= _lpReaction->m_aValue[0];
	int iX				= _lpReaction->m_aValue[1];
	int iY				= _lpReaction->m_aValue[2];
	int iTime			= _lpReaction->m_aValue[3];
	int iDrawingMethod	= _lpReaction->m_aValue[4];
	int iDistance		= _lpReaction->m_aValue[5];

	_lpAsker->sendEtcWork(eEW_DRAW_IMAGE,iIndex ,iX , iY , iTime,iDrawingMethod,iDistance);

	return TRUE;
}
BOOL
CKarma::useHaveSkill(CKarmaItem *_lpReaction,cACTOR *_lpOwner)			//09.08.14
{
	if(!_lpReaction || !_lpOwner)
		return FALSE;

	cAbility	*lpAbility = NULL;

	lpAbility	=	_lpOwner->getMonsterAbility(_lpReaction->m_aValue[0]);

	if(!lpAbility)
		return FALSE;

	return useSkill(lpAbility->m_wSkill, lpAbility->m_wLevel, _lpOwner);

}

BOOL
CKarma::useMonsterSkill(CKarmaItem *_lpReaction,cACTOR *_lpOwner)			//09.08.14
{
	if(!_lpReaction || !_lpOwner)
		return FALSE;


	int iSkill = _lpReaction->m_aValue[0];
	int iLevel = _lpReaction->m_aValue[1];


	return useSkill(iSkill, iLevel, _lpOwner);

}

BOOL
CKarma::useSkill(int _iSkill, int _iLevel, cACTOR *_lpOwner)			//09.08.14
{
	_lpOwner->m_bEngageReactionAbility = FALSE;


	if	(!_lpOwner || !_lpOwner->m_lpTarget)
		return	FALSE;


	_lpOwner->m_bEngageReactionAbility  = TRUE;

	_lpOwner->operateCommandSkill(_iSkill, _iLevel,_lpOwner->m_lpTarget,_lpOwner->m_lpTarget->m_pos.x,_lpOwner->m_lpTarget->m_pos.y);

	return TRUE;
}


BOOL
CKarma::changeJob(CKarmaItem *_lpReaction,cACTOR *_lpOwner)
{
	if (!_lpOwner)
		return	FALSE;
	int			iCharacter		=	_lpReaction->m_aValue[0];
	cFIELD		*lpField		=	_lpOwner->m_lpField;

	cCHARACTER	*lpCharacter	=	&lpField->m_aCharacter[iCharacter];

	if	(!lpCharacter || lpCharacter->m_wBaseJob	==	0xffff)
	{
		if	(g_bIsWantPrintTriggerCondition)						
			printf(" 직업 전환 실패!! 캐릭터가 이상해!\n");

		return	FALSE;		
	}

	_lpOwner->setDataByCharacter(lpCharacter,_lpOwner->m_wActorKind,lpCharacter->getGenerateLevel());

	_lpOwner->addSendSimpleInfo();

	return	TRUE;
}

BOOL
CKarma::setGuide(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cACTOR *_lpOwner)
{
	if (!_lpAsker || !_lpOwner)
		return	FALSE;
	
	int	iTargetLeader	=	_lpReaction->m_aValue[0];
	int	iSetCancel		=	_lpReaction->m_aValue[1];

	if(iSetCancel	==	1)
	{
		_lpOwner->setGuide(NULL);

		return	TRUE;
	}

	cACTOR	*lpGuide	=	NULL;

	lpGuide				=	_lpAsker;

	if (iTargetLeader	==	1)
	{
		cParty	*lpParty=	_lpAsker->getParty();

		if (lpParty)
		{
			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(lpLeader && lpLeader->isOnSameField(_lpOwner))
				lpGuide		=	lpLeader;
		}
	}

	if	(!lpGuide)
		return	FALSE;

	_lpOwner->setGuide(lpGuide);

	return	TRUE;
}

BOOL
CKarma::playPattern(CKarmaItem *_lpReaction,cACTOR *_lpOwner)
{
	if	(!_lpOwner)
		return	FALSE;

	if	(_lpOwner->m_wProcessPatternLevel	!=	0xffff)
		return	TRUE;

	return	_lpOwner->operatePattern(_lpReaction->m_aValue[0]);
}

BOOL
CKarma::playBossEvent(CKarmaItem *_lpReaction,cFIELD	*_lpField)
{
	_lpField->addSendEtcWork(0xffff,eEW_BOSS_BATTLE_EVENT,_lpReaction->m_aValue[0],_lpReaction->m_aValue[1]);

	return	TRUE;
}

BOOL
CKarma::giveScreenEvent(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cFIELD	*_lpField)
{
	if (!_lpAsker)
		return	FALSE;
	
	//	" [quest_target]0에게 [state]1을 [spin]2[1,10000]초 부여 한다.",
	int		iTarget	=	_lpReaction->m_aValue[0];
	int		iState	=	_lpReaction->m_aValue[1];
	int		iSecond	=	_lpReaction->m_aValue[2];
	
	switch(iTarget)
	{
	case 0:
		_lpField->addSendEtcWork(_lpAsker->m_wSerialInField,eEW_SCREEN_START_EVENT,_lpReaction->m_aValue[1],_lpReaction->m_aValue[2]);
		break;
	case 1:
		{
			cParty *lpParty = _lpAsker->getParty();
			
			if(!lpParty)
			{
				_lpField->addSendEtcWork(_lpAsker->m_wSerialInField,eEW_SCREEN_START_EVENT,_lpReaction->m_aValue[1],_lpReaction->m_aValue[2]);
			}
			else
			{
				for (int i = 0 ; i < lpParty->getMemberCount() ; i++)
				{
					cACTOR* lpMember = lpParty->getActorByMemberIndex(i);
					
					if(!lpMember)
						continue;
					
					_lpField->addSendEtcWork(lpMember->m_wSerialInField,eEW_SCREEN_START_EVENT,_lpReaction->m_aValue[1],_lpReaction->m_aValue[2]);
				}
			}
			break;
			
		}
	case 2:
		{
			cParty *lpParty = _lpAsker->getParty();
			
			if(!lpParty)
				break;
			
			cACTOR* lpLeader = lpParty->getActorByMemberIndex(0);
			
			if(!lpLeader)
				break;
			
			_lpField->addSendEtcWork(lpLeader->m_wSerialInField,eEW_SCREEN_START_EVENT,_lpReaction->m_aValue[1],_lpReaction->m_aValue[2]);
			
			break;
		}
	case 3:
		{
			cParty *lpParty = _lpAsker->getParty();
			
			if(!lpParty)
				break;
			
			int	iMember		=	random((int)lpParty->getMemberCount());
			
			cACTOR* lpMember = lpParty->getActorByMemberIndex(iMember);
			
			if(!lpMember)
				break;
			
			_lpField->addSendEtcWork(lpMember->m_wSerialInField,eEW_SCREEN_START_EVENT,_lpReaction->m_aValue[1],_lpReaction->m_aValue[2]);
			
			break;
			
		}
	}
	
	return	TRUE;

	
	
	return	TRUE;
}

BOOL
CKarma::fixFieldResistance(CKarmaItem *_lpReaction,cFIELD	*_lpField)
{
	//" [resistance]0 을 [spin]1[0,100]으로 변경",
	switch(_lpReaction->m_aValue[0])
	{
	case 0://불 저항
		_lpField->m_sCorrectMonsterFireResistance	=	_lpReaction->m_aValue[1];
		break;
	case 1://물 저항
		_lpField->m_sCorrectMonsterWaterResistance	=	_lpReaction->m_aValue[1];
		break;
	case 2://바람 저항
		_lpField->m_sCorrectMonsterWindResistance	=	_lpReaction->m_aValue[1];
		break;
	case 3://땅 저항
		_lpField->m_sCorrectMonsterEarthResistance	=	_lpReaction->m_aValue[1];
		break;
	case 4://빛 저항
		_lpField->m_sCorrectMonsterLightResistance	=	_lpReaction->m_aValue[1];
		break;
	case 5://어둠 저항
		_lpField->m_sCorrectMonsterDarkResistance	=	_lpReaction->m_aValue[1];
		break;

	case 6://장님 저항
		_lpField->m_sCorrectMonsterBlindResistance	=	_lpReaction->m_aValue[1];
		break;
	case 7://중독 저항
		_lpField->m_sCorrectMonsterPoisonResistance	=	_lpReaction->m_aValue[1];
		break;
	case 8://잠 저항
		_lpField->m_sCorrectMonsterSleepResistance	=	_lpReaction->m_aValue[1];
		break;
	case 9://콜드 저항
		_lpField->m_sCorrectMonsterColdResistance	=	_lpReaction->m_aValue[1];
		break;
	case 10://프리즈 저항
		_lpField->m_sCorrectMonsterFreezeResistance	=	_lpReaction->m_aValue[1];
		break;
	case 11://경직 저항
		_lpField->m_sCorrectMonsterStunResistance	=	_lpReaction->m_aValue[1];
		break;
	case 12://석화 저항
		_lpField->m_sCorrectMonsterStoneResistance	=	_lpReaction->m_aValue[1];
		break;
	case 13://혼란 저항
		_lpField->m_sCorrectMonsterConfuseResistance	=	_lpReaction->m_aValue[1];
		break;
	case 14://챠밍 저항
		_lpField->m_sCorrectMonsterCharmingResistance	=	_lpReaction->m_aValue[1];
		break;
	case 15://크리티컬 저항
		_lpField->m_sCorrectMonsterCriticalResistance	=	_lpReaction->m_aValue[1];
		break;
	case 16://결정타 저항
		_lpField->m_sCorrectMonsterCrushResistance	=	_lpReaction->m_aValue[1];
		break;
	}
	
	return	TRUE;
}

BOOL
CKarma::playScreenEvent(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if (!_lpAsker)
		return	FALSE;

	int	iTargetType	=	_lpReaction->m_aValue[4];			//타켓의 대상

	switch (iTargetType)
	{
	case 0:					//요청한 사람만...
		_lpAsker->m_lpField->sendSimpleInfoToInAreaActor(_lpReaction->m_aValue[5] , _lpAsker);
		_lpAsker->sendScreenEvent(_lpReaction->m_aValue[0] , _lpReaction->m_aValue[1] , _lpReaction->m_aValue[2] , _lpReaction->m_aValue[3]);
		break;
	case 1:					//모든 파티원...
		if	(_lpAsker->getPartySerial()	!=	0xffff)
		{
			cParty	party;
			
			g_pPartyManager->getParty(&party,_lpAsker->getPartySerial());
			
			for (int i=0;i<(int)party.getMemberCount();i++)
			{
				cACTOR	*lpMember	=	party.getActorByMemberIndex(i);
				
				if	(!lpMember	||	 !lpMember->isOnSameField(_lpAsker))
					continue;
				
				lpMember->m_lpField->sendSimpleInfoToInAreaActor(_lpReaction->m_aValue[5] , lpMember);
				lpMember->sendScreenEvent(_lpReaction->m_aValue[0] , _lpReaction->m_aValue[1] , _lpReaction->m_aValue[2] , _lpReaction->m_aValue[3]);
			}
		}
		break;
	case 2:
		break;
	case 3:
		break;
	}

	return	TRUE;
}

BOOL
CKarma::setTraceGuide(CKarmaItem *_lpReaction,cACTOR *_lpOwner)
{
	if	(!_lpOwner)
		return	FALSE;

	int	iSetCancel		=	_lpReaction->m_aValue[0];

	if (!_lpOwner->getGuide())
	{
		if (g_bIsWantPrintTriggerCondition)						
			printf(" 길잡이 따라가기 실패!! 길잡이가 없잖아!!\n");

		return	FALSE;		
	}

	if (iSetCancel	==	0)
		_lpOwner->setTraceGuide();
	else
		_lpOwner->cancelTraceGuide();

	return	TRUE;
}

BOOL
CKarma::regenMonster(cACTOR *_lpOwner,CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if	(!_lpField)
		return	FALSE;

	int	iArea		=	_lpReaction->m_aValue[0];
	int	iCharacter	=	_lpReaction->m_aValue[1];

	if	(!_lpField)
	{
		if	(g_bIsWantPrintTriggerCondition)						
			printf(" 몬스터 리젠 실패!! 필드가 없다!!\n");

		return	FALSE;
	}

	_lpField->regenMonsterInArea(iArea,iCharacter);

	return	TRUE;
}

BOOL
CKarma::changeTargetByRandom(CKarmaItem *_lpReaction,cACTOR *_lpOwner)
{
	if (!_lpOwner)
		return	FALSE;

	int		iTime		=	_lpReaction->m_aValue[0];
	cACTOR	*lpTarget	=	_lpOwner->m_lpField->findRandomTargetInInsightEnemy(_lpOwner);

	if (lpTarget)
	{
		_lpOwner->revenge(lpTarget);
		_lpOwner->setUpkeepTargetTime(iTime);
	}

	return	TRUE;
}

BOOL
CKarma::removeTargeting(CKarmaItem *_lpReaction,cACTOR *_lpOwner)
{
	if (!_lpOwner)
		return	FALSE;

	_lpOwner->removeTargeting();
	_lpOwner->setUpkeepTargetTime(0);
	
	return	TRUE;
}



BOOL
CKarma::releaseWrongStatus(CKarmaItem *_lpReaction,cFIELD	*_lpField)
{
	if (!_lpField)
		return	FALSE;

//	" [area]0 안에 [all_character]1의 이상 상태를 모두 해제한다.",
	int	iArea		=	_lpReaction->m_aValue[0],i;
	int	iCharacter	=	_lpReaction->m_aValue[1];

	cAreaInfo	*lpArea		=	_lpField->getArea(iArea);

	if (!lpArea)
		return	FALSE;

	for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR	*lpActor=	_lpField->getActor(i);

		if (lpActor		==	NULL)
			continue;

		cNPC	*lpNpc	=	lpActor->getNpc();

		if (lpNpc		==	NULL)
			continue;
		if (iCharacter	!=	dCHARACTER_ALL && lpNpc->m_wCharacter != iCharacter	)
			continue;
		if (!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y)					)
			continue;

		lpActor->cureWrongStatus();
	}

	return	TRUE;
}

BOOL
CKarma::setInvincibleStatus(CKarmaItem *_lpReaction,cFIELD	*_lpField)
{
	if (!_lpField)
		return	FALSE;

	int	iArea		=	_lpReaction->m_aValue[0],i;
	int	iCharacter	=	_lpReaction->m_aValue[1];
	int	iOnOff		=	_lpReaction->m_aValue[2];
//	" [area]0 안에 [all_character]1를 [invincible_onoff]2",

	cAreaInfo	*lpArea		=	_lpField->getArea(iArea);

	if (!lpArea)
		return	FALSE;

	for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR	*lpActor=	_lpField->getActor(i);

		if (lpActor		==	NULL)
			continue;

		cNPC	*lpNpc	=	lpActor->getNpc();

		if (lpNpc		==	NULL)
			continue;
		if (iCharacter	!=	dCHARACTER_ALL && lpNpc->m_wCharacter != iCharacter	)
			continue;
		if (!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y)					)
			continue;

		if (iOnOff	==	0)
			lpActor->m_wInvincibleTime	=	0xffff;
		else
			lpActor->m_wInvincibleTime	=	0;
	}

	return	TRUE;
}

BOOL
CKarma::controlWorldValue(CKarmaItem *_lpReaction)
{
	SERVERStoWORLD_WORLD_VALUE_INFO	packet;
	
	packet.base.set(sizeof(packet),dSERVERStoWORLD_UPDATE_WORLD_VALUE_INFO);

	packet.wWorldValueSerial = _lpReaction->m_aValue[0];
	packet.iValue			 = _lpReaction->m_aValue[1];
	packet.iAddRemoveSet	 = _lpReaction->m_aValue[2];
	
	SEND_WOLRD_PACKET(&packet);

	return	TRUE;
}


BOOL
CKarma::controlFieldValue(CKarmaItem *_lpReaction,cFIELD	*_lpField)
{
	if	(!_lpField)
		return	FALSE;

	int	iValueIndex		=	_lpReaction->m_aValue[0];
	int	iValue			=	_lpReaction->m_aValue[1];
	int	iAddRemoveSet	=	_lpReaction->m_aValue[2];
//	" [dungeon_value]0을 [spin]1[0,30000] [add_remove_set]2",

	if	(iAddRemoveSet	==	0)
		_lpField->m_aiFieldValue[iValueIndex]	+=	iValue;
	if(iAddRemoveSet	==	1)
		_lpField->m_aiFieldValue[iValueIndex]	-=	iValue;
	if	(iAddRemoveSet	==	2)
		_lpField->m_aiFieldValue[iValueIndex]	=	iValue;

	_lpField->m_aiFieldValue[iValueIndex]	=	max(_lpField->m_aiFieldValue[iValueIndex],0);

	return	TRUE;
}

BOOL
CKarma::controlDungeonValue(CKarmaItem *_lpReaction,cFIELD	*_lpField)
{
	if (!_lpField)
		return	FALSE;

	int	iValueIndex		=	_lpReaction->m_aValue[0];
	int	iValue			=	_lpReaction->m_aValue[1];
	int	iAddRemoveSet	=	_lpReaction->m_aValue[2];
//	" [dungeon_value]0을 [spin]1[0,30000] [add_remove_set]2",

	cInstanceField	*lpIF	=	_lpField->getMasterInstanceField();

	if (!lpIF)
	{
		if (g_bIsWantPrintTriggerCondition)						
			printf(" 여긴 비밀 던전이 아니잖아!!\n");

		return	FALSE;
	}

	if(iAddRemoveSet	==	0)
		lpIF->addDungeonValue(iValueIndex,iValue);
	if(iAddRemoveSet	==	1)
		lpIF->addDungeonValue(iValueIndex,-iValue);
	if(iAddRemoveSet	==	2)
		lpIF->setDungeonValue(iValueIndex,iValue);

	return	TRUE;
}

BOOL
CKarma::setArcaStatus(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if (!_lpField)
		return	FALSE;

	int	iArea	=	_lpReaction->m_aValue[0];
	int	iWork	=	_lpReaction->m_aValue[2];

	_lpField->operateArcaWorkInAreaByKarma(iArea,iWork);

	return	TRUE;
}

BOOL
CKarma::setDoorStatus(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if (!_lpField)
		return	FALSE;

	int	iArea	=	_lpReaction->m_aValue[0];
	int	iWork	=	_lpReaction->m_aValue[2];

	_lpField->operateDoorWorkInAreaByKarma(iArea,iWork);

	return	TRUE;
}

BOOL
CKarma::setTrapStatus(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if (!_lpField)
		return	FALSE;

	int	iArea	=	_lpReaction->m_aValue[0];
	int	iWork	=	_lpReaction->m_aValue[2];

	_lpField->operateTrapWorkInAreaByKarma(iArea,iWork);
	
	return	TRUE;
}

BOOL
CKarma::setDoorEnableDisable(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if (!_lpField)
		return	FALSE;

	int	iArea			=	_lpReaction->m_aValue[0];
	int	iEnableDisable	=	_lpReaction->m_aValue[1];
	int	iWork;

	if	(iEnableDisable	==	0)
		iWork	=	eADTWK_ENABLE_DOOR;
	else
		iWork	=	eADTWK_DISABLE_DOOR;

	_lpField->operateDoorWorkInAreaByKarma(iArea,iWork);

	return	TRUE;
}

BOOL
CKarma::setArcaDoorTrapStatus(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if (!_lpField)
		return	FALSE;
//	" [area]0의 [arca_door_trap]1을 [arca_door_trap_status]2",
	int	iArcaDoorTrap	=	_lpReaction->m_aValue[1];

	switch(iArcaDoorTrap)
	{
		case	0	:
			setArcaStatus(_lpReaction,_lpField);
			break;
		case	1	:
			setDoorStatus(_lpReaction,_lpField);
			break;
		case	2	:
			setTrapStatus(_lpReaction,_lpField);
			break;
	}

	return	TRUE;
}

BOOL
CKarma::beginEvent(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if (!_lpField)
		return	FALSE;
//	" [event_begin_end]0 [battle_status]1",
	int	iBeginEnd				=	_lpReaction->m_aValue[0];
	int	iBattleEnableDisable	=	_lpReaction->m_aValue[1];

	cInstanceField	*lpIF		=	_lpField->getMasterInstanceField();

	if (!lpIF)
	{
		if (g_bIsWantPrintTriggerCondition)						
			printf(" 이벤트 시작 실패!! 비밀던전이 아니잖하아아아!!!\n");

		return	FALSE;
	}

	lpIF->setEvent(iBeginEnd,iBattleEnableDisable);

	return	TRUE;
}

BOOL
CKarma::occurSkillEffect(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if (!_lpField)
		return	FALSE;
//	" [spin]0[1,30000],[spin]1[1,30000]에 [skill]2의 [skill_effect_type]3 효과를 발동시킨다.",
	int	iX		=	_lpReaction->m_aValue[0]*dTILE_XS;
	int	iY		=	_lpReaction->m_aValue[1]*dTILE_YS;
	int	iSkill	=	_lpReaction->m_aValue[2];
	int	iEffect	=	_lpReaction->m_aValue[3];

	_lpField->addSendOccurEffectBySkill(iX,iY,iSkill,iEffect);

	return	TRUE;
}

BOOL
CKarma::setGroundFocus(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if (!_lpField)
		return	FALSE;
//	" 포커스를 [spin]0[1,30000],[spin]1[1,30000]에 맞춘다. [set_reset]2",
	int	iX				=	_lpReaction->m_aValue[0];
	int	iY				=	_lpReaction->m_aValue[1];
	int	iSetResetScroll	=	_lpReaction->m_aValue[2];
	int	iTime			=	_lpReaction->m_aValue[3];

	iX					=	iX*dTILE_XS+dTILE_XS/2;
	iY					=	iY*dTILE_YS+dTILE_YS/2;

	if	(iSetResetScroll==	1)	//	리셋이다.
		iX				=	0;

	cInstanceField	*lpIF		=	_lpField->getMasterInstanceField();

	if	(!lpIF	||	iSetResetScroll	==	2)
	{
		_lpField->addSendEtcWork(0xffff,eEW_SET_FOCUS,iX,iY,iTime);

		return	TRUE;
	}

	if	(iSetResetScroll	==	1)
	{
		lpIF->releaseEventFocus();

		return	TRUE;
	}

	int	iFloor	=	_lpField->m_iUniqueSerial%dINSTANCE_FIELD_FLAG;

	lpIF->setGroundFocus(iX,iY,iFloor);

	return	TRUE;
}

BOOL
CKarma::setCharacterFocus(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if (!_lpField)
		return	FALSE;
//	" 포커스를 [character]0에 맞춘다. [set_reset]1",
	int	iCharacter	=	_lpReaction->m_aValue[0];
	int	iSetReset	=	_lpReaction->m_aValue[1];

	cInstanceField	*lpIF		=	_lpField->getMasterInstanceField();

	if (!lpIF)
	{
		if (g_bIsWantPrintTriggerCondition)						
			printf(" 캐릭터 포커스 설정 실패!! 비밀던전이 아니잖하아아아!!!\n");

		return	FALSE;
	}

	if (iSetReset	==	1)
	{
		lpIF->releaseEventFocus();

		return	TRUE;
	}

	int	iFloor	=	_lpField->m_iUniqueSerial%dINSTANCE_FIELD_FLAG;

	for (int i=0;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR	*lpActor=	_lpField->getActor(i);

		if (lpActor		==	NULL)
			continue;

		cNPC	*lpNpc	=	lpActor->getNpc();

		if (lpNpc		==	NULL)
			continue;

		if (lpNpc->m_wCharacter != iCharacter	)
			continue;

		lpIF->setCharacterFocus(i,iFloor);

		return	TRUE;
	}

	if (g_bIsWantPrintTriggerCondition)						
		printf(" 포커스 설정 실패!! 캐릭터 못찾겠다아아아!!!\n");

	return	FALSE;
}

BOOL
CKarma::shakeScreen(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if	(!_lpField)
		return	FALSE;
//	" [spin]0[1,30000],[spin]1[1,30000]에 사정 거리 [spin]2[1,30000] 흔들림 [spin]3[1,10]의 지진 발생. 지속시간 [spin]4[1,30000] MS"
	int	iX		=	_lpReaction->m_aValue[0]*dTILE_XS+dTILE_XS/2;
	int	iY		=	_lpReaction->m_aValue[1]*dTILE_YS+dTILE_YS/2;
	int	iRange	=	_lpReaction->m_aValue[2];
	int	iShake	=	_lpReaction->m_aValue[3];
	int	iTime	=	_lpReaction->m_aValue[4];

	_lpField->addSendEtcWork(0xffff,eEW_SHAKE_SCREEN,iX,iY,iRange,iShake,iTime);

	return	TRUE;
}

BOOL
CKarma::playSound(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if	(!_lpField)
		return	FALSE;
//	" [all_area]0 안에 플레이어에게 [string]1 파일을 플레이 한다.",
	int	iArea	=	_lpReaction->m_aValue[0];

	SG_PLAY_SOUND	packet;

	packet.base.set(sizeof(packet),dSG_PLAY_SOUND);
	strncpy(packet.strSoundFile,_lpReaction->m_strString,sizeof(packet.strSoundFile)-2);

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strSoundFile) + strlen(packet.strSoundFile)+1;

	_lpField->sendPacketToInAreaActor((ALL_MSG *)&packet,iArea);

	return	TRUE;
}

BOOL
CKarma::stressMinimap(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if (!_lpField)
		return	FALSE;
//	" 미니맵의 [spin]0[1,30000],[spin]1[1,30000]에 강조 효과를 출력한다.",
	int	iX			=	_lpReaction->m_aValue[0];
	int	iY			=	_lpReaction->m_aValue[1];
	int	iSetReset	=	_lpReaction->m_aValue[2];

	iX				=	iX*dTILE_XS+dTILE_XS/2;
	iY				=	iY*dTILE_YS+dTILE_YS/2;

	cInstanceField	*lpIF		=	_lpField->getMasterInstanceField();

	if (!lpIF)
	{
		if (g_bIsWantPrintTriggerCondition)						
			printf(" 미니맵 강조 실패!! 비밀던전이 아니잖하아아아!!!\n");

		return	FALSE;
	}

	if (iSetReset	==	1)
	{
		lpIF->releaseStressMinimap();

		return	TRUE;
	}

	int	iFloor	=	_lpField->m_iUniqueSerial%dINSTANCE_FIELD_FLAG;

	lpIF->stressMinimap(iX,iY,iFloor);

	return	TRUE;
}

BOOL
CKarma::npcAction(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cFIELD *_lpField)
{
	if (!_lpField || !_lpAsker)
		return	FALSE;
//	" [area]0 안에 [all_character]1들이 [action]2을 한다.",
	SG_SET_ANM	packet;

	packet.base.set(sizeof(packet),dSG_SET_ANM);
	packet.wCount	=	0;

	int	iArea		=	_lpReaction->m_aValue[0],i;
	int	iCharacter	=	_lpReaction->m_aValue[1];
	int	iDirect		=	_lpReaction->m_aValue[2];
	int	iAction		=	_lpReaction->m_aValue[3];
	int	iReplayCount	=	_lpReaction->m_aValue[4];			//몇번 반복하냐?

	cAreaInfo	*lpArea		=	_lpField->getArea(iArea);

	if (!lpArea)
		return	FALSE;

	for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cSetAnmPacketActorInfo	*lpData	=	&packet.aActorList[packet.wCount];

		cACTOR	*lpActor	=	_lpField->getActor(i);

		if (lpActor			==	NULL)
			continue;

		lpData->set(i,iAction,15);

		cNPC	*lpNpc	=	lpActor->getNpc();

		if (lpNpc		==	NULL)
			continue;
		if (iCharacter	!=	dCHARACTER_ALL && lpNpc->m_wCharacter != iCharacter	)
			continue;
		if (!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y)					)
			continue;

		switch(iDirect)
		{
			case	eCDK_SPEECH_TARGET	:
			{
				if (!_lpAsker)
				{
					if (g_bIsWantPrintTriggerCondition)						
						printf(" 타겟이 없는데 타겟쪽으로 방향 전환 시도 [%s]\n",lpActor->m_strName);
					break;
				}

				lpActor->m_wDirect	=	lpActor->getDirect(_lpAsker->m_pos.x,_lpAsker->m_pos.y,TRUE);
				lpData->m_bf4Direct	=	lpActor->m_wDirect;
				lpData->m_bf15ReplayCount = iReplayCount;
				break;
			}
			case	eCDK_GUIDE			:
			{
				cACTOR	*lpGuide	=	lpActor->getGuide();

				if (!lpGuide)
				{
					if (g_bIsWantPrintTriggerCondition)						
						printf(" 길잡이 없는데 길잡이쪽으로 방향 전환 시도 [%s]\n",lpActor->m_strName);
					break;
				}
				lpActor->m_wDirect	=	lpActor->getDirect(lpGuide->m_pos.x,lpGuide->m_pos.y,TRUE);
				lpData->m_bf4Direct	=	lpActor->m_wDirect;
				lpData->m_bf15ReplayCount = iReplayCount;
				break;
			}
			case	eCDK_PARTY_LEADER	:
			{
				if (!_lpAsker)
				{
					if (g_bIsWantPrintTriggerCondition)						
						printf(" 파티가 없는데 파티 리더쪽으로 방향 전환 시도 [%s]\n",lpActor->m_strName);
					break;
				}

				cParty	*lpParty	=	_lpAsker->getParty();

				if (!lpParty)
				{
					if (g_bIsWantPrintTriggerCondition)						
						printf(" 파티가 없는데 파티 리더쪽으로 방향 전환 시도 [%s]\n",lpActor->m_strName);
					break;
				}

				cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

				if (!lpLeader || lpLeader->isOnSameField(_lpField->m_iUniqueSerial,_lpField->m_wGateField))
				{
					if (g_bIsWantPrintTriggerCondition)						
						printf(" 리더가 같은 필드에 없는데 파티 리더이쪽으로 방향 전환 시도 [%s]\n",lpActor->m_strName);
					break;
				}
				lpActor->m_wDirect	=	lpActor->getDirect(lpLeader->m_pos.x,lpLeader->m_pos.y,TRUE);
				lpData->m_bf4Direct	=	lpActor->m_wDirect;
				lpData->m_bf15ReplayCount = iReplayCount;
				break;
			}

			default		:
				lpActor->m_wDirect	=	iDirect	-	eCDK_N;
				lpData->m_bf4Direct	=	lpActor->m_wDirect;
				lpData->m_bf15ReplayCount = iReplayCount;
				break;
		}

		if (lpData->m_bf4Direct	==	15	&&	lpData->m_bf5Anm	==	0)
			continue;

		packet.wCount++;

		if (packet.wCount	>=	dMAX_SET_ANM_PACKET_ACTOR_COUNT)
		{
			_lpField->addSendPacket((ALL_MSG *)&packet,0xffff);
			packet.wCount	=	0;
		}
	}

	if (packet.wCount)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aActorList)+packet.wCount*sizeof(cSetAnmPacketActorInfo);

		_lpField->addSendPacket((ALL_MSG *)&packet,0xffff);
	}

	return	TRUE;
}

BOOL
CKarma::extendSecretDungeonUpkeepTime(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if	(!_lpField)
		return	FALSE;

	int	iExtendMinute			=	_lpReaction->m_aValue[0];

	cInstanceField	*lpIF		=	_lpField->getMasterInstanceField();

	if	(!lpIF)
	{
		if	(g_bIsWantPrintTriggerCondition)						
			printf(" completeSecretDungeon여긴 비밀 던전이 아니잖아!!\n");

		return	FALSE;
	}

	lpIF->extendUpkeepTime(iExtendMinute);

	return	TRUE;
}

BOOL
CKarma::completeSecretDungeon(CKarmaItem *_lpReaction,cFIELD *_lpField)
{
	if	(!_lpField)
		return	FALSE;

	cInstanceField	*lpIF		=	_lpField->getMasterInstanceField();

	if	(!lpIF)
	{
		if (g_bIsWantPrintTriggerCondition)						
			printf(" completeSecretDungeon여긴 비밀 던전이 아니잖아!!\n");

		return	FALSE;
	}

	int	iMethod			=	_lpReaction->m_aValue[0];
	int	iRemainTime		=	_lpReaction->m_aValue[1];
	int iIsGuildDungen	=	_lpReaction->m_aValue[2];

	lpIF->complete(iMethod,iRemainTime,iIsGuildDungen);

	return	TRUE;
}


BOOL
CKarma::setChapter(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	int	iChapter	=	_lpReaction->m_aValue[0];
	int	iHighQuest	=	_lpReaction->m_aValue[1];
	int	iSetCancel	=	_lpReaction->m_aValue[2];

	if	(iSetCancel	==	0)
		_lpAsker->setChapter(iChapter,iHighQuest);
	else
		_lpAsker->cancelChapter();

	return	TRUE;
}

BOOL
CKarma::setMainQuestValue(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	int	iValueIndex	=	_lpReaction->m_aValue[0];
	int	iValue		=	_lpReaction->m_aValue[1];
	int	iSetPlusMinus=	_lpReaction->m_aValue[2];
//	" 메인 퀘스트 수치 [main_quest_value]0을 [spin]1[0,20000] [set_plus_minus]2.",

	if	(iSetPlusMinus	==	0)
		_lpAsker->setMainQuestValue(iValueIndex,iValue);
	else
	if	(iSetPlusMinus	==	1)
		_lpAsker->addMainQuestValue(iValueIndex,iValue);
	else
	if	(iSetPlusMinus	==	2)
		_lpAsker->addMainQuestValue(iValueIndex,-iValue);

	return	TRUE;
}

BOOL
CKarma::setMainQuestSwitch(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
//	" 메인 퀘스트 스위치 [main_quest_switch]0을 [set_cancel]1",
	int	iSwitchIndex	=	_lpReaction->m_aValue[0];
	int	iSetCancel		=	_lpReaction->m_aValue[1];

	_lpAsker->setMainQuestSwitch(iSwitchIndex,1-iSetCancel);

	return	TRUE;
}

BOOL
CKarma::resetMainQuestValue(cACTOR *_lpAsker)
{
	_lpAsker->resetMainQuestValue();

	return	TRUE;
}

BOOL
CKarma::displayMainQuestEpilog(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	_lpAsker->sendEtcWork(eEW_DISPLAY_MAIN_QUEST_EPILOG,_lpReaction->m_aValue[0],_lpReaction->m_aValue[1],_lpReaction->m_aValue[2]);
	_lpAsker->m_bf1IsProcessEvent	=	TRUE;

	return	TRUE;
}

BOOL
CKarma::resetMainQuestSwitch(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	_lpAsker->resetMainQuestSwitch();

	return	TRUE;
}

BOOL
CKarma::addGGGtoTarget(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	int	iGrace			=	_lpReaction->m_aValue[0] + 1;			//은총.
	int iAddOrRemove	=	_lpReaction->m_aValue[1];			//추가 혹은 제거
	
	if(iAddOrRemove)			//제거한다.
	{
		_lpAsker->m_GGG.ResetGGG();
		_lpAsker->m_bf3GreateGodGrace = 0;
		_lpAsker->SendRemoveGGG();
		_lpAsker->buildPower();
		_lpAsker->addSendActorMagicEffectStatus();
	}
	else
	{
		_lpAsker->addGGG(iGrace);
	}

	return TRUE;
}

BOOL
CKarma::writeBookPage(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	int	iBook		=	_lpReaction->m_aValue[0];
	int	iCategory	=	_lpReaction->m_aValue[1]-1;
	int	iPage		=	_lpReaction->m_aValue[2]-1;
	int	iWriteMethod=	_lpReaction->m_aValue[3];

	if	(iWriteMethod	==	0)
		_lpAsker->writeBookPage(iBook,iCategory,iPage);
	else
		_lpAsker->eraseBookPage(iBook,iCategory,iPage);

	return	TRUE;
}

BOOL
CKarma::explosionEventBalloon(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	int		iItem		=	_lpReaction->m_aValue[0];
	int		iSlotIndex	=	_lpAsker->getItemSlotByBasicItem(iItem);

	if	(iSlotIndex		==	0xffff)
		return	FALSE;

	cItem	*lpItem		=	_lpAsker->getInventoryItem(iSlotIndex);

	if	(!lpItem)
		return	FALSE;

	if(!lpItem->isEventBalloon())
		return FALSE;

	cBasicItem* lpBasicItem = lpItem->getBasicItem();

	if(!lpBasicItem)
		return FALSE;

	int	iValue	=	lpItem->m_aOption[0];
	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_EVENT_BALLOON)
	{

		_lpAsker->m_wBE_MultiExperience	=	iValue;
		return	TRUE;
	}
	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_EVENT_BALLOON_INC_ITEM_DROP_CHANCE)
	{
		_lpAsker->m_wBE_IncreaseItemDropChance	=	iValue;
		return	TRUE;
	}


	return	TRUE;
}


BOOL
CKarma::extendItemValidateTimeToNextDay(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	int		iItem	=	_lpReaction->m_aValue[0];
	int		iSlotIndex=	_lpAsker->getItemSlotByBasicItem(iItem);

	if	(iSlotIndex	==	0xffff)
		return	FALSE;

	cItem	*lpItem	=	_lpAsker->getInventoryItem(iSlotIndex);

	if	(!lpItem)
		return	FALSE;

	lpItem->m_year	=	EncodeItemExpireYear(g_currentTime.m_wYear);
	lpItem->m_month	=	g_currentTime.m_wMonth;
	lpItem->m_day	=	g_currentTime.m_wDay;
	lpItem->m_hour	=	g_currentTime.m_wHour;
	lpItem->m_minute=	g_currentTime.m_wMinute;

	lpItem->increaseMinute(60*24);

	lpItem->m_hour	=	23;
	lpItem->m_minute=	50;

	_lpAsker->sendUpdateItemData(iSlotIndex,TRUE);

	return	TRUE;
}

BOOL
CKarma::createBook(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	BOOL	bIsRemove	=	_lpReaction->m_aValue[1];

	if	(bIsRemove)
		_lpAsker->removeBook(_lpReaction->m_aValue[0]);
	else
	{
		cItem	item;

		if	(!g_im.generateBook(&item,_lpReaction->m_aValue[0]))
			return	FALSE;

		cItem	*lpSlot	=	_lpAsker->getValidInventorySlot();

		if	(!lpSlot)
			return	FALSE;

		memcpy(lpSlot,&item,sizeof(cItem));

		_lpAsker->sendPickItemLog(lpSlot,1);
		_lpAsker->sendAddItem(lpSlot);
	}

	return	TRUE;
}

BOOL
CKarma::createAddCategoryToBookItem(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	cItem	item;

	int	iBeginCategory	=	max(_lpReaction->m_aValue[1]-1,0);
	int	iEndCategory	=	max(_lpReaction->m_aValue[2]-1,0);

	if	(!g_im.generateBookCategory(&item,_lpReaction->m_aValue[0],iBeginCategory,iEndCategory))
		return	FALSE;

	cItem	*lpSlot	=	_lpAsker->getValidInventorySlot();

	if	(!lpSlot)
		return	FALSE;

	memcpy(lpSlot,&item,sizeof(cItem));

	_lpAsker->sendPickItemLog(lpSlot,1);
	_lpAsker->sendAddItem(lpSlot);

	return	TRUE;
}

BOOL
CKarma::createAddPageToBookItem(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	cItem	item;

	int	iBook			=	_lpReaction->m_aValue[0];
	int	iCategory		=	max(_lpReaction->m_aValue[1]-1,0);
	int	iBeginPage		=	max(_lpReaction->m_aValue[2]-1,0);
	int	iEndPage		=	max(_lpReaction->m_aValue[3]-1,0);
	int	iAddRemove		=	_lpReaction->m_aValue[4];

	if	(iAddRemove	==	0)
	{
		if	(!g_im.generateBookPage(&item,iBook,iCategory,iBeginPage,iEndPage))
			return	FALSE;

		cItem	*lpSlot	=	_lpAsker->getValidInventorySlot();

		if	(!lpSlot)
			return	FALSE;

		memcpy(lpSlot,&item,sizeof(cItem));

		_lpAsker->sendPickItemLog(lpSlot,1);
		_lpAsker->sendAddItem(lpSlot);
	}
	else
		_lpAsker->removeAddPageItem(iBook,iCategory,iBeginPage,iEndPage);

	return	TRUE;
}

BOOL
CKarma::createAddRandomCategoryToBookItem(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	cBookInfoDefine	*lpBook	=	g_book.getBook(_lpReaction->m_aValue[0]);

	if	(!lpBook)
		return	FALSE;

	int	iCategory	=	random(lpBook->m_wCategoryCount);

	cItem	item;

	if	(!g_im.generateBookCategory(&item,_lpReaction->m_aValue[0],iCategory,iCategory))
		return	FALSE;

	cItem	*lpSlot	=	_lpAsker->getValidInventorySlot();

	if	(!lpSlot)
		return	FALSE;

	memcpy(lpSlot,&item,sizeof(cItem));

	_lpAsker->sendPickItemLog(lpSlot,1);
	_lpAsker->sendAddItem(lpSlot);

	return	TRUE;
}

BOOL
CKarma::createAddRandomPageToBookItem(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	cBookInfoDefine	*lpBook	=	g_book.getBook(_lpReaction->m_aValue[0]);

	if	(!lpBook)
		return	FALSE;

	int	iCategory	=	random(lpBook->m_wCategoryCount);
	int	iPage		=	random(lpBook->m_aCategory[iCategory].m_iPageCount);

	cItem	item;

	if	(!g_im.generateBookPage(&item,_lpReaction->m_aValue[0],iCategory,iPage,iPage))
		return	FALSE;

	cItem	*lpSlot	=	_lpAsker->getValidInventorySlot();

	if	(!lpSlot)
		return	FALSE;

	memcpy(lpSlot,&item,sizeof(cItem));

	_lpAsker->sendPickItemLog(lpSlot,1);
	_lpAsker->sendAddItem(lpSlot);

	return	TRUE;
}

BOOL
CKarma::changeName(CKarmaItem *_lpReaction,cACTOR * _lpOwner)
{
	if	(!_lpOwner	||	!_lpReaction->m_strString)
		return	FALSE;

	strncpy(_lpOwner->m_strName,_lpReaction->m_strString,dNAME_LENGTH);

	_lpOwner->m_strName[dNAME_LENGTH-1]	=	NULL;	

	return	TRUE;
}

BOOL
CKarma::setEventValue(CKarmaItem *_lpReaction,cACTOR *_lpOwner)
{
	if	(!_lpOwner)
		return	FALSE;

	_lpOwner->m_wEventValue	=	_lpReaction->m_aValue[0];

	return	TRUE;
}

BOOL
CKarma::giveRedStonePiece(CKarmaItem *_lpReaction,cACTOR *_lpPlayer,cACTOR *_lpOwner)	//	붉은 보석 조각 주기
{
	cParty	*lpParty	=	_lpPlayer->getParty();
	cACTOR	*lpPlayer	=	_lpPlayer;

	if	(lpParty)
	{
		if	(lpParty->checkMemberLevel(0,_lpOwner->m_iLevel +  50 + lpPlayer->m_iLevelGetingRedStone)	==	FALSE)
			return	FALSE;

		lpPlayer	=	lpParty->getRandomInrangeMember(_lpPlayer);
	}

	if	(!lpPlayer || lpPlayer->isDeath())
		return	FALSE;

	int	iProcessLevel		=	lpPlayer->getQuestProcessLevel(eQT_NORMAL,167);

	if	(iProcessLevel		!=	0)
		return	FALSE;

	int	iRedStonePiece		=	3298;
	int	iRedStonePieceCount	=	lpPlayer->getItemCount(iRedStonePiece,FALSE);

	if	(iRedStonePieceCount	>=	50	)
		return	FALSE;

	if	(iRedStonePieceCount	==	0	&&	lpPlayer->getRemainInventorySlotCount()	<=	0)
		return	FALSE;

//	몬스터의 레벨이 플레이어 레벨 -50 이상이다.
	if	(lpPlayer->m_iLevel	>	_lpOwner->m_iLevel + 50 + lpPlayer->m_iLevelGetingRedStone)
		return	FALSE;

	cItem	item;

	g_im.generateItemByBaseItem(&item,iRedStonePiece,1);

	item.m_dwSerial	=	largeRandom(0x7ffffff0)+10;

	lpPlayer->sendPickItemLog(&item,1);
	lpPlayer->sendAddItem(&item);

	if	(item.isMoneyItem())
		CLOG("goldIOError","giveRedStonePiece");

	lpPlayer->addItemToInventory(&item);

	return	TRUE;
}

BOOL
CKarma::increaseEventValue(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
//	" 이벤트 [event]0 관련 수치를 증가 시킨다. 최대치[spin]1[1,30000].  중복 수령 체크 [hand_anhanda]2",
	int	iEvent				=	_lpReaction->m_aValue[0];
	int	iMax				=	_lpReaction->m_aValue[1];
	int	iHandaAnhanda		=	_lpReaction->m_aValue[2];

	SERVERStoWORLD_INCREASE_DB_EVENT_VALUE	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_INCREASE_DB_EVENT_VALUE);

	strcpy(packet.strUserID,_lpTarget->m_strId);
	strcpy(packet.strUserName,_lpTarget->m_strName);

	packet.wEventIndex		=	iEvent;
	packet.iMaxCount		=	iMax;
	packet.bReset			=	FALSE;
	if	(iHandaAnhanda	==	2)
		packet.bReset		=	TRUE;
	packet.bIsAllowMulipleID=	iHandaAnhanda;

	SEND_WOLRD_PACKET(&packet);

	return	TRUE;
}


BOOL
CKarma::timeStamp(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	int		iItem		=	_lpReaction->m_aValue[0];
	cItem	*lpItem		=	_lpTarget->getItemByBasicItem(iItem);
	int		iSlotIndex	=	_lpTarget->getItemSlotByBasicItem(iItem);

	if	(!lpItem)
		return	FALSE;

	CTimeInfo	itemTime;

	lpItem->m_year		=	EncodeItemExpireYear(g_currentTime.m_wYear);
	lpItem->m_month		=	g_currentTime.m_wMonth;
	lpItem->m_day		=	g_currentTime.m_wDay;
	lpItem->m_hour		=	g_currentTime.m_wHour;
	lpItem->m_minute	=	g_currentTime.m_wMinute;

	_lpTarget->sendUpdateItemData(iSlotIndex,TRUE);

	return	TRUE;
}

BOOL
CKarma::heavenReward(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	int					iTitleLevel	=	0;
	CPlayerTitleInfo	*lpTitle	=	_lpTarget->getTitle(eTN_RED_STONE_HUNTER);

	if	(lpTitle)
		iTitleLevel	=	lpTitle->m_bLevel;

	BOOL	bIsResetAllignmentPoint	=	FALSE;

	switch(_lpReaction->m_aValue[1])
	{
		case	0	:	//	"유니크 아이템 혹은 슈퍼 유니크 아이템 한 개 습득",
		{
			cItem	item;
			if	(!g_im.getCh5HeavenRewardUnique(&item,iTitleLevel,_lpTarget->m_wJob))
				return	FALSE;
			if	(!addItemByKaram(_lpTarget,&item))
				return	FALSE;

			bIsResetAllignmentPoint	=	TRUE;
			break;
		}
		case	1	:	//"보너스 스탯치",
		{
			if	(_lpTarget->m_bf4CH5StateBonusCount	>=	10)
				return	FALSE;

			_lpTarget->m_bf4CH5StateBonusCount++;
			_lpTarget->m_wLevelPoint	+=	5;

			_lpTarget->sendEtcWork(eEW_CH5_POINT_BONUS,0,_lpTarget->m_bf4CH5StateBonusCount,_lpTarget->m_wLevelPoint);

			break;
		}
		case	2	:	//	"보너스 스킬 포인트",
		{
			if	(_lpTarget->m_bf4CH5SkillBonusCount	>=	10)
				return	FALSE;

			_lpTarget->m_bf4CH5SkillBonusCount++;
			_lpTarget->m_dwSkillExperience	+=	100;

			_lpTarget->sendEtcWork(eEW_CH5_POINT_BONUS,1,_lpTarget->m_bf4CH5SkillBonusCount,_lpTarget->m_dwSkillExperience);
			break;
		}
		case	3	:	//	"보너스 HP",
		{
			if	(_lpTarget->m_bf4CH5HPBonusCount	>=	10)
				return	FALSE;

			_lpTarget->m_bf4CH5HPBonusCount++;

			_lpTarget->sendEtcWork(eEW_CH5_POINT_BONUS,2,_lpTarget->m_bf4CH5HPBonusCount);
			break;
		}
		case	4	:	//	"보너스 CP",
		{
			if	(_lpTarget->m_bf4CH5CPBonusCount	>=	10)
				return	FALSE;

			_lpTarget->m_bf4CH5CPBonusCount++;

			_lpTarget->sendEtcWork(eEW_CH5_POINT_BONUS,3,_lpTarget->m_bf4CH5CPBonusCount);
			break;
		}
		case	5	:	//	"보너스 경험치",
		{
			float		fPercentage;

			if	(_lpTarget->m_iLevel	>=	771)
				//	(11-0.01*플레이어레벨+0.6*사냥꾼 레벨)%
				fPercentage	=	(11.0f-0.01f*_lpTarget->m_iLevel+0.6f*iTitleLevel);
			else
			if	(_lpTarget->m_iLevel	>=	720)
				//	(274-0.35*플레이어레벨+0.6*사냥꾼 레벨)%
				fPercentage	=	(274.0f-0.35f*_lpTarget->m_iLevel+0.6f*iTitleLevel);
			else
				//	(30-0.01*플레이어레벨+0.6*사냥꾼 레벨)%
				fPercentage	=	(30-0.01f*_lpTarget->m_iLevel+0.6f*iTitleLevel);

			LONGLONG	llExp		=	g_aExpTable[_lpTarget->m_iLevel];

			llExp	=	(LONGLONG)(llExp*fPercentage/100.0f);

			_lpTarget->increaseExperience((int)llExp,TRUE,TRUE,FALSE);
			break;
		}
		case	6	:	//"칭호 보너스",
		{
			break;
		}
		case	7	:	//	"길드 레벨 보너스",
		{
			CGuild	*lpGuild	=	_lpTarget->getGuild();

			if	(!lpGuild	||	lpGuild->m_wLevel	>=	100)
				return	FALSE;

			int			iPercentage	=	30+iTitleLevel*2;
			LONGLONG	llExp		=	g_aExpTable[lpGuild->m_wLevel];

			llExp	=	llExp*iPercentage/100;

			lpGuild->m_dwExpInServer+=	(int)llExp;

			WORD	wValue1	=	(WORD)(llExp>>16);
			WORD	wValue2	=	(WORD)(llExp&0xffff);

			_lpTarget->sendEtcWork(eEW_GUILD_EXP,wValue1,wValue2);
			break;
		}

		case	8	:	//	"길드 스킬 보너스",
		{
			CGuild	*lpGuild	=	_lpTarget->getGuild();

			if	(!lpGuild	||	lpGuild->m_bf6BonusGuildPoint	>=	c_iMaxBonusGuildPoint)
				return	FALSE;

			g_game.sendWPSimpleWork(eSWGW_INCREASE_BONUS_GUILD_POINT,lpGuild->m_wSerial);

			break;
		}

		case	9	:	//	"길드 석상 보너스",
		{
			CGuild	*lpGuild	=	_lpTarget->getGuild();

			if	(!lpGuild)
				return	FALSE;

			if	(lpGuild->increaseRandomGuildStatueLevel(iTitleLevel/2)	==	FALSE)
				return	FALSE;

			break;
		}

		case	10	:	//	"성향 조정","end"},	//	eKARMA_KW_HEAVEN_REWARD,//	인스턴스 던전으로 이동
		{
			_lpTarget->m_sAllignment	+=	5+iTitleLevel;
			_lpTarget->sendEtcWork(eEW_INCREASE_ALLIGNMENT,5+iTitleLevel,_lpTarget->m_sAllignment);
			break;
		}
	}

	if	(bIsResetAllignmentPoint)
	{
		_lpTarget->m_sAllignment	=	0;
		_lpTarget->sendEtcWork(eEW_INCREASE_ALLIGNMENT,0,0);
	}
	else
	{
		_lpTarget->m_sAllignment	+=	10;

		_lpTarget->m_sAllignment	=	min(100,_lpTarget->m_sAllignment);

		_lpTarget->sendEtcWork(eEW_INCREASE_ALLIGNMENT,10,_lpTarget->m_sAllignment);
	}

	g_game.sendWPSimpleWork(eSWGW_INCREASE_RED_STONE_COUNT,eRSRT_HEAVEN);

	return	TRUE;
}

BOOL
CKarma::hellReward(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	int					iTitleLevel	=	0;
	CPlayerTitleInfo	*lpTitle	=	_lpTarget->getTitle(eTN_RED_STONE_HUNTER);

	if	(lpTitle)
		iTitleLevel	=	lpTitle->m_bLevel;

	BOOL	bIsResetAllignmentPoint	=	FALSE;
//	{"거래 불가 아이템 거래 가능하게 변경","접두사 제거","스탯 리버스","스킬 리버스","HP 보너스 복구","CP 보너스 복구","스탯 보너스 복구","스킬 보너스 복구","길드 스킬 리셋","길드 스킬 보너스 복구","성향 조정","end"},	//	eKARMA_KW_HELL_REWARD

	switch(_lpReaction->m_aValue[1])
	{
		case	0	:	//	"거래 불가 아이템 거래 가능하게 변경"
		{
			bIsResetAllignmentPoint	=	TRUE;
			break;
		}
		case	1	:	//	"접두사 제거"
		{
			break;
		}
		case	2	:	//	"스탯 리버스"
		case	3	:	//	,"스킬 리버스"
		{
			bIsResetAllignmentPoint	=	TRUE;
			break;
		}

		case	4	:	//HP 보너스 복구
		{
			_lpTarget->m_bf4CH5PeneltyHPBonusCount	=	0;
			bIsResetAllignmentPoint	=	TRUE;

			_lpTarget->sendEtcWork(eEW_CH5_RECOVER_REVERSE_PENELTY,0);
			break;
		}
		case	5	://"CP 보너스 복구
		{
			_lpTarget->m_bf4CH5PeneltyCPBonusCount	=	0;
			bIsResetAllignmentPoint	=	TRUE;

			_lpTarget->sendEtcWork(eEW_CH5_RECOVER_REVERSE_PENELTY,1);
			break;
		}
		case	8	://"길드 스킬 리셋
		{
			break;
		}
		case	9	://"길드 스킬 보너스 복구
		{
			CGuild	*lpGuild	=	_lpTarget->getGuild();

			if	(!lpGuild	||	lpGuild->m_bf6PeneltyBonusGuildPoint	==	0)
				return	FALSE;

			bIsResetAllignmentPoint	=	TRUE;

			g_game.sendWPSimpleWork(eSWGW_RECOVER_BONUS_GUILD_POINT_PENELTY,lpGuild->m_wSerial);

			break;
		}
		case	10	://"성향 조정
		{
			_lpTarget->m_sAllignment	-=	5+iTitleLevel;
			_lpTarget->sendEtcWork(eEW_DECREASE_ALLIGNMENT,5+iTitleLevel,_lpTarget->m_sAllignment);
			break;
		}
	}

	if	(bIsResetAllignmentPoint)
	{
		_lpTarget->m_sAllignment	=	0;
		_lpTarget->sendEtcWork(eEW_INCREASE_ALLIGNMENT,0,0);
	}
	else
	{
		_lpTarget->m_sAllignment	-=	10;

		_lpTarget->m_sAllignment	=	max(-100,_lpTarget->m_sAllignment);
		_lpTarget->sendEtcWork(eEW_DECREASE_ALLIGNMENT,10,_lpTarget->m_sAllignment);
	}

	g_game.sendWPSimpleWork(eSWGW_INCREASE_RED_STONE_COUNT,eRSRT_HELL);

	return	TRUE;
}

BOOL
CKarma::redDevilReward(CKarmaItem *_lpReaction,cACTOR *_lpTarget)
{
	int					iTitleLevel	=	0;
	CPlayerTitleInfo	*lpTitle	=	_lpTarget->getTitle(eTN_RED_STONE_HUNTER);

	if	(lpTitle)
		iTitleLevel	=	lpTitle->m_bLevel;

	BOOL	bIsResetAllignmentPoint	=	FALSE;

	switch(_lpReaction->m_aValue[1])
	{
		case	0	://아이템 복제
		{
			bIsResetAllignmentPoint	=	TRUE;
			break;
		}
		case	1	://"아이템 능력 향상
		{
			break;
		}
		case	2	://"노말, 레어 아이템 잠재 능령 향상
		{
			break;
		}
		case	3	://"슈퍼 유니크 아이템 접두사 능력 향상"
		{
			break;
		}
		case	4	://"슈퍼 유니크 아이템 접두사 잠재 능력 향상
		{
			break;
		}
		case	5	://"붉은 색의 바닥 없는 상자
		{
			bIsResetAllignmentPoint	=	TRUE;
			break;
		}
		case	6	://"보너스 HP 부적
		{
			cItem	item;

			if	(!g_im.generateCh5BonusHPAmulet(&item,iTitleLevel))
				return	FALSE;

			if	(!addItemByKaram(_lpTarget,&item))
				return	FALSE;

			break;
		}
		case	7	://"보너스 CP 부적
		{
			cItem	item;

			if	(!g_im.generateCh5BonusCPAmulet(&item,iTitleLevel))
				return	FALSE;

			if	(!addItemByKaram(_lpTarget,&item))
				return	FALSE;

			break;
		}
		case	8	://"보너스 장비 부적
		{
			cItem	item;

			if	(!g_im.generateCh5BonusOverEquipLevelAmulet(&item,iTitleLevel))
				return	FALSE;

			if	(!addItemByKaram(_lpTarget,&item))
				return	FALSE;

			_lpTarget->m_bf4LevelOfLatestReceivedCH5EquipLevelBonus	=	iTitleLevel;

			break;
		}
		case	9	://"보너스 골드
		{
			int	iGoldRange	=	iTitleLevel*10000000+1;
			int	iGold		=	largeRandom(iGoldRange);

			if	(!addGoldByKarma(_lpTarget,iGold,0))
				return	FALSE;

			break;
		}
		case	10	://"성향 조정//"end"},
		{
			break;
		}
	}

	if	(bIsResetAllignmentPoint)
	{
		_lpTarget->m_sAllignment	=	0;
		_lpTarget->sendEtcWork(eEW_INCREASE_ALLIGNMENT,0,0);
	}

	g_game.sendWPSimpleWork(eSWGW_INCREASE_RED_STONE_COUNT,eRSRT_RED_DEVIL);

	return	TRUE;
}

BOOL
CKarma::setIncreaseInstantExperience(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;
	
	_lpAsker->m_wIncreaseInstantExperience = _lpReaction->m_aValue[0];

	return TRUE;
}


BOOL
CKarma::showFireWorkInField(CKarmaItem *_lpReaction,cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;
	int iX		= _lpReaction->m_aValue[0];
	int iY		= _lpReaction->m_aValue[1];
	int iDestX	= _lpReaction->m_aValue[2];
	int iDestY	= _lpReaction->m_aValue[3];
	int iIndex	= _lpReaction->m_aValue[4];

	iX = iX<<dTILE_XSIZE_SHIFT;
	iY = iY<<dTILE_YSIZE_SHIFT;

	iDestX = iDestX<<dTILE_XSIZE_SHIFT;
	iDestY = iDestY<<dTILE_YSIZE_SHIFT;

	if(iIndex == 0)
		iIndex = random(10)+1;

	_lpAsker->m_lpField->addSendEtcWork(_lpAsker->m_wSerialInField,eEW_FIRE_FIRE_WORK,iX ,iY,iDestX ,iDestY,iIndex);

	return TRUE;
}
