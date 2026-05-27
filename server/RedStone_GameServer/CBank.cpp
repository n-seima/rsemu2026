#include "CBank.H"
#include "CField.H"
#include "CServer.H"
#include "CGame.H"
#include "GameDLL.H"

CBankManager	g_bankManager;

//
//	정상적인 데이터?
BOOL
CBank::checkIsIncorrectData(DWORD _dwChecksum)
{
	WORD	wActorItemChecksum	=	m_lpActor->getItemCheckSum();
	
	if (getCheckSum()		!=	_dwChecksum					)
	{
		CLOG("bank","CBank::checkIsIncorrectData - getCheckSum() != _dwChecksum %s/%s",m_lpActor->m_strId,m_lpActor->m_strName);

		return	TRUE;
	}

	if (STRICMP(m_bankInfo.m_strId,m_lpActor->m_strId)!=0	)
	{
		CLOG("bank","CBank::checkIsIncorrectData - STRICMP(m_bankInfo.m_strId,m_lpActor->m_strId) %s/%s",m_lpActor->m_strId,m_lpActor->m_strName);
		return	TRUE;
	}

	if (wActorItemChecksum	!=	m_wActorItemChecksum		)
	{
		CLOG("bank","CBank::checkIsIncorrectData - wActorItemChecksum != m_wActorItemChecksum %s/%s",m_lpActor->m_strId,m_lpActor->m_strName);
		return	TRUE;
	}

	return	FALSE;
}

//
//	은행 초기화
void
CBank::open(CBankInfo *_lpBankInfo,cACTOR *_lpActor)
{
	BOOL	bIsOvertimePremiumItem	=	FALSE;
	BOOL	bIsRemoveExpiredItem	=	FALSE;

	m_lpActor	=	_lpActor;

	for	(int i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		cItem	*lpItem	=	(cItem	*)&_lpBankInfo->m_aItems[i];

		if	(lpItem->isBreedingRecordBook())
		{
			cPetDefine	*lpPetInfo	=	(cPetDefine	*)&lpItem->m_bCount;

			_lpActor->revisePetInfo(lpPetInfo);
		}
	}

	memcpy(&m_bankInfo,_lpBankInfo,sizeof(CBankInfo));

	{
		if (m_bankInfo.m_wRequitalCounter <= 2)
		{
			for (int i=0;i<dMAX_ITEM_IN_BANK;i++)
			{
				cItem	*lpItem	=	(cItem *)&m_bankInfo.m_aItems[i];

				if (lpItem->m_wBaseItem == 0xffff)
					continue;

				if (lpItem->m_wBaseItem >= g_im.m_iItemCount)
					continue;

				if (!lpItem->isPremiumItem() || lpItem->m_year == 0|| lpItem->isExtraItem())
					continue;

				lpItem->increaseMinute(60*24);

				bIsOvertimePremiumItem	=	TRUE;
			}

			m_bankInfo.m_wRequitalCounter	=	3;
		}
	}

	bIsRemoveExpiredItem	=	removeExpiredItemAndReviseIncorrectPrefix();

	if	(bIsOvertimePremiumItem)
		_lpActor->sendBankMessage(eMOB_OVER_TIME_ITEM_TERM_OF_VALIDATE);

	if	(bIsRemoveExpiredItem || bIsOvertimePremiumItem)
	{
		WDMSG_SAVEBANK	packet;

		packet.base.set(sizeof(packet),dWDMSG_SAVEBANK);
		packet.iActorSerial	=	_lpActor->m_iZoneSerial;
		memcpy(&packet.cBank,&m_bankInfo,sizeof(CBankInfo));

		strcpy(packet.strID,_lpActor->m_strId);

		g_socketDBC.SendPacket((char *)&packet,((cMSG_BASE_TYPE *)&packet)->wSize);
	}


	memcpy(_lpBankInfo,&m_bankInfo,sizeof(CBankInfo));
	memcpy(&m_firstBankInfo,&m_bankInfo,sizeof(CBankInfo));

	m_wActorItemChecksum=	_lpActor->getItemCheckSum();
	m_iOwnGold			=	_lpActor->m_iGold;
	memcpy(m_aInventoryItem	,_lpActor->m_aItems,sizeof(m_aInventoryItem));
	memcpy(m_aEquipment		,_lpActor->m_aEquip,sizeof(m_aEquipment));
	m_wInventorySize	=	_lpActor->getItemSlotCount();
	m_wBankSize			=	_lpActor->m_wBankSize;
	m_wBankCharge		=	_lpActor->m_wBankCharge;

	m_llOpenGoldSum		=	getGoldSum();

	m_lpActor			=	_lpActor;
}

