#pragma	once

#include	"SFC.H"
#include	"life_code_common.H"

class	cLifeCode_ServerInfo
{
public:
	int		m_iStatus;
	int		m_iLastReceiveCodeSecond;
	char	*m_lpstrWorldName;

	char	m_strIP[64];

			cLifeCode_ServerInfo()
			{
				reset();
				m_lpstrWorldName	=	NULL;
			}

	void	reset()
	{
		memset(this,0,sizeof(cLifeCode_ServerInfo));
	}

	BOOL	isLife(int _iCurrentSecod);
};

class	cLifeCode_WorldInfo
{
public:
	char					m_strWorldName[128];
	int						m_iStatus;
	int						m_iGameServerCount;
	cLifeCode_ServerInfo	m_aServers[eLC_ServerCount];

	cLifeCode_WorldInfo()
	{
		reset();
	}

	void	reset()
	{
		memset(this,0,sizeof(cLifeCode_ServerInfo));
	}
};

#define	dWORLD_COUNT_FOR_LIFE_SIGN	64

class	cLifeCode_LoginServerMgr
{
public:
	char					m_strIP[64];
	HANDLE					m_hLifeSign;
	int						m_iWorldCount;
	cLifeCode_WorldInfo		m_aWorldList[dWORLD_COUNT_FOR_LIFE_SIGN];
	cLifeCode_ServerInfo	m_userDbc;
	cFILE					m_fileLog;
	BOOL					m_bIsUpkeep;
	char					m_strMessage[128];
	int						m_iUserCount;

	int						m_iLastSMS_AliveSec;

	cLifeCode_LoginServerMgr();
	~cLifeCode_LoginServerMgr();

	void					beginThread();
	void					end();

	cLifeCode_WorldInfo*	getWorld(char *_lpstrName);	//	월드 얻기
	void					registWorld(char *_lpstrName,int _iGameServerCount);	//	월드 등록

	void					lifeSign(int _iSign,cLifeCode_ServerInfo *_lpServerInfo,char *_lpstrWorldName,int _iServerType,char *_lpstrIP,char *_lpstrMessage=NULL);

	void					runThread();
	void					update();

	void					setUserCount(int _iCount)
	{
		m_iUserCount	=	_iCount;
	}
};

enum
{
	eLIFE_CODE_ACCEPT_LOOP,
	eLIFE_CODE_ADDITIONAL,
	eLIFE_CODE_USER_DBC,
	eLIFE_CODE_RECEIVE,
	eLIFE_CODE_WORKER,
	eLIFE_CODE_WORLD0,
	eLIFE_CODE_WORLD1,
	eLIFE_CODE_WORLD2,
	eLIFE_CODE_WORLD3,
	eLIFE_CODE_WORLD4,
	eLIFE_CODE_WORLD5,
	eLIFE_CODE_WORLD6,
	eLIFE_CODE_WORLD7,
	eLIFE_CODE_WORLD8,
	eLIFE_CODE_WORLD9,
	eLIFE_CODE_WORLD10,
	eLIFE_CODE_WORLD11,
	eLIFE_CODE_WORLD12,
	eLIFE_CODE_WORLD13,
	eLIFE_CODE_WORLD14,
	eLIFE_CODE_WORLD15,
	eLIFE_CODE_WORLD16,

	eLIFE_CODE_COUNT,
};

extern	void	InitLifeCode(char *_lpstrIP);

extern	cLifeCode_LoginServerMgr	g_lifeCodeLogServerMgr;
extern	cLifeCode_Mgr				g_lifeCodeMgr;
extern	BOOL						g_bIsDisplayLifeSign;