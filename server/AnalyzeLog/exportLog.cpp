#include "stdafx.h"
#include "AnalyzeLog.h"
#include "AnalyzeLogDlg.h"
#include "LinkedList.h"
#include "cITEM.h"
#include "cSRVUTIL.h"
#include "CMessage.h"
#include "mmsystem.h"
#include "Tracer.h"
#include "exportLog.h"
#include "quest.h"
#include "cItemPrefix.h"
#include "job.h"
#include "log_filtering.h"
#include <io.h>

MapLevelLog	g_mapLevelForRebirthLog;

BOOL	g_bIsHideItemInfo				=	FALSE;
BOOL	g_bIsExportIncorrectPrefixItem	=	FALSE;
enum
{ // 길드 삭제 이유 
		eDGR_NORMAL,		// 마지막 길드원 탈퇴
		eDGR_CREATE_FAIL,	//	생성 실패
		eDGR_TIME_OVER,		//	길드원 접속이 없어서
};
enum
{ // 길드 랭크 변경  
		eCGR_APPOINT	=	1,		// 	임명
		eCGR_DISMISSAL,		//	해임
		eCGR_CHANGERANK_MASTER_APPOINT_SPECIAL,	//	마스터가 케릭터 지움
		eCGR_TIME_OVER,		//	일정기간 접속안함
};

char*
GetMiniPetCode(cMiniPetCommon *_lpMiniPet)
{
	static char strCode[1024];

	sprintf(strCode,"#miniPet \"%s\" [%d,%d,%d] [%d/%d/%d %d:%d] [%d,%d,%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d]",
	_lpMiniPet->m_strName,
	_lpMiniPet->m_bf7Level,	_lpMiniPet->m_bf16Exp,	_lpMiniPet->m_bf5Type,
	_lpMiniPet->m_bf5ExpireYear+2000,_lpMiniPet->m_bf4ExpireMonth,_lpMiniPet->m_bf5ExpireDay,_lpMiniPet->m_bf5ExpireHour,_lpMiniPet->m_bf6ExpireMinute,
	_lpMiniPet->m_iVitalPoint,_lpMiniPet->m_iEnergyPoint,_lpMiniPet->m_iMineralPoint,
	_lpMiniPet->m_aAbility[0].m_bf11Skill,_lpMiniPet->m_aAbility[0].m_bf4Level,
	_lpMiniPet->m_aAbility[1].m_bf11Skill,_lpMiniPet->m_aAbility[1].m_bf4Level,
	_lpMiniPet->m_aAbility[2].m_bf11Skill,_lpMiniPet->m_aAbility[2].m_bf4Level,
	_lpMiniPet->m_aAbility[3].m_bf11Skill,_lpMiniPet->m_aAbility[3].m_bf4Level,
	_lpMiniPet->m_aAbility[4].m_bf11Skill,_lpMiniPet->m_aAbility[4].m_bf4Level,
	_lpMiniPet->m_aAbility[5].m_bf11Skill,_lpMiniPet->m_aAbility[5].m_bf4Level,
	_lpMiniPet->m_aAbility[6].m_bf11Skill,_lpMiniPet->m_aAbility[6].m_bf4Level,
	_lpMiniPet->m_aAbility[7].m_bf11Skill,_lpMiniPet->m_aAbility[7].m_bf4Level,
	_lpMiniPet->m_aAbility[8].m_bf11Skill,_lpMiniPet->m_aAbility[8].m_bf4Level,
	_lpMiniPet->m_aAbility[9].m_bf11Skill,_lpMiniPet->m_aAbility[9].m_bf4Level,
	_lpMiniPet->m_aAbility[10].m_bf11Skill,_lpMiniPet->m_aAbility[10].m_bf4Level,
	_lpMiniPet->m_bf7SkillPoint);

	return	strCode;
}

void
exportItemInfo(char *_lpstrText,CItemDefine *_lpItem)
{
	cITEM	*lpItem		=	(cITEM	*)_lpItem;

	if	(_lpItem->m_wBaseItem == 0)
		sprintf(_lpstrText,"GD [%d]\n",_lpItem->m_dwSerial);
	else
		sprintf(_lpstrText,"IT [%d:%u] [%d,%d] [%d,%d] <%d:%d,%d><%d:%d,%d><%d:%d,%d> %d       [%.2d/%.2d/%.2d %.2d:%.2d]\n",
			_lpItem->m_wBaseItem,_lpItem->m_dwSerial,_lpItem->m_bCount,_lpItem->m_bDurability,
			_lpItem->m_aOption[0],_lpItem->m_aOption[1],
			_lpItem->m_aPrefix[0].m_wPrefix,_lpItem->m_aPrefix[0].m_aValue[0],_lpItem->m_aPrefix[0].m_aValue[1],
			_lpItem->m_aPrefix[1].m_wPrefix,_lpItem->m_aPrefix[1].m_aValue[0],_lpItem->m_aPrefix[1].m_aValue[1],
			_lpItem->m_aPrefix[2].m_wPrefix,_lpItem->m_aPrefix[2].m_aValue[0],_lpItem->m_aPrefix[2].m_aValue[1],
			_lpItem->getBitFieldValue(),
			_lpItem->m_year,_lpItem->m_month,_lpItem->m_day,_lpItem->m_hour,_lpItem->m_minute);
}

void
exportItemFullInfo(char *_lpstrText,char *_lpstrComment,CItemDefine *_lpItem,BOOL _bIsExportComment)
{
	if	(g_bIsHideItemInfo)
		_bIsExportComment	=	FALSE;

	char	strCommentHead[256];

	strCommentHead[0]	=	NULL;

	cITEM	*lpItem		=	(cITEM	*)_lpItem;

	if	(_lpstrComment[0])
		strcpy(strCommentHead,_lpstrComment);
	
	if	(_lpItem->m_wBaseItem == 0)
		sprintf(_lpstrText,"%s GD [%d]\n",_lpstrText,_lpItem->m_dwSerial);
	else
	{
		if	(lpItem->getIncorrectPrefixCount())
		{
			sprintf(_lpstrText,"%s IT [%d:%u] [%d,%d] [%d,%d] <%d:%d,%d><%d:%d,%d><%d:%d,%d> %d       [%.2d/%.2d/%.2d %.2d:%.2d] incorrect!!!!!\n",_lpstrText,
				_lpItem->m_wBaseItem,_lpItem->m_dwSerial,_lpItem->m_bCount,_lpItem->m_bDurability,
				_lpItem->m_aOption[0],_lpItem->m_aOption[1],
				_lpItem->m_aPrefix[0].m_wPrefix,_lpItem->m_aPrefix[0].m_aValue[0],_lpItem->m_aPrefix[0].m_aValue[1],
				_lpItem->m_aPrefix[1].m_wPrefix,_lpItem->m_aPrefix[1].m_aValue[0],_lpItem->m_aPrefix[1].m_aValue[1],
				_lpItem->m_aPrefix[2].m_wPrefix,_lpItem->m_aPrefix[2].m_aValue[0],_lpItem->m_aPrefix[2].m_aValue[1],
				_lpItem->getBitFieldValue(),
				_lpItem->m_year,_lpItem->m_month,_lpItem->m_day,_lpItem->m_hour,_lpItem->m_minute);
		}
		else
		{
			sprintf(_lpstrText,"%s IT [%d:%u] [%d,%d] [%d,%d] <%d:%d,%d><%d:%d,%d><%d:%d,%d> %d       [%.2d/%.2d/%.2d %.2d:%.2d]\n",_lpstrText,
				_lpItem->m_wBaseItem,_lpItem->m_dwSerial,_lpItem->m_bCount,_lpItem->m_bDurability,
				_lpItem->m_aOption[0],_lpItem->m_aOption[1],
				_lpItem->m_aPrefix[0].m_wPrefix,_lpItem->m_aPrefix[0].m_aValue[0],_lpItem->m_aPrefix[0].m_aValue[1],
				_lpItem->m_aPrefix[1].m_wPrefix,_lpItem->m_aPrefix[1].m_aValue[0],_lpItem->m_aPrefix[1].m_aValue[1],
				_lpItem->m_aPrefix[2].m_wPrefix,_lpItem->m_aPrefix[2].m_aValue[0],_lpItem->m_aPrefix[2].m_aValue[1],
				_lpItem->getBitFieldValue(),
				_lpItem->m_year,_lpItem->m_month,_lpItem->m_day,_lpItem->m_hour,_lpItem->m_minute);
		}

		cSTRING	comment;

		if	(_bIsExportComment)
		{
			if	(lpItem->isBreedingRecordBook())
			{
				cPetDefine	petInfo;

				memcpy(&petInfo,&lpItem->m_bCount,sizeof(cPetDefine)-4);
				petInfo.m_dwExperience		=	lpItem->m_dwSerial;

				comment.Add("    ");
				if (strCommentHead[0])	
					comment.Add("%s",strCommentHead);
				comment.Add(">>>> ");

				char			strPetName[dNAME_LENGTH];

				memcpy(strPetName,petInfo.m_strName,dPET_NAME_LENGTH);
				strPetName[dPET_NAME_LENGTH]	=	NULL;

				comment.Add("[%s] [LV %d,EXP %d] job [%s]\n",strPetName,petInfo.m_bf10Level,petInfo.m_dwExperience,g_aJob[petInfo.m_bf10Job].m_strName);
			}
			else
			if	(lpItem->checkFirstEffect(eIE_MINIPET_POUCH))
			{
				cBlockLightPocket	*lpPocket	=	(cBlockLightPocket	*)_lpItem;

				comment.Add("    ");

				if	(strCommentHead[0])	
					comment.Add("%s",strCommentHead);
				comment.Add(">>>> ");

				comment.Add("[LV %d,EXP %d] [%s] Skill Point [%d] [%d/%d/%d %d:%d]\n",
						lpPocket->m_bf7Level,lpPocket->m_bf16Exp,gstrMiniPetName[lpPocket->m_bf5Type],lpPocket->m_bf7SkillPoint,
						lpPocket->m_bf5ExpireYear+2000,lpPocket->m_bf4ExpireMonth,lpPocket->m_bf5ExpireDay,lpPocket->m_bf5ExpireHour,lpPocket->m_bf6ExpireMinute);
			}
			else
			if	(_lpItem->m_aPrefix[0].m_wPrefix != 0xffff)
			{
				for (int i=0;i<dITEM_PREFIX_COUNT;i++)
				{
					char	*lpComment		=	((cITEM *)_lpItem)->getPrefixComment(i);
					if (!lpComment)
						break;

					cITEM_PREFIX *lpPrefix	=	&g_aItemPrefix[_lpItem->m_aPrefix[i].m_wPrefix];

					comment.Add("    ");
					if (strCommentHead[0])	
						comment.Add("%s",strCommentHead);
					comment.Add(">>>> ");

					comment.Add("%s  [%s]\n",lpComment,lpPrefix->m_str1stPrefix);
				}
			}

			if	(comment.String)
				strcpy(_lpstrComment,comment.String);
			else
				_lpstrComment[0]	=	NULL;

		}
		else
			_lpstrComment[0]		=	NULL;
	}
}


void
makeDelItemLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	cITEM			*lpItem	=	(cITEM *)&_lpLog->mLOG_ITEM_DEL.cItem;

	LOG_ITEM_DEL	*lpData	=	&_lpLog->mLOG_ITEM_DEL;

	switch(lpData->bf6Method)
	{
		case	eRI_BY_SEAL_MINIPET	:	//	길드 창고에 보관
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",
				lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_ITEM_BY_SEAL_MINIPET,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_UNSEAL_MINIPET	:	//	길드 창고에 보관
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_ITEM_BY_UNSEAL_MINIPET,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_USE_POLISHER	:	//	길드 창고에 보관
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_ITEM_BY_USE_POLISHER,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_STORE_GUILD	:	//	길드 창고에 보관
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_STORE_GUILD_INVEN,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_ADD_RELATED_PLACE	:	//	연고지 추가
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_ADD_RELATED_PLACE,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_FEED_TO_GUILD_PET	:	//	길드 펫에게 먹임
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_FEED_TO_GUILD_PET,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;

		case	eRI_DESTROY	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_DESTROY_ITEM_BY_PLAYER,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_QUEST	:
		case	eRI_BY_KARMA:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Q] [%s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_BY_QUEST,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName());
			break;

		case	eRI_RECOVER_DEATH_PENELTY	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%d %s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_RECOVER_DEATH_PENELTY_COST,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,lpItem->m_dwSerial,
				((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_UNSEAL_BREEDING_RECORD_BOOK	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_BREEDING_RECORD_BOOK_UNSEAL,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex);
			break;
		case	eRI_GUILD_BATTLE_ITEM_BY_NOT_GUILD_BATTLE_FIELD	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_DESTROY_ITEM_BY_GUILD_BATTLE_ITEM,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_REPAIR_ITEM_CAHRGE	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] -%s Gold",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REPAIR_ITEM_CHARGE,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName(),GetMoneyString(lpData->iCharge));
			break;
		case	eRI_REVISE_INCORRECT_PREFIX	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] ",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REVISE_INCORRECT_PREFIX,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_LOTTERY	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] ",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_LOTTERY_KWANG,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_MOVE_FIELD	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] ",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_ITEM_BY_MOVE_FIELD,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_USE_BOTTOMLESS_BOX	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] ",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_ITEM_BY_BOTTOMLESS_BOX,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_DISJOINTING_ITEM	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] ",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_DISJOINTING_ITEM,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;

		default	:
			sprintf(_lpstrReturnText,"errorLog!! in makeDelItemLog %d",lpData->bf6Method);
			break;
	}
}

void
makeDelItemFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment)
{
	LOG_ITEM_DEL	*lpData	=	&_lpLog->mLOG_ITEM_DEL;

	switch(lpData->bf6Method)
	{
		case	eRI_BY_SEAL_MINIPET	:	//	길드 창고에 보관
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",
				lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_ITEM_BY_SEAL_MINIPET,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_UNSEAL_MINIPET	:	//	길드 창고에 보관
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_ITEM_BY_UNSEAL_MINIPET,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_USE_POLISHER	:	//	길드 창고에 보관
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_ITEM_BY_USE_POLISHER,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_STORE_GUILD	:	//	길드 창고에 보관
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_STORE_GUILD_INVEN,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_ADD_RELATED_PLACE	:	//	연고지 추가
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_ADD_RELATED_PLACE,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_FEED_TO_GUILD_PET	:	//	길드 펫에게 먹임
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_FEED_TO_GUILD_PET,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;

		case	eRI_DESTROY	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_DESTROY_ITEM_BY_PLAYER,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_QUEST	:
		case	eRI_BY_KARMA:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Q] [%s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_BY_QUEST,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName());
			break;

		case	eRI_RECOVER_DEATH_PENELTY	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%d %s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_RECOVER_DEATH_PENELTY_COST,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,lpData->cItem.m_dwSerial,
				((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_UNSEAL_BREEDING_RECORD_BOOK	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_BREEDING_RECORD_BOOK_UNSEAL,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex);
			break;
		case	eRI_GUILD_BATTLE_ITEM_BY_NOT_GUILD_BATTLE_FIELD	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_DESTROY_ITEM_BY_GUILD_BATTLE_ITEM,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_REPAIR_ITEM_CAHRGE	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] -%s Gold",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REPAIR_ITEM_CHARGE,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName(),GetMoneyString(lpData->iCharge));
			break;
		case	eRI_REVISE_INCORRECT_PREFIX	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] ",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REVISE_INCORRECT_PREFIX,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_LOTTERY	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] ",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_LOTTERY_KWANG,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_MOVE_FIELD	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] ",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_ITEM_BY_MOVE_FIELD,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_USE_BOTTOMLESS_BOX	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] ",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_ITEM_BY_BOTTOMLESS_BOX,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		case	eRI_BY_DISJOINTING_ITEM	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] ",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_DISJOINTING_ITEM,lpData->strId,lpData->strName,lpData->strIP, 
				lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			break;
		default	:
			sprintf(_lpstrReturnText,"errorLog!! in makeDelItemFullLog %d",lpData->bf6Method);
			break;
	}

	exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpData->cItem);
}

void
makePetLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	LOG_PET_WORK	*lpData		=	(LOG_PET_WORK	*)_lpLog;
	char			*lpMsg		=	NULL;
	char			strPetName[dNAME_LENGTH];

	memcpy(strPetName,lpData->petInfo.m_strName,dPET_NAME_LENGTH);
	strPetName[dPET_NAME_LENGTH]	=	NULL;

	switch(lpData->bf6Work)
	{
		case	ePW_ADD		:	//	추가
			lpMsg	=	dMSG_TAME_PET;
			break;
		case	ePW_REMOVE	:	//	제거
			lpMsg	=	dMSG_REMOVE_PET;
			break;
		case	ePW_SEAL	:	//	봉인
			lpMsg	=	dMSG_SEAL_PET;
			break;
		case	ePW_UNSEAL	:	//	봉인 해제
			lpMsg	=	dMSG_UNSEAL_PET;
			break;
	}

	UINT	uiExp	=	lpData->petInfo.m_dwExperience;
	
	if	(uiExp & 0x80000000)
		uiExp	-=	0x80000000;

	sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] [%d/%d] [%s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
		lpMsg,lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
		strPetName,lpData->petInfo.m_bf10Level,uiExp,
		g_aJob[lpData->petInfo.m_bf10Job].m_strName);
}

void
makePetFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	LOG_PET_WORK	*lpData		=	(LOG_PET_WORK	*)_lpLog;
	char			*lpMsg		=	NULL;
	char			strPetName[dNAME_LENGTH];

	memcpy(strPetName,lpData->petInfo.m_strName,dPET_NAME_LENGTH);
	strPetName[dPET_NAME_LENGTH]	=	NULL;

	switch(lpData->bf6Work)
	{
		case	ePW_ADD		:	//	추가
			lpMsg	=	dMSG_TAME_PET;
			break;
		case	ePW_REMOVE	:	//	제거
			lpMsg	=	dMSG_REMOVE_PET;
			break;
		case	ePW_SEAL	:	//	봉인
			lpMsg	=	dMSG_SEAL_PET;
			break;
		case	ePW_UNSEAL	:	//	봉인 해제
			lpMsg	=	dMSG_UNSEAL_PET;
			break;
	}

	cBreedingRecordBookDefine	*lpBRBook	=	(cBreedingRecordBookDefine	*)&lpData->petInfo;
	cPetDefine					*lpPetInfo	=	&lpData->petInfo;

	UINT	uiExp	=	lpData->petInfo.m_dwExperience;
	UINT	uiSerial=	lpData->petInfo.m_dwExperience;
	
	if	(uiExp & 0x80000000)
	{
		uiExp	-=	0x80000000;
		uiSerial=	uiExp|0x80000000;
	}

	sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Name:%s] [LV %d,EXP %d] job [%s]\n    >>>> %s\n	>>>> %s\n\n",
		lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
		lpMsg,lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
		strPetName,lpPetInfo->m_bf10Level,uiExp,
		g_aJob[lpPetInfo->m_bf10Job].m_strName,lpPetInfo->getBookCode(g_iBreedingRecordBook,uiSerial),lpPetInfo->getPetCode());
}

