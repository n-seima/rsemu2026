#ifndef _classCartWindow_H_
#define	_classCartWindow_H_

#include "CGamePlay.H"

class	CCartWindow	:	public CGamePlay
{
public:
	int				m_iDragCartItem;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.
	int				m_iWithdrawItemCartSlot;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.
	int				m_iItemCount;
	cRECT			m_rect;
	int				m_iFocusCartSlot,m_iSelectCartSlot;

	void			open(void *_lpItemInfo,int _iCartSerial,int _iItemCount);

	void			askDestroyCartItem();
	void			close();
	int				getFocusCartItemSlot();
	inline	void	operateCartWindowPopupMenu();
	void			operateDragItem();

	void			update();	//	스테이터스 윈도우 업데이트
	void			draw();		//	스테이터스 윈도우 그려주기

	void			cancelCartTransaction();
	void			askWithdrawItemFromTheCart(BOOL _bIsWithdrawMagicBag=FALSE);
	void			withdrawItemFromTheCart(int _iCartslot,int _iInventorySlot,cItem *_lpItem);
	void			removeCartItem(int _iCartslot);

	BOOL			isOpened();
};

extern	CCartWindow	g_gwCart;

#endif