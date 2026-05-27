#include "cPLAYER_DATA.h"
#include "cADO_AnalyzeTool.h"
#include <stdio.h>
#include <process.h>
#include "cSRVUTIL.h"
#include "RSComm.h"

//_variant_t  vtEmpty2 (DISP_E_PARAMNOTFOUND, VT_ERROR);

cADO_AnalyzeTool::cADO_AnalyzeTool()
{
	GlastLine = 0;
	InitializeCriticalSection(&csConnectionPool);
	hThread = NULL;
	isEnd = FALSE;
	iErrorCount = 0;
	iConnectIndex = 0;
	InitializeCriticalSection(&m_csAC);
}

cADO_AnalyzeTool::~cADO_AnalyzeTool()
{
	Close();
	DeleteCriticalSection(&csConnectionPool);
	DeleteCriticalSection(&m_csAC);
}

void
cADO_AnalyzeTool::getTimeToUTime(char *_lpstrDate,UTime *_lpUTime)
{
	int	iYear,iMonth,iDay,iHour,iMinute,iSec;
#ifdef _FOR_USA
	{
		char	strTime[8];
		int idx = 0;

		memcpy(strTime,_lpstrDate+idx,2);	//	일
		strTime[2]	=	0;
		if(strTime[1] == '/')
		{
			strTime[1] = 0;
			iMonth		=	atoi(strTime);
			idx += 2;
		}else
		{
			iMonth		=	atoi(strTime);
			idx += 3;
		}

		memcpy(strTime,_lpstrDate+idx,2);	//	월
		strTime[2]	=	0;
		if(strTime[1] == '/')
		{
			strTime[1] = 0;
			iDay		=	atoi(strTime);
			idx += 2;
		}else
		{
			iDay		=	atoi(strTime);
			idx += 3;
		}

		memcpy(strTime,_lpstrDate+idx,4);		//	연
		strTime[4]	=	0;
		iYear		=	atoi(strTime);
		idx += 5;

		//	eng는 mm-dd-yyyy hh:mm:ss AM/PM형태 
		if(_lpstrDate[idx+1]==':')
		{
			memcpy(strTime,_lpstrDate+idx,1);	//	시
			idx -= 1;
			strTime[1]	=	0;
		}else
			memcpy(strTime,_lpstrDate+idx,2);	//	시
		strTime[2]	=	0;
		iHour		=	atoi(strTime);
		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	분
		strTime[2]	=	0;
		iMinute		=	atoi(strTime);

		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	분
		strTime[2]	= 0;
		iSec		=	atoi(strTime);
	}
#elif defined(_FOR_KOREA) || defined(_FOR_INTER) 
	{
		char	strTime[8];
		int idx = 0;
		memcpy(strTime,_lpstrDate+idx,4);		//	연
		strTime[4]	=	0;
		iYear		=	atoi(strTime);
		
		idx += 5;

		memcpy(strTime,_lpstrDate+idx,2);	//	월
		strTime[2]	=	0;
		iMonth		=	atoi(strTime);

		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	일
		strTime[2]	=	0;
		iDay		=	atoi(strTime);

		idx += 3;

		int addHour = 0;
		if(strncmp(_lpstrDate+idx,"오전",4)==0)
		{
			idx += 4;
		}else
		if(strncmp(_lpstrDate+idx,"오후",4)==0)
		{
			addHour = 1;
			idx += 4;
		}else
		{
			addHour = 2;
		}

		if(_lpstrDate[idx+2]==':')
		{
			memcpy(strTime,_lpstrDate+idx,2);	//	시
			idx -= 1;
		}else{
			memcpy(strTime,_lpstrDate+idx+1,2);	//	시
		}
		strTime[2]	=	0;
		iHour		=	atoi(strTime);
		if(addHour	==	1)
		{
			iHour += 12;
		}else if(addHour == 0)
		{
			if(iHour==12)
				iHour = 0;
		}
		if(iHour>=24)
		{
			iHour -= 12;
		}

		idx += 4;

		memcpy(strTime,_lpstrDate+idx,2);	//	분
		strTime[2]	=	0;
		iMinute		=	atoi(strTime);

		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	분
		strTime[2]	= 0;
		iSec		=	atoi(strTime);
	}
#else
	{
		char	strTime[8];
		int idx = 0;
		memcpy(strTime,_lpstrDate+idx,4);		//	연
		strTime[4]	=	0;
		iYear		=	atoi(strTime);
		
		idx += 5;

		memcpy(strTime,_lpstrDate+idx,2);	//	월
		strTime[2]	=	0;
		iMonth		=	atoi(strTime);

		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	일
		strTime[2]	=	0;
		iDay		=	atoi(strTime);

		idx += 3;

		//	일본은 yyyy-mm-dd hh:mm:ss형태 
		if(_lpstrDate[idx+1]==':')
		{
			memcpy(strTime,_lpstrDate+idx,1);	//	시
			idx -= 1;
			strTime[1]	=	0;
		}else
			memcpy(strTime,_lpstrDate+idx,2);	//	시
		strTime[2]	=	0;
		iHour		=	atoi(strTime);
		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	분
		strTime[2]	=	0;
		iMinute		=	atoi(strTime);

		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	분
		strTime[2]	= 0;
		iSec		=	atoi(strTime);
	}
#endif

	if (iYear	<	2000)
		_lpUTime->m_dwValue	=	0;
	else
	{
		_lpUTime->year	=	iYear-2000;
		_lpUTime->month	=	iMonth;
		_lpUTime->day	=	iDay;
		_lpUTime->hour	=	iHour;
		_lpUTime->minute=	iMinute;
		_lpUTime->second=	iSec;
	}
}