//
//  시간이 지난 아이템 제거
BOOL
CBank::removeExpiredItemAndReviseIncorrectPrefix()
{
	BOOL	bIsCorrectIncorrectData	=	FALSE,bIsReviseIncorrectPrefix=FALSE;
	int		iRemoveExpiredItemCount	=	0;

	for (int i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		cItem	*lpItem	=	(cItem *)&m_bankInfo.m_aItems[i];

		if (lpItem->m_wBaseItem == 0xffff)
			continue;

		if (lpItem->m_wBaseItem >= g_im.m_iItemCount)
		{
			bIsCorrectIncorrectData	=	TRUE;
			lpItem->reset();
			continue;
		}

		if (lpItem->isExpiredItem())
		{
			lpItem->reset();
			iRemoveExpiredItemCount++;
		}
		
		if	(lpItem->reviseIncorrectData(FALSE)	==	eRIR_REVISE_INCORRECT_PREFIX)
			bIsReviseIncorrectPrefix	=	TRUE;
	}

	if	(bIsReviseIncorrectPrefix)
		m_lpActor->sendRegistServerMessage(eRSM_REVISE_INCORRECT_PREFIX_IN_BANK);

	if	(iRemoveExpiredItemCount)
		m_lpActor->sendBankMessage(eMOB_REMOVE_EXPIRED_ITEM);

	if	(bIsCorrectIncorrectData || iRemoveExpiredItemCount || bIsReviseIncorrectPrefix)
		return	TRUE;

	return	FALSE;
}

//
//	체크 섬
DWORD
CBank::getCheckSum()
{
	int		i,iCheckSum;

	m_dwCheckSum	=	0;

	for (i=0;i<(int)strlen(m_bankInfo.m_strId);i++)
		m_dwCheckSum	+=	m_bankInfo.m_strId[i];

	m_dwCheckSum	+=	m_bankInfo.m_dwGold;
	m_dwCheckSum	+=	m_bankInfo.m_dwSaveCounter;
	m_dwCheckSum	+=	m_bankInfo.m_wRequitalCounter;

	for (i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		iCheckSum		=	((cItem *)&m_bankInfo.m_aItems[i])->getCheckSum();

		if (iCheckSum)
			m_dwCheckSum	+=	iCheckSum+(i+1)*3;
	}

	m_dwCheckSum	+=	m_iOwnGold;
	m_dwCheckSum	+=	m_wInventorySize;
	m_dwCheckSum	+=	m_wBankSize;
	m_dwCheckSum	+=	m_wBankCharge;

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		iCheckSum		=	m_aEquipment[i].getCheckSum();

		if (iCheckSum)
			m_dwCheckSum	+=	iCheckSum+(i+1)*3;
	}

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		iCheckSum		=	m_aInventoryItem[i].getCheckSum();

		if (iCheckSum)
			m_dwCheckSum	+=	iCheckSum+(i+1)*3;
	}
	
	return	m_dwCheckSum;
}

