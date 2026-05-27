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

int		cADO_ADMIN::spCTGetList()
{
	return 1;
}

int		cADO_ADMIN::spCTReserve(cAdCommandData * _data)
{
	try{
		_CommandPtr		pCmd = NULL;
		VARIANT bin;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spCT_RESERVE";
		pCmd->CommandType  = adCmdStoredProc;

		char	datetime[20];

		UTimeToDatetime(&_data->releasetime,datetime);
		DEF_PARAM(pCmd,"@RELEASETIME") = _variant_t((char *)datetime);
		DEF_PARAM(pCmd,"@COMMANDTYPE") = _variant_t((long)_data->commandtype);
		DEF_PARAM(pCmd,"@COMMANDTARGET") = _variant_t((char *)_data->commandtarget);
		UTimeToDatetime(&_data->finishtime,datetime);
		DEF_PARAM(pCmd,"@FINISHTIME") = _variant_t((char *)datetime);
		DEF_PARAM(pCmd,"@PARAM1") = _variant_t((long)_data->param1);
		DEF_PARAM(pCmd,"@PARAM2") = _variant_t((long)_data->param2);
		DEF_PARAM(pCmd,"@PARAM3") = _variant_t((char *)_data->param3);
		DEF_PARAM(pCmd,"@MSG") = _variant_t((char *)_data->msg);
		hr = pCmd->Execute(NULL,NULL,NULL);
		VariantClear(&bin);
		if(FAILED(hr))	return dSP_FAIL;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		DWORD iResult = (DWORD)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		
#ifdef _FOR_JAPAN
		_log("Command : param3[%s], msg[%s], who[%s], whod[%s]",_data->param3, _data->msg, _data->who, _data->whod);
#endif
		return iResult;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dSP_FAIL;
	}
	return 1;
}

int		cADO_ADMIN::spCTModify(cAdCommandData * _data)
{
	return 1;
}

int		cADO_ADMIN::spCTCancel(int _idx)
{
	return 1;
}
