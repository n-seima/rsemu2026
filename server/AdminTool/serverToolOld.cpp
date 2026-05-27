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


void CServerTool::OnCheckQuest050707() 
{
#ifndef _FOR_KOREA
	return;
#endif

	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

//	if	(g_iSelectedServer	!=	0)
	if	(g_iSelectedServer	>=	g_iWorldCount || g_iSelectedServer < 0)
	{
		MessageBox("please select test server","");
		return;
	}

	char	strDbName[512];
	char	strServerName[30];

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

	if	(stricmp(strDbName,"RedStone_Avatar_Source_0")==0)
		strcpy(strServerName,"프란델");
	else
	if	(stricmp(strDbName,"RedStone_Avatar_Source_1")==0)
		strcpy(strServerName,"에립트");
	else
	if	(stricmp(strDbName,"RedStone_Avatar_Source_2")==0)
		strcpy(strServerName,"제노스");
	else
	if	(stricmp(strDbName,"RedStone_Avatar_Source_3")==0)
		strcpy(strServerName,"가디우스");
	else
	if	(stricmp(strDbName,"RedStone_Avatar_Source_4")==0)
		strcpy(strServerName,"바헬");
	else
	{
		cMSG::Error("잘못 된 이름 입니다.",strDbName);
		return;
	}

	char			strExportDir[512],strExportFileName[512];

	sprintf(strExportDir,"퀘스트 수행자 리스트 [%s]",strServerName);

	if (cMSG::YESNO(strExportDir,"Continue?") == IDNO)
		return;

	SetLogFolder();
	cDECODER		decoder;

	if (!decoder.Upload("QuestcheckExp - 4ST.txt",NULL))
	{
		ERRMSG("화일 없슈","QuestcheckExp - 4ST.txt");
		return;
	}

	int		iCheckQuestCount	=	0;

	class	cCheckQuestInfo
	{
	public:
		WORD	m_wQuestIndex;
		char	m_strQuestName[40];
		DWORD	m_dwExp;
	};

	cCheckQuestInfo	aCheckQuestList[100];

	while(1)
	{
		BOOL	bIsOk;
		int		iIndex		=	decoder.GetNumber(&bIsOk);

		if	(iIndex			==	-1)
			break;

		char	*lpstrName	=	decoder.GetStream();
		int		iExp		=	decoder.GetNumber(&bIsOk);

		aCheckQuestList[iCheckQuestCount].m_wQuestIndex	=	iIndex;
		strcpy(aCheckQuestList[iCheckQuestCount].m_strQuestName,lpstrName);
		aCheckQuestList[iCheckQuestCount].m_dwExp		=	iExp;

		CQuestItem	*lpQuest=	g_questArbeit.getQuestByName(lpstrName);

		if	(!lpQuest	||	lpQuest->m_wSerial	!=	iIndex)
		{
			cMSG::Error("잘못 된 퀘스트 정보입니다.",lpstrName);
			return;
		}

		iCheckQuestCount++;
	}

	int				i,j;
	_RecordsetPtr	pRs = NULL; 	
	_variant_t		v_bufferdata;
	char			str_Query[1024];
	int				i_RecCount;
	char			*pBuf = NULL;
	HRESULT			hr = CoInitialize(0);

	//	DB에 연결
	{
		if(!m_db.Connect(	sList[g_iSelectedServer].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
		{
			MessageBox(g_strToolMessage[50]);
			return;
		}
	}

//	sprintf(str_Query,"SELECT TOP 100 data FROM Avatar_Current");
//	sprintf(str_Query,"SELECT TOP 1000 data FROM Avatar_Current where lev > 100 and job = 10");
	sprintf(str_Query,"SELECT data FROM Avatar_Current where lev > 10");
//	sprintf(str_Query,"SELECT data FROM Avatar_Current");

	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)m_db.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	SetCurrentDirectory(g_strLogFolder);

	cFILE	file,file2;
	int		iItemCount	=	0;

	sprintf(strExportFileName,"퀘스트 수행자 리스트 4ST [%s].txt",strServerName);

	file.Open(strExportFileName,"wt",TRUE);

	sprintf(strExportFileName,"사육 기록서 보유자 4ST [%s].txt",strServerName);

	file2.Open(strExportFileName,"wt",TRUE);

	char	* uncompressdata	=	new char[sizeof(cPLAYER_DATA)];

	int		iPlayerCount						=	0;
	int		iBreedingRecordBookCount			=	0;
	int		iOwnBreedingRecordBookPlayerCount	=	0;
	int		iMaxExp				=	0;
	int		iLevelOfTop			=	0;
	char	strNameOfTop[dNAME_LENGTH],strIDOfTop[dID_LENGTH];
	char	strNameOfTopLevel[dNAME_LENGTH];
	int		iTopLevel			=	0;
	int		iTotalProcessQuest	=	0;

	if	(!FAILED(hr))
	{
		for(i = 0 ;i < i_RecCount ;i++)
		{
			cPLAYER_DATA	data;

			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			int unsize = m_db.VariantToBlob(v_bufferdata,uncompressdata);

			if	(unsize	<	sizeof(cPLAYER_DATA) && unsize != 2300 && unsize !=2272)
			{
				WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&data,unsize,sizeof(cPLAYER_DATA));

				if	(ww == 0)
				{
					if	(unsize >= 2048)
					{
						memset(&data,0,sizeof(cPLAYER_DATA));
						memcpy(&data,uncompressdata,unsize);
					}

					_log(" uncompress error 2");
				}
			}
			else
			{
				memcpy(&data,uncompressdata,sizeof(cPLAYER_DATA));

//				if	(unsize ==2300 || unsize ==2272)
//					cMSG::Put("","데이타에 문제가 있습니다. 임시 싱크 명령으로 처리해 주시기 바랍니다.\n\n%s",data.m_strName);
			}

			VariantClear(&v_bufferdata);

			int				iTotalExp		=	0;
			int				iQuestCount		=	0;

			for	(j=0;j<iCheckQuestCount;j++)
			{
				int			iQuestIndex		=	aCheckQuestList[j].m_wQuestIndex;
				char		*lpstrName		=	aCheckQuestList[j].m_strQuestName;

				CQuestItem	*lpQuest		=	g_questArbeit.getQuestByName(lpstrName);

				if	(data.m_bf1AddData2005_03_03	==	FALSE)
					continue;

				if	(data.m_aCompleteArbeit[0]	==	0xcc	&&	data.m_aCompleteArbeit[1]	==	0xcc	&&	data.m_aCompleteArbeit[2]	==	0xcc)
					continue;

				int	iSlot	=	iQuestIndex/8;

				BOOL	bIsCompleteQuest	=	FALSE;
				
				if	(iSlot	>=	dARBEIT_BUFFER_SIZE)
					bIsCompleteQuest	=	data.m_aCompleteArbeit2[iSlot-dARBEIT_BUFFER_SIZE]&(1<<(iQuestIndex%8));
				else
					bIsCompleteQuest	=	data.m_aCompleteArbeit[iSlot]&(1<<(iQuestIndex%8));

				if	(bIsCompleteQuest)
				{
					int	iBoostExp			=	data.getBoostExperienceValue();
					int	iExp				=	aCheckQuestList[j].m_dwExp;

					LONGLONG	llExp		=	iExp;
					llExp					=	llExp*iBoostExp/100;

					iExp					=	(int)llExp;
					iTotalExp				+=	iExp;
					iQuestCount++;
					iTotalProcessQuest++;

					file.writeStreamCR("[\"%s\"/\"%s\"] %d \"%s\" %d [boost %d％]",data.m_strId,data.m_strName,data.m_iLevel,lpstrName,iExp,iBoostExp-100);
				}
			}

			if	(iQuestCount)
			{
				file.writeStreamCR("         - \"quest count\" %d,total exp %d",iQuestCount,iTotalExp);

				iPlayerCount++;

				if	(iTotalExp	>	iMaxExp)
				{
					strcpy(strIDOfTop,data.m_strId);
					strcpy(strNameOfTop,data.m_strName);
					iMaxExp		=	iTotalExp;
					iLevelOfTop	=	data.m_iLevel;
				}

				if	(data.m_iLevel	>	iTopLevel)
				{
					strcpy(strNameOfTopLevel,data.m_strName);
					iTopLevel		=	data.m_iLevel;
				}

				int	iCount	=	data.operateBreedingRecordBook050708(&file2);

				if	(iCount)
				{
					iOwnBreedingRecordBookPlayerCount++;
					iBreedingRecordBookCount			+=	iCount;
				}
			}

			pRs->MoveNext();
		}
	}

	delete	[]	uncompressdata;
	
	file.writeStreamCR(" ");
	file.writeStreamCR("\"end\"");
	file2.writeStreamCR("\"end\"");
	
	if	(iPlayerCount)
	{
		file.writeStreamCR("-------------------------------------------------------");
		file.writeStreamCR("총 [%d]명의 유저중 [%d]명의 유저가 [%d]개의 퀘스트를 수행함.",i_RecCount,iPlayerCount,iTotalProcessQuest);
		file.writeStreamCR("최고 퀘스트 경험치 이득자 : [%s/%s] LV [%d] Exp [%d]",strIDOfTop,strNameOfTop,iLevelOfTop,iMaxExp);
		file.writeStreamCR("최고 레벨 : [%s] LV [%d]",strNameOfTopLevel,iTopLevel);
		file.Close();
	}

	if	(iOwnBreedingRecordBookPlayerCount)
	{
		file2.writeStreamCR("-------------------------------------------------------");
		file2.writeStreamCR("총 [%d]명의 유저중 [%d]명의 유저가 [%d]개의 사육 기록서를 가지고 있음.",i_RecCount,iOwnBreedingRecordBookPlayerCount,iBreedingRecordBookCount);
		file2.Close();
	}

	pRs->Close();
	m_db.Close();

	sprintf(strExportDir,"퀘스트 수행자 리스트 추출 완료 in server '%s'",strServerName);

	MessageBox(strExportDir);

	return;
	
}