//
//	아이템을 은행에 저장
int
CBank::storeItemToTheBank(int _iInventorySlot,int _iBankSlot,DWORD _dwChecksum)
{
	if	(checkIsIncorrectData(_dwChecksum) || _iBankSlot >= m_wBankSize)
		return	eRBW_FAILED;

	cItem	*lpSaveItem	=	NULL;
	cItem	*lpBankSlot	=	(cItem *)&m_bankInfo.m_aItems[_iBankSlot];

	if	(lpBankSlot->m_wBaseItem!=	0xffff)	
		return	eRBW_FAILED;

	if	(_iInventorySlot	>= dBORDER_OF_ITEM_AND_EQUIPMENT)
	{
		_iInventorySlot	-=	dBORDER_OF_ITEM_AND_EQUIPMENT;

		if	(_iInventorySlot	>=	dEQUIPMENT_PART_COUNT)	
			return	eRBW_FAILED;

		lpSaveItem		=	&m_aEquipment[_iInventorySlot];
	}
	else
	{
		if (_iInventorySlot	>=	m_wInventorySize	)	return	eRBW_FAILED;

		lpSaveItem		=	&m_aInventoryItem[_iInventorySlot];
	}

	cBasicItem	*lpBasicItem	=	lpSaveItem->getBasicItem();

	if	(!lpBasicItem)
		return	eRBW_FAILED;

	if	(lpSaveItem->isCanNotStoreBankItem())
		return	eRBW_CAN_NOT_STORE_ITEM;

	lpBankSlot->copy(lpSaveItem);
	lpSaveItem->reset();

	return	eRBW_SUCCESS;
}

LONGLONG
CBank::getGoldSum()
{
	int			i;
	LONGLONG	llGoldSum	=	0;

	for (i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		cItem		*lpItem		=	(cItem	*)&m_bankInfo.m_aItems[i];
		cBasicItem	*lpBasicItem=	lpItem->getBasicItem();

		if	(lpBasicItem	==	NULL)
			continue;

		if	(lpBasicItem->isGoldBar())
			llGoldSum	+=	lpItem->m_bCount;
	}

	llGoldSum	*=	100000000;
	llGoldSum	+=	m_bankInfo.m_dwGold;

	return	llGoldSum;
}

int
CBank::getGoldBarCount()
{
	int		i,iGoldBarCount=0;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem		*lpItem			=	&m_aInventoryItem[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(lpBasicItem	==	NULL)
			continue;

		if	(lpBasicItem->isGoldBar())
			iGoldBarCount+=lpItem->m_bCount;
	}

	for (i=0;i<m_lpActor->m_bf6ExtraInventorySize;i++)
	{
		cItem		*lpItem			=	&m_lpActor->m_aExtraInventory[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(lpBasicItem	==	NULL)
			continue;

		if	(lpBasicItem->isGoldBar())
			iGoldBarCount+=lpItem->m_bCount;
	}

	return	iGoldBarCount;
}
//
//	인벤토리에서 아이템 검색
cItem*
CBank::getItemInInventory(int _iBaseItem)
{
	int		i;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem		*lpItem		=	&m_aInventoryItem[i];

		if	(lpItem->m_wBaseItem==	_iBaseItem)
			return	lpItem;
	}

	for (i=0;i<m_lpActor->m_bf6ExtraInventorySize;i++)
	{
		cItem		*lpItem		=	&m_lpActor->m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem==	_iBaseItem)
			return	lpItem;
	}

	return	NULL;
}

