// cADO_USER.cpp: implementation of the cADO_USER class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "cADO_USER.h"
#include "cSRVUTIL.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cADO_USER::cADO_USER()
{

}

cADO_USER::~cADO_USER()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////
//		PRIVATE METHOD
/////////////////////////////////////////////////////////////////////////////////////////////////

int		cADO_USER::spCreateId(sCREATEINFO * cinfo)
{
/*	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_USER_CREATE";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)cinfo->name);
		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)cinfo->id);
		DEF_PARAM(pCmd,"@PW") = _variant_t((char *)cinfo->pw);
		DEF_PARAM(pCmd,"@RATETYPE") = _variant_t((long)cinfo->ratetype);
		DEF_PARAM(pCmd,"@BIRTH") = _variant_t((long)cinfo->birthday);
		DEF_PARAM(pCmd,"@TEL") = _variant_t((char *)cinfo->tel);
		DEF_PARAM(pCmd,"@PHONE") = _variant_t((char *)cinfo->phone);
		DEF_PARAM(pCmd,"@ADDR1") = _variant_t((char *)cinfo->addr1);
		DEF_PARAM(pCmd,"@ADDR2") = _variant_t((char *)cinfo->addr2);
		DEF_PARAM(pCmd,"@EMAIL") = _variant_t((char *)cinfo->email);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
			return dSP_FAIL;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.iVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;

		if((int)regIdx.iVal == -1)
			return dSP_CREATEID_USEDID;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dSP_FAIL;
	}
	return dSP_SUCCESS;
*/
	//	여기서 사용 안한다.
	return dSP_FAIL;
}

int		cADO_USER::spDeleteId(char *id,char *pw)
{
/*	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_USER_DELETE";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)id);
		DEF_PARAM(pCmd,"@PW") = _variant_t((char *)pw);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
			return dSP_FAIL;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.iVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;

		if((int)regIdx.iVal == -1)
			return dSP_DELETEID_NOTEXISTS;
		else  if((int)regIdx.iVal == -2)
			return dSP_DELETEID_USED;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dSP_FAIL;
	}
	return dSP_SUCCESS;
*/
	//	여기서 사용 안한다.
	return dSP_FAIL;
}

/*
int		cADO_USER::spChangePass(char *id, char *oldpw, char *pw)
{
}
*/

std::string	cADO_USER::spLogin(char *id, char *pw,char * maccaddress,char * clientCode, char *sv,char *ip, char *why,WORD & isOper,WORD & isNetCafe_User, BOOL& isRegistOTP,int isDis)
{
	std::string ret;

	ret = "-5";
#ifdef _FOR_USA
	if(strlen(id) >= 20 || id[0] > 126)
	{
		_log(" ERROR : id is incorrect!! [%s]",id);
		return ret;
	}
#endif
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_USER_LOGIN";
#ifdef _OGP_SERVICE
		pCmd->CommandText = L"spRS_USER_LOGIN_OGP";
#else
		pCmd->CommandText = L"spRS_USER_LOGIN";
#endif
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)id);
#ifndef _OGP_SERVICE
		DEF_PARAM(pCmd,"@PW") = _variant_t((char *)pw);
#endif
		DEF_PARAM(pCmd,"@MACADDRESS") = _variant_t((char *)maccaddress);

		DEF_PARAM(pCmd,"@IP") = _variant_t((char *)ip);
		DEF_PARAM(pCmd,"@SV") = _variant_t((char *)sv);
		DEF_PARAM(pCmd,"@DIS") = _variant_t((long)isDis);
		DEF_PARAM(pCmd,"@CLIENT_PROC") = _variant_t((char *)clientCode);

		hr = pCmd->Execute(NULL,NULL,NULL);

		if	(FAILED(hr))
		{
			pCmd.Release();
			pCmd.Detach();

			pCmd	=	NULL;

			return ret;
		}

		VARIANT regIdx;	

#ifdef _USE_ACCOUNT_LEVEL
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@OPER")->GetValue();
		isOper = (int)regIdx.intVal;
#else
		isOper = 0;
#endif

