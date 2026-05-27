#include	"stdafx.h"
#include	"log_filtering.h"
#include	"Quest.h"
#include	"duel_filter.h"

cITEM		g_aItemListForLog[512];
char		g_astrIDForLog[512][dID_LENGTH];
char		g_astrNameForLog[512][dNAME_LENGTH];
char		g_astrIPForLog[512][dIP_LENGTH];
char		g_astrGuildNameLog[512][dNAME_LENGTH];

BOOL		g_abIsAboutItemLog[256];
BOOL		g_bIsSearchErrorLog	=	FALSE;

BOOL		g_bIsOnlySetItem	=	FALSE;
BOOL		g_bIsOnlyUnique		=	FALSE;
BOOL		g_bIsOnlyDX			=	FALSE;
BOOL		g_bIsOnlyDX_Unique	=	FALSE;
BOOL		g_bIsOnlyEternal	=	FALSE;		//	아이템 종류 필터
BOOL		g_bIsCheckIncorrectItem=	FALSE;	//	잘못된 아이템만 체크
int			g_iEternalGrade		=	0;
int			g_iJobFilter		=	-1;

BOOL
IsSkipDataByCheckTimeAndSerchType(BOOL _bIsIgnoreSaveLog,BOOL _bIsAddSaveLog,ALL_LOGMSG *_lpLog,tsLogTime *_lpTime,int yy1,int yy2,int mm1,int mm2,int dd1,int dd2,int hour1,int hour2,int min1,int min2,int _iType,int _iCompareIndex)
{
	if	(yy1 >= 2000)
		yy1	-=	2000;
	if	(yy2 >= 2000)
		yy2	-=	2000;

	int		iLogType			=	_lpLog->base.wType;
	DWORD	dwCheckBeginTime	=	yy1*12*31*24*60+mm1*31*24*60+dd1*24*60+hour1*60+min1;
	DWORD	dwCheckEndTime		=	yy2*12*31*24*60+mm2*31*24*60+dd2*24*60+hour2*60+min2;
	DWORD	dwLogTime			=	_lpTime->yy*12*31*24*60+_lpTime->mm*31*24*60+_lpTime->dd*24*60+_lpTime->hour*60+_lpTime->min;

	if	(dwLogTime < dwCheckBeginTime)
		return	TRUE;

	if	(dwLogTime > dwCheckEndTime)
		return	TRUE;

	if	(_bIsAddSaveLog	&&	iLogType	==	dLOG_AVA_SAVE_LOG)
		return	FALSE;

	if	(_iCompareIndex	!=	eAVA_ALL	&&	_iCompareIndex	!=	eAVA_SAVE	&&	_bIsIgnoreSaveLog	&&	iLogType	==	dLOG_AVA_SAVE_LOG)
		return	TRUE;

	switch(_iCompareIndex)
	{
		case	eABOUT_ERROR	:
			if	(iLogType	!=	dLOG_ERROR)
				return	FALSE;
			return	TRUE;

		case	eABOUT_OPERATOR	:
			if	(iLogType==	dLOG_ITEM_BANK_IN	||	iLogType==	dLOG_ITEM_BANK_OUT	||
 				 iLogType==	dLOG_SYSTEM			||	iLogType==	dLOG_ITEM_DEL		||	iLogType	==	dLOG_ITEM_TRADE	||	iLogType	==	dLOG_PITCHMAN_SHOP_TRADE)
				return	FALSE;
			return	TRUE;

		case	eALL	:
			return	FALSE;

		case	eITEM_ALL:
		case	eITEM_FIND_INCORECT_ITEM:
			return	1-g_abIsAboutItemLog[_lpLog->base.wType];

		case	eITEM_DROP:	//	아이템 드랍
			if	(iLogType	==	dLOG_ITEM_DROP)
				return	FALSE;

			return	TRUE;

		case	eITEM_PICK:	//	아이템 줏은거
			if	(iLogType	==	dLOG_ITEM_PICK)
				return	FALSE;

			return	TRUE;

		case	eITEM_BUY_SELL:	//	아이템 판매한거
			if	(iLogType	==	dLOG_SYSTEM	||	iLogType	==	dLOG_ITEM_SELL)
				return	FALSE;

			return	TRUE;

		case	eITEM_DEL	:	//	아이템 부순거
			if	(iLogType	==	dLOG_ITEM_DEL)
				return	FALSE;

			return	TRUE;

		case	eITEM_TRADE:
			if	(iLogType	==	dLOG_ITEM_TRADE)
				return	FALSE;

			return	TRUE;

		case	eITEM_BANK:
			if	(iLogType	==	dLOG_ITEM_BANK_IN	||	iLogType	==	dLOG_ITEM_BANK_OUT	)
				return	FALSE;

			return	TRUE;

		case	eITEM_PREMIUM_ITEM:
			if	(iLogType	==	dLOG_ITEM_WITHDRAW_PREMIUM_ITEM	||	iLogType	==	dLOG_ITEM_CREATE_PREMIUM_ITEM	)
				return	FALSE;
			return	TRUE;

		case	eITEM_CREATE_ITEM:
			if	(iLogType	==	dLOG_CREATE_ITEM	)
				return	FALSE;
			return	TRUE;

		case	eITEM_USE_ITEM:
			if	(iLogType	==	dLOG_USE_ITEM	)
				return	FALSE;
			return	TRUE;

		case	eITEM_EXPI:
			if	(iLogType	==	dLOG_EXPIRED_ITEM	)
				return	FALSE;

			return	TRUE;
			
		case	eITEM_COPY:
			if	(iLogType	==	dLOG_DOUBLE_ITEM	)
				return	FALSE;

			return	TRUE;
			
		case	eITEM_ENCH:
			if	(iLogType	==	dLOG_ENCHANT_ITEM	)
				return	FALSE;
			return	TRUE;
			
		case	eITEM_PITCHMAN_SHOP:
			if	(iLogType	==	dLOG_PITCHMAN_SHOP_TRADE)
				return	FALSE;
			return	TRUE;
			

		case	eITEM_RENEW_TERM:
			if	(iLogType	==	dLOG_ENCHANT_ITEM	)
				return	FALSE;
			return	TRUE;
			

		case	eAVA_ALL:
			if	(iLogType	==	dLOG_AVA_CREATE	||	iLogType	==	dLOG_AVA_DELETE		||	iLogType	==	dLOG_AVA_LOGIN	||
				iLogType	==	dLOG_AVA_LOGOUT	||	iLogType	==	dLOG_AVA_SAVE_LOG	)
				return	FALSE;
			return	TRUE;

		case	eAVA_CREATE:
			if	(iLogType	==	dLOG_AVA_CREATE	)
				return	FALSE;
			return	TRUE;
			
		case	eAVA_DELETE:
			if	(iLogType	==	dLOG_AVA_DELETE	)
				return	FALSE;
			return	TRUE;
			
		case	eAVA_LOGIN:
			if	(iLogType	==	dLOG_AVA_LOGIN	)
				return	FALSE;
			return	TRUE;
			
		case	eAVA_LOGOUT:
			if	(iLogType	==	dLOG_AVA_LOGOUT	)
				return	FALSE;
			return	TRUE;
			
		case	eAVA_SAVE:
			if	(iLogType	==	dLOG_AVA_SAVE_LOG	)
				return	FALSE;
			return	TRUE;

		case	eABOUT_EVENT:
			if	(iLogType	==	dLOG_EVENT	||	iLogType	==	dLOG_SYSTEM)
				return	FALSE;
			return	TRUE;

		case	eABOUT_GUILD_WORK:
			if	(iLogType	==	dLOG_ITEM_DEL			||	iLogType	==	dLOG_CREATE_ITEM||	iLogType	==	dLOG_SYSTEM||	
				iLogType	==	dLOG_GUILD_BATTLE_WORK	||	iLogType	==	dLOG_GUILD_WORK	||	iLogType	==	dLOG_ITEM_PICK)
				return	FALSE;
			return	TRUE;
		
		case	eABOUT_PET:
			if	(iLogType	==	dLOG_PET_WORK	||	iLogType	==	dLOG_CREATE_ITEM	||	iLogType	==	dLOG_ITEM_DEL)
				return	FALSE;
			return	TRUE;

		case	eABOUT_MINIPET:
			if	(iLogType	==	dLOG_SYSTEM	||	iLogType	==	dLOG_ITEM_PICK	||	iLogType	==	dLOG_ITEM_DEL)
				return	FALSE;
			return	TRUE;

		case	eABOUT_BOTTOMLESS_BOX:
			if	(iLogType	==	dLOG_CREATE_ITEM	||	iLogType	==	dLOG_ITEM_DEL	)
				return	FALSE;
			return	TRUE;

		case	eABOUT_CARROT_SHOP:
			if	(iLogType	==	dLOG_ITEM_CREATE_PREMIUM_ITEM	)
				return	FALSE;
			return	TRUE;

		case	eABOUT_QUEST:
		case	eABOUT_BACK_SERVER:
		case	eABOUT_HACKING	:
		case	eABOUT_DUEL:
		case	eABOUT_REBIRTH	:
			if	(iLogType	==	dLOG_SYSTEM	)
				return	FALSE;
			return	TRUE;

	}

	return	TRUE;
}

