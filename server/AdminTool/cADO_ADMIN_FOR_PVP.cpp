#include "StdAfx.h"
#include "cADO_ADMIN.h"
#include <stdio.h>
#include <process.h>
#include "cSRVUTIL.h"
#include "RSComm.h"

void	cADO_ADMIN::spClearWCGRank()
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spINIT_WCG_TEAM";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@RESERVE") = _variant_t((long)1);	//	랭킹만 초기화 하기
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr)){		printf("Failed to clear wcg ranking!! \n");		return ;}

		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return ;
	}catch( _com_error &e){
		DMPcomERR(e)
		return ;
	}
}

int		cADO_ADMIN::spInsertWCGTeamList(char * _strTeamName)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spINSERT_WCG_TEAM";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@TEAMNAME") = _variant_t((char *)_strTeamName);
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr)){		printf("Failed to regist wcg team!! \n");		return 0xffff;}


		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RET")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);

		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;
	}catch( _com_error &e){
		DMPcomERR(e)
		return -1;
	}
}

int		cADO_ADMIN::spInsertWCGTeamUserList(int _idx, char * _id, char * _name, int _job)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spINSERT_WCG_TEAM_USER";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@IDX") = _variant_t((long)_idx);
		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)_id);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_name);
		DEF_PARAM(pCmd,"@JOB") = _variant_t((long)_job);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr)){		printf("Failed to regist wcg team!! \n");		return 0xffff;}


		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_I4;
		regIdx.iVal = pCmd->GetParameters()->GetItem("@RET")->Value;
		int iResult = (int)regIdx.iVal;
		VariantClear(&regIdx);

		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;
	}catch( _com_error &e){
		DMPcomERR(e)
		return -1;
	}
}


int		cADO_ADMIN::spGetWCGTeamList()
{
	_RecordsetPtr  pRs = NULL;
	char	       str_Query[1024];
	int i_RecCount,i;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT * FROM tbPvP_WCG_TeamList");
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;
	m_cWCGInfo.init(i_RecCount);
	m_mapWCGTeamIndex.clear();

	if(!FAILED(hr))
	{ 
		for(i = 0 ;i < i_RecCount ;i++)
		{
			VARIANT     v_bufferdata;
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			int teamindex = (int)v_bufferdata.intVal;
			m_mapWCGTeamIndex.insert(std::pair<int,int>(teamindex,i));
			m_cWCGInfo.m_pcInfo[i].m_wIndex = teamindex;

			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			strcpy(m_cWCGInfo.m_pcInfo[i].m_strTeamName,(_bstr_t)v_bufferdata);

			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			m_cWCGInfo.m_pcInfo[i].m_record.m_iWinPoint = (int)v_bufferdata.intVal;
			
			v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
			m_cWCGInfo.m_pcInfo[i].m_record.m_iBattleCount = (int)v_bufferdata.intVal;

			v_bufferdata = pRs->Fields->GetItem((LONG)4)->Value;
			m_cWCGInfo.m_pcInfo[i].m_record.m_iWinCount = (int)v_bufferdata.intVal;

			v_bufferdata = pRs->Fields->GetItem((LONG)5)->Value;
			m_cWCGInfo.m_pcInfo[i].m_record.m_iDefeatCount = (int)v_bufferdata.intVal;

			v_bufferdata = pRs->Fields->GetItem((LONG)6)->Value;
			m_cWCGInfo.m_pcInfo[i].m_record.m_iDrawCount = (int)v_bufferdata.intVal;
			
			m_cWCGInfo.m_pcInfo[i].m_wMemberCount = 0;
			
			VariantClear(&v_bufferdata);
			pRs->MoveNext();
		}
	}
	pRs->Close();
	return 0;
}

int		cADO_ADMIN::spGetWCGTeamUserList()//(WCG팀 리스트를 가져온 후) 각 팀에 소속된 인원들의 리스트를 가져온다.
{
	_RecordsetPtr  pRs = NULL;
	char	       str_Query[1024];
	int i_RecCount,i;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT * FROM tbPvP_WCG_TeamUserList");
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(!FAILED(hr))
	{ 
		for(i = 0 ;i < i_RecCount ;i++)
		{
			VARIANT     v_bufferdata;
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			int teamindex = (int)v_bufferdata.intVal;
			int idx = m_mapWCGTeamIndex[teamindex];
			int mcount = m_cWCGInfo.m_pcInfo[idx].m_wMemberCount;

			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			strcpy(m_cWCGInfo.m_pcInfo[idx].m_aMembers[mcount].m_strID,(_bstr_t)v_bufferdata);

			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			strcpy(m_cWCGInfo.m_pcInfo[idx].m_aMembers[mcount].m_strName,(_bstr_t)v_bufferdata);

			v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
			m_cWCGInfo.m_pcInfo[idx].m_aMembers[mcount].m_wJob = (int)v_bufferdata.intVal;

			++m_cWCGInfo.m_pcInfo[idx].m_wMemberCount;
	
			VariantClear(&v_bufferdata);
			pRs->MoveNext();
		}
	}
	pRs->Close();
	return 0;
}

