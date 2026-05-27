#pragma	once

#include	"SFC.H"

enum
{
	eLC_World,
	eLC_AvatarDBC,
	eLC_Community,
	eLC_LoginServer,
	eLC_UserDBCServer,
	eLC_GameServer0,
	eLC_GameServer1,
	eLC_GameServer2,
	eLC_GameServer4,
	eLC_GameServer5,
	eLC_GameServer6,
	eLC_GameServer7,
	eLC_GameServer8,
	eLC_GameServer9,
	eLC_ServerCount,
};

enum
{
	eLCS_Sleep,
	eLCS_Begin,
	eLCS_IncorrectStatusSign,
	eLCS_Life,
	eLCS_End,
	eLCS_IsNotUse,
};

const	int	c_iDEFAULT_LIFE_CODE_DEATH_TIME		=	60*1000;

#define	dDEFAULT_LIFE_CODE_DEATH_TIME	c_iDEFAULT_LIFE_CODE_DEATH_TIME

class	cLifeCode_Info
{
public:
	BOOL	m_bIsCheckNow;
	DWORD	m_dwLastWorkTime;
	int		m_iDeathTime;
	char	m_strWarningLog[64];

	cLifeCode_Info()
	{
		memset(this,0,sizeof(cLifeCode_Info));

		m_iDeathTime	=	dDEFAULT_LIFE_CODE_DEATH_TIME;
	}

	inline	void	beginCheck()
	{
		putLifeSign();
		m_bIsCheckNow	=	TRUE;

	}
	inline	void	endCheck()
	{
		m_bIsCheckNow	=	FALSE;
	}
	inline	void	putLifeSign()
	{
		m_dwLastWorkTime=	timeGetTime();
	}

	BOOL	isLife(DWORD _dwCurTime);
};

class	cLifeCode_Mgr
{
public:
	cLifeCode_Info	*m_pLifeCode;
	int				m_iSize;
	char			m_strBadStatusLog[64];

	cLifeCode_Mgr();
	~cLifeCode_Mgr();

	void			init(int _iCount);
	void			set(int _iSlot,char *_lpstrWarningMsg,int _iDeathTime=dDEFAULT_LIFE_CODE_DEATH_TIME);

	inline	void	beginCheck(int _iSlot)
	{
		if	(!m_pLifeCode)
			return;

		m_pLifeCode[_iSlot].beginCheck();
	}
	inline	void	endCheck(int _iSlot)
	{
		if	(!m_pLifeCode)
			return;

		m_pLifeCode[_iSlot].endCheck();
	}
	inline	void	putLifeSign(int _iSlot)
	{
		if	(!m_pLifeCode)
			return;

		m_pLifeCode[_iSlot].putLifeSign();
	}

	inline	BOOL	isLife(int _iSlot,DWORD _dwCurTime)
	{
		if	(!m_pLifeCode)
			return	TRUE;

		return	m_pLifeCode[_iSlot].isLife(_dwCurTime);
	}

	inline	cLifeCode_Info*	get(int _iSlot);

	BOOL			checkAll(DWORD _dwCurTime);
};