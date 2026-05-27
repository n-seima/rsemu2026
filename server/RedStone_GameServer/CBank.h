#ifndef _classBank_h
#define	_classBank_h

#include "cItem.H"
#include "cPACKET_GAMESERVER.H"

#define	dMAX_OPEN_BANK	100

class	cACTOR;

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

//
//	Result Of Bank Work
enum
{
	eRBW_FAILED,
	eRBW_SUCCESS,
	eRBW_CAN_NOT_STORE_ITEM,
	eRBW_CAN_NOT_OWN_SAME_ITEM,
	eRBW_TOO_MANY_BADGE_ITEM,
	eRBW_IS_NOT_OWN_ITEM,
	eRBW_TOO_MANY_GOLD_BAR,
};


class	CBank : public CBankBaseData
{
public:
	WORD		m_wSerial;
	DWORD		m_dwCheckSum;
	cACTOR		*m_lpActor;
	LONGLONG	m_llOpenGoldSum;	//	열읍을융 

	WORD		m_wActorItemChecksum;
	CBankInfo	m_firstBankInfo;

				CBank()
				{
					reset();
				}

	void		reset()
	{
		m_wSerial	=	0xffff;
		m_lpActor	=	NULL;
	}

	void		open(CBankInfo *_lpBankInfo,cACTOR *_lpActor);
	BOOL		removeExpiredItemAndReviseIncorrectPrefix();	
	int			getBadgeCountInInventory();	//	배지의 수 계산
	cItem*		getItemInInventory(int _iBaseItem);	//	인벤토리에서 아이템 검색

	int			storeItemToTheBank(int _iInventorySlot,int _iBankSlot,DWORD _dwChecksum);
	BOOL		withdrawItemFromTheBank(int _iInventorySlot,int _iBankSlot,DWORD _dwChecksum,  cACTOR* _lpActor);	// 수정.

	int			storeGoldToTheBank(int _iGold,DWORD _dwChecksum);
	BOOL		withdrawGoldFromTheBank(int _iGold,DWORD _dwChecksum,int *_lpiWithDrawGold);

	BOOL		moveBankItem(int _iSlot1,int _iSlot2,DWORD _dwChecksum);
	BOOL		moveInventoryItem(int _iSlot1,int _iSlot2,DWORD _dwChecksum);
	BOOL		askFinishBankTransaction(DWORD _dwChecksum);
	BOOL		finishBankTransaction();

	BOOL		checkIsIncorrectData(DWORD _dwChecksum);
	DWORD		getCheckSum();

	cItem*		getActorItem(cItem *_lpItem);
	cItem*		getSameItemInBank(cItem *_lpItem,CBankInfo *_lpBankInfo);
	BOOL		sendWorkResultLog(int _iStoreGold);
	DWORD		getBankTransactionCharge();
	int			getGoldBarCount();		// 금괴개수..
	LONGLONG	getGoldSum();
};

class	CBankManager
{
public:
	CBank				*m_pBank;
	int					m_iBankCount,m_iBankSize;
	CRITICAL_SECTION	m_cs;

						CBankManager();
						~CBankManager();

	void				init(int _iBankCount = dMAX_OPEN_BANK);

	int					open(CBankInfo *_lpBankInfo,cACTOR *_lpActor);
	int					getFreeBankIndex();
	void				checkIncorrectBank();
	void				closeBank(int _iBankSerial,BOOL _bIsForced=FALSE);

	inline	CBank*		getBank(int _iIndex)
	{
		if (_iIndex < 0 || _iIndex >= m_iBankSize)	return	NULL;
		if (m_pBank[_iIndex].m_wSerial	==	0xffff)	return	NULL;

		return	&m_pBank[_iIndex];
	}
};

extern	CBankManager	g_bankManager;

#endif