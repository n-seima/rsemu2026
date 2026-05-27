#include	"life_code.h"
#include	"Client.h"
#include	"cPACKET_WORLDSERVER.h"

cLifeCode_Mgr	g_lifeCodeMgr;

void
InitLifeCode()
{
	g_lifeCodeMgr.init(eLIFE_CODE_COUNT);

	g_lifeCodeMgr.set(eLIFE_CODE_ADDITIONAL,"eLIFE_CODE_ADDITIONAL");
	g_lifeCodeMgr.set(eLIFE_CODE_COMMAND,"eLIFE_CODE_COMMAND");
	g_lifeCodeMgr.set(eLIFE_CODE_EVENT_MGR,"eLIFE_CODE_EVENT_MGR");
	g_lifeCodeMgr.set(eLIFE_CODE_RECEIVE,"eLIFE_CODE_RECEIVE");
	g_lifeCodeMgr.set(eLIFE_CODE_CONSOLE_COMMAND,"eLIFE_CODE_CONSOLE_COMMAND");
	g_lifeCodeMgr.set(eLIFE_CODE_WORKER,"eLIFE_CODE_WORKER");
	g_lifeCodeMgr.set(eLIFE_CODE_ACCEPT,"eLIFE_CODE_ACCEPT");
}

//	단순 작업
BOOL
SendLPLifeCode(int _iStatus,CClient *_lpLoginServer,char *_lpstrIP,char *_lpstrMsg)
{
	if	(!_lpLoginServer)
		return	FALSE;

	WORLDtoSERVERS_LIFE_SIGN	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_LIFE_SIGN);

	packet.wSign		=	_iStatus;	
	packet.iServerIndex	=	eLC_UserDBCServer;
	strcpy(packet.strIP,_lpstrIP);

	if	(_lpstrMsg)
		strcpy(packet.strMsg,_lpstrMsg);
	else
		strcpy(packet.strMsg,"");

	_lpLoginServer->AddSendPacket((char*)&packet,packet.base.wSize);

	return	TRUE;
}