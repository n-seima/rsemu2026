#ifndef _classPitchmanShopWindow_H_
#define	_classPitchmanShopWindow_H_

#include "CGamePlay.H"

enum
{
	ePS_SHOP_KEEPER,
	ePS_CLIENT,
};

class	CPitchmanShopWindow	:	public CGamePlay
{
public:
	int						m_iFocusSlot,m_iSelectShopItem,m_iDragItem,m_iSellItemSlot,m_iShopWindowScrollBarPos,m_iBookingBuyItem;
	WORD					m_wIsWaitPichmanShopWorkResult;
	cRECT					m_rect;
	int						m_iOwner,m_iShopStatus,m_iMenuMode;
	CPitchmanShopForClient	m_shop;

	char					m_strShopName[128];
	WORD					m_wTextColor;
	int						m_iSignboardShape;
	BOOL					m_bIsBold,m_bIsShadowText;

	void					openForClient(SG_PITCHMAN_SHOP_INFO *_lpPacket);
	void					closeByServer();
	void					close();	//	상점 닫기
	void					draw();		//	그리기
	void					drawForShopkeeper();		//	그리기
	void					drawForClient();		//	그리기
	void					update();	//	업데이트
	void					operateDropItem(int _iInventorySlot);
	void					operateDragItem();
	void					updateForShopkeeper();	//	업데이트
	void					updateForClient();	//	업데이트
	void					buyPitchmanShopItem();

	void					askAddSellingItem(int _iItemSlot,int _iPrice, BYTE _bDenomination);		//09.08.21 

	int						getFocusSlotForShopKeeper();
	int						getFocusSlotForClient();
	int						getSellingItemCount();

	void					openForShopkeeper(SG_OPEN_PITCHMAN_SHOP	*_lpPacket);
	void					addItem(SG_ADD_PITCHMAN_SHOP_ITEM *_lpPacket);
	void					removeItem(SG_REMOVE_PITCHMAN_SHOP_ITEM *_lpPacket);
	void					changeStatus(SG_CHANGE_PITCHMAN_SHOP_INFO	*_lpPacket);
	void					changeItemPlace(SG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE *_lpPacket);
	void					soldItem(SG_SOLD_PITCHMAN_SHOP_ITEM	*_lpPacket);
	void					buyItem(SG_BUY_PITCHMAN_SHOP_ITEM	*_lpPacket);
	
	BOOL					isSellingInventorySlot(int _iSlot);
	inline	BOOL			isOpened()	
	{
		if (s_iPopupInterface	==	ePIW_PITCHMAN_SHOP)
			return TRUE;

		return FALSE;
	}
};

extern	CPitchmanShopWindow	g_gwPitchmanShop;

#endif