BOOL
CServerTool::uploadCompleteExpWorkActorList(int _iServer,int _iDay)
{
	m_iQuestOperatorCount		=	0;
	m_iQuestReversionItemCount	=	0;
	m_iQuestLevelDownPetCount	=	0;
	m_iQuestLevelDownCount		=	0;
	m_iQuestAddDownCount		=	0;

	cDECODER	decoder;
	char		*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};

	if	(!decoder.Upload(_ms("05.07.%.2d correct quest exp result [%s].txt",_iDay,strServerName[_iServer]),NULL))
		return	FALSE;

	while(1)
	{
		char	*lpstr;
		BOOL	bIsOk;

		lpstr	=	decoder.GetStream();

		if	(stricmp(lpstr,"end")	==	0)
			break;
		if	(stricmp(lpstr,"IT")	==	0)
		{
			m_iQuestReversionItemCount++;
			continue;
		}
		if	(stricmp(lpstr,"pet level down")	==	0)
		{
			m_iQuestLevelDownPetCount++;
			continue;
		}

		cQuestOperatorInfo050708	*lpInfo	=	&m_aList[m_iQuestOperatorCount];

		strcpy(lpInfo->m_strId,lpstr);

		lpstr	=	decoder.GetStream();

		strcpy(lpInfo->m_strName,lpstr);

		decoder.GetStream();
		decoder.GetNumber(&bIsOk);	//	level
		decoder.GetNumber(&bIsOk);	//	exp

		lpstr	=	decoder.GetWord();

		if	(stricmp(lpstr,"add")==0)
			m_iQuestAddDownCount++;
		else
			m_iQuestLevelDownCount++;

		int		iExp		=	decoder.GetNumber(&bIsOk);	//	exp

		if	(iExp	==	-1)
			continue;

		lpInfo->m_dw1StExp	=	-iExp;

		m_iQuestOperatorCount++;
	}

	return	TRUE;
}

BOOL
CServerTool::operateCorrectExpWork(int _iServer,char *_lpstrName,int _iYesterDayLevel,int _iCorrectExp,cFILE *_lpFile,char *_lpstrWord,BOOL _bIsWantSave)
{
	if	(_lpstrWord	==	NULL)
		_lpstrWord	=	" ";

	CAvatarManager	m_am;

	if	(!m_db.loadData(NULL,_lpstrName,&m_am))
		return FALSE;

	if	(m_am.m_iLoadDataCount	<=	0)
		return	FALSE;

	CAvatarData	*lpPlayer	=	&m_am.m_aData[0];

	if	(lpPlayer->m_iLevel	<	_iYesterDayLevel)
	{
		_lpFile->writeStreamCR("\"%s\"/\"%s\" \"%s\" -1 캐릭터 삭제 후 생성 함",lpPlayer->m_strId,lpPlayer->m_strName,g_strJobName[lpPlayer->m_wJob]);
		_lpFile->writeStreamCR(" ");

		return	FALSE;
	}

	lpPlayer->operateCorrectExpWork050706(_iCorrectExp,_lpFile,_lpstrWord);

	if	(_bIsWantSave)
	{
		lpPlayer->m_dwSaveCount++;

		if	(!m_db.saveData((cP_PLAYER_SAVE_DATA *)lpPlayer))
			_lpFile->writeStreamCR("%s/%s DB에저장실패",lpPlayer->m_strId,lpPlayer->m_strName);
	}

	return	TRUE;
}

char	strId[10000][dID_LENGTH];
int		iNewIDCount	=	0;

BOOL	
IsExistId(char *_lpstrId)
{
	for	(int i=iNewIDCount;i;)
	{
		--i;

		if	(stricmp(strId[i],_lpstrId)==0)
			return	TRUE;
	}

	return	FALSE;
}

