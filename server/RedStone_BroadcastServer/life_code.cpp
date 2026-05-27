#include	"life_code.h"
#include	"util.h"
#include	"cPACKET_WORLDSERVER.h"

cLifeCode_Mgr	g_lifeCodeMgr;

void
InitLifeCode()
{
	g_lifeCodeMgr.init(eLIFE_CODE_COUNT);

	g_lifeCodeMgr.set(eLIFE_CODE_WORKER,"eLIFE_CODE_WORKER");
	g_lifeCodeMgr.set(eLIFE_CODE_ACCEPT_LOOP,"eLIFE_CODE_ACCEPT_LOOP");
	g_lifeCodeMgr.set(eLIFE_CODE_ADDITIONAL,"eLIFE_CODE_ADDITIONAL");
	g_lifeCodeMgr.set(eLIFE_CODE_FRIEND_LIST_UPDATE,"eLIFE_CODE_FRIEND_LIST_UPDATE");
	g_lifeCodeMgr.set(eLIFE_CODE_WORLD,"eLIFE_CODE_WORLD");
	g_lifeCodeMgr.set(eLIFE_CODE_AVATAR_DB,"eLIFE_CODE_AVATAR_DB");
	g_lifeCodeMgr.set(eLIFE_CODE_GUILD_MEMBER_LIST_UPDATE,"eLIFE_CODE_GUILD_MEMBER_LIST_UPDATE");		// 길드 맴버리스트
}

//	단순 작업
BOOL
SendWPLifeCode(int _iStatus,char *_lpstrIP,char *_lpstrMsg)
{
	SERVERStoWORLD_LIFE_SIGN	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_LIFE_SIGN);

	packet.wSign		=	_iStatus;	
	packet.iServerIndex	=	eLC_Community;
	strcpy(packet.strIP,_lpstrIP);
	
	if	(_lpstrMsg)
		strcpy(packet.strMsg,_lpstrMsg);
	else
		strcpy(packet.strMsg,"");

	return	SendPacketToWS(&packet);
}