#ifdef _USE_ACCOUNT_LEVEL
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@NC_USER")->GetValue();
		isNetCafe_User = (int)regIdx.intVal;
#else
		isNetCafe_User = 0;
#endif
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->GetValue();
		ret = (_bstr_t)regIdx;

		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT_WHY")->GetValue();
		strcpy(why,(_bstr_t)regIdx);

		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@ISOTPUSER")->GetValue();

#ifndef	_FOR_JAPAN
		isRegistOTP	=	FALSE;
#else
		isRegistOTP = (int)regIdx.iVal;
#endif
		
		VariantClear(&regIdx);
		pCmd.Release();	
		pCmd.Detach();
		
		pCmd = NULL;

		return ret;
	}

	catch( _com_error &e)
	{
		_log("LOGIN ERROR : id [%s], pw [%s], sv [%s], why [%s] , isDis [%d]",id, pw,sv,why,isDis);
		DMPcomERR(e)

		return ret;
	}

	printf("[%s]'s connected server : [%s] \n",id,ret.c_str());
	return ret;
}

int	
cADO_USER::CheckOTP(char *id, char *otp, char *ip)
{
	int ret = 0;
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_OTP_CHECK";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)id);
		DEF_PARAM(pCmd,"@OTP") = _variant_t((char *)otp);
		DEF_PARAM(pCmd,"@IP") = _variant_t((char *)ip);
		hr = pCmd->Execute(NULL,NULL,NULL);

		if	(FAILED(hr))
		{
			pCmd.Release();
			pCmd.Detach();

			pCmd	=	NULL;

			return ret;
		}

		VARIANT regIdx;	
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RET")->GetValue();
		ret = (int)regIdx.intVal;

		switch(ret)
		{
		case 1:	ret = 0;	break;	//	dRESULT_LOGIN_OTP_SUCCESS
		case 0:	ret = 1;	break;	//	dRESULT_LOGIN_OTP_INCORRECT
		case -1:ret = 2;	break;	//	dRESULT_LOGIN_OTP_EXPIRE
		case -2:ret = 3;	break;	//	dRESULT_LOGIN_OTP_ERROR
		}

		VariantClear(&regIdx);
		pCmd.Release();	
		pCmd.Detach();
		
		pCmd = NULL;

		return ret;
	}

	catch( _com_error &e)
	{
		DMPcomERR(e)

		return ret;
	}
	return ret;
}

int
cADO_USER::spLogout(char *id,char *ip)
{
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_USER_LOGOUT";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)id);
		DEF_PARAM(pCmd,"@IP") = _variant_t((char *)ip);

		hr = pCmd->Execute(NULL,NULL,NULL);

		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		if(FAILED(hr))
			return dSP_FAIL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dSP_FAIL;
	}
	return dSP_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//		PUBLIC METHOD
/////////////////////////////////////////////////////////////////////////////////////////////////

int		cADO_USER::CreateID(sCREATEINFO * cinfo)
{
	return spCreateId(cinfo);
}

int		cADO_USER::DeleteID(char *id,char *pw)
{
	return spDeleteId(id,pw);
}

std::string	cADO_USER::Login(char *id, char *pw,char * maccaddress,char * clientCode, char *sv,char *ip,char *why,WORD & isOper, BOOL& isRegistOTP,int isDis)
{
	WORD temp = 0;
	return spLogin(id,pw,maccaddress,clientCode, sv,ip,why,isOper,temp, isRegistOTP, isDis);
}

std::string	cADO_USER::WLogin(char *id, char *pw,char * maccaddress,char * clientCode, char *sv,char *ip,char *why,WORD & isOper,WORD & isNetCafe_User, BOOL& isRegistOTP, int isDis)
{
	return spLogin(id,pw,maccaddress,clientCode, sv,ip,why,isOper,isNetCafe_User,isRegistOTP, 1);
}

int		cADO_USER::Logout(char *id,char *ip)
{
	return spLogout(id,ip);
}

