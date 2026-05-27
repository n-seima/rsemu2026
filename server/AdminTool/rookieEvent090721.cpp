// ServerTool.cpp : implementation file
//

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
#include "cGuildDefine.h"
#include "assert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void
CServerTool::write090720Info(c090720RookieEventInfo *_lpInfo,cFILE *_lpLogFile)
{
	_lpLogFile->writeStreamCR("%s	%s	%s	%s	%s	%s	%d	%d",
						_lpInfo->m_strRookieID,_lpInfo->m_strCallerID,
						_lpInfo->m_strReceiveRookieName,GetServerNameForEvent(_lpInfo->m_iReceiveRookieServer),
						_lpInfo->m_strReceiveCallerName,GetServerNameForEvent(_lpInfo->m_iReceiveCallerServer),
						_lpInfo->m_iReceivedTimeRookieLevelForRookie,_lpInfo->m_iReceivedTimeRookieLevelForCaller);
}

char*
CServerTool::GetMaxLevelActor(int *_lpiServerIndex,int *_lpiMaxLevel,char *_lpstrId,int *_lpiResult)
{
	static	char	strName[dNAME_LENGTH];

	*_lpiMaxLevel	=	0;
	*_lpiServerIndex=	-1;

	for	(int iServer=0;iServer<10;iServer++)
	{
		if	(stricmp(GetServerNameForEvent(iServer),"end")==0)
			break;

		if	(!m_aDb[iServer].loadData(_lpstrId,NULL,&m_commonAm))
		{
			*_lpiResult	=	eCanNotFindID;
			return	NULL;
		}

		for (int iSlot=0;iSlot<m_commonAm.m_iLoadDataCount;iSlot++)
		{
			if	(m_commonAm.m_aData[iSlot].m_iLevel	>	*_lpiMaxLevel)
			{
				*_lpiServerIndex	=	iServer;
				*_lpiMaxLevel		=	m_commonAm.m_aData[iSlot].m_iLevel;

				strcpy(strName,m_commonAm.m_aData[iSlot].m_strName);
			}
		}
	}

	*_lpiResult	=	eResult_OK;

	if	(*_lpiMaxLevel	==	0)
		return	NULL;

	return	strName;
}

int
CServerTool::GetCurrentLevelForEvent090720(int _iLevel)
{
	int	iCurrentCheckLevel	=	-1;

	for (int iLevelGrade=0;;iLevelGrade++)
	{
		if	(_iLevel	>=	m_rookieEventProcessInfo.m_aiCheckLevel[iLevelGrade])
		{
			iCurrentCheckLevel	=	m_rookieEventProcessInfo.m_aiCheckLevel[iLevelGrade];
			break;
		}
	}

	return	iCurrentCheckLevel;
}

