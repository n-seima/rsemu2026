#pragma	once

#include	"life_code_common.H"
#include	"cPACKET_WORLDSERVER.H"

enum
{
	eLIFE_CODE_USER_DBC,
	eLIFE_CODE_ACCEPT_LOOP,
	eLIFE_CODE_ADDITIONAL,
	eLIFE_CODE_DBC,
	eLIFE_CODE_COMMAND,
	eLIFE_CODE_WORKER,
	eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA,

	eLIFE_CODE_COUNT,
};

extern	cLifeCode_Mgr	g_lifeCodeMgr;
extern	BOOL			g_bIsLifeCodeLoop;

class	CClient;

extern	void	InitLifeCode();
extern	BOOL	SendLPLifeCode(int _iStatus,CClient *_lpLoginServer,char *_lpstrIP,char *_lpstrMsg=NULL);
extern	BOOL	OperateLifeSignPacket(CClient *_lpLoginServer,SERVERStoWORLD_LIFE_SIGN *_lpPacket);