#include "cFIELD.H"
#include "cGAME.H"
#include "debugCode.H"

//
//	은행에 아이템 보관
void
cFIELD::storeItemToTheBank(CG_STORE_ITEM_TO_THE_BANK *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	lpActor->storeItemToTheBank(_lpPacket->wBankSerial,_lpPacket->wInventorySlot,_lpPacket->wBankSlot,_lpPacket->dwCheckSum);
}

void
cFIELD::storeGoldToTheBank(CG_STORE_GOLD_TO_THE_BANK *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	lpActor->storeGoldToTheBank(_lpPacket->wBankSerial,_lpPacket->iGold,_lpPacket->dwCheckSum);
}

void
cFIELD::withdrawItemFromTheBank(CG_WITHDRAW_ITEM_FROM_THE_BANK *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->withdrawItemFromTheBank(_lpPacket->wBankSerial,_lpPacket->wInventorySlot,_lpPacket->wBankSlot,_lpPacket->dwCheckSum);
}

void
cFIELD::withdrawGoldFromTheBank(CG_WITHDRAW_GOLD_FROM_THE_BANK *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	lpActor->withdrawGoldFromTheBank(_lpPacket->wBankSerial,_lpPacket->iGold,_lpPacket->dwCheckSum);
}

void
cFIELD::moveBankItem(CG_MOVE_BANK_ITEM *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	lpActor->moveBankItem(_lpPacket->wBankSerial,_lpPacket->wPos1,_lpPacket->wPos2,_lpPacket->dwCheckSum);
}

void
cFIELD::moveInventoryItemWhenBankTransaction(CG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	lpActor->moveInventoryItemWhenBankTransaction(_lpPacket->wBankSerial,_lpPacket->wPos1,_lpPacket->wPos2,_lpPacket->dwCheckSum);
}

void
cFIELD::finishBankTransaction(CG_FINISH_BANK_TRANSACTION *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->askFinishBankTransaction(_lpPacket->wBankSerial,_lpPacket->dwCheckSum);
}

void
cFIELD::cancelBankTransaction(CG_CANCEL_BANK_TRANSACTION *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	lpActor->cancelBankTransaction(FALSE);
}