void	cADO_ADMIN::spClearWCGTeamList()
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spINIT_WCG_TEAM";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@RESERVE") = _variant_t((long)2);	//	랭킹만 초기화 하기
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr)){		printf("Failed to clear wcg team list!! \n");		return ;}

		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return ;
	}catch( _com_error &e){
		DMPcomERR(e)
		return ;
	}
}

int		cADO_ADMIN::spGetWCGRank()	//WCG 랭킹 가져오기. 100위 까지만...
{
	char			str_Query[1024];
	DWORD			dw_RecCount;
	DWORD			i;
	HRESULT			hr = CoInitialize(0);

	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"SELECT TOP 100 strName, iPoint, iBattleCount, iWinCount, iDefeatCount, iDrawCount FROM tbPvP_Rank_WCG ORDER BY iPoint DESC, iWinCount DESC, iDefeatCount DESC, iDrawCount DESC, iBattleCount DESC");
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		dw_RecCount = pRs->RecordCount;

		if(FAILED(hr))	return 0;

		for(i = 0 ;i < dw_RecCount ;i++)
		{
			m_cWCGInfo.m_pcRank[i].m_wRank = (short)i;
			_variant_t		v_bufferdata;
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			strcpy(m_cWCGInfo.m_pcRank[i].m_strName,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			m_cWCGInfo.m_pcRank[i].m_iWinPoint = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			m_cWCGInfo.m_pcRank[i].m_iBattleCount = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
			m_cWCGInfo.m_pcRank[i].m_iWinCount = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)4)->Value;
			m_cWCGInfo.m_pcRank[i].m_iDefeatCount = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)5)->Value;
			m_cWCGInfo.m_pcRank[i].m_iDrawCount = (DWORD)v_bufferdata.intVal;
			
			VariantClear(&v_bufferdata);
			pRs->MoveNext();
		}
		pRs->Close();
		pRs.Release();	pRs = NULL;
		return dw_RecCount;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}
	
	return 0;
}

int		cADO_ADMIN::spModifyPvPTeamScore(char * _strName, cDuelTeamRecord * _record)//_strName 팀의 WCG 성적을 수정한다.
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spSET_WCG_DATA";
		pCmd->CommandType  = adCmdStoredProc;

		int reserve = 0;
		DEF_PARAM(pCmd,"@TEAMNAME") = _variant_t((char *)_strName);
		DEF_PARAM(pCmd,"@POINT") = _variant_t((long)_record->m_iWinPoint);
		DEF_PARAM(pCmd,"@BATTLECOUNT") = _variant_t((long)_record->m_iBattleCount);
		DEF_PARAM(pCmd,"@WINCOUNT") = _variant_t((long)_record->m_iWinCount);
		DEF_PARAM(pCmd,"@DEFEATCOUNT") = _variant_t((long)_record->m_iDefeatCount);
		DEF_PARAM(pCmd,"@DRAWCOUNT") = _variant_t((long)_record->m_iDrawCount);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			_log(" Failed to update WCG info [%s]",_strName);
			return 0;
		}
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 1;
}

