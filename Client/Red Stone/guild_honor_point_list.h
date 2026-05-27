#ifndef _class_guild_honor_point_list_H_
#define	_class_guild_honor_point_list_H_

#include "CGamePlay.H"
#include "CGuild.H"

class	cGuildHonorPointList	:	public CGamePlay
{
public:
	class	cHonorPointInfo
	{
	public:
		int		m_iHonorPoint;
		char	m_strGuildName[dGUILD_NAME_LENGTH];
	};

	cRECT					m_rect;
	WORD					m_wListCount,m_wHallLevel;
	cHonorPointInfo			m_aList[30];	
	CPos					m_pos;
	cBARMENU				m_bmButton;

						//	cGuildHallList();
							
	BOOL					isOpened();	//	열렸냐?

	void					open(void *_lpBuffer);	//	열기

	void					close();	//창 닫기

	void					draw();		//그리기
	void					update();	//업데이트
};

extern	cGuildHonorPointList	g_gwHonorPointList;

#endif