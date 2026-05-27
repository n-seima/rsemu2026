#ifndef _class_guild_inventory_H_
#define	_class_guild_inventory_H_

#include "CGamePlay.H"
#include "CGuild.H"

class	cGuildInventory	:	public CGamePlay
{
public:
	cRECT					m_rect;
	CPos					m_pos;
	cBARMENU				m_bmButton;
	int						m_iDragItem;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.
	int						m_iFocusSlot,m_iScrollBarPos;
	int						m_iFocusStorage,m_iSelectStorage,m_iPopupedStorage;
	WORD					m_wIsWaitWorkResult;

							cGuildInventory();
							
	BOOL					isOpened();	//	열렸냐?

	void					open(cGuildInventoryItemForPacket	*_lpItemList,int _iCount);	//	열기

	void					close();
	int						getFocusItemSlot();
	int						getFocusStorage();
	void					operateDragItem();

	void					update();	//	스테이터스 윈도우 업데이트
	void					draw();		//	스테이터스 윈도우 그려주기

	void					cancel();

	void					storeItem(int _iItemSlot);
	void					withdrawItem();
	
	void					storeGold(int _iGold);
	void					withdrawGold(int _iGold);
	void					changeItemPlace();
};

extern	cGuildInventory	g_gwGuildInventory;

#endif