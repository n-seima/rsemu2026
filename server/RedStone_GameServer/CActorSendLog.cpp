#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "CGame.H"
#include "CDataSave.H"
#include "CGuild.H"
#include "cPacket_USERDBSERVER.h"
#include "cServerStatus.h"

void
cACTOR::sendTransAvatarLog(char *_lpstrTransName)	//	아이템 판매 결과
{		// 마지막 수정일 : 09.10.27
	LOG_S_TRNAS_AVATAR	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWork				=	eSLT_TRANS_AVATAR_TO_GVG_SERVER;

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);
	strcpy(logPacket.strNewAvatarName,_lpstrTransName);
	
	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}


void
cACTOR::sendBuyItemLog(cItem *_lpItem,DWORD _dwPrice,int _iCount,int _iMethod, WORD _wTokenItem)	//	아이템 판매 결과
{		// 마지막 수정일 : 09.10.27
	LOG_S_BUY_ITEM	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWork				=	eSLT_BUY_ITEM;
	logPacket.wBuyMethod		=	_iMethod;
	logPacket.dwBuyPrice		=	_dwPrice;
	logPacket.wCount			=	_iCount;
	logPacket.wTokenItem		=	_wTokenItem;		// 토큰아이템 추가...... 토큰으로 안살때는 0xffff 값이 들어간다.
	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	memcpy(&logPacket.cItem,_lpItem,sizeof(CItemDefine));

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendExtendMiniPetTimeLog(cMiniPet *_lpMiniPet,int _iIndex)
{
	LOG_S_EXTEND_MINI_PET_TIME	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWork				=	eSLT_EXTEND_MINI_PET_TIME;

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	memcpy(&logPacket.miniPet,_lpMiniPet,sizeof(cMiniPetCommon));
	logPacket.wSlot	=	_iIndex;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendFeedItemMiniPetLog(cItem *_lpItem,cMiniPet *_lpMiniPet,int _iIndex)
{
	LOG_S_MINI_PET_FEED_ITEM	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWork				=	eSLT_MINI_PET_FEED_ITEM;

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	memcpy(&logPacket.item,_lpItem,sizeof(CItemDefine));
	memcpy(&logPacket.miniPet,_lpMiniPet,sizeof(cMiniPetCommon));
	logPacket.wSlot	=	_iIndex;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}


void
cACTOR::sendSealMiniPetLog(cMiniPet *_lpMiniPet,int _iIndex)
{
	LOG_S_SEAL_MINI_PET	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWork				=	eSLT_SEAL_MINI_PET;

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	memcpy(&logPacket.miniPet,_lpMiniPet,sizeof(cMiniPetCommon));
	logPacket.wSlot	=	_iIndex;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendUnsealMiniPetLog(cMiniPet *_lpMiniPet,int _iIndex)
{
	LOG_S_UNSEAL_MINI_PET	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWork				=	eSLT_UNSEAL_MINI_PET;

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	memcpy(&logPacket.miniPet,_lpMiniPet,sizeof(cMiniPetCommon));
	logPacket.wSlot	=	_iIndex;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendCreateMiniPetLog(cItem *_lpItem,cMiniPet *_lpMiniPet,int _iIndex)
{
	LOG_S_CREATE_MINI_PET	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWork				=	eSLT_CREATE_MINI_PET;

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	memcpy(&logPacket.cItem,_lpItem,sizeof(CItemDefine));
	memcpy(&logPacket.miniPet,_lpMiniPet,sizeof(cMiniPetCommon));
	logPacket.wSlot	=	_iIndex;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendRebirthLog(int _iPreLevel,int _iPreExp)
{
	LOG_S_REBIRTH	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWork				=	eSLT_REBIRTH;

	logPacket.wRebirthCount		=	m_bf4RebirthCount;

	logPacket.wPreLevel			=	_iPreLevel;
	logPacket.iPreExp			=	_iPreExp;
	logPacket.wLevel			=	m_iLevel;
	logPacket.iExp				=	m_iExperience;

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendRestraintMiniPetLog(cMiniPet *_lpMiniPet)
{
	LOG_S_RESTRAINT_MINI_PET	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWork				=	eSLT_RESTRAINT_MINIPET;

	logPacket.m_wFireCount		=	m_bf4EatFireMiniPetCount;
	logPacket.m_wWaterCount		=	m_bf4EatWaterMiniPetCount;
	logPacket.m_wWindCount		=	m_bf4EatWindMiniPetCount;
	logPacket.m_wEarthCount		=	m_bf4EatEarthMiniPetCount;
	logPacket.m_wLightCount		=	m_bf4EatLightMiniPetCount;
	logPacket.m_wDarkCount		=	m_bf4EatDarkMiniPetCount;

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	memcpy(&logPacket.miniPet,_lpMiniPet,sizeof(cMiniPetCommon));

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	

}

void
cACTOR::sendReleaseMiniPetLog(cMiniPet *_lpMiniPet,int _iWork)
{
	if	(_iWork	==	-1)
		_iWork	=	eSLT_REMOVE_MINI_PET;

	LOG_S_RELEASE_MINI_PET	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWork				=	_iWork;

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	memcpy(&logPacket.miniPet,_lpMiniPet,sizeof(cMiniPetCommon));

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendRenewItemLog(cItem *_lpItem,int _iTerm)
{
	LOG_ENCHANT_ITEM	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_ENCHANT_ITEM);
	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWork				=	eEIW_RENEW_TERM;
	logPacket.wRenewTerm		=	_iTerm;

	memcpy(&logPacket.cItem,_lpItem,sizeof(cItem));	

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
			memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else	strcpy(logPacket.strIP,"invalid IP");

	if (logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","sendRenewItemLog");
		return;
	}
	
	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendCreateItemLog(cItem *_lpItem,int _iMethod,char *_lpstrGuildName)
{
	LOG_CREATE_ITEM	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_CREATE_ITEM);
	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wMethod			=	_iMethod;

	if	(_lpstrGuildName)
		strcpy(logPacket.strGuildName,_lpstrGuildName);
	else
		logPacket.base.wSize	-=	sizeof(logPacket.strGuildName);

	memcpy(&logPacket.cItem,_lpItem,sizeof(logPacket.cItem));

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	if (logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_CREATE_ITEM");
		return;
	}

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendDropItemLog(cItem *_lpItem,int _iMethod)
{
	LOG_ITEM_DROP	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_ITEM_DROP);
	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wMethod			=	_iMethod;
	memcpy(&logPacket.cItem,_lpItem,sizeof(logPacket.cItem));

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
			memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else	strcpy(logPacket.strIP,"invalid IP");

	if (logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_ITEM_DROP");
		return;
	}
	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendUseItemLog(cItem *_lpItem)
{
	LOG_USE_ITEM	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_USE_ITEM);
	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	memcpy(&logPacket.cItem,_lpItem,sizeof(logPacket.cItem));

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
			memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else	strcpy(logPacket.strIP,"invalid IP");

	if (logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_USE_ITEM");
		return;
	}

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendRemoveItemLog(cItem *_lpItem,int _iMethod,int _iField)
{
	LOG_ITEM_DEL	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_ITEM_DEL);
	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.bf6Method			=	_iMethod;
	logPacket.bf1IsOperator		=	FALSE;
	logPacket.bf1IsIncludeGuildInfo=	FALSE;//	로그에 길드 정보가 포함되어 있다.
	logPacket.bf8Spare			=	0;//	로그에 길드 정보가 포함되어 있다.
	logPacket.base.wSize		=	sizeof(logPacket)-sizeof(logPacket.strGuildName);

	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();

	if	(!lpBasicItem)
		return;

	if	(lpBasicItem->m_bf1IsRequireLogWithGuildInfo)
	{
		CGuild	*lpGuild		=	g_guildManager.getBySerial(m_wGuildSerial);

		if	(lpGuild)
		{
			logPacket.base.wSize=	sizeof(logPacket);
			logPacket.bf1IsIncludeGuildInfo	=	TRUE;

			strcpy(logPacket.strGuildName,lpGuild->m_strName);
		}
	}

	if	(_iField	==	-1)
		logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	else
		logPacket.wFieldUniqueIndex	=	_iField;

	if	(isTester())
		logPacket.bf1IsOperator	=	TRUE;

	memcpy(&logPacket.cItem,_lpItem,sizeof(logPacket.cItem));

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	if (logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_ITEM_DEL");
		return;
	}
	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendRepairItemLog(cItem *_lpItem,int _iCharge)
{
	LOG_ITEM_DEL	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_ITEM_DEL);
	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.iCharge			=	_iCharge;
	logPacket.bf1IsOperator		=	FALSE;
	logPacket.bf6Method			=	eRI_REPAIR_ITEM_CAHRGE;
	logPacket.bf1IsOperator		=	FALSE;
	logPacket.bf1IsIncludeGuildInfo=	FALSE;//	로그에 길드 정보가 포함되어 있다.
	logPacket.bf8Spare			=	0;//	로그에 길드 정보가 포함되어 있다.

	logPacket.base.wSize		=	sizeof(logPacket)-sizeof(logPacket.strGuildName);

	if	(isTester())
		logPacket.bf1IsOperator	=	TRUE;

	memcpy(&logPacket.cItem,_lpItem,sizeof(logPacket.cItem));

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	if	(logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_ITEM_DEL");
		return;
	}
	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendPickItemLog(cItem *_lpItem,int _iMethod)
{
	LOG_ITEM_PICK	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_ITEM_PICK);
	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.bf6Method			=	_iMethod;
	logPacket.bf1IsIncludeGuildInfo=	FALSE;
	logPacket.base.wSize		=	sizeof(logPacket)-sizeof(logPacket.strGuildName);
	logPacket.bf1IsOperator		=	isTester();

	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();

	if	(!lpBasicItem)
		return;

	if	(lpBasicItem->m_bf1IsRequireLogWithGuildInfo)
	{
		CGuild	*lpGuild		=	g_guildManager.getBySerial(m_wGuildSerial);

		if	(lpGuild)
		{
			logPacket.base.wSize=	sizeof(logPacket);
			logPacket.bf1IsIncludeGuildInfo	=	TRUE;

			strcpy(logPacket.strGuildName,lpGuild->m_strName);
		}
	}

	memcpy(&logPacket.cItem,_lpItem,sizeof(logPacket.cItem));

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	if	(logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_ITEM_PICK");
		return;
	}

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendCreatePremiumItemLog(cItem *_lpItem,int _iMethod,int _iCarrotPrice)
{
	LOG_ITEM_CREATE_PREMIUM_ITEM	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_ITEM_CREATE_PREMIUM_ITEM);
	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wMethod			=	_iMethod;
	logPacket.wCarrotPrice		=	_iCarrotPrice;

	memcpy(&logPacket.cItem,_lpItem,sizeof(logPacket.cItem));

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	if (logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_ITEM_CREATE_PREMIUM_ITEM");
		return;
	}
	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendSellItemLog(cItem *_lpItem,DWORD _dwPrice)
{
	LOG_ITEM_SELL	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_ITEM_SELL);
	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.dwSellPrice		=	_dwPrice;

	memcpy(&logPacket.cItem,_lpItem,sizeof(logPacket.cItem));

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	if (logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_ITEM_SELL");
		return;
	}
	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendExpiredItemLog(cItem *_lpItem)
{
	LOG_EXPIRED_ITEM	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_EXPIRED_ITEM);
	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	memcpy(&logPacket.cItem,_lpItem,sizeof(logPacket.cItem));

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
			memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else	strcpy(logPacket.strIP,"invalid IP");

	if (logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_EXPIRED_ITEM");
		return;
	}
	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendDoubleItemLog(cItem *_lpItem,int _iWhere)
{
	LOG_DOUBLE_ITEM	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_DOUBLE_ITEM);
	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWhere			=	_iWhere;

	memcpy(&logPacket.cItem,_lpItem,sizeof(logPacket.cItem));

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	if (logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_DOUBLE_ITEM");
		return;
	}
	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendEnchantItemLog(cItem *_lpEnchanted,int _iWork,DWORD _dwPrice)
{
	LOG_ENCHANT_ITEM	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_ENCHANT_ITEM);
	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.wWork				=	_iWork;
	logPacket.dwPrice			=	_dwPrice;

	memcpy(&logPacket.cItem,_lpEnchanted,sizeof(cItem));	

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	if (logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_ENCHANT_ITEM");
		return;
	}

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}
void
cACTOR::sendCarveItemLog(int _iWork, cItem *_lpTargetItem,WORD _wUseItemSerial,
						   WORD _bf8BeforCarvingLevel,WORD _bf8AfterCarvingLevel, DWORD _dwGold,  WORD _wCrystalCount)	
{
	LOG_CARVE_ITEM	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_ENCHANT_ITEM);
	logPacket.logTime.yy							=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm							=	g_currentTime.m_wMonth;
	logPacket.logTime.dd							=	g_currentTime.m_wDay;
	logPacket.logTime.hour							=	g_currentTime.m_wHour;
	logPacket.logTime.min							=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex						=	m_wCurrentField;
	logPacket.wWork									=	_iWork;
	logPacket.wUseItem								=	_wUseItemSerial;			
	logPacket.dwGold								=	_dwGold;						
	logPacket.bf5CrystalCount						=	_wCrystalCount;
	logPacket.bf8BeforCarvingLevel					=	_bf8BeforCarvingLevel;				//  강화 전  레벨
	logPacket.bf8AfterCarvingLevel					=	_bf8AfterCarvingLevel;				//  강화 후  레벨




	memcpy(&logPacket.cTargetItem,_lpTargetItem,sizeof(cItem));	

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	if (logPacket.cTargetItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_UPGRADE_ETERNAL_ITEM");
		return;
	}

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}
void
cACTOR::sendUpgradeItemLog(int _iWork, cItem *_lpTargetItem,WORD _wUpgradeItemSerial, DWORD _dwGold, WORD _wStoneOfMesteryCount,
						   WORD  _wTantillessCount, WORD _wCrystalCount, WORD _wUpgradeMaterialsItemSerial, WORD _wUpgradePremiumMaterialsItemSerial)		//  업그레이드 아이템~~!!! 09.09.01
{
	LOG_UPGRADE_ETERNAL_ITEM	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_ENCHANT_ITEM);
	logPacket.logTime.yy							=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm							=	g_currentTime.m_wMonth;
	logPacket.logTime.dd							=	g_currentTime.m_wDay;
	logPacket.logTime.hour							=	g_currentTime.m_wHour;
	logPacket.logTime.min							=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex						=	m_wCurrentField;
	logPacket.wWork									=	_iWork;
	logPacket.wUpgradeItemSerial					=	_wUpgradeItemSerial;				//  강화 후  아이템
	logPacket.dwGold								=	_dwGold;							// 재료 금..
	logPacket.wStoneOfMesteryCount					=	_wStoneOfMesteryCount;			// 신비석
	logPacket.wTantillessCount						=	_wTantillessCount;				// 탄틸리스
	logPacket.wCrystalCount							=	_wCrystalCount;					// 결정석
	logPacket.wUpgradeMaterialsItemSerial			=	_wUpgradeMaterialsItemSerial;			// 일반강화제 시리얼
	logPacket.wUpgradePremiumMaterialsItemSerial	=	_wUpgradePremiumMaterialsItemSerial;		// 프리미엄아이템 강화제 시리얼
	
	memcpy(&logPacket.cTargetItem,_lpTargetItem,sizeof(cItem));	

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	if (logPacket.cTargetItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_UPGRADE_ETERNAL_ITEM");
		return;
	}

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}
void
cACTOR::sendDivideItemLog(cItem *_lpTargetItem,WORD _wBeforDragCount,WORD _wAfterDragCount,WORD _wFocusCount )		//  스택나누기.
{
	LOG_S_DIVIDE_ITEM	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_SYSTEM);
	logPacket.logTime.yy							=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm							=	g_currentTime.m_wMonth;
	logPacket.logTime.dd							=	g_currentTime.m_wDay;
	logPacket.logTime.hour							=	g_currentTime.m_wHour;
	logPacket.logTime.min							=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex						=	m_wCurrentField;
	logPacket.wWork									=	eSLT_DIVIDE_ITEM;

	logPacket.wBeforDragCount						=	_wBeforDragCount;
	logPacket.wAfterDragCount						=	_wAfterDragCount;
	logPacket.wFocusCount							=	_wFocusCount;
	
	memcpy(&logPacket.cTargetItem,_lpTargetItem,sizeof(cItem));	

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if	(lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
	
}
void
cACTOR::sendPitchmanShopTradeLog(cItem *_lpItem,cACTOR *_lpOwner,DWORD _dwPrice,BOOL _bIsByOperator, BYTE _bDenomination)		// 09.08.21
{
	LOG_PITCHMAN_SHOP_TRADE	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_PITCHMAN_SHOP_TRADE);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;

	memcpy(&logPacket.cItem,_lpItem,sizeof(cItem));	

	strcpy(logPacket.strId,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	logPacket.bf31ItemPrice		=	_dwPrice;
	logPacket.bf1IsOperator		=	_bIsByOperator;
	logPacket.bDenominationType	=	_bDenomination;		// 통화 단위~~~ 09.08.21

	//	판매자 정보
	{
		strcpy(logPacket.strOwnerId,_lpOwner->m_strId);
		strcpy(logPacket.strOwnerName,_lpOwner->m_strName);

		lpClient				=	_lpOwner->getClient();

		if (lpClient)
				memcpy(logPacket.strOwnerIP,lpClient->ip,dIP_LENGTH);
		else	strcpy(logPacket.strOwnerIP,"invalid IP");
	}

	if (logPacket.cItem.m_wBaseItem>=g_im.m_iItemCount)
	{
		CLOG("itemLog","LOG_PITCHMAN_SHOP_TRADE");
		return;
	}
	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}
void
cACTOR::sendPetWorkLog(int _iPetIndex,int _iWork)
{
	cPetDefine	*lpPet	=	getPet(_iPetIndex);

	if	(!lpPet)
		return;

	LOG_PET_WORK	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_PET_WORK);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wFieldUniqueIndex	=	m_wCurrentField;
	logPacket.bf2Index			=	_iPetIndex;
	logPacket.bf6Work			=	_iWork;

	cBreedingRecordBookDefine	*lpBRBook	=	(cBreedingRecordBookDefine*)&logPacket.petInfo;

	lpBRBook->m_dwExperience	=	lpBRBook->buildByPetInfo(lpPet,m_wAvatarIndex,m_iLevel);
	
	strcpy(logPacket.strId	,m_strId);
	strcpy(logPacket.strName,m_strName);

	CClient	*lpClient			=	getClient();

	if (lpClient)
		memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(logPacket.strIP,"invalid IP");

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
GetSaveLogIDAndName(char *_lpstrIDAnmName,char *_lpstrID,char *_lpstrName)
{
	int	iIDLength,iNameLength,iBufferPoint=0;

	iIDLength	=	_lpstrIDAnmName[iBufferPoint];
	iBufferPoint++;
	memcpy(_lpstrID,_lpstrIDAnmName+iBufferPoint,iIDLength);
	iBufferPoint+=	iIDLength;
	_lpstrID[iIDLength]		=	NULL;

	iNameLength	=	_lpstrIDAnmName[iBufferPoint];
	iBufferPoint++;
	memcpy(_lpstrName,_lpstrIDAnmName+iBufferPoint,iNameLength);
	_lpstrName[iNameLength]	=	NULL;
}

void
cACTOR::sendSaveLog()
{
	LOG_AVA_SAVE_LOG	logPacket;

	logPacket.base.set(sizeof(logPacket),dLOG_AVA_SAVE_LOG);

	logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
	logPacket.logTime.mm		=	g_currentTime.m_wMonth;
	logPacket.logTime.dd		=	g_currentTime.m_wDay;
	logPacket.logTime.hour		=	g_currentTime.m_wHour;
	logPacket.logTime.min		=	g_currentTime.m_wMinute;
	logPacket.wField			=	m_wCurrentField;
	logPacket.wLevel			=	m_iLevel;
	logPacket.dwExperience		=	m_iExperience;
	logPacket.dwGold			=	m_iGold;
	logPacket.dwSaveCount		=	m_dwSaveCount;

	int	iIDLength				=	strlen(m_strId);
	int	iNameLength				=	strlen(m_strName);
	int	iBufferPoint			=	0;

	logPacket.strIdAndName[iBufferPoint]	=	iIDLength;
	iBufferPoint++;
	memcpy(logPacket.strIdAndName+iBufferPoint,m_strId,iIDLength);
	iBufferPoint				+=	iIDLength;

	logPacket.strIdAndName[iBufferPoint]	=	iNameLength;
	iBufferPoint++;
	memcpy(logPacket.strIdAndName+iBufferPoint,m_strName,iNameLength);
	iBufferPoint				+=	iNameLength;

	logPacket.base.wSize		=	sizeof(logPacket)-sizeof(logPacket.strIdAndName)+iBufferPoint;

	g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
}

void
cACTOR::sendBackServerLog(DWORD _dwCurrentSaveCount,DWORD _dwOldSaveCount)
{
	LOG_SYSTEM			logSystem;
	LOG_S_BACK_SERVER	*lpLog	=	(LOG_S_BACK_SERVER *)&logSystem;

	lpLog->base.set(sizeof(LOG_S_BACK_SERVER),dLOG_SYSTEM);

	lpLog->logTime.yy			=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm			=	g_currentTime.m_wMonth;
	lpLog->logTime.dd			=	g_currentTime.m_wDay;
	lpLog->logTime.hour			=	g_currentTime.m_wHour;
	lpLog->logTime.min			=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex	=	m_wCurrentField;
	lpLog->wWork				=	eSLT_BACK_SERVER;
	lpLog->dwCurrentSaveCount	=	_dwCurrentSaveCount;
	lpLog->dwOldSaveCount		=	_dwOldSaveCount;

	strcpy(lpLog->strId,m_strId);
	strcpy(lpLog->strName,m_strName);

	CClient	*lpClient		=	getClient();

	if	(lpClient)
		memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(lpLog->strIP,"invalid IP");

	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);	
}

void
cACTOR::sendTresureMapLog(int _iEvent)
{
	LOG_SYSTEM				logSystem;
	LOG_S_TRESURE_MAP_EVENT	*lpLog	=	(LOG_S_TRESURE_MAP_EVENT	*)&logSystem;

	lpLog->base.set(sizeof(LOG_S_TRESURE_MAP_EVENT),dLOG_SYSTEM);

	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	m_wCurrentField;
	lpLog->wWork			=	eSLT_TREASURE_MAP_EVENT;
	lpLog->wEvent			=	(WORD)_iEvent;

	strcpy(lpLog->strId,m_strId);
	strcpy(lpLog->strName,m_strName);

	CClient	*lpClient		=	getClient();

	if	(lpClient)
		memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(lpLog->strIP,"invalid IP");

	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);	
}

void
cACTOR::sendHackingLog(int _iReason,char *_lpstrReason)
{
	LOG_SYSTEM			logSystem;
	LOG_S_HACKING_USER	*lpLog	=	(LOG_S_HACKING_USER	*)&logSystem;
	
	lpLog->base.set(sizeof(LOG_S_HACKING_USER),dLOG_SYSTEM);
	
	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	m_wCurrentField;
	lpLog->wWork			=	eSLT_HACKING;
	lpLog->wPeneltyTime		=	(WORD)m_dwPeneltyTime;
	lpLog->wReason			=	_iReason;

	memset(lpLog->strHackingProgramName,0,sizeof(lpLog->strHackingProgramName));

	strncpy(lpLog->strHackingProgramName,_lpstrReason,sizeof(lpLog->strHackingProgramName)-2);

	strcpy(lpLog->strId,m_strId);
	strcpy(lpLog->strName,m_strName);
	
	CClient	*lpClient		=	getClient();

	if	(lpClient)
		memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(lpLog->strIP,"invalid IP");
	
	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);	
}

void
cACTOR::sendHackingLog(int _iReason,int _iValue)
{
	LOG_SYSTEM			logSystem;
	LOG_S_HACKING_USER	*lpLog	=	(LOG_S_HACKING_USER	*)&logSystem;

	lpLog->base.set(sizeof(LOG_S_HACKING_USER),dLOG_SYSTEM);

	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	m_wCurrentField;
	lpLog->wWork			=	eSLT_HACKING;
	lpLog->wPeneltyTime		=	(WORD)m_dwPeneltyTime;
	lpLog->wReason			=	_iReason;

	memset(lpLog->strHackingProgramName,0,sizeof(lpLog->strHackingProgramName));

	if	(_iReason	==	ePPPTR_BY_XTRAP)
		sprintf(lpLog->strHackingProgramName,"%d",_iValue);

	strcpy(lpLog->strId,m_strId);
	strcpy(lpLog->strName,m_strName);

	CClient	*lpClient		=	getClient();

	if	(lpClient)
		memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(lpLog->strIP,"invalid IP");

	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);	
}

