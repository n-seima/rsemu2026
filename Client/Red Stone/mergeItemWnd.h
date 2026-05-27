#ifndef _classMergeItemWnd_H_
#define	_classMergeItemWnd_H_

#include "CGamePlay.H"

class	cMergeItemWnd	:	public CGamePlay
{
public:
	class	cItemInfo
	{
	public:
		cItem		m_item;
		WORD		m_wIsMerged;
		WORD		m_wItemSlot;
		WORD		m_wPieceSerial;
		WORD		m_wExistPieceCount;
		WORD		m_wRequirePieceCount;
		WORD		m_awLackPieceList[dMAX_PIECE_ITEM_PIECE_COUNT];
	};

	cBARMENU		m_bmMenu;
	int				m_iClientSerial,m_iItemCount;
	cItemInfo		m_aItemList[20];
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
	void			mergeItem(int _iItem);
};

extern	cMergeItemWnd	g_gwMergeItem;

#endif