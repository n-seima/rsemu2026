#ifndef _classGAME_SELECT_AVATAR_H_
#define	_classGAME_SELECT_AVATAR_H_

#include "CGame.H"

////////////////////////////////////////////////////////////////////////
//	아이디 생성
////////////////////////////////////////////////////////////////////////

#define	dMAX_LIGHT	20

class	cGAME_SELECT_AVATAR	:	public CGame
{
public:
	static	cGAME_UTIL::cLIGHT	s_aLight[dMAX_LIGHT];

	static	cSPRITE		s_sprSprite;
	static	cANM		s_anmLight;

	static	char		s_astrAvatarField[dMAX_AVATAR_COUNT][256];
	static	int			s_iAvatarCount;
	static	CActor		s_aModel[dMAX_AVATAR_COUNT];	//모델.. -_-a
	static	int			s_iModelFrame;
	static	BOOL		s_isWaitWorldServerConnectMessage;	//	서버 컨넥트 메시지 기다리는중
	static	BOOL		s_bIsWaitAvatarList;
	static	BOOL		s_bIsWaitSelectAvatarResult;
	static	int			s_iTimeOfConnectTry;				//	컨넥트 시도 시간

	static	BOOL		Run();
	static	BOOL		Draw();

	static	BOOL		UpdateInit();
	static	BOOL		UpdateReady();
	static	BOOL		UpdateMain();
	static	BOOL		UpdateFinish();
	static	BOOL		UpdateClose();
	static	void		UpdateModel();

	static	void		DrawAvatarList(int _iDx,int _iDy);//아바타 정보를 그려준다.
	static	void		DrawLight();
	static	void		DrawInit();
	static	void		DrawReady();
	static	void		DrawMain();
	static	void		DrawFinish();
	static	void		DrawClose();

	static	void		CopySelectAvatarDataToTestServer();
	static	void		CopySelectAvatarDataToDuelServer();

	static	BOOL		OperatePacket();
	static	BOOL		OperateMessageBox();
	static	void		UpdateAvatarList(BOOL _isCheck=FALSE);
	static	void		UpdateLight();

};

#endif