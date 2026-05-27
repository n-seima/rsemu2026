#ifndef _classTRADE_H
#define	_classTRADE_H

#include "cItem.H"

#define	dTRADE_ITEM_COUNT	18
#define	dNAME_LENGTH		18

//
//	Trade Status
enum
{
	eTS_OPEN	,
	eTS_READY	,
	eTS_PERMIT	,
};

class	CTradeBox
{
public:
	cItem			m_aItems[dTRADE_ITEM_COUNT];
	int				m_iGold;
	WORD			m_wStatus;
	WORD			m_wSerial;
	char			m_strName[dNAME_LENGTH];

	void			reset();
};

class	CActor;

class	CTradeManager
{
public:
	CTradeBox		m_myTradeBox,m_mateTradeBox;
	BOOL			m_bIsActive;
	WORD			m_wTradeBoxSerial;

	char			m_strLastestTradeUser[dNAME_LENGTH];
//	m_myTradeBox m_aItems의 m_wBaseItem은 인벤토리에 슬롯을 가르킨다.
					CTradeManager(){reset();}

	BOOL			isReady()
	{
		if (m_myTradeBox.m_wStatus	>=	eTS_READY	&&	m_mateTradeBox.m_wStatus	>=	eTS_READY)	return	TRUE;

		return	FALSE;
	}
	void			reset();

	void			beginTrade(CActor *_lpMate,int _iTradeBoxSerial);	//	트레이드 시작

	void			resetTradeStatus();
	BOOL			addMateItem(cItem *_lpItem);
	BOOL			addMyItem(cItem *_lpItem);
	BOOL			changeTradeItemDataByMe(int _iTradeSlot,cItem *_lpItem);
	BOOL			changeTradeItemDataByMate(int _iTradeSlot,cItem *_lpItem);
	BOOL			removeItem(int _iTrader,int _iTradeBoxSlot);
	BOOL			setGold(int _iTrader,int _iGold);
	BOOL			readyTrade(int _iTrader);
	BOOL			permitTrade(int _iTrader);
	BOOL			isTradeItem(int _iSlot);
	BOOL			getTradeItem(int _iSlot,cItem *_lpItem);
	int				getTradeItemSlot(int _iSlot);
	int				getCheckSum();
};


#endif