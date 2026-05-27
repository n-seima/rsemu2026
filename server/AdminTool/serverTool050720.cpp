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


class	cActor_serverTool050720
{
public:
	char	m_strId[dID_LENGTH];
	char	m_strName[dNAME_LENGTH];
	UTime	m_timeBegin,m_timeEnd;
	WORD	m_wLevel,m_wLastLevel;
	DWORD	m_dwExp,m_dwLastExp;
	WORD	m_wField;
	WORD	m_wHitCount;
	WORD	m_wIsEnd;

	void	reset(){}
	void	copy(cActor_serverTool050720 *_lpData){}
};

class	cActor_serverTool050720Manager
{
public:
	int							m_iActorCount;
	cActor_serverTool050720		m_aList[10000];

	void						reset();
	BOOL						add(cActor_serverTool050720 *_lpData);
	cActor_serverTool050720*	get(char *_lpstrName);
	void						remove(char *_lpstrName);
};

void
cActor_serverTool050720Manager::reset()
{
	m_iActorCount	=	0;
}

BOOL
cActor_serverTool050720Manager::add(cActor_serverTool050720 *_lpData)
{

	cActor_serverTool050720*	lpActor	=	get(_lpData->m_strName);

	if	(lpActor)
	{
		if	(lpActor->m_wIsEnd)
			return	TRUE;

		lpActor->m_wHitCount++;

		if	(lpActor->m_wField	!=	_lpData->m_wField)
		{
			if	(lpActor->m_wLastLevel	>=	35)
			{
				lpActor->m_wIsEnd	=	TRUE;
				return	TRUE;
			}
			else
			{
				remove(_lpData->m_strName);
				lpActor	=	NULL;
			}
		}
		else
		{
			if	(_lpData->m_wLevel	<	30)
			{
				memcpy(lpActor,_lpData,sizeof(cActor_serverTool050720)-4);
				lpActor->m_wLastLevel	=	0;
				lpActor->m_wHitCount	=	1;
				lpActor->m_wIsEnd		=	FALSE;
			}
			else
			{
				lpActor->m_wLastLevel	=	_lpData->m_wLevel;
				lpActor->m_dwLastExp	=	_lpData->m_dwExp;
				memcpy(&lpActor->m_timeEnd,&_lpData->m_timeBegin,sizeof(_lpData->m_timeBegin));
			}
		}
	}

	if	(lpActor	==	NULL)
	{
		if	(_lpData->m_wLevel	>	30)
			return	FALSE;

		lpActor	=	&m_aList[m_iActorCount];

		memcpy(lpActor,_lpData,sizeof(cActor_serverTool050720)-4);
		lpActor->m_wLastLevel	=	0;
		lpActor->m_wHitCount	=	1;
		lpActor->m_wIsEnd		=	FALSE;

		if	(stricmp(lpActor->m_strName,"천궁의활")==0)
			lpActor->m_wIsEnd		=	FALSE;

		
		m_iActorCount++;
	}

	return	TRUE;
}

void
cActor_serverTool050720Manager::remove(char *_lpstrName)
{
	for	(int i=0;i<m_iActorCount;i++)
	{
		if	(stricmp(_lpstrName,m_aList[i].m_strName)	==	0)
		{
			for	(int j=i;j<min(m_iActorCount-1,10000-1);j++)
				memcpy(&m_aList[j],&m_aList[j+1],sizeof(m_aList[j]));

			m_iActorCount--;

			return;
		}
	}
}

cActor_serverTool050720*
cActor_serverTool050720Manager::get(char *_lpstrName)
{
	for	(int i=0;i<m_iActorCount;i++)
	{
		if	(stricmp(_lpstrName,m_aList[i].m_strName)	==	0)
			return	&m_aList[i];
	}

	return	NULL;
}

cActor_serverTool050720Manager	l_am;

