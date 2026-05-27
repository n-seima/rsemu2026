// cADO_AVATAR.cpp: implementation of the cADO_USER class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "cADO_STATISTIC.h"
#include "RSComm.h"
#include "cSRVUTIL.h"

cADO_STAT::~cADO_STAT()
{
	
}

BOOL cADO_STAT::ConnectDB()
{
	return Connect(m_strIp,m_strId,m_strPw,m_strDBSource,4);
}

void
cADO_STAT::spSend_GOLD(const char *_strServer, const char *_strID, const char *_strName, const LONGLONG _allGold, const LONGLONG _gettingGold)
{
	if	(_gettingGold	==	0)
		return;
	
	CCritical		cs(&m_csAC);
	HRESULT			hr = CoInitialize(0);
	try
	{
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGold_SET_GAME";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();	pRs.Release();	pRs = NULL;	return ;	}

		DEF_PARAM(pCmd,"@SERVER") = _variant_t((char *)_strServer);
		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)_strID);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);
		DEF_PARAM(pCmd,"@CUR_ALL") = _variant_t((double)_allGold,VT_R8);
		DEF_PARAM(pCmd,"@GOLD") = _variant_t((double)_gettingGold,VT_R8);
// 		_log("	GoldLog4	%s	%s	%I64d	%I64d",_strID,_strName,_allGold, _gettingGold);
		pRs = pCmd->Execute(NULL,NULL, NULL);

		pCmd.Release();
		pRs.Release();
	}catch(_com_error &e)
	{
		DMPcomERR(e)
		return ;
	}
	return ;
}