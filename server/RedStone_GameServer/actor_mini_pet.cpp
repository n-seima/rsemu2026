#include	"actor_mini_pet.h"
#include	"packetManager.h"
#include	"cGAME.h"
#include	"Cguild.h"
#include	"field_oc.h"

const	int	c_aiMiniPetEvoultionBonusRate[3]	=	
{
	50,80,99
};

enum
{
	eIncrease_MiniPet_FireDamage,
	eIncrease_MiniPet_WaterDamage,
	eIncrease_MiniPet_WindDamage,
	eIncrease_MiniPet_EarthDamage,
	eIncrease_MiniPet_LightDamage,
	eIncrease_MiniPet_DarkDamage,
	eIncrease_MiniPet_MagicDamage,
	eIncrease_MiniPet_SkillActiveChance,
	eIncrease_MiniPet_BasicSkillLevel,
	eIncrease_MiniPet_SkillLevel,
	eIncrease_MiniPet_MagicDamage_vsBoss,
};

//	빈 미니펫 슬롯 구하기
cMiniPet*
cActor_miniPet::getInvalidMiniPetSlot()
{
	for (int i=0;i<c_iMiniPetCount;i++)
	{
		if	(m_aMiniPet[i].m_bf7Level	==	0)
			return	getMiniPet(i);
	}

	return	NULL;
}

int
cActor_miniPet::getInvalidMiniPetSlotIndex()
{
	for (int i=0;i<c_iMiniPetCount;i++)
	{
		if	(m_aMiniPet[i].m_bf7Level	==	0)
			return	i;
	}

	return	-1;
}

cMiniPet*
cActor_miniPet::getActiveMiniPet(int _iIndex)//	활성화된 미니펫
{
	if	 (_iIndex	==	0)
		_iIndex	=	m_bf5ActiveMiniPet;
	else
		_iIndex	=	m_bf5ActiveMiniPet2;

	cMiniPet	*lpMiniPet	=	getMiniPet(_iIndex);

	if	(lpMiniPet==NULL || lpMiniPet->m_bf7Level	==	0)
		return	NULL;

	return	lpMiniPet;
}

BOOL
cActor_miniPet::unsealMiniPet(cItem *_lpItem)
{
	int	iMiniPetCount		=	getMiniPetCount();
	int	iInvalidMiniPetSlot	=	getInvalidMiniPetSlotIndex();

	if	(iMiniPetCount	>=	c_iMiniPetCount)
	{
		sendRegistServerMessage(eRSM_CAN_NOT_SUMMON_MINI_PET_BY_FULL_COUNT);
		return	FALSE;
	}

	cMiniPet			*lpMiniPet	=	getMiniPet(iInvalidMiniPetSlot);

	int	iItemSlot	=	_lpItem	-	m_aItems;

	if	(iItemSlot	>=	dOWN_ITEM_COUNT)
	{
		iItemSlot	=	_lpItem	-	m_aExtraInventory;
		iItemSlot	+=	dOWN_ITEM_COUNT;
	}

	lpMiniPet->unseal(_lpItem);

	sendRemoveItemLog(_lpItem,eRI_BY_UNSEAL_MINIPET,m_wCurrentField);
	sendUnsealMiniPetLog(lpMiniPet,iInvalidMiniPetSlot);

	_lpItem->reset();

	sendMiniPetInfo(iInvalidMiniPetSlot,eUMIM_UNSEAL_MINIPET,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);

	immediatelySendSaveDataToDBMemory();

	return	TRUE;
}

void
cActor_miniPet::changeMiniPetPlace(int _iPetIndex,int _iDirect)	//	미니펫 위치 변경
{
	if	(_iPetIndex	<	0 || _iPetIndex	>=	c_iMiniPetCount)
		return;

	if	(m_aMiniPet[_iPetIndex].m_bf7Level	==	0)
		return;

	int	iTargetPlace	=	-1;

	if	(_iDirect	==	0)	//	왼쪽으로
	{
		if	((_iPetIndex	%	8)	==	0)
			return;

		iTargetPlace	=	_iPetIndex-1;
	}
	else
	if	(_iDirect	==	1)	//	오른으로
	{
		if	((_iPetIndex	%	8)	==	7)
			return;
		iTargetPlace	=	_iPetIndex+1;
	}
	else
	if	(_iDirect	==	2)	//	위쪽으로
	{
		if	((_iPetIndex/8)	==	0)
			return;
		iTargetPlace	=	_iPetIndex-8;
	}
	else
	if	(_iDirect	==	3)	//	아래으로
	{
		if	((_iPetIndex/8)	==	1)
			return;
		iTargetPlace	=	_iPetIndex+8;
	}
	else
		return;

	if	(iTargetPlace	<	0	||	iTargetPlace	>=	c_iMiniPetCount)
		return;

	cMiniPet	miniPet;

	memcpy(&miniPet,&m_aMiniPet[iTargetPlace],sizeof(cMiniPet));
	memcpy(&m_aMiniPet[iTargetPlace],&m_aMiniPet[_iPetIndex],sizeof(cMiniPet));
	memcpy(&m_aMiniPet[_iPetIndex],&miniPet,sizeof(cMiniPet));

	int	iOldPet1=m_bf5ActiveMiniPet,iOldPet2=m_bf5ActiveMiniPet2;

	if	(_iPetIndex			==	iOldPet1)
		m_bf5ActiveMiniPet	=	iTargetPlace;
	else
	if	(iTargetPlace		==	iOldPet1)
		m_bf5ActiveMiniPet	=	_iPetIndex;

	if	(_iPetIndex			==	iOldPet2)
		m_bf5ActiveMiniPet2	=	iTargetPlace;
	else
	if	(iTargetPlace		==	iOldPet2)
		m_bf5ActiveMiniPet2	=	_iPetIndex;

	buildPower();

	sendEtcWork(eEW_CHANGE_MINIPET_PLACE,_iPetIndex,iTargetPlace);
}
BOOL
cActor_miniPet::changeType(int _iPetIndex,int _iItemSlot)
{
	if	(_iPetIndex	<	0	||	_iPetIndex	>=	c_iMiniPetCount	||	m_aMiniPet[_iPetIndex].m_bf7Level == 0)
		return FALSE;

	if	(_iItemSlot	<	0)
		return FALSE;

	if	(_iItemSlot	>=	m_wItemSlotCount)
	{
		if	(_iItemSlot	<	dOWN_ITEM_COUNT)
			return FALSE;

		if	(_iItemSlot	>=	dOWN_ITEM_COUNT+m_bf6ExtraInventorySize)
			return FALSE;
	}

	cMiniPet	*lpPet			=	&m_aMiniPet[_iPetIndex];
	cItem		*lpItem			=	getInventoryItem(_iItemSlot);
	if(!lpPet)
		return FALSE;
	if(!lpItem)
		return FALSE;

	int iType =	lpItem->getSetMiniPetType();
	
	if(lpPet->m_iVitalPoint + lpPet->m_iEnergyPoint + lpPet->m_iMineralPoint == 0)
		return FALSE;
	if	(iType	==	nsMiniPetStyle::Bio)
	{
		lpPet->m_iVitalPoint += lpPet->m_iEnergyPoint + lpPet->m_iMineralPoint;

		lpPet->m_iEnergyPoint = 0;
		lpPet->m_iMineralPoint = 0;
	}
	else if(iType	==	nsMiniPetStyle::Energy)
	{
		lpPet->m_iEnergyPoint += lpPet->m_iVitalPoint + lpPet->m_iMineralPoint;

		lpPet->m_iVitalPoint = 0;
		lpPet->m_iMineralPoint = 0;
	}
	else if(iType	==	nsMiniPetStyle::Mineral)
	{
		lpPet->m_iMineralPoint += lpPet->m_iEnergyPoint + lpPet->m_iVitalPoint;
		lpPet->m_iEnergyPoint = 0;
		lpPet->m_iVitalPoint = 0;
	}
	lpPet->resetSkill();

	BOOL	bIsEvolution =	FALSE;

	if	(lpPet->m_bf7Level	>=	c_iMiniPetEvolutionLevel)
	{
		int iType =	lpPet->m_bf5Type - lpPet->m_bf5Type/6 * 6;
		lpPet->m_bf5Type = iType;
		lpPet->evolution(TRUE);
		bIsEvolution = TRUE;
	}
	buildPower();
	sendUseItemLog(lpItem);
	
	lpItem->decreaseCount(1);
	
	sendUpdateItemData(_iItemSlot,TRUE);
	sendMiniPetInfo(_iPetIndex,eUMIM_CHANGE_TYPE,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2,0, bIsEvolution);

	return TRUE;

}


BOOL
cActor_miniPet::resetSkill(int _iPetIndex,int _iItemSlot)
{
	if	(_iPetIndex	<	0	||	_iPetIndex	>=	c_iMiniPetCount	||	m_aMiniPet[_iPetIndex].m_bf7Level == 0)
		return FALSE;

	if	(_iItemSlot	<	0)
		return FALSE;

	if	(_iItemSlot	>=	m_wItemSlotCount)
	{
		if	(_iItemSlot	<	dOWN_ITEM_COUNT)
			return FALSE;

		if	(_iItemSlot	>=	dOWN_ITEM_COUNT+m_bf6ExtraInventorySize)
			return FALSE;
	}


	cMiniPet	*lpPet			=	&m_aMiniPet[_iPetIndex];
	cItem		*lpItem			=	getInventoryItem(_iItemSlot);
	if(!lpPet)
		return FALSE;
	if(!lpItem)
		return FALSE;

	BOOL bIsResetSkill = lpPet->resetSkill();

	
	if(bIsResetSkill)
	{
		buildPower();
		sendUseItemLog(lpItem);
		
		lpItem->decreaseCount(1);
		
		sendUpdateItemData(_iItemSlot,TRUE);
		sendMiniPetInfo(_iPetIndex,eUMIM_RESET_SKILL,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);
	}
	else
	{
		sendMiniPetInfo(_iPetIndex,eUMIM_FAIL_RESET_SKILL,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);
	}
	return TRUE;
}
void
cActor_miniPet::resetMiniPetName(int _iItem,int _iMiniPet)
{
	cItem	*lpItem	=	getInventoryItem(_iItem);

	if	(!lpItem)
		return;

	cItem	itemForLog;

	itemForLog.copy(lpItem);

	cMiniPet	*lpMiniPet	=	getMiniPet(_iMiniPet);

	if	(lpItem->checkFirstEffect(eIE_CHANGE_MINIPET_NAME)	==	FALSE	||	!lpMiniPet)
		return;

	if	(lpMiniPet->m_bf7Level == 0)
	{
		sendRegistServerMessage(eRSM_PLEASE_SELECT_MINIPET_FOR_CHANGE_NAME,eSM_SYSTEM);
		return;
	}

	if	(STRICMP(lpMiniPet->m_strName,"noname")	==	0)
		return;

	strcpy(lpMiniPet->m_strName,"noname");
	sendMiniPetInfo(_iMiniPet,eUMIM_RESET_MINIPET_NAME,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);

	lpItem->decreaseCount(1);

	sendUpdateItemData(_iItem,TRUE);

	immediatelySendSaveDataToDBMemory();
}

void
cActor_miniPet::usePolisher(int _iItem,int _iMiniPet,BOOL _bIsMassive)
{
	cItem	*lpItem	=	getInventoryItem(_iItem);

	if	(!lpItem)
		return;

	if	(getMiniPetCount()	<=	0)
	{
		sendRegistServerMessage(eRSM_REQUIRE_MINIPET_FOR_USE_THAT_ITEM,eSM_SYSTEM);
		return;
	}

	cItem	itemForLog;

	itemForLog.copy(lpItem);
	
	if	(_bIsMassive)
	{
		if	(lpItem->isMassivePolisher()	==	FALSE)
			return;

		for (int i=0;i<c_iMiniPetCount;i++)
		{
			if	(m_aMiniPet[i].m_bf7Level	==	0)
				continue;

			m_aMiniPet[i].usePolisher();
			sendMiniPetInfo(i,eUMIM_USE_POLISHER,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);
			sendExtendMiniPetTimeLog(&m_aMiniPet[i],i);
		}
	}
	else
	{
		cMiniPet	*lpMiniPet	=	getMiniPet(_iMiniPet);

		if	(lpItem->isPolisher()	==	FALSE	||	!lpMiniPet)
			return;

		lpMiniPet->usePolisher();
		sendMiniPetInfo(_iMiniPet,eUMIM_USE_POLISHER,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);
		sendExtendMiniPetTimeLog(lpMiniPet,_iMiniPet);
	}

	lpItem->decreaseCount(1);

	sendUpdateItemData(_iItem,TRUE);

	if	(lpItem->m_bCount	==	0)
		sendRemoveItemLog(&itemForLog,eRI_BY_USE_POLISHER,m_wCurrentField);

	immediatelySendSaveDataToDBMemory();

}


//JBC		미니펫 3창 강화 아이템 사용 , 각성의 씨앗 , 각성의 열매		09-01-02
void
cActor_miniPet::useAwakenItem(int _iItem,int _iMiniPet,BOOL _bIs100PersentItem)
{
	cItem	*lpItem	=	getInventoryItem(_iItem);
	
	if	(!lpItem)
		return;
	
	if	(getMiniPetCount()	<=	0)
	{
		sendRegistServerMessage(eRSM_REQUIRE_MINIPET_FOR_USE_THAT_ITEM,eSM_SYSTEM);
		return;
	}

	cMiniPet	*lpMiniPet	=	getMiniPet(_iMiniPet);

	if(!lpMiniPet)
		return;

	//50%각성 해있을때는 미니펫이 각성햇으면 실패.
	if	(_bIs100PersentItem == FALSE && (lpMiniPet->m_bf1MinipetAwaken100 == TRUE || lpMiniPet->m_bf1MinipetAwaken50 == TRUE))	
	{
		sendRegistServerMessage(eRSM_MINI_PET_CANT_USE_AWAKEN_ITEM_LOWER,eSM_SYSTEM);
		return;
	}

	//100%각성아이템을 썻는데 이미 100%각성해 있으면 실패.
	if	(_bIs100PersentItem == TRUE && lpMiniPet->m_bf1MinipetAwaken100 == TRUE )
	{
		sendRegistServerMessage(eRSM_MINI_PET_CANT_USE_AWAKEN_ITEM_LOWER,eSM_SYSTEM);
		return;
	}
	
	cItem	itemForLog;
	
	if	(_bIs100PersentItem)
	{
		if	(lpItem->isMiniPetAwakenItem100()	==	FALSE)
			return;

		lpMiniPet->m_bf1MinipetAwaken100 = TRUE;
	}
	else
	{
		cMiniPet	*lpMiniPet	=	getMiniPet(_iMiniPet);
		
		if	(lpItem->isMiniPetAwakenItem50()	==	FALSE	||	!lpMiniPet)
			return;
		
		lpMiniPet->m_bf1MinipetAwaken50 = TRUE;
	}
	
	itemForLog.copy(lpItem);

	lpItem->decreaseCount(1);
	
	sendUpdateItemData(_iItem,TRUE);

	sendMiniPetInfo(_iMiniPet , eUMIM_FEED_AWAKEN_ITEM , m_bf5ActiveMiniPet , m_bf5ActiveMiniPet2 , 0 , FALSE , 0 , !_bIs100PersentItem , _bIs100PersentItem);
	
	if	(lpItem->m_bCount	==	0 && _bIs100PersentItem)
		sendRemoveItemLog(&itemForLog,eRI_BY_USE_POLISHER,m_wCurrentField);
	
	immediatelySendSaveDataToDBMemory();
	
}

