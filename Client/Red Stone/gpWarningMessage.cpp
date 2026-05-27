#include "CGamePlay.H"
#include "CDebugWindow.H"
#include "cMessage.H"
#include "cHero.H"

char	CGamePlay::s_strWarningMessage[256];
DWORD	CGamePlay::s_dwBeginWarningMessageTime	=	0;

void
CGamePlay::WarningMessage(char *_lpstrMessage)
{
	strcpy(s_strWarningMessage,_lpstrMessage);

	s_dwBeginWarningMessageTime	=	timeGetTime();
}

void
CGamePlay::DrawWarningMessage()
{
#ifdef	_DEBUG
	if	(g_bIsHideDamageAndHideName)
		return;
#endif

	if	(s_dwBeginWarningMessageTime	==	0)
		return;

	DWORD	dwCurrentTime	=	timeGetTime();

	if (dwCurrentTime > s_dwBeginWarningMessageTime+3900 ||	dwCurrentTime < s_dwBeginWarningMessageTime)
	{
		s_dwBeginWarningMessageTime	=	0;
		return;
	}

	int	iPos	=	g_iScreenHeight*2/9;

	cDRAW::FillHB(0,0,iPos,g_iScreenWidth,iPos+20);

	if (((dwCurrentTime-s_dwBeginWarningMessageTime)%2000) < 1700)
		s_textBold14.cPut(g_iScreenWidth/2,iPos+4,LTYELLOW,s_strWarningMessage);
}

BOOL
CGamePlay::CheckBCS(BOOL _bIsRequireWarning)
{
	if	(g_hero.m_wIsProcessInitData)
		return	FALSE;

	if	(s_agent.isConnectedBCServer()	==	FALSE)
	{
		AddSystemMessage(LTYELLOW,dMSG_DISCONNECTED_TO_BROAD_CAST_SERVER);

		if	(_bIsRequireWarning)
			AddSystemMessage(LTYELLOW,dMSG_NOT_SAVE_THIS_SETTING_DATA);
		
		return	FALSE;
	}

	return	TRUE;
}