//
//	배지의 수 계산
int
CBank::getBadgeCountInInventory()
{
	int		i,iBadgeCount=0;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem		*lpItem			=	&m_aInventoryItem[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(lpBasicItem	==	NULL)
			continue;

		if	(lpBasicItem->m_attr.isBadge)
			iBadgeCount++;
	}

	for (i=0;i<m_lpActor->m_bf6ExtraInventorySize;i++)
	{
		cItem		*lpItem			=	&m_lpActor->m_aExtraInventory[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(lpBasicItem	==	NULL)
			continue;

		if	(lpBasicItem->m_attr.isBadge)
			iBadgeCount++;
	}

	return	iBadgeCount;
}

//
//	아이템을 은행에서 찾아옴 
int
CBank::withdrawItemFromTheBank(int _iInventorySlot,int _iBankSlot,DWORD _dwChecksum, cACTOR* _lpActor)
{
	if	(checkIsIncorrectData(_dwChecksum) || _iBankSlot >= m_wBankSize)
		return	eRBW_FAILED;

	cItem	*lpLoadSlot	=	NULL;
	cItem	*lpBankItem	=	(cItem *)&m_bankInfo.m_aItems[_iBankSlot];

	if	(_iInventorySlot	>= dBORDER_OF_ITEM_AND_EQUIPMENT)
	{
		_iInventorySlot	-=	dBORDER_OF_ITEM_AND_EQUIPMENT;

		if	(_iInventorySlot	>=	dEQUIPMENT_PART_COUNT)
			return	eRBW_FAILED;

		lpLoadSlot		=	&m_aEquipment[_iInventorySlot];
	}
	else
	{
		if	(_iInventorySlot	>=	m_wInventorySize	)
			return	eRBW_FAILED;

		lpLoadSlot		=	&m_aInventoryItem[_iInventorySlot];
	}
	
	if (lpBankItem->m_wBaseItem == 0xffff)
		return	eRBW_FAILED;
	if (lpLoadSlot->m_wBaseItem != 0xffff)
		return	eRBW_FAILED;

	cBasicItem	*lpBasicItem	=	lpBankItem->getBasicItem();

	if	(lpBasicItem )
	{
		if	(lpBasicItem->m_attr.isCanNotOwnSameItem)
			if	(getItemInInventory(lpBankItem->m_wBaseItem))
				return	eRBW_CAN_NOT_OWN_SAME_ITEM;

		if	(lpBasicItem->m_attr.isBadge)
			if	(getBadgeCountInInventory()	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
				return	eRBW_TOO_MANY_BADGE_ITEM;
	}
	
	if	(lpBankItem->isBreedingRecordBook())
	{
		char						*lpData	=	(char *)lpBankItem;
		cBreedingRecordBookDefine	*lpBook	=	(cBreedingRecordBookDefine	*)(lpData+6);

		int		iAvatarIndex,iAvatarLevel;

		if	(lpBook->isAvailInfo(lpBankItem->m_dwSerial))
		{
			iAvatarIndex	=	lpBook->getAvatarIndex();
			iAvatarLevel	=	lpBook->getAvatarLevel();
		}
		else
		{
			cPetDefine	petInfo;

			memcpy(&petInfo,&lpBankItem->m_bCount,sizeof(cPetDefine)-4);
			petInfo.m_dwExperience		=	lpBankItem->m_dwSerial;
			iAvatarIndex				=	petInfo.m_bf2TamerSlot;
			iAvatarLevel				=	0;
		}

		if	(!_lpActor)
			return	eRBW_FAILED;
	
		if	(_lpActor->m_iLevel < iAvatarLevel || _lpActor->m_wAvatarIndex	!= iAvatarIndex)
			return eRBW_IS_NOT_OWN_ITEM;

		if	(_lpActor->m_wJob != dJOB_BEAST_TAMER && _lpActor->m_wJob != dJOB_SUMMONER)
			return eRBW_IS_NOT_OWN_ITEM;
	}

	lpLoadSlot->copy(lpBankItem);
	lpBankItem->reset();

	return	eRBW_SUCCESS;
}
//
//	돈 저장하기
int
CBank::storeGoldToTheBank(int _iGold,DWORD _dwChecksum)
{
	if	(checkIsIncorrectData(_dwChecksum) || m_iOwnGold	< _iGold)
		return	-1;

	LONGLONG	llGold	=	m_bankInfo.m_dwGold;

	llGold	+=	_iGold;

	if	(llGold > dMAX_OWN_BANK_GOLD)
	{
		if (m_bankInfo.m_dwGold	>=	dMAX_OWN_BANK_GOLD)
		{
			m_lpActor->sendRegistServerMessage(eRSM_OWN_BANK_GOLD_LIMIT,eSM_SYSTEM);

			return	0;
		}

		_iGold	=	dMAX_OWN_BANK_GOLD-m_bankInfo.m_dwGold;
	}

	if	(_iGold<	0)
		return	-1;

	m_iOwnGold			-=	_iGold;
	m_bankInfo.m_dwGold	+=	_iGold;
		
	return	_iGold;
}

//
//	돈 꺼내기
BOOL
CBank::withdrawGoldFromTheBank(int _iGold,DWORD _dwChecksum,int *_lpiWithDrawGold)
{
	if (checkIsIncorrectData(_dwChecksum))
	{
		CLOG("bank","돈찾기 : %s/%s 체크섬 실패",m_lpActor->m_strId,m_lpActor->m_strName);

		return	FALSE;
	}

	if (m_bankInfo.m_dwGold	< (DWORD)_iGold )
	{
		CLOG("bank","돈찾기 : %s/%s[%d,%d] 은행에 남은 돈보다 찾으려는 돈이 많다.",m_lpActor->m_strId,m_lpActor->m_strName,m_bankInfo.m_dwGold,_iGold);

		return	FALSE;
	}

	LONGLONG	llGold	=	(LONGLONG	)m_iOwnGold;

	llGold	+=	 _iGold;

	if	(llGold > dMAX_OWN_GOLD)
	{
		_iGold				=	dMAX_OWN_GOLD-m_iOwnGold;

		*_lpiWithDrawGold	=	_iGold;

		if	(m_iOwnGold	>=	dMAX_OWN_GOLD)
		{
			m_lpActor->sendRegistServerMessage(eRSM_OWN_GOLD_LIMIT,eSM_SYSTEM);
			return	TRUE;
		}

		m_bankInfo.m_dwGold	-=	_iGold;
		m_iOwnGold			=	dMAX_OWN_GOLD;

		return	TRUE;
	}

	m_iOwnGold			+=	_iGold;
	m_bankInfo.m_dwGold	-=	_iGold;
	*_lpiWithDrawGold	=	_iGold;

	return	TRUE;
}

BOOL
CBank::moveBankItem(int _iSlot1,int _iSlot2,DWORD _dwChecksum)
{
	if	(checkIsIncorrectData(_dwChecksum) ||  _iSlot1	>=	m_wBankSize || _iSlot2 >= m_wBankSize )
		return	FALSE;

	if	(_iSlot1	==	_iSlot2 )
		return	FALSE;

	cItem	*lpSlot1,*lpSlot2;

	lpSlot1	=	(cItem*)&m_bankInfo.m_aItems[_iSlot1];
	lpSlot2	=	(cItem*)&m_bankInfo.m_aItems[_iSlot2];

	if (lpSlot1->m_wBaseItem == 0xffff)	return	FALSE;

	cItem	tempItem;

	tempItem.copy(lpSlot1);
	lpSlot1->copy(lpSlot2);
	lpSlot2->copy(&tempItem);

	return	TRUE;
}

BOOL
CBank::moveInventoryItem(int _iSlot1,int _iSlot2,DWORD _dwChecksum)
{
	if	(checkIsIncorrectData(_dwChecksum) ||  _iSlot1	>=	m_wInventorySize || _iSlot2 >= m_wInventorySize)
		return	FALSE;

	if	(_iSlot1	==	_iSlot2 )
		return	FALSE;

	cItem	*lpSlot1,*lpSlot2;

	lpSlot1	=	&m_aInventoryItem[_iSlot1];
	lpSlot2	=	&m_aInventoryItem[_iSlot2];

	if	(lpSlot1->m_wBaseItem == 0xffff)
		return	FALSE;

	cItem	tempItem;

	tempItem.copy(lpSlot1);
	lpSlot1->copy(lpSlot2);
	lpSlot2->copy(&tempItem);

	return	TRUE;
}

//
//	은행 거래 종료 요청
BOOL
CBank::askFinishBankTransaction(DWORD _dwChecksum)
{
	if	(checkIsIncorrectData(_dwChecksum))
		return	FALSE;

	DWORD	dwCharge	=	getBankTransactionCharge();

	if	((int)dwCharge	>	m_iOwnGold)
		return FALSE;

	{
		WDMSG_SAVEBANK	packet;

		packet.base.set(sizeof(packet),dWDMSG_SAVEBANK);
		packet.iActorSerial	=	m_lpActor->m_iZoneSerial;
		memcpy(&packet.cBank,&m_bankInfo,sizeof(CBankInfo));

		strcpy(packet.strID,m_lpActor->m_strId);

		g_socketDBC.SendPacket((char *)&packet,((cMSG_BASE_TYPE *)&packet)->wSize);
	}

	return	TRUE;
}

cItem*
CBank::getSameItemInBank(cItem *_lpItem,CBankInfo *_lpBankInfo)
{
	for (int i=0;i<m_wBankSize;i++)
	{
		cItem *lpItem	= (cItem *)&_lpBankInfo->m_aItems[i];

		if	(lpItem->m_wBaseItem == 0xffff)
			continue;

		if	(lpItem->isExactlySameItem(_lpItem))
			return	lpItem;
	}

	return	NULL;
}

BOOL
CBank::sendWorkResultLog(int _iStoreGold)
{
	int	iWithdrawGold	=	-_iStoreGold;

	LOG_ITEM_BANK_IN	storeItemPacket;
	LOG_ITEM_BANK_OUT	withdrawItemPacket;

	storeItemPacket.base.set(sizeof(storeItemPacket),dLOG_ITEM_BANK_IN);
	withdrawItemPacket.base.set(sizeof(withdrawItemPacket),dLOG_ITEM_BANK_OUT);

	storeItemPacket.bf1IsOperator	=	FALSE;
	withdrawItemPacket.bf1IsOperator=	FALSE;

	if	(m_lpActor->isTester())
	{
		storeItemPacket.bf1IsOperator	=	TRUE;
		withdrawItemPacket.bf1IsOperator=	TRUE;
	}

	withdrawItemPacket.logTime.yy	=	storeItemPacket.logTime.yy	=	g_currentTime.m_wYear%100;
	withdrawItemPacket.logTime.mm	=	storeItemPacket.logTime.mm	=	g_currentTime.m_wMonth;
	withdrawItemPacket.logTime.dd	=	storeItemPacket.logTime.dd	=	g_currentTime.m_wDay;
	withdrawItemPacket.logTime.hour	=	storeItemPacket.logTime.hour=	g_currentTime.m_wHour;
	withdrawItemPacket.logTime.min	=	storeItemPacket.logTime.min	=	g_currentTime.m_wMinute;
	storeItemPacket.iGold			=	0;
	withdrawItemPacket.iGold		=	0;

	if	(_iStoreGold	>	0)
		storeItemPacket.iGold	=	_iStoreGold;

	if	(iWithdrawGold	>	0)
		withdrawItemPacket.iGold=	iWithdrawGold;

	strcpy(withdrawItemPacket.strId,m_lpActor->m_strId);
	strcpy(withdrawItemPacket.strName,m_lpActor->m_strName);
	strcpy(storeItemPacket.strId,m_lpActor->m_strId);
	strcpy(storeItemPacket.strName,m_lpActor->m_strName);
	storeItemPacket.wFieldUniqueIndex		=	m_lpActor->m_lpField->m_iUniqueSerial;
	withdrawItemPacket.wFieldUniqueIndex	=	m_lpActor->m_lpField->m_iUniqueSerial;
	storeItemPacket.bf15Count				=	0;
	withdrawItemPacket.bf15Count			=	0;

	CClient	*lpClient	=	m_lpActor->getClient();

	if (lpClient)
	{
		memcpy(withdrawItemPacket.strIP,lpClient->ip,dIP_LENGTH);
		memcpy(storeItemPacket.strIP,lpClient->ip,dIP_LENGTH);
	}

	cItem	*lpItem;

	for (int i=0;i<m_wBankSize;i++)
	{
		lpItem	= (cItem *)&m_firstBankInfo.m_aItems[i];

		if (lpItem->m_wBaseItem != 0xffff)
		{
			if (getSameItemInBank(lpItem,&m_bankInfo) == NULL)	//	찾아간 아이템
			{
				memcpy(&withdrawItemPacket.cItem[withdrawItemPacket.bf15Count],lpItem,sizeof(cItem));

				withdrawItemPacket.bf15Count++;

				if	(withdrawItemPacket.bf15Count		>=	dMAX_STORE_ITEM_COUNT_BY_LOG)
				{
					withdrawItemPacket.base.wSize	=	sizeof(withdrawItemPacket);

					g_LogQue.addPacket((ALL_LOGMSG *)&withdrawItemPacket);

					withdrawItemPacket.bf15Count	=	0;
				}
			}
		}

		lpItem	= (cItem *)&m_bankInfo.m_aItems[i];

		if (lpItem->m_wBaseItem != 0xffff)
		{
			if (getSameItemInBank(lpItem,&m_firstBankInfo) == NULL)	//	추가한 아이템
			{
				memcpy(&storeItemPacket.cItem[storeItemPacket.bf15Count],lpItem,sizeof(cItem));

				storeItemPacket.bf15Count++;

				if	(storeItemPacket.bf15Count		>=	dMAX_STORE_ITEM_COUNT_BY_LOG)
				{
					storeItemPacket.base.wSize	=	sizeof(storeItemPacket);

					g_LogQue.addPacket((ALL_LOGMSG *)&storeItemPacket);

					storeItemPacket.bf15Count	=	0;
				}
			}
		}
	}

	if (storeItemPacket.bf15Count > dMAX_STORE_ITEM_COUNT_BY_LOG || withdrawItemPacket.bf15Count > dMAX_STORE_ITEM_COUNT_BY_LOG)
	{
		CLOG("itemLog","bank Item log failed %d,%d",storeItemPacket.bf15Count,withdrawItemPacket.bf15Count);
	}
	if (storeItemPacket.bf15Count || storeItemPacket.iGold)
	{
		storeItemPacket.base.wSize	=	sizeof(storeItemPacket)-sizeof(storeItemPacket.cItem)+sizeof(cItem)*storeItemPacket.bf15Count;
		g_LogQue.addPacket((ALL_LOGMSG *)&storeItemPacket);
	}

	if (withdrawItemPacket.bf15Count || withdrawItemPacket.iGold)
	{
		withdrawItemPacket.base.wSize	=	sizeof(withdrawItemPacket)-sizeof(withdrawItemPacket.cItem)+sizeof(cItem)*withdrawItemPacket.bf15Count;
		g_LogQue.addPacket((ALL_LOGMSG *)&withdrawItemPacket);
	}

	return	TRUE;
}

//
//	은행거래 수수료
DWORD
CBank::getBankTransactionCharge()
{
	cItem	*lpItem;

	DWORD	dwCharge	=	0;

	for (int i=0;i<m_wBankSize;i++)
	{
		lpItem	= (cItem *)&m_firstBankInfo.m_aItems[i];
		cBasicItem* lpBasicItem = lpItem->getBasicItem();

		if(lpItem->m_wBaseItem == 0xffff || (lpBasicItem && lpBasicItem->isGoldBar()) )		// 09.08.28
			continue;

			if (getSameItemInBank(lpItem,&m_bankInfo) == NULL)	//	찾아간 아이템
			{
				DWORD	dwPrice	=	lpItem->getPrice(lpItem->m_bCount);

				if	(lpItem->isExtraItem())
					dwPrice		=	0;

				LONGLONG	llMoney	=	dwPrice;
				llMoney				*=	m_wBankCharge;	
				llMoney				/=	10000;

				dwCharge		=	(DWORD)(dwCharge+llMoney);
			}
	}

	return	dwCharge;
}


//
//	은행 거래 종료
BOOL
CBank::finishBankTransaction()
{
	DWORD	dwCharge	=	getBankTransactionCharge();
	m_iOwnGold			-=	dwCharge;

	LONGLONG	llBankGold	=	getGoldSum();

	if	(llBankGold	!=	m_llOpenGoldSum)
		m_lpActor->sendBankGoldLog(llBankGold);

	if	(dwCharge)
	{
		LONGLONG	llCharge	=	dwCharge;
		m_lpActor->updateGoldLog(-llCharge,eSPENT_GOLD_BY_BANK_TRANSACTUION_CHARGE);
	}

	int	iStoreGold		=	m_lpActor->m_iGold-m_iOwnGold;

	m_lpActor->m_iGold	=	max(m_iOwnGold,0);

	int	iOldGoldBarCount=	m_lpActor->getItemCount(dITEM_INDEX_GOLD_BAR, FALSE);

	memcpy(m_lpActor->m_aItems,m_aInventoryItem	,sizeof(m_aInventoryItem));
	memcpy(m_lpActor->m_aEquip,m_aEquipment		,sizeof(m_aEquipment));

	sendWorkResultLog(iStoreGold);

	m_lpActor->immediatelySendSaveDataToDBMemory(TRUE);

	{
		if	(iStoreGold)
		{
			if	(iStoreGold	>	0)
				m_lpActor->m_llTradeSpentGold	-=	iStoreGold;	//	플러스이니 다시 마이너스로 변경
			else
			if	(iStoreGold	<	0)
				m_lpActor->m_llTradeGettingGold	-=	iStoreGold;	//	마이너스니 다시 마이너스 해서 플러스로..
		}

		int	iGoldBarCount	=	m_lpActor->getItemCount(dITEM_INDEX_GOLD_BAR, FALSE);
		int	iDeltaGoldBar	=	iOldGoldBarCount-iGoldBarCount;

		if	(iDeltaGoldBar)
		{
			LONGLONG	llDeltaGold	=	iDeltaGoldBar;
			llDeltaGold	*=	100000000;
			
			if	(llDeltaGold	>	0)	
				m_lpActor->m_llTradeSpentGold	-=	llDeltaGold;
			else
			if	(llDeltaGold	<	0)
				m_lpActor->m_llTradeGettingGold	-=	llDeltaGold;
		}
	}

	return	TRUE;
}

//
//
CBankManager::CBankManager()
{
	m_pBank	=	NULL;
}

CBankManager::~CBankManager()
{
	if (m_pBank)
	{
		pKILL(m_pBank);

		DeleteCriticalSection(&m_cs);
	}
}

//
//	초기화
void
CBankManager::init(int _iBankCount)
{
	m_iBankSize	=	_iBankCount;
	m_iBankCount=	0;

	m_pBank		=	new CBank [m_iBankSize];

	FPInitCS(&m_cs);
}

//
//	연다.
int
CBankManager::open(CBankInfo *_lpBankInfo,cACTOR *_lpActor)
{
	mCS(m_cs);

	int	iFreeBankIndex	=	getFreeBankIndex();

	if (iFreeBankIndex	==	0xffff)
	{
		return	0xffff;
	}

	CBank	*lpBank		=	&m_pBank[iFreeBankIndex];

	lpBank->open(_lpBankInfo,_lpActor);
	lpBank->m_wSerial	=	iFreeBankIndex;

	return	iFreeBankIndex;
}

//
//	빈 은행 찾기
void
CBankManager::checkIncorrectBank()
{
	int i;

	for (i=0;i<m_iBankSize;i++)
	{
		if (m_pBank[i].m_wSerial	==	0xffff)
			continue;
		
		if (!m_pBank[i].m_lpActor || m_pBank[i].m_lpActor->m_wBankSerial	!=	i)
			closeBank(i);
	}
}

//
//	빈 은행 찾기
int
CBankManager::getFreeBankIndex()
{
	checkIncorrectBank();

	for (int i=0;i<m_iBankSize;i++)
	{
		if (m_pBank[i].m_wSerial	==	0xffff)
			return i;
	}

	return	0xffff;
}

//
//	은행 닫기
void
CBankManager::closeBank(int _iBankIndex,BOOL _bIsForced)
{
	CBank	*lpBank	=	&m_pBank[_iBankIndex];

	if (lpBank->m_wSerial	==	0xffff)
		return;

	mCS(m_cs);

	lpBank->reset();
	m_iBankCount--;
}