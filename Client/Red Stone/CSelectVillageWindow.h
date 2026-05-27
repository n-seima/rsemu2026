#ifndef _classSelectVillageWindow_H_
#define	_classSelectVillageWindow_H_

#include "CGamePlay.H"

class	CSelectVillageWindow	:	public CGamePlay
{
public:
	int				m_iVillageCount,m_iSelectVillage,m_iFocusVillage;
	int				m_aiVillageList[100];
	BOOL			m_bIsReadyToCreatePortal;
	CPos			m_pos;

	void			open(WORD *_lpwList,int _iCount,BOOL _bIsCreatePortal);	//	열기
	void			close();	//	닫기
	void			draw();		//	그리기
	int				getFocusVillage();	//	업데이트
	void			update();	//	업데이트
	BOOL			isOpened();
};

extern	CSelectVillageWindow	g_gwSelectVillage;

#endif