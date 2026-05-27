#include	"life_code.h"
#include	"cgame.h"

cLifeCode_Mgr	g_lifeCodeMgr;

char	*l_lpstrServerIP	=	NULL;

void
InitLifeCode(char *_lpstrIP)
{
	l_lpstrServerIP	=	_lpstrIP;

	g_lifeCodeMgr.init(eLIFE_CODE_COUNT);
	g_lifeCodeMgr.set(eLIFE_CODE_ACCEPT_LOOP,"eLIFE_CODE_ACCEPT_LOOP");
	g_lifeCodeMgr.set(eLIFE_CODE_WORKER,"eLIFE_CODE_WORKER");
	g_lifeCodeMgr.set(eLIFE_CODE_BC_CONTROL,"eLIFE_CODE_BC_CONTROL");
	g_lifeCodeMgr.set(eLIFE_CODE_ADDITIONAL,"eLIFE_CODE_ADDITIONAL");
	g_lifeCodeMgr.set(eLIFE_CODE_DATA_SAVE,"eLIFE_CODE_DATA_SAVE");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD_CTRL,"eLIFE_CODE_WORLD_CTRL");
	g_lifeCodeMgr.set(eLIFE_CODE_BC_PACKET,"eLIFE_CODE_BC_PACKET");
	g_lifeCodeMgr.set(eLIFE_CODE_AVATAR_DB,"eLIFE_CODE_AVATAR_DB");
	g_lifeCodeMgr.set(eLIFE_CODE_GAME,"eLIFE_CODE_GAME");


}

//	단순 작업
BOOL
SendWPLifeCode(int _iStatus,char *_lpstrIP,char *_lpstrMsg)
{
	SERVERStoWORLD_LIFE_SIGN	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_LIFE_SIGN);

	packet.wSign		=	_iStatus;	
	packet.iServerIndex	=	eLC_GameServer0+g_iServerIndex;

	if	(_lpstrMsg)
		strcpy(packet.strMsg,_lpstrMsg);
	else
		strcpy(packet.strMsg,"");

	strcpy(packet.strIP,l_lpstrServerIP);

	return	SEND_WOLRD_PACKET(&packet);
}