void
cACTOR::sendReceiveQuestRewardExp(int _iRewardExp,int _iRealExp,int _iQuestIndex)
{
	LOG_SYSTEM						logSystem;
	LOG_S_RECEIVE_QUEST_REWARD_EXP	*lpLog	=	(LOG_S_RECEIVE_QUEST_REWARD_EXP	*)&logSystem;

	lpLog->base.set(sizeof(LOG_S_RECEIVE_QUEST_REWARD_EXP),dLOG_SYSTEM);

	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	m_wCurrentField;
	lpLog->wWork			=	eSLT_RECEIVE_QUEST_EXP;
	lpLog->iRewardExp		=	_iRewardExp;
	lpLog->iRealReceiveExp	=	_iRealExp;
	lpLog->iQuest			=	_iQuestIndex;

	strcpy(lpLog->strId,m_strId);
	strcpy(lpLog->strName,m_strName);

	CClient	*lpClient		=	getClient();

	if	(lpClient)
		memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(lpLog->strIP,"invalid IP");

	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);	
}

void
cACTOR::sendWarningQuestExpSerialReceive(int _iRewardExp,int _iQuestIndex)
{
	LOG_ERROR	logError;
	LOG_ERROR_QUEST_EXP_SERIAL_RECEIVE	*lpLog	=	(LOG_ERROR_QUEST_EXP_SERIAL_RECEIVE	*)&logError;

	lpLog->base.set(sizeof(LOG_ERROR_QUEST_EXP_SERIAL_RECEIVE),dLOG_ERROR);

	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	m_wCurrentField;
	lpLog->wWork			=	eSLT_WARNING_QUEST_EXP_SERIAL_RECEIVE;
	lpLog->iQuestExp		=	_iRewardExp;
	lpLog->iQuestIndex		=	_iQuestIndex;

	strcpy(lpLog->strId,m_strId);
	strcpy(lpLog->strName,m_strName);

	CClient	*lpClient		=	getClient();

	if	(lpClient)
		memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(lpLog->strIP,"invalid IP");

	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);	
}