BOOL
GetSaveLogIDAndName(char *_lpstrIDAnmName,char *_lpstrID,char *_lpstrName)
{
	int	iIDLength,iNameLength,iBufferPoint=0;

	iIDLength	=	_lpstrIDAnmName[iBufferPoint];

	if (iIDLength	>=	dID_LENGTH || iIDLength	< 4)
		return	FALSE;

	iBufferPoint++;

	if	(_lpstrID)
		memcpy(_lpstrID,_lpstrIDAnmName+iBufferPoint,iIDLength);
	iBufferPoint+=	iIDLength;

	if	(_lpstrID)
		_lpstrID[iIDLength]		=	NULL;

	iNameLength	=	_lpstrIDAnmName[iBufferPoint];

	if	(iNameLength	==	13	&&	_lpstrIDAnmName[iBufferPoint+1]	==	10)
	{
		iBufferPoint++;
		iNameLength	=	_lpstrIDAnmName[iBufferPoint];
	}

	if	(iNameLength	>=	dNAME_LENGTH || iNameLength	<	4)
		return	FALSE;

	if	(_lpstrName)
	{
		iBufferPoint++;
		memcpy(_lpstrName,_lpstrIDAnmName+iBufferPoint,iNameLength);
		_lpstrName[iNameLength]	=	NULL;
	}

	return	TRUE;
}

BOOL
GetSaveLogIDAndName2(char *_lpstrIDAnmName,char *_lpstrID,char *_lpstrName)
{
	int	iIDLength,iNameLength,iBufferPoint=0;

	iIDLength	=	_lpstrIDAnmName[iBufferPoint];

	if	(iIDLength	>=	dID_LENGTH-1 || iIDLength<=0)
	{
		strcpy(_lpstrID,"break ID log!!");
		strcpy(_lpstrName,"break Name log!!");
		return FALSE;
	}

	iBufferPoint++;
	memcpy(_lpstrID,_lpstrIDAnmName+iBufferPoint,iIDLength);
	iBufferPoint+=	iIDLength;
	_lpstrID[iIDLength]		=	NULL;

	iNameLength	=	_lpstrIDAnmName[iBufferPoint];

	if	(iNameLength	>=	dNAME_LENGTH-1 || iNameLength	<= 0)
	{
		strcpy(_lpstrName,"break Name log!!");
		return FALSE;
	}

	iBufferPoint++;
	memcpy(_lpstrName,_lpstrIDAnmName+iBufferPoint,iNameLength);
	_lpstrName[iNameLength]	=	NULL;
	
	return TRUE;
}

int
ExportIDWithLog(ALL_LOGMSG *_lpAllLog)
{
	int	iCount	=	1;
	
	switch(_lpAllLog->base.wType)
	{
		case	dLOG_PET_WORK	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_PET_WORK.strId);
			break;

		case	dLOG_ITEM_DROP	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_ITEM_DROP.strId);
			break;
		case	dLOG_ITEM_PICK	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_ITEM_PICK.strId);
			break;

		case	dLOG_ITEM_SELL	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_ITEM_SELL.strId);
			break;


		case	dLOG_USE_ITEM	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_USE_ITEM.strId);
			break;
		case	dLOG_CREATE_ITEM	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_CREATE_ITEM.strId);
			break;
		case	dLOG_EXPIRED_ITEM	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_EXPIRED_ITEM.strId);
			break;
		case	dLOG_DOUBLE_ITEM	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_DOUBLE_ITEM.strId);
			break;
		case	dLOG_ENCHANT_ITEM	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_ENCHANT_ITEM.strId);
			break;
		case	dLOG_PITCHMAN_SHOP_TRADE	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_PITCHMAN_SHOP_TRADE.strId);
			strcpy(g_astrIDForLog[1],_lpAllLog->mLOG_PITCHMAN_SHOP_TRADE.strOwnerId);
			iCount	=	2;
			break;
		case	dLOG_ITEM_DEL	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_ITEM_DEL.strId);
			break;
		case	dLOG_ITEM_TRADE	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_ITEM_TRADE.strSenderId);
			strcpy(g_astrIDForLog[1],_lpAllLog->mLOG_ITEM_TRADE.strRecverId);
			iCount	=	2;
			break;
		case	dLOG_ITEM_BANK_IN	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_ITEM_BANK_IN.strId);
			break;
		case	dLOG_ITEM_BANK_OUT	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_ITEM_BANK_OUT.strId);
			break;
		case	dLOG_ITEM_WITHDRAW_PREMIUM_ITEM	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_ITEM_WITHDRAW_PREMIUM_ITEM.strId);
			break;
		case	dLOG_ITEM_CREATE_PREMIUM_ITEM	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_ITEM_CREATE_PREMIUM_ITEM.strId);
			break;
		case	dLOG_AVA_CREATE	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_AVA_CREATE.strId);
			break;
		case	dLOG_AVA_DELETE	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_AVA_DELETE.strId);
			break;
		case	dLOG_AVA_LOGIN	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_AVA_LOGIN.strId);
			break;
		case	dLOG_AVA_LOGOUT	:
			strcpy(g_astrIDForLog[0],_lpAllLog->mLOG_AVA_LOGOUT.strId);
			break;
		case	dLOG_AVA_SAVE_LOG	:
			GetSaveLogIDAndName(_lpAllLog->mLOG_AVA_SAVE_LOG.strIdAndName,g_astrIDForLog[0],NULL);
			break;

		case	dLOG_SYSTEM		:
		{
			LOG_SYSTEM	*lpLog	=	(LOG_SYSTEM	*)_lpAllLog;

			switch(lpLog->wWork)
			{
				case	eSLT_HACKING	:
				case	eSLT_COMPLETE_QUEST	:
				case	eSLT_RECEIVE_QUEST_EXP	:
				case	eSLT_TREASURE_MAP_EVENT	:
				case	eSLT_BACK_SERVER	:
				case	eSLT_CREATE_MINI_PET	:
				case	eSLT_EXTEND_MINI_PET_TIME:
				case	eSLT_SEAL_MINI_PET	:
				case	eSLT_UNSEAL_MINI_PET	:
				case	eSLT_TRANS_AVATAR_TO_GVG_SERVER	:
				case	eSLT_MINI_PET_FEED_ITEM	:
				case	eSLT_REMOVE_MINI_PET	:
				case	eSLT_RESTRAINT_MINIPET	:
				case	eSLT_OPERATOR_CHAT_LOG	:	//	이름 2개
				case	eSLT_REMOVE_A_PLACE_RELATED_OF_GUILD	:
				case	eSLT_BUY_ITEM	:
				case	eSLT_RECORD_GUILD_HONOR_POINT	:
				case	eSLT_BEGIN_DUEL	:
				case	eSLT_RESULT_OF_DUEL	:
					
				case	eSLT_DISCONNECT_IN_DUEL_FIELD	:
				case	eSLT_REBIRTH	:
					strcpy(g_astrIDForLog[0],lpLog->strId);
					break;
				default	:
					iCount	=	0;
					break;
			}

			break;
		}

		case	dLOG_GUILD_WORK	:
		{
			LOG_GUILD_WORK	*lpLog	=	(LOG_GUILD_WORK	*)_lpAllLog;

			switch(lpLog->wWork)
			{
				case	eGW_TRANS_GUILD_TO_GVG_SERVER	:
				case	eGW_GUILD_DUNGEON_PLAY			:
				case	eGW_ACTIVE_GUILD_MASTER_GRACE	:
				case	eGW_ACTIVE_GUILD_ROEN_GOOD_WILL	:
				case	eGW_DELETE_GUILD	:
				case	eGW_CHANGE_GUILD_RANK	:
					strcpy(g_astrIDForLog[0],lpLog->strId);
					break;
				default	:
					iCount	=	0;
					break;
			}

			break;
		}

		default	:
			iCount	=	0;
			break;
	}

	return	iCount;
}

