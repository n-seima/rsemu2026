#include	"life_code.h"
#include	"Client.h"
#include	"server.h"

cLifeCode_Mgr	g_lifeCodeMgr;
BOOL			g_bIsLifeCodeLoop	=	TRUE;

void
InitLifeCode()
{
	g_lifeCodeMgr.init(eLIFE_CODE_COUNT);

	g_lifeCodeMgr.set(eLIFE_CODE_USER_DBC,"eLIFE_CODE_USER_DBC");
	g_lifeCodeMgr.set(eLIFE_CODE_ACCEPT_LOOP,"eLIFE_CODE_ACCEPT_LOOP");
	g_lifeCodeMgr.set(eLIFE_CODE_ADDITIONAL,"eLIFE_CODE_ADDITIONAL");
	g_lifeCodeMgr.set(eLIFE_CODE_DBC,"eLIFE_CODE_DBC");
	g_lifeCodeMgr.set(eLIFE_CODE_COMMAND,"eLIFE_CODE_COMMAND");
	g_lifeCodeMgr.set(eLIFE_CODE_WORKER,"eLIFE_CODE_WORKER");
	g_lifeCodeMgr.set(eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA,"eLIFE_CODE_SEND_GAME_SERVER_INIT_DATA");
}

//	단순 작업
BOOL
SendLPLifeCode(int _iStatus,CClient *_lpLoginServer,char *_lpstrIP,char *_lpstrMsg)
{
	if	(!_lpLoginServer)
		return	FALSE;

	WORLDtoSERVERS_LIFE_SIGN	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_LIFE_SIGN);

	packet.wSign			=	_iStatus;	
	packet.iServerIndex		=	eLC_World;
	packet.iGameServerCount	=	g_iGameServerCount;

	strcpy(packet.strIP,_lpstrIP);

	if	(_lpstrMsg)
		strcpy(packet.strMsg,_lpstrMsg);
	else
		strcpy(packet.strMsg,"");

	_lpLoginServer->AddSendPacket((char*)&packet,packet.base.wSize);

	return	TRUE;
}

BOOL
OperateLifeSignPacket(CClient *_lpLoginServer,SERVERStoWORLD_LIFE_SIGN *_lpPacket)
{
	if	(!_lpLoginServer)
		return	FALSE;

	WORLDtoSERVERS_LIFE_SIGN	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_LIFE_SIGN);

	packet.wSign			=	_lpPacket->wSign;	
	packet.iServerIndex		=	_lpPacket->iServerIndex;
	packet.iGameServerCount	=	g_iGameServerCount;

	strcpy(packet.strIP,_lpPacket->strIP);
	strcpy(packet.strMsg,_lpPacket->strMsg);

	_lpLoginServer->AddSendPacket((char*)&packet,packet.base.wSize);

	return	TRUE;
}
