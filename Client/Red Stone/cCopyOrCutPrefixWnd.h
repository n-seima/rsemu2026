#ifndef _classcCopyOrCutPrefixWnd_H_
#define	_classcCopyOrCutPrefixWnd_H_

#include "CGamePlay.H"

class	cCopyOrCutPrefixWnd:	public CGamePlay
{
public:
	int				m_iSrcItem,m_iDestItem;
	int				m_iSelectPrefix,m_iFocusPrefix;
	CPos			m_pos;
	cBARMENU		m_bmMenu;
	BOOL			m_bIsCut;
	int				m_aEquipLevel[dITEM_PREFIX_COUNT];
				
	void			open(int _iSrcItem,int _iDestItem, BOOL _bIsCut);//	¿
	void			close();
	void			update();
	int				getFocusPrefix();

	void			updateBarmenu();

	void			draw();// ±×¸®±
};

extern	cCopyOrCutPrefixWnd	g_gwCopyOrCutPrefix;

#endif