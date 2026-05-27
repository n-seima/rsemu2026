//	AdoDB.h: interface for the IDataBase class.
//
//	2007-08-10 Kwon Yong Jae (amadeusj@gmail.com)
//	AdoDB 연결 (MS-SQL 만 적용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#import "c:\Program Files\Common Files\System\ADO\msado20.tlb" no_namespace rename("EOF", "adoEOF") rename("BOF", "adoBOF")		// Link 해당파일을 가져와서 사용


class cAdoDB
{
private:
	typedef enum _enumBASETYPE
	{
		//eCONNECTDBPORT	= 7963,						// MS_SQL 서버 포트 (Default: 1433) 		
		eMAXOUTPUTPARAMCNT	= 20,						// output 파라미터 최대수
		eMAXALLPARAMCNT		= eMAXOUTPUTPARAMCNT + 20,	// 모든 파라미터 최대수
		ePARAMTEXTSIZE		= 40,						// 파라미터 텍스트 사이즈
	}enumBASETYPE;

	// DB 로그인 정보, 접속이 끊겼을경우 재접속을위해
	typedef struct _stLOGININFO
	{
		TCHAR szDBName[80];
		TCHAR szID[32];
		TCHAR szPW[32];
		TCHAR szIP[20];	
	}stLOGININFO;

	// Output 파라미터 정보
	typedef struct _stOUTPUTPARAM
	{
		int iDataType;
		TCHAR szParamName[40];
		void *pValue;
	}stOUTPUTPARAM;

	// AdoDB 에 사용되는 데이타
	typedef struct _stADODBDATA
	{
		_CommandPtr pCommand;
		_ConnectionPtr pConnection;		
		_RecordsetPtr pRecordset;
		int iOutputParamIdx;		// 현재 적용되고 있는 Output 파라미터 인덱스
		int iAllParamIdx;			// 현재 적용되고 있는 모든 파라미터 인덱스
		TCHAR aszParamName[eMAXALLPARAMCNT][ePARAMTEXTSIZE];			// 모든 파라미터는 SP 를 호출후에 지워주어야한다
		stOUTPUTPARAM astOutputParam[eMAXOUTPUTPARAMCNT];
	}stADODBDATA;

	stADODBDATA *m_pstAdoDBData;	// Connect 를 여러개를 한다
	int m_iDBConnectCnt;			// DB 연결수
	stLOGININFO m_stLoginInfo;		// 접속이 끊길경우 재접속을 위해	
	CRITICAL_SECTION m_csDBIdx;		// 놀고 있는 Connect 를 구할때 사용	
	BOOL *m_pbRestDBConnect;		// DB Connect 가 휴식중인지 체크

public:
	typedef enum _enumERRORTYPE
	{
		eERRORRESTDBCONNECT	= -1,	// 놀고 있는 디비접속이 없을때		
	}enumERRORTYPE;

	typedef enum _enumDATATYPE
	{
		eTINYINT	= 0,			// 1Byte (0 ~ 255)
		eSMALLINT,					// 2Byte (-2^15 ~ 2^15-1)
		eINT,						// 4Byte
		eBIGINT,					// 8Byte
		eCHAR,						// char
		eNCHAR,						// nchar
		eVARCHAR,					// varchar
		eNVARCHAR,					// nvarchar
		eDATETIME,					// datetime, smalldatetime
		eBINARY, 					// binary
		eVARBINARY, 				// varbinary
	}enumDATATYPE;

private:
	void strToTime(TCHAR *_pstStr, SYSTEMTIME *_st);
	BOOL reConnect(int _iDBIdx);
	void errorHandling(int _iDBIdx, _com_error &e, TCHAR *_pszSQL, TCHAR *_pszFunction, int _iLine);
	void setOutputParamData(int _iDBIdx, int _iDataType, TCHAR *_pszParamName, void *_pParamValue);
	void blobToVariant(VARIANT &varChunk, const void *pData, UINT nBytes);	//	data -> blob
	int variantToBlob(VARIANT &varChunk, void *pData);						//	blob -> data	