int		cADO_USER::GetCarrotShopInfo(cCarrotShopDefine * _cpCSInfo,int _isFirst)
{
//	CCritical		cs(&m_csAC);
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_CARROTSHOP_GET";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)
			return 0;
		DEF_PARAM(pCmd,"@RESERVE") = _variant_t((long)_isFirst);
 		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD	l_wResult = (WORD)regIdx.intVal;
		VariantClear(&regIdx);

		if(!l_wResult)
		{
			pCmd.Release();		pCmd.Detach();		pCmd = NULL;
			return 0;
		}

		_variant_t	v_varient;
		char	* uncompressdata = new char[sizeof(cCarrotShopDefine)];
		memset(uncompressdata,0,sizeof(cCarrotShopDefine));
		v_varient = pCmd->GetParameters()->GetItem("@DATA")->Value;
		int unsize = VariantToBlob(v_varient,uncompressdata);
		WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)_cpCSInfo,unsize,sizeof(cCarrotShopDefine));
		if(ww == 0)
		{
			_log(" ERROR : in UNCOMPRESS , GetCarrotShopInfo");
			_cpCSInfo->m_wCategoryCount = 0;
		}
		delete [] uncompressdata; uncompressdata = 0;
		VariantClear(&v_varient);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return 1;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	_log(" ERRO 3");
	return 0;
}

int		cADO_USER::GetList_EventInGame(int * _list)
{
	DWORD			i;
	HRESULT			hr = CoInitialize(0);
	try
	{
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spEventInGame_GetList";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();	pRs.Release();	pRs = NULL;	return 0;	}

		DEF_PARAM(pCmd,"@Reserve") = _variant_t((long)0);
		pRs = pCmd->Execute(NULL,NULL, NULL);

		VARIANT regIdx;
		i = 0;
		while(!pRs->adoEOF)
		{
			regIdx = pRs->Fields->GetItem((LONG)0)->Value;
			int idx = (int)regIdx.intVal;
			regIdx = pRs->Fields->GetItem((LONG)1)->Value;
			_list[idx] = (int)regIdx.intVal;
			++i;
			VariantClear(&regIdx);
			pRs->MoveNext();
		}

		pRs->Close();
		pRs.Release();	pRs = NULL;

		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

	}catch(_com_error &e)
	{
		DMPcomERR(e)
		return 0;
	}
	return 1;
}

int		cADO_USER::InsertInfo_EventInGame(int _isreset, int _idx, char * _id, char * _name, int _max, int _isAllowDuplication, int &_lastValue)
{
	int l_wResult = 0;
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"speventInGame_Insert";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@RESET") = _variant_t((long)_isreset);
		DEF_PARAM(pCmd,"@IDX") = _variant_t((long)_idx);
		DEF_PARAM(pCmd,"@MAXCOUNT") = _variant_t((long)_max);
		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)_id);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_name);
		DEF_PARAM(pCmd,"@ISALLOW_DUPLICATION") = _variant_t((long)_isAllowDuplication);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			pCmd.Release();		pCmd.Detach();		pCmd = NULL;
			return dSP_FAIL;
		}

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RET")->Value;
		l_wResult = (int)regIdx.intVal;

		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@LAST")->Value;
		_lastValue = (int)regIdx.intVal;

		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return l_wResult;
}

int		cADO_USER::GetPvPDailyRanking(cDuelRankingInfo * _list)
{
	char			str_Query[1024];
	DWORD			dw_RecCount;
	DWORD			i;
	HRESULT			hr = CoInitialize(0);

	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"SELECT TOP 100 strID, strName, iJob, iPoint, iBattleCount, iWinCount, iDefeatCount, iDrawCount FROM tbPvP_Rank_Daily ORDER BY iPoint DESC, iWinCount DESC, iDefeatCount DESC, iDrawCount DESC, iBattleCount DESC");
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		dw_RecCount = pRs->RecordCount;

		if(FAILED(hr))
		{
			pRs->Close();	pRs.Release();	pRs = NULL;
			return 0;
		}

		for(i = 0 ;i < dw_RecCount ;i++)
		{
			memset(&_list[i], 0, sizeof(cDuelRankingInfo));
			_variant_t		v_bufferdata;
			_list[i].m_wRank = (short)i;
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			strcpy(_list[i].m_strID,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			strcpy(_list[i].m_strName,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			_list[i].m_wJob = (WORD)v_bufferdata.intVal;

			v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
			_list[i].m_iWinPoint = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)4)->Value;
			_list[i].m_iBattleCount = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)5)->Value;
			_list[i].m_iWinCount = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)6)->Value;
			_list[i].m_iDefeatCount = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)7)->Value;
			_list[i].m_iDrawCount = (DWORD)v_bufferdata.intVal;
			
			VariantClear(&v_bufferdata);
			pRs->MoveNext();
		}
