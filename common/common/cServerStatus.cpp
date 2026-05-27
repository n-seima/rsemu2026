#include "cServerStatus.h"
#include "cSRVUTIL.h"

cServerStatus::cServerStatus()
{
	memset(m_strServerName,0,64);
	memset(m_strIp,0,32);

	memset(m_strUserDBServerSource,0,128);
	memset(m_strUserDBServerId,0,64);
	memset(m_strUserDBServerPw,0,64);
	memset(m_strUserDBIP,0,32);
}

cServerStatus::~cServerStatus()
{
	release();
}

BOOL
cServerStatus::init(char * _worldname, char * _subname)
{
	WSADATA wsadata;
	if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
		return FALSE;
	
	char name[256];
	PHOSTENT hostinfo;
	if( gethostname(name, sizeof(name)) == 0)
		if((hostinfo = gethostbyname(name)) != NULL)
			strcpy( m_strIp, inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list) );

	sprintf(m_strServerName, "%s_%s",_worldname,_subname);

	FILE * accInfoFp = fopen("SSDBInfo.bin","rb");
	if(!accInfoFp)
	{
		ERRMSG("Can not read Account db connection info");
		return FALSE;
	}
	fread(m_strUserDBServerSource,128,1,accInfoFp);
	fread(m_strUserDBServerId,64,1,accInfoFp);
	fread(m_strUserDBServerPw,64,1,accInfoFp);
	fread(m_strUserDBIP,32,1,accInfoFp);
	fclose(accInfoFp);

	return Connect(m_strUserDBIP,m_strUserDBServerId,m_strUserDBServerPw,m_strUserDBServerSource,1);
}

void	
cServerStatus::release()
{
	Close();
}

int		
cServerStatus::update(int _iErrorType, int _iParam1, char * _strParam2 )
{
/*	try
	{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spSS_Insert";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return 0;	}

		DEF_PARAM(pCmd,"@strName") = _variant_t((char *)m_strServerName);
		DEF_PARAM(pCmd,"@strIp") = _variant_t((char *)m_strIp);
		DEF_PARAM(pCmd,"@iErrorType") = _variant_t((long)_iErrorType);
		DEF_PARAM(pCmd,"@Param1_int") = _variant_t((long)_iParam1);
		DEF_PARAM(pCmd,"@Param2_char512") = _variant_t((char *)_strParam2);
		pCmd->Execute(NULL,NULL,NULL);
		pCmd.Release();
		return 1 ;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e)
		return 0;
	}
*/
	return 1;
}

int
cServerStatus::getSS()
{
	char			str_Query[1024];
	DWORD			dw_RecCount;
	DWORD			i;
	HRESULT			hr = CoInitialize(0);

	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"SELECT iIdx,dateValue,strName,strIp,iErrorType,Param1_int,Param2_char512 FROM ServerStatus WHERE iErrorType = %d" , cServerStatus::eERROR_TYPE_SERVERSTATUS);
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		dw_RecCount = pRs->RecordCount;

		if(FAILED(hr))	return FALSE;

		m_listSSInfo.clear();
		for(i = 0 ;i < dw_RecCount ;i++)
		{
			cSSInfoTable	info;
			char	strDate[64];
			_variant_t		v_bufferdata;
			
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			info.iIdx = (int)v_bufferdata.intVal;

			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			info.iIdx = (int)v_bufferdata.intVal;
			strcpy(strDate,(_bstr_t)v_bufferdata);
			getTimeToUTime(strDate,&info.utTime);

			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			strcpy(info.strName,(_bstr_t)v_bufferdata);
			
			v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
			strcpy(info.strIp,(_bstr_t)v_bufferdata);

			v_bufferdata = pRs->Fields->GetItem((LONG)4)->Value;
			info.iErrorType = (int)v_bufferdata.intVal;

			v_bufferdata = pRs->Fields->GetItem((LONG)5)->Value;
			info.Param1_int = (int)v_bufferdata.intVal;

			v_bufferdata = pRs->Fields->GetItem((LONG)6)->Value;
			strcpy(info.Param2_char512,(_bstr_t)v_bufferdata);
			VariantClear(&v_bufferdata);
			m_listSSInfo.push_back(info);

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

int
cServerStatus::getError(BOOL isFirst)
{
	char			str_Query[1024];
	DWORD			dw_RecCount;
	DWORD			i;
	HRESULT			hr = CoInitialize(0);

	try
	{
		_RecordsetPtr	pRs = NULL;
		if(isFirst)
		{
			char stDate[64];
			sprintf(stDate,"%d-%d-%d %d:%d:00", cSRVUTIL::GetYear()+1900, cSRVUTIL::GetMonth()+1, cSRVUTIL::GetDay(), cSRVUTIL::GetHour(),cSRVUTIL::GetMin() );
			sprintf(str_Query,"SELECT iIdx,dateValue,strName,strIp,iErrorType,Param1_int,Param2_char512 FROM ServerStatus WHERE iErrorType >= %d and dateValue >= '%s' " , cServerStatus::eERROR_TYPE_DB_SHUTDOWN, stDate);
		}else
		{
			char stDate[64];
			sprintf(stDate,"%d-%d-%d %d:00:00", cSRVUTIL::GetYear()+1900, cSRVUTIL::GetMonth()+1, cSRVUTIL::GetDay(), cSRVUTIL::GetHour());
			sprintf(str_Query,"SELECT iIdx,dateValue,strName,strIp,iErrorType,Param1_int,Param2_char512 FROM ServerStatus WHERE iErrorType >= %d and dateValue >= '%s' " , cServerStatus::eERROR_TYPE_DB_SHUTDOWN, stDate);
		}
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		dw_RecCount = pRs->RecordCount;

		if(FAILED(hr))	return FALSE;

		for(i = 0 ;i < dw_RecCount ;i++)
		{
			cSSInfoTable	info;
			char	strDate[64];
			_variant_t		v_bufferdata;
			
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			info.iIdx = (int)v_bufferdata.intVal;

			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			info.iIdx = (int)v_bufferdata.intVal;
			strcpy(strDate,(_bstr_t)v_bufferdata);
			getTimeToUTime(strDate,&info.utTime);

			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			strcpy(info.strName,(_bstr_t)v_bufferdata);
			
			v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
			strcpy(info.strIp,(_bstr_t)v_bufferdata);

			v_bufferdata = pRs->Fields->GetItem((LONG)4)->Value;
			info.iErrorType = (int)v_bufferdata.intVal;

			v_bufferdata = pRs->Fields->GetItem((LONG)5)->Value;
			info.Param1_int = (int)v_bufferdata.intVal;

			v_bufferdata = pRs->Fields->GetItem((LONG)6)->Value;
			strcpy(info.Param2_char512,(_bstr_t)v_bufferdata);
			VariantClear(&v_bufferdata);

			for(int l=0; l<m_listSSError.size(); l++)	//	같은 인덱스가 존재하면 취소..
			{
				if( info == m_listSSError.at(l) )
					break;
			}
			if(l == m_listSSError.size())
				m_listSSError.push_back(info);

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