void CServerTool::OnCheckQuest50708() 
{
#ifndef _FOR_KOREA
	return;
#endif
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

	class	cNominataionList
	{
	public:
		char	m_strId[dID_LENGTH];
		char	m_strName[20];
		char	m_strEMail[50];
		char	m_strId2[dID_LENGTH];
		char	m_strName2[20];
		char	m_strEMail2[50];

		char	m_strServer[30];
		char	m_strTopLevelCharacter[dNAME_LENGTH];
		int		m_iTopLevelCharacterLevel;
	};

	cNominataionList	*pList	=	new cNominataionList [2000];
	int					iIndex	=	0,iCount;
	CAvatarManager		m_am;

	cDECODER	decoder;

	if	(!decoder.Upload("추천인이벤트명단.csv",NULL))
	{
		cMSG::Put("","추천인이벤트명단.csv 파일이 없다");
		return;
	}

	while(1)
	{
		BOOL	bIsEnd;
		char*				lpstrWord	=	decoder.getCSVWord(&bIsEnd);

		if	(bIsEnd	||	(lpstrWord	&&	stricmp(lpstrWord,"begin_data")	==	0))
			break;
	}

	while(1)
	{
//	회원ID	회원이름	E-mail	추천인ID	추천인이름	추천인E-mail
		BOOL				bIsEnd;
		char*				lpstrId	=	decoder.getCSVWord(&bIsEnd);
		cNominataionList	*lpInfo	=	&pList[iIndex];

		if	(bIsEnd	==	TRUE)
			break;

		if	(!lpstrId)
			continue;

		lpInfo->m_iTopLevelCharacterLevel	=	0	;

		strcpy(lpInfo->m_strId,lpstrId);

		char*				lpstrName	=	decoder.getCSVWord(&bIsEnd);
		if	(lpstrName)
			strcpy(lpInfo->m_strName,lpstrName);

		char*				lpstrEMail	=	decoder.getCSVWord(&bIsEnd);
		if	(lpstrEMail)
			strcpy(lpInfo->m_strEMail,lpstrEMail);
	
		char*				lpstrID2	=	decoder.getCSVWord(&bIsEnd);
		if	(lpstrID2)
			strcpy(lpInfo->m_strId2,lpstrID2);

		char*				lpstrName2	=	decoder.getCSVWord(&bIsEnd);
		if	(lpstrName2)
			strcpy(lpInfo->m_strName2,lpstrName2);

		char*				lpstrEMail2	=	decoder.getCSVWord(&bIsEnd);
		if	(lpstrEMail2)
			strcpy(lpInfo->m_strEMail2,lpstrEMail2);

		decoder.getCSVWord();

		iIndex++;
	}

	iCount	=	iIndex;

	char	*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};
	int		iServerIndex		=	0;

	char	strDbName[256];

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

	for	(int iServer=0;iServer<10;iServer++)
	{
		if	(stricmp(strServerName[iServer],"end")==0)
			break;

		if	(!m_db.Connect(	sList[iServer].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
		{
			MessageBox(g_strToolMessage[50]);

			return;
		}

		for (int iIndex=0;iIndex<iCount;iIndex++)
		{
			cNominataionList	*lpInfo	=	&pList[iIndex];

			if	(!m_db.loadData(lpInfo->m_strId,NULL,&m_am))
				continue;

			if	(m_am.m_iLoadDataCount	<=	0)
				continue;

			for	(int iPlayer=0;iPlayer<m_am.m_iLoadDataCount;iPlayer++)
			{
				if	(m_am.m_aData[iPlayer].m_iLevel	>	lpInfo->m_iTopLevelCharacterLevel)
				{
					lpInfo->m_iTopLevelCharacterLevel	=	m_am.m_aData[iPlayer].m_iLevel;
					strcpy(lpInfo->m_strServer,strServerName[iServer]);
					strcpy(lpInfo->m_strTopLevelCharacter,m_am.m_aData[iPlayer].m_strName);
				}
			}
		}
	}

	cFILE	file;

	file.Open("추천인이벤트결과.txt","wt");

//	회원ID	회원이름	E-mail	추천인ID	추천인이름	추천인E-mail
	file.writeStream("회원ID	회원이름	E-mail	추천인ID	추천인이름	추천인E-mail	서버	캐릭터명	최고 레벨");

	for (iIndex=0;iIndex<iCount;iIndex++)
	{
		cNominataionList	*lpInfo	=	&pList[iIndex];

		if	(lpInfo->m_iTopLevelCharacterLevel	==	0)
			continue;

		file.writeStreamCR("%s	%s	%s	%s	%s	%s	%s	%s	%d",
			lpInfo->m_strId,lpInfo->m_strName,lpInfo->m_strEMail,lpInfo->m_strId2,lpInfo->m_strName2,lpInfo->m_strEMail2,
			lpInfo->m_strServer,lpInfo->m_strTopLevelCharacter,lpInfo->m_iTopLevelCharacterLevel);
	}

	file.Close();

	pKILL(pList);

	cMSG::Put("","작업 완료");

}


void CServerTool::OnCheckQuest50709() 
{
#ifndef _FOR_KOREA
	return;
#endif
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

	SetLogFolder();

	char	*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};
	int		i;
	cFILE	file;

	for	(i=0;i<10;i++)
	{
		if	(stricmp(strServerName[i],"end")==0)
			break;

		cDECODER	decoder;

		int		j;

		m_iQuestCount	=	0;

		for	(j=0;j<1024;j++)
			m_aQuestList[j].reset();

		for	(j=1;j<4;j++)
		{
			if	(!decoder.Upload(_ms("퀘스트 수행자 리스트 %dST [%s].txt",j,strServerName[i]),NULL))
				continue;

			cQuestOperatorInfo050708ForQuest	*lpAddQuest;

			while(1)
			{
				char	*lpstr	=	decoder.GetStream();
				BOOL	bResult;
				char	strId[dID_LENGTH],strName[dNAME_LENGTH];

				if	(stricmp(lpstr,"end")	==	0)
					break;

				if	(stricmp(lpstr,"quest count")	==	0)
					continue;

				strcpy(strId,lpstr);
				lpstr			=	decoder.GetStream();
				strcpy(strName,lpstr);

				lpAddQuest	=	getQuestInfoByUser(strName);

				strcpy(lpAddQuest->m_strId,strId);
				strcpy(lpAddQuest->m_strName,strName);

				lpAddQuest->m_iLevel	=	decoder.GetNumber(&bResult);

				cQuestInfo050708		questInfo;

				lpstr					=	decoder.GetStream();
				strcpy(questInfo.m_strName,lpstr);
				questInfo.m_iExp		=	decoder.GetNumber(&bResult);

				decoder.GetWord();
				
				questInfo.m_iBoostExp	=	decoder.GetNumber(&bResult);

				lpAddQuest->addQuest(&questInfo);

				Sleep(1);
			}
		}

		file.Open(_ms("퀘스트 수행자 리스트 최종 [%s].txt",strServerName[i]),"wt");

		int	iQuestTotalCount	=	0;

		for	(j=0;j<m_iQuestCount;j++)
		{
			cQuestOperatorInfo050708ForQuest	*lpQuest	=	&m_aQuestList[j];

			for	(int k=0;k<lpQuest->m_iQuestCount;k++)
			{
				file.writeStreamCR("[\"%s\"/\"%s\"] %d \"%s\" %d [boost %d％]",lpQuest->m_strId,lpQuest->m_strName,lpQuest->m_iLevel,
					lpQuest->m_aQuestList[k].m_strName,lpQuest->m_aQuestList[k].m_iExp,lpQuest->m_aQuestList[k].m_iBoostExp);
			}

			iQuestTotalCount	+=	lpQuest->m_iQuestCount;
			file.writeStreamCR("         - \"quest count\" %d,total exp %d",lpQuest->m_iQuestCount,lpQuest->m_iTotalExp);
		}
		
		file.writeStreamCR("\n\"end\"");
		file.writeStreamCR("-------------------------------------------------------");
		file.writeStreamCR("[%d]명의 유저가 [%d]개의 퀘스트를 수행함.",m_iQuestCount,iQuestTotalCount);

		file.Close();
	}

	cMSG::Put("완료","퀘스트 수행자 병합 완료 ");
}