//		_log(" Add First IndexManager Count %d, rec count %d",g_Im.dwCount,dw_RecCount);
		pRs->Close();	pRs.Release();	pRs = NULL;

		return dw_RecCount;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}
	
	return 0;
}

int		cADO_USER::GetPvPWeeklyRanking(cDuelRankingInfo * _list)
{
	char			str_Query[1024];
	DWORD			dw_RecCount;
	DWORD			i;
	HRESULT			hr = CoInitialize(0);

	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"SELECT TOP 100 strID, strName, iJob, iPoint, iBattleCount, iWinCount, iDefeatCount, iDrawCount FROM tbPvP_Rank_Weekly ORDER BY iPoint DESC, iWinCount DESC, iDefeatCount DESC, iDrawCount DESC, iBattleCount DESC");
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		dw_RecCount = pRs->RecordCount;

		if(FAILED(hr))	return 0;

		for(i = 0 ;i < dw_RecCount ;i++)
		{
			memset(&_list[i], 0, sizeof(cDuelRankingInfo));
			_variant_t		v_bufferdata;
			_list[i].m_wRank = (short)i;
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			strcpy(_list[i].m_strID,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			strcpy(_list[i].m_strName,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			_list[i].m_wJob = (WORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
			_list[i].m_iWinPoint = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)4)->Value;
			_list[i].m_iBattleCount = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)5)->Value;
			_list[i].m_iWinCount = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)6)->Value;
			_list[i].m_iDefeatCount = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)7)->Value;
			_list[i].m_iDrawCount = (DWORD)v_bufferdata.intVal;
			
			VariantClear(&v_bufferdata);
			pRs->MoveNext();
		}
		pRs->Close();	pRs.Release();	pRs = NULL;
		return dw_RecCount;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}
	
	return 0;
}

int		cADO_USER::GetPvPTrialGameRanking(cDuelRankingInfo * _list)
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
			memset(&_list[i], 0, sizeof(cDuelRankingInfo));
			_list[i].m_wRank = (short)i;
			_variant_t		v_bufferdata;
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			strcpy(_list[i].m_strName,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			_list[i].m_iWinPoint = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			_list[i].m_iBattleCount = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
			_list[i].m_iWinCount = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)4)->Value;
			_list[i].m_iDefeatCount = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)5)->Value;
			_list[i].m_iDrawCount = (DWORD)v_bufferdata.intVal;
			
			VariantClear(&v_bufferdata);
			pRs->MoveNext();
		}
		pRs->Close();	pRs.Release();	pRs = NULL;
		return dw_RecCount;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}
	
	return 0;
}

int		cADO_USER::GetPvPData(char * _strId, cDuelRecord * _record)
{
	_log(" GetPvPData : [%s] ",_strId);
	int iCheck_line = __LINE__;
	try{
		_CommandPtr		pCmd = NULL;
		iCheck_line = __LINE__;
		HRESULT			hr = CoInitialize(0);
		iCheck_line = __LINE__;

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		iCheck_line = __LINE__;
		pCmd->ActiveConnection = GetConnection();
		iCheck_line = __LINE__;
		pCmd->CommandText = L"spGET_PvPData";
		iCheck_line = __LINE__;
		pCmd->CommandType  = adCmdStoredProc;
		iCheck_line = __LINE__;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)_strId);
		iCheck_line = __LINE__;

		hr = pCmd->Execute(NULL,NULL,NULL);
		iCheck_line = __LINE__;
		if(FAILED(hr))
		{
			pCmd.Release();		pCmd.Detach();		pCmd = NULL;
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

//			VariantInit(&regIdx);
//			regIdx = pCmd->GetParameters()->GetItem("@ID")->Value;
//			iCheck_line = __LINE__;
//			strcpy(_record->m_strID,(_bstr_t)regIdx);
//			iCheck_line = __LINE__;

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
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		_log("Error : %d, please send this message to L&K",iCheck_line);
		return 0;
	}
	return 1;
}