BOOL
CServerTool::read090720RookieEventExistText(char *_lpstrFileName,c090720RookieEventInfo *_lpList,int _iMaxCount,cFILE *_lpLogFile) 
{
	cDECODER	decoder;

	if	(!decoder.Upload(_ms("%s.txt",_lpstrFileName),NULL))
	{
		cMSG::Error("","[%s] 파일 없어!!",_lpstrFileName);

		return	FALSE;
	}

	int		iErrorCount	=	0;

	m_rookieEventProcessInfo.m_iTargetCount				=	0;
	m_rookieEventProcessInfo.m_iInvalidCharacterCount	=	0;

	//	기존 문서 로딩
	for(;;m_rookieEventProcessInfo.m_iTargetCount++)
	{
		c090720RookieEventInfo	*lpCurrent	=	&_lpList[m_rookieEventProcessInfo.m_iTargetCount];

		char	*lpstrWord;

		//	신규 계정
		{
			lpstrWord	=	decoder.GetStream(9,10);

			if	(!lpstrWord)
				break;

			strcpy(lpCurrent->m_strRookieID,lpstrWord);
		}

		//	추천인 계정
		{
			lpstrWord	=	decoder.GetStream(9,10);

			if	(lpstrWord)
				strcpy(lpCurrent->m_strCallerID,lpstrWord);

			if	(decoder.getLastSymbol()	==	10)
				continue;
		}

		//	지급 캐릭터(신규) 이름
		{
			lpstrWord	=	decoder.GetStream(9,10);

			if	(lpstrWord)
				strcpy(lpCurrent->m_strReceiveRookieName,lpstrWord);
			else
			if	(lpCurrent->m_strCallerID[0])
				m_rookieEventProcessInfo.m_iInvalidCharacterCount++;

			if	(decoder.getLastSymbol()	==	10)
				continue;
		}

		//	지급 캐릭터(신규) 서버
		{
			lpstrWord	=	decoder.GetStream(9,10);	//	

			if	(lpstrWord)
			{
				lpCurrent->m_iReceiveRookieServer	=	GetServerIndexForEvent(lpstrWord);

				if	(lpCurrent->m_iReceiveRookieServer	==	-1)
				{
					_lpLogFile->writeStreamCR("%s 추천인 서버 이름 이상 [%s]",lpCurrent->m_strRookieID,lpstrWord);
					iErrorCount++;
				}
			}

			if	(decoder.getLastSymbol()	==	10)
				continue;
		}

		//	지급 캐릭터(추천인) 이름
		{
			lpstrWord	=	decoder.GetStream(9,10);

			if	(lpstrWord)
				strcpy(lpCurrent->m_strReceiveCallerName,lpstrWord);
			else
				m_rookieEventProcessInfo.m_iInvalidCharacterCount++;

			if	(decoder.getLastSymbol()	==	10)
				continue;
		}

		//	지급 캐릭터(추천인) 서버
		{
			lpstrWord	=	decoder.GetStream(9,10);	//	

			if	(lpstrWord)
			{
				lpCurrent->m_iReceiveCallerServer	=	GetServerIndexForEvent(lpstrWord);

				if	(lpCurrent->m_iReceiveCallerServer	==	-1)
				{
					_lpLogFile->writeStreamCR("%s:%s 지급인 서버 이름 이상 [%s]",lpCurrent->m_strRookieID,lpCurrent->m_strCallerID,lpstrWord);
					iErrorCount++;
				}
			}

			if	(decoder.getLastSymbol()	==	10)
				continue;
		}

		//	지급 레벨(신규)
		{
			lpstrWord	=	decoder.GetStream(9,10);	//
			
			if	(lpstrWord)
				lpCurrent->m_iReceivedTimeRookieLevelForRookie	=	atoi(lpstrWord);
		}

		//	지급 레벨(추천인)
		{
			lpstrWord	=	decoder.GetStream(9,10);	//

			if	(lpstrWord)
				lpCurrent->m_iReceivedTimeRookieLevelForCaller	=	atoi(lpstrWord);
		}
	}

	if	(iErrorCount)
	{
		cMSG::Error("문서 로딩 에러!!","문서 로딩중 %d개의 문제가 발견되었습니다.\n\n%s_로그.txt 파일을 확인해 주세요.",iErrorCount,_lpstrFileName);

		return	FALSE;
	}

	return	TRUE;
}

