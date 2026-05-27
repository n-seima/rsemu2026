#pragma	once

#include	"SFC.H"
#include	"cSOCKET.H"

extern	BOOL		g_bIsDevServer;
extern	int			g_iGameServerCount;
extern	int			g_iWorldServerIndex;
extern	BOOL		g_bIsInnerTestServer;
extern	int			g_iWorldType;
extern	CTimeInfo	g_currentTime;

class	cSOCKET;
class	cGuild;

extern	cSOCKET*	GetADBSocket();
extern	cSOCKET*	GetIDDBSocket();

extern	void		DeleteGuildPacket(DWORD _dwGameServerSerial,WORD _wGuildSerial,char * _strGuildName, char * _strRequestMan, WORD _wReason = eDGR_NORMAL);
extern	void		DeleteGuild(WORD _wGuildSerial, char * _strGuildName,char * _strWhy);
extern	void		UpdateGuildData(cGuild * guild,DWORD	_dwSerial = 0xffffffff);