void CServerTool::On050720() 
{
	cDECODER	decoder;
	char		*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};

	SetLogFolder();

	int	iMaxHitCount	=	0;

	for(int	i=0;;i++)
	{
		if	(stricmp(strServerName[i],"end")==0)
			break;

		if	(!decoder.Upload(_ms("050720_%s.txt",strServerName[i]),NULL))
			continue;

		l_am.reset();

		cFILE	file;

		while(1)
		{
			BOOL	bIsOk;
			char	*lpstrText;

			cActor_serverTool050720	actor;

			actor.m_timeBegin.year	=	decoder.GetNumber(&bIsOk);

			if	(!bIsOk)
				break;

			actor.m_timeBegin.month	=	decoder.GetNumber(&bIsOk);
			actor.m_timeBegin.day	=	decoder.GetNumber(&bIsOk);
			actor.m_timeBegin.hour	=	decoder.GetNumber(&bIsOk);
			actor.m_timeBegin.minute=	decoder.GetNumber(&bIsOk);

			lpstrText	=	decoder.GetStream();
			strcpy(actor.m_strId,lpstrText);

			lpstrText	=	decoder.GetStream();
			strcpy(actor.m_strName,lpstrText);

			decoder.GetWord();
			actor.m_wLevel	=	decoder.GetNumber(&bIsOk);
			
			decoder.GetWord();
			actor.m_dwExp	=	decoder.GetNumber(&bIsOk);

			if	(stricmp(actor.m_strName,"블레이드댄서")	==	0)//	&&	actor.m_dwExp	==	26477)
				i	=i;
			
			decoder.GetWord();
			actor.m_wField	=	decoder.GetNumber(&bIsOk);

			l_am.add(&actor);
		}

		file.Open(_ms("050720_%s_result.txt",strServerName[i]),"wb",TRUE);

		for	(int j=0;j<l_am.m_iActorCount;j++)
		{
			cActor_serverTool050720	*lpActor	=	&l_am.m_aList[j];

			if	(stricmp(lpActor->m_strName,"블레이드댄서")	==	0)
				j	=j;

			iMaxHitCount	=	max(iMaxHitCount,lpActor->m_wHitCount);

			if	(lpActor->m_wLastLevel	<=	35)
				continue;

			{
				decoder.rewind();

				while(1)
				{
					BOOL	bIsOk;
					char	*lpstrText;

					cActor_serverTool050720	actor;

					actor.m_timeBegin.year	=	decoder.GetNumber(&bIsOk);

					if	(!bIsOk)
						break;

					actor.m_timeBegin.month	=	decoder.GetNumber(&bIsOk);
					actor.m_timeBegin.day	=	decoder.GetNumber(&bIsOk);
					actor.m_timeBegin.hour	=	decoder.GetNumber(&bIsOk);
					actor.m_timeBegin.minute=	decoder.GetNumber(&bIsOk);

					lpstrText	=	decoder.GetStream();
					strcpy(actor.m_strId,lpstrText);

					lpstrText	=	decoder.GetStream();
					strcpy(actor.m_strName,lpstrText);

					decoder.GetWord();
					actor.m_wLevel	=	decoder.GetNumber(&bIsOk);
					
					decoder.GetWord();
					actor.m_dwExp	=	decoder.GetNumber(&bIsOk);
					
					decoder.GetWord();
					actor.m_wField	=	decoder.GetNumber(&bIsOk);

					if	(stricmp(lpActor->m_strName,actor.m_strName)	!=	0)
						continue;

					file.writeStreamCR("\"%18s\"/\"%18s\" [%2d %2d:%2d %d/%d]",actor.m_strId,actor.m_strName,
						actor.m_timeBegin.day,actor.m_timeBegin.hour,actor.m_timeBegin.minute,actor.m_wLevel,actor.m_dwExp);
				}
				file.writeStreamCR("");
			}
		}

		file.Close();
	}	

	cMSG::Put("end","");
}

void CServerTool::On50721() 
{
	cDECODER	decoder;
	char		*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};

	SetLogFolder();

	cFILE	file;

	file.Open("050720 퀘스트 경험치 반복 획득자.txt","wb",TRUE);

	for(int	i=0;;i++)
	{
		if	(stricmp(strServerName[i],"end")==0)
			break;

		if	(!decoder.Upload(_ms("050720 [%s].txt",strServerName[i]),NULL))
			return;

		while(1)
		{
			BOOL	bIsOk;
			char	*lpstrText	=	decoder.GetStream();

			if	(stricmp(lpstrText,"end")	==	0)
				break;

			char	strId[dID_LENGTH],strName[dNAME_LENGTH];

			strcpy(strId,lpstrText);

			lpstrText	=	decoder.GetStream();

			strcpy(strName,lpstrText);

			decoder.GetNumber(&bIsOk);
			decoder.GetNumber(&bIsOk);
			decoder.GetNumber(&bIsOk);

			int	iLevel1	=	decoder.GetNumber(&bIsOk);
			int	iExp1	=	decoder.GetNumber(&bIsOk);

			decoder.GetStream();
			decoder.GetStream();
			
			decoder.GetNumber(&bIsOk);
			decoder.GetNumber(&bIsOk);
			decoder.GetNumber(&bIsOk);

			int	iLevel2	=	decoder.GetNumber(&bIsOk);
			int	iExp2	=	decoder.GetNumber(&bIsOk);
			int	iExpGap	=	ExpCalc::GetExp(iLevel1,iExp1,iLevel2,iExp2);

			file.writeStreamCR("%s \"%s\"/\"%s\" [%d/%d] %d",strServerName[i],strId,strName,iLevel2,iExp2,iExpGap);
		}
	}

	file.writeStreamCR("\"end\"");

	file.Close();

	cMSG::Put("end","");
}


