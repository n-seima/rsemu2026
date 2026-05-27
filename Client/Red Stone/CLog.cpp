#include "CLog.h"
#include "time.h"
#include "CActor.h"
#include "CGame.h"
#include "CMessage.h"

CLogChatMessage	g_logChat;

int				l_iLogFileVersion	=	1;

DWORD
CChatMessage::mix()
{
	int		i;
	DWORD	dwCheckSum	=	0;

	for (i=0;i<dNAME_LENGTH;i++)
	{
		m_strName[i]+=	random(100);
		dwCheckSum	+=	m_strName[i];
	}

	for (i=0;i<dCHAT_LENGTH;i++)
	{
		m_strMessage[i]	+=	random(100);
		dwCheckSum		+=	m_strMessage[i];
	}

	return	dwCheckSum;
}

CLogChatMessage::CLogChatMessage()
{
	reset();
}

CLogChatMessage::~CLogChatMessage()
{
	close();
}

//
//	초기화
void
CLogChatMessage::init()
{
	m_dwBookedChattingLogTime	=	0xffffffff;
	m_dwBookedTradeLogTime		=	0xffffffff;

	reset();
}

//
//	종료
void
CLogChatMessage::close()
{
	reset();
}

//
//	리셋
void
CLogChatMessage::reset()
{
	if	(m_dwBookedChattingLogTime	!=	0xffffffff)
		writeChattingLog();
	if	(m_dwBookedTradeLogTime		!=	0xffffffff)
		writeTradeLog();

	m_iFirstQue					=	0;
	m_iLastQue					=	0;
	m_iCount					=	0;
	m_dwBookedChattingLogTime	=	0xffffffff;
	m_dwBookedTradeLogTime		=	0xffffffff;
}

//
//	패킷 추가
void
CLogChatMessage::update()
{
	char	*lpstrLogFile;

	lpstrLogFile	=	g_logChat.updateChattingLog();			//	채팅 로그 업데이트

	if	(lpstrLogFile)
	{
		CGamePlay::AddSystemMessage(LTYELLOW,dMSG_WRITE_CHAT_LOG);
		CGamePlay::AddSystemMessage(WHITE,dMSG_FOLDER_AND_FILE_FORM,dMSG_ACCUSE,lpstrLogFile);

	}

	lpstrLogFile	=	g_logChat.updateTradeLog();			//	거래 로그 업데이트

	if	(lpstrLogFile)
	{
		CGamePlay::AddSystemMessage(LTYELLOW,dMSG_WRITE_TRADE_LOG);
		CGamePlay::AddSystemMessage(WHITE,dMSG_FOLDER_AND_FILE_FORM,dMSG_ACCUSE,lpstrLogFile);
	}
}

//
//	패킷 추가
BOOL
CLogChatMessage::add(char *_lpstrName,char *_lpstrMessage,int _iChatType)
{
	strcpy(m_aData[m_iLastQue].m_strName	,_lpstrName);
	strncpy(m_aData[m_iLastQue].m_strMessage,_lpstrMessage,c_iLogChatLength-2);

	m_aData[m_iLastQue].m_strMessage[c_iLogChatLength-2]	=	NULL;

	m_aData[m_iLastQue].m_wChatType	=	_iChatType;

	m_iLastQue++;
	m_iCount++;

	if	(m_iCount	>	dMAX_CHAT_LOG_COUNT)
		m_iCount	=	dMAX_CHAT_LOG_COUNT;

	if	(m_iLastQue	>=	dMAX_CHAT_LOG_COUNT)
		m_iLastQue	=	0;	//	처음으로 돌아간다.

	update();

	return	TRUE;
}

//
//	
char*
CLogChatMessage::updateChattingLog()
{
	if	(m_dwBookedChattingLogTime	==	0xffffffff)
		return	NULL;

	BOOL	bIsShowTime		=	FALSE;

	if (m_iChattingLogBeginLine==	0)
	{
		if	(m_iLastQue	==	dMAX_CHAT_LOG_COUNT-1)
			bIsShowTime	=	TRUE;
	}
	else
	if	(m_iChattingLogBeginLine==	m_iLastQue+1)
		bIsShowTime	=	TRUE;

	if	(m_dwBookedChattingLogTime	+	dACCUSE_TIME	<	timeGetTime() || m_dwBookedChattingLogTime > timeGetTime() || bIsShowTime)
	{
		m_dwBookedChattingLogTime	=	0xffffffff;

		return	writeChattingLog();
	}

	return	NULL;
}

//
//	
char*
CLogChatMessage::updateTradeLog()
{
	if	(m_dwBookedTradeLogTime	==	0xffffffff)
		return	NULL;

	BOOL	bIsShowTime		=	FALSE;

	if	(m_iTradeLogBeginLine==	0)
	{
		if	(m_iLastQue	==	dMAX_CHAT_LOG_COUNT-1)
			bIsShowTime	=	TRUE;
	}
	else
	if	(m_iTradeLogBeginLine==	m_iLastQue+1)
		bIsShowTime	=	TRUE;

	if	(m_dwBookedTradeLogTime	+	dACCUSE_TIME*2	<	timeGetTime() || m_dwBookedTradeLogTime > timeGetTime() || bIsShowTime)
	{
		m_dwBookedTradeLogTime	=	0xffffffff;

		return	writeTradeLog();
	}

	return	NULL;
}