BOOL
CServerTool::build090720RookieEvent(char *_lpstrFileName,c090720RookieEventInfo *_lpList,cFILE *_lpLogFile)
{
	cFILE	exportFile;

	exportFile.Open(_ms("%s_빌드.txt",_lpstrFileName),"wb",TRUE);
	exportFile.writeStreamCR("신규 가입	추천인	지급 캐릭터(신규)	서버(신규)	지급 캐릭터(추천인)	서버(추천인)	지급된 레벨(신규)	지급된 레벨(추천인)");

	DWORD	dwBeginTime		=	timeGetTime();
	DWORD	dwTotalPastTime	=	0;
	int		iAverageTime	=	0;
	int		iAverageTimeZeroCount	=	0;
	int		iSerchCount		=	0;
	int		iErrorCount		=	0;

	//	기존 문서 로딩
	for(int iIndex=0;iIndex<m_rookieEventProcessInfo.m_iTargetCount;iIndex++)
	{
		c090720RookieEventInfo	*lpCurrent	=	&_lpList[iIndex];
		int		iHitCount	=	0;

		if	(lpCurrent->m_strReceiveRookieName[0]	==	NULL)
		{
			int		iServer,iTopLevel,iResult;

			char*	lpstrName	=	GetMaxLevelActor(&iServer,&iTopLevel,lpCurrent->m_strRookieID,&iResult);

			iHitCount++;
			iSerchCount++;

			switch(iResult)
			{
				case	eCanNotConnectServer	:
					iErrorCount	++;
					_lpLogFile->writeStreamCR("[%s] 서버 연결 실패",lpCurrent->m_strRookieID);
					continue;
				case	eCanNotFindID			:
					iErrorCount	++;
					_lpLogFile->writeStreamCR("[%s] 신규 계정 검색 실패",lpCurrent->m_strRookieID);
					write090720Info(lpCurrent,&exportFile);
					continue;
			}

			if	(GetCurrentLevelForEvent090720(iTopLevel)	==	0)
			{
				Printf("#%4d/%d [%8s:%20s:%18s] [%8s:%20s:%18s] ???분 ??초\n",iIndex,m_rookieEventProcessInfo.m_iTargetCount,
							GetServerNameForEvent(lpCurrent->m_iReceiveRookieServer),lpCurrent->m_strRookieID,lpCurrent->m_strReceiveRookieName,
							GetServerNameForEvent(lpCurrent->m_iReceiveCallerServer),lpCurrent->m_strCallerID,lpCurrent->m_strReceiveCallerName);

				write090720Info(lpCurrent,&exportFile);

				continue;
			}

			if	(iTopLevel)
			{
				strcpy(lpCurrent->m_strReceiveRookieName,lpstrName);
				lpCurrent->m_iReceiveRookieServer	=	iServer;
			}
		}

		if	(lpCurrent->m_strCallerID[0] && strlen(lpCurrent->m_strCallerID) >= 4 && lpCurrent->m_strReceiveCallerName[0] == NULL)
		{
			int		iServer,iTopLevel,iResult;
			char*	lpstrName	=	GetMaxLevelActor(&iServer,&iTopLevel,lpCurrent->m_strCallerID,&iResult);

			iHitCount++;
			iSerchCount++;

			switch(iResult)
			{
				case	eCanNotConnectServer	:
					iErrorCount	++;
					_lpLogFile->writeStreamCR("[%s] 서버 연결 실패",lpCurrent->m_strRookieID);
					continue;
				case	eCanNotFindID			:
				{
					iErrorCount++;
					_lpLogFile->writeStreamCR("[%s][%s] 추천자 계정 검색 실패",lpCurrent->m_strRookieID,lpCurrent->m_strCallerID);
					write090720Info(lpCurrent,&exportFile);
					continue;
				}
			}

			if	(iTopLevel)
			{
				strcpy(lpCurrent->m_strReceiveCallerName,lpstrName);
				lpCurrent->m_iReceiveCallerServer	=	iServer;
			}
		}

		int	iPastTime	=	timeGetTime()-dwBeginTime;
		int	iRemainTime	=	0;

		if	(iSerchCount)
		{
			iAverageTime	=	iPastTime/iSerchCount;

			LONGLONG	llRemainCount=	(m_rookieEventProcessInfo.m_iInvalidCharacterCount-iSerchCount);

			llRemainCount*=	iAverageTime;
			iRemainTime	=	llRemainCount/1000;
		}

		Printf("#%4d/%d [%8s:%20s:%18s] [%8s:%20s:%18s] %3d분 %.2d초 [%d]\n",iIndex,m_rookieEventProcessInfo.m_iTargetCount,
					GetServerNameForEvent(lpCurrent->m_iReceiveRookieServer),lpCurrent->m_strRookieID,lpCurrent->m_strReceiveRookieName,
					GetServerNameForEvent(lpCurrent->m_iReceiveCallerServer),lpCurrent->m_strCallerID,lpCurrent->m_strReceiveCallerName,
					iRemainTime/60,iRemainTime%60,iErrorCount);

		write090720Info(lpCurrent,&exportFile);
	}

	exportFile.Close();

	if	(iErrorCount)
		cMSG::Error("문서 로딩 에러!!","문서 로딩중 %d개의 문제가 발견되었습니다.\n\n%s_로그.txt 파일을 확인해 주세요.",iErrorCount,_lpstrFileName);

	return	TRUE;
}

