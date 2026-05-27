#ifndef _class_guild_feed_pet_window_H_
#define	_class_guild_feed_pet_window_H_

#include "CGamePlay.H"
#include "CGuild.H"

class	cGuildFeedPetWindow	:	public CGamePlay
{
public:
	cRECT					m_rect;
	CPos					m_pos;
	cBARMENU				m_bmButton;
	int						m_iSelectPet,m_iFocusPet,m_iFeedItem;

							cGuildFeedPetWindow();
							
	BOOL					isOpened();	//	열렸냐?
	void					open();	//	열기
	void					close();	//창 닫기

	int						getFocusPet();

	void					draw();		//그리기
	void					update();	//업데이트

	void					feedItem();
};

extern	cGuildFeedPetWindow	g_gwFeedGuildPet;

#endif