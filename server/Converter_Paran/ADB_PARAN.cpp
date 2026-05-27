#include "stdafx.h"
#include "ADB_PARAN.h"
#include "cSRVUTIL.h"

cADO_PARAN::cADO_PARAN()
{
}

cADO_PARAN::~cADO_PARAN()
{
}

//	Account에 적용할 것
cParanInfo
cADO_PARAN::getNewConvertingData()		//	변경할 아이디가 있는지 검사해서 정보를 가져온다.
{
	cParanInfo retInfo;

	CCritical		cs(&m_csAC);
	char			str_Query[1024];
	HRESULT			hr = CoInitialize(0);

	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"SELECT TOP 1 paran_id,new_id,isOverwrite,isLastData,SrvIdx,AvaIdx0,AvaIdx1,AvaIdx2,AvaIdx3 FROM Paran_Table Where isFinish = 0");
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

		if(pRs->GetState() == adStateClosed) return retInfo;
		if(FAILED(hr))			return retInfo;
		if(!pRs->RecordCount)	return retInfo;

		_variant_t		v_bufferdata;
		v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
		strcpy(retInfo.strParanId,(_bstr_t)v_bufferdata);
		v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
		strcpy(retInfo.strNewId,(_bstr_t)v_bufferdata);
		v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
		retInfo.isOverwrite = v_bufferdata.intVal;
		v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
		retInfo.isLastData = v_bufferdata.intVal;
		v_bufferdata = pRs->Fields->GetItem((LONG)4)->Value;
		retInfo.iServerIdx = v_bufferdata.intVal;
		v_bufferdata = pRs->Fields->GetItem((LONG)5)->Value;
		retInfo.iIdx[0] = v_bufferdata.intVal;
		v_bufferdata = pRs->Fields->GetItem((LONG)6)->Value;
		retInfo.iIdx[1] = v_bufferdata.intVal;
		v_bufferdata = pRs->Fields->GetItem((LONG)7)->Value;
		retInfo.iIdx[2] = v_bufferdata.intVal;
		v_bufferdata = pRs->Fields->GetItem((LONG)8)->Value;
		retInfo.iIdx[3] = v_bufferdata.intVal;

		VariantClear(&v_bufferdata);

		pRs->Close();
		pRs.Release();	pRs = NULL;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}

	return retInfo;
}

int
cADO_PARAN::finishConvertingWork(char * _strId, int _srvIdx)
{
	CCritical		cs(&m_csAC);
	char			str_Query[1024];
	HRESULT			hr = CoInitialize(0);

	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"UPDATE Paran_Table SET isFinish = 1 WHERE new_id = '%s' and SrvIdx=%d",_strId,_srvIdx);
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

		if(pRs->GetState() == adStateClosed) return TRUE;
		if(FAILED(hr))			return FALSE;
		
		VARIANT v_return;
		v_return = pRs->Fields->Item[_variant_t("RETURN VALUE")]->Value;
		int	l_iResult = (int)v_return.intVal;
		VariantClear(&v_return);

		pRs->Close();
		pRs.Release();	pRs = NULL;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}

	return TRUE;
}

int
cADO_PARAN::unsetBlock(char * _strId)		//	변경이 완료된 아이디의 블럭정보를 해제한다.
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_USER_SETBAD";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@GAMEID") = _variant_t((char *)_strId);
		DEF_PARAM(pCmd,"@BADDATE") = _variant_t((long)0);
		DEF_PARAM(pCmd,"@BAD_WHY") = _variant_t((char *)_strId);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
			return dSP_FAIL;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.iVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int	l_iResult = (int)regIdx.intVal;
		VariantClear(&regIdx);
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return FALSE;
	}
	return TRUE;
}

int
cADO_PARAN::convertPvPInfo(char * _strYahooId,char * _strNewId)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_CONVERT_PVP_YAHOO";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@YAHOOID") = _variant_t((char *)_strYahooId);
		DEF_PARAM(pCmd,"@NEWID") = _variant_t((char *)_strNewId);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
			return dSP_FAIL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return FALSE;
	}
	return TRUE;
}