void
makeSystemLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	switch(_lpLog->mLOG_SYSTEM.wWork)
	{
		case	eSLT_BEGIN_DUEL		:
		{
			LOG_S_BEGIN_DUEL	*lpData	=	(LOG_S_BEGIN_DUEL	*)&_lpLog->mLOG_SYSTEM;
			lpData->strIP[dIP_LENGTH-1]	=	NULL;
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -Begin Duel- Serial %d [%s vs %s] [%s] [%s/%s] [IP %s]",
				lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				lpData->iDuelSerial,lpData->astrTeamName[0],lpData->astrTeamName[1],lpData->astrTeamName[lpData->wTeam],lpData->strId,lpData->strName,lpData->strIP);
			break;
		}
		case	eSLT_RESULT_OF_DUEL	:
		{
			LOG_S_RESULT_OF_DUEL	*lpData	=	(LOG_S_RESULT_OF_DUEL	*)&_lpLog->mLOG_SYSTEM;
			lpData->strIP[dIP_LENGTH-1]	=	NULL;

			int		iWin			=	FALSE;
			char	*strWinText[]	=	{"lose","win"};
			
			if	(lpData->wWinTeam	==	lpData->wTeam)
				iWin	=	TRUE;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -Result of Duel- Serial %d [%s vs %s] [%s/%s] [IP %s] [%s]",
				lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				lpData->iDuelSerial,lpData->astrTeamName[0],lpData->astrTeamName[1],lpData->strId,lpData->strName,lpData->strIP,strWinText[iWin]);
			break;
		}
		case	eSLT_DISCONNECT_IN_DUEL_FIELD	:
		{
			LOG_S_DISCONNECT_IN_DUEL_FIELD	*lpData	=	(LOG_S_DISCONNECT_IN_DUEL_FIELD	*)&_lpLog->mLOG_SYSTEM;
			lpData->strIP[dIP_LENGTH-1]	=	NULL;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -Disconnect- Serial %d [%s vs %s] [%s/%s] [IP %s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				lpData->iDuelSerial,lpData->astrTeamName[0],lpData->astrTeamName[1],lpData->strId,lpData->strName,lpData->strIP);
			break;
		}

		case	eSLT_RECORD_GUILD_HONOR_POINT	:
		{
			LOG_S_RECORD_GUILD_HONOR_POINT	*lpData	=	(LOG_S_RECORD_GUILD_HONOR_POINT	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- guild %s point[%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_GUILD_HONOR_POINT,lpData->strGuildName,lpData->iHonorPoint);
			break;
		}

		case	eSLT_MINI_PET_FEED_ITEM	:		//	먹이 먹이기
		{
			LOG_S_MINI_PET_FEED_ITEM	*lpData	=	(LOG_S_MINI_PET_FEED_ITEM	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_FEED_TO_MINIPET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->item)->getName());
			break;
		}
		case	eSLT_TRANS_AVATAR_TO_GVG_SERVER	:
		{
			LOG_S_TRNAS_AVATAR	*lpData	=	(LOG_S_TRNAS_AVATAR	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_TRANS_AVATAR_TO_GVG,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				lpData->strNewAvatarName);
			break;
		}

		case	eSLT_BUY_ITEM	:
		{	// 토큰 상점 로그도 남긴다....!
			LOG_S_BUY_ITEM	*lpData	=	(LOG_S_BUY_ITEM	*)&_lpLog->mLOG_SYSTEM;
			LONGLONG		llPrice	=	lpData->dwBuyPrice*lpData->wCount;
		
			if	(lpData->cItem.m_wBaseItem	==	dITEM_INDEX_GOLD_BAR)
				llPrice	=	lpData->dwBuyPrice;

			BOOL	bTokenShop = FALSE;

			DWORD	dwPrice		=	(DWORD)llPrice;

			if(lpData->base.wSize == sizeof(LOG_S_BUY_ITEM))
			{
				if(lpData->wTokenItem != 0xffff)
				{
					bTokenShop = TRUE;
				}

			}
			char strWhere[64] = "";

			switch(lpData->wBuyMethod)
			{
			case eBUY_ITEM_IN_SHOP:
				strcpy(strWhere,dMSG_NORMAL_SHOP);
				break;
			case eBUY_ITEM_IN_PIGEON_SHOP:
				strcpy(strWhere,dMSG_PIGEON_SHOP);
				break;
			case eBUY_ITEM_IN_GUILD_SHOP:
				strcpy(strWhere,dMSG_GUILD_SHOP);
				break;
			case eBUY_ITEM_IN_BANK_GOLDBAR:
				strcpy(strWhere,dMSG_BANK_GOLDBAR);
				break;
			case eBUY_ITEM_IN_BADGE_OF_TRADER_SHOP:
				strcpy(strWhere,dMSG_BADGE_OF_TRADER_SHOP);
				break;
			}

			if(bTokenShop)
			{				
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s[%s/%s]- [%s/%s] [IP %s] FI[%d] [%s/%d] [%s %d %s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_BUY_ITEM,dMSG_TOKKEN_SHOP,strWhere,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName(),lpData->wCount,g_aBasicItem[lpData->wTokenItem].m_strName,dwPrice, dMSG_COUNT);
				//[091027 17:23:23] -구매[토큰상점]- [id/이름] [IP 22.22.2.2] FI[3] [아이템이름/5] [토큰아이템이름 5 개]
			}
			else
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s[%s/%s]- [%s/%s] [IP %s] FI[%d] [%s/%d] [%s Gold]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_BUY_ITEM,dMSG_NORMAL_SHOP,strWhere,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName(),lpData->wCount,GetMoneyString(dwPrice));	
				//[091027 17:23:23] -구매[일반상점]- [id/이름] [IP 22.22.2.2] FI[3] [아이템이름/5] [5억 gold]
			}

			break;
		}


		case	eSLT_CREATE_MINI_PET	:		//	미니펫 생성
		{
			LOG_S_CREATE_MINI_PET	*lpData	=	(LOG_S_CREATE_MINI_PET	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] Slot [%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_CREATE_MINI_PET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				gstrMiniPetName[lpData->miniPet.m_bf5Type],lpData->wSlot);
			break;
		}
		case	eSLT_REMOVE_MINI_PET	:		//	미니펫 제거
		{
			LOG_S_RELEASE_MINI_PET	*lpData	=	(LOG_S_RELEASE_MINI_PET	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s/%s] FI[%d] [%s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_MINI_PET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				gstrMiniPetName[lpData->miniPet.m_bf5Type]);
			break;
		}
		case	eSLT_EXTEND_MINI_PET_TIME	:		//	미니펫 제거
		{
			LOG_S_EXTEND_MINI_PET_TIME	*lpData	=	(LOG_S_EXTEND_MINI_PET_TIME	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] Time [30 Days]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_EXTEND_MINI_PET_TIME,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				gstrMiniPetName[lpData->miniPet.m_bf5Type]);
			break;
		}

		case	eSLT_REMOVE_A_PLACE_RELATED_OF_GUILD	:
		{
			LOG_S_REMOVE_A_PLACE_RELATED_OF_GUILD	*lpData	=	(LOG_S_REMOVE_A_PLACE_RELATED_OF_GUILD	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Guild %s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_A_PLACE_RELATED_OF_GUILD,lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,lpData->strGuildName);
			break;
		}
		case	eSLT_HACKING			:
		{
			LOG_S_HACKING_USER	*lpData	=	(LOG_S_HACKING_USER	*)&_lpLog->mLOG_SYSTEM;

			if	(lpData->wReason	==	ePPPTR_BY_XTRAP && lpData->strHackingProgramName[0] && lpData->strHackingProgramName[0] != 0xcc)
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] [XTrap %s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
					dMSG_USE_HACKING_TOOL,lpData->strId,lpData->strName,lpData->strIP,lpData->strHackingProgramName);
			}
			else
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
					dMSG_USE_HACKING_TOOL,lpData->strId,lpData->strName,lpData->strIP);
			}

			break;
		}
		case	eSLT_RECEIVE_QUEST_EXP	:
		{
			LOG_S_RECEIVE_QUEST_REWARD_EXP	*lpData	=	(LOG_S_RECEIVE_QUEST_REWARD_EXP	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_QUEST_REWARD_EXP,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex);
			break;
		}
		case	eSLT_COMPLETE_QUEST		:
		{
			LOG_S_COMPLETE_QUEST	*lpData	=	(LOG_S_COMPLETE_QUEST	*)&_lpLog->mLOG_SYSTEM;
			CQuestItem				*lpQeust=	CQuest::GetQuest(lpData->bf2Type,lpData->bf14QuestIndex);

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_COMPLETE_QUEST,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				lpQeust->m_strName);
			break;
		}

		case	eSLT_BACK_SERVER		:
		{
			LOG_S_BACK_SERVER	*lpData	=	(LOG_S_BACK_SERVER	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%d/%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_BACK_SERVER,lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				lpData->dwCurrentSaveCount,lpData->dwOldSaveCount);
			break;
		}

		case	eSLT_TREASURE_MAP_EVENT	:
		{
			LOG_S_TRESURE_MAP_EVENT	*lpData	=	(LOG_S_TRESURE_MAP_EVENT	*)&_lpLog->mLOG_SYSTEM;

			char	*lpstrEvent;
			
			switch(lpData->wEvent)
			{
				case	eTME_LOG_BOOST_EXP_IN_SERVER	:
					lpstrEvent	=	dMSG_BOOST_EXP_IN_SERVER;
					break;
				case	eTME_LOG_BOOST_GOLD_IN_SERVER	:
					lpstrEvent	=	dMSG_BOOST_GOLD_IN_SERVER;
					break;
				case	eTME_LOG_BOOST_PLAYER_EXP		:
					lpstrEvent	=	dMSG_BOOST_PLAYER_EXP;
					break;
				case	eTME_LOG_BOOST_PLAYER_GOLD		:
					lpstrEvent	=	dMSG_BOOST_PLAYER_GOLD;
					break;
			}

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				lpstrEvent,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex);
			break;
		}

		case	eSLT_OPERATOR_CHAT_LOG	:
		{
			LOG_S_OPERATOR_CHAT_LOG	*lpData	=	(LOG_S_OPERATOR_CHAT_LOG *)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_CHATTING,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex);
			break;
		}

		case	eSLT_SEAL_MINI_PET		:
		{
			LOG_S_SEAL_MINI_PET	*lpData	=	(LOG_S_SEAL_MINI_PET *)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_SEAL_MINIPET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex);
			break;
		}

		case	eSLT_UNSEAL_MINI_PET	:
		{
			LOG_S_UNSEAL_MINI_PET	*lpData	=	(LOG_S_UNSEAL_MINI_PET *)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_UNSEAL_MINIPET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex);
			break;
		}

		case	eSLT_REBIRTH			:
		{
			LOG_S_REBIRTH	*lpData	=	(LOG_S_REBIRTH *)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s-[%s/%s] [IP %s] FI[%d] [%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REBIRTH,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,lpData->wRebirthCount);
			break;
		}

		case	eSLT_MIX_MINIPET		:
		{
			LOG_S_RELEASE_MINI_PET	*lpData	=	(LOG_S_RELEASE_MINI_PET *)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_MIX_MINIPET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex);
			break;
		}

		case	eSLT_RESTRAINT_MINIPET	:
		{
			LOG_S_RESTRAINT_MINI_PET	*lpData	=	(LOG_S_RESTRAINT_MINI_PET *)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_RESTRAINT_MINIPET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex);
			break;
		}
		case	eSLT_DIVIDE_ITEM	:
		{
			LOG_S_DIVIDE_ITEM	*lpData	=	(LOG_S_DIVIDE_ITEM *)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -DI- [%s/%s] [IP %s] FI[%d] %d:%s [%d - %d/%d]",
				lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				lpData->cTargetItem.m_dwSerial,	((cITEM *)&lpData->cTargetItem)->getName(), lpData->wBeforDragCount,lpData->wAfterDragCount,lpData->wFocusCount);
			break;
		}
		default	:
			sprintf(_lpstrReturnText,"errorLog!! in makeSystemLog %d",_lpLog->mLOG_SYSTEM.wWork);
			break;
	}
}

enum
{
	eCT_TALK							,//	일반 말하기
	eCT_SAY								,//	귓말하기
	eCT_PARTY							,//	파티원 대화
	eCT_GUILD							,//	길드원 대화
	eCT_SHOUT							,//	외치기
	eCT_ADMIN							,//	관리자 전달 메세지
	eCT_RETURN_SAY						,//	내가 날린 세이가 돌아왔다.
	eCT_EVENT_SHOUT						,//	이벤트 샤우팅
	eCT_EVENT_CHAT						,//	이벤트 채팅
	eCT_EVENT_NOTICE_IN_CHAT_BOX		,// 이벤트로 채팅창에 공지
	eCT_EVENT_NOTICE_IN_SYSTEM_MESSAGE	,// 이벤트로 시스템 창에 공지
	eCT_EVENT_NOTICE_IN_NOTICE_BOX		,// 이벤트로 공지 창에 공지
	eCT_EVENT_OX_QUIZ					,// OX퀴즈 공지
	eCT_SYSTEM_MESSAGE					,// OX퀴즈 공지
	eCT_GET_PARTY_ITEM_MESSAGE			,// 파티 아이템 획득 메시지
};

