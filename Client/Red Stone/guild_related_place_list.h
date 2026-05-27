#ifndef _class_guild_related_place_list_H_
#define	_class_guild_related_place_list_H_

#include "CGamePlay.H"
#include "CGuild.H"

class	cGuildRelatedPlaceList	:	public CGamePlay
{
public:
	cRECT					m_rect;
	WORD					m_wPlaceCount,m_awPlaceList[dMAX_GUILD_VILLAGE_COUNT],m_wIsWaitSelectResult;
	CPos					m_pos;
	cBARMENU				m_bmButton;
	int						m_iSelectPlace,m_iFocusPlace,m_iAppointmentPlace;

							cGuildRelatedPlaceList();

	BOOL					isOpened();	//	열렸냐?

	void					open(WORD *_lpPlaceList,int _iCount);	//	열기

	void					close();	//창 닫기

	int						getFocusPlace();

	void					draw();		//그리기
	void					update();	//업데이트

	void					moveToSelectPlace();
};

extern	cGuildRelatedPlaceList	g_gwGuildRelatedPlaceList;

#endif