BOOL
cActor_miniPet::extendReversalFrameTime(int _iTimes,BOOL _bIsDay,BOOL _bIsSendMessage)	//	양면 프레임 소환 시간 연장
{
	CTimeInfo	expireTime,currentTime;

	currentTime.update();

	expireTime.m_wYear	=	m_reversalFrameYear+2000;
	expireTime.m_wMonth	=	m_reversalFrameMonth;
	expireTime.m_wDay	=	m_reversalFrameDay;
	expireTime.m_wHour	=	m_reversalFrameHour;
	expireTime.m_wMinute=	m_reversalFrameMinute;	//	25

	if	(currentTime.getMinuteValue(2000)	<	expireTime.getMinuteValue(2000))
	{
		currentTime.m_wYear	=	m_reversalFrameYear	+2000;
		currentTime.m_wMonth=	m_reversalFrameMonth	;
		currentTime.m_wDay	=	m_reversalFrameDay		;
		currentTime.m_wHour	=	m_reversalFrameHour		;
		currentTime.m_wMinute=	m_reversalFrameMinute	;
	}

	if	(_bIsDay)
		currentTime.increaseDay(_iTimes);
	else
		currentTime.increaseMinute(_iTimes);

	m_reversalFrameYear		=	currentTime.m_wYear-2000;
	m_reversalFrameMonth	=	currentTime.m_wMonth;
	m_reversalFrameDay		=	currentTime.m_wDay;
	m_reversalFrameHour		=	currentTime.m_wHour;
	m_reversalFrameMinute	=	currentTime.m_wMinute;

	if	(_bIsSendMessage)
		sendEtcWork(eEW_EXTEND_REVERSIBLE_VALIDATE,m_reversalFrameYear,m_reversalFrameMonth,m_reversalFrameDay,m_reversalFrameHour,m_reversalFrameMinute);

	return	TRUE;
}

void
cActor_miniPet::useBlockLightPocket(int _iMiniPet)
{
	if	(_iMiniPet	<	0	||	_iMiniPet	>=	c_iMiniPetCount)
		return;

	cMiniPet	*lpMiniPet	=	getMiniPet(_iMiniPet);

	if	(lpMiniPet==NULL || lpMiniPet->m_bf7Level	==	0)
		return;

	int			iItemSlot	=	getBlockLightPocketItemIndex();

	if	(iItemSlot	==	0xffff)
		return;

	cItem		*lpItem		=	getInventoryItem(iItemSlot);
	cItem		itemForLog;

	itemForLog.copy(lpItem);

	cBlockLightPocket	*lpPocket	=	(cBlockLightPocket	*)lpItem;

	lpPocket->m_wBaseItem		=	g_im.m_iBlockLightPocket;
	lpPocket->m_bf7Level		=	lpMiniPet->m_bf7Level;
	lpPocket->m_bf5Type			=	lpMiniPet->m_bf5Type;
	lpPocket->m_bf16Exp			=	lpMiniPet->m_bf16Exp;
	lpPocket->m_bf7SkillPoint	=	lpMiniPet->m_bf7SkillPoint;
	lpPocket->m_iEnergyPoint	=	lpMiniPet->m_iEnergyPoint;
	lpPocket->m_iMineralPoint	=	lpMiniPet->m_iMineralPoint;
	lpPocket->m_iVitalPoint		=	lpMiniPet->m_iVitalPoint;

	lpPocket->m_bf4ExpireMonth	=	lpMiniPet->m_bf4ExpireMonth;
	lpPocket->m_bf5ExpireYear	=	lpMiniPet->m_bf5ExpireYear;
	lpPocket->m_bf5ExpireDay	=	lpMiniPet->m_bf5ExpireDay;
	lpPocket->m_bf5ExpireHour	=	lpMiniPet->m_bf5ExpireHour;
	lpPocket->m_bf6ExpireMinute	=	lpMiniPet->m_bf6ExpireMinute;
	lpPocket->m_bf1isUseAwakenItem50	=	lpMiniPet->m_bf1MinipetAwaken50;
	lpPocket->m_bf1isUseAwakenItem100	=	lpMiniPet->m_bf1MinipetAwaken100;

	lpPocket->m_bf3SkillLevel1=	lpMiniPet->m_aAbility[0].m_bf4Level;
	lpPocket->m_bf3SkillLevel2=	lpMiniPet->m_aAbility[1].m_bf4Level;
	lpPocket->m_bf3SkillLevel3=	lpMiniPet->m_aAbility[2].m_bf4Level;
	lpPocket->m_bf3SkillLevel4=	lpMiniPet->m_aAbility[3].m_bf4Level;
	lpPocket->m_bf3SkillLevel5=	lpMiniPet->m_aAbility[4].m_bf4Level;
	lpPocket->m_bf3SkillLevel6=	lpMiniPet->m_aAbility[5].m_bf4Level;
	lpPocket->m_bf3SkillLevel7=	lpMiniPet->m_aAbility[6].m_bf4Level;
	lpPocket->m_bf3SkillLevel8=	lpMiniPet->m_aAbility[7].m_bf4Level;
	lpPocket->m_bf3SkillLevel9=	lpMiniPet->m_aAbility[8].m_bf4Level;
	lpPocket->m_bf3SkillLevel10=lpMiniPet->m_aAbility[9].m_bf4Level;	//	30
	lpPocket->m_bf3SkillLevel11=lpMiniPet->m_aAbility[10].m_bf4Level;	//	30

	sendUpdateItemData(iItemSlot,TRUE);
	sendRemoveItemLog(&itemForLog,eRI_BY_SEAL_MINIPET,m_wCurrentField);
	sendSealMiniPetLog(lpMiniPet,_iMiniPet);
	sendCreateItemLog(lpItem,eCIM_BLOCK_LIGHT_POCKET);

	if	(_iMiniPet	==	(int)m_bf5ActiveMiniPet)
	{
		copy2To1();
		reset2();
	}
	else
	if	(_iMiniPet	==	(int)m_bf5ActiveMiniPet2)
		reset2();

	lpMiniPet->reset();

	sendMiniPetInfo(_iMiniPet,eUMIM_SEAL_MINIPET,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);

	immediatelySendSaveDataToDBMemory();
}	
	
void
cActor_miniPet::changeMinipetSkillOnOff(int _iIndex,int _iSkillSlot)
{
	cMiniPet	*lpMiniPet	=	getMiniPet(_iIndex);

	if	(lpMiniPet==NULL || lpMiniPet->m_bf7Level	==	0	||	_iSkillSlot	<	0	||	_iSkillSlot	>=	c_iMiniPetSkillCount)
		return;

	cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[_iSkillSlot];

	if(lpAbility->m_bf4Level <= 0)
		return;

	lpAbility->m_bf1IsOFF = 1 - lpAbility->m_bf1IsOFF;
	buildPower();
	sendMiniPetInfo(_iIndex,eUMIM_UPDATE,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);
}


void
cActor_miniPet::useChargeSkill(cACTOR *_lpTarget,int _iPetOrder)
{
	if	(_iPetOrder	<	0	||	_iPetOrder	>	2)
		return;

	if	(_iPetOrder	&&	m_wIsActiveReversalFrame	==	0)
		return;

	int	iPetIndex	=	m_bf5ActiveMiniPet;

	if	 (_iPetOrder)
		iPetIndex	=	m_bf5ActiveMiniPet2;

	if	(!isAvailMiniPet(_iPetOrder))
		return;

	if	(!_lpTarget || _lpTarget->isNpc() || isOwnForce(_lpTarget))
		return;

	cMiniPet			*lpMiniPet	=	&m_aMiniPet[iPetIndex];
	cAbility			ability;
	cMiniPetAbilityInfo	*lpChargeAbility	=	NULL;
	cActiveMiniPetInfo	*lpInfo		=	&m_aActiveMiniPetInfo[_iPetOrder];

	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[iSkill];

		if	(lpAbility->m_bf4Level	==	0)
			continue;

		if	(lpAbility->m_bf1IsOFF)
			continue;

		CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

		if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::ChargeAndFire)
			continue;

		int	iChargeCount	=	lpSkill->m_aValue[0];
		iChargeCount		-=	lpMiniPet->getDecreaseChargeCount(this , _iPetOrder);

		if(lpMiniPet->m_bf1MinipetAwaken100)
			iChargeCount	-=	lpSkill->m_wAwakenSkillBonusActiveChance*2;	//무기질에서는 발동확률이 차지카운트 줄이는 수이다.
		else if(lpMiniPet->m_bf1MinipetAwaken50)
			iChargeCount	-=	lpSkill->m_wAwakenSkillBonusActiveChance;

		if	(lpInfo->m_wChargeCountForMiniPet	<	iChargeCount)
			break;

		lpChargeAbility	=	lpAbility;
		ability.set(lpAbility->m_bf11Skill,lpAbility->m_bf4Level);

		if	(ability.getSkill()->m_bf1IsNormalMiniPetSkill)
		{
			ability.m_wLevel	+=	m_bIncreaseMiniPetBasicSkillLevel;
			ability.m_wLevel	+=	m_abCorrectMiniPetBasicSkillLevel[_iPetOrder];
		}

		ability.m_wLevel	+=	m_bIncreaseMiniPetSkillLevel;
		ability.m_wLevel	+=	m_abCorrectMiniPetSkillLevel[_iPetOrder];

		break;
	}

	if	(!lpChargeAbility)
	{
		sendEtcWork(eEW_READY_TO_FIRE_CHARGE_MINIPET_SKILL,FALSE,_iPetOrder);
		return;
	}

	CSkill	*lpSkill	=	ability.getSkill();

	if	(lpSkill->m_wType	==	eSKILL_TYPE_MISSILE)
		castContinuousShoot(lpMiniPet,lpChargeAbility,_lpTarget,iPetIndex);
	else
	if	(lpSkill->m_wType	==	eSKILL_TYPE_IMPACT)
	{
		if	(lpSkill->isCastOnEnemySkill())
		{
			if	(!_lpTarget)
			{
				sendRegistServerMessage(eRSM_ITS_USE_TO_ENEMY_SKILL,eSM_WARNING);
				return;
			}

			if	(!_lpTarget->isInAttackRange(m_pos.x,m_pos.y,ability.getAttackRange(NULL,0)))
			{
				sendRegistServerMessage(eRSM_TOO_FAR_TO_USE_SKILL,eSM_WARNING);
				return;
			}
		}

		if	(castImpactSkill(lpMiniPet,lpChargeAbility,_lpTarget,iPetIndex,m_iLevel))
		{
			lpInfo->m_wChargeCountForMiniPet	=	0;
			lpInfo->m_wIsCharged	=	FALSE;
			sendEtcWork(eEW_READY_TO_FIRE_CHARGE_MINIPET_SKILL,FALSE,_iPetOrder);
		}
		else
			sendRegistServerMessage(eRSM_CAN_NOT_FIND_ATTACK_ABLE_TARGET,eSM_WARNING);
	}
}

void
cActor_miniPet::increaseMiniPetSkillLevel(int _iIndex,int _iSkillSlot)
{
	cMiniPet	*lpMiniPet	=	getMiniPet(_iIndex);

	if	(lpMiniPet==NULL || lpMiniPet->m_bf7Level	==	0	||	_iSkillSlot	<	0	||	_iSkillSlot	>=	c_iMiniPetSkillCount)
		return;

	cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[_iSkillSlot];

	if	(lpMiniPet->isLevelUpAbleAbility(lpAbility)	==	FALSE)
		return;

	lpAbility->m_bf4Level++;

	lpMiniPet->m_bf7SkillPoint	-=	lpAbility->m_bf4Level;

	int	iAddSkill	=	lpMiniPet->addSpareSkill();

	if	(iAddSkill	!=	-1)
		sendEtcWork(eEW_ADD_MINIPET_SPARE_SKILL,iAddSkill);

	if	(_iIndex	==	(int)m_bf5ActiveMiniPet	||	(_iIndex	==	(int)m_bf5ActiveMiniPet2	&&	m_wIsActiveReversalFrame))
		buildPower();

	sendMiniPetInfo(_iIndex,eUMIM_INCREASE_SKILL_LEVEL,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);
	updateChargeCount(TRUE);
}



void
cActor_miniPet::copy2To1()
{
	memcpy(&m_aActiveMiniPetInfo[0],&m_aActiveMiniPetInfo[1],sizeof(m_aActiveMiniPetInfo[0]));
	m_bf5ActiveMiniPet	=	m_bf5ActiveMiniPet2;
}

void
cActor_miniPet::copy1To2()
{
	memcpy(&m_aActiveMiniPetInfo[1],&m_aActiveMiniPetInfo[0],sizeof(m_aActiveMiniPetInfo[0]));
	m_bf5ActiveMiniPet2	=	m_bf5ActiveMiniPet;
}

void
cActor_miniPet::reset1()
{
	memset(&m_aActiveMiniPetInfo[0],0,sizeof(m_aActiveMiniPetInfo[0]));
	m_bf5ActiveMiniPet		=	c_iMiniPetCount;
}

void
cActor_miniPet::reset2()
{
	memset(&m_aActiveMiniPetInfo[1],0,sizeof(m_aActiveMiniPetInfo[0]));
	m_bf5ActiveMiniPet2		=	c_iMiniPetCount;
}


void
cActor_miniPet::activateMiniPet(int _iIndex,BOOL _bIsActivate)
{
	if	(_bIsActivate)
	{
		cMiniPet	*lpMiniPet	=	getMiniPet(_iIndex);

		if	(lpMiniPet==NULL || lpMiniPet->m_bf7Level	==	0)
			return;

		if	(lpMiniPet->isExpired())
		{
			sendRegistServerMessage(eRSM_CAN_NOT_ACTIVATE_MINIPET_BY_IS_EXPIRED_MINIPET,eSM_WARNING);
			return;
		}

		if	(m_bf5ActiveMiniPet	==	(UINT)_iIndex)
			return;

		if	(m_wIsActiveReversalFrame)
		{
			if	(m_bf5ActiveMiniPet2	==	(UINT)_iIndex)
				return;

			if	(m_bf5ActiveMiniPet		!=	c_iMiniPetCount)
				copy1To2();
		}

		reset1();
		m_bf5ActiveMiniPet	=	_iIndex;

		sendMiniPetInfo(_iIndex,eUMIM_ACTIVATE,_iIndex,m_bf5ActiveMiniPet2);
		buildPower();

		addSendSimpleInfo();
	}
	else
	{
		if	(m_bf5ActiveMiniPet	==	(DWORD)_iIndex)
		{
			if	(m_wIsActiveReversalFrame)
			{
				copy2To1();
				m_bf5ActiveMiniPet	=	m_bf5ActiveMiniPet2;

				reset2();
			}
			else
				reset1();
		}

		if	(m_bf5ActiveMiniPet2==	_iIndex)
			reset2();

		sendMiniPetInfo(_iIndex,eUMIM_DISACTIVATE,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2,_iIndex);
		buildPower();

		addSendSimpleInfo();
	}
}

