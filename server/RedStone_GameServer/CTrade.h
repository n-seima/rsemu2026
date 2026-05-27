#ifndef _classTRADE_H
#define	_classTRADE_H

#include "cItem.H"
#include "cPACKET_GAMESERVER.H"

#define	dTRADE_ITEM_COUNT	18

//
//	Trade Status
enum
{
	eTS_OPEN	,
	eTS_READY	,
	eTS_PERMIT	,
};

//
//	Result of Add Trade Item
enum
{
	eRATI_OK,
	eRATI_ALREADY_ADDED_ITEM,
	eRATI_VALID_ITEM,
	eRATI_INCORRECT_TRADER,
	eRATI_TRADE_BOX_FULL,
	eRATI_INCORRECT_TRADER_ITEM_COUNT,
	eRATI_INCORRECT_TRADE_SLOT,	//	setTradeItem용 리턴값
	eRATI_CAN_NOT_TRADE_ITEM,	//	트레이드 할 수 없는 아이템이다.
	eRATI_CAN_NOT_TRADE_ITEM_IS_REVERSION_ITEM,	//	귀속 아이템이다.
	eRATI_CAN_NOT_OWN_SAME_ITEM,
	eRATI_TOO_MANY_BADGE_ITEM,
	eRATI_COUPLE_RING_TRADE_ONLY_ONE_AT_ONE_TIME,
	eRATI_TOO_MANY_GOLD_BAR,
};

//
//	Result of Remove Trade Item
enum
{
	eRRTI_OK,
	eRRTI_INCORRECT_TRADE_BOX_SLOT,
	eRRTI_VALID_TRADE_BOX_SLOT,
	eRRTI_INCORRECT_TRADER,
};

//
//	Result of Set Trade Gold
enum
{
	eRSTG_OK,
	eRSTG_INCORRECT_TRADER,
	eRSTG_LOW_GOLD,
	eRSTG_TARGET_GOLD_FULL,
};

//
//	Result of Trade
enum
{
	eRT_OK,
	eRT_LOW_SLOT,
	eRT_INCORRECT_TRADER,
};

class	cACTOR;
class	cFIELD;

class	CTradeBox
{
public:
	class			CTradeItem
	{
	public:
		WORD		m_wSlot;
		WORD		m_wCount;

					CTradeItem()
					{
						m_wSlot	=	0xffff;
					}
	};

	cFIELD			*m_lpField;
	WORD			m_wSerial;
	char			m_strName[2][dNAME_LENGTH];
	WORD			m_aTraderSerial[2];
	int				m_iaTradeGold[2];
	CTradeItem		m_aTradeItem[2][dTRADE_ITEM_COUNT];

	WORD			m_aTradeStatus[2];

					CTradeBox()
					{
						m_lpField	=	NULL;
						reset();
					}
					
	void			reset();

	void			init(int _iSerial,cFIELD *_lpField,char *_lpstrName1,char *_lpstrName2,int _iTrader1,int _iTrader2);
	BOOL			check();
	void			sendTradeCancelMessage();
	int				getTraderIndex(cACTOR *_lpTrader);
	int				getTraderIndex(char *_lpstrName);
	int				getTradeItemSlotIndex(int _iTraderIndex,int _iItemSlot);
	cACTOR*			getMate(cACTOR *_lpActor);
	
	int				addTradeItem(cACTOR *_lpTrader,int _iItemSlot,int _iCount);
	int				changeTradeItemData(cACTOR *_lpTrader,int _iTradeSlot,int _iItemSlot,int _iCount);
	BOOL			removeTradeItem(cACTOR *_lpTrader,int _iTradeBoxSlot);
	BOOL			setTradeGold(cACTOR *_lpTrader,int _iGold);
	cACTOR*			getTradeMate(cACTOR	*_lpAsker);	//	트레이드 상대를 얻어온다.
	BOOL			readyTrade(cACTOR *_lpTrader);
	BOOL			permitTrade(cACTOR *_lpTrader);
	BOOL			isReadyToTrade();
	BOOL			getTradePacketInfo(cACTOR *_lpTrader,SG_TRADE *_lpPacket);

	int				isTradeAble(cACTOR *_lpTrader);	//	트레이드 가능?
	int				getCheckSum(cACTOR *_lpTrader);
	BOOL			trade(cACTOR *_lpTrader);	//	트레이드 -_-
};

class	CTradeManager
{
public:
	WORD				m_wTradeBoxCount;	//	현재 트레이드 되고 있는것들?
	WORD				m_wMaxTradeBoxCount,m_wRookieTradeBox;
	CTradeBox			*m_pTradeBox;

						CTradeManager();
						~CTradeManager();

	void				init(int _iMaxTraderCount);
	int					addTradeBox(cACTOR *_lpTrader1,cACTOR *_lpTrader2);
	BOOL				removeTradeBox(int _iTraderIndex,char *_lpstrName);
	int					getValidTradeBox();
	CTradeBox*			getTradeBox(cACTOR *_lpTrader);

	inline	BOOL		addTradeItem(int _iTraderIndex,cACTOR *_lpTrader,int _iSlot,int _iCount)
	{
		return	m_pTradeBox[_iTraderIndex].addTradeItem(_lpTrader,_iSlot,_iCount);
	}
	inline	BOOL		removeTradeItem(int _iTraderIndex,cACTOR *_lpTrader,int _iSlot)
	{
		return	m_pTradeBox[_iTraderIndex].removeTradeItem(_lpTrader,_iSlot);
	}

};

#endif