_ConnectionPtr		cADO_AnalyzeTool::GetConnection()	//	연결되어 있으나 사용중이지 않은 것을 보내준다.
{
	EnterCriticalSection(&csConnectionPool);
	for(int i = iConnectIndex*3;i< iConnectIndex*3+3;i++)
	{
		if(pConnection[i]->GetState() == adStateOpen)
		{
			LeaveCriticalSection(&csConnectionPool);
			return pConnection[i];
		}else if(pConnection[i]->GetState() == adStateClosed)
		{
//			reConnect(i);
		}
	}
	_log("cADO_AnalyzeTool::GetConnection() -> return NULL \n");
	LeaveCriticalSection(&csConnectionPool);
	return NULL;
}

BOOL	cADO_AnalyzeTool::Connect(char *ip,char *id,char *pw,char *db,int count)
{
//	iMaxCount = count;
	iMaxCount = dDBCONNECT_COUNT;
	iErrorCount = 0;
	iConnectIndex = 0;
	strcpy(strDBIp,ip);
	strcpy(strDBId,id);
	strcpy(strDBPw,pw);
	strcpy(strDBDb,db);

	try
	{
		HRESULT hr	=	CoInitialize(0);
		sprintf(pro,"PROVIDER=SQLOLEDB;SERVER=%s;UID=%s;PWD=%s;DATABASE=%s;Network Address=%s,%d;Network Library=DBMSSOCN",strDBIp,strDBId,strDBPw,strDBDb,strDBIp,dDATABASEPORT);
		_bstr_t		strProvider(pro);

		for(int i = 0;i< 3 ;i++)
		{
			pConnection[i].CreateInstance("ADODB.Connection");
			hr = pConnection[i]->Open(strProvider,L"",L"",-1);
			if(FAILED(hr))
			{
				_log("Fisrt Connection 실패 \n");
				return FALSE;
			}
		}
		return TRUE;
	}
	catch(_com_error &e)
	{
		_log(pro);
		DMPcomERR(e);
		return FALSE;
	}
}

void	cADO_AnalyzeTool::Close()
{
	if(!isEnd)
	{
		continueThread = FALSE;
		for(int i = 0;i< iMaxCount;i++)
		{
			if(pConnection[i])
				if(pConnection[i]->GetState()==adStateOpen)	pConnection[i]->Close();
		}

		isEnd = TRUE;
	}
}

