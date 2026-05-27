#ifndef	_boss_monster_event_common_h
#define	_boss_monster_event_common_h

#include	"movie.h"
#include	"CGamePlay.h"

class	cBM_EventCommon
{
public:
	cMovie			*m_pMovie;

	virtual	BOOL	open()	{return	TRUE;};
	virtual	void	close()	{};
	virtual	BOOL	update(){return	TRUE;};
	virtual	void	draw()	{};
};

class	cBM_Event	: public CGamePlay
{
public:
	cBM_EventCommon	*m_lpCurrentEvent;

	cBM_Event();
	~cBM_Event();

	BOOL	buildScript();
	BOOL	beginBossBattleEvent(int _iBoss,int _iBeginEnd);

	void	draw();
	void	update();
	void	close();

	inline	BOOL	isAvail()
	{
		if	(m_lpCurrentEvent)
			return	TRUE;

		return	FALSE;
	}
};

extern	cBM_Event	g_bossMonsterEvent;	//	보스 몬스터 이벤트 공통

#endif