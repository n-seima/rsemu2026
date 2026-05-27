#include "stdafx.h"
#include "admintool.h"
#include "ServerTool.h"
#include "CMessage.h"
#include "CItem.h"
#include "CDecoder.h"
#include "cPACKET_WORLDSERVER.h"
#include "RenewItemExpireDate.h"
#include "cSRVUTIL.h"
#include "characterInfo.h"
#include "quest.h"
#include "ExpCalc.h"

void CServerTool::On050818()
{
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

	char		*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};
	char		strCurrentServer[20]		=	"";
	cDECODER	decoder;

	cFILE		file,fileError;

	SetLogFolder();

	if	(!decoder.Upload("_text/퀘스트 초기화 유저 명단.txt",NULL))
	{
		ERRMSG("error","'_text/퀘스트 초기화 유저 명단.txt' 파일이 없어요");
		return;
	}

	file.Open("_text/05.08.22 퀘스트 리셋 유저 정보.txt","wb",TRUE);
	fileError.Open("_text/05.08.22 퀘스트 리셋 유저 정보[추출 에러!!].txt","wb",TRUE);

	while(1)
	{
		char	*lpstrText	=	decoder.GetWord();

		if	(stricmp(lpstrText,"end")	==	0)
			break;

		if	(stricmp(lpstrText,strCurrentServer)	!=	0)
		{
			strcpy(strCurrentServer,lpstrText);

			int	iCurServer	=	-1;

			for	(int	iServer=0;iServer<dMAX_WORLD_SERVER_COUNT;iServer++)
			{
				if	(stricmp(sList[iServer].strName,lpstrText)	==	0)
				{
					iCurServer	=	iServer;
					break;
				}
			}

			if	(iCurServer	==	-	1	||	!m_db.Connect(	sList[iCurServer].strDBIP	,strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source_2",1))
			{
				MessageBox(g_strToolMessage[50]);
				return;
			}
		}

		lpstrText	=	decoder.GetStream();

		CAvatarManager	m_am;

		if	(!m_db.loadData(NULL,lpstrText,&m_am))
		{
			fileError.writeStreamCR("find error!! : %s/%s",strCurrentServer,lpstrText);
			continue;
		}

		if	(m_am.m_iLoadDataCount	<=	0)
		{
			fileError.writeStreamCR("find error!! : %s/%s",strCurrentServer,lpstrText);
			continue;
		}

		CAvatarData	*lpPlayer	=	&m_am.m_aData[0];

		file.writeStreamCR("player	\"%s\"	\"%s\"",strCurrentServer,lpPlayer->m_strName);

		int	iOwnQuestCount		=	0;

		for	(int iSlot=0;iSlot<dMAX_PROCESS_QUEST_COUNT;iSlot++)
		{
			tsProcessQuestField*	lpQuestInfo		=	&lpPlayer->m_aProcessQuest[iSlot];

			if	(lpQuestInfo->isActive	==	FALSE)
				continue;

			CQuestItem	*lpQuest	=	GetQuest(lpQuestInfo->questKind,lpQuestInfo->questIndex);

			if	(!lpQuest)
			{
				lpQuestInfo->isActive	=	FALSE;

				fileError.writeStreamCR("invalidQuest : %s/%s",strCurrentServer,lpPlayer->m_strName);
				continue;
			}

			iOwnQuestCount++;

			file.writeStreamCR("	quest	\"%s\" %d,%d,%d,%d,%d,%d,%d",lpQuest->m_strName,lpQuestInfo->isActive,lpQuestInfo->questKind,lpQuestInfo->questProcessLevel,lpQuestInfo->questIndex,lpQuestInfo->questValue,lpQuestInfo->isComplete,lpQuestInfo->countdownStatus);

			int	iCount	=	0;

			while(1)
			{
				cITEM	*lpQuestItem	=	lpPlayer->findQuestItem(lpQuestInfo->questKind,lpQuestInfo->questIndex,iCount);

				iCount++;

				if	(!lpQuestItem)
					break;
				
				file.writeStreamCR("		item [%d:%d] [%d,%d] [%d,%d] <%d:%d,%d><%d:%d,%d><%d:%d,%d> %d",
									lpQuestItem->m_wBaseItem,lpQuestItem->m_dwSerial,lpQuestItem->m_bCount,lpQuestItem->m_bDurability,
									lpQuestItem->m_aOption[0],lpQuestItem->m_aOption[1],
									lpQuestItem->m_aPrefix[0].m_wPrefix,lpQuestItem->m_aPrefix[0].m_aValue[0],lpQuestItem->m_aPrefix[0].m_aValue[1],
									lpQuestItem->m_aPrefix[1].m_wPrefix,lpQuestItem->m_aPrefix[1].m_aValue[0],lpQuestItem->m_aPrefix[1].m_aValue[1],
									lpQuestItem->m_aPrefix[2].m_wPrefix,lpQuestItem->m_aPrefix[2].m_aValue[0],lpQuestItem->m_aPrefix[2].m_aValue[1],
									lpQuestItem->getBitFieldValue());
			}
		}

		if	(iOwnQuestCount	==	0)
			file.writeStreamCR("questCountZero",strCurrentServer,lpPlayer->m_strName);

		file.writeStreamCR(" ");
	}
	file.writeStreamCR("");
	file.writeStreamCR("end");

	file.Close();
	fileError.Close();

	MessageBox("추출 완료!");
}

void CServerTool::On50819() 
{
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

	char		*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};
	char		strCurrentServer[20]		=	"";
	cDECODER	decoder;

	cFILE		file,fileError;

	SetLogFolder();

	if	(!decoder.Upload("_text/05.08.22 퀘스트 리셋 유저 정보.txt",NULL))
	{
		ERRMSG("error","'_text/05.08.22 퀘스트 리셋 유저 정보.txt' 파일이 없어요");
		return;
	}

	file.Open("_text/05.08.22 퀘스트 리셋 유저 처리 결과.txt","wb",TRUE);
	fileError.Open("_text/05.08.22 퀘스트 리셋 유저 처리 결과[에러!!].txt","wb",TRUE);

	CAvatarManager		m_am;
	CAvatarData			*lpPlayer	=	NULL;
	char				strQuestName[64];
	BOOL				bResult;
	tsProcessQuestField	questInfo;
	BOOL				bIsCompleteQuest;

	while(1)
	{
		char	*lpstrText	=	decoder.GetWord();

		if	(stricmp(lpstrText,"end")	==	0)
		{
			if	(lpPlayer)
			{
				lpPlayer->m_dwSaveCount++;

				file.writeStreamCR("complete \"%s\" \"%s\"",strCurrentServer,lpPlayer->m_strName);
				file.writeStreamCR(" ");
				file.writeStreamCR("end");
				fileError.writeStreamCR(" ");
				fileError.writeStreamCR("end");

				if	(!m_db.saveData((cP_PLAYER_SAVE_DATA *)lpPlayer))
					fileError.writeStreamCR("dbSaveError \"%s\" \"%s\"",strCurrentServer,lpPlayer->m_strName);
			}
			break;
		}

		if	(stricmp(lpstrText,"player")	==	0)
		{
			if	(lpPlayer)
			{
				lpPlayer->m_dwSaveCount++;

				file.writeStreamCR("complete \"%s\" \"%s\"",strCurrentServer,lpPlayer->m_strName);

				if	(!m_db.saveData((cP_PLAYER_SAVE_DATA *)lpPlayer))
					fileError.writeStreamCR("dbSaveError \"%s\" \"%s\"",strCurrentServer,lpPlayer->m_strName);
			}
			
			lpstrText	=	decoder.GetStream();

			if	(stricmp(lpstrText,strCurrentServer)	!=	0)
			{
				strcpy(strCurrentServer,lpstrText);

				int	iCurServer	=	-1;

				for	(int	iServer=0;iServer<dMAX_WORLD_SERVER_COUNT;iServer++)
				{
					if	(stricmp(sList[iServer].strName,lpstrText)	==	0)
					{
						iCurServer	=	iServer;
						break;
					}
				}

				if	(iCurServer	==	-	1	||	!m_db.Connect(	sList[iCurServer].strDBIP	,strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
				{
					MessageBox(g_strToolMessage[50]);
					return;
				}
			}

			lpstrText	=	decoder.GetStream();

			if	(!m_db.loadData(NULL,lpstrText,&m_am))
			{
				fileError.writeStreamCR("findError \"%s\" \"%s\"",strCurrentServer,lpstrText);
				lpPlayer	=	NULL;
				continue;
			}

			if	(m_am.m_iLoadDataCount	<=	0)
			{
				fileError.writeStreamCR("findError \"%s\" \"%s\"",strCurrentServer,lpstrText);
				lpPlayer	=	NULL;
				continue;
			}

			lpPlayer	=	&m_am.m_aData[0];

			continue;
		}

		if	(stricmp(lpstrText,"quest")	==	0)
		{
			bIsCompleteQuest	=	FALSE;
			lpstrText			=	decoder.GetStream();

			strcpy(strQuestName,lpstrText);

			questInfo.isActive			=	decoder.GetNumber(&bResult);
			questInfo.questKind			=	decoder.GetNumber(&bResult);
			questInfo.questProcessLevel	=	decoder.GetNumber(&bResult);
			questInfo.questIndex		=	decoder.GetNumber(&bResult);
			questInfo.questValue		=	decoder.GetNumber(&bResult);
			questInfo.isComplete		=	decoder.GetNumber(&bResult);
			questInfo.countdownStatus	=	decoder.GetNumber(&bResult);

			if	(lpPlayer	==	NULL)
				continue;

			bIsCompleteQuest			=	lpPlayer->isCompleteQuest(questInfo.questKind,questInfo.questIndex);

			if	(bIsCompleteQuest)
			{
				fileError.writeStreamCR("alreadyCompleteQuest \"%s\" \"%s\" \"%s\"",strCurrentServer,lpPlayer->m_strName,strQuestName);
				continue;
			}

			tsProcessQuestField	*lpExistQuest	=	lpPlayer->getQuest(questInfo.questKind,questInfo.questIndex);

			if	(lpExistQuest)
			{
				if	(lpExistQuest->questProcessLevel	>=	questInfo.questProcessLevel)
				{
					fileError.writeStreamCR("existSameQuest \"%s\" \"%s\" \"%s\"",strCurrentServer,lpPlayer->m_strName,strQuestName);
					lpPlayer	=	NULL;
					continue;
				}

				lpPlayer->removeQuest(questInfo.questKind,questInfo.questIndex);
			}

			tsProcessQuestField	*lpQuestSlot	=	lpPlayer->getValidQuest();

			if	(!lpQuestSlot)
			{
				fileError.writeStreamCR("canNotFindValidQeustSlot	\"%s\" \"%s\" \"%s\"",strCurrentServer,lpPlayer->m_strName,strQuestName);
				lpPlayer	=	NULL;
				continue;
			}

			memcpy(lpQuestSlot,&questInfo,sizeof(questInfo));

			continue;
		}

		if	(stricmp(lpstrText,"item")	==	0)
		{
			cITEM	item;

			item.m_wBaseItem				=	(WORD)decoder.GetNumber(&bResult);
			item.m_dwSerial					=	(DWORD)decoder.GetNumber(&bResult);
			item.m_bCount					=	(BYTE)decoder.GetNumber(&bResult);
			item.m_bDurability				=	(BYTE)decoder.GetNumber(&bResult);

			item.m_aOption[0]				=	(BYTE)decoder.GetNumber(&bResult);
			item.m_aOption[1]				=	(BYTE)decoder.GetNumber(&bResult);

			item.m_aPrefix[0].m_wPrefix		=	(WORD)decoder.GetNumber(&bResult);
			item.m_aPrefix[0].m_aValue[0]	=	(BYTE)decoder.GetNumber(&bResult);
			item.m_aPrefix[0].m_aValue[1]	=	(BYTE)decoder.GetNumber(&bResult);

			item.m_aPrefix[1].m_wPrefix		=	(WORD)decoder.GetNumber(&bResult);
			item.m_aPrefix[1].m_aValue[0]	=	(BYTE)decoder.GetNumber(&bResult);
			item.m_aPrefix[1].m_aValue[1]	=	(BYTE)decoder.GetNumber(&bResult);

			item.m_aPrefix[2].m_wPrefix		=	(WORD)decoder.GetNumber(&bResult);
			item.m_aPrefix[2].m_aValue[0]	=	(BYTE)decoder.GetNumber(&bResult);
			item.m_aPrefix[2].m_aValue[1]	=	(BYTE)decoder.GetNumber(&bResult);

			item.setBitFieldValue(decoder.GetNumber(&bResult));

			if	(!lpPlayer	||	bIsCompleteQuest)
				continue;

			cITEM	*lpValidSlot			=	lpPlayer->findValidItemSlot();

			if	(lpValidSlot)
			{
				memcpy(lpValidSlot,&item,sizeof(item));
				continue;
			}

			fileError.writeStreamCR("inventoryFull	\"%s\" \"%s\" \"%s\"",strCurrentServer,lpPlayer->m_strName,strQuestName);
			lpPlayer	=	NULL;

			continue;
		}

		if	(stricmp(lpstrText,"questCountZero")	==	0)
		{
			fileError.writeStreamCR("questCountZero	\"%s\" \"%s\" \"%s\"",strCurrentServer,lpPlayer->m_strName,strQuestName);
			lpPlayer	=	NULL;
			continue;
		}
		break;
	}

	file.Close();
	fileError.Close();

	MessageBox("처리 완료!");
}