int
ExportNameWithLog(ALL_LOGMSG *_lpAllLog)
{
	int	iCount	=	1;
	
	switch(_lpAllLog->base.wType)
	{
		case	dLOG_PET_WORK	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_PET_WORK.strName);
			break;

		case	dLOG_ITEM_DROP	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_ITEM_DROP.strName);
			break;
		case	dLOG_ITEM_PICK	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_ITEM_PICK.strName);
			break;

		case	dLOG_ITEM_SELL	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_ITEM_SELL.strName);
			break;

		case	dLOG_USE_ITEM	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_USE_ITEM.strName);
			break;
		case	dLOG_CREATE_ITEM	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_CREATE_ITEM.strName);
			break;
		case	dLOG_EXPIRED_ITEM	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_EXPIRED_ITEM.strName);
			break;
		case	dLOG_DOUBLE_ITEM	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_DOUBLE_ITEM.strName);
			break;
		case	dLOG_ENCHANT_ITEM	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_ENCHANT_ITEM.strName);
			break;
		case	dLOG_PITCHMAN_SHOP_TRADE	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_PITCHMAN_SHOP_TRADE.strName);
			strcpy(g_astrNameForLog[1],_lpAllLog->mLOG_PITCHMAN_SHOP_TRADE.strOwnerName);
			iCount	=	2;
			break;
		case	dLOG_ITEM_DEL	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_ITEM_DEL.strName);
			break;
		case	dLOG_ITEM_TRADE	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_ITEM_TRADE.strSenderName);
			strcpy(g_astrNameForLog[1],_lpAllLog->mLOG_ITEM_TRADE.strRecverName);
			iCount	=	2;
			break;
		case	dLOG_ITEM_BANK_IN	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_ITEM_BANK_IN.strName);
			break;
		case	dLOG_ITEM_BANK_OUT	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_ITEM_BANK_OUT.strName);
			break;
		case	dLOG_ITEM_WITHDRAW_PREMIUM_ITEM	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_ITEM_WITHDRAW_PREMIUM_ITEM.strName);
			break;
		case	dLOG_ITEM_CREATE_PREMIUM_ITEM	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_ITEM_CREATE_PREMIUM_ITEM.strName);
			break;
		case	dLOG_AVA_CREATE	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_AVA_CREATE.strName);
			break;
		case	dLOG_AVA_DELETE	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_AVA_DELETE.strName);
			break;
		case	dLOG_AVA_LOGIN	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_AVA_LOGIN.strName);
			break;
		case	dLOG_AVA_LOGOUT	:
			strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_AVA_LOGOUT.strName);
			break;
		case	dLOG_AVA_SAVE_LOG	:
			GetSaveLogIDAndName(_lpAllLog->mLOG_AVA_SAVE_LOG.strIdAndName,NULL,g_astrNameForLog[0]);
			break;

		case	dLOG_EVENT		:
		{
			LOG_EVENT	*lpLog	=	(LOG_EVENT	*)_lpAllLog;

			for (int i=0;i<lpLog->wCount;i++)
				strcpy(g_astrNameForLog[i],lpLog->astrName[i]);

			iCount	=	lpLog->wCount;
			break;
		}


		case	dLOG_SYSTEM		:
		{
			LOG_SYSTEM	*lpLog	=	(LOG_SYSTEM	*)_lpAllLog;

			switch(lpLog->wWork)
			{
				case	eSLT_HACKING	:
				case	eSLT_COMPLETE_QUEST	:
				case	eSLT_RECEIVE_QUEST_EXP	:
				case	eSLT_TREASURE_MAP_EVENT	:
				case	eSLT_BACK_SERVER	:
				case	eSLT_CREATE_MINI_PET	:
				case	eSLT_EXTEND_MINI_PET_TIME:
				case	eSLT_SEAL_MINI_PET	:
				case	eSLT_UNSEAL_MINI_PET	:
				case	eSLT_MINI_PET_FEED_ITEM	:
				case	eSLT_REMOVE_MINI_PET	:
				case	eSLT_RESTRAINT_MINIPET	:
				case	eSLT_REMOVE_A_PLACE_RELATED_OF_GUILD	:
				case	eSLT_BUY_ITEM	:
				case	eSLT_RECORD_GUILD_HONOR_POINT	:
				case	eSLT_BEGIN_DUEL	:
				case	eSLT_RESULT_OF_DUEL	:
					
				case	eSLT_DISCONNECT_IN_DUEL_FIELD	:
				case	eSLT_REBIRTH	:
					strcpy(g_astrNameForLog[0],lpLog->strName);
					break;

				case	eSLT_TRANS_AVATAR_TO_GVG_SERVER	:
				{
					LOG_S_TRNAS_AVATAR	*lpData	=	(LOG_S_TRNAS_AVATAR	*)_lpAllLog;
					strcpy(g_astrNameForLog[0],lpLog->strName);
					strcpy(g_astrNameForLog[1],lpData->strNewAvatarName);
					iCount	=	2;
					break;
				}

				case	eSLT_OPERATOR_CHAT_LOG	:	//	이름 2개
					{
						LOG_S_OPERATOR_CHAT_LOG	*lpChatLog	=	(LOG_S_OPERATOR_CHAT_LOG	*)lpLog;
						
						strcpy(g_astrNameForLog[0],lpChatLog->strName);
						strcpy(g_astrNameForLog[1],lpChatLog->strTarget);
						iCount	=	2;
						break;
					}

				default	:
					iCount	=	0;
					break;
			}
			
			break;
		}

		case	dLOG_GUILD_BATTLE_WORK	:
		{
			LOG_GUILD_BATTLE_WORK	*lpLog	=	(LOG_GUILD_BATTLE_WORK	*)_lpAllLog;

			switch(lpLog->wWork)
			{
				case	eGBW_NEW_KILL	:
				{
					LOG_GUILD_BATTLE_WORK_KILL3	*lpGBLog	=	(LOG_GUILD_BATTLE_WORK_KILL3	*)lpLog;
					strcpy(g_astrNameForLog[0],lpGBLog->strKiller);
					strcpy(g_astrNameForLog[1],lpGBLog->strTarget);
					iCount	=	2;
					break;
				}

				case	eGBW_PRE_RESULT		:
				{
					LOG_GUILD_BATTLE_WORK_RESULT	*lpGBLog	=	(LOG_GUILD_BATTLE_WORK_RESULT	*)lpLog;
					strcpy(g_astrNameForLog[0],lpGBLog->strGuild1MemberName);
					strcpy(g_astrNameForLog[1],lpGBLog->strGuild2MemberName);
					iCount	=	2;
					break;
				}

				case	eGBW_LEAVE			:
				case	eGBW_JOIN			:
				{
					strcpy(g_astrNameForLog[0],lpLog->strGuild2Name);
					break;
				}

				default	:
					iCount	=	0;
					break;
			}
		}
		break;

		case	dLOG_GUILD_WORK	:
		{
			LOG_GUILD_WORK	*lpLog	=	(LOG_GUILD_WORK	*)_lpAllLog;

			switch(lpLog->wWork)
			{
				case	eGW_TRANS_GUILD_TO_GVG_SERVER	:
				case	eGW_GUILD_DUNGEON_PLAY			:
				case	eGW_ACTIVE_GUILD_MASTER_GRACE	:
				case	eGW_ACTIVE_GUILD_ROEN_GOOD_WILL	:
				case	eGW_DELETE_GUILD				:
				case	eGW_CHANGE_GUILD_RANK	:
					strcpy(g_astrNameForLog[0],lpLog->strName);
					break;

				default	:
					iCount	=	0;
					break;
			}

			break;
		}

		default	:
			iCount	=	0;
			break;
	}

	return	iCount;
}