void CServerTool::OnCheckQuest50710() 
{
	SetLogFolder();

	cFILE	file;
	char	*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};

	for	(int	i=0;i<10;i++)
	{
		if	(stricmp(strServerName[i],"end")==0)
			break;

		uploadCompleteExpWorkActorList(i);

		cDECODER	decoder;

		int		j;

		m_iQuestCount	=	0;

		for	(j=0;j<1024;j++)
			m_aQuestList[j].reset();

		if	(!decoder.Upload(_ms("퀘스트 수행자 리스트 최종 [%s].txt",strServerName[i]),NULL))
			continue;

		cQuestOperatorInfo050708ForQuest	*lpAddQuest;

		while(1)
		{
			char	*lpstr	=	decoder.GetStream();
			BOOL	bResult;
			char	strId[dID_LENGTH],strName[dNAME_LENGTH];

			if	(stricmp(lpstr,"end")	==	0)
				break;

			if	(stricmp(lpstr,"quest count")	==	0)
				continue;

			strcpy(strId,lpstr);
			lpstr			=	decoder.GetStream();
			strcpy(strName,lpstr);

			lpAddQuest	=	getQuestInfoByUser(strName);

			strcpy(lpAddQuest->m_strId,strId);
			strcpy(lpAddQuest->m_strName,strName);

			lpAddQuest->m_iLevel	=	decoder.GetNumber(&bResult);

			cQuestInfo050708		questInfo;

			lpstr					=	decoder.GetStream();
			strcpy(questInfo.m_strName,lpstr);
			questInfo.m_iExp		=	decoder.GetNumber(&bResult);

			decoder.GetWord();
			
			questInfo.m_iBoostExp	=	decoder.GetNumber(&bResult);

			lpAddQuest->addQuest(&questInfo);
		}

		file.Open(_ms("2,3,4 문제 퀘스트 경험치 보정자 리스트 [%s].txt",strServerName[i]),"wt");

		int	iPlayerCount		=	0;

		for	(j=0;j<m_iQuestCount;j++)
		{
			cQuestOperatorInfo050708ForQuest	*lpQuest	=	&m_aQuestList[j];

			cQuestOperatorInfo050708*	lpOperator	=	getQuestOperator(lpQuest->m_strName);

			if	(!lpOperator)
			{
				file.writeStreamCR("[\"%s\"/\"%s\"] [LV %d] %d new",lpQuest->m_strId,lpQuest->m_strName,lpQuest->m_iLevel,lpQuest->m_iTotalExp);
				iPlayerCount++;
			}
			else
			{
				if	(lpOperator->m_dw1StExp	<	(DWORD)lpQuest->m_iTotalExp)
				{
					iPlayerCount++;
					file.writeStreamCR("[\"%s\"/\"%s\"] [LV %d] %d add",lpQuest->m_strId,lpQuest->m_strName,lpQuest->m_iLevel,lpQuest->m_iTotalExp-lpOperator->m_dw1StExp);
				}
			}
		}
		
		file.writeStreamCR("\n\"end\"");
		file.writeStreamCR("-------------------------------------------------------");
		file.writeStreamCR("대상 [%d]명의 유저",iPlayerCount);

		file.Close();
	}

	cMSG::Put("완료","2,3,4 문제 퀘스트 경험치 보정자 리스트 완료 ");
}

