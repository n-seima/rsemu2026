#ifndef _classGAME_PLAY_INTERFACE_
#define _classGAME_PLAY_INTERFACE_

#include "CGamePlay.H"

#define	dBM_ICW_1		1000
#define	dBM_ICW_5		1001
#define	dBM_ICW_10		1002
#define	dBM_ICW_HALF	1003
#define	dBM_ICW_ALL		1004

enum		//09.08.18
{
	eBM_RADIO_GOLD,
	eBM_RADIO_GOLD_BAR,
};
//
//	ICWR - Item Count Window Result
enum
{
	eICWR_NONE									,
	eICWR_CANCEL								,
	eICWR_RETURN_DROP_ITEM_COUNT				,
	eICWR_RETURN_SELL_ITEM_COUNT				,
	eICWR_RETURN_TRADE_ITEM_COUNT				,
	eICWR_RETURN_CHANGE_TRADE_ITEM_COUNT		,
	eICWR_RETURN_TRADE_GOLD						,
	eICWR_RETURN_DROP_GOLD						,
	eICWR_RETURN_STORE_GOLD_TO_THE_BANK			,
	eICWR_RETURN_STORE_GOLD_TO_GUILD_INVENTORY	,
	eICWR_RETURN_WITHDRAW_GOLD_FROM_THE_BANK	,
	eICWR_RETURN_WITHDRAW_GOLD_FROM_GUILD_INVENTORY	,
	eICWR_RETURN_PITCHMAN_SHOP_SELL_ITEM_COUNT	,
	eICWR_RETURN_DROP_INVENTORY_ITEM_DIVIDE,				// 인벤토리에서~ 아이템 나누기~		09.08.31
	eICWR_RETURN_MOTP_CODE	,				// 아이템 카운트가 아닌MOTP 코드
};

class	CGamePlayInterfaceItemCount	:	public CGamePlay
{
public:
	CPos				m_pos;
	CImageScrollBar		m_sbCount;
	cBARMENU			m_bmButton, m_bmRadioButton;		//09.08.18	//	버튼들
	BYTE				m_bDenomination;					// 통화 단위 09.08.21	
	BOOL				m_bIsOpend;
	cItem				m_item;
	int					m_iInputPosForItemPrice;
	int					m_iItemSlot;
	int					m_iFocusSlot;		/// 09.09.01		 
	int					m_iCount,m_iItemCount;
	int					m_iPrice;
	int					m_iWorkResult;
	int					m_iPriceFactor;	//	가격 계산을 위한 팩터
	char				m_strTitle[64];
	char*				m_strDenomination;		// 09.08.21
	cTALK				m_inputGold,*m_lpRefInput;
	int					m_iRefInputIndex;
	cRECT				m_rect;
						CGamePlayInterfaceItemCount();

	void				init();	//	초기화
	
	BOOL				reset();	//	닫기
	BOOL				close();	//	닫기
	BOOL				popup(int _iX,int _iY,int _iDropItemSlot,cItem *_lpItem,int _iWorkResult,int _iPriceFactor=100, int _iFocusSlot = 0xffff,
								cTALK* _lpInput=NULL,int _iIndex=0);	//	아이템 윈도우 열기	09.09.01 
	
	inline	DWORD		getItemCount()
	{
		if (m_item.m_wBaseItem	==	dITEM_MONEY	||	m_item.m_wBaseItem	==	dITEM_CARROT)
			return	m_item.m_dwSerial;

		return	m_item.m_bCount;
	}

	int					update();
	void				draw();
	void				drawPitchmanShopSellItemCount();		// 노점상아이템 등록시 가격 등록창그리기~  09.08.20 
};
extern	CGamePlayInterfaceItemCount	g_itemCountWindow;

#endif