int
ExportGuildNameWithLog(ALL_LOGMSG *_lpAllLog)
{
	int	iCount	=	1;
	
	switch(_lpAllLog->base.wType)
	{
		case	dLOG_ITEM_PICK	:
			if	(_lpAllLog->mLOG_ITEM_PICK.bf1IsIncludeGuildInfo)
				strcpy(g_astrGuildNameLog[0],_lpAllLog->mLOG_ITEM_PICK.strGuildName);
			break;

		case	dLOG_ITEM_DEL	:
			if	(_lpAllLog->mLOG_ITEM_DEL.bf1IsIncludeGuildInfo)
				strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_ITEM_DEL.strGuildName);
			break;

		case	dLOG_CREATE_ITEM	:
			if	(_lpAllLog->mLOG_CREATE_ITEM.wMethod	==	eCIM_WITHDRAW_FROM_GUILD_INVENTORY2)
				strcpy(g_astrNameForLog[0],_lpAllLog->mLOG_CREATE_ITEM.strGuildName);
			break;

		case	dLOG_SYSTEM		:
		{
			LOG_SYSTEM	*lpLog	=	(LOG_SYSTEM	*)_lpAllLog;

			switch(lpLog->wWork)
			{
				case	eSLT_REMOVE_A_PLACE_RELATED_OF_GUILD	:
				{
					LOG_S_REMOVE_A_PLACE_RELATED_OF_GUILD	*lpGuildLog	=	(LOG_S_REMOVE_A_PLACE_RELATED_OF_GUILD	*)lpLog;

					strcpy(g_astrGuildNameLog[0],lpGuildLog->strGuildName);
					
					break;
				}
				case	eSLT_RECORD_GUILD_HONOR_POINT			:
				{
					LOG_S_RECORD_GUILD_HONOR_POINT	*lpGuildLog	=	(LOG_S_RECORD_GUILD_HONOR_POINT	*)lpLog;

					strcpy(g_astrGuildNameLog[0],lpGuildLog->strGuildName);
					
					break;
				}

				default	:
					iCount	=	0;
					break;

			}
			break;
		}

		case	dLOG_GUILD_BATTLE_WORK	:
		{
			LOG_GUILD_BATTLE_WORK	*lpLog	=	(LOG_GUILD_BATTLE_WORK	*)_lpAllLog;

			switch(lpLog->wWork)
			{
				case	eGBW_BOOKING:	//	예약
				case	eGBW_CANCEL:	//	취소
				case	eGBW_JOIN:		//	길드 가입
				case	eGBW_LEAVE:		//	길드 탈퇴
					strcpy(g_astrGuildNameLog[0],lpLog->strGuild1Name);
					break;

				case	eGBW_ENGAGE:	//	결정됨
				case	eGBW_RESULT:	//	길드전 결과
				case	eGBW_BEGIN:		//	길드전 시작
				case	eGBW_EXTERMINATE:	//	전멸 시켰다.
					iCount	=	2;
					strcpy(g_astrGuildNameLog[0],lpLog->strGuild1Name);
					strcpy(g_astrGuildNameLog[1],lpLog->strGuild2Name);
					break;
	

				case	eGBW_NEW_KILL	:
				{
					LOG_GUILD_BATTLE_WORK_KILL3	*lpGBLog	=	(LOG_GUILD_BATTLE_WORK_KILL3	*)lpLog;
					strcpy(g_astrGuildNameLog[0],lpGBLog->strGuildName[0]);
					strcpy(g_astrGuildNameLog[1],lpGBLog->strGuildName[1]);
					iCount	=	2;
					break;
				}

				case	eGBW_PRE_RESULT		:
				{
					LOG_GUILD_BATTLE_WORK_RESULT	*lpGBLog	=	(LOG_GUILD_BATTLE_WORK_RESULT	*)lpLog;
					strcpy(g_astrGuildNameLog[0],lpGBLog->strGuild1Name);
					strcpy(g_astrGuildNameLog[1],lpGBLog->strGuild2Name);
					iCount	=	2;
					break;
				}

				default	:
					iCount	=	0;
					break;

			}

			break;
		}

		case	dLOG_GUILD_WORK	:
		{
			LOG_GUILD_WORK	*lpLog	=	(LOG_GUILD_WORK	*)_lpAllLog;

			switch(lpLog->wWork)
			{
				case	eGW_GUILD_DUNGEON_PLAY			:
				case	eGW_ACTIVE_GUILD_MASTER_GRACE	:
				case	eGW_ACTIVE_GUILD_ROEN_GOOD_WILL	:
				case	eGW_DELETE_GUILD				:
				case	eGW_CHANGE_GUILD_RANK	:
					strcpy(g_astrGuildNameLog[0],lpLog->strGuildName);
					break;

				case	eGW_TRANS_GUILD_TO_GVG_SERVER	:
				{
					LOG_S_TRANS_GUILD *lpTLog	= (LOG_S_TRANS_GUILD*)_lpAllLog;

					strcpy(g_astrGuildNameLog[0],lpTLog->strGuildName);
					strcpy(g_astrGuildNameLog[1],lpTLog->strTransGuildName);

					iCount	=	2;
					break;
				}

				default	:
					iCount	=	0;
					break;
			}

			break;
		}

		default	:
			iCount	=	0;
			break;
	}

	return	iCount;
}


