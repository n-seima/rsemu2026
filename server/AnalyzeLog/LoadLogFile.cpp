//	사실상 지워 버리려다... 혹시 또 쓸일 있을까 해서 남김.
//	그냥 냅두면 코드 지저분해 지고 검색 힘들어 져서 일루 뺌

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
#include "job.h"
#include "../buildVersion.h"
#include <io.h>
#include <process.h>
#include <atlbase.h>		//	for CRegKey
#include "quest.h"
#include "cADO_AnalyzeTool.h"
#include "CFTPclient.h"
#include "log_filtering.h"
#include "../buildVersion.h"	//	이건 공유 안하고 쓴다.
							//	없으면 파일을 생성한다.(공유 하지 말것!!)
							//	안에는
							//	#pragma	once
							//	#define	dBUILDER_NAME	"young"
							//	와 같이 선언 되어 있다. 뒤에 이름은 프로그래머 이름을 적을것


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CAnalyzeLogDlg::LoadLogFile(char * fn)
{
	FILE * fp = NULL;
	fp = fopen(fn,"rb");
	if(!fp){	MSGBOX("ERROR LOG FILE OPEN","File is not complete.");	return;	}
	DWORD	filesize = filelength(fileno(fp));
	if(filesize==0)	{	MSGBOX("ERROR LOG FILE OPEN","File is not complete. size = 0");	return;	}
	int iDelCount = m_ctlLogData.GetCount();for(int c=0;c<iDelCount;c++)	m_ctlLogData.DeleteString(0);
	allData->Reset();	searchData->Reset();

	DWORD	readsize = 0;
	ALL_LOGMSG	logMsg;
	//	초기화할 데이타
	m_dwCurListCount = m_dwSearchListCount = 0;	isSearchList = FALSE;	allData->Reset();	searchData->Reset();

	while(readsize<filesize)
	{
		int ret = 0;
		memset(&logMsg,0,sizeof(ALL_LOGMSG));
		ALL_LOGMSG	*curPos	=	&logMsg;
		ret = fread(&curPos->base.wSize,sizeof(WORD),1,fp);	if(ret != 1)	{	/*MSGBOX("ERROR LOG FILE READ","파일 읽기 오류 count = %d , type [0x%x] size[%d] readsize[%d]",m_dwCurListCount,curPos->base.wType,curPos->base.wSize,readsize);*/break;}
		ret = fread(&curPos->base.wType,sizeof(WORD),1,fp);	if(ret != 1)	{	/*MSGBOX("ERROR LOG FILE READ","파일 읽기 오류 count = %d , type [0x%x] size[%d] readsize[%d]",m_dwCurListCount,curPos->base.wType,curPos->base.wSize,readsize);*/break;}
		if(curPos->base.wType<0x21 || curPos->base.wType>0x34)
		{	MSGBOX("ERROR LOG FILE READ","%dst type [0x%x] size[%d] readsize[%d]에 문제가 발생했습니다.",m_dwCurListCount,curPos->base.wType,curPos->base.wSize,readsize);
			break;
		}
		fseek(fp,-4,SEEK_CUR);
/*		if(readsize>=2398)
		{
			ret = fread(&logMsg,curPos->base.wSize,1,fp);	if(ret != 1)	{	MSGBOX("ERROR LOG FILE READ","파일 읽기 오류 count = %d , type [0x%x] size[%d] readsize[%d]",m_dwCurListCount,curPos->base.wType,curPos->base.wSize,readsize);break;}
			readsize += curPos->base.wSize;
			char	data[63];
			memcpy(data,&logMsg,63);
			fseek(fp,-(curPos->base.wSize-1),SEEK_CUR);
			fread(&logMsg,curPos->base.wSize,1,fp);
			memcpy(&logMsg,data,63);
			fseek(fp,-1,SEEK_CUR);
		}else{
*/	
		ret = fread(&logMsg,curPos->base.wSize,1,fp);	if(ret != 1)	{	MSGBOX("ERROR LOG FILE READ","파일 읽기 오류 count = %d , type [0x%x] size[%d] readsize[%d]",m_dwCurListCount,curPos->base.wType,curPos->base.wSize,readsize);break;}
		readsize += curPos->base.wSize;
		allData->AddNode(logMsg);

		char	addText[512];
		if(curPos->base.wType<dLOG_AVA_CREATE)
		{
			switch(curPos->base.wType)
			{

			case	dLOG_PET_WORK	:
			{
				makePetLog(curPos,addText);

				break;
			}

			case dLOG_SYSTEM	:
			{
				makeSystemLog(curPos,addText);

				break;
			}

			case dLOG_EVENT	:
				{
					makeEventLog(curPos,addText);
					break;
				}

			case dLOG_GUILD_BATTLE_WORK	:
				{
					LOG_GUILD_BATTLE_WORK	*lpPacket	=	&curPos->mLOG_GUILD_BATTLE_WORK;
					
					if(lpPacket->wWork	==	eGBW_BOOKING)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s] [%d/%d]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_BOOKING_GUILD_BATTLE,lpPacket->strGuild1Name,lpPacket->bTimeOrder,lpPacket->bFieldOrder);
					}
					else
					if(lpPacket->wWork	==	eGBW_CANCEL)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_CANCEL_GUILD_BATTLE,lpPacket->strGuild1Name);
					}
					else
					if(lpPacket->wWork	==	eGBW_ENGAGE)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s] vs [%s] [%d/%d]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_ENGAGE_GUILD_BATTLE,lpPacket->strGuild1Name,lpPacket->strGuild2Name,lpPacket->bTimeOrder,lpPacket->bFieldOrder);
					}
					else
					if(lpPacket->wWork	==	eGBW_RESULT)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s:%d] vs [%s:%d]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_RESULT_OF_GUILD_BATTLE,lpPacket->strGuild1Name,lpPacket->bGuild1VictoryPoint,lpPacket->strGuild2Name,lpPacket->bGuild2VictoryPoint);
					}
					else
					if(lpPacket->wWork	==	eGBW_JOIN)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_JOIN_GUILD,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
					}
					else
					if(lpPacket->wWork	==	eGBW_LEAVE)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_LEAVE_GUILD,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
					}
					else
					if(lpPacket->wWork	==	eGBW_KILL)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_GUILD_BATTLE_KILL_ENEMY,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
					}
					else
					if(lpPacket->wWork	==	eGBW_BEGIN)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_BEGIN_GUILD_BATTLE,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
					}
					else
					if(lpPacket->wWork	==	eGBW_PRE_RESULT)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_GUILD_BATTLE_PRE_RESUT,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
					}
					else
					if(lpPacket->wWork	==	eGBW_ERROR_IN_KILL)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- ",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_GUILD_BATTLE_KILL_ERROR);
					}
					else
					if(lpPacket->wWork	==	eGBW_ERROR_IN_CHECK_RESULT)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_GUILD_BATTLE_RESULT_ERROR);
					}
					else
					if(lpPacket->wWork	==	eGBW_NEW_KILL)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s:%s]",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_GUILD_BATTLE_KILL_ENEMY,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
					}
					else
					if(lpPacket->wWork	==	eGBW_EXTERMINATE)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s:%s] winner ",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_EXTERMINATE_GUILD,lpPacket->strGuild1Name,lpPacket->strGuild2Name);
					}
					else
					if(lpPacket->wWork	==	eGW_GUILD_DUNGEON_PLAY)
					{
						LOG_S_RECORD_GUILD_DUNGEON_PLAYTIME *lpPacket = (LOG_S_RECORD_GUILD_DUNGEON_PLAYTIME*)&curPos->mLOG_GUILD_BATTLE_WORK;
						
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s][ID:%s NAME:%s IP:%s Party Member:%d] ,PlayTime:  ",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_GUILD_DUNGEON_PLAYTIME,lpPacket->strGuildName,lpPacket->strId , lpPacket->strName , lpPacket->strIP,lpPacket->wPlayTime , lpPacket->wPartyMemberCount , lpPacket->wPlayTime);
					}
					else
					if(lpPacket->wWork	==	eGW_ACTIVE_GUILD_MASTER_GRACE)
					{
						LOG_S_RECORD_GUILD_MASTER_GRACE *lpPacket = (LOG_S_RECORD_GUILD_MASTER_GRACE*)&curPos->mLOG_GUILD_BATTLE_WORK;
						
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s][ID:%s NAME:%s IP:%s] ,PlayTime:  ",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_GUILD_MASTER_GRACE,lpPacket->strGuildName,lpPacket->strId , lpPacket->strName , lpPacket->strIP,lpPacket->wPlayTime , lpPacket->wPlayTime);
					}
					else
					if(lpPacket->wWork	==	eGW_ACTIVE_GUILD_ROEN_GOOD_WILL)
					{
						LOG_S_RECORD_GUILD_ROEN_GOOD_WILL *lpPacket = (LOG_S_RECORD_GUILD_ROEN_GOOD_WILL*)&curPos->mLOG_GUILD_BATTLE_WORK;
						
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s][LAST:%s NEW:%s] ",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
							dMSG_GUILD_CHANGE_ROEN_GOOD_WILL_DATA,lpPacket->strGuildName,lpPacket->wLast,lpPacket->wNew);
					}
					break;
				}

			case dLOG_ITEM_DROP:
				{
					if (curPos->mLOG_ITEM_DROP.wMethod == 0)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -DROP- [%s(%s):%s:%d] [%s]",curPos->mLOG_ITEM_DROP.logTime.yy,curPos->mLOG_ITEM_DROP.logTime.mm,curPos->mLOG_ITEM_DROP.logTime.dd,curPos->mLOG_ITEM_DROP.logTime.hour,curPos->mLOG_ITEM_DROP.logTime.min,
							curPos->mLOG_ITEM_DROP.strId,curPos->mLOG_ITEM_DROP.strName,curPos->mLOG_ITEM_DROP.strIP, 
							curPos->mLOG_ITEM_DROP.wFieldUniqueIndex,
							((cITEM *)&curPos->mLOG_ITEM_DROP.cItem)->getName());
					}
					else
					if (curPos->mLOG_ITEM_DROP.wMethod == 1)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -DROP- [%s(%s):%s:%d] [Q] [%s]",curPos->mLOG_ITEM_DROP.logTime.yy,curPos->mLOG_ITEM_DROP.logTime.mm,curPos->mLOG_ITEM_DROP.logTime.dd,curPos->mLOG_ITEM_DROP.logTime.hour,curPos->mLOG_ITEM_DROP.logTime.min,
							curPos->mLOG_ITEM_DROP.strId,curPos->mLOG_ITEM_DROP.strName,curPos->mLOG_ITEM_DROP.strIP, 
							curPos->mLOG_ITEM_DROP.wFieldUniqueIndex,
							((cITEM *)&curPos->mLOG_ITEM_DROP.cItem)->getName());
					}
					break;
				}
			case dLOG_ITEM_PICK:
				{
					makeItemPickLog(curPos,addText);
					break;
				}
			case dLOG_ITEM_SELL:
				{
					DWORD	dwPrice	=	curPos->mLOG_ITEM_SELL.dwSellPrice;

					if (dwPrice == 0xcccccccc)
						dwPrice	=	0;

					if (dwPrice & 0x80000000)
					{
						dwPrice	-=	0x80000000;
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -BUY - [%s(%s):%s:%d] [%s] [%d Gold]",curPos->mLOG_ITEM_SELL.logTime.yy,curPos->mLOG_ITEM_SELL.logTime.mm,curPos->mLOG_ITEM_SELL.logTime.dd,curPos->mLOG_ITEM_SELL.logTime.hour,curPos->mLOG_ITEM_SELL.logTime.min,
							curPos->mLOG_ITEM_SELL.strId,curPos->mLOG_ITEM_SELL.strName,curPos->mLOG_ITEM_SELL.strIP, 
							curPos->mLOG_ITEM_SELL.wFieldUniqueIndex,
							((cITEM *)&curPos->mLOG_ITEM_SELL.cItem)->getName(),dwPrice);
					}
					else
					sprintf(addText,"[%02d%02d%02d %02d:%02d] -SELL- [%s(%s):%s:%d] [%s] [%d Gold]",curPos->mLOG_ITEM_SELL.logTime.yy,curPos->mLOG_ITEM_SELL.logTime.mm,curPos->mLOG_ITEM_SELL.logTime.dd,curPos->mLOG_ITEM_SELL.logTime.hour,curPos->mLOG_ITEM_SELL.logTime.min,
						curPos->mLOG_ITEM_SELL.strId,curPos->mLOG_ITEM_SELL.strName,curPos->mLOG_ITEM_SELL.strIP, 
						curPos->mLOG_ITEM_SELL.wFieldUniqueIndex,
						((cITEM *)&curPos->mLOG_ITEM_SELL.cItem)->getName(),dwPrice);
					break;
				}
			case dLOG_ITEM_DEL:
				{
					makeDelItemLog(curPos,addText);
					break;
				}
			case dLOG_PITCHMAN_SHOP_TRADE	:
				{
					sprintf(addText,"[%02d%02d%02d %02d:%02d] -NOJUM- [%s(%s):%s:%d] [%s]",curPos->mLOG_ITEM_DROP.logTime.yy,curPos->mLOG_ITEM_DROP.logTime.mm,curPos->mLOG_ITEM_DROP.logTime.dd,curPos->mLOG_ITEM_DROP.logTime.hour,curPos->mLOG_ITEM_DROP.logTime.min,
						curPos->mLOG_ITEM_DROP.strId,curPos->mLOG_ITEM_DROP.strName,curPos->mLOG_ITEM_DROP.strIP, 
						curPos->mLOG_ITEM_DROP.wFieldUniqueIndex,
						((cITEM *)&curPos->mLOG_ITEM_DROP.cItem)->getName());
					break;
				}

			case dLOG_ITEM_TRADE:
				{
					sprintf(addText,"[%02d%02d%02d %02d:%02d] -TRAD- [%s(%s):%s] <-> [%s(%s):%s]",curPos->mLOG_ITEM_TRADE.logTime.yy,curPos->mLOG_ITEM_DROP.logTime.mm,curPos->mLOG_ITEM_TRADE.logTime.dd,curPos->mLOG_ITEM_TRADE.logTime.hour,curPos->mLOG_ITEM_TRADE.logTime.min,
						curPos->mLOG_ITEM_TRADE.strRecverId,curPos->mLOG_ITEM_TRADE.strRecverName,curPos->mLOG_ITEM_TRADE.strRecverIP, 
						curPos->mLOG_ITEM_TRADE.strSenderId,curPos->mLOG_ITEM_TRADE.strSenderName,curPos->mLOG_ITEM_TRADE.strSenderIP);
					break;
				}
			case dLOG_ITEM_BANK_IN:
				{
					sprintf(addText,"[%02d%02d%02d %02d:%02d] -BNKI- [%s(%s):%s]",curPos->mLOG_ITEM_BANK_IN.logTime.yy,curPos->mLOG_ITEM_DROP.logTime.mm,curPos->mLOG_ITEM_BANK_IN.logTime.dd,curPos->mLOG_ITEM_BANK_IN.logTime.hour,curPos->mLOG_ITEM_BANK_IN.logTime.min,
						curPos->mLOG_ITEM_BANK_IN.strId,curPos->mLOG_ITEM_BANK_IN.strName,curPos->mLOG_ITEM_BANK_IN.strIP);
					break;
				}
			case dLOG_ITEM_BANK_OUT:
				{
					sprintf(addText,"[%02d%02d%02d %02d:%02d] -BNKO- [%s(%s):%s]",curPos->mLOG_ITEM_BANK_OUT.logTime.yy,curPos->mLOG_ITEM_DROP.logTime.mm,curPos->mLOG_ITEM_BANK_OUT.logTime.dd,curPos->mLOG_ITEM_BANK_OUT.logTime.hour,curPos->mLOG_ITEM_BANK_OUT.logTime.min,
						curPos->mLOG_ITEM_BANK_OUT.strId,curPos->mLOG_ITEM_BANK_OUT.strName,curPos->mLOG_ITEM_BANK_OUT.strIP);
					break;
				}
			case dLOG_ITEM_WITHDRAW_PREMIUM_ITEM:
				{
					LOG_ITEM_WITHDRAW_PREMIUM_ITEM	*lpData	=	&curPos->mLOG_ITEM_WITHDRAW_PREMIUM_ITEM;

					if (lpData->wMethod	==	eWPIM_WITHDRAW_CART_ITEM	||	lpData->base.wSize	!=	sizeof(LOG_ITEM_WITHDRAW_PREMIUM_ITEM))
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s(%s):%s] Item%d %d",lpData->logTime.yy,curPos->mLOG_ITEM_DROP.logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,
							dMSG_WITHDRAW_PREMIUM_ITEM,lpData->strId,lpData->strName,lpData->strIP,lpData->dwItemNo,lpData->dwSqnNumber);
					}
					break;
				}
			case dLOG_ITEM_CREATE_PREMIUM_ITEM:
				{
					LOG_ITEM_CREATE_PREMIUM_ITEM	*lpData	=	&curPos->mLOG_ITEM_CREATE_PREMIUM_ITEM;

					if (lpData->wMethod	==	eCPIM_WITHDRAW_CART_ITEM	||	lpData->base.wSize	!=	sizeof(LOG_ITEM_CREATE_PREMIUM_ITEM))
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s(%s):%s] %d:%s",lpData->logTime.yy,curPos->mLOG_ITEM_DROP.logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,
							dMSG_CREATE_PREMIUM_ITEM,lpData->strId,lpData->strName,lpData->strIP,
							lpData->cItem.m_dwSerial,	((cITEM *)&lpData->cItem)->getName()
							);
					}
					else
					if (lpData->wMethod	==	eCPIM_BUY_BY_CARROT)
					{
						sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s(%s):%s] %d:%s [%d Carrot]",lpData->logTime.yy,curPos->mLOG_ITEM_DROP.logTime.mm,lpData->logTime.dd,lpData->logTime.hour,lpData->logTime.min,
							dMSG_CREATE_PREMIUM_ITEM_BY_CARROT,lpData->strId,lpData->strName,lpData->strIP,
							lpData->cItem.m_dwSerial,	((cITEM *)&lpData->cItem)->getName(),lpData->wCarrotPrice
							);
					}

					break;
				}
			case dLOG_CREATE_ITEM:
				{
					if (curPos->mLOG_CREATE_ITEM.base.wSize	!=	sizeof(LOG_CREATE_ITEM))	//	옛날꺼
						curPos->mLOG_CREATE_ITEM.wMethod	=	eCIM_OLD;

					if	(!makeCreateItemLog(curPos,addText))
						break;

					break;
				}
			case dLOG_USE_ITEM:
				{
					sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s(%s):%s] [%s]",curPos->mLOG_USE_ITEM.logTime.yy,curPos->mLOG_ITEM_DROP.logTime.mm,curPos->mLOG_USE_ITEM.logTime.dd,curPos->mLOG_USE_ITEM.logTime.hour,curPos->mLOG_USE_ITEM.logTime.min,
						dMSG_USE_ITEM,curPos->mLOG_USE_ITEM.strId,curPos->mLOG_USE_ITEM.strName,curPos->mLOG_USE_ITEM.strIP,((cITEM *)&curPos->mLOG_USE_ITEM.cItem)->getName());
					break;
				}
			case dLOG_EXPIRED_ITEM:
				{
					sprintf(addText,"[%02d%02d%02d %02d:%02d] -EXPI- [%s(%s):%s] [%s]",curPos->mLOG_EXPIRED_ITEM.logTime.yy,curPos->mLOG_ITEM_DROP.logTime.mm,curPos->mLOG_EXPIRED_ITEM.logTime.dd,curPos->mLOG_EXPIRED_ITEM.logTime.hour,curPos->mLOG_EXPIRED_ITEM.logTime.min,
						curPos->mLOG_EXPIRED_ITEM.strId,curPos->mLOG_EXPIRED_ITEM.strName,curPos->mLOG_EXPIRED_ITEM.strIP,((cITEM *)&curPos->mLOG_EXPIRED_ITEM.cItem)->getName());
					break;
				}
			case dLOG_DOUBLE_ITEM:
				{
					char	strWhere[20];	memset(strWhere,0,20);
					switch(curPos->mLOG_DOUBLE_ITEM.wWhere)
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

					sprintf(addText,"[%02d%02d%02d %02d:%02d] -COPY- [%s(%s):%s] [%s] %s",curPos->mLOG_DOUBLE_ITEM.logTime.yy,curPos->mLOG_ITEM_DROP.logTime.mm,curPos->mLOG_DOUBLE_ITEM.logTime.dd,curPos->mLOG_DOUBLE_ITEM.logTime.hour,curPos->mLOG_DOUBLE_ITEM.logTime.min,
						curPos->mLOG_DOUBLE_ITEM.strId,curPos->mLOG_DOUBLE_ITEM.strName,curPos->mLOG_DOUBLE_ITEM.strIP,((cITEM *)&curPos->mLOG_DOUBLE_ITEM.cItem)->getName(),strWhere);
					break;
				}
			case dLOG_ENCHANT_ITEM:
				{
					makeEnchantItemLog(curPos,addText);
					break;
				}

			default:
				continue;
			}
		}else{
			switch(curPos->base.wType)
			{
			case dLOG_AVA_CREATE:
				{
					sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s/%s:%s]",curPos->mLOG_AVA_CREATE.logTime.yy,curPos->mLOG_AVA_CREATE.logTime.mm,curPos->mLOG_AVA_CREATE.logTime.dd,curPos->mLOG_AVA_CREATE.logTime.hour,curPos->mLOG_AVA_CREATE.logTime.min,
						dMSG_CREATE_PLAYER,curPos->mLOG_AVA_CREATE.strId,curPos->mLOG_AVA_CREATE.strName,curPos->mLOG_AVA_CREATE.strIP);
					break;
				}
			case dLOG_AVA_DELETE:
				{
					sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s/%s:%s]",curPos->mLOG_AVA_DELETE.logTime.yy,curPos->mLOG_AVA_DELETE.logTime.mm,curPos->mLOG_AVA_DELETE.logTime.dd,curPos->mLOG_AVA_DELETE.logTime.hour,curPos->mLOG_AVA_DELETE.logTime.min,
						dMSG_DELETE_PLAYER,curPos->mLOG_AVA_DELETE.strId,curPos->mLOG_AVA_DELETE.strName,curPos->mLOG_AVA_DELETE.strIP);
					break;
				}
			case dLOG_AVA_LOGIN:
				{
					sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s/%s:%s]",curPos->mLOG_AVA_LOGIN.logTime.yy,curPos->mLOG_AVA_LOGIN.logTime.mm,curPos->mLOG_AVA_LOGIN.logTime.dd,curPos->mLOG_AVA_LOGIN.logTime.hour,curPos->mLOG_AVA_LOGIN.logTime.min,
						dMSG_LOGIN,curPos->mLOG_AVA_LOGIN.strId,curPos->mLOG_AVA_LOGIN.strName,curPos->mLOG_AVA_LOGIN.strIP);
					break;
				}
			case dLOG_AVA_LOGOUT:
				{
					sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s/%s:%s]",curPos->mLOG_AVA_LOGOUT.logTime.yy,curPos->mLOG_AVA_LOGOUT.logTime.mm,curPos->mLOG_AVA_LOGOUT.logTime.dd,curPos->mLOG_AVA_LOGOUT.logTime.hour,curPos->mLOG_AVA_LOGOUT.logTime.min,
						dMSG_LOGOUT,curPos->mLOG_AVA_LOGOUT.strId,curPos->mLOG_AVA_LOGOUT.strName,curPos->mLOG_AVA_LOGOUT.strIP);
					break;
				}
			case dLOG_AVA_SAVE_LOG:
				{
					LOG_AVA_SAVE_LOG	*lpPacket	=	(LOG_AVA_SAVE_LOG	*)curPos;

					char	strUserId[dID_LENGTH],strUserName[dNAME_LENGTH];

					GetSaveLogIDAndName(lpPacket->strIdAndName,strUserId,strUserName);

					sprintf(addText,"[%02d%02d%02d %02d:%02d] -%s- [%s/%s] LV %d,EXP %d",lpPacket->logTime.yy,lpPacket->logTime.mm,lpPacket->logTime.dd,lpPacket->logTime.hour,lpPacket->logTime.min,
						dMSG_SAVE,strUserId,strUserName,lpPacket->wLevel,lpPacket->dwExperience);
					break;
				}
			default:
				continue;
			}
		}
		m_ctlLogData.InsertString(m_dwCurListCount,addText);
		m_dwCurListCount++;
		Sleep(1);
	}
	MSGBOX("결과","총 [%d]개의 데이타가 로그되었습니다.",m_dwCurListCount);
}