void
CServerTool::OnRookieEvent090720() 
{
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

	// Allocate a new console for our app
	if	(AllocConsole())
	{
		// Create the actual console
		g_hConsole	= CreateFile(_T("CONOUT$"), GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

		if	(g_hConsole != INVALID_HANDLE_VALUE)
		{
			SetConsoleMode(g_hConsole	, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

			SetConsoleTitle("event090720");
		}
	}

	char		strFile[256]	=	"신규가입이벤트(090720)";

	cFILE		logFile;

	logFile.Open(_ms("%s_로그.txt",strFile),"wb",TRUE);

	c090720RookieEventInfo	*pList		=	new c090720RookieEventInfo[50000];

	if	(!read090720RookieEventExistText(strFile,pList,50000,&logFile))
		return;

	if	(!ConnectAllServerDB())
	{
		cMSG::Error("DB 연결 실패!!","DB 연결에 실패 했습니다.");

		return;
	}

	if	(!build090720RookieEvent(strFile,pList,&logFile))
		return;

	cMSG::Put("작업 완료","작업이 완료 되었습니다.\n\n가공된 데이터가 [%s_빌드.txt] 파일에 저장되었습니다.",strFile);

	pKILL(pList);

	FreeConsole();
}

BOOL
CServerTool::give090720RookieEventItem(cADO_ADMIN *_lpDB,c090720RookieEventInfo *_lpInfo,int _iCurrentRookieLevel,CAvatarData *_lpData,BOOL _bIsRookie,cFILE *_lpLogFile)
{
	cITEM	*alpItem			=	NULL;
	int		iAddItemCount		=	0;
	int		iAddBankItemCount	=	0;
	int		iLastReceivedTimeRookieLevel=	-1;
	char	*lpstrTargetID		=	NULL;

	char	*strRookie[]		=	{"추천인","신규"};

	if	(_bIsRookie)
	{
		alpItem				=	m_rookieEventProcessInfo.m_aRookieItem;
		lpstrTargetID		=	_lpInfo->m_strRookieID;
		iLastReceivedTimeRookieLevel	=	_lpInfo->m_iReceivedTimeRookieLevelForRookie;
	}
	else
	{
		alpItem				=	m_rookieEventProcessInfo.m_aCallerItem;
		lpstrTargetID		=	_lpInfo->m_strCallerID;
		iLastReceivedTimeRookieLevel	=	_lpInfo->m_iReceivedTimeRookieLevelForCaller;
	}

	for (int i=0;;i++)
	{
//		int	aiCheckLevelASC[]	=	{30,50,100,200,300,0xffff};
		int	iCheckLevel	=	m_rookieEventProcessInfo.m_aiCheckLevelASC[i];

		if	(_iCurrentRookieLevel		<	iCheckLevel)
			break;

		if	(iLastReceivedTimeRookieLevel	>=	iCheckLevel)
			continue;

		BOOL			bIsAddToBank	=	FALSE;

		CItemDefine		*lpInvalidSlot	=	_lpData->getValidItem();

		if	(!lpInvalidSlot)
		{
			lpInvalidSlot	=	m_commonAm.getFreeSlotInBank();
			bIsAddToBank	=	TRUE;
		}

		if	(!lpInvalidSlot)
		{
			_lpLogFile->writeStreamCR("지급 실패	%s [%s:%s] 빈 슬롯이 없어서 아이템 추가 실패",strRookie[_bIsRookie],lpstrTargetID,_lpData->m_strName);

			return	FALSE;
		}

		iAddItemCount++;

		memcpy(lpInvalidSlot,&alpItem[i],sizeof(CItemDefine));

		iAddBankItemCount	+=	bIsAddToBank;

		if	(bIsAddToBank)
			m_commonAm.m_bIsChangeBankData	=	TRUE;
	}

	if	(iAddItemCount)
	{
		_lpData->m_dwSaveCount++;

		if	(!_lpDB->saveData((cP_PLAYER_SAVE_DATA *)_lpData))
		{
			_lpLogFile->writeStreamCR("지급 실패	%s [%s:%s] DB에 저장 실패",strRookie[_bIsRookie],lpstrTargetID,_lpData->m_strName);

			return	FALSE;
		}

		if	(_bIsRookie)
			_lpInfo->m_iReceivedTimeRookieLevelForRookie	=	_iCurrentRookieLevel;
		else
			_lpInfo->m_iReceivedTimeRookieLevelForCaller	=	_iCurrentRookieLevel;

		_lpLogFile->writeStreamCR("아이템 지급	%s [%s:%s] [이전 지급 레벨 %d:현재 지급 레벨 %d] 아이템 %d개 추가[인벤 %d개,은행 %d개]",
								strRookie[_bIsRookie],lpstrTargetID,_lpData->m_strName,
								iLastReceivedTimeRookieLevel,_iCurrentRookieLevel,
								iAddItemCount,iAddItemCount-iAddBankItemCount,iAddBankItemCount);
	}
	else
	{
		_lpLogFile->writeStreamCR("지급 안함	%s [%s:%s] [이전 지급 레벨 %d:현재 레벨 %d]",
								strRookie[_bIsRookie],lpstrTargetID,_lpData->m_strName,
								iLastReceivedTimeRookieLevel,_iCurrentRookieLevel);
	}

	return	TRUE;
}

BOOL
CServerTool::give090720RookieEventItemRollBack(cADO_ADMIN *_lpDB,c090720RookieEventInfo *_lpInfo,int _iCurrentRookieLevel,CAvatarData *_lpData,BOOL _bIsRookie,cFILE *_lpLogFile)
{
	cITEM	*alpItem			=	NULL;
	int		iAddItemCount		=	0;
	int		iAddBankItemCount	=	0;
	int		iLastReceivedTimeRookieLevel=	-1;
	char	*lpstrTargetID		=	NULL;

	char	*strRookie[]		=	{"추천인","신규"};

	if	(_bIsRookie)
	{
		alpItem				=	m_rookieEventProcessInfo.m_aRookieItem;
		lpstrTargetID		=	_lpInfo->m_strRookieID;
		iLastReceivedTimeRookieLevel	=	_lpInfo->m_iReceivedTimeRookieLevelForRookie;
	}
	else
	{
		alpItem				=	m_rookieEventProcessInfo.m_aCallerItem;
		lpstrTargetID		=	_lpInfo->m_strCallerID;
		iLastReceivedTimeRookieLevel	=	_lpInfo->m_iReceivedTimeRookieLevelForCaller;
	}

	for (int i=0;;i++)
	{
//		int	aiCheckLevelASC[]	=	{30,50,100,200,300,0xffff};
		int	iCheckLevel	=	m_rookieEventProcessInfo.m_aiCheckLevelASC[i];

		if	(_iCurrentRookieLevel		<	iCheckLevel)
			break;

		if	(iLastReceivedTimeRookieLevel	>=	iCheckLevel)
			continue;

		BOOL			bIsAddToBank	=	FALSE;

		CItemDefine		*lpInvalidSlot	=	_lpData->getExistItem(&alpItem[i]);

		if	(!lpInvalidSlot)
		{
			lpInvalidSlot	=	m_commonAm.getExistItemInBank(&alpItem[i]);
			bIsAddToBank	=	TRUE;
		}

		if	(!lpInvalidSlot)
			return	FALSE;

		lpInvalidSlot->reset();

		iAddItemCount++;
		iAddBankItemCount	+=	bIsAddToBank;

		if	(bIsAddToBank)
			m_commonAm.m_bIsChangeBankData	=	TRUE;
	}

	if	(iAddItemCount)
	{
		_lpData->m_dwSaveCount++;

		if	(!_lpDB->saveData((cP_PLAYER_SAVE_DATA *)_lpData))
		{
			_lpLogFile->writeStreamCR("회수 실패	%s [%s:%s] DB에 저장 실패",strRookie[_bIsRookie],lpstrTargetID,_lpData->m_strName);

			return	FALSE;
		}

		_lpLogFile->writeStreamCR("아이템 회수	%s [%s:%s] [이전 지급 레벨 %d:현재 지급 레벨 %d] 아이템 %d개 추가[인벤 %d개,은행 %d개]",
								strRookie[_bIsRookie],lpstrTargetID,_lpData->m_strName,
								iLastReceivedTimeRookieLevel,_iCurrentRookieLevel,
								iAddItemCount,iAddItemCount-iAddBankItemCount,iAddBankItemCount);
	}
	else
	{
		_lpLogFile->writeStreamCR("아이템 회수 안함	%s [%s:%s] [이전 지급 레벨 %d:현재 레벨 %d]",
								strRookie[_bIsRookie],lpstrTargetID,_lpData->m_strName,
								iLastReceivedTimeRookieLevel,_iCurrentRookieLevel);
	}

	return	TRUE;
}

void
CServerTool::build090720RookieEventRewardItem()
{
	int	aiCheckLevel[]		=	{300,200,100,50,30,0};
	int	aiCheckLevelASC[]	=	{30,50,100,200,300,0xffff};

	{
		int	iSize	=	0;
		int i;

		for (i=0;;i++)
		{
			if	(aiCheckLevel[i]	!=	0)
				iSize++;
			else
				break;
		}

		for (i=0;i<iSize;i++)
		{
			assert(aiCheckLevel[i] == aiCheckLevelASC[iSize-i-1]);
		}

		assert(aiCheckLevelASC[iSize] == 0xffff);
	}

	memcpy(m_rookieEventProcessInfo.m_aiCheckLevel,aiCheckLevel,sizeof(m_rookieEventProcessInfo.m_aiCheckLevel));
	memcpy(m_rookieEventProcessInfo.m_aiCheckLevelASC,aiCheckLevelASC,sizeof(m_rookieEventProcessInfo.m_aiCheckLevel));

	for (int i=0;i<5;i++)
	{
		cITEM	*lpItem	=	&m_rookieEventProcessInfo.m_aRookieItem[i];

		lpItem->reset();
		lpItem->setBitFieldValue(6);

		switch(i)
		{
			case	0	:
				//	하드 피스트 [336:0] [1,16] [1,2] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
				lpItem->m_wBaseItem		=	336;
				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
				lpItem->m_bCount		=	1;
				lpItem->m_bDurability	=	16;

				lpItem->m_aOption[0]	=	1;
				lpItem->m_aOption[1]	=	2;
				break;

			case	1	:
				//	알파캡 [283:0] [1,25] [4,2] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
				lpItem->m_wBaseItem		=	283;
				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
				lpItem->m_bCount		=	1;
				lpItem->m_bDurability	=	25;

				lpItem->m_aOption[0]	=	4;
				lpItem->m_aOption[1]	=	2;
				break;

			case	2	:
				//	천리화 [315:0] [1,32] [3,10] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
				lpItem->m_wBaseItem		=	315;
				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
				lpItem->m_bCount		=	1;
				lpItem->m_bDurability	=	32;

				lpItem->m_aOption[0]	=	3;
				lpItem->m_aOption[1]	=	10;
				break;

			case	3	:
				//	버터플라이 스팅 [296:0] [1,20] [2,0] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
				lpItem->m_wBaseItem		=	296;
				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
				lpItem->m_bCount		=	1;
				lpItem->m_bDurability	=	20;

				lpItem->m_aOption[0]	=	2;
				lpItem->m_aOption[1]	=	0;
				break;

			case	4	:
				//	세이프 케이지 [353:0] [1,28] [10,0] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
				lpItem->m_wBaseItem		=	353;
				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
				lpItem->m_bCount		=	1;
				lpItem->m_bDurability	=	28;

				lpItem->m_aOption[0]	=	10;
				lpItem->m_aOption[1]	=	0;
				break;
		}

		lpItem	=	&m_rookieEventProcessInfo.m_aCallerItem[i];

		lpItem->reset();
		lpItem->setBitFieldValue(6);

		switch(i)
		{
			case	0	:
				//	브룬의 국새 [333:0] [1,34] [10,0] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
				lpItem->m_wBaseItem		=	333;
				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
				lpItem->m_bCount		=	1;
				lpItem->m_bDurability	=	34;

				lpItem->m_aOption[0]	=	10;
				lpItem->m_aOption[1]	=	0;
				break;

			case	1	:
				//	루시드 헬름 [286:0] [1,45] [6,10] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
				lpItem->m_wBaseItem		=	286;
				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
				lpItem->m_bCount		=	1;
				lpItem->m_bDurability	=	45;

				lpItem->m_aOption[0]	=	6;
				lpItem->m_aOption[1]	=	10;
				break;

			case	2	:
				//	스피릿 오브 커머셜 [313:0] [1,16] [1,6] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
				lpItem->m_wBaseItem		=	313;
				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
				lpItem->m_bCount		=	1;
				lpItem->m_bDurability	=	16;

				lpItem->m_aOption[0]	=	1;
				lpItem->m_aOption[1]	=	6;
				break;

			case	3	:
				//	핸즈 오브 애드버서리 [292:0] [1,30] [2,0] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
				lpItem->m_wBaseItem		=	292;
				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
				lpItem->m_bCount		=	1;
				lpItem->m_bDurability	=	30;

				lpItem->m_aOption[0]	=	2;
				lpItem->m_aOption[1]	=	0;
				break;

			case	4	:
				//	버터플라이 스팅 [296:0] [1,20] [2,0] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
				lpItem->m_wBaseItem		=	296;
				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
				lpItem->m_bCount		=	1;
				lpItem->m_bDurability	=	20;

				lpItem->m_aOption[0]	=	2;
				lpItem->m_aOption[1]	=	0;
				break;
		}
	}
}

BOOL
CServerTool::run090720RookieEvent(char *_lpstrFileName,c090720RookieEventInfo *_lpList,cFILE *_lpLogFile)
{
	DWORD	dwBeginTime		=	timeGetTime();
	DWORD	dwTotalPastTime	=	0;
	int		iAverageTime	=	0;
	int		iAverageTimeZeroCount	=	0;
	int		iSerchCount		=	0;
	int		iErrorCount		=	0;

	cFILE	exportFile;

	exportFile.Open(_ms("%s_지급.txt",_lpstrFileName),"wb",TRUE);
	exportFile.writeStreamCR("신규 가입	추천인	지급 캐릭터(신규)	서버(신규)	지급 캐릭터(추천인)	서버(추천인)	지급된 레벨(신규)	지급된 레벨(추천인)");

	//	기존 문서 로딩
	for(int iIndex=0;iIndex<m_rookieEventProcessInfo.m_iTargetCount;iIndex++)
	{
		c090720RookieEventInfo	*lpCurrent	=	&_lpList[iIndex];

		int		iHitCount	=	0;

		Printf("operate #%4d/%d %s\n",iIndex,m_rookieEventProcessInfo.m_iTargetCount,lpCurrent->m_strRookieID);

		if	(lpCurrent->m_strReceiveRookieName[0]	==	NULL)
		{
			_lpLogFile->writeStreamCR("캐릭터 없음	[%s] 레벨 30이상 신규 캐릭터 없어서 지급 안함",lpCurrent->m_strRookieID);

			write090720Info(lpCurrent,&exportFile);

			continue;
		}

		int	iRookieLevel	=	lpCurrent->m_iReceivedTimeRookieLevelForRookie;

		if	(lpCurrent->m_iReceivedTimeRookieLevelForRookie	<	300)
		{
			if	(!m_aDb[lpCurrent->m_iReceiveRookieServer].loadData(NULL,lpCurrent->m_strReceiveRookieName,&m_commonAm) || m_commonAm.m_iLoadDataCount == 0)
			{
				_lpLogFile->writeStreamCR("신규 검색 실패	[%s] [%s:%s] 캐릭터 검색 실패",
					lpCurrent->m_strRookieID,GetServerNameForEvent(lpCurrent->m_iReceiveRookieServer),lpCurrent->m_strReceiveRookieName);

				write090720Info(lpCurrent,&exportFile);
				continue;
			}

			iRookieLevel	=	m_commonAm.m_aData[0].m_iLevel;

			give090720RookieEventItem(&m_aDb[lpCurrent->m_iReceiveRookieServer],lpCurrent,iRookieLevel,&m_commonAm.m_aData[0],TRUE,_lpLogFile);
		}
//		give090720RookieEventItemRollBack(&m_aDb[lpCurrent->m_iReceiveRookieServer],lpCurrent,iRookieLevel,&m_commonAm.m_aData[0],TRUE,_lpLogFile);

		if	(lpCurrent->m_strReceiveCallerName[0]	==	NULL)
		{
			write090720Info(lpCurrent,&exportFile);

			continue;
		}

		if	(lpCurrent->m_iReceivedTimeRookieLevelForCaller	<	300)
		{
			if	(!m_aDb[lpCurrent->m_iReceiveCallerServer].loadData(NULL,lpCurrent->m_strReceiveCallerName,&m_commonAm) || m_commonAm.m_iLoadDataCount == 0)
			{
				_lpLogFile->writeStreamCR("추천인 검색 실패	[%s] [%s:%s] 캐릭터 검색 실패",
					lpCurrent->m_strRookieID,GetServerNameForEvent(lpCurrent->m_iReceiveCallerServer),lpCurrent->m_strReceiveCallerName);

				write090720Info(lpCurrent,&exportFile);

				continue;
			}

			give090720RookieEventItem(&m_aDb[lpCurrent->m_iReceiveCallerServer],lpCurrent,iRookieLevel,&m_commonAm.m_aData[0],FALSE,_lpLogFile);
		}
//		give090720RookieEventItemRollBack(&m_aDb[lpCurrent->m_iReceiveCallerServer],lpCurrent,iRookieLevel,&m_commonAm.m_aData[0],FALSE,_lpLogFile);

		write090720Info(lpCurrent,&exportFile);
	}

	exportFile.Close();

	if	(_lpLogFile->Length)
		cMSG::Error("에러!!","작업 처리중 문제가 발견되었습니다.\n\n%s_로그.txt 파일을 확인해 주세요.",_lpstrFileName);

	return	TRUE;
}

void CServerTool::OnRookieEventRun090720() 
{
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

	build090720RookieEventRewardItem();

	// Allocate a new console for our app
	if	(AllocConsole())
	{
		// Create the actual console
		g_hConsole	= CreateFile(_T("CONOUT$"), GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

		if	(g_hConsole != INVALID_HANDLE_VALUE)
		{
			SetConsoleMode(g_hConsole	, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

			SetConsoleTitle("event090720");
		}
	}

	char		strFile[256]	=	"신규가입이벤트(090720)_빌드";

	cFILE		logFile;

	logFile.Open(_ms("%s_로그.txt",strFile),"wb",TRUE);

	c090720RookieEventInfo	*pList		=	new c090720RookieEventInfo[50000];

	if	(!read090720RookieEventExistText(strFile,pList,50000,&logFile))
		return;

	if	(!ConnectAllServerDB())
	{
		cMSG::Error("DB 연결 실패!!","DB 연결에 실패 했습니다.");

		return;
	}

	if	(!run090720RookieEvent(strFile,pList,&logFile))
		return;

	cMSG::Put("작업 완료","작업이 완료 되었습니다.\n\n가공된 데이터가 [%s_지급.txt] 파일에 저장되었습니다.",strFile);

	FreeConsole();

	pKILL(pList);
}