int
ExportIPWithLog(ALL_LOGMSG *_lpAllLog)
{
	int	iCount	=	1;
	
	switch(_lpAllLog->base.wType)
	{
		case	dLOG_PET_WORK	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_PET_WORK.strIP);
			break;

		case	dLOG_ITEM_DROP	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_ITEM_DROP.strIP);
			break;
		case	dLOG_ITEM_PICK	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_ITEM_PICK.strIP);
			break;

		case	dLOG_ITEM_SELL	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_ITEM_SELL.strIP);
			break;


		case	dLOG_USE_ITEM	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_USE_ITEM.strIP);
			break;
		case	dLOG_CREATE_ITEM	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_CREATE_ITEM.strIP);
			break;
		case	dLOG_EXPIRED_ITEM	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_EXPIRED_ITEM.strIP);
			break;
		case	dLOG_DOUBLE_ITEM	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_DOUBLE_ITEM.strIP);
			break;
		case	dLOG_ENCHANT_ITEM	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_ENCHANT_ITEM.strIP);
			break;
		case	dLOG_PITCHMAN_SHOP_TRADE	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_PITCHMAN_SHOP_TRADE.strIP);
			strcpy(g_astrIPForLog[1],_lpAllLog->mLOG_PITCHMAN_SHOP_TRADE.strOwnerIP);
			iCount	=	2;
			break;
		case	dLOG_ITEM_DEL	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_ITEM_DEL.strIP);
			break;
		case	dLOG_ITEM_TRADE	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_ITEM_TRADE.strSenderIP);
			strcpy(g_astrIPForLog[1],_lpAllLog->mLOG_ITEM_TRADE.strRecverIP);
			iCount	=	2;
			break;
		case	dLOG_ITEM_BANK_IN	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_ITEM_BANK_IN.strIP);
			break;
		case	dLOG_ITEM_BANK_OUT	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_ITEM_BANK_OUT.strIP);
			break;
		case	dLOG_ITEM_WITHDRAW_PREMIUM_ITEM	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_ITEM_WITHDRAW_PREMIUM_ITEM.strIP);
			break;
		case	dLOG_ITEM_CREATE_PREMIUM_ITEM	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_ITEM_CREATE_PREMIUM_ITEM.strIP);
			break;
		case	dLOG_AVA_CREATE	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_AVA_CREATE.strIP);
			break;
		case	dLOG_AVA_DELETE	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_AVA_DELETE.strIP);
			break;
		case	dLOG_AVA_LOGIN	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_AVA_LOGIN.strIP);
			break;
		case	dLOG_AVA_LOGOUT	:
			strcpy(g_astrIPForLog[0],_lpAllLog->mLOG_AVA_LOGOUT.strIP);
			break;

		case	dLOG_SYSTEM		:
		{
			LOG_SYSTEM	*lpLog	=	(LOG_SYSTEM	*)_lpAllLog;

			switch(lpLog->wWork)
			{
				case	eSLT_HACKING	:
				case	eSLT_COMPLETE_QUEST	:
				case	eSLT_RECEIVE_QUEST_EXP	:
				case	eSLT_TREASURE_MAP_EVENT	:
				case	eSLT_BACK_SERVER	:
				case	eSLT_CREATE_MINI_PET	:
				case	eSLT_EXTEND_MINI_PET_TIME:
				case	eSLT_SEAL_MINI_PET	:
				case	eSLT_UNSEAL_MINI_PET	:

				case	eSLT_TRANS_AVATAR_TO_GVG_SERVER	:
				case	eSLT_MINI_PET_FEED_ITEM	:
				case	eSLT_REMOVE_MINI_PET	:
				case	eSLT_RESTRAINT_MINIPET	:
				case	eSLT_OPERATOR_CHAT_LOG	:	//	이름 2개
				case	eSLT_REMOVE_A_PLACE_RELATED_OF_GUILD	:
				case	eSLT_BUY_ITEM	:
				case	eSLT_RECORD_GUILD_HONOR_POINT	:
				case	eSLT_BEGIN_DUEL	:
				case	eSLT_RESULT_OF_DUEL	:
					
				case	eSLT_DISCONNECT_IN_DUEL_FIELD	:
				case	eSLT_REBIRTH	:
					strcpy(g_astrIPForLog[0],lpLog->strIP);
					break;

				default	:
					iCount	=	0;
					break;
			}

			break;
		}

		case	dLOG_GUILD_WORK	:
		{
			LOG_GUILD_WORK	*lpLog	=	(LOG_GUILD_WORK	*)_lpAllLog;

			switch(lpLog->wWork)
			{
				case	eGW_TRANS_GUILD_TO_GVG_SERVER			:
				case	eGW_GUILD_DUNGEON_PLAY			:
				case	eGW_ACTIVE_GUILD_MASTER_GRACE	:
				case	eGW_ACTIVE_GUILD_ROEN_GOOD_WILL	:
					strcpy(g_astrIPForLog[0],lpLog->strIP);
					break;

				default	:
					iCount	=	0;
					break;
			}

			break;
		}


		default	:
			iCount	=	0;
			break;
	}

	return	iCount;
}

