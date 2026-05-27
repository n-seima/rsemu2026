#include "StdAfx.h"
#include ".\commonlib.h"
#include <process.h>
#include <shellapi.h>
#include <shlwapi.h>
#pragma comment(lib, "winmm.lib ")
#pragma comment(lib, "shlwapi")



// ===============================================================================================================================
// ========================================================= cConsoleOption ======================================================
// ===============================================================================================================================

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
HANDLE cConsoleOption::m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);     	


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 콘솔이벤트 처리
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI ____ConsoleHandler(DWORD _dwEvent)
{
	char szPrint[512];
	szPrint[0] = NULL;
	switch(_dwEvent)
	{
		case CTRL_C_EVENT:
			strcpy_s(szPrint, sizeof(szPrint), "\nCTRL_C_EVENT\n");
			break;
		case CTRL_BREAK_EVENT:
			strcpy_s(szPrint, sizeof(szPrint), "\nCTRL_BREAK_EVENT\n");
			break;
		case CTRL_CLOSE_EVENT:
			strcpy_s(szPrint, sizeof(szPrint), "\nCTRL_CLOSE_EVENT\n");
			break;
		case CTRL_LOGOFF_EVENT:
			strcpy_s(szPrint, sizeof(szPrint), "\nCTRL_LOGOFF_EVENT\n");
			break;
		case CTRL_SHUTDOWN_EVENT:
			strcpy_s(szPrint, sizeof(szPrint), "\nCTRL_SHUTDOWN_EVENT\n");
			break;
	}
	printf(szPrint);	
	return TRUE;
}

	
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
cConsoleOption::cConsoleOption()
{
	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
cConsoleOption::~cConsoleOption()
{
	CloseHandle(m_hConsole);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cConsoleOption::disableCloseBtn()
{
	TCHAR szConsoleTitle[256];	
	getConsoleTitle(szConsoleTitle, sizeof(szConsoleTitle));		
	HWND hWnd = FindWindow(_T("ConsoleWindowClass"), szConsoleTitle);
	HMENU hMenu = GetSystemMenu(hWnd,0);
	if(hMenu != NULL)
	{
		DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
		DrawMenuBar(hWnd);
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)____ConsoleHandler, TRUE);		// 콘솔이벤트 처리
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cConsoleOption::setConsoleTitle(TCHAR *_pszTitle)
{
	SetConsoleTitle(_pszTitle);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cConsoleOption::getConsoleTitle(TCHAR *_pszTitle, int _iSize)
{
	GetConsoleTitle(_pszTitle, _iSize);
}	





// ===============================================================================================================================
// ============================================================ cUnicode =========================================================
// ===============================================================================================================================

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cUnicode::m_bUseCriticalSection;
CRITICAL_SECTION cUnicode::m_CS;
TCHAR cUnicode::m_szPrint[2048] = {0, };




// ===============================================================================================================================
// ====================================================== cFileManager ===========================================================
// ===============================================================================================================================


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
cFileManager::cFileManager(void)
{
	m_hFile = NULL;

	m_szFileFullName[0] = NULL;
	m_szFileName[0] = NULL;
	m_szDirectory[0] = NULL;
	m_szFormat[0] = NULL;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
cFileManager::~cFileManager(void)
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 파일을 연다
// 
//        _iDesiredAccess ==>
//        GENERIC_READ
//        GENERIC_WRITE
//
//        _CreationDisposition ==>
//        CREATE_NEW,		// 새로운 파일을 만들되 만약 지정한 파일이 이미 있으면 만들지 않는다
//        CREATE_ALWAYS		// 새로운 파일을 만들되 만약 지정한 파일이 이미 있으면 기존 파일을 삭제하고 다시 만든다, 이떄 파일의 속성은 다시 설정된다 
//        OPEN_EXISTING		// 이미 있는 파일을 열되 파일이 없으면 에러코드를 리턴한다
//        OPEN_ALWAYS		// 파일을 열되 만약없으면 새로만든다
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFileManager::createFile(const TCHAR *_pszFileName, int _iDesiredAccess, int _CreationDisposition)
{
	if(m_hFile){
		return FALSE;
	}

	if(!_iDesiredAccess  ||  !_CreationDisposition){
		return FALSE;
	}

	TCHAR szDrive[4];
	TCHAR szDir[512];
	_tsplitpath_s(_pszFileName, szDrive, 4, szDir, 512, m_szFileName, sizeof(m_szFileName) / sizeof(TCHAR), m_szFormat, sizeof(m_szFormat) / sizeof(TCHAR));
	_tcsncpy_s(m_szFileFullName, sizeof(m_szFileFullName) / sizeof(TCHAR), _pszFileName, sizeof(m_szFileFullName) / sizeof(TCHAR) - 1);
	_stprintf_s(m_szDirectory, sizeof(m_szDirectory) / sizeof(TCHAR), _T("%s%s"), szDrive, szDir);

	m_hFile = CreateFile(_pszFileName, _iDesiredAccess, FILE_SHARE_READ, 0, _CreationDisposition, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, 0);		// FILE_FLAG_WRITE_THROUGH => fflush(), FILE_SHARE_READ => 파일을 열어서 확인할수 있다(3번째 파라미터)
	if(m_hFile == INVALID_HANDLE_VALUE){
		m_hFile = NULL;
		return FALSE;
	}
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cFileManager::closeFile()
{
	if(m_hFile){
		CloseHandle(m_hFile);
		m_hFile = NULL;

		m_szFileFullName[0] = NULL;
		m_szFileName[0] = NULL;
		m_szDirectory[0] = NULL;
		m_szFormat[0] = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD cFileManager::writeFile(const void *_pData, int _iDataSize)
{
	if(!m_hFile){
		return -1;
	}

	DWORD dwLen;
	WriteFile(m_hFile, _pData, _iDataSize, &dwLen, NULL);
	return dwLen;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD cFileManager::readFile(void *_pBuffer, int _iBufferSize)
{
	if(!m_hFile){
		return -1;
	}

	DWORD dwLen;
	ReadFile(m_hFile, _pBuffer, _iBufferSize, &dwLen, NULL);		
	return dwLen;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : Ascii => Unicode
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD cFileManager::readFile(char *_pTempBuffer, int _iTempBufferSize, TCHAR *_pBuffer, int _iBufferSize)
{
	if(!m_hFile){
		return -1;
	}

	DWORD dwLen = readFile(_pTempBuffer, _iTempBufferSize);
	if(cUnicode::T2W2(_pTempBuffer, _pBuffer, _iBufferSize) == 0){
		return -1;
	}
	return dwLen;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : ReadFile() 로 파일을 읽은후 \r\n 을 찾아서 다시 파일포인터를 복귀시킨다
//        fgets(), ifstream::getline() 로 처리할수 있지만 cFileManager 에서는 HANDLE 을 사용했으므로 
//        약간 복잡하더라도 해당방법으로 처리한다
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD cFileManager::readFileLine(void *_pBuffer, int _iBufferSize)
{
	if(!m_hFile){
		return -1;
	}

	DWORD dwLen;
	ReadFile(m_hFile, _pBuffer, _iBufferSize - 1, &dwLen, NULL);
	char *pBuffer = (char *)_pBuffer;
	pBuffer[dwLen] = NULL;

	char *p = strstr(pBuffer, "\r\n");		// 문자열로 검색
	if(!p){									// 찾을수 없다 (마지막 데이타이다)
		;
	}else{
		int iResult = (int)(p - pBuffer);	// 몇번째 값인지 구해준다, _pBuffer + iResult = p
		pBuffer[iResult] = NULL;

		// 파일의 포인터를 앞으로 당긴다
		int iRe = dwLen - (iResult + 2);	// "\r\n"
		SetFilePointer(m_hFile, -iRe, NULL, FILE_CURRENT);
	}
	
	return dwLen;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : Ascii => Unicode
//        ReadFile() 로 파일을 읽은후 \r\n 을 찾아서 다시 파일포인터를 복귀시킨다
//        fgets(), ifstream::getline() 로 처리할수 있지만 cFileManager 에서는 HANDLE 을 사용했으므로 
//        약간 복잡하더라도 해당방법으로 처리한다
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD cFileManager::readFileLine(char *_pTempBuffer, int _iTempBufferSize, TCHAR *_pBuffer, int _iBufferSize)
{
	if(!m_hFile){
		return -1;
	}

	DWORD dwLen = readFileLine(_pTempBuffer, _iTempBufferSize);
	if(cUnicode::T2W2(_pTempBuffer, _pBuffer, _iBufferSize) == 0){
		return -1;
	}
	
	return dwLen;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 파일포인터 이동, 파일을 열면 파일의 포인터가 맨앞이다
//
//        _dwMoveMethod ==>
//        FILE_BEGIN
//        FILE_END
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFileManager::moveFilePoint(DWORD _dwMoveMethod)
{
	if(!m_hFile){
		return FALSE;
	}
	SetFilePointer(m_hFile, 0, NULL, _dwMoveMethod);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD cFileManager::getFileSize()
{
	if(!m_hFile){
		return -1;
	}
	DWORD dwHighSize;
	return GetFileSize(m_hFile, &dwHighSize);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD cFileManager::getFileSize(const TCHAR *_pszFileName)
{
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
	BOOL succeeded = GetFileAttributesEx(_pszFileName, GetFileExInfoStandard, &fileInfo);
	if(!succeeded){
		return 0;
	}
	return fileInfo.nFileSizeLow;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 라인수구하기
//        한라인의 데이타가 4096 바이트가 넘으면 문제가 생긴다
///////////////////////////////////////////////////////////////////////////////////////////////////////////
int cFileManager::getFileLineCnt(const TCHAR *_pszFileName)
{
	FILE *pFile;
	if(_tfopen_s(&pFile, _pszFileName, _T("rb"))){		// _tfopen_s() 로 파일을 열면 닫을때까지 그파일에 접근할수 없다(열어 볼수도 없다)
		return -1;
	}
	int iCnt = 0;
	char szTemp[4096];
	BOOL bLast = FALSE;
	while(fgets(szTemp, sizeof(szTemp), pFile)){
		// 혹시 버퍼가 모잘라서 한줄의 데이타를 못읽는 것을 방지하기 위해
		if(bLast){
			return -1;
		}
		if(strncmp(szTemp + strlen(szTemp) - 2, "\r\n", 2) != 0){
			bLast = TRUE;
		}
		iCnt++;
	}
	fclose(pFile);
	return iCnt;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 파일명을 확인한다, 만일 같은 파일명이 존재하면 파일명을 변경한다 ("Test(1).exe")
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cFileManager::getFileNameExist(const TCHAR *_pszFileName, TCHAR *_pszNewFileName, int _iNewFileNameSize)
{
	HANDLE hFile = CreateFile(_pszFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, 0);
	if(hFile == INVALID_HANDLE_VALUE){		// 해당 파일이 없다
		_tcscpy_s(_pszNewFileName, _iNewFileNameSize, _pszFileName);
	}else{									// 해당파일이 있다
		CloseHandle(hFile);

		TCHAR szDrive[4];
		TCHAR szDir[512];
		TCHAR szFileName[256];
		TCHAR szExt[40];
		_tsplitpath_s(_pszFileName, szDrive, 4, szDir, 512, szFileName, 256, szExt, 40);
		TCHAR szTempNewFile[512];
		int iNum = 1;
		while(1){
			_stprintf_s(szTempNewFile, 512, _T("%s%s%s(%d)%s"), szDrive, szDir, szFileName, iNum, szExt);
			hFile = CreateFile(szTempNewFile, GENERIC_READ, FILE_SHARE_READ , 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, 0);
			if(hFile == INVALID_HANDLE_VALUE){		// 해당 파일이 없다
				_tcscpy_s(_pszNewFileName, _iNewFileNameSize, szTempNewFile);
				break;
			}else{									// 해당파일이 있다
				CloseHandle(hFile);
			}
			++iNum;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 폴더전체 작업처리
//
//        _iOperationType ==>
//        FO_MOVE
//        FO_COPY 
//        FO_DELETE
//        FO_RENAME 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFileManager::operationFile(int _iOperationType, const TCHAR *_pszSourceFileName, const TCHAR *_pszDestFileName)
{
	SHFILEOPSTRUCT FileOp = {0};
	FileOp.hwnd = NULL;
	FileOp.wFunc = _iOperationType;
	FileOp.pFrom = NULL;
	FileOp.pTo = NULL;
	FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_ALLOWUNDO;		// FOF_NOCONFIRMATION | FOF_NOERRORUI 확인메시지가 안뜨게, FOF_ALLOWUNDO 삭제시 휴지통으로
	FileOp.fAnyOperationsAborted = false;
	FileOp.hNameMappings = NULL;
	FileOp.lpszProgressTitle = NULL;

	TCHAR szSourceFileName[512];	
	if(szSourceFileName){
		_tcsncpy_s(szSourceFileName, 512, _pszSourceFileName, 512 - 2);
		szSourceFileName[_tcslen(szSourceFileName) + 1] = NULL;	// 문자열 뒤에 NULL 이 두번 들어가야 한다
	}else{
		szSourceFileName[0] = NULL;
		szSourceFileName[1] = NULL;
	}

	TCHAR szDestFileName[512];
	if(_pszDestFileName){
		_tcsncpy_s(szDestFileName, 512, _pszDestFileName, 512 - 2);
		szDestFileName[_tcslen(szDestFileName) + 1] = NULL;		// 문자열 뒤에 NULL 이 두번 들어가야 한다
	}else{
		szDestFileName[0] = NULL;
		szDestFileName[1] = NULL;
	}	

	FileOp.pFrom = szSourceFileName;
	FileOp.pTo = szDestFileName;
	BOOL bResult;
	if(SHFileOperation(&FileOp)){							// 실패
		bResult = 0;
	}else{
		bResult = 1;
	}		
	return bResult;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 폴더 생성하기
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFileManager::createDirectory(const TCHAR *_pszDirectory)
{
	wstring strPath = _pszDirectory;
	if(PathFileExists(strPath.c_str()) != 0){
		return TRUE;
	}else{
		int i=0;
		while(1){
			i = (int)strPath.find(_T("\\"), i + 1);
			if(i <= 0){
				CreateDirectory(strPath.c_str(), 0);
				break;
			}
			wstring strTemp = strPath.substr(0, i + 1).c_str();
			CreateDirectory(strTemp.c_str(), 0);
		}
	}

	if(PathFileExists(strPath.c_str()) != 0){
		return TRUE;
	}else{
		return FALSE;
	}
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 파일 존재 체크
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFileManager::isFileExist(const TCHAR *_pszFileName)
{
	DWORD attr = GetFileAttributes(_pszFileName);
	return (attr != INVALID_FILE_ATTRIBUTES) && ((attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 디렉터리 존재 체크
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFileManager::isDirectoryExist(const TCHAR *_pszPath)
{
	DWORD attr = GetFileAttributes(_pszPath);
	return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 폴더내의 모든 파일 검색 (리커시브)
//        _pszPath => "c:\\aa\\*.*"
//        _pvecFileBaseInfo or _pvecFileDetailInfo 둘중에 하나만 사용하면 된다
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cFileManager::getFileList(const TCHAR *_pszPath, vector<stFILEBASEINFO> *_pvecFileBaseInfo, vector<stFILEDETAILINFO> *_pvecFileDetailInfo)
{
	// 현재 경로, 경로에서 "*.*" 제거
	TCHAR szCurPath[512];
	_tcsncpy_s(szCurPath, 512, _pszPath, 512 - 1);
	szCurPath[_tcslen(szCurPath) - 3] = NULL;

	stFILEBASEINFO stFileBaseInfo;
	stFILEDETAILINFO stFileDetailInfo;
	TCHAR szNextPath[512];
	WIN32_FIND_DATA w32fd;	
	int iFileNameSize = sizeof(stFileBaseInfo.szFileName) / sizeof(TCHAR);
	int iDirectorySize = sizeof(stFileBaseInfo.szDirectory) / sizeof(TCHAR);
	HANDLE hFindFile = FindFirstFile(_pszPath, &w32fd);
	if(hFindFile != INVALID_HANDLE_VALUE){
		while(1){
			if(w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {		// 디렉토리
				if(_tcscmp(w32fd.cFileName, _T("."))  &&  _tcscmp(w32fd.cFileName, _T(".."))){					
					_stprintf_s(szNextPath, 512, _T("%s%s\\*.*"), szCurPath, w32fd.cFileName);
					getFileList(szNextPath, _pvecFileBaseInfo, _pvecFileDetailInfo);
				}				
			}else{														// 파일								
				if(_pvecFileBaseInfo){
					stFileBaseInfo.dwFileSize = w32fd.nFileSizeLow;
					_tcsncpy_s(stFileBaseInfo.szFileName, iFileNameSize, w32fd.cFileName, iFileNameSize - 1);
					_tcsncpy_s(stFileBaseInfo.szDirectory, iDirectorySize, szCurPath, iDirectorySize - 1);
					_pvecFileBaseInfo->push_back(stFileBaseInfo);
				}else if(_pvecFileDetailInfo){
					stFileDetailInfo.stBaseInfo.dwFileSize = w32fd.nFileSizeLow;
					_tcsncpy_s(stFileDetailInfo.stBaseInfo.szFileName, iFileNameSize, w32fd.cFileName, iFileNameSize - 1);
					_tcsncpy_s(stFileDetailInfo.stBaseInfo.szDirectory, iDirectorySize, szCurPath, iDirectorySize - 1);					
					FileTimeToSystemTime(&w32fd.ftCreationTime, &stFileDetailInfo.systimeCreation);
					FileTimeToSystemTime(&w32fd.ftLastAccessTime, &stFileDetailInfo.systimeLastAccess);
					FileTimeToSystemTime(&w32fd.ftLastWriteTime, &stFileDetailInfo.systimeLastWrite);
					_pvecFileDetailInfo->push_back(stFileDetailInfo);
				}
			}
			
			if(FindNextFile(hFindFile, &w32fd) == FALSE){
				break;
			}
		}
	}

	FindClose(hFindFile);
}




// ===============================================================================================================================
// ========================================================= cLog ================================================================
// ===============================================================================================================================

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cLog::cLog()
{	
	m_bActive = FALSE;
	m_bUseCriticalSection = FALSE;		
	m_pCS = NULL;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cLog::~cLog()
{
	closeFile();
	if(m_pCS){
		DeleteCriticalSection(m_pCS);
		delete m_pCS;
		m_pCS = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 조금이라도 부하를 줄이기위해 cCriticalSection 클래스를 사용안함 (항상 CS 사용시 주의)
//       날짜가 변경될때 closeFile(), openFile() 가 연달아 실행이 되므로 내부에서 CS 를 해주면 안된다, 날짜가 변경될때에 CS 처리
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cLog::closeFile()
{
	if(m_bActive){
		SYSTEMTIME curTime;
		GetLocalTime(&curTime);
		TCHAR szTemp[512];
		_stprintf_s(szTemp, _countof(szTemp), _T("== END %d-%02d-%02d %02d:%02d:%02d ==\r\n"), 
				curTime.wYear, curTime.wMonth, curTime.wDay, 
				curTime.wHour, curTime.wMinute, curTime.wSecond);	

		// 유니코드일때 파일작업이 약간 복잡해서 그냥 Ascii 로 작성, #ifdef 을 처리안해되는데 조금이라도 작업량을 줄이기위해
		DWORD dwLen;
		#if defined _UNICODE  ||  defined UNICODE
			// Unicode => Multi
			char szMulti[sizeof(szTemp)];
			int iRe = cUnicode::W2T2(szTemp, szMulti, sizeof(szMulti));
			if(!iRe){
				TCHAR szTemp[256];
				_stprintf_s(szTemp, _countof(szTemp), 
						_T("Error! cUnicode::W2T2(), Function: %s, Line: %d"), _T("cLog::~cLog()"), __LINE__);
				cUnicode::print(eCONSOLECOLOR_ERROR, szTemp);			
				return;
			}
			dwLen = m_cFileManager.writeFile(szMulti, (int)strlen(szMulti));
		#else
			dwLen = m_cFileManager.writeFile(szTemp, (int)strlen(szTemp));			
		#endif
				
		m_cFileManager.closeFile();
		m_bActive = FALSE;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 조금이라도 부하를 줄이기위해 cCriticalSection 클래스를 사용안함 (항상 CS 사용시 주의)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cLog::writeLog(const TCHAR *_pszMessage, BOOL _bWriteDate)
{
	// 파일을 해당 날짜별로 생성하므로 fileBackup() 를 사용하지 않는다
	//DWORD iFileSize = m_cFileManager.getFileSize();
	//if(iFileSize > eMAXFILESIZE){	// 파일크기가 1Mb 가넘으면 백업파일을 만든다
	//	if(!fileBackup()){
	//		return FALSE;
	//	}
	//}

	// 프로그램이 실행중에 날짜가 바뀔수 있으므로 여기서 확인
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	if(!(m_systimeCurDate.wYear == curTime.wYear  &&  m_systimeCurDate.wMonth == curTime.wMonth  &&  m_systimeCurDate.wDay == curTime.wDay)){
		if(m_bUseCriticalSection){
			EnterCriticalSection(m_pCS);
		}
		closeFile();
		openFile(m_szOriginalFileName, m_bUseCriticalSection);
		if(m_bUseCriticalSection){
			LeaveCriticalSection(m_pCS);
		}
	}

	TCHAR szTemp[eMAXLOGLENGTH];		
	if(_tcslen(_pszMessage) >= eMAXLOGLENGTH - 1){
		TCHAR szTemp2[eMAXLOGLENGTH];
		memcpy(szTemp2, _pszMessage, eMAXLOGLENGTH);			
		szTemp2[eMAXLOGLENGTH - 54] = '.';
		szTemp2[eMAXLOGLENGTH - 53] = '.';
		szTemp2[eMAXLOGLENGTH - 52] = '.';
		szTemp2[eMAXLOGLENGTH - 51] = '\0';
		if(_bWriteDate){
			_stprintf_s(szTemp, _countof(szTemp), _T("%d-%02d-%02d %02d:%02d:%02d ==> %s\r\n"), 
					curTime.wYear, curTime.wMonth, curTime.wDay, 
					curTime.wHour, curTime.wMinute, curTime.wSecond, szTemp2);	
		}else{
			_stprintf_s(szTemp, _countof(szTemp), _T("%s\r\n"), szTemp2);
		}
	}else{
		if(_bWriteDate){
			_stprintf_s(szTemp, _countof(szTemp), _T("%d-%02d-%02d %02d:%02d:%02d ==> %s\r\n"), 
					curTime.wYear, curTime.wMonth, curTime.wDay, 
					curTime.wHour, curTime.wMinute, curTime.wSecond, _pszMessage);	
		}else{
			_stprintf_s(szTemp, _countof(szTemp), _T("%s\r\n"), _pszMessage);
		}
	}	

	// 유니코드일때 파일작업이 약간 복잡해서 그냥 Ascii 로 작성, #ifdef 을 처리안해되는데 조금이라도 작업량을 줄이기위해
	DWORD dwLen;
	#if defined _UNICODE  ||  defined UNICODE
		// Unicode => Multi
		char szMulti[sizeof(szTemp)];
		int iRe = cUnicode::W2T2(szTemp, szMulti, sizeof(szMulti));
		if(!iRe){
			TCHAR szTemp[256];
			_stprintf_s(szTemp, _countof(szTemp), 
					_T("Error! cUnicode::W2T2(), Function: %s, Line: %d"), _T("cLog::writeLog()"), __LINE__);
			cUnicode::print(eCONSOLECOLOR_ERROR, szTemp);
			return FALSE;
		}
		if(m_bUseCriticalSection){
			EnterCriticalSection(m_pCS);
		}
		dwLen = m_cFileManager.writeFile(szMulti, (int)strlen(szMulti));
		if(m_bUseCriticalSection){
			LeaveCriticalSection(m_pCS);
		}
	#else
		if(m_bUseCriticalSection){
			EnterCriticalSection(m_pCS);
		}
		dwLen = m_cFileManager.writeFile(szTemp, (int)strlen(szTemp));		
		if(m_bUseCriticalSection){
			LeaveCriticalSection(m_pCS);
		}
	#endif
	
	if(dwLen != -1){
		return TRUE;
	}else{
		return FALSE;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 날짜가 변경될때 closeFile(), openFile() 가 연달아 실행이 되므로 내부에서 CS 를 해주면 안된다, 
//       날짜가 변경될때에 CS 처리
//       _pszFileName 는 순수한 파일명 ("test.txt")
//       내부적으로 절대경로로 변환한다
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cLog::openFile(const TCHAR *_pszFileName, BOOL _bUseCriticalSection)
{
	m_bUseCriticalSection = _bUseCriticalSection;
	if(m_bUseCriticalSection){
		if(!m_pCS){
			m_pCS = new CRITICAL_SECTION;
			InitializeCriticalSection(m_pCS);
		}
	}

	// 자신 프로그램의 경로 
	// 상대경로로 처리하면 나중에 경로가 꼬이는 경우가 있다, GetCurrentDirectory() 로 확인해보면 경로가 바뀌어져있다
	// 파일의 경로작업을 할때는 될수 있으면 절대경로를 사용
	TCHAR szTotalCurExeFileName[512];	// 현재의 프로그램 총경로
	TCHAR szTotalPath[512];				// 파일명을 제외한 경로 ("\\" 포함)
	GetModuleFileName(NULL, szTotalCurExeFileName, _countof(szTotalCurExeFileName));	
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	_tsplitpath_s(szTotalCurExeFileName, drive, dir, fname, ext);
	_stprintf_s(szTotalPath, _countof(szTotalPath), _T("%s%s"), drive, dir);	

	_tcscpy_s(m_szOriginalFileName, _countof(m_szOriginalFileName), _pszFileName);
	GetLocalTime(&m_systimeCurDate);
	TCHAR szTemp[512];

	// 해당 년, 월 을 가지고 폴더를 생성
	TCHAR szTodayFolderName[512];	
	_stprintf_s(szTodayFolderName, _countof(szTodayFolderName), _T("%s%s%04d_%02d"), 
			szTotalPath, dLOGFILEFOLDER, m_systimeCurDate.wYear, m_systimeCurDate.wMonth);		
	cFileManager::createDirectory(szTodayFolderName);

	TCHAR szRealFileName[512];
	_stprintf_s(szRealFileName, _countof(szRealFileName), _T("%02d_%s"), 
			m_systimeCurDate.wDay, _pszFileName);
	_stprintf_s(m_szFileTotalName, _countof(m_szFileTotalName), _T("%s\\%s"), szTodayFolderName, szRealFileName);
	
	if(!m_cFileManager.createFile(m_szFileTotalName, GENERIC_WRITE, OPEN_ALWAYS)){
		// 같은 파일명이 CreateFile() 이 되어있을지 모르니 파일명을 변경해서 다시 시도	
		BOOL bSuccess = FALSE;
		TCHAR szFileName[256];
		TCHAR szExt[40];
		_tsplitpath_s(m_szFileTotalName, NULL, 0, NULL, 0, szFileName, _countof(szFileName), szExt, _countof(szExt));
		for(int i=0; i<30; i++){
			_stprintf_s(m_szFileTotalName, _countof(m_szFileTotalName), _T("%s\\%s(%d)%s"), szTodayFolderName, szFileName, i + 1, szExt);	
			if(m_cFileManager.createFile(m_szFileTotalName, GENERIC_WRITE, OPEN_ALWAYS)){		// 성공일때
				bSuccess = TRUE;
				break;	
			}			
		}	
		if(!bSuccess){
			return FALSE;
		}
	}
	m_cFileManager.moveFilePoint(FILE_END);

	_stprintf_s(szTemp, _countof(szTemp), _T("\r\n\r\n== START %d-%02d-%02d %02d:%02d:%02d ==\r\n"),
			m_systimeCurDate.wYear, m_systimeCurDate.wMonth, m_systimeCurDate.wDay, 
			m_systimeCurDate.wHour, m_systimeCurDate.wMinute, m_systimeCurDate.wSecond);	

	// 유니코드일때 파일작업이 약간 복잡해서 그냥 Ascii 로 작성, #ifdef 을 처리안해되는데 조금이라도 작업량을 줄이기위해
	DWORD dwLen;
	#if defined _UNICODE  ||  defined UNICODE
		// Unicode => Multi
		char szMulti2[sizeof(szTemp)];
		int iRe = cUnicode::W2T2(szTemp, szMulti2, sizeof(szMulti2));
		if(!iRe){
			TCHAR szTemp[256];
			_stprintf_s(szTemp, _countof(szTemp), 
					_T("Error! cUnicode::W2T2(), Function: %s, Line: %d"), _T("cLog::openFile()"), __LINE__);
			cUnicode::print(eCONSOLECOLOR_ERROR, szTemp);			
			return FALSE;
		}		
		dwLen = m_cFileManager.writeFile(szMulti2, (int)strlen(szMulti2));
	#else
		dwLen = m_cFileManager.writeFile(szTemp, (int)strlen(szTemp));		
	#endif
	
	m_bActive = TRUE;
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cLog::clearData()
{
	if(m_bActive){
		if(m_bUseCriticalSection){
			EnterCriticalSection(m_pCS);
		}

		// 다른 방법이 없어서 파일을 닫고 제거한후 다시 연다
		m_cFileManager.closeFile();
		cFileManager::operationFile(FO_DELETE, m_szFileTotalName, NULL);
		openFile(m_szOriginalFileName, m_bUseCriticalSection);		

		if(m_bUseCriticalSection){
			LeaveCriticalSection(m_pCS);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cLog::fileBackup()
{
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	if(m_bUseCriticalSection){
		EnterCriticalSection(m_pCS);
	}

	m_cFileManager.closeFile();
	TCHAR szRename[512];	

	TCHAR *pDot = _tcsrchr(m_szFileTotalName, _T('.'));
	++pDot;

	int iNameSize = (int)_tcslen(m_szFileTotalName);		
	_tcscpy_s(szRename, _countof(szRename), m_szFileTotalName);	
	szRename[iNameSize - _tcslen(pDot) - 1] = 0;		// .txt
	TCHAR szTemp[40];
	_stprintf_s(szTemp, _countof(szTemp), _T("_OLD_%d-%02d-%02d(%02d-%02d-%02d).%s"), 
			curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond, pDot);	
	_tcscat_s(szRename, _countof(szRename), szTemp);

	// Unicode => Multi
	char szMulti[sizeof(m_szFileTotalName)];
	int iRe = cUnicode::W2T2(m_szFileTotalName, szMulti, sizeof(szMulti));
	if(!iRe){
		if(m_bUseCriticalSection){
			LeaveCriticalSection(m_pCS);
		}
		TCHAR szTemp[256];
		_stprintf_s(szTemp, _countof(szTemp), 
				_T("Error! cUnicode::W2T2(), Function: %s, Line: %d"), _T("cLog::fileBackup()"), __LINE__);
		cUnicode::print(eCONSOLECOLOR_ERROR, szTemp);
		return FALSE;
	}

	// Unicode => Multi
	char szMulti2[sizeof(szRename)];
	iRe = cUnicode::W2T2(szRename, szMulti2, sizeof(szMulti2));
	if(!iRe){
		if(m_bUseCriticalSection){
			LeaveCriticalSection(m_pCS);
		}
		TCHAR szTemp[256];
		_stprintf_s(szTemp, _countof(szTemp), 
				_T("Error! cUnicode::W2T2(), Function: %s, Line: %d"), _T("cLog::fileBackup()"), __LINE__);
		cUnicode::print(eCONSOLECOLOR_ERROR, szTemp);			
		return FALSE;
	}
	rename(szMulti, szMulti2);
	
	// 원래 파일이름 찾기
	int i = iNameSize;
	while(1){			
		if(m_szFileTotalName[i - 1] == '\\'){
			break;
		}
		i--;
	}
	TCHAR szOriFileName[256];
	_tcscpy_s(szOriFileName, _countof(szOriFileName), m_szFileTotalName + i);	
	openFile(szOriFileName, m_bUseCriticalSection);		
	if(m_bUseCriticalSection){
		LeaveCriticalSection(m_pCS);
	}
	return TRUE;
}



// ===============================================================================================================================
// ========================================================= cThread =============================================================
// ===============================================================================================================================

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cThread::cThread()
{
	m_phThread = NULL;	
	m_pbThreadWhile = NULL;
	m_pcParentPrt = NULL;
	m_iSleepMillisecond = 0;
	m_szCaption[0] = NULL;
	pThreadMainProcPtr = NULL;
	pThreadEndProcPtr = NULL;
	m_iThreadCnt = 0;
	m_bEndThread = FALSE;
	m_iCurThreadCnt = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cThread::~cThread()
{
	endThread();	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cThread::clearMem()
{
	if(m_phThread){
		delete []m_phThread;
		m_phThread = NULL;
	}
	if(m_pbThreadWhile){
		delete []m_pbThreadWhile;
		m_pbThreadWhile = NULL;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cThread::startThread(void *_pThis, int _iSleepMillisecond, int _iThreadCnt,
							DWORD (*_pThreadMainProcPtr)(void *_pThis, int _iThreadIdx), 
							void (*_pThreadEndProcPtr)(void *_pThis, int _iThreadIdx, DWORD _dwReturnValue),
							TCHAR *_pszCaption)
{
	if(m_phThread){
		return FALSE;
	}	
	if(_iThreadCnt <= 0){
		return FALSE;
	}

	m_bEndThread = FALSE;
	m_iSleepMillisecond = _iSleepMillisecond;
	m_iThreadCnt = _iThreadCnt;
	pThreadMainProcPtr = _pThreadMainProcPtr;
	pThreadEndProcPtr = _pThreadEndProcPtr;
	m_pcParentPrt = _pThis;			// cThread 를 사용하는 부모 클래스의 포인터
	m_iCurThreadCnt = 0;

	if(_pszCaption){
		_tcscpy_s(m_szCaption, _countof(m_szCaption), _pszCaption);
	}

	m_phThread = new HANDLE[m_iThreadCnt];
	m_pbThreadWhile = new BOOL[m_iThreadCnt];	
	UINT uiThreadID = 0;	
	for(int i=0; i<m_iThreadCnt; i++){
		stTHREADPARAM *pstParam;
		pstParam = new stTHREADPARAM;
		pstParam->iIdx = i;
		pstParam->pData = this;

		m_pbThreadWhile[i] = TRUE;
		m_phThread[i] = NULL;
		m_phThread[i] = (HANDLE)_beginthreadex((LPSECURITY_ATTRIBUTES)NULL, (DWORD)0, threadMain, (LPVOID)pstParam,
				CREATE_SUSPENDED, &uiThreadID);
		
		// 실패라면 이전에 성공한 핸들을 전부 정리해준다
		if(!m_phThread[i]){
			for(int k=0; k<i; k++){
				CloseHandle(m_phThread[k]);
			}
			delete []m_phThread;
			m_phThread = NULL;
			return FALSE;
		}
	}

	// Thread 시작
	for(int i=0; i<m_iThreadCnt; i++){
		resumeThread(i);					
	}

	TCHAR szPrint[512];
	if(m_iThreadCnt == 1){
		_stprintf_s(szPrint, _countof(szPrint), _T("== %s Thread Start =="), m_szCaption);
		cUnicode::print(szPrint);			
	}else{
		for(int i=0; i<m_iThreadCnt; i++){
			_stprintf_s(szPrint, _countof(szPrint), _T("== %s_%d Thread Start =="), m_szCaption, i);
			cUnicode::print(szPrint);			
		}
	}	

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cThread::endThread()
{
	if(!m_phThread){
		return;
	}

	m_bEndThread = TRUE;
	TCHAR szPrint[512];
	for(int i=0; i<m_iThreadCnt; i++){
		m_pbThreadWhile[i] = FALSE;
		resumeThread(i);								// 혹시 중지되있을수도 있으므로	
		WaitForSingleObject(m_phThread[i], INFINITE);	// 쓰레드가 종료될때까지 대기

		if(m_iThreadCnt == 1){
			_stprintf_s(szPrint, _countof(szPrint), _T("== %s Thread End =="), m_szCaption);
		}else{
			_stprintf_s(szPrint, _countof(szPrint), _T("== %s_%d Thread End =="), m_szCaption, i);
		}
		cUnicode::print(szPrint);
	}	

	clearMem();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 쓰레드 강제 종료 (사용 비추천)
//        쓰레드에서 EnterCriticalSection() 을 처리하고 죽어버리면 데드락 걸린다 (주의)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cThread::forcedEndThread()
{
	if(!m_phThread){
		return;
	}

	m_bEndThread = TRUE;
	TCHAR szPrint[512];
	for(int i=0; i<m_iThreadCnt; i++){
		m_pbThreadWhile[i] = FALSE;
		TerminateThread(m_phThread[i], 0);
		m_phThread[i] = NULL;	

		if(m_iThreadCnt == 1){
			_stprintf_s(szPrint, _countof(szPrint), _T("== %s Forced Thread End =="), m_szCaption);
		}else{
			_stprintf_s(szPrint, _countof(szPrint), _T("== %s_%d Forced Thread End =="), m_szCaption, i);
		}
		cUnicode::print(szPrint);
	}	

	clearMem();	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT __stdcall cThread::threadMain(LPVOID _lpData)
{
	stTHREADPARAM *pstParam = (stTHREADPARAM *)_lpData;

	int iThreadIdx = pstParam->iIdx;
	cThread *pThis = (cThread *)pstParam->pData;

	// 쓰레드 호출시 동적생성했으므로 여기서 delete
	delete pstParam;
	
	// cThread 를 사용하는 클래스의 포인터를 다시 보낸다(static 함수이므로 자신의 멤버를 사용하지 못하므로)
	// 쓰레드 메인	
	DWORD dwRe = pThis->pThreadMainProcPtr(pThis->m_pcParentPrt, iThreadIdx);
	pThis->m_pbThreadWhile[iThreadIdx] = FALSE;

	// 쓰레드 종료시 호출
	if(pThis->pThreadEndProcPtr){
		pThis->pThreadEndProcPtr(pThis->m_pcParentPrt, iThreadIdx, dwRe);
	}

	// endThread() 에 의해 쓰레드가 죽을때 endThread() 의 WaitForSingleObject() 가 호출된다
	CloseHandle(pThis->m_phThread[iThreadIdx]);	
	pThis->m_phThread[iThreadIdx] = NULL;

	// endThread() 에 의한 종료가 아닐경우 처리
	if(!pThis->m_bEndThread){
		TCHAR szPrint[512];
		if(pThis->m_iThreadCnt == 1){
			_stprintf_s(szPrint, _countof(szPrint), _T("== %s Thread End =="), pThis->m_szCaption);
		}else{
			_stprintf_s(szPrint, _countof(szPrint), _T("== %s_%d Thread End =="), pThis->m_szCaption, iThreadIdx);
		}
		cUnicode::print(szPrint);
		
		// 가장마지막 쓰레드에서 처리		
		if(pThis->m_iThreadCnt == ++pThis->m_iCurThreadCnt){
			pThis->clearMem();
		}
	}	

	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 쓰레드 중지
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cThread::suspendThread(int _iThreadIdx)
{
	if(!m_phThread){
		return;
	}	
	if(_iThreadIdx >= m_iThreadCnt){
		return;
	}

	SuspendThread(m_phThread[_iThreadIdx]);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 쓰레드 재시작
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cThread::resumeThread(int _iThreadIdx)
{
	if(!m_phThread){
		return;
	}	
	if(_iThreadIdx >= m_iThreadCnt){
		return;
	}

	ResumeThread(m_phThread[_iThreadIdx]);
}

