#include	"life_code.h"
#include	"Client.h"
#include	"cPACKET_WORLDSERVER.h"

cLifeCode_Mgr	g_lifeCodeMgr;

void
InitLifeCode()
{
	g_lifeCodeMgr.init(eLIFE_CODE_COUNT);

	g_lifeCodeMgr.set(eLIFE_CODE_ADDITIONAL,"eLIFE_CODE_ADDITIONAL");
	g_lifeCodeMgr.set(eLIFE_CODE_EVENT_MGR,"eLIFE_CODE_EVENT_MGR");
	g_lifeCodeMgr.set(eLIFE_CODE_PAY_LETTER,"eLIFE_CODE_PAY_LETTER");
	g_lifeCodeMgr.set(eLIFE_CODE_WORKER,"eLIFE_CODE_WORKER");
	g_lifeCodeMgr.set(eLIFE_CODE_ACCEPT,"eLIFE_CODE_ACCEPT");
	g_lifeCodeMgr.set(eLIFE_CODE_COMMAND,"eLIFE_CODE_COMMAND");
}

//	단순 작업
BOOL
SendWPLifeCode(int _iStatus,CClient *_lpWS,char *_lpstrIP,char *_lpstrMsg)
{
	if	(!_lpWS	||	_lpWS->GetSrvType()	!=	CClient::eSRVTYPE_WORLD)
		return	FALSE;

	SERVERStoWORLD_LIFE_SIGN	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_LIFE_SIGN);

	packet.wSign		=	_iStatus;	
	packet.iServerIndex	=	eLC_AvatarDBC;
	strcpy(packet.strIP,_lpstrIP);

	if	(_lpstrMsg)
		strcpy(packet.strMsg,_lpstrMsg);
	else
		strcpy(packet.strMsg,"");

	_lpWS->AddSendPacket((char*)&packet,packet.base.wSize);

	return	TRUE;
}

