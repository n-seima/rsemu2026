// cADO_AVATAR.cpp: implementation of the cADO_USER class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "cADO_AVATAR.h"
#include "cSRVUTIL.h"
#include "RSComm.h"
#include "CSaveData.h"
#include "Client.h"
#include "userCustomDataDefine.h"

int cADO_AVATAR::GetProvisionRank(DGMSG_PROVISION_RANK_RET * _rank)
{
//	CCritical		cs(&m_csAC);
	char			str_Query[1024];
	DWORD			i;
	HRESULT			hr = CoInitialize(0);

	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"SELECT rank, strName FROM PROVISIONRANK");
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		_rank->wCount = (WORD)pRs->RecordCount;
		if(_rank->wCount>dMAX_PROVRANK_COUNT)		_rank->wCount = dMAX_PROVRANK_COUNT;

		if(FAILED(hr))	return FALSE;

		CINDEX_TABLE	idxTable;
		for(i = 0 ;i < _rank->wCount ;i++)
		{
			_variant_t		v_bufferdata;
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			_rank->list[i].wRank = (WORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			strcpy(_rank->list[i].strName,(_bstr_t)v_bufferdata);
			VariantClear(&v_bufferdata);
			pRs->MoveNext();
		}
		pRs->Close();
		pRs.Release();	pRs = NULL;
		return TRUE;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}
	return FALSE;
}

int cADO_AVATAR::spEvent_GoldRush_Get(cGoldRushDefine * _cGoldRush)
{
//	CCritical		cs(&m_csAC);
	_cGoldRush->reset();
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spEvent_GoldRush_Get";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return 0;
		DEF_PARAM(pCmd,"@RESERVE") = _variant_t((long)0);
 		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->GetValue();
		WORD	l_wResult = (WORD)regIdx.intVal;
		VariantClear(&regIdx);
		if(!l_wResult)	return 0;

		_variant_t	v_varient;
		{
			char		strDate[64];

			v_varient							= pCmd->GetParameters()->GetItem("@BEGINDATE")->GetValue();
			strcpy(strDate,(_bstr_t)v_varient);
			getTimeToUTime(strDate,&_cGoldRush->m_begin);
			v_varient							= pCmd->GetParameters()->GetItem("@ENDDATE")->GetValue();
			strcpy(strDate,(_bstr_t)v_varient);
			getTimeToUTime(strDate,&_cGoldRush->m_end);

			v_varient							= pCmd->GetParameters()->GetItem("@BOOSTEXP")->GetValue();
			_cGoldRush->m_wBoostExperience		= (WORD)v_varient.intVal;
			v_varient							= pCmd->GetParameters()->GetItem("@CIDC")->GetValue();
			_cGoldRush->m_wCorrectItemDropChance= (WORD)v_varient.intVal;

			v_varient							= pCmd->GetParameters()->GetItem("@BCOMM")->GetValue();
			strcpy(_cGoldRush->m_strBeginComment,(_bstr_t)v_varient);
			v_varient							= pCmd->GetParameters()->GetItem("@ECOMM")->GetValue();
			strcpy(_cGoldRush->m_strEndComment,(_bstr_t)v_varient);
		}
		VariantClear(&v_varient);
		iErrorCount = 0;
		pCmd.Release();
		return 1;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 0;
}

int cADO_AVATAR::spEvent_GoldRush_Get_First(cGoldRushDefine * _cGoldRush)
{
//	CCritical		cs(&m_csAC);
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
		if(pRs->adoEOF)	return 0;
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
		pRs->Close();
		return 1;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 0;
}

int cADO_AVATAR::spEvent_ItemPack_Get(cItemPackInfo * _cIPInfo)
{
//	CCritical		cs(&m_csAC);
	_cIPInfo->reset();
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spEvent_ItemPack_Get";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return 0;
		DEF_PARAM(pCmd,"@RESERVE") = _variant_t((long)0);
 		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->GetValue();
		WORD	l_wResult = (WORD)regIdx.intVal;
		VariantClear(&regIdx);

		if(!l_wResult)		return 0;

		_variant_t	v_varient;
		char	* uncompressdata = new char[sizeof(cItemPackInfo)];
		memset(uncompressdata,0,sizeof(cItemPackInfo));
		v_varient = pCmd->GetParameters()->GetItem("@DATA")->GetValue();
		int unsize = VariantToBlob(v_varient,uncompressdata);
		if(unsize != sizeof(cItemPackInfo))
		{
			WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)_cIPInfo,unsize,sizeof(cItemPackInfo));
			if(ww == 0)
			{
				_cIPInfo->reset();
			}
		}else
			memcpy(_cIPInfo,uncompressdata,sizeof(cItemPackInfo));
		delete [] uncompressdata;
		VariantClear(&v_varient);

		pCmd.Release();
		return 1;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 0;
}

int cADO_AVATAR::spEvent_ItemPack_Get_First(int _iSerial,cItemPackInfo * _cIPInfo)
{
//	CCritical		cs(&m_csAC);
	_cIPInfo->reset();
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spEvent_ItemPack_Get_For_Admin";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return 0;
		DEF_PARAM(pCmd,"@SERIAL") = _variant_t((long)_iSerial);
 		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->GetValue();
		WORD	l_wResult = (WORD)regIdx.intVal;
		VariantClear(&regIdx);

		if(!l_wResult)		return 0;

		_variant_t	v_varient;
		char	* uncompressdata = new char[sizeof(cItemPackInfo)];
		memset(uncompressdata,0,sizeof(cItemPackInfo));
		v_varient = pCmd->GetParameters()->GetItem("@DATA")->GetValue();
		int unsize = VariantToBlob(v_varient,uncompressdata);
		if(unsize != sizeof(cItemPackInfo))
		{
			WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)_cIPInfo,unsize,sizeof(cItemPackInfo));
			if(ww == 0)
			{
				_cIPInfo->reset();
			}
		}else
			memcpy(_cIPInfo,uncompressdata,sizeof(cItemPackInfo));
		delete [] uncompressdata;
		VariantClear(&v_varient);

		pCmd.Release();
		return 1;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 0;
}