void
cACTOR::sendLogPlayerLogInOut(BOOL isLogOut)
{
	WU_LOG_INOUT			statisticsPacket;
	ZeroMemory(&statisticsPacket , sizeof(WU_LOG_INOUT));
	
	statisticsPacket.base.set(sizeof(WU_LOG_INOUT) , dSERVERStoWORLD_LOG_INOUT);
	
	statisticsPacket.isIn			= isLogOut;
	statisticsPacket.lev			= m_iLevel;
	statisticsPacket.job			= m_wJob;
	strcpy(statisticsPacket.strID , m_strId);
	strcpy(statisticsPacket.strName , m_strName);
	
	SEND_WOLRD_PACKET(&statisticsPacket);
}

//
//로그큐와 데이터 통계 작업에 필요한 정보를 전송한다.
void
cACTOR::sendCompleteQuest(int _iType,int _iQuest)
{
	LOG_SYSTEM				logSystem;
	LOG_S_COMPLETE_QUEST	*lpLog	=	(LOG_S_COMPLETE_QUEST	*)&logSystem;

	lpLog->base.set(sizeof(LOG_S_COMPLETE_QUEST),dLOG_SYSTEM);

	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	m_wCurrentField;
	lpLog->wWork			=	eSLT_COMPLETE_QUEST;
	lpLog->bf2Type			=	_iType;
	lpLog->bf14QuestIndex	=	_iQuest;

	strcpy(lpLog->strId,m_strId);
	strcpy(lpLog->strName,m_strName);

	CClient	*lpClient		=	getClient();

	if (lpClient)
		memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(lpLog->strIP,"invalid IP");

	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);	

	sendLogstatisticsQuest(_iQuest+_iType*10000 , 0);
}

