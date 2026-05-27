#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "CGame.H"
#include "CDataSave.H"
#include "CBank.h"
#include "packetManager.h"

//
//	뱅크 오픈 요청
BOOL
cACTOR::askOpenBank()
{
	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
		return	FALSE;

	if	(isTester()	==	FALSE)
		if	(m_wWaitBankDataTime	|| m_wWaitBankDataSaveResultTime)
		{
			sendBankMessage(eMOB_WAIT_TO_BANK_OPEN_PERIOD_MESSAGE);
			return	FALSE;
		}

	if	(m_wBankSerial	!=	0xffff)
	{
		sendBankMessage(eMOB_WAIT_TO_BANK_OPEN_PERIOD_MESSAGE);
		m_wWaitBankDataTime	=	dWAIT_BANK_DATE_PERIOD;
		cancelBankTransaction(FALSE);

		return	TRUE;
	}

	m_wWaitBankDataTime	=	dWAIT_BANK_DATE_PERIOD;

	if	(g_bankManager.getFreeBankIndex() == 0xffff)
	{
		sendBankMessage(eMOB_TOO_MANY_BANK_TRANSACTION);

		return	FALSE;
	}

	if	(m_bf1IsProcessRebirth	==	FALSE)
		sendBankMessage(eMOB_WAIT_BANK_DATA);

	{
		WDMSG_BANKOPEN	packet;

		packet.base.set(sizeof(packet),dWDMSG_BANKOPEN);
		packet.iActorSerial	=	m_iZoneSerial;

		strcpy(packet.strID,m_strId);

		g_socketDBC.SendPacket((char *)&packet,((cMSG_BASE_TYPE *)&packet)->wSize);
	}

	return	TRUE;
}

BOOL
cACTOR::operateRebirthBankWork(CBankInfo *_lpBank)
{
	int	iChangeDataCount	=	0;

	for (int i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		cItem	*lpItem	=	(cItem *)&_lpBank->m_aItems[i];

		if	(lpItem->m_wBaseItem == 0xffff || !lpItem->isBreedingRecordBook())
			continue;

		cPetDefine	*lpPetInfo	=	(cPetDefine	*)&lpItem->m_bCount;

		if	(lpPetInfo->m_bf2TamerSlot	!=	m_wAvatarIndex)
			continue;

		lpPetInfo->m_bf10Level		=	1;
		lpItem->m_dwSerial			=	0;

		iChangeDataCount++;
	}

	if	(iChangeDataCount)
	{
		WDMSG_SAVEBANK	packet;

		packet.base.set(sizeof(packet),dWDMSG_SAVEBANK);
		packet.iActorSerial	=	m_iZoneSerial;

		memcpy(&packet.cBank,_lpBank,sizeof(CBankInfo));

		strcpy(packet.strID,m_strId);

		g_socketDBC.SendPacket((char *)&packet,((cMSG_BASE_TYPE *)&packet)->wSize);

		//	은행 데이터 저장후 성공 결과가 날아와야 전생 진행
	}
	else	//	아이템 데이터 바뀐게 없으면 그냥 전생 시킨다.
		rebirth(TRUE);

	return	TRUE;
}

