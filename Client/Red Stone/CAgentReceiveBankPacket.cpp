#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CSkill.H"
#include "cMESSAGE.H"
#include "CBank.H"
#include "CWindowInterface.H"

void
cPACKET_HANDLER::ReceiveOpenBank(SERVER_PACKETS *_lpPacket)
{
	SG_OPEN_BANK					*lpPacket	=	&_lpPacket->mSG_OPEN_BANK;

	g_gwBank.open(&lpPacket->bankInfo,lpPacket->wBankSerial,lpPacket->wBankSize,lpPacket->wBankCharge);
//	CGamePlay::AddSystemMessage(WHITE,dMSG_RECEIVE_BANK_DATA);
}	//	cPACKET_HANDLER::ReceiveOpenBank(SERVER_PACKETS *_lpPacket)

void
cPACKET_HANDLER::ReceiveBankMessage(SERVER_PACKETS *_lpPacket)
{
	SG_BANK_MESSAGE					*lpPacket	=	&_lpPacket->mSG_BANK_MESSAGE;

	g_gwBank.m_wIsWaitSaveResult	=	FALSE;

	switch(lpPacket->wMessage)
	{
		case	eMOB_REMOVE_EXPIRED_ITEM						:
			CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_EXPIRED_ITEM_IN_BANK);
			break;
		case	eMOB_CAN_NOT_STORE_ITEM							:	//	은행에 보관 할 수 없는 아이템
			CGamePlay::AddSystemMessage(WHITE,dMSG_CAN_NOT_STORE_IN_BANK_ITEM);
			break;

		case	eMOB_NOT_BANK_CLIENT							:
			CGamePlay::AddSystemMessage(WHITE,dMSG_NEED_BANK_CLIENT_TITLE);
			break;

		case	eMOB_WAIT_BANK_DATA								:
			CGamePlay::AddSystemMessage(WHITE,dMSG_WAIT_BANK_DATA);
			break;

		case	eMOB_WAIT_TO_BANK_OPEN_PERIOD_MESSAGE			:
			CGamePlay::AddSystemMessage(WHITE,dMSG_WAIT_TO_BANK_OPEN_PERIOD);
			break;

		case	eMOB_CANCELED_BY_NOT_MATCHED_DATA_WITH_SERVER	:
		{
			CGamePlay::AddSystemMessage(WHITE,dMSG_CANCELED_BY_NOT_MATCHED_DATA_WITH_SERVER);
			g_bank.cancelTransaction();
			break;
		}

		case	eMOB_FAILED										:
		{
			CGamePlay::AddSystemMessage(WHITE,dMSG_BANK_TRANSACTION_FAILED);
			g_bank.cancelTransaction();
			break;
		}

		case	eMOB_TRANSACTION_FINISH							:
		{
			CGamePlay::AddSystemMessage(WHITE,dMSG_COMPLETE_BANK_TRANSACTION);
			g_bank.completeTransaction();
			g_hero.buildPower();
			break;
		}

		case	eMOB_CAN_OWN_SAME_ITEM							:
			g_msgBox.cPopup("",dMSG_CAN_NOT_OWN_SAME_ITEM,dMSG_OK);
			break;

		case	eMOB_TOO_MANY_BADGE_ITEM						:
			g_msgBox.cPopup("",_ms(dMSG_TOO_MANY_BADGE_ITEM_FORM,dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY),dMSG_OK);
			break;
		case	eMOB_IS_NOT_OWN_ITEM								:
			g_msgBox.cPopup("", dMSG_IS_NOT_OWN_ITEM, dMSG_OK);
			break;
		case	eMOB_TOO_MANY_GOLD_BAR										:
		{
			CGamePlay::AddSystemMessage(WHITE,dMSG_NO_MORE_STORE_GOLD);
			break;
		}
		case	eMOB_PLAYER_HAVE_TOO_MANY_GOLD_BAR										:
		{
			CGamePlay::AddSystemMessage(WHITE,dMSG_CAN_NOT_POSSESS_NO_MORE_THE_GOLD);
			break;
		}
	}
}	//	cPACKET_HANDLER::ReceiveBankMessage(SERVER_PACKETS *_lpPacket)

