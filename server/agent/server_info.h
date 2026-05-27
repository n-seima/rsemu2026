#pragma	once

#include	"server_type_define.h"
#include	"SFC.h"
#include	"cPACKET_AGENT.h"
#include	"COMMCTRL.h"

class	CAgentDlg;

#define	dIP_LENGTH				16
#define	dMAX_GAME_SERVER_COUNT	12

class	cServerInfo
{
public:
	int			m_iServerType;
	int			m_iWorldIndex;	//	월드서버 인덱스
	int			m_iGameIndex;	//	게임서버 인덱스
	int			m_iStatus;
	int			m_iVersion;

	char		m_strIP[dIP_LENGTH];
	char		m_strName[32];
	HTREEITEM	m_hTreeNode;

	cServerInfo()
	{
		memset(this,0,sizeof(cServerInfo));
		m_iServerType	=	-1;
	}
};

class	cWorldServerInfo
{
public:
	int			m_iIndex;
	int			m_iGameServerCount;

	cServerInfo	m_aServerList[eSTIW_COUNT];

	cWorldServerInfo()
	{
		memset(this,0,sizeof(cWorldServerInfo));
		m_iIndex	=	-1;
	}
};

class	cServerList
{
public:
	cServerInfo			m_aSingleServerList[dSINGLE_SERVER_COUNT];
	cWorldServerInfo	m_aWorldList[dMAX_WORLD_SERVER_COUNT];

	cWorldServerInfo*	getWorldByIP(char *_lpstrIP);	//	아이피로 월드 찾기
	void				updateServerInfo(cAgentServerInfo *_lpInfo,CAgentDlg *_lpMainDlg);
};

extern	cServerList		g_serverList;