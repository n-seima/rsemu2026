#include "StdAfx.h"
#include "AdoDB.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
cAdoDB::cAdoDB(void)
{
	m_iDBConnectCnt = 0;	
	m_pstAdoDBData = NULL;
	m_pbRestDBConnect = NULL;
	CoInitialize(0);
	InitializeCriticalSection(&m_csDBIdx);	
	m_iDBConnectCnt = 0;	
	m_pbRestDBConnect = NULL;
	InitializeCriticalSection(&m_csDBIdx);	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
cAdoDB::~cAdoDB(void)
{
	if(m_pstAdoDBData){
		disConnect();
	}
	CoUninitialize();
	DeleteCriticalSection(&m_csDBIdx);
	DeleteCriticalSection(&m_csDBIdx);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cAdoDB::connect(TCHAR *_pszIP, int _iPort, TCHAR *_pszDBName, TCHAR *_pszID, TCHAR *_pszPW, int _iDBConnectCnt)
{	
	if(m_pstAdoDBData){
		return FALSE;
	}
	m_iDBConnectCnt = _iDBConnectCnt;

	m_pstAdoDBData = new stADODBDATA[_iDBConnectCnt];
	memset(m_pstAdoDBData, 0, sizeof(stADODBDATA) * _iDBConnectCnt);
	m_pbRestDBConnect = new BOOL[_iDBConnectCnt];
	memset(m_pbRestDBConnect, TRUE, sizeof(BOOL) * _iDBConnectCnt);		// TRUE 로 초기화
	
	TCHAR szPro[512];
	try{		
		HRESULT hr;

		// 접속이 끊길경우 재접속을 위해
		_tcscpy_s(m_stLoginInfo.szIP, _countof(m_stLoginInfo.szIP), _pszIP);
		_tcscpy_s(m_stLoginInfo.szDBName, _countof(m_stLoginInfo.szDBName), _pszDBName);
		_tcscpy_s(m_stLoginInfo.szID, _countof(m_stLoginInfo.szID), _pszID);
		_tcscpy_s(m_stLoginInfo.szPW, _countof(m_stLoginInfo.szPW), _pszPW);		

		_stprintf_s(szPro, _countof(szPro), 
				_T("PROVIDER=SQLOLEDB;SERVER=%s;UID=%s;PWD=%s;DATABASE=%s;Network Address=%s,%d;Network Library=DBMSSOCN"), 
				_pszIP, _pszID, _pszPW, _pszDBName, _pszIP, _iPort);		
		bstr_t strProvider(szPro);
		for(int i=0; i<_iDBConnectCnt; i++){
			m_pstAdoDBData[i].pConnection.CreateInstance(_T("ADODB.Connection"));			
			m_pstAdoDBData[i].pCommand.CreateInstance( __uuidof( Command ) );	
			m_pstAdoDBData[i].pRecordset.CreateInstance(__uuidof(Recordset));
			m_pstAdoDBData[i].pRecordset->CursorLocation = adUseClient;			
			hr = m_pstAdoDBData[i].pConnection->Open(strProvider, _T(""), _T(""), adCmdUnspecified);
			if(FAILED(hr)){
				disConnect();
				return FALSE;
			}
			m_pstAdoDBData[i].pCommand->ActiveConnection = m_pstAdoDBData[i].pConnection;
		}		
	}catch(_com_error &e){
		errorHandling(0, e, szPro, _T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::disConnect()
{
	for(int i=0; i<m_iDBConnectCnt; i++){
		if(m_pstAdoDBData[i].pConnection){
			if(m_pstAdoDBData[i].pConnection->GetState() == adStateOpen){
				m_pstAdoDBData[i].pConnection->Close();
				m_pstAdoDBData[i].pConnection = NULL;
			}
		}
		if(m_pstAdoDBData[i].pCommand){
			if(m_pstAdoDBData[i].pCommand->GetState() == adStateOpen){
				m_pstAdoDBData[i].pCommand->Release();
				m_pstAdoDBData[i].pCommand = NULL;
			}
		}
		if(m_pstAdoDBData[i].pRecordset){
			if(m_pstAdoDBData[i].pRecordset->GetState() == adStateOpen){				
				m_pstAdoDBData[i].pRecordset->Release();
				m_pstAdoDBData[i].pRecordset = NULL;
			}
		}
	}

	// 가끔 에러가 나서 try() 처리
	try{
		if(m_pstAdoDBData){
			delete []m_pstAdoDBData;
			m_pstAdoDBData = NULL;
		}
		if(m_pbRestDBConnect){
			delete []m_pbRestDBConnect;
			m_pbRestDBConnect = NULL;
		}
	}catch(...){
		;	// 어차피 대부분 프로그램 종료시에 호출되므로 catch() 로 빠져도 특별히 해줄것은 없다
	}

	m_iDBConnectCnt = 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cAdoDB::execSql(int _iDBIdx, TCHAR *_pszSQL, TCHAR *_pszFunction, int _iLine)
{
	try{
		m_pstAdoDBData[_iDBIdx].pCommand->CommandText = _pszSQL;
		m_pstAdoDBData[_iDBIdx].pRecordset = m_pstAdoDBData[_iDBIdx].pCommand->Execute(NULL, NULL, adCmdText);
		if(FAILED(m_pstAdoDBData[_iDBIdx].pRecordset)){
			DWORD hr = GetLastError();
			_com_error e(hr);
			errorHandling(_iDBIdx, e, _pszSQL, _pszFunction, _iLine);
			return FALSE;
		}
	}catch(_com_error &e){
		errorHandling(_iDBIdx, e, _pszSQL, _pszFunction, _iLine);
		return FALSE;
	}
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::errorHandling(int _iDBIdx, _com_error &e, TCHAR *_pszSQL, TCHAR *_pszFunction, int _iLine)
{
	TCHAR szPrint[512];
	_stprintf_s(szPrint, _countof(szPrint), _T("Error! %s(%d), %s"), _pszFunction, _iLine, _pszSQL);
	_bstr_t bstrSource(e.Source());
	_bstr_t bs = _bstr_t(e.ErrorMessage()) + _bstr_t(_T("\n")) + _bstr_t(e.Description());

	clearData(_iDBIdx);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 데이타 초기화
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::clearData(int _iDBIdx)
{
	// 사용한 파라미터를 지운다
	for(int i=0; i<m_pstAdoDBData[_iDBIdx].iAllParamIdx; i++){
		m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Delete(m_pstAdoDBData[_iDBIdx].aszParamName[i]);
	}
	m_pstAdoDBData[_iDBIdx].iOutputParamIdx = 0;
	m_pstAdoDBData[_iDBIdx].iAllParamIdx = 0;

	// pRecordset 객체를 사용했으므로 
	if(m_pstAdoDBData[_iDBIdx].pRecordset->GetState() == adStateOpen){
		m_pstAdoDBData[_iDBIdx].pRecordset->Close();
	}

	setRestDBConnect(_iDBIdx, TRUE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::moveNextData(int _iDBIdx)
{
	m_pstAdoDBData[_iDBIdx].pRecordset->MoveNext();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cAdoDB::getData(int _iDBIdx)
{	
	if(m_pstAdoDBData[_iDBIdx].pRecordset == NULL){
		return FALSE;
	}else{
		return !m_pstAdoDBData[_iDBIdx].pRecordset->adoEOF;
	}
}


	
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 1Byte 정수형 컬럼 읽기
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BYTE cAdoDB::getTinyInt(int _iDBIdx, TCHAR *_pszCol)
{
	if(m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value.vt == VT_NULL){
		return 0;	
	}else{
		return m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value;
	}	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 2Byte 정수형 컬럼 읽기	
///////////////////////////////////////////////////////////////////////////////////////////////////////////
short cAdoDB::getSmallInt(int _iDBIdx, TCHAR *_pszCol)
{
	if(m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value.vt == VT_NULL){
		return 0;	
	}else{
		return m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value;
	}	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 4Byte 정수형 컬럼 읽기	
///////////////////////////////////////////////////////////////////////////////////////////////////////////
int cAdoDB::getInt(int _iDBIdx, TCHAR *_pszCol)
{
	if(m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value.vt == VT_NULL){
		return 0;	
	}else{
		return m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value;
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 8Byte 정수형 컬럼 읽기	
///////////////////////////////////////////////////////////////////////////////////////////////////////////
LONGLONG cAdoDB::getBigInt(int _iDBIdx, TCHAR *_pszCol)
{
	if(m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value.vt == VT_NULL){
		return 0;	
	}else{
		VARIANT temp;
		LONGLONG llResult;
		temp = m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value;
		llResult = temp.llVal;
		VariantClear(&temp);
		return llResult;
	}	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : char, DB 컬럼 Type 이 char 일때 버퍼의 크기가 DB 의 컬럼 사이즈보다 1Byte 가 더 커야한다 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::getStr(int _iDBIdx, TCHAR *_pszCol, char *_pszBuf, int _iBufSize)
{
	if(m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value.vt == VT_NULL){
		strcpy_s(_pszBuf, _iBufSize, "");		
	}else{
		strcpy_s(_pszBuf, _iBufSize, (_bstr_t)m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value);	
	}	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : varchar
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::getVarStr(int _iDBIdx, TCHAR *_pszCol, char *_pszBuf, int _iBufSize)
{
	getStr(_iDBIdx, _pszCol, _pszBuf, _iBufSize);		
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : nchar, DB 컬럼 Type 이 nchar 일때 버퍼의 크기가 DB 의 컬럼 사이즈보다 1Byte 가 더 커야한다
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::getNStr(int _iDBIdx, TCHAR *_pszCol, TCHAR *_pszBuf, int _iBufSize)
{
	if(m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value.vt == VT_NULL){
		_tcscpy_s(_pszBuf, _iBufSize, _T(""));		
	}else{
		_tcscpy_s(_pszBuf, _iBufSize, (_bstr_t)m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value);	
	}	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : nvarchar
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::getNVarStr(int _iDBIdx, TCHAR *_pszCol, TCHAR *_pszBuf, int _iBufSize)
{
	getNStr(_iDBIdx, _pszCol, _pszBuf, _iBufSize);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : datetime
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::getDate(int _iDBIdx, TCHAR *_pszCol, SYSTEMTIME *_psystimeDate)
{
	TCHAR szDate[40];
	getNStr(_iDBIdx, _pszCol, szDate, _countof(szDate));
	strToTime(szDate, _psystimeDate);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : Binary
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::getBinary(int _iDBIdx, TCHAR *_pszCol, void *_pszBuf, int _iBufSize)
{
	if(m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value.vt == VT_NULL){
		memset(_pszBuf, 0, _iBufSize);		
	}else{
		_variant_t bin;		// _variant_t 를 VARIANT 로 하면 Error
		VariantInit(&bin);		
		bin = m_pstAdoDBData[_iDBIdx].pRecordset->Fields->Item[_pszCol]->Value;
		variantToBlob(bin, _pszBuf);			
		VariantClear(&bin);
	}	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : VarBinary
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::getVarBinary(int _iDBIdx, TCHAR *_pszCol, void *_pszBuf, int _iBufSize)
{
	getBinary(_iDBIdx, _pszCol, _pszBuf, _iBufSize);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 프로시져 INPUT PARAM
//        TCHAR 관련 데이타를 넣을때는 _iParamSize 는 _tcslen() 의 값이다
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::setProcedureInputParam(int _iDBIdx, const TCHAR *_pszParamName, int _iDataType, const void *_pParamValue, int _iParamSize)
{
	switch(_iDataType)
	{
		case eTINYINT:		// TinyInt
			{
				m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
						m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adTinyInt, adParamInput, _iParamSize, *(BYTE *)_pParamValue));
			}
			break;
		case eSMALLINT:		// SmallInt
			{
				m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
						m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adSmallInt, adParamInput, _iParamSize, *(short *)_pParamValue));
			}
			break;
		case eINT:			// Int
			{
				m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
						m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adInteger, adParamInput, _iParamSize, *(int *)_pParamValue));
			}
			break;
		case eBIGINT:		// BigInt
			{
				VARIANT temp;
				temp.vt = VT_I8;
				temp.llVal = *(LONGLONG *)_pParamValue;				
				m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
						m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adBigInt, adParamInput, _iParamSize, &temp));
				VariantClear(&temp);
			}
			break;	
		case eCHAR:			// Char
			{
				_iParamSize = (_iParamSize == 0 ? 1 : _iParamSize);		// "" 빈 데이타가 들어오면 _iParamSize 이 0이된다, 그러면 에러가 나서 데이타가 없을경우 _iParamSize = 1
				m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
						m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adChar, adParamInput, _iParamSize, (char *)_pParamValue));
			}
			break;
		case eNCHAR:		// NChar
			{
				_iParamSize = (_iParamSize == 0 ? 1 : _iParamSize);		// "" 빈 데이타가 들어오면 _iParamSize 이 0이된다, 그러면 에러가 나서 데이타가 없을경우 _iParamSize = 1
				m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
						m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adWChar, adParamInput, _iParamSize, (TCHAR *)_pParamValue));
			}
			break;
		case eVARCHAR:			// VarChar
			{
				_iParamSize = (_iParamSize == 0 ? 1 : _iParamSize);		// "" 빈 데이타가 들어오면 _iParamSize 이 0이된다, 그러면 에러가 나서 데이타가 없을경우 _iParamSize = 1
				m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
						m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adVarChar, adParamInput, _iParamSize, (char *)_pParamValue));
			}
			break;
		case eNVARCHAR:			// NVarChar
			{
				_iParamSize = (_iParamSize == 0 ? 1 : _iParamSize);		// "" 빈 데이타가 들어오면 _iParamSize 이 0이된다, 그러면 에러가 나서 데이타가 없을경우 _iParamSize = 1
				m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
						m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adVarWChar, adParamInput, _iParamSize, (TCHAR *)_pParamValue));
			}
			break;

		case eDATETIME:			// DateTime
			{
				_iParamSize = (_iParamSize == 0 ? 1 : _iParamSize);		// "" 빈 데이타가 들어오면 _iParamSize 이 0이된다, 그러면 에러가 나서 데이타가 없을경우 _iParamSize = 1
				m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
						m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adDBTimeStamp, adParamInput, _iParamSize, (TCHAR *)_pParamValue));
			}
			break;
		case eBINARY:			// Binary, Ascii 로 처리
			{
				// 바이너리 데이타로 변환
				if(_iParamSize == 0){		// 사이즈가 0 일때는 그냥 처리하면 에러가 나서 1바이트의 더미데이타를 넣는다 (나중에 getBinary() 에서 문제 생길수 있다, 원인을 찾아서 수정해야한다)
					BYTE ucDummyData = 0;
					_variant_t bin;	
					VariantInit(&bin);		
					blobToVariant(bin, &ucDummyData, 1);					
					m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
							m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adBinary, adParamInput, 1, bin));
					VariantClear(&bin);		// 바이너리 데이타 정리
				}else{
					_variant_t bin;	
					VariantInit(&bin);		
					blobToVariant(bin, _pParamValue, _iParamSize);					
					m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
							m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adBinary, adParamInput, _iParamSize, bin));
					VariantClear(&bin);		// 바이너리 데이타 정리
				}					
			}
			break;
		case eVARBINARY:		// VarBinary, Ascii 로 처리
			{
				// 바이너리 데이타로 변환
				if(_iParamSize == 0){		// 사이즈가 0 일때는 그냥 처리하면 에러가 나서 1바이트의 더미데이타를 넣는다 (나중에 getBinary() 에서 문제 생길수 있다, 원인을 찾아서 수정해야한다)
					BYTE ucDummyData = 0;
					_variant_t bin;	
					VariantInit(&bin);		
					blobToVariant(bin, &ucDummyData, 1);					
					m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
							m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adVarBinary, adParamInput, 1, bin));
					VariantClear(&bin);		// 바이너리 데이타 정리
				}else{
					_variant_t bin;	
					VariantInit(&bin);		
					blobToVariant(bin, _pParamValue, _iParamSize);					
					m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
							m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adVarBinary, adParamInput, _iParamSize, bin));
					VariantClear(&bin);		// 바이너리 데이타 정리
				}				
			}
			break;
	}

	// 모든 파라미터 관리
	_tcscpy_s(m_pstAdoDBData[_iDBIdx].aszParamName[m_pstAdoDBData[_iDBIdx].iAllParamIdx], 
			_countof(m_pstAdoDBData[_iDBIdx].aszParamName[m_pstAdoDBData[_iDBIdx].iAllParamIdx]), _pszParamName);
	++m_pstAdoDBData[_iDBIdx].iAllParamIdx;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 현재 Int 관련 타입만 지원
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::setProcedureOutputParam(int _iDBIdx, const TCHAR *_pszParamName, int _iDataType, void *_pParamValue)
{
	switch(_iDataType)
	{
		case eTINYINT:		// TinyInt
			m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
							m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adTinyInt, adParamOutput, sizeof(BYTE)));
			break;
		case eSMALLINT:		// SmallInt
			m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
							m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adSmallInt, adParamOutput, sizeof(short)));
			break;
		case eINT:			// Int
			m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
							m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adInteger, adParamOutput, sizeof(int)));
			break;
		case eBIGINT:		// BigInt
			m_pstAdoDBData[_iDBIdx].pCommand->Parameters->Append(
							m_pstAdoDBData[_iDBIdx].pCommand->CreateParameter(_bstr_t(_pszParamName), adBigInt, adParamOutput, sizeof(LONGLONG)));
			break;	
	}

	// Output 파라미터 관리
	_tcscpy_s(m_pstAdoDBData[_iDBIdx].astOutputParam[m_pstAdoDBData[_iDBIdx].iOutputParamIdx].szParamName, 
			_countof(m_pstAdoDBData[_iDBIdx].astOutputParam[m_pstAdoDBData[_iDBIdx].iOutputParamIdx].szParamName), 
			_pszParamName);
	m_pstAdoDBData[_iDBIdx].astOutputParam[m_pstAdoDBData[_iDBIdx].iOutputParamIdx].pValue = _pParamValue;
	m_pstAdoDBData[_iDBIdx].astOutputParam[m_pstAdoDBData[_iDBIdx].iOutputParamIdx].iDataType = _iDataType;
	++m_pstAdoDBData[_iDBIdx].iOutputParamIdx;

	// 모든 파라미터 관리
	_tcscpy_s(m_pstAdoDBData[_iDBIdx].aszParamName[m_pstAdoDBData[_iDBIdx].iAllParamIdx], 
			_countof(m_pstAdoDBData[_iDBIdx].aszParamName[m_pstAdoDBData[_iDBIdx].iAllParamIdx]), _pszParamName);
	++m_pstAdoDBData[_iDBIdx].iAllParamIdx;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : SP 호출후 output 파라미터에 값넣어주기
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::setOutputParamData(int _iDBIdx, int _iDataType, TCHAR *_pszParamName, void *_pParamValue)
{
	switch(_iDataType)
	{
		case eTINYINT:		// TinyInt
			if(m_pstAdoDBData[_iDBIdx].pCommand->GetParameters()->GetItem(_pszParamName)->Value.vt == VT_NULL){
				*((BYTE *)_pParamValue) = 0;
			}else{
				*((BYTE *)_pParamValue) = m_pstAdoDBData[_iDBIdx].pCommand->GetParameters()->GetItem(_pszParamName)->Value;
			}
			break;
		case eSMALLINT:		// SmallInt
			if(m_pstAdoDBData[_iDBIdx].pCommand->GetParameters()->GetItem(_pszParamName)->Value.vt == VT_NULL){
				*((short *)_pParamValue) = 0;
			}else{
				*((short *)_pParamValue) = m_pstAdoDBData[_iDBIdx].pCommand->GetParameters()->GetItem(_pszParamName)->Value;
			}
			break;
		case eINT:			// Int
			if(m_pstAdoDBData[_iDBIdx].pCommand->GetParameters()->GetItem(_pszParamName)->Value.vt == VT_NULL){
				*((int *)_pParamValue) = 0;
			}else{
				*((int *)_pParamValue) = m_pstAdoDBData[_iDBIdx].pCommand->GetParameters()->GetItem(_pszParamName)->Value;
			}
			break;
		case eBIGINT:		// BigInt
			if(m_pstAdoDBData[_iDBIdx].pCommand->GetParameters()->GetItem(_pszParamName)->Value.vt == VT_NULL){
				*((LONGLONG *)_pParamValue) = 0;
			}else{
				VARIANT temp;
				temp = m_pstAdoDBData[_iDBIdx].pCommand->GetParameters()->GetItem(_pszParamName)->Value;
				temp.vt = VT_I8;
				*((LONGLONG *)_pParamValue) = temp.llVal;
				VariantClear(&temp);
			}
			break;	
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : select 되는 SP 가 아니고 output 을 받는 SP일때
//        m_pstAdoDBData[_iDBIdx].pRecordset = m_pstAdoDBData[_iDBIdx].pCommand->Execute(NULL, NULL, adCmdStoredProc | adCmdUnspecified);
//        이런식으로 처리하면 output 값이 0으로 초기화가 된다
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cAdoDB::callProcedure(int _iDBIdx, TCHAR *_pszProName, BOOL bSelectField, TCHAR *_pszFunction, int _iLine)
{
	TCHAR szTemp[512];	
	if(m_pstAdoDBData[_iDBIdx].iAllParamIdx){	// 모든 파라미터가 바인딩되었으므로 이값이 총 파라미터 값이다
		_stprintf_s(szTemp, _countof(szTemp), _T("{CALL %s("), _pszProName);
		for(int i=0; i<m_pstAdoDBData[_iDBIdx].iAllParamIdx; i++){			
			_tcscat_s(szTemp, _countof(szTemp), _T("?,"));
		}
		szTemp[_tcslen(szTemp) - 1] = NULL;
		_tcscat_s(szTemp, _countof(szTemp), _T(")}"));		
	}else{
		_stprintf_s(szTemp, _countof(szTemp), _T("{CALL %s}"), _pszProName);		
	}

	m_pstAdoDBData[_iDBIdx].pCommand->CommandText = szTemp;		
	m_pstAdoDBData[_iDBIdx].pCommand->CommandType = adCmdText;	

	try{
		if(bSelectField){
			m_pstAdoDBData[_iDBIdx].pRecordset = m_pstAdoDBData[_iDBIdx].pCommand->Execute(NULL, NULL, adCmdStoredProc | adCmdUnspecified);					
		}else{
			m_pstAdoDBData[_iDBIdx].pCommand->Execute(NULL, NULL, adCmdStoredProc | adCmdUnspecified);			
		}
		if(FAILED(m_pstAdoDBData[_iDBIdx].pRecordset)){
			DWORD hr = GetLastError();
			_com_error e(hr);
			errorHandling(_iDBIdx, e, _pszProName, _pszFunction, _iLine);
			return FALSE;
		}
	}catch(_com_error &e){
		errorHandling(_iDBIdx, e, _pszProName, _pszFunction, _iLine);
		return FALSE;
	}

	// Output 파라미터 셋팅
	if(m_pstAdoDBData[_iDBIdx].iOutputParamIdx){		// output 파라미터가 바인딩되었으므로 이값이 총 output 파라미터 값이다
		for(int i=0; i<m_pstAdoDBData[_iDBIdx].iOutputParamIdx; i++){
			setOutputParamData(_iDBIdx, 
					m_pstAdoDBData[_iDBIdx].astOutputParam[i].iDataType, 
					m_pstAdoDBData[_iDBIdx].astOutputParam[i].szParamName,
					m_pstAdoDBData[_iDBIdx].astOutputParam[i].pValue);			
		}
	}
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::strToTime(TCHAR *_pszStr, SYSTEMTIME *_st)
{
	memset(_st, 0, sizeof(SYSTEMTIME));

	TCHAR szYear[8] = {0, };
	TCHAR szMonth[4] = {0, };
	TCHAR szDay[4] = {0, };
	TCHAR szHour[4] = {0, };
	TCHAR szMin[4] = {0, };
	TCHAR szSec[4] = {0, };

	_tcsncpy_s(szYear, _countof(szYear), _pszStr, 4);
	_tcsncpy_s(szMonth, _countof(szMonth), _pszStr + 5, 2);
	_tcsncpy_s(szDay, _countof(szDay), _pszStr + 8, 2);

	if(_tcslen(_pszStr) == 10){				// "2004-01-03"
		_tcscpy_s(szMin, _countof(szMin), _T("00"));
		_tcscpy_s(szSec, _countof(szSec), _T("00"));
		_st->wHour = 0;
	}else if(_tcslen(_pszStr) == 19){		// "2004-01-03 13:12:22"
		_tcsncpy_s(szHour, _countof(szHour), _pszStr + 11, 2);
		_tcsncpy_s(szMin, _countof(szMin), _pszStr + 14, 2);
		_tcsncpy_s(szSec, _countof(szSec), _pszStr + 17, 2);
		_st->wHour = _ttoi(szHour);
	}else{									// "2004-01-03 오전 1:12:22" or  "2004-01-03 오전 11:12:22"
		TCHAR szType[4];
		int iAdd;
		_tcsncpy_s(szType, _countof(szType), _pszStr + 11, 2);
		if(_tcscmp(szType, _T("오전")) == 0  ||  (_tcscmp(szType, _T("AM")) == 0  ||  _tcscmp(szType, _T("am")) == 0)){
			iAdd = 0;
		}else if(_tcscmp(szType, _T("오후")) == 0  ||  (_tcscmp(szType, _T("PM")) == 0  ||  _tcscmp(szType, _T("pm")) == 0)){
			iAdd = 12;
		}else{
			memset(_st, 0, sizeof(SYSTEMTIME));
			return;
		}

		szHour[0] = _pszStr[14];
		if(_pszStr[15] == ':'){
			szHour[1] = NULL;
			_tcsncpy_s(szMin, _countof(szDay), _pszStr + 16, 2);
			_tcsncpy_s(szSec, _countof(szDay), _pszStr + 19, 2);
		}else{
			szHour[1] = _pszStr[15];
			szHour[2] = NULL;
			_tcsncpy_s(szMin, _countof(szDay), _pszStr + 17, 2);
			_tcsncpy_s(szSec, _countof(szDay), _pszStr + 20, 2);
		}
		_st->wHour = _ttoi(szHour);
		if(_st->wHour == 12){
			if(iAdd == 12){		// 오후 12 시 (변화없음)

			}else{				// 오전 12 시 (0)
				_st->wHour = 0;
			}				
		}else{
			_st->wHour += iAdd;			
		}
	}

	_st->wYear = _ttoi(szYear);
	_st->wMonth = _ttoi(szMonth);
	_st->wDay = _ttoi(szDay);	
	_st->wMinute = _ttoi(szMin);
	_st->wSecond = _ttoi(szSec);	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc :	data -> blob (사용방법 =>)
//			VARIANT bin;	VariantInit(&bin);		
//			BlobToVariant(bin, _pstData, sizeof(stAVATARINFO));							//압축할 데이터를 VARIANT로 구겨넣고
//			pCmd->Parameters->Item["@BINARYINFO"]->AppendChunk(bin);					//데이터를 Command객체에 첨부하여
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cAdoDB::blobToVariant(VARIANT &varChunk, const void *pData, UINT nBytes)	
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
		psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
		if(psa != NULL)
		{
			while(lngOffset < (long)nBytes)
			{
				chData	= ((UCHAR *)pData)[lngOffset];
				hr = SafeArrayPutElement(psa, &lngOffset, &chData);

				lngOffset++;
			}
			lngOffset = 0;

			//Assign the Safe array  to a variant.
			VariantInit(&varChunk);
			varChunk.vt = VT_ARRAY | VT_UI1;
			varChunk.parray = psa;
		}else{
			//m_cDBLog.writeLog(_T("ERROR BlobToVariant : psa = NULL"));			
		}
	}catch(_com_error &e){
		errorHandling(0, e, _T("blobToVariant()"), _T("blobToVariant"), __LINE__);
	}
	return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc :	blob -> data (사용방법 =>)
//			_variant_t bin;		
//			VariantInit(&bin);	
//			bin = pRs->Fields->Item["BinaryInfo"]->Value;									
//			int unsize = VariantToBlob(bin, pUncompressdata);				//임시버퍼에 DB에서 받은 바이러리를 컨버팅하고
//			memcpy(&_pstData[*_piAvatarCnt], pUncompressdata, unsize);		//원래 넣으려던 데이터에 넣고
///////////////////////////////////////////////////////////////////////////////////////////////////////////
int	cAdoDB::variantToBlob(VARIANT & varChunk,void * pData)
{
	if(varChunk.vt==VT_NULL)	return FALSE;

	HRESULT	hr;
	UCHAR chData;
	long lBytesCopied = 0;
	long length;
	length = varChunk.parray->rgsabound->cElements;	
	
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