void	cADO_USER::SetPvPData(char * _strId, cDuelRecord * _record)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spSET_PvPData";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)_strId);
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
			pCmd.Release();		pCmd.Detach();		pCmd = NULL;
			_log(" Failed to update pvp info [%s]",_strId);
			return;
		}
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return;
	}
	return;
}


int		cADO_USER::GetPvPTeamList(cDuelTeamInfo * _pList,std::map<int, int> & _list)		//	팀전체 리스트를 받는다.
{
	char			str_Query[1024];
	DWORD			dw_RecCount;
	DWORD			i;
	HRESULT			hr = CoInitialize(0);

	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"SELECT iIndex, strName, iPoint, iBattleCount, iWinCount, iDefeatCount, iDrawCount FROM tbPvP_WCG_TeamList");
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		dw_RecCount = pRs->RecordCount;

		if(FAILED(hr))	return 0;

		for(i = 0 ;i < dw_RecCount ;i++)
		{
			memset(&_pList[i], 0, sizeof(cDuelTeamInfo));

			_variant_t		v_bufferdata;
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			_pList[i].m_wIndex = (WORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			strcpy(_pList[i].m_strTeamName,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			_pList[i].m_record.m_iWinPoint = (int)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
			_pList[i].m_record.m_iBattleCount = (int)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)4)->Value;
			_pList[i].m_record.m_iWinCount = (int)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)5)->Value;
			_pList[i].m_record.m_iDefeatCount = (int)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)6)->Value;
			_pList[i].m_record.m_iDrawCount = (int)v_bufferdata.intVal;

			_pList[i].m_wMemberCount = 0;

			_list[_pList[i].m_wIndex] = i;
			
			VariantClear(&v_bufferdata);
			pRs->MoveNext();
		}
		pRs->Close();	pRs.Release();	pRs = NULL;
		return dw_RecCount;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}
	
	return 0;
}

int		cADO_USER::GetPvPTeamUserList(cDuelTeamInfo * _pList,std::map<int, int> & _list)	//	각 팀별 인원을 받는다.
{
	char			str_Query[1024];
	DWORD			dw_RecCount;
	DWORD			i;
	HRESULT			hr = CoInitialize(0);

	//	팀 정보의 규칙 
	//	중간 삭제는 허용되지 않는다.
	//	삽입은 마지막에만 가능하며, 실시간 갱신은 되지 않는다.

	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"SELECT iIndex, strID, strName, iJob FROM tbPvP_WCG_TeamUserList");
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		dw_RecCount = pRs->RecordCount;

		if(FAILED(hr))	return 0;

		for(i = 0 ;i < dw_RecCount ;i++)
		{
			_variant_t		v_bufferdata;

			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			WORD wIndex = _list[(WORD)v_bufferdata.intVal];
			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			strcpy(_pList[wIndex].m_aMembers[ _pList[wIndex].m_wMemberCount ].m_strID ,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			strcpy(_pList[wIndex].m_aMembers[ _pList[wIndex].m_wMemberCount ].m_strName ,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
			_pList[wIndex].m_aMembers[ _pList[wIndex].m_wMemberCount ].m_wJob = (int)v_bufferdata.intVal;

			_pList[wIndex].m_wMemberCount++;
			
			VariantClear(&v_bufferdata);
			pRs->MoveNext();
		}
		pRs->Close();	pRs.Release();	pRs = NULL;
		return dw_RecCount;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}
	
	return 0;
}

void	cADO_USER::SetPvPTeamInfoInit()	//	전체의 초기화
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spSET_WCG_INIT";
		pCmd->CommandType  = adCmdStoredProc;

		int reserve = 0;
		DEF_PARAM(pCmd,"@RESERVE") = _variant_t((long)reserve);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			pCmd.Release();		pCmd.Detach();		pCmd = NULL;
			_log(" Failed to WCG Info Init");
			return;
		}
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return;
	}
	return;
}