//	로그 타입에 따라 필터링 한 번 했지만
//	추가로 로그의 세부 로그 변수에 따라 다시 한번 필터링 한다.
BOOL
CheckDetailDataFilter(int _iFilterIndex,char *_lpstrValue,BOOL _bIsCheckAndCondition,char *_lpstrFilterId,char *_lpstrFilterName,DWORD _dwFilterItem,
			DWORD _dwFilterItemSerial,char* _lpstrFilterIp,ALL_LOGMSG *_lpLog)
{
	if	(_lpLog->base.wType == dLOG_DOUBLE_ITEM && _lpLog->mLOG_DOUBLE_ITEM.wWhere	>	3)
		_lpLog->base.wType = dLOG_ENCHANT_ITEM;

	if	(_iFilterIndex == eITEM_FIND_INCORECT_ITEM)
		if	(!IsExistIncorrectPrefixItemLog(_lpLog))
			return	FALSE;

	switch(_lpLog->base.wType)
	{
		case	dLOG_ERROR	:
			if	(_iFilterIndex	!=	eABOUT_ERROR)
				return	FALSE;
			return	TRUE;

		case dLOG_ITEM_CREATE_PREMIUM_ITEM	:
		{
			LOG_ITEM_CREATE_PREMIUM_ITEM	*lpData	=	&_lpLog->mLOG_ITEM_CREATE_PREMIUM_ITEM;

			if	(_iFilterIndex == eABOUT_CARROT_SHOP && lpData->wMethod	!=	eCPIM_BUY_BY_CARROT)
				return	FALSE;
			break;
		}
		case dLOG_CREATE_ITEM:
		{
			if	(_lpLog->mLOG_CREATE_ITEM.base.wSize	!=	sizeof(LOG_CREATE_ITEM))	//	옛날꺼
				_lpLog->mLOG_CREATE_ITEM.wMethod		=	eCIM_OLD;

			if	(_iFilterIndex == eABOUT_PET	&&	_lpLog->mLOG_CREATE_ITEM.wMethod	!=	eCIM_BREEDING_RECORD_BOOK)
				return	FALSE;
			if	(_iFilterIndex == eABOUT_BOTTOMLESS_BOX	&&	_lpLog->mLOG_CREATE_ITEM.wMethod	!=	eCIM_USE_BOTTOMLESS_BOX)
				return	FALSE;
			if	(_iFilterIndex == eABOUT_GUILD_WORK)
				if	(_lpLog->mLOG_CREATE_ITEM.wMethod	!=	eCIM_WITHDRAW_FROM_GUILD_INVENTORY && _lpLog->mLOG_CREATE_ITEM.wMethod	!=	eCIM_WITHDRAW_FROM_GUILD_INVENTORY2)
					return	FALSE;

			if	(_iFilterIndex == eABOUT_MINIPET)
				if	(_lpLog->mLOG_CREATE_ITEM.wMethod	!=	eCIM_BLOCK_LIGHT_POCKET)
					return	FALSE;
			break;
		}
		case dLOG_ENCHANT_ITEM:
		{
			if	(_iFilterIndex == eITEM_RENEW_TERM)
				if	(_lpLog->mLOG_ENCHANT_ITEM.wWork != eEIW_RENEW_TERM)
					return	FALSE;
			break;
		}

		case dLOG_ITEM_WITHDRAW_PREMIUM_ITEM:
		{
			LOG_ITEM_WITHDRAW_PREMIUM_ITEM	*lpData	=	&_lpLog->mLOG_ITEM_WITHDRAW_PREMIUM_ITEM;

			if	(lpData->wMethod	!=	eWPIM_WITHDRAW_CART_ITEM)
				return	FALSE;

			break;
		}

		case dLOG_ITEM_BANK_OUT:
		{
			if	(_iFilterIndex	== eABOUT_OPERATOR)
				if	(_lpLog->mLOG_ITEM_BANK_OUT.bf1IsOperator	==	FALSE)
					return	FALSE;

			break;
		}

		case dLOG_ITEM_BANK_IN:
		{
			if	(_iFilterIndex	== eABOUT_OPERATOR)
				if	(_lpLog->mLOG_ITEM_BANK_IN.bf1IsOperator	==	FALSE)
					return	FALSE;

			break;
		}

		case dLOG_ITEM_TRADE:
		{
			if	(_iFilterIndex	== eABOUT_OPERATOR)
				if	(_lpLog->mLOG_ITEM_TRADE.bf1IsOperator	==	FALSE)
					return	FALSE;

			break;
		}

		case dLOG_PITCHMAN_SHOP_TRADE	:
		{
			if	(_iFilterIndex	== eABOUT_OPERATOR)
				if	(_lpLog->mLOG_ITEM_TRADE.bf1IsOperator	==	FALSE)
					return	FALSE;
			break;
		}

		case dLOG_ITEM_DEL:
		{
			LOG_ITEM_DEL	*lpDelLog	=	(LOG_ITEM_DEL	*)_lpLog;

			if	(_iFilterIndex == eABOUT_PET	&&	lpDelLog->bf6Method	!=	eRI_UNSEAL_BREEDING_RECORD_BOOK)
				return	FALSE;

			if	(_iFilterIndex == eABOUT_BOTTOMLESS_BOX)
				if	(lpDelLog->bf6Method	!=	eRI_BY_USE_BOTTOMLESS_BOX	)
					return	FALSE;

			if	(_iFilterIndex == eABOUT_MINIPET)
				if	(lpDelLog->bf6Method	!=	eRI_BY_SEAL_MINIPET	&&	lpDelLog->bf6Method	!=	eRI_BY_UNSEAL_MINIPET	&&	lpDelLog->bf6Method	!=	eRI_BY_USE_POLISHER)
					return	FALSE;

			if	(_iFilterIndex == eABOUT_GUILD_WORK)
			{
				if	(lpDelLog->bf6Method	!=	eRI_BY_STORE_GUILD	&&	lpDelLog->bf6Method	!=	eRI_BY_FEED_TO_GUILD_PET	&&
					lpDelLog->bf6Method	!=	eRI_BY_ADD_RELATED_PLACE)
					return	FALSE;
			}

			break;
		}

		case dLOG_ITEM_PICK	:
		{
			if	(_iFilterIndex	== eABOUT_MINIPET)
				if	(_lpLog->mLOG_ITEM_PICK.bf6Method	!=	ePIM_MINIPET_PICK)
					return	FALSE;

			if	(_iFilterIndex	== eABOUT_GUILD_WORK)
				if	(_lpLog->mLOG_ITEM_PICK.bf1IsIncludeGuildInfo==	FALSE)
					return	FALSE;
			break;
		}

		case dLOG_SYSTEM	:
		{
			LOG_SYSTEM	*lpLogSystem	=	(LOG_SYSTEM	*)_lpLog;
			int			iSystemLogType	=	lpLogSystem->wWork;

			if	(_iFilterIndex == eITEM_BUY_SELL	||	_iFilterIndex == eITEM_ALL)
			{
				if	(iSystemLogType	!=	eSLT_BUY_ITEM)
					return	FALSE;
			}
			else
			if	(_iFilterIndex == eABOUT_QUEST)
			{
				if	(iSystemLogType	!=	eSLT_RECEIVE_QUEST_EXP	&&	iSystemLogType	!=	eSLT_COMPLETE_QUEST)
					return	FALSE;

				LOG_S_COMPLETE_QUEST	*lpData	=	(LOG_S_COMPLETE_QUEST	*)lpLogSystem;
				CQuestItem				*lpQuest=	CQuest::GetQuest(lpData->bf2Type,lpData->bf14QuestIndex);

				if	(iSystemLogType	==	eSLT_COMPLETE_QUEST)
					if	(_lpstrValue[0] && lpQuest && strstr(lpQuest->m_strName,_lpstrValue)	==	NULL)
						return	FALSE;

				if	(iSystemLogType	==	eSLT_RECEIVE_QUEST_EXP)
				{
					if	(_dwFilterItemSerial	!=	0xffffffff)
					{
						LOG_S_RECEIVE_QUEST_REWARD_EXP	*lpData	=	(LOG_S_RECEIVE_QUEST_REWARD_EXP	*)lpLogSystem;

						if	(_dwFilterItemSerial	!=	(DWORD)lpData->iRewardExp)
							return	FALSE;

						_dwFilterItemSerial	=	_dwFilterItemSerial;
					}
				}
			}
			else
			if	(_iFilterIndex == eABOUT_EVENT)
			{
				if	(iSystemLogType	!=	eSLT_TREASURE_MAP_EVENT)
					return	FALSE;
			}
			else
			if	(_iFilterIndex == eABOUT_BACK_SERVER)
			{
				if	(iSystemLogType	!=	eSLT_BACK_SERVER)
					return	FALSE;
			}
			else
			if	(_iFilterIndex == eABOUT_OPERATOR)
			{
				if	(iSystemLogType	!=	eSLT_OPERATOR_CHAT_LOG)
					return	FALSE;
			}
			else
			if	(_iFilterIndex == eABOUT_GUILD_WORK)
			{
				if	(iSystemLogType	!=	eSLT_REMOVE_A_PLACE_RELATED_OF_GUILD	&&	
					iSystemLogType	!=	eSLT_RECORD_GUILD_HONOR_POINT &&
					iSystemLogType	!=	eSLT_TRANS_AVATAR_TO_GVG_SERVER	)

					return	FALSE;
			}
			else
			if	(_iFilterIndex	==	eABOUT_HACKING)
			{
				if	(iSystemLogType	!=	eSLT_HACKING)
					return	FALSE;
			}
			else
			if	(_iFilterIndex	==	eABOUT_REBIRTH)
			{
				if	(iSystemLogType	!=	eSLT_REBIRTH)
					return	FALSE;
			}
			else
			if	(_iFilterIndex	==	eABOUT_DUEL)
			{
				if	(iSystemLogType	<	eSLT_BEGIN_DUEL	||	iSystemLogType	>	eSLT_DISCONNECT_IN_DUEL_FIELD)
					return	FALSE;

				LOG_S_BEGIN_DUEL	*lpLog	=	(LOG_S_BEGIN_DUEL	*)lpLogSystem;

				if	(g_duelFilter.isExistDuelSerial(lpLog->iDuelSerial)	==	FALSE)
					return	FALSE;

				int	iValue	=	-1;

				if	(_lpstrValue[0])
					iValue	=	atoi(_lpstrValue);

				if	(iValue	!=	-1)
					if	(lpLog->iDuelSerial	!=	iValue)
						return	FALSE;
			}
			else
			if	(_iFilterIndex	==	eABOUT_MINIPET)
			{
				if	(iSystemLogType	!=	eSLT_CREATE_MINI_PET	&&	iSystemLogType	!=	eSLT_REMOVE_MINI_PET	&&
					iSystemLogType	!=	eSLT_EXTEND_MINI_PET_TIME	&&	iSystemLogType	!=	eSLT_MINI_PET_FEED_ITEM	&&
					iSystemLogType	!=	eSLT_MIX_MINIPET	&&	iSystemLogType	!=	eSLT_RESTRAINT_MINIPET)
					return	FALSE;
			}

			break;
		}
	}

	return	TRUE;
}