void
cACTOR::sendLogstatisticsQuest(int _iQuest , int _isStartQuest)
{
	WU_LOG_QUEST			statisticsPacket;
	ZeroMemory(&statisticsPacket , sizeof(WU_LOG_QUEST));
	
	statisticsPacket.base.set(sizeof(WU_LOG_QUEST) , dSERVERStoWORLD_LOG_QUEST);
	
	statisticsPacket.isStartQuest	= _isStartQuest;
	statisticsPacket.questIdx		= _iQuest;	
	statisticsPacket.lev			= m_iLevel;
	statisticsPacket.job			= m_wJob;
	strcpy(statisticsPacket.strID , m_strId);
	strcpy(statisticsPacket.strName , m_strName);
	strncpy(statisticsPacket.worldName , g_SS.m_strServerName , 20);
	
	SEND_WOLRD_PACKET(&statisticsPacket);
}

void
cACTOR::sendOperatorChatLog(int _iType,BOOL _bIsListen,char *_lpstrTarget,char *_lpstrText)
{
	LOG_SYSTEM				logSystem;
	LOG_S_OPERATOR_CHAT_LOG	*lpLog	=	(LOG_S_OPERATOR_CHAT_LOG	*)&logSystem;

	lpLog->base.set(sizeof(LOG_S_OPERATOR_CHAT_LOG),dLOG_SYSTEM);
	
	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	m_wCurrentField;
	lpLog->wWork			=	eSLT_OPERATOR_CHAT_LOG;
	lpLog->bf10ChatMethod	=	_iType;
	lpLog->bf1IsListen		=	_bIsListen;

	strcpy(lpLog->strId,m_strId);
	strcpy(lpLog->strName,m_strName);

	strncpy(lpLog->strText,_lpstrText,dCHAT_LENGTH-2);

	if	(_lpstrTarget)
		strncpy(lpLog->strTarget,_lpstrTarget,dNAME_LENGTH);

	lpLog->base.wSize		=	sizeof(LOG_S_OPERATOR_CHAT_LOG)-sizeof(lpLog->strText)+strlen(lpLog->strText)+1;

	CClient	*lpClient		=	getClient();

	if (lpClient)
		memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(lpLog->strIP,"invalid IP");

	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);	
}