void	cADO_USER::SetPvPTeamInfo(char * _strTeamName, cDuelTeamRecord * _record)
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
		DEF_PARAM(pCmd,"@TEAMNAME") = _variant_t((char *)_strTeamName);
		DEF_PARAM(pCmd,"@POINT") = _variant_t((long)_record->m_iWinPoint);
		DEF_PARAM(pCmd,"@BATTLECOUNT") = _variant_t((long)_record->m_iBattleCount);
		DEF_PARAM(pCmd,"@WINCOUNT") = _variant_t((long)_record->m_iWinCount);
		DEF_PARAM(pCmd,"@DEFEATCOUNT") = _variant_t((long)_record->m_iDefeatCount);
		DEF_PARAM(pCmd,"@DRAWCOUNT") = _variant_t((long)_record->m_iDrawCount);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			pCmd.Release();		pCmd.Detach();		pCmd = NULL;
			_log(" Failed to update WCG info [%s]",_strTeamName);
			return;
		}
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return;
	}
	return;
}

//	주간 랭킹/WCG 랭킹을 산정 하고 모든 계정의 주간 전적을 초기화 시킨다.
void	cADO_USER::initWeeklyPvPInfo()
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spINIT_WEEKLY_INFO";
		pCmd->CommandType  = adCmdStoredProc;

		int reserve = 0;
		DEF_PARAM(pCmd,"@RESERVE") = _variant_t((long)reserve);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			pCmd.Release();		pCmd.Detach();		pCmd = NULL;
			_log(" Failed to spINIT_WEEKLY_INFO");
			return;
		}
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return;
	}
	return;
}

//	일간 랭킹 정보 초기화
void	cADO_USER::initDailyPvPRankingInfo()
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spCLEAR_DAILY_RANK";
		pCmd->CommandType  = adCmdStoredProc;
		
		int reserve = 0;
		DEF_PARAM(pCmd,"@RESERVE") = _variant_t((long)reserve);
		
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			pCmd.Release();		pCmd.Detach();		pCmd = NULL;
			_log(" Failed to spINIT_WEEKLY_INFO");
			return;
		}
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
			return;
	}
	return;
}

//	일간/WCG 랭킹 정보 산정
void	cADO_USER::updateDailyPvPRankingInfo()
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spINIT_DAILY_RANK";
		pCmd->CommandType  = adCmdStoredProc;
		
		int reserve = 0;
		DEF_PARAM(pCmd,"@RESERVE") = _variant_t((long)reserve);
		
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			pCmd.Release();		pCmd.Detach();		pCmd = NULL;
			_log(" Failed to spINIT_WEEKLY_INFO");
			return;
		}
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
			return;
	}
	return;
}

int		cADO_USER::spGetPoll(char *id)	//	마지막 참여한 pool인덱스를 어어온다.
{
	int l_wResult = 0;
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_POLL_GET";
		pCmd->CommandType  = adCmdStoredProc;
		
		int reserve = 0;
		DEF_PARAM(pCmd,"@I_GAMEID") = _variant_t((char *)id);
		
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			pCmd.Release();		pCmd.Detach();		pCmd = NULL;
			_log(" Failed to spRS_POLL_GET");
			return 0;
		}

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@POLL")->Value;
		l_wResult = (int)regIdx.intVal;
		VariantClear(&regIdx);

		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
			return 0;
	}
	return l_wResult;
}

int		cADO_USER::spSetPoll(char *id, int poll)	//	마지막 참여한 pool인덱스를 설정한다.
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_POLL_SET";
		pCmd->CommandType  = adCmdStoredProc;
		
		int reserve = 0;
		DEF_PARAM(pCmd,"@I_GAMEID") = _variant_t((char *)id);
		DEF_PARAM(pCmd,"@POLL") = _variant_t((long)poll);
		
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			pCmd.Release();		pCmd.Detach();		pCmd = NULL;
			_log(" Failed to spRS_POLL_GET");
			return 0;
		}

		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
			return 0;
	}
	return 1;
}