void
makeSystemFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment,FILE *_lpFP)
{
	switch(_lpLog->mLOG_SYSTEM.wWork)
	{
		case	eSLT_BEGIN_DUEL		:
		{
			LOG_S_BEGIN_DUEL	*lpData	=	(LOG_S_BEGIN_DUEL	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -Begin Duel- Serial %d [%s vs %s] [%s/%s] [IP %s] Team %s\n",
				lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				lpData->iDuelSerial,lpData->astrTeamName[0],lpData->astrTeamName[1],lpData->strId,lpData->strName,lpData->strIP,lpData->astrTeamName[lpData->wTeam]);
			break;
		}
		case	eSLT_RESULT_OF_DUEL	:
		{
			LOG_S_RESULT_OF_DUEL	*lpData	=	(LOG_S_RESULT_OF_DUEL	*)&_lpLog->mLOG_SYSTEM;

			int		iWin			=	FALSE;
			char	*strWinText[]	=	{"lose","win"};

			if	(lpData->wWinTeam	==	lpData->wTeam)
				iWin	=	TRUE;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -Result of Duel- Serial %d [%s vs %s] [%s/%s] [IP %s]\n    result %s,[Get VP %d],[Get DP %d],[VP %d],[DP %d],[Record:Battle Count %d/Win %d/Lose %d/Draw %d]\n",
				lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				lpData->iDuelSerial,lpData->astrTeamName[0],lpData->astrTeamName[1],lpData->strId,lpData->strName,lpData->strIP,strWinText[iWin],lpData->iGetVP,lpData->iGetDP,lpData->iVP,lpData->iDP,lpData->iBattle,lpData->iWin,lpData->iDefeat,lpData->iDraw);
			break;
		}
		case	eSLT_DISCONNECT_IN_DUEL_FIELD	:
		{
			LOG_S_DISCONNECT_IN_DUEL_FIELD	*lpData	=	(LOG_S_DISCONNECT_IN_DUEL_FIELD	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -Disconnect- Serial %d [%s vs %s] [%s/%s] [IP %s]\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				lpData->astrTeamName[0],lpData->astrTeamName[1],lpData->strId,lpData->strName,lpData->strIP);
			break;
		}

		case	eSLT_RECORD_GUILD_HONOR_POINT	:
		{
			LOG_S_RECORD_GUILD_HONOR_POINT	*lpData	=	(LOG_S_RECORD_GUILD_HONOR_POINT	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- guild %s point[%d]\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_GUILD_HONOR_POINT,lpData->strGuildName,lpData->iHonorPoint);
			break;
		}

		case	eSLT_MINI_PET_FEED_ITEM	:		//	먹이 먹이기
		{
			LOG_S_MINI_PET_FEED_ITEM	*lpData	=	(LOG_S_MINI_PET_FEED_ITEM	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s/%s] FI[%d] [%s] %s Slot [%d] %s\n",
				lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_FEED_TO_MINIPET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->item)->getName(),gstrMiniPetName[lpData->miniPet.m_bf5Type],lpData->wSlot,GetMiniPetCode(&lpData->miniPet));

			exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpData->item);

			break;
		}

		case	eSLT_TRANS_AVATAR_TO_GVG_SERVER	:
		{
			LOG_S_TRNAS_AVATAR	*lpData	=	(LOG_S_TRNAS_AVATAR	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_TRANS_AVATAR_TO_GVG,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				lpData->strNewAvatarName);
			break;
		}

		case	eSLT_BUY_ITEM	:
		{	// 토큰 상점도 로그를 남긴다.
			LOG_S_BUY_ITEM	*lpData	=	(LOG_S_BUY_ITEM	*)&_lpLog->mLOG_SYSTEM;

			BOOL	bTokenShop = FALSE;
			
			if(lpData->base.wSize == sizeof(LOG_S_BUY_ITEM))
			{
				if(lpData->wTokenItem != 0xffff)
				{
					bTokenShop = TRUE;
				}
				
			}
	
			char strShop[64] = "";
			strcpy(strShop, dMSG_NORMAL_SHOP);
			if(bTokenShop)
			{				
				strcpy(strShop, dMSG_TOKKEN_SHOP);
			}
			char strWhere[64] = "";

			switch(lpData->wBuyMethod)
			{
			case eBUY_ITEM_IN_SHOP:
				strcpy(strWhere,dMSG_NORMAL_SHOP);
				break;
			case eBUY_ITEM_IN_PIGEON_SHOP:
				strcpy(strWhere,dMSG_PIGEON_SHOP);
				break;
			case eBUY_ITEM_IN_GUILD_SHOP:
				strcpy(strWhere,dMSG_GUILD_SHOP);
				break;
			case eBUY_ITEM_IN_BANK_GOLDBAR:
				strcpy(strWhere,dMSG_BANK_GOLDBAR);
				break;
			case eBUY_ITEM_IN_BADGE_OF_TRADER_SHOP:
				strcpy(strWhere,dMSG_BADGE_OF_TRADER_SHOP);
				break;
			}

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s[%s/%s]- [%s:%s/%s] FI[%d] [%s x %d]",
				lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_BUY_ITEM,strShop,strWhere,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				((cITEM *)&lpData->cItem)->getName(),lpData->wCount);

			exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpData->cItem);

			fprintf(_lpFP,_lpstrReturnText);

			if	(_lpstrComment[0])
			{
				fprintf(_lpFP,_lpstrComment);
				_lpstrComment[0]	=	NULL;
			}

			LONGLONG	llPrice	=	lpData->dwBuyPrice*lpData->wCount;

			if	(lpData->cItem.m_wBaseItem	==	dITEM_INDEX_GOLD_BAR)
				llPrice	=	lpData->dwBuyPrice;

			DWORD	dwPrice	=	(DWORD)llPrice;

			if(bTokenShop)
			{
				sprintf(_lpstrReturnText,"    %s %d %s\n",g_aBasicItem[lpData->wTokenItem].m_strName, dwPrice, dMSG_COUNT);
			}
			else
			{
				sprintf(_lpstrReturnText,"    %s Gold\n",GetMoneyString(dwPrice));
			}
			
			fprintf(_lpFP,_lpstrReturnText);
			strcpy(_lpstrReturnText,"\n");

			break;
		}

		case	eSLT_CREATE_MINI_PET	:		//	미니펫 생성
		{
			LOG_S_CREATE_MINI_PET	*lpData	=	(LOG_S_CREATE_MINI_PET	*)&_lpLog->mLOG_SYSTEM;

			char	*lpstrItemName	=	((cITEM *)&lpData->cItem)->getName();

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s/%s] FI[%d] %s Slot [%d] %s\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_CREATE_MINI_PET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				gstrMiniPetName[lpData->miniPet.m_bf5Type],lpData->wSlot,GetMiniPetCode(&lpData->miniPet));
			break;
		}
		case	eSLT_REMOVE_MINI_PET	:		//	미니펫 제거
		{
			LOG_S_RELEASE_MINI_PET	*lpData	=	(LOG_S_RELEASE_MINI_PET	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s/%s] FI[%d] %s %s\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_MINI_PET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				gstrMiniPetName[lpData->miniPet.m_bf5Type],GetMiniPetCode(&lpData->miniPet));
			break;
		}
		case	eSLT_EXTEND_MINI_PET_TIME	:		//	미니펫 제거
		{
			LOG_S_EXTEND_MINI_PET_TIME	*lpData	=	(LOG_S_EXTEND_MINI_PET_TIME	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s/%s] FI[%d] %s Time [30 Days] %s\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_EXTEND_MINI_PET_TIME,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				gstrMiniPetName[lpData->miniPet.m_bf5Type],GetMiniPetCode(&lpData->miniPet));
			break;
		}

		case	eSLT_REMOVE_A_PLACE_RELATED_OF_GUILD	:
		{
			LOG_S_REMOVE_A_PLACE_RELATED_OF_GUILD	*lpData	=	(LOG_S_REMOVE_A_PLACE_RELATED_OF_GUILD	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s/%s] FI[%d] Guild [%s] Target Field [%d]\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_REMOVE_A_PLACE_RELATED_OF_GUILD,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				lpData->strGuildName,lpData->wField);
			break;
		}
		case	eSLT_HACKING			:
		{
			LOG_S_HACKING_USER	*lpData	=	(LOG_S_HACKING_USER	*)&_lpLog->mLOG_SYSTEM;

			lpData->strHackingProgramName[31]	=	NULL;

			if	(lpData->wReason	==	ePPPTR_BY_XTRAP)
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] Penelty Time %d [XTrap %s]\n",
					lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
					dMSG_USE_HACKING_TOOL,
					lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
					lpData->wPeneltyTime,lpData->strHackingProgramName);
			}
			else
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] Penelty Time %d [%d]\n",
					lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
					dMSG_USE_HACKING_TOOL,
					lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
					lpData->wPeneltyTime,lpData->wReason);//,lpData->strHackingProgramName);
			}
			break;
		}
		case	eSLT_RECEIVE_QUEST_EXP	:
		{
			LOG_S_RECEIVE_QUEST_REWARD_EXP	*lpData	=	(LOG_S_RECEIVE_QUEST_REWARD_EXP	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] Quest Reward [%d],Real Receive Exp [%d]\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_QUEST_REWARD_EXP,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				lpData->iRewardExp,lpData->iRealReceiveExp);
			break;
		}
		case	eSLT_COMPLETE_QUEST		:
		{
			LOG_S_COMPLETE_QUEST	*lpData	=	(LOG_S_COMPLETE_QUEST	*)&_lpLog->mLOG_SYSTEM;

			CQuestItem				*lpQeust=	CQuest::GetQuest(lpData->bf2Type,lpData->bf14QuestIndex);

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] Quest Name [%s]\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_COMPLETE_QUEST,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				lpQeust->m_strName);
			break;
		}

		case	eSLT_BACK_SERVER		:
		{
			LOG_S_BACK_SERVER	*lpData	=	(LOG_S_BACK_SERVER	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%d/%d]\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_BACK_SERVER,lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				lpData->dwCurrentSaveCount,lpData->dwOldSaveCount);
			break;
		}

		case	eSLT_TREASURE_MAP_EVENT	:
		{
			LOG_S_TRESURE_MAP_EVENT	*lpData	=	(LOG_S_TRESURE_MAP_EVENT	*)&_lpLog->mLOG_SYSTEM;

			char	*lpstrEvent;
			
			switch(lpData->wEvent)
			{
				case	eTME_LOG_BOOST_EXP_IN_SERVER	:
					lpstrEvent	=	dMSG_BOOST_EXP_IN_SERVER;
					break;
				case	eTME_LOG_BOOST_GOLD_IN_SERVER	:
					lpstrEvent	=	dMSG_BOOST_GOLD_IN_SERVER;
					break;
				case	eTME_LOG_BOOST_PLAYER_EXP		:
					lpstrEvent	=	dMSG_BOOST_PLAYER_EXP;
					break;
				case	eTME_LOG_BOOST_PLAYER_GOLD		:
					lpstrEvent	=	dMSG_BOOST_PLAYER_GOLD;
					break;
			}

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d]\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				lpstrEvent,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex);
			break;
		}

		case	eSLT_OPERATOR_CHAT_LOG	:
		{
			LOG_S_OPERATOR_CHAT_LOG	*lpData	=	(LOG_S_OPERATOR_CHAT_LOG *)&_lpLog->mLOG_SYSTEM;

			char	strTalker[dNAME_LENGTH],strListener[dNAME_LENGTH];

			if	(lpData->bf1IsListen)
			{
				memcpy(strTalker,lpData->strTarget,dNAME_LENGTH-2);
				strTalker[dNAME_LENGTH-2]	=	NULL;

				memcpy(strListener,lpData->strName,dNAME_LENGTH-2);
				strListener[dNAME_LENGTH-2]	=	NULL;
			}
			else
			{
				memcpy(strTalker,lpData->strName,dNAME_LENGTH-2);
				strTalker[dNAME_LENGTH-2]	=	NULL;

				memcpy(strListener,lpData->strTarget,dNAME_LENGTH-2);
				strListener[dNAME_LENGTH-2]	=	NULL;
			}

			char	*lpstrChatType	=	dMSG_TALK;

			switch(lpData->bf10ChatMethod)
			{
				case	eCT_TALK		://	일반 말하기
					lpstrChatType	=	dMSG_TALK;
					break;
				case	eCT_SAY			://	귓말하기
					lpstrChatType	=	dMSG_SAY;
					break;
				case	eCT_PARTY		://	파티원 대화
					lpstrChatType	=	dMSG_PARTY;
					break;
				case	eCT_GUILD		://	길드원 대화
					lpstrChatType	=	dMSG_GUILD;
					break;
				case	eCT_SHOUT		://	외치기
					lpstrChatType	=	dMSG_SHOUT;
					break;
				case	eCT_ADMIN		://	관리자 전달 메세지
					lpstrChatType	=	dMSG_ADMIN;
					break;
			}

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d]\n    [%s->%s] %s\n\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				lpstrChatType,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				strTalker,strListener,lpData->strText);

			break;
		}

		case	eSLT_SEAL_MINI_PET		:
		{
			LOG_S_SEAL_MINI_PET	*lpData	=	(LOG_S_SEAL_MINI_PET	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s Slot [%d] %s\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_SEAL_MINIPET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				gstrMiniPetName[lpData->miniPet.m_bf5Type],lpData->wSlot,GetMiniPetCode(&lpData->miniPet));
			break;
		}

		case	eSLT_UNSEAL_MINI_PET	:
		{
			LOG_S_UNSEAL_MINI_PET	*lpData	=	(LOG_S_UNSEAL_MINI_PET	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s Slot [%d] %s\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_UNSEAL_MINIPET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				gstrMiniPetName[lpData->miniPet.m_bf5Type],lpData->wSlot,GetMiniPetCode(&lpData->miniPet));
			break;
		}

		case	eSLT_REBIRTH			:
		{
			LOG_S_REBIRTH	*lpData	=	(LOG_S_REBIRTH *)&_lpLog->mLOG_SYSTEM;

 			if	(_lpLog->mLOG_SYSTEM.base.wSize	==	sizeof(LOG_S_REBIRTH) && lpData->wPreLevel >= 600)
			{
				BOOL	bIsReviseLog	=	FALSE;

				if	(lpData->wRebirthCount	>=	0x8000)
				{
					lpData->wRebirthCount	-=	0x8000;
					bIsReviseLog			=	TRUE;
				}

				if	(bIsReviseLog)
				{
					WORD	wYM			=	lpData->wLevel;
					wYM					>>=	8;

					int		iYear		=	(wYM>>4)+2000;
					int		iMonth		=	wYM&0x000f;
					int		iDay		=	lpData->wLevel&0x00ff;

					int		iLogDay		=	lpData->logTime.yy*365+lpData->logTime.mm*31+lpData->logTime.dd;
					int		iMapExpDay	=	(iYear-2000)*365+iMonth*31+iDay;

					if	(abs(iLogDay-iMapExpDay) > 2)
						sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Rebirth count %d] [Rebirth Level %d:%d->%d:%d] [Ref save log time %d/%d/%d]\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
							dMSG_REBIRTH,
							lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
							lpData->wRebirthCount,lpData->wPreLevel,lpData->iPreExp,1,0,
							iYear,iMonth,iDay);
					else
						sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Rebirth count %d] [Rebirth Level %d:%d->%d:%d] [Ref save log time %d/%d/%d]\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
							dMSG_REBIRTH,
							lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
							lpData->wRebirthCount,lpData->wPreLevel,lpData->iPreExp,1,0,
							iYear,iMonth,iDay);
				}
				else
					sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Rebirth count %d] [Rebirth Level %d:%d->%d:%d]\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
						dMSG_REBIRTH,
						lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
						lpData->wRebirthCount,lpData->wPreLevel,lpData->iPreExp,lpData->wLevel,lpData->iExp);
			}
			else
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Rebirth count %d] Error!!!!!!!!!\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
					dMSG_REBIRTH,
					lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
					lpData->wRebirthCount);
			break;
		}

		case	eSLT_MIX_MINIPET		:
		{
			LOG_S_RELEASE_MINI_PET	*lpData	=	(LOG_S_RELEASE_MINI_PET	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s %s\n",
				lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_MIX_MINIPET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				gstrMiniPetName[lpData->miniPet.m_bf5Type],GetMiniPetCode(&lpData->miniPet));
			break;
		}

		case	eSLT_RESTRAINT_MINIPET	:
		{
			LOG_S_RESTRAINT_MINI_PET	*lpData	=	(LOG_S_RESTRAINT_MINI_PET	*)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s %s\n\n    Fire %d,Water %d,Wind %d,Earth %d,Light %d,Dark %d\n",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_RESTRAINT_MINIPET,
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				gstrMiniPetName[lpData->miniPet.m_bf5Type],GetMiniPetCode(&lpData->miniPet),
				lpData->m_wFireCount,lpData->m_wWaterCount,lpData->m_wWindCount,lpData->m_wEarthCount,lpData->m_wLightCount,lpData->m_wDarkCount);
			break;
		}
		case	eSLT_DIVIDE_ITEM			:
		{
			LOG_S_DIVIDE_ITEM	*lpData	=	(LOG_S_DIVIDE_ITEM *)&_lpLog->mLOG_SYSTEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -DI- [%s/%s] [IP %s] FI[%d] %d:%s [%d - %d/%d]\n",
				lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				lpData->cTargetItem.m_dwSerial,	((cITEM *)&lpData->cTargetItem)->getName(), lpData->wBeforDragCount,lpData->wAfterDragCount,lpData->wFocusCount);
			break;
		}
		default	:
			sprintf(_lpstrReturnText,"errorLog!! in makeSystemFullLog %d\n",_lpLog->mLOG_SYSTEM.wWork);
			break;
	}
}

void
makeEnchantItemFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	BOOL	bIsEnchantResult	=	FALSE;
	char	strWork[128];
	memset(strWork,0,128);

	switch(_lpLog->mLOG_ENCHANT_ITEM.wWork)
	{
		case	eEIW_RENEW_TERM	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s + %d.%.2d Hour",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				dMSG_RWNEW_ITEM_TERM,
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
				_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm/60,_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm&60);
			return;

		case eEIW_SUCCESS:
			bIsEnchantResult	=	TRUE;
			strcpy(strWork,dMSG_SUCCESS);
			break;
		case eEIW_BROKEN:
			bIsEnchantResult	=	TRUE;
			strcpy(strWork,dMSG_BROKEN2);
			break;
		case eEIW_DESTROY:
			bIsEnchantResult	=	TRUE;
			strcpy(strWork,dMSG_DESTROY);
			break;

		case eEIW_REPAIR_BROKEN:
			strcpy(strWork,dMSG_REPAIR_BROKEN);
			break;
		case eEIW_REPAIR_CURSE:
			strcpy(strWork,dMSG_REPAIR_CURSE);
			break;
		case eEIW_REPAIR_CAN_NOT_ATTACH_PREFIX_STATUS:
			strcpy(strWork,dMSG_REPAIR_CAN_NOT_STICK_PREFIX);
			break;
		case eEIW_REPAIR_AND_REMOVE_PREFIX:
			strcpy(strWork,dMSG_REPAIR_BUT_REMOVE_PREFIX);
			break;

		case eEIW_REPAIR_DX_ITEM:			//	DX 아이템 수리
			strcpy(strWork,dMSG_EIW_REPAIR_DX_ITEM);
			break;
		case eEIW_REMOVE_PREFIX:				//	접두사 제거
			strcpy(strWork,dMSG_EIW_REMOVE_PREFIX);
			break;
		case eEIW_MERGE_ITEM:				//	아이템 합체
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s [%s Gold]",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				dMSG_EIW_MERGE_ITEM,
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
				_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),strWork,GetMoneyString(_lpLog->mLOG_ENCHANT_ITEM.dwPrice));
			return;
		case eEIW_JUDGE_ITEM:				//	아이템 판별
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s [%s Gold]",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,
				dMSG_EIW_JUDGE_ITEM,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
				_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),strWork,GetMoneyString(_lpLog->mLOG_ENCHANT_ITEM.dwPrice));
			return;
		case eEIW_UPGRADE_ETERNAL_ITEM : //09.09.04
			{
			LOG_UPGRADE_ETERNAL_ITEM* lpLog = (LOG_UPGRADE_ETERNAL_ITEM*) &_lpLog->mLOG_ENCHANT_ITEM;
		
			
			int iUpgradeNormalItem	= lpLog->dwGold;
			int iCrystalCount		= lpLog->wCrystalCount;
			char strUpgradeMaterialsNormalItemName[64]	= ""; 		// 일반보조제 이름
			char strUpgradeMaterialsItemName[64]		= "";		// 프리미엄 보조제이름
			char strSuccess[16] = "";
			strcpy(strUpgradeMaterialsNormalItemName,dMSG_NOT_USE);
			strcpy(strUpgradeMaterialsItemName,dMSG_NOT_USE);
			strcpy(strSuccess,dMSG_UPGRADE_SUCCESS);

			if( g_aBasicItem[lpLog->cTargetItem.m_wBaseItem].m_bf4EternalItemCurrentLevel  >= g_aBasicItem[lpLog->wUpgradeItemSerial].m_bf4EternalItemCurrentLevel)
				strcpy(strSuccess, dMSG_UPGRADE_FAILED);
		
			if(lpLog->wUpgradeMaterialsItemSerial != 0xffff)	
				strcpy(strUpgradeMaterialsNormalItemName, g_aBasicItem[lpLog->wUpgradeMaterialsItemSerial].m_strName);
			
			if(lpLog->wUpgradePremiumMaterialsItemSerial != 0xffff)
				strcpy(strUpgradeMaterialsItemName, g_aBasicItem[lpLog->wUpgradePremiumMaterialsItemSerial].m_strName);

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] %s%s%s [%s] gold : %d / %s : %d / %s : %d / %s : %d [%s] %s : %s / %s : %s",
				lpLog->logTime.yy, lpLog->logTime.mm, lpLog->logTime.dd, lpLog->logTime.hour, lpLog->logTime.min, lpLog->logTime.getSecond(),
				dMSG_UPGRADE_ITEM, lpLog->strId, lpLog->strName, lpLog->strIP, lpLog->wFieldUniqueIndex,
				strSuccess, ((cITEM *)& lpLog->cTargetItem)->getName(), dMSG_RIGHT_ARROW, g_aBasicItem[lpLog->wUpgradeItemSerial].m_strName,
				dMSG_MATERIALS, lpLog->dwGold, dMSG_STONE_OF_MESTERY, lpLog->wStoneOfMesteryCount, dMSG_TANTILLESS, lpLog->wTantillessCount, dMSG_CRYSTAL, lpLog->wCrystalCount,
				dMSG_UPGRADE_MATERIALS_ITEM, dMSG_NORMAL,strUpgradeMaterialsNormalItemName, dMSG_PREMIUM , strUpgradeMaterialsItemName);

//예 : [090904 11:11:50] - 아이템 강화 - [zclunt2/짐승남:192.168.0.1] FI[550] [성공] 이터널소드 I → 이터널소드 II [재료] gold : 500만 
// 신비석 : 1 / 탈틸리스유물 : 2/ 결정석 : 2  [보조제] 일반 : 미사용 / 프리미엄 : 짱강화제
			return;
			}

		case eEIW_SUCCESS_COPY_PREFIX : 
			{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s + %d.%.2d Hour",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				dMSG_SUCCESS_COPY_PREFIX,
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
				_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm/60,_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm&60);
				
				return;
			}
		case eEIW_FAILED_COPY_PREFIX : 
			{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s + %d.%.2d Hour",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				dMSG_FAILED_COPY_PREFIX,
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
				_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm/60,_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm&60);
				
				return;
			}
		case eEIW_SUCCESS_ATTACH_PREFIX : 
			{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s + %d.%.2d Hour",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				dMSG_SUCCESS_ATTACH_PREFIX,
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
				_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm/60,_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm&60);
				
				return;
			}
		case eEIW_FAILED_ATTACH_PREFIX : 
			{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s + %d.%.2d Hour",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				dMSG_FAILED_ATTACH_PREFIX,
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
				_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm/60,_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm&60);
				
				return;
			}

		case eEIW_SUCCESS_INCREASE_CARVING_LEVEL : 
			{// 각인 레벨증가 성공
			LOG_CARVE_ITEM* lpLog = (LOG_CARVE_ITEM*) &_lpLog->mLOG_ENCHANT_ITEM;
		
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s Lv %d%s%d] [%s] %s/gold : %d / %s : %d",
				lpLog->logTime.yy, lpLog->logTime.mm, lpLog->logTime.dd, lpLog->logTime.hour, lpLog->logTime.min, lpLog->logTime.getSecond(),
				dMSG_SUCCESS_INCREASE_CARVING_LEVEL, lpLog->strId, lpLog->strName, lpLog->strIP, lpLog->wFieldUniqueIndex,
				 ((cITEM *)& lpLog->cTargetItem)->getName(),lpLog->bf8BeforCarvingLevel ,dMSG_RIGHT_ARROW,lpLog->bf8AfterCarvingLevel,
				dMSG_MATERIALS, g_aBasicItem[lpLog->wUseItem].m_strName, lpLog->dwGold, dMSG_CRYSTAL, lpLog->bf5CrystalCount);
//예 : [090904 11:11:50] - 아이템 강화 - [zclunt2/짐승남:192.168.0.1] FI[550] [무기 Lv 1 →  2] [재료] 주문서 / gold : 500만 /결정석 : 2 
  			
				return;
			}
		case eEIW_FAILED_INCREASE_CARVING_LEVEL : 
			{// 각인 레벨증가 실패
			LOG_CARVE_ITEM* lpLog = (LOG_CARVE_ITEM*) &_lpLog->mLOG_ENCHANT_ITEM;
		
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s Lv %d%s%d] [%s] %s/gold : %d / %s : %d",
				lpLog->logTime.yy, lpLog->logTime.mm, lpLog->logTime.dd, lpLog->logTime.hour, lpLog->logTime.min, lpLog->logTime.getSecond(),
				dMSG_FAILED_INCREASE_CARVING_LEVEL, lpLog->strId, lpLog->strName, lpLog->strIP, lpLog->wFieldUniqueIndex,
				 ((cITEM *)& lpLog->cTargetItem)->getName(),lpLog->bf8BeforCarvingLevel ,dMSG_RIGHT_ARROW,lpLog->bf8AfterCarvingLevel,
				dMSG_MATERIALS, g_aBasicItem[lpLog->wUseItem].m_strName, lpLog->dwGold, dMSG_CRYSTAL, lpLog->bf5CrystalCount);
//예 : [090904 11:11:50] - 아이템 강화 - [zclunt2/짐승남:192.168.0.1] FI[550] [무기 Lv 1 →  2] [재료] 주문서 / gold : 500만 /결정석 : 2 
				return;
			}
		case eEIW_SUCCESS_SET_CARVING_LEVEL : 
			{// 각인 레벨설정 성공
			LOG_CARVE_ITEM* lpLog = (LOG_CARVE_ITEM*) &_lpLog->mLOG_ENCHANT_ITEM;
		
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s Lv %d%s%d] [%s] %s/gold : %d / %s : %d",
				lpLog->logTime.yy, lpLog->logTime.mm, lpLog->logTime.dd, lpLog->logTime.hour, lpLog->logTime.min, lpLog->logTime.getSecond(),
				dMSG_SUCCESS_SET_CARVING_LEVEL, lpLog->strId, lpLog->strName, lpLog->strIP, lpLog->wFieldUniqueIndex,
				 ((cITEM *)& lpLog->cTargetItem)->getName(),lpLog->bf8BeforCarvingLevel ,dMSG_RIGHT_ARROW,lpLog->bf8AfterCarvingLevel,
				dMSG_MATERIALS, g_aBasicItem[lpLog->wUseItem].m_strName, lpLog->dwGold, dMSG_CRYSTAL, lpLog->bf5CrystalCount);
//예 : [090904 11:11:50] - 아이템 강화 - [zclunt2/짐승남:192.168.0.1] FI[550] [무기 Lv 1 →  2] [재료] 주문서 / gold : 500만 /결정석 : 2 
				return;
			}
		case eEIW_FAILED_SET_CARVING_LEVEL : 
			{// 각인 레벨설정 실패
			LOG_CARVE_ITEM* lpLog = (LOG_CARVE_ITEM*) &_lpLog->mLOG_ENCHANT_ITEM;
		
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s Lv %d%s%d] [%s] %s/gold : %d / %s : %d",
				lpLog->logTime.yy, lpLog->logTime.mm, lpLog->logTime.dd, lpLog->logTime.hour, lpLog->logTime.min, lpLog->logTime.getSecond(),
				dMSG_FAILED_SET_CARVING_LEVEL, lpLog->strId, lpLog->strName, lpLog->strIP, lpLog->wFieldUniqueIndex,
				 ((cITEM *)& lpLog->cTargetItem)->getName(),lpLog->bf8BeforCarvingLevel ,dMSG_RIGHT_ARROW,lpLog->bf8AfterCarvingLevel,
				dMSG_MATERIALS, g_aBasicItem[lpLog->wUseItem].m_strName, lpLog->dwGold, dMSG_CRYSTAL, lpLog->bf5CrystalCount);
//예 : [090904 11:11:50] - 아이템 강화 - [zclunt2/짐승남:192.168.0.1] FI[550] [무기 Lv 1 →  2] [재료] 주문서 / gold : 500만 /결정석 : 2 
				return;
			}
		default	:
			strcpy(_lpstrReturnText,"errorLog!! in makeEnchantItemFullLog");
			return;
	}

	if	(bIsEnchantResult)
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s %s- [%s/%s] [IP %s] FI[%d] %s",
			_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,
			_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
			dMSG_ENCHANT,strWork,
			_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
			_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName());
	else
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",
			_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,
			_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
			strWork,
			_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
			_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName());
}

void
makeEnchantItemLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	BOOL	bIsEnchantResult	=	FALSE;

	char	strWork[128];

	memset(strWork,0,128);

	switch(_lpLog->mLOG_ENCHANT_ITEM.wWork)
	{
		case	eEIW_RENEW_TERM	:
			strcpy(strWork,dMSG_RWNEW_ITEM_TERM);
			return;

		case	eEIW_SUCCESS:
			bIsEnchantResult	=	TRUE;
			strcpy(strWork,dMSG_SUCCESS);
			break;
		case eEIW_BROKEN:
			bIsEnchantResult	=	TRUE;
			strcpy(strWork,dMSG_BROKEN2);
			break;
		case eEIW_DESTROY:
			bIsEnchantResult	=	TRUE;
			strcpy(strWork,dMSG_DESTROY);
			break;

		case eEIW_REPAIR_BROKEN:
			strcpy(strWork,dMSG_REPAIR_BROKEN);
			break;

		case eEIW_REPAIR_CURSE:
			strcpy(strWork,dMSG_REPAIR_CURSE);
			break;
		case eEIW_REPAIR_CAN_NOT_ATTACH_PREFIX_STATUS:
			strcpy(strWork,dMSG_REPAIR_CAN_NOT_STICK_PREFIX);
			break;
		case eEIW_REPAIR_AND_REMOVE_PREFIX:
			strcpy(strWork,dMSG_REPAIR_BUT_REMOVE_PREFIX);
			break;

		case eEIW_REPAIR_DX_ITEM:			//	DX 아이템 수리
			strcpy(strWork,dMSG_EIW_REPAIR_DX_ITEM);
			break;
		case eEIW_REMOVE_PREFIX:				//	접두사 제거
			strcpy(strWork,dMSG_EIW_REMOVE_PREFIX);
			break;
		case eEIW_MERGE_ITEM:				//	아이템 합체
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] %s [%s Gold]",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				dMSG_EIW_MERGE_ITEM,
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP,_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,
				((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),strWork,GetMoneyString(_lpLog->mLOG_ENCHANT_ITEM.dwPrice));
			return;
		case eEIW_JUDGE_ITEM:				//	아이템 판별
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] %s [%s Gold]",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				dMSG_EIW_JUDGE_ITEM,
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP,_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,
				((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),strWork,GetMoneyString(_lpLog->mLOG_ENCHANT_ITEM.dwPrice));
			return;
		case eEIW_UPGRADE_ETERNAL_ITEM : //09.09.04
			{
			LOG_UPGRADE_ETERNAL_ITEM* lpLog = (LOG_UPGRADE_ETERNAL_ITEM*) &_lpLog->mLOG_ENCHANT_ITEM;
		
			
			int iUpgradeNormalItem	= lpLog->dwGold;
			int iCrystalCount		= lpLog->wCrystalCount;
			char strUpgradeMaterialsNormalItemName[64]	= ""; 		// 일반보조제 이름
			char strUpgradeMaterialsItemName[64]		= "";		// 프리미엄 보조제이름
			char strSuccess[16] = "";
			strcpy(strUpgradeMaterialsNormalItemName,dMSG_NOT_USE);
			strcpy(strUpgradeMaterialsItemName,dMSG_NOT_USE);
			strcpy(strSuccess,dMSG_UPGRADE_SUCCESS);

			if( g_aBasicItem[lpLog->cTargetItem.m_wBaseItem].m_bf4EternalItemCurrentLevel  >= g_aBasicItem[lpLog->wUpgradeItemSerial].m_bf4EternalItemCurrentLevel)
				strcpy(strSuccess, dMSG_UPGRADE_FAILED);
		
			if(lpLog->wUpgradeMaterialsItemSerial != 0xffff)	
				strcpy(strUpgradeMaterialsNormalItemName, g_aBasicItem[lpLog->wUpgradeMaterialsItemSerial].m_strName);
			
			if(lpLog->wUpgradePremiumMaterialsItemSerial != 0xffff)
				strcpy(strUpgradeMaterialsItemName, g_aBasicItem[lpLog->wUpgradePremiumMaterialsItemSerial].m_strName);

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] %s%s%s [%s] gold : %d / %s : %d / %s : %d / %s : %d [%s] %s : %s / %s : %s",
				lpLog->logTime.yy, lpLog->logTime.mm, lpLog->logTime.dd, lpLog->logTime.hour, lpLog->logTime.min, lpLog->logTime.getSecond(),
				dMSG_UPGRADE_ITEM, lpLog->strId, lpLog->strName, lpLog->strIP, lpLog->wFieldUniqueIndex,
				strSuccess, ((cITEM *)& lpLog->cTargetItem)->getName(), dMSG_RIGHT_ARROW, g_aBasicItem[lpLog->wUpgradeItemSerial].m_strName,
				dMSG_MATERIALS, lpLog->dwGold, dMSG_STONE_OF_MESTERY, lpLog->wStoneOfMesteryCount, dMSG_TANTILLESS, lpLog->wTantillessCount, dMSG_CRYSTAL, lpLog->wCrystalCount,
				dMSG_UPGRADE_MATERIALS_ITEM, dMSG_NORMAL,strUpgradeMaterialsNormalItemName, dMSG_PREMIUM , strUpgradeMaterialsItemName);

//예 : [090904 11:11:50] - 아이템 강화 - [zclunt2/짐승남:192.168.0.1] FI[550] [성공] 이터널소드 I → 이터널소드 II [재료] gold : 500만 
// 신비석 : 1 / 탈틸리스유물 : 2/ 결정석 : 2  [보조제] 일반 : 미사용 / 프리미엄 : 짱강화제

			return;
			}
		case eEIW_SUCCESS_COPY_PREFIX : 
			{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s + %d.%.2d Hour",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				dMSG_SUCCESS_COPY_PREFIX,
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
				_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm/60,_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm&60);
				
				return;
			}
		case eEIW_FAILED_COPY_PREFIX : 
			{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s + %d.%.2d Hour",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				dMSG_FAILED_COPY_PREFIX,
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
				_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm/60,_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm&60);
				
				return;
			}
		case eEIW_SUCCESS_ATTACH_PREFIX : 
			{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s + %d.%.2d Hour",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				dMSG_SUCCESS_ATTACH_PREFIX,
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
				_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm/60,_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm&60);
				
				return;
			}
		case eEIW_FAILED_ATTACH_PREFIX : 
			{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s + %d.%.2d Hour",
				_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,
				_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
				dMSG_FAILED_ATTACH_PREFIX,
				_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP, 
				_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName(),_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm/60,_lpLog->mLOG_ENCHANT_ITEM.wRenewTerm&60);
				
				return;
			}

		case eEIW_SUCCESS_INCREASE_CARVING_LEVEL : 
			{// 각인 레벨증가 성공
			LOG_CARVE_ITEM* lpLog = (LOG_CARVE_ITEM*) &_lpLog->mLOG_ENCHANT_ITEM;
		
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s Lv %d%s%d] [%s] %s/gold : %d / %s : %d",
				lpLog->logTime.yy, lpLog->logTime.mm, lpLog->logTime.dd, lpLog->logTime.hour, lpLog->logTime.min, lpLog->logTime.getSecond(),
				dMSG_SUCCESS_INCREASE_CARVING_LEVEL, lpLog->strId, lpLog->strName, lpLog->strIP, lpLog->wFieldUniqueIndex,
				 ((cITEM *)& lpLog->cTargetItem)->getName(),lpLog->bf8BeforCarvingLevel ,dMSG_RIGHT_ARROW,lpLog->bf8AfterCarvingLevel,
				dMSG_MATERIALS, g_aBasicItem[lpLog->wUseItem].m_strName, lpLog->dwGold, dMSG_CRYSTAL, lpLog->bf5CrystalCount);
//예 : [090904 11:11:50] - 아이템 강화 - [zclunt2/짐승남:192.168.0.1] FI[550] [무기 Lv 1 →  2] [재료] 주문서 / gold : 500만 /결정석 : 2 
  			
				return;
			}
		case eEIW_FAILED_INCREASE_CARVING_LEVEL : 
			{// 각인 레벨증가 실패
			LOG_CARVE_ITEM* lpLog = (LOG_CARVE_ITEM*) &_lpLog->mLOG_ENCHANT_ITEM;
		
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s Lv %d%s%d] [%s] %s/gold : %d / %s : %d",
				lpLog->logTime.yy, lpLog->logTime.mm, lpLog->logTime.dd, lpLog->logTime.hour, lpLog->logTime.min, lpLog->logTime.getSecond(),
				dMSG_FAILED_INCREASE_CARVING_LEVEL, lpLog->strId, lpLog->strName, lpLog->strIP, lpLog->wFieldUniqueIndex,
				 ((cITEM *)& lpLog->cTargetItem)->getName(),lpLog->bf8BeforCarvingLevel ,dMSG_RIGHT_ARROW,lpLog->bf8AfterCarvingLevel,
				dMSG_MATERIALS, g_aBasicItem[lpLog->wUseItem].m_strName, lpLog->dwGold, dMSG_CRYSTAL, lpLog->bf5CrystalCount);
//예 : [090904 11:11:50] - 아이템 강화 - [zclunt2/짐승남:192.168.0.1] FI[550] [무기 Lv 1 →  2] [재료] 주문서 / gold : 500만 /결정석 : 2 
				return;
			}
		case eEIW_SUCCESS_SET_CARVING_LEVEL : 
			{// 각인 레벨설정 성공
			LOG_CARVE_ITEM* lpLog = (LOG_CARVE_ITEM*) &_lpLog->mLOG_ENCHANT_ITEM;
		
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s Lv %d%s%d] [%s] %s/gold : %d / %s : %d",
				lpLog->logTime.yy, lpLog->logTime.mm, lpLog->logTime.dd, lpLog->logTime.hour, lpLog->logTime.min, lpLog->logTime.getSecond(),
				dMSG_SUCCESS_SET_CARVING_LEVEL, lpLog->strId, lpLog->strName, lpLog->strIP, lpLog->wFieldUniqueIndex,
				 ((cITEM *)& lpLog->cTargetItem)->getName(),lpLog->bf8BeforCarvingLevel ,dMSG_RIGHT_ARROW,lpLog->bf8AfterCarvingLevel,
				dMSG_MATERIALS, g_aBasicItem[lpLog->wUseItem].m_strName, lpLog->dwGold, dMSG_CRYSTAL, lpLog->bf5CrystalCount);
//예 : [090904 11:11:50] - 아이템 강화 - [zclunt2/짐승남:192.168.0.1] FI[550] [무기 Lv 1 →  2] [재료] 주문서 / gold : 500만 /결정석 : 2 
				return;
			}
		case eEIW_FAILED_SET_CARVING_LEVEL : 
			{// 각인 레벨설정 실패
			LOG_CARVE_ITEM* lpLog = (LOG_CARVE_ITEM*) &_lpLog->mLOG_ENCHANT_ITEM;
		
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s Lv %d%s%d] [%s] %s/gold : %d / %s : %d",
				lpLog->logTime.yy, lpLog->logTime.mm, lpLog->logTime.dd, lpLog->logTime.hour, lpLog->logTime.min, lpLog->logTime.getSecond(),
				dMSG_FAILED_SET_CARVING_LEVEL, lpLog->strId, lpLog->strName, lpLog->strIP, lpLog->wFieldUniqueIndex,
				 ((cITEM *)& lpLog->cTargetItem)->getName(),lpLog->bf8BeforCarvingLevel ,dMSG_RIGHT_ARROW,lpLog->bf8AfterCarvingLevel,
				dMSG_MATERIALS, g_aBasicItem[lpLog->wUseItem].m_strName, lpLog->dwGold, dMSG_CRYSTAL, lpLog->bf5CrystalCount);
//예 : [090904 11:11:50] - 아이템 강화 - [zclunt2/짐승남:192.168.0.1] FI[550] [무기 Lv 1 →  2] [재료] 주문서 / gold : 500만 /결정석 : 2 
				return;
			}
		default	:
			strcpy(_lpstrReturnText,"errorLog!! in makeEnchantItemLog");
			break;
	}

	if	(bIsEnchantResult)
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s %s- [%s/%s] [IP %s] FI[%d] [%s]",
			_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,
			_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
			dMSG_ENCHANT,strWork,
			_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP,_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,
			((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName());
	else
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",
			_lpLog->mLOG_ENCHANT_ITEM.logTime.yy,_lpLog->mLOG_ENCHANT_ITEM.logTime.mm,_lpLog->mLOG_ENCHANT_ITEM.logTime.dd,_lpLog->mLOG_ENCHANT_ITEM.logTime.hour,
			_lpLog->mLOG_ENCHANT_ITEM.logTime.min,_lpLog->mLOG_ENCHANT_ITEM.logTime.getSecond(),
			strWork,
			_lpLog->mLOG_ENCHANT_ITEM.strId,_lpLog->mLOG_ENCHANT_ITEM.strName,_lpLog->mLOG_ENCHANT_ITEM.strIP,_lpLog->mLOG_ENCHANT_ITEM.wFieldUniqueIndex,
			((cITEM *)&_lpLog->mLOG_ENCHANT_ITEM.cItem)->getName());
}

BOOL
makeCreateItemLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	LOG_CREATE_ITEM	*lpCreateItem	=	&_lpLog->mLOG_CREATE_ITEM;

	char	strMethod[32];
	BOOL	bIsExistSamePrefix	=	FALSE;
	int		i,j;

	cITEM		*lpItem		=	(cITEM *)&lpCreateItem->cItem;
	cBASIC_ITEM	*lpBasicItem=	lpItem->getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(lpBasicItem->m_bIsExceptionItem	==	FALSE)
	{
		for	(i=0;i<dITEM_PREFIX_COUNT;i++)
		{
			int	iPrefix1	=	lpItem->m_aPrefix[i].m_wPrefix;

			if	(iPrefix1	==	0xffff)
				break;

			int	iPrefixType1	=	g_aItemPrefix[iPrefix1].m_wType;

			for	(j=0;j<dITEM_PREFIX_COUNT;j++)
			{
				int	iPrefix2	=	lpItem->m_aPrefix[j].m_wPrefix;

				if	(iPrefix2	==	0xffff)
					continue;

				int	iPrefixType2	=	g_aItemPrefix[iPrefix2].m_wType;

				if	(iPrefix1	==	iPrefix2 && i!=j)	//	접두사 타입이 같고 위치가 다르다.
					bIsExistSamePrefix	=	TRUE;
			}
		}
	}

	switch(lpCreateItem->wMethod)
	{
		case	eCIM_LOTTERY				:
			strcpy(strMethod,dMSG_LOTTERY_ITEM);
			break;
		case	eCIM_ITEM_PACK				:
			strcpy(strMethod,dMSG_ITEM_PACK);
			break;
		case	eCIM_BREEDING_RECORD_BOOK	:
			strcpy(strMethod,dMSG_BREEDING_RECORD_BOOK);
			break;
		case	eCIM_OLD					:
			strcpy(strMethod,dMSG_CREATE_ITEM);
			break;
		case	eCIM_USE_BOTTOMLESS_BOX		:
			strcpy(strMethod,dMSG_CREATE_BY_BOTTOMLESS_BOX);
			break;
		case	eCIM_WITHDRAW_FROM_GUILD_INVENTORY	:
		case	eCIM_WITHDRAW_FROM_GUILD_INVENTORY2	:
			strcpy(strMethod,dMSG_WITHDRAW_FROM_GUILD_INVENTORY);
			break;
		case	eCIM_BLOCK_LIGHT_POCKET		:
			strcpy(strMethod,dMSG_BLOCK_LIGHT_POCKET);
			break;
		case	eCIM_CUT_PREFIX		:		// 잘라내기한 접두사 아이템
			strcpy(strMethod,dMSG_CUT_PREFIX);
			break;
	}

	if	(g_bIsExportIncorrectPrefixItem	&&	bIsExistSamePrefix	==	FALSE)
		return	FALSE;

	char	*strCheck[2]	=	{"","!!!!!!!!!!!!!!!!"};

	if	(lpCreateItem->wMethod	==	eCIM_WITHDRAW_FROM_GUILD_INVENTORY2)
		sprintf(_lpstrReturnText,"%s[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [guild %s] %d:%s ",strCheck[bIsExistSamePrefix],
			lpCreateItem->logTime.yy,lpCreateItem->logTime.mm,lpCreateItem->logTime.dd,lpCreateItem->logTime.hour,lpCreateItem->logTime.min,lpCreateItem->logTime.getSecond(),
			strMethod,lpCreateItem->strId,lpCreateItem->strName,lpCreateItem->strIP,lpCreateItem->wFieldUniqueIndex,
			lpCreateItem->strGuildName,
			lpCreateItem->cItem.m_dwSerial,	((cITEM *)&lpCreateItem->cItem)->getName());
	else
		sprintf(_lpstrReturnText,"%s[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %d:%s",strCheck[bIsExistSamePrefix],
			lpCreateItem->logTime.yy,lpCreateItem->logTime.mm,lpCreateItem->logTime.dd,lpCreateItem->logTime.hour,lpCreateItem->logTime.min,lpCreateItem->logTime.getSecond(),
			strMethod,lpCreateItem->strId,lpCreateItem->strName,lpCreateItem->strIP,lpCreateItem->wFieldUniqueIndex,
			lpCreateItem->cItem.m_dwSerial,	((cITEM *)&lpCreateItem->cItem)->getName());

	return	TRUE;
}

BOOL
makeCreateItemFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment)
{
	char		strMethod[32];
	BOOL		bIsExistSamePrefix	=	FALSE;
	int			i,j;

	LOG_CREATE_ITEM	*lpCreateItem	=	&_lpLog->mLOG_CREATE_ITEM;

	cITEM		*lpItem		=	(cITEM *)&lpCreateItem->cItem;
	cBASIC_ITEM	*lpBasicItem=	lpItem->getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(lpBasicItem->m_bIsExceptionItem	==	FALSE)
	{
		for	(i=0;i<dITEM_PREFIX_COUNT;i++)
		{
			int	iPrefix1	=	lpItem->m_aPrefix[i].m_wPrefix;

			if	(iPrefix1	==	0xffff)
				break;

			int	iPrefixType1	=	g_aItemPrefix[iPrefix1].m_wType;

			for	(j=0;j<dITEM_PREFIX_COUNT;j++)
			{
				int	iPrefix2	=	lpItem->m_aPrefix[j].m_wPrefix;

				if	(iPrefix2	==	0xffff)
					continue;

				int	iPrefixType2	=	g_aItemPrefix[iPrefix2].m_wType;

				if	(iPrefix1	==	iPrefix2 && i!=j)	//	접두사 타입이 같고 위치가 다르다.
					bIsExistSamePrefix	=	TRUE;
			}
		}
	}

	switch(lpCreateItem->wMethod)
	{
		case	eCIM_LOTTERY				:
			strcpy(strMethod,dMSG_LOTTERY_ITEM);
			break;
		case	eCIM_ITEM_PACK				:
			strcpy(strMethod,dMSG_ITEM_PACK);
			break;
		case	eCIM_BREEDING_RECORD_BOOK	:
			strcpy(strMethod,dMSG_BREEDING_RECORD_BOOK);
			break;
		case	eCIM_OLD					:
			strcpy(strMethod,dMSG_CREATE_ITEM);
			break;
		case	eCIM_USE_BOTTOMLESS_BOX		:
			strcpy(strMethod,dMSG_CREATE_BY_BOTTOMLESS_BOX);
			break;
		case	eCIM_WITHDRAW_FROM_GUILD_INVENTORY	:
		case	eCIM_WITHDRAW_FROM_GUILD_INVENTORY2	:
			strcpy(strMethod,dMSG_WITHDRAW_FROM_GUILD_INVENTORY);
			break;
		case	eCIM_BLOCK_LIGHT_POCKET		:
			strcpy(strMethod,dMSG_BLOCK_LIGHT_POCKET);
			break;
		case	eCIM_CUT_PREFIX		:		// 잘라내기한 접두사 아이템
			strcpy(strMethod,dMSG_CUT_PREFIX);
			break;
	}

	if	(g_bIsExportIncorrectPrefixItem	&&	bIsExistSamePrefix	==	FALSE)
		return	FALSE;

	char	*strCheck[2]	=	{"","!!!!!!!!!!!!!!!!"};

	if	(lpCreateItem->wMethod	==	eCIM_WITHDRAW_FROM_GUILD_INVENTORY2)
		sprintf(_lpstrReturnText,"%s[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Guild %s] %s",strCheck[bIsExistSamePrefix],
			lpCreateItem->logTime.yy,lpCreateItem->logTime.mm,lpCreateItem->logTime.dd,lpCreateItem->logTime.hour,lpCreateItem->logTime.min,lpCreateItem->logTime.getSecond(),
			strMethod,lpCreateItem->strId,lpCreateItem->strName,lpCreateItem->strIP,lpCreateItem->wFieldUniqueIndex,
			lpCreateItem->strGuildName,
			((cITEM *)&lpCreateItem->cItem)->getName());
	else
		sprintf(_lpstrReturnText,"%s[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",strCheck[bIsExistSamePrefix],
			lpCreateItem->logTime.yy,lpCreateItem->logTime.mm,lpCreateItem->logTime.dd,lpCreateItem->logTime.hour,lpCreateItem->logTime.min,lpCreateItem->logTime.getSecond(),
			strMethod,lpCreateItem->strId,lpCreateItem->strName,lpCreateItem->strIP,lpCreateItem->wFieldUniqueIndex,
			((cITEM *)&lpCreateItem->cItem)->getName());

	exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpCreateItem->cItem);

	return	TRUE;
}

void
makeItemPickLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	LOG_ITEM_PICK	*lpPickLog	=	(LOG_ITEM_PICK	*)_lpLog;

	switch(lpPickLog->bf6Method)
	{
		case	ePIM_NORMAL	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",
				lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_PCIK_ITEM,
				lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,((cITEM *)&lpPickLog->cItem)->getName());
			break;
		case	ePIM_QUEST	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Q] [%s]",
				lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_PCIK_ITEM,
				lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,((cITEM *)&lpPickLog->cItem)->getName());
			break;
		case	ePIM_BEAST_PICK	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_BEAST_PICK,lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,((cITEM *)&lpPickLog->cItem)->getName());
			break;
		case	ePIM_BEAST_RETURN	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_RECEIVE_FROM_BEAST,lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,((cITEM *)&lpPickLog->cItem)->getName());
			break;
		case	ePIM_ARCA	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_ARCA,lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,((cITEM *)&lpPickLog->cItem)->getName());
			break;
		case	ePIM_STEAL	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_STEAL,lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,((cITEM *)&lpPickLog->cItem)->getName());
			break;
		case	ePIM_MINIPET_PICK	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_PICK_BY_MINIPET,lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,((cITEM *)&lpPickLog->cItem)->getName());
			break;

		case	ePIM_TREASURE_MAP_EVENT	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_GET_GOLD_BY_TREASURE_EVENET,lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,((cITEM *)&lpPickLog->cItem)->getName());
			break;

		default	:
			strcpy(_lpstrReturnText,"errorLog!! in makeItemPickLog");
			break;
	}
}

BOOL
makeItemPickFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment)
{
	LOG_ITEM_PICK	*lpPickLog	=	(LOG_ITEM_PICK	*)_lpLog;

	char	*lpstrItemName	=	((cITEM *)&lpPickLog->cItem)->getName();

	switch(lpPickLog->bf6Method)
	{
		case	ePIM_NORMAL	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",
				lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_PCIK_ITEM,
				lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,lpstrItemName);
			break;
		}
		case	ePIM_QUEST	:	//	퀘스트 아이템 줏음
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Q] %s",
				lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_PCIK_ITEM,
				lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,lpstrItemName);
			break;
		}
		case	ePIM_BEAST_PICK	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_BEAST_PICK,lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,lpstrItemName);
			break;
		}
		case	ePIM_BEAST_RETURN	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_RECEIVE_FROM_BEAST,lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,lpstrItemName);
			break;
		}
		case	ePIM_ARCA	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_ARCA,lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,lpstrItemName);
			break;
		}
		case	ePIM_STEAL	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_STEAL,lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,lpstrItemName);
			break;
		}
		case	ePIM_MINIPET_PICK	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_PICK_BY_MINIPET,lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,lpstrItemName);
			break;
		}
		case	ePIM_TREASURE_MAP_EVENT	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",lpPickLog->logTime.yy,lpPickLog->logTime.mm,lpPickLog->logTime.dd,lpPickLog->logTime.hour,lpPickLog->logTime.min,lpPickLog->logTime.getSecond(),
				dMSG_GET_GOLD_BY_TREASURE_EVENET,lpPickLog->strId,lpPickLog->strName,lpPickLog->strIP, 
				lpPickLog->wFieldUniqueIndex,lpstrItemName);
			break;

		default	:
			strcpy(_lpstrReturnText,"errorLog!! in makeItemPickFullLog");
			return	FALSE;
	}

	exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpPickLog->cItem);

	return	TRUE;
}


void
makeEventLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	LOG_EVENT	*lpLog	=	&_lpLog->mLOG_EVENT;

	if (lpLog->wType	==	eEW_OXQUIZ_WINNER)
	{
		if (lpLog->wValue == 0)
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- Winner Count %d",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_OX_QUIZ_WINNER,lpLog->wCount);
		}
		else
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- %dth Winner Count %d",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_OX_QUIZ_WINNER,lpLog->wValue,lpLog->wCount);
		}
	}
	else
	if (lpLog->wType	==	eEW_OXQUIZ_COMER_LIST)
	{
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- Comer Count %d",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_OX_QUIZ_COMER,lpLog->wCount);
	}
	else
	if (lpLog->wType	==	eEW_OXQUIZ_LOSER_LIST)
	{
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- Loser Count %d,Question Count %d",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_OX_QUIZ_LOSER,lpLog->wCount,lpLog->wValue);
	}
	else
	if (lpLog->wType	==	eEW_OXQUIZ_QUESTION)
	{
		COXQuizQuesionForLogPacket	*lpQuestion	=	(COXQuizQuesionForLogPacket	*)lpLog->astrName;
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- ",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_OX_QUIZ_QUESTION);
	}
	else
	if (lpLog->wType	==	eEW_WORDQUIZ_WINNER)
	{
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- Winner Count %d",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_WORD_QUIZ_WINNER,lpLog->wCount);
	}
	else
	if (lpLog->wType	==	eEW_WORDQUIZ_COMER_LIST)
	{
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- Comer Count %d",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_WORD_QUIZ_COMER,lpLog->wCount);
	}
	else
	if (lpLog->wType	==	eEW_WORDQUIZ_LOSER_LIST)
	{
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- Loser Count %d,Question Count %d",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_WORD_QUIZ_LOSER,lpLog->wCount,lpLog->wValue);
	}
	else
	if (lpLog->wType	==	eEW_WORDQUIZ_QUESTION)
	{
		cWordQuizQuesionForLogPacket	*lpQuestion	=	(cWordQuizQuesionForLogPacket	*)lpLog->astrName;
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- ",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_WORD_QUIZ_QUESTION);
	}
}

void
makeEventFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,FILE *_lpFP)
{
	LOG_EVENT	*lpLog	=	&_lpLog->mLOG_EVENT;

	if (lpLog->wType	==	eEW_OXQUIZ_WINNER)
	{
		if (lpLog->wValue == 0)
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- Winner Count %d\n",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_OX_QUIZ_WINNER,lpLog->wCount);
		}
		else
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- %dth Winner Count %d\n",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_OX_QUIZ_WINNER,lpLog->wValue,lpLog->wCount);
		}

		fprintf(_lpFP,_lpstrReturnText);

		for (int i=0;i<lpLog->wCount;i++)
			fprintf(_lpFP,"    %s\n",lpLog->astrName[i]);

		strcpy(_lpstrReturnText,"\n");
	}
	else
	if (lpLog->wType	==	eEW_OXQUIZ_COMER_LIST)
	{
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- Count %d\n",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_OX_QUIZ_COMER,lpLog->wCount);
		fprintf(_lpFP,_lpstrReturnText);
		for (int i=0;i<lpLog->wCount;i++)
		{
			fprintf(_lpFP,"    %s\n",lpLog->astrName[i]);
		}
		strcpy(_lpstrReturnText,"\n");
	}
	else
	if (lpLog->wType	==	eEW_OXQUIZ_LOSER_LIST)
	{
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- Loser Count %d,Question Count %d\n",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_OX_QUIZ_LOSER,lpLog->wCount,lpLog->wValue);
		fprintf(_lpFP,_lpstrReturnText);
		for (int i=0;i<lpLog->wCount;i++)
		{
			fprintf(_lpFP,"    %s\n",lpLog->astrName[i]);
		}
		strcpy(_lpstrReturnText,"\n");
	}
	else
	if (lpLog->wType	==	eEW_OXQUIZ_QUESTION)
	{
		char	*strOX[]	=	{"O","X"};

		COXQuizQuesionForLogPacket	*lpQuestion	=	(COXQuizQuesionForLogPacket	*)lpLog->astrName;
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- \n",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_OX_QUIZ_QUESTION);
		fprintf(_lpFP,_lpstrReturnText);
		fprintf(_lpFP,"    Q %d] %s A] %s\n",lpQuestion->m_wQuestionCounter,lpQuestion->m_strQuestion,strOX[lpQuestion->m_wSolution]);
		strcpy(_lpstrReturnText,"\n");
	}
	else
	if (lpLog->wType	==	eEW_WORDQUIZ_WINNER)
	{
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- Winner Count %d\n",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_WORD_QUIZ_WINNER,lpLog->wCount);

		fprintf(_lpFP,_lpstrReturnText);

		for (int i=0;i<lpLog->wCount;i++)
		{
			fprintf(_lpFP,"    %s\n",lpLog->astrName[i]);
		}
		strcpy(_lpstrReturnText,"\n");
	}
	else
	if (lpLog->wType	==	eEW_WORDQUIZ_COMER_LIST)
	{
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- Count %d\n",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_WORD_QUIZ_COMER,lpLog->wCount);
		fprintf(_lpFP,_lpstrReturnText);
		for (int i=0;i<lpLog->wCount;i++)
		{
			fprintf(_lpFP,"    %s\n",lpLog->astrName[i]);
		}
		strcpy(_lpstrReturnText,"\n");
	}
	else
	if (lpLog->wType	==	eEW_WORDQUIZ_LOSER_LIST)
	{
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- Loser Count %d,Question Count %d\n",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_WORD_QUIZ_LOSER,lpLog->wCount,lpLog->wValue);
		fprintf(_lpFP,_lpstrReturnText);
		for (int i=0;i<lpLog->wCount;i++)
		{
			fprintf(_lpFP,"    %s\n",lpLog->astrName[i]);
		}
		strcpy(_lpstrReturnText,"\n");
	}
	else
	if (lpLog->wType	==	eEW_WORDQUIZ_QUESTION)
	{
		cWordQuizQuesionForLogPacket	*lpQuestion	=	(cWordQuizQuesionForLogPacket	*)lpLog->astrName;
		sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- \n",lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
			dMSG_WORD_QUIZ_QUESTION);
		fprintf(_lpFP,_lpstrReturnText);
		fprintf(_lpFP,"    Q %d] %s A] %s\n",lpQuestion->m_wQuestionCounter,lpQuestion->m_strQuestion,lpQuestion->m_strSolution);
		strcpy(_lpstrReturnText,"\n");
	}
}


void
makeErrorLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	LOG_GUILD_WORK	*lpPacket	=	&_lpLog->mLOG_GUILD_WORK;

	switch(lpPacket->wWork)
	{
		case	eERROR_LOG_QUEST_EXP_SERIAL_RECEIVE	:
		{
			LOG_ERROR_QUEST_EXP_SERIAL_RECEIVE *lpLog	= (LOG_ERROR_QUEST_EXP_SERIAL_RECEIVE*)lpPacket;

			char	strQuestName[128];

			CQuestItem	*lpQuest	=	CQuest::GetQuest(lpLog->iQuestIndex);

			if	(lpQuest)
			{
				if	(lpLog->iQuestIndex	>=	dQUEST_AND_ARBEIT_BORDER_VALUE)
					sprintf(strQuestName,"[A:%d]%s",lpLog->iQuestIndex-dQUEST_AND_ARBEIT_BORDER_VALUE,lpQuest->m_strName);
				else
					sprintf(strQuestName,"[Q:%d]%s",lpLog->iQuestIndex,lpQuest->m_strName);
			}
			else
			{
				if	(lpLog->iQuestIndex	>=	dQUEST_AND_ARBEIT_BORDER_VALUE)
					sprintf(strQuestName,"[A:%d]",lpLog->iQuestIndex-dQUEST_AND_ARBEIT_BORDER_VALUE);
				else
					sprintf(strQuestName,"[Q:%d]",lpLog->iQuestIndex);
			}
			
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Quest:%s] [exp:%d]",
				lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				"EXP BUG",
				lpLog->strId , lpLog->strName , lpLog->strIP,lpLog->wFieldUniqueIndex,
				strQuestName,lpLog->iQuestExp);

			break;
		}
	}
}


void
makeErrorFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	LOG_GUILD_WORK	*lpPacket	=	&_lpLog->mLOG_GUILD_WORK;

	switch(lpPacket->wWork)
	{
		case	eERROR_LOG_QUEST_EXP_SERIAL_RECEIVE	:
		{
			LOG_ERROR_QUEST_EXP_SERIAL_RECEIVE *lpLog	= (LOG_ERROR_QUEST_EXP_SERIAL_RECEIVE*)lpPacket;

			char	strQuestName[128];

			CQuestItem	*lpQuest	=	CQuest::GetQuest(lpLog->iQuestIndex);

			if	(lpQuest)
			{
				if	(lpLog->iQuestIndex	>=	dQUEST_AND_ARBEIT_BORDER_VALUE)
					sprintf(strQuestName,"[A:%d]%s",lpLog->iQuestIndex-dQUEST_AND_ARBEIT_BORDER_VALUE,lpQuest->m_strName);
				else
					sprintf(strQuestName,"[Q:%d]%s",lpLog->iQuestIndex,lpQuest->m_strName);
			}
			else
			{
				if	(lpLog->iQuestIndex	>=	dQUEST_AND_ARBEIT_BORDER_VALUE)
					sprintf(strQuestName,"[A:%d]",lpLog->iQuestIndex-dQUEST_AND_ARBEIT_BORDER_VALUE);
				else
					sprintf(strQuestName,"[Q:%d]",lpLog->iQuestIndex);
			}
			
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Quest:%s] [exp:%d]",
				lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				"EXP BUG",
				lpLog->strId , lpLog->strName , lpLog->strIP,lpLog->wFieldUniqueIndex,
				strQuestName,lpLog->iQuestExp);

			break;
		}
	}
}

void
makeGWLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	LOG_GUILD_WORK	*lpPacket	=	&_lpLog->mLOG_GUILD_WORK;

	switch(lpPacket->wWork)
	{
		case	eGW_TRANS_GUILD_TO_GVG_SERVER	:
		{
			LOG_S_TRANS_GUILD *lpLog	= (LOG_S_TRANS_GUILD*)lpPacket;
			
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [guild %s]",
				lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_TRANS_GUILD_TO_GVG,
				lpLog->strId , lpLog->strName , lpLog->strIP,lpLog->wFieldUniqueIndex,
				lpLog->strGuildName);
			break;
			
		}
		case	eGW_GUILD_DUNGEON_PLAY	:
		{
			LOG_S_RECORD_GUILD_DUNGEON_PLAYTIME *lpLog	= (LOG_S_RECORD_GUILD_DUNGEON_PLAYTIME*)lpPacket;
			
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [guild %s]",
				lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_GUILD_DUNGEON_PLAYTIME,
				lpLog->strId , lpLog->strName , lpLog->strIP,lpLog->wFieldUniqueIndex,
				lpLog->strGuildName);
			break;
		}
		case	eGW_ACTIVE_GUILD_MASTER_GRACE	:
		{
			LOG_S_RECORD_GUILD_MASTER_GRACE *lpLog = (LOG_S_RECORD_GUILD_MASTER_GRACE*)lpPacket;
			
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [guild %s]",
				lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_GUILD_MASTER_GRACE,
				lpLog->strId , lpLog->strName , lpLog->strIP,lpLog->wFieldUniqueIndex,
				lpLog->strGuildName);
			break;
		}
		case	eGW_ACTIVE_GUILD_ROEN_GOOD_WILL	:
		{
			LOG_S_RECORD_GUILD_ROEN_GOOD_WILL *lpLog = (LOG_S_RECORD_GUILD_ROEN_GOOD_WILL*)lpPacket;
			
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [guild %s] [LAST:%d NEW:%d] ",
				lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_GUILD_CHANGE_ROEN_GOOD_WILL_DATA,
				lpLog->strId , lpLog->strName , lpLog->strIP,lpLog->wFieldUniqueIndex,
				lpLog->strGuildName,lpLog->wLast,lpLog->wNew);
			break;
		}
		case	eGW_DELETE_GUILD	:
		{
			LOG_DELETE_GUILD *lpLog = (LOG_DELETE_GUILD*)lpPacket;
			char	strReason[32] = "";
			switch(lpLog->wReason)
			{
			case eDGR_NORMAL:
				strcpy(strReason, dMSG_DELETE_GUILD_REASON_NORMAL);
				break;

			case eDGR_CREATE_FAIL:
				strcpy(strReason, dMSG_DELETE_GUILD_REASON_FAIL_CREATE);
				break;

			case eDGR_TIME_OVER:
				strcpy(strReason, dMSG_DELETE_GUILD_REASON_TIME_OVER);
				break;
			}
			
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- requestMan[requestMan %s] [guild %s] [reason %s] ",
				lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_DELETE_GUILD,
				lpLog->strName ,lpLog->strGuildName,strReason);
			break;
		}
		case	eGW_CHANGE_GUILD_RANK	:
			{
				LOG_CHANGE_GUILD_RANK *lpLog = (LOG_CHANGE_GUILD_RANK*)lpPacket;
				char	strReason[32]	= "";
				char	strRank[32]		= "";
				
				switch(lpLog->wReason)
				{
				case eCGR_APPOINT:
					strcpy(strReason, dMSG_APPOINT);
					break;
					
				case eCGR_DISMISSAL:
					strcpy(strReason, dMSG_DISMISSAL);
					break;
					
				case eCGR_CHANGERANK_MASTER_APPOINT_SPECIAL:
					strcpy(strReason, dMSG_CHANGERANK_MASTER_APPOINT_SPECIAL);
					break;
				case eCGR_TIME_OVER	:
					strcpy(strReason, dMSG_DELETE_GUILD_REASON_TIME_OVER);
					break;
				}
				
				switch(lpLog->wMasterRank)
				{
				case dGUILD_CLASS_ROOKIE:
					strcpy(strRank, dMSG_GUILD_CLASS_ROOKIE);
					break;
				case dGUILD_CLASS_MEMBER:
					strcpy(strRank, dMSG_GUILD_CLASS_MEMBER);
					break;
				case dGUILD_CLASS_PARLIAMENT:
					strcpy(strRank, dMSG_GUILD_CLASS_PARLIAMENT);
					break;
				case  dGUILD_CLASS_SUBMASTER:
					strcpy(strRank, dMSG_GUILD_CLASS_SUBMASTER);
					break;
				case dGUILD_CLASS_ELDER:
					strcpy(strRank, dMSG_GUILD_CLASS_ELDER);
					break;
				case dGUILD_CLASS_MASTER:
					strcpy(strRank, dMSG_GUILD_CLASS_MASTER);
					break;
					
				}
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -change guild rank- [guild:%s] [change master:%s->%s] [reason:%s]",
					lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
					lpLog->strGuildName, lpLog->strName ,lpLog->strTargetName, strReason);		
				
				break;
			}
	}
}

void
makeGWFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	LOG_GUILD_WORK	*lpPacket	=	&_lpLog->mLOG_GUILD_WORK;

	switch(lpPacket->wWork)
	{
		case	eGW_TRANS_GUILD_TO_GVG_SERVER	:
		{
			LOG_S_TRANS_GUILD *lpLog	= (LOG_S_TRANS_GUILD*)lpPacket;
			
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [guild:%s] [trans guild name:%s]",
				lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_TRANS_GUILD_TO_GVG,
				lpLog->strId , lpLog->strName , lpLog->strIP,lpLog->wFieldUniqueIndex,
				lpLog->strGuildName,lpLog->strTransGuildName);
			break;
			
		}

		case	eGW_GUILD_DUNGEON_PLAY	:
		{
			LOG_S_RECORD_GUILD_DUNGEON_PLAYTIME *lpLog	= (LOG_S_RECORD_GUILD_DUNGEON_PLAYTIME*)lpPacket;
			
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [guild %s] [Party Member Count:%d] [Floor %d] [PlayCount %d]\n",
				lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_GUILD_DUNGEON_PLAYTIME,
				lpLog->strId , lpLog->strName , lpLog->strIP,lpLog->wFieldUniqueIndex,
				lpLog->strGuildName,lpLog->wPlayTime , lpLog->wPartyMemberCount , lpLog->wFloor, lpLog->wPlayTime);
			break;
		}
		case	eGW_ACTIVE_GUILD_MASTER_GRACE	:
		{
			LOG_S_RECORD_GUILD_MASTER_GRACE *lpLog = (LOG_S_RECORD_GUILD_MASTER_GRACE*)lpPacket;
			
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [guild %s] [PlayCount %d]\n",
				lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_GUILD_MASTER_GRACE,
				lpLog->strId , lpLog->strName , lpLog->strIP,lpLog->wFieldUniqueIndex,
				lpLog->strGuildName,lpLog->wPlayTime );
			break;
		}
		case	eGW_ACTIVE_GUILD_ROEN_GOOD_WILL	:
		{
			LOG_S_RECORD_GUILD_ROEN_GOOD_WILL *lpLog = (LOG_S_RECORD_GUILD_ROEN_GOOD_WILL*)lpPacket;
			
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [guild %s] [LAST:%d NEW:%d]\n",
				lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_GUILD_CHANGE_ROEN_GOOD_WILL_DATA,
				lpLog->strId , lpLog->strName , lpLog->strIP,lpLog->wFieldUniqueIndex,
				lpLog->strGuildName,lpLog->wLast,lpLog->wNew);
			break;
		}
		case	eGW_DELETE_GUILD	:
		{
			LOG_DELETE_GUILD *lpLog = (LOG_DELETE_GUILD*)lpPacket;
			char	strReason[32] = "";
			switch(lpLog->wReason)
			{
			case eDGR_NORMAL:
				strcpy(strReason, dMSG_DELETE_GUILD_REASON_NORMAL);
				break;

			case eDGR_CREATE_FAIL:
				strcpy(strReason, dMSG_DELETE_GUILD_REASON_FAIL_CREATE);
				break;

			case eDGR_TIME_OVER:
				strcpy(strReason, dMSG_DELETE_GUILD_REASON_TIME_OVER);
				break;
			}
			
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- requestMan[requestMan %s] [guild %s] [reason %s] ",
				lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
				dMSG_DELETE_GUILD,
				lpLog->strName ,lpLog->strGuildName,strReason);
			break;
		}
		case	eGW_CHANGE_GUILD_RANK	:
			{
				LOG_CHANGE_GUILD_RANK *lpLog = (LOG_CHANGE_GUILD_RANK*)lpPacket;
				char	strReason[32]	= "";
				char	strRank[32]		= "";
				
				switch(lpLog->wReason)
				{
				case eCGR_APPOINT:
					strcpy(strReason, dMSG_APPOINT);
					break;
					
				case eCGR_DISMISSAL:
					strcpy(strReason, dMSG_DISMISSAL);
					break;
					
				case eCGR_CHANGERANK_MASTER_APPOINT_SPECIAL:
					strcpy(strReason, dMSG_CHANGERANK_MASTER_APPOINT_SPECIAL);
					break;
				case eCGR_TIME_OVER	:
					strcpy(strReason, dMSG_DELETE_GUILD_REASON_TIME_OVER);
					break;
				}
				
				switch(lpLog->wMasterRank)
				{
				case dGUILD_CLASS_ROOKIE:
					strcpy(strRank, dMSG_GUILD_CLASS_ROOKIE);
					break;
				case dGUILD_CLASS_MEMBER:
					strcpy(strRank, dMSG_GUILD_CLASS_MEMBER);
					break;
				case dGUILD_CLASS_PARLIAMENT:
					strcpy(strRank, dMSG_GUILD_CLASS_PARLIAMENT);
					break;
				case  dGUILD_CLASS_SUBMASTER:
					strcpy(strRank, dMSG_GUILD_CLASS_SUBMASTER);
					break;
				case dGUILD_CLASS_ELDER:
					strcpy(strRank, dMSG_GUILD_CLASS_ELDER);
					break;
				case dGUILD_CLASS_MASTER:
					strcpy(strRank, dMSG_GUILD_CLASS_MASTER);
					break;
					
				}
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -change guild rank- [guild:%s] [change master:%s->%s] [reason:%s]",
					lpLog->logTime.yy,lpLog->logTime.mm,lpLog->logTime.dd,lpLog->logTime.hour,lpLog->logTime.min,lpLog->logTime.getSecond(),
					lpLog->strGuildName, lpLog->strName ,lpLog->strTargetName, strReason);		
				
				break;
			}
	}
}

void
makeGBWLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	LOG_GUILD_BATTLE_WORK	*lpPacket	=	&_lpLog->mLOG_GUILD_BATTLE_WORK;

	switch(lpPacket->wWork)
	{
		case	eGBW_BOOKING	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s] [%d/%d]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_BOOKING_GUILD_BATTLE,lpPacket->strGuild1Name,lpPacket->bTimeOrder,lpPacket->bFieldOrder);
			break;
		}

		case	eGBW_CANCEL	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_CANCEL_GUILD_BATTLE,lpPacket->strGuild1Name);
			break;
		}
		
		case	eGBW_ENGAGE	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s] vs [%s] [%d/%d]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_ENGAGE_GUILD_BATTLE,lpPacket->strGuild1Name,lpPacket->strGuild2Name,lpPacket->bTimeOrder,lpPacket->bFieldOrder);
			break;
		}
		
		case	eGBW_RESULT	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%d] vs [%s:%d]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_RESULT_OF_GUILD_BATTLE,lpPacket->strGuild1Name,lpPacket->bGuild1VictoryPoint,lpPacket->strGuild2Name,lpPacket->bGuild2VictoryPoint);
			break;
		}
		
		case	eGBW_JOIN	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_JOIN_GUILD,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
			break;
		}
		
		case	eGBW_LEAVE	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_LEAVE_GUILD,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
			break;
		}
		
		case	eGBW_KILL	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_GUILD_BATTLE_KILL_ENEMY,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
			break;
		}
		
		case	eGBW_BEGIN	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_BEGIN_GUILD_BATTLE,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
			break;
		}
		
		case	eGBW_PRE_RESULT	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_GUILD_BATTLE_PRE_RESUT,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
			break;
		}
		
		case	eGBW_ERROR_IN_KILL	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- ",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_GUILD_BATTLE_KILL_ERROR);
			break;
		}
		
		case	eGBW_ERROR_IN_CHECK_RESULT	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_GUILD_BATTLE_RESULT_ERROR);
			break;
		}
		
		case	eGBW_NEW_KILL	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_GUILD_BATTLE_KILL_ENEMY,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
			break;
		}
		
		case	eGBW_EXTERMINATE	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s] winner ",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_EXTERMINATE_GUILD,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
			break;
		}
	}
}

void
makeGBWFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	LOG_GUILD_BATTLE_WORK	*lpPacket	=	&_lpLog->mLOG_GUILD_BATTLE_WORK;

	switch(lpPacket->wWork)
	{
		case	eGBW_BOOKING	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s] [%d/%d]\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_BOOKING_GUILD_BATTLE,lpPacket->strGuild1Name,lpPacket->bTimeOrder,lpPacket->bFieldOrder);
			break;
		case	eGBW_CANCEL	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s]\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_CANCEL_GUILD_BATTLE,lpPacket->strGuild1Name);
			break;
		case	eGBW_ENGAGE	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s] vs [%s] [%d/%d]\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_ENGAGE_GUILD_BATTLE,lpPacket->strGuild1Name,lpPacket->strGuild2Name,lpPacket->bTimeOrder,lpPacket->bFieldOrder);
			break;
		case	eGBW_RESULT	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%d] vs [%s:%d]\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_RESULT_OF_GUILD_BATTLE,lpPacket->strGuild1Name,lpPacket->bGuild1VictoryPoint,lpPacket->strGuild2Name,lpPacket->bGuild2VictoryPoint);
			break;
		case	eGBW_JOIN	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s]\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_JOIN_GUILD,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
			break;
		case	eGBW_LEAVE	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s]\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_LEAVE_GUILD,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
			break;
		case	eGBW_EXTERMINATE	:
		{
			LOG_GUILD_BATTLE_EXTERMINATE	*lpKill	=	(LOG_GUILD_BATTLE_EXTERMINATE	*)lpPacket;
			char	astrGuildName[3][dNAME_LENGTH];

			strcpy(astrGuildName[0],lpKill->strGuild1Name);
			strcpy(astrGuildName[1],lpKill->strGuild2Name);
			strcpy(astrGuildName[2],"draw game");

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s vs %s] winner : [%s]\n",lpKill->logTime.yy,lpKill->logTime.mm,lpKill->logTime.dd,lpKill->logTime.hour,lpKill->logTime.min,lpKill->logTime.getSecond(),
				dMSG_EXTERMINATE_GUILD,lpKill->strGuild1Name,lpKill->strGuild2Name,astrGuildName[lpKill->wWinner]);

			break;
		}
		case	eGBW_KILL	:
		{
			if(lpPacket->base.wSize	==	sizeof(LOG_GUILD_BATTLE_WORK))
			{
				LOG_GUILD_BATTLE_WORK_KILL	*lpKill	=	(LOG_GUILD_BATTLE_WORK_KILL	*)lpPacket;

				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s(kp %d) vs %s(kp %d)] '%s' kill '%s'. [%s] guild get %d kill point\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
					dMSG_GUILD_BATTLE_KILL_ENEMY,lpKill->strGuildName[0],lpKill->wKP1,lpKill->strGuildName[1],lpKill->wKP2,lpKill->strKiller,lpKill->strTarget,lpKill->strGuildName[lpKill->wKillerTeam],lpKill->wAddKP);	//	이긴 길드 승점
			}
			else
			{
				LOG_GUILD_BATTLE_WORK_KILL2	*lpKill	=	(LOG_GUILD_BATTLE_WORK_KILL2	*)lpPacket;

				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s(kp %d) vs %s(kp %d)] '%s' kill '%s'. [%s] guild get %d kill point\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
					dMSG_GUILD_BATTLE_KILL_ENEMY,lpKill->strGuildName[0],lpKill->bf10KP1,lpKill->strGuildName[1],lpKill->bf10KP2,lpKill->strKiller,lpKill->strTarget,lpKill->strGuildName[lpKill->bf2KillerTeam],lpKill->bf10AddKP);	//	이긴 길드 승점
			}
			break;
		}

		case	eGBW_NEW_KILL	:
		{
			LOG_GUILD_BATTLE_WORK_KILL3	*lpKill	=	(LOG_GUILD_BATTLE_WORK_KILL3	*)lpPacket;

			if (lpKill->base.wSize	!=	sizeof(LOG_GUILD_BATTLE_WORK_KILL3))
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s(kp %d) vs %s(kp %d)] '%s' kill '%s'.\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
					dMSG_GUILD_BATTLE_KILL_ENEMY,lpKill->strGuildName[0],lpKill->wKP1,lpKill->strGuildName[1],lpKill->wKP2,lpKill->strKiller,lpKill->strTarget,lpKill->strGuildName[lpKill->bf2KillerTeam],lpKill->bf10AddKP);	//	이긴 길드 승점
			}
			else
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s(kp %d) vs %s(kp %d)] '%s' kill '%s'. [%s] guild get [%d] kill point\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
					dMSG_GUILD_BATTLE_KILL_ENEMY,lpKill->strGuildName[0],lpKill->wKP1,lpKill->strGuildName[1],lpKill->wKP2,lpKill->strKiller,lpKill->strTarget,lpKill->strGuildName[lpKill->bf2KillerTeam],lpKill->bf10AddKP);	//	이긴 길드 승점
			
			break;
		}
		case	eGBW_BEGIN	:
		{
			if(lpPacket->base.wSize	==	sizeof(LOG_GUILD_BATTLE_WORK))
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%d vs %s:%d]\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
					dMSG_BEGIN_GUILD_BATTLE,lpPacket->strGuild1Name,lpPacket->bGuild1VictoryPoint,lpPacket->strGuild2Name,lpPacket->bGuild2VictoryPoint);
			}
			else
			{
				LOG_GUILD_BATTLE_BEGIN_WORK	*lpBegin	=	(LOG_GUILD_BATTLE_BEGIN_WORK *)lpPacket;
				cBeginGuildBattleInfo		*lpGuild0	=	&lpBegin->m_aInfo[0];
				cBeginGuildBattleInfo		*lpGuild1	=	&lpBegin->m_aInfo[1];

				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%d v:%d d:%d cv:%d fw:%d vp:%d vs %s:%d v:%d d:%d cv:%d fw:%d vp:%d]\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
					dMSG_BEGIN_GUILD_BATTLE,
					lpPacket->strGuild1Name,lpGuild0->m_wMemberCount,lpGuild0->m_wVictory,lpGuild0->m_wDefeat,lpGuild0->m_wCVCount,lpGuild0->m_wFightAtWeek,lpGuild0->m_iVictoryPoint,
					lpPacket->strGuild2Name,lpGuild1->m_wMemberCount,lpGuild1->m_wVictory,lpGuild1->m_wDefeat,lpGuild1->m_wCVCount,lpGuild1->m_wFightAtWeek,lpGuild1->m_iVictoryPoint);
			}
			break;
		}

		case	eGBW_PRE_RESULT	:
		{
			LOG_GUILD_BATTLE_WORK_RESULT	*lpResult	=	(LOG_GUILD_BATTLE_WORK_RESULT	*)lpPacket;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%d(kp %d) vs %s:%d(kp %d)] remain member %s:%s\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_GUILD_BATTLE_PRE_RESUT,lpResult->strGuild1Name,lpResult->bGuild1MemberCount,lpResult->wKP1,lpResult->strGuild2Name,lpResult->bGuild2MemberCount,lpResult->wKP2,
				lpResult->strGuild1MemberName,lpResult->strGuild2MemberName);
			break;
		}

		case	eGBW_ERROR_IN_KILL	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s-\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_GUILD_BATTLE_KILL_ERROR);
			break;
		}
		case	eGBW_ERROR_IN_CHECK_RESULT	:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s:%s]\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_GUILD_BATTLE_RESULT_ERROR);
			break;
		}								
	}
}

void
makeItemDropLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	LOG_ITEM_DROP	*lpDropLog	=	(LOG_ITEM_DROP	*)_lpLog;

	switch(lpDropLog->wMethod)
	{
		case	0	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",
				lpDropLog->logTime.yy,lpDropLog->logTime.mm,lpDropLog->logTime.dd,lpDropLog->logTime.hour,lpDropLog->logTime.min,lpDropLog->logTime.getSecond(),
				dMSG_DROP_ITEM,
				lpDropLog->strId,lpDropLog->strName,lpDropLog->strIP, 
				lpDropLog->wFieldUniqueIndex,((cITEM *)&lpDropLog->cItem)->getName());
			break;
		case	1	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Q] [%s]",
				lpDropLog->logTime.yy,lpDropLog->logTime.mm,lpDropLog->logTime.dd,lpDropLog->logTime.hour,lpDropLog->logTime.min,lpDropLog->logTime.getSecond(),
				dMSG_DROP_ITEM,
				lpDropLog->strId,lpDropLog->strName,lpDropLog->strIP, 
				lpDropLog->wFieldUniqueIndex,((cITEM *)&lpDropLog->cItem)->getName());
			break;

		default	:
			sprintf(_lpstrReturnText,"errorLog!! in makeItemDropLog %d",lpDropLog->wMethod);
			break;
	}
}

void
makeItemDropFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment)
{
	LOG_ITEM_DROP	*lpDropLog	=	(LOG_ITEM_DROP	*)_lpLog;

	switch(lpDropLog->wMethod)
	{
		case	0	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",
				lpDropLog->logTime.yy,lpDropLog->logTime.mm,lpDropLog->logTime.dd,lpDropLog->logTime.hour,lpDropLog->logTime.min,lpDropLog->logTime.getSecond(),
				dMSG_DROP_ITEM,
				lpDropLog->strId,lpDropLog->strName,lpDropLog->strIP, 
				lpDropLog->wFieldUniqueIndex,((cITEM *)&lpDropLog->cItem)->getName());
			break;
		case	1	:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [Q] %s",
				lpDropLog->logTime.yy,lpDropLog->logTime.mm,lpDropLog->logTime.dd,lpDropLog->logTime.hour,lpDropLog->logTime.min,lpDropLog->logTime.getSecond(),
				dMSG_DROP_ITEM,
				lpDropLog->strId,lpDropLog->strName,lpDropLog->strIP, 
				lpDropLog->wFieldUniqueIndex,((cITEM *)&lpDropLog->cItem)->getName());
			break;

		default	:
			sprintf(_lpstrReturnText,"errorLog!! in makeItemDropFullLog %d",lpDropLog->wMethod);
			break;
	}

	exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpDropLog->cItem);
}

BOOL
makeLogText(ALL_LOGMSG *_lpLog,char *_lpstrReturnText)
{
	int	iLogType	=	_lpLog->base.wType;

	switch(iLogType)
	{
		case	dLOG_ERROR	:
			makeErrorLog(_lpLog,_lpstrReturnText);
			break;

		case	dLOG_GUILD_WORK			:
			makeGWLog(_lpLog,_lpstrReturnText);
			break;

		case	dLOG_GUILD_BATTLE_WORK	:
			makeGBWLog(_lpLog,_lpstrReturnText);
			break;
		case	dLOG_ITEM_DROP:
			makeItemDropLog(_lpLog,_lpstrReturnText);
			break;
		case	dLOG_PET_WORK	:
			makePetLog(_lpLog,_lpstrReturnText);
			break;
		case	dLOG_SYSTEM		:
			makeSystemLog(_lpLog,_lpstrReturnText);
			break;

		case dLOG_EVENT		:
			makeEventLog(_lpLog,_lpstrReturnText);
			break;

		case dLOG_ITEM_PICK	:
			makeItemPickLog(_lpLog,_lpstrReturnText);
			break;

		case dLOG_ITEM_DEL	:
			makeDelItemLog(_lpLog,_lpstrReturnText);
			break;

		case dLOG_PITCHMAN_SHOP_TRADE	:
		{
			LOG_PITCHMAN_SHOP_TRADE	*lpTradeLog	=	(LOG_PITCHMAN_SHOP_TRADE	*)_lpLog;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s]",
					lpTradeLog->logTime.yy,lpTradeLog->logTime.mm,lpTradeLog->logTime.dd,lpTradeLog->logTime.hour,lpTradeLog->logTime.min,lpTradeLog->logTime.getSecond(),
					dMSG_NOJUM,
					lpTradeLog->strId,lpTradeLog->strName,lpTradeLog->strIP, 
					lpTradeLog->wFieldUniqueIndex,((cITEM *)&lpTradeLog->cItem)->getName());
			break;
		}

		case dLOG_ITEM_TRADE:
		{
			LOG_ITEM_TRADE	*lpTradeLog	=	(LOG_ITEM_TRADE	*)_lpLog;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] <-> [%s/%s] [IP %s] FI[%d]",
				lpTradeLog->logTime.yy,lpTradeLog->logTime.mm,lpTradeLog->logTime.dd,lpTradeLog->logTime.hour,lpTradeLog->logTime.min,lpTradeLog->logTime.getSecond(),
				dMSG_TRADE_ITEM,
				lpTradeLog->strRecverId,lpTradeLog->strRecverName,lpTradeLog->strRecverIP, 
				lpTradeLog->strSenderId,lpTradeLog->strSenderName,lpTradeLog->strSenderIP,lpTradeLog->wFieldUniqueIndexSender);
			break;
		}

		case dLOG_ITEM_SELL:
		{
			LOG_ITEM_SELL	*lpSellLog	=	(LOG_ITEM_SELL	*)_lpLog;
			DWORD			dwPrice		=	lpSellLog->dwSellPrice;

			if	(dwPrice==	0xcccccccc)
				dwPrice	=	0;

			if	(dwPrice &	0x80000000)
			{
				dwPrice	-=	0x80000000;

				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] [%s Gold]",
					lpSellLog->logTime.yy,lpSellLog->logTime.mm,lpSellLog->logTime.dd,
					lpSellLog->logTime.hour,lpSellLog->logTime.min,lpSellLog->logTime.getSecond(),
					dMSG_BUY_ITEM,
					lpSellLog->strId,lpSellLog->strName,lpSellLog->strIP, 
					lpSellLog->wFieldUniqueIndex,
					((cITEM *)&lpSellLog->cItem)->getName(),GetMoneyString(dwPrice));
			}
			else
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] [%s] [%s Gold]",
					lpSellLog->logTime.yy,lpSellLog->logTime.mm,lpSellLog->logTime.dd,
					lpSellLog->logTime.hour,lpSellLog->logTime.min,lpSellLog->logTime.getSecond(),
					dMSG_SELL_ITEM,
					lpSellLog->strId,lpSellLog->strName,lpSellLog->strIP, 
					lpSellLog->wFieldUniqueIndex,
					((cITEM *)&lpSellLog->cItem)->getName(),GetMoneyString(dwPrice));
			}
			break;
		}

		case dLOG_ITEM_BANK_IN:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -BNKI- [%s/%s] [IP %s] FI[%d]",
				_lpLog->mLOG_ITEM_BANK_IN.logTime.yy,_lpLog->mLOG_ITEM_BANK_IN.logTime.mm,_lpLog->mLOG_ITEM_BANK_IN.logTime.dd,_lpLog->mLOG_ITEM_BANK_IN.logTime.hour,_lpLog->mLOG_ITEM_BANK_IN.logTime.min,_lpLog->mLOG_ITEM_BANK_IN.logTime.getSecond(),
				_lpLog->mLOG_ITEM_BANK_IN.strId,_lpLog->mLOG_ITEM_BANK_IN.strName,_lpLog->mLOG_ITEM_BANK_IN.strIP,_lpLog->mLOG_ITEM_BANK_IN.wFieldUniqueIndex);
			break;

		case dLOG_ITEM_BANK_OUT:
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -BNKO- [%s/%s] [IP %s] FI[%d]",
				_lpLog->mLOG_ITEM_BANK_OUT.logTime.yy,_lpLog->mLOG_ITEM_BANK_OUT.logTime.mm,_lpLog->mLOG_ITEM_BANK_OUT.logTime.dd,_lpLog->mLOG_ITEM_BANK_OUT.logTime.hour,_lpLog->mLOG_ITEM_BANK_OUT.logTime.min,_lpLog->mLOG_ITEM_BANK_OUT.logTime.getSecond(),
				_lpLog->mLOG_ITEM_BANK_OUT.strId,_lpLog->mLOG_ITEM_BANK_OUT.strName,_lpLog->mLOG_ITEM_BANK_OUT.strIP,_lpLog->mLOG_ITEM_BANK_OUT.wFieldUniqueIndex);
			break;

		case dLOG_ITEM_WITHDRAW_PREMIUM_ITEM:
		{
			LOG_ITEM_WITHDRAW_PREMIUM_ITEM	*lpData	=	&_lpLog->mLOG_ITEM_WITHDRAW_PREMIUM_ITEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] Item %d %d",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
				dMSG_WITHDRAW_PREMIUM_ITEM,lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
				lpData->dwItemNo,lpData->dwSqnNumber);
			break;
		}

		case dLOG_ITEM_CREATE_PREMIUM_ITEM:
		{
			LOG_ITEM_CREATE_PREMIUM_ITEM	*lpData	=	&_lpLog->mLOG_ITEM_CREATE_PREMIUM_ITEM;

			if	(lpData->wMethod	==	eCPIM_WITHDRAW_CART_ITEM	||	lpData->base.wSize	!=	sizeof(LOG_ITEM_CREATE_PREMIUM_ITEM))
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %d:%s",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
					dMSG_CREATE_PREMIUM_ITEM,lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
					lpData->cItem.m_dwSerial,	((cITEM *)&lpData->cItem)->getName()
					);
			}
			else
			if	(lpData->wMethod	==	eCPIM_BUY_BY_CARROT)
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %d:%s [%d Carrot]",lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
					dMSG_CREATE_PREMIUM_ITEM_BY_CARROT,lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex,
					lpData->cItem.m_dwSerial,	((cITEM *)&lpData->cItem)->getName(),lpData->wCarrotPrice
					);
			}
			break;
		}
		case dLOG_CREATE_ITEM:
		{
			return	makeCreateItemLog(_lpLog,_lpstrReturnText);
		}
		case dLOG_USE_ITEM:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %d:%s",
				_lpLog->mLOG_USE_ITEM.logTime.yy,_lpLog->mLOG_USE_ITEM.logTime.mm,_lpLog->mLOG_USE_ITEM.logTime.dd,_lpLog->mLOG_USE_ITEM.logTime.hour,_lpLog->mLOG_USE_ITEM.logTime.min,_lpLog->mLOG_USE_ITEM.logTime.getSecond(),
				dMSG_USE_ITEM,_lpLog->mLOG_USE_ITEM.strId,_lpLog->mLOG_USE_ITEM.strName,_lpLog->mLOG_USE_ITEM.strIP,_lpLog->mLOG_USE_ITEM.wFieldUniqueIndex,
				_lpLog->mLOG_USE_ITEM.cItem.m_dwSerial,	((cITEM *)&_lpLog->mLOG_USE_ITEM.cItem)->getName()
				);
			break;
		}
		case dLOG_EXPIRED_ITEM:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %d:%s",
				_lpLog->mLOG_EXPIRED_ITEM.logTime.yy,_lpLog->mLOG_EXPIRED_ITEM.logTime.mm,_lpLog->mLOG_EXPIRED_ITEM.logTime.dd,_lpLog->mLOG_EXPIRED_ITEM.logTime.hour,_lpLog->mLOG_EXPIRED_ITEM.logTime.min,_lpLog->mLOG_EXPIRED_ITEM.logTime.getSecond(),
				dMSG_EXPIRED_ITEM,
				_lpLog->mLOG_EXPIRED_ITEM.strId,_lpLog->mLOG_EXPIRED_ITEM.strName,_lpLog->mLOG_EXPIRED_ITEM.strIP,_lpLog->mLOG_EXPIRED_ITEM.wFieldUniqueIndex,
				_lpLog->mLOG_EXPIRED_ITEM.cItem.m_dwSerial,	((cITEM *)&_lpLog->mLOG_EXPIRED_ITEM.cItem)->getName()
				);
			break;
		}
		case dLOG_DOUBLE_ITEM:
		{
			char	strWhere[20];	memset(strWhere,0,20);

			switch(_lpLog->mLOG_DOUBLE_ITEM.wWhere)
			{
				case eDIL_TRADE:
					strcpy(strWhere,"in TRADE");
					break;
				case eDIL_PICK:
					strcpy(strWhere,"in PICK");
					break;
				case eDIL_LOGIN:
					strcpy(strWhere,"in LOGIN");
					break;
				case eDIL_BANK:
					strcpy(strWhere,"in BANK");
					break;
			}

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %d:%s %s",
				_lpLog->mLOG_DOUBLE_ITEM.logTime.yy,_lpLog->mLOG_DOUBLE_ITEM.logTime.mm,_lpLog->mLOG_DOUBLE_ITEM.logTime.dd,_lpLog->mLOG_DOUBLE_ITEM.logTime.hour,_lpLog->mLOG_DOUBLE_ITEM.logTime.min,_lpLog->mLOG_DOUBLE_ITEM.logTime.getSecond(),
				dMSG_COPY_ITEM,
				_lpLog->mLOG_DOUBLE_ITEM.strId,_lpLog->mLOG_DOUBLE_ITEM.strName,_lpLog->mLOG_DOUBLE_ITEM.strIP,_lpLog->mLOG_DOUBLE_ITEM.wFieldUniqueIndex,
				_lpLog->mLOG_DOUBLE_ITEM.cItem.m_dwSerial,	((cITEM *)&_lpLog->mLOG_DOUBLE_ITEM.cItem)->getName(),strWhere
				);
			break;
		}

		case dLOG_ENCHANT_ITEM:
		{
			makeEnchantItemLog(_lpLog,_lpstrReturnText);
			break;
		}

		case dLOG_AVA_CREATE:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s]",_lpLog->mLOG_AVA_CREATE.logTime.yy,_lpLog->mLOG_AVA_CREATE.logTime.mm,_lpLog->mLOG_AVA_CREATE.logTime.dd,_lpLog->mLOG_AVA_CREATE.logTime.hour,_lpLog->mLOG_AVA_CREATE.logTime.min,_lpLog->mLOG_AVA_CREATE.logTime.getSecond(),
				dMSG_CREATE_PLAYER,_lpLog->mLOG_AVA_CREATE.strId,_lpLog->mLOG_AVA_CREATE.strName,_lpLog->mLOG_AVA_CREATE.strIP);
			break;
		}
		case dLOG_AVA_DELETE:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s]",_lpLog->mLOG_AVA_DELETE.logTime.yy,_lpLog->mLOG_AVA_DELETE.logTime.mm,_lpLog->mLOG_AVA_DELETE.logTime.dd,_lpLog->mLOG_AVA_DELETE.logTime.hour,_lpLog->mLOG_AVA_DELETE.logTime.min,_lpLog->mLOG_AVA_DELETE.logTime.getSecond(),
				dMSG_DELETE_PLAYER,_lpLog->mLOG_AVA_DELETE.strId,_lpLog->mLOG_AVA_DELETE.strName,_lpLog->mLOG_AVA_DELETE.strIP);
			break;
		}
		case dLOG_AVA_LOGIN:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s]",_lpLog->mLOG_AVA_LOGIN.logTime.yy,_lpLog->mLOG_AVA_LOGIN.logTime.mm,_lpLog->mLOG_AVA_LOGIN.logTime.dd,_lpLog->mLOG_AVA_LOGIN.logTime.hour,_lpLog->mLOG_AVA_LOGIN.logTime.min,_lpLog->mLOG_AVA_LOGIN.logTime.getSecond(),
				dMSG_LOGIN,_lpLog->mLOG_AVA_LOGIN.strId,_lpLog->mLOG_AVA_LOGIN.strName,_lpLog->mLOG_AVA_LOGIN.strIP);
			break;
		}
		case dLOG_AVA_LOGOUT:
		{
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s]",_lpLog->mLOG_AVA_LOGOUT.logTime.yy,_lpLog->mLOG_AVA_LOGOUT.logTime.mm,_lpLog->mLOG_AVA_LOGOUT.logTime.dd,_lpLog->mLOG_AVA_LOGOUT.logTime.hour,_lpLog->mLOG_AVA_LOGOUT.logTime.min,_lpLog->mLOG_AVA_LOGOUT.logTime.getSecond(),
				dMSG_LOGOUT,_lpLog->mLOG_AVA_LOGOUT.strId,_lpLog->mLOG_AVA_LOGOUT.strName,_lpLog->mLOG_AVA_LOGOUT.strIP);
			break;
		}
		case dLOG_AVA_SAVE_LOG	:
		{
			LOG_AVA_SAVE_LOG	*lpPacket	=	(LOG_AVA_SAVE_LOG	*)_lpLog;
			char				strUserId[dID_LENGTH],strUserName[dNAME_LENGTH];
			char				buffer[200];

			memcpy(buffer,lpPacket,lpPacket->base.wSize);

			if	(!GetSaveLogIDAndName(lpPacket->strIdAndName,strUserId,strUserName) || lpPacket->wLevel	>	2000 || lpPacket->wField	>	2048)
			{
				if	(lpPacket->wLevel	<=	1000	&&	lpPacket->wField	<=	2048)
				{
					for (int i=1;i<4;i++)
					{
						memcpy(lpPacket->strIdAndName,buffer+25+i,sizeof(lpPacket->strIdAndName)-i);

						if	(GetSaveLogIDAndName(lpPacket->strIdAndName,strUserId,strUserName))
							break;
					}
				}
				else
					memcpy(&lpPacket->wLevel,((char *)&lpPacket->wLevel)+1,lpPacket->base.wSize-9);

				if	(!GetSaveLogIDAndName(lpPacket->strIdAndName,strUserId,strUserName) || lpPacket->wLevel	>	2000)
					return	FALSE;
			}

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] LV %d,EXP %d,GOLD %s",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
				dMSG_SAVE,strUserId,strUserName,lpPacket->wLevel,lpPacket->dwExperience,GetMoneyString(lpPacket->dwGold));
			break;
		}
	
		default:
			return	FALSE;
	}

	return	TRUE;
}

void
makePitchmanShopFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment,FILE *_lpFP)		// 09.08.24
{
	LOG_PITCHMAN_SHOP_TRADE	*lpPitchmanLog	=	(LOG_PITCHMAN_SHOP_TRADE	*)_lpLog;

	 

	sprintf(_lpstrReturnText,"\n[%02d%02d%02d %02d:%02d:%02d] -%s- FI[%d]\n",
		lpPitchmanLog->logTime.yy,lpPitchmanLog->logTime.mm,lpPitchmanLog->logTime.dd,
		lpPitchmanLog->logTime.hour,lpPitchmanLog->logTime.min,lpPitchmanLog->logTime.getSecond(),
		dMSG_NOJUM,lpPitchmanLog->wFieldUniqueIndex);

	fprintf(_lpFP,_lpstrReturnText);

	char* strDenomination = "Gold";
	
	if	(sizeof(LOG_PITCHMAN_SHOP_TRADE) == _lpLog->base.wSize && lpPitchmanLog->bDenominationType)
		strDenomination = "Gold Bar";

	sprintf(_lpstrReturnText,"    [%s/%s] [IP %s] FI[%d]\n        %d %s\n",lpPitchmanLog->strId,lpPitchmanLog->strName,lpPitchmanLog->strIP,lpPitchmanLog->wFieldUniqueIndex,lpPitchmanLog->bf31ItemPrice, strDenomination);
	fprintf(_lpFP,_lpstrReturnText);
	sprintf(_lpstrReturnText,"    [%s/%s] [IP %s]\n",lpPitchmanLog->strOwnerId,lpPitchmanLog->strOwnerName,lpPitchmanLog->strOwnerIP);
	fprintf(_lpFP,_lpstrReturnText);
	sprintf(_lpstrReturnText ,"        %s",((cITEM *)&lpPitchmanLog->cItem)->getName());
	strcpy(_lpstrComment,"        ");
	exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpPitchmanLog->cItem);
	fprintf(_lpFP,_lpstrReturnText);

	if	(_lpstrComment[0])
	{
		fprintf(_lpFP,_lpstrComment);
		fprintf(_lpFP,"\n");
	}

	sprintf(_lpstrReturnText," \n\n");

	_lpstrComment[0]	=	NULL;
}

