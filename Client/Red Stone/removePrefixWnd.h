#ifndef _classRemovePrefixWnd_H_
#define	_classRemovePrefixWnd_H_

#include "CGamePlay.H"

class	cRemovePrefixWnd	:	public CGamePlay
{
public:
	int				m_iSrcItem,m_iDestItem;
	int				m_iSelectPrefix,m_iFocusPrefix;
	CPos			m_pos;
	cBARMENU		m_bmMenu;
	int				m_aEquipLevel[dITEM_PREFIX_COUNT];

				//	cEasySkillTree();
					
	void			open(int _iSrcItem,int _iDestItem);//	¿
	void			close();
	void			update();
	int				getFocusPrefix();

	void			updateBarmenu();

	void			draw();// ±×¸®±
};

extern	cRemovePrefixWnd	g_gwRemovePrefix;

#define	dBORDER_OF_BOTTOM_SKILL	1000

#endif