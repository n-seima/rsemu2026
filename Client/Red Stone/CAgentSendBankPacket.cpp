#include "CAgent.H"
#include "CActor.H"
#include "CHero.H"

BOOL
CAgent::sendStoreItemToTheBank(int _iBankSerial,int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum)
{
	CG_STORE_ITEM_TO_THE_BANK		packet;

	packet.base.set(sizeof(packet),dCG_STORE_ITEM_TO_THE_BANK		);
	packet.dwCheckSum	=	_dwCheckSum;
	packet.wBankSerial	=	_iBankSerial;
	packet.wInventorySlot=	_iInventorySlot;
	packet.wBankSlot	=	_iBankSlot;

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendStoreItemToTheBank(int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum)

BOOL
CAgent::sendStoreGoldToTheBank(int _iBankSerial,int _iGold,DWORD _dwCheckSum)
{
	CG_STORE_GOLD_TO_THE_BANK		packet;

	packet.base.set(sizeof(packet),dCG_STORE_GOLD_TO_THE_BANK		);
	packet.dwCheckSum	=	_dwCheckSum;
	packet.wBankSerial	=	_iBankSerial;
	packet.iGold		=	_iGold;

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendStoreGoldToTheBank(int _iGold,DWORD _dwCheckSum)

BOOL
CAgent::sendWithdrawItemFromTheBank(int _iBankSerial,int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum)
{
	CG_WITHDRAW_ITEM_FROM_THE_BANK	packet;

	packet.base.set(sizeof(packet),dCG_WITHDRAW_ITEM_FROM_THE_BANK	);
	packet.dwCheckSum	=	_dwCheckSum;
	packet.wBankSerial	=	_iBankSerial;
	packet.wInventorySlot=	_iInventorySlot;
	packet.wBankSlot	=	_iBankSlot;

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendWithdrawItemFromTheBank(int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum)

BOOL
CAgent::sendWithdrawGoldFromTheBank(int _iBankSerial,int _iGold,DWORD _dwCheckSum)
{
	CG_WITHDRAW_GOLD_FROM_THE_BANK	packet;

	packet.base.set(sizeof(packet),dCG_WITHDRAW_GOLD_FROM_THE_BANK	);
	packet.dwCheckSum	=	_dwCheckSum;
	packet.wBankSerial	=	_iBankSerial;
	packet.iGold		=	_iGold;

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendWithdrawGoldFromTheBank(int _iGold,DWORD _dwCheckSum)

BOOL
CAgent::sendMoveBankItem(int _iBankSerial,int _iSlot1,int _iSlot2,DWORD _dwCheckSum)
{
	CG_MOVE_BANK_ITEM				packet;

	packet.base.set(sizeof(packet),dCG_MOVE_BANK_ITEM				);
	packet.dwCheckSum	=	_dwCheckSum;
	packet.wBankSerial	=	_iBankSerial;
	packet.wPos1		=	_iSlot1;
	packet.wPos2		=	_iSlot2;

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendMoveBankItem(int _iSlot1,int _iSlot2,DWORD _dwCheckSum)

BOOL
CAgent::sendMoveInventoryItemWhenBankTransaction(int _iBankSerial,int _iSlot1,int _iSlot2,DWORD _dwCheckSum)
{
	CG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	packet;

	packet.base.set(sizeof(packet),dCG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	);
	packet.dwCheckSum	=	_dwCheckSum;
	packet.wBankSerial	=	_iBankSerial;
	packet.wPos1		=	_iSlot1;
	packet.wPos2		=	_iSlot2;

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendMoveInventoryItemWhenBankTransaction(int _iSlot1,int _iSlot2,DWORD _dwCheckSum)

BOOL
CAgent::sendCancelBankTransaction()
{
	CG_CANCEL_BANK_TRANSACTION		packet;

	packet.base.set(sizeof(packet),dCG_CANCEL_BANK_TRANSACTION		);

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendCancelBankTransaction()

BOOL
CAgent::sendFinishBankTransaction(int _iBankSerial,DWORD _dwCheckSum)
{
	CG_FINISH_BANK_TRANSACTION		packet;

	packet.base.set(sizeof(packet),dCG_FINISH_BANK_TRANSACTION		);
	packet.dwCheckSum	=	_dwCheckSum;
	packet.wBankSerial	=	_iBankSerial;

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendFinishBankTransaction()