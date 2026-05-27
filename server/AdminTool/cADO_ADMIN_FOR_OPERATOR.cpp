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


WORD cADO_ADMIN::spResetOperator()
{
	_RecordsetPtr  pRs = NULL;
	char	       str_Query[1024];
	WORD i_RecCount;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"UPDATE Avatar_Current SET isOper = 1 WHERE isOper=2 or isOper=3");
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	pRs->Close();
	return i_RecCount;
}

int	cADO_ADMIN::spGetOperLevel(char * _name)
{
	_RecordsetPtr  pRs = NULL;
	char	       str_Query[1024];
	int i_RecCount,i,ret;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT isOper FROM Avatar_Current WHERE name='%s'",_name);
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
			ret = v_bufferdata.intVal;
			VariantClear(&v_bufferdata);
			break;
		}
	}
	pRs->Close();
	return ret;
}

WORD	cADO_ADMIN::spIsOper(char *name)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_ISOPER";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)name);
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr)){		printf("Save Fail- Execute !! \n");		return 0xffff;}

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
		return 0xffff;
	}
}

int		cADO_ADMIN::spGetOperCount()
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_GETOPERCOUNT";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@OPER") = _variant_t((long)1);
		hr = pCmd->Execute(NULL,NULL,NULL);

		if(FAILED(hr)){	return 0xffff;}
		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;

	}catch( _com_error &e)
	{
		DMPcomERR(e)
		return 0;
	}
}

void	cADO_ADMIN::spSetOper(char *name,int level)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_SETOPER";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)name);
		DEF_PARAM(pCmd,"@LEVEL") = _variant_t((long)level);
		hr = pCmd->Execute(NULL,NULL,NULL);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}catch( _com_error &e){
		DMPcomERR(e)
	}
}

int		cADO_ADMIN::spAdminLogin(char *name,char *pw,int ver)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spADMIN_LOGIN";
		pCmd->CommandType  = adCmdStoredProc;

//		char tmp[256];
//		sprintf(tmp,"Version %d",ver);
//		MessageBox(NULL,tmp,"Check!!",MB_OK);

		DEF_PARAM(pCmd,"@VER") = _variant_t((long)ver);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)name);
		DEF_PARAM(pCmd,"@PW") = _variant_t((char *)pw);
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr)){	return 0;}
		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd = NULL;
		return iResult;
	}catch( _com_error &e){
		MessageBox(NULL,"data error","ERROR - Touch your administrator",MB_OK);
		DMPcomERR(e)
		return 0;
	}
}

WORD	cADO_ADMIN::spGetOperatorCount()
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_GetOperatorCount";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@RESERVE") = _variant_t((long)1);
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))	return dSP_FAIL;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return iResult;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}

}

WORD	cADO_ADMIN::spSetOperatorInAccount(char * _strId,int _isOper)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_SetOperatorInAccount";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)_strId);
		DEF_PARAM(pCmd,"@ISOPER") = _variant_t((long)_isOper);
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))	return dSP_FAIL;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return iResult;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
}
