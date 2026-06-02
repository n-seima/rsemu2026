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

int cADO_ADMIN::spEvent_GoldRush_Create(cGoldRushDefine * _cGoldRush,char * _strWho)
{
	CCritical		cs(&m_csAC);
	try{
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;

		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spEvent_GoldRush_Create";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return 0;

		char strDate[64];
		sprintf(strDate,"%d-%d-%d %d:%d:%d",_cGoldRush->m_begin.year+2000,_cGoldRush->m_begin.month,_cGoldRush->m_begin.day,_cGoldRush->m_begin.hour,_cGoldRush->m_begin.minute,_cGoldRush->m_begin.second);
		DEF_PARAM(pCmd,"@BEGINDATE") = _variant_t((char *)strDate);
		sprintf(strDate,"%d-%d-%d %d:%d:%d",_cGoldRush->m_end.year+2000,_cGoldRush->m_end.month,_cGoldRush->m_end.day,_cGoldRush->m_end.hour,_cGoldRush->m_end.minute,_cGoldRush->m_end.second);
		DEF_PARAM(pCmd,"@ENDDATE") = _variant_t((char *)strDate);
		DEF_PARAM(pCmd,"@BOOSTEXP") = _variant_t((long)_cGoldRush->m_wBoostExperience);
		DEF_PARAM(pCmd,"@CIDC") = _variant_t((long)_cGoldRush->m_wCorrectItemDropChance);
		DEF_PARAM(pCmd,"@BEGINCOMMENT") = _variant_t((char *)_cGoldRush->m_strBeginComment);
		DEF_PARAM(pCmd,"@ENDCOMMENT") = _variant_t((char *)_cGoldRush->m_strEndComment);
		DEF_PARAM(pCmd,"@WHO") = _variant_t((char *)_strWho);
 		pRs = pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int	l_iResult = (int)regIdx.intVal;
		VariantClear(&regIdx);
		
		pCmd.Release();
		if	(pRs && pRs->State == adStateOpen)
			pRs->Close();
		return 1;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 0;
}

int cADO_ADMIN::spEvent_GoldRush_Get(cGoldRushDefine * _cGoldRush)
{
	CCritical		cs(&m_csAC);
	_cGoldRush->reset();
	try{
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;

		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spEvent_GoldRush_Get_For_Admin";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return 0;

 		pRs = pCmd->Execute(NULL,NULL,NULL);
		_variant_t	v_varient;
		{
			char		strDate[64];

			v_varient							= pRs->Fields->Item[_variant_t("begindate")]->Value;
			strcpy(strDate,(_bstr_t)v_varient);
			getTimeToUTime(strDate,&_cGoldRush->m_begin);

			v_varient							= pRs->Fields->Item[_variant_t("enddate")]->Value;
			strcpy(strDate,(_bstr_t)v_varient);
			getTimeToUTime(strDate,&_cGoldRush->m_end);

			v_varient							= pRs->Fields->Item[_variant_t("boostexp")]->Value;
			_cGoldRush->m_wBoostExperience		= (WORD)v_varient.intVal;
			v_varient							= pRs->Fields->Item[_variant_t("cidc")]->Value;
			_cGoldRush->m_wCorrectItemDropChance= (WORD)v_varient.intVal;

			v_varient							= pRs->Fields->Item[_variant_t("begincomment")]->Value;
			strcpy(_cGoldRush->m_strBeginComment,(_bstr_t)v_varient);
			v_varient							= pRs->Fields->Item[_variant_t("endcomment")]->Value;
			strcpy(_cGoldRush->m_strEndComment,(_bstr_t)v_varient);
		}
		VariantClear(&v_varient);
		iErrorCount = 0;
		pCmd.Release();
		if	(pRs && pRs->State == adStateOpen)
			pRs->Close();
		return 1;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 0;
}


int cADO_ADMIN::spEvent_ItemPack_Create(int _iSerial,cItemPackInfo * _cIPInfo)
{
	CCritical		cs(&m_csAC);
	try
	{
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;

		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spEvent_ItemPack_Create";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return 0;

		DEF_PARAM(pCmd,"@SERIAL") = _variant_t((long)_iSerial);
		DEF_PARAM(pCmd,"@CNT") = _variant_t((long)_cIPInfo->m_wCount);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_cIPInfo->m_strName);
		
		int iSize = sizeof(cItemPackInfo);// - sizeof(cItemInPack)*(abs(_cIPInfo->m_wCount - dMAX_ITEM_IN_ITEM_PACK));
		char	* compressData = new char[iSize];
		VARIANT bin;
		VariantInit(&bin);
		memset(compressData,0,iSize);
		WORD sz = cSRVUTIL::Compress((unsigned char *)_cIPInfo,(unsigned char *)compressData,iSize);
		BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);

		pRs = pCmd->Execute(NULL,NULL,NULL);

		VariantClear(&bin);
		delete [] compressData;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int	l_iResult = (int)regIdx.intVal;
		VariantClear(&regIdx);

		pCmd.Release();
		if	(pRs && pRs->State == adStateOpen)
			pRs->Close();

		return l_iResult;
	}
	catch( _com_error &e){
		DMPcomERR(e)
	}
	return 0;
}

