#include "cPLAYER_DATA.h"
#include "cADO_BASE.h"
#include <stdio.h>
#include <process.h>
#include "cSRVUTIL.h"
#include "RSComm.h"
#include <string>

//_variant_t  vtEmpty2 (DISP_E_PARAMNOTFOUND, VT_ERROR);

cADO_BASE::cADO_BASE()
{
	GlastLine = 0;
	InitializeCriticalSection(&csConnectionPool);
	hThread = NULL;
	isEnd = FALSE;
	iErrorCount = 0;
	iConnectIndex = 0;
	iErrorProblem = 0;
	InitializeCriticalSection(&m_csAC);
}

cADO_BASE::~cADO_BASE()
{
	Close();
	DeleteCriticalSection(&csConnectionPool);
	DeleteCriticalSection(&m_csAC);
}

void
cADO_BASE::getTimeToUTime(char *_lpstrDate,UTime *_lpUTime)
{
	int	iYear,iMonth,iDay,iHour,iMinute,iSec;

	std::string _checkTimeFormat = _lpstrDate;

#if defined(_FOR_KOREA) || defined(_FOR_INTER)
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
#elif	defined(_OGP_SERVICE) || defined(_FOR_USA)
	{
		char	strTime[8];
		int idx = 0;

		if(_checkTimeFormat.find("오전")!=std::string::npos  || _checkTimeFormat.find("오후")!=std::string::npos)
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
		else
		{
			memcpy(strTime,_lpstrDate+idx,2);	//	월
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
			
			memcpy(strTime,_lpstrDate+idx,2);	//	일
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
			
			memcpy(strTime,_lpstrDate+idx,2);	//	초
			strTime[2]	= 0;
			iSec		=	atoi(strTime);
			
			idx += 3;
			
			memcpy(strTime,_lpstrDate+idx,2);	//	AM/PM
			strTime[2]	= 0;
			_log("strTime = %s , %d ",strTime, iHour);
			if( strncmp(strTime, "AM", 2) ==0 && iHour == 12)
			{
				iHour = 0;
			}else
			if( strncmp(strTime, "PM", 2) ==0 && iHour != 12)
			{
				_log("again strTime = %s , %d ",strTime, iHour);
				iHour += 12;
			}			
		}
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

		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	분
		strTime[2]	= 0;
		if( strncmp(strTime, "AM", 2) ==0 && iHour == 12)
		{
			iHour = 0;
		}else
		if( strncmp(strTime, "PM", 2) ==0 && iHour != 12)
		{
			iHour += 12;
		}
	}
#endif

	if (iYear	<	2000 || iYear	>	2100)
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

_ConnectionPtr		cADO_BASE::GetConnection()	//	연결되어 있으나 사용중이지 않은 것을 보내준다.
{
	try
	{
		EnterCriticalSection(&csConnectionPool);
		for(int i = 0;i< iMaxCount ;i++)
		{
			if(pConnection[i]->GetState() == adStateOpen)
			{
				if(i<2 && iErrorProblem)
					continue;
					
				LeaveCriticalSection(&csConnectionPool);
				return pConnection[i];
			}else if(pConnection[i]->GetState() == adStateClosed)
			{
				_log(" connection is closed. idx %d",i);
				++iErrorCount;
		//			reConnect(i);
			}
		}
		_log("cADO_BASE::GetConnection() -> return NULL \n");
		LeaveCriticalSection(&csConnectionPool);
	}
	catch(_com_error &e)
	{
		_log(mainPro);
		DMPcomERR(e);
		return FALSE;
	}
	return NULL;
}

BOOL	cADO_BASE::reConnect()
{
	Close();
	isEnd = FALSE;

	try
	{
		HRESULT hr	=	CoInitialize(0);
		_bstr_t		strProvider(mainPro);

		for(int i = 0;i< iMaxCount ;i++)
		{
			pConnection[i].CreateInstance("ADODB.Connection");
			hr = pConnection[i]->Open(strProvider,L"",L"",-1);
			if(FAILED(hr))
			{
				_log("reConnection 실패 \n");
				return FALSE;
			}
		}
		return TRUE;
	}
	catch(_com_error &e)
	{
		_log(mainPro);
		DMPcomERR(e);
		return FALSE;
	}
}

BOOL	cADO_BASE::Connect(char *ip,char *id,char *pw,char *db,int count)
{
	iMaxCount = count;
//	iMaxCount = dDBCONNECT_COUNT;
	iErrorCount = 0;
	iConnectIndex = 0;
	strcpy(strDBIp,ip);
	strcpy(strDBId,id);
	strcpy(strDBPw,pw);
	strcpy(strDBDb,db);

	try
	{
		HRESULT hr	=	CoInitialize(0);
		sprintf(mainPro,"PROVIDER=SQLOLEDB;SERVER=%s;UID=%s;PWD=%s;DATABASE=%s;Network Address=%s,%d;Network Library=DBMSSOCN",strDBIp,strDBId,pw,strDBDb,strDBIp,dDATABASEPORT);
//		_log(" check DB Connection infomation : [%s]",mainPro);
		_bstr_t		strProvider(mainPro);
		for(int i = 0;i< iMaxCount ;i++)
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
//		_log(mainPro);
		ERRMSG(mainPro);
		DMPcomERR(e);
		strcpy(mainPro,e.ErrorMessage());
		ERRMSG(mainPro);
		return FALSE;
	}
}

void	cADO_BASE::Close()
{
	if(!isEnd)
	{
		continueThread = FALSE;
		for(int i = 0;i< iMaxCount ;i++)
		{
			if(pConnection[i])
			{
				if(pConnection[i]->GetState() != adStateClosed)	
					pConnection[i]->Close();
			}
		}
		isEnd = TRUE;
	}
}

void	cADO_BASE::dmpCOMerr( _com_error &e, int line, char *file, bool traceOnly)
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
		if(iErrorCount>1)
			iErrorProblem = 1;
		if(iErrorCount >dERROR_COUNT)
		{
			_log(" error count %d , 접속이 끊어진 걸로 간주하고, 새롭게 연결해서 사용한다. ",iErrorCount);
			//	접속이 끊어진 걸로 간주하고, 새롭게 연결해서 사용한다.
			reConnect();
			iErrorCount = 0;
		}
		LeaveCriticalSection(&csConnectionPool);
	}
}

void	cADO_BASE::BlobToVariant(VARIANT &varChunk,void *pData, UINT nBytes)	//	data -> blob
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

int		cADO_BASE::VariantToBlob(VARIANT & varChunk,void * pData)	//	blob -> data
{
	if(varChunk.vt==VT_NULL || pData == NULL)	return FALSE;

	HRESULT			hr = CoInitialize(0);
	UCHAR chData;
	long lBytesCopied = 0;

	if(!varChunk.parray || !varChunk.parray->rgsabound)
		return 0;
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

BOOL	__cdecl cADO_BASE::sqlExecute(char *str,...)	//가능하면 이걸로 사용하지 말고 SP를 사용해라.
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
