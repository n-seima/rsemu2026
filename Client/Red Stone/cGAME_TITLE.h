#ifndef _classGAME_TITLE_H_
#define	_classGAME_TITLE_H_

#include "CGame.H"


class	cLoginServerList	:	public CGame
{
public:

};
////////////////////////////////////////////////////////////////////////
//	게임 로고
////////////////////////////////////////////////////////////////////////
class	cGAME_TITLE	:	public CGame
{
public:
	static	char		s_strID[dID_LENGTH],s_strPass[dPASS_LENGTH];
	static	BOOL		s_isWaitWorldServerConnectMessage;
	static	int			s_iFocusServer,s_iTimeOfConnectTry;//연결을 시도한 시간
	static	char		s_strFocusServerName[dNAME_LENGTH];
	static	DWORD		s_dwWaitForSafeDisconnectTime;
	static	cSPRITE		s_sprSprite,s_sprTitle;
	static	cRECT		s_rectServerListBorder,s_rectFocusWorld;

	static	WORD		*s_pPannel;

	static	BOOL		Run();
	static	BOOL		Draw();


	static	BOOL		UpdateInit();
	static	BOOL		UpdateReady();
	static	BOOL		UpdateMain();
	static	BOOL		UpdateFinish();
	static	BOOL		UpdateClose();
	static	int			GetSelectWorld();

	static	void		DrawInit();
	static	void		DrawReady();
	static	void		DrawMain();
	static	void		DrawFinish();
	static	void		DrawClose();
	static	void		DrawWorldList();

	static	BOOL		HoldConnectState();//	접속 상태를 유지한다.
	static	BOOL		WaitConnectMessage();
	static	void		UpdateServerList();
	static	BOOL		OperateMessageBox();
	static	BOOL		OperatePacket();

	static	cRECT		s_arectList[100];
	static	int			s_iLoginServerLineCount,s_iCurrentLoginServer,s_iFocusLoginServer;
	static	void		DrawLoginServerList();
	static	int			GetFocusLoginServer();
};

#endif