void CServerTool::On50722() 
{
	cFILE		file;
	cDECODER	decoder;
	char		*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};

	SetLogFolder();

	file.Open("050720 퀘스트 경험치 반복 획득자 result.txt","wb",TRUE);

	if	(!decoder.Upload("050720 퀘스트 경험치 반복 획득자.txt",NULL))
		return;

	char	strDbName[256];

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

	char	strCurrentServer[20]		=	"";
	int		iRemoveCharacterCount		=	0;
	int		iCanNotFindCharacterCount	=	0;
	int		iOperateCharacterCount		=	0;

	while(1)
	{
		BOOL	bIsOk;
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

			if	(iCurServer	==	-	1	||	!m_db.Connect(	sList[iCurServer].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
			{
				MessageBox(g_strToolMessage[50]);
				return;
			}
		}
		

		char	strId[dID_LENGTH],strName[dNAME_LENGTH];

		lpstrText	=	decoder.GetStream();

		strcpy(strId,lpstrText);

		lpstrText	=	decoder.GetStream();

		strcpy(strName,lpstrText);

		int	iCheckLevel	=	decoder.GetNumber(&bIsOk);
		int	iCheckExp	=	decoder.GetNumber(&bIsOk);
		int	iExp		=	decoder.GetNumber(&bIsOk);

		CAvatarManager	m_am;

		if	(!m_db.loadData(NULL,strName,&m_am))
		{
			file.writeStreamCR("%s \"%s\"/\"%s\" dbConnectFailed",strCurrentServer,strId,strName);
			continue;
		}

		if	(m_am.m_iLoadDataCount	<=	0)
		{
			iCanNotFindCharacterCount++;
			file.writeStreamCR("%s \"%s\"/\"%s\" characterNotFound",strCurrentServer,strId,strName);
			continue;
		}

		CAvatarData	*lpPlayer	=	&m_am.m_aData[0];

		int	iOldLevel	=	lpPlayer->m_iLevel;
		int	iOldExp		=	lpPlayer->m_iExperience;

		if	(iOldLevel	<iCheckLevel	||	(iOldLevel	==	iCheckLevel	&&	iOldExp	<	iCheckExp))
		{
			iRemoveCharacterCount++;
			file.writeStreamCR("%s \"%s\"/\"%s\" removeCharacter",strCurrentServer,strId,strName);
			continue;
		}

		lpPlayer->decreaseExp(iExp);

		lpPlayer->m_dwSaveCount++;

		if	(!m_db.saveData((cP_PLAYER_SAVE_DATA *)lpPlayer))
			file.writeStreamCR("%s \"%s\"/\"%s\" saveFailed",strCurrentServer,lpPlayer->m_strId,lpPlayer->m_strName);

		iOperateCharacterCount++;

		file.writeStreamCR("%s \"%s\"/\"%s\" expDown [%d/%d] -%d [%d/%d]",strCurrentServer,strId,strName,iOldLevel,iOldExp,iExp,lpPlayer->m_iLevel,lpPlayer->m_iExperience);
	}	

	file.writeStreamCR("\"end\"");

	file.writeStreamCR("---------------------------------------------------");

	file.writeStreamCR("             대상 캐릭터 수 : %d",iOperateCharacterCount+iRemoveCharacterCount +iCanNotFindCharacterCount);
	file.writeStreamCR("    경험치 다운된 캐릭터 수 : %d",iOperateCharacterCount);
	file.writeStreamCR("    삭제후 생성한 캐릭터 수 : %d",iRemoveCharacterCount);
	file.writeStreamCR("           삭제한 캐릭터 수 : %d",iCanNotFindCharacterCount);

	file.Close();

	cMSG::Put("complete","경험치 다운 처리");
}

