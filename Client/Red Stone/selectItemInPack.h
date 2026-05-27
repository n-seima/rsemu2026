#ifndef _classSelectItemInPack_H_
#define	_classSelectItemInPack_H_

#include "CGamePlay.H"
#include "cItem.H"

class	cSelectItemInPackWnd	:	public CGamePlay
{
public:
	cItemPackInfo	m_itemPackInfo;
	int				m_iSelectItem;	//	구입을 위해 선택한 아이템
	int				m_iFocusItem;	//	포커스가 간 아이템
	cRECT			m_rect;
	int				m_iScrollBarPos;
	WORD			m_wIsWaitResult;
	WORD			m_wPackItem;

	void			open();
	void			close();

	void			update();
	void			draw();

	int				getFocusItem();
	void			selectItem(int _iItem,int _iPrefix);
	void			setData(int _iItem,void *_lpData);

	BOOL			isOpened();
};

extern	cSelectItemInPackWnd	g_gwSelectItemInPack;

#endif
