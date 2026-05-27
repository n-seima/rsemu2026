#ifndef _class_guild_hall_list_H_
#define	_class_guild_hall_list_H_

#include "CGamePlay.H"
#include "CGuild.H"

class	cGuildHallList	:	public CGamePlay
{
public:
	class	cGuildHallInfo
	{
	public:
		char	m_strGuildName[dGUILD_NAME_LENGTH];
		WORD	m_wGuild;
		WORD	m_bf3HallLevel		:	3;
		WORD	m_bf1IsPermitVisitor:	1;
	};

	cRECT					m_rect;
	WORD					m_wHallCount,m_wIsWaitSelectResult;
	cGuildHallInfo			m_aHallList[c_iGuildHallCount];	
	CPos					m_pos;
	cBARMENU				m_bmButton;
	int						m_iSelectHall,m_iFocusHall,m_iAppointmentGuild;

							cGuildHallList();
							
	BOOL					isOpened();	//	열렸냐?

	void					open(char *_lpBuffer,int _iCount);	//	열기

	void					close();	//창 닫기

	int						getFocusHallIndex();

	void					draw();		//그리기
	void					update();	//업데이트

	void					moveToSelectGuildHall();
};

extern	cGuildHallList	g_gwGuildHallList;

#endif