//
//	DB에서 은행 정보가 왔다.
BOOL
cACTOR::openBank(CBankInfo *_lpBank)
{
	if	(!isOwnedTitle(eTN_BANK_CLIENT))
	{
		if	(m_bf1IsProcessRebirth)
			rebirth(TRUE);
		else
			sendBankMessage(eMOB_NOT_BANK_CLIENT);

		return	FALSE;
	}

	if	(_lpBank->m_dwSaveCounter == 0)
	{
		if	(m_bf1IsProcessRebirth)
			rebirth(TRUE);
		else
			sendBankMessage(eMOB_FAILED);

		return	FALSE;
	}

	if	(m_bf1IsProcessRebirth)
	{
		operateRebirthBankWork(_lpBank);

		return	TRUE;
	}

	int	iBankSerial	=	g_bankManager.open(_lpBank,this);

	if	(iBankSerial	== 0xffff)
	{
		sendBankMessage(eMOB_TOO_MANY_BANK_TRANSACTION);

		return	FALSE;
	}

	m_wBankSerial		=	iBankSerial;

	SG_OPEN_BANK	packet;

	packet.base.set(sizeof(packet),dSG_OPEN_BANK);
	packet.wBankSerial	=	iBankSerial;
	packet.wBankSize	=	m_wBankSize;
	packet.wBankCharge	=	m_wBankCharge;
	
	memcpy(&packet.bankInfo,_lpBank,sizeof(CBankInfo));

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

//
//	뱅크 오픈 요청
BOOL
cACTOR::closeBank()
{
	if	(m_wBankSerial == 0xffff	||	m_wWaitBankDataSaveResultTime)
		return	FALSE;

	g_bankManager.closeBank(m_wBankSerial);

	m_wBankSerial	=	0xffff;

	return	TRUE;
}

//
//	은행 거래 취소
BOOL
cACTOR::cancelBankTransaction(BOOL _bIsSendCancelMessageToClient)
{
	if	(m_wWaitBankDataSaveResultTime)
		return	FALSE;

	closeBank();

	if	(_bIsSendCancelMessageToClient)
		sendBankMessage(eMOB_CANCELED_BY_NOT_MATCHED_DATA_WITH_SERVER);

	return	FALSE;
}

//
//	은행에 아이템 저장
BOOL
cACTOR::storeItemToTheBank(int _iBankSerial,int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum)
{
	CBank	*lpBank				=	g_bankManager.getBank(_iBankSerial);

	if	(lpBank	== NULL || _iBankSerial != m_wBankSerial					)
		return	cancelBankTransaction();

	int	iResult	=	lpBank->storeItemToTheBank(_iInventorySlot,_iBankSlot,_dwCheckSum);

	switch(iResult)
	{
	case	eRBW_FAILED:
		return	cancelBankTransaction();

	case	eRBW_CAN_NOT_STORE_ITEM:
		sendBankMessage(eMOB_CAN_NOT_STORE_ITEM);
		return	TRUE;

	case	eRBW_TOO_MANY_GOLD_BAR:
		sendBankMessage(eMOB_TOO_MANY_GOLD_BAR);	
		return	TRUE;
	}

	SG_STORE_ITEM_TO_THE_BANK	packet;

	packet.base.set(sizeof(packet),dSG_STORE_ITEM_TO_THE_BANK);

	packet.dwCheckSum		=	lpBank->m_dwCheckSum;
	packet.wInventorySlot	=	_iInventorySlot;
	packet.wBankSlot		=	_iBankSlot;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

BOOL
cACTOR::storeGoldToTheBank(int _iBankSerial,int _iGold,DWORD _dwCheckSum)
{
	CBank	*lpBank	=	g_bankManager.getBank(_iBankSerial);

	if (lpBank	== NULL || _iBankSerial != m_wBankSerial)
		return	cancelBankTransaction();

	int	iStoreGold	=	lpBank->storeGoldToTheBank(_iGold,_dwCheckSum);

	if	(iStoreGold	<	0	)
		return	cancelBankTransaction();
	
	if	(iStoreGold	==	0)
		return	TRUE;

	SG_STORE_GOLD_TO_THE_BANK	packet;

	packet.base.set(sizeof(packet),dSG_STORE_GOLD_TO_THE_BANK);

	packet.dwCheckSum		=	lpBank->m_dwCheckSum;
	packet.iGold			=	iStoreGold;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

BOOL
cACTOR::withdrawItemFromTheBank(int _iBankSerial,int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum)
{
	CBank	*lpBank	=	g_bankManager.getBank(_iBankSerial);

	if (lpBank	== NULL || _iBankSerial != m_wBankSerial							)
		return	cancelBankTransaction();
	
	int	iWithdrawResult	=	lpBank->withdrawItemFromTheBank(_iInventorySlot,_iBankSlot,_dwCheckSum, this);


	switch(iWithdrawResult)
	{
	case eRBW_CAN_NOT_OWN_SAME_ITEM:		
		sendBankMessage(eMOB_CAN_OWN_SAME_ITEM);
		return	TRUE;
		
	case eRBW_TOO_MANY_BADGE_ITEM:
		sendBankMessage(eMOB_TOO_MANY_BADGE_ITEM);
		return	TRUE;
		
	case	eRBW_IS_NOT_OWN_ITEM:
		sendBankMessage(eMOB_IS_NOT_OWN_ITEM);
		return	TRUE;

	case eRBW_FAILED:
		return	cancelBankTransaction();
	case	eRBW_TOO_MANY_GOLD_BAR:
		sendBankMessage(eMOB_PLAYER_HAVE_TOO_MANY_GOLD_BAR);
		return	TRUE;

		
	}

	SG_WITHDRAW_ITEM_FROM_THE_BANK	packet;

	packet.base.set(sizeof(packet),dSG_WITHDRAW_ITEM_FROM_THE_BANK);

	packet.dwCheckSum		=	lpBank->m_dwCheckSum;
	packet.wInventorySlot	=	_iInventorySlot;
	packet.wBankSlot		=	_iBankSlot;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

BOOL
cACTOR::withdrawGoldFromTheBank(int _iBankSerial,int _iGold,DWORD _dwCheckSum)
{
	CBank	*lpBank	=	g_bankManager.getBank(_iBankSerial);

	if	(lpBank	== NULL || _iBankSerial != m_wBankSerial		)
		return	cancelBankTransaction();

	int	iWithDrawGold	=	0;

	if	(!lpBank->withdrawGoldFromTheBank(_iGold,_dwCheckSum,&iWithDrawGold)	)	
		return	cancelBankTransaction();

	if	(iWithDrawGold)
	{
		SG_WITHDRAW_GOLD_FROM_THE_BANK	packet;

		packet.base.set(sizeof(packet),dSG_WITHDRAW_GOLD_FROM_THE_BANK);

		packet.dwCheckSum		=	lpBank->m_dwCheckSum;
		packet.iGold			=	iWithDrawGold;

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
	}

	return	TRUE;
}

BOOL
cACTOR::moveBankItem(int _iBankSerial,int _iPos1,int _iPos2,DWORD _dwCheckSum)
{
	CBank	*lpBank	=	g_bankManager.getBank(_iBankSerial);

	if	(lpBank	== NULL || _iBankSerial != m_wBankSerial)
		return	cancelBankTransaction();
	if	(!lpBank->moveBankItem(_iPos1,_iPos2,_dwCheckSum))
		return	cancelBankTransaction();

	SG_MOVE_BANK_ITEM	packet;

	packet.base.set(sizeof(packet),dSG_MOVE_BANK_ITEM);
	packet.wPos1		=	_iPos1;
	packet.wPos2		=	_iPos2;
	packet.dwCheckSum	=	lpBank->m_dwCheckSum;
	
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

BOOL
cACTOR::moveInventoryItemWhenBankTransaction(int _iBankSerial,int _iPos1,int _iPos2,DWORD _dwCheckSum)
{
	CBank	*lpBank	=	g_bankManager.getBank(_iBankSerial);

	if (lpBank	== NULL || _iBankSerial != m_wBankSerial	)
		return	cancelBankTransaction();
	if (!lpBank->moveInventoryItem(_iPos1,_iPos2,_dwCheckSum))
		return	cancelBankTransaction();

	SG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	packet;

	packet.base.set(sizeof(packet),dSG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION);
	packet.wPos1		=	_iPos1;
	packet.wPos2		=	_iPos2;
	packet.dwCheckSum	=	lpBank->m_dwCheckSum;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

//
//	은행 거래 종료
BOOL
cACTOR::askFinishBankTransaction(int _iBankSerial,DWORD _dwCheckSum)
{
	CBank	*lpBank	=	g_bankManager.getBank(_iBankSerial);

	if (getClient()	==	NULL								)
		return	cancelBankTransaction();
	if (lpBank	== NULL || _iBankSerial != m_wBankSerial	)
		return	cancelBankTransaction();
	if (!lpBank->askFinishBankTransaction(_dwCheckSum)		)
		return	cancelBankTransaction();

	m_wWaitBankDataSaveResultTime	=	dSYNC_FPS*60;	//	약 1분간 기다린다.

	return	TRUE;
}

//
//	은행 거래 종료
BOOL
cACTOR::finishBankTransaction()
{
	if	(m_bf1IsProcessRebirth)
	{
		rebirth(TRUE);

		return	TRUE;
	}

	CBank	*lpBank	=	g_bankManager.getBank(m_wBankSerial);

	if	(lpBank	== NULL						)
		return	cancelBankTransaction();
	if	(!lpBank->finishBankTransaction()	)
		return	cancelBankTransaction();

	m_wWaitBankDataSaveResultTime	=	0;

	buildPower();
	closeBank();

	sendBankMessage(eMOB_TRANSACTION_FINISH);

	m_wWaitBankDataSaveResultTime	=	0;

	checkIncorrectItem(eDIL_BANK);

	return	TRUE;
}

//
//	은행관련 메시지 전송
void
cACTOR::sendBankMessage(int _iMessage)
{
	SG_BANK_MESSAGE	packet;

	packet.base.set(sizeof(packet),dSG_BANK_MESSAGE);

	packet.wMessage	=	_iMessage;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}