BOOL
cActor_miniPet::sendMiniPetInfo(int _iSlot,int _iMethod,int _iActivePet,int _iActivePet2,int _iIncreaseExp,BOOL _bIsEvolution,int _iFeedType,  BOOL _bIsuseAwakenItme50 , BOOL _bIsuseAwakenItme100)
{
	cMiniPet	*lpMiniPet	=	getMiniPet(_iSlot);

	SG_MINI_PET_INFO	packet;

	packet.base.set(sizeof(packet),dSG_MINI_PET_INFO);
	packet.wPetIndex	=	_iSlot;
	packet.wMethod		=	_iMethod;
	packet.bf5ActivePet	=	_iActivePet;
	packet.bf5ActivePet2=	_iActivePet2;
	
	packet.bf1IsEvolution=	_bIsEvolution;
	packet.wIncreaseExp	=	_iIncreaseExp;
	packet.bf4FeedType	=	_iFeedType;
	packet.bf1IsUseMiniPetAwakenItem50		= _bIsuseAwakenItme50;
	packet.bf1IsUseMiniPetAwakenItem100		= _bIsuseAwakenItme100;

	packet.bf1Charge1	=	m_aActiveMiniPetInfo[0].m_wIsCharged;
	if	(m_wIsActiveReversalFrame)
		packet.bf1Charge2	=	m_aActiveMiniPetInfo[1].m_wIsCharged;
	else
		packet.bf1Charge2	=	FALSE;

	if	(lpMiniPet)
		memcpy(&packet.miniPet,lpMiniPet,sizeof(cMiniPetCommon));
	else
		memset(&packet.miniPet,0,sizeof(cMiniPet));

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

//	미니펫 소환
BOOL
cActor_miniPet::createMiniPet(cItem *_lpItem,int _iCreateMethod)
{
	if	(_iCreateMethod	==	-1)
		_iCreateMethod	=	eUMIM_CREATE;

	int	iMiniPetCount		=	getMiniPetCount();
	int	iInvalidMiniPetSlot	=	getInvalidMiniPetSlotIndex();

	if	(iMiniPetCount	>=	c_iMiniPetCount)
	{
		sendRegistServerMessage(eRSM_CAN_NOT_SUMMON_MINI_PET_BY_FULL_COUNT);
		return	FALSE;
	}

	cMiniPet	*lpMiniPet	=	getMiniPet(iInvalidMiniPetSlot);

	int	iChanceOfRecallLightOrDarkSprit	=	40+5*iMiniPetCount;
	int	iRecallMinipetType				=	-1;

	if	(random(1000)	<	iChanceOfRecallLightOrDarkSprit)
	{
		iRecallMinipetType	=	nsMiniPetType::SpritOfLight+random(2);
	}
	else
	{
		iRecallMinipetType	=	nsMiniPetType::SpritOfFire+random(4);
	}

	lpMiniPet->create(iRecallMinipetType);

	if	(iMiniPetCount	==	0)
		activateMiniPet(0,TRUE);
	else
	{
		sendMiniPetInfo(iInvalidMiniPetSlot,_iCreateMethod,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);
		addSendSimpleInfo();
	}

	sendCreateMiniPetLog(_lpItem,lpMiniPet,iInvalidMiniPetSlot);

	immediatelySendSaveDataToDBMemory();

	return	TRUE;
}

int
cActor_miniPet::getMiniPetCount()
{
	int	iCount	=	0;

	for (int i=c_iMiniPetCount;i;)
	{
		--i;

		if	(m_aMiniPet[i].m_bf7Level	==	0)
			continue;

		iCount++;
	}

	return	iCount;
}

void
cActor_miniPet::feedAllItem(int _iPetIndex,int _iItemType , BOOL _bIncludeExtraInven)
{
	if	(_iPetIndex	<	0	||	_iPetIndex	>=	c_iMiniPetCount	||	m_aMiniPet[_iPetIndex].m_bf7Level == 0)
		return;
	

	int	i,iCount=0;
	
	for (i=0;i<dOWN_ITEM_COUNT;i++)
		if	(m_aItems[i].getMiniPetFeedType()	==	_iItemType)	
		{
			feedItem(_iPetIndex , i);
		}

	if(_bIncludeExtraInven)
		for (i=0;i<m_bf6ExtraInventorySize;i++)
			if (m_aExtraInventory[i].getMiniPetFeedType()	==	_iItemType)	
			{
				feedItem(dOWN_ITEM_COUNT + _iPetIndex , i);
			}
}

void
cActor_miniPet::restraintMiniPet(int _iItem,int _iMiniPet)
{
	cItem	*lpItem	=	getInventoryItem(_iItem);

	if	(!lpItem)
		return;

	if	(!lpItem->isOwnSpecificEffectItem(eIE_CHAIN_OF_RESTRAINT))
		return;

	cMiniPet*	lpMiniPet	=	getMiniPet(_iMiniPet);

	if	(!lpMiniPet || lpMiniPet->m_bf7Level == 0)
		return;

	int	iAttr	=	lpMiniPet->getAttr();

	switch(iAttr)
	{
		case	nsMiniPetType::SpritOfFire		:
			if	(m_bf4EatFireMiniPetCount	>=	dLIMIT_EAT_SAME_TYPE_MINIPET_COUNT)
				return;

			m_bf4EatFireMiniPetCount++;
			break;
		case	nsMiniPetType::SpritOfWater		:
			if	(m_bf4EatWaterMiniPetCount	>=	dLIMIT_EAT_SAME_TYPE_MINIPET_COUNT)
				return;

			m_bf4EatWaterMiniPetCount++;
			break;
		case	nsMiniPetType::SpritOfWind		:
			if	(m_bf4EatWindMiniPetCount	>=	dLIMIT_EAT_SAME_TYPE_MINIPET_COUNT)
				return;

			m_bf4EatWindMiniPetCount++;
			break;
		case	nsMiniPetType::SpritOfEarth		:
			if	(m_bf4EatEarthMiniPetCount	>=	dLIMIT_EAT_SAME_TYPE_MINIPET_COUNT)
				return;

			m_bf4EatEarthMiniPetCount++;
			break;
		case	nsMiniPetType::SpritOfLight		:
			if	(m_bf4EatLightMiniPetCount	>=	dLIMIT_EAT_SAME_TYPE_MINIPET_COUNT)
				return;

			m_bf4EatLightMiniPetCount++;
			break;
		case	nsMiniPetType::SpritOfDark		:
			if	(m_bf4EatDarkMiniPetCount	>=	dLIMIT_EAT_SAME_TYPE_MINIPET_COUNT)
				return;

			m_bf4EatDarkMiniPetCount++;
			break;
	}

	sendRestraintMiniPetLog(lpMiniPet);
	lpMiniPet->reset();

	sendRemovePet(_iMiniPet,TRUE,FALSE);

	buildPower();
	sendUseItemLog(lpItem);

	lpItem->decreaseCount(1);

	sendUpdateItemData(_iItem,TRUE);

	sendEtcWork(eEW_RESTRAINT_MINIPET,iAttr);

	immediatelySendSaveDataToDBMemory();
}


void
cActor_miniPet::mixMiniPet(int _iItem,WORD *_lpwMixList)
{
	cItem	*lpItem	=	getInventoryItem(_iItem);

	if	(!lpItem)
		return;

	if	(!lpItem->isOwnSpecificEffectItem(eIE_REFLECTING_MIXER))
		return;

	int	i;

	for (i=0;i<lpItem->m_aOption[0];i++)
	{
		cMiniPet*	lpMiniPet	=	getMiniPet(_lpwMixList[i]);

		if	(!lpMiniPet	||	lpMiniPet->m_bf7Level == 0)
			return;
	}

	int	iInvalidMiniPetSlot	=	getInvalidMiniPetSlotIndex();

	for (i=0;i<lpItem->m_aOption[0];i++)
	{
		cMiniPet*	lpMiniPet	=	getMiniPet(_lpwMixList[i]);

		sendReleaseMiniPetLog(lpMiniPet,eSLT_MIX_MINIPET);
		lpMiniPet->reset();
		sendRemovePet(_lpwMixList[i],TRUE,FALSE);
	}

	buildPower();

	createMiniPet(lpItem,eUMIM_MIXED);

	sendUseItemLog(lpItem);

	lpItem->decreaseCount(1);

	sendUpdateItemData(_iItem,TRUE);
}


void
cActor_miniPet::feedItem(int _iPetIndex,int _iItemSlot)
{
	if	(_iPetIndex	<	0	||	_iPetIndex	>=	c_iMiniPetCount	||	m_aMiniPet[_iPetIndex].m_bf7Level == 0)
		return;

	if	(_iItemSlot	<	0)
		return;

	if	(_iItemSlot	>=	m_wItemSlotCount)
	{
		if	(_iItemSlot	<	dOWN_ITEM_COUNT)
			return;

		if	(_iItemSlot	>=	dOWN_ITEM_COUNT+m_bf6ExtraInventorySize)
			return;
	}


	cMiniPet	*lpPet			=	&m_aMiniPet[_iPetIndex];
	cItem		*lpItem			=	getInventoryItem(_iItemSlot);

	if	(!lpItem)
		return;

	if	(lpItem->isCanNotFeedToPetItem())
		return;
	if	(lpItem->isExpirationData())		// 기간이 있는 아이템은 먹일수 없다.
		return;
	if	(lpPet->isExpired())
		return;
	
	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem)
		return;

	int		iFeedExp=	lpItem->getExpForMiniPet();

	if	(iFeedExp	<=	0)
		return;

	if(lpItem->isExistSpecificEffect(eIE_MINIPET_FEED))
	{
		cACTOR*	lpActor	=	(cACTOR*)this;

		if(!lpActor)
			return;

		if(lpActor->m_iLevel < lpPet->m_bf7Level)
			return;
		
		int iExp	= lpPet->getNeedExp( lpPet->m_bf7Level,lpActor->m_iLevel);
		iExp		= (lpPet->m_bf16Exp + iFeedExp) - iExp;
		if(iExp > 0xffff)	
			return;
	}
	int		iFeedType	=	lpItem->getMiniPetFeedType();
	UINT	uiOldPetJob	=	lpPet->m_bf5Type;

	int		iFeedItemType=	0;

	if	(lpItem->isRareItem())
		iFeedItemType	=	1;
	else
	if	(lpItem->isMiniPetFeed())
		iFeedItemType	=	2;

	int	iOldRate	=	lpPet->getEvolutionRate(lpPet->getStyle());

	iFeedExp		*=	m_wCorrectMiniPetGainExpByFeedItem;

	if	(!lpPet->increaseExp(iFeedExp,iFeedType,m_level.m_sValue))
		return;

	BOOL	bIsEvolution=	FALSE;

	if	(uiOldPetJob!=	lpPet->m_bf5Type)
		bIsEvolution=	TRUE;
	
	sendMiniPetInfo(_iPetIndex,eUMIM_FEEDING,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2,iFeedExp,bIsEvolution,iFeedItemType);

	cItem	logItem;

	logItem.copy(lpItem);
	lpItem->decreaseCount(1);

	sendFeedItemMiniPetLog(&logItem,lpPet,_iPetIndex);

	sendUpdateItemData(_iItemSlot,TRUE);

	if	(lpPet->getStyle()	!=	nsMiniPetStyle::Deault)
		if	(m_bf5ActiveMiniPet	==	(UINT)_iPetIndex	||	(m_bf5ActiveMiniPet2	==	(UINT)_iPetIndex	&&	m_wIsActiveReversalFrame))
		{
			int		iCurrentRate	=	lpPet->getEvolutionRate(lpPet->getStyle());
			BOOL	bRequireRebuild	=	FALSE;

			for	(int i=	0;i<3;i++)
			{
				int	iValue	=	c_aiMiniPetEvoultionBonusRate[i];

				if	(iOldRate	<	iValue	&&	iCurrentRate	>=	iValue)
				{
					bRequireRebuild	=	TRUE;

					break;

				}
			}

			if	(bRequireRebuild)
				buildPower();
		}

	int	iAddSkill	=	lpPet->addSpareSkill();

	if	(iAddSkill	!=	-1)
		sendEtcWork(eEW_ADD_MINIPET_SPARE_SKILL,iAddSkill);
}

int
cActor_miniPet::getMiniPetMagicDamage(cAbility *_lpAbility,int _iPetLevel,int _iValue,int _iValuePerLevel,int _iValueRange,int _iValueRangePerLevel ,cMiniPet* _lpMiniPet , int _iUserLevel)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();	

	int		iValue;
	int		iSkillLevel		=	_lpAbility->m_wLevel;
	float	fPet_slash_2	=	(float)_iPetLevel/2;
	float	fSkill_slash_2	=	(float)iSkillLevel/2;
	int		iDamageExpressionType	=	lpSkill->m_bf4PetSkillDamageExpressionType;

	//JBC	미니펫 3차 강화(각성의 씨앗 열매)를 위한 작업들...09-01-08
	//기존 데미지 수식의(고정값) 강화
	if(_lpMiniPet->m_bf1MinipetAwaken100)	
		_iValue	=	2 * lpSkill->m_wAwakenSkillBonusDamageForOrigin;
	else if(_lpMiniPet->m_bf1MinipetAwaken50)
		_iValue	=  lpSkill->m_wAwakenSkillBonusDamageForOrigin;

	//기존 데미지 수식의 레벨당 앞에 붙는 값을 변화
	if(_lpMiniPet->m_bf1MinipetAwaken100)	
		_iValuePerLevel	=	2 * lpSkill->m_wAwakenSkillBonusDamagePerLevelForOrigin;
	else if(_lpMiniPet->m_bf1MinipetAwaken50)
		_iValuePerLevel	=  lpSkill->m_wAwakenSkillBonusDamagePerLevelForOrigin;

	//기존 데미지 수식의 범위 값의 변화를 변화.-_-;
	if(_lpMiniPet->m_bf1MinipetAwaken100)	
		_iValueRange	=	2 * lpSkill->m_wAwakenSkillBonusDamageValueRangeForOrigin;
	else if(_lpMiniPet->m_bf1MinipetAwaken50)
		_iValueRange	=  lpSkill->m_wAwakenSkillBonusDamageValueRangeForOrigin;

	//기존 데미지 수식의 레벨당 범위 값의 변화를 변화.-_-;
	if(_lpMiniPet->m_bf1MinipetAwaken100)	
		_iValueRangePerLevel	=	2 * lpSkill->m_wAwakenSkillBonusDamageValueRangePerLevelForOrigin;
	else if(_lpMiniPet->m_bf1MinipetAwaken50)
		_iValueRangePerLevel	=  lpSkill->m_wAwakenSkillBonusDamageValueRangePerLevelForOrigin;

	if(_lpMiniPet->m_bf1MinipetAwaken100 || _lpMiniPet->m_bf1MinipetAwaken50)
		iDamageExpressionType = lpSkill->m_bf4PetSkillDamageExpressionTypeForAwaken;

	switch(iDamageExpressionType)
	{
		case	0	:
		{
			int	iValueRange	=	_iValueRange+_iValueRangePerLevel*iSkillLevel;

			iValue	=	_iValue+iSkillLevel*_iValuePerLevel+random(iValueRange*2)-iValueRange;
			break;
		}
		case	1	:
		{
	//		불 : (10+펫Lv/2±5) * 스킬Lv/2
			int	iValueRange	=	_iValueRange+_iValueRangePerLevel*iSkillLevel;

			iValue	=	(int)(_iValue+(fPet_slash_2)*_iValuePerLevel+random(iValueRange*2)-iValueRange);
			iValue	=	(int)(iValue*fSkill_slash_2);
			break;
		}
		case	2	:
		{
	//		바람 : (2+펫Lv/2±펫Lv/4) * 스킬Lv/2

			int	iValueRange	=	_iValueRange+_iValueRangePerLevel*_iPetLevel/4;

			iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+random(iValueRange*2)-iValueRange);
			iValue	=	(int)(iValue*fSkill_slash_2);
			break;
		}
		case	3	:
		{
	//	불 : (50+25*펫Lv/2±25)*스킬Lv
			int	iValueRange	=	_iValueRange+_iValueRangePerLevel*iSkillLevel;

			iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+random(iValueRange*2)-iValueRange);
			iValue	*=	iSkillLevel;
			break;
		}
		case	4	:
		{
	//	바람 : (50+25*펫Lv/2±12*펫Lv/2)*스킬Lv
			int	iValueRange	=	(int)(_iValueRange+_iValueRangePerLevel*fPet_slash_2);

			iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+random(iValueRange*2)-iValueRange);
			iValue	*=	iSkillLevel;
			break;
		}
		case	5	:
		{
	//	"불:(5+1*펫Lv/2±3) 연속 공격: 5Lv 번
			int	iValueRange	=	_iValueRange+_iValueRangePerLevel*iSkillLevel;

			iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+random(iValueRange*2)-iValueRange);
			break;
		}
		case	6	:
		{
	//		빛 : (2+펫Lv/2±펫Lv/2) * 스킬Lv/2
			int	iValueRange	=	(int)(_iValueRange+_iValueRangePerLevel*fPet_slash_2);

			iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+random(iValueRange*2)-iValueRange);
			iValue	=	(int)(iValue*fSkill_slash_2);
			break;
		}
		case	7	:
		{
	//	물:(3+0.9펫Lv/2±2)*2Lv
			int	iValueRange	=	_iValueRange+_iValueRangePerLevel;

			iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+random(iValueRange*2)-iValueRange);
			iValue	*=	iSkillLevel*2;
			break;
		}
		case	8	:
		{
	//	바람:(3+1펫Lv/2±0.7펫Lv/2)
			int	iValueRange	=	(int)(_iValueRange+_iValueRangePerLevel*fPet_slash_2);

			iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+random(iValueRange*2)-iValueRange);
			break;
		}
		case	9	:
		{
			int	iValueRange	=	(int)(_iValueRange+_iValueRangePerLevel*fPet_slash_2);

			iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+random(iValueRange*2)-iValueRange);
			iValue	*=	iSkillLevel*2;
			break;
		}
		//보통의 파이어 볼트 락바운딩 펫 라이트 빔과 같은 기술에 적용되는 공식.
		case	10	:	
		{
			int	iValueRange	=	_iValueRange+_iValueRangePerLevel*_iPetLevel/2;

			iValue	=	_iValue+_iPetLevel/2*_iValuePerLevel+random(iValueRange*2)-iValueRange;

			if (_lpMiniPet == NULL)
				return	iValue;

			break;
		}
		case	11	:
		{
			int iValueRange	= _iValueRange + _iValueRangePerLevel * _iPetLevel/2;

			iValue = _iValue + _iValuePerLevel * _iPetLevel/2 + random(2 * iValueRange) - iValueRange;
			//iValue = iValue * iSkillLevel / 2;

			break;
		}
		case	12	:
		{
			int iValueRange	= _iValueRange + _iValueRangePerLevel * _iPetLevel/2;
			
			iValue = _iValue + _iValuePerLevel * _iPetLevel/2 + random(2 * iValueRange) - iValueRange;
			iValue = iValue * iSkillLevel;

			break;
		}
		case	13	:
		{
			int iValueRange	= _iValueRange + _iValueRangePerLevel * _iPetLevel/2;
			
			iValue = _iValue + _iValuePerLevel * _iPetLevel/2 + random(2 * iValueRange) - iValueRange;

			break;
		}
		case	14	:
		{
			int iValueRange	= _iValueRange + _iValueRangePerLevel * _iPetLevel/2;
				
			iValue = _iValue + _iValuePerLevel * _iPetLevel/2 + random(2 * iValueRange) - iValueRange;
			iValue = iValue * 2 * iSkillLevel;
			
			break;
		}
		
		case	15	:
		{
			int iValueRange	= _iValueRange + _iValueRangePerLevel * _iPetLevel/4;
			
			iValue = _iValue + _iValuePerLevel * _iPetLevel/2 + random(2 * iValueRange) - iValueRange;
			iValue = iValue * iSkillLevel / 2;
			break;
		}
	}

	if(_lpMiniPet->m_bf1MinipetAwaken100)
		iValue *= (2 * lpSkill->m_wAwakenSkillBonusDamage + 2 * lpSkill->m_wAwakenSkillBonusDamagePerLevel * _iUserLevel)/100;
	else if(_lpMiniPet->m_bf1MinipetAwaken50)
		iValue *= (lpSkill->m_wAwakenSkillBonusDamage + lpSkill->m_wAwakenSkillBonusDamagePerLevel * _iUserLevel)/100;

	return	iValue;
}