void
cACTOR::sendRemovePlaceRelatedOfGuild(char *_lpstrGuildName,int _iField)
{
	LOG_SYSTEM				logSystem;
	LOG_S_REMOVE_A_PLACE_RELATED_OF_GUILD	*lpLog	=	(LOG_S_REMOVE_A_PLACE_RELATED_OF_GUILD	*)&logSystem;

	lpLog->base.set(sizeof(LOG_S_REMOVE_A_PLACE_RELATED_OF_GUILD),dLOG_SYSTEM);
	
	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	m_wCurrentField;
	lpLog->wWork			=	eSLT_REMOVE_A_PLACE_RELATED_OF_GUILD;
	lpLog->wField			=	_iField;
	strcpy(lpLog->strGuildName,_lpstrGuildName);

	CClient	*lpClient		=	getClient();

	if	(lpClient)
		memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(lpLog->strIP,"invalid IP");

	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);	
}

void
cACTOR::sendDisconnectDuelFieldLog()
{
	LOG_SYSTEM						logSystem;
	LOG_S_DISCONNECT_IN_DUEL_FIELD	*lpLog	=	(LOG_S_DISCONNECT_IN_DUEL_FIELD	*)&logSystem;
	
	lpLog->base.set(sizeof(LOG_S_DISCONNECT_IN_DUEL_FIELD),dLOG_SYSTEM);
	
	lpLog->logTime.yy		=	g_currentTime.m_wYear%100;
	lpLog->logTime.mm		=	g_currentTime.m_wMonth;
	lpLog->logTime.dd		=	g_currentTime.m_wDay;
	lpLog->logTime.hour		=	g_currentTime.m_wHour;
	lpLog->logTime.min		=	g_currentTime.m_wMinute;
	lpLog->wFieldUniqueIndex=	m_wCurrentField;
	lpLog->wWork			=	eSLT_DISCONNECT_IN_DUEL_FIELD;
	lpLog->iDuelSerial		=	0;

	int iTeam;

	if	(m_lpField)
	{
		lpLog->iDuelSerial		=	m_lpField->m_iDuelSerial;

		for (iTeam=0;iTeam<2;iTeam++)
			strcpy(lpLog->astrTeamName[iTeam],m_lpField->m_aDuelTeam[iTeam].m_strTeamName);
	}
	else
	{
		lpLog->iDuelSerial		=	0xffffffff;
		strcpy(lpLog->astrTeamName[0]," ");
		strcpy(lpLog->astrTeamName[1]," ");
	}

	strcpy(lpLog->strId,m_strId);
	strcpy(lpLog->strName,m_strName);
			
	CClient			*lpClient	=	getClient();
	
	if	(lpClient)
		memcpy(lpLog->strIP,lpClient->ip,dIP_LENGTH);
	else
		strcpy(lpLog->strIP,"invalid IP");
	
	g_LogQue.addPacket((ALL_LOGMSG *)lpLog);	
}

