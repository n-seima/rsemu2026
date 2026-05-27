#ifndef _classJudgeToItemWnd_H_
#define	_classJudgeToItemWnd_H_

#include "CGamePlay.H"

class	cJudgeToItemWnd	:	public CGamePlay
{
public:
	cBARMENU		m_bmMenu;
	int				m_iClientSerial,m_iItemCount;
	int				m_aiItemList[dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE];
	int				m_iSelectItem;
	int				m_iFocusItem;
	CPos			m_pos;

	void			build();
	void			open(int _iClientSerial);
	void			close();

	void			update();
	void			updateBarmenu();
	void			draw();

	int				getFocusItem();

	BOOL			isOpened();
	int				getSelectItem();
};

extern	cJudgeToItemWnd	g_gwJudgeToItem;

#endif