void	cADO_AnalyzeTool::dmpCOMerr( _com_error &e, int line, char *file, bool traceOnly)
{
	char pline[1024];
	sprintf(pline," Line: %d  File %s", line, file);
	if(GlastLine)
		sprintf(pline," \n Line: %d ; Last Line = %d File %s \n", line,GlastLine, file);
	try
	{
		_bstr_t bstrSource(e.Source());
		_bstr_t bs =  _bstr_t(" \n Error: ") + _bstr_t(e.Error()) + _bstr_t(" \n Msg: ") 
			+ _bstr_t(e.ErrorMessage()) + _bstr_t(" \n Description: ") 
			+ _bstr_t(e.Description()) + _bstr_t(pline) ;
		_log("\n %s %s \n",(char *) bs, (char *) bstrSource);
	}
	catch(...)	{}
	if(e.Error() == -2147024882)	//	저장소가 부족하데.. -_-;
	{
	}
	if(e.Error()==-2146825023 || e.Error()==-2147467259)
	{
		EnterCriticalSection(&csConnectionPool);
		iErrorCount++;	//	연속적인 에러일경우..
		if(iErrorCount >dERROR_COUNT)
		{
			_log(" error count %d , 접속이 끊어진 걸로 간주하고, 새롭게 연결해서 사용한다. ",iErrorCount);
			//	접속이 끊어진 걸로 간주하고, 새롭게 연결해서 사용한다.
			try
			{
				HRESULT hr	=	CoInitialize(0);
				sprintf(pro,"PROVIDER=SQLOLEDB;SERVER=%s;UID=%s;PWD=%s;DATABASE=%s;Network Address=%s,5316;Network Library=DBMSSOCN",strDBIp,strDBId,strDBPw,strDBDb,strDBIp);
				_bstr_t		strProvider(pro);
				for(int i = (iConnectIndex+1)*3;i< (iConnectIndex+1)*3+3 ;i++)
				{
					pConnection[i].CreateInstance("ADODB.Connection");
					hr = pConnection[i]->Open(strProvider,L"",L"",-1);
					if(FAILED(hr))
					{
						_log("%d번째 New Connection 실패 1\n",iConnectIndex+1);
						iErrorCount = 0;
//						dwErrorTime = 0;
						LeaveCriticalSection(&csConnectionPool);
						return ;
					}
				}
			}catch(...)
			{
				_log("%d번째 New Connection 실패 2\n",iConnectIndex);
				iErrorCount = 0;
//				dwErrorTime = 0;
				LeaveCriticalSection(&csConnectionPool);
				return;
			}
			iConnectIndex++;
			_log("%d번째 New Connection 성공.. \n",iConnectIndex);
			iErrorCount = 0;
//			dwErrorTime = 0;
		}
		LeaveCriticalSection(&csConnectionPool);
	}
}

void	cADO_AnalyzeTool::BlobToVariant(VARIANT &varChunk,void *pData, UINT nBytes)	//	data -> blob
{
	HRESULT hr;
	long lngOffset = 0;
	UCHAR chData;
	SAFEARRAY FAR *psa = NULL;
	SAFEARRAYBOUND rgsabound[1];
	try
	{
		//Create a safe array to store the array of BYTES 
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = nBytes;
		psa = SafeArrayCreate(VT_UI1,1,rgsabound);
		if(psa != NULL)
		{
			while(lngOffset < (long)nBytes)
			{
				chData	= ((UCHAR*)pData)[lngOffset];
				hr = SafeArrayPutElement(psa, &lngOffset, &chData);

				lngOffset++;
			}
			lngOffset = 0;

			//Assign the Safe array  to a variant.
			VariantInit(&varChunk);
			varChunk.vt = VT_ARRAY|VT_UI1;
			varChunk.parray = psa;
		}else{
			_log("ERROR BlobToVariant : psa = NULL");
		}
	}
	catch(_com_error &e)
	{
		DMPcomERR(e);
	}
//	SafeArrayUnaccessData(psa);
//	SafeArrayDestroyData(psa);
//	SafeArrayDestroy(psa);

	return;
}