BOOL
cACTOR::isRequireSendSaveLog()
{
	if (m_iLastLogExp	<	m_iExperience	&&	m_iLevel	>=	10)
	{
		if (m_iExperience	<	m_iLastLogExp+g_aExpTable[m_iLevel]/m_iLevel)
			return FALSE;
	}

	m_iLastLogExp	=	m_iExperience;

	return	TRUE;
}

void
cACTOR::updateGoldLog(LONGLONG _llGettingGold,int _iMethod)
{
	if	(_llGettingGold > 0)
	{
		if	(_iMethod	==	eGET_GOLD_IGNORE)
			return;
			
		g_allGettingGold[_iMethod]	+=	_llGettingGold;
		m_llGettingGold				+=	_llGettingGold;
	}
	else
	if	(_llGettingGold < 0)
	{
		if	(_iMethod	==	eSPENT_GOLD_IGNORE)
			return;

		g_allSpentGold[_iMethod]	+=	_llGettingGold;
		m_llSpentGold				+=	_llGettingGold;
	}

	if	(isTester())// && m_bf1IsSendGoldLog)
	{
		LONGLONG	llOwnGold		=	getOwnGoldSum();
		int iFrontGoldAll			= llOwnGold>>32;
		int iBackGoldAll			= llOwnGold&0xffffffff;
		int iFrontGettingGold		= _llGettingGold>>32;
		int iBackGettingGold		= _llGettingGold&0xffffffff;

		sendEtcWork(eEW_SEND_GOLD_LOG,iFrontGoldAll,iBackGoldAll,iFrontGettingGold,iBackGettingGold);
	}
}