void
cPACKET_HANDLER::ReceiveStoreItemToTheBank(SERVER_PACKETS *_lpPacket)
{
	SG_STORE_ITEM_TO_THE_BANK		*lpPacket	=	&_lpPacket->mSG_STORE_ITEM_TO_THE_BANK;

	if (!g_bank.storeItemToTheBank(lpPacket->wInventorySlot,lpPacket->wBankSlot,lpPacket->dwCheckSum,FALSE))
		g_gwBank.cancelBankTransaction();
}	//	cPACKET_HANDLER::ReceiveStoreItemToTheBank(SERVER_PACKETS *_lpPacket)

void
cPACKET_HANDLER::ReceiveWithdrawItemFromTheBank(SERVER_PACKETS *_lpPacket)
{
	SG_WITHDRAW_ITEM_FROM_THE_BANK	*lpPacket	=	&_lpPacket->mSG_WITHDRAW_ITEM_FROM_THE_BANK;

	if (!g_bank.withdrawItemFromTheBank(lpPacket->wInventorySlot,lpPacket->wBankSlot,lpPacket->dwCheckSum,FALSE))
		g_gwBank.cancelBankTransaction();
}	//	cPACKET_HANDLER::ReceiveWithdrawItemToTheBank(SERVER_PACKETS *_lpPacket)

void
cPACKET_HANDLER::ReceiveStoreGoldToTheBank(SERVER_PACKETS *_lpPacket)
{
	SG_STORE_GOLD_TO_THE_BANK		*lpPacket	=	&_lpPacket->mSG_STORE_GOLD_TO_THE_BANK;

	if (!g_bank.storeGoldToTheBank(lpPacket->iGold,lpPacket->dwCheckSum,FALSE))
		g_gwBank.cancelBankTransaction();
}	//	cPACKET_HANDLER::ReceiveStoreGoldToTheBank(SERVER_PACKETS *_lpPacket)

void
cPACKET_HANDLER::ReceiveWithdrawGoldFromTheBank(SERVER_PACKETS *_lpPacket)
{
	SG_WITHDRAW_GOLD_FROM_THE_BANK	*lpPacket	=	&_lpPacket->mSG_WITHDRAW_GOLD_FROM_THE_BANK;

	if (!g_bank.withdrawGoldFromTheBank(lpPacket->iGold,lpPacket->dwCheckSum,FALSE))
		g_gwBank.cancelBankTransaction();
}	//	cPACKET_HANDLER::ReceiveWithdrawGoldToTheBank(SERVER_PACKETS *_lpPacket)

void
cPACKET_HANDLER::ReceiveMoveBankItem(SERVER_PACKETS *_lpPacket)
{
	SG_MOVE_BANK_ITEM				*lpPacket	=	&_lpPacket->mSG_MOVE_BANK_ITEM;

	if (!g_bank.moveItemInTheBank(lpPacket->wPos1,lpPacket->wPos2,lpPacket->dwCheckSum,FALSE))
		g_gwBank.cancelBankTransaction();
}	//	cPACKET_HANDLER::ReceiveMoveBankItem(SERVER_PACKETS *_lpPacket)


void
cPACKET_HANDLER::ReceiveMoveInventoryItemWhenBankTransaction(SERVER_PACKETS *_lpPacket)
{
	SG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	*lpPacket	=	&_lpPacket->mSG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION;

	if (!g_bank.moveItemInInventory(lpPacket->wPos1,lpPacket->wPos2,lpPacket->dwCheckSum,FALSE))
		g_gwBank.cancelBankTransaction();
}	//	cPACKET_HANDLER::ReceiveMoveInventoryItemWhenBankTransaction(SERVER_PACKETS *_lpPacket)