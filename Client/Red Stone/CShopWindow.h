#ifndef _classShopWindow_H_
#define	_classShopWindow_H_

#include "CGamePlay.H"
enum
{
	eSK_NORMAL,
	eSK_PIGEON_POST,
	eSK_GUILD_SHOP
};

class	CShopWindow	:	public CGamePlay
{	// 마지막 수정일 : 09.10.26
public:
	int				m_iSelectShopItem,m_iSellItemSlot,m_iShopWindowScrollBarPos;
	BOOL			m_bIsDuelPointShop, m_bIsLevelPerMoneyShop;
	WORD			m_wLevelPerGold;
	cRECT			m_rect;
	int				m_iShopKind;
	int				m_iBuyItemCount;
	WORD			m_wGuildShopItemCount,m_awGuildShopItemList[c_iGuildHallSellEnchantItemCount];
	WORD			m_wTokkenItem;
	BOOL			m_bIsNotBuyItem;		// 상점 구입 불가.
	BOOL			m_bIsBadgeOfTraderShop; // 거래상인의 배지 상점..
	void			initCommon();
	void			open(SG_OPEN_SHOP *_lpPacket);	//	상점 열기
	void			openForGuildShop(SG_OPEN_GUILD_SHOP *_lpPacket);	//	상점 열기
	void			openForPigeonPost(int _iCharge,int _iCount);
	void			close();	//	상점 닫기
	void			draw();		//	그리기
	void			update();	//	업데이트
	void			sellItem(int _iItem);	//	아이템 판매
	inline	BOOL	isOpened()	
	{
		if	(s_shop.m_wItemCount > 0)
			return TRUE;

		return FALSE;
	}
};

extern	CShopWindow	g_gwShop;

#endif