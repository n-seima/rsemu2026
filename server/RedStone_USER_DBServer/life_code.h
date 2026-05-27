#pragma	once

#include	"life_code_common.H"

enum
{
	eLIFE_CODE_ADDITIONAL,
	eLIFE_CODE_COMMAND,
	eLIFE_CODE_CONSOLE_COMMAND,
	eLIFE_CODE_ACCEPT,
	eLIFE_CODE_EVENT_MGR,
	eLIFE_CODE_RECEIVE,
	eLIFE_CODE_WORKER,

	eLIFE_CODE_COUNT,
};

extern	cLifeCode_Mgr	g_lifeCodeMgr;

class	CClient;

extern	void	InitLifeCode();
extern	BOOL	SendLPLifeCode(int _iStatus,CClient *_lpLoginServer,char *_lpstrIP,char *_lpstrMsg=NULL);