void
cACTOR::sendGoldLog(LONGLONG _llOwnGold)
{
	{
		LONGLONG	llDeltaGold	=	_llOwnGold-m_llLastGold;
		LONGLONG	llDeltaGold2=	m_llGettingGold+m_llSpentGold+m_llTradeGettingGold+m_llTradeSpentGold;
		LONGLONG	llLastGold	=	m_llLastGold+llDeltaGold2;

// 		printf("sendGoldLog %s/%s %I64d==%I64d %I64d==%I64d\n",m_strId,m_strName,_llOwnGold,llLastGold,llDeltaGold,llDeltaGold2);

		CLOG("goldLog","	%s	%s	%I64d	%I64d	%I64d	%I64d	%I64d	%I64d	%I64d",
			m_strId,m_strName,
			_llOwnGold,llLastGold,
			llDeltaGold,
			m_llGettingGold,m_llSpentGold,m_llTradeGettingGold,m_llTradeSpentGold);
	}

	m_llLastGold	=	_llOwnGold;

	GDMSG_STAT_GOLD	packet;

	packet.base.set(sizeof(packet),dGDMSG_STAT_GOLD);

	strcpy(packet.strServer,g_strGameServerName);
	strcpy(packet.strId,m_strId);
	strcpy(packet.strName,m_strName);

	packet.llGoldAll		=	m_llLastGold;

	if	(m_llGettingGold)
	{
		packet.llGettingGold	=	m_llGettingGold;

		g_socketDBC.SendPacket((char *)&packet,packet.base.wSize);
	}

	if	(m_llSpentGold)
	{
		packet.llGettingGold	=	m_llSpentGold;

		g_socketDBC.SendPacket((char *)&packet,packet.base.wSize);
	}

	m_llGettingGold		=	0;
	m_llSpentGold		=	0;
	m_llTradeGettingGold=	0;
	m_llTradeSpentGold	=	0;
}

void
cACTOR::sendBankGoldLog(LONGLONG _llOwnGold)
{
	GDMSG_STAT_GOLD	packet;

	packet.base.set(sizeof(packet),dGDMSG_STAT_GOLD);

	strcpy(packet.strServer,g_strGameServerName);
	strcpy(packet.strId,m_strId);
	strcpy(packet.strName,"BNK");
	packet.llGoldAll		=	_llOwnGold;
	packet.llGettingGold	=	0;

	g_socketDBC.SendPacket((char *)&packet,packet.base.wSize);
}