void CServerTool::On50723() 
{
//	On050720();
//	return;
	cFILE		file;
	cDECODER	decoder;
	char		*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};

	SetLogFolder();

	file.Open("050722 퀘스트 경험치 반복 획득자_바헬.txt","wb",TRUE);

	if	(!decoder.Upload("050720_바헬_result.txt",NULL))
		return;

	char	strDbName[256];

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

	char	strCurrentServer[20]		=	"";
	int		iRemoveCharacterCount		=	0;
	int		iCanNotFindCharacterCount	=	0;
	int		iOperateCharacterCount		=	0;

	char	strCurrentID[dID_LENGTH+2]	=	"";
	char	strCurrentName[dNAME_LENGTH+2]=	"";

	int		iBeginLevel,iBeginExp;
	UTime	timeBegin;
	BOOL	bIsWrite	=	FALSE;

	while(1)
	{
		BOOL	bIsOk;
		char	*lpstrText	=	decoder.GetStream();

		if	(stricmp(lpstrText,"end")	==	0)
			break;

		char	strId[dID_LENGTH],strName[dNAME_LENGTH];

		strcpy(strId,lpstrText);

		lpstrText	=	decoder.GetStream();

		strcpy(strName,lpstrText);

		int	iDay	=	decoder.GetNumber(&bIsOk);
		int	iHour	=	decoder.GetNumber(&bIsOk);
		int	iMinute	=	decoder.GetNumber(&bIsOk);
		int	iLevel	=	decoder.GetNumber(&bIsOk);
		int	iExp	=	decoder.GetNumber(&bIsOk);

		if	(stricmp(strName,strCurrentName)	!=	0)
		{
			strcpy(strCurrentName,strName);
			iBeginLevel		=	iLevel;
			iBeginExp		=	iExp;
			timeBegin.day	=	iDay;
			timeBegin.hour	=	iHour;
			timeBegin.minute=	iMinute;
			bIsWrite		=	FALSE;
		}
		else
		{
			if	(bIsWrite)
				continue;

			if	(iLevel		>=	35)
			{
				int	iBeginTime	=	timeBegin.day*24*60+timeBegin.hour*60+timeBegin.minute;
				int	iEndTime	=	iDay*24*60+iHour*60+iMinute;

				int	iIndex		=	0,iPos	=	0;

				char	strLogId[dID_LENGTH],strLogName[dNAME_LENGTH];

				while(1)
				{
					if	(strId[iIndex]	!=	' ')
						strLogId[iPos++]	=	strId[iIndex];
						
					if	(strId[iIndex]	==	NULL)
						break;
					iIndex++;
				}
				iIndex		=	0,iPos	=	0;
				while(1)
				{
					if	(strName[iIndex]	!=	' ')
						strLogName[iPos++]	=	strName[iIndex];
						
					if	(strName[iIndex]	==	NULL)
						break;
					iIndex++;
				}

				if	(iEndTime	-	iBeginTime	<	60*2)
				{
					file.writeStreamCR("2hour \"%s\"/\"%s\" [%2d %2d:%2d] [%d/%d] [%d/%d] exp %d",strLogId,strLogName,
						timeBegin.day,timeBegin.hour,timeBegin.minute,
						iBeginLevel,iBeginExp,iLevel,iExp,ExpCalc::GetExp(iBeginLevel,iBeginExp,iLevel,iExp)
						);
				}
				else
				if	(iEndTime	-	iBeginTime	<	60*5)
				{
					file.writeStreamCR("5hour \"%s\"/\"%s\" [%2d %2d:%2d] [%d/%d] [%d/%d] exp %d",strLogId,strLogName,
						timeBegin.day,timeBegin.hour,timeBegin.minute,
						iBeginLevel,iBeginExp,iLevel,iExp,ExpCalc::GetExp(iBeginLevel,iBeginExp,iLevel,iExp)
						);
				}
				else
				if	(iEndTime	-	iBeginTime	<	60*24)
				{
					file.writeStreamCR("24hour \"%s\"/\"%s\" [%2d %2d:%2d] [%d/%d] [%d/%d] exp %d",strLogId,strLogName,
						timeBegin.day,timeBegin.hour,timeBegin.minute,
						iBeginLevel,iBeginExp,iLevel,iExp,ExpCalc::GetExp(iBeginLevel,iBeginExp,iLevel,iExp)
						);
				}

				bIsWrite		=	TRUE;
			}
			else
			{
				iBeginLevel		=	iLevel;
				iBeginExp		=	iExp;
				timeBegin.day	=	iDay;
				timeBegin.hour	=	iHour;
				timeBegin.minute=	iMinute;
				bIsWrite		=	FALSE;
			}
		}
	}	

	file.writeStreamCR("\"end\"");

	file.Close();

	cMSG::Put("complete","경험치 다운 처리");
}


