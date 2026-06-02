#pragma	once

enum
{
	eST_WORLD,
	eST_AVATAR_DBC,
	eST_SMMANAGER,
	eST_COMMUNITY,
	eST_GAME,

	eST_SINGLE_SERVER_BEGIN,
	eST_USER_DBC=	eST_SINGLE_SERVER_BEGIN,
	eST_LOGIN	,
	eST_LOG,
	eST_SINGLE_SERVER_END,
	eST_COUNT	=	eST_SINGLE_SERVER_END,
	eST_INVALID,
};

const	int	c_iSingleServerTypeCount	=	eST_SINGLE_SERVER_END-eST_SINGLE_SERVER_BEGIN;

#define	dSINGLE_SERVER_COUNT	c_iSingleServerTypeCount

enum
{
	eSTIW_WORLD,//	Server Type In World
	eSTIW_AVATAR_DBC,//	Server Type In World
	eSTIW_SMMANAGER,//	Server Type In World
	eSTIW_COMMUNITY,//	Server Type In World
	eSTIW_GAME0,	//	Server Type In World
	eSTIW_GAME1,	//	Server Type In World
	eSTIW_GAME2,	//	Server Type In World
	eSTIW_GAME3,	//	Server Type In World
	eSTIW_GAME4,	//	Server Type In World
	eSTIW_GAME5,	//	Server Type In World
	eSTIW_GAME6,	//	Server Type In World
	eSTIW_GAME7,	//	Server Type In World
	eSTIW_GAME8,	//	Server Type In World
	eSTIW_GAME9,	//	Server Type In World
	eSTIW_COUNT,
};

enum
{
	eSERVER_STATUS_OFF		,	//	정상적으로 꺼져 있는 경우
	eSERVER_STATUS_BOOTING	,	//	켜지는 중이다.
	eSERVER_STATUS_ON		,	//	켜졌다. 이 경우 프로세스가 죽어 있다면 자동 재시작
	eSERVER_STATUS_QUIT		,	//	서버 끄는중
};

const	int		c_ServerTypeCount	=	eST_COUNT;
#define	dSERVER_TYPE_COUNT	c_ServerTypeCount	

extern	char	*g_strServerName[];

extern	int		g_bIsBeginByAgent;