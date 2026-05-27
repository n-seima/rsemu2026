#include "CSaveData.h"
#include "Client.h"
CDataSaveManager	g_dsManager;

CDataSaveManager::CDataSaveManager()
{
}	//	CDataSaveManager::CDataSaveManager()

CDataSaveManager::~CDataSaveManager()
{
}	//	CDataSaveManager::~CDataSaveManager()

//
//	초기화
void
CDataSaveManager::init(char *_lpstrPath,char *_lpstrListPath,char *_lpstrBankPath)
{
	strcpy(m_strPath,_lpstrPath);
	strcpy(m_strListPath,_lpstrListPath);
	strcpy(m_strBankPath,_lpstrBankPath);
}	//	CDataSaveManager::init(char *_lpstrPath,char *_lpstrListPath,char *_lpstrBankPath,char *_lpstrGuildPath)

//
//	데이터 세이브
int
CDataSaveManager::save(cPLAYER_DATA *_lpData)
{
	if	(_lpData->m_wJob	>	dPLAYER_JOB_COUNT	|| strcmp(_lpData->m_strId,"")==0)
	{
#ifdef _FOR_KOREA
		_log("__DB SAVE 실패 : eSAVE_DB_INCORRECT_DATA [%s/%s:SC %d] [LV %d/%d] [JOB %d] [FIELD %d]",_lpData->m_strId,_lpData->m_strName,_lpData->m_dwSaveCount,_lpData->m_iLevel,_lpData->m_iExperience,_lpData->m_wJob,_lpData->m_wCurrentField);
#endif

		return eSAVE_DB_INCORRECT_DATA;
	}

	int	iResult		=	g_adoAvatar.SaveAva(_lpData,_lpData->m_dwPlayTime,_lpData->m_bf1IsLevelDown);

	switch(iResult)
	{
		case	eSAVE_SUCCESS_DB		:
//			_log("DB SAVE성공 : [%s/%s:SC %d] [LV %d/%d] [JOB %d] [FIELD %d]",_lpData->m_strId,_lpData->m_strName,_lpData->m_dwSaveCount,_lpData->m_iLevel,_lpData->m_iExperience,_lpData->m_wJob,_lpData->m_wCurrentField);
			break;

		case	eSAVE_FAIL_LITTLECOUNT	:
			_logc("DB SAVE fail : eSAVE_FAIL_LITTLECOUNT [%s/%s:SC %d] [LV %d/%d] [JOB %d] [FIELD %d]",_lpData->m_strId,_lpData->m_strName,_lpData->m_dwSaveCount,_lpData->m_iLevel,_lpData->m_iExperience,_lpData->m_wJob,_lpData->m_wCurrentField);
			break;

		case	eSAVE_CAN_NOT_FIND_DATA	:
			_logc("__DB SAVE fail : eSAVE_CAN_NOT_FIND_DATA [%s/%s:SC %d] [LV %d/%d] [JOB %d] [FIELD %d]",_lpData->m_strId,_lpData->m_strName,_lpData->m_dwSaveCount,_lpData->m_iLevel,_lpData->m_iExperience,_lpData->m_wJob,_lpData->m_wCurrentField);
			break;

		case	eSAVE_DB_FAILED			:
			_log("__DB SAVE fail : eSAVE_DB_FAILED [%s/%s:SC %d] [LV %d/%d] [JOB %d] [FIELD %d]",_lpData->m_strId,_lpData->m_strName,_lpData->m_dwSaveCount,_lpData->m_iLevel,_lpData->m_iExperience,_lpData->m_wJob,_lpData->m_wCurrentField);
			break;

		case	eSAVE_DB_INCORRECT_DATA	:
			_logf("__DB SAVE fail : eSAVE_DB_INCORRECT_DATA [%s/%s:SC %d] [LV %d/%d] [JOB %d] [FIELD %d]",_lpData->m_strId,_lpData->m_strName,_lpData->m_dwSaveCount,_lpData->m_iLevel,_lpData->m_iExperience,_lpData->m_wJob,_lpData->m_wCurrentField);
			break;

		case	eSAVE_DB_UNKNOWN_ERROR	:
			_logf("__DB SAVE fail : eSAVE_DB_UNKNOWN_ERROR [%s/%s:SC %d] [LV %d/%d] [JOB %d] [FIELD %d]",_lpData->m_strId,_lpData->m_strName,_lpData->m_dwSaveCount,_lpData->m_iLevel,_lpData->m_iExperience,_lpData->m_wJob,_lpData->m_wCurrentField);
			break;

		case	eSAVE_DB_DATA_NULL		:
			_logf("__DB SAVE fail : eSAVE_DB_DATA_NULL [%s/%s:SC %d] [LV %d/%d] [JOB %d] [FIELD %d]",_lpData->m_strId,_lpData->m_strName,_lpData->m_dwSaveCount,_lpData->m_iLevel,_lpData->m_iExperience,_lpData->m_wJob,_lpData->m_wCurrentField);
			break;

		default							:
			_log("__DB SAVE fail : default?? [%s/%s:SC %d] [LV %d/%d] [JOB %d] [FIELD %d]",_lpData->m_strId,_lpData->m_strName,_lpData->m_dwSaveCount,_lpData->m_iLevel,_lpData->m_iExperience,_lpData->m_wJob,_lpData->m_wCurrentField);
			break;
	}

	return	iResult;
}	//	cACTOR::saveData(BOOL _bIsBackup)

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	FOR BANK
BOOL
CDataSaveManager::openBank(char * _lpstrId,CBankInfo * bank)
{
	g_adoAvatar.LoadBankInfo(_lpstrId,bank);
	return TRUE;
}