//
//
char*
CLogChatMessage::writeChattingLog()
{
	if	(m_iCount	<=	0			)
		return	NULL;

	DWORD	dwSeedValue	=	60000,dwCheckSum=	0;
	int		i;

	for (i=0;i<dID_LENGTH;i++)	
		dwSeedValue	+=	(BYTE)g_hero.m_strId[i];

	for (i=0;i<dNAME_LENGTH;i++)
		dwSeedValue	+=	g_hero.m_strName[i];

	for (i=0;i<dNAME_LENGTH;i++)
		dwSeedValue	+=	(BYTE)CGame::s_strLastSelectServerName[i];

	srand(dwSeedValue);

	cFILE			file;
	CChatMessage	current;

	int				iPoint	=	m_iChattingLogBeginLine;
	int				iYear	=	CGamePlay::s_currentTime.m_wYear%100;
	int				iMonth	=	CGamePlay::s_currentTime.m_wMonth;
	int				iDay	=	CGamePlay::s_currentTime.m_wDay;
	int				iHour	=	CGamePlay::s_currentTime.m_wHour;
	int				iMinute	=	CGamePlay::s_currentTime.m_wMinute;
	int				iSecond	=	CGamePlay::s_currentTime.m_wSecond;

	sprintf(m_strLogFileName,"%s[%s] %.2d%.2d%.2d %.2d%.2d%.2d.log",dMSG_ACCUSE,g_hero.m_strId,iYear%100,iMonth,iDay,iHour,iMinute,iSecond);

	if	(SetCurrentDirectory(dMSG_ACCUSE) == FALSE)
	{
		CreateDirectory(dMSG_ACCUSE,NULL);
		SetCurrentDirectory(dMSG_ACCUSE);
	}

	file.Open(m_strLogFileName,"wb");
	SetLogFolder();

	DWORD	dwCheckCode	=	0x12345678;

	file.Write(&dwCheckCode,4);			//	체크 코드 
	file.Write(&l_iLogFileVersion,4);	//	버젼

	file.Write(&iYear,4);
	file.Write(&iMonth,4);
	file.Write(&iDay,4);
	file.Write(&iHour,4);
	file.Write(&iMinute,4);
	file.Write(&iSecond,4);

	for (i=0;i<iSecond+iMinute+iHour+iMonth+iDay;i++)
		random(100);

	strcpy(current.m_strName,"●● Field Name");
	strcpy(current.m_strMessage,g_map.m_strName);

	current.m_wChatType	=	0;
	dwCheckSum			+=	current.mix();

	file.Write(&current,sizeof(CChatMessage));

	while(iPoint!=m_iLastQue)
	{
		memcpy(&current,&m_aData[iPoint],sizeof(CChatMessage));
		dwCheckSum	+=	current.mix();

		file.Write(&current,sizeof(CChatMessage));

		iPoint++;

		if (iPoint	>=	dMAX_CHAT_LOG_COUNT	)
			iPoint	=	0;	//	처음으로 돌아간다.
	}

	for (i=0;i<dID_LENGTH;i++)
		dwCheckSum	+=	g_hero.m_strId[i];
	for (i=0;i<dNAME_LENGTH;i++)
		dwCheckSum	+=	g_hero.m_strName[i];
	for (i=0;i<dNAME_LENGTH;i++)
		dwCheckSum	+=	CGame::s_strLastSelectServerName[i];

	file.Write(g_hero.m_strId,dID_LENGTH);
	file.Write(g_hero.m_strName,dNAME_LENGTH);
	file.Write(CGame::s_strLastSelectServerName,dNAME_LENGTH);
	file.Write(&dwCheckSum,4);
	file.Write(&dwSeedValue,4);

	file.Close();

	return	m_strLogFileName;
}

