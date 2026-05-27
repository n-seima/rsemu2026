#include "AdminLogCode.h"
#include "cPACKET_LOGSERVER.h"
#include "cPACKET_TOOLLOGSERVER.h"

cToolLog		g_toolLog;
BOOL			g_bIsRequireSendToolLog	=	FALSE;

cSOCKET			logSock;
char			strAdminId[dID_LENGTH];
char			strAdminIp[dIP_LENGTH];

BOOL
cToolLog::isEnable()
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	return	TRUE;
}

BOOL
cToolLog::sendLogPacket(void *_lpPacket,int _iSize)
{
	if	(!isEnable())
		return	TRUE;

	if	(!logSock.SendPacket((char *)_lpPacket,_iSize))
	{
		cMSG::Put("error!! in send log packet","Could not Send to Log Server!!");

		return	FALSE;
	}

	return	TRUE;
}

BOOL
cToolLog::loadPlayerData(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_LOAD_PLAYER_DATA	packet;

	packet.base.set(sizeof(packet),ePACKET_LOAD_PLAYER_DATA);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	strcpy(packet.strServerName,_lpstrWorldName);

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::addItemToPlayer(void *_lpItem,char *_lpstrId,char *_lpstrName,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_INSERTITEM	packet;

	packet.base.set(sizeof(packet),ePACKET_INSERTITEM);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strMsg);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	strcpy(packet.strServerName,_lpstrWorldName);
	memcpy(&packet.item,_lpItem,sizeof(CItemDefine));

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::addItemToBank(void *_lpItem,char *_lpstrId,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_INSERTITEMTOBANK	packet;

	packet.base.set(sizeof(packet),ePACKET_INSERTITEMTOBANK);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strMsg);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,"");
	strcpy(packet.strServerName,_lpstrWorldName);
	memcpy(&packet.item,_lpItem,sizeof(CItemDefine));

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::removeItemFromPlayer(void *_lpItem,char *_lpstrId,char *_lpstrName,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_REMOVEITEM	packet;

	packet.base.set(sizeof(packet),ePACKET_REMOVEITEM);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strMsg);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	strcpy(packet.strServerName,_lpstrWorldName);
	memcpy(&packet.item,_lpItem,sizeof(CItemDefine));

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::removeItemFromBank(void *_lpItem,char *_lpstrId,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_REMOVEITEM_FROM_BANK	packet;

	packet.base.set(sizeof(packet),ePACKET_REMOVEITEM_FROM_BANK);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strMsg);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,"");
	strcpy(packet.strServerName,_lpstrWorldName);
	memcpy(&packet.item,_lpItem,sizeof(CItemDefine));

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::renewPlayItemPeriod(void *_lpItem,int iExtendDay,int iExtendHour,int iExtendMinute,char *_lpstrId,char *_lpstrName,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_RENEW_PLAYER_ITEM	packet;

	packet.base.set(sizeof(packet),ePACKET_RENEW_PLAYER_ITEM);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	strcpy(packet.strServerName,_lpstrWorldName);
	memcpy(&packet.item,_lpItem,sizeof(CItemDefine));

	packet.wDay		=	iExtendDay;
	packet.wHour	=	iExtendHour;
	packet.wMinute	=	iExtendMinute;

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::renewBankItemPeriod(void *_lpItem,int iExtendDay,int iExtendHour,int iExtendMinute,char *_lpstrId,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_RENEW_BANK_ITEM	packet;

	packet.base.set(sizeof(packet),ePACKET_RENEW_BANK_ITEM);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,"");
	strcpy(packet.strServerName,_lpstrWorldName);
	memcpy(&packet.item,_lpItem,sizeof(CItemDefine));

	packet.wDay		=	iExtendDay;
	packet.wHour	=	iExtendHour;
	packet.wMinute	=	iExtendMinute;

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::addGoldToPlayer(DWORD _dwGold,char *_lpstrId,char *_lpstrName,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_ADDGOLD	packet;

	packet.base.set(sizeof(packet),ePACKET_REMOVEITEM);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	strcpy(packet.strServerName,_lpstrWorldName);
	packet.dwAddGold	=	_dwGold;

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::addGoldToBank(DWORD _dwGold,char *_lpstrId,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_ADDGOLDTOBANK	packet;

	packet.base.set(sizeof(packet),ePACKET_ADDGOLDTOBANK);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,"");
	strcpy(packet.strServerName,_lpstrWorldName);
	packet.dwAddGold	=	_dwGold;

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::removeGoldFromPlayer(DWORD _dwGold,char *_lpstrId,char *_lpstrName,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_REMOVEGOLD	packet;

	packet.base.set(sizeof(packet),ePACKET_REMOVEGOLD);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	strcpy(packet.strServerName,_lpstrWorldName);
	packet.dwRemoveGold	=	_dwGold;

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::removeGoldFromBank(DWORD _dwGold,char *_lpstrId,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_REMOVEGOLDTOBANK	packet;

	packet.base.set(sizeof(packet),ePACKET_REMOVEGOLDTOBANK);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,"");
	strcpy(packet.strServerName,_lpstrWorldName);
	packet.dwRemoveGold	=	_dwGold;

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::savePlayer(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_DATASAVE		packet;

	packet.base.set(sizeof(packet),ePACKET_DATASAVE);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	strcpy(packet.strServerName,_lpstrWorldName);

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::saveBankData(char *_lpstrId,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_SAVE_BANK_DATA		packet;

	packet.base.set(sizeof(packet),ePACKET_SAVE_BANK_DATA);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,"");
	strcpy(packet.strServerName,_lpstrWorldName);

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::changeLevel(char *_lpstrId,char *_lpstrName,int _iOldLevel,int _iDestLevel,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_CHANGELEVEL	packet;
	packet.base.set(sizeof(packet),ePACKET_CHANGELEVEL);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strServerName,_lpstrWorldName);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	packet.wOldLev	=	(WORD)_iOldLevel;
	packet.wNewLev	=	(WORD)_iDestLevel;
	
	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::workMessage(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,char *_lpstrWorkMessage)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_WORK_MESSAGE	packet;
	packet.base.set(sizeof(packet),ePACKET_WORK_MESSAGE);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strMessage)+strlen(_lpstrWorkMessage)+2;

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strServerName,_lpstrWorldName);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	strcpy(packet.strMessage,_lpstrWorkMessage);
	
	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::resetSkillPoint(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_RESETSKILLPOINT	packet;
	packet.base.set(sizeof(packet),ePACKET_RESETSKILLPOINT);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strServerName,_lpstrWorldName);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	
	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::resetStatePoint(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_RESETSTATPOINT	packet;
	packet.base.set(sizeof(packet),ePACKET_RESETSTATPOINT);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strServerName,_lpstrWorldName);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	
	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::moveTo(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iWork)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_MOVETO	packet;

	packet.base.set(sizeof(packet),ePACKET_MOVETO);
	strcpy(packet.strId,strAdminId);
	strcpy(packet.strServerName,_lpstrWorldName);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);

	packet.wPoint	=	_iWork;

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::dataCopy(char *_lpstrTargetId,char *_lpstrTargetName,char *_lpstrSourceId,char *_lpstrSourceName,char *_lpstrWorldName)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_DATACOPY		packet;
	packet.base.set(sizeof(packet),ePACKET_DATACOPY);
	strcpy(packet.strId,strAdminId);
	strcpy(packet.strServerName,_lpstrWorldName);
	strcpy(packet.strSourceId,_lpstrSourceId);
	strcpy(packet.strSourceName,_lpstrSourceName);
	strcpy(packet.strTargetId,_lpstrTargetId);
	strcpy(packet.strTargetName,_lpstrTargetName);

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::changeAvatarInfo(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew,int _iType)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_CHANGE_AVATAR_INDEX		packet;
	packet.base.set(sizeof(packet),_iType);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	strcpy(packet.strServerName,_lpstrWorldName);

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::changeAvatarIndex(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew)
{
	return	changeAvatarInfo(_lpstrId,_lpstrName,_lpstrWorldName,_iOld,_iNew,ePACKET_CHANGE_AVATAR_INDEX);
}

BOOL
cToolLog::changeCurrentField(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew)
{
	return	changeAvatarInfo(_lpstrId,_lpstrName,_lpstrWorldName,_iOld,_iNew,ePACKET_CHANGE_CURRENT_FIELD);
}

BOOL
cToolLog::changeSecretDungeonPlayCount(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew)
{
	return	changeAvatarInfo(_lpstrId,_lpstrName,_lpstrWorldName,_iOld,_iNew,ePACKET_CHANGE_SECRET_DUNGEON_PLAY_COUNT);
}

BOOL
cToolLog::changeGuildDungeonPlayCount(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew)
{
	return	changeAvatarInfo(_lpstrId,_lpstrName,_lpstrWorldName,_iOld,_iNew,ePACKET_CHANGE_GUILD_DUNGEON_PLAYTIME);
}

BOOL
cToolLog::changeExtraInventorySize(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew)
{
	return	changeAvatarInfo(_lpstrId,_lpstrName,_lpstrWorldName,_iOld,_iNew,ePACKET_CHANGE_EXTRA_INVENTORY_SIZE);
}

BOOL
cToolLog::changePeneltyTime(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew)
{
	return	changeAvatarInfo(_lpstrId,_lpstrName,_lpstrWorldName,_iOld,_iNew,ePACKET_CHANGE_PENELTY_TIME);
}

BOOL
cToolLog::changeDeathPeneltyTime(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew)
{
	return	changeAvatarInfo(_lpstrId,_lpstrName,_lpstrWorldName,_iOld,_iNew,ePACKET_CHANGE_DEATH_PENELTY_TIME);
}

BOOL
cToolLog::addExp(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iExp)
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_ADDEXP	packet;
	packet.base.set(sizeof(packet),ePACKET_ADDEXP);

	strcpy(packet.strId,strAdminId);
	strcpy(packet.strUserId,_lpstrId);
	strcpy(packet.strUserName,_lpstrName);
	strcpy(packet.strServerName,_lpstrWorldName);
	packet.iAddExp	=	_iExp;

	return	sendLogPacket(&packet,packet.base.wSize);
}

BOOL
cToolLog::failedLog()
{
	if	(g_bIsRequireSendToolLog	==	FALSE)
		return	TRUE;

	PACKET_FAILED_WORK	packet;

	packet.base.set(sizeof(packet),ePACKET_FAILED_WORK);

	strcpy(packet.strId,strAdminId);

	return	sendLogPacket(&packet,packet.base.wSize);
}