void
cActor_miniPet::operateMiniPetChargeSkill()
{
	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iIndex	=	m_bf5ActiveMiniPet;

		if	(i	==	1)
			iIndex	=	m_bf5ActiveMiniPet2;

		cActiveMiniPetInfo	*lpInfo	=	&m_aActiveMiniPetInfo[i];

		if	(lpInfo->m_wRemainFireCountOfMiniPetChargeSkill	==	0)
			continue;

		if	(!isAvailMiniPet(i))
		{
			lpInfo->m_wRemainFireCountOfMiniPetChargeSkill	=	0;
			return;
		}

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iIndex];

		if	((lpInfo->m_wRemainFireCountOfMiniPetChargeSkill	%	(dSYNC_FPS/8)) == 0)
		{
			cACTOR		*lpTarget	=	m_lpField->getTestedActor(lpInfo->m_wMiniPetChargeSkillTarget);
			cAbility	ability;

			ability.set(lpInfo->m_firedMiniPetChargeAbility.m_bf11Skill,lpInfo->m_firedMiniPetChargeAbility.m_bf4Level);

			if	(ability.getSkill()->m_bf1IsNormalMiniPetSkill)
			{
				ability.m_wLevel	+=	m_bIncreaseMiniPetBasicSkillLevel;
				ability.m_wLevel	+=	m_abCorrectMiniPetBasicSkillLevel[i];
			}

			ability.m_wLevel	+=	m_bIncreaseMiniPetSkillLevel;
			ability.m_wLevel	+=	m_abCorrectMiniPetSkillLevel[i];

			if	(!lpTarget)
			{
				lpInfo->m_wRemainFireCountOfMiniPetChargeSkill	=	0;
				continue;
			}

			if	(m_lpField->isMissileBlockedLine(m_pos.x,m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
				continue;

			if	(!lpTarget->isInAttackRange(m_pos.x,m_pos.y,ability.getAttackRange(NULL,0)))
				continue;

			CHitInfo	hitInfo;

			hitInfo.reset(TRUE);

			lpTarget->iMiniPet()->getMagicDamageByMiniPet(lpMiniPet,this,&ability,&hitInfo,i,m_iLevel);

			m_lpField->sendMachineMissile(m_wSerialInField,lpTarget->m_wSerialInField,ability.m_wSkill,&hitInfo,i);
			lpTarget->reduceHP(hitInfo.getDamage(),this);
			lpTarget->operateRevenge(this,&ability);

			if	(lpInfo->m_wCountOfMiniPetChargeSkill	==	lpInfo->m_wRemainFireCountOfMiniPetChargeSkill/(dSYNC_FPS/8))
			{
				m_lpField->addSendEtcWork(m_wSerialInField,eEW_MINIPET_ACTION,m_wSerialInField,lpTarget->isDeath(),lpTarget->isBossMonster(),i);
				lpTarget->applySkillExtraEffects(this,&ability);
			}
		}

		lpInfo->m_wRemainFireCountOfMiniPetChargeSkill--;
	}
}


void
cActor_miniPet::castContinuousShoot(cMiniPet *_lpMiniPet,cMiniPetAbilityInfo *_lpAbility,cACTOR *_lpTarget,int _iPetIndex)
{
	int	iPetOrder	=	0;

	if	(_iPetIndex	==	m_bf5ActiveMiniPet2	&&	m_wIsActiveReversalFrame)
		iPetOrder	=	1;

	cAbility	ability;
	CSkill		*lpSkill	=	&g_aSkill[_lpAbility->m_bf11Skill];

	ability.set(_lpAbility->m_bf11Skill,_lpAbility->m_bf4Level);

	if	(ability.getSkill()->m_bf1IsNormalMiniPetSkill)
	{
		ability.m_wLevel	+=	m_bIncreaseMiniPetBasicSkillLevel;
		ability.m_wLevel	+=	m_abCorrectMiniPetBasicSkillLevel[iPetOrder];
	}

	ability.m_wLevel	+=	m_bIncreaseMiniPetSkillLevel;
	ability.m_wLevel	+=	m_abCorrectMiniPetSkillLevel[iPetOrder];

	if	(!_lpTarget	||	isOwnForce(_lpTarget))
	{
		sendRegistServerMessage(eRSM_ITS_USE_TO_ENEMY_SKILL,eSM_WARNING);
		return;
	}

	if	(!_lpTarget->isInAttackRange(m_pos.x,m_pos.y,ability.getAttackRange(NULL,0)))
	{
		sendRegistServerMessage(eRSM_TOO_FAR_TO_USE_SKILL,eSM_WARNING);
		return;
	}

	if	(m_lpField->isMissileBlockedLine(m_pos.x,m_pos.y,_lpTarget->m_pos.x,_lpTarget->m_pos.y))
	{
		sendRegistServerMessage(eRSM_MISSILE_BLOCKED_LINE,eSM_WARNING);
		return;
	}
	
	int		iShotCount	=	ability.getContinuousShotCount(FALSE);
	
	cActiveMiniPetInfo	*lpInfo	=	&m_aActiveMiniPetInfo[iPetOrder];

	lpInfo->m_wMiniPetChargeSkillTarget				=	_lpTarget->m_wSerialInField;
	lpInfo->m_wRemainFireCountOfMiniPetChargeSkill	=	iShotCount*dSYNC_FPS/8;
	lpInfo->m_wCountOfMiniPetChargeSkill			=	iShotCount;
	lpInfo->m_wChargeCountForMiniPet				=	0;
	lpInfo->m_wIsCharged	=	FALSE;

	sendEtcWork(eEW_READY_TO_FIRE_CHARGE_MINIPET_SKILL,FALSE,iPetOrder);

	memcpy(&lpInfo->m_firedMiniPetChargeAbility,_lpAbility,sizeof(cMiniPetAbilityInfo));
}



BOOL
cActor_miniPet::castImpactSkill(cMiniPet *_lpMiniPet,cMiniPetAbilityInfo *_lpAbility,cACTOR *_lpTarget,int _iPetIndex , int _iUserLevel)
{
	int	iPetOrder	=	0;

	if	(_iPetIndex	==	m_bf5ActiveMiniPet2	&&	m_wIsActiveReversalFrame)
		iPetOrder	=	1;

	cAbility	ability;
	CSkill		*lpSkill	=	&g_aSkill[_lpAbility->m_bf11Skill];

	ability.set(_lpAbility->m_bf11Skill,_lpAbility->m_bf4Level);

	if	(ability.getSkill()->m_bf1IsNormalMiniPetSkill)
	{
		ability.m_wLevel	+=	m_bIncreaseMiniPetBasicSkillLevel;
		ability.m_wLevel	+=	m_abCorrectMiniPetBasicSkillLevel[iPetOrder];
	}
	ability.m_wLevel	+=	m_bIncreaseMiniPetSkillLevel;
	ability.m_wLevel	+=	m_abCorrectMiniPetSkillLevel[iPetOrder];

	int	iLevel			=	_lpAbility->m_bf4Level;
	
	int	iActiveChance	=	lpSkill->m_sActiveChancePerLevel*iLevel;
	int	iRange			=	ability.getHitRange();

	if	(lpSkill->m_bf1IsXMiniPetLevel)
		iActiveChance	*=	_lpMiniPet->m_bf7Level;

	if(!lpSkill->m_bf1IsNormalMiniPetSkill)
	{
		//JBC	레벨당 증가하는 발동 확률 제어.	09-01-06
		if(_lpMiniPet->m_bf1MinipetAwaken100)				
			iActiveChance	+=	lpSkill->m_wAwakenSkillBonusActiveChancePerLevel*iLevel * 2;
		else if(_lpMiniPet->m_bf1MinipetAwaken50)
			iActiveChance	+=	lpSkill->m_wAwakenSkillBonusActiveChancePerLevel*iLevel;
		
		iActiveChance		+=	lpSkill->m_sActiveChance;
		
		//JBC	고정적으로 증가하는 발동 확률 제어.	09-01-06
		if(_lpMiniPet->m_bf1MinipetAwaken100)				
			iActiveChance	+=	lpSkill->m_wAwakenSkillBonusActiveChance * 2;
		else if(_lpMiniPet->m_bf1MinipetAwaken50)
			iActiveChance	+=	lpSkill->m_wAwakenSkillBonusActiveChance;

	}

	cAbility	abilityPowerUp;

	{
		cMiniPetAbilityInfo	*lpPowerUpAbility	=	_lpMiniPet->getPowerUpSkill(ability.m_wSkill);

		if	(lpPowerUpAbility)
		{
			abilityPowerUp.set(lpPowerUpAbility->m_bf11Skill,lpPowerUpAbility->m_bf4Level);

			if	(abilityPowerUp.getSkill()->m_bf1IsNormalMiniPetSkill)
			{
				abilityPowerUp.m_wLevel	+=	m_bIncreaseMiniPetBasicSkillLevel;
				abilityPowerUp.m_wLevel	+=	m_abCorrectMiniPetBasicSkillLevel[iPetOrder];
			}

			abilityPowerUp.m_wLevel	+=	m_bIncreaseMiniPetSkillLevel;
			abilityPowerUp.m_wLevel	+=	m_abCorrectMiniPetSkillLevel[iPetOrder];

			CSkill		*lpPowerUpSkill	=	abilityPowerUp.getSkill();

			if	(lpPowerUpSkill)
			{
				iRange			+=	abilityPowerUp.getHitRange();
				iActiveChance	+=	(lpPowerUpSkill->m_sActiveChance+lpPowerUpSkill->m_sActiveChancePerLevel*abilityPowerUp.m_wLevel);

				if(g_iSeasonVariable >= dSEASON_VARIABLE_PARTY_UNION)
				{
					//JBC	레벨당 증가하는 발동 확률 제어.	09-01-06
					if(_lpMiniPet->m_bf1MinipetAwaken100)
						iActiveChance	+=	lpPowerUpSkill->m_wAwakenSkillBonusActiveChanceForBoJoSKill + lpPowerUpSkill->m_wAwakenSkillBonusActiveChanceForBoJoSKillPerLevel* 2 *_lpMiniPet->m_bf7Level;
					else if(_lpMiniPet->m_bf1MinipetAwaken50)
						iActiveChance	+=	lpPowerUpSkill->m_wAwakenSkillBonusActiveChanceForBoJoSKill + lpPowerUpSkill->m_wAwakenSkillBonusActiveChanceForBoJoSKillPerLevel*_lpMiniPet->m_bf7Level;

					if(_lpMiniPet->m_bf1MinipetAwaken100)
						iRange	+=	(lpPowerUpSkill->m_wAwakenSkillBonusRange + lpPowerUpSkill->m_wAwakenSkillBonusRangePerLevel* 2 *_lpMiniPet->m_bf7Level)/100;
					else if(_lpMiniPet->m_bf1MinipetAwaken50)
						iRange	+=	(lpPowerUpSkill->m_wAwakenSkillBonusRange + lpPowerUpSkill->m_wAwakenSkillBonusRangePerLevel*_lpMiniPet->m_bf7Level)/100;
				}
			}
		}
	}

	iActiveChance		+=	_lpMiniPet->getVSBossCorrectActiveChance(_lpTarget);
	iActiveChance		+=	m_abCorrectMiniPetSkillActiveChance[iPetOrder];
	iActiveChance		+=	iActiveChance*m_bIncreaseMiniPetSkillActiveChance/100;
	if	(g_bIsChansTestServer	&&	iActiveChance	<	10000)	iActiveChance	=	10000;
	if	(random(10000)	>=	iActiveChance)
		return	FALSE;

	int		iFireDamage	=0,iWaterDamage=0,iWindDamage=0,iEarthDamage=0,iLightDamage=0,iDarkDamage=0,iPhysicalDamage=0;

	int		iCorrectDamage	=	m_abCorrectMiniPetSkillDamage[iPetOrder]+m_bIncreaseMiniPetMagicDamage;

	if	(lpSkill->isFireDamageSkill())
	{
		iFireDamage		=	getMiniPetMagicDamage(&ability,_lpMiniPet->m_bf7Level,lpSkill->m_sFireDamage,lpSkill->m_sFireDamagePerLevel,lpSkill->m_sFireDamageRange,lpSkill->m_sFireDamageRangePerLevel , _lpMiniPet , _iUserLevel);
		iFireDamage		+=	iFireDamage*abilityPowerUp.getFireDamagePercentage()/100;
		iFireDamage		+=	iFireDamage*(m_bIncreaseMiniPetFireDamage+iCorrectDamage)/100;
	}

	if	(lpSkill->isWindDamageSkill())
	{
		iWindDamage		=	getMiniPetMagicDamage(&ability,_lpMiniPet->m_bf7Level,lpSkill->m_sWindDamage,lpSkill->m_sWindDamagePerLevel,lpSkill->m_sWindDamageRange,lpSkill->m_sWindDamageRangePerLevel, _lpMiniPet , _iUserLevel);
		iWindDamage		+=	iWindDamage*abilityPowerUp.getWindDamagePercentage()/100;
		iWindDamage		+=	iWindDamage*(m_bIncreaseMiniPetWindDamage+iCorrectDamage)/100;
	}
	if	(lpSkill->isEarthDamageSkill())
	{
		iEarthDamage	=	getMiniPetMagicDamage(&ability,_lpMiniPet->m_bf7Level,lpSkill->m_sEarthDamage,lpSkill->m_sEarthDamagePerLevel,lpSkill->m_sEarthDamageRange,lpSkill->m_sEarthDamageRangePerLevel, _lpMiniPet , _iUserLevel);
		iEarthDamage	+=	iEarthDamage*abilityPowerUp.getEarthDamagePercentage()/100;
		iEarthDamage	+=	iEarthDamage*(m_bIncreaseMiniPetEarthDamage+iCorrectDamage)/100;
	}
	if	(lpSkill->isLightDamageSkill())
	{
		iLightDamage	=	getMiniPetMagicDamage(&ability,_lpMiniPet->m_bf7Level,lpSkill->m_sLightDamage,lpSkill->m_sLightDamagePerLevel,lpSkill->m_sLightDamageRange,lpSkill->m_sLightDamageRangePerLevel, _lpMiniPet , _iUserLevel);
		iLightDamage	+=	iLightDamage*abilityPowerUp.getLightDamagePercentage()/100;
		iLightDamage	+=	iLightDamage*(m_bIncreaseMiniPetLightDamage+iCorrectDamage)/100;
	}
	if	(lpSkill->isDarkDamageSkill())
	{
		iDarkDamage		=	getMiniPetMagicDamage(&ability,_lpMiniPet->m_bf7Level,lpSkill->m_sDarkDamage,lpSkill->m_sDarkDamagePerLevel,lpSkill->m_sDarkDamageRange,lpSkill->m_sDarkDamageRangePerLevel, _lpMiniPet , _iUserLevel);
		iDarkDamage		+=	iDarkDamage*abilityPowerUp.getDarkDamagePercentage()/100;
		iDarkDamage		+=	iDarkDamage*(m_bIncreaseMiniPetDarkDamage+iCorrectDamage)/100;
	}
	if (lpSkill->isWaterDamageSkill())
	{
		iWaterDamage	=	getMiniPetMagicDamage(&ability,_lpMiniPet->m_bf7Level,lpSkill->m_sWaterDamage,lpSkill->m_sWaterDamagePerLevel,lpSkill->m_sWaterDamageRange,lpSkill->m_sWaterDamageRangePerLevel, _lpMiniPet , _iUserLevel);
		iWaterDamage	+=	iWaterDamage*abilityPowerUp.getWaterDamagePercentage()/100;
		iWaterDamage	+=	iWaterDamage*(m_bIncreaseMiniPetWaterDamage+iCorrectDamage)/100;
	}

	int	iX	=	m_pos.x,iY	=	m_pos.y;

	if	(lpSkill->isQuickCastSkill())
		_lpTarget	=	NULL;

	if	(_lpTarget)
	{
		iX	=	_lpTarget->m_pos.x;
		iY	=	_lpTarget->m_pos.y;
	}

	if	(m_lpField->isMissileBlockedLine(m_pos.x,m_pos.y,iX,iY))
		return	FALSE;
	
	int	iDeathCount	=	0;

	if	(m_lpField->strikeWideAreaMagicDamage(this,iX,iY,TRUE,&ability,iRange,iFireDamage,iWaterDamage,iWindDamage,iEarthDamage,iLightDamage,iDarkDamage,-1,&iDeathCount,TRUE))
	{
		m_lpField->addSendOccurEffectBySkill(iX,iY,ability.m_wSkill,eSE_EXPLOSION);
		m_lpField->addSendEtcWork(m_wSerialInField,eEW_MINIPET_ACTION,m_wSerialInField,iDeathCount,0,iPetOrder);
		return	TRUE;
	}

	return	FALSE;
}
//	타격 입을때 발동하는 스킬 처리