//
//
char*
CLogChatMessage::writeTradeLog()
{
	if	(m_iCount	<=	0			)
		return	NULL;

	DWORD	dwSeedValue	=	60000,dwCheckSum=	0;
	int		i;

	for (i=0;i<dID_LENGTH;i++)	
		dwSeedValue	+=	(BYTE)g_hero.m_strId[i];
	for (i=0;i<dNAME_LENGTH;i++)
		dwSeedValue	+=	g_hero.m_strName[i];
	for (i=0;i<dNAME_LENGTH;i++)
		dwSeedValue	+=	(BYTE)CGame::s_strLastSelectServerName[i];

	srand(dwSeedValue);

	cFILE			file;
	CChatMessage	current;
	int				iPoint	=	m_iTradeLogBeginLine;
	int				iYear	=	CGamePlay::s_currentTime.m_wYear%100;
	int				iMonth	=	CGamePlay::s_currentTime.m_wMonth;
	int				iDay	=	CGamePlay::s_currentTime.m_wDay;
	int				iHour	=	CGamePlay::s_currentTime.m_wHour;
	int				iMinute	=	CGamePlay::s_currentTime.m_wMinute;
	int				iSecond	=	CGamePlay::s_currentTime.m_wSecond;

	sprintf(m_strLogFileName,"%s[%s] %.2d%.2d%.2d %.2d%.2d%.2d.log",dMSG_TRADE,g_hero.m_strId,iYear,iMonth,iDay,iHour,iMinute,iSecond);

	if	(SetCurrentDirectory(dMSG_ACCUSE) == FALSE)
	{
		CreateDirectory(dMSG_ACCUSE,NULL);
		SetCurrentDirectory(dMSG_ACCUSE);
	}

	file.Open(m_strLogFileName,"wb");
	SetLogFolder();

	DWORD	dwCheckCode	=	0x12345678;

	file.Write(&dwCheckCode,4);			//	체크 코드 
	file.Write(&l_iLogFileVersion,4);	//	버젼

	file.Write(&iYear,4);
	file.Write(&iMonth,4);
	file.Write(&iDay,4);
	file.Write(&iHour,4);
	file.Write(&iMinute,4);
	file.Write(&iSecond,4);

	for (i=0;i<iSecond+iMinute+iHour+iMonth+iDay;i++)
		random(100);

	strcpy(current.m_strName,"●● Field Name");
	strcpy(current.m_strMessage,g_map.m_strName);

	current.m_wChatType	=	0;
	dwCheckSum			+=	current.mix();

	file.Write(&current,sizeof(CChatMessage));

	while(iPoint!=m_iLastQue)
	{
		if (STRICMP(m_aData[iPoint].m_strName,m_strTargetName	) == 0 ||
			STRICMP(m_aData[iPoint].m_strName,m_strOwnerName	) == 0 ||
			STRICMP(m_aData[iPoint].m_strName,"<■■ TRADE ■■>") == 0 )
		{
			memcpy(&current,&m_aData[iPoint],sizeof(CChatMessage));
			dwCheckSum	+=	current.mix();

			file.Write(&current,sizeof(CChatMessage));
		}

		iPoint++;
		if (iPoint	>=	dMAX_CHAT_LOG_COUNT	)	iPoint	=	0;	//	처음으로 돌아간다.
	}

	for (i=0;i<dID_LENGTH;i++)	dwCheckSum	+=	g_hero.m_strId[i];
	for (i=0;i<dNAME_LENGTH;i++)dwCheckSum	+=	g_hero.m_strName[i];
	for (i=0;i<dNAME_LENGTH;i++)dwCheckSum	+=	CGame::s_strLastSelectServerName[i];

	file.Write(g_hero.m_strId,dID_LENGTH);
	file.Write(g_hero.m_strName,dNAME_LENGTH);
	file.Write(CGame::s_strLastSelectServerName,dNAME_LENGTH);
	file.Write(&dwCheckSum,4);
	file.Write(&dwSeedValue,4);

	file.Close();

	return	m_strLogFileName;
}

//
//	신고?
BOOL
CLogChatMessage::accuseChattingLog()
{
	if (m_dwBookedChattingLogTime	!=	0xffffffff)
	{
		m_dwBookedChattingLogTime	=	timeGetTime();

		return FALSE;
	}

	add("<■■ REPORT ■■>","---------------------------------------------------------------------------------------",eLCMT_CHAT);

	int	iPreCount			=	min(m_iCount,dMAX_CHAT_LOG_COUNT/2);

	m_dwBookedChattingLogTime=	timeGetTime();
	m_iChattingLogBeginLine	=	m_iLastQue-iPreCount;

	if	(m_iChattingLogBeginLine<	0)
		m_iChattingLogBeginLine	+=	dMAX_CHAT_LOG_COUNT;

	return	TRUE;
}


BOOL
CLogChatMessage::accuseTradeLog(char *_lpstrOwner,char *_lpstrTarget,BOOL _bIsPitchmanShop)
{
	if	(m_dwBookedTradeLogTime	!=	0xffffffff)
	{
		m_dwBookedTradeLogTime	=	timeGetTime();

		return FALSE;
	}

	char	strText[512];

	if	(_bIsPitchmanShop)
		sprintf(strText,"%s Trade(%s) with %s",_lpstrOwner,dMSG_PITCHMAN_SHOP,_lpstrTarget);
	else
		sprintf(strText,"%s Trade with %s",_lpstrOwner,_lpstrTarget);

	add("<■■ TRADE ■■>",strText,eLCMT_CHAT);

	strcpy(m_strOwnerName,_lpstrOwner);
	strcpy(m_strTargetName,_lpstrTarget);

	int	iPreCount			=	min(m_iCount,dMAX_CHAT_LOG_COUNT/2);

	m_dwBookedTradeLogTime	=	timeGetTime();
	m_iTradeLogBeginLine	=	m_iLastQue-iPreCount;

	if	(m_iTradeLogBeginLine<	0)
		m_iTradeLogBeginLine	+=	dMAX_CHAT_LOG_COUNT;

	return	TRUE;
}