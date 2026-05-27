#ifndef _classGAME_CREATE_AVATAR_H_
#define	_classGAME_CREATE_AVATAR_H_

#include "CGame.H"

////////////////////////////////////////////////////////////////////////
//	아이디 생성
////////////////////////////////////////////////////////////////////////

#define	dMAX_LIGHT	20

class	cGAME_CREATE_AVATAR	:	public CGame
{
public:
	static	cGAME_UTIL::cLIGHT	s_aLight[dMAX_LIGHT];

	static	int			s_iAvatarCount;
	static	int			s_iTimeOfConnectTry;				//	컨넥트 시도 시간

	static	cSPRITE		s_sprSprite;
	static	cANM		s_anmLight;

	static	int			s_iSelectJob;

	static	BOOL		Run();
	static	BOOL		Draw();

	static	void		UpdateLight();

	static	BOOL		UpdateInit();
	static	BOOL		UpdateReady();
	static	BOOL		UpdateMain();
	static	BOOL		UpdateFinish();
	static	BOOL		UpdateClose();

	static	void		DrawLight();
	static	void		DrawInit();
	static	void		DrawReady();
	static	void		DrawMain();
	static	void		DrawFinish();
	static	void		DrawClose();
	static  void		DrawAvatar( );


	static	BOOL		OperatePacket();
	static	BOOL		OperateMessageBox();
};

#endif