void
cActor_miniPet::operateMiniPetDamagedTriggerSkill()
{
	if	(isIdlePlayer())
		return;

	for	(int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		if	(!isAvailMiniPet(i))
			continue;

		int	iIndex	=	m_bf5ActiveMiniPet;

		if	(i	==	1)
			iIndex	=	m_bf5ActiveMiniPet2;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iIndex];
		cAbility	ability;

		for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
		{
			cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[iSkill];

			if	(lpAbility->m_bf4Level	==	0)
				continue;
			if	(lpAbility->m_bf1IsOFF)
				continue;
			CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

 			if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::ActionWhenDamaged)
 				continue;

			castImpactSkill(lpMiniPet,lpAbility,m_lpTarget,i , m_iLevel);
		}
	}
}


void
cActor_miniPet::updateChargeCount(BOOL _bIsUpdate)
{
	int	iPetOrder	=	random(m_wIsActiveReversalFrame+1);

	if	(m_wIsActiveReversalFrame	==	FALSE)
		m_aActiveMiniPetInfo[1].m_wIsCharged	=	FALSE;

	if	(!isAvailMiniPet(iPetOrder))
		return;

	int	iIndex	=	m_bf5ActiveMiniPet;

	if	(iPetOrder	==	1)
		iIndex	=	m_bf5ActiveMiniPet2;

	cActiveMiniPetInfo	*lpInfo		=	&m_aActiveMiniPetInfo[iPetOrder];
	cMiniPet			*lpMiniPet	=	&m_aMiniPet[iIndex];
	cAbility			ability;

	if	(lpInfo->m_wChargeCountForMiniPet	<	0xffff	&&	_bIsUpdate	==	FALSE)
		lpInfo->m_wChargeCountForMiniPet++;

	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[iSkill];

		if	(lpAbility->m_bf4Level	==	0)
			continue;

		if	(lpAbility->m_bf1IsOFF)
			continue;

		CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

		if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::ChargeAndFire)
			continue;

		int	iChargeCount	=	lpSkill->m_aValue[0];
		iChargeCount		-=	lpMiniPet->getDecreaseChargeCount(this, iPetOrder);

		if(lpMiniPet->m_bf1MinipetAwaken100)
			iChargeCount	-=	lpSkill->m_wAwakenSkillBonusActiveChance*2;	//무기질에서는 발동확률이 차지카운트 줄이는 수이다.
		else if(lpMiniPet->m_bf1MinipetAwaken50)
			iChargeCount	-=	lpSkill->m_wAwakenSkillBonusActiveChance;


		if	(lpInfo->m_wChargeCountForMiniPet	>=	iChargeCount)
		{
			lpInfo->m_wIsCharged	=	TRUE;

			if	(lpInfo->m_wChargeCountForMiniPet	==	iChargeCount	||	_bIsUpdate)
				sendEtcWork(eEW_READY_TO_FIRE_CHARGE_MINIPET_SKILL,TRUE,iPetOrder);
		}

		break;
	}
}


void
cActor_miniPet::operateMiniPetSkill()//	미니펫 스킬 처리
{
	if	(m_wIsActiveReversalFrame	==	FALSE)
		reset2();

	if	(isIdlePlayer())
		return;

	for	(int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		if	(!isAvailMiniPet(i))
			continue;

		int	iIndex	=	m_bf5ActiveMiniPet;

		if	(i	==	1)
			iIndex	=	m_bf5ActiveMiniPet2;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iIndex];
		cAbility	ability;

	//	m_wUnderBattleTime	=	60000;
	//	int	iDestLevel	=	99;lpMiniPet->m_bf7SkillPoint	=	iDestLevel-lpMiniPet->m_bf7Level;lpMiniPet->m_bf7Level	=	iDestLevel;activateMiniPet(m_bf5ActiveMiniPet);

		for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
		{
			cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[iSkill];

			if	(lpAbility->m_bf4Level	==	0)
				continue;

			if	(lpAbility->m_bf1IsOFF)
				continue;

			CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];
			
			int	iLevel			=	lpAbility->m_bf4Level;
			int	iActivePeriod	=	max(lpSkill->m_sOccurActionPeriod/100,1);

			if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			{
				iLevel			+=	m_bIncreaseMiniPetBasicSkillLevel;
				iLevel			+=	m_abCorrectMiniPetBasicSkillLevel[i];
			}
			iLevel				+=	m_bIncreaseMiniPetSkillLevel;
			iLevel				+=	m_abCorrectMiniPetSkillLevel[i];

			if	(g_iCounterBySecond	%	iActivePeriod	!=	0)
				continue;

			int	iActiveChance	=	lpSkill->m_sActiveChancePerLevel*iLevel;

			iActiveChance		+=	lpMiniPet->getVSBossCorrectActiveChance(m_lpTarget);

			{
				cMiniPetAbilityInfo	*lpPowerUpSkill	=	lpMiniPet->getPowerUpSkill(lpSkill->m_wSerial);

				if	(lpPowerUpSkill)
				{
					CSkill		*lpPowerUp	=	&g_aSkill[lpPowerUpSkill->m_bf11Skill];

					iActiveChance	+=	lpPowerUp->m_sActiveChance;

					int	iLevel		=	lpPowerUpSkill->m_bf4Level;

					if	(lpSkill->m_bf1IsNormalMiniPetSkill)
					{
						iLevel		+=	m_bIncreaseMiniPetBasicSkillLevel;
						iLevel		+=	m_abCorrectMiniPetBasicSkillLevel[i];
					}
					iLevel			+=	m_bIncreaseMiniPetSkillLevel;
					iLevel			+=	m_abCorrectMiniPetSkillLevel[i];

					iActiveChance	+=	lpPowerUp->m_sActiveChancePerLevel*iLevel;

					if(lpMiniPet->m_bf1MinipetAwaken100)
						iActiveChance	+= 	lpPowerUp->m_wAwakenSkillBonusActiveChanceForBoJoSKill + lpPowerUp->m_wAwakenSkillBonusActiveChanceForBoJoSKillPerLevel * lpMiniPet->m_bf7Level * 2;
					else if(lpMiniPet->m_bf1MinipetAwaken50)
						iActiveChance	+= 	lpPowerUp->m_wAwakenSkillBonusActiveChanceForBoJoSKill + lpPowerUp->m_wAwakenSkillBonusActiveChanceForBoJoSKillPerLevel * lpMiniPet->m_bf7Level;
				}
			}

			if	(lpSkill->m_bf1IsXMiniPetLevel)
				iActiveChance	*=	lpMiniPet->m_bf7Level;

			iActiveChance		+=	lpSkill->m_sActiveChance;

			iActiveChance		+=	m_abCorrectMiniPetSkillActiveChance[i];
			iActiveChance		+=	iActiveChance*m_bIncreaseMiniPetSkillActiveChance/100;
 			if(g_bIsMinipetActiveChaceTest)
 				iActiveChance	=	10000;

			if	(random(10000)	>=	iActiveChance)
				continue;

//JBC	미니펫 관련 버그 스킬증가가 2번씩 먹고 있어요...ㅠ.ㅠ;	08-09-18
// 			if	(lpSkill->m_bf1IsNormalMiniPetSkill)
// 			{
// 				iLevel			+=	m_bIncreaseMiniPetBasicSkillLevel;
// 				iLevel			+=	m_abCorrectMiniPetBasicSkillLevel[i];
// 			}
// 			iLevel			+=	m_bIncreaseMiniPetSkillLevel;
// 			iLevel			+=	m_abCorrectMiniPetSkillLevel[i];

			ability.set(lpAbility->m_bf11Skill,iLevel);

			CHitInfo	hitInfo;

			hitInfo.reset(TRUE);

			switch(lpSkill->m_bf5MiniPetSkillType)
			{
				case	nsMiniPetST::AttackUnderBattleEnemy	:
				{
					if	(m_lpTarget	==	NULL)
						break;

					if	(!m_lpTarget->isInAttackRange(m_pos.x,m_pos.y,ability.getAttackRange(NULL,0)))
						break;

					m_lpTarget->iMiniPet()->getMagicDamageByMiniPet(lpMiniPet,this,&ability,&hitInfo,i,m_iLevel);

					m_lpField->sendMachineMissile(m_wSerialInField,m_lpTarget->m_wSerialInField,ability.m_wSkill,&hitInfo,i);
					m_lpTarget->reduceHP(hitInfo.getDamage(),this);
					m_lpField->addSendEtcWork(m_wSerialInField,eEW_MINIPET_ACTION,m_wSerialInField,m_lpTarget->isDeath(),m_lpTarget->isBossMonster(),i);
					m_lpTarget->applySkillExtraEffects(this,&ability);
					break;
				}

				case	nsMiniPetST::AidToUnderBattleLord	:
				{
					if	(m_wUnderBattleTime	==	0)
						break;

					if	(applySkillExtraEffects(this,&ability))
					{
						addSendSkillEffect(&ability,eSE_CAST);
						m_lpField->addSendEtcWork(m_wSerialInField,eEW_MINIPET_ACTION,m_wSerialInField,0,0,i);
					}
					break;
				}

				case	nsMiniPetST::AidToLord	:
				{
					if	(applySkillExtraEffects(this,&ability))
					{
						addSendSkillEffect(&ability,eSE_CAST);
						m_lpField->addSendEtcWork(m_wSerialInField,eEW_MINIPET_ACTION,m_wSerialInField,0,0,i);
					}
					break;
				}
			}
		}
	}
}


