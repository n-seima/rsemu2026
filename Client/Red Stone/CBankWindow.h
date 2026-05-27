#ifndef _classBankWindow_H_
#define	_classBankWindow_H_

#include "CGamePlay.H"

class	CBankWindow	:	public CGamePlay
{
public:
	int				m_iDragBankItem;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.
	cRECT			m_rect;
	CImageScrollBar	m_sbBank;
	int				m_iScrollBarPos;
	int				m_iFocusBankSlot;
	WORD			m_wIsWaitSaveResult;

	void			open(CBankInfo *_lpBankInfo,int _iBankSerial,int _iBankSize,int _iBankCharge);
	void			close();
	int				getFocusBankItemSlot();
	void			operateDragItem();

	void			update();	//	스테이터스 윈도우 업데이트
	void			draw();		//	스테이터스 윈도우 그려주기

	void			cancelBankTransaction();

	void			storeItemToTheBank(int _iItemSlot);
	void			withdrawItemFromTheBank();
	
	void			storeGoldToTheBank(int _iGold);
	void			withdrawGoldFromTheBank(int _iGold);
	void			moveBankItem();
	void			moveInventoryItemWhenBankTransaction();

	BOOL			isOpened();
};

extern	CBankWindow	g_gwBank;

#endif