int cADO_ADMIN::spEvent_ItemPack_Get(int _iSerial,cItemPackInfo * _cIPInfo)
{
	CCritical		cs(&m_csAC);
	_cIPInfo->reset();
	_cIPInfo->m_wSerial = _iSerial;
	try
	{
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;

		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spEvent_ItemPack_Get_For_Admin";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return 0;

		DEF_PARAM(pCmd,"@SERIAL") = _variant_t((long)_iSerial);

 		pRs = pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD	l_wResult = (WORD)regIdx.intVal;
		VariantClear(&regIdx);

		if(!l_wResult)		return 0;

		_variant_t	v_varient;
		char	* uncompressdata = new char[sizeof(cItemPackInfo)];
		memset(uncompressdata,0,sizeof(cItemPackInfo));
		v_varient = pCmd->GetParameters()->GetItem("@DATA")->Value;
		int unsize = VariantToBlob(v_varient,uncompressdata);
		WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)_cIPInfo,unsize,sizeof(cItemPackInfo));
		if(ww == 0)
		{
			_cIPInfo->reset();
		}
		delete [] uncompressdata;
		VariantClear(&v_varient);

		pCmd.Release();
//		pRs->Close();
		return 1;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 0;
}

int
cADO_ADMIN::GetCarrotShopInfo(cCarrotShopDefine * _cpCSInfo,int _isFirst)
{
	CCritical		cs(&m_csAC);
	_cpCSInfo->m_wCategoryCount = 0;
	try
	{
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;

		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_CARROTSHOP_GET";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return 0;
		DEF_PARAM(pCmd,"@RESERVE") = _variant_t((long)_isFirst);
 		pRs = pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD	l_wResult = (WORD)regIdx.intVal;
		VariantClear(&regIdx);

		if(!l_wResult)
			return 0;

		_variant_t	v_varient;
		char	* uncompressdata = new char[sizeof(cCarrotShopDefine)];
		memset(uncompressdata,0,sizeof(cCarrotShopDefine));
		v_varient = pCmd->GetParameters()->GetItem("@DATA")->Value;
		int unsize = VariantToBlob(v_varient,uncompressdata);
		WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)_cpCSInfo,unsize,sizeof(cCarrotShopDefine));
		if(ww == 0)
			_cpCSInfo->m_wCategoryCount = 0;
		delete [] uncompressdata;uncompressdata = NULL;
		VariantClear(&v_varient);


		pCmd.Release();
		if	(pRs && pRs->State == adStateOpen)
			pRs->Close();
		return 1;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 0;
}

int
cADO_ADMIN::SetCarrotShopInfo(cCarrotShopDefine * _cpCSInfo)
{
	CCritical		cs(&m_csAC);
	HRESULT			hr = CoInitialize(0);
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_CARROTSHOP_SET";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return 0;

		int iSize = sizeof(cCarrotShopDefine);// - sizeof(cItemInPack)*(abs(_cIPInfo->m_wCount - dMAX_ITEM_IN_ITEM_PACK));
		char	* compressData = new char[iSize];
		VARIANT bin;
		VariantInit(&bin);
		memset(compressData,0,iSize);
		WORD sz = cSRVUTIL::Compress((unsigned char *)_cpCSInfo,(unsigned char *)compressData,iSize);
		BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))	return 0;

		VariantClear(&bin);
		delete [] compressData; compressData = NULL;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int	l_iResult = (int)regIdx.intVal;
		VariantClear(&regIdx);

		pCmd.Release();

		return l_iResult;
	}
	catch( _com_error &e){
		DMPcomERR(e)
	}
	return 0;
}