	inline void setRestDBConnect(int _iDBIdx, BOOL _bFlag)
	{
		EnterCriticalSection(&m_csDBIdx);
		m_pbRestDBConnect[_iDBIdx] = _bFlag;
		LeaveCriticalSection(&m_csDBIdx);
	}
	
public:
	cAdoDB(void);
	~cAdoDB(void);
	BOOL connect(TCHAR *_pszIP, int _iPort, TCHAR *_pszDBName, TCHAR *_pszID, TCHAR *_pszPW, int _iDBConnectCnt);
	void disConnect();
	BOOL execSql(int _iDBIdx, TCHAR *_pszSQL, TCHAR *_pszFunction, int _iLine);
	BYTE getTinyInt(int _iDBIdx, TCHAR *_pszCol);								// 1Byte 정수형 컬럼 읽기	
	short getSmallInt(int _iDBIdx, TCHAR *_pszCol);								// 2Byte 정수형 컬럼 읽기	
	int getInt(int _iDBIdx, TCHAR *_pszCol);									// 4Byte 정수형 컬럼 읽기	
	LONGLONG getBigInt(int _iDBIdx, TCHAR *_pszCol);							// 8Byte 정수형 컬럼 읽기	
	void getStr(int _iDBIdx, TCHAR *_pszCol, char *_pszBuf, int _iBufSize);		// 문자열형 컬럼 읽기	 (char)
	void getVarStr(int _iDBIdx, TCHAR *_pszCol, char *_pszBuf, int _iBufSize);	// 문자열형 컬럼 읽기	 (varchar)
	void getNStr(int _iDBIdx, TCHAR *_pszCol, TCHAR *_pszBuf, int _iBufSize);	// 문자열형 컬럼 읽기	 (nchar)
	void getNVarStr(int _iDBIdx, TCHAR *_pszCol, TCHAR *_pszBuf, int _iBufSize);// 문자열형 컬럼 읽기	 (nvarchar)
	void getDate(int _iDBIdx, TCHAR *_pszCol, SYSTEMTIME *_psystimeDate);		// 데이트형 컬럼 읽기
	void getBinary(int _iDBIdx, TCHAR *_pszCol, void *_pszBuf, int _iBufSize);	// 바이너리형 컬럼 읽기 (Binary)	
	void getVarBinary(int _iDBIdx, TCHAR *_pszCol, void *_pszBuf, int _iBufSize);// 바이너리형 컬럼 읽기 (VarBinary)	
	void clearData(int _iDBIdx);												// 커서 해제 및 언바인딩
	BOOL getData(int _iDBIdx);													// 데이타 가져오기
	void moveNextData(int _iDBIdx);												// 다음 레코드로 이동	
	BOOL callProcedure(int _iDBIdx, TCHAR *_pszProName, BOOL bSelectField, TCHAR *_pszFunction, int _iLine);
	void setProcedureInputParam(int _iDBIdx, const TCHAR *_pszParamName, int _iDataType, const void *_pParamValue, int _iParamSize);	// 프로시져 INPUT PARAM
	void setProcedureOutputParam(int _iDBIdx, const TCHAR *_pszParamName, int _iDataType, void *_pParamValue);							// 프로시져 OUTPUT PARAM

	inline int getRestDBConnect()
	{
		EnterCriticalSection(&m_csDBIdx);
		for(int i=0; i<m_iDBConnectCnt; i++){
			if(m_pbRestDBConnect[i]){
				m_pbRestDBConnect[i] = FALSE;
				LeaveCriticalSection(&m_csDBIdx);
				return i;
			}
		}
		LeaveCriticalSection(&m_csDBIdx);

		// 놀고 있는 디비접속이 없을때
		int iErrorCnt = 0;
		while(1){
			if(iErrorCnt++ == 500){			// 무한루프 방지
				return eERRORRESTDBCONNECT;	// Error
			}
			Sleep(10);

			EnterCriticalSection(&m_csDBIdx);
			for(int i=0; i<m_iDBConnectCnt; i++){
				if(m_pbRestDBConnect[i]){
					m_pbRestDBConnect[i] = FALSE;
					LeaveCriticalSection(&m_csDBIdx);
					return i;
				}
			}
			LeaveCriticalSection(&m_csDBIdx);
		}
	}
};




