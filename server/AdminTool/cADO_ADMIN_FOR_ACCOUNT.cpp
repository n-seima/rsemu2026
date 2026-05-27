#include "StdAfx.h"
#include "cPLAYER_DATA.h"
#include "cADO_ADMIN.h"
#include <stdio.h>
#include <process.h>
#include "cSRVUTIL.h"
#include "RSComm.h"
#include "cDECODER.h"
#include "cITEM.h"
#include "CGuildDefine.h"


WORD	cADO_ADMIN::spSetBlockByRecover(char *id,char *why)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_USER_SETBLOCK";
		pCmd->CommandType  = adCmdStoredProc;
		DWORD	dwblockDate = 29991231;

		DEF_PARAM(pCmd,"@GAMEID") = _variant_t((char *)id);
		DEF_PARAM(pCmd,"@BADDATE") = _variant_t((long)dwblockDate);
		DEF_PARAM(pCmd,"@BAD_WHY") = _variant_t((char *)why);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))		return 2;

		VARIANT regIdx;
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->GetValue();
		int iResult = (int)regIdx.intVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;

	}catch(...)
	{	
		strcpy(why,"");	
	}
	return 2;
}

WORD	cADO_ADMIN::spGetIDInfo(char * id, stIDInfo	* _info)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_USER_GETINFO";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@GAMEID") = _variant_t((char *)id);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr)){		return 2;	}

		WORD wResult = 0;
		VARIANT regIdx;
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		wResult = regIdx.intVal;
		regIdx = pCmd->GetParameters()->GetItem("@STARTDATE")->Value;
		_info->dwStartConnect = regIdx.intVal;
		regIdx = pCmd->GetParameters()->GetItem("@LASTDATE")->Value;
		_info->dwLastConnect = regIdx.intVal;
		regIdx = pCmd->GetParameters()->GetItem("@SERVERNAME")->Value;
		strcpy(_info->strConnectedServerName,(_bstr_t)regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@BLOCK")->Value;
		_info->dwBlock = regIdx.intVal;
		regIdx = pCmd->GetParameters()->GetItem("@WHYBLOCK")->Value;
		strcpy(_info->strWhyBlock,(_bstr_t)regIdx);
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return wResult;

	}catch(...)
	{	
		
	}
	return 0;		//	접속 실패 
}

void	cADO_ADMIN::spGetBadInfo(char * id,char * why)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_USER_INFOBAD";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@GAMEID") = _variant_t((char *)id);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr)){		return ;	}

		VARIANT regIdx;	VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@WHY")->GetValue();
		if(strcpy((_bstr_t)regIdx,"")!=0)	strcpy(why,(_bstr_t)regIdx);
		else								strcpy(why,"");
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}catch( _com_error &e){
		DMPcomERR(e)
		return ;
	}
}

WORD	cADO_ADMIN::spSetBadUser(char *name,int dd,char * why)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_USER_SETBAD";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@GAMEID") = _variant_t((char *)name);
		DEF_PARAM(pCmd,"@BADDATE") = _variant_t((long)dd);
		DEF_PARAM(pCmd,"@BAD_WHY") = _variant_t((char *)why);


		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr)){		return 0xffff;	}

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD iResult = (WORD)regIdx.lVal;
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0xffff;
	}
}

int		cADO_ADMIN::spDeleteID(char *name)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_USER_DELETE";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@I_GAMEID") = _variant_t((char *)name);
		hr = pCmd->Execute(NULL,NULL,NULL);

		if(FAILED(hr)){	return 0;}
		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;
	}catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
}

int		cADO_ADMIN::spRecoverID(char *id,char *newid)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_USER_RECOVER";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@I_GAMEID") = _variant_t((char *)id);
		DEF_PARAM(pCmd,"@I_GAMEID_NEW") = _variant_t((char *)newid);
		hr = pCmd->Execute(NULL,NULL,NULL);

		if(FAILED(hr)){	return 0;}
		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;
	}catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
}

void
cADO_ADMIN::spDeleteLoginInfo(char * _worldName)		//	해당 월드의 접속 정보를 모두 삭제한다.
{
	_RecordsetPtr  pRs = NULL; 	
	char	       str_Query[1024];
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"UPDATE USER_TABLE SET isConnect = 0,W_Server=' ' WHERE W_Server LIKE '%%%s%%'",_worldName);
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

	if(!FAILED(hr))
	{
		MessageBox(NULL,"작업이 완료되었습니다.","result",MB_OK);
	}else{
		MessageBox(NULL,"뭔가 문제가 있을지도..","result",MB_OK);
	}

	return ;
}

DWORD	cADO_ADMIN::spGetLastDate(char * id)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGETLASTDATE";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)id);
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))	return dSP_FAIL;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RET")->Value;
		DWORD iResult = (DWORD)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		
		return iResult;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dSP_FAIL;
	}
}