void
makeItemTradeFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment,FILE *_lpFP)
{
	LOG_ITEM_TRADE	*lpTradeLog	=	(LOG_ITEM_TRADE	*)_lpLog;

	sprintf(_lpstrReturnText,"\n[%02d%02d%02d %02d:%02d:%02d] -%s- FI[%d]\n",
		lpTradeLog->logTime.yy,lpTradeLog->logTime.mm,lpTradeLog->logTime.dd,
		lpTradeLog->logTime.hour,lpTradeLog->logTime.min,lpTradeLog->logTime.getSecond(),
		dMSG_TRADE_ITEM,
		lpTradeLog->wFieldUniqueIndexRecver);

	fprintf(_lpFP,_lpstrReturnText);

	if (lpTradeLog->wTrader1ItemCount		>	dTRADE_ITEM_COUNT	||	lpTradeLog->bf15Trader2ItemCount	>	dTRADE_ITEM_COUNT	||
		lpTradeLog->wFieldUniqueIndexSender	>	2048)
	{
		sprintf(_lpstrReturnText,"    broken log!! [%s/%s] [IP %s] <-> [%s/%s] [IP %s] FI[%d]\n",
				lpTradeLog->strSenderId,lpTradeLog->strSenderName,lpTradeLog->strSenderIP,
				lpTradeLog->strRecverId,lpTradeLog->strRecverName,lpTradeLog->strRecverIP,
				lpTradeLog->wFieldUniqueIndexSender);
		fprintf(_lpFP,_lpstrReturnText);
	}
	else
	{
		sprintf(_lpstrReturnText,"    [%s/%s] [IP %s] FI[%d]\n        %s Gold\n",lpTradeLog->strSenderId,lpTradeLog->strSenderName,lpTradeLog->strSenderIP, lpTradeLog->wFieldUniqueIndexSender,GetMoneyString(lpTradeLog->dwGoldSender));
		fprintf(_lpFP,_lpstrReturnText);

		for(int i=0;i<lpTradeLog->wTrader1ItemCount;i++)
		{	
			if	(lpTradeLog->cItem[i].m_wBaseItem == 0xffff	||	lpTradeLog->cItem[i].m_wBaseItem >=	dBASIC_ITEM_COUNT)
				continue;

			sprintf(_lpstrReturnText ,"        %s",((cITEM *)&lpTradeLog->cItem[i])->getName());
			strcpy(_lpstrComment,"        ");
			exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpTradeLog->cItem[i]);

			fprintf(_lpFP,_lpstrReturnText);

			if	(_lpstrComment[0])
			{
				fprintf(_lpFP,_lpstrComment);
				fprintf(_lpFP,"\n");
			}
		}

		sprintf(_lpstrReturnText,"    [%s/%s] [IP %s] FI[%d]\n        %s Gold\n",lpTradeLog->strRecverId,lpTradeLog->strRecverName,lpTradeLog->strRecverIP, lpTradeLog->wFieldUniqueIndexRecver,GetMoneyString(lpTradeLog->dwGoldRecver));
		fprintf(_lpFP,_lpstrReturnText);

		for(i=lpTradeLog->wTrader1ItemCount;i<lpTradeLog->wTrader1ItemCount+lpTradeLog->bf15Trader2ItemCount;i++)
		{
			if	(lpTradeLog->cItem[i].m_wBaseItem == 0xffff	||	lpTradeLog->cItem[i].m_wBaseItem >=	dBASIC_ITEM_COUNT)
				continue;

			sprintf(_lpstrReturnText ,"        %s",((cITEM *)&lpTradeLog->cItem[i])->getName());
			strcpy(_lpstrComment,"        ");
			exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpTradeLog->cItem[i]);
			fprintf(_lpFP,_lpstrReturnText);

			if	(_lpstrComment[0])
			{
				fprintf(_lpFP,_lpstrComment);
				fprintf(_lpFP,"\n");
			}
		}
	}

	sprintf(_lpstrReturnText,"End TRADE\n\n");
	_lpstrComment[0]	=	NULL;
}

void
makeBankInFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment,FILE *_lpFP)
{
	int i = 0;

	LOG_ITEM_BANK_IN	*lpBankIn	=	(LOG_ITEM_BANK_IN	*)_lpLog;

	sprintf(_lpstrReturnText,"\n[%02d%02d%02d %02d:%02d:%02d] Start BANK IN [%s/%s] [IP %s] FI[%d]\n",
			lpBankIn->logTime.yy,lpBankIn->logTime.mm,lpBankIn->logTime.dd,lpBankIn->logTime.hour,lpBankIn->logTime.min,lpBankIn->logTime.getSecond(),
			lpBankIn->strId,lpBankIn->strName,lpBankIn->strIP,lpBankIn->wFieldUniqueIndex);
	fprintf(_lpFP,_lpstrReturnText);
	sprintf(_lpstrReturnText,"    GOLD : [%s]\n",GetMoneyString(lpBankIn->iGold));
	fprintf(_lpFP,_lpstrReturnText);

	for(i = 0;i<min(lpBankIn->bf15Count,dMAX_STORE_ITEM_COUNT_BY_LOG);i++)
	{
		if	(lpBankIn->cItem[i].m_wBaseItem	==	0xffff || 
			 lpBankIn->cItem[i].m_wBaseItem	>=	dBASIC_ITEM_COUNT
			)
			continue;

		sprintf(_lpstrReturnText,"    %s",((cITEM *)&lpBankIn->cItem[i])->getName());
		strcpy(_lpstrComment,"    ");
		exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpBankIn->cItem[i]);
		fprintf(_lpFP,_lpstrReturnText);

		if (_lpstrComment[0])
		{
			fprintf(_lpFP,_lpstrComment);
			fprintf(_lpFP,"\n");
		}
	}

	sprintf(_lpstrReturnText,"End BANK IN\n\n");
	_lpstrComment[0]	=	NULL;
}

void
makeBankOutFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment,FILE *_lpFP)
{
	int i = 0;

	LOG_ITEM_BANK_OUT	*lpBankOut	=	(LOG_ITEM_BANK_OUT	*)_lpLog;

	sprintf(_lpstrReturnText,"\n[%02d%02d%02d %02d:%02d:%02d] Start BANK OUT [%s/%s] [IP %s] FI[%d]\n",
		lpBankOut->logTime.yy,lpBankOut->logTime.mm,lpBankOut->logTime.dd,lpBankOut->logTime.hour,lpBankOut->logTime.min,lpBankOut->logTime.getSecond(),
		lpBankOut->strId,lpBankOut->strName,lpBankOut->strIP,lpBankOut->wFieldUniqueIndex);
	fprintf(_lpFP,_lpstrReturnText);
	sprintf(_lpstrReturnText,"    GOLD : [%s]\n",GetMoneyString(lpBankOut->iGold));
	fprintf(_lpFP,_lpstrReturnText);

	for(i = 0;i<min(lpBankOut->bf15Count,dMAX_STORE_ITEM_COUNT_BY_LOG);i++)
	{
		if	(lpBankOut->cItem[i].m_wBaseItem	==	0xffff || lpBankOut->cItem[i].m_wBaseItem	>=	dBASIC_ITEM_COUNT)
			continue;

		sprintf(_lpstrReturnText,"    %s",((cITEM *)&lpBankOut->cItem[i])->getName());
		strcpy(_lpstrComment,"    ");
		exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpBankOut->cItem[i]);
		fprintf(_lpFP,_lpstrReturnText);

		if	(_lpstrComment[0])
		{
			fprintf(_lpFP,_lpstrComment);
			fprintf(_lpFP,"\n");
		}
	}

	sprintf(_lpstrReturnText,"End BANK OUT\n\n");
	_lpstrComment[0]	=	NULL;
}

BOOL
makeFullLogText(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment,FILE *_lpFP)
{
	int	iLogType	=	_lpLog->base.wType;

	switch(iLogType)
	{
		case	dLOG_ERROR	:
			makeErrorFullLog(_lpLog,_lpstrReturnText);
			return	TRUE;

		case	dLOG_GUILD_WORK			:
			makeGWFullLog(_lpLog,_lpstrReturnText);
			break;

		case	dLOG_GUILD_BATTLE_WORK	:
			makeGBWFullLog(_lpLog,_lpstrReturnText);
			break;

		case	dLOG_ITEM_DROP:
			makeItemDropFullLog(_lpLog,_lpstrReturnText,_lpstrComment);
			break;
		case	dLOG_PET_WORK	:
			makePetFullLog(_lpLog,_lpstrReturnText);
			break;
		case	dLOG_SYSTEM		:
			makeSystemFullLog(_lpLog,_lpstrReturnText,_lpstrComment,_lpFP);
			break;

		case dLOG_EVENT		:
			makeEventFullLog(_lpLog,_lpstrReturnText,_lpFP);
			break;

		case dLOG_ITEM_PICK	:
			makeItemPickFullLog(_lpLog,_lpstrReturnText,_lpstrComment);
			break;

		case dLOG_ITEM_DEL	:
			makeDelItemFullLog(_lpLog,_lpstrReturnText,_lpstrComment);
			break;

		case dLOG_PITCHMAN_SHOP_TRADE	:
		{
			makePitchmanShopFullLog(_lpLog,_lpstrReturnText,_lpstrComment,_lpFP);
			break;
		}

		case dLOG_ITEM_TRADE:
		{
			makeItemTradeFullLog(_lpLog,_lpstrReturnText,_lpstrComment,_lpFP);
			break;
		}

		case dLOG_ITEM_SELL:
		{
			LOG_ITEM_SELL	*lpSellLog	=	(LOG_ITEM_SELL	*)_lpLog;

			DWORD	dwPrice	=	lpSellLog->dwSellPrice;

			if	(dwPrice == 0xcccccccc)
				dwPrice	=	0;

			if	(dwPrice & 0x80000000)
			{
				dwPrice	-=	0x80000000;

				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",
					lpSellLog->logTime.yy,lpSellLog->logTime.mm,lpSellLog->logTime.dd,lpSellLog->logTime.hour,lpSellLog->logTime.min,lpSellLog->logTime.getSecond(),
					dMSG_BUY_ITEM,
					lpSellLog->strId,lpSellLog->strName,lpSellLog->strIP, 
					lpSellLog->wFieldUniqueIndex,((cITEM *)&lpSellLog->cItem)->getName());
			}
			else
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",
					lpSellLog->logTime.yy,lpSellLog->logTime.mm,lpSellLog->logTime.dd,lpSellLog->logTime.hour,lpSellLog->logTime.min,lpSellLog->logTime.getSecond(),
					lpSellLog->strId,lpSellLog->strName,lpSellLog->strIP, 
					dMSG_SELL_ITEM,
					lpSellLog->wFieldUniqueIndex,((cITEM *)&lpSellLog->cItem)->getName());

			exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpSellLog->cItem);

			fprintf(_lpFP,_lpstrReturnText);

			if	(_lpstrComment[0])
			{
				fprintf(_lpFP,_lpstrComment);
				_lpstrComment[0]	=	NULL;
			}

			sprintf(_lpstrReturnText,"    %d Gold\n",dwPrice);
			fprintf(_lpFP,_lpstrReturnText);
			strcpy(_lpstrReturnText,"\n");
			break;
		}

		case dLOG_ITEM_BANK_IN:
			makeBankInFullLog(_lpLog,_lpstrReturnText,_lpstrComment,_lpFP);
			break;

		case dLOG_ITEM_BANK_OUT:
			makeBankOutFullLog(_lpLog,_lpstrReturnText,_lpstrComment,_lpFP);
			break;

		case dLOG_ITEM_WITHDRAW_PREMIUM_ITEM:
		{
			LOG_ITEM_WITHDRAW_PREMIUM_ITEM	*lpData	=	&_lpLog->mLOG_ITEM_WITHDRAW_PREMIUM_ITEM;

			if	(lpData->wMethod	==	eWPIM_WITHDRAW_CART_ITEM	||	lpData->base.wSize	!=	sizeof(LOG_ITEM_WITHDRAW_PREMIUM_ITEM))
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] ITEMNO [%d]\n",
					lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
					dMSG_WITHDRAW_PREMIUM_ITEM,lpData->strId,lpData->strName,lpData->strIP,lpData->wFieldUniqueIndex, lpData->dwItemNo);
			}
			break;
		}

		case dLOG_ITEM_CREATE_PREMIUM_ITEM:
		{
			LOG_ITEM_CREATE_PREMIUM_ITEM	*lpData	=	(LOG_ITEM_CREATE_PREMIUM_ITEM	*)_lpLog;

			if	(lpData->wMethod	==	eCPIM_WITHDRAW_CART_ITEM	||	lpData->base.wSize	!=	sizeof(LOG_ITEM_CREATE_PREMIUM_ITEM))
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",
					lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
					dMSG_CREATE_PREMIUM_ITEM,lpData->strId,lpData->strName,lpData->strIP, 
					lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName());
			}
			else
			if	(lpData->wMethod	==	eCPIM_BUY_BY_CARROT)
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s [%d Carrot]",
					lpData->logTime.yy,lpData->logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,lpData->logTime.getSecond(),
					dMSG_CREATE_PREMIUM_ITEM_BY_CARROT,lpData->strId,lpData->strName,lpData->strIP, 
					lpData->wFieldUniqueIndex,((cITEM *)&lpData->cItem)->getName(),lpData->wCarrotPrice);
			}

			exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpData->cItem);
			break;
		}

		case dLOG_CREATE_ITEM:
		{
			return	makeCreateItemFullLog(_lpLog,_lpstrReturnText,_lpstrComment);
		}
		case dLOG_USE_ITEM:
		{
			LOG_USE_ITEM	*lpUseLog	=	&_lpLog->mLOG_USE_ITEM;

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",
				lpUseLog->logTime.yy,lpUseLog->logTime.mm,lpUseLog->logTime.dd,lpUseLog->logTime.hour,lpUseLog->logTime.min,lpUseLog->logTime.getSecond(),
				dMSG_USE_ITEM,lpUseLog->strId,lpUseLog->strName,lpUseLog->strIP, 
				lpUseLog->wFieldUniqueIndex,((cITEM *)&lpUseLog->cItem)->getName());

			exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpUseLog->cItem);
			break;
		}
		case dLOG_EXPIRED_ITEM:
		{
			LOG_EXPIRED_ITEM	*lpExpireLog	=	&_lpLog->mLOG_EXPIRED_ITEM;
			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s",
				lpExpireLog->logTime.yy,lpExpireLog->logTime.mm,lpExpireLog->logTime.dd,lpExpireLog->logTime.hour,lpExpireLog->logTime.min,lpExpireLog->logTime.getSecond(),
				dMSG_EXPIRED_ITEM,
				lpExpireLog->strId,lpExpireLog->strName,lpExpireLog->strIP, 
				lpExpireLog->wFieldUniqueIndex,((cITEM *)&lpExpireLog->cItem)->getName());
			exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpExpireLog->cItem);
			break;
		}
		case dLOG_DOUBLE_ITEM:
		{
			LOG_DOUBLE_ITEM	*lpDoubleLog	=	(LOG_DOUBLE_ITEM	*)&_lpLog->mLOG_DOUBLE_ITEM;

			char	strWhere[20];
			memset(strWhere,0,20);

			switch(lpDoubleLog->wWhere)
			{
				case eDIL_TRADE:
					strcpy(strWhere,"in TRADE");
					break;
				case eDIL_PICK:
					strcpy(strWhere,"in PICK");
					break;
				case eDIL_LOGIN:
					strcpy(strWhere,"in LOGIN");
					break;
				case eDIL_BANK:
					strcpy(strWhere,"in BANK");
					break;
			}

			sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s] FI[%d] %s %s",
				lpDoubleLog->logTime.yy,lpDoubleLog->logTime.mm,lpDoubleLog->logTime.dd,lpDoubleLog->logTime.hour,lpDoubleLog->logTime.min,lpDoubleLog->logTime.getSecond(),
				dMSG_COPY_ITEM,
				lpDoubleLog->strId,lpDoubleLog->strName,lpDoubleLog->strIP, 
				lpDoubleLog->wFieldUniqueIndex,((cITEM *)&lpDoubleLog->cItem)->getName(),strWhere);

			exportItemFullInfo(_lpstrReturnText,_lpstrComment,&lpDoubleLog->cItem);
			break;
		}

		case dLOG_ENCHANT_ITEM:
		{
			makeEnchantItemFullLog(_lpLog,_lpstrReturnText);
			exportItemFullInfo(_lpstrReturnText,_lpstrComment,&_lpLog->mLOG_ENCHANT_ITEM.cItem);
			break;
		}

		case dLOG_AVA_CREATE:
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s]\n",
					_lpLog->mLOG_AVA_CREATE.logTime.yy,_lpLog->mLOG_AVA_CREATE.logTime.mm,_lpLog->mLOG_AVA_CREATE.logTime.dd,_lpLog->mLOG_AVA_CREATE.logTime.hour,_lpLog->mLOG_AVA_CREATE.logTime.min,_lpLog->mLOG_AVA_CREATE.logTime.getSecond(),
					dMSG_CREATE_PLAYER,_lpLog->mLOG_AVA_CREATE.strId,_lpLog->mLOG_AVA_CREATE.strName,_lpLog->mLOG_AVA_CREATE.strIP);
				break;
			}
		case dLOG_AVA_DELETE:
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s]\n",
					_lpLog->mLOG_AVA_DELETE.logTime.yy,_lpLog->mLOG_AVA_DELETE.logTime.mm,_lpLog->mLOG_AVA_DELETE.logTime.dd,_lpLog->mLOG_AVA_DELETE.logTime.hour,_lpLog->mLOG_AVA_DELETE.logTime.min,_lpLog->mLOG_AVA_DELETE.logTime.getSecond(),
					dMSG_DELETE_PLAYER,_lpLog->mLOG_AVA_DELETE.strId,_lpLog->mLOG_AVA_DELETE.strName,_lpLog->mLOG_AVA_DELETE.strIP);
				break;
			}
		case dLOG_AVA_LOGIN:
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s]\n",
					_lpLog->mLOG_AVA_LOGIN.logTime.yy,_lpLog->mLOG_AVA_LOGIN.logTime.mm,_lpLog->mLOG_AVA_LOGIN.logTime.dd,_lpLog->mLOG_AVA_LOGIN.logTime.hour,_lpLog->mLOG_AVA_LOGIN.logTime.min,_lpLog->mLOG_AVA_LOGIN.logTime.getSecond(),
					dMSG_LOGIN,_lpLog->mLOG_AVA_LOGIN.strId,_lpLog->mLOG_AVA_LOGIN.strName,_lpLog->mLOG_AVA_LOGIN.strIP);
				break;
			}
		case dLOG_AVA_LOGOUT:
			{
				sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [IP %s]\n",
					_lpLog->mLOG_AVA_LOGOUT.logTime.yy,_lpLog->mLOG_AVA_LOGOUT.logTime.mm,_lpLog->mLOG_AVA_LOGOUT.logTime.dd,_lpLog->mLOG_AVA_LOGOUT.logTime.hour,_lpLog->mLOG_AVA_LOGOUT.logTime.min,_lpLog->mLOG_AVA_LOGOUT.logTime.getSecond(),
					dMSG_LOGOUT,_lpLog->mLOG_AVA_LOGOUT.strId,_lpLog->mLOG_AVA_LOGOUT.strName,_lpLog->mLOG_AVA_LOGOUT.strIP);
				break;
			}
		case dLOG_AVA_SAVE_LOG	:
			{
				LOG_AVA_SAVE_LOG	*lpPacket	=	(LOG_AVA_SAVE_LOG	*)_lpLog;

				char	strUserId[dID_LENGTH] ="";
				char	strUserName[dNAME_LENGTH] = "";

				if(GetSaveLogIDAndName2(lpPacket->strIdAndName,strUserId,strUserName))
				{		
					sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [%s/%s] [LV %d] [EXP %d] [GOLD %s] [SC %d] [FI %d]\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
						dMSG_SAVE,strUserId,strUserName,lpPacket->wLevel,lpPacket->dwExperience,GetMoneyString(lpPacket->dwGold),lpPacket->dwSaveCount,lpPacket->wField);					
				}
				else
				{
					char errStr[dID_LENGTH+dNAME_LENGTH+1]  = "";
					strncpy(errStr,lpPacket->strIdAndName,dID_LENGTH+dNAME_LENGTH);
					sprintf(_lpstrReturnText,"[%02d%02d%02d %02d:%02d:%02d] -%s- [Err %s][%s/%s] [LV %d] [EXP %d] [GOLD %s] [SC %d] [FI %d]\n",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,lpPacket->logTime.getSecond(),
						dMSG_SAVE,errStr,strUserId,strUserName,lpPacket->wLevel,lpPacket->dwExperience,GetMoneyString(lpPacket->dwGold),lpPacket->dwSaveCount,lpPacket->wField);	

				}

				break;
			}

		default:
			return	FALSE;
	}

	return	TRUE;
}