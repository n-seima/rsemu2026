#pragma	once

#include	"life_code_common.H"

enum
{
	eLIFE_CODE_ACCEPT_LOOP,
	eLIFE_CODE_WORKER,
	eLIFE_CODE_BC_CONTROL,
	eLIFE_CODE_ADDITIONAL,
	eLIFE_CODE_DATA_SAVE,
	eLIFE_CODE_WORLD_CTRL,
	eLIFE_CODE_BC_PACKET,
	eLIFE_CODE_AVATAR_DB,
	eLIFE_CODE_GAME,
	eLIFE_CODE_COUNT,
};

extern	cLifeCode_Mgr	g_lifeCodeMgr;

extern	void	InitLifeCode(char *_lpstrIP);
extern	BOOL	SendWPLifeCode(int _iStatus,char *_lpstrIP,char *_lpstrMsg=NULL);
