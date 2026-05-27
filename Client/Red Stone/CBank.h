#ifndef _classBANK_H
#define _classBANK_H

#include "SFC.H"
#include "definePacketData.h"
#include "cItem.h"

class	CBankBaseData
{
public:
	CBankInfo	m_bankInfo;

	int			m_iOwnGold;
	WORD		m_wInventorySize;
	WORD		m_wBankSize,m_wBankCharge;

	cItem		m_aEquipment[dEQUIPMENT_PART_COUNT];
	cItem		m_aInventoryItem[dOWN_ITEM_COUNT];
};

class	CBank : public CBankBaseData
{
public:
	WORD		m_wBankSerial;
	WORD		m_wGoldBarCount;		// 금괴 개수.... 09.08.20
	WORD		m_wGoldBarTax;		// 금괴 개수.... 09.08.20
	CBankInfo	m_firstBankInfo;

	void		reset();
	void		open(CBankInfo *_lpBankData,int _iBankSerial,int _iBankSize,int _iBankCharge);

	cItem*		getItemInInventory(int _iBaseItem);	//	아이템 얻기
	int			getBadgeCountInInventory();	//	배지의 수 얻기

	BOOL		storeItemToTheBank(int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum,BOOL _bIsCheck);
	BOOL		withdrawItemFromTheBank(int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum,BOOL _bIsCheck);
	BOOL		storeGoldToTheBank(int _iGold,DWORD _dwCheckSum,BOOL _bIsCheck);
	BOOL		withdrawGoldFromTheBank(int _iGold,DWORD _dwCheckSum,BOOL _bIsCheck);

	BOOL		moveItemInTheBank(int _iSlot1,int _iSlot2,DWORD _dwCheckSum,BOOL _bIsCheck);
	BOOL		moveItemInInventory(int _iSlot1,int _iSlot2,DWORD _dwCheckSum,BOOL _bIsCheck);

	DWORD		getCheckSum();
	DWORD		getBankTransactionCharge();
	cItem*		getSameItemInBank(cItem *_lpItem,CBankInfo *_lpBankInfo);


	void		cancelTransaction();
	void		completeTransaction();

	BOOL		isOpened(){if (m_wBankSerial == 0xffff) return FALSE;return TRUE;}

	cItem*		getBankItem(int _iSlot)
	{
		if	(_iSlot < 0 || _iSlot >= m_wBankSize)
			return NULL;
		if	(m_bankInfo.m_aItems[_iSlot].m_wBaseItem == 0xffff)
			return NULL;

		return	(cItem *)&m_bankInfo.m_aItems[_iSlot];
	}
};

extern	CBank	g_bank;

#endif