int		cADO_AnalyzeTool::VariantToBlob(VARIANT & varChunk,void * pData)	//	blob -> data
{
	if(varChunk.vt==VT_NULL)	return FALSE;

	HRESULT			hr = CoInitialize(0);
	UCHAR chData;
	long lBytesCopied = 0;

	long length = varChunk.parray->rgsabound->cElements;
	memset(pData,0,length);
	for(long lIndex = 0; lIndex < length; lIndex++)
	{
		hr= SafeArrayGetElement(varChunk.parray, &lIndex, &chData);
		if(SUCCEEDED(hr))
		{
			((UCHAR*)pData)[lBytesCopied] = chData;
			lBytesCopied++;
		}
		else
			break;
	}
	return lBytesCopied;
}

BOOL	__cdecl cADO_AnalyzeTool::sqlExecute(char *str,...)	//가능하면 이걸로 사용하지 말고 SP를 사용해라.
{
	_CommandPtr	pCmd				= NULL;
	char		str_Query[1024];
    HRESULT		hr					= CoInitialize(0);

	va_list     ap; 
	va_start(ap, str); 
	vsprintf(str_Query, str, ap); 
	va_end(ap); 
		
	pCmd.CreateInstance(__uuidof(Command));
	pCmd->ActiveConnection  = GetConnection();
	pCmd->CommandText  = str_Query;
	hr = pCmd->Execute(NULL,NULL,adCmdText);

	if(FAILED(hr))
		return FALSE;
	return TRUE;
}

int		cADO_AnalyzeTool::spGetDetailUserCountStat(int day,int srvIdx, vector<stUserStatsPack> * _pVector)
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
	if(FAILED(hr) || i_RecCount<=0)	
		return 0;

	_pVector->clear();
	stUserStatsPack	tmpPack;
	memset(&tmpPack,0,sizeof(stUserStatsPack));
	int i=0;
//	for(int i=0; i< 1440;i++)
//		_pVector->push_back(tmpPack);
	
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
		if(pack.iReserve1UserCount>100000 || pack.iReserve1UserCount<0)		pack.iReserve1UserCount = 0;
		v_bufferdata = pRs->Fields->GetItem((LONG)3)->GetValue();
		pack.iReserve2UserCount = (int)v_bufferdata.intVal;
		if(pack.iReserve2UserCount>100000 || pack.iReserve2UserCount<0)		pack.iReserve2UserCount = 0;
		v_bufferdata = pRs->Fields->GetItem((LONG)4)->GetValue();
		pack.iReserve3UserCount = (int)v_bufferdata.intVal;
		if(pack.iReserve3UserCount>100000 || pack.iReserve3UserCount<0)		pack.iReserve3UserCount = 0;
		v_bufferdata = pRs->Fields->GetItem((LONG)5)->GetValue();
		pack.iReserve4UserCount = (int)v_bufferdata.intVal;
		if(pack.iReserve4UserCount>100000 || pack.iReserve4UserCount<0)		pack.iReserve4UserCount = 0;
		v_bufferdata = pRs->Fields->GetItem((LONG)6)->GetValue();
		pack.iReserve5UserCount = (int)v_bufferdata.intVal;
		if(pack.iReserve5UserCount>100000 || pack.iReserve5UserCount<0)		pack.iReserve5UserCount = 0;

		int t = time.hour*60 + time.minute;
		if(t<1440)
			_pVector->push_back(pack);
		VariantClear(&v_bufferdata);

		pRs->MoveNext();
	}
	if(i<1440)
	{
		int l = 1440-i;
		while(l--)
		{
			_pVector->push_back(tmpPack);
		}
	}
	pRs->Close();

	return 1;
}