BOOL
CheckIsSerchData(int _iFilterIndex,char *_lpstrValue,BOOL _bIsCheckAndCondition,
				 char *_lpstrFilterId,char *_lpstrFilterName,DWORD _dwFilterItem,DWORD _dwFilterItemSerial,
				char* _lpstrFilterIp,ALL_LOGMSG *_lpLog)
{
	if	(CheckDetailDataFilter(_iFilterIndex,_lpstrValue,_bIsCheckAndCondition,_lpstrFilterId,_lpstrFilterName,_dwFilterItem,
					_dwFilterItemSerial,_lpstrFilterIp,_lpLog)	==	FALSE)
		return	FALSE;

	if	(_iFilterIndex == eABOUT_QUEST)
		_dwFilterItemSerial	=	0xffffffff;

	BOOL	bIsTargetLog	=	FALSE;
	BOOL	bIsExistFilter	=	FALSE;

	BOOL	bIsFindID		=	TRUE;
	BOOL	bIsFindName		=	TRUE;
	BOOL	bIsFindIP		=	TRUE;
	BOOL	bIsFindItem		=	TRUE;
	BOOL	bIsFindItemSerial=	TRUE;
	BOOL	bIsFindSetItem	=	TRUE;
	BOOL	bIsFindEternalItem=	TRUE;
	BOOL	bIsFindUniqueItem=	TRUE;
	BOOL	bIsFindDX_Item	=	TRUE;
	BOOL	bIsFindEternalGrade=	TRUE;
	BOOL	bIsFindJobFilter	=	TRUE;

	if	(_lpstrFilterId[0])
	{
		bIsExistFilter	=	TRUE;
		bIsFindID		=	FALSE;

		int	iCount	=	ExportIDWithLog(_lpLog);

		for (int i=0;i<iCount;i++)
		{
			if	(stricmp(_lpstrFilterId,g_astrIDForLog[i])==0)
			{
				bIsTargetLog	=	TRUE;
				bIsFindID		=	TRUE;

				break;
			}
		}
	}

	if	(_lpstrFilterName[0])
	{
		bIsExistFilter	=	TRUE;
		bIsFindName		=	FALSE;

		if	(_iFilterIndex	==	eABOUT_GUILD_WORK)
		{
			int	iCount	=	ExportGuildNameWithLog(_lpLog);

			if	(iCount	==	0	&&	_bIsCheckAndCondition	==	TRUE)
				return	FALSE;

			for (int i=0;i<iCount;i++)
			{
				if	(stricmp(_lpstrFilterName,g_astrGuildNameLog[i])==0)
				{
					bIsTargetLog	=	TRUE;
					bIsFindName		=	TRUE;

					break;
				}
			}
		}
		else
		{
			int	iCount	=	ExportNameWithLog(_lpLog);

			if	(iCount	==	0	&&	_bIsCheckAndCondition	==	TRUE)
				return	FALSE;

			for (int i=0;i<iCount;i++)
			{
				if	(stricmp(_lpstrFilterName,g_astrNameForLog[i])==0)
				{
					bIsTargetLog	=	TRUE;
					bIsFindName		=	TRUE;

					break;
				}
			}
		}
	}

	if	(_dwFilterItem	!=	0xffffffff)
	{
		bIsExistFilter	=	TRUE;
		bIsFindItem		=	FALSE;

		int	iCount		=	GetItemInLog(_lpLog);

		for (int i=0;i<iCount;i++)
		{
			cITEM	*lpItem	=	&g_aItemListForLog[i];

			if	(lpItem->m_wBaseItem	==	_dwFilterItem)
			{
				bIsFindItem		=	TRUE;
				bIsTargetLog	=	TRUE;
				break;
			}
		}
	}

	if	(g_bIsCheckIncorrectItem)
	{
		bIsExistFilter	=	TRUE;
		bIsFindItem		=	FALSE;

		if	(GetItemInLog(_lpLog))
		{
			bIsFindItem		=	TRUE;
			bIsTargetLog	=	TRUE;
		}
	}

	if	(g_bIsOnlyUnique)
	{
		bIsExistFilter		=	TRUE;
		bIsFindUniqueItem	=	FALSE;

		int		iCount		=	GetItemInLog(_lpLog);

		for (int i=0;i<iCount;)
		{
			cITEM	*lpItem	=	&g_aItemListForLog[i];

			if	(lpItem->isUniqueItem())
			{
				bIsFindUniqueItem	=	TRUE;
				bIsTargetLog		=	TRUE;
				i++;
			}
			else
			{
				memcpy(&g_aItemListForLog[i],&g_aItemListForLog[iCount-1],sizeof(cITEM));
				iCount--;
			}
		}
	}
	
	if	(g_bIsOnlyDX)
	{
		bIsExistFilter	=	TRUE;
		bIsFindDX_Item	=	FALSE;

		int		iCount		=	GetItemInLog(_lpLog);

		for (int i=0;i<iCount;)
		{
			cITEM	*lpItem	=	&g_aItemListForLog[i];

			if	(lpItem->isDXItem())
			{
				bIsFindDX_Item	=	TRUE;
				bIsTargetLog		=	TRUE;
				i++;
			}
			else
			{
				memcpy(&g_aItemListForLog[i],&g_aItemListForLog[iCount-1],sizeof(cITEM));
				iCount--;
			}
		}
	}
	
	if	(g_bIsOnlyEternal)
	{
		bIsExistFilter	=	TRUE;
		bIsFindEternalItem	=	FALSE;

		int		iCount		=	GetItemInLog(_lpLog);

		for (int i=0;i<iCount;)
		{
			cITEM	*lpItem	=	&g_aItemListForLog[i];

			if	(lpItem->getBasicItem()->isEternalItem())
			{
				bIsFindEternalItem	=	TRUE;
				bIsTargetLog		=	TRUE;
				i++;
			}
			else
			{
				memcpy(&g_aItemListForLog[i],&g_aItemListForLog[iCount-1],sizeof(cITEM));
				iCount--;
			}
		}
	}

	if	(g_bIsOnlySetItem)
	{
		bIsExistFilter	=	TRUE;
		bIsFindSetItem	=	FALSE;

		int	iCount		=	GetItemInLog(_lpLog);

		for (int i=0;i<iCount;)
		{
			cITEM	*lpItem	=	&g_aItemListForLog[i];

			if	(lpItem->getSetItem())
			{
				bIsFindSetItem	=	TRUE;
				bIsTargetLog	=	TRUE;
				i++;
			}
			else
			{
				memcpy(&g_aItemListForLog[i],&g_aItemListForLog[iCount-1],sizeof(cITEM));
				iCount--;
			}
		}
	}

	if	(g_iEternalGrade)
	{
		bIsExistFilter		=	TRUE;
		bIsFindEternalGrade	=	FALSE;

		int	iCount		=	GetItemInLog(_lpLog);

		for (int i=0;i<iCount;)
		{
			cITEM	*lpItem	=	&g_aItemListForLog[i];

			if	(lpItem->getBasicItem()->getCurrentEternalItemLevel()	>=	g_iEternalGrade)
			{
				bIsFindEternalGrade	=	TRUE;
				bIsTargetLog	=	TRUE;
				i++;
			}
			else
			{
				memcpy(&g_aItemListForLog[i],&g_aItemListForLog[iCount-1],sizeof(cITEM));
				iCount--;
			}
		}
	}

	if	(g_iJobFilter	!=	-1)
	{
		bIsExistFilter		=	TRUE;
		bIsFindJobFilter	=	FALSE;

		int	iCount		=	GetItemInLog(_lpLog);

		for (int i=0;i<iCount;)
		{
			if	(g_aItemListForLog[i].isEnableJob(g_iJobFilter))
			{
				bIsFindJobFilter=	TRUE;
				bIsTargetLog	=	TRUE;
				i++;
			}
			else
			{
				memcpy(&g_aItemListForLog[i],&g_aItemListForLog[iCount-1],sizeof(cITEM));
				iCount--;
			}
		}
	}

	if	(_dwFilterItemSerial	!=	0xffffffff)
	{
		bIsExistFilter	=	TRUE;
		bIsFindItemSerial=	FALSE;
		int	iCount	=	GetItemInLog(_lpLog);

		for (int i=0;i<iCount;i++)
		{
			cITEM	*lpItem	=	&g_aItemListForLog[i];

			if	(lpItem->m_dwSerial	==	_dwFilterItemSerial)
			{
				bIsFindItemSerial=	TRUE;
				bIsTargetLog	=	TRUE;
				break;
			}
		}
	}

	if	(_lpstrFilterIp[0])
	{
		bIsExistFilter	=	TRUE;
		bIsFindIP		=	FALSE;

		int	iCount	=	ExportIPWithLog(_lpLog);

		for (int i=0;i<iCount;i++)
		{
			if	(stricmp(_lpstrFilterIp,g_astrIPForLog[i])	==	0)
			{
				bIsFindIP		=	TRUE;
				bIsTargetLog	=	TRUE;

				break;
			}
		}
	}

	if	(!bIsExistFilter)
		return	TRUE;

	if	(_bIsCheckAndCondition)
	{
		if	(bIsFindID		&&	bIsFindName		&&	bIsFindIP		&&	bIsFindItem		&&	bIsFindItemSerial	&&	
			bIsFindSetItem	&&	bIsFindEternalItem	&&	bIsFindUniqueItem	&&	bIsFindDX_Item	&&	bIsFindEternalGrade && bIsFindJobFilter)
			return	TRUE;

		return	FALSE;
	}

	return	bIsTargetLog;
}