int		cADO_ADMIN::spModifyPvPScore(cDuelRecord * _record)//_strName 캐릭터의 WCG 성적을 수정한다.
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spSET_PvPData";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)_record->m_strID);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_record->m_strName);
		DEF_PARAM(pCmd,"@JOB") = _variant_t((long)_record->m_wJob);
		DEF_PARAM(pCmd,"@WCGPOINT") = _variant_t((long)_record->m_iTrialGameWinPoint);
		DEF_PARAM(pCmd,"@DuelPoint") = _variant_t((long)_record->m_iDuelPoint);
		DEF_PARAM(pCmd,"@BattleCount") = _variant_t((long)_record->m_iBattleCount);
		DEF_PARAM(pCmd,"@WinCount") = _variant_t((long)_record->m_iWinCount);
		DEF_PARAM(pCmd,"@DefeatCount") = _variant_t((long)_record->m_iDefeatCount);
		DEF_PARAM(pCmd,"@DrawCount") = _variant_t((long)_record->m_iDrawCount);
		DEF_PARAM(pCmd,"@WeeklyWinPoint") = _variant_t((long)_record->m_iWeeklyWinPoint);
		DEF_PARAM(pCmd,"@WeeklyBattleCount") = _variant_t((long)_record->m_iWeeklyBattleCount);
		DEF_PARAM(pCmd,"@WeeklyWinCount") = _variant_t((long)_record->m_iWeeklyWinCount);
		DEF_PARAM(pCmd,"@WeeklyDefeatCount") = _variant_t((long)_record->m_iWeeklyDefeatCount);
		DEF_PARAM(pCmd,"@WeeklyDrawCount") = _variant_t((long)_record->m_iWeeklyDrawCount);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			_log(" Failed to update pvp info [%s]",_record->m_strID);
			return 0;
		}
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 1;
}

int
cADO_ADMIN::spGetPvPScore(char * _strId, cDuelRecord * _record)
{
	int iCheck_line = __LINE__;

	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGET_PvPData";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)_strId);
		
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			_log(" Failed to Get pvp info [%s]",_strId);
			return 0;
		}

		VARIANT regIdx;	VariantInit(&regIdx);

		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@WCGPOINT")->Value;
		iCheck_line = __LINE__;
		_record->m_iTrialGameWinPoint = (int)regIdx.intVal;
		iCheck_line = __LINE__;

		if(_record->m_iTrialGameWinPoint >=0 )
		{
			regIdx = pCmd->GetParameters()->GetItem("@NAME")->Value;
			iCheck_line = __LINE__;
			strcpy(_record->m_strName,(_bstr_t)regIdx);
			iCheck_line = __LINE__;

			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@ID")->Value;
			iCheck_line = __LINE__;
			strcpy(_record->m_strID,(_bstr_t)regIdx);
			iCheck_line = __LINE__;

			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@JOB")->Value;
			iCheck_line = __LINE__;
			_record->m_wJob = (WORD)regIdx.intVal;
			iCheck_line = __LINE__;

			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@DuelPoint")->Value;
			iCheck_line = __LINE__;
			_record->m_iDuelPoint = (int)regIdx.intVal;
			iCheck_line = __LINE__;

			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@BattleCount")->Value;
			iCheck_line = __LINE__;
			_record->m_iBattleCount = (int)regIdx.intVal;
			iCheck_line = __LINE__;

			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@WinCount")->Value;
			iCheck_line = __LINE__;
			_record->m_iWinCount = (int)regIdx.intVal;
			iCheck_line = __LINE__;

			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@DefeatCount")->Value;
			iCheck_line = __LINE__;
			_record->m_iDefeatCount = (int)regIdx.intVal;
			iCheck_line = __LINE__;

			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@DrawCount")->Value;
			iCheck_line = __LINE__;
			_record->m_iDrawCount = (int)regIdx.intVal;
			iCheck_line = __LINE__;

			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@WeeklyWinPoint")->Value;
			iCheck_line = __LINE__;
			_record->m_iWeeklyWinPoint = (int)regIdx.intVal;
			iCheck_line = __LINE__;

			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@WeeklyBattleCount")->Value;
			iCheck_line = __LINE__;
			_record->m_iWeeklyBattleCount = (int)regIdx.intVal;
			iCheck_line = __LINE__;

			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@WeeklyWinCount")->Value;
			iCheck_line = __LINE__;
			_record->m_iWeeklyWinCount = (int)regIdx.intVal;
			iCheck_line = __LINE__;

			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@WeeklyDefeatCount")->Value;
			iCheck_line = __LINE__;
			_record->m_iWeeklyDefeatCount = (int)regIdx.intVal;
			iCheck_line = __LINE__;

			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@WeeklyDrawCount")->Value;
			iCheck_line = __LINE__;
			_record->m_iWeeklyDrawCount = (int)regIdx.intVal;
			iCheck_line = __LINE__;
		}
		VariantClear(&regIdx);
	}
	catch( _com_error &e){
		DMPcomERR(e)
		_log("지현봐라 : %d",iCheck_line);
		return 0;
	}
	return 1;
}