void
cActor_miniPet::applyMiniPetBuffSkillEffects(cAbility *_lpAbility)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(!lpSkill)
		return;

	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		CSkillExtraEffect		*lpExtraEffect	=	&lpSkill->m_aExtraEffect[i];
		CSkillExtraEffectInfo	skillEE;
		cACTOR*					cAcotor_load = this;

		if	(!_lpAbility->getExtraEffect(i,&skillEE))
			break;

		int	iValue		=	(skillEE.m_aiValue[0]+skillEE.m_aiValue[1]*_lpAbility->m_wLevel)/100;
		int iValueForIncPowerLevel	=	0;

		//JBC	미니펫 관련 스텟 증가 수식 변경.	08-11-03
		switch (g_iSeasonVariable)
		{
		case	0:			//0시즌	결국 기존 공식이라는 뜻임...
			iValueForIncPowerLevel		=	0;
			break;
		default:			// 미니펫 2차 강화(시즌 1) 참조.
			iValueForIncPowerLevel		=	cAcotor_load->m_iLevel / 10 + (skillEE.m_aiValue[2])/100;
			break;
			
		}

		int iTempValue3 = skillEE.m_aiValue[3];
		//힐링양및 차징양 수식 변경.			//0으로 나누면 안된다...
		if(skillEE.m_aiValue[3] == 0)		
			iTempValue3 = 1;

		switch(skillEE.m_wEffect)
		{
			case	eSKILL_EE_CORRECT_ITEM_EQUIP_LEVEL	:
				m_wLimitEquipmentLevelOverPoint	+=	iValue;
				break;

			case	eSKILL_EE_INCREASE_SKILL_LEVEL		:
				m_correctAllSkillLevel.addValue(iValue);
				break;

			case	eSKILL_EE_INCREASE_DEFENSE_POWER_PERCENTAGE	:
				m_defensePower.m_sCorrectPercentageValueByPrefix	+=	iValue;
				break;

			case	eSKILL_EE_INCLINE_MOVE_SPEED				:
				m_moveSpeed.addValue(iValue);
				break;

			case	eSKILL_EE_INCLINE_MAX_CP					:
				m_maxCP.m_sCorrectValueByPrefix	+=	iValue;
				break;

			case	eSKILL_EE_INCLINE_MAX_HP					:
				m_maxHP.m_sCorrectValueByPrefix	+=	iValue;
				break;

			case	eSKILL_EE_CORRECT_FIELD_FIRE_PENELTY		:
				m_bCorrectFieldFirePenelty		+=	iValue;
				break;

			case	eSKILL_EE_CORRECT_FIELD_WATER_PENELTY		:
				m_bCorrectFieldWaterPenelty		+=	iValue;
				break;

			case	eSKILL_EE_CORRECT_FIELD_WIND_PENELTY		:
				m_bCorrectFieldWindPenelty		+=	iValue;
				break;

			case	eSKILL_EE_CORRECT_FIELD_EARTH_PENELTY		:
				m_bCorrectFieldEarthPenelty		+=	iValue;
				break;

			case	eSKILL_EE_CORRECT_FIELD_LIGHT_PENELTY		:
				m_bCorrectFieldLightPenelty		+=	iValue;
				break;

			case	eSKILL_EE_CORRECT_FIELD_DARK_PENELTY		:
				m_bCorrectFieldDarkPenelty		+=	iValue;
				break;

			case	eSKILL_EE_INCREASE_MAGIC_ITEM_DROP_CHANCE		:
				m_bCorrectMagicItemDropChance	+=	iValue;
				break;

			case	eSKILL_EE_INCREASE_GOLD_DROP_CHANCE				:
				m_bCorrectGoldDropChance	+=	iValue;
				break;

			case	eSKILL_EE_STRENGTHENING_MAGIC_DAMAGE			:
				m_strengtheningMagicDamage.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_HIT_CHANCE					:
				m_hitChance.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_ATTACK_SPEED					:
				m_actionSpeed.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_DODGE_CHANCE					:
				m_dodgeChance.addValue(iValue);
				break;
			case	eSKILL_EE_DECLINE_TARGET_MAGIC_RESISTANCE		:
				m_weakningTargetMagicResistance.addValue(iValue);
				break;
			case	eSKILL_EE_INCREASE_FINAL_PHYSICAL_ATTACK_POWER	:
				m_wIncreaseFinalPhysicalAttackPower	+=	iValue;
				break;

			case	eSKILL_EE_INCLINE_MAGIC_RESISTANCE			:
				m_allMagicResistance.addValue(iValue);
				break;
				
			case	eSKILL_EE_INCLINE_FIRE_RESISTANCE			:	//	불저항 증가
				m_fireResistance.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_WATER_RESISTANCE			:	//	물저항 증가
				m_waterResistance.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_EARTH_RESISTANCE			:	//	대지저항 증가
				m_earthResistance.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_WIND_RESISTANCE			:	//	바람저항 증가
				m_windResistance.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_LIGHT_RESISTANCE			:	//	빛저항 증가
				m_lightResistance.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_DARK_RESISTANCE			:	//	어둠저항 증가
				m_darkResistance.addValue(iValue);
				break;

			case	eSKILL_EE_INCLINE_POWER						:	//	힘증가"	
				m_strength.addValue(iValue + iValueForIncPowerLevel);
				break;
			case	eSKILL_EE_INCLINE_AGILITY					:	//	민첩성증가"
				m_agility.addValue(iValue + iValueForIncPowerLevel);
				break;
			case	eSKILL_EE_INCLINE_CONSTITUTION				:	//	건강증가"	
				m_constitution.addValue(iValue + iValueForIncPowerLevel);
				break;
			case	eSKILL_EE_INCLINE_CHARISMA					:	//	건강증가"	
				m_charisma.addValue(iValue + iValueForIncPowerLevel);
				break;
			case	eSKILL_EE_INCLINE_INTELIGENCE				:	//	지식증가"	
				m_intelligence.addValue(iValue + iValueForIncPowerLevel);
				break;
			case	eSKILL_EE_INCLINE_WISDOM					:	//	지혜증가"	
				m_wisdom.addValue(iValue + iValueForIncPowerLevel);
				break;
		}
	}
}

BOOL
cActor_miniPet::getDroppedItemByMiniPet(CDroppedItem *_lpItem,cAbility *_lpAbility)
{
	int		iX	=	_lpItem->m_pos.x,iY	=	_lpItem->m_pos.y;

	if	(!m_lpField->pickItem(_lpItem->m_wSerial,-1,m_wSerialInField,TRUE))
		return	FALSE;

	m_lpField->addSendOccurEffectBySkill(iX,iY,_lpAbility->m_wSkill,eSE_SKILL);

	return	TRUE;
}


void
cActor_miniPet::operateHealSkillWork(int _iHealHP)//	CP 감소시 액션
{
	if	(isIdlePlayer())
		return;

	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet			*lpMiniPet	=	&m_aMiniPet[iIndex];
		cMiniPetAbilityInfo	*lpAbility	=	lpMiniPet->getSkillByType(nsMiniPetST::HealBonusByHealSkill);

		if	(!lpAbility)
			continue;

		cAbility	ability;
		CSkill		*lpSkill;

		ability.set(lpAbility->m_bf11Skill,lpAbility->m_bf4Level);

		lpSkill	=	ability.getSkill();

		if	(lpSkill->m_bf1IsNormalMiniPetSkill)
		{
			ability.m_wLevel	+=	m_bIncreaseMiniPetBasicSkillLevel;
			ability.m_wLevel	+=	m_abCorrectMiniPetBasicSkillLevel[i];
		}
		ability.m_wLevel		+=	m_bIncreaseMiniPetSkillLevel;
		ability.m_wLevel		+=	m_abCorrectMiniPetSkillLevel[i];

		if	(!lpSkill)
			return;

		if	(random(100)	>=	ability.getActivateChance())
			continue;

		int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*ability.m_wLevel)/100;

		_iHealHP	=	_iHealHP*iValue/100;

		recoverHPByAidSkill(lpSkill->m_wSerial,_iHealHP);
	}
}



void
cActor_miniPet::operateReduceCPWork(int _iSkill,int _iReduceCP)//	CP 감소시 액션
{
	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet			*lpMiniPet	=	&m_aMiniPet[iIndex];
		cMiniPetAbilityInfo	*lpAbility	=	lpMiniPet->getSkillByType(nsMiniPetST::RecoverUseCP);

		if	(!lpAbility)
			return;

		cAbility	ability;
		CSkill		*lpSkill;

		ability.set(lpAbility->m_bf11Skill,lpAbility->m_bf4Level);

		lpSkill	=	ability.getSkill();

		if	(!lpSkill)
			continue;

		if	(lpSkill->m_bf1IsNormalMiniPetSkill)
		{
			ability.m_wLevel	+=	m_bIncreaseMiniPetBasicSkillLevel;
			ability.m_wLevel	+=	m_abCorrectMiniPetBasicSkillLevel[i];
		}
		ability.m_wLevel		+=	m_bIncreaseMiniPetSkillLevel;
		ability.m_wLevel		+=	m_abCorrectMiniPetSkillLevel[i];

		if	(lpSkill->m_wDifficultyLevel	>	ability.m_wLevel)
			continue;

		if	(random(100)	>=	ability.getActivateChance())
			continue;

		int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*ability.m_wLevel)/100;

		_iReduceCP	=	_iReduceCP*iValue/100;

		m_iCP		+=	_iReduceCP;
		m_iCP		=	min(m_iCP,getMaxCP());

		syncPlayerBasicStatus(TRUE);
		addSendSkillEffect(&ability,eSE_CAST);
		m_lpField->addSendEtcWork(m_wSerialInField,eEW_MINIPET_ACTION,m_wSerialInField,0,0,i);
	}
}

//	포션힐 보너스

void
cActor_miniPet::operateUseHealPotionWork(int _iHP)
{
	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet			*lpMiniPet	=	&m_aMiniPet[iIndex];
		cMiniPetAbilityInfo	*lpAbility	=	lpMiniPet->getSkillByType(nsMiniPetST::UseHealPotion);

		if	(!lpAbility)
			continue;

		cAbility	ability;
		CSkill		*lpSkill;

		ability.set(lpAbility->m_bf11Skill,lpAbility->m_bf4Level);
		lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

		if	(lpSkill->m_bf1IsNormalMiniPetSkill)
		{
			ability.m_wLevel	+=	m_bIncreaseMiniPetBasicSkillLevel;
			ability.m_wLevel	+=	m_abCorrectMiniPetBasicSkillLevel[i];
		}
		ability.m_wLevel		+=	m_bIncreaseMiniPetSkillLevel;
		ability.m_wLevel		+=	m_abCorrectMiniPetSkillLevel[i];

		if	(random(10000)	>=	lpSkill->m_sActiveChance+lpSkill->m_sActiveChancePerLevel*ability.m_wLevel)
			continue;

		int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*ability.m_wLevel)/100;
		int	iHealHP	=	_iHP*iValue/100;

		recoverHPByAidSkill(lpSkill->m_wSerial,iHealHP);
	}
}

//	포션힐 보너스

void
cActor_miniPet::operateUseChargePotionWork(int _iCP)
{
	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet			*lpMiniPet	=	&m_aMiniPet[iIndex];
		cMiniPetAbilityInfo	*lpAbility	=	lpMiniPet->getSkillByType(nsMiniPetST::UseChargePotion);

		if	(!lpAbility)
			continue;

		cAbility	ability;
		CSkill		*lpSkill;

		ability.set(lpAbility->m_bf11Skill,lpAbility->m_bf4Level);
		lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

		if	(lpSkill->m_bf1IsNormalMiniPetSkill)
		{
			ability.m_wLevel	+=	m_bIncreaseMiniPetBasicSkillLevel;
			ability.m_wLevel	+=	m_abCorrectMiniPetBasicSkillLevel[i];
		}
		ability.m_wLevel		+=	m_bIncreaseMiniPetSkillLevel;
		ability.m_wLevel		+=	m_abCorrectMiniPetSkillLevel[i];

		if	(random(10000)	>=	lpSkill->m_sActiveChance+lpSkill->m_sActiveChancePerLevel*ability.m_wLevel)
			continue;

		int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*ability.m_wLevel)/100;
		int	iChargeCP=	_iCP*iValue/100;

		m_iCP		+=	iChargeCP;

		m_iCP		=	min(m_iCP,getMaxCP());

		syncPlayerBasicStatus(TRUE);
		addSendSkillEffect(&ability,eSE_CAST);
		m_lpField->addSendEtcWork(m_wSerialInField,eEW_MINIPET_ACTION,m_wSerialInField,0,0,i);
	}
}



//	부확 페널티 감소 수치

int
cActor_miniPet::getDecreaseDeathPeneltyValue()
{
	int	iMax	=	0;

	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iIndex];
		cAbility	ability;

		for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
		{
			cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[iSkill];

			if	(lpAbility->m_bf4Level	==	0)
				continue;
			if	(lpAbility->m_bf1IsOFF)
				continue;
			CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

			if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::DecreaseResurrectionPenelty)
				continue;

			int	iLevel			=	lpAbility->m_bf4Level;

			if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			{
				iLevel			+=	m_bIncreaseMiniPetBasicSkillLevel;
				iLevel			+=	m_abCorrectMiniPetBasicSkillLevel[i];
			}
			iLevel			+=	m_bIncreaseMiniPetSkillLevel;
			iLevel			+=	m_abCorrectMiniPetSkillLevel[i];

			int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*iLevel)/100;

			iMax	=	max(iMax,iValue);
		}
	}

	return	iMax;
}



int
cActor_miniPet::getCorrectUnlockLevel()
{
	int	iMax	=	0;

	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iIndex];
		cAbility	ability;

		for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
		{
			cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[iSkill];

			if	(lpAbility->m_bf4Level	==	0)
				continue;

			if	(lpAbility->m_bf1IsOFF)
				continue;

			CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

			if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::IncreaeUnlockLevel)
				continue;

			int	iLevel			=	lpAbility->m_bf4Level;

			if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			{
				iLevel			+=	m_bIncreaseMiniPetBasicSkillLevel;
				iLevel			+=	m_abCorrectMiniPetBasicSkillLevel[i];
			}
			iLevel			+=	m_bIncreaseMiniPetSkillLevel;
			iLevel			+=	m_abCorrectMiniPetSkillLevel[i];

			int	iValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*iLevel)/100;
			iMax	=	max(iMax,iValue);
		}
	}

	return	iMax;
}

void
cActor_miniPet::operateDestroyObjectSkill(int *_lpiAttackPower,int *_lpiMagicPower)
{
	int	iMaxP=*_lpiAttackPower,iMaxM=*_lpiMagicPower;

	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iIndex];
		cAbility	ability;

		for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
		{
			cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[iSkill];

			if	(lpAbility->m_bf4Level	==	0)
				continue;

			if	(lpAbility->m_bf1IsOFF)
				continue;

			CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

			if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::AttackTheObject)
				continue;

			int	iLevel			=	lpAbility->m_bf4Level;

			if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			{
				iLevel			+=	m_bIncreaseMiniPetBasicSkillLevel;
				iLevel			+=	m_abCorrectMiniPetBasicSkillLevel[i];
			}
			iLevel			+=	m_bIncreaseMiniPetSkillLevel;
			iLevel			+=	m_abCorrectMiniPetSkillLevel[i];

			int	iCorrectValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*iLevel)/100;

			int	iP	=	*_lpiAttackPower+*_lpiAttackPower*iCorrectValue/100;
			int	iM	=	*_lpiMagicPower+*_lpiMagicPower*iCorrectValue/100;

			iMaxP	=	max(iMaxP,iP);
			iMaxM	=	max(iMaxM,iM);
			break;
		}
	}

	*_lpiAttackPower	=	iMaxP;
	*_lpiMagicPower		=	iMaxM;
}




void
cActor_miniPet::operateDodgeTrapSkill(int *_lpiDodgeChance)
{
	int	iMax	=	*_lpiDodgeChance;

	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iIndex];
		cAbility	ability;

		for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
		{
			cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[iSkill];

			if	(lpAbility->m_bf4Level	==	0)
				continue;

			if	(lpAbility->m_bf1IsOFF)
				continue;

			CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

			if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::AvoidTrap)
				continue;

			int	iLevel			=	lpAbility->m_bf4Level;

			if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			{
				iLevel			+=	m_bIncreaseMiniPetBasicSkillLevel;
				iLevel			+=	m_abCorrectMiniPetBasicSkillLevel[i];
			}
			iLevel			+=	m_bIncreaseMiniPetSkillLevel;
			iLevel			+=	m_abCorrectMiniPetSkillLevel[i];

			int	iCorrectValue	=	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*iLevel)/100;

			int	iValue			=	*_lpiDodgeChance+iCorrectValue/100;
			iMax	=	max(iValue,iMax);
		}
	}

	*_lpiDodgeChance	=	iMax;
}



void
cActor_miniPet::operateEvolutionGradeBonus()
{
	memset(m_abCorrectMiniPetBasicSkillLevel,0,2);
	memset(m_abCorrectMiniPetSkillLevel,0,2);
	memset(m_abCorrectMiniPetSkillActiveChance,0,2);
	memset(m_abCorrectMiniPetSkillDamage,0,2);

	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iPetIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iPetIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iPetIndex];

		int	iRate;

		if	(lpMiniPet->getStyle()	==	nsMiniPetStyle::Bio)
		{
			iRate	=	lpMiniPet->getEvolutionRate(nsMiniPetStyle::Bio);

			if	(iRate	>=	99)
			{
				m_abCorrectMiniPetSkillDamage[i]	=	50;
			}
			else
			if	(iRate	>=	80)
			{
				m_abCorrectMiniPetSkillDamage[i]	=	20;
			}
			else
			if	(iRate	>=	50)
			{
				m_abCorrectMiniPetSkillDamage[i]	=	10;
			}
		}
		else
		if	(lpMiniPet->getStyle()	==	nsMiniPetStyle::Energy)
		{
			iRate	=	lpMiniPet->getEvolutionRate(nsMiniPetStyle::Energy);

			if	(iRate	>=	99)
			{
				m_abCorrectMiniPetSkillActiveChance[i]	=	10;
			}
			else
			if	(iRate	>=	80)
			{
				m_abCorrectMiniPetSkillActiveChance[i]	=	3;
			}
			else
			if	(iRate	>=	50)
			{
				m_abCorrectMiniPetSkillActiveChance[i]	=	1;
			}
		}
		else
		if	(lpMiniPet->getStyle()	==	nsMiniPetStyle::Mineral)
		{
			iRate	=	lpMiniPet->getEvolutionRate(nsMiniPetStyle::Mineral);

			if	(iRate	>=	99)
			{
				m_abCorrectMiniPetSkillLevel[i]			=	1;
			}
			else
			if	(iRate	>=	80)
			{
				m_abCorrectMiniPetBasicSkillLevel[i]	=	1;
			}
		}
	}
}