BOOL
IsExistIncorrectPrefixItemLog(ALL_LOGMSG *_lpLog)	//	잘못된 접두사 아이템을 가진 로그냐?
{
	int	iCount	=	GetItemInLog(_lpLog);

	for (int i=0;i<iCount;i++)
		if	(g_aItemListForLog[i].getIncorrectPrefixCount())
			return	TRUE;

	return	FALSE;
}

int
AddFilterItem(cITEM *_lpItemBuffer,cITEM *_lpItem,int _iCount)
{
	if	(!_lpItem || _lpItem->getBasicItem() == NULL)
		return	_iCount;

	if	(g_bIsCheckIncorrectItem)
	{
		if	(_lpItem->getBasicItem()->m_bIsExceptionItem || _lpItem->getBasicItem()->m_attr.isDurabilityToUseCount)
			return	_iCount;

		if	(_lpItem->m_bCount	<=	_lpItem->getBasicItem()->m_wStackLimit)
			return	_iCount;
	}

	memcpy(&_lpItemBuffer[_iCount],_lpItem,sizeof(cITEM));

	return	_iCount+1;
}

int
GetItemInLog(ALL_LOGMSG *_lpLog,cITEM *_lpItemBuffer)	//	이 로그에서 아이템 추출
{
	if	(_lpItemBuffer	==	NULL)
		_lpItemBuffer	=	g_aItemListForLog;

	int	iCount	=	0;

	switch(_lpLog->base.wType)
	{
		case dLOG_SYSTEM				:
		{
			LOG_SYSTEM	*lpSystemLog	=	&_lpLog->mLOG_SYSTEM;

			if	(lpSystemLog->wWork	==	eSLT_CREATE_MINI_PET)
			{	
				LOG_S_CREATE_MINI_PET	*lpLog	=	(LOG_S_CREATE_MINI_PET	*)lpSystemLog;

				iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
				break;
			}
			if	(lpSystemLog->wWork	==	eSLT_MINI_PET_FEED_ITEM)
			{	
				LOG_S_MINI_PET_FEED_ITEM	*lpLog	=	(LOG_S_MINI_PET_FEED_ITEM	*)lpSystemLog;

				iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->item,iCount);
				break;
			}
			if	(lpSystemLog->wWork	==	eSLT_BUY_ITEM)
			{	
				LOG_S_BUY_ITEM	*lpLog	=	(LOG_S_BUY_ITEM	*)lpSystemLog;

				iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
				break;
			}

			break;
		}

		case dLOG_USE_ITEM				:
		{
			LOG_USE_ITEM	*lpLog	=	&_lpLog->mLOG_USE_ITEM;

			iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
			break;
		}

		case	dLOG_EXPIRED_ITEM		:
		{
			LOG_EXPIRED_ITEM	*lpLog	=	&_lpLog->mLOG_EXPIRED_ITEM;

			iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
			break;
		}

		case	dLOG_DOUBLE_ITEM			:
		{
			LOG_DOUBLE_ITEM	*lpLog	=	&_lpLog->mLOG_DOUBLE_ITEM;

			iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
			break;
		}

		case	dLOG_ENCHANT_ITEM			:
		{
			LOG_ENCHANT_ITEM	*lpLog	=	&_lpLog->mLOG_ENCHANT_ITEM;

			iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
			break;
		}

		case dLOG_ITEM_DROP				:		//	87
		{
			LOG_ITEM_DROP	*lpLog	=	&_lpLog->mLOG_ITEM_DROP;

			iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
			break;
		}
		case dLOG_ITEM_PICK				:		//	87
		{
			LOG_ITEM_PICK	*lpLog	=	&_lpLog->mLOG_ITEM_PICK;

			iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
			break;
		}

		case dLOG_ITEM_SELL				:		//	87
		{
			LOG_ITEM_SELL	*lpLog	=	&_lpLog->mLOG_ITEM_SELL;

			iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
			break;
		}

		case dLOG_ITEM_DEL				:		//	87
		{
			LOG_ITEM_DEL	*lpLog	=	&_lpLog->mLOG_ITEM_DEL;

			iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
			break;
		}
		case dLOG_CREATE_ITEM			:
		{
			LOG_CREATE_ITEM	*lpLog	=	&_lpLog->mLOG_CREATE_ITEM;

			iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
			break;
		}

		case dLOG_ITEM_CREATE_PREMIUM_ITEM	:
		{
			LOG_ITEM_CREATE_PREMIUM_ITEM	*lpLog	=	&_lpLog->mLOG_ITEM_CREATE_PREMIUM_ITEM;

			iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
			break;
		}

		case dLOG_PITCHMAN_SHOP_TRADE	:
		{
			LOG_ITEM_PICK	*lpLog	=	&_lpLog->mLOG_ITEM_PICK;

			iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem,iCount);
			break;
		}

		case dLOG_ITEM_TRADE			:		//	921
		{
			LOG_ITEM_TRADE	*lpLog	=	&_lpLog->mLOG_ITEM_TRADE;

			int	iLogCount	=	lpLog->wTrader1ItemCount+lpLog->bf15Trader2ItemCount;

			if	(iLogCount	>	sizeof(lpLog->cItem)/sizeof(CItemDefine))
			{
				int	iInfoSize	=	sizeof(LOG_ITEM_TRADE)-sizeof(lpLog->cItem);
				iLogCount		=	(lpLog->base.wSize-iInfoSize)/sizeof(CItemDefine);
			}

			for (int i=0;i<iLogCount;i++)
				iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem[i],iCount);
			break;
		}

		case dLOG_ITEM_BANK_IN			:
		{
			LOG_ITEM_BANK_IN	*lpLog	=	&_lpLog->mLOG_ITEM_BANK_IN;

			int	iLogCount	=	lpLog->bf15Count;

			if	(iLogCount	>	sizeof(lpLog->cItem)/sizeof(CItemDefine))
			{
				int	iInfoSize	=	sizeof(LOG_ITEM_BANK_IN)-sizeof(lpLog->cItem);
				iLogCount		=	(lpLog->base.wSize-iInfoSize)/sizeof(CItemDefine);
			}

			for (int i=0;i<iLogCount;i++)
				iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem[i],iCount);

			break;
		}

		case dLOG_ITEM_BANK_OUT			:
		{
			LOG_ITEM_BANK_OUT	*lpLog	=	&_lpLog->mLOG_ITEM_BANK_OUT;

			int	iLogCount	=	lpLog->bf15Count;

			if	(iLogCount	>	sizeof(lpLog->cItem)/sizeof(CItemDefine))
			{
				int	iInfoSize	=	sizeof(LOG_ITEM_BANK_OUT)-sizeof(lpLog->cItem);
				iLogCount		=	(lpLog->base.wSize-iInfoSize)/sizeof(CItemDefine);
			}

			for (int i=0;i<iLogCount;i++)
				iCount	=	AddFilterItem(_lpItemBuffer,(cITEM*)&lpLog->cItem[i],iCount);

			break;
		}

	}

	return	iCount;
}
