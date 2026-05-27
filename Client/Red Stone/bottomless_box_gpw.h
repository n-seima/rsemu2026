#ifndef	_classBottomlessBox_h
#define	_classBottomlessBox_h

#include "CGamePlay.H"
#include "carrotShopDefine.H"

class	cBottomlessBoxWindow	:	public CGamePlay
{
public:
	cItem			m_aMixItem[2];
	int				m_aiMixItem[2];
	int				m_iFocusSlot;
	cRECT			m_rect;
	DWORD			m_dwWaitToResultTime;
	int				m_iBottomlessBox;

	void			open(int _iItemSlot);
	void			close();

	BOOL			addItem(int _iItem);

	void			update();
	void			mix();

	void			draw();

	int				getFocusSlot();

	BOOL			isOpened();
};

extern	cBottomlessBoxWindow	g_gwBottomlessBoxWindow;

#endif
