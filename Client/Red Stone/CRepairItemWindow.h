#ifndef _classRepairItemWindow_H_
#define	_classRepairItemWindow_H_

#include "CGamePlay.H"

class	CRepairItemWindow	:	public CGamePlay
{
public:
	int				m_iSelectItem,m_iScrollBarPos;
	int				m_iBreakDownItemCount;
	int				m_aiBreakDownItemList[dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE];
	int				m_iNpcSerial;
	cRECT			m_rect;

	void			open(int _iNpcSerial);	//	열기
	void			close();	//	닫기
	void			draw();		//	그리기
	void			update();	//	업데이트
	BOOL			isOpened();
};

extern	CRepairItemWindow	g_gwRepairItem;

#endif