BOOL
cActor_miniPet::operatePlaceCombiConditionBonus()
{
	m_bIncreaseMiniPetFireDamage		=	0;
	m_bIncreaseMiniPetWaterDamage		=	0;
	m_bIncreaseMiniPetWindDamage		=	0;
	m_bIncreaseMiniPetEarthDamage		=	0;
	m_bIncreaseMiniPetLightDamage		=	0;
	m_bIncreaseMiniPetDarkDamage		=	0;
	m_bIncreaseMiniPetMagicDamage		=	0;
	m_bIncreaseMiniPetSkillActiveChance	=	0;
	m_bIncreaseMiniPetBasicSkillLevel	=	0;
	m_bIncreaseMiniPetSkillLevel		=	0;
	m_bIncreaseMiniPetMagicDamageVsBoss	=	0;

	if	(getMiniPetCount()	<	c_iMiniPetCount)
		return	FALSE;

	int	iCorrectConditionCount	=	0;

	for	(int i=g_sm.m_iMiniPetCombiSkillCount;i;)
	{
		--i;

		int		iSkill		=	g_sm.m_aiMiniPetCombiSkill[i];
		CSkill	*lpSkill	=	&g_aSkill[iSkill];

		BOOL	bIsCorrect	=	TRUE;

		for	(int i=0;i<16;i++)
		{
			if	(lpSkill->m_aValue[i]	==	-1)	//	아무거나
				continue;

			if	(lpSkill->m_aValue[i]	>=	100)
			{
				if	((int)(m_aMiniPet[i].m_bf5Type%6)	!=	lpSkill->m_aValue[i]-100)
				{
					bIsCorrect	=	FALSE;
					break;
				}
			}
			else
			if	((int)m_aMiniPet[i].m_bf5Type	!=	lpSkill->m_aValue[i])
			{
				bIsCorrect	=	FALSE;
				break;
			}
		}

		if	(bIsCorrect	==	FALSE)
			continue;

		int	iEffect	=	lpSkill->m_aValue[16];
		int	iValue	=	lpSkill->m_aValue[17];

		switch(iEffect)
		{
			case	eIncrease_MiniPet_FireDamage:
				m_bIncreaseMiniPetFireDamage	=	max(m_bIncreaseMiniPetFireDamage,iValue);
				break;
			case	eIncrease_MiniPet_WaterDamage:
				m_bIncreaseMiniPetWaterDamage	=	max(m_bIncreaseMiniPetWaterDamage,iValue);
				break;
			case	eIncrease_MiniPet_WindDamage:
				m_bIncreaseMiniPetWindDamage	=	max(m_bIncreaseMiniPetWindDamage,iValue);
				break;
			case	eIncrease_MiniPet_EarthDamage:
				m_bIncreaseMiniPetEarthDamage	=	max(m_bIncreaseMiniPetEarthDamage,iValue);
				break;
			case	eIncrease_MiniPet_LightDamage:
				m_bIncreaseMiniPetLightDamage	=	max(m_bIncreaseMiniPetLightDamage,iValue);
				break;
			case	eIncrease_MiniPet_DarkDamage:
				m_bIncreaseMiniPetDarkDamage	=	max(m_bIncreaseMiniPetDarkDamage,iValue);
				break;
			case	eIncrease_MiniPet_MagicDamage:
				m_bIncreaseMiniPetMagicDamage	=	max(m_bIncreaseMiniPetMagicDamage,iValue);
				break;
			case	eIncrease_MiniPet_SkillActiveChance:
				m_bIncreaseMiniPetSkillActiveChance	=	max(m_bIncreaseMiniPetSkillActiveChance,iValue);
				break;
			case	eIncrease_MiniPet_BasicSkillLevel:
				m_bIncreaseMiniPetBasicSkillLevel	=	max(m_bIncreaseMiniPetBasicSkillLevel,iValue);
				break;
			case	eIncrease_MiniPet_SkillLevel:
				m_bIncreaseMiniPetSkillLevel		=	max(m_bIncreaseMiniPetSkillLevel	,iValue);
				break;
			case	eIncrease_MiniPet_MagicDamage_vsBoss:
				m_bIncreaseMiniPetMagicDamageVsBoss	=	max(m_bIncreaseMiniPetMagicDamageVsBoss,iValue);
				break;
		}

		iCorrectConditionCount++;

		break;
	}

	if	(iCorrectConditionCount)
		return	TRUE;

	return	FALSE;
}

//	미니펫 버프 스킬 처리

void
cActor_miniPet::operatePowerUpSkill()
{
	operatePlaceCombiConditionBonus();
	operateEvolutionGradeBonus();

	for (int i=0;i<2;i++)
	{
		int	iPetIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iPetIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iPetIndex];
		cAbility	ability;

		for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
		{
			cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[iSkill];

			if	(lpAbility->m_bf4Level	==	0)
				continue;

			if	(lpAbility->m_bf1IsOFF)
				continue;

			CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

			if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::LordPowerUp)
				continue;

			int	iLevel			=	lpAbility->m_bf4Level;

			if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			{
				iLevel			+=	m_bIncreaseMiniPetBasicSkillLevel;
				iLevel			+=	m_abCorrectMiniPetBasicSkillLevel[i];
			}
			iLevel			+=	m_bIncreaseMiniPetSkillLevel;
			iLevel			+=	m_abCorrectMiniPetSkillLevel[i];

			ability.set(lpAbility->m_bf11Skill,iLevel);

			applyMiniPetBuffSkillEffects(&ability);
		}
	}
}


void
cActor_miniPet::operateSkillForKilled()//	죽으면 발동하는 스킬 처리
{
	if	(isIdlePlayer())
		return;

	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iPetIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iPetIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iPetIndex];
		cAbility	ability;

		for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
		{
			cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[iSkill];

			if	(lpAbility->m_bf4Level	==	0)
				continue;
			if	(lpAbility->m_bf1IsOFF)
				continue;
			CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

			if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::Resurrection)
				continue;

			int	iLevel			=	lpAbility->m_bf4Level;

			if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			{
				iLevel		+=	m_bIncreaseMiniPetBasicSkillLevel;
				iLevel		+=	m_abCorrectMiniPetBasicSkillLevel[i];
			}
			iLevel			+=	m_bIncreaseMiniPetSkillLevel;
			iLevel			+=	m_abCorrectMiniPetSkillLevel[i];

			int	iActiveChance	=	lpSkill->m_sActiveChancePerLevel*iLevel;

			if	(lpSkill->m_bf1IsXMiniPetLevel)
				iActiveChance	*=	lpMiniPet->m_bf7Level;

			iActiveChance		+=	lpSkill->m_sActiveChance;
			iActiveChance		+=	m_abCorrectMiniPetSkillActiveChance[i];
			iActiveChance		+=	iActiveChance*m_bIncreaseMiniPetSkillActiveChance/100;

			//JBC	펫 부활 2차 미니펫 강화 적용.	08-11-03 짤렷음...
// 			if(g_iSeasonVariable >= 1)
// 			{
// 				int iValue3 = lpSkill->m_aValue[3];
// 				int iValue4 = lpSkill->m_aValue[4];
// 
// 				if(iValue4 == 0)		iValue4 = 1;
// 
// 				iActiveChance	+=	iValue3 + (m_iLevel * 100 ) / iValue4;
// 			}
			

			if	(random(10000)	>=	iActiveChance)
				continue;

			ability.set(lpAbility->m_bf11Skill,iLevel);

			int	iRecoverPeneltyOfPower	=	lpSkill->m_aValue[1] + lpSkill->m_aValue[0] * ability.m_wLevel;

			if	(revive(iRecoverPeneltyOfPower,ability.getHealPoint(TRUE , m_iLevel),lpSkill->m_aValue[2]))
				addSendSkillEffect(&ability,eSE_CAST);
		}
	}
}

void
cActor_miniPet::operateVSBossCorrectMagicPower(cACTOR *_lpActor,int *_lpiFireDamage,int *_lpiWaterDamage,int *_lpiWindDamage,int *_lpiEarthDamage,int *_lpiLightDamage,int *_lpiDarkDamage)
{
	if	(!_lpActor	||	!_lpActor->isBossMonster())
		return;

	int	iFire=*_lpiFireDamage,iWater=*_lpiWaterDamage,iWind=*_lpiWindDamage,iEarth=*_lpiEarthDamage,iLight=*_lpiLightDamage,iDark=*_lpiDarkDamage;

	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iPetIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iPetIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iPetIndex];

		int	iFireD=*_lpiFireDamage,iWaterD=*_lpiWaterDamage,iWindD=*_lpiWindDamage,iEarthD=*_lpiEarthDamage,iLightD=*_lpiLightDamage,iDarkD=*_lpiDarkDamage;

		lpMiniPet->operateVSBossCorrectMagicPower(_lpActor,&iFireD,&iWaterD,&iWindD,&iEarthD,&iLightD,&iDarkD);

		iFireD	+=	iFireD*m_bIncreaseMiniPetMagicDamageVsBoss/100;
		iWaterD	+=	iWaterD*m_bIncreaseMiniPetMagicDamageVsBoss/100;
		iWindD	+=	iWindD*m_bIncreaseMiniPetMagicDamageVsBoss/100;
		iEarthD	+=	iEarthD*m_bIncreaseMiniPetMagicDamageVsBoss/100;
		iLightD	+=	iLightD*m_bIncreaseMiniPetMagicDamageVsBoss/100;
		iDarkD	+=	iDarkD*m_bIncreaseMiniPetMagicDamageVsBoss/100;

		iFire	=	max(iFireD,iFire);
		iWater	=	max(iWaterD,iWater);
		iWind	=	max(iWindD,iWind);
		iEarth	=	max(iEarthD,iEarth);
		iLight	=	max(iLightD,iLight);
		iDark	=	max(iDarkD,iDark);
	}

	*_lpiFireDamage	=	iFire;
	*_lpiWaterDamage=	iWater;
	*_lpiWindDamage	=	iWind;
	*_lpiEarthDamage=	iEarth;
	*_lpiLightDamage=	iLight;
	*_lpiDarkDamage	=	iDark;
}

int
cActor_miniPet::getVSBossCorrectTargetMagicResistance(cACTOR *_lpActor)
{
	int	iMin	=	0;

	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iPetIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iPetIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iPetIndex];

		iMin	=	min(iMin,lpMiniPet->getVSBossCorrectTargetMagicResistance(_lpActor));
	}

	return	iMin;
}

//
//	마법 데미지 산출

