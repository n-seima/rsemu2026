#pragma	once

#include	"life_code_common.H"

enum
{
	eLIFE_CODE_ADDITIONAL,
	eLIFE_CODE_EVENT_MGR,
	eLIFE_CODE_PAY_LETTER,
	eLIFE_CODE_COMMAND,
	eLIFE_CODE_ACCEPT,
	eLIFE_CODE_WORKER,

	eLIFE_CODE_COUNT,
};

class	CClient;

extern	void	InitLifeCode();
extern	BOOL	SendWPLifeCode(int _iStatus,CClient *_lpWS,char *_lpstrIP,char *_lpstrMsg=NULL);


extern	cLifeCode_Mgr	g_lifeCodeMgr;