WORD
CDataSaveManager::closeBank(CBankInfo * bank)
{
	WORD	wResult = g_adoAvatar.SaveBank(bank);
	switch(wResult)
	{
	case 2:		//	정상 저장
		{
			char	strFileName[512];
			sprintf(strFileName,"%s/%s.bank",m_strBankPath,bank->m_strId);
			FILE * fp = NULL;
			fp = fopen(strFileName,"wb");
			if(fp)
			{
				fwrite(bank,sizeof(CBankInfo),1,fp);
				fclose(fp);	fp = NULL;
			}
			wResult = eRBDS_SUCCESS;
			break;
		}
	case 1:		//	카운터 문제
		wResult = eRBDS_INCORRECT_COUNTER;
		_log("ERROR BANK SAVE <COUNT>: [%d] [%s]",wResult,bank->m_strId);
		break;
	case 0:		//	저장에 문제가 생김.
		wResult = eRBDS_FAILED;
		_log("ERROR BANK SAVE <UNKNOWN>: [%d] [%s]",wResult,bank->m_strId);
		break;
	}
	return wResult;
}

void
CDataSaveManager::delBank(char * _lpstrId)
{
	char	strFileName[512];
	sprintf(strFileName,"%s/%s.bank",m_strBankPath,_lpstrId);

	DeleteFile(strFileName);
}

void
CDataSaveManager::updateBank(char * _lpstrId)
{
	CBankInfo		bank;
	char	strFileName[512];
	sprintf(strFileName,"%s/%s.bank",m_strBankPath,_lpstrId);
	FILE *fp = NULL;
	fp = fopen(strFileName,"rb");
	fread(&bank,sizeof(CBankInfo),1,fp);
	fclose(fp);
	if(STRICMP(bank.m_strId,_lpstrId)!=0)
	{
#ifdef _FOR_KOREA
		_log("++++++++++++++++++++ ERROR ERROR : UPDATEBANK [%s]가 [%s] 파일 호출??",_lpstrId,bank.m_strId);
#endif
		return ;
	}
	g_adoAvatar.SaveBank(&bank);
	DeleteFile(strFileName);
}
//┃	FOR BANK
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void
CDataSaveManager::makeDuelPlayer(cPLAYER_DATA	*_lpPlayer)
{
	_lpPlayer->m_wCurrentField	=	c_iDuelServerLobyIndex;
	_lpPlayer->m_wLastVillage	=	c_iDuelServerLobyIndex;
	_lpPlayer->m_iLevel			=	c_iLevelForDuelServer;

	_lpPlayer->resetStatePoint();
	_lpPlayer->resetSkillPoint();

	_lpPlayer->m_iGold			=	c_iDefaultGoldForDuelPlayer;

	for (int i=0;i<dEQUIPMENT_PART_COUNT;i++)
		_lpPlayer->m_aEquip[i].reset();
}

