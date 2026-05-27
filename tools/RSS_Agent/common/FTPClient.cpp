#include "stdafx.h"
#include "FTPClient.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <shellapi.h>

#pragma warning( push )
#pragma warning( disable : 4311)		// '형식 캐스팅' : 'cFTPClient *const '에서 'DWORD'(으)로 포인터가 잘립니다.


//////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
//////////////////////////////////////////////////////////////////////////////////////////////////
cFTPClient::cFTPClient()
{
	m_hInternet = NULL;
	m_hFTP = NULL;
	m_dwCurFileCalcByte = 0;
	m_dwTotalFileCalcByte = 0;
	m_bThreadBlock = FALSE;
	m_bUpload = TRUE;

// 	m_cLog.openFile(_T("FTP_LOGFILE.txt"), FALSE);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
//////////////////////////////////////////////////////////////////////////////////////////////////
cFTPClient::~cFTPClient()
{
	disconnect();
}



//////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 접속
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFTPClient::connect(const TCHAR *_pszServer, int _iPort, const TCHAR *_pszID, const TCHAR *_pszPW)
{
	if(m_hFTP){
		return FALSE;		
	}

	m_hInternet = InternetOpen(_T("FTPServer"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!m_hInternet){
		return FALSE;
	}

	m_hFTP = InternetConnect(m_hInternet, _pszServer, _iPort, _pszID, _pszPW, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if(!m_hFTP){
		InternetCloseHandle(m_hInternet);
		return FALSE;
	}	

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : FTP 경로변경
//        경로는 무조건 "\\" 이것을 사용한다
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFTPClient::setDirectory(const TCHAR *_pszFTPDirectory, BOOL _bCreateDirectory)
{
	// 현재의 디렉토리를 초기화
	TCHAR szFTPDirectory[MAX_PATH];
	DWORD dwLen = MAX_PATH;
	FtpGetCurrentDirectory(m_hFTP, szFTPDirectory, &dwLen);

	if(_tcscmp(szFTPDirectory, _T("/")) != 0){			
		if(!FtpSetCurrentDirectory(m_hFTP, _T("/"))){
			DWORD dwError = GetLastError();
			errorHandling(dwError, _T("FtpSetCurrentDirectory(), Directory: %s"), _T("/"));
			return FALSE;				
		}
	}

	if(_tcscmp(_pszFTPDirectory, _T("/")) == 0  ||  _tcscmp(_pszFTPDirectory, _T("")) == 0){
		return TRUE;
	}	

	if(!FtpSetCurrentDirectory(m_hFTP, _pszFTPDirectory)){
		FtpGetCurrentDirectory(m_hFTP, szFTPDirectory, &dwLen);

		// 디렉토리가 존재하지 않을수 있으므로
		if(_bCreateDirectory){
			wstring strPath = _pszFTPDirectory;
			wstring strTemp;
			int iFind;
			int iLen;
			while(1){				
				iFind = (int)strPath.find(_T("\\"));
				if(iFind == -1){
					if(!FtpCreateDirectory(m_hFTP, strPath.c_str())){
						DWORD dwError = GetLastError();
						errorHandling(dwError, _T("FtpCreateDirectory(), Directory: %s"), _pszFTPDirectory);
						return FALSE;
					}	
					if(!FtpSetCurrentDirectory(m_hFTP, strPath.c_str())){
						DWORD dwError = GetLastError();
						errorHandling(dwError, _T("FtpSetCurrentDirectory(), Directory: %s"), _T("/"));
						return FALSE;				
					}
					break;
				}
				iLen = (int)strPath.length() - iFind;
				strTemp = strPath.substr(0, iFind);
				strPath = strPath.substr(iFind + 1, iLen - 1);
				if(!FtpCreateDirectory(m_hFTP, strTemp.c_str())){
					// 이미 존재하는 디렉토리일수 있다
					//DWORD dwError = GetLastError();
					//errorHandling(dwError, _T("FtpCreateDirectory(), Directory: %s"), _pszFTPDirectory);
					//return FALSE;
				}
				if(!FtpSetCurrentDirectory(m_hFTP, strTemp.c_str())){
					DWORD dwError = GetLastError();
					errorHandling(dwError, _T("FtpSetCurrentDirectory(), Directory: %s"), _T("/"));
					return FALSE;				
				}
			}			
		}else{
			DWORD dwError = GetLastError();
			errorHandling(dwError, _T("FtpSetCurrentDirectory(), Directory: %s"), _pszFTPDirectory);
			return FALSE;
		}		
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 파일다운
//        경로는 무조건 "\\" 이것을 사용한다
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFTPClient::downloadFile(const TCHAR *_pszFTPDirectory, const TCHAR *_pszFileName, const TCHAR *_pszLocalDirectory,const TCHAR *_lpstrDestFile)
{	
	TCHAR szFTPDirectory[MAX_PATH];
	TCHAR szLocalDirectory[MAX_PATH];
	_tcscpy_s(szFTPDirectory, _countof(szFTPDirectory), _pszFTPDirectory);	
	_tcscpy_s(szLocalDirectory, _countof(szLocalDirectory), _pszLocalDirectory);
	checkDerectory(szFTPDirectory);	
	checkDerectory(szLocalDirectory);	

	if(!setDirectory(szFTPDirectory, FALSE)){
		return FALSE;
	}

	TCHAR szNewFile[512];

	if(_tcscmp(szFTPDirectory, _T("")) != 0)
	{
		_stprintf_s(szNewFile, _countof(szNewFile), _T("%s\\%s\\%s"), szLocalDirectory, szFTPDirectory, _pszFileName);
	}else{
		_stprintf_s(szNewFile, _countof(szNewFile), _T("%s\\%s"), szLocalDirectory, _pszFileName);
	}

	// FtpGetFile() 함수를 사용하면 받은 양을 알수 없으므로 다른방법으로 처리
	// return FtpGetFile(m_hFTP, _pszFileName, szNewFile, FALSE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0);
	
	DWORD dwBytesRead = 0;
	const DWORD dwBufferSize = 2048;
	char szBuffer[dwBufferSize];		// Ascii	
	m_dwCurFileCalcByte = 0;	

	if(!openFile(_pszFileName, FALSE)){
		return FALSE;
	}

	// 로컬에 디렉토리 생성
	TCHAR szLocalTempDirectory[MAX_PATH];
	_tcsncpy_s(szLocalTempDirectory, _countof(szLocalTempDirectory), szNewFile, _tcslen(szNewFile) - _tcslen(_pszFileName) - 1);
	cFileManager::createDirectory(szLocalTempDirectory);

	// 로컬에 파일생성
	TCHAR szTempFile[512];
	_tcscpy_s(szTempFile, _countof(szTempFile), szNewFile);	// 임시로 다른이름으로 받은다음에 다받고 나서 이름을 원래 이름으로 수정
	_tcscat_s(szTempFile, _countof(szTempFile), _T(".tmp"));

	if	(_lpstrDestFile)
		_tcscpy_s(szTempFile, _lpstrDestFile);	// 임시로 다른이름으로 받은다음에 다받고 나서 이름을 원래 이름으로 수정

	cFileManager file;

	if(!file.createFile(szTempFile, GENERIC_WRITE, CREATE_ALWAYS)){
		if(m_hFTPFile){
			InternetCloseHandle(m_hFTPFile);
		}
		DWORD dwError = GetLastError();
		errorHandling(dwError, _T("cFileManager::createFile(), FileName: %s"), szTempFile);
		return FALSE;			
	}

	////////////////////////////////////////////////////////////
	// 파일을 받으면서 로컬 파일에 쓰기
	do{
		if(!InternetReadFile(m_hFTPFile, szBuffer, dwBufferSize, &dwBytesRead)){
			if(m_hFTPFile){
				InternetCloseHandle(m_hFTPFile);
			}
			file.closeFile();
			DWORD dwError = GetLastError();
			errorHandling(dwError, _T("InternetReadFile(), FileName: %s"), szTempFile);
			return FALSE;			
		}
		m_dwCurFileCalcByte += dwBytesRead;  
		m_dwTotalFileCalcByte += dwBytesRead;
		file.writeFile(szBuffer, dwBytesRead);
	}while(dwBytesRead);
	// 파일을 받으면서 로컬 파일에 쓰기
	////////////////////////////////////////////////////////////	

	file.closeFile();

	// 임시파일명이므로 원래의 파일명으로 수정	
	if	(!_lpstrDestFile && !cFileManager::operationFile(FO_RENAME, szTempFile, szNewFile))
	{
		// 혹시 기존의 파일이 있을수 있으므로
		if(!cFileManager::operationFile(FO_DELETE, szNewFile, NULL)){
			DWORD dwError = GetLastError();
			errorHandling(dwError, _T("cFileManager::operationFile(FO_DELETE), FileName: %s"), szNewFile);
			return FALSE;			
		}
		if(!cFileManager::operationFile(FO_RENAME, szTempFile, szNewFile)){
			DWORD dwError = GetLastError();
			errorHandling(dwError, _T("cFileManager::operationFile(FO_RENAME), FileName: %s"), szNewFile);
			return FALSE;	
		}
	}

	if(m_hFTPFile){
		InternetCloseHandle(m_hFTPFile);
	}

	const	TCHAR	*lpstrDestFile	=	szNewFile;

	if	(_lpstrDestFile)
		lpstrDestFile	=	_lpstrDestFile;

	// download 한것이므로 생성날짜, 수정날짜 등을 수정한다
	HINTERNET hFind;
	WIN32_FIND_DATA wfd;
	hFind = FtpFindFirstFile(m_hFTP, _pszFileName, &wfd, 0, 0);
	if(hFind)
	{
		HANDLE hFindFile = CreateFile(lpstrDestFile, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFindFile != INVALID_HANDLE_VALUE){
			SetFileTime(hFindFile, NULL, NULL, &wfd.ftLastWriteTime);
			CloseHandle(hFindFile);
		}else{
			DWORD dwError = GetLastError();
			errorHandling(dwError, _T("CreateFile(), FileName: %s"), lpstrDestFile);
			return FALSE;
		}	
		InternetCloseHandle(hFind);
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 일단은 .tmp 파일로 upload 한후에 파일명을 변경한다
//        경로는 무조건 "\\" 이것을 사용한다
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFTPClient::uploadFile(const TCHAR *_pszFTPDirectory, const TCHAR *_pszFileFullName)
{
	TCHAR szFTPDirectory[MAX_PATH];
	_tcscpy_s(szFTPDirectory, _countof(szFTPDirectory), _pszFTPDirectory);	
	checkDerectory(szFTPDirectory);

	if(!setDirectory(szFTPDirectory, TRUE)){
		return FALSE;
	}

	// 구분하기
	TCHAR szExt[5];
	TCHAR szFileName[256];
	_tsplitpath_s(_pszFileFullName, NULL, 0, NULL, 0, szFileName, _countof(szFileName), szExt, _countof(szExt));
	_tcscat_s(szFileName, _countof(szFileName), szExt);

	// FtpGetFile() 함수를 사용하면 받은 양을 알수 없으므로 다른방법으로 처리
	// return FtpPutFile(m_hFTP, _pszFileFullName, szFileName, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0);

	DWORD dwBytesRead = 0;
	const DWORD dwBufferSize = 2048;
	char szBuffer[dwBufferSize];		// Ascii	
	DWORD dwResult;
	m_dwCurFileCalcByte = 0;	
	
	// 임시로 다른이름으로 올린다음에 다올린고 나서 이름을 원래 이름으로 수정
	TCHAR szTempFile[512];
	_tcscpy_s(szTempFile, _countof(szTempFile), szFileName);	

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// FTP 서버에서 파일명변경을 지원안하는 곳이 있어서 파일명을 변경하지 않고 그냥 올린다
	//_tcscat_s(szTempFile, _countof(szTempFile), _T(".tmp"));

	if(!openFile(szTempFile, TRUE)){
		return FALSE;
	}	

	// 파일열기
	cFileManager file;
	if(!file.createFile(_pszFileFullName, GENERIC_READ, OPEN_EXISTING)){
		if(m_hFTPFile){
			InternetCloseHandle(m_hFTPFile);
		}
		DWORD dwError = GetLastError();		
		errorHandling(dwError, _T("file.createFile(), FileName: %s"), _pszFileFullName);
		return FALSE;			
	}

	////////////////////////////////////////////////////////////
	// 파일을 읽으면서 FTP 에 파일쓰기
	while(1){
		dwBytesRead = file.readFile(szBuffer, dwBufferSize);
		if(dwBytesRead == 0){	// 완료
			break;
		}
		if (!InternetWriteFile(m_hFTPFile, szBuffer, dwBytesRead, &dwResult)){
			if(m_hFTPFile){
				InternetCloseHandle(m_hFTPFile);
			}
			file.closeFile();
			DWORD dwError = GetLastError();
			errorHandling(dwError, _T("InternetWriteFile(), FileName: %s"), szTempFile);
			return FALSE;			
		}
		m_dwCurFileCalcByte += dwResult;  
		m_dwTotalFileCalcByte += dwResult; 		
	}
	// 파일을 읽으면서 FTP 에 파일쓰기
	////////////////////////////////////////////////////////////	

	file.closeFile();

	if(m_hFTPFile){
		InternetCloseHandle(m_hFTPFile);
	}

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// FTP 서버에서 파일명변경을 지원안하는 곳이 있어서 파일명을 변경하지 않고 그냥 올린다
	/*
	// 임시파일에 다썼으므로 원래 파일로 이름수정		
	if(!FtpRenameFile(m_hFTP, szTempFile, szFileName)){
		// 같은 파일명의 파일이 있을수 있으므로, 해당 파일을 제거한후 다시 시도해본다
		if(FtpDeleteFile(m_hFTP, szFileName)){		// 지웠을경우
			if(!FtpRenameFile(m_hFTP, szTempFile, szFileName)){
				DWORD dwError = GetLastError();
				errorHandling(dwError, _T("FtpRenameFile(), FileName: %s"), szFileName);
				return FALSE;
			}
		}else{
			DWORD dwError = GetLastError();
			errorHandling(dwError, _T("FtpRenameFile()"));			
			return FALSE;
		}
	}
	*/

	// 현재 적용되지가 않는다 (코드를 수정해야한다), 2008-10-23
	/**
	// upload 한것이므로 생성날짜, 수정날짜 등을 수정한다
	HINTERNET hFind;
	WIN32_FIND_DATA wfd;
	hFind = FtpFindFirstFile(m_hFTP, szFileName, &wfd, 0, 0);
	if(hFind){
		WIN32_FIND_DATA wfdLocal;
		HANDLE hFindLocal = FindFirstFile(_pszFileFullName, &wfdLocal);		
		if(hFindLocal){
			int aa = SetFileTime(hFind, NULL, NULL, &wfdLocal.ftLastWriteTime);
			aa = GetLastError();
			FindClose(hFindLocal);
		}else{
			TCHAR szPrint[512];
			_stprintf_s(szPrint, _countof(szPrint), _T("Fail: Modify filetime"));
			g_cUnicode.print(szPrint, TRUE);
			m_cLog.writeLog(szPrint);
			// return FALSE;	// 날짜변경에 실패했다고해서 return FALSE 처리하지는 않는다
		}		
		InternetCloseHandle(hFind);
	}
	**/	

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFTPClient::getFileExist(const TCHAR *_pszFTPDirectory, const TCHAR *_pszFileName)
{
	TCHAR szFTPDirectory[MAX_PATH];
	_tcscpy_s(szFTPDirectory, _countof(szFTPDirectory), _pszFTPDirectory);	
	checkDerectory(szFTPDirectory);

	if(!setDirectory(szFTPDirectory, FALSE)){
		return FALSE;
	}

	WIN32_FIND_DATA wfd;
	HINTERNET hFind = FtpFindFirstFile(m_hFTP, _pszFileName, &wfd, 0, 0);
	if(hFind){
		InternetCloseHandle(hFind);
		return TRUE;
	}else{
		InternetCloseHandle(hFind);
		return FALSE;
	}	
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFTPClient::getFileSize(const TCHAR *_pszFTPDirectory, const TCHAR *_pszFileName, DWORD *_pdwSize)
{
	TCHAR szFTPDirectory[MAX_PATH];
	_tcscpy_s(szFTPDirectory, _countof(szFTPDirectory), _pszFTPDirectory);	
	checkDerectory(szFTPDirectory);

	*_pdwSize = 0;
	if(!setDirectory(szFTPDirectory, FALSE)){
		return FALSE;
	}

	WIN32_FIND_DATA wfd;
	HINTERNET hFind = FtpFindFirstFile(m_hFTP, _pszFileName, &wfd, 0, 0);
	if(hFind){
		*_pdwSize = wfd.nFileSizeLow;
		InternetCloseHandle(hFind);
		return TRUE;
	}else{
		InternetCloseHandle(hFind);
		return FALSE;
	}	
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFTPClient::getFileList(const TCHAR *_pszFTPDirectory, vector<stFILEINFO> *_pvecData)
{
	TCHAR szFTPDirectory[MAX_PATH];
	_tcscpy_s(szFTPDirectory, _countof(szFTPDirectory), _pszFTPDirectory);	
	checkDerectory(szFTPDirectory);

	if(!setDirectory(szFTPDirectory, FALSE)){
		return FALSE;
	}

	_pvecData->clear();

	stFILEINFO stFileInfo;
	WIN32_FIND_DATA wfd;
	BOOL bResult = TRUE;
	HINTERNET hFind;
	hFind = FtpFindFirstFile(m_hFTP, _T("*"), &wfd, 0, 0);
	if(hFind){
		stFileInfo.bFile = wfd.dwFileAttributes == 128 ? TRUE : FALSE;
		stFileInfo.dwSize = wfd.nFileSizeLow;
		_tcscpy_s(stFileInfo.szName, _countof(stFileInfo.szName), wfd.cFileName);
		stFileInfo.ftCreateTime = wfd.ftCreationTime;
		stFileInfo.ftLastAccessTime = wfd.ftLastAccessTime;
		stFileInfo.ftLastWriteTime = wfd.ftLastWriteTime;
		_pvecData->push_back(stFileInfo);
		while(bResult){
			bResult = InternetFindNextFile(hFind, &wfd);
			if(bResult){
				stFileInfo.bFile = wfd.dwFileAttributes == 128 ? TRUE : FALSE;
				stFileInfo.dwSize = wfd.nFileSizeLow;
				_tcscpy_s(stFileInfo.szName, _countof(stFileInfo.szName), wfd.cFileName);
				stFileInfo.ftCreateTime = wfd.ftCreationTime;
				stFileInfo.ftLastAccessTime = wfd.ftLastAccessTime;
				stFileInfo.ftLastWriteTime = wfd.ftLastWriteTime;
				_pvecData->push_back(stFileInfo);
			}
		}
		InternetCloseHandle(hFind);
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFTPClient::openFile(const TCHAR *_pszFileName, BOOL _bUpload)
{
	// FtpOpenFile() 함수가 방화벽에 의해 블럭 될수 있으므로 쓰레드에서 처리	
	m_bUpload = _bUpload;
	_tcscpy_s(m_szFileName, _countof(m_szFileName), _pszFileName);
	m_bThreadBlock = TRUE;
	m_cFTPOpenFileThread.startThread(this, 1, 1, FTPOpenFileThreadProc, NULL, _T("FTPOpenFileThreadProc"));

	int iErrorCheck = 0;
	while(m_bThreadBlock){			// m_cFTPOpenFileThread 쓰레드가 return 되면 FALSE
		if(++iErrorCheck > 30){		// 3초안에 반응이 없다면 방화벽에 막혀있으므로 강제로 쓰레드를 죽인다
			m_cFTPOpenFileThread.forcedEndThread();
			DWORD dwError = GetLastError();
			errorHandling(dwError, _T("FtpOpenFile(), FileName: %s\n\nIn compliance with the fire-wall it will not be able to connect in server!\nCancel a fire-wall set and again execute!"), _pszFileName);		
			return FALSE;			
		}
		Sleep(100);
	}

	if(!m_hFTPFile){
		DWORD dwError = GetLastError();
		errorHandling(dwError, _T("openFile(), FileName: %s"), _pszFileName);		
		return FALSE;			
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 마지막의 "\\" 를 제거한다
//////////////////////////////////////////////////////////////////////////////////////////////////
void cFTPClient::checkDerectory(TCHAR *_pszFTPDirectory)
{
	if(_tcscmp(_pszFTPDirectory, _T("")) != 0){
		if(_pszFTPDirectory[_tcslen(_pszFTPDirectory) - 1] == '\\'){
			_pszFTPDirectory[_tcslen(_pszFTPDirectory) - 1] = NULL;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
//////////////////////////////////////////////////////////////////////////////////////////////////
void cFTPClient::disconnect()
{
	if(m_hFTP){
		InternetCloseHandle(m_hFTP);

		m_hFTP	=	NULL;
	}
	if(m_hInternet){
		InternetCloseHandle(m_hInternet);

		m_hInternet	=	NULL;
	}

	m_dwCurFileCalcByte = 0;
	m_dwTotalFileCalcByte = 0;
	m_bThreadBlock = FALSE;
	m_bUpload = TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : static 함수(cThread Class 에 의해 사용됨)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD cFTPClient::FTPOpenFileThreadProc(void *_pcPtr, int _iThreadIdx)
{
	cFTPClient *pData = (cFTPClient *)_pcPtr;	

	// GENERIC_ALL 적용이 안되어서 따로 구분
	if(pData->m_bUpload){
		pData->m_hFTPFile = FtpOpenFile(pData->m_hFTP, pData->m_szFileName, GENERIC_WRITE, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, NULL);
	}else{
		pData->m_hFTPFile = FtpOpenFile(pData->m_hFTP, pData->m_szFileName, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, NULL);
	}
	pData->m_bThreadBlock = FALSE;
	int	iError	=	GetLastError();

	DWORD	dwError,dwSize=256;
	TCHAR	strBuffer[256];
	InternetGetLastResponseInfo(&dwError,strBuffer,&dwSize);


	return 1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFTPClient::deleteFile(const TCHAR *_pszFTPDirectory, const TCHAR *_pszFileName)
{
	TCHAR szFTPDirectory[MAX_PATH];
	_tcscpy_s(szFTPDirectory, _countof(szFTPDirectory), _pszFTPDirectory);	
	checkDerectory(szFTPDirectory);

	if(!setDirectory(szFTPDirectory, FALSE)){
		return FALSE;
	}

	if(!FtpDeleteFile(m_hFTP, _pszFileName)){
		DWORD dwError = GetLastError();
		errorHandling(dwError, _T("FtpDeleteFile()"));		
		return FALSE;
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cFTPClient::renameFile(const TCHAR *_pszFTPDirectory, const TCHAR *_pszOldFileName, const TCHAR *_pszNewFileName)
{
	TCHAR szFTPDirectory[MAX_PATH];
	_tcscpy_s(szFTPDirectory, _countof(szFTPDirectory), _pszFTPDirectory);	
	checkDerectory(szFTPDirectory);

	if(!setDirectory(szFTPDirectory, FALSE)){
		return FALSE;
	}

	if(!FtpRenameFile(m_hFTP, _pszOldFileName, _pszNewFileName)){
		DWORD dwError = GetLastError();
		errorHandling(dwError, _T("FtpRenameFile()"));
		return FALSE;
	}
	return TRUE;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cFTPClient::errorHandling(DWORD _dwErrorCode, const TCHAR *_pszMessage, ...)
{
	TCHAR szPrint[1024];
	TCHAR szTemp[1024];
	va_list va;
	va_start(va, _pszMessage);
	_vstprintf_s(szTemp, _countof(szTemp), _pszMessage, va);
	va_end(va);	

	TCHAR szBuffer[512];
	DWORD dwLen = (DWORD)_countof(szBuffer);
	InternetGetLastResponseInfo(&_dwErrorCode, szBuffer, &dwLen);

	_stprintf_s(szPrint, _countof(szPrint), _T("Error!, %s\n%s"), szTemp, szBuffer);

	cUnicode::print(szPrint, eCONSOLECOLOR_ERROR);
// 	m_cLog.writeLog(szPrint);
}

#pragma warning( pop )