void
cActor_miniPet::getMagicDamageByMiniPet(cMiniPet *_lpMiniPet,cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,int _iPetOrder, int _iLevel)
{
	if	(m_wJumppingSkippingTime	||	m_wTransToFrogTime	||	m_wHwabyungTime)
		return;
	
	if	(m_wDodgeMagicDamageTime)
		if	(random(100)	<	m_wDodgeMagicDamageChance)
			return;

	if	(m_bIsGuildObject)
	{
		_lpHitInfo->reset(TRUE);

		CGuild	*lpGuild			=	getGuild();
		CGuild	*lpAttackerGuild	=	_lpAttacker->getGuild();

		int		iDamage				=	100;

		if	(m_bIsGuildCrest)
		{
			if	(lpAttackerGuild	&&	random(100)	<	lpAttackerGuild->m_wIncreaseGuildCrestEfficient)
			{
				iDamage			=	200;
				_lpHitInfo->setCritical();
			}

			if	(lpGuild	&&	random(100)	<	lpGuild->m_wIncreaseGuildCrestEfficient)
			{
				iDamage			=	0;
				_lpHitInfo->reset(TRUE);
			}
		}
		else
		{
			if	(lpAttackerGuild	&&	random(100)	<	lpAttackerGuild->m_wIncreaseGuildFlagEfficient)
			{
				iDamage			=	200;
				_lpHitInfo->setCritical();
			}

			if	(lpGuild	&&	random(100)	<	lpGuild->m_wIncreaseGuildFlagEfficient)
			{
				iDamage			=	0;
				_lpHitInfo->reset(TRUE);
			}
		}

		if	(iDamage)
		{
			_lpHitInfo->setHit();

			_lpHitInfo->m_dwMagicDamage		=	iDamage;
			_lpHitInfo->m_dwPhysicalDamage	=	0;
		}

		return;
	}

	int								iFireDamage	=0,iWaterDamage=0,iWindDamage=0,iEarthDamage=0,iLightDamage=0,iDarkDamage=0,iPhysicalDamage=0;
	CSkill							*lpSkill	=	_lpAbility->getSkill();
	CContinuousAttackedPeneltyInfo	*lpContinuousAttackedPeneltyInfo	=	NULL;
	BOOL							bIsDamaged	=	FALSE;
	BOOL							bIsPVP		=	FALSE;
	BOOL							bIsBossBattle=	FALSE;
	cAbility						abilityPowerUp;

	bIsPVP		=	m_lpField->isPVP(_lpAttacker,this);

	if	(_lpAttacker->m_wIsBoss	||	m_wIsBoss)
		bIsBossBattle	=	TRUE;

	int	iLevelCorrectDamagePercentage	=	0;

	if	(bIsPVP	&&	g_bIsEnablePVPMagicDamageRule)
		iLevelCorrectDamagePercentage	=	(_lpAttacker->m_iLevel-m_iLevel)*30/25;

	if	(bIsBossBattle)
		iLevelCorrectDamagePercentage	=	(m_iLevel-_lpAttacker->m_iLevel)*30/25;

	{
		cMiniPetAbilityInfo	*lpPowerUpSkill	=	_lpMiniPet->getPowerUpSkill(_lpAbility->m_wSkill);

		if	(lpPowerUpSkill)
		{
			abilityPowerUp.set(lpPowerUpSkill->m_bf11Skill,lpPowerUpSkill->m_bf4Level);

			if	(abilityPowerUp.getSkill()->m_bf1IsNormalMiniPetSkill)
			{
				abilityPowerUp.m_wLevel	+=	m_bIncreaseMiniPetBasicSkillLevel;
				abilityPowerUp.m_wLevel	+=	m_abCorrectMiniPetBasicSkillLevel[_iPetOrder];
			}

			abilityPowerUp.m_wLevel	+=	m_bIncreaseMiniPetSkillLevel;
			abilityPowerUp.m_wLevel	+=	m_abCorrectMiniPetSkillLevel[_iPetOrder];
		}
	}

	CSkill							*lpPowerUpSkill	=	abilityPowerUp.getSkill();

	int	iCorrectResistance	=	0;

	iCorrectResistance		=	_lpMiniPet->getVSBossCorrectTargetMagicResistance(this);

	int	iCorrectDamage			=	_lpAttacker->m_abCorrectMiniPetSkillDamage[_iPetOrder]+_lpAttacker->m_bIncreaseMiniPetMagicDamage;

	if	(lpSkill->isFireDamageSkill())
	{
		iFireDamage		=	getMiniPetMagicDamage(_lpAbility,_lpMiniPet->m_bf7Level,lpSkill->m_sFireDamage,lpSkill->m_sFireDamagePerLevel,lpSkill->m_sFireDamageRange,lpSkill->m_sFireDamageRangePerLevel,_lpMiniPet , _iLevel);

		int iAbilityPowerUpValue = abilityPowerUp.getFireDamagePercentage();

		if (_lpMiniPet->m_bf1MinipetAwaken100)
			iAbilityPowerUpValue += abilityPowerUp.getCorrectAwakenDamage(dAWAKENTYPE_100 , _lpMiniPet->m_bf7Level);
		else if(_lpMiniPet->m_bf1MinipetAwaken50)
			iAbilityPowerUpValue += abilityPowerUp.getCorrectAwakenDamage(dAWAKENTYPE_50 , _lpMiniPet->m_bf7Level);
 
 		iFireDamage		+=	iFireDamage*(iAbilityPowerUpValue + _lpAttacker->m_bIncreaseMiniPetFireDamage+iCorrectDamage)/100;

		if	(isBossMonster())
			iFireDamage		+=	iFireDamage*_lpAttacker->m_bIncreaseMiniPetMagicDamageVsBoss/100;

		iFireDamage		-=	iFireDamage*getFireResistance(iCorrectResistance)/100;
		bIsDamaged		=	TRUE;
	}

	if	(lpSkill->isWindDamageSkill())
	{
		iWindDamage		=	getMiniPetMagicDamage(_lpAbility,_lpMiniPet->m_bf7Level,lpSkill->m_sWindDamage,lpSkill->m_sWindDamagePerLevel,lpSkill->m_sWindDamageRange,lpSkill->m_sWindDamageRangePerLevel,_lpMiniPet, _iLevel);

		int iAbilityPowerUpValue = abilityPowerUp.getWindDamagePercentage();
		
		if (_lpMiniPet->m_bf1MinipetAwaken100)
			iAbilityPowerUpValue += abilityPowerUp.getCorrectAwakenDamage(dAWAKENTYPE_100 , _lpMiniPet->m_bf7Level);
		else if(_lpMiniPet->m_bf1MinipetAwaken50)
			iAbilityPowerUpValue += abilityPowerUp.getCorrectAwakenDamage(dAWAKENTYPE_50 , _lpMiniPet->m_bf7Level);

		iWindDamage		+=	iWindDamage*(iAbilityPowerUpValue + _lpAttacker->m_bIncreaseMiniPetWindDamage+iCorrectDamage)/100;

		if	(isBossMonster())
			iWindDamage		+=	iWindDamage*_lpAttacker->m_bIncreaseMiniPetMagicDamageVsBoss/100;

		iWindDamage		-=	iWindDamage*getWindResistance(iCorrectResistance)/100;
		bIsDamaged		=	TRUE;
	}

	if	(lpSkill->isEarthDamageSkill())
	{
		iEarthDamage	=	getMiniPetMagicDamage(_lpAbility,_lpMiniPet->m_bf7Level,lpSkill->m_sEarthDamage,lpSkill->m_sEarthDamagePerLevel,lpSkill->m_sEarthDamageRange,lpSkill->m_sEarthDamageRangePerLevel,_lpMiniPet, _iLevel);
		
		int iAbilityPowerUpValue = abilityPowerUp.getEarthDamagePercentage();

		if (_lpMiniPet->m_bf1MinipetAwaken100)
			iAbilityPowerUpValue += abilityPowerUp.getCorrectAwakenDamage(dAWAKENTYPE_100 , _lpMiniPet->m_bf7Level);
		else if(_lpMiniPet->m_bf1MinipetAwaken50)
			iAbilityPowerUpValue += abilityPowerUp.getCorrectAwakenDamage(dAWAKENTYPE_50 , _lpMiniPet->m_bf7Level);

		iEarthDamage	+=	iEarthDamage*(iAbilityPowerUpValue + _lpAttacker->m_bIncreaseMiniPetEarthDamage+iCorrectDamage)/100;

		if	(isBossMonster())
			iEarthDamage		+=	iEarthDamage*_lpAttacker->m_bIncreaseMiniPetMagicDamageVsBoss/100;

		iEarthDamage	-=	iEarthDamage*getEarthResistance(iCorrectResistance)/100;
		bIsDamaged		=	TRUE;

	}

	if	(lpSkill->isLightDamageSkill())
	{
		iLightDamage	=	getMiniPetMagicDamage(_lpAbility,_lpMiniPet->m_bf7Level,lpSkill->m_sLightDamage,lpSkill->m_sLightDamagePerLevel,lpSkill->m_sLightDamageRange,lpSkill->m_sLightDamageRangePerLevel,_lpMiniPet, _iLevel);

		int iAbilityPowerUpValue = abilityPowerUp.getLightDamagePercentage();

		if (_lpMiniPet->m_bf1MinipetAwaken100)
			iAbilityPowerUpValue += abilityPowerUp.getCorrectAwakenDamage(dAWAKENTYPE_100 , _lpMiniPet->m_bf7Level);
		else if(_lpMiniPet->m_bf1MinipetAwaken50)
			iAbilityPowerUpValue += abilityPowerUp.getCorrectAwakenDamage(dAWAKENTYPE_50 , _lpMiniPet->m_bf7Level);

		iLightDamage	+=	iLightDamage*(iAbilityPowerUpValue+_lpAttacker->m_bIncreaseMiniPetLightDamage+iCorrectDamage)/100;

		if	(isBossMonster())
			iLightDamage		+=	iLightDamage*_lpAttacker->m_bIncreaseMiniPetMagicDamageVsBoss/100;

		iLightDamage	-=	iLightDamage*getLightResistance(iCorrectResistance)/100;
		bIsDamaged		=	TRUE;
	}

	if	(lpSkill->isDarkDamageSkill())
	{
		iDarkDamage		=	getMiniPetMagicDamage(_lpAbility,_lpMiniPet->m_bf7Level,lpSkill->m_sDarkDamage,lpSkill->m_sDarkDamagePerLevel,lpSkill->m_sDarkDamageRange,lpSkill->m_sDarkDamageRangePerLevel,_lpMiniPet, _iLevel);

		int iAbilityPowerUpValue = abilityPowerUp.getDarkDamagePercentage();

		if (_lpMiniPet->m_bf1MinipetAwaken100)
			iAbilityPowerUpValue += abilityPowerUp.getCorrectAwakenDamage(dAWAKENTYPE_100 , _lpMiniPet->m_bf7Level);
		else if(_lpMiniPet->m_bf1MinipetAwaken50)
			iAbilityPowerUpValue += abilityPowerUp.getCorrectAwakenDamage(dAWAKENTYPE_50 , _lpMiniPet->m_bf7Level);
		
		iDarkDamage		+=	iDarkDamage*(iAbilityPowerUpValue+_lpAttacker->m_bIncreaseMiniPetDarkDamage+iCorrectDamage)/100;

		if	(isBossMonster())
			iDarkDamage		+=	iDarkDamage*_lpAttacker->m_bIncreaseMiniPetMagicDamageVsBoss/100;

		iDarkDamage		-=	iDarkDamage*getDarkResistance(iCorrectResistance)/100;
		bIsDamaged		=	TRUE;
	}

	if	(lpSkill->isWaterDamageSkill())
	{
		iWaterDamage	=	getMiniPetMagicDamage(_lpAbility,_lpMiniPet->m_bf7Level,lpSkill->m_sWaterDamage,lpSkill->m_sWaterDamagePerLevel,lpSkill->m_sWaterDamageRange,lpSkill->m_sWaterDamageRangePerLevel,_lpMiniPet, _iLevel);

		int iAbilityPowerUpValue = abilityPowerUp.getWaterDamagePercentage();

		if (_lpMiniPet->m_bf1MinipetAwaken100)
			iAbilityPowerUpValue += abilityPowerUp.getCorrectAwakenDamage(dAWAKENTYPE_100 , _lpMiniPet->m_bf7Level);
		else if(_lpMiniPet->m_bf1MinipetAwaken50)
			iAbilityPowerUpValue += abilityPowerUp.getCorrectAwakenDamage(dAWAKENTYPE_50 , _lpMiniPet->m_bf7Level);

		iWaterDamage	+=	iWaterDamage*(iAbilityPowerUpValue + _lpAttacker->m_bIncreaseMiniPetWaterDamage+iCorrectDamage)/100;

		if	(isBossMonster())
			iWaterDamage		+=	iWaterDamage*_lpAttacker->m_bIncreaseMiniPetMagicDamageVsBoss/100;

		iWaterDamage	-=	iWaterDamage*getWaterResistance(iCorrectResistance)/100;
		bIsDamaged		=	TRUE;
	}

	_lpMiniPet->operateVSBossCorrectMagicPower(this,&iFireDamage	,&iWaterDamage,&iWindDamage,&iEarthDamage,&iLightDamage,&iDarkDamage);

	int	iAbsorbDamage		=	0;

	if	(m_bAbsorbMagicDamage+m_bAbsorbFireDamage	&&	iFireDamage)	iAbsorbDamage	+=	iFireDamage*(m_bAbsorbFireDamage+m_bAbsorbMagicDamage)/100;		//	불꽃 데미지를 흡수해서 체력으로 변환
	if	(m_bAbsorbMagicDamage+m_bAbsorbWaterDamage	&&	iWaterDamage)	iAbsorbDamage	+=	iWaterDamage*(m_bAbsorbWaterDamage+m_bAbsorbMagicDamage)/100;	//	물 데미지를 흡수해서 체력으로 변환
	if	(m_bAbsorbMagicDamage+m_bAbsorbWindDamage	&&	iWindDamage)	iAbsorbDamage	+=	iWindDamage*(m_bAbsorbWindDamage+m_bAbsorbMagicDamage)/100;		//	바람 데미지를 흡수해서 체력으로 변환
	if	(m_bAbsorbMagicDamage+m_bAbsorbEarthDamage	&&	iEarthDamage)	iAbsorbDamage	+=	iEarthDamage*(m_bAbsorbEarthDamage+m_bAbsorbMagicDamage)/100;	//	대지 데미지를 흡수해서 체력으로 변환
	if	(m_bAbsorbMagicDamage+m_bAbsorbLightDamage	&&	iLightDamage)	iAbsorbDamage	+=	iLightDamage*(m_bAbsorbLightDamage+m_bAbsorbMagicDamage)/100;	//	빛 데미지를 흡수해서 체력으로 변환
	if	(m_bAbsorbMagicDamage+m_bAbsorbDarkDamage	&&	iDarkDamage)	iAbsorbDamage	+=	iDarkDamage*(m_bAbsorbDarkDamage+m_bAbsorbMagicDamage)/100;		//	어둠 데미지를 흡수해서 체력으로 변환

	iFireDamage		-=	iFireDamage*(m_bAbsorbFireDamage+m_bAbsorbMagicDamage)/100;
	iWaterDamage	-=	iWaterDamage*(m_bAbsorbWaterDamage+m_bAbsorbMagicDamage)/100;
	iWindDamage		-=	iWindDamage*(m_bAbsorbWindDamage+m_bAbsorbMagicDamage)/100;
	iEarthDamage	-=	iEarthDamage*(m_bAbsorbEarthDamage+m_bAbsorbMagicDamage)/100;
	iLightDamage	-=	iLightDamage*(m_bAbsorbLightDamage+m_bAbsorbMagicDamage)/100;
	iDarkDamage		-=	iDarkDamage*(m_bAbsorbDarkDamage+m_bAbsorbMagicDamage)/100;

	if	(iFireDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iFireDamage;}
	if	(iWaterDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iWaterDamage;}
	if	(iWindDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iWindDamage;}
	if	(iEarthDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iEarthDamage;}
	if	(iLightDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iLightDamage;}
	if	(iDarkDamage>	0)	{_lpHitInfo->m_dwMagicDamage	+=	iDarkDamage;}

	if	(bIsPVP)
		iAbsorbDamage	=	iAbsorbDamage*dPVP_ABSORB_DAMAGE_FACTOR/100;

	if	(m_lpField->isPVP(_lpAttacker,this))
		_lpHitInfo->m_dwMagicDamage	=	GetP2PCorrectDamage(_lpHitInfo->m_dwMagicDamage);

	if	(_lpHitInfo->m_dwMagicDamage)
		_lpHitInfo->setHit();
	else
	if	(bIsDamaged)
		_lpHitInfo->setImmune();
	
	if	(!_lpAbility->isWideAreaAttack() && _lpHitInfo->m_dwMagicDamage)
	{
		int	iBlock	=	blocking(_lpAttacker,_lpAbility);

		if	(iBlock)
		{
			_lpHitInfo->m_dwMagicDamage	/=	dBLOCKING_FACTOR;

			if	(iBlock	==	eBLOCKING					)
				_lpHitInfo->setBlock();
			if	(iBlock	==	eBLOCKING_NO_ACTION			)
				_lpHitInfo->setNoActionBlock();
			if	(iBlock	==	eBLOCKING_BY_DANCING_BLOCKER)
				_lpHitInfo->setDancingBlockerBlock();
			if	(iBlock	==	eBLOCKING_IGNORE)
				_lpHitInfo->setIgnoreBlock();
		}
	}

	if	(m_wCorrectFinalDamage	!=	10000)
	{
		LONGLONG	llDamage;
		llDamage	=	_lpHitInfo->m_dwPhysicalDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		_lpHitInfo->m_dwPhysicalDamage	=	(DWORD)llDamage;
		_lpHitInfo->m_dwPhysicalDamage	=	max(_lpHitInfo->m_dwPhysicalDamage,100);

		llDamage	=	_lpHitInfo->m_dwMagicDamage;
		llDamage	=	llDamage*m_wCorrectFinalDamage/10000;
		_lpHitInfo->m_dwMagicDamage		=	(DWORD)llDamage;
		_lpHitInfo->m_dwMagicDamage		=	max(_lpHitInfo->m_dwMagicDamage,100);
	}

}


//	테스트 미니펫으로 설정
void
cActor_miniPet::setTestMiniPet(int _iStyle)
{
	cMiniPet	*lpMiniPet	=	getActiveMiniPet(0);

	if	(lpMiniPet)
	{
		lpMiniPet->setTest(_iStyle);
		sendMiniPetInfo(m_bf5ActiveMiniPet,eUMIM_UPDATE,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);
	}
}

void
cActor_miniPet::checkCurrentMiniPetExpiredInfo(BOOL _bIsSendMessage)//	현재 활성화된 미니펫의 유효기간을 확인한다.
{
	m_wIsActiveReversalFrame	=	FALSE;
	BOOL	bIsRequireRebuild	=	FALSE;

	if	(m_reversalFrameMonth	>	12	||	m_reversalFrameHour	>	24	||	m_reversalFrameMinute	>	60)
	{
		m_reversalFrameYear		=	0;
		m_reversalFrameMonth	=	0;
		m_reversalFrameDay		=	0;
		m_reversalFrameHour		=	0;
		m_reversalFrameMinute	=	0;	//	25
	}

	{
		CTimeInfo	currentTime,expireTime;
		currentTime.update();

		expireTime.m_wYear	=	m_reversalFrameYear+2000;
		expireTime.m_wMonth	=	m_reversalFrameMonth;
		expireTime.m_wDay	=	m_reversalFrameDay;
		expireTime.m_wHour	=	m_reversalFrameHour;
		expireTime.m_wMinute=	m_reversalFrameMinute;	//	25

		if	(currentTime.getMinuteValue(2000)	<	expireTime.getMinuteValue(2000))
			m_wIsActiveReversalFrame	=	TRUE;
	}

	if	(m_wIsActiveReversalFrame	==	FALSE)
	{
		if	(m_bf5ActiveMiniPet2	!=	c_iMiniPetCount)
		{
			reset2();

			bIsRequireRebuild	=	TRUE;

			if	(_bIsSendMessage)
				sendRegistServerMessage(eRSM_EXPIRED_REVERSAL_FRAME_TIME,eSM_WARNING);
		}
	}

	for (int i=0;i<m_wIsActiveReversalFrame+1;i++)
	{
		int	iPetIndex	=	m_bf5ActiveMiniPet;

		if	 (i	==	1)
			iPetIndex	=	m_bf5ActiveMiniPet2;

		if	(!isAvailMiniPet(i))
			continue;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iPetIndex];

		if	(lpMiniPet->isExpired())
		{
			if	(i	==	0)
			{
				m_bf5ActiveMiniPet	=	c_iMiniPetCount;
				reset1();
			}
			else
			{
				m_bf5ActiveMiniPet2	=	c_iMiniPetCount;
				reset2();
			}

			bIsRequireRebuild	=	TRUE;

			if	(_bIsSendMessage)
				sendMiniPetInfo(iPetIndex,eUMIM_EXPIRED,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);
		}
	}

	if	(bIsRequireRebuild)
	{
		if	(m_wIsActiveReversalFrame	&&	m_bf5ActiveMiniPet	==	c_iMiniPetCount	&&	m_bf5ActiveMiniPet2	!=	c_iMiniPetCount	)
		{
			copy2To1();
			m_bf5ActiveMiniPet	=	m_bf5ActiveMiniPet2;
			reset2();

			if	(_bIsSendMessage)
				sendMiniPetInfo(0,eUMIM_UPDATE,m_bf5ActiveMiniPet,m_bf5ActiveMiniPet2);
		}

		buildPower();
	}
}