void CServerTool::On50724() 
{
	cFILE		file;
	cDECODER	decoder;
	char		*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};

	SetLogFolder();

	char	strDbName[256];

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

	int	iServer	=	0;

	for(iServer=0;;iServer++)
	{
		if	(stricmp(strServerName[iServer],"end")	==	0)
			break;

		if	(!decoder.Upload(_ms("050722 퀘스트 경험치 반복 획득자_%s_처리 결과.txt",strServerName[iServer]),NULL))
			continue;;
		file.Open(_ms("050722 퀘스트 경험치 반복 획득자_%s_처리 결과 222.txt",strServerName[iServer]),"wb",TRUE);
		
//		if	(!decoder.Upload(_ms("050722 퀘스트 경험치 반복 획득자_%s.txt",strServerName[iServer]),NULL))
//			continue;;
//
//		file.Open(_ms("050722 퀘스트 경험치 반복 획득자_%s_처리 결과.txt",strServerName[iServer]),"wb",TRUE);

		if	(!m_db.Connect(	sList[iServer+1].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
		{
			MessageBox(g_strToolMessage[50]);
			return;
		}

		while(1)
		{
//			BOOL	bIsOk;
			char	*lpstrText	=	decoder.GetStream();

			if	(stricmp(lpstrText,"end")	==	0)
				break;

			char	strId[dID_LENGTH],strName[dNAME_LENGTH];

			strcpy(strId,lpstrText);

			lpstrText	=	decoder.GetStream();
			strcpy(strName,lpstrText);

			CAvatarManager	m_am;

			if	(!m_db.loadData(NULL,strName,&m_am))
			{
				file.writeStreamCR("\"%s\"/\"%s\" dbConnectFailed",strId,strName);
				continue;
			}

			if	(m_am.m_iLoadDataCount	<=	0)
				continue;

			CAvatarData	*lpPlayer	=	&m_am.m_aData[0];

			lpPlayer->resetStatePoint();
			lpPlayer->resetSkillPoint();

			lpPlayer->m_dwSaveCount++;

			if	(!m_db.saveData((cP_PLAYER_SAVE_DATA *)lpPlayer))
				file.writeStreamCR("\"%s\"/\"%s\" saveFailed",lpPlayer->m_strId,lpPlayer->m_strName);

			Sleep(1);
		}	

/*
		while(1)
		{
			BOOL	bIsOk;
			char	*lpstrText	=	decoder.GetWord();

			if	(stricmp(lpstrText,"end")	==	0)
				break;

			char	strId[dID_LENGTH],strName[dNAME_LENGTH];

			lpstrText	=	decoder.GetStream();
			strcpy(strId,lpstrText);

			lpstrText	=	decoder.GetStream();
			strcpy(strName,lpstrText);

			int	iDay		=	decoder.GetNumber(&bIsOk);
			int	iHour		=	decoder.GetNumber(&bIsOk);
			int	iMinute		=	decoder.GetNumber(&bIsOk);
			
			int	iBeginLevel	=	decoder.GetNumber(&bIsOk);
			int	iBeginExp	=	decoder.GetNumber(&bIsOk);
			int	iEndLevel	=	decoder.GetNumber(&bIsOk);
			int	iEndExp		=	decoder.GetNumber(&bIsOk);

			decoder.GetWord();

			int	iDecreaseExp=	decoder.GetNumber(&bIsOk);

			CAvatarManager	m_am;

			if	(!m_db.loadData(NULL,strName,&m_am))
			{
				file.writeStreamCR("\"%s\"/\"%s\" dbConnectFailed",strId,strName);
				continue;
			}

			if	(m_am.m_iLoadDataCount	<=	0)
				continue;

			CAvatarData	*lpPlayer	=	&m_am.m_aData[0];

			int	iOldLevel	=	lpPlayer->m_iLevel;
			int	iOldExp		=	lpPlayer->m_iExperience;

			if	(iOldLevel	<iEndLevel	||	(iOldLevel	==	iEndLevel	&&	iOldExp	<	iEndExp))
				continue;

			lpPlayer->decreaseExp(iDecreaseExp);
			lpPlayer->resetStatePoint();
			lpPlayer->resetSkillPoint();

			lpPlayer->m_dwSaveCount++;

			if	(!m_db.saveData((cP_PLAYER_SAVE_DATA *)lpPlayer))
				file.writeStreamCR("\"%s\"/\"%s\" saveFailed",lpPlayer->m_strId,lpPlayer->m_strName);

			file.writeStreamCR("\"%s\"/\"%s\" expDown [%d/%d] -%d [%d/%d]",strId,strName,iOldLevel,iOldExp,iDecreaseExp,lpPlayer->m_iLevel,lpPlayer->m_iExperience);
		}	
*/
		file.writeStreamCR("\"end\"");

		file.Close();
	}

	cMSG::Put("complete","경험치 다운 처리");
}
