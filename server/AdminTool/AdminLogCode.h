#ifndef __ADMINLOGCODE_H
#define __ADMINLOGCODE_H

#include "SFC.h"
#include "RSComm.h"
#include "cSOCKET.h"

	#define dADMINLOGSERVERPORT		dTOOLLOGSERVER_PORT
#ifdef _FOR_CHINA
	#define dADMINLOGSERVERIP		"192.168.0.209"
#elif _FOR_JAPAN
	#define dADMINLOGSERVERIP		""
#elif _FOR_USA
	#define dADMINLOGSERVERIP		""
#elif _FOR_INTER
	#define dADMINLOGSERVERIP		""
#elif _FOR_KOREA
	#define dADMINLOGSERVERIP		"211.189.66.54"
//	#define dADMINLOGSERVERIP		"211.235.238.107"
#endif

class	cToolLog	
{
public:
	BOOL	isEnable();

	BOOL	sendLogPacket(void *_lpPacket,int _iSize);	//	실제로 패킷을 전송함
	BOOL	failedLog();
	BOOL	loadPlayerData(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName);

	BOOL	addItemToPlayer(void *_lpItem,char *_lpstrId,char *_lpstrName,char *_lpstrWorldName);
	BOOL	addItemToBank(void *_lpItem,char *_lpstrId,char *_lpstrWorldName);
	BOOL	removeItemFromPlayer(void *_lpItem,char *_lpstrId,char *_lpstrName,char *_lpstrWorldName);
	BOOL	removeItemFromBank(void *_lpItem,char *_lpstrId,char *_lpstrWorldName);
	BOOL	renewPlayItemPeriod(void *_lpItem,int iExtendDay,int iExtendHour,int iExtendMinute,char *_lpstrId,char *_lpstrName,char *_lpstrWorldName);
	BOOL	renewBankItemPeriod(void *_lpItem,int iExtendDay,int iExtendHour,int iExtendMinute,char *_lpstrId,char *_lpstrWorldName);
	BOOL	addGoldToPlayer(DWORD _dwGold,char *_lpstrId,char *_lpstrName,char *_lpstrWorldName);
	BOOL	addGoldToBank(DWORD _dwGold,char *_lpstrId,char *_lpstrWorldName);
	BOOL	removeGoldFromPlayer(DWORD _dwGold,char *_lpstrId,char *_lpstrName,char *_lpstrWorldName);
	BOOL	removeGoldFromBank(DWORD _dwGold,char *_lpstrId,char *_lpstrWorldName);
	BOOL	changeLevel(char *_lpstrId,char *_lpstrName,int _iOldLevel,int _iDestLevel,char *_lpstrWorldName);
	BOOL	workMessage(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,char *_lpstrWork);
	BOOL	moveTo(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iWork);
	BOOL	dataCopy(char *_lpstrId,char *_lpstrName,char *_lpstrTargetId,char *_lpstrTargetName,char *_lpstrWorldName);

	BOOL	changeAvatarInfo(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew,int _iPacketType);
	BOOL	changeAvatarIndex(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew);
	BOOL	changeCurrentField(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew);
	BOOL	changeSecretDungeonPlayCount(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew);
	BOOL	changeGuildDungeonPlayCount(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew);
	BOOL	changeExtraInventorySize(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew);
	BOOL	changePeneltyTime(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew);
	BOOL	changeDeathPeneltyTime(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iOld,int _iNew);

	BOOL	addExp(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName,int _iExp);
	BOOL	resetSkillPoint(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName);
	BOOL	resetStatePoint(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName);


	BOOL	savePlayer(char *_lpstrId,char *_lpstrName,char *_lpstrWorldName);
	BOOL	saveBankData(char *_lpstrId,char *_lpstrWorldName);
};

extern	cToolLog	g_toolLog;
extern	BOOL		g_bIsRequireSendToolLog;	//	툴 로그를 전송할 필요가 있음.

//	아래 내용들은 AdminLogCode.cpp 파일을 만들어서 넣음
extern	char		strAdminId[dID_LENGTH];
extern	char		strAdminIp[dIP_SIZE];
extern	cSOCKET		logSock;

#endif	//	__ADMINLOGCODE_H