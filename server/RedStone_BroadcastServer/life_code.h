#pragma	once

#include	"life_code_common.H"

enum
{
	eLIFE_CODE_WORKER,
	eLIFE_CODE_ACCEPT_LOOP,
	eLIFE_CODE_ADDITIONAL,
	eLIFE_CODE_FRIEND_LIST_UPDATE,
	eLIFE_CODE_WORLD,
	eLIFE_CODE_AVATAR_DB,
	eLIFE_CODE_GUILD_MEMBER_LIST_UPDATE,		// 길드원 맴버 리스트
	eLIFE_CODE_COUNT,
};

extern	cLifeCode_Mgr	g_lifeCodeMgr;

extern	void	InitLifeCode();
extern	BOOL	SendWPLifeCode(int _iStatus,char *_lpstrIP,char *_lpstrMsg=NULL);
