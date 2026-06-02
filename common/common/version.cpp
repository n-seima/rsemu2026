#include "version.h"

int		g_aiServerVersion[dSERVER_TYPE_COUNT]	=
{
	1	,	//게임 서버 버젼
	1	,	//월드 서버 버젼
	1	,	//아바타 DBC 버젼
	1	,	//아바타 DBC 공유 메모리 관리자 버젼
	1	,	//유저 DBC 서버 버젼
	1	,	//로그인 서버 버젼
	1	,	//로그 서버 버젼
	1	,	//커뮤니티 서버 버젼
};

DWORD	g_adwCurrentServerVersion[dSERVER_TYPE_COUNT]	=	
{
	0	,	//게임 서버 버젼
	0	,	//월드 서버 버젼
	0	,	//아바타 DBC 버젼
	0	,	//아바타 DBC 공유 메모리 관리자 버젼
	0	,	//유저 DBC 서버 버젼
	0	,	//로그인 서버 버젼
	0	,	//로그 서버 버젼
	0	,	//커뮤니티 서버 버젼
};
