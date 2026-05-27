#ifndef _classGAME_CONNECT_GAME_SERVER_H_
#define	_classGAME_CONNECT_GAME_SERVER_H_

#include "CGame.H"

////////////////////////////////////////////////////////////////////////
//	게임 서버에 연결
////////////////////////////////////////////////////////////////////////

class	cGAME_CONNECT_GAME_SERVER	:	public CGame
{
public:
	static	BOOL		s_isWaitGameServerConnectMessage;

	static	BOOL		Run();
	static	BOOL		Draw();

	static	BOOL		UpdateInit();
	static	BOOL		UpdateReady();
	static	BOOL		UpdateMain();
	static	BOOL		UpdateFinish();
	static	BOOL		UpdateClose();

	static	void		DrawInit();
	static	void		DrawReady();
	static	void		DrawMain();
	static	void		DrawFinish();
	static	void		DrawClose();

	static	BOOL		OperatePacket();
	static	BOOL		OperateMessageBox();

	static	BOOL		WaitConnectMessage();
};

#endif