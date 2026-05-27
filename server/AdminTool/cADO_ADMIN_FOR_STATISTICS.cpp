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


void	cADO_ADMIN::spSUM_ForLevel()//마지막 접속 일자 통계
{
	_RecordsetPtr  pRs = NULL;
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT data FROM Avatar_Current");
	printf(str_Query);
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(!FAILED(hr))
	{ 
		for(i = 0 ;i < i_RecCount ;i++)
		{
			cPLAYER_DATA	pData;
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			VariantToBlob(v_bufferdata,&pData);
			pRs->MoveNext();
		}
	}
	pRs->Close();
}

//	통계 관련 메소드 
int		cADO_ADMIN::spGetTotalUserCountByDay(int day,int & y_result,int & p_result,
											 int &week, int &week_y,int &week_p,
											 int &month,int &month_y,int &month_p)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spRS_GetUserCountByDay";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@TODAY") = _variant_t((long)day);
		hr = pCmd->Execute(NULL,NULL,NULL);

		if(FAILED(hr)){	return 0;}
		VARIANT regIdx;	//VariantInit(&regIdx);	regIdx.vt = VT_UI8;
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int iResult = (int)regIdx.lVal;
		regIdx = pCmd->GetParameters()->GetItem("@RESWEEK")->Value;
		week = (int)regIdx.lVal;
		regIdx = pCmd->GetParameters()->GetItem("@RESMON")->Value;
		month = (int)regIdx.lVal;
		VariantClear(&regIdx);

#ifdef _FOR_KOREA
		if(day>20040908)
		{
			//VariantInit(&regIdx);	regIdx.vt = VT_UI8;
			regIdx = pCmd->GetParameters()->GetItem("@RESULT_Y")->Value;
			y_result = (int)regIdx.lVal;
			regIdx = pCmd->GetParameters()->GetItem("@RESULT_P")->Value;
			p_result = (int)regIdx.lVal;
			regIdx = pCmd->GetParameters()->GetItem("@RESWEEK_Y")->Value;
			week_y = (int)regIdx.lVal;
			regIdx = pCmd->GetParameters()->GetItem("@RESWEEK_P")->Value;
			week_p = (int)regIdx.lVal;
			regIdx = pCmd->GetParameters()->GetItem("@RESMON_Y")->Value;
			month_y = (int)regIdx.lVal;
			regIdx = pCmd->GetParameters()->GetItem("@RESMON_P")->Value;
			month_p = (int)regIdx.lVal;
		}else{
			y_result = p_result = 0;
		}
#endif

		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;
	}catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
}

void		cADO_ADMIN::spGetAgeConnectCount(char * tablename, int today,int * ttt)
{
	_RecordsetPtr  pRs = NULL;
	
	char	       str_Query[1024];
	int i_RecCount;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT * FROM %s WHERE today=%d and type=0",tablename,today);
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(!FAILED(hr) && i_RecCount>0)
	{ 
		for(int i=0;i<24;i++)
		{
			_variant_t     v_bufferdata;
			v_bufferdata = pRs->Fields->GetItem((LONG)(i+2))->GetValue();
			ttt[i] = (int)v_bufferdata.intVal / 3;
			VariantClear(&v_bufferdata);
		}
	}
	pRs->Close();
}

void	cADO_ADMIN::spGetAgePlayTimeCount(char * tablename, int today,int * ttt)
{
	_RecordsetPtr  pRs = NULL;
	
	char	       str_Query[1024];
	int i_RecCount;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT * FROM %s WHERE today=%d and type=1",tablename,today);
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(!FAILED(hr) && i_RecCount>0)
	{ 
		for(int i=0;i<24;i++)
		{
			_variant_t     v_bufferdata;
			v_bufferdata = pRs->Fields->GetItem((LONG)(i+2))->GetValue();
			ttt[i] = (int)v_bufferdata.intVal;
			VariantClear(&v_bufferdata);
		}
	}
	pRs->Close();
}

void	cADO_ADMIN::spGetDistriLevel(int today, char * txt)
{
	_RecordsetPtr  pRs = NULL;
	
	char	       str_Query[1024];
	int i_RecCount;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT data FROM distriLevel WHERE today=%d",today);
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(!FAILED(hr) && i_RecCount>0)
	{	
		_variant_t     v_bufferdata;
		v_bufferdata = pRs->Fields->GetItem((LONG)0)->GetValue();
		strcpy(txt,(_bstr_t)v_bufferdata);
		VariantClear(&v_bufferdata);
	}
	pRs->Close();
}

int		cADO_ADMIN::spGetDetailUserCountStat(int day,int srvIdx, vector<stUserStatsPack> * _pVector)
{
	_RecordsetPtr  pRs = NULL;

	char	       str_Query[1024];
	int i_RecCount;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT tiTime,usercount,usercountReserve1,usercountReserve2,usercountReserve3,usercountReserve4,usercountReserve5 FROM UserCountStat WHERE srvIdx = %d and tiDate=%d ORDER BY tiTime",srvIdx+1,day);
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;
	if(FAILED(hr) || i_RecCount<=0)	return 0;

	_pVector->clear();
	stUserStatsPack	tmpPack;
	memset(&tmpPack,0,sizeof(stUserStatsPack));
	for(int i=0; i< 1440;i++)
		_pVector->push_back(tmpPack);
	
	for(i = 0 ;i < i_RecCount ;i++)
	{
		_variant_t     v_bufferdata;
		v_bufferdata = pRs->Fields->GetItem((LONG)0)->GetValue();
		UTime	time;
		getTimeToUTime((_bstr_t)v_bufferdata,&time);

		stUserStatsPack	pack;
		v_bufferdata = pRs->Fields->GetItem((LONG)1)->GetValue();
		pack.iAllUserCount = (int)v_bufferdata.intVal;
		v_bufferdata = pRs->Fields->GetItem((LONG)2)->GetValue();
		pack.iReserve1UserCount = (int)v_bufferdata.intVal;
		v_bufferdata = pRs->Fields->GetItem((LONG)3)->GetValue();
		pack.iReserve2UserCount = (int)v_bufferdata.intVal;
		v_bufferdata = pRs->Fields->GetItem((LONG)4)->GetValue();
		pack.iReserve3UserCount = (int)v_bufferdata.intVal;
		v_bufferdata = pRs->Fields->GetItem((LONG)5)->GetValue();
		pack.iReserve4UserCount = (int)v_bufferdata.intVal;
		v_bufferdata = pRs->Fields->GetItem((LONG)6)->GetValue();
		pack.iReserve5UserCount = (int)v_bufferdata.intVal;


		int t = time.hour*60 + time.minute;
		if(t<1440)
			_pVector->at(t) = pack;
		VariantClear(&v_bufferdata);

		pRs->MoveNext();
	}
	pRs->Close();

	return 1;
}