BOOL
CDataSaveManager::copyData(cAVATAR_INFO *_lpData,char *_lpstrId)
{
	cPLAYER_DATA	player;

	if	(_lpData->m_wJob	>=	dPLAYER_JOB_COUNT || _lpData->m_wLevel <= 0 || _lpData->m_wLevel >= 1000)
		return	FALSE;

	player.initializeJobData(_lpstrId,_lpData->m_strName,_lpData->m_wJob,_lpData->m_wLastField);

	player.m_iLevel	=	_lpData->m_wLevel;

	player.resetStatePoint();
	player.resetSkillPoint();
	player.m_wAvatarIndex = 0;
	player.m_wCurrentField = 0;

	player.m_iGold	=	player.m_iLevel*10000;

//	해당 인덱스에 뭔가 있다면 삭제한다.
	DWORD	dwDbIndex	=	0x7fffffff;
	DWORD	dwTableIndex=	g_Im.getTableIndexByAvaIndex(_lpstrId,0);

	if	(dwTableIndex < 0xffffffff)
	{	//	삭제한다.
		WORD	wGuildIndex = 0xffff;

		_log(" copyData : 0st index is exist : [%s, %d]",_lpstrId,dwTableIndex);

		int  result = g_adoAvatar.spDeleteAva(g_Im.idxTable[dwTableIndex].strId,g_Im.idxTable[dwTableIndex].strName,wGuildIndex);

		if	(result>=0 && result<4)
		{
			g_adoAvatar.spCreateAva(dwDbIndex,&player,g_Im.getBlankPos(_lpstrId));
		}
		else 
		if	(result==65533)
		{
			g_adoAvatar.spCreateAva(dwDbIndex,&player,g_Im.getBlankPos(_lpstrId));
		}
		else
		{
#ifdef _FOR_KOREA
			_log("ERROR copyData : [%s]의 데이타중 0번째 삭제 실패로 복사실패",_lpstrId);
#endif
		}
	}
	else
	{
		g_adoAvatar.spCreateAva(dwDbIndex,&player,g_Im.getBlankPos(_lpstrId));

		if	(dwDbIndex==0xffffffff)
		{	//	중복된 이름이 걸린다면..
			_log(" COPYDATA ERROR : 중복된 이름 [%s]",player.m_strName);

			return	FALSE;
		}
	}

	g_Im.removeAll(_lpstrId);
 	//	DB에서 id로 검색해서 가져오고.. 
	_DBINDEXTABLE	avaList;
	g_adoAvatar.GetDBIndex(_lpstrId,avaList);

	for(int i = 0;i<4;i++)
	{
		if	(avaList.wAvatarIndex[i]==0xffff)
			continue;

		int	iIndex	=	g_Im.getIndex(avaList.strName[i],_lpstrId);

		if	(iIndex	==	0x7fffffff)
		{
			if	(g_Im.add(avaList.dwIndex[i],avaList.strName[i],_lpstrId,avaList.wAvatarIndex[i]))
				_logf(" ADD DBINDEX : [%d] [%s] [%s] [%d]",avaList.dwIndex[i],avaList.strName[i],_lpstrId,avaList.wAvatarIndex[i]);
			else
				_logf(" ADD DBINDEX FAIL: [%d] [%s] [%s] [%d]",avaList.dwIndex[i],avaList.strName[i],_lpstrId,avaList.wAvatarIndex[i]);
		}
	}

	return	TRUE;
}