#ifndef _classUTIL_H
#define _classUTIL_H

#include "SFC.H"
#include "RSComm.H"

#define	dMAX_GAME_SERVER_COUNT		100

struct	SERVERStoWORLD_ACCOUNT_INFO;

class	cWorldUtil
{
public:
	CRITICAL_SECTION			m_csWork;

	int		m_aPlayerCount[dMAX_GAME_SERVER_COUNT];
	WORD	m_awPlayerCountPerLevel[dMAX_GAME_SERVER_COUNT][2000];
	DWORD	m_dwLastSendAccountInfoTime;
	
	WORD	m_wYear,m_wMonth,m_wDay;
	WORD	m_awPlayerCountInField[24][1000];
	WORD	m_awPlayerCountInSecretDungeon[24][200];
	WORD	m_wIsExportedPlayerCountPerFiledInfo;

			cWorldUtil();
			~cWorldUtil();

	int		getPlayerCount();
	void	setPlayerCount(SERVERStoWORLD_ACCOUNT_INFO *_lpPacket);

	BOOL	isSendAccountInfoTime();

	void	updatePlayerCountInField(int _iHour,int _iCount,WORD *_lpwList);
};

extern	cWorldUtil	g_worldUtil;

extern	char		strWorldName[dWORLD_NAME_LENGTH];

#endif