void CServerTool::OnCheckQuest50711() 
{
#ifndef _FOR_KOREA
	return;
#endif

	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

	char	strDbName[256];

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

	SetLogFolder();

	char	*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};
	int		i;
	cFILE	file;

	for	(i=0;i<10;i++)
	{
		if	(stricmp(strServerName[i],"end")==0)
			break;

		cDECODER	decoder;

		if	(!decoder.Upload(_ms("2,3,4 문제 퀘스트 경험치 보정자 리스트 [%s].txt",strServerName[i]),NULL))
			continue;

		if	(!m_db.Connect(	sList[i+1].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
		{
			MessageBox(g_strToolMessage[50]);

			return;
		}

		file.Open(_ms("05.07.09 correct quest exp result [%s].txt",strServerName[i]),"wt",TRUE);

		char	strId[dID_LENGTH],strName[dNAME_LENGTH],strPetName[dNAME_LENGTH+2],strWord[20];
		int		iCorrectExp,iYesterDayLevel;
		BOOL	bResult;

		char	*lpstr;

		while(1)
		{
			lpstr	=	decoder.GetStream();

			if	(stricmp(lpstr,"end")	==	0)
				break;

			strcpy(strId,lpstr);

			lpstr			=	decoder.GetStream();

			strcpy(strName,lpstr);

			decoder.GetWord();
			
			iYesterDayLevel	=	decoder.GetNumber(&bResult);
			iCorrectExp		=	decoder.GetNumber(&bResult);
			lpstr			=	decoder.GetWord();

			strcpy(strWord,lpstr);

			operateCorrectExpWork(i+1,strName,iYesterDayLevel,iCorrectExp,&file,strWord);

			Sleep(1);
		}

		file.writeStreamCR("\n\"사육 기록서 미처리 분\"\n");

		if	(decoder.Upload(_ms("사육 기록서 보유자 [%s].txt",strServerName[i]),NULL))
		{
			lpstr	=	decoder.GetStream();

			if	(stricmp(lpstr,"end")	==	0)
				break;

			strcpy(strId,lpstr);

			lpstr			=	decoder.GetStream();

			strcpy(strName,lpstr);

			lpstr			=	decoder.GetStream();
			strcpy(strPetName,lpstr);

			operateCorrectExpWork(i+1,strName,0,0,&file,"사육 기록서 미처리 분");
		}

		file.writeStreamCR("\"end\"");
		m_db.Close();

		file.Close();
	}

	cMSG::Put("완료","최종(이길 희망하는..) 경험치 보정 및 사육 기록서 펫 레벨 싱크 완료");
}

void CServerTool::OnCheckQuest50712() 
{
	char	strDbName[256];

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

	if	(!m_db.Connect(	sList[1].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
	{
		MessageBox(g_strToolMessage[50]);

		return;
	}
	
	cFILE	file;

	file.Open("싹3러버려.txt","wt");

	operateCorrectExpWork(1,"싹3러버려",1,493200000,&file);

	file.Close();

	m_db.Close();
/*
	SetLogFolder();

	cFILE	file;
	char	*strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};

	for	(int	i=0;i<10;i++)
	{
		if	(stricmp(strServerName[i],"end")==0)
			break;

		uploadCompleteExpWorkActorList(i,9);

		if	(!file.Open(_ms("05.07.09 correct quest exp result [%s].txt",strServerName[i]),"rb+"))
			continue;

		file.Seek(0,SEEK_END);

		file.writeStreamCR("\n-------------------------------------------------------------------------------");
		file.writeStreamCR("경험치가 하락된 유저 수 %d\n경험치가 하락된 펫 수 %d\n레벨에 맞게 착용 레벨을 변경한 아이템 수 %d",m_iQuestOperatorCount,m_iQuestReversionItemCount,m_iQuestLevelDownPetCount);

	}
*/
	cMSG::Put("완료","2,3,4 문제 퀘스트 경험치 보정자 리스트 완료 ");
	
}

// 
// BOOL
// CServerTool::ConnectAllServerDB()
// {
// 	char	strDbName[256];
// 
// 	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));
// 
// 	for	(int iServer=0;iServer<10;iServer++)
// 	{
// 		if	(stricmp(GetServerNameForEvent(iServer),"end")==0)
// 			break;
// 
// 		if	(m_abDBConnetionInfo[iServer])
// 			continue;
// 
// 		if	(!m_aDb[iServer].Connect(sList[iServer].strDBIP,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
// 			return	FALSE;
// 
// 		m_abDBConnetionInfo[iServer]	=	TRUE;
// 	}
// 
// 	return	TRUE;
// }
// 
// char*
// CServerTool::GetMaxLevelActor(int *_lpiServerIndex,int *_lpiMaxLevel,char *_lpstrId,int *_lpiResult)
// {
// 	static	char	strName[dNAME_LENGTH];
// 
// 	*_lpiMaxLevel	=	0;
// 	*_lpiServerIndex=	-1;
// 
// 	for	(int iServer=0;iServer<10;iServer++)
// 	{
// 		if	(stricmp(GetServerNameForEvent(iServer),"end")==0)
// 			break;
// 
// 		if	(!m_aDb[iServer].loadData(_lpstrId,NULL,&m_commonAm))
// 		{
// 			*_lpiResult	=	eCanNotFindID;
// 			return	NULL;
// 		}
// 
// 		for (int iSlot=0;iSlot<m_commonAm.m_iLoadDataCount;iSlot++)
// 		{
// 			if	(m_commonAm.m_aData[iSlot].m_iLevel	>	*_lpiMaxLevel)
// 			{
// 				*_lpiServerIndex	=	iServer;
// 				*_lpiMaxLevel		=	m_commonAm.m_aData[iSlot].m_iLevel;
// 
// 				strcpy(strName,m_commonAm.m_aData[iSlot].m_strName);
// 			}
// 		}
// 	}
// 
// 	*_lpiResult	=	eResult_OK;
// 
// 	if	(*_lpiMaxLevel	==	0)
// 		return	NULL;
// 
// 	return	strName;
// }
// 
// int
// CServerTool::GetCurrentLevelForEvent090720(int _iLevel)
// {
// 	int	iCurrentCheckLevel	=	-1;
// 
// 	for (int iLevelGrade=0;;iLevelGrade++)
// 	{
// 		if	(_iLevel	>=	m_rookieEventProcessInfo.m_aiCheckLevel[iLevelGrade])
// 		{
// 			iCurrentCheckLevel	=	m_rookieEventProcessInfo.m_aiCheckLevel[iLevelGrade];
// 			break;
// 		}
// 	}
// 
// 	return	iCurrentCheckLevel;
// }

// BOOL
// CServerTool::read090720RookieEventExistText(char *_lpstrFileName,c090720RookieEventInfo *_lpList,int _iMaxCount,cFILE *_lpLogFile) 
// {
// 	cDECODER	decoder;
// 
// 	if	(!decoder.Upload(_ms("%s.txt",_lpstrFileName),NULL))
// 	{
// 		cMSG::Error("","[%s] 파일 없어!!",_lpstrFileName);
// 
// 		return	FALSE;
// 	}
// 
// 	int		iErrorCount	=	0;
// 
// 	m_rookieEventProcessInfo.m_iTargetCount				=	0;
// 	m_rookieEventProcessInfo.m_iInvalidCharacterCount	=	0;
// 
// 	//	기존 문서 로딩
// 	for(;;m_rookieEventProcessInfo.m_iTargetCount++)
// 	{
// 		c090720RookieEventInfo	*lpCurrent	=	&_lpList[m_rookieEventProcessInfo.m_iTargetCount];
// 
// 		char	*lpstrWord;
// 
// 		//	신규 계정
// 		{
// 			lpstrWord	=	decoder.GetStream(9,10);
// 
// 			if	(!lpstrWord)
// 				break;
// 
// 			strcpy(lpCurrent->m_strRookieID,lpstrWord);
// 		}
// 
// 		//	추천인 계정
// 		{
// 			lpstrWord	=	decoder.GetStream(9,10);
// 
// 			if	(lpstrWord)
// 				strcpy(lpCurrent->m_strCallerID,lpstrWord);
// 
// 			if	(decoder.getLastSymbol()	==	10)
// 				continue;
// 		}
// 
// 		//	지급 캐릭터(신규) 이름
// 		{
// 			lpstrWord	=	decoder.GetStream(9,10);
// 
// 			if	(lpstrWord)
// 				strcpy(lpCurrent->m_strReceiveRookieName,lpstrWord);
// 			else
// 			if	(lpCurrent->m_strCallerID[0])
// 				m_rookieEventProcessInfo.m_iInvalidCharacterCount++;
// 
// 			if	(decoder.getLastSymbol()	==	10)
// 				continue;
// 		}
// 
// 		//	지급 캐릭터(신규) 서버
// 		{
// 			lpstrWord	=	decoder.GetStream(9,10);	//	
// 
// 			if	(lpstrWord)
// 			{
// 				lpCurrent->m_iReceiveRookieServer	=	GetServerIndexForEvent(lpstrWord);
// 
// 				if	(lpCurrent->m_iReceiveRookieServer	==	-1)
// 				{
// 					_lpLogFile->writeStreamCR("%s 추천인 서버 이름 이상 [%s]",lpCurrent->m_strRookieID,lpstrWord);
// 					iErrorCount++;
// 				}
// 			}
// 
// 			if	(decoder.getLastSymbol()	==	10)
// 				continue;
// 		}
// 
// 		//	지급 캐릭터(추천인) 이름
// 		{
// 			lpstrWord	=	decoder.GetStream(9,10);
// 
// 			if	(lpstrWord)
// 				strcpy(lpCurrent->m_strReceiveCallerName,lpstrWord);
// 			else
// 				m_rookieEventProcessInfo.m_iInvalidCharacterCount++;
// 
// 			if	(decoder.getLastSymbol()	==	10)
// 				continue;
// 		}
// 
// 		//	지급 캐릭터(추천인) 서버
// 		{
// 			lpstrWord	=	decoder.GetStream(9,10);	//	
// 
// 			if	(lpstrWord)
// 			{
// 				lpCurrent->m_iReceiveCallerServer	=	GetServerIndexForEvent(lpstrWord);
// 
// 				if	(lpCurrent->m_iReceiveCallerServer	==	-1)
// 				{
// 					_lpLogFile->writeStreamCR("%s:%s 지급인 서버 이름 이상 [%s]",lpCurrent->m_strRookieID,lpCurrent->m_strCallerID,lpstrWord);
// 					iErrorCount++;
// 				}
// 			}
// 
// 			if	(decoder.getLastSymbol()	==	10)
// 				continue;
// 		}
// 
// 		//	지급 레벨(신규)
// 		{
// 			lpstrWord	=	decoder.GetStream(9,10);	//
// 			
// 			if	(lpstrWord)
// 				lpCurrent->m_iReceivedTimeRookieLevelForRookie	=	atoi(lpstrWord);
// 		}
// 
// 		//	지급 레벨(추천인)
// 		{
// 			lpstrWord	=	decoder.GetStream(9,10);	//
// 
// 			if	(lpstrWord)
// 				lpCurrent->m_iReceivedTimeRookieLevelForCaller	=	atoi(lpstrWord);
// 		}
// 	}
// 
// 	if	(iErrorCount)
// 	{
// 		cMSG::Error("문서 로딩 에러!!","문서 로딩중 %d개의 문제가 발견되었습니다.\n\n%s_로그.txt 파일을 확인해 주세요.",iErrorCount,_lpstrFileName);
// 
// 		return	FALSE;
// 	}
// 
// 	return	TRUE;
// }
// 
// BOOL
// CServerTool::build090720RookieEvent(char *_lpstrFileName,c090720RookieEventInfo *_lpList,cFILE *_lpLogFile)
// {
// 	cFILE	exportFile;
// 
// 	exportFile.Open(_ms("%s_빌드.txt",_lpstrFileName),"wb",TRUE);
// 	exportFile.writeStreamCR("신규 가입	추천인	지급 캐릭터(신규)	서버(신규)	지급 캐릭터(추천인)	서버(추천인)	지급된 레벨(신규)	지급된 레벨(추천인)");
// 
// 	DWORD	dwBeginTime		=	timeGetTime();
// 	DWORD	dwTotalPastTime	=	0;
// 	int		iAverageTime	=	0;
// 	int		iAverageTimeZeroCount	=	0;
// 	int		iSerchCount		=	0;
// 	int		iErrorCount		=	0;
// 
// 	//	기존 문서 로딩
// 	for(int iIndex=0;iIndex<m_rookieEventProcessInfo.m_iTargetCount;iIndex++)
// 	{
// 		c090720RookieEventInfo	*lpCurrent	=	&_lpList[iIndex];
// 		int		iHitCount	=	0;
// 
// 		if	(lpCurrent->m_strReceiveRookieName[0]	==	NULL)
// 		{
// 			int		iServer,iTopLevel,iResult;
// 			char*	lpstrName	=	GetMaxLevelActor(&iServer,&iTopLevel,lpCurrent->m_strRookieID,&iResult);
// 
// 			iHitCount++;
// 			iSerchCount++;
// 
// 			switch(iResult)
// 			{
// 				case	eCanNotConnectServer	:
// 					iErrorCount	++;
// 					_lpLogFile->writeStreamCR("[%s] 서버 연결 실패",lpCurrent->m_strRookieID);
// 					continue;
// 				case	eCanNotFindID			:
// 					iErrorCount	++;
// 					_lpLogFile->writeStreamCR("[%s] 신규 계정 검색 실패",lpCurrent->m_strRookieID);
// 					write090720Info(lpCurrent,&exportFile);
// 					continue;
// 			}
// 
// 			if	(GetCurrentLevelForEvent090720(iTopLevel)	==	0)
// 			{
// 				Printf("#%4d/%d [%8s:%20s:%18s] [%8s:%20s:%18s] ???분 ??초\n",iIndex,m_rookieEventProcessInfo.m_iTargetCount,
// 							GetServerNameForEvent(lpCurrent->m_iReceiveRookieServer),lpCurrent->m_strRookieID,lpCurrent->m_strReceiveRookieName,
// 							GetServerNameForEvent(lpCurrent->m_iReceiveCallerServer),lpCurrent->m_strCallerID,lpCurrent->m_strReceiveCallerName);
// 
// 				write090720Info(lpCurrent,&exportFile);
// 
// 				continue;
// 			}
// 
// 			if	(iTopLevel)
// 			{
// 				strcpy(lpCurrent->m_strReceiveRookieName,lpstrName);
// 				lpCurrent->m_iReceiveRookieServer	=	iServer;
// 			}
// 		}
// 
// 		if	(lpCurrent->m_strCallerID[0] && lpCurrent->m_strReceiveCallerName[0] == NULL)
// 		{
// 			int		iServer,iTopLevel,iResult;
// 			char*	lpstrName	=	GetMaxLevelActor(&iServer,&iTopLevel,lpCurrent->m_strCallerID,&iResult);
// 
// 			iHitCount++;
// 			iSerchCount++;
// 
// 			switch(iResult)
// 			{
// 				case	eCanNotConnectServer	:
// 					iErrorCount	++;
// 					_lpLogFile->writeStreamCR("[%s] 서버 연결 실패",lpCurrent->m_strRookieID);
// 					continue;
// 				case	eCanNotFindID			:
// 				{
// 					iErrorCount++;
// 					_lpLogFile->writeStreamCR("[%s][%s] 추천자 계정 검색 실패",lpCurrent->m_strRookieID,lpCurrent->m_strCallerID);
// 					write090720Info(lpCurrent,&exportFile);
// 					continue;
// 				}
// 			}
// 
// 			if	(iTopLevel)
// 			{
// 				strcpy(lpCurrent->m_strReceiveCallerName,lpstrName);
// 				lpCurrent->m_iReceiveCallerServer	=	iServer;
// 			}
// 		}
// 
// 		int	iPastTime	=	timeGetTime()-dwBeginTime;
// 		int	iRemainTime	=	0;
// 
// 		if	(iSerchCount)
// 		{
// 			iAverageTime	=	iPastTime/iSerchCount;
// 
// 			LONGLONG	llRemainCount=	(m_rookieEventProcessInfo.m_iInvalidCharacterCount-iSerchCount);
// 
// 			llRemainCount*=	iAverageTime;
// 			iRemainTime	=	llRemainCount/1000;
// 		}
// 
// 		Printf("#%4d/%d [%8s:%20s:%18s] [%8s:%20s:%18s] %3d분 %.2d초 [%d]\n",iIndex,m_rookieEventProcessInfo.m_iTargetCount,
// 					GetServerNameForEvent(lpCurrent->m_iReceiveRookieServer),lpCurrent->m_strRookieID,lpCurrent->m_strReceiveRookieName,
// 					GetServerNameForEvent(lpCurrent->m_iReceiveCallerServer),lpCurrent->m_strCallerID,lpCurrent->m_strReceiveCallerName,
// 					iRemainTime/60,iRemainTime%60,iErrorCount);
// 
// 		write090720Info(lpCurrent,&exportFile);
// 	}
// 
// 	exportFile.Close();
// 
// 	if	(iErrorCount)
// 		cMSG::Error("문서 로딩 에러!!","문서 로딩중 %d개의 문제가 발견되었습니다.\n\n%s_로그.txt 파일을 확인해 주세요.",iErrorCount,_lpstrFileName);
// 
// 	return	TRUE;
// }
// 
// void
// CServerTool::OnRookieEvent090720() 
// {
// 	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
// 		return;
// 
// 	// Allocate a new console for our app
// 	if	(AllocConsole())
// 	{
// 		// Create the actual console
// 		g_hConsole	= CreateFile(_T("CONOUT$"), GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
// 
// 		if	(g_hConsole != INVALID_HANDLE_VALUE)
// 		{
// 			SetConsoleMode(g_hConsole	, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
// 
// 			SetConsoleTitle("event090720");
// 		}
// 	}
// 
// 	char		strFile[256]	=	"신규가입이벤트(090720)";
// 
// 	cFILE		logFile;
// 
// 	logFile.Open(_ms("%s_로그.txt",strFile),"wb",TRUE);
// 
// 	c090720RookieEventInfo	*pList		=	new c090720RookieEventInfo[10000];
// 
// 	if	(!read090720RookieEventExistText(strFile,pList,10000,&logFile))
// 		return;
// 
// 	if	(!ConnectAllServerDB())
// 	{
// 		cMSG::Error("DB 연결 실패!!","DB 연결에 실패 했습니다.");
// 
// 		return;
// 	}
// 
// 	if	(!build090720RookieEvent(strFile,pList,&logFile))
// 		return;
// 
// 	cMSG::Put("작업 완료","작업이 완료 되었습니다.\n\n가공된 데이터가 [%s_빌드.txt] 파일에 저장되었습니다.",strFile);
// 
// 	FreeConsole();
// }
// 
// BOOL
// CServerTool::give090720RookieEventItem(cADO_ADMIN *_lpDB,c090720RookieEventInfo *_lpInfo,int _iCurrentRookieLevel,CAvatarData *_lpData,BOOL _bIsRookie,cFILE *_lpLogFile)
// {
// 	cITEM	*alpItem			=	NULL;
// 	int		iAddItemCount		=	0;
// 	int		iAddBankItemCount	=	0;
// 	int		iReceivedTimeRookieLevel=	-1;
// 	char	*lpstrTargetID		=	NULL;
// 
// 	char	*strRookie[]		=	{"추천인","신규"};
// 
// 	if	(_bIsRookie)
// 	{
// 		alpItem				=	m_rookieEventProcessInfo.m_aRookieItem;
// 		lpstrTargetID		=	_lpInfo->m_strRookieID;
// 		iReceivedTimeRookieLevel	=	_lpInfo->m_iReceivedTimeRookieLevelForRookie;
// 	}
// 	else
// 	{
// 		alpItem				=	m_rookieEventProcessInfo.m_aCallerItem;
// 		lpstrTargetID		=	_lpInfo->m_strCallerID;
// 		iReceivedTimeRookieLevel	=	_lpInfo->m_iReceivedTimeRookieLevelForCaller;
// 	}
// 
// 	for (int i=0;;i++)
// 	{
// //		int	aiCheckLevelASC[]	=	{30,50,100,200,300,0xffff};
// 		int	iCheckLevel	=	m_rookieEventProcessInfo.m_aiCheckLevelASC[i];
// 
// 		if	(_iCurrentRookieLevel		<	iCheckLevel)
// 			break;
// 
// 		if	(iReceivedTimeRookieLevel	>=	iCheckLevel)
// 			continue;
// 
// 		BOOL			bIsAddToBank	=	FALSE;
// 
// 		CItemDefine		*lpInvalidSlot	=	_lpData->getValidItem();
// 
// 		if	(!lpInvalidSlot)
// 		{
// 			lpInvalidSlot	=	m_commonAm.getFreeSlotInBank();
// 			bIsAddToBank	=	TRUE;
// 		}
// 
// 		if	(!lpInvalidSlot)
// 		{
// 			_lpLogFile->writeStreamCR("지급 실패	%s [%s:%s] 빈 슬롯이 없어서 아이템 추가 실패",strRookie[_bIsRookie],lpstrTargetID,_lpData->m_strName);
// 
// 			return	FALSE;
// 		}
// 
// 		iAddItemCount++;
// 
// 		memcpy(lpInvalidSlot,&alpItem[i],sizeof(CItemDefine));
// 
// 		iAddBankItemCount	+=	bIsAddToBank;
// 
// 		if	(bIsAddToBank)
// 			m_commonAm.m_bIsChangeBankData	=	TRUE;
// 	}
// 
// 	if	(iAddItemCount)
// 	{
// 		_lpData->m_dwSaveCount++;
// 
// 		if	(!_lpDB->saveData((cP_PLAYER_SAVE_DATA *)_lpData))
// 		{
// 			_lpLogFile->writeStreamCR("지급 실패	%s [%s:%s] DB에 저장 실패",strRookie[_bIsRookie],lpstrTargetID,_lpData->m_strName);
// 
// 			return	FALSE;
// 		}
// 
// 		if	(_bIsRookie)
// 			_lpInfo->m_iReceivedTimeRookieLevelForRookie	=	_iCurrentRookieLevel;
// 		else
// 			_lpInfo->m_iReceivedTimeRookieLevelForCaller	=	_iCurrentRookieLevel;
// 
// 		_lpLogFile->writeStreamCR("아이템 지급	%s [%s:%s] [이전 지급 레벨 %d:현재 지급 레벨 %d] 아이템 %d개 추가[인벤 %d개,은행 %d개]",
// 								strRookie[_bIsRookie],lpstrTargetID,_lpData->m_strName,
// 								iReceivedTimeRookieLevel,_lpData->m_iLevel,
// 								iAddItemCount,iAddItemCount-iAddBankItemCount,iAddBankItemCount);
// 	}
// 	else
// 	{
// 		_lpLogFile->writeStreamCR("지급 안함	%s [%s:%s] [이전 지급 레벨 %d:현재 레벨 %d]",
// 								strRookie[_bIsRookie],lpstrTargetID,_lpData->m_strName,
// 								iReceivedTimeRookieLevel,_lpData->m_iLevel);
// 	}
// 
// 	return	TRUE;
// }
// 
// void
// CServerTool::build090720RookieEventRewardItem()
// {
// 	int	aiCheckLevel[]		=	{300,200,100,50,30,0};
// 	int	aiCheckLevelASC[]	=	{30,50,100,200,300,0xffff};
// 
// 	{
// 		int	iSize	=	0;
// 		int i;
// 
// 		for (i=0;;i++)
// 		{
// 			if	(aiCheckLevel[i]	!=	0)
// 				iSize++;
// 			else
// 				break;
// 		}
// 
// 		for (i=0;i<iSize;i++)
// 		{
// 			assert(aiCheckLevel[i] == aiCheckLevelASC[iSize-i-1]);
// 		}
// 
// 		assert(aiCheckLevelASC[iSize] == 0xffff);
// 	}
// 
// 	memcpy(m_rookieEventProcessInfo.m_aiCheckLevel,aiCheckLevel,sizeof(m_rookieEventProcessInfo.m_aiCheckLevel));
// 	memcpy(m_rookieEventProcessInfo.m_aiCheckLevelASC,aiCheckLevelASC,sizeof(m_rookieEventProcessInfo.m_aiCheckLevel));
// 
// 	for (int i=0;i<5;i++)
// 	{
// 		cITEM	*lpItem	=	&m_rookieEventProcessInfo.m_aRookieItem[i];
// 
// 		lpItem->reset();
// 
// 		switch(i)
// 		{
// 			case	0	:
// 				//	하드 피스트 [336:0] [1,16] [1,2] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
// 				lpItem->m_wBaseItem		=	336;
// 				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
// 				lpItem->m_bCount		=	1;
// 				lpItem->m_bDurability	=	16;
// 
// 				lpItem->m_aOption[0]	=	1;
// 				lpItem->m_aOption[1]	=	2;
// 				break;
// 
// 			case	1	:
// 				//	알파캡 [283:0] [1,25] [4,2] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
// 				lpItem->m_wBaseItem		=	283;
// 				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
// 				lpItem->m_bCount		=	1;
// 				lpItem->m_bDurability	=	25;
// 
// 				lpItem->m_aOption[0]	=	4;
// 				lpItem->m_aOption[1]	=	2;
// 				break;
// 
// 			case	2	:
// 				//	천리화 [315:0] [1,32] [3,10] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
// 				lpItem->m_wBaseItem		=	315;
// 				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
// 				lpItem->m_bCount		=	1;
// 				lpItem->m_bDurability	=	32;
// 
// 				lpItem->m_aOption[0]	=	3;
// 				lpItem->m_aOption[1]	=	10;
// 				break;
// 
// 			case	3	:
// 				//	버터플라이 스팅 [296:0] [1,20] [2,0] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
// 				lpItem->m_wBaseItem		=	296;
// 				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
// 				lpItem->m_bCount		=	1;
// 				lpItem->m_bDurability	=	20;
// 
// 				lpItem->m_aOption[0]	=	2;
// 				lpItem->m_aOption[1]	=	0;
// 				break;
// 
// 			case	4	:
// 				//	세이프 케이지 [353:0] [1,28] [10,0] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
// 				lpItem->m_wBaseItem		=	353;
// 				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
// 				lpItem->m_bCount		=	1;
// 				lpItem->m_bDurability	=	28;
// 
// 				lpItem->m_aOption[0]	=	10;
// 				lpItem->m_aOption[1]	=	0;
// 				break;
// 		}
// 
// 		lpItem	=	&m_rookieEventProcessInfo.m_aCallerItem[i];
// 
// 		lpItem->reset();
// 
// 		switch(i)
// 		{
// 			case	0	:
// 				//	브룬의 국새 [333:0] [1,34] [10,0] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
// 				lpItem->m_wBaseItem		=	333;
// 				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
// 				lpItem->m_bCount		=	1;
// 				lpItem->m_bDurability	=	34;
// 
// 				lpItem->m_aOption[0]	=	10;
// 				lpItem->m_aOption[1]	=	0;
// 				break;
// 
// 			case	1	:
// 				//	루시드 헬름 [286:0] [1,45] [6,10] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
// 				lpItem->m_wBaseItem		=	286;
// 				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
// 				lpItem->m_bCount		=	1;
// 				lpItem->m_bDurability	=	45;
// 
// 				lpItem->m_aOption[0]	=	6;
// 				lpItem->m_aOption[1]	=	10;
// 				break;
// 
// 			case	2	:
// 				//	스피릿 오브 커머셜 [313:0] [1,16] [1,6] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
// 				lpItem->m_wBaseItem		=	313;
// 				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
// 				lpItem->m_bCount		=	1;
// 				lpItem->m_bDurability	=	16;
// 
// 				lpItem->m_aOption[0]	=	1;
// 				lpItem->m_aOption[1]	=	6;
// 				break;
// 
// 			case	3	:
// 				//	핸즈 오브 애드버서리 [292:0] [1,30] [2,0] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
// 				lpItem->m_wBaseItem		=	292;
// 				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
// 				lpItem->m_bCount		=	1;
// 				lpItem->m_bDurability	=	30;
// 
// 				lpItem->m_aOption[0]	=	2;
// 				lpItem->m_aOption[1]	=	0;
// 				break;
// 
// 			case	4	:
// 				//	세이프 케이지 [353:0] [1,28] [10,0] [65535:255,255] [65535:255,255] [65535:255,255] 6 expireDate 0/0/0 0:0
// 				lpItem->m_wBaseItem		=	353;
// 				lpItem->m_dwSerial		=	largeRandom(0x7fffffff-2)+2;
// 				lpItem->m_bCount		=	1;
// 				lpItem->m_bDurability	=	28;
// 
// 				lpItem->m_aOption[0]	=	10;
// 				lpItem->m_aOption[1]	=	0;
// 				break;
// 		}
// 	}
// }
// 
// void
// CServerTool::write090720Info(c090720RookieEventInfo *_lpInfo,cFILE *_lpLogFile)
// {
// 	_lpLogFile->writeStreamCR("%s	%s	%s	%s	%s	%s	%d	%d",
// 						_lpInfo->m_strRookieID,_lpInfo->m_strCallerID,
// 						_lpInfo->m_strReceiveRookieName,GetServerNameForEvent(_lpInfo->m_iReceiveRookieServer),
// 						_lpInfo->m_strReceiveCallerName,GetServerNameForEvent(_lpInfo->m_iReceiveCallerServer),
// 						_lpInfo->m_iReceivedTimeRookieLevelForRookie,_lpInfo->m_iReceivedTimeRookieLevelForCaller);
// }
// 
// BOOL
// CServerTool::run090720RookieEvent(char *_lpstrFileName,c090720RookieEventInfo *_lpList,cFILE *_lpLogFile)
// {
// 	DWORD	dwBeginTime		=	timeGetTime();
// 	DWORD	dwTotalPastTime	=	0;
// 	int		iAverageTime	=	0;
// 	int		iAverageTimeZeroCount	=	0;
// 	int		iSerchCount		=	0;
// 	int		iErrorCount		=	0;
// 
// 	cFILE	exportFile;
// 
// 	exportFile.Open(_ms("%s_지급.txt",_lpstrFileName),"wb",TRUE);
// 	exportFile.writeStreamCR("신규 가입	추천인	지급 캐릭터(신규)	서버(신규)	지급 캐릭터(추천인)	서버(추천인)	지급된 레벨(신규)	지급된 레벨(추천인)");
// 
// 	//	기존 문서 로딩
// 	for(int iIndex=0;iIndex<m_rookieEventProcessInfo.m_iTargetCount;iIndex++)
// 	{
// 		c090720RookieEventInfo	*lpCurrent	=	&_lpList[iIndex];
// 
// 		int		iHitCount	=	0;
// 
// 		Printf("operate #%4d/%d %s\n",iIndex,m_rookieEventProcessInfo.m_iTargetCount,lpCurrent->m_strRookieID);
// 
// 		if	(lpCurrent->m_strReceiveRookieName[0]	==	NULL)
// 		{
// 			_lpLogFile->writeStreamCR("캐릭터 없음	[%s] 레벨 30이상 신규 캐릭터 없어서 지급 안함",lpCurrent->m_strRookieID);
// 
// 			write090720Info(lpCurrent,&exportFile);
// 
// 			continue;
// 		}
// 
// 		if	(!m_aDb[lpCurrent->m_iReceiveRookieServer].loadData(NULL,lpCurrent->m_strReceiveRookieName,&m_commonAm) || m_commonAm.m_iLoadDataCount == 0)
// 		{
// 			_lpLogFile->writeStreamCR("신규 검색 실패	[%s] [%s:%s] 캐릭터 검색 실패",
// 				lpCurrent->m_strRookieID,GetServerNameForEvent(lpCurrent->m_iReceiveRookieServer),lpCurrent->m_strReceiveRookieName);
// 
// 			write090720Info(lpCurrent,&exportFile);
// 			continue;
// 		}
// 
// 		int	iRookieLevel	=	m_commonAm.m_aData[0].m_iLevel;
// 
// 		give090720RookieEventItem(&m_aDb[lpCurrent->m_iReceiveRookieServer],lpCurrent,iRookieLevel,&m_commonAm.m_aData[0],TRUE,_lpLogFile);
// 
// 		if	(lpCurrent->m_strReceiveCallerName[0]	==	NULL)
// 		{
// 			write090720Info(lpCurrent,&exportFile);
// 
// 			continue;
// 		}
// 
// 		if	(!m_aDb[lpCurrent->m_iReceiveCallerServer].loadData(NULL,lpCurrent->m_strReceiveCallerName,&m_commonAm) || m_commonAm.m_iLoadDataCount == 0)
// 		{
// 			_lpLogFile->writeStreamCR("추천인 검색 실패	[%s] [%s:%s] 캐릭터 검색 실패",
// 				lpCurrent->m_strRookieID,GetServerNameForEvent(lpCurrent->m_iReceiveCallerServer),lpCurrent->m_strReceiveCallerName);
// 
// 			write090720Info(lpCurrent,&exportFile);
// 
// 			continue;
// 		}
// 
// 		give090720RookieEventItem(&m_aDb[lpCurrent->m_iReceiveCallerServer],lpCurrent,iRookieLevel,&m_commonAm.m_aData[0],FALSE,_lpLogFile);
// 
// 		write090720Info(lpCurrent,&exportFile);
// 	}
// 
// 	exportFile.Close();
// 
// 	if	(_lpLogFile->Length)
// 		cMSG::Error("에러!!","작업 처리중 문제가 발견되었습니다.\n\n%s_로그.txt 파일을 확인해 주세요.",_lpstrFileName);
// 
// 	return	TRUE;
// }
// 
// void CServerTool::OnRookieEventRun090720() 
// {
// 	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
// 		return;
// 
// 	build090720RookieEventRewardItem();
// 
// 	// Allocate a new console for our app
// 	if	(AllocConsole())
// 	{
// 		// Create the actual console
// 		g_hConsole	= CreateFile(_T("CONOUT$"), GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
// 
// 		if	(g_hConsole != INVALID_HANDLE_VALUE)
// 		{
// 			SetConsoleMode(g_hConsole	, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
// 
// 			SetConsoleTitle("event090720");
// 		}
// 	}
// 
// 	char		strFile[256]	=	"신규가입이벤트(090720)_빌드";
// 
// 	cFILE		logFile;
// 
// 	logFile.Open(_ms("%s_로그.txt",strFile),"wb",TRUE);
// 
// 	c090720RookieEventInfo	*pList		=	new c090720RookieEventInfo[10000];
// 
// 	if	(!read090720RookieEventExistText(strFile,pList,10000,&logFile))
// 		return;
// 
// 	if	(!ConnectAllServerDB())
// 	{
// 		cMSG::Error("DB 연결 실패!!","DB 연결에 실패 했습니다.");
// 
// 		return;
// 	}
// 
// 	if	(!run090720RookieEvent(strFile,pList,&logFile))
// 		return;
// 
// 	cMSG::Put("작업 완료","작업이 완료 되었습니다.\n\n가공된 데이터가 [%s_지급.txt] 파일에 저장되었습니다.",strFile);
// 
// 	FreeConsole();
// }
